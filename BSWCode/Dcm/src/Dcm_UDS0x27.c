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
/*
 ************************************************************************************************************************
 **
 **  @file               : Dcm_UDS0x27.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm UDS service 0x27 function implementation
 **
 ***********************************************************************************************************************/
/* PRQA S 3120, 1532, 1505 EOF */ /* VL_QAC_MagicNum, VL_QAC_OneFunRef, VL_QAC_OneFunRef */
/* =================================================== inclusions =================================================== */
#include "Dcm_Cfg.h"
#if (STD_ON == DCM_UDS_0X27)
#ifdef SEV_UDS_ECU_UNLOCK_SUCCESSFUL
#include "IdsM.h"
#endif
#include "Dcm_Internal.h"
/* ===================================================== macros ===================================================== */
/**
 * @brief The minimum request length of UDS 0x27
 */
#define DCM_UDS27_MINREQLEN 1uL
/* ============================================ external data definitions =========================================== */
#define DCM_START_SEC_VAR_INIT_8
#include "Dcm_MemMap.h"
/**
 * @brief store the previous requested seed security level
 */
uint8 Dcm_reqSeedSecurityLevel = 0u;
#define DCM_STOP_SEC_VAR_INIT_8
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
/**
 * @brief security control unit
 */
Dcm_SecurityCtrlType Dcm_SecurityCtrl[DCM_SECURITY_NUM];
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
#if (STD_ON == DCM_SECURITY_READOUT)
#define DCM_START_SEC_VAR_INIT_16
#include "Dcm_MemMap.h"
/**
 * @brief current security readout timer for initial read
 */
uint32 Dcm_SecurityReadoutTimer = 0u;
#define DCM_STOP_SEC_VAR_INIT_16
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_INIT_BOOLEAN
#include "Dcm_MemMap.h"
/**
 * @brief indicates if there is pending getAttemptCounter globally
 */
boolean Dcm_PendingGetAttemptCounter = TRUE;
/**
 * @brief indicates if there is pending setAttemptCounter globally
 */
boolean Dcm_PendingSetAttemptCounter = TRUE;
#define DCM_STOP_SEC_VAR_INIT_BOOLEAN
#include "Dcm_MemMap.h"
#endif
/* ========================================= internal function declarations ========================================= */
/**
 * @brief         This function deal with request seed subfunction
 * @param[in]     OpStatus      : current operation status
 * @param[in]     securityIndex : the requested security row configuration
 * @param[in]     securityLevel : the requested security level
 * @param[inout]  pMsgContext   : Message-related information for one diagnostic protocol identifier. The pointers in
 *                                pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode     : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm
 *                                module shall send a negative response with NRC code equal to the parameter
 *                                ErrorCode parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was unsuccessful
 * @reentrant     TRUE
 * @synchronous   Synchronous
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x27_RequestSeed(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint8                         securityIndex,
    uint8                         securityLevel,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         This function deal with send key subfunction
 * @param[in]     OpStatus      : current operation status
 * @param[in]     securityIndex : the requested security row configuration
 * @param[in]     securityLevel : the requested security level
 * @param[inout]  pMsgContext   : Message-related information for one diagnostic protocol identifier. The pointers in
 *                                pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode     : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm
 *                                module shall send a negative response with NRC code equal to the parameter
 *                                ErrorCode parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was unsuccessful
 * @reentrant     TRUE
 * @synchronous   Synchronous
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x27_SendKey(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint8                         securityIndex,
    uint8                         securityLevel,
    const Dcm_MsgContextType*     pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         This function deal with request seed callout
 * @param[in]     OpStatus      : current operation status
 * @param[in]     securityLevel : the requested security level
 * @param[in]     securityRow   : the requested security row configuration
 * @param[inout]  pMsgContext   : Message-related information for one diagnostic protocol identifier. The pointers in
 *                                pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode     : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm
 *                                module shall send a negative response with NRC code equal to the parameter
 *                                ErrorCode parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was unsuccessful
 * @reentrant     TRUE
 * @synchronous   Synchronous
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x27_RequestSeedProcess(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint8                         securityLevel,
    const Dcm_MsgContextType*     pMsgContext,
    const Dcm_DspSecurityRowType* securityRow,
    Dcm_NegativeResponseCodeType* ErrorCode);

#if (STD_ON == DCM_SECURITY_READOUT)
/**
 * @brief         This function deal with getAttemptCounter callout
 * @param[in]     OpStatus      : current operation status
 * @param[in]     currentResult : the result of previous operation
 * @param[in]     index         : the requested security row configuration index
 * @param[inout]  finish        : whether the whole process if finished
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was unsuccessful
 * @reentrant     TRUE
 * @synchronous   Synchronous
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x27_GetAttemptCounterProcess(
    Std_ReturnType   currentResult,
    uint8            index,
    Dcm_OpStatusType OpStatus,
    boolean*         finish);
#endif
/* ========================================== external function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * The service interpreter for UDS 0x27
 */
