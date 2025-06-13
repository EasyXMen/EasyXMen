/* PRQA S 3108++ */
/**
 * Copyright (C) 2008-2025 isoft Infrastructure Software Co., Ltd.
 * SPDX-License-Identifier: LGPL-2.1-only-with-exception
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the
 * GNU Lesser General Public License as published by the Free Software Foundation; version 2.1.
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License along with this library;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * or see <https://www.gnu.org/licenses/>.
 */
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : Com_Internal.c                                              **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : COM internal API definitions                                **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Com_Internal.h"
/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/
#if (COM_TXSIGNALGROUP_NUMBER > 0u)
#define COM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Com_MemMap.h"
VAR(Com_TxSignalGroupRunTimeStateType, COM_VAR) Com_TxSignalGroupState[COM_TXSIGNALGROUP_NUMBER];
#define COM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Com_MemMap.h"
#endif

#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE)
#define COM_START_SEC_VAR_NO_INIT_16
#include "Com_MemMap.h"
VAR(uint16, COM_VAR) Com_RxSignalTimeOutCnt[COM_RXSIG_TIMEOUT_NUMBER];
#define COM_STOP_SEC_VAR_NO_INIT_16
#include "Com_MemMap.h"
#endif

#if (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)
#define COM_START_SEC_VAR_NO_INIT_16
#include "Com_MemMap.h"
VAR(uint16, COM_VAR) Com_RxSignalGroupTimeOutCnt[COM_RXSIGGRP_TIMEOUT_NUMBER];
#define COM_STOP_SEC_VAR_NO_INIT_16
#include "Com_MemMap.h"
#endif

#if (COM_ONEEVERYNFILTERSIGNAL_NUMBER > 0u)
#define COM_START_SEC_VAR_NO_INIT_32
#include "Com_MemMap.h"
VAR(uint32, COM_VAR) Com_OneEveryNcnt[COM_ONEEVERYNFILTERSIGNAL_NUMBER];
#define COM_STOP_SEC_VAR_NO_INIT_32
#include "Com_MemMap.h"
#endif

#if (COM_RXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
#define COM_START_SEC_VAR_NO_INIT_BOOLEAN
#include "Com_MemMap.h"
VAR(boolean, COM_VAR) Com_MaskNewDifferMaskOldTimeOut[COM_RXMASKNEWDIFFERMASKOLD_NUMBER];
#define COM_STOP_SEC_VAR_NO_INIT_BOOLEAN
#include "Com_MemMap.h"
#endif
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
/* Called by Com_SignalUnPackHandle
 * for Endian convert,Swap byte mirror */
static void Com_EndianSwap(void* dataPtr, uint8 size);
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/******************************************************************************/
/*
 * Brief               Called by Com_SignalUnPackHandle
 *                     for Endian convert,Swap byte mirror
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      size
 * Param-Name[out]     None
 * Param-Name[in/out]  dataPtr
 * Return              None
 */
/******************************************************************************/
/* PRQA S 3673 ++ */ /* MISRA Rule 8.13 */
static void Com_EndianSwap(void* dataPtr, uint8 size)
/* PRQA S 3673 -- */ /* MISRA Rule 8.13 */
{
    uint8* data = (uint8*)dataPtr;
    uint8 i;
    uint8 temp;
    uint8 loopEndIdx = size / 2u;
    uint8 arrayEndIdx = size - 1u;
    uint8 offset;

    for (i = 0u; i < loopEndIdx; ++i)
    {
        offset = arrayEndIdx - i;
        temp = data[offset];
        data[offset] = data[i];
        data[i] = temp;
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/******************************************************************************/
/*
 * Brief               Init Runtime TMC of all tx signals and tx group siganls
 *                     and GW dest signals,which the comfilter is not NULL
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
FUNC(void, COM_CODE)
Com_TxSignalTMCStateInit(void)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
#if (0u < COM_TMCTXSIGNAL_NUMBER)
#if (0u < COM_TXGRPSIG_FILTERTYPE_MAX_NUMBER)
    (void)ILib_memcpy(&Com_TxGroupSignalTMCBuffer[0], &Com_TxSignalInitTMC[0], sizeof(Com_TxGroupSignalTMCBuffer));
#endif
    (void)ILib_memcpy(&Com_TxSignalTMCRunTime[0], &Com_TxSignalInitTMC[0], COM_TMCTXSIGNAL_NUMBER);
#endif /* 0u < COM_TMCTXSIGNAL_NUMBER */
}

#if (0u < COM_TXSIGNALGROUP_NUMBER)
/******************************************************************************/
/*
 * Brief               set all tx signal group state to not need to
 *                     trigger transmit
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
FUNC(void, COM_CODE)
Com_TxSignalGroupTriggerFlagInit(void)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    Com_TxSignalGroupRunTimeStateType* Com_TxSigGrpStPtr;
    for (uint16 cirCnt = 0u; cirCnt < COM_TXSIGNALGROUP_NUMBER; ++cirCnt) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
    {
        Com_TxSigGrpStPtr = &Com_TxSignalGroupState[cirCnt];
        Com_TxSigGrpStPtr->TxSigGrpRTStFlag &= COM_NTIMESTRANSMITNEED_DIS;
    }
}
#endif

#if ((0u < COM_IPDUGROUP_NUMBER) || (0u < COM_RXIPDU_NUMBER))
/******************************************************************************/
/*
 * Brief               init signal/group signal buffer
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalType,SignalInitValueId,SignalLength
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, COM_CODE)
Com_InitSignalBuffer(Com_SignalType SignalType, uint16 SignalInitValueId, uint16 SignalLength)
{
    SchM_Enter_Com_Context();
    /* PRQA S 3315 ++ */ /* MISRA Rule 16.6 */
    switch (SignalType)
    {
/*According to the signal type,
 *Copy the init signal value to signal runtime buffer*/
#if (0u < COM_SIGNAL_BOOLBUFF_SIZE)
    case COM_BOOLEAN:
        Com_SignalBoolRuntimeBuff[SignalInitValueId] = Com_SignalBoolInitValue[SignalInitValueId];
        break;
#endif
#if (0u < COM_SIGNAL_8BITBUFF_SIZE)
    case COM_UINT8_N:
    case COM_UINT8_DYN:
        (void)ILib_memcpy(
            &Com_Signal8BitRuntimeBuff[SignalInitValueId],
            &Com_Signal8BitInitValue[SignalInitValueId],
            SignalLength);
        break;
    case COM_UINT8:
    case COM_SINT8:
        Com_Signal8BitRuntimeBuff[SignalInitValueId] = Com_Signal8BitInitValue[SignalInitValueId];
        break;
#endif
#if (0u < COM_SIGNAL_16BITBUFF_SIZE)
    case COM_UINT16:
    case COM_SINT16:
        Com_Signal16BitRuntimeBuff[SignalInitValueId] = Com_Signal16BitInitValue[SignalInitValueId];
        break;
#endif
#if (0u < COM_SIGNAL_32BITBUFF_SIZE)
    case COM_UINT32:
    case COM_SINT32:
    case COM_FLOAT32:
        Com_Signal32BitRuntimeBuff[SignalInitValueId] = Com_Signal32BitInitValue[SignalInitValueId];
        break;
#endif
#if (0u < COM_SIGNAL_64BITBUFF_SIZE)
    case COM_UINT64:
    case COM_SINT64:
    case COM_FLOAT64:
        Com_Signal64BitRuntimeBuff[SignalInitValueId] = Com_Signal64BitInitValue[SignalInitValueId];
        break;
#endif
    default:
        /*do nothing*/
        break;
    }
    SchM_Exit_Com_Context();
    /* PRQA S 3315 -- */ /* MISRA Rule 16.6 */
    COM_NOUSED(SignalInitValueId);
    COM_NOUSED(SignalLength);
}
#endif

