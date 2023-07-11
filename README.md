### macOS&reg; Library for PCAN-USB Interfaces

*Copyright &copy;  2013-2023 by UV Software, Berlin.*\
*All rights reserved.*

# Running CAN and CAN FD on a Mac&reg;

The PCBUSB library realizes an USB to CAN user-space driver under macOS for PCAN-USB interfaces from PEAK-System Technik, Darmstadt.
It supports up to 8 PCAN-USB or PCAN-USB FD devices.
The library offers an easy to use API to read received CAN messages from a 64K message queue and to transmit CAN messages.
Standard CAN frames (11-bit identifier) as well as extended CAN frames (29-bit identifier) are supported.
The PCAN-USB FD device can be operated in CAN 2.0 and CAN FD mode.

## PCBUSB library

The dynamic library libPCBUSB is running under macOS 10.13 and later (Intel architecture and Apple silicon).
The API is almost compatible to PEAK´s PCANBasic DLL on Windows.
See the [MacCAN](https://www.mac-can.com/) website for details.

### Target Platform

- macOS 11.0 and later (Intel and Apple silicon)
- OS X 10.13 and later (Intel architecture only)

### Development Environment

#### macOS Ventura

- macOS Ventura (13.4.1) on a Mac mini (M1, 2020)
- Apple clang version 14.0.3 (clang-1403.0.22.14.1)
- Xcode Version 14.3.1 (14E300c)

#### macOS Big Sur

- macOS Big Sur (11.7.8) on a MacBook Pro (2019)
- Apple clang version 13.0.0 (clang-1300.0.29.30)
- Xcode Version 13.2.1 (13C100)

#### macOS High Sierra

- macOS High Sierra (10.13.6) on a MacBook Pro (late 2011)
- Apple LLVM version 10.0.0 (clang-1000.11.45.5)
- Xcode Version 10.1 (10B61)

### Supported Devices

Only the following devices from PEAK-System Technik are supported:
- PCAN-USB (product code: IPEH-002021, IPEH-002022)

Since version 0.8 (Build 689 of September 20, 2017):
- PCAN-USB FD (product code: IPEH-004022)

### Known Bugs and Caveats

For a list of known bugs and caveats see the README file delivered with each release
and tab [Issues](https://github.com/mac-can/PCBUSB-Library/issues).

## This and That

This repo contains only binaries files of the PCBUSB library.

### License

The PCBUSB library is freeware without any warranty or support!

Please note the copyright and license agreement.

### Trademarks

Mac and macOS are trademarks of Apple Inc., registered in the U.S. and other countries. \
PCAN is a registered trademark of PEAK-System Technik GmbH, Darmstadt, Germany. \
All other company, product and service names mentioned herein may be trademarks, registered trademarks or service marks of their respective owners.

### Hazard Note

_If you connect your CAN device to a real CAN network when using this library, you might damage your application._

### Contact

E-Mail: mailto://info@mac.can.com \
Internet: https://www.mac-can.com
