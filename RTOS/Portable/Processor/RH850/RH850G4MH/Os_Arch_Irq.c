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
 **  FILENAME    : Os_Arch_Irq.c                                               **
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
#include "Os_Arch_Processor.h"
#include "Os_Interrupt_CfgData.h"

/*=======[M A C R O S]========================================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/
#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
/*Holds the entry address of an FE interrupt.*/
Os_isrhnd Os_FE_Handler[OS_ARCH_FE_INTR_NUM]; /* PRQA S 3672 */  /* VL_Os_3672 */
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
/*Holds the entry address of an EI interrupt.*/
Os_isrhnd Os_Vector_Handler[OS_ARCH_EI_INTR_NUM]; /* PRQA S 3672 */  /* VL_Os_3672 */
/*The top of the stack that holds the FE level interrupt.*/
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
/**When an interrupt occurs, the stacked SP is stored in Os_ArchTempSp, 
 * and Os_ArchTempSp is stored in this parameter.
 */
uint32 Os_IsrNestStack[CFG_ISR_MAX];
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
/*Save the ISR1 context stack pointer address.*/
uint32 Os_ISR1SP[OS_ISR1_MAX_NESTED_DEPTH]; /* PRQA S 1502, 1751 */  /* VL_QAC_OneRefSymbol , VL_Os_1502 */
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

/*=======[I N T E R N A L   D A T A]==========================================*/
#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
static volatile uint32 Os_IsrPriority; /* PRQA S 3218, 3229 */ /* MISRA Rule 8.9, VL_Os_3229 */
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#define OS_START_SEC_CODE_FAST
#include "Os_MemMap.h"

