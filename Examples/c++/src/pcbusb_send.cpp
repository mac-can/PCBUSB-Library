#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define PCAN_CHANNEL PCAN_USBBUS2
#define PCAN_BAUDRATE PCAN_BAUD_250K

#ifndef __APPLE__
#include <asm/types.h>
#define DWORD  __u32
#define WORD   unsigned short
#define BYTE   unsigned char
#define LPSTR  char*
#include "PCANBasic.h"
#else
#include "PCBUSB.h"
#endif

int main(int argc, char *argv[]) 
{
	TPCANMsg message;
	TPCANStatus status;
	unsigned long n = 0;

	status = CAN_Initialize(PCAN_CHANNEL, PCAN_BAUDRATE, 0, 0, 0);
	printf("Initialize CAN: 0x%lx\n", status);
	if(status != PCAN_ERROR_OK) goto leave;

	message.ID = 0x100;
	message.LEN = 8;
	message.MSGTYPE = PCAN_MESSAGE_STANDARD;
	memset(message.DATA, 0x00, message.LEN);

	for(;;) {
	    while((status = CAN_Write(PCAN_CHANNEL, &message)) == PCAN_ERROR_OK) {
			message.DATA[0]++;
	        if((++n % 1000) == 0)
		        printf("  - T Message %lu\n", n);
	    }
	}
leave:
	CAN_Uninitialize(PCAN_NONEBUS);
	return 0;
}
