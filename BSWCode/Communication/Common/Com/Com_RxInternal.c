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
**  FILENAME    : Com_RxInternal.c                                            **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : internal implementation for COM receive                     **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Com_Internal.h"
#if (STD_ON == COM_DEV_ERROR_DETECT)
#include "Det.h"
#endif

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

#define COM_RX_SIGNAL_DM             (1u)
#define COM_RX_SIGNAL_GROUP_DM       (2u)
#define COM_RX_IPDU_DM               (4u)
#define COM_RX_IPDU_DM_ONLY          (8u)
#define COM_SET_RX_DM_FLAG(Var, val) (Var) = ((uint8)((Var) | (uint8)(val)))
#define COM_CLEAR_RX_DM_FLAG(Var)    (Var) = ((uint8)(0u))

/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/* called by Com_RxIpduController
 * reset OCCURRENCE of filter with ComFilterAlgorithm ONE_EVERY_N*/
#if ((COM_IPDUGROUP_NUMBER > 0u) && (COM_ONEEVERYNFILTERSIGNAL_NUMBER > 0u))
static FUNC(void, COM_CODE) Com_ResetRxOccurrenceOfPduFilter(const Com_RxIPduType* RxIpduPtr);
#endif
/*Called by Com_RxIndication.
 *Receive Rx Pdu handle*/
#if (0u < COM_RXIPDU_NUMBER)
static FUNC(void, COM_CODE)
    Com_RxPduHandle(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr);
#endif
/* called by Com_MainFunctionRx
 * RxSignalGroup receive timeout handle*/
#if ((0u < COM_RXIPDU_NUMBER) && (COM_RXSIGNALGROUP_NUMBER > 0u) && (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE))
static FUNC(void, COM_CODE)
    Com_RxSignalGroupTimeOutHandle(Com_RxIPduRunTimeStateType* RxIpduStatePtr, const Com_RxIPduType* RxIpduPtr);
#endif /* 0u < COM_RXIPDU_NUMBER) && (COM_RXSIGNALGROUP_NUMBER > 0u) && (STD_ON == \
          COM_RX_SIG_GROUP_TIMEOUT_ENABLE */
/* called by Com_RxIpduController
 * init the rx ipdu buffer,all signal buffer(included in the ipdu,except source signal)*/
#if (COM_IPDUGROUP_NUMBER > 0u)
static FUNC(void, COM_CODE) Com_ResetRxPduBufferAndSignalBuffer(PduIdType IpduId);
#endif
/* called by Com_IndicationProcess.
 * unpack the rx source description signal to gateway buffer*/
#if (0u < COM_GW_SOURCE_DESCRIPTION_NUMBER) && (COM_GWMAPPING_NUMBER > 0u)
static FUNC(void, COM_CODE) Com_SourceSignalUnPack(
    const Com_GwSourceSignalType* GwSourceSignalPtr,
    uint16 RxDynSignalLength,
    Com_RxIpduBufIdType IpduBufferId);
#endif /* 0u < COM_GW_SOURCE_DESCRIPTION_NUMBER && COM_GWMAPPING_NUMBER > 0u */
/* called by Com_IndicationProcess.
 * unpack the rx signal and notification up layer*/
#if (0u < COM_RXSIGNAL_NUMBER)
static FUNC(void, COM_CODE) Com_SignalRxIndication(
    const Com_RxSignalType* RxSignalPtr,
    uint16 RxDynSignalLength,
    Com_RxIpduBufIdType IpduBufferId);
#endif
/* called by Com_SignalRxIndication.
 * unpack the rx signal(signal type is COM_UINT8_N) and notification up layer*/
#if ((0u < COM_RXSIGNAL_NUMBER) && (0u < COM_RXIPDUBUFF_SIZE) && (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE))
static FUNC(void, COM_CODE)
    Com_Rx8NSignalHandle(const Com_RxSignalType* RxSignalPtr, uint16 SignalLength, Com_RxIpduBufIdType IpduBufferId);
#endif /* 0u < COM_RXSIGNAL_NUMBER && 0u < COM_RXIPDUBUFF_SIZE && STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE \
        */
/* called by Com_SignalRxIndication.
 * unpack the rx signal(signal type is COM_UINT8_DYN) and notification up layer*/
#if ((0u < COM_RXSIGNAL_NUMBER) && (0u < COM_RXIPDUBUFF_SIZE) && (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE))
static FUNC(void, COM_CODE)
    Com_Rx8DYNSignalHandle(const Com_RxSignalType* RxSignalPtr, uint16 SignalLength, Com_RxIpduBufIdType IpduBufferId);
#endif /* 0u < COM_RXSIGNAL_NUMBER) && (0u < COM_RXIPDUBUFF_SIZE) && (STD_ON == \
          COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE */
/* called by Com_SignalRxIndication.
 * unpack the rx signal(signal type isn't COM_UINT8_N or COM_UINT8_DYN) and notification up layer*/
#if ((0u < COM_RXSIGNAL_NUMBER) && (0u < COM_RXIPDUBUFF_SIZE))
static FUNC(void, COM_CODE) Com_RxSignalHandle(const Com_RxSignalType* RxSignalPtr);
#endif
/* called by Com_RxSignalHandle,Com_SignalGroupRxIndication.
 * judge the rx (group) signal value is invalid value or not*/
#if ((STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE) || (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ENABLE))
static FUNC(boolean, COM_CODE)
    Com_SignalInvalidateHandle(Com_SignalType SignalType, uint16 InvalidValueId, uint64 Value);
#endif

/* called by Com_RxSignalHandle,Com_InvalidSignalReplaceAndNotification,Com_RxSignalGroupFilter.
 * get the rx (group) signal's init value*/
#if (                                                                                      \
    ((0u < COM_RXGROUPSIGNAL_NUMBER) && (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE)) \
    || ((0u < COM_RXSIGNAL_NUMBER) && (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ENABLE)))

static FUNC(uint64, COM_CODE) Com_RxSignalReplaceHanlde(Com_SignalType SignalType, uint16 InitValueId);
#endif /* 0u < COM_RXSIGNAL_NUMBER || 0u < COM_RXGROUPSIGNAL_NUMBER and STD_ON == \
          COM_RX_SIGNAL_INVALID_DATA_ENABLE || STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE */
/* called by Com_RxSignalHandle,Com_InvalidSignalReplaceAndNotification.
 * calculate the rx signal value is filter out or not*/
#if (                                                                         \
    ((0u < COM_RXSIGNAL_NUMBER) && (COM_RXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)) \
    || ((0u < COM_RXGROUPSIGNAL_NUMBER) && (COM_RXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)))
static FUNC(boolean, COM_CODE) Com_RxSignalFilter(
    Com_SignalType SignalType,
    Com_FilterAlgorithmType FilterType,
    Com_SignalIdType ComFilterIndex,
#if (COM_RXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
    uint16 SignalBufferId,
#endif
    uint64 NewSignalValue);
#endif

/* called by Com_RxSignalHandle,Com_InvalidSignalReplaceAndNotification.
 * update the rx signal value buffer*/
#if (0u < COM_RXSIGNAL_NUMBER) || (0u < COM_RXGROUPSIGNAL_NUMBER)
static FUNC(void, COM_CODE) Com_RxSignalBuffHandle(uint64 Value, Com_SignalType SignalType, uint16 InitValueId);
#endif
/* called by Com_RxSignalHandle,Com_SignalGroupRxIndication.
 * update the gateway signal value buffer of the rx signal*/
#if (((0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXGROUPSIGNAL_NUMBER)) || (0u < COM_RXSIGNAL_NUMBER)) \
    && (COM_GWMAPPING_NUMBER > 0u)
static FUNC(void, COM_CODE) Com_UpdateGWSignalBuffer(uint64 Value, uint16 GWSignalBufferId, Com_SignalType SignalType);
#endif

/* called by Com_TpPduInvalidAction.
 * rx signal value replace,and notification*/
#if ((STD_ON == COM_RXTPPDU_SUPPORT) && (0u < COM_RXSIGNAL_NUMBER) && (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ENABLE))
static FUNC(void, COM_CODE) Com_InvalidSignalReplaceAndNotification(const Com_RxSignalType* RxSignalPtr);
#endif

/* called by Com_IndicationProcess.
 * handle signal group RxIndication of one rx pdu*/
#if ((0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXGROUPSIGNAL_NUMBER))
static FUNC(void, COM_CODE) Com_SignalGroupRxIndication(
    const Com_RxSignalGroupType* RxSignalGroupPtr,
    uint16 RxDynSignalLength,
    Com_RxIpduBufIdType IpduBufferId);
#endif

/* called by Com_SignalGroupRxIndication,Com_TpPduInvalidAction.
 * update the rx group signal value buffer*/
#if ((0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXGROUPSIGNAL_NUMBER))
static FUNC(void, COM_CODE) Com_RxGroupSignalBuffHanlde(
    boolean InvalidSignalGroup,
    Com_RxGroupSignalIdType Id,
    Com_RxIpduBufIdType IpduBufferId,
    uint16 RxDynSignalLength);
#endif

#if ((0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXGROUPSIGNAL_NUMBER))
#if ((STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE))
/*Uint8_Dyn/Uint8_N Group Signal Invalid Calculate*/
static FUNC(void, COM_CODE) Com_Uint8NDynGroupSignalInvalidCalculate(
    uint16 SignalLength,
    Com_SignalIdType GroupSignalId,
#if (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE)
    boolean* SignalGroupInvalid,
#endif
    Com_RxIpduBufIdType IpduBufferId);
#endif

/*Called by Com_SignalGroupRxIndication.
 *Signal Group Invalid Calculate*/
/*Signal Group Invalid Calculate*/
static FUNC(boolean, COM_CODE) Com_SignalGroupInvalidCalculate(
    const Com_RxSignalGroupType* RxSignalGroupPtr,
    Com_RxIpduBufIdType IpduBufferId,
    uint16 RxDynSignalLength);
#endif /* 0u < COM_RXSIGNALGROUP_NUMBER && 0u < COM_RXGROUPSIGNAL_NUMBER */

/* RxSignal receive timeout handle*/
#if ((0u < COM_RXIPDU_NUMBER) && (COM_RXSIGNAL_NUMBER > 0u) && (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE))
static FUNC(void, COM_CODE)
    Com_RxSignalTimeOutHandle(Com_RxIPduRunTimeStateType* RxIpduStatePtr, const Com_RxIPduType* RxIpduPtr);
#endif /* 0u < COM_RXIPDU_NUMBER) && (COM_RXSIGNAL_NUMBER > 0u) && (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE */

#if (                                                                        \
    ((0u < COM_RXSIGNAL_NUMBER) && (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE)) \
    || ((0u < COM_RXGROUPSIGNAL_NUMBER) && (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)))
static FUNC(void, COM_CODE)
    Com_RxIpduBaseDMTimeOutHandle(Com_RxIPduRunTimeStateType* RxIpduStatePtr, const Com_RxIPduType* RxIpduPtr);
#endif /* 0u < COM_RXSIGNAL_NUMBER && STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE \
       or 0u < COM_RXGROUPSIGNAL_NUMBER && STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE */

#if (0u < COM_RXSIGNAL_NUMBER) && (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE)
static FUNC(void, COM_CODE)
    Com_RxDMTimeOutHandleSignal(Com_RxIPduRunTimeStateType* RxIpduStatePtr, const Com_RxIPduType* RxIpduPtr);
#endif

#if (0u < COM_RXGROUPSIGNAL_NUMBER) && (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)
static FUNC(void, COM_CODE)
    Com_RxDMTimeOutHandleSignalGroup(Com_RxIPduRunTimeStateType* RxIpduStatePtr, const Com_RxIPduType* RxIpduPtr);
#endif
#if ((COM_RXSIGNAL_NUMBER + COM_RXSIGNALGROUP_NUMBER) > 0u)
/*set the reception deadline monitoring timer for the
 *included signals and signal groups to the configured
 *ComFirstTimeout*/
static FUNC(void, COM_CODE) Com_ResetRxPduTimeOut(PduIdType IpduId);
/*set the reception deadline monitoring timer for the
 *included signals and signal groups to 0*/
static FUNC(void, COM_CODE) Com_DisableRxPduTimeOut(PduIdType IpduId);
#endif
/* called by Com_RxIndication,Com_TpRxIndication.
 * reset receive MD timer for all signals/signal groups(which is update) of one rx pdu*/
#if (COM_RXSIGNAL_NUMBER + COM_RXSIGNALGROUP_NUMBER > 0u) \
    && ((STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE) || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE))
/*reset receive MD timer for all signals/signal groups
 *(which is update) of one rx pdu.*/
static FUNC(void, COM_CODE)
    Com_ResetUpdateDMTime(PduIdType IpduId, P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfo);
#endif
/* called by Com_TpRxIndication.
 * unpack the receive pdu's counter*/
#if (STD_ON == COM_RXTPPDU_SUPPORT) && (STD_ON == COM_RX_IPDU_COUNTER_ENABLE)
static FUNC(uint8, COM_CODE)
    Com_UnPackCounterValue(Com_RxIPduCounterIdType IpduCounterId, Com_RxIpduBufIdType RxPduBufferId);
#endif
/* called by Com_TpRxIndication,Com_RxIndication.
 * judge the receive pdu's counter is valid or not*/
#if (STD_ON == COM_RX_IPDU_COUNTER_ENABLE)
static FUNC(boolean, COM_CODE) Com_RxPduCounterValid(PduIdType IpduId, uint8 RxCounter);
#endif /* STD_ON == COM_RX_IPDU_COUNTER_ENABLE */
/* called by Com_RxIndication,Com_RxPduReplicationValid.
 * copy the Rx Pdu data to RxPdu Buffer*/
static FUNC(void, COM_CODE)
    Com_CopyPduDataToRxBuffer(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr);
/* called by Com_RxIndication,Com_TpRxIndication,Com_MainFunctionRx.
 * unpack the rx pdu data and invoke notification*/
static FUNC(void, COM_CODE)
    Com_IndicationProcess(const Com_RxIPduRunTimeStateType* RxIpduStatePtr, const Com_RxIPduType* RxIpduPtr);
/* called by Com_TpRxIndication.
 * execute all configured ComDataInvalidActions for all included signals and signal groups*/
#if (STD_ON == COM_RXTPPDU_SUPPORT) \
    && ((STD_ON == COM_RX_SIGNAL_INVALID_DATA_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE))
static FUNC(void, COM_CODE) Com_TpPduInvalidAction(PduIdType RxIpduId);
#endif
/* called by Com_RxIndication.
 * judge the receive pdu's Replication is valid or not*/
#if (STD_ON == COM_IPDU_REPLICATION_ENABLE) && (STD_ON == COM_RX_IPDU_COUNTER_ENABLE)
static FUNC(boolean, COM_CODE) Com_RxPduReplicationValid(
    PduIdType IpduId,
    uint8 RxCounter,
    P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr);
#endif
#if (                                                              \
    (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_REPLACE_ENABLE)        \
    || (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)) \
    || ((STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_REPLACE_ENABLE) \
        || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_SUBSTITUTE_ENABLE))
static void Com_RxDataTimeOutActionHandle(
    Com_RxIPduRunTimeStateType* RxIpduStatePtr,
    const Com_RxIPduType* RxIpduPtr,
    Com_RxDataTimeoutActionType RxTimeoutAction,
    const void* RxArgPtr,
    boolean IsRxSignal,
    Com_SignalIdType SubstituteBufId);
#endif
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
#if ((COM_IPDUGROUP_NUMBER > 0u) && (COM_ONEEVERYNFILTERSIGNAL_NUMBER > 0u))
/******************************************************************************/
/*
 * Brief               reset OCCURRENCE of filter with ComFilterAlgorithm
 *                     ONE_EVERY_N.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      IpduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE) Com_ResetRxOccurrenceOfPduFilter(const Com_RxIPduType* RxIpduPtr)
{
/*reset signal/group signal of rx pdu which ComFilterAlgorithm is ONE_EVERY_N*/
#if (0u < COM_RXSIGNAL_NUMBER) && (COM_RXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)
    Com_RxSignalIdType ipduSignalRefNumber, cnt;
    const Com_RxSignalType* rxSignalPtr;
    ipduSignalRefNumber = RxIpduPtr->ComIPduSignalsRefNumber;
    cnt = RxIpduPtr->ComIpduSignalRefStartId;
    for (; cnt < ipduSignalRefNumber; ++cnt)
    {
        rxSignalPtr = &Com_ConfigStd->ComRxSignal[cnt];
        if (COM_UNUSED_RXSIGNALFILTERID != rxSignalPtr->ComFilterIndex)
        {
            Com_OneEveryNcnt[rxSignalPtr->ComFilterIndex] = 0u;
        }
    }
#endif
#if ((0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXGROUPSIGNAL_NUMBER) && (COM_RXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u))
    Com_RxSignalGroupIdType ipduSignalGroupNumber, counter;
    Com_RxGroupSignalIdType groupSignalNumber, groupSignalId;
    const Com_RxGroupSignalType* rxGroupSignalPtr;
    ipduSignalGroupNumber = RxIpduPtr->ComIPduSignalGroupsRefNumber;
    counter = RxIpduPtr->ComIPduSignalGroupsRefStartId;
    Com_RxSignalGroupType* rxSignalGroupPtr;
    for (; counter < ipduSignalGroupNumber; ++counter)
    {
        rxSignalGroupPtr = &Com_ConfigStd->ComRxSignalGroup[counter];
        groupSignalNumber = rxSignalGroupPtr->ComGroupSignalRefNumber;
        groupSignalId = rxSignalGroupPtr->ComGroupSignalRefStartId;
        for (; groupSignalId < groupSignalNumber; ++groupSignalId)
        {
            rxGroupSignalPtr = &Com_ConfigStd->ComRxGroupSignal[groupSignalId];
            if (COM_UNUSED_RXGRPSIGFILTERID != rxGroupSignalPtr->ComFilterIndex)
            {
                Com_OneEveryNcnt[rxGroupSignalPtr->ComFilterIndex] = 0u;
            }
        }
    }
#endif
    return;
}
#endif

#if ((0u < COM_RXSIGNAL_NUMBER) && (0u < COM_SIGNAL_8BITBUFF_SIZE) && (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE)) \
    && (0u < COM_RXIPDU_NUMBER)
/******************************************************************************/
/*
 * Brief               Receive Dynamic Signal handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalId
 * Param-Name[out]     SignalDataPtr
 * Param-Name[in/out]  Length
 * Return              uint8
 */
/******************************************************************************/
/* PRQA S 3432,3673,1532 ++ */ /* MISRA Rule 20.7,8.13,8.7 */
FUNC(uint8, COM_CODE)
Com_ReceiveDynSignalHandle(
    Com_SignalIdType SignalId,
    P2VAR(void, AUTOMATIC, COM_APPL_CONST) SignalDataPtr,
    P2VAR(uint16, AUTOMATIC, COM_APPL_CONST) Length)
/* PRQA S 3432,3673,1532 -- */ /* MISRA Rule 20.7,8.13,8.7 */
{
    const Com_RxSignalType* rxSignalPtr = &Com_ConfigStd->ComRxSignal[SignalId];
    PduIdType ipduRef = rxSignalPtr->ComIpduRefIndex;
    const Com_RxIPduRunTimeStateType* rxIpduStatePtr = &Com_RxIPduRunTimeState[ipduRef];
    uint8 ret = COM_SERVICE_NOT_AVAILABLE;
    /*the signal type must be COM_UINT8_DYN */
    if (COM_UINT8_DYN == rxSignalPtr->ComSignalType)
    {
        /*IPDU is not receiving data*/
        uint8 Receiving = rxIpduStatePtr->RxIpduRTStFlag & Com_RX_RECEIVING_EN;
        if (Com_RX_RECEIVING_EN != Receiving)
        {
            uint16 signalLength =
                (rxIpduStatePtr->RxIpduLength - (uint16)Com_ConfigStd->ComRxIPdu[ipduRef].IPduNoDynSignalLength);
            if (signalLength > (*Length))
            {
#if (STD_ON == COM_DEV_ERROR_DETECT)
                (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_RECEIVEDYNSIGNAL_ID, COM_E_PARAM);
#endif
                ret = E_NOT_OK;
            }
            else
            {
                uint16 signalInitValueId = rxSignalPtr->ComSignalInitValueId;
                /* Copy Uint8DYN signal value one byte by one */
                SchM_Enter_Com_Context();
                (void)ILib_memcpy((uint8*)SignalDataPtr, &Com_Signal8BitRuntimeBuff[signalInitValueId], signalLength);
                SchM_Exit_Com_Context();
                *Length = signalLength;
                /* Check IPdu which this group signal belonged is Active or not*/
                uint8 activeEnable = rxIpduStatePtr->RxIpduRTStFlag & Com_RX_ACTIVE_EN;
                if (Com_RX_ACTIVE_EN == activeEnable)
                {
                    ret = E_OK;
                }
            }
        }
        else
        {
            ret = COM_BUSY;
        }
    }
    return ret;
}
#endif

