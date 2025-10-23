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
 **  @file               : SoAd_Types.h
 **  @author             : zhengfei.li
 **  @date               : 2020/05/07
 **  @vendor             : isoft
 **  @description        : Socket Adaptor data type definition header
 **
 ***********************************************************************************************************************/

#ifndef SOAD_TYPES_H_
#define SOAD_TYPES_H_

/* =================================================== inclusions =================================================== */
#include "SoAd_Cfg.h"
#include "ComStack_Types.h"
#include "TcpIp.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */
/**
 * Id of drop TCP measurement
 */
#define SOAD_MEAS_DROP_TCP (SoAd_MeasurementIdxType)0x01u
/**
 * Id of drop UDP measurement
 */
#define SOAD_MEAS_DROP_UDP (SoAd_MeasurementIdxType)0x02u

/**
 * Locate the line that close the socket for debug
 */
#define SOAD_SET_SOCKET_NEED_CLOSE_DEBUG STD_OFF
/* ================================================ type definitions ================================================ */

#if (0x100u > SOAD_SO_CON_MAX)
/**
 * @brief     Define socket connection id type
 */
typedef uint8 SoAd_SoConIdType;
/**
 * No tx metadata
 */
#define SOAD_NO_TXMETADATA 0xFFu
#else
typedef uint16 SoAd_SoConIdType;
#define SOAD_NO_TXMETADATA 0xFFFFu
#endif

/**
 * @brief     SoAd Routing Group identifier
 */
#if (0x100u > SOAD_ROUTING_GROUP_MAX)
typedef uint8 SoAd_RoutingGroupIdType;
#else
typedef uint16 SoAd_RoutingGroupIdType;
#endif

/**
 * @brief     SoAd Internal unknown type definition
 */
typedef uint16 SoAd_uintx;

/**
 * @brief     SoAd Measurement index type definition
 */
typedef uint8 SoAd_MeasurementIdxType;

/**
 * @brief     SoAd module initialization state
 */
typedef enum SoAd_Init_StateTypeTag
{
    SOAD_STATE_UNINIT, /**< Noinitialized state of SoAd module. @range [0] */
    SOAD_STATE_INIT    /**< Initialized state of SoAd module. @range [1] */
} SoAd_Init_StateType;

/**
 * @brief     SoAd socket connection mode
 */
typedef enum SoAd_SoConModeTypeTag
{
    SOAD_SOCON_ONLINE,    /**< Online mode of socket connection. @range [0] */
    SOAD_SOCON_RECONNECT, /**< Reconnect mode of socket connection. @range [1] */
    SOAD_SOCON_OFFLINE    /**< Offline mode of socket connection. @range [2] */
} SoAd_SoConModeType;

/**
 * @brief     SoAd support uplayer function table
 */
typedef struct SoAd_UpLayerFuncTypeTag
{
    void (*IfRxIndicationFunc)(
        PduIdType          RxPduId,
        const PduInfoType* PduInfoPtr); /**< Pointer to the receive indication function. @range NA */
    Std_ReturnType (*IfTriggerTransmitFunc)(
        PduIdType    TxPduId,
        PduInfoType* PduInfoPtr); /**< Pointer to the trigger transmit function. @range NA */
    void (*IfTxConfirmationFunc)(
        PduIdType      TxPduId,
        Std_ReturnType result); /**< Pointer to the transmit confirmation function. @range NA */
    BufReq_ReturnType (*TpStartOfReceptionFunc)(
        PduIdType          RxPduId,
        const PduInfoType* info,
        PduLengthType      TpSduLength,
        PduLengthType* bufferSizePtr); /**< Pointer to the transport protocol start of reception function. @range NA */
    BufReq_ReturnType (*TpCopyRxDataFunc)(
        PduIdType          id,
        const PduInfoType* info,
        PduLengthType* bufferSizePtr); /**< Pointer to the transport protocol copy receive data function. @range NA */
    void (*TpRxindicationFunc)(
        PduIdType      RxPduId,
        Std_ReturnType result); /**< Pointer to the transport protocol receive indication function. @range NA */
    BufReq_ReturnType (*TpCopyTxDataFunc)(
        PduIdType            id,
        const PduInfoType*   info,
        const RetryInfoType* retry,
        PduLengthType*
            availableDataPtr); /**< Pointer to the transport protocol copy transmit data function. @range NA */
    void (*TpTxConfirmationFunc)(
        PduIdType      TxPduId,
        Std_ReturnType result); /**< Pointer to the transport protocol transmit confirmation function. @range NA */
    void (*SoConModeChgFunc)(
        SoAd_SoConIdType   SoConId,
        SoAd_SoConModeType Mode); /**< Pointer to the socket connection mode change function. @range NA */
    void (*LocalIpAddrAssignmentChgFunc)(
        SoAd_SoConIdType      SoConId,
        TcpIp_IpAddrStateType State); /**< Pointer to the local IP address assignment change function. @range NA */
} SoAd_UpLayerFuncType;

