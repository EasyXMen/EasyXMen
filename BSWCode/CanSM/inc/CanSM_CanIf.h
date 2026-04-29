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
 **  @file               : CanSM_CanIf.h
 **  @author             : Wanglili
 **  @date               : 2020/08/17
 **  @vendor             : isoft
 **  @description        : A list of functions provided for other modules
 **
 ***********************************************************************************************************************/

/* ================================================ misar justifications ============================================ */

#ifndef CANSM_CANIF_H_
#define CANSM_CANIF_H_

/* =================================================== inclusions =================================================== */
#include "CanSM_Cfg.h"
#include "CanIf_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief       This callback function notifies the CanSM about a bus-off event on a certain CAN controller, which
 *              needs to be considered with the specified bus-off recovery handling for the impacted CAN network.
 * @param[in]   ControllerId   : CAN controller, which detected a bus-off event
 * @reentrant   TRUE (only for different CanControllers)
 * @synchronous TRUE
 * @trace       CPD-70686
 */
extern void CanSM_ControllerBusOff(uint8 ControllerId);

/**
 * @brief       This callback shall notify the CanSM module about a CAN controller mode change.
 * @param[in]   ControllerId   : CAN controller, whose mode has changed
 * @param[in]   ControllerMode   : Notified CAN controller mode
 * @reentrant   TRUE (only for different CanControllers)
 * @synchronous TRUE
 * @trace       CPD-70687
 */
extern void CanSM_ControllerModeIndication(uint8 ControllerId, Can_ControllerStateType ControllerMode);

/**
 * @brief       This callback shall notify the CanSM module about a CAN transceiver mode change
 * @param[in]   TransceiverId   : CAN transceiver, whose mode has changed
 * @param[in]   TransceiverMode   : Notified CAN transceiver mode
 * @reentrant   TRUE (only for different CAN Transceivers)
 * @synchronous TRUE
 * @trace       CPD-70688
 */
extern void CanSM_TransceiverModeIndication(uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode);

/**
 * @brief       This function shall notify the CanSM module, that the CanNm has detected for the affected
 *              partial CAN network a tx timeout exception, which shall be recovered within the respective
 *              network state machine of the CanSM module
 * @param[in]   Channel   : Affected CAN network
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70689
 */
extern void CanSM_TxTimeoutException(NetworkHandleType Channel);

#if CANSM_PNC_SUPPORT == STD_ON
/**
 * @brief       This callback function shall indicate the CanIf_ClearTrcvWufFlag API process end for the
 *              notified CAN Transceiver.
 * @param[in]   Transceiver   : Requested Transceiver
 * @reentrant   TRUE (only for different CAN Transceivers)
 * @synchronous TRUE
 * @trace       CPD-70690
 */
extern void CanSM_ClearTrcvWufFlagIndication(uint8 Transceiver);

/**
 * @brief       This callback function indicates the CanIf_CheckTrcvWakeFlag API process end for the notified
 *              CAN Transceiver.
 * @param[in]   Transceiver   : Requested Transceiver
 * @reentrant   TRUE (only for different CAN Transceivers)
 * @synchronous TRUE
 * @trace       CPD-70691
 */
extern void CanSM_CheckTransceiverWakeFlagIndication(uint8 Transceiver);

/**
 * @brief       This callback function indicates that the transceiver is running in PN communication mode
 * @param[in]   TransceiverId   : CAN transceiver, which was checked for PN availability
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70692
 */
extern void CanSM_ConfirmPnAvailability(uint8 TransceiverId);

#endif

#ifdef __cplusplus
}
#endif
#endif