#if (0u < COM_RXIPDUBUFF_SIZE) || ((STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && (COM_TXSIGNALGROUP_NUMBER > 0u))
/******************************************************************************/
/*
 * Brief               unpack all byte value(group signal/signal cover,except MSB byte).
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SigPackInfoPtr,IPduBufferPtr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint64
 */
/******************************************************************************/
FUNC(uint64, COM_CODE)
Com_SignalUnPackHandle(
#if (STD_ON == COM_SIGNAL_SIGNED_TYPE_ENABLE)
    Com_SignalType SignalType,
#endif
    Com_SignalConfigType SignalConfigType,
    const void* SignalPtr,
    const uint8* IPduBufferPtr)

{
#if (0u < COM_RXSIGNAL_NUMBER)
    const Com_RxSignalType* rxSignalPtr;
#endif
#if ((0u < COM_RXGROUPSIGNAL_NUMBER) && (0u < COM_RXSIGNALGROUP_NUMBER))
    const Com_RxGroupSignalType* rxGroupSignalPtr;
#endif
#if (0u < COM_GW_SOURCE_DESCRIPTION_NUMBER)
    const Com_GwSourceSignalType* gwSourceSignalPtr;
#endif
#if ((0u < COM_TXGROUPSIGNAL_NUMBER) && (0u < COM_TXSIGNALGROUP_NUMBER))
    const Com_TxGroupSignalType* txGroupSignalPtr;
#endif
    uint64 signalValue = 0u;
    uint16 srcStartIdx;
    uint16 destStartIdx = 0u;
    Com_SignalPositionType sigLsbBytePos;
    uint8 lsbPosInByte;
    uint8 signalByteLength;
    uint8 bitSize;
    Com_SignalEndiannessType signalEndianness;
    boolean isTrue = FALSE;

    switch (SignalConfigType)
    {
#if (0u < COM_RXSIGNAL_NUMBER)
    case COM_RX_SIGNAL:
        rxSignalPtr = (const Com_RxSignalType*)SignalPtr;
        sigLsbBytePos = rxSignalPtr->ComSigLsbBytePos;
        lsbPosInByte = rxSignalPtr->ComSignalPosInByte;
        bitSize = rxSignalPtr->ComBitSize;
        signalByteLength = rxSignalPtr->ComSignalByteLength;
        signalEndianness = rxSignalPtr->ComSignalEndianness;
        isTrue = TRUE;
        break;
#endif
#if ((0u < COM_RXGROUPSIGNAL_NUMBER) && (0u < COM_RXSIGNALGROUP_NUMBER))
    case COM_RX_GROUP_SIGNAL:
        rxGroupSignalPtr = (const Com_RxGroupSignalType*)SignalPtr;
        sigLsbBytePos = rxGroupSignalPtr->ComSigLsbBytePos;
        lsbPosInByte = rxGroupSignalPtr->ComSignalPosInByte;
        bitSize = rxGroupSignalPtr->ComBitSize;
        signalByteLength = rxGroupSignalPtr->ComSignalByteLength;
        signalEndianness = rxGroupSignalPtr->ComSignalEndianness;
        isTrue = TRUE;
        break;
#endif
#if (0u < COM_GW_SOURCE_DESCRIPTION_NUMBER)
    case COM_RX_DESCRIPTION:
        gwSourceSignalPtr = (const Com_GwSourceSignalType*)SignalPtr;
        sigLsbBytePos = gwSourceSignalPtr->ComSigLsbBytePos;
        lsbPosInByte = gwSourceSignalPtr->ComSignalPosInByte;
        bitSize = gwSourceSignalPtr->ComBitSize;
        signalByteLength = gwSourceSignalPtr->ComSignalByteLength;
        signalEndianness = gwSourceSignalPtr->ComSignalEndianness;
        isTrue = TRUE;
        break;
#endif
#if ((0u < COM_TXGROUPSIGNAL_NUMBER) && (0u < COM_TXSIGNALGROUP_NUMBER))
    case COM_TX_GROUP_SIGNAL:
        txGroupSignalPtr = (const Com_TxGroupSignalType*)SignalPtr;
        sigLsbBytePos = txGroupSignalPtr->ComSigLsbBytePos;
        lsbPosInByte = txGroupSignalPtr->ComSignalPosInByte;
        bitSize = txGroupSignalPtr->ComBitSize;
        signalByteLength = txGroupSignalPtr->ComSignalByteLength;
        signalEndianness = txGroupSignalPtr->ComSignalEndianness;
        isTrue = TRUE;
        break;
#endif
    default:
        /* do nothing */
        break;
    }
    if (isTrue)
    {

        uint8 tSignalByteLength = (signalByteLength > 8u) ? 8u : signalByteLength;
#if (TRUE == CPU_32_WITH_16_ADR)
        uint8 bufPtr[8u] = {0u};
#elif (FALSE == CPU_32_WITH_16_ADR)
        uint8* bufPtr = (uint8*)&signalValue;
#if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
        destStartIdx = (uint16)(8u - tSignalByteLength);
#endif
#endif
        srcStartIdx = (COM_BIG_ENDIAN == signalEndianness)
                          ? ((uint16)sigLsbBytePos - (uint16)tSignalByteLength + (uint16)1u)
                          : (uint16)sigLsbBytePos;

        SchM_Enter_Com_Context();
        (void)ILib_memcpy(&bufPtr[destStartIdx], &IPduBufferPtr[srcStartIdx], tSignalByteLength);
        SchM_Exit_Com_Context();

#if (TRUE == CPU_32_WITH_16_ADR)
        if (COM_BIG_ENDIAN == signalEndianness)
        {
            Com_EndianSwap(&bufPtr[destStartIdx], tSignalByteLength);
        }
        for (uint8 i = ((uint16)tSignalByteLength - 1u); i > 0u; --i)
        {
            signalValue |= (uint64)bufPtr[i];
            signalValue = signalValue << 8u;
        }
        signalValue |= (uint64)(bufPtr[0u]);
#elif (FALSE == CPU_32_WITH_16_ADR)
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST) /* Little endian byte order */
        if (COM_BIG_ENDIAN == signalEndianness)
        {
            Com_EndianSwap(&bufPtr[destStartIdx], tSignalByteLength);
        }
#else
        if (COM_LITTLE_ENDIAN == signalEndianness)
        {
            Com_EndianSwap(&bufPtr[destStartIdx], tSignalByteLength);
        }
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */
#endif

        signalValue = (signalValue >> lsbPosInByte) & (COM_UNUSED_UINT64 >> (64u - bitSize));

        /* SignalByteLength range 1..9 */
        if (signalByteLength == 9u)
        {
            /* Handle MSB byte, MSB Byte is byte 0(BigEndian) or byte 8(LittleEndian) */
            uint8 msbByteBitSize = (uint8)(lsbPosInByte + (bitSize & 0x07u));
            SchM_Enter_Com_Context();
            uint64 msbByteValue =
                (COM_BIG_ENDIAN == signalEndianness) ? IPduBufferPtr[sigLsbBytePos - 8u] : IPduBufferPtr[sigLsbBytePos];
            SchM_Exit_Com_Context();
            msbByteValue &= ((uint64)COM_UNUSED_UINT8 >> (8u - msbByteBitSize));
            signalValue |= (msbByteValue << (bitSize - msbByteBitSize));
        }

        /* Flag extension */
#if (STD_ON == COM_SIGNAL_SIGNED_TYPE_ENABLE)
        if (((COM_SINT8 == SignalType) || (COM_SINT16 == SignalType) || (COM_SINT32 == SignalType)
             || (COM_SINT64 == SignalType))
            && ((signalValue >> (bitSize - 1u)) > 0u))
        {
            signalValue = (COM_UNUSED_UINT64 << bitSize) | signalValue;
        }
#endif /* STD_ON == COM_SIGNAL_SIGNED_TYPE_ENABLE */
    }

    return signalValue;
}
#endif

