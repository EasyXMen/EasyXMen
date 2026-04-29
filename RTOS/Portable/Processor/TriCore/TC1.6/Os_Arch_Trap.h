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
 **  @file               : Os_Arch_Trap.h
 **  @author             : i-soft-os
 **  @date               : 2024/02/10
 **  @vendor             : isoft
 **  @description        : Arch header file for Trap api declarations
 **
 ***********************************************************************************************************************/

#ifndef OS_TRAP_H
#define OS_TRAP_H

/*=======[I N C L U D E S]====================================================*/
#include "Std_Types.h"
#include "Os_Arch_Define.h"
#include "Os_FaultManager.h"

/*=======[M A C R O S]========================================================*/
#ifndef MAX_TRAPS
#define MAX_TRAPS 8u /* The maximum number of trap requests. */
#endif

/*it's not standard autosar error status*/
#define E_OS_Illegal_Opcode                25U
#define E_OS_Unimplemented_Opcode          26U
#define E_OS_Invalid_Operand               27U
#define E_OS_Data_Address_Align            28U
#define E_OS_Invalid_Local_Memory          29U
#define E_OS_Program_Fetch_Synchronous     30U
#define E_OS_Data_Access_Synchronous       31U
#define E_OS_Data_Acess_Asynchronous       32U
#define E_OS_Coprocessor_Trap_Asynchronous 33U
#define E_OS_Program_Memory_Integrity      34U
#define E_OS_Data_Memory_Integrity         35U
#define E_OS_Temporal_Asynchronous         36U
#define E_OS_Arithmetic_Overflow           37U
#define E_OS_Sticky_Arithmetic_Overflow    38U
#define E_OS_NMI                           39U
#define E_OS_Virtual_Address_Fill          40U
#define E_OS_Virtual_Address_Protection    41U
#define E_OS_Free_Context_List_Depletion   42U
#define E_OS_Call_Depth_Overflow           43U
#define E_OS_Call_Depth_Underflow          44U
#define E_OS_Free_Context_List_Underflow   45U
#define E_OS_Call_Stack_Underflow          46U
#define E_OS_Context_Type                  47U
#define E_OS_Nesting_Error                 48U

/* PRQA S 3458 ++ */ /* VL_Os_3412 */
#define Os_ArchSyscallSwitchToKernelStack(userSp, kerStackTop)        \
    {                                                                 \
        OS_ARCH_GET_TRAP_SP();                                        \
        (userSp)      = Os_ArchTrapSp;                                \
        Os_ArchTrapSp = (kerStackTop);                                \
        OS_ARCH_STORE_TRAP_SP();                                      \
    }

#define Os_ArchSyscallSwitchToPrevStack(userSp)      \
    {                                                \
        Os_ArchTrapSp = (userSp);                    \
        OS_ARCH_STORE_TRAP_SP();                     \
    }
/* PRQA S 3458 -- */
/*=======[T Y P E   D E F I N I T I O N S]====================================*/
/* Type of the trap. */
typedef void (*Os_traphnd)(uint32 Os_Tin); /* PRQA S 3432, 5004 */ /* VL_Os_3432, VL_QAC_FuncPtr */

/* Type of the trap msg. */
typedef struct
{
    uint32 Pcx;
    uint32 Tin;
} Os_ArchTrapMsgType;
/*=======[E X T E R N A L   D A T A]==========================================*/
extern Os_traphnd Tdisptab[MAX_TRAPS]; /* PRQA S 3672 */ /* VL_Os_3672 */
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]======*/
/******************************************************************************/
/*
 * Brief                <Call PendSV to switch task.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_SystemCallTrap>
 * REQ ID               <None>
 */
/******************************************************************************/
#if (TRUE == CFG_FAULT_MANAGEMENT_ENABLE)
extern void Os_Hal_ExceptionInformationParse(Os_TrapContextType* TrapContext);
extern void Os_HalExceptionSaveContext(const void *trapMsg);
extern void Os_HalExceptionResumeContext(const void *trapMsg);
#endif /* TRUE == CFG_FAULT_MANAGEMENT_ENABLE */

extern void Os_ArchDispatcher(void);
extern void Trap0(uint32 Os_Tin);
extern void Trap1(uint32 Os_Tin);
extern void Trap2(uint32 Os_Tin);
extern void Trap3(uint32 Os_Tin);
extern void Trap4(uint32 Os_Tin);
extern void Trap5(uint32 Os_Tin);
extern void Os_SystemCallTrap(uint32 Os_Tin);
extern void Trap7(uint32 Os_Tin);
/*=======[I N T E R N A L   D A T A]==========================================*/
extern volatile uint32 Os_ArchTrapSp;
/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]======*/
#endif /* OS_TRAP_H */
/*=======[E N D   O F   F I L E]==============================================*/
