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
 **  @file               : DoIP_Internal_TCP_Non_Diagnostic.h
 **  @author             : hao.wen
 **  @date               : 2024/09/08
 **  @vendor             : isoft
 **  @description        : Implement the non-diagnostic part of TCP
 **
 ***********************************************************************************************************************/

#ifndef DOIP_INTERNAL_TCP_NON_DIAGNOSTIC_H_
#define DOIP_INTERNAL_TCP_NON_DIAGNOSTIC_H_

/* =================================================== inclusions =================================================== */

#include "DoIP_Internal_TCP.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================= external function declarations ========================================= */

/* connect */

/**
 * @brief       Handle routing activation request when close tcp connection.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
void DoIP_HandleRaWhenCloseTcpConnection(void);

/* pending */

/**
 * @brief       Pending non diagnostic message.
 * @param[in]   soadTxPduRef: The connection to send non message.
 * @param[in]   pduInfoPtr:  Provides the destination buffer (SduDataPtr) and the number of bytes to be copied
 * (SduLength). .
 * @param[out]  queue: Save non diagnostic message.
 * @return      Std_ReturnType
 * @retval      E_OK: Suspended message successfully.
 * @retval      E_NOT_OK: Queue is full.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
Std_ReturnType
    DoIP_EnqueueNonDiagMsg(DoIP_NonDiagQueueType* queue, PduIdType soadTxPduRef, const PduInfoType* pduInfoPtr);

/**
 * @brief       Handle non diagnostic message in queued.
 * @param[in]   ctx: send/recive context.
 * @return      boolean
 * @retval      TRUE: Suspended non diagnostic message was successfully sent.
 * @retval      FALSE: Add the non diagnostic message back to the queue.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
boolean DoIP_HandlePendingNonDiagMsg(DoIP_TcpTxRxContextType* ctx);

/* rcv */

/**
 * @brief       Handle tcp non diagnostic message.
 * @param[in]   soadTxPduRef: The connection that received the message.
 * @param[in]   txRxCtxIdx: index of DoIP_TcpTxRxContext.
 * @param[in]   payloadType: message type.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
void DoIP_HandleTcpNonDiagMsg(PduIdType soadTxPduRef, uint8 txRxCtxIdx, uint16 payloadType);

#ifdef __cplusplus
}
#endif
#endif
