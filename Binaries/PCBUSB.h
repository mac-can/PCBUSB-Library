/* -- $HeadURL: http://svn.uv-software.com/opensource/mac/CAN/PCANUSB/Library/trunk/drv/pcan_api.h $ --
 *
 *	project   :  CAN - Controller Area Network
 *
 *	purpose   :  PCAN Application Programming Interface
 *
 *	copyright :  (C) 2013 by UV Software, Berlin
 *
 *	compiler  :  GCC - GNU C Compiler (Darwin Kernel 11.4.0)
 *
 *	export    :  TPCANStatus CAN_Initialize(TPCANHandle Channel, TPCANBaudrate Btr0Btr1, TPCANType HwType, DWORD IOPort, WORD Interrupt);
 *	             TPCANStatus CAN_Uninitialize(TPCANHandle Channel);
 *	             TPCANStatus CAN_Reset(TPCANHandle Channel);
 *	             TPCANStatus CAN_GetStatus(TPCANHandle Channel);
 *	             TPCANStatus CAN_Read(TPCANHandle Channel, TPCANMsg* MessageBuffer, TPCANTimestamp* TimestampBuffer);
 *	             TPCANStatus CAN_Write(TPCANHandle Channel, TPCANMsg* MessageBuffer);
 *	             TPCANStatus CAN_FilterMessages(TPCANHandle Channel, DWORD FromID, DWORD ToID, TPCANMode Mode);
 *	             TPCANStatus CAN_GetValue(TPCANHandle Channel, TPCANParameter Parameter, void* Buffer, DWORD BufferLength);
 *	             TPCANStatus CAN_SetValue(TPCANHandle Channel, TPCANParameter Parameter, void* Buffer, DWORD BufferLength);
 *	             TPCANStatus CAN_GetErrorText(TPCANStatus Error, WORD Language, LPSTR Buffer);
 *
 *	includes  :  (none)
 *
 *	author    :  Uwe Vogt, UV Software
 *
 *	e-mail    :  uwe.vogt@uv-software.de
 *
 *
 *	-----------  description  --------------------------------------------
 *
 *	PCAN API  -  PEAK CAN Application Programming Interface
 *
 *	This Application Programming Interface (API) is a nearly compatible
 *	implementation of the PEAK PCANBasic DLL on Mac OS X (Darwin 11.4.0).
 *
 *	Supported CAN Interfaces:
 *	- PCAN-USB (up to 8 device; only the 1st channel is supported)
 */

#ifndef __PCAN_API_H
#define __PCAN_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*  -----------  includes  -----------------------------------------------
 */


/*  -----------  defines  ------------------------------------------------
 */

#ifndef BYTE
#define BYTE	unsigned char
#endif
#ifndef WORD
#define WORD	unsigned short
#endif
#ifndef DWORD
#define DWORD	unsigned long
#endif
#ifndef LPSTR
#define LPSTR	char*
#endif

/* Defined and supported PCAN channels
 */
#define PCAN_NONEBUS             0x00  //!< Undefined/default value for a PCAN bus

#define PCAN_USBBUS1             0x51  //!< PCAN-USB interface, channel 1
#define PCAN_USBBUS2             0x52  //!< PCAN-USB interface, channel 2
#define PCAN_USBBUS3             0x53  //!< PCAN-USB interface, channel 3
#define PCAN_USBBUS4             0x54  //!< PCAN-USB interface, channel 4
#define PCAN_USBBUS5             0x55  //!< PCAN-USB interface, channel 5
#define PCAN_USBBUS6             0x56  //!< PCAN-USB interface, channel 6
#define PCAN_USBBUS7             0x57  //!< PCAN-USB interface, channel 7
#define PCAN_USBBUS8             0x58  //!< PCAN-USB interface, channel 8

/* PCAN error and status codes
 */
