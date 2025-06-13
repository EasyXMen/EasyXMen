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
/******************************* references ************************************/
#include "Dcm_Internal.h"

/*******************internal functions****************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/*************************************************************************/
/*
 * Brief               <Find SID corresponding service configuration table index
 *                      and the service index in corresponding service configuration table.>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <Sid: Service Id;
 *                      ProtocolCtrlId: The corresponding protocol control block ID number>
 * Param-Name[out]     <(*pSidTabIndex):Request packet index number
 * in the service configuration table;
 *                      (*pSidTabServieCfgIndex): the service index
 *                      in corresponding service configuration table>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
FUNC(Std_ReturnType, DCM_CODE)
DsdInternal_SearchSidTabServiceIndex(
    uint8 Sid,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint16, AUTOMATIC, DCM_VAR) pSidTabIndex,
    P2VAR(uint16, AUTOMATIC, DCM_VAR) pSidTabServieCfgIndex)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 SidTabId;
    uint8 Service_Num;
    uint16 SidTabIndex;
    uint16 ServiceIndex;
    boolean Flag = FALSE;
    const Dcm_DsdServiceCfgType* pDsdSidTab;
    Std_ReturnType ret = E_OK;
    const Dcm_DslCfgType* pDcmDslCfg = DcmPbCfgPtr->pDcmDslCfg;
    const Dcm_DsdCfgType* pDcmDsdCfg = DcmPbCfgPtr->pDcmDsdCfg;
    uint16 ServiceTable_Num = pDcmDsdCfg->DcmDsdServiceTable_Num;
    const Dcm_DsdServiceTableCfgType* pServiceTable = pDcmDsdCfg->pDcmDsdServiceTable;

    if ((NULL_PTR != pSidTabIndex) && (NULL_PTR != pSidTabServieCfgIndex))
    {
        /*Read the protocol includes the service table ID */
        SidTabId = (pDcmDslCfg->pDcmDslProtocol->pDcmDslProtocolRow)[ProtocolCtrlId].DcmDslServiceTableID;
        /*find SidTabId configuration position in the service configuration table*/
        for (SidTabIndex = 0; (SidTabIndex < ServiceTable_Num) && (FALSE == Flag); SidTabIndex++)
        {
            if (SidTabId == (pServiceTable)[SidTabIndex].DcmDsdSidTabId)
            {
                Flag = TRUE;
            }
        }
        if (FALSE == Flag)
        {
            /*Did not find the service table ID,in the configuration table*/
            ret = E_NOT_OK;
        }
        else
        {
            (*pSidTabIndex) = SidTabIndex - (uint16)1;
        }
        if (E_OK == ret)
        {
            /*Read the first address of the corresponding service table*/
            pDsdSidTab = pServiceTable[SidTabIndex - 1u].pDcmDsdService;
            /*Read the number of services,in the corresponding service table*/
            Service_Num = pServiceTable[SidTabIndex - 1u].DcmDsdSidTab_ServiceNum;
            Flag = FALSE;
            for (ServiceIndex = 0; (ServiceIndex < Service_Num) && (FALSE == Flag); ServiceIndex++)
            {
                if ((Sid == pDsdSidTab[ServiceIndex].DcmDsdServiceId)
                    && (pDsdSidTab[ServiceIndex].DcmDsdServiceUsed == TRUE))
                {
                    Flag = TRUE;
                }
            }
            if (FALSE == Flag)
            {
                /*Did not find the service ID,in the corresponding service table*/
                ret = E_NOT_OK;
            }
            else
            {
                (*pSidTabServieCfgIndex) = (ServiceIndex - (uint16)1);
            }
        }
    }
    else
    {
        ret = E_NOT_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <DsdInternal_SearchSidTabSubServiceIndex>
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
FUNC(Std_ReturnType, DCM_CODE)
DsdInternal_SearchSidTabSubServiceIndex(
    uint8 ProtocolCtrlId,
    uint16 SidTabIndex,
    uint16 ServiceIndex,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint16, AUTOMATIC, DCM_VAR) pSubServieCfgIndex)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 MsgCtrlId;
    uint8 SubFunctionId;
    uint8 SubService_Num;
    uint16 SubServiceIndex;
    boolean Flag = FALSE;
    Std_ReturnType ret = E_OK;
    const Dcm_DsdServiceCfgType* pService;
    const Dcm_DsdServiceTableCfgType* pServiceTable = &(DcmPbCfgPtr->pDcmDsdCfg->pDcmDsdServiceTable)[SidTabIndex];

    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    SubFunctionId = Dcm_MsgCtrl[MsgCtrlId].Subfunction;
    pService = &((pServiceTable->pDcmDsdService)[ServiceIndex]);
    SubService_Num = pService->DcmDsdSubService_Num;
    if ((SubService_Num > 0u) && (TRUE == pService->DcmDsdSubfuncAvial))
    {
        for (SubServiceIndex = 0; (SubServiceIndex < SubService_Num) && (FALSE == Flag); SubServiceIndex++)
        {
            const Dcm_DsdSubServiceCfgType* pSubService = &(pService->DcmDsdSubService[SubServiceIndex]);
            if ((SubFunctionId == pSubService->DcmDsdSubServiceId) && (TRUE == pSubService->DcmDsdSubServiceUsed))
            {
                Flag = TRUE;
            }
        }
        if (FALSE == Flag)
        {
            ret = E_NOT_OK;
        }
        (*pSubServieCfgIndex) = SubServiceIndex - (uint16)1;
    }
    else
    {
        (*pSubServieCfgIndex) = DCM_INVALID_UINT16;
        ret = E_NOT_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <Save a positive response prohibit information>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <Sid: Service Id;
 *                      ProtocolCtrlId:The corresponding protocol control block ID number;>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(void, DCM_CODE)
    DsdInternal_SuppressPosRsp(uint8 ProtocolCtrlId, uint16 SidTabIndex, uint16 SidTabServieCfgIndex)
{
    uint8 MsgCtrlIndexx;
    uint8 SubFunction;
    const Dcm_DsdServiceTableCfgType* pServiceTable = &(DcmPbCfgPtr->pDcmDsdCfg->pDcmDsdServiceTable)[SidTabIndex];
    const Dcm_DsdServiceCfgType* pService = &(pServiceTable->pDcmDsdService[SidTabServieCfgIndex]);

    MsgCtrlIndexx = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    if (TRUE == pService->DcmDsdSubfuncAvial)
    {
        SubFunction = Dcm_MsgCtrl[MsgCtrlIndexx].Subfunction;
        SchM_Enter_Dcm_ExclusiveArea();
        Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.MsgAddInfo.SuppressPosResponse = FALSE;
        if ((0x80u == (SubFunction & 0x80u)) && (TRUE == pService->DcmDsdSuppressPosRsp))
        {
            Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.MsgAddInfo.SuppressPosResponse = TRUE;
            /*Set response type "DCM_POS_RSP_SUPPRESS"*/
            Dcm_MsgCtrl[MsgCtrlIndexx].RspStyle = DCM_POS_RSP_SUPPRESS;
            /*Clear positive response Prohibition information*/
            (Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.pReqData)[1] = (SubFunction & 0x7Fu);
            Dcm_MsgCtrl[MsgCtrlIndexx].Subfunction = (SubFunction & 0x7Fu);
        }
        SchM_Exit_Dcm_ExclusiveArea();
    }
}

/*************************************************************************/
/*
 * Brief               <Check request packet addressing type whether is correct.>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:The corresponding protocol control block ID number;
 *                      Sid: Service Id;>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(Std_ReturnType, DCM_CODE)
    DsdInternal_AddressingFormatCheck(uint8 ProtocolCtrlId, uint16 SidTabCfgIndex, uint16 SidTabServiceCfgIndex)
{
    uint8 MsgCtrlIndexx;
    uint8 RxServiceAddressingFormat;
    Dcm_DslProtocolRxAddrType CfgServiceAddressingFormat;
    Std_ReturnType ret = E_OK;
    const Dcm_DsdServiceTableCfgType* pServiceTable = &(DcmPbCfgPtr->pDcmDsdCfg->pDcmDsdServiceTable)[SidTabCfgIndex];
    const Dcm_DsdServiceCfgType* pService = &(pServiceTable->pDcmDsdService[SidTabServiceCfgIndex]);

    /*Read the service configuration support addressing*/
    CfgServiceAddressingFormat = pService->AddressingFormat;
    if (DCM_PHYANDFUNC != CfgServiceAddressingFormat)
    {
        /*Read request packet addressing type*/
        MsgCtrlIndexx = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
        RxServiceAddressingFormat = Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.MsgAddInfo.ReqType;
        if ((uint8)CfgServiceAddressingFormat != RxServiceAddressingFormat)
        {
            /*Configure addressing type is different from the actual addressing type*/
            ret = E_NOT_OK;
        }
    }
    /*else The service supports functional addressing and physical addressing*/
    return ret;
}

