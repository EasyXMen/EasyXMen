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
**  FILENAME    : Os_Internal.h                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/
#ifndef OS_INTERNAL_H
#define OS_INTERNAL_H

/*=======[I N C L U D E S]====================================================*/
#include "Arch_Processor.h"
#include "Os_Hook.h"
#include "Os_Trace.h"
#include "Os_Monitor.h"

#ifdef OS_GLOBAL_VAR
#define OS_EXTERN
#else
#define OS_EXTERN extern
#endif

#define OS_KNL_INIT_DATA(KNL_INIT_DATA) KNL_INIT_DATA
#define OS_KNL_BSS(KNL_BSS)             KNL_BSS
#define OS_KNL_DATA(KNL_DATA)           KNL_DATA

/*=======[M A C R O S]========================================================*/
#define OS_EnableInterrupts()  Os_ArchEnableInt()       /* Enable interrupts. */
#define OS_DisableInterrupts() Os_ArchDisableInt()      /* Disable interrupts. */
/* PRQA S 3472 ++*/ /* MISRA  Dir-4.9*/                 /* OS_INTERNAL_MACRO_001 */
#define Os_Dispatch()         Os_ArchDispatch()         /* Run the scheduler. */
#define Os_Switch2System()                              /* Switch to the system stack. */
#define Os_InitCPU()          Os_ArchInitCPU()          /* Initialize the CPU. */
#define Os_PreSwitch2System() Os_ArchPreSwitch2System() /* Prepare to switch to the system stack. */
#define Os_StartScheduler()   Os_ArchStartScheduler()   /* schedule for the first time. */
/* PRQA S 3472 --*/                                     /* MISRA  Dir-4.9*/

#define OS_TMPROT_TARGET_ISR_OFFSET (2U)

#define CHECK_ID_INVALID(id, CfgMaxTable) \
    ((Os_GetObjCoreId(id) >= OS_AUTOSAR_CORES) || (Os_GetObjLocalId(id) >= CfgMaxTable[Os_GetObjCoreId(id)]))

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/
extern Os_CoreCBType volatile Os_CoreCB;

/*============================@+SPINLOCK+@====================================*/
#if (OS_AUTOSAR_CORES > 1U)
#define OS_START_SEC_VAR_CLEARED_GLOBAL_32
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(VAR(Os_SpinlockType, OS_VAR) Os_SpinlockSync);
#define OS_STOP_SEC_VAR_CLEARED_GLOBAL_32
#include "Os_MemMap.h"
#endif
/*============================@-SPINLOCK-@====================================*/

/*============================ @+KERNEL+@=====================================*/
/* PRQA S 0791++ */ /* MISRA Rule 5.4 */ /*OS_INTERNAL_SEGMENTNAME_SIMILAR_002*/
#define OS_START_SEC_VAR_CLONE_16
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(VAR(uint16, OS_VAR) Os_CfgPriorityMax);
#define OS_STOP_SEC_VAR_CLONE_16
#include "Os_MemMap.h"

/* PRQA S 3432 ++*/ /* MISRA Rule 20.7 */ /* OS_INTERNAL_COMPILER_001 */
#define OS_START_SEC_VAR_CLONE_16
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(P2VAR(Os_PriorityType, AUTOMATIC, OS_VAR) Os_ReadyMap);
#define OS_STOP_SEC_VAR_CLONE_16
#include "Os_MemMap.h"
/* PRQA S 3432 --*/ /* MISRA Rule 20.7 */

#if ((OS_BCC1 == CFG_CC) || (OS_ECC1 == CFG_CC))
#define OS_START_SEC_VAR_CLONE_16
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(P2VAR(Os_TaskType, AUTOMATIC, OS_VAR) Os_ReadyTable);
#define OS_STOP_SEC_VAR_CLONE_16
#include "Os_MemMap.h"
#else
/* PRQA S 3432 ++*/ /* MISRA Rule 20.7 */ /* OS_INTERNAL_COMPILER_001 */
#define OS_START_SEC_VAR_CLONE_UNSPECIFIED
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(P2VAR(Os_ReadyQueueType, AUTOMATIC, OS_VAR) Os_ReadyQueueMark);
#define OS_STOP_SEC_VAR_CLONE_UNSPECIFIED
#include "Os_MemMap.h"
/* PRQA S 3432 --*/ /* MISRA Rule 20.7 */

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(P2CONST(uint32, AUTOMATIC, OS_VAR) Os_ActivateQueueSize);
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(P2CONST(Os_TaskRefType, AUTOMATIC, OS_VAR) Os_ReadyQueue);
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
#endif /* OS_BCC1 == CFG_CC || OS_ECC1 == CFG_CC */

#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(P2CONST(Os_PriorityType, AUTOMATIC, OS_VAR) Os_PrioGroup);
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(P2CONST(Os_PriorityType, AUTOMATIC, OS_VAR) Os_PrioMask);
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(P2CONST(Os_StackType, AUTOMATIC, OS_VAR) Os_SystemStack);
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
/*===============================@-KERNEL-@===================================*/

/*===============================@+TASK+@=====================================*/
#if (CFG_TASK_MAX > 0U)
#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(P2CONST(Os_TaskCfgType, AUTOMATIC, OS_VAR) Os_TaskCfg);
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(P2CONST(Os_StackType, AUTOMATIC, OS_VAR) Os_TaskStack);
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

/* PRQA S 3432 ++*/ /* MISRA Rule 20.7 */ /* OS_INTERNAL_COMPILER_001 */
#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(P2VAR(Os_TCBType, AUTOMATIC, OS_VAR) Os_TCB);
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
/* PRQA S 3432 --*/ /* MISRA Rule 20.7 */

