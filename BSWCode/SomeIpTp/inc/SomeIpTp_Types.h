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
 **  @file               : SomeIpTp_Types.h
 **  @author             : HuRongbo
 **  @date               : 2020/07/07
 **  @vendor             : isoft
 **  @description        : SOMEIP Transport Protocol
 **
 ***********************************************************************************************************************/

#ifndef SOMEIPTP_TYPES_H
#define SOMEIPTP_TYPES_H

/* PRQA S 1536 EOF */ /* VL_SomeIpTp_1536 */

/* =================================================== inclusions =================================================== */
#include "ComStack_Types.h"
#include "SomeIpTp_Cfg.h"

#ifdef __cplusplus
extern "C" {
#endif
/* =============================================== version information ============================================== */

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* Pdu handle state */
typedef enum SomeIpTp_PduStateTag
{
    SOMEIPTP_PDU_IDLE,
    /* Tx request is accepted */
    SOMEIPTP_PDU_TX_WAIT,
    SOMEIPTP_PDU_TX_COPY_DATA,
    SOMEIPTP_PDU_TX_WAIT_CONF,
    SOMEIPTP_PDU_TX_CONFIRMED,
    /* Inconstant state start after received single segemt */
    SOMEIPTP_PDU_RX_SINGLE_SEG,
    /* Inconstant state start after received first segemt */
    SOMEIPTP_PDU_RX_FIRST_SEG,
    /* Start wait Rx Consecutive segment */
    SOMEIPTP_PDU_RX_CONSECUTIVE_SEG,
} SomeIpTp_PduStateType;

/* SomeIp Header type define */
typedef struct SomeIpTp_HeaderTag
{
    uint32 RequestID;
    uint8  ProtocolVersion;
    uint8  InterfaceVersion;
    uint8  MessageType;
    uint8  ReturnCode;
} SomeIpTp_HeaderType;

/* Define receive pdu runtime value */
typedef struct SomeIpTp_RxPduRTTag
{
    /* Timer to monitor the successful reception. */
    uint32 RxTimeoutTimer;
    /* Used to save recviced header in fisrt segment */
    SomeIpTp_HeaderType Header;
    /* Pdu state */
    SomeIpTp_PduStateType PduState;
    /* Received payload data counter */
    PduLengthType RxPayloadCnt;
    /* Available buffer size  in upper layer */
    PduLengthType BufferSize;

} SomeIpTp_RxPduRTType;

/* Define transmit pdu runtime value */
typedef struct SomeIpTp_TxPduRTTag
{
    /*
     * The duration of the minimum time the SomeIpTp module shall wait between
     * the transmissions of N-PDUs.
     */
    uint32 SeparationTimer;
    uint32 TxTimeoutTimer;
#if (SOMEIPTP_HAS_CONFIGED_TX_BURST_SIZE == STD_ON)
    uint32 TxBurstSegmentCount;
#endif
    /* Pdu state */
    SomeIpTp_PduStateType PduState;
    /* Save recviced SOME/IP Header(not include TP Header) in fisrt segment */
    SomeIpTp_HeaderType Header;

    /* Transmited data counter */
    PduLengthType TxDataCnt;
    /* Pdu length passed by SomeIpTp_Transmit() API */
    PduLengthType TxPduLen;
    /* Available data length in upper layer */
    PduLengthType AvailableDataLen;
    /* Last calling API PduR_SomeIpTpTransmit() used segment size */
    PduLengthType SegmentSize;
    boolean       FirstSegmentFlag;
    boolean       NoSegmentFlag;
    boolean       LastSegmentFlag;

} SomeIpTp_TxPduRTType;

/* NSDU which has to be passed as one assembled RxPdu to the upper layer.*/
typedef struct SomeIpTp_RxNSduTag
{
    /*
     * This parameter defines the handle ID that is used by the PduR when
     * calling SomeIpTp_RxIndication.
     */
    PduIdType SomeIpTpRxNPduHandleId;
    /*
     * Reference to a Pdu in the COM-Stack that represents the assembled
     * RxPdu which is passed via the PduR to the upper layer
     */
    PduIdType SomeIpTpRxSduRef;
#if (SOMEIPTP_USED_PARTITION_NUM > 1)
    /* Positiion of this RxPdu on its SomeIpTp_RxPduRTData_PartitionX */
    PduIdType OnPartitionIndex;
    /* Index in SomeIpTp Partitions */
    uint8 SomeIpTpPartition;
#endif
} SomeIpTp_RxNSduType;

/* NSDU that the SomeIpTp module transmits via the SomeIpTpChannel. */
typedef struct SomeIpTp_TxNSduTag
{
    /*
     * This handle ID is used by PduR when calling SomeIpTp_Transmit.
     * SomeIpTp as Lower Layer.
     */
    PduIdType SomeIpTpTxNSduHandleId;
    /* SomeIpTp as lower layer calling PduR's API with this Id */
    PduIdType SomeIpTpTxNSduUpperLayerId;

    /*
     * This handle ID is used by PduR when calling SomeIpTp_TriggerTransmit.
     * SomeIpTp as Upper Layre.
     */
    PduIdType SomeIpTpTxNPduHandleId;
    /* SomeIpTp as upper layer calling PduR's API with this Id */
    PduIdType SomeIpTpTxNPduLowerLayerId;

    /*
     * The maximum possible size of the segmented SOME/IP PDUs derive from
     * parameter SomeIpTpTxNPduRef.
     */
    PduLengthType SomeIpTpTxMaxPduLen;
#if (SOMEIPTP_USED_PARTITION_NUM > 1)
    /* Positiion of this TxPdu on its SomeIpTp_TxPduRTData_PartitionX */
    PduIdType OnPartitionIndex;
    /* Index in SomeIpTp Partitions */
    uint8 SomeIpTpPartition;
#endif
} SomeIpTp_TxNSduType;

/* Configuration parameters of the SomeIpTp channel. */
typedef struct SomeIpTp_ChannelTag
{
    /*
     * Sets the duration of the minimum time in seconds the SomeIpTp module
     * shall wait between the transmissions of N-PDUs.
     */
    uint32 SomeIpTpNPduSeparationTime;

    /* Timer to monitor the successful reception. */
    uint32 SomeIpTpRxTimeoutTime;

    /* Timer to monitor the successful transmission. */
    uint32 SomeIpTpTxTimeoutTime;
#if (SOMEIPTP_HAS_CONFIGED_TX_BURST_SIZE == STD_ON)
    uint32 SomeIpTpTxBurstSize;
#endif
    /* NSDU which has to be passed as one assembled RxPdu to the upper layer.*/
    const SomeIpTp_RxNSduType* SomeIpTpRxNSdu;
    /* Number of RxNSdus  belonging to this channnel */
    PduIdType SomeIpTpRxNSduNum;

    /* NSDU that the SomeIpTp module transmits via the SomeIpTpChannel. */
    const SomeIpTp_TxNSduType* SomeIpTpTxNSdu;
    PduIdType                  SomeIpTpTxNSduNum;
} SomeIpTp_ChannelType;

/* Configuration of the SomeIpTp module. */
typedef struct SomeIpTp_ConfigTag
{
    const SomeIpTp_ChannelType* SomeIpTpChannel;
#if (SOMEIPTP_CONFIG_NUM_OF_VARIANTS_USED > 1)
    /* map RxNSdu and RxNPdu Macro Id to its configuration index */
    const PduIdType* RxNSduAndRxNPduMacroMap;
    /* map TxNSdu and TxNPdu Macro Id to its configuration index */
    const PduIdType* TxNSduAndTxNPduMacroMap;
    /* Maximum number of RxNSdus/RxNPdus in current variant */
    PduIdType RxPduMaxNmberInCurrentVariant;
    /* Maximum number of TxNSdus/TxNPdus in current variant */
    PduIdType TxPduMaxNmberInCurrentVariant;
    /* Maximum number of Channels in current variant */
    PduIdType ChannelMaxNmberInCurrentVariant;
#endif

} SomeIpTp_ConfigType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

#ifdef __cplusplus
}
#endif

#endif
