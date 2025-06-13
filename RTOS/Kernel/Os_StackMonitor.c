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
********************************************************************************
**                                                                            **
**  FILENAME    :  Os_StackMonitor.c                                          **
**                                                                            **
**  Created on  :                                                             **
**  Author      :  i-soft-os                                                  **
**  Vendor      :                                                             **
**  DESCRIPTION :  Stack Monitor                                              **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Os_Internal.h"

/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#if (TRUE == CFG_STACK_CHECK)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <init the system stack with the value of 0xcc>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <StartOS>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_InitSystemStack(void)
{
    Os_FillStack(*Os_SystemStack);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Os_StackMonitor>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3673 ++ */ /* MISRA Dir 8.13 */ /* OS_STACKMONITOR_POINTER_CONST_2 */
FUNC(void, OS_CODE) Os_StackMonitor(Os_StackPtrType pStack)
/* PRQA S 3673 -- */ /* MISRA Dir 8.13 */
{
    /* check system stack overflow */
    /* PRQA S 0488 ++ */ /* MISRA Rule 18.4 */ /* OS_STACKMONITOR_PTR_OPERATIONS_001 */
    if (((*(pStack)) != OS_STACK_FILL_PATTERN) || ((*(pStack + (Os_StackDataType)1)) != OS_STACK_FILL_PATTERN)
        || ((*(pStack + (Os_StackDataType)2)) != OS_STACK_FILL_PATTERN)
        || ((*(pStack + (Os_StackDataType)3)) != OS_STACK_FILL_PATTERN))
    /* PRQA S 0488 -- */ /* MISRA Rule 18.4 */
    {
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_STACKMONITOR_MACRO_TO_FUNCTION_003 */
        Os_ErrorHook(E_OS_STACKFAULT);
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */

#if (TRUE == CFG_PROTECTIONHOOK)
        (void)ProtectionHook(E_OS_STACKFAULT);
#endif

        Os_ShutdownOS(E_OS_STACKFAULT, SHUTDOWN_OS);
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <fill the stack of task or system with the value of 0xCC>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <stack:the stack of task or system>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_InitSystem>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_FillStack(Os_StackType stack)
{
    VAR(Os_StackPtrType, OS_VAR) ptr;

#if (CHECK_STACK_USAGE > 0u)
    /*
     * MISRA-C:2004 Rule 17.4
     * Msg(4:0489) The integer value 1 is being added or subtracted from a pointer.
     */
    for (ptr = (Os_StackDataType*)stack.stackBottom; ptr < (Os_StackDataType*)stack.stackTop; ptr++)
    {
        *ptr = OS_STACK_FILL_PATTERN;
    }
#else
    ptr = (Os_StackDataType*)stack.stackBottom;
    /* PRQA S 3469, 0488 ++ */ /* MISRA Dir 4.9 */ /* OS_STACKMONITOR_MACRO_TO_FUNCTION_003 */
    OS_FILL_SAFET_STACK();                         /* fill stack to ensure safety*/
/* PRQA S 3469, 0488 -- */                         /* MISRA Dir 4.9 */
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

/*=======[E N D   O F   F I L E]==============================================*/
