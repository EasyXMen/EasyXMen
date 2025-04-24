/* PRQA S 3108++ */
/**
 * Copyright (C) 2024 Isoft Infrastructure Software Co., Ltd.
 * SPDX-License-Identifier: LGPL-2.1-only-with-exception OR  LicenseRef-Commercial-License
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the
 * GNU Lesser General Public License as published by the Free Software Foundation; version 2.1.
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License along with this library;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * or see <https://www.gnu.org/licenses/>.
 *
 * Alternatively, this file may be used under the terms of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License, in which case the provisions of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License shall apply instead of those of the GNU Lesser General Public License.
 *
 * You should have received a copy of the Isoft Infrastructure Software Co., Ltd.  Commercial License
 * along with this program. If not, please find it at <https://EasyXMen.com/xy/reference/permissions.html>
 */
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : Com_TxInternal.c                                            **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : internal implementation for COM transmit                    **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Com_Internal.h"
#include "Det.h"
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/* called by Com_TxIpduController
 * reset OCCURRENCE of filter with ComFilterAlgorithm ONE_EVERY_N*/
#if (COM_ONEEVERYNFILTERSIGNAL_NUMBER > 0u)
static FUNC(void, COM_CODE) Com_ResetTxOccurrenceOfPduFilter(PduIdType IpduId);
#endif
/* called by Com_TxIpduController
 * init the tx ipdu buffer,all signal buffer(included in the ipdu) and init the TMC*/
#if (COM_IPDUGROUP_NUMBER > 0u)
static FUNC(void, COM_CODE) Com_ResetTxPduBufferAndSignalBuffer(PduIdType IpduId);
#endif
/* called by Com_TxSignalGroupPack.
 * pack tx group signal(not dynamic)*/
#if (COM_TXSIGNALGROUP_NUMBER > 0u)
static FUNC(void, COM_CODE) Com_TxGroupSignalPack(const Com_TxGroupSignalType* TxGroupSignalPtr);
#endif /* COM_TXSIGNALGROUP_NUMBER > 0u */
/* called by Com_TxSignalTMHandle,Com_SendTxGroupSignalHandle,Com_UpdateTxBufferAndCalculateTMCOfEveryGroupSignal.
 * calculate the new TMC of the signal new value*/
#if (0u < COM_TMCTXSIGNAL_NUMBER) && (STD_ON == COM_TMS_ENABLE)
static FUNC(boolean, COM_CODE) Com_TxSignalTMCCalculate(
    Com_SignalConfigType DestSignalType,
    P2CONST(void, AUTOMATIC, COM_APPL_CONST) SignalValuePtr,
    P2CONST(void, AUTOMATIC, COM_APPL_CONST) TxSignalTMCPtr,
    uint64 SignalOldValue);
#endif
/* called by Com_TxSignalTMCCalculate.
 * change the signal value to uint64 type*/
#if (0u < COM_TMCTXSIGNAL_NUMBER) && (STD_ON == COM_TMS_ENABLE)
static FUNC(uint64, COM_CODE) Com_GetTxSignalValue(
    Com_SignalType SignalType,
    P2CONST(void, AUTOMATIC, COM_APPL_CONST) SignalValuePtr,
    uint8 BitSize);
#endif
/* called by Com_UpdateTxBufferAndCalculateTMCOfEveryGroupSignal.
 * update the uint8_n signal value*/
#if ((STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && (COM_TXSIGNALGROUP_NUMBER > 0u))
#if ((STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) && (0u < COM_TXIPDUBUFF_SIZE) && (0u < COM_SIGNAL_8BITBUFF_SIZE))
static FUNC(boolean, COM_CODE) Com_TxGroupSignalUpdateUint8N(
    Com_TxIpduBufIdType TxIPduBufferId,
    Com_SignalIdType GroupSignalId,
    uint16 GroupSignalInitValueId);
#endif
#endif
#if ((0u < COM_TXSIGNALGROUP_NUMBER) && (STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API))
/*Send Signal Group handle*/
static FUNC(void, COM_CODE) Com_SendSignalGroupArrayInternalHandle(
    const Com_TxSignalGroupType* TxSignalGroupPtr,
    Com_TxIPduRunTimeStateType* TxIpduStatePtr,
    PduIdType IpduRef,
    boolean TriggerOnChange,
    uint16 RptNum);
#endif

/*Trigger IPdu Send handle*/
#if (0u < COM_TXIPDU_NUMBER)
static FUNC(Std_ReturnType, COM_CODE) Com_TriggerIPDUSendInternalHandle(PduIdType TxIpduId);
#endif

#if (STD_ON == COM_MDT_ENABLE)
static FUNC(void, COM_CODE)
    Com_ResetTxIpduMDT(Com_TxIPduRunTimeStateType* TxIpduStatePtr, const Com_TxIPduType* TxIpduPtr);
#endif

static void Com_GetTxModeOffset(
    Com_TxIPduRunTimeStateType* TxIpduStatePtr,
    const Com_TxIPduType* TxIpduPtr,
    boolean TMS);

/* called by Com_MainFunctionTx.
 * tx pdu DM timeout notification*/
#if (STD_ON == COM_TX_SIGNAL_TIMEOUT_NOTIFICATION_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_TIMEOUT_NOTIFICATION_ENABLE)
static FUNC(void, COM_CODE) Com_TxDMTimeOutNotification(const Com_TxIPduType* TxIpduPtr);
#endif

/* called by Com_MainFunctionTx.
 * the handle of Com_MainFunctionTx,when the tx mode is direct.
 * return the boolean value for the tx pdu need to transmit or not*/
#if (0u < COM_TX_MODE_TRUE_DIRECT_NUMBER) || (0u < COM_TX_MODE_FALSE_DIRECT_NUMBER)                              \
    || (0u < COM_TX_MODE_TRUE_DIRECT_NOREPETITION_NUMBER) || (0u < COM_TX_MODE_FALSE_DIRECT_NOREPETITION_NUMBER) \
    || ((0u < COM_TX_MODE_TRUE_MIXED_NUMBER) || (0u < COM_TX_MODE_FALSE_MIXED_NUMBER)                            \
        || (0u < COM_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER) || (0u < COM_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER))
static FUNC(boolean, COM_CODE)
    Com_MainFuncTxDirect(Com_TxIPduRunTimeStateType* TxIpduStatePtr, const Com_TxIPduType* TxIpduPtr, boolean MDTRun);
#endif
/* called by Com_MainFunctionTx.
 * the handle of Com_MainFunctionTx,when the tx mode is period.
 * return the boolean value for the tx pdu need to transmit or not*/
#if (0u < COM_TX_MODE_TRUE_PERIOD_NUMBER) || (0u < COM_TX_MODE_FALSE_PERIOD_NUMBER)  \
    || (0u < COM_TX_MODE_TRUE_MIXED_NUMBER) || (0u < COM_TX_MODE_FALSE_MIXED_NUMBER) \
    || (0u < COM_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER) || (0u < COM_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER)
static FUNC(boolean, COM_CODE)
    Com_MainFuncTxPeriod(Com_TxIPduRunTimeStateType* TxIpduStatePtr, const Com_TxIPduType* TxIpduPtr, boolean MDTRun);
#endif
/* called by Com_MainFunctionTx.
 * the handle of Com_MainFunctionTx,when the tx mode is mixed.
 * return the boolean value for the tx pdu need to transmit or not*/
#if (0u < COM_TX_MODE_TRUE_MIXED_NUMBER) || (0u < COM_TX_MODE_FALSE_MIXED_NUMBER) \
    || (0u < COM_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER) || (0u < COM_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER)
static FUNC(boolean, COM_CODE)
    Com_MainFuncTxMixed(Com_TxIPduRunTimeStateType* TxIpduStatePtr, const Com_TxIPduType* TxIpduPtr, boolean MDTRun);
#endif
/* called by Com_TriggerTransmit,Com_TriggerIPDUSend,Com_TriggerIPDUSendWithMetaData,Com_TxIpduController.
 * It is called in case the transmission is not possible because the corresponding I-PDU group is stopped.*/
#if (STD_ON == COM_TX_SIGNAL_ERROR_NOTIFICATION_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_ERROR_NOTIFICATION_ENABLE)
static FUNC(void, COM_CODE) Com_InvokeErrorNotification(PduIdType IpduId);
#endif

/*Send the tx pdu in Com_MainFunctionTx*/
#if (0u < COM_TXIPDU_NUMBER)
static FUNC(void, COM_CODE) Com_MainFunction_SendPdu(PduIdType TxIpduId);
#endif
/* called by Com_MainFunctionTx,Com_TriggerIPDUSend,Com_TriggerIPDUSendWithMetaData.
 * Pack the counter value in the pdu buffer*/
#if (STD_ON == COM_TX_IPDU_COUNTER_ENABLE)
static FUNC(void, COM_CODE)
    Com_PackCounterValue(const Com_TxIPduRunTimeStateType* TxIpduStatePtr, const Com_TxIPduType* TxIpduPtr);
#endif
/*called by Com_SendSignal.
 * the tx signal value change,calculate the new TMC and TMS*/
#if (0u < COM_TMCTXSIGNAL_NUMBER) && (STD_ON == COM_TMS_ENABLE)
static FUNC(uint64, COM_CODE) Com_GetOldSignalValue(Com_SignalType SignalType, uint16 SignalBufferId);
#endif
#if (0u < COM_TXSIGNAL_NUMBER) && (STD_ON == COM_TMS_ENABLE)
static FUNC(void, COM_CODE) Com_TxSignalTMHandle(
    Com_TxIPduRunTimeStateType* TxIpduStatePtr,
    const Com_TxIPduType* TxIpduPtr,
    Com_SignalIdType TxSignalId,
    P2CONST(void, AUTOMATIC, COM_APPL_CONST) TxSignalDataPtr);
#endif
/* called by Com_SendSignal,Com_SendTxGroupSignalHandle.
 * set Tx signal(not dynamic signal) to signalbuffer,return the buffer value is changed or not*/
#if ((COM_TXGROUPSIGNAL_NUMBER > 0u) || (0u < COM_TXSIGNAL_NUMBER))
static FUNC(boolean, COM_CODE) Com_SetTxSignalBuff(
    Com_SignalType SignalType,
    uint16 SignalLength,
    uint16 SignalBufferId,
    P2CONST(void, AUTOMATIC, COM_APPL_CONST) SignalNewDataPtr,
    uint8 BitSize);
#endif
/* called by Com_SendSignal.
 * pack the Tx signal into ipdu Tx buffer*/
#if (0u < COM_TXSIGNAL_NUMBER)
static FUNC(void, COM_CODE) Com_TxSignalPack(Com_SignalIdType TxSignalId);
#endif /*0u < COM_TXSIGNAL_NUMBER*/
/* called by Com_SendSignal,Com_SendDynSignal.
 * send Tx signal handle*/
#if (0u < COM_TXSIGNAL_NUMBER)
static FUNC(uint8, COM_CODE) Com_SendTxSignalHandle(
    PduIdType TxIpduId,
    Com_TransferPropertyType SignalTxProperty,
    boolean DMSignal,
    boolean ValueChanged);
#endif /*0u < COM_TXSIGNAL_NUMBER*/
/* called by Com_SendSignal,Com_InvalidateSignalGroup.
 * send Tx group signal handle*/
#if (COM_TXGROUPSIGNAL_NUMBER > 0u)
static FUNC(uint8, COM_CODE) Com_SendTxGroupSignalHandle(
    Com_SignalIdType TxGroupSignalId,
    P2CONST(void, AUTOMATIC, COM_APPL_CONST) SignalDataPtr);
#endif

/* called by Com_SendDynSignal.
 * set Tx dynamic signal to signalbuffer,return the buffer value is changed or not*/
#if (                                                                                        \
    ((0u < COM_TXSIGNALGROUP_NUMBER) && (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)) \
    || ((0u < COM_TXSIGNAL_NUMBER) && (STD_ON == COM_TX_SIGNAL_TYPE_UINT8_DYN_ENABLE)))
static FUNC(boolean, COM_CODE) Com_SetDynTxSignalBuff(
    PduIdType TxPduId,
    uint16 SignalBufferId,
    P2CONST(void, AUTOMATIC, COM_APPL_CONST) SignalNewDataPtr,
    uint16 Length);
/* called by Com_SendDynSignal,Com_TxSignalGroupPack.
 * dynamic signal pack to tx pdu buffer*/
static FUNC(void, COM_CODE)
    Com_TxDynSignalPack(PduIdType TxPduId, Com_SignalPositionType SigLsbBytePos, uint16 SignalBufferId, uint16 Length);
#endif
/* called by Com_SendSignalGroup.
 * pack the Tx signal group into ipdu Tx buffer*/
#if (COM_TXSIGNALGROUP_NUMBER > 0u)
static FUNC(void, COM_CODE) Com_TxSignalGroupPack(Com_SignalGroupIdType SignalGroupId);
#endif
/* called by Com_SendSignalGroupArray.
 * update every group signal buffer,calculate every TMC,return the signal group is changed or not for trigger transmit*/
#if ((STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && (COM_TXSIGNALGROUP_NUMBER > 0u))
static FUNC(boolean, COM_CODE) Com_UpdateTxBufferAndCalculateTMCOfEveryGroupSignal(
    Com_SignalGroupIdType TxSignalGroupId,
    Com_TxGroupSignalIdType GroupSignalNumber,
    Com_TxIpduBufIdType TxIPduBufferId);
#endif
/* called by Com_TriggerIPDUSend,Com_TriggerIPDUSendWithMetaData,Com_TriggerTransmit,
 * Com_TxConfirmation,Com_TpTxConfirmation,Com_MainFunctionTx,Com_TxIpduController.
 * clear all signal group/signal/dest description signal update bit of the Tx Pdu*/
#if (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE) || (STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE) \
    || (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
static FUNC(void, COM_CODE) Com_ClearTxIPduUpdates(const Com_TxIPduType* TxIpduPtr);
#endif
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
static void Com_GetTxModeOffset(
    Com_TxIPduRunTimeStateType* TxIpduStatePtr,
    const Com_TxIPduType* TxIpduPtr,
    boolean TMS)
{
    Com_TxModeTimeOffsetType txModeOffset = 1u;
    PduIdType txModeIndex;
    Com_TxModeModeType ipduTxMode = TxIpduStatePtr->ipduTxMode;
#if (STD_ON == COM_TMS_ENABLE)
    if (!TMS)
    {
        txModeIndex = TxIpduPtr->ComTxModeFalseRefId;
        if (COM_TX_MODE_MIXED == ipduTxMode)
        {
#if (0u < COM_TX_MODE_FALSE_MIXED_NUMBER)
            txModeOffset = Com_TxModeFalseMixedRepetitions[txModeIndex].ComTxModeTimeOffset;
#endif
        }
        else
        {
#if (0u < COM_TX_MODE_FALSE_PERIOD_NUMBER) || (0u < COM_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER)
            txModeOffset = Com_TxModeFalsePeriod[txModeIndex].ComTxModeTimeOffset;
#endif
        }
    }
    else
#endif
    {
        txModeIndex = TxIpduPtr->ComTxModeTrueRefId;
        if (COM_TX_MODE_MIXED == ipduTxMode)
        {
#if (0u < COM_TX_MODE_TRUE_MIXED_NUMBER)
            txModeOffset = Com_TxModeTrueMixedRepetitions[txModeIndex].ComTxModeTimeOffset;
#endif
        }
        else
        {
#if (0u < COM_TX_MODE_TRUE_PERIOD_NUMBER) || (0u < COM_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER)
            txModeOffset = Com_TxModeTruePeriod[txModeIndex].ComTxModeTimeOffset;
#endif
        }
    }
    TxIpduStatePtr->PeriodCnt = txModeOffset;
}

#if (COM_ONEEVERYNFILTERSIGNAL_NUMBER > 0u)
/******************************************************************************/
/*
 * Brief               reset OCCURRENCE of filter with ComFilterAlgorithm ONE_EVERY_N.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      IpduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE) Com_ResetTxOccurrenceOfPduFilter(PduIdType IpduId)
{
    const Com_TxIPduType* txIpduPtr = &Com_ConfigStd->ComTxIPdu[IpduId];
/*reset signal/group signal/dest description signal of tx pdu which ComFilterAlgorithm is ONE_EVERY_N*/
#if (COM_TXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)
    const Com_TxSignalType* txSignalPtr;
    Com_TxSignalIdType txSignalNumber, cnt;
    txSignalNumber = txIpduPtr->ComIPduSignalsRefNumber;
    cnt = txIpduPtr->ComIpduSignalRefStartId;
    for (; cnt < txSignalNumber; ++cnt)
    {
        txSignalPtr = &Com_ConfigStd->ComTxSignal[cnt];
        if (COM_UNUSED_TXSIGNALFILTERID != txSignalPtr->ComFilterIndex)
        {
            Com_OneEveryNcnt[txSignalPtr->ComFilterIndex] = 0u;
        }
    }
#endif
#if (COM_DESTSIG_FILTERTYPE_MAX_NUMBER > 0u)
    const Com_GwDestSignalType* ComDestSignalPtr;
    Com_GwDestinationDescriptionIdType ipduDestSignalNumber, counter;
    ipduDestSignalNumber = txIpduPtr->ComIPduDespSignalsRefNumber;
    counter = txIpduPtr->ComIPduDespSignalsRefStartId;
    for (; counter < ipduDestSignalNumber; ++counter)
    {
        ComDestSignalPtr = &Com_ConfigStd->ComDestSignal[counter];
        if (COM_UNUSED_DESTSIGNALFILTERID != ComDestSignalPtr->ComFilterIndex)
        {
            Com_OneEveryNcnt[ComDestSignalPtr->ComFilterIndex] = 0u;
        }
    }
#endif
#if (COM_TXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
    const Com_TxSignalGroupType* txSignalGroupPtr;
    const Com_TxGroupSignalType* txGroupSignalPtr;
    Com_TxSignalGroupIdType signalGroupNumber, signalGroupId;
    Com_TxGroupSignalIdType groupSignalNumber, index;
    signalGroupNumber = txIpduPtr->ComIPduSignalGroupsRefNumber;
    signalGroupId = txIpduPtr->ComIPduSignalGroupsRefStartId;
    for (; signalGroupId < signalGroupNumber; ++signalGroupId)
    {
        txSignalGroupPtr = &Com_ConfigStd->ComTxSignalGroup[signalGroupId];
        groupSignalNumber = txSignalGroupPtr->ComGroupSignalRefNumber;
        index = txSignalGroupPtr->ComGroupSignalRefStartId;
        for (; index < groupSignalNumber; ++index)
        {
            txGroupSignalPtr = &Com_ConfigStd->ComTxGroupSignal[index];
            if (COM_UNUSED_TXGRPSIGFILTERID != txGroupSignalPtr->ComFilterIndex)
            {
                Com_OneEveryNcnt[txGroupSignalPtr->ComFilterIndex] = 0u;
            }
        }
    }
#endif
    return;
}
#endif

#if (COM_IPDUGROUP_NUMBER > 0u)
/******************************************************************************/
/*
 * Brief               called by Com_TxIpduController.
 *                     init the tx ipdu buffer,all signal buffer
 *                     (included in the ipdu) and init the TMC.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      IpduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE) Com_ResetTxPduBufferAndSignalBuffer(PduIdType IpduId)
{
    const Com_TxIPduType* txIpduPtr = &Com_ConfigStd->ComTxIPdu[IpduId];
    Com_TxIPduRunTimeStateType* txIpduStatePtr = &Com_TxIPduRunTimeState[IpduId];
    const Com_TxIPduRunTimeStateType* txIpduInitStatePtr = &Com_TxIPduInitState[IpduId];
    const Com_TxSignalType* txSignalPtr;
    const Com_GwDestSignalType* gwDestSignalPtr;
    const Com_TxSignalGroupType* txSignalGroupPtr;
    const Com_TxGroupSignalType* txGroupSignalPtr;
    uint16 signalInitValueId;
    uint16 signalLength;
    Com_SignalType signalType;
    Com_SignalIdType txSignalTMCId;

/*init the tx ipdu buffer,all signal buffer(included in the ipdu) and init the TMC*/
#if (0u < COM_TXIPDUBUFF_SIZE)
    Com_TxIpduBufIdType pduBufferId = txIpduPtr->ComTxIPduBufIndex;
    uint16 initPduLength = Com_TxIPduInitState[IpduId].TxIpduLength;
    SchM_Enter_Com_Context();
    (void)ILib_memcpy(&Com_TxIPduRuntimeBuff[pduBufferId], &Com_TxIPduInitValue[pduBufferId], initPduLength);
    SchM_Exit_Com_Context();
#endif
#if (COM_TXSIGNAL_NUMBER > 0u)
    Com_TxSignalIdType txSignalNumber, cnt;
    /*init signal buffer and TMC*/
    txSignalNumber = txIpduPtr->ComIPduSignalsRefNumber;
    cnt = txIpduPtr->ComIpduSignalRefStartId;
    for (; cnt < txSignalNumber; ++cnt)
    {
        txSignalPtr = &Com_ConfigStd->ComTxSignal[cnt];
        signalInitValueId = txSignalPtr->ComSignalInitValueId;
        signalType = txSignalPtr->ComSignalType;
#if (STD_ON == COM_TX_SIGNAL_TYPE_UINT8_DYN_ENABLE) || (STD_ON == COM_TX_SIGNAL_TYPE_UINT8_N_ENABLE)
        if ((COM_UINT8_DYN != signalType) && (COM_UINT8_N != signalType))
#endif /* STD_ON == COM_TX_SIGNAL_TYPE_UINT8_DYN_ENABLE) || (STD_ON == COM_TX_SIGNAL_TYPE_UINT8_N_ENABLE */
        {
            signalLength = 0u;
        }
#if (STD_ON == COM_TX_SIGNAL_TYPE_UINT8_DYN_ENABLE) || (STD_ON == COM_TX_SIGNAL_TYPE_UINT8_N_ENABLE)
        else
        {
            signalLength = (COM_UINT8_DYN == signalType) ? (txSignalPtr->ComSignalDataInitValueLength)
                                                         : (txSignalPtr->ComSignalLength);
        }
#endif /* STD_ON == COM_TX_SIGNAL_TYPE_UINT8_DYN_ENABLE) || (STD_ON == COM_TX_SIGNAL_TYPE_UINT8_N_ENABLE */
        Com_InitSignalBuffer(signalType, signalInitValueId, signalLength);
#if (0u < COM_TXSIGNAL_FILTERTYPE_MAX_NUMBER)
        if (COM_UNUSED_TXSIGNALFILTERID != txSignalPtr->ComFilterIndex)
        {
            txSignalTMCId = txSignalPtr->ComTMCBufferId;
            Com_TxSignalTMCRunTime[txSignalTMCId] = Com_TxSignalInitTMC[txSignalTMCId];
        }
#endif
    }
#endif
#if (COM_GW_DESTINATION_DESCRIPTION_NUMBER > 0u)
    Com_GwDestinationDescriptionIdType destSignalNumber, destSignalId;
    /*init dest description signal buffer and TMC*/
    destSignalNumber = txIpduPtr->ComIPduDespSignalsRefNumber;
    destSignalId = txIpduPtr->ComIPduDespSignalsRefStartId;
    for (; destSignalId < destSignalNumber; ++destSignalId)
    {
        gwDestSignalPtr = &Com_ConfigStd->ComDestSignal[destSignalId];
        signalInitValueId = gwDestSignalPtr->ComSignalInitValueId;
        signalType = gwDestSignalPtr->ComSignalType;
        if ((COM_UINT8_DYN != signalType) && (COM_UINT8_N != signalType))
        {
            signalLength = 0u;
        }
        else
        {
            signalLength = (COM_UINT8_DYN == signalType) ? (gwDestSignalPtr->ComSignalDataInitValueLength)
                                                         : (gwDestSignalPtr->ComSignalLength);
        }
        Com_InitSignalBuffer(signalType, signalInitValueId, signalLength);
#if (COM_DESTSIG_FILTERTYPE_MAX_NUMBER > 0u)
        if (COM_UNUSED_DESTSIGNALFILTERID != gwDestSignalPtr->ComFilterIndex)
        {
            txSignalTMCId = gwDestSignalPtr->ComTMCBufferId;
            Com_TxSignalTMCRunTime[txSignalTMCId] = Com_TxSignalInitTMC[txSignalTMCId];
        }
#endif
    }