/*******************public functions******************************/
/*************************************************************************/
/*
 * Brief               <The diagnostic request processing in DSD layer>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:The corresponding protocol control block ID number;>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(Std_ReturnType, DCM_CODE) DsdInternal_RxIndication_Check(uint8 ProtocolCtrlId)
{
    uint8 Sid;
    uint8 MsgCtrlId;
    Dcm_ProtocolStateType ProtocolState;
    Std_ReturnType ret = E_OK;
    uint16 SidTabCfgIndex;
    uint16 SidTabServiceCfgIndex;

    ProtocolState = Dcm_ProtocolCtrl[ProtocolCtrlId].Dcm_ProtocolState;
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    Sid = Dcm_MsgCtrl[MsgCtrlId].SID;

    if (DCM_PROTOCOL_VALID != ProtocolState)
    {
        /*xxx_startProtocol in DslInternal_ProtocolStart not return all ok */
        DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_CONDITIONSNOTCORRECT);
        DsdInternal_ProcessingDone(ProtocolCtrlId);
        ret = E_NOT_OK;
    }
/*SID range checks*/
#if (STD_OFF == DCM_RESPOND_ALL_REQUEST)
    if ((E_OK == ret) && (((Sid >= 0x40u) && (Sid <= 0x7Fu)) || (Sid >= 0xC0u)))
    {
        /*(0x40=<SID<=0x7F)||(0xC0=<SID<=0xFF)*/
        DslInternal_ResetResource(ProtocolCtrlId);
        ret = E_NOT_OK;
    }