#if ((0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXIPDU_NUMBER))
/******************************************************************************/
/*
 * Brief               Receive all group signal of the SignalGroup
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      RxPduId,SignalGroupId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
FUNC(void, COM_CODE) Com_ReceiveSignalGroupHandle(PduIdType RxPduId, Com_SignalGroupIdType SignalGroupId)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    const Com_RxSignalGroupType* rxSignalGroupPtr = &Com_ConfigStd->ComRxSignalGroup[SignalGroupId];
    const Com_RxGroupSignalType* rxGroupSignalPtr;
    Com_RxGroupSignalIdType groupSignalNumber = rxSignalGroupPtr->ComGroupSignalRefNumber;
    Com_RxGroupSignalIdType cnt = rxSignalGroupPtr->ComGroupSignalRefStartId;
    Com_SignalType signalType;
    uint16 signalInitValueId;
#if ((0u < COM_RXGROUPSIGNAL_8BITBUFF_SIZE) && (0u < COM_SIGNAL_8BITBUFF_SIZE))
    uint16 signalLength;
#endif

    SchM_Enter_Com_Context();
    for (; cnt < groupSignalNumber; ++cnt)
    {
        rxGroupSignalPtr = &Com_ConfigStd->ComRxGroupSignal[cnt];
        signalInitValueId = rxGroupSignalPtr->ComSignalInitValueId;
        signalType = rxGroupSignalPtr->ComSignalType;
        switch (signalType)
        {
#if ((0u < COM_RXGROUPSIGNAL_BOOLBUFF_SIZE) && (0u < COM_SIGNAL_BOOLBUFF_SIZE))
        case COM_BOOLEAN:
            Com_RxGroupSignalBoolShadowBuff[signalInitValueId] = Com_SignalBoolRuntimeBuff[signalInitValueId];
            break;
#endif
#if ((0u < COM_RXGROUPSIGNAL_8BITBUFF_SIZE) && (0u < COM_SIGNAL_8BITBUFF_SIZE))
        case COM_SINT8:
        case COM_UINT8:
            Com_RxGroupSignal8BitShadowBuff[signalInitValueId] = Com_Signal8BitRuntimeBuff[signalInitValueId];
            break;
#endif
#if ((0u < COM_RXGROUPSIGNAL_16BITBUFF_SIZE) && (0u < COM_SIGNAL_16BITBUFF_SIZE))
        case COM_SINT16:
        case COM_UINT16:
            Com_RxGroupSignal16BitShadowBuff[signalInitValueId] = Com_Signal16BitRuntimeBuff[signalInitValueId];
            break;
#endif
#if ((0u < COM_RXGROUPSIGNAL_32BITBUFF_SIZE) && (0u < COM_SIGNAL_32BITBUFF_SIZE))
        case COM_SINT32:
        case COM_UINT32:
        case COM_FLOAT32:
            Com_RxGroupSignal32BitShadowBuff[signalInitValueId] = Com_Signal32BitRuntimeBuff[signalInitValueId];
            break;
#endif
#if ((0u < COM_RXGROUPSIGNAL_64BITBUFF_SIZE) && (0u < COM_SIGNAL_64BITBUFF_SIZE))
        case COM_SINT64:
        case COM_UINT64:
        case COM_FLOAT64:
            Com_RxGroupSignal64BitShadowBuff[signalInitValueId] = Com_Signal64BitRuntimeBuff[signalInitValueId];
            break;
#endif
#if ((0u < COM_RXGROUPSIGNAL_8BITBUFF_SIZE) && (0u < COM_SIGNAL_8BITBUFF_SIZE))
#if (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE)
        case COM_UINT8_N:
            signalLength = rxGroupSignalPtr->ComSignalLength;
            (void)ILib_memcpy(
                &Com_RxGroupSignal8BitShadowBuff[signalInitValueId],
                &Com_Signal8BitRuntimeBuff[signalInitValueId],
                signalLength);
            break;
#endif /* STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE */
#if (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
        case COM_UINT8_DYN:
            signalLength = Com_RxIPduRunTimeState[RxPduId].RxIpduLength
                           - (uint16)Com_ConfigStd->ComRxIPdu[RxPduId].IPduNoDynSignalLength;
            (void)ILib_memcpy(
                &Com_RxGroupSignal8BitShadowBuff[signalInitValueId],
                &Com_Signal8BitRuntimeBuff[signalInitValueId],
                signalLength);
            break;
#endif /* STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE */
#endif
        default:
            /*do nothing*/
            break;
        }
    }
    SchM_Exit_Com_Context();
    COM_NOUSED(RxPduId);
    return;
}
#endif

#if (0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXGROUPSIGNAL_NUMBER) && (0u < COM_RXIPDU_NUMBER)
/******************************************************************************/
/*
 * Brief               Receive group signal handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalId
 * Param-Name[out]     SignalDataPtr
 * Param-Name[in/out]  None
 * Return              uint8
 */
/******************************************************************************/
/* PRQA S 3673,1532 ++ */ /* MISRA Rule 8.13,8.7 */
FUNC(uint8, COM_CODE)
Com_ReceiveGroupSignalHandle(Com_SignalIdType SignalId, P2VAR(void, AUTOMATIC, COM_APPL_CONST) SignalDataPtr)
/* PRQA S 3673,1532 -- */ /* MISRA Rule 8.13,8.7 */
{
    const Com_RxGroupSignalType* rxGroupSignalPtr = &Com_ConfigStd->ComRxGroupSignal[SignalId - COM_RXSIGNAL_NUMBER];
    Com_RxSignalGroupIdType signalGroupId = rxGroupSignalPtr->ComSignalGroupRef;
    PduIdType ipduRef = Com_ConfigStd->ComRxSignalGroup[signalGroupId].ComIpduRefIndex;
    const Com_RxIPduRunTimeStateType* rxIpduStatePtr = &Com_RxIPduRunTimeState[ipduRef];
    uint16 signalInitValueId;
    uint8 ret = COM_SERVICE_NOT_AVAILABLE;
#if (0u < COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE)
    uint16 signalLength;
#endif

    /*TP IPDU is not receiving data*/
    uint8 Receiving = rxIpduStatePtr->RxIpduRTStFlag & Com_RX_RECEIVING_EN;
    if (Com_RX_RECEIVING_EN != Receiving)
    {
        signalInitValueId = rxGroupSignalPtr->ComSignalInitValueId;
        /* Get signal type, decide signal value is in which Runtime buffer */
        /* PRQA S 3315 ++ */ /* MISRA Rule 16.6 */
        switch (rxGroupSignalPtr->ComSignalType)
        {
#if (0u < COM_RXGROUPSIGNAL_BOOLBUFF_SIZE)
        case COM_BOOLEAN:
            *((boolean*)SignalDataPtr) = Com_RxGroupSignalBoolShadowBuff[signalInitValueId];
            break;
#endif
#if (0u < COM_RXGROUPSIGNAL_8BITBUFF_SIZE)
        case COM_UINT8:
        case COM_SINT8:
            *((uint8*)SignalDataPtr) = Com_RxGroupSignal8BitShadowBuff[signalInitValueId];
            break;
#if (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE)
        case COM_UINT8_N:
            signalLength = rxGroupSignalPtr->ComSignalLength;
            /* Copy Uint8N signal value one byte by one */
            (void)ILib_memcpy((uint8*)SignalDataPtr, &Com_RxGroupSignal8BitShadowBuff[signalInitValueId], signalLength);
            break;
#endif
#endif /* 0u < COM_RXGROUPSIGNAL_8BITBUFF_SIZE */
#if (0u < COM_RXGROUPSIGNAL_16BITBUFF_SIZE)
        case COM_UINT16:
        case COM_SINT16:
            *((uint16*)SignalDataPtr) = Com_RxGroupSignal16BitShadowBuff[signalInitValueId];
            break;
#endif
#if (0u < COM_RXGROUPSIGNAL_32BITBUFF_SIZE)
        case COM_SINT32:
        case COM_UINT32:
            *((uint32*)SignalDataPtr) = Com_RxGroupSignal32BitShadowBuff[signalInitValueId];
            break;
        case COM_FLOAT32:
            /* PRQA S 0310 ++ */ /* MISRA Rule 11.3 */
            *((float32*)SignalDataPtr) = *(float32*)(&Com_RxGroupSignal32BitShadowBuff[signalInitValueId]);
            /* PRQA S 0310 -- */ /* MISRA Rule 11.3 */
            break;
#endif
#if (0u < COM_RXGROUPSIGNAL_64BITBUFF_SIZE)
        case COM_SINT64:
        case COM_UINT64:
            *((uint64*)SignalDataPtr) = Com_RxGroupSignal64BitShadowBuff[signalInitValueId];
            break;
        case COM_FLOAT64:
            /* PRQA S 0310 ++ */ /* MISRA Rule 11.3 */
            *((float64*)SignalDataPtr) = *(float64*)(&Com_RxGroupSignal64BitShadowBuff[signalInitValueId]);
            /* PRQA S 0310 -- */ /* MISRA Rule 11.3 */
            break;
#endif
        default:
            /*do nothing*/
            break;
        }
        /* PRQA S 3315 -- */ /* MISRA Rule 16.6 */
        /* Check IPdu which this group signal belonged is Active or not */
        uint8 activeEnable = rxIpduStatePtr->RxIpduRTStFlag & Com_RX_ACTIVE_EN;
        if (Com_RX_ACTIVE_EN == activeEnable)
        {
            ret = E_OK;
        }
    }
    else
    {
        ret = COM_BUSY;
    }
    COM_NOUSED(SignalDataPtr);
    return ret;
}
#endif

#if (0u < COM_RXSIGNAL_NUMBER) && (0u < COM_RXIPDU_NUMBER)
/******************************************************************************/
/*
 * Brief               Receive signal handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalId
 * Param-Name[out]     SignalDataPtr
 * Param-Name[in/out]  None
 * Return              uint8
 */
/******************************************************************************/
/* PRQA S 3673,1532 ++ */ /* MISRA Rule 8.13,8.7 */
FUNC(uint8, COM_CODE)
Com_ReceiveSignalHandle(Com_SignalIdType SignalId, P2VAR(void, AUTOMATIC, COM_APPL_CONST) SignalDataPtr)
/* PRQA S 3673,1532 -- */ /* MISRA Rule 8.13,8.7 */
{
    const Com_RxSignalType* rxSignalPtr = &Com_ConfigStd->ComRxSignal[SignalId];
    uint8 ret = COM_SERVICE_NOT_AVAILABLE;
    PduIdType ipduRef = rxSignalPtr->ComIpduRefIndex;
    const Com_RxIPduRunTimeStateType* rxIpduStatePtr = &Com_RxIPduRunTimeState[ipduRef];
    uint16 signalInitValueId;
#if (0u < COM_SIGNAL_8BITBUFF_SIZE)
    uint16 signalLength;
#endif

    /*IPDU is not receiving data*/
    uint8 Receiving = rxIpduStatePtr->RxIpduRTStFlag & Com_RX_RECEIVING_EN;
    if (Com_RX_RECEIVING_EN != Receiving)
    {
        signalInitValueId = rxSignalPtr->ComSignalInitValueId;
        /* Get signal type, decide signal value is in which Runtime buffer */
        SchM_Enter_Com_Context();
        switch (rxSignalPtr->ComSignalType)
        {
#if (0u < COM_SIGNAL_BOOLBUFF_SIZE)
        case COM_BOOLEAN:
            *((boolean*)SignalDataPtr) = Com_SignalBoolRuntimeBuff[signalInitValueId];
            break;
#endif
#if (0u < COM_SIGNAL_8BITBUFF_SIZE)
        case COM_UINT8:
        case COM_SINT8:
            *((uint8*)SignalDataPtr) = Com_Signal8BitRuntimeBuff[signalInitValueId];
            break;
#if (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE)
        case COM_UINT8_N:
            signalLength = rxSignalPtr->ComSignalLength;
            /* Copy Uint8N signal value one byte by one */
            (void)ILib_memcpy((uint8*)SignalDataPtr, &Com_Signal8BitRuntimeBuff[signalInitValueId], signalLength);
            break;
#endif
#endif /* 0u < COM_SIGNAL_8BITBUFF_SIZE */
#if (0u < COM_SIGNAL_16BITBUFF_SIZE)
        case COM_UINT16:
        case COM_SINT16:
            *((uint16*)SignalDataPtr) = Com_Signal16BitRuntimeBuff[signalInitValueId];
            break;
#endif
#if (0u < COM_SIGNAL_32BITBUFF_SIZE)
        case COM_SINT32:
        case COM_UINT32:
            *((uint32*)SignalDataPtr) = Com_Signal32BitRuntimeBuff[signalInitValueId];
            break;
        case COM_FLOAT32:
            /* PRQA S 0310 ++ */ /* MISRA Rule 11.3 */
            *((float32*)SignalDataPtr) = *(float32*)(&Com_Signal32BitRuntimeBuff[signalInitValueId]);
            /* PRQA S 0310 -- */ /* MISRA Rule 11.3 */
            break;
#endif
#if (0u < COM_SIGNAL_64BITBUFF_SIZE)
        case COM_SINT64:
        case COM_UINT64:
            *((uint64*)SignalDataPtr) = Com_Signal64BitRuntimeBuff[signalInitValueId];
            break;
        case COM_FLOAT64:
            /* PRQA S 0310 ++ */ /* MISRA Rule 11.3 */
            *((float64*)SignalDataPtr) = *(float64*)(&Com_Signal64BitRuntimeBuff[signalInitValueId]);
            /* PRQA S 0310 -- */ /* MISRA Rule 11.3 */
            break;
#endif
        default:
            /*do nothing*/
            break;
        }
        SchM_Exit_Com_Context();
        /* Check IPdu which this Signal belonged is Active or not */
        uint8 activeEnable = rxIpduStatePtr->RxIpduRTStFlag & Com_RX_ACTIVE_EN;
        if (Com_RX_ACTIVE_EN == activeEnable)
        {
            ret = E_OK;
        }
    }
    else
    {
        ret = COM_BUSY;
    }
    COM_NOUSED(SignalDataPtr);
    return ret;
}
#endif

#if (0u < COM_RXIPDU_NUMBER)
/******************************************************************************/
/*
 * Brief               Receive Rx Pdu handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      RxPduId,PduInfoPtr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE)
    Com_RxPduHandle(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr)
{
    const Com_RxIPduType* rxIpduPtr = &Com_ConfigStd->ComRxIPdu[RxPduId];
    Com_RxIPduRunTimeStateType* rxIpduStatePtr = &Com_RxIPduRunTimeState[RxPduId];

#if (STD_ON == COM_RX_IPDU_CALLOUT_ENABLE)
    boolean ret = TRUE;
    if (NULL_PTR != rxIpduPtr->ComIPduCallout)
    {
        ret = rxIpduPtr->ComIPduCallout(RxPduId, PduInfoPtr);
    }
    if (ret)
#endif /* STD_ON == COM_RX_IPDU_CALLOUT_ENABLE */
    {
        /*signal/signal group RxIndication is made immediate*/
#if (STD_ON == COM_RX_IPDU_SIGNAL_PROCESS_IMMEDIATE_ENABLE)
        if (COM_IMMEDIATE == rxIpduPtr->ComIPduSignalProcessing)
        {
            rxIpduStatePtr->RxIpduRTStFlag &= Com_RX_INDICATION_DIS;
            /*rx indication handle*/
            Com_IndicationProcess(rxIpduStatePtr, rxIpduPtr);
        }
        /*signal/signal group RxIndication is made in next MainFunctionRx*/
        else
#endif /* STD_ON == COM_RX_IPDU_SIGNAL_PROCESS_IMMEDIATE_ENABLE */
        {
            rxIpduStatePtr->RxIpduRTStFlag |= Com_RX_INDICATION_EN;
        }
    }
    COM_NOUSED(PduInfoPtr);
    return;
}
#endif

#if ((0u < COM_RXIPDU_NUMBER) && (COM_RXSIGNALGROUP_NUMBER > 0u) && (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE))
/******************************************************************************/
/*
 * Brief               RxSignalGroup receive timeout handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      rxIpduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE)
    Com_RxSignalGroupTimeOutHandle(Com_RxIPduRunTimeStateType* RxIpduStatePtr, const Com_RxIPduType* RxIpduPtr)
{
    Com_RxDataTimeoutActionType timeoutAction;
#if (STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API)
    uint16 iPduStartBufferId;
    uint16 signalGroupArrayLength;
#endif /*STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API*/
    uint16 substituteBufId = COM_UNUSED_UINT16;
#if (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)
    /* substituteStartId is continuous */
    Com_RxGroupSignalIdType substituteStartId = RxIpduPtr->ComGrpSigSubstituteStartId;
#endif
    const Com_RxSignalGroupType* rxSignalGroupPtr;
    const Com_RxGroupSignalType* rxGroupSignalPtr;
    Com_RxSigGrpTimeoutIdType ipduSignalGroupTimeoutNumber, counter;
    Com_RxSignalGroupIdType rxSignalGroupId;
    Com_RxGroupSignalIdType groupSignalNumber, groupSignalId;
    ipduSignalGroupTimeoutNumber = RxIpduPtr->ComIPduSignalGroupsTimeoutRefNumber;
    counter = RxIpduPtr->ComIPduSignalGroupsTimeoutRefStartId;

    for (; counter < ipduSignalGroupTimeoutNumber; ++counter)
    {
        rxSignalGroupId = Com_TimeoutRxSigGrpRef[counter];
        rxSignalGroupPtr = &Com_ConfigStd->ComRxSignalGroup[rxSignalGroupId];
        if (Com_RxSignalGroupTimeOutCnt[counter] > 0u)
        {
            Com_RxSignalGroupTimeOutCnt[counter] -= 1u;
            if (0u == Com_RxSignalGroupTimeOutCnt[counter])
            {
                groupSignalNumber = rxSignalGroupPtr->ComGroupSignalRefNumber;
                groupSignalId = rxSignalGroupPtr->ComGroupSignalRefStartId;
#if (COM_RXMASKNEWDIFFERMASKOLD_NUMBER > 0u) && (COM_RXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
                for (; groupSignalId < groupSignalNumber; ++groupSignalId)
                {
                    rxGroupSignalPtr = &Com_ConfigStd->ComRxGroupSignal[groupSignalId];
                    if ((COM_UNUSED_RXGRPSIGFILTERID != rxGroupSignalPtr->ComFilterIndex)
                        && (COM_MASKED_NEW_DIFFERS_MASKED_OLD == rxGroupSignalPtr->ComFilterAlgorithm))
                    {
                        Com_MaskNewDifferMaskOldTimeOut[rxGroupSignalPtr->ComFilterIndex] = TRUE;
                    }
                }
#endif
#if (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_REPLACE_ENABLE) \
    || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)
                timeoutAction = Com_RxSigGrpDataTimeoutActionCfg[counter];
                if (COM_TIMEOUT_ACTION_NONE != timeoutAction)
                {
/*replace the rx pdu buffer for signal group array*/
#if (STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API)
                    if (COM_UNUSED_COM_SIGNALGROUPARRYID != rxSignalGroupPtr->ComSignalGroupArrayId)
                    {
                        iPduStartBufferId =
                            Com_SignalGroupArray[rxSignalGroupPtr->ComSignalGroupArrayId].ComIPduStartBufIndex;
                        signalGroupArrayLength =
                            Com_SignalGroupArray[rxSignalGroupPtr->ComSignalGroupArrayId].SignalGroupArrayLength;
                        SchM_Enter_Com_Context();
                        (void)ILib_memcpy(
                            &Com_RxIPduRuntimeBuff[iPduStartBufferId],
                            &Com_RxIPduInitValue[iPduStartBufferId],
                            signalGroupArrayLength);
                        SchM_Exit_Com_Context();
                    }
#endif
                    /*replace the signal group(include all group signals) value with init value*/
                    groupSignalId = rxSignalGroupPtr->ComGroupSignalRefStartId;
                    for (; groupSignalId < groupSignalNumber; ++groupSignalId)
                    {
                        rxGroupSignalPtr = &Com_ConfigStd->ComRxGroupSignal[groupSignalId];
#if (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)
                        substituteBufId = Com_RxGrpSigSubstituteValueIdRef[substituteStartId];
                        substituteStartId++;
#endif
                        Com_RxDataTimeOutActionHandle(
                            RxIpduStatePtr,
                            RxIpduPtr,
                            timeoutAction,
                            (void*)rxGroupSignalPtr,
                            FALSE,
                            substituteBufId);
                    }
                }
#endif /* STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_REPLACE_ENABLE \
    || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_SUBSTITUTE_ENABLE */
#if (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_NOTIFICATION_ENABLE)
                if (NULL_PTR != Com_RxSigGrpTimeoutNotificationCfg[counter])
                {
                    Com_RxSigGrpTimeoutNotificationCfg[counter]();
                }
#endif /* STD_ON == COM_RX_SIG_GROUP_TIMEOUT_NOTIFICATION_ENABLE */
                Com_RxSignalGroupTimeOutCnt[counter] = Com_RxSigGrpTimeoutVal[counter];
            }
        }
    }
}
#endif

/******************************************************************************/
/*
 * Brief               called by Com_RxIpduController.
 *                     init the rx ipdu buffer,all signal buffer(included in
 *                     the ipdu,except source signal).
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      IpduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if (COM_IPDUGROUP_NUMBER > 0u)
/******************************************************************************/
/*
 * Brief               init the rx ipdu buffer,all signal buffer(included in
 *                     the ipdu,except source signal).
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      IpduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE) Com_ResetRxPduBufferAndSignalBuffer(PduIdType IpduId)
{
    const Com_RxIPduType* rxIpduPtr = &Com_ConfigStd->ComRxIPdu[IpduId];
    Com_RxIPduRunTimeStateType* rxIpduStatePtr = &Com_RxIPduRunTimeState[IpduId];
    uint16 signalInitValueId;
    Com_SignalType signalType;
    uint16 signalLength;
    /*init the rx ipdu buffer,all signal buffer(included in the ipdu,except source description signal)*/
#if (0u < COM_RXIPDUBUFF_SIZE)
    Com_RxIpduBufIdType pduBufferId = rxIpduPtr->ComRxIPduBufIndex;
    uint16 initPduLength = Com_RxIPduInitState[IpduId].RxIpduLength;
    SchM_Enter_Com_Context();
    /*init rx pdu buffer*/
    (void)ILib_memcpy(&Com_RxIPduRuntimeBuff[pduBufferId], &Com_RxIPduInitValue[pduBufferId], initPduLength);
    SchM_Exit_Com_Context();
#endif
    /*init signal buffer*/
#if (0u < COM_RXSIGNAL_NUMBER)
    Com_RxSignalIdType ipduSignalRefNumber, cnt;
    const Com_RxSignalType* rxSignalPtr;
    ipduSignalRefNumber = rxIpduPtr->ComIPduSignalsRefNumber;
    cnt = rxIpduPtr->ComIpduSignalRefStartId;
    for (; cnt < ipduSignalRefNumber; ++cnt)
    {
        rxSignalPtr = &Com_ConfigStd->ComRxSignal[cnt];
        signalInitValueId = rxSignalPtr->ComSignalInitValueId;
        signalType = rxSignalPtr->ComSignalType;
#if (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE)
        if (signalType < COM_UINT8_DYN)
#endif /* STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE || STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE */
        {
            signalLength = 0u;
        }
#if (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE)
        else
        {
            signalLength = (COM_UINT8_DYN == signalType) ? (rxSignalPtr->ComSignalDataInitValueLength)
                                                         : (rxSignalPtr->ComSignalLength);
        }
#endif /* STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE || STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE */
        Com_InitSignalBuffer(signalType, signalInitValueId, signalLength);
#if COM_GWMAPPING_NUMBER > 0u
        Com_GwMappingIdType gwMappingId = rxSignalPtr->GwMappingId;
        if (gwMappingId != COM_NOT_USED_GW_MAPPING_ID)
        {
            Com_GwUpdateDestinationRequest(gwMappingId, FALSE);
        }
#endif
    }
#endif
/*init group signal buffer*/
#if ((0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXGROUPSIGNAL_NUMBER))
    Com_RxSignalGroupIdType ipduSignalGroupNumber, counter;
    Com_RxGroupSignalIdType groupSignalNumber, groupSignalId;
    const Com_RxSignalGroupType* rxSignalGroupPtr;
    const Com_RxGroupSignalType* rxGroupSignalPtr;
    ipduSignalGroupNumber = rxIpduPtr->ComIPduSignalGroupsRefNumber;
    counter = rxIpduPtr->ComIPduSignalGroupsRefStartId;
    for (; counter < ipduSignalGroupNumber; ++counter)
    {
        rxSignalGroupPtr = &Com_ConfigStd->ComRxSignalGroup[counter];
        groupSignalNumber = rxSignalGroupPtr->ComGroupSignalRefNumber;
        groupSignalId = rxSignalGroupPtr->ComGroupSignalRefStartId;
        for (; groupSignalId < groupSignalNumber; ++groupSignalId)
        {
            rxGroupSignalPtr = &Com_ConfigStd->ComRxGroupSignal[groupSignalId];
            signalInitValueId = rxGroupSignalPtr->ComSignalInitValueId;
            signalType = rxGroupSignalPtr->ComSignalType;

#if COM_GWMAPPING_NUMBER > 0u
            Com_GwMappingIdType gwMappingId = rxGroupSignalPtr->GwMappingId;
            if (gwMappingId != COM_NOT_USED_GW_MAPPING_ID)
            {
                Com_GwUpdateDestinationRequest(gwMappingId, FALSE);
            }
#endif

#if (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
            if (signalType < COM_UINT8_DYN)
#endif /* STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE */
            {
                signalLength = 0u;
            }
#if (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
            else
            {
                signalLength = (COM_UINT8_DYN == signalType) ? (rxGroupSignalPtr->ComSignalDataInitValueLength)
                                                             : (rxGroupSignalPtr->ComSignalLength);
            }
#endif /* STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE */
            Com_InitSignalBuffer(signalType, signalInitValueId, signalLength);
        }
    }
