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
**  FILENAME    : Com_GwInternal.c                                            **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : internal implementation for COM gateway signal              **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Com_Internal.h"

#if COM_GWMAPPING_NUMBER > 0u
/*******************************************************************************
**                      Internal Data Definitions                             **
*******************************************************************************/
#define COM_START_SEC_VAR_NO_INIT_BOOLEAN
#include "Com_MemMap.h"
static boolean Com_GwDestinationRequest[COM_GW_DESTINATION_NUM];
#define COM_STOP_SEC_VAR_NO_INIT_BOOLEAN
#include "Com_MemMap.h"

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/*Get the GW Signal's New value and Old value*/
static FUNC(uint64, COM_CODE) Com_GwSignalNewOldValueGet(
    Com_SignalType SignalType,
    uint16 GWSignalBufferId,
    uint16 TxSignalValueId,
    uint64* SourceSignalValue);

#if (                                                                            \
    ((0u < COM_TXSIGNAL_NUMBER) || (0u < COM_GW_DESTINATION_DESCRIPTION_NUMBER)) \
    || ((0u < COM_TXGROUPSIGNAL_NUMBER) && (0u < COM_TXSIGNALGROUP_NUMBER)))
/*gateway pdu(which include gateway signal and group signal) handle*/
static FUNC(void, COM_CODE) Com_GwSignalOfPduHandle(
    Com_TxIPduRunTimeStateType* TxIpduStatePtr,
    Com_TransferPropertyType SignalTxProperty,
    uint16 RptNum,
    boolean ValueChanged);
#endif
/* called by Com_GwSignal.
 * gateway signal/group signal/description signal which signal type is COM_UINT8_DYN*/
#if (0u < COM_GWSIGNAL_8BITBUFF_SIZE)                                                                               \
    && ((STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE)            \
        || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE) \
        || (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_DYN_ENABLE))
static FUNC(void, COM_CODE) Com_GwUint8NDYNSignal(
    Com_SignalType SignalType,
    uint16 SignalBufferId,
    uint16 SignalLength,
    Com_SignalConfigType DestSignalType,
    Com_SignalIdType DestSignalId);
#endif

/* gateway signal/group signal/description signal
 * which signal type isn't COM_UINT8_N and COM_UINT8_DYN*/
#if ((0u < COM_GWMAPPING_NUMBER) && (0u < COM_RXIPDU_NUMBER) && (0u < COM_TXIPDU_NUMBER))
static void Com_GwNotArraySignal(
    Com_SignalType SignalType,
    uint16 GWSignalBufferId,
    Com_SignalConfigType DestSignalType,
    Com_SignalIdType DestSignalId);
#endif
/* called by Com_MainFunctionRouteSignals.
 * gateway signal/group signal/signal group/description signal*/
#if ((0u < COM_GWMAPPING_NUMBER) && (0u < COM_RXIPDU_NUMBER) && (0u < COM_TXIPDU_NUMBER))
static FUNC(void, COM_CODE)
    Com_GwSignal(Com_SignalConfigType GWSourceSignalType, Com_SignalIdType GWSourceSignalId, Com_SignalIdType GWDestId);
#endif
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/******************************************************************************/
/*
 * Brief               Called by Com_GwSignal.
 *                     gateway signal/group signal/description signal which
 *                     signal type isn't COM_UINT8_N and COM_UINT8_DYN
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalType,GWSignalBufferId,DestSignalType,DestSignalId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if ((0u < COM_GWMAPPING_NUMBER) && (0u < COM_RXIPDU_NUMBER) && (0u < COM_TXIPDU_NUMBER))
static void Com_GwNotArraySignal(
    Com_SignalType SignalType,
    uint16 GWSignalBufferId,
    Com_SignalConfigType DestSignalType,
    Com_SignalIdType DestSignalId)
{
    const Com_TxIPduType* txIpduPtr;
    Com_TxIPduRunTimeStateType* txIpduStatePtr;
    boolean valueChanged = FALSE;
    uint64 sourceSignalValue = 0u;
    uint64 destSignalValue;
    PduIdType txPduId;
    uint16 txSignalValueId;
    Com_TxIpduBufIdType ipduBufferIndex;
#if (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
    Com_SignalPositionType updateBitPosition = COM_UNUSED_SIGNALPOSITION;
    uint8 updateBitMask;
#endif /* STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE */
    Com_TransferPropertyType signalTxProperty;
    uint16 rptNum = 1u;
#if ((0u < COM_TXGROUPSIGNAL_NUMBER) && (0u < COM_TXSIGNALGROUP_NUMBER))
    Com_TxSignalGroupIdType txSignalGroupId;
    const Com_TxGroupSignalType* txGroupSignalPtr;
    const Com_TxSignalGroupType* txSignalGroupPtr;
#endif
#if ((0u < COM_TXSIGNAL_NUMBER) || (0u < COM_GW_DESTINATION_DESCRIPTION_NUMBER))
    const Com_TxSignalType* txSignalPtr;
    const Com_GwDestSignalType* gwDestSignalPtr;