/**
 * @brief     SoAd support uplayer configuration type definition
 */
typedef struct SoAd_PcCfgTypeTag
{
    SoAd_UpLayerFuncType UpLayerFuncTable[SOAD_SUPPORT_UPPERLAYMODULE_MAX]; /**< Array of upper layer function tables.
                                                                               @range SOAD_SUPPORT_UPPERLAYMODULE_MAX */
} SoAd_PcCfgType;

/**
 * @brief     SoAd support uplayer configuration type definition
 */
typedef enum SoAd_UpLayerModuleTypeTag
{
#if (STD_ON == SOAD_PDUR)
    SOAD_UP_MODULE_PDUR, /**< PduR uplayer Module. @range NA */
#endif
#if (STD_ON == SOAD_UDPNM)
    SOAD_UP_MODULE_UDPNM, /**< UdpNm uplayer Module. @range NA */
#endif
#if (STD_ON == SOAD_XCP)
    SOAD_UP_MODULE_XCP, /**< Xcp uplayer Module. @range NA */
#endif
#if (STD_ON == SOAD_SD)
    SOAD_UP_MODULE_SD, /**< Sd uplayer Module. @range NA */
#endif
#if (STD_ON == SOAD_DOIP)
    SOAD_UP_MODULE_DOIP, /**< DoIP uplayer Module. @range NA */
#endif
#if (STD_ON == SOAD_CDD)
    SOAD_UP_MODULE_CDD, /**< Cdd uplayer Module. @range NA */
#endif
#if (STD_ON == SOAD_CDD_DOIP_CLIENT)
    SOAD_UP_MODULE_CDD_DOIP_CLIENT, /**< Cdd uplayer Module. @range NA */
#endif
    SOAD_UP_MODULE_NONE, /**< unknown uplayer Module. @range NA */
} SoAd_UpLayerModuleType;

/**
 * @brief     SoAd socket connection buffer configuration type definition
 */
typedef struct SoAd_SoBufferCfgTypeTag
{
    PduLengthType BuffLen; /**< Buffer length. @range [0, 0xFFFFu] */
    PduLengthType BuffPos; /**< Buffer position. @range [0, 0xFFFFu]  */
} SoAd_SoBufferCfgType;

#if (STD_ON == SOAD_SUPPORT_UDP)

/**
 * @brief     SoAd socket connection UDP protocol configuration type definition
 */
typedef struct SoAd_SocketUdpTypeTag
{
    boolean SocketUdpListenOnly; /**< Flag indicating whether the UDP socket listens only. @range [0, 1] */
    boolean SocketUdpStrictHeaderLenCheckEnabled; /**< Flag indicating whether strict header length check is enabled for
                                                     UDP. @range [0, 1]  */
    uint32 SocketUdpAliveSupervisionTimeout;      /**< UDP alive supervision timeout. @range [0, 0xFFFFFFFFU] */
#if (SOAD_UDP_SOCON_IMMEDIATE_TXCONFIRMATION == STD_ON)
    boolean SocketUdpImmediateTxConfirmEnabled; /**< Flag indicating whether immediate transmit confirmation is enabled
                                                   for UDP. @range [0, 1] */
#endif                                          /* SOAD_UDP_SOCON_IMMEDIATE_TXCONFIRMATION == STD_ON */
} SoAd_SocketUdpType;
#endif

#if (STD_ON == SOAD_SUPPORT_TCP)
/**
 * @brief     SoAd socket connection TCP protocol configuration type definition
 */
typedef struct SoAd_SocketTcpTypeTag
{
    boolean SocketTcpImmediateTpTxConfirmation; /**< Flag indicating whether immediate transport protocol transmit
                                                   confirmation is enabled for TCP. @range [0, 1] */
    boolean SocketTcpInitiate;  /**< Flag indicating whether the TCP socket initiates connections. @range [0, 1] */
    boolean SocketTcpKeepAlive; /**< Flag indicating whether TCP keep-alive is enabled. @range [0, 1] */
    uint32  SocketTcpKeepAliveInterval;  /**< TCP keep-alive interval. @range [0, 0xFFFFFFFFU] */
    uint16  SocketTcpKeepAliveProbesMax; /**< Maximum number of TCP keep-alive probes. @range [0, 0xFFFFu] */
    uint32  SocketTcpKeepAliveTime;      /**< TCP keep-alive time. @range [0, 0xFFFFFFFFU] */
    uint8   SocketTcpNoDelay;            /**< Flag indicating whether TCP no-delay is enabled. @range [0, 0xFFU] */
    uint32  SocketTcpTxQuota;            /**< TCP transmit quota. @range [0, 0xFFFFFFFFU] */
    uint8   SocketTCPOptionFilterId;     /**< TCP option filter ID. @range [0, 0xFFU] */
    uint16  SocketTcpTlsConnectionId;    /**< TLS connection ID for TCP. @range [0, 0xFFFFu] */
} SoAd_SocketTcpType;
#endif

