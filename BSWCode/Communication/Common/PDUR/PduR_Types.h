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
**  FILENAME    : PduR_Types.h                                                **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Type definitions of PDUR                                    **
**                                                                            **
**  SPECIFICATION(S) : AUTOSAR Classic Platform 4.2.2 and R19_11              **
**                                                                            **
*******************************************************************************/
#ifndef PDUR_TYPES_H
#define PDUR_TYPES_H

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "PduR_Cfg.h"

#if (STD_OFF == PDUR_ZERO_COST_OPERATION)
/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
/* Published information */
#if !defined(PDUR_PUBLISHED_INFORMATION)
#define PDUR_PUBLISHED_INFORMATION
#define PDUR_MODULE_ID                   51u
#define PDUR_VENDOR_ID                   62u
#define PDUR_AR_RELEASE_MAJOR_VERSION    4u
#define PDUR_AR_RELEASE_MINOR_VERSION    2u
#define PDUR_AR_RELEASE_REVISION_VERSION 2u
#define PDUR_SW_MAJOR_VERSION            2u
#define PDUR_SW_MINOR_VERSION            3u
#define PDUR_SW_PATCH_VERSION            5u
#elif ((PDUR_SW_MAJOR_VERSION != 2u) || (PDUR_SW_MINOR_VERSION != 3u) || (PDUR_SW_PATCH_VERSION != 5u))
#error "PduR: Mismatch in Software Version"
#endif

typedef enum
{
    PDUR_DIRECT = 0u,
    PDUR_TRIGGERTRANSMIT
} PduR_DestPduDataProvisionType;

typedef uint16 PduR_PBConfigIdType;
typedef uint16 PduR_RoutingPathGroupIdType;

typedef uint32 PduR_TpThresholdType;

typedef enum
{
    PDUR_UNINIT = 0u,
    PDUR_ONLINE
} PduR_StateType;

/*****************************************************************************************************************/
typedef struct
{
    PduLengthType DefaultValueStart;
    PduLengthType DefaultValueLength;
} PduRDefaultValueType;

typedef enum
{
    PDUR_BUFFER_UNLOCK,
    PDUR_BUFFER_LOCK
} PduR_BufferLockStateType;

typedef enum
{
    PDUR_BUFFER_WRITE_OK,
    PDUR_BUFFER_WRITE_NOT_OK,
    PDUR_BUFFER_WRITE_PENDING
} PduR_BufferWriteStateType;

typedef enum
{
    PDUR_ROUTE_IF_TX_NOBUFFERED,
    PDUR_ROUTE_IF_RX_NOBUFFERED,
    PDUR_ROUTE_IF_RX_BUFFERED,
    PDUR_ROUTE_IF_GW_NOBUFFERED,
    PDUR_ROUTE_IF_GW_BUFFERED,
    PDUR_ROUTE_TP_TX_NOBUFFERED,
    PDUR_ROUTE_TP_RX_NOBUFFERED,
    PDUR_ROUTE_TP_RX_BUFFERED,
    PDUR_ROUTE_TP_GW_BUFFERED
} PduR_RouteType;

typedef enum
{
    PDUR_RES_INITIAL,
    PDUR_RES_PENDING,
    PDUR_RES_OK,
    PDUR_RES_NOT_OK
} PduR_RouteStatusType;

typedef enum
{
    PDUR_QUEUESTORE_NOT_OK,
    PDUR_QUEUESTORE_OK,
    PDUR_QUEUESTORE_FLUSH_OK
} PduR_QueueStoreType;

typedef struct
{
    boolean PduRSrcPduUpTxConf;
    uint8 BswModuleIndex;
#if (STD_ON == PDUR_META_DATA_SUPPORT)
    uint8 MetaDataLength;
#endif
#if (STD_ON == PDUR_MULITIPARTITION_SUPPORT)
    uint8 PartitionIndex;
#endif /* STD_ON == PDUR_MULITIPARTITION_SUPPORT */
    PduIdType PduRSrcModulePduIndex;
} PduRSrcPduType;

