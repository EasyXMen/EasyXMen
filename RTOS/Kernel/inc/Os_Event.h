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
 ************************************************************************************************************************
 **
 **  @file               : Os_Event.h
 **  @author             : i-soft-os
 **  @date               : 2024/02/10
 **  @vendor             : isoft
 **  @description        : Os header file for event api declarations
 **
 ***********************************************************************************************************************/

#ifndef OS_EVENT_H
#define OS_EVENT_H

/* =================================================== inclusions =================================================== */
#include "Os_Event_CfgData.h"

/* ===================================================== macros ===================================================== */
#define OS_EVENT_32_MSAK_SIZE (0xFFFFFFFFU)

/**
 * @brief Check if event is configured with access permissions.
 *
 * Macro parameters:
 *  @param[in] eventMask: The event being checked.
 *  @param[in] accessMask: event with permissions.
 */
/* PRQA S 3472 ++ */ /* VL_Os_3472 */
#define Os_EventPermissionCheck(eventMask, accessMask) \
    Os_EventBitCompare(eventMask, accessMask)

/**
 * @brief Check that all masks passed on event are set.
 *
 * Macro parameters:
 *  @param[in] eventMask: Waiting event.
 *  @param[in] setEvent: Events that have been set.
 */
#define Os_EventIsAllEventsTriggered(eventMask, setEvent) \
    Os_EventBitCompare(eventMask, setEvent)
/* PRQA S 3472 -- */

/* ========================================== external function definitions ========================================= */
#if (CFG_EXTENDED_TASK_MAX > 0)
/**
 * @brief         Init the Event control block.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         -
 */
extern void Os_InitEvent(void);

/**
 * @brief              The service may be called from an interrupt service routine and
 *                     from the task level, but not from hook routines.
 *                     The events of task <taskId> are set according to the event
 *                     mask <mask>. Calling SetEvent causes the task <taskId> to
 *                     be transferred to the ready state, if it was waiting for at least
 *                     one of the events specified in <mask>
 * @param[in]          taskId: Task ID.
 * @param[in]          mask: Event mask.
 * @return             StatusType
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern StatusType Os_SetEvent(TaskType taskId, EventMaskType mask);

#if (OS_AUTOSAR_CORES > 1)
/**
 * @brief           Makes a remote procedure call to set events on another core
 * @param[in]       ownerCore: Target core ID where the task resides
 * @param[in]       syncType: Synchronization type (sync or async)
 * @param[in]       taskId: ID of the task to set events for
 * @param[in]       mask: Event mask to set
 * @return          StatusType
 * @retval          E_OK: Remote call successful
 * @retval          Other: Error from remote call execution
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern StatusType Os_RpcCall_SetEvent(
    Os_CoreIdType ownerCore,
    Os_RpcSyncType syncType,
    TaskType taskId,
    EventMaskType mask);
#endif

/* ========================================== internal function definitions ========================================= */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Clear the Event control block.
 * @param[in]          taskId: Task ID.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE void Os_ClearECB(Os_TaskType taskId)
{
    Os_ECB[taskId]->SetEventMask = 0U;
    Os_ECB[taskId]->WaitEventMask = 0U;
    Os_ECB[taskId]->IsWaitAllEvents = FALSE;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#endif

/*=======[E N D   O F   F I L E]============================================*/