#endif
    boolean notArrSigIsTrue = FALSE;

    switch (DestSignalType)
    {
#if (0u < COM_TXSIGNAL_NUMBER)
    case COM_TX_SIGNAL:
        txSignalPtr = &Com_ConfigStd->ComTxSignal[DestSignalId];
        txSignalValueId = txSignalPtr->ComSignalInitValueId;
        txPduId = txSignalPtr->ComIpduRefIndex;
        txIpduPtr = &Com_ConfigStd->ComTxIPdu[txPduId];
#if (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE) && (STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE)
        updateBitPosition = txSignalPtr->ComUpdateLsbBytePos;
        updateBitMask = txSignalPtr->ComUpdateBitMask;
#endif
        signalTxProperty = txSignalPtr->ComTransferProperty;
        ipduBufferIndex = txIpduPtr->ComTxIPduBufIndex;
        SchM_Enter_Com_MultiCore_Lock();
        destSignalValue = Com_GwSignalNewOldValueGet(SignalType, GWSignalBufferId, txSignalValueId, &sourceSignalValue);
        SchM_Exit_Com_MultiCore_Lock();
        if (sourceSignalValue != destSignalValue)
        {
            valueChanged = TRUE;
/*pack the new value*/
#if (0u < COM_TXIPDUBUFF_SIZE)
            Com_TxSignalPackHandle(
                COM_TX_SIGNAL,
                txSignalPtr,
                sourceSignalValue,
                &Com_TxIPduRuntimeBuff[ipduBufferIndex]);
#endif
        }
        notArrSigIsTrue = TRUE;
        break;
#endif
#if ((0u < COM_TXGROUPSIGNAL_NUMBER) && (0u < COM_TXSIGNALGROUP_NUMBER))
    case COM_TX_GROUP_SIGNAL:
        txGroupSignalPtr = &Com_ConfigStd->ComTxGroupSignal[DestSignalId];
        txSignalValueId = txGroupSignalPtr->ComSignalInitValueId;
        txSignalGroupId = txGroupSignalPtr->ComSignalGroupRef;
        txSignalGroupPtr = &Com_ConfigStd->ComTxSignalGroup[txSignalGroupId];
        txPduId = txSignalGroupPtr->ComIpduRefIndex;
        txIpduPtr = &Com_ConfigStd->ComTxIPdu[txPduId];
#if (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE) && (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE)
        updateBitPosition = txSignalGroupPtr->ComUpdateLsbBytePos;
        updateBitMask = txSignalGroupPtr->ComUpdateBitMask;
#endif
        ipduBufferIndex = txIpduPtr->ComTxIPduBufIndex;
        signalTxProperty = (COM_GROUPSIGNAL_TMC_NONE != txGroupSignalPtr->ComTransferProperty)
                               ? txGroupSignalPtr->ComTransferProperty
                               : txSignalGroupPtr->ComTransferProperty;
        SchM_Enter_Com_MultiCore_Lock();
        destSignalValue = Com_GwSignalNewOldValueGet(SignalType, GWSignalBufferId, txSignalValueId, &sourceSignalValue);
        SchM_Exit_Com_MultiCore_Lock();
        if (sourceSignalValue != destSignalValue)
        {
            valueChanged = TRUE;
/*pack the new value*/
#if (0u < COM_TXIPDUBUFF_SIZE)
            Com_TxSignalPackHandle(
                COM_TX_GROUP_SIGNAL,
                txGroupSignalPtr,
                sourceSignalValue,
                &Com_TxIPduRuntimeBuff[ipduBufferIndex]);
#endif
        }
        notArrSigIsTrue = TRUE;
        break;
#endif
#if (0u < COM_GW_DESTINATION_DESCRIPTION_NUMBER)
    case COM_TX_DESCRIPTION:
        gwDestSignalPtr = &Com_ConfigStd->ComDestSignal[DestSignalId];
        txSignalValueId = gwDestSignalPtr->ComSignalInitValueId;
        txPduId = gwDestSignalPtr->ComGwIPduRef;
        txIpduPtr = &Com_ConfigStd->ComTxIPdu[txPduId];
#if (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
        updateBitPosition = gwDestSignalPtr->ComUpdateLsbBytePos;
        updateBitMask = gwDestSignalPtr->ComUpdateBitMask;
#endif
        signalTxProperty = gwDestSignalPtr->ComTransferProperty;
        ipduBufferIndex = txIpduPtr->ComTxIPduBufIndex;
        SchM_Enter_Com_MultiCore_Lock();
        destSignalValue = Com_GwSignalNewOldValueGet(SignalType, GWSignalBufferId, txSignalValueId, &sourceSignalValue);
        SchM_Exit_Com_MultiCore_Lock();
        if (sourceSignalValue != destSignalValue)
        {
            valueChanged = TRUE;
/*pack the new value*/
#if (0u < COM_TXIPDUBUFF_SIZE)
            Com_TxSignalPackHandle(
                COM_TX_DESCRIPTION,
                gwDestSignalPtr,
                sourceSignalValue,
                &Com_TxIPduRuntimeBuff[ipduBufferIndex]);
#endif
        }
        notArrSigIsTrue = TRUE;
        break;
#endif
    default:
        /* do nothing */
        break;
    }

    if (notArrSigIsTrue)
    {
#if (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
        /*if configuration update bit,set update bit to 1*/
        Com_SetSignalUpdateBit(ipduBufferIndex, updateBitPosition, updateBitMask);
#endif

        txIpduStatePtr = &Com_TxIPduRunTimeState[txPduId];
        switch (DestSignalType)
        {
#if (0u < COM_TXSIGNAL_NUMBER)
        case COM_TX_SIGNAL:
#if (COM_TXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)
            if (COM_UNUSED_TXSIGNALFILTERID != txSignalPtr->ComFilterIndex)
            {
                Com_TxSignalTMCRunTime[txSignalPtr->ComTMCBufferId] = Com_DestSignalTMCCalculate(
                    SignalType,
                    txSignalPtr->ComFilterAlgorithm,
                    (Com_SignalIdType)txSignalPtr->ComFilterIndex,
#if (COM_TXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
                    destSignalValue,
#endif
                    sourceSignalValue);
            }
#endif
            /*calculate the TMS,store new Tx mode,get rptNum value for DIRECT/MIXED tx mode,if TMS changed
             * will reset periodic mode counter.*/
            Com_TxIpduTMSHandle(txIpduStatePtr, txIpduPtr, &rptNum);
            /*handle the tx pdu (which include the gateway signal) transmit parameter*/
            Com_GwSignalOfPduHandle(txIpduStatePtr, signalTxProperty, rptNum, valueChanged);
/*the tx signal configuration timeout*/
#if (STD_ON == COM_TX_SIGNAL_TIMEOUT_ENABLE)
            if (COM_UNUSED_TXSIGNALTIMEOUTID != txSignalPtr->ComTimeoutIndex)
            {
                if ((0u == txIpduStatePtr->DMCnt) || (1u < txIpduStatePtr->RptNum))
                {
                    txIpduStatePtr->DMCnt = Com_ConfigStd->ComTxIPdu[txPduId].ComTxIpduDM;
                }
            }
#endif /* STD_ON == COM_TX_SIGNAL_TIMEOUT_ENABLE */
            break;
#endif /* 0u < COM_TXSIGNAL_NUMBER */
#if (0u < COM_TXGROUPSIGNAL_NUMBER) && (0u < COM_TXSIGNALGROUP_NUMBER)
        case COM_TX_GROUP_SIGNAL:
#if (COM_TXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
            if (COM_UNUSED_TXGRPSIGFILTERID != txGroupSignalPtr->ComFilterIndex)
            {
                Com_TxSignalTMCRunTime[txGroupSignalPtr->ComTMCBufferId] = Com_DestSignalTMCCalculate(
                    SignalType,
                    txGroupSignalPtr->ComFilterAlgorithm,
                    (Com_SignalIdType)txGroupSignalPtr->ComFilterIndex,
#if (COM_TXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
                    destSignalValue,
#endif
                    sourceSignalValue);
            }
#endif
            /*calculate the TMS,store new Tx mode,get rptNum value for DIRECT/MIXED tx mode,if TMS changed
             * will reset periodic mode counter.*/
            Com_TxIpduTMSHandle(txIpduStatePtr, txIpduPtr, &rptNum);
            /*handle the tx pdu (which include the gateway group signal) transmit parameter*/
            Com_GwSignalOfPduHandle(txIpduStatePtr, signalTxProperty, rptNum, valueChanged);
/*the tx signal group(include the gateway group signal)configuration timeout*/
#if (STD_ON == COM_TX_SIG_GROUP_TIMEOUT_ENABLE)
            if (COM_UNUSED_TXSIGGRPTIMEOUTID != txSignalGroupPtr->ComTimeoutIndex)
            {
                if ((0u == txIpduStatePtr->DMCnt) || (1u < txIpduStatePtr->RptNum))
                {
                    txIpduStatePtr->DMCnt = txIpduPtr->ComTxIpduDM;
                }
            }
#endif /* STD_ON == COM_TX_SIG_GROUP_TIMEOUT_ENABLE */
            break;
#endif /* 0u < COM_TXGROUPSIGNAL_NUMBER && 0u < COM_TXSIGNALGROUP_NUMBER */
#if (0u < COM_GW_DESTINATION_DESCRIPTION_NUMBER)
        case COM_TX_DESCRIPTION:
#if (COM_DESTSIG_FILTERTYPE_MAX_NUMBER > 0u)
            if (COM_UNUSED_DESTSIGNALFILTERID != gwDestSignalPtr->ComFilterIndex)
            {
                Com_TxSignalTMCRunTime[gwDestSignalPtr->ComTMCBufferId] = Com_DestSignalTMCCalculate(
                    SignalType,
                    gwDestSignalPtr->ComFilterAlgorithm,
                    (Com_SignalIdType)gwDestSignalPtr->ComFilterIndex,
#if (COM_TXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
                    destSignalValue,
#endif
                    sourceSignalValue);
            }
#endif
            /*calculate the TMS,store new Tx mode,get rptNum value for DIRECT/MIXED tx mode,if TMS changed
             * will reset periodic mode counter.*/
            Com_TxIpduTMSHandle(txIpduStatePtr, txIpduPtr, &rptNum);
            /*handle the tx pdu (which include the gateway signal) transmit parameter*/
            Com_GwSignalOfPduHandle(txIpduStatePtr, signalTxProperty, rptNum, valueChanged);
            break;
#endif /* 0u < COM_GW_DESTINATION_DESCRIPTION_NUMBER */
        default:
            /*do nothing*/
            break;
        }
    }
    return;
}
#endif
/******************************************************************************/
/*
 * Brief               Called by Com_GwSignal.
 *                     gateway signal/group signal/description signal which
 *                     signal type is COM_UINT8_DYN,UINT8_N
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalType,SignalBufferId,SignalLength,DestSignalType,DestSignalId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if (0u < COM_GWSIGNAL_8BITBUFF_SIZE)                                                                               \
    && ((STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE)            \
        || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE) \
        || (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_DYN_ENABLE))
static FUNC(void, COM_CODE) Com_GwUint8NDYNSignal(
    Com_SignalType SignalType,
    uint16 SignalBufferId,
    uint16 SignalLength,
    Com_SignalConfigType DestSignalType,
    Com_SignalIdType DestSignalId)
{
#if (0u < COM_TXIPDUBUFF_SIZE)
    boolean valueChanged = FALSE;
    PduIdType txPduId;
    uint16 rptNum = 1u;
    Com_SignalPositionType sigLsbBytePos;
#if (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
    Com_SignalPositionType updateBitPosition = COM_UNUSED_SIGNALPOSITION;
    uint8 updateBitMask;
#endif
    Com_TransferPropertyType signalTxProperty;
#if ((0u < COM_TXSIGNAL_NUMBER) || (0u < COM_GW_DESTINATION_DESCRIPTION_NUMBER))
    const Com_TxSignalType* txSignalPtr;
    const Com_GwDestSignalType* gwDestSignalPtr;
#endif
#if ((0u < COM_TXGROUPSIGNAL_NUMBER) && (COM_TXSIGNALGROUP_NUMBER))
    Com_TxSignalGroupIdType txSignalGroupId;
    const Com_TxSignalGroupType* txSignalGroupPtr;
    const Com_TxGroupSignalType* txGroupSignalPtr;
#endif

    switch (DestSignalType)
    {
#if (0u < COM_TXSIGNAL_NUMBER)
    case COM_TX_SIGNAL:
        txSignalPtr = &Com_ConfigStd->ComTxSignal[DestSignalId];
        signalTxProperty = txSignalPtr->ComTransferProperty;
        txPduId = txSignalPtr->ComIpduRefIndex;
        sigLsbBytePos = txSignalPtr->ComSigLsbBytePos;
#if (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE) && (STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE)
        updateBitPosition = txSignalPtr->ComUpdateLsbBytePos;
        updateBitMask = txSignalPtr->ComUpdateBitMask;
#endif
        break;
#endif
#if ((0u < COM_TXGROUPSIGNAL_NUMBER) && (0u < COM_TXSIGNALGROUP_NUMBER))
    case COM_TX_GROUP_SIGNAL:
        txGroupSignalPtr = &Com_ConfigStd->ComTxGroupSignal[DestSignalId];
        txSignalGroupId = txGroupSignalPtr->ComSignalGroupRef;
        txSignalGroupPtr = &Com_ConfigStd->ComTxSignalGroup[txSignalGroupId];
        txPduId = txSignalGroupPtr->ComIpduRefIndex;
        sigLsbBytePos = txGroupSignalPtr->ComSigLsbBytePos;
        signalTxProperty = (COM_GROUPSIGNAL_TMC_NONE != txGroupSignalPtr->ComTransferProperty)
                               ? txGroupSignalPtr->ComTransferProperty
                               : txSignalGroupPtr->ComTransferProperty;
#if (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE) && (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE)
        updateBitPosition = txSignalGroupPtr->ComUpdateLsbBytePos;
        updateBitMask = txSignalGroupPtr->ComUpdateBitMask;
#endif
        break;
#endif
#if (0u < COM_GW_DESTINATION_DESCRIPTION_NUMBER)
    case COM_TX_DESCRIPTION:
        gwDestSignalPtr = &Com_ConfigStd->ComDestSignal[DestSignalId];
        txPduId = gwDestSignalPtr->ComGwIPduRef;
        sigLsbBytePos = gwDestSignalPtr->ComSigLsbBytePos;
        signalTxProperty = gwDestSignalPtr->ComTransferProperty;
#if (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
        updateBitPosition = gwDestSignalPtr->ComUpdateLsbBytePos;
        updateBitMask = gwDestSignalPtr->ComUpdateBitMask;
#endif
        break;
#endif
    default:
        /*do nothing*/
        break;
    }
