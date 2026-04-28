/* PRQA S 3108++ */
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
 **  FILENAME    : Os_Arch_Processor                                           **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      :  i-soft-os                                                  **
 **  Vendor      :                                                             **
 **  DESCRIPTION :                                                             **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
 **  Version :   AUTOSAR classic Platform R19--Function Safety                 **
 **                                                                            **
 *******************************************************************************/
/* PRQA S 3108-- */
#ifndef ARCH_PROCESSOR_H
#define ARCH_PROCESSOR_H

/*=======[I N C L U D E S]====================================================*/
#include "Os_Types.h"

#include "Os_Arch_Irq.h"
#include "Os_Arch_Mpu.h"
#include "Os_Mcu_Core.h"
#include "Os_Mcu_Irq.h"
#include "Os_Mcu_Timer.h"

/*=======[Porting Macro]======================================================*/
/*Program registers:r6-r31,r1-r2*/
#define OS_PROGRAM_REGS_NUM (27U)

#define OS_PSW_EBV_BIT (15U)
#define OS_PSW_FPU_BIT (16U)
#define OS_PSW_UM_BIT  (30U)

#define OS_FPSR_FS_BIT (17U)

/* PRQA S 3472 ++*/ /* VL_Os_3409*/
#define OS_ARCH_STACK_ALIGN(addr) ((addr) & 0xFFFFFFFCUL)
/* PRQA S 3472 --*/

#define OS_ARCH_SUPERVISOR_MODE (0x00U)

/* SP = Os_ArchTempSp */
#define OS_SET_SP_VAL()                      \
    do                                       \
    {                                        \
        ASM("mov _Os_ArchTempSp, r7    \n"   \
            "ld.w 0[r7], sp            \n"); \
    } while (0)

/* switch to system stack for RH850 */
#define Os_Hal_Switch2System()                                                      \
    do                                                                              \
    {                                                                               \
        Os_ArchTempSp = OS_ARCH_STACK_ALIGN((uint32)Os_SystemStack_Inf[Os_Hal_GetCoreID()]->StackTop); \
        OS_SET_SP_VAL();                                                            \
    } while (0)

#define Os_Hal_Dispatch()      ASM("trap 0x00")
#define Os_ArchFirstDispatch() ASM("trap 0x10")
/* Start Dispatch */
#define OS_START_DISPATCH()

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
#define Os_GetCpuMode()       (OS_ARCH_REG_READ(OS_PSW_NUM) & 0x40000000U)
#define Os_ArchSwith2SvMode() ASM("trap 0x11")
#define Os_ArchSwith2UserMode()                                                        \
    do                                                                                 \
    {                                                                                  \
        if (OS_ARCH_SUPERVISOR_MODE == Os_GetCpuMode())                                \
        {                                                                              \
            OS_ARCH_REG_WRITE(OS_PSW_NUM, OS_ARCH_REG_READ(OS_PSW_NUM) | 0x40000000U); \
        }                                                                              \
    } while (0)

#define OS_HAL_ENTER_KERNEL() /* PRQA S 3412 */ /* VL_Os_3412 */ \
    uint32 Os_CurCpuMode;                                        \
    {                                                            \
        Os_CurCpuMode = Os_GetCpuMode();                         \
        if (OS_ARCH_SUPERVISOR_MODE != Os_CurCpuMode)            \
        {                                                        \
            Os_ArchSwith2SvMode();                               \
        }                                                        \
    }

#define OS_HAL_EXIT_KERNEL() /* PRQA S 3472 */ /* VL_Os_3472 */ \
    do                                                          \
    {                                                           \
        if (OS_ARCH_SUPERVISOR_MODE != Os_CurCpuMode)           \
        {                                                       \
            Os_ArchSwith2UserMode();                            \
        }                                                       \
    } while (0)
#else
#define OS_HAL_ENTER_KERNEL()
#define OS_HAL_EXIT_KERNEL()
#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */

#define OS_HAL_TERMINATE_TASK(x)

/*=======[I N T E R N A L   M A C R O]========================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/
extern volatile uint32   Os_ArchTempSp;
extern Os_TaskCBExtType Os_TaskCBExt[CFG_TASK_MAX];

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
extern void   Os_Hal_FirstEnterTask(Os_TaskType TaskID);
extern void   Os_Hal_StartScheduler(void);
extern uint32 Os_Hal_CmpSwapW(uint32* address, uint32 compareVal, uint32 exchangedVal);

extern void OS_GetSystemStackPointer(void);
extern void OS_GetTaskStackPointer(void);
extern void OS_SaveTaskStackPointer(void);
/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

#endif /* ARCH_PROCESSOR_H */
/*=======[E N D   O F   F I L E]==============================================*/
