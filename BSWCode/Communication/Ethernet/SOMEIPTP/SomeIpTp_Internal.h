/* PRQA S 3108++ */
/**
 * Copyright (C) 2024 Isoft Infrastructure Software Co., Ltd.
 * SPDX-License-Identifier: LGPL-2.1-only-with-exception OR  LicenseRef-Commercial-License
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the
 * GNU Lesser General Public License as published by the Free Software Foundation; version 2.1.
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License along with this library;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * or see <https://www.gnu.org/licenses/>.
 *
 * Alternatively, this file may be used under the terms of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License, in which case the provisions of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License shall apply instead of those of the GNU Lesser General Public License.
 *
 * You should have received a copy of the Isoft Infrastructure Software Co., Ltd.  Commercial License
 * along with this program. If not, please find it at <https://EasyXMen.com/xy/reference/permissions.html>
 */
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : SomeIpTp_Internal.h                                         **
**                                                                            **
**  Created on  : 19/07/07                                                    **
**  Author      : HuRongbo                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION : SomeIpTp internal use                                       **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
#ifndef SOMEIPTP_INTERNAL_H
#define SOMEIPTP_INTERNAL_H

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "ComStack_Types.h"

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
#define SOMEIPTP_TPFLAG_MASK       0x20u
#define SOMEIPTP_OFFSETFIELD_LSB   16u
#define SOMEIPTP_TOTAL_HEADER_LEN  12u
#define SOMEIPTP_MAX_PAYLOAD_LEN   1400u
#define SOMEIPTP_SOMEIP_HEADER_LEN 8u
#define SOMEIPTP_TP_HEADER_LEN     4u
/* SOMEIPTP_MAX_PAYLOAD_LEN + SOMEIPTP_SOMEIP_HEADER_LEN + SOMEIPTP_TP_HEADER_LEN */
#define SOMEIPTP_MAX_PDU_LEN               1404u

#define SOMEIPTP_HEADER_MESSAGE_TYPE_INDEX 6u

#define SOMEIPTP_DET_REPORT_RUNTIMEERROR(ApiId, ErrorId) \
    ((void)Det_ReportRuntimeError(SOMEIPTP_MODULE_ID, SOMEIPTP_INSTANCE_ID, (ApiId), (ErrorId)))

/*******************************************************************************
**                      Private Type Definitions                               *
*******************************************************************************/
/* Define the initialization states of the SOMEIPTP module */
typedef enum
{
    SOMEIPTP_UNINIT,
    SOMEIPTP_INIT
} SomeIpTp_InitStateType;

/* Pdu handle state */
typedef enum
{
    SOMEIPTP_PDU_IDLE,
    SOMEIPTP_PDU_TX_WAIT,
    SOMEIPTP_PDU_TX_COPY_DATA,
    SOMEIPTP_PDU_TX_WAIT_CONF,
    SOMEIPTP_PDU_TX_CONFIRMED,
    SOMEIPTP_PDU_RX_SINGLE_SEG,
    SOMEIPTP_PDU_RX_FIRST_SEG,
    SOMEIPTP_PDU_RX_CONSECUTIVE_SEG,
} SomeIpTp_PduStateType;

/* SomeIp Header type define */
typedef struct
{
    uint32 requestID;
    uint8 protocolVersion;
    uint8 interfaceVersion;
    uint8 messageType;
    uint8 returnCode;
} SomeIpTp_HeaderType;

/* PRQA S 0635 ++ */ /* MISRA Rule 6.1 */
/* SomeIp TP header type define */
typedef struct
{
#if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
    uint32 offset : 28;
    uint32 res : 3;
    uint32 moreSegment : 1;
#else
    uint32 moreSegment : 1;
    uint32 res : 3;
    uint32 offset : 28;
#endif
} SomeIpTp_TpHeaderType;
/* PRQA S 0635 -- */ /* MISRA Rule 6.1 */

/* Define receive pdu runtime value */
typedef struct
{
    /* Pdu state */
    SomeIpTp_PduStateType pduState;
    /* Used to save recviced header in fisrt segment */
    SomeIpTp_HeaderType header;
    /* Timer to monitor the successful reception. */
    uint32 rxTimeoutTimer;
    /* Received payload data counter */
    PduLengthType rxPayloadCnt;
    /* Available buffer size  in upper layer */
    PduLengthType bufferSize;
} SomeIpTp_RxPduRTType;

/* Define transmit pdu runtime value */
typedef struct
{
    /* Pdu state */
    SomeIpTp_PduStateType pduState;
    /* Save recviced SOME/IP Header(not include TP Header) in fisrt segment */
    SomeIpTp_HeaderType header;
    /*
     * The duration of the minimum time the SomeIpTp module shall wait between
     * the transmissions of N-PDUs.
     */
    uint32 separationTimer;
    uint32 txTimeoutTimer;
    /* Transmited data counter */
    PduLengthType txDataCnt;
    /* Pdu length passed by SomeIpTp_Transmit() API */
    PduLengthType txPduLen;
    /* Available data length in upper layer */
    PduLengthType availableDataLen;
    /* Last calling API PduR_SomeIpTpTransmit() used segment size */
    PduLengthType segmentSize;
    boolean firstSegmentFlag;
    boolean noSegmentFlag;
    boolean lastSegmentFlag;
} SomeIpTp_TxPduRTType;

#endif /* SOMEIPTP_INTERNAL_H */
