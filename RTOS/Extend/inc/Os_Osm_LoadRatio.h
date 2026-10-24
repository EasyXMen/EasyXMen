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
 **  FILENAME    : Os_Osm_LoadRatio.h                                              **
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
#ifndef OS_LOADRATIO_H
#define OS_LOADRATIO_H

/* =================================================== inclusions =================================================== */
#include "Os_Osm_CfgData.h"

/* ===================================================== macros ===================================================== */

/* ========================================== external function definitions ========================================= */
#if (TRUE == CFG_LOAD_RATIO_CALC_ENABLE) /* PRQA S 3332 */ /* VL_Os_3332 */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief          Initialize the load ratio function module.
 * @param[in]      coreId: The id of core.
 * @reentrant      FALSE
 * @synchronous    TRUE
 * @trace          -
 */
extern void Os_InitLoadRatio(Os_CoreIdType coreId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief          Start the CPU load ratio timing.
 * @param[in]      pScb: The pointer to current core's SCB.
 * @reentrant      FALSE
 * @synchronous    TRUE
 * @trace          -
 */
extern void Os_StartLoadRatio(Os_SCBType* pScb);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief          Calculate the load ratio.
 * @param[in]      curTicks: The current timer ticks.
 * @reentrant      FALSE
 * @synchronous    TRUE
 * @trace          -
 */
extern void Os_CalcLoadRatio(Os_TimerTickType curTicks);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief          Calculate exectution time ticks when entering an task.
 * @param[in]      TaskID: The id of task.
 * @param[in]      curTicks: The current timer ticks.
 * @reentrant      FALSE
 * @synchronous    TRUE
 * @trace          -
 */
extern void Os_EnterTaskRecordTick(TaskType TaskID, Os_TimerTickType curTicks);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief          Calculate exectution time ticks when exiting an task.
 * @param[in]      TaskID: The id of task.
 * @reentrant      FALSE
 * @synchronous    TRUE
 * @trace          -
 */
extern Os_TimerTickType Os_ExitTaskRecordTick(TaskType TaskID);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief          Calculate exectution time ticks when entering an interrupt.
 * @param[in]      ISRID: The id of interrupt.
 * @reentrant      FALSE
 * @synchronous    TRUE
 * @trace          -
 */
extern void Os_EnterIsrRecordTick(ISRType ISRID);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief          Calculate exectution time ticks when exiting an interrupt.
 * @param[in]      ISRID: The id of interrupt.
 * @return         Os_TimerTickType
 * @retval         Return the exectution time ticks when exiting an interrupt.
 * @reentrant      FALSE
 * @synchronous    TRUE
 * @trace          -
 */
extern Os_TimerTickType Os_ExitIsrRecordTick(ISRType ISRID);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#endif
/*=======[E N D   O F   F I L E]==============================================*/