/**
 * @brief     SoAd socket connection protocol configuration type definition
 */
typedef struct SoAd_SocketProtocolTypeTag
{
#if (STD_ON == SOAD_SUPPORT_TCP)
    const SoAd_SocketTcpType* const SocketTcpPtr; /**< Pointer to the TCP socket configuration. @range NA */
#endif
#if (STD_ON == SOAD_SUPPORT_UDP)
    const SoAd_SocketUdpType* const SocketUdpPtr; /**< Pointer to the UDP socket configuration. @range NA */
#endif
} SoAd_SocketProtocolType;

/**
 * @brief     SoAd socket connection reference address configuration type definition
 */
typedef struct SoAd_SocketLocalAddressRefTypeTag
{
    TcpIp_DomainType DomainType; /**< Domain type for the socket local address. @range [TCPIP_AF_INET/TCPIP_AF_INET6] */
    TcpIp_LocalAddrIdType LocalAddrId; /**< Local address ID for the socket. @range [0, 0xFFU]  */
} SoAd_SocketLocalAddressRefType;

/**
 * @brief     SoAd socket connection configuration type definition
 */
typedef struct SoAd_SocketConnectionTypeTag
{
    SoAd_uintx SocketConnectionGroupRef; /**< Reference to the socket connection group. @range [0, 35535] */
    const TcpIp_SockAddrType* const SocketRemoteAddress; /**< Pointer to the remote address for the socket. @range NA */
    uint8                           RefUpNum;            /**< Number of upper layer references. @range [0, 0xFFU] */
    const SoAd_UpLayerModuleType* const RefUpModulePtr;  /**< Pointer to the upper layer module types. @range NA */
    SoAd_uintx              RefSocketRouteNum;           /**< Number of socket routes referenced. @range [0, 0xFFFFu] */
    const SoAd_uintx* const RefSocketRoutePtr;           /**< Pointer to the socket route indices. @range NA */
    const SoAd_uintx* const TxBuffCfgIndexPtr; /**< Pointer to the transmit buffer configuration indices. @range NA */
    const SoAd_uintx* const RxBuffCfgIndexPtr; /**< Pointer to the receive buffer configuration indices. @range NA */
    TcpIp_ProtocolType TcpUdpProtocol; /**< Protocol type (TCP or UDP). @range [TCPIP_IPPROTO_TCP/TCPIP_IPPROTO_UDP] */
    SoAd_uintx         RefPduRouteDestNum;      /**< Number of PDU route destinations referenced. @range [0, 0xFFFFu] */
    const SoAd_uintx* const RefPduRouteDestPtr; /**< Pointer to the PDU route destination indices. @range NA */
#if (SOAD_PARTITION_NUM > 1u)
    SoAd_SoConIdType PartitionSoConId; /**< Socket connection id of partition. @range [0, 0xFFu/0xFFFFu] */
#endif
} SoAd_SocketConnectionType;

/**
 * @brief     SoAd socket connection group configuration type definition
 */
typedef struct SoAd_SocketConnectionGroupTypeTag
{
    uint16  PartitionId;               /**< Partition ID for the socket connection group. @range [0, 0xFFFFu] */
    boolean PduHeaderEnable;           /**< Flag indicating whether PDU header is enabled. @range [0, 1] */
    boolean SocketAutomaticSoConSetup; /**< Flag indicating whether automatic socket connection setup is enabled. @range
                                          [0, 1] */
    boolean SocketMsgAcceptanceFilterEnabled; /**< Flag indicating whether message acceptance filter is enabled for the
                                                 socket. @range [0, 1] */
    boolean SocketIpAddrAssignmentChgNotification; /**< Flag indicating whether IP address assignment change
                                                      notification is enabled. @range [0, 1] */
    boolean SocketSoConModeChgNotification;   /**< Flag indicating whether socket connection mode change notification is
                                                 enabled. @range [0, 1] */
    uint16 SocketLocalPort;                   /**< Local port number for the socket. @range [0, 0xFFFFu] */
    uint8  SocketFramePriority;               /**< Frame priority for the socket. @range [0, 0xFFU] */
    uint8  SocketDifferentiatedServicesField; /**< Differentiated services field for the socket. @range [0, 0xFFU] */
    uint8  SocketPathMTUEnable;     /**< Flag indicating whether path MTU discovery is enabled. @range [0, 0xFFU] */
    uint32 SocketFlowLabel;         /**< Flow label for the socket. @range [0, 0xFFFFFFFFu] */
    SoAd_uintx SocketTpRxBufferMin; /**< Minimum transport protocol receive buffer size. @range [0, 0xFFU] */
    SoAd_SocketLocalAddressRefType SocketLocalAddressRef; /**< Local address reference for the socket. @range reference
                                                             to SoAd_SocketLocalAddressRefType. @range [0, 0xFFFFu] */
    SoAd_SocketProtocolType SocketProtocol;               /**< Socket protocol configuration. @range reference to
                                                             SoAd_SocketProtocolType. @range reference to SoAd_SocketProtocolType */
    SoAd_SoConIdType SocketConNum;                        /**< Number of socket connections. @range [0, 0xFFFFu] */
    SoAd_SoConIdType SocketConStartId;                    /**< Start ID for socket connections. @range [0, 0xFFFFu] */
#if (SOAD_SOCKET_VLAN == STD_ON)
    uint32 SocketVlan; /**< VLAN ID for the socket. @range [0, 0xFFFFu] */
#endif
#if (SOAD_PARTITION_NUM > 1u)
    SoAd_uintx PartitionSoConGroupId; /**< Socket connection group id of partition. @range [0, 0xFFu/0xFFFFu] */
#endif
} SoAd_SocketConnectionGroupType;

