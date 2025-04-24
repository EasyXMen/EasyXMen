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
#include "Dcm_Internal.h"

/****************************************************************
     UDS: ReadScalingDataByIdentifier (24 hex) service
 ***************************************************************/
#if (STD_ON == DCM_UDS_SERVICE0X24_ENABLED)
/************************/
/*************************************************************************/
/*
 * Brief               <UDS  0x24 service Did Check>
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
static FUNC(Std_ReturnType, DCM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    DspInternalUDS0x24_DidCheck(uint16 receiveDid, P2VAR(uint16, AUTOMATIC, DCM_VAR_NOINIT) pDidCfgIndex)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint16 Index;
    Std_ReturnType ret = E_NOT_OK;
    const Dcm_DspCfgType* pDcmDspCfg = DcmPbCfgPtr->pDcmDspCfg;
    const Dcm_DspDidType* pDcmDspDid = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDid;
    uint16 DidNum = pDcmDspCfg->DcmDspDidNum;
    for (Index = 0; (Index < DidNum) && (E_NOT_OK == ret); Index++)
    {
        /*single did check*/
        if ((receiveDid == pDcmDspDid[Index].DcmDspDidId) && (TRUE == pDcmDspDid[Index].DcmDspDidUsed))
        {
            *pDidCfgIndex = Index;
            ret = E_OK;
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <UDS  0x24 service Service Condition Check>
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
static FUNC(Std_ReturnType, DCM_CODE) Dcm_Uds0x24ServiceConditionCheck(
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint16, AUTOMATIC, DCM_VAR) DidCfgIndex,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;
    uint16 DidInfoCfgIndex;
#if (STD_ON == DCM_DSP_DID_FUNC_ENABLED)
    uint16 RecDid;
#endif
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    const Dcm_DspDidType* pDcmDspDid = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDid;
    const Dcm_MsgContextType* pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;

    /*length check*/
    if (DCM_UDS0X24_REQ_DATA_LENGTH != pMsgContext->ReqDataLen)
    {
        /*the length of message is not correct,send NRC 0x13*/
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        ret = E_NOT_OK;
    }
#if (STD_OFF == DCM_DSP_DID_FUNC_ENABLED)
    if (E_OK == ret)
    {
        /*NRC 0x31:request out of range*/
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        ret = E_NOT_OK;
    }
#else
    if (E_OK == ret)
    {
        RecDid = (uint16)(((uint16)pMsgContext->pReqData[1u]) << 8u) | ((uint16)(pMsgContext->pReqData[2u]));
        /*find the required DID in configuration*/
        ret = DspInternalUDS0x24_DidCheck(RecDid, DidCfgIndex);
        if (E_NOT_OK == ret)
        {
            /*if the required DID is not found in DcmDspDid,send NRC 0x31*/
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        }
    }
    if (E_OK == ret)
    {
        DidInfoCfgIndex = pDcmDspDid[*DidCfgIndex].DcmDspDidInfoIndex;
        /*check the current session*/
#if ((STD_ON == DCM_SESSION_FUNC_ENABLED) && (STD_ON == DCM_DSP_DID_FUNC_ENABLED))
        ret = DsdInternal_DidSessionCheck(DidInfoCfgIndex, ErrorCode);
#endif
    }
#if ((STD_ON == DCM_SECURITY_FUNC_ENABLED) && (STD_ON == DCM_DSP_DID_FUNC_ENABLED))
    /*check the current security level*/
    if (E_OK == ret)
    {
        ret = DsdInternal_DidSecurityCheck(DidInfoCfgIndex, ErrorCode);
    }
#endif
#endif
    return ret;
}

