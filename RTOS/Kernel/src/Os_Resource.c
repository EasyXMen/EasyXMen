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
 **  FILENAME    : Os_Resource.c                                               **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      :  i-soft-os                                                  **
 **  Vendor      :                                                             **
 **  DESCRIPTION : resource manager                                            **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
 **  Version :   AUTOSAR classic Platform R19--Function Safety                 **
 **                                                                            **
 *******************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Os_Internal.h"

#if (CFG_RESOURCE_MAX > 0U)
/*=======[M A C R O S]========================================================*/

/*=======[I N T E R N A L   D A T A]==========================================*/
#define OS_START_SEC_VAR_CLONE_16
#include "Os_MemMap.h"
static uint16 Os_CfgStdResourceMax; /* PRQA S 3218 */ /* VL_Os_3218 */
#define OS_STOP_SEC_VAR_CLONE_16
#include "Os_MemMap.h"

#if (OS_STATUS_EXTENDED == CFG_STATUS)
#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
/* PRQA S 3678 ++ */ /* VL_Os_3678 */
static const uint16** Os_TaskResourceAccessMask;
/* PRQA S 3678 -- */
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
/* PRQA S 3678 ++ */ /* VL_Os_3678 */
static const uint16** Os_IsrResourceAccessMask;
/* PRQA S 3678 -- */
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */

#if (CFG_INTERNAL_RESOURCE_MAX > 0U)
#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
static Os_PriorityType* Os_OccupyInterRes;
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

