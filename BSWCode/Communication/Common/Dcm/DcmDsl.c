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

/*=======[I N C L U D E S]====================================================*/
#include "Dcm_Internal.h"

/*******************************************************
 *********************Resource define*******************
 ********************************************************/
#define DCM_START_SEC_VAR_POWER_ON_INIT_8
#include "Dcm_MemMap.h"
static VAR(boolean, DCM_VAR_POWER_ON_INIT) StopProtocolFaild = FALSE;
/*StopProtocolFaild Then Rxindication is come shall send NRC*/
#define DCM_STOP_SEC_VAR_POWER_ON_INIT_8
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/*************************************************************************/
/*
 * Brief               <Dcm_CheckProtocol>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(Std_ReturnType, DCM_CODE) Dcm_CheckProtocol(uint8 ProtocolCfgId, uint8 ServiceType);

#if (STD_ON == DCM_GENERIC_CONNECTION)
/*************************************************************************/
/*
 * Brief               <Dcm_StartOfReception_metadata>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(BufReq_ReturnType, DCM_CODE)
    Dcm_StartOfReception_metadata(PduIdType id, P2CONST(PduInfoType, AUTOMATIC, DCM_CONST) info);
#endif /* STD_ON == DCM_GENERIC_CONNECTION */

/*************************************************************************/
/*
 * Brief               <Dcm_CopyTxData_CheckCtrlStatus>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static inline FUNC(Std_ReturnType, DCM_CODE) Dcm_CopyTxData_CheckCtrlStatus(PduIdType DcmTxPduId);

/*************************************************************************/
/*
 * Brief               <Dcm_TpRxIndication_IsFuncAddress>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(Std_ReturnType, DCM_CODE) Dcm_TpRxIndication_IsFuncAddress(PduIdType DcmRxPduId);

/*************************************************************************/
/*
 * Brief               <Dcm_TpRxIndication_SubDeal>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/

#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
/*************************************************************************/
/*
 * Brief               <Dcm_CopyTxData_PagebufferDeal>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(BufReq_ReturnType, DCM_CODE) Dcm_CopyTxData_PagebufferDeal(
    uint8 ProtocolCfgId,
    P2CONST(PduInfoType, AUTOMATIC, DCM_CONST) info,
    uint8 TxChannelCtrlIndex,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(PduLengthType, AUTOMATIC, DCM_VAR) availableDataPtr);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /* STD_ON == DCM_PAGEDBUFFER_ENABLED */

/*************************************************************************/
/*
 * Brief               <Dcm_CancelPendingService>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(void, DCM_CODE) Dcm_CancelPendingService(uint8 ProtocolCfgId);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

/****************************** implementations ********************************/
/*************************************************************************/
/*
 * Brief               <ComM module notice DCM modules,
 * network communication mode is DCM_COMM_NO_COMMUNICATION>
 * ServiceId           <0x21>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#define DCM_START_SEC_DCMCOMMNOCOMMODEENTERED_CALLBACK_CODE
#include "Dcm_MemMap.h"
FUNC(void, DCM_CODE) Dcm_ComM_NoComModeEntered(uint8 NetworkId)
{
    uint8 index;
    uint8 MainConnectionNum = Dcm_GetMainConnectionNum();

#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if (DCM_ON != Dcm_MkCtrl.Dcm_MkState)
    {
        DCM_DET_REPORTERROR(DCM_COMM_NOCOMMODEENTERED, DCM_E_UNINIT);
    }
    else
#endif
    {
        SchM_Enter_Dcm_ExclusiveArea();
        for (index = 0; index < MainConnectionNum; index++)
        {
            if (Dcm_CommCtrl[index].DcmDslProtocolComMChannelId == NetworkId)
            {
                Dcm_CommCtrl[index].Dcm_CommState = DCM_COMM_NO_COMMUNICATION;
            }
        }
        SchM_Exit_Dcm_ExclusiveArea();
    }
}
#define DCM_STOP_SEC_DCMCOMMNOCOMMODEENTERED_CALLBACK_CODE
#include "Dcm_MemMap.h"

/*************************************************************************/
/*
 * Brief               <ComM module notice DCM modules, network
 * communication mode is DCM_COMM_SILENT_COMMUNICATION>
 * ServiceId           <0x22>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#define DCM_START_SEC_DCMCOMMSILENTCOMMODEENTERED_CALLBACK_CODE
#include "Dcm_MemMap.h"
FUNC(void, DCM_CODE) Dcm_ComM_SilentComModeEntered(uint8 NetworkId)
{
    uint8 index;
    uint8 MainConnectionNum = Dcm_GetMainConnectionNum();

#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if (DCM_ON != Dcm_MkCtrl.Dcm_MkState)
    {
        DCM_DET_REPORTERROR(DCM_COMM_SILENTCOMMODEENTERED, DCM_E_UNINIT);
    }
    else
#endif
    {
        SchM_Enter_Dcm_ExclusiveArea();
        for (index = 0; index < MainConnectionNum; index++)
        {
            if (Dcm_CommCtrl[index].DcmDslProtocolComMChannelId == NetworkId)
            {
                Dcm_CommCtrl[index].Dcm_CommState = DCM_COMM_SILENT_COMMUNICATION;
            }
        }
        SchM_Exit_Dcm_ExclusiveArea();
    }
}
#define DCM_STOP_SEC_DCMCOMMSILENTCOMMODEENTERED_CALLBACK_CODE
#include "Dcm_MemMap.h"

/*************************************************************************/
/*
 * Brief               <ComM module notice DCM modules,
 * network communication mode is DCM_COMM_FULL_COMMUNICATION>
 * ServiceId           <0x23>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <>
 */
/*************************************************************************/
#define DCM_START_SEC_DCMCOMMFULLCOMMODEENTERED_CALLBACK_CODE
#include "Dcm_MemMap.h"
FUNC(void, DCM_CODE) Dcm_ComM_FullComModeEntered(uint8 NetworkId)
{
    uint8 index;
    uint8 MainConnectionNum = Dcm_GetMainConnectionNum();

#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if (DCM_ON != Dcm_MkCtrl.Dcm_MkState)
    {
        DCM_DET_REPORTERROR(DCM_COMM_FULLCOMMODEENTERED, DCM_E_UNINIT);
    }
    else
#endif
    {
        SchM_Enter_Dcm_ExclusiveArea();
        for (index = 0; index < MainConnectionNum; index++)
        {
            if (Dcm_CommCtrl[index].DcmDslProtocolComMChannelId == NetworkId)
            {
                Dcm_CommCtrl[index].Dcm_CommState = DCM_COMM_FULL_COMMUNICATION;
            }
        }
        SchM_Exit_Dcm_ExclusiveArea();
    }
}
#define DCM_STOP_SEC_DCMCOMMFULLCOMMODEENTERED_CALLBACK_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/*************************************************************************/
/*
 * Brief               <P2Timer timeout processing>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:Protocol control block index number.>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
FUNC(void, DCM_CODE) DslInternal_P2ServerTimeout(uint8 ProtocolCtrlId) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    const Dcm_DslCfgType* pDcmDslCfg = DcmPbCfgPtr->pDcmDslCfg;
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    uint8 PendingNumx = Dcm_MsgCtrl[MsgCtrlId].Dcm_P2Ctrl.PendingNum;
    uint8 PendingMaxTimes = pDcmDslCfg->pDcmDslDiagRespCfg->DcmDslDiagRespMaxNumRespPend;

    if ((PendingNumx < PendingMaxTimes) || (PendingMaxTimes == 0u))
    {
        /* Send Nrc=0x78 */
        DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_RESPONSE_PENDING);
        DsdInternal_ProcessingDone(ProtocolCtrlId);
    }
    else
    {
        /* Send Nrc=0x10 */
        Dcm_CancelPendingService(ProtocolCtrlId);
        DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_GENERALREJECT);
        DsdInternal_ProcessingDone(ProtocolCtrlId);
    }
}

