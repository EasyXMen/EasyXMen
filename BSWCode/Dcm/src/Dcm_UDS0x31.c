/**
 * Copyright (C) 2008-2026 isoft Infrastructure Software Co., Ltd.
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
/*
 ************************************************************************************************************************
 **
 **  @file               : Dcm_UDS0x31.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm UDS service 0x31 function implementation
 **
 ***********************************************************************************************************************/
/* PRQA S 3120, 1532 EOF */ /* VL_QAC_MagicNum, VL_QAC_OneFunRef */
/* =================================================== inclusions =================================================== */
#include "Dcm_Cfg.h"
#if (STD_ON == DCM_UDS_0X31)
#if (defined DCM_SECURE_CODING_VALIDATION_ROUTINE) && ((STD_ON == DCM_UDS_0X29_0X4) && (STD_ON == DCM_UDS_0X2E))
#include "Csm.h"
#endif
#include "Dcm_Internal.h"
/* ===================================================== macros ===================================================== */
/**
 * @brief The minimum request length of UDS 0x31
 */
#define DCM_UDS31_MINREQLEN 3uL
#ifdef DCM_UDS_0X31_0X1
/**
 * @brief The subfunction startRoutine
 */
#define DCM_UDS31_STARTROUTINE 1u
#endif
#ifdef DCM_UDS_0X31_0X2
/**
 * @brief The subfunction stopRoutine
 */
#define DCM_UDS31_STOPROUTINE 2u
#endif
#ifdef DCM_UDS_0X31_0X3
/**
 * @brief The subfunction requestResult
 */
#define DCM_UDS31_REQUESTROUTINERESULT 3u
#endif
/**
 * @brief The lower limit of OBD RID
 */
#define DCM_OBD_RID_RANGE_LOWER 0xE000u
/**
 * @brief The upper limit of OBD RID
 */
#define DCM_OBD_RID_RANGE_UPPER 0xE0FFu
/* ================================================ type definitions ================================================ */
/**
 * @brief Routine current status
 */