#endif

#if COM_GW_SOURCE_DESCRIPTION_NUMBER > 0u
    Com_GwSourceDescriptionIdType gwSourceDescriptionNumber = rxIpduPtr->ComIPduSourceSignalsRefNumber;
    Com_GwSourceDescriptionIdType gwSourceDescriptionId = rxIpduPtr->ComIPduSourceSignalsRefStartId;
    for (; gwSourceDescriptionId < gwSourceDescriptionNumber; ++gwSourceDescriptionId)
    {
        const Com_GwSourceSignalType* gwSourceDescriptionPtr = &Com_ConfigStd->ComSourceSignal[gwSourceDescriptionId];
        Com_GwMappingIdType gwMappingId = gwSourceDescriptionPtr->GwMappingId;
        if (gwMappingId != COM_NOT_USED_GW_MAPPING_ID)
        {
            Com_GwUpdateDestinationRequest(gwMappingId, FALSE);
        }
    }
#endif

    rxIpduStatePtr->RxIpduRTStFlag &= Com_RX_RECEIVING_DIS;
    rxIpduStatePtr->RxIpduRTStFlag &= Com_RX_INDICATION_DIS;
    rxIpduStatePtr->RxIpduLength = Com_RxIPduInitState[IpduId].RxIpduLength;
}
#endif

#if (0u < COM_GW_SOURCE_DESCRIPTION_NUMBER) && (COM_GWMAPPING_NUMBER > 0u)
/******************************************************************************/
/*
 * Brief               called by Com_IndicationProcess.
 *                     unpack the rx source description signal to gateway buffer.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      GwSourceSignalPtr,RxDynSignalLength,IpduBufferId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE) Com_SourceSignalUnPack(
    const Com_GwSourceSignalType* GwSourceSignalPtr,
    uint16 RxDynSignalLength,
    Com_RxIpduBufIdType IpduBufferId)
{
#if (0u < COM_RXIPDUBUFF_SIZE)
    uint64 sourceSignalValue;
    uint16 signalBufferId = GwSourceSignalPtr->GWSignalBufferId;
    Com_SignalType signalType = GwSourceSignalPtr->ComSignalType;

#if (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_DYN_ENABLE)
    if ((signalType != COM_UINT8_N) && (signalType != COM_UINT8_DYN))
#endif /* STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_N_ENABLE || STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_DYN_ENABLE \
        */
    {
        sourceSignalValue = Com_SignalUnPackHandle(
#if (STD_ON == COM_SIGNAL_SIGNED_TYPE_ENABLE)
            signalType,
#endif
            COM_RX_DESCRIPTION,
            GwSourceSignalPtr,
            &Com_RxIPduRuntimeBuff[IpduBufferId]);
        SchM_Enter_Com_MultiCore_Lock();
        Com_UpdateGWSignalBuffer(sourceSignalValue, signalBufferId, signalType);
        SchM_Exit_Com_MultiCore_Lock();
    }
#if ((STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_DYN_ENABLE)) \
    && (0u < COM_GWSIGNAL_8BITBUFF_SIZE)
    else
    {
        Com_SignalPositionType sigLsbBytePos = GwSourceSignalPtr->ComSigLsbBytePos;
        Com_RxIpduBufIdType ipduBufferPos = IpduBufferId + (Com_RxIpduBufIdType)sigLsbBytePos;
        /* COM_UINT8_DYN or COM_UINT8_N */
        uint16 signalLength = (COM_UINT8_N == signalType) ? GwSourceSignalPtr->ComSignalLength : RxDynSignalLength;
        SchM_Enter_Com_MultiCore_Lock();
        SchM_Enter_Com_Context();
#if (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_DYN_ENABLE)
        if (COM_UINT8_DYN == signalType)
        {
            /* PRQA S 1340 ++ */ /* MISRA Rule 17.8 */
            (void)ILib_memcpy(&Com_GWSignal8BitBuff[signalBufferId], &RxDynSignalLength, sizeof(RxDynSignalLength));
            /* PRQA S 1340 -- */ /* MISRA Rule 17.8 */
            signalBufferId += (uint16)(sizeof(RxDynSignalLength));
        }
#endif
        (void)ILib_memcpy(&Com_GWSignal8BitBuff[signalBufferId], &Com_RxIPduRuntimeBuff[ipduBufferPos], signalLength);
        SchM_Exit_Com_Context();
        SchM_Exit_Com_MultiCore_Lock();
    }
#endif
#endif

#if COM_GWMAPPING_NUMBER > 0u
    Com_GwMappingIdType gwMappingId = GwSourceSignalPtr->GwMappingId;
    if (gwMappingId != COM_NOT_USED_GW_MAPPING_ID)
    {
        Com_GwUpdateDestinationRequest(gwMappingId, TRUE);
    }
#endif

    COM_NOUSED(RxDynSignalLength);
    COM_NOUSED(IpduBufferId);
    return;
}
#endif /* 0u < COM_GW_SOURCE_DESCRIPTION_NUMBER) && (COM_GWMAPPING_NUMBER > 0u */

#if (0u < COM_RXSIGNAL_NUMBER)
/******************************************************************************/
/*
 * Brief               called by Com_IndicationProcess.
 *                     unpack the rx signal and notification up layer.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      RxSignalPtr,RxDynSignalLength,IpduBufferId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE) Com_SignalRxIndication(
    const Com_RxSignalType* RxSignalPtr,
    uint16 RxDynSignalLength,
    Com_RxIpduBufIdType IpduBufferId)
{
#if (0u < COM_RXIPDUBUFF_SIZE)
    Com_SignalType signalType = RxSignalPtr->ComSignalType;
#if (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE)
    if ((signalType != COM_UINT8_N) && (signalType != COM_UINT8_DYN))
#endif /* STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE */
    {
        Com_RxSignalHandle(RxSignalPtr);
    }
#if (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE)
    else
    {
        /* COM_UINT8_DYN or COM_UINT8_N */
        uint16 signalLength = (COM_UINT8_N == signalType) ? RxSignalPtr->ComSignalLength : RxDynSignalLength;
#if (0u < COM_GWSIGNAL_8BITBUFF_SIZE) && (COM_GWMAPPING_NUMBER > 0u)
        uint16 gwSignalBufferId = RxSignalPtr->GWSignalBufferId;
        Com_RxIpduBufIdType ipduBufferPos = IpduBufferId + (Com_RxIpduBufIdType)(RxSignalPtr->ComSigLsbBytePos);

        /*the rx signal need to be gateway,then store the value to gateway buffer*/
        if (COM_UNUSED_UINT16 != gwSignalBufferId)
        {
            SchM_Enter_Com_MultiCore_Lock();
            SchM_Enter_Com_Context();
            if (COM_UINT8_DYN == signalType)
            {
                /* PRQA S 1340 ++ */ /* MISRA Rule 17.8 */
                (void)
                    ILib_memcpy(&Com_GWSignal8BitBuff[gwSignalBufferId], &RxDynSignalLength, sizeof(RxDynSignalLength));
                /* PRQA S 1340 -- */ /* MISRA Rule 17.8 */
                gwSignalBufferId += (uint16)(sizeof(RxDynSignalLength));
            }
            (void)ILib_memcpy(
                &Com_GWSignal8BitBuff[gwSignalBufferId],
                &Com_RxIPduRuntimeBuff[ipduBufferPos],
                signalLength);
            SchM_Exit_Com_Context();
            SchM_Exit_Com_MultiCore_Lock();
        }
#endif /* 0u < COM_GWSIGNAL_8BITBUFF_SIZE) && (COM_GWMAPPING_NUMBER > 0u */
#if (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE)
        if (COM_UINT8_N == signalType)
        {
            Com_Rx8NSignalHandle(RxSignalPtr, signalLength, IpduBufferId);
        }
#endif /* STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE */
#if (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE)
        if (COM_UINT8_DYN == signalType)
        {
            Com_Rx8DYNSignalHandle(RxSignalPtr, signalLength, IpduBufferId);
        }
#endif
    }
#endif
#endif

#if COM_GWMAPPING_NUMBER > 0u
    Com_GwMappingIdType gwMappingId = RxSignalPtr->GwMappingId;
    if (gwMappingId != COM_NOT_USED_GW_MAPPING_ID)
    {
        Com_GwUpdateDestinationRequest(gwMappingId, TRUE);
    }
#endif
}
#endif

#if ((0u < COM_RXSIGNAL_NUMBER) && (0u < COM_RXIPDUBUFF_SIZE) && (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE))
/******************************************************************************/
/*
 * Brief               called by Com_SignalRxIndication.
 *                     unpack the rx signal(signal type is COM_UINT8_N)
 *                     and notification up layer.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      RxSignalPtr,SignalLength,IpduBufferId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE)
    Com_Rx8NSignalHandle(const Com_RxSignalType* RxSignalPtr, uint16 SignalLength, Com_RxIpduBufIdType IpduBufferId)
{
    Com_RxIpduBufIdType ipduBufferPos = IpduBufferId + (Com_RxIpduBufIdType)(RxSignalPtr->ComSigLsbBytePos);
#if (0u < COM_SIGNAL_8BITBUFF_SIZE)
    uint16 initValueId = RxSignalPtr->ComSignalInitValueId;
#endif /* 0u < COM_SIGNAL_8BITBUFF_SIZE */
       /*the rx signal configuration invalid value*/
#if (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ENABLE)
    boolean ret = TRUE;
    Com_RxSignalInvalidIdType signalInvalidId = RxSignalPtr->ComSignalInvalidRefId;
    if (COM_UNUSED_RXSIGNALINVALIDID != signalInvalidId)
    {
        uint16 invalidValueId = Com_RxSignalDataInvalidValueIdRef[signalInvalidId];
#if (0u < COM_SIGNAL_8BIT_INVALID_SIZE)
        if (0
            == ILib_memcmp(
                &Com_Signal8BitInvalidValue[invalidValueId],
                &Com_RxIPduRuntimeBuff[ipduBufferPos],
                SignalLength))
#endif
        {
#if (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ACTION_NOTIFY_ENABLE)
            if (COM_INVALID_ACTION_NOTIFY == Com_RxSignalDataInvalidActionRef[signalInvalidId])
            {
                if (NULL_PTR != Com_RxInvalidNotificationCfg[signalInvalidId])
                {
                    Com_RxInvalidNotificationCfg[signalInvalidId]();
                }
            }
            else
#endif /* STD_ON == COM_RX_SIGNAL_INVALID_DATA_ACTION_NOTIFY_ENABLE */
            {
#if (COM_RXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)
                if ((COM_UNUSED_RXSIGNALFILTERID == RxSignalPtr->ComFilterIndex)
                    || (COM_ALWAYS == RxSignalPtr->ComFilterAlgorithm))
#endif
                {
#if (0u < COM_SIGNAL_8BITBUFF_SIZE)
                    SchM_Enter_Com_Context();
                    (void)ILib_memcpy(
                        &Com_Signal8BitRuntimeBuff[initValueId],
                        &Com_Signal8BitInitValue[initValueId],
                        SignalLength);
                    SchM_Exit_Com_Context();
#endif
#if (STD_ON == COM_RX_SIGNAL_NOTIFICATION_ENABLE)
                    if (NULL_PTR != RxSignalPtr->ComNotification)
                    {
                        RxSignalPtr->ComNotification();
                    }
#endif /* STD_ON == COM_RX_SIGNAL_NOTIFICATION_ENABLE */
                }
            }
            ret = FALSE;
        }
    }
    if (ret)
#endif /* STD_ON == COM_RX_SIGNAL_INVALID_DATA_ENABLE */
    {
#if (COM_RXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)
        if ((COM_UNUSED_RXSIGNALFILTERID == RxSignalPtr->ComFilterIndex)
            || (COM_ALWAYS == RxSignalPtr->ComFilterAlgorithm))
#endif
        {
#if (0u < COM_SIGNAL_8BITBUFF_SIZE)
            SchM_Enter_Com_Context();
            (void)ILib_memcpy(
                &Com_Signal8BitRuntimeBuff[initValueId],
                &Com_RxIPduRuntimeBuff[ipduBufferPos],
                SignalLength);
            SchM_Exit_Com_Context();
#endif
#if (STD_ON == COM_RX_SIGNAL_NOTIFICATION_ENABLE)
            if (NULL_PTR != RxSignalPtr->ComNotification)
            {
                RxSignalPtr->ComNotification();
            }
#endif /* STD_ON == COM_RX_SIGNAL_NOTIFICATION_ENABLE */
        }
    }
    COM_NOUSED(SignalLength);
    return;
}
#endif

#if ((0u < COM_RXSIGNAL_NUMBER) && (0u < COM_RXIPDUBUFF_SIZE) && (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE))
/******************************************************************************/
/*
 * Brief               called by Com_SignalRxIndication.
 *                     unpack the rx signal(signal type is COM_UINT8_DYN)
 *                     and notification up layer.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      RxSignalPtr,SignalLength,IpduBufferId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE)
    Com_Rx8DYNSignalHandle(const Com_RxSignalType* RxSignalPtr, uint16 SignalLength, Com_RxIpduBufIdType IpduBufferId)
{
#if (0u < COM_RX_SIGNAL_8BIT_INVALIDVALUE_NUMBER)
    Com_RxIPduRunTimeStateType* rxIpduStatePtr = &Com_RxIPduRunTimeState[RxSignalPtr->ComIpduRefIndex];
    boolean ret = TRUE;
    Com_RxSignalInvalidIdType signalInvalidId = RxSignalPtr->ComSignalInvalidRefId;
    uint16 initValueLength = RxSignalPtr->ComSignalDataInitValueLength;
#endif
    Com_RxIpduBufIdType ipduBufferPos = IpduBufferId + (Com_RxIpduBufIdType)(RxSignalPtr->ComSigLsbBytePos);
#if (0u < COM_SIGNAL_8BITBUFF_SIZE)
    uint16 initValueId = RxSignalPtr->ComSignalInitValueId;
#endif

    /*the rx signal configuration invalid value*/
#if (0u < COM_RX_SIGNAL_8BIT_INVALIDVALUE_NUMBER)
    if (COM_UNUSED_RXSIGNALINVALIDID != signalInvalidId)
    {
        if (SignalLength == Com_RxSignalDataInvalidValueLengthRef[signalInvalidId])
        {
            uint16 invalidValueId = Com_RxSignalDataInvalidValueIdRef[signalInvalidId];
#if (0u < COM_SIGNAL_8BIT_INVALID_SIZE)
            if (0
                == ILib_memcmp(
                    &Com_Signal8BitInvalidValue[invalidValueId],
                    &Com_RxIPduRuntimeBuff[ipduBufferPos],
                    SignalLength))
#endif
            {
#if (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ACTION_NOTIFY_ENABLE)
                if (COM_INVALID_ACTION_NOTIFY == Com_RxSignalDataInvalidActionRef[signalInvalidId])
                {
                    if (NULL_PTR != Com_RxInvalidNotificationCfg[signalInvalidId])
                    {
                        Com_RxInvalidNotificationCfg[signalInvalidId]();
                    }
                }
                else
#endif /* STD_ON == COM_RX_SIGNAL_INVALID_DATA_ACTION_NOTIFY_ENABLE */
                {
#if (COM_RXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)
                    if ((COM_UNUSED_RXSIGNALFILTERID == RxSignalPtr->ComFilterIndex)
                        || (COM_ALWAYS == RxSignalPtr->ComFilterAlgorithm))
#endif
                    {
#if (0u < COM_SIGNAL_8BITBUFF_SIZE)
                        SchM_Enter_Com_Context();
                        (void)ILib_memcpy(
                            &Com_Signal8BitRuntimeBuff[initValueId],
                            &Com_Signal8BitInitValue[initValueId],
                            initValueLength);
                        SchM_Exit_Com_Context();
#endif
                        /*change the rx Ipdu length to Init rx Ipdu length*/
                        uint16 rxIpduLength = rxIpduStatePtr->RxIpduLength - SignalLength + initValueLength;
                        rxIpduStatePtr->RxIpduLength = rxIpduLength;
#if (STD_ON == COM_RX_SIGNAL_NOTIFICATION_ENABLE)
                        if (NULL_PTR != RxSignalPtr->ComNotification)
                        {
                            RxSignalPtr->ComNotification();
                        }
#endif /* STD_ON == COM_RX_SIGNAL_NOTIFICATION_ENABLE */
                    }
                }
                ret = FALSE;
            }
        }
    }
    if (ret)
#endif
    {
#if (COM_RXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)
        if ((COM_UNUSED_RXSIGNALFILTERID == RxSignalPtr->ComFilterIndex)
            || (COM_ALWAYS == RxSignalPtr->ComFilterAlgorithm))
#endif
        {
#if (0u < COM_SIGNAL_8BITBUFF_SIZE)
            SchM_Enter_Com_Context();
            (void)ILib_memcpy(
                &Com_Signal8BitRuntimeBuff[initValueId],
                &Com_RxIPduRuntimeBuff[ipduBufferPos],
                SignalLength);
            SchM_Exit_Com_Context();
#endif
#if (STD_ON == COM_RX_SIGNAL_NOTIFICATION_ENABLE)
            if (NULL_PTR != RxSignalPtr->ComNotification)
            {
                RxSignalPtr->ComNotification();
            }
#endif /* STD_ON == COM_RX_SIGNAL_NOTIFICATION_ENABLE */
        }
    }
    COM_NOUSED(SignalLength);
    return;
}
#endif

#if ((0u < COM_RXSIGNAL_NUMBER) && (0u < COM_RXIPDUBUFF_SIZE))
/******************************************************************************/
/*
 * Brief               called by Com_SignalRxIndication.
 *                     unpack the rx signal(signal type isn't COM_UINT8_N or
 *                     COM_UINT8_DYN) and notification up layer.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      RxSignalPtr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE) Com_RxSignalHandle(const Com_RxSignalType* RxSignalPtr)
{
    uint16 initValueId = RxSignalPtr->ComSignalInitValueId;
    Com_SignalType signalType = RxSignalPtr->ComSignalType;
    /* unpack the signal value from rx ipdu buffer, and do B-End and L-End exchanging,Flag Extension
       and return the result as an uint64 value to variable 'unPackValue'. */
    Com_RxIpduBufIdType ipduBufferIndex = Com_ConfigStd->ComRxIPdu[RxSignalPtr->ComIpduRefIndex].ComRxIPduBufIndex;
    uint64 unPackValue = Com_SignalUnPackHandle(
#if (STD_ON == COM_SIGNAL_SIGNED_TYPE_ENABLE)
        signalType,
#endif
        COM_RX_SIGNAL,
        RxSignalPtr,
        &Com_RxIPduRuntimeBuff[ipduBufferIndex]);
/* update signal value into GW signal buffer */
#if (COM_GWMAPPING_NUMBER > 0u)
    uint16 gwSignalBufferId = RxSignalPtr->GWSignalBufferId;
    SchM_Enter_Com_MultiCore_Lock();
    Com_UpdateGWSignalBuffer(unPackValue, gwSignalBufferId, signalType);
    SchM_Exit_Com_MultiCore_Lock();
#endif /* COM_GWMAPPING_NUMBER > 0u */

/*the rx signal configuration invalid value*/
#if (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ENABLE)
    Com_RxSignalInvalidIdType signalInvalidId = RxSignalPtr->ComSignalInvalidRefId;
    boolean needFilter = TRUE;
    if (COM_UNUSED_RXSIGNALINVALIDID != signalInvalidId)
    {
        uint16 invalidValueId = Com_RxSignalDataInvalidValueIdRef[signalInvalidId];
        if (Com_SignalInvalidateHandle(signalType, invalidValueId, unPackValue))
        {
#if (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ACTION_NOTIFY_ENABLE)
            if (COM_INVALID_ACTION_NOTIFY == Com_RxSignalDataInvalidActionRef[signalInvalidId])
            {
                if (NULL_PTR != Com_RxInvalidNotificationCfg[signalInvalidId])
                {
                    Com_RxInvalidNotificationCfg[signalInvalidId]();
                }
                needFilter = FALSE;
            }
            else
#endif /* STD_ON == COM_RX_SIGNAL_INVALID_DATA_ACTION_NOTIFY_ENABLE */
            {
                /*replace signal runtime buffer, with init value. */
                unPackValue = Com_RxSignalReplaceHanlde(signalType, initValueId);
            }
        }
    }
    if (needFilter)
#endif /* STD_ON == COM_RX_SIGNAL_INVALID_DATA_ENABLE */
    {
#if (COM_RXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)
        boolean filterPass = Com_RxSignalFilter(
            signalType,
            RxSignalPtr->ComFilterAlgorithm,
            (Com_SignalIdType)RxSignalPtr->ComFilterIndex,
#if (COM_RXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
            initValueId,
#endif
            unPackValue);

        if (filterPass)
#endif /* COM_RXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u */
        {
            /* update signal value into signal runtime buffer */
            Com_RxSignalBuffHandle(unPackValue, signalType, initValueId);
/* if this signal has normal rx notification function configured,
   invoke this function. */
#if (STD_ON == COM_RX_SIGNAL_NOTIFICATION_ENABLE)
            if (NULL_PTR != RxSignalPtr->ComNotification)
            {
                RxSignalPtr->ComNotification();
            }
#endif /* STD_ON == COM_RX_SIGNAL_NOTIFICATION_ENABLE */
        }
    }
    return;
}
#endif