/*************************************************************************/
/*
 * Brief               <UDS  0x24 service>
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
Dcm_UDS0x24(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432,1532-- */ /* MISRA Rule 20.7,8.7 */
{
    uint8 MsgCtrlId;
    Std_ReturnType ret;
    uint8 TxChannelCtrlIndex;
    uint8 TxChannelCfgIndex;
    uint32 Offset;
#if (STD_ON == DCM_DSP_DID_FUNC_ENABLED)
    uint16 DidCfgIndex;
    uint16 RecDid;
    uint16 SignalNum;
    uint16 iloop;
    const Dcm_DspDidSignalType* pDcmDspDidSignal;
    uint32 DidSignalPos = 0;
    uint32 ResOffset;
    uint16 DataSize;
#endif
    const Dcm_DspDidType* pDcmDspDid = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDid;
    const Dcm_DspDataInfoType* pDcmDspDataInfo = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDataInfo;
    Dcm_MsgContextType* pMsgContext;
    const Dcm_DslBufferType* pDcmChannelCfg;
    const Dcm_DspDataType* pDcmDspDidData;

    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;

    RecDid = (uint16)(((uint16)pMsgContext->pReqData[1u]) << 8u) | ((uint16)(pMsgContext->pReqData[2u]));
    /*************************************************/
    ret = Dcm_Uds0x24ServiceConditionCheck(ProtocolCtrlId, &DidCfgIndex, ErrorCode);

#if (STD_ON == DCM_DSP_DID_FUNC_ENABLED)
    if (E_OK == ret)
    {
        TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
        TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
        pDcmChannelCfg = &DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex];
        Offset = pDcmChannelCfg->offset;

        SignalNum = pDcmDspDid[DidCfgIndex].DcmDspDidSignalNum;
        pDcmDspDidSignal = pDcmDspDid[DidCfgIndex].pDcmDspDidSignal;
        pDcmDspDidData = pDcmDspDidSignal->pDcmDspDidData;
        ResOffset = Offset + 3u;
        for (iloop = 0; (iloop < SignalNum) && (E_OK == ret); iloop++)
        {
            if (pDcmDspDidSignal != NULL_PTR)
            {
                DataSize = 0;
                DidSignalPos += (ResOffset + (uint32)pDcmDspDidSignal->DcmDspDidDataPos);
                if (pDcmDspDidData->DcmDspDataGetScalingInfoFnc != NULL_PTR)
                {
                    ret = pDcmDspDidData->DcmDspDataGetScalingInfoFnc(&Dcm_Channel[DidSignalPos], ErrorCode);
                }
                else
                {
                    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
                    ret = E_NOT_OK;
                }
                if (E_OK == ret)
                {
#if (STD_ON == DCM_DATA_TYPE_UINT8_DYN_ENABLED)
                    if (DCM_UINT8_DYN == pDcmDspDidData->DcmDspDataType)
                    {
                        /*DidDataSize is dynamic*/
                        if (NULL_PTR == pDcmDspDidData->DcmDspDataReadDataLengthFnc)
                        {
                            *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
                            ret = E_NOT_OK;
                        }
                        else
                        {
                            (void)pDcmDspDidData->DcmDspDataReadDataLengthFnc(OpStatus, &DataSize);
                        }
                    }
                    else
#else
                    DCM_UNUSED(OpStatus);
#endif
                    {
                        /*DidDataSize is fixed*/
                        if (0xffu != pDcmDspDidData->DcmDspDataInfoIndex)
                        {
                            DataSize = pDcmDspDataInfo[pDcmDspDidData->DcmDspDataInfoIndex].DcmDspDataScalingInfoSize;
                        }
                        else
                        {
                            DataSize = 0;
                        }
                    }
                    ResOffset += DataSize;
                }
            }
            pDcmDspDidSignal++;
        }
    }

    if ((E_OK == ret) && ((ResOffset - Offset) > pDcmChannelCfg->Dcm_DslBufferSize))
    {
        /* check tx data length */
        /*Pdu length is bigger than buffer size */
        *ErrorCode = DCM_E_RESPONSETOOLONG;
        ret = E_NOT_OK;
    }
    if (E_OK == ret)
    {
        /*the processing is successful,assemble positive response*/
        Dcm_Channel[Offset] = 0x64; /*response SID*/
        Dcm_Channel[Offset + 1u] = (uint8)(RecDid >> 8u);
        Dcm_Channel[Offset + 2u] = (uint8)RecDid;
        pMsgContext->ResMaxDataLen = (Dcm_MsgLenType)ResOffset - Offset;
        pMsgContext->ResDataLen = (Dcm_MsgLenType)ResOffset - Offset;
        pMsgContext->pResData = &Dcm_Channel[Offset];
        DsdInternal_ProcessingDone(ProtocolCtrlId);
    }
#endif
    return ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#endif
