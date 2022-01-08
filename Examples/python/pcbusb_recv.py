from PCANBasic import *        ## PCAN-Basic library import

import string                  ## String functions
import time                    ## Time-related library
import select                  ## Waiting for I/O completion
import signal                  ## Set handlers for asynchronous events
import sys                     ## System-specific parameters and functions

def sigterm(signo, frame):
    print #"got signal %d" % signo
    PCANBasic().Uninitialize(PCAN_NONEBUS)
    sys.exit(0)

class PCBUSB_Recv:
    pcan = PCANBasic()
    channel = PCAN_USBBUS1
    baudrate = PCAN_BAUD_250K
    result = pcan.Initialize(channel,baudrate)
    print "init result = ",result
    if result != PCAN_ERROR_OK:
        print "Error!",pcan.GetErrorText(result)
        sys.exit(1)

    result = pcan.GetValue(channel,PCAN_RECEIVE_EVENT)
    if result[0] == PCAN_ERROR_OK:
        fd = result[1]
    else:
        fd = -1

    signal.signal(signal.SIGTERM, sigterm)
    signal.signal(signal.SIGINT, sigterm)

    while 1:
        result = pcan.Read(channel)
        if result[0] == PCAN_ERROR_OK:
            CANmsg=result[1]
            print "ID %3X" % CANmsg.ID,
            print "len=%d [ " % CANmsg.LEN,
            for i in range(CANmsg.LEN):
                print "%02X " % CANmsg.DATA[i],
            print "]"
        elif result[0] == PCAN_ERROR_QRCVEMPTY:
            if fd != -1:
                select.select([fd],[],[])
            else:
                time.sleep(0.001)

# end PCBUSB_Recv