/*************************************************************************/
/*
 * Brief               <protocal start>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:Protocol control block index number.>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) DslInternal_ProtocolStart(uint8 ProtocolCtrlId) /* PRQA S 1505 */ /* MISRA Rule 8.7 */
{
    boolean Flag;
    uint8 Index;
    uint8 NotOkNum = 0u;
    uint8 NotAllowedNum = 0u;
    Dcm_ProtocolType ProtocolIdx;
    Std_ReturnType ret = E_OK;
    const Dcm_DslCfgType* pDcmDslCfg = DcmPbCfgPtr->pDcmDslCfg;
    uint8 Request_PortNum = pDcmDslCfg->DCMCallBackDcmRequest_PortNum;
    const Dcm_DslCallbackDCMRequestServiceType* pDcmDslCallback_DCMRequestService =
        pDcmDslCfg->pDcmDslCallback_DCMRequestService;

    if (DCM_PROTOCOL_INVALID == Dcm_ProtocolCtrl[ProtocolCtrlId].Dcm_ProtocolState)
    {
        ProtocolIdx = pDcmDslCfg->pDcmDslProtocol->pDcmDslProtocolRow[ProtocolCtrlId].DcmDslProtocolID;
        Flag = FALSE;
        for (Index = 0; (Index < Request_PortNum) && (FALSE == Flag); Index++)
        {
            if (NULL_PTR == pDcmDslCallback_DCMRequestService[Index].StartProtocol)
            {
                Flag = TRUE;
            }
            else
            {
                /*Notify the application, Start Protocol*/
                ret = (*(pDcmDslCallback_DCMRequestService[Index].StartProtocol))(ProtocolIdx);
                switch (ret)
                {
                case E_OK:
                    break;
                case E_PROTOCOL_NOT_ALLOWED:
                    NotAllowedNum++;
                    break;
                case E_NOT_OK:
                    NotOkNum++;
                    break;
                default:
                    NotOkNum++;
                    break;
                }
            }
        }
        if ((TRUE == Flag) || (0u != NotOkNum))
        {
            ret = E_NOT_OK;
        }
        else if (0u != NotAllowedNum)
        {
            /*Application does not allow Start Protocol.*/
            ret = E_PROTOCOL_NOT_ALLOWED;
        }
        else
        {
            /*Save the valid protocol ID number*/
            SchM_Enter_Dcm_ExclusiveArea();
            Dcm_MkCtrl.Dcm_ActiveProtocolCfgCtrlId = ProtocolCtrlId;
            Dcm_ProtocolCtrl[ProtocolCtrlId].Dcm_ProtocolState = DCM_PROTOCOL_VALID;
            SchM_Exit_Dcm_ExclusiveArea();
            DslInternal_ProtocolStartSession();
            ret = E_OK;
        }
    }
    else
    {
        /*protocol is already started*/
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <protocal stop>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:Protocol control block index number.>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) DslInternal_ProtocolStop(uint8 ProtocolCtrlId)
{
    boolean Flag;
    uint8 Index;
    uint8 NotOkNum = 0u;
    uint8 NotAllowedNum = 0u;
    Dcm_ProtocolType ProtocolIdx;
    Std_ReturnType ret = E_OK;
    const Dcm_DslCfgType* pDcmDslCfg = DcmPbCfgPtr->pDcmDslCfg;
    uint8 Request_PortNum = pDcmDslCfg->DCMCallBackDcmRequest_PortNum;
    const Dcm_DslCallbackDCMRequestServiceType* pDcmDslCallback_DCMRequestService =
        pDcmDslCfg->pDcmDslCallback_DCMRequestService;

    if (DCM_PROTOCOL_VALID == Dcm_ProtocolCtrl[ProtocolCtrlId].Dcm_ProtocolState)
    {
        ProtocolIdx = pDcmDslCfg->pDcmDslProtocol->pDcmDslProtocolRow[ProtocolCtrlId].DcmDslProtocolID;
        Flag = FALSE;
        for (Index = 0; (Index < Request_PortNum) && (FALSE == Flag); Index++)
        {
            if (NULL_PTR == pDcmDslCallback_DCMRequestService[Index].StopProtocol)
            {
                Flag = TRUE;
            }
            else
            {
                /*Notify the application, Stop Protocol*/
                ret = (*(pDcmDslCallback_DCMRequestService[Index].StopProtocol))(ProtocolIdx);
                switch (ret)
                {
                case E_OK:
                    break;
                case E_PROTOCOL_NOT_ALLOWED:
                    NotAllowedNum++;
                    break;
                case E_NOT_OK:
                    NotOkNum++;
                    break;
                default:
                    NotOkNum++;
                    break;
                }
            }
        }
        if ((TRUE == Flag) || (0u != NotOkNum))
        {
            ret = E_NOT_OK;
        }
        else if (0u != NotAllowedNum)
        {
            /*Application does not allow Stop Protocol.*/
            ret = E_PROTOCOL_NOT_ALLOWED;
        }
        else
        {
            Dcm_ProtocolCtrl[ProtocolCtrlId].Dcm_ProtocolState = DCM_PROTOCOL_INVALID;
            ret = E_OK;
        }
    }
    /* else protocol is not started*/
    return ret;
}
/*************************************************************************/
/*
 * Brief               <Reset protocol control block members MsgCtrlIndex +
 *                      Reset corresponding message control block +
 *                      Reset corresponding receive / transmit channel control block>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:protocol control block ID>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <>
 * CallByAPI           <>
 */
/*************************************************************************/
FUNC(void, DCM_CODE) DslInternal_ResetResource(uint8 ProtocolCtrlId)
{
    PduIdType TxPduId;
    uint8 ConnectionCfgCtrlId = 0xFFu;
    NetworkHandleType NetworkChannel;
    uint8 MsgCtrlIndexx;
    uint8 RxChannelCtrlIndex;
    uint8 TxChannelCtrlIndex;
    uint8 Index;
    boolean Flag;
    Dcm_ProtocolType ProtocolId;
    uint8 ServiceType;
    uint8 ProtocolMsgNum = Dcm_GetProtocolMsgNum();

    MsgCtrlIndexx = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    NetworkChannel = Dcm_MkCtrl.Dcm_ActiveNetwork;
    TxPduId = Dcm_MsgCtrl[MsgCtrlIndexx].DcmTxPduId;
    for (Index = 0; Index < DCM_DSL_TX_ID_NUM; Index++)
    {
        if (TxPduId == DcmPbCfgPtr->DslProtocolConnectionTxCfg[Index].DcmDslTxPduRPduId)
        {
            ConnectionCfgCtrlId = DcmPbCfgPtr->DslProtocolConnectionTxCfg[Index].DcmDslParentConnectionCtrlId;
            DslInternal_InitConnectionCtrl(ConnectionCfgCtrlId);
        }
    }
    /**********************************************************/
#if (STD_ON == DCM_UDS_SERVICE0X29_ENABLED)
    uint8 connectionId = Dcm_GetconnectionIndex(ConnectionCfgCtrlId);
    DspInternal_SetAuthenticationCtrlOn(connectionId);
#endif
    /*ComM Inactive processing*/
    if ((DCM_DEFAULT_SESSION == Dcm_MkCtrl.Dcm_ActiveSes) && (DCM_INVALID_UINT8 != NetworkChannel))
    {
        Flag = FALSE;
        for (Index = 0; (Index < ProtocolMsgNum) && (FALSE == Flag); Index++)
        {
            if ((ProtocolCtrlId != Index) && (DCM_MSG_WAIT != Dcm_MsgCtrl[Index].Dcm_MsgState))
            {
                Flag = TRUE;
            }
        }
        if (FALSE == Flag)
        {
#if DCM_DELAY_COMM_INACTIVE
            /*delay S3 time to Notice ComM exit "FULL Communication"*/
            DslInternal_S3ServerStart(ConnectionCfgCtrlId);
#else
            /*Notice ComM exit "FULL Communication"*/
            ComM_DCM_InactiveDiagnostic(NetworkChannel);
            Dcm_MkCtrl.Dcm_ActiveNetwork = DCM_INVALID_UINT8;
#endif
        }
    }
/*****************S3Serer timer****************/
#if (STD_ON == DCM_SESSION_FUNC_ENABLED)
    if (DCM_DEFAULT_SESSION != Dcm_MkCtrl.Dcm_ActiveSes)
    {
        Flag = FALSE;
        for (Index = 0; (Index < ProtocolMsgNum) && (FALSE == Flag); Index++)
        {
            if ((ProtocolCtrlId != Index) && (DCM_MSG_WAIT != Dcm_MsgCtrl[Index].Dcm_MsgState))
            {
                Flag = TRUE;
            }
        }
        if (FALSE == Flag)
        {
            /*restart s3timer */
            DslInternal_S3ServerStart(ConnectionCfgCtrlId);
        }
    }
#endif
    /*****************Reset Message*****************/
    RxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_RxCtrlChannelIndex;
    TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_TxCtrlChannelIndex;

    DslInternal_InitMsgCtrl(MsgCtrlIndexx);
    DslInternal_InitChannelCtrl(RxChannelCtrlIndex);
    DslInternal_InitChannelCtrl(TxChannelCtrlIndex);
    ProtocolId = DcmPbCfgPtr->pDcmDslCfg->pDcmDslProtocol->pDcmDslProtocolRow[ProtocolCtrlId].DcmDslProtocolID;
    ServiceType = Dcm_ClassifyServiceType(ProtocolId);
    if (ServiceType == DCM_OBD)
    {
        Dcm_OBDMessage.Length = 0u;
    }
}

/*************************************************************************/
/*
 * Brief               <Dcm_StartOfReception_Checklength>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(BufReq_ReturnType, DCM_CODE)
Dcm_StartOfReception_Checklength(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, DCM_CONST) info,
    PduLengthType TpSduLength,
    P2VAR(PduLengthType, AUTOMATIC, DCM_VAR) bufferSizePtr)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    BufReq_ReturnType bufRet = BUFREQ_E_NOT_OK;
    uint8 ConnectionCfgCtrlId;
    uint8 ProtocolCfgCtrlId;
    uint8 MsgCtrlIndex;
    uint8 RxChannelCfgCtrlId;
    uint8 TxChannelCfgCtrlId;
    Dcm_ProtocolType ProtocolId;
    uint8 ServiceType;
    uint32 len;
    Dcm_DslProtocolRxAddrType RxAddrType;
    const Dcm_DslCfgType* pDcmDslCfg = DcmPbCfgPtr->pDcmDslCfg;
    const Dcm_DslConnectionType* pDsl_Protocol_ConnectionCfg = DcmPbCfgPtr->DslProtocolConnectionCfg;
    const Dcm_DslProtocolRxType* pDsl_Protocol_Connection_RxCfg = DcmPbCfgPtr->DslProtocolConnectionRxCfg;
#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
    uint8 tempQueueIndex;
#endif /* STD_ON == DCM_REQUEST_QUEUED_ENABLED */
    const Dcm_DslProtocolRowType* pDcmDslProtocolRow;

    ConnectionCfgCtrlId = pDsl_Protocol_Connection_RxCfg[id].DcmDslParentConnectionCtrlId;
    ProtocolCfgCtrlId = pDsl_Protocol_ConnectionCfg[ConnectionCfgCtrlId].DcmDslParentProtocolRowCtrlId;
    MsgCtrlIndex = ProtocolCfgCtrlId;
    pDcmDslProtocolRow = &pDcmDslCfg->pDcmDslProtocol->pDcmDslProtocolRow[ProtocolCfgCtrlId];
    RxChannelCfgCtrlId = pDcmDslProtocolRow->DcmDslProtocolRxBufferRef->Dcm_DslBufferId;
    TxChannelCfgCtrlId = pDcmDslProtocolRow->DcmDslProtocolTxBufferRef->Dcm_DslBufferId;
    RxAddrType = pDsl_Protocol_Connection_RxCfg[id].DcmDslProtocolRxAddrType;
    ProtocolId = pDcmDslProtocolRow->DcmDslProtocolID;
    ServiceType = Dcm_ClassifyServiceType(ProtocolId);
    len = Dcm_StartOfReception_GetLen(ServiceType, RxChannelCfgCtrlId);
    /*Compare application buffer length and configuration of the channel length*/
    if (len < (uint32)TpSduLength)
    {
        bufRet = BUFREQ_E_OVFL;
    }
    else if (0u == TpSduLength)
    {
        /*idle*/
    }
    else
    {
        if ((Dcm_ChannelCtrl[RxChannelCfgCtrlId].Dcm_ChannelRxState == DCM_CH_OCCUPIED)
            || (Dcm_ChannelCtrl[TxChannelCfgCtrlId].Dcm_ChannelTxState == DCM_CH_OCCUPIED))
        {
#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
            if ((TRUE == pDcmDslProtocolRow->DcmDslProtocolRequestQueued) && (DCM_FUNCTIONAL != RxAddrType))
            {
                if (DCM_PENDING_REQUEST_NONE == Dcm_QueuedRequestCtrl[Dcm_QueuedIndex].Dcm_PendingRequestState)
                {
                    bufRet = BUFREQ_OK;
                    Dcm_QueuedRequestCtrl[Dcm_QueuedIndex].Dcm_PendingRequestState = DCM_PENDING_REQUEST_RECEIVE;
                }
                else if (
                    (DCM_MSG_WAIT == Dcm_MsgCtrl[MsgCtrlIndex].Dcm_MsgState)
                    || (DCM_PENDING_REQUEST_PROCESS == Dcm_QueuedRequestCtrl[Dcm_QueuedIndex].Dcm_PendingRequestState))
                {
                    tempQueueIndex = (Dcm_QueuedIndex == 0u) ? 1u : 0u;
                    if (DCM_PENDING_REQUEST_NONE == Dcm_QueuedRequestCtrl[tempQueueIndex].Dcm_PendingRequestState)
                    {
                        bufRet = BUFREQ_OK;
                        Dcm_QueuedIndex = tempQueueIndex;
                        Dcm_QueuedRequestCtrl[Dcm_QueuedIndex].Dcm_PendingRequestState = DCM_PENDING_REQUEST_RECEIVE;
                    }
                }
                else
                {
                    /* idle */
                }
            }
            else
            {
#endif /* STD_ON == DCM_REQUEST_QUEUED_ENABLED */
                if (FALSE == Dcm_ConnectionCtrl[ConnectionCfgCtrlId].Dcm_ConnectionActive)
                {
                    /*new diagnostic request with different connection*/
                    if ((DCM_FUNCTIONAL == RxAddrType) && (0u == Dcm_FunctionalMessage.Length))
                    {
                        /*new diagnostic request is test present or
                         * OBD service for function addressing*/
                        bufRet = BUFREQ_OK;
                    }
                    else
                    {
                        if (TRUE == pDcmDslCfg->pDcmDslDiagRespCfg->DcmDslDiagRespOnSecondDeclinedRequest)
                        {
                            /*the Dcm respond with a NRC 0x21*/
                            Dcm_ProtocolCtrl[ProtocolCfgCtrlId].MsgCtrlIndex = MsgCtrlIndex;
                            DsdInternal_SetNrc(ProtocolCfgCtrlId, DCM_E_BUSYREPEATREQUEST);
#if (STD_ON == DCM_GENERIC_CONNECTION)
                            if (DCM_INVALID_PDUID == Dcm_MsgCtrl[MsgCtrlIndex].DcmRxPduId)
                            {
                                Dcm_MsgCtrl[MsgCtrlIndex].Dcm_TxCtrlChannelIndex = TxChannelCfgCtrlId;
                                Dcm_MsgCtrl[MsgCtrlIndex].DcmRxPduId = id;
                            }
#endif /* STD_ON == DCM_GENERIC_CONNECTION */
                            DsdInternal_ProcessingDone(ProtocolCfgCtrlId);
                            bufRet = BUFREQ_OK;
                        }
                    }
                }
                else
                {
                    /*,new diagnostic request with the same DcmDslConnection*/
                    if ((DCM_FUNCTIONAL == RxAddrType) && (0u == Dcm_FunctionalMessage.Length))
                    {
                        /*functional addressing eg. concurrent Tester Present*/
                        bufRet = BUFREQ_OK;
                    }
                }
#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
            }
#endif /* STD_ON == DCM_REQUEST_QUEUED_ENABLED */
        }
        else
        {
            /*no diagnostic request is processing*/
            bufRet = BUFREQ_OK;
        }
    } /*ENDOF len < TpSduLength*/
    if (BUFREQ_OK == bufRet)
    {
        if ((info != NULL_PTR) && (len < (uint32)info->SduLength))
        {
            *bufferSizePtr = 0;
        }
        else
        {
            *bufferSizePtr = (PduLengthType)len;
        }
#if (STD_ON == DCM_GENERIC_CONNECTION)
        bufRet = Dcm_StartOfReception_metadata(id, info);
#endif
    }
    if (BUFREQ_OK == bufRet)
    {
        /*Start of a multi-frame request message or Reception of single-frame request message.*/
        Dcm_StopS3timer();
#if (STD_ON == DCM_UDS_SERVICE0X29_ENABLED)
        DspInternal_SetAuthenticationCtrlOff(ConnectionCfgCtrlId);
#endif
    }
    return bufRet;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

/*************************************************************************/
/*
 * Brief               <This function is called at the start of receiving an N-SDU.
                        The N-SDU might be fragmented into multiple N-PDUs (FF with one or more following CFs)
                        or might consist of a single N-PDU (SF).>
 * ServiceId           <0x46>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <id: Identification of the I-PDU.
 *                      info: Pointer to a PduInfoType structure containing the payload data
 *                      TpSduLength: Total length of the N-SDU to be received.>
 * Param-Name[out]     <bufferSizePtr:  Available receive buffer in the receiving module.>
 * Param-Name[in/out]  <None>
 * Return              <BUFREQ_OK
 *                      BUFREQ_E_NOT_OK
 *                      BUFREQ_E_OVFL
 *                      BUFREQ_E_BUSY>
 * PreCondition        <None>
 * CallByAPI           <>
 */
/*************************************************************************/
#define DCM_START_SEC_DCMSTARTOFRECEPTION_CALLBACK_CODE
#include "Dcm_MemMap.h"
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(BufReq_ReturnType, DCM_CODE)
Dcm_StartOfReception(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, DCM_CONST) info,
    PduLengthType TpSduLength,
    P2VAR(PduLengthType, AUTOMATIC, DCM_VAR) bufferSizePtr)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    BufReq_ReturnType bufRet = BUFREQ_E_NOT_OK;