#if ((STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE) || (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ENABLE))
/******************************************************************************/
/*
 * Brief               called by Com_RxSignalHandle,Com_SignalGroupRxIndication.
 *                     judge the rx (group) signal value is invalid value or not.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalType,InvalidValueId,Value
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/******************************************************************************/
static FUNC(boolean, COM_CODE)
    Com_SignalInvalidateHandle(Com_SignalType SignalType, uint16 InvalidValueId, uint64 Value)
{
    boolean ret = FALSE;
    switch (SignalType)
    {
#if (0u < COM_SIGNAL_BOOL_INVALID_SIZE)
    case COM_BOOLEAN:
        if (((boolean)Value) == Com_SignalBoolInvalidValue[InvalidValueId])
        {
            ret = TRUE;
        }
        break;
#endif
#if (0u < COM_SIGNAL_8BIT_INVALID_SIZE)
    case COM_SINT8:
    case COM_UINT8:
        if (((uint8)Value) == Com_Signal8BitInvalidValue[InvalidValueId])
        {
            ret = TRUE;
        }
        break;
#endif
#if (0u < COM_SIGNAL_16BIT_INVALID_SIZE)
    case COM_SINT16:
    case COM_UINT16:
        if (((uint16)Value) == Com_Signal16BitInvalidValue[InvalidValueId])
        {
            ret = TRUE;
        }
        break;
#endif
#if (0u < COM_SIGNAL_32BIT_INVALID_SIZE)
    case COM_SINT32:
    case COM_UINT32:
    case COM_FLOAT32:
        if (((uint32)Value) == Com_Signal32BitInvalidValue[InvalidValueId])
        {
            ret = TRUE;
        }
        break;
#endif
#if (0u < COM_SIGNAL_64BIT_INVALID_SIZE)
    case COM_SINT64:
    case COM_UINT64:
    case COM_FLOAT64:
        if (Value == Com_Signal64BitInvalidValue[InvalidValueId])
        {
            ret = TRUE;
        }
        break;
#endif
    default:
        /*do nothing*/
        break;
    }
    COM_NOUSED(InvalidValueId);
    COM_NOUSED(Value);
    return ret;
}
#endif

#if (                                                                                      \
    ((0u < COM_RXGROUPSIGNAL_NUMBER) && (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE)) \
    || ((0u < COM_RXSIGNAL_NUMBER) && (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ENABLE)))
/******************************************************************************/
/*
 * Brief               called by Com_RxSignalHandle,Com_InvalidSignalReplaceAndNotification,
 *                     Com_RxSignalGroupFilter.
 *                     get the rx (group) signal's init value.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalType,InitValueId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint64
 */
/******************************************************************************/
static FUNC(uint64, COM_CODE) Com_RxSignalReplaceHanlde(Com_SignalType SignalType, uint16 InitValueId)
{
    uint64 unPackValue = COM_UNUSED_UINT64;
    /* PRQA S 3315 ++ */ /* MISRA Rule 16.6 */
    switch (SignalType)
    {
#if (0u < COM_SIGNAL_BOOLBUFF_SIZE)
    case COM_BOOLEAN:
        /* PRQA S 4304 ++ */ /* MISRA Rule 10.5 */
        unPackValue = (uint64)Com_SignalBoolInitValue[InitValueId];
        /* PRQA S 4304 -- */ /* MISRA Rule 10.5 */
        break;
#endif
#if (0u < COM_SIGNAL_8BITBUFF_SIZE)
    case COM_SINT8:
    case COM_UINT8:
        unPackValue = (uint64)Com_Signal8BitInitValue[InitValueId];
        break;
#endif
#if (0u < COM_SIGNAL_16BITBUFF_SIZE)
    case COM_SINT16:
    case COM_UINT16:
        unPackValue = (uint64)Com_Signal16BitInitValue[InitValueId];
        break;
#endif
#if (0u < COM_SIGNAL_32BITBUFF_SIZE)
    case COM_SINT32:
    case COM_UINT32:
    case COM_FLOAT32:
        unPackValue = (uint64)Com_Signal32BitInitValue[InitValueId];
        break;
#endif
#if (0u < COM_SIGNAL_64BITBUFF_SIZE)
    case COM_SINT64:
    case COM_UINT64:
    case COM_FLOAT64:
        unPackValue = Com_Signal64BitInitValue[InitValueId];
        break;
#endif
    default:
        /*do nothing*/
        break;
    }
    /* PRQA S 3315 -- */ /* MISRA Rule 16.6 */
    COM_NOUSED(InitValueId);
    return unPackValue;
}
#endif

#if (0u < COM_RXSIGNAL_NUMBER) || (0u < COM_RXGROUPSIGNAL_NUMBER)
/******************************************************************************/
/*
 * Brief               called by Com_RxSignalHandle,
 *                     Com_InvalidSignalReplaceAndNotification.
 *                     update the rx signal value buffer.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Value,SignalType,InitValueId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE) Com_RxSignalBuffHandle(uint64 Value, Com_SignalType SignalType, uint16 InitValueId)
{
    switch (SignalType)
    {
#if (0u < COM_SIGNAL_BOOLBUFF_SIZE)
    case COM_BOOLEAN:
        /* PRQA S 4340 ++ */ /* MISRA Rule 10.5 */
        Com_SignalBoolRuntimeBuff[InitValueId] = (boolean)Value;
        /* PRQA S 4340 -- */ /* MISRA Rule 10.5 */
        break;
#endif
#if (0u < COM_SIGNAL_8BITBUFF_SIZE)
    case COM_SINT8:
    case COM_UINT8:
        Com_Signal8BitRuntimeBuff[InitValueId] = (uint8)Value;
        break;
#endif
#if (0u < COM_SIGNAL_16BITBUFF_SIZE)
    case COM_SINT16:
    case COM_UINT16:
        Com_Signal16BitRuntimeBuff[InitValueId] = (uint16)Value;
        break;
#endif
#if (0u < COM_SIGNAL_32BITBUFF_SIZE)
    case COM_SINT32:
    case COM_UINT32:
    case COM_FLOAT32:
        Com_Signal32BitRuntimeBuff[InitValueId] = (uint32)Value;
        break;
#endif
#if (0u < COM_SIGNAL_64BITBUFF_SIZE)
    case COM_SINT64:
    case COM_UINT64:
    case COM_FLOAT64:
        Com_Signal64BitRuntimeBuff[InitValueId] = (uint64)Value;
        break;
#endif
    default:
        /*do nothing*/
        break;
    }
    COM_NOUSED(Value);
    COM_NOUSED(InitValueId);
    return;
}
#endif

#if (((0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXGROUPSIGNAL_NUMBER)) || (0u < COM_RXSIGNAL_NUMBER)) \
    && (COM_GWMAPPING_NUMBER > 0u)
/******************************************************************************/
/*
 * Brief               called by Com_RxSignalHandle,Com_SignalGroupRxIndication.
 *                     update the gateway signal value buffer of the rx signal.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Value,GWSignalBufferId,SignalType
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE) Com_UpdateGWSignalBuffer(uint64 Value, uint16 GWSignalBufferId, Com_SignalType SignalType)
{
    /*the rx signal need gateway,store signal value in gateway buffer*/
    if (COM_UNUSED_UINT16 != GWSignalBufferId)
    {
        switch (SignalType)
        {
#if (0u < COM_GWSIGNAL_BOOLBUFF_SIZE)
        case COM_BOOLEAN:
            Com_GWSignalBoolBuff[GWSignalBufferId] = Value != 0u;
            break;
#endif
#if (0u < COM_GWSIGNAL_8BITBUFF_SIZE)
        case COM_SINT8:
        case COM_UINT8:
            Com_GWSignal8BitBuff[GWSignalBufferId] = (uint8)Value;
            break;
#endif
#if (0u < COM_GWSIGNAL_16BITBUFF_SIZE)
        case COM_SINT16:
        case COM_UINT16:
            Com_GWSignal16BitBuff[GWSignalBufferId] = (uint16)Value;
            break;
#endif
#if (0u < COM_GWSIGNAL_32BITBUFF_SIZE)
        case COM_SINT32:
        case COM_UINT32:
        case COM_FLOAT32:
            Com_GWSignal32BitBuff[GWSignalBufferId] = (uint32)Value;
            break;
#endif
#if (0u < COM_GWSIGNAL_64BITBUFF_SIZE)
        case COM_SINT64:
        case COM_UINT64:
        case COM_FLOAT64:
            Com_GWSignal64BitBuff[GWSignalBufferId] = (uint64)Value;
            break;
#endif
        default:
            /*do nothing*/
            break;
        }
    }
    COM_NOUSED(Value);
    return;
}
#endif

#if ((0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXGROUPSIGNAL_NUMBER))
/******************************************************************************/
/*
 * Brief               called by Com_IndicationProcess.
 *                     handle signal group RxIndication of one rx pdu.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      RxSignalGroupPtr,RxDynSignalLength,IpduBufferId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE) Com_SignalGroupRxIndication(
    const Com_RxSignalGroupType* RxSignalGroupPtr,
    uint16 RxDynSignalLength,
    Com_RxIpduBufIdType IpduBufferId)
{
    boolean normalRxIndication = FALSE;
    /*if the signal group have invalid signal or not, and update gwGroupSignal buffer*/
    boolean signalGroupInvalid = Com_SignalGroupInvalidCalculate(RxSignalGroupPtr, IpduBufferId, RxDynSignalLength);
/*the signal group is invalid if at least one group signal is invalid*/
#if (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE)
    if (signalGroupInvalid)
    {
#if (STD_ON == COM_RX_SIG_GROUP_INVALID_DATA_ACTION_NOTIFY_ENABLE)
        if ((COM_INVALID_ACTION_NOTIFY == RxSignalGroupPtr->ComDataInvalidAction)
            && (NULL_PTR != RxSignalGroupPtr->ComInvalidNotification))
        {
            RxSignalGroupPtr->ComInvalidNotification();
        }
#endif
        if (COM_INVALID_ACTION_REPLACE == RxSignalGroupPtr->ComDataInvalidAction)
        {
            normalRxIndication = TRUE;
/*replace the rx pdu buffer for signal group array*/
#if ((STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && (0u < COM_RXIPDUBUFF_SIZE))
            if (COM_UNUSED_COM_SIGNALGROUPARRYID != RxSignalGroupPtr->ComSignalGroupArrayId)
            {
                uint16 iPduSignalGroupArrayStartBufId =
                    Com_SignalGroupArray[RxSignalGroupPtr->ComSignalGroupArrayId].ComIPduStartBufIndex;
                uint16 signalGroupArrayLength =
                    Com_SignalGroupArray[RxSignalGroupPtr->ComSignalGroupArrayId].SignalGroupArrayLength;
                SchM_Enter_Com_Context();
                (void)ILib_memcpy(
                    &Com_RxIPduRuntimeBuff[iPduSignalGroupArrayStartBufId],
                    &Com_RxIPduInitValue[iPduSignalGroupArrayStartBufId],
                    signalGroupArrayLength);
                SchM_Exit_Com_Context();
            }
#endif
        }
    }
#endif
    if ((!signalGroupInvalid) || (normalRxIndication == TRUE))
    {
#if (COM_RXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
        if (Com_RxSignalGroupFilter(RxSignalGroupPtr, signalGroupInvalid, IpduBufferId))
#endif
        {
            Com_RxGroupSignalIdType groupSignalNumber = RxSignalGroupPtr->ComGroupSignalRefNumber;
            Com_RxGroupSignalIdType counter = RxSignalGroupPtr->ComGroupSignalRefStartId;
            /* update all group signal init value into group signal runtime buffer */
            for (; counter < groupSignalNumber; ++counter)
            {
                Com_RxGroupSignalBuffHanlde(signalGroupInvalid, counter, IpduBufferId, RxDynSignalLength);
            }
            /* if this group signal has normal rx notification function configured,
            invoke this function. */
#if (STD_ON == COM_RX_SIG_GROUP_NOTIFICATION_ENABLE)
            if (NULL_PTR != RxSignalGroupPtr->ComNotification)
            {
                RxSignalGroupPtr->ComNotification();
            }
#endif /* STD_ON == COM_RX_SIG_GROUP_NOTIFICATION_ENABLE */
        }
    }

    return;
}
#endif

#if ((0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXGROUPSIGNAL_NUMBER))
/******************************************************************************/
/*
 * Brief               called by Com_SignalGroupRxIndication,Com_TpPduInvalidAction.
 *                     update the rx group signal value buffer.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      InvalidSignalGroup,Id,IpduBufferId,RxDynSignalLength
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE) Com_RxGroupSignalBuffHanlde(
    /* PRQA S 3206 ++ */ /* MISRA Rule 2.7 */
    boolean InvalidSignalGroup,
    /* PRQA S 3206 -- */ /* MISRA Rule 2.7 */
    Com_RxGroupSignalIdType Id,
    Com_RxIpduBufIdType IpduBufferId,
    uint16 RxDynSignalLength)
{
    const Com_RxGroupSignalType* rxGroupSignalPtr = &Com_ConfigStd->ComRxGroupSignal[Id];
    uint16 initValueId = rxGroupSignalPtr->ComSignalInitValueId;
    Com_SignalType signalType = rxGroupSignalPtr->ComSignalType;
    uint16 SignalLength;

    /*rx signal group is invalid,use the init value update the rx group signal buffer*/
#if (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE)
    if (InvalidSignalGroup)
    {
        SignalLength = 0u;
#if (0u < COM_SIGNAL_8BITBUFF_SIZE)
#if (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE)
        if (COM_UINT8_N == signalType)
        {
            SignalLength = rxGroupSignalPtr->ComSignalLength;
        }
#endif /* STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE */
#if (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
        if (COM_UINT8_DYN == signalType)
        {
            SignalLength = rxGroupSignalPtr->ComSignalDataInitValueLength;
            /*change the rx Ipdu length to Init rx Ipdu length*/
            Com_RxSignalGroupIdType signalGroupId = rxGroupSignalPtr->ComSignalGroupRef;
            PduIdType rxIpduId = Com_ConfigStd->ComRxSignalGroup[signalGroupId].ComIpduRefIndex;
            uint16 rxIpduMinLength = (uint16)Com_ConfigStd->ComRxIPdu[rxIpduId].IPduNoDynSignalLength;
            Com_RxIPduRunTimeState[rxIpduId].RxIpduLength = rxIpduMinLength + SignalLength;
        }
#endif /* STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE */
#endif /*0u < COM_SIGNAL_8BITBUFF_SIZE*/
        Com_InitSignalBuffer(signalType, initValueId, SignalLength);
    }
    /*rx signal group is valid,use the really rx value update the rx group signal buffer*/
    else
#endif /* STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE */
    {
#if (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
        if ((COM_UINT8_N != signalType) && (COM_UINT8_DYN != signalType))
#endif /* STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE || STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE */
        {
            uint64 value = Com_SignalUnPackHandle(
#if (STD_ON == COM_SIGNAL_SIGNED_TYPE_ENABLE)
                signalType,
#endif
                COM_RX_GROUP_SIGNAL,
                rxGroupSignalPtr,
                &Com_RxIPduRuntimeBuff[IpduBufferId]);
            Com_RxSignalBuffHandle(value, signalType, initValueId);
        }
#if ((0u < COM_SIGNAL_8BITBUFF_SIZE) && (0u < COM_RXIPDUBUFF_SIZE))
#if (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
        else
        {
            Com_RxIpduBufIdType ipduBufferPos =
                IpduBufferId + (Com_RxIpduBufIdType)(rxGroupSignalPtr->ComSigLsbBytePos);
            SignalLength = (COM_UINT8_N == signalType) ? rxGroupSignalPtr->ComSignalLength : RxDynSignalLength;
            SchM_Enter_Com_Context();
            (void)ILib_memcpy(
                &Com_Signal8BitRuntimeBuff[initValueId],
                &Com_RxIPduRuntimeBuff[ipduBufferPos],
                SignalLength);
            SchM_Exit_Com_Context();
        }
#endif /* STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE || STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE */
#endif /* 0u < COM_SIGNAL_8BITBUFF_SIZE && 0u < COM_RXIPDUBUFF_SIZE */
    }
    COM_NOUSED(RxDynSignalLength);
    COM_NOUSED(InvalidSignalGroup);
    return;
}
#endif

#if ((STD_ON == COM_RXTPPDU_SUPPORT) && (0u < COM_RXSIGNAL_NUMBER) && (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ENABLE))
/******************************************************************************/
/*
 * Brief               called by Com_TpPduInvalidAction.
 *                     rx signal value replace,and notification.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      RxSignalPtr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE) Com_InvalidSignalReplaceAndNotification(const Com_RxSignalType* RxSignalPtr)
{
    uint16 initValueId = RxSignalPtr->ComSignalInitValueId;
    Com_SignalType signalType = RxSignalPtr->ComSignalType;

#if (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE)
    if ((COM_UINT8_N != signalType) && (COM_UINT8_DYN != signalType))
#endif /* STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE || STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE */
    {
        /*replace signal runtime buffer, with init value. */
        uint64 initValue = Com_RxSignalReplaceHanlde(signalType, initValueId);
#if (COM_RXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)
        if (Com_RxSignalFilter(
                signalType,
                RxSignalPtr->ComFilterAlgorithm,
                (Com_SignalIdType)RxSignalPtr->ComFilterIndex,
#if (COM_RXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
                initValueId,
#endif
                initValue))
#endif /* COM_RXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u */
        {
            /* update signal value into signal runtime buffer */
            Com_RxSignalBuffHandle(initValue, signalType, initValueId);
#if (STD_ON == COM_RX_SIGNAL_NOTIFICATION_ENABLE)
            if (NULL_PTR != RxSignalPtr->ComNotification)
            {
                RxSignalPtr->ComNotification();
            }
#endif /* STD_ON == COM_RX_SIGNAL_NOTIFICATION_ENABLE */
        }
    }
    /*COM_UINT8_N or COM_UINT8_DYN*/
#if (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE)
    else
    {
#if (COM_RXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)
        if ((COM_UNUSED_RXSIGNALFILTERID == RxSignalPtr->ComFilterIndex)
            || (COM_ALWAYS == RxSignalPtr->ComFilterAlgorithm))
#endif /* COM_RXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u */
        {
#if (0u < COM_SIGNAL_8BITBUFF_SIZE)
            uint16 signalLength =
                (COM_UINT8_N == signalType) ? RxSignalPtr->ComSignalLength : RxSignalPtr->ComSignalDataInitValueLength;
            SchM_Enter_Com_Context();
            (void)ILib_memcpy(
                &Com_Signal8BitRuntimeBuff[initValueId],
                &Com_Signal8BitInitValue[initValueId],
                signalLength);
            SchM_Exit_Com_Context();
#endif /* 0u < COM_SIGNAL_8BITBUFF_SIZE */
#if (STD_ON == COM_RX_SIGNAL_NOTIFICATION_ENABLE)
            if (NULL_PTR != RxSignalPtr->ComNotification)
            {
                RxSignalPtr->ComNotification();
            }
#endif /* STD_ON == COM_RX_SIGNAL_NOTIFICATION_ENABLE */
        }
    }
#endif /* STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE || STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE */

    return;
}
#endif

#if (                                                                         \
    ((0u < COM_RXSIGNAL_NUMBER) && (COM_RXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)) \
    || ((0u < COM_RXGROUPSIGNAL_NUMBER) && (COM_RXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)))
/******************************************************************************/
/*
 * Brief               called by Com_RxSignalHandle,Com_InvalidSignalReplaceAndNotification,
 *                     Com_RxSignalGroupFilter.
 *                     calculate the rx signal value is filter out or not.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Value,SignalBufferId,SignalType
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/******************************************************************************/
static FUNC(boolean, COM_CODE) Com_RxSignalFilter(
    Com_SignalType SignalType,
    Com_FilterAlgorithmType FilterType,
    Com_SignalIdType ComFilterIndex,
#if (COM_RXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
    uint16 SignalBufferId,
#endif
    uint64 NewSignalValue)
{
    boolean ret = TRUE;
    Com_FilterMaskType ComFilterMask;
    Com_FilterXType ComFilterX;
    Com_FilterMaxType ComFilterMax;
    Com_FilterMinType ComFilterMin;
#if (COM_ONEEVERYNFILTERSIGNAL_NUMBER > 0u)
    Com_SignalIdType ComFilterOffset;
    Com_SignalIdType ComFilterPeriod;
#endif
    if (COM_FILTER_NONE != FilterType)
    {
        ret = FALSE;
        switch (FilterType)
        {
        case COM_ALWAYS:
            ret = TRUE;
            break;
#if (COM_RXMASKNEWDIFFERX_NUMBER > 0u)
        case COM_MASKED_NEW_DIFFERS_X:
            ComFilterMask = Com_RxMaskedNewDiffersX[ComFilterIndex].ComFilterMask;
            ComFilterX = Com_RxMaskedNewDiffersX[ComFilterIndex].ComFilterX;
            if ((NewSignalValue & ComFilterMask) != ComFilterX)
            {
                ret = TRUE;
            }
            break;
#endif
#if (COM_RXMASKNEWEQUALSX_NUMBER > 0u)
        case COM_MASKED_NEW_EQUALS_X:
            ComFilterMask = Com_RxMaskedNewEqualsX[ComFilterIndex].ComFilterMask;
            ComFilterX = Com_RxMaskedNewEqualsX[ComFilterIndex].ComFilterX;
            if ((NewSignalValue & ComFilterMask) == ComFilterX)
            {
                ret = TRUE;
            }
            break;
#endif
            /*COM_BOOLEAN not support WITHIN and WITHOUT*/
#if (COM_RXNEWISWITHIN_NUMBER > 0u)
        case COM_NEW_IS_WITHIN:
            ComFilterMax = Com_RxNewIsWithin[ComFilterIndex].ComFilterMax;
            ComFilterMin = Com_RxNewIsWithin[ComFilterIndex].ComFilterMin;
            ret = Com_NewIsWInWOtFilterCalculate(SignalType, NewSignalValue, ComFilterMax, ComFilterMin, TRUE);
            break;
#endif
            /*COM_BOOLEAN not support WITHIN and WITHOUT*/
#if (COM_RXNEWISOUTSIDE_NUMBER > 0u)
        case COM_NEW_IS_OUTSIDE:
            ComFilterMax = Com_RxNewIsOutside[ComFilterIndex].ComFilterMax;
            ComFilterMin = Com_RxNewIsOutside[ComFilterIndex].ComFilterMin;
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
#if (COM_RXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
        case COM_MASKED_NEW_DIFFERS_MASKED_OLD:
            ComFilterMask = Com_RxMaskedNewDiffersMaskedOld[ComFilterIndex];
            if (Com_MaskNewDifferMaskOldTimeOut[ComFilterIndex])
            {
                ret = TRUE;
                Com_MaskNewDifferMaskOldTimeOut[ComFilterIndex] = FALSE;
            }
            else
            {
                ret = Com_MaskedNewDiffersMaskedOldFilterCalculate(
                    SignalType,
                    NewSignalValue,
                    SignalBufferId,
                    ComFilterMask);
            }
            break;
#endif
        default:
            /*do nothing*/
            break;
        }
    }
    return ret;
}
#endif

