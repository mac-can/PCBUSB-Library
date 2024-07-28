//
//  PCBUSB.c
//  MacCAN Monitor
//  Wrapper for libPCBUSB
//
//  Created by Uwe Vogt on 18.08.13.
//  Copyright (c) 2013-2024 UV Software. All rights reserved.
//
//  This software is freeware without any warranty or support!
//
//  NO WARRANTIES: You expressly acknowledge and agree that use of the
//  SOFTWARE PRODUCT is at your sole risk. The SOFTWARE PRODUCT is provided
//  "AS IS" and without warranty of any kind. UV Software hereby EXPRESSLY
//  DISCLAIMS ALL WARRANTIES OR CONDITIONS, EITHER EXPRESS OR IMPLIED,
//  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OR CONDITIONS OF
//  MERCHANTABILITY, NON INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
//  You are solely responsible for determining the appropriateness of using
//  this SOFTWARE PRODUCT and assume all risks associated with the use of
//  this SOFTWARE PRODUCT, including but not limited to the risks of program
//  errors, damage to or loss of data, programs or equipment, and
//  unavailability or interruption of operations.
//
//  NO LIABILITY FOR DAMAGES: UNDER NO CIRCUMSTANCES, INCLUDING NEGLIGENCE,
//  SHALL UV Software BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
//  CONSEQUENTIAL DAMAGES (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS
//  OF BUSINESS PROFITS, BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION,
//  OR ANY OTHER PECUNIARY LOSS) ARISING OUT OF OR RELATING TO THIS LICENSE,
//  EVEN IF UV Software HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
//  In no event shall UV Software's total liability to you for all damages
//  exceed the amount paid for this license to the SOFTWARE PRODUCT. The
//  entire risk arising out of use or performance of the SOFTWARE PRODUCT
//  remains with you.
//
#if defined(__APPLE__)
#include "PCBUSB.h"
#define CAN_LIBRARY  "libPCBUSB.dylib"
#else
#include "PCANBasic.h"
#define CAN_LIBRARY  "libpcanbasic.so"
#endif
#include <stdio.h>
#include <dlfcn.h>
#include <errno.h>
#include <string.h>

typedef TPCANStatus (*CAN_Initialize_t)(TPCANHandle Channel, TPCANBaudrate Btr0Btr1, TPCANType HwType, DWORD IOPort, WORD Interrupt);
typedef TPCANStatus (*CAN_Uninitialize_t)(TPCANHandle Channel);
typedef TPCANStatus (*CAN_Reset_t)(TPCANHandle Channel);
typedef TPCANStatus (*CAN_GetStatus_t)(TPCANHandle Channel);
typedef TPCANStatus (*CAN_Read_t)(TPCANHandle Channel, TPCANMsg* MessageBuffer, TPCANTimestamp* TimestampBuffer);
typedef TPCANStatus (*CAN_Write_t)(TPCANHandle Channel, TPCANMsg* MessageBuffer);
typedef TPCANStatus (*CAN_FilterMessages_t)(TPCANHandle Channel, DWORD FromID, DWORD ToID, TPCANMode Mode);
typedef TPCANStatus (*CAN_GetValue_t)(TPCANHandle Channel, TPCANParameter Parameter, void* Buffer, DWORD BufferLength);
typedef TPCANStatus (*CAN_SetValue_t)(TPCANHandle Channel, TPCANParameter Parameter, void* Buffer, DWORD BufferLength);
typedef TPCANStatus (*CAN_GetErrorText_t)(TPCANStatus Error, WORD Language, char* Buffer);
typedef TPCANStatus (*CAN_InitializeFD_t)(TPCANHandle Channel, TPCANBitrateFD BitrateFD);
typedef TPCANStatus (*CAN_ReadFD_t)(TPCANHandle Channel, TPCANMsgFD* MessageBuffer, TPCANTimestampFD* TimestampBuffer);
typedef TPCANStatus (*CAN_WriteFD_t)(TPCANHandle Channel, TPCANMsgFD* MessageBuffer);
typedef TPCANStatus (*CAN_LookUpChannel_t)(LPSTR Parameters, TPCANHandle* FoundChannel);

static CAN_Initialize_t fpCAN_Initialize = NULL;
static CAN_Uninitialize_t fpCAN_Uninitialize = NULL;
static CAN_Reset_t fpCAN_Reset = NULL;
static CAN_GetStatus_t fpCAN_GetStatus = NULL;
static CAN_Read_t fpCAN_Read = NULL;
static CAN_Write_t fpCAN_Write = NULL;
static CAN_FilterMessages_t fpCAN_FilterMessages = NULL;
static CAN_GetValue_t fpCAN_GetValue = NULL;
static CAN_SetValue_t fpCAN_SetValue = NULL;
static CAN_GetErrorText_t fpCAN_GetErrorText = NULL;
static CAN_InitializeFD_t fpCAN_InitializeFD = NULL;
static CAN_ReadFD_t fpCAN_ReadFD = NULL;
static CAN_WriteFD_t fpCAN_WriteFD = NULL;
static CAN_LookUpChannel_t fpCAN_LookUpChannel = NULL;

