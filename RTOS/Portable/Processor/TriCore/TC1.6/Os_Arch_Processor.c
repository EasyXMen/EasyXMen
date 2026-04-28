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
 **  @file               : Os_Arch_Processor.c
 **  @author             : i-soft-os
 **  @date               : 2025/02/10
 **  @vendor             : isoft
 **  @description        : Arch source file for Processor API implementations
 **
 ***********************************************************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Os_Arch_Processor.h"
#include "Os_Arch_Irq.h"
#include "Os_Task.h"
#include "Os_Appl.h"
#include "Os_Interrupt.h"
#include "Os_Mprot.h"
#include "Os_Sprot.h"

/*=======[M A C R O S]========================================================*/
#define PSW_IS_BIT (9u)
#define PSW_GW_MASK (0xFFFFFEFFu)

/*=======[E X T E R N A L   D A T A]==========================================*/
/* PRQA S 0791 ++ */ /* VL_Os_0791 */
#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
Os_TaskCBExtType* Os_TaskCBExt;
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
Os_ArchCsaType* Os_Csas; /* PRQA S 3432 */ /* VL_Os_3432 */
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_16
#include "Os_MemMap.h"
uint16 Os_CfgCsaMax;
#define OS_STOP_SEC_VAR_CLONE_16
#include "Os_MemMap.h"

/* PRQA S 1514, 1502, 1533 ++ */ /* VL_Os_1514, VL_Os_1502, VL_QAC_OneRefSymbol */
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
/* PRQA S 1514, 1502, 1533 -- */
/*=======[I N T E R N A L   D A T A]==========================================*/

