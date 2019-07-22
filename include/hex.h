/* author: snikons */

#ifndef _HEX_H_
#define _HEX_H_

/* glib */
#include <glib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* call g_free() to free the returned data */
gchar *bytes_to_hex(const guint8 *bytes, gsize len, gsize *out_len);
/* call g_free() to free the returned data */
guint8 *hex_to_bytes(const gchar *hex, gsize len, gsize *out_len);

#ifdef __cplusplus
}
#endif

#endif /* _HEX_H_ */