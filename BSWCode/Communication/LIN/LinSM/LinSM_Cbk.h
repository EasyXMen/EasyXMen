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
**  FILENAME    : LinSM_Cbk.h                                                 **
**                                                                            **
**  Created on  :                                                             **
**  Author      : HuRongbo                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION : Callback declaration supplied by LinSM                      **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/

#ifndef LinSM_CBK_H
#define LinSM_CBK_H

#include "ComStack_Types.h"
#include "LinSM_Cfg.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/*Published information elements*/
#define LINSM_CBK_H_VENDOR_ID                   (uint16)62
#define LINSM_CBK_H_MODULE_ID                   (uint16)141
#define LINSM_CBK_H_AR_RELEASE_MAJOR_VERSION    (uint8)4
#define LINSM_CBK_H_AR_RELEASE_MINOR_VERSION    (uint8)5
#define LINSM_CBK_H_AR_RELEASE_REVISION_VERSION (uint8)0

#define LINSM_CBK_H_SW_MAJOR_VERSION            (uint8)2
#define LINSM_CBK_H_SW_MINOR_VERSION            (uint8)0
#define LINSM_CBK_H_SW_PATCH_VERSION            (uint8)0

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
#if (LINSM_MASTER_NODE_SUPPORT == STD_ON)

/******************************************************************************/
/*
 * Brief               The LinIf module will call this callback when the new
 *                     requested schedule table is active.
 * ServiceId           0x20
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      network:  Identification of the LIN channel
 *                     schedule: Pointer to the new active Schedule table
 * Param-Name[in/out]  N/A
 * Param-Name[out]     N/A
 * Return              N/A
 */
/******************************************************************************/
FUNC(void, LINSM_CODE)
LinSM_ScheduleRequestConfirmation(NetworkHandleType network, LinIf_SchHandleType schedule);

#endif

#if (LINSM_SLAVE_NODE_SUPPORT == STD_ON)

/******************************************************************************/
/*
 * Brief               The LinIf will call this callback when the go to sleep
 *                     command is received on the network or a bus idle timeout
 *                     occurs.
 * ServiceId           0x03
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      network: Identification of the LIN channel
 * Param-Name[in/out]  N/A
 * Param-Name[out]     N/A
 * Return              N/A
 */
/******************************************************************************/
FUNC(void, LINSM_CODE)
LinSM_GotoSleepIndication(NetworkHandleType network);

#endif

/******************************************************************************/
/*
 * Brief               The LinIf will call this callback when the go to sleep
 *                     command is sent successfully or not sent successfully
 *                     on the network.
 * ServiceId           0x22
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      network: Identification of the LIN channel
 *                     success: True if goto sleep was successfully sent,
 *                              false otherwise
 * Param-Name[in/out]  N/A
 * Param-Name[out]     N/A
 * Return              N/A
 */
/******************************************************************************/
FUNC(void, LINSM_CODE)
LinSM_GotoSleepConfirmation(NetworkHandleType network, boolean success);

/******************************************************************************/
/*
 * Brief               The LinIf will call this callback when the wake up signal
 *                     command is sent not successfully/successfully on the
 *                     network.
 * ServiceId           0x21
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      network: Identification of the LIN channel
 *                     success: True if wakeup was successfully sent,
 *                              false otherwise
 * Param-Name[in/out]  N/A
 * Param-Name[out]     N/A
 * Return              N/A
 */
/******************************************************************************/
FUNC(void, LINSM_CODE)
LinSM_WakeupConfirmation(NetworkHandleType network, boolean success);

#endif /* LINSM_CBK_H */
