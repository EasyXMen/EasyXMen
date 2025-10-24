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
/*
 ************************************************************************************************************************
 **
 **  @file               : Com_Internal.c
 **  @author             : shengnan.sun
 **  @date               : 2024/09/01
 **  @vendor             : isoft
 **  @description        : Com internal API definitions
 **
 ***********************************************************************************************************************/
/* PRQA S 3120,3132 ++ */ /* VL_QAC_MagicNum */
/* PRQA S 2814,2824 ++ */ /* VL_QAC_DerefNullPtr */
/* PRQA S 0316,2844 ++ */ /* VL_QAC_0316,VL_Com_NullPtrValue */
/* =================================================== inclusions =================================================== */
#include "Com_Internal.h"
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
/**
 * @brief         Called by Com_SignalUnPackHandle for Endian convert,Swap byte mirror
 * @param[in]     dataPtr:Pointer of data
 * @param[in]     size:Size
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66519
 */
COM_LOCAL void Com_EndianSwap(void* dataPtr, uint8 size); /* PRQA S 5015 */ /* VL_Com_UntrustedMethod */
/**
 * @brief         unpack all byte value(group signal/signal cover),is called by Com_SignalUnPackHandle
 * @param[in]     SigLsbBytePos: Signal LSB byte position
 * @param[in]     SignalByteLength: Signal byte length
 * @param[in]     IPduBufferPtr: Ipdu buffer pointer
 * @param[in]     SignalEndianness: Signal endianness type
 * @retval        signalValue:signalValue
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace
 */
