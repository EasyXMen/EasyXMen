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
 **  @file               : Os_Arch_Irq.h
 **  @author             : i-soft-os
 **  @date               : 2024/02/10
 **  @vendor             : isoft
 **  @description        : Arch header file for Irq api declarations
 **
 ***********************************************************************************************************************/

#ifndef OS_IRQ_H
#define OS_IRQ_H

/*=======[I N C L U D E S]=================================================*/
#include "Os_Kernel2Port.h"
#include "Os_Arch_Define.h"
#include "Os_Arch_Mpu.h"
/*=======[M A C R O S]=====================================================*/

extern uint32* Os_IsrNestPcxStack;

#define TRAP_0_NUM  (0)
#define TRAP_1_NUM  (1)
#define TRAP_2_NUM  (2)
#define TRAP_3_NUM  (3)
#define TRAP_4_NUM  (4)
#define TRAP_5_NUM  (5)
#define SYSCALL_NUM (6)
#define TRAP_7_NUM  (7)

#define ARCH_IPI_MASK     (0xFFFFFF00U)
#define ARCH_ICR_CCR_BIT  (15U)
#define ARCH_ICR_IPI_MASK (0xFFU)

#define Os_ArchRemoteClear()

#ifndef MAX_INTRS
#define MAX_INTRS 256u /* Maximum number of interrupt requests. */
#endif

#define Os_HAL_INT_IS_CLEARPENDING_SUPPORTED() (TRUE) /* PRQA S 3472 */ /* VL_Os_3472 */

/* Critical */
#define OS_HAL_DECLARE_CRITICAL() \
    Os_ArchMsrType msr /* Declares the variables that hold the interrupt control register. */
/* Declares the variables that hold the interrupt control register. */
#define OS_HAL_ENTRY_CRITICAL() /* PRQA S 3409 */ /* VL_Os_3409 */                                                                                    \
    msr = Os_Hal_SuspendInt()                    /* Save the variables that hold the interrupt control register. */ \
        /* PRQA S 3472, 3409 */ /* VL_Os_3472 */ /* VL_Os_3409 */
#define OS_HAL_EXIT_CRITICAL() Os_Hal_RestoreInt(msr) /* Resume the variables that hold the interrupt control register. */ /* PRQA S 3472 */ /* VL_Os_3472 */

#define OS_HAL_ISR2_EPILOGUE_KILL_ISR(isrId) OS_ARCH_ISR2_EPILOGUE(isrId)

#if (TRUE == CFG_PROTECTIONHOOK)
#define OS_HAL_ISR2_EPILOGUE(isrId) OS_ARCH_ISR2_EPILOGUE(isrId)
#endif

/* Switch to ISR2 stack */
#if (CFG_ISR2_MAX > 0U)
#define Os_ArchSwitch2ISR2Stk(isrId) /* PRQA S 3458 */ /* VL_Os_3458 */        \
    {                                                                          \
        Os_ArchSp = OS_ARCH_STACK_ALIGN((uint32)Os_ISR2Stack[isrId].StackTop); \
        OS_ARCH_STORE_SP();                                                    \
    }
#else /* CFG_ISR2_MAX == 0U */
#define Os_ArchSwitch2ISR2Stk(isrId)
#endif /* CFG_ISR2_MAX > 0U */

/* Save the context. */
#define OS_ARCH_SAVE_CONTEXT(TaskID) /* PRQA S 3472, 3458 */ /* VL_Os_3472 */ /* VL_Os_3458 */ \
    {                                                                                          \
        Os_TaskCBExt[TaskID].pcx = OS_ARCH_MFCR(OS_REG_PCX);                                   \
    }
/* Resume the context. */
#define OS_ARCH_RESUME_CONTEXT(TaskID) /* PRQA S 3412 */ /* VL_Os_3412 */ \
    do                                                                    \
    {                                                                     \
        OS_ARCH_RECLAIM_CSAS_WITH_ISR_RECOVERY_EXCEPTION();               \
        Os_ArchResumeTaskContext(TaskID);                                 \
    } while (0);

