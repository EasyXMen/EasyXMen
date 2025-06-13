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
     UDS:SecurityAccess(27 hex) service
 ***************************************************************/
#if (STD_ON == DCM_UDS_SERVICE0X27_ENABLED)

typedef struct
{
    uint8 SecCfgIndex;
    Dcm_CompareKeyFncType ComparedKey;
    Dcm_GetSeedFncType GottSeed;
} Dcm_0x27Types;

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/*************************************************************************/
/*
 * Brief               <UDS  0x27 service Sub function Check>
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
#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
static FUNC(Std_ReturnType, DCM_CODE) Dcm_Uds0x27ServiceSubfunctionCheck(
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DCM_VAR) SecCfgIndex,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;
    uint8 MsgCtrlID;
    uint8 ReqSecLevel;

    /********************************/
    /*try to find the corresponding security level in configuration*/
    MsgCtrlID = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    ReqSecLevel = (Dcm_MsgCtrl[MsgCtrlID].Subfunction + 1u) / 2u;
    /*********************************************/
    if (ReqSecLevel == 0x00u)
    {
        /*if not found in DcmDspSecurityRow,send NRC 0x12*/
        *ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTED;
        ret = E_NOT_OK;
    }
    /*********************************************/
    if (E_OK == ret)
    {
        ret = DslInternal_GetSecurityCfgBySecLevel(ReqSecLevel, SecCfgIndex);
        if (E_NOT_OK == ret)
        {
            /*if not found in DcmDspSecurityRow,send NRC 0x12*/
            *ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTED;
        }
    }
#if (STD_ON == DCM_UDS_SERVICE0X29_ENABLED)
    if (E_OK == ret)
    {
        ret = DspInternal_SubServiceAuthenticationCheck(ProtocolCtrlId, ErrorCode);
    }
#endif
    return ret;
}
#endif

/*************************************************************************/
/*
 * Brief               <UDS  0x27 service Active SecLevel Check>
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
#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
static FUNC(Std_ReturnType, DCM_CODE) Dcm_Uds0x27ServiceActiveSecLevelCheck(uint8 ProtocolCtrlId, uint8 SeedSize)
{
    uint8 Index;
    Std_ReturnType ret = E_OK;
    uint8 MsgCtrlID = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    const Dcm_MsgCtrlType* pDcm_MsgCtrl = &Dcm_MsgCtrl[MsgCtrlID];
    Dcm_MsgContextType* pMsgContext = &Dcm_MsgCtrl[MsgCtrlID].MsgContext;
    uint8 TxChannelCtrlIndex = pDcm_MsgCtrl->Dcm_TxCtrlChannelIndex;
    uint8 TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
    uint32 Offset = (DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg)[TxChannelCfgIndex].offset;
    uint8 SubFunction = pDcm_MsgCtrl->Subfunction;
    uint8 ReqSecLevel = (SubFunction + 1u) / 2u;

    if (ReqSecLevel == Dcm_SecCtrl.Dcm_ActiveSec)
    {
        /*if the server is on the request security level,set seed to zero*/
        Dcm_Channel[Offset] = 0x67;
        Dcm_Channel[Offset + 1u] = SubFunction;
        for (Index = 0; Index < SeedSize; Index++)
        {
            Dcm_Channel[Offset + (uint16)Index + 2u] = 0x00u;
        }
        pMsgContext->ResMaxDataLen = ((uint32)SeedSize + 2u);
        pMsgContext->ResDataLen = ((uint32)SeedSize + 2u);
        pMsgContext->pResData = &Dcm_Channel[Offset];
        DsdInternal_ProcessingDone(ProtocolCtrlId);
        ret = DCM_E_PENDING;
    }
    return ret;
}
#endif