#endif /* COM_GWMAPPING_NUMBER > 0u */
#if (COM_TXSIGNALGROUP_NUMBER > 0u)
    Com_TxSignalGroupIdType signalGroupNumber, signalGroupId;
    Com_TxGroupSignalIdType groupSignalNumber, counter;
    /*init group signal buffer and TMC*/
    signalGroupNumber = txIpduPtr->ComIPduSignalGroupsRefNumber;
    signalGroupId = txIpduPtr->ComIPduSignalGroupsRefStartId;
    for (; signalGroupId < signalGroupNumber; ++signalGroupId)
    {
        txSignalGroupPtr = &Com_ConfigStd->ComTxSignalGroup[signalGroupId];
        groupSignalNumber = txSignalGroupPtr->ComGroupSignalRefNumber;
        counter = Com_ConfigStd->ComTxSignalGroup[signalGroupId].ComGroupSignalRefStartId;
        for (; counter < groupSignalNumber; ++counter)
        {
            txGroupSignalPtr = &Com_ConfigStd->ComTxGroupSignal[counter];
            signalInitValueId = txGroupSignalPtr->ComSignalInitValueId;
            signalType = txGroupSignalPtr->ComSignalType;
#if (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE) || (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE)
            if ((COM_UINT8_DYN != signalType) && (COM_UINT8_N != signalType))
#endif /* STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE) || (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE */
            {
                signalLength = 0u;
            }
#if (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE) || (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE)
            else
            {
                signalLength = (COM_UINT8_DYN == signalType) ? (txGroupSignalPtr->ComSignalDataInitValueLength)
                                                             : (txGroupSignalPtr->ComSignalLength);
            }
#endif /* STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE) || (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE */
            Com_InitSignalBuffer(signalType, signalInitValueId, signalLength);
#if ((0u < COM_TMCTXSIGNAL_NUMBER) && (0u < COM_TXGRPSIG_FILTERTYPE_MAX_NUMBER))
            if (COM_UNUSED_TXGRPSIGFILTERID != txGroupSignalPtr->ComFilterIndex)
            {
                txSignalTMCId = txGroupSignalPtr->ComTMCBufferId;
                Com_TxGroupSignalTMCBuffer[txSignalTMCId] = Com_TxSignalInitTMC[txSignalTMCId];
                Com_TxSignalTMCRunTime[txSignalTMCId] = Com_TxSignalInitTMC[txSignalTMCId];
            }
#endif
        }
    }
#endif
    txIpduStatePtr->TxIpduRTStFlag |= (txIpduInitStatePtr->TxIpduRTStFlag & Com_TX_TMS_EN);
    txIpduStatePtr->ipduTxMode = txIpduInitStatePtr->ipduTxMode;
    txIpduStatePtr->TxIpduLength = txIpduInitStatePtr->TxIpduLength;
    txIpduStatePtr->TxIpduRTStFlag &= Com_TX_CONFIRM_DIS;
    txIpduStatePtr->TxIpduRTStFlag &= Com_TX_TRANSMITING_DIS;
    txIpduStatePtr->TxIpduRTStFlag &= Com_TX_DELAY_DIS;
    txIpduStatePtr->NTimeCnt = 0u;
    txIpduStatePtr->RptNum = 0u;
    return;
}
#endif

#if (COM_TXSIGNALGROUP_NUMBER > 0u)
/******************************************************************************/
/*
 * Brief               pack tx group signal(not dynamic).
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      GroupSignalId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE) Com_TxGroupSignalPack(const Com_TxGroupSignalType* TxGroupSignalPtr)
{
    Com_TxSignalGroupIdType signalGroupRef = TxGroupSignalPtr->ComSignalGroupRef;
    PduIdType txIpduId = Com_ConfigStd->ComTxSignalGroup[signalGroupRef].ComIpduRefIndex;
    Com_SignalType signalType = TxGroupSignalPtr->ComSignalType;
    Com_TxIpduBufIdType ipduBufferIndex = Com_ConfigStd->ComTxIPdu[txIpduId].ComTxIPduBufIndex;
    uint16 signalBufferId = TxGroupSignalPtr->ComSignalInitValueId;
    uint64 txData;
#if (                                                                                          \
    (0u < COM_TXIPDUBUFF_SIZE)                                                                 \
    && (((0u < COM_SIGNAL_8BITBUFF_SIZE) && (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE)) \
        || (0u < COM_SIGNAL_BOOLBUFF_SIZE)))
    Com_TxIpduBufIdType ipduBuffPos = ipduBufferIndex + (Com_TxIpduBufIdType)(TxGroupSignalPtr->ComSigLsbBytePos);
    uint8* iPduBufferPtr = &Com_TxIPduRuntimeBuff[ipduBuffPos];
#endif

    if (COM_UINT8_N == signalType)
    {
#if ((0u < COM_SIGNAL_8BITBUFF_SIZE) && (0u < COM_TXIPDUBUFF_SIZE) && (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE))
        /*pack COM_UINT8_N type group signal*/
        uint16 signalLength = TxGroupSignalPtr->ComSignalLength;
        SchM_Enter_Com_Context();
        (void)ILib_memcpy(iPduBufferPtr, &Com_Signal8BitRuntimeBuff[signalBufferId], signalLength);
        SchM_Exit_Com_Context();
#endif /* 0u < COM_SIGNAL_8BITBUFF_SIZE && 0u < COM_TXIPDUBUFF_SIZE && STD_ON == \
          COM_TX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE */
    }
    /*pack group signal expect signal type(boolean,COM_UINT8_N,COM_UINT8_DYN)*/
    else if (COM_BOOLEAN == signalType)
    {
#if ((0u < COM_SIGNAL_BOOLBUFF_SIZE) && (0u < COM_TXIPDUBUFF_SIZE))
        uint8 value = (uint8)(1u << TxGroupSignalPtr->ComSignalPosInByte);
        SchM_Enter_Com_Context();
        if (Com_SignalBoolRuntimeBuff[signalBufferId])
        {
            *iPduBufferPtr |= value;
        }
        else
        {
            *iPduBufferPtr &= ~value;
        }
        SchM_Exit_Com_Context();
#endif /* 0u < COM_SIGNAL_BOOLBUFF_SIZE && 0u < COM_TXIPDUBUFF_SIZE */
    }
    else
    {
        /*get the group signal value need to be packed*/
        switch (signalType)
        {
#if (0u < COM_SIGNAL_8BITBUFF_SIZE)
        case COM_UINT8:
        case COM_SINT8:
            txData = (uint64)(Com_Signal8BitRuntimeBuff[signalBufferId]);
            break;
#endif
#if (0u < COM_SIGNAL_16BITBUFF_SIZE)
        case COM_UINT16:
        case COM_SINT16:
            txData = (uint64)(Com_Signal16BitRuntimeBuff[signalBufferId]);
            break;
#endif
#if (0u < COM_SIGNAL_32BITBUFF_SIZE)
        case COM_SINT32:
        case COM_UINT32:
        case COM_FLOAT32:
            txData = (uint64)(Com_Signal32BitRuntimeBuff[signalBufferId]);
            break;
#endif
#if (0u < COM_SIGNAL_64BITBUFF_SIZE)
        case COM_SINT64:
        case COM_UINT64:
        case COM_FLOAT64:
            txData = Com_Signal64BitRuntimeBuff[signalBufferId];
            break;
#endif
        default:
            /*reach here,the signal type is error*/
            txData = 0u;
            break;
        }
        Com_TxSignalPackHandle(COM_TX_GROUP_SIGNAL, TxGroupSignalPtr, txData, &Com_TxIPduRuntimeBuff[ipduBufferIndex]);
    }
}
#endif

#if (0u < COM_TMCTXSIGNAL_NUMBER) && (STD_ON == COM_TMS_ENABLE)
/******************************************************************************/
/*
 * Brief               called by Com_TxSignalTMHandle,Com_SendTxGroupSignalHandle,
 *                     Com_UpdateTxBufferAndCalculateTMCOfEveryGroupSignal.
 *                     the group signal value changed,judge it will trigger
 *                     calculate the new TMC of the signal new value.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalValuePtr,SignalType,SignalBufferId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/******************************************************************************/
static FUNC(boolean, COM_CODE) Com_TxSignalTMCCalculate(
    Com_SignalConfigType DestSignalType,
    P2CONST(void, AUTOMATIC, COM_APPL_CONST) SignalValuePtr,
    P2CONST(void, AUTOMATIC, COM_APPL_CONST) TxSignalTMCPtr,
    uint64 SignalOldValue)
{
    boolean ret = FALSE;
#if (COM_TXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u) || (COM_TXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
#if (COM_TXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)
    const Com_TxSignalType* txSignalPtr;
#endif
#if (COM_TXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
    const Com_TxGroupSignalType* txGroupSignalPtr;
#endif
    uint64 signalNewValue;
    Com_SignalType signalType;
    Com_FilterAlgorithmType filterType;
    Com_SignalIdType filterIndex;
    uint8 bitsize;
    boolean isTrue = FALSE;

    switch (DestSignalType)
    {
#if (COM_TXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)
    case COM_TX_SIGNAL:
        txSignalPtr = (const Com_TxSignalType*)TxSignalTMCPtr;
        signalType = txSignalPtr->ComSignalType;
        filterType = txSignalPtr->ComFilterAlgorithm;
        filterIndex = (Com_SignalIdType)(txSignalPtr->ComFilterIndex);
        bitsize = txSignalPtr->ComBitSize;
        isTrue = TRUE;
        break;
#endif
#if (COM_TXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
    case COM_TX_GROUP_SIGNAL:
        txGroupSignalPtr = (const Com_TxGroupSignalType*)TxSignalTMCPtr;
        signalType = txGroupSignalPtr->ComSignalType;
        filterType = txGroupSignalPtr->ComFilterAlgorithm;
        filterIndex = (Com_SignalIdType)(txGroupSignalPtr->ComFilterIndex);
        bitsize = txGroupSignalPtr->ComBitSize;
        isTrue = TRUE;
        break;
#endif
    default:
        /* do nothing */
        break;
    }

    /*COM_UINT8_N just support ALWAYS and NEVER FilterAlgorithm*/
    if (isTrue)
    {
#if (STD_ON == COM_TX_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE)
        if (COM_UINT8_N == signalType)
        {
            if (COM_ALWAYS == filterType)
            {
                ret = TRUE;
            }
        }
        else
#endif /* STD_ON == COM_TX_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE */
        {
            /*tx signal changed to uint64 type*/
            signalNewValue = Com_GetTxSignalValue(signalType, SignalValuePtr, bitsize);
            ret = Com_DestSignalTMCCalculate(
                signalType,
                filterType,
                filterIndex,
#if (COM_TXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
                SignalOldValue,
#endif
                signalNewValue);
        }
    }
#endif
    COM_NOUSED(DestSignalType);
    COM_NOUSED(SignalValuePtr);
    COM_NOUSED(TxSignalTMCPtr);
    COM_NOUSED(SignalOldValue);
    return ret;
}
#endif

#if (0u < COM_TMCTXSIGNAL_NUMBER) && (STD_ON == COM_TMS_ENABLE)
/******************************************************************************/
/*
 * Brief               change the signal value to uint64 type.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalType,SignalValuePtr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint64
 */
/******************************************************************************/
static FUNC(uint64, COM_CODE) Com_GetTxSignalValue(
    Com_SignalType SignalType,
    P2CONST(void, AUTOMATIC, COM_APPL_CONST) SignalValuePtr,
    uint8 BitSize)
{
    uint64 signalNewValue = 0u;
    /* PRQA S 4391++ */ /* MISRA Rule 10.8 */
    switch (SignalType)
    {
    case COM_BOOLEAN:
        /* PRQA S 4304 ++ */ /* MISRA Rule 10.5 */
        signalNewValue = (uint64)(*((const boolean*)SignalValuePtr));
        /* PRQA S 4304 -- */ /* MISRA Rule 10.5 */
        break;
    case COM_UINT8:
    case COM_SINT8:
        signalNewValue = (uint64)((*((const uint8*)SignalValuePtr)) & (COM_UNUSED_UINT8 >> (8u - BitSize)));
        break;
    case COM_SINT16:
    case COM_UINT16:
        signalNewValue = (uint64)((*((const uint16*)SignalValuePtr)) & (COM_UNUSED_UINT16 >> (16u - BitSize)));
        break;
    case COM_SINT32:
    case COM_UINT32:
        signalNewValue = (uint64)((*((const uint32*)SignalValuePtr)) & (COM_UNUSED_UINT32 >> (32u - BitSize)));
        break;
    case COM_SINT64:
    case COM_UINT64:
        signalNewValue = (*((const uint64*)SignalValuePtr));
        break;
    default:
        /*do nothing*/
        break;
    }
    /* PRQA S 4391 -- */ /* MISRA Rule 10.8 */
    return signalNewValue;
}
#endif

#if ((STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && (COM_TXSIGNALGROUP_NUMBER > 0u))
#if ((STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) && (0u < COM_TXIPDUBUFF_SIZE) && (0u < COM_SIGNAL_8BITBUFF_SIZE))
/******************************************************************************/
/*
 * Brief               update the uint8_n signal value.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxIPduBufferId,groupSignalId,groupSignalInitValueId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/******************************************************************************/
static FUNC(boolean, COM_CODE) Com_TxGroupSignalUpdateUint8N(
    Com_TxIpduBufIdType TxIPduBufferId,
    Com_SignalIdType GroupSignalId,
    uint16 GroupSignalInitValueId)
{
    const Com_TxGroupSignalType* txGroupSignalPtr = &Com_ConfigStd->ComTxGroupSignal[GroupSignalId];
    boolean triggerOnChange = FALSE;
    uint16 groupSignalLength = txGroupSignalPtr->ComSignalLength;
    Com_TxIpduBufIdType sigBufPosBase = TxIPduBufferId + (Com_TxIpduBufIdType)(txGroupSignalPtr->ComSigLsbBytePos);

    /*if the group signal changed,then update the signal buffer*/
    if (0
        != ILib_memcmp(
            &Com_Signal8BitRuntimeBuff[GroupSignalInitValueId],
            &Com_TxIPduRuntimeBuff[sigBufPosBase],
            groupSignalLength))
    {
        SchM_Enter_Com_Context();
        (void)ILib_memcpy(
            &Com_Signal8BitRuntimeBuff[GroupSignalInitValueId],
            &Com_TxIPduRuntimeBuff[sigBufPosBase],
            groupSignalLength);
        SchM_Exit_Com_Context();
        triggerOnChange = TRUE;
    }
    return triggerOnChange;
}
#endif
#endif

#if ((0u < COM_TXSIGNALGROUP_NUMBER) && (STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API))
/******************************************************************************/
/*
 * Brief               Send Signal Group(Array) handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxIPduBufferId,groupSignalId,groupSignalInitValueId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/******************************************************************************/
static FUNC(void, COM_CODE) Com_SendSignalGroupArrayInternalHandle(
    const Com_TxSignalGroupType* TxSignalGroupPtr,
    Com_TxIPduRunTimeStateType* TxIpduStatePtr,
    PduIdType IpduRef,
    /* PRQA S 3206 ++ */ /* MISRA Rule 2.7 */
    boolean TriggerOnChange,
    /* PRQA S 3206 -- */ /* MISRA Rule 2.7 */
    uint16 RptNum)
{
#if (STD_ON == COM_TX_SIG_GROUP_TIMEOUT_ENABLE)
    const Com_TxIPduType* txIpduPtr = &Com_ConfigStd->ComTxIPdu[IpduRef];
#endif
    Com_TxModeModeType ipduTxMode = TxIpduStatePtr->ipduTxMode;
    boolean initDMCnt = FALSE;
/*the signal group configuration timeout,the DM timer isn't start by other signals/signal groups,Tx Mode isn't
 * NONE*/
#if (STD_ON == COM_TX_SIG_GROUP_TIMEOUT_ENABLE)
    if ((COM_UNUSED_TXSIGGRPTIMEOUTID != TxSignalGroupPtr->ComTimeoutIndex) && (COM_TX_MODE_NONE != ipduTxMode)
        && (0u == TxIpduStatePtr->DMCnt))
    {
        /*restart the tx DM timer*/
        TxIpduStatePtr->DMCnt = txIpduPtr->ComTxIpduDM;
        initDMCnt = TRUE;
    }
#endif
#if (0u < COM_TX_MODE_TRUE_DIRECT_NUMBER) || (0u < COM_TX_MODE_TRUE_MIXED_NUMBER)                              \
    || (0u < COM_TX_MODE_FALSE_DIRECT_NUMBER) || (0u < COM_TX_MODE_FALSE_MIXED_NUMBER)                         \
    || (0u < COM_TX_MODE_TRUE_DIRECT_NOREPETITION_NUMBER) || (0u < COM_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER) \
    || (0u < COM_TX_MODE_FALSE_DIRECT_NOREPETITION_NUMBER) || (0u < COM_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER)
    if ((COM_TX_MODE_DIRECT == ipduTxMode) || (COM_TX_MODE_MIXED == ipduTxMode)
        || (COM_TX_MODE_DIRECT_WITHOUT_REPETITION == ipduTxMode)
        || (COM_TX_MODE_MIXED_WITHOUT_REPETITION == ipduTxMode))
    {
        /* PRQA S 3315 ++ */ /* MISRA Rule 16.6 */
        switch (TxSignalGroupPtr->ComTransferProperty)
        {
#if (STD_ON == COM_TX_SIG_GROUP_PROP_TRIGGERED_ENABLE)
        case COM_TRIGGERED:
            TxIpduStatePtr->NTimeCnt = 0u;
            TxIpduStatePtr->RptNum = RptNum;
            /*For an I-PDU with ComTxModeMode DIRECT and ComTxModeNumberOfRepetitions > 0, the AUTOSAR COM module
              shall reset an already running transmission deadline monitoring timer in case another send request for
              this I-PDU is initiated*/
#if (STD_ON == COM_TX_SIG_GROUP_TIMEOUT_ENABLE)
            if ((1u < RptNum) && (!initDMCnt) && (COM_UNUSED_TXSIGGRPTIMEOUTID != TxSignalGroupPtr->ComTimeoutIndex))
            {
                TxIpduStatePtr->DMCnt = txIpduPtr->ComTxIpduDM;
            }
#endif
            break;
#endif /* STD_ON == COM_TX_SIG_GROUP_PROP_TRIGGERED_ENABLE */
#if (STD_ON == COM_TX_SIG_GROUP_PROP_TRIGGERED_ON_CHANGE_ENABLE)
        case COM_TRIGGERED_ON_CHANGE:
            if (TriggerOnChange)
            {
                TxIpduStatePtr->NTimeCnt = 0u;
                TxIpduStatePtr->RptNum = RptNum;
                /*For an I-PDU with ComTxModeMode DIRECT and ComTxModeNumberOfRepetitions > 0, the AUTOSAR COM
                  module shall reset an already running transmission deadline monitoring timer in case another send
                  request for this I-PDU is initiated*/
#if (STD_ON == COM_TX_SIG_GROUP_TIMEOUT_ENABLE)
                if ((1u < RptNum) && (!initDMCnt)
                    && (COM_UNUSED_TXSIGGRPTIMEOUTID != TxSignalGroupPtr->ComTimeoutIndex))
                {
                    TxIpduStatePtr->DMCnt = txIpduPtr->ComTxIpduDM;
                }
#endif
            }
            break;
#endif /* STD_ON == COM_TX_SIG_GROUP_PROP_TRIGGERED_ON_CHANGE_ENABLE */
#if (STD_ON == COM_TX_SIG_GROUP_PROP_TRIG_ON_CH_NO_REPETITION_ENABLE)
        case COM_TRIGGERED_ON_CHANGE_WITHOUT_REPETITION:
            if (TriggerOnChange)
            {
                TxIpduStatePtr->NTimeCnt = 0u;
                TxIpduStatePtr->RptNum = 1u;
            }
            break;
#endif /* STD_ON == COM_TX_SIG_GROUP_PROP_TRIG_ON_CH_NO_REPETITION_ENABLE */
#if (STD_ON == COM_TX_SIG_GROUP_PROP_TRIG_NO_REPETITION_ENABLE)
        case COM_TRIGGERED_WITHOUT_REPETITION:
            TxIpduStatePtr->NTimeCnt = 0u;
            TxIpduStatePtr->RptNum = 1u;
            break;
#endif /* STD_ON == COM_TX_SIG_GROUP_PROP_TRIG_NO_REPETITION_ENABLE */
        default:
            /*do nothing*/
            break;
        }
        /* PRQA S 3315 -- */ /* MISRA Rule 16.6 */
    }
    else
#endif
    {
        TxIpduStatePtr->NTimeCnt = 0u;
        TxIpduStatePtr->RptNum = 0u;
    }
    COM_NOUSED(RptNum);
    COM_NOUSED(TriggerOnChange);
    COM_NOUSED(initDMCnt);
    COM_NOUSED(IpduRef);
    return;
}
#endif

#if (0u < COM_TXIPDU_NUMBER)
/******************************************************************************/
/*
 * Brief               Trigger IPdu Send handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      txIpduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 */
