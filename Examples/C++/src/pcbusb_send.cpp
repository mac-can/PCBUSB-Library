//
//  macOS Library for PCAN-USB Interfaces
//  Example: send CAN messages
//
//  To compile type `clang++ pcbusb_send.cpp -l PCBUSB -o pcbusb_send'
//  libPCBUSB.dylib is expected to be in the library search path.
//
//  Note: Press Ctrl+C to terminate the program.
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

#include "PCBUSB.h"

#define PCAN_CHANNEL PCAN_USBBUS2
#define PCAN_BAUDRATE PCAN_BAUD_250K

static volatile int running = 1;
static void sigterm(int signo)
{
    //printf("got signal %d\n", signo);
    running = 0;
}

int main(int argc, char *argv[])
{
    TPCANMsg message;
    TPCANStatus status;
    unsigned long frames = 0;

    if ((signal(SIGTERM, sigterm) == SIG_ERR) ||
        (signal(SIGINT, sigterm) == SIG_ERR)) {
        perror("+++ Error");
        return errno;
    }
    status = CAN_Initialize(PCAN_CHANNEL, PCAN_BAUDRATE, 0, 0, 0);
    printf("Initialize CAN: 0x%x\n", status);
    if (status != PCAN_ERROR_OK) goto leave;

    message.ID = 0x100;
    message.LEN = 8;
    message.MSGTYPE = PCAN_MESSAGE_STANDARD;
    memset(message.DATA, 0x00, message.LEN);

    while (running) {
        if ((status = CAN_Write(PCAN_CHANNEL, &message)) == PCAN_ERROR_OK) {
            frames++;
//            if ((frames % 2048) == 0)
//                printf("  - T Message %lu\n", frames);
            message.DATA[0] = (BYTE)((frames & 0x000000FFU) >> 0);
            message.DATA[1] = (BYTE)((frames & 0x0000FF00U) >> 8);
            message.DATA[2] = (BYTE)((frames & 0x00FF0000U) >> 16);
            message.DATA[3] = (BYTE)((frames & 0xFF000000U) >> 24);
        }
    }
leave:
    status = CAN_Uninitialize(PCAN_NONEBUS);
    return 0;
}
