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
 **  @file               : Os_TrustedFunc.c
 **  @author             : i-soft-os
 **  @date               : 2025/02/10
 **  @vendor             : isoft
 **  @description        : Os source file for TrustedFunc API implementations
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Os_Arch_Processor.h"
#include "Os_TrustedFunc.h"
#include "Os_Sprot.h"
#include "Os_Appl.h"
#include "Os_ProtectHook.h"
#include "Os_Hook.h"
#include "Os_Kernel.h"
#include "Os_Err.h"
#include "Os_Rti.h"
#include "Os_Arti.h"
#include "Os_ReadyQue.h"


/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ============================================ external data definitions =========================================== */

/* ============================================ internal data definitions =========================================== */

/* ========================================== internal function declarations ======================================== */
#if (CFG_TRUSTED_SYSTEM_SERVICE_MAX > 0U)
/**
 * @brief           Calls a trusted function with the given parameters
 * @param[in]       pScb: Pointer to the System Control Block
 * @param[in]       applId: ID of the application owning the trusted function
 * @param[in]       functionIndex: Index of the trusted function to call
 * @param[in]       functionParams: Pointer to parameters for the trusted function
 * @return          -
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_CallTrustedFunction(Os_SCBType *pScb, ApplicationType applId, TrustedFunctionIndexType functionIndex, TrustedFunctionParameterRefType functionParams);
#endif

/* ========================================== external function definitions ========================================= */

