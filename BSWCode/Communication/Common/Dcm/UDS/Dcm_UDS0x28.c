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
             UDS:CommunicationControl (28 hex) service
 ***************************************************************/
#if (STD_ON == DCM_UDS_SERVICE0X28_ENABLED)
/********************************/
/*************************************************************************/
/*
 * Brief               <UDS  0x28 service Get Request Mode>
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
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
static FUNC(Dcm_CommunicationModeType, DCM_CODE) DspInternalUDS0x28GetReqMode_0x01(uint8 subFunc)
{
    Dcm_CommunicationModeType RequestedMode = DCM_ENABLE_RX_TX_NORM;

    switch (subFunc)
    {
    case 0x00u:
        RequestedMode = DCM_ENABLE_RX_TX_NORM;
        break;
    case 0x01u:
        RequestedMode = DCM_ENABLE_RX_DISABLE_TX_NORM;
        break;
    case 0x02u:
        RequestedMode = DCM_DISABLE_RX_ENABLE_TX_NORM;
        break;
    case 0x03u:
        RequestedMode = DCM_DISABLE_RX_TX_NORMAL;
        break;
    default:
        /*idle*/
        break;
    }
    return RequestedMode;
}

/*************************************************************************/
/*
 * Brief               <UDS  0x28 service Get Request Mode>
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
static FUNC(Dcm_CommunicationModeType, DCM_CODE) DspInternalUDS0x28GetReqMode_0x02(uint8 subFunc)
{
    Dcm_CommunicationModeType RequestedMode = DCM_ENABLE_RX_TX_NORM;

    switch (subFunc)
    {
    case 0x00u:
        RequestedMode = DCM_ENABLE_RX_TX_NM;
        break;
    case 0x01u:
        RequestedMode = DCM_ENABLE_RX_DISABLE_TX_NM;
        break;
    case 0x02u:
        RequestedMode = DCM_DISABLE_RX_ENABLE_TX_NM;
        break;
    case 0x03u:
        RequestedMode = DCM_DISABLE_RX_TX_NM;
        break;
    default:
        /*idle*/
        break;
    }
    return RequestedMode;
}

/*************************************************************************/
/*
 * Brief               <UDS  0x28 service Get Request Mode>
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
static FUNC(Dcm_CommunicationModeType, DCM_CODE) DspInternalUDS0x28GetReqMode_0x03(uint8 subFunc)
{
    Dcm_CommunicationModeType RequestedMode = DCM_ENABLE_RX_TX_NORM;

    switch (subFunc)
    {
    case 0x00u:
        RequestedMode = DCM_ENABLE_RX_TX_NORM_NM;
        break;
    case 0x01u:
        RequestedMode = DCM_ENABLE_RX_DISABLE_TX_NORM_NM;
        break;
    case 0x02u:
        RequestedMode = DCM_DISABLE_RX_ENABLE_TX_NORM_NM;
        break;
    case 0x03u:
        RequestedMode = DCM_DISABLE_RX_TX_NORM_NM;
        break;
    default:
        /*idle*/
        break;
    }
    return RequestedMode;
}
/*************************************************************************/
/*
 * Brief               <UDS  0x28 service Get Request Mode>
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
static FUNC(Dcm_CommunicationModeType, DCM_CODE) DspInternalUDS0x28_0x00_0x03_GetReqMode(uint8 subFunc, uint8 comType)
{
    Dcm_CommunicationModeType RequestedMode = DCM_ENABLE_RX_TX_NORM;

    switch (comType)
    {
    case 0x01u:
        RequestedMode = DspInternalUDS0x28GetReqMode_0x01(subFunc);
        break;
    case 0x02u:
        RequestedMode = DspInternalUDS0x28GetReqMode_0x02(subFunc);
        break;
    case 0x03u:
        RequestedMode = DspInternalUDS0x28GetReqMode_0x03(subFunc);
        break;
    default:
        /*idle*/
        break;
    }
    return RequestedMode;
}

