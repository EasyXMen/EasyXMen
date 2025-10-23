/**
 * Copyright (C) 2008-2025 isoft Infrastructure Software Co., Ltd.
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
 **  @file               : LinIf_Master.h
 **  @author             : HuRongbo
 **  @date               :
 **  @vendor             : isoft
 **  @description        : LinIf master implementation header file
 **
 ***********************************************************************************************************************/

#ifndef LINIF_MASTER_H_
#define LINIF_MASTER_H_

/* =================================================== inclusions =================================================== */
#include "LinIf.h"
#include "LinIf_Internal.h"

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================= external function declarations ========================================= */

#if (STD_ON == LINIF_MASTER_SUPPORT)
/**
 * @brief               Initializes the LinIf master channel.
 * @param[in]           applicationId: The partition ID of LinIf.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
void LinIf_MasterInit(ApplicationType applicationId);

#if (LINIF_MASTER_SPORADIC_FRAME_SUPPORT == STD_ON)
/**
 * @brief               Transmit request process in master node.
 * @param[in]           ch: LinIf channel id
 * @param[in]           frameIdx: LinIf master channel frame index
 * @return              E_OK: Send master transmit request success
 * @return              E_NOT_OK: Send master transmit request failure
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
Std_ReturnType LinIf_MasterTransmit(NetworkHandleType ch, uint16 frameIdx);
#endif

/**
 * @brief               Schedule change request process in master node.
 * @param[in]           ch: LinIf channel id
 * @param[in]           Schedule: LinIf master channel schedule table index
 * @return              E_OK: Change schedule request success
 * @return              E_NOT_OK: Change schedule request failure
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
Std_ReturnType LinIf_MasterScheduleRequest(NetworkHandleType ch, LinIf_SchHandleType LinIfSchedule);

/**
 * @brief               Go-to-sleep process in master node.
 * @param[in]           ch: LinIf channel id
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
void LinIf_MasterGotoSleep(NetworkHandleType ch);

/**
 * @brief               Process sleep state transmit
 * @param[inout]        masterChRtDataPtr: LinIf master channel runtime data
 * @param[in]           ch: LinIf channel id
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
void LinIf_SleepProcess(LinIf_MasterRuntimeType* masterChRtDataPtr, uint8 ch);

/**
 * @brief               Process wake-up state transmit
 * @param[in]           ch: LinIf channel id
 * @return              E_OK: Wakeup success
 * @return              E_NOT_OK: Wakeup failure
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
Std_ReturnType LinIf_MasterWakeUp(NetworkHandleType ch);

/**
 * @brief               Wakeup confirmation process in master node
 * @param[in]           WakeupSource: LinIf master channel wakeup source
 * @param[in]           applicationId: The partition id of LinIf
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
void LinIf_MasterWakeupConfirmation(EcuM_WakeupSourceType WakeupSource, ApplicationType applicationId);

/**
 * @brief               Process wake-up state transmit
 * @param[inout]        masterChRtDataPtr: LinIf master channel runtime data
 * @param[in]           ch: LinIf channel id
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
void LinIf_WakeUpProcess(LinIf_MasterRuntimeType* masterChRtDataPtr, uint8 ch);

/**
 * @brief               Process the preview frame transmission
 * @param[inout]        masterChRtDataPtr: LinIf master channel runtime data
 * @param[in]           ch: LinIf channel id
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
void LinIf_PrevTransmit(LinIf_MasterRuntimeType* masterChRtDataPtr, uint8 ch);

/**
 * @brief               Process the next frame transmission
 * @param[inout]        masterChRtDataPtr: LinIf master channel runtime data
 * @param[in]           ch: LinIf channel id
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
void LinIf_NextTransmit(LinIf_MasterRuntimeType* masterChRtDataPtr, uint8 ch);

/**
 * @brief               Updates the schedule strategy
 * @param[inout]        masterChRtDataPtr: LinIf master channel runtime data
 * @param[in]           ch: LinIf channel id
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
void LinIf_UpdateSchedule(LinIf_MasterRuntimeType* masterChRtDataPtr, uint8 ch);

/**
 * @brief               Get current frame type
 * @param[in]           ch: LinIf channel id
 * @param[in]           frameType: LinIf frame type
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
Std_ReturnType LinIf_GetCurFrameType(NetworkHandleType ch, LinIf_FrameTypeType* frameType);

#endif

#endif
