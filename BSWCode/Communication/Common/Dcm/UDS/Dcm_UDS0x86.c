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
**************************************************************************** **
**                                                                           **
**  FILENAME    :                                                            **
**                                                                           **
**  Created on  : 2020/5/6 14:29:43                                          **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION :                                                            **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */
#include "Dcm_Internal.h"

/****************************************************************
                 UDS:ResponseOnEvent (86 hex) service
 ***************************************************************/
#if (STD_ON == DCM_UDS_SERVICE0X86_ENABLED)

#define DCM_UDS86_SUBFUNCTION_MASK      0x3Fu
#define DCM_UDS86_SUBFUNCTION_BIT6_MASK 0x40u

#define DCM_UDS86_DO_NOT_STORE_EVENT    (boolean)0x00u
#define DCM_UDS86_STORE_EVENT           (boolean)0x01u

typedef struct
{
    Dcm_RoeEventStatesTypes Dcm_RoeEventStates;
    boolean Dcm_RoeStoreEvent;
    uint8 EventWindowTime;
    uint8 DTCStatusMask;
    uint16 DcmDspRoeDidRef;
    uint8 ProtocolCtrlId;
    Dcm_SesCtrlType ROEActiveSes;
    uint8 RoeCnt;
    uint16 SA;
    uint16 TA;
} Dcm_RoeStatesTypes;

typedef struct
{
    Dcm_RoeStatesTypes Dcm_RoeStates[DCM_DSP_ROE_NUM];
    uint8 ProtocolCtrlId;
} Dcm_RoeRuntimeTypes;

typedef struct
{
    uint8 Data[3];
    PduLengthType Length;
    boolean Dcm_DspRoeNeedSend;
    boolean IsRoeMessage;
    PduIdType pduRxId;
    uint8 RoeId;
    boolean NeedStore;
} Dcm_DspRoeNeedSendTypes;

#define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"
static VAR(Dcm_RoeRuntimeTypes, DCM_VAR_NOINIT) Dcm_RoeRuntime;
#define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_NO_INIT_32
#include "Dcm_MemMap.h"
static VAR(uint32, DCM_VAR_NOINIT) Dcm_DspRoeInterMessageTime;
#define DCM_STOP_SEC_VAR_NO_INIT_32
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"
static VAR(Dcm_DspRoeNeedSendTypes, DCM_VAR_NOINIT) Dcm_DspRoeNeedSend;
#define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
#if (NVM_ENABLE == STD_ON)
static FUNC(void, DCM_CODE) Dcm_UDS0x86_StoreRoe(void)
{
    NvM_RequestResultType NvmRet;
    (void)NvM_GetErrorStatus(DCM_ROE_STORAGE_BLOCKID, &NvmRet);
    if (NVM_REQ_PENDING != NvmRet)
    {
        Dcm_DspRoeNeedSend.NeedStore = FALSE;
        (void)NvM_WriteBlock(DCM_ROE_STORAGE_BLOCKID, (void*)&Dcm_RoeRuntime);
    }
    else
    {
        Dcm_DspRoeNeedSend.NeedStore = TRUE;
    }
}
#else
#define Dcm_UDS0x86_StoreRoe(void)
#endif

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
static FUNC(void, DCM_CODE) Dcm_Uds0X86_CheckEventWindowTime(uint8 RoeEventId, boolean SubRequest)
{
    boolean end = FALSE;
    Dcm_RoeStatesTypes* pDcm_RoeStates = &Dcm_RoeRuntime.Dcm_RoeStates[RoeEventId];
    switch (pDcm_RoeStates->EventWindowTime)
    {
    case (uint8)DCM_ROE_EVENT_WINDOW_CURRENT_AND_FOLLOWING_CYCLE:
        if (DCM_DEFAULT_SESSION == pDcm_RoeStates->ROEActiveSes)
        {
            pDcm_RoeStates->EventWindowTime = (uint8)DCM_ROE_EVENT_WINDOW_CURRENT_CYCLE;
        }
        break;
    case (uint8)DCM_ROE_EVENT_WINDOW_CURRENT_CYCLE:
        if ((DCM_DEFAULT_SESSION == pDcm_RoeStates->ROEActiveSes) && (TRUE == SubRequest))
        {
            end = TRUE;
        }
        break;
    case (uint8)DCM_ROE_EVENT_WINDOW_INFINITE:
        if ((DCM_DEFAULT_SESSION == pDcm_RoeStates->ROEActiveSes)
            && (DCM_UDS86_DO_NOT_STORE_EVENT == pDcm_RoeStates->Dcm_RoeStoreEvent))
        {
            end = TRUE;
        }
        break;
    default:
        /*idle*/
        break;
    }
    if ((end == TRUE) && (pDcm_RoeStates->Dcm_RoeEventStates == DCM_ROE_STARTED))
    {
        pDcm_RoeStates->Dcm_RoeEventStates = DCM_ROE_STOPPED;
        pDcm_RoeStates->ROEActiveSes = DCM_ALL_SESSION_LEVEL;
    }
}

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
FUNC(void, DCM_CODE) Dcm_UDS0x86_Init(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint8 iloop;
#if (NVM_ENABLE == STD_ON)
    NvM_RequestResultType NvmRet;
    Dcm_RoeRuntimeTypes pRoeRuntime;
#endif
    Dcm_RoeStatesTypes* pDcm_RoeStates;
    const Dcm_DspRoeTypes* Dcm_DspRoe = DcmPbCfgPtr->pDcmDspCfg->Dcm_DspRoe;
    const Dcm_DspRoeEventPropertiesType* pDcmDspRoeEventProperties;

    for (iloop = 0; iloop < DCM_DSP_ROE_NUM; iloop++)
    {
        pDcm_RoeStates = &Dcm_RoeRuntime.Dcm_RoeStates[iloop];
        pDcm_RoeStates->Dcm_RoeEventStates = Dcm_DspRoe->DcmDspRoeEvent[iloop].DcmDspRoeInitialEventStatus;
        pDcmDspRoeEventProperties = Dcm_DspRoe->DcmDspRoeEvent[iloop].DcmDspRoeEventProperties;
        if (DCM_ROE_STOPPED == pDcm_RoeStates->Dcm_RoeEventStates)
        {
            pDcm_RoeStates->Dcm_RoeStoreEvent = DCM_UDS86_STORE_EVENT;
            pDcm_RoeStates->EventWindowTime = (uint8)DCM_ROE_EVENT_WINDOW_INFINITE;
            pDcm_RoeStates->ROEActiveSes = DCM_ALL_SESSION_LEVEL;
            pDcm_RoeStates->RoeCnt = 0;
            if (NULL_PTR != pDcmDspRoeEventProperties->DcmDspRoeOnDTCStatusChange)
            {
                pDcm_RoeStates->DTCStatusMask =
                    pDcmDspRoeEventProperties->DcmDspRoeOnDTCStatusChange->DcmDspRoeDTCStatusMask;
            }
            if (NULL_PTR != pDcmDspRoeEventProperties->DcmDspRoeOnChangeOfDataIdentifier)
            {
                pDcm_RoeStates->DcmDspRoeDidRef =
                    pDcmDspRoeEventProperties->DcmDspRoeOnChangeOfDataIdentifier->DcmDspRoeDidRef;
            }
        }
    }
#if ((NVM_ENABLE == STD_ON) && (DCM_ROE_STORAGE_BLOCKID != 0xFFu))
    (void)NvM_GetErrorStatus(DCM_ROE_STORAGE_BLOCKID, &NvmRet);
    if (NVM_REQ_OK == NvmRet)
    {
        Dcm_MemoryCopy(
            (uint8*)NvM_BlockDescriptor[DCM_ROE_STORAGE_BLOCKID - 1u].NvmRamBlockDataAddress,
            (uint8*)&(pRoeRuntime),
            sizeof(Dcm_RoeRuntimeTypes));
        for (iloop = 0; iloop < DCM_DSP_ROE_NUM; iloop++)
        {
            if (DCM_UDS86_STORE_EVENT == pRoeRuntime.Dcm_RoeStates[iloop].Dcm_RoeStoreEvent)
            {
                Dcm_MemoryCopy(
                    (uint8*)&(pRoeRuntime.Dcm_RoeStates[iloop]),
                    (uint8*)&(Dcm_RoeRuntime.Dcm_RoeStates[iloop]),
                    sizeof(Dcm_RoeStatesTypes));
            }
        }
    }
#endif
    for (iloop = 0; iloop < DCM_DSP_ROE_NUM; iloop++)
    {
        pDcm_RoeStates = &Dcm_RoeRuntime.Dcm_RoeStates[iloop];
        Dcm_Uds0X86_CheckEventWindowTime(iloop, FALSE);
        if ((pDcm_RoeStates->ROEActiveSes == DCM_DEFAULT_SESSION)
            && (DCM_UDS86_STORE_EVENT == pDcm_RoeStates->Dcm_RoeStoreEvent)
            && (pDcm_RoeStates->EventWindowTime == (uint8)DCM_ROE_EVENT_WINDOW_INFINITE))
        {
            pDcm_RoeStates->Dcm_RoeEventStates = DCM_ROE_STARTED;
        }
        pDcm_RoeStates->SA = 0;
        pDcm_RoeStates->TA = 0;
    }
    Dcm_DspRoeInterMessageTime = 0;
    Dcm_RoeRuntime.ProtocolCtrlId = 0xFF;
    Dcm_DspRoeNeedSend.Dcm_DspRoeNeedSend = FALSE;
    Dcm_DspRoeNeedSend.IsRoeMessage = FALSE;
    Dcm_DspRoeNeedSend.NeedStore = FALSE;
}

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
static FUNC(void, DCM_CODE) Dcm_UDS0x86_StartTimer(void)
{
    const Dcm_DspRoeTypes* Dcm_DspRoe = DcmPbCfgPtr->pDcmDspCfg->Dcm_DspRoe;
    Dcm_DspRoeInterMessageTime = Dcm_DspRoe->DcmDspRoeInterMessageTime;
}

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
FUNC(boolean, DCM_CODE)
/* PRQA S 3432,1532++ */ /* MISRA Rule 20.7,8.7 */
Dcm_UDS0x86_GetTxPduId(P2VAR(PduIdType, AUTOMATIC, DCM_VAR) pduTxId)
/* PRQA S 3432,1532-- */ /* MISRA Rule 20.7,8.7 */
{
    boolean ret = TRUE;
    uint8 iloop;
    const Dcm_DslConnectionType* pDsl_Protocol_ConnectionCfg = DcmPbCfgPtr->DslProtocolConnectionCfg;
    const Dcm_DslProtocolTxType* pDcmDslROEConnectionRef;

    if (TRUE == Dcm_DspRoeNeedSend.IsRoeMessage)
    {
        for (iloop = 0; iloop < DCM_CONNECTION_NUM; iloop++)
        {
            pDcmDslROEConnectionRef = pDsl_Protocol_ConnectionCfg[iloop].pDcmDslMainConnection->pDcmDslROEConnectionRef;
            if ((Dcm_RoeRuntime.ProtocolCtrlId == pDsl_Protocol_ConnectionCfg[iloop].DcmDslParentProtocolRowCtrlId)
                && (NULL_PTR != pDcmDslROEConnectionRef))
            {
                ret = FALSE;
                *pduTxId = pDcmDslROEConnectionRef->DcmDslTxPduRPduId;
                break;
            }
        }
    }
    return ret;
}

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(BufReq_ReturnType, DCM_CODE) Dcm_UDS0x86_CheckProcessRoe(uint8 ProtocolCtrlId)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    BufReq_ReturnType bufRet = BUFREQ_OK;
#if (PREEMPTION_PROTOCOL_CANCEL_SUPPORT == STD_ON)
    PduIdType DcmTxPduIdx;