/*************************************************************************/
/*
 * Brief               <DspInternalUDS0x28_CheckNewSes>
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
FUNC(void, DCM_CODE) DspInternalUDS0x28_CheckNewSes(Dcm_SesCtrlType NewSes) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint8 iloop;
    Dcm_CommunicationModeType RequestedMode = DCM_ENABLE_RX_TX_NORM_NM;
    Std_ReturnType ret = E_OK;
    uint8 ProtocolCtrlId = Dcm_MkCtrl.Dcm_ActiveProtocolCfgCtrlId;
    uint16 SidTabCfgIndex;
    uint16 SidTabServiceCfgIndex;
    const Dcm_DspComControlType* pDcmDspComControl;
    const Dcm_DspComControlAllChannelType* pDcmDspComControlAllChannel;
    uint8 AllChannelNum;
#if (DCM_COMCONTROL_SUBNODE_ENABLED == STD_ON)
    uint8 SubNodeNum;
    const Dcm_DspComControlSubNodeType* pDcmDspComControlSubNode;
#endif
#if (DCM_COMCONTROL_SPECIFICCHANNEL_ENABLED == STD_ON)
    uint8 SpecificChannelNum;
    const Dcm_DspComControlSpecificChannelType* pDcmDspComControlSpecificChannel;
#endif
    const Dcm_DspCfgType* pDcmDspCfg = DcmPbCfgPtr->pDcmDspCfg;
    const Dcm_DslCfgType* pDcmDslCfg = DcmPbCfgPtr->pDcmDslCfg;
    const Dcm_DslProtocolRowType* pDcmDslProtocolRow = pDcmDslCfg->pDcmDslProtocol->pDcmDslProtocolRow;
    const Dcm_DslMainConnectionType* pDcmDslMainConnection;

    if (DCM_INVALID_UINT8 != ProtocolCtrlId)
    {
        ret = DsdInternal_SearchSidTabServiceIndex(
            SID_COMMUNICATION_CONTROL,
            ProtocolCtrlId,
            &SidTabCfgIndex,
            &SidTabServiceCfgIndex);
        if (E_OK == ret)
        {
            ret = DsdInternal_SesCheck(SidTabCfgIndex, SidTabServiceCfgIndex);
        }
    }
    if (((E_NOT_OK == ret) || (DCM_DEFAULT_SESSION == NewSes)) && ((DCM_INVALID_UINT8 != ProtocolCtrlId))
        && ((NULL_PTR != pDcmDspCfg) && (NULL_PTR != pDcmDspCfg->pDcmDspComControl)))
    {
        pDcmDspComControl = pDcmDspCfg->pDcmDspComControl;
        pDcmDspComControlAllChannel = pDcmDspComControl->DcmDspComControlAllChannel;

        if (NULL_PTR != pDcmDspComControlAllChannel)
        {
            AllChannelNum = pDcmDspComControl->DcmDspComControlAllChannelNum;
            for (iloop = 0; iloop < AllChannelNum; iloop++)
            {
                NetworkHandleType DcmDspComMChannelId = pDcmDspComControlAllChannel[iloop].DcmDspComMChannelId;
                if (TRUE == pDcmDspComControlAllChannel[iloop].DcmDspComControlAllChannelUsed)
                {
#if (DCM_BSWM_ENABLE == STD_ON)
                    BswM_Dcm_CommunicationMode_CurrentState(DcmDspComMChannelId, RequestedMode);
#else
                    Rte_DcmControlCommunicationMode(DcmDspComMChannelId, RequestedMode);
#endif
                }
            }
        }
#if (DCM_MAINCONNECTION_ENABLED == STD_ON)
        if ((NULL_PTR != pDcmDslProtocolRow) && (NULL_PTR != pDcmDslProtocolRow[ProtocolCtrlId].pDcmDslConnection)
            && (NULL_PTR != pDcmDslProtocolRow[ProtocolCtrlId].pDcmDslConnection->pDcmDslMainConnection))
        {
            pDcmDslMainConnection = pDcmDslProtocolRow[ProtocolCtrlId].pDcmDslConnection->pDcmDslMainConnection;
#if (DCM_BSWM_ENABLE == STD_ON)
            BswM_Dcm_CommunicationMode_CurrentState(pDcmDslMainConnection->DcmDslProtocolComMChannelId, RequestedMode);
#else
            Rte_DcmControlCommunicationMode(pDcmDslMainConnection->DcmDslProtocolComMChannelId, RequestedMode);
#endif
        }
#endif
#if (DCM_COMCONTROL_SPECIFICCHANNEL_ENABLED == STD_ON)
        pDcmDspComControlSpecificChannel = pDcmDspComControl->DcmDspComControlSpecificChannel;
        if (NULL_PTR != pDcmDspComControlSpecificChannel)
        {
            SpecificChannelNum = pDcmDspComControl->DcmDspComControlSpecificChannelNum;
            for (iloop = 0; iloop < SpecificChannelNum; iloop++)
            {
                if (TRUE == pDcmDspComControlSpecificChannel[iloop].DcmDspComControlSpecificChannelUsed)
                {
#if (DCM_BSWM_ENABLE == STD_ON)
                    BswM_Dcm_CommunicationMode_CurrentState(
                        pDcmDspComControlSpecificChannel[iloop].DcmDspSpecificComMChannelId,
                        RequestedMode);
#else
                    Rte_DcmControlCommunicationMode(
                        pDcmDspComControlSpecificChannel[iloop].DcmDspSpecificComMChannelId,
                        RequestedMode);
#endif
                }
            }
        }
#endif
#if (DCM_COMCONTROL_SUBNODE_ENABLED == STD_ON)
        pDcmDspComControlSubNode = pDcmDspComControl->DcmDspComControlSubNode;
        if (NULL_PTR != pDcmDspComControlSubNode)
        {
            SubNodeNum = pDcmDspComControl->DcmDspComControlSubNodeNum;
            for (iloop = 0; iloop < SubNodeNum; iloop++)
            {
                if (TRUE == pDcmDspComControlSubNode[iloop].DcmDspComControlSubNodeUsed)
                {
#if (DCM_BSWM_ENABLE == STD_ON)
                    BswM_Dcm_CommunicationMode_CurrentState(
                        pDcmDspComControlSubNode[iloop].DcmDspComMSubNodeChannelId,
                        RequestedMode);
#else
                    Rte_DcmControlCommunicationMode(
                        pDcmDspComControlSubNode[iloop].DcmDspComMSubNodeChannelId,
                        RequestedMode);
#endif
                }
            }
        }
#endif
    }
}

/*************************************************************************/
/*
 * Brief               <UDS  0x28 service Get Request Mode>
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
static FUNC(Std_ReturnType, DCM_CODE)
    DspInternalUDS0x28_0x00_0x03_SubFuction(uint8 ProtocolCtrlId, uint8 subFunc, uint8 comType, uint16 subnet)
{
    uint8 iloop;
    Dcm_CommunicationModeType RequestedMode;

    const Dcm_DspComControlType* pDcmDspComControl;
    const Dcm_DspComControlAllChannelType* pDcmDspComControlAllChannel;
    uint8 AllChannelNum;
#if (DCM_COMCONTROL_SPECIFICCHANNEL_ENABLED == STD_ON)
    uint8 SpecificChannelNum;
    const Dcm_DspComControlSpecificChannelType* pDcmDspComControlSpecificChannel;
#endif
    const Dcm_DspCfgType* pDcmDspCfg = DcmPbCfgPtr->pDcmDspCfg;
    const Dcm_DslCfgType* pDcmDslCfg = DcmPbCfgPtr->pDcmDslCfg;
    const Dcm_DslProtocolRowType* pDcmDslProtocolRow = pDcmDslCfg->pDcmDslProtocol->pDcmDslProtocolRow;
    const Dcm_DslMainConnectionType* pDcmDslMainConnection;
    Std_ReturnType ret = E_NOT_OK;

    RequestedMode = DspInternalUDS0x28_0x00_0x03_GetReqMode(subFunc, comType);
    if ((NULL_PTR != pDcmDspCfg) && (NULL_PTR != pDcmDspCfg->pDcmDspComControl))
    {
        pDcmDspComControl = pDcmDspCfg->pDcmDspComControl;
        pDcmDspComControlAllChannel = pDcmDspComControl->DcmDspComControlAllChannel;
        if ((NULL_PTR != pDcmDspComControlAllChannel) && (subnet == 0u))
        {
            AllChannelNum = pDcmDspComControl->DcmDspComControlAllChannelNum;
            for (iloop = 0; iloop < AllChannelNum; iloop++)
            {
                if (TRUE == pDcmDspComControlAllChannel[iloop].DcmDspComControlAllChannelUsed)
                {
#if (DCM_BSWM_ENABLE == STD_ON)
                    BswM_Dcm_CommunicationMode_CurrentState(
                        pDcmDspComControlAllChannel[iloop].DcmDspComMChannelId,
                        RequestedMode);
#else
                    Rte_DcmControlCommunicationMode(
                        pDcmDspComControlAllChannel[iloop].DcmDspComMChannelId,
                        RequestedMode);
#endif
                    ret = E_OK;
                }
            }
        }
#if (DCM_MAINCONNECTION_ENABLED == STD_ON)
        if ((NULL_PTR != pDcmDslProtocolRow) && (NULL_PTR != pDcmDslProtocolRow[ProtocolCtrlId].pDcmDslConnection)
            && (NULL_PTR != pDcmDslProtocolRow[ProtocolCtrlId].pDcmDslConnection->pDcmDslMainConnection)
            && (subnet == 0x0Fu))
        {
            pDcmDslMainConnection = pDcmDslProtocolRow[ProtocolCtrlId].pDcmDslConnection->pDcmDslMainConnection;
#if (DCM_BSWM_ENABLE == STD_ON)
            BswM_Dcm_CommunicationMode_CurrentState(pDcmDslMainConnection->DcmDslProtocolComMChannelId, RequestedMode);
#else
            Rte_DcmControlCommunicationMode(pDcmDslMainConnection->DcmDslProtocolComMChannelId, RequestedMode);
#endif
            ret = E_OK;
        }
#endif /* DCM_MAINCONNECTION_ENABLED == STD_ON*/
#if (DCM_COMCONTROL_SPECIFICCHANNEL_ENABLED == STD_ON)
        pDcmDspComControlSpecificChannel = pDcmDspComControl->DcmDspComControlSpecificChannel;
        if ((NULL_PTR != pDcmDspComControlSpecificChannel) && (subnet <= 0x0Eu) && (subnet >= 0x01u))
        {
            SpecificChannelNum = pDcmDspComControl->DcmDspComControlSpecificChannelNum;
            for (iloop = 0; iloop < SpecificChannelNum; iloop++)
            {
                if (TRUE == pDcmDspComControlSpecificChannel[iloop].DcmDspComControlSpecificChannelUsed)
                {
                    if (subnet == pDcmDspComControlSpecificChannel[iloop].DcmDspSubnetNumber)
                    {
#if (DCM_BSWM_ENABLE == STD_ON)
                        BswM_Dcm_CommunicationMode_CurrentState(
                            pDcmDspComControlSpecificChannel[iloop].DcmDspSpecificComMChannelId,
                            RequestedMode);
#else
                        Rte_DcmControlCommunicationMode(
                            pDcmDspComControlSpecificChannel[iloop].DcmDspSpecificComMChannelId,
                            RequestedMode);
#endif
                        ret = E_OK;
                    }
                }
            }
        }