#if ((0u < COM_TXSIGNALGROUP_NUMBER) || (0u < COM_TXSIGNAL_NUMBER) || (0u < COM_GWMAPPING_NUMBER))
/******************************************************************************/
/*
 * Brief               Called by Com_TxSignalPack,Com_TxGroupSignalPack
 *                     pack signal which signal type is not boolean,COM_UINT8_N,
 *                     COM_UINT8_DYN,and the signal value bits cover 2-9 bytes
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalConfigType,TxSignalPtr,TxValue
 * Param-Name[out]     IPduBufferPtr
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, COM_CODE)
Com_TxSignalPackHandle(
    Com_SignalConfigType SignalConfigType,
    const void* TxSignalPtr,
    uint64 TxValue,
    uint8* IPduBufferPtr)
{
#if (0u < COM_TXIPDUBUFF_SIZE)
#if (0u < COM_TXSIGNAL_NUMBER)
    const Com_TxSignalType* txSignalPtr;
#endif
#if ((0u < COM_TXGROUPSIGNAL_NUMBER) && (0u < COM_TXSIGNALGROUP_NUMBER))
    const Com_TxGroupSignalType* txGroupSignalPtr;
#endif
#if (0u < COM_GW_DESTINATION_DESCRIPTION_NUMBER)
    const Com_GwDestSignalType* gwDestSignalPtr;
#endif
    uint8 msbByteValue;
    Com_SignalPositionType sigLsbBytePos;
    uint16 srcStartIdx = 0u;
    uint16 destStartIdx;
    uint8 lsbPosInByte;
    uint64 signalValue;
    uint8 msbByteBitSize;
    uint8 msbByteHighBitMask;
    uint8 signalByteLength;
    uint8 bitSize;
    Com_SignalEndiannessType signalEndianness;
    boolean isTrue = FALSE;

    switch (SignalConfigType)
    {
#if (0u < COM_TXSIGNAL_NUMBER)
    case COM_TX_SIGNAL:
        txSignalPtr = (const Com_TxSignalType*)TxSignalPtr;
        sigLsbBytePos = txSignalPtr->ComSigLsbBytePos;
        lsbPosInByte = txSignalPtr->ComSignalPosInByte;
        bitSize = txSignalPtr->ComBitSize;
        signalByteLength = txSignalPtr->ComSignalByteLength;
        signalEndianness = txSignalPtr->ComSignalEndianness;
        isTrue = TRUE;
        break;
#endif
#if ((0u < COM_TXGROUPSIGNAL_NUMBER) && (0u < COM_TXSIGNALGROUP_NUMBER))
    case COM_TX_GROUP_SIGNAL:
        txGroupSignalPtr = (const Com_TxGroupSignalType*)TxSignalPtr;
        sigLsbBytePos = txGroupSignalPtr->ComSigLsbBytePos;
        lsbPosInByte = txGroupSignalPtr->ComSignalPosInByte;
        bitSize = txGroupSignalPtr->ComBitSize;
        signalByteLength = txGroupSignalPtr->ComSignalByteLength;
        signalEndianness = txGroupSignalPtr->ComSignalEndianness;
        isTrue = TRUE;
        break;
#endif
#if (0u < COM_GW_DESTINATION_DESCRIPTION_NUMBER)
    case COM_TX_DESCRIPTION:
        gwDestSignalPtr = (const Com_GwDestSignalType*)TxSignalPtr;
        sigLsbBytePos = gwDestSignalPtr->ComSigLsbBytePos;
        lsbPosInByte = gwDestSignalPtr->ComSignalPosInByte;
        bitSize = gwDestSignalPtr->ComBitSize;
        signalByteLength = gwDestSignalPtr->ComSignalByteLength;
        signalEndianness = gwDestSignalPtr->ComSignalEndianness;
        isTrue = TRUE;
        break;
#endif
    default:
        /* do nothing */
        break;
    }

    if (isTrue)
    {
        signalValue = TxValue << lsbPosInByte;
#if (TRUE == CPU_32_WITH_16_ADR)
        uint8 bufPtr[8u] = {0u};
        for (uint8 i = 0u; i < tSignalByteLength; ++i)
        {
            bufPtr[i] = (uint8)(signalValue & COM_UNUSED_UINT8);
            signalValue = signalValue >> 8u;
        }
#elif (FALSE == CPU_32_WITH_16_ADR)
        uint8* bufPtr = (uint8*)&signalValue;
#endif
        uint16 sigMsbBytePosBE = 1u + (uint16)sigLsbBytePos - (uint16)signalByteLength;
        uint16 sigMsbBytePosOt = (uint16)sigLsbBytePos + (uint16)signalByteLength - 1u;
        uint16 sigMsbBytePos = (COM_BIG_ENDIAN == signalEndianness) ? sigMsbBytePosBE : sigMsbBytePosOt;
        uint8 tSignalByteLength = (signalByteLength < 9u) ? signalByteLength : 8u;

/* LSB Byte */
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST) /* Little endian byte order */
        bufPtr[0u] |= (IPduBufferPtr[sigLsbBytePos] & (COM_UNUSED_UINT8 >> (8u - lsbPosInByte)));