#endif
    if (E_OK == ret)
    {
        /*Check whether the SID is configured in the corresponding service table*/
        ret = DsdInternal_SearchSidTabServiceIndex(Sid, ProtocolCtrlId, &SidTabCfgIndex, &SidTabServiceCfgIndex);
        if (E_NOT_OK == ret)
        {
            /*Set Nrc=0x11(service not supported )*/
            DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_SERVICENOTSUPPORTED);
            DsdInternal_ProcessingDone(ProtocolCtrlId);
        }
    }
    if (E_OK == ret)
    {
        /*Check whether received packets addressing modes support*/
        ret = DsdInternal_AddressingFormatCheck(ProtocolCtrlId, SidTabCfgIndex, SidTabServiceCfgIndex);
        if (E_NOT_OK == ret)
        {
            /*Set Nrc=0x11(service not supported )*/
            DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_SERVICENOTSUPPORTED);
            DsdInternal_ProcessingDone(ProtocolCtrlId);
        }
        else
        {
            /*positive response  Suppress bit processing*/
            DsdInternal_SuppressPosRsp(ProtocolCtrlId, SidTabCfgIndex, SidTabServiceCfgIndex);
        }
    }
#if (STD_ON == DCM_UDS_SERVICE0X29_ENABLED)
    /*SWS_Dcm_01558 Skipping authentication check for tester present
    The Dcm shall process the UDS service 0x3E (TesterPresent) independently
    from the current authentication state.*/
    if ((E_OK == ret) && (SID_AUTHENTICATION != Sid) && (SID_TESTER_PRESENT != Sid) && (Sid >= 0x10u)
        && (NULL_PTR != DcmPbCfgPtr->pDcmDspCfg->Dcm_DspAuthentication))
    {
        /*check Authentication*/
        ret = DsdInternal_AuthenticationCheck(ProtocolCtrlId, SidTabCfgIndex, SidTabServiceCfgIndex);
        if (E_NOT_OK == ret)
        {
            /*Set Nrc=0x34*/
            DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_AUTHENTICATIONREQUIRED);
            DsdInternal_ProcessingDone(ProtocolCtrlId);
        }
    }
#endif
#if (STD_ON == DCM_SESSION_FUNC_ENABLED)
    if (E_OK == ret)
    {
        /*session check,check whether the current session supports the request service*/
        ret = DsdInternal_SesCheck(SidTabCfgIndex, SidTabServiceCfgIndex);
        if (E_NOT_OK == ret)
        {
            /*the current session does not support the request service,send NRC = 0x7F*/
            DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION);
            DsdInternal_ProcessingDone(ProtocolCtrlId);
        }
    }
