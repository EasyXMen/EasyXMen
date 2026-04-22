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
 **  @file               : Os_Alarm.h
 **  @author             : i-soft-os
 **  @date               : 2024/02/10
 **  @vendor             : isoft
 **  @description        : Os header file for alarm api declarations
 **
 ***********************************************************************************************************************/

#ifndef OS_ALARM_H
#define OS_ALARM_H

/* =================================================== inclusions =================================================== */
#include "Os_Alarm_CfgData.h"

/* ===================================================== macros ===================================================== */
#if (CFG_ALARM_MAX > 0)
/**
 * @brief SCB and CoreId are obtained by alarmId.
 */
/* PRQA S 3472 ++ */ /* VL_Os_3472 */
#define OS_ALARM_GET_COREID(alarmId) (Os_AlarmCfg[alarmId].HostCore)
/* PRQA S 3472 -- */

#if (TRUE == ENABLE_CLONE_TECH)
#define OS_ALARM_GET_SCB(alarmId) (&Os_SCB_FA_Inf[0])
#else
#define OS_ALARM_GET_SCB(alarmId) (Os_SCB_FA_Inf[OS_ALARM_GET_COREID(alarmId)])
#endif

/* ========================================= external function declarations ========================================= */
/**
 * @brief         Init the alarm control block.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         -
 */
extern void Os_InitAlarm(void);

/**
 * @brief              Process the incoming counterId; if the trigger time of the Alarm is reached,
 *                     perform the corresponding Alarm handling.
 * @param[in]          counterId: Indicates the counterId that needs to be processed.
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern void Os_WorkAlarm(Os_CounterType counterId);

/**
 * @brief              Internal implementation of OS service: CancelAlarm
 * @param[in]          alarmId: AlarmId.
 * @return             StatusType
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern StatusType Os_CancelAlarm(AlarmType alarmId);

#endif
#endif

/*=======[E N D   O F   F I L E]============================================*/