#else
        bufPtr[7u] |= (IPduBufferPtr[sigLsbBytePos] & (COM_UNUSED_UINT8 >> (8u - lsbPosInByte)));
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */
        /* MSB Byte*/
        msbByteBitSize = (uint8)((lsbPosInByte + bitSize) & 0x07u);
        if (0u != msbByteBitSize)
        {
            msbByteHighBitMask = (uint8)(COM_UNUSED_UINT8 << msbByteBitSize);
            /* SignalByteLength range 1..9 */
            if (signalByteLength < 9u)
            {
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST) /* Little endian byte order */
                bufPtr[signalByteLength - 1u] |= (IPduBufferPtr[sigMsbBytePos] & msbByteHighBitMask);
#else
                bufPtr[8 - signalByteLength] |= (IPduBufferPtr[sigMsbBytePos] & msbByteHighBitMask);
#endif
            }
            else
            {
                msbByteValue = (uint8)(TxValue >> (bitSize - msbByteBitSize));
                IPduBufferPtr[sigMsbBytePos] =
                    (IPduBufferPtr[sigMsbBytePos] & msbByteHighBitMask) | (uint8)msbByteValue;
            }
        }
#if (TRUE == CPU_32_WITH_16_ADR)
        if (COM_BIG_ENDIAN == signalEndianness)
        {
            Com_EndianSwap(&bufPtr[srcStartIdx], tSignalByteLength);
        }
#elif (FALSE == CPU_32_WITH_16_ADR)
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST) /* Little endian byte order */
        if (COM_BIG_ENDIAN == signalEndianness)
        {
            Com_EndianSwap(&bufPtr[srcStartIdx], tSignalByteLength);
        }
#else
        srcStartIdx = 8u - tSignalByteLength;
        if (COM_LITTLE_ENDIAN == signalEndianness)
        {
            Com_EndianSwap(&bufPtr[srcStartIdx], tSignalByteLength);
        }
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */
#endif
        destStartIdx = (COM_BIG_ENDIAN == signalEndianness)
                           ? ((uint16)1u + (uint16)sigLsbBytePos - (uint16)tSignalByteLength)
                           : (uint16)sigLsbBytePos;

        SchM_Enter_Com_Context();
        (void)ILib_memcpy(&IPduBufferPtr[destStartIdx], &bufPtr[srcStartIdx], tSignalByteLength);
        SchM_Exit_Com_Context();
    }
#endif
}
#endif

#if (COM_TXNEWISWITHIN_NUMBER > 0u) || (COM_RXNEWISWITHIN_NUMBER > 0u) || (COM_RXNEWISOUTSIDE_NUMBER > 0u) \
    || (COM_TXNEWISOUTSIDE_NUMBER > 0U)
/******************************************************************************/
/*
 * Brief               Called by Com_TxSignalTMCCalculate,
 *                     Com_DestSignalTMCCalculate,Com_RxSignalFilter.
 *                     when FilterAlgorithm is NEW_IS_WITHIN or NEW_IS_WITHOUT,calculate
 *                     the signal's TMC
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalType,SignalValue,ComFilter,Within
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(boolean, COM_CODE)
Com_NewIsWInWOtFilterCalculate(
    Com_SignalType SignalType,
    uint64 SignalValue,
    Com_FilterMaxType ComFilterMax,
    Com_FilterMinType ComFilterMin,
    boolean Within)
{
    /* FilterAlgorithm is NEW_IS_WITHIN , input parameter Within is TRUE,FilterAlgorithm is NEW_IS_WITHOUT, input
     * parameter Within is FALSE */
    boolean IsWith = FALSE;
    switch (SignalType)
    {
    /*According to the signal type,
     *Calculated the signal value is filtered or not?*/
    case COM_SINT8:
        if ((((sint8)(SignalValue)) <= ((sint8)(ComFilterMax))) && (((sint8)(SignalValue)) >= ((sint8)(ComFilterMin))))
        {
            IsWith = TRUE;
        }
        break;
    case COM_SINT16:
        if ((((sint16)(SignalValue)) <= ((sint16)(ComFilterMax)))
            && (((sint16)(SignalValue)) >= ((sint16)(ComFilterMin))))
        {
            IsWith = TRUE;
        }
        break;
    case COM_SINT32:
        if ((((sint32)(SignalValue)) <= ((sint32)(ComFilterMax)))
            && (((sint32)(SignalValue)) >= ((sint32)(ComFilterMin))))
        {
            IsWith = TRUE;
        }
        break;
    case COM_SINT64:
        if ((((sint64)(SignalValue)) <= ((sint64)(ComFilterMax)))
            && (((sint64)(SignalValue)) >= ((sint64)(ComFilterMin))))
        {
            IsWith = TRUE;
        }
        break;
    case COM_UINT8:
    case COM_UINT16:
    case COM_UINT32:
    case COM_UINT64:
        if ((SignalValue <= ComFilterMax) && (SignalValue >= ComFilterMin))
        {
            IsWith = TRUE;
        }
        break;
    default:
        /* error */
        IsWith = !Within;
        break;
    }

    return Within == IsWith;
}
#endif

