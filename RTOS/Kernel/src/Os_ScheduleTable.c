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
 **  @file               : Os_ScheduleTable.c
 **  @author             : i-soft-os
 **  @date               : 2025/02/10
 **  @vendor             : isoft
 **  @description        : Os source file for ScheduleTable API implementations
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Os_Arch_Processor.h"
#include "Os_ScheduleTable.h"
#include "Os_Appl.h"
#include "Os_Counter.h"
#include "Os_Sprot.h"
#include "Os_Task.h"
#include "Os_Task.h"
#include "Os_Event.h"
#include "Os_Rpc.h"
#include "Os_Hook.h"
#include "Os_Kernel.h"
#include "Os_Err.h"
#include "Os_Rti.h"
#include "Os_Arti.h"

#if (CFG_SCHEDTBL_MAX > 0U)
/* ============================================ external data definitions =========================================== */

/* ============================================ internal data definitions =========================================== */

/* ========================================== internal function declarations ======================================== */
/**
 * @brief           Initializes a Schedule Table Control Block
 * @param[in]       schedTblId: ID of the schedule table to initialize
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_InitSchedTblCB(Os_ScheduleTableType schedTblId);

/**
 * @brief           Inserts a schedule table node into the schedule table list
 * @param[in]       schedTblId: ID of the schedule table to insert
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_InsertSchedTblNode(Os_ScheduleTableType schedTblId);

/**
 * @brief           Calculates the delay between expiry points
 * @param[in]       schedTblCfgRef: Pointer to schedule table configuration
 * @param[in]       pSchedTblNode: Pointer to schedule table control block
 * @return          Os_TickType
 * @retval          Time delay in ticks between current expiry point and the next one
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL Os_TickType Os_GetEpDelay(const Os_SchedTblCfgType *schedTblCfgRef, const Os_STCBType *const pSchedTblNode);

/**
 * @brief           Initializes a schedule table with startup parameters
 * @param[in]       startAbsTick: Absolute start tick value
 * @param[inout]    pStcb: Pointer to schedule table control block
 * @param[in]       schedTblCfgRef: Pointer to schedule table configuration
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_SchedTblInnerStart(Os_TickType startAbsTick, Os_STCBType *const pStcb, const Os_SchedTblCfgType *schedTblCfgRef);

/**
 * @brief           Initializes auto-start parameters for a schedule table
 * @param[inout]    pStcb: Pointer to schedule table control block
 * @param[in]       schedTblCfgRef: Pointer to schedule table configuration
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_InitSchedTblAutoStart(Os_STCBType *const pStcb, const Os_SchedTblCfgType *schedTblCfgRef);

/**
 * @brief           Checks if a schedule table should auto-start based on application mode
 * @param[in]       schedTblAutoStartRef: Pointer to schedule table auto-start configuration
 * @return          boolean
 * @retval          TRUE: Schedule table should auto-start
 * @retval          FALSE: Schedule table should not auto-start
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL boolean Os_SchedTblAutoStartCheck(const Os_SchedTblAutostart *schedTblAutoStartRef);

/**
 * @brief           Inserts a schedule table node into the schedule table list based on expiry time
 * @param[in]       schedTblRefCounterId: Counter ID referenced by the schedule table
 * @param[inout]    pInsertStcb: Pointer to schedule table control block to insert
 * @param[inout]    pCcb: Pointer to counter control block
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_InsertSchedTblList(Os_CounterType schedTblRefCounterId, Os_STCBType *pInsertStcb, Os_CCBType *pCcb);

/**
 * @brief           Removes a schedule table node from the schedule table list
 * @param[in]       delSchedTblId: ID of the schedule table to delete
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_DelSchedTblNode(ScheduleTableType delSchedTblId);

/**
 * @brief           Activates tasks at expiry points of a schedule table
 * @param[in]       taskListRef: Pointer to task activation list
 * @param[in]       taskCnt: Number of tasks in the activation list
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_SchedTblEpActiveTask(const Os_EPActivateTaskAction *taskListRef, uint16 taskCnt);

#if (CFG_EXTENDED_TASK_MAX > 0)
/**
 * @brief           Sets events for tasks at expiry points of a schedule table
 * @param[in]       eventListRef: Pointer to event setting list
 * @param[in]       eventCnt: Number of events in the list
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_SchedTblEpSetEvent(const Os_EPSetEventAction *eventListRef, uint16 eventCnt);
#endif

/**
 * @brief           Processes the work to be done at an expiry point
 * @param[in]       pSchedTblNode: Pointer to schedule table control block
 * @param[in]       schedTblCfgRef: Pointer to schedule table configuration
 * @return          boolean
 * @retval          TRUE: This was the last expiry point
 * @retval          FALSE: This was not the last expiry point
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL boolean Os_WorkSchedTblEp(const Os_STCBType *const pSchedTblNode, const Os_SchedTblCfgType *schedTblCfgRef);

/**
 * @brief           Handles the repeating of a schedule table after reaching the final expiry point
 * @param[inout]    pSchedTblNode: Pointer to schedule table control block
 * @param[in]       schedTblCfgRef: Pointer to schedule table configuration
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_WorkSchedTblRepeat(Os_STCBType *const pSchedTblNode, const Os_SchedTblCfgType *schedTblCfgRef);

/**
 * @brief           Updates schedule table state to process the next expiry point
 * @param[inout]    pSchedTblNode: Pointer to schedule table control block
 * @param[in]       schedTblCfgRef: Pointer to schedule table configuration
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_WorkSchedTblToNextEp(Os_STCBType *const pSchedTblNode, const Os_SchedTblCfgType *schedTblCfgRef);

#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
/**
 * @brief           Performs synchronization adjustment by adding ticks
 * @param[inout]    pStcb: Pointer to schedule table control block
 * @param[in]       schedTblCfgRef: Pointer to schedule table configuration
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_SyncAdjustEpByAdd(Os_STCBType *const pStcb, const Os_SchedTblCfgType *schedTblCfgRef);

/**
 * @brief           Performs synchronization adjustment by subtracting ticks
 * @param[inout]    pStcb: Pointer to schedule table control block
 * @param[in]       schedTblCfgRef: Pointer to schedule table configuration
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_SyncAdjustEpBySub(Os_STCBType *const pStcb, const Os_SchedTblCfgType *schedTblCfgRef);

/**
 * @brief           Performs synchronization adjustment for an expiry point
 * @param[inout]    pStcb: Pointer to schedule table control block
 * @param[in]       schedTblCfgRef: Pointer to schedule table configuration
 * @param[in]       counterCfgRef: Pointer to counter configuration
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_WorkSyncAdjustEp(Os_STCBType *const pStcb, const Os_SchedTblCfgType *schedTblCfgRef, const Os_CounterCfgType *counterCfgRef);
#endif

/**
 * @brief           Handles schedule table processing when in RUNNING state
 * @param[inout]    pSchedTblNode: Pointer to schedule table control block
 * @param[in]       schedTblCfgRef: Pointer to schedule table configuration
 * @param[in]       counterCfgRef: Pointer to counter configuration
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_WorkSchedTblRunningState(Os_STCBType *const pSchedTblNode, const Os_SchedTblCfgType *schedTblCfgRef, const Os_CounterCfgType *counterCfgRef);

#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
/**
 * @brief           Handles schedule table processing when in RUNNING_AND_SYNCHRONOUS state
 * @param[inout]    pSchedTblNode: Pointer to schedule table control block
 * @param[in]       schedTblCfgRef: Pointer to schedule table configuration
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_WorkSchedTblRunningAndSyncState(Os_STCBType *const pSchedTblNode, const Os_SchedTblCfgType *schedTblCfgRef);

/**
 * @brief           Handles schedule table synchronization when in WAITING state
 * @param[in]       syncVal: Synchronization value (position in physical time)
 * @param[in]       counterCurVal: Current counter value
 * @param[inout]    pStcb: Pointer to schedule table control block
 * @param[in]       schedTblCfgRef: Pointer to schedule table configuration
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_SchedTblSyncInWaittingState(Os_TickType syncVal, Os_TickType counterCurVal, Os_STCBType *const pStcb, const Os_SchedTblCfgType *schedTblCfgRef);

/**
 * @brief           Calculates the difference value for schedule table synchronization
 * @param[in]       syncVal: Synchronization value (position in physical time)
 * @param[in]       counterCurVal: Current counter value
 * @param[inout]    pStcb: Pointer to schedule table control block
 * @param[in]       schedTblCfgRef: Pointer to schedule table configuration
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_SchedTblSyncGetDiffVal(Os_TickType syncVal, Os_TickType counterCurVal, Os_STCBType *const pStcb, const Os_SchedTblCfgType *schedTblCfgRef);

/**
 * @brief           Handles schedule table synchronization when in RUNNING state
 * @param[in]       syncVal: Synchronization value (position in physical time)
 * @param[in]       counterCurVal: Current counter value
 * @param[inout]    pStcb: Pointer to schedule table control block
 * @param[in]       schedTblCfgRef: Pointer to schedule table configuration
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_SchedTblSyncInRunningState(Os_TickType syncVal, Os_TickType counterCurVal, Os_STCBType *const pStcb, const Os_SchedTblCfgType *schedTblCfgRef);

/**
 * @brief           Handles schedule table synchronization when in RUNNING_AND_SYNCHRONOUS state
 * @param[in]       syncVal: Synchronization value (position in physical time)
 * @param[in]       counterCurVal: Current counter value
 * @param[inout]    pStcb: Pointer to schedule table control block
 * @param[in]       schedTblCfgRef: Pointer to schedule table configuration
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_SchedTblSyncInRunningAndSyncState(Os_TickType syncVal, Os_TickType counterCurVal, Os_STCBType *const pStcb, const Os_SchedTblCfgType *schedTblCfgRef);
#endif

/**
 * @brief           Processes an expiry point of a schedule table
 * @param[inout]    pScb: Pointer to system control block
 * @param[inout]    pSchedTblListHead: Pointer to head of schedule table list
 * @param[in]       counterCfgRef: Pointer to counter configuration
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_SchedTblEpProc(Os_SCBType *pScb, Os_STCBType *pSchedTblListHead, const Os_CounterCfgType *counterCfgRef);

/**
 * @brief           Retrieves the current status of a schedule table
 * @param[in]       schedTblId: ID of the schedule table
 * @param[out]      schedTblStatus: Pointer to store the schedule table status
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_GetScheduleTableStatus(ScheduleTableType schedTblId, ScheduleTableStatusRefType schedTblStatus);

/**
 * @brief           Checks if a relative schedule table start request is valid
 * @param[in]       schedTblId: ID of the schedule table
 * @param[in]       offset: Relative start offset in ticks
 * @return          StatusType
 * @retval          E_OK: Request is valid
 * @retval          E_OS_ID: Schedule table ID is invalid
 * @retval          E_OS_VALUE: Offset is invalid
 * @retval          E_OS_STATE: Schedule table is not in STOPPED state
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_StartRelStatusCheck(ScheduleTableType schedTblId, TickType offset);

/**
 * @brief           Starts a schedule table with a relative offset
 * @param[in]       schedTblId: ID of the schedule table
 * @param[in]       offset: Relative start offset in ticks
 * @return          StatusType
 * @retval          E_OK: Schedule table started successfully
 * @retval          E_OS_ID: Schedule table ID is invalid
 * @retval          E_OS_VALUE: Offset is invalid
 * @retval          E_OS_STATE: Schedule table is not in STOPPED state
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_StartScheduleTableRel(ScheduleTableType schedTblId, TickType offset);

/**
 * @brief           Starts a schedule table at an absolute counter value
 * @param[in]       schedTblId: ID of the schedule table
 * @param[in]       start: Absolute start tick value
 * @return          StatusType
 * @retval          E_OK: Schedule table started successfully
 * @retval          E_OS_VALUE: Start value is invalid
 * @retval          E_OS_STATE: Schedule table is not in STOPPED state
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_StartScheduleTableAbs(ScheduleTableType schedTblId, TickType start);

/**
 * @brief           Sets up a next schedule table to be started after a currently running one
 * @param[in]       schedTblIdFrom: ID of the currently running schedule table
 * @param[in]       schedTblIdTo: ID of the schedule table to start next
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_NextScheduleTable(ScheduleTableType schedTblIdFrom, ScheduleTableType schedTblIdTo);

#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
/**
 * @brief           Checks if a synchronized start request for a schedule table is valid
 * @param[in]       schedTblId: ID of the schedule table
 * @return          StatusType
 * @retval          E_OK: Request is valid
 * @retval          E_OS_ID: Schedule table doesn't support explicit synchronization
 * @retval          E_OS_STATE: Schedule table is not in STOPPED state
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_StartSyncStatusCheck(ScheduleTableType schedTblId);

/**
 * @brief           Starts a schedule table in synchronous mode
 * @param[in]       schedTblId: ID of the schedule table
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_StartScheduleTableSynchron(ScheduleTableType schedTblId);

/**
 * @brief           Checks if a schedule table synchronization request is valid
 * @param[in]       schedTblId: ID of the schedule table
 * @param[in]       value: Synchronization value (position in physical time)
 * @return          StatusType
 * @retval          E_OK: Request is valid
 * @retval          E_OS_ID: Schedule table doesn't support explicit synchronization
 * @retval          E_OS_VALUE: Synchronization value is invalid
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_SyncStatusCheck(ScheduleTableType schedTblId, TickType value);

/**
 * @brief           Synchronizes a schedule table with a given synchronization value
 * @param[in]       schedTblId: ID of the schedule table
 * @param[in]       value: Synchronization value (position in physical time)
 * @return          StatusType
 * @retval          E_OK: Schedule table synchronized successfully
 * @retval          E_OS_STATE: Schedule table is not in appropriate state for synchronization
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_SyncScheduleTable(ScheduleTableType schedTblId, TickType value);

/**
 * @brief           Sets a schedule table to asynchronous mode
 * @param[in]       schedTblId: ID of the schedule table
 * @return          StatusType
 * @retval          E_OK: Schedule table set to asynchronous successfully
 * @retval          E_OS_ID: Schedule table doesn't support explicit synchronization
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_SetScheduleTableAsync(ScheduleTableType schedTblId);
#endif

#if (OS_AUTOSAR_CORES > 1)
/**
 * @brief           RPC action handler for starting a schedule table with a relative offset
 * @param[in]       inPara: Parameter array containing schedule table ID and offset
 * @return          StatusType
 * @retval          E_OK: Schedule table started successfully
 * @retval          Other: Error code from schedule table start
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_RpcAction_StartScheduleTableRel(uint32 *inPara);

/**
 * @brief           Makes an RPC call to start a schedule table with a relative offset on another core
 * @param[in]       ownerCore: ID of the core owning the schedule table
 * @param[in]       schedTblId: ID of the schedule table
 * @param[in]       offset: Relative start offset in ticks
 * @return          StatusType
 * @retval          E_OK: Schedule table started successfully
 * @retval          Other: Error code from remote core
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_RpcCall_StartScheduleTableRel(Os_CoreIdType ownerCore, ScheduleTableType schedTblId, TickType offset);

/**
 * @brief           RPC action handler for starting a schedule table at an absolute counter value
 * @param[in]       inPara: Parameter array containing schedule table ID and start value
 * @return          StatusType
 * @retval          E_OK: Schedule table started successfully
 * @retval          Other: Error code from schedule table start
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_RpcAction_StartScheduleTableAbs(uint32 *inPara);

/**
 * @brief           Makes an RPC call to start a schedule table at an absolute counter value on another core
 * @param[in]       ownerCore: ID of the core owning the schedule table
 * @param[in]       schedTblId: ID of the schedule table
 * @param[in]       start: Absolute start tick value
 * @return          StatusType
 * @retval          E_OK: Schedule table started successfully
 * @retval          Other: Error code from remote core
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_RpcCall_StartScheduleTableAbs(Os_CoreIdType ownerCore, ScheduleTableType schedTblId, TickType start);

/**
 * @brief           RPC action handler for stopping a schedule table
 * @param[in]       inPara: Parameter array containing schedule table ID
 * @return          StatusType
 * @retval          E_OK: Schedule table stopped successfully
 * @retval          Other: Error code from schedule table stop
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_RpcAction_StopScheduleTable(uint32 *inPara);

/**
 * @brief           Makes an RPC call to stop a schedule table on another core
 * @param[in]       ownerCore: ID of the core owning the schedule table
 * @param[in]       schedTblId: ID of the schedule table
 * @return          StatusType
 * @retval          E_OK: Schedule table stopped successfully
 * @retval          Other: Error code from remote core
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_RpcCall_StopScheduleTable(Os_CoreIdType ownerCore, ScheduleTableType schedTblId);
#endif

/**
 * @brief           Verifies if a next schedule table request is valid
 * @param[in]       schedTblIdFrom: ID of the currently running schedule table
 * @param[in]       schedTblIdTo: ID of the schedule table to start next
 * @return          StatusType
 * @retval          E_OK: Request is valid
 * @retval          E_OS_ID: Schedule tables use different counters or sync strategies
 * @retval          E_OS_NOFUNC: From schedule table is not in proper state
 * @retval          E_OS_STATE: To schedule table is not stopped or source is repeating
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_NextSTCheckStatus(ScheduleTableType schedTblIdFrom, ScheduleTableType schedTblIdTo);

/* ========================================== external function definitions ========================================= */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Init ScheduleTable during os_startup.
 */