#if ((0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXGROUPSIGNAL_NUMBER))
#if ((STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE))
/******************************************************************************/
/*
 * Brief               called by Com_SignalGroupInvalidCalculate.
 *                     Uint8_Dyn/Uint8_N Group Signal Invalid Calculate.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalLength,GroupSignalId,IpduBufferId
 * Param-Name[out]     None
 * Param-Name[in/out]  SignalGroupInvalid
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE) Com_Uint8NDynGroupSignalInvalidCalculate(
    uint16 SignalLength,
    Com_SignalIdType GroupSignalId,
#if (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE)
    boolean* SignalGroupInvalid,
#endif
    Com_RxIpduBufIdType IpduBufferId)
{
    const Com_RxGroupSignalType* rxGroupSignalPtr = &Com_ConfigStd->ComRxGroupSignal[GroupSignalId];
    Com_RxIpduBufIdType ipduBufferPos = IpduBufferId + (Com_RxIpduBufIdType)rxGroupSignalPtr->ComSigLsbBytePos;
#if ((0u < COM_GWSIGNAL_8BITBUFF_SIZE) && (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE))     \
    || ((0u < COM_SIGNAL_8BIT_INVALID_SIZE) && (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE) \
        && (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE))
    Com_SignalType signalType = rxGroupSignalPtr->ComSignalType;
#endif
#if (0u < COM_SIGNAL_8BIT_INVALID_SIZE) && (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE)
    uint16 invalidValueId = rxGroupSignalPtr->ComSignalInvalidBuffRefId;
#endif

/*the group signal need gateway,store group signal value in gateway buffer*/
#if (COM_GWMAPPING_NUMBER > 0u)
    uint16 gwSignalBufferId = rxGroupSignalPtr->GWSignalBufferId;
    if (COM_UNUSED_UINT16 != gwSignalBufferId)
    {
#if (0u < COM_GWSIGNAL_8BITBUFF_SIZE)
        SchM_Enter_Com_MultiCore_Lock();
        SchM_Enter_Com_Context();
#if (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
        if (COM_UINT8_DYN == signalType)
        {
            (void)ILib_memcpy(&Com_GWSignal8BitBuff[gwSignalBufferId], &SignalLength, sizeof(SignalLength));
            gwSignalBufferId += sizeof(SignalLength);
        }
#endif
        (void)ILib_memcpy(&Com_GWSignal8BitBuff[gwSignalBufferId], &Com_RxIPduRuntimeBuff[ipduBufferPos], SignalLength);
        SchM_Exit_Com_Context();
        SchM_Exit_Com_MultiCore_Lock();
#endif
    }
#endif /* COM_GWMAPPING_NUMBER > 0u */

#if (0u < COM_SIGNAL_8BIT_INVALID_SIZE)
#if (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE) && (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE)
    if (((COM_UINT8_DYN == signalType) && (SignalLength == rxGroupSignalPtr->ComSignalDataInvalidValueLength))
        || (COM_UINT8_N == signalType))
#endif
    {
#if (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE)
        if ((COM_UNUSED_UINT16 != invalidValueId) && (!(*SignalGroupInvalid)))
        {
            if (0
                == ILib_memcmp(
                    &Com_Signal8BitInvalidValue[invalidValueId],
                    &Com_RxIPduRuntimeBuff[ipduBufferPos],
                    SignalLength))
            {
                *SignalGroupInvalid = TRUE;
            }
        }
#endif
    }
#endif
    return;
}
#endif

/******************************************************************************/
/*
 * Brief               called by Com_SignalGroupRxIndication.
 *                     Signal Group Invalid Calculate.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      RxSignalGroupPtr,IpduBufferId,RxDynSignalLength
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/******************************************************************************/
static FUNC(boolean, COM_CODE) Com_SignalGroupInvalidCalculate(
    const Com_RxSignalGroupType* RxSignalGroupPtr,
    Com_RxIpduBufIdType IpduBufferId,
    uint16 RxDynSignalLength)
{
    boolean signalGroupInvalid = FALSE;
#if (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE)
    uint16 invalidValueId;
#endif
    uint16 gwSignalBufferId;
#if ((0u < COM_RXIPDUBUFF_SIZE) && ((0u < COM_SIGNAL_8BIT_INVALID_SIZE) || (0u < COM_GWSIGNAL_8BITBUFF_SIZE)))
    uint16 signalLength;
#endif
    uint64 unPackValue;
    Com_SignalType signalType;
    const Com_RxGroupSignalType* rxGroupSignalPtr;
    Com_RxGroupSignalIdType groupSignalNumber = RxSignalGroupPtr->ComGroupSignalRefNumber;
    Com_RxGroupSignalIdType cnt = RxSignalGroupPtr->ComGroupSignalRefStartId;

    for (; cnt < groupSignalNumber; ++cnt)
    {
        rxGroupSignalPtr = &Com_ConfigStd->ComRxGroupSignal[cnt];
#if (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE)
        invalidValueId = rxGroupSignalPtr->ComSignalInvalidBuffRefId;
#endif
        signalType = rxGroupSignalPtr->ComSignalType;

#if (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
        if ((COM_UINT8_N == signalType) || (COM_UINT8_DYN == signalType))
        {
#if ((0u < COM_RXIPDUBUFF_SIZE) && ((0u < COM_SIGNAL_8BIT_INVALID_SIZE) || (0u < COM_GWSIGNAL_8BITBUFF_SIZE)))
            signalLength = (COM_UINT8_N == signalType) ? rxGroupSignalPtr->ComSignalLength : RxDynSignalLength;
            Com_Uint8NDynGroupSignalInvalidCalculate(
                signalLength,
                cnt,
#if (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE)
                &signalGroupInvalid,
#endif
                IpduBufferId);
#endif
        }
        else
#endif
        {
            unPackValue = Com_SignalUnPackHandle(
#if (STD_ON == COM_SIGNAL_SIGNED_TYPE_ENABLE)
                signalType,
#endif
                COM_RX_GROUP_SIGNAL,
                rxGroupSignalPtr,
                &Com_RxIPduRuntimeBuff[IpduBufferId]);
#if (COM_GWMAPPING_NUMBER > 0u)
            SchM_Enter_Com_MultiCore_Lock();
            gwSignalBufferId = rxGroupSignalPtr->GWSignalBufferId;
            Com_UpdateGWSignalBuffer(unPackValue, gwSignalBufferId, signalType);
            SchM_Exit_Com_MultiCore_Lock();
#endif /* COM_GWMAPPING_NUMBER > 0u */

#if (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE)
            if ((COM_UNUSED_UINT16 != invalidValueId) && (!signalGroupInvalid))
            {
                if (Com_SignalInvalidateHandle(signalType, invalidValueId, unPackValue))
                {
                    signalGroupInvalid = TRUE;
                }
            }
#endif
        }

#if COM_GWMAPPING_NUMBER > 0u
        Com_GwMappingIdType gwMappingId = rxGroupSignalPtr->GwMappingId;
        if (gwMappingId != COM_NOT_USED_GW_MAPPING_ID)
        {
            Com_GwUpdateDestinationRequest(gwMappingId, TRUE);
        }
#endif
    }
    COM_NOUSED(RxDynSignalLength);
    return signalGroupInvalid;
}
#endif /*  0u < COM_RXSIGNALGROUP_NUMBER && 0u < COM_RXGROUPSIGNAL_NUMBER */

#if (COM_RXSIGNAL_NUMBER > 0u) && (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE)
/******************************************************************************/
/*
 * Brief               RxSignal receive timeout handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      rxIpduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/******************************************************************************/
/* PRQA S 3673 ++ */ /* MISRA Rule 8.13 */
static FUNC(void, COM_CODE)
    Com_RxSignalTimeOutHandle(Com_RxIPduRunTimeStateType* RxIpduStatePtr, const Com_RxIPduType* RxIpduPtr)
/* PRQA S 3673 -- */ /* MISRA Rule 8.13 */
{
    Com_RxSigTimeoutIdType ipduSignalTimeoutRefNumber, cnt;
    Com_SignalIdType rxSignalId;
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_REPLACE_ENABLE) \
    || (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)
    Com_RxDataTimeoutActionType timeoutAction;
    uint16 substituteBufId = COM_UNUSED_UINT16;
#endif
    uint16* rxSignalTimeoutCnt;
    const Com_RxSignalType* rxSignalPtr;
#if (COM_RXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
    Com_SignalIdType maskNewDifferMaskOldId;
#endif

    ipduSignalTimeoutRefNumber = RxIpduPtr->ComIPduSignalsTimeoutRefNumber;
    cnt = RxIpduPtr->ComIPduSignalsTimeoutRefStartId;
    for (; cnt < ipduSignalTimeoutRefNumber; ++cnt)
    {
        rxSignalId = Com_TimeoutRxSignalRef[cnt];
        /* PRQA S 2983 ++ */ /* MISRA Rule 2.2*/
        rxSignalPtr = &Com_ConfigStd->ComRxSignal[rxSignalId];
        /* PRQA S 2983 -- */ /* MISRA Rule 2.2*/
        rxSignalTimeoutCnt = &Com_RxSignalTimeOutCnt[cnt];
        if (*rxSignalTimeoutCnt > 0u)
        {
            *rxSignalTimeoutCnt -= 1u;
            if (0u == *rxSignalTimeoutCnt)
            {
/*let pass any value for the filter MASKED_NEW_DIFFERS_MASKED_OLD after an RX deadline timeout*/
#if (COM_RXMASKNEWDIFFERMASKOLD_NUMBER > 0u) && (COM_RXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)
                if ((COM_UNUSED_RXSIGNALFILTERID != rxSignalPtr->ComFilterIndex)
                    && (COM_MASKED_NEW_DIFFERS_MASKED_OLD == rxSignalPtr->ComFilterAlgorithm))
                {
                    maskNewDifferMaskOldId = rxSignalPtr->ComFilterIndex;
                    Com_MaskNewDifferMaskOldTimeOut[maskNewDifferMaskOldId] = TRUE;
                }
#endif
                /*handle the timeout replace action*/
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_REPLACE_ENABLE) \
    || (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)
                timeoutAction = Com_RxSignalDataTimeoutActionCfg[cnt];
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)
                substituteBufId = Com_RxSignalSubstituteValueIdRef[cnt];
#endif
                if (COM_TIMEOUT_ACTION_NONE != timeoutAction)
                {
                    Com_RxDataTimeOutActionHandle(
                        RxIpduStatePtr,
                        RxIpduPtr,
                        timeoutAction,
                        (const void*)rxSignalPtr,
                        TRUE,
                        substituteBufId);
                }
#endif
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_NOTIFICATION_ENABLE)
                if (NULL_PTR != Com_RxSignalTimeoutNotificationCfg[cnt])
                {
                    Com_RxSignalTimeoutNotificationCfg[cnt]();
                }
#endif
                *rxSignalTimeoutCnt = Com_RxSignalTimeoutVal[cnt];
            }
        }
    }
    COM_NOUSED(RxIpduStatePtr);
    return;
}
#endif

#if (                                                                        \
    ((COM_RXSIGNAL_NUMBER > 0u) && (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE)) \
    || ((0u < COM_RXGROUPSIGNAL_NUMBER) && (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)))
/******************************************************************************/
/*
 * Brief               Rx ipdu based receive DM timeout handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      rxIpduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/******************************************************************************/
static FUNC(void, COM_CODE)
    Com_RxIpduBaseDMTimeOutHandle(Com_RxIPduRunTimeStateType* RxIpduStatePtr, const Com_RxIPduType* RxIpduPtr)
{
    if (RxIpduStatePtr->RxIpduBaseDMTimeout > 0u)
    {
        RxIpduStatePtr->RxIpduBaseDMTimeout -= 1u;
        if (0u == RxIpduStatePtr->RxIpduBaseDMTimeout)
        {
#if (0u < COM_RXSIGNAL_NUMBER) && (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE)
            Com_RxDMTimeOutHandleSignal(RxIpduStatePtr, RxIpduPtr);
#endif /* 0u < COM_RXSIGNAL_NUMBER && STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE */

#if (0u < COM_RXGROUPSIGNAL_NUMBER) && (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)
            Com_RxDMTimeOutHandleSignalGroup(RxIpduStatePtr, RxIpduPtr);
#endif /* 0u < COM_RXGROUPSIGNAL_NUMBER && STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE */
            RxIpduStatePtr->RxIpduBaseDMTimeout = RxIpduPtr->ComRxIPduDMTimeout;
        }
    }
}
#endif

#if (0u < COM_RXSIGNAL_NUMBER) && (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE)
/******************************************************************************/
/*
 * Brief               Rx ipdu based receive DM timeout handle signal
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      rxIpduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/******************************************************************************/
/* PRQA S 3673 ++ */ /* MISRA Rule 8.13 */
static FUNC(void, COM_CODE)
    Com_RxDMTimeOutHandleSignal(Com_RxIPduRunTimeStateType* RxIpduStatePtr, const Com_RxIPduType* RxIpduPtr)
/* PRQA S 3673 -- */ /* MISRA Rule 8.13 */
{
    const Com_RxSignalType* rxSignalPtr;
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_REPLACE_ENABLE) \
    || (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)
    Com_RxDataTimeoutActionType timeoutAction;
    uint16 substituteBufId = COM_UNUSED_UINT16; /* PRQA S 2983 */ /* MISRA Rule 2.2*/
#endif
#if (COM_RXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
    Com_SignalIdType maskNewDifferMaskOldId;
#endif
    Com_RxSigTimeoutIdType ipduSignalTimeoutRefNumber, cnt;
    Com_SignalIdType rxSignalId;
    ipduSignalTimeoutRefNumber = RxIpduPtr->ComIPduSignalsTimeoutRefNumber;
    cnt = RxIpduPtr->ComIPduSignalsTimeoutRefStartId;
    for (; cnt < ipduSignalTimeoutRefNumber; ++cnt)
    {
        rxSignalId = Com_TimeoutRxSignalRef[cnt];
        rxSignalPtr = &Com_ConfigStd->ComRxSignal[rxSignalId]; /* PRQA S 2983 */ /* MISRA Rule 2.2*/
#if (STD_ON == COM_RX_SIGNAL_UPDATE_BIT_ENABLE)
        /*rx signal ComTimeout > 0 and have not update-bit*/
        if (COM_UNUSED_SIGNALPOSITION == rxSignalPtr->ComUpdateLsbBytePos)
#endif
        {
            if (Com_RxSignalTimeoutVal[cnt] > 0u)
            {
/*let pass any value for the filter MASKED_NEW_DIFFERS_MASKED_OLD after an RX deadline timeout*/
#if (COM_RXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
                maskNewDifferMaskOldId = rxSignalPtr->ComFilterIndex;
                if ((COM_UNUSED_RXSIGNALFILTERID == maskNewDifferMaskOldId)
                    && (COM_MASKED_NEW_DIFFERS_MASKED_OLD == rxSignalPtr->ComFilterAlgorithm))
                {
                    Com_MaskNewDifferMaskOldTimeOut[maskNewDifferMaskOldId] = TRUE;
                }
#endif

#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_REPLACE_ENABLE) \
    || (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)
                timeoutAction = Com_RxSignalDataTimeoutActionCfg[cnt];
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)
                substituteBufId = Com_RxSignalSubstituteValueIdRef[cnt];
#endif
                Com_RxDataTimeOutActionHandle(
                    RxIpduStatePtr,
                    RxIpduPtr,
                    timeoutAction,
                    (const void*)rxSignalPtr,
                    TRUE,
                    substituteBufId);
#endif
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_NOTIFICATION_ENABLE)
                if (NULL_PTR != Com_RxSignalTimeoutNotificationCfg[cnt])
                {
                    Com_RxSignalTimeoutNotificationCfg[cnt]();
                }
#endif
            }
        }
    }
    COM_NOUSED(RxIpduStatePtr);
}
#endif

#if (0u < COM_RXGROUPSIGNAL_NUMBER) && (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)
/******************************************************************************/
/*
 * Brief               Rx ipdu based receive DM timeout handle signal group
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      rxIpduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/******************************************************************************/
static FUNC(void, COM_CODE)
    Com_RxDMTimeOutHandleSignalGroup(Com_RxIPduRunTimeStateType* RxIpduStatePtr, const Com_RxIPduType* RxIpduPtr)
{
    Com_RxDataTimeoutActionType timeoutAction;
#if (COM_RXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
    Com_SignalIdType maskNewDifferMaskOldId;
#endif
    uint16 substituteBufId = COM_UNUSED_UINT16;
#if (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)
    /* substituteStartId is continuous */
    Com_RxGroupSignalIdType substituteStartId = RxIpduPtr->ComGrpSigSubstituteStartId;
#endif
    const Com_RxSignalGroupType* rxSignalGroupPtr;
    const Com_RxGroupSignalType* rxGroupSignalPtr;
    Com_RxSigGrpTimeoutIdType ipduSignalGroupTimeoutNumber, counter;
    Com_RxSignalGroupIdType rxSignalGroupId;
    ipduSignalGroupTimeoutNumber = RxIpduPtr->ComIPduSignalGroupsTimeoutRefNumber;
    counter = RxIpduPtr->ComIPduSignalGroupsTimeoutRefStartId;
    for (; counter < ipduSignalGroupTimeoutNumber; ++counter)
    {
        rxSignalGroupId = Com_TimeoutRxSigGrpRef[counter];
        rxSignalGroupPtr = &Com_ConfigStd->ComRxSignalGroup[rxSignalGroupId];
/*rx signal group ComTimeout > 0 and have not update-bit*/
#if (STD_ON == COM_RX_SIG_GROUP_UPDATE_BIT_ENABLE)
        if (COM_UNUSED_SIGNALPOSITION == rxSignalGroupPtr->ComUpdateLsbBytePos)
#endif
        {
            if (Com_RxSigGrpTimeoutVal[counter] > 0u)
            {
                Com_RxGroupSignalIdType groupSignalNumber = rxSignalGroupPtr->ComGroupSignalRefNumber;
                Com_RxGroupSignalIdType groupSignalId = rxSignalGroupPtr->ComGroupSignalRefStartId;
                /* update all group signal init value into group signal runtime buffer */
#if (COM_RXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
                for (; groupSignalId < groupSignalNumber; ++groupSignalId)
                {
                    rxGroupSignalPtr = &Com_ConfigStd->ComRxGroupSignal[groupSignalId];
                    if ((COM_UNUSED_RXGRPSIGFILTERID != rxGroupSignalPtr->ComFilterIndex)
                        && (COM_MASKED_NEW_DIFFERS_MASKED_OLD == rxGroupSignalPtr->ComFilterAlgorithm))
                    {
                        maskNewDifferMaskOldId = rxGroupSignalPtr->ComFilterIndex;
                        Com_MaskNewDifferMaskOldTimeOut[maskNewDifferMaskOldId] = TRUE;
                    }
                }
#endif
#if (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_REPLACE_ENABLE) \
    || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)
                timeoutAction = Com_RxSigGrpDataTimeoutActionCfg[counter];
                if (COM_TIMEOUT_ACTION_NONE != timeoutAction)
                {
/*replace the rx pdu buffer for signal group array*/
#if (STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API)
                    if (COM_UNUSED_COM_SIGNALGROUPARRYID != rxSignalGroupPtr->ComSignalGroupArrayId)
                    {
                        uint16 iPduSignalGroupArrayStartBufId =
                            Com_SignalGroupArray[rxSignalGroupPtr->ComSignalGroupArrayId].ComIPduStartBufIndex;
                        uint16 signalGroupArrayLength =
                            Com_SignalGroupArray[rxSignalGroupPtr->ComSignalGroupArrayId].SignalGroupArrayLength;
                        SchM_Enter_Com_Context();
                        (void)ILib_memcpy(
                            &Com_RxIPduRuntimeBuff[iPduSignalGroupArrayStartBufId],
                            &Com_RxIPduInitValue[iPduSignalGroupArrayStartBufId],
                            signalGroupArrayLength);
                        SchM_Exit_Com_Context();
                    }
#endif
                    /*replace the signal group(include all group signals) value with init value*/
                    groupSignalId = rxSignalGroupPtr->ComGroupSignalRefStartId;
                    for (; groupSignalId < groupSignalNumber; ++groupSignalId)
                    {
                        rxGroupSignalPtr = &Com_ConfigStd->ComRxGroupSignal[groupSignalId];
#if (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)
                        if (COM_TIMEOUT_ACTION_SUBSTITUTE == timeoutAction)
                        {
                            substituteBufId = Com_RxGrpSigSubstituteValueIdRef[substituteStartId];
                            substituteStartId++;
                        }
#endif

                        Com_RxDataTimeOutActionHandle(
                            RxIpduStatePtr,
                            RxIpduPtr,
                            timeoutAction,
                            (void*)rxGroupSignalPtr,
                            FALSE,
                            substituteBufId);
                    }
                }
#endif
#if (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_NOTIFICATION_ENABLE)
                if (NULL_PTR != Com_RxSigGrpTimeoutNotificationCfg[counter])
                {
                    Com_RxSigGrpTimeoutNotificationCfg[counter]();
                }
#endif
            }
        }
    }
}
#endif

#if ((COM_IPDUGROUP_MAX > 0u) && ((COM_RXSIGNAL_NUMBER + COM_RXSIGNALGROUP_NUMBER) > 0u))
/******************************************************************************/
/*
 * Brief               set the reception deadline monitoring timer for the
 *                     included signals and signal groups to the configured
 *                     ComFirstTimeout
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      IpduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE) Com_ResetRxPduTimeOut(PduIdType IpduId)
{
    const Com_RxIPduType* rxIpduPtr = &Com_ConfigStd->ComRxIPdu[IpduId];
#if (STD_ON == COM_RX_SIGNAL_UPDATE_BIT_ENABLE) || (STD_ON == COM_RX_SIG_GROUP_UPDATE_BIT_ENABLE)
    Com_SignalPositionType updateBitPosition;
#endif /* STD_ON == COM_RX_SIGNAL_UPDATE_BIT_ENABLE || STD_ON == COM_RX_SIG_GROUP_UPDATE_BIT_ENABLE */
    uint8 rxDMFlag = 0u;

#if (COM_RXSIGNAL_NUMBER > 0u) && (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE)
    const Com_RxSignalType* rxSignalPtr;
    Com_RxSigTimeoutIdType ipduSignalTimeoutRefNumber, cnt;
    Com_SignalIdType rxSignalId;
    ipduSignalTimeoutRefNumber = rxIpduPtr->ComIPduSignalsTimeoutRefNumber;
    cnt = rxIpduPtr->ComIPduSignalsTimeoutRefStartId;
    for (; cnt < ipduSignalTimeoutRefNumber; ++cnt)
    {
#if (STD_ON == COM_RX_SIGNAL_UPDATE_BIT_ENABLE)
        rxSignalId = Com_TimeoutRxSignalRef[cnt];
        rxSignalPtr = &Com_ConfigStd->ComRxSignal[rxSignalId];
        updateBitPosition = rxSignalPtr->ComUpdateLsbBytePos;
        /*rx signal with update-bits*/
        if (COM_UNUSED_SIGNALPOSITION != updateBitPosition)
        {
            Com_RxSignalTimeOutCnt[cnt] = Com_RxSignalFirstTimeoutVal[cnt];
            rxDMFlag |= COM_RX_SIGNAL_DM;
        }
        /* rx signal without update-bits */
        else
#endif
        {
            rxDMFlag |= COM_RX_IPDU_DM;
#if (STD_OFF == COM_RX_SIGNAL_UPDATE_BIT_ENABLE)
            break;
#endif
        }
    }
