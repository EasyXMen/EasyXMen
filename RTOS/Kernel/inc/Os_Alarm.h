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

/* ========================================= external function declarations ========================================= */
#if (CFG_ALARM_MAX > 0)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief         Init the alarm control block.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         -
 */
extern void Os_InitAlarm(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Process the incoming counterId; if the trigger time of the Alarm is reached,
 *                     perform the corresponding Alarm handling.
 * @param[in]          counterId: Indicates the counterId that needs to be processed.
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern void Os_WorkAlarm(Os_CounterType counterId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Internal implementation of OS service: CancelAlarm
 * @param[in]          alarmId: AlarmId.
 * @return             StatusType
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern StatusType Os_CancelAlarm(AlarmType alarmId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif
#endif

/*=======[E N D   O F   F I L E]============================================*/