/**
 * @brief     SoAd support uplayer type definition
 */
typedef enum SoAd_UpperLayerTypeTag
{
    SOAD_UPPER_LAYER_IF, /**< Upper layer type: Interface. @range [0] */
    SOAD_UPPER_LAYER_TP  /**< Upper layer type: Transport Protocol. @range [1] */
} SoAd_UpperLayerType;

/**
 * @brief     SoAd socket route dest type definition
 */
typedef struct SoAd_SocketRouteDestTypeTag
{
    SoAd_UpperLayerType
        RxUpperLayerType; /**< Upper layer type for the receiving side. @range reference to SoAd_UpperLayerType */
    SoAd_RoutingGroupIdType
        RxRoutingGroupRefNum; /**< Number of routing group references for the receiving side. @range [0, 0xFFFFu] */
    const SoAd_RoutingGroupIdType* const
        RxRoutingGroupRefPtr; /**< Pointer to the routing group reference indices for the receiving side. @range NA */
    SoAd_UpLayerModuleType
              RefUpModule; /**< Upper layer module type reference. @range reference to SoAd_UpLayerModuleType */
    PduIdType UpRxPduId;   /**< Upper layer receive PDU ID. @range [0, 0xFFFFu] */
#if (STD_ON == SOAD_PDU_ROUTE_METADATA_SUPPORT)
    boolean MetaDataEnable; /**< Flag indicating whether metadata is enabled. @range [0,1] */
#endif
    PduIdType SocketRouteIndex; /**< Socket route index. @range [0, 0xFFu/0xFFFFu] */
#if (SOAD_PARTITION_NUM > 1u)
    PduIdType PartitionSocketRouteDestId; /**< Socket route dest id of partition. @range [0, 0xFFu/0xFFFFu] */
#endif
} SoAd_SocketRouteDestType;

/**
 * @brief     SoAd socket route type definition
 */
typedef struct SoAd_SocketRouteTypeTag
{
    uint16 PartitionId; /**< Partition ID for the socket route. @range [0, 0xFFFFu] */
#if (STD_ON == SOAD_SUPPORT_HEADERID)
    const uint32* const RxPduHeaderId; /**< Pointer to the receive PDU header ID . @range NA*/
#endif
    SoAd_SoConIdType
        RxSocketConnRefNum; /**< Number of socket connection references for the receiving side. @range [0, 0xFFFFu] */
    const SoAd_SoConIdType* const RxSocketConnRefIndexPtr; /**< Pointer to the socket connection reference indices for
                                                              the receiving side. @range NA */
} SoAd_SocketRouteType;

/**
 * @brief     SoAd transmission Udp Trigger Mode type definition
 */
typedef enum SoAd_TriggerModeTypeTag
{
    SOAD_TRIGGER_ALWAYS, /**< Trigger mode: Always. @range [0] */
    SOAD_TRIGGER_NEVER,  /**< Trigger mode: Never. @range [1] */
    SOAD_TRIGGER_UNSET   /**< Trigger mode: Unset. @range [2] */
} SoAd_TriggerModeType;

/**
 * @brief     SoAd Pdu route dest type definition
 */
typedef struct SoAd_PduRouteDestTypeTag
{
    uint16 PartitionId; /**< Partition ID for the PDU route destination. @range [0, 0xFFFFu] */
#if (STD_ON == SOAD_SUPPORT_HEADERID)
    const uint32* const TxPduHeaderId; /**< Pointer to the transmit PDU header ID . @range NA*/
#endif
#if (STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER)
    SoAd_TriggerModeType TxUdpTriggerMode;     /**< UDP transmit trigger mode. @range refer to SoAd_TriggerModeType */
    uint32               CfgUdpTriggerTimeout; /**< UDP trigger timeout configuration. @range [0, 0xFFFFFFFFu] */
#endif
    SoAd_RoutingGroupIdType
        TxRoutingGroupRefNum; /**< Number of routing group references for the transmitting side. @range [0, 0xFFu] */
    const SoAd_RoutingGroupIdType* const TxRoutingGroupRefPtr; /**< Pointer to the routing group reference indices for
                                                                  the transmitting side. @range [0, NA] */
    SoAd_SoConIdType TxPduRouteDestSocketConRefNum;       /**< Number of socket connection references for the PDU route
                                                             destination. @range [0, 0xFFu] */
    const SoAd_SoConIdType* const TxSocketConRefIndexPtr; /**< Pointer to the socket connection reference indices for
                                                             the PDU route destination. @range NA */
    PduIdType PduRouteIndex;                              /**< PDU route index. @range [0, 0xFFu/0xFFFFu] */
#if (SOAD_PARTITION_NUM > 1u)
    PduIdType PartitionPduRouteDestId; /**< PDU route dest index of partition. @range [0, 0xFFu/0xFFFFu] */
#endif
} SoAd_PduRouteDestType;

