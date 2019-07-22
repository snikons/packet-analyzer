/* author: snikons */

#include "hex.h"

/* glib */
#include <glib.h>
#include <gmodule.h>

/* assert() */
#include <assert.h>

gchar *bytes_to_hex(const guint8 *bytes, gsize len, gsize *out_len) {
  gsize i = 0;
  GString *s = NULL;

  if (!bytes || len == 0) return NULL;
  if (out_len) *out_len = 0;
  s = g_string_new(NULL);
  if (!s) return NULL;

  for (i = 0; i < len; ++i) {
    if (i % 16 == 0 && i > 0) {
      g_string_append_c(s, '\n');
      if (out_len) ++*out_len;
    }
    g_string_append_printf(s, "%02X ", bytes[i]);
    if (out_len) *out_len += 3;
  }

  return g_string_free(s, FALSE);
}

guint8 *hex_to_bytes(const gchar *hex, gsize len, gsize *out_len) {
  gsize i = 0;
  guint8 *bytes = NULL;
  gsize size = 0;
  GByteArray *byte_array = NULL;
  guint8 byte = 0;
  gint8 nibbles = 0; /* count of nibbles held in byte */

  if (!hex || len == 0) return NULL;
  if (out_len) *out_len = 0;
  size = len;
  byte_array = g_byte_array_new();
  if (!byte_array) return NULL;

  for (i = 0; i < size; ++i) {
    guint8 nibble = 0;
    if (hex[i] >= '0' && hex[i] <= '9') nibble = hex[i] - '0';
    else if (hex[i] >= 'a' && hex[i] <= 'f') nibble = 10 + hex[i] - 'a';
    else if (hex[i] >= 'A' && hex[i] <= 'F') nibble = 10 + hex[i] - 'A';
    else continue;

    switch (nibbles) {
      case 0: /* save the nibble */
        byte = nibble;
        ++nibbles;
        break;

      case 1: /* complete the byte */
        byte = byte << 4;
        byte |= nibble;
        nibbles = 0;
        g_byte_array_append(byte_array, &byte, 1);
        if (out_len) ++*out_len;
        break;

      default:
        assert (FALSE); /* unexpected */
    }
  }

  return g_byte_array_free(byte_array, FALSE);
}