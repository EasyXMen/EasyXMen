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
**  FILENAME    : CanIf_Can.h                                                 **
**                                                                            **
**  Created on  : 2022/10/14 11:30:42                                         **
**  Author      : xiaojian.liang                                              **
**  Vendor      :                                                             **
**  DESCRIPTION : Can callback declaration supplied by CANIF                  **
**                                                                            **
**  SPECIFICATION(S) : AUTOSAR Classic Platform 4.2.2 and R19-11              **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/* Refer to CanIf.h */

#ifndef CANIF_CAN_H
#define CANIF_CAN_H

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
/*************************************************************************/
/*
 * Brief               This service confirms a previously successfully processed transmission of a CAN TxPDU.
 * ServiceId           0x13
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      CanTxPduId: L-PDU handle of CAN L-PDU successfully transmitted.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Can driver
 */
/*************************************************************************/
extern FUNC(void, CANIF_CODE) CanIf_TxConfirmation(PduIdType CanTxPduId);

/*************************************************************************/
/*
 * Brief               This service indicates a successful reception of a received CAN Rx LPDU
 *                     to the CanIf after passing all filters and validation checks.
 * ServiceId           0x14
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Mailbox: Identifies the HRH and its corresponding CAN Controller
 *                     PduInfoPtr: Pointer to the received L-PDU
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Can driver
 */
/*************************************************************************/
extern FUNC(void, CANIF_CODE) CanIf_RxIndication(
    P2CONST(Can_HwType, AUTOMATIC, CANIF_APPL_DATA) Mailbox,
    P2CONST(PduInfoType, AUTOMATIC, CANIF_APPL_DATA) PduInfoPtr);
/*************************************************************************/
/*
 * Brief               This service indicates a Controller BusOff event referring to
 *                     the corresponding CAN Controller with the abstract CanIf ControllerId.
 * ServiceId           0x16
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ControllerId: Abstract CanIf ControllerId which is assigned to a CAN
 *                     controller, where a BusOff occured.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Can driver
 */
/*************************************************************************/
extern FUNC(void, CANIF_CODE) CanIf_ControllerBusOff(uint8 ControllerId);

/*************************************************************************/
/*
 * Brief               This service indicates a controller state transition referring
 *                     to the corresponding CAN controller with the abstract CanIf ControllerId.
 * ServiceId           0x17
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ControllerId: Abstract CanIf ControllerId which is assigned to a
 *                     CAN controller, which state has been transitioned.
 *                     ControllerMode: Mode to which the CAN controller transitioned
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Can driver
 */
/*************************************************************************/
extern FUNC(void, CANIF_CODE)
    CanIf_ControllerModeIndication(uint8 ControllerId, Can_ControllerStateType ControllerMode);

#if (STD_ON == CANIF_PUBLIC_ICOM_SUPPORT)
/*************************************************************************/
/*
 * Brief               This service shall inform about the change of the Icom Configuration of
 *                     a CAN controller using the abstract CanIf ControllerId.
 * ServiceId           0x26
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant only for different controller Ids
 * Param-Name[in]      ControllerId: Abstract CanIf ControllerId which is assigned to a CAN
 *                     controller, which informs about the Configuration Id.
 *                     ConfigurationId: Active Configuration Id.
 *                     Error:
 *                     ICOM_SWITCH_E_OK: No Error
 *                     ICOM_SWITCH_E_FAILED: Switch to requested Configuration failed. Severe Error.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        CANIF_PUBLIC_ICOM_SUPPORT is TRUE
 * CallByAPI           Can driver
 */
/*************************************************************************/
extern FUNC(void, CANIF_CODE)
    CanIf_CurrentIcomConfiguration(uint8 ControllerId, IcomConfigIdType ConfigurationId, IcomSwitch_ErrorType Error);
#endif /*STD_ON == CANIF_PUBLIC_ICOM_SUPPORT*/

#endif /* CANIF_CAN_H */
/*******************************************************************************
**                      End of file                                           **
*******************************************************************************/
