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
/**
 * @brief SCB and CoreId are obtained by taskId.
 */
#define OS_TASK_GET_COREID(taskId) (Os_TaskCfg[taskId].HostCore) /* PRQA S 3472 */ /* VL_Os_3472 */
#if (TRUE == ENABLE_CLONE_TECH)
#define OS_TASK_GET_SCB(taskId) (&Os_SCB_FA_Inf[0])
#else
#define OS_TASK_GET_SCB(taskId) (Os_SCB_FA_Inf[OS_TASK_GET_COREID(taskId)])
#endif

/**
 * @brief Check if the external taskId on a certain core is valid.
 *
 * Macro parameters:
 *  @param[in] taskId: The external taskId that needs to be checked.
 *  @param[in] coreId: The core to which the checked external task belongs.
 */
/* PRQA S 3472 ++ */ /* VL_Os_3472 */
#define Os_CheckExternalTaskId(taskId, coreId) (            \
    ((taskId) >= (Os_TaskIdRange[coreId].ExtendTask.Start)) && \
    ((taskId) < (Os_TaskIdRange[coreId].ExtendTask.End)))
/* PRQA S 3472 -- */

/**
 * @brief Check if the basic taskId on a certain core is valid.
 *
 * Macro parameters:
 *  @param[in] taskId: The basic taskId that needs to be checked.
 *  @param[in] coreId: The core to which the checked basic task belongs.
 */
#define CheckBasicTaskId(taskId, coreId) (                 \
    ((taskId) >= (Os_TaskIdRange[coreId].BasicTask.Start)) && \
    ((taskId) < (Os_TaskIdRange[coreId].BasicTask.End)))

/**
 * @brief Check if the taskId on a certain core is valid.
 *
 * Macro parameters:
 *  @param[in] taskId: The taskId that needs to be checked.
 *  @param[in] coreId: The core to which the checked task belongs.
 */
/* PRQA S 3472 ++ */ /* VL_Os_3472 */
#define CheckCoreTaskId(taskId, coreId) (                 \
    ((taskId) >= (Os_TaskIdRange[coreId].AllTask.Start)) && \
    ((taskId) < (Os_TaskIdRange[coreId].AllTask.End)))
/* PRQA S 3472 -- */

/* ========================================= external function declarations ========================================= */
/**
 * @brief              Init the task control block.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_InitTask(void);

/**
 * @brief              Pre entry highest priority task.
 * @param[in]          coreId: The current core ID.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_SwitchTask(Os_CoreIdType coreId);

/**
 * @brief              Internal implementation of OS service:ActivateTask.
 * @param[in]          taskId: The task ID that requires activation.
 * @return             StatusType
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern StatusType Os_ActivateTask(TaskType taskId);

/**
 * @brief              Set the task status to ready and prepare for scheduling.
 * @param[in]          taskId: Task Id.
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern void Os_SetTaskToReady(TaskType taskId);

/**
 * @brief              Update the task with the highest priority.
 * @param[in]          pScb: Points to the system control block.
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern void Os_UpdateHighPrioTask(Os_SCBType *pScb);

#if (OS_AUTOSAR_CORES > 1)
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
extern StatusType Os_RpcCall_ActivateTask(
    Os_CoreIdType ownerCore,
    Os_RpcSyncType syncType,
    TaskType taskId);
#endif
#endif

/*=======[E N D   O F   F I L E]==============================================*/
