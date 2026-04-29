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
 **  FILENAME    : Os_Appl.c                                                   **
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

/*=======[I N C L U D E S]====================================================*/
#include "Os_Internal.h"

/*=======[M A C R O S]========================================================*/
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
/* The size of the minimum access bit unit for one application */
/* DD_1_0176 */
#define OS_APPACCBITUNIT_SIZE (sizeof(ApplicationType) * CHAR_BIT)

/*the access application bit position based on the access bit unit size*/
#define OS_APPGETACCESS_BP(osAppId) ((osAppId) % OS_APPACCBITUNIT_SIZE)

/* PRQA S 3472 ++ */ /* VL_Os_3472 */
/*the access application bit group*/
/*osAppId /OS_APPACCBITUNIT_SIZE*/
#define OS_APPGETACCESS_GP(osAppId) ((osAppId) >> 4u)

/*the access mask of the application*/
#define OS_APPACCESS_MASK(osAppId) ((uint16)1u << OS_APPGETACCESS_BP(osAppId))
/* PRQA S 3472 -- */
#endif /* OS_SC3 == CFG_SC || OS_SC4 == CFG_SC */
/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#if (CFG_OSAPPLICATION_MAX > 0U)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Init the Application control block>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * PreCondition         <None>
 * CallByAPI            <Os_InitSystem>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_InitApplication(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    ApplicationType i;
    uint16          vCoreId = Os_SCB.sysCore;
    /* PRQA S 0310, 0311 ++ */ /* VL_Os_0310, VL_Os_0311 */
    Os_ObjectAppCfg = (const Os_ObjectAppCfgType**)((void**)Os_ObjectAppCfg_Inf[vCoreId]);
    /* PRQA S 0310, 0311 -- */
    Os_SCB.sysAppId = Os_AppSysId_Inf[vCoreId];

    Os_SCB.sysRunningAppID = Os_SCB.sysAppId;
    for (i = 0u; i < CFG_OSAPPLICATION_MAX; i++)
    {
        Os_AppCB[i].appState = APPLICATION_ACCESSIBLE;
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (CFG_STD_RESOURCE_MAX > 0)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <task or ISR terminate its resource occupied before>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Yes>
 * @param[in]           <ResID>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_AppTerminateIsrKernelProc>
 *                      <Os_ApplTerminateOneTask>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 6030, 6010, 3450 ++ */ /* VL_MTR_Os_STMIF, VL_MTR_Os_STCYC, VL_Os_3450 */
static void Os_ApplReleaseResource(ResourceType ResID, ObjectTypeType Object, Os_AppObjectIDType OsAppObjID)
/* PRQA S 6030, 6010, 3450 -- */
{
    Os_PriorityType prio;
    StatusType      err = E_OK;
    Os_PriorityType savePrioTemp;
    Os_TCBType*     pTcb;
    Os_ICBType*     pIcb;

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* No any resource to release */
    if (0U == Os_RCB[ResID].saveCount)
    {
        /*nothing to do*/
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
    {
        prio         = Os_ResourceCfg[ResID].ceiling;
        savePrioTemp = Os_RCB[ResID].savePrio;

        switch (Os_ResourceCfg[ResID].resourceOccupyType)
        {
        case OCCUPIED_BY_TASK:
            pTcb = &Os_TCB[OsAppObjID];

            if (Os_TaskCfg[OsAppObjID].osTaskPriority > prio)
            {
                err = E_NOT_OK;
                break; /* PRQA S 3333 */ /* VL_Os_3333 */
            }

            if (pTcb->taskResCount > 0u)
            {
                pTcb->taskResCount = pTcb->taskResCount - 1u;
            }

            if (prio > savePrioTemp)
            {
                Os_ReadyQueueRemove(OS_LEVEL_STANDARD_RESOURCE, Os_TCB[OsAppObjID].taskRunPrio);
                pTcb->taskRunPrio = Os_TaskCfg[OsAppObjID].osTaskPriority;
            }

            break;

        case OCCUPIED_BY_TASK_OR_INTERRUPT:
            pTcb = &Os_TCB[OsAppObjID];

            if (OBJECT_TASK == Object)
            {
                if (pTcb->taskResCount > 0u)
                {
                    pTcb->taskResCount = pTcb->taskResCount - 1u;
                }

                Os_ArchSetIpl(Os_PrioToIpl(savePrioTemp), OS_ISR_ENABLE);
                Os_SCB.sysDispatchLocker = Os_SCB.sysDispatchLocker - 1u;
            }
            else if (OBJECT_ISR == Object)
            {
                pIcb = &Os_ICB[OsAppObjID];

#if (TRUE == CFG_INT_NEST_ENABLE)
                if (prio > savePrioTemp)
                {
                    Os_ArchSetIpl(Os_PrioToIpl(savePrioTemp), OS_ISR_ENABLE);
                }
#endif

/* AutoSar: add ResID to stack. For protection hook. */
#if (CFG_ISR2_MAX > 0)
                if (pIcb->IsrC2ResCount > 0u)
                {
                    pIcb->IsrC2ResCount = pIcb->IsrC2ResCount - 1u;
                }
#endif
            }
            else
            {
                /*nothing to do*/
            }
            break;

        case OCCUPIED_BY_INTERRUPT:
            pIcb = &Os_ICB[OsAppObjID];

#if (TRUE == CFG_INT_NEST_ENABLE)
            if (prio > savePrioTemp)
            {
                Os_ArchSetIpl(Os_PrioToIpl(savePrioTemp), OS_ISR_ENABLE);
            }
#endif

/* AutoSar: add ResID to stack. For protection hook. */
#if (CFG_ISR2_MAX > 0)
            if (pIcb->IsrC2ResCount > 0u)
            {
                pIcb->IsrC2ResCount = pIcb->IsrC2ResCount - 1u;
            }
#endif

            break;

        /*add comments to pass QAC.*/
        default:
            /* Nothing to do */
            break;
        }

        if ((StatusType)E_OK == err)
        {
/* AutoSar SC2: Timing protection, resource lock. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
            Os_TmProtResEnd(ResID);
#endif

            Os_RCB[ResID].saveCount = 0u;
            Os_RCB[ResID].savePrio  = OS_PRIORITY_INVALID;
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* CFG_STD_RESOURCE_MAX > 0 */

#if (CFG_ISR_MAX > 0)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <free the ISR running control block and the resources occupied>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Yes>
 * Param-Name[in]       <OsIsrID>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_TerminateISRObjectHandler>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 6010, 6080, 3450 ++ */ /* VL_MTR_Os_STCYC, VL_MTR_Os_STPTH, VL_Os_3450 */
static void Os_AppTerminateIsrKernelProc(Os_IsrType OsIsrID)
/* PRQA S 6010, 6080, 3450 -- */
{
#if ((TRUE == CFG_TIMING_PROTECTION_ENABLE) || (CFG_STD_RESOURCE_MAX > 0) || (CFG_SPINLOCK_MAX > 0U))
    Os_ICBType* pOsICB;
#endif

#if (CFG_SPINLOCK_MAX > 0U)
    SpinlockIdType spinLockIdx;
    SpinlockIdType SpinlockId;
#endif /* CFG_SPINLOCK_MAX > 0U */
#if (CFG_STD_RESOURCE_MAX > 0)
    Os_IsrType      i;
    Os_ResourceType osTemp;
#endif

    if (OsIsrID < Os_CfgIsr2Max)
    {
#if ((TRUE == CFG_TIMING_PROTECTION_ENABLE) || (CFG_STD_RESOURCE_MAX > 0) || (CFG_SPINLOCK_MAX > 0U))
        pOsICB = &Os_ICB[OsIsrID];
#endif

/* Timing protection. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        Os_TmProtIsrEnd(OsIsrID, TP_ISR_CAT2_EXE);

        pOsICB->IsrC2IsrOpt = TP_OPT_BUTT;

        if (pOsICB->isrC2DisableAllCount > 0u)
        {
            Os_TmProtIsrEnd(OsIsrID, TP_ISR_CAT2_DIS_ALL_INT);

            pOsICB->isrC2DisableAllCount = 0u;
            Os_ArchEnableAllInt_ButTimingProtInt();
        }

        if (pOsICB->isrC2SuspendAllCount > 0u)
        {
            Os_TmProtIsrEnd(OsIsrID, TP_ISR_CAT2_SUS_ALL_INT);

            Os_SuspendAllCount -= pOsICB->isrC2SuspendAllCount;
            if (0u == Os_SuspendAllCount)
            {
                Os_ArchEnableAllInt_ButTimingProtInt();
            }
        }

        if (pOsICB->isrC2SuspendOSCount > 0u)
        {
            Os_TmProtIsrEnd(OsIsrID, TP_ISR_CAT2_SUS_OS_INT);

            Os_SuspendOsCount -= pOsICB->isrC2SuspendOSCount;
            if (0u == Os_SuspendOsCount)
            {
                Os_ArchSetIpl(Os_SaveOsIntNested, OS_ISR_ENABLE);
            }
        }
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */

/* Release resource occupied by this isr. */
#if (CFG_STD_RESOURCE_MAX > 0)
        osTemp = pOsICB->IsrC2ResCount;
        if (osTemp > 0u)
        {
            for (i = osTemp; i > 0u; i--)
            {
                Os_ApplReleaseResource(pOsICB->IsrC2ResourceStack[i - 1u], OBJECT_ISR, OsIsrID);
            }
        }
#endif

#if (CFG_SPINLOCK_MAX > 0U)
        for (spinLockIdx = pOsICB->isr2CriticalZoneCount; spinLockIdx > 0u; spinLockIdx--)
        {
            if (OBJECT_SPINLOCK == pOsICB->isr2CriticalZoneType[pOsICB->isr2CriticalZoneCount - 1u])
            {
                SpinlockId = pOsICB->isr2CriticalZoneStack[pOsICB->isr2CriticalZoneCount - 1u];
                (void)Os_ReleaseSpinlock(SpinlockId);
            }
            else
            {
                pOsICB->isr2CriticalZoneCount--;
            }
        }
#endif
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Terminate the ISR objects of the calling application>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Yes>
 * Param-Name[in]       <pOsAppIsrRef: the application specified Isrs ref>
 *                      <OsAppIsrCnt: the number or size of the Isrs >
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_TerminateObjects>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3450 ++ */ /* VL_Os_3450 */
static void Os_TerminateISRObjectHandler(const Os_IsrType* pOsAppIsrRef, Os_IsrType OsAppIsrCnt)
/* PRQA S 3450 -- */
{
    Os_IsrType OsIsrLoopi;

    if ((0u == OsAppIsrCnt) || (Os_CfgIsrMax < OsAppIsrCnt))
    {
        /*nothing to do*/
    }
    else
    {
        for (OsIsrLoopi = 0u; OsIsrLoopi < OsAppIsrCnt; OsIsrLoopi++)
        {
            /* MISRA-C: 17.4  Array subscripting applied to an object of pointer type.
            pOsAppIsrRef is pointor of static_cfg_array, OsAppIsrCnt is the size of array,
            it must be used like below. */

            /* Arch platform related process. */
#if (TRUE == CFG_INT_NEST_ENABLE)
            Os_ArchAppTerminateIsrProc(Os_GetObjLocalId(pOsAppIsrRef[OsIsrLoopi]));
#endif
            Os_AppTerminateIsrKernelProc(Os_GetObjLocalId(pOsAppIsrRef[OsIsrLoopi]));
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* CFG_ISR_MAX > 0 */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Removing task_node from ready_queue in TerminateApplication.>
 * ServiceId           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Yes>
 * Param-Name[in]      <OsTaskID>
 *                     <OsQueuePriority>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ApplTerminateOneTask>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3450, 2755 ++ */ /* VL_Os_3450, VL_Os_2755 */
static void Os_ApplReadyQueueRemove(Os_TaskType OsTaskID, Os_PriorityType OsQueuePriority)
/* PRQA S 3450, 2755 -- */
{
    uint32             OsQueueHead;
    uint32             OsQueueTail;
    uint32             OsQueueSize;
    Os_ReadyQueueType* pOsReadyQueueMark;
    Os_TaskType*       pOsReadyQueue;

    pOsReadyQueue     = Os_ReadyQueue[OsQueuePriority];
    pOsReadyQueueMark = &Os_ReadyQueueMark[OsQueuePriority];

    /* Remove one node from Os_ReadyQueue and shift to the left. */
    OsQueueHead = pOsReadyQueueMark->queueHead;
    OsQueueTail = pOsReadyQueueMark->queueTail;

    OsQueueSize = Os_ActivateQueueSize[OsQueuePriority];

    /*Retrieve the previous position of the queue's tail.*/
    /* PRQA S 2834 ++ */ /* VL_Os_2834 */
    uint32 OsQueueTailLast = (OsQueueTail + (OsQueueSize - 1U)) % OsQueueSize;
    /* PRQA S 2834 -- */

    /* Queue is empty. */
    if ((OsQueueHead == OsQueueTail) && (OS_TASK_INVALID == pOsReadyQueue[OsQueueTailLast]))
    {
        /*nothing to do*/
    }
    else
    {
        uint32 queueFront = OsQueueHead;
        /* PRQA S 2834 ++ */ /* VL_Os_2834 */
        uint32 loopWidth =
            (OsQueueHead == OsQueueTail) ? OsQueueSize : ((OsQueueTail + OsQueueSize - OsQueueHead) % OsQueueSize);
        /* PRQA S 2834 -- */
        uint32 hitCount = 0u;

        do
        {
            /*If the queue element is equal to OsTaskID, increment hitCount.*/
            /* PRQA S 2834 ++ */ /* VL_Os_2834 */
            while ((OsTaskID == pOsReadyQueue[(queueFront + hitCount) % OsQueueSize]) && (hitCount < loopWidth))
            /* PRQA S 2834 -- */
            {
                hitCount++;
                OsQueueTail = (OsQueueTail + (OsQueueSize - 1U)) % OsQueueSize; /* PRQA S 2834 */ /* VL_Os_2834 */
            }

            /*If, after moving OsQueueTail, queueFront becomes equal to OsQueueTail, exit the loop.*/
            if ((queueFront == OsQueueTail) && (hitCount > 0U))
            {
                break;
            }

            if (hitCount > 0U)
            {
                /* PRQA S 2834 ++ */ /* VL_Os_2834 */
                pOsReadyQueue[queueFront] = pOsReadyQueue[(queueFront + hitCount) % OsQueueSize];
                /* PRQA S 2834 -- */
            }

            queueFront = (queueFront + 1u) % OsQueueSize; /* PRQA S 2834 */ /* VL_Os_2834 */

        } while (queueFront != OsQueueTail);

        /*Set the deleted QueueTail as invalid.*/
        while (0U != hitCount)
        {
            /* PRQA S 2834 ++ */ /* VL_Os_2834 */
            pOsReadyQueue[(OsQueueTail + hitCount) % OsQueueSize] = OS_TASK_INVALID;
            /* PRQA S 2834 -- */
            hitCount = hitCount - 1u;
        }
        /*Update the QueueTail.*/
        pOsReadyQueueMark->queueTail = OsQueueTail;

        /* Queue for this prio is empty. */
        if (pOsReadyQueueMark->queueHead == pOsReadyQueueMark->queueTail)
        {
            /* Clear prio_has_task bit flag of Os_ReadyMap. */
            Os_ClearPrioReadyMap(OsQueuePriority);
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Terminate one task.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Yes>
 * Param-Name[in]       <OsTaskID>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_TerminateTaskObjectHandler>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 6010, 6070, 6080, 3450 ++ */ /* VL_MTR_Os_STCYC, VL_MTR_Os_STCAL, VL_MTR_Os_STPTH, VL_Os_3450 */
static StatusType Os_ApplTerminateOneTask(Os_TaskType OsTaskID)
/* PRQA S 6010, 6070, 6080, 3450 -- */
{
#if (CFG_STD_RESOURCE_MAX > 0U)
    uint16          i;
    Os_ResourceType osTemp;
#endif /* CFG_STD_RESOURCE_MAX > 0 */
#if (CFG_SPINLOCK_MAX > 0U)
    SpinlockIdType spinLockIdx;
    SpinlockIdType SpinlockId;
#endif /* CFG_SPINLOCK_MAX > 0U */
    StatusType  osRet = E_OK;
    Os_TCBType* pTcb;

    pTcb = &Os_TCB[OsTaskID];

    /* Terminated task no need to process. */
    if (TASK_STATE_SUSPENDED == pTcb->taskState)
    {
        osRet = E_NOT_OK;
    }
    else
    {
        if (TASK_STATE_START == pTcb->taskState)
        {
            osRet = E_NOT_OK;
        }
        else
        {
#if (CFG_INTERNAL_RESOURCE_MAX > 0U)
            Os_ReleaseInternalResource(OsTaskID);
#endif

/* Release resource occupied by this task. */
#if (CFG_STD_RESOURCE_MAX > 0u)
            if (pTcb->taskResCount > 0u)
            {
                osTemp = pTcb->taskResCount;

                for (i = osTemp; i > 0u; i--)
                {
                    Os_ApplReleaseResource(pTcb->taskResourceStack[i - 1u], OBJECT_TASK, OsTaskID);
                }
            }
#endif

#if (CFG_SPINLOCK_MAX > 0U)
            for (spinLockIdx = pTcb->taskCriticalZoneCount; spinLockIdx > 0u; spinLockIdx--)
            {
                if (OBJECT_SPINLOCK == pTcb->taskCriticalZoneType[pTcb->taskCriticalZoneCount - 1u])
                {
                    SpinlockId = pTcb->taskCriticalZoneStack[pTcb->taskCriticalZoneCount - 1u];
                    (void)Os_ReleaseSpinlock(SpinlockId);
                }
                else
                {
                    pTcb->taskCriticalZoneCount--;
                }
            }
#endif

            /*If the current task is a non-preemptible task and is in the running state,
             * the scheduling lock should be reduced by one*/
            if ((TASK_STATE_RUNNING == pTcb->taskState) && (OS_PREEMPTIVE_NON == Os_TaskCfg[OsTaskID].osTaskSchedule))
            {
                Os_SCB.sysDispatchLocker = Os_SCB.sysDispatchLocker - 1U;
            }
        }
        /* Reset task_state. */
        pTcb->taskState = TASK_STATE_SUSPENDED;
        /* Remove from ready_queue. */
        Os_ApplReadyQueueRemove(OsTaskID, pTcb->taskRunPrio);

#if (CFG_STD_RESOURCE_MAX > 0U)
        pTcb->taskResCount = 0U;
#endif

#if ((OS_BCC2 == CFG_CC) || (OS_ECC2 == CFG_CC))
        pTcb->taskActCount = 0u;
#endif

        pTcb->taskRunPrio = Os_TaskCfg[OsTaskID].osTaskPriority;

#if (CFG_EXTENDED_TASK_MAX > 0U)
        if (OsTaskID < Os_CfgExtendTaskMax)
        {
            Os_ECB[OsTaskID].eventSetEvent  = 0U;
            Os_ECB[OsTaskID].eventWaitEvent = 0U;
        }
#endif

        Os_SCB.sysHighPrio   = Os_GetHighPrio();
        Os_SCB.sysHighTaskID = Os_ReadyQueueGetFirst(Os_SCB.sysHighPrio);

/* Timing protection. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        /* Task_exe budget. */
        Os_TmProtTaskEnd(OsTaskID, TP_TASK_EXE);

        /* Task isr_timing_protection process. */
        pTcb->taskIsrOpt = TP_OPT_BUTT;
        if (pTcb->taskDisableAllCount > 0u)
        {
            Os_TmProtTaskEnd(OsTaskID, TP_TASK_DIS_ALL_INT);
            pTcb->taskDisableAllCount = 0u;
            Os_ArchEnableAllInt_ButTimingProtInt();
        }

        if (pTcb->taskSuspendAllCount > 0u)
        {
            Os_TmProtTaskEnd(OsTaskID, TP_TASK_SUS_ALL_INT);
            Os_SuspendAllCount -= pTcb->taskSuspendAllCount;
            if (0u == Os_SuspendAllCount)
            {
                Os_ArchEnableAllInt_ButTimingProtInt();
            }
        }

        if (pTcb->taskSuspendOSCount > 0u)
        {
            Os_TmProtTaskEnd(OsTaskID, TP_TASK_SUS_OS_INT);
            Os_SuspendOsCount -= pTcb->taskSuspendOSCount;
            if (0u == Os_SuspendOsCount)
            {
                Os_ArchSetIpl(Os_SaveOsIntNested, OS_ISR_ENABLE);
            }
        }
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */
    }

    return osRet;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Terminate the task objects of the calling application>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Yes>
 * Param-Name[in]       <pOsAppTaskRef: the application specified tasks ref>
 *                      <OsAppTaskCnt: the task count of this app.>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_TerminateObjects>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3450 ++ */ /* VL_Os_3450 */
static void Os_TerminateTaskObjectHandler(const Os_TaskType* pOsAppTaskRef, Os_TaskType OsAppTaskCnt)
/* PRQA S 3450 -- */
{
    Os_TaskType i;
    Os_TaskType osTaskId;
    StatusType  osRet;

    /*check the count to be valid */
    if ((0u == OsAppTaskCnt) || (Os_SCB.sysTaskMax <= OsAppTaskCnt))
    {
        /*nothing to do*/
    }
    else
    {
        for (i = 0u; i < OsAppTaskCnt; i++)
        {
            osTaskId = Os_GetObjLocalId(pOsAppTaskRef[i]);
            osRet    = Os_ApplTerminateOneTask(osTaskId);

            /* Os_App terminate task arch_related process. */
            if ((StatusType)E_OK == osRet)
            {
                Os_ArchAppTerminateTaskProc(osTaskId);
            }
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (CFG_ALARM_MAX > 0)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Terminate the Alarm objects of the calling application>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Yes>
 * Param-Name[in]       <pOsAppAlarmRef: the application specified Alarms ref>
 *                      <OsAppAlarmCnt : the count/size of the pOsAppAlarmRef>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_TerminateObjects>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3450 ++ */ /* VL_Os_3450 */
static void Os_TerminateAlarmObjectHandler(const Os_AlarmType* pOsAppAlarmRef, Os_AlarmType OsAppAlarmCnt)
/* PRQA S 3450 -- */
{
    Os_AlarmType i;
    /*check the count to be valid */
    if ((0u == OsAppAlarmCnt) || (Os_SCB.sysAlarmMax < OsAppAlarmCnt))
    {
        /*nothing to do*/
    }
    else
    {
        for (i = 0u; i < OsAppAlarmCnt; i++)
        {
            (void)Os_CancelAlarm(pOsAppAlarmRef[i]);
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* CFG_ALARM_MAX > 0 */

#if (CFG_SCHEDTBL_MAX > 0)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Terminate the ScheduleTable objects of the calling application>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Yes>
 * Param-Name[in]       <pOsAppSchTblRef: the application specified scheduleTbls ref>
 *                      <OsAppSchTblCnt : the count/size of the pOsAppSchTblRef>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_TerminateObjects>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3450 ++ */  /* VL_Os_3450 */
static void Os_TerminateScheduleTblObjectHandler(
/* PRQA S 3450 -- */
    const Os_ScheduleTableType* pOsAppSchTblRef,
    Os_ScheduleTableType        OsAppSchTblCnt)
{
    Os_ScheduleTableType i;
    Os_CallLevelType     sysCallLevel;

    /*check the count to be valid */
    if ((0u == OsAppSchTblCnt) || (CFG_SCHEDTBL_MAX < OsAppSchTblCnt))
    {
        /*nothing to do*/
    }
    else
    {
        /* in order to pass service protection*/
        sysCallLevel      = Os_SCB.sysOsLevel;
        Os_SCB.sysOsLevel = OS_LEVEL_TASK;
        for (i = 0u; i < OsAppSchTblCnt; i++)
        {
            (void)Os_StopScheduleTable(pOsAppSchTblRef[i]);
        }
        Os_SCB.sysOsLevel = sysCallLevel;
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* CFG_SCHEDTBL_MAX > 0 */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Terminate the objects belonging to the calling application>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Yes>
 * Param-Name[in]       <pOsCurAppCfg: the application config reference>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <TerminateApplication>
 * REQ ID               <DD_1_1013, DD_1_1014>
 */
/******************************************************************************/
/* PRQA S 3450 ++ */ /* VL_Os_3450 */
static void Os_TerminateObjects(const Os_ApplicationCfgType* pOsCurAppCfg)
/* PRQA S 3450 -- */
{
#if (CFG_ALARM_MAX > 0)
    /**
     * stop all active alarms
     */
    Os_TerminateAlarmObjectHandler(
        (Os_AlarmRefType)pOsCurAppCfg->OsAppObjectRef[OBJECT_ALARM],
        pOsCurAppCfg->OsAppAlarmRefCnt);
#endif /* CFG_ALARM_MAX > 0 */
/**
 * All other interrupt sources should be already disabled in above handling,
 * which as one precondition here.
 */
#if (CFG_ISR_MAX > 0)
    /**
     * Terminating ISR should check the current level to kill and free current
     * stack if in ISR Level, and take care if support nested ISRS. Besides,
     * As the resources occupied either on Task or ISR, here should free the resources
     * occupied on the ISR to continue the ISR terminating operation. And which occupied
     * on the Tasks will be handled while in the following terminating the Task objects.
     */
    /* MISRA-C: 17.4  Array subscripting applied to an object of pointer type.
       OsAppObjectRef is pointor of static_cfg_array, OsAppIsrRefCnt is the size of array,
       it must be used like below. */
    Os_TerminateISRObjectHandler((Os_IsrType*)pOsCurAppCfg->OsAppObjectRef[OBJECT_ISR], pOsCurAppCfg->OsAppIsrRefCnt);
#endif /* CFG_ISR_MAX > 0 */

#if (CFG_TASK_MAX > 0)
    /**
     * Terminate all the tasks of the application, kick out the queued request and free
     * all of the control block context, also will free the resources occupied on them.
     */
    Os_TerminateTaskObjectHandler(
        (Os_TaskRefType)pOsCurAppCfg->OsAppObjectRef[OBJECT_TASK],
        pOsCurAppCfg->OsAppTaskCnt);
#endif /* CFG_TASK_MAX > 0 */

#if (CFG_SCHEDTBL_MAX > 0)
    /**
     * stop all schedule tables
     */
    Os_TerminateScheduleTblObjectHandler(
        (Os_ScheduleTableType*)pOsCurAppCfg->OsAppObjectRef[OBJECT_SCHEDULETABLE],
        pOsCurAppCfg->OsAppScheduleTableCnt);
#endif /* CFG_SCHEDTBL_MAX > 0 */

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <This service determines the currently running
 *                      OS-Application (a unique identifier has to be
 *                      allocated to each application)>
 * Service ID           <0x27>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Return value         <OS-Application id>
 * PreCondition         <SC3 and SC4>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3006, 1503, 6070 ++ */ /* VL_Os_3006, VL_QAC_NoUsedApi, VL_MTR_Os_STCAL */
ApplicationType GetCurrentApplicationID(void)
/* PRQA S 3006, 1503, 6070 -- */
{
    /* PRQA S 2741, 2742, 2880, 3138 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2741, 2742, 2880, 3138 -- */
    ApplicationType ApplID = INVALID_OSAPPLICATION;
    StatusType      status = E_OK;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_GetApplicationID);
#endif /* TRUE == CFG_TRACE_ENABLE */

    OS_ARCH_DECLARE_CRITICAL();

/*service protection*/
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    if (Os_WrongContext(OS_CONTEXT_GET_CURRENT_APPLICATION_ID) != TRUE)
    {
        status = E_OS_CALLEVEL;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        status = E_OS_DISABLEDINT;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
        OS_ARCH_ENTRY_CRITICAL();

        /*OS798: return the identifier in which the current Task/ISR/hook is executed*/
        ApplID = Os_SCB.sysRunningAppID;
        if (ApplID < CFG_OSAPPLICATION_MAX) /* OS799 */
        {
            if (Os_SCB.sysAppId == ApplID)
            {
                ApplID = INVALID_OSAPPLICATION;
            }
        }
        else
        {
            ApplID = INVALID_OSAPPLICATION;
        }

        OS_ARCH_EXIT_CRITICAL();
    }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK)
    {
	/* PRQA S 3138 ++ */ /* VL_Os_3138 */
        Os_TraceErrorHook(OSError_Save_GetCurrentApplicationID(), OSServiceId_GetCurrentApplicationID, status);
	/* PRQA S 3138 -- */
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_GetApplicationID);
#endif /* TRUE == CFG_TRACE_ENABLE */

    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    UNUSED_PARAMETER(status);
    return ApplID;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <This service sets the own state of an OS-Application
 *                         from APPLICATION_RESTARTING to APPLICATION_ACCESSIBLE.>
 * Service ID           <0x13>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Return value         <StatusType>
 * PreCondition         <SC3 and SC4>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3006, 1503, 6070 ++ */ /* VL_Os_3006, VL_QAC_NoUsedApi, VL_MTR_Os_STCAL */
StatusType AllowAccess(void)
/* PRQA S 3006, 1503, 6070 -- */
{
    /* PRQA S 2741, 2742, 2880, 3138 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2741, 2742, 2880, 3138 -- */
    StatusType err = E_OK;
    OS_ARCH_DECLARE_CRITICAL();

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    if (Os_WrongContext(OS_CONTEXT_ALLOW_ACCESS) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        err = E_OS_DISABLEDINT;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
        OS_ARCH_ENTRY_CRITICAL();
        /* PRQA S 3442 ++ */ /* VL_Os_3442 */
        if (APPLICATION_RESTARTING != Os_AppCB[Os_SCB.sysRunningAppID].appState)
        /* PRQA S 3442 -- */
        {
            OS_ARCH_EXIT_CRITICAL();
            err = E_OS_STATE;
        }
        else
        {
            Os_AppCB[Os_SCB.sysRunningAppID].appState = APPLICATION_ACCESSIBLE;
            OS_ARCH_EXIT_CRITICAL();
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
	/* PRQA S 3138 ++ */ /* VL_Os_3138 */
        Os_TraceErrorHook(OSError_Save_AllowAccess(), OSServiceId_AllowAccess, err);
	/* PRQA S 3138 -- */
    }
#endif
    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <This service returns the current state of an OS-Application.>
 * Service ID           <0x14>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Return value         <StatusType>
 * PreCondition         <SC3 and SC4>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 6030, 3006, 1503 ++ */ /* VL_MTR_Os_STMIF, VL_Os_3006, VL_QAC_NoUsedApi */
StatusType GetApplicationState(ApplicationType Application, ApplicationStateRefType Value)
/* PRQA S 6030, 3006, 1503 -- */
{
    /* PRQA S 2741, 2742, 2880, 3138 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2741, 2742, 2880, 3138 -- */
    StatusType err = E_OK;

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (NULL_PTR == Value)
    {
        err = E_OS_PARAM_POINTER;
    }
    else if (CFG_OSAPPLICATION_MAX <= Application)
    {
        err = E_OS_ID;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_GET_APPLICATION_STATE) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    else if (Os_AddressWritable((uint32)Value) != TRUE) /* PRQA S 0306 */ /* VL_Os_0306 */
    {
        err = E_OS_ILLEGAL_ADDRESS;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        err = E_OS_DISABLEDINT;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
        *Value = Os_AppCB[Application].appState;
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
	/* PRQA S 3138 ++ */ /* VL_Os_3138 */
        Os_TraceErrorHook(OSError_Save_GetApplicationState(Application, Value), OSServiceId_GetApplicationState, err);
	/* PRQA S 3138 -- */
    }
#endif

    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <This service determines if the OS-Applications,
 *                       given by ApplID, is allowed to use the IDs of a
 *                       Task, ISR, Resource, Counter, Alarm or Schedule
 *                       Table in API calls.>
 * Service ID           <0x05>
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
/* PRQA S 6030, 3006, 1503, 6070 ++ */ /* VL_MTR_Os_STMIF, VL_Os_3006, VL_QAC_NoUsedApi, VL_MTR_Os_STCAL */
ObjectAccessType CheckObjectAccess(ApplicationType ApplID, ObjectTypeType ObjectType, AppObjectId ObjectID)
/* PRQA S 6030, 3006, 1503, 6070 -- */
{
    /* PRQA S 2741, 2742, 2880, 3138 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2741, 2742, 2880, 3138 -- */
    const Os_AppObjectId* pObjectIDMaxTableRef;
    ObjectAccessType      err    = NO_ACCESS;
    StatusType            Status = E_OK;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_CheckObjectAccess);
#endif /* TRUE == CFG_TRACE_ENABLE */

    pObjectIDMaxTableRef = Os_ObjectIDMaxTable_Inf[Os_GetObjCoreId(ObjectID)];

/*service protection*/
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    if (Os_WrongContext(OS_CONTEXT_CHECK_OBJECT_ACCESS) != TRUE)
    {
        Status = E_OS_CALLEVEL;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        Status = E_OS_DISABLEDINT;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */

        if (ApplID >= CFG_OSAPPLICATION_MAX)
        {
            err = NO_ACCESS;
        }
        /*OS423: if the objectType is not a valid , return NO_ACCESS*/
        else if (ObjectType >= OBJECT_MAX)
        {
            err = NO_ACCESS;
        }
        else if (Os_GetObjLocalId(ObjectID) >= pObjectIDMaxTableRef[ObjectType])
        {
            err = NO_ACCESS;
        }
        else
        {
            err = Os_CheckObjectAccess(ApplID, ObjectType, ObjectID);
        }

#if (CFG_ERRORHOOK == TRUE)
    if (Status != E_OK)
    {
        Os_TraceErrorHook(
            OSError_Save_CheckObjectAccess(ApplID, ObjectType, ObjectID),
            OSServiceId_CheckObjectAccess,
            Status); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_CheckObjectAccess);
#endif /* TRUE == CFG_TRACE_ENABLE */

    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */

    UNUSED_PARAMETER(Status);
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Terminate Application>
 * ServiceId            <0x12>
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
/* PRQA S 6030, 6010, 3006, 1503, 6070 ++ */ /* VL_MTR_Os_STMIF, VL_MTR_Os_STCYC, VL_Os_3006, VL_QAC_NoUsedApi, VL_MTR_Os_STCAL */
StatusType TerminateApplication(ApplicationType Application, RestartType RestartOption)
/* PRQA S 6030, 6010, 3006, 1503, 6070 -- */
{
    /* PRQA S 2741, 2742, 2880, 3138 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2741, 2742, 2880, 3138 -- */

    StatusType err = E_OK;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_TerminateApplication);
#endif /* TRUE == CFG_TRACE_ENABLE */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /*SWS_Os_00493*/
    if (CFG_OSAPPLICATION_MAX <= Application)
    {
        err = E_OS_ID;
    }
    /*SWS_Os_00459: if the <RestartOption> is invalid, return E_OS_VALUE*/
    else if ((RESTART != RestartOption) && (NO_RESTART != RestartOption))
    {
        err = E_OS_VALUE;
    }
    /*SWS_Os_00494*/
    else if ((FALSE == Os_AppCfg[Os_SCB.sysRunningAppID].OsTrusted) && (Application != Os_SCB.sysRunningAppID))
    {
        err = E_OS_ACCESS;
    }
    /*SWS_Os_00507*/
    /* PRQA S 3442 ++ */ /* VL_Os_3442 */
    else if (APPLICATION_TERMINATED == Os_AppCB[Application].appState)
    {
        err = E_OS_STATE;
    }
    /*SWS_Os_00508,SWS_Os_00548*/
    else if (
        (APPLICATION_RESTARTING == Os_AppCB[Application].appState)
        && ((Os_SCB.sysRunningAppID != Application) || (RESTART == RestartOption)))
    /* PRQA S 3442 -- */
    {
        err = E_OS_STATE;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        /*OS088*/
        if (Os_WrongContext(OS_CONTEXT_TERMINATE_APPLICATION) != TRUE)
        {
            err = E_OS_CALLEVEL;
        }
        else if (Os_IgnoreService() != TRUE)
        {
            err = E_OS_DISABLEDINT;
        }
        else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
        {
#if (OS_AUTOSAR_CORES > 1)
            Os_CoreIdType coreId = Os_AppCfg[Application].OsHostCore;
            if (coreId != Os_SCB.sysCore)
            {
                RpcInputType rpcData = {
                    .sync         = RPC_SYNC,
                    .remoteCoreId = coreId,
                    .serviceId    = OSServiceId_TerminateApplication,
                    .srvPara0     = (uint32)Application,
                    .srvPara1     = (uint32)RestartOption,
                    /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral  */
                    .srvPara2 = (uint32)NULL_PARA,
                    /* PRQA S 1258 -- */
                };
                err = Os_RpcCallService(&rpcData);
            }
            else
#endif /* OS_AUTOSAR_CORES > 1 */
            {
                Os_TerminateApplication(Application, RestartOption);
            }
        }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(
            OSError_Save_TerminateApplication(Application, RestartOption),
            OSServiceId_TerminateApplication,
            err); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_TerminateApplication);
#endif /* TRUE == CFG_TRACE_ENABLE */

    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
/* PRQA S 6030, 6010, 6070 ++ */ /* VL_MTR_Os_STMIF, VL_MTR_Os_STCYC, VL_MTR_Os_STCAL */
/* PRQA S 6050, 3006 ++ */       /* VL_MTR_Os_STST3, VL_Os_3006 */
void Os_TerminateApplication(ApplicationType Application, RestartType RestartOption)
/* PRQA S 6050, 3006 -- */
/* PRQA S 6030, 6010, 6070 -- */
{
    Os_CoreIdType                coreId;
    Os_CoreIdType                coreIndex;
    Os_CallLevelType             sysCallLevel;
    ApplicationStateType         tempAppState;
    const Os_ApplicationCfgType* posCurAppCfg;

    OS_ARCH_DECLARE_CRITICAL();
    OS_ARCH_ENTRY_CRITICAL();
    sysCallLevel = Os_SCB.sysOsLevel;
    posCurAppCfg = &Os_AppCfg[Application];
    OS_ARCH_EXIT_CRITICAL();

    OS_ARCH_SUSPEND_ALLINT();
    if (APPLICATION_ACCESSIBLE != Os_AppCB[Application].appState) /* PRQA S 3442 */ /* VL_Os_3442 */
    {
        /* nothing to do */
    }
    else
    {
        /*@OS287: If Called from allowed context, terminate the calling OS-Application*/
        Os_TerminateObjects(posCurAppCfg);

/* OS447: Disable the interrupt source of the OsIsrs owned by the application */
#if (CFG_ISR_MAX > 0)
        Os_ArchDisableIntInApp(posCurAppCfg);
#endif

        if (RESTART == RestartOption)
        {
            Os_AppCB[Application].appState = APPLICATION_RESTARTING;
        }
        else
        {
            Os_AppCB[Application].appState = APPLICATION_TERMINATED;
        }

        /*@OS346: if RESTART, activate the OsRestartTask of the application*/
        if ((RESTART == RestartOption) && (Os_SCB.sysTaskMax > posCurAppCfg->OsRestartTask))
        {
            /* Set OsLevel to OS_LEVEL_TASK in order to call ActivateTask.
             * ActivateTask can not be called in OS_LEVEL_ERRORHOOK_APP level. */
            if (OS_LEVEL_ERRORHOOK_APP == sysCallLevel)
            {
                Os_SCB.sysOsLevel = OS_LEVEL_TASK;
            }
            coreId       = Os_SCB.sysCore;
            coreIndex    = coreId << 12; /* PRQA S 4544, 3120 */ /*  VL_Os_4544, VL_QAC_MagicNum */
            tempAppState = Os_AppCB[Application].appState;

            Os_AppCB[Application].appState = APPLICATION_ACCESSIBLE;

            /*Activate the RestartTask*/
            Os_SCB.sysDispatchLocker = Os_SCB.sysDispatchLocker + (Os_LockerType)1;
            /* PRQA S 1520 ++ */ /* VL_Os_1520 */
            (void)Os_ActivateTask((coreIndex | posCurAppCfg->OsRestartTask));
            /* PRQA S 1520 -- */
            Os_SCB.sysDispatchLocker = Os_SCB.sysDispatchLocker - (Os_LockerType)1;

            Os_AppCB[Application].appState = tempAppState;

            /* In level OS_LEVEL_ERRORHOOK_APP, restore OsLevel. */
            if (OS_LEVEL_ERRORHOOK_APP == sysCallLevel)
            {
                Os_SCB.sysOsLevel = sysCallLevel;
            }
        }

        if (Application == Os_SCB.sysRunningAppID)
        {
#if (CFG_ISR2_MAX > 0)
            if (TRUE == Os_SCB.sysInIsrCat2)
            {
                /* If this service is called by isr_c2, sysRunningTaskID can be
                 * set to IDLE_TASK, in order to call Os_SwitchTask in OS_ARCH_ISR2_EPILOGUE. */
                /* Tasks of this OsApp are all terminated, the sysRunningTaskID is also might be terminated,
                 * so Os_SwitchTask must be called in OS_ARCH_ISR2_EPILOGUE. */
                /*If called context is in ISR2, Now Exit */
                OS_ARCH_EXIT_CRITICAL();
                /* PRQA S 3442, 2481, 0306 ++ */ /* VL_Os_3442, VL_Os_2481, VL_Os_0306 */
                /* PRQA S 0310, 2743, 3415 ++ */ /* VL_Os_0310, VL_Os_2743, VL_Os_3415 */
                /* PRQA S 0404, 3345 ++ */       /* VL_Os_VolatileAccess */
                /* PRQA S 1006, 0499, 4397 ++ */ /* VL_Os_1006, VL_Os_0499, VL_Os_4397 */
                /* PRQA S 1520 ++ */ /* VL_Os_1520 */
                OS_ARCH_ISR2_EPILOGUE_KILL_ISR();
                /* PRQA S 1520 -- */
                /* PRQA S 1006, 0499, 4397 -- */
                /* PRQA S 0404, 3345 -- */
                /* PRQA S 0310, 2743, 3415 -- */
                /* PRQA S 3442, 2481, 0306 -- */
            }
            else
#endif /* CFG_ISR2_MAX > 0 */
            {
                /* Task dispatch. */
                Os_SCB.sysOsLevel        = OS_LEVEL_TASK;
                Os_SCB.sysDispatchLocker = 0u;

#if (TRUE == CFG_TRACE_ENABLE)
                Os_TraceTaskSwitch(
                    Os_SCB.sysRunningTaskID,
                    Os_SCB.sysHighTaskID,
                    OS_TRACE_TASK_SWITCH_REASON_APP_TERMINATE,
                    OS_TRACE_TASK_SWITCH_REASON_APP_TERMINATE_ACTIVE);
#endif

                OS_START_DISPATCH(); /* PRQA S 3141, 3138 */ /* VL_Os_PlatformNoDef  */
                Os_Dispatch(); /* PRQA S 1290, 3138 */       /* VL_Os_1290, VL_Os_PlatformDef */
            }
        }
    }

    OS_ARCH_RESTORE_ALLINT();
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
ObjectAccessType Os_CheckObjectAccess(ApplicationType ApplID, ObjectTypeType ObjectType, AppObjectId ObjectID)
{
    ObjectAccessType       err = ACCESS;
    ApplicationType        osAccAppNodePos;
    ApplicationType        osAccAppBitPos;
    const ApplicationType* pOsAccAppRef;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    const Os_ObjectAppCfgType** pObjectAppCfgRef;
    /* PRQA S 3678 -- */

    OS_ARCH_DECLARE_CRITICAL();
    /* PRQA S 0310, 0311 ++ */ /* VL_Os_0310, VL_Os_0311 */
    pObjectAppCfgRef = (const Os_ObjectAppCfgType**)((void**)Os_ObjectAppCfg_Inf[Os_GetObjCoreId(ObjectID)]);
    /* PRQA S 0310, 0311 -- */

    /*MultiCore: Change to local id*/
    ObjectID = Os_GetObjLocalId(ObjectID); /* PRQA S 1338 */ /* VL_Os_1338 */

    /*OS423: if the ApplID is invalid , return NO_ACCESS*/
    OS_ARCH_ENTRY_CRITICAL();

    if ((OBJECT_RESOURCE == ObjectType) || (OBJECT_SPINLOCK == ObjectType))
    {
        /*Object do not belong to any application*/
    }
    else
    {
        /* SWS_Os_00504, SWS_Os_00509 */
        /* PRQA S 3442 ++ */ /* VL_Os_3442 */
        if ((pObjectAppCfgRef[ObjectType][ObjectID].hostApp != Os_SCB.sysRunningAppID)
            && (Os_AppCB[pObjectAppCfgRef[ObjectType][ObjectID].hostApp].appState != APPLICATION_ACCESSIBLE))
        /* PRQA S 3442 -- */
        {
            OS_ARCH_EXIT_CRITICAL();
            err = NO_ACCESS;
        }
    }

    if ((ObjectAccessType)ACCESS == err)
    {
/* OS318: if the object type is OBJECT_RESOURCE and the object is
 * RES_SCHEDULER , return ACCESS */
#if (TRUE == CFG_USERESSCHEDULER)
        if ((OBJECT_RESOURCE == ObjectType) && (RES_SCHEDULER == ObjectID))
        {
            err = ACCESS;
        }
        else
#endif /* TRUE == CFG_USERESSCHEDULER */
        {
            /* ApplID in which bit_group(uint16). */
            osAccAppNodePos = OS_APPGETACCESS_GP(ApplID);
            /* ApplID in which bit_pos of uint16. */
            /* PRQA S 1840, 0582 ++ */ /* VL_Os_1840, VL_Os_0582 */
            osAccAppBitPos = OS_APPACCESS_MASK(ApplID);
            /* PRQA S 1840, 0582 -- */
            /* Access right bit map. */
            pOsAccAppRef = pObjectAppCfgRef[ObjectType][ObjectID].accAppRef;

            /* Check object access right by app_id. */
            if ((osAccAppBitPos & pOsAccAppRef[osAccAppNodePos]) > 0u)
            {
                err = ACCESS;
            }
            else
            {
                err = NO_ACCESS;
            }
        }
        OS_ARCH_EXIT_CRITICAL();
    }

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* OS_SC3 == CFG_SC || OS_SC4 == CFG_SC */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <This service determines the OS-Application (a unique
 *                         identifier has to be allocated to each application)
 *                         where the caller originally belongs to (was configured to)>
 * Service ID           <0x00>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Return value         <OS-Application id>
 * PreCondition         <SC3 and SC4>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3006, 1503 ++ */ /* VL_Os_3006, VL_QAC_NoUsedApi */
ApplicationType GetApplicationID(void)
/* PRQA S 3006, 1503 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006, 3141 ++ */ /* VL_Os_1006, VL_Os_3141 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006, 3141 -- */
    /* PRQA S 2742, 2880, 3138, 2741 -- */
    ApplicationType ApplID = INVALID_OSAPPLICATION;
    StatusType      status = E_OK;
    OS_ARCH_DECLARE_CRITICAL();

/*service protection*/
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    if (Os_WrongContext(OS_CONTEXT_GET_APPLICATION_ID) != TRUE)
    {
        status = E_OS_CALLEVEL;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
        OS_ARCH_ENTRY_CRITICAL();
/*OS261: return the identifier to which the executing Task/ISR/hook was configured*/
#if (CFG_TRUSTED_SYSTEM_SERVICE_MAX > 0U)
        if (0u != Os_TrustedFuncNest)
        {
            ApplID = Os_TrustedFuncNestQueue[Os_TrustedFuncNest - 1U];
        }
        else
#endif /* CFG_TRUSTED_SYSTEM_SERVICE_MAX > 0U */
        {
            ApplID = Os_SCB.sysRunningAppID;
        }

        if (ApplID < CFG_OSAPPLICATION_MAX) /* OS262 */
        {
            if (Os_SCB.sysAppId == ApplID)
            {
                ApplID = INVALID_OSAPPLICATION;
            }
        }
        else
        {
            ApplID = INVALID_OSAPPLICATION;
        }

        OS_ARCH_EXIT_CRITICAL();
    }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK) /* PRQA S 2996, 2992 */ /* VL_Os_2996, VL_Os_2992 */
    {
        /* PRQA S 3138, 2880 ++ */ /* VL_Os_3138, VL_Os_2880 */
        Os_TraceErrorHook(OSError_Save_GetApplicationID(), OSServiceId_GetApplicationID, status);
        /* PRQA S 3138, 2880 -- */
    }
#endif

    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    UNUSED_PARAMETER(status);
    return ApplID;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Internal implementation of OS service:CheckObjectOwnership>
 * Service ID           <OSServiceId_CheckObjectOwnership>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-Name[in]       <ObjectType>
 *                      <ObjectID>
 * Return value         <OS-Application id>
 * PreCondition         <SC3 and SC4>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3450 ++ */ /* VL_Os_3450 */
static ApplicationType Os_CheckObjectOwnership(ObjectTypeType ObjectType, AppObjectId ObjectID)
/* PRQA S 3450 -- */
{
    ApplicationType       ApplID = INVALID_OSAPPLICATION;
    const Os_AppObjectId* pObjectIDMaxTableRef;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    const Os_ObjectAppCfgType** pObjectAppCfgRef;
    /* PRQA S 3678 -- */

    OS_ARCH_DECLARE_CRITICAL();

    /* PRQA S 0311 ++ */ /* VL_Os_0311 */
    pObjectAppCfgRef = (const Os_ObjectAppCfgType**)Os_ObjectAppCfg_Inf[Os_GetObjCoreId(ObjectID)];
    /* PRQA S 0311 -- */
    pObjectIDMaxTableRef = Os_ObjectIDMaxTable_Inf[Os_GetObjCoreId(ObjectID)];

    /*MultiCore: Change to local id*/
    ObjectID = Os_GetObjLocalId(ObjectID); /* PRQA S 1338 */ /* VL_Os_1338 */

    if (ObjectType >= OBJECT_MAX)
    {
        ApplID = INVALID_OSAPPLICATION;
    }
    else
    {
        if (ObjectID < pObjectIDMaxTableRef[ObjectType])
        {
            OS_ARCH_ENTRY_CRITICAL();
/* OS318: if the object type is OBJECT_RESOURCE and the object is
 * RES_SCHEDULER , return ACCESS */
#if (TRUE == CFG_USERESSCHEDULER)
            if ((OBJECT_RESOURCE == ObjectType) && (RES_SCHEDULER == ObjectID))
            {
                ApplID = INVALID_OSAPPLICATION;
            }
            else
#endif /* TRUE == CFG_USERESSCHEDULER */
            {
                ApplID = pObjectAppCfgRef[ObjectType][ObjectID].hostApp;
            }
            OS_ARCH_EXIT_CRITICAL();
        }
        else
        {
            ApplID = INVALID_OSAPPLICATION;
        }
    }

    return ApplID;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <This service determines to which OS-Application
 *                      a given Task, ISR, Resource, Counter, Alarm or
 *                      Schedule Table belongs>
 * Service ID           <0x06>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-Name[in]       <ObjectType>
 *                      <ObjectID>
 * Return value         <OS-Application id>
 * PreCondition         <SC3 and SC4>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3006, 1503, 6070 ++ */ /* VL_Os_3006, VL_QAC_NoUsedApi, VL_MTR_Os_STCAL */
ApplicationType CheckObjectOwnership(ObjectTypeType ObjectType, AppObjectId ObjectID)
/* PRQA S 3006, 1503, 6070 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006, 3141 ++ */ /* VL_Os_1006, VL_Os_3141 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006, 3141 -- */
    /* PRQA S 2742, 2880, 3138, 2741 -- */
    ApplicationType ApplID = INVALID_OSAPPLICATION;
    StatusType      Status = E_OK;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_CheckObjectOwnership);
#endif /* TRUE == CFG_TRACE_ENABLE */

/*service protection*/
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    if (Os_WrongContext(OS_CONTEXT_CHECK_OBJECT_OWNERSHIP) != TRUE)
    {
        Status = E_OS_CALLEVEL;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        Status = E_OS_DISABLEDINT;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
        ApplID = Os_CheckObjectOwnership(ObjectType, ObjectID);
    }

#if (CFG_ERRORHOOK == TRUE)
    if (Status != E_OK) /* PRQA S 2996, 2992 */ /* VL_Os_2996, VL_Os_2992 */
    {
        /* PRQA S 2880, 3138, 3141 ++ */ /* VL_Os_2880, VL_Os_3138, VL_Os_3141 */
        Os_TraceErrorHook(
            OSError_Save_CheckObjectOwnership(ObjectType, ObjectID),
            OSServiceId_CheckObjectOwnership,
            Status);
        /* PRQA S 2880, 3138, 3141 -- */
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_CheckObjectOwnership);
#endif /* TRUE == CFG_TRACE_ENABLE */

    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    UNUSED_PARAMETER(Status);
    return ApplID;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /*CFG_OSAPPLICATION_MAX >0U */
/*=======[E N D   O F   F I L E]==============================================*/
/* PRQA S 6530 EOF */ /* VL_MTR_Os_STECT */
/* PRQA S 0553 EOF */ /* VL_QAC_UnUsedFiles */