typedef enum
{
    DCM_ROUTINE_INITIAL,
    DCM_ROUTINE_START,
    DCM_ROUTINE_STOP
} Dcm_RoutineStatusType;
/* ============================================ internal data definitions =========================================== */
#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
DCM_LOCAL Dcm_RoutineStatusType Dcm_RoutineStatus[DCM_ROUTINE_NUM];
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
#ifdef DCM_SECURE_CODING_VALIDATION_ROUTINE
#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
DCM_LOCAL Dcm_ExtendedOpStatusType Dcm_SecureCodingOpStatus;
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
#endif
/* ========================================== internal function declarations ======================================== */
/**
 * @brief         find the configured rid index
 * @param[in]     rid          : the input rid
 * @param[out]    routineIndex : the configuration index
 * @param[out]    ErrorCode    : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                               shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                               value.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType
    Dcm_UDS0x31_FindRoutineIndex(uint16 rid, uint16* routineIndex, Dcm_NegativeResponseCodeType* ErrorCode);

#if (STD_ON == DCM_OBD_RID_MIRROR)
/**
 * @brief         process OBD type rid
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[inout]  totalLength : the total length of the response
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x31_HandleOBDRID(
    Dcm_MsgContextType*           pMsgContext,
    uint16*                       totalLength,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#ifdef DCM_SECURE_CODING_VALIDATION_ROUTINE
/**
 * @brief         process secure coding rid
 * @param[in]     OpStatus     : Indicates the current operation status
 * @param[in]     routineIndex : target routine index
 * @param[inout]  pMsgContext  : Message-related information for one diagnostic protocol identifier. The pointers in
 *                               pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode    : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                               shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                               value.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x31_SecureCoding(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint16                        routineIndex,
    const Dcm_MsgContextType*     pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         handle secure coding for DCM_INITIAL opstatus
 * @param[in]     routineIndex : target routine index
 * @param[inout]  pMsgContext  : Message-related information for one diagnostic protocol identifier. The pointers in
 *                               pMsgContext shall point behind the SID.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x31_SecureCodingInitial(const Dcm_MsgContextType* pMsgContext, uint16 routineIndex);

/**
 * @brief         handle secure coding for DCM_PENDING opstatus
 * @param[out]    resData  : The output data
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x31_SecureCodingPending(uint8* resData);

/**
 * @brief         finish secure coding
 * @param[out]    resData  : The output data
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x31_SecureCodingFinish(uint8* resData);
#endif

/**
 * @brief         handle UDS 0x31 subfunctions, check condition
 * @param[in]     OpStatus     : current operation status
 * @param[in]     protocolId   : the current protocolId
 * @param[in]     routineIndex : routine configuration index
 * @param[in]     subFunction  : the requested subfunction
 * @param[out]    routineSub   : the requested subConfiguration of routine
 * @param[out]    ErrorCode    : nrc to send
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x31_HandleSubFunction(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint8                         protocolId,
    uint16                        routineIndex,
    uint8                         subFunction,
    const Dcm_DspRoutineSubType** routineSub,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         check routine common authroization
 * @param[in]     commonAuthorization : routine configuration index
 * @param[out]    ErrorCode           : nrc to send
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x31_CheckRoutineCommonAuth(
    const Dcm_DspCommonAuthorizationType* commonAuthorization,
    Dcm_NegativeResponseCodeType*         ErrorCode
#if (STD_ON == DCM_UDS_0X29)
    ,
    uint16 connectionId,
    uint16 routineIndex
#endif
);

/**
 * @brief         process regular type rid
 * @param[in]     OpStatus     : current operation status
 * @param[in]     routineIndex : current routine configuration index
 * @param[inout]  pMsgContext  : Message-related information for one diagnostic protocol identifier. The pointers in
 *                               pMsgContext shall point behind the SID.
 * @param[out]    totalLength  : the total length of the response
 * @param[out]    ErrorCode    : nrc to send
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x31_HandleRegularRID(
    Dcm_ExtendedOpStatusType      OpStatus,
    const Dcm_MsgContextType*     pMsgContext,
    uint16                        routineIndex,
    uint16*                       totalLength,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         process routine callout
 * @param[in]     OpStatus     : current operation status
 * @param[in]     routineSub   : the requested subConfiguration of routine
 * @param[in]     routineIndex : the routine configuration index
 * @param[inout]  pMsgContext  : Message-related information for one diagnostic protocol identifier. The pointers in
 *                               pMsgContext shall point behind the SID.
 * @param[out]    totalLength  : the total length of the response
 * @param[out]    ErrorCode    : nrc to send
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x31_RoutineCallout(
    Dcm_ExtendedOpStatusType      OpStatus,
    const Dcm_MsgContextType*     pMsgContext,
    const Dcm_DspRoutineSubType*  routineSub,
    uint16                        routineIndex,
    uint16*                       totalLength,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         check inSignal request length
 * @param[in]     reqDataLen : request data length
 * @param[in]     routineSub : the requested subConfiguration of routine
 * @param[out]    ErrorCode  : nrc to send
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x31_InSignalCheck(
    Dcm_MsgLenType                reqDataLen,
    const Dcm_DspRoutineSubType*  routineSub,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         check outSignal response length
 * @param[in]     routineSub        : the requested subConfiguration of routine
 * @param[in]     currentDataLength : current response data length
 * @param[out]    totalLength       : the response length
 * @param[out]    ErrorCode         : nrc to send
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x31_OutSignalCheck(
    const Dcm_DspRoutineSubType*  routineSub,
    uint16                        currentDataLength,
    uint16*                       totalLength,
    Dcm_NegativeResponseCodeType* ErrorCode);
/* ========================================== external function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * The service interpreter for UDS 0x31
 */