/******************************************************************************/
static FUNC(Std_ReturnType, COM_CODE) Com_TriggerIPDUSendInternalHandle(PduIdType TxIpduId)
{
    const Com_TxIPduType* txIpduPtr = &Com_ConfigStd->ComTxIPdu[TxIpduId];
    Com_TxIPduRunTimeStateType* txIpduStatePtr = &Com_TxIPduRunTimeState[TxIpduId];
    Com_TxIpduBufIdType ipduBufferId = txIpduPtr->ComTxIPduBufIndex;
    uint16 ipduLength = txIpduStatePtr->TxIpduLength;
    PduInfoType pduInfo;
    Std_ReturnType returnValue = E_NOT_OK;

#if (STD_ON == COM_TX_IPDU_COUNTER_ENABLE)
    uint8 txPduCounter = 0u;
    if (COM_UNUSED_TXIPDUCOUNTERID != txIpduPtr->ComIPduCounterIndex)
    {
        /*Pack the counter value in the pdu buffer*/
        Com_PackCounterValue(txIpduStatePtr, txIpduPtr);
        /*recover the active counter value if the transmit is E_NOT_OK*/
        txPduCounter = txIpduStatePtr->TxIpduCounter;
        /*Increase the counter value by +1*/
        Com_PduCounterIncrease(TRUE, TxIpduId);
    }
#endif /* STD_ON == COM_TX_IPDU_COUNTER_ENABLE */
    pduInfo.SduDataPtr = &Com_TxIPduRuntimeBuff[ipduBufferId];
    pduInfo.SduLength = (PduLengthType)ipduLength;
#if (STD_ON == COM_METADATA_SUPPORT)
    uint8 metaDataLength = txIpduPtr->MetaDataLength;
    if (0u < metaDataLength)
    {
        PduLengthType metaDataStartId =
            txIpduPtr->IPduNoDynSignalLength + txIpduPtr->IPduMaxDynSignalLength + (PduLengthType)ipduBufferId;
        pduInfo.MetaDataPtr = &Com_TxIPduRuntimeBuff[metaDataStartId];
    }
    else
#endif
    {
        pduInfo.MetaDataPtr = NULL_PTR;
    }
#if (STD_ON == COM_TX_IPDU_CALLOUT_ENABLE)
    boolean ret = TRUE;
    /*invoke the callout API*/
    if (NULL_PTR != txIpduPtr->ComIPduCallout)
    {
        ret = txIpduPtr->ComIPduCallout((TxIpduId + (COM_RXIPDU_NUMBER)), &pduInfo);
    }
    if (ret)
#endif /* STD_ON == COM_TX_IPDU_CALLOUT_ENABLE */
    {
#if (STD_ON == COM_TXTPPDU_SUPPORT)
        if (COM_PDU_TP == txIpduPtr->ComIPduType)
        {
            pduInfo.SduDataPtr = NULL_PTR;
        }
#endif /* STD_ON == COM_TXTPPDU_SUPPORT */
        txIpduStatePtr->TxIpduRTStFlag |= Com_TX_TRANSMITING_EN;
        txIpduStatePtr->TxOffset = 0u;
        if (E_OK == PduR_ComTransmit(txIpduPtr->ComPduIdRef, &pduInfo))
        {
/*Reset Tx_IPduMDT*/
#if (STD_ON == COM_MDT_ENABLE)
            Com_ResetTxIpduMDT(txIpduStatePtr, txIpduPtr);
#endif
#if (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE) || (STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE) \
    || (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
            if (COM_TRANSMIT == txIpduPtr->ComTxIPduClearUpdateBit)
            {
                /*clear all signal group/signal/dest description signal update bit of the Tx Pdu*/
                Com_ClearTxIPduUpdates(txIpduPtr);
            }
#endif /* STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE || STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE) || (STD_ON == \
COM_GW_DEST_SIG_UPDATE_BIT_ENABLE */
            returnValue = E_OK;
        }
        else
        {
            txIpduStatePtr->TxIpduRTStFlag &= Com_TX_TRANSMITING_DIS;
#if (STD_ON == COM_TX_IPDU_COUNTER_ENABLE)
            if (COM_UNUSED_TXIPDUCOUNTERID != txIpduPtr->ComIPduCounterIndex)
            {
                /*transmit not OK,recover the old counter value*/
                txIpduStatePtr->TxIpduCounter = txPduCounter;
            }
#endif /* STD_ON == COM_TX_IPDU_COUNTER_ENABLE */
/*ComRetryFailedTransmitRequests*/
#if (STD_ON == COM_RETRY_FAILED_TRANSMIT_REQUESTS)
            txIpduStatePtr->TxIpduRTStFlag |= Com_TX_DELAY_EN;
            returnValue = E_OK;
#else
/*Reset Tx_IPduMDT*/
#if (STD_ON == COM_MDT_ENABLE)
            Com_ResetTxIpduMDT(txIpduStatePtr, txIpduPtr);
#endif
#endif /* STD_ON == COM_RETRY_FAILED_TRANSMIT_REQUESTS */
        }
    }
#if (STD_ON == COM_TX_IPDU_CALLOUT_ENABLE) && (STD_ON == COM_TX_IPDU_COUNTER_ENABLE)
    else
    {
        if (COM_UNUSED_TXIPDUCOUNTERID != txIpduPtr->ComIPduCounterIndex)
        {
            /*transmit not OK,recover the old counter value*/
            txIpduStatePtr->TxIpduCounter = txPduCounter;
        }
    }
#endif /* STD_ON == COM_TX_IPDU_CALLOUT_ENABLE && STD_ON == COM_TX_IPDU_COUNTER_ENABLE */
    return returnValue;
}
#endif

#if (STD_ON == COM_MDT_ENABLE)
/******************************************************************************/
/*
 * Brief               Reset the counter of tx Ipdu MDT.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxIpduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
/* PRQA S 3673 ++ */ /* MISRA Rule 8.13 */
static FUNC(void, COM_CODE)
    Com_ResetTxIpduMDT(Com_TxIPduRunTimeStateType* TxIpduStatePtr, const Com_TxIPduType* TxIpduPtr)
/* PRQA S 3673 -- */ /* MISRA Rule 8.13 */
{
#if (STD_ON == COM_ENABLE_MDT_FOR_CYCLIC_TRANSMISSION)
    TxIpduStatePtr->MDTCnt = TxIpduPtr->ComMinimumDelayTime;
#elif (0u < COM_TX_MODE_TRUE_DIRECT_NUMBER) || (0u < COM_TX_MODE_FALSE_DIRECT_NUMBER) \
    || (0u < COM_TX_MODE_TRUE_DIRECT_NOREPETITION_NUMBER) || (0u < COM_TX_MODE_FALSE_DIRECT_NOREPETITION_NUMBER)
    if ((COM_TX_MODE_DIRECT == ipduTxMode) || (COM_TX_MODE_DIRECT_WITHOUT_REPETITION == ipduTxMode))
    {
        if (TxIpduStatePtr->RptNum <= 1u)
        {
            TxIpduStatePtr->MDTCnt = TxIpduPtr->ComMinimumDelayTime;
        }
    }
#endif
    COM_NOUSED(TxIpduPtr);
    COM_NOUSED(TxIpduStatePtr);
}
#endif

#if (STD_ON == COM_TX_SIGNAL_TIMEOUT_NOTIFICATION_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_TIMEOUT_NOTIFICATION_ENABLE)
/******************************************************************************/
/*
 * Brief               Called by Com_MainFunctionTx.
 *                     tx pdu DM timeout notification.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxIpduPtr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE) Com_TxDMTimeOutNotification(const Com_TxIPduType* TxIpduPtr)
{
#if (STD_ON == COM_TX_SIGNAL_TIMEOUT_NOTIFICATION_ENABLE)
    Com_TxSignalIdType txSignalNumber, cnt;
    const Com_TxSignalType* txSignalPtr;
    txSignalNumber = TxIpduPtr->ComIPduSignalsRefNumber;
    cnt = TxIpduPtr->ComIpduSignalRefStartId;
    Com_TxSignalTimeoutIdType txSigTimeoutIndex;
    for (; cnt < txSignalNumber; ++cnt)
    {
        txSignalPtr = &Com_ConfigStd->ComTxSignal[cnt];
        txSigTimeoutIndex = txSignalPtr->ComTimeoutIndex;
        if ((txSigTimeoutIndex != COM_UNUSED_TXSIGNALTIMEOUTID)
            && (NULL_PTR != Com_TxSignalTimeoutNotificationCfg[txSigTimeoutIndex]))
        {
            Com_TxSignalTimeoutNotificationCfg[txSigTimeoutIndex]();
        }
    }
#endif /* STD_ON == COM_TX_SIGNAL_TIMEOUT_NOTIFICATION_ENABLE */
#if (STD_ON == COM_TX_SIG_GROUP_TIMEOUT_NOTIFICATION_ENABLE)
    Com_TxSignalGroupIdType signalGroupNumber, signalGroupId;
    signalGroupNumber = TxIpduPtr->ComIPduSignalGroupsRefNumber;
    signalGroupId = TxIpduPtr->ComIPduSignalGroupsRefStartId;
    const Com_TxSignalGroupType* txSignalGroupPtr;
    Com_TxSigGrpTimeoutIdType timeoutIndex;
    for (; signalGroupId < signalGroupNumber; ++signalGroupId)
    {
        txSignalGroupPtr = &Com_ConfigStd->ComTxSignalGroup[signalGroupId];
        timeoutIndex = txSignalGroupPtr->ComTimeoutIndex;
        if ((timeoutIndex != COM_UNUSED_TXSIGGRPTIMEOUTID)
            && (NULL_PTR != Com_TxSigGrpTimeoutNotificationCfg[timeoutIndex]))
        {
            Com_TxSigGrpTimeoutNotificationCfg[timeoutIndex]();
        }
    }
#endif /* STD_ON == COM_TX_SIG_GROUP_TIMEOUT_NOTIFICATION_ENABLE */
    return;
}
#endif

#if (0u < COM_TX_MODE_TRUE_DIRECT_NUMBER) || (0u < COM_TX_MODE_FALSE_DIRECT_NUMBER)                              \
    || (0u < COM_TX_MODE_TRUE_DIRECT_NOREPETITION_NUMBER) || (0u < COM_TX_MODE_FALSE_DIRECT_NOREPETITION_NUMBER) \
    || ((0u < COM_TX_MODE_TRUE_MIXED_NUMBER) || (0u < COM_TX_MODE_FALSE_MIXED_NUMBER)                            \
        || (0u < COM_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER) || (0u < COM_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER))
/******************************************************************************/
/*
 * Brief               Called by Com_MainFunctionTx.
 *                     the handle of Com_MainFunctionTx,when the tx mode is direct.
 *                     return the boolean value for the tx pdu need to transmit or not.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxIpduStatePtr,TxIpduPtr,MDTRun
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(boolean, COM_CODE)
    Com_MainFuncTxDirect(Com_TxIPduRunTimeStateType* TxIpduStatePtr, const Com_TxIPduType* TxIpduPtr, boolean MDTRun)
{
    boolean ret = FALSE;
    /* PRQA S 2983,2995 ++ */ /* MISRA Rule 2.2*/
    PduIdType txModeIndex;
    Com_TxModeModeType txMode;
    uint8 TxDelay = TxIpduStatePtr->TxIpduRTStFlag & Com_TX_DELAY_EN;

#if (0u < COM_TX_MODE_TRUE_DIRECT_NUMBER) || (0u < COM_TX_MODE_TRUE_MIXED_NUMBER) \
    || (0u < COM_TX_MODE_FALSE_DIRECT_NUMBER) || (0u < COM_TX_MODE_FALSE_MIXED_NUMBER)
    Com_TxModeNumberOfRepetitionsType numOfRepetition;
    if ((!MDTRun) && (Com_TX_DELAY_EN == TxDelay))
    {
        TxIpduStatePtr->NTimeCnt = 0u;
    }
    if (TxIpduStatePtr->NTimeCnt > 0u)
    {
        (TxIpduStatePtr->NTimeCnt) -= 1u;
    }
#endif
    uint8 Transmiting = TxIpduStatePtr->TxIpduRTStFlag & Com_TX_TRANSMITING_EN;
    if ((0u == TxIpduStatePtr->NTimeCnt) && (TxIpduStatePtr->RptNum > 0u) && (Com_TX_TRANSMITING_EN != Transmiting))
    {
/*delay transmit of the MDT */
#if (STD_ON == COM_MDT_ENABLE)
        if (MDTRun)
        {
            TxIpduStatePtr->TxIpduRTStFlag |= Com_TX_DELAY_EN;
        }
        else
#endif /* STD_ON == COM_MDT_ENABLE */
        {
            ret = TRUE;
        }

#if (STD_ON == COM_TMS_ENABLE)
        uint8 TMS = TxIpduStatePtr->TxIpduRTStFlag & Com_TX_TMS_EN;
        if (TMS != Com_TX_TMS_EN)
        {
            txModeIndex = TxIpduPtr->ComTxModeFalseRefId;
            txMode = TxIpduPtr->ComTxFalseModeMode;
            switch (txMode)
            {
            case COM_TX_MODE_DIRECT:
#if (0u < COM_TX_MODE_FALSE_DIRECT_NUMBER)
                numOfRepetition = Com_TxModeFalseDirectRepetitions[txModeIndex].ComTxModeNumberOfRepetitions;
                if (numOfRepetition <= 1u)
#endif
                {
                    TxIpduStatePtr->RptNum = 0u;
                }
#if (0u < COM_TX_MODE_FALSE_DIRECT_NUMBER)
                else
                {
                    TxIpduStatePtr->NTimeCnt = Com_TxModeFalseDirectRepetitions[txModeIndex].ComTxModeRepetitionPeriod;
                }
#endif
                break;
            case COM_TX_MODE_MIXED:
#if (0u < COM_TX_MODE_FALSE_MIXED_NUMBER)
                numOfRepetition = Com_TxModeFalseMixedRepetitions[txModeIndex].ComTxModeNumberOfRepetitions;
                if (numOfRepetition <= 1u)
#endif
                {
                    TxIpduStatePtr->RptNum = 0u;
                }
#if (0u < COM_TX_MODE_FALSE_MIXED_NUMBER)
                else
                {
                    TxIpduStatePtr->NTimeCnt = Com_TxModeFalseMixedRepetitions[txModeIndex].ComTxModeRepetitionPeriod;
                }
#endif
                break;
            default:
                /* do nothing */
                break;
            }
        }
        else
#endif
        {
            txModeIndex = TxIpduPtr->ComTxModeTrueRefId;
            txMode = TxIpduPtr->ComTxTrueModeMode;
            switch (txMode)
            {
            case COM_TX_MODE_DIRECT:
#if (0u < COM_TX_MODE_TRUE_DIRECT_NUMBER)
                numOfRepetition = Com_TxModeTrueDirectRepetitions[txModeIndex].ComTxModeNumberOfRepetitions;
                if (numOfRepetition <= 1u)
#endif
                {
                    TxIpduStatePtr->RptNum = 0u;
                }
#if (0u < COM_TX_MODE_TRUE_DIRECT_NUMBER)
                else
                {
                    TxIpduStatePtr->NTimeCnt = Com_TxModeTrueDirectRepetitions[txModeIndex].ComTxModeRepetitionPeriod;
                }
#endif
                break;
            case COM_TX_MODE_MIXED:
#if (0u < COM_TX_MODE_TRUE_MIXED_NUMBER)
                numOfRepetition = Com_TxModeTrueMixedRepetitions[txModeIndex].ComTxModeNumberOfRepetitions;
                if (numOfRepetition <= 1u)
#endif
                {
                    TxIpduStatePtr->RptNum = 0u;
                }
#if (0u < COM_TX_MODE_TRUE_MIXED_NUMBER)
                else
                {
                    TxIpduStatePtr->NTimeCnt = Com_TxModeTrueMixedRepetitions[txModeIndex].ComTxModeRepetitionPeriod;
                }
#endif
                break;
            default:
                /* do nothing */
                break;
            }
        }
    }
    if ((!MDTRun) && (Com_TX_DELAY_EN == TxDelay))
    {
        ret = TRUE;
    }
    /* PRQA S 2983,2995 -- */ /* MISRA Rule 2.2*/
    return ret;
}
#endif

#if (0u < COM_TX_MODE_TRUE_PERIOD_NUMBER) || (0u < COM_TX_MODE_FALSE_PERIOD_NUMBER)  \
    || (0u < COM_TX_MODE_TRUE_MIXED_NUMBER) || (0u < COM_TX_MODE_FALSE_MIXED_NUMBER) \
    || (0u < COM_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER) || (0u < COM_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER)
/******************************************************************************/
/*
 * Brief               Called by Com_MainFunctionTx.
 *                     the handle of Com_MainFunctionTx,when the tx mode is period.
 *                     return the boolean value for the tx pdu need to transmit or not.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxIpduStatePtr,TxIpduPtr,MDTRun
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(boolean, COM_CODE)
    Com_MainFuncTxPeriod(Com_TxIPduRunTimeStateType* TxIpduStatePtr, const Com_TxIPduType* TxIpduPtr, boolean MDTRun)
{
    boolean ret = FALSE;
    PduIdType txModeIndex;
    if (TxIpduStatePtr->PeriodCnt > 0u)
    {
        (TxIpduStatePtr->PeriodCnt) -= 1u;
    }
    if (0u == TxIpduStatePtr->PeriodCnt)
    {
/*delay transmit of the MDT */
#if (STD_ON == COM_MDT_ENABLE)
        if (MDTRun)
        {
            TxIpduStatePtr->TxIpduRTStFlag |= Com_TX_DELAY_EN;
        }
        else
#endif /* STD_ON == COM_MDT_ENABLE */
        {
            ret = TRUE;
        }

#if (STD_ON == COM_TMS_ENABLE)
        uint8 TMS = (TxIpduStatePtr->TxIpduRTStFlag & Com_TX_TMS_EN);
        if (TMS != Com_TX_TMS_EN)
        {
            txModeIndex = TxIpduPtr->ComTxModeFalseRefId;
            if (COM_TX_MODE_MIXED == TxIpduStatePtr->ipduTxMode)
            {
#if (0u < COM_TX_MODE_FALSE_MIXED_NUMBER)
                TxIpduStatePtr->PeriodCnt = Com_TxModeFalseMixedRepetitions[txModeIndex].ComTxModeTimePeriod;
#endif
            }
            else
            {
#if ((0u < COM_TX_MODE_FALSE_PERIOD_NUMBER) || (0u < COM_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER))
                TxIpduStatePtr->PeriodCnt = Com_TxModeFalsePeriod[txModeIndex].ComTxModeTimePeriod;
#endif
            }
        }
        else
#endif
        {
            txModeIndex = TxIpduPtr->ComTxModeTrueRefId;
            if (COM_TX_MODE_MIXED == TxIpduStatePtr->ipduTxMode)
            {
#if (0u < COM_TX_MODE_TRUE_MIXED_NUMBER)
                TxIpduStatePtr->PeriodCnt = Com_TxModeTrueMixedRepetitions[txModeIndex].ComTxModeTimePeriod;
#endif
            }
            else
            {
#if (0u < COM_TX_MODE_TRUE_PERIOD_NUMBER) || (0u < COM_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER)
                TxIpduStatePtr->PeriodCnt = Com_TxModeTruePeriod[txModeIndex].ComTxModeTimePeriod;
#endif
            }
        }
    }
    uint8 TxDelay = TxIpduStatePtr->TxIpduRTStFlag & Com_TX_DELAY_EN;
    if ((!MDTRun) && (Com_TX_DELAY_EN == TxDelay))
    {
        ret = TRUE;
    }
    if (ret && (0u == TxIpduStatePtr->DMCnt))
    {
        /*reset the tx MD counter*/
        TxIpduStatePtr->DMCnt = TxIpduPtr->ComTxIpduDM;
    }
    return ret;
}
#endif

#if (0u < COM_TX_MODE_TRUE_MIXED_NUMBER) || (0u < COM_TX_MODE_FALSE_MIXED_NUMBER) \
    || (0u < COM_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER) || (0u < COM_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER)
/******************************************************************************/
/*
 * Brief               Called by Com_MainFunctionTx.
 *                     the handle of Com_MainFunctionTx,when the tx mode is mixed.
 *                     return the boolean value for the tx pdu need to transmit or not
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxIpduStatePtr,TxIpduPtr,MDTRun
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(boolean, COM_CODE)
    Com_MainFuncTxMixed(Com_TxIPduRunTimeStateType* TxIpduStatePtr, const Com_TxIPduType* TxIpduPtr, boolean MDTRun)
{
    boolean transmitRequest = FALSE;
    boolean directTransmitRequest = Com_MainFuncTxDirect(TxIpduStatePtr, TxIpduPtr, MDTRun);
    boolean periodTransmitRequest = Com_MainFuncTxPeriod(TxIpduStatePtr, TxIpduPtr, MDTRun);

    if (directTransmitRequest || periodTransmitRequest)
    {
        transmitRequest = TRUE;
    }
    return transmitRequest;
}
#endif

#if (STD_ON == COM_TX_SIGNAL_ERROR_NOTIFICATION_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_ERROR_NOTIFICATION_ENABLE)
/******************************************************************************/
/*
 * Brief               Called by Com_TriggerTransmit,Com_TriggerIPDUSend,
 *                     Com_TriggerIPDUSendWithMetaData,Com_TxIpduController.
 *                     It is called in case the transmission is not possible
 *                     because the corresponding I-PDU group is stopped.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      IpduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE) Com_InvokeErrorNotification(PduIdType IpduId)
{
    const Com_TxIPduType* txIpduPtr = &Com_ConfigStd->ComTxIPdu[IpduId];
#if (COM_TXSIG_ERROR_AND_NOTIFY_NUMBER > 0u)
    Com_TxSignalIdType txSignalNumber, cnt;
    const Com_TxSignalType* txSignalPtr;
    txSignalNumber = txIpduPtr->ComIPduSignalsRefNumber;
    cnt = txIpduPtr->ComIpduSignalRefStartId;
    for (; cnt < txSignalNumber; ++cnt)
    {
        txSignalPtr = &Com_ConfigStd->ComTxSignal[cnt];
        /*call signal Tx Err Notification*/
        if (COM_TX_SIGNAL_ERROR_NOTIFICATION_EN
            == (txSignalPtr->ComTxSignalNotificationType & COM_TX_SIGNAL_ERROR_NOTIFICATION_EN))
        {
            /* only ErrorNotify or both ErrorNotify and Notify*/
            Com_TxSignalErrorAndNotification[txSignalPtr->ComTxSignalErrAndNotifyFncId]();
        }
    }
#endif
#if (COM_TXSIGGRP_ERROR_AND_NOTIFY_NUMBER > 0u)
    Com_TxSignalGroupIdType signalGroupNumber, signalGroupId;
    signalGroupNumber = txIpduPtr->ComIPduSignalGroupsRefNumber;
    signalGroupId = txIpduPtr->ComIPduSignalGroupsRefStartId;
    const Com_TxSignalGroupType* txSignalGroupPtr;
    for (; signalGroupId < signalGroupNumber; ++signalGroupId)
    {
        txSignalGroupPtr = &Com_ConfigStd->ComTxSignalGroup[signalGroupId];
        /*call signal group Tx Err Notification*/
        if (COM_TX_SIGNAL_ERROR_NOTIFICATION_EN
            == (txSignalGroupPtr->ComTxSignalNotificationType & COM_TX_SIGNAL_ERROR_NOTIFICATION_EN))
        {
            Com_TxSignalGroupErrorAndNotification[txSignalGroupPtr->ComTxSigGrpErrAndNotifyFncId]();
        }
    }
#endif
    return;
}
#endif

#if (0u < COM_TXIPDU_NUMBER)
/******************************************************************************/
/*
 * Brief               Send the tx pdu in Com_MainFunctionTx
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      txIpduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE) Com_MainFunction_SendPdu(PduIdType TxIpduId)
{
    Com_TxIPduRunTimeStateType* txIpduStatePtr = &Com_TxIPduRunTimeState[TxIpduId];
    const Com_TxIPduType* txIpduPtr = &Com_ConfigStd->ComTxIPdu[TxIpduId];
    PduInfoType pduInfo;
    Com_TxIpduBufIdType ipduBufferId = txIpduPtr->ComTxIPduBufIndex;
    uint16 ipduLength = txIpduStatePtr->TxIpduLength;

#if (STD_ON == COM_TX_IPDU_COUNTER_ENABLE)
    uint8 txPduCounter = 0u;
    if (COM_UNUSED_TXIPDUCOUNTERID != txIpduPtr->ComIPduCounterIndex)
    {
        /*Pack the counter value in the pdu buffer*/
        Com_PackCounterValue(txIpduStatePtr, txIpduPtr);
        /*recover the active counter value if the transmit is E_NOT_OK*/
        txPduCounter = txIpduStatePtr->TxIpduCounter;
        /*Increase the counter value by +1*/
        Com_PduCounterIncrease(TRUE, TxIpduId);
    }
#endif /* STD_ON == COM_TX_IPDU_COUNTER_ENABLE */
    pduInfo.SduDataPtr = &Com_TxIPduRuntimeBuff[ipduBufferId];
    pduInfo.SduLength = ipduLength;
#if (STD_ON == COM_METADATA_SUPPORT)
    uint8 metaDataLength = txIpduPtr->MetaDataLength;
    if (0u < metaDataLength)
    {
        PduLengthType metaDataPos =
            txIpduPtr->IPduNoDynSignalLength + txIpduPtr->IPduMaxDynSignalLength + (PduLengthType)ipduBufferId;
        pduInfo.MetaDataPtr = &Com_TxIPduRuntimeBuff[metaDataPos];
    }
    else
#endif
    {
        pduInfo.MetaDataPtr = NULL_PTR;
    }
#if (STD_ON == COM_TX_IPDU_CALLOUT_ENABLE)
    boolean ret = TRUE;
    /*invoke the callout API*/
    if (NULL_PTR != txIpduPtr->ComIPduCallout)
    {
        ret = txIpduPtr->ComIPduCallout(TxIpduId + COM_RXIPDU_NUMBER, &pduInfo);
    }
    if (ret)
#endif /* STD_ON == COM_TX_IPDU_CALLOUT_ENABLE */
    {
#if (STD_ON == COM_TXTPPDU_SUPPORT)
        if (COM_PDU_TP == txIpduPtr->ComIPduType)
        {
            pduInfo.SduDataPtr = NULL_PTR;
        }
#endif /* STD_ON == COM_TXTPPDU_SUPPORT */
        /*Set the Transmiting flag before call PduR_Transmit,
         *avoid the TxConfirmation come before set the Transmiting flag*/
        txIpduStatePtr->TxIpduRTStFlag |= Com_TX_TRANSMITING_EN;
        if (E_OK == PduR_ComTransmit(txIpduPtr->ComPduIdRef, &pduInfo))
        {
            uint8 TxDelay = txIpduStatePtr->TxIpduRTStFlag & Com_TX_DELAY_EN;
            if (Com_TX_DELAY_EN == TxDelay)
            {
                txIpduStatePtr->TxIpduRTStFlag &= Com_TX_DELAY_DIS;
            }
/*Reset Tx_IPduMDT*/
#if (STD_ON == COM_MDT_ENABLE)
            Com_ResetTxIpduMDT(txIpduStatePtr, txIpduPtr);
#endif
#if (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE) || (STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE) \
    || (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
            if (COM_TRANSMIT == txIpduPtr->ComTxIPduClearUpdateBit)
            {
                /*clear all signal group/signal/dest description signal update bit of the Tx Pdu*/
                Com_ClearTxIPduUpdates(txIpduPtr);
            }
#endif /* STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE) || (STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE) || (STD_ON == \
COM_GW_DEST_SIG_UPDATE_BIT_ENABLE */
            txIpduStatePtr->TxOffset = 0u;
        }
        else
        {
            txIpduStatePtr->TxIpduRTStFlag &= Com_TX_TRANSMITING_DIS;
#if (STD_ON == COM_TX_IPDU_COUNTER_ENABLE)
            if (COM_UNUSED_TXIPDUCOUNTERID != txIpduPtr->ComIPduCounterIndex)
            {
                /*transmit not OK,recover the old counter value*/
                txIpduStatePtr->TxIpduCounter = txPduCounter;
            }
#endif /* STD_ON == COM_TX_IPDU_COUNTER_ENABLE */
/*ComRetryFailedTransmitRequests*/
#if (STD_ON == COM_RETRY_FAILED_TRANSMIT_REQUESTS)
            txIpduStatePtr->TxIpduRTStFlag |= Com_TX_DELAY_EN;
#else
/*Reset Tx_IPduMDT*/
#if (STD_ON == COM_MDT_ENABLE)
            Com_ResetTxIpduMDT(txIpduStatePtr, txIpduPtr);
#endif
#endif /*STD_ON == COM_RETRY_FAILED_TRANSMIT_REQUESTS*/
        }
    }
#if (STD_ON == COM_TX_IPDU_CALLOUT_ENABLE) && (STD_ON == COM_TX_IPDU_COUNTER_ENABLE)
    else
    {
        if (COM_UNUSED_TXIPDUCOUNTERID != txIpduPtr->ComIPduCounterIndex)
        {
            /*transmit not OK,recover the old counter value*/
            txIpduStatePtr->TxIpduCounter = txPduCounter;
        }
    }
#endif /* STD_ON == COM_TX_IPDU_CALLOUT_ENABLE && STD_ON == COM_TX_IPDU_COUNTER_ENABLE */
    return;
}
#endif

