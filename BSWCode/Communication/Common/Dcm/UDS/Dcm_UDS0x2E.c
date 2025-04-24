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
         UDS:WriteDataByIdentifier (2E hex) service
 ***************************************************************/
#if (STD_ON == DCM_UDS_SERVICE0X2E_ENABLED)

typedef struct
{
    uint8* pRangeDidCfgIndex;
    uint16* pDidCfgIndex;
    boolean* pRangeDidFlag;
    uint16* DidInfoCfgIndex;
} Dcm_0x2ETypes;

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
#if (STD_ON == DCM_DSP_DID_FUNC_ENABLED)
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
static Std_ReturnType DspInternalUDS0x2E_DidCheck(
    Dcm_OpStatusType OpStatus,
    uint16 RecDid,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2CONST(Dcm_0x2ETypes, AUTOMATIC, DCM_VAR) Dcm_0x2EType,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) pNrc)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint16 Index;
    boolean Flag = FALSE;
#if (DCM_DID_RANGE_ENABLED == STD_ON)
    uint8 Idx;
    boolean Flagx = FALSE;
    Dcm_DidSupportedType DidSupported;
    const Dcm_DspDidRangeType* pDcmDspDidRange = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDidRange;
#endif
    Std_ReturnType ret = E_NOT_OK;
    const Dcm_DspCfgType* pDcmDspCfg = DcmPbCfgPtr->pDcmDspCfg;
    const Dcm_DspDidType* pDcmDspDid = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDid;
    *Dcm_0x2EType->pRangeDidFlag = FALSE;
    /*find the corresponding DID in configuration*/
    /*first check whether the receiveDid is single did*/
    for (Index = 0; (Index < pDcmDspCfg->DcmDspDidNum) && (FALSE == Flag); Index++)
    {
        /*single did check*/
        if ((RecDid == pDcmDspDid[Index].DcmDspDidId) && (TRUE == pDcmDspDid[Index].DcmDspDidUsed))
        {
            Flag = TRUE;
            (*Dcm_0x2EType->pDidCfgIndex) = Index;
            ret = E_OK;
        }
    }
#if (DCM_DID_RANGE_ENABLED == STD_ON)
    /*if the receiveDid is not single did,the check whether the receiveDid is range did*/
    if (FALSE == Flag)
    {
        /*range did check*/
        if ((RecDid < 0xF200u) || (RecDid > 0xF3FFu))
        {
            /*range did can not be DDDid*/
            for (Idx = 0; (Idx < pDcmDspCfg->DcmDspDidRangeNum) && (FALSE == Flagx); Idx++)
            {
                /*this range not have gaps*/
                if ((RecDid >= pDcmDspDidRange[Idx].DcmDspDidRangeIdentifierLowerLimit)
                    && (RecDid <= pDcmDspDidRange[Idx].DcmDspDidRangeIdentifierUpperLimit))
                {
                    if (TRUE == pDcmDspDidRange[Idx].DcmDspDidRangeHasGaps)
                    {
                        if (pDcmDspDidRange[Idx].DcmDspDidRangeIsDidAvailableFnc != NULL_PTR)
                        {
                            ret = pDcmDspDidRange[Idx].DcmDspDidRangeIsDidAvailableFnc(RecDid, OpStatus, &DidSupported);
                            if ((ret == E_OK) && (DCM_DID_SUPPORTED == DidSupported))
                            {
                                *Dcm_0x2EType->pRangeDidCfgIndex = Idx;
                                *Dcm_0x2EType->pRangeDidFlag = TRUE;
                                Flagx = TRUE;
                            }
                            else
                            {
                                ret = E_NOT_OK;
                            }
                        }
                    }
                    else
                    {
                        *Dcm_0x2EType->pRangeDidCfgIndex = Idx;
                        *Dcm_0x2EType->pRangeDidFlag = TRUE;
                        Flagx = TRUE;
                    }
                }
            }
        }
        if (TRUE == Flagx)
        {
            (*Dcm_0x2EType->pDidCfgIndex) = Index;
            ret = E_OK;
        }
    }
#else
    DCM_UNUSED(OpStatus);
