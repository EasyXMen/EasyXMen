/* PRQA S 3108++ */
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
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : PduR_Route.h                                                **
**                                                                            **
**  Created on  :                                                             **
**  Author      : xiaojian.liang                                              **
**  Vendor      :                                                             **
**  DESCRIPTION : Inter-Partition Routing Header                              **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2 and R19_11            **
**                                                                            **
*******************************************************************************/
#ifndef PDUR_ROUTE_H
#define PDUR_ROUTE_H
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/* Published information */
#if !defined(PDUR_PUBLISHED_INFORMATION)
#define PDUR_PUBLISHED_INFORMATION
#define PDUR_MODULE_ID                   51u
#define PDUR_VENDOR_ID                   62u
#define PDUR_AR_RELEASE_MAJOR_VERSION    4u
#define PDUR_AR_RELEASE_MINOR_VERSION    5u
#define PDUR_AR_RELEASE_REVISION_VERSION 0u
#define PDUR_SW_MAJOR_VERSION            2u
#define PDUR_SW_MINOR_VERSION            3u
#define PDUR_SW_PATCH_VERSION            5
#elif ((PDUR_SW_MAJOR_VERSION != 2u) || (PDUR_SW_MINOR_VERSION != 3u) || (PDUR_SW_PATCH_VERSION != 5u))
#error "PduR: Mismatch in Software Version"
#endif

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
#if (PDUR_TRANSMIT_SUPPORT == STD_ON)
extern Std_ReturnType PduR_PduRIfTransmit(PduIdType srcPduId, PduIdType destPduId, const PduInfoType* pduInfoPtr);
#endif /* PDUR_TRANSMIT_SUPPORT == STD_ON */

#if (PDUR_TRANSMIT_SUPPORT == STD_ON)
extern Std_ReturnType PduR_PduRTpTransmit(PduIdType srcPduId, PduIdType destPduId, const PduInfoType* pduInfoPtr);
#endif /* PDUR_TRANSMIT_SUPPORT == STD_ON */

#if (PDUR_CANCEL_TRANSMIT == STD_ON)
extern Std_ReturnType PduR_PduRCanceTransmit(PduIdType srcPduId, PduIdType destPduId);
#endif /* PDUR_CANCEL_TRANSMIT == STD_ON */

#if (PDUR_CANCEL_RECEIVE == STD_ON)
extern Std_ReturnType PduR_PduRCancelReceive(PduIdType srcPduId, PduIdType destPduId);
#endif /* PDUR_CANCEL_RECEIVE == STD_ON */

#if (PDUR_RX_INDICATION == STD_ON)
extern void PduR_PduRIfRxIndication(PduIdType srcPduId, PduIdType destPduId, const PduInfoType* pduInfoPtr);
#endif /* PDUR_RX_INDICATION == STD_ON */

#if (PDUR_TX_CONFIRMATION == STD_ON)
extern void PduR_PduRIfTxConfirmation(PduIdType srcPduId, PduIdType destPduId, Std_ReturnType result);
#endif /* PDUR_TX_CONFIRMATION == STD_ON */

#if (PDUR_TRIGGER_TRANSMIT == STD_ON)
extern Std_ReturnType PduR_PduRTriggerTransmit(PduIdType srcPduId, PduIdType destPduId, PduInfoType* pduInfoPtr);
#endif /* PDUR_TRIGGER_TRANSMIT == STD_ON */

#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON
extern BufReq_ReturnType PduR_StartOfReceptionForward(
    PduIdType destPduId,
    const PduInfoType* pduInfoPtr,
    PduLengthType tpSduLength,
    PduLengthType* bufferSizePtr);
extern BufReq_ReturnType PduR_CopyRxDataForward(
    PduIdType destPduId,
    const PduInfoType* pduInfoPtr,
    PduLengthType* bufferSizePtr);
extern void PduR_TpRxIndicationForward(PduIdType destPduId, Std_ReturnType result);
#endif /* PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON \
          || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON */

#if PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON
extern BufReq_ReturnType PduR_PduRStartOfReceptionBuffered(
    PduIdType srcPduId,
    PduIdType destPduId,
    const PduInfoType* pduInfoPtr,
    PduLengthType tpSduLength,
    PduLengthType* bufferSizePtr);
extern BufReq_ReturnType PduR_PduRCopyRxDataBuffered(
    PduIdType srcPduId,
    PduIdType destPduId,
    const PduInfoType* pduInfoPtr,
    PduLengthType* bufferSizePtr);
extern void PduR_PduRTpRxIndicationBuffered(PduIdType srcPduId, PduIdType destPduId, Std_ReturnType result);
#endif /* PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON \
          || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON */

#if (PDUR_TP_COPYTXDATA == STD_ON)
extern BufReq_ReturnType PduR_PduRCopyTxData(
    PduIdType destPduId,
    const PduInfoType* pduInfoPtr,
    const RetryInfoType* retryInfoPtr,
    PduLengthType* availableDataPtr);
#endif /* PDUR_TP_COPYTXDATA == STD_ON */

#if (PDUR_TP_TXCONFIRMATION == STD_ON)
extern void PduR_PduRTpTxConfirmation(PduIdType srcPduId, PduIdType destPduId, Std_ReturnType result);
#endif /* PDUR_TP_TXCONFIRMATION == STD_ON */

#if PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON
extern void PduR_TpDestinationProcess(PduIdType destPduId, uint8 apiId);
#endif /* PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON \
          || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON */

extern void PduR_RouteInit(void);
#endif /* end of PDUR_ROUTE_H */
/*******************************************************************************
**                      End Of File                                           **
*******************************************************************************/