#endif /* COM_RXSIGNAL_NUMBER > 0u && STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE */
#if (COM_RXSIGNALGROUP_NUMBER > 0u) && (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)
    Com_RxSigGrpTimeoutIdType ipduSignalGroupTimeoutNumber, counter;
    Com_RxSignalGroupIdType rxSignalGroupId;
    Com_RxSignalGroupType* rxSignalGroupPtr;
    ipduSignalGroupTimeoutNumber = rxIpduPtr->ComIPduSignalGroupsTimeoutRefNumber;
    counter = rxIpduPtr->ComIPduSignalGroupsTimeoutRefStartId;
    for (; counter < ipduSignalGroupTimeoutNumber; ++counter)
    {
#if (STD_ON == COM_RX_SIG_GROUP_UPDATE_BIT_ENABLE)
        rxSignalGroupId = Com_TimeoutRxSigGrpRef[counter];
        rxSignalGroupPtr = &Com_ConfigStd->ComRxSignalGroup[rxSignalGroupId];
        updateBitPosition = rxSignalGroupPtr->ComUpdateLsbBytePos;
        /*rx signal group with update-bits*/
        if (COM_UNUSED_SIGNALPOSITION != updateBitPosition)
        {
            Com_RxSignalGroupTimeOutCnt[cnt] = Com_RxSigGrpFirstTimeoutVal[cnt];
            rxDMFlag |= COM_RX_SIGNAL_GROUP_DM;
        }
        /* rx signal group without update-bits */
        else
#endif
        {
            rxDMFlag |= COM_RX_IPDU_DM;
#if (STD_OFF == COM_RX_SIG_GROUP_UPDATE_BIT_ENABLE)
            break;
#endif
        }
    }
#endif /* COM_RXSIGNALGROUP_NUMBER > 0u && STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE */

#if (                                                                        \
    ((COM_RXSIGNAL_NUMBER > 0u) && (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE)) \
    || ((COM_RXSIGNALGROUP_NUMBER > 0u) && (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)))
    if (rxDMFlag > 0u)
    {
        Com_RxIPduRunTimeStateType* rxIpduStatePtr = &Com_RxIPduRunTimeState[IpduId];
        if (COM_RX_IPDU_DM == (rxDMFlag & COM_RX_IPDU_DM)) /* PRQA S 2991,2995 */ /* MISRA Rule 14.3,2.2 */
        {
            rxIpduStatePtr->RxIpduBaseDMTimeout = rxIpduPtr->ComRxIPduDMFirstTimeout;
        }
#if (STD_ON == COM_RX_SIGNAL_UPDATE_BIT_ENABLE) || (STD_ON == COM_RX_SIG_GROUP_UPDATE_BIT_ENABLE)
        if ((COM_RX_SIGNAL_DM == (rxDMFlag & COM_RX_SIGNAL_DM))
            || (COM_RX_SIGNAL_GROUP_DM == (rxDMFlag & COM_RX_SIGNAL_GROUP_DM)))
        {
            COM_SET_RX_DM_FLAG(rxIpduStatePtr->RxIpduDMFlag, rxDMFlag);
        }
        /* rx ipdu base DM */
        else
#endif
        {
            COM_CLEAR_RX_DM_FLAG(rxIpduStatePtr->RxIpduDMFlag);
            COM_SET_RX_DM_FLAG(rxIpduStatePtr->RxIpduDMFlag, COM_RX_IPDU_DM_ONLY);
        }
    }
#endif
}
#endif

#if ((COM_IPDUGROUP_NUMBER > 0u) && ((COM_RXSIGNAL_NUMBER + COM_RXSIGNALGROUP_NUMBER) > 0u))
/******************************************************************************/
/*
 * Brief               set the reception deadline monitoring timer for the
 *                     included signals and signal groups to 0
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      IpduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE) Com_DisableRxPduTimeOut(PduIdType IpduId)
{
    const Com_RxIPduType* rxIpduPtr = &Com_ConfigStd->ComRxIPdu[IpduId];
    Com_RxIPduRunTimeStateType* rxIpduStatePtr = &Com_RxIPduRunTimeState[IpduId];
#if (COM_RXSIGNAL_NUMBER > 0u) && (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE)
    Com_RxSigTimeoutIdType ipduSignalTimeoutRefNumber, cnt;
    ipduSignalTimeoutRefNumber = rxIpduPtr->ComIPduSignalsTimeoutRefNumber;
    cnt = rxIpduPtr->ComIPduSignalsTimeoutRefStartId;
    for (; cnt < ipduSignalTimeoutRefNumber; ++cnt)
    {
        Com_RxSignalTimeOutCnt[cnt] = 0u;
    }
#endif
#if (COM_RXSIGNALGROUP_NUMBER > 0u) && (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)
    Com_RxSigGrpTimeoutIdType ipduSignalGroupTimeoutNumber, counter;
    Com_RxSignalGroupType* rxSignalGroupPtr;
    ipduSignalGroupTimeoutNumber = rxIpduPtr->ComIPduSignalGroupsTimeoutRefNumber;
    counter = rxIpduPtr->ComIPduSignalGroupsTimeoutRefStartId;
    for (; counter < ipduSignalGroupTimeoutNumber; ++counter)
    {
        Com_RxSignalGroupTimeOutCnt[counter] = 0u;
    }
#endif

    COM_CLEAR_RX_DM_FLAG(rxIpduStatePtr->RxIpduDMFlag);
    rxIpduStatePtr->RxIpduBaseDMTimeout = 0u;
}
#endif

#if (COM_RXSIGNAL_NUMBER + COM_RXSIGNALGROUP_NUMBER > 0u) \
    && ((STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE) || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE))
/******************************************************************************/
/*
 * Brief               reset receive MD timer for all signals/signal groups
 *                     (which is update) of one rx pdu
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      IpduId,PduInfo
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE)
    Com_ResetUpdateDMTime(PduIdType IpduId, P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfo)
{
    const Com_RxIPduType* rxIpduPtr = &Com_ConfigStd->ComRxIPdu[IpduId];
    Com_RxIPduRunTimeStateType* rxIpduStatePtr = &Com_RxIPduRunTimeState[IpduId];
#if (STD_ON == COM_RX_SIGNAL_UPDATE_BIT_ENABLE) || (STD_ON == COM_RX_SIG_GROUP_UPDATE_BIT_ENABLE)
    Com_SignalPositionType updateBitPosition;
    uint8 updateBitOffset;
#endif /* STD_ON == COM_RX_SIGNAL_UPDATE_BIT_ENABLE || STD_ON == COM_RX_SIG_GROUP_UPDATE_BIT_ENABLE */
    uint8 rxDMFlag = 0u;

#if (COM_RXSIGNAL_NUMBER > 0u) && (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE)
    const Com_RxSignalType* rxSignalPtr;
    Com_RxSigTimeoutIdType ipduSignalTimeoutRefNumber, cnt;
    Com_SignalIdType rxSignalId;
    ipduSignalTimeoutRefNumber = rxIpduPtr->ComIPduSignalsTimeoutRefNumber;
    cnt = rxIpduPtr->ComIPduSignalsTimeoutRefStartId;
    for (; cnt < ipduSignalTimeoutRefNumber; ++cnt)
    {
#if (STD_ON == COM_RX_SIGNAL_UPDATE_BIT_ENABLE)
        rxSignalId = Com_TimeoutRxSignalRef[cnt];
        rxSignalPtr = &Com_ConfigStd->ComRxSignal[rxSignalId];
        updateBitPosition = rxSignalPtr->ComUpdateLsbBytePos;
        /*the rx signal have update bit*/
        if (COM_UNUSED_SIGNALPOSITION != updateBitPosition)
        {
            updateBitOffset = rxSignalPtr->ComUpdateBitMask;
            /*update bit is 1*/
            if (updateBitOffset == ((PduInfo->SduDataPtr[updateBitPosition]) & updateBitOffset))
            {
                Com_RxSignalTimeOutCnt[cnt] = Com_RxSignalTimeoutVal[cnt];
                rxDMFlag |= COM_RX_SIGNAL_DM;
            }
        }
        /*the rx signal not have update bit*/
        else
#endif
        {
            rxDMFlag |= COM_RX_IPDU_DM;
#if (STD_OFF == COM_RX_SIGNAL_UPDATE_BIT_ENABLE)
            break;
#endif
        }
    }
#endif /* COM_RXSIGNAL_NUMBER > 0u && STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE */
#if (COM_RXSIGNALGROUP_NUMBER > 0u) && (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)
    Com_RxSigGrpTimeoutIdType ipduSignalGroupTimeoutNumber, counter;
    Com_RxSignalGroupIdType rxSignalGroupId;
    Com_RxSignalGroupType* rxSignalGroupPtr;
    ipduSignalGroupTimeoutNumber = rxIpduPtr->ComIPduSignalGroupsTimeoutRefNumber;
    counter = rxIpduPtr->ComIPduSignalGroupsTimeoutRefStartId;
    for (; counter < ipduSignalGroupTimeoutNumber; ++counter)
    {
#if (STD_ON == COM_RX_SIG_GROUP_UPDATE_BIT_ENABLE)
        rxSignalGroupId = Com_TimeoutRxSigGrpRef[counter];
        rxSignalGroupPtr = &Com_ConfigStd->ComRxSignalGroup[rxSignalGroupId];
        updateBitPosition = rxSignalGroupPtr->ComUpdateLsbBytePos;
        /*the rx signal group have update bit*/
        if (COM_UNUSED_SIGNALPOSITION != updateBitPosition)
        {
            updateBitOffset = rxSignalGroupPtr->ComUpdateBitMask;
            /*update bit is 1*/
            if (updateBitOffset == ((PduInfo->SduDataPtr[updateBitPosition]) & updateBitOffset))
            {
                Com_RxSignalGroupTimeOutCnt[counter] = Com_RxSigGrpTimeoutVal[counter];
                rxDMFlag |= COM_RX_SIGNAL_GROUP_DM;
            }
        }
        /*the rx signal group not have update bit*/
        else
#endif
        {
            rxDMFlag |= COM_RX_IPDU_DM;
#if (STD_OFF == COM_RX_SIG_GROUP_UPDATE_BIT_ENABLE)
            break;
#endif
        }
    }
#endif /* COM_RXSIGNALGROUP_NUMBER > 0u && STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE */

#if (                                                                        \
    ((COM_RXSIGNAL_NUMBER > 0u) && (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE)) \
    || ((COM_RXSIGNALGROUP_NUMBER > 0u) && (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)))
    if (rxDMFlag > 0u)
    {
        if (COM_RX_IPDU_DM == (rxDMFlag & COM_RX_IPDU_DM)) /* PRQA S 2991,2995 */ /* MISRA Rule 14.3,2.2 */
        {
            rxIpduStatePtr->RxIpduBaseDMTimeout = rxIpduPtr->ComRxIPduDMTimeout;
        }
#if (STD_ON == COM_RX_SIGNAL_UPDATE_BIT_ENABLE) || (STD_ON == COM_RX_SIG_GROUP_UPDATE_BIT_ENABLE)
        if ((COM_RX_SIGNAL_DM == (rxDMFlag & COM_RX_SIGNAL_DM))
            || (COM_RX_SIGNAL_GROUP_DM == (rxDMFlag & COM_RX_SIGNAL_GROUP_DM)))
        {
            COM_SET_RX_DM_FLAG(rxIpduStatePtr->RxIpduDMFlag, rxDMFlag);
        }
        /* rx ipdu base DM */
        else
#endif
        {
            COM_CLEAR_RX_DM_FLAG(rxIpduStatePtr->RxIpduDMFlag);
            COM_SET_RX_DM_FLAG(rxIpduStatePtr->RxIpduDMFlag, COM_RX_IPDU_DM_ONLY);
        }
    }
#endif
    COM_NOUSED(PduInfo);
    return;
}
#endif

#if (STD_ON == COM_RXTPPDU_SUPPORT) && (STD_ON == COM_RX_IPDU_COUNTER_ENABLE)
/******************************************************************************/
/*
 * Brief               unpack the receive pdu's counter
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      IpduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(uint8, COM_CODE)
    Com_UnPackCounterValue(Com_RxIPduCounterIdType IpduCounterId, Com_RxIpduBufIdType RxPduBufferId)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    uint8 counterValue;
    uint32 bitPosition;
    uint8 bitSize;
    bitPosition = Com_RxIPduCounter[IpduCounterId].ComIPduCounterStartPosition;
    bitSize = Com_RxIPduCounter[IpduCounterId].ComIPduCounterSize;
    counterValue = Com_RxIPduRuntimeBuff[RxPduBufferId + (Com_RxIpduBufIdType)(bitPosition >> 3u)];
    counterValue = counterValue << (8u - (((uint8)bitPosition & 0x07u) + bitSize));
    counterValue = counterValue >> (8u - bitSize);
    return counterValue;
}
#endif
#if (STD_ON == COM_RX_IPDU_COUNTER_ENABLE)
/******************************************************************************/
/*
 * Brief               judge the receive pdu's counter is valid or not
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      IpduId,RxCounter
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(boolean, COM_CODE) Com_RxPduCounterValid(PduIdType IpduId, uint8 RxCounter)
{
    const Com_RxIPduType* rxIpduPtr = &Com_ConfigStd->ComRxIPdu[IpduId];
    Com_RxIPduRunTimeStateType* rxIpduStatePtr = &Com_RxIPduRunTimeState[IpduId];
    uint8 counterThreshold;
    uint8 maxCounter;
    uint8 expectedCounter;
    uint8 RxAnyCounterPdu = rxIpduStatePtr->RxIpduRTStFlag & Com_RX_COUNTER_PDU_EN;
    boolean receiveCounterIsValid = FALSE;

    /*accept any incoming I-PDU, regardless of the value of the I-PDU counter*/
    if (Com_RX_COUNTER_PDU_EN == RxAnyCounterPdu)
    {
        /*update the expected counter*/
        rxIpduStatePtr->RxIpduCounter = RxCounter;
        Com_PduCounterIncrease(FALSE, IpduId);
        rxIpduStatePtr->RxIpduRTStFlag &= Com_RX_COUNTER_PDU_DIS;
        receiveCounterIsValid = TRUE;
    }
    else
    {
        maxCounter = Com_RxIPduCounter[rxIpduPtr->ComIPduCounterIndex].ComIPduMaxCounter;
        counterThreshold = Com_RxIPduCounter[rxIpduPtr->ComIPduCounterIndex].ComIPduCounterThreshold;
        expectedCounter = rxIpduStatePtr->RxIpduCounter;
        /*respect to counter wrap-around*/
        if ((maxCounter - expectedCounter) < counterThreshold)
        {
            if ((RxCounter >= expectedCounter) || (RxCounter < (counterThreshold - (maxCounter - expectedCounter))))
            {
                receiveCounterIsValid = TRUE;
            }
        }
        /*respect to counter not wrap-around*/
        else
        {
            if ((RxCounter >= expectedCounter) && (RxCounter <= (expectedCounter + counterThreshold)))
            {
                receiveCounterIsValid = TRUE;
            }
        }
        /*update the expected counter*/
        rxIpduStatePtr->RxIpduCounter = RxCounter;
        Com_PduCounterIncrease(FALSE, IpduId);
        if ((!receiveCounterIsValid)
            && (NULL_PTR != Com_RxIPduCounter[rxIpduPtr->ComIPduCounterIndex].ComIPduCounterErrorNotification))
        {
            Com_RxIPduCounter[rxIpduPtr->ComIPduCounterIndex].ComIPduCounterErrorNotification(
                IpduId,
                expectedCounter,
                RxCounter);
        }
    }
    return receiveCounterIsValid;
}
#endif /* STD_ON == COM_RX_IPDU_COUNTER_ENABLE */
/******************************************************************************/
/*
 * Brief               copy the Rx Pdu data to RxPdu Buffer
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      RxPduId,PduInfoPtr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE)
    Com_CopyPduDataToRxBuffer(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr)
{
    const Com_RxIPduType* rxIpduPtr = &Com_ConfigStd->ComRxIPdu[RxPduId];
    SchM_Enter_Com_Context();
#if (0u < COM_RXIPDUBUFF_SIZE)
    Com_RxIpduBufIdType pduBufferPos = rxIpduPtr->ComRxIPduBufIndex;
    /*copy receive pdu data to rx pdu buffer*/
    (void)ILib_memcpy(&Com_RxIPduRuntimeBuff[pduBufferPos], &PduInfoPtr->SduDataPtr[0], PduInfoPtr->SduLength);
#if (STD_ON == COM_METADATA_SUPPORT)
    uint8 metadataLength = rxIpduPtr->MetaDataLength;
    PduLengthType metaDataStartPos =
        rxIpduPtr->IPduNoDynSignalLength + rxIpduPtr->IPduMaxDynSignalLength + (PduLengthType)pduBufferPos;
    if ((0u < metadataLength) && (NULL_PTR != PduInfoPtr->MetaDataPtr))
    {
        /*copy meta data to rx pdu buffer*/
        (void)ILib_memcpy(&Com_RxIPduRuntimeBuff[metaDataStartPos], &PduInfoPtr->MetaDataPtr[0], metadataLength);
    }
#endif /* STD_ON == COM_METADATA_SUPPORT */
#endif /* 0u < COM_RXIPDUBUFF_SIZE */
    SchM_Exit_Com_Context();

    /*update the pdu length*/
    Com_RxIPduRunTimeState[RxPduId].RxIpduLength = (uint16)PduInfoPtr->SduLength;
    return;
}
/******************************************************************************/
/*
 * Brief               unpack the rx pdu data and invoke notification
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      RxIpduStatePtr, RxIpduPtr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE)
    Com_IndicationProcess(const Com_RxIPduRunTimeStateType* RxIpduStatePtr, const Com_RxIPduType* RxIpduPtr)
{
    uint16 rxDynSignalLength = RxIpduStatePtr->RxIpduLength - (uint16)RxIpduPtr->IPduNoDynSignalLength;
    Com_RxIpduBufIdType ipduBufferId = RxIpduPtr->ComRxIPduBufIndex;
#if (STD_ON == COM_RX_SIGNAL_UPDATE_BIT_ENABLE) || (STD_ON == COM_RX_SIG_GROUP_UPDATE_BIT_ENABLE) \
    || (STD_ON == COM_GW_SRC_SIG_UPDATE_BIT_ENABLE)
    Com_SignalPositionType updateBitPosition;
    uint8 updateBitOffset;
#endif

/*store the rx dynamic signal length which really receive,not replace by any reason for gateway*/
#if (0u < COM_RXSIGNAL_NUMBER)
    Com_RxSignalIdType ipduSignalRefNumber, cnt;
    const Com_RxSignalType* rxSignalPtr;
    ipduSignalRefNumber = RxIpduPtr->ComIPduSignalsRefNumber;
    cnt = RxIpduPtr->ComIpduSignalRefStartId;
    for (; cnt < ipduSignalRefNumber; ++cnt)
    {
        rxSignalPtr = &Com_ConfigStd->ComRxSignal[cnt];
#if (STD_ON == COM_RX_SIGNAL_UPDATE_BIT_ENABLE)
        updateBitPosition = rxSignalPtr->ComUpdateLsbBytePos;
        /*the rx signal have update bit*/
        if (COM_UNUSED_SIGNALPOSITION != updateBitPosition)
        {
            updateBitOffset = rxSignalPtr->ComUpdateBitMask;
            /*update bit is 1*/
            if (updateBitOffset
                == ((Com_RxIPduRuntimeBuff[ipduBufferId + (Com_RxIpduBufIdType)(updateBitPosition)]) & updateBitOffset))
            {
                Com_SignalRxIndication(rxSignalPtr, rxDynSignalLength, ipduBufferId);
            }
        }
        /*the rx signal not have update bit*/
        else
#endif /* STD_ON == COM_RX_SIGNAL_UPDATE_BIT_ENABLE */
        {
            Com_SignalRxIndication(rxSignalPtr, rxDynSignalLength, ipduBufferId);
        }
    }
#endif /*0u < COM_RXSIGNAL_NUMBER*/
#if ((0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXGROUPSIGNAL_NUMBER))
    Com_RxSignalGroupIdType ipduSignalGroupNumber, counter;
    const Com_RxSignalGroupType* rxSignalGroupPtr;
    ipduSignalGroupNumber = RxIpduPtr->ComIPduSignalGroupsRefNumber;
    counter = RxIpduPtr->ComIPduSignalGroupsRefStartId;
    for (; counter < ipduSignalGroupNumber; ++counter)
    {
        rxSignalGroupPtr = &Com_ConfigStd->ComRxSignalGroup[counter];
#if (STD_ON == COM_RX_SIG_GROUP_UPDATE_BIT_ENABLE)
        updateBitPosition = rxSignalGroupPtr->ComUpdateLsbBytePos;
        /*the rx group signal have update bit*/
        if (COM_UNUSED_SIGNALPOSITION != updateBitPosition)
        {
            updateBitOffset = rxSignalGroupPtr->ComUpdateBitMask;
            /*update bit is 1*/
            if (updateBitOffset
                == ((Com_RxIPduRuntimeBuff[ipduBufferId + (Com_RxIpduBufIdType)(updateBitPosition)]) & updateBitOffset))
            {
                Com_SignalGroupRxIndication(rxSignalGroupPtr, rxDynSignalLength, ipduBufferId);
            }
        }
        /*the rx group signal not have update bit*/
        else
#endif /* STD_ON == COM_RX_SIG_GROUP_UPDATE_BIT_ENABLE */
        {
            Com_SignalGroupRxIndication(rxSignalGroupPtr, rxDynSignalLength, ipduBufferId);
        }
    }
#endif
#if (0u < COM_GW_SOURCE_DESCRIPTION_NUMBER) && (COM_GWMAPPING_NUMBER > 0u)
    Com_GwSourceDescriptionIdType gwSourceSignalNumber, id;
    const Com_GwSourceSignalType* gwSourceSignalPtr;
    gwSourceSignalNumber = RxIpduPtr->ComIPduSourceSignalsRefNumber;
    id = RxIpduPtr->ComIPduSourceSignalsRefStartId;
    for (; id < gwSourceSignalNumber; ++id)
    {
        gwSourceSignalPtr = &Com_ConfigStd->ComSourceSignal[id];
#if (STD_ON == COM_GW_SRC_SIG_UPDATE_BIT_ENABLE)
        updateBitPosition = gwSourceSignalPtr->ComUpdateLsbBytePos;
        /*the source signal have update bit*/
        if (COM_UNUSED_SIGNALPOSITION != updateBitPosition)
        {
            updateBitOffset = gwSourceSignalPtr->ComUpdateBitMask;
            /*update bit is 1*/
            if (updateBitOffset
                == ((Com_RxIPduRuntimeBuff[ipduBufferId + (Com_RxIpduBufIdType)updateBitPosition]) & updateBitOffset))
            {
                Com_SourceSignalUnPack(gwSourceSignalPtr, rxDynSignalLength, ipduBufferId);
            }
        }
        /*the rx signal not have update bit*/
        else
#endif /* STD_ON == COM_GW_SRC_SIG_UPDATE_BIT_ENABLE */
        {
            /*unpack the rx source description signal to gateway buffer*/
            Com_SourceSignalUnPack(gwSourceSignalPtr, rxDynSignalLength, ipduBufferId);
        }
    }
#endif /* 0u < COM_GW_SOURCE_DESCRIPTION_NUMBER && COM_GWMAPPING_NUMBER > 0u */
    return;
}
#if (STD_ON == COM_RXTPPDU_SUPPORT) \
    && ((STD_ON == COM_RX_SIGNAL_INVALID_DATA_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE))