/* PRQA S 0791 -- */
/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
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
#if (TRUE != CFG_MEMORY_PROTECTION_ENABLE)                       // NOSC or SC1 or SC2
/* PRQA S 1532,3006 ++ */ /* VL_QAC_OneFunRef,VL_Os_3006 */
void Os_Hal_FirstEnterTask(Os_TaskType taskId)
/* PRQA S 1532,3006 -- */
{
    /* PRQA S 0306 ++ */ /* VL_Os_0306 */
    /* PRQA S 3432 ++ */ /* VL_Os_3432 */
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    Os_ArchCsaType* lowCsa;
    Os_ArchCsaType* upperCsa;
    Os_ArchCsaType* upperCsa1;

    lowCsa                       = OS_PCX_TO_EA(OS_ARCH_MFCR(OS_REG_FCX));
    upperCsa                     = OS_CSA_PREV(lowCsa);
    upperCsa1                    = OS_CSA_PREV(upperCsa);
    Os_TaskCBExt[taskId].pcx     = OS_ARCH_MFCR(OS_REG_FCX);
    Os_TaskCBExt[taskId].pcxLast = OS_EA_TO_PCX(upperCsa1);
    /* PRQA S 1006, 3138 ++ */ /* VL_Os_1006, VL_Os_3138 */
    OS_ARCH_MTCR(OS_REG_FCX, OS_EA_TO_PCX(OS_CSA_PREV(upperCsa1)));
    /* PRQA S 1006, 3138 -- */
    /* PRQA S 3432 --*/

    /* PRQA S 0305, 0499 ++ */                                   /* VL_Os_0305, VL_Os_0499 */
    lowCsa->reg[0] |= ((1u << PCXI_PIE_BIT) | (1u << PCXI_UL_BIT)); /* PRQA S 4397 */ /* VL_Os_4397 */ /* PCXI */
    lowCsa->reg[1] = (uint32)Os_TaskCfg[taskId].TaskEntry;                                             /* A11(RA) */

    upperCsa->reg[0] |= (1u << PCXI_UL_BIT); /* PRQA S 4397 */ /* VL_Os_4397 */            /* PCXI */
    upperCsa->reg[1] = PSW_DEFAULT_VALUE | 0x1u;                                           /* PSW  */
    upperCsa->reg[2] = OS_ARCH_STACK_ALIGN((uint32)Os_TaskCfg[taskId].TaskStack.StackTop); /* A10(SP) */
    upperCsa->reg[3] = (uint32)Os_TaskErrBack; /* PRQA S 0428*/ /* VL_Os_0428 */           /* A11(RA) */
    /* PRQA S 0305, 0499 -- */
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
void Os_Hal_FirstEnterTask(Os_TaskType taskId) /* PRQA S 1532, 3006 */ /* VL_QAC_OneFunRef, VL_Os_3006 */
{
    /* PRQA S 0306 ++ */ /* VL_Os_0306 */
    /* PRQA S 0499, 4397 ++ */ /* VL_Os_0499, VL_Os_4397 */
    /* PRQA S 3432 ++ */ /* VL_Os_3432 */
    Os_ArchCsaType* lowcsa_1  = OS_PCX_TO_EA(OS_ARCH_MFCR(OS_REG_FCX));
    Os_ArchCsaType* lowCsa    = OS_CSA_PREV(lowcsa_1);
    Os_ArchCsaType* upperCsa  = OS_CSA_PREV(lowCsa);
    Os_ArchCsaType* upperCsa1 = OS_CSA_PREV(upperCsa);

    Os_TaskCBExt[taskId].pcx     = OS_ARCH_MFCR(OS_REG_FCX);
    Os_TaskCBExt[taskId].pcxLast = OS_EA_TO_PCX(upperCsa1);
    OS_ARCH_MTCR(OS_REG_FCX, OS_EA_TO_PCX(OS_CSA_PREV(upperCsa1))); /* PRQA S 3138, 1006 */ /* VL_Os_3138 */ /* VL_Os_1006 */
    /* PRQA S 3432 --*/

    /* PRQA S 0305 ++ */ /* VL_Os_0305 */
    /* PRQA S 1252 ++ */ /* VL_Os_1252 */
    lowcsa_1->reg[0] |= ((1u << PCXI_PIE_BIT) | (1u << PCXI_UL_BIT));
    lowcsa_1->reg[1] = (uint32)Os_ModeModify; /* PRQA S 0428*/ /* VL_Os_0428 */

    lowCsa->reg[0] |= (1u << PCXI_UL_BIT); /* PCXI */
    lowCsa->reg[1] = PSW_DEFAULT_VALUE | 0x1u;  /* PSW  */

    lowCsa->reg[2] = (uint32)Os_TaskCfg[taskId].TaskStack.StackTop; /* A10(SP)  */
    lowCsa->reg[3] = (uint32)Os_TaskCfg[taskId].TaskEntry;          /* A11(RA) */

    upperCsa->reg[0] |= (1u << PCXI_UL_BIT); /* PCXI */
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
    upperCsa->reg[1] = PSW_DEFAULT_VALUE | 0x1u | Os_PSW_PRS; /* PSW  */
#else
    upperCsa->reg[1] = PSW_DEFAULT_VALUE | 0x1u; /* PSW  */
#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */

    upperCsa->reg[2]  = OS_ARCH_STACK_ALIGN((uint32)Os_TaskCfg[taskId].TaskStack.StackTop); /* A10(SP)  */
    upperCsa->reg[3]  = (uint32)Os_TaskErrBack; /* A11(RA)  */ /* PRQA S 0428 */            /* VL_Os_0428 */
    upperCsa->reg[4]  = 0U;                                                                 /* D8   */
    upperCsa->reg[5]  = 0U;                                                                 /* D9   */
    upperCsa->reg[6]  = 0U;                                                                 /* D10  */
    upperCsa->reg[7]  = 0U;                                                                 /* D11  */
    upperCsa->reg[8]  = 0U;                                                                 /* A12  */
    upperCsa->reg[9]  = 0U;                                                                 /* A13  */
    upperCsa->reg[10] = 0U;                                                                 /* A14  */
    upperCsa->reg[11] = 0U;                                                                 /* A15  */
    upperCsa->reg[12] = 0U;                                                                 /* D12  */
    upperCsa->reg[13] = 0U;                                                                 /* D13  */
    upperCsa->reg[14] = 0U;                                                                 /* D14  */
    upperCsa->reg[15] = 0U;                                                                 /* D15  */

    upperCsa1->reg[0] = 0U; /* PCXI */
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
    upperCsa1->reg[1] = (PSW_DEFAULT_VALUE & PSW_GW_MASK) | (1u << PSW_IS_BIT) | 0x1u;                                                              /* PSW  */
    upperCsa1->reg[2] = OS_ARCH_STACK_ALIGN((uint32)Os_SystemStack_Inf[Os_McuGetCoreID()]->StackTop); /* PRQA S 3455 */ /* VL_Os_3455 */
#else
    upperCsa1->reg[1] = PSW_DEFAULT_VALUE; /* PSW  */
#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */
    /* PRQA S 0499, 4397 -- */ 
    /* PRQA S 0305 -- */
    /* PRQA S 0306 -- */
    /* PRQA S 3120 -- */
    /* PRQA S 1252 -- */
}
#endif /* TRUE != CFG_MEMORY_PROTECTION_ENABLE */

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
/* PRQA S 3006, 1532, 6030 ++ */ /* VL_Os_3006, VL_QAC_OneFunRef, VL_MTR_Os_STMIF */
void Os_Hal_StartScheduler(void)
/* PRQA S 3006, 1532, 6030 -- */
{
    Os_CoreIdType coreId = Os_McuGetCoreID();
    uint32 temp = 0u;

    Os_SwitchTask(coreId);

    /*clear CDC*/
    temp = (Os_ArchMsrType)OS_ARCH_MFCR(OS_REG_PSW);
    temp &= CDC_MASK; /* PRQA S 3120, 1252 */ /* VL_QAC_MagicNum */ /* VL_Os_1252 */
    OS_ARCH_MTCR(OS_REG_PSW, temp) /* PRQA S 1006 */                   /* VL_Os_1006 */

        /* PRQA S 0310, 0306, 3455 ++ *//* VL_Os_0310, VL_Os_0306, VL_Os_3455 */
        /* PRQA S 3138, 3442, 2743 ++ *//* VL_Os_3138, VL_Os_3442, VL_Os_2743 */
        /* PRQA S 2995, 1006 ++ */ /* VL_Os_2995, VL_Os_1006 */
        OS_ARCH_RESUME_CONTEXT(Os_Kernel_GetSysRunningTaskID());
        /* PRQA S 2995, 1006 -- */
        /* PRQA S 0310, 0306, 3455 -- */
        /* PRQA S 3138, 3442, 2743 -- */
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
void Os_ArchInitCsas(Os_ArchCsaType* csas, uint16 cfgCsaMax) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    uint32 i = 0U;

    csas[0].reg[0] = 0U;
    csas[0].reg[1] = 0U;

    for (i = 1U; i < cfgCsaMax; i++)
    {
        csas[i].reg[0] = OS_EA_TO_PCX((uint32)&csas[i - 1u]); /* PRQA S 0306, 1252 */ /* VL_Os_0306, VL_Os_1252 */
        csas[i].reg[1] = 0U;
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/* OS447: Disable all interrupts in os_app during TerminateApplication. */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <OS_ShutdownAllCores is not provide user>
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
uint32 Os_Hal_CmpSwapW(uint32* address, uint32  compareVal, uint32  exchangedVal) /* PRQA S 1532, 3206, 3006 */ /* VL_QAC_OneFunRef, VL_Os_3206, VL_Os_3006 */
{
    /* PRQA S 3432, 1006, 3138 ++ */ /* VL_Os_3432, VL_Os_1006, VL_Os_3138 */
    OS_ARCH_CMPSWAP(tmp, address, exchangedVal, compareVal);
    /* PRQA S 3432, 1006, 3138 -- */
    return (uint32)tmp;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/*=======[E N D   O F   F I L E]==============================================*/