#define OS_START_SEC_VAR_CLONE_16
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(VAR(uint16, OS_VAR) OS_TASK_IDLE);
#define OS_STOP_SEC_VAR_CLONE_16
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_16
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(VAR(uint16, OS_VAR) Os_CfgExtendTaskMax);
#define OS_STOP_SEC_VAR_CLONE_16
#include "Os_MemMap.h"
#endif /* CFG_TASK_MAX > 0U */
/*===============================@-TASK-@=====================================*/

/*===============================@+ISR+@======================================*/
#if (CFG_ISR_MAX > 0U)
#define OS_START_SEC_VAR_CLONE_16
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(VAR(uint16, OS_VAR) Os_CfgIsrMax);
#define OS_STOP_SEC_VAR_CLONE_16
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_16
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(VAR(uint16, OS_VAR) Os_CfgIsr2Max);
#define OS_STOP_SEC_VAR_CLONE_16
#include "Os_MemMap.h"

/* PRQA S 3432 ++*/ /* MISRA Rule 20.7 */ /* OS_INTERNAL_COMPILER_001 */
#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(P2VAR(Os_ICBType, AUTOMATIC, OS_VAR) Os_ICB);
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
/* PRQA S 3432 --*/ /* MISRA Rule 20.7 */

#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(P2CONST(Os_StackType, AUTOMATIC, OS_VAR) Os_ISR2Stack);
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(P2CONST(Os_IsrCfgType, AUTOMATIC, OS_VAR) Os_IsrCfg);
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_16
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(VAR(Os_IsrType, OS_VAR) Os_IntCfgIsrId);
#define OS_STOP_SEC_VAR_CLONE_16
#include "Os_MemMap.h"
#endif /* CFG_ISR_MAX > 0U */

#define OS_START_SEC_VAR_CLONE_8
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(VAR(uint8, OS_VAR) Os_SuspendAllCount);
#define OS_STOP_SEC_VAR_CLONE_8
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_8
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(VAR(uint8, OS_VAR) Os_SuspendOsCount);
#define OS_STOP_SEC_VAR_CLONE_8
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_16
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(VAR(Os_IPLType, OS_VAR) Os_SaveOsIntNested);
#define OS_STOP_SEC_VAR_CLONE_16
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(VAR(uint8, OS_VAR) Os_IntNestISR1);
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(VAR(uint8, OS_VAR) Os_IntNestISR2);
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
/*===============================@-ISR-@======================================*/

/*===============================@+COUNTER+@==================================*/
#if (CFG_COUNTER_MAX > 0U)
#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(P2CONST(Os_CounterCfgType, AUTOMATIC, OS_VAR) Os_CounterCfg);
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

/* PRQA S 3432 ++*/ /* MISRA Rule 20.7 */ /* OS_INTERNAL_COMPILER_001 */
#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(P2VAR(Os_CCBType, AUTOMATIC, OS_VAR) Os_CCB);
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
/* PRQA S 3432 --*/ /* MISRA Rule 20.7 */
#endif              /* CFG_COUNTER_MAX > 0U */
/*===============================@-COUNTER-@==================================*/

/*===============================@+RESOURCE+@=================================*/
#if (CFG_STD_RESOURCE_MAX > 0U)
#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(P2CONST(Os_ResourceCfgType, AUTOMATIC, OS_VAR) Os_ResourceCfg);
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

/* PRQA S 3432 ++*/ /* MISRA Rule 20.7 */ /* OS_INTERNAL_COMPILER_001 */
#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(P2VAR(Os_RCBType, AUTOMATIC, OS_VAR) Os_RCB);
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
/* PRQA S 3432 --*/ /* MISRA Rule 20.7 */

#define OS_START_SEC_VAR_CLONE_16
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(VAR(uint16, OS_VAR) Os_CfgResourceMax);
#define OS_STOP_SEC_VAR_CLONE_16
#include "Os_MemMap.h"
#endif /* CFG_STD_RESOURCE_MAX > 0U */

#if (CFG_INTERNAL_RESOURCE_MAX > 0U)
#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(P2CONST(Os_PriorityType, AUTOMATIC, OS_VAR) Os_InterResCeiling);
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
#endif /* CFG_INTERNAL_RESOURCE_MAX > 0U */

#if (CFG_STD_RESOURCE_MAX > 0U)
#define OS_START_SEC_VAR_CLONE_16
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(VAR(Os_ResourceType, OS_VAR) RES_SCHEDULER);
#define OS_STOP_SEC_VAR_CLONE_16
#include "Os_MemMap.h"
#endif /* CFG_STD_RESOURCE_MAX > 0U */
/*===============================@-RESOURCE-@=================================*/

/*===============================@+OS-APP+@===================================*/
#if (CFG_OSAPPLICATION_MAX > 0U)
/* PRQA S 3432 ++*/ /* MISRA Rule 20.7 */ /* OS_INTERNAL_COMPILER_001 */
#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(P2CONST(P2VAR(Os_ObjectAppCfgType, AUTOMATIC, OS_VAR), AUTOMATIC, OS_VAR) Os_ObjectAppCfg);
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
/* PRQA S 3432 --*/ /* MISRA Rule 20.7 */
#endif              /*CFG_OSAPPLICATION_MAX >0U */
/*===============================@-OS-APP-@===================================*/

/*===============================@+SERVICE_PROTECTION+@=======================*/
#if (CFG_TRUSTED_SYSTEM_SERVICE_MAX > 0U)
/* PRQA S 3432 ++*/ /* MISRA Rule 20.7 */ /* OS_INTERNAL_COMPILER_001 */
#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(P2VAR(TrustedFunctionIndexType, AUTOMATIC, OS_VAR) Os_TrustedFuncNestQueue);
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
/* PRQA S 3432 --*/ /* MISRA Rule 20.7 */

