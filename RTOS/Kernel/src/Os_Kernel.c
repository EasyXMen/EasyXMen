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
 ************************************************************************************************************************
 **
 **  @file               : Os_Kernel.c
 **  @author             : i-soft-os
 **  @date               : 2025/02/10
 **  @vendor             : isoft
 **  @description        : Os source file for Kernel API implementations
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Os_Arch_Processor.h"
#include "Os_Kernel.h"
#include "Os_ReadyQue.h"
#include "Os_Core.h"
#include "Os_Task.h"
#include "Os_Alarm.h"
#include "Os_Counter.h"
#include "Os_Event.h"
#include "Os_Interrupt.h"
#include "Os_Resource.h"
#include "Os_Spinlock.h"
#include "Os_Rpc.h"
#include "Os_Sprot.h"
#include "Os_Appl.h"
#include "Ioc.h"
#include "Os_ScheduleTable.h"
#include "Os_TrustedFunc.h"
#include "Os_Mprot.h"
#include "Os_StackMonitor.h"
#include "Os_Tprot.h"
#include "Os_Hook.h"
#include "Os_FaultManager.h"
#include "Os_Barrier.h"
#include "Os_Err.h"
#include "Os_Rti.h"
#include "Os_Arti.h"
#include "Os_Monitor.h"

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ============================================ external data definitions =========================================== */

/* ============================================ internal data definitions =========================================== */

/* ========================================== internal function declarations ======================================== */
/**
 * @brief           Initializes the System Control Block (SCB) for a specific core
 * @param[in]       pScb: Pointer to the System Control Block to initialize
 * @param[in]       coreId: ID of the core associated with this SCB
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_InitScb(Os_SCBType *pScb, Os_CoreIdType coreId);

/**
 * @brief           Initializes all OS system components in sequence
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_InitSystem(void);

/**
 * @brief           Initializes OS kernel variables in multicore configuration
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_MultiCoreInitKernel(void);

/**
 * @brief           Validates and synchronizes application modes across cores
 * @param[in]       mode: Application mode requested for this core
 * @return          Os_AppModeType
 * @retval          Application mode selected for all cores
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL Os_AppModeType Os_CheckAppMode(Os_AppModeType mode);

/* ========================================== internal function definitions ========================================= */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * init the os module
 */
OS_LOCAL void Os_InitScb(Os_SCBType *pScb, Os_CoreIdType coreId)
{
    pScb->SysDispatchLocker = 0U;
    pScb->SysOsLevel = OS_LEVEL_TASK;
    pScb->SysHighPrio = OS_PRIORITY_INVALID;
    pScb->SysHighTaskId = INVALID_TASK; /* PRQA S 1461, 1297 */ /* VL_Os_1461 *//* VL_Os_1297 */
    pScb->SysRunningTaskId = INVALID_TASK; /* PRQA S 1461, 1297 */ /* VL_Os_1461 *//* VL_Os_1297 */
    pScb->SysRunningTCB = NULL_PTR;
    pScb->QueueMg = Os_ReadyQueMg_Inf[coreId];

#if (CFG_ISR2_MAX > 0)
    pScb->SysInIsrCat2 = FALSE;
    pScb->SysRunningIsrCat2Id = INVALID_ISR; /* PRQA S 1297*/ /* VL_Os_1297*/
#endif

#if (CFG_OSAPPLICATION_MAX > 0)
    pScb->SysRunningAppObj = OS_OBJECT_MAX;
    pScb->SysRunningAppId = INVALID_OSAPPLICATION;
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/* ========================================== external function definitions ========================================= */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * The user can call this  system service to start the
 *                     operating system in a specific mode.
 */
/* PRQA S 1503, 3006, 6070, 3408, 1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_MTR_Os_STCAL, VL_Os_3408, VL_Os_1512 */
void StartOS(AppModeType Mode)
/* PRQA S 1503, 3006, 6070, 3408, 1512 -- */
{
    StatusType err = E_OK;

    /* Get the unique logical CoreID.The operation must be in the front,
     *  Will be used in the processof the system is running. */
    Os_CoreIdType sysCore = Os_Hal_GetCoreID();
    Os_SCBType *pScb = Os_GetSystemContext(sysCore);
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_StartOS);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_StartOS_Start, Mode);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    pScb->SysCore = sysCore;

    /* The Core is Non AUTOSAR Core. */
    if (OS_CORE_INVALID == pScb->SysCore)
    {
        err = E_NOT_OK;
    }
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    else if (pScb->SysOsLevel != OS_LEVEL_MAIN)
    {
        err = E_NOT_OK;
    }
