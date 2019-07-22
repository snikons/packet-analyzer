/* author: snikons */

#include "modbus_tests.h"
#include "hex.h"

/* strlen(), memset() */
#include <string.h>

void mb_tests_setup(MBTestsFixture *fixture, gconstpointer user_data) {

}

void mb_tests_teardown(MBTestsFixture *fixture, gconstpointer user_data) {

}

void mb_device_info_test(MBTestsFixture *fixture, gconstpointer user_data) {
  const MBTestData *data = (const MBTestData *)user_data;
  mb_device_info device_info;
  gsize i = 0;

  memset(&device_info, 0, sizeof(mb_device_info));
  while (data->packets[i]) {
    gsize len = 0;
    guint8 *packet = hex_to_bytes(data->packets[i], strlen(data->packets[i]),
                                  &len);
    g_assert(
      mb_find_device_info_in_packet(packet, len, &device_info) == 0
    );
    g_free(packet);
    ++i;
  }

  /* verify vendor_name */
  if (!data->expected_device_info.vendor_name.data) {
    g_assert_null(device_info.vendor_name.data);
  } else {
    g_assert_nonnull(device_info.vendor_name.data);
    g_assert_cmpmem(data->expected_device_info.vendor_name.data, data->expected_device_info.vendor_name.len,
                    device_info.vendor_name.data, device_info.vendor_name.len);
    g_free(device_info.vendor_name.data);
  }

  /* verify product_code */
  if (!data->expected_device_info.product_code.data) {
    g_assert_null(device_info.product_code.data);
  } else {
    g_assert_nonnull(device_info.product_code.data);
    g_assert_cmpmem(data->expected_device_info.product_code.data, data->expected_device_info.product_code.len,
                    device_info.product_code.data, device_info.product_code.len);
    g_free(device_info.product_code.data);
  }

  /* verify product_version */
  if (!data->expected_device_info.product_version.data) {
    g_assert_null(device_info.product_version.data);
  } else {
    g_assert_nonnull(device_info.product_version.data);
    g_assert_cmpmem(data->expected_device_info.product_version.data, data->expected_device_info.product_version.len,
                    device_info.product_version.data, device_info.product_version.len);
    g_free(device_info.product_version.data);
  }

  /* verify vendor_url */
  if (!data->expected_device_info.vendor_url.data) {
    g_assert_null(device_info.vendor_url.data);
  } else {
    g_assert_nonnull(device_info.vendor_url.data);
    g_assert_cmpmem(data->expected_device_info.vendor_url.data, data->expected_device_info.vendor_url.len,
                    device_info.vendor_url.data, device_info.vendor_url.len);
    g_free(device_info.vendor_url.data);
  }

  /* verify product_name */
  if (!data->expected_device_info.product_name.data) {
    g_assert_null(device_info.product_name.data);
  } else {
    g_assert_nonnull(device_info.product_name.data);
    g_assert_cmpmem(data->expected_device_info.product_name.data, data->expected_device_info.product_name.len,
                    device_info.product_name.data, device_info.product_name.len);
    g_free(device_info.product_name.data);
  }

  /* verify model_name */
  if (!data->expected_device_info.model_name.data) {
    g_assert_null(device_info.model_name.data);
  } else {
    g_assert_nonnull(device_info.model_name.data);
    g_assert_cmpmem(data->expected_device_info.model_name.data, data->expected_device_info.model_name.len,
                    device_info.model_name.data, device_info.model_name.len);
    g_free(device_info.model_name.data);
  }

  /* verify server_id */
  if (!data->expected_device_info.server_id.data) {
    g_assert_null(device_info.server_id.data);
  } else {
    g_assert_nonnull(device_info.server_id.data);
    g_assert_cmpmem(data->expected_device_info.server_id.data, data->expected_device_info.server_id.len,
                    device_info.server_id.data, device_info.server_id.len);
    g_free(device_info.server_id.data);
  }
}