#if (STD_ON == COM_TX_IPDU_COUNTER_ENABLE)
/******************************************************************************/
/*
 * Brief               Called by Com_MainFunctionTx,Com_TriggerIPDUSend,
 *                     Com_TriggerIPDUSendWithMetaData.
 *                     Pack the counter value in the pdu buffer.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxIpduStatePtr,TxIpduPtr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE)
    Com_PackCounterValue(const Com_TxIPduRunTimeStateType* TxIpduStatePtr, const Com_TxIPduType* TxIpduPtr)
{
#if (0u < COM_TXIPDUBUFF_SIZE)
    uint8 txCounter;
    Com_TxIpduBufIdType ipduBufferIndex = TxIpduPtr->ComTxIPduBufIndex;
    Com_TxIPduCounterIdType ipduCounterId = TxIpduPtr->ComIPduCounterIndex;
    uint32 bitPosition = Com_TxIPduCounter[ipduCounterId].ComIPduCounterStartPosition;
    uint8 bitSize = Com_TxIPduCounter[ipduCounterId].ComIPduCounterSize;
    uint8 bitPosInByte = (uint8)(bitPosition & 0x07u);

    ipduBufferIndex = ipduBufferIndex + (Com_TxIpduBufIdType)(bitPosition >> 3u);
    /* Align LSB of txData and signal_value in IPdu */
    txCounter = TxIpduStatePtr->TxIpduCounter << bitPosInByte;
    /*Clear corresponding bits in IPdu*/
    Com_TxIPduRuntimeBuff[ipduBufferIndex] &=
        (COM_UNUSED_UINT8 >> (8u - bitPosInByte)) | (uint8)(COM_UNUSED_UINT8 << (bitPosInByte + bitSize));
    /*write corresponding bits to IPdu buffer*/
    Com_TxIPduRuntimeBuff[ipduBufferIndex] |= ((COM_UNUSED_UINT8 >> (8u - (bitPosInByte + bitSize))) & txCounter);
#endif
    return;
}
#endif

#if (0u < COM_TMCTXSIGNAL_NUMBER) && (STD_ON == COM_TMS_ENABLE)
static FUNC(uint64, COM_CODE) Com_GetOldSignalValue(Com_SignalType SignalType, uint16 SignalBufferId)
{
    uint64 oldValue = COM_UNUSED_UINT64;
    switch (SignalType)
    {
/*According to the signal type,
 *Calculated the signal value is filtered or not?*/
#if (0u < COM_SIGNAL_BOOLBUFF_SIZE)
    case COM_BOOLEAN:
        /* PRQA S 4304 ++ */ /* MISRA Rule 10.5 */
        oldValue = (uint64)Com_SignalBoolRuntimeBuff[SignalBufferId];
        /* PRQA S 4304 -- */ /* MISRA Rule 10.5 */
        break;
#endif
#if (0u < COM_SIGNAL_8BITBUFF_SIZE)
    case COM_UINT8:
    case COM_SINT8:
        oldValue = (uint64)Com_Signal8BitRuntimeBuff[SignalBufferId];
        break;
#endif
#if (0u < COM_SIGNAL_16BITBUFF_SIZE)
    case COM_UINT16:
    case COM_SINT16:
        oldValue = (uint64)Com_Signal16BitRuntimeBuff[SignalBufferId];
        break;
#endif
#if (0u < COM_SIGNAL_32BITBUFF_SIZE)
    case COM_UINT32:
    case COM_SINT32:
        oldValue = (uint64)Com_Signal32BitRuntimeBuff[SignalBufferId];
        break;
#endif
#if (0u < COM_SIGNAL_64BITBUFF_SIZE)
    case COM_UINT64:
    case COM_SINT64:
        oldValue = (uint64)Com_Signal8BitRuntimeBuff[SignalBufferId];
        break;
#endif
    default:
        /*do nothing*/
        break;
    }
    return oldValue;
}
#endif
#if (0u < COM_TXSIGNAL_NUMBER) && (STD_ON == COM_TMS_ENABLE)
/******************************************************************************/
/*
 * Brief               the tx signal value change,calculate the new TMC and TMS.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxSignalId,TxPduId,TxSignalDataPtr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE) Com_TxSignalTMHandle(
    Com_TxIPduRunTimeStateType* TxIpduStatePtr,
    const Com_TxIPduType* TxIpduPtr,
    Com_SignalIdType TxSignalId,
    P2CONST(void, AUTOMATIC, COM_APPL_CONST) TxSignalDataPtr)
{
#if (0u < COM_TXSIGNAL_FILTERTYPE_MAX_NUMBER)
    const Com_TxSignalType* txSignalPtr = &Com_ConfigStd->ComTxSignal[TxSignalId];
    if (COM_UNUSED_TXSIGNALFILTERID != txSignalPtr->ComFilterIndex)
    {
        Com_SignalIdType txSignalTMCId = txSignalPtr->ComTMCBufferId;
        uint64 oldValue = Com_GetOldSignalValue(txSignalPtr->ComSignalType, txSignalPtr->ComSignalInitValueId);
        /*calculate the TMC of the signal*/
        Com_TxSignalTMCRunTime[txSignalTMCId] =
            Com_TxSignalTMCCalculate(COM_TX_SIGNAL, TxSignalDataPtr, txSignalPtr, oldValue);
    }
#endif

    uint8 oldTMS = TxIpduStatePtr->TxIpduRTStFlag & Com_TX_TMS_EN;
    boolean TMS = Com_TxIpduTMSCalculate(TxIpduPtr);
    if (!TMS && (Com_TX_TMS_EN == oldTMS))
    {
        TxIpduStatePtr->TxIpduRTStFlag &= Com_TX_TMS_DIS;
        TxIpduStatePtr->ipduTxMode = TxIpduPtr->ComTxFalseModeMode;
        if ((COM_TX_MODE_MIXED == TxIpduStatePtr->ipduTxMode)
            || (COM_TX_MODE_MIXED_WITHOUT_REPETITION == TxIpduStatePtr->ipduTxMode)
            || (COM_TX_MODE_PERIODIC == TxIpduStatePtr->ipduTxMode))
        {
            Com_GetTxModeOffset(TxIpduStatePtr, TxIpduPtr, FALSE);
        }
    }
    else if (TMS && (Com_TX_TMS_EN != oldTMS))
    {
        TxIpduStatePtr->TxIpduRTStFlag |= Com_TX_TMS_EN;
        TxIpduStatePtr->ipduTxMode = TxIpduPtr->ComTxTrueModeMode;
        if ((COM_TX_MODE_MIXED == TxIpduStatePtr->ipduTxMode)
            || (COM_TX_MODE_MIXED_WITHOUT_REPETITION == TxIpduStatePtr->ipduTxMode)
            || (COM_TX_MODE_PERIODIC == TxIpduStatePtr->ipduTxMode))
        {
            Com_GetTxModeOffset(TxIpduStatePtr, TxIpduPtr, TRUE);
        }
    }
    else
    {
        /* do nothing */
    }
    COM_NOUSED(TxSignalId);
    COM_NOUSED(TxSignalDataPtr);
    return;
}
#endif

#if ((COM_TXGROUPSIGNAL_NUMBER > 0u) || (0u < COM_TXSIGNAL_NUMBER))
/******************************************************************************/
/*
 * Brief               Called by Com_SendSignal,Com_SendTxGroupSignalHandle.
 *                     set Tx signal(not dynamic signal) to signalbuffer,return
 *                     the buffer value is changed or not.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalType,SignalLength,SignalBufferId,SignalNewDataPtr,BitSize
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/******************************************************************************/
static FUNC(boolean, COM_CODE) Com_SetTxSignalBuff(
    Com_SignalType SignalType,
    uint16 SignalLength,
    uint16 SignalBufferId,
    P2CONST(void, AUTOMATIC, COM_APPL_CONST) SignalNewDataPtr,
    uint8 BitSize)
{
#if (0u < COM_SIGNAL_8BITBUFF_SIZE)
    uint8 signalValueU8;
#endif
#if (0u < COM_SIGNAL_16BITBUFF_SIZE)
    uint16 signalValueU16;
#endif
#if (0u < COM_SIGNAL_32BITBUFF_SIZE)
    uint32 signalValueU32;
#endif
#if (0u < COM_SIGNAL_64BITBUFF_SIZE)
    uint64 signalNewValue;
#endif
    boolean ret = FALSE;

    SchM_Enter_Com_Context();
    switch (SignalType)
    {
#if (0u < COM_SIGNAL_BOOLBUFF_SIZE)
    case COM_BOOLEAN:
        if (*((const boolean*)(SignalNewDataPtr)) != Com_SignalBoolRuntimeBuff[SignalBufferId])
        {
            Com_SignalBoolRuntimeBuff[SignalBufferId] = *((const boolean*)SignalNewDataPtr);
            ret = TRUE;
        }
        break;
#endif
#if (0u < COM_SIGNAL_8BITBUFF_SIZE)
    case COM_UINT8:
    case COM_SINT8:
        signalValueU8 = *((const uint8*)SignalNewDataPtr) & (COM_UNUSED_UINT8 >> (8u - BitSize));
        if (signalValueU8 != Com_Signal8BitRuntimeBuff[SignalBufferId])
        {
            Com_Signal8BitRuntimeBuff[SignalBufferId] = signalValueU8;
            ret = TRUE;
        }
        break;
    case COM_UINT8_N:
        /* Copy Uint8N signal value one byte by one */
        if (0 != ILib_memcmp(&Com_Signal8BitRuntimeBuff[SignalBufferId], (const uint8*)SignalNewDataPtr, SignalLength))
        {
            (void)ILib_memcpy(&Com_Signal8BitRuntimeBuff[SignalBufferId], (const uint8*)SignalNewDataPtr, SignalLength);
            ret = TRUE;
        }
        break;
#endif
#if (0u < COM_SIGNAL_16BITBUFF_SIZE)
    case COM_UINT16:
    case COM_SINT16:
        signalValueU16 = *((const uint16*)SignalNewDataPtr) & (COM_UNUSED_UINT16 >> (16u - BitSize));
        if (signalValueU16 != Com_Signal16BitRuntimeBuff[SignalBufferId])
        {
            Com_Signal16BitRuntimeBuff[SignalBufferId] = signalValueU16;
            ret = TRUE;
        }
        break;
#endif
#if (0u < COM_SIGNAL_32BITBUFF_SIZE)
    case COM_SINT32:
    case COM_UINT32:
    case COM_FLOAT32:
        signalValueU32 = *((const uint32*)SignalNewDataPtr);
        signalValueU32 =
            (COM_FLOAT32 != SignalType) ? (signalValueU32 & (COM_UNUSED_UINT32 >> (32u - BitSize))) : signalValueU32;
        if (signalValueU32 != Com_Signal32BitRuntimeBuff[SignalBufferId])
        {
            Com_Signal32BitRuntimeBuff[SignalBufferId] = signalValueU32;
            ret = TRUE;
        }
        break;
#endif
#if (0u < COM_SIGNAL_64BITBUFF_SIZE)
    case COM_SINT64:
    case COM_UINT64:
    case COM_FLOAT64:
        signalNewValue = *((const uint64*)SignalNewDataPtr);
        signalNewValue =
            (COM_FLOAT64 != SignalType) ? (signalNewValue & (COM_UNUSED_UINT64 >> (64u - BitSize))) : signalNewValue;
        if (signalNewValue != Com_Signal64BitRuntimeBuff[SignalBufferId])
        {
            Com_Signal64BitRuntimeBuff[SignalBufferId] = signalNewValue;
            ret = TRUE;
        }
        break;
#endif
    default:
        /*do nothing*/
        break;
    }
    SchM_Exit_Com_Context();
    COM_NOUSED(SignalLength);
    COM_NOUSED(SignalBufferId);
    COM_NOUSED(SignalNewDataPtr);
    return ret;
}
#endif
#if (0u < COM_TXSIGNAL_NUMBER)
/******************************************************************************/
/*
 * Brief               Called by Com_SendSignal.
 *                     pack the Tx signal into ipdu Tx buffer.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxSignalId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE) Com_TxSignalPack(Com_SignalIdType TxSignalId)
{
    const Com_TxSignalType* txSignalPtr = &Com_ConfigStd->ComTxSignal[TxSignalId];
    PduIdType txIpduId = txSignalPtr->ComIpduRefIndex;
    Com_SignalType signalType = txSignalPtr->ComSignalType;
    Com_TxIpduBufIdType ipduBufferIndex = Com_ConfigStd->ComTxIPdu[txIpduId].ComTxIPduBufIndex;
    uint16 signalBufferId = txSignalPtr->ComSignalInitValueId;
    uint64 txData;
#if (                                                                                      \
    (0u < COM_TXIPDUBUFF_SIZE)                                                             \
    && (((0u < COM_SIGNAL_8BITBUFF_SIZE) && (STD_ON == COM_TX_SIGNAL_TYPE_UINT8_N_ENABLE)) \
        || (0u < COM_SIGNAL_BOOLBUFF_SIZE)))
    Com_TxIpduBufIdType ipduBuffPos = ipduBufferIndex + (Com_TxIpduBufIdType)(txSignalPtr->ComSigLsbBytePos);
    uint8* iPduBufferPtr = &Com_TxIPduRuntimeBuff[ipduBuffPos];
#endif

    if (COM_UINT8_N == signalType)
    {
#if ((0u < COM_SIGNAL_8BITBUFF_SIZE) && (0u < COM_TXIPDUBUFF_SIZE) && (STD_ON == COM_TX_SIGNAL_TYPE_UINT8_N_ENABLE))
        /*pack COM_UINT8_N type group signal*/
        uint16 signalLength = txSignalPtr->ComSignalLength;
        SchM_Enter_Com_Context();
        (void)ILib_memcpy(iPduBufferPtr, &Com_Signal8BitRuntimeBuff[signalBufferId], signalLength);
        SchM_Exit_Com_Context();
#endif
    }
    /*pack group signal expect signal type(boolean,COM_UINT8_N,COM_UINT8_DYN)*/
    else if (COM_BOOLEAN == signalType)
    {
#if ((0u < COM_SIGNAL_BOOLBUFF_SIZE) && (0u < COM_TXIPDUBUFF_SIZE))
        uint8 value = (uint8)(1u << ((uint8)txSignalPtr->ComSignalPosInByte));
        if (Com_SignalBoolRuntimeBuff[signalBufferId])
        {
            *iPduBufferPtr |= value;
        }
        else
        {
            *iPduBufferPtr &= ~value;
        }
#endif /* 0u < COM_SIGNAL_BOOLBUFF_SIZE && 0u < COM_TXIPDUBUFF_SIZE */
    }
    /*pack signal expect signal type(boolean,COM_UINT8_N,COM_UINT8_DYN)*/
    else
    {
        /*get the signal value need to be packed*/
        switch (signalType)
        {
#if (0u < COM_SIGNAL_8BITBUFF_SIZE)
        case COM_UINT8:
        case COM_SINT8:
            txData = (uint64)(Com_Signal8BitRuntimeBuff[signalBufferId]);
            break;
#endif
#if (0u < COM_SIGNAL_16BITBUFF_SIZE)
        case COM_UINT16:
        case COM_SINT16:
            txData = (uint64)(Com_Signal16BitRuntimeBuff[signalBufferId]);
            break;
#endif
#if (0u < COM_SIGNAL_32BITBUFF_SIZE)
        case COM_SINT32:
        case COM_UINT32:
        case COM_FLOAT32:
            txData = (uint64)(Com_Signal32BitRuntimeBuff[signalBufferId]);
            break;
#endif
#if (0u < COM_SIGNAL_64BITBUFF_SIZE)
        case COM_SINT64:
        case COM_UINT64:
        case COM_FLOAT64:
            txData = Com_Signal64BitRuntimeBuff[signalBufferId];
            break;
#endif
        default:
            txData = 0u;
            break;
        }
        Com_TxSignalPackHandle(COM_TX_SIGNAL, txSignalPtr, txData, &Com_TxIPduRuntimeBuff[ipduBufferIndex]);
    }
}
#endif

#if (0u < COM_TXSIGNAL_NUMBER)
/******************************************************************************/
/*
 * Brief               Called by Com_SendSignal,Com_SendDynSignal.
 *                     send Tx signal handle.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxIpduId,SignalTxProperty,DMSignal,ValueChanged
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint8
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
static FUNC(uint8, COM_CODE) Com_SendTxSignalHandle(
    /* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
    PduIdType TxIpduId,
    Com_TransferPropertyType SignalTxProperty,
    boolean DMSignal,
    boolean ValueChanged)
{
    /* PRQA S 2983 ++ */ /* MISRA Rule 2.2*/
    const Com_TxIPduType* txIpduPtr = &Com_ConfigStd->ComTxIPdu[TxIpduId];
    Com_TxIPduRunTimeStateType* txIpduStatePtr = &Com_TxIPduRunTimeState[TxIpduId];
    uint16 txIpduDM = txIpduPtr->ComTxIpduDM;
    Com_TxModeNumberOfRepetitionsType txModeRptNum = 1u;
    PduIdType txModeIndex;
    Com_TxModeModeType ipduTxMode = txIpduStatePtr->ipduTxMode;
#if ((STD_ON == COM_TX_SIG_PROP_TRIGGERED_ENABLE) || (STD_ON == COM_TX_SIG_PROP_TRIGGERED_ON_CHANGE_ENABLE))
    boolean initDMCnt = FALSE;
#endif
    /*the signal config timeout,the DM timer isn't start by other signals,the Tx mode is not NONE.then start the DM
     * timer here*/
    if ((0u == txIpduStatePtr->DMCnt) && DMSignal && (COM_TX_MODE_NONE != txIpduStatePtr->ipduTxMode))
    {
        txIpduStatePtr->DMCnt = txIpduDM;
#if ((STD_ON == COM_TX_SIG_PROP_TRIGGERED_ENABLE) || (STD_ON == COM_TX_SIG_PROP_TRIGGERED_ON_CHANGE_ENABLE))
        initDMCnt = TRUE;
#endif
    }
#if (0u < COM_TX_MODE_TRUE_DIRECT_NUMBER) || (0u < COM_TX_MODE_TRUE_MIXED_NUMBER)                              \
    || (0u < COM_TX_MODE_FALSE_DIRECT_NUMBER) || (0u < COM_TX_MODE_FALSE_MIXED_NUMBER)                         \
    || (0u < COM_TX_MODE_TRUE_DIRECT_NOREPETITION_NUMBER) || (0u < COM_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER) \
    || (0u < COM_TX_MODE_FALSE_DIRECT_NOREPETITION_NUMBER) || (0u < COM_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER)
    if ((COM_TX_MODE_DIRECT == ipduTxMode) || (COM_TX_MODE_MIXED == ipduTxMode)
        || (COM_TX_MODE_DIRECT_WITHOUT_REPETITION == ipduTxMode)
        || (COM_TX_MODE_MIXED_WITHOUT_REPETITION == ipduTxMode))
    {
        if ((COM_TX_MODE_DIRECT == ipduTxMode) || (COM_TX_MODE_MIXED == ipduTxMode))
        {
#if (STD_ON == COM_TMS_ENABLE)
            uint8 TMS = txIpduStatePtr->TxIpduRTStFlag & Com_TX_TMS_EN;
            if (TMS != Com_TX_TMS_EN)
            {
                txModeIndex = txIpduPtr->ComTxModeFalseRefId;
                if (COM_TX_MODE_DIRECT == ipduTxMode)
                {
#if (0u < COM_TX_MODE_FALSE_DIRECT_NUMBER)
                    txModeRptNum = Com_TxModeFalseDirectRepetitions[txModeIndex].ComTxModeNumberOfRepetitions;
#endif
                }
                else
                {
#if (0u < COM_TX_MODE_FALSE_MIXED_NUMBER)
                    txModeRptNum = Com_TxModeFalseMixedRepetitions[txModeIndex].ComTxModeNumberOfRepetitions;
#endif
                }
            }
            else
#endif
            {
                txModeIndex = txIpduPtr->ComTxModeTrueRefId;
                if (COM_TX_MODE_DIRECT == ipduTxMode)
                {
#if (0u < COM_TX_MODE_TRUE_DIRECT_NUMBER)
                    txModeRptNum = Com_TxModeTrueDirectRepetitions[txModeIndex].ComTxModeNumberOfRepetitions;
#endif
                }
                else
                {
#if (0u < COM_TX_MODE_TRUE_MIXED_NUMBER)
                    txModeRptNum = Com_TxModeTrueMixedRepetitions[txModeIndex].ComTxModeNumberOfRepetitions;
#endif
                }
            }
        }

        switch (SignalTxProperty)
        {
#if (STD_ON == COM_TX_SIG_PROP_TRIGGERED_ENABLE)
        case COM_TRIGGERED:
            txIpduStatePtr->NTimeCnt = 0u;
            txIpduStatePtr->RptNum = txModeRptNum;
            /*For an I-PDU with ComTxModeMode DIRECT and ComTxModeNumberOfRepetitions > 0, the AUTOSAR COM module
              shall reset an already running transmission deadline monitoring timer in case another send request for
              this I-PDU is initiated*/
            if ((1u < txModeRptNum) && (!initDMCnt) && DMSignal)
            {
                txIpduStatePtr->DMCnt = txIpduDM;
            }
            break;
#endif /* STD_ON == COM_TX_SIG_PROP_TRIGGERED_ENABLE */
#if (STD_ON == COM_TX_SIG_PROP_TRIGGERED_ON_CHANGE_ENABLE)
        case COM_TRIGGERED_ON_CHANGE:
            if (ValueChanged)
            {
                txIpduStatePtr->NTimeCnt = 0u;
                txIpduStatePtr->RptNum = txModeRptNum;
                /*For an I-PDU with ComTxModeMode DIRECT and ComTxModeNumberOfRepetitions > 0, the AUTOSAR COM
                  module shall reset an already running transmission deadline monitoring timer in case another send
                  request for this I-PDU is initiated*/
                if ((1u < txModeRptNum) && (!initDMCnt) && DMSignal)
                {
                    txIpduStatePtr->DMCnt = txIpduDM;
                }
            }
            break;
#endif /* STD_ON == COM_TX_SIG_PROP_TRIGGERED_ON_CHANGE_ENABLE */
#if (STD_ON == COM_TX_SIG_PROP_TRIG_ON_CH_NO_REPETITION_ENABLE)
        case COM_TRIGGERED_ON_CHANGE_WITHOUT_REPETITION:
            if (ValueChanged)
            {
                txIpduStatePtr->NTimeCnt = 0u;
                txIpduStatePtr->RptNum = 1u;
            }
            break;
#endif /* STD_ON == COM_TX_SIG_PROP_TRIG_ON_CH_NO_REPETITION_ENABLE */
#if (STD_ON == COM_TX_SIG_PROP_TRIG_NO_REPETITION_ENABLE)
        case COM_TRIGGERED_WITHOUT_REPETITION:
            txIpduStatePtr->NTimeCnt = 0u;
            txIpduStatePtr->RptNum = 1u;
            break;
#endif /* STD_ON == COM_TX_SIG_PROP_TRIG_NO_REPETITION_ENABLE */
        default:
            /*do nothing*/
            break;
        }
    }
    else
#endif
    {
        txIpduStatePtr->NTimeCnt = 0u;
        txIpduStatePtr->RptNum = 0u;
    }
    /* PRQA S 2983 -- */ /* MISRA Rule 2.2*/
    COM_NOUSED(ValueChanged);
    return E_OK;
}
#endif

#if (COM_TXGROUPSIGNAL_NUMBER > 0u)
/******************************************************************************/
/*
 * Brief               Called by Com_SendSignal,Com_InvalidateSignalGroup.
 *                     send Tx group signal handle.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxGroupSignalId,SignalDataPtr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK,COM_SERVICE_NOT_AVAILABLE
 */
/******************************************************************************/
static FUNC(uint8, COM_CODE)
    /* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
    Com_SendTxGroupSignalHandle(
        Com_SignalIdType TxGroupSignalId,
        P2CONST(void, AUTOMATIC, COM_APPL_CONST) SignalDataPtr)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    boolean valueChanged;
    const Com_TxGroupSignalType* txGroupSignalPtr = &Com_ConfigStd->ComTxGroupSignal[TxGroupSignalId];
    Com_TxSignalGroupIdType signalGroupId = txGroupSignalPtr->ComSignalGroupRef;
    const Com_TxSignalGroupType* txSignalGroupPtr = &Com_ConfigStd->ComTxSignalGroup[signalGroupId];
    Com_SignalType groupSignalType = txGroupSignalPtr->ComSignalType;
#if (STD_ON == COM_TX_SIG_GROUP_PROP_TRIGGERED_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_PROP_TRIGGERED_ON_CHANGE_ENABLE) \
    || (STD_ON == COM_TX_SIG_GROUP_PROP_TRIG_ON_CH_NO_REPETITION_ENABLE)                                               \
    || (STD_ON == COM_TX_SIG_GROUP_PROP_TRIG_NO_REPETITION_ENABLE)
    Com_TxSignalGroupRunTimeStateType* Com_TxSigGrpStPtr = &Com_TxSignalGroupState[signalGroupId];
#endif
    uint8 ret;

/*dynamic signal isn't send by the API,InitialValueOnly signal group don't expect to be called by any API*/
#if (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_INITIAL_VALUE_ONLY_ENABLE)
    if ((COM_UINT8_DYN == groupSignalType) || txSignalGroupPtr->ComInitialValueOnly)
    {
        ret = COM_SERVICE_NOT_AVAILABLE;
    }
    else