#if (0u < COM_TMCTXSIGNAL_NUMBER)
/******************************************************************************/
/*
 * Brief               Called by Com_GwNotArraySignal.
 *                     calculate dest signal/group signal/description signal TMC
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalType,ComFilter,NewSignalValue,OldSignalValue
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/******************************************************************************/
FUNC(boolean, COM_CODE)
Com_DestSignalTMCCalculate(
    Com_SignalType SignalType,
    Com_FilterAlgorithmType FilterType,
    Com_SignalIdType ComFilterIndex,
#if (COM_TXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
    uint64 OldSignalValue,
#endif
    uint64 NewSignalValue)
{
    boolean ret = FALSE;
#if (COM_TXMASKNEWEQUALSX_NUMBER > 0u) || (COM_TXMASKNEWDIFFERX_NUMBER > 0u) || (COM_TXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
    Com_FilterMaskType ComFilterMask;
    Com_FilterXType ComFilterX;
#endif
#if (COM_TXNEWISWITHIN_NUMBER > 0u) || (COM_TXNEWISOUTSIDE_NUMBER > 0u)
    Com_FilterMaxType ComFilterMax;
    Com_FilterMinType ComFilterMin;
#endif
#if (COM_ONEEVERYNFILTERSIGNAL_NUMBER > 0u)
    Com_SignalIdType ComFilterOffset;
    Com_SignalIdType ComFilterPeriod;
#endif

    switch (FilterType)
    {
    case COM_ALWAYS:
        ret = TRUE;
        break;
#if (COM_TXMASKNEWEQUALSX_NUMBER > 0u)
    case COM_MASKED_NEW_EQUALS_X:
        ComFilterMask = Com_TxMaskedNewEqualsX[ComFilterIndex].ComFilterMask;
        ComFilterX = Com_TxMaskedNewEqualsX[ComFilterIndex].ComFilterX;
        if ((NewSignalValue & ComFilterMask) == ComFilterX)
        {
            ret = TRUE;
        }
        break;
#endif
#if (COM_TXMASKNEWDIFFERX_NUMBER > 0u)
    case COM_MASKED_NEW_DIFFERS_X:
        ComFilterMask = Com_TxMaskedNewDiffersX[ComFilterIndex].ComFilterMask;
        ComFilterX = Com_TxMaskedNewDiffersX[ComFilterIndex].ComFilterX;
        if ((NewSignalValue & ComFilterMask) != ComFilterX)
        {
            ret = TRUE;
        }
        break;
#endif
        /*COM_BOOLEAN not support WITHIN and WITHOUT*/
#if (COM_TXNEWISWITHIN_NUMBER > 0u)
    case COM_NEW_IS_WITHIN:
        ComFilterMax = Com_TxNewIsWithin[ComFilterIndex].ComFilterMax;
        ComFilterMin = Com_TxNewIsWithin[ComFilterIndex].ComFilterMin;
        ret = Com_NewIsWInWOtFilterCalculate(SignalType, NewSignalValue, ComFilterMax, ComFilterMin, TRUE);
        break;
#endif
        /*COM_BOOLEAN not support WITHIN and WITHOUT*/
#if (COM_TXNEWISOUTSIDE_NUMBER > 0u)
    case COM_NEW_IS_OUTSIDE:
        ComFilterMax = Com_TxNewIsOutside[ComFilterIndex].ComFilterMax;
        ComFilterMin = Com_TxNewIsOutside[ComFilterIndex].ComFilterMin;
        ret = Com_NewIsWInWOtFilterCalculate(SignalType, NewSignalValue, ComFilterMax, ComFilterMin, FALSE);
        break;
#endif
#if (COM_ONEEVERYNFILTERSIGNAL_NUMBER > 0u)
    case COM_ONE_EVERY_N:
        ComFilterOffset = Com_FilterOneEveryN[ComFilterIndex].ComFilterOffset;
        ComFilterPeriod = Com_FilterOneEveryN[ComFilterIndex].ComFilterPeriod;
        if (ComFilterPeriod == Com_OneEveryNcnt[ComFilterIndex])
        {
            Com_OneEveryNcnt[ComFilterIndex] = 0u;
        }
        if (ComFilterOffset == Com_OneEveryNcnt[ComFilterIndex])
        {
            ret = TRUE;
        }
        Com_OneEveryNcnt[ComFilterIndex]++;
        break;
#endif
#if (COM_TXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
    case COM_MASKED_NEW_DIFFERS_MASKED_OLD:
        ComFilterMask = Com_TxMaskedNewDiffersMaskedOld[ComFilterIndex];
        if ((NewSignalValue & ComFilterMask) != (OldSignalValue & ComFilterMask))
        {
            ret = TRUE;
        }
        break;
#endif
    default:
        /*do nothing*/
        break;
    }
    return ret;
}
#endif

#if (                                                                                           \
    (0u < COM_TXSIGNAL_NUMBER) || ((0u < COM_TXSIGNALGROUP_NUMBER) && (0u < COM_TXIPDU_NUMBER)) \
    || (0u < COM_GWMAPPING_NUMBER))                                                             \
    && (STD_ON == COM_TMS_ENABLE)
