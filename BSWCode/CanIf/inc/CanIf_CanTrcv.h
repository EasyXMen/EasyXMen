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
 **  @file               : CanIf_CanTrcv.h
 **  @author             : xiaojian.liang
 **  @date               : 2022/10/14
 **  @vendor             : isoft
 **  @description        : CanIf-supplied CanTrcv callback header file
 **
 ***********************************************************************************************************************/

#ifndef CANIF_CANTRCV_H_
#define CANIF_CANTRCV_H_

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
#if ((STD_ON == CANIF_PUBLIC_PN_SUPPORT) && (STD_ON == CANIF_PUBLIC_TRCV_PN_SUPPORT))
/**
 * @brief           This service indicates that the transceiver is running in PN communication mode
 *                  referring to the corresponding CAN transceiver with the abstract CanIf TransceiverId.
 * @param[in]       TransceiverId: Abstract CanIf TransceiverId, which is assigned to a CAN transceiver,
 *                  which was checked for PN availability.
 * @synchronous     TRUE
 * @reentrant       Reentrant
 * @trace           CPD-61419
 */
extern void CanIf_ConfirmPnAvailability(uint8 TransceiverId);

/**
 * @brief           This service indicates that the transceiver has cleared the WufFlag referring to
 *                  the corresponding CAN transceiver with the abstract CanIf TransceiverId.
 * @param[in]       TransceiverId: Abstract CanIf TransceiverId, which is assigned to a CAN transceiver,
 *                  for which this function was called.
 * @synchronous     TRUE
 * @reentrant       Reentrant
 * @trace           CPD-61420
 */
extern void CanIf_ClearTrcvWufFlagIndication(uint8 TransceiverId);

/**
 * @brief           This service indicates that the check of the transceivers wake-up flag has been
 *                  finished by the corresponding CAN transceiver with the abstract CanIf TransceiverId.
 *                  This indication is used to cope with the asynchronous transceiver communication.
 * @param[in]       TransceiverId: Abstract CanIf TransceiverId, which is assigned to a CAN transceiver,
 *                  for which this function was called.
 * @synchronous     TRUE
 * @reentrant       Reentrant
 * @trace           CPD-61421
 */
extern void CanIf_CheckTrcvWakeFlagIndication(uint8 TransceiverId);
#endif

#if (CANIF_TRCV_NUMBER > 0u)
/**
 * @brief           This service indicates a transceiver state transition referring to the corresponding
 *                  CAN transceiver with the abstract CanIf TransceiverId.
 * @param[in]       TransceiverId: Abstract CanIf TransceiverId, which is assigned to a CAN transceiver,
 *                  which state has been transitioned.
 * @param[in]       TransceiverMode: Mode to which the CAN transceiver transitioned.
 * @synchronous     TRUE
 * @reentrant       Reentrant
 * @trace           CPD-61423
 */
extern void CanIf_TrcvModeIndication(uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode);
#endif

#ifdef __cplusplus
}
#endif

#endif