#if (0u < COM_TXSIGNAL_NUMBER) || ((0u < COM_TXGROUPSIGNAL_NUMBER) && (0u < COM_TXSIGNALGROUP_NUMBER)) \
    || (0u < COM_GW_DESTINATION_DESCRIPTION_NUMBER)
    /* PRQA S 2962 ++ */ /* MISRA Rule 9.1 */
    Com_TxIPduRunTimeStateType* txIpduStatePtr = &Com_TxIPduRunTimeState[txPduId];
    const Com_TxIPduType* txIpduPtr = &Com_ConfigStd->ComTxIPdu[txPduId];
    Com_TxIpduBufIdType txIPduBufId = txIpduPtr->ComTxIPduBufIndex + (Com_TxIpduBufIdType)sigLsbBytePos;
    uint16 SigBufId = (COM_UINT8_DYN == SignalType) ? (SignalBufferId + (uint16)sizeof(SignalLength)) : SignalBufferId;
    uint16 len = SignalLength;
    /* PRQA S 2962 -- */ /* MISRA Rule 9.1 */
    SchM_Enter_Com_MultiCore_Lock();
    SchM_Enter_Com_Context();
    if (COM_UINT8_DYN == SignalType)
    {
        (void)ILib_memcpy(&len, &Com_GWSignal8BitBuff[SignalBufferId], sizeof(SignalLength));
    }
    if (0 != ILib_memcmp(&Com_TxIPduRuntimeBuff[txIPduBufId], &Com_GWSignal8BitBuff[SigBufId], len))
    {
        (void)ILib_memcpy(&Com_TxIPduRuntimeBuff[txIPduBufId], &Com_GWSignal8BitBuff[SigBufId], len);
        valueChanged = TRUE;
    }
    SchM_Exit_Com_Context();
    SchM_Exit_Com_MultiCore_Lock();
    if (COM_UINT8_DYN == SignalType)
    {
        uint16 destSignalLength = txIpduStatePtr->TxIpduLength - (uint16)txIpduPtr->IPduNoDynSignalLength;
        if (len != destSignalLength)
        {
            valueChanged = TRUE;
            txIpduStatePtr->TxIpduLength = (uint16)txIpduPtr->IPduNoDynSignalLength + len;
        }
    }