#if (STD_ON == DCM_DEV_ERROR_DETECT)
    PduIdType RxIdNum = Dcm_GetRxIdNum();
    /*check whether Dcm is initialized*/
    if (DCM_ON != (Dcm_MkCtrl.Dcm_MkState))
    {
        DCM_DET_REPORTERROR(DCM_STARTOFRECEPTION_ID, DCM_E_UNINIT);
    }
    else if (NULL_PTR == bufferSizePtr)
    {
        /*Entry parameter validity checks*/
        DCM_DET_REPORTERROR(DCM_STARTOFRECEPTION_ID, DCM_E_PARAM_POINTER);
    }
    else if (id >= RxIdNum)
    {
        /*Entry parameter validity checks*/
        DCM_DET_REPORTERROR(DCM_STARTOFRECEPTION_ID, DCM_E_PARAM);
    }
    else
#endif
    {
        bufRet = Dcm_InterStartOfReception(id, info, TpSduLength, bufferSizePtr);
    } /*ENDOF DET*/
    return bufRet;
}
#define DCM_STOP_SEC_DCMSTARTOFRECEPTION_CALLBACK_CODE
#include "Dcm_MemMap.h"

/*************************************************************************/
/*
 * Brief               <This function is called to provide the received data
 *                      of an I-PDU segment (N-PDU) to the upper layer.Each
 *                      call to this function provides the next part of the
 *                      I-PDU data.The size of the remaining data is written
 *                      to the position indicated by bufferSizePtr>
 * ServiceId           <0x44>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <id: Identification of the received I-PDU.
 *                      info: Provides the source buffer (SduDataPtr) and the number
 *                            of bytes to be copied (SduLength).>
 * Param-Name[out]     <bufferSizePtr: Available receive buffer after data has been copied.>
 * Param-Name[in/out]  <None>
 * Return              <BUFREQ_OK
 *                      BUFREQ_E_NOT_OK>
 * PreCondition        <None>
 * CallByAPI           <>
 */
/*************************************************************************/
#define DCM_START_SEC_DCMCOPYRXDATA_CALLBACK_CODE
#include "Dcm_MemMap.h"
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(BufReq_ReturnType, DCM_CODE)
Dcm_CopyRxData(
    /* PRQA S 1532-- */ /* MISRA Rule 8.7 */
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, DCM_CONST) info,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(PduLengthType, AUTOMATIC, DCM_VAR) bufferSizePtr)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 Sid = 0;
    uint8 SubFunction;
    uint8 ConnectionCfgId;
    uint8 ProtocolCfgId;
    uint8 RxChannelCfgCtrlId;
    uint32 Offset;
    uint32 AvailableBufferSize;
    Dcm_ProtocolType ProtocolRowID;
    uint8 ServiceType;
    BufReq_ReturnType bufRet = BUFREQ_E_NOT_OK;
    const Dcm_DslCfgType* pDcmDslCfg;
    const Dcm_DslConnectionType* pDsl_Protocol_ConnectionCfg;
    const Dcm_DslProtocolRxType* pDsl_Protocol_Connection_RxCfg;
    Dcm_DslProtocolRxAddrType RxAddrType;
    const Dcm_DslBufferType* pDcmChannelCfg;
#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
    Dcm_QueuedRequestCtrlType* pDcm_QueuedRequestCtrl = &Dcm_QueuedRequestCtrl[Dcm_QueuedIndex];
#endif /* STD_ON == DCM_REQUEST_QUEUED_ENABLED */

#if (STD_ON == DCM_DEV_ERROR_DETECT)
    PduIdType RxIdNum = Dcm_GetRxIdNum();
    /*The DCM module status check*/
    if (DCM_ON != (Dcm_MkCtrl.Dcm_MkState))
    {
        DCM_DET_REPORTERROR(DCM_COPYRXDATA_ID, DCM_E_UNINIT);
    }
    else if ((NULL_PTR == info) || (NULL_PTR == bufferSizePtr))
    {
        /*Parameter validation*/
        DCM_DET_REPORTERROR(DCM_COPYRXDATA_ID, DCM_E_PARAM_POINTER);
    }
    else if (id >= RxIdNum)
    {
        /*Parameter validation*/
        DCM_DET_REPORTERROR(DCM_COPYRXDATA_ID, DCM_E_PARAM);
    }
    else
