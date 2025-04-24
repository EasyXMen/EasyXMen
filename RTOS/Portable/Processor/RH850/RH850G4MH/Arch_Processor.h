/* PRQA S 3108++ */
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
 ********************************************************************************
 **                                                                            **
 **  FILENAME    :  Arch_Processor.h                                           **
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

#include "Arch_Irq.h"
#include "Arch_Mpu.h"
#include "Mcu_Core.h"
#include "Mcu_Int.h"
#include "Mcu_Timer.h"

/*=======[Porting Macro]======================================================*/
/* Provided for external use */

#define OS_ARCH_STACK_ALIGN(addr) ((addr) & 0xFFFFFFFCU)

#define ASM                       __asm volatile

/* RPC */
#define E_BUSY      0x02u
#define RPC_TIMEOUT (100)
#define TIMER_FRE   (100)
/* stimate the number of instructions that will run */
#define RPC_INS_NUM   (5)
#define RPC_WAIT_TIME ((RPC_TIMEOUT * TIMER_FRE * 1000 / RPC_INS_NUM))

/* Base Register Read and Write */
/* PRQA S 3206,3008,6008++ */ /* MISRA Rule 2.7 Dir 4.3*/
static inline void OS_ARCH_REG_WRITE(uint32 regId, uint32 selId, uint32 val)
{
    ASM("LDSR %2, %0 , %1" ::"%"(regId), "%"(selId), "r"(val) : "memory");
}

static inline uint32 OS_ARCH_REG_READ(uint32 regId, uint32 selId)
{
    uint32 val;
    ASM("STSR %1, %0, %2" : "=r"(val) : "%"(regId), "%"(selId) : "memory");
    return val;
}
/* PRQA S 3206,3008,6008-- */ /* MISRA Rule 2.7 Dir 4.3 */

#define OS_ARCH_SUPERVISOR_MODE (0x00U)

/* SP = Os_ArchTempSp */
#define OS_SET_SP_VAL()                      \
    {                                        \
        ASM("mov _Os_ArchTempSp, r7    \n"   \
            "ld.w 0[r7], sp            \n"); \
    }

/* switch to system stack for RH850 */
#define Os_ArchSwitch2System()                                                 \
    {                                                                          \
        Os_ArchTempSp = OS_ARCH_STACK_ALIGN((uint32)Os_SystemStack->stackTop); \
        OS_SET_SP_VAL();                                                       \
    }

/* switch to ISR2 stack for RH850 */
#if (CFG_ISR2_MAX > 0U)
#define Os_ArchSwitch2ISR2Stk(isrId)                                               \
    {                                                                              \
        Os_ArchTempSp = OS_ARCH_STACK_ALIGN((uint32)Os_ISR2Stack[isrId].stackTop); \
        OS_SET_SP_VAL();                                                           \
    }
#endif

#define Os_ArchDispatch()      ASM("trap 0x00")
#define Os_ArchFirstDispatch() ASM("trap 0x10")
/* Start Dispatch */
/* Provided for external use */
#define OS_START_DISPATCH()
/* Switch Task */
/* Provided for external use */
#define OS_TASK_SWITCH_PROC()      Os_SwitchTask()

#define OS_ARCH_DECLARE_CRITICAL() Os_ArchMsrType msr
#define OS_ARCH_ENTRY_CRITICAL()   Os_ArchSuspendInt(&msr)
#define OS_ARCH_EXIT_CRITICAL()    Os_ArchRestoreInt(msr)
#define OS_ARCH_SUSPEND_ALLINT()   Os_ArchSuspendInt(&msr)
#define OS_ARCH_RESTORE_ALLINT()   Os_ArchRestoreInt(msr)

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
#define Os_GetCpuMode()       (OS_ARCH_REG_READ(OS_PSW_NUM) & 0x40000000U)
#define Os_ArchSwith2SvMode() ASM("trap 0x11")
#define Os_ArchSwith2UserMode()                                                        \
    {                                                                                  \
        if (OS_ARCH_SUPERVISOR_MODE == Os_GetCpuMode())                                \
        {                                                                              \
            OS_ARCH_REG_WRITE(OS_PSW_NUM, OS_ARCH_REG_READ(OS_PSW_NUM) | 0x40000000U); \
        }                                                                              \
    }

