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
 **  @file               : Os_ReadyQue.h
 **  @author             : i-soft-os
 **  @date               : 2024/02/10
 **  @vendor             : isoft
 **  @description        : Os header file for ReadyQue api declarations
 **
 ***********************************************************************************************************************/

#ifndef OS_READYQUE_H
#define OS_READYQUE_H

/* =================================================== inclusions =================================================== */
#include "Os_ReadyQue_CfgData.h"

/**
 * @brief Priority calculation parameters. 
 */
#define OS_NUM_PRIORITYBITS_PERWORD 16u

/* ========================================== external function definitions ========================================= */
/**
 * @brief              Init priority ready map.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_InitReadyTable(void);

/**
 * @brief              Clear priority ready map.
 * @param[in]          queueMg: Points to queueMg.
 * @param[in]          prio: The priority of the task.
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern void Os_ClearPrioReadyMap(const Os_ReadyQueueManageType *queueMg, Os_PriorityType prio);

/**
 * @brief              get the highest priority.
 * @param[in]          queueMg: Points to queueMg.
 * @return             Os_PriorityType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern Os_PriorityType Os_GetHighPrio(const Os_ReadyQueueManageType *queueMg);

/**
 * @brief              get the first task of a ready queue that the value of
 *                     priority is equal to prio.
 * @param[in]          queueMg: Points to queueMg.
 * @param[in]          prio: The priority of the task.
 * @return             Os_TaskType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern Os_TaskType Os_ReadyQueueGetFirst(const Os_ReadyQueueManageType *queueMg, Os_PriorityType prio);

/**
 * @brief              insert a priority to ready queue.
 * @param[in]          queueMg: Points to queueMg.
 * @param[in]          object: the ID of task or resource.
 * @param[in]          level: the call level.
 * @param[in]          prio: inserted priority.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_ReadyQueueInsert(const Os_ReadyQueueManageType *queueMg,
                                Os_TaskType object,
                                Os_CallLevelType level,
                                Os_PriorityType prio);

/**
 * @brief              remove a priority from ready queue.
 * @param[in]          queueMg: Points to queueMg.
 * @param[in]          prio: removed priority.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_ReadyQueueRemove(const Os_ReadyQueueManageType *queueMg, Os_PriorityType prio);

/**
 * @brief              Removing task_node from ready_queue in TerminateApplication.
 * @param[in]          queueMg: Points to queueMg.
 * @param[in]          taskId: Task Id.
 * @param[in]          queuePriority: Removed priority.
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern void Os_ApplReadyQueueRemove(const Os_ReadyQueueManageType *queueMg,
                                    Os_TaskType taskId,
                                    Os_PriorityType queuePriority);
#endif

/*=======[E N D   O F   F I L E]==============================================*/
