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
 **  FILENAME    : Arch_Irq.c                                                  **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      : i-soft-os                                                   **
 **  Vendor      :                                                             **
 **  DESCRIPTION : Deal with operations related to processor interrupts        **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
 **  Version :   AUTOSAR classic Platform R19--Function Safety                 **
 **                                                                            **
 *******************************************************************************/
/* PRQA S 3108-- */
/*=======[I N C L U D E S]====================================================*/
#include "Os_Internal.h"
/*=======[M A C R O S]========================================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/
/* PRQA S 0791 ++ */ /* MISRA Rule 5.4 */
#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
/*The top of the stack that holds the FE level interrupt.*/
/*Provided for external use*/
VAR(uint32, OS_VAR) Os_IsrFE_SP;
/*The level of the system call when a type of ISR1 is triggered.*/
/*Provided for external use*/
VAR(Os_CallLevelType, OS_VAR) Os_SaveLevelISR1;
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
/*Trigger time protection after termination of the flag.*/
/*Provided for external use*/
VAR(volatile uint32, OS_VAR) Os_TprotTerminateIsr;
#endif
/* When an interrupt occurs, the stacked SP is stored in Os_ArchTempSp, and Os_ArchTempSp is stored in this parameter.
 */
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
P2VAR(uint32, AUTOMATIC, OS_VAR) Os_IsrNestPcxStack;
/*Isr1 stack top.*/
P2VAR(uint32, AUTOMATIC, OS_VAR) Os_ISR1SP;
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
VAR(uint32, OS_VAR) InitInterruptFlag;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
/*=======[I N T E R N A L   D A T A]==========================================*/
#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
static VAR(volatile uint32, OS_VAR) Os_IsrPriority; /* PRQA S 3218 */ /* MISRA Rule 8.9 */
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#define OS_START_SEC_CODE_FAST
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Default interrupts handler>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <None>
 * REQ ID               <>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_IntHandler(void)
{
    /* Just ignore this interrupt.  */
    while (1)
    {
    }
}
#define OS_STOP_SEC_CODE_FAST
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Set the priority of the current CPU. >
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <ipl,isrdesc>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ResumeOSInterrupts and so on>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ArchSetIpl(Os_IPLType ipl, Os_IsrDescriptionType isrdesc)
{
    (void)isrdesc;
    Os_IPLType cpuPrio = (Os_IPLType)ipl;

    if (INT_PRIORITY_MAX <= cpuPrio)
    {
        cpuPrio = 0;
    }
    else
    {
        cpuPrio = INT_PRIORITY_MAX - cpuPrio;
    }

    /*All interrupts with priority Numbers greater than or equal to
     * 'PMR' are turned off*/
    OS_ARCH_REG_WRITE(OS_PLMR_NUM, cpuPrio);
}

/******************************************************************************/
/*
 * Brief                <Get the priority of the current CPU. >
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <Os_IPLType>
 * PreCondition         <None>
 * CallByAPI            <SuspendOSInterrupts and so on>
 */
/******************************************************************************/
FUNC(Os_IPLType, OS_CODE) Os_ArchGetIpl(void)
{
    Os_IPLType result = 0;

    uint32 pro = OS_ARCH_REG_READ(OS_PLMR_NUM) & 0x3FU; /* read interrupt priority masking, 0-15 */
    if (0U != pro)
    {
        result = INT_PRIORITY_MAX - pro;
    }
    else
    {
        result = INT_PRIORITY_MAX;
    }

    return (result);
}

/******************************************************************************/
/*
 * Brief                <Save the status register and disable maskable interrupts.>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <msr>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <SuspendAllInterrupts>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ArchSuspendInt(Os_ArchMsrRefType msr)
{
    uint32 psw = OS_ARCH_REG_READ(OS_PSW_NUM);
    uint32 id = psw & 0x00000020u;

    if (0U == id)
    {
        Os_ArchDisableInt();
    }
    *msr = id;
}

/******************************************************************************/
/*
 * Brief                <Resume the variables that hold the status register.>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <msr>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ResumeAllInterrupts>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ArchRestoreInt(Os_ArchMsrType msr)
{
    if (0U == msr)
    {
        Os_ArchEnableInt();
    }
}

/******************************************************************************/
/*
 * Brief                <Enable the interrupt source.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Param-Name[in]       <vIsrSrc,vIsrSrcType>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_EnableInterruptSource(uint32 vIsrSrc, uint32 vIsrSrcType)
{
    (void)vIsrSrcType;
    VAR(Os_ArchMsrType, OS_VAR) state;

    Os_ArchSuspendInt(&state);
    OS_INTERRUPT_ENABLEREQ(vIsrSrc); /* PRQA S 0303 */ /* MISRA Rule 11.4 */
    Os_ArchRestoreInt(state);
}
/******************************************************************************/
/*
 * Brief                <Disable the interrupt source.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Param-Name[in]       <vIsrSrc,vIsrSrcType>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_DisableInterruptSource(uint32 vIsrSrc, uint32 vIsrSrcType)
{
    (void)vIsrSrcType;
    VAR(Os_ArchMsrType, OS_VAR) state;

    Os_ArchSuspendInt(&state);
    OS_INTERRUPT_DISABLEREQ(vIsrSrc); /* PRQA S 0303 */ /* MISRA Rule 11.4 */
    Os_ArchRestoreInt(state);
}