#define PCAN_ERROR_OK            0x00000  //!< No error
#define PCAN_ERROR_XMTFULL       0x00001  //!< Transmit buffer in CAN controller is full
#define PCAN_ERROR_OVERRUN       0x00002  //!< CAN controller was read too late
#define PCAN_ERROR_BUSLIGHT      0x00004  //!< Bus error: an error counter reached the 'light' limit
#define PCAN_ERROR_BUSHEAVY      0x00008  //!< Bus error: an error counter reached the 'heavy' limit
#define PCAN_ERROR_BUSOFF        0x00010  //!< Bus error: the CAN controller is in bus-off state
#define PCAN_ERROR_ANYBUSERR     (PCAN_ERROR_BUSLIGHT | PCAN_ERROR_BUSHEAVY | PCAN_ERROR_BUSOFF) //!< Mask for all bus errors
#define PCAN_ERROR_QRCVEMPTY     0x00020  //!< Receive queue is empty
#define PCAN_ERROR_QOVERRUN      0x00040  //!< Receive queue was read too late
#define PCAN_ERROR_QXMTFULL      0x00080  //!< Transmit queue is full
#define PCAN_ERROR_REGTEST       0x00100  //!< Test of the CAN controller hardware registers failed (no hardware found)
#define PCAN_ERROR_NODRIVER      0x00200  //!< Driver not loaded
#define PCAN_ERROR_HWINUSE       0x00400  //!< Hardware already in use by a Net
#define PCAN_ERROR_NETINUSE      0x00800  //!< A Client is already connected to the Net
#define PCAN_ERROR_ILLHW         0x01400  //!< Hardware handle is invalid
#define PCAN_ERROR_ILLNET        0x01800  //!< Net handle is invalid
#define PCAN_ERROR_ILLCLIENT     0x01C00  //!< Client handle is invalid
#define PCAN_ERROR_ILLHANDLE     (PCAN_ERROR_ILLHW | PCAN_ERROR_ILLNET | PCAN_ERROR_ILLCLIENT)  //!< Mask for all handle errors
#define PCAN_ERROR_RESOURCE      0x02000  //!< Resource (FIFO, Client, timeout) cannot be created
#define PCAN_ERROR_ILLPARAMTYPE  0x04000  //!< Invalid parameter
#define PCAN_ERROR_ILLPARAMVAL   0x08000  //!< Invalid parameter value
#define PCAN_ERROR_UNKNOWN       0x10000  //!< Unknown error
#define PCAN_ERROR_INITIALIZE    0x40000  //!< Channel is not initialized

/* PCAN devices
 */
#define PCAN_NONE                0x00  //!< Undefined, unknown or not selected PCAN device value
#define PCAN_PEAKCAN             0x01  //!< PCAN Non-Plug&Play devices. NOT USED WITHIN PCAN-Basic API
#define PCAN_ISA                 0x02  //!< PCAN-ISA, PCAN-PC/104, and PCAN-PC/104-Plus
#define PCAN_DNG                 0x03  //!< PCAN-Dongle
#define PCAN_PCI                 0x04  //!< PCAN-PCI, PCAN-cPCI, PCAN-miniPCI, and PCAN-PCI Express
#define PCAN_USB                 0x05  //!< PCAN-USB and PCAN-USB Pro
#define PCAN_PCC                 0x06  //!< PCAN-PC Card

/* PCAN parameters
 */