Std_ReturnType Dcm_UDS0x31(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result;
    Dcm_MsgLenType reqDataLen = pMsgContext->reqDataLen;

    if (reqDataLen < DCM_UDS31_MINREQLEN)
    {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        result     = E_NOT_OK;
    }
    else
    {
        const uint8* reqData      = pMsgContext->reqData;
        uint16       rid          = DCM_U8N_TO_U16(&reqData[1u]);
        uint16       routineIndex = 0u;
        uint8*       resData      = pMsgContext->resData;
        uint8        subFunction  = reqData[0u];
        uint16       totalLength;
        result = Dcm_UDS0x31_FindRoutineIndex(rid, &routineIndex, ErrorCode);
#if (STD_ON == DCM_OBD_RID_MIRROR)
        if ((rid >= DCM_OBD_RID_RANGE_LOWER) && (rid <= DCM_OBD_RID_RANGE_UPPER) && (E_NOT_OK == result))
        {
            result = Dcm_UDS0x31_HandleOBDRID(pMsgContext, &totalLength, ErrorCode);
        }
        else
#endif
            if (E_OK == result)
        {
            result = Dcm_UDS0x31_HandleRegularRID(OpStatus, pMsgContext, routineIndex, &totalLength, ErrorCode);
        }
#if (STD_ON == DCM_OBD_RID_MIRROR)
        else
        {
            /* idle */
        }
#endif

        if (E_OK == result)
        {
            if (totalLength > pMsgContext->resMaxDataLen)
            {
                *ErrorCode = DCM_E_RESPONSETOOLONG;
                result     = E_NOT_OK;
            }
            else
            {
                resData[0u] = subFunction;
                DcmInternal_Memcpy(&resData[1u], &reqData[1u], 2u);
                pMsgContext->resDataLen = totalLength;
            }
        }
    }

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/* ========================================== internal function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * find DcmDspRoutine index for given rid
 */
DCM_LOCAL Std_ReturnType
    Dcm_UDS0x31_FindRoutineIndex(uint16 rid, uint16* routineIndex, Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_NOT_OK;

    for (uint16 index = 0u; index < DCM_ROUTINE_NUM; index++)
    {
        if ((Dcm_CfgPtr->RoutineUsed[index]) && (Dcm_DspRoutine[index].RoutineIdentifier == rid))
        {
            result        = E_OK;
            *routineIndex = index;
            break;
        }
    }

    if (E_NOT_OK == result)
    {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
    }

    return result;
}

#if (STD_ON == DCM_OBD_RID_MIRROR)
DCM_LOCAL Std_ReturnType Dcm_UDS0x31_HandleOBDRID(
    Dcm_MsgContextType*           pMsgContext,
    uint16*                       totalLength,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result  = E_NOT_OK;
    uint8*         reqData = pMsgContext->reqData;
    uint8*         resData = pMsgContext->resData;
    uint16         rid     = DCM_U8N_TO_U16(&reqData[1u]);
    uint16         currentDataLength;

    if (DCM_UDS31_STARTROUTINE == reqData[0u])
    {
        *ErrorCode        = DCM_POS_RESP;
        currentDataLength = pMsgContext->resMaxDataLen - 4u;
        result            = Dcm_OBD0x08_ReadTid(
            (uint8)rid,
            &reqData[1u],
            pMsgContext->reqDataLen - 3u,
            &resData[3u],
            (Dcm_MsgLenType*)&currentDataLength,
            ErrorCode);
    }
    else
    {
        *ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTED;
    }

    if (E_OK == result)
    {
        *totalLength = currentDataLength + 4u;
#if (STD_ON == DCM_OBD_0X8)
        uint8 tidIndex;
        (void)Dcm_OBD0x08_FindTid((uint8)rid, &tidIndex);
        if (NULL_PTR != Dcm_DspRequestControl[tidIndex].RequestControlInfoByte)
        {
            resData[*totalLength] = *(Dcm_DspRequestControl[tidIndex].RequestControlInfoByte);
            (*totalLength)++;
        }
#endif
    }

    return result;
}
#endif

#ifdef DCM_SECURE_CODING_VALIDATION_ROUTINE
DCM_LOCAL Std_ReturnType Dcm_UDS0x31_SecureCoding(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint16                        routineIndex,
    const Dcm_MsgContextType*     pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result  = E_OK;
    uint8*         resData = pMsgContext->resData;
#if ((STD_ON == DCM_UDS_0X29_0X4) && (STD_ON == DCM_UDS_0X2E))
    if (TRUE == Dcm_TransmitCertificate)
    {
        if (0u == Dcm_SecureCodingDataLength)
        {
            result = E_NOT_OK;
        }
        else
        {
            if (DCM_INITIAL == OpStatus)
            {
                result = Dcm_UDS0x31_SecureCodingInitial(pMsgContext, routineIndex);
            }
            else if (DCM_PENDING == OpStatus)
            {
                /* PRQA S 3446++ */ /* VL_Dcm_3446 */
                result = (DCM_PENDING == Dcm_SecureCodingOpStatus) ? Dcm_UDS0x31_SecureCodingFinish(resData)
                                                                   : Dcm_UDS0x31_SecureCodingPending(resData);
                /* PRQA S 3446-- */ /* VL_Dcm_3446 */
            }
            else
            {
                /* idle */
            }
        }
    }
    else
    {
        result = E_NOT_OK;
    }
#else
    result = E_NOT_OK;
#endif
    if (E_NOT_OK == result)
    {
        *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
    }

    return result;
}

DCM_LOCAL Std_ReturnType Dcm_UDS0x31_SecureCodingInitial(const Dcm_MsgContextType* pMsgContext, uint16 routineIndex)
{
    Crypto_VerifyResultType verifyResult;
    Std_ReturnType          result = Csm_SignatureVerify(
        DCM_SECURE_CODING_AUTHENTICATION_JOB_REF,
        CRYPTO_OPERATIONMODE_SINGLECALL,
        Dcm_SecureCodingBuffer,
        (uint32)Dcm_SecureCodingDataLength,
        &pMsgContext->reqData[3u],
        (uint32)Dcm_DspRoutine[routineIndex].StartRoutine->InSignalDataLength,
        &verifyResult);

    if (E_OK == result)
    {
        result                = DCM_E_PENDING;
        Dcm_AuthenticateAsync = DCM_CSM_SIGNATURE_VERIFY_ASYNC_PENDING;
        Dcm_CsmJobId          = DCM_SECURE_CODING_AUTHENTICATION_JOB_REF;
    }
    else
    {
        pMsgContext->resData[3] = 0x02u;
    }

    return result;
}

DCM_LOCAL Std_ReturnType Dcm_UDS0x31_SecureCodingPending(uint8* resData)
{
    Std_ReturnType result = E_OK;
    if (DCM_CSM_FINISH_SIGNATURE_VERIFY_ASYNC == Dcm_AuthenticateAsync)
    {
        if (E_OK == Dcm_CsmResult)
        {
            result = Dcm_UDS0x31_SecureCodingFinish(resData);
        }
        else
        {
            resData[3] = 0x02u;
        }
        Dcm_AuthenticateAsync = DCM_ASYNC_IDLE;
    }
    else if (DCM_CSM_SIGNATURE_VERIFY_ASYNC_PENDING == Dcm_AuthenticateAsync)
    {
        result = DCM_E_PENDING;
    }
    else
    {
        /* idle */
    }

    return result;
}

DCM_LOCAL Std_ReturnType Dcm_UDS0x31_SecureCodingFinish(uint8* resData)
{
    uint16                       index;
    uint16                       didIndex;
    uint16                       did;
    Dcm_NegativeResponseCodeType tempErrorCode;
    Std_ReturnType               result      = E_OK;
    Std_ReturnType               finalResult = E_OK;

    for (index = 0u; index < DCM_SECURE_CODING_DID_NUM; index++)
    {
        if (DCM_INVALID_UINT16 != Dcm_SecureCodingDidOffset[index])
        {
            did    = DCM_U8N_TO_U16(&Dcm_SecureCodingBuffer[Dcm_SecureCodingDidOffset[index]]);
            result = Dcm_UDS_FindDid(did, NULL_PTR, &didIndex, NULL_PTR);
            if (E_OK == result)
            {
                result = Dcm_UDS0x2E_WriteDspData(
                    Dcm_SecureCodingOpStatus,
                    &Dcm_DspDid[didIndex],
                    &Dcm_SecureCodingBuffer[Dcm_SecureCodingDidOffset[index]],
                    (Dcm_MsgLenType)Dcm_DspDid[didIndex].DidSize + 2u,
                    &tempErrorCode);

                if (DCM_E_PENDING == result)
                {
                    Dcm_SecureCodingOpStatus = DCM_PENDING;
                    finalResult              = DCM_E_PENDING;
                }
            }
            if (E_NOT_OK == result)
            {
                resData[3] = 0x01u;
                break;
            }
        }
    }
    if ((E_OK == result) && (E_OK == finalResult))
    {
        Dcm_SecureCodingOpStatus = DCM_INITIAL;
        resData[3]               = 0x00u;
    }

    return finalResult;
}
#endif

/* PRQA S 6040++ */ /* VL_MTR_Dcm_STPAR */
DCM_LOCAL Std_ReturnType Dcm_UDS0x31_HandleSubFunction(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint8                         protocolId,
    uint16                        routineIndex,
    uint8                         subFunction,
    const Dcm_DspRoutineSubType** routineSub,
    Dcm_NegativeResponseCodeType* ErrorCode)
/* PRQA S 6040-- */ /* VL_MTR_Dcm_STPAR */
{
    Std_ReturnType            result           = E_OK;
    Dcm_RoutineStatusType     oldRoutineStatus = Dcm_RoutineStatus[routineIndex];
    const Dcm_DspRoutineType* DcmDspRoutineCfg = &Dcm_DspRoutine[routineIndex];

#if (STD_ON == DCM_P4_SERVER)

    if (DCM_INITIAL == OpStatus)
    {
        Dcm_ProtocolCtrl[protocolId].P4Timer = DcmDspRoutineCfg->RoutineP4ServerMax;
    }
#else
    DCM_UNUSED(OpStatus);
    DCM_UNUSED(protocolId);
#endif
    switch (subFunction)
    {
#ifdef DCM_UDS_0X31_0X1
    case DCM_UDS31_STARTROUTINE:
    {
#ifndef DCM_DSP_ROUTINE_CHECK_RESTART
        *routineSub = DcmDspRoutineCfg->StartRoutine;
        if (!((*routineSub)->RoutineRestartEnabled) && (DCM_ROUTINE_START == oldRoutineStatus))
        {
            *ErrorCode = DCM_E_REQUESTSEQUENCEERROR;
            result     = E_NOT_OK;
        }
#endif
        break;
    }
#endif
#ifdef DCM_UDS_0X31_0X2
    case DCM_UDS31_STOPROUTINE:
    {
        *routineSub = DcmDspRoutineCfg->StopRoutine;
        if (DCM_ROUTINE_START != oldRoutineStatus)
        {
            *ErrorCode = DCM_E_REQUESTSEQUENCEERROR;
            result     = E_NOT_OK;
        }
        break;
    }
#endif
#ifdef DCM_UDS_0X31_0X3
    case DCM_UDS31_REQUESTROUTINERESULT:
    {
        *routineSub = DcmDspRoutineCfg->RequestRoutineResults;
        if (DCM_ROUTINE_INITIAL == oldRoutineStatus)
        {
            *ErrorCode = DCM_E_REQUESTSEQUENCEERROR;
            result     = E_NOT_OK;
        }
        break;
    }
#endif
    default:
    {
        /* idle */
        break;
    }
    }
    if (NULL_PTR == *routineSub)
    {
        *ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTED;
        result     = E_NOT_OK;
    }

    return result;
}

DCM_LOCAL Std_ReturnType Dcm_UDS0x31_CheckRoutineCommonAuth(
    const Dcm_DspCommonAuthorizationType* commonAuthorization,
    Dcm_NegativeResponseCodeType*         ErrorCode
#if (STD_ON == DCM_UDS_0X29)
    ,
    uint16 connectionId,
    uint16 routineIndex
#endif
)
{
    Std_ReturnType result = E_OK;
    if (NULL_PTR != commonAuthorization)
    {
        result = Dcm_UDS0x10_CheckSession(
            commonAuthorization->CommonAuthorizationSessionRef,
            commonAuthorization->CommonAuthorizationSessionRefNum,
            ErrorCode);
    }

#if (STD_ON == DCM_UDS_0X29)
    if (E_OK == result)
    {
        result = Dcm_UDS0x29_RidAuthenticationCheck(connectionId, routineIndex, ErrorCode);
    }
#endif

#if (STD_ON == DCM_UDS_0X27)
    if ((E_OK == result) && (NULL_PTR != commonAuthorization))
    {
        result = Dcm_UDS0x27_CheckSecurity(
            commonAuthorization->CommonAuthorizationSecurityLevelRef,
            commonAuthorization->CommonAuthorizationSecurityLevelRefNum,
            ErrorCode);
    }
#endif
    if ((E_OK == result) && (NULL_PTR != commonAuthorization)
        && (NULL_PTR != commonAuthorization->CommonAuthorizationModeRuleRef))
    {
        result = commonAuthorization->CommonAuthorizationModeRuleRef(ErrorCode);
    }

    return result;
}

DCM_LOCAL Std_ReturnType Dcm_UDS0x31_HandleRegularRID(
    Dcm_ExtendedOpStatusType      OpStatus,
    const Dcm_MsgContextType*     pMsgContext,
    uint16                        routineIndex,
    uint16*                       totalLength,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_OK;
    uint16         connectionId;
    uint8          protocolId;
    (void)DslInternal_FindProtocolRowByRxPduId(pMsgContext->dcmRxPduId, &protocolId, &connectionId);

#ifdef DCM_SECURE_CODING_VALIDATION_ROUTINE
    boolean secureCodingRoutine = FALSE;
    if (DCM_SECURE_CODING_VALIDATION_ROUTINE == routineIndex)
    {
        secureCodingRoutine = TRUE;
#if ((STD_ON == DCM_UDS_0X29_0X4) && (STD_ON == DCM_UDS_0X2E))
        if (TRUE == Dcm_TransmitCertificate)
        {
            result = Dcm_UDS0x31_SecureCoding(OpStatus, routineIndex, pMsgContext, ErrorCode);
        }
        else
        {
            result = E_NOT_OK;
        }
#else
        result = E_NOT_OK;
#endif
        if (E_OK == result)
        {
            *totalLength = 4u;
        }
        else if (E_NOT_OK == result)
        {
            *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
        }
        else
        {
            /* idle */
        }
    }
#endif

#ifdef DCM_SECURE_CODING_VALIDATION_ROUTINE
    if (FALSE == secureCodingRoutine)
#endif
    {
        const Dcm_DspRoutineType*    DcmDspRoutineCfg = &Dcm_DspRoutine[routineIndex];
        const Dcm_DspRoutineSubType* tempRoutineSub   = NULL_PTR;
        const Dcm_DspRoutineSubType* routineSub       = NULL_PTR;

        /**< Same session and security settings for same routine identifier */
        if (DcmDspRoutineCfg->StartRoutine != NULL_PTR)
        {
            tempRoutineSub = DcmDspRoutineCfg->StartRoutine;
        }
        else if (DcmDspRoutineCfg->StopRoutine != NULL_PTR)
        {
            tempRoutineSub = DcmDspRoutineCfg->StopRoutine;
        }
        else if (DcmDspRoutineCfg->RequestRoutineResults != NULL_PTR)
        {
            tempRoutineSub = DcmDspRoutineCfg->RequestRoutineResults;
        }
        else
        {
            /**<  no-supported SubFunction for RID*/
            result     = E_NOT_OK;
            *ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTED; /**< NRC 0x12 */
        }

        if (E_OK == result)
        {
            const Dcm_DspCommonAuthorizationType* commonAuthorization = tempRoutineSub->RoutineCommonAuthorizationRef;
            result                                                    = Dcm_UDS0x31_CheckRoutineCommonAuth(
                commonAuthorization,
                ErrorCode
#if (STD_ON == DCM_UDS_0X29)
                ,
                connectionId,
                routineIndex
#endif
            );
        }
        if (E_OK == result)
        {
            result = Dcm_UDS0x31_HandleSubFunction(
                OpStatus,
                protocolId,
                routineIndex,
                pMsgContext->reqData[0u],
                &routineSub,
                ErrorCode);
        }

        if (E_OK == result)
        {
            result =
                Dcm_UDS0x31_RoutineCallout(OpStatus, pMsgContext, routineSub, routineIndex, totalLength, ErrorCode);
        }
    }

    return result;
}

/* PRQA S 6040,6010,6030 ++ */ /* VL_MTR_Dcm_STPAR,VL_MTR_Dcm_STCYC,VL_MTR_Dcm_STMIF */
DCM_LOCAL Std_ReturnType Dcm_UDS0x31_RoutineCallout(
    Dcm_ExtendedOpStatusType      OpStatus,
    const Dcm_MsgContextType*     pMsgContext,
    const Dcm_DspRoutineSubType*  routineSub,
    uint16                        routineIndex,
    uint16*                       totalLength,
    Dcm_NegativeResponseCodeType* ErrorCode)
/* PRQA S 6040,6010,6030 -- */
{
    Std_ReturnType            result           = E_OK;
    Dcm_MsgLenType            reqDataLen       = pMsgContext->reqDataLen;
    const Dcm_DspRoutineType* DcmDspRoutineCfg = &Dcm_DspRoutine[routineIndex];
    uint16                    rid              = DcmDspRoutineCfg->RoutineIdentifier;
    const uint8*              reqData          = pMsgContext->reqData;
    uint8*                    resData          = pMsgContext->resData;

    if (0u != routineSub->InSignalNum)
    {
        result = Dcm_UDS0x31_InSignalCheck(reqDataLen, routineSub, ErrorCode);
    }
    else
    {
        if (reqDataLen != DCM_UDS31_MINREQLEN)
        {
            *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
            result     = E_NOT_OK;
        }
    }

    uint16 currentDataLength;
    if (E_OK == result)
    {
        if (pMsgContext->resMaxDataLen >= ((Dcm_MsgLenType)routineSub->OutSignalDataLength + 3uL))
        {
            currentDataLength = (uint16)reqDataLen - 3u - routineSub->InSignalDataLength;
            DcmInternal_Memset(&resData[3u], 0u, routineSub->OutSignalDataLength);
            result = routineSub->RoutineFnc(&reqData[3u], OpStatus, &resData[3u], &currentDataLength, ErrorCode);
            if (E_NOT_OK == result)
            {
                Dcm_RoutineStatus[routineIndex] = DCM_ROUTINE_INITIAL;
            }
            else if (E_OK == result)
            {
                switch (reqData[0u])
                {
#ifdef DCM_UDS_0X31_0X1
                case DCM_UDS31_STARTROUTINE:
                {
                    Dcm_RoutineStatus[routineIndex] = DCM_ROUTINE_START;
                    break;
                }
#endif
#ifdef DCM_UDS_0X31_0X2
                case DCM_UDS31_STOPROUTINE:
                {
                    Dcm_RoutineStatus[routineIndex] = DCM_ROUTINE_STOP;
                    break;
                }
#endif
                default:
                    /* idle */
                    break;
                }
            }
            else if ((DCM_E_PENDING != result) && (DCM_E_FORCE_RCRRP != result))
            {
#if (STD_ON == DCM_DEV_ERROR_DETECT)
                DCM_DET_REPORT(DCM_MAIN_FUNCTION_ID, DCM_E_INTERFACE_RETURN_VALUE);
#endif
                *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
                result     = E_NOT_OK;
            }
            else
            {
                /* idle */
            }
        }
        else
        {
            *ErrorCode = DCM_E_RESPONSETOOLONG;
            result     = E_NOT_OK;
        }
    }

    if (E_OK == result)
    {
        result = Dcm_UDS0x31_OutSignalCheck(routineSub, currentDataLength, totalLength, ErrorCode);
        if ((E_OK == result) && (rid >= DCM_OBD_RID_RANGE_LOWER) && (rid <= DCM_OBD_RID_RANGE_UPPER)
            && (NULL_PTR != DcmDspRoutineCfg->RoutineInfoByte))
        {
            pMsgContext->resData[*totalLength] = *(DcmDspRoutineCfg->RoutineInfoByte);
            (*totalLength)++;
        }
    }

    return result;
}

DCM_LOCAL Std_ReturnType Dcm_UDS0x31_InSignalCheck(
    Dcm_MsgLenType                reqDataLen,
    const Dcm_DspRoutineSubType*  routineSub,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_OK;

    if (routineSub->InSignalVariableLength)
    {
        uint16 maxVariableLength = routineSub->InSignal[routineSub->InSignalNum - 1u].ParameterSize;
        if ((reqDataLen < ((Dcm_MsgLenType)routineSub->InSignalDataLength + 3uL))
            || ((reqDataLen - ((Dcm_MsgLenType)routineSub->InSignalDataLength + 3uL)) > maxVariableLength))
        {
            *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
            result     = E_NOT_OK;
        }
    }
    else
    {
        if (reqDataLen != ((Dcm_MsgLenType)routineSub->InSignalDataLength + 3uL))
        {
            *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
            result     = E_NOT_OK;
        }
    }

    return result;
}

DCM_LOCAL Std_ReturnType Dcm_UDS0x31_OutSignalCheck(
    const Dcm_DspRoutineSubType*  routineSub,
    uint16                        currentDataLength,
    uint16*                       totalLength,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_OK;

    if (0u != routineSub->OutSignalNum)
    {
        *totalLength = 3u + routineSub->OutSignalDataLength;
        if (routineSub->OutSignalVariableLength)
        {
            uint16 maxVariableLength = routineSub->OutSignal[routineSub->OutSignalNum - 1u].ParameterSize;
            if (currentDataLength > maxVariableLength)
            {
                *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
                result     = E_NOT_OK;
            }
            else
            {
                (*totalLength) += currentDataLength;
            }
        }
    }
    else
    {
        *totalLength = 3u;
    }

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