Std_ReturnType Dcm_UDS0x27(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    uint8          securityIndex;
    Std_ReturnType result      = E_OK;
    const uint8*   reqData     = pMsgContext->reqData;
    uint8*         resData     = pMsgContext->resData;
    uint8          subFunction = reqData[0u];
    uint8          securityLevel;

    securityLevel = (subFunction / 2u) + (subFunction % 2u);
    DcmInternal_FindSecurityIndex(securityLevel, &securityIndex);

    /* Request Seed */
    if (1u == (subFunction % 2u))
    {
        result = Dcm_UDS0x27_RequestSeed(OpStatus, securityIndex, securityLevel, pMsgContext, ErrorCode);
    }
    /* Send Key */
    else
    {
        result = Dcm_UDS0x27_SendKey(OpStatus, securityIndex, securityLevel, pMsgContext, ErrorCode);
    }

    if (E_OK == result)
    {
        resData[0u] = subFunction;
        pMsgContext->resDataLen++;
    }

    return result;
}

/**
 * check whether current security satisfies
 */
Std_ReturnType Dcm_UDS0x27_CheckSecurity(
    const Dcm_SecLevelType*       security,
    uint8                         securityNum,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_NOT_OK;

    if ((0u == securityNum) || (NULL_PTR == security))
    {
        result = E_OK;
    }
    else
    {
        for (uint8 index = 0u; index < securityNum; index++)
        {
            if (Dcm_CurrentSecurity == security[index])
            {
                result = E_OK;
                break;
            }
        }
    }

    if ((E_NOT_OK == result) && (NULL_PTR != ErrorCode))
    {
        *ErrorCode = DCM_E_SECURITYACCESSDENIED;
    }

    return result;
}

/**
 * deal with security access dealy timer
 */
void Dcm_UDS0x27_CheckTimer(void)
{
    for (uint8 index = 0u; index < DCM_SECURITY_NUM; index++)
    {
        if (DcmInternal_DecreaseTimer(&Dcm_SecurityCtrl[index].SecurityTimer))
        {
#if (STD_ON == DCM_SECURITY_ATTEMPT_COUNTER_ON_TIMEOUT)
            Dcm_UDS0x27_SetAttemptCounter(DCM_INITIAL, index, 0u);
#else
            uint8 attemptCounter =
                (Dcm_SecurityCtrl[index].AttemptCounter > 0u) ? (Dcm_SecurityCtrl[index].AttemptCounter - 1u) : 0u;
            Dcm_UDS0x27_SetAttemptCounter(DCM_INITIAL, index, attemptCounter);
#endif
        }
    }
}

#if (STD_ON == DCM_SECURITY_READOUT)
/**
 * get current attempt counter during the first few calls of mainFunction
 */
