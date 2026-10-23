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
#if (CANTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
#include "Csm.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
#define CANTSYN_START_SEC_CODE_FAST
#include "CanTSyn_MemMap.h"
/**
 * @brief        Indication of a received PDU from a lower layer communication interface module.
 * @param[in]    RxPduId    ID of the received PDU.
 * @param[in]    PduInfoPtr Contains the length (SduLength) of the received PDU, a pointer to a buffer (SduDataPtr)
 *               containing the PDU, and the MetaData related to this PDU.
 * @reentrant    TRUE(Reentrant for different PduIds. Non reentrant for the same PduId.)
 * @synchronous  TRUE
 * @trace        CPD-85826
 */
void CanTSyn_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr);
#define CANTSYN_STOP_SEC_CODE_FAST
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE_FAST
#include "CanTSyn_MemMap.h"
/**
 * @brief        The lower layer communication interface module confirms the transmission of a PDU,
 *               or the failure to transmit a PDU.
 * @param[in]    TxPduId ID of the PDU that has been transmitted.
 * @param[in]    result  E_OK:     The PDU was transmitted.
 *                       E_NOT_OK: Transmission of the PDU failed.
 * @reentrant    TRUE(Reentrant for different PduIds. Non reentrant for the same PduId.)
 * @synchronous  TRUE
 * @trace        CPD-85811
 */
void CanTSyn_TxConfirmation(PduIdType TxPduId, Std_ReturnType result);
#define CANTSYN_STOP_SEC_CODE_FAST
#include "CanTSyn_MemMap.h"

#if (CANTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
#define CANTSYN_START_SEC_CODE_FAST
#include "CanTSyn_MemMap.h"
/**
 * @brief         By this API service the CanTSyn gets an indication and the result of ICV generation.
 * @param[in]     jobId: JobID of the operation that caused the callback.
 * @param[in]     result: Contains the result of the cryptographic operation.
 * @synchronous   TRUE
 * @reentrant     TRUE
 * @trace         CPD-84311
 */
void CanTSyn_IcvGenerationIndication(uint32 jobId, Crypto_ResultType result);
#define CANTSYN_STOP_SEC_CODE_FAST
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE_FAST
#include "CanTSyn_MemMap.h"
/**
 * @brief         By this API service the CanTSyn gets an indication and the result of ICV verification.
 * @param[in]     jobId: JobID of the operation that caused the callback.
 * @param[in]     result: Contains the result of the cryptographic operation.
 * @synchronous   TRUE
 * @reentrant     TRUE
 * @trace         CPD-84312
 */
void CanTSyn_IcvVerificationIndication(uint32 jobId, Crypto_ResultType result);
#define CANTSYN_STOP_SEC_CODE_FAST
#include "CanTSyn_MemMap.h"
#endif
#ifdef __cplusplus
}
#endif

#endif