#endif
    {
        pDcmDslCfg = DcmPbCfgPtr->pDcmDslCfg;
        pDsl_Protocol_ConnectionCfg = DcmPbCfgPtr->DslProtocolConnectionCfg;
        pDsl_Protocol_Connection_RxCfg = DcmPbCfgPtr->DslProtocolConnectionRxCfg;
        ConnectionCfgId = pDsl_Protocol_Connection_RxCfg[id].DcmDslParentConnectionCtrlId;
        ProtocolCfgId = pDsl_Protocol_ConnectionCfg[ConnectionCfgId].DcmDslParentProtocolRowCtrlId;
        RxChannelCfgCtrlId = DcmPbCfgPtr->pDcmDslCfg->pDcmDslProtocol->pDcmDslProtocolRow[ProtocolCfgId]
                                 .DcmDslProtocolRxBufferRef->Dcm_DslBufferId;
        pDcmChannelCfg = &pDcmDslCfg->pDcmChannelCfg[RxChannelCfgCtrlId];
        ProtocolRowID = ((pDcmDslCfg->pDcmDslProtocol)->pDcmDslProtocolRow)[ProtocolCfgId].DcmDslProtocolID;
        ServiceType = Dcm_ClassifyServiceType(ProtocolRowID);
        RxAddrType = pDsl_Protocol_Connection_RxCfg[id].DcmDslProtocolRxAddrType;

        if (RxAddrType == DCM_FUNCTIONAL)
        {
            bufRet = Dcm_CopyRxData_SessionDeal(info, ServiceType, &Sid, &SubFunction, ProtocolCfgId);
        }
        else
        {
            Offset = Dcm_ChannelCtrl[RxChannelCfgCtrlId].Dcm_BufferCunrentPosition;
#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
            if (DCM_PENDING_REQUEST_RECEIVE != Dcm_QueuedRequestCtrl[Dcm_QueuedIndex].Dcm_PendingRequestState)
            {
                AvailableBufferSize = pDcmChannelCfg->Dcm_DslBufferSize - (Offset - pDcmChannelCfg->offset);
            }
            else
            {
                AvailableBufferSize = pDcmChannelCfg->Dcm_DslBufferSize
                                      - Dcm_QueuedRequestCtrl[Dcm_QueuedIndex].Dcm_QueuedRequestBufferCunrentPosition;
            }
#else  /* STD_ON == DCM_REQUEST_QUEUED_ENABLED */
            AvailableBufferSize = pDcmChannelCfg->Dcm_DslBufferSize - (Offset - pDcmChannelCfg->offset);
#endif /* STD_ON == DCM_REQUEST_QUEUED_ENABLED */
            if (0u == info->SduLength)
            {
                *bufferSizePtr = (PduLengthType)AvailableBufferSize;
                bufRet = BUFREQ_OK;
            }
            else if ((uint32)info->SduLength > AvailableBufferSize)
            {
                bufRet = BUFREQ_E_NOT_OK;
            }
            else
#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
                if (DCM_PENDING_REQUEST_RECEIVE != pDcm_QueuedRequestCtrl->Dcm_PendingRequestState)
#endif /* STD_ON == DCM_REQUEST_QUEUED_ENABLED */
            {
                /* lock buffer*/
                SchM_Enter_Dcm_ExclusiveArea(); /* Dcm_ChannelCtrl */ /*Dcm_Channel */
                Dcm_ChannelCtrl[RxChannelCfgCtrlId].Dcm_ChannelRxState = DCM_CH_OCCUPIED;
                Dcm_MemoryCopy(info->SduDataPtr, &Dcm_Channel[Offset], info->SduLength);
                Offset = Offset + (uint32)info->SduLength;
                Dcm_ChannelCtrl[RxChannelCfgCtrlId].Dcm_BufferCunrentPosition = Offset;
                SchM_Exit_Dcm_ExclusiveArea();
                *bufferSizePtr = (PduLengthType)(pDcmChannelCfg->Dcm_DslBufferSize
                                                 - (Offset - (pDcmDslCfg->pDcmChannelCfg)[RxChannelCfgCtrlId].offset));
                bufRet = BUFREQ_OK;
            }
#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
            else
            {
                Offset = pDcm_QueuedRequestCtrl->Dcm_QueuedRequestBufferCunrentPosition;
                Dcm_MemoryCopy(info->SduDataPtr, &(Dcm_RequestQueuedBuffer[Dcm_QueuedIndex][Offset]), info->SduLength);
                pDcm_QueuedRequestCtrl->Dcm_QueuedRequestBufferCunrentPosition += info->SduLength;
                *bufferSizePtr = (PduLengthType)(pDcmChannelCfg->Dcm_DslBufferSize
                                                 - pDcm_QueuedRequestCtrl->Dcm_QueuedRequestBufferCunrentPosition);
                bufRet = BUFREQ_OK;
            }
#endif /* STD_ON == DCM_REQUEST_QUEUED_ENABLED */
        }
    }
    return bufRet;
}
#define DCM_STOP_SEC_DCMCOPYRXDATA_CALLBACK_CODE
#include "Dcm_MemMap.h"

/*************************************************************************/
/*
 * Brief               <Called after an I-PDU has been received via the
 *                      TP API, the result indicates whether the
 *                      transmission was successful or not.>
 * ServiceId           <0x45>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <id:ID of DCM I-PDU that has been received
 *                      result: The result of the diagnostic request message received>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <this function must be called after
 * call of Dcm_StartOfReception() and Dcm_CopyRxData()>
 * CallByAPI           <None>
 */
/*************************************************************************/
#define DCM_START_SEC_DCMTPRXINDICATION_CALLBACK_CODE
#include "Dcm_MemMap.h"
FUNC(void, DCM_CODE) Dcm_TpRxIndication(PduIdType id, Std_ReturnType result) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    const Dcm_DslConnectionType* pDslConnection;
    Dcm_ProtocolType ProtocolId;
    uint8 ServiceType;
    Dcm_ChannelStateType TxChannelTxState;
    uint8 ProtocolCfgCtrlId;
    uint8 ConnectionCfgId;
    uint8 MsgCtrlIndexx;
    uint8 RxChannelCfgCtrlId;
    uint8 TxChannelCfgCtrlId;
    Dcm_DslProtocolRxAddrType DslProtocolRxAddrType;
    Std_ReturnType ret = E_OK;
    const Dcm_DslCfgType* pDcmDslCfg;
    const Dcm_DslConnectionType* pDsl_Protocol_ConnectionCfg;
    const Dcm_DslProtocolRxType* pDsl_Protocol_Connection_RxCfg;
    NetworkHandleType NetworkChannel;
    uint8 Index;
    uint8 MainConnectionNum = Dcm_GetMainConnectionNum();
#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
    Dcm_QueuedRequestCtrlType* pDcm_QueuedRequestCtrl = &Dcm_QueuedRequestCtrl[Dcm_QueuedIndex];
#endif /* STD_ON == DCM_REQUEST_QUEUED_ENABLED */

#if (STD_ON == DCM_DEV_ERROR_DETECT)
    /*To determine whether the DCM module initialization*/
    if (DCM_ON != (Dcm_MkCtrl.Dcm_MkState))
    {
        DCM_DET_REPORTERROR(DCM_TPRXINDICATION_ID, DCM_E_UNINIT);
    }
    else
#endif
    {
        pDsl_Protocol_ConnectionCfg = DcmPbCfgPtr->DslProtocolConnectionCfg;
        pDsl_Protocol_Connection_RxCfg = DcmPbCfgPtr->DslProtocolConnectionRxCfg;
        ConnectionCfgId = pDsl_Protocol_Connection_RxCfg[id].DcmDslParentConnectionCtrlId;
        ProtocolCfgCtrlId = pDsl_Protocol_ConnectionCfg[ConnectionCfgId].DcmDslParentProtocolRowCtrlId;
        RxChannelCfgCtrlId = DcmPbCfgPtr->pDcmDslCfg->pDcmDslProtocol->pDcmDslProtocolRow[ProtocolCfgCtrlId]
                                 .DcmDslProtocolRxBufferRef->Dcm_DslBufferId;
        pDcmDslCfg = DcmPbCfgPtr->pDcmDslCfg;
        /**No MsgCtrlIndex ,ResetResource will not clear connect*/
        /*Receive complete results analysis*/
        if (result != E_OK)
        {
#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
            if (DCM_PENDING_REQUEST_RECEIVE != pDcm_QueuedRequestCtrl->Dcm_PendingRequestState)
#else
            if (Dcm_FunctionalMessage.ConcurrentTesterFailed == FALSE)
#endif
            {
                /*****************S3Serer timer****************/
                Dcm_RestartS3timer(ConnectionCfgId);
                DslInternal_InitChannelCtrl(RxChannelCfgCtrlId);
            }
            else
#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
            {
                pDcm_QueuedRequestCtrl->Dcm_PendingRequestState = DCM_PENDING_REQUEST_NONE;
                pDcm_QueuedRequestCtrl->Dcm_QueuedRequestBufferCunrentPosition = 0u;
            }
#else
            {
                /* 2nd functional requests with the same protocol.*/
                Dcm_FunctionalMessage.ConcurrentTesterFailed = FALSE;
            }
#endif
        }
        else
        {
            /*functional request packet processing*/
            ProtocolId =
                DcmPbCfgPtr->pDcmDslCfg->pDcmDslProtocol->pDcmDslProtocolRow[ProtocolCfgCtrlId].DcmDslProtocolID;
            ServiceType = Dcm_ClassifyServiceType(ProtocolId);
            if (DCM_UDS == ServiceType)
            {
                /*check for UDS*/
                ret = Dcm_TpRxIndication_IsFuncAddress(id);
            }
            if (E_OK == ret)
            {
#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
                if (DCM_PENDING_REQUEST_RECEIVE != pDcm_QueuedRequestCtrl->Dcm_PendingRequestState)
#endif /* STD_ON == DCM_REQUEST_QUEUED_ENABLED */
                {
                    /*To find DcmRxPduId corresponding protocol index number,
                     *in the protocol configuration table */
                    MsgCtrlIndexx = ProtocolCfgCtrlId;
                    TxChannelCfgCtrlId = DcmPbCfgPtr->pDcmDslCfg->pDcmDslProtocol->pDcmDslProtocolRow[ProtocolCfgCtrlId]
                                             .DcmDslProtocolTxBufferRef->Dcm_DslBufferId;
                    /*Determine relationship between the message control
                     * block and the corresponding protocol control block */
                    SchM_Enter_Dcm_ExclusiveArea();
                    Dcm_ProtocolCtrl[ProtocolCfgCtrlId].ProtocolId =
                        (pDcmDslCfg->pDcmDslProtocol->pDcmDslProtocolRow)[ProtocolCfgCtrlId].DcmDslProtocolID;
                    Dcm_ProtocolCtrl[ProtocolCfgCtrlId].ProtocolPri =
                        (pDcmDslCfg->pDcmDslProtocol->pDcmDslProtocolRow)[ProtocolCfgCtrlId].DcmDslProtocolPriority;
                    Dcm_ProtocolCtrl[ProtocolCfgCtrlId].MsgCtrlIndex = MsgCtrlIndexx;
                    Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_RxCtrlChannelIndex = RxChannelCfgCtrlId;
                    Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_TxCtrlChannelIndex = TxChannelCfgCtrlId;
                    SchM_Exit_Dcm_ExclusiveArea();
                    Dcm_MkCtrl.Dcm_ActiveNetwork =
                        pDsl_Protocol_ConnectionCfg[ConnectionCfgId].pDcmDslMainConnection->DcmDslProtocolComMChannelId;
                    pDslConnection =
                        pDcmDslCfg->pDcmDslProtocol->pDcmDslProtocolRow[ProtocolCfgCtrlId].pDcmDslConnection;
                    DslProtocolRxAddrType = pDsl_Protocol_Connection_RxCfg[id].DcmDslProtocolRxAddrType;
                    /* Determine relationship between the static configuration channel
                     * and receive/transmit channel control block */
                    SchM_Enter_Dcm_ExclusiveArea();
                    Dcm_ChannelCtrl[TxChannelCfgCtrlId].Dcm_ChannelCfgIndex = TxChannelCfgCtrlId;
                    Dcm_ChannelCtrl[RxChannelCfgCtrlId].Dcm_ChannelCfgIndex = RxChannelCfgCtrlId;
                    SchM_Exit_Dcm_ExclusiveArea();
                    TxChannelTxState = Dcm_ChannelCtrl[TxChannelCfgCtrlId].Dcm_ChannelTxState;
                    if (DCM_CH_OCCUPIED == TxChannelTxState)
                    {
                        /*Corresponding transmit channel status is not normal*/
                        DslInternal_ResetResource(ProtocolCfgCtrlId);
                    }
                    else
                    {
                        SchM_Enter_Dcm_ExclusiveArea();
                        Dcm_ChannelCtrl[TxChannelCfgCtrlId].Dcm_ChannelTxState = DCM_CH_OCCUPIED;
                        if (Dcm_ChannelCtrl[RxChannelCfgCtrlId].Dcm_ChannelRxState != DCM_CH_OCCUPIED)
                        {
                            /*added for Fun Eth Request*/
                            Dcm_ChannelCtrl[RxChannelCfgCtrlId].Dcm_ChannelRxState = DCM_CH_OCCUPIED;
                        }
                        /*Determine relationships between the receive/transmit channel control
                         * block and the corresponding message control block */
                        if (ServiceType == DCM_OBD)
                        {
                            Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.ReqDataLen = (Dcm_MsgLenType)Dcm_OBDMessage.Length;
                            Dcm_MsgCtrl[MsgCtrlIndexx].SID = Dcm_OBDMessage.Buffer[0];
                            Dcm_MsgCtrl[MsgCtrlIndexx].Subfunction = Dcm_OBDMessage.Buffer[1];
                            Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.pReqData = &Dcm_OBDMessage.Buffer[0];
                        }
                        else
                        {
                            Dcm_MsgCtrl[MsgCtrlIndexx].SID = Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.pReqData[0];
                            Dcm_MsgCtrl[MsgCtrlIndexx].Subfunction =
                                (Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.pReqData)[1];
                        }
                        Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.MsgAddInfo.ReqType = (uint8)DslProtocolRxAddrType;
                        Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.DcmRxPduId = id;
                        Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_RxCtrlChannelIndex = RxChannelCfgCtrlId;
                        Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_TxCtrlChannelIndex = TxChannelCfgCtrlId;
                        SchM_Exit_Dcm_ExclusiveArea();
                        if (NULL_PTR == pDslConnection->pDcmDslMainConnection->pDcmDslProtocolTx)
                        {
                            Dcm_MsgCtrl[MsgCtrlIndexx].DcmTxPduId = DCM_INVALID_UINT8;
                        }
                        else
                        {
                            Dcm_TpRxIndication_SetTxPduId(MsgCtrlIndexx, pDslConnection);
                        }
                        Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_MsgState = DCM_MSG_RECEIVED;
                        NetworkChannel = pDslConnection->pDcmDslMainConnection->DcmDslProtocolComMChannelId;
                        for (Index = 0; Index < MainConnectionNum; Index++)
                        {
                            if (NetworkChannel == Dcm_CommCtrl[Index].DcmDslProtocolComMChannelId)
                            {
                                /*ComM interactive processing*/
                                if ((DCM_COMM_ACTIVE == Dcm_CommCtrl[Index].Dcm_ActiveDiagnostic)
                                    && (DCM_DEFAULT_SESSION == Dcm_MkCtrl.Dcm_ActiveSes))
                                {
                                    /*The notification ComM into the "FULL Communication"*/
                                    ComM_DCM_ActiveDiagnostic(NetworkChannel);
                                }
                            }
                        }
                        Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_MsgState = DCM_MSG_PROCESSED;
                    }
                }
#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
                else
                {
                    pDcm_QueuedRequestCtrl->Dcm_PendingRequestState = DCM_PENDING_REQUEST_READY;
                }
#endif /* STD_ON == DCM_REQUEST_QUEUED_ENABLED */
            }
#if DCM_DELAY_COMM_INACTIVE
            else
            {
                /** 3E80 Specific process */
                Dcm_MkCtrl.Dcm_ActiveNetwork =
                    pDsl_Protocol_ConnectionCfg[ConnectionCfgId].pDcmDslMainConnection->DcmDslProtocolComMChannelId;
                ComM_DCM_ActiveDiagnostic(Dcm_MkCtrl.Dcm_ActiveNetwork);
            }
#endif
        }
    }
}
#define DCM_STOP_SEC_DCMTPRXINDICATION_CALLBACK_CODE
#include "Dcm_MemMap.h"