#endif
#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
    if (E_OK == ret)
    {
        /*security check,check whether the current security supports the request service*/
        ret = DsdInternal_SecurityCheck(SidTabCfgIndex, SidTabServiceCfgIndex);
        if (E_NOT_OK == ret)
        {
            DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_SECURITYACCESSDENIED); /*NRC = 0x33*/
            DsdInternal_ProcessingDone(ProtocolCtrlId);
        }
    }
#endif
    if ((E_OK == ret) && (Sid >= 0x10u) && (Sid != 0x31u)
        && (TRUE
            == DcmPbCfgPtr->pDcmDsdCfg->pDcmDsdServiceTable[SidTabCfgIndex]
                   .pDcmDsdService[SidTabServiceCfgIndex]
                   .DcmDsdSubfuncAvial))
    {
        if (DCM_UDS_REQ_DATA_MINLENGTH > Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
        {
            /*the min length of massage is not correct,send NRC 0x13*/
            DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
            DsdInternal_ProcessingDone(ProtocolCtrlId);
            ret = E_NOT_OK;
        }
        if (E_OK == ret)
        {
            uint16 SubServieCfgIndex;
            ret = DsdInternal_SearchSidTabSubServiceIndex(
                ProtocolCtrlId,
                SidTabCfgIndex,
                SidTabServiceCfgIndex,
                &SubServieCfgIndex);
            if (E_NOT_OK == ret)
            {
                /* deactivate the usage of a subservice or no support subservice ，send NRC 0x12 */
                DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_SUBFUNCTIONNOTSUPPORTED);
                DsdInternal_ProcessingDone(ProtocolCtrlId);
            }
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <The diagnostic request processing in DSD layer>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:The corresponding protocol control block ID number;>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) DsdInternal_RxIndication(uint8 ProtocolCtrlId) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret;

    /*The error message status, reset message processing*/
/*Application environment and permissions verification*/
#if (STD_ON == DCM_DSD_REQUEST_MANUFACTURER_NOTIFICATION_ENABLED)
    ret = DsdInternal_ManufacturerIndication(ProtocolCtrlId);
    if (E_OK == ret)
#endif
    {
        ret = DsdInternal_RxIndication_Check(ProtocolCtrlId);
    }

#if (STD_ON == DCM_DSD_REQUEST_SUPPLIER_NOTIFICATION_ENABLED)
    if (E_OK == ret)
    {
        ret = DsdInternal_SupplierIndication(ProtocolCtrlId);
    }
#endif
    return ret;
}

#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
/*************************************************************************/
/*
 * Brief               <DsdInternal_ProcessPage>
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
FUNC(void, DCM_CODE) DsdInternal_ProcessPage(uint8 ProtocolCtrlId)
{
    PduInfoType PduInfo;
    PduIdType DcmTxPduIdx;
    uint8 MsgCtrlIndexx;
#if (STD_ON == DCM_GENERIC_CONNECTION)
    PduIdType DcmRxPduIdx;

    PduInfo.MetaDataPtr = &(Dcm_Metadata[ProtocolCtrlId][0]);
#endif
#if (STD_ON == DCM_GENERIC_CONNECTION)
    const Dcm_DslProtocolRxType* pDsl_Protocol_Connection_RxCfg = DcmPbCfgPtr->DslProtocolConnectionRxCfg;
#endif
    uint8 DcmDslMetaDataFlag;
    /*check the pagebuffer timer whether timeout*/
    if (Dcm_PageBufferData.TimeOut != TRUE)
    {
        /*DSP requests transmission of filled page*/
        MsgCtrlIndexx = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
        DcmTxPduIdx = Dcm_MsgCtrl[MsgCtrlIndexx].DcmTxPduId;
#if (STD_ON == DCM_GENERIC_CONNECTION)
        DcmRxPduIdx = Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.DcmRxPduId;
        DcmDslMetaDataFlag = pDsl_Protocol_Connection_RxCfg[DcmRxPduIdx].DcmDslMetaDataFlag;
        if ((DcmDslMetaDataFlag & DCM_METADATA_LENGTH_MASK) > 0u)
        {
            if ((DcmDslMetaDataFlag & DCM_SA16_AND_TA16_MASK) == DCM_SA16_AND_TA16_MASK)
            {
#if (DCM_CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
                PduInfo.MetaDataPtr[0u] = (uint8)((Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_MetaData_TA & 0xFF00u) >> 8u);
                PduInfo.MetaDataPtr[1u] = (uint8)(Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_MetaData_TA & 0xFFu);
                PduInfo.MetaDataPtr[2u] = (uint8)((Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_MetaData_SA & 0xFF00u) >> 8u);
                PduInfo.MetaDataPtr[3u] = (uint8)(Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_MetaData_SA & 0xFFu);
#else
                PduInfo.MetaDataPtr[0u] = (uint8)(Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_MetaData_TA & 0xFFu);
                PduInfo.MetaDataPtr[1u] = (uint8)((Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_MetaData_TA & 0xFF00u) >> 8u);
                PduInfo.MetaDataPtr[2u] = (uint8)(Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_MetaData_SA & 0xFFu);
                PduInfo.MetaDataPtr[3u] = (uint8)((Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_MetaData_SA & 0xFF00u) >> 8u);
#endif
            }
        }
#else
        PduInfo.MetaDataPtr = NULL_PTR;
#endif
        /*total length to be transmit of all pages*/
        PduInfo.SduLength = (PduLengthType)Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.ResMaxDataLen;
        (void)PduR_DcmTransmit(DcmTxPduIdx, &PduInfo);
        /****Close P2Server Timer****/
        DslInternal_P2ServerStop(ProtocolCtrlId);
    }
    return;
}

