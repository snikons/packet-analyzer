# packet-analyzer
A packet analyzer in C

# Dependencies
* glib2
* CMake
* C compiler

# Build
`./build.sh`

# Run
`bin/packet-analyzer <options>`

```
Usage:
  packet-analyzer [OPTION?] - Packet Analyzer

Help Options:
  -h, --help            Show help options

Application Options:
  -l, --log             Enable logging
  -p, --pcap-path=P     PCAP file path
  -f, --find=F          search target
  -t, --list            List available targets to find
```

```
Available search targets:
  tcp-mb-device-info: MODBUS device vendor, product and version
```

# Run unit and system tests
`./test.sh`  
* Runs unit tests
* Runs `bin/packet-analyzer` for each PCAP file in test/data

# Tested on
* CentOS 7

# Wireshark
## Filter to find modbus packets with Vendor, product and version information
* `modbus.func_code == 43`
* `modbus.func_code == 17`

# References
* [MODBUS APPLICATION PROTOCOL SPECIFICATION V1.1b3](http://www.modbus.org/docs/Modbus_Application_Protocol_V1_1b3.pdf)
* [MODBUS Messaging Implementation Guide V1.0a](http://www.modbus.org/docs/Modbus_Messaging_Implementation_Guide_V1_0a.pdf)
* [Sample pcap files](https://www.pcapr.net/browse?q=modbus)
* [Sample modbus pcap file](https://github.com/ITI/ICS-Security-Tools/blob/master/pcaps/bro/modbus/modbus.pcap)
* [modbus simulators](https://wingpath.co.uk)
  * requires registration
* [modbus simulator](https://github.com/riptideio/modbus-simulator)
* [Modbus Message Generator](https://pymodbus.readthedocs.io/en/v1.3.2/examples/message-generator.html)
* [libmodbus](https://libmodbus.org/docs/v3.1.1/)
* [packetdrill](https://github.com/google/packetdrill)
