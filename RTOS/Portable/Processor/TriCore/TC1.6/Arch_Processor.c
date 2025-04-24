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
 */
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : Arch_Processor.c                                            **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : Processor Request Managment                                 **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Os_Internal.h"

/*=======[M A C R O S]========================================================*/
#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
VAR(volatile uint32, OS_VAR)
Os_LoopPcx;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
VAR(volatile uint32, OS_VAR)
Os_EndPcx;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
P2VAR(Os_ArchCsaType, AUTOMATIC, OS_VAR)
Os_Csas; /* PRQA S 3432 */ /* MISRA Rule 20.7 */ /* ARCH_PROCESSOR_COMPILER_002 */
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
P2VAR(Os_TaskCBExtType, AUTOMATIC, OS_VAR)
Os_TaskCBExt; /* PRQA S 3432 */ /* MISRA Rule 20.7 */ /* ARCH_PROCESSOR_COMPILER_002 */
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_16
#include "Os_MemMap.h"
VAR(uint16, OS_VAR)
Os_CfgCsaMax;
#define OS_STOP_SEC_VAR_CLONE_16
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
VAR(volatile uint32, OS_VAR)
Os_TempPcx;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
VAR(uint32, OS_VAR)
Os_NextPcx;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
VAR(volatile uint32, OS_VAR)
Os_TempPcxTermApp;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
VAR(volatile uint32, OS_VAR)
Os_LoopPcxTermApp;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
VAR(volatile uint32, OS_VAR)
Os_NextPcxTermApp;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
VAR(volatile uint32, OS_VAR)
Os_BeginPcxTermApp;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
/*=======[E X T E R N A L   D A T A]==========================================*/
/* PRQA S 0791++ */ /* MISRA Rule 5.4 */ /*ARCH_PROCESSOR_SEGMENTNAME_SIMILAR_001*/

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <first entry task>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_SwitchTask>
 * REQ ID               <None>
 */
