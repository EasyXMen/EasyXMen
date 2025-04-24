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
         UDS:DynamicallyDefineDataIdentifier  (2C hex) service
 ***************************************************************/
#if (STD_ON == DCM_UDS_SERVICE0X2C_ENABLED)

#define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"
VAR(Dcm_DDDidTypes, DCM_VAR_NOINIT) Dcm_DDDid[DCM_DSP_DDDID_MAX_NUMBER];
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
#if (STD_ON == DCM_DSP_DID_FUNC_ENABLED)
static FUNC(Std_ReturnType, DCM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    DspInternalUDS0x2C_DidCheck(uint16 RecDid, P2VAR(uint16, AUTOMATIC, DCM_VAR) pDidCfgIndex)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint16 Index;
    Std_ReturnType ret = E_NOT_OK;
    const Dcm_DspCfgType* pDcmDspCfg = DcmPbCfgPtr->pDcmDspCfg;
    const Dcm_DspDidType* pDcmDspDid = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDid;
    const Dcm_DspDidInfoType* pDcmDspDidInfo = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDidInfo;

    if ((RecDid >= 0xF200u) && (RecDid <= 0xF3FFu))
    {
        /*find the corresponding DID in configuration*/
        for (Index = 0; (Index < pDcmDspCfg->DcmDspDidNum) && (E_NOT_OK == ret); Index++)
        {
            /*single did check*/
            if ((RecDid == pDcmDspDid[Index].DcmDspDidId) && (TRUE == pDcmDspDid[Index].DcmDspDidUsed)
                && (pDcmDspDidInfo[pDcmDspDid[Index].DcmDspDidInfoIndex].DcmDspDidDynamicallyDefined == TRUE)
                && (pDcmDspDidInfo[pDcmDspDid[Index].DcmDspDidInfoIndex].DcmDspDDDIDMaxElements > 0u))
            {
                (*pDidCfgIndex) = Index;
                ret = E_OK;
            }
        }
    }
    return ret;
}
#endif /* STD_ON == DCM_DSP_DID_FUNC_ENABLED */

