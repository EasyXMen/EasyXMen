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
**  FILENAME    : Os_Arch_Processor.c                                         **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : Processor Request Managment                                 **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r23                         **
**  Version :   AUTOSAR classic Platform R23--Function Safety                 **
**                                                                            **
*******************************************************************************/

/*=======[I N C L U D E S]===================================================*/

#include "Os_Types.h"
#include "Os_Arch_Define.h"
#include "Os_Arch_Processor.h"
#include "Os_Arch_Timer.h"
#include "Os_Task_CfgData.h"
#include "Os_Sprot.h"
#include "Os_Appl_CfgData.h"

/*=======[M A C R O S]=======================================================*/

/*=======[I N T E R N A L   D A T A]=========================================*/

#define OS_START_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "Os_MemMap.h"
static Os_StackPtrType Os_ArchTaskSwicthSavedStack[CFG_TASK_MAX];
#define OS_STOP_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "Os_MemMap.h"

#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static uint32 Os_Arch_GetControlVale(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC) */ /* PRQA S 2053 */ /*VL_Os_2053*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]===================*/

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <First entry task>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_PreTask>
 */
/*****************************************************************************/
void Os_Hal_FirstEnterTask(Os_TaskType taskId) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    /* PRQA S 3432,0306 ++ */ /* VL_Os_3432, VL_Os_0306 */
    uint32* Os_ArchTopStkPtr = (uint32*)OS_ARCH_STACK_ALIGN((uint32)(Os_TaskCfg[taskId].TaskStack.StackTop));
    /* PRQA S 3432,0306 -- */

    /*registers stacked as if auto-saved on exception.
     * The follow registers will automatic restore when exiting an exception.
     * --xPSR, PC, LR, R12, R3, R2, R1, R0--*/
    /* PRQA S 3387,3440,0489 ++ */    /* VL_Os_3387, VL_Os_3440, VL_Os_0489 */
    *(--Os_ArchTopStkPtr) = OS_ARCH_XPSR_FOR_FIRST_ENTRY;             /* xPSR */
    /* PRQA S 0305 ++*/                    /* VL_Os_0305 */
    *(--Os_ArchTopStkPtr) = (uint32)Os_TaskCfg[taskId].TaskEntry;
    *(--Os_ArchTopStkPtr) = (uint32)Os_TaskErrBack; /* PRQA S 0428 */ /* VL_Os_0428 */
    /* PRQA S 0305 --*/    
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */                                          
    *(--Os_ArchTopStkPtr) = 12u;                                      /* R12 */
    *(--Os_ArchTopStkPtr) = 3u;                                       /* R3 */
    *(--Os_ArchTopStkPtr) = 2u;                                       /* R2 */
    *(--Os_ArchTopStkPtr) = 1u;                                       /* R1 */
    *(--Os_ArchTopStkPtr) = 0u;                                       /* R0 */

    *(--Os_ArchTopStkPtr) = 0xFFFFFFFDu;                              /* R14: EXEC_RETURN */
    /*remaining registers saved on process stack*/
    *(--Os_ArchTopStkPtr) = 11U;                                      /* R11 */
    *(--Os_ArchTopStkPtr) = 10U;                                      /* R10 */
    *(--Os_ArchTopStkPtr) = 9U;                                       /* R9 */
    *(--Os_ArchTopStkPtr) = 8U;                                       /* R8 */
    *(--Os_ArchTopStkPtr) = 7U;                                       /* R7 */
    *(--Os_ArchTopStkPtr) = 6U;                                       /* R6 */
    *(--Os_ArchTopStkPtr) = 5U;                                       /* R5 */
    *(--Os_ArchTopStkPtr) = 4U;                                       /* R4 */
    /* PRQA S 3120 -- */
    /*CONTROL register*/
    Os_ArchTopStkPtr = Os_ArchTopStkPtr - 1;
    /* PRQA S 3387,3440,0489 -- */ 
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    if ((TRUE == Os_AppCfg[Os_Kernel_GetSysRunningAppID()].Trusted)
        && (TRUE != Os_AppCfg[Os_Kernel_GetSysRunningAppID()].TrustedAppWithProtection))
    {
        *Os_ArchTopStkPtr = 0x00000000u;
    }
    else
    {
        *Os_ArchTopStkPtr = 0x00000001u;
    }
#else
    *Os_ArchTopStkPtr = 0x00000000u;
