/* author: snikons */

#include "modbus.h"
#include "modbus_private.h"

/* hex api */
#include "hex.h"

/* logging api */
#include "log.h"

/* glib */
#include <glib.h>
#include <gmodule.h>

/* assert() */
#include <assert.h>

/* pcap_lib_version(), pcap_open_offline(), pcap_loop(), pcap_geterr() */
#include <pcap/pcap.h>

/* struct ether_header, ETHERTYPE_IP */
#include <netinet/if_ether.h>

/* struct ip */
#include <netinet/ip.h>

/* IPPROTO_TCP */
#include <netinet/in.h>

/* struct tcphdr */
#include <netinet/tcp.h>

/* ntohs() */
#include <arpa/inet.h>

/* uint16_t */
#include <stdint.h>

/* standard error codes */
#include <errno.h>

void log_mb_packet(guint8 *bytes, gsize len) {
  if (logging_enabled()) { /* debug log */
    gchar *hex = bytes_to_hex(bytes, len, NULL);
    LOG_MSG("MODBUS response:\n%s", (hex ? hex : "NULL"));
    g_free(hex);
  }
}

void log_mb_header(const mb_header *mbh) {
  if (logging_enabled()) { /* debug log */
    LOG_MSG("MODBUS header:\ntid = %u, pid = %u, len = %u, uid = %u, "
            "func_code = %u",
            ntohs(mbh->tid), ntohs(mbh->pid), ntohs(mbh->len), mbh->uid,
            mbh->func_code);
  }
}

void mb_device_info_finder(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes) {
  shared_info *si;

  if (!h || !bytes) {
    LOG_ERROR("invalid param(s)");
    if (si && si->fd) {
      pcap_breakloop(si->fd);
      return;
    }
  }

  si = (shared_info *)user;
  assert(si->device_info != NULL);
  ++(si->packet_id);

  /* stop if error or device info found */
  if (
    mb_find_device_info_in_packet((guint8 *)bytes, h->caplen, si->device_info) ||
    (
      si->device_info->vendor_name.data &&
      si->device_info->product_code.data &&
      si->device_info->product_version.data
    )
  ) {
    LOG_MSG("sdevice info found in packet #%u", si->packet_id);
    pcap_breakloop(si->fd);
    return;
  }

  /* si->device_info->server_id is a fallback, so continue looking for the info */
  if (si->device_info->server_id.data) {
    gchar *hex = bytes_to_hex(si->device_info->server_id.data, si->device_info->server_id.len, NULL);
    LOG_MSG("server id found in packet #%u", si->packet_id);
    LOG_MSG("%s", hex);
    g_free(hex);
  }
}

int mb_find_device_info_in_pcap(gchar *pcap_path, mb_device_info *device_info) {
  char errbuf[PCAP_ERRBUF_SIZE];
  pcap_t *fd = NULL;
  shared_info si;

  if (!pcap_path || !device_info) {
    LOG_ERROR("invalid param(s)");
    return 1;
  }

  /* open pcap file */
  fd = pcap_open_offline(pcap_path, errbuf);
  if (!fd) {
    LOG_ERROR("pcap_open_offline() failed with error %s", errbuf);
    return 1;
  }

  si.fd = fd;
  si.device_info = device_info;
  si.packet_id = 0; /* updated by mb_device_info_finder() */

  /* parse pcap */
  if (
    pcap_loop(
      fd,                     /* pcap file descriptor */
      -1,                     /* for compatibility with older libcap versions; process all packets */
      mb_device_info_finder,  /* callback function to process packets */
      (u_char *)(&si)         /* user data */
    ) == -1) {
      LOG_ERROR("pcap_loop() failed with error %s", pcap_geterr(fd));
      pcap_close(fd);
      return 1;
    }

  pcap_close(fd);
  return 0;
}

int mb_find_device_info_in_f43_response(mb_f43_response* r, gsize len, mb_device_info *device_info) {
  gsize rlen = len; /* remaining number of bytes */
  mb_object *object = NULL;
  assert(r != NULL);
  assert(device_info != NULL);

  /* enough data length for a MODBUS response with function code 43? */
  if (len < MB_F43_RESPONSE_SIZE) {
    LOG_MSG("malformed packet. len (%u) < MB_F43_RESPONSE_SIZE", len);
    return 1;
  }

  /* 43 / 14 (0x2B / 0x0E) Read Device Identification */
  if (r->mei_type != 14) return 0;

  /* 03 is device dependent, so need to know the specs for parsing */
  if (r->conformity_level == 3) return 0;

  /* first object in the response */
  object = (mb_object *)((guint8 *)r + MB_F43_RESPONSE_SIZE);
  rlen -= MB_F43_RESPONSE_SIZE;

  /* read objects until all remaining bytes are read */
  while (rlen >= MB_OBJECT_SIZE(object)) {
    if (object->id == 0x00 && !(device_info->vendor_name.data)) {
      device_info->vendor_name.len = object->len;
      device_info->vendor_name.data = g_memdup(MB_OBJECT_DATA(object), object->len);
    } else if (object->id == 0x01 && !(device_info->product_code.data)) {
      device_info->product_code.len = object->len;
      device_info->product_code.data = g_memdup(MB_OBJECT_DATA(object), object->len);
    } else if (object->id == 0x02 && !(device_info->product_version.data)) {
      device_info->product_version.len = object->len;
      device_info->product_version.data = g_memdup(MB_OBJECT_DATA(object), object->len);
    } else if (object->id == 0x03 && !(device_info->vendor_url.data)) {
      device_info->vendor_url.len = object->len;
      device_info->vendor_url.data = g_memdup(MB_OBJECT_DATA(object), object->len);
    } else if (object->id == 0x04 && !(device_info->product_name.data)) {
      device_info->product_name.len = object->len;
      device_info->product_name.data = g_memdup(MB_OBJECT_DATA(object), object->len);
    } else if (object->id == 0x05 && !(device_info->model_name.data)) {
      device_info->model_name.len = object->len;
      device_info->model_name.data = g_memdup(MB_OBJECT_DATA(object), object->len);
    }
    rlen -= MB_OBJECT_SIZE(object);
    object = (mb_object *)((guint8 *)object + MB_OBJECT_SIZE(object));
  }

  return 0;
}

