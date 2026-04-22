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
 **  @file               : DoIP_Internal_TCP_Diagnostic.h
 **  @author             : hao.wen
 **  @date               : 2024/11/01
 **  @vendor             : isoft
 **  @description        : Implement the diagnostic part of TCP
 **
 ***********************************************************************************************************************/

#ifndef DOIP_INTERNAL_TCP_DIAGNOSTIC_H_
#define DOIP_INTERNAL_TCP_DIAGNOSTIC_H_

/* =================================================== inclusions =================================================== */

#include "DoIP_Internal_TCP.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================= external function declarations ========================================= */

/* connect */
/* PRQA S 1512 EOF */ /* VL_DoIP_1512 */

/**
 * @brief       Notify PdurR when close tcp connection.
 * @param[in]   txRxCtxIdx: index of DoIP_TcpTxRxContext.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
void DoIP_NotifyPduRWhenCloseTcpConnection(uint8 txRxCtxIdx);

/* pending */

/**
 * @brief       Handle Tp message in queued.
 * @param[in]   ctx: send/recive context.
 * @return      boolean
 * @retval      TRUE: Suspended TP message was successfully sent.
 * @retval      FALSE: Add the TP message back to the queue.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
boolean DoIP_HandlePendingTpMsg(DoIP_TcpTxRxContextType* ctx);

/**
 * @brief       Handle IF message in queued.
 * @param[in]   ctx: send/recive context.
 * @return      boolean
 * @retval      TRUE: Suspended IF message was successfully sent.
 * @retval      FALSE: Add the IF message back to the queue.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
boolean DoIP_HandlePendingIfMsg(DoIP_TcpTxRxContextType* ctx);

/* rcv */

/**
 * @brief       Handle diagnostic message.
 * @param[in]   pduInfoPtr: Provides the source buffer (SduDataPtr) and the number of bytes to be copied (SduLength).
 * @param[in]   handleDiagCtx: Hand diagnostic context.
 * @param[out]  bufferSizePtr:  Available receive buffer after data has been copied.
 * @return      BufReq_ReturnType
 * @retval      BUFREQ_OK: Data copied successfully.
 * @retval      BUFREQ_E_NOT_OK: Data was not copied because an error occurred.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
BufReq_ReturnType DoIP_HandleDiagMsg(
    const PduInfoType*                pduInfoPtr,
    PduLengthType*                    bufferSizePtr,
    const DoIP_HandleDiagContextType* handleDiagCtx);

#ifdef __cplusplus
}
#endif
#endif