#define OS_START_SEC_VAR_CLONE_8
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(VAR(uint8, OS_VAR) Os_TrustedFuncNest);
#define OS_STOP_SEC_VAR_CLONE_8
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_UNSPECIFIED
#include "Os_MemMap.h"
OS_EXTERN OS_KNL_BSS(VAR(Os_TrustedFuncTpDataDef, OS_VAR) Os_TrustedFuncTp);
#define OS_STOP_SEC_VAR_CLONE_UNSPECIFIED
#include "Os_MemMap.h"
#endif              /* CFG_TRUSTED_SYSTEM_SERVICE_MAX > 0U */
/* PRQA S 0791-- */ /* MISRA Rule 5.4 */
/*===============================@-SERVICE_PROTECTION-@=======================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*============================ @+HOOK+@=======================================*/
#if (TRUE == CFG_ERRORHOOK)
/******************************************************************************/
/*
 * Brief                <Call the error hook function>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <Error>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <TerminateApplication and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_CallErrorHook(StatusType Error);
#endif /* TRUE == CFG_ERRORHOOK */

#if (TRUE == CFG_PRETASKHOOK)
/********************************************************************/
/*
 * Brief                <Call the PreTask hook function>
 * Service ID           <none>
 * Sync/Async           <none>
 * Reentrancy           <none>
 * param-eventId[in]    <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_SwitchTask>
 * REQ ID               <None>
 *
 */
/********************************************************************/
extern FUNC(void, OS_CODE) Os_CallPreTaskHook(void);
#endif /* TRUE == CFG_PRETASKHOOK */

#if (TRUE == CFG_POSTTASKHOOK)
/******************************************************************************/
/*
 * Brief                <Call the Post Task hook function>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_CallPostTaskHook(void);
#endif /* TRUE == CFG_POSTTASKHOOK */

#if (TRUE == CFG_SHUTDOWNHOOK)
/******************************************************************************/
/*
 * Brief                <Call the Shutdown hook function>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <Error>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <ShutdownOS and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_CallShutdownHook(StatusType Error);
#endif /* TRUE == CFG_SHUTDOWNHOOK */

#if (TRUE == CFG_STARTUPHOOK)
/******************************************************************************/
/*
 * Brief                <Call the Startup hook function>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <StartOS>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_CallStartupHook(void);
#endif /* TRUE == CFG_STARTUPHOOK */

#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
/********************************************************************/
/*
 * Brief                <ProtectionHook called by OS.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <osErrType>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <WaitEvent>
 * REQ ID               <None>
 */
/********************************************************************/
extern FUNC(ProtectionReturnType, OS_CODE) Os_CallProtectionHook(StatusType osErrType, uint32 osWhoHook);
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC */

#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_APPL_SHUTDOWNHOOK)
/******************************************************************************/
/*
 * Brief                <Call Appl's Shutdown hook function>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <Fatalerror>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <ShutdownOS and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_ApplShutdownHook(StatusType Fatalerror);
#endif /* TRUE == CFG_APPL_SHUTDOWNHOOK */

#if (TRUE == CFG_APPL_STARTUPHOOK)
/******************************************************************************/
/*
 * Brief                <Call Appl's startup hook function>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <StartOS>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_ApplStartupHook(void);
#endif /* TRUE == CFG_APPL_STARTUPHOOK */
#endif /* OS_SC3 == CFG_SC || OS_SC4 == CFG_SC*/
/*============================ @-HOOK-@=======================================*/

/*============================ @+APPLICATION+@================================*/
#if (CFG_OSAPPLICATION_MAX > 0U)
/******************************************************************************/
/*
 * Brief                <Init the Application control block>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * PreCondition         <None>
 * CallByAPI            <Os_InitSystem>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_InitApplication(void);
#endif

#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
/******************************************************************************/
/*
 * Brief                <Internal implementation of OS service:TerminateApplication>
 * ServiceId            <OSServiceId_TerminateApplication>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Yes>
 * Param-Name[in]       <Application>
 *                      <RestartOption>
 * Return               <E_OS_CALLEVEL>
 *                      <E_OS_VALUE>
 * PreCondition         <None>
 * PreCondition         <SC3 and SC4>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_TerminateApplication(ApplicationType Application, RestartType RestartOption);
#endif /* OS_SC3 == CFG_SC || OS_SC4 == CFG_SC */
/*============================ @-APPLICATION-@================================*/

/*============================ @+RESOURCE+@===================================*/
#if (CFG_RESOURCE_MAX > 0U)
/******************************************************************************/
/*
 * Brief                <Init the resource control block>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <ResID>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_InitSystem>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_InitResource(void);

/******************************************************************************/
/*
 * Brief                <Internal implementation of OS service:GetResource>
 *                      ReleaseResource.>
 * Service ID           <OSServiceId_GetResource>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <ResID>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <StatusType>
 * PreCondition         <None>
 * CallByAPI            <task or ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(StatusType, OS_CODE) Os_GetResource(ResourceType ResID);

/******************************************************************************/
/*
 * Brief                <task or ISR release a resource occupied before>
 *                      <ReleaseResource is the counterpart of  GetResource and
 *                      serves to leave critical sections in the code that are
 *                      assigned to the resource referenced by <ResID>. > .
 * Service ID           <OSServiceId_ReleaseResource>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <ResID>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <StatusType>
 * PreCondition         <None>
 * CallByAPI            <task or ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(StatusType, OS_CODE) Os_ReleaseResource(ResourceType ResID);

/******************************************************************************/
/*
 * Brief                <get the internal resource >
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ArchExitISR2 and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_GetInternalResource(void);

/******************************************************************************/
/*
 * Brief                <Release the internal resource >
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <TerminateTask and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_ReleaseInternalResource(Os_TaskType osTaskId);
#endif /*CFG_RESOURCE_MAX > 0U*/
/*============================ @-RESOURCE-@===================================*/

