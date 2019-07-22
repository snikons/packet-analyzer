/* author: snikons */

#include "hex_tests.h"
#include "hex.h"

/* glib */
#include <gmodule.h>

void hex_tests_setup(HexTestsFixture *fixture, gconstpointer user_data) {

}

void hex_tests_teardown(HexTestsFixture *fixture, gconstpointer user_data) {

}

void hex_encode_decode_test(HexTestsFixture *fixture, gconstpointer user_data) {
  gchar *hex = NULL;
  guint8 *decoded = NULL;
  const gsize input_size = (const gsize)((GByteArray *)user_data)->len;
  const guint8 *data = (const guint8 *)((GByteArray *)user_data)->data;
  gsize size = input_size;
  hex = bytes_to_hex(data, size, &size);
  decoded = hex_to_bytes(hex, size, &size);
  g_assert_cmpuint(input_size, ==, size);
  g_assert_cmpmem(data, input_size, decoded, size);
  g_free(hex);
  g_free(decoded);
}

void hex_params_test(HexTestsFixture *fixture, gconstpointer user_data) {
  gsize size = 0;
  const guint8 data = 0;
  const gchar hex = 0;

  g_assert_null(bytes_to_hex(NULL, 0, NULL));
  g_assert_null(bytes_to_hex(NULL, 0, &size));
  g_assert_null(bytes_to_hex(NULL, 1, NULL));
  g_assert_null(bytes_to_hex(&data, 0, NULL));

  g_assert_null(hex_to_bytes(NULL, 0, NULL));
  g_assert_null(hex_to_bytes(NULL, 0, &size));
  g_assert_null(hex_to_bytes(NULL, 1, NULL));
  g_assert_null(hex_to_bytes(&hex, 0, NULL));
}