typedef struct
{
    uint8 BswModuleIndex;
#if (STD_ON == PDUR_META_DATA_SUPPORT)
    uint8 MetaDataLength;
#endif /* STD_ON == PDUR_META_DATA_SUPPORT */
#if (STD_ON == PDUR_MULITIPARTITION_SUPPORT)
    uint8 PartitionIndex;
#endif /* STD_ON == PDUR_MULITIPARTITION_SUPPORT */
    boolean transmissionConfirmation;
#if (PDUR_NUMBER_OF_QUEUES > 0u)
    uint16 PduRDestQueueIdRef;
#endif /* PDUR_NUMBER_OF_QUEUES > 0u */
    uint8 PduSrcSum;
    P2CONST(PduIdType, AUTOMATIC, PDUR_CONST_PBCFG) PduRSrcPduRef;
    PduIdType PduRDestModulePduIndex;
    PduLengthType configuredLength;
#if (PDUR_NUMBER_OF_QUEUES > 0u)
    PduR_TpThresholdType PduRTpThreshold;
#endif /* PDUR_NUMBER_OF_QUEUES >0u */
    PduR_RouteType routeType;
    PduR_DestPduDataProvisionType PduRDestPduDataProvision;
#if (PDUR_DEFAULT_VALUE_PDU > 0u)
    P2CONST(PduRDefaultValueType, AUTOMATIC, PDUR_CONST_PBCFG) PduRDefaultValueRef;
#endif /* PDUR_DEFAULT_VALUE_PDU > 0u */
} PduRDestPduType;

typedef struct
{
    uint8 PduDestSum;
    boolean TpRoute;
    P2CONST(PduIdType, AUTOMATIC, PDUR_CONST_PBCFG) PduRDestPduIdRef;
} PduRRoutingPathType;

typedef struct
{
    P2CONST(PduRRoutingPathType, AUTOMATIC, PDUR_CONST_PBCFG) PduRRoutingPathRef;
} PduRRoutingTableType;
/*****************************************************************************************************************/
typedef struct
{
    boolean PduRIsEnabledAtInit;
    PduR_RoutingPathGroupIdType PduRRoutingPathGroupId;
    PduIdType PduRDestPduRefNumber;
    P2CONST(PduIdType, AUTOMATIC, PDUR_CONST_PBCFG) PduRDestPduIdRef;
} PduRRoutingPathGroupType;
/*****************************************************************************************************************/
typedef struct
{
    PduR_PBConfigIdType PduRConfigId;
    uint16 routingPathGroupCnt;
    uint16 PduRSrcPduNum;
    PduIdType PduRRoutingPathNum;
    P2CONST(PduRRoutingPathGroupType, AUTOMATIC, PDUR_CONST_PBCFG) PduRRoutingPathGroupRef;
    P2CONST(PduRRoutingTableType, AUTOMATIC, PDUR_CONST_PBCFG) PduRRoutingTableRef;
    P2CONST(PduRSrcPduType, AUTOMATIC, PDUR_CONST_PBCFG) PduRSrcPduRef;
    P2CONST(PduRDestPduType, AUTOMATIC, PDUR_CONST_PBCFG) PduRDestPduRef;
} PduR_PBConfigType;

typedef Std_ReturnType (*ModuleCancelReceiveApiType)(PduIdType RxPduId);
typedef Std_ReturnType (*ModuleIfTpCancelTransmitApiType)(PduIdType TxPduId);
typedef Std_ReturnType (*ModuleTriggertransmitApiType)(PduIdType TxPduId, PduInfoType* PduInfoPtr);
typedef Std_ReturnType (*ModuleIfTransmitApiType)(PduIdType id, const PduInfoType* info);
typedef void (*ModuleTxConfirmationApiType)(PduIdType TxPduId);
typedef void (*ModuleIfRxIndicationApiType)(PduIdType RxPduId, const PduInfoType* PduInfoPtr);
typedef Std_ReturnType (*ModuleTpTransmitApiType)(PduIdType id, const PduInfoType* info);
typedef BufReq_ReturnType (*ModuleCopyTxDataApiType)(
    PduIdType id,
    const PduInfoType* info,
    const RetryInfoType* retry,
    PduLengthType* availableDataPtr);
