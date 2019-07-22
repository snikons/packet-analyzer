# author: snikons

add_executable(
  packet-analyzer-unit-tests
  src/modbus_tests.c
  src/hex_tests.c
  ../src/modbus.c
  ../src/hex.c
  ../src/log.c
  src/main.c
)

target_link_libraries(
  packet-analyzer-unit-tests
  glib-2.0
  pcap
)