int mb_find_device_info_in_f17_response(mb_f17_response* r, gsize len, mb_device_info *device_info) {
  gsize rlen = len; /* remaining number of bytes */
  uint8_t *data = NULL;
  assert(r != NULL);
  assert(device_info != NULL);

  /* already found server_id? */
  if (device_info->server_id.data) return 0;

  /* enough data length for a MODBUS response with function code 17? */
  if (len < MB_F17_RESPONSE_SIZE) {
    LOG_MSG("malformed packet. len (%u) < MB_F17_RESPONSE_SIZE", len);
    return 1;
  }

  /* is there any data? */
  if (r->byte_count == 0) return 0;

  data = ((uint8_t *)(r) + MB_F17_RESPONSE_SIZE);
  rlen -= MB_F17_RESPONSE_SIZE;
  device_info->server_id.len = (rlen < r->byte_count) ? rlen : r->byte_count;
  device_info->server_id.data = g_memdup(data, device_info->server_id.len);
  return 0;
}

/* expected packet structure:
case 1: 43 / 14 (0x2B / 0x0E) Read Device Identification
ether_header, ip, tcphdr, mb_header, mb_f43_response [, mb_object [, mb_object ...]]

case 2: 17 (0x11) Report Server ID (Serial Line only)
ether_header, ip, tcphdr, mb_header, mb_f17_response [bytes]
 */
int mb_find_device_info_in_packet(guint8 *bytes, gsize len, mb_device_info *device_info) {
  struct ether_header *eth = NULL;
  struct ip *iph = NULL;
  struct tcphdr *tcph = NULL;
  mb_header *mbh = NULL;
  uint16_t mblen = 0;
  gsize rlen = len; /* remaining packet length */
  assert(bytes != NULL);

  /* is packet size >= min size of a valid MODBUS PDU? */
  if (rlen < (
    sizeof(struct ether_header) +
    sizeof(struct ip) +
    sizeof(struct tcphdr) +
    MB_HEADER_SIZE))
    return 0;

  eth = (struct ether_header *)bytes;
  rlen -= sizeof(struct ether_header);

  /* handle only IP packets */
  if (ntohs(eth->ether_type) == ETHERTYPE_IP) {
    iph = (struct ip *)((guint8 *)eth + sizeof(struct ether_header));
    rlen -= sizeof(struct ip);

    /* handle only TCP packets */
    if (iph->ip_p == IPPROTO_TCP) {
      tcph = (struct tcphdr *)((guint8 *)iph + sizeof(struct ip));
      rlen -= sizeof(struct tcphdr);

      /* handle only modbus response messages (source port = 502) */
      if (ntohs(tcph->source) == 502) {
        /*log_mb_packet(bytes, len);*/
        mbh = (mb_header *)((guint8 *)tcph + sizeof(struct tcphdr));
        rlen -= MB_HEADER_SIZE;
        /*log_mb_header(mbh);*/

        mblen = ntohs(mbh->len);
        if (mblen < 2) { /* can't read function code */
          LOG_MSG("malformed packet. mbh->len (%u) < 2", mblen);
          return 0;
        }

        /* update mblen to skip over the uid and func_code members */
        mblen -= 2;

        if (mblen > rlen) {
          LOG_MSG("malformed packet. mblen (%u) > remaining packet size (%u)", mblen, rlen);
          return 0;
        }

        if (mbh->func_code == 43) { /* potential case 1 */

          mb_f43_response *r = NULL;
          if (mblen < MB_F43_RESPONSE_SIZE) {
            LOG_MSG("malformed packet. mblen (%u) < MB_F43_RESPONSE_SIZE", mblen);
            return 0;
          }
          r = (mb_f43_response *)((guint8 *)mbh + MB_HEADER_SIZE);
          rlen -= MB_F43_RESPONSE_SIZE;
          mb_find_device_info_in_f43_response(r, mblen, device_info);

        } else if (mbh->func_code == 17) { /* potential case 2 */

          mb_f17_response *r = NULL;
          if (mblen < MB_F17_RESPONSE_SIZE) {
            LOG_MSG("malformed packet. mblen (%u) < MB_F17_RESPONSE_SIZE", mblen);
            return 0;
          }
          r = (mb_f17_response *)((guint8 *)mbh + MB_HEADER_SIZE);
          rlen -= MB_F17_RESPONSE_SIZE;
          mb_find_device_info_in_f17_response(r, mblen, device_info);

        }
      }
    }
  }

  return 0;
}