#endif
    if ((TRUE == Dcm_DspRoeNeedSend.IsRoeMessage) && (ProtocolCtrlId == Dcm_RoeRuntime.ProtocolCtrlId)
        && (Dcm_MsgCtrl[Dcm_RoeRuntime.ProtocolCtrlId].Dcm_MsgState != DCM_MSG_WAIT))
    {
        bufRet = BUFREQ_E_NOT_OK;
    }
    else if ((TRUE == Dcm_DspRoeNeedSend.IsRoeMessage) && (ProtocolCtrlId != Dcm_RoeRuntime.ProtocolCtrlId))
    {
#if (PREEMPTION_PROTOCOL_CANCEL_SUPPORT == STD_ON)
        DcmTxPduIdx = Dcm_MsgCtrl[Dcm_ProtocolCtrl[Dcm_RoeRuntime.ProtocolCtrlId].MsgCtrlIndex].DcmTxPduId;
        if (DCM_MSG_TRANSMISSION == Dcm_MsgCtrl[Dcm_RoeRuntime.ProtocolCtrlId].Dcm_MsgState)
        {
            (void)PduR_DcmCancelTransmit(DcmTxPduIdx);
        }
#endif
        DslInternal_ResetResource(Dcm_RoeRuntime.ProtocolCtrlId);
        Dcm_DspRoeNeedSend.IsRoeMessage = FALSE;
    }
    else
    {
        /*idle*/
    }

    return bufRet;
}

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
FUNC(boolean, DCM_CODE) Dcm_UDS0x86_TxConfirmation(uint8 ProtocolCtrlId) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    boolean ret = TRUE;
    if ((ProtocolCtrlId == Dcm_RoeRuntime.ProtocolCtrlId) && (TRUE == Dcm_DspRoeNeedSend.IsRoeMessage))
    {
        Dcm_RoeRuntime.Dcm_RoeStates[Dcm_DspRoeNeedSend.RoeId].RoeCnt++;
        Dcm_UDS0x86_StartTimer();
        DslInternal_ResetResource(ProtocolCtrlId);
        ret = FALSE;
        Dcm_DspRoeNeedSend.IsRoeMessage = FALSE;
    }
    return ret;
}

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
FUNC(void, DCM_CODE) Dcm_UDS0x86_CheckTimer(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    PduInfoType PduBuffer;
    PduInfoType* PduInfo = &PduBuffer;
    uint8 iloop;
    uint8 MsgCtrlId;
    boolean Find = FALSE;
#if (STD_ON == DCM_GENERIC_CONNECTION)
    uint8 index;
    const Dcm_DslConnectionType* pDsl_Protocol_ConnectionCfg = DcmPbCfgPtr->DslProtocolConnectionCfg;
#endif
    uint8 RoeId;
    if (Dcm_DspRoeInterMessageTime > Dcm_GeneralCfg.DcmTaskTime)
    {
        Dcm_DspRoeInterMessageTime -= Dcm_GeneralCfg.DcmTaskTime;
    }
    else
    {
        Dcm_DspRoeInterMessageTime = 0;
        /*check all protocol whether have one protocol had already started*/
        for (iloop = 0; iloop < DCM_DSLPROTOCOLROW_NUM_MAX; iloop++)
        {
            MsgCtrlId = Dcm_ProtocolCtrl[iloop].MsgCtrlIndex;
            if ((MsgCtrlId != 0xFFu) && (DCM_MSG_WAIT != Dcm_MsgCtrl[MsgCtrlId].Dcm_MsgState))
            {
                Find = TRUE; /*have finded a starting protocol*/
                break;
            }
        }
        if ((Dcm_DspRoeNeedSend.Dcm_DspRoeNeedSend == TRUE) && (Find == FALSE))
        {
            Dcm_DspRoeNeedSend.Dcm_DspRoeNeedSend = FALSE;
            Dcm_DspRoeNeedSend.IsRoeMessage = TRUE;
            PduInfo->SduDataPtr = Dcm_DspRoeNeedSend.Data;
            PduInfo->SduLength = Dcm_DspRoeNeedSend.Length;
#if (STD_ON == DCM_GENERIC_CONNECTION)
            for (iloop = 0; iloop < DCM_CONNECTION_NUM; iloop++)
            {
                if (Dcm_RoeRuntime.ProtocolCtrlId == pDsl_Protocol_ConnectionCfg[iloop].DcmDslParentProtocolRowCtrlId)
                {
                    const Dcm_DslMainConnectionType* pDcmDslMainConnection =
                        pDsl_Protocol_ConnectionCfg[iloop].pDcmDslMainConnection;
                    for (index = 0; index < pDcmDslMainConnection->DcmDslProtocolRx_Num; index++)
                    {
                        if (Dcm_DspRoeNeedSend.pduRxId
                            == pDcmDslMainConnection->pDcmDslProtocolRx[index].DcmDslProtocolRxPduId)
                        {
                            break;
                        }
                    }
                    break;
                }
            }
            if ((pDsl_Protocol_ConnectionCfg[iloop].pDcmDslMainConnection->pDcmDslProtocolRx->DcmDslMetaDataFlag
                 & DCM_METADATA_LENGTH_MASK)
                > 0u)
            {
                RoeId = Dcm_DspRoeNeedSend.RoeId;
                PduInfo->MetaDataPtr = &(Dcm_Metadata[Dcm_RoeRuntime.ProtocolCtrlId][0]);
#if (DCM_CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
                PduInfo->MetaDataPtr[0u] = (uint8)((Dcm_RoeRuntime.Dcm_RoeStates[RoeId].SA & 0xFF00u) >> 8u);
                PduInfo->MetaDataPtr[1u] = (uint8)(Dcm_RoeRuntime.Dcm_RoeStates[RoeId].SA & 0xFFu);
                PduInfo->MetaDataPtr[2u] = (uint8)((Dcm_RoeRuntime.Dcm_RoeStates[RoeId].TA & 0xFF00u) >> 8u);
                PduInfo->MetaDataPtr[3u] = (uint8)(Dcm_RoeRuntime.Dcm_RoeStates[RoeId].TA & 0xFFu);
#else
                PduInfo->MetaDataPtr[0u] = (uint8)(Dcm_RoeRuntime.Dcm_RoeStates[RoeId].SA & 0xFFu);
                PduInfo->MetaDataPtr[1u] = (uint8)((Dcm_RoeRuntime.Dcm_RoeStates[RoeId].SA & 0xFF00u) >> 8u);
                PduInfo->MetaDataPtr[2u] = (uint8)(Dcm_RoeRuntime.Dcm_RoeStates[RoeId].TA & 0xFFu);
                PduInfo->MetaDataPtr[3u] = (uint8)((Dcm_RoeRuntime.Dcm_RoeStates[RoeId].TA & 0xFF00u) >> 8u);
#endif
            }
#else
            PduInfo->MetaDataPtr = NULL_PTR;
#endif
            (void)DslInternal_ResponseOnOneEvent(PduInfo, Dcm_DspRoeNeedSend.pduRxId);
        }
    }
    if (Dcm_DspRoeNeedSend.NeedStore == TRUE)
    {
        Dcm_UDS0x86_StoreRoe();
    }
}

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
FUNC(void, DCM_CODE) Dcm_UDS0x86_SesTranslation(Dcm_SesCtrlType NewSes) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint8 iloop;
    if (DCM_DEFAULT_SESSION == NewSes)
    {
        for (iloop = 0; iloop < DCM_DSP_ROE_NUM; iloop++)
        {
            /*All RoeEvents which have been in ROE started state when leaving
             * the default session shall change back into ROE started state
             * when (re-) entering the default session.*/
            if (NewSes == Dcm_RoeRuntime.Dcm_RoeStates[iloop].ROEActiveSes)
            {
                Dcm_RoeRuntime.Dcm_RoeStates[iloop].Dcm_RoeEventStates = DCM_ROE_STARTED;
            }
        }
    }
    else
    {
        for (iloop = 0; iloop < DCM_DSP_ROE_NUM; iloop++)
        {
            /*dBy leaving the current session all started RoeEvents
             * shall change to the ROE stopped state.*/
            if (Dcm_RoeRuntime.Dcm_RoeStates[iloop].Dcm_RoeEventStates == DCM_ROE_STARTED)
            {
                Dcm_RoeRuntime.Dcm_RoeStates[iloop].Dcm_RoeEventStates = DCM_ROE_STOPPED;
            }
        }
    }
}

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
static FUNC(Std_ReturnType, DCM_CODE) Dcm_Uds0X86EventWindowTimeCheck(
    uint8 EventWindowTime,
    boolean StoreEvent,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    uint8 iloop;
    const Dcm_DspRoeTypes* Dcm_DspRoe = DcmPbCfgPtr->pDcmDspCfg->Dcm_DspRoe;

    for (iloop = 0; (iloop < Dcm_DspRoe->DcmDspRoeEventWindowTimeNum) && (ret == E_NOT_OK); iloop++)
    {
        if (EventWindowTime == (uint8)Dcm_DspRoe->DcmDspRoeEventWindowTime[iloop].DcmDspRoeEventWindowTime)
        {
            ret = E_OK;
        }
    }
    if (ret == E_NOT_OK)
    {
        /*EventWindowTime not supported,send NRC 0x31*/
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
    }
    else
    {
        if ((EventWindowTime != (uint8)DCM_ROE_EVENT_WINDOW_INFINITE) && (StoreEvent == DCM_UDS86_STORE_EVENT))
        {
            /*storageState equal to storeEvent and contains an
             * EventWindowTime that is not infinite,send NRC 0x31*/
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            ret = E_NOT_OK;
        }
    }
    return ret;
}

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
#if (DCM_UDS_SERVICE0X86_00_ENABLED == STD_ON)
static FUNC(Std_ReturnType, DCM_CODE) Dcm_Uds0X86SubDeal_0x00(
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_MsgLenType, AUTOMATIC, DCM_VAR) ResLength,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;
    uint8 iloop;
    uint8 Cnt = 0;
    uint8 MsgCtrlId;
    uint8 TxChannelCtrlIndex;
    uint8 TxChannelCfgIndex;
    uint32 Offset;

    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
    Offset = (DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg)[TxChannelCfgIndex].offset;

    for (iloop = 0; iloop < DCM_DSP_ROE_NUM; iloop++)
    {
        Dcm_Uds0X86_CheckEventWindowTime(iloop, TRUE);
        if (DCM_ROE_CLEARED == Dcm_RoeRuntime.Dcm_RoeStates[iloop].Dcm_RoeEventStates)
        {
            Cnt++;
        }
        else
        {
            Dcm_RoeRuntime.Dcm_RoeStates[iloop].Dcm_RoeEventStates = DCM_ROE_STOPPED;
            Dcm_RoeRuntime.Dcm_RoeStates[iloop].ROEActiveSes = DCM_ALL_SESSION_LEVEL;
        }
    }
    if (Cnt == DCM_DSP_ROE_NUM)
    {
        /*all RoeEvents are in ROE cleared state ,send NRC 24*/
        *ErrorCode = DCM_E_REQUESTSEQUENCEERROR;
        ret = E_NOT_OK;
    }
    else
    {
        boolean StoreEvent = (0u != (Dcm_MsgCtrl[MsgCtrlId].Subfunction & DCM_UDS86_SUBFUNCTION_BIT6_MASK))
                                 ? DCM_UDS86_STORE_EVENT
                                 : DCM_UDS86_DO_NOT_STORE_EVENT;
        if (DCM_UDS86_STORE_EVENT == StoreEvent)
        {
            Dcm_UDS0x86_StoreRoe();
        }
        Dcm_Channel[Offset + 2u] = 0;
        Dcm_Channel[Offset + 3u] = Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[2];
        *ResLength = 4;
    }
    return ret;
}
#endif

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
#if (DCM_UDS_SERVICE0X86_01_ENABLED == STD_ON)
static FUNC(Std_ReturnType, DCM_CODE) Dcm_Uds0X86SubDeal_0x01(
    uint8 ProtocolCtrlId,
    uint8 EventWindowTime,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_MsgLenType, AUTOMATIC, DCM_VAR) ResLength,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;
    uint8 iloop;
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    uint8 Mask;
    uint8 TxChannelCtrlIndex;
    uint8 TxChannelCfgIndex;
    uint32 Offset;
    Dcm_RoeStatesTypes* pDcm_RoeStates = Dcm_RoeRuntime.Dcm_RoeStates;

    boolean StoreEvent = (0u != (Dcm_MsgCtrl[MsgCtrlId].Subfunction & DCM_UDS86_SUBFUNCTION_BIT6_MASK))
                             ? DCM_UDS86_STORE_EVENT
                             : DCM_UDS86_DO_NOT_STORE_EVENT;
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
    Offset = (DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg)[TxChannelCfgIndex].offset;

    Mask = Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[3];

    if ((Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[4] == 0x19u)
        && ((Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[5] == 0x0Eu)))
    {
        for (iloop = 0; iloop < DCM_DSP_ROE_NUM; iloop++)
        {
            if (NULL_PTR
                != DcmPbCfgPtr->pDcmDspCfg->Dcm_DspRoe->DcmDspRoeEvent[iloop]
                       .DcmDspRoeEventProperties->DcmDspRoeOnDTCStatusChange)
            {
                pDcm_RoeStates[iloop].Dcm_RoeEventStates = DCM_ROE_STOPPED;
                pDcm_RoeStates[iloop].DTCStatusMask = Mask;
                pDcm_RoeStates[iloop].EventWindowTime = EventWindowTime;
                pDcm_RoeStates[iloop].Dcm_RoeStoreEvent = StoreEvent;
                break; /*can only one Roe on DTC*/
            }
        }
    }
    else
    {
        /*service not match,can only 19 0E,send NRC 31*/
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        ret = E_NOT_OK;
    }
    if (ret == E_OK)
    {
        if (DCM_UDS86_STORE_EVENT == StoreEvent)
        {
            Dcm_UDS0x86_StoreRoe();
        }
        Dcm_Channel[Offset + 2u] = 0;
        Dcm_Channel[Offset + 3u] = EventWindowTime;
        Dcm_Channel[Offset + 4u] = Mask;
        Dcm_Channel[Offset + 5u] = 0x19u;
        Dcm_Channel[Offset + 6u] = 0x0Eu;
        *ResLength = 7;
    }
    return ret;
}
#endif

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
#if (DCM_UDS_SERVICE0X86_03_ENABLED == STD_ON)
static FUNC(Std_ReturnType, DCM_CODE) Dcm_Uds0X86SubDeal_0x03(
    uint8 ProtocolCtrlId,
    uint8 EventWindowTime,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_MsgLenType, AUTOMATIC, DCM_VAR) ResLength,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;
    uint8 iloop;
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    uint16 Did;
    boolean Find = FALSE;
    uint16 ReadDid;
    uint8 TxChannelCtrlIndex;
    uint8 TxChannelCfgIndex;
    uint32 Offset;
    const uint8* pReqData;

    boolean StoreEvent = (0u != (Dcm_MsgCtrl[MsgCtrlId].Subfunction & DCM_UDS86_SUBFUNCTION_BIT6_MASK))
                             ? DCM_UDS86_STORE_EVENT
                             : DCM_UDS86_DO_NOT_STORE_EVENT;
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    pReqData = Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData;
    TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
    Offset = (DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg)[TxChannelCfgIndex].offset;

    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    if (pReqData[5] == 0x22u)
    {
        Did = (uint16)((uint16)pReqData[3] << 8u) | ((uint16)pReqData[4]);
        ReadDid = (uint16)((uint16)pReqData[6] << 8u) | ((uint16)pReqData[7]);
        for (iloop = 0; (iloop < DCM_DSP_ROE_NUM) && (Find == FALSE); iloop++)
        {
            Dcm_RoeStatesTypes* pDcm_RoeStates = &Dcm_RoeRuntime.Dcm_RoeStates[iloop];
            if ((NULL_PTR
                 != DcmPbCfgPtr->pDcmDspCfg->Dcm_DspRoe->DcmDspRoeEvent[iloop]
                        .DcmDspRoeEventProperties->DcmDspRoeOnChangeOfDataIdentifier)
                && (Did == pDcm_RoeStates->DcmDspRoeDidRef))
            {
                pDcm_RoeStates->EventWindowTime = EventWindowTime;
                pDcm_RoeStates->Dcm_RoeEventStates = DCM_ROE_STOPPED;
                pDcm_RoeStates->Dcm_RoeStoreEvent = StoreEvent;
                Find = TRUE;
            }
        }
    }
    if ((Find == FALSE) || (Did != ReadDid))
    {
        /*service not match,can only 19 0E,send NRC 31*/
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        ret = E_NOT_OK;
    }
    if (ret == E_OK)
    {

        if (DCM_UDS86_STORE_EVENT == StoreEvent)
        {
            Dcm_UDS0x86_StoreRoe();
        }
        Dcm_Channel[Offset + 2u] = 0;
        Dcm_Channel[Offset + 3u] = EventWindowTime;
        Dcm_Channel[Offset + 4u] = pReqData[3];
        Dcm_Channel[Offset + 5u] = pReqData[4];
        Dcm_Channel[Offset + 6u] = 0x22u;
        Dcm_Channel[Offset + 7u] = pReqData[6];
        Dcm_Channel[Offset + 8u] = pReqData[7];
        *ResLength = 9;
    }
    return ret;
}
#endif

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
#if (DCM_UDS_SERVICE0X86_04_ENABLED == STD_ON)
static FUNC(Std_ReturnType, DCM_CODE) Dcm_Uds0X86SubDeal_0x04(
    uint8 ProtocolCtrlId,
    uint32 Offset,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_MsgLenType, AUTOMATIC, DCM_VAR) ResLength)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 iloop;
    uint8 Cnt = 0;
    uint32 ResOffest = Offset + 2u;
    const Dcm_DspRoeEventType* pDcmDspRoeEvent = DcmPbCfgPtr->pDcmDspCfg->Dcm_DspRoe->DcmDspRoeEvent;

    for (iloop = 0; iloop < DCM_DSP_ROE_NUM; iloop++)
    {
        const Dcm_RoeStatesTypes* pDcm_RoeStates = &Dcm_RoeRuntime.Dcm_RoeStates[iloop];
        uint16 DcmDspRoeDidRef = pDcm_RoeStates->DcmDspRoeDidRef;
        if (DCM_ROE_STARTED == pDcm_RoeStates->Dcm_RoeEventStates)
        {
            Cnt++;
            if (NULL_PTR != pDcmDspRoeEvent[iloop].DcmDspRoeEventProperties->DcmDspRoeOnChangeOfDataIdentifier)
            {
                ResOffest++;
                Dcm_Channel[ResOffest] = 3;
                ResOffest++;
                Dcm_Channel[ResOffest] = pDcm_RoeStates->EventWindowTime;
                ResOffest++;
                Dcm_Channel[ResOffest] = (uint8)(DcmDspRoeDidRef >> 8u);
                ResOffest++;
                Dcm_Channel[ResOffest] = (uint8)DcmDspRoeDidRef;
                ResOffest++;
                Dcm_Channel[ResOffest] = 0x22u;
                ResOffest++;
                Dcm_Channel[ResOffest] = (uint8)(DcmDspRoeDidRef >> 8u);
                ResOffest++;
                Dcm_Channel[ResOffest] = (uint8)DcmDspRoeDidRef;
            }
            else if (NULL_PTR != pDcmDspRoeEvent[iloop].DcmDspRoeEventProperties->DcmDspRoeOnDTCStatusChange)
            {
                ResOffest++;
                Dcm_Channel[ResOffest] = 1;
                ResOffest++;
                Dcm_Channel[ResOffest] = pDcm_RoeStates->EventWindowTime;
                ResOffest++;
                Dcm_Channel[ResOffest] = pDcm_RoeStates->DTCStatusMask;
                ResOffest++;
                Dcm_Channel[ResOffest] = 0x19u;
                ResOffest++;
                Dcm_Channel[ResOffest] = 0x0Eu;
            }
            else
            {
                /*idle*/
            }
        }
    }
    Dcm_Channel[Offset + 2u] = Cnt;
    *ResLength = (Dcm_MsgLenType)ResOffest + 1u - Offset;
    DCM_UNUSED(ProtocolCtrlId);
    return E_OK;
}
#endif

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
#if (DCM_UDS_SERVICE0X86_05_ENABLED == STD_ON)
static FUNC(Std_ReturnType, DCM_CODE) Dcm_Uds0X86SubDeal_0x05(
    uint8 ProtocolCtrlId,
    boolean StoreEvent,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_MsgLenType, AUTOMATIC, DCM_VAR) ResLength,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;
    uint8 iloop;
    uint8 Cnt = 0;
    uint8 MsgCtrlId;
    uint8 TxChannelCtrlIndex;
    uint8 TxChannelCfgIndex;
    uint32 Offset;

    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
    Offset = (DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg)[TxChannelCfgIndex].offset;
    for (iloop = 0; iloop < DCM_DSP_ROE_NUM; iloop++)
    {
        Dcm_RoeStatesTypes* pDcm_RoeStates = &Dcm_RoeRuntime.Dcm_RoeStates[iloop];
        if (DCM_ROE_STOPPED == pDcm_RoeStates->Dcm_RoeEventStates)
        {
            pDcm_RoeStates->Dcm_RoeEventStates = DCM_ROE_STARTED;
            pDcm_RoeStates->ROEActiveSes = Dcm_MkCtrl.Dcm_ActiveSes;
            Dcm_RoeRuntime.ProtocolCtrlId = ProtocolCtrlId;
#if (STD_ON == DCM_GENERIC_CONNECTION)
            pDcm_RoeStates->SA = Dcm_MsgCtrl[MsgCtrlId].Dcm_MetaData_SA;
            pDcm_RoeStates->TA = Dcm_MsgCtrl[MsgCtrlId].Dcm_MetaData_TA;
#endif
        }
        else if (DCM_ROE_CLEARED == pDcm_RoeStates->Dcm_RoeEventStates)
        {
            Cnt++;
        }
        else
        {
            /*idle*/
        }
    }
    if (Cnt == DCM_DSP_ROE_NUM)
    {
        /*all RoeEvents are in ROE cleared state ,send NRC 24*/
        *ErrorCode = DCM_E_REQUESTSEQUENCEERROR;
        ret = E_NOT_OK;
    }
    else
    {
        if (DCM_UDS86_STORE_EVENT == StoreEvent)
        {
            Dcm_UDS0x86_StoreRoe();
        }
        Dcm_Channel[Offset + 2u] = 0;
        Dcm_Channel[Offset + 3u] = Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[2];
        *ResLength = 4;
    }
    return ret;
}
#endif

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
#if (DCM_UDS_SERVICE0X86_06_ENABLED == STD_ON)
static FUNC(Std_ReturnType, DCM_CODE) Dcm_Uds0X86SubDeal_0x06(
    uint8 ProtocolCtrlId,
    uint32 Offset,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_MsgLenType, AUTOMATIC, DCM_VAR) ResLength)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 iloop;
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    boolean StoreEvent = (0u != (Dcm_MsgCtrl[MsgCtrlId].Subfunction & DCM_UDS86_SUBFUNCTION_BIT6_MASK))
                             ? DCM_UDS86_STORE_EVENT
                             : DCM_UDS86_DO_NOT_STORE_EVENT;
    for (iloop = 0; iloop < DCM_DSP_ROE_NUM; iloop++)
    {
        Dcm_RoeStatesTypes* pDcm_RoeStates = &Dcm_RoeRuntime.Dcm_RoeStates[iloop];
        Dcm_Uds0X86_CheckEventWindowTime(iloop, TRUE);
        pDcm_RoeStates->Dcm_RoeEventStates = DCM_ROE_CLEARED;
        pDcm_RoeStates->Dcm_RoeStoreEvent = DCM_UDS86_DO_NOT_STORE_EVENT;
        pDcm_RoeStates->ROEActiveSes = DCM_ALL_SESSION_LEVEL;
        pDcm_RoeStates->Dcm_RoeStoreEvent = StoreEvent;
        pDcm_RoeStates->RoeCnt = 0;
    }
    if (DCM_UDS86_STORE_EVENT == StoreEvent)
    {
        Dcm_UDS0x86_StoreRoe();
    }
    Dcm_Channel[Offset + 2u] = 0;
    Dcm_Channel[Offset + 3u] = Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[2];
    *ResLength = 4;
    return E_OK;
}
#endif

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
static FUNC(boolean, DCM_CODE) Dcm_UDS0x86_CheckFullCom(void)
{
    uint8 Index;
    uint8 iloop;
    boolean ret = FALSE;
    const Dcm_DslConnectionType* pDsl_Protocol_ConnectionCfg = DcmPbCfgPtr->DslProtocolConnectionCfg;
    for (Index = 0; Index < DCM_CONNECTION_NUM; Index++)
    {
        if (pDsl_Protocol_ConnectionCfg[Index].DcmDslParentProtocolRowCtrlId == Dcm_RoeRuntime.ProtocolCtrlId)
        {
            for (iloop = 0; iloop < DCM_MAINCONNECTION_NUM; iloop++)
            {
                if ((pDsl_Protocol_ConnectionCfg[Index].pDcmDslMainConnection != NULL_PTR)
                    && (pDsl_Protocol_ConnectionCfg[Index].pDcmDslMainConnection->DcmDslProtocolComMChannelId
                        == Dcm_CommCtrl[iloop].DcmDslProtocolComMChannelId))
                {
                    if (Dcm_CommCtrl[iloop].Dcm_CommState == DCM_COMM_FULL_COMMUNICATION)
                    {
                        ret = TRUE;
                        break;
                    }
                }
            }
        }
    }
    return ret;
}

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x86_ChangeOfDataIdentifier(uint8 RoeEventId)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    uint8 iloop;
    uint8 MsgCtrlId;
    boolean Find = FALSE;
    PduIdType pduRxId = 0;
    uint8 Data[3] = {0};
    uint8 index;
    uint8 ROEIndex;
    boolean ProcressOn = FALSE;
    Std_ReturnType ret = E_OK;
    PduInfoType PduBuffer;
    PduInfoType* PduInfo = &PduBuffer;
    const Dcm_DspRoeTypes* Dcm_DspRoe = DcmPbCfgPtr->pDcmDspCfg->Dcm_DspRoe;
    const Dcm_DslConnectionType* pDsl_Protocol_ConnectionCfg = DcmPbCfgPtr->DslProtocolConnectionCfg;
    Dcm_RoeStatesTypes* pDcm_RoeStates;

    for (ROEIndex = 0; ROEIndex < DCM_DSP_ROE_NUM; ROEIndex++)
    {
        if (RoeEventId == Dcm_DspRoe->DcmDspRoeEvent[ROEIndex].DcmDspRoeEventId)
        {
            ProcressOn = TRUE;
            break;
        }
    }
    if (ProcressOn == TRUE)
    {
        ProcressOn = Dcm_UDS0x86_CheckFullCom();
    }
    else
    {
        ret = E_NOT_OK;
    }
    pDcm_RoeStates = &Dcm_RoeRuntime.Dcm_RoeStates[ROEIndex];
    if ((ProcressOn == TRUE) && (DCM_ROE_STARTED == pDcm_RoeStates->Dcm_RoeEventStates)
        && (NULL_PTR
            != Dcm_DspRoe->DcmDspRoeEvent[ROEIndex].DcmDspRoeEventProperties->DcmDspRoeOnChangeOfDataIdentifier))
    {
        /*check all protocol whether have one protocol had already started*/
        for (iloop = 0; iloop < DCM_DSLPROTOCOLROW_NUM_MAX; iloop++)
        {
            MsgCtrlId = Dcm_ProtocolCtrl[iloop].MsgCtrlIndex;
            if ((MsgCtrlId != 0xFFu) && (DCM_MSG_WAIT != Dcm_MsgCtrl[MsgCtrlId].Dcm_MsgState))
            {
                Find = TRUE; /*have finded a starting protocol*/
                break;
            }
        }
        Data[0] = 0x22;
        Data[1] = (uint8)(pDcm_RoeStates->DcmDspRoeDidRef >> 8u);
        Data[2] = (uint8)pDcm_RoeStates->DcmDspRoeDidRef;

        for (iloop = 0; iloop < DCM_CONNECTION_NUM; iloop++)
        {
            if (Dcm_RoeRuntime.ProtocolCtrlId == pDsl_Protocol_ConnectionCfg[iloop].DcmDslParentProtocolRowCtrlId)
            {
                for (index = 0; index < pDsl_Protocol_ConnectionCfg[iloop].pDcmDslMainConnection->DcmDslProtocolRx_Num;
                     index++)
                {
                    if (DCM_FUNCTIONAL
                        != pDsl_Protocol_ConnectionCfg[iloop]
                               .pDcmDslMainConnection->pDcmDslProtocolRx->DcmDslProtocolRxAddrType)
                    {
                        pduRxId = pDsl_Protocol_ConnectionCfg[iloop]
                                      .pDcmDslMainConnection->pDcmDslProtocolRx->DcmDslProtocolRxPduId;
                        break;
                    }
                }
                break;
            }
        }
        Dcm_DspRoeNeedSend.RoeId = ROEIndex;
        if ((Dcm_DspRoeInterMessageTime == 0u) && (Find == FALSE))
        {
            Dcm_DspRoeNeedSend.IsRoeMessage = TRUE;
            pDcm_RoeStates->RoeCnt++;
#if (STD_ON == DCM_GENERIC_CONNECTION)
            if ((pDsl_Protocol_ConnectionCfg[iloop].pDcmDslMainConnection->pDcmDslProtocolRx->DcmDslMetaDataFlag
                 & DCM_METADATA_LENGTH_MASK)
                > 0u)
            {
                PduInfo->MetaDataPtr = &(Dcm_Metadata[Dcm_RoeRuntime.ProtocolCtrlId][0]);
#if (DCM_CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
                PduInfo->MetaDataPtr[0u] = (uint8)((pDcm_RoeStates->SA & 0xFF00u) >> 8u);
                PduInfo->MetaDataPtr[1u] = (uint8)(pDcm_RoeStates->SA & 0xFFu);
                PduInfo->MetaDataPtr[2u] = (uint8)((pDcm_RoeStates->TA & 0xFF00u) >> 8u);
                PduInfo->MetaDataPtr[3u] = (uint8)(pDcm_RoeStates->TA & 0xFFu);
#else
                PduInfo->MetaDataPtr[0u] = (uint8)(pDcm_RoeStates->SA & 0xFFu);
                PduInfo->MetaDataPtr[1u] = (uint8)((pDcm_RoeStates->SA & 0xFF00u) >> 8u);
                PduInfo->MetaDataPtr[2u] = (uint8)(pDcm_RoeStates->TA & 0xFFu);
                PduInfo->MetaDataPtr[3u] = (uint8)((pDcm_RoeStates->TA & 0xFF00u) >> 8u);
#endif
            }
#else
            PduInfo->MetaDataPtr = NULL_PTR;
#endif
            PduInfo->SduDataPtr = Data;
            PduInfo->SduLength = 3;
            (void)DslInternal_ResponseOnOneEvent(PduInfo, pduRxId);
        }
        else
        {
            Dcm_MemoryCopy(Data, Dcm_DspRoeNeedSend.Data, 3);
            Dcm_DspRoeNeedSend.Length = 3;
            Dcm_DspRoeNeedSend.Dcm_DspRoeNeedSend = TRUE;
            Dcm_DspRoeNeedSend.pduRxId = pduRxId;
        }
    }
    return ret;
}

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
FUNC(void, DCM_CODE)
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
Dcm_UDS0x86_DTCStatusChange(uint8 DTCStatusOld, uint8 DTCStatusNew)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    uint8 iloop;
    uint8 Cnt;
    uint8 index;
    uint8 MsgCtrlId;
    boolean Find = FALSE;
    boolean ProcressOn;
    PduIdType pduRxId = 0;
    uint8 Data[2] = {0};
    PduInfoType PduBuffer;
    PduInfoType* PduInfo = &PduBuffer;
    const Dcm_DspRoeTypes* Dcm_DspRoe = DcmPbCfgPtr->pDcmDspCfg->Dcm_DspRoe;
    const Dcm_DslConnectionType* pDsl_Protocol_ConnectionCfg = DcmPbCfgPtr->DslProtocolConnectionCfg;

    ProcressOn = Dcm_UDS0x86_CheckFullCom();
    for (Cnt = 0; (Cnt < DCM_DSP_ROE_NUM) && (ProcressOn == TRUE); Cnt++)
    {
        Dcm_RoeStatesTypes* pDcm_RoeStates = &Dcm_RoeRuntime.Dcm_RoeStates[Cnt];
        if ((DCM_ROE_STARTED == pDcm_RoeStates->Dcm_RoeEventStates)
            && (NULL_PTR != Dcm_DspRoe->DcmDspRoeEvent[Cnt].DcmDspRoeEventProperties->DcmDspRoeOnDTCStatusChange)
            && ((DTCStatusOld & pDcm_RoeStates->DTCStatusMask) != (DTCStatusNew & pDcm_RoeStates->DTCStatusMask))
            && (DTCStatusNew != 0x50u)) /*clear the EventMemory shall ignore*/
        {
            /*check all protocol whether have one protocol had already started*/
            for (iloop = 0; iloop < DCM_DSLPROTOCOLROW_NUM_MAX; iloop++)
            {
                MsgCtrlId = Dcm_ProtocolCtrl[iloop].MsgCtrlIndex;
                if ((MsgCtrlId != 0xFFu) && (DCM_MSG_WAIT != Dcm_MsgCtrl[MsgCtrlId].Dcm_MsgState))
                {
                    Find = TRUE; /*have finded a starting protocol*/
                    break;
                }
            }
            Data[0] = 0x19;
            Data[1] = 0x0E;

            for (iloop = 0; iloop < DCM_CONNECTION_NUM; iloop++)
            {
                if (Dcm_RoeRuntime.ProtocolCtrlId == pDsl_Protocol_ConnectionCfg[iloop].DcmDslParentProtocolRowCtrlId)
                {
                    const Dcm_DslMainConnectionType* pDcmDslMainConnection =
                        pDsl_Protocol_ConnectionCfg[iloop].pDcmDslMainConnection;
                    for (index = 0; index < pDcmDslMainConnection->DcmDslProtocolRx_Num; index++)
                    {
                        if (DCM_FUNCTIONAL != pDcmDslMainConnection->pDcmDslProtocolRx[index].DcmDslProtocolRxAddrType)
                        {
                            pduRxId = pDcmDslMainConnection->pDcmDslProtocolRx[index].DcmDslProtocolRxPduId;
                            break;
                        }
                    }
                    break;
                }
            }
            Dcm_DspRoeNeedSend.RoeId = Cnt;
            if ((Dcm_DspRoeInterMessageTime == 0u) && (Find == FALSE))
            {
                Dcm_DspRoeNeedSend.IsRoeMessage = TRUE;
                pDcm_RoeStates->RoeCnt++;
#if (STD_ON == DCM_GENERIC_CONNECTION)
                if ((pDsl_Protocol_ConnectionCfg[iloop].pDcmDslMainConnection->pDcmDslProtocolRx->DcmDslMetaDataFlag
                     & DCM_METADATA_LENGTH_MASK)
                    > 0u)
                {
                    PduInfo->MetaDataPtr = &(Dcm_Metadata[Dcm_RoeRuntime.ProtocolCtrlId][0]);
#if (DCM_CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
                    PduInfo->MetaDataPtr[0u] = (uint8)((pDcm_RoeStates->SA & 0xFF00u) >> 8u);
                    PduInfo->MetaDataPtr[1u] = (uint8)(pDcm_RoeStates->SA & 0xFFu);
                    PduInfo->MetaDataPtr[2u] = (uint8)((pDcm_RoeStates->TA & 0xFF00u) >> 8u);
                    PduInfo->MetaDataPtr[3u] = (uint8)(pDcm_RoeStates->TA & 0xFFu);
#else
                    PduInfo->MetaDataPtr[0u] = (uint8)(pDcm_RoeStates->SA & 0xFFu);
                    PduInfo->MetaDataPtr[1u] = (uint8)((pDcm_RoeStates->SA & 0xFF00u) >> 8u);
                    PduInfo->MetaDataPtr[2u] = (uint8)(pDcm_RoeStates->TA & 0xFFu);
                    PduInfo->MetaDataPtr[3u] = (uint8)((pDcm_RoeStates->TA & 0xFF00u) >> 8u);
#endif
                }
#else
                PduInfo->MetaDataPtr = NULL_PTR;
#endif
                PduInfo->SduDataPtr = Data;
                PduInfo->SduLength = 2;
                (void)DslInternal_ResponseOnOneEvent(PduInfo, pduRxId);
            }
            else
            {
                Dcm_MemoryCopy(Data, Dcm_DspRoeNeedSend.Data, 2);
                Dcm_DspRoeNeedSend.Length = 2;
                Dcm_DspRoeNeedSend.Dcm_DspRoeNeedSend = TRUE;
                Dcm_DspRoeNeedSend.pduRxId = pduRxId;
            }
            break; /*one DTC change can only send one RoeMessage*/
        }
    }
}

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
static FUNC(Std_ReturnType, DCM_CODE) Dcm_Uds0X86ServiceConditionCheck(
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 MsgCtrlId;
    Std_ReturnType ret;
    uint8 SubFunction;
    boolean StoreEvent;
    uint8 iloop;
    uint8 Cnt = 0;
    uint8 EventWindowTime;

    /*if the required protocol is configured,get the index of runtime datum*/
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    SubFunction = Dcm_MsgCtrl[MsgCtrlId].Subfunction & DCM_UDS86_SUBFUNCTION_MASK;
    StoreEvent = (0u != (Dcm_MsgCtrl[MsgCtrlId].Subfunction & DCM_UDS86_SUBFUNCTION_BIT6_MASK))
                     ? DCM_UDS86_STORE_EVENT
                     : DCM_UDS86_DO_NOT_STORE_EVENT;
    Dcm_MsgCtrl[MsgCtrlId].Subfunction = Dcm_MsgCtrl[MsgCtrlId].Subfunction & (~DCM_UDS86_SUBFUNCTION_BIT6_MASK);

    /*AUTOSAR is not support 0x02 and 0x07*/
    ret = Dcm_UdsSubServicesCheck(ProtocolCtrlId);
    if (E_NOT_OK == ret)
    {
        /*the required sub-function is not supported,send NRC 0x12*/
        *ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTED;
    }
#if (STD_ON == DCM_UDS_SERVICE0X29_ENABLED)
    if (E_OK == ret)
    {
        ret = DspInternal_SubServiceAuthenticationCheck(ProtocolCtrlId, ErrorCode);
    }
#endif
#if (STD_ON == DCM_SESSION_FUNC_ENABLED)
    if (E_OK == ret)
    {
        ret = DsdInternal_SubSesCheck(ProtocolCtrlId, SID_RESPONSE_ON_EVENT, ErrorCode);
    }
#endif /* STD_ON == DCM_SESSION_FUNC_ENABLED */

#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
    if (E_OK == ret)
    {
        /*security check,check whether the current security supports the request service*/
        ret = DsdInternal_SubSecurityCheck(ProtocolCtrlId, SID_RESPONSE_ON_EVENT, ErrorCode);
    }
#endif /* STD_ON == DCM_SECURITY_FUNC_ENABLED */

    if (E_OK == ret)
    {
        if (((3u != Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
             && ((SubFunction == DCM_UDS0X86_STOP_RESPONSE_ON_EVENT)
                 || (SubFunction == DCM_UDS0X86_REPORT_ACTIVATED_EVENTS)
                 || (SubFunction == DCM_UDS0X86_START_RESPONSE_ON_EVENT)
                 || (SubFunction == DCM_UDS0X86_CLEAR_RESPONSE_ON_EVENT)))
            || ((6u != Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
                && (SubFunction == DCM_UDS0X86_ON_DTC_STATUS_CHANGE))
            || ((8u != Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
                && (SubFunction == DCM_UDS0X86_ON_CHANGE_OF_DATA_IDENTIFIER)))
        {
            /*the length of massage is not correct,send NRC 0x13*/
            *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
            ret = E_NOT_OK;
        }
    }

    if ((E_OK == ret)
        && ((SubFunction == DCM_UDS0X86_ON_DTC_STATUS_CHANGE)
            || (SubFunction == DCM_UDS0X86_ON_CHANGE_OF_DATA_IDENTIFIER)))
    {
        EventWindowTime = Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[2];
        ret = Dcm_Uds0X86EventWindowTimeCheck(EventWindowTime, StoreEvent, ErrorCode);
    }

    if ((E_OK == ret) && (SubFunction != DCM_UDS0X86_STOP_RESPONSE_ON_EVENT)
        && (SubFunction != DCM_UDS0X86_CLEAR_RESPONSE_ON_EVENT))
    {
        for (iloop = 0; iloop < DCM_DSP_ROE_NUM; iloop++)
        {
            if (DCM_ROE_STARTED == Dcm_RoeRuntime.Dcm_RoeStates[iloop].Dcm_RoeEventStates)
            {
                Cnt++;
            }
        }
        if ((Cnt > 0u) && (ProtocolCtrlId != Dcm_RoeRuntime.ProtocolCtrlId))
        {
            /*If at least one RoeEvent is in ROE started state the DCM shall
             * reject all ROE request received on a different DcmDslProtocol than
             * the protocol where the RoeEvents were started with an NRC 0x22*/
            *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
            ret = E_NOT_OK;
        }
    }
    return ret;
}

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
/* PRQA S 3432,1532++ */ /* MISRA Rule 20.7,8.7 */
FUNC(Std_ReturnType, DCM_CODE)
Dcm_UDS0x86(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432,1532-- */ /* MISRA Rule 20.7,8.7 */
{
    uint8 MsgCtrlId;
    uint8 TxChannelCtrlIndex;
    uint8 TxChannelCfgIndex;
    uint32 Offset;
    Std_ReturnType ret;
    uint8 SubFunction;
    boolean StoreEvent;
    uint8 EventWindowTime;
    Dcm_MsgLenType ResLength = 0;

    DCM_UNUSED(OpStatus);
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    SubFunction = Dcm_MsgCtrl[MsgCtrlId].Subfunction & DCM_UDS86_SUBFUNCTION_MASK;
    StoreEvent = (0u != (Dcm_MsgCtrl[MsgCtrlId].Subfunction & DCM_UDS86_SUBFUNCTION_BIT6_MASK))
                     ? DCM_UDS86_STORE_EVENT
                     : DCM_UDS86_DO_NOT_STORE_EVENT;

    ret = Dcm_Uds0X86ServiceConditionCheck(ProtocolCtrlId, ErrorCode);

    if (E_OK == ret)
    {
        TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
        TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
        Offset = (DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg)[TxChannelCfgIndex].offset;
        EventWindowTime = Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[2];

        switch (SubFunction)
        {
#if (DCM_UDS_SERVICE0X86_00_ENABLED == STD_ON)
        case DCM_UDS0X86_STOP_RESPONSE_ON_EVENT:
            ret = Dcm_Uds0X86SubDeal_0x00(ProtocolCtrlId, &ResLength, ErrorCode);
            break;
#endif
#if (DCM_UDS_SERVICE0X86_01_ENABLED == STD_ON)
        case DCM_UDS0X86_ON_DTC_STATUS_CHANGE:
            ret = Dcm_Uds0X86SubDeal_0x01(ProtocolCtrlId, EventWindowTime, &ResLength, ErrorCode);
            break;
#endif
#if (DCM_UDS_SERVICE0X86_03_ENABLED == STD_ON)
        case DCM_UDS0X86_ON_CHANGE_OF_DATA_IDENTIFIER:
            ret = Dcm_Uds0X86SubDeal_0x03(ProtocolCtrlId, EventWindowTime, &ResLength, ErrorCode);
            break;
#endif
#if (DCM_UDS_SERVICE0X86_04_ENABLED == STD_ON)
        case DCM_UDS0X86_REPORT_ACTIVATED_EVENTS:
            ret = Dcm_Uds0X86SubDeal_0x04(ProtocolCtrlId, Offset, &ResLength);
            break;
#endif
#if (DCM_UDS_SERVICE0X86_05_ENABLED == STD_ON)
        case DCM_UDS0X86_START_RESPONSE_ON_EVENT:
            ret = Dcm_Uds0X86SubDeal_0x05(ProtocolCtrlId, StoreEvent, &ResLength, ErrorCode);
            break;
#endif
#if (DCM_UDS_SERVICE0X86_06_ENABLED == STD_ON)
        case DCM_UDS0X86_CLEAR_RESPONSE_ON_EVENT:
            ret = Dcm_Uds0X86SubDeal_0x06(ProtocolCtrlId, Offset, &ResLength);
            break;
#endif
        default:
            /*the required sub-function is not supported,send NRC 0x12*/
            *ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTED;
            ret = E_NOT_OK;
            break;
        }
    }
    if (E_OK == ret)
    {
        /* check tx data length */
        if (ResLength > (DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex].Dcm_DslBufferSize))
        { /*Pdu length is bigger than buffer size,ignore the request message */
            *ErrorCode = DCM_E_RESPONSETOOLONG;
            ret = E_NOT_OK;
        }
        else
        {
            /*assemble positive response*/
            Dcm_Channel[Offset] = 0xC6; /*response SID*/
            if (DCM_UDS86_STORE_EVENT == StoreEvent)
            {
                Dcm_Channel[Offset + 1u] = Dcm_MsgCtrl[MsgCtrlId].Subfunction | DCM_UDS86_SUBFUNCTION_BIT6_MASK;
            }
            else
            {
                Dcm_Channel[Offset + 1u] = Dcm_MsgCtrl[MsgCtrlId].Subfunction;
            }
            Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResMaxDataLen = ResLength;
            Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResDataLen = ResLength;
            Dcm_MsgCtrl[MsgCtrlId].MsgContext.pResData = &Dcm_Channel[Offset];
            DsdInternal_ProcessingDone(ProtocolCtrlId);
        }
    }
    return ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
