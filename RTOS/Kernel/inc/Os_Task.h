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
 **  @file               : Os_Task.h
 **  @author             : i-soft-os
 **  @date               : 2024/02/10
 **  @vendor             : isoft
 **  @description        : Os header file for task api declarations
 **
 ***********************************************************************************************************************/

#ifndef OS_TASK_H
#define OS_TASK_H

/* =================================================== inclusions =================================================== */
#include "Os_Task_CfgData.h"

/* ===================================================== macros ===================================================== */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/* ========================================= external function declarations ========================================= */
/**
 * @brief              Init the task control block.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_InitTask(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Pre entry highest priority task.
 * @param[in]          coreId: The current core ID.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
/* PRQA S 3449, 3451 ++ */ /* VL_Os_3449, VL_Os_3451 */
extern void Os_SwitchTask(Os_CoreIdType coreId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
/* PRQA S 3449, 3451 --*/

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Internal implementation of OS service:ActivateTask.
 * @param[in]          taskId: The task ID that requires activation.
 * @return             StatusType
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern StatusType Os_ActivateTask(TaskType taskId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Set the task status to ready and prepare for scheduling.
 * @param[in]          taskId: Task Id.
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern void Os_SetTaskToReady(TaskType taskId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Set the task status to Waiting.
 * @param[in]          pScb: Points to the system control block.
 * @param[in]          runningTaskId: The ID of the currently running task.
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern void Os_SetTaskToWait(Os_SCBType* pScb, TaskType runningTaskId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Update the task with the highest priority.
 * @param[in]          pScb: Points to the system control block.
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern void Os_UpdateHighPrioTask(Os_SCBType* pScb);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (OS_AUTOSAR_CORES > 1)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           Makes an RPC call to activate a task on the specified core
 * @param[in]       ownerCore: The target core ID for task activation
 * @param[in]       syncType: Synchronization type for the RPC call
 * @param[in]       taskId: Identifier of the task to be activated
 * @return          StatusType
 * @retval          E_OK: Task activation request was successful
 * @retval          Other: Error codes from Os_RpcCallService
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern StatusType Os_RpcCall_ActivateTask(Os_CoreIdType ownerCore, Os_RpcSyncType syncType, TaskType taskId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif
#endif

/*=======[E N D   O F   F I L E]==============================================*/
