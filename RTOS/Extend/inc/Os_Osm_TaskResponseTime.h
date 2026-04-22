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
 ********************************************************************************
 **                                                                            **
 **  FILENAME    : Os_Osm_TaskResponseTime.h                                       **
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
#ifndef OS_TASKRESPONSETIME_H
#define OS_TASKRESPONSETIME_H

/*=======[I N C L U D E S]====================================================*/
#include "Os_Osm_CfgData.h"

/*=======[M A C R O S]========================================================*/
#if (TRUE == CFG_TASK_RESPONSE_TIME_ENABLE) /* PRQA S 3332 */ /* VL_Os_3332 */

#define OS_TASK_RESPONSE_TIME_CALC_NUM (10u)
/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
/**
 * @brief          Initialize the task response time module.
 * @param[in]      coreId: The id of core.
 * @reentrant      FALSE
 * @synchronous    TRUE
 * @trace          -
 */
extern void Os_InitTaskResponseTime(Os_CoreIdType coreId);

/**
 * @brief          Record the start time of the task.
 * @param[in]      TaskID: The id of task.
 * @reentrant      FALSE
 * @synchronous    TRUE
 * @trace          -
 */
extern void Os_TaskRecordStartTick(TaskType TaskID);

/**
 * @brief          Calculate the total duration of the computing task.
 * @param[in]      TaskID: The id of task.
 * @reentrant      FALSE
 * @synchronous    TRUE
 * @trace          -
 */
extern void Os_TaskRecordTotalTick(TaskType TaskID);
#endif

#endif
/*=======[E N D   O F   F I L E]==============================================*/
