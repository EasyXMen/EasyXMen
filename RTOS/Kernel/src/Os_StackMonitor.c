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
 **  @file               : Os_StackMonitor.c
 **  @author             : i-soft-os
 **  @date               : 2025/02/10
 **  @vendor             : isoft
 **  @description        : Os source file for StackMonitor API implementations
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Os_Hal_Define.h"
#include "Os_StackMonitor.h"
#include "Os_Kernel.h"
#include "Os_Hook.h"
#include "Os_Arti.h"
#include "Os_Err.h"

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ============================================ external data definitions =========================================== */

/* ============================================ internal data definitions =========================================== */

/* ========================================== internal function declarations ======================================== */

/* ========================================== external function definitions ========================================= */
#if (TRUE == CFG_STACK_CHECK)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           Check the stack usage of the task
 * @param[in]       StackPtr: Pointer to the stack pointer to be detected
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL boolean Os_StackMonitorCheck(const Os_StackPtrType StackPtr, Os_StackDataType GuardSize);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (CFG_STACK_USAGE_CHECK == FALSE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Internal Implementation of Os_SillStack Interface
 */
/* PRQA S 3450 ++ */ /* VL_Os_3450 */
static void Os_FillStackByte(Os_StackPtrType StackPtr, Os_StackDataType GuardSize)
/* PRQA S 3450 -- */
{
    Os_StackDataType CheckLoop = 0U;
    /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral */
    while (CheckLoop < GuardSize)
    /* PRQA S 1258 -- */
    {
        /* PRQA S 0488 ++ */ /* VL_Os_0488 */
        *(StackPtr + CheckLoop) = OS_STACK_FILL_PATTERN;
        /* PRQA S 0488 -- */
        CheckLoop++;
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Check the stack usage of the task
 */
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
OS_LOCAL boolean Os_StackMonitorCheck(const Os_StackPtrType StackPtr, Os_StackDataType GuardSize)
/* PRQA S 3673 -- */
{
    boolean          result = TRUE;
    Os_StackDataType size   = GuardSize;
    while (size > 0U)
    {
        size--;
        /* PRQA S 0488 ++ */ /* VL_Os_0488 */
        if (*(StackPtr + size) != OS_STACK_FILL_PATTERN)
        /* PRQA S 0488 -- */
        {
            result = FALSE;
            break;
        }
    }
    return result;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Check whether the stack is overflowed.
 */
/* PRQA S 3673, 6070 ++ */ /* VL_QAC_3673, VL_MTR_Os_STCAL */
void Os_StackMonitor(const Os_StackType* stack)
/* PRQA S 3673, 6070 -- */
{
    /* check system stack overflow */
    /* PRQA S 1259, 0488, 1258 ++ */ /* VL_Os_1259, VL_Os_0488, VL_Os_1258 */
    if ((Os_StackMonitorCheck(stack->StackBottom, stack->GuardSize) == FALSE)
#if (TRUE == OS_STACK_MONITOR_BIDIRECTIONAL_CHECK)
        || (Os_StackMonitorCheck(stack->StackTop, stack->GuardSize) == FALSE)
#endif
    )
    /* PRQA S 1259, 0488, 1258 -- */
    {
        /* PRQA S 3138, 3141, 3455, 3678 ++ */ /* VL_Os_3138, VL_Os_3141, VL_Os_3455, VL_Os_3678 */
        Os_SCBType* pScb = Os_GetCurrentContext();
        Os_ErrorHook(E_OS_STACKFAULT, pScb);
        /* PRQA S 3138, 3141, 3455, 3678 -- */
#if (TRUE == CFG_PROTECTIONHOOK)
        Os_CallLevelType osSameLevel;
        OS_HAL_DECLARE_CRITICAL();

        OS_HAL_ENTRY_CRITICAL();
        osSameLevel      = pScb->SysOsLevel;
        pScb->SysOsLevel = OS_LEVEL_PROTECTIONHOOK;
        /* PRQA S 3138, 3141 ++ */       /* VL_Os_PlatformNoDef */
        /* PRQA S 1841, 4544, 4542 ++ */ /* VL_Os_1841, VL_Os_4544, VL_Os_4542 */
        /* PRQA S 4543, 1277, 1258 ++ */ /* VL_Os_4543, VL_Os_1277, VL_Os_1258 */
        ARTI_TRACE(NOSUSP, AR_CP_OS_HOOK, Os, pScb->SysCore, OsHook_ProtectionHook_Start, E_OS_STACKFAULT);
        /* PRQA S 4543, 1277, 1258 -- */
        /* PRQA S 1841, 4544, 4542 -- */
        /* PRQA S 3138, 3141 -- */
        (void)ProtectionHook(E_OS_STACKFAULT);
        /* PRQA S 3138, 3141 ++ */       /* VL_Os_PlatformNoDef */
        /* PRQA S 1841, 4544, 4542 ++ */ /* VL_Os_1841, VL_Os_4544, VL_Os_4542 */
        /* PRQA S 4543, 1277, 1258 ++ */ /* VL_Os_4543, VL_Os_1277, VL_Os_1258 */
        ARTI_TRACE(NOSUSP, AR_CP_OS_HOOK, Os, pScb->SysCore, OsHook_ProtectionHook_Return, E_OS_STACKFAULT);
        /* PRQA S 4543, 1277, 1258 -- */
        /* PRQA S 1841, 4544, 4542 -- */
        /* PRQA S 3138, 3141 -- */
        pScb->SysOsLevel = osSameLevel;
        OS_HAL_EXIT_CRITICAL();
#endif

        UNUSED_PARAMETER(pScb);
        Os_ShutdownOS(E_OS_STACKFAULT, OS_SHUTDOWN_OS);
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * fill the stack of task or system with the value of 0xCC
 */
void Os_FillStack(const Os_StackType* stack)
{
#if (CFG_STACK_USAGE_CHECK == TRUE)
    Os_StackPtrType ptr;
    /* PRQA S 1258, 0488, 3679 ++ */ /* VL_Os_ConstToIntegral, VL_Os_0488, VL_Os_3679 */
    Os_StackPtrType endPtr = (Os_StackDataType*)(stack->StackTop + (Os_StackDataType)stack->GuardSize);
    /* PRQA S 1258, 0488, 3679 -- */
    /* PRQA S 0490 ++ */ /* VL_Os_0490 */
    for (ptr = (Os_StackDataType*)stack->StackBottom; ptr < endPtr; ptr++)
    /* PRQA S 0490 -- */
    {
        *ptr = OS_STACK_FILL_PATTERN;
    }
#else

    Os_FillStackByte(stack->StackBottom, stack->GuardSize); /* fill stack to ensure safety*/
#if (TRUE == OS_STACK_MONITOR_BIDIRECTIONAL_CHECK)
    Os_FillStackByte(stack->StackTop, stack->GuardSize); /* fill stack to ensure safety*/
#endif
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#if (TRUE == CFG_STACK_USAGE_CHECK)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Calculate stack usage in words
 */
uint32 Os_CalcStackUsage(const Os_StackType* stack) /* PRQA S 1505 */ /* VL_Os_1505 */
{
    const uint32* ptr = NULL_PTR;
    /* PRQA S 0490 ++ */ /* VL_Os_0490 */
    for (ptr = stack->StackBottom; ptr < stack->StackTop; ptr++)
    /* PRQA S 0490 -- */
    {
        if (OS_STACK_FILL_PATTERN != (*ptr))
        {
            break;
        }
    }
    /* PRQA S 0488 ++ */ /* VL_Os_0488 */
    return (uint32)(stack->StackTop - ptr);
    /* PRQA S 0488 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Calculate stack usage level (0-3)
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
uint8 Os_CalcStackUsageLevel(const Os_StackType* stack)
/* PRQA S 1532 -- */
{
    uint8 stackUsage = OS_STACK_USAGE_LEVEL_LOW;
    /* PRQA S 0488,3120,2834 ++ */ /* VL_Os_0488,VL_QAC_MagicNum,VL_Os_2834 */
    uint32 usedWords  = Os_CalcStackUsage(stack);
    uint32 totalWords = (uint32)(stack->StackTop - stack->StackBottom);
    uint32 percent    = (usedWords * OS_STACK_USAGE_PERCENT_100) / (totalWords);
    /* PRQA S 0488,3120,2834 -- */

    if ((0u < percent) && (OS_STACK_USAGE_PERCENT_25 >= percent))
    {
        stackUsage = OS_STACK_USAGE_LEVEL_LOW;
    }
    else if ((OS_STACK_USAGE_PERCENT_25 < percent) && (OS_STACK_USAGE_PERCENT_50 >= percent))
    {
        stackUsage = OS_STACK_USAGE_LEVEL_MEDIUM_LOW;
    }
    else if ((OS_STACK_USAGE_PERCENT_50 < percent) && (OS_STACK_USAGE_PERCENT_75 >= percent))
    {
        stackUsage = OS_STACK_USAGE_LEVEL_MEDIUM_HIGH;
    }
    else if ((OS_STACK_USAGE_PERCENT_75 < percent) && (OS_STACK_USAGE_PERCENT_100 >= percent))
    {
        stackUsage = OS_STACK_USAGE_LEVEL_HIGH;
    }
    else
    {
        /* Nothing */
    }

    return stackUsage;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

/*=======[E N D   O F   F I L E]==============================================*/