typedef void (*ModuleTpTxConfirmationApiType)(PduIdType id, Std_ReturnType result);
typedef BufReq_ReturnType (*ModuleStartOfReceptionApiType)(
    PduIdType id,
    const PduInfoType* info,
    PduLengthType TpSduLength,
    PduLengthType* bufferSizePtr);
typedef BufReq_ReturnType (
    *ModuleCopyRxDataApiType)(PduIdType id, const PduInfoType* info, PduLengthType* bufferSizePtr);
typedef void (*ModuleTpRxIndicationApiType)(PduIdType id, Std_ReturnType result);

typedef struct
{
    uint8 PduRBswModuleRef;
#if (STD_OFF == PDUR_MULITIPARTITION_SUPPORT)
#if (STD_ON == PDUR_CANCEL_RECEIVE)
    ModuleCancelReceiveApiType CancelReceiveApi;
#endif /* STD_ON == PDUR_CANCEL_RECEIVE */
#if (STD_ON == PDUR_CANCEL_TRANSMIT)
    ModuleIfTpCancelTransmitApiType IfCancelTransmitApi;
    ModuleIfTpCancelTransmitApiType TpCancelTransmitApi;
#endif /* STD_ON == PDUR_CANCEL_TRANSMIT */
#if (STD_ON == PDUR_TRIGGER_TRANSMIT)
    ModuleTriggertransmitApiType TriggertransmitApi;
#endif /* STD_ON == PDUR_TRIGGER_TRANSMIT */
#if (STD_ON == PDUR_TRANSMIT_SUPPORT)
    ModuleIfTransmitApiType IfTransmitApi;
#endif /* STD_ON == PDUR_TRANSMIT_SUPPORT */
#if (STD_ON == PDUR_TX_CONFIRMATION)
    ModuleTxConfirmationApiType TxConfirmationApi;
#endif /* STD_ON == PDUR_TX_CONFIRMATION */
#if (STD_ON == PDUR_RX_INDICATION)
    ModuleIfRxIndicationApiType IfRxIndicationApi;
#endif /* STD_ON == PDUR_RX_INDICATION */
#if (STD_ON == PDUR_TRANSMIT_SUPPORT)
    ModuleTpTransmitApiType TpTransmitApi;
#endif /* STD_ON == PDUR_TRANSMIT_SUPPORT */
#if (STD_ON == PDUR_TP_COPYTXDATA)
    ModuleCopyTxDataApiType CopyTxDataApi;
#endif /* STD_ON == PDUR_TP_COPYTXDATA */
#if (STD_ON == PDUR_TP_TXCONFIRMATION)
    ModuleTpTxConfirmationApiType TpTxConfirmationApi;
#endif /* STD_ON == PDUR_TP_TXCONFIRMATION */
#if (STD_ON == PDUR_TP_STARTOFRECEPTION_TRANSMIT)
    ModuleStartOfReceptionApiType StartOfReceptionApi;
#endif /* STD_ON == PDUR_TP_STARTOFRECEPTION_TRANSMIT */
#if (STD_ON == PDUR_TP_COPYRXDATA)
    ModuleCopyRxDataApiType CopyRxDataApi;
#endif /* STD_ON == PDUR_TP_COPYRXDATA */
#if (STD_ON == PDUR_TP_RXINDICATION)
    ModuleTpRxIndicationApiType TpRxIndicationApi;
#endif /* STD_ON == PDUR_TP_RXINDICATION */
#else
    uint8 PduRBswModulePartitionNum;
    const uint8* PduRBswModulePartitionRef;
#if (STD_ON == PDUR_CANCEL_RECEIVE)
    ModuleCancelReceiveApiType* CancelReceiveApi;
#endif /* STD_ON == PDUR_CANCEL_RECEIVE */
#if (STD_ON == PDUR_CANCEL_TRANSMIT)
    ModuleIfTpCancelTransmitApiType* IfCancelTransmitApi;
    ModuleIfTpCancelTransmitApiType* TpCancelTransmitApi;
#endif /* STD_ON == PDUR_CANCEL_TRANSMIT */
#if (STD_ON == PDUR_TRIGGER_TRANSMIT)
    ModuleTriggertransmitApiType* TriggertransmitApi;
#endif /* STD_ON == PDUR_TRIGGER_TRANSMIT */
#if (STD_ON == PDUR_TRANSMIT_SUPPORT)
    ModuleIfTransmitApiType* IfTransmitApi;
#endif /* STD_ON == PDUR_TRANSMIT_SUPPORT */
#if (STD_ON == PDUR_TX_CONFIRMATION)
    ModuleTxConfirmationApiType* TxConfirmationApi;
#endif /* STD_ON == PDUR_TX_CONFIRMATION */
#if (STD_ON == PDUR_RX_INDICATION)
    ModuleIfRxIndicationApiType* IfRxIndicationApi;
#endif /* STD_ON == PDUR_RX_INDICATION */
#if (STD_ON == PDUR_TRANSMIT_SUPPORT)
    ModuleTpTransmitApiType* TpTransmitApi;
#endif /* STD_ON == PDUR_TRANSMIT_SUPPORT */
#if (STD_ON == PDUR_TP_COPYTXDATA)
    ModuleCopyTxDataApiType* CopyTxDataApi;
#endif /* STD_ON == PDUR_TP_COPYTXDATA */
#if (STD_ON == PDUR_TP_TXCONFIRMATION)
    ModuleTpTxConfirmationApiType* TpTxConfirmationApi;
#endif /* STD_ON == PDUR_TP_TXCONFIRMATION */
#if (STD_ON == PDUR_TP_STARTOFRECEPTION_TRANSMIT)
    ModuleStartOfReceptionApiType* StartOfReceptionApi;
#endif /* STD_ON == PDUR_TP_STARTOFRECEPTION_TRANSMIT */
#if (STD_ON == PDUR_TP_COPYRXDATA)
    ModuleCopyRxDataApiType* CopyRxDataApi;
#endif /* STD_ON == PDUR_TP_COPYRXDATA */
#if (STD_ON == PDUR_TP_RXINDICATION)
    ModuleTpRxIndicationApiType* TpRxIndicationApi;
#endif /* STD_ON == PDUR_TP_RXINDICATION */
#endif /* STD_OFF == PDUR_MULITIPARTITION_SUPPORT */
} PduRBswModuleType;