#if (0u < COM_MAX_RXIPDUBUFF_SIZE) \
    || ((STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && (COM_MAX_TXSIGNALGROUP_NUMBER > 0u))
COM_LOCAL uint64 Com_SignalUnPackDataHandle(
    Com_SignalPositionType   SigLsbBytePos,
    uint8                    SignalByteLength,
    const uint8*             IPduBufferPtr,
    Com_SignalEndiannessType SignalEndianness);
#endif
/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */
#if (COM_MAX_ONEEVERYNFILTERSIGNAL_NUMBER > 0u)
#define COM_START_SEC_VAR_CLEARED_32
#include "Com_MemMap.h"
uint32 Com_OneEveryNcnt[COM_MAX_ONEEVERYNFILTERSIGNAL_NUMBER];
#define COM_STOP_SEC_VAR_CLEARED_32
#include "Com_MemMap.h"
#endif

#if (COM_MAX_TXSIGNALGROUP_NUMBER > 0u)
#define COM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"
Com_TxSignalGroupRunTimeStateType Com_TxSignalGroupState[COM_MAX_TXSIGNALGROUP_NUMBER];
#define COM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"
#endif

#if (COM_MAX_RXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
#define COM_START_SEC_VAR_CLEARED_BOOLEAN
#include "Com_MemMap.h"
boolean Com_MaskNewDifferMaskOldTimeOut[COM_MAX_RXMASKNEWDIFFERMASKOLD_NUMBER];
#define COM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Com_MemMap.h"
#endif

#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE)
#define COM_START_SEC_VAR_CLEARED_16
#include "Com_MemMap.h"
uint16 Com_RxSignalTimeOutCnt[COM_MAX_RXSIG_TIMEOUT_NUMBER];
#define COM_STOP_SEC_VAR_CLEARED_16
#include "Com_MemMap.h"
#endif

#if (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)
#define COM_START_SEC_VAR_CLEARED_16
#include "Com_MemMap.h"
uint16 Com_RxSignalGroupTimeOutCnt[COM_MAX_RXSIGGRP_TIMEOUT_NUMBER];
#define COM_STOP_SEC_VAR_CLEARED_16
#include "Com_MemMap.h"
#endif
/* ========================================== external function definitions ========================================= */
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/* PRQA S 2895 ++ */ /* VL_Com_NegtiveValueType */
/**
 * @brief get partitionId from partitioIdList
 *
 */
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
/* PRQA S 1532 ++*/ /* VL_QAC_OneFunRef */
uint16 Com_PartitonIdFromList(ApplicationType osApplicationId)
/* PRQA S 1532 --*/
{
    uint16 partitionId = COM_UNUSED_UINT16;
    for (uint16 index = 0u; index < COM_MAX_PARTITION_NUMBER; ++index)
    {
        if (osApplicationId == Com_PartitionList[index])
        {
            partitionId = index;
            break;
        }
    }
    return partitionId;
}
#endif
/**
 * @brief  Init Runtime TMC of all tx signals and tx group siganls
 *         and GW dest signals,which the comfilter is not NULL
 *
 */
/* PRQA S 1532 ++ */ /* VL_Com_ReferencedOnlyOne */
void Com_TxSignalTMCStateInit(Com_MainFunctionIdType TxMainfunctionId)
/* PRQA S 1532 -- */
{
#if (0u < COM_MAX_TMCTXSIGNAL_NUMBER)
    const boolean* txSigTMCInitPtr = Com_CfgTxModeAndSignalFilterPtr->TxSignalTMCInitVale[TxMainfunctionId];
    if (NULL_PTR != txSigTMCInitPtr)
    {
        const Com_TxTMCSizeType* txTMCSizePtr = &Com_CfgTxModeAndSignalFilterPtr->TxTMCSize[TxMainfunctionId];
#if (0u < COM_MAX_TXGRPSIG_FILTERTYPE_MAX_NUMBER)
        (void)IStdLib_MemCpy(
            Com_TxGroupSignalTMCBuffer[TxMainfunctionId],
            txSigTMCInitPtr,
            txTMCSizePtr->TxGroupSignalSize);
#endif
        (void)IStdLib_MemCpy(Com_TxSignalTMCRunTime[TxMainfunctionId], txSigTMCInitPtr, txTMCSizePtr->TxSignalSize);
    }
#endif
}

/**
 * @brief set all tx signal group state to not need to trigger transmit
 *
 */
#if (0u < COM_MAX_TXSIGNALGROUP_NUMBER)
/* PRQA S 1532 ++ */ /* VL_Com_ReferencedOnlyOne */
void Com_TxSignalGroupTriggerFlagInit(void)
/* PRQA S 1532 -- */
{
    for (uint16 cirCnt = 0u; ((Com_ConfigStd->TxSignalGroupNum > 0U) && (cirCnt < Com_ConfigStd->TxSignalGroupNum));
         ++cirCnt)
    {
        Com_TxSignalGroupState[cirCnt].TxSigGrpRTStFlag &= COM_NTIMESTRANSMITNEED_DIS;
    }
}
#endif

/**
 * @brief init signal/group signal buffer
 *
 */
#if ((0u < COM_MAX_IPDUGROUP_NUMBER) || (0u < COM_MAX_RXIPDU_NUMBER))
void Com_InitSignalBuffer(
    Com_SignalType SignalType,
    uint16         SignalInitValueId,
    uint16         ComSignalLength,
    uint16         PartitionId)
{
    const Com_SignalInitValueType* signalInitValuePtr = Com_CfgSignalInitValuePtr;
#if (0u < COM_MAX_SIGNAL_BOOLBUFF_SIZE)
    boolean* signalBoolInitValueBuff = Com_SignalBoolRuntimeBuff[PartitionId];
#endif
#if (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE)
    uint8* signal8BitInitValueBuff = Com_Signal8BitRuntimeBuff[PartitionId];
#endif
#if (0u < COM_MAX_SIGNAL_16BITBUFF_SIZE)
    uint16* signalBit16InitValueBuff = Com_SignalBit16RuntimeBuff[PartitionId];
#endif
#if (0u < COM_MAX_SIGNAL_32BITBUFF_SIZE)
    uint32* signal32BitInitValueBuff = Com_Signal32BitRuntimeBuff[PartitionId];
#endif
#if (0u < COM_MAX_SIGNAL_64BITBUFF_SIZE)
    uint64* signal64BitInitValueBuff = Com_Signal64BitRuntimeBuff[PartitionId];
#endif
    SchM_Enter_Com_Context();
    switch (SignalType)
    {
/*According to the signal type,
 *Copy the init signal value to signal runtime buffer*/
#if (0u < COM_MAX_SIGNAL_BOOLBUFF_SIZE)
    case COM_BOOLEAN:
        signalBoolInitValueBuff[SignalInitValueId] =
            signalInitValuePtr->SignalBoolInitValue[PartitionId][SignalInitValueId];
        break;
#endif
#if (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE)
    case COM_UINT8_N:
    case COM_UINT8_DYN:
        (void)IStdLib_MemCpy(
            &signal8BitInitValueBuff[SignalInitValueId],
            &signalInitValuePtr->Signal8BitInitValue[PartitionId][SignalInitValueId],
            ComSignalLength);
        break;
    case COM_UINT8:
    case COM_SINT8:
        signal8BitInitValueBuff[SignalInitValueId] =
            signalInitValuePtr->Signal8BitInitValue[PartitionId][SignalInitValueId];
        break;
#endif
#if (0u < COM_MAX_SIGNAL_16BITBUFF_SIZE)
    case COM_UINT16:
    case COM_SINT16:
        signalBit16InitValueBuff[SignalInitValueId] =
            signalInitValuePtr->SignalBit16InitValue[PartitionId][SignalInitValueId];
        break;
#endif
#if (0u < COM_MAX_SIGNAL_32BITBUFF_SIZE)
    case COM_UINT32:
    case COM_SINT32:
    case COM_FLOAT32:
        signal32BitInitValueBuff[SignalInitValueId] =
            signalInitValuePtr->Signal32BitInitValue[PartitionId][SignalInitValueId];
        break;
#endif
#if (0u < COM_MAX_SIGNAL_64BITBUFF_SIZE)
    case COM_UINT64:
    case COM_SINT64:
    case COM_FLOAT64:
        signal64BitInitValueBuff[SignalInitValueId] =
            signalInitValuePtr->Signal64BitInitValue[PartitionId][SignalInitValueId];
        break;
#endif
    default:
        /*do nothing*/
        break;
    }
    SchM_Exit_Com_Context();
    COM_NOUSED(SignalInitValueId);
    COM_NOUSED(ComSignalLength);
}
#endif
/**
 * @brief unpack all byte value(group signal/signal cover,except MSB byte).
 *
 */
#if (0u < COM_MAX_RXIPDUBUFF_SIZE) \
    || ((STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && (COM_MAX_TXSIGNALGROUP_NUMBER > 0u))
uint64 Com_SignalUnPackHandle(
#if (STD_ON == COM_SIGNAL_SIGNED_TYPE_ENABLE)
    Com_SignalType SignalType,
#endif
    Com_SignalConfigType SignalConfigType,
    const void*          SignalPtr,
    const uint8*         IPduBufferPtr)
{
#if (0u < COM_MAX_RXSIGNAL_NUMBER)
    const Com_RxSignalType* rxSignalPtr;
#endif
#if ((0u < COM_MAX_RXGROUPSIGNAL_NUMBER) && (0u < COM_MAX_RXSIGNALGROUP_NUMBER))
    const Com_RxGroupSignalType* rxGroupSignalPtr;
#endif
#if (0u < COM_MAX_GW_SOURCE_DESCRIPTION_NUMBER)
    const Com_GwSourceSignalType* gwSourceSignalPtr;
#endif
#if ((0u < COM_MAX_TXGROUPSIGNAL_NUMBER) && (0u < COM_MAX_TXSIGNALGROUP_NUMBER))
    const Com_TxGroupSignalType* txGroupSignalPtr;
#endif
    Com_SignalPositionType   sigLsbBytePos;
    uint8                    lsbPosInByte;
    uint8                    signalByteLength;
    uint8                    bitSize;
    Com_SignalEndiannessType signalEndianness;
    boolean                  isTrue = FALSE;

    switch (SignalConfigType)
    {
#if (0u < COM_MAX_RXSIGNAL_NUMBER)
    case COM_RX_SIGNAL:
        rxSignalPtr      = (const Com_RxSignalType*)SignalPtr;
        sigLsbBytePos    = rxSignalPtr->SigLsbBytePos;
        lsbPosInByte     = rxSignalPtr->SignalPosInByte;
        bitSize          = rxSignalPtr->ComBitSize;
        signalByteLength = rxSignalPtr->ComSignalByteLength;
        signalEndianness = rxSignalPtr->ComSignalEndianness;
        isTrue           = TRUE;
        break;
#endif
#if ((0u < COM_MAX_RXGROUPSIGNAL_NUMBER) && (0u < COM_MAX_RXSIGNALGROUP_NUMBER))
    case COM_RX_GROUP_SIGNAL:
        rxGroupSignalPtr = (const Com_RxGroupSignalType*)SignalPtr;
        sigLsbBytePos    = rxGroupSignalPtr->SigLsbBytePos;
        lsbPosInByte     = rxGroupSignalPtr->SignalPosInByte;
        bitSize          = rxGroupSignalPtr->ComBitSize;
        signalByteLength = rxGroupSignalPtr->ComSignalByteLength;
        signalEndianness = rxGroupSignalPtr->ComSignalEndianness;
        isTrue           = TRUE;
        break;
#endif
#if (0u < COM_MAX_GW_SOURCE_DESCRIPTION_NUMBER)
    case COM_RX_DESCRIPTION:
        gwSourceSignalPtr = (const Com_GwSourceSignalType*)SignalPtr;
        sigLsbBytePos     = gwSourceSignalPtr->SigLsbBytePos;
        lsbPosInByte      = gwSourceSignalPtr->SignalPosInByte;
        bitSize           = gwSourceSignalPtr->ComBitSize;
        signalByteLength  = gwSourceSignalPtr->ComSignalByteLength;
        signalEndianness  = gwSourceSignalPtr->ComSignalEndianness;
        isTrue            = TRUE;
        break;
#endif
#if ((0u < COM_MAX_TXGROUPSIGNAL_NUMBER) && (0u < COM_MAX_TXSIGNALGROUP_NUMBER))
    case COM_TX_GROUP_SIGNAL:
        txGroupSignalPtr = (const Com_TxGroupSignalType*)SignalPtr;
        sigLsbBytePos    = txGroupSignalPtr->SigLsbBytePos;
        lsbPosInByte     = txGroupSignalPtr->SignalPosInByte;
        bitSize          = txGroupSignalPtr->ComBitSize;
        signalByteLength = txGroupSignalPtr->ComSignalByteLength;
        signalEndianness = txGroupSignalPtr->ComSignalEndianness;
        isTrue           = TRUE;
        break;
#endif
    default:
        /* do nothing */
        break;
    }
    uint64 signalValue = 0u;
    if (isTrue)
    {
        signalValue = Com_SignalUnPackDataHandle(sigLsbBytePos, signalByteLength, IPduBufferPtr, signalEndianness);
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
            signalValue |= (msbByteValue << (bitSize - msbByteBitSize));
            /* PRQA S 2986 */ /* VL_Com_OperationRedundant */
        }

        /* Flag extension */
#if (STD_ON == COM_SIGNAL_SIGNED_TYPE_ENABLE)
        if (((COM_SINT8 == SignalType) || (COM_SINT16 == SignalType) || (COM_SINT32 == SignalType)
             || (COM_SINT64 == SignalType))
            && ((signalValue >> (bitSize - 1u)) > 0ULL))
        {
            /* PRQA S 2921 ++ */ /* VL_Com_ShiftOperation */
            signalValue = signalValue | (uint64)(COM_UNUSED_UINT64 << bitSize);
            /* PRQA S 2921 -- */
        }
#endif
    }

    return signalValue;
}
#endif

/**
 * @brief Called by Com_TxSignalPack,Com_TxGroupSignalPack
 *        pack signal which signal type is not boolean,COM_UINT8_N,
 *        COM_UINT8_DYN,and the signal value bits cover 2-9 bytes
 *
 */
#if ((0u < COM_MAX_TXSIGNALGROUP_NUMBER) || (0u < COM_MAX_TXSIGNAL_NUMBER) || (0u < COM_MAX_GWMAPPING_NUMBER))
void Com_TxSignalPackHandle(
    Com_SignalConfigType SignalConfigType,
    const void*          TxSignalPtr,
    uint64               TxValue,
    uint8*               IPduBufferPtr)
{
#if (0u < COM_MAX_TXIPDUBUFF_SIZE)
#if (0u < COM_MAX_TXSIGNAL_NUMBER)
    const Com_TxSignalType* txSignalPtr;
#endif
#if ((0u < COM_MAX_TXGROUPSIGNAL_NUMBER) && (0u < COM_MAX_TXSIGNALGROUP_NUMBER))
    const Com_TxGroupSignalType* txGroupSignalPtr;
#endif
#if (0u < COM_MAX_GW_DESTINATION_DESCRIPTION_NUMBER)
    const Com_GwDestSignalType* gwDestSignalPtr;
#endif
    Com_SignalPositionType   sigLsbBytePos    = COM_UNUSED_SIGNALPOSITION;
    uint8                    lsbPosInByte     = COM_UNUSED_UINT8;
    uint8                    signalByteLength = COM_UNUSED_UINT8;
    uint8                    bitSize          = COM_UNUSED_UINT8;
    Com_SignalEndiannessType signalEndianness = COM_ENDIAN_NONE;

    switch (SignalConfigType)
    {
#if (0u < COM_MAX_TXSIGNAL_NUMBER)
    case COM_TX_SIGNAL:
        txSignalPtr      = (const Com_TxSignalType*)TxSignalPtr;
        sigLsbBytePos    = txSignalPtr->SigLsbBytePos;
        lsbPosInByte     = txSignalPtr->SignalPosInByte;
        bitSize          = txSignalPtr->ComBitSize;
        signalByteLength = txSignalPtr->ComSignalByteLength;
        signalEndianness = txSignalPtr->ComSignalEndianness;
        break;
#endif
#if ((0u < COM_MAX_TXGROUPSIGNAL_NUMBER) && (0u < COM_MAX_TXSIGNALGROUP_NUMBER))
    case COM_TX_GROUP_SIGNAL:
        txGroupSignalPtr = (const Com_TxGroupSignalType*)TxSignalPtr;
        sigLsbBytePos    = txGroupSignalPtr->SigLsbBytePos;
        lsbPosInByte     = txGroupSignalPtr->SignalPosInByte;
        bitSize          = txGroupSignalPtr->ComBitSize;
        signalByteLength = txGroupSignalPtr->ComSignalByteLength;
        signalEndianness = txGroupSignalPtr->ComSignalEndianness;
        break;
#endif
#if (0u < COM_MAX_GW_DESTINATION_DESCRIPTION_NUMBER)
    case COM_TX_DESCRIPTION:
        gwDestSignalPtr  = (const Com_GwDestSignalType*)TxSignalPtr;
        sigLsbBytePos    = gwDestSignalPtr->SigLsbBytePos;
        lsbPosInByte     = gwDestSignalPtr->SignalPosInByte;
        bitSize          = gwDestSignalPtr->ComBitSize;
        signalByteLength = gwDestSignalPtr->ComSignalByteLength;
        signalEndianness = gwDestSignalPtr->ComSignalEndianness;
        break;
#endif
    default:
        /* do nothing */
        break;
    }

    uint64 signalValue = TxValue << lsbPosInByte; /* PRQA S 2792 */ /* VL_Com_ShiftOperation */
#if (TRUE == CPU_32_WITH_16_ADR)
    uint8 bufPtr[8u] = {0u};
    for (uint8 i = 0u; i < tSignalByteLength; ++i)
    {
        bufPtr[i]   = (uint8)(signalValue & COM_UNUSED_UINT8);
        signalValue = signalValue >> 8u;
    }
#elif (FALSE == CPU_32_WITH_16_ADR)
    uint8* bufPtr = (uint8*)&signalValue;
#endif
    /* PRQA S 2907 ++ */ /* VL_Com_NegtiveValueType */
    uint16 sigMsbBytePosBE   = 1u + (uint16)sigLsbBytePos - (uint16)signalByteLength;
    uint16 sigMsbBytePosOt   = (uint16)((uint16)sigLsbBytePos + (uint16)signalByteLength - 1u);
    uint16 sigMsbBytePos     = (COM_BIG_ENDIAN == signalEndianness) ? sigMsbBytePosBE : sigMsbBytePosOt;
    uint8  tSignalByteLength = (signalByteLength < 9u) ? signalByteLength : 8u;

    /* LSB Byte */
    /* PRQA S 2792 ++ */ /* VL_Com_ShiftOperation */
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
    bufPtr[0u] |= (IPduBufferPtr[sigLsbBytePos] & (COM_UNUSED_UINT8 >> (8u - lsbPosInByte)));
#else
    bufPtr[7u] |= (IPduBufferPtr[sigLsbBytePos] & (COM_UNUSED_UINT8 >> (8u - lsbPosInByte)));
#endif
    /* PRQA S 2792 -- */
    /* MSB Byte*/
    uint8 msbByteBitSize = (uint8)((lsbPosInByte + bitSize) & 0x07u);
    if (0u != msbByteBitSize)
    {
        /* PRQA S 2921,2906 ++ */ /* VL_Com_ShiftOperation */
        uint8 msbByteHighBitMask = (uint8)(COM_UNUSED_UINT8 << msbByteBitSize);
        /* PRQA S 2921,2906 -- */
        /* SignalByteLength range 1..9 */
        if (signalByteLength < 9u)
        {
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
            bufPtr[signalByteLength - 1u] |= (IPduBufferPtr[sigMsbBytePos] & msbByteHighBitMask);
#else
            bufPtr[8 - signalByteLength] |= (IPduBufferPtr[sigMsbBytePos] & msbByteHighBitMask);
#endif
        }
        else
        {
            /* PRQA S 2792 ++ */ /* VL_Com_ShiftOperation */
            uint8 msbByteValue = (uint8)(TxValue >> (bitSize - msbByteBitSize));
            /* PRQA S 2792 -- */
            IPduBufferPtr[sigMsbBytePos] = (IPduBufferPtr[sigMsbBytePos] & msbByteHighBitMask) | (uint8)msbByteValue;
        }
    }
    uint16 srcStartIdx = 0u;
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
#endif
#endif
    uint16 destStartIdx = (COM_BIG_ENDIAN == signalEndianness)
                              ? ((uint16)1u + (uint16)sigLsbBytePos - (uint16)tSignalByteLength)
                              : (uint16)sigLsbBytePos;
    /* PRQA S 2907 -- */

    SchM_Enter_Com_Context();
    (void)IStdLib_MemCpy(&IPduBufferPtr[destStartIdx], &bufPtr[srcStartIdx], tSignalByteLength);
    SchM_Exit_Com_Context();
#endif
}
#endif

/**
 * @brief Called by Com_TxSignalTMCCalculate,
 *        Com_DestSignalTMCCalculate,Com_RxSignalFilter.
 *        when FilterAlgorithm is NEW_IS_WITHIN or NEW_IS_WITHOUT,calculate the signal's TMC.
 *
 */
#if (COM_MAX_TXNEWISWITHIN_NUMBER > 0u) || (COM_MAX_RXNEWISWITHIN_NUMBER > 0u) || (COM_MAX_RXNEWISOUTSIDE_NUMBER > 0u) \
    || (COM_MAX_TXNEWISOUTSIDE_NUMBER > 0U)
/* PRQA S 1505 ++ */ /* VL_Com_1505 */
boolean Com_NewIsWInWOtFilterCalculate(
    Com_SignalType    SignalType,
    uint64            SignalValue,
    Com_FilterMaxType ComFilterMax,
    Com_FilterMinType ComFilterMin,
    boolean           Within)
/* PRQA S 1505 -- */ /* VL_Com_1505 */
{
    /* FilterAlgorithm is NEW_IS_WITHIN , input parameter Within is TRUE,FilterAlgorithm is NEW_IS_WITHOUT, input
     * parameter Within is FALSE */
    boolean IsWith = FALSE;
    /* PRQA S 2105,2144 ++ */ /* VL_Com_ConversionSigned */
    switch (SignalType)
    {
    /*According to the signal type,
     *Calculated the signal value is filtered or not?*/
    case COM_SINT8:
        IsWith = (((sint8)(SignalValue)) > ((sint8)(ComFilterMax)))
                     ? FALSE
                     : ((((sint8)(SignalValue)) < ((sint8)(ComFilterMin))) ? FALSE : TRUE);
        break;
    case COM_SINT16:
        IsWith = (((sint16)(SignalValue)) > ((sint16)(ComFilterMax)))
                     ? FALSE
                     : ((((sint16)(SignalValue)) < ((sint16)(ComFilterMin))) ? FALSE : TRUE);
        break;
    case COM_SINT32:
        IsWith = (((sint32)(SignalValue)) > ((sint32)(ComFilterMax)))
                     ? FALSE
                     : ((((sint32)(SignalValue)) < ((sint32)(ComFilterMin))) ? FALSE : TRUE);
        break;
    case COM_SINT64:
        IsWith = (((sint64)(SignalValue)) > ((sint64)(ComFilterMax)))
                     ? FALSE
                     : ((((sint64)(SignalValue)) < ((sint64)(ComFilterMin))) ? FALSE : TRUE);
        break;
    case COM_UINT8:
    case COM_UINT16:
    case COM_UINT32:
    case COM_UINT64:
        IsWith = (SignalValue > ComFilterMax) ? FALSE : ((SignalValue < ComFilterMin) ? FALSE : TRUE);
        break;
    default:
        /* error */
        IsWith = !Within;
        break;
    }
    /* PRQA S 2105,2144 -- */

    return IsWith == Within; /* PRQA S 2109 */ /* VL_Com_BoolPromoted */
}
#endif

#if (0u < COM_MAX_TMCTXSIGNAL_NUMBER)
/**
 * @brief Called by Com_GwNotArraySignal.
 *        calculate dest signal/group signal/description signal TMC
 *
 */
/* PRQA S 1532 ++*/ /* VL_QAC_OneFunRef */
boolean Com_DestSignalTMCCalculate(
    Com_SignalType          SignalType,
    Com_FilterAlgorithmType FilterType,
    Com_SignalIdType        FilterIndex,
#if (COM_MAX_TXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
    uint64 OldSignalValue,
#endif
    uint64 NewSignalValue)
/* PRQA S 1532 --*/
{
    boolean                                 ret         = FALSE;
    const Com_TxModeAndSignalFilterPtrType* txFilterPtr = Com_CfgTxModeAndSignalFilterPtr;
#if (COM_MAX_TXMASKNEWEQUALSX_NUMBER > 0u) || (COM_MAX_TXMASKNEWDIFFERX_NUMBER > 0u) \
    || (COM_MAX_TXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
    Com_FilterMaskType filterMask;
    Com_FilterXType    filterX;
#endif
#if (COM_MAX_TXNEWISWITHIN_NUMBER > 0u) || (COM_MAX_TXNEWISOUTSIDE_NUMBER > 0u)
    Com_FilterMaxType filterMax;
    Com_FilterMinType filterMin;
#endif
#if (COM_MAX_ONEEVERYNFILTERSIGNAL_NUMBER > 0u)
    Com_SignalIdType filterOffset;
    Com_SignalIdType filterPeriod;
#endif

    switch (FilterType)
    {
    case COM_ALWAYS:
        ret = TRUE;
        break;
#if (COM_MAX_TXMASKNEWEQUALSX_NUMBER > 0u)
    case COM_MASKED_NEW_EQUALS_X:
        filterMask = txFilterPtr->TxMaskedNewEqualsX[FilterIndex].FilterMask;
        filterX    = txFilterPtr->TxMaskedNewEqualsX[FilterIndex].FilterX;
        ret        = ((NewSignalValue & filterMask) == filterX) ? TRUE : FALSE;
        break;
#endif
#if (COM_MAX_TXMASKNEWDIFFERX_NUMBER > 0u)
    case COM_MASKED_NEW_DIFFERS_X:
        filterMask = txFilterPtr->TxMaskedNewDiffersX[FilterIndex].FilterMask;
        filterX    = txFilterPtr->TxMaskedNewDiffersX[FilterIndex].FilterX;
        ret        = ((NewSignalValue & filterMask) != filterX) ? TRUE : FALSE;
        break;
#endif
        /*COM_BOOLEAN not support WITHIN and WITHOUT*/
#if (COM_MAX_TXNEWISWITHIN_NUMBER > 0u)
    case COM_NEW_IS_WITHIN:
        filterMax = txFilterPtr->TxNewIsWithin[FilterIndex].FilterMax;
        filterMin = txFilterPtr->TxNewIsWithin[FilterIndex].FilterMin;
        ret       = Com_NewIsWInWOtFilterCalculate(SignalType, NewSignalValue, filterMax, filterMin, TRUE);
        break;
#endif
        /*COM_BOOLEAN not support WITHIN and WITHOUT*/
#if (COM_MAX_TXNEWISOUTSIDE_NUMBER > 0u)
    case COM_NEW_IS_OUTSIDE:
        filterMax = txFilterPtr->TxNewIsOutside[FilterIndex].FilterMax;
        filterMin = txFilterPtr->TxNewIsOutside[FilterIndex].FilterMin;
        ret       = Com_NewIsWInWOtFilterCalculate(SignalType, NewSignalValue, filterMax, filterMin, FALSE);
        break;
#endif
#if (COM_MAX_ONEEVERYNFILTERSIGNAL_NUMBER > 0u)
    case COM_ONE_EVERY_N:
        filterOffset = txFilterPtr->FilterOneEveryN[FilterIndex].FilterOffset;
        filterPeriod = txFilterPtr->FilterOneEveryN[FilterIndex].FilterPeriod;
        Com_OneEveryNcnt[FilterIndex] =
            (filterPeriod == Com_OneEveryNcnt[FilterIndex]) ? 0u : Com_OneEveryNcnt[FilterIndex];
        ret = (filterOffset == Com_OneEveryNcnt[FilterIndex]) ? TRUE : FALSE;
        Com_OneEveryNcnt[FilterIndex]++;
        break;
#endif
#if (COM_MAX_TXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
    case COM_MASKED_NEW_DIFFERS_MASKED_OLD:
        filterMask = txFilterPtr->TxMaskedNewDiffersMaskedOld[FilterIndex];
        ret        = ((NewSignalValue & filterMask) != (OldSignalValue & filterMask)) ? TRUE : FALSE;
        break;
#endif
    default:
        /*do nothing*/
        break;
    }
    return ret;
}
#endif
/**
 * @brief Called by Com_TxSignalTMHandle,Com_TxIpduTMSHandle.
 *        calculate the tx ipdu TMS(signal/Group signal/Dest signal)
 *
 */
#if (                                                                                                       \
    (0u < COM_MAX_TXSIGNAL_NUMBER) || ((0u < COM_MAX_TXSIGNALGROUP_NUMBER) && (0u < COM_MAX_TXIPDU_NUMBER)) \
    || (0u < COM_MAX_GWMAPPING_NUMBER))                                                                     \
    && (STD_ON == COM_TMS_ENABLE)
boolean Com_TxIpduTMSCalculate(const Com_TxIPduType* TxIpduPtr, Com_MainFunctionIdType TxMainfunctionId)
{
    boolean ret = TRUE;
#if (0u < COM_MAX_TMCTXSIGNAL_NUMBER)
    const boolean* txSignalTMCRuntimePtr = Com_TxSignalTMCRunTime[TxMainfunctionId];
    /*just configuration two TxMode,need to Calculate the TMS of TxPdu.
     *ComTxModeTrue must be configured,ComTxModeFalse select configuration*/
    if (TxIpduPtr->TxFalseModeMode != COM_TX_MODE_NULL)
    {
        ret = FALSE;
#if (0u < COM_MAX_TXSIGNAL_FILTERTYPE_MAX_NUMBER)
        /*Calculate TMS based on all signals contained in this TxPdu*/
        Com_TxSignalIdType ipduSignalEndId   = TxIpduPtr->IPduSignalsRefEndId;
        Com_TxSignalIdType ipduSignalStartId = TxIpduPtr->IpduSignalRefStartId;
        for (; ((ipduSignalStartId < ipduSignalEndId) && !ret); ++ipduSignalStartId)
        {
            const Com_TxSignalType* ComTxSignalPtr = &Com_CfgTxSignalPtr[ipduSignalStartId];
            if ((COM_UNUSED_TXSIGNALFILTERID != ComTxSignalPtr->FilterIndex)
                && (txSignalTMCRuntimePtr[ComTxSignalPtr->TMCBufferId]))
            {
                ret = TRUE;
            }
        }
#endif

#if (0u < COM_MAX_DESTSIG_FILTERTYPE_MAX_NUMBER)
        /*Calculate TMS based on all dest description signals contained in this TxPdu*/
        Com_GwDestinationDescriptionIdType destSignalNumber, destSignalId;
        destSignalNumber = TxIpduPtr->IPduDespSignalsRefNumber;
        destSignalId     = TxIpduPtr->IPduDespSignalsRefStartId;
        for (; ((destSignalId < destSignalNumber) && !ret); ++destSignalId)
        {
            const Com_GwDestSignalType* ComDestSignalPtr = &Com_CfgGwMappingConfigPtr->DestSignal[destSignalId];
            if ((COM_UNUSED_DESTSIGNALFILTERID != ComDestSignalPtr->FilterIndex)
                && (txSignalTMCRuntimePtr[ComDestSignalPtr->TMCBufferId]))
            {
                ret = TRUE;
            }
        }
#endif

#if (0u < COM_MAX_TXGRPSIG_FILTERTYPE_MAX_NUMBER)
        Com_TxSignalGroupIdType signalGroupNumber, signalGroupId;
        Com_TxGroupSignalIdType groupSignalNumber, counter;
        /*Calculate TMS based on all group signals contained in this TxPdu*/
        signalGroupNumber = TxIpduPtr->IPduSignalGroupsRefEndId;
        signalGroupId     = TxIpduPtr->IPduSignalGroupsRefStartId;
        for (; ((signalGroupId < signalGroupNumber) && !ret); ++signalGroupId)
        {
            const Com_TxSignalGroupType* txSignalGroupPtr = &Com_CfgTxSignalGroupPtr[signalGroupId];
            groupSignalNumber                             = txSignalGroupPtr->GroupSignalRefEndId;
            counter                                       = txSignalGroupPtr->GroupSignalRefStartId;
            for (; (counter < groupSignalNumber) && (!ret); ++counter)
            {
                const Com_TxGroupSignalType* ComTxGroupSignalPtr = &Com_CfgTxGroupSignalPtr[counter];
                if ((COM_UNUSED_TXGRPSIGFILTERID != ComTxGroupSignalPtr->FilterIndex)
                    && (txSignalTMCRuntimePtr[ComTxGroupSignalPtr->TMCBufferId]))
                {
                    ret = TRUE;
                }
            }
        }
#endif
    }
#endif
    COM_NOUSED(TxIpduPtr);
    COM_NOUSED(TxMainfunctionId);
    return ret;
}
#endif

/**
 * @brief Called by Com_SendSignalGroupArray,Com_SendSignalGroup,
 *        Com_GwUint8NDYNSignal,Com_GwNotArraySignal.
 *        calculate the TMS,store new Tx mode,get rptNum value for
 *        DIRECT/MIXED tx mode,if TMS changed will reset periodic mode counter
 *
 */
#if (0u < COM_MAX_TXIPDU_NUMBER) || (0u < COM_MAX_GWMAPPING_NUMBER)
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
void Com_TxIpduTMSHandle(Com_TxIPduRunTimeStateType* TxIpduStatePtr, const Com_TxIPduType* TxIpduPtr, uint16* RptNum)
/* PRQA S 3673 -- */
{
#if (STD_ON == COM_TMS_ENABLE)
    uint8   oldTMS    = TxIpduStatePtr->TxIpduRTStFlag & COM_TX_TMS_EN;
    boolean txIpduTMS = Com_TxIpduTMSCalculate(TxIpduPtr, TxIpduPtr->TxMainfunctionId);
#endif
#if (0u < COM_MAX_TX_MODE_TRUE_DIRECT_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_DIRECT_NUMBER)    \
    || (0u < COM_MAX_TX_MODE_TRUE_PERIOD_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_PERIOD_NUMBER) \
    || (0u < COM_MAX_TX_MODE_TRUE_MIXED_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NUMBER)   \
    || (0u < COM_MAX_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER)
    const Com_TxModeAndSignalFilterPtrType* txModeAmdSignalFilterPtr = Com_CfgTxModeAndSignalFilterPtr;
#if (STD_ON == COM_TMS_ENABLE)
    /*According to the TMS get send mode, event trigger send times, send cycles*/
    if (!txIpduTMS)
    {
        TxIpduStatePtr->TxIpduRTStFlag &= COM_TX_TMS_DIS;
        TxIpduStatePtr->IpduTxMode = TxIpduPtr->TxFalseModeMode;
#if (0u < COM_MAX_TX_MODE_FALSE_DIRECT_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NUMBER) \
    || (0u < COM_MAX_TX_MODE_FALSE_PERIOD_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER)
        switch (TxIpduStatePtr->IpduTxMode)
        {
#if (0u < COM_MAX_TX_MODE_FALSE_DIRECT_NUMBER)
        case COM_TX_MODE_DIRECT:
            *RptNum = txModeAmdSignalFilterPtr->TxModeFalseDirectRepetitions[TxIpduPtr->TxModeFalseRefId]
                          .TxModeNumberOfRepetitions;
            break;
#endif
#if (0u < COM_MAX_TX_MODE_FALSE_MIXED_NUMBER)
        case COM_TX_MODE_MIXED:
            *RptNum = txModeAmdSignalFilterPtr->TxModeFalseMixedRepetitions[TxIpduPtr->TxModeFalseRefId]
                          .TxModeNumberOfRepetitions;
            if (COM_TX_TMS_EN == oldTMS)
            {
                TxIpduStatePtr->PeriodCnt =
                    txModeAmdSignalFilterPtr->TxModeFalseMixedRepetitions[TxIpduPtr->TxModeFalseRefId].TxModeTimeOffset;
            }
            break;
#endif
#if (0u < COM_MAX_TX_MODE_FALSE_PERIOD_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER)
        case COM_TX_MODE_MIXED_WITHOUT_REPETITION:
        case COM_TX_MODE_PERIODIC:
            if (COM_TX_TMS_EN == oldTMS)
            {
                TxIpduStatePtr->PeriodCnt =
                    txModeAmdSignalFilterPtr->TxModeFalsePeriod[TxIpduPtr->TxModeFalseRefId].TxModeTimeOffset;
            }
            break;
#endif
        default:
            /* do nothing */
            break;
        }
#endif
    }
    else
#endif
    {
        TxIpduStatePtr->TxIpduRTStFlag |= COM_TX_TMS_EN;
        TxIpduStatePtr->IpduTxMode = TxIpduPtr->TxTrueModeMode;
        switch (TxIpduStatePtr->IpduTxMode)
        {
#if (0u < COM_MAX_TX_MODE_TRUE_DIRECT_NUMBER)
        case COM_TX_MODE_DIRECT:
            *RptNum = txModeAmdSignalFilterPtr->TxModeTrueDirectRepetitions[TxIpduPtr->TxModeTrueRefId]
                          .TxModeNumberOfRepetitions;
            break;
#endif
#if (0u < COM_MAX_TX_MODE_TRUE_MIXED_NUMBER)
        case COM_TX_MODE_MIXED:
            *RptNum = txModeAmdSignalFilterPtr->TxModeTrueMixedRepetitions[TxIpduPtr->TxModeTrueRefId]
                          .TxModeNumberOfRepetitions;
#if (STD_ON == COM_TMS_ENABLE)
            if (COM_TX_TMS_EN != oldTMS)
            {
                TxIpduStatePtr->PeriodCnt =
                    txModeAmdSignalFilterPtr->TxModeTrueMixedRepetitions[TxIpduPtr->TxModeTrueRefId].TxModeTimeOffset;
            }
#endif
            break;
#endif
#if (STD_ON == COM_TMS_ENABLE) \
    && ((0u < COM_MAX_TX_MODE_TRUE_PERIOD_NUMBER) || (0u < COM_MAX_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER))
        case COM_TX_MODE_MIXED_WITHOUT_REPETITION:
        case COM_TX_MODE_PERIODIC:
            if (COM_TX_TMS_EN != oldTMS)
            {
                TxIpduStatePtr->PeriodCnt =
                    txModeAmdSignalFilterPtr->TxModeTruePeriod[TxIpduPtr->TxModeTrueRefId].TxModeTimeOffset;
            }
            break;
#endif
        default:
            /* do nothing */
            break;
        }
    }
#endif
    COM_NOUSED(RptNum);
    return;
}
#endif

/**
 * @brief set tx signal update bit to 1
 *
 */
#if (0u < COM_MAX_TXIPDU_NUMBER)                                                                      \
    && ((STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE) \
        || (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE))
/* PRQA S 1532 ++ */ /* VL_Com_ReferencedOnlyOne */
void Com_SetSignalUpdateBit(
    Com_TxIpduBufIdType    IpduBufferIndex,
    Com_SignalPositionType UpdateBitPosition,
    uint8                  UpdateBitMask,
    uint8*                 TxIpduRuntimeBuffPtr)
/* PRQA S 1532 -- */
{
#if (0u < COM_MAX_TXIPDUBUFF_SIZE)
    if (COM_UNUSED_SIGNALPOSITION != UpdateBitPosition)
    {
        /*If the update bit is configured, the corresponding bit is set to 1*/
        TxIpduRuntimeBuffPtr[IpduBufferIndex + (Com_TxIpduBufIdType)UpdateBitPosition] |= UpdateBitMask;
    }
#endif
    COM_NOUSED(IpduBufferIndex);
    COM_NOUSED(UpdateBitPosition);
    COM_NOUSED(UpdateBitMask);
    COM_NOUSED(TxIpduRuntimeBuffPtr);
    return;
}
#endif
/* PRQA S 2895 -- */
/**
 * @brief Called by Com_TxSignalTMCCalculate,
 *         Com_RxSignalFilter.
 *         when FilterAlgorithm is MASKED_NEW_DIFFERS_MASKED_OLD,
 *         calculate the signal's TMC
 *
 */
#if (COM_MAX_RXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
/* PRQA S 1532 ++ */ /* VL_Com_ReferencedOnlyOne */
boolean Com_MaskedNewDiffersMaskedOldFilterCalculate(
    Com_SignalType     SignalType,
    uint64             SignalValue,
    uint16             SignalBufferId,
    Com_FilterMaskType ComFilterMask,
    uint16             PartitionId)
/* PRQA S 1532 -- */
{
    boolean filterOk = FALSE;
#if (0u < COM_MAX_SIGNAL_BOOLBUFF_SIZE)
    boolean* signalBoolRuntimeBuffPtr = Com_SignalBoolRuntimeBuff[PartitionId];
#endif
#if (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE)
    const uint8* signal8BitRuntimeBuffPtr = Com_Signal8BitRuntimeBuff[PartitionId];
#endif
#if (0u < COM_MAX_SIGNAL_16BITBUFF_SIZE)
    const uint16* signalBit16RuntimeBuffPtr = Com_SignalBit16RuntimeBuff[PartitionId];
#endif
#if (0u < COM_MAX_SIGNAL_32BITBUFF_SIZE)
    const uint32* signal32BitRuntimeBuffPtr = Com_Signal32BitRuntimeBuff[PartitionId];
#endif
#if (0u < COM_MAX_SIGNAL_64BITBUFF_SIZE)
    const uint64* signal64BitRuntimeBuffPtr = Com_Signal64BitRuntimeBuff[PartitionId];
#endif

    switch (SignalType)
    {
/*According to the signal type,
 *Calculated the signal value is filtered or not?*/
#if (0u < COM_MAX_SIGNAL_BOOLBUFF_SIZE)
    case COM_BOOLEAN:
        if ((((boolean)(SignalValue)) & ((boolean)(ComFilterMask)))
            != (signalBoolRuntimeBuffPtr[SignalBufferId] & ((boolean)(ComFilterMask))))
        {
            filterOk = TRUE;
        }
        break;
#endif
#if (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE)
    case COM_UINT8:
    case COM_SINT8:
        if ((((uint8)(SignalValue)) & ((uint8)(ComFilterMask)))
            != (signal8BitRuntimeBuffPtr[SignalBufferId] & ((uint8)(ComFilterMask))))
        {
            filterOk = TRUE;
        }
        break;
#endif
#if (0u < COM_MAX_SIGNAL_16BITBUFF_SIZE)
    case COM_UINT16:
    case COM_SINT16:
        if ((((uint16)(SignalValue)) & ((uint16)(ComFilterMask)))
            != (signalBit16RuntimeBuffPtr[SignalBufferId] & ((uint16)(ComFilterMask))))
        {
            filterOk = TRUE;
        }
        break;
#endif
#if (0u < COM_MAX_SIGNAL_32BITBUFF_SIZE)
    case COM_UINT32:
    case COM_SINT32:
        if ((((uint32)(SignalValue)) & ((uint32)(ComFilterMask)))
            != (signal32BitRuntimeBuffPtr[SignalBufferId] & ((uint32)(ComFilterMask))))
        {
            filterOk = TRUE;
        }
        break;
#endif
#if (0u < COM_MAX_SIGNAL_64BITBUFF_SIZE)
    case COM_UINT64:
    case COM_SINT64:
        if (((SignalValue) & (ComFilterMask)) != (signal64BitRuntimeBuffPtr[SignalBufferId] & (ComFilterMask)))
        {
            filterOk = TRUE;
        }
        break;
#endif
    default:
        /*do nothing*/
        break;
    }
    COM_NOUSED(SignalValue);
    COM_NOUSED(SignalBufferId);
    COM_NOUSED(ComFilterMask);
    return filterOk;
}
#endif

/**
 * @brief substitute signal/group signal buffer
 *
 */
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_SUBSTITUTE_ENABLE) \
    || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)