static void *hLibrary = NULL;

static int LoadLibrary(void) {
    if (!hLibrary) {
        errno = 0;
        hLibrary = dlopen(CAN_LIBRARY, RTLD_LAZY);
        if (!hLibrary)
            return -1;
        if ((fpCAN_Initialize = (CAN_Initialize_t)dlsym(hLibrary, "CAN_Initialize")) == NULL)
            goto err;
        if ((fpCAN_Uninitialize = (CAN_Uninitialize_t)dlsym(hLibrary, "CAN_Uninitialize")) == NULL)
            goto err;
        if ((fpCAN_Reset = (CAN_Reset_t)dlsym(hLibrary, "CAN_Reset")) == NULL)
            goto err;
        if ((fpCAN_GetStatus = (CAN_GetStatus_t)dlsym(hLibrary, "CAN_GetStatus")) == NULL)
            goto err;
        if ((fpCAN_Read = (CAN_Read_t)dlsym(hLibrary, "CAN_Read")) == NULL)
            goto err;
        if ((fpCAN_Write = (CAN_Write_t)dlsym(hLibrary, "CAN_Write")) == NULL)
            goto err;
        if ((fpCAN_FilterMessages = (CAN_FilterMessages_t)dlsym(hLibrary, "CAN_FilterMessages")) == NULL)
            goto err;
        if ((fpCAN_GetValue = (CAN_GetValue_t)dlsym(hLibrary, "CAN_GetValue")) == NULL)
            goto err;
        if ((fpCAN_SetValue = (CAN_SetValue_t)dlsym(hLibrary, "CAN_SetValue")) == NULL)
            goto err;
        if ((fpCAN_GetErrorText = (CAN_GetErrorText_t)dlsym(hLibrary, "CAN_GetErrorText")) == NULL)
            goto err;
        if ((fpCAN_InitializeFD = (CAN_InitializeFD_t)dlsym(hLibrary, "CAN_InitializeFD")) == NULL)
            goto err;
        if ((fpCAN_ReadFD = (CAN_ReadFD_t)dlsym(hLibrary, "CAN_ReadFD")) == NULL)
            goto err;
        if ((fpCAN_WriteFD = (CAN_WriteFD_t)dlsym(hLibrary, "CAN_WriteFD")) == NULL)
            goto err;
        if ((fpCAN_LookUpChannel = (CAN_LookUpChannel_t)dlsym(hLibrary, "CAN_LookUpChannel")) == NULL)
            fpCAN_LookUpChannel = NULL;  // New function w/o implementation: accept the NULL pointer!
    }
    return 0;
err:
    fpCAN_Initialize = NULL;
    fpCAN_Uninitialize = NULL;
    fpCAN_Reset = NULL;
    fpCAN_GetStatus = NULL;
    fpCAN_Read = NULL;
    fpCAN_Write = NULL;
    fpCAN_FilterMessages = NULL;
    fpCAN_GetValue = NULL;
    fpCAN_SetValue = NULL;
    fpCAN_GetErrorText = NULL;
#ifndef CAN_2_0_ONLY
    fpCAN_InitializeFD = NULL;
    fpCAN_ReadFD = NULL;
    fpCAN_WriteFD = NULL;
#endif
    fpCAN_LookUpChannel = NULL;
    dlclose(hLibrary);
    hLibrary = NULL;
    return -1;
}

TPCANStatus CAN_Initialize(TPCANHandle Channel, TPCANBaudrate Btr0Btr1, TPCANType HwType, DWORD IOPort, WORD Interrupt) {
    if (LoadLibrary() != 0)
        return PCAN_ERROR_NODRIVER;
    if (fpCAN_Initialize)
        return fpCAN_Initialize(Channel, Btr0Btr1, HwType, IOPort, Interrupt);
    else
        return PCAN_ERROR_UNKNOWN;
}

TPCANStatus CAN_Uninitialize(TPCANHandle Channel) {
    //if (LoadLibrary() != 0)
    //    return PCAN_ERROR_NODRIVER;
    if (fpCAN_Uninitialize)
        return fpCAN_Uninitialize(Channel);
    else
        return PCAN_ERROR_UNKNOWN;
}