#endif
    if (E_OK != ret)
    {
        /*if not found,send NRC 0x31*/
        *pNrc = DCM_E_REQUESTOUTOFRANGE;
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
static Std_ReturnType DspInternalUDS0x2E_DidSesCheck(uint16 DidInfoCfgIndex)
{
    uint8 Index;
    Std_ReturnType ret = E_NOT_OK;
    const Dcm_DspDidWriteType* pDspDidWrite;
    const Dcm_DspCfgType* pDcmDspCfg = DcmPbCfgPtr->pDcmDspCfg;
    uint8 SessionRefNum;

    pDspDidWrite = pDcmDspCfg->pDcmDspDidInfo[DidInfoCfgIndex].pDcmDspDidWrite;
    SessionRefNum = pDspDidWrite->DcmDspDidWriteSessionRefNum;
    if ((NULL_PTR == pDspDidWrite))
    {
        /*Judge DID the configuration items pDcmDspDidWrite whether NULL_PTR,
         *if not supported,send Nrc 0x31*/
        ret = E_NOT_OK;
    }
    else
    {
        if (SessionRefNum != 0u)
        {
            for (Index = 0; (Index < SessionRefNum) && (E_NOT_OK == ret); Index++)
            {
                if (Dcm_MkCtrl.Dcm_ActiveSes == pDspDidWrite->pDcmDspDidWriteSessionRow[Index])
                {
                    ret = E_OK;
                }
            }
        }
        else
        {
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
static Std_ReturnType DspInternalUDS0x2E_DidSecCheck(
    uint16 DidInfoCfgIndex,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) pNrc)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    const Dcm_DspDidWriteType* pDspDidWrite;
    Std_ReturnType ret = E_NOT_OK;
    uint8 Index;
    const Dcm_DspCfgType* pDcmDspCfg = DcmPbCfgPtr->pDcmDspCfg;
    uint8 SecRefNum;

    pDspDidWrite = pDcmDspCfg->pDcmDspDidInfo[DidInfoCfgIndex].pDcmDspDidWrite;
    SecRefNum = pDspDidWrite->DcmDspDidWriteSecurityLevelRefNum;
    if ((NULL_PTR == pDspDidWrite))
    {
        /*Judge DID the configuration items pDcmDspDidWrite whether NULL_PTR,
         *if not supported,send Nrc 0x31*/
        *pNrc = DCM_E_REQUESTOUTOFRANGE;
    }
    else
    {
        if (SecRefNum != 0u)
        {
            for (Index = 0; (Index < SecRefNum) && (E_NOT_OK == ret); Index++)
            {
                if (Dcm_MkCtrl.Dcm_ActiveSec == pDspDidWrite->pDcmDspDidWriteSecurityLevelRow[Index])
                {
                    ret = E_OK;
                }
            }
            if (E_NOT_OK == ret)
            {
                /*if the current security is not supported,send NRC 0x33*/
                *pNrc = DCM_E_SECURITYACCESSDENIED;
            }
        }
        else
        {
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
static Std_ReturnType DspInternalUDS0x2E_DidWriteDataLength(
    uint16 DidCfgIndex,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint16, AUTOMATIC, DCM_VAR) pDidSize)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint16 DidSignalIndex;
    const Dcm_DspDataType* pDspDidData;
    Std_ReturnType ret = E_OK;
    const Dcm_DspDidType* pDcmDspDid = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDid;
    uint16 DidSignalNum = pDcmDspDid[DidCfgIndex].DcmDspDidSignalNum;

    *pDidSize = 0;
    for (DidSignalIndex = 0; DidSignalIndex < DidSignalNum; DidSignalIndex++)
    {
        pDspDidData = pDcmDspDid[DidCfgIndex].pDcmDspDidSignal[DidSignalIndex].pDcmDspDidData;
        if (pDspDidData->DcmDspDataType != DCM_UINT8_DYN)
        {
            (*pDidSize) += pDspDidData->DcmDspDataSize;
        }
        else
        {
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
static Std_ReturnType DspInternalUDS0x2E_DidWrite(
    Dcm_OpStatusType OpStatus,
    uint8 MsgCtrlId,
    uint16 DidCfgIndex,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) pNrc)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint16 DidSignalIndex;
    uint16 DidSignalNum;
    uint16 DidSignalPos;
    uint16 Offest = 0;
    const Dcm_DspDataType* pDspDidData;
    Std_ReturnType ret = E_OK;
    const Dcm_DspDidType* pDcmDspDid = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDid;
    const Dcm_DspDidSignalType* pDcmDspDidSignal = pDcmDspDid[DidCfgIndex].pDcmDspDidSignal;
    const Dcm_MsgContextType* pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
#if ((NVM_ENABLE == STD_ON) && (DCM_DATA_USE_BLOCK_ID_ENABLED == STD_ON))
    NvM_RequestResultType NvmRet;
#endif /* NVM_ENABLE == STD_ON && DCM_DATA_USE_BLOCK_ID_ENABLED == STD_ON */
    DidSignalNum = pDcmDspDid[DidCfgIndex].DcmDspDidSignalNum;
    for (DidSignalIndex = 0; ((DidSignalIndex < DidSignalNum) && (ret == E_OK)); DidSignalIndex++)
    {
        pDspDidData = pDcmDspDidSignal[DidSignalIndex].pDcmDspDidData;
        DidSignalPos = Offest + pDcmDspDidSignal[DidSignalIndex].DcmDspDidDataPos;
#if (DCM_DATA_USE_BLOCK_ID_ENABLED == STD_ON)
        if (USE_BLOCK_ID != pDspDidData->DcmDspDataUsePort)
#endif
        {
            if (pDspDidData->DcmDspDataWriteFnc == NULL_PTR)
            {
                /*if DcmDspDidWriteFnc is NULL,send NRC 0x22*/
                *pNrc = DCM_E_CONDITIONSNOTCORRECT;
                ret = E_NOT_OK;
            }
            else
            {
                ret = pDspDidData->DcmDspDataWriteFnc(
                    &pMsgContext->pReqData[3u + DidSignalPos],
                    pDspDidData->DcmDspDataSize,
                    OpStatus,
                    pNrc);
            }
        }
        Offest += pDspDidData->DcmDspDataSize;
        switch (ret)
        {
        case E_OK:
        case E_NOT_OK:
            break;
        case DCM_E_PENDING:
            Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
            break;
        default:
            *pNrc = DCM_E_CONDITIONSNOTCORRECT;
            ret = E_NOT_OK;
            break;
        }
#if ((NVM_ENABLE == STD_ON) && (DCM_DATA_USE_BLOCK_ID_ENABLED == STD_ON))
        if ((USE_BLOCK_ID == pDspDidData->DcmDspDataUsePort) && (ret == E_OK))
        {
            if (DCM_INITIAL == OpStatus)
            {
                NvM_SetBlockLockStatus(pDspDidData->DcmDspDataBlockId, FALSE);
                ret = NvM_WriteBlock(pDspDidData->DcmDspDataBlockId, (void*)&pMsgContext->pReqData[3u + DidSignalPos]);
                if (E_OK == ret)
                {
                    Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
                    ret = DCM_E_PENDING;
                }
                else
                {
                    *pNrc = DCM_E_GENERALPROGRAMMINGFAILURE;
                    ret = E_NOT_OK;
                }
            }
            else if (DCM_CANCEL == OpStatus)
            {
                /* If the Dcm cancels a service with NvM access, it shall call NvM_CancelJobs(). */
                (void)NvM_CancelJobs(pDspDidData->DcmDspDataBlockId);
            }
            else
            {
                ret = NvM_GetErrorStatus(pDspDidData->DcmDspDataBlockId, &NvmRet);
                if (E_OK == ret)
                {
                    if (NVM_REQ_OK == NvmRet)
                    {
                        NvM_SetBlockLockStatus(pDspDidData->DcmDspDataBlockId, TRUE);
                        ret = E_OK;
                    }
                    else if (NVM_REQ_PENDING == NvmRet)
                    {
                        Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
                        ret = DCM_E_PENDING;
                    }
                    else
                    {
                        *pNrc = DCM_E_GENERALPROGRAMMINGFAILURE;
                        ret = E_NOT_OK;
                    }
                }
            }
        }
#endif /* NVM_ENABLE == STD_ON */
    }
    return ret;
}
#endif /* STD_ON == DCM_DSP_DID_FUNC_ENABLED */

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
static FUNC(Std_ReturnType, DCM_CODE) Dcm_Uds0x2EServiceConditionCheck(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2CONST(Dcm_0x2ETypes, AUTOMATIC, DCM_VAR) Dcm_0x2EType,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;
    uint8 MsgCtrlId;
#if (STD_ON == DCM_DSP_DID_FUNC_ENABLED)
    uint16 RecDid;
#endif /* STD_ON == DCM_DSP_DID_FUNC_ENABLED */
    const Dcm_DspDidType* pDcmDspDid = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDid;
#if (DCM_DID_RANGE_ENABLED == STD_ON)
    const Dcm_DspDidRangeType* pDcmDspDidRange = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDidRange;
#endif
    const Dcm_DspDidInfoType* pDcmDspDidInfo = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDidInfo;
    const Dcm_MsgContextType* pMsgContext;

    /*if the required protocol is configured,get the index of runtime datum*/
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
    /*check the massage length*/
    if (pMsgContext->ReqDataLen < DCM_UDS0X2E_REQ_DATA_MINLENGTH)
    {
        /*the length of massage is not correct,send NRC 0x13*/
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        ret = E_NOT_OK;
    }
#if (STD_OFF == DCM_DSP_DID_FUNC_ENABLED)
    /*NRC 0x31:request out of range*/
    if (E_OK == ret)
    {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        ret = E_NOT_OK;
    }
#else /* STD_OFF == DCM_DSP_DID_FUNC_ENABLED */
    /*get the required DID from request message*/
    if (E_OK == ret)
    {
        RecDid = (uint16)((uint16)((uint16)pMsgContext->pReqData[1]) << 8u) | ((uint16)pMsgContext->pReqData[2]);
        /*Determine if the DID is configured*/
        ret = DspInternalUDS0x2E_DidCheck(OpStatus, RecDid, Dcm_0x2EType, ErrorCode);
        if (DCM_E_PENDING == ret)
        {
            Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
        }
    }
    if (E_OK == ret)
    {
#if (DCM_DID_RANGE_ENABLED == STD_ON)
        if (TRUE == *Dcm_0x2EType->pRangeDidFlag)
        {
            *Dcm_0x2EType->DidInfoCfgIndex = pDcmDspDidRange[*Dcm_0x2EType->pRangeDidCfgIndex].DcmDspDidRangeInfoIndex;
        }
        else
#endif
        {
            *Dcm_0x2EType->DidInfoCfgIndex = pDcmDspDid[*Dcm_0x2EType->pDidCfgIndex].DcmDspDidInfoIndex;
        }
        /*check whether the Did can be configured*/
        if ((TRUE == (pDcmDspDidInfo[*Dcm_0x2EType->DidInfoCfgIndex].DcmDspDidDynamicallyDefined))
            || (pDcmDspDidInfo[*Dcm_0x2EType->DidInfoCfgIndex].pDcmDspDidWrite == NULL_PTR))
        {
            /*if the Did can be dynamically defined,send NRC 0x31*/
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            ret = E_NOT_OK;
        }
    }
#endif /* STD_OFF == DCM_DSP_DID_FUNC_ENABLED */
/*check the current session*/
#if (STD_ON == DCM_SESSION_FUNC_ENABLED)
    if (E_OK == ret)
    {
        ret = DspInternalUDS0x2E_DidSesCheck(*Dcm_0x2EType->DidInfoCfgIndex);
        if (E_NOT_OK == ret)
        {
            /*if the current session is not support the Did,send NRC 31*/
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        }
    }
#endif /* STD_ON == DCM_SESSION_FUNC_ENABLED */
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
static FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x2ESubCheck(
    uint8 ProtocolCtrlId,
    uint16 RecDid,
    uint16 DidInfoCfgIndex,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK; /* PRQA S 2981 */ /* MISRA Rule 2.2 */
#if (STD_ON == DCM_UDS_SERVICE0X29_ENABLED)
    ret = DspInternal_DidAuthenticationCheck(ProtocolCtrlId, RecDid, DidInfoCfgIndex, ErrorCode);
    if (E_OK == ret)
#else
    DCM_UNUSED(ProtocolCtrlId);
    DCM_UNUSED(RecDid);
#endif /* STD_ON == DCM_UDS_SERVICE0X29_ENABLED */
#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
    {
        /*check the current security level*/
        ret = DspInternalUDS0x2E_DidSecCheck(DidInfoCfgIndex, ErrorCode);
    }
#endif /* STD_ON==DCM_SECURITY_FUNC_ENABLED */
#if ((STD_OFF == DCM_UDS_SERVICE0X29_ENABLED) && (STD_OFF == DCM_SECURITY_FUNC_ENABLED))
    DCM_UNUSED(ProtocolCtrlId);
    DCM_UNUSED(RecDid);
    DCM_UNUSED(DidInfoCfgIndex);
    DCM_UNUSED(ErrorCode);
#endif
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
Dcm_UDS0x2E(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432,1532-- */ /* MISRA Rule 20.7,8.7 */
{
#if (STD_ON == DCM_DSP_DID_FUNC_ENABLED)
    uint16 RecDid;
    uint16 DidSize = 0;
    uint32 Offset;
    uint16 DidCfgIndex;
    uint8 TxChannelCtrlIndex;
    uint8 TxChannelCfgIndex;
    uint8 RangeDidCfgIndex;
    boolean RangeDidFlag;
    uint16 DidInfoCfgIndex;
#endif /* STD_ON == DCM_DSP_DID_FUNC_ENABLED */
    uint8 MsgCtrlId;
    Std_ReturnType ret;
    Dcm_0x2ETypes Dcm_0x2EType;
    const Dcm_DspDidRangeType* pDcmDspDidRange = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDidRange;
    Dcm_MsgContextType* pMsgContext;
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;

    RecDid = (uint16)((uint16)((uint16)pMsgContext->pReqData[1]) << 8u) | ((uint16)pMsgContext->pReqData[2]);
    TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
    Offset = (DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg)[TxChannelCfgIndex].offset;

    Dcm_0x2EType.pDidCfgIndex = &DidCfgIndex;
    Dcm_0x2EType.pRangeDidCfgIndex = &RangeDidCfgIndex;
    Dcm_0x2EType.pRangeDidFlag = &RangeDidFlag;
    Dcm_0x2EType.DidInfoCfgIndex = &DidInfoCfgIndex;
    ret = Dcm_Uds0x2EServiceConditionCheck(OpStatus, ProtocolCtrlId, &Dcm_0x2EType, ErrorCode);
#if (STD_ON == DCM_DSP_DID_FUNC_ENABLED)
    if (E_OK == ret)
    {
        if (FALSE == RangeDidFlag)
        {
            ret = DspInternalUDS0x2E_DidWriteDataLength(DidCfgIndex, &DidSize);
            if ((E_OK == ret) && ((3u + DidSize) != (uint16)pMsgContext->ReqDataLen))
            {
                /*check message length*/
                /*if the message length is not correct,send NRC 0x13*/
                *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                ret = E_NOT_OK;
            }
            if (E_OK == ret)
            {
                ret = Dcm_UDS0x2ESubCheck(ProtocolCtrlId, RecDid, DidInfoCfgIndex, ErrorCode);
            }
            if (E_OK == ret)
            {
                ret = DspInternalUDS0x2E_DidWrite(OpStatus, MsgCtrlId, DidCfgIndex, ErrorCode);
            }
        }
        else
        {
            /*read data length to be writen*/
            if (NULL_PTR == pDcmDspDidRange[RangeDidCfgIndex].DcmDspDidRangeReadDataLengthFnc)
            {
                /*DcmDspDidReadDataLengthFnc is NULL,send NRC 0x22*/
                *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
                ret = E_NOT_OK;
            }
            if (E_OK == ret)
            {
                ret = pDcmDspDidRange[RangeDidCfgIndex].DcmDspDidRangeReadDataLengthFnc(RecDid, OpStatus, &DidSize);
                if (E_NOT_OK == ret)
                {
                    *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
                    ret = E_NOT_OK;
                }
                else if (DCM_E_PENDING == ret)
                {
                    Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
                }
                else
                {
                    ret = Dcm_UDS0x2ESubCheck(ProtocolCtrlId, RecDid, DidInfoCfgIndex, ErrorCode);
                }
            }
            /*write data*/
            if ((E_OK == ret) && (NULL_PTR == pDcmDspDidRange[RangeDidCfgIndex].DcmDspDidRangeWriteDidFnc))
            {
                *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
                ret = E_NOT_OK;
            }
            if (E_OK == ret)
            {
                ret = pDcmDspDidRange[RangeDidCfgIndex]
                          .DcmDspDidRangeWriteDidFnc(RecDid, &(pMsgContext->pReqData[3]), OpStatus, DidSize, ErrorCode);
                if (DCM_E_PENDING == ret)
                {
                    Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
                }
            }
        }
    }
    if ((E_OK == ret) && ((3u) > (DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex].Dcm_DslBufferSize)))
    {
        /*assemble and send positive response*/
        /* check tx data length */
        /*Pdu length is bigger than buffer size,ignore the request message */
        *ErrorCode = DCM_E_RESPONSETOOLONG;
        ret = E_NOT_OK;
    }
    if (E_OK == ret)
    {
        Dcm_Channel[Offset] = 0x6E;
        Dcm_Channel[Offset + 1u] = (uint8)(RecDid >> 8u);
        Dcm_Channel[Offset + 2u] = (uint8)RecDid;
        pMsgContext->ResMaxDataLen = 3u;
        pMsgContext->ResDataLen = 3u;
        pMsgContext->pResData = &Dcm_Channel[Offset];
        DsdInternal_ProcessingDone(ProtocolCtrlId);
    }
#endif /* STD_ON == DCM_DSP_DID_FUNC_ENABLED */
    return ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif /* STD_ON == DCM_UDS_SERVICE0X2E_ENABLED */
