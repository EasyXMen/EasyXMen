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
 **  @file               : PduR_Route.h
 **  @author             : xiaojian.liang
 **  @date               : 2023/12/5
 **  @vendor             : isoft
 **  @description        : Inter-Partition Routing Header
 **
 ***********************************************************************************************************************/
#ifndef PDUR_ROUTE_H_
#define PDUR_ROUTE_H_

/* =================================================== inclusions =================================================== */
#ifdef __cplusplus
extern "C" {
#endif
/* ================================================= Version  Check ================================================= */

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
#if (PDUR_TRANSMIT_SUPPORT == STD_ON)
extern Std_ReturnType PduR_PduRIfTransmit(PduIdType srcPduId, PduIdType destPduId, const PduInfoType* pduInfoPtr);
#endif

#if (PDUR_TRANSMIT_SUPPORT == STD_ON)
extern Std_ReturnType PduR_PduRTpTransmit(PduIdType srcPduId, PduIdType destPduId, const PduInfoType* pduInfoPtr);
#endif

#if (PDUR_CANCEL_TRANSMIT == STD_ON)
extern Std_ReturnType PduR_PduRCanceTransmit(PduIdType srcPduId, PduIdType destPduId);
#endif

#if (PDUR_CANCEL_RECEIVE == STD_ON)
extern Std_ReturnType PduR_PduRCancelReceive(PduIdType srcPduId, PduIdType destPduId);
#endif

#if (PDUR_RX_INDICATION == STD_ON)
extern void PduR_PduRIfRxIndication(PduIdType srcPduId, PduIdType destPduId, const PduInfoType* pduInfoPtr);
#endif

#if (PDUR_TX_CONFIRMATION == STD_ON)
extern void PduR_PduRIfTxConfirmation(PduIdType srcPduId, PduIdType destPduId, Std_ReturnType result);
#endif

#if (PDUR_TRIGGER_TRANSMIT == STD_ON)
extern Std_ReturnType PduR_PduRTriggerTransmit(PduIdType srcPduId, PduIdType destPduId, PduInfoType* pduInfoPtr);
#endif

#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON
extern BufReq_ReturnType PduR_StartOfReceptionForward(
    PduIdType          destPduId,
    const PduInfoType* pduInfoPtr,
    PduLengthType      tpSduLength,
    PduLengthType*     bufferSizePtr);
extern BufReq_ReturnType PduR_CopyRxDataForward(
    PduIdType          srcPduId,
    PduIdType          destPduId,
    const PduInfoType* pduInfoPtr,
    PduLengthType*     bufferSizePtr);
extern void PduR_TpRxIndicationForward(PduIdType destPduId, Std_ReturnType result);
#endif

#if PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON
extern BufReq_ReturnType PduR_PduRStartOfReceptionBuffered(
    PduIdType          srcPduId,
    PduIdType          destPduId,
    const PduInfoType* pduInfoPtr,
    PduLengthType      tpSduLength,
    PduLengthType*     bufferSizePtr);
extern BufReq_ReturnType PduR_PduRCopyRxDataBuffered(
    PduIdType          srcPduId,
    PduIdType          destPduId,
    const PduInfoType* pduInfoPtr,
    PduLengthType*     bufferSizePtr);
extern void PduR_PduRTpRxIndicationBuffered(PduIdType srcPduId, PduIdType destPduId, Std_ReturnType result);
#endif

#if (PDUR_TP_COPYTXDATA == STD_ON)
extern BufReq_ReturnType PduR_PduRCopyTxData(
    PduIdType            srcPduId,
    PduIdType            destPduId,
    const PduInfoType*   pduInfoPtr,
    const RetryInfoType* retryInfoPtr,
    PduLengthType*       availableDataPtr);
#endif

#if (PDUR_TP_TXCONFIRMATION == STD_ON)
extern void PduR_PduRTpTxConfirmation(PduIdType srcPduId, PduIdType destPduId, Std_ReturnType result);
#endif

extern void PduR_RouteInit(void);

#ifdef __cplusplus
}
#endif
#endif
