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
**  FILENAME    : CanIf_CanTrcv.h                                             **
**                                                                            **
**  Created on  : 2022/10/14 11:30:42                                         **
**  Author      : xiaojian.liang                                              **
**  Vendor      :                                                             **
**  DESCRIPTION : CanTrcv callback declaration supplied by CANIF              **
**                                                                            **
**  SPECIFICATION(S) : AUTOSAR Classic Platform 4.2.2 and R19-11              **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/* Refer to CanIf.h */

#ifndef CANIF_CANTRCV_H
#define CANIF_CANTRCV_H
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "CanIf_Types.h"

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/* Published information */
#if !defined(CANIF_PUBLISHED_INFORMATION)
#define CANIF_PUBLISHED_INFORMATION
#define CANIF_MODULE_ID                   60u
#define CANIF_VENDOR_ID                   62u
#define CANIF_AR_RELEASE_MAJOR_VERSION    4u
#define CANIF_AR_RELEASE_MINOR_VERSION    5u
#define CANIF_AR_RELEASE_REVISION_VERSION 0u
#define CANIF_SW_MAJOR_VERSION            2u
#define CANIF_SW_MINOR_VERSION            2u
#define CANIF_SW_PATCH_VERSION            3u
#elif ((CANIF_SW_MAJOR_VERSION != 2u) || (CANIF_SW_MINOR_VERSION != 2u) || (CANIF_SW_PATCH_VERSION != 3u))
#error "CanIf: Mismatch in Software Version"
#endif

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
#if (STD_ON == CANIF_PUBLIC_PN_SUPPORT)
/*************************************************************************/
/*
 * Brief               This service indicates that the transceiver is running in PN
 *                     communication mode referring to the corresponding CAN transceiver
 *                     with the abstract CanIf TransceiverId.
 * ServiceId           0x1a
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      TransceiverId: Abstract CanIf TransceiverId, which is assigned to
 *                     a CAN transceiver, which was checked for PN availability.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        CANIF_PUBLIC_PN_SUPPORT is TRUE
 * CallByAPI           CanTrcv driver
 */
/*************************************************************************/
extern FUNC(void, CANIF_CODE) CanIf_ConfirmPnAvailability(uint8 TransceiverId);

/*************************************************************************/
/*
 * Brief               This service indicates that the transceiver has cleared the WufFlag
 *                     referring to the corresponding CAN transceiver with the abstract CanIf TransceiverId.
 * ServiceId           0x20
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      TransceiverId: Abstract CanIf TransceiverId, which is assigned to a CAN transceiver,
 *                     for which this function was called.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        CANIF_PUBLIC_PN_SUPPORT is TRUE
 * CallByAPI           CanTrcv driver
 */
/*************************************************************************/
extern FUNC(void, CANIF_CODE) CanIf_ClearTrcvWufFlagIndication(uint8 TransceiverId);

/*************************************************************************/
/*
 * Brief               This service indicates that the check of the transceiver��s wake-up flag
 *                     has been finished by the corresponding CAN transceiver with the abstract
 *                     CanIf TransceiverId. This indication is used to cope with the asynchronous
 *                     transceiver communication.
 * ServiceId           0x21
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      TransceiverId: Abstract CanIf TransceiverId, which is assigned to a CAN
 *                     transceiver, for which this function was called.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        CANIF_PUBLIC_PN_SUPPORT is TRUE
 * CallByAPI           CanTrcv driver
 */
/*************************************************************************/
extern FUNC(void, CANIF_CODE) CanIf_CheckTrcvWakeFlagIndication(uint8 TransceiverId);
#endif /*STD_ON == CANIF_PUBLIC_PN_SUPPORT*/

#if (CANIF_TRCV_NUMBER > 0u)
/*************************************************************************/
/*
 * Brief               This service indicates a transceiver state transition referring
 *                     to the corresponding CAN transceiver with the abstract CanIf TransceiverId.
 * ServiceId           0x22
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      TransceiverId: Abstract CanIf TransceiverId, which is assigned to a
 *                     CAN transceiver, which state has been transitioned.
 *                     TransceiverMode Mode to which the CAN transceiver transitioned
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        CANIF_TRCV_NUMBER > 0
 * CallByAPI           CanTrcv driver
 */
/*************************************************************************/
extern FUNC(void, CANIF_CODE) CanIf_TrcvModeIndication(uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode);
#endif /*CANIF_TRCV_NUMBER > 0u*/
#endif /* CANIF_CANTRCV_H */
/*******************************************************************************
**                      End of file                                           **
*******************************************************************************/
