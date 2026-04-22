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
 **  @file               : CanIf_Can.h
 **  @author             : xiaojian.liang
 **  @date               : 2022/10/14
 **  @vendor             : isoft
 **  @description        : CanIf-supplied Can callback header file
 **
 ***********************************************************************************************************************/

#ifndef CANIF_CAN_H_
#define CANIF_CAN_H_

/* =================================================== inclusions =================================================== */
#include "CanIf_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
#if (STD_ON == CANIF_TRIGGER_TRANSMIT_SUPPORT)
/**
 * @brief           Within this API, the upper layer module (called module) shall check whether
 *                  the available data fits into the buffer size reported by PduInfoPtr->SduLength.
 *                  If it fits, it shall copy its data into the buffer provided by PduInfoPtr->SduDataPtr
 *                  and update the length of the actual copied data in PduInfoPtr->SduLength.
 *                  If not, it returns E_NOT_OK without changing PduInfoPtr.
 * @param[in]       TxPduId: ID of the SDU that is requested to be transmitted.
 * @param[inout]    PduInfoPtr: Contains a pointer to a buffer (SduDataPtr) to where the SDU
 *                  data shall be copied, and the available buffer size in SduLengh. On return,
 *                  the service will indicate the length of the copied SDU data in SduLength.
 * @return          Std_ReturnType
 * @retval          E_OK: SDU has been copied and SduLength indicates the number of copied bytes.
 * @retval          E_NOT_OK: No SDU data has been copied. PduInfoPtr must not be used since
 *                  it may contain a NULL pointer or point to invalid data.
 * @synchronous     TRUE
 * @reentrant       Reentrant for different PduIds. Non reentrant for the same PduId.
 * @trace           CPD-61415
 */
extern Std_ReturnType CanIf_TriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr);
#endif

/**
 * @brief           This service confirms a previously successfully processed transmission of a CAN TxPDU.
 * @param[in]       CanTxPduId: L-PDU handle of CAN L-PDU successfully transmitted.
 * @synchronous     TRUE
 * @reentrant       Reentrant
 * @trace           CPD-61416
 */
extern void CanIf_TxConfirmation(PduIdType CanTxPduId);

/**
 * @brief           This service indicates a successful reception of a received CAN Rx L-PDU to
 *                  the CanIf after passing all filters and validation checks.
 * @param[in]       Mailbox: Identifies the HRH and its corresponding CAN Controller.
 * @param[in]       PduInfoPtr: Pointer to the received L-PDU.
 * @synchronous     TRUE
 * @reentrant       Reentrant
 * @trace           CPD-61417
 */
extern void CanIf_RxIndication(const Can_HwType* Mailbox, const PduInfoType* PduInfoPtr);

/**
 * @brief           This service indicates a Controller BusOff event referring to the corresponding
 *                  CAN Controller with the abstract CanIf ControllerId.
 * @param[in]       ControllerId: Abstract CanIf ControllerId which is assigned to a CAN controller,
 *                  where a BusOff occured.
 * @synchronous     TRUE
 * @reentrant       Reentrant
 * @trace           CPD-61418
 */
extern void CanIf_ControllerBusOff(uint8 ControllerId);

/**
 * @brief           This service indicates a controller state transition referring to the
 *                  corresponding CAN controller with the abstract CanIf ControllerId.
 * @param[in]       ControllerId: Abstract CanIf ControllerId which is assigned to a CAN controller,
 *                  which state has been transitioned.
 * @param[in]       ControllerMode: Mode to which the CAN controller transitioned.
 * @synchronous     TRUE
 * @reentrant       Reentrant
 * @trace           CPD-61422
 */
extern void CanIf_ControllerModeIndication(uint8 ControllerId, Can_ControllerStateType ControllerMode);

#ifdef __cplusplus
}
#endif

#endif