/*************************************************************************/
/*
 * Brief               <This function is called to provide the received data
 *                      of an I-PDU segment (N-PDU) to the upper layer.Each
 *                      call to this function provides the next part of the
 *                      I-PDU data.The size of the remaining data is written
 *                      to the position indicated by bufferSizePtr>
 * ServiceId           <0x43>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <id: Identification of the transmitted I-PDU.
 *                      info: Provides the destination buffer (SduDataPtr) and the number
 *                            of bytes to be copied (SduLength).
 *                      retry: This parameter is used to acknowledge transmitted data or to
 *                             retransmit data after transmission problems.>
 * Param-Name[out]     <availableDataPtr: Indicates the remaining number of bytes that are
 *                                        available in the upper layer module's Tx buffer.>
 * Param-Name[in/out]  <None>
 * Return              <BUFREQ_OK
 *                      BUFREQ_E_NOT_OK
 *                      BUFREQ_E_BUSY>
 * PreCondition        <None>
 * CallByAPI           <>
 */
/*************************************************************************/
#define DCM_START_SEC_DCMCOPYTXDATA_CALLBACK_CODE
#include "Dcm_MemMap.h"
FUNC(BufReq_ReturnType, DCM_CODE)
Dcm_CopyTxData(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, DCM_CONST) info,
    P2CONST(RetryInfoType, AUTOMATIC, DCM_CONST) retry,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(PduLengthType, AUTOMATIC, DCM_VAR) availableDataPtr)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 ProtocolCfgId;
    uint8 ConnectionCfgId;
    uint8 MsgCtrlId;
    uint8 TxChannelCtrlIndex;
    uint32* Position;
    uint32 Len;
    Std_ReturnType ret;
    BufReq_ReturnType bufRet = BUFREQ_E_NOT_OK;
    const Dcm_DslConnectionType* pDsl_Protocol_ConnectionCfg;
    const Dcm_DslProtocolTxType* pDsl_Protocol_Connection_TxCfg;
    const Dcm_MsgContextType* pMsgContext;

#if (STD_ON == DCM_DEV_ERROR_DETECT)
    PduIdType TxIdNum = Dcm_GetTxIdNum();
    /*The DCM module status check*/
    if (DCM_ON != Dcm_MkCtrl.Dcm_MkState)
    {
        DCM_DET_REPORTERROR(DCM_COPYTXDATA_ID, DCM_E_UNINIT);
    }
    /*Parameter validation*/
    else if ((NULL_PTR == info) || (NULL_PTR == availableDataPtr))
    {
        DCM_DET_REPORTERROR(DCM_COPYTXDATA_ID, DCM_E_PARAM_POINTER);
    }
    /*Parameter validation*/
    else if (id >= TxIdNum)
    {
        DCM_DET_REPORTERROR(DCM_COPYTXDATA_ID, DCM_E_PARAM);
    }
    else
#endif
    {
        ret = Dcm_CopyTxData_CheckCtrlStatus(id);
        if (E_NOT_OK == ret)
        {
            bufRet = BUFREQ_E_NOT_OK;
        }
        else
        {
            pDsl_Protocol_ConnectionCfg = DcmPbCfgPtr->DslProtocolConnectionCfg;
            pDsl_Protocol_Connection_TxCfg = DcmPbCfgPtr->DslProtocolConnectionTxCfg;
            ConnectionCfgId = pDsl_Protocol_Connection_TxCfg[id].DcmDslParentConnectionCtrlId;
            ProtocolCfgId = pDsl_Protocol_ConnectionCfg[ConnectionCfgId].DcmDslParentProtocolRowCtrlId;
            MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCfgId].MsgCtrlIndex;
            TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
            pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
            Position = &Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_BufferCunrentPosition;
            Len = *Position + (uint32)info->SduLength;
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
            if (Dcm_PageBufferData.TotalSize != 0UL)
            {
                bufRet = Dcm_CopyTxData_PagebufferDeal(ProtocolCfgId, info, TxChannelCtrlIndex, availableDataPtr);
            }
            else
#endif
            {
                /*ResDataLen is total length*/
                if (Len > pMsgContext->ResDataLen)
                {
                    bufRet = BUFREQ_E_BUSY;
                }
                else
                {
                    if ((NULL_PTR == retry) || (TP_DATACONF == retry->TpDataState))
                    {
                        Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_BufferErasePosition = *Position;
                    }
                    else if (TP_DATARETRY == retry->TpDataState)
                    {
                        *Position = *Position - (uint32)retry->TxTpDataCnt;
                    }
                    else
                    {
                        /*TP_CONFPENDING,do nothing*/
                    }

                    if (info->SduLength != 0u)
                    {
                        Dcm_MemoryCopy(&(pMsgContext->pResData[*Position]), info->SduDataPtr, info->SduLength);
                        *Position = *Position + (uint32)info->SduLength;
                    }

                    *availableDataPtr = (PduLengthType)(pMsgContext->ResMaxDataLen - *Position);
                    bufRet = BUFREQ_OK;
                }
            }
        }
    }
    return bufRet;
}
#define DCM_STOP_SEC_DCMCOPYTXDATA_CALLBACK_CODE
#include "Dcm_MemMap.h"

/*************************************************************************/
/*
 * Brief               <This function is called after the I-PDU has been transmitted
                        on its network, the result indicates whether the transmission
                        was successful or not.>
 * ServiceId           <0x48>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <id: Identification of the transmitted I-PDU.
 *                      result: Result of the transmission of the I-PDU.>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <>
 */
/*************************************************************************/
#define DCM_START_SEC_DCMTPTXCONFIRMATION_CALLBACK_CODE
#include "Dcm_MemMap.h"
FUNC(void, DCM_CODE) Dcm_TpTxConfirmation(PduIdType id, Std_ReturnType result)
{
    uint8 ProtocolCfgCtrlId;
    uint8 ConnectionCfgId;
    uint8 MsgCtrlIndex;
    const Dcm_DslConnectionType* pDsl_Protocol_ConnectionCfg;
    const Dcm_DslProtocolTxType* pDsl_Protocol_Connection_TxCfg;

#if (STD_ON == DCM_DEV_ERROR_DETECT)
    /*The DCM module status check*/
    if (DCM_ON != (Dcm_MkCtrl.Dcm_MkState))
    {
        DCM_DET_REPORTERROR(DCM_TXCONFIRMATION_ID, DCM_E_UNINIT);
    }
    else
#endif
    {
        pDsl_Protocol_ConnectionCfg = DcmPbCfgPtr->DslProtocolConnectionCfg;
        pDsl_Protocol_Connection_TxCfg = DcmPbCfgPtr->DslProtocolConnectionTxCfg;
        /*find DcmTxPduId corresponding protocol index number,
         * in the protocol configuration table*/
        ConnectionCfgId = pDsl_Protocol_Connection_TxCfg[id].DcmDslParentConnectionCtrlId;
        ProtocolCfgCtrlId = pDsl_Protocol_ConnectionCfg[ConnectionCfgId].DcmDslParentProtocolRowCtrlId;
        MsgCtrlIndex = ProtocolCfgCtrlId;
        switch (result)
        {
        case E_NOT_OK:
            /*Confirmation message shows: response failed to send*/
            DslInternal_ResetResource(ProtocolCfgCtrlId);
            break;
        case E_OK:
            if (TRUE == Dcm_CheckUDS0x86_TxConfirmation(ProtocolCfgCtrlId))
            {
                /*Confirmation message shows: the response
                 * is sent to notify the DSD layer further confirmation*/
                /*Set corresponding to the message status = "DCM_MSG_CONFIRMATION*/
                SchM_Enter_Dcm_ExclusiveArea();
                Dcm_MsgCtrl[MsgCtrlIndex].Dcm_MsgState = DCM_MSG_CONFIRMATION;
#if (STD_ON == DCM_DSLDIAGRESP_FORCERESPENDEN)
                if (DCM_E_FORCE_RCRRP == Dcm_MsgCtrl[MsgCtrlIndex].Dcm_Ret)
                {
                    Dcm_MsgCtrl[MsgCtrlIndex].Dcm_OpStatus = DCM_FORCE_RCRRP_OK;
                }
#endif
                SchM_Exit_Dcm_ExclusiveArea();
                DsdInternal_TxConfirmation(ProtocolCfgCtrlId);
            }
            break;
        default:
            DslInternal_ResetResource(ProtocolCfgCtrlId);
            break;
        }
    }
}
#define DCM_STOP_SEC_DCMTPTXCONFIRMATION_CALLBACK_CODE
#include "Dcm_MemMap.h"

