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
**  FILENAME    : Os_Kernel.c                                                 **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : code about priority processing and ready queue              **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Os_Internal.h"

/*=======[V E R S I O N  C H E C K]===========================================*/
/* DD_1_0162, DD_1_0163, DD_1_0164, DD_1_0165, DD_1_0166, DD_1_0167 */
#if (19U != OS_CFG_H_AR_MAJOR_VERSION)
#error "Os_Kernel.c:Mismatch with OS_CFG_H_AR_MAJOR_VERSION"
#endif /* 4U != OS_CFG_H_AR_MAJOR_VERSION */
#if (11U != OS_CFG_H_AR_MINOR_VERSION)
#error "Os_Kernel.c:Mismatch with OS_CFG_H_AR_MINOR_VERSION"
#endif /* 2U != OS_CFG_H_AR_MINOR_VERSION */
#if (0U != OS_CFG_H_AR_PATCH_VERSION)
#error "Os_Kernel.c:Mismatch with OS_CFG_H_AR_PATCH_VERSION"
#endif /* 2U != OS_CFG_H_AR_PATCH_VERSION */
#if (2U != OS_CFG_H_SW_MAJOR_VERSION)
#error "Os_Kernel.c:Mismatch with OS_CFG_H_SW_MAJOR_VERSION"
#endif /* 1U != OS_CFG_H_SW_MAJOR_VERSION */
#if (0U != OS_CFG_H_SW_MINOR_VERSION)
#error "Os_Kernel.c:Mismatch with OS_CFG_H_SW_MINOR_VERSION"
#endif /* 0U != OS_CFG_H_SW_MINOR_VERSION */
#if (0U != OS_CFG_H_SW_PATCH_VERSION)
#error "Os_Kernel.c:Mismatch with OS_CFG_H_SW_PATCH_VERSION"
#endif /* 0U != OS_CFG_H_SW_PATCH_VERSION */