#endif /* STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE) || (STD_ON == \
          COM_TX_SIG_GROUP_INITIAL_VALUE_ONLY_ENABLE */
    {
#if (0u < COM_TXGRPSIG_FILTERTYPE_MAX_NUMBER) && (STD_ON == COM_TMS_ENABLE)
        if (COM_UNUSED_TXGRPSIGFILTERID != txGroupSignalPtr->ComFilterIndex)
        {
            Com_SignalIdType txSignalTMCId = txGroupSignalPtr->ComTMCBufferId;
            uint64 oldValue =
                Com_GetOldSignalValue(txGroupSignalPtr->ComSignalType, txGroupSignalPtr->ComSignalInitValueId);
            /*calculate the TMC of the signal*/
            Com_TxGroupSignalTMCBuffer[txSignalTMCId] =
                Com_TxSignalTMCCalculate(COM_TX_GROUP_SIGNAL, SignalDataPtr, txGroupSignalPtr, oldValue);
        }
#endif /* 0u < COM_TXGRPSIG_FILTERTYPE_MAX_NUMBER) && (STD_ON == COM_TMS_ENABLE */
        /*groupSignalLength is used when the signal type is COM_UINT8_N*/
        /*update tx signal buffer and check if the sending signal value changed*/
        uint16 groupSignalLength = txGroupSignalPtr->ComSignalLength;
        valueChanged = Com_SetTxSignalBuff(
            groupSignalType,
            groupSignalLength,
            txGroupSignalPtr->ComSignalInitValueId,
            SignalDataPtr,
            txGroupSignalPtr->ComBitSize);
        /* PRQA S 2983 ++ */ /* MISRA Rule 2.2*/
        Com_TransferPropertyType signalTxProperty =
#if (STD_ON == COM_TX_SIG_GROUP_PROP_TRIGGERED_ON_CHANGE_ENABLE)
            (COM_GROUPSIGNAL_TMC_NONE != txGroupSignalPtr->ComTransferProperty) ? txGroupSignalPtr->ComTransferProperty
                                                                                :
#endif
                                                                                txSignalGroupPtr->ComTransferProperty;
        /* PRQA S 2983 -- */ /* MISRA Rule 2.2*/
        /* PRQA S 3315 ++ */ /* MISRA Rule 16.6*/
        switch (signalTxProperty)
        {
#if (STD_ON == COM_TX_SIG_GROUP_PROP_TRIGGERED_ENABLE)
        case COM_TRIGGERED:
            Com_TxSigGrpStPtr->TxSigGrpRTStFlag |= COM_NTIMESTRANSMITNEED_EN;
            Com_TxSigGrpStPtr->TxSigGrpRTStFlag &= COM_WITHOUTREPETITION_DIS;
            break;
#endif /* STD_ON == COM_TX_SIG_GROUP_PROP_TRIGGERED_ENABLE */
#if (STD_ON == COM_TX_SIG_GROUP_PROP_TRIGGERED_ON_CHANGE_ENABLE)
        case COM_TRIGGERED_ON_CHANGE:
            if (valueChanged)
            {
                Com_TxSigGrpStPtr->TxSigGrpRTStFlag |= COM_NTIMESTRANSMITNEED_EN;
                Com_TxSigGrpStPtr->TxSigGrpRTStFlag &= COM_WITHOUTREPETITION_DIS;
            }
            break;
#endif /* STD_ON == COM_TX_SIG_GROUP_PROP_TRIGGERED_ON_CHANGE_ENABLE */
#if (STD_ON == COM_TX_SIG_GROUP_PROP_TRIG_ON_CH_NO_REPETITION_ENABLE)
        case COM_TRIGGERED_ON_CHANGE_WITHOUT_REPETITION:
            if (valueChanged)
            {
                Com_TxSigGrpStPtr->TxSigGrpRTStFlag |= COM_NTIMESTRANSMITNEED_EN;
                Com_TxSigGrpStPtr->TxSigGrpRTStFlag |= COM_WITHOUTREPETITION_EN;
            }
            break;
#endif /* STD_ON == COM_TX_SIG_GROUP_PROP_TRIG_ON_CH_NO_REPETITION_ENABLE */
#if (STD_ON == COM_TX_SIG_GROUP_PROP_TRIG_NO_REPETITION_ENABLE)
        case COM_TRIGGERED_WITHOUT_REPETITION:
            Com_TxSigGrpStPtr->TxSigGrpRTStFlag |= COM_NTIMESTRANSMITNEED_EN;
            Com_TxSigGrpStPtr->TxSigGrpRTStFlag |= COM_WITHOUTREPETITION_EN;
            break;
#endif /* STD_ON == COM_TX_SIG_GROUP_PROP_TRIG_NO_REPETITION_ENABLE */
        default:
            /*do nothing*/
            break;
        }
        /* PRQA S 3315 -- */ /* MISRA Rule 16.6*/
        ret = E_OK;
    }
    COM_NOUSED(SignalDataPtr);
    COM_NOUSED(valueChanged);
    return ret;
}
#endif

#if (COM_IPDUGROUP_MAX > 0u) && (COM_TXIPDUGROUP_NUMBER > 0u)
/******************************************************************************/
/*
 * Brief               used to handle all Tx Ipdu state change or not.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      initialize
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
FUNC(void, COM_CODE)
Com_TxIpduController(boolean initialize)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    Com_TxIPduRunTimeStateType* txIpduStatePtr;
    const Com_TxIPduType* txIpduPtr;
    Com_TxIpduGroupIdType ipduGroupRefNumber;
    Com_TxIpduGroupIdType ipduGroupRefIndex;
    Com_TxIpduGroupIdType ipduGroupId;
    PduIdType ipduCnt;
    uint8 activeEnable;
    Com_TxModeModeType ipduTxMode;
    for (ipduCnt = 0u; ipduCnt < COM_TXIPDU_NUMBER; ++ipduCnt)
    {
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
        if (Com_GetTxIPduPartitionId(ipduCnt) == GetApplicationID())
#endif
        {
            txIpduPtr = &Com_ConfigStd->ComTxIPdu[ipduCnt];
            ipduGroupRefNumber = txIpduPtr->ComIPduGroupsRefNumber;
            /*if the Pdu not included in any Ipdu Group,the state is always start*/
            if (ipduGroupRefNumber > 0u)
            {
                ipduGroupRefIndex = txIpduPtr->ComIPduGroupsRefStartId;
                SchM_Enter_Com_MultiCore_Lock();
                txIpduStatePtr = &Com_TxIPduRunTimeState[ipduCnt];
                ipduTxMode = txIpduStatePtr->ipduTxMode;
                activeEnable = txIpduStatePtr->TxIpduRTStFlag & Com_TX_ACTIVE_EN;
                SchM_Exit_Com_MultiCore_Lock();
                for (; ipduGroupRefIndex < ipduGroupRefNumber; ++ipduGroupRefIndex)
                {
                    ipduGroupId = Com_TxIPduGroupsRef[ipduGroupRefIndex];
                    if (Com_IpduGroupEnable[ipduGroupId])
                    {
                        break;
                    }
                }
                /*at least one Ipdu Group is start,the Pdu shall be active*/
                if (ipduGroupRefIndex < ipduGroupRefNumber)
                {
                    if (Com_TX_ACTIVE_EN != activeEnable)
                    {
                        SchM_Enter_Com_MultiCore_Lock();
                        txIpduStatePtr->TxIpduRTStFlag |= Com_TX_ACTIVE_EN;
                        SchM_Exit_Com_MultiCore_Lock();
#if (STD_ON == COM_MDT_ENABLE)
                        /*reset ComMinimumDelayTime of I-PDUs in transmission*/
                        txIpduStatePtr->MDTCnt = 0u;
                        /*reset Tx time out counter*/
                        if ((COM_UNUSED_UINT16 == txIpduPtr->ComTxModeFalseRefId)
                            && (COM_TX_MODE_NONE == txIpduPtr->ComTxTrueModeMode))
                        {
                            txIpduStatePtr->DMCnt = txIpduPtr->ComTxIpduDM;
                        }
                        else
                        {
                            txIpduStatePtr->DMCnt = 0u;
                        }
#endif
                        /*clear all signal/signal group/dest description signal's update bit of the tx Ipdu*/
#if (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE) || (STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE) \
    || (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
                        Com_ClearTxIPduUpdates(txIpduPtr);
#endif /* STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE || STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE || STD_ON == \
          COM_GW_DEST_SIG_UPDATE_BIT_ENABLE */
#if (COM_ONEEVERYNFILTERSIGNAL_NUMBER > 0u)
                        /*reset OCCURRENCE of filter with ComFilterAlgorithm ONE_EVERY_N*/
                        Com_ResetTxOccurrenceOfPduFilter(ipduCnt);
#endif
                        if (initialize)
                        {
                            /*init the tx ipdu buffer,all signal buffer(included in the ipdu) and init the TMC,TMS*/
                            Com_ResetTxPduBufferAndSignalBuffer(ipduCnt);
                            if ((COM_TX_MODE_MIXED == ipduTxMode) || (COM_TX_MODE_PERIODIC == ipduTxMode)
                                || (COM_TX_MODE_MIXED_WITHOUT_REPETITION == ipduTxMode))
                            {
                                boolean TMS = (Com_TX_TMS_EN == (txIpduStatePtr->TxIpduRTStFlag & Com_TX_TMS_EN));
                                Com_GetTxModeOffset(txIpduStatePtr, txIpduPtr, TMS);
                            }
#if (COM_TXIPDU_COUNTER_NUMBER > 0u)
                            if (COM_UNUSED_TXIPDUCOUNTERID != txIpduPtr->ComIPduCounterIndex)
                            {
                                /*set the I-PDU counter to 0 for I-PDUs with ComIPduDirection configured to SEND*/
                                txIpduStatePtr->TxIpduCounter = 0u;
                            }
#endif
                        }
                    }
                }
                else
                {
                    if (Com_TX_ACTIVE_EN == activeEnable)
                    {
                        SchM_Enter_Com_MultiCore_Lock();
                        txIpduStatePtr->TxIpduRTStFlag &= Com_TX_ACTIVE_DIS;
                        SchM_Exit_Com_MultiCore_Lock();
                        uint8 Transmiting = txIpduStatePtr->TxIpduRTStFlag & Com_TX_TRANSMITING_EN;
                        if (Com_TX_TRANSMITING_EN == Transmiting)
                        {
#if (STD_ON == COM_TX_SIGNAL_ERROR_NOTIFICATION_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_ERROR_NOTIFICATION_ENABLE)
                            Com_InvokeErrorNotification(ipduCnt);
#endif
#if (STD_ON == COM_CANCELLATION_SUPPORT)
                            if (txIpduPtr->ComIPduCancellationSupport)
                            {
                                (void)PduR_ComCancelTransmit(txIpduPtr->ComPduIdRef);
                            }
#endif
                            txIpduStatePtr->TxIpduRTStFlag &= Com_TX_TRANSMITING_DIS;
                        }
                        txIpduStatePtr->DMCnt = 0u;
                    }
                }
            }
        }
    }
}
#endif

#if (                                                                                        \
    ((0u < COM_TXSIGNALGROUP_NUMBER) && (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)) \
    || ((0u < COM_TXSIGNAL_NUMBER) && (STD_ON == COM_TX_SIGNAL_TYPE_UINT8_DYN_ENABLE)))
/******************************************************************************/
/*
 * Brief               Called by Com_SendDynSignal.
 *                     set Tx dynamic signal to signalbuffer,return the buffer
 *                     value is changed or not.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxPduId,SignalBufferId,SignalNewDataPtr,Length
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/******************************************************************************/
static FUNC(boolean, COM_CODE) Com_SetDynTxSignalBuff(
    PduIdType TxPduId,
    uint16 SignalBufferId,
    P2CONST(void, AUTOMATIC, COM_APPL_CONST) SignalNewDataPtr,
    uint16 Length)
{
    boolean ret = FALSE;
    uint16 oldSignalLength;

/* Copy Uint8N signal value one byte by one */
#if (0u < COM_SIGNAL_8BITBUFF_SIZE)
    SchM_Enter_Com_Context();
    if (0 != ILib_memcmp(&Com_Signal8BitRuntimeBuff[SignalBufferId], (const uint8*)SignalNewDataPtr, Length))
    {
        (void)ILib_memcpy(&Com_Signal8BitRuntimeBuff[SignalBufferId], (const uint8*)SignalNewDataPtr, Length);
        ret = TRUE;
    }
    SchM_Exit_Com_Context();
#endif
    if (!ret)
    {
        oldSignalLength = Com_TxIPduRunTimeState[TxPduId].TxIpduLength
                          - (uint16)Com_ConfigStd->ComTxIPdu[TxPduId].IPduNoDynSignalLength;
        if (oldSignalLength != Length)
        {
            ret = TRUE;
        }
    }
    COM_NOUSED(SignalBufferId);
    COM_NOUSED(SignalNewDataPtr);
    return ret;
}
/******************************************************************************/
/*
 * Brief               Called by Com_SendDynSignal,Com_TxSignalGroupPack.
 *                     dynamic signal pack to tx pdu buffer.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxPduId,SigLsbBytePos,SignalBufferId,Length
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE)
    Com_TxDynSignalPack(PduIdType TxPduId, Com_SignalPositionType SigLsbBytePos, uint16 SignalBufferId, uint16 Length)
{
#if ((0u < COM_TXIPDUBUFF_SIZE) && (0u < COM_SIGNAL_8BITBUFF_SIZE))
    Com_TxIpduBufIdType ipduBufferIndex =
        Com_ConfigStd->ComTxIPdu[TxPduId].ComTxIPduBufIndex + (Com_TxIpduBufIdType)SigLsbBytePos;
    SchM_Enter_Com_Context();
    (void)ILib_memcpy(&Com_TxIPduRuntimeBuff[ipduBufferIndex], &Com_Signal8BitRuntimeBuff[SignalBufferId], Length);
    SchM_Exit_Com_Context();
#endif
    COM_NOUSED(TxPduId);
    COM_NOUSED(SigLsbBytePos);
    COM_NOUSED(SignalBufferId);
    COM_NOUSED(Length);
    return;
}
#endif
#if (COM_TXSIGNALGROUP_NUMBER > 0u)
/******************************************************************************/
/*
 * Brief               Called by Com_SendSignalGroup.
 *                     pack the Tx signal group into ipdu Tx buffer.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalGroupId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE) Com_TxSignalGroupPack(Com_SignalGroupIdType SignalGroupId)
{
    const Com_TxSignalGroupType* txSignalGroupPtr = &Com_ConfigStd->ComTxSignalGroup[SignalGroupId];
    const Com_TxGroupSignalType* txGroupSignalPtr;
    Com_TxGroupSignalIdType groupSignalNumber = txSignalGroupPtr->ComGroupSignalRefNumber;
    Com_TxGroupSignalIdType cnt = txSignalGroupPtr->ComGroupSignalRefStartId;

#if (0u < COM_TXGRPSIG_FILTERTYPE_MAX_NUMBER)
    Com_SignalIdType groupSignalTMCId;
#endif
#if (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
    uint16 length;
    PduIdType txPduId;
    uint16 signalBufferId;
    Com_SignalPositionType sigLsbBytePos;
    Com_TxSignalGroupRunTimeStateType* Com_TxSigGrpStPtr = &Com_TxSignalGroupState[SignalGroupId];
#endif /* STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE */

    for (; cnt < groupSignalNumber; ++cnt)
    {
        txGroupSignalPtr = &Com_ConfigStd->ComTxGroupSignal[cnt];

#if (0u < COM_TXGRPSIG_FILTERTYPE_MAX_NUMBER)
        if (COM_UNUSED_TXGRPSIGFILTERID != txGroupSignalPtr->ComFilterIndex)
        {
            groupSignalTMCId = txGroupSignalPtr->ComTMCBufferId;
            Com_TxSignalTMCRunTime[groupSignalTMCId] = Com_TxGroupSignalTMCBuffer[groupSignalTMCId];
        }
#endif
#if (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
        if (COM_UINT8_DYN == txGroupSignalPtr->ComSignalType)
        {
            sigLsbBytePos = txGroupSignalPtr->ComSigLsbBytePos;
            txPduId = txSignalGroupPtr->ComIpduRefIndex;
            signalBufferId = txGroupSignalPtr->ComSignalInitValueId;
            length = Com_TxSigGrpStPtr->DynamicSignalLength;
            Com_TxDynSignalPack(txPduId, sigLsbBytePos, signalBufferId, length);
        }
        else
#endif /* STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE */
        {
            Com_TxGroupSignalPack(txGroupSignalPtr);
        }
    }
    return;
}
#endif
#if ((STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && (COM_TXSIGNALGROUP_NUMBER > 0u))
/******************************************************************************/
/*
 * Brief               Called by Com_SendSignalGroupArray.
 *                     update every group signal buffer,calculate every TMC,
 *                     return the signal group is changed or not for trigger
 *                     transmit.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxSignalGroupId,GroupSignalNumber,TxIPduBufferId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/******************************************************************************/
static FUNC(boolean, COM_CODE)
    /* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
    Com_UpdateTxBufferAndCalculateTMCOfEveryGroupSignal(
        /* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
        Com_SignalGroupIdType TxSignalGroupId,
        Com_TxGroupSignalIdType GroupSignalNumber,
        Com_TxIpduBufIdType TxIPduBufferId)
{
    boolean triggerOnChange = FALSE;
    Com_SignalType groupSignalType;
    uint16 groupSignalInitValueId;
    uint64 groupSignalValue;
#if (0u < COM_TXGRPSIG_FILTERTYPE_MAX_NUMBER) && (STD_ON == COM_TMS_ENABLE)
    Com_SignalIdType txSignalTMCId;
    uint64 oldValue;
#endif /* 0u < COM_TMCTXSIGNAL_NUMBER && STD_ON == COM_TMS_ENABLE */
    const Com_TxGroupSignalType* txGroupSignalPtr;
    const Com_TxSignalGroupType* txSignalGroupPtr = &Com_ConfigStd->ComTxSignalGroup[TxSignalGroupId];
    Com_TxGroupSignalIdType cnt = txSignalGroupPtr->ComGroupSignalRefStartId;

    for (; cnt < GroupSignalNumber; ++cnt)
    {
        txGroupSignalPtr = &Com_ConfigStd->ComTxGroupSignal[cnt];
        groupSignalType = txGroupSignalPtr->ComSignalType;
        groupSignalInitValueId = txGroupSignalPtr->ComSignalInitValueId;
#if (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE)
        if (COM_UINT8_N == groupSignalType)
        {
#if ((0u < COM_TXIPDUBUFF_SIZE) && (0u < COM_SIGNAL_8BITBUFF_SIZE))
            triggerOnChange = Com_TxGroupSignalUpdateUint8N(TxIPduBufferId, cnt, groupSignalInitValueId);
#endif
        }
        else
#endif /* STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE */
        {
            /*UnPack tx group signal from signal group array,used to calculate the TMC of the signal and judge the
             * signal value if changed or not,if changed update the signal value*/
            groupSignalValue = Com_SignalUnPackHandle(
#if (STD_ON == COM_SIGNAL_SIGNED_TYPE_ENABLE)
                groupSignalType,
#endif
                COM_TX_GROUP_SIGNAL,
                txGroupSignalPtr,
                &Com_TxIPduRuntimeBuff[TxIPduBufferId]);
#if (0u < COM_TXGRPSIG_FILTERTYPE_MAX_NUMBER) && (STD_ON == COM_TMS_ENABLE)
            if (COM_UNUSED_TXGRPSIGFILTERID != txGroupSignalPtr->ComFilterIndex)
            {
                txSignalTMCId = txGroupSignalPtr->ComTMCBufferId;
                oldValue =
                    Com_GetOldSignalValue(txGroupSignalPtr->ComSignalType, txGroupSignalPtr->ComSignalInitValueId);
                Com_TxSignalTMCRunTime[txSignalTMCId] =
                    Com_TxSignalTMCCalculate(COM_TX_GROUP_SIGNAL, &groupSignalValue, txGroupSignalPtr, oldValue);
            }
#endif /* 0u < COM_TMCTXSIGNAL_NUMBER) && (STD_ON == COM_TMS_ENABLE */
            SchM_Enter_Com_Context();
            switch (groupSignalType)
            {
#if (0u < COM_SIGNAL_BOOLBUFF_SIZE)
            case COM_BOOLEAN:
                /* PRQA S 4304,4340 ++ */ /* MISRA Rule 10.5,10.5 */
                if (Com_SignalBoolRuntimeBuff[groupSignalInitValueId] != (boolean)groupSignalValue)
                {
                    Com_SignalBoolRuntimeBuff[groupSignalInitValueId] = (boolean)groupSignalValue;
                    triggerOnChange = TRUE;
                }
                /* PRQA S 4304,4340 -- */ /* MISRA Rule 10.5,10.5 */
                break;
#endif
#if (0u < COM_SIGNAL_8BITBUFF_SIZE)
            case COM_UINT8:
            case COM_SINT8:
                if (Com_Signal8BitRuntimeBuff[groupSignalInitValueId] != (uint8)groupSignalValue)
                {
                    Com_Signal8BitRuntimeBuff[groupSignalInitValueId] = (uint8)groupSignalValue;
                    triggerOnChange = TRUE;
                }
                break;
#endif
#if (0u < COM_SIGNAL_16BITBUFF_SIZE)
            case COM_UINT16:
            case COM_SINT16:
                if (Com_Signal16BitRuntimeBuff[groupSignalInitValueId] != (uint16)groupSignalValue)
                {
                    Com_Signal16BitRuntimeBuff[groupSignalInitValueId] = (uint16)groupSignalValue;
                    triggerOnChange = TRUE;
                }
                break;
#endif
#if (0u < COM_SIGNAL_32BITBUFF_SIZE)
            case COM_UINT32:
            case COM_SINT32:
            case COM_FLOAT32:
                if (Com_Signal32BitRuntimeBuff[groupSignalInitValueId] != (uint32)groupSignalValue)
                {
                    Com_Signal32BitRuntimeBuff[groupSignalInitValueId] = (uint32)groupSignalValue;
                    triggerOnChange = TRUE;
                }
                break;
#endif
#if (0u < COM_SIGNAL_64BITBUFF_SIZE)
            case COM_UINT64:
            case COM_SINT64:
            case COM_FLOAT64:
                if (Com_Signal64BitRuntimeBuff[groupSignalInitValueId] != (uint64)groupSignalValue)
                {
                    Com_Signal64BitRuntimeBuff[groupSignalInitValueId] = (uint64)groupSignalValue;
                    triggerOnChange = TRUE;
                }
                break;
#endif
            default:
                /*do nothing*/
                break;
            }
            SchM_Exit_Com_Context();
        }
#if (0u < COM_TXIPDUBUFF_SIZE) && (STD_ON == COM_TX_SIG_GROUP_PROP_TRIGGERED_ON_CHANGE_ENABLE)
        triggerOnChange =
            ((TRUE == triggerOnChange) && (COM_GROUPSIGNAL_TMC_NONE != txGroupSignalPtr->ComTransferProperty))
                ? (txGroupSignalPtr->ComTransferProperty == COM_TRIGGERED_ON_CHANGE)
                : triggerOnChange;
#endif
    }
    return triggerOnChange;
}
#endif