/*************************************************************************/
/*
 * Brief               <The lower layer communication interface module
 *                       confirms the transmission of an IPDU.>
 * ServiceId           <0x40>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <DcmTxPduId:ID of DCM I-PDU that has been transmitted.
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <>
 */
/*************************************************************************/
#define DCM_START_SEC_DCMTXCONFIRMATION_CALLBACK_CODE
#include "Dcm_MemMap.h"
FUNC(void, DCM_CODE) Dcm_TxConfirmation(PduIdType DcmTxPduId)
{
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    /*The DCM module status check*/
    if (DCM_ON != (Dcm_MkCtrl.Dcm_MkState))
    {
        DCM_DET_REPORTERROR(DCM_TXCONFIRMATION_ID, DCM_E_UNINIT);
    }
    else
#endif
    {
        Dcm_TxConfirmation_2A(DcmTxPduId);
    }
}
#define DCM_STOP_SEC_DCMTXCONFIRMATION_CALLBACK_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"

/*************************************************************************/
/*
 * Brief               <DSL layer sends the response function>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:Protocol control block index number.>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <DsdInternal_ProcessingDone>
 */
/*************************************************************************/
FUNC(void, DCM_CODE) DslInternal_ProcessingDone(uint8 ProtocolCtrlId)
{
    PduIdType DcmTxPduIdx;
    PduInfoType PduInfo;
    uint8 MsgCtrlIndexx;
    Std_ReturnType ret;
    uint8 Index;
    uint8 iloop;
    boolean ProcessOn = TRUE;
    uint8 ConnectionNum = Dcm_GetConnectionNum();
    uint8 MainConnectionNum = Dcm_GetMainConnectionNum();
    const Dcm_DslConnectionType* pDsl_Protocol_ConnectionCfg = DcmPbCfgPtr->DslProtocolConnectionCfg;
#if (STD_ON == DCM_GENERIC_CONNECTION)
    const Dcm_DslProtocolRxType* pDsl_Protocol_Connection_RxCfg = DcmPbCfgPtr->DslProtocolConnectionRxCfg;
    PduIdType DcmRxPduIdx;
    uint8 MetaDataFlag;

    PduInfo.MetaDataPtr = &(Dcm_Metadata[ProtocolCtrlId][0]);
#endif
    const Dcm_DslMainConnectionType* pDcmDslMainConnection;
    Dcm_MsgCtrlType* pDcm_MsgCtrl;

    MsgCtrlIndexx = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    pDcm_MsgCtrl = &Dcm_MsgCtrl[MsgCtrlIndexx];
    DcmTxPduIdx = pDcm_MsgCtrl->DcmTxPduId;
#if (STD_ON == DCM_GENERIC_CONNECTION)
    DcmRxPduIdx = pDcm_MsgCtrl->MsgContext.DcmRxPduId;
    MetaDataFlag = pDsl_Protocol_Connection_RxCfg[DcmRxPduIdx].DcmDslMetaDataFlag;
    if ((MetaDataFlag & DCM_METADATA_LENGTH_MASK) > 0u)
    {
        if ((MetaDataFlag & DCM_SA16_AND_TA16_MASK) == DCM_SA16_AND_TA16_MASK)
        {
#if (DCM_CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
            PduInfo.MetaDataPtr[0u] = (uint8)((pDcm_MsgCtrl->Dcm_MetaData_TA & 0xFF00u) >> 8u);
            PduInfo.MetaDataPtr[1u] = (uint8)(pDcm_MsgCtrl->Dcm_MetaData_TA & 0xFFu);
            PduInfo.MetaDataPtr[2u] = (uint8)((pDcm_MsgCtrl->Dcm_MetaData_SA & 0xFF00u) >> 8u);
            PduInfo.MetaDataPtr[3u] = (uint8)(pDcm_MsgCtrl->Dcm_MetaData_SA & 0xFFu);
#else
            PduInfo.MetaDataPtr[0u] = (uint8)(pDcm_MsgCtrl->Dcm_MetaData_TA & 0xFFu);
            PduInfo.MetaDataPtr[1u] = (uint8)((pDcm_MsgCtrl->Dcm_MetaData_TA & 0xFF00u) >> 8u);
            PduInfo.MetaDataPtr[2u] = (uint8)(pDcm_MsgCtrl->Dcm_MetaData_SA & 0xFFu);
            PduInfo.MetaDataPtr[3u] = (uint8)((pDcm_MsgCtrl->Dcm_MetaData_SA & 0xFF00u) >> 8u);
#endif
        }
    }
#else
    PduInfo.MetaDataPtr = NULL_PTR;
#endif
    PduInfo.SduLength = (PduLengthType)(pDcm_MsgCtrl->MsgContext.ResMaxDataLen);
    for (Index = 0; Index < ConnectionNum; Index++)
    {
        if (pDsl_Protocol_ConnectionCfg[Index].DcmDslParentProtocolRowCtrlId == ProtocolCtrlId)
        {
            pDcmDslMainConnection = pDsl_Protocol_ConnectionCfg[Index].pDcmDslMainConnection;
            for (iloop = 0; iloop < MainConnectionNum; iloop++)
            {
                if ((pDcmDslMainConnection != NULL_PTR)
                    && (pDcmDslMainConnection->DcmDslProtocolComMChannelId
                        == Dcm_CommCtrl[iloop].DcmDslProtocolComMChannelId)
                    && (Dcm_CommCtrl[iloop].Dcm_CommState != DCM_COMM_FULL_COMMUNICATION))
                {
                    ret = DslInternal_MaxIsLargerThanP2Server(ProtocolCtrlId);
                    if (E_OK == ret)
                    {
                        /*time expired*/
                        DslInternal_ResetResource(ProtocolCtrlId);
                    }
                    ProcessOn = FALSE;
                }
            }
        }
    }

    /*the Multiplicity of DcmDslProtocolTx is "0"*/
    if ((ProcessOn == TRUE) && (DcmTxPduIdx == DCM_INVALID_UINT8))
    {
        /*process the received diagnostic request without sending a response*/
        DslInternal_ResetResource(ProtocolCtrlId);
        ProcessOn = FALSE;
    }

    if (ProcessOn == TRUE)
    {
        ret = DslInternal_MinIsLargerThanP2Server(ProtocolCtrlId);
        if (E_NOT_OK == ret)
        {
            /*Response time <P2ServerMin*/
            ProcessOn = FALSE;
        }
    }
    if (ProcessOn == TRUE)
    {
        ret = PduR_DcmTransmit(DcmTxPduIdx, &PduInfo);
        if (E_NOT_OK == ret)
        {
            /*Failed to send*/
            DslInternal_ResetResource(ProtocolCtrlId);
            ProcessOn = FALSE;
        }
    }
    if (ProcessOn == TRUE)
    {
        pDcm_MsgCtrl->SendFlag = TRUE;

        /****Close P2Server Timer****/
        DslInternal_P2ServerStop(ProtocolCtrlId);
    }
}