/******************************************************************************/
/*
 * Brief               execute all configured ComDataInvalidActions for all
 *                     included signals and signal groups
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      RxIpduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE) Com_TpPduInvalidAction(PduIdType RxIpduId)
{
    const Com_RxIPduType* rxIpduPtr = &Com_ConfigStd->ComRxIPdu[RxIpduId];
#if (0u < COM_RXSIGNAL_NUMBER) && (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ENABLE)
    Com_RxSignalIdType ipduSignalRefNumber, cnt;
    Com_RxSignalInvalidIdType signalInvalidId;
    const Com_RxSignalType* rxSignalPtr;
    ipduSignalRefNumber = rxIpduPtr->ComIPduSignalsRefNumber;
    cnt = rxIpduPtr->ComIpduSignalRefStartId;
    for (; cnt < ipduSignalRefNumber; ++cnt)
    {
        rxSignalPtr = &Com_ConfigStd->ComRxSignal[cnt];
        signalInvalidId = rxSignalPtr->ComSignalInvalidRefId;
        if (COM_UNUSED_RXSIGNALINVALIDID != signalInvalidId)
        {
#if (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ACTION_NOTIFY_ENABLE)
            if (COM_INVALID_ACTION_NOTIFY == Com_RxSignalDataInvalidActionRef[signalInvalidId])
            {
                if (NULL_PTR != Com_RxInvalidNotificationCfg[signalInvalidId])
                {
                    Com_RxInvalidNotificationCfg[signalInvalidId]();
                }
            }
            else
#endif /* STD_ON == COM_RX_SIGNAL_INVALID_DATA_ACTION_NOTIFY_ENABLE */
            {
                Com_InvalidSignalReplaceAndNotification(rxSignalPtr);
            }
        }
    }
#endif /* 0u < COM_RXSIGNAL_NUMBER && STD_ON == COM_RX_SIGNAL_INVALID_DATA_ENABLE */
#if (                                                                  \
    (0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXGROUPSIGNAL_NUMBER) \
    && (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE))
    Com_RxSignalGroupIdType ipduSignalGroupNumber, counter;
    Com_RxGroupSignalIdType groupSignalNumber, groupSignalId;
    uint16 rxDynSignalLength;
    Com_RxIpduBufIdType ipduBufferId;
    Com_RxSignalGroupType* rxSignalGroupPtr;
    ipduSignalGroupNumber = rxIpduPtr->ComIPduSignalGroupsRefNumber;
    counter = rxIpduPtr->ComIPduSignalGroupsRefStartId;
    for (; counter < ipduSignalGroupNumber; ++counter)
    {
        rxSignalGroupPtr = &Com_ConfigStd->ComRxSignalGroup[counter];
#if (STD_ON == COM_RX_SIG_GROUP_INVALID_DATA_ACTION_NOTIFY_ENABLE)
        if (COM_INVALID_ACTION_NOTIFY == rxSignalGroupPtr->ComDataInvalidAction)
        {
            if (NULL_PTR != rxSignalGroupPtr->ComInvalidNotification)
            {
                rxSignalGroupPtr->ComInvalidNotification();
            }
        }
        else
#endif /* STD_ON == COM_RX_SIG_GROUP_INVALID_DATA_ACTION_NOTIFY_ENABLE */
        {
            ipduBufferId = rxIpduPtr->ComRxIPduBufIndex;
#if (COM_RXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
            if (Com_RxSignalGroupFilter(rxSignalGroupPtr, TRUE, ipduBufferId))
#endif
            {
                rxDynSignalLength =
                    Com_RxIPduRunTimeState[RxIpduId].RxIpduLength - (uint16)rxIpduPtr->IPduNoDynSignalLength;
                groupSignalNumber = rxSignalGroupPtr->ComGroupSignalRefNumber;
                groupSignalId = rxSignalGroupPtr->ComGroupSignalRefStartId;
                /* update all group signal init value into group signal runtime buffer */
                for (; groupSignalId < groupSignalNumber; ++groupSignalId)
                {
                    Com_RxGroupSignalBuffHanlde(TRUE, groupSignalId, ipduBufferId, rxDynSignalLength);
                }
#if (STD_ON == COM_RX_SIG_GROUP_NOTIFICATION_ENABLE)
                if (NULL_PTR != rxSignalGroupPtr->ComNotification)
                {
                    rxSignalGroupPtr->ComNotification();
                }
#endif /* STD_ON == COM_RX_SIG_GROUP_NOTIFICATION_ENABLE */
            }
        }
    }
#endif /* 0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXGROUPSIGNAL_NUMBER) && (STD_ON == \
          COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE */
    return;
}
#endif
#if (STD_ON == COM_RX_IPDU_COUNTER_ENABLE) && (STD_ON == COM_IPDU_REPLICATION_ENABLE)
/******************************************************************************/
/*
 * Brief               judge the receive pdu's Replication is valid or not
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      IpduId,RxCounter,PduInfoPtr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(boolean, COM_CODE)
    /* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
    Com_RxPduReplicationValid(
        PduIdType IpduId,
        uint8 RxCounter,
        P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    Com_RxIPduRunTimeStateType* rxIpduStatePtr = &Com_RxIPduRunTimeState[IpduId];
    const Com_RxIPduType* rxIpduPtr = &Com_ConfigStd->ComRxIPdu[IpduId];
    Com_RxIPduCounterIdType counterRxIndex = rxIpduPtr->ComIPduCounterIndex;
    uint8 iPduReplicationQuorum = Com_IPduReplication[counterRxIndex].ComIPduReplicationQuorum;
    uint8 updateCounter;
    uint8 RxAnyCounterPdu = rxIpduStatePtr->RxIpduRTStFlag & Com_RX_COUNTER_PDU_EN;
    boolean rxPduValid = FALSE;

    /*accept any incoming I-PDU, regardless of the value of the I-PDU counter*/
    if (Com_RX_COUNTER_PDU_EN == RxAnyCounterPdu)
    {
        rxIpduStatePtr->RxIpduCounter = RxCounter;
        /*copy receive pdu data to rx pdu buffer*/
        Com_CopyPduDataToRxBuffer(IpduId, PduInfoPtr);
        rxIpduStatePtr->RxReplicationNumber = 1u;
        if (1u == iPduReplicationQuorum)
        {
            /*update the expected counter*/
            Com_PduCounterIncrease(FALSE, IpduId);
            rxIpduStatePtr->RxReplicationNumber = 0u;
            rxPduValid = TRUE;
        }
        rxIpduStatePtr->RxIpduRTStFlag &= Com_RX_COUNTER_PDU_DIS;
    }
    else
    {
        /*the rx pdu counter is expect counter*/
        if (RxCounter == rxIpduStatePtr->RxIpduCounter)
        {
            if (0u == rxIpduStatePtr->RxReplicationNumber)
            {
                /*copy receive pdu data to rx pdu buffer*/
                Com_CopyPduDataToRxBuffer(IpduId, PduInfoPtr);
                rxIpduStatePtr->RxReplicationNumber = 1u;
                if (1u == iPduReplicationQuorum)
                {
                    /*update the expected counter*/
                    Com_PduCounterIncrease(FALSE, IpduId);
                    rxIpduStatePtr->RxReplicationNumber = 0u;
                    rxPduValid = TRUE;
                }
            }
            else
            {
                if (rxIpduStatePtr->RxIpduLength == PduInfoPtr->SduLength)
                {
                    if (0
                        == ILib_memcmp(
                            &Com_RxIPduRuntimeBuff[rxIpduPtr->ComRxIPduBufIndex],
                            &PduInfoPtr->SduDataPtr[0],
                            PduInfoPtr->SduLength))
                    {
                        (rxIpduStatePtr->RxReplicationNumber) += 1u;
                        if (iPduReplicationQuorum == rxIpduStatePtr->RxReplicationNumber)
                        {
                            /*update the expected counter*/
                            Com_PduCounterIncrease(FALSE, IpduId);
                            rxIpduStatePtr->RxReplicationNumber = 0u;
                            rxPduValid = TRUE;
                        }
                    }
                }
            }
        }
        /*rx counter isn't expect counter*/
        else
        {
            updateCounter = (RxCounter == Com_RxIPduCounter[counterRxIndex].ComIPduMaxCounter) ? 0u : (RxCounter + 1u);

            if (updateCounter != rxIpduStatePtr->RxIpduCounter)
            {
                if (NULL_PTR != Com_RxIPduCounter[counterRxIndex].ComIPduCounterErrorNotification)
                {
                    Com_RxIPduCounter[counterRxIndex].ComIPduCounterErrorNotification(
                        IpduId,
                        rxIpduStatePtr->RxIpduCounter,
                        RxCounter);
                }
                /*update the expected counter*/
                Com_PduCounterIncrease(FALSE, IpduId);
                rxIpduStatePtr->RxReplicationNumber = 0u;
            }
        }
    }
    return rxPduValid;
}
#endif
#if (                                                              \
    (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_REPLACE_ENABLE)        \
    || (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)) \
    || ((STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_REPLACE_ENABLE) \
        || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_SUBSTITUTE_ENABLE))
/******************************************************************************/
/*
 * Brief               Rx data timeout action handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      rxIpduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/******************************************************************************/
static void Com_RxDataTimeOutActionHandle(
    Com_RxIPduRunTimeStateType* RxIpduStatePtr,
    const Com_RxIPduType* RxIpduPtr,
    Com_RxDataTimeoutActionType RxTimeoutAction,
    const void* RxArgPtr,
    boolean IsRxSignal,
    Com_SignalIdType SubstituteBufId)
{
    Com_SignalType signalType;
    uint16 initValueLength;
    uint16 initValueId;
    const Com_RxSignalType* rxSignalPtr;
    const Com_RxGroupSignalType* rxGroupSignalPtr;

    /*rx signal*/
    if (IsRxSignal)
    {
        rxSignalPtr = (const Com_RxSignalType*)RxArgPtr;
        initValueId = rxSignalPtr->ComSignalInitValueId;
        signalType = rxSignalPtr->ComSignalType;
    }
    /*rx signal group*/
    else
    {
        rxGroupSignalPtr = (const Com_RxGroupSignalType*)RxArgPtr;
        initValueId = rxGroupSignalPtr->ComSignalInitValueId;
        signalType = rxGroupSignalPtr->ComSignalType;
    }

    /*handle the timeout replace or substitute action*/
#if (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE) \
    || (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_DYN_ENABLE)
    if (COM_UINT8_DYN == signalType)
    {
        initValueLength = (TRUE == IsRxSignal) ? (rxSignalPtr->ComSignalDataInitValueLength)
                                               : (rxGroupSignalPtr->ComSignalDataInitValueLength);
        RxIpduStatePtr->RxIpduLength = (uint16)RxIpduPtr->IPduNoDynSignalLength + initValueLength;
    }
    else
#endif /* STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE || STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE) \
    || STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_DYN_ENABLE */
#if (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) \
    || (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_N_ENABLE)
        if (COM_UINT8_N == signalType)
    {
        initValueLength = (TRUE == IsRxSignal) ? (rxSignalPtr->ComSignalLength) : (rxGroupSignalPtr->ComSignalLength);
    }
    else
#endif /* STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE || STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) \
    || STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_N_ENABLE */
    {
        initValueLength = 0u;
    }

#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_REPLACE_ENABLE) \
    || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_REPLACE_ENABLE)
    if (COM_TIMEOUT_ACTION_REPLACE == RxTimeoutAction)
    {
        Com_InitSignalBuffer(signalType, initValueId, initValueLength);
    }
#endif /* STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_REPLACE_ENABLE \
    || STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_REPLACE_ENABLE */
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_SUBSTITUTE_ENABLE) \
    || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)
    if (COM_TIMEOUT_ACTION_SUBSTITUTE == RxTimeoutAction)
    {
        Com_SubstituteSignalBuffer(signalType, initValueId, initValueLength, SubstituteBufId);
    }
#endif
    COM_NOUSED(SubstituteBufId);
}
#endif
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
#if (STD_ON == COM_RXTPPDU_SUPPORT) && (0u < COM_RXIPDU_NUMBER)
/******************************************************************************/
/*
 * Brief               TP RxPdu RxIndication handle
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
Com_TpRxIndicationHandle(PduIdType id, Std_ReturnType result) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    Com_RxIPduRunTimeStateType* rxIpduStatePtr = &Com_RxIPduRunTimeState[id];
    PduInfoType info;
    uint8 activeEnable = rxIpduStatePtr->RxIpduRTStFlag & Com_RX_ACTIVE_EN;
    rxIpduStatePtr->RxIpduRTStFlag &= Com_RX_RECEIVING_DIS;

    if (Com_RX_ACTIVE_EN == activeEnable)
    {
        const Com_RxIPduType* rxIpduPtr = &Com_ConfigStd->ComRxIPdu[id];
        Com_RxIpduBufIdType rxPduBufferId = rxIpduPtr->ComRxIPduBufIndex;
        /*TP PDU successful receipt*/
        if (E_OK == result)
        {
            rxIpduStatePtr->RxIpduLength = (uint16)rxIpduStatePtr->RxOffset;
#if (                                                                                             \
    (STD_ON == COM_RX_IPDU_CALLOUT_ENABLE)                                                        \
    || (((STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE) || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)) \
        && (COM_RXSIGNAL_NUMBER + COM_RXSIGNALGROUP_NUMBER > 0u)))
            info.SduDataPtr = &Com_RxIPduRuntimeBuff[rxPduBufferId];
            info.SduLength = rxIpduStatePtr->RxIpduLength;
#endif
#if (STD_ON == COM_METADATA_SUPPORT)
            uint8 metaDataLength = rxIpduPtr->MetaDataLength;
            if (0u < metaDataLength)
            {
                uint16 metaDataPos = (uint16)rxIpduPtr->IPduNoDynSignalLength
                                     + (uint16)rxIpduPtr->IPduMaxDynSignalLength + rxPduBufferId;
                info.MetaDataPtr = &Com_RxIPduRuntimeBuff[metaDataPos];
            }
            else
#endif
            {
                info.MetaDataPtr = NULL_PTR;
            }
#if ((STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE) || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE))
            uint8 DMEnable = rxIpduStatePtr->RxIpduRTStFlag & Com_RX_DM_EN;
            if (Com_RX_DM_EN == DMEnable)
            {
/*reset all signals and signal groups DM time(except the signal/signal group have update bit and the update bit is not
 * 1)*/
#if (COM_RXSIGNAL_NUMBER + COM_RXSIGNALGROUP_NUMBER > 0u)
                Com_ResetUpdateDMTime(id, &info);
#endif
            }
#endif /* STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE || STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE */
#if (STD_ON == COM_RX_IPDU_COUNTER_ENABLE)
            /*not configuration rx counter*/
            boolean receiveCounterIsValid = TRUE;
            if (COM_UNUSED_RXIPDUCOUNTERID != rxIpduPtr->ComIPduCounterIndex)
            {
                /*unpack the receive pdu's counter*/
                uint8 receiveCounter = Com_UnPackCounterValue(rxIpduPtr->ComIPduCounterIndex, rxPduBufferId);
                /*judge the receive pdu's counter is valid or not*/
                receiveCounterIsValid = Com_RxPduCounterValid(id, receiveCounter);
            }
            /*not configuration rx counter*/
            if (receiveCounterIsValid)
#endif /* STD_ON == COM_RX_IPDU_COUNTER_ENABLE */
            {
#if (STD_ON == COM_RX_IPDU_CALLOUT_ENABLE)
                boolean ret = TRUE;
                if (NULL_PTR != rxIpduPtr->ComIPduCallout)
                {
                    ret = rxIpduPtr->ComIPduCallout(id, &info);
                }
                if (ret)
#endif /* STD_ON == COM_RX_IPDU_CALLOUT_ENABLE */
                {
                    /*signal/signal group RxIndication is made immediate*/
#if (STD_ON == COM_RX_IPDU_SIGNAL_PROCESS_IMMEDIATE_ENABLE)
                    if (COM_IMMEDIATE == rxIpduPtr->ComIPduSignalProcessing)
                    {
                        rxIpduStatePtr->RxIpduRTStFlag &= Com_RX_INDICATION_DIS;
                        /*rx indication handle*/
                        Com_IndicationProcess(rxIpduStatePtr, rxIpduPtr);
                    }
                    /*signal/signal group RxIndication is made in next MainFunctionRx*/
                    else
#endif /* STD_ON == COM_RX_IPDU_SIGNAL_PROCESS_IMMEDIATE_ENABLE */
                    {
                        rxIpduStatePtr->RxIpduRTStFlag |= Com_RX_INDICATION_EN;
                    }
                }
            }
        }
        /*TP PDU false receipt:the AUTOSAR COM module shall treat all signals of the
         * received large I-PDU exactly like invalidated signals.*/
        else
        {
            rxIpduStatePtr->RxIpduLength = Com_RxIPduInitState[id].RxIpduLength;
            rxPduBufferId = rxIpduPtr->ComRxIPduBufIndex;
            SchM_Enter_Com_Context();
            (void)ILib_memcpy(
                &Com_RxIPduRuntimeBuff[rxPduBufferId],
                &Com_RxIPduInitValue[rxPduBufferId],
                Com_RxIPduInitState[id].RxIpduLength);
            SchM_Exit_Com_Context();
#if (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE)
            /*execute all configured ComDataInvalidActions for all included signals and signal groups*/
            Com_TpPduInvalidAction(id);
#endif /* STD_ON == COM_RX_SIGNAL_INVALID_DATA_ENABLE || STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE */
        }
    }
}
/******************************************************************************/
/*
 * Brief               TP Pdu copy receive data handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      info
 * Param-Name[out]     bufferSizePtr
 * Param-Name[in/out]  None
 * Return              BufReq_ReturnType
 */
/******************************************************************************/
/* PRQA S 1532,3432 ++ */ /* MISRA Rule 8.7,20.7 */
FUNC(BufReq_ReturnType, COM_CODE)
Com_CopyRxDataHandle(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) info,
    P2VAR(PduLengthType, AUTOMATIC, COM_APPL_DATA) bufferSizePtr)
/* PRQA S 1532,3432 -- */ /* MISRA Rule 8.7,20.7 */
{
    Com_RxIPduRunTimeStateType* rxIpduStatePtr = &Com_RxIPduRunTimeState[id];
    BufReq_ReturnType bufReq = BUFREQ_E_NOT_OK;
    uint8 activeEnable = rxIpduStatePtr->RxIpduRTStFlag & Com_RX_ACTIVE_EN;
    uint8 Receiving = rxIpduStatePtr->RxIpduRTStFlag & Com_RX_RECEIVING_EN;

    if ((Com_RX_ACTIVE_EN == activeEnable) && (Com_RX_RECEIVING_EN == Receiving)
        && ((rxIpduStatePtr->RxOffset + info->SduLength) <= rxIpduStatePtr->RxIpduLength))
    {
        const Com_RxIPduType* rxIpduPtr = &Com_ConfigStd->ComRxIPdu[id];
        Com_RxIpduBufIdType copyStartPos = rxIpduPtr->ComRxIPduBufIndex + (Com_RxIpduBufIdType)rxIpduStatePtr->RxOffset;
        /*copy the RX data to RX PDU buffer*/
        SchM_Enter_Com_Context();
        (void)ILib_memcpy(&Com_RxIPduRuntimeBuff[copyStartPos], &info->SduDataPtr[0], info->SduLength);
        SchM_Exit_Com_Context();
        rxIpduStatePtr->RxOffset += info->SduLength;
        PduLengthType rxPduMaxLength = rxIpduPtr->IPduNoDynSignalLength + rxIpduPtr->IPduMaxDynSignalLength;
        *bufferSizePtr = rxPduMaxLength - rxIpduStatePtr->RxOffset;
        bufReq = BUFREQ_OK;
    }
    return bufReq;
}
/******************************************************************************/
/*
 * Brief               TP Pdu Start receive handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      id,info,TpSduLength
 * Param-Name[out]     bufferSizePtr
 * Param-Name[in/out]  None
 * Return              BufReq_ReturnType
 */
/******************************************************************************/
/* PRQA S 1532,3432 ++ */ /* MISRA Rule,8.7,20.7 */
FUNC(BufReq_ReturnType, COM_CODE)
Com_StartOfReceptionHandle(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) info,
    PduLengthType TpSduLength,
    P2VAR(PduLengthType, AUTOMATIC, COM_APPL_DATA) bufferSizePtr)
/* PRQA S 1532,3432 -- */ /* MISRA Rule,8.7,20.7 */
{
    BufReq_ReturnType bufReq = BUFREQ_E_NOT_OK;
    Com_RxIPduRunTimeStateType* rxIpduStatePtr = &Com_RxIPduRunTimeState[id];
    uint8 activeEnable = rxIpduStatePtr->RxIpduRTStFlag & Com_RX_ACTIVE_EN;

    if (Com_RX_ACTIVE_EN == activeEnable)
    {
        const Com_RxIPduType* rxIpduPtr = &Com_ConfigStd->ComRxIPdu[id];
        PduLengthType rxPduMaxLength = rxIpduPtr->IPduNoDynSignalLength + rxIpduPtr->IPduMaxDynSignalLength;
        /*the TP PDU length is too large than the RX PDU buffer*/
        if (TpSduLength > rxPduMaxLength)
        {
            bufReq = BUFREQ_E_OVFL;
        }
        else
        {
            /*the tp pdu is not receiving and the rx pdu length >= min length of the pdu*/
            uint8 Receiving = rxIpduStatePtr->RxIpduRTStFlag & Com_RX_RECEIVING_EN;
            if ((Com_RX_RECEIVING_EN != Receiving) && (TpSduLength >= rxIpduPtr->IPduNoDynSignalLength))
            {
                rxIpduStatePtr->RxIpduLength = (uint16)TpSduLength;
                rxIpduStatePtr->RxIpduRTStFlag |= Com_RX_RECEIVING_EN;
                rxIpduStatePtr->RxOffset = (NULL_PTR == info) ? 0u : info->SduLength;
                *bufferSizePtr = rxPduMaxLength;
                Com_RxIpduBufIdType rxIPduBufIndex = rxIpduPtr->ComRxIPduBufIndex;
                if (NULL_PTR != info)
                {
                    if (NULL_PTR != info->SduDataPtr)
                    {
                        SchM_Enter_Com_Context();
                        (void)
                            ILib_memcpy(&Com_RxIPduRuntimeBuff[rxIPduBufIndex], &info->SduDataPtr[0], info->SduLength);
                        SchM_Exit_Com_Context();
                        *bufferSizePtr = rxPduMaxLength - info->SduLength;
                    }
#if (STD_ON == COM_METADATA_SUPPORT)
                    if (NULL_PTR != info->MetaDataPtr)
                    {
                        uint8 metadataLength = rxIpduPtr->MetaDataLength;
                        uint16 metaDataPos = (uint16)rxPduMaxLength + (uint16)rxIPduBufIndex;
                        /*copy the metadata to RX PDU buffer*/
                        SchM_Enter_Com_Context();
                        (void)ILib_memcpy(&Com_RxIPduRuntimeBuff[metaDataPos], &info->MetaDataPtr[0], metadataLength);
                        SchM_Exit_Com_Context();
                    }
#endif
                }
                bufReq = BUFREQ_OK;
            }
        }
    }
    return bufReq;
}
#endif