void Com_SubstituteSignalBuffer(
    Com_SignalType SignalType,
    uint16         SignalInitValueId,
    uint16         ComSignalLength,
    uint16         SignalSubstituteValueId,
    uint16         PartitionId)
{
#if (0u < COM_MAX_SIGNAL_BOOLBUFF_SIZE)
    boolean* signalBoolRuntimeBuffPtr = Com_SignalBoolRuntimeBuff[PartitionId];
#endif
#if (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE)
    uint8* signal8BitRuntimeBuffPtr = Com_Signal8BitRuntimeBuff[PartitionId];
#endif
#if (0u < COM_MAX_SIGNAL_16BITBUFF_SIZE)
    uint16* signalBit16RuntimeBuffPtr = Com_SignalBit16RuntimeBuff[PartitionId];
#endif
#if (0u < COM_MAX_SIGNAL_32BITBUFF_SIZE)
    uint32* signal32BitRuntimeBuffPtr = Com_Signal32BitRuntimeBuff[PartitionId];
#endif
#if (0u < COM_MAX_SIGNAL_64BITBUFF_SIZE)
    uint64* signal64BitRuntimeBuffPtr = Com_Signal64BitRuntimeBuff[PartitionId];
#endif
    const Com_SignalSubstituteValueType* ComSignalSubstitueteValuePtr = Com_ConfigStd->SignalSubstitueteValuePtr;

    SchM_Enter_Com_Context();
    switch (SignalType)
    {
/*According to the signal type,
 *Copy the signal substitute value to signal runtime buffer*/
#if (0u < COM_MAX_SIGNAL_BOOL_SUBSTITUTE_SIZE)
    case COM_BOOLEAN:
        signalBoolRuntimeBuffPtr[SignalInitValueId] =
            ComSignalSubstitueteValuePtr->SignalBoolSubstituteValue[SignalSubstituteValueId];
        break;
#endif
#if (0u < COM_MAX_SIGNAL_8BIT_SUBSTITUTE_SIZE)
    case COM_UINT8_N:
    case COM_UINT8_DYN:
        (void)IStdLib_MemCpy(
            &signal8BitRuntimeBuffPtr[SignalInitValueId],
            &ComSignalSubstitueteValuePtr->Signal8BitSubstituteValue[SignalSubstituteValueId],
            ComSignalLength);
        break;
    case COM_UINT8:
    case COM_SINT8:
        signal8BitRuntimeBuffPtr[SignalInitValueId] =
            ComSignalSubstitueteValuePtr->Signal8BitSubstituteValue[SignalSubstituteValueId];
        break;
#endif
#if (0u < COM_MAX_SIGNAL_16BIT_SUBSTITUTE_SIZE)
    case COM_UINT16:
    case COM_SINT16:
        signalBit16RuntimeBuffPtr[SignalInitValueId] =
            ComSignalSubstitueteValuePtr->SignalBit16SubstituteValue[SignalSubstituteValueId];
        break;
#endif
#if (0u < COM_MAX_SIGNAL_32BIT_SUBSTITUTE_SIZE)
    case COM_UINT32:
    case COM_SINT32:
    case COM_FLOAT32:
        signal32BitRuntimeBuffPtr[SignalInitValueId] =
            ComSignalSubstitueteValuePtr->Signal32BitSubstituteValue[SignalSubstituteValueId];
        break;
#endif
#if (0u < COM_MAX_SIGNAL_64BIT_SUBSTITUTE_SIZE)
    case COM_UINT64:
    case COM_SINT64:
    case COM_FLOAT64:
        signal64BitRuntimeBuffPtr[SignalInitValueId] =
            ComSignalSubstitueteValuePtr->Signal64BitSubstituteValue[SignalSubstituteValueId];
        break;
#endif
    default:
        /*do nothing*/
        break;
    }
    SchM_Exit_Com_Context();
    COM_NOUSED(SignalType);
    COM_NOUSED(SignalInitValueId);
    COM_NOUSED(ComSignalLength);
    COM_NOUSED(SignalSubstituteValueId);
}
#endif
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
/* ========================================== internal function definitions ========================================= */
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/**
 * @brief Called by Com_SignalUnPackHandle for Endian convert,Swap byte mirror
 *
 */
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
COM_LOCAL void Com_EndianSwap(void* dataPtr, uint8 size)
/* PRQA S 3673 -- */
{
    uint8* data = (uint8*)dataPtr;
    uint8  i;
    uint8  temp;
    uint8  loopEndIdx  = size / 2u;
    uint8  arrayEndIdx = size - 1u;
    uint8  offset;

    for (i = 0u; i < loopEndIdx; ++i)
    {
        offset       = arrayEndIdx - i;
        temp         = data[offset];
        data[offset] = data[i];
        data[i]      = temp;
    }
}
/**
 * @brief unpack all byte value(group signal/signal cover),is called by Com_SignalUnPackHandle
 *
 */