#endif
    else
    {
        /* 01. must disable all interrupts during start os */
        Os_Hal_DisableInt(); /* PRQA S 1753, 1512, 1513 */ /* VL_Os_1753, VL_Os_1512, VL_Os_1513 */

        /* 02. Check application mode. */
        pScb->SysActiveAppMode = Os_CheckAppMode(Mode);

        /* 03. Init processor. */
        Os_Hal_MultiCoreInit(sysCore);

        /* 04. Init kernel. */
        Os_MultiCoreInitKernel();

        /* 05. init system stack and switch to system stack */
#if (TRUE == CFG_STACK_CHECK)
        Os_FillStack(pScb->SystemStack);
#endif

        /* 06. Init os. */
        Os_InitSystem();

        /* 07. Init OS CPU arch. */
        /* PRQA S 3138,0306,1258,1006 ++ */ /* VL_Os_3138, VL_Os_0306, VL_Os_ConstToIntegral, VL_Os_1006 */
        Os_Hal_InitCPU();
        /* PRQA S 3138,0306,1258,1006 -- */

        /* 08. Init IOC. */
#if (CFG_IOC_MAX > 0U)
        Os_InitIoc();
#endif

        /* 09. Init memory protection. */
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
        Os_InitMemProt();
        /* PRQA S 3138, 1006, 4543, 1021 ++ */ /* VL_Os_3138, VL_Os_1006, VL_Os_4543, VL_Os_1021 */
        Os_Hal_MemProtEnable();
        /* PRQA S 3138, 1006, 4543, 1021 -- */
#endif

        /* 10. synchronize before the global StartupHook-<SWS_Os_00580>. */
        Os_SynPoint(1U);

        /* 011. Startup hook. */
        Os_StartupHook(pScb); /* PRQA S 3138, 3141 */ /* VL_Os_3138, VL_Os_3141 */

        /* 012. Application specific hook. */
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_APPL_STARTUPHOOK)
        Os_ApplStartupHook(pScb);
#endif
#endif

        /* 013. synchronize after the global StartupHook-<SWS_Os_00579>. */
        Os_SynPoint(2U);/* PRQA S 3120 */ /* VL_QAC_MagicNum */

        /* 014. Task Schedule. */
        Os_Hal_StartScheduler();
    }
    UNUSED_PARAMETER(err);
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544, 1259 ++ */ /* VL_Os_3432, VL_Os_4544, VL_Os_1259 */
    OSRtiExitApi(pScb, OSApiId_StartOS);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_StartOS_Return, 0);
    /* PRQA S 3432, 4544, 1259 -- */
    /* PRQA S 3138, 3141 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Internal implementation of OS service:ShutdownOS
 */
void Os_ShutdownOS(StatusType error, Os_ShutdownAction action)  /* PRQA S 2755 */ /* VL_Os_2755 */
{
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    Os_CoreCB.CoreStatus[pScb->SysCore] = OS_CORE_PRE_SHUTDOWN;
/* SWS_Os_00586: Application specific hook. */
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_APPL_SHUTDOWNHOOK)
    Os_ApplShutdownHook(pScb, error);
#endif
#endif

#if (OS_AUTOSAR_CORES > 1)
    if (OS_SHUTDOWN_ALL_OS == action)
    {
        Os_SynPoint(3U);/* PRQA S 3120 */ /* VL_QAC_MagicNum */
    }
#endif

    Os_Hal_DisableInt();
/* SWS_Os_00588: Global ShutdownHook. */
#if (TRUE == CFG_SHUTDOWNHOOK)
    Os_CallShutdownHook(pScb, error);
#endif

