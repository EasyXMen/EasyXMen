/* PRQA S 3108++ */
/**
 * Copyright (C) 2008-2025 isoft Infrastructure Software Co., Ltd.
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
/* PRQA S 3108-- */
/*
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
/*=======[I N C L U D E S]===================================================*/
#include "Os_Arch_Irq.h"
/*=======[M A C R O S]=======================================================*/
/*=======[E X T E R N A L   D A T A]=========================================*/
#define OS_START_SEC_VAR_INTVECTOR_GLOBAL_32
#include "Os_MemMap.h"
uint32 Os_GlobalVector[OS_NVIC_NUM + 1u];
#define OS_STOP_SEC_VAR_INTVECTOR_GLOBAL_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_INTVECTOR_LOCAL_32
#include "Os_MemMap.h"
uint32 Os_LocalVector[OS_NVIC_NUM + 1u];
#define OS_STOP_SEC_VAR_INTVECTOR_LOCAL_32
#include "Os_MemMap.h"
/* PRQA S 0791++ */ /* MISRA Rule 5.4 */
#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
uint32 Os_ArchMasterSp_ARRAY[CFG_ISR_MAX];
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
uint32 Os_ISRxPSRStack[CFG_ISR_MAX];
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
uint32 Os_IsrTempIPSR;
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
uint32 Os_Isr2_Ipl_Limit;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