#if ((STD_ON == DCM_DSP_DID_FUNC_ENABLED) && (DCM_DSP_DDDID_MAX_NUMBER > 0u))
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
static FUNC(Std_ReturnType, DCM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    DspInternalUDS0x2C_SourceDidCheck(uint16 RecDid, P2VAR(uint16, AUTOMATIC, DCM_VAR) pDidCfgIndex)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint16 Index;
    Std_ReturnType ret = E_NOT_OK;
    const Dcm_DspCfgType* pDcmDspCfg = DcmPbCfgPtr->pDcmDspCfg;
    const Dcm_DspDidType* pDcmDspDid = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDid;

    /*find the corresponding DID in configuration*/
    for (Index = 0; (Index < pDcmDspCfg->DcmDspDidNum) && (E_NOT_OK == ret); Index++)
    {
        /*single did check*/
        if ((RecDid == pDcmDspDid[Index].DcmDspDidId) && (TRUE == pDcmDspDid[Index].DcmDspDidUsed))
        {
            (*pDidCfgIndex) = Index;
            ret = E_OK;
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
static FUNC(Std_ReturnType, DCM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    DspInternalUDS0x2C_SourceDidLength(uint16 DidCfgIndex, P2VAR(uint16, AUTOMATIC, DCM_VAR) Length)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint16 DidSignalIndex;
    uint16 DidSignalNum;
    const Dcm_DspDataType* pDspDidData;
    Std_ReturnType ret = E_OK;
    uint16 Size;
    const Dcm_DspDidType* pDcmDspDid = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDid;

    DidSignalNum = pDcmDspDid[DidCfgIndex].DcmDspDidSignalNum;
    for (DidSignalIndex = 0; (DidSignalIndex < DidSignalNum) && (E_OK == ret); DidSignalIndex++)
    {
        pDspDidData = pDcmDspDid[DidCfgIndex].pDcmDspDidSignal[DidSignalIndex].pDcmDspDidData;
        Size = 0;
#if (STD_ON == DCM_DATA_TYPE_UINT8_DYN_ENABLED)
        if (DCM_UINT8_DYN == pDspDidData->DcmDspDataType)
        {
            /*DidDataSize is dynamic*/
            if (NULL_PTR == pDspDidData->DcmDspDataReadDataLengthFnc)
            {
                ret = E_NOT_OK;
            }
            else
            {
                ret = pDspDidData->DcmDspDataReadDataLengthFnc(DCM_INITIAL, &Size);
            }
        }
        else
#endif
        {
            /*DidDataSize is fixed*/
            Size = pDspDidData->DcmDspDataSize;
        }
        (*Length) += Size;
    }
    return ret;
}
#endif /* STD_ON == DCM_DSP_DID_FUNC_ENABLED && DCM_DSP_DDDID_MAX_NUMBER > 0u */

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
static FUNC(Std_ReturnType, DCM_CODE) Dcm_Uds0x2CServiceConditionCheck(
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret;
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    uint8 Subfunction;
    uint8 addressAndLengthFormatIdentifier;
    uint8 memoryAddressByteLength;
    uint8 memorySizeByteLength;
    const Dcm_MsgContextType* pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
    ret = Dcm_UdsSubServicesCheck(ProtocolCtrlId);
    if (E_NOT_OK == ret)
    {
        /*if the required reset type is not supported,send NRC 0x12*/
        *ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTED;
    }
#if (STD_ON == DCM_UDS_SERVICE0X29_ENABLED)
    if (E_OK == ret)
    {
        ret = DspInternal_SubServiceAuthenticationCheck(ProtocolCtrlId, ErrorCode);
    }
#endif /* STD_ON == DCM_UDS_SERVICE0X29_ENABLED */
#if (STD_ON == DCM_SESSION_FUNC_ENABLED)
    if (E_OK == ret)
    {
        ret = DsdInternal_SubSesCheck(ProtocolCtrlId, SID_DYNAMICALLY_DEFINE_DATA_IDENTIFER, ErrorCode);
    }
#endif /* STD_ON == DCM_SESSION_FUNC_ENABLED */
#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
    if (E_OK == ret)
    {
        /*security check,check whether the current security supports the request service*/
        ret = DsdInternal_SubSecurityCheck(ProtocolCtrlId, SID_DYNAMICALLY_DEFINE_DATA_IDENTIFER, ErrorCode);
    }
#endif /* STD_ON == DCM_SECURITY_FUNC_ENABLED */
    if (E_OK == ret)
    {
        Subfunction = pMsgContext->pReqData[1u];
        if (Subfunction == DCM_UDS0X2C_01_DDBYDID)
        {
            /*check the massage length*/
            if ((Dcm_MsgCtrl[Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex].MsgContext.ReqDataLen
                 < DCM_UDS0X2C_SUBFUNC0X01_REQ_DATA_MINLENGTH)
                || ((pMsgContext->ReqDataLen % 4u) != 0u))
            {
                /*the length of massage is not correct,send NRC 0x13*/
                *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                ret = E_NOT_OK;
            }
        }
        else if (Subfunction == DCM_UDS0X2C_02_DDBYMEMORY)
        {
            if (Dcm_MsgCtrl[Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex].MsgContext.ReqDataLen
                < DCM_UDS0X2C_SUBFUNC0X02_REQ_DATA_MINLENGTH)
            {
                /*the length of massage is not correct,send NRC 0x13*/
                *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                ret = E_NOT_OK;
            }
            else
            {
                addressAndLengthFormatIdentifier = pMsgContext->pReqData[4u];
                ret = DsdInternal_ServiceAddressAndLengthFormatIdentifierCheck(addressAndLengthFormatIdentifier);
                if (E_NOT_OK == ret)
                {
                    /*the processing is not successful,send NRC 0x31 */
                    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
                    ret = E_NOT_OK;
                }
                else
                {
                    memoryAddressByteLength = addressAndLengthFormatIdentifier & 0x0Fu;
                    memorySizeByteLength = (addressAndLengthFormatIdentifier & 0xF0u) >> 4u;
                    /*check the massage length*/
                    if ((addressAndLengthFormatIdentifier == 0u)
                        || (((pMsgContext->ReqDataLen - 5UL)
                             % ((uint32)((uint32)memorySizeByteLength + memoryAddressByteLength)))
                            != 0UL))
                    {
                        /*the length of massage is not correct,send NRC 0x13*/
                        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                        ret = E_NOT_OK;
                    }
                }
            }
        }
        else if (Subfunction == DCM_UDS0X2C_03_CLEARDDDID)
        {
            /*check the massage length*/
            if ((pMsgContext->ReqDataLen != DCM_UDS0X2C_REQ_DATA_MINLENGTH)
                && (pMsgContext->ReqDataLen != DCM_UDS0X2C_SUBFUNC0X03_REQ_DATA_MINLENGTH))
            {
                /*the length of massage is not correct,send NRC 0x13*/
                *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                ret = E_NOT_OK;
            }
        }
        else
        {
            /*the sub-function of massage is not correct,send NRC 0x12*/
            *ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTED;
            ret = E_NOT_OK;
        }
    }
    return ret;
}

#if (DCM_DSP_DDDID_MAX_NUMBER > 0u)
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
static FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x2CSubDealDid(
    uint8 ProtocolCtrlId,
    uint16 iloop,
    uint32 TotalSize,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;
    uint32 MemoryAddress;
    uint8 Cnt;
    uint16 Length = 0;
    uint16 pDidCfgIndex = 0xFFFF;
    uint8 RequestNum;
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    const Dcm_MsgContextType* pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
    uint8 Subfunction = pMsgContext->pReqData[1u];
    uint16 RecDid;
    Dcm_DDDidElementsDataTypes* pArgumentData;
    uint32 Size = TotalSize;

    RequestNum = (uint8)((pMsgContext->ReqDataLen >> 2u) - 1UL);
    for (Cnt = 0u; (Cnt < RequestNum) && (ret == E_OK); Cnt++)
    {
        pArgumentData = &Dcm_DDDid[iloop].DcmDspAlternativeArgumentData[Dcm_DDDid[iloop].DDDIDNumOfElements];
        pArgumentData->Subfunction = Subfunction;
        RecDid = (uint16)((uint16)((uint16)pMsgContext->pReqData[4u + (Cnt * (4u))]) << 8u)
                 | ((uint16)pMsgContext->pReqData[5u + (Cnt * (4u))]);
        ret = DspInternalUDS0x2C_SourceDidCheck(RecDid, &pDidCfgIndex);
        if (pDidCfgIndex != 0xFFFFu)
        {
            (void)DspInternalUDS0x2C_SourceDidLength(pDidCfgIndex, &Length);
        }
        if ((ret == E_NOT_OK)
            || (Length < (uint16)((uint16)pMsgContext->pReqData[6u + (Cnt * (4u))]
                                  + (uint16)pMsgContext->pReqData[7u + (Cnt * (4u))] - 1u))
            || (pMsgContext->pReqData[6u + (Cnt * (4u))] == 0u))
        {
            /*Source Did not support,send NRC 0x31*/
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            ret = E_NOT_OK;
        }
        else
        {
            MemoryAddress = (uint32)(((uint32)pMsgContext->pReqData[4u + (Cnt * (4u))]) << 8u);
            MemoryAddress = MemoryAddress | (uint32)(((uint32)pMsgContext->pReqData[5u + (Cnt * (4u))]));
            MemoryAddress = MemoryAddress | (uint32)(((uint32)pMsgContext->pReqData[6u + (Cnt * (4u))] - 1u) << 16u);
            pArgumentData->Data = MemoryAddress;
            pArgumentData->Size = (uint16)((uint16)pMsgContext->pReqData[7u + (Cnt * (4u))]);
            Size += pArgumentData->Size;
            if (((Size > 7u) && (pMsgContext->pReqData[2] == 0xF2u)) || (pArgumentData->Size == 0u))
            {
                /*total Did size over 7,send NRC 0x31*/
                *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
                ret = E_NOT_OK;
            }
            else
            {
                Dcm_DDDid[iloop].DDDIDNumOfElements++;
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
static FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x2CSubDealMemory(
    uint8 ProtocolCtrlId,
    uint16 iloop,
    uint32 TotalSize,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;
    uint32 MemoryAddress;
    uint32 Memorysize;
    uint8 index;
    uint8 Cnt;
    uint8 Cntt;
    uint8 addressAndLengthFormatIdentifier;
    uint8 memoryAddressByteLength;
    uint8 memorySizeByteLength;
    uint8 RequestNum;
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    const uint8* pReqData = Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData;
    uint8 Subfunction = pReqData[1u];
    Dcm_DDDidElementsDataTypes* pAlternativeArgumentData;
    uint32 Size = TotalSize;

    addressAndLengthFormatIdentifier = pReqData[4u];
    memoryAddressByteLength = addressAndLengthFormatIdentifier & 0x0Fu;
    memorySizeByteLength = (addressAndLengthFormatIdentifier & 0xF0u) >> 4u;
    RequestNum = (uint8)(Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen - 5u);
    RequestNum /= (memorySizeByteLength + memoryAddressByteLength);
    for (Cnt = 0u; (Cnt < RequestNum) && (ret == E_OK); Cnt++)
    {
        Cntt = Cnt * (memorySizeByteLength + memoryAddressByteLength);
        pAlternativeArgumentData = &Dcm_DDDid[iloop].DcmDspAlternativeArgumentData[Dcm_DDDid[iloop].DDDIDNumOfElements];
        pAlternativeArgumentData->Subfunction = Subfunction;
        /*caculate the MemoryAddress of the request message*/
        MemoryAddress = 0;
        for (index = 0u; index < memoryAddressByteLength; index++)
        {
            MemoryAddress = MemoryAddress << 8u;
            MemoryAddress = MemoryAddress | (uint32)(((uint32)pReqData[5u + index + Cntt]));
        }
        Memorysize = 0;
        for (index = 0u; index < memorySizeByteLength; index++)
        {
            Memorysize = Memorysize << 8u;
            Memorysize = Memorysize | (uint32)(((uint32)pReqData[5u + memoryAddressByteLength + index + Cntt]));
        }
        pAlternativeArgumentData->Data = MemoryAddress;
        pAlternativeArgumentData->Size = (uint16)Memorysize;
        Size += Memorysize;
        if (((Size > 7u) && (pReqData[2] == 0xF2u)) || (Memorysize == 0u))
        {
            /*total Did size over 7,send NRC 0x31*/
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            ret = E_NOT_OK;
        }
        else
        {
            Dcm_DDDid[iloop].DDDIDNumOfElements++;
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
static FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x2CSubDeal_NoFind(
    uint8 ProtocolCtrlId,
    uint16 DidCfgIndex,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;
    boolean Find = FALSE;
    uint16 iloop;
    uint8 addressAndLengthFormatIdentifier;
    uint8 memoryAddressByteLength;
    uint8 memorySizeByteLength;
    uint8 RequestNum;
    uint32 TotalSize = 0;
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    const Dcm_MsgContextType* pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
    const uint8* pReqData = pMsgContext->pReqData;
    uint8 Subfunction = pReqData[1u];
    uint16 RecDid = (uint16)((uint16)((uint16)pReqData[2]) << 8u) | ((uint16)pReqData[3]);
    const Dcm_DspDidType* pDcmDspDid = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDid;
    const Dcm_DspDidInfoType* pDcmDspDidInfo = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDidInfo;
    uint16 DidInfoIndex = pDcmDspDid[DidCfgIndex].DcmDspDidInfoIndex;

    for (iloop = 0; (iloop < DCM_DSP_DDDID_MAX_NUMBER) && (Find == FALSE); iloop++)
    {
        if (0u == Dcm_DDDid[iloop].DDDid)
        {
            Find = TRUE;
        }
    }
    iloop--;
    if (Find == TRUE)
    {
        Dcm_DDDid[iloop].DDDid = RecDid;
        if (Subfunction == DCM_UDS0X2C_01_DDBYDID)
        {
            RequestNum = (uint8)((pMsgContext->ReqDataLen >> 2u) - 1UL);
            if ((Dcm_DDDid[iloop].DDDIDNumOfElements + RequestNum)
                > pDcmDspDidInfo[DidInfoIndex].DcmDspDDDIDMaxElements)
            {
                /*over DcmDspDDDIDMaxElements,send NRC 0x31*/
                *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
                ret = E_NOT_OK;
            }
            else
            {
                ret = Dcm_UDS0x2CSubDealDid(ProtocolCtrlId, iloop, TotalSize, ErrorCode);
            }
        }
        else if (Subfunction == DCM_UDS0X2C_02_DDBYMEMORY)
        {
            addressAndLengthFormatIdentifier = pReqData[4u];
            memoryAddressByteLength = addressAndLengthFormatIdentifier & 0x0Fu;
            memorySizeByteLength = (addressAndLengthFormatIdentifier & 0xF0u) >> 4u;
            RequestNum = (uint8)(pMsgContext->ReqDataLen - 5u);
            RequestNum /= (memorySizeByteLength + memoryAddressByteLength);
            if (((Dcm_DDDid[iloop].DDDIDNumOfElements + RequestNum)
                 > pDcmDspDidInfo[DidInfoIndex].DcmDspDDDIDMaxElements)
                || (memorySizeByteLength == 0u))
            {
                *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
                ret = E_NOT_OK;
            }
            else
            {
                ret = Dcm_UDS0x2CSubDealMemory(ProtocolCtrlId, iloop, TotalSize, ErrorCode);
            }
        }
        else if (Subfunction == DCM_UDS0X2C_03_CLEARDDDID)
        {
            Dcm_DDDid[iloop].DDDid = 0u; /*clear DDDID*/
            Dcm_DDDid[iloop].DDDIDNumOfElements = 0;
        }
        else
        {
            /*idle*/
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
static FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x2CSubDeal(
    uint8 ProtocolCtrlId,
    uint16 DidCfgIndex,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;
    boolean Find = FALSE;
    uint16 iloop;
    uint8 Cnt;
    uint8 addressAndLengthFormatIdentifier;
    uint8 memoryAddressByteLength;
    uint8 memorySizeByteLength;
    uint8 RequestNum;
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    const Dcm_MsgContextType* pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
    const uint8* pReqData = pMsgContext->pReqData;
    uint8 Subfunction = pReqData[1u];
    uint16 RecDid;
    uint32 TotalSize = 0;
    const Dcm_DspDidType* pDcmDspDid = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDid;
    const Dcm_DspDidInfoType* pDcmDspDidInfo = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDidInfo;
    uint16 DidInfoIndex;
    uint8 DDDIDMaxElements;

    if (DidCfgIndex != 0xFFFFu)
    {
        RecDid = (uint16)((uint16)((uint16)pReqData[2]) << 8u) | ((uint16)pReqData[3]);

        for (iloop = 0; (iloop < DCM_DSP_DDDID_MAX_NUMBER) && (Find == FALSE); iloop++)
        {
            if (RecDid == Dcm_DDDid[iloop].DDDid)
            {
                Find = TRUE;
            }
        }
        iloop--;
        if (Find == FALSE)
        {
            ret = Dcm_UDS0x2CSubDeal_NoFind(ProtocolCtrlId, DidCfgIndex, ErrorCode);
        }
        else
        {
            DidInfoIndex = pDcmDspDid[DidCfgIndex].DcmDspDidInfoIndex;
            DDDIDMaxElements = pDcmDspDidInfo[DidInfoIndex].DcmDspDDDIDMaxElements;
            if (Subfunction == DCM_UDS0X2C_01_DDBYDID)
            {
                RequestNum = (uint8)((pMsgContext->ReqDataLen >> 2u) - 1UL);
                if ((Dcm_DDDid[iloop].DDDIDNumOfElements + RequestNum) > DDDIDMaxElements)
                {
                    /*over DcmDspDDDIDMaxElements,send NRC 0x31*/
                    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
                    ret = E_NOT_OK;
                }
                else
                {
                    for (Cnt = 0u; Cnt < Dcm_DDDid[iloop].DDDIDNumOfElements; Cnt++)
                    {
                        TotalSize += Dcm_DDDid[iloop].DcmDspAlternativeArgumentData[Cnt].Size;
                    }
                    ret = Dcm_UDS0x2CSubDealDid(ProtocolCtrlId, iloop, TotalSize, ErrorCode);
                }
            }
            else if (Subfunction == DCM_UDS0X2C_02_DDBYMEMORY)
            {
                addressAndLengthFormatIdentifier = pReqData[4u];
                memoryAddressByteLength = addressAndLengthFormatIdentifier & 0x0Fu;
                memorySizeByteLength = (addressAndLengthFormatIdentifier & 0xF0u) >> 4u;
                RequestNum = (uint8)(pMsgContext->ReqDataLen - 5u);
                RequestNum /= (memorySizeByteLength + memoryAddressByteLength);
                if (((Dcm_DDDid[iloop].DDDIDNumOfElements + RequestNum) > DDDIDMaxElements)
                    || (memorySizeByteLength == 0u))
                {
                    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
                    ret = E_NOT_OK;
                }
                else
                {
                    for (Cnt = 0u; Cnt < Dcm_DDDid[iloop].DDDIDNumOfElements; Cnt++)
                    {
                        TotalSize += Dcm_DDDid[iloop].DcmDspAlternativeArgumentData[Cnt].Size;
                    }
                    ret = Dcm_UDS0x2CSubDealMemory(ProtocolCtrlId, iloop, TotalSize, ErrorCode);
                }
            }
            else if (Subfunction == DCM_UDS0X2C_03_CLEARDDDID)
            {
#if (STD_ON == DCM_UDS_SERVICE0X2A_ENABLED)
                /* DDDID reported periodically shall be stopped firstly*/
                Dcm_UDS0x2A_DDDidStop(iloop);
#endif
                Dcm_DDDid[iloop].DDDid = 0u; /*clear DDDID*/
                Dcm_DDDid[iloop].DDDIDNumOfElements = 0;
            }
            else
            {
                /*idle*/
            }
        }
    }
    else
    {
        if (Subfunction == DCM_UDS0X2C_03_CLEARDDDID)
        {
            for (iloop = 0; iloop < DCM_DSP_DDDID_MAX_NUMBER; iloop++)
            {
#if (STD_ON == DCM_UDS_SERVICE0X2A_ENABLED)
                /* DDDID reported periodically shall be stopped firstly*/
                Dcm_UDS0x2A_DDDidStop(iloop);
#endif
                Dcm_DDDid[iloop].DDDid = 0u; /*clear DDDID*/
                Dcm_DDDid[iloop].DDDIDNumOfElements = 0;
            }
        }
    }
    return ret;
}
#endif /* DCM_DSP_DDDID_MAX_NUMBER > 0u */

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
#if (DCM_DDDID_STORAGE == STD_ON)
FUNC(void, DCM_CODE) Dcm_UDS0x2C_Init(void)
{
    uint16 Index;
#if (NVM_ENABLE == STD_OFF)
    for (Index = 0; Index < DCM_DSP_DDDID_MAX_NUMBER; Index++)
    {
        Dcm_DDDid[Index].DDDid = 0;
        Dcm_DDDid[Index].DDDIDNumOfElements = 0;
    }
#else /* NVM_ENABLE == STD_OFF */
#ifdef DCM_DDDID_STORAGE_BLOCKID
    NvM_RequestResultType NvmRet;

    (void)NvM_GetErrorStatus(DCM_DDDID_STORAGE_BLOCKID, &NvmRet);
    if (NVM_REQ_OK == NvmRet)
    {
        (void)NvM_ReadBlock(DCM_DDDID_STORAGE_BLOCKID, (void*)&Dcm_DDDid);
    }
    else
#endif
    {
        for (Index = 0; Index < DCM_DSP_DDDID_MAX_NUMBER; Index++)
        {
            Dcm_DDDid[Index].DDDid = 0;
            Dcm_DDDid[Index].DDDIDNumOfElements = 0;
        }
    }
#endif /* NVM_ENABLE == STD_OFF */
}
#endif /* DCM_DDDID_STORAGE == STD_ON */

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
Dcm_UDS0x2C(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432,1532-- */ /* MISRA Rule 20.7,8.7 */
{
#if (STD_ON == DCM_DSP_DID_FUNC_ENABLED)
    uint16 RecDid = 0;
    uint32 Offset;
    uint16 DidCfgIndex = 0xFFFF;
    uint8 TxChannelCtrlIndex;
    uint8 TxChannelCfgIndex;
#endif /* STD_ON == DCM_DSP_DID_FUNC_ENABLED */
    uint8 MsgCtrlId;
    Std_ReturnType ret;
    uint8 Subfunction;
    Dcm_MsgContextType* pMsgContext;
    const uint8* pReqData;

    DCM_UNUSED(OpStatus);
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
    pReqData = pMsgContext->pReqData;
    Subfunction = pReqData[1u];
    ret = Dcm_Uds0x2CServiceConditionCheck(ProtocolCtrlId, ErrorCode);

#if (STD_OFF == DCM_DSP_DID_FUNC_ENABLED)
    /*NRC 0x31:request out of range*/
    if (E_OK == ret)
    {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        ret = E_NOT_OK;
    }
#else /* STD_OFF == DCM_DSP_DID_FUNC_ENABLED */
    if ((E_OK == ret) && (pMsgContext->ReqDataLen > DCM_UDS0X2C_REQ_DATA_MINLENGTH))
    {
        /*get the required DID from request message*/
        RecDid = (uint16)((uint16)((uint16)pReqData[2]) << 8u) | ((uint16)pReqData[3]);
        /*Determine if the DID is configured*/
        ret = DspInternalUDS0x2C_DidCheck(RecDid, &DidCfgIndex);
        if (E_NOT_OK == ret)
        {
            /*if not found,send NRC 0x31*/
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        }
    }
#if (DCM_DSP_DDDID_MAX_NUMBER > 0u)
    if (E_OK == ret)
    {
        ret = Dcm_UDS0x2CSubDeal(ProtocolCtrlId, DidCfgIndex, ErrorCode);
    }
#else  /* DCM_DSP_DDDID_MAX_NUMBER > 0u */
    /*NRC 0x31:request out of range*/
    if (E_OK == ret)
    {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        ret = E_NOT_OK;
    }
#endif /* DCM_DSP_DDDID_MAX_NUMBER > 0u */

    if (E_OK == ret)
    {
        /*assemble and send positive response*/
        TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
        TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
        Offset = (DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg)[TxChannelCfgIndex].offset;
        /* check tx data length */
        if ((4u) > (DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex].Dcm_DslBufferSize))
        {
            /*Pdu length is bigger than buffer size,ignore the request message */
            *ErrorCode = DCM_E_RESPONSETOOLONG;
            ret = E_NOT_OK;
        }
    }
    if (E_OK == ret)
    {
        Dcm_Channel[Offset] = 0x6C;
        Dcm_Channel[Offset + 1u] = Subfunction;
        if (pMsgContext->ReqDataLen > DCM_UDS0X2C_REQ_DATA_MINLENGTH)
        {
            Dcm_Channel[Offset + 2u] = (uint8)(RecDid >> 8u);
            Dcm_Channel[Offset + 3u] = (uint8)RecDid;
            pMsgContext->ResMaxDataLen = 4u;
            pMsgContext->ResDataLen = 4u;
        }
        else
        {
            pMsgContext->ResMaxDataLen = 2u;
            pMsgContext->ResDataLen = 2u;
        }
        pMsgContext->pResData = &Dcm_Channel[Offset];
        DsdInternal_ProcessingDone(ProtocolCtrlId);
#if ((NVM_ENABLE == STD_ON) && (DCM_DDDID_STORAGE == STD_ON) && (DCM_DSP_DDDID_MAX_NUMBER > 0u))
#ifdef DCM_DDDID_STORAGE_BLOCKID
        (void)NvM_WriteBlock(DCM_DDDID_STORAGE_BLOCKID, (void*)&Dcm_DDDid);
#endif
#endif /* NVM_ENABLE == STD_ON && DCM_DDDID_STORAGE == STD_ON && DCM_DSP_DDDID_MAX_NUMBER > \
          0u */
    }
#endif /* STD_OFF == DCM_DSP_DID_FUNC_ENABLED */
    return ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif /* STD_ON == DCM_UDS_SERVICE0X2C_ENABLED */