/*Os_00620*/
#if (CFG_SPINLOCK_MAX > 0U)
    Os_TCBType *pTCB = NULL_PTR;
    SpinlockIdType SpinlockId = 0u; /* PRQA S 1297 */ /* VL_Os_1297 */
    Os_SpinlockType spinLockIdx = 0u;
    Os_TaskType taskIdStartRange = Os_TaskIdRange[pScb->SysCore].AllTask.Start;
    Os_TaskType taskIdEndRange = Os_TaskIdRange[pScb->SysCore].AllTask.End;
    for (uint16 i = (uint16)taskIdStartRange; i < taskIdEndRange; i++) /* PRQA S 1880 */ /* VL_Os_1880 */
    {
        pTCB = Os_TCB[i];
        for (spinLockIdx = pTCB->TaskCriticalZoneCount; spinLockIdx > 0u; spinLockIdx--)
        {
            if (OS_OBJECT_SPINLOCK == pTCB->TaskCriticalZoneType[pTCB->TaskCriticalZoneCount - 1u])
            {
                SpinlockId = pTCB->TaskCriticalZoneStack[pTCB->TaskCriticalZoneCount - 1u]; /* PRQA S 4442 */ /* VL_Os_4442 */
                (void)Os_ReleaseSpinlock(pScb, SpinlockId);
            }
            else
            {
                pTCB->TaskCriticalZoneCount--;
            }
        }
    }

    Os_ICBType *pICB = NULL_PTR;
    Os_IsrType isrIdStartRange = Os_IsrIdRange[pScb->SysCore].Isr2.IsrStart;
    Os_IsrType isrIdEndRange = Os_IsrIdRange[pScb->SysCore].Isr2.IsrEnd;
    for (uint16 i = (uint16)isrIdStartRange; i < isrIdEndRange; i++) /* PRQA S 1880 */ /* VL_Os_1880 */
    {
        pICB = Os_ICB[i];
        for (spinLockIdx = pICB->Isr2CriticalZoneCount; spinLockIdx > 0u; spinLockIdx--)
        {
            if (OS_OBJECT_SPINLOCK == pICB->Isr2CriticalZoneType[pICB->Isr2CriticalZoneCount - 1u])
            {
                SpinlockId = pICB->Isr2CriticalZoneStack[pICB->Isr2CriticalZoneCount - 1u]; /* PRQA S 4442 */ /* VL_Os_4442 */
                (void)Os_ReleaseSpinlock(pScb, SpinlockId);
            }
            else
            {
                pICB->Isr2CriticalZoneCount--;
            }
        }
    }
#endif
    Os_DeInitCoreInfo();
    UNUSED_PARAMETER(error);
    UNUSED_PARAMETER(action);
    UNUSED_PARAMETER(pScb);
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
/**
 * terminate the os,the system will enter the background program
 */
/* PRQA S 1503, 3006, 6070, 3408, 1512 ++ */ /* VL_QAC_NoUsedApi,VL_Os_3006,VL_MTR_Os_STCAL, VL_Os_3408, VL_Os_1512 */
void ShutdownOS(StatusType Error)
/* PRQA S 1503, 3006, 6070, 3408, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    StatusType err = E_OK; /* PRQA S 2983 */ /* VL_Os_2983 */

    /* PRQA S 2983, 3678 ++ */ /* VL_Os_2983, VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 2983, 3678 -- */

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_ShutdownOS);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ShutdownOS_Start, Error);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    Os_ServicePortParamType SprotParam = {
        .AllowedContext = OS_SERVICEPORT_CHECK_SHUTDOWN_OS,
        .ObjectType = (Os_ObjectTypeType)OS_OBJECT_INVALID,
        .ObjectID = (Os_AppObjectId)OS_OBJECT_INVALID,
        /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral */
        .Address = NULL_PARA,
        /* PRQA S 1258 -- */
    };
    err = Os_ServiceProtCheck(pScb, &SprotParam);
    if (E_OK == err)
#endif
    {
        /*OS054*/
#if (CFG_OSAPPLICATION_MAX > 0U)
        if (FALSE == Os_AppCfg[pScb->SysRunningAppId].Trusted)
        {
            err = E_OS_CALLEVEL;
        }
        else
#endif
        {
            Os_ShutdownOS(Error, OS_SHUTDOWN_OS);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (E_OK != err) /* PRQA S 2991, 2995, 2880 */ /* VL_Os_2991, VL_Os_2995, VL_Os_2880 */
    {
        Os_TraceErrorHook(OSError_Save_ShutDownOs(Error),
                          OSServiceId_ShutdownOS,
                          err, pScb);/* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544, 1259 ++ */ /* VL_Os_3432, VL_Os_4544, VL_Os_1259 */
    OSRtiExitApi(pScb, OSApiId_ShutdownOS); /* PRQA S 2880 */ /* VL_Os_2880 */
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ShutdownOS_Return, 0);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544, 1259 -- */
    /* PRQA S 3138, 3141 -- */
    UNUSED_PARAMETER(err);
    UNUSED_PARAMETER(pScb);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * get the active application mode of os
 */
/* PRQA S 1503, 3006, 3408, 1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_Os_3408, VL_Os_1512 */
AppModeType GetActiveApplicationMode(void)
/* PRQA S 1503, 3006, 3408, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */ 
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    AppModeType appMode;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 1259, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_1259, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_GetActiveApplicationMode);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_GetActiveApplicationMode_Start, 0);
    /* PRQA S 1317, 1259, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    if (Os_WrongContext(pScb, OS_CONTEXT_GET_ACTIV_APPLICATION_MODE) != TRUE)
    {
        appMode = OS_NULL_APPMODE;
    }
    else if (Os_IgnoreService(pScb) != TRUE)
    {
        appMode = OS_NULL_APPMODE;
    }
    else
#endif
    {
        appMode = pScb->SysActiveAppMode;
    }

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544, 1259 ++ */ /* VL_Os_3432, VL_Os_4544, VL_Os_1259 */
    OSRtiExitApi(pScb, OSApiId_GetActiveApplicationMode);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_GetActiveApplicationMode_Return, 0);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544, 1259 -- */
    /* PRQA S 3138, 3141 -- */
    return appMode;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * init the os module
 */
