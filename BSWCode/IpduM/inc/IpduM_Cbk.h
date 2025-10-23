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
 **  @file               : IpduM.h
 **  @author             : darren.zhang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : I-PDU Multiplexer
 **
 ***********************************************************************************************************************/

/* ================================================ misar justifications ============================================ */

#ifndef IPDUM_CBK_H_
#define IPDUM_CBK_H_

/* =================================================== inclusions =================================================== */

#include "IpduM_Internal.h"

#ifdef __cplusplus
extern "C" {
#endif
/* =============================================== version information ============================================== */
/*Published information elements*/
#define IPDUM_CBK_H_AR_RELEASE_MAJOR_VERSION    4u
#define IPDUM_CBK_H_AR_RELEASE_MINOR_VERSION    5u
#define IPDUM_CBK_H_AR_RELEASE_REVISION_VERSION 0u

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

/**
 * @brief         Indication of a received I-PDU from a lower layer communication interface module.
 * @param[in]     RxPduId : ID of the received I-PDU.
 * @param[in]     PduInfoPtr : Contains the length of the received I-PDU and a pointer to a buffer (SduDataPtr)
 * containing the I-PDU.
 * @reentrant     Reentrant for different PduIds. Non reentrant for the same PduId.
 * @synchronous   TRUE
 * @trace         CPD-60207
 */
void IpduM_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr);

/**
 * @brief         The lower layer communication interface module confirms the transmission of an I-PDU.
 * @param[in]     TxPduId : ID of the I-PDU that has been transmitted.
 * @param[in]     result : result of confirmation.
 * @reentrant     Reentrant for different PduIds. Non reentrant for the same PduId.
 * @synchronous   TRUE
 * @trace         CPD-60205
 */
void IpduM_TxConfirmation(PduIdType TxPduId, Std_ReturnType result);

/**
 * @brief         The lower layer communication interface module confirms the transmission of an I-PDU.
 * @param[in]     TxPduId : ID of the I-PDU that has been transmitted.
 * @param[in out] PduInfoPtr : Contains a pointer to a buffer (SduDataPtr) to where the SDU data shall be copied,
 *                and the available buffer size in SduLengh.On return, the service will indicate the length of the
 * copied SDU data in SduLength.
 * @reentrant     Reentrant for different PduIds. Non reentrant for the same PduId.
 * @synchronous   TRUE
 * @trace         CPD-60206
 */
Std_ReturnType IpduM_TriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr);

#ifdef __cplusplus
}
#endif

#endif