/* PRQA S 1006++ */ /* VL_Os_1006 */
static inline OS_ALWAYS_INLINE void Os_ArchReturnFromTrapOrISR(void)
{
    OS_ARCH_DSYNC();
    OS_ASM("rslcx");
    OS_ASM("nop");
    OS_ASM("rfe");
}

static inline OS_ALWAYS_INLINE void Os_ArchResumeTaskContext(Os_TaskType TaskID) /* PRQA S 3006 */ /* VL_Os_3006 */
{
    OS_ARCH_MTCR(OS_REG_PCX, Os_TaskCBExt[TaskID].pcx)
    Os_ArchReturnFromTrapOrISR();
}
/* PRQA S 1006-- */

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
#define Os_MemProtSetIsr1() Os_ArchSetMemProtSet(PSW_PRS_SET0)
#define Os_ResumeIsrMode()  OS_SYSCALL(2u); /* PRQA S 3412 */ /* VL_Os_3412 */
#else
#define Os_MemProtSetIsr1()
#define Os_ResumeIsrMode()
#endif

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#define Os_ArchTprotEnterIsr2Proc(isrId) /* PRQA S 3412 */ /* VL_Os_3412 */ \
    if (NULL_PTR != Os_IsrCfg[isrId].isrTmProtCfgRef)                       \
    {                                                                       \
        if (E_OK != Os_TmProtIsrFrameChk(isrId))                            \
        {                                                                   \
            Os_ArchReturnFromTrapOrISR();                                   \
        }                                                                   \
    }
#else
#define Os_ArchTprotEnterIsr2Proc(isrId)
#endif

#if (TRUE == CFG_INT_NEST_ENABLE)
#define Os_ArchSavePreConetxtIsr2() /* PRQA S 3412 */ /* VL_Os_3412 */            \
    Os_IsrNestPcxStack[Os_Kernel_GetIntNestNumISR2()] = OS_ARCH_MFCR(OS_REG_PCX); \
    if (0u == Os_Kernel_GetIntNestNumISR2())                                      \
    {                                                                             \
        OS_ARCH_SAVE_CONTEXT(Os_Kernel_GetSysRunningTaskID());                    \
    }

#define Os_ArchEnableNestIsr(isrId) /* PRQA S 3412 */ /* VL_Os_3412 */ \
    if (TRUE == Os_IsrCfg[isrId].NestedEnable)                         \
    {                                                                  \
        Os_Hal_EnableInt();                                            \
    }

#define Os_ArchIsr1Switch2System() /* PRQA S 3412 */ /* VL_Os_3412 */ \
    if (0U == Os_Kernel_GetIntNestNumISR1())                          \
    {                                                                 \
        Os_Hal_Switch2System();                                       \
    }
#else
#define Os_ArchSavePreConetxtIsr2() OS_ARCH_SAVE_CONTEXT(Os_Kernel_GetSysRunningTaskID()); /* PRQA S 3412 */ /* VL_Os_3412 */
#define Os_ArchEnableNestIsr(isrId)
#define Os_ArchIsr1Switch2System()
#endif

/* +++++++++++++++++++++++ ISR1 +++++++++++++++++++++++++++++++ */
#define OS_ARCH_ISR1_PROLOGUE(isrId) \
    do                               \
    {                                \
        Os_MemProtSetIsr1();         \
        Os_ArchIsr1Switch2System();  \
        Os_EnterISR1(isrId);         \
        Os_ArchEnableNestIsr(isrId); \
        Os_ArchMemProtSwithToUser(); \
    } while (0)

#define OS_ARCH_ISR1_EPILOGUE(isrId) /* PRQA S 3472 */ /* VL_Os_3472 */ \
    do                                                                  \
    {                                                                   \
        Os_Hal_DisableInt();                                            \
        Os_ExitISR1(isrId);                                             \
        Os_ArchReturnFromTrapOrISR();                                       \
    } while (0)
/* ------------------------- ISR1 ------------------------------- */

