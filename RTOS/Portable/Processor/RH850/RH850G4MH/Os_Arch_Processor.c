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
 **  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
 **  Version :   AUTOSAR classic Platform R19--Function Safety                 **
 **                                                                            **
 *******************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Os_Types.h"
#include "Os_Kernel2Port.h"
#include "Os_Arch_Processor.h"
#include "Os_Arch_Define.h"
#include "Os_Arch_Irq.h"
#include "Os_Cfg.h"
#include "Os_Task_CfgData.h"
#include "Os_Appl_CfgData.h"
#include "Os_Mprot_CfgData.h"
#include "Os_Sprot.h"
#include "Os_Cfg_S.h"
#include "Os_Mcu_Config.h"
/*=======[M A C R O S]========================================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/
#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
/*Save the top of the stack for now.*/
volatile uint32 Os_ArchTempSp;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
/*Save the task context stack pointer address.*/
Os_ArchTaskCBExtType Os_TaskCBExt[CFG_TASK_MAX];
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/

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
 * Return               <void>
 * PreCondition         <None>
 * CallByAPI            <Os_PreTask>
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_Hal_FirstEnterTask(Os_TaskType TaskID)
/* PRQA S 1532 -- */
{
    /* PRQA S 3469, 0306 ++ */ /* VL_Os_3469, VL_Os_0306 */
    uint32* Os_ArchTopStkPtr = (uint32*)(OS_ARCH_STACK_ALIGN((uint32)Os_TaskCfg[TaskID].TaskStack.StackTop));
    /* PRQA S 3469, 0306 -- */

    /* R1, R2, R6-R31 */
    /*Build the system register stack and restore to the register when entering the task*/
    /* PRQA S 3440, 0305, 3387, 0489 ++ */ /* VL_Os_3440, VL_Os_0305, VL_Os_3387,VL_Os_0489 */
    Os_ArchTopStkPtr      = Os_ArchTopStkPtr - OS_ARCH_PROGRAM_REGS_NUM; /* PRQA S 0488 */ /*VL_Os_0488 */
    *(--Os_ArchTopStkPtr) = (uint32)Os_TaskErrBack; /*LP*/ /* PRQA S 0428 */               /*VL_Os_0428 */
    *(--Os_ArchTopStkPtr) = (uint32)Os_TaskCfg[TaskID].TaskEntry;                          /*PC*/

    /* PSW */
    Os_ArchTopStkPtr = Os_ArchTopStkPtr - 1U;
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    if (((boolean)TRUE == Os_AppCfg[Os_Kernel_GetSysRunningAppID()].Trusted)
        && ((boolean)TRUE != Os_AppCfg[Os_Kernel_GetSysRunningAppID()].TrustedAppWithProtection))
    {
        *Os_ArchTopStkPtr = (1UL << OS_ARCH_PSW_EBV_BIT); /* UM=0, CU2-CU0 = 000, EBV=1 */
    }
    else
    {
        *Os_ArchTopStkPtr = (1UL << OS_ARCH_PSW_EBV_BIT) | (1UL << OS_ARCH_PSW_UM_BIT); /* UM=1, CU2-CU0 = 000, EBV=1 */
    }
#else
    *Os_ArchTopStkPtr = (1UL << OS_ARCH_PSW_EBV_BIT); /* CU2-CU0 = 000, EBV=1 */ /* PRQA S 0499 */ /* VL_Os_0499 */
#endif

#if ((1U) == CFG_FPU_ENABLE)
    *Os_ArchTopStkPtr |= (1UL << OS_ARCH_PSW_FPU_BIT);                                  /* PSW.CU2-CU0 = 001 */
    *(--Os_ArchTopStkPtr) = (1UL << OS_ARCH_FPSR_FS_BIT); /* fpsr  */ /* PRQA S 3120 */ /* VL_QAC_MagicNum */
    *(--Os_ArchTopStkPtr) = 0x00000000U;                                               /* fpepc */
    *(--Os_ArchTopStkPtr) = 0x00000000U;                                               /* fpst  */
    *(--Os_ArchTopStkPtr) = 0x00000000U;                                               /* fpcc  */
    *(--Os_ArchTopStkPtr) = 0x00000000U;                                               /* fpcfg */
    *(--Os_ArchTopStkPtr) = 0x00000000U;                                               /* fpec  */
#endif
    /* PRQA S 3440, 0305, 3387, 0489 -- */

    /*save the new top-of-stack pointer*/
    Os_TaskCBExt[TaskID] = (uint32)Os_ArchTopStkPtr; /* PRQA S 0306 */ /* VL_Os_0306 */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The OS starts scheduling.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <void>
 * PreCondition         <None>
 * CallByAPI            <StartOS>
 */
/******************************************************************************/
/* PRQA S 1532, 3006 ++ */ /* VL_QAC_OneFunRef, VL_Os_3006 */
void Os_Hal_StartScheduler(void)
/* PRQA S 1532, 3006 -- */
{
    Os_ArchFirstDispatch(); /* PRQA S 1006*/ /* VL_Os_1006*/
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <OS_ArchGetSystemStackPointer>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 */
/*****************************************************************************/
void OS_ArchGetSystemStackPointer(void)
{
    /* PRQA S 0306, 3455 ++ */ /* VL_Os_0306, VL_Os_3455 */
    Os_ArchTempSp = OS_ARCH_STACK_ALIGN((uint32)(Os_SystemStack_Inf[Os_Hal_GetCoreID()]->StackTop));
    /* PRQA S 0306, 3455 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <OS_ArchGetTaskStackPointer>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Return               <>
 * PreCondition         <None>
 * CallByAPI            <>
 */
/*****************************************************************************/
void OS_ArchGetTaskStackPointer(void)
{
    Os_ArchTempSp = Os_TaskCBExt[Os_Kernel_GetSysRunningTaskID()];
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <OS_ArchSaveTaskStackPointer>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Return               <>
 * PreCondition         <None>
 * CallByAPI            <>
 */
/*****************************************************************************/
void OS_ArchSaveTaskStackPointer(void)
{
    Os_TaskCBExt[Os_Kernel_GetSysRunningTaskID()] = Os_ArchTempSp;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
/*=======[E N D   O F   F I L E]==============================================*/