/*************************************************************************/
/*
 * Brief               <UDS  0x27 service Service MsgLen Check>
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
#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
static FUNC(Std_ReturnType, DCM_CODE) Dcm_Uds0x27ServiceMsgLenCheck(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    uint8 SecCfgIndex,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 ReqCfgLen;
    Std_ReturnType ret = E_OK;
    uint8 ReqLen;
    uint8 AdrSize;
    uint8 KeySize;
    const Dcm_DspSecurityRowType* pSecurityRow;
    uint8 MsgCtrlID;
    uint8 SubFunction;
    const Dcm_DspSecurityType* pDcm_DspSecurity = DcmPbCfgPtr->pDcmDspCfg->pDcm_DspSecurity;
    Dcm_SecTimerCtrlType* pDcm_RunDlyCtrl = &Dcm_SecCtrl.Dcm_RunDlyCtrl;
    uint8* pFalseAcessCount = &Dcm_SecCtrl.Dcm_FalseAcessCount[SecCfgIndex];

    pSecurityRow = &(pDcm_DspSecurity->pDcm_DspSecurityRow[SecCfgIndex]);

    MsgCtrlID = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    SubFunction = Dcm_MsgCtrl[MsgCtrlID].Subfunction;
    AdrSize = pSecurityRow->DcmDspSecurityADRSize;
    KeySize = pSecurityRow->DcmDspSecurityKeySize;
    ReqLen = (uint8)(Dcm_MsgCtrl[MsgCtrlID].MsgContext.ReqDataLen);

    if (1u == (SubFunction & 0x01u))
    {
        ReqCfgLen = (uint8)(AdrSize + 2u);
    }
    else
    {
        ReqCfgLen = (uint8)(KeySize + 2u);
    }
    if (ReqLen != ReqCfgLen)
    {
        /*if the request message length is not correct,send NRC 0x13*/
        /*deal with 'send key'*/
        if ((0x01u != (SubFunction & 0x01u)) && (SubFunction == (Dcm_SecCtrl.Dcm_SubfunctionForSeed + 1u))
            && (Dcm_SecCtrl.Dcm_SecServiceState == DCM_SERVICE_SEED))
        {
            *pFalseAcessCount += (uint8)1;
            if (*pFalseAcessCount >= pSecurityRow->DcmDspSecurityNumAttDelay)
            {
                Dcm_ResetTime(&pDcm_RunDlyCtrl->Dcm_SecCurTimer[SecCfgIndex]);
                pDcm_RunDlyCtrl->Dcm_SecExpiredTimer[SecCfgIndex] = pSecurityRow->DcmDspSecurityDelayTime;
                pDcm_RunDlyCtrl->Dcm_SecTimerState[SecCfgIndex] = DCM_SECTIMER_ON;
            }
            if ((TRUE == pSecurityRow->DcmDspSecurityAttemptCounterEnabled)
                && ((USE_ASYNCH_FNC == pSecurityRow->DcmDspSecurityUsePort)
                    || (USE_ASYNCH_CLIENT_SERVER == pSecurityRow->DcmDspSecurityUsePort)))
            {
                if (pSecurityRow->Dcm_SetSecurityAttemptCounterFnc != NULL_PTR)
                {
                    (void)pSecurityRow->Dcm_SetSecurityAttemptCounterFnc(OpStatus, *pFalseAcessCount);
                }
            }
        }
        DslInternal_SetSecurityAccessStatus(DCM_SERVICE_IDLE);
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        ret = E_NOT_OK;
    }
    return ret;
}
#endif

