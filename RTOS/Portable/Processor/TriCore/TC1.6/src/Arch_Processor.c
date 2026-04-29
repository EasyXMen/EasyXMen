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
/*******************************************************************************
**                                                                            **
**  FILENAME    :  Arch_Processor.c                                           **
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

/*=======[I N C L U D E S]====================================================*/
#include "Os_Internal.h"

/*=======[M A C R O S]========================================================*/
#define PSW_IS_BIT (9u)
#define PSW_GW_MASK (0xFFFFFEFFu)

/*=======[E X T E R N A L   D A T A]==========================================*/
#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
volatile uint32 Os_LoopPcx;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
volatile uint32 Os_EndPcx;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
Os_ArchCsaType* Os_Csas; /* PRQA S 3432 */ /* VL_Os_3432 */
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
Os_TaskCBExtType* Os_TaskCBExt; /* PRQA S 3432 */ /* VL_Os_3432 */
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_16
#include "Os_MemMap.h"
uint16 Os_CfgCsaMax;
#define OS_STOP_SEC_VAR_CLONE_16
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
volatile uint32 Os_TempPcx;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
uint32 Os_NextPcx;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

/* PRQA S 1502 ++ */ /* VL_Os_1502 */
#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
volatile uint32 Os_TempPcxTermApp;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
volatile uint32 Os_LoopPcxTermApp;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
volatile uint32 Os_NextPcxTermApp;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
volatile uint32 Os_BeginPcxTermApp;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
/* PRQA S 1502 -- */
/*=======[E X T E R N A L   D A T A]==========================================*/
/* PRQA S 0791++ */ /* VL_Os_0791 */

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
void Os_ArchFirstEnterTask(void)
{
    /* PRQA S 0306 ++ */ /* VL_Os_0306 */
    /* PRQA S 3432 ++ */ /* VL_Os_3432*/
    Os_ArchCsaType* lowCsa;
    Os_ArchCsaType* upperCsa;
    /* PRQA S 3469 ++ */ /* VL_Os_3469*/
    Os_ArchCsaType* upperCsa1;

    lowCsa                                        = OS_PCX_TO_EA(OS_ARCH_MFCR(OS_REG_FCX));
    upperCsa                                      = OS_CSA_PREV(lowCsa);
    upperCsa1                                     = OS_CSA_PREV(upperCsa);
    Os_TaskCBExt[Os_SCB.sysRunningTaskID].pcx     = OS_ARCH_MFCR(OS_REG_FCX);
    Os_TaskCBExt[Os_SCB.sysRunningTaskID].pcxLast = OS_EA_TO_PCX(upperCsa1);
    OS_ARCH_MTCR(OS_REG_FCX, OS_EA_TO_PCX(OS_CSA_PREV(upperCsa1)));
    /* PRQA S 3432 --*/

    /* PRQA S 0305 ++ */           /* VL_Os_0305 */
    lowCsa->reg[0] |= ((1u << PCXI_PIE_BIT) | (1u << PCXI_UL_BIT)); /* PCXI */

    lowCsa->reg[1] = (uint32)Os_TaskCfg[Os_SCB.sysRunningTaskID].osTaskEntry; /* A11(RA) */

    upperCsa->reg[0] |= (1u << PCXI_UL_BIT);                                               /* PCXI */
    upperCsa->reg[1] = PSW_DEFAULT_VALUE | 0x1u;                                                /* PSW  */
    upperCsa->reg[2] = OS_ARCH_STACK_ALIGN((uint32)Os_SCB.sysRunningTCB->taskTop); /* A10(SP) */
    /* PRQA S 3469 -- */
    upperCsa->reg[3] = (uint32)Os_TaskErrBack; /* A11(RA) */
    /* PRQA S 0305 -- */
    upperCsa->reg[4]  = 0U; /* D8   */
    upperCsa->reg[5]  = 0U; /* D9   */
    upperCsa->reg[6]  = 0U; /* D10  */
    upperCsa->reg[7]  = 0U; /* D11  */
    upperCsa->reg[8]  = 0U; /* A12  */
    upperCsa->reg[9]  = 0U; /* A13  */
    upperCsa->reg[10] = 0U; /* A14  */
    upperCsa->reg[11] = 0U; /* A15  */
    upperCsa->reg[12] = 0U; /* D12  */
    upperCsa->reg[13] = 0U; /* D13  */
    upperCsa->reg[14] = 0U; /* D14  */
    upperCsa->reg[15] = 0U; /* D15  */

    upperCsa1->reg[0] = 0U;          /* PCXI */
    upperCsa1->reg[1] = PSW_DEFAULT_VALUE; /* PSW  */
    /* PRQA S 0306 -- */
}
#else
/* PRQA S 1532 ++ */                               /* VL_QAC_OneFunRef */
void Os_ArchFirstEnterTask(void) /* PRQA S 3006 */ /* VL_Os_3006 */
/* PRQA S 1532 -- */
{
    /* PRQA S 0306 ++ */ /* VL_Os_0306 */
    /* PRQA S 3432 ++ */ /* VL_Os_3432*/
    /* PRQA S 3469 ++*/  /*VL_Os_3469*/
    Os_ArchCsaType* lowcsa_1  = OS_PCX_TO_EA(OS_ARCH_MFCR(OS_REG_FCX));
    Os_ArchCsaType* lowCsa    = OS_CSA_PREV(lowcsa_1);
    Os_ArchCsaType* upperCsa  = OS_CSA_PREV(lowCsa);
    Os_ArchCsaType* upperCsa1 = OS_CSA_PREV(upperCsa);

    Os_TaskCBExt[Os_SCB.sysRunningTaskID].pcx     = OS_ARCH_MFCR(OS_REG_FCX);
    Os_TaskCBExt[Os_SCB.sysRunningTaskID].pcxLast = OS_EA_TO_PCX(upperCsa1);
    /* PRQA S 3138, 1006 ++ */ /* VL_Os_3138, VL_Os_1006 */
    OS_ARCH_MTCR(OS_REG_FCX, OS_EA_TO_PCX(OS_CSA_PREV(upperCsa1)));
    /* PRQA S 3138, 1006 --*/
    /* PRQA S 3469 --*/
    /* PRQA S 3432 --*/

    /* PRQA S 0305 ++ */ /* VL_Os_0305 */
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    lowcsa_1->reg[0] |= ((1u << PCXI_PIE_BIT) | (1u << PCXI_UL_BIT));
    lowcsa_1->reg[1] = (uint32)Os_ModeModify; /* PRQA S 0428 */ /* VL_Os_0428 */

    lowCsa->reg[0] |= (1u << PCXI_UL_BIT); /* PCXI */
    lowCsa->reg[1] = PSW_DEFAULT_VALUE | 0x1u;  /* PSW  */

    lowCsa->reg[2] = (uint32)Os_SCB.sysRunningTCB->taskTop;                   /* A10(SP)  */
    lowCsa->reg[3] = (uint32)Os_TaskCfg[Os_SCB.sysRunningTaskID].osTaskEntry; /* A11(RA) */

    upperCsa->reg[0] |= (1u << PCXI_UL_BIT); /* PCXI */
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
    upperCsa->reg[1] = PSW_DEFAULT_VALUE | 0x1u | Os_PSW_PRS; /* PSW  */
#else
    upperCsa->reg[1] = PSW_DEFAULT_VALUE | 0x1u; /* PSW  */
#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */
    /* PRQA S 3469 ++*/                                                            /* VL_Os_3469 */
    upperCsa->reg[2] = OS_ARCH_STACK_ALIGN((uint32)Os_SCB.sysRunningTCB->taskTop); /* A10(SP)  */
    /* PRQA S 3469 --*/
    upperCsa->reg[3]  = (uint32)Os_TaskErrBack; /* PRQA S 0428 */ /* VL_Os_0428 */ /* A11(RA)  */
    upperCsa->reg[4]  = 0U;                                                        /* D8   */
    upperCsa->reg[5]  = 0U;                                                        /* D9   */
    upperCsa->reg[6]  = 0U;                                                        /* D10  */
    upperCsa->reg[7]  = 0U;                                                        /* D11  */
    upperCsa->reg[8]  = 0U;                                                        /* A12  */
    upperCsa->reg[9]  = 0U;                                                        /* A13  */
    upperCsa->reg[10] = 0U;                                                        /* A14  */
    upperCsa->reg[11] = 0U;                                                        /* A15  */
    upperCsa->reg[12] = 0U;                                                        /* D12  */
    upperCsa->reg[13] = 0U;                                                        /* D13  */
    upperCsa->reg[14] = 0U;                                                        /* D14  */
    upperCsa->reg[15] = 0U;                                                        /* D15  */

    upperCsa1->reg[0] = 0U; /* PCXI */
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
    upperCsa1->reg[1] = (PSW_DEFAULT_VALUE & PSW_GW_MASK) | (1u << PSW_IS_BIT) | 0x1u; /* PSW  */
    /* PRQA S 3469 ++*/              /* VL_Os_3469 */
    upperCsa1->reg[2] = OS_ARCH_STACK_ALIGN((uint32)Os_SystemStack->stackTop);
/* PRQA S 3120 --*/
/* PRQA S 3469 --*/
#else
    upperCsa1->reg[1] = PSW_DEFAULT_VALUE; /* PSW  */
#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */
    /* PRQA S 0305 -- */
    /* PRQA S 0306 -- */
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
/* PRQA S 6030, 1532++*/                           /* VL_MTR_Os_STMIF, VL_QAC_OneFunRef */
void Os_ArchStartScheduler(void) /* PRQA S 3006 */ /* VL_Os_3006 */
/* PRQA S 6030, 1532--*/
{
    uint32 temp = 0u;
    Os_SwitchTask();

    /*clear CDC*/
    temp = (Os_ArchMsrType)OS_ARCH_MFCR(OS_REG_PSW);
    temp &= CDC_MASK; /* PRQA S 3120 */            /* VL_QAC_MagicNum */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ARCH_MTCR(OS_REG_PSW, temp); /* PRQA S 3138 */ /* VL_Os_3138 */
    /* PRQA S 0306, 0310, 2481, 3345 ++ */            /* VL_Os_0306, VL_Os_0310, VL_Os_2481, VL_Os_3345 */
    /* PRQA S 0404, 3442, 2743 ++ */                  /* VL_Os_0404, VL_Os_3442, VL_Os_2743 */
    OS_ARCH_RESUME_CONTEXT();/* PRQA S 3415 */        /* VL_Os_3415 */
    /* PRQA S 0404, 3442, 2743 -- */
    /* PRQA S 0306, 0310, 2481, 3345 -- */
    /* PRQA S 1006 -- */
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
/* PRQA S 6030, 1503, 1512 ++*/                      /* VL_MTR_Os_STMIF, VL_Os_1503, VL_Os_1512 */
void Os_ArchDispatcher(void) /* PRQA S 3006, 3408 */ /* VL_Os_3006, VL_Os_3408 */
/* PRQA S 6030, 1503, 1512 --*/
{
    OS_ARCH_SAVE_CONTEXT(); /* PRQA S 3469, 2743 */ /* VL_Os_3469, VL_Os_2743 */

    /* PRQA S 0306++ */                             /* VL_Os_0306 */
    /* PRQA S 1006 ++ */                            /* VL_Os_1006 */
    OS_ARCH_MTCR(OS_REG_PCX, 0U); /* PRQA S 3138 */ /* VL_Os_3138 */
    /* avoid switch to task self */
    if ((TASK_STATE_SUSPENDED == Os_SCB.sysRunningTCB->taskState)
        || (TASK_STATE_START == Os_SCB.sysRunningTCB->taskState))
    {
        Os_ArchReclaimCsas(Os_SCB.sysRunningTaskID); /* PRQA S 2743 */ /* VL_Os_2743 */
    }
    Os_ArchSwitch2System(); /* PRQA S 0318, 3138 */ /* VL_Os_0318, VL_Os_3138 */
    Os_SwitchTask();
    /* PRQA S 0306, 0310, 2481, 3345 ++ */ /* VL_Os_0306, VL_Os_0310, VL_Os_2481, VL_Os_3345 */
    /* PRQA S 0404, 3442, 2743 ++ */       /* VL_Os_0404, VL_Os_3442, VL_Os_2743 */
    OS_ARCH_RESUME_CONTEXT();/* PRQA S 3415 */ /* VL_Os_3415 */
    /* PRQA S 0404, 3442, 2743 -- */
    /* PRQA S 0306, 0310, 2481, 3345 -- */
    /* PRQA S 1006 -- */
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
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_ArchInitCsas(void)
/* PRQA S 1532 -- */
{
    uint32 i = 0U;

    Os_Csas[0].reg[0] = 0U;
    Os_Csas[0].reg[1] = 0U;

    for (i = 1U; i < Os_CfgCsaMax; i++)
    {
        /* PRQA S 0306 ++ */ /* VL_Os_0306 */
        Os_Csas[i].reg[0] = OS_EA_TO_PCX((uint32)&Os_Csas[i - 1u]);
        /* PRQA S 0306 -- */
        Os_Csas[i].reg[1] = 0U;
    }
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
/* PRQA S 1532 ++ */                                                     /* VL_QAC_OneFunRef */
void Os_ArchAppTerminateTaskProc(Os_TaskType OsTaskID) /* PRQA S 3006 */ /* VL_Os_3006 */
/* PRQA S 1532 -- */
{
    /* PRQA S 0306++ */ /* VL_Os_0306 */
    if ((OS_LEVEL_TASK == Os_SCB.sysOsLevel)
        || ((OS_LEVEL_ERRORHOOK_APP == Os_SCB.sysOsLevel) && (TRUE != Os_SCB.sysInIsrCat2)))
    {
        /* If TerminateApplication is called by task or isr2 or app_error_hook,
         * the csa of running task is reclaimed in the dispatcher at the end of TerminateApplication. */
        if (OsTaskID != Os_SCB.sysRunningTaskID)
        {
            /* PRQA S 1006 ++ */ /* VL_Os_1006 */
            Os_ArchReclaimCsas(OsTaskID); /* PRQA S 2743 */ /* VL_Os_2743 */
            /* PRQA S 1006 -- */
        }
    }
    else
    {
        /* PRQA S 1006 ++ */ /* VL_Os_1006 */
        Os_ArchReclaimCsas(OsTaskID); /* PRQA S 2743 */ /* VL_Os_2743 */
        /* PRQA S 1006 -- */
    }
    /* PRQA S 0306-- */

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
/* PRQA S 3206, 3008 ++ */ /* VL_Os_3206, VL_Os_3008 */
uint32 Os_Extru(uint32 a, uint32 p, uint32 w)
/* PRQA S 3206, 3008 -- */
{
    uint32 res;
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ASM volatile(
        "mov %%d14,%2  \n\
                   mov %%d15,%3  \n\
                   extr.u %0,%1,%%e14" : "=d"(res) : "d"(a),
        "d"(p),
        "d"(w) : "d14",
        "d15");
    /* PRQA S 1006 -- */
    return res; /* PRQA S 2961 */ /* VL_Os_2961 */
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
/* PRQA S 3206, 3006 ++ */ /* VL_Os_3206, VL_Os_3006 */
uint32 Os_CmpSwapW(uint32* address, uint32 compareVal, uint32 exchangedVal)
/* PRQA S 3206, 3006 -- */
{
    uint64 tmp = (uint64)exchangedVal | ((uint64)((uint64)compareVal << 32u)); /* PRQA S 3120 */ /* VL_QAC_MagicNum */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ASM volatile("cmpswap.w [%[addr]]0, %A[reg]" : [reg] "+d"(tmp) : [addr] "a"(address) : "memory");
    /* PRQA S 1006 -- */
    tmp = ((uint32)tmp == compareVal) ? 0 : 1; /* PRQA S 4434 */ /* VL_Os_4434 */
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
/* PRQA S 1503 ++ */ /* VL_Os_1503 */
osCsaUsageType OSGetSystemCsaUsage(void)
/* PRQA S 1503 -- */
{
    uint16         i        = 0U;
    osCsaUsageType maxUsage = 0U;

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
/* PRQA S 0791 -- */
/*=======[E N D   O F   F I L E]==============================================*/