/*************************************************************************/
/*
 * Brief               <Dcm_CopyTxData_CheckCtrlStatus>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static inline FUNC(Std_ReturnType, DCM_CODE) Dcm_CopyTxData_CheckCtrlStatus(PduIdType DcmTxPduId)
{
    uint8 ProtocolCfgId;
    uint8 ConnectionCfgId;
    uint8 MsgCtrlId;
    uint8 RxChannelCtrlId;
    uint8 TxChannelCtrlId;
    Std_ReturnType ret = E_OK;
    const Dcm_DslConnectionType* pDsl_Protocol_ConnectionCfg = DcmPbCfgPtr->DslProtocolConnectionCfg;
    const Dcm_DslProtocolTxType* pDsl_Protocol_Connection_TxCfg = DcmPbCfgPtr->DslProtocolConnectionTxCfg;

    /*To find DcmTxPduId corresponding Protocol index number,
     *in the protocol configuration table*/
    ConnectionCfgId = pDsl_Protocol_Connection_TxCfg[DcmTxPduId].DcmDslParentConnectionCtrlId;
    ProtocolCfgId = pDsl_Protocol_ConnectionCfg[ConnectionCfgId].DcmDslParentProtocolRowCtrlId;
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCfgId].MsgCtrlIndex;

    if (DCM_INVALID_UINT8 == MsgCtrlId)
    {
        ret = E_NOT_OK;
    }
    else
    {
        RxChannelCtrlId = Dcm_MsgCtrl[MsgCtrlId].Dcm_RxCtrlChannelIndex;
        TxChannelCtrlId = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    }
    if ((E_OK == ret) && (DCM_MSG_TRANSMISSION != Dcm_MsgCtrl[MsgCtrlId].Dcm_MsgState)
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
        && (Dcm_PageBufferData.TotalSize == 0UL)
#endif
    )
    {
        /*The state machine does not operate properly, return BUFREQ_E_NOT_OK*/
        ret = E_NOT_OK;
    }

    if ((E_OK == ret) && (DCM_CH_OCCUPIED != Dcm_ChannelCtrl[RxChannelCtrlId].Dcm_ChannelRxState))
    {
        /*Receiver channel status is not correct*/
        ret = E_NOT_OK;
    }
    if ((E_OK == ret) && (DCM_CH_OCCUPIED != Dcm_ChannelCtrl[TxChannelCtrlId].Dcm_ChannelTxState))
    {
        /*The send channel status is incorrect*/
        ret = E_NOT_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <Dcm_CheckProtocol>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(Std_ReturnType, DCM_CODE) Dcm_CheckProtocol(uint8 ProtocolCfgId, uint8 ServiceType)
{
    uint8 FindProtocolCfgId;
    uint8 iloop;
    boolean FindProtocolValid = FALSE;
    uint8 OldProtocolPri;
    uint8 NewProtocolPri;
    Std_ReturnType ret = E_OK;
    uint8 FindServiceType;
    uint8 ProtocolMsgNum = Dcm_GetProtocolMsgNum();
    const Dcm_DslCfgType* pDcmDslCfg = DcmPbCfgPtr->pDcmDslCfg;
    const Dcm_DslConnectionType* pDsl_Protocol_ConnectionCfg = DcmPbCfgPtr->DslProtocolConnectionCfg;
#if (PREEMPTION_PROTOCOL_CANCEL_SUPPORT == STD_ON)
    PduIdType DcmTxPduIdx;
    PduIdType DcmRxPduIdx;
#endif
    uint8 MsgCtrlIndexx;
    if (DCM_PROTOCOL_VALID != Dcm_ProtocolCtrl[ProtocolCfgId].Dcm_ProtocolState)
    {
        /*check all protocol whether have one protocol had already started*/
        for (iloop = 0; (iloop < ProtocolMsgNum) && (FindProtocolValid == FALSE); iloop++)
        {
            FindProtocolCfgId = pDsl_Protocol_ConnectionCfg[iloop].DcmDslParentProtocolRowCtrlId;
            MsgCtrlIndexx = Dcm_ProtocolCtrl[FindProtocolCfgId].MsgCtrlIndex;
            FindServiceType = Dcm_ClassifyServiceType(
                pDcmDslCfg->pDcmDslProtocol->pDcmDslProtocolRow[FindProtocolCfgId].DcmDslProtocolID);
            if ((FindProtocolCfgId != ProtocolCfgId)
                && (DCM_PROTOCOL_VALID == Dcm_ProtocolCtrl[FindProtocolCfgId].Dcm_ProtocolState)
                && (FindServiceType == ServiceType))
            {
                FindProtocolValid = TRUE; /*have finded a starting protocol*/
            }
        }
        if (FindProtocolValid == TRUE)
        {
            if (ServiceType == DCM_OBD)
            {
                if (Dcm_OBDMessage.Buffer[0] == 0x3Eu)
                {
                    ret = E_NOT_OK;
                }
                else
                {
                    Dcm_MsgCtrl[ProtocolCfgId].SID = Dcm_OBDMessage.Buffer[0];
                }
            }
            else
            {
                if ((Dcm_MsgCtrl[ProtocolCfgId].MsgContext.pReqData)[0] == 0x3Eu)
                {
                    ret = E_NOT_OK;
                }
                else
                {
                    Dcm_MsgCtrl[ProtocolCfgId].SID = Dcm_MsgCtrl[ProtocolCfgId].MsgContext.pReqData[0];
                }
                OldProtocolPri = Dcm_ProtocolCtrl[FindProtocolCfgId].ProtocolPri;
                NewProtocolPri =
                    (pDcmDslCfg->pDcmDslProtocol->pDcmDslProtocolRow)[ProtocolCfgId].DcmDslProtocolPriority;
                if (E_OK == ret)
                {
                    if (NewProtocolPri < OldProtocolPri)
                    {
                        if (E_OK != DslInternal_ProtocolStop(FindProtocolCfgId))
                        {
                            StopProtocolFaild = TRUE;
                            ret = E_NOT_OK;
                        }
                        else
                        { /* protocol Preemption successful*/
                            if (MsgCtrlIndexx != DCM_INVALID_UINT8)
                            {
#if (PREEMPTION_PROTOCOL_CANCEL_SUPPORT == STD_ON)
                                DcmTxPduIdx = Dcm_MsgCtrl[MsgCtrlIndexx].DcmTxPduId;
                                DcmRxPduIdx = Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.DcmRxPduId;
                                switch (Dcm_MsgCtrl[FindProtocolCfgId].Dcm_MsgState)
                                {
                                case DCM_MSG_TRANSMISSION:
                                    (void)PduR_DcmCancelTransmit(DcmTxPduIdx);
                                    break;
                                case DCM_MSG_RECEIVED:
                                    (void)PduR_DcmCancelReceive(DcmRxPduIdx);
                                    break;
                                default:
                                    /* idle */
                                    break;
                                }
#endif
                                Dcm_CancelPendingService(FindProtocolCfgId);
                                DslInternal_ResetResource(FindProtocolCfgId);
                            }
                        }
                    }
                    else
                    {
                        if (((Dcm_MsgCtrl[FindProtocolCfgId].Dcm_MsgState == DCM_MSG_WAIT)
                             || (Dcm_MsgCtrl[FindProtocolCfgId].Dcm_MsgState == DCM_MSG_CONFIRMATION))
#if (STD_ON == DCM_SESSION_FUNC_ENABLED)
                            && (DCM_DEFAULT_SESSION == Dcm_MkCtrl.Dcm_ActiveSes)
#endif
                        )
                        {
                            if (E_OK != DslInternal_ProtocolStop(FindProtocolCfgId))
                            {
                                StopProtocolFaild = TRUE;
                                ret = E_NOT_OK;
                            }
                        }
                        else
                        {
                            ret = E_NOT_OK;
                        }
                    }
                }
            }
        }
    }
    else
    {
        /*protocol is not already started*/
    }
    return ret;
}

#if (STD_ON == DCM_GENERIC_CONNECTION)
/*************************************************************************/
/*
 * Brief               <Dcm_StartOfReception_metadata>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(BufReq_ReturnType, DCM_CODE)
    Dcm_StartOfReception_metadata(PduIdType id, P2CONST(PduInfoType, AUTOMATIC, DCM_CONST) info)
{
    BufReq_ReturnType bufRet = BUFREQ_OK;
    uint8 MetaDataLen;
    uint8 ConnectionCfgCtrlId;
    uint8 ProtocolCfgCtrlId;
    uint8 MsgCtrlIndex;
    Dcm_DslProtocolRxAddrType RxAddrType;
    const Dcm_DslConnectionType* pDsl_Protocol_ConnectionCfg = DcmPbCfgPtr->DslProtocolConnectionCfg;
    const Dcm_DslProtocolRxType* pDsl_Protocol_Connection_RxCfg = DcmPbCfgPtr->DslProtocolConnectionRxCfg;
    uint8 MetaDataFlag = pDsl_Protocol_Connection_RxCfg[id].DcmDslMetaDataFlag;
    uint16 DcmDslProtocolRxTesterSourceAddr;

    ConnectionCfgCtrlId = pDsl_Protocol_Connection_RxCfg[id].DcmDslParentConnectionCtrlId;
    ProtocolCfgCtrlId = pDsl_Protocol_ConnectionCfg[ConnectionCfgCtrlId].DcmDslParentProtocolRowCtrlId;
    MsgCtrlIndex = ProtocolCfgCtrlId;
    RxAddrType = pDsl_Protocol_Connection_RxCfg[id].DcmDslProtocolRxAddrType;

    MetaDataLen = MetaDataFlag & DCM_METADATA_LENGTH_MASK;
    if ((MetaDataLen > 0u)
        && ((DCM_ADDRESS_EXTENSION_8_MASK == (MetaDataFlag & DCM_ADDRESS_EXTENSION_8_MASK))
            || (DCM_SA16_AND_TA16_MASK == (MetaDataFlag & DCM_SA16_AND_TA16_MASK))))
    {
        SchM_Enter_Dcm_ExclusiveArea();
        Dcm_MsgCtrlType* pMsgCtrl = &Dcm_MsgCtrl[MsgCtrlIndex];
        if (((MetaDataFlag & DCM_SA16_AND_TA16_MASK) == DCM_SA16_AND_TA16_MASK) && (NULL_PTR != info))
        {
            if (DCM_PHYSICAL == RxAddrType)
            {
#if (DCM_CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
                pMsgCtrl->Dcm_MetaData_SA = (uint16)((uint16)info->MetaDataPtr[0u] << 8u) | info->MetaDataPtr[1u];
                pMsgCtrl->Dcm_MetaData_TA = (uint16)((uint16)info->MetaDataPtr[2u] << 8u) | info->MetaDataPtr[3u];
#else
                pMsgCtrl->Dcm_MetaData_SA = (uint16)((uint16)info->MetaDataPtr[1u] << 8u) | info->MetaDataPtr[0u];
                pMsgCtrl->Dcm_MetaData_TA = (uint16)((uint16)info->MetaDataPtr[3u] << 8u) | info->MetaDataPtr[2u];
#endif
            }
            else
            {
#if (DCM_CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
                pMsgCtrl->Dcm_MetaData_SA_Backup =
                    (uint16)((uint16)info->MetaDataPtr[0u] << 8u) | info->MetaDataPtr[1u];
                pMsgCtrl->Dcm_MetaData_TA_Backup =
                    (uint16)((uint16)info->MetaDataPtr[2u] << 8u) | info->MetaDataPtr[3u];
#else
                pMsgCtrl->Dcm_MetaData_SA_Backup =
                    (uint16)((uint16)info->MetaDataPtr[1u] << 8u) | info->MetaDataPtr[0u];
                pMsgCtrl->Dcm_MetaData_TA_Backup =
                    (uint16)((uint16)info->MetaDataPtr[3u] << 8u) | info->MetaDataPtr[2u];
#endif
            }
        }
        SchM_Exit_Dcm_ExclusiveArea();
        DcmDslProtocolRxTesterSourceAddr =
            DcmPbCfgPtr->pDcmDslCfg->pDcmDslProtocol->pDcmDslProtocolRow[ProtocolCfgCtrlId]
                .pDcmDslConnection->pDcmDslMainConnection->DcmDslProtocolRxTesterSourceAddr;
#if (DCM_CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
        DcmDslProtocolRxTesterSourceAddr =
            (DcmDslProtocolRxTesterSourceAddr << 8u) | (DcmDslProtocolRxTesterSourceAddr >> 8u);
#endif
        if (DCM_PHYSICAL == RxAddrType)
        {
            if (pMsgCtrl->Dcm_MetaData_TA != DcmDslProtocolRxTesterSourceAddr)
            {
                bufRet = BUFREQ_E_NOT_OK;
            }
        }
        else
        {
            pMsgCtrl->Dcm_MetaData_TA_Backup = DcmDslProtocolRxTesterSourceAddr;
        }
    }
    return bufRet;
}
#endif

/*************************************************************************/
/*
 * Brief               <Dcm_TpRxIndication_IsFuncAddress>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(Std_ReturnType, DCM_CODE) Dcm_TpRxIndication_IsFuncAddress(PduIdType DcmRxPduId)
{
    uint16 Index;
    uint32 Offset;
    uint8 ProtocolCfgId;
    uint8 ConnectionCfgId;
    uint8 MsgCtrlIndex;
    uint8 RxChannelCfgCtrlId;
    Dcm_DslProtocolRxAddrType DslProtocolRxAddrType;
    Std_ReturnType ret = E_OK;
    const Dcm_DslCfgType* pDcmDslCfg = DcmPbCfgPtr->pDcmDslCfg;
    const Dcm_DslConnectionType* pDsl_Protocol_ConnectionCfg = DcmPbCfgPtr->DslProtocolConnectionCfg;
    const Dcm_DslProtocolRxType* pDsl_Protocol_Connection_RxCfg = DcmPbCfgPtr->DslProtocolConnectionRxCfg;
    Dcm_MsgCtrlType* pDcm_MsgCtrl;

    ConnectionCfgId = pDsl_Protocol_Connection_RxCfg[DcmRxPduId].DcmDslParentConnectionCtrlId;
    ProtocolCfgId = pDsl_Protocol_ConnectionCfg[ConnectionCfgId].DcmDslParentProtocolRowCtrlId;
    MsgCtrlIndex = ProtocolCfgId;
    RxChannelCfgCtrlId = DcmPbCfgPtr->pDcmDslCfg->pDcmDslProtocol->pDcmDslProtocolRow[ProtocolCfgId]
                             .DcmDslProtocolRxBufferRef->Dcm_DslBufferId;
    DslProtocolRxAddrType = pDsl_Protocol_Connection_RxCfg[DcmRxPduId].DcmDslProtocolRxAddrType;
    Offset = (pDcmDslCfg->pDcmChannelCfg)[RxChannelCfgCtrlId].offset;
    pDcm_MsgCtrl = &Dcm_MsgCtrl[MsgCtrlIndex];

    if (DCM_FUNCTIONAL == DslProtocolRxAddrType)
    {
        ret = Dcm_TpRxIndication_3E80(ConnectionCfgId);
        /*(SID=0x3E + subfunction(0x00))||(SID != 0x3E),Need to be submitted to Dsd layer handle*/
        if (E_OK == ret)
        {
            SchM_Enter_Dcm_ExclusiveArea(); /* locked Dcm_Channel, Dcm_FunctionalMessage,Dcm_MsgCtrl*/
            if ((DCM_MSG_WAIT != pDcm_MsgCtrl->Dcm_MsgState)
#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
                || (DCM_PENDING_REQUEST_NONE != Dcm_QueuedRequestCtrl[0u].Dcm_PendingRequestState)
                || (DCM_PENDING_REQUEST_NONE != Dcm_QueuedRequestCtrl[1u].Dcm_PendingRequestState)
#endif
            )
            {
                ret = E_NOT_OK;
                Dcm_FunctionalMessage.Length = 0;
            }
            else
            {
                /*The main channel is idle,copy data to main channel*/
                for (Index = 0u; Index < Dcm_FunctionalMessage.Length; Index++)
                {
                    Dcm_Channel[Offset + Index] = Dcm_FunctionalMessage.Buffer[Index];
                }
                pDcm_MsgCtrl->MsgContext.ReqDataLen = (Dcm_MsgLenType)Dcm_FunctionalMessage.Length;
                Dcm_FunctionalMessage.Length = 0;
                pDcm_MsgCtrl->MsgContext.pReqData = &Dcm_Channel[Offset];
#if (STD_ON == DCM_GENERIC_CONNECTION)
                pDcm_MsgCtrl->Dcm_MetaData_SA = pDcm_MsgCtrl->Dcm_MetaData_SA_Backup;
                pDcm_MsgCtrl->Dcm_MetaData_TA = pDcm_MsgCtrl->Dcm_MetaData_TA_Backup;
                pDcm_MsgCtrl->Dcm_MetaData_SA_Backup = DCM_INVALID_UINT16;
                pDcm_MsgCtrl->Dcm_MetaData_TA_Backup = DCM_INVALID_UINT16;
#endif
            }
            SchM_Exit_Dcm_ExclusiveArea();
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <Dcm_TpRxIndication_SubDeal>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) DslInternal_RxIndicationDeal(uint8 ProtocolCfgCtrlId)
{
    Std_ReturnType ret = E_NOT_OK;
    Dcm_ProtocolType ProtocolId =
        DcmPbCfgPtr->pDcmDslCfg->pDcmDslProtocol->pDcmDslProtocolRow[ProtocolCfgCtrlId].DcmDslProtocolID;
    /*check Protocol*/
    if (E_OK != Dcm_CheckProtocol(ProtocolCfgCtrlId, Dcm_ClassifyServiceType(ProtocolId)))
    {
        SchM_Enter_Dcm_ExclusiveArea();
        if (TRUE == StopProtocolFaild)
        {
            StopProtocolFaild = FALSE;
            DsdInternal_SetNrc(ProtocolCfgCtrlId, DCM_E_CONDITIONSNOTCORRECT);
            DsdInternal_ProcessingDone(ProtocolCfgCtrlId);
        }
        else if (DcmPbCfgPtr->pDcmDslCfg->pDcmDslDiagRespCfg->DcmDslDiagRespOnSecondDeclinedRequest == TRUE)
        {
            Dcm_MsgCtrl[ProtocolCfgCtrlId].NRC = DCM_E_BUSYREPEATREQUEST;
            Dcm_MsgCtrl[ProtocolCfgCtrlId].RspStyle = DCM_NEG_RSP;
            DsdInternal_ProcessingDone(ProtocolCfgCtrlId);
        }
        else
        {
            /* do nothing */
        }
        SchM_Exit_Dcm_ExclusiveArea();
    }
    else
    {
        /*Notice the application to check for new protocol boot environment*/
        ret = DslInternal_ProtocolStart(ProtocolCfgCtrlId);
        if (E_OK != ret)
        {
            DsdInternal_SetNrc(ProtocolCfgCtrlId, DCM_E_CONDITIONSNOTCORRECT);
            DsdInternal_ProcessingDone(ProtocolCfgCtrlId);
        }
        else
        {
            /*Start P2Timer Timer*/
            DslInternal_P2ServerStart(ProtocolCfgCtrlId);
            if (DCM_E_BUSYREPEATREQUEST == Dcm_MsgCtrl[ProtocolCfgCtrlId].NRC)
            {
                DsdInternal_ProcessingDone(ProtocolCfgCtrlId);
            }
            else
            {
                /* save valid Protocol ID number*/
                Dcm_MkCtrl.Dcm_ActiveProtocol = Dcm_ProtocolCtrl[ProtocolCfgCtrlId].ProtocolId;
                /****Notification DSD layer, and reception processing****/
                ret = DsdInternal_RxIndication(ProtocolCfgCtrlId);
            }
        }
    }
    return ret;
}

#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
/*************************************************************************/
/*
 * Brief               <Dcm_CopyTxData_PagebufferDeal>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(BufReq_ReturnType, DCM_CODE) Dcm_CopyTxData_PagebufferDeal(
    uint8 ProtocolCfgId,
    P2CONST(PduInfoType, AUTOMATIC, DCM_CONST) info,
    uint8 TxChannelCtrlIndex,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(PduLengthType, AUTOMATIC, DCM_VAR) availableDataPtr)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    BufReq_ReturnType bufRet;
    const Dcm_DslCfgType* pDcmDslCfg = DcmPbCfgPtr->pDcmDslCfg;
    uint8 Dcm_ChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
    uint32* PageTxSize = &Dcm_PageBufferData.ThisPageTxSize;
    uint32* LastNotTxDataSize = &Dcm_PageBufferData.LastNotTxDataSize;
    /*check the pagebuffer timer whether timeout*/
    if (Dcm_PageBufferData.TimeOut == TRUE)
    {
        DslInternal_InitPageBuffer();
        DslInternal_ResetResource(ProtocolCfgId);
        bufRet = BUFREQ_E_NOT_OK;
    }
    else
    {
        if (Dcm_PageBufferData.Filled == FALSE)
        {
            bufRet = BUFREQ_E_BUSY;
        }
        else
        {
            if ((*PageTxSize + info->SduLength) > Dcm_PageBufferData.ThisPageSize)
            {
                /*last one message not has 8 byte, memory it for next page to send*/
                SchM_Enter_Dcm_ExclusiveArea();
                if ((Dcm_PageBufferData.ThisPageSize - *PageTxSize) != 0u)
                {
                    *LastNotTxDataSize = Dcm_PageBufferData.ThisPageSize - *PageTxSize;
                    Dcm_MemoryCopy(
                        &(Dcm_PageBufferData.pResData[*PageTxSize]),
                        Dcm_PageBufferData.LastNotTxData,
                        *LastNotTxDataSize);
                }
                SchM_Exit_Dcm_ExclusiveArea();
                /*busy should call the api to fill the page*/
                DspInternal_DcmUpdatePage(ProtocolCfgId);
                bufRet = BUFREQ_E_BUSY;
            }
            else
            {
                if (info->SduLength != 0u)
                {
                    SchM_Enter_Dcm_ExclusiveArea();
                    /*Store the response data to the corresponding transmit channel*/
                    Dcm_PageBufferData.pResData =
                        &(Dcm_Channel[pDcmDslCfg->pDcmChannelCfg[Dcm_ChannelCfgIndex].offset]);

                    if (*LastNotTxDataSize != 0u)
                    {
                        /*last one message not has 8 byte, to send it with next page data filled 8
                         * byte*/
                        Dcm_MemoryCopy(Dcm_PageBufferData.LastNotTxData, info->SduDataPtr, *LastNotTxDataSize);
                        Dcm_MemoryCopy(
                            &(Dcm_PageBufferData.pResData[*PageTxSize]),
                            &(info->SduDataPtr[*LastNotTxDataSize]),
                            (info->SduLength - *LastNotTxDataSize));
                        *PageTxSize += (info->SduLength - *LastNotTxDataSize);
                        *LastNotTxDataSize = 0u;
                    }
                    else
                    {
                        Dcm_MemoryCopy(&(Dcm_PageBufferData.pResData[*PageTxSize]), info->SduDataPtr, info->SduLength);
                        *PageTxSize += info->SduLength;
                    }
                    Dcm_PageBufferData.AlreadyPageSize += info->SduLength;
                    SchM_Exit_Dcm_ExclusiveArea();
                }
                *availableDataPtr = (PduLengthType)(Dcm_PageBufferData.TotalSize - Dcm_PageBufferData.AlreadyPageSize);
                bufRet = BUFREQ_OK;
            }
        }
    }
    return bufRet;
}
#endif /* STD_ON == DCM_PAGEDBUFFER_ENABLED */

