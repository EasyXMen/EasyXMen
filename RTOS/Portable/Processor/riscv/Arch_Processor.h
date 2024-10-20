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
 **  FILENAME    : Arch_Processor.h                                            **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      : i-soft-os                                                   **
 **  Vendor      :                                                             **
 **  DESCRIPTION : Platform dependence                                         **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
 **  Version :   AUTOSAR classic Platform R19--Function Safety                 **
 **                                                                            **
 *******************************************************************************/
/* PRQA S 3108-- */
#ifndef Arch_Processor_H
#define Arch_Processor_H

/*=======[I N C L U D E S]====================================================*/
#include "Os.h"
#include "Arch_Irq.h"
#include "Arch_Mpu.h"
#include <csr.h>
#include <sbi.h>

/*=======[M A C R O S]========================================================*/
/* OS ISR */
#define  OS_ARCH_INT_CPU0                                   (0UL)
#define  OS_ARCH_INT_CPU1                                   (1UL)
#define  OS_ARCH_INT_CPU2                                   (2UL)

/* RPC */
#define  E_BUSY     0x02u
#define  RPC_TIMEOUT    (100)   //ms
#define  TIMER_FRE      (100)   //MHZ
/* stimate the number of instructions that will run */
#define  RPC_INS_NUM    (5)
#define  RPC_WAIT_TIME    ((RPC_TIMEOUT*TIMER_FRE*1000/RPC_INS_NUM))  // ull ?

/* Switch Task */
#define OS_TASK_SWITCH_PROC() Os_SwitchTask()
/* Start Dispatch */
#define OS_START_DISPATCH()

/*=======[Porting Macro]======================================================*/
#define CHECK_STACK_USAGE             1u
#define OS_ARCH_STACK_ALIGN(addr)   ((addr) & 0xFFFFFFFCu)/*4-byte alignment*/
/* DD_1_0335, Disables global interrupt. */
#define Os_ArchDisableInt()	csr_clear(CSR_STATUS, SR_IE);
/* DD_1_0336, Enables global interrupt. */
#define Os_ArchEnableInt()	csr_set(CSR_STATUS, SR_IE);
#define OS_ARCH_DECLARE_CRITICAL()  Os_ArchMsrType msr      /* DD_1_0338, Declares the variables that hold the interrupt control register. */
#define OS_ARCH_ENTRY_CRITICAL()    Os_ArchSuspendInt(&msr) /* DD_1_0339, Save the variables that hold the interrupt control register. */
#define OS_ARCH_EXIT_CRITICAL()     Os_ArchRestoreInt(msr)  /* DD_1_0340, Resume the variables that hold the interrupt control register. */

/* Disable/Enable interrupt X */
#define OS_INTERRUPT_ENABLE(id) Os_SetPending(id)
#define OS_INTERRUPT_DISABLE(id) Os_ClearPending(id)
/* Set/Clear interrupt pending X */
#define OS_INTERRUPT_CLEAR_PENDING(id)

#if (TRUE == CFG_SRV_SHELLOS)
/*to change to the user or supervisor mode*/
/* DD_1_0348 */
#define OS_CHANGCPUMODE(mode)              \
{                                          \
    CpuInformation = Os_GetCpuMode();      \
    if ((mode) == CpuInformation)          \
    {                                      \
        /* nothing to do. */               \
    }                                      \
    else                                   \
    {                                      \
            Os_ArchSyscall();              \
    }                                      \
}

#define OS_RESUMECPUMODE()                   \
{                                            \
    if (OS_CPUMODE_USER1 == CpuInformation)  \
    {                                        \
        Os_SetCpuMode(OS_CPUMODE_USER1);     \
    }                                        \
    else                                     \
    {                                        \
        /* nothing to do. */                 \
    }                                        \
}
#else /* FALSE == CFG_SRV_SHELLOS */
#define OS_CHANGCPUMODE()
#define OS_RESUMECPUMODE()
#endif /* TRUE == CFG_SRV_SHELLOS */

#if (TRUE == CFG_SRV_SHELLOS)
#define OS_ENTER_KERNEL() \
Os_CPUModeType CpuInformation;\
OS_CHANGCPUMODE(OS_CPUMODE_SUPERVISOR);
#define OS_EXIT_KERNEL() OS_RESUMECPUMODE()
#else
#define OS_ENTER_KERNEL()
#define OS_EXIT_KERNEL()
#endif