/*============================ @+SPINLOCK+@===================================*/
#if (CFG_SPINLOCK_MAX > 0U)
/******************************************************************************/
/*
 * Brief                <Initialize the spin lock>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <Os_InitSystem>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_InitSpinlock(void);
/******************************************************************************/
/*
 * Brief                <TCheck if there are any outstanding spinlocks
 *                         when scheduling is required>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <TerminateTask and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(StatusType, OS_CODE) Os_SpinlockSafetyCheck(void);

/******************************************************************************/
/*
 * Brief                <Internal implementation of OS service:ReleaseSpinlock>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <SpinlockId>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_ReleaseSpinlock(SpinlockIdType SpinlockId);
#endif /* CFG_SPINLOCK_MAX > 0U */

#if ((OS_AUTOSAR_CORES > 1U) || (CFG_SPINLOCK_MAX > 0U))
/******************************************************************************/
/*
 * Brief                <The get spin lock function inside the OS>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <spinlock>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <ShutdownAllCores and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_GetInternalSpinlock(Os_SpinlockRefType spinlock);

/******************************************************************************/
/*
 * Brief                <The release spin lock function inside the OS>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <spinlock>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <ShutdownAllCores and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_ReleaseInternalSpinlock(Os_SpinlockRefType spinlock);
#endif /* OS_AUTOSAR_CORES > 1U ||CFG_SPINLOCK_MAX > 0U */
/*============================ @-SPINLOCK-@===================================*/

/*============================ @+MEMORY PROTECTION+@==========================*/
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
/******************************************************************************/
/*
 * Brief                <Internal implementation of OS service:CheckISRMemoryAccess>
 * Service ID           <CheckISRMemoryAccess>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-Name[in]       <ISRID>
 *                      <Address>
 *                      <Size>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return value         <AccessType>
 * Caveats              <None>
 * Configuration        <SC3 and SC4>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(AccessType, OS_CODE)
    Os_CheckISRMemoryAccess(ISRType ISRID, MemoryStartAddressType Address, MemorySizeType Size);

/******************************************************************************/
/*
 * Brief                <Internal implementation of OS service:CheckTaskMemoryAccess>
 * Service ID           <CheckTaskMemoryAccess>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-Name[in]       <TaskID>
 *                      <Address>
 *                      <Size>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return value         AccessType
 * Caveats              <None>
 * Configuration        <SC3 and SC4>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(AccessType, OS_CODE)
    Os_CheckTaskMemoryAccess(TaskType TaskID, MemoryStartAddressType Address, MemorySizeType Size);
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
/********************************************************************/
/*
 * Brief                <Init function by memory protection>
 * Service ID           <none>
 * Sync/Async           <none>
 * Reentrancy           <none>
 * param-eventId[in]    <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <StartOS>
 * REQ ID               <None>
 */
/********************************************************************/
extern FUNC(void, OS_CODE) Os_InitMemProt(void);
#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */
/*============================ @-MEMORY PROTECTION-@==========================*/

/*============================ @+KERNEL+@=====================================*/
/******************************************************************************/
/*
 * Brief                <pre entry highest priority task>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_SwitchTask(void);
/******************************************************************************/
/*
 * Brief                <Os_TaskErrBack>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <the priority value>
 * PreCondition         <None>
 * CallByAPI            <GetResource and so on >
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_TaskErrBack(void);
/******************************************************************************/
/*
 * Brief                <Initialize the multicore processor>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <StartOS>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_MultiCoreInitProcessor(void);
/********************************************************************/
/*
 * Brief                <Clear priority ready map>
 * Service ID           <none>
 * Sync/Async           <none>
 * Reentrancy           <none>
 * param-eventId[in]    <OsPrio>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * REQ ID               <None>
 *
 */
/********************************************************************/
extern FUNC(void, OS_CODE) Os_ClearPrioReadyMap(Os_PriorityType OsPrio);
/******************************************************************************/
/*
 * Brief                <get the highest priority>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <the highest priority>
 * PreCondition         <None>
 * CallByAPI            <TerminateTask and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(Os_PriorityType, OS_CODE) Os_GetHighPrio(void);
/******************************************************************************/
/*
 * Brief                <get the first task of a ready queue that the value of
 *                      priority is equal to prio>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <prio:the priority value of the ready queue >
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <the first task of a ready queue >
 * PreCondition         <None>
 * CallByAPI            <TerminateTask and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(Os_TaskType, OS_CODE) Os_ReadyQueueGetFirst(Os_PriorityType prio);
/******************************************************************************/
/*
 * Brief                <Internal implementation of OS service:ShutdownOS>
 * Service ID           <OSServiceId_ShutdownOS>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_ShutdownOS(StatusType Error, Os_ShutdownAction Action);
/******************************************************************************/
/*
 * Brief                <insert a priority to ready queue>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <object:the ID of task or resource, level: the call level,
 *                                                          prio:inserted priority>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <TerminateTask and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_ReadyQueueInsert(Os_TaskType object, Os_CallLevelType level, Os_PriorityType prio);
/******************************************************************************/
/*
 * Brief                <remove a priority from ready queue>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <object:the ID of task or resource, level: the call level,
 *                                                          prio:removed priority>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <TerminateTask and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_ReadyQueueRemove(Os_CallLevelType level, Os_PriorityType prio);

/*============================ @+LERNEL+@=====================================*/

/*============================ @+TASK+@=======================================*/
/******************************************************************************/
/*
 * Brief                <Init the task control block>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <Os_InitSystem>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_InitTask(void);
/******************************************************************************/
/*
 * Brief                <Internal implementation of OS service:ActivateTask>
 * Service ID           <OSServiceId_ActivateTask>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-TaskID[in]     <Reference to the task>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(StatusType, OS_CODE) Os_ActivateTask(TaskType TaskID);
/*============================ @+TASK+@=======================================*/