#if (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
    /*if configuration update bit,set update bit to 1*/
    Com_SetSignalUpdateBit(txIPduBufId, updateBitPosition, updateBitMask);
#endif /* STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE */
    /*calculate the TMS,store new Tx mode,get rptNum value for DIRECT/MIXED tx mode,if TMS changed will
     * reset periodic mode counter.*/
    Com_TxIpduTMSHandle(txIpduStatePtr, txIpduPtr, &rptNum);
#endif

    switch (DestSignalType)
    {
#if (0u < COM_TXSIGNAL_NUMBER)
    case COM_TX_SIGNAL:
        /*handle the tx pdu (which include the gateway signal) transmit parameter*/
        Com_GwSignalOfPduHandle(txIpduStatePtr, signalTxProperty, rptNum, valueChanged);
/*the tx signal configuration timeout*/
#if (STD_ON == COM_TX_SIGNAL_TIMEOUT_ENABLE)
        if (COM_UNUSED_TXSIGNALTIMEOUTID != txSignalPtr->ComTimeoutIndex)
        {
            if ((0u == txIpduStatePtr->DMCnt) || (1u < txIpduStatePtr->RptNum))
            {
                txIpduStatePtr->DMCnt = txIpduPtr->ComTxIpduDM;
            }
        }
#endif /* STD_ON == COM_TX_SIGNAL_TIMEOUT_ENABLE */
        break;
#endif
#if ((0u < COM_TXGROUPSIGNAL_NUMBER) && (0u < COM_TXSIGNALGROUP_NUMBER))
    case COM_TX_GROUP_SIGNAL:
        /*handle the tx pdu (which include the gateway group signal) transmit parameter*/
        Com_GwSignalOfPduHandle(txIpduStatePtr, signalTxProperty, rptNum, valueChanged);
/*the tx signal group(include the gateway group signal)configuration timeout*/
#if (STD_ON == COM_TX_SIG_GROUP_TIMEOUT_ENABLE)
        if (COM_UNUSED_TXSIGGRPTIMEOUTID != txSignalGroupPtr->ComTimeoutIndex)
        {
            if ((0u == txIpduStatePtr->DMCnt) || (1u < txIpduStatePtr->RptNum))
            {
                txIpduStatePtr->DMCnt = txIpduPtr->ComTxIpduDM;
            }
        }
#endif /* STD_ON == COM_TX_SIG_GROUP_TIMEOUT_ENABLE */
        break;
#endif
#if (0u < COM_GW_DESTINATION_DESCRIPTION_NUMBER)
    case COM_TX_DESCRIPTION:
        /*handle the tx pdu (which include the gateway description signal) transmit parameter*/
        Com_GwSignalOfPduHandle(txIpduStatePtr, signalTxProperty, rptNum, valueChanged);
        break;
#endif
    default:
        /*do nothing*/
        break;
    }