/* Request an interrupt service to the specified core. */
#define OS_INTERRUPT_SRB_SETREQ(core)
/* Request an interrupt service to the specified core. */
#define Os_ArchRemoteCall(coreId)                   \
{                                                   \
    OS_INTERRUPT_SRB_SETREQ((coreId));              \
}
/* clear an interrupt service. */
#define Os_ArchRemoteClear()

/* Pre-switch to system stack. */
#define Os_ArchPreSwitch2System()
/* Switch to system stack */
#define Os_ArchSwitch2System()                           \
{                                                        \
    Os_ArchTempSp = (uint32)Os_SystemStack->stackTop;    \
    OS_WRITE_SP(Os_ArchTempSp);                          \
}

/* Switch to ISR2 stack */
#if (CFG_ISR2_MAX > 0U)
#define Os_ArchSwitch2ISR2Stk(isrId)                       \
{                                                          \
    Os_ArchTempSp = (uint32)Os_ISR2Stack[(isrId)].stackTop;\
    OS_WRITE_SP(Os_ArchTempSp);                            \
}
#else
#define Os_ArchSwitch2ISR2Stk(isrId) 
#endif

/*update PSW for memory protection and user mode 1*/
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
#define Os_ISR_UPDATE_PSW()                               \
if ((Os_AppCfg[Os_SCB.sysRunningAppID].OsTrusted != TRUE) ||\
    (TRUE == Os_AppCfg[Os_SCB.sysRunningAppID].OsTrustedAppWithProtection))\
{                                                         \
}
#else
#define Os_ISR_UPDATE_PSW()
#endif

/*=======[I N T E R N A L   M A C R O]========================================*/
#define OS_SAVE_SP()                                                           \
{                                                                              \
    OS_READ_SP(Os_Isr2NestSp[Os_IntNestISR2]);                                 \
    if(0 == Os_IntNestISR2)                                                    \
    {                                                                          \
        Os_TaskCBExt[Os_SCB.sysRunningTaskID] = Os_ArchTempSp;                 \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        Os_IsrNestPcxStack[Os_IntNestISR2] = Os_ArchTempSp;                    \
    }                                                                          \
}
#define OS_RESUME_SP()                                                         \
{                                                                              \
    if (0U == Os_IntNestISR2)                                                  \
    {                                                                          \
        Os_ArchTempSp = Os_TaskCBExt[Os_SCB.sysRunningTaskID];                 \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        Os_ArchTempSp = Os_IsrNestPcxStack[Os_IntNestISR2];                    \
    }                                                                          \
    OS_WRITE_SP(Os_Isr2NestSp[Os_IntNestISR2]);                                \
}


#if (TRUE == CFG_INT_NEST_ENABLE)
#define OS_ISR_NEST_ENABLE(isrId)                                            \
{                                                                            \
    if (TRUE == Os_IsrCfg[isrId].OsNestedEnable)                             \
    {                                                                        \
        Os_ArchEnableInt();                                                  \
    }                                                                        \
}
#define OS_ISR_NEST_EXIT(isrId)                                              \
{                                                                            \
    if (TRUE == Os_IsrCfg[isrId].OsNestedEnable)                             \
    {                                                                        \
        Os_ArchDisableInt();                                                 \
    }                                                                        \
}
#else
#define OS_ISR_NEST_ENABLE(isrId)
#define OS_ISR_NEST_EXIT(isrId)
#endif

#if (TRUE == CFG_INT_NEST_ENABLE)
#define OS_IRQ_MODE_CHECK()
#else/* #if (FALSE == CFG_INT_NEST_ENABLE) */
#define OS_IRQ_MODE_CHECK()
#endif /*(TRUE == CFG_INT_NEST_ENABLE)*/

/* The interrupt occupies interrupt resources. */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
#define OS_ISR2OCCUPYINTRES() Os_Isr2OccupyIntRes()
#else
#define OS_ISR2OCCUPYINTRES()
#endif



/* Some operations to be performed when ISR1 enters an interrupt. */
#define OS_ARCH_ISR1_PROLOGUE(isrId)                       \
{                                                          \
    OS_READ_SP(Os_Isr1NestSp[Os_IntNestISR1]);             \
    Os_Isr1NestStack[Os_IntNestISR1] = Os_ArchTempSp;      \
    if (0U == Os_IntNestISR1)                   	       \
	{                                                      \
    	Os_SaveLevelISR1 = Os_SCB.sysOsLevel;              \
    	Os_SCB.sysOsLevel = OS_LEVEL_ISR1;                 \
    	Os_ArchSwitch2System();                            \
	}                                                      \
    Os_EnterISR1();                                        \
    Os_IntNestISR1++;                                      \
    OS_ISR_NEST_ENABLE(isrId);                             \
}