/**
 * @brief     SoAd Pdu route type definition
 */
typedef struct SoAd_PduRouteTypeTag
{
    uint16 PartitionId; /**< Partition ID for the PDU route. @range [0, 0xFFFFu] */
    SoAd_UpperLayerType
        TxUpperLayerType; /**< Upper layer type for the transmitting side. @range reference SoAd_UpperLayerType */
    SoAd_uintx       PduRouteDestNum;     /**< Number of PDU route destinations. @range [0, 0xFFFFu] */
    const SoAd_uintx PduRouteDestStartId; /**< Start ID for PDU route destinations. @range [0, 0xFFFFu] */
    SoAd_UpLayerModuleType
              RefUpModule; /**< Upper layer module type reference. @range reference SoAd_UpLayerModuleType */
    PduIdType UpPduId;     /**< Upper layer PDU ID. @range [0, 0xFFu/0xFFFFu] */
#if (STD_ON == SOAD_PDU_ROUTE_METADATA_SUPPORT)
    boolean MetaDataEnable; /**< Flag indicating whether metadata is enabled. @range [0, 1] */
#endif
#if (SOAD_UDP_SOCON_IMMEDIATE_TXCONFIRMATION == STD_ON)
    boolean UDPImmediateTxConfirmation; /**< Flag indicating whether immediate UDP transmit confirmation is enabled.
                                           @range [0, 1] */
#endif
#if (SOAD_PARTITION_NUM > 1u)
    PduIdType PartitionPduRouteId; /**< Pdu route of partition. @range [0, 0xFFu/0xFFFFu] */
#endif
} SoAd_PduRouteType;

/**
 * @brief     SoAd routing group type definition
 */
typedef struct SoAd_RoutingGroupTypeTag
{
    uint16  PartitionId;                 /**< Partition ID for the routing group. @range [0, 0xFFFFu] */
    boolean RoutingGroupIsEnabledAtInit; /**< Flag indicating whether the routing group is enabled at initialization.
                                            @range [0, 1] */
    boolean RoutingGroupTxTriggerable;   /**< Flag indicating whether the routing group is transmit triggerable. @range
                                            [0, 1] */
    SoAd_uintx              PduRouteDestNum;      /**< Number of PDU route destinations. @range [0, 0xFFFFu] */
    const SoAd_uintx* const PduRouteDestIndexPtr; /**< Pointer to the PDU route destination indices. @range NA */
    SoAd_uintx              SocketRouteDestNum;   /**< Number of socket route destinations. @range [0, 0xFFFFu] */
    const SoAd_uintx* const
        SocketRouteDestIndexPtr; /**< Pointer to the socket route destination indices. @range [0, 1] */
#if (SOAD_PARTITION_NUM > 1u)
    SoAd_RoutingGroupIdType PartitionRoutGroupId; /**< Routing group id of partition. @range [0, 0xFFu/0xFFFFu] */
#endif
} SoAd_RoutingGroupType;

/**
 * @brief     SoAd multiple partition infomation type definition
 */
