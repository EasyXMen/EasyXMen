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
 **  @file               : Os_Fault.c
 **  @author             : tong.yin
 **  @date               : 2025/02/17
 **  @vendor             : isoft
 **  @description        : Os source file for Fault API implementations
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Os_Hal_Define.h"
#include "Os_FaultManager.h"
#include "Os_Kernel.h"
#include "Os_Task.h"
#include "Os_Interrupt.h"
#include "Os_Extend.h"
#include "Os_Sprot.h"
#include "Os_ProtectHook.h"
#include "Os_Hook.h"
#include "Os_StackMonitor.h"

/* ===================================================== macros ===================================================== */
#if (TRUE == CFG_FAULT_MANAGEMENT_ENABLE)

/* Load rate threshold is 90% */
#define FAULT_LOAD_RATIO_MAX (9000u)

/* ================================================ type definitions ================================================ */

/* ============================================ external data definitions =========================================== */
#if (TRUE == CFG_HARDWARE_FAULT_ENABLE)
#define OS_START_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "Os_MemMap.h"
/* PRQA S 3408, 1504, 1512 ++ */ /* VL_Os_3408, VL_QAC_OneRefSymbol, VL_Os_1512 */
Os_ExceptionContextType Os_HalContextExceptionContext[OS_AUTOSAR_CORES];
/* PRQA S 3408, 1504, 1512 -- */
#define OS_STOP_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "Os_MemMap.h"

/* ========================================== internal function declarations ======================================== */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           Saves current object runtime information (lightweight)
 * @param[in]       pScb: Pointer to system control block
 * @param[in]       systemInf: Pointer to system information
 * @param[in]       err: Error code
 * @param[in]       whoHook: Who triggered the hook
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_FaultRecordCurrentObject(
    const Os_SCBType*     pScb,
    Os_SystemInformation* systemInf,
    StatusType            err,
    uint32                whoHook);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           Saves hardware exception context information
 * @param[in]       pScb: Pointer to system control block
 * @param[in]       TrapMsg: Pointer to trap context information
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_FaultSaveExceptionContext(const Os_SCBType* pScb, const Os_TrapContextType* TrapMsg);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           Saves task runtime information
 * @param[in]       pScb: Pointer to system control block
 * @param[in]       systemInf: Pointer to system information
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_FaultRecordTaskRuntimeInform(
    const Os_SCBType*           pScb,
    const Os_SystemInformation* systemInf,
    StatusType                  err,
    uint32                      whoHook);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           Saves ISR2 runtime information
 * @param[in]       pScb: Pointer to system control block
 * @param[in]       systemInf: Pointer to system information
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_FaultRecordIsr2RuntimeInform(
    const Os_SCBType*           pScb,
    const Os_SystemInformation* systemInf,
    StatusType                  err,
    uint32                      whoHook);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_HARDWARE_FAULT_ENABLE */