#endif /* DCM_COMCONTROL_SPECIFICCHANNEL_ENABLED == STD_ON */
    }
    return ret;
}
#if (DCM_UDS0X28_0x04_0x05_ENABLED == STD_ON)
/*************************************************************************/
/*
 * Brief               <UDS  0x28 service Get Request Mode>
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
static FUNC(Dcm_CommunicationModeType, DCM_CODE) DspInternalUDS0x28_0x04_0x05_GetReqMode(uint8 subFunc, uint8 comType)
{
    Dcm_CommunicationModeType RequestedMode = DCM_ENABLE_RX_TX_NORM;

    if ((subFunc == 0x04u) && (comType == 0x01u))
    {
        RequestedMode = DCM_ENABLE_RX_DISABLE_TX_NORM;
    }
    else if ((subFunc == 0x04u) && (comType == 0x02u))
    {
        RequestedMode = DCM_ENABLE_RX_DISABLE_TX_NM;
    }
    else if ((subFunc == 0x04u) && (comType == 0x03u))
    {
        RequestedMode = DCM_ENABLE_RX_DISABLE_TX_NORM_NM;
    }
    else if ((subFunc == 0x05u) && (comType == 0x01u))
    {
        RequestedMode = DCM_ENABLE_RX_TX_NORM;
    }
    else if ((subFunc == 0x05u) && (comType == 0x02u))
    {
        RequestedMode = DCM_ENABLE_RX_TX_NM;
    }
    else if ((subFunc == 0x05u) && (comType == 0x03u))
    {
        RequestedMode = DCM_ENABLE_RX_TX_NORM_NM;
    }
    else
    {
        /*idle*/
    }
    return RequestedMode;
}

