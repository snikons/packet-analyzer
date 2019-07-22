
#!/bin/sh
# author: snikons

set -e

function build {
  echo "building..."
  exec > _build.log
  exec 2>&1
  ./build.sh
}

function unit_tests {
  echo "running unit tests..."
  bin/packet-analyzer-unit-tests
}

function system_tests {
  echo "running system tests..."
  for pcap_file in test/data/*.pcap; do
    echo "analyzing $pcap_file..."
    bin/packet-analyzer -f tcp-mb-device-info -p "$pcap_file"
  done
}

(build)
echo ""
(unit_tests)
echo ""
(system_tests)