/* ========================================== external function definitions ========================================= */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Init the Fault management module
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_InitFaultManagement(void)
/* PRQA S 1532 -- */
{
    Os_CoreIdType coreId = Os_GetCoreIdLocal();
    Os_SCBType*   pScb   = Os_GetSystemContext(coreId);

    pScb->FaultSySInformation = Os_FaultSySInformation_Inf[coreId];

#if (TRUE == CFG_HARDWARE_FAULT_ENABLE)
    Os_TrapMsgType* trapMsg         = &pScb->FaultSySInformation->TrapContext.TrapMsg;
    trapMsg->ExceptionAddress       = 0U;
    trapMsg->ExceptionCase          = 0U;
    trapMsg->ExceptionCaseMsg       = NULL_PTR;
    trapMsg->ExceptionSP            = 0U;
    trapMsg->ExceptionSysteamStatus = 0U;
#endif /* TRUE == CFG_HARDWARE_FAULT_ENABLE */
    Os_SystemInformation* systemInf = &pScb->FaultSySInformation->SystemInf;

    pScb->FaultSySInformation->IsHardFault = FALSE;

    systemInf->SysOsLevel = OS_LEVEL_MAIN;
#if (CFG_OSAPPLICATION_MAX > 0)
    systemInf->SysRunningAppId = INVALID_OSAPPLICATION; /* PRQA S 1461, 1297*/ /* VL_Os_1461*/ /* VL_Os_1297*/
#endif
    systemInf->SysRunningTaskId    = INVALID_TASK;
    systemInf->SysRunningIsrCat2Id = INVALID_ISR; /* PRQA S 1297*/ /* VL_Os_1297*/
    systemInf->SysInIsrCat2        = FALSE;
    systemInf->SysDispatchLocker   = 0U;
    systemInf->IntNestISR2         = 0U;
    systemInf->IntDisableFlag      = FALSE;
    systemInf->TaskRuntimeInf      = Os_FaultTaskRuntimeInf;
    systemInf->IsrRuntimeInf       = Os_FaultIsrRuntimeInf;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Save current object runtime information (lightweight)
 */
/* PRQA S 3450 ++ */ /* VL_Os_3450 */
OS_LOCAL void
    Os_FaultRecordCurrentObject(const Os_SCBType* pScb, Os_SystemInformation* systemInf, StatusType err, uint32 whoHook)
/* PRQA S 3450 -- */
{
    TaskIsrRuntimeInformType runtimeInf;
    /* PRQA S 2982 ++ */ /* VL_Os_2982 */
    runtimeInf.State      = 0u;
    runtimeInf.StackUsage = 0u;
    runtimeInf.LoadRatio  = 0u;
    runtimeInf.ServiceId  = 0u;
    runtimeInf.ErrorCode  = 0u;
    runtimeInf.Reserved = 0u;
    /* PRQA S 2982 -- */

    if (OS_LEVEL_TASK == pScb->SysOsLevel)
    {
        systemInf->SysRunningTaskId = pScb->SysRunningTaskId;

#if (TRUE == CFG_STACK_USAGE_CHECK)
        const Os_StackType* stack = &Os_TaskStack[pScb->SysRunningTaskId];
        runtimeInf.StackUsage     = Os_CalcStackUsageLevel(stack);
#endif

#if (TRUE == CFG_LOAD_RATIO_CALC_ENABLE)
        uint32 averageLoadRatio = pScb->LoadRatioMonitor.Os_TaskLoadRatio[pScb->SysRunningTaskId].averageLoadRatio;
        if (averageLoadRatio > FAULT_LOAD_RATIO_MAX)
        {
            runtimeInf.LoadRatio = 1u;
        }
#endif

        runtimeInf.State = (uint8)Os_TaskState[pScb->SysRunningTaskId];
#if (TRUE == CFG_USEGETSERVICEID)
        runtimeInf.ServiceId = (uint8)pScb->SysServiceId;
#endif
        runtimeInf.ErrorCode                              = err;
        systemInf->TaskRuntimeInf[pScb->SysRunningTaskId] = runtimeInf;
    }
    else if (OS_LEVEL_ISR2 == pScb->SysOsLevel)
    {
        systemInf->SysRunningIsrCat2Id = pScb->SysRunningIsrCat2Id;

#if (TRUE == CFG_STACK_USAGE_CHECK)
        const Os_StackType* stack = &Os_ISR2Stack[pScb->SysRunningIsrCat2Id];
        runtimeInf.StackUsage     = Os_CalcStackUsageLevel(stack);
#endif

#if (TRUE == CFG_LOAD_RATIO_CALC_ENABLE)
        uint32 averageLoadRatio = pScb->LoadRatioMonitor.Os_IsrLoadRatio[pScb->SysRunningIsrCat2Id].averageLoadRatio;
        if (averageLoadRatio > FAULT_LOAD_RATIO_MAX)
        {
            runtimeInf.LoadRatio = 1u;
        }
#endif

        runtimeInf.State = 1u;
#if (TRUE == CFG_USEGETSERVICEID)
        runtimeInf.ServiceId = (uint8)pScb->SysServiceId;
#endif
        runtimeInf.ErrorCode                                = err;
        systemInf->IsrRuntimeInf[pScb->SysRunningIsrCat2Id] = runtimeInf;
    }
    else
    {
        /* Nothing to do for other levels */
    }

    UNUSED_PARAMETER(whoHook);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Save Task runtime information
 */
/* PRQA S 3450 ++ */ /* VL_Os_3450 */
OS_LOCAL void Os_FaultRecordTaskRuntimeInform(
    const Os_SCBType*           pScb,
    const Os_SystemInformation* systemInf,
    StatusType                  err,
    uint32                      whoHook)
/* PRQA S 3450 -- */
{
    TaskIsrRuntimeInformType taskRuntimeInf;
    /* PRQA S 2982 ++ */ /* VL_Os_2982 */
    taskRuntimeInf.State      = 0u;
    taskRuntimeInf.StackUsage = 0u;
    taskRuntimeInf.LoadRatio  = 0u;
    taskRuntimeInf.ServiceId  = 0u;
    taskRuntimeInf.ErrorCode  = 0u;
    taskRuntimeInf.Reserved = 0u;
    /* PRQA S 2982 -- */

    Os_CoreIdType coreId       = pScb->SysCore;
    Os_TaskType   idStartRange = Os_TaskIdRange[coreId].AllTask.Start;
    Os_TaskType   idEndRange   = Os_TaskIdRange[coreId].AllTask.End;

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    Os_TaskType TProtTaskId = INVALID_TASK;

    /*  Timing Protection task fault */
    if (whoHook == OS_TMPROT_HOOK_TASK)
    {
        if ((err == E_OS_PROTECTION_TIME) || (err == E_OS_PROTECTION_LOCKED))
        {
            TProtTaskId = pScb->SysRunningTaskId;
        }
    }
#endif

    for (uint16 i = (uint16)idStartRange; i < (uint16)idEndRange; i++)
    {
        uint8 taskState  = 0u;
        uint8 stackUsage = 0u;
        uint8 loadRatio  = 0u;
        uint8 serviceId  = 0u;
        uint8 errorCode  = 0u;

        taskState = (uint8)Os_TaskState[i];

#if (TRUE == CFG_STACK_USAGE_CHECK)
        const Os_StackType* stack = &Os_TaskStack[i];
        stackUsage                = Os_CalcStackUsageLevel(stack);
#endif

#if (TRUE == CFG_LOAD_RATIO_CALC_ENABLE)
        uint32 averageLoadRatio = pScb->LoadRatioMonitor.Os_TaskLoadRatio[i].averageLoadRatio;
        if (averageLoadRatio > FAULT_LOAD_RATIO_MAX)
        {
            loadRatio = 1u;
        }
#endif

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        if (i == (uint16)TProtTaskId)
        {
#if (TRUE == CFG_USEGETSERVICEID)
            serviceId = (uint8)pScb->SysServiceId;
#endif
            errorCode = err;
        }
#endif
        if ((OS_LEVEL_TASK == pScb->SysOsLevel) && (i == (uint16)pScb->SysRunningTaskId))
        {
#if (TRUE == CFG_USEGETSERVICEID)
            serviceId = (uint8)pScb->SysServiceId;
#endif
            errorCode = err;
        }

        /* state | stackUsage | loadRatio | serviceId | errorCode
          4bit       2bit         2bit        8bit         8bit  */
        taskRuntimeInf.State         = taskState;
        taskRuntimeInf.StackUsage    = stackUsage;
        taskRuntimeInf.LoadRatio     = loadRatio;
        taskRuntimeInf.ServiceId     = serviceId;
        taskRuntimeInf.ErrorCode     = errorCode;
        systemInf->TaskRuntimeInf[i] = taskRuntimeInf;
    }

    UNUSED_PARAMETER(whoHook);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Save Isr2 runtime information
 */
/* PRQA S 3450 ++ */ /* VL_Os_3450 */
OS_LOCAL void Os_FaultRecordIsr2RuntimeInform(
    const Os_SCBType*           pScb,
    const Os_SystemInformation* systemInf,
    StatusType                  err,
    uint32                      whoHook)
/* PRQA S 3450 -- */
{
    TaskIsrRuntimeInformType isrRuntimeInf;
    /* PRQA S 2982 ++ */ /* VL_Os_2982 */
    isrRuntimeInf.State      = 0u;
    isrRuntimeInf.StackUsage = 0u;
    isrRuntimeInf.LoadRatio  = 0u;
    isrRuntimeInf.ServiceId  = 0u;
    isrRuntimeInf.ErrorCode  = 0u;
    isrRuntimeInf.Reserved = 0u;
    /* PRQA S 2982 -- */

    Os_CoreIdType coreId       = pScb->SysCore;
    Os_IsrType    idStartRange = Os_IsrIdRange[coreId].Isr2.IsrStart;
    Os_IsrType    idEndRange   = Os_IsrIdRange[coreId].Isr2.IsrEnd;

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    Os_IsrType TProtIsrId = INVALID_ISR;

    /*  Timing Protection isr fault */
    if (whoHook == OS_TMPROT_HOOK_ISR)
    {
        if ((err == E_OS_PROTECTION_TIME) || (err == E_OS_PROTECTION_LOCKED))
        {
            TProtIsrId = pScb->SysIsrNestQueue[pScb->IntNestISR2 - OS_TMPROT_TARGET_ISR_OFFSET];
        }
    }
#endif

    for (uint16 i = (uint16)idStartRange; i < (uint16)idEndRange; i++)
    {
        uint8 isrState   = 0u;
        uint8 stackUsage = 0u;
        uint8 loadRatio  = 0u;
        uint8 serviceId  = 0u;
        uint8 errorCode  = 0u;

        if (i == (uint16)pScb->SysRunningIsrCat2Id)
        {
            isrState = 1u;
        }

#if (TRUE == CFG_STACK_USAGE_CHECK)
        const Os_StackType* stack = &Os_ISR2Stack[i];
        stackUsage                = Os_CalcStackUsageLevel(stack);
#endif

#if (TRUE == CFG_LOAD_RATIO_CALC_ENABLE)
        uint32 averageLoadRatio = pScb->LoadRatioMonitor.Os_IsrLoadRatio[i].averageLoadRatio;
        if (averageLoadRatio > FAULT_LOAD_RATIO_MAX)
        {
            loadRatio = 1u;
        }
#endif

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        if (i == (uint16)TProtIsrId)
        {
#if (TRUE == CFG_USEGETSERVICEID)
            serviceId = (uint8)pScb->SysServiceId;
#endif
            errorCode = err;
        }
#endif
        if ((err != E_OS_PROTECTION_TIME) && (err != E_OS_PROTECTION_LOCKED))
        {
            if ((OS_LEVEL_ISR2 == pScb->SysOsLevel) && (i == (uint16)pScb->SysRunningIsrCat2Id))
            {
#if (TRUE == CFG_USEGETSERVICEID)
                serviceId = (uint8)pScb->SysServiceId;
#endif
                errorCode = err;
            }
        }

        /* state | stackUsage | loadRatio | serviceId | errorCode
          4bit       2bit         2bit        8bit        8bit  */
        isrRuntimeInf.State         = isrState;
        isrRuntimeInf.StackUsage    = stackUsage;
        isrRuntimeInf.LoadRatio     = (uint8)loadRatio;
        isrRuntimeInf.ServiceId     = serviceId;
        isrRuntimeInf.ErrorCode     = errorCode;
        systemInf->IsrRuntimeInf[i] = isrRuntimeInf;
    }

    UNUSED_PARAMETER(whoHook);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Save system param with mode
 */
void Os_FaultRecordSysRuntimeInform(const Os_SCBType* pScb, StatusType err, uint32 whoHook, uint8 recordMode)
{
    if (NULL_PTR != pScb->FaultSySInformation)
    {
        Os_SystemInformation* systemInf = &pScb->FaultSySInformation->SystemInf;

        systemInf->SysOsLevel = pScb->SysOsLevel;
#if (CFG_OSAPPLICATION_MAX > 0)
        systemInf->SysRunningAppId = pScb->SysRunningAppId;
#endif
        systemInf->SysDispatchLocker = pScb->SysDispatchLocker;
        systemInf->SysInIsrCat2      = pScb->SysInIsrCat2;
        systemInf->IntNestISR2       = pScb->IntNestISR2;
        if ((pScb->SuspendAllCount > 0U) || (pScb->SuspendOsCount > 0U))
        {
            systemInf->IntDisableFlag = TRUE;
        }
        else
        {
            systemInf->IntDisableFlag = FALSE;
        }

        if (OS_LEVEL_TASK == pScb->SysOsLevel)
        {
            systemInf->SysRunningTaskId = pScb->SysRunningTaskId;
        }
        else if (OS_LEVEL_ISR2 == pScb->SysOsLevel) /* PRQA S 2004 */ /* VL_Os_2004 */
        {
            systemInf->SysRunningIsrCat2Id = pScb->SysRunningIsrCat2Id;
        }

        if ((FAULT_RECORD_MODE_LIGHT == recordMode) && (OS_TMPROT_HOOK_TASK != whoHook) && (OS_TMPROT_HOOK_ISR != whoHook))
        {
            Os_FaultRecordCurrentObject(pScb, systemInf, err, whoHook);
        }
        else
        {
            Os_FaultRecordTaskRuntimeInform(pScb, systemInf, err, whoHook);
            Os_FaultRecordIsr2RuntimeInform(pScb, systemInf, err, whoHook);
        }

        FaultManagerHook(pScb->FaultSySInformation);
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (TRUE == CFG_HARDWARE_FAULT_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Hardware fault save handler
 */
OS_LOCAL void Os_FaultSaveExceptionContext(const Os_SCBType* pScb, const Os_TrapContextType* TrapMsg)
{
    Os_TrapContextType* trapContext = &pScb->FaultSySInformation->TrapContext;
    /* Hardware exception has occurred */
    pScb->FaultSySInformation->IsHardFault = TRUE;

    trapContext->TrapRegs                       = TrapMsg->TrapRegs;
    trapContext->TrapMsg.ExceptionAddress       = TrapMsg->TrapMsg.ExceptionAddress;
    trapContext->TrapMsg.ExceptionCase          = TrapMsg->TrapMsg.ExceptionCase;
    trapContext->TrapMsg.ExceptionCaseMsg       = TrapMsg->TrapMsg.ExceptionCaseMsg;
    trapContext->TrapMsg.ExceptionSP            = TrapMsg->TrapMsg.ExceptionSP;
    trapContext->TrapMsg.ExceptionSysteamStatus = TrapMsg->TrapMsg.ExceptionSysteamStatus;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_HARDWARE_FAULT_ENABLE */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Save all abnormal information with record mode
 */
/* PRQA S 1532, 1503, 1505 ++ */ /* VL_QAC_OneFunRef, VL_QAC_NoUsedApi, VL_Os_1505 */
void Os_FaultSaveInformation(StatusType errType, uint8 recordMode)
/* PRQA S 1532, 1503, 1505 -- */
{
    const Os_SCBType* pScb = Os_GetCurrentContext();

#if (TRUE == CFG_HARDWARE_FAULT_ENABLE)
    Os_TrapContextType TrapContext;
    TrapContext.TrapRegs = Os_HalContextExceptionContext[pScb->SysCore];

    Os_Hal_ExceptionInformationParse(&TrapContext);
    Os_FaultSaveExceptionContext(pScb, &TrapContext);
#endif /* TRUE == CFG_HARDWARE_FAULT_ENABLE */

    Os_FaultRecordSysRuntimeInform(pScb, errType, OS_NO_CARE, recordMode);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (TRUE == CFG_HARDWARE_FAULT_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Getter function for the exception context.
 */
/* PRQA S 1503, 3006 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006 */
StatusType Os_GetExceptionContext(Os_ExceptionContextRefType context)
/* PRQA S 1503, 3006 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */                         /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */     /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    StatusType err = E_OK;
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    const Os_SCBType* pScb = Os_GetCurrentContext();
#endif
#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (NULL_PTR == context)
    {
        err = E_OS_ILLEGAL_ADDRESS;
    }
    else
#endif
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(pScb, OS_CONTEXT_GETEXCEPTIONCONTEXT) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    else
#endif
    {
        *context = Os_HalContextExceptionContext[Os_GetCoreIdLocal()];
    }

    OS_HAL_EXIT_KERNEL(); /* PRQA S 3138, 3141, 2743 */ /* VL_Os_PlatformNoDef */ /* VL_Os_2743*/

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Setter function for the exception context.
 */
/* PRQA S 1503, 3006 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006 */
StatusType Os_SetExceptionContext(const Os_ExceptionContextType* context)
/* PRQA S 1503, 3006 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */                         /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */     /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    StatusType err = E_OK;
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    const Os_SCBType* pScb = Os_GetCurrentContext();
#endif
#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (NULL_PTR == context)
    {
        err = E_OS_ILLEGAL_ADDRESS;
    }
    else
#endif
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(pScb, OS_CONTEXT_SETEXCEPTIONCONTEXT) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    else
#endif
    {
        Os_HalContextExceptionContext[Os_GetCoreIdLocal()] = *context;
    }

    OS_HAL_EXIT_KERNEL(); /* PRQA S 3138, 3141, 2743 */ /* VL_Os_PlatformNoDef */ /* VL_Os_2743*/

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_HARDWARE_FAULT_ENABLE */

#endif /*TRUE == CFG_FAULT_MANAGEMENT_ENABLE*/

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
void Os_ExceptionHandler(const void* errMsg, StatusType errType) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    Os_SCBType* pScb    = Os_GetCurrentContext(); /* PRQA S 3678 */ /* VL_Os_3678 */
    uint32      whoHook = OS_TMPROT_HOOK_OSAPP;

    if (OS_LEVEL_TASK == pScb->SysOsLevel)
    {
        whoHook = OS_TMPROT_HOOK_TASK;
    }
    else if (OS_LEVEL_ISR2 == pScb->SysOsLevel) /* PRQA S 2004 */ /* VL_Os_2004 */
    {
        whoHook = OS_TMPROT_HOOK_ISR;
    }
#endif /* (OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC) */ /* PRQA S 2053 */ /*VL_Os_2053*/

#if (TRUE == CFG_FAULT_MANAGEMENT_ENABLE)
#if (TRUE == CFG_HARDWARE_FAULT_ENABLE)
    if (NULL_PTR != errMsg)
    {
        Os_HalExceptionSaveContext(errMsg);
    }
#endif /* TRUE == CFG_HARDWARE_FAULT_ENABLE */
    Os_FaultSaveInformation(errType, FAULT_RECORD_MODE_FULL);
#endif

#if (TRUE == CFG_ERRORHOOK)
    (void)Os_CallErrorHook(errType);
#endif

#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_PROTECTIONHOOK)
    /* OS245:  protection exception hook. */
    (void)Os_CallProtectionHook(errType, whoHook); /* PRQA S 1520 */ /* VL_Os_1520 */
#endif
#endif /* (OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC)*/ /* PRQA S 2053 */ /*VL_Os_2053*/

#if ((TRUE == CFG_FAULT_MANAGEMENT_ENABLE) && (TRUE == CFG_HARDWARE_FAULT_ENABLE)) /* PRQA S 3332 */ /* VL_Os_3332*/
    if (NULL_PTR != errMsg)
    {
        Os_HalExceptionResumeContext(errMsg);
    }
#else
    while (1) /* PRQA S 2740, 2870 */ /* VL_Os_2740 */ /* VL_Os_2870 */
    {
        /* Nothing to do. */
    }
#endif
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    UNUSED_PARAMETER(whoHook); /* PRQA S 2880 */ /* VL_Os_2880 */
#endif
    UNUSED_PARAMETER(errType); /* PRQA S 2880 */ /* VL_Os_2880 */
    UNUSED_PARAMETER(errMsg); /* PRQA S 2880 */  /* VL_Os_2880 */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
/* PRQA S 0553 EOF */ /* VL_QAC_UnUsedFiles */
