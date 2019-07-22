/* author: snikons */

#ifndef _MODBUS_PRIVATE_H_
#define _MODBUS_PRIVATE_H_

#ifdef __cplusplus
extern "C" {
#endif

/* MBAP header */
typedef struct {
  uint16_t tid; /* transaction id */
  uint16_t pid; /* protocol id */
  uint16_t len; /* number of following bytes */
  uint8_t uid;  /* unit id */
  uint8_t func_code; /* function code */
} __attribute__ ((__packed__)) mb_header;

#define MB_HEADER_SIZE 8

/* for mb_header.func_code = 43 */
typedef struct {
  uint8_t mei_type;
  uint8_t device_id_code;
  uint8_t conformity_level;
  uint8_t more_follows;
  uint8_t next_object_id;
  uint8_t num_objects;
} __attribute__ ((__packed__)) mb_f43_response;

#define MB_F43_RESPONSE_SIZE 6

/* for mb_header.func_code = 17 */
typedef struct {
  uint8_t byte_count;
} __attribute__ ((__packed__)) mb_f17_response;

#define MB_F17_RESPONSE_SIZE 1

typedef struct {
  uint8_t id;
  uint8_t len;
} __attribute__ ((__packed__)) mb_object;

#define MB_OBJECT_DATA(objptr) ((uint8_t *)(objptr) + 2)
#define MB_OBJECT_SIZE(objptr) (2 + objptr->len)

typedef struct {
  pcap_t *fd;
  mb_device_info *device_info;
  gsize packet_id;
} shared_info;

void log_mb_packet(guint8 *bytes, gsize len);
void log_mb_header(const mb_header *mbh);
/* pcap_loop() callback function */
void mb_device_info_finder(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes);
int mb_find_device_info_in_pcap(gchar *pcap_path, mb_device_info *device_info);
int mb_parse_mb_f43_response(mb_f43_response* r, gsize len, mb_device_info *device_info);
int mb_parse_mb_f17_response(mb_f17_response* r, gsize len, mb_device_info *device_info);
int mb_find_device_info_in_packet(guint8 *bytes, gsize len, mb_device_info *device_info);

#ifdef __cplusplus
}
#endif

#endif /* _MODBUS_PRIVATE_H_ */