#endif /*0u < COM_TXIPDUBUFF_SIZE*/
    return;
}
#endif
#if (                                                                            \
    ((0u < COM_TXSIGNAL_NUMBER) || (0u < COM_GW_DESTINATION_DESCRIPTION_NUMBER)) \
    || ((0u < COM_TXGROUPSIGNAL_NUMBER) && (0u < COM_TXSIGNALGROUP_NUMBER)))
/******************************************************************************/
/*
 * Brief               gateway pdu(which include gateway signal and group signal) handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxPduId,SignalTxProperty,RptNum,ValueChanged
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE) Com_GwSignalOfPduHandle(
    Com_TxIPduRunTimeStateType* TxIpduStatePtr,
    Com_TransferPropertyType SignalTxProperty,
    uint16 RptNum,
    boolean ValueChanged)
{
    Com_TxModeModeType ipduTxMode = TxIpduStatePtr->ipduTxMode;

#if (0u < COM_TX_MODE_TRUE_DIRECT_NUMBER) || (0u < COM_TX_MODE_TRUE_MIXED_NUMBER)                              \
    || (0u < COM_TX_MODE_FALSE_DIRECT_NUMBER) || (0u < COM_TX_MODE_FALSE_MIXED_NUMBER)                         \
    || (0u < COM_TX_MODE_TRUE_DIRECT_NOREPETITION_NUMBER) || (0u < COM_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER) \
    || (0u < COM_TX_MODE_FALSE_DIRECT_NOREPETITION_NUMBER) || (0u < COM_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER)
    if ((COM_TX_MODE_DIRECT == ipduTxMode) || (COM_TX_MODE_MIXED == ipduTxMode)
        || (COM_TX_MODE_DIRECT_WITHOUT_REPETITION == ipduTxMode)
        || (COM_TX_MODE_MIXED_WITHOUT_REPETITION == ipduTxMode))
    {
        switch (SignalTxProperty)
        {
        case COM_TRIGGERED:
            TxIpduStatePtr->NTimeCnt = 0u;
            TxIpduStatePtr->RptNum = RptNum;
            break;
        case COM_TRIGGERED_ON_CHANGE:
            if (ValueChanged)
            {
                TxIpduStatePtr->NTimeCnt = 0u;
                TxIpduStatePtr->RptNum = RptNum;
            }
            break;
        case COM_TRIGGERED_ON_CHANGE_WITHOUT_REPETITION:
            if (ValueChanged)
            {
                TxIpduStatePtr->NTimeCnt = 0u;
                TxIpduStatePtr->RptNum = 1u;
            }
            break;
        case COM_TRIGGERED_WITHOUT_REPETITION:
            TxIpduStatePtr->NTimeCnt = 0u;
            TxIpduStatePtr->RptNum = 1u;
            break;
        default:
            /*do nothing*/
            break;
        }
    }
    else
