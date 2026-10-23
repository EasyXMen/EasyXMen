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
**  SPECIFICATION(S) :   AUTOSAR classic Platform r23                         **
**  Version :   AUTOSAR classic Platform R23--Function Safety                 **
**                                                                            **
*******************************************************************************/
/*=======[I N C L U D E S]===================================================*/
#include "Os_Arch_Irq.h"
#include "Os_Arch_Define.h"
#include "Os_Arch_Timer.h"
#include "Os_Mcu_Config.h"
#include "Os_Core_CfgData.h"

/*=======[M A C R O S]=======================================================*/

/*=======[E X T E R N A L   D A T A]=========================================*/
#define OS_START_SEC_VAR_CLEARED_GLOBAL_32
#include "Os_MemMap.h"
/* Aligned 32 * (INTLINESNUM + 1) * 4 = bytes align */
/* PRQA S 3672,3218 ++ */ /* VL_Os_3672, VL_Os_3218 */
static OS_ALIGN(OS_MCU_NVIC_VECTOR_ALIGNE) Os_IsrHandler Os_ArchVectorTable[OS_MCU_NVIC_NUM + OS_NUM_1]; 
/* PRQA S 3672,3218 -- */
#define OS_STOP_SEC_VAR_CLEARED_GLOBAL_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
Os_IsrHandler Os_ArchExternIrqTable[OS_MCU_NVIC_NUM + OS_NUM_1]; /* PRQA S 3672,1514 */ /* VL_Os_3672, VL_Os_1514 */
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
uint32 Os_ArchMainSpNest[CFG_ISR_MAX]; /* PRQA S 1504 */ /* VL_QAC_OneRefSymbol */
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h" 

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
uint32 Os_ArchIsrxPSRNest[CFG_ISR_MAX]; /* PRQA S 1514 */ /* VL_Os_1514 */
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
uint32 Os_ArchIsrTempIPSR; /* PRQA S 1514 */ /* VL_Os_1514 */
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]===================*/

#define OS_START_SEC_CODE_FAST
#include "Os_MemMap.h"
static void Os_IntHandler(void) /* PRQA S 3450 */ /* VL_Os_3450 */
{
    /* Just ignore this interrupt.  */
    while(1) /* PRQA S 2740,2870 */  /* VL_Os_2740,VL_Os_2870 */
    {
    }
}
#define OS_STOP_SEC_CODE_FAST
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Save ccr to variable msr and disable maskable interrupt>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <SuspendAllInterrupts>
 */
