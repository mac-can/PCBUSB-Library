from PCANBasic import *        ## PCAN-Basic library import

import string                  ## String functions
import time                    ## Time-related library
import select                  ## Waiting for I/O completion
import signal                  ## Set handlers for asynchronous events
import sys                     ## System-specific parameters and functions


def sigterm(signo, frame):
    result = PCANBasic().Uninitialize(PCAN_NONEBUS)
    if result != PCAN_ERROR_OK:
        print("Error: {}".format(PCANBasic().GetErrorText(result)))
        sys.exit(1)
    else:
        sys.exit(0)


pcan = PCANBasic()
channel = PCAN_USBBUS1
baudrate = PCAN_BAUD_250K
result = pcan.GetValue(channel,PCAN_CHANNEL_VERSION)
if result[0] == PCAN_ERROR_OK:
    print(cast(result[1], c_char_p).value.decode("ascii"))

result = pcan.Initialize(channel,baudrate)
if result != PCAN_ERROR_OK:
    print("Error: {}".format(pcan.GetErrorText(result)))
    sys.exit(1)

result = pcan.GetValue(channel,PCAN_RECEIVE_EVENT)
if result[0] == PCAN_ERROR_OK:
    fd = result[1]
else:
    fd = -1

print("Press ^C to abort...")
signal.signal(signal.SIGTERM, sigterm)
signal.signal(signal.SIGINT, sigterm)

while 1:
    result = pcan.Read(channel)
    if result[0] == PCAN_ERROR_OK:
        CANmsg = result[1]
        print("ID={:03X} LEN={:d} DATA=[{}]".format(CANmsg.ID,CANmsg.LEN,' '.join('{:02X}'.format(x) for x in CANmsg.DATA)))
    elif result[0] == PCAN_ERROR_QRCVEMPTY:
        if fd != -1:
            select.select([fd],[],[])
        else:
            time.sleep(0.001)

result = pcan.Uninitialize()
if result != PCAN_ERROR_OK:
    print("Error: {}".format(pcan.GetErrorText(result)))
    sys.exit(1)
else:
    sys.exit(0)