/*************************************************************************/
/*
 * Brief               <DsdInternal_StartPagedProcessing>
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
FUNC(void, DCM_CODE) DsdInternal_StartPagedProcessing(uint8 ProtocolCtrlId)
{
    /****Close P2Server Timer****/
    DslInternal_P2ServerStop(ProtocolCtrlId);
    SchM_Enter_Dcm_ExclusiveArea();
    Dcm_PageBufferData.PageIndex = 0;
    Dcm_PageBufferData.Filled = FALSE;
    SchM_Exit_Dcm_ExclusiveArea();
    DspInternal_DcmUpdatePage(ProtocolCtrlId);
}
#endif
/*************************************************************************/
/*
 * Brief               <Response to a diagnosis request processing>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:The corresponding protocol control block ID number;>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
FUNC(void, DCM_CODE) DsdInternal_ProcessingDone(uint8 ProtocolCtrlId)
{
    uint8 Nrc;
    uint8 MsgCtrlIndexx;
    uint8 TxChannelCtrlIndex;
    uint8 TxChannelCfgIndex;
    uint32 Offset;
    Dcm_RspType RspStylex;
    uint8 ReqTypex;
    /*Send NRC = 0x78 channel (Note: The channel length range: 8 ~ 4095)*/
    static VAR(uint8, DCM_VAR_POWER_ON_INIT) Dcm_Nrc78Channel[8] = {0};
    const Dcm_DslCfgType* pDcmDslCfg = DcmPbCfgPtr->pDcmDslCfg;

    MsgCtrlIndexx = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    /*Set message status "DCM_MSG_TRANSMISSION "*/
    Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_MsgState = DCM_MSG_TRANSMISSION;
    TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_TxCtrlChannelIndex;
    RspStylex = Dcm_MsgCtrl[MsgCtrlIndexx].RspStyle;
    switch (RspStylex)
    {
    case DCM_POS_RSP: /*--------------positive response---------------*/
        TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
        Offset = (pDcmDslCfg->pDcmChannelCfg)[TxChannelCfgIndex].offset;
        /*Store the response data to the corresponding transmit channel*/
        Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.pResData = &Dcm_Channel[Offset];
        DslInternal_ProcessingDone(ProtocolCtrlId);
        break;
    case DCM_NEG_RSP: /*---------------Negative response---------------*/
        /* Diagnosis  request type */
        ReqTypex = Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.MsgAddInfo.ReqType;
        Nrc = Dcm_MsgCtrl[MsgCtrlIndexx].NRC;
        if (((uint8)DCM_FUNCTIONAL == ReqTypex)
            && ((DCM_E_SERVICENOTSUPPORTED == Nrc) || (DCM_E_SUBFUNCTIONNOTSUPPORTED == Nrc)
                || (DCM_E_REQUESTOUTOFRANGE == Nrc) || (DCM_E_SUBFUNCTIONNOTSUPPORTEDINACTIVESESSION == Nrc)
                || (DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION == Nrc)))
        {
            DslInternal_ResetResource(ProtocolCtrlId);
        }
        else if (DCM_E_RESPONSE_PENDING == Nrc)
        {
            /*NRC=0x78*/
            Dcm_Nrc78Channel[0] = 0x7Fu;
            Dcm_Nrc78Channel[1] = Dcm_MsgCtrl[MsgCtrlIndexx].SID;
            Dcm_Nrc78Channel[2] = DCM_E_RESPONSE_PENDING;
            SchM_Enter_Dcm_ExclusiveArea();
            Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.pResData = &Dcm_Nrc78Channel[0];
            Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.ResDataLen = 3u;
            Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.ResMaxDataLen = 3u;
            SchM_Exit_Dcm_ExclusiveArea();
            DslInternal_ProcessingDone(ProtocolCtrlId);
        }
        else
        {
            /*Other Nrc code*/
            TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
            Offset = (pDcmDslCfg->pDcmChannelCfg)[TxChannelCfgIndex].offset;
            SchM_Enter_Dcm_ExclusiveArea();
            Dcm_Channel[Offset] = 0x7Fu;
            Dcm_Channel[Offset + 1u] = Dcm_MsgCtrl[MsgCtrlIndexx].SID;
            Dcm_Channel[Offset + 2u] = (uint8)(Dcm_MsgCtrl[MsgCtrlIndexx].NRC);
#if (STD_ON == DCM_NRC22_SPECIFIC_CAUSE_CODE)
            if (0x22u == Dcm_MsgCtrl[MsgCtrlIndexx].NRC)
            {
                Dcm_SpecificCauseCodeType Code;
                if (E_OK == Dcm_GetSpecificCauseCode(&Code))
                {
                    Dcm_Channel[Offset + 3u] = Code;
                }
                else
                {
                    Dcm_Channel[Offset + 3u] = 0xFFu;
                }
                Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.ResDataLen = 4u;
                Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.ResMaxDataLen = 4u;
            }
            else
#endif /* STD_ON == DCM_NRC22_SPECIFIC_CAUSE_CODE */
            {
                Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.ResDataLen = 3u;
                Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.ResMaxDataLen = 3u;
            }
            Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.pResData = &Dcm_Channel[Offset];
            SchM_Exit_Dcm_ExclusiveArea();
            DslInternal_ProcessingDone(ProtocolCtrlId);
        }
        break;
    case DCM_POS_RSP_SUPPRESS: /*-----positive response suppress---------------*/
        DsdInternal_ManufacturerConfirmation(ProtocolCtrlId);
        DsdInternal_SupplierConfirmation(ProtocolCtrlId);
        DspInternal_DcmConfirmation(ProtocolCtrlId);
        break;
    default:
        DslInternal_ResetResource(ProtocolCtrlId);
        break;
    }
    return;
}