/*************************************************************************/
/*
 * Brief               <UDS  0x28 service Get Request Mode>
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
static FUNC(Std_ReturnType, DCM_CODE)
    DspInternalUDS0x28_0x04_0x05_SubFuction(uint16 nodeIdentification, uint8 subFunc, uint8 comType)
{

    boolean Find = FALSE;
    Std_ReturnType ret = E_OK;
#if (DCM_COMCONTROL_SUBNODE_ENABLED == STD_ON)
    uint8 iloop;
    const Dcm_DspCfgType* pDcmDspCfg = DcmPbCfgPtr->pDcmDspCfg;
    const Dcm_DspComControlType* pDcmDspComControl;
    uint8 SubNodeNum;
    const Dcm_DspComControlSubNodeType* pDcmDspComControlSubNode;
    Dcm_CommunicationModeType RequestedMode;
#endif

#if (DCM_COMCONTROL_SUBNODE_ENABLED == STD_ON)
    if ((NULL_PTR != pDcmDspCfg) && (NULL_PTR != pDcmDspCfg->pDcmDspComControl)
        && (NULL_PTR != pDcmDspCfg->pDcmDspComControl->DcmDspComControlSubNode))
    {
        pDcmDspComControl = pDcmDspCfg->pDcmDspComControl;
        pDcmDspComControlSubNode = pDcmDspCfg->pDcmDspComControl->DcmDspComControlSubNode;
        SubNodeNum = pDcmDspComControl->DcmDspComControlSubNodeNum;

        RequestedMode = DspInternalUDS0x28_0x04_0x05_GetReqMode(subFunc, comType);
        for (iloop = 0; (iloop < SubNodeNum) && (Find == FALSE); iloop++)
        {
            if ((TRUE == pDcmDspComControlSubNode->DcmDspComControlSubNodeUsed)
                && (nodeIdentification == pDcmDspComControlSubNode[iloop].DcmDspComControlSubNodeId))
            {
#if (DCM_BSWM_ENABLE == STD_ON)
                BswM_Dcm_CommunicationMode_CurrentState(
                    DcmPbCfgPtr->pDcmDspCfg->pDcmDspComControl->DcmDspComControlSubNode[iloop]
                        .DcmDspComMSubNodeChannelId,
                    RequestedMode);
#else
                Rte_DcmControlCommunicationMode(
                    DcmPbCfgPtr->pDcmDspCfg->pDcmDspComControl->DcmDspComControlSubNode[iloop]
                        .DcmDspComMSubNodeChannelId,
                    RequestedMode);
#endif
                Find = TRUE;
            }
        }
    }
#else
    DCM_UNUSED(nodeIdentification);
    DCM_UNUSED(subFunc);
    DCM_UNUSED(comType);
#endif
    if (Find == FALSE)
    {
        ret = E_NOT_OK;
    }

    return ret;
}
#endif
/*************************************************************************/
/*
 * Brief               <UDS  0x28 service Get Request Mode>
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
static FUNC(Std_ReturnType, DCM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    Dcm_UDS0x28_ConditionCheck(uint8 ProtocolCtrlId, P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 MsgCtrlId;
    uint8 subFunc;
    uint8 iloop;
    Std_ReturnType ret = E_OK;
    uint8 SidTabId;
    uint16 SidTabIndex;
    uint16 ServiceIndex;
    boolean Flag = FALSE;
    const Dcm_DsdCfgType* pDcmDsdCfg = DcmPbCfgPtr->pDcmDsdCfg;
    uint16 ServiceTable_Num = pDcmDsdCfg->DcmDsdServiceTable_Num;
    const Dcm_DsdServiceTableCfgType* pDcmDsdServiceTable = pDcmDsdCfg->pDcmDsdServiceTable;
    const Dcm_DsdServiceCfgType* pService;
    uint8 ServiceNum;
    uint8 SubService_Num;

    /*if the required protocol is configuted,get the index of runtime datum*/
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    /*get the sub-function */
    subFunc = Dcm_MsgCtrl[MsgCtrlId].Subfunction;
    SidTabId = (DcmPbCfgPtr->pDcmDslCfg->pDcmDslProtocol->pDcmDslProtocolRow)[ProtocolCtrlId].DcmDslServiceTableID;
    /*find SidTabId configuration position in the service configuration table*/
    for (SidTabIndex = 0; (SidTabIndex < ServiceTable_Num) && (FALSE == Flag); SidTabIndex++)
    {
        if (SidTabId == (pDcmDsdServiceTable[SidTabIndex].DcmDsdSidTabId))
        {
            pService = pDcmDsdServiceTable[SidTabIndex].pDcmDsdService;
            ServiceNum = pDcmDsdServiceTable[SidTabIndex].DcmDsdSidTab_ServiceNum;
            for (ServiceIndex = 0; (ServiceIndex < ServiceNum) && (FALSE == Flag); ServiceIndex++)
            {
                if ((0x28u == (pService[ServiceIndex].DcmDsdServiceId))
                    && (pService[ServiceIndex].DcmDsdSubfuncAvial == TRUE))
                {
                    SubService_Num = pService[ServiceIndex].DcmDsdSubService_Num;
                    for (iloop = 0; ((iloop < SubService_Num) && (Flag == FALSE)); iloop++)
                    {
                        if (subFunc == pService[ServiceIndex].DcmDsdSubService[iloop].DcmDsdSubServiceId)
                        {
                            Flag = TRUE;
                        }
                    }
                }
            }
        }
    }
    if ((Flag == FALSE) || (subFunc > 5u))
    {
        /*the required sub-function is not supported,send NRC 0x12*/
        *ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTED;
        ret = E_NOT_OK;
    }