/******************************************************************************/
/*
 * Brief               Called by Com_TxSignalTMHandle,Com_TxIpduTMSHandle.
 *                     calculate the tx ipdu TMS(signal/Group signal/Dest signal)
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxIpduPtr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/******************************************************************************/
FUNC(boolean, COM_CODE)
Com_TxIpduTMSCalculate(const Com_TxIPduType* TxIpduPtr)
{
    boolean ret = TRUE;
#if (0u < COM_TMCTXSIGNAL_NUMBER)
    /*just configuration two TxMode,need to Calculate the TMS of TxPdu.
     *ComTxModeTrue must be configured,ComTxModeFalse select configuration*/
    if (COM_UNUSED_UINT16 != TxIpduPtr->ComTxModeFalseRefId)
    {
        ret = FALSE;
#if (0u < COM_TXSIGNAL_FILTERTYPE_MAX_NUMBER)
        Com_TxSignalIdType txSignalNumber, cnt;
        const Com_TxSignalType* ComTxSignalPtr;
        /*Calculate TMS based on all signals contained in this TxPdu*/
        txSignalNumber = TxIpduPtr->ComIPduSignalsRefNumber;
        cnt = TxIpduPtr->ComIpduSignalRefStartId;
        for (; cnt < txSignalNumber; ++cnt)
        {
            ComTxSignalPtr = &Com_ConfigStd->ComTxSignal[cnt];
            if ((COM_UNUSED_TXSIGNALFILTERID != ComTxSignalPtr->ComFilterIndex)
                && (Com_TxSignalTMCRunTime[ComTxSignalPtr->ComTMCBufferId]))
            {
                ret = TRUE;
                break;
            }
        }
#endif
        if (!ret)
        {
#if (0u < COM_DESTSIG_FILTERTYPE_MAX_NUMBER)
            Com_GwDestinationDescriptionIdType destSignalNumber, destSignalId;
            const Com_GwDestSignalType* ComDestSignalPtr;
            /*Calculate TMS based on all dest description signals contained in this TxPdu*/
            destSignalNumber = TxIpduPtr->ComIPduDespSignalsRefNumber;
            destSignalId = TxIpduPtr->ComIPduDespSignalsRefStartId;
            for (; destSignalId < destSignalNumber; ++destSignalId)
            {
                ComDestSignalPtr = &Com_ConfigStd->ComDestSignal[destSignalId];
                if ((COM_UNUSED_DESTSIGNALFILTERID != ComDestSignalPtr->ComFilterIndex)
                    && (Com_TxSignalTMCRunTime[ComDestSignalPtr->ComTMCBufferId]))
                {
                    ret = TRUE;
                    break;
                }
            }
#endif
        }
        if (!ret)
        {
#if (0u < COM_TXGRPSIG_FILTERTYPE_MAX_NUMBER)
            Com_TxSignalGroupIdType signalGroupNumber, signalGroupId;
            Com_TxGroupSignalIdType groupSignalNumber, counter;
            const Com_TxGroupSignalType* ComTxGroupSignalPtr;
            const Com_TxSignalGroupType* txSignalGroupPtr;
            /*Calculate TMS based on all group signals contained in this TxPdu*/
            signalGroupNumber = TxIpduPtr->ComIPduSignalGroupsRefNumber;
            signalGroupId = TxIpduPtr->ComIPduSignalGroupsRefStartId;
            for (; signalGroupId < signalGroupNumber; ++signalGroupId)
            {
                txSignalGroupPtr = &Com_ConfigStd->ComTxSignalGroup[signalGroupId];
                groupSignalNumber = txSignalGroupPtr->ComGroupSignalRefNumber;
                counter = txSignalGroupPtr->ComGroupSignalRefStartId;
                for (; (counter < groupSignalNumber) && (!ret); ++counter)
                {
                    ComTxGroupSignalPtr = &Com_ConfigStd->ComTxGroupSignal[counter];
                    if ((COM_UNUSED_TXGRPSIGFILTERID != ComTxGroupSignalPtr->ComFilterIndex)
                        && (Com_TxSignalTMCRunTime[ComTxGroupSignalPtr->ComTMCBufferId]))
                    {
                        ret = TRUE;
                        break;
                    }
                }
            }
#endif
        }
    }
#endif
    COM_NOUSED(TxIpduPtr);
    return ret;
}
#endif

#if (0u < COM_TXIPDU_NUMBER) || (0u < COM_GWMAPPING_NUMBER)
/******************************************************************************/
/*
 * Brief               Called by Com_SendSignalGroupArray,Com_SendSignalGroup,
 *                     Com_GwUint8NDYNSignal,Com_GwNotArraySignal.
 *                     calculate the TMS,store new Tx mode,get rptNum value for
 *                     DIRECT/MIXED tx mode,if TMS changed will reset periodic mode counter
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxIpduStatePtr,TxIpduPtr
 * Param-Name[out]     None
 * Param-Name[in/out]  RptNum
 * Return              None
 */
/******************************************************************************/
FUNC(void, COM_CODE)
Com_TxIpduTMSHandle(Com_TxIPduRunTimeStateType* TxIpduStatePtr, const Com_TxIPduType* TxIpduPtr, uint16* RptNum)
{
#if (STD_ON == COM_TMS_ENABLE)
    uint8 oldTMS = TxIpduStatePtr->TxIpduRTStFlag & Com_TX_TMS_EN;
    boolean txIpduTMS = Com_TxIpduTMSCalculate(TxIpduPtr);
#endif
#if (0u < COM_TX_MODE_TRUE_DIRECT_NUMBER) || (0u < COM_TX_MODE_FALSE_DIRECT_NUMBER)    \
    || (0u < COM_TX_MODE_TRUE_PERIOD_NUMBER) || (0u < COM_TX_MODE_FALSE_PERIOD_NUMBER) \
    || (0u < COM_TX_MODE_TRUE_MIXED_NUMBER) || (0u < COM_TX_MODE_FALSE_MIXED_NUMBER)   \
    || (0u < COM_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER) || (0u < COM_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER)
    PduIdType txModeIndex;
#if (STD_ON == COM_TMS_ENABLE)
    /*According to the TMS get send mode, event trigger send times, send cycles*/
    if (!txIpduTMS)
    {
        TxIpduStatePtr->TxIpduRTStFlag &= Com_TX_TMS_DIS;
        txModeIndex = TxIpduPtr->ComTxModeFalseRefId;
        TxIpduStatePtr->ipduTxMode = TxIpduPtr->ComTxFalseModeMode;
        switch (TxIpduStatePtr->ipduTxMode)
        {
#if (0u < COM_TX_MODE_FALSE_DIRECT_NUMBER)
        case COM_TX_MODE_DIRECT:
            *RptNum = Com_TxModeFalseDirectRepetitions[txModeIndex].ComTxModeNumberOfRepetitions;
            break;
#endif
#if (0u < COM_TX_MODE_FALSE_MIXED_NUMBER)
        case COM_TX_MODE_MIXED:
            *RptNum = Com_TxModeFalseMixedRepetitions[txModeIndex].ComTxModeNumberOfRepetitions;
            if (Com_TX_TMS_EN == oldTMS)
            {
                TxIpduStatePtr->PeriodCnt = Com_TxModeFalseMixedRepetitions[txModeIndex].ComTxModeTimeOffset;
            }
            break;
#endif
#if (0u < COM_TX_MODE_FALSE_PERIOD_NUMBER) || (0u < COM_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER)
        case COM_TX_MODE_MIXED_WITHOUT_REPETITION:
        case COM_TX_MODE_PERIODIC:
            if (Com_TX_TMS_EN == oldTMS)
            {
                TxIpduStatePtr->PeriodCnt = Com_TxModeFalsePeriod[txModeIndex].ComTxModeTimeOffset;
            }
            break;
#endif
        default:
            /* do nothing */
            break;
        }
    }
    else
#endif
    {
        TxIpduStatePtr->TxIpduRTStFlag |= Com_TX_TMS_EN;
        txModeIndex = TxIpduPtr->ComTxModeTrueRefId;
        TxIpduStatePtr->ipduTxMode = TxIpduPtr->ComTxTrueModeMode;
        switch (TxIpduStatePtr->ipduTxMode)
        {
#if (0u < COM_TX_MODE_TRUE_DIRECT_NUMBER)
        case COM_TX_MODE_DIRECT:
            *RptNum = Com_TxModeTrueDirectRepetitions[txModeIndex].ComTxModeNumberOfRepetitions;
            break;
#endif
#if (0u < COM_TX_MODE_TRUE_MIXED_NUMBER)
        case COM_TX_MODE_MIXED:
            *RptNum = Com_TxModeTrueMixedRepetitions[txModeIndex].ComTxModeNumberOfRepetitions;
#if (STD_ON == COM_TMS_ENABLE)
            if (Com_TX_TMS_EN != oldTMS)
            {
                TxIpduStatePtr->PeriodCnt = Com_TxModeTrueMixedRepetitions[txModeIndex].ComTxModeTimeOffset;
            }
#endif
            break;
#endif
#if (STD_ON == COM_TMS_ENABLE) \
    && ((0u < COM_TX_MODE_TRUE_PERIOD_NUMBER) || (0u < COM_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER))
        case COM_TX_MODE_MIXED_WITHOUT_REPETITION:
        case COM_TX_MODE_PERIODIC:
            if (Com_TX_TMS_EN != oldTMS)
            {
                TxIpduStatePtr->PeriodCnt = Com_TxModeTruePeriod[txModeIndex].ComTxModeTimeOffset;
            }
            break;
#endif
        default:
            /* do nothing */
            break;
        }
    }
#endif
    return;
}
#endif

