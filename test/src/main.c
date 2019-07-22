/* author: snikons */

/* glib */
#include <glib.h>
#include <gmodule.h>

/* setlocale() */
#include <locale.h>

/* modbus component unit tests */
#include "modbus_tests.h"

/* hex component unit tests */
#include "hex_tests.h"

int main(int argc, char* argv[]) {
  setlocale (LC_ALL, "");
  g_test_init (&argc, &argv, NULL);
  
  {
    /* test data */
    const GByteArray data1 =  {
      (guint8 *)"\x00\x01\x02\x03",
      4
    };
    const GByteArray data2 = {
      (guint8 *)"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F"
      "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F",
      32
    };
    
    /* transactions = 1, function code = 43, MEI type = 14,
        object ids = 0x00, 0x01, 0x02 */
    static const MBTestData data3 = {
      { /* expect */
        {(guint8 *)"A", 1},           /* vendor_name */
        {(guint8 *)"B", 1},           /* product_code */
        {(guint8 *)"C", 1},           /* product_version */
        {NULL, 0},                    /* vendor_url */
        {NULL, 0},                    /* product_name */
        {NULL, 0},                    /* model_name */
        {NULL, 0}                     /* server_id */
      },
      { /* packets */
        "01 02 03 04 05 06 06 05 04 03 02 01 08 00 45 00 \n"
        "00 33 03 00 00 00 40 06 79 c3 7f 00 00 01 7f 00 \n"
        "00 01 01 f6 75 04 00 00 00 02 00 00 00 0d 50 18 \n"
        "80 00 ab 84 00 00 00 00 00 00 00 11 01 2b 0e 01 \n"
        "01 00 00 03 00 01 41 01 01 42 02 01 43",
        NULL
      }
    };
    
    /* transactions = 3, function code = 43, MEI type = 14,
        object ids = 0x00, 0x01, 0x02 */
    static const MBTestData data4 = {
      { /* expect */
        {(guint8 *)"vendor name", 11},     /* vendor_name */
        {(guint8 *)"product code", 12},    /* product_code */
        {(guint8 *)"product version", 15}, /* product_version */
        {(guint8 *)"vendor url", 10},      /* vendor_url */
        {(guint8 *)"product name", 12},    /* product_name */
        {(guint8 *)"model name", 10},      /* model_name */
        {NULL, 0}                          /* server_id */
      },
      { /* packets */
        /* packet #1: vendor_name (00) */
        "01 02 03 04 05 06 06 05 04 03 02 01 08 00 45 00 \n"
        "00 33 03 00 00 00 40 06 79 c3 7f 00 00 01 7f 00 \n"
        "00 01 01 f6 75 04 00 00 00 02 00 00 00 0d 50 18 \n"
        "80 00 ab 84 00 00 00 00 00 00 00 15 01 2b 0e 01 \n"
        "01 FF 01 06 00 0B 76 65 6E 64 6F 72 20 6E 61 6D \n"
        "65",
        /* packet #2: product_code (01) */
        "01 02 03 04 05 06 06 05 04 03 02 01 08 00 45 00 \n"
        "00 33 03 00 00 00 40 06 79 c3 7f 00 00 01 7f 00 \n"
        "00 01 01 f6 75 04 00 00 00 02 00 00 00 0d 50 18 \n"
        "80 00 ab 84 00 00 00 00 00 00 00 16 01 2b 0e 01 \n"
        "01 FF 02 06 01 0C 70 72 6F 64 75 63 74 20 63 6F \n"
        "64 65",
        /* packet #3: vendor_url (03), product_version (02),
            product_name (04), model_name (05)
            object ids not in a sequential order */
        "01 02 03 04 05 06 06 05 04 03 02 01 08 00 45 00 \n"
        "00 33 03 00 00 00 40 06 79 c3 7f 00 00 01 7f 00 \n"
        "00 01 01 f6 75 04 00 00 00 02 00 00 00 0d 50 18 \n"
        "80 00 ab 84 00 00 00 00 00 00 00 3F 01 2b 0e 01 \n"
        "01 00 01 06 03 0A 76 65 6E 64 6F 72 20 75 72 6C \n"
        "02 0F 70 72 6F 64 75 63 74 20 76 65 72 73 69 6F \n"
        "6E 04 0C 70 72 6F 64 75 63 74 20 6E 61 6D 65 05 \n"
        "0A 6D 6F 64 65 6C 20 6E 61 6D 65",
        NULL
      }
    };
    
    /* transactions = 1, function code = 43, MEI type = 14,
        object ids = none */
    static const MBTestData data5 = {
      { /* expect */
        {NULL, 0},                    /* vendor_name */
        {NULL, 0},                    /* product_code */
        {NULL, 0},                    /* product_version */
        {NULL, 0},                    /* vendor_url */
        {NULL, 0},                    /* product_name */
        {NULL, 0},                    /* model_name */
        {NULL, 0}                     /* server_id */
      },
      { /* packets */
        "01 02 03 04 05 06 06 05 04 03 02 01 08 00 45 00 \n"
        "00 33 03 00 00 00 40 06 79 c3 7f 00 00 01 7f 00 \n"
        "00 01 01 f6 75 04 00 00 00 02 00 00 00 0d 50 18 \n"
        "80 00 ab 84 00 00 00 00 00 00 00 08 01 2b 0e 01 \n"
        "01 00 00 00",
        NULL
      }
    };
    
    /* transactions = 1, function code = 43, MEI type = 15,
        object ids = 00 */
    static const MBTestData data6 = {
      { /* expect */
        {NULL, 0},                    /* vendor_name */
        {NULL, 0},                    /* product_code */
        {NULL, 0},                    /* product_version */
        {NULL, 0},                    /* vendor_url */
        {NULL, 0},                    /* product_name */
        {NULL, 0},                    /* model_name */
        {NULL, 0}                     /* server_id */
      },
      { /* packets */
        "01 02 03 04 05 06 06 05 04 03 02 01 08 00 45 00 \n"
        "00 33 03 00 00 00 40 06 79 c3 7f 00 00 01 7f 00 \n"
        "00 01 01 f6 75 04 00 00 00 02 00 00 00 0d 50 18 \n"
        "80 00 ab 84 00 00 00 00 00 00 00 08 01 2b 0F 01 \n"
        "01 00 00 01 00 01 41",
        NULL
      }
    };
    
    /* transactions = 1, function code = 44, MEI type = 14,
        object ids = 00 */
    static const MBTestData data7 = {
      { /* expect */
        {NULL, 0},                    /* vendor_name */
        {NULL, 0},                    /* product_code */
        {NULL, 0},                    /* product_version */
        {NULL, 0},                    /* vendor_url */
        {NULL, 0},                    /* product_name */
        {NULL, 0},                    /* model_name */
        {NULL, 0}                     /* server_id */
      },
      { /* packets */
        "01 02 03 04 05 06 06 05 04 03 02 01 08 00 45 00 \n"
        "00 33 03 00 00 00 40 06 79 c3 7f 00 00 01 7f 00 \n"
        "00 01 01 f6 75 04 00 00 00 02 00 00 00 0d 50 18 \n"
        "80 00 ab 84 00 00 00 00 00 00 00 08 01 2c 0e 01 \n"
        "01 00 00 01 00 01 41",
        NULL
      }
    };

    /* transactions = 1, function code = 43, MEI type = 14,
        no data after MEI type */
    static const MBTestData data8 = {
      { /* expect */
        {NULL, 0},                    /* vendor_name */
        {NULL, 0},                    /* product_code */
        {NULL, 0},                    /* product_version */
        {NULL, 0},                    /* vendor_url */
        {NULL, 0},                    /* product_name */
        {NULL, 0},                    /* model_name */
        {NULL, 0}                     /* server_id */
      },
      { /* packets */
        "01 02 03 04 05 06 06 05 04 03 02 01 08 00 45 00 \n"
        "00 33 03 00 00 00 40 06 79 c3 7f 00 00 01 7f 00 \n"
        "00 01 01 f6 75 04 00 00 00 02 00 00 00 0d 50 18 \n"
        "80 00 ab 84 00 00 00 00 00 00 00 08 01 2b 0e",
        NULL
      }
    };

    /* malformed TCP packet */
    static const MBTestData data9 = {
      { /* expect */
        {NULL, 0},                    /* vendor_name */
        {NULL, 0},                    /* product_code */
        {NULL, 0},                    /* product_version */
        {NULL, 0},                    /* vendor_url */
        {NULL, 0},                    /* product_name */
        {NULL, 0},                    /* model_name */
        {NULL, 0}                     /* server_id */
      },
      { /* packets */
        "01 02 03 04 05 06 06 05 04 03 02 01 08 00 45 00 \n"
        "00 33 03 00 00 00 40 06 79 c3 7f 00 00 01 7f 00 \n"
        "00 01 01 f6",
        NULL
      }
    };

    /* non-MODBUS TCP packet */
    static const MBTestData data10 = {
      { /* expect */
        {NULL, 0},                    /* vendor_name */
        {NULL, 0},                    /* product_code */
        {NULL, 0},                    /* product_version */
        {NULL, 0},                    /* vendor_url */
        {NULL, 0},                    /* product_name */
        {NULL, 0},                    /* model_name */
        {NULL, 0}                     /* server_id */
      },
      { /* packets */
        "06 05 04 03 02 01 01 02 03 04 05 06 08 00 45 00 \n"
        "00 28 01 00 00 00 40 06 7b ce 7f 00 00 01 7f 00 \n"
        "00 01 75 04 01 f6 00 00 00 01 00 00 00 00 50 02 \n"
        "80 00 ba e4 00 00",
        NULL
      }
    };
    
    /* transactions = 1, function code = 17 */
    static const MBTestData data11 = {
      { /* expect */
        {NULL, 0},                    /* vendor_name */
        {NULL, 0},                    /* product_code */
        {NULL, 0},                    /* product_version */
        {NULL, 0},                    /* vendor_url */
        {NULL, 0},                    /* product_name */
        {NULL, 0},                    /* model_name */
        {(guint8 *)"device info", 11} /* server_id */
      },
      { /* packets */
        "00 20 78 00 62 0d 00 02 b3 ce 70 51 08 00 45 00 \n"
        "00 43 ff f8 40 00 80 06 e6 80 0a 00 00 03 0a 00 \n"
        "00 39 01 f6 0a 12 70 f1 ad 6f 61 97 f1 eb 50 18 \n"
        "ff 97 d5 c7 00 00 00 00 00 00 00 0E 0a 11 0B 64 \n"
        "65 76 69 63 65 20 69 6E 66 6F",
        NULL
      }
    };
    
    /* transactions = 1, function code = 17
        MODBUS msg length < data length */
    static const MBTestData data12 = {
      { /* expect */
        {NULL, 0},                    /* vendor_name */
        {NULL, 0},                    /* product_code */
        {NULL, 0},                    /* product_version */
        {NULL, 0},                    /* vendor_url */
        {NULL, 0},                    /* product_name */
        {NULL, 0},                    /* model_name */
        {(guint8 *)"device", 6}       /* server_id */
      },
      { /* packets */
        "00 20 78 00 62 0d 00 02 b3 ce 70 51 08 00 45 00 \n"
        "00 43 ff f8 40 00 80 06 e6 80 0a 00 00 03 0a 00 \n"
        "00 39 01 f6 0a 12 70 f1 ad 6f 61 97 f1 eb 50 18 \n"
        "ff 97 d5 c7 00 00 00 00 00 00 00 09 0a 11 0B 64 \n"
        "65 76 69 63 65 20 69 6E 66 6F",
        NULL
      }
    };
    
    /* transactions = 1, function code = 17
        MODBUS msg length > data length */
    static const MBTestData data13 = {
      { /* expect */
        {NULL, 0},                    /* vendor_name */
        {NULL, 0},                    /* product_code */
        {NULL, 0},                    /* product_version */
        {NULL, 0},                    /* vendor_url */
        {NULL, 0},                    /* product_name */
        {NULL, 0},                    /* model_name */
        {(guint8 *)"device info", 11} /* server_id */
      },
      { /* packets */
        "00 20 78 00 62 0d 00 02 b3 ce 70 51 08 00 45 00 \n"
        "00 43 ff f8 40 00 80 06 e6 80 0a 00 00 03 0a 00 \n"
        "00 39 01 f6 0a 12 70 f1 ad 6f 61 97 f1 eb 50 18 \n"
        "ff 97 d5 c7 00 00 00 00 00 00 00 10 0a 11 0B 64 \n"
        "65 76 69 63 65 20 69 6E 66 6F 41 41",
        NULL
      }
    };

    /* hex component unit tests */
    g_test_add(
      "/hex/smalldata",
      HexTestsFixture,
      &data1,
      hex_tests_setup,
      hex_encode_decode_test,
      hex_tests_teardown);

    g_test_add(
      "/hex/largedata",
      HexTestsFixture,
      &data2,
      hex_tests_setup,
      hex_encode_decode_test,
      hex_tests_teardown);

    g_test_add(
      "/hex/params",
      HexTestsFixture,
      NULL,
      hex_tests_setup,
      hex_params_test,
      hex_tests_teardown);

    /* modbus component unit tests */
    g_test_add(
      "/modbus/deviceinfo/transactions_1_func_code_43_mei_type_14_objects_3",
      MBTestsFixture,
      &data3,
      mb_tests_setup,
      mb_device_info_test,
      mb_tests_teardown);

    g_test_add(
      "/modbus/deviceinfo/transactions_3_func_code_43_mei_type_14_objects_3",
      MBTestsFixture,
      &data4,
      mb_tests_setup,
      mb_device_info_test,
      mb_tests_teardown);

    g_test_add(
      "/modbus/deviceinfo/transactions_1_func_code_43_mei_type_14_objects_0",
      MBTestsFixture,
      &data5,
      mb_tests_setup,
      mb_device_info_test,
      mb_tests_teardown);

    g_test_add(
      "/modbus/deviceinfo/transactions_1_func_code_43_mei_type_15_objects_1",
      MBTestsFixture,
      &data6,
      mb_tests_setup,
      mb_device_info_test,
      mb_tests_teardown);

    g_test_add(
      "/modbus/deviceinfo/transactions_1_func_code_44_mei_type_14_objects_1",
      MBTestsFixture,
      &data7,
      mb_tests_setup,
      mb_device_info_test,
      mb_tests_teardown);

    g_test_add(
      "/modbus/deviceinfo/malformed_modbus_packet",
      MBTestsFixture,
      &data8,
      mb_tests_setup,
      mb_device_info_test,
      mb_tests_teardown);

    g_test_add(
      "/modbus/deviceinfo/malformed_tcp_packet",
      MBTestsFixture,
      &data9,
      mb_tests_setup,
      mb_device_info_test,
      mb_tests_teardown);

    g_test_add(
      "/modbus/deviceinfo/non_modbus_packet",
      MBTestsFixture,
      &data10,
      mb_tests_setup,
      mb_device_info_test,
      mb_tests_teardown);

    g_test_add(
      "/modbus/deviceinfo/transactions_1_func_code_17",
      MBTestsFixture,
      &data11,
      mb_tests_setup,
      mb_device_info_test,
      mb_tests_teardown);

    g_test_add(
      "/modbus/deviceinfo/transactions_1_func_code_17_msglen_smaller_than_datalen",
      MBTestsFixture,
      &data12,
      mb_tests_setup,
      mb_device_info_test,
      mb_tests_teardown);

    g_test_add(
      "/modbus/deviceinfo/transactions_1_func_code_17_msglen_greater_than_datalen",
      MBTestsFixture,
      &data13,
      mb_tests_setup,
      mb_device_info_test,
      mb_tests_teardown);

    return g_test_run();
  }
}