/* PduR_BufferType is designed to be compatible with PduR_TxBufferType and PduR_TpBufferTableType */
typedef struct
{
    const PduLengthType pduMaxLength; /* Configured PduLength + metadata length */
    PduLengthType sduLength;          /* Total length of the Tp N-SDU to be received or the I-SDU to be transmitted */
    PduLengthType pduCurLength;       /* Current length of the data stored in the buffer */
    PduIdType srcPduId;               /* Source Pdu Id */
    uint8* data;
    PduR_BufferLockStateType lockState;   /* Occupied by a queue */
    PduR_BufferWriteStateType writeState; /* Data receive state */
} PduR_BufferType;

typedef uint8 PduR_QueueDepthType;
typedef uint16 PduR_BufferPoolSizeType;
typedef uint16 PduR_BufferIndexType;
typedef PduIdType PduR_QueueIndexType; /* PduRDestPdu => PduRRoutingPath => PduRQueue */
typedef struct
{
    PduR_BufferPoolSizeType size;
    const PduR_BufferIndexType* indexes;
} PduR_BufferPoolType;

typedef struct
{
    const PduR_QueueDepthType depth;
    PduLengthType
        curPosition; /* Current position of head buffer which identifies the number of bytes to be transmitted */
    PduLengthType
        curSduLength; /* Used in inter-partition TP Transmit to determine the tpSdu is already copied to PduR Buffer */
    PduR_BufferType** bufferPtrs; /* Point to a PduR_BufferType* array of size depth, init to ALL NULL PTR */
    const PduR_BufferPoolType* const bufferPoolPtr;
} PduR_QueueType;

/* The range of MetaDataItemLength is "1 .. 64" (ECUC_EcuC_00075) */
typedef uint8 PduR_MetaDataLengthType;

#endif /*STD_OFF == PDUR_ZERO_COST_OPERATION*/

#endif /* end of PDUR_TYPES_H */
