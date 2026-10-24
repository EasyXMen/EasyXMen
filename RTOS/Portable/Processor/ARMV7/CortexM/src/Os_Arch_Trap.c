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
**  FILENAME    : Os_Arch_Trap.c                                              **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : functions related to the processor exception                **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r23                         **
**  Version :   AUTOSAR classic Platform R23--Function Safety                 **
**                                                                            **
*******************************************************************************/

/*=======[I N C L U D E S]====================================================*/

#include "Os_Types.h"
#include "Os_Kernel2Port.h"
#include "Os_Arch_Processor.h"
#include "Os_Arch_Trap.h"
#include "Os_Arch_Irq.h"
#include "Os_Mcu_Config.h"
#if ((OS_MCU_EXTERNAL_MPU_USED == FALSE) && (TRUE == CFG_MEMORY_PROTECTION_ENABLE))
#include "Os_Arch_Mpu.h"
#endif /*  (OS_MCU_EXTERNAL_MPU_USED == FALSE) && (TRUE == CFG_MEMORY_PROTECTION_ENABLE) */ /* PRQA S 2053 */ /*VL_Os_2053*/

/*=======[E X T E R N A L   D A T A]=========================================*/
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
#define OS_START_SEC_VAR_CLONE_16
#include "Os_MemMap.h"
boolean OS_ArchHardFaultFlag;
#define OS_STOP_SEC_VAR_CLONE_16
#include "Os_MemMap.h"
#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */

#if ((TRUE == CFG_FAULT_MANAGEMENT_ENABLE) && (TRUE == CFG_HARDWARE_FAULT_ENABLE))

extern Os_ExceptionContextType Os_HalContextExceptionContext[]; /* PRQA S 3684 */ /* VL_Os_3684 */

#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
/* PRQA S 3625,3218, 3232 ++ */ /* VL_Os_3625, VL_Os_3218, VL_Os_3232 */
static const char* Os_TrapMsg = "NULL";
/* PRQA S 3625,3218, 3232 -- */
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

#endif /* (TRUE == CFG_FAULT_MANAGEMENT_ENABLE) && (TRUE == CFG_HARDWARE_FAULT_ENABLE) */ /* PRQA S 2053 */ /*VL_Os_2053*/

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static StatusType Os_Arch_GetFault(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]===================*/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <SVC_Handler>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <System services>
 */