#define OS_ENTER_KERNEL()                             \
    uint32 Os_CurCpuMode;                             \
    {                                                 \
        Os_CurCpuMode = Os_GetCpuMode();              \
        if (OS_ARCH_SUPERVISOR_MODE != Os_CurCpuMode) \
        {                                             \
            Os_ArchSwith2SvMode();                    \
        }                                             \
    }

#define OS_EXIT_KERNEL()                              \
    {                                                 \
        if (OS_ARCH_SUPERVISOR_MODE != Os_CurCpuMode) \
        {                                             \
            Os_ArchSwith2UserMode();                  \
        }                                             \
    }
#else
#define OS_ENTER_KERNEL()
#define OS_EXIT_KERNEL()
#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */

/* Provided for external use */
#define Os_ArchPreSwitch2System()

#if (OS_AUTOSAR_CORES > 1)
#define Os_ArchRemoteCall(remoteCoreId)                            \
    {                                                              \
        OS_IPIR_IPInREQm(Os_SCB.sysCore, 0) = (1 << remoteCoreId); \
    }

#define Os_ArchRemoteClear()                            \
    {                                                   \
        uint8 flags = OS_IPIR_IPInFLGS(0);              \
        uint8 flagMask = flags - ((flags - 1) & flags); \
        OS_IPIR_IPInFCLRS(0) = flagMask;                \
    }

/* Boot Control Register */
#define OS_BOOTCTRL_ADDR (*((volatile uint32*)(0xFFFB2000U)))
#endif

/*=======[I N T E R N A L   M A C R O]========================================*/
/*============[STACK CHECK]=====================*/
/* Provided for external use */
#define OS_FILL_SAFET_STACK()                                 \
    {                                                         \
        *ptr = OS_STACK_FILL_PATTERN;                         \
        *(ptr + (Os_StackDataType)1) = OS_STACK_FILL_PATTERN; \
        *(ptr + (Os_StackDataType)2) = OS_STACK_FILL_PATTERN; \
        *(ptr + (Os_StackDataType)3) = OS_STACK_FILL_PATTERN; \
    }

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
typedef uint32 Os_TaskCBExtType;

/*=======[E X T E R N A L   D A T A]==========================================*/
/* PRQA S 3684++ */ /* MISRA Rule 8.11 */
extern uint8 _OS_SYSTEM_STACK_START[];
extern uint8 _OS_SYSTEM_STACK_END[];
/* PRQA S 3684-- */ /* MISRA Rule 8.11 */

extern VAR(volatile uint32, OS_VAR) Os_ArchTempSp;

/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern P2VAR(Os_TaskCBExtType, AUTOMATIC, OS_VAR) Os_TaskCBExt;

extern VAR(volatile uint32, OS_VAR) Os_ArchMasterSp;

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
extern FUNC(void, OS_CODE) Os_ArchInitCPU(void);
extern FUNC(void, OS_CODE) Os_ArchFirstEnterTask(void);
extern FUNC(void, OS_CODE) Os_ArchStartScheduler(void);

extern FUNC(void, OS_CODE) OS_GetSystemStackPointer(void);
extern FUNC(void, OS_CODE) OS_GetTaskStackPointer(void);
extern FUNC(void, OS_CODE) OS_SaveTaskStackPointer(void);

#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (CFG_ISR_MAX > 0)
extern FUNC(void, OS_CODE) Os_ArchDisableIntInApp(P2CONST(Os_ApplicationCfgType, AUTOMATIC, OS_VAR) posCurAppCfg);
extern FUNC(void, OS_CODE) Os_ArchAppTerminateIsrProc(Os_IsrType OsIsrID);
#endif /* CFG_ISR_MAX > 0 */
extern FUNC(void, OS_CODE) Os_ArchAppTerminateTaskProc(Os_TaskType OsTaskID);
extern FUNC(void, OS_CODE) Os_ArchReclaimIsrCsasTermApp(uint32 isrSP_Next, uint32 isrSP_Cur, uint32 isrId);
#endif /* (OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC) */

extern uint32 Os_CmpSwapW(uint32* address, uint32 compareVal, uint32 exchangedVal);
/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

#endif /* ARCH_PROCESSOR_H */
/*=======[E N D   O F   F I L E]==============================================*/
