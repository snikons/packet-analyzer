/* author: snikons */

#ifndef _MODBUS_TESTS_H_
#define _MODBUS_TESTS_H_

#include <glib.h>
#include "modbus.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
} MBTestsFixture;

typedef struct {
  const mb_device_info expected_device_info;
  const gchar *packets[]; /* array of hex encoded packets */
} MBTestData;

void mb_tests_setup(MBTestsFixture *fixture, gconstpointer user_data);
void mb_tests_teardown(MBTestsFixture *fixture, gconstpointer user_data);
void mb_device_info_test(MBTestsFixture *fixture, gconstpointer user_data);

#ifdef __cplusplus
}
#endif

#endif /* _MODBUS_TESTS_H_ */