static void Os_IntHandler(void);
static void Os_IntHandler(void)
{
    /* Just ignore this interrupt.  */
    while (1) /* PRQA S 2870, 2740 */ /* VL_Os_2870, VL_Os_2740 */
    {
        /* Nothing to do. */
    }
}
#define OS_STOP_SEC_CODE_FAST
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Initialize interrupts>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <Os_Hal_InitCPU>
 * REQ ID               <DD_1_0902, DD_1_1298>
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_InterruptInit(void)
/* PRQA S 1532 -- */ 
{
    uint32        isrNum;
    
    /* Set exception handler vector address */
    OS_ARCH_REG_WRITE(OS_EBASE_NUM, (uint32)&Os_ExceptionTable); /* PRQA S 0305,0428 */ /* VL_Os_0305, VL_Os_0428 */
    OS_ARCH_REG_WRITE(OS_PSW_NUM, OS_ARCH_REG_READ(OS_PSW_NUM) | (1UL << OS_PSW_EBV_BIT));

    /* PRQA S 0303-- */ /* MISRA Rule 11.4 */
    /*Initialize the interrupt handlers.*/
    for (isrNum = 0U; isrNum < OS_ARCH_EI_INTR_NUM; ++isrNum)
    {
        Os_Vector_Handler[isrNum] = (Os_isrhnd)&Os_IntHandler;
    }

    for (isrNum = 0U; isrNum < OS_ARCH_FE_INTR_NUM; ++isrNum)
    {
        Os_FE_Handler[isrNum] = &Os_FEIntHandler;
    }
}
#define OS_STOP_SEC_CODE
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
void Os_Hal_SetIpl(Os_IPLType ipl, Os_IsrDescriptionType isrdesc)
{
    (void)isrdesc;
    Os_IPLType cpuPrio = (Os_IPLType)ipl;

    if (INT_PRIORITY_MAX <= cpuPrio)
    {
        cpuPrio = 0U;
    }
    else
    {
        cpuPrio = INT_PRIORITY_MAX - cpuPrio;
    }

    /*All interrupts with priority Numbers greater than or equal to
     * 'PMR' are turned off*/
    OS_ARCH_REG_WRITE(OS_PLMR_NUM, cpuPrio);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
Os_IPLType Os_Hal_GetIpl(void)
{
    Os_IPLType result = 0U;

    uint32 prio = OS_ARCH_REG_READ(OS_PLMR_NUM) & 0x3FU; /* read interrupt priority masking, 0-15 */ /* PRQA S 3120 */ /* VL_QAC_MagicNum*/
    if (0U != prio)
    {
        result = INT_PRIORITY_LOG_TO_PHY(prio);
    }
    else
    {
        result = INT_PRIORITY_MAX;
    }

    return (result);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Save the status register and disable maskable interrupts.>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <Os_ArchMsrType>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <SuspendAllInterrupts>
 */
/******************************************************************************/
Os_ArchMsrType Os_Hal_SuspendInt(void)
{
    uint32 psw = OS_ARCH_REG_READ(OS_PSW_NUM);
    Os_ArchMsrType id  = psw & OS_PSW_GET_ID_BIT;

    if (0U == id)
    {
        Os_Hal_DisableInt();
    }
    return id;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
void Os_Hal_RestoreInt(Os_ArchMsrType msr)
{
    if (0U == msr)
    {
        Os_Hal_EnableInt();
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
void Os_Hal_EnableInterruptSource(uint32 vIsrSrc, uint32 vIsrSrcType)
{
    Os_ArchMsrType state;

    state = Os_Hal_SuspendInt();
    if (0U != (vIsrSrc & OS_EIINT_TYPE))
    {
        OS_INTC_EICn(OS_EIINTSRC_2_NUM(vIsrSrc)) &= (uint16)(~((uint16)((uint16)1U << OS_EIC_MK_BIT))); /* PRQA S 3442, 0303 */ /* VL_Os_3442, VL_Os_0303 */
    }
    else
    {
        OS_FEINTMSK(vIsrSrcType) &= ~(uint32)(1UL << OS_FEINTSRC_2_NUM(vIsrSrc)); /* PRQA S 3442, 0303 */ /* VL_Os_3442, VL_Os_0303 */
    }
    Os_Hal_RestoreInt(state);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
void Os_Hal_DisableInterruptSource(uint32 vIsrSrc, uint32 vIsrSrcType)
{
    Os_ArchMsrType state;

    state = Os_Hal_SuspendInt();
    if (0U != (vIsrSrc & OS_EIINT_TYPE))
    {
        OS_INTC_EICn(OS_EIINTSRC_2_NUM(vIsrSrc)) |= (uint16)((uint16)1U << OS_EIC_MK_BIT); /* PRQA S 3442, 0303 */ /* VL_Os_3442, VL_Os_0303 */
    }
    else
    {
        OS_FEINTMSK(vIsrSrcType) |= (uint32)(1UL << OS_FEINTSRC_2_NUM(vIsrSrc)); /* PRQA S 3442, 0303 */ /* VL_Os_3442, VL_Os_0303 */
    }
    Os_Hal_RestoreInt(state);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
void Os_Hal_ClearPendingInterrupt(uint32 vIsrSrc, uint32 vIsrSrcType)
{
    Os_ArchMsrType state;

    state = Os_Hal_SuspendInt();
    if (0U != (vIsrSrc & OS_EIINT_TYPE))
    {
        OS_INTC_EICn(OS_EIINTSRC_2_NUM(vIsrSrc)) &= (uint16)(~((uint16)((uint16)1U << OS_EIC_RF_BIT))); /* PRQA S 3442, 0303 */ /* VL_Os_3442, VL_Os_0303 */
    }
    else
    {
        OS_FEINTC(vIsrSrcType) |= (uint32)(1UL << OS_FEINTSRC_2_NUM(vIsrSrc)); /* PRQA S 3442, 0303 */ /* VL_Os_3442, VL_Os_0303 */
    }
    Os_Hal_RestoreInt(state);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Set software interrupt source.>
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
void Os_Hal_SetPendingInterrupt(uint32 vIsrSrc, uint32 vIsrSrcType)
{
    (void)vIsrSrcType;
    Os_ArchMsrType state;

    state = Os_Hal_SuspendInt();
    if (0U != (vIsrSrc & OS_EIINT_TYPE))
    {
        OS_INTC_EICn(OS_EIINTSRC_2_NUM(vIsrSrc)) |= (uint16)((uint16)1U << OS_EIC_RF_BIT); /* PRQA S 3442, 0303 */ /* VL_Os_3442, VL_Os_0303 */
    }
    Os_Hal_RestoreInt(state);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
Os_IsrStateType Os_Hal_GetIsrSourceState(uint32 vIsrSrc, uint32 vIsrSrcType)
{
    Os_IsrStateType isrSourceState = OS_ISR_DISABLED;

    if (0U != (vIsrSrc & OS_EIINT_TYPE))
    {
        if (0U == ((OS_INTC_EICn(OS_EIINTSRC_2_NUM(vIsrSrc)) >> OS_EIC_MK_BIT) & 1U)) /* PRQA S 3442, 0303 */ /* VL_Os_3442, VL_Os_0303 */
        {
            isrSourceState = OS_ISR_ENABLED;
        }
    }
    else
    {
        if (0U == (OS_FEINTMSK(vIsrSrcType) & (uint32)(1UL << OS_FEINTSRC_2_NUM(vIsrSrc)))) /* PRQA S 3442, 0303 */ /* VL_Os_3442, VL_Os_0303 */
        {
            isrSourceState = OS_ISR_ENABLED;
        }
    }

    return isrSourceState;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get the current running IRQ.>
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
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
uint32 Os_Hal_GetCurrentIrq(void)
/* PRQA S 1532 -- */
{
    uint32 eiic = OS_ARCH_REG_READ(OS_EIIC_NUM);
    return (eiic & OS_EIIC_GET_FEILD);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get the current running Exception.>
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
/* PRQA S 1503 ++ */                                /* VL_QAC_NoUsedApi */
uint32 Os_Hal_GetCurrentExc(void) /* PRQA S 1532 */ /* VL_Os_1532 */
/* PRQA S 1503 -- */
{
    uint32 eiic = OS_ARCH_REG_READ(OS_FEIC_NUM);
    return (eiic & OS_FEIC_GET_FEILD);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
/* PRQA S 3006 ++ */ /* MISRA Dir 4.3 */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
/* PRQA S 1006 ++ */ /* VL_Os_1006 */
void Os_ArchReclaimIsrCsasTermApp(uint32 isrId)
{
    uint32 isrNum  = OS_EIINTSRC_2_NUM(Os_IsrCfg[isrId].IsrSrc);
    uint16 eicValue = OS_INTC_EICn(isrNum);   /* PRQA S 0303 */ /* VL_Os_0303*/
    uint32 priority = (uint32)eicValue & (uint32)(OS_EIC_P_FEILD_MASK);  
    Os_IsrPriority = priority;

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
/* PRQA S 1006 -- */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
/* PRQA S 3006 -- */ /* MISRA Dir 4.3 */
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC */

/*=======[E N D   O F   F I L E]==============================================*/