/*============================ @+Interrupt+@==========================*/
/******************************************************************************/
/*
 * Brief                <Init the Interrupt block control.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_InitSystem>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_InitInterrupt(void);
/******************************************************************************/
/*
 * Brief                <Internal implementation of OS service:EnableAllInterrupts>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_EnableAllInterrupts(void);
/******************************************************************************/
/*
 * Brief                <Internal implementation of OS service:SuspendAllInterrupts >
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_SuspendAllInterrupts(void);
/******************************************************************************/
/*
 * Brief                <Internal implementation of OS service:SuspendOSInterrupts>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_SuspendOSInterrupts(void);
/******************************************************************************/
/*
 * Brief                <Internal implementation of OS service:ResumeAllInterrupts>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_ResumeAllInterrupts(void);
/******************************************************************************/
/*
 * Brief                <Internal implementation of OS service:ResumeOSInterrupts>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_ResumeOSInterrupts(void);
/******************************************************************************/
/*
 * Brief                <enter ISR1>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_EnterISR1(void);
/******************************************************************************/
/*
 * Brief                <exit ISR1>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_ExitISR1(void);
/******************************************************************************/
/*
 * Brief                <enter ISR2>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_EnterISR2(void);
/******************************************************************************/
/*
 * Brief                <exit ISR2>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_ExitISR2(void);
/******************************************************************************/
/*
 * Brief                <make the priority to ipl>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <prio:the priority value of one os object>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <IPL>
 * PreCondition         <None>
 * CallByAPI            <Os_ArchSetIpl>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(Os_IPLType, OS_CODE) Os_PrioToIpl(Os_PriorityType prio);
/******************************************************************************/
/*
 * Brief                <make the ipl to priority >
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <ipl:interrupt priority level>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <the priority value>
 * PreCondition         <None>
 * CallByAPI            <GetResource and so on >
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(Os_PriorityType, OS_CODE) Os_IplToPrio(Os_IPLType ipl);
/*============================ @-Interrupt-@==========================*/

/*============================ @+Counter+@==========================*/
/******************************************************************************/
/*
 * Brief                <Init the counter control block>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_InitSystem>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_InitCounter(void);
/******************************************************************************/
/*
 * Brief                <Calculate the absolute ticks that baseTick add the
 *                       offset>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-baseTick[in]   <the begin point>
 * param-offset[in]     <the offset from begin point>
 * param-CounterID[in]  <counter id>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <Os_TickType>
 * PreCondition         <None>
 * CallByAPI            <SetRelAlarm and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(Os_TickType, OS_CODE) Os_CalcAbsTicks(Os_TickType baseTick, Os_TickType offset, Os_CounterType CounterID);
/******************************************************************************/
/*
 * Brief                <Get the distance from baseTick to destTick>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-baseTick[in]   <the begin point >
 * param-destTick[in]   <the end point>
 * param-CounterID[in]  <counter id>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <Os_TickType>
 * PreCondition         <None>
 * CallByAPI            <GetAlarm and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(Os_TickType, OS_CODE) Os_GetDistance(Os_TickType baseTick, Os_TickType destTick, Os_CounterType CounterID);
/******************************************************************************/
/*
 * Brief                <Increment Hardware counter>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-CounterID[in]  <reference to counter>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(StatusType, OS_CODE) Os_IncrementHardCounter(CounterType CounterID);
/******************************************************************************/
/*
 * Brief                    <Get elapsed counter value>
 * Service ID               <None>
 * Sync/Async               <Synchronous>
 * Reentrancy               <Reentrant>
 * param-CounterID[in]      <reference to counter>
 * Param-Value[out]         <current value of the counter>
 * Param-ElapsedValue[out]  <Elapsed value of the counter>
 * Param-Name[in/out]       <None>
 * return                   <None>
 * PreCondition             <None>
 * CallByAPI                <None>
 * REQ ID                   <None>
 */
/******************************************************************************/
extern FUNC(StatusType, OS_CODE) Os_GetElapsedValue(CounterType CounterID, TickRefType Value, TickRefType ElapsedValue);
/*============================ @-Counter-@==========================*/

/*============================ @+Alarm+@==========================*/
/******************************************************************************/
/*
 * Brief                <Init the alarm control block>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_InitSystem>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_InitAlarm(void);
/******************************************************************************/
/*
 * Brief                <Os_WorkAlarm>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-CounterID[in]  <reference to counter>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_WorkAlarm(Os_CounterType CounterID);
/******************************************************************************/
/*
 * Brief                <Internal implementation of OS service: CancelAlarm>
 * Service ID           <OSServiceId_CancelAlarm>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-AlarmID[in]    <reference to alarm>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(StatusType, OS_CODE) Os_CancelAlarm(AlarmType AlarmID);
/******************************************************************************/
/*
 * Brief                <The system service GetAlarmBase reads the alarm base
 *                       characteristics. The return value <Info> is a structure in which
 *                       the information of data type Os_AlarmBaseType is stored.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-AlarmID[in]    <reference to alarm>
 * Param-Info[out]      <Reference to structure with constants of the alarm base>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_GetAlarmBase(AlarmType AlarmID, AlarmBaseRefType Info);
/******************************************************************************/
/*
 * Brief                <The system service GetAlarm returns the relative value
 *                       in ticksbefore the alarm <AlarmID> expires>
 * Service ID           <>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-AlarmID[in]    <reference to alarm>
 * Param-Tick[out]      <Relative value in ticks before the alarm <AlarmID>
 *                       expires>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(StatusType, OS_CODE) Os_GetAlarm(AlarmType AlarmID, TickRefType Tick);
/*============================ @-Alarm-@==========================*/