/******************************************************************************/
#if ((OS_NOSC == CFG_SC) || (OS_SC1 == CFG_SC) || (OS_SC2 == CFG_SC))
FUNC(void, OS_CODE)
Os_ArchFirstEnterTask(void)
{
    /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */ /* ARCH_PROCESSOR_TYPE_CAST_003 */
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7*/  /* ARCH_PROCESSOR_COMPILER_002 */
    P2VAR(Os_ArchCsaType, AUTOMATIC, OS_VAR)
    lowCsa;
    P2VAR(Os_ArchCsaType, AUTOMATIC, OS_VAR)
    upperCsa;
    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9*/ /*ARCH_PROCESSOR_MACRO_001*/
    P2VAR(Os_ArchCsaType, AUTOMATIC, OS_VAR)
    upperCsa1;

    lowCsa = OS_PCX_TO_EA(OS_ARCH_MFCR(OS_REG_FCX));
    upperCsa = OS_CSA_PREV(lowCsa);
    upperCsa1 = OS_CSA_PREV(upperCsa);
    Os_TaskCBExt[Os_SCB.sysRunningTaskID].pcx = OS_ARCH_MFCR(OS_REG_FCX);
    Os_TaskCBExt[Os_SCB.sysRunningTaskID].pcxLast = OS_EA_TO_PCX(upperCsa1);
    OS_ARCH_MTCR(OS_REG_FCX, OS_EA_TO_PCX(OS_CSA_PREV(upperCsa1)));
    /* PRQA S 3432 --*/ /* MISRA Rule 20.7*/

    /* PRQA S 0305 ++ */ /* MISRA Rule 11.1 */ /* ARCH_PROCESSOR_TYPE_CAST_004 */
    lowCsa->reg[0] |= 0x00300000u;             /* PCXI */

    lowCsa->reg[1] = (uint32)Os_TaskCfg[Os_SCB.sysRunningTaskID].osTaskEntry; /* A11(RA) */

    upperCsa->reg[0] |= 0x00100000u;                                               /* PCXI */
    upperCsa->reg[1] = 0x00000981u;                                                /* PSW  */
    upperCsa->reg[2] = OS_ARCH_STACK_ALIGN((uint32)Os_SCB.sysRunningTCB->taskTop); /* A10(SP) */
    /* PRQA S 3469 -- */                                                           /* MISRA Dir 4.9*/
    upperCsa->reg[3] = (uint32)Os_TaskErrBack;                                     /* A11(RA) */
    /* PRQA S 0305 -- */                                                           /* MISRA Rule 11.1 */
    upperCsa->reg[4] = 0U;                                                         /* D8   */
    upperCsa->reg[5] = 0U;                                                         /* D9   */
    upperCsa->reg[6] = 0U;                                                         /* D10  */
    upperCsa->reg[7] = 0U;                                                         /* D11  */
    upperCsa->reg[8] = 0U;                                                         /* A12  */
    upperCsa->reg[9] = 0U;                                                         /* A13  */
    upperCsa->reg[10] = 0U;                                                        /* A14  */
    upperCsa->reg[11] = 0U;                                                        /* A15  */
    upperCsa->reg[12] = 0U;                                                        /* D12  */
    upperCsa->reg[13] = 0U;                                                        /* D13  */
    upperCsa->reg[14] = 0U;                                                        /* D14  */
    upperCsa->reg[15] = 0U;                                                        /* D15  */

    upperCsa1->reg[0] = 0U;          /* PCXI */
    upperCsa1->reg[1] = 0x00000980u; /* PSW  */
    /* PRQA S 0306 -- */             /* MISRA Rule 11.4 */
}
#else
FUNC(void, OS_CODE)
Os_ArchFirstEnterTask(void)
{
    /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */ /* ARCH_PROCESSOR_TYPE_CAST_003 */
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7*/  /* ARCH_PROCESSOR_COMPILER_002 */
    /* PRQA S 3469 ++*/ /*MISRA Dir 4.9*/      /*ARCH_PROCESSOR_MACRO_001*/
    P2VAR(Os_ArchCsaType, AUTOMATIC, OS_VAR)
    lowcsa_1 = OS_PCX_TO_EA(OS_ARCH_MFCR(OS_REG_FCX));
    P2VAR(Os_ArchCsaType, AUTOMATIC, OS_VAR)
    lowCsa = OS_CSA_PREV(lowcsa_1);
    P2VAR(Os_ArchCsaType, AUTOMATIC, OS_VAR)
    upperCsa = OS_CSA_PREV(lowCsa);
    P2VAR(Os_ArchCsaType, AUTOMATIC, OS_VAR)
    upperCsa1 = OS_CSA_PREV(upperCsa);

    Os_TaskCBExt[Os_SCB.sysRunningTaskID].pcx = OS_ARCH_MFCR(OS_REG_FCX);
    Os_TaskCBExt[Os_SCB.sysRunningTaskID].pcxLast = OS_EA_TO_PCX(upperCsa1);
    OS_ARCH_MTCR(OS_REG_FCX, OS_EA_TO_PCX(OS_CSA_PREV(upperCsa1)));
    /* PRQA S 3469 --*/ /*MISRA Dir 4.9*/
    /* PRQA S 3432 --*/ /* MISRA Rule 20.7*/

    /* PRQA S 0305 ++ */ /* MISRA Rule 11.1 */ /* ARCH_PROCESSOR_TYPE_CAST_004 */
    lowcsa_1->reg[0] |= 0x00300000u;
    lowcsa_1->reg[1] = (uint32)Os_ModeModify;

    lowCsa->reg[0] |= 0x00100000u; /* PCXI */
    lowCsa->reg[1] = 0x00000981u;  /* PSW  */

    lowCsa->reg[2] = (uint32)Os_SCB.sysRunningTCB->taskTop;                   /* A10(SP)  */
    lowCsa->reg[3] = (uint32)Os_TaskCfg[Os_SCB.sysRunningTaskID].osTaskEntry; /* A11(RA) */

    upperCsa->reg[0] |= 0x00100000u; /* PCXI */
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
    upperCsa->reg[1] = 0x00000981u | Os_PSW_PRS; /* PSW  */
#else
    upperCsa->reg[1] = 0x00000981u; /* PSW  */
#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */
    /* PRQA S 3469 ++*/ /* MISRA Dir 4.9 */                                        /*ARCH_PROCESSOR_MACRO_001*/
    upperCsa->reg[2] = OS_ARCH_STACK_ALIGN((uint32)Os_SCB.sysRunningTCB->taskTop); /* A10(SP)  */
    /* PRQA S 3469 --*/                                                            /* MISRA Dir 4.9 */
    upperCsa->reg[3] = (uint32)Os_TaskErrBack;                                     /* A11(RA)  */
    upperCsa->reg[4] = 0U;                                                         /* D8   */
    upperCsa->reg[5] = 0U;                                                         /* D9   */
    upperCsa->reg[6] = 0U;                                                         /* D10  */
    upperCsa->reg[7] = 0U;                                                         /* D11  */
    upperCsa->reg[8] = 0U;                                                         /* A12  */
    upperCsa->reg[9] = 0U;                                                         /* A13  */
    upperCsa->reg[10] = 0U;                                                        /* A14  */
    upperCsa->reg[11] = 0U;                                                        /* A15  */
    upperCsa->reg[12] = 0U;                                                        /* D12  */
    upperCsa->reg[13] = 0U;                                                        /* D13  */
    upperCsa->reg[14] = 0U;                                                        /* D14  */
    upperCsa->reg[15] = 0U;                                                        /* D15  */

    upperCsa1->reg[0] = 0U; /* PCXI */
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
    upperCsa1->reg[1] = 0x00000A81U;        /* PSW  */
    /* PRQA S 3469 ++*/ /* MISRA Dir 4.9 */ /*ARCH_PROCESSOR_MACRO_001*/
    upperCsa1->reg[2] = OS_ARCH_STACK_ALIGN((uint32)Os_SystemStack->stackTop);
/* PRQA S 3469 --*/ /* MISRA Dir 4.9 */
#else
    upperCsa1->reg[1] = 0x00000980u; /* PSW  */
#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */
    /* PRQA S 0305 -- */ /* MISRA Rule 11.1 */
    /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */
}
#endif /* OS_NOSC == CFG_SC || OS_SC1 == CFG_SC || OS_SC2 == CFG_SC */