TPCANStatus CAN_Reset(TPCANHandle Channel) {
    if (LoadLibrary() != 0)
        return PCAN_ERROR_NODRIVER;
    if (fpCAN_Reset)
        return fpCAN_Reset(Channel);
    else
        return PCAN_ERROR_UNKNOWN;
}

TPCANStatus CAN_GetStatus(TPCANHandle Channel) {
    if (LoadLibrary() != 0)
        return PCAN_ERROR_NODRIVER;
    if (fpCAN_GetStatus)
        return fpCAN_GetStatus(Channel);
    else
        return PCAN_ERROR_UNKNOWN;
}

TPCANStatus CAN_Read(TPCANHandle Channel, TPCANMsg* MessageBuffer, TPCANTimestamp* TimestampBuffer) {
    if (LoadLibrary() != 0)
        return PCAN_ERROR_NODRIVER;
    if (fpCAN_Read)
        return fpCAN_Read(Channel, MessageBuffer, TimestampBuffer);
    else
        return PCAN_ERROR_UNKNOWN;
}

TPCANStatus CAN_Write(TPCANHandle Channel, TPCANMsg* MessageBuffer) {
    if (LoadLibrary() != 0)
        return PCAN_ERROR_NODRIVER;
    if (fpCAN_Write)
        return fpCAN_Write(Channel, MessageBuffer);
    else
        return PCAN_ERROR_UNKNOWN;
}

TPCANStatus CAN_FilterMessages(TPCANHandle Channel, DWORD FromID, DWORD ToID, TPCANMode Mode) {
    if (LoadLibrary() != 0)
        return PCAN_ERROR_NODRIVER;
    if (fpCAN_FilterMessages)
        return fpCAN_FilterMessages(Channel, FromID, ToID, Mode);
    else
        return PCAN_ERROR_UNKNOWN;
}

TPCANStatus CAN_GetValue(TPCANHandle Channel, TPCANParameter Parameter, void* Buffer, DWORD BufferLength) {
    if (LoadLibrary() != 0)
        return PCAN_ERROR_NODRIVER;
    if (fpCAN_GetValue)
        return fpCAN_GetValue(Channel, Parameter, Buffer, BufferLength);
    else
        return PCAN_ERROR_UNKNOWN;
}

TPCANStatus CAN_SetValue(TPCANHandle Channel, TPCANParameter Parameter, void* Buffer, DWORD BufferLength) {
    if (LoadLibrary() != 0)
        return PCAN_ERROR_NODRIVER;
    if (fpCAN_SetValue)
        return fpCAN_SetValue(Channel, Parameter, Buffer, BufferLength);
    else
        return PCAN_ERROR_UNKNOWN;
}

TPCANStatus CAN_GetErrorText(TPCANStatus Error, WORD Language, char* Buffer) {
    if (LoadLibrary() != 0) {
        if (Buffer)
            strcpy(Buffer, "PCBUSB library could not be loaded");
        return PCAN_ERROR_NODRIVER;
    }
    if (fpCAN_GetErrorText)
        return fpCAN_GetErrorText(Error, Language, Buffer);
    else
        return PCAN_ERROR_UNKNOWN;
}

TPCANStatus CAN_InitializeFD(TPCANHandle Channel, TPCANBitrateFD BitrateFD) {
    if (LoadLibrary() != 0)
        return PCAN_ERROR_NODRIVER;
    if (fpCAN_InitializeFD)
        return fpCAN_InitializeFD(Channel, BitrateFD);
    else
        return PCAN_ERROR_UNKNOWN;
}

TPCANStatus CAN_ReadFD(TPCANHandle Channel, TPCANMsgFD* MessageBuffer, TPCANTimestampFD* TimestampBuffer) {
    if (LoadLibrary() != 0)
        return PCAN_ERROR_NODRIVER;
    if (fpCAN_ReadFD)
        return fpCAN_ReadFD(Channel, MessageBuffer, TimestampBuffer);
    else
        return PCAN_ERROR_UNKNOWN;
}

TPCANStatus CAN_WriteFD(TPCANHandle Channel, TPCANMsgFD* MessageBuffer) {
    if (LoadLibrary() != 0)
        return PCAN_ERROR_NODRIVER;
    if (fpCAN_WriteFD)
        return fpCAN_WriteFD(Channel, MessageBuffer);
    else
        return PCAN_ERROR_UNKNOWN;
}

TPCANStatus CAN_LookUpChannel(LPSTR Parameters, TPCANHandle* FoundChannel) {
    if (LoadLibrary() != 0)
        return PCAN_ERROR_NODRIVER;
    if (fpCAN_LookUpChannel)
        return fpCAN_LookUpChannel(Parameters, FoundChannel);
    else
        return PCAN_ERROR_UNKNOWN;
}
