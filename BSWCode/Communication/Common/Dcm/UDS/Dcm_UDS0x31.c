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
                 UDS:RoutineControl (31 hex) service
 ***************************************************************/
#if (STD_ON == DCM_UDS_SERVICE0X31_ENABLED)
typedef struct
{
    boolean Find;
    Dcm_OpStatusType OpStatus;
    uint8* pRoutineCfgId;
    uint32* ResLen;
    uint16 currentDataLength;
} Dcm_0x31Types;

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
#if (STD_ON == DCM_DSP_ROUTINE_FUNC_ENABLED)
static FUNC(Std_ReturnType, DCM_CODE) Dcm_UdsSesAndSecCheck(
#if (STD_ON == DCM_UDS_SERVICE0X29_ENABLED)
    uint8 ProtocolCtrlId,
    uint8 RoutineCfgIdx,
#endif
    P2CONST(Dcm_DspCommonAuthorizationType, AUTOMATIC, DCM_CONST) pDspCommonAuthorization,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
#if ((STD_ON == DCM_SESSION_FUNC_ENABLED) || (STD_ON == DCM_SECURITY_FUNC_ENABLED))
    uint8 Index;
    boolean Flag = FALSE;
#endif
#if (STD_ON == DCM_SESSION_FUNC_ENABLED)
    uint8 RoutineSesNum;
#endif
#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
    uint8 RoutineSecNum;
#endif
    Std_ReturnType ret = E_OK;

#if (STD_ON == DCM_SESSION_FUNC_ENABLED)
    if (NULL_PTR != pDspCommonAuthorization)
    {
        RoutineSesNum = pDspCommonAuthorization->DcmDspCommonAuthorizationSessionRefNum;
        if (RoutineSesNum != 0u)
        {
            for (Index = 0; (Index < RoutineSesNum) && (FALSE == Flag); Index++)
            {
                if (Dcm_MkCtrl.Dcm_ActiveSes == pDspCommonAuthorization->DcmDspCommonAuthorizationSessionRef[Index])
                {
                    Flag = TRUE;
                }
            }
            if (FALSE == Flag)
            {
                *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
                ret = E_NOT_OK;
            }
        }
    }
#endif
#if (STD_ON == DCM_UDS_SERVICE0X29_ENABLED)
    if (E_OK == ret)
    {
        ret = DspInternal_RidAuthenticationCheck(ProtocolCtrlId, RoutineCfgIdx, ErrorCode);
    }
#endif
    /*check security*/
#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
    if (NULL_PTR != pDspCommonAuthorization)
    {
        Flag = FALSE;
        RoutineSecNum = pDspCommonAuthorization->DcmDspCommonAuthorizationSecurityLevelRefNum;
        if ((RoutineSecNum != 0u) && (E_OK == ret))
        {
            for (Index = 0; (Index < RoutineSecNum) && (FALSE == Flag); Index++)
            {
                if (Dcm_MkCtrl.Dcm_ActiveSec
                    == pDspCommonAuthorization->DcmDspCommonAuthorizationSecurityLevelRef[Index])
                {
                    Flag = TRUE;
                }
            }
            if (FALSE == Flag)
            {
                /*the processing is not supported in current security level,send NRC 0x33*/
                *ErrorCode = DCM_E_SECURITYACCESSDENIED;
                ret = E_NOT_OK;
            }
        }
    }
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
static FUNC(Std_ReturnType, DCM_CODE) Dcm_UdsRCSesAndSecCheck(
    uint8 ProtocolCtrlId,
    uint8 RoutineCfgIdx,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    const Dcm_DspCommonAuthorizationType* pDspRoutineAuthorization;
    const Dcm_DspRoutineType* pDcmDspRoutine = &DcmPbCfgPtr->pDcmDspCfg->pDcmDspRoutine[RoutineCfgIdx];
#if (DCM_UDS0X31_STARTROUTINE_ENABLED == STD_ON)
    const Dcm_DspCommonAuthorizationType* pDspStartRoutineAuthorization;
#endif
#if (DCM_UDS0X31_STOPROUTINE_ENABLED == STD_ON)
    const Dcm_DspCommonAuthorizationType* pDspStopRoutineAuthorization;
#endif
#if (DCM_UDS0X31_REQUESTROUTINERESULTS_ENABLED == STD_ON)
    const Dcm_DspCommonAuthorizationType* pDspRequestRoutineResultsAuthorization;
    const Dcm_DspRequestRoutineResultsType* pDcmDspRequestRoutineResults = pDcmDspRoutine->DcmDspRequestRoutineResults;
#endif
    Std_ReturnType ret = E_OK;
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    uint8 SubFunction = Dcm_MsgCtrl[MsgCtrlId].Subfunction;

    pDspRoutineAuthorization = pDcmDspRoutine->DcmDspCommonAuthorizationRef;
    ret = Dcm_UdsSesAndSecCheck(
#if (STD_ON == DCM_UDS_SERVICE0X29_ENABLED)
        ProtocolCtrlId,
        RoutineCfgIdx,
#endif
        pDspRoutineAuthorization,
        ErrorCode);

#if (DCM_UDS0X31_STARTROUTINE_ENABLED == STD_ON)
    if ((E_OK == ret) && (SubFunction == DCM_UDS0X31_STARTROUTINE) && (pDcmDspRoutine->DcmDspStartRoutine != NULL_PTR))
    {
        pDspStartRoutineAuthorization = pDcmDspRoutine->DcmDspStartRoutine->DcmDspStartRoutineCommonAuthorizationRef;
        ret = Dcm_UdsSesAndSecCheck(
#if (STD_ON == DCM_UDS_SERVICE0X29_ENABLED)
            ProtocolCtrlId,
            RoutineCfgIdx,
#endif
            pDspStartRoutineAuthorization,
            ErrorCode);
    }
#endif /* DCM_UDS0X31_STARTROUTINE_ENABLED == STD_ON */
#if (DCM_UDS0X31_STOPROUTINE_ENABLED == STD_ON)
    if ((E_OK == ret) && (SubFunction == DCM_UDS0X31_STOPROUTINE) && (pDcmDspRoutine->DcmDspStopRoutine != NULL_PTR))
    {
        pDspStopRoutineAuthorization = pDcmDspRoutine->DcmDspStopRoutine->DcmDspStopRoutineCommonAuthorizationRef;
        ret = Dcm_UdsSesAndSecCheck(
#if (STD_ON == DCM_UDS_SERVICE0X29_ENABLED)
            ProtocolCtrlId,
            RoutineCfgIdx,
#endif
            pDspStopRoutineAuthorization,
            ErrorCode);
    }
#endif /* DCM_UDS0X31_STOPROUTINE_ENABLED == STD_ON */
#if (DCM_UDS0X31_REQUESTROUTINERESULTS_ENABLED == STD_ON)
    if ((E_OK == ret) && (SubFunction == DCM_UDS0X31_REQUESTROUTINERESULTS)
        && (pDcmDspRequestRoutineResults != NULL_PTR))
    {
        pDspRequestRoutineResultsAuthorization =
            pDcmDspRequestRoutineResults->DcmDspRequestRoutineResultsCommonAuthorizationRef;
        ret = Dcm_UdsSesAndSecCheck(
#if (STD_ON == DCM_UDS_SERVICE0X29_ENABLED)
            ProtocolCtrlId,
            RoutineCfgIdx,
#endif
            pDspRequestRoutineResultsAuthorization,
            ErrorCode);
    }
#endif /*DCM_UDS0X31_REQUESTROUTINERESULTS_ENABLED == STD_ON  */
    if ((E_OK == ret)
#if (DCM_UDS0X31_REQUESTROUTINERESULTS_ENABLED == STD_ON)
        && (pDcmDspRequestRoutineResults == NULL_PTR)
#endif
#if (DCM_UDS0X31_STOPROUTINE_ENABLED == STD_ON)
        && (pDcmDspRoutine->DcmDspStopRoutine == NULL_PTR)
#endif
#if (STD_ON == DCM_UDS0X31_STARTROUTINE_ENABLED)
        && (pDcmDspRoutine->DcmDspStartRoutine == NULL_PTR)
#endif
    )
    {
        *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
        ret = E_NOT_OK;
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
#if ((STD_ON == DCM_DSP_ROUTINE_FUNC_ENABLED) & (DCM_UDS0X31_STARTROUTINE_ENABLED == STD_ON))
static FUNC(uint32, DCM_CODE) Dcm_UdsRCStartRoutine(
    uint8 MsgCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_0x31Types, AUTOMATIC, DCM_VAR) Dcm_0x31Type,
    P2VAR(Std_ReturnType, AUTOMATIC, DCM_VAR) ret,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint32 ReqLen = 0;
    uint32 ResLen = 0;
    uint8 Index;
    Dcm_StartRoutineFncType StartRoutineFnc;
    const Dcm_DspStartRoutineType* StartRoutine;
    const uint8* pInbuffer;
    uint8* pOutBuffer;
    uint8 TxCtrlChannelIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    uint32 Offset =
        (DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg)[Dcm_ChannelCtrl[TxCtrlChannelIndex].Dcm_ChannelCfgIndex].offset;
    const Dcm_DspRoutineType* pDcmDspRoutine = DcmPbCfgPtr->pDcmDspCfg->pDcmDspRoutine;
    uint8 InSignalNum;
    uint8 OutSignalNum;
    const uint8* pRoutineCfgId = Dcm_0x31Type->pRoutineCfgId;
    StartRoutine = pDcmDspRoutine[*pRoutineCfgId].DcmDspStartRoutine;
    if (NULL_PTR == StartRoutine)
    {
        /*StartRoutineFnc is NULL,send NRC 0x12*/
        *ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTED;
        *ret = E_NOT_OK;
    }
    if (E_OK == *ret)
    {
        if (NULL_PTR != StartRoutine->DcmDspStartRoutineIn)
        {
            InSignalNum = StartRoutine->DcmDspStartRoutineIn->RoutineInOutSignalNum;
            for (Index = 0; Index < InSignalNum; Index++)
            {
                uint16 length =
                    StartRoutine->DcmDspStartRoutineIn->DcmDspRoutineInOutSignal[Index].DcmDspRoutineSignalLength;
                ReqLen += (uint32)length;
            }
        }
        else
        {
            ReqLen = 0;
        }
        if (NULL_PTR != StartRoutine->DcmDspStartRoutineOut)
        {
            OutSignalNum = StartRoutine->DcmDspStartRoutineOut->RoutineInOutSignalNum;
            for (Index = 0; Index < OutSignalNum; Index++)
            {
                uint16 length =
                    StartRoutine->DcmDspStartRoutineOut->DcmDspRoutineInOutSignal[Index].DcmDspRoutineSignalLength;
                ResLen += (uint32)length;
            }
        }
        else
        {
            ResLen = 0;
        }

        /*check the request message length*/
        if ((NULL_PTR != StartRoutine->DcmDspStartRoutineIn)
            && (StartRoutine->DcmDspStartRoutineIn
                    ->DcmDspRoutineInOutSignal[StartRoutine->DcmDspStartRoutineIn->RoutineInOutSignalNum - 1u]
                    .DcmDspRoutineSignalType
                == DCM_VARIABLE_LENGTH))
        {
            if ((ReqLen + DCM_UDS0X31_REQ_DATA_MINLENGTH) > Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
            {
                /*the request message length is not correct,send NRC 0x13*/
                *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                *ret = E_NOT_OK;
            }
        }
        else
        {
            if ((ReqLen + DCM_UDS0X31_REQ_DATA_MINLENGTH) != Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
            {
                /*the request message length is not correct,send NRC 0x13*/
                *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                *ret = E_NOT_OK;
            }
        }
    }

    if (E_OK == *ret)
    {
        StartRoutineFnc = StartRoutine->DcmDspStartRoutineFnc;
        if (NULL_PTR == StartRoutineFnc)
        {
            /*StartRoutineFnc is NULL,send NRC 0x31*/
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            *ret = E_NOT_OK;
        }
    }
#if (STD_ON == DCM_DSP_ROUTINE_CHECK_RESTART)
    if ((E_OK == *ret) && (DCM_DSP_ROUTINE_ON == Dcm_RoutineControlState[*pRoutineCfgId]))
    {
        /*the request sequence error,send NRC 0x24*/
        *ErrorCode = DCM_E_REQUESTSEQUENCEERROR;
        *ret = E_NOT_OK;
    }
#endif /* STD_ON == DCM_DSP_ROUTINE_CHECK_RESTART */
    if (E_OK == *ret)
    {
        pInbuffer = NULL_PTR;
        if (0u != ReqLen)
        {
            pInbuffer = &Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[4];
        }
        pOutBuffer = NULL_PTR;
        if (0u != ResLen)
        {
            pOutBuffer = &Dcm_Channel[Offset + 4u];
        }
        Dcm_0x31Type->currentDataLength =
            (uint16)(Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen - ReqLen - DCM_UDS0X31_REQ_DATA_MINLENGTH);
        *ret = (*StartRoutineFnc)(
            pInbuffer,
            Dcm_0x31Type->OpStatus,
            pOutBuffer,
            &Dcm_0x31Type->currentDataLength,
            ErrorCode);
        if ((NULL_PTR != StartRoutine->DcmDspStartRoutineOut)
            && (StartRoutine->DcmDspStartRoutineOut
                    ->DcmDspRoutineInOutSignal[StartRoutine->DcmDspStartRoutineOut->RoutineInOutSignalNum - 1u]
                    .DcmDspRoutineSignalType
                == DCM_VARIABLE_LENGTH))
        {
            ResLen += Dcm_0x31Type->currentDataLength;
        }
        if (E_OK == *ret)
        {
            Dcm_RoutineControlState[*pRoutineCfgId] = DCM_DSP_ROUTINE_ON;
        }
    }
    return ResLen;
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
#if ((STD_ON == DCM_DSP_ROUTINE_FUNC_ENABLED) && (DCM_UDS0X31_STOPROUTINE_ENABLED == STD_ON))
static FUNC(uint32, DCM_CODE) Dcm_UdsRCStopRoutine(
    uint8 MsgCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_0x31Types, AUTOMATIC, DCM_VAR) Dcm_0x31Type,
    P2VAR(Std_ReturnType, AUTOMATIC, DCM_VAR) ret,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint32 ReqLen = 0;
    uint32 ResLen = 0;
    uint8 Index;
    const Dcm_DspStopRoutineType* StopRoutine;
    Dcm_StopRoutineFncType StopRoutineFnc;
    const uint8* pInbuffer;
    uint8* pOutBuffer;
    uint32 Offset =
        DcmPbCfgPtr->pDcmDslCfg
            ->pDcmChannelCfg[Dcm_ChannelCtrl[Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex].Dcm_ChannelCfgIndex]
            .offset;
    const Dcm_DspRoutineType* pDcmDspRoutine = DcmPbCfgPtr->pDcmDspCfg->pDcmDspRoutine;
    uint8 InSignalNum;
    uint8 OutSignalNum;

    if (pDcmDspRoutine[*Dcm_0x31Type->pRoutineCfgId].DcmDspStopRoutine == NULL_PTR)
    {
        /*StopRoutineFnc is NULL,send NRC 0x12*/
        *ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTED;
        *ret = E_NOT_OK;
    }

    if (E_OK == *ret)
    {
        StopRoutine = pDcmDspRoutine[*Dcm_0x31Type->pRoutineCfgId].DcmDspStopRoutine;
        if (StopRoutine->DcmDspStopRoutineIn != NULL_PTR)
        {
            InSignalNum = StopRoutine->DcmDspStopRoutineIn->RoutineInOutSignalNum;
            for (Index = 0; Index < InSignalNum; Index++)
            {
                uint16 length =
                    StopRoutine->DcmDspStopRoutineIn->DcmDspRoutineInOutSignal[Index].DcmDspRoutineSignalLength;
                ReqLen += (uint32)length;
            }
        }
        else
        {
            ReqLen = 0;
        }
        if (StopRoutine->DcmDspStopRoutineOut != NULL_PTR)
        {
            OutSignalNum = StopRoutine->DcmDspStopRoutineOut->RoutineInOutSignalNum;
            for (Index = 0; Index < OutSignalNum; Index++)
            {
                uint16 length =
                    StopRoutine->DcmDspStopRoutineOut->DcmDspRoutineInOutSignal[Index].DcmDspRoutineSignalLength;
                ResLen += (uint32)length;
            }
        }
        else
        {
            ResLen = 0;
        }

        if ((NULL_PTR != StopRoutine->DcmDspStopRoutineIn)
            && (StopRoutine->DcmDspStopRoutineIn
                    ->DcmDspRoutineInOutSignal[StopRoutine->DcmDspStopRoutineIn->RoutineInOutSignalNum - 1u]
                    .DcmDspRoutineSignalType
                == DCM_VARIABLE_LENGTH))
        {
            if ((ReqLen + DCM_UDS0X31_REQ_DATA_MINLENGTH) > Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
            {
                /*the request message length is not correct,send NRC 0x13*/
                *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                *ret = E_NOT_OK;
            }
        }
        else
        {
            if ((ReqLen + DCM_UDS0X31_REQ_DATA_MINLENGTH) != Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
            {
                /*the request message length is not correct,send NRC 0x13*/
                *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                *ret = E_NOT_OK;
            }
        }
    }

    if (E_OK == *ret)
    {
        StopRoutineFnc = StopRoutine->DcmDspRoutineStopFnc;
        if (NULL_PTR == StopRoutineFnc)
        {
            /*StopRoutineFnc is NULL,send NRC 0x31*/
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            *ret = E_NOT_OK;
        }
    }

    if ((E_OK == *ret) && (DCM_DSP_ROUTINE_ON != Dcm_RoutineControlState[*Dcm_0x31Type->pRoutineCfgId]))
    {
        /*the request sequence error,send NRC 0x24*/
        *ErrorCode = DCM_E_REQUESTSEQUENCEERROR;
        *ret = E_NOT_OK;
    }
    if (E_OK == *ret)
    {
        pInbuffer = NULL_PTR;
        if (0u != ReqLen)
        {
            pInbuffer = &Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[4];
        }
        pOutBuffer = NULL_PTR;
        if (0u != ResLen)
        {
            pOutBuffer = &Dcm_Channel[Offset + 4u];
        }
        Dcm_0x31Type->currentDataLength =
            (uint16)(Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen - ReqLen - DCM_UDS0X31_REQ_DATA_MINLENGTH);
        *ret = (*StopRoutineFnc)(
            pInbuffer,
            Dcm_0x31Type->OpStatus,
            pOutBuffer,
            &Dcm_0x31Type->currentDataLength,
            ErrorCode);
        if ((NULL_PTR != StopRoutine->DcmDspStopRoutineOut)
            && (StopRoutine->DcmDspStopRoutineOut
                    ->DcmDspRoutineInOutSignal[StopRoutine->DcmDspStopRoutineOut->RoutineInOutSignalNum - 1u]
                    .DcmDspRoutineSignalType
                == DCM_VARIABLE_LENGTH))
        {
            ResLen += Dcm_0x31Type->currentDataLength;
        }
        if (E_OK == *ret)
        {
            Dcm_RoutineControlState[*Dcm_0x31Type->pRoutineCfgId] = DCM_DSP_ROUTINE_OFF;
        }
    }
    return ResLen;
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
#if ((STD_ON == DCM_DSP_ROUTINE_FUNC_ENABLED) && (DCM_UDS0X31_REQUESTROUTINERESULTS_ENABLED == STD_ON))
static FUNC(uint32, DCM_CODE) Dcm_UdsRCReqRoutineResult(
    uint8 MsgCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_0x31Types, AUTOMATIC, DCM_VAR) Dcm_0x31Type,
    P2VAR(Std_ReturnType, AUTOMATIC, DCM_VAR) ret,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint32 ReqLen = 0;
    uint32 ResLen = 0;
    uint8 Index;
    const Dcm_DspRequestRoutineResultsType* RequestRoutineResults;
    Dcm_RequestResultsRoutineFncType RequestResultsFnc;
    uint8* pOutBuffer;
    const uint8* pInbuffer;
    uint32 Offset =
        (DcmPbCfgPtr->pDcmDslCfg
             ->pDcmChannelCfg)[Dcm_ChannelCtrl[Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex].Dcm_ChannelCfgIndex]
            .offset;
    const Dcm_DspRoutineType* pDcmDspRoutine = DcmPbCfgPtr->pDcmDspCfg->pDcmDspRoutine;
    uint8 InSignalNum;
    uint8 OutSignalNum;

    RequestRoutineResults = pDcmDspRoutine[*Dcm_0x31Type->pRoutineCfgId].DcmDspRequestRoutineResults;
    if (NULL_PTR == RequestRoutineResults)
    {
        *ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTED;
        *ret = E_NOT_OK;
    }

    if (E_OK == *ret)
    {
        if (NULL_PTR != RequestRoutineResults->DcmDspRequestRoutineResultsIn)
        {
            InSignalNum = RequestRoutineResults->DcmDspRequestRoutineResultsIn->RoutineInOutSignalNum;
            for (Index = 0; Index < InSignalNum; Index++)
            {
                uint16 length = RequestRoutineResults->DcmDspRequestRoutineResultsIn->DcmDspRoutineInOutSignal[Index]
                                    .DcmDspRoutineSignalLength;
                ReqLen += (uint32)length;
            }
        }
        else
        {
            ReqLen = 0;
        }
        if (RequestRoutineResults->DcmDspRequestRoutineResultsOut != NULL_PTR)
        {
            OutSignalNum = RequestRoutineResults->DcmDspRequestRoutineResultsOut->RoutineInOutSignalNum;
            for (Index = 0; Index < OutSignalNum; Index++)
            {
                uint16 length = RequestRoutineResults->DcmDspRequestRoutineResultsOut->DcmDspRoutineInOutSignal[Index]
                                    .DcmDspRoutineSignalLength;
                ResLen += (uint32)length;
            }
        }
        else
        {
            ResLen = 0;
        }
        if ((NULL_PTR != RequestRoutineResults->DcmDspRequestRoutineResultsIn)
            && (RequestRoutineResults->DcmDspRequestRoutineResultsIn
                    ->DcmDspRoutineInOutSignal
                        [RequestRoutineResults->DcmDspRequestRoutineResultsIn->RoutineInOutSignalNum - 1u]
                    .DcmDspRoutineSignalType
                == DCM_VARIABLE_LENGTH))
        {
            if ((ReqLen + DCM_UDS0X31_REQ_DATA_MINLENGTH) > Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
            {
                /*the request message length is not correct,send NRC 0x13*/
                *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                *ret = E_NOT_OK;
            }
        }
        else
        {
            if ((ReqLen + DCM_UDS0X31_REQ_DATA_MINLENGTH) != Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
            {
                /*the request message length is not correct,send NRC 0x13*/
                *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                *ret = E_NOT_OK;
            }
        }
    }

    if (E_OK == *ret)
    {
        RequestResultsFnc = RequestRoutineResults->DcmDspRequestResultsRoutineFnc;
        if (NULL_PTR == RequestResultsFnc)
        {
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            *ret = E_NOT_OK;
        }
    }
    if ((E_OK == *ret) && (DCM_DSP_ROUTINE_INIT == Dcm_RoutineControlState[*Dcm_0x31Type->pRoutineCfgId]))
    {
        /*the request sequence error,send NRC 0x24*/
        *ErrorCode = DCM_E_REQUESTSEQUENCEERROR;
        *ret = E_NOT_OK;
    }
    if (E_OK == *ret)
    {
        pInbuffer = NULL_PTR;
        if (0u != ReqLen)
        {
            pInbuffer = &Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[4];
        }
        pOutBuffer = NULL_PTR;
        if (0u != ResLen)
        {
            pOutBuffer = &Dcm_Channel[Offset + 4u];
        }
        Dcm_0x31Type->currentDataLength =
            (uint16)(Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen - ReqLen - DCM_UDS0X31_REQ_DATA_MINLENGTH);
        *ret = (*RequestResultsFnc)(
            pInbuffer,
            Dcm_0x31Type->OpStatus,
            pOutBuffer,
            &Dcm_0x31Type->currentDataLength,
            ErrorCode);
        if ((NULL_PTR != RequestRoutineResults->DcmDspRequestRoutineResultsOut)
            && (RequestRoutineResults->DcmDspRequestRoutineResultsOut
                    ->DcmDspRoutineInOutSignal
                        [RequestRoutineResults->DcmDspRequestRoutineResultsOut->RoutineInOutSignalNum - 1u]
                    .DcmDspRoutineSignalType
                == DCM_VARIABLE_LENGTH))
        {
            ResLen += Dcm_0x31Type->currentDataLength;
        }
    }
    return ResLen;
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
#if ((STD_ON == DCM_DSP_ROUTINE_FUNC_ENABLED) && (STD_ON == DCM_DSP_REQUESTCONTROL_FUNC_ENABLED))
static FUNC(uint32, DCM_CODE) Dcm_UdsOBDStartRoutine(
    uint8 MsgCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_0x31Types, AUTOMATIC, DCM_VAR) Dcm_0x31Type,
    P2VAR(Std_ReturnType, AUTOMATIC, DCM_VAR) ret,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint32 ReqLen = 0;
    uint32 ResLen = 0;
    uint8 Index;
    Dcm_StartRoutineFncType StartRoutineFnc;
    const Dcm_DspStartRoutineType* StartRoutine;
    const uint8* pInbuffer;
    uint8* pOutBuffer;
    uint32 Offset =
        (DcmPbCfgPtr->pDcmDslCfg
             ->pDcmChannelCfg)[Dcm_ChannelCtrl[Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex].Dcm_ChannelCfgIndex]
            .offset;
    const Dcm_DspRoutineType* pDcmDspRoutine = DcmPbCfgPtr->pDcmDspCfg->pDcmDspRoutine;
    uint8 InSignalNum;
    uint8 OutSignalNum;

    StartRoutine = pDcmDspRoutine[*Dcm_0x31Type->pRoutineCfgId].DcmDspStartRoutine;
    if (NULL_PTR == StartRoutine)
    {
        *ret = E_NOT_OK;
    }
    if (E_OK == *ret)
    {
        if (NULL_PTR != StartRoutine->DcmDspStartRoutineIn)
        {
            InSignalNum = StartRoutine->DcmDspStartRoutineIn->RoutineInOutSignalNum;
            for (Index = 0; Index < InSignalNum; Index++)
            {
                uint16 length =
                    StartRoutine->DcmDspStartRoutineIn->DcmDspRoutineInOutSignal[Index].DcmDspRoutineSignalLength;
                ReqLen += (uint32)length;
            }
        }
        else
        {
            ReqLen = 0;
        }
        if (NULL_PTR != StartRoutine->DcmDspStartRoutineOut)
        {
            OutSignalNum = StartRoutine->DcmDspStartRoutineOut->RoutineInOutSignalNum;
            for (Index = 0; Index < OutSignalNum; Index++)
            {
                uint16 length =
                    StartRoutine->DcmDspStartRoutineOut->DcmDspRoutineInOutSignal[Index].DcmDspRoutineSignalLength;
                ResLen += (uint32)length;
            }
        }
        else
        {
            ResLen = 0;
        }
        /*check the request message length*/
        if ((NULL_PTR != StartRoutine->DcmDspStartRoutineIn)
            && (StartRoutine->DcmDspStartRoutineIn
                    ->DcmDspRoutineInOutSignal[StartRoutine->DcmDspStartRoutineIn->RoutineInOutSignalNum - 1u]
                    .DcmDspRoutineSignalType
                == DCM_VARIABLE_LENGTH))
        {
            if ((ReqLen + DCM_UDS0X31_REQ_DATA_MINLENGTH) > Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
            {
                /*the request message length is not correct,send NRC 0x13*/
                *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                *ret = E_NOT_OK;
            }
        }
        else
        {
            if ((ReqLen + DCM_UDS0X31_REQ_DATA_MINLENGTH) != Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
            {
                /*the request message length is not correct,send NRC 0x13*/
                *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                *ret = E_NOT_OK;
            }
        }
    }

    if (E_OK == *ret)
    {
        StartRoutineFnc = StartRoutine->DcmDspStartRoutineFnc;
        if (NULL_PTR == StartRoutineFnc)
        {
            *ret = E_NOT_OK;
        }
    }
    if (E_OK == *ret)
    {
        pInbuffer = NULL_PTR;
        if (0u != ReqLen)
        {
            pInbuffer = &Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[4];
        }
        pOutBuffer = NULL_PTR;
        if (0u != ResLen)
        {
            pOutBuffer = &Dcm_Channel[Offset + 4u];
        }
        Dcm_0x31Type->currentDataLength =
            (uint16)(Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen - ReqLen - DCM_UDS0X31_REQ_DATA_MINLENGTH);
        *ret = (*StartRoutineFnc)(
            pInbuffer,
            Dcm_0x31Type->OpStatus,
            pOutBuffer,
            &Dcm_0x31Type->currentDataLength,
            ErrorCode);
        if ((NULL_PTR != StartRoutine->DcmDspStartRoutineOut)
            && (StartRoutine->DcmDspStartRoutineOut
                    ->DcmDspRoutineInOutSignal[StartRoutine->DcmDspStartRoutineOut->RoutineInOutSignalNum - 1u]
                    .DcmDspRoutineSignalType
                == DCM_VARIABLE_LENGTH))
        {
            ResLen += Dcm_0x31Type->currentDataLength;
        }
    }
    return ResLen;
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
#if ((STD_ON == DCM_DSP_ROUTINE_FUNC_ENABLED) && (STD_ON == DCM_DSP_REQUESTCONTROL_FUNC_ENABLED))
static FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x31OBDSubDeal(
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2CONST(Dcm_0x31Types, AUTOMATIC, DCM_VAR) Dcm_0x31Type,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;
    uint32 SupportBuffer = 0;
    uint8 iloop;
    uint8 index;
    uint8 routineIdNum;
    boolean Find = FALSE;
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    uint32 Offset =
        (DcmPbCfgPtr->pDcmDslCfg
             ->pDcmChannelCfg)[Dcm_ChannelCtrl[Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex].Dcm_ChannelCfgIndex]
            .offset;
    uint16 routineId;
    const Dcm_DspCfgType* pDcmDspCfg = DcmPbCfgPtr->pDcmDspCfg;
    const Dcm_DspRoutineType* pDcmDspRoutine = DcmPbCfgPtr->pDcmDspCfg->pDcmDspRoutine;
    Dcm_MsgLenType ReqDataLen = Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen;
    const uint8* pReqData = Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData;

    if ((ReqDataLen % 2u) == 0u)
    {
        routineIdNum = (uint8)((ReqDataLen - 2u) >> 1u);
        for (index = 0; index < routineIdNum; index++)
        {
            routineId =
                (uint16)(((uint16)((uint16)pReqData[2u + (2u * index)]) << 8u) | ((uint16)pReqData[3u + (2u * index)]));
            switch (routineId)
            {
            case 0xE000u:
            case 0xE020u:
            case 0xE040u:
            case 0xE060u:
            case 0xE080u:
            case 0xE0A0u:
            case 0xE0C0u:
            case 0xE0E0u:
                if ((pDcmDspCfg->DcmDspEnableObdMirror == TRUE) && (FALSE == Dcm_0x31Type->Find))
                {
                    OBD_CheckSupportedTIDs((uint8)(routineId & 0xFFu), &SupportBuffer);
                    if (SupportBuffer != 0UL)
                    {
                        if (index > 0u)
                        {
                            Dcm_Channel[Offset + 4u] = (uint8)(routineId >> 8u);
                            Offset++;
                            Dcm_Channel[Offset + 4u] = (uint8)(routineId);
                            Offset++;
                            (*Dcm_0x31Type->ResLen) += 2u;
                        }
                        Dcm_FillTo4bytes(&(Dcm_Channel[Offset + 4u]), &SupportBuffer);
                        Offset += 4u;
                        (*Dcm_0x31Type->ResLen) += 4u;
                    }
                }
                else
                {
                    for (iloop = 0; iloop < pDcmDspCfg->DcmDspRoutineNum; iloop++)
                    {
                        if ((pDcmDspRoutine[iloop].DcmDspRoutineId >= (routineId + 0x01u))
                            && (pDcmDspRoutine[iloop].DcmDspRoutineId <= (routineId + 0x20u)))
                        {
                            SupportBuffer |= (uint32)1UL
                                             << (0x20u - (pDcmDspRoutine[iloop].DcmDspRoutineId - routineId));
                        }
                        if (pDcmDspRoutine[iloop].DcmDspRoutineId == routineId)
                        {
                            Find = TRUE;
                        }
                    }
                    if (Find != TRUE)
                    {
                        SupportBuffer = 0;
                    }
                    if (SupportBuffer != 0UL)
                    {
                        if (index > 0u)
                        {
                            Dcm_Channel[Offset + 4u] = (uint8)(routineId >> 8u);
                            Offset++;
                            Dcm_Channel[Offset + 4u] = (uint8)(routineId);
                            Offset++;
                            (*Dcm_0x31Type->ResLen) += 2u;
                        }
                        Dcm_FillTo4bytes(&(Dcm_Channel[Offset + 4u]), &SupportBuffer);
                        Offset += 4u;
                        (*Dcm_0x31Type->ResLen) += 4u;
                    }
                }
                break;
            default:
                ret = E_NOT_OK;
                break;
            }
        }
        if (*Dcm_0x31Type->ResLen == 0u)
        {
            ret = E_NOT_OK;
        }
    }
    else
    {
        /*the request message length is not correct,send NRC 0x13*/
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        ret = E_NOT_OK;
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
static FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x31OBDDeal(
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_0x31Types, AUTOMATIC, DCM_VAR) Dcm_0x31Type,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;
    uint8 CtrlIndex;
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    const Dcm_MsgContextType* pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
    uint32 Offset =
        (DcmPbCfgPtr->pDcmDslCfg
             ->pDcmChannelCfg)[Dcm_ChannelCtrl[Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex].Dcm_ChannelCfgIndex]
            .offset;
    Dcm_MsgLenType ReqDataLen = pMsgContext->ReqDataLen;
    const uint8* pReqData = pMsgContext->pReqData;
    uint16 routineId = (uint16)((uint16)((uint16)pReqData[2]) << 8u) | ((uint16)pReqData[3]);
    const Dcm_DspCfgType* pDcmDspCfg = DcmPbCfgPtr->pDcmDspCfg;
    const Dcm_DspRequestControlType* pDcmDspRequestControl = DcmPbCfgPtr->pDcmDspCfg->pDcmDspRequestControl;

    switch (routineId)
    {
    case 0xE000u:
    case 0xE020u:
    case 0xE040u:
    case 0xE060u:
    case 0xE080u:
    case 0xE0A0u:
    case 0xE0C0u:
    case 0xE0E0u:
        ret = Dcm_UDS0x31OBDSubDeal(ProtocolCtrlId, Dcm_0x31Type, ErrorCode);
        break;
    default:
        if ((pDcmDspCfg->DcmDspEnableObdMirror == TRUE) && (FALSE == Dcm_0x31Type->Find))
        {
            if (E_OK == OBD_FindCfgIndexOfTestID((uint8)(routineId & 0xFFu), &CtrlIndex))
            {
#if (STD_ON == DCM_OBD_SERVICE0X08_ENABLED)
                if (pDcmDspRequestControl[CtrlIndex].DcmDspRequestControlInBufferSize == (ReqDataLen - 4u))
                {
                    if (E_OK
                        == Rte_RequestControl(
                            (uint8)(routineId & 0xFFu),
                            &pMsgContext->pReqData[4],
                            &(Dcm_Channel[Offset + 4u])))
                    {
                        *Dcm_0x31Type->ResLen = pDcmDspRequestControl[CtrlIndex].DcmDspRequestControlOutBufferSize;
                    }
                    else
#endif
                    {
                        /* the conditions of the system are not proper to run,send NRC 0x22*/
                        *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
                        ret = E_NOT_OK;
                    }
#if (STD_ON == DCM_OBD_SERVICE0X08_ENABLED)
                }
                else
                {
                    ret = E_NOT_OK;
                }
#endif
            }
            else
            {
                /* there is no supported VehInfotype,ignore the request message */
                ret = E_NOT_OK;
            }
        }
        else
        {
            if (TRUE == Dcm_0x31Type->Find)
            {
                (*Dcm_0x31Type->ResLen) = Dcm_UdsOBDStartRoutine(MsgCtrlId, Dcm_0x31Type, &ret, ErrorCode);
                if (ret == E_NOT_OK)
                {
                    *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
                }
            }
            else
            {
                /*if not found,send NRC 0x31*/
                *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
                ret = E_NOT_OK;
            }
        }
        break;
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
static FUNC(Std_ReturnType, DCM_CODE) Dcm_RCConditonCheck(
    uint8 ProtocolCtrlId,
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
#if (STD_ON == DCM_DSP_REQUESTCONTROL_FUNC_ENABLED)
    P2VAR(Dcm_0x31Types, AUTOMATIC, DCM_VAR) Dcm_0x31Type,
#else
    P2CONST(Dcm_0x31Types, AUTOMATIC, DCM_VAR) Dcm_0x31Type,
#endif
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
#if ((STD_ON == DCM_DSP_ROUTINE_FUNC_ENABLED) && (DCM_DSP_ROUTINE_MAX_NUM > 0))
    boolean Find = FALSE;
    uint16 routineId;
    uint8 Index;
#endif
    const Dcm_DspCfgType* pDcmDspCfg = DcmPbCfgPtr->pDcmDspCfg;
    const Dcm_DspRoutineType* pDcmDspRoutine = DcmPbCfgPtr->pDcmDspCfg->pDcmDspRoutine;
    Dcm_MsgLenType ReqDataLen = Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen;
    const uint8* pReqData = Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData;

    if (ReqDataLen < DCM_UDS0X31_REQ_DATA_MINLENGTH)
    {
        /*the length of massage is not correct,send NRC 0x13*/
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        ret = E_NOT_OK;
    }

#if ((STD_OFF == DCM_DSP_ROUTINE_FUNC_ENABLED) || (DCM_DSP_ROUTINE_MAX_NUM == 0))
    /*NRC 0x31:request out of range*/
    if (E_OK == ret)
    {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        ret = E_NOT_OK;
    }
#else
    if (E_OK == ret)
    {
        /*find the required RoutineID in configuration*/
        routineId = (uint16)((uint16)((uint16)pReqData[2]) << 8u) | ((uint16)pReqData[3]);
        for (Index = 0; (Index < pDcmDspCfg->DcmDspRoutineNum) && (Find == FALSE); Index++)
        {
            if ((routineId == pDcmDspRoutine[Index].DcmDspRoutineId)
                && (TRUE == pDcmDspRoutine[Index].DcmDspRoutineUsed))
            {
                *Dcm_0x31Type->pRoutineCfgId = Index;
                Find = TRUE;
            }
        }

#if (STD_ON == DCM_DSP_REQUESTCONTROL_FUNC_ENABLED)
        if ((0xE000u <= routineId) && (0xE0FFu >= routineId))
        {
            Dcm_0x31Type->Find = Find;
            ret = Dcm_UDS0x31OBDDeal(ProtocolCtrlId, Dcm_0x31Type, ErrorCode);
        }
        else
#endif
        {
            if (Find == FALSE)
            {
                /*if not found,send NRC 0x31*/
                *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
                ret = E_NOT_OK;
            }
        }
    }
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
Dcm_UDS0x31(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432,1532-- */ /* MISRA Rule 20.7,8.7 */
{
    uint8 MsgCtrlId;
    uint32 Offset;
    uint8 TxChannelCtrlIndex;
    uint8 TxChannelCfgIndex;
    uint8 SubFunction;
    uint32 ResLen = 0;
    uint8 RoutineCfgId;
#if ((STD_ON == DCM_DSP_ROUTINE_FUNC_ENABLED) && (DCM_DSP_ROUTINE_MAX_NUM > 0))
    uint16 routineId = 0u;
#endif
    Std_ReturnType ret;
    Dcm_0x31Types Dcm_0x31Type;
    Dcm_MsgContextType* pMsgContext;
    const Dcm_DslBufferType* pDcmChannelCfg;

    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
    /*get the Tx buffer*/
    TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
    pDcmChannelCfg = &DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex];
    Offset = pDcmChannelCfg->offset;
    SubFunction = Dcm_MsgCtrl[MsgCtrlId].Subfunction;

    Dcm_0x31Type.OpStatus = OpStatus;
    Dcm_0x31Type.pRoutineCfgId = &RoutineCfgId;
    Dcm_0x31Type.ResLen = &ResLen;

    ret = Dcm_RCConditonCheck(ProtocolCtrlId, &Dcm_0x31Type, ErrorCode);

#if ((STD_OFF == DCM_DSP_ROUTINE_FUNC_ENABLED) || (DCM_DSP_ROUTINE_MAX_NUM == 0))
    /*NRC 0x31:request out of range*/
    if (E_OK == ret)
    {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        ret = E_NOT_OK;
    }
#else
    if (E_OK == ret)
    {
        /*find the required RoutineID in configuration*/
        routineId = (uint16)((uint16)((uint16)pMsgContext->pReqData[2]) << 8u) | ((uint16)pMsgContext->pReqData[3]);
#if (STD_ON == DCM_DSP_REQUESTCONTROL_FUNC_ENABLED)
        if ((0xE000u > routineId) || (0xE0FFu < routineId))
        {
#endif
            /*check session and security*/
            ret = Dcm_UdsRCSesAndSecCheck(ProtocolCtrlId, RoutineCfgId, ErrorCode);
            if (E_OK == ret)
            {
                /*check whether the sub-function is supported*/
                if ((SubFunction != DCM_UDS0X31_STARTROUTINE) && (SubFunction != DCM_UDS0X31_STOPROUTINE)
                    && (SubFunction != DCM_UDS0X31_REQUESTROUTINERESULTS))
                {
                    /*if the required sub-function is not supported,send NRC 0x12*/
                    *ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTED;
                    ret = E_NOT_OK;
                }
            }

            if (E_OK == ret)
            {
                switch (SubFunction)
                {
#if (DCM_UDS0X31_STARTROUTINE_ENABLED == STD_ON)
                case DCM_UDS0X31_STARTROUTINE:
                    ResLen = Dcm_UdsRCStartRoutine(MsgCtrlId, &Dcm_0x31Type, &ret, ErrorCode);
                    break;
#endif
#if (DCM_UDS0X31_STOPROUTINE_ENABLED == STD_ON)
                case DCM_UDS0X31_STOPROUTINE:
                    ResLen = Dcm_UdsRCStopRoutine(MsgCtrlId, &Dcm_0x31Type, &ret, ErrorCode);
                    break;
#endif
#if (DCM_UDS0X31_REQUESTROUTINERESULTS_ENABLED == STD_ON)
                case DCM_UDS0X31_REQUESTROUTINERESULTS:
                    ResLen = Dcm_UdsRCReqRoutineResult(MsgCtrlId, &Dcm_0x31Type, &ret, ErrorCode);
                    break;
#endif
                default:
                    *ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTED;
                    ret = E_NOT_OK;
                    break;
                }
            }
#if (STD_ON == DCM_DSP_REQUESTCONTROL_FUNC_ENABLED)
        }
#endif
    }
    /* check tx data length */
    if ((E_OK == ret) && ((ResLen + 4u) > (pDcmChannelCfg->Dcm_DslBufferSize)))
    {
        /*Pdu length is bigger than buffer size,ignore the request message */
        *ErrorCode = DCM_E_RESPONSETOOLONG;
        ret = E_NOT_OK;
    }
    /*check the return value*/
    switch (ret)
    {
    case E_OK:
        Dcm_Channel[Offset] = 0x71;
        Dcm_Channel[Offset + 1u] = SubFunction;
        Dcm_Channel[Offset + 2u] = (uint8)(routineId >> 8u);
        Dcm_Channel[Offset + 3u] = (uint8)(routineId);
        pMsgContext->ResMaxDataLen = (uint32)ResLen + 4u;
        pMsgContext->ResDataLen = (uint32)ResLen + 4u;
        pMsgContext->pResData = &Dcm_Channel[Offset];
        DsdInternal_ProcessingDone(ProtocolCtrlId);
        break;
    case E_NOT_OK:
        break;
    case DCM_E_PENDING:
        Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
        break;
#if (STD_ON == DCM_DSLDIAGRESP_FORCERESPENDEN)
    case DCM_E_FORCE_RCRRP:
        Dcm_MsgCtrl[MsgCtrlId].Dcm_Ret = DCM_E_FORCE_RCRRP;
        DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_RESPONSE_PENDING);
        DsdInternal_ProcessingDone(ProtocolCtrlId);
        break;
#endif
    default:
        /*unexpected return value,send NRC 0x22*/
        DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_CONDITIONSNOTCORRECT);
        DsdInternal_ProcessingDone(ProtocolCtrlId);
        ret = E_NOT_OK;
        break;
    }
#endif
    return ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