void Os_InitScheduleTable(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    Os_CoreIdType coreId = Os_GetCoreIdLocal();
    Os_ScheduleTableType idStartRange = Os_CfgScheduleTableIndex_Inf[coreId].SchedTblStart;
    Os_ScheduleTableType idEndRange = Os_CfgScheduleTableIndex_Inf[coreId].SchedTblEnd;

    for (uint16 i = (uint16)idStartRange; i < idEndRange; i++) /* PRQA S 1880 */ /* VL_Os_1880 */
    {
        const Os_SchedTblCfgType *schedTblCfgRef = &Os_SchedTblCfg[i];

        Os_InitSchedTblCB(i); /* PRQA S 4442, 1441 */ /* VL_Os_4442, VL_Os_1441 */

        /* Auto start. */
        if (TRUE == Os_SchedTblAutoStartCheck(schedTblCfgRef->SchedTblAutoStartRef))
        {
            Os_InitSchedTblAutoStart(Os_STCB[i], schedTblCfgRef);
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Os_InitSchedTblAutoStart.
 */
OS_LOCAL void Os_InitSchedTblAutoStart(
    Os_STCBType *const pStcb, /* PRQA S 3432 */ /* VL_Os_3432 */
    const Os_SchedTblCfgType *schedTblCfgRef)
{
    switch (schedTblCfgRef->SchedTblAutoStartRef->SchedTblAutostartType)
    {
    case OS_ST_START_ABSOLUTE:
        Os_SchedTblInnerStart(schedTblCfgRef->SchedTblAutoStartRef->SchedTblAbsValue,
                              pStcb,
                              schedTblCfgRef);

        /* Insert ScheduleTable to list. */
        Os_InsertSchedTblNode(pStcb->SchedTblId);
        break;

    case OS_ST_START_RELATIVE:
    {
        Os_TickType startAbsTick = Os_CalcAbsTicks(
            Os_CCB[schedTblCfgRef->SchedTblCounterRef]->CounterCurVal,
            schedTblCfgRef->SchedTblAutoStartRef->SchedTblRelOffset,
            schedTblCfgRef->SchedTblCounterRef);

        Os_SchedTblInnerStart(startAbsTick, pStcb, schedTblCfgRef);

        /* Insert ScheduleTable to list. */
        Os_InsertSchedTblNode(pStcb->SchedTblId);
        break;
    }

#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
    case OS_ST_START_SYNCHRON:
        pStcb->SchedTblState = SCHEDULETABLE_WAITING;
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
/**
 * Check whether this st can auto start.
 */
OS_LOCAL boolean Os_SchedTblAutoStartCheck(
    const Os_SchedTblAutostart *schedTblAutoStartRef)
{
    boolean autoStart = FALSE;

    if (NULL_PTR != schedTblAutoStartRef)
    {
        Os_SCBType *pScb = Os_GetCurrentContext(); /* PRQA S 3678 */ /* VL_Os_3678 */

        /* Check whether appmode during StartOs in appmode bitmap. */
        if (0U != (schedTblAutoStartRef->SchedTblAppModeBitmap & pScb->SysActiveAppMode))
        {
            autoStart = TRUE;
        }
    }

    return autoStart;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Init ScheduleTable.
 */
OS_LOCAL void Os_InitSchedTblCB(Os_ScheduleTableType schedTblId)
{
    Os_STCBType *pStcb = Os_STCB[schedTblId];
    const Os_SchedTblCfgType *schedTblCfgRef = &Os_SchedTblCfg[schedTblId];

    /* Init STCB */
    pStcb->SchedTblId = schedTblId;
    pStcb->SchedTblIsAdjust = FALSE;
    pStcb->SchedTblIsAdjustAdd = FALSE;
    pStcb->SchedTblIsStarted = FALSE;
    pStcb->SchedTblIsStopAdjust = FALSE;
    pStcb->SchedTblNextEP = schedTblCfgRef->SchedTblEP;
    pStcb->SchedTblDiff = 0U;
    pStcb->SchedTblDev = 0U;
    pStcb->SchedTblStartAbsTick = 0U;
    pStcb->SchedTblNextEpAbsTick = 0U;
    pStcb->SchedTblState = SCHEDULETABLE_STOPPED;
    pStcb->SchedTblEpId = 0U;
    pStcb->NextSchedTblId = OS_SCHEDTABLE_INVALID; /* PRQA S 4342 */ /* VL_Os_4342 */
    pStcb->PrevSchedTblId = OS_SCHEDTABLE_INVALID; /* PRQA S 4342 */ /* VL_Os_4342 */
    pStcb->SchedTblNextNode = NULL_PTR;
    pStcb->SchedTblPreNode = NULL_PTR;
    pStcb->SchedTblFinalDelay = schedTblCfgRef->SchedTblDuration -
                                schedTblCfgRef->SchedTblEP[schedTblCfgRef->SchedTblEPsize - 1u].SchedTblEPOffset;
    pStcb->SchedTblDelay = Os_GetEpDelay(schedTblCfgRef, pStcb);

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Insert scheduleTable node to STlist.
 */
OS_LOCAL void Os_InsertSchedTblList(
    Os_CounterType schedTblRefCounterId,
    Os_STCBType *pInsertStcb,
    Os_CCBType *pCcb)
{
    /* If list is not empty, insert node by EP rel tick, increase order.
     * Modified, use relative ticks between current tick and EP*/
    Os_TickType InsertEpRelTick = Os_GetDistance(pCcb->CounterLastVal,
                                                 pInsertStcb->SchedTblNextEpAbsTick,
                                                 schedTblRefCounterId);
    Os_STCBType *pCurStcb = pCcb->CounterStListHead;
    Os_STCBType *pPreStcb = NULL_PTR;

    /* Find insert position. */
    while (NULL_PTR != pCurStcb)
    {
        /* Insert node by increase order. */
        if (InsertEpRelTick >= Os_GetDistance(pCcb->CounterLastVal,
                                              pCurStcb->SchedTblNextEpAbsTick,
                                              schedTblRefCounterId))
        {
            pPreStcb = pCurStcb;
            pCurStcb = pCurStcb->SchedTblNextNode;
        }
        else
        {
            break;
        }
    }

    /* Insert to head position. */
    if (NULL_PTR == pPreStcb)
    {
        /* Update head node. */
        pCcb->CounterStListHead = pInsertStcb;
        pInsertStcb->SchedTblNextNode = pCurStcb;
        pInsertStcb->SchedTblPreNode = NULL_PTR;
        pCurStcb->SchedTblPreNode = pInsertStcb; /* PRQA S 2813 */ /* VL_Os_2813 */
    }
    else
    {
        /* Insert to other position. */
        pInsertStcb->SchedTblNextNode = pPreStcb->SchedTblNextNode;
        pInsertStcb->SchedTblPreNode = pPreStcb;
        pPreStcb->SchedTblNextNode = pInsertStcb;

        /* Process pCurStcb. */
        if (NULL_PTR != pCurStcb)
        {
            pCurStcb->SchedTblPreNode = pInsertStcb;
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Insert scheduleTable node to list(Array).
 */
OS_LOCAL void Os_InsertSchedTblNode(Os_ScheduleTableType schedTblId)
{
    Os_CounterType schedTblRefCounterId = Os_SchedTblCfg[schedTblId].SchedTblCounterRef;
    Os_STCBType *pInsertStcb = Os_STCB[schedTblId];
    Os_CCBType *pCcb = Os_CCB[schedTblRefCounterId];

    /* Note: can not repeat insert. */
    if ((NULL_PTR != pInsertStcb->SchedTblNextNode) || (NULL_PTR != pInsertStcb->SchedTblPreNode))
    {
        /*nothing to do*/
    }
    /* Flexible design: if list is empty, insert node directly. */
    else if (NULL_PTR == pCcb->CounterStListHead)
    {
        pInsertStcb->SchedTblNextNode = NULL_PTR;
        pCcb->CounterStListHead = pInsertStcb;
    }
    else
    {
        /* STList is not empty */
        Os_InsertSchedTblList(schedTblRefCounterId, pInsertStcb, pCcb);
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Remove the ScheduleTable node from STList.
 */
OS_LOCAL void Os_DelSchedTblNode(ScheduleTableType delSchedTblId)
{
    Os_CCBType *pCcb = Os_CCB[Os_SchedTblCfg[delSchedTblId].SchedTblCounterRef];

    /* If list is null, return. */
    if (NULL_PTR == pCcb->CounterStListHead)
    {
        /*nothing to do*/
    }
    else
    {
        Os_STCBType *pDelStcb = Os_STCB[delSchedTblId];

        /* Flexible design: delSchedTblId is head node. */
        if (NULL_PTR == pDelStcb->SchedTblPreNode)
        {
            pCcb->CounterStListHead = pDelStcb->SchedTblNextNode;

            if (NULL_PTR != pDelStcb->SchedTblNextNode)
            {
                pDelStcb->SchedTblNextNode->SchedTblPreNode = NULL_PTR;
                pDelStcb->SchedTblNextNode = NULL_PTR;
            }
        }
        else
        {
            /* Normal delete. */
            pDelStcb->SchedTblPreNode->SchedTblNextNode = pDelStcb->SchedTblNextNode;

            if (NULL_PTR != pDelStcb->SchedTblNextNode) /* Not tail node. */
            {
                pDelStcb->SchedTblNextNode->SchedTblPreNode = pDelStcb->SchedTblPreNode;
            }

            pDelStcb->SchedTblPreNode = NULL_PTR;
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Calculate the delay between EPs.
 */
OS_LOCAL Os_TickType Os_GetEpDelay(
    const Os_SchedTblCfgType *schedTblCfgRef,
    const Os_STCBType *const pSchedTblNode)
{
    Os_TickType osDelay = 0U;

    /* Last Ep. Figure 7.1 */
    if (pSchedTblNode->SchedTblEpId >= (schedTblCfgRef->SchedTblEPsize - 1u))
    {
        osDelay = pSchedTblNode->SchedTblFinalDelay + schedTblCfgRef->SchedTblEP[0].SchedTblEPOffset;
    }
    else
    {
        osDelay = schedTblCfgRef->SchedTblEP[pSchedTblNode->SchedTblEpId + 1u].SchedTblEPOffset -
                  schedTblCfgRef->SchedTblEP[pSchedTblNode->SchedTblEpId].SchedTblEPOffset;
    }

    return osDelay;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Save start para of schedule table.
 */
OS_LOCAL void Os_SchedTblInnerStart(
    Os_TickType startAbsTick,
    Os_STCBType *const pStcb, /* PRQA S 3432 */ /* VL_Os_3432 */
    const Os_SchedTblCfgType *schedTblCfgRef)
{
    pStcb->SchedTblStartAbsTick = startAbsTick;
    pStcb->SchedTblNextEpAbsTick = Os_CalcAbsTicks(pStcb->SchedTblStartAbsTick,
                                                   schedTblCfgRef->SchedTblEP[0].SchedTblEPOffset,
                                                   schedTblCfgRef->SchedTblCounterRef);
    pStcb->SchedTblEpId = 0u;
    pStcb->SchedTblIsStarted = TRUE;
    /* Set state to running. */
    pStcb->SchedTblState = SCHEDULETABLE_RUNNING;

/* If SyncStrategy is IMPLICIT, state should be RUNNING_AND_SYNCHRONOUS. */
#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
    if (OS_ST_SYNC_IMPLICIT == schedTblCfgRef->SchedTblSync.SchedTblSyncStrategy)
    {
        pStcb->SchedTblState = SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS;
    }
#endif

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Activate tasks at expiry points
 */
OS_LOCAL void Os_SchedTblEpActiveTask(const Os_EPActivateTaskAction *taskListRef, uint16 taskCnt)
{
    for (uint16 i = 0u; i < taskCnt; i++)
    {
        Os_TaskType taskId = taskListRef[i].SchedTblActivateTaskRef;

#if (OS_AUTOSAR_CORES > 1)
        Os_SCBType *pScb = Os_GetCurrentContext(); /* PRQA S 3678 */ /* VL_Os_3678 */
        Os_CoreIdType coreId = OS_TASK_GET_COREID(taskId);
        if (coreId != pScb->SysCore)
        {
            (void)Os_RpcCall_ActivateTask(coreId, OS_RPC_ASYNC, taskId);
        }
        else
#endif
        {
            (void)Os_ActivateTask(taskId);
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (CFG_EXTENDED_TASK_MAX > 0)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Set Event at expiry points
 */
OS_LOCAL void Os_SchedTblEpSetEvent(const Os_EPSetEventAction *eventListRef, uint16 eventCnt)
{
    for (uint16 i = 0u; i < eventCnt; i++)
    {
        Os_TaskType taskId = eventListRef[i].SchedTblSetEventTaskRef;
        Os_EventMaskType eventMask = eventListRef[i].SchedTblSetEventRef;

#if (OS_AUTOSAR_CORES > 1)
        Os_SCBType *pScb = Os_GetCurrentContext(); /* PRQA S 3678 */ /* VL_Os_3678 */
        Os_CoreIdType coreId = OS_TASK_GET_COREID(taskId);
        if (coreId != pScb->SysCore)
        {
            (void)Os_RpcCall_SetEvent(coreId, OS_RPC_ASYNC, taskId, eventMask);
        }
        else
#endif
        {
            (void)Os_SetEvent(taskId, eventMask);
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Procedures for handling at expiry points
 */
OS_LOCAL boolean Os_WorkSchedTblEp(
    const Os_STCBType *const pSchedTblNode,
    const Os_SchedTblCfgType *schedTblCfgRef)
{
    boolean lastEp = FALSE;

    /* Active task. */
    Os_SchedTblEpActiveTask(pSchedTblNode->SchedTblNextEP->ActivateTaskList,
                            pSchedTblNode->SchedTblNextEP->ActivateTaskListSize);

/* Set event. */
#if (CFG_EXTENDED_TASK_MAX > 0)
    Os_SchedTblEpSetEvent(pSchedTblNode->SchedTblNextEP->SetEventList,
                          pSchedTblNode->SchedTblNextEP->SetEventListSize);
#endif

    /* Last EP ? */
    if (pSchedTblNode->SchedTblEpId >= (schedTblCfgRef->SchedTblEPsize - 1u))
    {
        lastEp = TRUE;
    }

    return lastEp;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * A schedule table may repeat after the final expiry point is processed.
 *                       So the API can work
 */
OS_LOCAL void Os_WorkSchedTblRepeat(
    Os_STCBType *const pSchedTblNode, /* PRQA S 3432 */ /* VL_Os_3432 */
    const Os_SchedTblCfgType *schedTblCfgRef)
{
    pSchedTblNode->SchedTblNextEP = schedTblCfgRef->SchedTblEP;
    pSchedTblNode->SchedTblEpId = 0u;

/* Update start abs tick. */
    pSchedTblNode->SchedTblNextEpAbsTick = Os_CalcAbsTicks(pSchedTblNode->SchedTblNextEpAbsTick,
                                                            pSchedTblNode->SchedTblDelay,
                                                            schedTblCfgRef->SchedTblCounterRef);
    pSchedTblNode->SchedTblDelay = Os_GetEpDelay(schedTblCfgRef, pSchedTblNode);

    /* Re-sort by abs tick of next EP. */
    Os_DelSchedTblNode(pSchedTblNode->SchedTblId);
    Os_InsertSchedTblNode(pSchedTblNode->SchedTblId);

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Procedures for handling the next expiry point
 */
OS_LOCAL void Os_WorkSchedTblToNextEp(
    Os_STCBType *const pSchedTblNode,
    const Os_SchedTblCfgType *schedTblCfgRef)
{
    /* Next EP. */
    pSchedTblNode->SchedTblEpId = pSchedTblNode->SchedTblEpId + 1u;
    pSchedTblNode->SchedTblNextEP = &schedTblCfgRef->SchedTblEP[pSchedTblNode->SchedTblEpId];
    pSchedTblNode->SchedTblNextEpAbsTick = Os_CalcAbsTicks(pSchedTblNode->SchedTblNextEpAbsTick,
                                                           pSchedTblNode->SchedTblDelay,
                                                           schedTblCfgRef->SchedTblCounterRef);
    /* Update delay. */
    pSchedTblNode->SchedTblDelay = Os_GetEpDelay(schedTblCfgRef, pSchedTblNode);

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/* Sync adjust. */
#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Complete EPs synchronization using addition.
 */
OS_LOCAL void Os_SyncAdjustEpByAdd(
    Os_STCBType *const pStcb,
    const Os_SchedTblCfgType *schedTblCfgRef)
{
    /* Deviatifon. */
    pStcb->SchedTblDev = (((pStcb->SchedTblDiff - pStcb->SchedTblAdjustDevTotal) > pStcb->SchedTblNextEP->SchedTblMaxAdvance) ? (pStcb->SchedTblNextEP->SchedTblMaxAdvance) : (pStcb->SchedTblDiff - pStcb->SchedTblAdjustDevTotal));

    /* pStcb->SchedTblAdjustDevTotal <= pStcb->SchedTblDiff. */
    if ((pStcb->SchedTblDiff - pStcb->SchedTblAdjustDevTotal) <= schedTblCfgRef->SchedTblSync.SchedTblExplicitPrecision)
    {
        /* Sync success. */
        pStcb->SchedTblIsAdjust = FALSE;
        pStcb->SchedTblAdjustDevTotal = 0u;
        pStcb->SchedTblDiff = 0u;
        pStcb->SchedTblDev = 0u;
    }
    else
    {
        /* SchedTblNextEP need adjust. */
        pStcb->SchedTblIsAdjust = TRUE;
        pStcb->SchedTblAdjustDevTotal += pStcb->SchedTblDev;

        /* Update delay of SchedTblNextEP. */
        pStcb->SchedTblDelay += pStcb->SchedTblDev;
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Complete EPs synchronization using subtraction.
 */
OS_LOCAL void Os_SyncAdjustEpBySub(
    Os_STCBType *const pStcb,
    const Os_SchedTblCfgType *schedTblCfgRef)
{
    /* Deviatifon. */
    pStcb->SchedTblDev = (((pStcb->SchedTblDiff - pStcb->SchedTblAdjustDevTotal) > pStcb->SchedTblNextEP->SchedTblMaxRetard) ? (pStcb->SchedTblNextEP->SchedTblMaxRetard) : (pStcb->SchedTblDiff - pStcb->SchedTblAdjustDevTotal));

    /* pStcb->SchedTblAdjustDevTotal <= pStcb->SchedTblDiff. */
    if ((pStcb->SchedTblDiff - pStcb->SchedTblAdjustDevTotal) <= schedTblCfgRef->SchedTblSync.SchedTblExplicitPrecision)
    {
        /* Sync success. */
        pStcb->SchedTblIsAdjust = FALSE;
        pStcb->SchedTblAdjustDevTotal = 0u;
        pStcb->SchedTblDiff = 0u;
        pStcb->SchedTblDev = 0u;
    }
    else
    {
        /* SchedTblNextEP need adjust. */
        pStcb->SchedTblIsAdjust = TRUE;
        pStcb->SchedTblAdjustDevTotal += pStcb->SchedTblDev;

        /* Update delay of SchedTblNextEP. */
        pStcb->SchedTblDelay -= pStcb->SchedTblDev;
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * The API can use When expiry point need synchronization
 */
OS_LOCAL void Os_WorkSyncAdjustEp(
    Os_STCBType *const pStcb,
    const Os_SchedTblCfgType *schedTblCfgRef,
    const Os_CounterCfgType *counterCfgRef)
{
    /* Delay + Deviatifon. */
    if (TRUE == pStcb->SchedTblIsAdjustAdd)
    {
        /* OS437 */
        if (((pStcb->SchedTblNextEP->SchedTblEPOffset) + (pStcb->SchedTblNextEP->SchedTblMaxAdvance)) > (schedTblCfgRef->SchedTblDuration))
        {
            /*nothing to do*/
        }
        else
        {
            Os_SyncAdjustEpByAdd(pStcb, schedTblCfgRef);
        }
    }
    else /* Delay - Deviatifon. */
    {
        /* Offset of pre EP. */
        Os_TickType preEpOffset = ((0u == pStcb->SchedTblEpId) ? (0u) : (schedTblCfgRef->SchedTblEP[pStcb->SchedTblEpId - 1u].SchedTblEPOffset));
        /* OS436 */
        if (((pStcb->SchedTblNextEP->SchedTblEPOffset) - (pStcb->SchedTblNextEP->SchedTblMaxRetard)) < ((preEpOffset) + (counterCfgRef->CounterMinCycle)))
        {
            /*nothing to do*/
        }
        else
        {
            Os_SyncAdjustEpBySub(pStcb, schedTblCfgRef);
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Os_WorkSchedTblRunningState.
 */
OS_LOCAL void Os_WorkSchedTblRunningState(
    Os_STCBType *const pSchedTblNode,
    const Os_SchedTblCfgType *schedTblCfgRef,
    const Os_CounterCfgType *counterCfgRef)
{
    boolean lastEp = Os_WorkSchedTblEp(pSchedTblNode, schedTblCfgRef);

/* Sync: adjust. */
#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
    if ((FALSE == pSchedTblNode->SchedTblIsStopAdjust) && (TRUE == pSchedTblNode->SchedTblIsAdjust))
    {
        Os_WorkSyncAdjustEp(pSchedTblNode, schedTblCfgRef, counterCfgRef);

        if (TRUE != pSchedTblNode->SchedTblIsAdjust)
        {
            pSchedTblNode->SchedTblState = SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS;
        }
    }
#else
    UNUSED_PARAMETER(counterCfgRef);
#endif

    /* Last EP. */
    if (TRUE == lastEp)
    {
        /* Repeating. */
        /* Note: in standard, from 7.3.2.4 && Figure 7.8, we conclude that a schedule table which can synchronous explicitly
                 might be repeating. But we can not conclude that explicit synchronous schedule table must be repeating,
                 because there is a next state in Figure7.5. */
        if (TRUE == schedTblCfgRef->SchedTblRepeating)
        {
            Os_WorkSchedTblRepeat(pSchedTblNode, schedTblCfgRef);
        }
        else
        {
            /* If has next schedule table, set state of schedule table to SCHEDULETABLE_RUNNING. */
            if (OS_SCHEDTABLE_INVALID != pSchedTblNode->NextSchedTblId) /* PRQA S 4342 */ /* VL_Os_4342 */
            {
                /*SWS_Os_00505*/
                if (pSchedTblNode->SchedTblState == SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS)
                {
                    Os_STCB[pSchedTblNode->NextSchedTblId]->SchedTblState = SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS;
                }
                else
                {
                    Os_STCB[pSchedTblNode->NextSchedTblId]->SchedTblState = SCHEDULETABLE_RUNNING;
                }
            }

            /* Non-repeating, delete from list. */
            Os_DelSchedTblNode(pSchedTblNode->SchedTblId);
            Os_InitSchedTblCB(pSchedTblNode->SchedTblId);
        }
    }
    else
    {
        Os_WorkSchedTblToNextEp(pSchedTblNode, schedTblCfgRef);
        /* Re-sort by abs tick of next EP. */
        Os_DelSchedTblNode(pSchedTblNode->SchedTblId);
        Os_InsertSchedTblNode(pSchedTblNode->SchedTblId);
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/**
 * The API will call after schedule table synchronization success
 */
#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/* PRQA S 6030 ++ */ /* VL_MTR_Os_STMIF */
OS_LOCAL void Os_WorkSchedTblRunningAndSyncState(
    Os_STCBType *const pSchedTblNode,
    const Os_SchedTblCfgType *schedTblCfgRef)
/* PRQA S 6030 -- */
{
    boolean lastEp = Os_WorkSchedTblEp(pSchedTblNode, schedTblCfgRef);
    if (TRUE == lastEp)
    {
        if (TRUE == schedTblCfgRef->SchedTblRepeating)
        {
            Os_WorkSchedTblRepeat(pSchedTblNode, schedTblCfgRef);
        }
        else
        {
            if (OS_ST_SYNC_EXPLICIT == schedTblCfgRef->SchedTblSync.SchedTblSyncStrategy)
            {
                if (OS_SCHEDTABLE_INVALID != pSchedTblNode->NextSchedTblId) /* PRQA S 4342 */ /* VL_Os_4342 */
                {
                    /*SWS_Os_00505*/
                    if (SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS == pSchedTblNode->SchedTblState)
                    {
                        Os_STCB[pSchedTblNode->NextSchedTblId]->SchedTblState = SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS;
                    }
                    else
                    {
                        Os_STCB[pSchedTblNode->NextSchedTblId]->SchedTblState = SCHEDULETABLE_RUNNING;
                    }
                }
            }
            else if (OS_ST_SYNC_IMPLICIT == schedTblCfgRef->SchedTblSync.SchedTblSyncStrategy)
            {
                if (OS_SCHEDTABLE_INVALID != pSchedTblNode->NextSchedTblId) /* PRQA S 4342 */ /* VL_Os_4342 */
                {
                    Os_STCB[pSchedTblNode->NextSchedTblId]->SchedTblState = SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS;
                }
            }
            else
            {
                /* nothing to do. */
            }
            /* Non-repeating, delete from list. */
            Os_DelSchedTblNode(pSchedTblNode->SchedTblId);
            Os_InitSchedTblCB(pSchedTblNode->SchedTblId);
        }
    }
    else
    {
        Os_WorkSchedTblToNextEp(pSchedTblNode, schedTblCfgRef);
        /* Re-sort by abs tick of next EP. */
        Os_DelSchedTblNode(pSchedTblNode->SchedTblId);
        Os_InsertSchedTblNode(pSchedTblNode->SchedTblId);
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

/* ScheduleTable Sync. */
#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Sync ScheduleTable: Proc waitting state.
 */
OS_LOCAL void Os_SchedTblSyncInWaittingState(
    Os_TickType syncVal,
    Os_TickType counterCurVal,
    Os_STCBType *const pStcb,
    const Os_SchedTblCfgType *schedTblCfgRef)
{
    /* Save start Abs tick: Now + (Duration - syncVal). */
    pStcb->SchedTblStartAbsTick = Os_CalcAbsTicks(counterCurVal,
                                                  (schedTblCfgRef->SchedTblDuration - syncVal),
                                                  schedTblCfgRef->SchedTblCounterRef);
    pStcb->SchedTblNextEpAbsTick = Os_CalcAbsTicks(pStcb->SchedTblStartAbsTick,
                                                   schedTblCfgRef->SchedTblEP[0].SchedTblEPOffset,
                                                   schedTblCfgRef->SchedTblCounterRef);
    /* Set state to SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS. */
    pStcb->SchedTblState = SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS;

    /* Insert to list. */
    Os_InsertSchedTblNode(pStcb->SchedTblId);

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * The API can get difference value when schedule table need Sync
 */
OS_LOCAL void Os_SchedTblSyncGetDiffVal(
    Os_TickType syncVal,
    Os_TickType counterCurVal,
    Os_STCBType *const pStcb,
    const Os_SchedTblCfgType *schedTblCfgRef)
{
    Os_TickType diffVal = 0u;
    /* Position on Tbl. */
    Os_TickType tickOnSchedTbl = Os_GetDistance(counterCurVal,
                                                pStcb->SchedTblNextEpAbsTick,
                                                schedTblCfgRef->SchedTblCounterRef);

    if (pStcb->SchedTblNextEP->SchedTblEPOffset >= tickOnSchedTbl)
    {
        tickOnSchedTbl = pStcb->SchedTblNextEP->SchedTblEPOffset - tickOnSchedTbl;
    }
    else
    {
        tickOnSchedTbl = (pStcb->SchedTblNextEP->SchedTblEPOffset - tickOnSchedTbl) + schedTblCfgRef->SchedTblDuration - 1u; /* PRQA S 2911 */ /* VL_Os_2911 */
    }

    /* Deviation: added. */
    if (tickOnSchedTbl >= syncVal)
    {
        diffVal = tickOnSchedTbl - syncVal;

        pStcb->SchedTblIsAdjustAdd = TRUE;
    }
    else /* Deviation: subtracted. */
    {
        diffVal = syncVal - tickOnSchedTbl;

        pStcb->SchedTblIsAdjustAdd = FALSE;
    }

    pStcb->SchedTblDiff = diffVal;

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Sync ScheduleTable: Proc running state.
 */
OS_LOCAL void Os_SchedTblSyncInRunningState(
    Os_TickType syncVal,
    Os_TickType counterCurVal,
    Os_STCBType *const pStcb,
    const Os_SchedTblCfgType *schedTblCfgRef)
{
    /* Get diff value. */
    Os_SchedTblSyncGetDiffVal(syncVal, counterCurVal, pStcb, schedTblCfgRef);

    /* No need sync. */
    if (pStcb->SchedTblDiff <= schedTblCfgRef->SchedTblSync.SchedTblExplicitPrecision)
    {
        pStcb->SchedTblState = SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS;
        pStcb->SchedTblIsAdjust = FALSE;
    }
    else
    {
        /* Need sync. */
        pStcb->SchedTblIsAdjust = TRUE;

        /* OS362 */
        pStcb->SchedTblIsStopAdjust = FALSE;

        /* Re-sort. */
        Os_DelSchedTblNode(pStcb->SchedTblId);
        Os_InsertSchedTblNode(pStcb->SchedTblId);
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Sync ScheduleTable: Proc running and sync state.
 */
OS_LOCAL void Os_SchedTblSyncInRunningAndSyncState(
    Os_TickType syncVal,
    Os_TickType counterCurVal,
    Os_STCBType *const pStcb,
    const Os_SchedTblCfgType *schedTblCfgRef)
{
    /* Get diff value. */
    Os_SchedTblSyncGetDiffVal(syncVal, counterCurVal, pStcb, schedTblCfgRef);

    /* No need sync. */
    if (pStcb->SchedTblDiff <= schedTblCfgRef->SchedTblSync.SchedTblExplicitPrecision)
    {
        pStcb->SchedTblIsAdjust = FALSE;
    }
    else
    {
        /* Need sync. */
        pStcb->SchedTblIsAdjust = TRUE;
        /* OS362 */
        pStcb->SchedTblIsStopAdjust = FALSE;
        pStcb->SchedTblState = SCHEDULETABLE_RUNNING;

        /* Re-sort. */
        Os_DelSchedTblNode(pStcb->SchedTblId);
        Os_InsertSchedTblNode(pStcb->SchedTblId);
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Process each EP.
 */
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
OS_LOCAL void Os_SchedTblEpProc(
    Os_SCBType *pScb,
    Os_STCBType *pSchedTblListHead,
    const Os_CounterCfgType *counterCfgRef)
/* PRQA S 3673 -- */
{
/* Update  SysRunningAppId according to SchedID. */
#if (CFG_OSAPPLICATION_MAX > 0U)
    pScb->SysRunningAppId = Os_GetObjectAppID(OS_OBJECT_SCHEDULETABLE, (AppObjectId)pSchedTblListHead->SchedTblId);
#endif

    const Os_SchedTblCfgType *schedTblCfgRef = &Os_SchedTblCfg[pSchedTblListHead->SchedTblId];

    /* EP process by different state. */
    switch (pSchedTblListHead->SchedTblState)
    {
    case SCHEDULETABLE_RUNNING:
        Os_WorkSchedTblRunningState(pSchedTblListHead, schedTblCfgRef, counterCfgRef);
        break;

#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
    case SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS:
        Os_WorkSchedTblRunningAndSyncState(pSchedTblListHead, schedTblCfgRef);
        break;
#endif

    /*add comments to pass QAC.*/
    default:
        /* Nothing to do. */
        break;
    }

    UNUSED_PARAMETER(pScb);
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Os_WorkSchedTbl.
 */
void Os_WorkSchedTbl(Os_CounterType counterId) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    OS_HAL_DECLARE_CRITICAL();
    OS_HAL_ENTRY_CRITICAL();

    Os_CCBType *pCcb = Os_CCB[counterId]; /* PRQA S 3678 */ /* VL_Os_3678 */
    if (NULL_PTR == pCcb->CounterStListHead)
    {
        /*nothing to do*/
        OS_HAL_EXIT_CRITICAL();
    }
    else
    {
        Os_SCBType *pScb = Os_GetCurrentContext();
        Os_STCBType *pSchedTblListHead = pCcb->CounterStListHead;
        const Os_CounterCfgType *counterCfgRef = &Os_CounterCfg[counterId];

/* Process each SchedTbl(EP) which is reach the offset. */
#if (CFG_OSAPPLICATION_MAX > 0U)
        Os_ApplicationType bakAppId = pScb->SysRunningAppId;
#endif

        /* More then one EP may have same tick value. */
        while (NULL_PTR != pSchedTblListHead)
        {
            if (Os_GetDistance(pSchedTblListHead->SchedTblNextEpAbsTick,
                               pCcb->CounterCurVal,
                               counterId) > counterCfgRef->CounterMaxAllowedValue)
            {
                break;
            }
            /* Process each EP */
            Os_SchedTblEpProc(pScb, pSchedTblListHead, counterCfgRef);
            /* Update ST List head */
            pSchedTblListHead = pCcb->CounterStListHead;
        }
        OS_HAL_EXIT_CRITICAL();

#if (CFG_OSAPPLICATION_MAX > 0U)
        pScb->SysRunningAppId = bakAppId;
#endif
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Internal implementation of OS service:GetScheduleTableStatus
 */
OS_LOCAL void Os_GetScheduleTableStatus(
    ScheduleTableType schedTblId,
    ScheduleTableStatusRefType schedTblStatus)
{
    Os_STCBType *pStcb = Os_STCB[schedTblId]; /* PRQA S 3678 */ /* VL_Os_3678 */
    /* Return state of schedTblId. */
    *schedTblStatus = pStcb->SchedTblState;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Status check for the relative start of ScheduleTable.
 */
OS_LOCAL StatusType Os_StartRelStatusCheck(
    ScheduleTableType schedTblId,
    TickType offset)
{
    StatusType err = E_OK;
    const Os_SchedTblCfgType *schedTblCfgRef = &Os_SchedTblCfg[schedTblId];

/* OS452 */
#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
    if (OS_ST_SYNC_IMPLICIT == schedTblCfgRef->SchedTblSync.SchedTblSyncStrategy)
    {
        err = E_OS_ID;
    }
    else
#endif
    {
/* OS332 + OS276 */
#if (OS_STATUS_EXTENDED == CFG_STATUS)
        if ((offset > (Os_CounterCfg[schedTblCfgRef->SchedTblCounterRef].CounterMaxAllowedValue -
                       schedTblCfgRef->SchedTblEP[0].SchedTblEPOffset)) ||
            (0u == offset))
        {
            err = E_OS_VALUE;
        }
        else
#endif
        {
            /* OS277 */
            if (SCHEDULETABLE_STOPPED != Os_STCB[schedTblId]->SchedTblState)
            {
                err = E_OS_STATE;
            }
        }
    }

    UNUSED_PARAMETER(offset);
    UNUSED_PARAMETER(schedTblCfgRef);
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Internal implementation of OS service:StartScheduleTableRel.
 */
OS_LOCAL StatusType Os_StartScheduleTableRel(
    ScheduleTableType schedTblId,
    TickType offset)
{
    StatusType err = Os_StartRelStatusCheck(schedTblId, offset);

    if ((StatusType)E_OK == err)
    {
        const Os_SchedTblCfgType *schedTblCfgRef = &Os_SchedTblCfg[schedTblId];

        OS_HAL_DECLARE_CRITICAL();
        OS_HAL_ENTRY_CRITICAL();
        /* Save abs start tick. */
        Os_TickType startAbsTick = Os_CalcAbsTicks(Os_CCB[schedTblCfgRef->SchedTblCounterRef]->CounterCurVal,
                                                   offset,
                                                   schedTblCfgRef->SchedTblCounterRef);

        Os_SchedTblInnerStart(startAbsTick, Os_STCB[schedTblId], schedTblCfgRef);

        /* Insert ScheduleTable to list. */
        Os_InsertSchedTblNode(schedTblId);
        OS_HAL_EXIT_CRITICAL();
    }

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Internal implementation of OS service:StartScheduleTableAbs.
 */
OS_LOCAL StatusType Os_StartScheduleTableAbs(
    ScheduleTableType schedTblId,
    TickType start)
{
    StatusType err = E_OK;
    Os_STCBType *pStcb = NULL_PTR;
    const Os_SchedTblCfgType *schedTblCfgRef = &Os_SchedTblCfg[schedTblId];

/* OS349 */
#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (start > Os_CounterCfg[schedTblCfgRef->SchedTblCounterRef].CounterMaxAllowedValue)
    {
        err = E_OS_VALUE;
    }
    else
#endif
    {
        pStcb = Os_STCB[schedTblId];

        /* OS350 */
        if (SCHEDULETABLE_STOPPED != pStcb->SchedTblState)
        {
            err = E_OS_STATE;
        }
    }

    if ((StatusType)E_OK == err)
    {
        OS_HAL_DECLARE_CRITICAL();
        OS_HAL_ENTRY_CRITICAL();
        /* Save abs start tick. */
        Os_SchedTblInnerStart(start, pStcb, schedTblCfgRef);

        /* Insert ScheduleTable to list. */
        Os_InsertSchedTblNode(schedTblId);
        OS_HAL_EXIT_CRITICAL();
    }

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Internal implementation of OS service:StopScheduleTable.
 */
StatusType Os_StopScheduleTable(ScheduleTableType schedTblId) /* PRQA S 1505 */ /* VL_Os_1505 */
{
    StatusType err = E_OK;
    Os_STCBType *pStcb = Os_STCB[schedTblId]; /* PRQA S 3678 */ /* VL_Os_3678 */

    /* OS280 */
    if (SCHEDULETABLE_STOPPED == pStcb->SchedTblState)
    {
        err = E_OS_NOFUNC;
    }

    if ((StatusType)E_OK == err)
    {
        OS_HAL_DECLARE_CRITICAL();
        OS_HAL_ENTRY_CRITICAL();
        /* OS453: Delete next-ScheduleTable releted by schedTblId. */
        if (OS_SCHEDTABLE_INVALID != pStcb->NextSchedTblId) /* PRQA S 4342 */ /* VL_Os_4342 */
        {
            Os_DelSchedTblNode(pStcb->NextSchedTblId);
            Os_InitSchedTblCB(pStcb->NextSchedTblId);
        }

        /* If state is ST_STATE_NEXT, prev-st should be cared. */
        if (OS_SCHEDTABLE_INVALID != pStcb->PrevSchedTblId) /* PRQA S 4342 */ /* VL_Os_4342 */
        {
            Os_STCB[pStcb->PrevSchedTblId]->NextSchedTblId = OS_SCHEDTABLE_INVALID; /* PRQA S 4342 */ /* VL_Os_4342 */
        }

        /* Delete node from list. */
        Os_DelSchedTblNode(schedTblId);

        /* Re-init STCB. Note: in function Os_InitSchedTblCB,
            state will be set to ST_STATE_STOPPED. */
        Os_InitSchedTblCB(schedTblId);
        OS_HAL_EXIT_CRITICAL();
    }

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Internal implementation of OS service:NextScheduleTable.
 */
OS_LOCAL void Os_NextScheduleTable(
    ScheduleTableType schedTblIdFrom,
    ScheduleTableType schedTblIdTo)
{
    Os_STCBType *pStcbFrom = Os_STCB[schedTblIdFrom];
    Os_STCBType *pStcbTo = Os_STCB[schedTblIdTo];
    const Os_SchedTblCfgType *schedTblFromCfgRef = &Os_SchedTblCfg[schedTblIdFrom];
    const Os_SchedTblCfgType *schedTblToCfgRef = &Os_SchedTblCfg[schedTblIdTo];

    OS_HAL_DECLARE_CRITICAL();
    OS_HAL_ENTRY_CRITICAL();

    /* OS284 */
    pStcbTo->SchedTblStartAbsTick = Os_CalcAbsTicks(pStcbFrom->SchedTblStartAbsTick,
                                                    schedTblFromCfgRef->SchedTblDuration,
                                                    schedTblFromCfgRef->SchedTblCounterRef);
    pStcbTo->SchedTblNextEpAbsTick = Os_CalcAbsTicks(pStcbTo->SchedTblStartAbsTick,
                                                     schedTblToCfgRef->SchedTblEP[0].SchedTblEPOffset,
                                                     schedTblToCfgRef->SchedTblCounterRef);
    pStcbTo->SchedTblState = SCHEDULETABLE_NEXT;
    pStcbTo->SchedTblIsStarted = TRUE;

    /* OS324 */
    if (OS_SCHEDTABLE_INVALID != pStcbFrom->NextSchedTblId) /* PRQA S 4342 */ /* VL_Os_4342 */
    {
        Os_DelSchedTblNode(pStcbFrom->NextSchedTblId);
        Os_InitSchedTblCB(pStcbFrom->NextSchedTblId);
    }

    pStcbFrom->NextSchedTblId = schedTblIdTo;
    pStcbTo->PrevSchedTblId = schedTblIdFrom;

    /* OS284: Insert to list. */
    Os_InsertSchedTblNode(schedTblIdTo);

    /* OS363: Do nothing here. */

    /* OS453: in StopScheduleTable. */

    OS_HAL_EXIT_CRITICAL();
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Internal implementation of OS service:StartScheduleTableSynchron.
 */
OS_LOCAL void Os_StartScheduleTableSynchron(
    ScheduleTableType schedTblId)
{
    Os_STCBType *pStcb = Os_STCB[schedTblId];

    OS_HAL_DECLARE_CRITICAL();
    OS_HAL_ENTRY_CRITICAL();
    /* OS389 */
    pStcb->SchedTblState = SCHEDULETABLE_WAITING;
    OS_HAL_EXIT_CRITICAL();
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Internal implementation of OS service:SyncScheduleTable.
 */
OS_LOCAL StatusType Os_SyncScheduleTable(
    ScheduleTableType schedTblId,
    TickType value)
{
    StatusType err = E_OK;
    Os_STCBType *pStcb = Os_STCB[schedTblId];
    const Os_SchedTblCfgType *schedTblCfgRef = &Os_SchedTblCfg[schedTblId];

    OS_HAL_DECLARE_CRITICAL();
    OS_HAL_ENTRY_CRITICAL();
    /* Get counterCB. */
    Os_TickType counterCurVal = Os_CCB[schedTblCfgRef->SchedTblCounterRef]->CounterCurVal;

    /* Proc by different state. */
    switch (pStcb->SchedTblState)
    {
/* OS456. */
#if (OS_STATUS_EXTENDED == CFG_STATUS)
    case SCHEDULETABLE_STOPPED:
#endif

    case SCHEDULETABLE_NEXT:
        err = E_OS_STATE;
        break;

    case SCHEDULETABLE_WAITING:
        Os_SchedTblSyncInWaittingState(value, counterCurVal, pStcb, schedTblCfgRef);
        break;

    case SCHEDULETABLE_RUNNING:
        Os_SchedTblSyncInRunningState(value, counterCurVal, pStcb, schedTblCfgRef);
        break;

    case SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS:
        Os_SchedTblSyncInRunningAndSyncState(value, counterCurVal, pStcb, schedTblCfgRef);
        break;

    default:
        /* Nothing to do. */
        break;
    }
    OS_HAL_EXIT_CRITICAL();

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Internal implementation of OS service:SetScheduleTableAsync.
 */
OS_LOCAL StatusType Os_SetScheduleTableAsync(ScheduleTableType schedTblId)
{
    StatusType err = E_OK;
    Os_STCBType *pStcb = Os_STCB[schedTblId];
    const Os_SchedTblCfgType *schedTblCfgRef = &Os_SchedTblCfg[schedTblId];

    OS_HAL_DECLARE_CRITICAL();
    OS_HAL_ENTRY_CRITICAL();

    /*SWS_Os_00300*/
    if (OS_ST_SYNC_EXPLICIT == schedTblCfgRef->SchedTblSync.SchedTblSyncStrategy)
    {
        pStcb->SchedTblState = SCHEDULETABLE_RUNNING;
        /* OS362 && OS323 */
        pStcb->SchedTblIsStopAdjust = TRUE;

        OS_HAL_EXIT_CRITICAL();
    }
    else /* OS458 */
    {
        OS_HAL_EXIT_CRITICAL();
        err = E_OS_ID;
    }

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * GetScheduleTableStatus.
 */
/* PRQA S 1503, 3006, 3408, 6070, 1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_Os_3408, VL_MTR_Os_STCAL, VL_Os_1512 */
StatusType GetScheduleTableStatus(
    ScheduleTableType ScheduleTableID,
    ScheduleTableStatusRefType ScheduleStatus)
/* PRQA S 1503, 3006, 3408, 6070, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    StatusType err = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_GetScheduleTableStatus);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_GetScheduleTableStatus_Start, ScheduleTableID);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* Check input_pointor null is better. */
    if (NULL_PTR == ScheduleStatus)
    {
        err = E_OS_ILLEGAL_ADDRESS;
    }
    /* OS293 */
    else if (Os_ObjectIDCheck((ObjectType)ScheduleTableID, (uint8)OS_OBJECT_SCHEDULETABLE) != TRUE)
    {
        err = E_OS_ID;
    }
    else
#endif
    {
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        Os_ServicePortParamType SprotParam = {
            .AllowedContext = OS_SERVICEPORT_CHECK_GET_SCHEDULE_TABLE_STATUS,
            .ObjectType = OS_OBJECT_SCHEDULETABLE,
            .ObjectID = ScheduleTableID, /* PRQA S 4424 */ /* VL_Os_4424 */
            .Address = (uint32)ScheduleStatus, /* PRQA S 0306 */ /* VL_Os_0306 */
        };
        err = Os_ServiceProtCheck(pScb, &SprotParam);
        if (err == E_OK)
#endif
        {
            Os_GetScheduleTableStatus(ScheduleTableID, ScheduleStatus);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK) /* PRQA S 2992, 2996 */ /* VL_Os_2992, VL_Os_2996 */
    {
        Os_TraceErrorHook(OSError_Save_GetScheduleTableStatus(ScheduleTableID, ScheduleStatus), /* PRQA S 2880 */ /* VL_Os_2880 */
                          OSServiceId_GetScheduleTableStatus,
                          err, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544, 2812, 1258, 4342, 2995, 2997 ++ */ /* VL_Os_3432, VL_Os_4544, VL_Os_2812, VL_Os_1258, VL_Os_4342, VL_Os_2995, VL_Os_2997 */
    OSRtiExitApi(pScb, OSApiId_GetScheduleTableStatus);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_GetScheduleTableStatus_Return, (err == E_OK ) ? *ScheduleStatus : (ScheduleTableStatusType)0U);
    UNUSED_PARAMETER(pScb);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/  
    /* PRQA S 3432, 4544, 2812, 1258, 4342, 2995, 2997 -- */
    /* PRQA S 3138, 3141 -- */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Start schedule table in Rel mode.
 */
#if (OS_AUTOSAR_CORES > 1)
OS_LOCAL StatusType Os_RpcAction_StartScheduleTableRel(uint32 *inPara) /* PRQA S 3673 */ /* VL_QAC_3673 */
{
    /* PRQA S 4342 ++ */ /* VL_Os_4342 */
    return Os_StartScheduleTableRel((ScheduleTableType)inPara[0],
                                    (TickType)inPara[1]);
    /* PRQA S 4342 -- */
}

OS_LOCAL StatusType Os_RpcCall_StartScheduleTableRel(
    Os_CoreIdType ownerCore,
    ScheduleTableType schedTblId,
    TickType offset)
{
    StatusType err = E_OK;
    Os_RpcInputType rpcData = {
        .RpcSync = OS_RPC_SYNC,
        .RemoteCoreId = ownerCore,
        .ActionFn = Os_RpcAction_StartScheduleTableRel, /* PRQA S 0674 */ /* VL_Os_0674 */
        .InPara[0] = (uint32)schedTblId,
        .InPara[1] = (uint32)offset, /* PRQA S 0691 */ /* VL_Os_0691 */
    }; /* PRQA S 0704 */ /* VL_Os_0704 */

    err = Os_RpcCallService(&rpcData);
    return err;
}
#endif

/* PRQA S 1503, 3006, 3408, 6070, 1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_Os_3408, VL_MTR_Os_STCAL, VL_Os_1512 */
StatusType StartScheduleTableRel(
    ScheduleTableType ScheduleTableID,
    TickType Offset)
/* PRQA S 1503, 3006, 3408, 6070, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    StatusType err = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_StartScheduleTableRel);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_StartScheduleTableRel_Start, ScheduleTableID);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* OS275 */
    if (Os_ObjectIDCheck((ObjectType)ScheduleTableID, (uint8)OS_OBJECT_SCHEDULETABLE) != TRUE)
    {
        err = E_OS_ID;
    }
    else
#endif
    {
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        Os_ServicePortParamType SprotParam = {
            .AllowedContext = OS_SERVICEPORT_CHECK_START_ST_REL,
            .ObjectType = OS_OBJECT_SCHEDULETABLE,
            .ObjectID = ScheduleTableID, /* PRQA S 4424 */ /* VL_Os_4424 */
            .Address = NULL_PARA, /* PRQA S 1258 */ /* VL_Os_1258 */
        };
        err = Os_ServiceProtCheck(pScb, &SprotParam);
        if (err == E_OK)
#endif
        {
/* SWS_Os_00645:The API call "tartScheduleTableRel" shall
 * be able to start schedule tables of OS-Applications
 * residing on other cores. */
#if (OS_AUTOSAR_CORES > 1)
            Os_CoreIdType coreId = OS_SCHEDTBL_GET_COREID(ScheduleTableID);
            if (coreId != pScb->SysCore)
            {
                err = Os_RpcCall_StartScheduleTableRel(coreId, ScheduleTableID, Offset);
            }
            else
#endif
            {
                err = Os_StartScheduleTableRel(ScheduleTableID, Offset);
            }
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_StartScheduleTableRel(ScheduleTableID, Offset),
                          OSServiceId_StartScheduleTableRel, err, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_StartScheduleTableRel);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_StartScheduleTableRel_Return, err);
    UNUSED_PARAMETER(pScb);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Start schedule table in Abs mode.
 */
#if (OS_AUTOSAR_CORES > 1)
OS_LOCAL StatusType Os_RpcAction_StartScheduleTableAbs(uint32 *inPara) /* PRQA S 3673 */ /* VL_QAC_3673 */
{
    /* PRQA S 4342 ++ */ /* VL_Os_4342 */
    return Os_StartScheduleTableAbs((ScheduleTableType)inPara[0],
                                    (TickType)inPara[1]);
    /* PRQA S 4342 -- */
}

OS_LOCAL StatusType Os_RpcCall_StartScheduleTableAbs(
    Os_CoreIdType ownerCore,
    ScheduleTableType schedTblId,
    TickType start)
{
    StatusType err = E_OK;
    Os_RpcInputType rpcData = {
        .RpcSync = OS_RPC_SYNC,
        .RemoteCoreId = ownerCore,
        .ActionFn = Os_RpcAction_StartScheduleTableAbs, /* PRQA S 0674 */ /* VL_Os_0674 */
        .InPara[0] = (uint32)schedTblId,
        .InPara[1] = (uint32)start, /* PRQA S 0691 */ /* VL_Os_0691 */
    }; /* PRQA S 0704 */ /* VL_Os_0704 */

    err = Os_RpcCallService(&rpcData);
    return err;
}
#endif

/* PRQA S 1503, 3006, 3408, 6070, 1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_Os_3408, VL_MTR_Os_STCAL, VL_Os_1512 */
StatusType StartScheduleTableAbs(ScheduleTableType ScheduleTableID, TickType Start)
/* PRQA S 1503, 3006, 3408, 6070, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    StatusType err = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_StartScheduleTableAbs);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_StartScheduleTableAbs_Start, ScheduleTableID);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* OS348 */
    if (Os_ObjectIDCheck((ObjectType)ScheduleTableID, (uint8)OS_OBJECT_SCHEDULETABLE) != TRUE)
    {
        err = E_OS_ID;
    }
    else
#endif
    {
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        Os_ServicePortParamType SprotParam = {
            .AllowedContext = OS_SERVICEPORT_CHECK_START_ST_ABS,
            .ObjectType = OS_OBJECT_SCHEDULETABLE,
            .ObjectID = ScheduleTableID, /* PRQA S 4424 */ /* VL_Os_4424 */
            .Address = NULL_PARA, /* PRQA S 1258 */ /* VL_Os_1258 */
        };
        err = Os_ServiceProtCheck(pScb, &SprotParam);
        if (err == E_OK)
#endif
        {
/* SWS_Os_00644:The API call "tartScheduleTableAbs"
 * shall be able to start schedule tables of
 * OS-Applications residing on other cores. */
#if (OS_AUTOSAR_CORES > 1)
            Os_CoreIdType coreId = OS_SCHEDTBL_GET_COREID(ScheduleTableID);
            if (coreId != pScb->SysCore)
            {
                err = Os_RpcCall_StartScheduleTableAbs(coreId, ScheduleTableID, Start);
            }
            else
#endif
            {
                err = Os_StartScheduleTableAbs(ScheduleTableID, Start);
            }
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_StartScheduleTableAbs(ScheduleTableID, Start),
                          OSServiceId_StartScheduleTableAbs, err, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_StartScheduleTableAbs);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_StartScheduleTableAbs_Return, err);
    UNUSED_PARAMETER(pScb);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Start schedule table in Abs mode.
 */
#if (OS_AUTOSAR_CORES > 1)
OS_LOCAL StatusType Os_RpcAction_StopScheduleTable(uint32 *inPara) /* PRQA S 3673 */ /* VL_QAC_3673 */
{
    return Os_StopScheduleTable((ScheduleTableType)inPara[0]); /* PRQA S 4342 */ /* VL_Os_4342 */
}

OS_LOCAL StatusType Os_RpcCall_StopScheduleTable(
    Os_CoreIdType ownerCore,
    ScheduleTableType schedTblId)
{
    StatusType err = E_OK;
    Os_RpcInputType rpcData = {
        .RpcSync = OS_RPC_SYNC,
        .RemoteCoreId = ownerCore,
        .ActionFn = Os_RpcAction_StopScheduleTable, /* PRQA S 0674 */ /* VL_Os_0674 */
        .InPara[0] = (uint32)schedTblId, /* PRQA S 0691 */ /* VL_Os_0691 */
    }; /* PRQA S 0704 */ /* VL_Os_0704 */

    err = Os_RpcCallService(&rpcData);
    return err;
}
#endif

/* PRQA S 1503, 3006, 3408, 6070, 1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_Os_3408, VL_MTR_Os_STCAL, VL_Os_1512 */
StatusType StopScheduleTable(ScheduleTableType ScheduleTableID)
/* PRQA S 1503, 3006, 3408, 6070, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    StatusType err = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_StopScheduleTable);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_StopScheduleTable_Start, ScheduleTableID);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* OS279 */
    if (Os_ObjectIDCheck((ObjectType)ScheduleTableID, (uint8)OS_OBJECT_SCHEDULETABLE) != TRUE)
    {
        err = E_OS_ID;
    }
    else
#endif
    {
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        Os_ServicePortParamType SprotParam = {
            .AllowedContext = OS_SERVICEPORT_CHECK_STOP_SCHEDULE_TABLE,
            .ObjectType = OS_OBJECT_SCHEDULETABLE,
            .ObjectID = ScheduleTableID, /* PRQA S 4424 */ /* VL_Os_4424 */
            .Address = NULL_PARA, /* PRQA S 1258 */ /* VL_Os_1258 */
        };
        err = Os_ServiceProtCheck(pScb, &SprotParam);
        if (err == E_OK)
#endif
        {
/* SWS_Os_00646:The API call StopScheduleTable
 * shall be able to stop schedule tables of
 * OS-Applications residing on other cores.  */
#if (OS_AUTOSAR_CORES > 1)
            Os_CoreIdType coreId = OS_SCHEDTBL_GET_COREID(ScheduleTableID);
            if (coreId != pScb->SysCore)
            {
                err = Os_RpcCall_StopScheduleTable(coreId, ScheduleTableID);
            }
            else
#endif
            {
                err = Os_StopScheduleTable(ScheduleTableID);
            }
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_StopScheduleTable(ScheduleTableID),
                          OSServiceId_StopScheduleTable, err, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_StopScheduleTable);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_StopScheduleTable_Return, err);
    UNUSED_PARAMETER(pScb);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Status check when next ScheduleTable.
 */
OS_LOCAL StatusType Os_NextSTCheckStatus(
    ScheduleTableType schedTblIdFrom,
    ScheduleTableType schedTblIdTo)
{
    StatusType err = E_OK;
    const Os_SchedTblCfgType *schedTblFromCfgRef = &Os_SchedTblCfg[schedTblIdFrom];

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    const Os_SchedTblCfgType *schedTblToCfgRef = &Os_SchedTblCfg[schedTblIdTo];
    if (schedTblFromCfgRef->SchedTblCounterRef != schedTblToCfgRef->SchedTblCounterRef)
    {
        err = E_OS_ID; /* OS330 */
    }
    else
    {
#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
        if (schedTblFromCfgRef->SchedTblSync.SchedTblSyncStrategy != schedTblToCfgRef->SchedTblSync.SchedTblSyncStrategy)
        {
            err = E_OS_ID; /* OS484 */
        }
#endif
    }
#endif

    if ((StatusType)E_OK == err) /* PRQA S 2991, 2995 */ /* VL_Os_2991, VL_Os_2995 */
    {
        Os_STCBType *pStcbFrom = Os_STCB[schedTblIdFrom]; /* PRQA S 3678 */ /* VL_Os_3678 */
        Os_STCBType *pStcbTo = Os_STCB[schedTblIdTo]; /* PRQA S 3678 */ /* VL_Os_3678 */

        if ((SCHEDULETABLE_STOPPED == pStcbFrom->SchedTblState) || (SCHEDULETABLE_NEXT == pStcbFrom->SchedTblState))
        {
            err = E_OS_NOFUNC; /* OS283 */
        }
        else if (SCHEDULETABLE_STOPPED != pStcbTo->SchedTblState)
        {
            err = E_OS_STATE; /* OS309 */
        }
        /* If ScheduleTable From is repeating, The functionality is not support. */
        else if (TRUE == schedTblFromCfgRef->SchedTblRepeating)
        {
            err = E_OS_STATE;
        }
        else
        {
            /*nothing to do*/
        }
    }

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Next ScheduleTable Process.
 */
/* PRQA S 1503, 3006, 3408, 6030, 6010, 6070, 1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_Os_3408, VL_MTR_Os_STMIF, VL_MTR_Os_STCYC, VL_MTR_Os_STCAL, VL_Os_1512 */
StatusType NextScheduleTable(
    ScheduleTableType ScheduleTableID_From,
    ScheduleTableType ScheduleTableID_To)
/* PRQA S 1503, 3006, 3408, 6030, 6010, 6070, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    StatusType err = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_NextScheduleTable);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_NextScheduleTable_Start, ScheduleTableID_To);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    Os_ServicePortParamType SprotParam = {
        .AllowedContext = OS_SERVICEPORT_CHECK_STOP_SCHEDULE_TABLE,
        .ObjectType = OS_OBJECT_SCHEDULETABLE,
        .ObjectID = ScheduleTableID_From, /* PRQA S 4424 */ /* VL_Os_4424 */
        .Address = NULL_PARA, /* PRQA S 1258 */ /* VL_Os_1258 */
    };
#endif

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if ((Os_ObjectIDCheck((ObjectType)ScheduleTableID_From, (uint8)OS_OBJECT_SCHEDULETABLE) != TRUE) ||
        (Os_ObjectIDCheck((ObjectType)ScheduleTableID_To, (uint8)OS_OBJECT_SCHEDULETABLE) != TRUE))
    {
        err = E_OS_ID;
    }
    else
#endif
        if ((pScb->SysCore != OS_SCHEDTBL_GET_COREID(ScheduleTableID_From)) ||
            (pScb->SysCore != OS_SCHEDTBL_GET_COREID(ScheduleTableID_To)))
        {
            err = E_OS_CORE;
        }
        else
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
            if (Os_CheckObjAcs(pScb, OS_OBJECT_SCHEDULETABLE, (Os_AppObjectId)ScheduleTableID_To) != TRUE)
        {
            err = E_OS_ACCESS;
        }
        else if ((err = Os_ServiceProtCheck(pScb, &SprotParam)) == E_OK) /* PRQA S 3326, 2004 */ /* VL_Os_3326, VL_Os_2004 */
#endif
        {
            err = Os_NextSTCheckStatus(ScheduleTableID_From, ScheduleTableID_To);
            if ((StatusType)E_OK == err)
            {
                Os_NextScheduleTable(ScheduleTableID_From, ScheduleTableID_To);
            }
        }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
        /* PRQA S 4424 ++ */ /* VL_Os_4424 */
        Os_TraceErrorHook(OSError_Save_NextScheduleTable(ScheduleTableID_From, ScheduleTableID_To), 
                          OSServiceId_NextScheduleTable, err, pScb);
        /* PRQA S 4424 -- */
        /* PRQA S 3138, 3141 -- */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_NextScheduleTable);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_NextScheduleTable_Return, err);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/* Sync. */
#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Status check for synchronized start of ScheduleTable.
 */
OS_LOCAL StatusType Os_StartSyncStatusCheck(
    ScheduleTableType schedTblId)
{
    StatusType err = E_OK;

    /* OS387 */
    if (OS_ST_SYNC_EXPLICIT != Os_SchedTblCfg[schedTblId].SchedTblSync.SchedTblSyncStrategy)
    {
        err = E_OS_ID;
    }
    /* OS388 */
    else if (SCHEDULETABLE_STOPPED != Os_STCB[schedTblId]->SchedTblState)
    {
        err = E_OS_STATE;
    }
    else
    {
        /*nothing to do*/
    }

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * StartScheduleTableSynchron.
 */
/* PRQA S 1503, 3006, 3408, 6030, 6070, 1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_Os_3408, VL_MTR_Os_STMIF, VL_MTR_Os_STCAL, VL_Os_1512 */
StatusType StartScheduleTableSynchron(
    ScheduleTableType ScheduleTableID)
/* PRQA S 1503, 3006, 3408, 6030, 6070, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    StatusType err = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_StartScheduleTableSynchron);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_StartScheduleTableSynchron_Start, ScheduleTableID);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (Os_ObjectIDCheck((ObjectType)ScheduleTableID, (uint8)OS_OBJECT_SCHEDULETABLE) != TRUE)
    {
        err = E_OS_ID;
    }
    else
#endif
        /* Cross core use shall be not supported. */
        if (pScb->SysCore != OS_SCHEDTBL_GET_COREID(ScheduleTableID))
        {
            err = E_OS_CORE;
        }
        else
        {
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
            Os_ServicePortParamType SprotParam = {
                .AllowedContext = OS_SERVICEPORT_CHECK_START_ST_SYN,
                .ObjectType = OS_OBJECT_SCHEDULETABLE,
                .ObjectID = ScheduleTableID, /* PRQA S 4424 */ /* VL_Os_4424 */
                .Address = NULL_PARA, /* PRQA S 1258 */ /* VL_Os_1258 */
            };
            err = Os_ServiceProtCheck(pScb, &SprotParam);
            if (err == E_OK)
#endif
            {
                err = Os_StartSyncStatusCheck(ScheduleTableID);
                if (err == E_OK)
                {
                    Os_StartScheduleTableSynchron(ScheduleTableID);
                }
            }
        }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_StartScheduleTableSynchron(ScheduleTableID),
                          OSServiceId_StartScheduleTableSynchron, err, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_StartScheduleTableSynchron);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_StartScheduleTableSynchron_Return, err);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Status check during ScheduleTable synchronization.
 */
OS_LOCAL StatusType Os_SyncStatusCheck(
    ScheduleTableType schedTblId,
    TickType value)
{
    StatusType err = E_OK;

/* OS454. */
#if (OS_STATUS_EXTENDED == CFG_STATUS)
    const Os_SchedTblCfgType *schedTblCfgRef = &Os_SchedTblCfg[schedTblId];
    if (OS_ST_SYNC_EXPLICIT != schedTblCfgRef->SchedTblSync.SchedTblSyncStrategy)
    {
        err = E_OS_ID;
    }
    /* OS455. */
    else if (value >= schedTblCfgRef->SchedTblDuration)
    {
        err = E_OS_VALUE;
    }
    else
    {
        /*nothing to do*/
    }
#endif

    UNUSED_PARAMETER(schedTblId);
    UNUSED_PARAMETER(value);
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Sync ScheduleTable.
 */
/* PRQA S 1503, 3006, 3408, 6030, 6070, 1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_Os_3408, VL_MTR_Os_STMIF, VL_MTR_Os_STCAL, VL_Os_1512 */
StatusType SyncScheduleTable(ScheduleTableType ScheduleTableID, TickType Value)
/* PRQA S 1503, 3006, 3408, 6030, 6070, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    StatusType err = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_SyncScheduleTable);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_SyncScheduleTable_Start, ScheduleTableID);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (Os_ObjectIDCheck((ObjectType)ScheduleTableID, (uint8)OS_OBJECT_SCHEDULETABLE) != TRUE)
    {
        err = E_OS_ID;
    }
    else
#endif
        /* Cross core use shall be not supported. */
        if (pScb->SysCore != OS_SCHEDTBL_GET_COREID(ScheduleTableID))
        {
            err = E_OS_CORE;
        }
        else
        {
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
            Os_ServicePortParamType SprotParam = {
                .AllowedContext = OS_SERVICEPORT_CHECK_SYNC_SCHEDULE_TABLE,
                .ObjectType = OS_OBJECT_SCHEDULETABLE,
                .ObjectID = ScheduleTableID, /* PRQA S 4424 */ /* VL_Os_4424 */
                .Address = NULL_PARA, /* PRQA S 1258 */ /* VL_Os_1258 */
            };
            err = Os_ServiceProtCheck(pScb, &SprotParam);
            if (E_OK == err)
#endif
            {
                err = Os_SyncStatusCheck(ScheduleTableID, Value);
                if ((StatusType)E_OK == err) /* PRQA S 2991, 2995 */ /* VL_Os_2991, VL_Os_2995 */
                {
                    err = Os_SyncScheduleTable(ScheduleTableID, Value);
                }
            }
        }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_SyncScheduleTable(ScheduleTableID, Value),
                          OSServiceId_SyncScheduleTable, err, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_SyncScheduleTable);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_SyncScheduleTable_Return, err);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * SetScheduleTableAsync.
 */
/* PRQA S 1503, 3006, 3408, 6030, 6010, 6070, 1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_Os_3408, VL_MTR_Os_STMIF, VL_MTR_Os_STCYC, VL_MTR_Os_STCAL, VL_Os_1512 */
StatusType SetScheduleTableAsync(ScheduleTableType ScheduleTableID)
/* PRQA S 1503, 3006, 3408, 6030, 6010, 6070, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    StatusType err = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_SetScheduleTableAsync);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_SetScheduleTableAsync_Start, ScheduleTableID);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (Os_ObjectIDCheck((ObjectType)ScheduleTableID, (uint8)OS_OBJECT_SCHEDULETABLE) != TRUE)
    {
        err = E_OS_ID;
    }
    else
#endif
        if (pScb->SysCore != OS_SCHEDTBL_GET_COREID(ScheduleTableID))
        {
            err = E_OS_CORE;
        }
        else
        {
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
            Os_ServicePortParamType SprotParam = {
                .AllowedContext = OS_SERVICEPORT_CHECK_SET_SCHEDULE_TABLE_ASYNC,
                .ObjectType = OS_OBJECT_SCHEDULETABLE,
                .ObjectID = ScheduleTableID, /* PRQA S 4424 */ /* VL_Os_4424 */
                .Address = NULL_PARA, /* PRQA S 1258 */ /* VL_Os_1258 */
            };
            err = Os_ServiceProtCheck(pScb, &SprotParam);
            if (E_OK == err)
#endif
            {
                Os_STCBType *pStcb = Os_STCB[ScheduleTableID]; /* PRQA S 3678 */ /* VL_Os_3678 */
                if ((SCHEDULETABLE_STOPPED == pStcb->SchedTblState) || (SCHEDULETABLE_NEXT == pStcb->SchedTblState) || (SCHEDULETABLE_WAITING == pStcb->SchedTblState))
                {
                    err = E_OS_STATE; /* OS483 */
                }

                if ((StatusType)E_OK == err)
                {
                    err = Os_SetScheduleTableAsync(ScheduleTableID);
                }
            }
        }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_SetScheduleTableAsync(ScheduleTableID),
                          OSServiceId_SetScheduleTableAsync, err, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_SetScheduleTableAsync);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_SetScheduleTableAsync_Return, err);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#endif

/*=======[E N D   O F   F I L E]==============================================*/
/* PRQA S 0553 EOF */ /* VL_QAC_UnUsedFiles */