typedef struct SoAd_MultiPartitionInfoTypeTag
{
    SoAd_SoConIdType SoConStartId; /**< Start ID for socket connections in multiple partitions. @range [0, 0xFFu] */
    SoAd_SoConIdType SoConEndId;   /**< End ID for socket connections in multiple partitions. @range [0, 0xFFu] */
    SoAd_uintx
        SoConGroupStartId; /**< Start ID for socket connection groups in multiple partitions. @range [0, 0xFFFFu] */
    SoAd_uintx SoConGroupEndId; /**< End ID for socket connection groups in multiple partitions. @range [0, 0xFFFFu] */
    SoAd_RoutingGroupIdType
        RoutGroupStartId; /**< Start ID for routing groups in multiple partitions. @range [0, 0xFFu] */
    SoAd_RoutingGroupIdType RoutGroupEndId; /**< End ID for routing groups in multiple partitions. @range [0, 0xFFu] */
    SoAd_uintx SocketRouteDestStartId; /**< Start ID for socket route destinations in multiple partitions. @range [0,
                                          0xFFFFu] */
    SoAd_uintx
        SocketRouteDestEndId; /**< End ID for socket route destinations in multiple partitions. @range [0, 0xFFFFu] */
    SoAd_uintx
        PduRouteDestStartId; /**< Start ID for PDU route destinations in multiple partitions. @range [0, 0xFFFFu] */
    SoAd_uintx PduRouteDestEndId; /**< End ID for PDU route destinations in multiple partitions. @range [0, 0xFFFFu] */
    SoAd_uintx
        SocketTxBuffStartId; /**< Start ID for socket transmit buffers in multiple partitions. @range [0, 0xFFFFu] */
    SoAd_uintx SocketTxBuffEndId; /**< End ID for socket transmit buffers in multiple partitions. @range [0, 0xFFFFu] */
    SoAd_uintx
        SocketRxBuffStartId; /**< Start ID for socket receive buffers in multiple partitions. @range [0, 0xFFFFu] */
    SoAd_uintx SocketRxBuffEndId; /**< End ID for socket receive buffers in multiple partitions. @range [0, 0xFFFFu] */
    PduIdType  PduRoutStartId;    /**< Start ID for PDU routes in multiple partitions. @range [0, 0xFFu/0xFFFFu] */
    PduIdType  PduRoutEndId;      /**< End ID for PDU routes in multiple partitions. @range [0, 0xFFu/0xFFFFu] */
} SoAd_MultiPartitionInfoType;

/**
 * @brief     SoAd multiple partition infomation type definition
 */
typedef struct SoAd_SoAd_PartitionInfoTypeTag
{
    uint16 ApplicationId;  /**< Application ID for the partition. @range [0, 0xFFFFu] */
    uint8  PartitionIndex; /**< Partition index. @range [0, 0xFFu] */
} SoAd_PartitionInfoType;

/**
 * @brief     SoAd socket close type definition
 */
typedef enum SoAd_SocketCloseTypeTag
{
    SOAD_SOCKET_CLOSE_NONE,      /**< Socket close type: None. @range [0] */
    SOAD_SOCKET_CLOSE_IMMEDIATE, /**< Socket close type: Immediate. @range [1] */
    SOAD_SOCKET_CLOSE_DEFERRED   /**< Socket close type: Deferred. @range [2] */
} SoAd_SocketCloseType;

/**
 * @brief     SoAd socket connection request type definition
 */
typedef enum SoAd_SoConRequestTypeTag
{
    SOAD_SOCON_REQUEST_NONE, /**< Socket connection request type: None. @range [0] */
    SOAD_SOCON_REQUEST_OPEN, /**< Socket connection request type: Open. @range [1] */
    SOAD_SOCON_REQUEST_CLOSE /**< Socket connection request type: Close. @range [2] */
} SoAd_SoConRequestType;

/**
 * @brief     SoAd socket connection manage type definition
 */
typedef struct SoAd_InnerSocketConManageTypeTag
{
    boolean SoConCloseAbort;         /**< Flag indicating whether socket connection close is aborted. @range [0, 1] */
    boolean RemoteAddrIsSet;         /**< Flag indicating whether the remote address is set. @range [0, 1] */
    TcpIp_SockAddrType   RemoteAddr; /**< Remote address for the socket connection. @range ref TcpIp_SockAddrType */
    boolean              TcpIpSocketValid; /**< Flag indicating whether the TCP/IP socket is valid. @range [0, 1] */
    SoAd_SocketCloseType SocketNeedClose;  /**< Socket close type needed. @range ref SoAd_SocketCloseType */
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
    uint32 SetSocketNeedCloseLine; /**< Line number where socket needs to be closed (debug). @range [0, 0xFFFFFFFFu] */
#endif
    SoAd_SoConModeType SoConMode;           /**< Socket connection mode. @range ref SoAd_SoConModeType */
    /* PRQA S 3132 ++*/                     /* VL_QAC_MagicNum */
    SoAd_SoConRequestType SoConRequest[2u]; /**< Socket connection requests. @range ref SoAd_SoConRequestType */
    /* PRQA S 3132 --*/
    PduIdType ActiveSoAdPduRouteId; /**< Active SoAd PDU route ID. @range [0, 0xFFu] */
#if (STD_ON == SOAD_SUPPORT_UDP)
    uint32 UdpAliveSupervisionTime; /**< UDP alive supervision time. @range [0, 0xFFFFFFFFu] */
#endif
    TcpIp_SocketIdType TcpIpSocketId;     /**< TCP/IP socket ID. @range [0, 0xFFu] */
    PduLengthType      TxNoAckLength;     /**< Length of transmitted data not acknowledged. @range [0, 0xFFFFu] */
    boolean            TpPduTransmitting; /**< Flag indicating whether a TP PDU is being transmitted. @range [0, 1] */
    boolean            RemoteAddrNeedRelease; /**< Release flag for remote addresses. @range [0, 1] */
    boolean ChangeModeToOffline; /**< Flag indicating whether the mode should change to offline. @range [0, 1] */
    boolean RxIfProcessing;      /**< Flag indicating whether receive interface processing is ongoing. @range [0, 1] */
} SoAd_InnerSocketConManageType;