#if (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE) || (STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE) \
    || (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
/******************************************************************************/
/*
 * Brief               Called by Com_TriggerIPDUSend,Com_TriggerIPDUSendWithMetaData,
 *                     Com_TriggerTransmit,Com_TxConfirmation,Com_TpTxConfirmation,
 *                     Com_MainFunctionTx,Com_TxIpduController.
 *                     clear all signal group/signal/dest description signal
 *                     update bit of the Tx Pdu
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxIpduPtr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE) Com_ClearTxIPduUpdates(const Com_TxIPduType* TxIpduPtr)
{
#if (0u < COM_TXIPDUBUFF_SIZE)
#if (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE) || (STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE) \
    || (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
    Com_TxIpduBufIdType ipduBufferIndex = TxIpduPtr->ComTxIPduBufIndex;
    Com_SignalPositionType updateBitPosition;
#endif

#if (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE)
    Com_TxSignalGroupIdType signalGroupNumber, signalGroupId;
    signalGroupNumber = TxIpduPtr->ComIPduSignalGroupsRefNumber;
    signalGroupId = TxIpduPtr->ComIPduSignalGroupsRefStartId;
    const Com_TxSignalGroupType* txSignalGroupPtr;
    for (; signalGroupId < signalGroupNumber; ++signalGroupId)
    {
        txSignalGroupPtr = &Com_ConfigStd->ComTxSignalGroup[signalGroupId];
        updateBitPosition = txSignalGroupPtr->ComUpdateLsbBytePos;
        if (COM_UNUSED_SIGNALPOSITION != updateBitPosition)
        {
            Com_TxIPduRuntimeBuff[ipduBufferIndex + (Com_TxIpduBufIdType)updateBitPosition] &=
                (uint8)(~(txSignalGroupPtr->ComUpdateBitMask));
        }
    }
#endif /* STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE */
#if (STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE)
    Com_TxSignalIdType txSignalNumber, cnt;
    txSignalNumber = TxIpduPtr->ComIPduSignalsRefNumber;
    cnt = TxIpduPtr->ComIpduSignalRefStartId;
    const Com_TxSignalType* txSignalPtr;
    for (; cnt < txSignalNumber; ++cnt)
    {
        txSignalPtr = &Com_ConfigStd->ComTxSignal[cnt];
        updateBitPosition = txSignalPtr->ComUpdateLsbBytePos;
        if (COM_UNUSED_SIGNALPOSITION != updateBitPosition)
        {
            Com_TxIPduRuntimeBuff[ipduBufferIndex + (Com_TxIpduBufIdType)updateBitPosition] &=
                (uint8)(~(txSignalPtr->ComUpdateBitMask));
        }
    }
#endif /* STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE */
#if (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
    Com_GwDestinationDescriptionIdType destSignalNumber, destSignalId;
    destSignalNumber = TxIpduPtr->ComIPduDespSignalsRefNumber;
    destSignalId = TxIpduPtr->ComIPduDespSignalsRefStartId;
    const Com_GwDestSignalType* gwDestSignalPtr;
    for (; destSignalId < destSignalNumber; ++destSignalId)
    {
        gwDestSignalPtr = &Com_ConfigStd->ComDestSignal[destSignalId];
        updateBitPosition = gwDestSignalPtr->ComUpdateLsbBytePos;
        if (COM_UNUSED_SIGNALPOSITION != updateBitPosition)
        {
            Com_TxIPduRuntimeBuff[ipduBufferIndex + (Com_TxIpduBufIdType)updateBitPosition] &=
                (uint8)(~(gwDestSignalPtr->ComUpdateBitMask));
        }
    }
#endif /* STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE */
#endif
    COM_NOUSED(TxIpduPtr);
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
#if (STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && (0u < COM_TXSIGNALGROUP_NUMBER) && (0u < COM_TXIPDU_NUMBER)
/******************************************************************************/
/*
 * Brief               Send SignalGroup(Array) handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalGroupId,SignalGroupArrayPtr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint8
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
FUNC(uint8, COM_CODE)
Com_SendSignalGroupArrayHandle(
    Com_SignalGroupIdType SignalGroupId,
    P2CONST(uint8, AUTOMATIC, COM_APPL_CONST) SignalGroupArrayPtr)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    const Com_TxSignalGroupType* txSignalGroupPtr = &Com_ConfigStd->ComTxSignalGroup[SignalGroupId];
    uint8 ret = COM_SERVICE_NOT_AVAILABLE;
    boolean triggerOnChange;
    uint16 rptNum = 1u;

    /*the signal group isn't InitialValueOnly,the signal group is array*/
    if ((!txSignalGroupPtr->ComInitialValueOnly)
        && (COM_UNUSED_COM_SIGNALGROUPARRYID != txSignalGroupPtr->ComSignalGroupArrayId))
    {
        PduIdType ipduRef = txSignalGroupPtr->ComIpduRefIndex;
        const Com_TxIPduType* txIpduPtr = &Com_ConfigStd->ComTxIPdu[ipduRef];
        Com_TxIPduRunTimeStateType* txIpduStatePtr = &Com_TxIPduRunTimeState[ipduRef];
        /*the IPDU(TP) is sending*/
#if (STD_ON == COM_TXTPPDU_SUPPORT)
        uint8 Transmiting = txIpduStatePtr->TxIpduRTStFlag & Com_TX_TRANSMITING_EN;
        if ((COM_PDU_TP == txIpduPtr->ComIPduType) && (Com_TX_TRANSMITING_EN == Transmiting))
        {
            ret = COM_BUSY;
        }
        else
#endif /* STD_ON == COM_TXTPPDU_SUPPORT */
        {
#if (0u < COM_TXIPDUBUFF_SIZE)
            uint16 iPduSignalGroupArrayStartBufId =
                Com_SignalGroupArray[txSignalGroupPtr->ComSignalGroupArrayId].ComIPduStartBufIndex;
            uint16 signalGroupArrayLength =
                Com_SignalGroupArray[txSignalGroupPtr->ComSignalGroupArrayId].SignalGroupArrayLength;
            /*pack the Tx signal group(array) into ipdu Tx buffer*/
            SchM_Enter_Com_Context();
            (void)ILib_memcpy(
                &Com_TxIPduRuntimeBuff[iPduSignalGroupArrayStartBufId],
                &SignalGroupArrayPtr[0],
                signalGroupArrayLength);
            SchM_Exit_Com_Context();
#endif
            Com_TxIpduBufIdType txIPduBufferId = Com_ConfigStd->ComTxIPdu[ipduRef].ComTxIPduBufIndex;
            Com_TxGroupSignalIdType groupSignalNumber = txSignalGroupPtr->ComGroupSignalRefNumber;
#if (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE)
            /*set the update bit if the signal group configuration */
            Com_SignalPositionType updateBitPosition = txSignalGroupPtr->ComUpdateLsbBytePos;
            uint8 updateBitMask = txSignalGroupPtr->ComUpdateBitMask;
            Com_SetSignalUpdateBit(txIPduBufferId, updateBitPosition, updateBitMask);
#endif /* STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE */
            /*update every group signal buffer,calculate every TMC,return the signal group is changed or not for
             * trigger transmit*/
            triggerOnChange =
                Com_UpdateTxBufferAndCalculateTMCOfEveryGroupSignal(SignalGroupId, groupSignalNumber, txIPduBufferId);
            /*calculate the TMS,store new Tx mode,get rptNum value for DIRECT/MIXED tx mode,if TMS changed will
             * reset periodic mode counter.*/
            Com_TxIpduTMSHandle(txIpduStatePtr, txIpduPtr, &rptNum);
            if ((txIpduStatePtr->TxIpduRTStFlag & Com_TX_ACTIVE_EN) == Com_TX_ACTIVE_EN)
            {
                Com_SendSignalGroupArrayInternalHandle(
                    txSignalGroupPtr,
                    txIpduStatePtr,
                    ipduRef,
                    triggerOnChange,
                    rptNum);
                ret = E_OK;
            }
#if (COM_TXSIGGRP_ERROR_AND_NOTIFY_NUMBER > 0u)
            else
            {
                /*call Tx Err Notification*/
                if (COM_TX_SIGNAL_ERROR_NOTIFICATION_EN
                    == (txSignalGroupPtr->ComTxSignalNotificationType & COM_TX_SIGNAL_ERROR_NOTIFICATION_EN))
                {
                    Com_TxSigGrpErrAndNotifyIdType txSigGrpErrAndNotifyFncId =
                        (COM_TX_SIGNAL_ERROR_AND_NOTIFICATION == txSignalGroupPtr->ComTxSignalNotificationType)
                            ? (txSignalGroupPtr->ComTxSigGrpErrAndNotifyFncId + 1u)
                            : txSignalGroupPtr->ComTxSigGrpErrAndNotifyFncId;
                    Com_TxSignalGroupErrorAndNotification[txSigGrpErrAndNotifyFncId]();
                }
            }
#endif
        }
    }
    COM_NOUSED(SignalGroupId);
    COM_NOUSED(SignalGroupArrayPtr);
    return ret;
}
#endif

#if (COM_TXSIGNALGROUP_NUMBER > 0u) && (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
/******************************************************************************/
/*
 * Brief               Called by Com_SendDynSignal,Com_InvalidateSignalGroup.
 *                     send dynamic Tx group signal handle.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxGroupSignalId,SignalDataPtr,DynLength
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK,COM_SERVICE_NOT_AVAILABLE
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
FUNC(uint8, COM_CODE)
Com_SendTxDynGroupSignalHandle(
    /* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
    Com_SignalIdType TxGroupSignalId,
    P2CONST(void, AUTOMATIC, COM_APPL_CONST) SignalDataPtr,
    uint16 DynLength)
{
    uint8 ret;
    PduIdType ipduRef;
    Com_TransferPropertyType groupSignalTxProperty;
    boolean valueChanged;
    uint16 groupSignalBufferId;
    const Com_TxGroupSignalType* txGroupSignalPtr = &Com_ConfigStd->ComTxGroupSignal[TxGroupSignalId];
    Com_TxSignalGroupIdType signalGroupId = txGroupSignalPtr->ComSignalGroupRef;
    const Com_TxSignalGroupType* txSignalGroupPtr = &Com_ConfigStd->ComTxSignalGroup[signalGroupId];
    Com_TxSignalGroupRunTimeStateType* Com_TxSigGrpStPtr = &Com_TxSignalGroupState[signalGroupId];
    /*group signal(not dynamic) isn't send by the API,InitialValueOnly signal group don't expect to be called by any
     * API*/
    if ((COM_UINT8_DYN != txGroupSignalPtr->ComSignalType) || txSignalGroupPtr->ComInitialValueOnly)
    {
        ret = COM_SERVICE_NOT_AVAILABLE;
    }
    else
    {
        if (DynLength > txGroupSignalPtr->ComSignalLength)
        {
#if (STD_ON == COM_DEV_ERROR_DETECT)
            (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_SENDDYNSIGNAL_ID, COM_E_PARAM);
#endif
            ret = E_NOT_OK;
        }
        else
        {
            ipduRef = txSignalGroupPtr->ComIpduRefIndex;
            groupSignalBufferId = txGroupSignalPtr->ComSignalInitValueId;
            /*update Dynamic tx group signal buffer and check if the sending signal value changed*/
            valueChanged = Com_SetDynTxSignalBuff(ipduRef, groupSignalBufferId, SignalDataPtr, DynLength);
            Com_TxSigGrpStPtr->DynamicSignalLength = DynLength;
            groupSignalTxProperty =
#if (STD_ON == COM_TX_SIG_GROUP_PROP_TRIGGERED_ON_CHANGE_ENABLE)
                (COM_GROUPSIGNAL_TMC_NONE != txGroupSignalPtr->ComTransferProperty)
                    ? txGroupSignalPtr->ComTransferProperty
                    :
#endif
                    txSignalGroupPtr->ComTransferProperty;
            switch (groupSignalTxProperty)
            {
#if (STD_ON == COM_TX_SIG_GROUP_PROP_TRIGGERED_ENABLE)
            case COM_TRIGGERED:
                Com_TxSigGrpStPtr->TxSigGrpRTStFlag |= COM_NTIMESTRANSMITNEED_EN;
                Com_TxSigGrpStPtr->TxSigGrpRTStFlag &= COM_WITHOUTREPETITION_DIS;
                break;
#endif /* STD_ON == COM_TX_SIG_GROUP_PROP_TRIGGERED_ENABLE */
#if (STD_ON == COM_TX_SIG_GROUP_PROP_TRIGGERED_ON_CHANGE_ENABLE)
            case COM_TRIGGERED_ON_CHANGE:
                if (valueChanged)
                {
                    Com_TxSigGrpStPtr->TxSigGrpRTStFlag |= COM_NTIMESTRANSMITNEED_EN;
                    Com_TxSigGrpStPtr->TxSigGrpRTStFlag &= COM_WITHOUTREPETITION_DIS;
                }
                break;
#endif /* STD_ON == COM_TX_SIG_GROUP_PROP_TRIGGERED_ON_CHANGE_ENABLE */
#if (STD_ON == COM_TX_SIG_GROUP_PROP_TRIG_ON_CH_NO_REPETITION_ENABLE)
            case COM_TRIGGERED_ON_CHANGE_WITHOUT_REPETITION:
                if (valueChanged)
                {
                    Com_TxSigGrpStPtr->TxSigGrpRTStFlag |= COM_NTIMESTRANSMITNEED_EN;
                    Com_TxSigGrpStPtr->TxSigGrpRTStFlag |= COM_WITHOUTREPETITION_EN;
                }
                break;
#endif /* STD_ON == COM_TX_SIG_GROUP_PROP_TRIG_ON_CH_NO_REPETITION_ENABLE */
#if (STD_ON == COM_TX_SIG_GROUP_PROP_TRIG_NO_REPETITION_ENABLE)
            case COM_TRIGGERED_WITHOUT_REPETITION:
                Com_TxSigGrpStPtr->TxSigGrpRTStFlag |= COM_NTIMESTRANSMITNEED_EN;
                Com_TxSigGrpStPtr->TxSigGrpRTStFlag |= COM_WITHOUTREPETITION_EN;
                break;
#endif /* STD_ON == COM_TX_SIG_GROUP_PROP_TRIG_NO_REPETITION_ENABLE */
            default:
                /*do nothing*/
                break;
            }
            ret = E_OK;
        }
    }
    return ret;
}
#endif

#if (STD_ON == COM_TX_SIGNAL_NOTIFICATION_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_NOTIFICATION_ENABLE)
/******************************************************************************/
/*
 * Brief               Called by Com_TxConfirmation,Com_TpTxConfirmation,
 *                     Com_MainFunctionTx.
 *                     Tx notification for all signals/signal groups of the Tx Pdu
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxIpduPtr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, COM_CODE)
Com_ConfirmationProcess(const Com_TxIPduType* TxIpduPtr) /* PRQA S 1532,1505 */ /* MISRA Rule 8.7 */
{
#if (COM_TXSIG_ERROR_AND_NOTIFY_NUMBER > 0u)
    Com_TxSignalIdType txSignalNumber, cnt;
    const Com_TxSignalType* txSignalPtr;
    txSignalNumber = TxIpduPtr->ComIPduSignalsRefNumber;
    cnt = TxIpduPtr->ComIpduSignalRefStartId;
    for (; cnt < txSignalNumber; ++cnt)
    {
        txSignalPtr = &Com_ConfigStd->ComTxSignal[cnt];
        /*call signal group Tx Err Notification*/
        if (COM_TX_SIGNAL_NOTIFICATION_EN == (txSignalPtr->ComTxSignalNotificationType & COM_TX_SIGNAL_NOTIFICATION_EN))
        {
            Com_TxSignalErrorAndNotification[txSignalPtr->ComTxSignalErrAndNotifyFncId + 1u]();
        }
    }
#endif
#if (COM_TXSIGGRP_ERROR_AND_NOTIFY_NUMBER > 0u)
    Com_TxSignalGroupIdType signalGroupNumber, signalGroupId;
    signalGroupNumber = TxIpduPtr->ComIPduSignalGroupsRefNumber;
    signalGroupId = TxIpduPtr->ComIPduSignalGroupsRefStartId;
    const Com_TxSignalGroupType* txSignalGroupPtr;
    for (; signalGroupId < signalGroupNumber; ++signalGroupId)
    {
        txSignalGroupPtr = &Com_ConfigStd->ComTxSignalGroup[signalGroupId];
        /*call signal group Tx Err Notification*/
        if (COM_TX_SIGNAL_NOTIFICATION_EN
            == (txSignalGroupPtr->ComTxSignalNotificationType & COM_TX_SIGNAL_NOTIFICATION_EN))
        {
            Com_TxSignalGroupErrorAndNotification[txSignalGroupPtr->ComTxSigGrpErrAndNotifyFncId + 1u]();
        }
    }
#endif
    COM_NOUSED(TxIpduPtr);
    return;
}
#endif

#if (((COM_TXSIGNAL_NUMBER > 0u) || (COM_TXGROUPSIGNAL_NUMBER > 0u)) && (0u < COM_TXIPDU_NUMBER))
/******************************************************************************/
/*
 * Brief               Send Signal handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalId,SignalDataPtr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint8
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
FUNC(uint8, COM_CODE)
Com_SendSignalHandle(Com_SignalIdType SignalId, P2CONST(void, AUTOMATIC, COM_APPL_CONST) SignalDataPtr)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    uint8 ret = COM_SERVICE_NOT_AVAILABLE;
#if (0u < COM_TXSIGNAL_NUMBER)
    boolean valueChanged;
    boolean signalIsDM;
    Com_SignalType signalType;
#endif /* 0u < COM_TXSIGNAL_NUMBER */
#if (COM_TXSIGNALGROUP_NUMBER > 0u)
    Com_SignalIdType groupSignalId;
#endif /* COM_TXSIGNALGROUP_NUMBER > 0u */

    /*the signal is Tx Signal*/
    if (SignalId < COM_TXSIGNAL_NUMBER)
    {
#if (0u < COM_TXSIGNAL_NUMBER)
        const Com_TxSignalType* txSignalPtr = &Com_ConfigStd->ComTxSignal[SignalId];
        signalType = txSignalPtr->ComSignalType;
        /*dynamic signal isn't send by the API,InitialValueOnly signal don't expect to be called by any API*/
#if (STD_ON == COM_TX_SIGNAL_TYPE_UINT8_DYN_ENABLE) || (STD_ON == COM_TX_SIG_INITIAL_VALUE_ONLY_ENABLE)
        if ((COM_UINT8_DYN != signalType) && (!txSignalPtr->ComInitialValueOnly))
#endif /* STD_ON == COM_TX_SIGNAL_TYPE_UINT8_DYN_ENABLE || STD_ON == COM_TX_SIG_INITIAL_VALUE_ONLY_ENABLE */
        {
            PduIdType ipduRef = txSignalPtr->ComIpduRefIndex;
            const Com_TxIPduType* txIpduPtr = &Com_ConfigStd->ComTxIPdu[ipduRef];
            Com_TxIPduRunTimeStateType* txIpduStatePtr = &Com_TxIPduRunTimeState[ipduRef];
            /*the IPDU(TP) is sending,return busy*/
#if (STD_ON == COM_TXTPPDU_SUPPORT)
            uint8 Transmiting = txIpduStatePtr->TxIpduRTStFlag & Com_TX_TRANSMITING_EN;
            if ((COM_PDU_TP == txIpduPtr->ComIPduType) && (Com_TX_TRANSMITING_EN == Transmiting))
            {
                ret = COM_BUSY;
            }
            else
#endif /* STD_ON == COM_TXTPPDU_SUPPORT */
            {
#if (STD_ON == COM_TMS_ENABLE)
                Com_TxSignalTMHandle(txIpduStatePtr, txIpduPtr, SignalId, SignalDataPtr);
#endif /* STD_ON == COM_TMS_ENABLE */
                /*signalLength is used when the signal type is COM_UINT8_N*/
                uint16 signalLength = txSignalPtr->ComSignalLength;
                uint16 signalBufferId = txSignalPtr->ComSignalInitValueId;
                /*update tx signal buffer and check if the sending signal value changed*/
                valueChanged = Com_SetTxSignalBuff(
                    signalType,
                    signalLength,
                    signalBufferId,
                    SignalDataPtr,
                    txSignalPtr->ComBitSize);
                if (valueChanged)
                {
                    /*pack the Tx signal into ipdu Tx buffer*/
                    Com_TxSignalPack(SignalId);
                }
#if (STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE)
                /*handle update bit function. */
                Com_SignalPositionType updateBitPosition = txSignalPtr->ComUpdateLsbBytePos;
                uint8 updateBitMask = txSignalPtr->ComUpdateBitMask;
                Com_SetSignalUpdateBit(txIpduPtr->ComTxIPduBufIndex, updateBitPosition, updateBitMask);
#endif /* STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE */
                /*the Tx Pdu is enabled*/
                if ((txIpduStatePtr->TxIpduRTStFlag & Com_TX_ACTIVE_EN) == Com_TX_ACTIVE_EN)
                {
#if (STD_ON == COM_TX_SIGNAL_TIMEOUT_ENABLE)
                    if (COM_UNUSED_TXSIGNALTIMEOUTID != txSignalPtr->ComTimeoutIndex)
                    {
                        signalIsDM = TRUE;
                    }
                    else
#endif /* STD_ON == COM_TX_SIGNAL_TIMEOUT_ENABLE */
                    {
                        signalIsDM = FALSE;
                    }
                    Com_TransferPropertyType signalTxProperty = txSignalPtr->ComTransferProperty;
                    ret = Com_SendTxSignalHandle(ipduRef, signalTxProperty, signalIsDM, valueChanged);
                }
                /*the Tx Pdu is disabled*/
#if (COM_TXSIG_ERROR_AND_NOTIFY_NUMBER > 0u)
                else
                {
                    /*call Tx Err Notification*/
                    if (COM_TX_SIGNAL_ERROR_NOTIFICATION_EN
                        == (txSignalPtr->ComTxSignalNotificationType & COM_TX_SIGNAL_ERROR_NOTIFICATION_EN))
                    {
                        Com_TxSignalErrorAndNotification[txSignalPtr->ComTxSignalErrAndNotifyFncId]();
                    }
                }
#endif /* STD_ON == COM_TX_SIGNAL_ERROR_NOTIFICATION_ENABLE */
            }
        }
#endif /*0u < COM_TXSIGNAL_NUMBER*/
    }
#if (COM_TXSIGNALGROUP_NUMBER > 0u)
    /*the signal is Tx Group Signal*/
    else
    {
        groupSignalId = SignalId - COM_TXSIGNAL_NUMBER;
        ret = Com_SendTxGroupSignalHandle(groupSignalId, SignalDataPtr);
    }
#endif /* COM_TXSIGNALGROUP_NUMBER > 0u */
    COM_NOUSED(SignalId);
    COM_NOUSED(SignalDataPtr);
    return ret;
}

/******************************************************************************/
/*
 * Brief               Send Dynamic Signal handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalId,SignalDataPtr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint8
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
FUNC(uint8, COM_CODE)
Com_SendDynSignalHandle(
    Com_SignalIdType SignalId,
    P2CONST(void, AUTOMATIC, COM_APPL_CONST) SignalDataPtr,
    uint16 Length)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    uint8 ret = COM_SERVICE_NOT_AVAILABLE;
#if (0u < COM_TXSIGNAL_NUMBER)
    boolean valueChanged;
    boolean signalIsDM;
    uint16 signalBufferId;
#endif /* 0u < COM_TXSIGNAL_NUMBER */
#if (COM_TXSIGNALGROUP_NUMBER > 0u) && (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
    Com_SignalIdType groupSignalId;
#endif /* COM_TXSIGNALGROUP_NUMBER > 0u && STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE */

    /*the signal is Tx Signal*/
    if (SignalId < COM_TXSIGNAL_NUMBER)
    {
#if (0u < COM_TXSIGNAL_NUMBER) && (STD_ON == COM_TX_SIGNAL_TYPE_UINT8_DYN_ENABLE)
        const Com_TxSignalType* txSignalPtr = &Com_ConfigStd->ComTxSignal[SignalId];
        /*signal(not dynamic) isn't send by the API,InitialValueOnly signal don't expect to be called by any API*/
        if ((COM_UINT8_DYN == txSignalPtr->ComSignalType) && (!txSignalPtr->ComInitialValueOnly))
        {
            PduIdType ipduRef = txSignalPtr->ComIpduRefIndex;
            Com_TxIPduRunTimeStateType* txIpduStatePtr = &Com_TxIPduRunTimeState[ipduRef];
            const Com_TxIPduType* txIpduPtr = &Com_ConfigStd->ComTxIPdu[ipduRef];
            /*the IPDU(TP) is sending,return busy*/
#if (STD_ON == COM_TXTPPDU_SUPPORT)
            uint8 Transmiting = txIpduStatePtr->TxIpduRTStFlag & Com_TX_TRANSMITING_EN;
            if ((COM_PDU_TP == txIpduPtr->ComIPduType) && (Com_TX_TRANSMITING_EN == Transmiting))
            {
                ret = COM_BUSY;
            }
            else
#endif /* STD_ON == COM_TXTPPDU_SUPPORT */
            {
                if (Length > txSignalPtr->ComSignalLength)
                {
#if (STD_ON == COM_DEV_ERROR_DETECT)
                    (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_SENDDYNSIGNAL_ID, COM_E_PARAM);
#endif
                    ret = E_NOT_OK;
                }
                else
                {
                    signalBufferId = txSignalPtr->ComSignalInitValueId;
                    /*update tx Dyn signal buffer and check if the sending signal value changed*/
                    valueChanged = Com_SetDynTxSignalBuff(ipduRef, signalBufferId, SignalDataPtr, Length);
                    if (valueChanged)
                    {
                        Com_SignalPositionType SigLsbBytePos = txSignalPtr->ComSigLsbBytePos;
                        /*pack the Tx Dynamic signal into ipdu Tx buffer*/
                        Com_TxDynSignalPack(ipduRef, SigLsbBytePos, signalBufferId, Length);
                        txIpduStatePtr->TxIpduLength = (uint16)txIpduPtr->IPduNoDynSignalLength + Length;
                    }
#if (STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE)
                    /*handle update bit function. */
                    Com_SignalPositionType updateBitPosition = txSignalPtr->ComUpdateLsbBytePos;
                    uint8 updateBitMask = txSignalPtr->ComUpdateBitMask;
                    Com_SetSignalUpdateBit(txIpduPtr->ComTxIPduBufIndex, updateBitPosition, updateBitMask);
#endif /* STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE */
                    /*the Tx Pdu is enabled*/
                    if ((txIpduStatePtr->TxIpduRTStFlag & Com_TX_ACTIVE_EN) == Com_TX_ACTIVE_EN)
                    {
#if (STD_ON == COM_TX_SIGNAL_TIMEOUT_ENABLE)
                        if (COM_UNUSED_TXSIGNALTIMEOUTID != txSignalPtr->ComTimeoutIndex)
                        {
                            signalIsDM = TRUE;
                        }
                        else
#endif /* STD_ON == COM_TX_SIGNAL_TIMEOUT_ENABLE */
                        {
                            signalIsDM = FALSE;
                        }
                        Com_TransferPropertyType signalTxProperty = txSignalPtr->ComTransferProperty;
                        ret = Com_SendTxSignalHandle(ipduRef, signalTxProperty, signalIsDM, valueChanged);
                    }
                    /*the Tx Pdu is disabled*/
#if (COM_TXSIG_ERROR_AND_NOTIFY_NUMBER > 0u)
                    else
                    {
                        /*call Tx Err Notification*/
                        if (COM_TX_SIGNAL_ERROR_NOTIFICATION_EN
                            == (txSignalPtr->ComTxSignalNotificationType & COM_TX_SIGNAL_ERROR_NOTIFICATION_EN))
                        {
                            Com_TxSignalErrorAndNotification[txSignalPtr->ComTxSignalErrAndNotifyFncId]();
                        }
                    }
#endif /* STD_ON == COM_TX_SIGNAL_ERROR_NOTIFICATION_ENABLE */
                }
            }
        }
#endif /* 0u < COM_TXSIGNAL_NUMBER && STD_ON == COM_TX_SIGNAL_TYPE_UINT8_DYN_ENABLE */
    }