#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Start scheduling to execute the first task.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <StartOS>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE)
Os_ArchStartScheduler(void)
{
    VAR(uint32, OS_VAR)
    temp = 0u;
    Os_SwitchTask();

    /*clear CDC*/
    temp = (Os_ArchMsrType)OS_ARCH_MFCR(OS_REG_PSW);
    temp &= 0XFFFFFF80u;
    OS_ARCH_MTCR(OS_REG_PSW, temp);
    OS_ARCH_RESUME_CONTEXT(); /* PRQA S 0306,0310 */ /* MISRA Rule 11.3,11.4 */ /* ARCH_PROCESSOR_TYPE_CAST_003 */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <switch task.>
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
FUNC(void, OS_CODE)
Os_ArchDispatcher(void)
{
    OS_ARCH_SAVE_CONTEXT(); /* PRQA S 3469 */ /* MISRA Dir 4.9 */ /*ARCH_TRAP_MACRO_001*/

    /* PRQA S 0306++ */ /* MISRA Rule 11.4 */ /*ARCH_TRAP_TYPE_CAST_002*/
    OS_ARCH_MTCR(OS_REG_PCX, 0U);
    /* avoid switch to task self */
    if ((TASK_STATE_SUSPENDED == Os_SCB.sysRunningTCB->taskState)
        || (TASK_STATE_START == Os_SCB.sysRunningTCB->taskState))
    {
        Os_ArchReclaimCsas(Os_SCB.sysRunningTaskID);
    }
    Os_ArchSwitch2System();
    Os_SwitchTask();
    OS_ARCH_RESUME_CONTEXT(); /* PRQA S 0310 */ /* MISRA Rule 11.3 */ /* ARCH_TRAP_TYPE_CAST_003 */
    /* PRQA S 0306-- */                                               /* MISRA Rule 11.4 */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Initialization of the CSA module of TriCore Architecture>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <StartOS>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE)
Os_ArchInitCsas(void)
{
    VAR(uint32, OS_VAR)
    i = 0U;

    Os_Csas[0].reg[0] = 0U;
    Os_Csas[0].reg[1] = 0U;

    for (i = 1U; i < Os_CfgCsaMax; i++)
    {
        Os_Csas[i].reg[0] = OS_EA_TO_PCX((uint32)&Os_Csas[i - 1u]);
        /* PRQA S 0306,3469 */ /* MISRA Rule 11.4 */ /* ARCH_PROCESSOR_TYPE_CAST_003 */
        Os_Csas[i].reg[1] = 0U;
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Initialization of the CPU in the OS.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <StartOS>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE)
Os_ArchInitCPU(void)
{
    /* FIXME:DELETE */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <TerminateTask arch_process in TerminateApplication>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <Os_TerminateTaskObjectHandler>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE)
Os_ArchAppTerminateTaskProc(Os_TaskType OsTaskID)
{
    /* PRQA S 0306++ */ /* MISRA Rule 11.4 */ /*ARCH_PROCESSOR_TYPE_CAST_003*/
    if ((OS_LEVEL_TASK == Os_SCB.sysOsLevel)
        || ((OS_LEVEL_ERRORHOOK_APP == Os_SCB.sysOsLevel) && (TRUE != Os_SCB.sysInIsrCat2)))
    {
        /* If TerminateApplication is called by task or isr2 or app_error_hook,
         * the csa of running task is reclaimed in the dispatcher at the end of TerminateApplication. */
        if (OsTaskID != Os_SCB.sysRunningTaskID)
        {
            Os_ArchReclaimCsas(OsTaskID);
        }
    }
    else
    {
        Os_ArchReclaimCsas(OsTaskID);
    }
    /* PRQA S 0306-- */ /* MISRA Rule 11.4 */

    return;
}

#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* OS_SC3 == CFG_SC || OS_SC4 == CFG_SC */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*=======[E X C I P T I O N]==================================================*/

/******************************************************************************/
/*
 * Brief                <Assembly shift operation.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <Gtm_GetTbuChannelStatus>
 * REQ ID               <None>
 */
/******************************************************************************/
uint32 Os_Extru(
    uint32 a,
    uint32 p,
    uint32 w) /* PRQA S 3206,3008 */ /* MISRA Rule 2.7,4.3 */ /*ARCH_PROCESSOR_C_INLINE_ASM_006*/
{
    VAR(uint32, OS_VAR)
    res;
    OS_ASM volatile("mov %%d14,%2  \n\
                   extr.u %0,%1,%%e14"
                    : "=d"(res)
                    : "d"(a), "d"(p), "d"(w)
                    : "d14", "d15");
    return res; /* PRQA S 2961 */ /* MISRA Rule 9.1 */ /*ARCH_PROCESSOR_C_INLINE_ASM_007*/
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <OS_ShutdownAllCores is not provide user CAS>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <GetSpinlock and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
uint32 Os_CmpSwapW(
    uint32* address,
    uint32 compareVal,
    uint32
        exchangedVal) /* PRQA S 1532,3206,3006 */ /* MISRA Rule 8.7,2.7,Dir 4.3 */ /*ARCH_PROCESSOR_C_INLINE_ASM_006*/
{
    VAR(uint64, OS_VAR)
    tmp = (uint64)exchangedVal | ((uint64)((uint64)compareVal << 32u));
    OS_ASM volatile("cmpswap.w [%[addr]]0, %A[reg]" : [reg] "+d"(tmp) : [addr] "a"(address) : "memory");
    tmp = ((uint32)tmp == compareVal) ? 0 : 1;
    return (uint32)tmp;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Get max usage of system csa>
 * Service ID           <none>
 * Sync/Async           <none>
 * Reentrancy           <Yes>
 * param-eventId[in]    <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <osCsaUsageType>
 * PreCondition         <None>
 * REQ ID               <>
 */
/********************************************************************/
FUNC(osCsaUsageType, OS_CODE)
OSGetSystemCsaUsage(void)
{
    VAR(uint16, OS_VAR)
    i = 0U;
    VAR(osCsaUsageType, OS_VAR)
    maxUsage = 0U;

    for (i = (Os_CfgCsaMax - 1U); i > 0U; i--)
    {
        if (0U != Os_Csas[i].reg[1])
        {
            maxUsage++;
        }
    }

    return maxUsage;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/*=======[E N D   O F   F I L E]==============================================*/