/*=======[M A C R O S]========================================================*/
#define NUM_PRIORITYBITS_PERWORD 16u /* Priority calculation parameters. */

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/
/* PRQA S 0791++ */ /* MISRA Rule 5.4 */ /*OS_KERNEL_SEGMENTNAME_SIMILAR_004*/
#define OS_START_SEC_VAR_CLONE_16
#include "Os_MemMap.h"
static VAR(uint16, OS_VAR) Os_ReadyMapSize;
#define OS_STOP_SEC_VAR_CLONE_16
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_8
#include "Os_MemMap.h"
static VAR(uint8, OS_VAR) Os_CfgPriorityGroup;
#define OS_STOP_SEC_VAR_CLONE_8
#include "Os_MemMap.h"
/* PRQA S 0791-- */ /* MISRA Rule 5.4 */
/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static FUNC(void, OS_CODE) Os_InitSystem(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static FUNC(void, OS_CODE) Os_InitReadyTable(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static FUNC(Os_PriorityType, OS_CODE) Os_GetHighPrioBit(Os_PriorityType HighPriReadyTaskInQueue);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static FUNC(void, OS_CODE) Os_MultiCoreInitKernel(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static FUNC(Os_AppModeType, OS_CODE) Os_CheckAppMode(Os_AppModeType mode);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The user can call this  system service to start the
 *                     operating system in a specific mode.>
 * ServiceId           <0xf6>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]      <Mode: application mode>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return              <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3006 ++ */ /* MISRA Dir 4.3 */ /* OS_KERNEL_C_MIX_ASM_001 */
FUNC(void, OS_CODE) StartOS(AppModeType Mode)
/* PRQA S 3006 -- */ /* MISRA Dir 4.3 */
{
    OS_ENTER_KERNEL();
    VAR(StatusType, OS_VAR) err = E_OK;

    /* Get the unique logical CoreID.The operation must
     * be in the front, Will be used in the process of
     * the system is running. */
    Os_SCB.sysCore = Os_GetCoreLogID(Os_ArchGetCoreID());
    /* The Core is Non AUTOSAR Core. */
    if (OS_CORE_INVALID == Os_SCB.sysCore)
    {
        err = E_NOT_OK;
    }
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    else if (Os_SCB.sysOsLevel != OS_LEVEL_MAIN)
    {
        err = E_NOT_OK;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        err = E_NOT_OK;
    }
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    else
    {
        /* 01. must disable all interrupts during start os */
        OS_DisableInterrupts(); /* PRQA S 3469 */ /* MISRA  Dir-4.9 */ /* OS_KERNEL_MACRO_004 */

        /* 02. Check application mode. */
        Os_SCB.sysActiveAppMode = Os_CheckAppMode(Mode);

        /* 03. Init processor. */
        Os_MultiCoreInitProcessor();

        /* 04. Init kernel. */
        Os_MultiCoreInitKernel();

#if (TRUE == CFG_TRACE_ENABLE)
        Os_InitTrace();
#endif

/* 05. init system stack and switch to system stack */
#if (TRUE == CFG_STACK_CHECK)
        Os_InitSystemStack();
#endif

        /* 06. switch system stack */
        Os_PreSwitch2System(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_KERNEL_MACRO_004 */

        Os_Switch2System(); /* PRQA S 0306 */ /* MISRA Rule 11.4 */ /* OS_KERNEL_TYPE_CAST_002 */

        /* 07. Init os. */
        Os_InitSystem();
        /* 08. Init OS CPU arch. */
        Os_InitCPU(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_KERNEL_MACRO_004 */

/* 09. Init IOC. */
#if (CFG_IOC_MAX > 0U)
        Os_InitIoc();
#endif

/* 010. Init memory protection. */
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
        Os_InitMemProt();
        Os_ArchMemProtEnable();
#endif

        /* 011. synchronize before the global StartupHook-<SWS_Os_00580>. */
        Os_SynPoint(1);

        /* 012. Startup hook. */
        Os_StartupHook(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_KERNEL_MACRO_004 */

/* 013. Application specific hook. */
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_APPL_STARTUPHOOK)
        Os_ApplStartupHook();
#endif
#endif

        /* 014. synchronize after the global StartupHook-<SWS_Os_00579>. */
        Os_SynPoint(2);

        /* 015. Task Schedule. */
        OS_START_DISPATCH();
        Os_StartScheduler(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_KERNEL_MACRO_004 */
    }
    UNUSED_PARAMETER(err);
    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_KERNEL_MACRO_004 */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
FUNC(void, OS_CODE) Os_ShutdownOS(StatusType Error, Os_ShutdownAction Action)
{
#if (CFG_SPINLOCK_MAX > 0U)
    VAR(Os_TaskType, OS_VAR) i;
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_KERNEL_COMPILER_003 */
    /* PRQA S 3678 ++ */ /* MISRA Rule-8.13*/  /*OS_KERNEL_CONST_005*/
    P2VAR(Os_TCBType, AUTOMATIC, OS_VAR) pTCB;
    P2VAR(Os_ICBType, AUTOMATIC, OS_VAR) pICB;
    /* PRQA S 3678 -- */ /* MISRA Rule-8.13*/
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    VAR(SpinlockIdType, OS_VAR) SpinlockId;
    VAR(Os_SpinlockType, OS_VAR) spinLockIdx;
#endif /* CFG_SPINLOCK_MAX > 0U */

/* SWS_Os_00586: Application specific hook. */
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_APPL_SHUTDOWNHOOK)
    Os_ApplShutdownHook(Error);
#endif
#endif

#if (OS_AUTOSAR_CORES > 1)
    if (SHUTDOWN_ALL_OS == Action)
    {
        Os_SynPoint(3);
    }
#endif

    OS_DisableInterrupts(); /* PRQA S 3469 */ /* MISRA  Dir-4.9 */ /* OS_KERNEL_MACRO_004 */
/* SWS_Os_00588: Global ShutdownHook. */
#if (TRUE == CFG_SHUTDOWNHOOK)
    Os_CallShutdownHook(Error);
#endif

/*Os_00620*/
#if (CFG_SPINLOCK_MAX > 0U)
    for (i = 0; i < (Os_SCB.sysTaskMax - 1u); i++)
    {
        pTCB = &Os_TCB[i];
        for (spinLockIdx = pTCB->taskCriticalZoneCount; spinLockIdx > 0u; spinLockIdx--)
        {
            if (OBJECT_SPINLOCK == pTCB->taskCriticalZoneType[pTCB->taskCriticalZoneCount - 1u])
            {
                SpinlockId = pTCB->taskCriticalZoneStack[pTCB->taskCriticalZoneCount - 1u];
                (void)Os_ReleaseSpinlock(SpinlockId);
            }
            else
            {
                pTCB->taskCriticalZoneCount--;
            }
        }
    }
    for (i = 0; i < Os_CfgIsr2Max; i++)
    {
        pICB = &Os_ICB[i];
        for (spinLockIdx = pICB->isr2CriticalZoneCount; spinLockIdx > 0u; spinLockIdx--)
        {
            if (OBJECT_SPINLOCK == pICB->isr2CriticalZoneType[pICB->isr2CriticalZoneCount - 1u])
            {
                SpinlockId = pICB->isr2CriticalZoneStack[pICB->isr2CriticalZoneCount - 1u];
                (void)Os_ReleaseSpinlock(SpinlockId);
            }
            else
            {
                pICB->isr2CriticalZoneCount--;
            }
        }
    }
#endif /* CFG_SPINLOCK_MAX > 0U */
    Os_DeInitCoreInfo();
    UNUSED_PARAMETER(Error);
    UNUSED_PARAMETER(Action);
    /* SWS_Os_00715*/
    for (;;)
    {
        /* Nothing to do. */
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <terminate the os,the system will enter the background program>
 * Service ID           <0xf7>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ErrorHook>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) ShutdownOS(StatusType Error)
{
    OS_ENTER_KERNEL();

    StatusType err = E_OK;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_ShutdownOS);
#endif /* TRUE == CFG_TRACE_ENABLE */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    if (Os_WrongContext(OS_CONTEXT_SHUTDOWN_OS) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        err = E_OS_DISABLEDINT;
    }
    /*OS054*/
    else if (FALSE == Os_AppCfg[Os_SCB.sysRunningAppID].OsTrusted)
    {
        err = E_OS_CALLEVEL;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE*/
    {
        Os_ShutdownOS(Error, SHUTDOWN_OS);
    }

#if (CFG_ERRORHOOK == TRUE)
    if (E_OK != err)
    {
        Os_TraceErrorHook(OSError_Save_ShutDownOs(Error), OSServiceId_ShutdownOS, err);
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_ShutdownOS);
#endif /* TRUE == CFG_TRACE_ENABLE */

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_KERNEL_MACRO_004 */
    UNUSED_PARAMETER(err);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <get the active application mode of os>
 * Service ID           <0xf5>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <the active application mode>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(AppModeType, OS_CODE) GetActiveApplicationMode(void)
{
    OS_ENTER_KERNEL();
    VAR(AppModeType, OS_VAR) OsAppMode;

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    if (Os_WrongContext(OS_CONTEXT_GET_ACTIV_APPLICATION_MODE) != TRUE)
    {
        OsAppMode = OS_NULL_APPMODE;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        OsAppMode = OS_NULL_APPMODE;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
        OsAppMode = Os_SCB.sysActiveAppMode;
    }

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_KERNEL_MACRO_004 */
    return OsAppMode;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <init the os module>
 * Service ID:          <None>
 * Sync/Async:          <Synchronous>
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
static FUNC(void, OS_CODE) Os_InitSystem(void)
{
    /*01.Init Os_SCB */
    Os_SCB.sysDispatchLocker = 0U;
    Os_SCB.sysOsLevel = OS_LEVEL_TASK;
    Os_SCB.sysHighPrio = OS_PRIORITY_INVALID;
    Os_SCB.sysHighTaskID = OS_TASK_INVALID;
    Os_SCB.sysRunningTaskID = OS_TASK_INVALID;
    Os_SCB.sysRunningTCB = NULL_PTR;

#if (CFG_ISR2_MAX > 0)
    Os_SCB.sysInIsrCat2 = FALSE;
    Os_SCB.sysRunningIsrCat2Id = INVALID_ISR;
#endif

#if (CFG_OSAPPLICATION_MAX > 0)
    Os_SCB.sysRunningAppObj = OBJECT_MAX;
    Os_SCB.sysRunningAppID = INVALID_OSAPPLICATION;
#endif

    /*02.Init Os_CoreCB, OS_CORE_ID_MASTER is auto start, don't by means of
     * StartCore.*/
    if (OS_CORE_ID_MASTER == Os_SCB.sysCore)
    {
        Os_CoreCB.coreStateActive |= (uint8)(1u << OS_CORE_ID_MASTER);
        Os_CoreCB.coreStatus[OS_CORE_ID_MASTER] = OS_RUN;
    }

    /* 03.Init ready table */
    Os_InitReadyTable();

/* 04.Init TCB */
#if (CFG_TASK_MAX > 0U)
    Os_InitTask();
#endif

/* 05.Init ICB */
#if (CFG_ISR2_MAX > 0)
    Os_InitInterrupt();
#endif

/* 06.Init Counter */
#if (CFG_COUNTER_MAX > 0U)
    Os_InitCounter();
#endif

/* 07.Init Alarm */
#if (CFG_ALARM_MAX > 0U)
    Os_InitAlarm();
#endif

/* 08.Init Event */
#if (CFG_EXTENDED_TASK_MAX > 0)
    Os_InitEvent();
#endif

/* 09.Init Resource */
#if (CFG_RESOURCE_MAX > 0)
    Os_InitResource();
#endif

/* 10.Init schedule table. */
#if (CFG_SCHEDTBL_MAX > 0)
    Os_InitScheduleTable();
#endif

/* 11.Init protection. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    Os_InitTmProt();
#endif

/* 12.Init spinlock*/
#if (CFG_SPINLOCK_MAX > 0U)
    Os_InitSpinlock();
#endif

/* 13.Init application. */
#if (CFG_OSAPPLICATION_MAX > 0U)
    Os_InitApplication();
#endif

/* 14.Init TrustedFunction. */
#if (CFG_TRUSTED_SYSTEM_SERVICE_MAX > 0U)
    Os_InitTrustedFunction();
#endif

/* 15.Init RPC. */
#if (OS_AUTOSAR_CORES > 1)
    Os_InitRpc();
#endif

/* 16.Init OS monitor. */
#if (TRUE == CFG_OS_MONITOR_ENABLE)
    Os_InitOsMonitor(Os_SCB.sysCore);
#endif
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
FUNC(Os_PriorityType, OS_CODE) Os_GetHighPrio(void)
{
    Os_PriorityType highPrio;

    if (Os_CfgPriorityGroup > 1u)
    {
        Os_PriorityType const* ptr = NULL_PTR;
        Os_PriorityType priorityTemp;

        ptr = &Os_ReadyMap[Os_CfgPriorityGroup - 1u];
        priorityTemp = *ptr;
        highPrio = (((Os_PriorityType)Os_CfgPriorityGroup - 1u) << 4u);

        /*
         * MISRA-C:2004 Rule 17.4
         * Msg(4:0489) The integer value 1 is being added or subtracted from
         * a pointer.
         */
        while (0u == priorityTemp)
        {
            ptr--;

            if (ptr < &Os_ReadyMap[0])
            {
                while (1)
                {
                    /* Nothing to do. */
                }
            }

            priorityTemp = *ptr;
            /*here have a potential bug in the first version,here must be
             * decrease but not increase*/
            highPrio -= NUM_PRIORITYBITS_PERWORD;
        }

        highPrio += Os_GetHighPrioBit(priorityTemp);
    }
    else
    {
        if (NULL_PTR == Os_ReadyMap)
        {
            while (1)
            {
                /* Nothing to do. */
            }
        }

        highPrio = Os_GetHighPrioBit(Os_ReadyMap[0]);
    }

    return highPrio;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
FUNC(void, OS_CODE) Os_ReadyQueueInsert(Os_TaskType object, Os_CallLevelType level, Os_PriorityType prio)
{
    switch (level)
    {
#if (CFG_STD_RESOURCE_MAX > 0U)
    case OS_LEVEL_STANDARD_RESOURCE:
/*  make the prio related with the calling task*/
#if ((OS_BCC1 == CFG_CC) || (OS_ECC1 == CFG_CC))
        Os_ReadyTable[prio] = Os_SCB.sysRunningTaskID;
#else
        if (Os_ReadyQueueMark[prio].queueHead == 0u)
        {
            Os_ReadyQueueMark[prio].queueHead = Os_ActivateQueueSize[prio] - 1u;
        }
        else
        {
            Os_ReadyQueueMark[prio].queueHead--;
        }

        Os_ReadyQueue[prio][Os_ReadyQueueMark[prio].queueHead] = Os_SCB.sysRunningTaskID;
#endif /* OS_BCC1 == CFG_CC || OS_ECC1 == CFG_CC */

        Os_ReadyMap[Os_PrioGroup[prio]] |= Os_PrioMask[prio];
        break;
#endif /* CFG_STD_RESOURCE_MAX > 0U */

    case OS_LEVEL_TASK:
#if ((OS_BCC1 == CFG_CC) || (OS_ECC1 == CFG_CC))
        Os_ReadyTable[prio] = object;
#else

        Os_ReadyQueue[prio][Os_ReadyQueueMark[prio].queueTail] = object;
        Os_ReadyQueueMark[prio].queueTail++;

        if (Os_ReadyQueueMark[prio].queueTail == Os_ActivateQueueSize[prio])
        {
            Os_ReadyQueueMark[prio].queueTail = 0u;
        }
#endif /* OS_BCC1 == CFG_CC || OS_ECC1 == CFG_CC */

        Os_ReadyMap[Os_PrioGroup[prio]] |= Os_PrioMask[prio];
        break;

#if (CFG_INTERNAL_RESOURCE_MAX > 0)
    case OS_LEVEL_INTERNAL_RESOURCE:
/*  make the prio related with the calling task*/
#if ((OS_BCC1 == CFG_CC) || (OS_ECC1 == CFG_CC))
        Os_ReadyTable[prio] = object;
#else

        if (Os_ReadyQueueMark[prio].queueHead == 0u)
        {
            Os_ReadyQueueMark[prio].queueHead = Os_ActivateQueueSize[prio] - 1u;
        }
        else
        {
            Os_ReadyQueueMark[prio].queueHead--;
        }

        Os_ReadyQueue[prio][Os_ReadyQueueMark[prio].queueHead] = object;
#endif /* OS_BCC1 == CFG_CC || OS_ECC1 == CFG_CC */

        Os_ReadyMap[Os_PrioGroup[prio]] |= Os_PrioMask[prio];
        break;
#endif /* CFG_INTERNAL_RESOURCE_MAX > 0 */

    default:
        Os_Panic();
        break;
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
FUNC(void, OS_CODE) Os_ReadyQueueRemove(Os_CallLevelType level, Os_PriorityType prio)
{
    switch (level)
    {
#if (CFG_STD_RESOURCE_MAX > 0U)
    case OS_LEVEL_STANDARD_RESOURCE:
/*make the prio related with the calling task */
#if ((OS_BCC1 == CFG_CC) || (OS_ECC1 == CFG_CC))
        Os_ReadyMap[Os_PrioGroup[prio]] &= ~Os_PrioMask[prio];
        Os_ReadyTable[prio] = OS_TASK_INVALID;
#else
        Os_ReadyQueue[prio][Os_ReadyQueueMark[prio].queueHead] = OS_TASK_INVALID;
        Os_ReadyQueueMark[prio].queueHead++;

        if (Os_ReadyQueueMark[prio].queueHead == Os_ActivateQueueSize[prio])
        {
            Os_ReadyQueueMark[prio].queueHead = 0u;
        }

        if (Os_ReadyQueueMark[prio].queueHead == Os_ReadyQueueMark[prio].queueTail)
        {
            Os_ReadyMap[Os_PrioGroup[prio]] &= ~Os_PrioMask[prio];
        }
#endif /* OS_BCC1 == CFG_CC || OS_ECC1 == CFG_CC */
        break;
#endif /* CFG_STD_RESOURCE_MAX > 0U */

    case OS_LEVEL_TASK:
#if ((OS_BCC1 == CFG_CC) || (OS_ECC1 == CFG_CC))
        Os_ReadyTable[prio] = OS_TASK_INVALID;
        Os_ReadyMap[Os_PrioGroup[prio]] &= ~Os_PrioMask[prio];
#else
        Os_ReadyQueue[prio][Os_ReadyQueueMark[prio].queueHead] = OS_TASK_INVALID;
        Os_ReadyQueueMark[prio].queueHead++;

        if (Os_ReadyQueueMark[prio].queueHead == Os_ActivateQueueSize[prio])
        {
            Os_ReadyQueueMark[prio].queueHead = 0u;
        }

        if (Os_ReadyQueueMark[prio].queueHead == Os_ReadyQueueMark[prio].queueTail)
        {
            Os_ReadyMap[Os_PrioGroup[prio]] &= ~Os_PrioMask[prio];
        }
#endif /* OS_BCC1 == CFG_CC || OS_ECC1 == CFG_CC */
        break;

#if (CFG_INTERNAL_RESOURCE_MAX > 0U)
    case OS_LEVEL_INTERNAL_RESOURCE:
#if ((OS_BCC1 == CFG_CC) || (OS_ECC1 == CFG_CC))
        Os_ReadyMap[Os_PrioGroup[prio]] &= ~Os_PrioMask[prio];
        Os_ReadyTable[prio] = OS_TASK_INVALID;
#else
        Os_ReadyQueue[prio][Os_ReadyQueueMark[prio].queueHead] = OS_TASK_INVALID;
        Os_ReadyQueueMark[prio].queueHead++;

        if (Os_ReadyQueueMark[prio].queueHead == Os_ActivateQueueSize[prio])
        {
            Os_ReadyQueueMark[prio].queueHead = 0u;
        }

        if (Os_ReadyQueueMark[prio].queueHead == Os_ReadyQueueMark[prio].queueTail)
        {
            Os_ReadyMap[Os_PrioGroup[prio]] &= ~Os_PrioMask[prio];
        }
#endif /* OS_BCC1 == CFG_CC || OS_ECC1 == CFG_CC */
        break;
#endif /* CFG_INTERNAL_RESOURCE_MAX > 0U */

    default:
        Os_Panic();
        break;
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
FUNC(Os_TaskType, OS_CODE) Os_ReadyQueueGetFirst(Os_PriorityType prio)
{
    VAR(Os_TaskType, OS_VAR) Os_FirstTask;

    if (prio >= Os_CfgPriorityMax)
    {
        Os_FirstTask = OS_TASK_INVALID;
    }
    else
    {
#if ((OS_BCC1 == CFG_CC) || (OS_ECC1 == CFG_CC))
        Os_FirstTask = Os_ReadyTable[prio];
#else
        Os_FirstTask = Os_ReadyQueue[prio][Os_ReadyQueueMark[prio].queueHead];
#endif /* OS_BCC1 == CFG_CC || OS_ECC1 == CFG_CC */
    }

    return Os_FirstTask;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
FUNC(void, OS_CODE) Os_SwitchTask(void)
{
    Os_TaskStateType tempState;
#if (TRUE == CFG_LOAD_RATIO_CALC_ENABLE)
    VAR(uint32, OS_VAR) curTicks;
#endif /* TRUE == CFG_LOAD_RATIO_CALC_ENABLE */
    Os_SCB.sysPrevTaskID = Os_SCB.sysRunningTaskID;
#if (TRUE == CFG_STACK_CHECK)
    Os_StackPtrType StackPtr;
#endif

    if (NULL_PTR != Os_SCB.sysRunningTCB)
    {
/* check running task stack overflow. */
#if (TRUE == CFG_STACK_CHECK)
        StackPtr = Os_SCB.sysRunningTCB->taskStackBottom;
        Os_StackMonitor(StackPtr);
#endif

        if (TASK_STATE_RUNNING == Os_SCB.sysRunningTCB->taskState)
        {
            Os_SCB.sysRunningTCB->taskState = TASK_STATE_READY;

            Os_PostTaskHook(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_KERNEL_MACRO_004 */
        }
    }

    Os_SCB.sysRunningTaskID = Os_SCB.sysHighTaskID;
    Os_SCB.sysRunningTCB = &Os_TCB[Os_SCB.sysRunningTaskID];
#if (TRUE == CFG_SCHEDULE_COUNT_MONITOR)
    Os_IncrementTaskScheduleCount();
#endif
    /*Writing the running ApplID and Object */

#if (CFG_OSAPPLICATION_MAX > 0U)
    Os_SCB.sysRunningAppID = Os_ObjectAppCfg[OBJECT_TASK][Os_SCB.sysRunningTaskID].hostApp;
    Os_SCB.sysRunningAppObj = OBJECT_TASK;
#endif

#if ((OS_PREEMPTIVE_MIXED == CFG_SCHED_POLICY) || (OS_PREEMPTIVE_NON == CFG_SCHED_POLICY))
    if (OS_PREEMPTIVE_NON == Os_TaskCfg[Os_SCB.sysRunningTaskID].osTaskSchedule)
    {
        Os_SCB.sysDispatchLocker = 1U;
    }
#endif

#if (CFG_INTERNAL_RESOURCE_MAX > 0)
    Os_GetInternalResource();
#endif

    tempState = Os_SCB.sysRunningTCB->taskState;

    Os_SCB.sysRunningTCB->taskState = TASK_STATE_RUNNING;

    Os_PreTaskHook(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_KERNEL_MACRO_004 */

/* Timing protection: start task exe time. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    Os_TmProtTaskStart(Os_SCB.sysRunningTaskID, TP_TASK_EXE);
#endif

    if (TASK_STATE_START == tempState)
    {
        Os_SCB.sysRunningTCB->taskTop =
            /* PRQA S 3469 ++ */ /* MISRA  Dir-4.9*/ /* OS_KERNEL_MACRO_004 */
            Os_TaskStack[Os_GetObjLocalId(Os_TaskCfg[Os_SCB.sysRunningTaskID].osTaskStackId)].stackTop;
        /* PRQA S 3469 -- */ /* MISRA  Dir-4.9*/

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
        Os_MemProtTaskCat1Map();
#endif

        Os_ArchFirstEnterTask();

#if (TRUE == CFG_TASK_RESPONSE_TIME_ENABLE)
        Os_TaskRecordStartTick(Os_SCB.sysRunningTaskID);
#endif /* TRUE == CFG_TASK_RESPONSE_TIME_ENABLE */
    }
    else
    {
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
        Os_MemProtTaskCat2Map();
#endif
    }

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceTaskRun(Os_SCB.sysRunningTaskID);
#endif

#if (TRUE == CFG_LOAD_RATIO_CALC_ENABLE)
    curTicks = Os_ExitTaskRecordTick(Os_SCB.sysPrevTaskID);
    Os_EnterTaskRecordTick(Os_SCB.sysRunningTaskID, curTicks);
#endif /* TRUE == CFG_LOAD_RATIO_CALC_ENABLE */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <get the highest priority bit>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <HighPriReadyTaskInQueue: a mixed value of a priority queue>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <the highest bit value of the priority queue >
 * PreCondition         <None>
 * CallByAPI            <Os_GetHighPrio>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(Os_PriorityType, OS_CODE) Os_GetHighPrioBit(Os_PriorityType HighPriReadyTaskInQueue)
{
    /* Index into table is bit pattern to resolve highest priority. */
    /* DD_1_0149 */
    static CONST(uint8, OS_CONST) Os_MapTable[16] = {0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3};
    VAR(Os_PriorityType, OS_VAR) highPrio = 0u;

/*means that the value of Priority is proportional to Priority */
#if (NUM_PRIORITYBITS_PERWORD >= 32)
    if ((HighPriReadyTaskInQueue & 0xFFFF0000U) != 0u)
    {
        highPrio += 16u;
        HighPriReadyTaskInQueue >>= 16u;
    }
#endif

#if (NUM_PRIORITYBITS_PERWORD >= 16)
    if ((HighPriReadyTaskInQueue & 0xFF00U) != 0u)
    {
        highPrio += 8u;
        /* PRQA S 1338 ++ */ /* MISRA  Rule-17.8*/ /*OS_KERNEL_PARAMETER_MODIFIED_006*/
        HighPriReadyTaskInQueue >>= 8u;
        /* PRQA S 1338 -- */ /* MISRA  Rule-17.8*/
    }
#endif

    if ((HighPriReadyTaskInQueue & 0xF0U) != 0u)
    {
        highPrio += 4u;
        /* PRQA S 1338 ++ */ /* MISRA  Rule-17.8*/ /*OS_KERNEL_PARAMETER_MODIFIED_006*/
        HighPriReadyTaskInQueue >>= 4u;
        /* PRQA S 1338 -- */ /* MISRA  Rule-17.8*/
    }

    return (uint16)(highPrio + Os_MapTable[HighPriReadyTaskInQueue]);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <init the ready queue or ready table>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_InitSystem>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(void, OS_CODE) Os_InitReadyTable(void)
{
#if ((OS_BCC1 == CFG_CC) || (OS_ECC1 == CFG_CC))
    VAR(uint16, OS_VAR) i;
#else
    VAR(uint16, OS_VAR) i;
    VAR(uint32, OS_VAR) j;
#endif /* OS_BCC1 == CFG_CC || OS_ECC1 == CFG_CC */

    for (i = 0U; i < Os_ReadyMapSize; i++)
    {
        Os_ReadyMap[i] = 0U;
    }

    for (i = 0U; i < Os_CfgPriorityMax; i++)
    {
#if ((OS_BCC1 == CFG_CC) || (OS_ECC1 == CFG_CC))
        Os_ReadyTable[i] = OS_TASK_INVALID;
#else

        Os_ReadyQueueMark[i].queueHead = 0U;
        Os_ReadyQueueMark[i].queueTail = 0U;

        for (j = 0U; j < Os_ActivateQueueSize[i]; j++)
        {
            Os_ReadyQueue[i][j] = OS_TASK_INVALID;
        }
#endif /* OS_BCC1 == CFG_CC || OS_ECC1 == CFG_CC */
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
FUNC(void, OS_CODE) Os_ClearPrioReadyMap(Os_PriorityType OsPrio)
{
    Os_ReadyMap[Os_PrioGroup[OsPrio]] &= ~Os_PrioMask[OsPrio];

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
FUNC(void, OS_CODE) Os_TaskErrBack(void)
{
    OS_ENTER_KERNEL();
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    Os_TaskEndNoTerminate();
#endif
    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_KERNEL_MACRO_004 */
    while (1)
    {
        /* Nothing to do. */
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The function init os kernel variable in MultiCore.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <StartOS>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(void, OS_CODE) Os_MultiCoreInitKernel(void)
{
    VAR(uint16, OS_VAR) vCoreId = Os_SCB.sysCore;

    Os_SCB.sysIsrNestQueue = Os_SysIsrNestQueue_Inf[vCoreId];
    /* PRQA S 3469 ++*/ /* MISRA Dir 4.9 */ /* OS_KERNEL_MACRO_004 */
    Os_ReadyMapSize = READY_MAP_SIZE((Os_CfgPriorityMax_Inf[vCoreId]));
    /* PRQA S 3469 --*/ /* MISRA Dir 4.9 */ /* OS_KERNEL_MACRO_004 */
    Os_CfgPriorityMax = Os_CfgPriorityMax_Inf[vCoreId];
    Os_ReadyMap = Os_ReadyMap_Inf[vCoreId];
#if ((OS_BCC1 == CFG_CC) || (OS_ECC1 == CFG_CC))
    Os_ReadyTable = Os_ReadyTable_Inf[vCoreId];
#else
    Os_ReadyQueueMark = Os_ReadyQueueMark_Inf[vCoreId];
    Os_ActivateQueueSize = Os_ActivateQueueSize_Inf[vCoreId];
    Os_ReadyQueue = Os_ReadyQueue_Inf[vCoreId];
#endif /* OS_BCC1 == CFG_CC || OS_ECC1 == CFG_CC */
    Os_PrioGroup = Os_PrioGroup_Inf[vCoreId];
    Os_PrioMask = Os_PrioMask_Inf[vCoreId];
    Os_CfgPriorityGroup = Os_CfgPriorityGroup_Inf[vCoreId];
    Os_SystemStack = Os_SystemStack_Inf[vCoreId];
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
FUNC(void, OS_CODE) Os_SynPoint(uint8 point)
{
    VAR(uint16, OS_VAR) coreId = 0;
    VAR(uint16, OS_VAR) vCoreId = Os_SCB.sysCore;

    switch (point)
    {
    /* Multi core state Synchronous point0 ,for check start os appmode. */
    case 0:
        Os_CoreCB.coreStateSynPoint0[vCoreId] = TRUE; /* PRQA S 4404 */ /* MISRA Rule 10.3 */ /* OS_KERNEL_TYPE_007 */
        while (coreId < OS_AUTOSAR_CORES)
        {
            /* PRQA S 1881 ++*/ /* MISRA Rule 10.4 */ /* OS_KERNEL_TYPE_008 */
            if (TRUE == Os_CoreCB.coreStateSynPoint0[coreId])
            /* PRQA S 1881 --*/ /* MISRA Rule 10.4 */
            {
                coreId++;
            }
        }
        break;

    /* SWS_Os_00580:synchronize before the global StartupHook. */
    case 1:
        Os_CoreCB.coreStateSynPoint1[vCoreId] = TRUE; /* PRQA S 4404 */ /* MISRA Rule 10.3 */ /* OS_KERNEL_TYPE_007 */
        while (coreId < OS_AUTOSAR_CORES)
        {
            /* PRQA S 1881 ++*/ /* MISRA Rule 10.4 */ /* OS_KERNEL_TYPE_008 */
            if (TRUE == Os_CoreCB.coreStateSynPoint1[coreId])
            /* PRQA S 1881 --*/ /* MISRA Rule 10.4 */
            {
                coreId++;
            }
        }
        break;

        /* SWS_Os_00579:synchronize after the global StartupHook. */
    case 2:
        Os_CoreCB.coreStateSynPoint2[vCoreId] = TRUE; /* PRQA S 4404 */ /* MISRA Rule 10.3 */ /* OS_KERNEL_TYPE_007 */
        while (coreId < OS_AUTOSAR_CORES)
        {
            /* PRQA S 1881 ++*/ /* MISRA Rule 10.4 */ /* OS_KERNEL_TYPE_008 */
            if (TRUE == Os_CoreCB.coreStateSynPoint2[coreId])
            /* PRQA S 1881 --*/ /* MISRA Rule 10.4 */
            {
                coreId++;
            }
        }
        break;

        /* SWS_Os_00587:synchronize before calling the global ShutdownHook. */
    case 3:
        Os_CoreCB.coreStateSynPoint3[vCoreId] = TRUE; /* PRQA S 4404 */ /* MISRA Rule 10.3 */ /* OS_KERNEL_TYPE_007 */
        while (coreId < OS_AUTOSAR_CORES)
        {
            /* PRQA S 1881 ++*/ /* MISRA Rule 10.4 */ /* OS_KERNEL_TYPE_008 */
            if (TRUE == Os_CoreCB.coreStateSynPoint3[coreId])
            /* PRQA S 1881 --*/ /* MISRA Rule 10.4 */
            {
                coreId++;
            }
        }
        break;

    default:
        Os_Panic();
        break;
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Check the AppMode when calling StartOS in multicore.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <StartOS>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(Os_AppModeType, OS_CODE) Os_CheckAppMode(Os_AppModeType mode)
{
    VAR(Os_AppModeType, OS_VAR) retAppMode = DONOTCARE;
    VAR(Os_CoreIdType, OS_VAR) loop_core;

    /* It is not allowed to call StartOS on cores
     * activated by StartNonAutosarCore.*/
    if (OS_AUTOSAR_CORES <= Os_SCB.sysCore)
    {
        while (1)
        {
            /* Nothing to do. */
        }
    }

    /* Multi core state Synchronous point0, for check start os appmode. */
    Os_CoreCB.osAppMode[Os_SCB.sysCore] = mode;
    Os_SynPoint(0);

    /* SWS_Os_00609: if StartOS is called with the AppMode "DONOTCARE"
     * the application mode of the other core(s) (differing from
     * "DONOTCARE") shall be used. */
    for (loop_core = 0u; loop_core < OS_AUTOSAR_CORES; loop_core++)
    {
        if (DONOTCARE != Os_CoreCB.osAppMode[loop_core])
        {
            if (DONOTCARE == retAppMode)
            {
                retAppMode = Os_CoreCB.osAppMode[loop_core];
            }
            else
            {
                if (retAppMode != Os_CoreCB.osAppMode[loop_core])
                {
                    while (1)
                    {
                        /* Nothing to do. */
                    }
                }
            }
        }
    }

    /* SWS_Os_00610: At least one core shall define an AppMode other than
     * "DONOTCARE". */
    if (DONOTCARE == retAppMode)
    {
        while (1)
        {
            /* Nothing to do. */
        }
    }
    else
    {
        for (loop_core = 0u; loop_core < OS_AUTOSAR_CORES; loop_core++)
        {
            Os_CoreCB.osAppMode[loop_core] = retAppMode;
        }
    }

    return retAppMode;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Clear the CoreCB info>
 * Service ID           <none>
 * Sync/Async           <none>
 * Reentrancy           <none>
 * param-eventId[in]    <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <ShutdownOS>
 * REQ ID               <None>
 *
 */
/********************************************************************/
FUNC(void, OS_CODE) Os_DeInitCoreInfo(void)
{
    VAR(uint16, OS_VAR) vCoreId = Os_SCB.sysCore;

    /*clear the CoreCB info*/
#if (OS_AUTOSAR_CORES > 1U)
    Os_GetInternalSpinlock(&Os_SpinlockSync);
#endif
    Os_CoreCB.coreStateActive &= (Os_CoreIdType)(~((Os_CoreIdType)1 << vCoreId));
    Os_CoreCB.osAppMode[vCoreId] = OS_NULL_APPMODE;
    Os_CoreCB.coreStatus[vCoreId] = OS_CORE_STATUS_INVALID;
#if (OS_AUTOSAR_CORES > 1U)
    Os_ReleaseInternalSpinlock(&Os_SpinlockSync);
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/*=======[E N D   O F   F I L E]==============================================*/
