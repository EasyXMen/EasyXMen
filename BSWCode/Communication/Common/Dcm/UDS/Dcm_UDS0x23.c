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
     UDS: ReadMemoryByAddress (23 hex) service
 ***************************************************************/
#if (STD_ON == DCM_UDS_SERVICE0X23_ENABLED)
typedef struct
{
    uint32 MemoryAddress;
    uint32 Memorysize;
} Dcm_0x23Types;
/******************************/
/*************************************************************************/
/*
 * Brief               <UDS  0x23 service Memory Read Security Check>
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
static FUNC(Std_ReturnType, DCM_CODE) Dcm_0x23MemoryReadSecurityCheck(
    uint8 MemoryIdInfoIndex,
    uint8 MemoryRangeInfoIndex,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 index;
    Std_ReturnType ret = E_NOT_OK;
    const Dcm_DspCfgType* pDcmDspCfg = DcmPbCfgPtr->pDcmDspCfg;
    const Dcm_DspReadMemoryRangeInfoType* pDcmDspReadMemoryRangeInfo =
        &(pDcmDspCfg->pDcmDspMemory->DcmDspMemoryIdInfo[MemoryIdInfoIndex]
              .DcmDspReadMemoryRangeInfo[MemoryRangeInfoIndex]);
    uint8 SecRefNum = pDcmDspReadMemoryRangeInfo->DcmDspReadMemorySecurityLevelRefNum;
    const uint8* pSecurityLevelRow = pDcmDspReadMemoryRangeInfo->pDcmDspReadMemorySecurityLevelRow;
    if (SecRefNum != 0u)
    {
        for (index = 0; (index < SecRefNum); index++)
        {
            if (Dcm_MkCtrl.Dcm_ActiveSec == pSecurityLevelRow[index])
            {
                ret = E_OK;
            }
        }
    }
    else
    {
        ret = E_OK;
    }
    if (E_NOT_OK == ret)
    {
        /*security check not ok for requested memory interval,send NRC 0x33*/
        *ErrorCode = DCM_E_SECURITYACCESSDENIED;
        ret = E_NOT_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <UDS  0x23 service Memory Read Session Check>
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
static FUNC(Std_ReturnType, DCM_CODE) Dcm_0x23MemoryReadSessionCheck(
    uint8 MemoryIdInfoIndex,
    uint8 MemoryRangeInfoIndex,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 index;
    Std_ReturnType ret = E_NOT_OK;
    const Dcm_DspCfgType* pDcmDspCfg = DcmPbCfgPtr->pDcmDspCfg;
    const Dcm_DspReadMemoryRangeInfoType* pDcmDspReadMemoryRangeInfo =
        &(pDcmDspCfg->pDcmDspMemory->DcmDspMemoryIdInfo[MemoryIdInfoIndex]
              .DcmDspReadMemoryRangeInfo[MemoryRangeInfoIndex]);
    uint8 SecRefNum = pDcmDspReadMemoryRangeInfo->DcmDspReadMemorySecurityLevelRefNum;
    const uint8* pSecurityLevelRow = pDcmDspReadMemoryRangeInfo->pDcmDspReadMemorySecurityLevelRow;

    if (SecRefNum != 0u)
    {
        for (index = 0; (index < SecRefNum); index++)
        {
            if (Dcm_MkCtrl.Dcm_ActiveSes == pSecurityLevelRow[index])
            {
                ret = E_OK;
            }
        }
    }
    else
    {
        ret = E_OK;
    }
    if (E_NOT_OK == ret)
    {
        /*session check not ok for requested memory interval,send NRC 0x31*/
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <UDS  0x23 service Memory Read Range Check>
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
static FUNC(Std_ReturnType, DCM_CODE) Dcm_0x23ReadMemoryRangeCheck(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2CONST(Dcm_0x23Types, AUTOMATIC, DCM_VAR) Dcm_0x23Type,
    P2VAR(uint8, AUTOMATIC, DCM_VAR) MemoryIdInfoIndex,
    P2VAR(uint8, AUTOMATIC, DCM_VAR) MemoryRangeInfoIndex,
    P2VAR(uint8, AUTOMATIC, DCM_VAR) MemoryAddressMatchNum)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 IdInfoIndex;
    uint8 RangeInfoIndex;
    Std_ReturnType ret = E_NOT_OK;
    const Dcm_DspReadMemoryRangeInfoType* pDcmDspReadMemoryRangeInfo;
    const Dcm_DspMemoryIdInfoType* pDcmDspMemoryIdInfo;
    const Dcm_DspMemoryType* pDcmDspMemory = DcmPbCfgPtr->pDcmDspCfg->pDcmDspMemory;
    uint8 MemoryIdInfoNum = pDcmDspMemory->DcmDspMemoryIdInfoNum;
    uint8 MemoryRangeInfoNum;

    for (IdInfoIndex = 0u; IdInfoIndex < MemoryIdInfoNum; IdInfoIndex++)
    {
        pDcmDspMemoryIdInfo = &(pDcmDspMemory->DcmDspMemoryIdInfo[IdInfoIndex]);
        MemoryRangeInfoNum = pDcmDspMemoryIdInfo->DcmDspReadMemoryRangeInfoNum;
        if (pDcmDspMemoryIdInfo != NULL_PTR)
        {
            for (RangeInfoIndex = 0; RangeInfoIndex < MemoryRangeInfoNum; RangeInfoIndex++)
            {
                pDcmDspReadMemoryRangeInfo = &(pDcmDspMemoryIdInfo->DcmDspReadMemoryRangeInfo[RangeInfoIndex]);
                if (pDcmDspReadMemoryRangeInfo != NULL_PTR)
                {
                    if ((pDcmDspReadMemoryRangeInfo->DcmDspReadMemoryRangeLow <= Dcm_0x23Type->MemoryAddress)
                        && (pDcmDspReadMemoryRangeInfo->DcmDspReadMemoryRangeHigh
                            >= (Dcm_0x23Type->MemoryAddress + Dcm_0x23Type->Memorysize - 1u)))
                    {
                        *MemoryIdInfoIndex = IdInfoIndex;
                        *MemoryRangeInfoIndex = RangeInfoIndex;
                        *MemoryAddressMatchNum += 1u;
                        ret = E_OK;
                    }
                }
            }
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <UDS  0x23 service Memory Read Condition Check>
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
static FUNC(Std_ReturnType, DCM_CODE) Dcm_Uds0x23ServiceConditionCheck(
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;
    uint8 MsgCtrlId;
    uint8 AddressAndLengthFormatIdentifier;
    uint8 MemoryAddressSize;
    uint8 MemoryLengthSize;
    const Dcm_MsgContextType* pMsgContext;

    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
    /*min-length check*/
    if (DCM_UDS0X23_REQ_DATA_MINLENGTH > pMsgContext->ReqDataLen)
    {
        /*the min length of message is not correct,send NRC 0x13*/
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        ret = E_NOT_OK;
    }
    if (E_OK == ret)
    {
        AddressAndLengthFormatIdentifier = pMsgContext->pReqData[1];
        ret = DsdInternal_ServiceAddressAndLengthFormatIdentifierCheck(AddressAndLengthFormatIdentifier);
        if (E_NOT_OK == ret)
        {
            /*the processing is not successful,send NRC 0x31 */
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            ret = E_NOT_OK;
        }
    }
    if (E_OK == ret)
    {
        MemoryAddressSize = AddressAndLengthFormatIdentifier & 0x0Fu;
        MemoryLengthSize = (AddressAndLengthFormatIdentifier & 0xF0u) >> 4u;

        /*total length check*/
        if (((Dcm_MsgLenType)2 + MemoryAddressSize + MemoryLengthSize) != pMsgContext->ReqDataLen)
        {
            /*the length of message is not correct,send NRC 0x13*/
            *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
            ret = E_NOT_OK;
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <UDS  0x23 service >
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
Dcm_UDS0x23(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432,1532++ */ /* MISRA Rule 20.7,8.7 */
{
    uint8 MsgCtrlId;
    Std_ReturnType ret;
    uint8 AddressAndLengthFormatIdentifier;
    uint32 MemoryAddress = 0u;
    uint32 Memorysize = 0u;
    uint8 TxChannelCtrlIndex;
    uint8 TxChannelCfgIndex;
    uint32 Offset;
    uint8* DCM_MemoryData;
    uint8 MemoryIdInfoIndex = 0u;
    uint8 MemoryRangeInfoIndex = 0u;
    uint8 MemoryAddressSize;
    uint8 MemoryLengthSize;
    uint8 index;
    uint8 MemoryAddressMatchNum = 0;
    uint8 MemoryIdentifier;
    Dcm_0x23Types Dcm_0x23Type;
    const Dcm_DspMemoryType* pDcmDspMemory = DcmPbCfgPtr->pDcmDspCfg->pDcmDspMemory;
    Dcm_MsgContextType* pMsgContext;
    const Dcm_DslBufferType* pDcmChannelCfg;

    /*if the required protocol is configured,get the index of runtime datum*/
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
    ret = Dcm_Uds0x23ServiceConditionCheck(ProtocolCtrlId, ErrorCode);
    if (E_OK == ret)
    {
        AddressAndLengthFormatIdentifier = pMsgContext->pReqData[1];
        MemoryAddressSize = AddressAndLengthFormatIdentifier & 0x0Fu;
        MemoryLengthSize = (AddressAndLengthFormatIdentifier & 0xF0u) >> 4u;
        /*caculate the MemoryAddress of the request message*/
        for (index = 0u; index < MemoryAddressSize; index++)
        {
            MemoryAddress = MemoryAddress << 8u;
            MemoryAddress = MemoryAddress | ((uint32)pMsgContext->pReqData[2u + index]);
        }
        for (index = 0u; index < MemoryLengthSize; index++)
        {
            Memorysize = Memorysize << 8u;
            Memorysize = Memorysize | ((uint32)pMsgContext->pReqData[2u + MemoryAddressSize + index]);
        }
        /*MemoryAddress Range Check*/
        Dcm_0x23Type.MemoryAddress = MemoryAddress;
        Dcm_0x23Type.Memorysize = Memorysize;
        ret = Dcm_0x23ReadMemoryRangeCheck(
            &Dcm_0x23Type,
            &MemoryIdInfoIndex,
            &MemoryRangeInfoIndex,
            &MemoryAddressMatchNum);
        if ((E_NOT_OK == ret) || (MemoryAddressMatchNum > 1u) || (Memorysize == 0u))
        {
            /*different MemoryIdValue compare to the request memoryAddress,send NRC 0x31*/
            /*memoryAddress is not inside the allowed memory ranges,send NRC 0x31 */
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            ret = E_NOT_OK;
        }
    }
    if (E_OK == ret)
    {
        /*MemoryAddress session Accsess Check*/
        ret = Dcm_0x23MemoryReadSessionCheck(MemoryIdInfoIndex, MemoryRangeInfoIndex, ErrorCode);
    }
    if (E_OK == ret)
    {
        /*MemoryAddress Security Accsess Check*/
        ret = Dcm_0x23MemoryReadSecurityCheck(MemoryIdInfoIndex, MemoryRangeInfoIndex, ErrorCode);
    }

    if (E_OK == ret)
    { /*call the callout*/
        TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
        TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
        pDcmChannelCfg = &DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex];
        Offset = pDcmChannelCfg->offset;
        DCM_MemoryData = &Dcm_Channel[Offset + 1u];
        MemoryIdentifier = pDcmDspMemory->DcmDspMemoryIdInfo[MemoryIdInfoIndex].DcmDspMemoryIdValue;
        ret = Dcm_ReadMemory(OpStatus, MemoryIdentifier, MemoryAddress, Memorysize, DCM_MemoryData, ErrorCode);
        if (DCM_READ_FAILED == ret)
        {
            /*the processing is not successful,send NRC */
            ret = E_NOT_OK;
        }
#if (STD_ON == DCM_DSLDIAGRESP_FORCERESPENDEN)
        else if (DCM_READ_FORCE_RCRRP == ret)
        {
            /*the processing is pending,send NRC 0x78*/
            DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_RESPONSE_PENDING);
            DsdInternal_ProcessingDone(ProtocolCtrlId);
            Dcm_MsgCtrl[MsgCtrlId].Dcm_Ret = DCM_E_FORCE_RCRRP;
            ret = DCM_E_FORCE_RCRRP;
        }
#endif
        else if (DCM_READ_PENDING == ret)
        {
            ret = DCM_E_PENDING;
            Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
        }
        else
        {
            /*idle*/
        }
    }
    if ((E_OK == ret) && ((1u + Memorysize) > pDcmChannelCfg->Dcm_DslBufferSize))
    {
        /* check tx data length */
        /*Pdu length is bigger than buffer size */
        *ErrorCode = DCM_E_RESPONSETOOLONG;
        ret = E_NOT_OK;
    }
    if (E_OK == ret)
    {
        /*the processing is successful,assemble positive response*/
        Dcm_Channel[Offset] = 0x63; /*response SID*/
        pMsgContext->ResMaxDataLen = 1u + Memorysize;
        pMsgContext->ResDataLen = 1u + Memorysize;
        pMsgContext->pResData = &Dcm_Channel[Offset];
        DsdInternal_ProcessingDone(ProtocolCtrlId);
    }
    return ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#endif