/**
 * @brief     SoAd socket connection group manage type definition
 */
typedef struct SoAd_InnerSocketConGroupManageTypeTag
{
    boolean               TcpIpSocketValid;    /**< Flag indicating whether the TCP/IP socket is valid. @range [0, 1] */
    TcpIp_IpAddrStateType LocalIpAddrState;    /**< State of the local IP address. @range [0, 0xFFu] */
    TcpIp_SocketIdType    TcpIpSocketId;       /**< TCP/IP socket ID. @range [0, 0xFFu] */
    SoAd_SoConIdType      SoConSocketValidNum; /**< Number of valid socket connections. @range [0, 0xFFu] */
} SoAd_InnerSocketConGroupManageType;

/**
 * @brief     SoAd routing group manage type definition
 */
typedef struct SoAd_SoAd_InnerRoutingGroupManageTypeTag
{
    boolean RoutGroupEnable; /**< Flag indicating whether the routing group is enabled. @range [0, 1] */
} SoAd_InnerRoutingGroupManageType;

/**
 * @brief     SoAd PDU route dest manage type definition
 */
typedef struct SoAd_InnerPduRouteDestManagerTypeTag
{
    boolean PduRouteDestEnable;    /**< Flag indicating whether the PDU route destination is enabled. @range [0, 1] */
    PduLengthType TxPendingLength; /**< Length of pending transmit data. @range [0, 1] */
} SoAd_InnerPduRouteDestManagerType;

/**
 * @brief     SoAd socket route dest manage type definition
 */
typedef struct SoAd_InnerSocketRouteDestManagerTypeTag
{
    boolean
        SocketRouteDestEnable; /**< Flag indicating whether the socket route destination is enabled. @range [0, 1] */
} SoAd_InnerSocketRouteDestManagerType;

/**
 * @brief     SoAd PDU route manage type definition
 */
typedef struct SoAd_InnerPduRoutManageTypeTag
{
    uint8            TxPendingNum; /**< Number of pending transmits. @range [0, 0xFFu] */
    uint8*           PduDataPtr;   /**< Pointer to PDU data. @range NA */
    PduLengthType    PduLength;    /**< Length of PDU data. @range [0, 0xFFFFu] */
    SoAd_SoConIdType TxSoConId;    /**< Socket connection ID for transmit. @range [0, 0xFFu] */
} SoAd_InnerPduRoutManageType;

/**
 * @brief     SoAd socket connection transmission buffer manage type definition
 */
typedef struct SoAd_SoTxBufManTypeTag
{
    PduLengthType TxPduTotalLength; /**< Total length of transmitted PDU. @range [0, 0xFFFFu] */
    PduLengthType TxResidueLength;  /**< Remaining length of data to be transmitted. @range [0, 0xFFFFu] */
    PduLengthType UpCopyLength;     /**< Length of data copied from upper layer. @range [0, 0xFFFFu] */
#if (STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER)
    uint32 TriggerTimer; /**< Trigger timer for UDP transmit buffer. @range [0, 0xFFFFFFFFu] */
#endif
} SoAd_SoTxBufManType;

/**
 * @brief     SoAd socket connection receive buffer manage type definition
 */
typedef struct SoAd_SoRxBufferManTypeTag
{
    PduLengthType EnterPosition;        /**< Entry position in the receive buffer. @range [0, 0xFFFFu] */
    PduLengthType ExitPosition;         /**< Exit position in the receive buffer. @range [0, 0xFFFFu] */
    PduLengthType ExitPduResidueLength; /**< Remaining length of PDU data at exit position. @range [0, 0xFFFFu] */
    SoAd_uintx    SocketRouteId;        /**< Socket route ID. @range [0, 0xFFFFu] */
} SoAd_SoRxBufferManType;

/**
 * @brief     SoAd configuration data structure type definition
 */