void Dcm_UDS0x27_GetAttemptCounter(void)
{
    Dcm_OpStatusType opStatus = (0uL == Dcm_SecurityReadoutTimer) ? DCM_CANCEL : DCM_PENDING;
    boolean          finish   = TRUE;

    for (uint8 index = 0u; index < DCM_SECURITY_NUM; index++)
    {
        const Dcm_DspSecurityRowType* securityRowCfg = &Dcm_DspSecurityRow[index];
        Dcm_SecurityCtrlType*         securityCtrPtr = &Dcm_SecurityCtrl[index];
        Std_ReturnType                result         = E_OK;
        result = Dcm_UDS0x27_GetAttemptCounterProcess(result, index, opStatus, &finish);
        if (!finish)
        {
            break;
        }
        if (E_NOT_OK == result)
        {
            securityCtrPtr->PendingGetAttemptCounter = FALSE;
            securityCtrPtr->AttemptCounter           = securityRowCfg->NumAttDelay;
            securityCtrPtr->SecurityTimer            = (securityRowCfg->DelayTime > securityRowCfg->DelayTimeOnBoot)
                                                           ? securityRowCfg->DelayTime
                                                           : securityRowCfg->DelayTimeOnBoot;
        }
    }

    if (finish)
    {
        Dcm_PendingGetAttemptCounter = FALSE;
    }
    else
    {
        DcmInternal_DecreaseTimer(&Dcm_SecurityReadoutTimer); /* PRQA S 3200 */ /* VL_Dcm_3200 */
    }
}

/**
 * set attempt counter after change
 */
void Dcm_UDS0x27_SetAttemptCounterHandle(void)
{
    boolean finish = TRUE;

    for (uint8 index = 0u; index < DCM_SECURITY_NUM; index++)
    {
        Dcm_SecurityCtrlType* securityCtrPtr = &Dcm_SecurityCtrl[index];
        if (securityCtrPtr->PendingSetAttemptCounter)
        {
            finish = FALSE;
            Dcm_UDS0x27_SetAttemptCounter(DCM_PENDING, index, securityCtrPtr->AttemptCounter);
        }
        else if (securityCtrPtr->DelayedSetAttemptCounter)
        {
            finish = FALSE;
            Dcm_UDS0x27_SetAttemptCounter(DCM_INITIAL, index, securityCtrPtr->AttemptCounter);
            securityCtrPtr->DelayedSetAttemptCounter = FALSE;
        }
        else
        {
            /* idle */
        }
    }
    if (finish)
    {
        Dcm_PendingSetAttemptCounter = FALSE;
    }
}
#endif

/**
 * actual handle for setAttemptCounter
 */
