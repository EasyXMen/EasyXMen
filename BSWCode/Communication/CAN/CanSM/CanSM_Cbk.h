/* PRQA S 3108++ */
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
 */
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : CanSM_Cbk.h                                                 **
**                                                                            **
**  Created on  :                                                             **
**  Author      : Wanglili                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION : A list of functions provided for other modules              **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      REVISION   HISTORY                                    **
*******************************************************************************/
/* <VERSION>  <DATE>    <AUTHOR>      <REVISION LOG>
 *  V2.0.0    2020-08-17  lili.wang    R19_11 initial version.
 *  V2.0.1    2021-06-03  lili.wang    include head file modify.
 */
#ifndef CANSM_CBK_H
#define CANSM_CBK_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "CanSM_Cfg.h"
#include "CanIf_Types.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/
/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
/**
 * @brief         This callback function notifies the CanSM about a bus-off event on a certain CAN controller, which
 *                needs to be considered with the specified bus-off recovery handling for the impacted CAN network.
 * @id            0x04
 * @synchronous   Synchronous
 * @reentrancy    Reentrant (only for different CanControllers)
 * @param[in]     ControllerId  CAN controller, which detected a bus-off event
 * @remarks       The call context is either on interrupt level (interrupt mode) or on task level (polling mode).
 */
extern void CanSM_ControllerBusOff(uint8 ControllerId);

/**
 * @brief         This callback shall notify the CanSM module about a CAN controller mode change.
 * @id            0x07
 * @synchronous   Synchronous
 * @reentrancy    Reentrant (only for different CAN controllers)
 * @param[in]     ControllerId   CAN controller, whose mode has changed
 * @param[in]     ControllerMode Notified CAN controller mode
 */
extern void CanSM_ControllerModeIndication(uint8 ControllerId, Can_ControllerStateType ControllerMode);

/******************************************************************************/
/*
 * Brief               This callback shall notify the CanSM module about a CAN
 *                     transceiver mode change.
 * ServiceId           0x09
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different CAN Transceivers
 * Param-Name[in]      TransceiverId, CAN transceiver, whose mode has changed
 *                     TransceiverMode, Notified CAN transceiver mode
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              NA
 */
/******************************************************************************/
FUNC(void, CANSM_TRANSCEIVERMODEINDICATION_CODE)
CanSM_TransceiverModeIndication(uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode);

#if (STD_ON == CANSM_PNC_SUPPORT)

/******************************************************************************/
/*
 * Brief               This callback function shall indicate the CanIf_ClearTrcvWufFlag
 *                     API process end for the notified CAN Transceiver.
 * ServiceId           0x08
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different CAN Transceivers
 * Param-Name[in]      Transceiver, Requested Transceiver
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              NA
 */
/******************************************************************************/
FUNC(void, CANSM_CLEARTRCVWUFFLAGINDICATION_CODE)
CanSM_ClearTrcvWufFlagIndication(uint8 Transceiver);

/******************************************************************************/
/*
 * Brief               This callback function indicates the CheckTransceiverWakeFlag
 *                     API process end for the notified CAN Transceiver
 * ServiceId           0x0a
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different CAN Transceivers
 * Param-Name[in]      Transceiver, Requested Transceiver
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              NA
 */
/******************************************************************************/
FUNC(void, CANSM_CHECKTRANSCEIVERWAKEFLAGINDICATION_CODE)
CanSM_CheckTransceiverWakeFlagIndication(uint8 Transceiver);

/******************************************************************************/
/*
 * Brief               This callback function indicates that the transceiver is
 *                     running in PN communication mode
 * ServiceId           0x06
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      TransceiverId, CAN transceiver, which was checked for PN
 *                     availability
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              NA
 */
/******************************************************************************/
FUNC(void, CANSM_CONFIRMPNAVAILABILITY_CODE)
CanSM_ConfirmPnAvailability(uint8 TransceiverId);

#endif /*STD_ON == CANSM_PNC_SUPPORT*/
#endif /* CANSM_CBK_H */
