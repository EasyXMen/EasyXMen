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
/* PRQA S 3108-- */

/*=======[I N C L U D E S]====================================================*/
#include "Os_Arch_Processor.h"
#include "Os_Cfg.h"
#include "Os_Task_CfgData.h"
#include "Os_Appl_CfgData.h"
#include "Os_Mprot_CfgData.h"
#include "Os_Sprot.h"
#include "Os_Cfg_S.h"
#include <v800_ghs.h>
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
/* PRQA S 3432 ++ */ /* VL_Os_3432 */
/*Save the task context stack pointer address.*/
Os_TaskCBExtType Os_TaskCBExt[CFG_TASK_MAX];
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
    Os_ArchTopStkPtr      = Os_ArchTopStkPtr - OS_PROGRAM_REGS_NUM; /* PRQA S 0488 */ /*VL_Os_0488 */
    *(--Os_ArchTopStkPtr) = (uint32)Os_TaskErrBack; /*LP*/ /* PRQA S 0428 */          /*VL_Os_0428 */
    *(--Os_ArchTopStkPtr) = (uint32)Os_TaskCfg[TaskID].TaskEntry;                     /*PC*/

    /* PSW */
    Os_ArchTopStkPtr = Os_ArchTopStkPtr - 1U;
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    if (((boolean)TRUE == Os_AppCfg[Os_Kernel_GetSysRunningAppID()].Trusted)
        && ((boolean)TRUE != Os_AppCfg[Os_Kernel_GetSysRunningAppID()].TrustedAppWithProtection))
    {
        *Os_ArchTopStkPtr = (1UL << OS_PSW_EBV_BIT); /* UM=0, CU2-CU0 = 000, EBV=1 */
    }
    else
    {
        *Os_ArchTopStkPtr = (1UL << OS_PSW_EBV_BIT) | (1UL << OS_PSW_UM_BIT); /* UM=1, CU2-CU0 = 000, EBV=1 */
    }
#else
    *Os_ArchTopStkPtr = (1U << OS_PSW_EBV_BIT); /* CU2-CU0 = 000, EBV=1 */
#endif

#if ((1U) == CFG_FPU_ENABLE)
    *Os_ArchTopStkPtr |= (1U << OS_PSW_FPU_BIT);                                  /* PSW.CU2-CU0 = 001 */
    *(--Os_ArchTopStkPtr) = (1U << OS_FPSR_FS_BIT); /* fpsr  */ /* PRQA S 3120 */ /* VL_Os_3120 */
    *(--Os_ArchTopStkPtr) = 0x00000000U;                                          /* fpepc */
    *(--Os_ArchTopStkPtr) = 0x00000000U;                                          /* fpst  */
    *(--Os_ArchTopStkPtr) = 0x00000000U;                                          /* fpcc  */
    *(--Os_ArchTopStkPtr) = 0x00000000U;                                          /* fpcfg */
    *(--Os_ArchTopStkPtr) = 0x00000000U;                                          /* fpec  */
#endif
    /* PRQA S 3440, 0305, 3387, 0489 ++ */

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
 * Brief                <OS_GetSystemStackPointer>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 */
/*****************************************************************************/
void OS_GetSystemStackPointer(void)
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
 * Brief                <OS_GetTaskStackPointer>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Return               <>
 * PreCondition         <None>
 * CallByAPI            <>
 */
/*****************************************************************************/
void OS_GetTaskStackPointer(void)
{
    Os_ArchTempSp = Os_TaskCBExt[Os_Kernel_GetSysRunningTaskID()];
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <OS_SaveTaskStackPointer>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Return               <>
 * PreCondition         <None>
 * CallByAPI            <>
 */
/*****************************************************************************/
void OS_SaveTaskStackPointer(void)
{
    Os_TaskCBExt[Os_Kernel_GetSysRunningTaskID()] = Os_ArchTempSp;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                  <OS_ShutdownAllCores is not provide user>
 *
 * Service ID             <None>
 * Sync/Async             <Synchronous>
 * Reentrancy             <Reentrant>
 * param-address[in]      <Pointer to the address>
 * param-compareVal[in]   <The value to compare with the value at the address>
 * param-exchangedVal[in] <The value to replace the value at the address>
 * param-Name[out]        <None>
 * param-Name[in/out]     <None>
 * return                 <None>
 * CallByAPI              <GetSpinlock and so on>
 * REQ ID                 <None>
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
uint32 Os_Hal_CmpSwapW(uint32* address, uint32 compareVal, uint32 exchangedVal)
/* PRQA S 1532 -- */
{
    uint32 ret;
    ret = (uint32)__CAXI2((void*)address, (sint32)compareVal, (sint32)exchangedVal); /* PRQA S 0317, 0314 */  /* VL_QAC_0317 */ /* VL_Os_0314 */

    return (ret == 0UL) ? 1UL : 0UL; 
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
/* PRQA S 6012 ++ */
/*=======[E N D   O F   F I L E]==============================================*/