#endif
    Os_ArchTaskSwicthSavedStack[taskId] = (Os_StackPtrType)Os_ArchTopStkPtr;
    /* PRQA S 0303,1277,1258,3432,4397 ++ */ /* VL_Os_0303, VL_Os_1277, VL_Os_1258, VL_Os_3432, VL_Os_4397 */
    OS_ARCH_REG_SCB_SHPR(OS_PendSV_IRQn) = OS_ARCH_NVIC_CONVERT_SET_PRIO(OS_NVIC_PRIO_MIN);
    /* PRQA S 0303,1277,1258,3432,4397 -- */ 
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <Os_Hal_Dispatch>
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
void Os_Hal_Dispatch(void) /* PRQA S 3006 */ /* VL_Os_3006 */
{
    /* Call PendSV to switch task */
    Os_Hal_DisableInt(); /* PRQA S 1006 */ /* VL_Os_1006 */
    /* PRQA S 4397, 0303, 3442, 0499 ++ */ /* VL_Os_4397, VL_Os_0303, VL_Os_3442, VL_Os_0499 */
    OS_ARCH_REG_SCB_ICSR |= OS_ARCH_PENDSV_REQUEST_MASK;
    /* PRQA S 4397, 0303, 3442, 0499 -- */ 
    Os_Hal_EnableInt(); /* PRQA S 1006 */ /* VL_Os_1006 */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <Os_Hal_StartScheduler>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <ShutdownOS>
 */
/*****************************************************************************/
void Os_Hal_StartScheduler(void) /* PRQA S 3006,1532 */ /* VL_Os_3006, VL_QAC_OneFunRef */
{
    /* Call PendSV to switch task and switch to system stack*/
    Os_Hal_DisableInt(); /* PRQA S 1006 */ /* VL_Os_1006 */
    Os_Hal_Switch2System(); /* PRQA S 3138,1006 */ /* VL_Os_3138, VL_Os_1006 */
    Os_Arch_StartSysTimer();
    /* PRQA S 4397, 0303, 3442, 0499 ++ */ /* VL_Os_4397, VL_Os_0303, VL_Os_3442, VL_Os_0499 */
    OS_ARCH_REG_SCB_ICSR |= OS_ARCH_PENDSV_REQUEST_MASK;
    /* PRQA S 4397, 0303, 3442, 0499 -- */ 
    Os_Hal_EnableInt(); /* PRQA S 1006 */ /* VL_Os_1006 */

    while (OS_NUM_1) /*PRQA S 2740*/ /* VL_Os_2740 */
    {
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (TRUE == OS_CFG_ASM_FPU_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <Floating Point Unit initialization>
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
void Os_Arch_FpuInit(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    /* Enable CP10 and CP11 coprocessors */
    /* PRQA S 4397, 0303, 3442, 0499 ++ */ /* VL_Os_4397, VL_Os_0303, VL_Os_3442, VL_Os_0499 */
    OS_REG_SCB_CPACR |= (OS_CPACR_CP10_MASK | OS_CPACR_CP11_MASK);

    /*Enable CONTROL.FPCA setting on execution of a floating-point instruction.*/
    OS_REG_FPCCR |= (OS_1BIT_MASK << OS_NUM_31);
    OS_REG_FPCCR |= (OS_1BIT_MASK << OS_NUM_30);
    /* PRQA S 4397, 0303, 3442, 0499 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == OS_CFG_ASM_FPU_ENABLE */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
Os_StackPtrType Os_Arch_GetRunningTaskStack(void)
{
    return Os_ArchTaskSwicthSavedStack[Os_Kernel_GetSysRunningTaskID()];
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
void Os_Arch_SetRunningTaskStack(Os_StackPtrType stackTop)
{
    Os_ArchTaskSwicthSavedStack[Os_Kernel_GetSysRunningTaskID()] = stackTop;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
boolean Os_Arch_IsFirstDispatch(void)
{
    boolean RetVal;

    if (INVALID_TASK == Os_Kernel_GetSysRunningTaskID())
    {
        RetVal = (boolean)TRUE;
    }
    else
    {
        RetVal = (boolean)FALSE;
    }

    return RetVal;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get the CONTROL Mode>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-core[in]       <None>
 * Return               <uin32,the CONTOL Register vale>
 * PreCondition         <None>
 * CallByAPI            <Os_ArchGetThreadMode>
 */
/******************************************************************************/
uint32 Os_Arch_GetControlVale(void) 
{
    register uint32 RetVal;

    OS_ASM("MRS %0, control" : "=r"(RetVal));

    return RetVal;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
Os_CpuModeType Os_Arch_GetCpuMode(void)
{
    Os_CpuModeType CoreMode = OS_CPUMODE_SUPERVISOR;
    uint32 tpPrivilege = (Os_Arch_GetControlVale() & OS_1BIT_MASK);

    if (tpPrivilege == OS_NUM_1)
    {
        CoreMode = OS_CPUMODE_USER0;
    }

    return CoreMode;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC) */ /* PRQA S 2053 */ /*VL_Os_2053*/

/*=======[E N D   O F   F I L E]==============================================*/