/*************************************************************************/
/*
 * Brief               <Notify the DSD layer, respond to the confirmation>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:The corresponding protocol control block ID number;>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
FUNC(void, DCM_CODE) DsdInternal_TxConfirmation(uint8 ProtocolCtrlId) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    DsdInternal_ManufacturerConfirmation(ProtocolCtrlId);
    DsdInternal_SupplierConfirmation(ProtocolCtrlId);
    DspInternal_DcmConfirmation(ProtocolCtrlId);
}

/*************************************************************************/
/*
 * Brief               <DsdInternal_ServiceAddressAndLengthFormatIdentifierCheck>
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
FUNC(Std_ReturnType, DCM_CODE)
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
DsdInternal_ServiceAddressAndLengthFormatIdentifierCheck(const uint8 addressAndLengthFormatIdentifier)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    uint8 iloop;
    const Dcm_DspMemoryType* pDcmDspMemory = DcmPbCfgPtr->pDcmDspCfg->pDcmDspMemory;
    uint8 Num = pDcmDspMemory->DcmDspAddressAndLengthFormatIdentifierNum;

    for (iloop = 0; (iloop < Num) && (ret == E_NOT_OK); iloop++)
    {
        if (addressAndLengthFormatIdentifier
            == pDcmDspMemory->DcmDspAddressAndLengthFormatIdentifier[iloop]
                   .DcmDspSupportedAddressAndLengthFormatIdentifier)
        {
            ret = E_OK;
        }
    }
    if (pDcmDspMemory->DcmDspAddressAndLengthFormatIdentifierNum == 0u)
    {
        ret = E_OK;
    }
    return ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
