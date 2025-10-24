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
 **  @file               : PduR_Types.h
 **  @author             : zhengfei.li
 **  @date               : 2020/06/22
 **  @vendor             : isoft
 **  @description        : Type definitions of PduR
 **
 ***********************************************************************************************************************/
#ifndef PDUR_TYPES_H_
#define PDUR_TYPES_H_

/* =================================================== inclusions =================================================== */
#include "PduR_Cfg.h"

#ifdef __cplusplus
extern "C" {
#endif
/* PRQA S 3332,1252 ++ */ /* VL_QAC_3332,VL_QAC_1252 */
#if (STD_OFF == PDUR_ZERO_COST_OPERATION)
/* =============================================== version information ============================================== */

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */
/**
 * @brief     PduRDestPduDataProvision enum.
 */
typedef enum
{
    PDUR_DIRECT,
    PDUR_TRIGGERTRANSMIT
} PduR_DestPduDataProvisionType;

/**
 * @brief     PduR buffer lock state
 */
typedef enum
{
    PDUR_BUFFER_UNLOCK,
    PDUR_BUFFER_LOCK
} PduR_BufferLockStateType;

/**
 * @brief     PduR buffer write state
 */
typedef enum
{
    PDUR_BUFFER_WRITE_OK,
    PDUR_BUFFER_WRITE_NOT_OK,
    PDUR_BUFFER_WRITE_PENDING
} PduR_BufferWriteStateType;

/**
 * @brief     PduR Route type
 */
typedef enum
{
    PDUR_ROUTE_IF_TX_NOBUFFERED,
    PDUR_ROUTE_IF_RX_NOBUFFERED,
    PDUR_ROUTE_IF_GW_NOBUFFERED,
    PDUR_ROUTE_IF_GW_BUFFERED,
    PDUR_ROUTE_TP_TX_NOBUFFERED,
    PDUR_ROUTE_TP_RX_NOBUFFERED,
    PDUR_ROUTE_TP_RX_BUFFERED,
    PDUR_ROUTE_TP_GW_BUFFERED
} PduR_RouteType;

/**
 * @brief     PduR Route status type
 */
typedef enum
{
    PDUR_RES_INITIAL,
    PDUR_RES_PENDING,
    PDUR_RES_OK,
    PDUR_RES_NOT_OK
} PduR_RouteStatusType;

/**
 * @brief     PduR queue store status type
 */
typedef enum
{
    PDUR_QUEUESTORE_NOT_OK,
    PDUR_QUEUESTORE_OK,
    PDUR_QUEUESTORE_FLUSH_OK
} PduR_QueueStoreType;

/**
 * @brief     PduR module init state
 */
typedef enum
{
    PDUR_UNINIT,
    PDUR_ONLINE
} PduR_StateType;

/**
 * @brief     Identification of the post-build configuration
 */
typedef uint16 PduR_PBConfigIdType;

/**
 * @brief     Identification of a Routing group
 */
typedef uint16 PduR_RoutingPathGroupIdType;

/**
 * @brief     PduR internal type
 */
typedef uint8  PduR_QueueDepthType;
typedef uint8  PduR_BufferPoolSizeType;
typedef uint16 PduR_BufferIndexType;

/**
 * @brief     PduR route path group type
 */
typedef struct PduR_RoutingPathGroupTypeTag
{
    boolean                     PduRIsEnabledAtInit;    /** PduRIsEnabledAtInit */
    PduR_RoutingPathGroupIdType PduRRoutingPathGroupId; /** PduRRoutingPathGroupId */
    PduIdType                   PduRDestPduRefNumber;
    const PduIdType*            PduRDestPduIdRef;
} PduR_RoutingPathGroupType;

/**
 * @brief     PduR buffer pool type
 */
typedef struct PduR_BufferPoolTypeTag
{
    PduR_BufferPoolSizeType     Size;
    const PduR_BufferIndexType* Indexes;
} PduR_BufferPoolType;

/**
 * @brief     PduR queue configuration type
 */
typedef struct PduR_QueueCfgTypeTag
{
    PduR_QueueDepthType        Depth; /** PduRQueueDepth */
    const PduR_BufferPoolType* BufferPoolPtr;
} PduR_QueueCfgType;

/**
 * @brief     PduR path default value type
 */
typedef struct
{
    PduLengthType DefaultValueStart;
    PduLengthType DefaultValueLength;
} PduR_DefaultValueType;

/**
 * @brief     PduR src pdu type
 */
typedef struct PduR_SrcPduTypeTag
{
    boolean PduRSrcPduUpTxConf; /** PduRSrcPduUpTxConf */
    uint8   BswModuleIndex;
#if (STD_ON == PDUR_META_DATA_SUPPORT)
    uint8 MetaDataLength;
#endif
#if (STD_ON == PDUR_MULITIPARTITION_SUPPORT)
    uint8 PartitionIndex;
#endif
    PduIdType PduRSrcModulePduIndex;
} PduR_SrcPduType;

/**
 * @brief     PduR dest pdu type
 */
typedef struct PduR_DestPduTypeTag
{
    uint8 BswModuleIndex;
#if (STD_ON == PDUR_META_DATA_SUPPORT)
    uint8 MetaDataLength;
#endif
#if (STD_ON == PDUR_MULITIPARTITION_SUPPORT)
    uint8 PartitionIndex;
#endif
    boolean TransmissionConfirmation; /** PduRTransmissionConfirmation */
#if (PDUR_NUMBER_OF_BUFFERS > 0u)
    uint16 PduRDestTxBufferRef;
#endif
    PduIdType     PduRSrcPduRef;
    PduIdType     PduRDestModulePduIndex;
    PduLengthType ConfiguredLength; /** Pdu length */
#if (PDUR_NUMBER_OF_QUEUES_MAX > 0u)
    uint32 PduRTpThreshold; /** PduRTpThreshold */
#endif
    PduR_RouteType                RouteType;
    PduR_DestPduDataProvisionType PduRDestPduDataProvision; /** PduRDestPduDataProvision */
#if (PDUR_DEFAULT_VALUE_PDU > 0u)
    const PduR_DefaultValueType* PduRDefaultValueRef;
#endif
} PduR_DestPduType;

/**
 * @brief     PduR route path type
 */
typedef struct
{
    uint8   PduDestSum;
    boolean TpRoute;
#if (STD_ON == PDUR_MULITIPARTITION_SUPPORT)
    uint8* PduRBufDataInterPartitionRef;
#endif
    const PduIdType* PduRDestPduIdRef;
} PduR_RoutingPathType;

/**
 * @brief     PduR route table type
 */
typedef struct PduR_RoutingTableTypeTag
{
    const PduR_RoutingPathType* PduRRoutingPathRef;
} PduR_RoutingTableType;

/**
 * @brief     The root struct configuration parameters of PduR.
 */
typedef struct PduR_PBConfigTypeTag
{
    PduR_PBConfigIdType PduRConfigId;        /** PduRConfigurationId */
    uint16              RoutingPathGroupCnt; /** PduRRoutingPathGroup number */
    PduIdType           PduRSrcPduNum;       /** PduRSrcPdu number */
    PduIdType           PduRRoutingPathNum;  /** PduRRoutingPath number */
#if (PDUR_NUMBER_OF_QUEUES_MAX > 0u)
    uint16 PduRQueueNum;         /** PduRRoutingPath configured with PduRQueueDepth number */
    uint16 PduRQueueDepthSumNum; /** Sum of all PduRQueueDepth */
#endif
#if (PDUR_DEFAULT_VALUE_PDU_ENABLE == STD_ON)
    uint16 PduRDefaultValueNum; /** PduRRoutingPath configured with PduRDefaultValue number */
    uint8* PduRDefaultValuePtr; /** Pointer to DefaultValuePtr */
#endif
#if (PDUR_MULITIVARIANT_SUPPORT == STD_ON)
    const PduIdType* UserIdMapPduRSrcPduIdPtr;  /** User id map to PduR SrcPdu Id */
    const PduIdType* UserIdMapPduRDestPduIdPtr; /** User id map to PduR DestPdu Id */
#if (PDUR_ROUTING_PATH_GROUP_MAX > 0u)
    const PduIdType* UserIdMapPduRPathGroupPtr; /** User id map to PduR RoutingPathGroup Id */
#endif
#endif
#if (PDUR_ROUTING_PATH_GROUP_MAX > 0u)
    const struct PduR_RoutingPathGroupTypeTag* PduRRoutingPathGroupRef; /** Pointer to RoutingPathGroupRef */
#endif
    const struct PduR_RoutingTableTypeTag* PduRRoutingTableRef; /** Pointer to RoutingTableRef */
    const struct PduR_SrcPduTypeTag*       PduRSrcPduRef;       /** Pointer to SrcPduRef */
    const struct PduR_DestPduTypeTag*      PduRDestPduRef;      /** Pointer to DestPduRef */
#if (PDUR_NUMBER_OF_QUEUES_MAX > 0u)
    const struct PduR_QueueCfgTypeTag*   PduR_QueuesCfgPtr;        /** Pointer to QueuesCfgPtr */
    const struct PduR_BufferPoolTypeTag* PduR_GlobalBufferPoolPtr; /** Pointer to GlobalBufferPoolPtr */
#endif
} PduR_PBConfigType;

/* PRQA S 5004,1336 ++ */ /* VL_QAC_FuncPtr, VL_QAC_1336 */
/**
 * @brief     PduRCancelReceive function prototype
 */
typedef Std_ReturnType (*PduR_ModuleCancelReceiveApiType)(PduIdType);

/**
 * @brief     PduRCancelTransmit function prototype
 */
typedef Std_ReturnType (*PduR_ModuleIfTpCancelTransmitApiType)(PduIdType);

/**
 * @brief     PduRTriggertransmit function prototype
 */
typedef Std_ReturnType (*PduR_ModuleTriggertransmitApiType)(PduIdType, PduInfoType*);

/**
 * @brief     PduRTransmit function prototype
 */
typedef Std_ReturnType (*PduR_ModuleIfTransmitApiType)(PduIdType, const PduInfoType*);

/**
 * @brief     PduRTxConfirmation function prototype
 */
typedef void (*PduR_ModuleTxConfirmationApiType)(PduIdType, Std_ReturnType);

/**
 * @brief     PduRRxIndication function prototype
 */
typedef void (*PduR_ModuleIfRxIndicationApiType)(PduIdType, const PduInfoType*);

/**
 * @brief     PduRTpTransmit function prototype
 */
typedef Std_ReturnType (*PduR_ModuleTpTransmitApiType)(PduIdType, const PduInfoType*);

/**
 * @brief     PduRCopyTxData function prototype
 */
typedef BufReq_ReturnType (
    *PduR_ModuleCopyTxDataApiType)(PduIdType, const PduInfoType*, const RetryInfoType*, PduLengthType*);

/**
 * @brief     PduRTpTxConfirmation function prototype
 */
typedef void (*PduR_ModuleTpTxConfirmationApiType)(PduIdType, Std_ReturnType);

/**
 * @brief     PduRStartOfReception function prototype
 */
typedef BufReq_ReturnType (
    *PduR_ModuleStartOfReceptionApiType)(PduIdType, const PduInfoType*, PduLengthType, PduLengthType*);

/**
 * @brief     PduRCopyRxData function prototype
 */
typedef BufReq_ReturnType (*PduR_ModuleCopyRxDataApiType)(PduIdType, const PduInfoType*, PduLengthType*);

/**
 * @brief     PduRTpRxIndication function prototype
 */
typedef void (*PduR_ModuleTpRxIndicationApiType)(PduIdType, Std_ReturnType);
/* PRQA S 5004,1336 -- */
/**
 * @brief     PduR Module configuration type
 */
typedef struct
{
#if (STD_OFF == PDUR_MULITIPARTITION_SUPPORT)
#if (STD_ON == PDUR_CANCEL_RECEIVE)
    const PduR_ModuleCancelReceiveApiType CancelReceiveApi; /** PduRCancelReceive */
#endif
#if (STD_ON == PDUR_CANCEL_TRANSMIT)
    const PduR_ModuleIfTpCancelTransmitApiType IfCancelTransmitApi; /** PduRCancelTransmit */
    const PduR_ModuleIfTpCancelTransmitApiType TpCancelTransmitApi; /** PduRCancelTransmit */
#endif
#if (STD_ON == PDUR_TRIGGER_TRANSMIT)
    const PduR_ModuleTriggertransmitApiType TriggertransmitApi; /** PduRTriggertransmit */
#endif
#if (STD_ON == PDUR_TRANSMIT_SUPPORT)
    const PduR_ModuleIfTransmitApiType IfTransmitApi; /** PduRTransmit */
#endif
#if (STD_ON == PDUR_TX_CONFIRMATION)
    const PduR_ModuleTxConfirmationApiType TxConfirmationApi; /** PduRTxConfirmation */
#endif
#if (STD_ON == PDUR_RX_INDICATION)
    const PduR_ModuleIfRxIndicationApiType IfRxIndicationApi; /** PduRRxIndication */
#endif
#if (STD_ON == PDUR_TRANSMIT_SUPPORT)
    const PduR_ModuleTpTransmitApiType TpTransmitApi; /** PduRTpTransmit */
#endif
#if (STD_ON == PDUR_TP_COPYTXDATA)
    const PduR_ModuleCopyTxDataApiType CopyTxDataApi; /** PduRCopyTxData */
#endif
#if (STD_ON == PDUR_TP_TXCONFIRMATION)
    const PduR_ModuleTpTxConfirmationApiType TpTxConfirmationApi; /** PduRTpTxConfirmation */
#endif
#if (STD_ON == PDUR_TP_STARTOFRECEPTION_TRANSMIT)
    const PduR_ModuleStartOfReceptionApiType StartOfReceptionApi; /** PduRStartOfReception */
#endif
#if (STD_ON == PDUR_TP_COPYRXDATA)
    const PduR_ModuleCopyRxDataApiType CopyRxDataApi; /** PduRCopyRxData */
#endif
#if (STD_ON == PDUR_TP_RXINDICATION)
    const PduR_ModuleTpRxIndicationApiType TpRxIndicationApi; /** PduRTpRxIndication */
#endif
#else
    uint8        BswModulePartitionNum;
    const uint8* BswModulePartitionRef;
#if (STD_ON == PDUR_CANCEL_RECEIVE)
    const PduR_ModuleCancelReceiveApiType* CancelReceiveApi; /** PduRCancelReceive */
#endif
#if (STD_ON == PDUR_CANCEL_TRANSMIT)
    const PduR_ModuleIfTpCancelTransmitApiType* IfCancelTransmitApi; /** PduRCancelTransmit */
    const PduR_ModuleIfTpCancelTransmitApiType* TpCancelTransmitApi; /** PduRCancelTransmit */
#endif
#if (STD_ON == PDUR_TRIGGER_TRANSMIT)
    const PduR_ModuleTriggertransmitApiType* TriggertransmitApi; /** PduRTriggertransmit */
#endif
#if (STD_ON == PDUR_TRANSMIT_SUPPORT)
    const PduR_ModuleIfTransmitApiType* IfTransmitApi; /** PduRTransmit */
#endif
#if (STD_ON == PDUR_TX_CONFIRMATION)
    const PduR_ModuleTxConfirmationApiType* TxConfirmationApi; /** PduRTxConfirmation */
#endif
#if (STD_ON == PDUR_RX_INDICATION)
    const PduR_ModuleIfRxIndicationApiType* IfRxIndicationApi; /** PduRRxIndication */
#endif
#if (STD_ON == PDUR_TRANSMIT_SUPPORT)
    const PduR_ModuleTpTransmitApiType* TpTransmitApi; /** PduRTpTransmit */
#endif
#if (STD_ON == PDUR_TP_COPYTXDATA)
    const PduR_ModuleCopyTxDataApiType* CopyTxDataApi; /** PduRCopyTxData */
#endif
#if (STD_ON == PDUR_TP_TXCONFIRMATION)
    const PduR_ModuleTpTxConfirmationApiType* TpTxConfirmationApi; /** PduRTpTxConfirmation */
#endif
#if (STD_ON == PDUR_TP_STARTOFRECEPTION_TRANSMIT)
    const PduR_ModuleStartOfReceptionApiType* StartOfReceptionApi; /** PduRStartOfReception */
#endif
#if (STD_ON == PDUR_TP_COPYRXDATA)
    const PduR_ModuleCopyRxDataApiType* CopyRxDataApi; /** PduRCopyRxData */
#endif
#if (STD_ON == PDUR_TP_RXINDICATION)
    const PduR_ModuleTpRxIndicationApiType* TpRxIndicationApi; /** PduRTpRxIndication */
#endif
#endif /* end of PDUR_MULITIPARTITION_SUPPORT */
} PduR_BswModuleType;

/**
 * @brief     PduRTxBuffer design
 */
typedef struct
{
    PduLengthType PduMaxLength; /** Configured PduLength + metadata length (PduRPduMaxLength) */
    PduLengthType SduLength;    /** Total length of the Tp N-SDU to be received or the I-SDU to be transmitted */
    PduLengthType PduCurLength; /** Current length of the data stored in the buffer */
    uint8*        Data;         /** pointer to address for the saved data  */
    PduR_BufferLockStateType  LockState;  /** Occupied by a queue */
    PduR_BufferWriteStateType WriteState; /** Data receive state */
} PduR_BufferType;

/**
 * @brief     PduR queue runtime type
 */
typedef struct
{
    PduR_QueueDepthType Depth; /** Current queue depth */
    PduLengthType CurPosition; /** Current position of head buffer in current queue which identifies the number of bytes
                                  to be transmitted */
    PduR_BufferType**          BufferPtrs; /** Point to a PduR_BufferType* array of size Depth, init to ALL NULL PTR */
    const PduR_BufferPoolType* BufferPoolPtr; /** Point to all PduRTxBuffer of current queue info pointer */
} PduR_QueueRuntimeType;

#endif /*end of PDUR_ZERO_COST_OPERATION*/
/* PRQA S 3332,1252 -- */
#ifdef __cplusplus
}
#endif
#endif
