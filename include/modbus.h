/* author: snikons */

/* glib */
#include <glib.h>
#include <gmodule.h>

/* pcap_pkthdr */
#include <pcap/pcap.h>

#ifndef _MODBUS_H_
#define _MODBUS_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  GByteArray vendor_name;
  GByteArray product_code;
  GByteArray product_version;
  GByteArray vendor_url;
  GByteArray product_name;
  GByteArray model_name;
  GByteArray server_id;
} mb_device_info;

/* if not NULL, free data member of each mb_device_info GByteArray member by calling g_free() */
int mb_find_device_info_in_pcap(gchar *pcap_path, mb_device_info *device_info);
/* if not NULL, free data member of each mb_device_info GByteArray member by calling g_free() */
int mb_find_device_info_in_packet(guint8 *bytes, gsize len, mb_device_info *device_info);

#ifdef __cplusplus
}
#endif

#endif /* _MODBUS_H_ */