#if (COM_TXSIGNALGROUP_NUMBER > 0u) && (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
    /*the signal is Tx Group Signal*/
    else
    {
        groupSignalId = SignalId - COM_TXSIGNAL_NUMBER;
        ret = Com_SendTxDynGroupSignalHandle(groupSignalId, SignalDataPtr, Length);
    }
#endif
    COM_NOUSED(Length);
    return ret;
}
#endif

#if ((0u < COM_TXSIGNALGROUP_NUMBER) && (0u < COM_TXIPDU_NUMBER))
/******************************************************************************/
/*
 * Brief               Send SignalGroup handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalGroupId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint8
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
FUNC(uint8, COM_CODE)
Com_SendSignalGroupHandle(Com_SignalGroupIdType SignalGroupId)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    uint8 ret = COM_SERVICE_NOT_AVAILABLE;
    PduIdType ipduRef;
    uint16 rptNum = 1u;
#if (STD_ON == COM_TX_SIG_GROUP_TIMEOUT_ENABLE)
    boolean initDMCnt = FALSE;
#endif
#if (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE)
    Com_SignalPositionType updateBitPosition;
    uint8 updateBitMask;
#endif /* STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE */
    Com_TxModeModeType ipduTxMode;
    const Com_TxSignalGroupType* txSignalGroupPtr = &Com_ConfigStd->ComTxSignalGroup[SignalGroupId];
    const Com_TxIPduType* txIpduPtr;
    Com_TxIPduRunTimeStateType* txIpduStatePtr;
    Com_TxSignalGroupRunTimeStateType* txSigGrpStPtr = &Com_TxSignalGroupState[SignalGroupId];

    /*the signal group is configuration ComInitialValueOnly,don't expect to be called by any API*/
#if (STD_ON == COM_TX_SIG_GROUP_INITIAL_VALUE_ONLY_ENABLE)
    if (!txSignalGroupPtr->ComInitialValueOnly)
#endif /* STD_ON == COM_TX_SIG_GROUP_INITIAL_VALUE_ONLY_ENABLE */
    {
        ipduRef = txSignalGroupPtr->ComIpduRefIndex;
        txIpduPtr = &Com_ConfigStd->ComTxIPdu[ipduRef];
        txIpduStatePtr = &Com_TxIPduRunTimeState[ipduRef];
        /*the IPDU(TP) is sending*/
#if (STD_ON == COM_TXTPPDU_SUPPORT)
        uint8 Transmiting = txIpduStatePtr->TxIpduRTStFlag & Com_TX_TRANSMITING_EN;
        if ((COM_PDU_TP == txIpduPtr->ComIPduType) && (Com_TX_TRANSMITING_EN == Transmiting))
        {
            ret = COM_BUSY;
        }
        else
#endif /* STD_ON == COM_TXTPPDU_SUPPORT */
        {
            /*pack the Tx signal group into ipdu Tx buffer,copy Com_TxGroupSignalTMCBuffer to
             * Com_TxSignalTMCRunTime*/
            Com_TxSignalGroupPack(SignalGroupId);
            /*update the Tx Pdu length(if the signal group include dynamic group signal)*/
            txIpduStatePtr->TxIpduLength =
                ((uint16)txIpduPtr->IPduNoDynSignalLength + txSigGrpStPtr->DynamicSignalLength);
#if (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE)
            /*handle update bit function. */
            updateBitPosition = txSignalGroupPtr->ComUpdateLsbBytePos;
            updateBitMask = txSignalGroupPtr->ComUpdateBitMask;
            Com_SetSignalUpdateBit(txIpduPtr->ComTxIPduBufIndex, updateBitPosition, updateBitMask);
#endif /* STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE */
            /*calculate the TMS,store new Tx mode,get rptNum value for DIRECT/MIXED tx mode,if TMS changed will
             * reset periodic mode counter.*/
            Com_TxIpduTMSHandle(txIpduStatePtr, txIpduPtr, &rptNum);
            /*the Tx Pdu is enabled*/
            if ((txIpduStatePtr->TxIpduRTStFlag & Com_TX_ACTIVE_EN) == Com_TX_ACTIVE_EN)
            {
                /*the signal group configuration timeout,the DM timer isn't start by other signals/signal groups,Tx
                 * Mode isn't NONE*/
                ipduTxMode = txIpduStatePtr->ipduTxMode;
#if (STD_ON == COM_TX_SIG_GROUP_TIMEOUT_ENABLE)
                if ((COM_UNUSED_TXSIGGRPTIMEOUTID != txSignalGroupPtr->ComTimeoutIndex)
                    && (COM_TX_MODE_NONE != ipduTxMode) && (0u == txIpduStatePtr->DMCnt))
                {
                    /*restart the tx DM timer*/
                    txIpduStatePtr->DMCnt = txIpduPtr->ComTxIpduDM;
                    initDMCnt = TRUE;
                }
#endif

#if (0u < COM_TX_MODE_TRUE_DIRECT_NUMBER) || (0u < COM_TX_MODE_TRUE_MIXED_NUMBER)                              \
    || (0u < COM_TX_MODE_FALSE_DIRECT_NUMBER) || (0u < COM_TX_MODE_FALSE_MIXED_NUMBER)                         \
    || (0u < COM_TX_MODE_TRUE_DIRECT_NOREPETITION_NUMBER) || (0u < COM_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER) \
    || (0u < COM_TX_MODE_FALSE_DIRECT_NOREPETITION_NUMBER) || (0u < COM_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER)
                if ((COM_TX_MODE_DIRECT == ipduTxMode) || (COM_TX_MODE_MIXED == ipduTxMode)
                    || (COM_TX_MODE_DIRECT_WITHOUT_REPETITION == ipduTxMode)
                    || (COM_TX_MODE_MIXED_WITHOUT_REPETITION == ipduTxMode))
                {
                    uint8 NTimesTransmitNeed = txSigGrpStPtr->TxSigGrpRTStFlag & COM_NTIMESTRANSMITNEED_EN;
                    uint8 WithoutRepetition = txSigGrpStPtr->TxSigGrpRTStFlag & COM_WITHOUTREPETITION_EN;
                    if (COM_NTIMESTRANSMITNEED_EN == NTimesTransmitNeed)
                    {
                        txIpduStatePtr->NTimeCnt = 0u;
                        txIpduStatePtr->RptNum = (COM_WITHOUTREPETITION_EN == WithoutRepetition) ? 1u : rptNum;
                        /*For an I-PDU with ComTxModeMode DIRECT and ComTxModeNumberOfRepetitions > 0, the AUTOSAR
                          COM module shall reset an already running transmission deadline monitoring timer in case
                          another send request for this I-PDU is initiated*/
#if (STD_ON == COM_TX_SIG_GROUP_TIMEOUT_ENABLE)
                        if ((1u < txIpduStatePtr->RptNum) && (!initDMCnt)
                            && (COM_UNUSED_TXSIGGRPTIMEOUTID != txSignalGroupPtr->ComTimeoutIndex))
                        {
                            txIpduStatePtr->DMCnt = txIpduPtr->ComTxIpduDM;
                        }
#endif
                        txSigGrpStPtr->TxSigGrpRTStFlag &= COM_NTIMESTRANSMITNEED_DIS;
                    }
                }
                else
#endif
                {
                    txIpduStatePtr->NTimeCnt = 0u;
                    txIpduStatePtr->RptNum = 0u;
                }
                ret = E_OK;
            }
#if (COM_TXSIGGRP_ERROR_AND_NOTIFY_NUMBER > 0u)
            else
            {
                /*call Tx Err Notification*/
                if (COM_TX_SIGNAL_ERROR_NOTIFICATION_EN
                    == (txSignalGroupPtr->ComTxSignalNotificationType & COM_TX_SIGNAL_ERROR_NOTIFICATION_EN))
                {
                    Com_TxSigGrpErrAndNotifyIdType txSigGrpErrAndNotifyFncId =
                        (COM_TX_SIGNAL_ERROR_AND_NOTIFICATION == txSignalGroupPtr->ComTxSignalNotificationType)
                            ? (txSignalGroupPtr->ComTxSigGrpErrAndNotifyFncId + 1u)
                            : txSignalGroupPtr->ComTxSigGrpErrAndNotifyFncId;
                    Com_TxSignalGroupErrorAndNotification[txSigGrpErrAndNotifyFncId]();
                }
            }
#endif
        }
    }
    return ret;
}
#endif

#if ((COM_TXSIGNAL_NUMBER > 0u) || (COM_TXGROUPSIGNAL_NUMBER > 0u)) && (0u < COM_TXIPDU_NUMBER)
/******************************************************************************/
/*
 * Brief               Send Invalidate Signal handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint8
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
FUNC(uint8, COM_CODE)
Com_InvalidateSignalHandle(Com_SignalIdType SignalId)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    uint8 ret = COM_SERVICE_NOT_AVAILABLE;
#if (STD_ON == COM_TX_SIGNAL_INVALID_DATA_ENABLE)
    const Com_TxSignalType* txSignalPtr = &Com_ConfigStd->ComTxSignal[SignalId];
    Com_SignalType signalType = txSignalPtr->ComSignalType;
    Com_TxSignalInvalidIdType signalInvalidId = txSignalPtr->ComSignalInvalidRefId;
#if (0u < COM_TX_SIGNAL_8BIT_INVALIDVALUE_NUMBER) && (0u < COM_SIGNAL_8BIT_INVALID_SIZE)
    uint16 invalidValueLength;
#endif

    if ((COM_UNUSED_TXSIGNALINVALIDID != signalInvalidId) && (TRUE != txSignalPtr->ComInitialValueOnly))
    {
        uint16 invalidValueId = Com_TxSignalDataInvalidValueIdRef[signalInvalidId];
#if (STD_ON == COM_TX_SIGNAL_TYPE_UINT8_DYN_ENABLE)
        if (COM_UINT8_DYN == signalType)
        {
#if (0u < COM_TX_SIGNAL_8BIT_INVALIDVALUE_NUMBER) && (0u < COM_SIGNAL_8BIT_INVALID_SIZE)
            invalidValueLength = Com_TxSignalDataInvalidValueLengthRef[signalInvalidId];
            /*send signal use configuration invalidate value */
            ret = Com_SendDynSignalHandle(SignalId, &(Com_Signal8BitInvalidValue[invalidValueId]), invalidValueLength);
#endif
        }
        else
#endif /* STD_ON == COM_TX_SIGNAL_TYPE_UINT8_DYN_ENABLE */
        {
            /* PRQA S 3315 ++ */ /* MISRA Rule 16.6 */
            switch (signalType)
            {
/*send signal use configuration invalidate value */
#if (0u < COM_SIGNAL_BOOL_INVALID_SIZE)
            case COM_BOOLEAN:
                ret = Com_SendSignal(SignalId, &(Com_SignalBoolInvalidValue[invalidValueId]));
                break;
#endif
#if (0u < COM_SIGNAL_8BIT_INVALID_SIZE)
            case COM_SINT8:
            case COM_UINT8:
            case COM_UINT8_N:
                ret = Com_SendSignal(SignalId, &(Com_Signal8BitInvalidValue[invalidValueId]));
                break;
#endif
#if (0u < COM_SIGNAL_16BIT_INVALID_SIZE)
            case COM_SINT16:
            case COM_UINT16:
                ret = Com_SendSignal(SignalId, &(Com_Signal16BitInvalidValue[invalidValueId]));
                break;
#endif
#if (0u < COM_SIGNAL_32BIT_INVALID_SIZE)
            case COM_SINT32:
            case COM_UINT32:
            case COM_FLOAT32:
                ret = Com_SendSignal(SignalId, &(Com_Signal32BitInvalidValue[invalidValueId]));
                break;
#endif
#if (0u < COM_SIGNAL_64BIT_INVALID_SIZE)
            case COM_SINT64:
            case COM_UINT64:
            case COM_FLOAT64:
                ret = Com_SendSignal(SignalId, &(Com_Signal64BitInvalidValue[invalidValueId]));
                break;
#endif
            default:
                /*do nothing*/
                break;
            }
            /* PRQA S 3315 -- */ /* MISRA Rule 16.6 */
        }
    }
#endif
    COM_NOUSED(SignalId);
    return ret;
}
#endif
#if ((COM_TXSIGNAL_NUMBER > 0u) || (COM_TXGROUPSIGNAL_NUMBER > 0u)) && (0u < COM_TXIPDU_NUMBER)
/******************************************************************************/
/*
 * Brief               Send Invalidate SignalGroup handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalGroupId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint8
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
FUNC(uint8, COM_CODE)
Com_InvalidateSignalGroupHandle(Com_SignalGroupIdType SignalGroupId)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    uint8 ret;
    PduIdType ipduRef;
    const Com_TxSignalGroupType* txSignalGroupPtr = &Com_ConfigStd->ComTxSignalGroup[SignalGroupId];
#if (0u < COM_SIGNAL_8BIT_INVALID_SIZE) && (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
    uint16 invalidValueLength;
#endif /* 0u < COM_SIGNAL_8BIT_INVALID_SIZE && STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE */

    /*the signal group is InitialValueOnly,don't expect to be called by any API*/
#if (STD_ON == COM_TX_SIG_GROUP_INITIAL_VALUE_ONLY_ENABLE)
    ret = COM_SERVICE_NOT_AVAILABLE;
    if (TRUE != txSignalGroupPtr->ComInitialValueOnly)
#endif /* STD_ON == COM_TX_SIG_GROUP_INITIAL_VALUE_ONLY_ENABLE */
    {
        ipduRef = txSignalGroupPtr->ComIpduRefIndex;
        /*the IPDU(TP) is sending,return busy*/
#if (STD_ON == COM_TXTPPDU_SUPPORT)
        uint8 Transmiting = Com_TxIPduRunTimeState[ipduRef].TxIpduRTStFlag & Com_TX_TRANSMITING_EN;
        if ((COM_PDU_TP == Com_ConfigStd->ComTxIPdu[ipduRef].ComIPduType) && (Com_TX_TRANSMITING_EN == Transmiting))
        {
            ret = COM_BUSY;
        }
        else
#endif /* STD_ON == COM_TXTPPDU_SUPPORT */
        {
#if (STD_ON == COM_TX_GRP_SIGNAL_INVALID_DATA_ENABLE)
            Com_TxGroupSignalIdType groupSignalNumber, groupSignalId;
            groupSignalNumber = txSignalGroupPtr->ComGroupSignalRefNumber;
            groupSignalId = txSignalGroupPtr->ComGroupSignalRefStartId;
            for (; groupSignalId < groupSignalNumber; ++groupSignalId)
            {
                const Com_TxGroupSignalType* txGroupSignalPtr = &Com_ConfigStd->ComTxGroupSignal[groupSignalId];
                Com_SignalType signalType = txGroupSignalPtr->ComSignalType;
                uint16 invalidValueId = txGroupSignalPtr->ComSignalInvalidBuffRefId;
                if (COM_UNUSED_UINT16 != invalidValueId)
                {
#if (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
                    if (COM_UINT8_DYN == signalType)
                    {
#if (0u < COM_SIGNAL_8BIT_INVALID_SIZE)
                        invalidValueLength = txGroupSignalPtr->ComSignalDataInvalidValueLength;
                        /*send dynamic group signal use configuration invalidate value */
                        (void)Com_SendTxDynGroupSignalHandle(
                            groupSignalId,
                            &(Com_Signal8BitInvalidValue[invalidValueId]),
                            invalidValueLength);
#endif /* 0u < COM_SIGNAL_8BIT_INVALID_SIZE */
                    }
                    else
#endif /* STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE */
                    {
                        /* PRQA S 3315 ++ */ /* MISRA Rule 16.6 */
                        switch (signalType)
                        {
/*send group signal use configuration invalidate value */
#if (0u < COM_SIGNAL_BOOL_INVALID_SIZE)
                        case COM_BOOLEAN:
                            (void)Com_SendTxGroupSignalHandle(
                                groupSignalId,
                                &(Com_SignalBoolInvalidValue[invalidValueId]));
                            break;
#endif /* 0u < COM_SIGNAL_BOOL_INVALID_SIZE */
#if (0u < COM_SIGNAL_8BIT_INVALID_SIZE)
                        case COM_SINT8:
                        case COM_UINT8:
                        case COM_UINT8_N:
                            (void)Com_SendTxGroupSignalHandle(
                                groupSignalId,
                                &(Com_Signal8BitInvalidValue[invalidValueId]));
                            break;
#endif /* 0u < COM_SIGNAL_8BIT_INVALID_SIZE */
#if (0u < COM_SIGNAL_16BIT_INVALID_SIZE)
                        case COM_SINT16:
                        case COM_UINT16:
                            (void)Com_SendTxGroupSignalHandle(
                                groupSignalId,
                                &(Com_Signal16BitInvalidValue[invalidValueId]));
                            break;
#endif /* 0u < COM_SIGNAL_16BIT_INVALID_SIZE */
#if (0u < COM_SIGNAL_32BIT_INVALID_SIZE)
                        case COM_SINT32:
                        case COM_UINT32:
                        case COM_FLOAT32:
                            (void)Com_SendTxGroupSignalHandle(
                                groupSignalId,
                                &(Com_Signal32BitInvalidValue[invalidValueId]));
                            break;
#endif /* 0u < COM_SIGNAL_32BIT_INVALID_SIZE */
#if (0u < COM_SIGNAL_64BIT_INVALID_SIZE)
                        case COM_SINT64:
                        case COM_UINT64:
                        case COM_FLOAT64:
                            (void)Com_SendTxGroupSignalHandle(
                                groupSignalId,
                                &(Com_Signal64BitInvalidValue[invalidValueId]));
                            break;
#endif /* 0u < COM_SIGNAL_64BIT_INVALID_SIZE */
                        default:
                            /*do nothing*/
                            break;
                        }
                        /* PRQA S 3315 -- */ /* MISRA Rule 16.6 */
                    }
                }
            }
#endif
            ret = Com_SendSignalGroup(SignalGroupId);
        }
    }
    return ret;
}
#endif

#if (0u < COM_TXIPDU_NUMBER)
/******************************************************************************/
/*
 * Brief               Trigger IPDU send handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      PduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
FUNC(Std_ReturnType, COM_CODE)
Com_TriggerIPDUSendHandle(PduIdType PduId)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    Std_ReturnType returnValue = E_NOT_OK;
    PduIdType txIpduId = PduId - COM_RXIPDU_NUMBER;
    Com_TxIPduRunTimeStateType* txIpduStatePtr = &Com_TxIPduRunTimeState[txIpduId];

    /*In case a stopped I-PDU is triggered for transmission, just E_NOT_OK is returned.*/
    uint8 activeEnable = txIpduStatePtr->TxIpduRTStFlag & Com_TX_ACTIVE_EN;
    if (Com_TX_ACTIVE_EN == activeEnable)
    {
        uint8 Transmiting = txIpduStatePtr->TxIpduRTStFlag & Com_TX_TRANSMITING_EN;
        /*the transmit is delay by MDT or the pdu is transmitting*/
        if (
#if (STD_ON == COM_MDT_ENABLE)
            (0u < txIpduStatePtr->MDTCnt) ||
#endif /* STD_ON == COM_MDT_ENABLE */
            (Com_TX_TRANSMITING_EN == Transmiting))
        {
            txIpduStatePtr->TxIpduRTStFlag |= Com_TX_DELAY_EN;
            returnValue = E_OK;
        }
        else
        {
            returnValue = Com_TriggerIPDUSendInternalHandle(txIpduId);
        }
    }
    else
    {
#if (STD_ON == COM_TX_SIGNAL_ERROR_NOTIFICATION_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_ERROR_NOTIFICATION_ENABLE)
        Com_InvokeErrorNotification(txIpduId);
#endif
    }
    return returnValue;
}
#endif

#if (STD_ON == COM_METADATA_SUPPORT) && (0u < COM_TXIPDU_NUMBER)
/******************************************************************************/
/*
 * Brief               Trigger IPDU(with metadata) send handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      PduId,MetaData
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 */
/******************************************************************************/
/* PRQA S 1532,3432 ++ */ /* MISRA Rule 8.7,20.7 */
FUNC(Std_ReturnType, COM_CODE)
Com_TriggerIPDUSendWithMetaDataHandle(PduIdType PduId, P2CONST(uint8, AUTOMATIC, COM_APPL_CONST) MetaData)
/* PRQA S 1532,3432 -- */ /* MISRA Rule 8.7,20.7 */
{
    Std_ReturnType returnValue = E_NOT_OK;
    PduIdType txIpduId = PduId - COM_RXIPDU_NUMBER;
    Com_TxIpduBufIdType ipduBufferId;
    uint16 metaDataStartId;
    uint8 metaDataLength;
    const Com_TxIPduType* txIpduPtr;
    Com_TxIPduRunTimeStateType* txIpduStatePtr = &Com_TxIPduRunTimeState[txIpduId];
    uint8 Transmiting = txIpduStatePtr->TxIpduRTStFlag & Com_TX_TRANSMITING_EN;
    uint8 activeEnable = txIpduStatePtr->TxIpduRTStFlag & Com_TX_ACTIVE_EN;

    /*In case a stopped I-PDU is triggered for transmission, just E_NOT_OK is returned.*/
    if (Com_TX_ACTIVE_EN == activeEnable)
    {
        txIpduPtr = &Com_ConfigStd->ComTxIPdu[txIpduId];
        ipduBufferId = txIpduPtr->ComTxIPduBufIndex;
        metaDataLength = txIpduPtr->MetaDataLength;
        metaDataStartId =
            (uint16)txIpduPtr->IPduNoDynSignalLength + (uint16)txIpduPtr->IPduMaxDynSignalLength + (uint16)ipduBufferId;
        SchM_Enter_Com_Context();
        /*update the metadata of the tx ipdu*/
        (void)ILib_memcpy(&Com_TxIPduRuntimeBuff[metaDataStartId], &MetaData[0], metaDataLength);
        SchM_Exit_Com_Context();
        /*the transmit is delay by MDT or the pdu is transmitting*/
        if (
#if (STD_ON == COM_MDT_ENABLE)
            (0u < Com_TxIPduRunTimeState[txIpduId].MDTCnt) ||
#endif /* STD_ON == COM_MDT_ENABLE */
            (Com_TX_TRANSMITING_EN == Transmiting))
        {
            txIpduStatePtr->TxIpduRTStFlag |= Com_TX_DELAY_EN;
            returnValue = E_OK;
        }
        else
        {
            returnValue = Com_TriggerIPDUSendInternalHandle(txIpduId);
        }
    }
    else
    {
#if (STD_ON == COM_TX_SIGNAL_ERROR_NOTIFICATION_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_ERROR_NOTIFICATION_ENABLE)
        Com_InvokeErrorNotification(txIpduId);
#endif /* STD_ON == COM_TX_SIGNAL_ERROR_NOTIFICATION_ENABLE || STD_ON == \
          COM_TX_SIG_GROUP_ERROR_NOTIFICATION_ENABLE */
    }
    return returnValue;
}
#endif
#if (0u < COM_TXIPDU_NUMBER) && (STD_ON == COM_TMS_ENABLE)
/******************************************************************************/
/*
 * Brief               Switch Ipdu Transmit Mode Handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      PduId,Mode
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
FUNC(void, COM_CODE)
Com_SwitchIpduTxModeHandle(PduIdType PduId, boolean Mode)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    PduIdType txIpduId = PduId - COM_RXIPDU_NUMBER;
    const Com_TxIPduType* txIpduPtr = &Com_ConfigStd->ComTxIPdu[txIpduId];
    Com_TxIPduRunTimeStateType* txIpduStatePtr = &Com_TxIPduRunTimeState[txIpduId];
    uint8 TMS;
    TMS = txIpduStatePtr->TxIpduRTStFlag & Com_TX_TMS_EN;
    if (Mode)
    {
        if (Com_TX_TMS_EN != TMS)
        {
            txIpduStatePtr->TxIpduRTStFlag |= Com_TX_TMS_EN;
            txIpduStatePtr->ipduTxMode = txIpduPtr->ComTxTrueModeMode;
            txIpduStatePtr->NTimeCnt = 0u;
            txIpduStatePtr->RptNum = 0u;
        }
    }
    else
    {
        if (Com_TX_TMS_EN == TMS)
        {
            txIpduStatePtr->TxIpduRTStFlag &= Com_TX_TMS_DIS;
            txIpduStatePtr->ipduTxMode = txIpduPtr->ComTxFalseModeMode;
            txIpduStatePtr->NTimeCnt = 0u;
            txIpduStatePtr->RptNum = 0u;
        }
    }

    if ((COM_UNUSED_UINT16 != txIpduPtr->ComTxModeFalseRefId)
        && ((COM_TX_MODE_MIXED == txIpduStatePtr->ipduTxMode) || (COM_TX_MODE_PERIODIC == txIpduStatePtr->ipduTxMode)
            || (COM_TX_MODE_MIXED_WITHOUT_REPETITION == txIpduStatePtr->ipduTxMode)))
    {
        if ((Mode && (Com_TX_TMS_EN != TMS)) || (!Mode && (Com_TX_TMS_EN == TMS)))
        {
            Com_GetTxModeOffset(txIpduStatePtr, txIpduPtr, Mode);
        }
    }
}
#endif

#if (0u < COM_TXIPDU_NUMBER)
/******************************************************************************/
/*
 * Brief               TxPdu MainFunction handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, COM_CODE)
Com_MainFunctionTxHandle(Com_MainFunctionType mainFunctionId) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    PduIdType txIpduId;
    const Com_TxIPduType* txIpduPtr;
    Com_TxIPduRunTimeStateType* txIpduStatePtr;
    uint8 activeEnable;
    uint8 Transmiting;

    for (txIpduId = Com_GetStartOfMainFunctionTx(mainFunctionId); txIpduId < Com_GetEndOfMainFunctionTx(mainFunctionId);
         ++txIpduId)
    {
        txIpduStatePtr = &Com_TxIPduRunTimeState[txIpduId];
        activeEnable = txIpduStatePtr->TxIpduRTStFlag & Com_TX_ACTIVE_EN;
        if (Com_TX_ACTIVE_EN == activeEnable)
        {
            txIpduPtr = &Com_ConfigStd->ComTxIPdu[txIpduId];
            boolean ipduMDTRun = FALSE;
            boolean txSendFlag = FALSE;
            /*Tx notification for all signals/signal groups of the Tx Pdu is DEFERRED in the MainFunctionTx*/
