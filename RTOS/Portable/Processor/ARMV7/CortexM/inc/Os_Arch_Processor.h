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
********************************************************************************
**                                                                            **
**  FILENAME    : Os_Arch_Processor.h                                            **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : Processor Request Managment                                 **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r23                         **
**  Version :   AUTOSAR classic Platform r23--Function Safety                 **
**                                                                            **
*******************************************************************************/

#ifndef OS_ARCH_PROCESSOR_H
#define OS_ARCH_PROCESSOR_H

/*=======[I N C L U D E S]====================================================*/

#include "Os_Types.h"
#include "Os_Arch_Irq.h"
#include "Os_Hal_Compiler.h"
#include "Os_Kernel2Port.h"

/*=======[M A C R O S]========================================================*/
/* ========================================
* Functional Macro Defination Local Used *
======================================== */

/* XPSR for first */
#define OS_ARCH_XPSR_FOR_FIRST_ENTRY                (0x01000000U)

/* Stack alignment */
#define OS_ARCH_STACK_ALIGN_MASK                    (0xFFFFFFF8U)
#define OS_ARCH_STACK_ALIGN(addr)                   (((uint32)(addr)) & OS_ARCH_STACK_ALIGN_MASK) /* PRQA S 3472,3471 */ /* VL_Os_3472, VL_Os_3471 */
/* PRQA S 3458 ++ */ /* VL_Os_3458 */
#define OS_ARCH_SWITCH_THREAD_PRIVILEGE()                       \
        {                                                       \
            uint32 RegVal = 0U;                                  \
            OS_ASM(                                             \
                    "mrs %0, control\n"                         \
                    "bic %0, %0, #1\n"                          \
                    "msr control, %0\n"                         \
                    "isb\n"                                     \
                    : "+r"(RegVal)                              \
                    :                                           \
                    : "memory");                                \
        }

#define OS_ARCH_SWITCH_THREAD_UNPRIVILEGE()                     \
        {                                                       \
            uint32 RegVal = 0U;                                  \
            OS_ASM(                                             \
                    "mrs %0, control\n"                         \
                    "orr %0, %0, #1\n"                          \
                    "msr control, %0\n"                         \
                    "isb\n"                                     \
                    : "+r"(RegVal)                              \
                    :                                           \
                    : "memory");                                \
        }

#define OS_ARCH_PRIVILEGE_CALL()                                \
        {                                                       \
            OS_ASM("svc 0");                                    \
            OS_ASM("nop");                                      \
        }
/* PRQA S 3458 -- */
/* Msync/Isynch core instruction macros */
#define OS_ARCH_ISYNC()                             OS_ASM("isb")
#define OS_ARCH_DSYNC()                             OS_ASM("dsb")

/* Set Sp operation */
/* PRQA S 3458 ++ */ /* VL_Os_3458 */
#define OS_ARCH_SET_MSP_STACK(SP)                               \
        {                                                       \
            OS_ASM("msr msp, %0"::"r"(SP));                     \
            OS_ASM("dsb");                                      \
        }
/* PRQA S 3458 -- */
/* PendSV schedule */
#define OS_ARCH_PENDSV_REQUEST_MASK                 (OS_1BIT_MASK << OS_NUM_28)

/* ================================================
* Functional Macro Defination Provided to Kernel *
================================================ */

#if (TRUE == CFG_SRV_SHELLOS)
/* To change to the user or supervisor mode */
/* DD_1_0348 */
#define OS_HAL_ENTER_KERNEL()                               \
            uint32 CpuInformation;                          \
            CpuInformation = Os_Arch_GetCpuMode();          \
            if (OS_CPUMODE_USER0 == CpuInformation)         \
            {                                               \
                OS_ARCH_PRIVILEGE_CALL();                   \
            }                                               

#define OS_HAL_EXIT_KERNEL()                                \
            if (OS_CPUMODE_USER0 == CpuInformation)         \
            {                                               \
                OS_ARCH_SWITCH_THREAD_UNPRIVILEGE();        \
            }                                               
#else
#define OS_HAL_ENTER_KERNEL()
#define OS_HAL_EXIT_KERNEL()
#endif /* TRUE == CFG_SRV_SHELLOS */

/* Switch msp to system stack*/
#define Os_Hal_Switch2System()   /* PRQA S 3458 */ /* VL_Os_3458 */                                             \
    {                                                                                                           \
        OS_ARCH_SET_MSP_STACK(OS_ARCH_STACK_ALIGN((uint32)Os_SystemStack_Inf[Os_Hal_GetCoreID()]->StackTop));   \
    }

#define OS_HAL_TERMINATE_TASK(taskId)   /* Nothing need to do */

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
/* ============================
* Function Provided to Kernel *
============================ */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern uint32 Os_Hal_CmpSwapW(uint32* address, uint32 compareVal, uint32 exchangedVal);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Hal_FirstEnterTask(Os_TaskType taskId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Hal_StartScheduler(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Hal_Dispatch(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/* ======================
* Functional Other Used *
====================== */

#if (TRUE == OS_CFG_ASM_FPU_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Arch_FpuInit(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == OS_CFG_ASM_FPU_ENABLE */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern Os_StackPtrType Os_Arch_GetRunningTaskStack(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Arch_SetRunningTaskStack(Os_StackPtrType stackTop);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern boolean Os_Arch_IsFirstDispatch(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern Os_CpuModeType Os_Arch_GetCpuMode(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC) */ /* PRQA S 2053 */ /* VL_Os_2053 */ 

#endif /* OS_ARCH_PROCESSOR_H */

/*=======[E N D   O F   F I L E]==============================================*/