/*============================ @+Event+@==========================*/
#if (CFG_EXTENDED_TASK_MAX > 0)
/******************************************************************************/
/*
 * Brief                <Init the Event control block>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <Event control block id>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_InitSystem>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_InitEvent(void);
/******************************************************************************/
/*
 * Brief                <The service may be called from an interrupt service routine and
 *                       from the task level, but not from hook routines.
 *                       The events of task <TaskID> are set according to the event
 *                       mask <Mask>. Calling SetEvent causes the task <TaskID> to
 *                       be transferred to the ready state, if it was waiting for at least
 *                       one of the events specified in <Mask>>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-TaskID[in]     <Reference to the task for which one or several events are to be set.>
 * param-Mask[in]       <Mask of the events to be set>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(StatusType, OS_CODE) Os_SetEvent(TaskType TaskID, EventMaskType Mask);
/******************************************************************************/
/*
 * Brief                <The state of the calling task is set to waiting, unless
 *                       at least one of the events specified in <Mask> has
 *                       already been set.
 *                       Particularities: This call enforces rescheduling,
 *                       if the wait condition occurs. If rescheduling takes
 *                       place, the internal resource of the task is released
 *                       while the task is in the waiting state.
 *                       This service must only be called from the extended task
 *                       owning the event.
 *                       The system service ClearEvent is restricted to extended
 *                       taskswhich own the event.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-Mask[in]       <Mask of the events to be set>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(StatusType, OS_CODE) Os_WaitEvent(EventMaskType Mask);
/******************************************************************************/
/*
 * Brief                <The state of the calling task is set to waiting, must
 *                       all the events specified in <Mask> has already been set.
 *                       Particularities: This call enforces rescheduling,
 *                       if the wait condition occurs. If rescheduling takes
 *                       place, the internal resource of the task is released
 *                       while the task is in the waiting state.
 *                       This service must only be called from the extended task
 *                       owning the event.
 *                       The system service ClearEvent is restricted to extended
 *                       taskswhich own the event.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-Mask[in]       <Mask of the events to be set>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(StatusType, OS_CODE) Os_WaitAllEvents(EventMaskType Mask);
#endif
/*============================ @-Event-@==========================*/

/*============================ @+Timing Protection+@==========================*/
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
/********************************************************************/
/*
 * Brief                <Init protection module.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <None>
 * param-eventId[in]    <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_InitSystem>
 * REQ ID               <None>
 */
/********************************************************************/
extern FUNC(void, OS_CODE) Os_InitTmProt(void);
/********************************************************************/
/*
 * Brief                <Timing protection start process of task.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <osTaskId, osOptType>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <DisableAllInterrupts>
 * REQ ID               <None>
 */
/********************************************************************/
extern FUNC(void, OS_CODE) Os_TmProtTaskStart(Os_TaskType osTaskId, Os_TmProtOptTaskType osOptType);
/********************************************************************/
/*
 * Brief                <Timing protection end process of task.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <osTaskId, osOptType>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ApplTerminateOneTask and so on>
 * REQ ID               <None>
 */
/********************************************************************/
extern FUNC(void, OS_CODE) Os_TmProtTaskEnd(Os_TaskType osTaskId, Os_TmProtOptTaskType osOptType);
/********************************************************************/
/*
 * Brief                <Time frame of task: frame check process.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <osTaskId>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <ActivateTask>
 * REQ ID               <None>
 */
/********************************************************************/
extern FUNC(StatusType, OS_CODE) Os_TmProtTaskFrameChk(Os_TaskType osTaskId);
/********************************************************************/
/*
 * Brief                <Timing Protection isr callback.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <None>
 * param-eventId[in]    <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <ISR>
 * REQ ID               <None>
 */
/********************************************************************/
extern FUNC(void, OS_CODE) Os_TmProtMainProc(void);
#if (CFG_STD_RESOURCE_MAX > 0)
/********************************************************************/
/*
 * Brief                <Start process for budget of resource locking.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <None>
 * param-eventId[in]    <osResId>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <GetResource>
 * REQ ID               <None>
 */
/********************************************************************/
extern FUNC(void, OS_CODE) Os_TmProtResStart(ResourceType osResId, Os_TmProtResOccupyType osOccupyType);
/********************************************************************/
/*
 * Brief                <End process for budget of resource locking.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <None>
 * param-eventId[in]    <osResId>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ApplReleaseResource>
 * REQ ID               <None>
 */
/********************************************************************/
extern FUNC(void, OS_CODE) Os_TmProtResEnd(ResourceType osResId);
#endif

#if (CFG_ISR2_MAX > 0)
/********************************************************************/
/*
 * Brief                <Timing protection start process of isr.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <osTaskId, osOptType>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <DisableAllInterrupts>
 * REQ ID               <None>
 */
/********************************************************************/
extern FUNC(void, OS_CODE) Os_TmProtIsrStart(Os_IsrType osIsrId, Os_TmProtOptIsrType osOptType);
/********************************************************************/
/*
 * Brief                <Timing protection end process of isr.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <osTaskId, osOptType>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_AppTerminateIsrKernelProc and so on>
 * REQ ID               <None>
 */
/********************************************************************/
extern FUNC(void, OS_CODE) Os_TmProtIsrEnd(Os_IsrType osIsrId, Os_TmProtOptIsrType osOptType);
/********************************************************************/
/*
 * Brief                <Time frame of task: frame check process.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <osTaskId>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/********************************************************************/
extern FUNC(StatusType, OS_CODE) Os_TmProtIsrFrameChk(Os_IsrType osIsrId);
#endif
#endif
/*============================ @-Timing Protection-@==========================*/

/*============================ @+Schedule table+@==========================*/
/********************************************************************/
/*
 * Brief                <Init ScheduleTable during os_startup.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_InitSystem>
 * REQ ID               <None>
 */
/********************************************************************/
extern FUNC(void, OS_CODE) Os_InitScheduleTable(void);
/********************************************************************/
/*
 * Brief                <Os_WorkSchedTbl.>
 * Service ID           <none>
 * Sync/Async           <none>
 * Reentrancy           <none>
 * param-eventId[in]    <CounterID>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_IncrementHardCounter and so on>
 * REQ ID               <None>
 */