/* Some operations to be performed when ISR1 exits an interrupt. */
#define OS_ARCH_ISR1_EPILOGUE(isrId)                      \
{                                                         \
    OS_ISR_NEST_EXIT(isrId);                              \
    Os_ExitISR1();                                        \
    Os_IntNestISR1--;                                     \
    if(0U == Os_IntNestISR1)                              \
	{                                                     \
		Os_SCB.sysOsLevel = Os_SaveLevelISR1;       	  \
    }                                                     \
    Os_ArchTempSp = Os_Isr1NestStack[Os_IntNestISR1];     \
    OS_WRITE_SP(Os_Isr1NestSp[Os_IntNestISR1]);           \
}       


/* Some operations to be performed when ISR2 enters an interrupt. */
#define OS_ARCH_ISR2_PROLOGUE(isrId)           \
{                                              \
    OS_SAVE_SP();                              \
    Os_IntCfgIsrId = isrId;                    \
    Os_ArchSwitch2ISR2Stk(isrId);              \
    Os_EnterISR2();                            \
    OS_ISR_NEST_ENABLE(isrId);                 \
}  

/* Some operations to be performed when ISR2 exits an interrupt. */                        
#define OS_ARCH_ISR2_EPILOGUE(isrId)           \
{                                              \
    OS_ISR_NEST_EXIT(isrId);                   \
    Os_ExitISR2();                             \
    OS_RESUME_SP();                            \
}
                                  
                       
#define OS_ARCH_ISR2_EPILOGUE_KILL_ISR()                    

/*============[STACK CHECK]=====================*/
#define OS_FILL_SAFET_STACK()                                                \
{                                                                            \
    *ptr = OS_STACK_FILL_PATTERN;                                            \
    *(ptr + (Os_StackDataType)1) = OS_STACK_FILL_PATTERN;                    \
    *(ptr + (Os_StackDataType)2) = OS_STACK_FILL_PATTERN;                    \
    *(ptr + (Os_StackDataType)3) = OS_STACK_FILL_PATTERN;                    \
}