#if (STD_ON == COM_TX_SIGNAL_NOTIFICATION_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_NOTIFICATION_ENABLE)
            uint8 TxConfirm = txIpduStatePtr->TxIpduRTStFlag & Com_TX_CONFIRM_EN;
            if (Com_TX_CONFIRM_EN == TxConfirm)
            {
                /*Tx notification for all signals/signal groups of the Tx Pdu*/
                Com_ConfirmationProcess(txIpduPtr);
                txIpduStatePtr->TxIpduRTStFlag &= Com_TX_CONFIRM_DIS;
            }
#endif      /* STD_ON == COM_TX_SIGNAL_NOTIFICATION_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_NOTIFICATION_ENABLE */
            /* Calculate DM, if DM > 0u, DM-- every mainfunction */
#if (STD_ON == COM_TX_SIGNAL_TIMEOUT_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_TIMEOUT_ENABLE)
            if (txIpduStatePtr->DMCnt > 0u)
            {
                (txIpduStatePtr->DMCnt) -= 1u;
                /* DM_TimeOut */
                if (0u == txIpduStatePtr->DMCnt)
                {
#if (STD_ON == COM_TX_SIGNAL_TIMEOUT_NOTIFICATION_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_TIMEOUT_NOTIFICATION_ENABLE)
                    /* ToutNotification*/
                    Com_TxDMTimeOutNotification(txIpduPtr);
#endif /* STD_ON == COM_TX_SIGNAL_TIMEOUT_NOTIFICATION_ENABLE || STD_ON == \
          COM_TX_SIG_GROUP_TIMEOUT_NOTIFICATION_ENABLE */
                    /* CancelTx(Direct/N component */
                    txIpduStatePtr->RptNum = 0u;
                    txIpduStatePtr->NTimeCnt = 0u;
                    txIpduStatePtr->TxIpduRTStFlag &= Com_TX_TRANSMITING_DIS;
/*cancel the transmitting tx pdu*/
#if (STD_ON == COM_CANCELLATION_SUPPORT)
                    if (txIpduPtr->ComIPduCancellationSupport)
                    {
                        (void)PduR_ComCancelTransmit(txIpduPtr->ComPduIdRef);
                    }
#endif
                }
            }
#endif /* STD_ON == COM_TX_SIGNAL_TIMEOUT_ENABLE || STD_ON == COM_TX_SIG_GROUP_TIMEOUT_ENABLE */
#if (STD_ON == COM_MDT_ENABLE)
            /* Calculate MDT, if MDT > 0u, MDT-- every mainfunction  */
            if (txIpduStatePtr->MDTCnt > 0u)
            {
                (txIpduStatePtr->MDTCnt) -= 1u;
                /* MDT Timer > 0u, set MDT running flag */
                if (txIpduStatePtr->MDTCnt != 0u)
                {
                    ipduMDTRun = TRUE;
                }
            }
#endif /* STD_ON == COM_MDT_ENABLE */
            switch (txIpduStatePtr->ipduTxMode)
            {
#if (0u < COM_TX_MODE_TRUE_DIRECT_NUMBER) || (0u < COM_TX_MODE_FALSE_DIRECT_NUMBER) \
    || (0u < COM_TX_MODE_TRUE_DIRECT_NOREPETITION_NUMBER) || (0u < COM_TX_MODE_FALSE_DIRECT_NOREPETITION_NUMBER)
            case COM_TX_MODE_DIRECT:
            case COM_TX_MODE_DIRECT_WITHOUT_REPETITION:
                txSendFlag = Com_MainFuncTxDirect(txIpduStatePtr, txIpduPtr, ipduMDTRun);
                break;
#endif
#if (0u < COM_TX_MODE_TRUE_MIXED_NUMBER) || (0u < COM_TX_MODE_FALSE_MIXED_NUMBER) \
    || (0u < COM_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER) || (0u < COM_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER)
            case COM_TX_MODE_MIXED:
            case COM_TX_MODE_MIXED_WITHOUT_REPETITION:
                txSendFlag = Com_MainFuncTxMixed(txIpduStatePtr, txIpduPtr, ipduMDTRun);
                break;
#endif
#if (0u < COM_TX_MODE_TRUE_PERIOD_NUMBER) || (0u < COM_TX_MODE_FALSE_PERIOD_NUMBER)  \
    || (0u < COM_TX_MODE_TRUE_MIXED_NUMBER) || (0u < COM_TX_MODE_FALSE_MIXED_NUMBER) \
    || (0u < COM_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER) || (0u < COM_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER)
            case COM_TX_MODE_PERIODIC:
                txSendFlag = Com_MainFuncTxPeriod(txIpduStatePtr, txIpduPtr, ipduMDTRun);
                break;
#endif
            default:
                /*do nothing*/
                break;
            }
            if (txSendFlag)
            {
#if (STD_ON == COM_TXTPPDU_SUPPORT)
                Transmiting = txIpduStatePtr->TxIpduRTStFlag & Com_TX_TRANSMITING_EN;
                if ((COM_PDU_TP == txIpduPtr->ComIPduType) && (Com_TX_TRANSMITING_EN == Transmiting))
                {
                    /* in case a large I-PDU is currently transmitted and the same I-PDU is triggered for
                     * transmission again*/
                    (void)Det_ReportRuntimeError(
                        COM_MODULE_ID,
                        COM_INSTANCE_ID,
                        COM_MAINFUNCTIONTX_ID,
                        COM_E_SKIPPED_TRANSMISSION);
                }
                /*this MainFunctionTx need send the tx ipdu */
                else
#endif /* STD_ON == COM_TXTPPDU_SUPPORT */
                {
                    Com_MainFunction_SendPdu(txIpduId);
                }
            }
        }
    }
}
#endif
#if (0u < COM_TXIPDU_NUMBER)
/******************************************************************************/
/*
 * Brief               TxPdu Trigger Transmit handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxPduId
 * Param-Name[out]     None
 * Param-Name[in/out]  PduInfoPtr
 * Return              None
 */
/******************************************************************************/
/* PRQA S 1532,3432 ++ */ /* MISRA Rule 8.7,20.7 */
FUNC(Std_ReturnType, COM_CODE)
Com_TriggerTransmitHandle(PduIdType TxPduId, P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr)
/* PRQA S 1532,3432 -- */ /* MISRA Rule 8.7,20.7 */
{
    Std_ReturnType returnValue = E_NOT_OK;
    PduIdType txIpduId = TxPduId - COM_RXIPDU_NUMBER;
    Com_TxIPduRunTimeStateType* txIpduStatePtr = &Com_TxIPduRunTimeState[txIpduId];
    uint16 txIPduLength = txIpduStatePtr->TxIpduLength;
    const Com_TxIPduType* txIpduPtr = &Com_ConfigStd->ComTxIPdu[txIpduId];
    Com_TxIpduBufIdType ipduBufferId = txIpduPtr->ComTxIPduBufIndex;
    uint8 activeEnable = txIpduStatePtr->TxIpduRTStFlag & Com_TX_ACTIVE_EN;
#if (STD_ON == COM_METADATA_SUPPORT)
    uint8 metaDataLength;
    uint16 metaDataStartId;
#endif /*STD_ON == COM_METADATA_SUPPORT*/

    if (Com_TX_ACTIVE_EN == activeEnable)
    {
        if (txIPduLength <= PduInfoPtr->SduLength)
        {
            SchM_Enter_Com_Context();
            (void)ILib_memcpy(&PduInfoPtr->SduDataPtr[0], &Com_TxIPduRuntimeBuff[ipduBufferId], txIPduLength);
            SchM_Exit_Com_Context();
            PduInfoPtr->SduLength = txIPduLength;
#if (STD_ON == COM_METADATA_SUPPORT)
            metaDataLength = txIpduPtr->MetaDataLength;
            if (0u < metaDataLength)
            {
                metaDataStartId = (uint16)txIpduPtr->IPduNoDynSignalLength + (uint16)txIpduPtr->IPduMaxDynSignalLength
                                  + (uint16)ipduBufferId;
                SchM_Enter_Com_Context();
                (void)ILib_memcpy(&PduInfoPtr->MetaDataPtr[0], &Com_TxIPduRuntimeBuff[metaDataStartId], metaDataLength);
                SchM_Exit_Com_Context();
            }
            else
#endif
            {
                PduInfoPtr->MetaDataPtr = NULL_PTR;
            }
#if (COM_TXIPDU_TIGGERTRANSMIT_CALLOUT_ENABLE > 0u)
            if (NULL_PTR != txIpduPtr->ComIPduTriggerTransmitCallout)
            {
                PduInfoType pduInfo;
                pduInfo.SduDataPtr = &Com_TxIPduRuntimeBuff[ipduBufferId];
                pduInfo.SduLength = txIPduLength;
#if (STD_ON == COM_METADATA_SUPPORT)
                if (0u < metaDataLength)
                {
                    metaDataStartId = (uint16)txIpduPtr->IPduNoDynSignalLength
                                      + (uint16)txIpduPtr->IPduMaxDynSignalLength + ipduBufferId;
                    pduInfo.MetaDataPtr = &Com_TxIPduRuntimeBuff[metaDataStartId];
                }
                else
#endif
                {
                    pduInfo.MetaDataPtr = NULL_PTR;
                }
                (void)(txIpduPtr->ComIPduTriggerTransmitCallout(TxPduId, &pduInfo));
            }
#endif
#if (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE) || (STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE) \
    || (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
            if (COM_TRIGGER_TRANSMIT == txIpduPtr->ComTxIPduClearUpdateBit)
            {
                Com_ClearTxIPduUpdates(txIpduPtr);
            }
#endif /* STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE || STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE || STD_ON \
          == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE */
            txIpduStatePtr->TxIpduRTStFlag |= Com_TX_TRANSMITING_EN;
            returnValue = E_OK;
        }
    }
    else
    {
#if (STD_ON == COM_TX_SIGNAL_ERROR_NOTIFICATION_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_ERROR_NOTIFICATION_ENABLE)
        Com_InvokeErrorNotification(txIpduId);
#endif /* STD_ON == COM_TX_SIGNAL_ERROR_NOTIFICATION_ENABLE || STD_ON == \
          COM_TX_SIG_GROUP_ERROR_NOTIFICATION_ENABLE */
    }
    return returnValue;
}
#endif
#if (0u < COM_TXIPDU_NUMBER)
/******************************************************************************/
/*
 * Brief               IF TxPdu TxConfirmation handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxPduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, COM_CODE)
Com_TxConfirmationHandle(PduIdType TxPduId) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    PduIdType comTxPduId = TxPduId - COM_RXIPDU_NUMBER;
    const Com_TxIPduType* txIpduPtr = &Com_ConfigStd->ComTxIPdu[comTxPduId];
    Com_TxIPduRunTimeStateType* txIpduStatePtr = &Com_TxIPduRunTimeState[comTxPduId];
    uint8 activeEnable = txIpduStatePtr->TxIpduRTStFlag & Com_TX_ACTIVE_EN;

    if (Com_TX_ACTIVE_EN == activeEnable)
    {
/*Reset Tx_IPduMDT*/
#if (STD_ON == COM_MDT_ENABLE)
        Com_ResetTxIpduMDT(txIpduStatePtr, txIpduPtr);
#endif

        /*repetitions of transmission */
#if (0u < COM_TX_MODE_TRUE_DIRECT_NUMBER) || (0u < COM_TX_MODE_FALSE_DIRECT_NUMBER)                              \
    || (0u < COM_TX_MODE_TRUE_DIRECT_NOREPETITION_NUMBER) || (0u < COM_TX_MODE_FALSE_DIRECT_NOREPETITION_NUMBER) \
    || ((0u < COM_TX_MODE_TRUE_MIXED_NUMBER) || (0u < COM_TX_MODE_FALSE_MIXED_NUMBER)                            \
        || (0u < COM_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER) || (0u < COM_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER))
        if (txIpduStatePtr->RptNum > 0u)
        {
            (txIpduStatePtr->RptNum) -= 1u;
            if (0u == txIpduStatePtr->RptNum)
            {
                txIpduStatePtr->DMCnt = 0u;
                txIpduStatePtr->NTimeCnt = 0u;
            }
        }
        /*no repetitions of transmission */
        else
#endif
        {
            txIpduStatePtr->DMCnt = 0u;
        }
#if (STD_ON == COM_TX_SIGNAL_NOTIFICATION_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_NOTIFICATION_ENABLE)
        if (0u == txIpduStatePtr->RptNum)
        {
#if (STD_ON == COM_TX_IPDU_SIGNAL_PROCESS_IMMEDIATE_ENABLE)
            Com_IPduSignalProcessingType ipduSignalProcessing = txIpduPtr->ComIPduSignalProcessing;
            /*signal/signal group TxConfirmation handle is made immediate*/
            if (COM_IMMEDIATE == ipduSignalProcessing)
            {
                /*Tx notification for all signals/signal groups of the Tx Pdu*/
                Com_ConfirmationProcess(txIpduPtr);
                txIpduStatePtr->TxIpduRTStFlag &= Com_TX_CONFIRM_DIS;
            }
            else
#endif /* STD_ON == COM_TX_IPDU_SIGNAL_PROCESS_IMMEDIATE_ENABLE */
            {
                /*Tx notification for all signals/signal groups of the Tx Pdu in next MainFunctionTx*/
                txIpduStatePtr->TxIpduRTStFlag |= Com_TX_CONFIRM_EN;
            }
        }
#endif /* STD_ON == COM_TX_SIGNAL_NOTIFICATION_ENABLE || STD_ON == COM_TX_SIG_GROUP_NOTIFICATION_ENABLE */
#if (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE) || (STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE) \
    || (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
        if (COM_CONFIRMATION == txIpduPtr->ComTxIPduClearUpdateBit)
        {
            /*clear all update bit of the Tx Pdu*/
            Com_ClearTxIPduUpdates(txIpduPtr);
        }
#endif /* STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE || STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE || STD_ON == \
COM_GW_DEST_SIG_UPDATE_BIT_ENABLE */
        /*reset Tx time out counter for just NONE mode of Tx Mode*/
        if ((COM_UNUSED_UINT16 == txIpduPtr->ComTxModeFalseRefId) && (COM_TX_MODE_NONE == txIpduPtr->ComTxTrueModeMode))
        {
            txIpduStatePtr->DMCnt = txIpduPtr->ComTxIpduDM;
        }
    }
    txIpduStatePtr->TxIpduRTStFlag &= Com_TX_TRANSMITING_DIS;
}
#endif

#if ((0u < COM_TXIPDU_NUMBER) && (STD_ON == COM_TXTPPDU_SUPPORT))
/******************************************************************************/
/*
 * Brief               TP TxPdu TxConfirmation handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      id,result
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, COM_CODE)
Com_TpTxConfirmationHandle(PduIdType id, Std_ReturnType result) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    PduIdType comTxPduId = id - COM_RXIPDU_NUMBER;
    const Com_TxIPduType* txIpduPtr = &Com_ConfigStd->ComTxIPdu[comTxPduId];
    Com_TxIPduRunTimeStateType* txIpduStatePtr = &Com_TxIPduRunTimeState[comTxPduId];
    uint8 activeEnable = txIpduStatePtr->TxIpduRTStFlag & Com_TX_ACTIVE_EN;
    txIpduStatePtr->TxIpduRTStFlag &= Com_TX_TRANSMITING_DIS;
    if (Com_TX_ACTIVE_EN == activeEnable)
    {
        txIpduStatePtr->TxOffset = 0u;
        /*TP PDU successful transmit*/
        if (E_OK == result)
        {
            /*repetitions of transmission */
#if (0u < COM_TX_MODE_TRUE_DIRECT_NUMBER) || (0u < COM_TX_MODE_FALSE_DIRECT_NUMBER)                              \
    || (0u < COM_TX_MODE_TRUE_DIRECT_NOREPETITION_NUMBER) || (0u < COM_TX_MODE_FALSE_DIRECT_NOREPETITION_NUMBER) \
    || ((0u < COM_TX_MODE_TRUE_MIXED_NUMBER) || (0u < COM_TX_MODE_FALSE_MIXED_NUMBER)                            \
        || (0u < COM_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER) || (0u < COM_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER))
            if (txIpduStatePtr->RptNum > 0U)
            {
                (txIpduStatePtr->RptNum) -= 1u;
                if (0u == txIpduStatePtr->RptNum)
                {
                    txIpduStatePtr->DMCnt = 0u;
                    txIpduStatePtr->NTimeCnt = 0u;
                }
            }
            /*no repetitions of transmission */
            else
#endif
            {
                txIpduStatePtr->DMCnt = 0u;
            }
#if (STD_ON == COM_TX_SIGNAL_NOTIFICATION_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_NOTIFICATION_ENABLE)
            if (0u == txIpduStatePtr->RptNum)
            {
#if (STD_ON == COM_TX_IPDU_SIGNAL_PROCESS_IMMEDIATE_ENABLE)
                /*signal/signal group TxConfirmation handle is made immediate*/
                if (COM_IMMEDIATE == txIpduPtr->ComIPduSignalProcessing)
                {
                    /*Tx notification for all signals/signal groups of the Tx Pdu*/
                    Com_ConfirmationProcess(txIpduPtr);
                    txIpduStatePtr->TxIpduRTStFlag &= Com_TX_CONFIRM_DIS;
                }
                else
#endif
                {
                    /*Tx notification for all signals/signal groups of the Tx Pdu in next MainFunctionTx*/
                    txIpduStatePtr->TxIpduRTStFlag |= Com_TX_CONFIRM_EN;
                }
            }
#endif /* STD_ON == COM_TX_SIGNAL_NOTIFICATION_ENABLE || STD_ON == COM_TX_SIG_GROUP_NOTIFICATION_ENABLE */
#if (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE) || (STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE) \
    || (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
            if (COM_CONFIRMATION == txIpduPtr->ComTxIPduClearUpdateBit)
            {
                /*clear all update bit of the Tx Pdu*/
                Com_ClearTxIPduUpdates(txIpduPtr);
            }
#endif /* STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE || STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE || STD_ON == \
COM_GW_DEST_SIG_UPDATE_BIT_ENABLE */
        }
    }
}
/******************************************************************************/
/*
 * Brief               TP TxPdu copy pdu data handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      id,retry
 * Param-Name[out]     availableDataPtr
 * Param-Name[in/out]  info
 * Return              BufReq_ReturnType
 */
/******************************************************************************/
/* PRQA S 1532,3432 ++ */ /* MISRA Rule 8.7,20.7 */
FUNC(BufReq_ReturnType, COM_CODE)
Com_CopyTxDataHandle(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) info,
    P2CONST(RetryInfoType, AUTOMATIC, COM_APPL_DATA) retry,
    P2VAR(PduLengthType, AUTOMATIC, COM_APPL_DATA) availableDataPtr)
/* PRQA S 1532,3432 -- */ /* MISRA Rule 8.7,20.7 */
{
    BufReq_ReturnType bufReq = BUFREQ_E_NOT_OK;
    PduIdType txIpduId = id - COM_RXIPDU_NUMBER;
    uint16 copyStartPos;
    uint8 activeEnable, Transmiting;
#if (STD_ON == COM_METADATA_SUPPORT)
    uint8 metaDataLength;
    uint16 metaDataStartId;
#endif
    const Com_TxIPduType* txIpduPtr = &Com_ConfigStd->ComTxIPdu[txIpduId];
    Com_TxIPduRunTimeStateType* txIpduStatePtr = &Com_TxIPduRunTimeState[txIpduId];
    activeEnable = txIpduStatePtr->TxIpduRTStFlag & Com_TX_ACTIVE_EN;
    Transmiting = txIpduStatePtr->TxIpduRTStFlag & Com_TX_TRANSMITING_EN;

    if ((Com_TX_ACTIVE_EN == activeEnable) && (Com_TX_TRANSMITING_EN == Transmiting))
    {
        /*data retry offset*/
        if ((NULL_PTR != retry) && (TP_DATARETRY == retry->TpDataState))
        {
            txIpduStatePtr->TxOffset = (txIpduStatePtr->TxOffset >= retry->TxTpDataCnt)
                                           ? (txIpduStatePtr->TxOffset - (uint16)retry->TxTpDataCnt)
                                           : 0u;
            /*will not happen general,TP module guarantee*/
        }
        if (((uint16)txIpduStatePtr->TxOffset + (uint16)info->SduLength) <= txIpduStatePtr->TxIpduLength)
        {
            copyStartPos = (uint16)(txIpduPtr->ComTxIPduBufIndex + txIpduStatePtr->TxOffset);
            SchM_Enter_Com_Context();
            /*copy the TX data from TX PDU buffer*/
            (void)ILib_memcpy(&info->SduDataPtr[0], &Com_TxIPduRuntimeBuff[copyStartPos], info->SduLength);
            SchM_Exit_Com_Context();
/*copy the meta data from Tx Pdu Buffer*/
#if (STD_ON == COM_METADATA_SUPPORT)
            metaDataLength = txIpduPtr->MetaDataLength;
            if ((0u < metaDataLength) && (NULL_PTR != info->MetaDataPtr))
            {
                metaDataStartId = (uint16)txIpduPtr->IPduNoDynSignalLength + (uint16)txIpduPtr->IPduMaxDynSignalLength
                                  + txIpduPtr->ComTxIPduBufIndex;
                SchM_Enter_Com_Context();
                (void)ILib_memcpy(&info->MetaDataPtr[0], &Com_TxIPduRuntimeBuff[metaDataStartId], metaDataLength);
                SchM_Exit_Com_Context();
            }
#endif
            txIpduStatePtr->TxOffset += (uint16)info->SduLength;
            *availableDataPtr = (PduLengthType)txIpduStatePtr->TxIpduLength - (PduLengthType)txIpduStatePtr->TxOffset;
            bufReq = BUFREQ_OK;
        }
    }
    return bufReq;
}
#endif
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
/*******************************************************************************
**                      End of file                                           **
*******************************************************************************/
