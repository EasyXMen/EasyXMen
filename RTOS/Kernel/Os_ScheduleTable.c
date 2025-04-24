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
 */
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : Os_ScheduleTable.c                                          **
**                                                                            **
**  Created on  :                                                             **
**  Author      :  i-soft-os                                                  **
**  Vendor      :                                                             **
**  DESCRIPTION :  AutoSar SC2 Schedule Table Management                      **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/

#include "Os_Internal.h"

#if (CFG_SCHEDTBL_MAX > 0U)
/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   D A T A]==========================================*/
/* PRQA S 0791++ */ /* MISRA Rule 5.4 */ /*OS_SCHEDULETABLE_SEGMENTNAME_SIMILAR_004*/
#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
static P2CONST(Os_SchedTblCfgType, AUTOMATIC, OS_VAR) Os_SchedTblCfg;
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SCHEDULETABLE_COMPILER_001 */
static P2VAR(Os_STCBType, AUTOMATIC, OS_VAR) Os_STCB;
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
/* PRQA S 0791-- */ /* MISRA Rule 5.4 */
/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static FUNC(void, OS_CODE) Os_InitSchedTblCB(Os_ScheduleTableType StId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static FUNC(void, OS_CODE) Os_InsertSTNode(Os_ScheduleTableType InsertStId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static FUNC(Os_TickType, OS_CODE) Os_StGetEpDelay(
    P2CONST(Os_SchedTblCfgType, AUTOMATIC, OS_VAR) pStCfgRef,
    CONSTP2CONST(Os_STCBType, AUTOMATIC, OS_VAR) pStNode);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static FUNC(void, OS_CODE) Os_StInnerStart(
    Os_TickType osStartAbsTick,
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SCHEDULETABLE_COMPILER_002 */
    CONSTP2VAR(Os_STCBType, AUTOMATIC, OS_VAR) pStCB,
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    P2CONST(Os_SchedTblCfgType, AUTOMATIC, OS_VAR) pStCfgRef);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static FUNC(void, OS_CODE) Os_InitStAutoStart(
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SCHEDULETABLE_COMPILER_002 */
    CONSTP2VAR(Os_STCBType, AUTOMATIC, OS_VAR) pStCB,
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    P2CONST(Os_SchedTblCfgType, AUTOMATIC, OS_VAR) pStCfgRef);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static FUNC(boolean, OS_CODE)
    Os_InitStAutoStartChk(P2CONST(Os_SchedTblAutostart, AUTOMATIC, OS_VAR) osSchedTblAutostartRef);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
/********************************************************************/
/* Begin: INNER API.                                                */
/********************************************************************/

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
FUNC(void, OS_CODE) Os_InitScheduleTable(void)
{
    VAR(Os_ScheduleTableType, OS_VAR) i;
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SCHEDULETABLE_COMPILER_001 */
    P2VAR(Os_STCBType, AUTOMATIC, OS_VAR) pStCB;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    P2CONST(Os_SchedTblCfgType, AUTOMATIC, OS_VAR) pStCfgRef;
    VAR(uint16, OS_VAR) vCoreId = Os_SCB.sysCore;
    VAR(Os_ScheduleTableType, OS_VAR) Os_CfgSchedTblMax = Os_CfgSchedTblMax_Inf[vCoreId];

    Os_SchedTblCfg = Os_SchedTblCfg_Inf[vCoreId];
    Os_STCB = Os_STCB_Inf[vCoreId];

#if (CFG_SCHEDTBL_MAX > 0)
    for (i = 0U; i < Os_CfgSchedTblMax; i++)
    {
        pStCB = &Os_STCB[i];
        pStCfgRef = &Os_SchedTblCfg[i];

        Os_InitSchedTblCB(i);

        /* Auto start. */
        if (TRUE == Os_InitStAutoStartChk(pStCfgRef->osSchedTblAutostartRef))
        {
            Os_InitStAutoStart(pStCB, pStCfgRef);
        }
    }
#endif /* CFG_SCHEDTBL_MAX > 0 */

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Os_InitStAutoStart.>
 * Service ID           <none>
 * Sync/Async           <none>
 * Reentrancy           <none>
 * param-eventId[in]    <pStCB, pStCfgRef>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_InitScheduleTable>
 * REQ ID               <None>
 */
/********************************************************************/
static FUNC(void, OS_CODE) Os_InitStAutoStart(
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SCHEDULETABLE_COMPILER_002 */
    CONSTP2VAR(Os_STCBType, AUTOMATIC, OS_VAR) pStCB,
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    P2CONST(Os_SchedTblCfgType, AUTOMATIC, OS_VAR) pStCfgRef)
{
    VAR(Os_TickType, OS_VAR) osStartAbsTick;
    VAR(Os_SchedTblAutostartType, OS_VAR) osType;
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SCHEDULETABLE_COMPILER_001 */
    P2VAR(Os_CCBType const, AUTOMATIC, OS_VAR) pCcb;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

    osType = pStCfgRef->osSchedTblAutostartRef->osSchedTblAutostartType;
    pCcb = &Os_CCB[pStCfgRef->osSchedTblCounterRef];

    switch (osType)
    {
    case ST_START_ABSOLUTE:
        Os_StInnerStart(pStCfgRef->osSchedTblAutostartRef->osSchedTblAbsValue, pStCB, pStCfgRef);

        /* Insert ScheduleTable to list. */
        Os_InsertSTNode(pStCB->stId);
        break;

    case ST_START_RELATIVE:
        osStartAbsTick = Os_CalcAbsTicks(
            pCcb->counterCurVal,
            pStCfgRef->osSchedTblAutostartRef->osSchedTblRelOffset,
            pStCfgRef->osSchedTblCounterRef);

        Os_StInnerStart(osStartAbsTick, pStCB, pStCfgRef);

        /* Insert ScheduleTable to list. */
        Os_InsertSTNode(pStCB->stId);
        break;

#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
    case ST_START_SYNCHRON:
        pStCB->stState = SCHEDULETABLE_WAITING;
        break;
#endif

    /*add comments to pass QAC.*/
    default:
        /* Nothing to do. */
        break;
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Check whether this st can auto start.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <None>
 * param-eventId[in]    <osSchedTblAutostartRef>
 * Param-Name[out]      <boolean>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_InitScheduleTable>
 * REQ ID               <None>
 */
/********************************************************************/
static FUNC(boolean, OS_CODE)
    Os_InitStAutoStartChk(P2CONST(Os_SchedTblAutostart, AUTOMATIC, OS_VAR) osSchedTblAutostartRef)
{
    VAR(boolean, OS_VAR) bAutoStart = FALSE;

    if (NULL_PTR != osSchedTblAutostartRef)
    {
        /* Check whether appmode during StartOs in appmode bitmap. */
        if (0U != (osSchedTblAutostartRef->osSchedTblAppModeBitmap & Os_SCB.sysActiveAppMode))
        {
            bAutoStart = TRUE;
        }
    }

    return bAutoStart;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Init ScheduleTable.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <StId>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_InitScheduleTable and so on>
 * REQ ID               <None>
 */
/********************************************************************/
static FUNC(void, OS_CODE) Os_InitSchedTblCB(Os_ScheduleTableType StId)
{
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SCHEDULETABLE_COMPILER_001 */
    P2VAR(Os_STCBType, AUTOMATIC, OS_VAR) pStCB;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    P2CONST(Os_SchedTblCfgType, AUTOMATIC, OS_VAR) pStCfgRef;

    pStCB = &Os_STCB[StId];
    pStCfgRef = &Os_SchedTblCfg[StId];

    /* Init STCB */
    pStCB->stId = StId;
    pStCB->stIsAdjust = FALSE;
    pStCB->stIsAdjAdd = FALSE;
    pStCB->stIsStarted = FALSE;
    pStCB->stIsStopAdjust = FALSE;
    pStCB->stNextEP = pStCfgRef->osSchedTblEP;
    pStCB->stDiff = 0U;
    pStCB->stDev = 0U;
    pStCB->stStartAbsTick = 0U;
    pStCB->stNextEpAbsTick = 0U;
    pStCB->stState = SCHEDULETABLE_STOPPED;
    pStCB->stEpId = 0U;
    pStCB->NextStID = OS_SCHEDTABLE_INVALID;
    pStCB->PrevStID = OS_SCHEDTABLE_INVALID;
    pStCB->stNextNode = NULL_PTR;
    pStCB->stPreNode = NULL_PTR;

    pStCB->stFinalDelay =
        pStCfgRef->osSchedTblDuration - pStCfgRef->osSchedTblEP[pStCfgRef->osSchedTblEPsize - 1u].osSchedTblEPOffset;

    pStCB->stDelay = Os_StGetEpDelay(pStCfgRef, pStCB);

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Insert scheduleTable node to list(Array).>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <ScheduleTableID>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_InitStAutoStart and so on>
 * REQ ID               <None>
 */
/********************************************************************/
static FUNC(void, OS_CODE) Os_InsertSTNode(Os_ScheduleTableType InsertStId)
{
    VAR(Os_TickType, OS_VAR) InsertStEpRelTick;
    VAR(Os_CounterType, OS_VAR) stCounterId;
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SCHEDULETABLE_COMPILER_001 */
    P2VAR(Os_STCBType, AUTOMATIC, OS_VAR) pInsertStCB;
    P2VAR(Os_STCBType, AUTOMATIC, OS_VAR) pCurStCB;
    P2VAR(Os_STCBType, AUTOMATIC, OS_VAR) pPreStCB;
    P2VAR(Os_CCBType, AUTOMATIC, OS_VAR) pCcb;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

    pInsertStCB = &Os_STCB[InsertStId];
    stCounterId = Os_SchedTblCfg[InsertStId].osSchedTblCounterRef;
    pCcb = &Os_CCB[stCounterId];

    /* Note: can not repeat insert. */
    if ((NULL_PTR != pInsertStCB->stNextNode) || (NULL_PTR != pInsertStCB->stPreNode))
    {
        /*nothing to do*/
    }
    /* Flexible design: if list is empty, insert node directly. */
    else if (NULL_PTR == pCcb->counterStListHead)
    {
        pInsertStCB->stNextNode = NULL_PTR;
        pCcb->counterStListHead = pInsertStCB;
    }
    else
    {
        /* If list is not empty, insert node by EP rel tick, increase order.
         * Modified, use relative ticks between current tick and EP*/
        InsertStEpRelTick = Os_GetDistance(pCcb->counterLastVal, pInsertStCB->stNextEpAbsTick, stCounterId);

        pCurStCB = pCcb->counterStListHead;
        pPreStCB = NULL_PTR;

        /* Find insert position. */
        while (NULL_PTR != pCurStCB)
        {
            /* Insert node by increase order. */
            if (InsertStEpRelTick >= Os_GetDistance(pCcb->counterLastVal, pCurStCB->stNextEpAbsTick, stCounterId))
            {
                pPreStCB = pCurStCB;
                pCurStCB = pCurStCB->stNextNode;
            }
            else
            {
                break;
            }
        }

        /* Insert to head position. */
        if (NULL_PTR == pPreStCB)
        {
            /* Update head node. */
            pCcb->counterStListHead = pInsertStCB;

            pInsertStCB->stNextNode = pCurStCB;
            pInsertStCB->stPreNode = NULL_PTR;
            pCurStCB->stPreNode = pInsertStCB;
        }
        else
        {
            /* Insert to other position. */
            pInsertStCB->stNextNode = pPreStCB->stNextNode;
            pInsertStCB->stPreNode = pPreStCB;

            pPreStCB->stNextNode = pInsertStCB;

            /* Process pCurStCB. */
            if (NULL_PTR != pCurStCB)
            {
                pCurStCB->stPreNode = pInsertStCB;
            }
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Insert scheduleTable node to list(Array).>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <ScheduleTableID>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_WorkStRepeatProc and so on>
 * REQ ID               <None>
 */
/********************************************************************/
static FUNC(void, OS_CODE) Os_DelStNode(ScheduleTableType DelStId)
{
    VAR(Os_CounterType, OS_VAR) stCounterId;
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SCHEDULETABLE_COMPILER_001 */
    P2VAR(Os_STCBType, AUTOMATIC, OS_VAR) pDelStCB;
    P2VAR(Os_CCBType, AUTOMATIC, OS_VAR) pCcb;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

    stCounterId = Os_SchedTblCfg[DelStId].osSchedTblCounterRef;
    pCcb = &Os_CCB[stCounterId];

    /* If list is null, return. */
    if (NULL_PTR == pCcb->counterStListHead)
    {
        /*nothing to do*/
    }
    else
    {
        pDelStCB = &(Os_STCB[DelStId]);

        /* Flexible design: DelStId is head node. */
        if (NULL_PTR == pDelStCB->stPreNode)
        {
            pCcb->counterStListHead = pDelStCB->stNextNode;

            if (NULL_PTR != pDelStCB->stNextNode)
            {
                pDelStCB->stNextNode->stPreNode = NULL_PTR;
                pDelStCB->stNextNode = NULL_PTR;
            }
        }
        else
        {
            /* Normal delete. */
            pDelStCB->stPreNode->stNextNode = pDelStCB->stNextNode;

            if (NULL_PTR != pDelStCB->stNextNode) /* Not tail node. */
            {
                pDelStCB->stNextNode->stPreNode = pDelStCB->stPreNode;
            }

            pDelStCB->stPreNode = NULL_PTR;
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Insert scheduleTable node to list(Array)>
 * Service ID           <none>
 * Sync/Async           <none>
 * Reentrancy           <none>
 * param-eventId[in]    <pStCfgRef,pStNode>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_InitSchedTblCB>
 * REQ ID               <None>
 */
/********************************************************************/
static FUNC(Os_TickType, OS_CODE) Os_StGetEpDelay(
    P2CONST(Os_SchedTblCfgType, AUTOMATIC, OS_VAR) pStCfgRef,
    CONSTP2CONST(Os_STCBType, AUTOMATIC, OS_VAR) pStNode)
{
    VAR(Os_TickType, OS_VAR) osDelay;

    /* Last Ep. Figure 7.1 */
    if (pStNode->stEpId >= (pStCfgRef->osSchedTblEPsize - 1u))
    {
        osDelay = pStNode->stFinalDelay + pStCfgRef->osSchedTblEP[0].osSchedTblEPOffset;
    }
    else
    {
        osDelay = pStCfgRef->osSchedTblEP[pStNode->stEpId + 1u].osSchedTblEPOffset
                  - pStCfgRef->osSchedTblEP[pStNode->stEpId].osSchedTblEPOffset;
    }

    return osDelay;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Save start para of schedule table.>
 * Service ID           <none>
 * Sync/Async           <none>
 * Reentrancy           <none>
 * param-eventId[in]    <osStartAbsTick, osCounterMaxVal, pStCB, pStCfgRef>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_InitStAutoStart and so on>
 * REQ ID               <None>
 */
/********************************************************************/
static FUNC(void, OS_CODE) Os_StInnerStart(
    Os_TickType osStartAbsTick,
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SCHEDULETABLE_COMPILER_002 */
    CONSTP2VAR(Os_STCBType, AUTOMATIC, OS_VAR) pStCB,
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    P2CONST(Os_SchedTblCfgType, AUTOMATIC, OS_VAR) pStCfgRef)
{
    VAR(Os_CounterType, OS_VAR) stCounterId;

    stCounterId = pStCfgRef->osSchedTblCounterRef;
    pStCB->stStartAbsTick = osStartAbsTick;
    pStCB->stNextEpAbsTick =
        Os_CalcAbsTicks(pStCB->stStartAbsTick, pStCfgRef->osSchedTblEP[0].osSchedTblEPOffset, stCounterId);

    pStCB->stEpId = 0u;
    pStCB->stIsStarted = TRUE;

    /* Set state to running. */
    pStCB->stState = SCHEDULETABLE_RUNNING;

/* If SyncStrategy is IMPLICIT, state should be RUNNING_AND_SYNCHRONOUS. */
#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
    if (ST_SYNC_IMPLICIT == pStCfgRef->osSchedTblSync.osSchedTblSyncStrategy)
    {
        pStCB->stState = SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS;
    }
#endif

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Activate tasks at expiry points>
 * Service ID           <none>
 * Sync/Async           <none>
 * Reentrancy           <none>
 * param-eventId[in]    <pTaskList, osTaskCnt>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_WorkStEpProc>
 * REQ ID               <None>
 */
/********************************************************************/
static FUNC(void, OS_CODE)
    Os_StEpActiveTask(P2CONST(Os_EPActivateTaskAction, AUTOMATIC, OS_VAR) pTaskList, uint16 osTaskCnt)
{
    VAR(uint16, OS_VAR) i;
    VAR(Os_TaskType, OS_VAR) TaskID;

    for (i = 0u; i < osTaskCnt; i++)
    {
        TaskID = pTaskList[i].osSchedTblActivateTaskRef;

#if (OS_AUTOSAR_CORES > 1)
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
        VAR(Os_CoreIdType, OS_VAR) coreId = Os_GetObjCoreId(TaskID);
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
        if (coreId != Os_SCB.sysCore)
        {
            RpcInputType rpcData = {
                .sync = RPC_SYNC,
                .remoteCoreId = coreId,
                .serviceId = OSServiceId_ActivateTask,
                .srvPara0 = (uint32)TaskID,
                .srvPara1 = (uint32)NULL_PARA,
                .srvPara2 = (uint32)NULL_PARA,
            };
            (void)Os_RpcCallService(&rpcData);
        }
        else
#endif /* OS_AUTOSAR_CORES > 1 */
        {
            (void)Os_ActivateTask(TaskID);
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (CFG_EXTENDED_TASK_MAX > 0)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Set Event at expiry points>
 * Service ID           <none>
 * Sync/Async           <none>
 * Reentrancy           <none>
 * param-eventId[in]    <pEventList, osEventCnt>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_WorkStEpProc>
 * REQ ID               <None>
 */
/********************************************************************/
static FUNC(void, OS_CODE)
    Os_StEpSetEvent(P2CONST(Os_EPSetEventAction, AUTOMATIC, OS_VAR) pEventList, uint16 osEventCnt)
{
    VAR(uint16, OS_VAR) i;
    VAR(Os_TaskType, OS_VAR) TaskID;
    VAR(Os_EventMaskType, OS_VAR) Mask;

    for (i = 0u; i < osEventCnt; i++)
    {
        TaskID = pEventList[i].osSchedTblSetEventTaskRef;
        Mask = pEventList[i].osSchedTblSetEventRef;

#if (OS_AUTOSAR_CORES > 1)
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
        VAR(Os_CoreIdType, OS_VAR) coreId = Os_GetObjCoreId(TaskID);
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
        if (coreId != Os_SCB.sysCore)
        {
            RpcInputType rpcData = {
                .sync = RPC_SYNC,
                .remoteCoreId = coreId,
                .serviceId = OSServiceId_SetEvent,
                .srvPara0 = (uint32)TaskID,
                .srvPara1 = (uint32)Mask,
                .srvPara2 = (uint32)NULL_PARA,
            };
            (void)Os_RpcCallService(&rpcData);
        }
        else
#endif /* OS_AUTOSAR_CORES > 1 */
        {
            (void)Os_SetEvent(TaskID, Mask);
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* CFG_EXTENDED_TASK_MAX > 0 */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Procedures for handling at expiry points>
 * Service ID           <none>
 * Sync/Async           <none>
 * Reentrancy           <none>
 * param-eventId[in]    <pStNode, pStCfgRef>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_WorkStRunningState>
 * REQ ID               <None>
 */
/********************************************************************/
static FUNC(boolean, OS_CODE) Os_WorkStEpProc(
    CONSTP2CONST(Os_STCBType, AUTOMATIC, OS_VAR) pStNode,
    P2CONST(Os_SchedTblCfgType, AUTOMATIC, OS_VAR) pStCfgRef)
{
    VAR(boolean, OS_VAR) LastEP = FALSE;

    /* Active task. */
    Os_StEpActiveTask(pStNode->stNextEP->osActivateTaskList, pStNode->stNextEP->osActivateTaskListSize);

/* Set event. */
#if (CFG_EXTENDED_TASK_MAX > 0)
    Os_StEpSetEvent(pStNode->stNextEP->osSetEventList, pStNode->stNextEP->osSetEventListSize);
#endif

    /* Last EP ? */
    if (pStNode->stEpId >= (pStCfgRef->osSchedTblEPsize - 1u))
    {
        LastEP = TRUE;
    }

    return LastEP;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <A schedule table may repeat after the final expiry point is processed.
 *                       So the API can work>
 * Service ID           <none>
 * Sync/Async           <none>
 * Reentrancy           <none>
 * param-eventId[in]    <counterCurval, pStNode, pStCfgRef, pstCounterCfg>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_WorkStRunningState>
 * REQ ID               <None>
 */
/********************************************************************/
static FUNC(void, OS_CODE) Os_WorkStRepeatProc(
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SCHEDULETABLE_COMPILER_002 */
    CONSTP2VAR(Os_STCBType, AUTOMATIC, OS_VAR) pStNode,
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    P2CONST(Os_SchedTblCfgType, AUTOMATIC, OS_VAR) pStCfgRef)
{
    pStNode->stNextEP = pStCfgRef->osSchedTblEP;
    pStNode->stEpId = 0u;

/* Update start abs tick. */
#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
    if (ST_SYNC_IMPLICIT == pStCfgRef->osSchedTblSync.osSchedTblSyncStrategy)
    {
        pStNode->stStartAbsTick = 0u;
        pStNode->stNextEpAbsTick = pStCfgRef->osSchedTblEP[0].osSchedTblEPOffset;
    }
    else
#endif /* OS_SC2 == CFG_SC || OS_SC4 == CFG_SC */
    {
        pStNode->stNextEpAbsTick =
            Os_CalcAbsTicks(pStNode->stNextEpAbsTick, pStNode->stDelay, pStCfgRef->osSchedTblCounterRef);

        pStNode->stDelay = Os_StGetEpDelay(pStCfgRef, pStNode);
    }

    /* Re-sort by abs tick of next EP. */
    Os_DelStNode(pStNode->stId);
    Os_InsertSTNode(pStNode->stId);

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Procedures for handling the next expiry point>
 * Service ID           <none>
 * Sync/Async           <none>
 * Reentrancy           <none>
 * param-eventId[in]    <pStNode, pStCfgRef>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_WorkStRunningState>
 * REQ ID               <None>
 */
/********************************************************************/
static FUNC(void, OS_CODE) Os_WorkStToNextEp(
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SCHEDULETABLE_COMPILER_002 */
    CONSTP2VAR(Os_STCBType, AUTOMATIC, OS_VAR) pStNode,
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    P2CONST(Os_SchedTblCfgType, AUTOMATIC, OS_VAR) pStCfgRef)
{
    /* Next EP. */
    pStNode->stEpId = pStNode->stEpId + 1u;
    pStNode->stNextEP = &pStCfgRef->osSchedTblEP[pStNode->stEpId];

    pStNode->stNextEpAbsTick =
        Os_CalcAbsTicks(pStNode->stNextEpAbsTick, pStNode->stDelay, pStCfgRef->osSchedTblCounterRef);

    /* Update delay. */
    pStNode->stDelay = Os_StGetEpDelay(pStCfgRef, pStNode);

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/* Sync adjust. */
#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <The API can use When expiry point need synchronization>
 * Service ID           <none>
 * Sync/Async           <none>
 * Reentrancy           <none>
 * param-eventId[in]    <pStCfgRef, pstCounterCfg>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <pStCB>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_WorkStRunningState>
 * REQ ID               <None>
 */
/********************************************************************/
static FUNC(void, OS_CODE) Os_StWorkEpSyncAdjust(
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SCHEDULETABLE_COMPILER_002 */
    CONSTP2VAR(Os_STCBType, AUTOMATIC, OS_VAR) pStCB,
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    P2CONST(Os_SchedTblCfgType, AUTOMATIC, OS_VAR) pStCfgRef,
    P2CONST(Os_CounterCfgType, AUTOMATIC, OS_VAR) pstCounterCfg)
{
    VAR(Os_TickType, OS_VAR) osPreEpOffset;

    /* Offset of pre EP. */
    osPreEpOffset = ((0u == pStCB->stEpId) ? (0u) : (pStCfgRef->osSchedTblEP[pStCB->stEpId - 1u].osSchedTblEPOffset));

    /* Delay + Deviatifon. */
    if (TRUE == pStCB->stIsAdjAdd)
    {
        /* OS437 */
        if (((pStCB->stNextEP->osSchedTblEPOffset) + (pStCB->stNextEP->osSchedTblMaxAdvance))
            > (pStCfgRef->osSchedTblDuration))
        {
            /*nothing to do*/
        }
        else
        {
            /* Deviatifon. */
            pStCB->stDev =
                (((pStCB->stDiff - pStCB->stAdjDevTotal) > pStCB->stNextEP->osSchedTblMaxAdvance)
                     ? (pStCB->stNextEP->osSchedTblMaxAdvance)
                     : (pStCB->stDiff - pStCB->stAdjDevTotal));

            /* pStCB->stAdjDevTotal <= pStCB->stDiff. */
            if ((pStCB->stDiff - pStCB->stAdjDevTotal) <= pStCfgRef->osSchedTblSync.osSchedTblExplicitPrecision)
            {
                /* Sync success. */
                pStCB->stIsAdjust = FALSE;
                pStCB->stAdjDevTotal = 0u;
                pStCB->stDiff = 0u;
                pStCB->stDev = 0u;
            }
            else
            {
                /* stNextEP need adjust. */
                pStCB->stIsAdjust = TRUE;
                pStCB->stAdjDevTotal += pStCB->stDev;

                /* Update delay of stNextEP. */
                pStCB->stDelay += pStCB->stDev;
            }
        }
    }
    else /* Delay - Deviatifon. */
    {
        /* OS436 */
        if (((pStCB->stNextEP->osSchedTblEPOffset) - (pStCB->stNextEP->osSchedTblMaxRetard))
            < ((osPreEpOffset) + (pstCounterCfg->osCounterMinCycle)))
        {
            /*nothing to do*/
        }
        else
        {
            /* Deviatifon. */
            pStCB->stDev =
                (((pStCB->stDiff - pStCB->stAdjDevTotal) > pStCB->stNextEP->osSchedTblMaxRetard)
                     ? (pStCB->stNextEP->osSchedTblMaxRetard)
                     : (pStCB->stDiff - pStCB->stAdjDevTotal));

            /* pStCB->stAdjDevTotal <= pStCB->stDiff. */
            if ((pStCB->stDiff - pStCB->stAdjDevTotal) <= pStCfgRef->osSchedTblSync.osSchedTblExplicitPrecision)
            {
                /* Sync success. */
                pStCB->stIsAdjust = FALSE;
                pStCB->stAdjDevTotal = 0u;
                pStCB->stDiff = 0u;
                pStCB->stDev = 0u;
            }
            else
            {
                /* stNextEP need adjust. */
                pStCB->stIsAdjust = TRUE;
                pStCB->stAdjDevTotal += pStCB->stDev;

                /* Update delay of stNextEP. */
                pStCB->stDelay -= pStCB->stDev;
            }
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* OS_SC2 == CFG_SC || OS_SC4 == CFG_SC */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Os_WorkStRunningState.>
 * Service ID           <none>
 * Sync/Async           <none>
 * Reentrancy           <none>
 * param-eventId[in]    <counterCurval, pStNode, pStCfgRef, pstCounterCfg>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_WorkSchedTbl>
 * REQ ID               <None>
 */
/********************************************************************/
static FUNC(void, OS_CODE) Os_WorkStRunningState(
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SCHEDULETABLE_COMPILER_002 */
    CONSTP2VAR(Os_STCBType, AUTOMATIC, OS_VAR) pStNode,
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    P2CONST(Os_SchedTblCfgType, AUTOMATIC, OS_VAR) pStCfgRef,
    P2CONST(Os_CounterCfgType, AUTOMATIC, OS_VAR) pstCounterCfg)
{
    VAR(boolean, OS_VAR) osIsLastEp;

    osIsLastEp = Os_WorkStEpProc(pStNode, pStCfgRef);

/* Sync: adjust. */
#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
    if ((FALSE == pStNode->stIsStopAdjust) && (TRUE == pStNode->stIsAdjust))
    {
        Os_StWorkEpSyncAdjust(pStNode, pStCfgRef, pstCounterCfg);

        if (TRUE != pStNode->stIsAdjust)
        {
            pStNode->stState = SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS;
        }
    }
#endif

    /* Last EP. */
    if (TRUE == osIsLastEp)
    {
        /* Repeating. */
        /* Note: in standard, from 7.3.2.4 && Figure 7.8, we conclude that a schedule table which can synchronous
           explicitly might be repeating. But we can not conclude that explicit synchronous schedule table must be
           repeating, because there is a next state in Figure7.5. */
        if (TRUE == pStCfgRef->osSchedTblRepeating)
        {
            Os_WorkStRepeatProc(pStNode, pStCfgRef);
        }
        else
        {
            /* If has next schedule table, set state of schedule table to SCHEDULETABLE_RUNNING. */
            if (OS_SCHEDTABLE_INVALID != pStNode->NextStID)
            {
                /*SWS_Os_00505*/
                if (pStNode->stState == SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS)
                {
                    Os_STCB[pStNode->NextStID].stState = SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS;
                }
                else
                {
                    Os_STCB[pStNode->NextStID].stState = SCHEDULETABLE_RUNNING;
                }
            }

            /* Non-repeating, delete from list. */
            Os_DelStNode(pStNode->stId);
            Os_InitSchedTblCB(pStNode->stId);
        }
    }
    else
    {
        Os_WorkStToNextEp(pStNode, pStCfgRef);

        /* Re-sort by abs tick of next EP. */
        Os_DelStNode(pStNode->stId);
        Os_InsertSTNode(pStNode->stId);
    }

    UNUSED_PARAMETER(pstCounterCfg);
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/********************************************************************/
/*
 * Brief                <The API will call after schedule table synchronization success>
 * Service ID           <none>
 * Sync/Async           <none>
 * Reentrancy           <none>
 * param-eventId[in]    <counterCurval, pStNode, pStCfgRef, pstCounterCfg>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_WorkSchedTbl>
 * REQ ID               <None>
 */
/********************************************************************/
#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static FUNC(void, OS_CODE) Os_WorkStRunningSyncState(
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SCHEDULETABLE_COMPILER_002 */
    CONSTP2VAR(Os_STCBType, AUTOMATIC, OS_VAR) pStNode,
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    P2CONST(Os_SchedTblCfgType, AUTOMATIC, OS_VAR) pStCfgRef)
{
    VAR(boolean, OS_VAR) osIsLastEp;

    osIsLastEp = Os_WorkStEpProc(pStNode, pStCfgRef);
    if (TRUE == osIsLastEp)
    {
        if (TRUE == pStCfgRef->osSchedTblRepeating)
        {
            Os_WorkStRepeatProc(pStNode, pStCfgRef);
        }
        else
        {
            if (ST_SYNC_EXPLICIT == pStCfgRef->osSchedTblSync.osSchedTblSyncStrategy)
            {
                if (OS_SCHEDTABLE_INVALID != pStNode->NextStID)
                {
                    /*SWS_Os_00505*/
                    if (SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS == pStNode->stState)
                    {
                        Os_STCB[pStNode->NextStID].stState = SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS;
                    }
                    else
                    {
                        Os_STCB[pStNode->NextStID].stState = SCHEDULETABLE_RUNNING;
                    }
                }
            }
            else if (ST_SYNC_IMPLICIT == pStCfgRef->osSchedTblSync.osSchedTblSyncStrategy)
            {
                if (OS_SCHEDTABLE_INVALID != pStNode->NextStID)
                {
                    Os_STCB[pStNode->NextStID].stState = SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS;
                }
            }
            else
            {
                /* nothing to do. */
            }
            /* Non-repeating, delete from list. */
            Os_DelStNode(pStNode->stId);
            Os_InitSchedTblCB(pStNode->stId);
        }
    }
    else
    {
        Os_WorkStToNextEp(pStNode, pStCfgRef);

        /* Re-sort by abs tick of next EP. */
        Os_DelStNode(pStNode->stId);
        Os_InsertSTNode(pStNode->stId);
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* OS_SC2 == CFG_SC || OS_SC4 == CFG_SC */

/* ScheduleTable Sync. */
#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Sync ScheduleTable: Proc waitting state.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <SyncVal, oscounterCurVal, pStCB, pStCfgRef, pCounterCfgType>
 * Param-Name[out]      <StatusType>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <SyncScheduleTable>
 * REQ ID               <None>
 */
/********************************************************************/
static FUNC(void, OS_CODE) Os_StWaittingStateProc(
    Os_TickType osValue,
    Os_TickType oscounterCurVal,
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SCHEDULETABLE_COMPILER_002 */
    CONSTP2VAR(Os_STCBType, AUTOMATIC, OS_VAR) pStCB,
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    P2CONST(Os_SchedTblCfgType, AUTOMATIC, OS_VAR) pStCfgRef)
{
    /* Save start Abs tick: Now + (Duration - value). */
    pStCB->stStartAbsTick =
        Os_CalcAbsTicks(oscounterCurVal, (pStCfgRef->osSchedTblDuration - osValue), pStCfgRef->osSchedTblCounterRef);

    pStCB->stNextEpAbsTick = Os_CalcAbsTicks(
        pStCB->stStartAbsTick,
        pStCfgRef->osSchedTblEP[0].osSchedTblEPOffset,
        pStCfgRef->osSchedTblCounterRef);

    /* Set state to SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS. */
    pStCB->stState = SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS;

    /* Insert to list. */
    Os_InsertSTNode(pStCB->stId);

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The API can get difference value when schedule table need Sync>
 *
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <SyncVal, oscounterCurVal, pStCB, pStCfgRef>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <Os_StRunningStateProc and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(void, OS_CODE) Os_StSyncGetDiffVal(
    Os_TickType SyncVal,
    Os_TickType oscounterCurVal,
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SCHEDULETABLE_COMPILER_002 */
    CONSTP2VAR(Os_STCBType, AUTOMATIC, OS_VAR) pStCB,
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    P2CONST(Os_SchedTblCfgType, AUTOMATIC, OS_VAR) pStCfgRef)
{
    VAR(Os_TickType, OS_VAR) osDiff;
    VAR(Os_TickType, OS_VAR) osPosOnTbl;

    /* Position on Tbl. */
    osPosOnTbl = Os_GetDistance(oscounterCurVal, pStCB->stNextEpAbsTick, pStCfgRef->osSchedTblCounterRef);

    if (pStCB->stNextEP->osSchedTblEPOffset >= osPosOnTbl)
    {
        osPosOnTbl = pStCB->stNextEP->osSchedTblEPOffset - osPosOnTbl;
    }
    else
    {
        osPosOnTbl = (pStCB->stNextEP->osSchedTblEPOffset - osPosOnTbl) + pStCfgRef->osSchedTblDuration - 1u;
    }

    /* Deviation: added. */
    if (osPosOnTbl >= SyncVal)
    {
        osDiff = osPosOnTbl - SyncVal;

        pStCB->stIsAdjAdd = TRUE;
    }
    else /* Deviation: subtracted. */
    {
        osDiff = SyncVal - osPosOnTbl;

        pStCB->stIsAdjAdd = FALSE;
    }

    pStCB->stDiff = osDiff;

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Sync ScheduleTable: Proc running state.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <SyncVal, oscounterCurVal, pStCB, pStCfgRef, pCounterCfgType>
 * Param-Name[out]      <StatusType>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <SyncScheduleTable>
 * REQ ID               <None>
 */
/********************************************************************/
static FUNC(void, OS_CODE) Os_StRunningStateProc(
    Os_TickType SyncVal,
    Os_TickType oscounterCurVal,
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SCHEDULETABLE_COMPILER_002 */
    CONSTP2VAR(Os_STCBType, AUTOMATIC, OS_VAR) pStCB,
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    P2CONST(Os_SchedTblCfgType, AUTOMATIC, OS_VAR) pStCfgRef)
{
    /* Get diff value. */
    Os_StSyncGetDiffVal(SyncVal, oscounterCurVal, pStCB, pStCfgRef);

    /* No need sync. */
    if (pStCB->stDiff <= pStCfgRef->osSchedTblSync.osSchedTblExplicitPrecision)
    {
        pStCB->stState = SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS;
        pStCB->stIsAdjust = FALSE;
    }
    else
    {
        /* Need sync. */
        pStCB->stIsAdjust = TRUE;

        /* OS362 */
        pStCB->stIsStopAdjust = FALSE;

        /* Re-sort. */
        Os_DelStNode(pStCB->stId);
        Os_InsertSTNode(pStCB->stId);
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Sync ScheduleTable: Proc running and sync state.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <SyncVal, oscounterCurVal, pStCB, pStCfgRef, pCounterCfgType>
 * Param-Name[out]      <StatusType>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <SyncScheduleTable>
 * REQ ID               <None>
 */
/********************************************************************/
static FUNC(void, OS_CODE) Os_StRunningSyncStateProc(
    Os_TickType SyncVal,
    Os_TickType oscounterCurVal,
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SCHEDULETABLE_COMPILER_002 */
    CONSTP2VAR(Os_STCBType, AUTOMATIC, OS_VAR) pStCB,
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    P2CONST(Os_SchedTblCfgType, AUTOMATIC, OS_VAR) pStCfgRef)
{
    /* Get diff value. */
    Os_StSyncGetDiffVal(SyncVal, oscounterCurVal, pStCB, pStCfgRef);

    /* No need sync. */
    if (pStCB->stDiff <= pStCfgRef->osSchedTblSync.osSchedTblExplicitPrecision)
    {
        pStCB->stIsAdjust = FALSE;
    }
    else
    {
        /* Need sync. */
        pStCB->stIsAdjust = TRUE;

        /* OS362 */
        pStCB->stIsStopAdjust = FALSE;

        pStCB->stState = SCHEDULETABLE_RUNNING;

        /* Re-sort. */
        Os_DelStNode(pStCB->stId);
        Os_InsertSTNode(pStCB->stId);
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* OS_SC2 == CFG_SC || OS_SC4 == CFG_SC */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
FUNC(void, OS_CODE) Os_WorkSchedTbl(Os_CounterType CounterID)
{
    VAR(Os_TickType, OS_VAR) counterCurval;
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SCHEDULETABLE_COMPILER_001 */
    P2VAR(Os_STCBType, AUTOMATIC, OS_VAR) pstListHead;
    P2VAR(Os_CCBType const, AUTOMATIC, OS_VAR) pCcb;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    P2CONST(Os_SchedTblCfgType, AUTOMATIC, OS_VAR) pStCfgRef;
    P2CONST(Os_CounterCfgType, AUTOMATIC, OS_VAR) pstCounterCfg;
    OS_ARCH_DECLARE_CRITICAL();

#if (CFG_OSAPPLICATION_MAX > 0U)
    VAR(Os_ApplicationType, OS_VAR) bakappID;
#endif

    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
    OS_ARCH_ENTRY_CRITICAL();
    /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
    pCcb = &Os_CCB[CounterID];
    if (NULL_PTR == pCcb->counterStListHead)
    {
        /*nothing to do*/
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
        OS_ARCH_EXIT_CRITICAL();
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
    }
    else
    {
        pstListHead = pCcb->counterStListHead;
        pstCounterCfg = &Os_CounterCfg[CounterID];
        counterCurval = Os_CCB[CounterID].counterCurVal;

/* Process each SchedTbl(EP) which is reach the offset. */
#if (CFG_OSAPPLICATION_MAX > 0U)
        bakappID = Os_SCB.sysRunningAppID;
#endif

        /* More then one EP may have same tick value. */
        while (NULL_PTR != pstListHead)
        {
            if (Os_GetDistance(pstListHead->stNextEpAbsTick, counterCurval, CounterID)
                > pstCounterCfg->osCounterMaxAllowedValue)
            {
                break;
            }
            pStCfgRef = &Os_SchedTblCfg[pstListHead->stId];

#if (CFG_OSAPPLICATION_MAX > 0U)
            Os_SCB.sysRunningAppID = Os_ObjectAppCfg[OBJECT_SCHEDULETABLE][pstListHead->stId].hostApp;
#endif

            /* EP process by different state. */
            switch (pstListHead->stState)
            {
            case SCHEDULETABLE_RUNNING:
                Os_WorkStRunningState(pstListHead, pStCfgRef, pstCounterCfg);
                break;

#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
            case SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS:
                Os_WorkStRunningSyncState(pstListHead, pStCfgRef);
                break;
#endif

            /*add comments to pass QAC.*/
            default:
                /* Nothing to do. */
                break;
            }

            pstListHead = pCcb->counterStListHead;
        }
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
        OS_ARCH_EXIT_CRITICAL();
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */

#if (CFG_OSAPPLICATION_MAX > 0U)
        Os_SCB.sysRunningAppID = bakappID;
#endif
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
FUNC(void, OS_CODE)
Os_GetScheduleTableStatus(ScheduleTableType ScheduleTableID, ScheduleTableStatusRefType ScheduleStatus)
{
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SCHEDULETABLE_COMPILER_001 */
    P2VAR(Os_STCBType const, AUTOMATIC, OS_VAR) pStCB;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

#if (OS_AUTOSAR_CORES > 1)
    /* PRQA S 3469, 1338 ++ */ /* MISRA Dir 4.9, 17.8  */ /* OS_SCHEDULETABLE_PARAM_MACRO_006 */
    ScheduleTableID = Os_GetObjLocalId(ScheduleTableID);
/* PRQA S 3469, 1338 -- */ /* MISRA Dir 4.9, 17.8  */
#endif

    pStCB = &Os_STCB[ScheduleTableID];
    /* Return state of ScheduleTableID. */
    *ScheduleStatus = pStCB->stState;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
FUNC(StatusType, OS_CODE) Os_StartScheduleTableRel(ScheduleTableType ScheduleTableID, TickType Offset)
{
    VAR(Os_TickType, OS_VAR) osCounterCurVal;
    VAR(Os_TickType, OS_VAR) osStartAbsTick;
    P2CONST(Os_SchedTblCfgType, AUTOMATIC, OS_VAR) pStCfgRef;
    P2CONST(Os_CounterCfgType, AUTOMATIC, OS_VAR) pCounterCfgRef;
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SCHEDULETABLE_COMPILER_001 */
    P2VAR(Os_STCBType, AUTOMATIC, OS_VAR) pStCB = NULL_PTR;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    VAR(StatusType, OS_VAR) err = E_OK;

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    VAR(Os_TickType, OS_VAR) stInitOffset;
#endif

    OS_ARCH_DECLARE_CRITICAL();

#if (OS_AUTOSAR_CORES > 1)
    /* PRQA S 3469, 1338 ++ */ /* MISRA Dir 4.9, 17.8  */ /* OS_SCHEDULETABLE_PARAM_MACRO_006 */
    ScheduleTableID = Os_GetObjLocalId(ScheduleTableID);
/* PRQA S 3469, 1338 -- */ /* MISRA Dir 4.9, 17.8  */
#endif

    pStCfgRef = &Os_SchedTblCfg[ScheduleTableID];
    pCounterCfgRef = &Os_CounterCfg[pStCfgRef->osSchedTblCounterRef];

/* OS452 */
#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
    if (ST_SYNC_IMPLICIT == pStCfgRef->osSchedTblSync.osSchedTblSyncStrategy)
    {
        err = E_OS_ID;
    }
    else
#endif /* OS_SC2 == CFG_SC || OS_SC4 == CFG_SC */
    {
/* OS332 + OS276 */
#if (OS_STATUS_EXTENDED == CFG_STATUS)
        stInitOffset = pStCfgRef->osSchedTblEP[0].osSchedTblEPOffset;

        if ((Offset > (pCounterCfgRef->osCounterMaxAllowedValue - stInitOffset)) || (0u == Offset))
        {
            err = E_OS_VALUE;
        }
        else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
        {
            pStCB = &Os_STCB[ScheduleTableID];

            /* OS277 */
            if (SCHEDULETABLE_STOPPED != pStCB->stState)
            {
                err = E_OS_STATE;
            }
        }
    }

    if ((StatusType)E_OK == err)
    {
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
        OS_ARCH_ENTRY_CRITICAL();
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */

        /* Timer type. */
        osCounterCurVal = Os_CCB[pStCfgRef->osSchedTblCounterRef].counterCurVal;

        /* Save abs start tick. */
        osStartAbsTick = Os_CalcAbsTicks(osCounterCurVal, Offset, pStCfgRef->osSchedTblCounterRef);

        Os_StInnerStart(osStartAbsTick, pStCB, pStCfgRef);

        /* Insert ScheduleTable to list. */
        Os_InsertSTNode(ScheduleTableID);

        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
        OS_ARCH_EXIT_CRITICAL();
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
    }

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
FUNC(StatusType, OS_CODE) Os_StartScheduleTableAbs(ScheduleTableType ScheduleTableID, TickType Start)
{
    P2CONST(Os_SchedTblCfgType, AUTOMATIC, OS_VAR) pStCfgRef;
    P2CONST(Os_CounterCfgType, AUTOMATIC, OS_VAR) pCounterCfgRef;
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SCHEDULETABLE_COMPILER_001 */
    P2VAR(Os_STCBType, AUTOMATIC, OS_VAR) pStCB;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    VAR(StatusType, OS_VAR) err = E_OK;

    OS_ARCH_DECLARE_CRITICAL();

#if (OS_AUTOSAR_CORES > 1)
    /* PRQA S 3469, 1338 ++ */ /* MISRA Dir 4.9, 17.8  */ /* OS_SCHEDULETABLE_PARAM_MACRO_006 */
    ScheduleTableID = Os_GetObjLocalId(ScheduleTableID);
/* PRQA S 3469, 1338 -- */ /* MISRA Dir 4.9, 17.8  */
#endif

    /* OS348 */
    pStCfgRef = &Os_SchedTblCfg[ScheduleTableID];
    pCounterCfgRef = &Os_CounterCfg[pStCfgRef->osSchedTblCounterRef];

/* OS349 */
#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (Start > pCounterCfgRef->osCounterMaxAllowedValue)
    {
        err = E_OS_VALUE;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
    {
        pStCB = &Os_STCB[ScheduleTableID];

        /* OS350 */
        if (SCHEDULETABLE_STOPPED != pStCB->stState)
        {
            err = E_OS_STATE;
        }
    }
    if ((StatusType)E_OK == err)
    {
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
        OS_ARCH_ENTRY_CRITICAL();
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */

        /* Save abs start tick. */
        Os_StInnerStart(Start, pStCB, pStCfgRef);

        /* Insert ScheduleTable to list. */
        Os_InsertSTNode(ScheduleTableID);

        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
        OS_ARCH_EXIT_CRITICAL();
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
    }

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
FUNC(StatusType, OS_CODE) Os_StopScheduleTable(ScheduleTableType ScheduleTableID)
{
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SCHEDULETABLE_COMPILER_001 */
    P2VAR(Os_STCBType const, AUTOMATIC, OS_VAR) pStCB;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    VAR(StatusType, OS_VAR) err = E_OK;

    OS_ARCH_DECLARE_CRITICAL();

#if (OS_AUTOSAR_CORES > 1)
    /* PRQA S 3469, 1338 ++ */ /* MISRA Dir 4.9, 17.8  */ /* OS_SCHEDULETABLE_PARAM_MACRO_006 */
    ScheduleTableID = Os_GetObjLocalId(ScheduleTableID);
/* PRQA S 3469, 1338 -- */ /* MISRA Dir 4.9, 17.8  */
#endif

    pStCB = &Os_STCB[ScheduleTableID];

    /* OS280 */
    if (SCHEDULETABLE_STOPPED == pStCB->stState)
    {
        err = E_OS_NOFUNC;
    }

    if ((StatusType)E_OK == err)
    {
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
        OS_ARCH_ENTRY_CRITICAL();
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */

        /* OS453: Delete next-ScheduleTable releted by ScheduleTableID. */
        if (OS_SCHEDTABLE_INVALID != pStCB->NextStID)
        {
            Os_DelStNode(pStCB->NextStID);
            Os_InitSchedTblCB(pStCB->NextStID);
        }

        /* If state is ST_STATE_NEXT, prev-st should be cared. */
        if (OS_SCHEDTABLE_INVALID != pStCB->PrevStID)
        {
            Os_STCB[pStCB->PrevStID].NextStID = OS_SCHEDTABLE_INVALID;
        }

        /* Delete node from list. */
        Os_DelStNode(ScheduleTableID);

        /* Re-init STCB. Note: in function Os_InitSchedTblCB,
            state will be set to ST_STATE_STOPPED. */
        Os_InitSchedTblCB(ScheduleTableID);

        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
        OS_ARCH_EXIT_CRITICAL();
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
    }

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Internal implementation of OS service:NextScheduleTable.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <ScheduleTableID_From, ScheduleTableID_To>
 * Param-Name[out]      <StatusType>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * REQ ID               <None>
 */
/********************************************************************/
static FUNC(void, OS_CODE)
    Os_NextScheduleTable(ScheduleTableType ScheduleTableID_From, ScheduleTableType ScheduleTableID_To)
{
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SCHEDULETABLE_COMPILER_001 */
    P2VAR(Os_STCBType, AUTOMATIC, OS_VAR) pStFromCB;
    P2VAR(Os_STCBType, AUTOMATIC, OS_VAR) pStToCB;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    P2CONST(Os_SchedTblCfgType, AUTOMATIC, OS_VAR) pStFromCfgRef;
    P2CONST(Os_SchedTblCfgType, AUTOMATIC, OS_VAR) pStToCfgRef;

    OS_ARCH_DECLARE_CRITICAL();

    pStFromCfgRef = &Os_SchedTblCfg[ScheduleTableID_From];
    pStToCfgRef = &Os_SchedTblCfg[ScheduleTableID_To];

    pStFromCB = &Os_STCB[ScheduleTableID_From];
    pStToCB = &Os_STCB[ScheduleTableID_To];

    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
    OS_ARCH_ENTRY_CRITICAL();
    /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */

    /* OS284 */
    pStToCB->stStartAbsTick = Os_CalcAbsTicks(
        pStFromCB->stStartAbsTick,
        pStFromCfgRef->osSchedTblDuration,
        pStFromCfgRef->osSchedTblCounterRef);

    pStToCB->stNextEpAbsTick = Os_CalcAbsTicks(
        pStToCB->stStartAbsTick,
        pStToCfgRef->osSchedTblEP[0].osSchedTblEPOffset,
        pStToCfgRef->osSchedTblCounterRef);

    pStToCB->stState = SCHEDULETABLE_NEXT;
    pStToCB->stIsStarted = TRUE;

    /* OS324 */
    if (OS_SCHEDTABLE_INVALID != pStFromCB->NextStID)
    {
        Os_DelStNode(pStFromCB->NextStID);
        Os_InitSchedTblCB(pStFromCB->NextStID);
    }

    pStFromCB->NextStID = ScheduleTableID_To;
    pStToCB->PrevStID = ScheduleTableID_From;

    /* OS284: Insert to list. */
    Os_InsertSTNode(ScheduleTableID_To);

    /* OS363: Do nothing here. */

    /* OS453: in StopScheduleTable. */

    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
    OS_ARCH_EXIT_CRITICAL();
    /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Internal implementation of OS service:StartScheduleTableSynchron.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <ScheduleTableID>
 * Param-Name[out]      <StatusType>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * REQ ID               <None>
 */
/********************************************************************/
static FUNC(void, OS_CODE) Os_StartScheduleTableSynchron(ScheduleTableType ScheduleTableID)
{
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SCHEDULETABLE_COMPILER_001 */
    P2VAR(Os_STCBType, AUTOMATIC, OS_VAR) pStCB;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

    pStCB = &Os_STCB[ScheduleTableID];

    OS_ARCH_DECLARE_CRITICAL();
    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
    OS_ARCH_ENTRY_CRITICAL();
    /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
    /* OS389 */
    pStCB->stState = SCHEDULETABLE_WAITING;
    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
    OS_ARCH_EXIT_CRITICAL();
    /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Internal implementation of OS service:SyncScheduleTable.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Yes>
 * param-eventId[in]    <ScheduleTableID, value>
 * Param-Name[out]      <StatusType>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * REQ ID               <None>
 */
/********************************************************************/
static FUNC(StatusType, OS_CODE) Os_SyncScheduleTable(ScheduleTableType ScheduleTableID, TickType value)
{
    VAR(Os_TickType, OS_VAR) counterCurval;
    P2CONST(Os_SchedTblCfgType, AUTOMATIC, OS_VAR) pStCfgRef;
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SCHEDULETABLE_COMPILER_001 */
    P2VAR(Os_STCBType, AUTOMATIC, OS_VAR) pStCB;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    VAR(StatusType, OS_VAR) err = E_OK;

    OS_ARCH_DECLARE_CRITICAL();

    pStCfgRef = &Os_SchedTblCfg[ScheduleTableID];
    pStCB = &Os_STCB[ScheduleTableID];

    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
    OS_ARCH_ENTRY_CRITICAL();
    /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */

    /* Get counterCB. */
    counterCurval = Os_CCB[pStCfgRef->osSchedTblCounterRef].counterCurVal;

    /* Proc by different state. */
    switch (pStCB->stState)
    {
/* OS456. */
#if (OS_STATUS_EXTENDED == CFG_STATUS)
    case SCHEDULETABLE_STOPPED:
#endif

    case SCHEDULETABLE_NEXT:
        err = E_OS_STATE;
        break;

    case SCHEDULETABLE_WAITING:
        Os_StWaittingStateProc(value, counterCurval, pStCB, pStCfgRef);
        break;

    case SCHEDULETABLE_RUNNING:
        Os_StRunningStateProc(value, counterCurval, pStCB, pStCfgRef);
        break;

    case SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS:
        Os_StRunningSyncStateProc(value, counterCurval, pStCB, pStCfgRef);
        break;

    default:
        /* Nothing to do. */
        break;
    }
    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
    OS_ARCH_EXIT_CRITICAL();
    /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Internal implementation of OS service:SetScheduleTableAsync.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Yes>
 * param-eventId[in]    <ScheduleTableID>
 * Param-Name[out]      <StatusType>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * REQ ID               <None>
 */
/********************************************************************/
static FUNC(StatusType, OS_CODE) Os_SetScheduleTableAsync(ScheduleTableType ScheduleTableID)
{
    P2CONST(Os_SchedTblCfgType, AUTOMATIC, OS_VAR) pStCfgRef;
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SCHEDULETABLE_COMPILER_001 */
    P2VAR(Os_STCBType, AUTOMATIC, OS_VAR) pStCB;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    VAR(StatusType, OS_VAR) err = E_OK;

    OS_ARCH_DECLARE_CRITICAL();
    pStCfgRef = &Os_SchedTblCfg[ScheduleTableID];
    pStCB = &Os_STCB[ScheduleTableID];

    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
    OS_ARCH_ENTRY_CRITICAL();
    /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */

    /*SWS_Os_00300*/
    if (ST_SYNC_EXPLICIT == pStCfgRef->osSchedTblSync.osSchedTblSyncStrategy)
    {
        pStCB->stState = SCHEDULETABLE_RUNNING;
        /* OS362 && OS323 */
        pStCB->stIsStopAdjust = TRUE;

        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
        OS_ARCH_EXIT_CRITICAL();
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
    }
    else /* OS458 */
    {
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
        OS_ARCH_EXIT_CRITICAL();
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
        err = E_OS_ID;
    }

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* OS_SC2 == CFG_SC || OS_SC4 == CFG_SC */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <GetScheduleTableStatus.>
 * Service ID           <0x0e>
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
FUNC(StatusType, OS_CODE)
GetScheduleTableStatus(ScheduleTableType ScheduleTableID, ScheduleTableStatusRefType ScheduleStatus)
{
    OS_ENTER_KERNEL();

    VAR(StatusType, OS_VAR) err = E_OK;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_GetScheduleTableStatus);
#endif /* TRUE == CFG_TRACE_ENABLE */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (NULL_PTR == ScheduleStatus)
    {
        err = E_OS_PARAM_POINTER;
    }
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SCHEDULETABLE_MACRO_007 */
    else if (CHECK_ID_INVALID(ScheduleTableID, Os_CfgSchedTblMax_Inf))
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    {
        err = E_OS_ID;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_GET_SCHEDULE_TABLE_STATUS) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */ /* OS_SCHEDULETABLE_TYPE_CAST_003 */
    else if (Os_AddressWritable((uint32)ScheduleStatus) != TRUE)
    /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */
    {
        err = E_OS_ILLEGAL_ADDRESS;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        err = E_OS_DISABLEDINT;
    }
    else if (Os_CheckObjAcs(OBJECT_SCHEDULETABLE, ScheduleTableID) != TRUE)
    {
        err = E_OS_ACCESS;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
/* SWS_Os_00647:The API service "GetScheduleTableStatus" shall
 * be able to get the status of a schedule table that is part
 * of an OS-Application residing on a different core.  */
#if (OS_AUTOSAR_CORES > 1)
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
        VAR(Os_CoreIdType, OS_VAR) coreId = Os_GetObjCoreId(ScheduleTableID);
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
        if (coreId != Os_SCB.sysCore)
        {
            RpcInputType rpcData = {
                .sync = RPC_SYNC,
                .remoteCoreId = coreId,
                .serviceId = OSServiceId_GetScheduleTableStatus,
                .srvPara0 = (uint32)ScheduleTableID,
                /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */ /* OS_SCHEDULETABLE_TYPE_CAST_003 */
                .srvPara1 = (uint32)ScheduleStatus,
                /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */
                .srvPara2 = (uint32)NULL_PARA,
            };
            err = Os_RpcCallService(&rpcData);
        }
        else
#endif /* OS_AUTOSAR_CORES > 1 */
        {
            Os_GetScheduleTableStatus(ScheduleTableID, ScheduleStatus);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(
            OSError_Save_GetScheduleTableStatus(ScheduleTableID, ScheduleStatus),
            OSServiceId_GetScheduleTableStatus,
            err);
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_GetScheduleTableStatus);
#endif /* TRUE == CFG_TRACE_ENABLE */

    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
    OS_EXIT_KERNEL();
    /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Start schedule table in Rel mode.>
 * Service ID           <0x07>
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
FUNC(StatusType, OS_CODE) StartScheduleTableRel(ScheduleTableType ScheduleTableID, TickType Offset)
{
    OS_ENTER_KERNEL();
    VAR(StatusType, OS_VAR) err = E_OK;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_StartScheduleTableRel);
#endif /* TRUE == CFG_TRACE_ENABLE */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SCHEDULETABLE_MACRO_007 */
    if (CHECK_ID_INVALID(ScheduleTableID, Os_CfgSchedTblMax_Inf))
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    {
        err = E_OS_ID;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_START_ST_REL) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        err = E_OS_DISABLEDINT;
    }
    else if (Os_CheckObjAcs(OBJECT_SCHEDULETABLE, ScheduleTableID) != TRUE)
    {
        err = E_OS_ACCESS;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
/* SWS_Os_00645:The API call "tartScheduleTableRel" shall
 * be able to start schedule tables of OS-Applications
 * residing on other cores. */
#if (OS_AUTOSAR_CORES > 1)
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
        VAR(Os_CoreIdType, OS_VAR) coreId = Os_GetObjCoreId(ScheduleTableID);
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
        if (coreId != Os_SCB.sysCore)
        {
            RpcInputType rpcData = {
                .sync = RPC_SYNC,
                .remoteCoreId = coreId,
                .serviceId = OSServiceId_StartScheduleTableRel,
                .srvPara0 = (uint32)ScheduleTableID,
                .srvPara1 = (uint32)Offset,
                .srvPara2 = (uint32)NULL_PARA,
            };
            err = Os_RpcCallService(&rpcData);
        }
        else
#endif /* OS_AUTOSAR_CORES > 1 */
        {
            err = Os_StartScheduleTableRel(ScheduleTableID, Offset);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(
            OSError_Save_StartScheduleTableRel(ScheduleTableID, Offset),
            OSServiceId_StartScheduleTableRel,
            err);
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_StartScheduleTableRel);
#endif /* TRUE == CFG_TRACE_ENABLE */

    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
    OS_EXIT_KERNEL();
    /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Start schedule table in Abs mode.>
 * Service ID           <0x08>
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
FUNC(StatusType, OS_CODE) StartScheduleTableAbs(ScheduleTableType ScheduleTableID, TickType Start)
{
    OS_ENTER_KERNEL();
    VAR(StatusType, OS_VAR) err = E_OK;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_StartScheduleTableAbs);
#endif /* TRUE == CFG_TRACE_ENABLE */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SCHEDULETABLE_MACRO_007 */
    if (CHECK_ID_INVALID(ScheduleTableID, Os_CfgSchedTblMax_Inf))
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    {
        err = E_OS_ID;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_START_ST_ABS) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        err = E_OS_DISABLEDINT;
    }
    else if (Os_CheckObjAcs(OBJECT_SCHEDULETABLE, ScheduleTableID) != TRUE)
    {
        err = E_OS_ACCESS;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
/* SWS_Os_00644:The API call "tartScheduleTableAbs"
 * shall be able to start schedule tables of
 * OS-Applications residing on other cores. */
#if (OS_AUTOSAR_CORES > 1)
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
        VAR(Os_CoreIdType, OS_VAR) coreId = Os_GetObjCoreId(ScheduleTableID);
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
        if (coreId != Os_SCB.sysCore)
        {
            RpcInputType rpcData = {
                .sync = RPC_SYNC,
                .remoteCoreId = coreId,
                .serviceId = OSServiceId_StartScheduleTableAbs,
                .srvPara0 = (uint32)ScheduleTableID,
                .srvPara1 = (uint32)Start,
                .srvPara2 = (uint32)NULL_PARA,
            };
            err = Os_RpcCallService(&rpcData);
        }
        else
#endif /* OS_AUTOSAR_CORES > 1 */
        {
            err = Os_StartScheduleTableAbs(ScheduleTableID, Start);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(
            OSError_Save_StartScheduleTableAbs(ScheduleTableID, Start),
            OSServiceId_StartScheduleTableAbs,
            err);
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_StartScheduleTableAbs);
#endif /* TRUE == CFG_TRACE_ENABLE */

    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
    OS_EXIT_KERNEL();
    /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Start schedule table in Abs mode.>
 * Service ID           <0x09>
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
FUNC(StatusType, OS_CODE) StopScheduleTable(ScheduleTableType ScheduleTableID)
{
    OS_ENTER_KERNEL();
    VAR(StatusType, OS_VAR) err = E_OK;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_StopScheduleTable);
#endif /* TRUE == CFG_TRACE_ENABLE */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SCHEDULETABLE_MACRO_007 */
    if (CHECK_ID_INVALID(ScheduleTableID, Os_CfgSchedTblMax_Inf))
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    {
        err = E_OS_ID;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_STOP_SCHEDULE_TABLE) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        err = E_OS_DISABLEDINT;
    }
    else if (Os_CheckObjAcs(OBJECT_SCHEDULETABLE, ScheduleTableID) != TRUE)
    {
        err = E_OS_ACCESS;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
/* SWS_Os_00646:The API call StopScheduleTable
 * shall be able to stop schedule tables of
 * OS-Applications residing on other cores.  */
#if (OS_AUTOSAR_CORES > 1)
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
        VAR(Os_CoreIdType, OS_VAR) coreId = Os_GetObjCoreId(ScheduleTableID);
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
        if (coreId != Os_SCB.sysCore)
        {
            RpcInputType rpcData = {
                .sync = RPC_SYNC,
                .remoteCoreId = coreId,
                .serviceId = OSServiceId_StopScheduleTable,
                .srvPara0 = (uint32)ScheduleTableID,
                .srvPara1 = (uint32)NULL_PARA,
                .srvPara2 = (uint32)NULL_PARA,
            };
            err = Os_RpcCallService(&rpcData);
        }
        else
#endif /* OS_AUTOSAR_CORES > 1 */
        {
            err = Os_StopScheduleTable(ScheduleTableID);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_StopScheduleTable(ScheduleTableID), OSServiceId_StopScheduleTable, err);
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_StopScheduleTable);
#endif /* TRUE == CFG_TRACE_ENABLE */

    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
    OS_EXIT_KERNEL();
    /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Next ScheduleTable Process.>
 * Service ID           <0x0a>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <ScheduleTableID_From, ScheduleTableID_To>
 * Param-Name[out]      <StatusType>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * REQ ID               <None>
 */
/********************************************************************/
FUNC(StatusType, OS_CODE)
NextScheduleTable(ScheduleTableType ScheduleTableID_From, ScheduleTableType ScheduleTableID_To)
{
    OS_ENTER_KERNEL();
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SCHEDULETABLE_TYPE_CAST_001 */
    /* PRQA S 3678 ++ */ /* MISRA Rule 8.13 */ /*OS_SCHEDULETABLE_LACK_CONST_005*/
    P2VAR(Os_STCBType, AUTOMATIC, OS_VAR) pStFromCB;
    /* PRQA S 3678 -- */                       /* MISRA Rule 8.13 */
    /* PRQA S 3678 ++ */ /* MISRA Rule 8.13 */ /*OS_SCHEDULETABLE_LACK_CONST_005*/
    P2VAR(Os_STCBType, AUTOMATIC, OS_VAR) pStToCB;
    /* PRQA S 3678 -- */ /* MISRA Rule 8.13 */
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    P2CONST(Os_SchedTblCfgType, AUTOMATIC, OS_VAR) pStFromCfgRef;
    P2CONST(Os_SchedTblCfgType, AUTOMATIC, OS_VAR) pStToCfgRef;
    VAR(StatusType, OS_VAR) err = E_OK;
    VAR(Os_CoreIdType, OS_VAR) coreId_From;
    VAR(Os_CoreIdType, OS_VAR) coreId_To;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_NextScheduleTable);
#endif /* TRUE == CFG_TRACE_ENABLE */

    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
    coreId_From = Os_GetObjCoreId(ScheduleTableID_From);
    /* PRQA S 3469 -- */                     /* MISRA Dir 4.9 */
    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
    coreId_To = Os_GetObjCoreId(ScheduleTableID_To);
    /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */

    /* Cross core use shall be not supported. */
    if ((Os_SCB.sysCore != coreId_From) || (Os_SCB.sysCore != coreId_To))
    {
        err = E_OS_CORE;
    }
#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* OS282:If the input parameter <ScheduleTableID_From> or
     * <ScheduleTableID_To> in a call of NextScheduleTable()
     * is not valid,NextScheduleTable() shall return E_OS_ID.*/
    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
    else if (
        (Os_GetObjLocalId(ScheduleTableID_From) >= Os_CfgSchedTblMax_Inf[coreId_From])
        || (Os_GetObjLocalId(ScheduleTableID_To) >= Os_CfgSchedTblMax_Inf[coreId_To]))
    /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
    {
        err = E_OS_ID;
    }
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    else if (Os_WrongContext(OS_CONTEXT_NEXT_SCHEDULE_TABLE) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        err = E_OS_DISABLEDINT;
    }
    else if (Os_CheckObjAcs(OBJECT_SCHEDULETABLE, ScheduleTableID_From) != TRUE)
    {
        err = E_OS_ACCESS;
    }
    else if (Os_CheckObjAcs(OBJECT_SCHEDULETABLE, ScheduleTableID_To) != TRUE)
    {
        err = E_OS_ACCESS;
    }
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    else
    {
        /* PRQA S 3469, 1338 ++ */ /* MISRA Dir 4.9, 17.8  */ /* OS_SCHEDULETABLE_PARAM_MACRO_006 */
        ScheduleTableID_From = Os_GetObjLocalId(ScheduleTableID_From);
        ScheduleTableID_To = Os_GetObjLocalId(ScheduleTableID_To);
        /* PRQA S 3469, 1338 -- */ /* MISRA Dir 4.9, 17.8  */

        pStFromCfgRef = &Os_SchedTblCfg[ScheduleTableID_From];
        pStToCfgRef = &Os_SchedTblCfg[ScheduleTableID_To];

#if (OS_STATUS_EXTENDED == CFG_STATUS)
        /* OS330:If in a call of NextScheduleTable() schedule table
         * <ScheduleTableID_To> is driven by different counter than
         * schedule table <ScheduleTableID_From> then NextScheduleTable()
         * shall return an error E_OS_ID.*/
        if (pStFromCfgRef->osSchedTblCounterRef != pStToCfgRef->osSchedTblCounterRef)
        {
            err = E_OS_ID;
        }
        else
        {
            /* SWS_Os_00484:If OsScheduleTblSyncStrategy of <ScheduleTableID_To>
             * in a call of NextScheduleTable() is not equal to the
             * OsScheduleTblSyncStrategy of <ScheduleTableID_From> then
             * NextScheduleTable() shall return E_OS_ID.*/
#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
            if (pStFromCfgRef->osSchedTblSync.osSchedTblSyncStrategy
                != pStToCfgRef->osSchedTblSync.osSchedTblSyncStrategy)
            {
                err = E_OS_ID;
            }
#endif
        }
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */

        if ((StatusType)E_OK == err)
        {
            pStFromCB = &Os_STCB[ScheduleTableID_From];
            pStToCB = &Os_STCB[ScheduleTableID_To];

            /* OS283 */
            if ((SCHEDULETABLE_STOPPED == pStFromCB->stState) || (SCHEDULETABLE_NEXT == pStFromCB->stState))
            {
                err = E_OS_NOFUNC;
            }
            /* OS309 */
            else if (SCHEDULETABLE_STOPPED != pStToCB->stState)
            {
                err = E_OS_STATE;
            }
            /* If ScheduleTable From is repeating, The functionality is not support. */
            else if (TRUE == pStFromCfgRef->osSchedTblRepeating)
            {
                err = E_OS_STATE;
            }
            else
            {
                /*nothing to do*/
            }
        }

        if ((StatusType)E_OK == err)
        {
            Os_NextScheduleTable(ScheduleTableID_From, ScheduleTableID_To);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(
            OSError_Save_NextScheduleTable(ScheduleTableID_From, ScheduleTableID_To),
            OSServiceId_NextScheduleTable,
            err);
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_NextScheduleTable);
#endif /* TRUE == CFG_TRACE_ENABLE */

    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
    OS_EXIT_KERNEL();
    /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/* Sync. */
#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <StartScheduleTableSynchron.>
 * Service ID           <0x0b>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <ScheduleTableID>
 * Param-Name[out]      <StatusType>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * REQ ID               <None>
 */
/********************************************************************/
FUNC(StatusType, OS_CODE) StartScheduleTableSynchron(ScheduleTableType ScheduleTableID)
{
    OS_ENTER_KERNEL();
    VAR(StatusType, OS_VAR) err = E_OK;
    P2CONST(Os_SchedTblCfgType, AUTOMATIC, OS_VAR) pStCfgRef;
    /* PRQA S 3432, 3678 ++ */ /* MISRA Rule 20.7, 8.13 */ /* OS_SCHEDULETABLE_TYPE_CAST_001 */
    P2VAR(Os_STCBType, AUTOMATIC, OS_VAR) pStCB;
    /* PRQA S 3432, 3678 -- */ /* MISRA Rule 20.7, 8.13 */
    VAR(Os_CoreIdType, OS_VAR) coreId;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_StartScheduleTableSynchron);
#endif /* TRUE == CFG_TRACE_ENABLE */

    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
    coreId = Os_GetObjCoreId(ScheduleTableID);
    /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */

    /* Cross core use shall be not supported. */
    if (Os_SCB.sysCore != coreId)
    {
        err = E_OS_CORE;
    }
#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
    else if (Os_GetObjLocalId(ScheduleTableID) >= Os_CfgSchedTblMax_Inf[Os_GetObjCoreId(ScheduleTableID)])
    /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
    {
        err = E_OS_ID;
    }
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    else if (Os_WrongContext(OS_CONTEXT_START_ST_SYN) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        err = E_OS_DISABLEDINT;
    }
    else if (Os_CheckObjAcs(OBJECT_SCHEDULETABLE, ScheduleTableID) != TRUE)
    {
        err = E_OS_ACCESS;
    }
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    else
    {
        /* PRQA S 3469, 1338 ++ */ /* MISRA Dir 4.9, 17.8  */ /* OS_SCHEDULETABLE_PARAM_MACRO_006 */
        ScheduleTableID = Os_GetObjLocalId(ScheduleTableID);
        /* PRQA S 3469, 1338 -- */ /* MISRA Dir 4.9, 17.8  */

        pStCfgRef = &Os_SchedTblCfg[ScheduleTableID];
        pStCB = &Os_STCB[ScheduleTableID];

        /* OS387 */
        if (ST_SYNC_EXPLICIT != pStCfgRef->osSchedTblSync.osSchedTblSyncStrategy)
        {
            err = E_OS_ID;
        }

        /* OS388 */
        else if (SCHEDULETABLE_STOPPED != pStCB->stState)
        {
            err = E_OS_STATE;
        }
        else
        {
            /*nothing to do*/
        }

        if ((StatusType)E_OK == err)
        {
            Os_StartScheduleTableSynchron(ScheduleTableID);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(
            OSError_Save_StartScheduleTableSynchron(ScheduleTableID),
            OSServiceId_StartScheduleTableSynchron,
            err);
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_StartScheduleTableSynchron);
#endif /* TRUE == CFG_TRACE_ENABLE */

    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
    OS_EXIT_KERNEL();
    /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Sync ScheduleTable.>
 * Service ID           <0x0c>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Yes>
 * param-eventId[in]    <ScheduleTableID, value>
 * Param-Name[out]      <StatusType>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * REQ ID               <None>
 */
/********************************************************************/
FUNC(StatusType, OS_CODE) SyncScheduleTable(ScheduleTableType ScheduleTableID, TickType value)
{
    OS_ENTER_KERNEL();
    P2CONST(Os_SchedTblCfgType, AUTOMATIC, OS_VAR) pStCfgRef;
    VAR(StatusType, OS_VAR) err = E_OK;
    VAR(Os_CoreIdType, OS_VAR) coreId;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_SyncScheduleTable);
#endif /* TRUE == CFG_TRACE_ENABLE */

    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
    coreId = Os_GetObjCoreId(ScheduleTableID);
    /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */

    /* Cross core use shall be not supported. */
    if (Os_SCB.sysCore != coreId)
    {
        err = E_OS_CORE;
    }
#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
    else if (Os_GetObjLocalId(ScheduleTableID) >= Os_CfgSchedTblMax_Inf[Os_GetObjCoreId(ScheduleTableID)])
    /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
    {
        err = E_OS_ID;
    }
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    else if (Os_WrongContext(OS_CONTEXT_SYNC_SCHEDULE_TABLE) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        err = E_OS_DISABLEDINT;
    }
    else if (Os_CheckObjAcs(OBJECT_SCHEDULETABLE, ScheduleTableID) != TRUE)
    {
        err = E_OS_ACCESS;
    }
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    else
    {
        /* PRQA S 3469, 1338 ++ */ /* MISRA Dir 4.9, 17.8  */ /* OS_SCHEDULETABLE_PARAM_MACRO_006 */
        ScheduleTableID = Os_GetObjLocalId(ScheduleTableID);
        /* PRQA S 3469, 1338 -- */ /* MISRA Dir 4.9, 17.8  */

        pStCfgRef = &Os_SchedTblCfg[ScheduleTableID];

        /* OS454. */
#if (OS_STATUS_EXTENDED == CFG_STATUS)
        if (ST_SYNC_EXPLICIT != pStCfgRef->osSchedTblSync.osSchedTblSyncStrategy)
        {
            err = E_OS_ID;
        }
        /* OS455. */
        else if (value >= pStCfgRef->osSchedTblDuration)
        {
            err = E_OS_VALUE;
        }
        else
        {
            /*nothing to do*/
        }
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */

        if ((StatusType)E_OK == err)
        {
            err = Os_SyncScheduleTable(ScheduleTableID, value);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_SyncScheduleTable(ScheduleTableID, value), OSServiceId_SyncScheduleTable, err);
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_SyncScheduleTable);
#endif /* TRUE == CFG_TRACE_ENABLE */

    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
    OS_EXIT_KERNEL();
    /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <SetScheduleTableAsync.>
 * Service ID           <0x0d>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Yes>
 * param-eventId[in]    <ScheduleTableID>
 * Param-Name[out]      <StatusType>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * REQ ID               <None>
 */
/********************************************************************/
FUNC(StatusType, OS_CODE) SetScheduleTableAsync(ScheduleTableType ScheduleTableID)
{
    OS_ENTER_KERNEL();
    /* PRQA S 3432, 3678 ++ */ /* MISRA Rule 20.7, 8.13 */ /* OS_SCHEDULETABLE_TYPE_CAST_001 */
    P2VAR(Os_STCBType, AUTOMATIC, OS_VAR) pStCB;
    /* PRQA S 3432, 3678 -- */ /* MISRA Rule 20.7, 8.13 */
    VAR(StatusType, OS_VAR) err = E_OK;
    VAR(Os_CoreIdType, OS_VAR) coreId;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_SetScheduleTableAsync);
#endif /* TRUE == CFG_TRACE_ENABLE */

    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
    coreId = Os_GetObjCoreId(ScheduleTableID);
    /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */

    /* Cross core use shall be not supported. */
    if (Os_SCB.sysCore != coreId)
    {
        err = E_OS_CORE;
    }
#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
    else if (Os_GetObjLocalId(ScheduleTableID) >= Os_CfgSchedTblMax_Inf[coreId])
    /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
    {
        err = E_OS_ID;
    }
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    else if (Os_WrongContext(OS_CONTEXT_SET_SCHEDULE_TABLE_ASYNC) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        err = E_OS_DISABLEDINT;
    }
    else if (Os_CheckObjAcs(OBJECT_SCHEDULETABLE, ScheduleTableID) != TRUE)
    {
        err = E_OS_ACCESS;
    }
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    else
    {
        /* PRQA S 3469, 1338 ++ */ /* MISRA Dir 4.9, 17.8  */ /* OS_SCHEDULETABLE_PARAM_MACRO_006 */
        ScheduleTableID = Os_GetObjLocalId(ScheduleTableID);
        /* PRQA S 3469, 1338 -- */ /* MISRA Dir 4.9, 17.8  */

        /*SWS_Os_00458:If OsScheduleTblSyncStrategy of <ScheduleTableID> in
         * a call of SetScheduleTableAsync() is not equal to EXPLICIT OR if
         * <ScheduleTableID> is invalid then SetScheduleTableAsync() shall
         * return E_OS_ID. */
        pStCB = &Os_STCB[ScheduleTableID];

        /* SWS_Os_00483:If state is STOPPED/NEXT/WAITTING, return E_OS_STATE. */
        if ((SCHEDULETABLE_STOPPED == pStCB->stState) || (SCHEDULETABLE_NEXT == pStCB->stState)
            || (SCHEDULETABLE_WAITING == pStCB->stState))
        {
            err = E_OS_STATE;
        }

        if ((StatusType)E_OK == err)
        {
            err = Os_SetScheduleTableAsync(ScheduleTableID);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_SetScheduleTableAsync(ScheduleTableID), OSServiceId_SetScheduleTableAsync, err);
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_SetScheduleTableAsync);
#endif /* TRUE == CFG_TRACE_ENABLE */

    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SCHEDULETABLE_MACRO_TO_FUNCTION_005 */
    OS_EXIT_KERNEL();
    /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* OS_SC2 == CFG_SC || OS_SC4 == CFG_SC */

#endif /* CFG_SCHEDTBL_MAX > 0U */

/*=======[E N D   O F   F I L E]==============================================*/