/*************************************************************************/
/*
 * Brief               <UDS  0x27 service Sub Request Seed>
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
#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
static FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x27_SubReqSeed(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2CONST(Dcm_0x27Types, AUTOMATIC, DCM_VAR) Dcm_0x27Type,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;
    uint8 Index;
    uint8 MsgCtrlID = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    const Dcm_MsgCtrlType* pDcm_MsgCtrl = &Dcm_MsgCtrl[MsgCtrlID];
    uint8 TxChannelCtrlIndex = pDcm_MsgCtrl->Dcm_TxCtrlChannelIndex;
    uint8 TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
    const Dcm_DslBufferType* pDcmChannelCfg = &DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex];
    uint32 Offset = pDcmChannelCfg->offset;
    uint8 SubFunction = pDcm_MsgCtrl->Subfunction;
    const Dcm_DspSecurityRowType* pSecurityRow;
    Dcm_MsgContextType* pMsgContext = &Dcm_MsgCtrl[MsgCtrlID].MsgContext;
    static uint8 Last_Seed[DCM_SEED_MAX_SIZE] = {0};
#if (DCM_SECURITY_ADR_MAXSIZE > 0)
    uint8 Dcm_Adr[DCM_SECURITY_ADR_MAXSIZE] = {0};
    uint8 AdrSize;
#endif
    uint8 SeedSize;
    const Dcm_DspSecurityType* pDcm_DspSecurity = DcmPbCfgPtr->pDcmDspCfg->pDcm_DspSecurity;

    pSecurityRow = &(pDcm_DspSecurity->pDcm_DspSecurityRow[Dcm_0x27Type->SecCfgIndex]);
#if (DCM_SECURITY_ADR_MAXSIZE > 0)
    AdrSize = pSecurityRow->DcmDspSecurityADRSize;
#endif
    SeedSize = pSecurityRow->DcmDspSecuritySeedSize;
    /*check whether the Continuous request*/

    /*to avoid requirement consecutive seed request,
     * same seed is returned.there to clear ServiceState*/
    if (DCM_SEC_CALL_OVER_MASK != (Dcm_SecCtrl.Dcm_SecFlag & DCM_SEC_CALL_OVER_MASK))
    {
        *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
        ret = E_NOT_OK;
    }
    else
    {
        /*check whether the security service state is idle*/
        ret = Dcm_Uds0x27ServiceActiveSecLevelCheck(ProtocolCtrlId, SeedSize);
    }
    if (E_OK == ret)
    {
        /*if the server is on the other security level,
         * check the size of security access data record*/
#if (DCM_SECURITY_ADR_MAXSIZE > 0)
        if (0u == AdrSize)
        {
#endif
            if (DCM_SERVICE_IDLE == Dcm_SecCtrl.Dcm_SecServiceState)
            {
                ret = (*Dcm_0x27Type->GottSeed)(
#if (DCM_SECURITY_ADR_MAXSIZE > 0)
                    NULL_PTR,
#endif
                    OpStatus,
                    &Dcm_Channel[Offset + 2u],
                    ErrorCode);
                if (DCM_E_PENDING == ret)
                {
                    Dcm_MsgCtrl[MsgCtrlID].Dcm_OpStatus = DCM_PENDING;
                }
                for (Index = 0; Index < SeedSize; Index++)
                {
                    Last_Seed[Index] = Dcm_Channel[Offset + 2u + (uint32)Index];
                }
            }
            else
            {
                for (Index = 0; Index < SeedSize; Index++)
                {
                    Dcm_Channel[Offset + 2u + (uint32)Index] = Last_Seed[Index];
                }
            }
#if (DCM_SECURITY_ADR_MAXSIZE > 0)
        }
        else
        { /*read-out ADR  from request message*/
            for (Index = 0; Index < AdrSize; Index++)
            {
                Dcm_Adr[Index] = (pMsgContext->pReqData)[Index + 2u];
            }
            if (DCM_SERVICE_IDLE == Dcm_SecCtrl.Dcm_SecServiceState)
            {
                ret = (*Dcm_0x27Type->GottSeed)(&Dcm_Adr[0], OpStatus, &Dcm_Channel[Offset + 2u], ErrorCode);
                if (DCM_E_PENDING == ret)
                {
                    Dcm_MsgCtrl[MsgCtrlID].Dcm_OpStatus = DCM_PENDING;
                }
            }
        }
#endif
    }
    if ((E_OK == ret) && ((0x02u + (uint32)SeedSize) > pDcmChannelCfg->Dcm_DslBufferSize))
    {
        /* check tx data length */
        /*Pdu length is bigger than buffer size,ignore the request message */
        *ErrorCode = DCM_E_RESPONSETOOLONG;
        ret = E_NOT_OK;
    }
    if (E_OK == ret)
    {
        /* send message */
        Dcm_Channel[Offset] = 0x67;
        Dcm_Channel[Offset + 1u] = SubFunction;
        pMsgContext->ResMaxDataLen = ((uint32)SeedSize + 2UL);
        pMsgContext->ResDataLen = ((uint32)SeedSize + 2UL);
        pMsgContext->pResData = &Dcm_Channel[Offset];
        DsdInternal_ProcessingDone(ProtocolCtrlId);
        /*seed has been sent,set the status to DCM_SERVICE_SEED*/
        Dcm_SecCtrl.Dcm_SecServiceState = DCM_SERVICE_SEED;
        Dcm_SecCtrl.Dcm_SubfunctionForSeed = SubFunction;
    }
    return ret;
}
#endif
/*************************************************************************/
/*
 * Brief               <UDS  0x27 service Sub Compare Key>
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
#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
static FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x27_SubCompareKey(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2CONST(Dcm_0x27Types, AUTOMATIC, DCM_VAR) Dcm_0x27Type,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;
    uint8 MsgCtrlID = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    uint8 TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlID].Dcm_TxCtrlChannelIndex;
    uint8 TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
    const Dcm_DslBufferType* pDcmChannelCfg = &DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex];
    uint32 Offset = pDcmChannelCfg->offset;
    uint8 SubFunction = Dcm_MsgCtrl[MsgCtrlID].Subfunction;
    uint8 ReqSecLevel = (SubFunction + 1u) / 2u;
    const Dcm_DspSecurityRowType* pSecurityRow;
    const Dcm_DspSecurityType* pDcm_DspSecurity = DcmPbCfgPtr->pDcmDspCfg->pDcm_DspSecurity;
    Dcm_MsgContextType* pMsgContext = &Dcm_MsgCtrl[MsgCtrlID].MsgContext;
    uint8 SecCfgIndex = Dcm_0x27Type->SecCfgIndex;
    uint8* FalseAcessCount = &Dcm_SecCtrl.Dcm_FalseAcessCount[SecCfgIndex];
    Dcm_SecTimerCtrlType* Dcm_RunDlyCtrl = &Dcm_SecCtrl.Dcm_RunDlyCtrl;
    pSecurityRow = &(pDcm_DspSecurity->pDcm_DspSecurityRow[SecCfgIndex]);

    if ((SubFunction <= Dcm_SecCtrl.Dcm_SubfunctionForSeed)
        || (1u != (SubFunction - Dcm_SecCtrl.Dcm_SubfunctionForSeed)))
    {
        /*'requestSeed' and 'seedKey' don't  match*/
        *FalseAcessCount += (uint8)1;
        if (*FalseAcessCount >= pSecurityRow->DcmDspSecurityNumAttDelay)
        {
            Dcm_ResetTime(&Dcm_RunDlyCtrl->Dcm_SecCurTimer[SecCfgIndex]);
            Dcm_RunDlyCtrl->Dcm_SecExpiredTimer[SecCfgIndex] = pSecurityRow->DcmDspSecurityDelayTime;
            Dcm_RunDlyCtrl->Dcm_SecTimerState[SecCfgIndex] = DCM_SECTIMER_ON;
        }
        if (((TRUE == pSecurityRow->DcmDspSecurityAttemptCounterEnabled)
             && ((USE_ASYNCH_FNC == pSecurityRow->DcmDspSecurityUsePort)
                 || (USE_ASYNCH_CLIENT_SERVER == pSecurityRow->DcmDspSecurityUsePort)))
            && (pSecurityRow->Dcm_SetSecurityAttemptCounterFnc != NULL_PTR))
        {
            (void)pSecurityRow->Dcm_SetSecurityAttemptCounterFnc(OpStatus, *FalseAcessCount);
        }
        DslInternal_SetSecurityAccessStatus(DCM_SERVICE_IDLE);
        *ErrorCode = DCM_E_REQUESTSEQUENCEERROR;
        ret = E_NOT_OK;
    }
    if (E_OK == ret)
    {
        /*if 'requestSeed' and 'seedKey' match,compare the key*/
        ret = (*Dcm_0x27Type->ComparedKey)(&(pMsgContext->pReqData[2]), OpStatus, ErrorCode);
        if (DCM_E_PENDING == ret)
        {
            Dcm_MsgCtrl[MsgCtrlID].Dcm_OpStatus = DCM_PENDING;
        }
        else if (E_COMPARE_KEY_FAILED == ret)
        { /*'compare key' is failed,increase the false count*/
            Dcm_SecCtrl.Dcm_SubfunctionForSeed = 0u;
            *FalseAcessCount += (uint8)1;
            if ((TRUE == pSecurityRow->DcmDspSecurityAttemptCounterEnabled)
                && ((USE_ASYNCH_FNC == pSecurityRow->DcmDspSecurityUsePort)
                    || (USE_ASYNCH_CLIENT_SERVER == pSecurityRow->DcmDspSecurityUsePort))
                && (pSecurityRow->Dcm_SetSecurityAttemptCounterFnc != NULL_PTR))
            {
                (void)pSecurityRow->Dcm_SetSecurityAttemptCounterFnc(OpStatus, *FalseAcessCount);
            }
            if (*FalseAcessCount >= pSecurityRow->DcmDspSecurityNumAttDelay)
            {
                Dcm_ResetTime(&Dcm_RunDlyCtrl->Dcm_SecCurTimer[SecCfgIndex]);
                Dcm_RunDlyCtrl->Dcm_SecExpiredTimer[SecCfgIndex] = pSecurityRow->DcmDspSecurityDelayTime;
                Dcm_RunDlyCtrl->Dcm_SecTimerState[SecCfgIndex] = DCM_SECTIMER_ON;
                DslInternal_SetSecurityAccessStatus(DCM_SERVICE_IDLE);
                *ErrorCode = DCM_E_EXCEEDNUMBEROFATTEMPTS; /*NRC=0x36*/
                ret = E_NOT_OK;
            }
            else
            {
                DslInternal_SetSecurityAccessStatus(DCM_SERVICE_IDLE);
                *ErrorCode = DCM_E_INVALIDKEY; /*NRC=0x35*/
                ret = E_NOT_OK;
            }
        }
        else
        {
            /*idle*/
        }
    }
    if ((E_OK == ret) && ((2u) > pDcmChannelCfg->Dcm_DslBufferSize))
    {
        /* check tx data length */
        /*Pdu length is bigger than buffer size,ignore the request message */
        *ErrorCode = DCM_E_RESPONSETOOLONG;
        ret = E_NOT_OK;
    }
    /* send message*/
    if (E_OK == ret)
    {
        Dcm_Channel[Offset] = 0x67;
        Dcm_Channel[Offset + 1u] = SubFunction;
        pMsgContext->ResMaxDataLen = 2UL;
        pMsgContext->ResDataLen = 2UL;
        pMsgContext->pResData = &Dcm_Channel[Offset];
        Dcm_SecCtrl.Dcm_NewSec = ReqSecLevel;
        Dcm_SecCtrl.Dcm_SecServiceState = DCM_SERVICE_KEY;
        DsdInternal_ProcessingDone(ProtocolCtrlId);
    }
    return ret;
}
#endif
/*************************************************************************/
/*
 * Brief               <UDS  0x27 service>
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
Dcm_UDS0x27(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432,1532-- */ /* MISRA Rule 20.7,8.7 */
{
#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
    uint8 SubFunction;
    uint8 MsgCtrlID;
    uint8 SecCfgIndex = 0u;
    Dcm_CompareKeyFncType ComparedKey = NULL_PTR;
    Dcm_GetSeedFncType GottSeed = NULL_PTR;
    Dcm_0x27Types Dcm_0x27Type;
#endif
    Std_ReturnType ret;
    const Dcm_DspSecurityType* pDcm_DspSecurity = DcmPbCfgPtr->pDcmDspCfg->pDcm_DspSecurity;
    const Dcm_DspSecurityRowType* pDcm_DspSecurityRow;

    MsgCtrlID = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    SubFunction = Dcm_MsgCtrl[MsgCtrlID].Subfunction;
    ret = Dcm_Uds0x27ServiceSubfunctionCheck(ProtocolCtrlId, &SecCfgIndex, ErrorCode);
#if (STD_ON == DCM_SESSION_FUNC_ENABLED)
    if (E_OK == ret)
    {
        /*session check,check whether the current session supports the request service*/
        ret = DsdInternal_SubSesCheck(ProtocolCtrlId, SID_SECURITY_ACCESS, ErrorCode);
    }
#endif
#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
    if (E_OK == ret)
    {
        /*security check,check whether the current security supports the request service*/
        ret = DsdInternal_SubSecurityCheck(ProtocolCtrlId, SID_SECURITY_ACCESS, ErrorCode);
    }
#endif

    /*check the status*/
    if ((E_OK == ret) && (DCM_SERVICE_SEED != Dcm_SecCtrl.Dcm_SecServiceState) && (0x01u != (SubFunction & 0x01u)))
    {
        /*if the server is not on the status DCM_SERVICE_SEED,
         * send NRC 0x24:request sequence error*/
        DslInternal_SetSecurityAccessStatus(DCM_SERVICE_IDLE);
        *ErrorCode = DCM_E_REQUESTSEQUENCEERROR;
        ret = E_NOT_OK;
    }

    if (E_OK == ret)
    {
        /*check the request message length*/
        ret = Dcm_Uds0x27ServiceMsgLenCheck(OpStatus, ProtocolCtrlId, SecCfgIndex, ErrorCode);
    }

#if (STD_OFF == DCM_SECURITY_FUNC_ENABLED)
    if (E_OK == ret)
    {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        ret = E_NOT_OK;
    }
#else
    /*if the delay timer is active and a request is transmitted,send NRC 0x37*/
    if ((E_OK == ret) && (DCM_SECTIMER_ON == Dcm_SecCtrl.Dcm_RunDlyCtrl.Dcm_SecTimerState[SecCfgIndex])
        && (Dcm_SecCtrl.Dcm_FalseAcessCount[SecCfgIndex]
            >= pDcm_DspSecurity->pDcm_DspSecurityRow[SecCfgIndex].DcmDspSecurityNumAttDelay))
    {
        *ErrorCode = DCM_E_REQUIREDTIMEDELAYNOTEXPIRED;
        ret = E_NOT_OK;
    }
    /*check whether the corresponding callouts are configured*/
    if (E_OK == ret)
    {
        pDcm_DspSecurityRow = &pDcm_DspSecurity->pDcm_DspSecurityRow[SecCfgIndex];
        if ((USE_ASYNCH_FNC == pDcm_DspSecurityRow->DcmDspSecurityUsePort)
            || (USE_ASYNCH_CLIENT_SERVER == pDcm_DspSecurityRow->DcmDspSecurityUsePort))
        {
            ComparedKey = pDcm_DspSecurityRow->Dcm_CompareKeyFnc;
            GottSeed = pDcm_DspSecurityRow->Dcm_GetSeedFnc;
        }
        if ((NULL_PTR == ComparedKey) || (NULL_PTR == GottSeed))
        {
            /*if not configured,send NRC 0x22*/
            *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
            ret = E_NOT_OK;
        }
    }
    if (E_OK == ret)
    {
        /*deal with 'request seed'*/
        if (0x01u == (SubFunction & 0x01u))
        {
            Dcm_0x27Type.GottSeed = GottSeed;
            Dcm_0x27Type.SecCfgIndex = SecCfgIndex;
            ret = Dcm_UDS0x27_SubReqSeed(OpStatus, ProtocolCtrlId, &Dcm_0x27Type, ErrorCode);
        } /*ENDOF if(0x01 == (SubFunction & 0x01u))*/
        /*deal with 'send key'*/
        else
        {
            Dcm_0x27Type.ComparedKey = ComparedKey;
            Dcm_0x27Type.SecCfgIndex = SecCfgIndex;
            ret = Dcm_UDS0x27_SubCompareKey(OpStatus, ProtocolCtrlId, &Dcm_0x27Type, ErrorCode);
        }
    }
    return ret;
#endif
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
