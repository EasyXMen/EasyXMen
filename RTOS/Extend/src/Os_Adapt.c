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
 **  @file               : Os_Adapt.c
 **  @author             : i-soft-os
 **  @date               : 2025/04/02
 **  @vendor             : isoft
 **  @description        : Os source file for Adapt API implementations
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Os_Interrupt.h"
#include "Os_Extend.h"
#include "Os_Adapt.h"
#include "Os_Arch_Processor.h"
#include "Os_Tprot.h"
#include "Os_Sprot.h"
#include "Os_Kernel.h"

/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[I N T E R N A L   D A T A]==========================================*/
#if (FALSE == CFG_TIMING_PROTECTION_ENABLE)
#define OS_START_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "Os_MemMap.h"
OS_LOCAL Os_ArchMsrType Os_GlobalKMInt[CFG_CORE_MAX];
#define OS_STOP_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "Os_MemMap.h"
#endif

/*=======[E X T E R N A L   D A T A]==========================================*/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Returns error information of the last error occurred on the local core
 */
/* PRQA S 1503,3673 ++ */ /* VL_QAC_NoUsedApi,VL_QAC_3673 */
StatusType Os_GetDetailedError(Os_ErrorInformationRefType ErrorRef)
/* PRQA S 1503,3673 -- */
{
    StatusType err = E_OS_SERVICEID;
#if (TRUE == CFG_USEGETSERVICEID)
    const Os_SCBType *pScb = Os_GetCurrentContext();

    if(NULL_PTR == ErrorRef)
    {
        err = E_OS_ILLEGAL_ADDRESS; /* PRQA S 2982 */ /* VL_Os_2982 */
    }
    /*service protection*/
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    else if (Os_WrongContext(pScb, OS_CONTEXT_GETDETAILEDERROR) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    else
#endif
    {
        ErrorRef->DetailedError = pScb->SysOsError; /* PRQA S 2812 */ /* VL_Os_2812 */
        ErrorRef->Error = pScb->SysOsError;
        ErrorRef->Service = pScb->SysServiceId;
        err = E_OK;
        
    }
#endif

    UNUSED_PARAMETER(ErrorRef);
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (CFG_STACK_USAGE_CHECK == TRUE) && (CFG_STACK_CHECK == TRUE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Get the current stack usage of the given task
 */
/* PRQA S 1503,3006 ++ */ /* VL_QAC_NoUsedApi,VL_Os_3006 */
uint32 Os_GetTaskStackUsage(TaskType TaskID)
/* PRQA S 1503,3006 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006, 1021 ++ */ /* VL_Os_1006, VL_Os_1021 */
    OS_HAL_ENTER_KERNEL();
    /* PRQA S 1006, 1021 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    uint32 MaxUsage = 0U;
    MaxUsage = OSGetStackUsage(OS_STACK_TASK, (uint16)TaskID);

    /* PRQA S 3138,3141 ++ */ /* VL_Os_PlatformNoDef */
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3138,3141 -- */

    return MaxUsage;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Disables the recognition status of all interrupts. This service does not 
 * perform nesting nor service protection checks.
 */
/* PRQA S 1503,3006 ++ */ /* VL_QAC_NoUsedApi,VL_Os_3006 */
void Os_DisableGlobalKM(void)
/* PRQA S 1503,3006 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006, 1021 ++ */ /* VL_Os_1006, VL_Os_1021 */
    OS_HAL_ENTER_KERNEL();
    /* PRQA S 1006, 1021 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    const Os_SCBType *pScb = Os_GetCurrentContext();

    /* Timing protection: resource lock. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#if (CFG_ISR2_MAX > 0)
    if (TRUE == pScb->SysInIsrCat2)
    {
        Os_TmProtIsrStart(pScb->SysCore, pScb->SysRunningIsrCat2Id, TP_SUS_ALL_INT);
    }
    else
#endif
    {
        if (OS_LEVEL_TASK == pScb->SysOsLevel)
        {
            Os_TmProtTaskStart(pScb->SysCore, pScb->SysRunningTaskId, TP_SUS_ALL_INT);
        }
    }

    Os_Hal_DisableAllInt_ButTimingProtInt(pScb->SysCore);
#else
    Os_GlobalKMInt[pScb->SysCore] = Os_Hal_SuspendInt();
#endif

    /* PRQA S 3138,3141 ++ */ /* VL_Os_PlatformNoDef */
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3138,3141 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Enables the recognition status of all interrupts. This service does not 
 * perform nesting nor service protection checks.
 */
/* PRQA S 1503,3006 ++ */ /* VL_QAC_NoUsedApi,VL_Os_3006 */
void Os_EnableGlobalKM(void)
/* PRQA S 1503,3006 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006, 1021 ++ */ /* VL_Os_1006, VL_Os_1021 */
    OS_HAL_ENTER_KERNEL();
    /* PRQA S 1006, 1021 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    const Os_SCBType *pScb = Os_GetCurrentContext();
    
    /* AutoSar SC2: Timing protection, resource lock. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#if (CFG_ISR2_MAX > 0)
    if (TRUE == pScb->SysInIsrCat2)
    {
    	Os_TmProtIsrEnd(pScb->SysCore, pScb->SysRunningIsrCat2Id, TP_SUS_ALL_INT);
    }
    else
#endif
    {
        if (OS_LEVEL_TASK == pScb->SysOsLevel)
        {
        	Os_TmProtTaskEnd(pScb->SysCore, pScb->SysRunningTaskId, TP_SUS_ALL_INT);
        }
    }
    Os_Hal_EnableAllInt_ButTimingProtInt(pScb->SysCore);
#else
    Os_Hal_RestoreInt(Os_GlobalKMInt[pScb->SysCore]);
#endif

    /* PRQA S 3138,3141 ++ */ /* VL_Os_PlatformNoDef */
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3138,3141 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Disables the recognition status of category 2 interrupts. This service does 
 * not perform nesting nor service protection checks.
 */
/* PRQA S 6070,1503,3006 ++ */ /* VL_MTR_Os_STCAL,VL_QAC_NoUsedApi,VL_Os_3006 */
void Os_DisableLevelKM(void)
/* PRQA S 6070,1503,3006 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006, 1021 ++ */ /* VL_Os_1006, VL_Os_1021 */
    OS_HAL_ENTER_KERNEL();
    /* PRQA S 1006, 1021 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    OS_HAL_DECLARE_CRITICAL();
    Os_SCBType *pScb = Os_GetCurrentContext();

    /* AutoSar SC2: Timing protection, resource lock. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#if (CFG_ISR2_MAX > 0)
    if (TRUE == pScb->SysInIsrCat2)
    {
    	Os_TmProtIsrStart(pScb->SysCore, pScb->SysRunningIsrCat2Id, TP_SUS_OS_INT);
    }
    else
#endif
    {
        if (OS_LEVEL_TASK == pScb->SysOsLevel)
        {
        	Os_TmProtTaskStart(pScb->SysCore, pScb->SysRunningTaskId, TP_SUS_OS_INT);
        }
    }
#endif

    OS_HAL_ENTRY_CRITICAL();
    pScb->SaveOsIntNested = Os_Hal_GetIpl();
    Os_Hal_SetIpl(Os_CfgIsr2ExcludeTplMax_Inf[pScb->SysCore], OS_ISR_DISABLE);
    OS_HAL_EXIT_CRITICAL();

    /* PRQA S 3138,3141 ++ */ /* VL_Os_PlatformNoDef */
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3138,3141 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Enables the recognition status of category 2 interrupts. This service does not 
 * perform nesting nor service protection checks.
 */
/* PRQA S 1503,3006 ++ */ /* VL_QAC_NoUsedApi,VL_Os_3006 */
void Os_EnableLevelKM(void)
/* PRQA S 1503,3006 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006, 1021 ++ */ /* VL_Os_1006, VL_Os_1021 */
    OS_HAL_ENTER_KERNEL();
    /* PRQA S 1006, 1021 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    const Os_SCBType *pScb = Os_GetCurrentContext();

    /* AutoSar SC2: Timing protection, resource lock. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#if (CFG_ISR2_MAX > 0)
    if (TRUE == pScb->SysInIsrCat2)
    {
    	Os_TmProtIsrEnd(pScb->SysCore, pScb->SysRunningIsrCat2Id, TP_SUS_OS_INT);
    }
    else
#endif
    {
        if (OS_LEVEL_TASK == pScb->SysOsLevel)
        {
        	Os_TmProtTaskEnd(pScb->SysCore, pScb->SysRunningTaskId, TP_SUS_OS_INT);
        }
    }
#endif

    Os_Hal_SetIpl(pScb->SaveOsIntNested, OS_ISR_ENABLE);

    /* PRQA S 3138,3141 ++ */ /* VL_Os_PlatformNoDef */
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3138,3141 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Enable the interrupt sources of category 2 ISR on the current core.
 */
/* PRQA S 6070, 6030, 1503, 3006 ++ */ /* VL_MTR_Os_STCAL, VL_MTR_Os_STMIF, VL_QAC_NoUsedApi, VL_Os_3006 */
StatusType Os_InitialEnableInterruptSources(boolean ClearPending)
/* PRQA S 6070, 6030, 1503, 3006 -- */
{
#if (TRUE == Os_HAL_INT_IS_CLEARPENDING_SUPPORTED())
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006, 1021 ++ */ /* VL_Os_1006, VL_Os_1021 */
    OS_HAL_ENTER_KERNEL();
    /* PRQA S 1006, 1021 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    StatusType err = E_OK;
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    const Os_SCBType *pScb = Os_GetCurrentContext();
#endif

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    if (Os_WrongContext(pScb, OS_CONTEXT_INITIAL_ENABLE_INTERRUPT_SOURCE) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    else
#endif
    {
        uint32 i;
        Os_CoreIdType sysCore = Os_GetCoreIdLocal();
        Os_IsrType idStartRange = Os_IsrIdRange[sysCore].AllIsr.IsrStart;
        Os_IsrType idEndRange = Os_IsrIdRange[sysCore].AllIsr.IsrEnd;
        for (i = (uint32)idStartRange; i < (uint32)idEndRange; i++)
        {
            uint32  isrSrc = Os_IsrCfg[i].IsrSrc;
            uint32  isrSrcType = Os_IsrCfg[i].IsrSrcType;
            if (TRUE == ClearPending)
            {
                Os_Hal_ClearPendingInterrupt(isrSrc, isrSrcType);
            }
            Os_Hal_EnableInterruptSource(isrSrc, isrSrcType); /* PRQA S 0303, 3345, 3442*/ /* VL_Os_0303*//* VL_Os_3345*//* VL_Os_3442*/
        }
    }

    /* PRQA S 3138,3141 ++ */ /* VL_Os_PlatformNoDef */
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3138,3141 -- */
#else
    StatusType err = E_OS_NOFUNC;
    UNUSED_PARAMETER(ClearPending);
#endif

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Check if the given interrupt source is enabled.
 */
/* PRQA S 6070,6030,1503,3006 ++ */ /* VL_MTR_Os_STCAL,VL_MTR_Os_STMIF,VL_QAC_NoUsedApi,VL_Os_3006 */
StatusType Os_IsInterruptSourceEnabled(ISRType ISRID, boolean* IsEnabled)
/* PRQA S 6070,6030,1503,3006 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006, 1021 ++ */ /* VL_Os_1006, VL_Os_1021 */
    OS_HAL_ENTER_KERNEL();
    /* PRQA S 1006, 1021 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    StatusType err = E_OK;
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    const Os_SCBType *pScb = Os_GetCurrentContext();
#endif

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (Os_ObjectIDCheck((ObjectType)ISRID, (uint8)OS_OBJECT_ISR) != TRUE)
    {
        err = E_OS_ID;
    }
    else if (OS_ISR_CATEGORY2 != Os_IsrCfg[ISRID].IsrCatType)
    {
        err = E_OS_ID;
    }
    else if (NULL_PTR == IsEnabled)
    {
        err = E_OS_ILLEGAL_ADDRESS;
    }
    else
#endif
    {
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(pScb, OS_CONTEXT_IS_INTERRUPT_SOURCE_ENABLED) != TRUE)
        {
            err = E_OS_CALLEVEL;
        }        
        else if (Os_CheckObjAcs(pScb, OS_OBJECT_ISR, (uint8)ISRID) != TRUE)
        {
            err = E_OS_ACCESS;
        }
        else
#endif
        {
            uint32 isrSrc = Os_IsrCfg[ISRID].IsrSrc;
            uint32 isrSrcType = Os_IsrCfg[ISRID].IsrSrcType;
            if (OS_ISR_DISABLED == Os_Hal_GetIsrSourceState(isrSrc, isrSrcType))
            {
                (*IsEnabled) = FALSE;
            }
            else
            {
                (*IsEnabled) = TRUE;
            }
        }
    }

    /* PRQA S 3138,3141 ++ */ /* VL_Os_PlatformNoDef */
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3138,3141 -- */

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Set an unhandled interrupt request
 */
/* PRQA S 1532,3006 ++ */ /* VL_QAC_OneFunRef,VL_Os_3006 */
void Os_UnhandledIrq(void)
/* PRQA S 1532,3006 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006, 1021 ++ */ /* VL_Os_1006, VL_Os_1021 */
    OS_HAL_ENTER_KERNEL();
    /* PRQA S 1006, 1021 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    Os_SCBType * pScb = Os_GetCurrentContext();

    pScb->LastUnhandledIrq = Os_Hal_GetCurrentIrq();
    pScb->UnhandledIrqReported = TRUE;

    /* PRQA S 3138,3141 ++ */ /* VL_Os_PlatformNoDef */
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3138,3141 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * In case of an unhandled interrupt request the triggering interrupt source 
 * can be distinguished with this service.
 */
/* PRQA S 1503,3006 ++ */ /* VL_QAC_NoUsedApi,VL_Os_3006 */
StatusType Os_GetUnhandledIrq (Os_InterruptSourceIdRefType InterruptSource)
/* PRQA S 1503,3006 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006, 1021 ++ */ /* VL_Os_1006, VL_Os_1021 */
    OS_HAL_ENTER_KERNEL();
    /* PRQA S 1006, 1021 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    StatusType err = E_OK;
    const Os_SCBType *pScb = Os_GetCurrentContext();
#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (NULL_PTR == InterruptSource)
    {
        err = E_OS_ILLEGAL_ADDRESS;
    }
    else
#endif
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    if (Os_WrongContext(pScb, OS_CONTEXT_GETUNHANDLEDIRQ) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    else
#endif
    if (FALSE == pScb->UnhandledIrqReported)
    {
        err = E_OS_STATE;
    }
    else
    {
        *InterruptSource = (Os_InterruptSourceIdType)pScb->LastUnhandledIrq;
    }

    /* PRQA S 3138,3141 ++ */ /* VL_Os_PlatformNoDef */
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3138,3141 -- */

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Set an unhandled exception request.
 */
/* PRQA S 1503,3006 ++ */ /* VL_QAC_NoUsedApi,VL_Os_3006 */
void Os_UnhandledExc(void)
/* PRQA S 1503,3006 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006, 1021 ++ */ /* VL_Os_1006, VL_Os_1021 */
    OS_HAL_ENTER_KERNEL();
    /* PRQA S 1006, 1021 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    Os_SCBType *pScb = Os_GetCurrentContext();

    pScb->LastUnhandledExc = Os_Hal_GetCurrentExc();
    pScb->UnhandledExcReported = TRUE;

    /* PRQA S 3138,3141 ++ */ /* VL_Os_PlatformNoDef */
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3138,3141 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * In case of an unhandled exception request the triggering exception source 
 * can be distinguished with this service.
 */
/* PRQA S 1503,3006 ++ */ /* VL_QAC_NoUsedApi,VL_Os_3006 */
StatusType Os_GetUnhandledExc(Os_ExceptionSourceIdRefType ExceptionSource)
/* PRQA S 1503,3006 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006, 1021 ++ */ /* VL_Os_1006, VL_Os_1021 */
    OS_HAL_ENTER_KERNEL();
    /* PRQA S 1006, 1021 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    StatusType err = E_OK;
    const Os_SCBType *pScb = Os_GetCurrentContext();
#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (NULL_PTR == ExceptionSource)
    {
        err = E_OS_ILLEGAL_ADDRESS;
    }
    else
#endif
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    if (Os_WrongContext(pScb, OS_CONTEXT_GETUNHANDLEDEXE) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    else
#endif
    if (FALSE == pScb->UnhandledExcReported)
    {
        err = E_OS_STATE;
    }
    else
    {
        *ExceptionSource = (Os_ExceptionSourceIdType)pScb->LastUnhandledExc;
    }

    /* PRQA S 3138,3141 ++ */ /* VL_Os_PlatformNoDef */
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3138,3141 -- */

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"