#endif /* CFG_INTERNAL_RESOURCE_MAX > 0U */

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
/* PRQA S 6010, 6080, 1532 ++ */ /* VL_MTR_Os_STCYC, VL_MTR_Os_STPTH, VL_QAC_OneFunRef */
void Os_InitResource(void)
/* PRQA S 6010, 6080, 1532 -- */
{
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    uint16 i = 0U;
#endif
#if (CFG_STD_RESOURCE_MAX > 0U)
    Os_ResourceType resId;
    Os_RCBType*     pRcb;

#endif
    uint16 vCoreId = Os_SCB.sysCore;

#if (CFG_INTERNAL_RESOURCE_MAX > 0U)
    Os_InterResCeiling = Os_InterResCeiling_Inf[vCoreId];
    Os_OccupyInterRes  = Os_OccupyInterRes_Inf[vCoreId];
#endif

#if (CFG_STD_RESOURCE_MAX > 0U)
    if (Os_CfgStdResourceMax_Inf[vCoreId] > 0U)
    {
#if (TRUE == CFG_USERESSCHEDULER)
        RES_SCHEDULER = Os_ResScheduler_Inf[vCoreId];
#endif

#if (OS_STATUS_EXTENDED == CFG_STATUS)
        /*OS_RESOURCE_CAST_TYPE_006*/
        /* PRQA S 0310, 0311 ++ */ /* VL_Os_0310, VL_Os_0311 */
        Os_TaskResourceAccessMask = (const uint16**)((void**)Os_TaskResourceAccessMask_Inf[vCoreId]);
        Os_IsrResourceAccessMask  = (const uint16**)((void**)Os_IsrResourceAccessMask_Inf[vCoreId]);
        /* PRQA S 0310, 0311 -- */
#endif

        Os_ResourceCfg       = Os_ResourceCfg_Inf[vCoreId];
        Os_RCB               = Os_RCB_Inf[vCoreId];
        Os_CfgResourceMax    = Os_CfgResourceMax_Inf[vCoreId];
        Os_CfgStdResourceMax = Os_CfgStdResourceMax_Inf[vCoreId];

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        if (TRUE == Os_CfgTprot_Inf[vCoreId])
        {
            Os_RCB_Inf[vCoreId][i].osTmProtResBgtTask = NULL_PTR;
            Os_RCB_Inf[vCoreId][i].osTmProtResBgtIsr  = NULL_PTR;
            for (i = 0U; i < Os_CfgStdResourceMax; i++)
            {
                Os_RCB_Inf[vCoreId][i].saveLevel = OS_LEVEL_MAIN;
            }
            Os_TickType* pRCBTmProtResBgtTask = Os_RCBTmProtResBgtTask_Inf[vCoreId];
            if ((Os_SCB.sysTaskMax > 0U) && (NULL_PTR != pRCBTmProtResBgtTask))
            {
                for (i = 0U; i < Os_CfgStdResourceMax; i++)
                {
                    /* PRQA S 0488 ++ */ /* VL_Os_0488 */
                    Os_RCB_Inf[vCoreId][i].osTmProtResBgtTask = pRCBTmProtResBgtTask + (Os_SCB.sysTaskMax * i);
                    /* PRQA S 0488 -- */
                }
            }
            Os_TickType* pRCBTmProtResBgtIsr = Os_RCBTmProtResBgtIsr_Inf[vCoreId];
            if ((Os_CfgIsrMax > 0U) && (NULL_PTR != pRCBTmProtResBgtIsr))
            {
                for (i = 0U; i < Os_CfgStdResourceMax; i++)
                {
                    /* PRQA S 0488 ++ */ /* VL_Os_0488 */
                    Os_RCB_Inf[vCoreId][i].osTmProtResBgtIsr = pRCBTmProtResBgtIsr + (Os_CfgIsrMax * i);
                    /* PRQA S 0488 -- */
                }
            }
        }
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */
    }
#endif /* CFG_STD_RESOURCE_MAX > 0 */

#if (CFG_STD_RESOURCE_MAX > 0U)
    for (resId = 0U; resId < Os_CfgStdResourceMax; resId++)
    {
        pRcb = &Os_RCB[resId];

        pRcb->saveCount = 0u;
        pRcb->savePrio  = OS_PRIORITY_INVALID;

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        pRcb->osWhichTaskOccupy = OS_TASK_INVALID;
        pRcb->osWhichIsrOccupy  = INVALID_ISR;

/* Autosar SC2: for timing protection. Budget for resoruce lock init
 * to invalid. Invalid means not be configed. */
#if (CFG_TASK_MAX > 0)
        if (NULL_PTR != pRcb->osTmProtResBgtTask)
        {
            for (i = 0U; i < Os_SCB.sysTaskMax; i++)
            {
                /* PRQA S 1258 ++ */  /* VL_Os_1258 */
                pRcb->osTmProtResBgtTask[i] = OS_TICK_INVALID;
                /* PRQA S 1258 -- */
            }
        }
#endif

#if (CFG_ISR_MAX > 0)
        if (NULL_PTR != pRcb->osTmProtResBgtIsr)
        {
            for (i = 0U; i < Os_CfgIsrMax; i++)
            {
                /* PRQA S 1258 ++ */  /* VL_Os_1258 */
                pRcb->osTmProtResBgtIsr[i] = OS_TICK_INVALID;
                /* PRQA S 1258 -- */
            }
        }
#endif

        pRcb->osResTpData.osIsTpStart = FALSE;
        pRcb->osResTpData.osTpBudget  = 0u;
        pRcb->osResTpData.osTpTime    = 0u;
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */
    }
#endif /* CFG_STD_RESOURCE_MAX > 0 */

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (CFG_STD_RESOURCE_MAX > 0U)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Save the runtime environment and sequence of GetResource.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <pTCB:Task control block>
 *                      <ResID:Resource identifier>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <StatusType>
 * PreCondition         <None>
 * CallByAPI            <task or ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
static inline void Os_SaveResourceByTaskOrder(Os_TCBType* pTCB, ResourceType ResID)
{
    Os_RCB[ResID].saveLevel = Os_SCB.sysOsLevel;
    if (pTCB->taskResCount < Os_CfgResourceMax)
    {
        pTCB->taskResourceStack[pTCB->taskResCount] = ResID;
    }
    pTCB->taskResCount      = pTCB->taskResCount + 1u;
    Os_RCB[ResID].saveCount = pTCB->taskResCount;
#if (CFG_SPINLOCK_MAX > 0U)
    pTCB->taskCriticalZoneType[pTCB->taskCriticalZoneCount]  = OBJECT_RESOURCE;
    pTCB->taskCriticalZoneStack[pTCB->taskCriticalZoneCount] = ResID;
    pTCB->taskCriticalZoneCount                              = pTCB->taskCriticalZoneCount + 1u;
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Resume the runtime environment and sequence of Resource.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <pTCB:Task control block>
 *                      <ResID:Resource identifier>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <StatusType>
 * PreCondition         <None>
 * CallByAPI            <task or ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
static inline void Os_ResumeResourceByTaskOrder(Os_TCBType* pTCB, ResourceType ResID)
{
    pTCB->taskResCount      = pTCB->taskResCount - 1u;
    Os_RCB[ResID].saveCount = 0u;
/*res and spinlock should together release as LIFO order*/
#if (CFG_SPINLOCK_MAX > 0U)
    pTCB->taskCriticalZoneCount                              = pTCB->taskCriticalZoneCount - 1u;
    pTCB->taskCriticalZoneType[pTCB->taskCriticalZoneCount]  = OBJECT_MAX;
    pTCB->taskCriticalZoneStack[pTCB->taskCriticalZoneCount] = OS_OBJECT_INVALID;
#endif
    Os_RCB[ResID].saveLevel = OS_LEVEL_MAIN;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Save the runtime environment and sequence of GetResource.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <pICB:Interrupt control block>
 *                      <ResID:Resource identifier>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <StatusType>
 * PreCondition         <None>
 * CallByAPI            <task or ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
static inline void Os_SaveResourceByIsr2Order(Os_ICBType* pICB, ResourceType ResID)
{
    Os_RCB[ResID].saveLevel = Os_SCB.sysOsLevel;
    if (pICB->IsrC2ResCount < Os_CfgResourceMax)
    {
        pICB->IsrC2ResourceStack[pICB->IsrC2ResCount] = ResID;
    }
    pICB->IsrC2ResCount = pICB->IsrC2ResCount + 1u;
    /* PRQA S 4461 ++ */ /* VL_Os_4461 */
    Os_RCB[ResID].saveCount = pICB->IsrC2ResCount;
    /* PRQA S 4461 -- */
/* Multi core res and spinlock should together release as
 * LIFO order*/
#if (CFG_SPINLOCK_MAX > 0U)
    pICB->isr2CriticalZoneType[pICB->isr2CriticalZoneCount]  = OBJECT_RESOURCE;
    pICB->isr2CriticalZoneStack[pICB->isr2CriticalZoneCount] = ResID;
    pICB->isr2CriticalZoneCount                              = pICB->isr2CriticalZoneCount + 1u;
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Resume the runtime environment and sequence of Resource.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <pICB:Interrupt control block>
 *                      <ResID:Resource identifier>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <StatusType>
 * PreCondition         <None>
 * CallByAPI            <task or ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
static inline void Os_ResumeResourceByIsr2Order(Os_ICBType* pICB, ResourceType ResID)
{
    pICB->IsrC2ResCount     = pICB->IsrC2ResCount - 1u;
    Os_RCB[ResID].saveCount = 0u;
/*res and spinlock should together release as LIFO order*/
#if (CFG_SPINLOCK_MAX > 0U)
    pICB->isr2CriticalZoneCount                              = pICB->isr2CriticalZoneCount - 1u;
    pICB->isr2CriticalZoneType[pICB->isr2CriticalZoneCount]  = OBJECT_MAX;
    pICB->isr2CriticalZoneStack[pICB->isr2CriticalZoneCount] = OS_OBJECT_INVALID;
#endif
    Os_RCB[ResID].saveLevel = OS_LEVEL_MAIN;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get resources of type OCCUPIED_BY_TASK at the task level.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <pTCB:Task control block>
 *                      <ResID:Resource identifier>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <StatusType>
 * PreCondition         <None>
 * CallByAPI            <task or ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3450 ++ */ /* VL_Os_3450 */
static void Os_GetResourceByTask(Os_TCBType* pTCB, ResourceType ResID)
/* PRQA S 3450 -- */
{
    Os_PriorityType savePrioTemp = 0U;
    Os_PriorityType prio         = Os_ResourceCfg[ResID].ceiling;

    Os_SaveResourceByTaskOrder(pTCB, ResID);

    /* it's need to change these codes about CFG_PRIORITY_MAX.in fact,
     * the ceiling priority is related with CFG_PRIORITY_MAX but not
     * CFG_PRIORITY_MAX.otherwise this way will restrict the space of
     * TASK_STATE_READY map
     */
    savePrioTemp = pTCB->taskRunPrio;
    if (prio > pTCB->taskRunPrio)
    {
        Os_ReadyQueueInsert(ResID, OS_LEVEL_STANDARD_RESOURCE, prio);
        pTCB->taskRunPrio = prio;
    }
    if (prio > Os_SCB.sysHighPrio)
    {
        Os_SCB.sysHighPrio   = prio;
        Os_SCB.sysHighTaskID = Os_SCB.sysRunningTaskID;
    }

    Os_RCB[ResID].savePrio = savePrioTemp;
    /* AutoSar SC2: Timing protection, resource lock. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    if (Os_TaskCfg[Os_SCB.sysRunningTaskID].osTaskTmProtCfgRef != NULL_PTR)
    {
        if (Os_TaskCfg[Os_SCB.sysRunningTaskID].osTaskTmProtCfgRef->osTaskResLockCnt > 0U)
        {
            Os_TmProtResStart(ResID, TP_RES_OCCUPY_TASK);
        }
    }
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get resources of type OCCUPIED_BY_TASK_OR_INTERRUPT at the task level.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <pTCB:Task control block>
 *                      <ResID:Resource identifier>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <StatusType>
 * PreCondition         <None>
 * CallByAPI            <task or ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3450 ++ */ /* VL_Os_3450 */
static void Os_GetResourceByTaskOrInterrupt(Os_TCBType* pTCB, ResourceType ResID)
/* PRQA S 3450 -- */
{
    Os_PriorityType savePrioTemp = 0U;
    Os_PriorityType prio         = Os_ResourceCfg[ResID].ceiling;

    Os_SaveResourceByTaskOrder(pTCB, ResID);

    savePrioTemp = Os_IplToPrio(Os_ArchGetIpl());
    Os_ArchSetIpl(Os_PrioToIpl(prio), OS_ISR_DISABLE);
    Os_SCB.sysDispatchLocker = Os_SCB.sysDispatchLocker + 1u;

    Os_RCB[ResID].savePrio = savePrioTemp;
    /* AutoSar SC2: Timing protection, resource lock. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    if (Os_TaskCfg[Os_SCB.sysRunningTaskID].osTaskTmProtCfgRef != NULL_PTR)
    {
        if (Os_TaskCfg[Os_SCB.sysRunningTaskID].osTaskTmProtCfgRef->osTaskResLockCnt > 0U)
        {
            Os_TmProtResStart(ResID, TP_RES_OCCUPY_TASK);
        }
    }
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get OCCUPIED_BY_INTERRUPT or OCCUPIED_BY_INTERRUPT type resources at the interrupt level.
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <pICB:Interrupt control block>
 *                      <ResID:Resource identifier>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <StatusType>
 * PreCondition         <None>
 * CallByAPI            <task or ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3450 ++ */ /* VL_Os_3450 */
static void Os_GetResourceByInterrupt(Os_ICBType* pICB, ResourceType ResID)
/* PRQA S 3450 -- */
{
    Os_PriorityType savePrioTemp = 0U;
#if (TRUE == CFG_INT_NEST_ENABLE)
    Os_PriorityType prio = Os_ResourceCfg[ResID].ceiling;
    savePrioTemp         = Os_IplToPrio(Os_ArchGetIpl());
    if (prio > savePrioTemp)
    {
        Os_ArchSetIpl(Os_PrioToIpl(prio), OS_ISR_DISABLE);
    }
#endif

/* AutoSar: add ResID to stack. For protection hook. */
#if (CFG_ISR2_MAX > 0)
    if (TRUE == Os_SCB.sysInIsrCat2)
    {
        Os_SaveResourceByIsr2Order(pICB, ResID);
    }
#endif /* CFG_ISR2_MAX > 0 */

    Os_RCB[ResID].savePrio = savePrioTemp;
/* AutoSar SC2: Timing protection, resource lock. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    if (Os_IsrCfg[Os_SCB.sysRunningIsrCat2Id].OsIsrTimePt != NULL_PTR)
    {
        if (Os_IsrCfg[Os_SCB.sysRunningIsrCat2Id].OsIsrTimePt->osIsrResLockCnt > 0U)
        {
            Os_TmProtResStart(ResID, TP_RES_OCCUPY_ISR);
        }
    }
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Release resources of type OCCUPIED_BY_TASK at the task level.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <pTCB:Task control block>
 *                      <ResID:Resource identifier>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <StatusType>
 * PreCondition         <None>
 * CallByAPI            <task or ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3450 ++ */ /* VL_Os_3450 */
static StatusType Os_ReleaseResourceByTask(Os_TCBType* pTCB, ResourceType ResID)
/* PRQA S 3450 -- */
{
    Os_PriorityType prio         = Os_ResourceCfg[ResID].ceiling;
    Os_PriorityType savePrioTemp = Os_RCB[ResID].savePrio;
    StatusType      err          = E_OK;

    if (Os_TaskCfg[Os_SCB.sysRunningTaskID].osTaskPriority > prio)
    {
        err = E_OS_ACCESS;
    }
    else if (Os_RCB[ResID].saveCount != pTCB->taskResCount)
    {
        err = E_OS_NOFUNC;
    }
/*res and spinlock should together release as LIFO order*/
#if (CFG_SPINLOCK_MAX > 0U)
    else if (
        (pTCB->taskCriticalZoneType[pTCB->taskCriticalZoneCount - 1u] != OBJECT_RESOURCE)
        || (pTCB->taskCriticalZoneStack[pTCB->taskCriticalZoneCount - 1u] != ResID))
    {
        err = E_OS_NOFUNC;
    }
#endif /* CFG_SPINLOCK_MAX > 0U */
    else
    {
        if (prio > savePrioTemp)
        {
            Os_ReadyQueueRemove(OS_LEVEL_STANDARD_RESOURCE, pTCB->taskRunPrio);
            pTCB->taskRunPrio = savePrioTemp;
        }
        Os_ResumeResourceByTaskOrder(pTCB, ResID);
    }
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Release resources of type OCCUPIED_BY_TASK_OR_INTERRUPT at the task level.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <pTCB:Task control block>
 *                      <ResID:Resource identifier>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <StatusType>
 * PreCondition         <None>
 * CallByAPI            <task or ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3450 ++ */ /* VL_Os_3450 */
static StatusType Os_ReleaseResourceByTaskOrInterrupt(Os_TCBType* pTCB, ResourceType ResID)
/* PRQA S 3450 -- */
{
    Os_PriorityType savePrioTemp = Os_RCB[ResID].savePrio;
    StatusType      err          = E_OK;

    if (Os_RCB[ResID].saveCount != pTCB->taskResCount)
    {
        err = E_OS_NOFUNC;
    }
/*res and spinlock should together release as LIFO order*/
#if (CFG_SPINLOCK_MAX > 0U)
    else if (
        (pTCB->taskCriticalZoneType[pTCB->taskCriticalZoneCount - 1u] != OBJECT_RESOURCE)
        || (pTCB->taskCriticalZoneStack[pTCB->taskCriticalZoneCount - 1u] != ResID))
    {
        err = E_OS_NOFUNC;
    }
#endif /* CFG_SPINLOCK_MAX > 0U */
    else
    {
        Os_ArchSetIpl(Os_PrioToIpl(savePrioTemp), OS_ISR_ENABLE);
        Os_SCB.sysDispatchLocker = Os_SCB.sysDispatchLocker - 1u;

        Os_ResumeResourceByTaskOrder(pTCB, ResID);
    }
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Release OCCUPIED_BY_INTERRUPT or OCCUPIED_BY_INTERRUPT type resources at the interrupt level.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <pICB:Interrupt control block>
 *                      <ResID:Resource identifier>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <StatusType>
 * PreCondition         <None>
 * CallByAPI            <task or ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3450 ++ */ /* VL_Os_3450 */
static StatusType Os_ReleaseResourceByInterrupt(Os_ICBType* pICB, ResourceType ResID)
/* PRQA S 3450 -- */
{
    StatusType err = E_OK;

    if (Os_RCB[ResID].saveCount != pICB->IsrC2ResCount)
    {
        err = E_OS_NOFUNC;
    }
/*res and spinlock should together release as LIFO order*/
#if (CFG_SPINLOCK_MAX > 0U)
    else if (
        (pICB->isr2CriticalZoneType[pICB->isr2CriticalZoneCount - 1u] != OBJECT_RESOURCE)
        || (pICB->isr2CriticalZoneStack[pICB->isr2CriticalZoneCount - 1u] != ResID))
    {
        err = E_OS_NOFUNC;
    }
#endif /* CFG_SPINLOCK_MAX > 0U */
    else
    {
#if (TRUE == CFG_INT_NEST_ENABLE)
        Os_PriorityType savePrioTemp = Os_RCB[ResID].savePrio;
        Os_PriorityType prio         = Os_ResourceCfg[ResID].ceiling;
        if (prio > savePrioTemp)
        {
            Os_ArchSetIpl(Os_PrioToIpl(savePrioTemp), OS_ISR_ENABLE);
        }
#endif

/* AutoSar: add ResID to stack. For protection hook. */
#if (CFG_ISR2_MAX > 0)
        if (TRUE == Os_SCB.sysInIsrCat2)
        {
            Os_ResumeResourceByIsr2Order(pICB, ResID);
        }
#endif
    }
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <task or ISR ocuupy a resource referenced by <ResID >
 *                      <This call serves to enter critical sections in the code
 *                      that are assigned to the resource referenced by <ResID>.
 *                      A critical section shall always be left using
 *                      ReleaseResource.>
 * Service ID           <0xea>
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
/* PRQA S 6010, 6030, 6070, 6080 ++ */ /* VL_MTR_Os_STCYC, VL_MTR_Os_STMIF, VL_MTR_Os_STCAL, VL_MTR_Os_STPTH */
/* PRQA S 3006, 1532, 1503 ++ */ /* VL_Os_3006, VL_QAC_OneFunRef, VL_QAC_NoUsedApi */
StatusType GetResource(ResourceType ResID)
/* PRQA S 3006, 1532, 1503 -- */
/* PRQA S 6010, 6030, 6070, 6080 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    StatusType err = E_OK;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_GetResource);
#endif /* TRUE == CFG_TRACE_ENABLE */

    if (Os_SCB.sysCore != Os_GetObjCoreId(ResID))
    {
        err = E_OS_CORE;
    }
#if (OS_STATUS_EXTENDED == CFG_STATUS)

    else if (Os_GetObjLocalId(ResID) >= Os_CfgStdResourceMax)
    {
        err = E_OS_ID;
    }
#endif /* OS_STATUS_EXTENDED != CFG_STATUS */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    else if (Os_WrongContext(OS_CONTEXT_GET_RESOURCE) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        err = E_OS_DISABLEDINT;
    }
    else if (Os_CheckObjAcs(OBJECT_RESOURCE, ResID) != TRUE)
    {
        err = E_OS_ACCESS;
    }
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    else
    {
        /* OS_RESOURCE_PARAM_MACRO_008 */
        ResID = Os_GetObjLocalId(ResID); /* PRQA S 1338 */ /* VL_Os_1338 */
#if (OS_STATUS_EXTENDED == CFG_STATUS)
        OS_ARCH_DECLARE_CRITICAL();
        OS_ARCH_ENTRY_CRITICAL();
        /* the resource was occupied already */
        if (Os_RCB[ResID].saveCount > 0U)
        {
            err = E_OS_ACCESS;
        }
        /* resource has no accessing authentication. */
        /* PRQA S 3120, 1258 ++ */ /* VL_QAC_MagicNum, VL_Os_ConstToIntegral  */
        else if (
            (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
            && (0U
                == (Os_TaskResourceAccessMask[Os_SCB.sysRunningTaskID][ResID >> 4u] & ((uint32)1u << (ResID & 0x0Fu)))))
        {
            err = E_OS_ACCESS;
        }
        else if (
            (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
            && (0U
                == (Os_IsrResourceAccessMask[Os_SCB.sysRunningIsrCat2Id][ResID >> 4u]
                    & ((uint32)1u << (ResID & 0x0Fu)))))
        /* PRQA S 3120, 1258 -- */
        {
            err = E_OS_ACCESS;
        }
        else
        {
            /* nothing to do */
        }

        OS_ARCH_EXIT_CRITICAL();

        if ((StatusType)E_OK == err)
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
        {
            err = Os_GetResource(ResID);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        /* PRQA S 3138 ++ */ /* VL_Os_3138 */
        Os_TraceErrorHook(OSError_Save_GetResource(ResID), OSServiceId_GetResource, err);
        /* PRQA S 3138 -- */
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_GetResource);
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
/* PRQA S 6070, 1505 ++ */ /* VL_MTR_Os_STCAL, VL_Os_1505 */
StatusType Os_GetResource(ResourceType ResID)
/* PRQA S 6070, 1505 -- */
{
    StatusType err = E_OK;

    Os_ICBType* pICB = &Os_ICB[Os_SCB.sysRunningIsrCat2Id];
    Os_TCBType* pTCB = Os_SCB.sysRunningTCB;

    /* OS_RESOURCE_PARAM_MACRO_008 */
    OS_ARCH_DECLARE_CRITICAL();
    ResID = Os_GetObjLocalId(ResID); /* PRQA S 1338 */ /* VL_Os_1338 */

    OS_ARCH_SUSPEND_ALLINT();
    switch (Os_ResourceCfg[ResID].resourceOccupyType)
    {
    case OCCUPIED_BY_TASK:
#if (OS_STATUS_EXTENDED == CFG_STATUS)
        if (Os_TaskCfg[Os_SCB.sysRunningTaskID].osTaskPriority > Os_ResourceCfg[ResID].ceiling)
        {
            err = E_OS_ACCESS;
        }
        else if (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
        {
            err = E_OS_ACCESS;
        }
        else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
        {
            Os_GetResourceByTask(pTCB, ResID);
#if (TRUE == CFG_TRACE_ENABLE)
            Os_TraceResourceTaskGet(ResID, Os_SCB.sysRunningTaskID);
#endif /* TRUE == CFG_TRACE_ENABLE */
        }
        break;

    case OCCUPIED_BY_TASK_OR_INTERRUPT:
        /* it's necessary to distinguish the call level of task or ISR,
         * but the current means what use only Os_ResourceOccupyType
         * can't resolve it. in fact the way of old kernel is effective
         */
        if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
        {
            Os_GetResourceByTaskOrInterrupt(pTCB, ResID);
#if (TRUE == CFG_TRACE_ENABLE)
            Os_TraceResourceTaskGet(ResID, Os_SCB.sysRunningTaskID);
#endif /* TRUE == CFG_TRACE_ENABLE */
        }
        else
        {
            Os_GetResourceByInterrupt(pICB, ResID);
#if (TRUE == CFG_TRACE_ENABLE)
            Os_TraceResourceIsrGet(ResID, Os_SCB.sysRunningIsrCat2Id);
#endif /* TRUE == CFG_TRACE_ENABLE */
        }
        break;

    case OCCUPIED_BY_INTERRUPT:
        Os_GetResourceByInterrupt(pICB, ResID);
#if (TRUE == CFG_TRACE_ENABLE)
        Os_TraceResourceIsrGet(ResID, Os_SCB.sysRunningIsrCat2Id);
#endif /* TRUE == CFG_TRACE_ENABLE */
        break;

    default:
        Os_Panic();
        break;
    }

    OS_ARCH_RESTORE_ALLINT();

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <task or ISR release a resource occupied before>
 *                      <ReleaseResource is the counterpart of  GetResource and
 *                      serves to leave critical sections in the code that are
 *                      assigned to the resource referenced by <ResID>. > .
 * Service ID           <0xeb>
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
/* PRQA S 6010, 6030, 6070 ++ */ /* VL_MTR_Os_STCYC, VL_MTR_Os_STMIF, VL_MTR_Os_STCAL */
/* PRQA S 3006, 1532, 1503 ++ */ /* VL_Os_3006, VL_QAC_OneFunRef, VL_QAC_NoUsedApi */
StatusType ReleaseResource(ResourceType ResID)
/* PRQA S 3006, 1532, 1503 -- */
/* PRQA S 6010, 6030, 6070 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    StatusType err = E_OK;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_ReleaseResource);
#endif /* TRUE == CFG_TRACE_ENABLE */

    if (Os_SCB.sysCore != Os_GetObjCoreId(ResID))

    {
        err = E_OS_CORE;
    }
#if (OS_STATUS_EXTENDED == CFG_STATUS)

    else if (Os_GetObjLocalId(ResID) >= Os_CfgStdResourceMax)

    {
        err = E_OS_ID;
    }
#endif /* OS_STATUS_EXTENDED != CFG_STATUS */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    else if (Os_WrongContext(OS_CONTEXT_RELEASE_RESOURCE) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        err = E_OS_DISABLEDINT;
    }
    else if (Os_CheckObjAcs(OBJECT_RESOURCE, ResID) != TRUE)
    {
        err = E_OS_ACCESS;
    }
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    else
    {
        /* OS_RESOURCE_PARAM_MACRO_008 */
        ResID = Os_GetObjLocalId(ResID); /* PRQA S 1338 */ /* VL_Os_1338 */
#if (OS_STATUS_EXTENDED == CFG_STATUS)
        OS_ARCH_DECLARE_CRITICAL();
        OS_ARCH_ENTRY_CRITICAL();

        /* means that no any resource to release */
        if (0U == Os_RCB[ResID].saveCount)
        {
            err = E_OS_NOFUNC;
        }
        OS_ARCH_EXIT_CRITICAL();

        if ((StatusType)E_OK == err)
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
        {
            err = Os_ReleaseResource(ResID);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        /* PRQA S 3138 ++ */ /* VL_Os_3138 */
        Os_TraceErrorHook(OSError_Save_ReleaseResource(ResID), OSServiceId_ReleaseResource, err);
        /* PRQA S 3138 -- */
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_ReleaseResource);
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
/* PRQA S 6070 ++ */ /* VL_MTR_Os_STCAL */
StatusType Os_ReleaseResource(ResourceType ResID) /* PRQA S 1505 */ /* VL_Os_1505 */
/* PRQA S 6070 -- */
{
    StatusType err = E_OK;

    OS_ARCH_DECLARE_CRITICAL();

    Os_ICBType* pICB = &Os_ICB[Os_SCB.sysRunningIsrCat2Id];
    Os_TCBType* pTCB = Os_SCB.sysRunningTCB;

    /* OS_RESOURCE_PARAM_MACRO_008 */
    ResID = Os_GetObjLocalId(ResID); /* PRQA S 1338 */ /* VL_Os_1338 */

    OS_ARCH_SUSPEND_ALLINT();

    switch (Os_ResourceCfg[ResID].resourceOccupyType)
    {
    case OCCUPIED_BY_TASK:
        err = Os_ReleaseResourceByTask(pTCB, ResID);

#if (TRUE == CFG_TRACE_ENABLE)
        Os_TraceResourceRelease(ResID);
#endif /* TRUE == CFG_TRACE_ENABLE */
        break;

    case OCCUPIED_BY_TASK_OR_INTERRUPT:
        if (OS_LEVEL_TASK == Os_RCB[ResID].saveLevel)
        {
            err = Os_ReleaseResourceByTaskOrInterrupt(pTCB, ResID);

#if (TRUE == CFG_TRACE_ENABLE)
            Os_TraceResourceRelease(ResID);
#endif /* TRUE == CFG_TRACE_ENABLE */
        }
        else
        {
            err = Os_ReleaseResourceByInterrupt(pICB, ResID);

#if (TRUE == CFG_TRACE_ENABLE)
            Os_TraceResourceRelease(ResID);
#endif /* TRUE == CFG_TRACE_ENABLE */
        }
        break;

    case OCCUPIED_BY_INTERRUPT:
        err = Os_ReleaseResourceByInterrupt(pICB, ResID);

#if (TRUE == CFG_TRACE_ENABLE)
        Os_TraceResourceRelease(ResID);
#endif /* TRUE == CFG_TRACE_ENABLE */
        break;

    default: /* Nothing to do. */
        break;
    }

    if ((StatusType)E_OK == err)
    {
/*Timing protection, resource lock. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        Os_TmProtResEnd(ResID);
#endif

        Os_RCB[ResID].saveCount = 0u;
        Os_RCB[ResID].savePrio  = OS_PRIORITY_INVALID;
        Os_SCB.sysHighPrio      = Os_GetHighPrio();
        Os_SCB.sysHighTaskID    = Os_ReadyQueueGetFirst(Os_SCB.sysHighPrio);

#if (CFG_SCHED_POLICY != OS_PREEMPTIVE_NON)
        if (Os_SCB.sysHighTaskID != Os_SCB.sysRunningTaskID)
        {
            if (0u == Os_SCB.sysDispatchLocker)
            {
#if (TRUE == CFG_TRACE_ENABLE)
                Os_TraceTaskSwitch(
                    Os_SCB.sysRunningTaskID,
                    Os_SCB.sysHighTaskID,
                    OS_TRACE_TASK_SWITCH_REASON_RELEASE_RES_READY,
                    OS_TRACE_TASK_SWITCH_REASON_RELEASE_RES_ACTIVE);
#endif

                OS_START_DISPATCH(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
                Os_Dispatch(); /* PRQA S 1290, 3138 */       /* VL_Os_1290, VL_Os_PlatformDef */
            }
        }
#endif
    }

    OS_ARCH_RESTORE_ALLINT();

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* CFG_STD_RESOURCE_MAX > 0U */

#if (CFG_INTERNAL_RESOURCE_MAX > 0U)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
void Os_GetInternalResource(void)
{
    Os_PriorityType prio;

    if (Os_InterResCeiling[Os_SCB.sysRunningTaskID] > Os_TCB[Os_SCB.sysRunningTaskID].taskRunPrio)
    {
        prio = Os_InterResCeiling[Os_SCB.sysRunningTaskID];

        Os_TCB[Os_SCB.sysRunningTaskID].taskRunPrio = prio;

        Os_ReadyQueueInsert(Os_SCB.sysRunningTaskID, OS_LEVEL_INTERNAL_RESOURCE, prio);

        if (prio > Os_SCB.sysHighPrio)
        {
            Os_SCB.sysHighPrio   = prio;
            Os_SCB.sysHighTaskID = Os_SCB.sysRunningTaskID;
        }

        Os_OccupyInterRes[Os_SCB.sysRunningTaskID] = 1u;
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
void Os_ReleaseInternalResource(Os_TaskType osTaskId)
{
    if ((Os_InterResCeiling[osTaskId] != 0u) && (Os_OccupyInterRes[osTaskId] == 1u))
    {
        Os_TCB[osTaskId].taskRunPrio = Os_TaskCfg[osTaskId].osTaskPriority;

        Os_ReadyQueueRemove(OS_LEVEL_INTERNAL_RESOURCE, Os_InterResCeiling[osTaskId]);

        Os_OccupyInterRes[osTaskId] = 0u;
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* CFG_INTERNAL_RESOURCE_MAX>0 */
#endif /* CFG_RESOURCE_MAX > 0U */

/*=======[E N D   O F   F I L E]==============================================*/