#if (CFG_TRUSTED_SYSTEM_SERVICE_MAX > 0U)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Init the TrustedFunction
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_InitTrustedFunction(void)
/* PRQA S 1532 -- */
{
    Os_CoreIdType coreId = Os_GetCoreIdLocal();
    Os_SCBType *pScb = Os_GetSystemContext(coreId);
    uint8 trustedServiceMax = Os_CfgTrustedServiceMax_Inf[coreId]; /* PRQA S 4461 */ /* VL_Os_4461 */

    if (0u < trustedServiceMax)
    {
        pScb->Os_TrustedFuncTp.TrustedFuncNest = 0; /* PRQA S 1290 */ /* VL_Os_1290 */
        pScb->Os_TrustedFuncTp.TrustedFuncTportDelayCall = FALSE;
        pScb->Os_TrustedFuncTp.TrustedFuncTporFlag = FALSE;
        pScb->Os_TrustedFuncTp.TrustedFuncTporErrType = 0; /* PRQA S 1290 */ /* VL_Os_1290 */
        pScb->Os_TrustedFuncTp.TrustedFuncWhoHook = 0; /* PRQA S 1290 */ /* VL_Os_1290 */
        for (uint8 i = 0u; i < trustedServiceMax; i++)
        {
            pScb->TrustedFuncNestQueue[i] = INVALID_OSAPPLICATION;
        }
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Os Call Trusted Function
 */
/* PRQA S 6080, 6070, 6010 ++ */ /* VL_MTR_Os_STPTH, VL_MTR_Os_STCAL, VL_MTR_Os_STCYC */
OS_LOCAL void Os_CallTrustedFunction(
/* PRQA S 6080, 6070, 6010 -- */
    Os_SCBType *pScb,
    ApplicationType applId,
    TrustedFunctionIndexType functionIndex,
    TrustedFunctionParameterRefType functionParams)
{
    OS_HAL_DECLARE_CRITICAL();

    OS_HAL_ENTRY_CRITICAL();
    Os_TrustedFuncTpDataDef *trustedFuncTp = &pScb->Os_TrustedFuncTp;
    /* Save Trusted Function host App ID. */
    pScb->TrustedFuncNestQueue[trustedFuncTp->TrustedFuncNest] = applId;
    /* Save Trusted Function Caller ID. [SWS_Os_00860]*/

    Os_TCBType * pTCB = pScb->SysRunningTCB;
    Os_TaskType  runningTaskId = pScb->SysRunningTaskId;
    Os_PriorityType savePrioTemp = pTCB->TaskRunPrio;
    const Os_ApplicationCfgType *posCurAppCfg = &Os_AppCfg[pScb->SysRunningAppId];
    Os_PriorityType highPriority = posCurAppCfg->AppTaskHighestPriority;
    boolean intSourceFlag[CFG_ISR2_MAX] = {0};

    if(OS_LEVEL_TASK == pScb->SysOsLevel)
    {
        trustedFuncTp->TrustedFuncCaller[trustedFuncTp->TrustedFuncNest] = (uint32)runningTaskId;
    }
    else
    {
        trustedFuncTp->TrustedFuncCaller[trustedFuncTp->TrustedFuncNest] = 
                                    (uint32)(OS_TRUSTED_FUNCTION_ISR2_BIT| pScb->SysRunningIsrCat2Id); /* PRQA S 4522 */ /* VL_Os_4522 */
    }
    /* Save Trusted Function time protection delay call flag*/
    trustedFuncTp->TrustedFuncTportDelayCall = Os_AppCfg[applId].TrustedApplicationDelayTimingViolationCall;
    trustedFuncTp->TrustedFuncNest++;
    if (FALSE == Os_AppCfg[pScb->SysRunningAppId].Trusted)
    {
        if(OS_LEVEL_TASK == pScb->SysOsLevel)
        {
            if (savePrioTemp < highPriority)
            {
                /* PRQA S 4322 ++ */ /* VL_Os_4322 */
                Os_ReadyQueueInsert((const Os_ReadyQueueManageType *)pScb->QueueMg, (Os_TaskType)runningTaskId, OS_LEVEL_TASK, highPriority);
                /* PRQA S 4322 -- */
                pTCB->TaskRunPrio = highPriority;
            }
            if (highPriority > pScb->SysHighPrio)
            {
                pScb->SysHighPrio = highPriority;
                pScb->SysHighTaskId = runningTaskId;
            }
        }
        /* OS447: Disable the interrupt source of the OsIsrs owned by the application */
#if (CFG_ISR_MAX > 0)
        Os_DisableIntSourceInApp(posCurAppCfg, intSourceFlag);
#endif
    }
    OS_HAL_EXIT_CRITICAL();

    /*Os_TrustedServiceList is generate by tool*/
    (*(Os_TrustedServiceList[functionIndex]))(functionIndex, functionParams); /* PRQA S 2842 */ /* VL_Os_2842 */

    OS_HAL_ENTRY_CRITICAL();
    if (FALSE == Os_AppCfg[pScb->SysRunningAppId].Trusted)
    {
#if (CFG_ISR_MAX > 0)
        Os_ResumeIntSourceInApp(posCurAppCfg, intSourceFlag);
#endif

        if(OS_LEVEL_TASK == pScb->SysOsLevel)
        {
            if (savePrioTemp < highPriority)
            {
                Os_ReadyQueueRemove(pScb->QueueMg, pTCB->TaskRunPrio);
                pTCB->TaskRunPrio = savePrioTemp;
            }
            Os_UpdateHighPrioTask(pScb);

#if (CFG_SCHED_POLICY != OS_PREEMPTIVE_NON)
            if (pScb->SysHighTaskId != pScb->SysRunningTaskId)
            {
                if (0u == pScb->SysDispatchLocker)
                {
                    Os_Hal_Dispatch(); /* PRQA S 1006*/ /* VL_Os_1006*/
                }
            }
#endif
        }
    }
    trustedFuncTp->TrustedFuncNest--;
    pScb->TrustedFuncNestQueue[trustedFuncTp->TrustedFuncNest] = INVALID_OSAPPLICATION;

    if (0U == trustedFuncTp->TrustedFuncNest)
    {
        trustedFuncTp->TrustedFuncTportDelayCall = FALSE;
    }
    else
    {
        /* Save the last trusted function time protection delay call flag */
        applId = pScb->TrustedFuncNestQueue[trustedFuncTp->TrustedFuncNest - 1U]; /* PRQA S 1338 */ /* VL_Os_1338 */
        trustedFuncTp->TrustedFuncTportDelayCall = Os_AppCfg[applId].TrustedApplicationDelayTimingViolationCall;
    }
    OS_HAL_EXIT_CRITICAL();
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Call Trusted Function
 */
/* PRQA S 6070, 3408, 3006, 1503, 1512 ++ */ /* VL_MTR_Os_STCAL, VL_Os_3408, VL_Os_3006, VL_QAC_NoUsedApi, VL_Os_1512 */
/* PRQA S 6010, 6030 ++ */ /* VL_MTR_Os_STCYC, VL_MTR_Os_STMIF */
StatusType CallTrustedFunction(
    TrustedFunctionIndexType FunctionIndex,
    TrustedFunctionParameterRefType FunctionParams)
/* PRQA S 6010, 6030 -- */
/* PRQA S 6070, 3408, 3006, 1503, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_CallTrustedFunction);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_CallTrustedFunction_Start, FunctionIndex);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    StatusType err = E_OK;

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (NULL_PTR == FunctionParams)
    {
        err = E_OS_ILLEGAL_ADDRESS;
    }
    else
#endif
    {
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        Os_ServicePortParamType SprotParam = {
            .AllowedContext = OS_SERVICEPORT_CHECK_CALL_TRUSTED_FUNCTION,
            .ObjectType = OS_OBJECT_INVALID,
            .ObjectID = (Os_AppObjectId)OS_OBJECT_INVALID,
            .Address = (uint32)FunctionParams, /* PRQA S 0326 */ /* VL_Os_0326 */
        };
        err = Os_ServiceProtCheck(pScb, &SprotParam);
#endif

        if (E_OK == err)
        {
            ApplicationType applId = (OS_HIGH_8_BITS_MASK & FunctionIndex) >> OS_EIGHT;
            FunctionIndex = OS_LOW_8_BITS_MASK & FunctionIndex; /* PRQA S 1338 */ /* VL_Os_1338 */
/* On a Multi-Core system, these trusted function calls from
 * one OS-Application to another are limited to the same core.*/
#if (OS_AUTOSAR_CORES > 1)
            Os_CoreIdType ownerCore = Os_AppCfg[applId].HostCore;
            if (ownerCore != Os_GetCoreIdLocal())
            {
                err = E_OS_ACCESS;
            }
            else
#endif
            if (FunctionIndex >= CFG_TRUSTED_SYSTEM_SERVICE_MAX)
            {
                err = E_OS_SERVICEID;
            }
            else if (pScb->Os_TrustedFuncTp.TrustedFuncNest >= CFG_TRUSTED_SYSTEM_SERVICE_MAX)
            {
                err = E_OS_LIMIT;
            }
            else
            {
                Os_CallTrustedFunction(pScb, applId, FunctionIndex, FunctionParams);
            }

            /* Time protection and delay processing are triggered in the trusted function */
            Os_TrustedFuncTpDataDef *trustedFuncTp = &pScb->Os_TrustedFuncTp;
            if ((TRUE == trustedFuncTp->TrustedFuncTporFlag) && (FALSE == trustedFuncTp->TrustedFuncTportDelayCall))
            {
                /* Hook. */
                (void)Os_CallProtectionHook(trustedFuncTp->TrustedFuncTporErrType, trustedFuncTp->TrustedFuncWhoHook);
                trustedFuncTp->TrustedFuncTporFlag = FALSE;
                trustedFuncTp->TrustedFuncTporErrType = 0; /* PRQA S 1290 */ /* VL_Os_1290 */
                trustedFuncTp->TrustedFuncWhoHook = 0; /* PRQA S 1290 */ /* VL_Os_1290 */
            }
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_CallTrustedFunction(FunctionIndex, FunctionParams),
                          OSServiceId_CallTrustedFunction, err, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_CallTrustedFunction);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_CallTrustedFunction_Return, err);
    OS_HAL_EXIT_KERNEL();/* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * To check if the current error is caused by TrustedFunc and if the error is fatal.
 */
/* PRQA S 1532, 3673 ++ */ /* VL_QAC_OneFunRef, VL_QAC_3673 */
void Os_TrustedFuncCallerCheck(StatusType errType, uint32 whoHook, Os_SCBType *pScb, Os_ProtectionReturnType *Ret)
/* PRQA S 1532, 3673 -- */
{
    Os_TrustedFuncTpDataDef *trustedFuncTp = &pScb->Os_TrustedFuncTp; /* PRQA S 3678 */ /* VL_Os_3678 */
    uint8 FuncNest = trustedFuncTp->TrustedFuncNest;

    if((FuncNest > 0U) && ((errType == E_OS_PROTECTION_EXCEPTION) || (errType == E_OS_PROTECTION_MEMORY)))
    {
        uint32 RunningThreadId = NULL_PARA; /* PRQA S 1258 */ /* VL_Os_1258 */
        if (whoHook == OS_TMPROT_HOOK_TASK)
        {
            RunningThreadId = (uint32)pScb->SysRunningTaskId;
        }
        else if (whoHook == OS_TMPROT_HOOK_ISR) /* PRQA S 2004 */ /* VL_Os_2004 */
        {
            RunningThreadId = (uint32)(OS_TRUSTED_FUNCTION_ISR2_BIT| pScb->SysRunningIsrCat2Id); /* PRQA S 4522 */ /* VL_Os_4522 */
        }

        if(RunningThreadId == trustedFuncTp->TrustedFuncCaller[FuncNest - 1U])
        {
            *Ret = PRO_SHUTDOWN; /* SWS_Os_00860 */
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif
/* PRQA S 0553 EOF */ /* VL_QAC_UnUsedFiles */