#define PCAN_DEVICE_NUMBER       0x01  //!< PCAN-USB device number parameter
#define PCAN_5VOLTS_POWER        0x02  //!< PCAN-PC Card 5-Volt power parameter
#define PCAN_RECEIVE_EVENT       0x03  //!< PCAN receive event handler parameter
#define PCAN_MESSAGE_FILTER      0x04  //!< PCAN message filter parameter
#define PCAN_API_VERSION         0x05  //!< PCAN-Basic API version parameter
#define PCAN_CHANNEL_VERSION     0x06  //!< PCAN device channel version parameter
#define PCAN_BUSOFF_AUTORESET    0x07  //!< PCAN Reset-On-Busoff parameter
#define PCAN_LISTEN_ONLY         0x08  //!< PCAN Listen-Only parameter
#define PCAN_LOG_LOCATION        0x09  //!< Directory path for trace files
#define PCAN_LOG_STATUS          0x0A  //!< Debug-Trace activation status
#define PCAN_LOG_CONFIGURE       0x0B  //!< Configuration of the debugged information (LOG_FUNCTION_***)
#define PCAN_LOG_TEXT            0x0C  //!< Custom insertion of text into the log file
#define PCAN_CHANNEL_CONDITION   0x0D  //!< Availability status of a PCAN-Channel
#define PCAN_HARDWARE_NAME       0x0E  //!< PCAN hardware name parameter
#define PCAN_EXT_BTR0BTR1        0x80  //!< UVS: bit-timing register
#define PCAN_EXT_TX_COUNTER      0x81  //!< UVS: number of transmitted frames
#define PCAN_EXT_RX_COUNTER      0x82  //!< UVS: number of received frames
#define PCAN_EXT_ERR_COUNTER     0x83  //!< UVS: number of error frames
#define PCAN_EXT_RX_QUE_OVERRUN  0x84  //!< UVS: receive queue overrun counter
#define PCAN_EXT_LOG_USB         0x8F  //!< UVS: Log USB communication (URB buffer <==> CAN messages)

/* PCAN parameter values
 */
#define PCAN_PARAMETER_OFF       0x00  //!< The PCAN parameter is not set (inactive)
#define PCAN_PARAMETER_ON        0x01  //!< The PCAN parameter is set (active)
#define PCAN_FILTER_CLOSE        0x00  //!< The PCAN filter is closed. No messages will be received
#define PCAN_FILTER_OPEN         0x01  //!< The PCAN filter is fully opened. All messages will be received
#define PCAN_FILTER_CUSTOM       0x02  //!< The PCAN filter is custom configured. Only registered messages will be received
#define PCAN_CHANNEL_UNAVAILABLE 0x00  //!< The PCAN-Channel handle is illegal, or its associated hardware is not available
#define PCAN_CHANNEL_AVAILABLE   0x01  //!< The PCAN-Channel handle is available to be connected (Plug&Play Hardware: it means furthermore that the hardware is plugged-in)
#define PCAN_CHANNEL_OCCUPIED    0x02  //!< The PCAN-Channel handle is valid, and is already being used

/* PCAN message types
 */
#define PCAN_MESSAGE_STANDARD    0x00  //!< The PCAN message is a CAN Standard Frame (11-bit identifier)
#define PCAN_MESSAGE_RTR         0x01  //!< The PCAN message is a CAN Remote-Transfer-Request Frame
#define PCAN_MESSAGE_EXTENDED    0x02  //!< The PCAN message is a CAN Extended Frame (29-bit identifier)
#define PCAN_MESSAGE_STATUS      0x80  //!< The PCAN message represents a PCAN status message

/* Frame Type / Initialization Mode
 */
#define PCAN_MODE_STANDARD       PCAN_MESSAGE_STANDARD
#define PCAN_MODE_EXTENDED       PCAN_MESSAGE_EXTENDED

/* Baud rate codes = BTR0/BTR1 register values for the CAN controller.
 */
#define PCAN_BAUD_1M             0x0014  //!<   1 MBit/s
#define PCAN_BAUD_500K           0x001C  //!< 500 kBit/s
#define PCAN_BAUD_250K           0x011C  //!< 250 kBit/s
#define PCAN_BAUD_125K           0x031C  //!< 125 kBit/s
#define PCAN_BAUD_100K           0x432F  //!< 100 kBit/s
#define PCAN_BAUD_50K            0x472F  //!<  50 kBit/s
#define PCAN_BAUD_20K            0x532F  //!<  20 kBit/s
#define PCAN_BAUD_10K            0x672F  //!<  10 kBit/s
#define PCAN_BAUD_5K             0x7F7F  //!<   5 kBit/s