/******************************************************************************/
Os_ArchMsrType Os_Hal_SuspendInt(void) /*PRQA S 3006,3432,3008*/ /*VL_Os_3006,VL_Os_3432,VL_Os_3008*/
{
    register Os_ArchMsrType result; /* PRQA S 2011 */ /* VL_Os_2011 */
    /*PRQA S 1006 ++ */ /*VL_Os_1006*/
    OS_ASM("MRS %0, primask" : "=r"(result));
    OS_ASM("MSR primask, %0" : : "r"(OS_1BIT_MASK));
    /*PRQA S 1006 -- */

    return result;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <transfer variable msr back to ccr>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ResumeAllInterrupts>
 */
/******************************************************************************/
void Os_Hal_RestoreInt(Os_ArchMsrType msr) /*PRQA S 3006*/ /* VL_Os_3006 */
{
    UNUSED_PARAMETER(msr);
    /*PRQA S 1006 ++ */ /*VL_Os_1006*/
    OS_ARCH_DSYNC();
    OS_ASM("MSR primask, %0 " : : "r"(msr));
    /*PRQA S 1006 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <Installation interrupted>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_Hal_InitCPU>
 */
/*****************************************************************************/
/*PRQA S 3006,3672 ++*/ /* VL_Os_3006, VL_Os_3672 */
static void Os_Arch_InterruptInstall(uint8 id, uint8 prio, Os_IsrHandler isrProc) /* PRQA S 3450 */ /* VL_Os_3450 */
{
    /* Set interrupt priority */
    if (id == OS_ISR_SysTick_IRQn)
    {
        /*PRQA S 0303,1258,3432,1277,4397 ++ */ /*VL_Os_0303,VL_Os_1258,VL_Os_3432,VL_Os_1277,VL_Os_4397*/
        OS_ARCH_REG_SCB_SHPR(id) = OS_ARCH_NVIC_CONVERT_SET_PRIO(prio);
       }
    else if (id >= OS_ARCH_ISR_EXTERNAL_NUM)
    {
        OS_ARCH_REG_NVIC_IPR(id) = OS_ARCH_NVIC_CONVERT_SET_PRIO(prio);
        /*PRQA S 0303,1258,3432,1277,4397  -- */
    }
    else
    {
        /* Nothing need to do */
    }

    Os_Hal_EnableInterruptSource(id, 0); /* PRQA S 1290 */ /* VL_Os_1290 */   

    /* Save the former handler pointer */
    if (isrProc != NULL_PTR)
    {
        /* Set handler into vector table */
        Os_ArchExternIrqTable[id] = isrProc;
    }
    /*PRQA S 1006 ++ */ /*VL_Os_1006*/
    OS_ASM("dsb");
    OS_ASM("isb");
    /*PRQA S 1006 -- */
}
/*PRQA S 3006,3672 --*/
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <Installation interrupt by default>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_Hal_InitCPU>
 */
/*****************************************************************************/
void Os_Arch_InterruptInit(Os_CoreIdType coreId) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    uint32 Index;
    Os_ArchIsrTempIPSR            = OS_NUM_0;
    if(OS_CORE_ID_0 == coreId)
    {
        uint32 vector_table_base = OS_ARCH_REG_VTOR; /*PRQA S 0303 */ /*VL_Os_0303*/
        /*PRQA S 0303,0305 ++ */ /*VL_Os_0303,VL_Os_0305*/
        Os_ArchVectorTable[OS_NUM_0] = (Os_IsrHandler)(*((volatile uint32 *)vector_table_base));
        /*PRQA S 0303,0305 -- */ 
        Os_ArchVectorTable[OS_NUM_1]  = (Os_IsrHandler)(&Reset_Handler);
        Os_ArchVectorTable[OS_NUM_2]  = (Os_IsrHandler)(&NMI_Handler);
        Os_ArchVectorTable[OS_NUM_3]  = (Os_IsrHandler)(&Os_Arch_HardFault_Handler);
        Os_ArchVectorTable[OS_NUM_4]  = (Os_IsrHandler)(&MemManage_Handler);          /* not enter because not ena in SHCSR */
        Os_ArchVectorTable[OS_NUM_5]  = (Os_IsrHandler)(&BusFault_Handler);           /* not enter because not ena in SHCSR */
        Os_ArchVectorTable[OS_NUM_6]  = (Os_IsrHandler)(&UsageFault_Handler);         /* not enter because not ena in SHCSR */
        Os_ArchVectorTable[OS_NUM_7]  = (Os_IsrHandler)(&Os_IntHandler);              /* Reserved */
        Os_ArchVectorTable[OS_NUM_8]  = (Os_IsrHandler)(&Os_IntHandler);              /* Reserved */
        Os_ArchVectorTable[OS_NUM_9]  = (Os_IsrHandler)(&Os_IntHandler);              /* Reserved */
        Os_ArchVectorTable[OS_NUM_10] = (Os_IsrHandler)(&Os_IntHandler);              /* Reserved */
        Os_ArchVectorTable[OS_NUM_11] = (Os_IsrHandler)(&Os_Arch_SVC_Handler);
        Os_ArchVectorTable[OS_NUM_12] = (Os_IsrHandler)(&DebugMon_Handler);
        Os_ArchVectorTable[OS_NUM_13] = (Os_IsrHandler)(&Os_IntHandler);              /* Reserved */
        Os_ArchVectorTable[OS_NUM_14] = (Os_IsrHandler)(&Os_Arch_PendSV_Handler);
        Os_ArchVectorTable[OS_NUM_15] = (Os_IsrHandler)(&Os_Arch_DefaultIntEntry);  /* Systick irq */
        for (Index = OS_ARCH_ISR_EXTERNAL_NUM; Index < (OS_MCU_NVIC_NUM + OS_NUM_1); Index++)
        {
            Os_ArchVectorTable[Index] = (Os_IsrHandler)(&Os_Arch_DefaultIntEntry);
        }
    }
    OS_ARCH_REG_VTOR              = (uint32)&Os_ArchVectorTable[OS_NUM_0];  /*PRQA S 0306,0303 */ /*VL_Os_0306,VL_Os_0303*/
    /*PRQA S 1277,1258,3432,0303,4397 ++ */ /*VL_Os_1277,VL_Os_1258,VL_Os_3432,VL_Os_0303,VL_Os_4397*/
    OS_ARCH_REG_SCB_SHPR(OS_SVCall_IRQn) = OS_ARCH_NVIC_CONVERT_SET_PRIO(OS_NVIC_PRIO_MAX);
    OS_ARCH_REG_SCB_SHPR(OS_PendSV_IRQn) = OS_ARCH_NVIC_CONVERT_SET_PRIO(OS_NVIC_PRIO_MAX);
    /*PRQA S 1277,1258,3432,0303,4397 -- */

    for (Index = OS_ARCH_ISR_EXTERNAL_NUM; Index < (OS_MCU_NVIC_NUM + OS_NUM_1); Index++)
    {
        /*PRQA S 1290 ++ */ /*VL_Os_1290*/
        Os_Hal_DisableInterruptSource(Index, 0);
        Os_Hal_ClearPendingInterrupt(Index, 0);
        /*PRQA S 1290 -- */
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
void Os_Arch_InitIntPrio(Os_CoreIdType coreId) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    Os_IsrType           i;
    const Os_IsrCfgType* pIsrCfg;
    Os_IsrType           idStartRange = Os_IsrIdRange[coreId].AllIsr.IsrStart;
    Os_IsrType           idEndRange   = Os_IsrIdRange[coreId].AllIsr.IsrEnd;
    for (i = idStartRange; i < idEndRange; i++) /* PRQA S 4527 */ /* VL_Os_4527 */
    {
        pIsrCfg = &(Os_IsrCfg[i]);
        /*PRQA S 4461 ++ */ /*VL_Os_4461*/
        Os_Arch_InterruptInstall(pIsrCfg->IsrSrc, Os_IntInstall[i].IntPrio, Os_IntInstall[i].ISR);
        /*PRQA S 4461 -- */
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <Ipl greater than 0, disable interrupt otherwise enable interrupt>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <ipl>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ResumeOSInterrupts and so on>
 */
/******************************************************************************/
void Os_Hal_SetIpl(Os_IPLType ipl, Os_IsrDescriptionType isrdesc) /*PRQA S 3006*/ /* VL_Os_3006 */
{
    /* Disable interrupt prio - 1,CAN open the max OSinterrupt self*/
    /*PRQA S 3432,1277,4397 ++ */ /*VL_Os_3432,VL_Os_1277,VL_Os_4397*/
    register Os_IPLType converted_prio = OS_ARCH_NVIC_CONVERT_SET_PRIO(ipl); /* PRQA S 2011 */ /* VL_Os_2011 */
    /*PRQA S 3432,1277,4397 -- */
    OS_ASM("msr basepri, %0" : : "r"(converted_prio)); /* PRQA S 1006 */ /* VL_Os_1006 */
    
    UNUSED_PARAMETER(isrdesc);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get current IPL>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <0>
 * PreCondition         <None>
 * CallByAPI            <SuspendOSInterrupts and so on>
 */
/******************************************************************************/
Os_IPLType Os_Hal_GetIpl(void) /*PRQA S 3006*/ /* VL_Os_3006 */
{
    register Os_IPLType result; /* PRQA S 2011 */ /* VL_Os_2011 */

    OS_ASM("MRS %0, basepri_max" : "=r"(result)); /* PRQA S 1006 */ /* VL_Os_1006 */

    result = OS_ARCH_NVIC_CONVERT_GET_PRIO(result); /* PRQA S 1277 */ /* VL_Os_1277 */

    return (result);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Disable the given interrupt source>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-vIsrSrc[in]    <Interrupt source>
 * param-vIsrSrcType[in]<Type of Interrupt source>
 * param-Name[out]      <None>
 * param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <DisableInterruptSource>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_Hal_DisableInterruptSource(uint32 vIsrSrc, uint32 vIsrSrcType)
{
    if (vIsrSrc >= OS_ARCH_ISR_EXTERNAL_NUM)
    {
#if (OS_MCU_EXTEND_INTERRUPT_CONTROLLER_USED == TRUE) /* PRQA S 3332 */ /* VL_Os_3332 */
        Os_Mcu_DisableInterruptSource(vIsrSrc);
#else
        /*PRQA S 1258,0303 ++ */ /*VL_Os_1258,VL_Os_0303 */
        OS_ARCH_REG_NVIC_ICER(vIsrSrc) = OS_ARCH_NVIC_GET_REG32_BITMASK(vIsrSrc);
        /* PRQA S 1258,0303 -- */
#endif /* OS_MCU_EXTEND_INTERRUPT_CONTROLLER_USED == TRUE */
    }
    else if (vIsrSrc == OS_ISR_SysTick_IRQn)
    {
        /*PRQA S 0303,3442, 1258 ++ */ /*VL_Os_0303,VL_Os_3442, VL_Os_ConstToIntegral */
        OS_ARCH_SYSTICK_CTRL_REG &= ~OS_ARCH_SYSTICK_ENABLE_BIT;
        /* PRQA S 0303,3442, 1258 -- */
    }
    else
    {
        /* Nothing need to do */
    }

    UNUSED_PARAMETER(vIsrSrcType);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Clear the pending status of the given interrupt>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-vIsrSrc[in]    <Interrupt source>
 * param-vIsrSrcType[in]<Type of Interrupt source>
 * param-Name[out]      <None>
 * param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <EnableInterruptSource><ClearPendingInterrupt>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_Hal_ClearPendingInterrupt(uint32 vIsrSrc, uint32 vIsrSrcType)
{
    if (vIsrSrc >= OS_ARCH_ISR_EXTERNAL_NUM)
    {
#if (OS_MCU_EXTEND_INTERRUPT_CONTROLLER_USED == TRUE) /* PRQA S 3332 */ /* VL_Os_3332 */
        Os_Mcu_ClearPendingInterrupt(vIsrSrc);
#else
        /*PRQA S 1258,0303 ++ */ /*VL_Os_1258,VL_Os_0303 */
        OS_ARCH_REG_NVIC_ICPR(vIsrSrc) = OS_ARCH_NVIC_GET_REG32_BITMASK(vIsrSrc);
        /* PRQA S 1258,0303 -- */
#endif /* OS_MCU_EXTEND_INTERRUPT_CONTROLLER_USED == TRUE */
    }
    else if (vIsrSrc == OS_ISR_SysTick_IRQn)
    {
        /*PRQA S 4397,0499,3442,0303 ++ */ /*VL_Os_4397,VL_Os_0499,VL_Os_3442,VL_Os_0303 */
        OS_ARCH_REG_SCB_ICSR |= OS_ARCH_SYSTICK_CLEAR_MASK;
        /* PRQA S 4397,0499,3442,0303 -- */
    }
    else
    {
        /* Nothing need to do */
    }

    UNUSED_PARAMETER(vIsrSrcType);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get the status of the given interrupt.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <srcAddr,prio,srcType,isrProc>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <None>
 * REQ ID               <>
 */
/******************************************************************************/
Os_IsrStateType Os_Hal_GetIsrSourceState(uint32 vIsrSrc, uint32 vIsrSrcType)
{
    Os_IsrStateType IsrSourceState;
    
    IsrSourceState = OS_ISR_DISABLED;
    if (vIsrSrc >= OS_ARCH_ISR_EXTERNAL_NUM) /* PRQA S 3332 */ /* VL_Os_3332 */
    {
#if (OS_MCU_EXTEND_INTERRUPT_CONTROLLER_USED == TRUE) /* PRQA S 3332 */ /* VL_Os_3332 */
        IsrSourceState = Os_Mcu_GetIsrSourceState(vIsrSrc);
#else
        /*PRQA S 1258,0303,3442 ++ */ /*VL_Os_1258,VL_Os_0303,VL_Os_3442 */
        if (OS_NUM_1 == ((OS_ARCH_REG_NVIC_ISER(vIsrSrc) >> OS_ARCH_NVIC_GET_REG32_BIT_OFFSET(vIsrSrc)) & OS_1BIT_MASK)) 
        /* PRQA S 1258,0303,3442 -- */
        {
            IsrSourceState = OS_ISR_ENABLED;
        }
#endif /* OS_MCU_EXTEND_INTERRUPT_CONTROLLER_USED == TRUE */
    }
    
    UNUSED_PARAMETER(vIsrSrcType);
    return IsrSourceState;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Enable the given interrupt source vIsrSrc.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <srcAddr,prio,srcType,isrProc>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <None>
 * REQ ID               <>
 */
void Os_Hal_EnableInterruptSource(uint32 vIsrSrc, uint32 vIsrSrcType)
{
    if (vIsrSrc >= OS_ARCH_ISR_EXTERNAL_NUM)
    {
#if (OS_MCU_EXTEND_INTERRUPT_CONTROLLER_USED == TRUE) /* PRQA S 3332 */ /* VL_Os_3332 */
        Os_Mcu_EnableInterruptSource(vIsrSrc);
#else
        /*PRQA S 1258,0303 ++ */ /*VL_Os_1258,VL_Os_0303 */
        OS_ARCH_REG_NVIC_ISER(vIsrSrc) = OS_ARCH_NVIC_GET_REG32_BITMASK(vIsrSrc);
        /* PRQA S 1258,0303 -- */
#endif /* OS_MCU_EXTEND_INTERRUPT_CONTROLLER_USED == TRUE */
    }
    else if (vIsrSrc == OS_ISR_SysTick_IRQn)
    {
        /*PRQA S 0303,3442, 1258 ++ */ /*VL_Os_0303,VL_Os_3442, VL_Os_ConstToIntegral */
        OS_ARCH_SYSTICK_CTRL_REG |= OS_ARCH_SYSTICK_ENABLE_BIT;
        /* PRQA S 0303,3442, 1258 -- */
    }
    else
    {
        /* Nothing need to do */
    } 

    UNUSED_PARAMETER(vIsrSrcType);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get curent IRQ number>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-vIsrSrc[in]    <None>
 * param-vIsrSrcType[in]<None>
 * param-Name[out]      <None>
 * param-Name[in/out]   <None>
 * return               <uint32>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
uint32 Os_Hal_GetCurrentIrq(void) /* PRQA S 3006,1532 */ /* VL_Os_3006,VL_QAC_OneFunRef */
{
    register uint32 irqNum; /* PRQA S 2011 */ /* VL_Os_2011 */

#if (OS_MCU_EXTEND_INTERRUPT_CONTROLLER_USED == TRUE) /* PRQA S 3332 */ /* VL_Os_3332 */
    irqNum = Os_Mcu_GetCurrentIrq();
#else
    OS_ASM("mrs  %0, ipsr" : "=r"(irqNum)); /* PRQA S 1006 */ /* VL_Os_1006 */
    irqNum &= OS_ARCH_NVIC_IRQNUM_VALID_BITMASK;
#endif /* OS_MCU_EXTEND_INTERRUPT_CONTROLLER_USED == TRUE */

    return irqNum;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get Master SP>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-Name[in]       <None>
 * param-Name[out]      <uint32 SpVal>
 * param-Name[in/out]   <None>
 * return               <uint32>
 * PreCondition         <None>
 * CallByAPI            <ASM>
 * REQ ID               <None>
 */
/******************************************************************************/
uint32 Os_Arch_GetMainSP(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    return Os_ArchMainSpNest[Os_Kernel_GetIntNestNumISR1() + Os_Kernel_GetIntNestNumISR2()];
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Set Master SP>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-Name[in]       <None>
 * param-Name[out]      <uint32 SpVal>
 * param-Name[in/out]   <None>
 * return               <uint32>
 * PreCondition         <None>
 * CallByAPI            <ASM>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_Arch_SaveMainSpAndXpsr(uint32 vMsp,uint32 vXpsr)
{
    uint32 vIntNestNum = (uint32)Os_Kernel_GetIntNestNumISR2() + (uint32)Os_Kernel_GetIntNestNumISR1();
    Os_ArchMainSpNest[vIntNestNum] = vMsp;
    Os_ArchIsrxPSRNest[vIntNestNum] = vXpsr;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Set Master SP>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-Name[in]       <None>
 * param-Name[out]      <uint32 SpVal>
 * param-Name[in/out]   <None>
 * return               <uint32>
 * PreCondition         <None>
 * CallByAPI            <ASM>
 * REQ ID               <None>
 */
/******************************************************************************/
uint32 Os_Arch_GetTerminatedIntMainSP(void)
{
    return Os_ArchMainSpNest[Os_Kernel_GetIntNestNumISR1() + Os_Kernel_GetIntNestNumISR2() + OS_NUM_1];
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Set the pending status of the given interrupt>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-vIsrSrc[in]    <Interrupt source>
 * param-vIsrSrcType[in]<Type of Interrupt source>
 * param-Name[out]      <None>
 * param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <EnableInterruptSource><ClearPendingInterrupt>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_Arch_SetPendingInterrupt(uint32 vIsrSrc, uint32 vIsrSrcType) /* PRQA S 3006 */ /* VL_Os_3006 */
{
    OS_ARCH_REG_NVIC_STIR = ((uint32)(vIsrSrc) - OS_NUM_16) & OS_ARCH_NVIC_IRQNUM_VALID_BITMASK; /* PRQA S 0303 */ /* VL_Os_0303 */
    OS_ARCH_DSYNC(); /* PRQA S 1006 */ /* VL_Os_1006 */

    UNUSED_PARAMETER(vIsrSrcType);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/*=======[E N D   O F   F I L E]==============================================*/