FUNC(boolean, OS_CODE) Os_ArchIntIsClearPendingSupported(void)
{
    return TRUE;
}
/******************************************************************************/
/*
 * Brief                <Clear software interrupt source.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Param-Name[in]       <vIsrSrc,vIsrSrcType>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ClearPendingInterrupt(uint32 vIsrSrc, uint32 vIsrSrcType)
{
    (void)vIsrSrcType;
    VAR(Os_ArchMsrType, OS_VAR) state;
    Os_ArchSuspendInt(&state);
    OS_INTERRUPT_CLEARREQ(vIsrSrc); /* PRQA S 0303 */ /* MISRA Rule 11.4 */
    Os_ArchRestoreInt(state);
}

/******************************************************************************/
/*
 * Brief                <Get software interrupt state.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Param-Name[in]       <vIsrSrc,vIsrSrcType>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <Os_IsrStateType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(Os_IsrStateType, OS_CODE) Os_GetIsrSourceState(uint32 vIsrSrc, uint32 vIsrSrcType)
{
    (void)vIsrSrcType;
    VAR(Os_IsrStateType, OS_VAR) isrSourceState;
    if (0U == OS_INTERRUPT_CHKREQ(vIsrSrc)) /* PRQA S 0303 */ /* MISRA Rule 11.4 */
    {
        isrSourceState = OS_ISR_ENABLED;
    }
    else
    {
        isrSourceState = OS_ISR_DISABLED;
    }
    return isrSourceState;
}
/* OS447: Disable all interrupts in os_app during TerminateApplication. */
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (CFG_ISR_MAX > 0)
/******************************************************************************/
/*
 * Brief                <Disable the interrupt source of the Isrs owned by the application>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <posCurAppCfg>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <TerminateApplication>
 * REQ ID               <DD_1_1076>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ArchDisableIntInApp(P2CONST(Os_ApplicationCfgType, AUTOMATIC, OS_VAR) posCurAppCfg)
{
    uint16 i;
    uint16 osIsrRefCnt;
    Os_IsrType osIsrId;
    uint32 osIsrRegVal;

    osIsrRefCnt = posCurAppCfg->OsAppIsrRefCnt;

    for (i = 0u; i < osIsrRefCnt; i++)
    {
        osIsrId = posCurAppCfg->OsAppObjectRef[OBJECT_ISR][i];
        osIsrRegVal = Os_IsrCfg[Os_GetObjLocalId(osIsrId)].OsIsrSrc;
        /* PRQA S 0303++ */ /* MISRA Rule 11.4 */
        OS_INTERRUPT_CLEARREQ(osIsrRegVal);
        /* PRQA S 0303-- */ /* MISRA Rule 11.4 */
    }
}