/* PRQA S 0791-- */ /* MISRA Rule 5.4 */
/*=======[I N T E R N A L   D A T A]=========================================*/
/* PRQA S 0305,0428 ++ */ /* MISRA Rule 11.1,CWE-398 */

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]=======*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]===================*/

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
Os_ArchMsrType Os_Hal_SuspendInt(void) /*PRQA S 3006,3432*/ /*MISRA Dir 4.3,Rule 20.7*/
{
    register Os_ArchMsrType result; /* PRQA S 2011 */ /* MISRA CWE-398 */
    OS_ASM("MRS %0, primask" : "=r"(result));
    OS_ASM("MSR primask, %0" : : "r"(OS_MSR_PRIMASK_BIT0));
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
void Os_Hal_RestoreInt(Os_ArchMsrType msr) /*PRQA S 3006*/ /*MISRA Dir 4.3*/
{
    UNUSED_PARAMETER(msr);
    OS_ARCH_DSYNC();
    OS_ASM("MSR primask, %0 " : : "r"(msr));
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
/*PRQA S 3006,3672 ++*/ /*MISRA Dir 4.3,CWE-398*/
void Os_ArchInterruptInstall(uint8 id, uint8 prio, Os_isrhnd isrProc)
{
    /* PRQA S 0303,3345,3442 ++*/ /* MISRA Rule 11.4,CWE-398 */
    OS_INTERRUPT_INSTALL(id, prio);
    /* PRQA S 0303,3345,3442 --*/ /* MISRA Rule 11.4,CWE-398 */

    /* Save the former handler pointer */
    if (isrProc != NULL_PTR)
    {
        /* Set handler into vector table */
        Os_LocalVector[id] = (uint32)isrProc; /* PRQA S 0305 */ /* MISRA Rule 11.1 */
    }
    OS_ASM("dsb");
    OS_ASM("isb");
}
/*PRQA S 3006,3672 --*/ /*MISRA Dir 4.3,CWE-398*/
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
void Os_ArchInterruptInit(void)
{
    uint32 index;
    Os_IsrTempIPSR = 0;
    OS_REG_VTOR = (uint32)&Os_GlobalVector[0u]; /* PRQA S 0303,0306*/ /*Rule-11.4*/
    Os_GlobalVector[0] = (uint32)0u;
    Os_GlobalVector[1] = (uint32)(&Reset_Handler);
    Os_GlobalVector[2] = (uint32)(&NMI_Handler);
    Os_GlobalVector[3] = (uint32)(&HardFault_Handler);
    Os_GlobalVector[4] = (uint32)(&MemManage_Handler);
    Os_GlobalVector[5] = (uint32)(&BusFault_Handler);
    Os_GlobalVector[6] = (uint32)(&UsageFault_Handler);
    Os_GlobalVector[7] = (uint32)0u;  /* Reserved */
    Os_GlobalVector[8] = (uint32)0u;  /* Reserved */
    Os_GlobalVector[9] = (uint32)0u;  /* Reserved */
    Os_GlobalVector[10] = (uint32)0u; /* Reserved */
    Os_GlobalVector[11] = (uint32)(&Os_SVC_Handler);
    Os_GlobalVector[12] = (uint32)(&DebugMon_Handler);
    Os_GlobalVector[13] = (uint32)0u;
    Os_GlobalVector[14] = (uint32)(&PendSV_Handler);
    Os_GlobalVector[15] = (uint32)(&armv7_default_isr);
    /* PRQA S 0303 ++ */                                              /* MISRA Rule-11.4 */
    OS_INTERRUPT_SYS_SET_PRIO(OS_SVCall_IRQn, OS_NVIC_CONVERT_SET_PRIO(OS_NVIC_PRIO_MAX));
    OS_INTERRUPT_SYS_SET_PRIO(OS_PendSV_IRQn, OS_NVIC_CONVERT_SET_PRIO(OS_NVIC_PRIO_MAX));
    /* PRQA S 0303 -- */ /* MISRA Rule-11.4 */
    for (index = OS_ISR_EXTERNAL_NUM; index < OS_NVIC_NUM; index++)
    {
        Os_GlobalVector[index] = (uint32)(&armv7_default_isr);
        OS_INTERRUPT_DISABLE(index); /* PRQA S 0303,3345,3442 */ /* MISRA Rule 11.4,CWE-398 */
        OS_INTERRUPT_CLEAR_PENDING(index); /* PRQA S 0303 */     /* MISRA Rule 11.4 */
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
void Os_Hal_SetIpl(Os_IPLType ipl, Os_IsrDescriptionType isrdesc) /*PRQA S 3006*/ /*MISRA Dir 4.3*/
{
    /*add comments to pass QAC.*/
    (void)isrdesc;
    /* Disable interrupt prio - 1,CAN open the max OSinterrupt self*/
    register Os_IPLType converted_prio = OS_NVIC_CONVERT_SET_PRIO(ipl); /* PRQA S 2011 */ /* MISRA CWE-398 */

    OS_ASM("msr basepri, %0" : : "r"(converted_prio));
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
Os_IPLType Os_Hal_GetIpl(void) /*PRQA S 3006*/ /*MISRA Dir 4.3*/
{
    register Os_IPLType result; /* PRQA S 2011 */ /* MISRA CWE-398 */

    OS_ASM("MRS %0, basepri_max" : "=r"(result));

    result = OS_NVIC_CONVERT_GET_PRIO(result);

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
    UNUSED_PARAMETER(vIsrSrcType);
    OS_INTERRUPT_DISABLE(vIsrSrc); /* PRQA S 0303,3345,3442 */ /* MISRA Rule 11.4,CWE-398 */
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
    UNUSED_PARAMETER(vIsrSrcType);
    if(vIsrSrc >= 16)
    {
        OS_INTERRUPT_CLEAR_PENDING(vIsrSrc); /* PRQA S 0303 */ /* MISRA Rule 11.4 */
    }
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
    UNUSED_PARAMETER(vIsrSrcType);
    Os_IsrStateType isrSourceState = OS_ISR_DISABLED;
    if (1U == OS_INTERRUPT_CHECK_STATUS(vIsrSrc)) /* PRQA S 0303,3442 */ /* MISRA Rule 11.4,CWE-398 */
    {
        isrSourceState = OS_ISR_ENABLED;
    }
    return isrSourceState;
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
    UNUSED_PARAMETER(vIsrSrcType);
    OS_INTERRUPT_ENABLE(vIsrSrc); /* PRQA S 0303,3345,3442 */ /* MISRA Rule 11.4,CWE-398 */
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
uint32 Os_Hal_GetCurrentIrq(void)
{
    register uint32 irqNum;
    OS_ASM("mrs  %0, ipsr" : "=r"(irqNum));
    return (irqNum & 0x1ff);
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
uint32 Os_Hal_GetCurrentExc(void)
{
    return 0ul;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"


#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <Os_ArchDispatch_ISR>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 */
/*****************************************************************************/
void Os_ArchDispatch_ISR(void)
{
    /* Call PendSV to switch task */
    uint32 NVIC_PENDSVSET = 0x10000000;
    Os_Hal_DisableInt();
    OS_SCB_ICSE_BASE |= NVIC_PENDSVSET;
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
uint32 Os_ArchGetMasterSP(void)
{
   return Os_ArchMasterSp_ARRAY[Os_Kernel_GetIntNestNumISR1() + Os_Kernel_GetIntNestNumISR2()];
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"


#define OS_START_SEC_CODE
#include "Os_MemMap.h"
void Os_ArchSaveMasterSP(uint32* MSP)
{
    Os_ArchMasterSp_ARRAY[Os_Kernel_GetIntNestNumISR2() + Os_Kernel_GetIntNestNumISR1()] = (uint32)MSP;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"


#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get SaveArray of Xpsr 's Addr>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-Name[in]       <None>
 * param-Name[out]      <None>
 * param-Name[in/out]   <None>
 * return               <uint32>
 * PreCondition         <None>
 * CallByAPI            <ASM>
 * REQ ID               <None>
 */
/******************************************************************************/
uint32 Os_ArchGetSaveXpsrAddr(void)
{
    return (uint32)( &Os_ISRxPSRStack[Os_Kernel_GetIntNestNumISR2()] );
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/*=======[E N D   O F   F I L E]==============================================*/