#if (0u < COM_TXIPDU_NUMBER)                                                                          \
    && ((STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE) \
        || (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE))
/******************************************************************************/
/*
 * Brief               set tx signal update bit to 1
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxIpduId,UpdateBitPosition
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
FUNC(void, COM_CODE)
Com_SetSignalUpdateBit(
    Com_TxIpduBufIdType IpduBufferIndex,
    Com_SignalPositionType UpdateBitPosition,
    uint8 UpdateBitMask)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
#if (0u < COM_TXIPDUBUFF_SIZE)
    if (COM_UNUSED_SIGNALPOSITION != UpdateBitPosition)
    {
        /*If the update bit is configured, the corresponding bit is set to 1*/
        Com_TxIPduRuntimeBuff[IpduBufferIndex + (Com_TxIpduBufIdType)UpdateBitPosition] |= UpdateBitMask;
    }
#endif
    COM_NOUSED(IpduBufferIndex);
    COM_NOUSED(UpdateBitPosition);
    COM_NOUSED(UpdateBitMask);
    return;
}
#endif

#if (                                                                    \
    ((0u < COM_TXIPDU_NUMBER) && (STD_ON == COM_TX_IPDU_COUNTER_ENABLE)) \
    || ((0u < COM_RXIPDU_NUMBER) && (STD_ON == COM_RX_IPDU_COUNTER_ENABLE)))
/******************************************************************************/
/*
 * Brief               update the expected counter
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      IsTxPdu,IpduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, COM_CODE)
Com_PduCounterIncrease(boolean IsTxPdu, PduIdType IpduId)
{
    uint8 maxCounter;
    uint8 activeCounter;
    if (IsTxPdu)
    {
/*Update TxPdu's counter value*/
#if (0u < COM_TXIPDU_NUMBER) && (STD_ON == COM_TX_IPDU_COUNTER_ENABLE)
        Com_TxIPduRunTimeStateType* txIpduStatePtr = &Com_TxIPduRunTimeState[IpduId];
        Com_TxIPduCounterIdType counterTxIndex = Com_ConfigStd->ComTxIPdu[IpduId].ComIPduCounterIndex;
        maxCounter = Com_TxIPduCounter[counterTxIndex].ComIPduMaxCounter;
        activeCounter = txIpduStatePtr->TxIpduCounter;
        txIpduStatePtr->TxIpduCounter = (maxCounter == activeCounter) ? 0u : (activeCounter + 1u);
#endif /* 0u < COM_TXIPDU_NUMBER) && (STD_ON == COM_TX_IPDU_COUNTER_ENABLE */
    }
    else
    {
/*Update RxPdu's counter value*/
#if (0u < COM_RXIPDU_NUMBER) && (STD_ON == COM_RX_IPDU_COUNTER_ENABLE)
        Com_RxIPduRunTimeStateType* rxIpduStatePtr = &Com_RxIPduRunTimeState[IpduId];
        Com_RxIPduCounterIdType counterRxIndex = Com_ConfigStd->ComRxIPdu[IpduId].ComIPduCounterIndex;
        maxCounter = Com_RxIPduCounter[counterRxIndex].ComIPduMaxCounter;
        activeCounter = rxIpduStatePtr->RxIpduCounter;
        rxIpduStatePtr->RxIpduCounter = (maxCounter == activeCounter) ? 0u : (activeCounter + 1u);
#endif /* 0u < COM_RXIPDU_NUMBER) && (STD_ON == COM_RX_IPDU_COUNTER_ENABLE */
    }
    COM_NOUSED(IpduId);
    return;
}
#endif

