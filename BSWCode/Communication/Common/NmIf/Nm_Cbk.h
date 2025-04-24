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
**  FILENAME    : Nm_Cbk.h                                                    **
**                                                                            **
**  Created on  :                                                             **
**  Author      : Wanglili                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

#ifndef NM_CBK_H_
#define NM_CBK_H_

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Nm_Cfg.h"
#include "ComStack_Types.h"
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

/******************************************************************************/
/*
 * Brief               Notification that a NM-message has been received in the
 *                     Bus-Sleep Mode, what indicates that some nodes in the
 *                     network have already entered the Network Mode.
 * ServiceId           0x11
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      nmNetworkHandle,Identification of the NM-channel
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              NA
 */
/******************************************************************************/
FUNC(void, NM_CODE)
Nm_NetworkStartIndication(NetworkHandleType nmNetworkHandle);

/******************************************************************************/
/*
 * Brief               Notification that the network management has entered Network Mode.
 * ServiceId           0x12
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      nmNetworkHandle,Identification of the NM-channel
 * Param-Name[in/out]  NA
 * Param-Name[out]     NA
 * Return              NA
 */
/******************************************************************************/
FUNC(void, NM_CODE)
Nm_NetworkMode(NetworkHandleType nmNetworkHandle);

/******************************************************************************/
/*
 * Brief               Notification that the network management has entered Bus-Sleep
 *                     Mode.
 * ServiceId           0x14
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      nmNetworkHandle,Identification of the NM-channel
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              NA
 */
/******************************************************************************/
FUNC(void, NM_CODE) Nm_BusSleepMode(NetworkHandleType nmNetworkHandle);

/******************************************************************************/
/*
 * Brief               Notification that the network management has entered Prepare
 *                     Bus-Sleep Mode.
 * ServiceId           0x13
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      nmNetworkHandle,Identification of the NM-channel
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              NA
 */
/******************************************************************************/
FUNC(void, NM_CODE) Nm_PrepareBusSleepMode(NetworkHandleType nmNetworkHandle);

#if (STD_ON == NM_REMOTE_SLEEP_IND_ENABLED)
/******************************************************************************/
/*
 * Brief               Notification that the network management has detected that
 *                     all other nodes on the network are ready to enter Bus-Sleep
 *                     Mode.
 * ServiceId           0x17
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      nmNetworkHandle,Identification of the NM-channel
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              NA
 */
/******************************************************************************/

FUNC(void, NM_CODE)
Nm_RemoteSleepIndication(NetworkHandleType nmNetworkHandle);

/******************************************************************************/
/*
 * Brief               Notification that the network management has detected that
 *                     not all other nodes on the network are longer ready to enter
 *                     Bus-Sleep Mode
 * ServiceId           0x18
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      nmNetworkHandle,Identification of the NM-channel
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              NA
 */
/******************************************************************************/
FUNC(void, NM_CODE) Nm_RemoteSleepCancellation(NetworkHandleType nmNetworkHandle);

/******************************************************************************/
/*
 * Brief               Notification to the NM Coordinator functionality that this
 *                     is a suitable point in time to initiate the coordinated
 *                     shutdown on
 * ServiceId           0x19
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      nmNetworkHandle,Identification of the NM-channel
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              NA
 */
/******************************************************************************/
FUNC(void, NM_CODE)
Nm_SynchronizationPoint(NetworkHandleType nmNetworkHandle);

#endif /* STD_ON ==  NM_REMOTE_SLEEP_IND_ENABLED */

#if (STD_ON == NM_COORDINATOR_SYNC_SUPPORT)

/******************************************************************************/
/*
 * Brief               Sets an indication, when the NM Coordinator Sleep Ready
 *                     bit in the Control Bit Vector is set
 * ServiceId           0x1e
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      nmNetworkHandle,Identification of the NM-channel
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              NA
 */
/******************************************************************************/
FUNC(void, NM_CODE)
Nm_CoordReadyToSleepIndication(NetworkHandleType nmChannelHandle);

/******************************************************************************/
/*
 * Brief               Cancels an indication, when the NM Coordinator Sleep Ready
 *                     bit in the Control Bit Vector is set back to 0
 * ServiceId           0x1f
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      nmNetworkHandle,Identification of the NM-channel
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              NA
 */
/******************************************************************************/
FUNC(void, NM_CODE)
Nm_CoordReadyToSleepCancellation(NetworkHandleType nmChannelHandle);

#endif /* STD_ON ==  NM_COORDINATOR_SYNC_SUPPORT */

/* 8.4.2
Extra Call-back notifications */
#if (STD_ON == NM_PDU_RX_INDICATION_ENABLED)
/******************************************************************************/
/*
 * Brief               Notification that a NM message has been received.
 * ServiceId           0x15
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      nmNetworkHandle,Identification of the NM-channel
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              NA
 */
/******************************************************************************/

FUNC(void, NM_CODE)
Nm_PduRxIndication(NetworkHandleType nmNetworkHandle);

#endif /* STD_ON ==  NM_PDU_RX_INDICATION_ENABLED */

#if (STD_ON == NM_STATE_CHANGE_IND_ENABLED)
/******************************************************************************/
/*
 * Brief               Notification that a NM message has been received.
 * ServiceId           0x16
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      nmNetworkHandle,Identification of the NM-channel
 *                     nmPreviousState,Previous state of the NM-channel
 *                     nmCurrentState,Current (new) state of the NM-channel
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              NA
 */
/******************************************************************************/

FUNC(void, NM_CODE)
Nm_StateChangeNotification(
    NetworkHandleType nmNetworkHandle,
    Nm_StateType nmPreviousState,
    Nm_StateType nmCurrentState);

#endif /* STD_ON == NM_STATE_CHANGE_IND_ENABLED */

/******************************************************************************/
/*
 * Brief               Service to indicate that an NM message with set Repeat
 *                     Message Request Bit has been received
 * ServiceId           0x1a
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      nmNetworkHandle,Identification of the NM-channel
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              NA
 */
/******************************************************************************/

FUNC(void, NM_CODE)
Nm_RepeatMessageIndication(NetworkHandleType nmNetworkHandle);

/******************************************************************************/
/*
 * Brief               Service to indicate that an attempt to send an NM message
 *                     failed.
 * ServiceId           0x1b
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      nmNetworkHandle,Identification of the NM-channel
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              NA
 */
/******************************************************************************/

FUNC(void, NM_CODE)
Nm_TxTimeoutException(NetworkHandleType nmNetworkHandle);

#if (STD_ON == NM_CAR_WAKE_UP_RX_ENABLED)
/******************************************************************************/
/*
 * Brief               This function is called by a <Bus>Nm to indicate reception
 *                     of a CWU request
 * ServiceId           0x1d
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      nmNetworkHandle,Identification of the NM-channel
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              NA
 */
/******************************************************************************/

FUNC(void, NM_CODE)
Nm_CarWakeUpIndication(NetworkHandleType nmChannelHandle);

#endif /* STD_ON == NM_CAR_WAKE_UP_RX_ENABLED */

/******************************************************************************/
/**
 * Brief               Notification that the network management has entered
 * Synchronize Mode.
 * ServiceId           0x21
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      nmNetworkHandle,Identification of the NM-channel
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              NA
 */
/******************************************************************************/

FUNC(void, NM_CODE)
Nm_SynchronizeMode(NetworkHandleType nmNetworkHandle);

#endif /* NM_CBK_H_ */