#if (STD_ON == DCM_UDS_SERVICE0X29_ENABLED)
    if (E_OK == ret)
    {
        ret = DspInternal_SubServiceAuthenticationCheck(ProtocolCtrlId, ErrorCode);
    }
#endif
    return ret;
}

/*************************************************************************/
/*
 * Brief               <UDS  0x28 service>
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
/* PRQA S 3432,1532++ */ /* MISRA Rule 20.7,8.7 */
FUNC(Std_ReturnType, DCM_CODE)
Dcm_UDS0x28(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432,1532-- */ /* MISRA Rule 20.7,8.7 */
{
    uint8 MsgCtrlId;
    uint8 TxChannelCtrlIndex;
    uint8 TxChannelCfgIndex;
    uint8 subFunc;
    uint8 comType;
    uint32 Offset;
    Std_ReturnType ret;
#if (DCM_UDS0X28_0x04_0x05_ENABLED == STD_ON)
    uint16 nodeIdentification;
#endif
    Dcm_MsgLenType ReqDataLen;
    const Dcm_DslBufferType* pDcmChannelCfg;
    Dcm_MsgContextType* pMsgContext;
    uint16 Subnet;
    DCM_UNUSED(OpStatus);
    /*if the required protocol is configuted,get the index of runtime datum*/
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    /*get the communicationType*/
    pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
    comType = (pMsgContext->pReqData[2] & 0x0Fu);
    Subnet = (uint8)(pMsgContext->pReqData[2] >> 4u);
    /*get the sub-function */
    subFunc = Dcm_MsgCtrl[MsgCtrlId].Subfunction;
    ReqDataLen = pMsgContext->ReqDataLen;
    ret = Dcm_UDS0x28_ConditionCheck(ProtocolCtrlId, ErrorCode);

#if (STD_ON == DCM_SESSION_FUNC_ENABLED)
    if (E_OK == ret)
    {
        /*session check,check whether the current session supports the request service*/
        ret = DsdInternal_SubSesCheck(ProtocolCtrlId, SID_COMMUNICATION_CONTROL, ErrorCode);
    }
#endif

#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
    if (E_OK == ret)
    {
        /*security check,check whether the current security supports the request service*/
        ret = DsdInternal_SubSecurityCheck(ProtocolCtrlId, SID_COMMUNICATION_CONTROL, ErrorCode);
    }
#endif
    /*check the massage length*/
    if ((E_OK == ret)
        && (((DCM_UDS0X28_REQ_DATA_MAXLENGTH != ReqDataLen) && ((subFunc == 0x04u) || (subFunc == 0x05u)))
            || ((DCM_UDS0X28_REQ_DATA_MINLENGTH != ReqDataLen) && (subFunc < 0x04u))))
    {
        /*the length of massage is not correct,send NRC 0x13*/
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        ret = E_NOT_OK;
    }
    if (E_OK == ret)
    {
        if ((comType < COMTYPE_NORM) || (comType > COMTYPE_NORM_AND_NM))
        {
            /*communicationType is not supported,send NRC 0x31*/
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            ret = E_NOT_OK;
        }
    }
    if (E_OK == ret)
    {
        ret = RTE_PreConditonCheck();
        if (E_OK != ret)
        {
            *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
            ret = E_NOT_OK;
        }
    }
    /*callback function make communicaiton control*/
    if (E_OK == ret)
    {
        if (subFunc < 0x04u)
        {
            if (E_NOT_OK == DspInternalUDS0x28_0x00_0x03_SubFuction(ProtocolCtrlId, subFunc, comType, Subnet))
            {
                *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
                ret = E_NOT_OK;
            }
        }
#if (DCM_UDS0X28_0x04_0x05_ENABLED == STD_ON)
        else if ((subFunc == 0x04u) || (subFunc == 0x05u))
        {
            nodeIdentification =
                (uint16)((uint16)((uint16)pMsgContext->pReqData[3] << 8u) | (uint16)pMsgContext->pReqData[4]);
            if (E_NOT_OK == DspInternalUDS0x28_0x04_0x05_SubFuction(nodeIdentification, subFunc, comType))
            {
                /*NO DcmDspSubnetNumber Finded ,send NRC 0x31*/
                *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
                ret = E_NOT_OK;
            }
        }
#endif
        else
        {
            /*idle*/
        }
    }
    if (E_OK == ret)
    {
        /* assemble positive response*/
        /*the processing is successful,assemble positive response*/
        TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
        TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
        pDcmChannelCfg = &DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex];
        Offset = pDcmChannelCfg->offset;
        /* check tx data length */
        if ((0x02u) > pDcmChannelCfg->Dcm_DslBufferSize)
        {
            /*Pdu length is bigger than buffer size,ignore the request message */
            *ErrorCode = DCM_E_RESPONSETOOLONG;
            ret = E_NOT_OK;
        }
        else
        {
            Dcm_Channel[Offset] = 0x68;         /*response SID*/
            Dcm_Channel[Offset + 1u] = subFunc; /*echo of 0 - 6 bits of sub-function*/
            pMsgContext->ResMaxDataLen = 0x02;
            pMsgContext->ResDataLen = 0x02;
            pMsgContext->pResData = &Dcm_Channel[Offset];
            DsdInternal_ProcessingDone(ProtocolCtrlId);
        }
    }
    return ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