#endif
    {
        TxIpduStatePtr->NTimeCnt = 0u;
        TxIpduStatePtr->RptNum = 0u;
    }
    COM_NOUSED(SignalTxProperty);
    COM_NOUSED(ValueChanged);
    return;
}
#endif

/******************************************************************************/
/*
 * Brief               Get the GW Signal's New value and Old value
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalType,GWSignalBufferId,TxSignalValueId
 * Param-Name[out]     SourceSignalValue
 * Param-Name[in/out]  None
 * Return              uint64
 */
/******************************************************************************/
static FUNC(uint64, COM_CODE) Com_GwSignalNewOldValueGet(
    Com_SignalType SignalType,
    uint16 GWSignalBufferId,
    uint16 TxSignalValueId,
    uint64* SourceSignalValue)
{
    uint64 destSignalValue = 0u;
    switch (SignalType)
    {
#if (0u < COM_SIGNAL_BOOLBUFF_SIZE) && (0u < COM_GWSIGNAL_BOOLBUFF_SIZE)
    case COM_BOOLEAN:
        *SourceSignalValue = Com_GWSignalBoolBuff[GWSignalBufferId] ? 1u : 0u;
        destSignalValue = Com_SignalBoolRuntimeBuff[TxSignalValueId] ? 1u : 0u;
        Com_SignalBoolRuntimeBuff[TxSignalValueId] = Com_GWSignalBoolBuff[GWSignalBufferId];
        break;
#endif /* 0u < COM_SIGNAL_BOOLBUFF_SIZE && 0u < COM_GWSIGNAL_BOOLBUFF_SIZE */
#if (0u < COM_SIGNAL_8BITBUFF_SIZE) && (0u < COM_GWSIGNAL_8BITBUFF_SIZE)
    case COM_UINT8:
    case COM_SINT8:
        *SourceSignalValue = (uint64)Com_GWSignal8BitBuff[GWSignalBufferId];
        destSignalValue = (uint64)Com_Signal8BitRuntimeBuff[TxSignalValueId];
        Com_Signal8BitRuntimeBuff[TxSignalValueId] = Com_GWSignal8BitBuff[GWSignalBufferId];
        break;
#endif /* 0u < COM_SIGNAL_8BITBUFF_SIZE && 0u < COM_GWSIGNAL_8BITBUFF_SIZE */
#if (0u < COM_SIGNAL_16BITBUFF_SIZE) && (0u < COM_GWSIGNAL_16BITBUFF_SIZE)
    case COM_UINT16:
    case COM_SINT16:
        *SourceSignalValue = (uint64)Com_GWSignal16BitBuff[GWSignalBufferId];
        destSignalValue = (uint64)Com_Signal16BitRuntimeBuff[TxSignalValueId];
        Com_Signal16BitRuntimeBuff[TxSignalValueId] = Com_GWSignal16BitBuff[GWSignalBufferId];
        break;
#endif /* 0u < COM_SIGNAL_16BITBUFF_SIZE && 0u < COM_GWSIGNAL_16BITBUFF_SIZE */
#if (0u < COM_SIGNAL_32BITBUFF_SIZE) && (0u < COM_GWSIGNAL_32BITBUFF_SIZE)
    case COM_UINT32:
    case COM_SINT32:
    case COM_FLOAT32:
        *SourceSignalValue = (uint64)Com_GWSignal32BitBuff[GWSignalBufferId];
        destSignalValue = (uint64)Com_Signal32BitRuntimeBuff[TxSignalValueId];
        Com_Signal32BitRuntimeBuff[TxSignalValueId] = Com_GWSignal32BitBuff[GWSignalBufferId];
        break;
#endif /* 0u < COM_SIGNAL_32BITBUFF_SIZE && 0u < COM_GWSIGNAL_32BITBUFF_SIZE */
#if (0u < COM_SIGNAL_64BITBUFF_SIZE) && (0u < COM_GWSIGNAL_64BITBUFF_SIZE)
    case COM_UINT64:
    case COM_SINT64:
    case COM_FLOAT64:
        *SourceSignalValue = Com_GWSignal64BitBuff[GWSignalBufferId];
        destSignalValue = Com_Signal64BitRuntimeBuff[TxSignalValueId];
        Com_Signal64BitRuntimeBuff[TxSignalValueId] = Com_GWSignal64BitBuff[GWSignalBufferId];
        break;
#endif /* 0u < COM_SIGNAL_64BITBUFF_SIZE && 0u < COM_GWSIGNAL_64BITBUFF_SIZE */
    default:
        /*do nothing*/
        break;
    }
    COM_NOUSED(GWSignalBufferId);
    COM_NOUSED(TxSignalValueId);
    COM_NOUSED(SourceSignalValue);
    return destSignalValue;
}

