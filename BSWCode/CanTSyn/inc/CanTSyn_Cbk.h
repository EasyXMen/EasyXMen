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
 **  @file               : CanTSyn_Cbk.h
 **  @author             : xiongfei.shi
 **  @date               : 2024/12/13
 **  @vendor             : isoft
 **  @description        : Call-back function declarations
 **
 ***********************************************************************************************************************/

#ifndef CANTSYN_CBK_H_
#define CANTSYN_CBK_H_

/* =================================================== inclusions =================================================== */
#include "ComStack_Types.h"
#include "CanTSyn_Cfg.h"

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
 * @brief        Indication of a received PDU from a lower layer communication interface module.
 * @param[in]    RxPduId    ID of the received PDU.
 * @param[in]    PduInfoPtr Contains the length (SduLength) of the received PDU, a pointer to a buffer (SduDataPtr)
 *               containing the PDU, and the MetaData related to this PDU.
 * @reentrant    TRUE(Reentrant for different PduIds. Non reentrant for the same PduId.)
 * @synchronous  TRUE
 * @trace        CPD-75629
 */
void CanTSyn_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr);

/**
 * @brief        The lower layer communication interface module confirms the transmission of a PDU,
 *               or the failure to transmit a PDU.
 * @param[in]    TxPduId ID of the PDU that has been transmitted.
 * @param[in]    result  E_OK:     The PDU was transmitted.
 *                       E_NOT_OK: Transmission of the PDU failed.
 * @reentrant    TRUE(Reentrant for different PduIds. Non reentrant for the same PduId.)
 * @synchronous  TRUE
 * @trace        CPD-75628
 */
void CanTSyn_TxConfirmation(PduIdType TxPduId, Std_ReturnType result);

#ifdef __cplusplus
}
#endif

#endif