/* +++++++++++++++++++++++ ISR2 +++++++++++++++++++++++++++++++ */
#define OS_ARCH_ISR2_PROLOGUE(isrId)      \
    do                                    \
    {                                     \
        Os_ArchSavePreConetxtIsr2();      \
        Os_ArchTprotEnterIsr2Proc(isrId); \
        Os_ArchSwitch2ISR2Stk(isrId);     \
        Os_EnterISR2(isrId);              \
        Os_ArchEnableNestIsr(isrId);      \
        Os_ArchMemProtSwithToUser();      \
    } while (0)

#define OS_ARCH_ISR2_EPILOGUE(isrId) /* PRQA S 3472 */ /* VL_Os_3472 */ \
    do                                                                  \
    {                                                                   \
        Os_ResumeIsrMode();                                             \
        OS_ISR2OCCUPYINTRES(isrId);                                     \
        Os_Hal_DisableInt();                                            \
        Os_ArchMemProtSwithToSystem();                                  \
        Os_ArchTprotTerminateTask();                                    \
        Os_ExitISR2(isrId);                                             \
        OS_ASM("j Os_ArchResumeContextFromISR2");                       \
    } while (0)

/* ------------------------- ISR2 ------------------------------- */

/*=======[T Y P E   D E F I N I T I O N S]==================================*/
/* Type of the interrupt service routine (ISR). */
typedef void (*Os_isrhnd)(void); /* PRQA S 5004 */ /* VL_QAC_FuncPtr */

/*=======[E X T E R N A L   D A T A]========================================*/

extern Os_isrhnd Cdisptab[MAX_INTRS]; /* PRQA S 3672 */ /* VL_Os_3672 */

extern volatile uint32 Os_ArchSp; /* PRQA S 3451, 3449 */ /* VL_Os_3451 */ /* VL_Os_3449 */
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]======*/
/******************************************************************************/
/*
 * Brief                <get the IPL>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <0>
 * PreCondition         <None>
 * CallByAPI            <SuspendOSInterrupts and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
extern Os_IPLType Os_Hal_GetIpl(void);
/******************************************************************************/
/*
 * Brief                <Ipl greater than 0, disable interrupt otherwise enable
 *                       interrupt>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <ipl>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ResumeOSInterrupts and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
extern void Os_Hal_SetIpl(Os_IPLType ipl, Os_IsrDescriptionType isrDesc);
/******************************************************************************/
/*
 * Brief                <Save ccr to variable msr and disable maskable interrupt>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <SuspendAllInterrupts>
 * REQ ID               <None>
 */
/******************************************************************************/
extern Os_ArchMsrType Os_Hal_SuspendInt(void); /* PRQA S 3432 */ /* VL_Os_3432 */
/******************************************************************************/
/*
 * Brief                <transfer variable msr back to ccr>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ResumeAllInterrupts>
 * REQ ID               <None>
 */
/******************************************************************************/
extern void Os_Hal_RestoreInt(Os_ArchMsrType msr);

/*Default interrupt service function.*/
extern void Os_IntHandler(void);
/******************************************************************************/
/*
 * Brief                <Initialize interrupts and traps>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <Os_Hal_InitCPU>
 * REQ ID               <>
 */
/******************************************************************************/
extern void Os_ArchInterruptInit(void);

/* Memory protection. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
/******************************************************************************/
/*
 * Brief                <Disable all interrupts except for those related to time protection.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_DisableAllInterrupts,Os_SuspendAllInterrupts>
 * REQ ID               <None>
 */
/******************************************************************************/
extern void Os_Hal_DisableAllInt_ButTimingProtInt(Os_CoreIdType CoreId);
/******************************************************************************/
/*
 * Brief                <Enable all interrupts except for the time protection interrupt.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_EnableAllInterrupts,Os_ResumeAllInterrupts>
 * REQ ID               <None>
 */
/******************************************************************************/
extern void Os_Hal_EnableAllInt_ButTimingProtInt(Os_CoreIdType CoreId);
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */

/******************************************************************************/
/*
 * Brief                <Resume context from ISR2.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <None>
 * REQ ID               <>
 */
/******************************************************************************/
extern void Os_ArchResumeContextFromISR2(void);
/*=======[I N T E R N A L   D A T A]========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]======*/
#endif /* OS_IRQ_H */
/*=======[E N D   O F   F I L E]==============================================*/