#if ((0u < COM_GWMAPPING_NUMBER) && (0u < COM_RXIPDU_NUMBER) && (0u < COM_TXIPDU_NUMBER))
/******************************************************************************/
/*
 * Brief               gateway signal/group signal/signal group/description signal
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      GWMappingId,GWSourceSignalType,GWSourceSignalId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static void Com_GwSignal(
    Com_SignalConfigType GWSourceSignalType,
    Com_SignalIdType GWSourceSignalId,
    Com_SignalIdType GWDestId)
/* PRQA S 1532,1503 -- */ /* MISRA Rule 8.7,2.1 */
{
    Com_SignalType signalType;
    uint16 gwSignalBufferId;
    Com_SignalConfigType gwDestSignalType;
    Com_SignalIdType gwDestSignalId;
    boolean gwSigIsTrue;
#if (0u < COM_GWSIGNAL_8BITBUFF_SIZE)
    uint16 signalLength = 0u;
#endif
#if (0u < COM_RXGROUPSIGNAL_NUMBER)
    const Com_RxGroupSignalType* rxGroupSignalPtr;
#endif
    const Com_GwDestType* gwDestPtr;
#if (0u < COM_GW_SOURCE_DESCRIPTION_NUMBER)
    const Com_GwSourceSignalType* gwSourceSignalPtr;
#endif /*0u < COM_GW_SOURCE_DESCRIPTION_NUMBER*/
    const Com_RxSignalType* rxSignalPtr;

    switch (GWSourceSignalType)
    {
#if (0u < COM_RXSIGNAL_NUMBER)
    case COM_RX_SIGNAL:
        rxSignalPtr = &Com_ConfigStd->ComRxSignal[GWSourceSignalId];
        signalType = rxSignalPtr->ComSignalType;
        gwSignalBufferId = rxSignalPtr->GWSignalBufferId;
#if (0u < COM_GWSIGNAL_8BITBUFF_SIZE) && (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE)
        signalLength = (COM_UINT8_N == signalType) ? rxSignalPtr->ComSignalLength : signalLength;
#endif
        gwSigIsTrue = TRUE;
        break;

#endif
#if (0u < COM_RXGROUPSIGNAL_NUMBER)
    case COM_RX_GROUP_SIGNAL:
        rxGroupSignalPtr = &Com_ConfigStd->ComRxGroupSignal[GWSourceSignalId];
        signalType = rxGroupSignalPtr->ComSignalType;
        gwSignalBufferId = rxGroupSignalPtr->GWSignalBufferId;
#if (0u < COM_GWSIGNAL_8BITBUFF_SIZE) && (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE)
        signalLength = (COM_UINT8_N == signalType) ? rxGroupSignalPtr->ComSignalLength : signalLength;
#endif
        gwSigIsTrue = TRUE;
        break;
#endif /* 0u < COM_RXGROUPSIGNAL_NUMBER */
#if (0u < COM_GW_SOURCE_DESCRIPTION_NUMBER)
    case COM_RX_DESCRIPTION:
        gwSourceSignalPtr = &Com_ConfigStd->ComSourceSignal[GWSourceSignalId];
        signalType = gwSourceSignalPtr->ComSignalType;
        gwSignalBufferId = gwSourceSignalPtr->GWSignalBufferId;
#if (0u < COM_GWSIGNAL_8BITBUFF_SIZE) && (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_N_ENABLE)
        signalLength = (COM_UINT8_N == signalType) ? gwSourceSignalPtr->ComSignalLength : signalLength;
#endif
        gwSigIsTrue = TRUE;
        break;
#endif /*0u < COM_GW_SOURCE_DESCRIPTION_NUMBER*/
    default:
        gwSigIsTrue = FALSE;
        break;
    }

    if (gwSigIsTrue)
    {
        gwDestPtr = &Com_GwDest[GWDestId];
        gwDestSignalType = gwDestPtr->ComGwDestSignalType;
        gwDestSignalId = gwDestPtr->ComGwDestSignalId;
/*Except COM_UINT8_N,COM_UINT8_DYN Type Signal*/
#if (0u < COM_GWSIGNAL_8BITBUFF_SIZE)                                                                               \
    && ((STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE)            \
        || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE) \
        || (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_DYN_ENABLE))
        if ((COM_UINT8_N == signalType) || (COM_UINT8_DYN == signalType))
        {
            Com_GwUint8NDYNSignal(signalType, gwSignalBufferId, signalLength, gwDestSignalType, gwDestSignalId);
        }
        else
#endif
        {
            Com_GwNotArraySignal(signalType, gwSignalBufferId, gwDestSignalType, gwDestSignalId);
        }
    }
    COM_NOUSED(GWSourceSignalId);
    return;
}
#endif
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
#if COM_GWMAPPING_NUMBER > 0u
/******************************************************************************/
/*
 * Brief               Signal gateway handle.
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
void Com_MainFunctionRouteSignalsHandle(Com_MainFunctionType mainFunctionId)
{
    for (Com_SignalIdType gwMappingId = 0u; gwMappingId < COM_GWMAPPING_NUMBER; ++gwMappingId)
    {
        const Com_GwMappingType* gwMappingPtr = &Com_ConfigStd->ComGwMapping[gwMappingId];
        Com_SignalIdType gwDestStart = gwMappingPtr->ComGwDestSignalStartId;
        Com_SignalIdType gwDestEnd = gwMappingPtr->ComGwDestNumber;
        for (Com_SignalIdType gwDestId = gwDestStart; gwDestId < gwDestEnd; ++gwDestId)
        {
            if ((Com_GwDest[gwDestId].ComMainFunctionId == mainFunctionId) && Com_GwDestinationRequest[gwDestId])
            {
                Com_GwDestinationRequest[gwDestId] = FALSE;
                Com_GwSignal(gwMappingPtr->ComGwSourceSignalType, gwMappingPtr->ComGwSourceSignalId, gwDestId);
            }
        }
    }
}
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */

void Com_GwInitDestinationRequest(void)
{
    (void)ILib_memset(&Com_GwDestinationRequest[0], 0, COM_GW_DESTINATION_NUM);
}

void Com_GwUpdateDestinationRequest(Com_GwMappingIdType gwMappingId, boolean request)
{
    Com_SignalIdType gwDestStart = Com_ConfigStd->ComGwMapping[gwMappingId].ComGwDestSignalStartId;
    Com_SignalIdType gwDestEnd = Com_ConfigStd->ComGwMapping[gwMappingId].ComGwDestNumber;
    for (Com_SignalIdType gwDestId = gwDestStart; gwDestId < gwDestEnd; ++gwDestId)
    {
        Com_GwDestinationRequest[gwDestId] = request;
    }
}
#endif

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
#endif /* COM_GWMAPPING_NUMBER > 0u */
/*******************************************************************************
**                      End of file                                           **
*******************************************************************************/