/*************************************************************************/
/*
 * Brief               <Dcm_CancelPendingService>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(void, DCM_CODE) Dcm_CancelPendingService(uint8 ProtocolCfgId)
{
    uint8 MsgCtrlIndexx = Dcm_ProtocolCtrl[ProtocolCfgId].MsgCtrlIndex;
    if (DCM_PENDING == Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_OpStatus)
    {
        /*there shall call pending service to cancel*/
        P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_DiagnosticServicex)
        (Dcm_OpStatusType OpStatus,
         uint8 ProtocolCtrlId,
         P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
        Dcm_NegativeResponseCodeType errorCode;
        uint16 SidTabCfgIndex;
        uint16 SidTabServiceCfgIndex;

        (void)DsdInternal_SearchSidTabServiceIndex(
            Dcm_MsgCtrl[MsgCtrlIndexx].SID,
            ProtocolCfgId,
            &SidTabCfgIndex,
            &SidTabServiceCfgIndex);
        Dcm_DiagnosticServicex =
            ((DcmPbCfgPtr->pDcmDsdCfg->pDcmDsdServiceTable)[SidTabCfgIndex].pDcmDsdService)[SidTabServiceCfgIndex]
                .DcmDsdSidTabFnc;
        (void)(*Dcm_DiagnosticServicex)(DCM_CANCEL, ProtocolCfgId, &errorCode);
    }
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
