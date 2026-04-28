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
#include "Os_Arch_Processor.h"
#include "Os_StackMonitor.h"
#include "Os_Kernel.h"
#include "Os_Hook.h"
#include "Os_Arti.h"
#include "Os_Hook.h"


/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ============================================ external data definitions =========================================== */

/* ============================================ internal data definitions =========================================== */

/* ========================================== internal function declarations ======================================== */

/* ========================================== external function definitions ========================================= */
#if (TRUE == CFG_STACK_CHECK)
/**
 * @brief           Check the stack usage of the task
 * @param[in]       StackPtr: Pointer to the stack pointer to be detected
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL boolean Os_StackMonitorCheck(const Os_StackPtrType StackPtr);

#if (CFG_STACK_USAGE_CHECK == FALSE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Internal Implementation of Os_SillStack Interface
 */
/* PRQA S 3450 ++ */ /* VL_Os_3450 */
static void Os_FillStackByte(Os_StackPtrType StackPtr)
/* PRQA S 3450 -- */
{
    Os_StackDataType CheckLoop = 0U;
    /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral */
    while (CheckLoop < (Os_StackDataType)CFG_STACK_CHECK_BYTE)
    /* PRQA S 1258 -- */
    {
        /* PRQA S 0488 ++ */  /* VL_Os_0488 */
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
OS_LOCAL boolean Os_StackMonitorCheck(const Os_StackPtrType StackPtr)
/* PRQA S 3673 -- */
{
    boolean result = TRUE;
    /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral */
    Os_StackDataType CheckLoop = (Os_StackDataType)CFG_STACK_CHECK_BYTE;   
    /* PRQA S 1258 -- */                                                             
    while (CheckLoop > 0U)
    {
        CheckLoop--;
        /* PRQA S 0488 ++ */  /* VL_Os_0488 */
        if (*(StackPtr + CheckLoop) != OS_STACK_FILL_PATTERN)
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
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
void Os_StackMonitor(const Os_StackType *stack)
/* PRQA S 3673 -- */
{
    /* check system stack overflow */
    /* PRQA S 1259, 0488, 1258 ++ */ /* VL_Os_1259, VL_Os_0488, VL_Os_1258 */
    if ((Os_StackMonitorCheck(stack->StackBottom) == FALSE)
#if (TRUE == OS_STACK_MONITOR_BIDIRECTIONAL_CHECK)
    ||(Os_StackMonitorCheck(stack->StackTop) == FALSE)
#endif
    )
    /* PRQA S 1259, 0488, 1258 -- */
    {
        /* PRQA S 3138, 3141, 3455 ++ */ /* VL_Os_3138, VL_Os_3141, VL_Os_3455 */
        Os_ErrorHook(E_OS_STACKFAULT);
        /* PRQA S 3138, 3141, 3455 -- */
#if (TRUE == CFG_PROTECTIONHOOK)
        Os_SCBType *pScb = Os_GetCurrentContext();
        Os_CallLevelType osSameLevel;
        OS_HAL_DECLARE_CRITICAL();

        OS_HAL_ENTRY_CRITICAL();
        osSameLevel = pScb->SysOsLevel;
        pScb->SysOsLevel = OS_LEVEL_PROTECTIONHOOK;
        /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
        /* PRQA S 1841, 4544, 4542, 4543, 1277, 1258 ++ */ /* VL_Os_1841, VL_Os_4544, VL_Os_4542, VL_Os_4543, VL_Os_1277, VL_Os_1258 */
        ARTI_TRACE(NOSUSP, AR_CP_OS_HOOK, Os, pScb->SysCore, OsHook_ProtectionHook_Start, E_OS_STACKFAULT);
        /* PRQA S 1841, 4544, 4542, 4543, 1277, 1258 -- */
        /* PRQA S 3138, 3141 -- */
        (void)ProtectionHook(E_OS_STACKFAULT);
        /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
        /* PRQA S 1841, 4544, 4542, 4543, 1277, 1258 ++ */ /* VL_Os_1841, VL_Os_4544, VL_Os_4542, VL_Os_4543, VL_Os_1277, VL_Os_1258 */
        ARTI_TRACE(NOSUSP, AR_CP_OS_HOOK, Os, pScb->SysCore, OsHook_ProtectionHook_Return, E_OS_STACKFAULT);
        /* PRQA S 1841, 4544, 4542, 4543, 1277, 1258 -- */
        /* PRQA S 3138, 3141 -- */
        pScb->SysOsLevel = osSameLevel;
        OS_HAL_EXIT_CRITICAL();
#endif

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
void Os_FillStack(const Os_StackType *stack)
{
#if (CFG_STACK_USAGE_CHECK == TRUE)
    Os_StackPtrType ptr;
    /* PRQA S 1258, 0488, 3679 ++ */ /* VL_Os_ConstToIntegral, VL_Os_0488, VL_Os_3679 */
    Os_StackPtrType endPtr = (Os_StackDataType *)(stack->StackTop + (Os_StackDataType)CFG_STACK_CHECK_BYTE);
    /* PRQA S 1258, 0488, 3679 -- */
    /* PRQA S 0490 ++ */ /* VL_Os_0490 */
    for (ptr = (Os_StackDataType *)stack->StackBottom; ptr < endPtr; ptr++)
    /* PRQA S 0490 -- */
    {
        *ptr = OS_STACK_FILL_PATTERN;
    }
#else

    Os_FillStackByte(stack->StackBottom);/* fill stack to ensure safety*/
#if (TRUE == OS_STACK_MONITOR_BIDIRECTIONAL_CHECK)
    Os_FillStackByte(stack->StackTop);/* fill stack to ensure safety*/
#endif
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

/*=======[E N D   O F   F I L E]==============================================*/