/********************************************************************/
extern FUNC(void, OS_CODE) Os_WorkSchedTbl(Os_CounterType CounterID);
/********************************************************************/
/*
 * Brief                <Internal implementation of OS service:GetScheduleTableStatus>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Yes>
 * param-eventId[in]    <ScheduleTableID, ScheduleStatus>
 * Param-Name[out]      <StatusType>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * REQ ID               <None>
 */
/********************************************************************/
extern FUNC(void, OS_CODE)
    Os_GetScheduleTableStatus(ScheduleTableType ScheduleTableID, ScheduleTableStatusRefType ScheduleStatus);
/********************************************************************/
/*
 * Brief                <Internal implementation of OS service:StartScheduleTableRel.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <ScheduleTableID, Offset>
 * Param-Name[out]      <StatusType>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * REQ ID               <None>
 */
/********************************************************************/
extern FUNC(StatusType, OS_CODE) Os_StartScheduleTableRel(ScheduleTableType ScheduleTableID, TickType Offset);
/********************************************************************/
/*
 * Brief                <Internal implementation of OS service:StartScheduleTableAbs.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <ScheduleTableID, Start>
 * Param-Name[out]      <StatusType>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * REQ ID               <None>
 */
/********************************************************************/
extern FUNC(StatusType, OS_CODE) Os_StartScheduleTableAbs(ScheduleTableType ScheduleTableID, TickType Start);
/********************************************************************/
/*
 * Brief                <Internal implementation of OS service:StopScheduleTable.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <ScheduleTableID, Start>
 * Param-Name[out]      <StatusType>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * REQ ID               <None>
 */
/********************************************************************/
extern FUNC(StatusType, OS_CODE) Os_StopScheduleTable(ScheduleTableType ScheduleTableID);
/*============================ @-Schedule table-@==========================*/

/*============================ @+Service protection+@==========================*/
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
/********************************************************************/
/*
 * Brief                <If interrupts are disabled/suspended, OS service should ignore>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <None>
 * param-eventId[in]    <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <E_OS_DISABLEDINT>
 * PreCondition         <None>
 * CallByAPI            <GetAlarm and so on>
 * REQ ID               <None>
 */
/********************************************************************/
extern FUNC(boolean, OS_CODE) Os_IgnoreService(void);
/********************************************************************/
/*
 * Brief                <Indicate the address if writable by this OS-App>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <None>
 * param-eventId[in]    <Address>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <E_OS_ILLEGAL_ADDRESS>
 * PreCondition         <None>
 * CallByAPI            <GetAlarm and so on>
 * REQ ID               <None>
 */
/********************************************************************/
extern FUNC(boolean, OS_CODE) Os_AddressWritable(VAR(uint32, OS_VAR) Address);
/********************************************************************/
/*
 * Brief                <Indicate the service if in right context>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <None>
 * param-eventId[in]    <AllowedContext>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <E_OS_DISABLEDINT>
 * PreCondition         <None>
 * CallByAPI            <GetAlarm and so on>
 * REQ ID               <None>
 */
/********************************************************************/
extern FUNC(boolean, OS_CODE) Os_WrongContext(uint16 AllowedContext);
/********************************************************************/
/*
 * Brief                <Task ends without calling a TerminateTask or ChainTask>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <None>
 * param-eventId[in]    <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * PreCondition         <None>
 * CallByAPI            <Os_TaskErrBack>
 * REQ ID               <None>
 */
/********************************************************************/
extern FUNC(void, OS_CODE) Os_TaskEndNoTerminate(void);
/********************************************************************/
/*
 * Brief                <ISR2 ends with locked interrupts or allocated
 *                       resources>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <None>
 * param-eventId[in]    <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/********************************************************************/
extern FUNC(void, OS_CODE) Os_Isr2OccupyIntRes(void);
/********************************************************************/
/*
 * Brief                <Check object access to the calling Task/Isr2>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <None>
 * param-eventId[in]    <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * PreCondition         <None>
 * CallByAPI            <GetAlarm>
 * REQ ID               <None>
 */
/********************************************************************/
extern FUNC(boolean, OS_CODE) Os_CheckObjAcs(ObjectTypeType ObjectType, Os_AppObjectId ObjectID);
#endif
#endif
/******************************************************************************/
/*
 * Brief                <Internal implementation of OS service:CheckObjectAccess>
 * Service ID           <OSServiceId_CheckObjectAccess>
 * Sync/Async           <Synchronous>
 * Reentrancy           < Reentrant>
 * param-Name[in]       <ApplID>
 *                      <ObjectType>
 *                      <ObjectID>
 * Return value         <ACCESS>
 *                      <NO_ACCESS>
 * PreCondition         <SC3 and SC4>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(ObjectAccessType, OS_CODE)
    Os_CheckObjectAccess(ApplicationType ApplID, ObjectTypeType ObjectType, AppObjectId ObjectID);
/*============================ @-Service protection-@==========================*/

/*============================ @+RPC+@==========================*/
#if (OS_AUTOSAR_CORES > 1)
/******************************************************************************/
/*
 * Brief                <Init the RPC module>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_RpcAddService>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_InitRpc(void);
#endif
/*============================ @+RPC+@==========================*/

/*============================ @+Trusted Function+@==========================*/
#if (CFG_TRUSTED_SYSTEM_SERVICE_MAX > 0U)
/******************************************************************************/
/*
 * Brief                <Init the TrustedFunction>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * PreCondition         <None>
 * CallByAPI            <Os_InitSystem>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_InitTrustedFunction(void);
#endif
/*============================ @-Trusted Function-@==========================*/