/*  -----------  types  --------------------------------------------------
 */

#define TPCANHandle              BYTE  //!< PCAN hardware channel handle
#define TPCANStatus              DWORD //!< PCAN status/error code
#define TPCANParameter           BYTE  //!< PCAN parameter to be read or set
#define TPCANDevice              BYTE  //!< PCAN device
#define TPCANMessageType         BYTE  //!< The type of a PCAN message
#define TPCANType                BYTE  //!< The type of PCAN hardware to be initialized
#define TPCANMode                BYTE  //!< PCAN filter mode
#define TPCANBaudrate            WORD  //!< PCAN Baud rate register value

/** PCAN message
 */
typedef struct tagTPCANMsg
{
    DWORD             ID;      //!< 11/29-bit message identifier
    TPCANMessageType  MSGTYPE; //!< Type of the message
    BYTE              LEN;     //!< Data Length Code of the message (0..8)
    BYTE              DATA[8]; //!< Data of the message (DATA[0]..DATA[7])
} TPCANMsg;

/** Timestamp of a received PCAN message
 *  Total Microseconds = micros + 1000 * millis + 0xFFFFFFFF * 1000 * millis_overflow
 */
typedef struct tagTPCANTimestamp
{
    DWORD  millis;             //!< Base-value: milliseconds: 0.. 2^32-1
    WORD   millis_overflow;    //!< Roll-arounds of millis
    WORD   micros;             //!< Microseconds: 0..999
} TPCANTimestamp;


/*  -----------  variables  ----------------------------------------------
 */


/*  -----------  prototypes  ---------------------------------------------
 */

/** @brief       Initializes a PCAN Channel.
 *
 *  @param[in]   Channel    The handle of a PCAN Channel.
 *  @param[in]   Btr0Btr1   The speed for the communication (BTR0BTR1 code).
 *  @param[in]   HwType     (not used with PCAN USB devices)
 *  @param[in]   IOPort     (not used with PCAN USB devices)
 *  @param[in]   Interrupt  (not used with PCAN USB devices)
 *
 *  @returns     A TPCANStatus error code.
 */
TPCANStatus CAN_Initialize(
        TPCANHandle Channel,
        TPCANBaudrate Btr0Btr1,
        TPCANType HwType,
		DWORD IOPort,
		WORD Interrupt);

/** @brief       Uninitializes one or all PCAN Channels initialized by CAN_Initialize.
 *
 *  @note        Giving the TPCANHandle value "PCAN_NONEBUS" uninitializes all initialized channels.
 *
 *  @param[in]   Channel    The handle of a PCAN Channel.
 *
 *  @returns     A TPCANStatus error code.
 */
TPCANStatus CAN_Uninitialize(
        TPCANHandle Channel);

/** @brief       Resets the receive and transmit queues of the PCAN Channel.
 *
 *  @note        A reset of the CAN controller is not performed.
 *
 *  @param[in]   Channel    The handle of a PCAN Channel.
 *
 *  @returns     A TPCANStatus error code.
 */
TPCANStatus CAN_Reset(
        TPCANHandle Channel);

/** @brief       Gets the current status of a PCAN Channel.
 *
 *  @param[in]   Channel    The handle of a PCAN Channel.
 *
 *  @returns     A TPCANStatus error code.
 */
TPCANStatus CAN_GetStatus(
        TPCANHandle Channel);

/** @brief       Reads a CAN message from the receive queue of a PCAN Channel.
 *
 *  @param[in]   Channel          The handle of a PCAN Channel.
 *  @param[out]  MessageBuffer    A TPCANMsg structure buffer to store the CAN message.
 *  @param[out]  TimestampBuffer  A TPCANTimestamp structure buffer to get  the reception time of the message.
 *                                If this value is not desired, this parameter should be passed as NULL.
 *
 *  @returns     A TPCANStatus error code.
 */
