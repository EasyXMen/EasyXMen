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
 *
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
#include "Os_Arch_Processor.h"
#include "Os_FaultManager.h"
#include "Os_Kernel.h"
#include "Os_Task.h"
#include "Os_Interrupt.h"
#include "Os_Extend.h"
#include "Os_Sprot.h"
#include "Os_ProtectHook.h"
#include "Os_Hook.h"

/* ===================================================== macros ===================================================== */
#if (TRUE == CFG_FAULT_MANAGEMENT_ENABLE)
/* ================================================ type definitions ================================================ */

/* ============================================ external data definitions =========================================== */
#define OS_START_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "Os_MemMap.h"
/* PRQA S 3408, 1504 ++ */ /* VL_Os_3408, VL_QAC_OneRefSymbol */
Os_ExceptionContextType Os_HalContextExceptionContext[OS_AUTOSAR_CORES];
/* PRQA S 3408, 1504 -- */
#define OS_STOP_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "Os_MemMap.h"

/* ========================================== internal function declarations ======================================== */
/**
 * @brief           Saves hardware exception context information
 * @param[in]       pScb: Pointer to system control block
 * @param[in]       TrapMsg: Pointer to trap context information
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_FaultSaveExceptionContext(const Os_SCBType *pScb, const Os_TrapContextType *TrapMsg);

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
    Os_SCBType *pScb = Os_GetSystemContext(coreId);

    pScb->FaultSySInformation = Os_FaultSySInformation_Inf[coreId];
    Os_TrapMsgType *trapMsg = &pScb->FaultSySInformation->TrapContext.TrapMsg;
    Os_SystemInformation *systemInf = &pScb->FaultSySInformation->SystemInf;

    pScb->FaultSySInformation->IsHardFault = FALSE;

    trapMsg->ExceptionAddress = 0U;
    trapMsg->ExceptionCase = 0U;
    trapMsg->ExceptionCaseMsg = NULL_PTR;
    trapMsg->ExceptionSP = 0U;
    trapMsg->ExceptionSysteamStatus = 0U;

    systemInf->CoreId = 0U;
    systemInf->TaskID = INVALID_TASK; /* PRQA S 1461, 1297*/ /* VL_Os_1461*//* VL_Os_1297*/
    systemInf->ISRID = INVALID_ISR; /* PRQA S 1297*/ /* VL_Os_1297*/
    systemInf->SysStackMaxUsage = 0U;
    systemInf->TaskISR2MaxUsage = 0U;
    systemInf->ServiceID = OSServiceId_ShutdownHook;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Save system param
 */
void Os_FalultRecordSysRuntimeInform(const Os_SCBType *pScb, StatusType err)
{
    if (NULL_PTR != pScb->FaultSySInformation)
    {
        Os_SystemInformation *systemInf = &pScb->FaultSySInformation->SystemInf;

        systemInf->CoreId = pScb->SysCore;
#if (CFG_STACK_USAGE_CHECK == TRUE) && (CFG_STACK_CHECK == TRUE)
        systemInf->SysStackMaxUsage = OSGetStackUsage(OS_STACK_SYSTEM, 0u);
#endif
        systemInf->Error = err;

        if (OS_LEVEL_TASK == pScb->SysOsLevel)
        {
            systemInf->TaskID = pScb->SysRunningTaskId;
#if (CFG_STACK_USAGE_CHECK == TRUE) && (CFG_STACK_CHECK == TRUE)
            systemInf->TaskISR2MaxUsage = OSGetStackUsage(OS_STACK_TASK, (uint16)pScb->SysRunningTaskId);
#endif
        }
        else if (OS_LEVEL_ISR2 == pScb->SysOsLevel) /* PRQA S 2004 */ /* VL_Os_2004 */
        {
            systemInf->ISRID = pScb->SysRunningIsrCat2Id;
#if (CFG_STACK_USAGE_CHECK == TRUE) && (CFG_STACK_CHECK == TRUE)
            systemInf->TaskISR2MaxUsage = OSGetStackUsage(OS_STACK_ISR2, (uint16)pScb->SysRunningIsrCat2Id);
#endif
        }
#if (TRUE == CFG_USEGETSERVICEID)
        systemInf->ServiceID = pScb->SysServiceId;
#endif

        /* Exception information output */
        FaultManagerHook(pScb->FaultSySInformation);
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Hardware fault save handler
 */
OS_LOCAL void Os_FaultSaveExceptionContext(const Os_SCBType *pScb, const Os_TrapContextType * TrapMsg)
{
    Os_TrapContextType *trapContext = &pScb->FaultSySInformation->TrapContext;
    /* Hardware exception has occurred */
    pScb->FaultSySInformation->IsHardFault = TRUE;

    trapContext->TrapRegs = TrapMsg->TrapRegs;
    trapContext->TrapMsg.ExceptionAddress = TrapMsg->TrapMsg.ExceptionAddress;
    trapContext->TrapMsg.ExceptionCase = TrapMsg->TrapMsg.ExceptionCase;
    trapContext->TrapMsg.ExceptionCaseMsg = TrapMsg->TrapMsg.ExceptionCaseMsg;
    trapContext->TrapMsg.ExceptionSP = TrapMsg->TrapMsg.ExceptionSP;
    trapContext->TrapMsg.ExceptionSysteamStatus = TrapMsg->TrapMsg.ExceptionSysteamStatus;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Save all abnormal information
 */
/* PRQA S 1532, 1503, 1505 ++ */ /* VL_QAC_OneFunRef, VL_QAC_NoUsedApi, VL_Os_1505 */
void Os_FaultSaveInformation(StatusType errType)
/* PRQA S 1532, 1503, 1505 -- */
{
    Os_SCBType const *pScb = Os_GetCurrentContext();
    Os_TrapContextType TrapContext;

    TrapContext.TrapRegs = Os_HalContextExceptionContext[pScb->SysCore];

    Os_Hal_ExceptionInformationParse(&TrapContext);

    Os_FaultSaveExceptionContext(pScb, &TrapContext);

    Os_FalultRecordSysRuntimeInform(pScb, errType);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

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
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    StatusType err = E_OK;
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    const Os_SCBType *pScb = Os_GetCurrentContext();
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

    OS_HAL_EXIT_KERNEL();/* PRQA S 3138, 3141, 2743 */ /* VL_Os_PlatformNoDef *//* VL_Os_2743*/

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
StatusType Os_SetExceptionContext(const Os_ExceptionContextType * context)
/* PRQA S 1503, 3006 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    StatusType err = E_OK;
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    const Os_SCBType *pScb = Os_GetCurrentContext();
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

    OS_HAL_EXIT_KERNEL(); /* PRQA S 3138, 3141, 2743 */ /* VL_Os_PlatformNoDef *//* VL_Os_2743*/

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif /*TRUE == CFG_FAULT_MANAGEMENT_ENABLE*/

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
void Os_ExceptionHandler(const void *errMsg, StatusType errType) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    Os_SCBType *pScb = Os_GetCurrentContext();/* PRQA S 3678 */ /* VL_Os_3678 */
    uint32 whoHook = OS_TMPROT_HOOK_OSAPP;

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
    Os_HalExceptionSaveContext(errMsg);
    Os_FaultSaveInformation(errType);
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

#if (TRUE == CFG_FAULT_MANAGEMENT_ENABLE) /* PRQA S 3332 */ /* VL_Os_3332*/
    Os_HalExceptionResumeContext(errMsg);
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
    UNUSED_PARAMETER(errMsg); /* PRQA S 2880 */ /* VL_Os_2880 */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
/* PRQA S 0553 EOF */ /* VL_QAC_UnUsedFiles */
