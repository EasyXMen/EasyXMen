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
 **  @file               : XcpOnEth_Cbk.h
 **  @author             : qinchun.yang
 **  @date               : 2024/11/17
 **  @vendor             : isoft
 **  @description        : CBK API declaration of XCPOnEth
 **
 ***********************************************************************************************************************/
#ifndef XCPONETH_CBK_H
#define XCPONETH_CBK_H

/* =================================================== inclusions =================================================== */
#include "Xcp_GenericTypes.h"

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief       The lower layer communication interface module confirms the transmission of a PDU, or the failure to
 * transmit a PDU.
 * @param[in]   TxPduId   : The PDU ID to be notified as having been sent.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63959
 */
extern void Xcp_SoAdTxConfirmation(PduIdType TxPduId);

/**
 * @brief       Indication of a received PDU from a lower layer communication interface module.
 * @param[in]   RxPduId   : Receive the XCP message PDU ID.
 * @param[in]   PduInfoPtr : Pointer to the PDU of the received XCP message.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63958
 */
extern void Xcp_SoAdRxIndication(PduIdType RxPduId, const PduInfoType* RxPduPtr);

#endif