void Dcm_UDS0x27_SetAttemptCounter(Dcm_OpStatusType OpStatus, uint8 securityIndex, uint8 attemptCounter)
{
    const Dcm_DspSecurityRowType* securityRowCfg = &Dcm_DspSecurityRow[securityIndex];
    Dcm_SecurityCtrlType*         securityCtrPtr = &Dcm_SecurityCtrl[securityIndex];

    securityCtrPtr->AttemptCounter = attemptCounter;
#if (STD_ON == DCM_SECURITY_READOUT)
    if (securityRowCfg->AttemptCounterEnabled)
    {
        if ((DCM_INITIAL != OpStatus) || (!securityCtrPtr->PendingSetAttemptCounter))
        {
            Std_ReturnType result = securityRowCfg->SetAttemptCounterFnc(OpStatus, attemptCounter);
            if (DCM_E_PENDING == result)
            {
                securityCtrPtr->PendingSetAttemptCounter = TRUE;
                Dcm_PendingSetAttemptCounter             = TRUE;
            }
            else if ((E_OK == result) || (E_NOT_OK == result))
            {
                securityCtrPtr->PendingSetAttemptCounter = FALSE;
            }
            else
            {
#if (STD_ON == DCM_DEV_ERROR_DETECT)
                DCM_DET_REPORT(DCM_MAIN_FUNCTION_ID, DCM_E_INTERFACE_RETURN_VALUE);
#endif
            }
        }
        else
        {
            securityCtrPtr->DelayedSetAttemptCounter = TRUE;
            Dcm_PendingSetAttemptCounter             = TRUE;
        }
    }
#endif
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

/* ========================================== internal function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/* This function deal with request seed subfunction */
DCM_LOCAL Std_ReturnType Dcm_UDS0x27_RequestSeed(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint8                         securityIndex,
    uint8                         securityLevel,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    const Dcm_DspSecurityRowType* securityRow = &Dcm_DspSecurityRow[securityIndex];
    Std_ReturnType                result      = E_OK;

    if ((DCM_UDS27_MINREQLEN + securityRow->AdrSize) != pMsgContext->reqDataLen)
    {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        result     = E_NOT_OK;
    }
#if (STD_ON == DCM_SECURITY_READOUT)
    else
    {
        for (uint8 index = 0u; index < DCM_SECURITY_NUM; index++)
        {
            const Dcm_SecurityCtrlType* tempSecurityCtrlPtr = &Dcm_SecurityCtrl[index];
            if ((tempSecurityCtrlPtr->PendingGetAttemptCounter) || (tempSecurityCtrlPtr->InitialGetAttemptCounter))
            {
                result     = E_NOT_OK;
                *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
                break;
            }
        }
    }
#endif

    if (E_OK == result)
    {
        if (0uL != Dcm_SecurityCtrl[securityIndex].SecurityTimer)
        {
            *ErrorCode = DCM_E_REQUIREDTIMEDELAYNOTEXPIRED;
            result     = E_NOT_OK;
        }
        else if ((securityRow->SeedSize + 1uL) > pMsgContext->resMaxDataLen)
        {
            *ErrorCode = DCM_E_RESPONSETOOLONG;
            result     = E_NOT_OK;
        }
        else
        {
            /* idle */
        }
    }

    if (E_OK == result)
    {
        result = Dcm_UDS0x27_RequestSeedProcess(OpStatus, securityLevel, pMsgContext, securityRow, ErrorCode);
    }

    if (E_OK == result)
    {
        pMsgContext->resDataLen += securityRow->SeedSize;
    }
    else
    {
        Dcm_reqSeedSecurityLevel = 0u;
    }

    return result;
}

/* This function deal with request seed callout */
DCM_LOCAL Std_ReturnType Dcm_UDS0x27_RequestSeedProcess(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint8                         securityLevel,
    const Dcm_MsgContextType*     pMsgContext,
    const Dcm_DspSecurityRowType* securityRow,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_OK;

    if (Dcm_CurrentSecurity == securityLevel)
    {
        DcmInternal_Memset(&pMsgContext->resData[DCM_UDS27_MINREQLEN], 0u, securityRow->SeedSize);
        Dcm_reqSeedSecurityLevel = 0u;
    }
    else
    {
        Dcm_reqSeedSecurityLevel = securityLevel;
        result                   = securityRow->GetSeedFnc(
            &pMsgContext->reqData[DCM_UDS27_MINREQLEN],
            OpStatus,
            &pMsgContext->resData[DCM_UDS27_MINREQLEN],
            ErrorCode);
#if (STD_ON == DCM_DEV_ERROR_DETECT)
        if ((E_OK != result) && (E_NOT_OK != result) && (DCM_E_PENDING != result))
        {
            DCM_DET_REPORT(DCM_MAIN_FUNCTION_ID, DCM_E_INTERFACE_RETURN_VALUE);
            *ErrorCode = DCM_E_GENERALREJECT;
            result     = E_NOT_OK;
        }
#endif
    }

    return result;
}

DCM_LOCAL Std_ReturnType Dcm_UDS0x27_SendKey(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint8                         securityIndex,
    uint8                         securityLevel,
    const Dcm_MsgContextType*     pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType                result;
    Dcm_SecurityCtrlType*         securityCtrlPtr = &Dcm_SecurityCtrl[securityIndex];
    const Dcm_DspSecurityRowType* securityRow     = &Dcm_DspSecurityRow[securityIndex];

    if (Dcm_reqSeedSecurityLevel != securityLevel)
    {
        *ErrorCode = DCM_E_REQUESTSEQUENCEERROR;
        result     = E_NOT_OK;
    }
    else if ((DCM_UDS27_MINREQLEN + securityRow->KeySize) != pMsgContext->reqDataLen)
    {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        result     = E_NOT_OK;
    }
    else
    {
        result = securityRow->CompareKeyFnc(&pMsgContext->reqData[1u], OpStatus, ErrorCode);
        switch (result)
        {
        case E_OK:
        {
            Dcm_reqSeedSecurityLevel = 0u;
#ifdef SEV_UDS_ECU_UNLOCK_SUCCESSFUL
            IdsM_SetSecurityEvent(SEV_UDS_COMMUNICATION_CONTROL_SWITCHED_OFF);
#endif
            Dcm_UDS0x27_SetAttemptCounter(DCM_INITIAL, securityIndex, 0u);
            DslInternal_SetSecurityLevel(securityRow->SecurityLevel);
            break;
        }
        case DCM_E_COMPARE_KEY_FAILED:
        {
            Dcm_UDS0x27_SetAttemptCounter(DCM_INITIAL, securityIndex, securityCtrlPtr->AttemptCounter + 1u);
            if ((0u == securityRow->NumAttDelay) || (securityCtrlPtr->AttemptCounter < securityRow->NumAttDelay))
            {
                *ErrorCode = DCM_E_INVALIDKEY;
            }
            else
            {
                securityCtrlPtr->SecurityTimer = securityRow->DelayTime;
                *ErrorCode                     = DCM_E_EXCEEDNUMBEROFATTEMPTS;
            }
            result = E_NOT_OK;
            break;
        }
        case E_NOT_OK:
        case DCM_E_PENDING:
        {
            /* idle */
            break;
        }
        default:
        {
#if (STD_ON == DCM_DEV_ERROR_DETECT)
            DCM_DET_REPORT(DCM_MAIN_FUNCTION_ID, DCM_E_INTERFACE_RETURN_VALUE);
            *ErrorCode = DCM_E_GENERALREJECT;
            result     = E_NOT_OK;
#endif
            break;
        }
        }
    }

    if (E_NOT_OK == result)
    {
        Dcm_reqSeedSecurityLevel = 0u;
    }

    return result;
}

#if (STD_ON == DCM_SECURITY_READOUT)
/* This function deal with getAttemptCounter callout */
/* PRQA S 6030++ */ /* VL_MTR_Dcm_STMIF */
DCM_LOCAL Std_ReturnType Dcm_UDS0x27_GetAttemptCounterProcess(
    Std_ReturnType   currentResult,
    uint8            index,
    Dcm_OpStatusType OpStatus,
    boolean*         finish)
{
    Std_ReturnType                result         = currentResult;
    const Dcm_DspSecurityRowType* securityRowCfg = &Dcm_DspSecurityRow[index];
    Dcm_SecurityCtrlType*         securityCtrPtr = &Dcm_SecurityCtrl[index];

    if (securityCtrPtr->PendingGetAttemptCounter)
    {
        result = securityRowCfg->GetAttemptCounterFnc(OpStatus, &securityCtrPtr->AttemptCounter);
        if ((DCM_CANCEL == OpStatus) || (E_NOT_OK == result)
            || ((E_OK == result) && (securityCtrPtr->AttemptCounter >= securityRowCfg->NumAttDelay)))
        {
            result = E_NOT_OK;
        }
        switch (result)
        {
        case E_OK:
        {
            securityCtrPtr->PendingGetAttemptCounter = FALSE;
            break;
        }
        case DCM_E_PENDING:
        {
            *finish = FALSE;
            break;
        }
        case E_NOT_OK:
        {
            /* idle */
            break;
        }
        default:
        {
#if (STD_ON == DCM_DEV_ERROR_DETECT)
            DCM_DET_REPORT(DCM_MAIN_FUNCTION_ID, DCM_E_INTERFACE_RETURN_VALUE);
            result = E_NOT_OK;
#endif
            break;
        }
        }
    }
    else if (securityCtrPtr->InitialGetAttemptCounter)
    {
        securityCtrPtr->InitialGetAttemptCounter = FALSE;
        securityCtrPtr->AttemptCounter           = 0u;
        if (DCM_CANCEL != OpStatus)
        {
            result = securityRowCfg->GetAttemptCounterFnc(DCM_INITIAL, &securityCtrPtr->AttemptCounter);
            if (E_OK == result)
            {
                securityCtrPtr->SecurityTimer = securityRowCfg->DelayTimeOnBoot;
            }
            else if (DCM_E_PENDING == result)
            {
                *finish                                  = FALSE;
                securityCtrPtr->PendingGetAttemptCounter = TRUE;
            }
            else
            {
                result = E_NOT_OK;
            }
        }
        else
        {
            result = E_NOT_OK;
        }
    }
    else
    {
        result = E_NOT_OK;
    }

    return result;
}
/* PRQA S 6030-- */
#endif
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