typedef struct SoAd_ConfigTypeTag
{
    const SoAd_uintx                       PduRouteNum;         /**< Number of PDU routes. @range [0, 0xFFFFu] */
    const SoAd_PduRouteType* const         PduRoute;            /**< Array of PDU routes. @range NA */
    const SoAd_RoutingGroupIdType          RoutingGroupNum;     /**< Number of routing groups. @range [0, 0xFFu] */
    const SoAd_RoutingGroupType* const     RoutingGroup;        /**< Array of routing groups. @range NA */
    const SoAd_SoConIdType                 SocketConnectionNum; /**< Number of socket connections. @range [0, 0xFFu] */
    const SoAd_SocketConnectionType* const SocketConnection;    /**< Array of socket connections. @range NA */
    const SoAd_uintx SocketConnectionGroupNum; /**< Number of socket connection groups. @range [0, 0xFFFFu] */
    const SoAd_SocketConnectionGroupType* const
                                      SocketConnectionGroup; /**< Array of socket connection groups. @range NA */
    const SoAd_uintx                  SocketRouteNum;        /**< Number of socket routes. @range [0, 0xFFFFu] */
    const SoAd_SocketRouteType* const SocketRoute;           /**< Array of socket routes. @range NA */
    const SoAd_uintx SocketRouteDestTotalNum; /**< Total number of socket route destinations. @range [0, 0xFFFFu] */
    const SoAd_SocketRouteDestType* const SocketRouteDestTotalPtr; /**< Array of socket route destinations. @range NA */
    const SoAd_uintx PduRouteDestTotalNum; /**< Total number of PDU route destinations. @range [0, 0xFFFFu] */
    const SoAd_PduRouteDestType* const PduRouteDestTotalPtr; /**< Array of PDU route destinations. @range NA */
    const SoAd_uintx SocketBufferCfgNum; /**< Number of socket buffer configurations. @range [0, 0xFFFFu] */
    const SoAd_SoBufferCfgType* const SocketBufferCfgPtr; /**< Array of socket buffer configurations. @range NA */
#if (SOAD_PARTITION_NUM > 1u)
    SoAd_InnerSocketConManageType** const
        SoConManagerPtr; /**< Array of pointers to socket connection managers. @range NA */
    SoAd_InnerSocketConGroupManageType** const
        SoConGroupManagerPtr; /**< Array of pointers to socket connection group managers. @range NA */
#if (0u < SOAD_ROUTING_GROUP_MAX)
    SoAd_InnerRoutingGroupManageType** const
        RoutingGroupManagerPtr; /**< Array of pointers to routing group managers. @range NA */
#endif
#if (0u < SOAD_SOCKET_ROUTE_DEST_MAX)
    SoAd_InnerSocketRouteDestManagerType** const
        SocketRouteDestManagerPtr; /**< Array of pointers to socket route destination managers. @range NA */
#endif
#if (0u < SOAD_PDU_ROUTE_DEST_MAX)
    SoAd_InnerPduRouteDestManagerType** const
        PduRouteDestManagerPtr; /**< Array of pointers to PDU route destination managers. @range NA */
#endif
    const SoAd_uintx SocketTxBufferNum; /**< Number of socket transmit buffers. @range [0, 0xFFFFu] */
#if (STD_ON == SOAD_SUPPORT_TX_BUFFER)
    SoAd_SoTxBufManType** const
        SocketTxBufManPtr; /**< Array of pointers to socket transmit buffer managers. @range NA */
#endif
    const SoAd_uintx SocketRxBufferNum; /**< Number of socket receive buffers. @range [0, 0xFFFFu] */
#if (STD_ON == SOAD_SUPPORT_RX_BUFFER)
    SoAd_SoRxBufferManType** const
        SocketRxBufManPtr; /**< Array of pointers to socket receive buffer managers. @range NA */
#endif
#if (0u < SOAD_PDU_ROUTE_MAX)
    SoAd_InnerPduRoutManageType** const PduRoutManagerPtr; /**< Array of pointers to PDU route managers. @range NA */
#endif
#else
    SoAd_InnerSocketConManageType* SoConManagerPtr; /**< Array of pointers to socket connection managers. @range NA */
    SoAd_InnerSocketConGroupManageType*
        SoConGroupManagerPtr; /**< Array of pointers to socket connection group managers. @range NA */
#if (0u < SOAD_ROUTING_GROUP_MAX)
    SoAd_InnerRoutingGroupManageType*
        RoutingGroupManagerPtr; /**< Array of pointers to routing group managers. @range NA */
#endif
#if (0u < SOAD_SOCKET_ROUTE_DEST_MAX)
    SoAd_InnerSocketRouteDestManagerType*
        SocketRouteDestManagerPtr; /**< Array of pointers to socket route destination managers. @range NA */
#endif
#if (0u < SOAD_PDU_ROUTE_DEST_MAX)
    SoAd_InnerPduRouteDestManagerType*
        PduRouteDestManagerPtr; /**< Array of pointers to PDU route destination managers. @range NA */
#endif
    const SoAd_uintx SocketTxBufferNum; /**< Number of socket transmit buffers. @range [0, 0xFFFFu] */
#if (STD_ON == SOAD_SUPPORT_TX_BUFFER)
    SoAd_SoTxBufManType* SocketTxBufManPtr; /**< Array of pointers to socket transmit buffer managers. @range NA */
#endif
    const SoAd_uintx SocketRxBufferNum; /**< Number of socket receive buffers. @range [0, 0xFFFFu] */
#if (STD_ON == SOAD_SUPPORT_RX_BUFFER)
    SoAd_SoRxBufferManType* SocketRxBufManPtr; /**< Array of pointers to socket receive buffer managers. @range NA */
#endif
#if (0u < SOAD_PDU_ROUTE_MAX)
    SoAd_InnerPduRoutManageType* PduRoutManagerPtr; /**< Array of pointers to PDU route managers. @range NA */
#endif
#endif
} SoAd_ConfigType;

#ifdef __cplusplus
}
#endif

#endif