/* PRQA S 3006 ++ */ /* MISRA Dir 4.3 */
/******************************************************************************/
/*
 * Brief                <>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param[in]            <isrId>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <void>
 * CallByAPI            <>
 * REQ ID               <>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ArchReclaimIsrCsasTermApp(uint32 isrSP_Next, uint32 isrSP_Cur, uint32 isrId)
{
    (void)isrSP_Next;
    (void)isrSP_Cur;

    Os_IsrPriority = OS_INTC_SELF_EICn(Os_IsrCfg[isrId].OsIsrSrc) & 0x0FU;

    ASM("pushsp r6-r10");

    /* Clear the bit corresponding to Os_IsrPriority in ISPR  */
    ASM("mov _Os_IsrPriority, r10");
    ASM("ld.w 0[r10], r9"); /* r9 = Os_IsrPriority, e.g. 6 */
    ASM("mov 1, r8");       /* r8 = 1 */
    ASM("shl r9, r8");      /* r8 = r8 << r9, e.g. 1<<6=0x0040 */
    ASM("not r8, r8");      /* r8 = not(r8), e.g. 0xFFBF */
    ASM("stsr ispr, r7");   /* r7 = ispr */
    ASM("and r7, r8");      /* r7 = ispr & 0xFFBF */

    /* INTCFG.Bit0(ISPC) = 1
     * The ISPR register is not automatically updated.
     * Updates triggered by the program (via execution of LDSR instruction) are performed
     */
    ASM("mov 0x01, r7");
    ASM("stsr intcfg, r6");
    ASM("or r7, r6");
    ASM("ldsr r6, intcfg");

    ASM("ldsr r8, ispr");

    /* INTCFG.Bit0(ISPC) = 0
     * The ISPR register is automatically updated,
     * Updates triggered by the program (via execution of LDSR instruction) are ignored
     */
    ASM("mov 0xFFFFFFFE, r7");
    ASM("stsr intcfg, r6");
    ASM("and r7, r6");
    ASM("ldsr r6, intcfg");

    ASM("popsp r6-r10");
}
#if (TRUE == CFG_INT_NEST_ENABLE)
/* PRQA S 3006 -- */ /* MISRA Dir 4.3 */
/******************************************************************************/
/*
 * Brief                <Terminate an interrupt.>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param[in]            <OsIsrID>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <StatusType>
 * CallByAPI            <Os_ArchAppTerminateIsrProc>
 * REQ ID               <DD_1_0901>
 */
/******************************************************************************/
static FUNC(StatusType, OS_CODE) Os_ArchTerminateOneNestIsr(Os_IsrType OsIsrID)
{
    uint8 i;
    StatusType Status = E_OK;

    for (i = 0u; i < Os_IntNestISR2; i++)
    {
        /* find out the Isr nested position and reclaim its context saving area */
        if (OsIsrID == Os_SCB.sysIsrNestQueue[i])
        {
            /* Reclaim csa list used by this isr. */
            /* if OsIsrChkindex == Os_IntNestISR2, the isr is the innermost_isr, can not reclaim csas here */
            Os_ArchReclaimIsrCsasTermApp(Os_IsrNestPcxStack[i + 1u], Os_IsrNestPcxStack[i], OsIsrID);

            break;
        }
    }

    /* Not find out the Isr Id in the IsrStack from the SCB, not include outermost_isr. */
    if (i >= Os_IntNestISR2)
    {
        Status = E_NOT_OK;
    }
    else
    {
        /* Moving the following nested ISR2s to shift one position left */
        while (i < (Os_IntNestISR2 - 1u))
        {
            Os_SCB.sysIsrNestQueue[i] = Os_SCB.sysIsrNestQueue[i + 1u];

            if (i < (Os_IntNestISR2 - 2u))
            {
                Os_IsrNestPcxStack[i + 1u] = Os_IsrNestPcxStack[i + 2u];
            }

            i++;
        }
    }

    return Status;
}

/* Terminate Isr2 platform process in TerminateApplication */
/******************************************************************************/
/*
 * Brief                <Terminate the ISR objects of the calling application>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <OsIsrID>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <Os_TerminateISRObjectHandler>
 * REQ ID               <DD_1_1077>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ArchAppTerminateIsrProc(Os_IsrType OsIsrID)
{
    uint32 ulRet = E_OK;

    /* Isr_nest process. */
    /* Just find out the nested corresponding ISR2s(excluding the running one) */
    if (OsIsrID != Os_SCB.sysRunningIsrCat2Id)
    {
        /* Terminate one isr */
        ulRet = Os_ArchTerminateOneNestIsr(OsIsrID);

        if ((uint32)E_OK == ulRet)
        {
            if (Os_IntNestISR2 > 0u)
            {
                Os_IntNestISR2--;
            }

            if (Os_SCB.sysDispatchLocker > 0u)
            {
                /*Os_SCB.sysDispatchLocker--*/
                Os_SCB.sysDispatchLocker = Os_SCB.sysDispatchLocker - 1u;
            }
        }
    }
}
#endif /* TRUE == CFG_INT_NEST_ENABLE */
#endif /* #if (CFG_ISR_MAX > 0) */
#endif /* #if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC)) */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/*=======[E N D   O F   F I L E]==============================================*/