#define OS_ADDRESS_IS_IN_ALL_RAM(Address, Size) \
((((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pCPU5DataRamStart <= (Address))\
 && ((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pGlobleRamEnd >= ((Address) + (Size)))) \
|| (((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pLocalDRamStart <= (Address)) \
&& ((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pLocalDRamEnd >= ((Address) + (Size)))))

#define OS_ADDRESS_IS_IN_OTHER_APP(Address, Size, coreId, appsuborID) \
((((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pCPU5DataRamStart <= (Address)) \
&& ((uint32)Os_Core_App_DAddr[coreId].APP_ADDR_START >= ((Address) + (Size)))) \
|| (((uint32)Os_Core_App_DAddr[coreId].APP_ADDR_END <= (Address)) \
&& ((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pPeripheralEnd >= ((Address) + (Size)))) \
|| (((uint32)Os_App_DAddr[appsuborID].APP_ADDR_START <= (Address)) \
&& ((uint32)Os_App_DAddr[appsuborID].APP_ADDR_END >= ((Address) + (Size)))))
/*=======[T Y P E   D E F I N I T I O N S]====================================*/
/* Type of the variable to save the task control block extension. */
typedef uint32 Os_TaskCBExtType;
/* Type of the core mode. */
typedef enum
{
    OS_CORE_MODE_HALT       = 0U,
    OS_CORE_MODE_RUN        = 1U,
    OS_CORE_MODE_IDLE       = 2U,
    OS_CORE_MODE_SLEEP      = 3U,
    OS_CORE_MODE_STANDBY    = 4U,
    OS_CORE_MODE_UNKNOWN    = 5U
}Os_CoreModeType;

/*=======[E X T E R N A L   D A T A]==========================================*/
#if (TRUE == CFG_CORE0_AUTOSAROS_ENABLE)
extern VAR(Os_TaskCBExtType, OS_VAR) Os_TaskCBExtCore0[CFG_TASK_MAX_CORE0];
#endif
extern VAR(uint32, OS_VAR) Os_Isr1NestStack[CFG_ISR_MAX_CORE0];
extern VAR(uint32, OS_VAR) Os_ArchTempSp;
extern P2VAR(Os_TaskCBExtType, AUTOMATIC, OS_VAR) Os_TaskCBExt;
extern P2VAR(uint32, AUTOMATIC, OS_VAR) Os_IsrNestPcxStack;

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
extern VAR(volatile uint32, OS_VAR) Os_TprotTerminateIsr;
#endif

#if (CFG_ISR_MAX > 0U)
#if (TRUE == CFG_CORE0_AUTOSAROS_ENABLE)
extern VAR(uint32, OS_VAR) Os_IsrNestPcxStackCore0[CFG_ISR_MAX_CORE0];
#endif
extern VAR(Os_CallLevelType, OS_VAR) Os_SaveLevelISR1;
#endif /* CFG_ISR_MAX > 0U */

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
extern FUNC(void, OS_CODE) Os_ArchInitCPU(void);
extern FUNC(void, OS_CODE) Os_ArchInitIntPrio(void);
extern FUNC(void, OS_CODE) Os_ArchFirstEnterTask(void);
extern FUNC(void, OS_CODE) Os_ArchStartScheduler(void);
extern FUNC(void, OS_CODE) Os_ArchDispatch(void);
extern FUNC(void, OS_CODE) Os_StartUpResumeContext(void);

#if (TRUE == CFG_SYSTEM_TIMER_ENABLE)
extern FUNC(void, OS_CODE) Os_ArchInitSystemTimer(void);
extern FUNC(void, OS_CODE) Os_SystickIsrClearFlag(void);
#endif

extern FUNC(uint32, OS_CODE) Os_CmpSwapW(uint32* address, uint32 compareVal, uint32 exchangedVal);

extern FUNC(Os_CoreIdType,OS_CODE) Os_ArchGetCoreID(void);
extern FUNC(void, OS_CODE) Os_ArchStartCore(Os_CoreIdType coreId);
extern FUNC(Os_CoreModeType, OS_CODE) Os_GetCoreMode(Os_CoreIdType core);
extern FUNC(boolean, OS_CODE) Os_SetCoreMode(Os_CoreIdType core, Os_CoreModeType coreMode);

/* Shell protection */
extern FUNC(void, OS_CODE) Os_ArchSyscall(void);
extern FUNC(void, OS_CODE) Os_SetCpuMode(Os_CPUModeType mode); 
extern FUNC(Os_CPUModeType, OS_CODE) Os_GetCpuMode(void);

extern FUNC(void, OS_CODE) OS_GetSystemStackPointer(void);
extern FUNC(void, OS_CODE) OS_GetTaskStackPointer(void);
extern FUNC(void, OS_CODE) OS_SaveTaskStackPointer(void);
extern FUNC(void, OS_CODE) OS_SaveIsr2StackPointer(void);

extern FUNC(void, OS_CODE) Os_TaskEntry_IdleCore0(void);
extern FUNC(void, OS_APPL_CODE) IdleHook_Core0(void);
/* Timing protection. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
extern FUNC(void, OS_CODE) Os_ArchDisableAllInt_ButTimingProtInt(void);
extern FUNC(void, OS_CODE) Os_ArchEnableAllInt_ButTimingProtInt(void);

extern FUNC(void, OS_CODE) Os_TimeProtClearFlag(void);
extern FUNC(void, OS_CODE) Os_ArchInitTimingProtTimer(void);

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE_CORE0)
extern FUNC(void, OS_CODE_FAST) Os_TimeProtection_Handler0(void);
#endif
#endif /* #if (TRUE == CFG_TIMING_PROTECTION_ENABLE) */

#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (CFG_ISR_MAX > 0)
extern FUNC(void, OS_CODE) Os_ArchDisableIntInApp
(
    P2CONST(Os_ApplicationCfgType, AUTOMATIC, OS_VAR) posCurAppCfg
);
extern FUNC(void, OS_CODE) Os_ArchAppTerminateIsrProc(Os_IsrType OsIsrID);
#endif /* CFG_ISR_MAX > 0 */
extern FUNC(void, OS_CODE) Os_ArchAppTerminateTaskProc(Os_TaskType OsTaskID);
#endif /* (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC) */

#endif  /* #ifndef Arch_Processor_H */
/*=======[E N D   O F   F I L E]==============================================*/