/* PRQA S 6070++ */ /* VL_MTR_Os_STCAL */
OS_LOCAL void Os_InitSystem(void)
/* PRQA S 6070 -- */
{
    Os_CoreIdType coreId = Os_GetCoreIdLocal();
    Os_SCBType *pScb = Os_GetSystemContext(coreId);

    Os_InitScb(pScb, coreId); /*01.Init Os_SCB */

    /*02.Init Os_CoreCB, OS_CORE_ID_MASTER is auto start, don't by means of StartCore.*/
    if (OS_CORE_ID_MASTER == coreId)
    {
        /* PRQA S 3442 ++ */ /* VL_Os_3442 */
        Os_CoreCB.CoreStateActive |= (uint8)(1u << OS_CORE_ID_MASTER);
        /* PRQA S 3442 -- */
        Os_CoreCB.CoreStatus[OS_CORE_ID_MASTER] = OS_RUN;
    }

    Os_InitReadyTable(); /* 03.Init ready table */

#if (CFG_TASK_MAX > 0U)
    Os_InitTask(); /* 04.Init TCB */
#endif

#if (CFG_ISR2_MAX > 0)
    Os_InitInterrupt(); /* 05.Init ICB */
#endif

#if (CFG_COUNTER_MAX > 0U) /* 06.Init Counter */
    Os_InitCounter();
#endif

#if (CFG_ALARM_MAX > 0U) /* 07.Init Alarm */
    Os_InitAlarm();
#endif

#if (CFG_EXTENDED_TASK_MAX > 0)
    Os_InitEvent(); /* 08.Init Event */
#endif

#if (CFG_STD_RESOURCE_MAX > 0)
    Os_InitResource(); /* 09.Init Resource */
#endif

#if (CFG_SCHEDTBL_MAX > 0)
    Os_InitScheduleTable(); /* 10.Init schedule table. */
#endif

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    Os_InitTmProt(pScb->SysCore);
#endif

#if (CFG_SPINLOCK_MAX > 0U)
    Os_InitSpinlock(pScb->SysCore);
#endif

#if (CFG_OSAPPLICATION_MAX > 0U)
    Os_InitApplication(); /* 13.Init application. */
#endif

#if (CFG_TRUSTED_SYSTEM_SERVICE_MAX > 0U)
    Os_InitTrustedFunction(); /* 14.Init TrustedFunction. */
#endif

#if (OS_AUTOSAR_CORES > 1)
    Os_InitRpc(); /* 15.Init RPC. */
#endif

#if (TRUE == CFG_FAULT_MANAGEMENT_ENABLE)
    Os_InitFaultManagement(); /* 16.Init fault. */
#endif

#if (TRUE == CFG_OS_MONITOR_ENABLE) /* PRQA S 3332 */ /* VL_Os_3332 */
    Os_InitOsMonitor(coreId);
#endif

#if (CFG_BARRIER_MAX > 0U)
    Os_BarrierInit();
#endif

#if (CFG_ARTI_ENABLE == TRUE)
    Arti_Init();
#endif
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * The function init os kernel variable in MultiCore.
 */
OS_LOCAL void Os_MultiCoreInitKernel(void)
{
    Os_CoreIdType coreId = Os_GetCoreIdLocal();
    Os_SCBType *pScb = Os_GetSystemContext(coreId);

    pScb->SysIsrNestQueue = Os_SysIsrNestQueue_Inf[coreId];
    pScb->PriorityNum = Os_CfgPriorityMax_Inf[coreId];
    pScb->QueueMg = Os_ReadyQueMg_Inf[coreId];
    pScb->SystemStack = Os_SystemStack_Inf[coreId];
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * StartOS synchronizes all cores twice. The first
 *                         synchronization point is located before the StartupHooks
 *                         are executed, the second after the OS-Application specific
 *                         StartupHooks have finished and before the scheduler is started.
 */
void Os_SynPoint(uint8 point)
{
    uint16 coreId = 0U;
    /* PRQA S 4404 ++ */ /* VL_Os_4404 */
    uint16 localCoreId = Os_GetCoreIdLocal();
    /* PRQA S 4404 -- */

    if (point <= SYNC_POINTS_MAX)
    {
        /* PRQA S 4404 ++ */ /* VL_Os_4404 */
        Os_CoreCB.CoreStateSynPoint[point][localCoreId] = TRUE;
        /* PRQA S 4404 -- */
        while (coreId < OS_AUTOSAR_CORES)
        {
            /* PRQA S 3442, 1881 ++ */ /* VL_Os_3442, VL_Os_1881 */
            if (TRUE == Os_CoreCB.CoreStateSynPoint[point][coreId])
            /* PRQA S 3442, 1881 -- */
            {
                coreId++;
            }
        }
    }
    else
    {
        Os_Panic();
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Check the AppMode when calling StartOS in multicore.
 */
/* PRQA S 6030, 6010 ++ */ /* VL_MTR_Os_STMIF, VL_MTR_Os_STCYC */
OS_LOCAL Os_AppModeType Os_CheckAppMode(Os_AppModeType mode)
/* PRQA S 6030, 6010 -- */
{
    Os_AppModeType retAppMode = DONOTCARE;
    Os_CoreIdType coreId = Os_GetCoreIdLocal();

    /* It is not allowed to call StartOS on cores activated by StartNonAutosarCore.*/
    if (OS_AUTOSAR_CORES <= coreId)
    {
        while (1) /* PRQA S 2740 */ /* VL_Os_2740 */
        {
            /* Nothing to do. */
        }
    }

    /* Multi core state Synchronous point0, for check start os appmode. */
    Os_CoreCB.AppMode[coreId] = mode;
    Os_SynPoint(0U);

    /* SWS_Os_00609: if StartOS is called with the AppMode "DONOTCARE"
     * the application mode of the other core(s) (differing from
     * "DONOTCARE") shall be used. */
    for (Os_CoreIdType loop_core = 0u; loop_core < OS_AUTOSAR_CORES; loop_core++)
    {
        /* PRQA S 3442 ++ */ /* VL_Os_3442 */
        if (DONOTCARE != Os_CoreCB.AppMode[loop_core])
        /* PRQA S 3442 -- */
        {
            if (DONOTCARE == retAppMode)
            {
                retAppMode = Os_CoreCB.AppMode[loop_core];
            }
            else
            {
                /* PRQA S 3442 ++ */ /* VL_Os_3442 */
                if (retAppMode != Os_CoreCB.AppMode[loop_core])
                /* PRQA S 3442 -- */
                {
                    while (1) /* PRQA S 2740 */ /* VL_Os_2740 */
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
        while (1) /* PRQA S 2740 */ /* VL_Os_2740 */
        {
            /* Nothing to do. */
        }
    }
    else
    {
        for (Os_CoreIdType loop_core = 0u; loop_core < OS_AUTOSAR_CORES; loop_core++)
        {
            Os_CoreCB.AppMode[loop_core] = retAppMode;
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
/* PRQA S 1505 ++ */ /* VL_Os_1505 */
void Os_DeInitCoreInfo(void)
/* PRQA S 1505 -- */
{
    Os_CoreIdType localCoreId = Os_GetCoreIdLocal();

/*clear the CoreCB info*/
#if (OS_AUTOSAR_CORES > 1U)
    Os_GetInternalSpinlock(&Os_SpinlockSync);
#endif
    /* PRQA S 3442, 4397 ++ */ /* VL_Os_3442, VL_Os_4397 */
    Os_CoreCB.CoreStateActive &= (Os_CoreIdType)(~((Os_CoreIdType)1 << localCoreId));
    /* PRQA S 3442, 4397 -- */
    Os_CoreCB.AppMode[localCoreId] = OS_NULL_APPMODE;
    Os_CoreCB.CoreStatus[localCoreId] = OS_CORE_STATUS_INVALID;
#if (OS_AUTOSAR_CORES > 1U)
    Os_ReleaseInternalSpinlock(&Os_SpinlockSync);
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Os panic
 */
void Os_Panic(void) /* PRQA S 3408 */ /* VL_Os_3408 */
{
    Os_Hal_DisableInt();
    Os_PanicHandler();

    while (1) /* PRQA S 2740 */ /* VL_Os_2740 */
    {
        /* system crash. */
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/*=======[E N D   O F   F I L E]==============================================*/
