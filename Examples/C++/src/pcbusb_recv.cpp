//
//  macOS Library for PCAN-USB Interfaces
//  Example: receive CAN messages (Blockng read)
//
//  To compile type `clang++ pcbusb_recv.cpp -l PCBUSB -o pcbusb_recv'
//  libPCBUSB.dylib is expected to be in the library search path.
//
//  Note: Press Ctrl+C to terminate the program.
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <sys/time.h>

#include "PCBUSB.h"

#define PCAN_CHANNEL PCAN_USBBUS1
#define PCAN_BAUDRATE PCAN_BAUD_250K

static volatile int running = 1;
static void sigterm(int signo)
{
    //printf("got signal %d\n", signo);
    running = 0;
}

int main(int argc, char *argv[])
{
    int fdes;
    fd_set rdfs;

    TPCANMsg message;
    TPCANStatus status;

    if ((signal(SIGTERM, sigterm) == SIG_ERR) ||
        (signal(SIGINT, sigterm) == SIG_ERR)) {
        perror("Error");
        return errno;
    }
    status = CAN_Initialize(PCAN_CHANNEL, PCAN_BAUDRATE, 0, 0, 0);
    printf("Initialize CAN: 0x%x\n", status);
    if (status != PCAN_ERROR_OK) goto leave;

    // get a file descriptor for blocking read via `select'
    status = CAN_GetValue(PCAN_CHANNEL, PCAN_RECEIVE_EVENT, &fdes, sizeof(int));
    if (status != PCAN_ERROR_OK) goto leave;

    // see `man select' for details on I/O descriptor sets
    FD_ZERO(&rdfs);
    FD_SET(fdes, &rdfs);

    while (running) {
        status = CAN_Read(PCAN_CHANNEL, &message, NULL);
        if (status == PCAN_ERROR_OK) {
            if (!(message.MSGTYPE & PCAN_MESSAGE_STATUS))
                printf("  - R ID:%4x LEN:%1x DATA:%02x %02x %02x %02x %02x %02x %02x %02x\n",
                    (int) message.ID, (int) message.LEN, (int) message.DATA[0],
                    (int) message.DATA[1], (int) message.DATA[2],
                    (int) message.DATA[3], (int) message.DATA[4],
                    (int) message.DATA[5], (int) message.DATA[6],
                    (int) message.DATA[7]);
        } else if (status == PCAN_ERROR_QRCVEMPTY) {
            // block until at least one CAN message has been received
            if (select(fdes+1, &rdfs, NULL, NULL, NULL) < 0)
                perror("Error");
        } else {
            printf("Error 0x%x\n", status);
            break;
        }
    }
leave:
    CAN_Uninitialize(PCAN_NONEBUS);
    return 0;
}