#if (COM_RXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
/******************************************************************************/
/*
 * Brief               Called by Com_TxSignalTMCCalculate,
 *                     Com_RxSignalFilter.
 *                     when FilterAlgorithm is MASKED_NEW_DIFFERS_MASKED_OLD,
 *                     calculate the signal's TMC
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalType,SignalValue,SignalBufferId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/******************************************************************************/
FUNC(boolean, COM_CODE)
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
Com_MaskedNewDiffersMaskedOldFilterCalculate(
    Com_SignalType SignalType,
    uint64 SignalValue,
    uint16 SignalBufferId,
    Com_FilterMaskType ComFilterMask)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    boolean filterOk = FALSE;

    /* PRQA S 3315 ++ */ /* MISRA Rule 16.6 */
    switch (SignalType)
    {
/*According to the signal type,
 *Calculated the signal value is filtered or not?*/
#if (0u < COM_SIGNAL_BOOLBUFF_SIZE)
    case COM_BOOLEAN:
        /* PRQA S 4502,4340 ++ */ /* MISRA Rule 10.1,10.5 */
        if ((((boolean)(SignalValue)) & ((boolean)(ComFilterMask)))
            != (Com_SignalBoolRuntimeBuff[SignalBufferId] & ((boolean)(ComFilterMask))))
        /* PRQA S 4502,4340 -- */ /* MISRA Rule 10.1,10.5 */
        {
            filterOk = TRUE;
        }
        break;
#endif
#if (0u < COM_SIGNAL_8BITBUFF_SIZE)
    case COM_UINT8:
    case COM_SINT8:
        if ((((uint8)(SignalValue)) & ((uint8)(ComFilterMask)))
            != (Com_Signal8BitRuntimeBuff[SignalBufferId] & ((uint8)(ComFilterMask))))
        {
            filterOk = TRUE;
        }
        break;
#endif
#if (0u < COM_SIGNAL_16BITBUFF_SIZE)
    case COM_UINT16:
    case COM_SINT16:
        if ((((uint16)(SignalValue)) & ((uint16)(ComFilterMask)))
            != (Com_Signal16BitRuntimeBuff[SignalBufferId] & ((uint16)(ComFilterMask))))
        {
            filterOk = TRUE;
        }
        break;
#endif
#if (0u < COM_SIGNAL_32BITBUFF_SIZE)
    case COM_UINT32:
    case COM_SINT32:
        if ((((uint32)(SignalValue)) & ((uint32)(ComFilterMask)))
            != (Com_Signal32BitRuntimeBuff[SignalBufferId] & ((uint32)(ComFilterMask))))
        {
            filterOk = TRUE;
        }
        break;
#endif
#if (0u < COM_SIGNAL_64BITBUFF_SIZE)
    case COM_UINT64:
    case COM_SINT64:
        if (((SignalValue) & (ComFilterMask)) != (Com_Signal64BitRuntimeBuff[SignalBufferId] & (ComFilterMask)))
        {
            filterOk = TRUE;
        }
        break;
#endif
    default:
        /*do nothing*/
        break;
    }
    /* PRQA S 3315 -- */ /* MISRA Rule 16.6 */
    COM_NOUSED(SignalValue);
    COM_NOUSED(SignalBufferId);
    COM_NOUSED(ComFilterMask);
    return filterOk;
}
#endif

#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_SUBSTITUTE_ENABLE) \
    || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)
/******************************************************************************/
/*
 * Brief               substitute signal/group signal buffer
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalType,SignalInitValueId,SignalLength,SignalSubstituteValueId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, COM_CODE)
Com_SubstituteSignalBuffer(
    Com_SignalType SignalType,
    uint16 SignalInitValueId,
    uint16 SignalLength,
    uint16 SignalSubstituteValueId)
{
    SchM_Enter_Com_Context();
    /* PRQA S 3315 ++ */ /* MISRA Rule 16.6 */
    switch (SignalType)
    {
/*According to the signal type,
 *Copy the signal substitute value to signal runtime buffer*/
#if (0u < COM_SIGNAL_BOOL_SUBSTITUTE_SIZE)
    case COM_BOOLEAN:
        Com_SignalBoolRuntimeBuff[SignalInitValueId] = Com_SignalBoolSubstituteValue[SignalSubstituteValueId];
        break;
#endif /* 0u < COM_SIGNAL_BOOL_SUBSTITUTE_SIZE */
#if (0u < COM_SIGNAL_8BIT_SUBSTITUTE_SIZE)
    case COM_UINT8_N:
    case COM_UINT8_DYN:
        (void)ILib_memcpy(
            &Com_Signal8BitRuntimeBuff[SignalInitValueId],
            &Com_Signal8BitSubstituteValue[SignalSubstituteValueId],
            SignalLength);
        break;
    case COM_UINT8:
    case COM_SINT8:
        Com_Signal8BitRuntimeBuff[SignalInitValueId] = Com_Signal8BitSubstituteValue[SignalSubstituteValueId];
        break;
#endif /* 0u < COM_SIGNAL_8BIT_SUBSTITUTE_SIZE */
#if (0u < COM_SIGNAL_16BIT_SUBSTITUTE_SIZE)
    case COM_UINT16:
    case COM_SINT16:
        Com_Signal16BitRuntimeBuff[SignalInitValueId] = Com_Signal16BitSubstituteValue[SignalSubstituteValueId];
        break;
#endif /* 0u < COM_SIGNAL_16BIT_SUBSTITUTE_SIZE */
#if (0u < COM_SIGNAL_32BIT_SUBSTITUTE_SIZE)
    case COM_UINT32:
    case COM_SINT32:
    case COM_FLOAT32:
        Com_Signal32BitRuntimeBuff[SignalInitValueId] = Com_Signal32BitSubstituteValue[SignalSubstituteValueId];
        break;
#endif /* 0u < COM_SIGNAL_32BIT_SUBSTITUTE_SIZE */
#if (0u < COM_SIGNAL_64BIT_SUBSTITUTE_SIZE)
    case COM_UINT64:
    case COM_SINT64:
    case COM_FLOAT64:
        Com_Signal64BitRuntimeBuff[SignalInitValueId] = Com_Signal64BitSubstituteValue[SignalSubstituteValueId];
        break;
#endif /* 0u < COM_SIGNAL_64BIT_SUBSTITUTE_SIZE */
    default:
        /*do nothing*/
        break;
    }
    /* PRQA S 3315 -- */ /* MISRA Rule 16.6 */
    SchM_Exit_Com_Context();
    COM_NOUSED(SignalType);
    COM_NOUSED(SignalInitValueId);
    COM_NOUSED(SignalLength);
    COM_NOUSED(SignalSubstituteValueId);
}
#endif /* STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_SUBSTITUTE_ENABLE \
    || STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_SUBSTITUTE_ENABLE */

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
/*******************************************************************************
**                      End of file                                           **
*******************************************************************************/