#if (0u < COM_MAX_RXIPDUBUFF_SIZE) \
    || ((STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && (COM_MAX_TXSIGNALGROUP_NUMBER > 0u))
COM_LOCAL uint64 Com_SignalUnPackDataHandle(
    Com_SignalPositionType   SigLsbBytePos,
    uint8                    SignalByteLength,
    const uint8*             IPduBufferPtr,
    Com_SignalEndiannessType SignalEndianness)
{
    uint64 signalValue       = 0u;
    uint16 destStartIdx      = 0u;
    uint8  tSignalByteLength = (SignalByteLength > 8u) ? 8u : SignalByteLength;
#if (TRUE == CPU_32_WITH_16_ADR)
    uint8 bufPtr[8u] = {0u};
#elif (FALSE == CPU_32_WITH_16_ADR)
    uint8* bufPtr = (uint8*)&signalValue;
#if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
    destStartIdx = (uint16)(8u - tSignalByteLength);
#endif
#endif
    uint16 srcStartIdx = (COM_BIG_ENDIAN == SignalEndianness)
                             ? ((uint16)SigLsbBytePos - (uint16)tSignalByteLength + (uint16)1u)
                             : (uint16)SigLsbBytePos;

    SchM_Enter_Com_Context();
    (void)IStdLib_MemCpy(&bufPtr[destStartIdx], &IPduBufferPtr[srcStartIdx], tSignalByteLength);
    SchM_Exit_Com_Context();

#if (TRUE == CPU_32_WITH_16_ADR)
    if (COM_BIG_ENDIAN == SignalEndianness)
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
    if (COM_BIG_ENDIAN == SignalEndianness)
    {
        Com_EndianSwap(&bufPtr[destStartIdx], tSignalByteLength);
    }
#else
    if (COM_LITTLE_ENDIAN == SignalEndianness)
    {
        Com_EndianSwap(&bufPtr[destStartIdx], tSignalByteLength);
    }
#endif
#endif
    return signalValue;
}
#endif
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
/* PRQA S 0316,2844 -- */
/* PRQA S 2814,2824 -- */
/* PRQA S 3120,3132 -- */
/*******************************************************************************
**                      End of file                                           **
*******************************************************************************/