/* MultiCore. */
/******************************************************************************/
/*
 * Brief                <The function returns a unique logical Core ID .>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-phyCoreId[in]  <A valid physical Core ID defined by the AUTOSAR OS.>
 * return               <CoreIdType, The return value is the unique logical
 *                       CoreID map to the input physical core.>
 * PreCondition         <None>
 * CallByAPI            <StartCore>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(CoreIdType, OS_CODE) Os_GetCoreLogID(CoreIdType phyCoreId);
/******************************************************************************/
/*
 * Brief                <The function returns a unique physical Core ID .>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-logCoreId[in]  <A valid logical Core ID from GetCoreID,Os_GetCoreLogID.>
 * return               <CoreIdType, The return value is the unique physical
 *                       CoreID map to the input logical core.>
 * PreCondition         <None>
 * CallByAPI            <Os_InitSystem>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(CoreIdType, OS_CODE) Os_GetCorePhyID(CoreIdType logCoreId);

#if (OS_AUTOSAR_CORES > 1)
/******************************************************************************/
/*
 * Brief                <Make a remote service call to given core with
 *                       given service ID and input parameters>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <Os_RpcGetService>, <Os_RpcProcessParameter>
 *                      <Os_RpcAddService>, <Os_ArchRemoteCall>
 *                      <Os_RpcWaitResult>, <Os_RpcProcessResult>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(StatusType, OS_CODE) Os_RpcCallService(RpcInputRefType rpcData);
/******************************************************************************/
/*
 * Brief                <Process all received RPC service request on the
                         core given osCoreId>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_RpcGetService>, <Os_RpcServiceAction>
 *                      <Os_RpcAddService>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_RpcServiceOperation(Os_CoreIdType osCoreId);
#endif
/******************************************************************************/
/*
 * Brief                <Os panic>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_Panic(void);

/******************************************************************************/
/*
 * Brief                < Clear the CoreCB info. >
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_DeInitCoreInfo(void);

/*============================ @+IOC+@=====================================*/
#if (CFG_IOC_MAX > 0u)
/******************************************************************************/
/*
 * Brief                <Init the IOC control block list>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_InitSystem>
 * REQ ID               <>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_InitIoc(void);

/******************************************************************************/
/*
 * Brief                <In case of queued communication identified by the <IocId>
 *                       in the function name, the content of the IOC internal
 *                       communication queue shall be deleted.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <>
 */
/******************************************************************************/
extern FUNC(StatusType, OS_CODE) Os_IocEmpty(Os_IocComIdType comId);

/******************************************************************************/
/*
 * Brief                <>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_InitSystem>
 * REQ ID               <>
 */
/******************************************************************************/
extern FUNC(StatusType, OS_CODE) Os_IocTransmit(
    Os_IocComIdType comId,
    Os_IocSendDataSourceType* pData,
    Os_IocU16Type paraNum,
    Os_IocU16Type senderId);

/******************************************************************************/
/*
 * Brief                <>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_InitSystem>
 * REQ ID               <>
 */
/******************************************************************************/
extern FUNC(StatusType, OS_CODE)
    Os_IocReceive(Os_IocComIdType comId, Os_IocReceiveDataSourceType* pData, Os_IocU16Type paraNum);

/******************************************************************************/
/*
 * Brief                <IOC cross-core remote callback function>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(StatusType, OS_CODE)
Os_IocRemoteCallBack(Os_IocComIdType comId, Os_IocU16Type vReceiverId, Os_ApplicationType vRecAppId);
#endif
/*============================ @-IOC-@=====================================*/

#if (TRUE == CFG_STACK_CHECK)
/******************************************************************************/
/*
 * Brief                <Os_StackMonitor>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_StackMonitor(Os_StackPtrType pStack);
/******************************************************************************/
/*
 * Brief                <fill the stack of task or system with the value of 0xCC>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <stack:the stack of task or system>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_InitSystem>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_FillStack(Os_StackType stack);
/******************************************************************************/
/*
 * Brief                <init the system stack with the value of 0xcc>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <StartOS>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_InitSystemStack(void);
#endif
/******************************************************************************/
/*
 * Brief                <StartOS synchronizes all cores twice. The first
 *                         synchronization point is located before the StartupHooks
 *                         are executed, the second after the OS-Application specific
 *                         StartupHooks have finished and before the scheduler is started.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <StartOS>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_SynPoint(uint8 point);

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
/******************************************************************************/
/*
 * Brief                <Cat1 MemMap for Memory Protection Settings Task>
 *
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <>
 */
/******************************************************************************/
extern void Os_MemProtTaskCat1Map(void);

/******************************************************************************/
/*
 * Brief                <Cat2 MemMap for Memory Protection Settings Task>
 *
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <>
 */
/******************************************************************************/
extern void Os_MemProtTaskCat2Map(void);

/******************************************************************************/
/*
 * Brief                <MemMap for Memory Protection Settings ISR>
 *
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <>
 */
/******************************************************************************/
extern void Os_MemProtIsrMap(void);
#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */

#if (CFG_TASK_MAX > 0U)
/******************************************************************************/
/*
 * Brief               <Internal implementation of OS service:GetTaskState>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_GetTaskState(TaskType TaskID, TaskStateRefType State);
#endif /* CFG_TASK_MAX > 0U */

#if (CFG_ALARM_MAX > 0U)
/******************************************************************************/
/*
 * Brief               <Internal implementation of OS service:SetRelAlarm>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
extern FUNC(StatusType, OS_CODE) Os_SetRelAlarm(AlarmType AlarmID, TickType increment, TickType cycle);

/******************************************************************************/
/*
 * Brief               <Internal implementation of OS service:SetAbsAlarm>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
extern FUNC(StatusType, OS_CODE) Os_SetAbsAlarm(AlarmType AlarmID, TickType start, TickType cycle);
#endif /* CFG_ALARM_MAX > 0U */

#if (CFG_COUNTER_MAX > 0U)
/******************************************************************************/
/*
 * Brief               Internal implementation of OS service:GetCounterValue
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_GetCounterValue(CounterType CounterID, TickRefType Value);
#endif /* CFG_COUNTER_MAX > 0U */

#endif /* OS_INTERNAL_H */
/*=======[E N D   O F   F I L E]==============================================*/