TPCANStatus CAN_Read(
        TPCANHandle Channel,
        TPCANMsg* MessageBuffer,
        TPCANTimestamp* TimestampBuffer);

/** @brief       Transmits a CAN message.
 *
 *  @param[in]   Channel        The handle of a PCAN Channel.
 *  @param[in]   MessageBuffer  A TPCANMsg buffer with the message to be sent.
 *
 *  @returns     A TPCANStatus error code.
 */
TPCANStatus CAN_Write(
        TPCANHandle Channel,
        TPCANMsg* MessageBuffer);

/** @brief       Configures the reception filter.
 *
 *  @note        The message filter will be expanded with every call to  this function.
 *               If it is desired to reset the filter, please use the CAN_SetParameter function.
 *
 *  @param[in]   Channel    The handle of a PCAN Channel.
 *  @param[in]   FromID     The lowest CAN ID to be received.
 *  @param[in]   ToID       The highest CAN ID to be received.
 *  @param[in]   Mode       Message type, Standard (11-bit identifier) or Extended (29-bit identifier).
 *
 *  @returns     A TPCANStatus error code.
 */
TPCANStatus CAN_FilterMessages(
        TPCANHandle Channel,
        DWORD FromID,
        DWORD ToID,
        TPCANMode Mode);

/** @brief       Retrieves a PCAN Channel value.
 *
 *  @note        Parameters can be present or not according with the kind of Hardware (PCAN Channel) being used.
 *               If a parameter is not available, a PCAN_ERROR_ILLPARAMTYPE error will be returned.
 *
 *  @param[in]   Channel       The handle of a PCAN Channel.
 *  @param[in]   Parameter     The TPCANParameter parameter to get.
 *  @param[out]  Buffer        Buffer for the parameter value.
 *  @param[in]   BufferLength  Size in bytes of the buffer.
 *
 *  @returns     A TPCANStatus error code.
 */
TPCANStatus CAN_GetValue(
        TPCANHandle Channel,
        TPCANParameter Parameter,
        void* Buffer,
        DWORD BufferLength);


/** @brief       Configures or sets a PCAN Channel value.
 *
 *  @note        Parameters can be present or not according with the kind of Hardware (PCAN Channel) being used.
 *               If a parameter is not available, a PCAN_ERROR_ILLPARAMTYPE error will be returned.
 *
 *  @param[in]   Channel       The handle of a PCAN Channel.
 *  @param[in]   Parameter     The TPCANParameter parameter to set.
 *  @param[in]   Buffer        Buffer with the value to be set.
 *  @param[in]   BufferLength  Size in bytes of the buffer.
 *
 *  @returns     A TPCANStatus error code.
 */
TPCANStatus CAN_SetValue(
        TPCANHandle Channel,
        TPCANParameter Parameter,
        void* Buffer,
		DWORD BufferLength);

/** @brief       Returns a descriptive text of a given TPCANStatus error code, in any desired language.
 *
 *  @note        The current languages available for translation are:
 *               Neutral (0x00), German (0x07), English (0x09), Spanish (0x0A), Italian (0x10) and French (0x0C).
 *
 *  @param[in]   Error     A TPCANStatus error code.
 *  @param[in]   Language  Indicates a 'Primary language ID'.
 *  @param[out]  Buffer    Buffer for a null terminated char array.
 *
 *  @returns     A TPCANStatus error code.
 */
TPCANStatus CAN_GetErrorText(
        TPCANStatus Error,
        WORD Language,
        LPSTR Buffer);

#ifdef __cplusplus
}
#endif
#endif	/*__PCAN_API_H */

/*	----------------------------------------------------------------------
 *	Uwe Vogt,  UV Software,  Chausseestrasse 33 A,  10115 Berlin,  Germany
 *	Tel.: +49-30-46799872,  Fax: +49-30-46799873,  Mobile: +49-170-3801903
 *	E-Mail: uwe.vogt@uv-software.de,  Homepage: http://www.uv-software.de/
 */