/*****************************************************************************/
void Os_Arch_SVC_Handler(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    OS_ARCH_SWITCH_THREAD_PRIVILEGE();
#else
    while(OS_NUM_1) /* PRQA S 2740 */ /* VL_Os_2740 */
    {
        /* None SC3 or SC4 should not be in this handler */
    }
#endif /* (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC) */ /* PRQA S 2053 */ /*VL_Os_2053*/
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"

/* Memory access error handler */
void Os_Arch_HardFault_Process(uint32 TrapMsg)
{
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
    OS_ArchHardFaultFlag = (boolean)TRUE;
    OS_ARCH_SWITCH_THREAD_PRIVILEGE();
    Os_Hal_MemProtKernelMap();
#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */

    /* TrapMsg contains SP[31:1] and SP[0] store Fpu save flag */
    Os_ExceptionHandler((void*)TrapMsg,  Os_Arch_GetFault()); /* PRQA S 0326 */ /* VL_Os_0326 */

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
    OS_ArchHardFaultFlag = (boolean)FALSE;
#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */
}

#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get curent Exc number>
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
uint32 Os_Hal_GetCurrentExc(void) /* PRQA S 3006 */ /* VL_Os_3006 */
{
    register uint32 ExcNum; /* PRQA S 2011 */ /* VL_Os_2011 */

    OS_ASM("mrs  %0, ipsr" : "=r"(ExcNum)); /* PRQA S 1006 */ /* VL_Os_1006 */

    return (ExcNum & OS_ARCH_NVIC_IRQNUM_VALID_BITMASK);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if ((TRUE == CFG_FAULT_MANAGEMENT_ENABLE) && (TRUE == CFG_HARDWARE_FAULT_ENABLE))
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <exception information parse.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Trap function>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_Hal_ExceptionInformationParse(Os_TrapContextType* TrapContext) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    TrapContext->TrapMsg.ExceptionAddress       = TrapContext->TrapRegs.Ra;
    TrapContext->TrapMsg.ExceptionSP            = TrapContext->TrapRegs.Sp;
    TrapContext->TrapMsg.ExceptionSysteamStatus = TrapContext->TrapRegs.Psw;
    TrapContext->TrapMsg.ExceptionCase          = TrapContext->TrapRegs.CFSR;
    TrapContext->TrapMsg.ExceptionCaseMsg       = Os_TrapMsg;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <exception information parse.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Trap function>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_HalExceptionSaveContext(const void* TrapMsg) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    /* PRQA S 0306,0326 ++ */ /* VL_Os_0306,VL_Os_0326 */
    const uint32* Sp = (uint32*)(OS_ARCH_STACK_ALIGN(TrapMsg)); 
    /* PRQA S 0306,0326 -- */
#if (OS_CFG_ASM_FPU_ENABLE == TRUE)
    /* PRQA S 4340,0326 ++ */ /* VL_Os_4340,VL_Os_0326 */
    const boolean FpuSaveFlag = (boolean)(((((uint32)TrapMsg) & OS_1BIT_MASK) != 0u) ? TRUE : FALSE);
    /* PRQA S 4340,0326 -- */
#endif /* OS_CFG_ASM_FPU_ENABLE == TRUE */
    Os_ExceptionContextType* Context;
    uint8 Index;

    Context = &Os_HalContextExceptionContext[Os_Hal_GetCoreID()];

    Context->Sp = (uint32)Sp;                           /* Sp */ /* PRQA S 0306 */ /* VL_Os_0306 */
    Context->CFSR = OS_REG_SCB_CFSR;                    /* CFSR */ /* PRQA S 0303 */ /* VL_Os_0303 */
    Context->ExceptionSource = Os_Hal_GetCurrentExc();  /* Exception number */
    Context->Psw = Sp[OS_NUM_7];                        /* Xpsr */
    Context->Ra = Sp[OS_NUM_6];                         /* Return address */

    for (Index = OS_NUM_0; Index < OS_DATA_REG_NUM; Index++)
    {
        Context->GeneralRegisters[Index] = Sp[Index];   /* r0-r3 */
    }
    
#if (OS_CFG_ASM_FPU_ENABLE == TRUE)
    /* Fpu Save */
    if (FpuSaveFlag == TRUE)
    {
        for (Index = OS_NUM_0; Index < OS_FPU_REG_NUM; Index++)
        {
            Context->GeneralFpuRegister[Index] = Sp[Index + OS_NUM_8];   /* s0-s16, FPSCR */
        }
    }
    else
    {
        for (Index = OS_NUM_0; Index < OS_FPU_REG_NUM; Index++)
        {
            Context->GeneralFpuRegister[Index] = OS_32BIT_MASK;          /* s0-s16, FPSCR */
        }
    }
#endif /* OS_CFG_ASM_FPU_ENABLE == TRUE */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <exception information parse.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Trap function>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_HalExceptionResumeContext(const void* trapMsg) /* PRQA S 3206,1532 */ /* VL_Os_3206,VL_QAC_OneFunRef */
{
    /* Nothing need to do self return to restore */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* (TRUE == CFG_FAULT_MANAGEMENT_ENABLE) && (TRUE == CFG_HARDWARE_FAULT_ENABLE) */ /* PRQA S 2053 */ /* VL_Os_2053 */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The function get hard fault.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-Name[in]       <None>
 * Param-Name[in/out]   <None>
 * Param-Name[out]      <None>
 * Return               <uint8>
 * PreCondition         <None>
 * CallByAPI            <HardFault_Handler>
 * REQ ID               <None>
 */
/******************************************************************************/
static StatusType Os_Arch_GetFault(void)
{
    /* Cause StatusType is defined by unsigned char, define code by group */
    StatusType RetErr;
    /* PRQA S 0303,3442,4397,0499 ++ */ /* VL_Os_0303,VL_Os_3442,VL_Os_4397,VL_Os_0499 */
    if ((OS_REG_SCB_HFSR & OS_ARCH_ERROR_HARDFORCE_MASK) == OS_NUM_0)
    /* PRQA S 0303,3442,4397,0499 -- */
    {
        RetErr = (StatusType)OS_ARCH_ERROR_HARDFAULT;
    }
    else
    {
        RetErr = (StatusType)OS_ARCH_ERROR_HARDFORCE;
        /* PRQA S 0303,3442,4397 ++ */ /* VL_Os_0303,VL_Os_3442,VL_Os_4397 */
        if ((OS_REG_SCB_CFSR & OS_ARCH_CFSR_MEM_FAULT_MASK) != OS_NUM_0)
        /* PRQA S 0303,3442,4397 -- */
        {
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
            if ((OS_REG_SCB_CFSR & (OS_ARCH_CFSR_MMFSR_IACCVIOL_MASK | OS_ARCH_CFSR_MMFSR_DACCVIOL_MASK)) != OS_NUM_0)
            {
                RetErr = (StatusType)E_OS_PROTECTION_MEMORY;
                /* clear Mpu Error */
                OS_REG_SCB_CFSR = (OS_ARCH_CFSR_MMFSR_IACCVIOL_MASK | OS_ARCH_CFSR_MMFSR_DACCVIOL_MASK);
            }
            else
            {
                RetErr |= (StatusType)OS_ARCH_ERROR_MEMORYFALUT;
            }
#else
            RetErr |= (StatusType)OS_ARCH_ERROR_MEMORYFALUT;
#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */
        }
        /* PRQA S 0303,3442,4397,0499 ++ */ /* VL_Os_0303,VL_Os_3442,VL_Os_4397,VL_Os_0499 */
        if ((OS_REG_SCB_CFSR & OS_ARCH_CFSR_BUS_FAULT_MASK) != OS_NUM_0)
        /* PRQA S 0303,3442,4397,0499 -- */
        {
            #if (defined(OS_MCU_EXTERNAL_MPU_USED) && (TRUE == OS_MCU_EXTERNAL_MPU_USED) && (TRUE == CFG_MEMORY_PROTECTION_ENABLE))
                RetErr = (StatusType)E_OS_PROTECTION_MEMORY;
            #else
                RetErr |= (StatusType)OS_ARCH_ERROR_BUSFAULT;
            #endif
        }
        /* PRQA S 0303,3442,4397,0499 ++ */ /* VL_Os_0303,VL_Os_3442,VL_Os_4397,VL_Os_0499 */
        if ((OS_REG_SCB_CFSR & OS_ARCH_CFSR_USAGE_FAULT_MASK) != OS_NUM_0)
        /* PRQA S 0303,3442,4397,0499 -- */
        {
            RetErr |= (StatusType)OS_ARCH_ERROR_USAGEFAULT;
        }
    }

    /* Do not clear other error bit cause user may need it in ProtectionHook */
    return RetErr;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/*=======[E N D   O F   F I L E]==============================================*/
