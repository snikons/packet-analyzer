/* author: snikons */

#ifndef _HEX_TESTS_H_
#define _HEX_TESTS_H_

#include <glib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
} HexTestsFixture;

void hex_tests_setup(HexTestsFixture *fixture, gconstpointer user_data);
void hex_tests_teardown(HexTestsFixture *fixture, gconstpointer user_data);
void hex_encode_decode_test(HexTestsFixture *fixture, gconstpointer user_data);
void hex_params_test(HexTestsFixture *fixture, gconstpointer user_data);

#ifdef __cplusplus
}
#endif

#endif /* _HEX_TESTS_H_ */