#if (STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && ((0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXIPDU_NUMBER))
/******************************************************************************/
/*
 * Brief               receive SignalGroup(Array) data handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalGroupId
 * Param-Name[out]     SignalGroupArrayPtr
 * Param-Name[in/out]  None
 * Return              uint8
 */
/******************************************************************************/
/* PRQA S 3432,1532 ++ */ /* MISRA Rule 20.7,8.7 */
FUNC(uint8, COM_CODE)
Com_ReceiveSignalGroupArrayHandle(
    Com_SignalGroupIdType SignalGroupId,
    P2VAR(uint8, AUTOMATIC, COM_APPL_CONST) SignalGroupArrayPtr)
/* PRQA S 3432,1532 -- */ /* MISRA Rule 20.7,8.7 */
{
    uint8 ret = COM_SERVICE_NOT_AVAILABLE;
    const Com_RxSignalGroupType* rxSignalGroupPtr = &Com_ConfigStd->ComRxSignalGroup[SignalGroupId];
    /*the signal group is array*/
    if (COM_UNUSED_COM_SIGNALGROUPARRYID != rxSignalGroupPtr->ComSignalGroupArrayId)
    {
        PduIdType ipduRef = rxSignalGroupPtr->ComIpduRefIndex;
        const Com_RxIPduRunTimeStateType* rxIpduStatePtr = &Com_RxIPduRunTimeState[ipduRef];
        /*the I-Pdu is receiving,cann't copy data*/
        uint8 Receiving = rxIpduStatePtr->RxIpduRTStFlag & Com_RX_RECEIVING_EN;
        /*IPDU is receiving data*/
        if (Com_RX_RECEIVING_EN == Receiving)
        {
            ret = COM_BUSY;
        }
        else
        {
            uint16 iPduSignalGroupArrayStartBufId =
                Com_SignalGroupArray[rxSignalGroupPtr->ComSignalGroupArrayId].ComIPduStartBufIndex;
            uint16 signalGroupArrayLength =
                Com_SignalGroupArray[rxSignalGroupPtr->ComSignalGroupArrayId].SignalGroupArrayLength;
            SchM_Enter_Com_Context();
            (void)ILib_memcpy(
                &SignalGroupArrayPtr[0],
                &Com_RxIPduRuntimeBuff[iPduSignalGroupArrayStartBufId],
                signalGroupArrayLength);
            SchM_Exit_Com_Context();
            uint8 activeEnable = rxIpduStatePtr->RxIpduRTStFlag & Com_RX_ACTIVE_EN;
            if (Com_RX_ACTIVE_EN == activeEnable)
            {
                ret = E_OK;
            }
        }
    }
    return ret;
}
#endif

#if (COM_IPDUGROUP_MAX > 0u) && (COM_RXIPDUGROUP_NUMBER > 0u)
/******************************************************************************/
/*
 * Brief               used to handle all Rx Ipdu state change or not
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      initialize
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, COM_CODE)
Com_RxIpduController(boolean initialize) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    Com_RxIPduRunTimeStateType* rxIpduStatePtr;
    const Com_RxIPduType* rxIpduPtr;
    PduIdType ipduCnt;
    Com_IpduGroupIdType ipduGroupRefNumber;
    Com_IpduGroupIdType index;
    Com_IpduGroupIdType ipduGroupId;
    uint8 activeEnable;

    for (ipduCnt = 0u; ipduCnt < COM_RXIPDU_NUMBER; ++ipduCnt)
    {
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
        if (Com_GetRxIPduPartitionId(ipduCnt) == GetApplicationID())
#endif
        {
            rxIpduPtr = &Com_ConfigStd->ComRxIPdu[ipduCnt];
            ipduGroupRefNumber = rxIpduPtr->ComIPduGroupsRefNumber;
            /*if the Pdu not included in any Ipdu Group,the state is always start*/
            if (ipduGroupRefNumber > 0u)
            {
                index = rxIpduPtr->ComIPduGroupsRefStartId;
                rxIpduStatePtr = &Com_RxIPduRunTimeState[ipduCnt];
                activeEnable = rxIpduStatePtr->RxIpduRTStFlag & Com_RX_ACTIVE_EN;
                for (; index < ipduGroupRefNumber; ++index)
                {
                    ipduGroupId = Com_RxIPduGroupsRef[index];
                    if (Com_IpduGroupEnable[ipduGroupId])
                    {
                        break;
                    }
                }
                /*at least one Ipdu Group is start,the Pdu shall be active*/
                if (index < ipduGroupRefNumber)
                {
                    /*the pdu restart*/
                    if (Com_RX_ACTIVE_EN != activeEnable)
                    {
                        SchM_Enter_Com_MultiCore_Lock();
                        rxIpduStatePtr->RxIpduRTStFlag |= Com_RX_ACTIVE_EN;
                        SchM_Exit_Com_MultiCore_Lock();
/*reset rx i-pdu(all signal/signal group) DM Counter*/
#if ((COM_RXSIGNAL_NUMBER + COM_RXSIGNALGROUP_NUMBER) > 0u)
                        Com_ResetRxPduTimeOut(ipduCnt);
#endif
/*reset OCCURRENCE of filter with ComFilterAlgorithm ONE_EVERY_N*/
#if (COM_ONEEVERYNFILTERSIGNAL_NUMBER > 0u)
                        Com_ResetRxOccurrenceOfPduFilter(rxIpduPtr);
#endif
                        if (initialize)
                        {
                            /*init the rx ipdu buffer,all signal buffer(included in the ipdu,except source
                             * signal)*/
                            Com_ResetRxPduBufferAndSignalBuffer(ipduCnt);
#if (STD_ON == COM_RX_IPDU_COUNTER_ENABLE)
                            if (COM_UNUSED_RXIPDUCOUNTERID != rxIpduPtr->ComIPduCounterIndex)
                            {
                                /*accept for I-PDUs with ComIPduDirection configured to RECEIVED any next
                                 * incoming I-PDU counter*/
                                rxIpduStatePtr->RxIpduRTStFlag |= Com_RX_COUNTER_PDU_EN;
#if (STD_ON == COM_IPDU_REPLICATION_ENABLE)
                                rxIpduStatePtr->RxReplicationNumber = 0u;
#endif
                            }
#endif
                        }
                    }
                }
                else
                {
                    if (Com_RX_ACTIVE_EN == activeEnable)
                    {
                        SchM_Enter_Com_MultiCore_Lock();
                        rxIpduStatePtr->RxIpduRTStFlag &= Com_RX_ACTIVE_DIS;
                        SchM_Exit_Com_MultiCore_Lock();
/*disable rx i-pdu(all signal/signal group) DM Counter*/
#if ((COM_RXSIGNAL_NUMBER + COM_RXSIGNALGROUP_NUMBER) > 0u)
                        Com_DisableRxPduTimeOut(ipduCnt);
#endif
                    }
                }
            }
        }
    }
}
#endif

#if (0u < COM_RXIPDU_NUMBER)
/******************************************************************************/
/*
 * Brief               IF RxPdu RxIndication handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      RxPduId,PduInfoPtr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
FUNC(void, COM_CODE)
Com_RxIndicationHandle(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    const Com_RxIPduType* rxIpduPtr = &Com_ConfigStd->ComRxIPdu[RxPduId];
    const Com_RxIPduRunTimeStateType* rxIpduStatePtr = &Com_RxIPduRunTimeState[RxPduId];
    uint16 minPduLength = (uint16)rxIpduPtr->IPduNoDynSignalLength;
    uint16 maxPduLength = minPduLength + (uint16)rxIpduPtr->IPduMaxDynSignalLength;
    uint8 activeEnable = rxIpduStatePtr->RxIpduRTStFlag & Com_RX_ACTIVE_EN;

    /*the rx pdu is active,the receive length is suited to pdu configuration length*/
    if ((Com_RX_ACTIVE_EN == activeEnable) && (PduInfoPtr->SduLength >= minPduLength)
        && (PduInfoPtr->SduLength <= maxPduLength))
    {
#if ((STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE) || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE))
        uint8 DMEnable = rxIpduStatePtr->RxIpduRTStFlag & Com_RX_DM_EN;
        if (Com_RX_DM_EN == DMEnable)
        {
            /*reset all signals and signal groups DM time(except the signal/signal group have update bit and the
             * update bit is not 1)*/
            Com_ResetUpdateDMTime(RxPduId, PduInfoPtr);
        }
#endif /* STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE || STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE */
#if (STD_ON == COM_RX_IPDU_COUNTER_ENABLE)
        Com_RxIPduCounterIdType ipduCounterId = rxIpduPtr->ComIPduCounterIndex;
        if (COM_UNUSED_RXIPDUCOUNTERID != ipduCounterId)
        {
            uint32 bitPosition = Com_RxIPduCounter[ipduCounterId].ComIPduCounterStartPosition;
            uint8 bitSize = Com_RxIPduCounter[ipduCounterId].ComIPduCounterSize;
            uint8 receiveCounter;
            boolean rxPduValid;
            /*unpack the receive pdu's counter*/
            receiveCounter = PduInfoPtr->SduDataPtr[(uint16)(bitPosition >> 3u)];
            receiveCounter = receiveCounter << (8u - (((uint8)bitPosition & 0x07u) + bitSize));
            receiveCounter = receiveCounter >> (8u - bitSize);
#if (STD_ON == COM_RX_IPDU_COUNTER_ENABLE) && (STD_ON == COM_IPDU_REPLICATION_ENABLE)
            if (COM_UNUSED_UINT8 != Com_IPduReplication[ipduCounterId].ComIPduReplicationQuorum)
            {
                /*judge the receive pdu's Replication is valid or not*/
                rxPduValid = Com_RxPduReplicationValid(RxPduId, receiveCounter, PduInfoPtr);
            }
            else
#endif /* STD_ON == COM_IPDU_REPLICATION_ENABLE */
            {
                /*judge the receive pdu's counter is valid or not*/
                rxPduValid = Com_RxPduCounterValid(RxPduId, receiveCounter);
                if (rxPduValid)
                {
                    Com_CopyPduDataToRxBuffer(RxPduId, PduInfoPtr);
                }
            }
            if (rxPduValid)
            {
                Com_RxPduHandle(RxPduId, PduInfoPtr);
            }
        }
        else
#endif /* STD_ON == COM_RX_IPDU_COUNTER_ENABLE */
        {
            Com_CopyPduDataToRxBuffer(RxPduId, PduInfoPtr);
            Com_RxPduHandle(RxPduId, PduInfoPtr);
        }
    }
}
#endif

#if (0u < COM_RXIPDU_NUMBER)
/******************************************************************************/
/*
 * Brief               RxPdu MainFunction handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
void Com_MainFunctionRxHandle(Com_MainFunctionType mainFunctionId) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    const Com_RxIPduType* rxIpduPtr;
    Com_RxIPduRunTimeStateType* rxIpduStatePtr;
    PduIdType rxIpduId = Com_GetStartOfMainFunctionRx(mainFunctionId);
    uint8 DMEnable;
    uint8 activeEnable;
    uint8 RxIndication;
    for (; rxIpduId < Com_GetEndOfMainFunctionRx(mainFunctionId); ++rxIpduId)
    {
        rxIpduStatePtr = &Com_RxIPduRunTimeState[rxIpduId];
        rxIpduPtr = &Com_ConfigStd->ComRxIPdu[rxIpduId];
        activeEnable = rxIpduStatePtr->RxIpduRTStFlag & Com_RX_ACTIVE_EN;
        if (Com_RX_ACTIVE_EN == activeEnable)
        {
            RxIndication = rxIpduStatePtr->RxIpduRTStFlag & Com_RX_INDICATION_EN;
            /*signal/signal group RxIndication is DEFERRED in the MainFunctionRx*/
            if (Com_RX_INDICATION_EN == RxIndication)
            {
                /*rx indication handle*/
                Com_IndicationProcess(rxIpduStatePtr, rxIpduPtr);
                rxIpduStatePtr->RxIpduRTStFlag &= Com_RX_INDICATION_DIS;
                /*the rx pdu include signal/signal group which need gateway*/
            }
            /*if the rx DM is enabled*/
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE) || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)
            DMEnable = rxIpduStatePtr->RxIpduRTStFlag & Com_RX_DM_EN;
            if (Com_RX_DM_EN == DMEnable)
            {
#if (COM_RXSIGNAL_NUMBER > 0u) && (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE)
                if (COM_RX_SIGNAL_DM == (rxIpduStatePtr->RxIpduDMFlag & COM_RX_SIGNAL_DM))
                {
                    Com_RxSignalTimeOutHandle(rxIpduStatePtr, rxIpduPtr);
                }
#endif

#if (COM_RXSIGNALGROUP_NUMBER > 0u) && (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)
                if (COM_RX_SIGNAL_GROUP_DM == (rxIpduStatePtr->RxIpduDMFlag & COM_RX_SIGNAL_GROUP_DM))
                {
                    Com_RxSignalGroupTimeOutHandle(rxIpduStatePtr, rxIpduPtr);
                }
#endif

#if ((0u < COM_RXSIGNAL_NUMBER) || (0u < COM_RXGROUPSIGNAL_NUMBER))
                if ((COM_RX_IPDU_DM_ONLY == (rxIpduStatePtr->RxIpduDMFlag & COM_RX_IPDU_DM_ONLY))
                    || (COM_RX_IPDU_DM == (rxIpduStatePtr->RxIpduDMFlag & COM_RX_IPDU_DM)))
                {
                    Com_RxIpduBaseDMTimeOutHandle(rxIpduStatePtr, rxIpduPtr);
                }
#endif
            }
#endif
        }
    }
}
#endif
#if (((COM_RXSIGNALGROUP_NUMBER > 0u) && (COM_RXGROUPSIGNAL_NUMBER > 0u)) && (0u < COM_RXGROUPSIGNAL_8BITBUFF_SIZE)) \
    && (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE) && (0u < COM_RXIPDU_NUMBER)
/******************************************************************************/
/*
 * Brief               Receive Dynamic Group Signal handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalId
 * Param-Name[out]     SignalDataPtr
 * Param-Name[in/out]  Length
 * Return              uint8
 */
/******************************************************************************/
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
FUNC(uint8, COM_CODE)
Com_ReceiveDynGroupSignalHandle(
    Com_SignalIdType SignalId,
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(void, AUTOMATIC, COM_APPL_CONST) SignalDataPtr,
    P2VAR(uint16, AUTOMATIC, COM_APPL_CONST) Length)
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
{
    PduIdType ipduRef;
    uint16 signalLength;
    uint16 signalInitValueId;
    uint8 ret = COM_SERVICE_NOT_AVAILABLE;
    Com_RxSignalGroupIdType signalGroupId;
    Com_SignalIdType id = SignalId - COM_RXSIGNAL_NUMBER;

    /* Check IPdu which this group signal belonged is Active or not,the group signal type must be
     * COM_UINT8_DYN*/
    if (COM_UINT8_DYN == Com_ConfigStd->ComRxGroupSignal[id].ComSignalType)
    {
        signalGroupId = Com_ConfigStd->ComRxGroupSignal[id].ComSignalGroupRef;
        ipduRef = Com_ConfigStd->ComRxSignalGroup[signalGroupId].ComIpduRefIndex;
        Com_RxIPduRunTimeStateType* rxIpduStatePtr = &Com_RxIPduRunTimeState[ipduRef];
        uint8 Receiving = rxIpduStatePtr->RxIpduRTStFlag & Com_RX_RECEIVING_EN;
        /*IPDU is not receiving data*/
        if (Com_RX_RECEIVING_EN != Receiving)
        {
            signalLength = (rxIpduStatePtr->RxIpduLength - Com_ConfigStd->ComRxIPdu[ipduRef].IPduNoDynSignalLength);
            if (signalLength > (*Length))
            {
#if (STD_ON == COM_DEV_ERROR_DETECT)
                (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_RECEIVEDYNSIGNAL_ID, COM_E_PARAM);
#endif
                ret = E_NOT_OK;
            }
            else
            {
                signalInitValueId = Com_ConfigStd->ComRxGroupSignal[id].ComSignalInitValueId;
                /* Copy Uint8DYN signal value one byte by one */
                (void)ILib_memcpy(
                    (uint8*)SignalDataPtr,
                    &Com_RxGroupSignal8BitShadowBuff[signalInitValueId],
                    signalLength);
                *Length = signalLength;
                /* Check IPdu which this group signal belonged is Active or not*/
                uint8 activeEnable = rxIpduStatePtr->RxIpduRTStFlag & Com_RX_ACTIVE_EN;
                if (Com_RX_ACTIVE_EN == activeEnable)
                {
                    ret = E_OK;
                }
            }
        }
        else
        {
            ret = COM_BUSY;
        }
    }
    return ret;
}
#endif
#if (COM_RXIPDUGROUP_NUMBER > 0u) && (0u < COM_RXIPDU_NUMBER)
/******************************************************************************/
/*
 * Brief               Rx Pdu DM timeout enable/disable handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, COM_CODE) Com_RxPduDMCtrHandle(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    const Com_RxIPduType* rxIpduPtr;
    Com_RxIPduRunTimeStateType* rxIpduStatePtr;
    Com_IpduGroupIdType ipduGroupRefNumber;
    Com_IpduGroupIdType index;
    Com_IpduGroupIdType ipduGroupId;
    uint16 ipduCnt;
    uint8 DMEnable;
    for (ipduCnt = 0u; ipduCnt < COM_RXIPDU_NUMBER; ++ipduCnt)
    {
        rxIpduPtr = &Com_ConfigStd->ComRxIPdu[ipduCnt];
        index = rxIpduPtr->ComIPduGroupsRefStartId;
        ipduGroupRefNumber = rxIpduPtr->ComIPduGroupsRefNumber;
        /*if the Rx Pdu not included in any Ipdu Group,the DM state is always disabled*/
        if (ipduGroupRefNumber > 0u)
        {
            rxIpduStatePtr = &Com_RxIPduRunTimeState[ipduCnt];
            DMEnable = rxIpduStatePtr->RxIpduRTStFlag & Com_RX_DM_EN;
            for (; index < ipduGroupRefNumber; ++index)
            {
                ipduGroupId = Com_RxIPduGroupsRef[index];
                if (Com_IpduGroupDMEnable[ipduGroupId])
                {
                    break;
                }
            }
            if (index < ipduGroupRefNumber)
            {
                /*DM state changed from disabled to enabled*/
                if (Com_RX_DM_EN != DMEnable)
                {
                    rxIpduStatePtr->RxIpduRTStFlag |= Com_RX_DM_EN;
/*set the reception deadline monitoring timer for the included signals and signal groups to the configured
 * ComFirstTimeout*/
#if (COM_RXSIGNAL_NUMBER + COM_RXSIGNALGROUP_NUMBER > 0u)
                    Com_ResetRxPduTimeOut(ipduCnt);
#endif
                }
            }
            else
            {
                if (Com_RX_DM_EN == DMEnable)
                {
                    rxIpduStatePtr->RxIpduRTStFlag &= Com_RX_DM_DIS;
/*set the reception deadline monitoring timer for the included signals and signal groups to 0*/
#if (COM_RXSIGNAL_NUMBER + COM_RXSIGNALGROUP_NUMBER > 0u)
                    Com_DisableRxPduTimeOut(ipduCnt);
#endif
                }
            }
        }
    }
}
#endif
#if (COM_RXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
/******************************************************************************/
/*
 * Brief               called by Com_SignalGroupRxIndication,Com_TpPduInvalidAction.
 *                     judge the rx signal group value is filter out or not.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      RxSignalGroupPtr,InvalidSignalGroup,IpduBufferId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/******************************************************************************/
/* called by Com_SignalGroupRxIndication,Com_TpPduInvalidAction.
 * judge the rx signal group value is filter out or not*/
FUNC(boolean, COM_CODE)
Com_RxSignalGroupFilter(
    const Com_RxSignalGroupType* RxSignalGroupPtr,
    boolean InvalidSignalGroup,
    Com_RxIpduBufIdType IpduBufferId)
{
    const Com_RxGroupSignalType* rxGroupSignalPtr;
    uint64 groupSignalValue;
    uint16 groupSignalInitValueId;
    Com_SignalType groupSignalType;
    boolean signalGroupFilter = TRUE;
    Com_RxGroupSignalIdType groupSignalNumber = RxSignalGroupPtr->ComGroupSignalRefNumber;
    Com_RxGroupSignalIdType cnt = RxSignalGroupPtr->ComGroupSignalRefStartId;

    for (; (cnt < groupSignalNumber) && (TRUE == signalGroupFilter); ++cnt)
    {
        rxGroupSignalPtr = &Com_ConfigStd->ComRxGroupSignal[cnt];
        groupSignalType = rxGroupSignalPtr->ComSignalType;
/*Other Signal types besides  COM_UINT8_DYN or COM_UINT8_DYN*/
#if (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
        if ((groupSignalType != COM_UINT8_N) && (groupSignalType != COM_UINT8_DYN))
#endif /* STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE || STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE \
        */
        {
            groupSignalInitValueId = rxGroupSignalPtr->ComSignalInitValueId;
#if (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE)
            /*the signal group value is invalid,use the init value replace*/
            if (InvalidSignalGroup)
            {
                /*get the init value except COM_UINT8_N and UNIT8_DYN*/
                groupSignalValue = Com_RxSignalReplaceHanlde(groupSignalType, groupSignalInitValueId);
            }
            else
#endif /* STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE */
            {
                /*get the init value except COM_UINT8_N and UNIT8_DYN*/
                groupSignalValue = Com_SignalUnPackHandle(
#if (STD_ON == COM_SIGNAL_SIGNED_TYPE_ENABLE)
                    groupSignalType,
#endif
                    COM_RX_GROUP_SIGNAL,
                    rxGroupSignalPtr,
                    &Com_RxIPduRuntimeBuff[IpduBufferId]);
            }
#if (COM_RXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
            signalGroupFilter = Com_RxSignalFilter(
                groupSignalType,
                rxGroupSignalPtr->ComFilterAlgorithm,
                (Com_SignalIdType)rxGroupSignalPtr->ComFilterIndex,
#if (COM_RXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
                groupSignalInitValueId,
#endif
                groupSignalValue);
#endif
        }
/*COM_UINT8_N or COM_UINT8_DYN*/
#if (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
        else
        {
#if (COM_RXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
            if ((COM_UNUSED_RXGRPSIGFILTERID != rxGroupSignalPtr->ComFilterIndex)
                && (COM_NEVER == rxGroupSignalPtr->ComFilterAlgorithm))
            {
                signalGroupFilter = FALSE;
            }
#endif
        }
#endif /* STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE || STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE \
        */
    }

    return signalGroupFilter;
}
#endif
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
/*******************************************************************************
**                      End of file                                           **
*******************************************************************************/
