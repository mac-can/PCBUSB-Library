### macOS&reg; Library for PCAN-USB Interfaces

Copyright &copy; 2013-2020 by UV Software, Berlin.

# Running CAN and CAN FD on a Mac&reg;

The PCBUSB library realizes a CAN to USB user-space driver under macOS for PCAN-USB interfaces from PEAK-System Technik, Darmstadt.
It supports up to 8 PCAN-USB and PCAN-USB FD devices.
The library offers an easy to use API to read received CAN messages from a 64K message queue and to transmit CAN messages.
Standard CAN frames (11-bit identifier) as well as extended CAN frames (29-bit identifier) are supported.
The PCAN-USB FD device can be operated in CAN 2.0 and CAN FD mode.

## PCBUSB library

The dynamic library libPCBUSB is running under macOS 10.13 and later (x86_64 architecture).
The API is almost compatible to PEAK´s PCANBasic DLL on Windows.
See the [MacCAN](https://www.mac-can.com/) website for details.

#### Supported Devices

Only the following devices from PEAK-System Technik are supported:
- PCAN-USB (product code: IPEH-002021, IPEH-002022)

Since version 0.8 (Build 689 of September 20, 2017):
- PCAN-USB FD (product code: IPEH-004022)

## This and That

This repo contains only binaries files of the PCBUSB library.

### License

The PCBUSB library is freeware without any warranty or support!

Please note the copyright and license agreement.

### Trademarks

Mac and macOS are trademarks of Apple Inc., registered in the U.S. and other countries. \
PCAN is a registered trademark of PEAK-System Technik GmbH, Darmstadt, Germany.

### Hazard Note

_If you connect your CAN device to a real CAN network when using this library, you might damage your application._

### Contact

Uwe Vogt \
UV Software \
Chausseestrasse 33a \
10115 Berlin \
Germany

E-Mail: mailto://info@mac.can.com \
Internet: https://www.mac-can.com

##### *Happy CANgineering!*
