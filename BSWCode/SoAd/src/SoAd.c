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
 **  @file               : SoAd.c
 **  @author             : zhengfei.li
 **  @date               : 2020/05/07
 **  @vendor             : isoft
 **  @description        : Socket Adaptor
 **
 ***********************************************************************************************************************/
/* PRQA S 6540, 6520 EOF */ /* VL_MTR_SoAd_STTPP, VL_MTR_SoAd_STVAR */
/* =================================================== inclusions =================================================== */
#include "IStdLib.h"
#include "SoAd_Internal.h"
#include "SoAd.h"
#include "SoAd_Cbk.h"
#include "Det.h"

/* ===================================================== macros ===================================================== */

/**
 * Defines the local variable or function scope as static.
 */
#if !defined(SOAD_LOCAL)
#define SOAD_LOCAL static /* PRQA S 3414 */ /* VL_QAC_3414 */
#endif

/**
 * Defines the local inline function scope as static inline.
 */
#if !defined(SOAD_LOCAL_INLINE)
#define SOAD_LOCAL_INLINE static inline
#endif

/**
 * Defines the DHCPv4 client FQDN option code.
 */
#define SOAD_DCHP4_CLIENT_FQDN 81U
/**
 * Defines the DHCPv6 client FQDN option code.
 */
#define SOAD_DCHP6_CLIENT_FQDN 39U

/**
 * Defines the size of the PDU header data.
 */
#define SOAD_HEADER_DATA_SIZE (4UL)
/**
 * Defines the size of the PDU header length.
 */
#define SOAD_HEADER_LENGTH_SIZE (4UL)

#if (STD_ON == SOAD_DEV_ERROR_DETECT)
/**
 * @brief         report detect of the SoAd module
 * @param[in]     ApiId: Identifier of the PDU to be transmitted
 * @param[in]     ErrorId: Length of and pointer to the PDU data and pointer to MetaData.
 * @return        none
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74655
 */
SOAD_LOCAL_INLINE void SOAD_DET_REPORT(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE, ApiId, ErrorId);
}
#endif
/* BigEndian */
#if (STD_ON == SOAD_SUPPORT_HEADERID)
/**
 * @brief         insert the PDU Header with the configured HeaderId and the actual PDU length directly before
 *                the PDU data
 * @param[in,out] buffer: Pointer to the buffer where the header will be inserted
 * @param[in]     headerId: The header ID to be inserted
 * @param[in]     length: The length of the PDU data
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @return        none
 * @trace       CPD-74656
 */
SOAD_LOCAL_INLINE void SOAD_FILL_HEADER(uint8* buffer, uint32 headerId, uint16 length)
{
    (buffer)[SOAD_DATA_OFFSET_0] = (uint8)((headerId) >> SOAD_BIT_24);
    (buffer)[SOAD_DATA_OFFSET_1] = (uint8)((headerId) >> SOAD_BIT_16);
    (buffer)[SOAD_DATA_OFFSET_2] = (uint8)((headerId) >> SOAD_BIT_8);
    (buffer)[SOAD_DATA_OFFSET_3] = (uint8)(headerId);
    (buffer)[SOAD_DATA_OFFSET_4] = (uint8)(((uint32)(length)) >> SOAD_BIT_24);
    (buffer)[SOAD_DATA_OFFSET_5] = (uint8)(((uint32)(length)) >> SOAD_BIT_16);
    (buffer)[SOAD_DATA_OFFSET_6] = (uint8)(((uint32)(length)) >> SOAD_BIT_8);
    (buffer)[SOAD_DATA_OFFSET_7] = (uint8)((uint32)(length));
}

/**
 * @brief         Retrieves the header ID from the buffer.
 * @param[in]     dataPtr: Pointer to the buffer containing the header.
 * @param[in]     bufStartId: Starting index in the buffer where the header begins.
 * @return        The header ID extracted from the buffer.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74657
 */
SOAD_LOCAL_INLINE uint32 SOAD_GET_HEADER(const uint8* dataPtr, uint32 bufStartId)
{
    return (
        ((uint32)(dataPtr)[bufStartId] << SOAD_BIT_24)
        | ((uint32)(dataPtr)[bufStartId + SOAD_DATA_OFFSET_1] << SOAD_BIT_16)
        | ((uint32)(dataPtr)[bufStartId + SOAD_DATA_OFFSET_2] << SOAD_BIT_8)
        | ((uint32)(dataPtr)[bufStartId + SOAD_DATA_OFFSET_3]));
}
/**
 * @brief         Retrieves the length of the PDU data from the buffer.
 * @param[in]     dataPtr: Pointer to the buffer containing the length.
 * @param[in]     bufStartId: Starting index in the buffer where the length begins.
 * @return        The length of the PDU data extracted from the buffer.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74658
 */
SOAD_LOCAL_INLINE uint32 SOAD_GET_LENGTH(const uint8* dataPtr, uint32 bufStartId)
{
    return (
        ((uint32)(dataPtr)[bufStartId + SOAD_DATA_OFFSET_4] << SOAD_BIT_24)
        | ((uint32)(dataPtr)[bufStartId + SOAD_DATA_OFFSET_5] << SOAD_BIT_16)
        | ((uint32)(dataPtr)[bufStartId + SOAD_DATA_OFFSET_6] << SOAD_BIT_8)
        | ((uint32)(dataPtr)[bufStartId + SOAD_DATA_OFFSET_7]));
}
#endif

/**
 * @brief         Checks if the given IP address is any (0.0.0.0).
 * @param[in]     IpAddr: The IP address to check.
 * @return        TRUE if the IP address is any, FALSE otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74659
 */
SOAD_LOCAL_INLINE boolean SOAD_IS_IPADDR_ANY(TcpIp_SockAddrType IpAddr)
{
    return (IpAddr.addr[0u] == 0x00000000uL);
}
/**
 * @brief         Compares two IP addresses for equality.
 * @param[in]     IpAddr1: The first IP address.
 * @param[in]     IpAddr2: The second IP address.
 * @return        TRUE if the IP addresses are equal, FALSE otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74660
 */
SOAD_LOCAL_INLINE boolean SOAD_EQ_IPADDR(TcpIp_SockAddrType IpAddr1, TcpIp_SockAddrType IpAddr2)
{
    return (IpAddr1.addr[0u] == IpAddr2.addr[0u]);
}

/**
 * @brief         Encodes a 32-bit parameter into a buffer in big-endian format.
 * @param[in,out] buffer: Pointer to the buffer where the parameter will be encoded.
 * @param[in]     paramenter: The 32-bit parameter to encode.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74661
 */
SOAD_LOCAL_INLINE void SOAD_TCPIP_CHG_PARA_ENCODE_32(uint8* buffer, uint32 paramenter)
{
    (buffer)[SOAD_DATA_OFFSET_0] = (uint8)((paramenter) >> SOAD_BIT_24);
    (buffer)[SOAD_DATA_OFFSET_1] = (uint8)((paramenter) >> SOAD_BIT_16);
    (buffer)[SOAD_DATA_OFFSET_2] = (uint8)((paramenter) >> SOAD_BIT_8);
    (buffer)[SOAD_DATA_OFFSET_3] = (uint8)(paramenter);
}

/**
 * @brief         Encodes a 16-bit parameter into a buffer in big-endian format.
 * @param[in,out] buffer: Pointer to the buffer where the parameter will be encoded.
 * @param[in]     paramenter: The 16-bit parameter to encode.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74662
 */
SOAD_LOCAL_INLINE void SOAD_TCPIP_CHG_PARA_ENCODE_16(uint8* buffer, uint32 paramenter)
{
    (buffer)[SOAD_DATA_OFFSET_0] = (uint8)((paramenter) >> SOAD_BIT_8);
    (buffer)[SOAD_DATA_OFFSET_1] = (uint8)(paramenter);
}
#if (SOAD_PARTITION_NUM > 1u)
/**
 * @brief         Get SoAd running partition id.
 * @param[in]     cfgAppId: application id of configuration
 * @return        partition id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74663
 */
SOAD_LOCAL_INLINE uint16 SoAd_GetPartitionIndex(uint16 cfgAppId)
{
    uint16 partIndex = 0u;
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
    partIndex = SoAd_Partition[cfgAppId].PartitionIndex;
#endif
    SOAD_NOUSED(cfgAppId);
    return partIndex;
}
#endif
/**
 * @brief         Calculate the uint16 value by condition .
 * @param[in]     condition: condition of calculation
 * @param[in]     trueValue: the value when condition is true
 * @param[in]     falseValue: the value when condition is false
 * @return        uint16
 * @retval        value : value of calculation.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-74645
 */
SOAD_LOCAL_INLINE uint16 SoAd_CalculateU16ValueByCondition(boolean condition, uint16 trueValue, uint16 falseValue)
{
    uint16 value;

    if (condition)
    {
        value = trueValue;
    }
    else
    {
        value = falseValue;
    }

    return value;
}

/**
 * @brief         Calculate the boolean value by condition .
 * @param[in]     condition: condition of calculation
 * @param[in]     trueValue: the value when condition is true
 * @param[in]     falseValue: the value when condition is false
 * @return        uint16
 * @retval        value : value of calculation.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-74644
 */
SOAD_LOCAL_INLINE boolean
    SoAd_CalculateBooleanValueByCondition(boolean condition, boolean trueValue, boolean falseValue)
{
    boolean value;

    if (condition)
    {
        value = trueValue;
    }
    else
    {
        value = falseValue;
    }

    return value;
}
/**
 * @brief         Maps metadata to a socket connection ID.
 * @param[out]    SoConId: The ID of the socket connection.
 * @param[in]     MetaDataPtr: Pointer to the metadata.
 */
#if (0x100u > SOAD_SO_CON_MAX)
#define SOAD_METADATATOSOCONID(SoConId, MetaDataPtr) ((SoConId) = (MetaDataPtr)[0u])
#else
#if (LOW_BYTE_FIRST == CPU_BYTE_ORDER)
#define SOAD_METADATATOSOCONID(SoConId, MetaDataPtr)  (  \
        (SoConId) = (SoAd_SoConIdType)((MetaDataPtr)[0u]) |
        (SoAd_SoConIdType)(MetaDataPtr[1u]) << SOAD_BIT_8      \
        )
#else
#define SOAD_METADATATOSOCONID(SoConId, MetaDataPtr) \
    ((SoConId) = (SoAd_SoConIdType)((MetaDataPtr)[1u]) | (SoAd_SoConIdType)((MetaDataPtr)[0u]) << SOAD_BIT_8)
#endif
#endif

/**
 * Defines the header size.
 */
#define SOAD_HEADER_SIZE (8U)

/**
 * Defines the TCP protocol identifier.
 */
#define SOAD_TCP TCPIP_IPPROTO_TCP

/**
 * Defines the UDP protocol identifier.
 */
#define SOAD_UDP TCPIP_IPPROTO_UDP

/**
 * @brief         Retrieves the transmit buffer configuration index of a socket connection.
 * @param[in]     soConId: The ID of the socket connection.
 */
#define SOAD_SOCKET_TXBUF_INDEX_CFG(soConId) SoAd_SoConCfgPtr[soConId].TxBuffCfgIndexPtr

/**
 * @brief         Retrieves the socket connection group reference of a socket connection.
 * @param[in]     soConId: The ID of the socket connection.
 */
#define SOAD_SOCON_REFGORUP(soConId) SoAd_SoConCfgPtr[soConId].SocketConnectionGroupRef

/**
 * @brief         Retrieves the remote address configuration of a socket connection.
 * @param[in]     soConId: The ID of the socket connection.
 */
#define SOAD_SOCONREMOTEADDR_CFG(soConId) SoAd_SoConCfgPtr[soConId].SocketRemoteAddress

/**
 * @brief         Retrieves the start index of PDU route destinations for a PDU route.
 * @param[in]     pduId: The ID of the PDU route.
 */
#define SOAD_PDUROT_DESTINDEX_CFG(pduId) SoAd_PduRouteCfgPtr[pduId].PduRouteDestStartId

/**
 * @brief         Retrieves if a routing group is transmit triggerable.
 * @param[in]     routGroupId: The ID of the routing group.
 */
#define SOAD_ROUTGROUP_TXTRIG(routGroupId) SoAd_RouteGroupCfgPtr[routGroupId].RoutingGroupTxTriggerable

/**
 * The total number of pdu route destinations.
 */
#define SOAD_PDU_ROUTE_DEST_NUM (SoAd_PbCfgPtr->PduRouteDestTotalNum)

/**
 * The total number of socket route destinations.
 */
#define SOAD_SOCKET_ROUTE_DEST_NUM (SoAd_PbCfgPtr->SocketRouteDestTotalNum)

/**
 * The total number of PDU routes.
 */
#define SOAD_PDU_ROUTE_NUM (SoAd_PbCfgPtr->PduRouteNum)

/**
 * The total number of routing groups.
 */
#define SOAD_ROUTING_GROUP_NUM (SoAd_PbCfgPtr->RoutingGroupNum)

/**
 * The total number of socket connection groups.
 */
#define SOAD_SO_CON_GROUP_NUM (SoAd_PbCfgPtr->SocketConnectionGroupNum)

/**
 * @brief         Retrieves the local address configuration of a socket connection group.
 * @param[in]     soConGroupId: The ID of the socket connection group.
 */
#define SOAD_SOCKETLOCALADDR_CFG(soConGroupId) SoAd_SoConGroupCfgPtr[soConGroupId].SocketLocalAddressRef

#if (STD_ON == SOAD_SUPPORT_TCP)
/**
 * @brief         Retrieves the TCP configuration of a socket connection group.
 * @param[in]     soConGroupId: The ID of the socket connection group.
 */
#define SOAD_TCPTYPE_CFG(soConGroupId) SoAd_SoConGroupCfgPtr[soConGroupId].SocketProtocol.SocketTcpPtr

/**
 * @brief         Retrieves TcpImmediateTpTxConfirmation of the TCP configuration of a socket connection group.
 * @param[in]     soConGroupId: The ID of the socket connection group.
 */
#define SOAD_SOCONGROUP_TCPIMME_TXCONF(soConGroupId) \
    SoAd_SoConGroupCfgPtr[soConGroupId].SocketProtocol.SocketTcpPtr->SocketTcpImmediateTpTxConfirmation
#endif

/**
 * @brief         Retrieves the upper layer configuration for a module.
 * @param[in]     upModule: The upper layer module index.
 */
#if (STD_ON == SOAD_SUPPORT_UDP)
#define SOAD_UDPTYPE_CFG(soConGroupId) SoAd_SoConGroupCfgPtr[soConGroupId].SocketProtocol.SocketUdpPtr
#endif

/**
 * The configuration of Multiple Partitions information.
 */
#define SOAD_MULTI_PART_INFO SoAd_MultiPartitionInfo[partIndex]

/* ================================================ type definitions ================================================ */
/**
 * @brief     SoAd remote ip addr best match state
 */
typedef enum
{
    SOAD_SOCON_MATCH_REQUEST_NONE,            /**< No match request. @range [0] */
    SOAD_SOCON_MATCH_REQUEST_RECONNECT,       /**< Reconnect match request. @range [1] */
    SOAD_SOCON_MATCH_REQUEST_ONLINE_RECONNECT /**< Online reconnect match request. @range [2] */
} SoAd_BestMatchStateType;

/**
 * @brief     used by SoAd_RxFullTpPduHandle
 */
typedef struct SoAd_RxFullTpPduHandleTypeTag
{
    SoAd_SoConIdType       SoConId;       /**< Socket connection ID. @range [0, 0xFFu] */
    SoAd_uintx             SocketRouteId; /**< Socket route ID. @range [0, 0xFFFFu] */
    PduLengthType          RxDataLength;  /**< Length of received data. @range [0, 0xFFFFu] */
    uint8*                 DataPtr;       /**< Pointer to received data. @range [0, NA */
    PduLengthType          UpBufferSize;  /**< Upper layer buffer size. @range [0, 0xFFFFu] */
    PduLengthType          PduLength;     /**< PDU length. @range [0, 0xFFFFu] */
    SoAd_UpLayerModuleType UpModuleId;    /**< Upper layer module ID. @range ref SoAd_UpLayerModuleType */
    SoAd_uintx             RxBufferId;    /**< Receive buffer ID. @range [0, 0xFFFFu] */
    TcpIp_SocketIdType     TcpIpSocketId; /**< TCP/IP socket ID. @range ref TcpIp_SocketIdType */
} SoAd_RxFullTpPduHandleType;

#if (STD_ON == SOAD_SUPPORT_UDP)
/**
 * @brief     used by SoAd_RxFullTpPduHandle
 */
typedef struct SoAd_FindTheValidSoConTypeTag
{
    boolean            RemoteAddrChanged; /**< Flag indicating whether the remote address has changed. @range [0, 1] */
    TcpIp_SockAddrType RemoteAddr;        /**< Remote address. @range ref TcpIp_SockAddrType */
} SoAd_FindTheValidSoConType;
#endif

/* ============================================ internal data definition ============================================ */
#define SOAD_START_SEC_VAR_CLEARED_PTR
#include "SoAd_MemMap.h"
/**
 * @brief         Pointer to the SoAd configuration structure.
 */
SOAD_LOCAL const SoAd_ConfigType* SoAd_PbCfgPtr;
#define SOAD_STOP_SEC_VAR_CLEARED_PTR
#include "SoAd_MemMap.h"

#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
/**
 * @brief         Pointer to the SoAd module initialize status.
 */
#define SoAd_InitStatus (*SoAd_MultiPartitionInitStatusPtr[partIndex])
#else
#define SOAD_START_SEC_VAR_INIT_8
#include "SoAd_MemMap.h"
        /**
         * @brief         Module initialization status.
         */
        SOAD_LOCAL SoAd_Init_StateType SoAd_InitStatus = SOAD_STATE_UNINIT;
#define SOAD_STOP_SEC_VAR_INIT_8
#include "SoAd_MemMap.h"
#endif

#if (0u < SOAD_MAX_BUFFER_SIZE)
#define SOAD_START_SEC_VAR_CLEARED_8
#include "SoAd_MemMap.h"
/**
 * @brief         Buffer for PDU data.
 */
SOAD_LOCAL uint8 SoAd_PduBuffer[SOAD_MAX_BUFFER_SIZE];
#define SOAD_STOP_SEC_VAR_CLEARED_8
#include "SoAd_MemMap.h"
#endif

#if (0u < SOAD_MAX_IF_PDU_SIZE)
#define SOAD_START_SEC_VAR_CLEARED_8
#include "SoAd_MemMap.h"
/**
 * @brief         Buffer for IF PDU data.
 */
SOAD_LOCAL uint8 SoAd_IfPduBuffer[SOAD_MAX_IF_PDU_SIZE];
#define SOAD_STOP_SEC_VAR_CLEARED_8
#include "SoAd_MemMap.h"
#endif

#if (STD_ON == SOAD_SUPPORT_RX_BUFFER) || (STD_ON == SOAD_SUPPORT_TX_BUFFER)
#define SOAD_START_SEC_VAR_CLEARED_16
#include "SoAd_MemMap.h"
/**
 * The total number of socket transmit buffers.
 */
SOAD_LOCAL uint16 SoAd_TxBufferNum;
/**
 * The total number of socket receive buffers.
 */
SOAD_LOCAL uint16 SoAd_RxBufferNum;
#define SOAD_STOP_SEC_VAR_CLEARED_16
#include "SoAd_MemMap.h"
#endif

#define SOAD_START_SEC_VAR_CLEARED_PTR
#include "SoAd_MemMap.h"
/**
 * Pointer to the socket connection manager.
 */
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION) && (SOAD_PARTITION_NUM > 1u)
SOAD_LOCAL SoAd_InnerSocketConManageType** SoAd_SoConManagerPtr;
#else
        SOAD_LOCAL SoAd_InnerSocketConManageType* SoAd_SoConManagerPtr;
#endif
/**
 * Pointer to the socket connection group manager.
 */
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION) && (SOAD_PARTITION_NUM > 1u)
SOAD_LOCAL SoAd_InnerSocketConGroupManageType** SoAd_SoConGroupManagerPtr;
#else
        SOAD_LOCAL SoAd_InnerSocketConGroupManageType* SoAd_SoConGroupManagerPtr;
#endif
#if (SOAD_ROUTING_GROUP_MAX > 0u)
/**
 * Pointer to the routing group manager.
 */
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION) && (SOAD_PARTITION_NUM > 1u)
SOAD_LOCAL SoAd_InnerRoutingGroupManageType** SoAd_RoutGroupManagerPtr;
#else
SOAD_LOCAL SoAd_InnerRoutingGroupManageType* SoAd_RoutGroupManagerPtr;
#endif
#endif
#if (SOAD_SOCKET_ROUTE_DEST_MAX > 0u)
/**
 * Pointer to the socket route destination manager.
 */
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION) && (SOAD_PARTITION_NUM > 1u)
SOAD_LOCAL SoAd_InnerSocketRouteDestManagerType** SoAd_SoRouteDestManagerPtr;
#else
SOAD_LOCAL SoAd_InnerSocketRouteDestManagerType* SoAd_SoRouteDestManagerPtr;
#endif
#endif
#if (SOAD_PDU_ROUTE_DEST_MAX > 0u)
/**
 * Pointer to the PDU route destination manager.
 */
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION) && (SOAD_PARTITION_NUM > 1u)
SOAD_LOCAL SoAd_InnerPduRouteDestManagerType** SoAd_PduRouteDestManagerPtr;
#else
SOAD_LOCAL SoAd_InnerPduRouteDestManagerType* SoAd_PduRouteDestManagerPtr;
#endif
#endif
#if (STD_ON == SOAD_SUPPORT_TX_BUFFER)
/**
 * Pointer to the socket transmit buffer manager.
 */
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION) && (SOAD_PARTITION_NUM > 1u)
SOAD_LOCAL SoAd_SoTxBufManType** SoAd_SoTxBuffManagerPtr;
#else
SOAD_LOCAL SoAd_SoTxBufManType* SoAd_SoTxBuffManagerPtr;
#endif
#endif
#if (STD_ON == SOAD_SUPPORT_RX_BUFFER)
/**
 * Pointer to the socket receive buffer manager.
 */
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION) && (SOAD_PARTITION_NUM > 1u)
SOAD_LOCAL SoAd_SoRxBufferManType** SoAd_SoRxBuffManagerPtr;
#else
SOAD_LOCAL SoAd_SoRxBufferManType* SoAd_SoRxBuffManagerPtr;
#endif
#endif

#if (SOAD_PDU_ROUTE_MAX > 0u)
/**
 * Pointer to the PDU route manager.
 */
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION) && (SOAD_PARTITION_NUM > 1u)
SOAD_LOCAL SoAd_InnerPduRoutManageType** SoAd_PduRouteManagerPtr;
#else
SOAD_LOCAL SoAd_InnerPduRoutManageType* SoAd_PduRouteManagerPtr;
#endif
#endif

/**
 * @brief         Retrieves the configuration of a socket connection.
 */
SOAD_LOCAL const SoAd_SocketConnectionType* SoAd_SoConCfgPtr;
/**
 * @brief         Retrieves the configuration of a socket connection group.
 */
SOAD_LOCAL const SoAd_SocketConnectionGroupType* SoAd_SoConGroupCfgPtr;

/**
 * @brief         Retrieves the start index of PDU route destinations for a PDU route.
 */
SOAD_LOCAL const SoAd_PduRouteType* SoAd_PduRouteCfgPtr;

/**
 * @brief         Retrieves the configuration of a socket route destination.
 */
SOAD_LOCAL const SoAd_SocketRouteDestType* SoAd_SoRouteDestCfgPtr;

/**
 * @brief         Retrieves the configuration of a PDU route destination.
 */
SOAD_LOCAL const SoAd_PduRouteDestType* SoAd_PduRouteDestCfgPtr;

#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
/**
 * @brief         Retrieves the upper layer configuration for a module.
 */
SOAD_LOCAL const SoAd_UpLayerFuncType* SoAd_UpperLayerPCCfgPtr;
#endif
/**
 * @brief         Retrieves the configuration of a routing group.
 */
SOAD_LOCAL const SoAd_RoutingGroupType* SoAd_RouteGroupCfgPtr;
/**
 * @brief         Retrieves the configuration of a socket buffer.
 */
SOAD_LOCAL const SoAd_SoBufferCfgType* SoAd_SocketBufferCfgPtr;
/**
 * @brief         Retrieves the configuration of a socket route.
 */
SOAD_LOCAL const SoAd_SocketRouteType* SoAd_SocketRouteCfgPtr;
/**
 * The total number of socket connections.
 */
SOAD_LOCAL SoAd_SoConIdType SoAd_SoConNum;
#define SOAD_STOP_SEC_VAR_CLEARED_PTR
#include "SoAd_MemMap.h"

/* ============================================ external data definition ============================================ */

/* ========================================= internal function declarations ========================================= */

#define SOAD_START_SEC_CODE
#include "SoAd_MemMap.h"

/**
 * @brief         Check if TCP TP is transmiting
 * @param[in]     soConId: id of socket connection
 * @param[in]     tpPduTransmitting: TP Pdu transmitting
 * @return        boolean
 * @retval        TRUE  : transmiting.
 * @retval        FALSE : not transmiting.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-74643
 */
SOAD_LOCAL_INLINE boolean SoAd_CheckTCPTpTransmiting(SoAd_SoConIdType soConId, boolean tpPduTransmitting)
{
    return ((SOAD_TCP == SoAd_SoConCfgPtr[soConId].TcpUdpProtocol) && tpPduTransmitting);
}

/**
 * @brief         When SoAdSocketUdpListenOnly set TRUE for UDP, the socket connection group is only used for reception.
 * @param[in]     soConId: id of socket connection
 * @return        boolean
 * @retval        TRUE  : only reception.
 * @retval        FALSE : reception and transmission.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-74642
 */
SOAD_LOCAL_INLINE boolean SoAd_CheckUDPSoConOnlyListen(SoAd_SoConIdType soConId)
{
    return (
        (SOAD_UDP == SoAd_SoConCfgPtr[soConId].TcpUdpProtocol)
        && SOAD_UDPTYPE_CFG(SoAd_SoConCfgPtr[soConId].SocketConnectionGroupRef)->SocketUdpListenOnly);
}
#if (STD_ON == SOAD_PDU_ROUTE_METADATA_SUPPORT)
/**
 * @brief         Get socket connection id from PDU route metadata
 * @param[in]     txPduId: PduId of transmit
 * @param[in]     metaDataPtr: metadata of transmit
 * @return        SoAd_SoConIdType
 * @retval        socket connection id.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-74641
 */
SOAD_LOCAL_INLINE SoAd_SoConIdType SoAd_GetSoConIdFromTxMetaData(boolean metaDataEnable, uint8* metaDataPtr)
{
    SoAd_SoConIdType txSoConId = SOAD_NO_TXMETADATA;
    if (metaDataEnable && (NULL_PTR != metaDataPtr))
    {
        SOAD_METADATATOSOCONID(txSoConId, metaDataPtr);
    }
    return txSoConId;
}
#endif

/**
 * @brief         Get socket connection id from PDU route metadata
 * @param[in]     soConId: id of socket connection
 * @param[in]     socketId: socket id
 * @param[in]     length: update length
 * @return        void
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-74640
 */
SOAD_LOCAL_INLINE void SoAd_UpdateTcpReceived(SoAd_SoConIdType soConId, TcpIp_SocketIdType socketId, uint32 length)
{
    if (SOAD_TCP == SoAd_SoConCfgPtr[soConId].TcpUdpProtocol)
    {
        /*SWS_SoAd_00564:SoAd shall confirm the reception of all data*/
        (void)TcpIp_TcpReceived(socketId, length);
    }
}

#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
/**
 * @brief     Fill metadata to SDU
 * @param[in] SoConId: The ID of the socket connection.
 * @param[in] metaDataEnable: metadata enable flag.
 * @param[out] DataPtr: Pointer to the data.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74664
 */
SOAD_LOCAL uint8* SoAd_MetaDataFillHandle(SoAd_SoConIdType SoConId, boolean metaDataEnable, uint8* DataPtr);
#endif
/**
 * @brief         Sets up the automatic socket connection for transmission.
 * @param[in]     SoConId: The ID of the socket connection.
 * @param[in]     Protocol: The protocol type (TCP or UDP).
 * @param[in]     partIndex: The partition index.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74665
 */
SOAD_LOCAL void SoAd_SocnTransmitAutomaticSoConSetupHandle(
    SoAd_SoConIdType               SoConId,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    TcpIp_ProtocolType             Protocol);

#if (STD_ON == SOAD_SUPPORT_TCP)
/**
 * @brief         Opens a TCP server socket connection.
 * @param[in]     SoConId: The ID of the socket connection.
 * @param[in]     SoConGroup: The ID of the socket connection group.
 * @param[in]     partIndex: The partition index.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74666
 */
SOAD_LOCAL void SoAd_OpenTcpServerSoConHandle(
    SoAd_SoConIdType                    SoConId,
    SoAd_uintx                          SoConGroup,
    SoAd_InnerSocketConManageType*      soConMgmtPtr,
    SoAd_InnerSocketConGroupManageType* soConGroupMgmtPtr);
/**
 * @brief         Opens a TCP client socket connection.
 * @param[in]     SoConId: The ID of the socket connection.
 * @param[in]     SoConGroup: The ID of the socket connection group.
 * @param[in]     partIndex: The partition index.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74667
 */
SOAD_LOCAL void SoAd_OpenTcpClientSoConHandle(
    SoAd_SoConIdType               SoConId,
    SoAd_uintx                     SoConGroup,
    SoAd_InnerSocketConManageType* soConMgmtPtr);
#endif

#if (SOAD_PDU_ROUTE_DEST_MAX > 0)
/**
 * @brief         Handles the main function for transmission confirmation.
 * @param[in]     partIndex: The partition index.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74668
 */
SOAD_LOCAL void SoAd_MainFunctionTxConfirmationHandle(void);
#endif
#if (STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER)
/**
 * @brief         Handles the main function for NPDU UDP transmission.
 * @param[in]     SoConId: The ID of the socket connection.
 * @param[in]     partIndex: The partition index.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74669
 */
SOAD_LOCAL void SoAd_MainFunctionNPduUdpTxHandle(SoAd_SoConIdType SoConId);
#endif

#if ((0u < SOAD_MAX_IF_PDU_SIZE) && (SOAD_PDU_ROUTE_DEST_MAX > 0))
/**
 * @brief         Gets the IF PDU data.
 * @param[in]     partIndex: The partition index.
 * @param[in]     TxPduId: The ID of the transmit PDU.
 * @param[in]     pduInfoPtr: The pduInfo of the transmit PDU.
 * @return        E_OK if successful, E_NOT_OK otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74670
 */
SOAD_LOCAL Std_ReturnType SoAd_GetIfPduData(
    const SoAd_PduRouteType*     pduRouteCfgPtr,
    SoAd_InnerPduRoutManageType* pduRouteMgmtPtr,
    const PduInfoType*           pduInfoPtr);
#endif

#if (STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER)
/**
 * @brief Handles the "trigger always" operation for UDP transmission.
 * @param [in]    TxPduId:The ID of the PDU to be transmitted.
 * @param [in]    PduRouteDestId:The ID of the PDU route destination.
 * @param [in]    SoConId:The ID of the Socket connection.
 * @param [in]    partIndex:The partition index.
 * @return        E_OK if successful, E_NOT_OK otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74638
 */
SOAD_LOCAL Std_ReturnType SoAd_NPduUdpTransmitHandleTriggerAlways(
    SoAd_InnerPduRoutManageType*   pduRouteMgmtPtr,
    const SoAd_PduRouteDestType*   pduRouteDestCfgPtr,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    SoAd_SoConIdType               SoConId);

/**
 * @brief Handles the "trigger never" operation for UDP transmission.
 * @param [in]    TxPduId: The ID of the PDU to be transmitted.
 * @param [in]    PduRouteDestId: The ID of the PDU route destination.
 * @param [in]    SoConId :The ID of the Socket connection.
 * @param [in]    partIndex: The partition index.
 * @return        E_OK if successful, E_NOT_OK otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74639
 */
SOAD_LOCAL Std_ReturnType SoAd_NPduUdpTransmitHandleTriggerNever(
    SoAd_InnerPduRoutManageType*   pduRouteMgmtPtr,
    const SoAd_PduRouteDestType*   pduRouteDestCfgPtr,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    SoAd_SoConIdType               SoConI);
/**
 * @brief         Handles the transmission of NPDU UDP.
 * @param[in]     TxPduId: The ID of the transmit PDU.
 * @param[in]     PduRouteDestId: The ID of the PDU route destination.
 * @param[in]     SoConId: The ID of the socket connection.
 * @param[in]     partIndex: The partition index.
 * @return        E_OK if successful, E_NOT_OK otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74671
 */
SOAD_LOCAL Std_ReturnType SoAd_NPduUdpTransmitHandle(
    SoAd_InnerPduRoutManageType*   pduRouteMgmtPtr,
    const SoAd_PduRouteDestType*   pduRouteDestCfgPtr,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    SoAd_SoConIdType               SoConId);
#endif

#if ((0u < SOAD_MAX_IF_PDU_SIZE) && (SOAD_PDU_ROUTE_DEST_MAX > 0))
/**
 * @brief         Transmits an IF PDU on a specific socket connection.
 * @param[in]     TxPduId: The ID of the transmit PDU.
 * @param[in]     PduRouteDestId: The ID of the PDU route destination.
 * @param[in]     SoConId: The ID of the socket connection.
 * @param[in]     partIndex: The partition index.
 * @return        E_OK if successful, E_NOT_OK otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74672
 */
SOAD_LOCAL Std_ReturnType SoAd_IfTransmitSoCon(
    PduIdType                      TxPduId,
    SoAd_InnerPduRoutManageType*   pduRouteMgmtPtr,
    SoAd_uintx                     PduRouteDestId,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    SoAd_SoConIdType               SoConId);
#endif

#if (STD_ON == SOAD_SUPPORT_TX_BUFFER)
/**
 * @brief         Handles the error for a TP PDU transmission.
 * @param[in]     SoConId: The ID of the socket connection.
 * @param[in]     TxBufferId: The ID of the transmit buffer.
 * @param[in]     UpModule: The upper layer module type.
 * @param[in]     UpPduId: The ID of the upper layer PDU.
 * @param[in]     partIndex: The partition index.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74673
 */
SOAD_LOCAL void SoAd_TpPduTxErrorHandle(
    SoAd_SoConIdType       SoConId,
    SoAd_uintx             TxBufferId,
    SoAd_UpLayerModuleType UpModule,
    PduIdType              UpPduId);
#endif

/**
 * @brief         Checks if the remote address of a socket connection is locked.
 * @param[in]     SoConId: The ID of the socket connection.
 * @param[in]     partIndex: The partition index.
 * @return        TRUE if the remote address is locked, FALSE otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74674
 */
SOAD_LOCAL boolean
    SoAd_SoConRemoteAddrLocked(SoAd_SoConIdType SoConId, const SoAd_InnerSocketConManageType* soConMgmtPtr);

#if (STD_ON == SOAD_SUPPORT_TX_BUFFER)
/**
 * @brief         Handles the main function for TP PDU transmission.
 * @param[in]     SoConId: The ID of the socket connection.
 * @param[in]     partIndex: The partition index.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74675
 */
SOAD_LOCAL void SoAd_MainFunctionTpTransmitHandle(SoAd_SoConIdType SoConId);
#endif
/**
 * @brief         Releases the remote address for a socket connection.
 * @param[in]     SoConId: The ID of the socket connection.
 * @param[in]     SoConGroupId: The ID of the socket connection group.
 * @param[in]     partIndex: The partition index.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74676
 */
SOAD_LOCAL void SoAd_ReleaseRemoteAddrHandle(
    SoAd_SoConIdType               SoConId,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    SoAd_uintx                     SoConGroupId);
#if (SOAD_PDU_ROUTE_DEST_MAX > 0)
/**
 * @brief         Initializes the PDU route manager.
 * @param[in]     TxPduId: The ID of the transmit PDU.
 * @param[in]     partIndex: The partition index.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74677
 */
SOAD_LOCAL void SoAd_PduRouteManagerInit(PduIdType TxPduId);
#endif

/**
 * @brief         Releases the remote address in the main function for a socket connection.
 * @param[in]     SoConId: The ID of the socket connection.
 * @param[in]     partIndex: The partition index.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74678
 */
SOAD_LOCAL void SoAd_ReleaseRemoteAddrMainFunctionHandle(SoAd_SoConIdType SoConId);

#if (STD_ON == SOAD_SUPPORT_TX_BUFFER)
/**
 * @brief         Initializes the transmit buffer.
 * @param[in]     TxBufferId: The ID of the transmit buffer.
 * @param[in]     partIndex: The partition index.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74679
 */
SOAD_LOCAL void SoAd_TxBufferInit(SoAd_uintx TxBufferId, uint16 partIndex);
#endif

#if (STD_ON == SOAD_SUPPORT_RX_BUFFER)
/**
 * @brief         Initializes the receive buffer.
 * @param[in]     RxBufferId: The ID of the receive buffer.
 * @param[in]     partIndex: The partition index.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74680
 */
SOAD_LOCAL void SoAd_RxBufferInit(SoAd_uintx RxBufferId, uint16 partIndex);
#endif

#if (STD_ON == SOAD_SUPPORT_RX_BUFFER)
#if (STD_ON == SOAD_SUPPORT_HEADERID)
/**
 * @brief         Handles the new PDU for header enable.
 * @param[in]     SoConId: The ID of the socket connection.
 * @param[in]     BufferManageId: The ID of the buffer management.
 * @param[in]     partIndex: The partition index.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74681
 */
SOAD_LOCAL void SoAd_HeaderEnableNewPduHandle(
    SoAd_SoConIdType               SoConId,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    SoAd_uintx                     BufferManageId,
    SoAd_SoRxBufferManType*        soRxBuffMgmtPtr);
/**
 * @brief         Handles the old PDU for header enable.
 * @param[in]     SoConId: The ID of the socket connection.
 * @param[in]     BufferManageId: The ID of the buffer management.
 * @param[in]     partIndex: The partition index.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74682
 */
SOAD_LOCAL void SoAd_HeaderEnableOldPduHandle(
    SoAd_SoConIdType               SoConId,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    SoAd_uintx                     BufferManageId,
    SoAd_SoRxBufferManType*        soRxBuffMgmtPtr);
/**
 * @brief         Handles the TP PDU for header enable.
 * @param[in]     SoConId: The ID of the socket connection.
 * @param[in]     UpBufferSize: Size of the upper layer buffer.
 * @param[in]     partIndex: The partition index.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74683
 */
SOAD_LOCAL void SoAd_HeaderEnableTpPduHandle(
    SoAd_SoConIdType               SoConId,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    PduLengthType                  UpBufferSize,
    SoAd_SoRxBufferManType*        soRxBuffMgmtPtr);
#endif

#if ((0u < SOAD_MAX_IF_PDU_SIZE) && (STD_ON == SOAD_SUPPORT_HEADERID))
/**
 * @brief         Handles the IF PDU for header enable.
 * @param[in]     SoConId: The ID of the socket connection.
 * @param[in]     BufferManageId: The ID of the buffer management.
 * @param[in]     partIndex: The partition index.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74684
 */
SOAD_LOCAL void SoAd_HeaderEnableIfPduHandle(
    SoAd_SoConIdType                     SoConId,
    const SoAd_InnerSocketConManageType* soConMgmtPtr,
    SoAd_uintx                           BufferManageId,
    SoAd_SoRxBufferManType*              soRxBuffMgmtPtr);
#endif

/**
 * @brief         Handles the reception data in the main function with header disable.
 * @param[in]     SoConId: The ID of the socket connection.
 * @param[in]     BufferManageId: The ID of the buffer management.
 * @param[in]     partIndex: The partition index.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74685
 */
SOAD_LOCAL void SoAd_RxDataMainFunctionHeaderDisableHandle(
    SoAd_SoConIdType               SoConId,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    SoAd_uintx                     BufferManageId,
    SoAd_SoRxBufferManType*        soRxBuffMgmtPtr);

/**
 * @brief         Handles the reception data in the main function for a socket connection.
 * @param[in]     SoConId: The ID of the socket connection.
 * @param[in]     partIndex: The partition index.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74686
 */
SOAD_LOCAL void SoAd_SoConRxDataMainFunctionHandle(SoAd_SoConIdType SoConId);
#endif
#if (0u < SOAD_SOCKET_ROUTE_DEST_MAX)
#if (STD_ON == SOAD_SUPPORT_UDP)
#if (STD_ON == SOAD_SUPPORT_HEADERID)
#if (STD_ON == SOAD_SUPPORT_RX_BUFFER)
/**
 * @brief         Handle received TP PDU for a UDP Header enable socket connection.
 * @param[in]     SoConId: The ID of the socket connection.
 * @param[in]     SocketRouteId: The ID of the socket route.
 * @param[in,out] DataPosition: Pointer to the data position.
 * @param[in]     DataPtr: Pointer to the data buffer.
 * @param[in]     partIndex: The partition index.
 * @return        TRUE if the PDU data is valid, FALSE otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74687
 */
SOAD_LOCAL boolean SoAd_HeaderEnableUdpSoConRxTpPduHandle(
    SoAd_SoConIdType               SoConId,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    SoAd_uintx                     SocketRouteId,
    PduLengthType*                 DataPosition,
    uint8*                         DataPtr);
#endif
/**
 * @brief         Handles the reception of a UDP socket connection.
 * @param[in]     SoConId: The ID of the socket connection.
 * @param[in]     RxMessageLength: Length of the received message.
 * @param[in]     DataPtr: Pointer to the data buffer.
 * @param[in]     partIndex: The partition index.
 * @return        TRUE if the reception is successful, FALSE otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74688
 */
SOAD_LOCAL boolean SoAd_HeaderEnableUdpSoConRxHandle(
    SoAd_SoConIdType               SoConId,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    uint16                         RxMessageLength,
    uint8*                         DataPtr);
#endif
#endif

#if (STD_ON == SOAD_SUPPORT_RX_BUFFER)
#if (STD_ON == SOAD_SUPPORT_HEADERID)
/**
 * @brief         Handles the reception of a TCP socket connection for a TP PDU.
 * @param[in]     SoConId: The ID of the socket connection.
 * @param[in]     SocketRouteId: The ID of the socket route.
 * @param[in]     RxMessageLength: Length of the received message.
 * @param[in]     DataPtr: Pointer to the data buffer.
 * @param[in]     partIndex: The partition index.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74689
 */
SOAD_LOCAL void SoAd_HeaderEnableTcpSoConRxTpPduHandle(
    SoAd_SoConIdType               SoConId,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    SoAd_uintx                     SocketRouteId,
    PduLengthType                  RxMessageLength,
    uint8*                         DataPtr);
/**
 * @brief         Handles the full TP PDU reception.
 * @param[in]     Parameter: Pointer to the reception handle parameters.
 * @param[in]     partIndex: The partition index.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74690
 */
SOAD_LOCAL void SoAd_RxFullTpPduHandle(
    const SoAd_RxFullTpPduHandleType* Parameter,
    SoAd_InnerSocketConManageType*    soConMgmtPtr,
    SoAd_SoRxBufferManType*           soRxBuffMgmtPtr);
#endif
/**
 * @brief     Handle received TP PDU with disabled header
 * @param[in] SoConId: The ID of the socket connection.
 * @param[in] SocketRouteId: The ID of the socket route.
 * @param[in] RxMessageLength: Length of the received message.
 * @param[in] DataPtr: Pointer to the received data.
 * @param[in] partIndex: The partition index.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74691
 */
SOAD_LOCAL void SoAd_HeaderDisableSoConRxTpPduHandle(
    SoAd_SoConIdType               SoConId,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    SoAd_uintx                     SocketRouteId,
    PduLengthType                  RxMessageLength,
    uint8*                         DataPtr);

#if (STD_ON == SOAD_SUPPORT_HEADERID)
/**
 * @brief     Handle received PDU with enabled header for TCP socket connection
 * @param[in] SoConId: The ID of the socket connection.
 * @param[in] RxMessageLength: Length of the received message.
 * @param[in] DataPtr: Pointer to the received data.
 * @param[in] partIndex: The partition index.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74692
 */
SOAD_LOCAL void SoAd_HeaderEnableTcpSoConRxHandle(
    SoAd_SoConIdType               SoConId,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    PduLengthType                  RxMessageLength,
    uint8*                         DataPtr);
#endif

/**
 * @brief     Copy received data to SoAd Rx Buffer
 * @param[in] SoConId: The ID of the socket connection.
 * @param[in] RxMessageLength: Length of the received message.
 * @param[in] DataPtr: Pointer to the received data.
 * @param[in] partIndex: The partition index.
 * @return    TRUE if data copy is successful, FALSE otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74693
 */
SOAD_LOCAL boolean SoAd_CopyDataToRxBuffer(
    SoAd_SoConIdType               SoConId,
    PduLengthType                  RxMessageLength,
    const uint8*                   DataPtr,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    SoAd_SoRxBufferManType*        soRxBuffMgmtPtr);
#endif

/**
 * @brief     Handle received IF PDU with disabled header for socket connection
 * @param[in] SoConId: The ID of the socket connection.
 * @param[in] SocketRouteId: The ID of the socket route.
 * @param[in] RxMessageLength: Length of the received message.
 * @param[in] DataPtr: Pointer to the received data.
 * @param[in] partIndex: The partition index.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74694
 */
SOAD_LOCAL void SoAd_HeaderDisableSoConRxIfPduHandle(
    SoAd_SoConIdType SoConId,
    SoAd_uintx       SocketRouteId,
    PduLengthType    RxMessageLength,
    uint8*           DataPtr);
#endif
#if (0u < SOAD_SOCKET_ROUTE_DEST_MAX)
/**
 * @brief         Finds the valid socket connection based on the socket ID and remote address.
 * @param[in]     SocketId: The ID of the socket.
 * @param[in]     RemoteAddrPtr: Pointer to the remote address.
 * @param[in]     UdpParaPtr: Pointer to the UDP parameters (only used if UDP is supported).
 * @param[out]    SoConId: The ID of the socket connection.
 * @return        TRUE if a valid socket connection is found, FALSE otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74695
 */
SOAD_LOCAL boolean SoAd_FindTheValidSoCon(
    TcpIp_SocketIdType        SocketId,
    const TcpIp_SockAddrType* RemoteAddrPtr,
#if (STD_ON == SOAD_SUPPORT_UDP)
    SoAd_FindTheValidSoConType* UdpParaPtr,
#endif
    SoAd_SoConIdType* SoConId);
#endif

#if (STD_ON == SOAD_SUPPORT_UDP)
#if (STD_ON == SOAD_SUPPORT_HEADERID)
/**
 * @brief         Performs a strict header length check for UDP.
 * @param[in]     SoConGroupId: The ID of the socket connection group.
 * @param[in]     BufPtr: Pointer to the buffer.
 * @param[in]     Length: Length of the buffer.
 * @return        TRUE if the header length is valid, FALSE otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74696
 */
SOAD_LOCAL boolean SoAd_UdpStrictHeaderLenCheck(SoAd_uintx SoConGroupId, const uint8* BufPtr, uint16 Length);
#endif

/**
 * @brief         Handles the UDP alive supervision time for a socket connection.
 * @param[in]     SoConId: The ID of the socket connection.
 * @param[in]     partIndex: The partition index.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74697
 */
SOAD_LOCAL void SoAd_UdpAliveSupervisionTimeHandle(SoAd_SoConIdType SoConId);
#endif

/**
 * @brief         Finds the best match algorithm socket connection.
 * @param[in]     SoConGroupId: The ID of the socket connection group.
 * @param[in]     RemoteAddrPtr: Pointer to the remote address.
 * @param[out]    SoConId: The ID of the socket connection.
 * @param[in]     CheckSoConState: The state to check for the socket connection.
 * @param[in]     partIndex: The partition index.
 * @return        TRUE if a valid socket connection is found, FALSE otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74698
 */
SOAD_LOCAL boolean SoAd_GetBestMatchAlgorithmSoCon(
    SoAd_uintx                SoConGroupId,
    const TcpIp_SockAddrType* RemoteAddrPtr,
    SoAd_SoConIdType*         SoConId,
    SoAd_BestMatchStateType   CheckSoConState);

/**
 * @brief     Change socket parameters
 * @param[in] SocketId: The ID of the TCP/IP socket.
 * @param[in] SoConGroup: The ID of the socket connection group.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74699
 */
SOAD_LOCAL void SoAd_ChangeSocketParameter(TcpIp_SocketIdType SocketId, SoAd_uintx SoConGroup);
#if (STD_ON == SOAD_SUPPORT_TCP)
/**
 * @brief     Change TCP socket parameters
 * @param[in] SocketId: The ID of the TCP/IP socket.
 * @param[in] SoConGroup: The ID of the socket connection group.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74700
 */
SOAD_LOCAL void SoAd_ChangeTcpSocketParameter(TcpIp_SocketIdType SocketId, SoAd_uintx SoConGroup);
#endif

#if (STD_ON == SOAD_SUPPORT_UDP)
/**
 * @brief     Open UDP socket connection
 * @param[in] SoConId: The ID of the socket connection.
 * @param[in] SoConGroup: The ID of the socket connection group.
 * @param[in] partIndex: The partition index.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74701
 */
SOAD_LOCAL void SoAd_OpenUdpSoConHandle(
    SoAd_SoConIdType                    SoConId,
    SoAd_InnerSocketConManageType*      soConMgmtPtr,
    SoAd_uintx                          SoConGroup,
    SoAd_InnerSocketConGroupManageType* soConGroupMgmtPtr);
#endif
/**
 * @brief     Notify socket connection mode change and TP start reception
 * @param[in] SoConId: The ID of the socket connection.
 * @param[in] SoConGroup: The ID of the socket connection group.
 * @param[in] partIndex: The partition index.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74702
 */
SOAD_LOCAL void SoAd_SoConModeChgNotificationAndTpStartOfReception(
    SoAd_SoConIdType               SoConId,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    SoAd_uintx                     SoConGroup);

/**
 * @brief     Notify socket connection IP address assignment change
 * @param[in] SoConId: The ID of the socket connection.
 * @param[in] SoConGroup: The ID of the socket connection group.
 * @param[in] State: The IP address state.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74703
 */
SOAD_LOCAL void
    SoAd_SoConLocalIpAddrAssignmentChg(SoAd_SoConIdType SoConId, SoAd_uintx SoConGroup, TcpIp_IpAddrStateType State);

/**
 * @brief     Handle socket connection mode change
 * @param[in] SoConId: The ID of the socket connection.
 * @param[in] partIndex: The partition index.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74704
 */
SOAD_LOCAL void SoAd_SoConModeChangeHandle(SoAd_SoConIdType SoConId);

#if (STD_ON == SOAD_SUPPORT_RX_BUFFER)
/**
 * @brief     Handle received TP PDU after socket connection is closed
 * @param[in] SoConId: The ID of the socket connection.
 * @param[in] partIndex: The partition index.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74705
 */
SOAD_LOCAL void SoAd_CloseRxTpPduHandle(SoAd_SoConIdType SoConId);
/**
 * @brief     Check if the received TP PDU has been processed when TCP received FIN
 * @param[in] SoConId: The ID of the socket connection.
 * @param[in] partIndex: The partition index.
 * @return    TRUE if TP PDU data has been processed, FALSE otherwise.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74706
 */
SOAD_LOCAL boolean SoAd_CheckRxTpPduDataHandle(SoAd_SoConIdType SoConId);
#endif

/**
 * @brief     Close socket connection
 * @param[in] SoConId: The ID of the socket connection.
 * @param[in] SoConGroup: The ID of the socket connection group.
 * @param[in] CloseSocket: Flag to indicate if the socket should be closed.
 * @param[in] partIndex: The partition index.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74707
 */
SOAD_LOCAL void SoAd_SoConCloseHandle(
    SoAd_SoConIdType                    SoConId,
    boolean                             CloseSocket,
    SoAd_InnerSocketConManageType*      soConMgmtPtr,
    SoAd_InnerSocketConGroupManageType* soConGroupMgmtPtr);

/**
 * @brief     Handle socket connection disconnection and recovery
 * @param[in] SoConId: The ID of the socket connection.
 * @param[in] SoConGroup: The ID of the socket connection group.
 * @param[in] partIndex: The partition index.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74708
 */
SOAD_LOCAL void SoAd_DisconnectionAndRecoveryHandle(
    SoAd_SoConIdType                    SoConId,
    SoAd_uintx                          SoConGroup,
    SoAd_InnerSocketConManageType*      soConMgmtPtr,
    SoAd_InnerSocketConGroupManageType* soConGroupMgmtPtr);

/**
 * @brief         Initializes Routing group runtime variable
 * @param[in]     TxPduId: The ID of the transmit PDU.
 * @param[in]     partIndex: The partition index.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74709
 */
SOAD_LOCAL void SoAd_RoutingGroupInit(void);

/* id: routing group identifier specifying the routing group
enFlag: group control enable shall be set TRUE otherwise is FALSE
soConIdPtr:socket connection index specifying the socket connection on
which the routing group  */
#if (0u < SOAD_ROUTING_GROUP_MAX)
/**
 * @brief         Controls the routing group.
 * @param[in]     id: The ID of the routing group.
 * @param[in]     enFlag: Enable flag for the routing group.
 * @param[in]     partIndex: The partition index.
 * @return        E_OK if successful, E_NOT_OK otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74710
 */
SOAD_LOCAL Std_ReturnType SoAd_RoutingGroupControl(SoAd_RoutingGroupIdType id, boolean enFlag);
#endif

/* TpTransmit to specification socket connection id */
#if (STD_ON == SOAD_SUPPORT_TX_BUFFER)
/**
 * @brief         Transmits a TP PDU on a specific socket connection.
 * @param[in]     SoConId: The ID of the socket connection.
 * @param[in]     PduRouteDestId: The ID of the PDU route destination.
 * @param[in]     SoAdSrcPduInfoPtr: Pointer to the source PDU information.
 * @param[in]     partIndex: The partition index.
 * @return        E_OK if successful, E_NOT_OK otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74711
 */
SOAD_LOCAL Std_ReturnType SoAd_TpTransmitSoCon(
    SoAd_SoConIdType                   SoConId,
    SoAd_InnerSocketConManageType*     soConMgmtPtr,
    SoAd_uintx                         PduRouteDestId,
    const PduInfoType*                 SoAdSrcPduInfoPtr,
    SoAd_InnerPduRouteDestManagerType* pduRouteDestMPtr);
#endif

#if (STD_ON == SOAD_SUPPORT_UDP)
/**
 * @brief         Closes a UDP socket connection group.Called by SoAd_TcpIpEvent.
 * @param[in]     Event: The TCP/IP event.
 * @param[in]     SoConGroupId: The ID of the socket connection group.
 * @param[in]     partIndex: The partition index.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74712
 */
SOAD_LOCAL void SoAd_CloseUdpSoConGroup(TcpIp_EventType Event, SoAd_uintx soConGroupId);
#endif

/*Called by SoAd_TxConfirmation.
 *Tcp TxConfirmation handle*/
#if (STD_ON == SOAD_SUPPORT_TCP)
#if (SOAD_PDU_ROUTE_DEST_MAX > 0)
/**
 * @brief         Handles the TCP transmission confirmation.
 * @param[in]     soConId: The ID of the socket connection.
 * @param[in]     Length: Length of the data.
 * @param[in]     pduRouteDestId: The ID of the PDU route destination.
 * @param[in]     partIndex: The partition index.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74713
 */
SOAD_LOCAL void SoAd_TxConfirmationHandle(SoAd_SoConIdType soConId, uint16 Length, SoAd_uintx pduRouteDestId);
#endif
#endif

#if (STD_ON == SOAD_SUPPORT_HEADERID)
/**
 * @brief         Finds the header ID and socket route ID.
 * @param[in]     SoConId: The ID of the socket connection.
 * @param[in]     HeaderId: The header ID.
 * @param[out]    SocketRouteID: The ID of the socket route.
 * @return        TRUE if the header ID and socket route ID are found, FALSE otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74714
 */
SOAD_LOCAL boolean
    SoAd_FindHeaderIDAndSocketRouteId(SoAd_SoConIdType SoConId, uint32 HeaderId, SoAd_uintx* SocketRouteID);
#endif

#if (STD_ON == SOAD_SUPPORT_TX_BUFFER) || (STD_ON == SOAD_SUPPORT_RX_BUFFER)
/**
 * @brief         Initializes the transmit and receive buffers.
 * @param[in]     partIndex: The partition index.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74715
 */
SOAD_LOCAL void SoAd_TxRxBufferInit(uint16 partIndex);
#endif

/**
 * @brief     Match TCP socket ID
 * @param[in] socketId: The ID of the TCP/IP socket.
 * @param[out] soConIdPtr: Pointer to the socket connection ID.
 * @param[in] tpPduTransmitting: Flag to indicate if TP PDU is transmitting.
 * @return    TRUE if socket ID is valid, FALSE otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74716
 */
SOAD_LOCAL boolean
    SoAd_MatchTCPSocketId(TcpIp_SocketIdType socketId, SoAd_SoConIdType* soConIdPtr, boolean tpPduTransmitting);
#if (STD_ON == SOAD_SUPPORT_TCP)
/**
 * @brief     Match TCP socket group socket ID
 * @param[in] socketId: The ID of the TCP/IP socket.
 * @param[out] soConGroupIdPtr: Pointer to the socket connection group ID.
 * @return    TRUE if socket group socket ID is valid, FALSE otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74717
 */
SOAD_LOCAL boolean SoAd_MatchTCPSoConGroupSocketId(TcpIp_SocketIdType socketId, SoAd_uintx* soConGroupIdPtr);
#endif

#if (STD_ON == SOAD_DEV_ERROR_DETECT)
/**
 * @brief     Check initialization status of the SoAd module.
 * @param[in] apiId: The API ID.
 * @param[out] applicationIdPtr: Pointer to the application ID.
 * @return    TRUE if initialization is successful, FALSE otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74718
 */
SOAD_LOCAL boolean SoAd_DetCheckInitStatus(uint8 apiId, ApplicationType* applicationIdPtr);
/**
 * @brief     Check TX PDU ID
 * @param[in] apiId: The API ID.
 * @param[in] txPduId: The ID of the TX PDU.
 * @param[in] checkULType: Flag to check upper layer type.
 * @param[in] type: The upper layer type.
 * @return    TRUE if TX PDU ID is valid, FALSE otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74719
 */
SOAD_LOCAL boolean SoAd_DetCheckTxPduId(uint8 apiId, PduIdType txPduId, boolean checkULType, SoAd_UpperLayerType type);
/**
 * @brief     Check socket route destination ID
 * @param[in] apiId: The API ID.
 * @param[in] pduId: The ID of the PDU.
 * @param[in] checkULType: Flag to check upper layer type.
 * @param[in] type: The upper layer type.
 * @return    TRUE if socket route destination ID is valid, FALSE otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74720
 */
SOAD_LOCAL boolean
    SoAd_DetCheckSocketRouteDestId(uint8 apiId, PduIdType pduId, boolean checkULType, SoAd_UpperLayerType type);
/**
 * @brief     Check routing group ID
 * @param[in] apiId: The API ID.
 * @param[in] routingGroupId: The ID of the routing group.
 * @return    TRUE if routing group ID is valid, FALSE otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74721
 */
SOAD_LOCAL boolean SoAd_DetCheckRoutingGroupId(uint8 apiId, SoAd_RoutingGroupIdType routingGroupId);
/**
 * @brief     Check routing group ID
 * @param[in] apiId: The API ID.
 * @param[in] routingGroupId: The ID of the routing group.
 * @return    TRUE if routing group ID is valid, FALSE otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74722
 */
SOAD_LOCAL boolean SoAd_DetCheckRoutingGroupTxTriggerable(uint8 apiId, SoAd_RoutingGroupIdType routingGroupId);
/**
 * @brief     Check socket connection ID
 * @param[in] apiId: The API ID.
 * @param[in] soConId: The ID of the socket connection.
 * @return    TRUE if socket connection ID is valid, FALSE otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74723
 */
SOAD_LOCAL boolean SoAd_DetCheckSoConId(uint8 apiId, SoAd_SoConIdType soConId);
/**
 * @brief Checks if automatic socket connection setup is enabled for a given socket connection.
 * @param[in] apiId: The API identifier.
 * @param[in] soConId: The ID of the socket connection.
 * @return TRUE if automatic socket connection setup is enabled, FALSE otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74724
 */
SOAD_LOCAL boolean SoAd_DetCheckAutoSoConSetup(uint8 apiId, SoAd_SoConIdType soConId);
/**
 * @brief Checks if a pointer parameter is valid (not NULL).
 * @param[in] apiId: The API identifier.
 * @param[in] parameterPtr: The pointer to check.
 * @return TRUE if the pointer is valid, FALSE otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74725
 */
SOAD_LOCAL boolean SoAd_DetCheckPtrParameter(uint8 apiId, const void* parameterPtr);
/**
 * @brief Checks if a length parameter is valid (not zero).
 * @param[in] apiId: The API identifier.
 * @param[in] length: The length to check.
 * @return TRUE if the length is valid, FALSE otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74726
 */
SOAD_LOCAL boolean SoAd_DetCheckLength(uint8 apiId, uint16 length);
/**
 * @brief Checks if the length of data to be transmitted without acknowledgment is valid.
 * @param[in] apiId: The API identifier.
 * @param[in] length: The length of data to be transmitted.
 * @param[in] txNoAckLength: The length of data that can be transmitted without acknowledgment.
 * @return TRUE if the length is valid, FALSE otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74727
 */
SOAD_LOCAL boolean SoAd_DetCheckTCPTxNoAckLength(uint8 apiId, uint16 length, PduLengthType txNoAckLength);
/**
 * @brief Checks if a TCP event is valid.
 * @param[in] apiId: The API identifier.
 * @param[in] Event: The TCP event to check.
 * @return TRUE if the event is valid, FALSE otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74728
 */
SOAD_LOCAL boolean SoAd_DetCheckTCPEvent(uint8 apiId, TcpIp_EventType Event);
/**
 * @brief Checks if a socket ID is valid.
 * @param[in] apiId: The API identifier.
 * @param[in] socketIdValid: The validity of the socket ID.
 * @return TRUE if the socket ID is valid, FALSE otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74729
 */
SOAD_LOCAL boolean SoAd_DetCheckSocketIdValid(uint8 apiId, boolean socketIdValid);
/**
 * @brief Checks if the domain type of a socket is valid.
 * @param[in] apiId: The API identifier.
 * @param[in] domainType: The domain type to check.
 * @param[in] soConId: The ID of the socket connection.
 * @return TRUE if the domain type is valid, FALSE otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74730
 */
SOAD_LOCAL boolean SoAd_DetCheckTcpIpDomainType(uint8 apiId, TcpIp_DomainType domainType, SoAd_SoConIdType soConId);
/**
 * @brief Checks if the partition of a transmit PDU ID is valid.
 * @param[in] apiId: The API identifier.
 * @param[in] txPduId: The ID of the transmit PDU.
 * @param[in] curAppId: The current application ID.
 * @return TRUE if the partition is valid, FALSE otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74731
 */
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
SOAD_LOCAL boolean SoAd_DetCheckTxPduIdPartition(uint8 apiId, PduIdType txPduId, ApplicationType curAppId);

/**
 * @brief Checks if the partition of a receive PDU ID is valid.
 * @param[in] apiId: The API identifier.
 * @param[in] pduId: The ID of the receive PDU.
 * @param[in] curAppId: The current application ID.
 * @return TRUE if the partition is valid, FALSE otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74732
 */
SOAD_LOCAL boolean SoAd_DetCheckRxPduIdPartition(uint8 apiId, PduIdType pduId, ApplicationType curAppId);

/**
 * @brief Checks if the partition of a routing group ID is valid.
 * @param[in] apiId: The API identifier.
 * @param[in] routingGroupId: The ID of the routing group.
 * @param[in] curAppId: The current application ID.
 * @return TRUE if the partition is valid, FALSE otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74733
 */
SOAD_LOCAL boolean
    SoAd_DetCheckRoutingGroupIdPartition(uint8 apiId, SoAd_RoutingGroupIdType routingGroupId, ApplicationType curAppId);
#endif
/**
 * @brief Checks if the partition of a socket connection group ID is valid.
 * @param[in] apiId: The API identifier.
 * @param[in] soConGroupId: The ID of the socket connection group.
 * @param[in] curAppId: The current application ID.
 * @return TRUE if the partition is valid, FALSE otherwise.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74734
 */
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
SOAD_LOCAL boolean SoAd_DetCheckSoConIdPartition(uint8 apiId, SoAd_uintx soConGroupId, ApplicationType curAppId);
#endif
#endif

#if (STD_ON == SOAD_PDU_ROUTE_METADATA_SUPPORT)
/**
 * @brief match soConid of metadata in configuration txPdu
 * @param[in] txPduId: The API identifier.
 * @param[in] pduRouteDestId: The ID of the socket connection group.
 * @param[in] txSoConId: The current application ID.
 * @return        boolean
 * @retval        TRUE  : match.
 * @retval        FALSE : not match.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-78701
 */
SOAD_LOCAL boolean SoAd_MatchMetaDataSoConId(PduIdType txPduId, SoAd_uintx pduRouteDestId, SoAd_SoConIdType txSoConId);
#endif

#if (STD_ON == SOAD_SUPPORT_TCP)
/**
 * @brief handle TCP event
 * @param[in] soConId: socket connection id.
 * @param[in] Event: event of TcpIp.
 * @param[in] partIndex: partition id.
 * @param[in] soConMgmtPtr: socket connection management pointer.
 * @return        void
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-78702
 */
SOAD_LOCAL void SoAd_HandleTcpIpEventTCP(
    SoAd_SoConIdType               soConId,
    TcpIp_EventType                Event,
    SoAd_InnerSocketConManageType* soConMgmtPtr);
#endif

#if (STD_ON == SOAD_SUPPORT_HEADERID) && (0u < SOAD_SOCKET_ROUTE_DEST_MAX)
/**
 * @brief handle enable header reception message
 * @param[in] soConId: socket connection id.
 * @param[in] partIndex: partition id.
 * @param[in] bufPtr: reception message.
 * @param[in] length: length of reception message.
 * @return        boolean
 * @retval        TRUE  : reception message is valid.
 * @retval        FALSE : reception message is invalid.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-78703
 */
SOAD_LOCAL boolean SoAd_HandleRxIndicationEnableHeader(
    SoAd_SoConIdType               soConId,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    uint8*                         bufPtr,
    uint16                         length);
#endif
#if (0u < SOAD_SOCKET_ROUTE_DEST_MAX)
/**
 * @brief handle disable header reception message
 * @param[in] soConId: socket connection id.
 * @param[in] partIndex: partition id.
 * @param[in] bufPtr: reception message.
 * @param[in] length: length of reception message.
 * @return        boolean
 * @retval        TRUE  : reception message is valid.
 * @retval        FALSE : reception message is invalid.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-78704
 */
SOAD_LOCAL boolean SoAd_HandleRxIndicationDisableHeader(
    SoAd_SoConIdType               soConId,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    uint8*                         bufPtr,
    uint16                         length);
#endif

#if (STD_ON == SOAD_SUPPORT_UDP)
/**
 * @brief find valid udp socket connection
 * @param[in] soConGroupId: socket connection group id.
 * @param[in] RemoteAddrPtr: remote address.
 * @param[in] UdpParaPtr: udp parament.
 * @param[in] partIndex: partition id.
 * @param[in] soConIdPtr: socket connection id pointer.
 * @return        boolean
 * @retval        TRUE  : valid.
 * @retval        FALSE : invalid.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-78705
 */
SOAD_LOCAL boolean SoAd_FindUdpValidSoCon(
    SoAd_uintx                  soConGroupId,
    const TcpIp_SockAddrType*   RemoteAddrPtr,
    SoAd_FindTheValidSoConType* UdpParaPtr,
    SoAd_SoConIdType*           soConIdPtr);
#endif

#if (STD_ON == SOAD_SUPPORT_TCP)
/**
 * @brief handle open TCP socket connection
 * @param[in] soConId: socket connection id.
 * @param[in] soConGroup: socket connection group id.
 * @param[in] partIndex: partition id.
 * @param[in] socketIdValidPtr: socked id valid flag.
 * @return        none
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-78706
 */
SOAD_LOCAL void SoAd_OpenTCPSoConHandle(
    SoAd_SoConIdType                    soConId,
    SoAd_uintx                          soConGroup,
    SoAd_InnerSocketConManageType*      soConMgmtPtr,
    SoAd_InnerSocketConGroupManageType* soConGroupMgmtPtr);
#endif

#if (STD_ON == SOAD_SUPPORT_UDP)
/**
 * @brief Attempt open UDP socket connection
 * @param[in] soConId: socket connection id.
 * @param[in] soConGroup: socket connection group id.
 * @param[in] partIndex: partition id.
 * @return        none
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-78707
 */
SOAD_LOCAL void SoAd_AttemptOpenUDPSoCon(
    SoAd_SoConIdType                    soConId,
    SoAd_uintx                          soConGroup,
    SoAd_InnerSocketConManageType*      soConMgmtPtr,
    SoAd_InnerSocketConGroupManageType* soConGroupMgmtPtr);
#endif

/**
 * @brief close socket connection socket
 * @param[in] soConId: socket connection id.
 * @param[in] closeSocket: if close socket or not.
 * @param[in] soConGroupMgmtPtr: socket connection group management pointer.
 * @param[in] soConMgmtPtr: socket connection management pointer.
 * @param[in] partIndex: partition id.
 * @return        none
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-78708
 */
SOAD_LOCAL void SoAd_CloseSoConTcpIpSocket(
    SoAd_SoConIdType                    soConId,
    boolean                             closeSocket,
    SoAd_InnerSocketConGroupManageType* soConGroupMgmtPtr,
    SoAd_InnerSocketConManageType*      soConMgmtPtr);

/**
 * @brief control routing group specific socket connection
 * @param[in] id: routing group id.
 * @param[in] enFlag: enable or disable.
 * @param[in] soConId: socket connection id.
 * @param[in] partIndex: partition id.
 * @return        none
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-78709
 */
SOAD_LOCAL Std_ReturnType
    SoAd_RoutingGroupControlSpcificSoCon(SoAd_RoutingGroupIdType id, boolean enFlag, SoAd_SoConIdType soConId);

#if (STD_ON == SOAD_SUPPORT_TCP)
/**
 * @brief         Close  TCP socket connection group.Called by SoAd_TcpIpEvent.
 * @param[in]     Event: The TCP/IP event.
 * @param[in]     SoConGroupId: The ID of the socket connection group.
 * @param[in]     soConGroupMgmtPtr: The socket connection group manager pointer.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-78710
 */
SOAD_LOCAL void SoAd_CloseTcpSoConGroup(
    TcpIp_EventType                     event,
    SoAd_uintx                          soConGroupId,
    SoAd_InnerSocketConGroupManageType* soConGroupMgmtPtr);
#endif

#if (STD_ON == SOAD_SUPPORT_RX_BUFFER)
#if (STD_ON == SOAD_SUPPORT_HEADERID)
/**
 * @brief         Handle Header Enable received Tp Pdu.
 * @param[in]     pduLength: Length of Pdu.
 * @param[in]     soConId: socket connection id.
 * @param[in]     socketRouteId: socket route id.
 * @param[in]     partIndex: partition id.
 * @param[in]     soConMgmtPtr: socket connection manager pointer.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-78711
 */
SOAD_LOCAL void SoAd_RxTpPduHeaderEnableHandle(
    PduLengthType                  pduLength,
    SoAd_SoConIdType               soConId,
    SoAd_uintx                     socketRouteId,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    SoAd_SoRxBufferManType*        soRxBuffMgmtPtr);
#endif
#endif
/**
 * @brief         Initialize socket connection manager.
 * @param[in]     partIndex: partition id.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-78723
 */
SOAD_LOCAL void SoAd_SoConManagerInit(void);
#define SOAD_STOP_SEC_CODE
#include "SoAd_MemMap.h"

/* ========================================== external function definition ========================================== */
#define SOAD_START_SEC_CODE
#include "SoAd_MemMap.h"

/**
 * Initializes internal variables of the Socket Adaptor.
 */
/* PRQA S 6010,6030,1503 ++ */ /* VL_MTR_SoAd_STCYC,VL_MTR_SoAd_STMIF,VL_QAC_NoUsedApi */
void SoAd_Init(const SoAd_ConfigType* SoAdConfigPtr)
/* PRQA S 6010,6030,1503 -- */
{
    uint16 partIndex = 0u;
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
    ApplicationType curAppId = GetApplicationID();
    for (uint16 idx = 0u; idx < SOAD_PARTITION_MAX; idx++)
    {
        if (SoAd_Partition[idx].ApplicationId == curAppId)
        {
            partIndex = SoAd_Partition[idx].PartitionIndex;
            break;
        }
    }
#endif

#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /*SWS_SoAd_00216*/
    if (NULL_PTR == SoAdConfigPtr)
    {
        SOAD_DET_REPORT(SOAD_SID_INIT, SOAD_E_INIT_FAILED);
    }
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
    else if (partIndex >= SOAD_PARTITION_NUM)
    {
        SOAD_DET_REPORT(SOAD_SID_INIT, SOAD_E_INVALID_PARTITION);
    }
#endif
    else
#endif
    {
        if (SoAd_InitStatus != SOAD_STATE_INIT)
        {
            SoAd_uintx                          cnt;
            SoAd_InnerSocketConGroupManageType* soConGroupMgmtPtr;

            /*SWS_SoAd_00211*/
            SoAd_PbCfgPtr = SoAdConfigPtr;
#if (STD_ON == SOAD_SUPPORT_RX_BUFFER) || (STD_ON == SOAD_SUPPORT_TX_BUFFER)
            SoAd_TxBufferNum = SoAd_PbCfgPtr->SocketTxBufferNum;
            SoAd_RxBufferNum = SoAd_PbCfgPtr->SocketRxBufferNum;
#endif
            SoAd_SoConManagerPtr      = SoAd_PbCfgPtr->SoConManagerPtr;
            SoAd_SoConGroupManagerPtr = SoAd_PbCfgPtr->SoConGroupManagerPtr;
#if (SOAD_ROUTING_GROUP_MAX > 0u)
            SoAd_RoutGroupManagerPtr = SoAd_PbCfgPtr->RoutingGroupManagerPtr;
#endif
#if (SOAD_SOCKET_ROUTE_DEST_MAX > 0u)
            SoAd_SoRouteDestManagerPtr = SoAd_PbCfgPtr->SocketRouteDestManagerPtr;
#endif
#if (SOAD_PDU_ROUTE_DEST_MAX > 0u)
            SoAd_PduRouteDestManagerPtr = SoAd_PbCfgPtr->PduRouteDestManagerPtr;
#endif
#if (STD_ON == SOAD_SUPPORT_TX_BUFFER)
            SoAd_SoTxBuffManagerPtr = SoAd_PbCfgPtr->SocketTxBufManPtr;
#endif
#if (STD_ON == SOAD_SUPPORT_RX_BUFFER)
            SoAd_SoRxBuffManagerPtr = SoAd_PbCfgPtr->SocketRxBufManPtr;
#endif
#if (SOAD_PDU_ROUTE_MAX > 0u)
            SoAd_PduRouteManagerPtr = SoAd_PbCfgPtr->PduRoutManagerPtr;
#endif
            SoAd_SoConCfgPtr        = SoAd_PbCfgPtr->SocketConnection;
            SoAd_SoConGroupCfgPtr   = SoAd_PbCfgPtr->SocketConnectionGroup;
            SoAd_PduRouteCfgPtr     = SoAd_PbCfgPtr->PduRoute;
            SoAd_SoRouteDestCfgPtr  = SoAd_PbCfgPtr->SocketRouteDestTotalPtr;
            SoAd_PduRouteDestCfgPtr = SoAd_PbCfgPtr->PduRouteDestTotalPtr;
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
            SoAd_UpperLayerPCCfgPtr = SoAd_PcCfg.UpLayerFuncTable;
#endif
            SoAd_RouteGroupCfgPtr   = SoAd_PbCfgPtr->RoutingGroup;
            SoAd_SocketBufferCfgPtr = SoAd_PbCfgPtr->SocketBufferCfgPtr;
            SoAd_SocketRouteCfgPtr  = SoAd_PbCfgPtr->SocketRoute;
            SoAd_SoConNum           = SoAd_PbCfgPtr->SocketConnectionNum;

            /*SWS_SoAd_00723*/
            SoAd_SoConManagerInit();

#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION) && (SOAD_PARTITION_NUM > 1u)
            const SoAd_SocketConnectionGroupType* soConGroupCfgPtr;
#endif
            /*init SoAd_SoConGroupManage parameter*/
            for (cnt = 0u; cnt < SOAD_SO_CON_GROUP_NUM; cnt++)
            {
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION) && (SOAD_PARTITION_NUM > 1u)
                soConGroupCfgPtr = &SoAd_SoConGroupCfgPtr[cnt];
                if (soConGroupCfgPtr->PartitionId != curAppId)
                {
                    continue;
                }

                partIndex         = SoAd_GetPartitionIndex(soConGroupCfgPtr->PartitionId);
                soConGroupMgmtPtr = &SoAd_SoConGroupManagerPtr[partIndex][soConGroupCfgPtr->PartitionSoConGroupId];
#else
                        soConGroupMgmtPtr = &SoAd_SoConGroupManagerPtr[cnt];
#endif
                soConGroupMgmtPtr->LocalIpAddrState    = TCPIP_IPADDR_STATE_UNASSIGNED;
                soConGroupMgmtPtr->SoConSocketValidNum = 0u;
                soConGroupMgmtPtr->TcpIpSocketId       = (TcpIp_SocketIdType)TCPIP_SOCKETID_INVALID;
                soConGroupMgmtPtr->TcpIpSocketValid    = FALSE;
            }
            /* routing group init */
            SoAd_RoutingGroupInit();

            /* socket connection link buffer management data init */
#if (STD_ON == SOAD_SUPPORT_TX_BUFFER) || (STD_ON == SOAD_SUPPORT_RX_BUFFER)
            SoAd_TxRxBufferInit(partIndex);
#endif

#if (0u < SOAD_PDU_ROUTE_MAX)
            for (cnt = 0u; cnt < SOAD_PDU_ROUTE_NUM; cnt++)
            {
                SoAd_PduRouteManagerInit(cnt);
            }
#endif
            SoAd_InitStatus = SOAD_STATE_INIT;
        }
    }
}

/**
 *  Returns socket connection index related to the specified TxPduId.
 */
Std_ReturnType SoAd_GetSoConId(PduIdType TxPduId, SoAd_SoConIdType* SoConIdPtr)
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /**
     * SWS_SoAd_00609, SWS_SoAd_00610
     */
    if (SoAd_DetCheckInitStatus(SOAD_SID_GETSOCONID, NULL_PTR)
        && SoAd_DetCheckTxPduId(SOAD_SID_GETSOCONID, TxPduId, FALSE, SOAD_UPPER_LAYER_IF))
#endif
    {
        /*SWS_SoAd_00724*/
        if (1u == SoAd_PduRouteCfgPtr[TxPduId].PduRouteDestNum)
        {
            SoAd_uintx pduRouteDestId = SoAd_PduRouteCfgPtr[TxPduId].PduRouteDestStartId;
            *SoConIdPtr               = SoAd_PduRouteDestCfgPtr[pduRouteDestId].TxSocketConRefIndexPtr[0];
            result                    = E_OK;
        }
    }
    return result;
}

/**
 * Returns the version information.
 */
#if (STD_ON == SOAD_VERSION_INFO_API)
void SoAd_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
#if (STD_ON == SOAD_DEV_ERROR_DETECT)

    if (NULL_PTR == versioninfo)
    {
        SOAD_DET_REPORT(SOAD_SID_GETVERSIONINFO, SOAD_E_PARAM_POINTER);
    }
    else
#endif
    {
        versioninfo->moduleID         = (uint16)SOAD_MODULE_ID;
        versioninfo->vendorID         = (uint16)SOAD_VENDOR_ID;
        versioninfo->sw_major_version = (uint8)SOAD_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = (uint8)SOAD_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = (uint8)SOAD_SW_PATCH_VERSION;
    }
}
#endif

/**
 * Requests transmission of an IF I-PDU.
 */
/* PRQA S 6030,6070 ++ */ /* VL_MTR_SoAd_STMIF,VL_MTR_SoAd_STCAL */
Std_ReturnType SoAd_IfTransmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr)
/* PRQA S 6030,6070 -- */
{
    Std_ReturnType result = E_NOT_OK;

#if (0u < SOAD_MAX_IF_PDU_SIZE)
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /**
     * SWS_SoAd_00213,SWS_SoAd_00214
     */
    ApplicationType curAppId;
    if (SoAd_DetCheckInitStatus(SOAD_SID_IFTRANSMIT, &curAppId)
        && SoAd_DetCheckTxPduId(SOAD_SID_IFTRANSMIT, TxPduId, TRUE, SOAD_UPPER_LAYER_IF)
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
        && SoAd_DetCheckTxPduIdPartition(SOAD_SID_IFTRANSMIT, TxPduId, curAppId)
#endif
    )
#endif
    {
#if (SOAD_PDU_ROUTE_DEST_MAX > 0)
        SoAd_InnerPduRoutManageType* pduRouteMgmtPtr;
        const SoAd_PduRouteType*     pduRouteCfgPtr = &SoAd_PduRouteCfgPtr[TxPduId];
#if (SOAD_PARTITION_NUM > 1u)
        uint16 partIndex = SoAd_GetPartitionIndex(pduRouteCfgPtr->PartitionId);
        pduRouteMgmtPtr  = &SoAd_PduRouteManagerPtr[partIndex][pduRouteCfgPtr->PartitionPduRouteId];
#else
        pduRouteMgmtPtr = &SoAd_PduRouteManagerPtr[TxPduId];
#endif
        /*the TxPdu is transmitting,refuse the new transmit request*/
        if (SOAD_UNUSED_UINT8 == pduRouteMgmtPtr->TxPendingNum)
        {
            /*1.get pdu data from up module*/
            result = SoAd_GetIfPduData(pduRouteCfgPtr, pduRouteMgmtPtr, PduInfoPtr);
            if (E_OK == result)
            {
                /*2.check the PduRouteDest/SoCon transmit condition and transmit the pdu on related SoCon(s)*/
                SoAd_InnerSocketConManageType* soConMgmtPtr;
                SoAd_uintx                     pduRouteDestNum = pduRouteCfgPtr->PduRouteDestNum;
                result                                         = E_NOT_OK;
                SoAd_uintx                   pduRouteDestId    = SOAD_PDUROT_DESTINDEX_CFG(TxPduId);
                const SoAd_PduRouteDestType* pduRouteDestCfgPtr;

                for (SoAd_uintx cnt = 0u; cnt < pduRouteDestNum; cnt++)
                {
                    pduRouteDestId += cnt;
                    pduRouteDestCfgPtr = &SoAd_PduRouteDestCfgPtr[pduRouteDestId];
#if (SOAD_PARTITION_NUM > 1u)
                    PduIdType id = pduRouteDestCfgPtr->PartitionPduRouteDestId;
                    /*SWS_SoAd_00560*/
                    if (SoAd_PduRouteDestManagerPtr[partIndex][id].PduRouteDestEnable)
#else
                    /*SWS_SoAd_00560*/
                    if (SoAd_PduRouteDestManagerPtr[pduRouteDestId].PduRouteDestEnable)
#endif
                    {
                        result = E_NOT_OK;
#if (STD_ON == SOAD_PDU_ROUTE_METADATA_SUPPORT)
                        if (!SoAd_MatchMetaDataSoConId(TxPduId, pduRouteDestId, pduRouteMgmtPtr->TxSoConId))
                        {
                            result = E_NOT_OK;
                            break;
                        }
#endif
                        SoAd_SoConIdType soConId = pduRouteDestCfgPtr->TxSocketConRefIndexPtr[0];
#if (SOAD_PARTITION_NUM > 1u)
                        soConMgmtPtr = &SoAd_SoConManagerPtr[partIndex][SoAd_SoConCfgPtr[soConId].PartitionSoConId];
#else
                        soConMgmtPtr = &SoAd_SoConManagerPtr[soConId];
#endif

                        if (((SOAD_NO_TXMETADATA == pduRouteMgmtPtr->TxSoConId)
                             || (soConId == pduRouteMgmtPtr->TxSoConId))
                            && (SOAD_SOCON_ONLINE == soConMgmtPtr->SoConMode)
                            && (SOAD_SOCKET_CLOSE_NONE == soConMgmtPtr->SocketNeedClose)
                            && (!SoAd_CheckTCPTpTransmiting(soConId, soConMgmtPtr->TpPduTransmitting))
                            && (!SoAd_CheckUDPSoConOnlyListen(soConId)))
                        {
                            if (E_OK
                                != SoAd_IfTransmitSoCon(
                                    TxPduId,
                                    pduRouteMgmtPtr,
                                    pduRouteDestId,
                                    soConMgmtPtr,
                                    soConId))
                            {
                                result = E_NOT_OK;
                                break;
                            }
                            else
                            {
                                result = E_OK;
                            }
                        }
                    }
                }
            }
        }
#endif
    }
#endif
    SOAD_NOUSED(TxPduId);
    SOAD_NOUSED(PduInfoPtr);
    return result;
}

/**
 * Triggers the transmission of all If-TxPDUs identified by the parameter id after requesting the data from the
 * related upper layer.
 */
/* PRQA S 1503,6030,6070 ++ */ /* VL_QAC_NoUsedApi,VL_MTR_SoAd_STMIF,VL_MTR_SoAd_STCAL */
Std_ReturnType SoAd_IfRoutingGroupTransmit(SoAd_RoutingGroupIdType id)
/* PRQA S 1503,6030,6070 -- */
{
    Std_ReturnType result = E_NOT_OK;

#if ((0u < SOAD_ROUTING_GROUP_MAX) && (0u < SOAD_MAX_IF_PDU_SIZE))
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /**
     * SWS_SoAd_00661,SWS_SoAd_00658
     */
    ApplicationType curAppId;
    if (SoAd_DetCheckInitStatus(SOAD_SID_IFROUTINGGROUPTRANSMIT, &curAppId)
        && SoAd_DetCheckRoutingGroupId(SOAD_SID_IFROUTINGGROUPTRANSMIT, id)
        && SoAd_DetCheckRoutingGroupTxTriggerable(SOAD_SID_IFROUTINGGROUPTRANSMIT, id)
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
        && SoAd_DetCheckRoutingGroupIdPartition(SOAD_SID_IFROUTINGGROUPTRANSMIT, id, curAppId)
#endif
    )
#endif
    {
#if (SOAD_PDU_ROUTE_DEST_MAX > 0)
        SoAd_uintx                               cnt, pduRouteDestId, pduRouteDestNum;
        PduIdType                                txPduId = SOAD_PDUID_INVALID;
        SoAd_InnerPduRoutManageType*             pduRouteMgmtPtr;
        SoAd_InnerSocketConManageType*           soConMgmtPtr;
        const SoAd_RoutingGroupType*             routingGroupCfgPtr = &SoAd_RouteGroupCfgPtr[id];
        const SoAd_InnerPduRouteDestManagerType* pduRouteDestMgmtPtr;
        const SoAd_PduRouteDestType*             pduRouteDestCfgPtr;
        const SoAd_PduRouteType*                 pduRouteCfgPtr;
#if (SOAD_PARTITION_NUM > 1u)
        uint16 partIndex    = SoAd_GetPartitionIndex(routingGroupCfgPtr->PartitionId);
        pduRouteMgmtPtr     = SoAd_PduRouteManagerPtr[partIndex];
        pduRouteDestMgmtPtr = SoAd_PduRouteDestManagerPtr[partIndex];
        soConMgmtPtr        = SoAd_SoConManagerPtr[partIndex];
#endif

        pduRouteDestNum = routingGroupCfgPtr->PduRouteDestNum;
        for (cnt = 0u; cnt < pduRouteDestNum; cnt++)
        {
            pduRouteDestId       = routingGroupCfgPtr->PduRouteDestIndexPtr[cnt];
            pduRouteDestCfgPtr   = &SoAd_PduRouteDestCfgPtr[pduRouteDestId];
            PduIdType pduRouteId = pduRouteDestCfgPtr->PduRouteIndex;
            pduRouteCfgPtr       = &SoAd_PduRouteCfgPtr[pduRouteId];
#if (SOAD_PARTITION_NUM > 1u)
            pduRouteMgmtPtr     = pduRouteMgmtPtr + pduRouteCfgPtr->PartitionPduRouteId;
            pduRouteDestMgmtPtr = pduRouteDestMgmtPtr + pduRouteDestCfgPtr->PartitionPduRouteDestId;
#else
            pduRouteMgmtPtr     = &SoAd_PduRouteManagerPtr[pduRouteId];
            pduRouteDestMgmtPtr = &SoAd_PduRouteDestManagerPtr[pduRouteDestId];
#endif

            if ((SOAD_UPPER_LAYER_IF == pduRouteCfgPtr->TxUpperLayerType)
                && (SOAD_UNUSED_UINT8 == pduRouteMgmtPtr->TxPendingNum) && pduRouteDestMgmtPtr->PduRouteDestEnable)
            {
                SoAd_SoConIdType soConId = pduRouteDestCfgPtr->TxSocketConRefIndexPtr[0];
#if (SOAD_PARTITION_NUM > 1u)
                soConMgmtPtr = soConMgmtPtr + SoAd_SoConCfgPtr[soConId].PartitionSoConId;
#else
                soConMgmtPtr = &SoAd_SoConManagerPtr[soConId];
#endif
                if ((SOAD_SOCON_ONLINE == soConMgmtPtr->SoConMode)
                    && (SOAD_SOCKET_CLOSE_NONE == soConMgmtPtr->SocketNeedClose)
                    && (!SoAd_CheckTCPTpTransmiting(soConId, soConMgmtPtr->TpPduTransmitting)))
                {
                    if (txPduId != pduRouteId)
                    {
                        (void)SoAd_GetIfPduData(pduRouteCfgPtr, pduRouteMgmtPtr, NULL_PTR);
                        txPduId = pduRouteId;
                    }

                    if ((NULL_PTR != pduRouteMgmtPtr->PduDataPtr)
                        && ((SOAD_NO_TXMETADATA == pduRouteMgmtPtr->TxSoConId)
                            || (soConId == pduRouteMgmtPtr->TxSoConId))
                        && (!SoAd_CheckUDPSoConOnlyListen(soConId)))
                    {
                        (void)SoAd_IfTransmitSoCon(pduRouteId, pduRouteMgmtPtr, pduRouteDestId, soConMgmtPtr, soConId);
                    }
                }
            }
        }
        if (SOAD_PDUID_INVALID != txPduId)
        {
            result = E_OK;
        }
#endif
    }
#endif
    SOAD_NOUSED(id);
    return result;
}

/**
 * Triggers the transmission of all If-TxPDUs identified by the parameter id on the socket connection specified by
 *  SoConId after requesting the data from the related upper layer.
 */
/* PRQA S 1532,6030,6070 ++*/ /* VL_QAC_OneFunRef,VL_MTR_SoAd_STMIF,VL_MTR_SoAd_STCAL */
Std_ReturnType SoAd_IfSpecificRoutingGroupTransmit(SoAd_RoutingGroupIdType id, SoAd_SoConIdType SoConId)
/* PRQA S 1532,6030,6070 --*/
{
    Std_ReturnType result = E_NOT_OK;

#if ((0u < SOAD_ROUTING_GROUP_MAX) && (0u < SOAD_MAX_IF_PDU_SIZE))
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /**
     * SWS_SoAd_00712,SWS_SoAd_00713
     */
    ApplicationType curAppId;
    if (SoAd_DetCheckInitStatus(SOAD_SID_IFSPECIFICROUTINGGROUPTRANSMIT, &curAppId)
        && SoAd_DetCheckRoutingGroupId(SOAD_SID_IFSPECIFICROUTINGGROUPTRANSMIT, id)
        && SoAd_DetCheckRoutingGroupTxTriggerable(SOAD_SID_IFSPECIFICROUTINGGROUPTRANSMIT, id)
        && SoAd_DetCheckSoConId(SOAD_SID_IFSPECIFICROUTINGGROUPTRANSMIT, SoConId)
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
        && SoAd_DetCheckRoutingGroupIdPartition(SOAD_SID_IFSPECIFICROUTINGGROUPTRANSMIT, id, curAppId)
#endif
    )
#endif
    {
#if (SOAD_PDU_ROUTE_DEST_MAX > 0)
        SoAd_uintx                               cnt, pduRouteDestId, pduRouteDestNum;
        SoAd_InnerPduRoutManageType*             pduRouteMgmtPtr;
        SoAd_InnerSocketConManageType*           soConMgmtPtr;
        const SoAd_RoutingGroupType*             routingGroupCfgPtr = &SoAd_RouteGroupCfgPtr[id];
        const SoAd_InnerPduRouteDestManagerType* pduRouteDestMgmtPtr;
        const SoAd_PduRouteDestType*             pduRouteDestCfgPtr;
        const SoAd_PduRouteType*                 pduRouteCfgPtr;

#if (SOAD_PARTITION_NUM > 1u)
        uint16 partIndex    = SoAd_GetPartitionIndex(routingGroupCfgPtr->PartitionId);
        pduRouteMgmtPtr     = SoAd_PduRouteManagerPtr[partIndex];
        pduRouteDestMgmtPtr = SoAd_PduRouteDestManagerPtr[partIndex];
        soConMgmtPtr        = SoAd_SoConManagerPtr[partIndex];
#endif
        pduRouteDestNum = routingGroupCfgPtr->PduRouteDestNum;
        for (cnt = 0u; cnt < pduRouteDestNum; cnt++)
        {
            pduRouteDestId       = routingGroupCfgPtr->PduRouteDestIndexPtr[cnt];
            pduRouteDestCfgPtr   = &SoAd_PduRouteDestCfgPtr[pduRouteDestId];
            PduIdType pduRouteId = pduRouteDestCfgPtr->PduRouteIndex;
            pduRouteCfgPtr       = &SoAd_PduRouteCfgPtr[pduRouteId];
#if (SOAD_PARTITION_NUM > 1u)
            pduRouteMgmtPtr     = pduRouteMgmtPtr + pduRouteCfgPtr->PartitionPduRouteId;
            pduRouteDestMgmtPtr = pduRouteDestMgmtPtr + pduRouteDestCfgPtr->PartitionPduRouteDestId;
#else
            pduRouteMgmtPtr     = &SoAd_PduRouteManagerPtr[pduRouteId];
            pduRouteDestMgmtPtr = &SoAd_PduRouteDestManagerPtr[pduRouteDestId];
#endif

            if ((SOAD_UPPER_LAYER_IF == pduRouteCfgPtr->TxUpperLayerType)
                && (SOAD_UNUSED_UINT8 == pduRouteMgmtPtr->TxPendingNum) && pduRouteDestMgmtPtr->PduRouteDestEnable)
            {
                SoAd_SoConIdType soConId = pduRouteDestCfgPtr->TxSocketConRefIndexPtr[0];
#if (SOAD_PARTITION_NUM > 1u)
                soConMgmtPtr = soConMgmtPtr + SoAd_SoConCfgPtr[soConId].PartitionSoConId;
#else
                soConMgmtPtr = &SoAd_SoConManagerPtr[soConId];
#endif
                if ((SoConId == soConId) && (SOAD_SOCON_ONLINE == soConMgmtPtr->SoConMode)
                    && (SOAD_SOCKET_CLOSE_NONE == soConMgmtPtr->SocketNeedClose)
                    && (!SoAd_CheckTCPTpTransmiting(soConId, soConMgmtPtr->TpPduTransmitting))
                    && (!SoAd_CheckUDPSoConOnlyListen(soConId)))
                {
                    (void)SoAd_GetIfPduData(pduRouteCfgPtr, pduRouteMgmtPtr, NULL_PTR);
                    if ((NULL_PTR != pduRouteMgmtPtr->PduDataPtr)
                        && ((SOAD_NO_TXMETADATA == pduRouteMgmtPtr->TxSoConId)
                            || (soConId == pduRouteMgmtPtr->TxSoConId)))
                    {
                        (void)SoAd_IfTransmitSoCon(pduRouteId, pduRouteMgmtPtr, pduRouteDestId, soConMgmtPtr, soConId);
                    }
                    if (E_NOT_OK == result)
                    {
                        result = E_OK;
                    }
                }
            }
        }
#endif
    }
#endif
    SOAD_NOUSED(id);
    SOAD_NOUSED(SoConId);
    return result;
}

/**
 * Requests cancellation of the transmission via TP for a specific I-PDU.
 */
Std_ReturnType SoAd_TpCancelTransmit(PduIdType TxPduId)
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /**
     * SWS_SoAd_00605,SWS_SoAd_00606
     */
    ApplicationType curAppId;
    if (SoAd_DetCheckInitStatus(SOAD_SID_TPCANCELTRANSMIT, &curAppId)
        && SoAd_DetCheckTxPduId(SOAD_SID_TPCANCELTRANSMIT, TxPduId, TRUE, SOAD_UPPER_LAYER_TP)
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
        && SoAd_DetCheckTxPduIdPartition(SOAD_SID_TPCANCELTRANSMIT, TxPduId, curAppId)
#endif
    )
#endif
    {
#if (STD_ON == SOAD_SUPPORT_TX_BUFFER)
        /*TP PDU just reference one PduRouteDest(one SoCon),TP transmit just support 1:1*/
        SoAd_uintx                     pduRotDestIndex = SOAD_PDUROT_DESTINDEX_CFG(TxPduId);
        SoAd_SoConIdType               soConId = SoAd_PduRouteDestCfgPtr[pduRotDestIndex].TxSocketConRefIndexPtr[0];
        SoAd_InnerSocketConManageType* soConMgmtPtr;

#if (SOAD_PARTITION_NUM > 1u)
        uint16 partIndex = SoAd_GetPartitionIndex(SoAd_PduRouteDestCfgPtr[pduRotDestIndex].PartitionId);
        soConMgmtPtr     = &SoAd_SoConManagerPtr[partIndex][SoAd_SoConCfgPtr[soConId].PartitionSoConId];
#else
        soConMgmtPtr = &SoAd_SoConManagerPtr[soConId];
#endif
        if (soConMgmtPtr->TpPduTransmitting && (TxPduId == soConMgmtPtr->ActiveSoAdPduRouteId))
        {
            result                        = E_OK;
            soConMgmtPtr->SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
            soConMgmtPtr->SetSocketNeedCloseLine = __LINE__;
#endif
        }
#endif
    }
    SOAD_NOUSED(TxPduId);
    return result;
}

/**
 * Requests cancellation of the reception via TP for a specific I-PDU.
 */
Std_ReturnType SoAd_TpCancelReceive(PduIdType RxPduId)
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /**
     * SWS_SoAd_00607,SWS_SoAd_00608
     */
    ApplicationType curAppId;
    if (SoAd_DetCheckInitStatus(SOAD_SID_TPCANCELRECEIVE, &curAppId)
        && SoAd_DetCheckSocketRouteDestId(SOAD_SID_TPCANCELRECEIVE, RxPduId, TRUE, SOAD_UPPER_LAYER_TP)
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
        && SoAd_DetCheckRxPduIdPartition(SOAD_SID_TPCANCELRECEIVE, RxPduId, curAppId)
#endif
    )
#endif
    {
#if (STD_ON == SOAD_SUPPORT_RX_BUFFER)
        /*TP socket route can't reference SoConGroup*/
        SoAd_SoConIdType               soConId = *SoAd_SocketRouteCfgPtr[RxPduId].RxSocketConnRefIndexPtr;
        SoAd_InnerSocketConManageType* soConMgmtPtr;

#if (SOAD_PARTITION_NUM > 1u)
        uint16 partIndex = SoAd_GetPartitionIndex(SoAd_SocketRouteCfgPtr[RxPduId].PartitionId);
        soConMgmtPtr     = &SoAd_SoConManagerPtr[partIndex][SoAd_SoConCfgPtr[soConId].PartitionSoConId];
#else
        soConMgmtPtr = &SoAd_SoConManagerPtr[soConId];
#endif
        if (SOAD_SOCON_ONLINE == soConMgmtPtr->SoConMode)
        {
            result = E_OK;
#if (STD_ON == SOAD_SUPPORT_HEADERID)
            SoAd_uintx soConGroupId = SOAD_SOCON_REFGORUP(soConId);
            if (SoAd_SoConGroupCfgPtr[soConGroupId].PduHeaderEnable)
            {
                result              = E_NOT_OK;
                SoAd_uintx bufferId = SoAd_SoConCfgPtr[soConId].RxBuffCfgIndexPtr[0u] - SoAd_TxBufferNum;
#if (SOAD_PARTITION_NUM > 1u)
                SoAd_uintx                    startId        = bufferId - SOAD_MULTI_PART_INFO.SocketRxBuffStartId;
                const SoAd_SoRxBufferManType* soRxBufMgmtPtr = &SoAd_SoRxBuffManagerPtr[partIndex][startId];
#else
                const SoAd_SoRxBufferManType* soRxBufMgmtPtr = &SoAd_SoRxBuffManagerPtr[bufferId];
#endif
                if ((RxPduId == soRxBufMgmtPtr->SocketRouteId) && (soRxBufMgmtPtr->ExitPduResidueLength > 0u))
                {
                    result = E_OK;
                }
            }
#endif
            if (E_OK == result)
            {
                soConMgmtPtr->SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
                soConMgmtPtr->SetSocketNeedCloseLine = __LINE__;
#endif
            }
        }
#endif
    }
    SOAD_NOUSED(RxPduId);
    return result;
}

/**
 * Requests transmission of an I-PDU.
 */
/* PRQA S 6030 ++*/ /* VL_MTR_SoAd_STMIF */
Std_ReturnType SoAd_TpTransmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr)
/* PRQA S 6030 --*/
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /**
     * SWS_SoAd_00224,SWS_SoAd_00237
     */
    ApplicationType curAppId;
    if (SoAd_DetCheckInitStatus(SOAD_SID_TPTRANSMIT, &curAppId)
        && SoAd_DetCheckTxPduId(SOAD_SID_TPTRANSMIT, TxPduId, TRUE, SOAD_UPPER_LAYER_TP)
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
        && SoAd_DetCheckTxPduIdPartition(SOAD_SID_TPTRANSMIT, TxPduId, curAppId)
#endif
    )
#endif
    {
#if (STD_ON == SOAD_SUPPORT_TX_BUFFER)
#if (SOAD_PARTITION_NUM > 1u)
        uint16    partIndex    = SoAd_GetPartitionIndex(SoAd_PduRouteCfgPtr[TxPduId].PartitionId);
        PduIdType pduStartId   = SoAd_PduRouteCfgPtr[TxPduId].PartitionPduRouteId;
        uint8     txPendingNum = SoAd_PduRouteManagerPtr[partIndex][pduStartId].TxPendingNum;
#else
        uint8 txPendingNum = SoAd_PduRouteManagerPtr[TxPduId].TxPendingNum;
#endif

        if ((SOAD_UNUSED_UINT8 == txPendingNum) && (0u < PduInfoPtr->SduLength))
        {
            SoAd_uintx                         pduRouteDestId = SOAD_PDUROT_DESTINDEX_CFG(TxPduId);
            SoAd_InnerPduRouteDestManagerType* pduRouteDestMPtr;
#if (SOAD_PARTITION_NUM > 1u)
            PduIdType destId = SoAd_PduRouteDestCfgPtr[pduRouteDestId].PartitionPduRouteDestId;
            pduRouteDestMPtr = &SoAd_PduRouteDestManagerPtr[partIndex][destId];
#else
            pduRouteDestMPtr = &SoAd_PduRouteDestManagerPtr[pduRouteDestId];
#endif
            if (pduRouteDestMPtr->PduRouteDestEnable)
            {
                SoAd_SoConIdType soConId = SoAd_PduRouteDestCfgPtr[pduRouteDestId].TxSocketConRefIndexPtr[0];
                SoAd_InnerSocketConManageType* soConMgmtPtr;
#if (SOAD_PARTITION_NUM > 1u)
                soConMgmtPtr = &SoAd_SoConManagerPtr[partIndex][SoAd_SoConCfgPtr[soConId].PartitionSoConId];
#else
                soConMgmtPtr = &SoAd_SoConManagerPtr[soConId];
#endif
                if ((SOAD_SOCON_ONLINE == soConMgmtPtr->SoConMode)
                    && (SOAD_SOCKET_CLOSE_NONE == soConMgmtPtr->SocketNeedClose) && (!soConMgmtPtr->TpPduTransmitting))
                {
                    result = SoAd_TpTransmitSoCon(soConId, soConMgmtPtr, pduRouteDestId, PduInfoPtr, pduRouteDestMPtr);
                    if (E_OK == result)
                    {
                        soConMgmtPtr->ActiveSoAdPduRouteId = TxPduId;
                    }
                }
            }
        }
#endif
    }
    SOAD_NOUSED(TxPduId);
    SOAD_NOUSED(PduInfoPtr);
    return result;
}

/**
 * This service opens the socket connection specified by SoConId.
 */
/* PRQA S 6030 ++*/ /* VL_MTR_SoAd_STMIF */
Std_ReturnType SoAd_OpenSoCon(SoAd_SoConIdType SoConId)
/* PRQA S 6030 --*/
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /**
     * SWS_SoAd_00615,SWS_SoAd_00611,SWS_SoAd_00528
     */
    ApplicationType curAppId;
    if (SoAd_DetCheckInitStatus(SOAD_SID_OPENSOCON, &curAppId) && SoAd_DetCheckSoConId(SOAD_SID_OPENSOCON, SoConId)
        && SoAd_DetCheckAutoSoConSetup(SOAD_SID_OPENSOCON, SoConId)
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
        && SoAd_DetCheckSoConIdPartition(SOAD_SID_OPENSOCON, SOAD_SOCON_REFGORUP(SoConId), curAppId)
#endif
    )
#endif
    {
        SoAd_InnerSocketConManageType* soConMgmtPtr;
#if (SOAD_PARTITION_NUM > 1u)
        uint16 partIndex = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(SoConId)].PartitionId);
        soConMgmtPtr     = &SoAd_SoConManagerPtr[partIndex][SoAd_SoConCfgPtr[SoConId].PartitionSoConId];
#else
                soConMgmtPtr = &SoAd_SoConManagerPtr[SoConId];
#endif
        if (SOAD_SOCON_REQUEST_NONE == soConMgmtPtr->SoConRequest[0u])
        {
            soConMgmtPtr->SoConRequest[0u] = SOAD_SOCON_REQUEST_OPEN;
        }
        else
        {
            if (SOAD_SOCON_REQUEST_NONE == soConMgmtPtr->SoConRequest[1u])
            {
                if (SOAD_SOCON_REQUEST_CLOSE == soConMgmtPtr->SoConRequest[0u])
                {
                    soConMgmtPtr->SoConRequest[1u] = SOAD_SOCON_REQUEST_OPEN;
                }
            }
            else if (SOAD_SOCON_REQUEST_CLOSE == soConMgmtPtr->SoConRequest[1u])
            {
                soConMgmtPtr->SoConRequest[1u] = SOAD_SOCON_REQUEST_NONE;
            }
            else
            {
                /* do nothing */
            }
        }
        result = E_OK;
    }
    return result;
}

/**
 * This service closes the socket connection specified by SoConId.
 */
/* PRQA S 6030,4603,3334 ++ */ /* VL_MTR_SoAd_STMIF,VL_SoAd_4603AbortSameName,VL_SoAd_0334 */
Std_ReturnType SoAd_CloseSoCon(SoAd_SoConIdType SoConId, boolean abort)
/* PRQA S 6030,4603,3334 -- */
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /**
     * SWS_SoAd_00616,SWS_SoAd_00612,SWS_SoAd_00529
     */
    ApplicationType curAppId;
    if (SoAd_DetCheckInitStatus(SOAD_SID_CLOSESOCON, &curAppId) && SoAd_DetCheckSoConId(SOAD_SID_CLOSESOCON, SoConId)
        && SoAd_DetCheckAutoSoConSetup(SOAD_SID_CLOSESOCON, SoConId)
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
        && SoAd_DetCheckSoConIdPartition(SOAD_SID_CLOSESOCON, SOAD_SOCON_REFGORUP(SoConId), curAppId)
#endif
    )
#endif
    {
        SoAd_InnerSocketConManageType* soConMgmtPtr;
#if (SOAD_PARTITION_NUM > 1u)
        uint16 partIndex = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(SoConId)].PartitionId);
        soConMgmtPtr     = &SoAd_SoConManagerPtr[partIndex][SoAd_SoConCfgPtr[SoConId].PartitionSoConId];
#else
                soConMgmtPtr = &SoAd_SoConManagerPtr[SoConId];
#endif
        soConMgmtPtr->SoConCloseAbort = abort;
        if (!abort)
        {
            if (SOAD_SOCON_REQUEST_NONE == soConMgmtPtr->SoConRequest[0u])
            {
                soConMgmtPtr->SoConRequest[0u] = SOAD_SOCON_REQUEST_CLOSE;
            }
            else
            {
                if (SOAD_SOCON_REQUEST_NONE == soConMgmtPtr->SoConRequest[1u])
                {
                    if (SOAD_SOCON_REQUEST_OPEN == soConMgmtPtr->SoConRequest[0u])
                    {
                        soConMgmtPtr->SoConRequest[1u] = SOAD_SOCON_REQUEST_CLOSE;
                    }
                }
                else if (SOAD_SOCON_REQUEST_OPEN == soConMgmtPtr->SoConRequest[1u])
                {
                    soConMgmtPtr->SoConRequest[1u] = SOAD_SOCON_REQUEST_NONE;
                }
                else
                {
                    /* do nothing */
                }
            }
        }
        else
        {
            soConMgmtPtr->SoConRequest[0u] = SOAD_SOCON_REQUEST_CLOSE;
            soConMgmtPtr->SoConRequest[1u] = SOAD_SOCON_REQUEST_NONE;
        }
        result = E_OK;
    }
    return result;
}

/**
 * Returns current state of the socket connection specified by SoConId.
 */
/* PRQA S 1532 ++*/ /* VL_QAC_OneFunRef */
void SoAd_GetSoConMode(SoAd_SoConIdType SoConId, SoAd_SoConModeType* ModePtr)
/* PRQA S 1532 --*/
{
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    ApplicationType curAppId;
    if (SoAd_DetCheckInitStatus(SOAD_SID_GETSOCONMODE, &curAppId)
        && SoAd_DetCheckSoConId(SOAD_SID_GETSOCONMODE, SoConId)
        && SoAd_DetCheckPtrParameter(SOAD_SID_GETSOCONMODE, ModePtr)
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
        && SoAd_DetCheckSoConIdPartition(SOAD_SID_GETSOCONMODE, SOAD_SOCON_REFGORUP(SoConId), curAppId)
#endif
    )
#endif
    {
#if (SOAD_PARTITION_NUM > 1u)
        uint16 partIndex    = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(SoConId)].PartitionId);
        SoAd_SoConIdType id = SoAd_SoConCfgPtr[SoConId].PartitionSoConId;
        *ModePtr            = SoAd_SoConManagerPtr[partIndex][id].SoConMode;
#else
                *ModePtr = SoAd_SoConManagerPtr[SoConId].SoConMode;
#endif
    }
}

/**
 * By this API service the local IP address assignment which shall be used for the socket connection specified by
 * SoConId is initiated.
 */
Std_ReturnType SoAd_RequestIpAddrAssignment(
    SoAd_SoConIdType           SoConId,
    TcpIp_IpAddrAssignmentType Type,
    const TcpIp_SockAddrType*  LocalIpAddrPtr,
    uint8                      Netmask,
    const TcpIp_SockAddrType*  DefaultRouterPtr)
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /**
     * SWS_SoAd_00613,SWS_SoAd_00617
     */
    ApplicationType curAppId;
    if (SoAd_DetCheckInitStatus(SOAD_SID_REQUESTIPADDRASSIGNMENT, &curAppId)
        && SoAd_DetCheckSoConId(SOAD_SID_REQUESTIPADDRASSIGNMENT, SoConId)
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
        && SoAd_DetCheckSoConIdPartition(SOAD_SID_REQUESTIPADDRASSIGNMENT, SOAD_SOCON_REFGORUP(SoConId), curAppId)
#endif
    )
#endif
    {
        SoAd_uintx            soConGroup  = SOAD_SOCON_REFGORUP(SoConId);
        TcpIp_LocalAddrIdType localAddrId = SOAD_SOCKETLOCALADDR_CFG(soConGroup).LocalAddrId;
        result = TcpIp_RequestIpAddrAssignment(localAddrId, Type, LocalIpAddrPtr, Netmask, DefaultRouterPtr);
    }
    return result;
}

/**
 * By this API service the local IP address assignment used for the socket connection specified by SoConId is released.
 */
Std_ReturnType SoAd_ReleaseIpAddrAssignment(SoAd_SoConIdType SoConId)
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /**
     * SWS_SoAd_00618,SWS_SoAd_00619
     */
    ApplicationType curAppId;
    if (SoAd_DetCheckInitStatus(SOAD_SID_RELEASEIPADDRASSIGNMENT, &curAppId)
        && SoAd_DetCheckSoConId(SOAD_SID_RELEASEIPADDRASSIGNMENT, SoConId)
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
        && SoAd_DetCheckSoConIdPartition(SOAD_SID_RELEASEIPADDRASSIGNMENT, SOAD_SOCON_REFGORUP(SoConId), curAppId)
#endif
    )
#endif
    {
        SoAd_uintx            soConGroup  = SOAD_SOCON_REFGORUP(SoConId);
        TcpIp_LocalAddrIdType localAddrId = SOAD_SOCKETLOCALADDR_CFG(soConGroup).LocalAddrId;
        result                            = TcpIp_ReleaseIpAddrAssignment(localAddrId);
    }
    return result;
}

/**
 * Retrieves the local address (IP address and port) actually used for the SoAd socket connection specified by SoConId,
 * the netmask and default router
 */
/* PRQA S 1532 ++*/ /* VL_QAC_OneFunRef */
Std_ReturnType SoAd_GetLocalAddr(
    SoAd_SoConIdType    SoConId,
    TcpIp_SockAddrType* LocalAddrPtr,
    uint8*              NetmaskPtr,
    TcpIp_SockAddrType* DefaultRouterPtr)
/* PRQA S 1532 --*/
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /**
     * SWS_SoAd_00621,SWS_SoAd_00620
     */
    ApplicationType curAppId;
    if (SoAd_DetCheckInitStatus(SOAD_SID_GETLOCALADDR, &curAppId)
        && SoAd_DetCheckSoConId(SOAD_SID_GETLOCALADDR, SoConId)
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
        && SoAd_DetCheckSoConIdPartition(SOAD_SID_GETLOCALADDR, SOAD_SOCON_REFGORUP(SoConId), curAppId)
#endif
    )
#endif
    {
        SoAd_uintx            soConGroup  = SOAD_SOCON_REFGORUP(SoConId);
        TcpIp_LocalAddrIdType localAddrId = SOAD_SOCKETLOCALADDR_CFG(soConGroup).LocalAddrId;
        LocalAddrPtr->port                = SoAd_SoConGroupCfgPtr[soConGroup].SocketLocalPort;
        result                            = TcpIp_GetIpAddr(localAddrId, LocalAddrPtr, NetmaskPtr, DefaultRouterPtr);
    }
    return result;
}

/**
 * Retrieves the physical source address of the EthIf controller used by the SoAd socket connection specified by
 * SoConId.
 */
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType SoAd_GetPhysAddr(SoAd_SoConIdType SoConId, uint8* PhysAddrPtr)
/* PRQA S 1503 -- */
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /**
     * SWS_SoAd_00623,SWS_SoAd_00622
     */
    ApplicationType curAppId;
    if (SoAd_DetCheckInitStatus(SOAD_SID_GETPHYSADDR, &curAppId) && SoAd_DetCheckSoConId(SOAD_SID_GETPHYSADDR, SoConId)
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
        && SoAd_DetCheckSoConIdPartition(SOAD_SID_GETPHYSADDR, SOAD_SOCON_REFGORUP(SoConId), curAppId)
#endif
    )
#endif
    {
        SoAd_uintx            soConGroup  = SOAD_SOCON_REFGORUP(SoConId);
        TcpIp_LocalAddrIdType localAddrId = SOAD_SOCKETLOCALADDR_CFG(soConGroup).LocalAddrId;
        result                            = TcpIp_GetPhysAddr(localAddrId, PhysAddrPtr);
    }
    return result;
}

/**
 * Retrieves the remote address (IP address and port) actually used for the SoAdsocket connection specified by SoConId
 */
Std_ReturnType SoAd_GetRemoteAddr(SoAd_SoConIdType SoConId, TcpIp_SockAddrType* IpAddrPtr)
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /**
     * SWS_SoAd_00659,SWS_SoAd_00660
     */
    ApplicationType curAppId;
    if (SoAd_DetCheckInitStatus(SOAD_SID_GETREMOTEADDR, &curAppId)
        && SoAd_DetCheckSoConId(SOAD_SID_GETREMOTEADDR, SoConId)
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
        && SoAd_DetCheckSoConIdPartition(SOAD_SID_GETREMOTEADDR, SOAD_SOCON_REFGORUP(SoConId), curAppId)
#endif
    )
#endif
    {
        const SoAd_InnerSocketConManageType* soConMgmtPtr;
#if (SOAD_PARTITION_NUM > 1u)
        uint16 partIndex = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(SoConId)].PartitionId);
        soConMgmtPtr     = &SoAd_SoConManagerPtr[partIndex][SoAd_SoConCfgPtr[SoConId].PartitionSoConId];
#else
                soConMgmtPtr = &SoAd_SoConManagerPtr[SoConId];
#endif
        if (IpAddrPtr->domain == soConMgmtPtr->RemoteAddr.domain)
        {
            if (soConMgmtPtr->RemoteAddrIsSet)
            {
                /*SWS_SoAd_00664*/
                *IpAddrPtr = soConMgmtPtr->RemoteAddr;
                result     = E_OK;
            }
            /*SWS_SoAd_00666*/
            else
            {
            }
        }
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
        /*SWS_SoAd_00698*/
        else
        {
            SOAD_DET_REPORT(SOAD_SID_GETREMOTEADDR, SOAD_E_INV_ARG);
        }
#endif
    }
    return result;
}

/**
 * Enables routing of a group of PDUs in the SoAd related to the RoutingGroup specified by parameter id.
 */
/* PRQA S 1532 ++*/ /* VL_QAC_OneFunRef */
Std_ReturnType SoAd_EnableRouting(SoAd_RoutingGroupIdType id)
/* PRQA S 1532 --*/
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /**
     * SWS_SoAd_00624,SWS_SoAd_00625
     */
    ApplicationType curAppId;
    if (SoAd_DetCheckInitStatus(SOAD_SID_ENABLEROUTING, &curAppId)
        && SoAd_DetCheckRoutingGroupId(SOAD_SID_ENABLEROUTING, id)
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
        && SoAd_DetCheckRoutingGroupIdPartition(SOAD_SID_ENABLEROUTING, id, curAppId)
#endif
    )
#endif
    {
#if (0u < SOAD_ROUTING_GROUP_MAX)
        result = SoAd_RoutingGroupControl(id, TRUE);
#endif
    }
    SOAD_NOUSED(id);
    return result;
}

/**
 * Enables routing of a group of PDUs in the SoAd related to the RoutingGroupspecified by parameter id only on the
 * socket connection identified by SoConId.
 */
/* PRQA S 1532 ++*/ /* VL_QAC_OneFunRef */
Std_ReturnType SoAd_EnableSpecificRouting(SoAd_RoutingGroupIdType id, SoAd_SoConIdType SoConId)
/* PRQA S 1532 --*/
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /**
     * SWS_SoAd_00715,SWS_SoAd_00716
     */
    ApplicationType curAppId;
    if (SoAd_DetCheckInitStatus(SOAD_SID_ENABLESPECIFICROUTING, &curAppId)
        && SoAd_DetCheckSoConId(SOAD_SID_ENABLESPECIFICROUTING, SoConId)
        && SoAd_DetCheckRoutingGroupId(SOAD_SID_ENABLESPECIFICROUTING, id)
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
        && SoAd_DetCheckRoutingGroupIdPartition(SOAD_SID_ENABLESPECIFICROUTING, id, curAppId)
#endif
    )
#endif
    {
#if (0u < SOAD_ROUTING_GROUP_MAX)
        SoAd_SoConIdType localSoConId = SoConId;
        result                        = SoAd_RoutingGroupControlSpcificSoCon(id, TRUE, localSoConId);
#endif
    }
    SOAD_NOUSED(id);
    SOAD_NOUSED(SoConId);
    return result;
}

/**
 * Disables routing of a group of PDUs in the SoAd related to the RoutingGroup specified by parameter id.
 */
/* PRQA S 1532 ++*/ /* VL_QAC_OneFunRef */
Std_ReturnType SoAd_DisableRouting(SoAd_RoutingGroupIdType id)
/* PRQA S 1532 --*/
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /**
     * SWS_SoAd_00627,SWS_SoAd_00626
     */
    ApplicationType curAppId;
    if (SoAd_DetCheckInitStatus(SOAD_SID_DISABLEROUTING, &curAppId)
        && SoAd_DetCheckRoutingGroupId(SOAD_SID_DISABLEROUTING, id)
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
        && SoAd_DetCheckRoutingGroupIdPartition(SOAD_SID_DISABLEROUTING, id, curAppId)
#endif
    )
#endif
    {
#if (0u < SOAD_ROUTING_GROUP_MAX)
        result = SoAd_RoutingGroupControl(id, FALSE);
#endif
    }
    SOAD_NOUSED(id);
    return result;
}

/**
 * Disables routing of a group of PDUs in the SoAd related to the RoutingGroup specified by parameter id only on the
 * socket connection identified by SoConId.
 */
/* PRQA S 1532 ++*/ /* VL_QAC_OneFunRef */
Std_ReturnType SoAd_DisableSpecificRouting(SoAd_RoutingGroupIdType id, SoAd_SoConIdType SoConId)
/* PRQA S 1532 --*/
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /**
     * SWS_SoAd_00718,SWS_SoAd_00719
     */
    ApplicationType curAppId;
    if (SoAd_DetCheckInitStatus(SOAD_SID_DISABLESPECIFICROUTING, &curAppId)
        && SoAd_DetCheckSoConId(SOAD_SID_DISABLESPECIFICROUTING, SoConId)
        && SoAd_DetCheckRoutingGroupId(SOAD_SID_DISABLESPECIFICROUTING, id)
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
        && SoAd_DetCheckRoutingGroupIdPartition(SOAD_SID_DISABLESPECIFICROUTING, id, curAppId)
#endif
    )
#endif
    {
#if (0u < SOAD_ROUTING_GROUP_MAX)
        SoAd_SoConIdType localSoConId = SoConId;
        result                        = SoAd_RoutingGroupControlSpcificSoCon(id, FALSE, localSoConId);
#endif
    }
    SOAD_NOUSED(id);
    SOAD_NOUSED(SoConId);
    return result;
}

/**
 * By this API service the remote address (IP address and port) of the specified socket connection shall be set.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_SoAd_STCAL */
Std_ReturnType SoAd_SetRemoteAddr(SoAd_SoConIdType SoConId, const TcpIp_SockAddrType* RemoteAddrPtr)
/* PRQA S 6070 -- */
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /**
     * SWS_SoAd_00628,SWS_SoAd_00531,SWS_SoAd_00699
     */
    ApplicationType curAppId;
    if (SoAd_DetCheckInitStatus(SOAD_SID_SETREMOTEADDR, &curAppId)
        && SoAd_DetCheckSoConId(SOAD_SID_SETREMOTEADDR, SoConId)
        && SoAd_DetCheckPtrParameter(SOAD_SID_SETREMOTEADDR, RemoteAddrPtr)
        && SoAd_DetCheckAutoSoConSetup(SOAD_SID_SETREMOTEADDR, SoConId)
        && SoAd_DetCheckTcpIpDomainType(SOAD_SID_SETREMOTEADDR, RemoteAddrPtr->domain, SoConId)
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
        && SoAd_DetCheckSoConIdPartition(SOAD_SID_SETREMOTEADDR, SOAD_SOCON_REFGORUP(SoConId), curAppId)
#endif
    )
#endif
    {
        SoAd_InnerSocketConManageType* soConMgmtPtr;
#if (SOAD_PARTITION_NUM > 1u)
        uint16 partIndex = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(SoConId)].PartitionId);
        soConMgmtPtr     = &SoAd_SoConManagerPtr[partIndex][SoAd_SoConCfgPtr[SoConId].PartitionSoConId];
#else
                soConMgmtPtr = &SoAd_SoConManagerPtr[SoConId];
#endif
        boolean remoteAddrLocked = SoAd_SoConRemoteAddrLocked(SoConId, soConMgmtPtr);
        if (!remoteAddrLocked)
        {
            /*SWS_SoAd_00533*/
            soConMgmtPtr->RemoteAddrIsSet                    = TRUE;
            soConMgmtPtr->RemoteAddr                         = *RemoteAddrPtr;
            SoAd_uintx                          soConGroupId = SOAD_SOCON_REFGORUP(SoConId);
            SoAd_InnerSocketConGroupManageType* soConGroupMgmtPtr;
#if (SOAD_PARTITION_NUM > 1u)
            soConGroupMgmtPtr =
                &SoAd_SoConGroupManagerPtr[partIndex][SoAd_SoConGroupCfgPtr[soConGroupId].PartitionSoConGroupId];
#else
                    soConGroupMgmtPtr = &SoAd_SoConGroupManagerPtr[soConGroupId];
#endif
            /*SWS_SoAd_00687*/
            if ((SOAD_SOCON_ONLINE == soConMgmtPtr->SoConMode)
                && (SOAD_IS_IPADDR_ANY(*RemoteAddrPtr) || (TCPIP_PORT_ANY == RemoteAddrPtr->port)))
            {
                soConMgmtPtr->SoConMode = SOAD_SOCON_RECONNECT;
                SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, soConMgmtPtr, soConGroupId);
            }
            else
            {
                SoAd_DisconnectionAndRecoveryHandle(SoConId, soConGroupId, soConMgmtPtr, soConGroupMgmtPtr);
            }
            result = E_OK;
        }
    }
    return result;
}

/**
 * This API service shall either return the socket connection index of the SoAdSocketConnectionGroup where the
 * specified remote address (IP address and port) is set or assign the remote address to an unused socket connection
 * from the same SoAdSocketConnectionGroup.
 */
/* PRQA S 1532,6070 ++*/ /* VL_QAC_OneFunRef,VL_MTR_SoAd_STCAL */
Std_ReturnType SoAd_SetUniqueRemoteAddr(
    SoAd_SoConIdType          SoConId,
    const TcpIp_SockAddrType* RemoteAddrPtr,
    SoAd_SoConIdType*         AssignedSoConIdPtr)
/* PRQA S 1532,6070 --*/
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /**
     * SWS_SoAd_00672, SWS_SoAd_00673,similar to SWS_SoAd_00531,SWS_SoAd_00700
     */
    ApplicationType curAppId;
    if (SoAd_DetCheckInitStatus(SOAD_SID_SETUNIQUEREMOTEADDR, &curAppId)
        && SoAd_DetCheckSoConId(SOAD_SID_SETUNIQUEREMOTEADDR, SoConId)
        && SoAd_DetCheckPtrParameter(SOAD_SID_SETUNIQUEREMOTEADDR, RemoteAddrPtr)
        && SoAd_DetCheckAutoSoConSetup(SOAD_SID_SETUNIQUEREMOTEADDR, SoConId)
        && SoAd_DetCheckTcpIpDomainType(SOAD_SID_SETUNIQUEREMOTEADDR, RemoteAddrPtr->domain, SoConId)
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
        && SoAd_DetCheckSoConIdPartition(SOAD_SID_SETUNIQUEREMOTEADDR, SOAD_SOCON_REFGORUP(SoConId), curAppId)
#endif
    )
#endif
    {
        SoAd_SoConIdType tempSoConId = 0u;
        SoAd_uintx       soConGroup  = SOAD_SOCON_REFGORUP(SoConId);
        /*SWS_SoAd_00675,SWS_SoAd_00676*/
        boolean ret =
            SoAd_GetBestMatchAlgorithmSoCon(soConGroup, RemoteAddrPtr, &tempSoConId, SOAD_SOCON_MATCH_REQUEST_NONE);
        if (ret)
        {
            SoAd_InnerSocketConManageType*      soConMgmtPtr;
            SoAd_InnerSocketConGroupManageType* soConGroupMgmtPtr;

#if (SOAD_PARTITION_NUM > 1u)
            uint16 partIndex = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[soConGroup].PartitionId);
            soConMgmtPtr     = &SoAd_SoConManagerPtr[partIndex][SoAd_SoConCfgPtr[tempSoConId].PartitionSoConId];
            soConGroupMgmtPtr =
                &SoAd_SoConGroupManagerPtr[partIndex][SoAd_SoConGroupCfgPtr[soConGroup].PartitionSoConGroupId];
#else
                    soConMgmtPtr      = &SoAd_SoConManagerPtr[tempSoConId];
                    soConGroupMgmtPtr = &SoAd_SoConGroupManagerPtr[soConGroup];
#endif

            soConMgmtPtr->RemoteAddrIsSet = TRUE;
            soConMgmtPtr->RemoteAddr      = *RemoteAddrPtr;
            result                        = E_OK;
            *AssignedSoConIdPtr           = tempSoConId;
            SoAd_DisconnectionAndRecoveryHandle(tempSoConId, soConGroup, soConMgmtPtr, soConGroupMgmtPtr);
        }
        /*SWS_SoAd_00678*/
        else
        {
            result = E_NOT_OK;
        }
    }
    return result;
}

/**
 * By this API service the remote address (IP address and port) of the specified socket connection shall be released,
 * i.e. set back to the configured remote address setting.
 */
void SoAd_ReleaseRemoteAddr(SoAd_SoConIdType SoConId)
{
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /*similar to SWS_SoAd_00744*/
    ApplicationType curAppId;
    if (SoAd_DetCheckInitStatus(SOAD_SID_RELEASEREMOTEADDR, &curAppId)
        && SoAd_DetCheckSoConId(SOAD_SID_RELEASEREMOTEADDR, SoConId)
        && SoAd_DetCheckAutoSoConSetup(SOAD_SID_RELEASEREMOTEADDR, SoConId)
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
        && SoAd_DetCheckSoConIdPartition(SOAD_SID_RELEASEREMOTEADDR, SOAD_SOCON_REFGORUP(SoConId), curAppId)
#endif
    )
#endif
    {
        SoAd_InnerSocketConManageType* soConMgmtPtr;
#if (SOAD_PARTITION_NUM > 1u)
        uint16 partIndex    = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(SoConId)].PartitionId);
        SoAd_SoConIdType id = SoAd_SoConCfgPtr[SoConId].PartitionSoConId;
        soConMgmtPtr        = &SoAd_SoConManagerPtr[partIndex][id];
#else
                soConMgmtPtr = &SoAd_SoConManagerPtr[SoConId];
#endif
        boolean remoteAddrLocked = SoAd_SoConRemoteAddrLocked(SoConId, soConMgmtPtr);
        if (!remoteAddrLocked)
        {
            SoAd_uintx soConGroup = SOAD_SOCON_REFGORUP(SoConId);
            SoAd_ReleaseRemoteAddrHandle(SoConId, soConMgmtPtr, soConGroup);
        }
        else
        {
            soConMgmtPtr->RemoteAddrNeedRelease = TRUE;
        }
    }
}

/**
 * By this API service the SoAd or TCP/IP stack is requested to change a connection parameter. E.g. the Nagle algorithm
 * may be controlled by this API.
 */
/* PRQA S 1503 ++*/ /* VL_QAC_NoUsedApi */
Std_ReturnType SoAd_TpChangeParameter(PduIdType id, TPParameterType parameter, uint16 value)
/* PRQA S 1503 --*/
{
    /*follow AUTOSAR_R19_11:SWS_SoAd_00730
      not follow AUTOSAR4.2.2:SWS_SoAd_00629,SWS_SoAd_00630,SWS_SoAd_00631
    */
    SOAD_NOUSED(id);
    SOAD_NOUSED(parameter);
    SOAD_NOUSED(value);
    return E_NOT_OK;
}

/**
 * By this API service an upper layer of the SoAd can read the currently configured hostname, i.e. FQDN option in the
 * DHCP submodule of the TCP/IP stack.
 */
/* PRQA S 1503 ++*/ /* VL_QAC_NoUsedApi */
Std_ReturnType SoAd_ReadDhcpHostNameOption(SoAd_SoConIdType SoConId, uint8* length, uint8* data)
/* PRQA S 1503 --*/
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /**
     * SWS_SoAd_00701, SWS_SoAd_00702
     */
    ApplicationType curAppId;
    if (SoAd_DetCheckInitStatus(SOAD_SID_READDHCPHOSTNAMEOPTION, &curAppId)
        && SoAd_DetCheckSoConId(SOAD_SID_READDHCPHOSTNAMEOPTION, SoConId)
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
        && SoAd_DetCheckSoConIdPartition(SOAD_SID_READDHCPHOSTNAMEOPTION, SOAD_SOCON_REFGORUP(SoConId), curAppId)
#endif
    )
#endif
    {
        SoAd_uintx            soConGroup  = SOAD_SOCON_REFGORUP(SoConId);
        TcpIp_LocalAddrIdType localAddrId = SOAD_SOCKETLOCALADDR_CFG(soConGroup).LocalAddrId;
        if (TCPIP_AF_INET == SOAD_SOCKETLOCALADDR_CFG(soConGroup).DomainType)
        {
            /*SWS_SoAd_00703*/
            /* Note: According to RFC4702 . Client FQDN option has Code 81 */
            result = TcpIp_DhcpReadOption(localAddrId, SOAD_DCHP4_CLIENT_FQDN, length, data);
        }
#if (STD_ON == SOAD_IPV6_ADDRESS_ENABLED)
        else
        {
            uint16 dataLength;
            dataLength = (uint16)(*length);
            /*SWS_SoAd_00704*/
            /* Note: According to RFC4704 . Client FQDN option has Code 39 */
            result = TcpIp_DhcpV6ReadOption(localAddrId, SOAD_DCHP6_CLIENT_FQDN, &dataLength, data);
        }
#endif
    }
    return result;
}

/**
 * By this API service an upper layer of the SoAd can set the hostname, i.e. FQDN option in the DHCP submodule of the
 * TCP/IP stack.
 */
/* PRQA S 1503 ++*/ /* VL_QAC_NoUsedApi */
Std_ReturnType SoAd_WriteDhcpHostNameOption(SoAd_SoConIdType SoConId, uint8 length, const uint8* data)
/* PRQA S 1503 --*/
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /**
     * SWS_SoAd_00705, SWS_SoAd_00706
     */
    ApplicationType curAppId;
    if (SoAd_DetCheckInitStatus(SOAD_SID_WRITEDHCPHOSTNAMEOPTION, &curAppId)
        && SoAd_DetCheckSoConId(SOAD_SID_WRITEDHCPHOSTNAMEOPTION, SoConId)
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
        && SoAd_DetCheckSoConIdPartition(SOAD_SID_WRITEDHCPHOSTNAMEOPTION, SOAD_SOCON_REFGORUP(SoConId), curAppId)
#endif
    )
#endif
    {
        SoAd_uintx            soConGroup  = SOAD_SOCON_REFGORUP(SoConId);
        TcpIp_LocalAddrIdType localAddrId = SOAD_SOCKETLOCALADDR_CFG(soConGroup).LocalAddrId;
        if (TCPIP_AF_INET == SOAD_SOCKETLOCALADDR_CFG(soConGroup).DomainType)
        {
            /*SWS_SoAd_00707*/
            result = TcpIp_DhcpWriteOption(localAddrId, SOAD_DCHP4_CLIENT_FQDN, length, data);
        }
#if (STD_ON == SOAD_IPV6_ADDRESS_ENABLED)
        else
        {
            /*SWS_SoAd_00708*/
            result = TcpIp_DhcpV6WriteOption(localAddrId, SOAD_DCHP6_CLIENT_FQDN, (uint16)length, data);
        }
#endif
    }
    return result;
}
/**
 * API check if a communication over this socket connection is possible for a dedicated remote address.
 */
/* PRQA S 1532 ++*/ /* VL_QAC_OneFunRef */
TcpIp_ReturnType SoAd_IsConnectionReady(SoAd_SoConIdType SoConId, const TcpIp_SockAddrType* RemoteAddrPtr)
/* PRQA S 1532 --*/
{
    TcpIp_ReturnType result = TCPIP_E_NOT_OK;

#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /**
     * SWS_SoAd_00705,SWS_SoAd_00706
     */
    ApplicationType curAppId;
    if (SoAd_DetCheckInitStatus(SOAD_SID_ISCONNECTIONREADY, &curAppId)
        && SoAd_DetCheckSoConId(SOAD_SID_ISCONNECTIONREADY, SoConId)
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
        && SoAd_DetCheckSoConIdPartition(SOAD_SID_ISCONNECTIONREADY, SOAD_SOCON_REFGORUP(SoConId), curAppId)
#endif
    )
#endif
    {
#if (SOAD_PARTITION_NUM > 1u)
        uint16 partIndex = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(SoConId)].PartitionId);
#endif
        const SoAd_InnerSocketConManageType* soConMgmtPtr;
#if (SOAD_PARTITION_NUM > 1u)
        soConMgmtPtr = &SoAd_SoConManagerPtr[partIndex][SoAd_SoConCfgPtr[SoConId].PartitionSoConId];
#else
                soConMgmtPtr = &SoAd_SoConManagerPtr[SoConId];
#endif
        if (soConMgmtPtr->TcpIpSocketValid)
        {
            result = TcpIp_IsConnectionReady(soConMgmtPtr->TcpIpSocketId, RemoteAddrPtr);
        }
    }

    return result;
}

#define SOAD_STOP_SEC_CODE
#include "SoAd_MemMap.h"

/**
 * The TCP/IP stack calls this primitive after the reception of data on a socket. Thesocket identifier along with
 * configuration information determines which module is to be called.
 */
#define SOAD_START_SEC_CODE_FAST
#include "SoAd_MemMap.h"
/* PRQA S 1532,6070,6030 ++*/ /* VL_QAC_OneFunRef,VL_MTR_SoAd_STCAL,VL_MTR_SoAd_STMIF */
void SoAd_RxIndication(
    TcpIp_SocketIdType        SocketId,
    const TcpIp_SockAddrType* RemoteAddrPtr,
    const uint8*              BufPtr,
    uint16                    Length)
/* PRQA S 1532,6070,6030 --*/
{
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /*SWS_SoAd_00264*/
    ApplicationType curAppId;
    if (SoAd_DetCheckInitStatus(SOAD_SID_RXINDICATION, &curAppId)
        && SoAd_DetCheckPtrParameter(SOAD_SID_RXINDICATION, RemoteAddrPtr)
        && SoAd_DetCheckLength(SOAD_SID_RXINDICATION, Length)
        && SoAd_DetCheckPtrParameter(SOAD_SID_RXINDICATION, BufPtr))
#endif
    {
#if (0u < SOAD_SOCKET_ROUTE_DEST_MAX)
#if (STD_ON == SOAD_SUPPORT_UDP)
        /*used to revert the remote address change*/
        SoAd_FindTheValidSoConType udpParameter;
        udpParameter.RemoteAddrChanged = FALSE;
#endif
        SoAd_SoConIdType soConId = SOAD_NO_TXMETADATA;
        /*find the valid Socket Connection*/
        boolean socketIdValid = SoAd_FindTheValidSoCon(
            SocketId,
            RemoteAddrPtr,
#if (STD_ON == SOAD_SUPPORT_UDP)
            &udpParameter,
#endif
            &soConId);

        /*SWS_SoAd_00267*/
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
        if (SoAd_DetCheckSocketIdValid(SOAD_SID_RXINDICATION, socketIdValid)
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
            && (SoAd_DetCheckSoConIdPartition(SOAD_SID_RXINDICATION, SOAD_SOCON_REFGORUP(soConId), curAppId))
#endif
        )
#endif
        {
            /*handle the received TCP segment and UDP message */
            if (socketIdValid && (SOAD_NO_TXMETADATA != soConId))
            {
                SoAd_InnerSocketConManageType* soConMgmtPtr;
#if (SOAD_PARTITION_NUM > 1u)
                uint16 partIndex =
                    SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(soConId)].PartitionId);
                soConMgmtPtr = &SoAd_SoConManagerPtr[partIndex][SoAd_SoConCfgPtr[soConId].PartitionSoConId];
#else
                soConMgmtPtr = &SoAd_SoConManagerPtr[soConId];
#endif

                if (SOAD_SOCKET_CLOSE_NONE == soConMgmtPtr->SocketNeedClose)
                {
                    boolean    validPduData = TRUE;
                    SoAd_uintx soConGroupId = SOAD_SOCON_REFGORUP(soConId);
#if (STD_ON == SOAD_SUPPORT_HEADERID)
                    if (SoAd_SoConGroupCfgPtr[soConGroupId].PduHeaderEnable)
                    {
                        /* PRQA S 0311 ++ */ /* VL_SoAd_0311 */
                        validPduData =
                            SoAd_HandleRxIndicationEnableHeader(soConId, soConMgmtPtr, (uint8*)BufPtr, Length);
                        /* PRQA S 0311 -- */
                    }
                    /*Header is disabled,just one socket route ref the SoCon*/
                    else
#endif
                    {
                        /* PRQA S 0311 ++ */ /* VL_SoAd_0311 */
                        validPduData =
                            SoAd_HandleRxIndicationDisableHeader(soConId, soConMgmtPtr, (uint8*)BufPtr, Length);
                        /* PRQA S 0311 -- */
                    }
#if (STD_ON == SOAD_SUPPORT_UDP)
                    /*SWS_SoAd_00710*/
                    if (!validPduData && udpParameter.RemoteAddrChanged)
                    {
                        soConMgmtPtr->RemoteAddr              = udpParameter.RemoteAddr;
                        soConMgmtPtr->UdpAliveSupervisionTime = 0u;
                        soConMgmtPtr->SoConMode               = SOAD_SOCON_RECONNECT;
                        SoAd_SoConModeChgNotificationAndTpStartOfReception(soConId, soConMgmtPtr, soConGroupId);
                    }
#endif
                }
            }
        }
        SOAD_NOUSED(SocketId);
        SOAD_NOUSED(RemoteAddrPtr);
        SOAD_NOUSED(BufPtr);
        SOAD_NOUSED(Length);
#endif
    }
}
#define SOAD_STOP_SEC_CODE_FAST
#include "SoAd_MemMap.h"

/**
 * This service requests to copy data for transmission to the buffer indicated.This call is triggered by
 * TcpIp_Transmit(). Note: The call to <Up>_CopyTxData()may happen in the context of TcpIp_Transmit().
 */
#define SOAD_START_SEC_CODE_FAST
#include "SoAd_MemMap.h"
/* PRQA S 1532,6030 ++*/ /* VL_QAC_OneFunRef,VL_MTR_SoAd_STMIF */
BufReq_ReturnType SoAd_CopyTxData(TcpIp_SocketIdType SocketId, uint8* BufPtr, uint16 BufLength)
/* PRQA S 1532,6030 --*/
{
    BufReq_ReturnType ret = BUFREQ_E_NOT_OK;
    SoAd_SoConIdType  soConId;

#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    ApplicationType curAppId;
    if (SoAd_DetCheckInitStatus(SOAD_SID_COPYTXDATA, &curAppId))
#endif
    {
        boolean socketIdValid = SoAd_MatchTCPSocketId(SocketId, &soConId, TRUE);
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
        if (SoAd_DetCheckSocketIdValid(SOAD_SID_COPYTXDATA, socketIdValid)
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
            && (SoAd_DetCheckSoConIdPartition(SOAD_SID_COPYTXDATA, SOAD_SOCON_REFGORUP(soConId), curAppId))
#endif
        )
#endif
        {
#if (STD_ON == SOAD_SUPPORT_TX_BUFFER)
            if (socketIdValid && (NULL_PTR != BufPtr) && (0u < BufLength))
            {
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
                SoAd_InnerSocketConManageType* soConMgmtPtr;
#if (SOAD_PARTITION_NUM > 1u)
                uint16 partIndex =
                    SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(soConId)].PartitionId);
                soConMgmtPtr = &SoAd_SoConManagerPtr[partIndex][SoAd_SoConCfgPtr[soConId].PartitionSoConId];
#else
                soConMgmtPtr = &SoAd_SoConManagerPtr[soConId];
#endif

                PduIdType                pduRouteId     = soConMgmtPtr->ActiveSoAdPduRouteId;
                const SoAd_PduRouteType* pduRouteCfgPtr = &SoAd_PduRouteCfgPtr[pduRouteId];
                SoAd_UpLayerModuleType   upModule       = pduRouteCfgPtr->RefUpModule;

                PduInfoType   pduInfo;
                PduLengthType validDataLength;
                SoAd_uintx    txBufferId = *SOAD_SOCKET_TXBUF_INDEX_CFG(soConId);
                PduIdType     upPduId    = pduRouteCfgPtr->UpPduId;

                pduInfo.MetaDataPtr = NULL_PTR;
                pduInfo.SduLength   = BufLength;
                pduInfo.SduDataPtr  = BufPtr;
                ret = SoAd_UpperLayerPCCfgPtr[upModule].TpCopyTxDataFunc(upPduId, &pduInfo, NULL_PTR, &validDataLength);
                if (BUFREQ_OK == ret)
                {
#if (SOAD_PARTITION_NUM > 1u)
                    SoAd_uintx           startId         = txBufferId - SOAD_MULTI_PART_INFO.SocketTxBuffStartId;
                    SoAd_SoTxBufManType* soTxBuffMgmtPtr = &SoAd_SoTxBuffManagerPtr[partIndex][startId];
#else
                    SoAd_SoTxBufManType* soTxBuffMgmtPtr = &SoAd_SoTxBuffManagerPtr[txBufferId];
#endif
                    soTxBuffMgmtPtr->UpCopyLength += BufLength;
                    if (SOAD_SOCONGROUP_TCPIMME_TXCONF(SOAD_SOCON_REFGORUP(soConId))
                        && (soTxBuffMgmtPtr->TxPduTotalLength == soTxBuffMgmtPtr->UpCopyLength))
                    {
                        soTxBuffMgmtPtr->TxPduTotalLength = 0u;
                        soTxBuffMgmtPtr->UpCopyLength     = 0u;
                        soConMgmtPtr->TpPduTransmitting   = FALSE;

                        SoAd_UpperLayerPCCfgPtr[upModule].TpTxConfirmationFunc(upPduId, E_OK);
                    }
                    soConMgmtPtr->TxNoAckLength += BufLength;
                    SoAd_uintx pduRouteDestId = pduRouteCfgPtr->PduRouteDestStartId;
#if (SOAD_PARTITION_NUM > 1u)
                    startId = SoAd_PduRouteDestCfgPtr[pduRouteDestId].PartitionPduRouteDestId;
                    SoAd_PduRouteDestManagerPtr[partIndex][startId].TxPendingLength = soConMgmtPtr->TxNoAckLength;
#else
                    SoAd_PduRouteDestManagerPtr[pduRouteDestId].TxPendingLength = soConMgmtPtr->TxNoAckLength;
#endif
                }
                else
                {
                    SoAd_TpPduTxErrorHandle(soConId, txBufferId, upModule, upPduId);
                    ret = BUFREQ_E_NOT_OK;
                }
#endif
            }
#else
                    SOAD_NOUSED(SocketId);
                    SOAD_NOUSED(BufPtr);
                    SOAD_NOUSED(BufLength);
#endif
        }
    }

    return ret;
}
#define SOAD_STOP_SEC_CODE_FAST
#include "SoAd_MemMap.h"

/**
 * The TCP/IP stack calls this function after the data has been acknowledged by the peer for TCP.
 * Caveats: The upper layer might not be able to determine exactly which data bytes have been confirmed.
 */
#define SOAD_START_SEC_CODE_FAST
#include "SoAd_MemMap.h"
/* PRQA S 1532,6070 ++*/ /* VL_QAC_OneFunRef,VL_MTR_SoAd_STCAL */
void SoAd_TxConfirmation(TcpIp_SocketIdType SocketId, uint16 Length)
/* PRQA S 1532,6070 --*/
{
#if (STD_ON == SOAD_SUPPORT_TCP)
    SoAd_SoConIdType soConId = SOAD_NO_TXMETADATA;

#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    ApplicationType curAppId;
    if (SoAd_DetCheckInitStatus(SOAD_SID_TXCONFIRMATION, &curAppId))
#endif
    {
#if (SOAD_PARTITION_NUM > 1u)
        uint16 partIndex;
#endif

        SoAd_InnerSocketConManageType* soConMgmtPtr;
        boolean                        socketIdValid = SoAd_MatchTCPSocketId(SocketId, &soConId, FALSE);
        if (socketIdValid)
        {
#if (SOAD_PARTITION_NUM > 1u)
            partIndex    = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(soConId)].PartitionId);
            soConMgmtPtr = &SoAd_SoConManagerPtr[partIndex][SoAd_SoConCfgPtr[soConId].PartitionSoConId];
#else
            soConMgmtPtr = &SoAd_SoConManagerPtr[soConId];
#endif
        }
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
        if (SoAd_DetCheckSocketIdValid(SOAD_SID_TXCONFIRMATION, socketIdValid)
            && SoAd_DetCheckLength(SOAD_SID_TXCONFIRMATION, Length)
            && SoAd_DetCheckTCPTxNoAckLength(SOAD_SID_TXCONFIRMATION, Length, soConMgmtPtr->TxNoAckLength)
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
            && SoAd_DetCheckSoConIdPartition(SOAD_SID_TXCONFIRMATION, SOAD_SOCON_REFGORUP(soConId), curAppId)
#endif
        )
#endif
        {
#if (SOAD_PDU_ROUTE_DEST_MAX > 0u)
            const SoAd_SocketConnectionType* soConCfgPtr        = &SoAd_SoConCfgPtr[soConId];
            SoAd_uintx                       refPduRouteDestNum = soConCfgPtr->RefPduRouteDestNum;

            soConMgmtPtr->TxNoAckLength -= Length;
            for (SoAd_uintx cnt = 0u; cnt < refPduRouteDestNum; cnt++)
            {
                SoAd_uintx pduRouteDestId = soConCfgPtr->RefPduRouteDestPtr[cnt];
#if (SOAD_PARTITION_NUM > 1u)
                SoAd_uintx startId = SoAd_PduRouteDestCfgPtr[pduRouteDestId].PartitionPduRouteDestId;
                if (0u < SoAd_PduRouteDestManagerPtr[partIndex][startId].TxPendingLength)
#else
                if (0u < SoAd_PduRouteDestManagerPtr[pduRouteDestId].TxPendingLength)
#endif
                {
                    SoAd_TxConfirmationHandle(soConId, Length, pduRouteDestId);
                }
            }
#endif
        }
    }
#else
            SOAD_NOUSED(SocketId);
            SOAD_NOUSED(Length);
#endif
}
#define SOAD_STOP_SEC_CODE_FAST
#include "SoAd_MemMap.h"

/**
 * This service gets called if the stack put a socket into the listen mode before (as server) and a peer connected to
 * it (as client).
 * In detail: The TCP/IP stack calls this function after a socket was set into the listen state with TcpIp_TcpListen()
 * and a TCP connection is requested by the peer.
 */
#define SOAD_START_SEC_CODE_FAST
#include "SoAd_MemMap.h"
/* PRQA S 1532,6070,6030 ++*/ /* VL_QAC_OneFunRef,VL_MTR_SoAd_STCAL,VL_MTR_SoAd_STMIF */
Std_ReturnType SoAd_TcpAccepted(
    TcpIp_SocketIdType        SocketId,
    TcpIp_SocketIdType        SocketIdConnected,
    const TcpIp_SockAddrType* RemoteAddrPtr)
/* PRQA S 1532,6070,6030 -- */
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == SOAD_SUPPORT_TCP)
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /*SWS_SoAd_00272*/
    ApplicationType curAppId;
    if (SoAd_DetCheckInitStatus(SOAD_SID_TCPACCEPTED, &curAppId)
        && SoAd_DetCheckPtrParameter(SOAD_SID_TCPACCEPTED, RemoteAddrPtr))
#endif
    {
        SoAd_uintx soConGroupId;
        boolean    soConGroupValid = SoAd_MatchTCPSoConGroupSocketId(SocketId, &soConGroupId);

#if (STD_ON == SOAD_DEV_ERROR_DETECT)
        if (SoAd_DetCheckSocketIdValid(SOAD_SID_TCPACCEPTED, soConGroupValid)
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
            && (SoAd_DetCheckSoConIdPartition(SOAD_SID_TCPACCEPTED, soConGroupId, curAppId))
#endif
        )
#endif
        {
            SoAd_InnerSocketConManageType*        soConMgmtPtr;
            SoAd_SoConIdType                      soConId          = 0u;
            boolean                               soConValid       = FALSE;
            const SoAd_SocketConnectionGroupType* soConGroupCfgPtr = &SoAd_SoConGroupCfgPtr[soConGroupId];
            SoAd_InnerSocketConGroupManageType*   soConGroupMgmtPtr;
#if (SOAD_PARTITION_NUM > 1u)
            uint16 partIndex  = SoAd_GetPartitionIndex(soConGroupCfgPtr->PartitionId);
            soConGroupMgmtPtr = &SoAd_SoConGroupManagerPtr[partIndex][soConGroupCfgPtr->PartitionSoConGroupId];
            soConMgmtPtr      = SoAd_SoConManagerPtr[partIndex];
#else
            soConGroupMgmtPtr = &SoAd_SoConGroupManagerPtr[soConGroupId];
#endif

            /*SWS_SoAd_00594 */
            if (soConGroupCfgPtr->SocketMsgAcceptanceFilterEnabled)
            {
                boolean ret = SoAd_GetBestMatchAlgorithmSoCon(
                    soConGroupId,
                    RemoteAddrPtr,
                    &soConId,
                    SOAD_SOCON_MATCH_REQUEST_RECONNECT);
                if (ret)
                {
#if (SOAD_PARTITION_NUM > 1u)
                    soConMgmtPtr = soConMgmtPtr + SoAd_SoConCfgPtr[soConId].PartitionSoConId;
#else
                    soConMgmtPtr = &SoAd_SoConManagerPtr[soConId];
#endif
                    soConValid = TRUE;
                }
            }
            else
            {
                /* SWS_SoAd_00636
                 * if SoAd_SocketMsgAcceptanceFilterEnabled is FALSE:the SoConGroup just have only one SoCon
                 */
                soConId = soConGroupCfgPtr->SocketConStartId;
#if (SOAD_PARTITION_NUM > 1u)
                soConMgmtPtr = soConMgmtPtr + SoAd_SoConCfgPtr[soConId].PartitionSoConId;
#else
                soConMgmtPtr = &SoAd_SoConManagerPtr[soConId];
#endif
                if (!soConMgmtPtr->TcpIpSocketValid && (soConMgmtPtr->SoConMode != SOAD_SOCON_ONLINE))
                {
                    soConValid = TRUE;
                }
            }
            if (soConValid)
            {
                /* Set the new tcp communication socket parameter */
                SoAd_ChangeSocketParameter(SocketIdConnected, soConGroupId);
                SoAd_ChangeTcpSocketParameter(SocketIdConnected, soConGroupId);

                /*SWS_SoAd_00636:At SoAd_TcpAccepted(), SoAd shall perform the following
                actions if the TCP SoAdSocketConnectionGroup related to SocketId has both
                SoAdSocketTcpInitiate and SoAdSocketMsgAcceptanceFilterEnabled set to FALSE
                and is not online (i.e. current connection state not SOAD_SOCON_ONLINE)*/
                soConMgmtPtr->RemoteAddrIsSet  = TRUE;
                soConMgmtPtr->RemoteAddr       = *RemoteAddrPtr;
                soConMgmtPtr->TcpIpSocketValid = TRUE;
                soConMgmtPtr->TcpIpSocketId    = SocketIdConnected;
                soConMgmtPtr->SoConMode        = SOAD_SOCON_ONLINE;
                SoAd_SoConModeChgNotificationAndTpStartOfReception(soConId, soConMgmtPtr, soConGroupId);
                soConGroupMgmtPtr->SoConSocketValidNum += 1u;
                result = E_OK;
            }
        }
    }
#else
            SOAD_NOUSED(SocketId);
            SOAD_NOUSED(SocketIdConnected);
            SOAD_NOUSED(RemoteAddrPtr);
#endif
    return result;
}
#define SOAD_STOP_SEC_CODE_FAST
#include "SoAd_MemMap.h"

/**
 * This service gets called if the stack initiated a TCP connection before (as client) and the peer (the server)
 * acknowledged the connection set up.
 * In detail:The TCP/IP stack calls this function after a socket was requested to connect with TcpIp_TcpConnect() and
 * a TCP connection is confirmed by the peer.
 * The parameter value of SocketId equals the SocketId value of the preceeding TcpIp_TcpConnect() call.
 */
#define SOAD_START_SEC_CODE_FAST
#include "SoAd_MemMap.h"
/* PRQA S 1532 ++*/ /* VL_QAC_OneFunRef */
void SoAd_TcpConnected(TcpIp_SocketIdType SocketId)
/* PRQA S 1532 --*/
{
#if (STD_ON == SOAD_SUPPORT_TCP)
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /*SWS_SoAd_00274*/
    if (SoAd_DetCheckInitStatus(SOAD_SID_TCPCONNECTED, NULL_PTR))
#endif
    {
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION) && (SOAD_PARTITION_NUM > 1u)
        ApplicationType curAppId = GetApplicationID();
#endif
        SoAd_SoConIdType               soConId;
        SoAd_uintx                     soConGroupId;
        boolean                        socketIdValid = FALSE;
        SoAd_InnerSocketConManageType* soConMgmtPtr;
        const SoAd_SocketTcpType*      soTcpCfgPtr;

        for (soConId = 0u; soConId < SoAd_SoConNum; soConId++)
        {
            soConGroupId = SOAD_SOCON_REFGORUP(soConId);
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION) && (SOAD_PARTITION_NUM > 1u)
            uint16 partitionIdCfg = SoAd_SoConGroupCfgPtr[soConGroupId].PartitionId;
            if (partitionIdCfg != curAppId)
            {
                continue;
            }
            uint16 partIndex = SoAd_GetPartitionIndex(partitionIdCfg);
            soConMgmtPtr     = &SoAd_SoConManagerPtr[partIndex][SoAd_SoConCfgPtr[soConId].PartitionSoConId];
#else
            soConMgmtPtr = &SoAd_SoConManagerPtr[soConId];
#endif
            soTcpCfgPtr = SOAD_TCPTYPE_CFG(soConGroupId);
            /*SWS_SoAd_00593:Within SoAd_TcpConnected() SoAd shall change
             * the state of the socket connection to SOAD_SOCON_ONLINE*/
            if (soConMgmtPtr->TcpIpSocketValid && (SocketId == soConMgmtPtr->TcpIpSocketId)
                && (SOAD_TCP == SoAd_SoConCfgPtr[soConId].TcpUdpProtocol) && soTcpCfgPtr->SocketTcpInitiate)
            {
                if (SOAD_SOCON_ONLINE != soConMgmtPtr->SoConMode)
                {
                    soConMgmtPtr->SoConMode = SOAD_SOCON_ONLINE;
                    SoAd_SoConModeChgNotificationAndTpStartOfReception(soConId, soConMgmtPtr, soConGroupId);
                }
                socketIdValid = TRUE;
                break;
            }
        }
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
        (void)SoAd_DetCheckSocketIdValid(SOAD_SID_TCPCONNECTED, socketIdValid);
#endif
    }
#else
            SOAD_NOUSED(SocketId);
#endif
}
#define SOAD_STOP_SEC_CODE_FAST
#include "SoAd_MemMap.h"

/**
 * This service gets called if the stack encounters a condition described by the values in Event.
 */
#define SOAD_START_SEC_CODE_FAST
#include "SoAd_MemMap.h"
/* PRQA S 1532 ++*/ /* VL_QAC_OneFunRef */
void SoAd_TcpIpEvent(TcpIp_SocketIdType SocketId, TcpIp_EventType Event)
/* PRQA S 1532 --*/
{
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /**
     * SWS_SoAd_00276,SWS_SoAd_00278
     */
    if (SoAd_DetCheckInitStatus(SOAD_SID_TCPIPEVENT, NULL_PTR) && SoAd_DetCheckTCPEvent(SOAD_SID_TCPIPEVENT, Event))
#endif
    {
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION) && (SOAD_PARTITION_NUM > 1u)
        ApplicationType curAppId = GetApplicationID();
#endif
        if (TCPIP_TLS_HANDSHAKE_SUCCEEDED != Event)
        {
            boolean                             socketIdValid = FALSE;
            SoAd_InnerSocketConGroupManageType* soConGroupMgmtPtr;
            /*find a udp SoCon Group or a tcp SoCon Group(listen socket)*/

            for (SoAd_uintx groupId = 0u; (groupId < SOAD_SO_CON_GROUP_NUM) && (!socketIdValid); groupId++)
            {
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION) && (SOAD_PARTITION_NUM > 1u)
                uint16 partitionIdCfg = SoAd_SoConGroupCfgPtr[groupId].PartitionId;
                if (partitionIdCfg != curAppId)
                {
                    continue;
                }
                uint16 partIndex = SoAd_GetPartitionIndex(partitionIdCfg);
                soConGroupMgmtPtr =
                    &SoAd_SoConGroupManagerPtr[partIndex][SoAd_SoConGroupCfgPtr[groupId].PartitionSoConGroupId];
#else
                        soConGroupMgmtPtr = &SoAd_SoConGroupManagerPtr[groupId];
#endif
                if (soConGroupMgmtPtr->TcpIpSocketValid && (SocketId == soConGroupMgmtPtr->TcpIpSocketId))
                {
#if (STD_ON == SOAD_SUPPORT_TCP)
                    SoAd_CloseTcpSoConGroup(Event, groupId, soConGroupMgmtPtr);
#endif
#if (STD_ON == SOAD_SUPPORT_UDP)
                    SoAd_CloseUdpSoConGroup(Event, groupId);
#endif
                    socketIdValid = TRUE;
                }
            }

/*find a tcp socket connection*/
#if (STD_ON == SOAD_SUPPORT_TCP)
            SoAd_InnerSocketConManageType* soConMgmtPtr;

            for (SoAd_SoConIdType soConId = 0u; (soConId < SoAd_SoConNum) && (!socketIdValid); soConId++)
            {
/*SWS_SoAd_00646,SWS_SoAd_00688*/
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION) && (SOAD_PARTITION_NUM > 1u)
                uint16 partitionIdCfg = SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(soConId)].PartitionId;
                if (partitionIdCfg != curAppId)
                {
                    continue;
                }
                uint16 partIndex = SoAd_GetPartitionIndex(partitionIdCfg);
                soConMgmtPtr     = &SoAd_SoConManagerPtr[partIndex][SoAd_SoConCfgPtr[soConId].PartitionSoConId];
#else
                soConMgmtPtr = &SoAd_SoConManagerPtr[soConId];
#endif
                if (soConMgmtPtr->TcpIpSocketValid && (SocketId == soConMgmtPtr->TcpIpSocketId)
                    && (TCPIP_UDP_CLOSED != Event))
                {
                    SoAd_HandleTcpIpEventTCP(soConId, Event, soConMgmtPtr);
                    socketIdValid = TRUE;
                }
            }
#endif
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
            /*SWS_SoAd_00277*/
            (void)SoAd_DetCheckSocketIdValid(SOAD_SID_TCPIPEVENT, socketIdValid);
#endif
        }
    }
}
#define SOAD_STOP_SEC_CODE_FAST
#include "SoAd_MemMap.h"

/**
 * This service gets called by the TCP/IP stack if an IP address assignment changes (i.e. new address assigned or
 * assigned address becomes invalid).
 */
#define SOAD_START_SEC_CODE_FAST
#include "SoAd_MemMap.h"
/* PRQA S 1532,6030 ++*/ /* VL_QAC_OneFunRef,VL_MTR_SoAd_STMIF */
void SoAd_LocalIpAddrAssignmentChg(TcpIp_LocalAddrIdType IpAddrId, TcpIp_IpAddrStateType State)
/* PRQA S 1532,6030 --*/
{
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /*SWS_SoAd_00279*/
    if (SoAd_DetCheckInitStatus(SOAD_SID_LOCALIPADDRASSIGNMENTCHG, NULL_PTR))
#endif
    {
        /*SWS_SoAd_00729*/
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION) && (SOAD_PARTITION_NUM > 1u)
        ApplicationType curAppId = GetApplicationID();
#endif
        const SoAd_SocketConnectionGroupType* soConGroupCfgPtr;

        for (SoAd_uintx groupId = 0u; groupId < SOAD_SO_CON_GROUP_NUM; groupId++)
        {
            soConGroupCfgPtr = &SoAd_SoConGroupCfgPtr[groupId];
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION) && (SOAD_PARTITION_NUM > 1u)
            uint16 partitionIdCfg = soConGroupCfgPtr->PartitionId;
            if (partitionIdCfg != curAppId)
            {
                continue;
            }
            uint16 partIndex = SoAd_GetPartitionIndex(partitionIdCfg);
#endif
            if (IpAddrId == SOAD_SOCKETLOCALADDR_CFG(groupId).LocalAddrId)
            {
#if (SOAD_PARTITION_NUM > 1u)
                SoAd_SoConGroupManagerPtr[partIndex][soConGroupCfgPtr->PartitionSoConGroupId].LocalIpAddrState = State;
#else
                        SoAd_SoConGroupManagerPtr[groupId].LocalIpAddrState = State;
#endif
                SoAd_uintx                     socketConNum = soConGroupCfgPtr->SocketConNum;
                SoAd_InnerSocketConManageType* soConMgmtPtr;

                for (SoAd_SoConIdType cnt1 = 0u; cnt1 < socketConNum; cnt1++)
                {
                    SoAd_SoConIdType soConId = soConGroupCfgPtr->SocketConStartId + cnt1;
#if (SOAD_PARTITION_NUM > 1u)
                    soConMgmtPtr = &SoAd_SoConManagerPtr[partIndex][SoAd_SoConCfgPtr[soConId].PartitionSoConId];
#else
                            soConMgmtPtr = &SoAd_SoConManagerPtr[soConId];
#endif
                    if ((SOAD_SOCON_OFFLINE != soConMgmtPtr->SoConMode) && (TCPIP_IPADDR_STATE_UNASSIGNED == State))
                    {
                        soConMgmtPtr->SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
                        soConMgmtPtr->SetSocketNeedCloseLine = __LINE__;
#endif
                    }
                    SoAd_SoConLocalIpAddrAssignmentChg(soConId, groupId, State);
                }
            }
        }
    }
}
#define SOAD_STOP_SEC_CODE_FAST
#include "SoAd_MemMap.h"

/**
 * Schedules the Socket Adaptor. (Entry point for scheduling)
 */
#define SOAD_START_SEC_CODE
#include "SoAd_MemMap.h"
/**
 * @brief         Schedules the Socket Adaptor. (Entry point for scheduling).
 * @return        none
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace       CPD-74647
 */
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
/* PRQA S 1532,6070 ++*/ /* VL_QAC_OneFunRef,VL_MTR_SoAd_STCAL */
void SoAd_MainFunction(uint16 PartitionId)
/* PRQA S 1532,6070 --*/
{
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    ApplicationType curAppId = GetApplicationID();
    if (curAppId != PartitionId)
    {
        SOAD_DET_REPORT(SOAD_SID_MAINFUNCTION, SOAD_E_INVALID_PARTITION);
    }
#endif
#if (SOAD_PARTITION_NUM > 1u)
    uint16 partIndex = SoAd_GetPartitionIndex(PartitionId);
#else
    uint16 partIndex = 0u;
#endif
#else
        /* PRQA S 1532,6070 ++*/ /* VL_QAC_OneFunRef,VL_MTR_SoAd_STCAL */
        void SoAd_MainFunction(void)
        /* PRQA S 1532,6070 --*/
        {
#endif
    if (SOAD_STATE_INIT == SoAd_InitStatus)
    {
        for (SoAd_SoConIdType soConCnt = 0u; soConCnt < SoAd_SoConNum; soConCnt++)
        {
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION) && (SOAD_PARTITION_NUM > 1u)
            if (SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(soConCnt)].PartitionId != PartitionId)
            {
                continue;
            }
#endif
#if (STD_ON == SOAD_SUPPORT_TX_BUFFER)
            if (NULL_PTR != SOAD_SOCKET_TXBUF_INDEX_CFG(soConCnt))
            {
                SoAd_MainFunctionTpTransmitHandle(soConCnt);
            }
#endif
#if (STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER)
            SoAd_MainFunctionNPduUdpTxHandle(soConCnt);
#endif
            SoAd_ReleaseRemoteAddrMainFunctionHandle(soConCnt);
#if (STD_ON == SOAD_SUPPORT_UDP)
            SoAd_UdpAliveSupervisionTimeHandle(soConCnt);
#endif
            /*handle the Open/Close of requested(SoAd_OpenSoCon,SoAd_CloseSoCon,
             * SoAdSocketAutomaticSoConSetup) socket connections;
             * handle the Disconnection and recovery of SoCon*/
            SoAd_SoConModeChangeHandle(soConCnt);
#if (STD_ON == SOAD_SUPPORT_RX_BUFFER)
            /*copy the PDU data(in the Rx Buffer) to the Up module*/
            SoAd_SoConRxDataMainFunctionHandle(soConCnt);
#endif
        }
#if (SOAD_PDU_ROUTE_DEST_MAX > 0)
        SoAd_MainFunctionTxConfirmationHandle();
#endif
    }
}
#define SOAD_STOP_SEC_CODE
#include "SoAd_MemMap.h"

#define SOAD_START_SEC_CODE
#include "SoAd_MemMap.h"

/* PRQA S 1503 ++*/ /* VL_QAC_NoUsedApi */
void SoAd_DummyIfRxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr)
/* PRQA S 1503 --*/
{
    SOAD_NOUSED(RxPduId);
    SOAD_NOUSED(PduInfoPtr);
}
/* PRQA S 1532,3673 ++*/ /* VL_QAC_OneFunRef,VL_QAC_3673 */
Std_ReturnType SoAd_DummyIfTriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr)
/* PRQA S 1532,3673 --*/
{
    SOAD_NOUSED(TxPduId);
    SOAD_NOUSED(PduInfoPtr);
    return E_NOT_OK;
}

void SoAd_DummyIfTxConfirmation(PduIdType TxPduId, Std_ReturnType result)
{
    SOAD_NOUSED(TxPduId);
    SOAD_NOUSED(result);
}
/* PRQA S 1532,3673 ++*/ /* VL_QAC_OneFunRef,VL_QAC_3673 */
BufReq_ReturnType SoAd_DummyTpStartOfReception(
    PduIdType          RxPduId,
    const PduInfoType* info,
    PduLengthType      TpSduLength,
    PduLengthType*     bufferSizePtr)
/* PRQA S 1532,3673 --*/
{
    SOAD_NOUSED(RxPduId);
    SOAD_NOUSED(info);
    SOAD_NOUSED(TpSduLength);
    SOAD_NOUSED(bufferSizePtr);
    return BUFREQ_E_NOT_OK;
}
/* PRQA S 1532,3673 ++*/ /* VL_QAC_OneFunRef,VL_QAC_3673 */
BufReq_ReturnType SoAd_DummyTpCopyRxData(PduIdType id, const PduInfoType* info, PduLengthType* bufferSizePtr)
/* PRQA S 1532,3673 --*/
{
    SOAD_NOUSED(id);
    SOAD_NOUSED(info);
    SOAD_NOUSED(bufferSizePtr);
    return BUFREQ_E_NOT_OK;
}
/* PRQA S 1532 ++*/ /* VL_QAC_OneFunRef */
void SoAd_DummyTpRxIndication(PduIdType RxPduId, Std_ReturnType result)
/* PRQA S 1532 --*/
{
    SOAD_NOUSED(RxPduId);
    SOAD_NOUSED(result);
}
/* PRQA S 1532,3673 ++*/ /* VL_QAC_OneFunRef,VL_QAC_3673 */
BufReq_ReturnType SoAd_DummyTpCopyTxData(
    PduIdType            id,
    const PduInfoType*   info,
    const RetryInfoType* retry,
    PduLengthType*       availableDataPtr)
/* PRQA S 1532,3673 --*/
{
    SOAD_NOUSED(id);
    SOAD_NOUSED(info);
    SOAD_NOUSED(retry);
    SOAD_NOUSED(availableDataPtr);
    return BUFREQ_E_NOT_OK;
}
/* PRQA S 1532 ++*/ /* VL_QAC_OneFunRef */
void SoAd_DummyTpTxConfirmation(PduIdType TxPduId, Std_ReturnType result)
/* PRQA S 1532 --*/
{
    SOAD_NOUSED(TxPduId);
    SOAD_NOUSED(result);
}
/* PRQA S 1532 ++*/ /* VL_QAC_OneFunRef */
void SoAd_DummySoConModeChg(SoAd_SoConIdType SoConId, SoAd_SoConModeType Mode)
/* PRQA S 1532 --*/
{
    SOAD_NOUSED(SoConId);
    SOAD_NOUSED(Mode);
}
/* PRQA S 1532 ++*/ /* VL_QAC_OneFunRef */
void SoAd_DummyLocalIpAddrAssignmentChg(SoAd_SoConIdType SoConId, TcpIp_IpAddrStateType State)
/* PRQA S 1532 --*/
{
    SOAD_NOUSED(SoConId);
    SOAD_NOUSED(State);
}
#define SOAD_STOP_SEC_CODE
#include "SoAd_MemMap.h"

/* ========================================== internal function definition ========================================== */
#define SOAD_START_SEC_CODE
#include "SoAd_MemMap.h"
/**
 * @brief         Sets up the automatic socket connection for transmission.
 */
SOAD_LOCAL void SoAd_SocnTransmitAutomaticSoConSetupHandle(
    SoAd_SoConIdType               SoConId,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    TcpIp_ProtocolType             Protocol)
{
    /* socket connection of type automatic (i.e.
    configuration parameter SoAdSocketAutomaticSoConSetup set to TRUE)
    which uses a wildcard in the configured remote address */
    SoAd_uintx soConGroup = SOAD_SOCON_REFGORUP(SoConId);

    if (SoAd_SoConGroupCfgPtr[soConGroup].SocketAutomaticSoConSetup)
    {
        const TcpIp_SockAddrType* soConRemoteAdrCfgPtr = SOAD_SOCONREMOTEADDR_CFG(SoConId);
        if ((NULL_PTR != soConRemoteAdrCfgPtr)
            && ((SOAD_IS_IPADDR_ANY(*soConRemoteAdrCfgPtr)) || (TCPIP_PORT_ANY == soConRemoteAdrCfgPtr->port)))
        {
#if (STD_ON == SOAD_SUPPORT_TCP)
            if (SOAD_TCP == Protocol)
            {
                soConMgmtPtr->SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
                soConMgmtPtr->SetSocketNeedCloseLine = __LINE__;
#endif
            }
#endif
#if (STD_ON == SOAD_SUPPORT_UDP)
            if (SOAD_UDP == Protocol)
            {
                soConMgmtPtr->RemoteAddr = *soConRemoteAdrCfgPtr;
                soConMgmtPtr->SoConMode  = SOAD_SOCON_RECONNECT;
                SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, soConMgmtPtr, soConGroup);
            }
#endif
        }
    }
}

#if (SOAD_PDU_ROUTE_DEST_MAX > 0)
/**
 * @brief         Initializes the PDU route manager.
 */
SOAD_LOCAL void SoAd_PduRouteManagerInit(PduIdType TxPduId)
{
    SoAd_InnerPduRoutManageType* pduRouteMgmtPtr;
#if (SOAD_PARTITION_NUM > 1u)
    uint16 partIndex = SoAd_GetPartitionIndex(SoAd_PduRouteCfgPtr[TxPduId].PartitionId);
    pduRouteMgmtPtr  = &SoAd_PduRouteManagerPtr[partIndex][SoAd_PduRouteCfgPtr[TxPduId].PartitionPduRouteId];
#else
    pduRouteMgmtPtr = &SoAd_PduRouteManagerPtr[TxPduId];
#endif
    pduRouteMgmtPtr->PduDataPtr   = NULL_PTR;
    pduRouteMgmtPtr->PduLength    = 0u;
    pduRouteMgmtPtr->TxPendingNum = SOAD_UNUSED_UINT8;
    pduRouteMgmtPtr->TxSoConId    = SOAD_NO_TXMETADATA;
}
#endif

/**
 * @brief         Initializes Routing group runtime variable
 */
SOAD_LOCAL void SoAd_RoutingGroupInit(void)
{
    SoAd_RoutingGroupIdType routeGroupId;
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION) && (SOAD_PARTITION_NUM > 1u)
    ApplicationType curAppId = GetApplicationID();
#endif

    /* init all RouteGroup enable state */
#if (0u < SOAD_ROUTING_GROUP_MAX)
    const SoAd_RoutingGroupType* routeGroupCfgPtr;
    for (SoAd_RoutingGroupIdType cnt0 = 0u; cnt0 < SOAD_ROUTING_GROUP_NUM; cnt0++)
    {
        routeGroupCfgPtr = &SoAd_RouteGroupCfgPtr[cnt0];
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION) && (SOAD_PARTITION_NUM > 1u)
        if (routeGroupCfgPtr->PartitionId == curAppId)
        {
            uint16 partIndex = SoAd_GetPartitionIndex(routeGroupCfgPtr->PartitionId);
            SoAd_RoutGroupManagerPtr[partIndex][routeGroupCfgPtr->PartitionRoutGroupId].RoutGroupEnable =
                routeGroupCfgPtr->RoutingGroupIsEnabledAtInit;
        }
#else
        SoAd_RoutGroupManagerPtr[cnt0].RoutGroupEnable = routeGroupCfgPtr->RoutingGroupIsEnabledAtInit;
#endif
    }
#endif

    /* init all SocketRouteDest enable state */
#if (0u < SOAD_SOCKET_ROUTE_DEST_MAX)
    SoAd_InnerSocketRouteDestManagerType* socketRoutDestMPtr;

    for (SoAd_uintx cnt1 = 0u; cnt1 < SOAD_SOCKET_ROUTE_DEST_NUM; cnt1++)
    {
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION) && (SOAD_PARTITION_NUM > 1u)
        uint16 partitionId = SoAd_SocketRouteCfgPtr[SoAd_SoRouteDestCfgPtr[cnt1].SocketRouteIndex].PartitionId;
        if (partitionId != curAppId)
        {
            continue;
        }

        uint16 partIndex = SoAd_GetPartitionIndex(partitionId);
        socketRoutDestMPtr =
            &SoAd_SoRouteDestManagerPtr[partIndex][SoAd_SoRouteDestCfgPtr[cnt1].PartitionSocketRouteDestId];
#else
        socketRoutDestMPtr = &SoAd_SoRouteDestManagerPtr[cnt1];
#endif
        if (0u == SoAd_SoRouteDestCfgPtr[cnt1].RxRoutingGroupRefNum)
        {
            socketRoutDestMPtr->SocketRouteDestEnable = TRUE;
        }
#if (0u < SOAD_ROUTING_GROUP_MAX)
        else
        {
            SoAd_RoutingGroupIdType rxRoutingGroupRefNum = SoAd_SoRouteDestCfgPtr[cnt1].RxRoutingGroupRefNum;
            for (SoAd_RoutingGroupIdType cnt0 = 0u;
                 (cnt0 < rxRoutingGroupRefNum) && (!socketRoutDestMPtr->SocketRouteDestEnable);
                 cnt0++)
            {
                routeGroupId = SoAd_SoRouteDestCfgPtr[cnt1].RxRoutingGroupRefPtr[cnt0];
#if (SOAD_PARTITION_NUM > 1u)
                SoAd_RoutingGroupIdType groupId = SoAd_RouteGroupCfgPtr[routeGroupId].PartitionRoutGroupId;
                if (SoAd_RoutGroupManagerPtr[partIndex][groupId].RoutGroupEnable)
#else
                if (SoAd_RoutGroupManagerPtr[routeGroupId].RoutGroupEnable)
#endif
                {
                    socketRoutDestMPtr->SocketRouteDestEnable = TRUE;
                }
            }
        }
#endif
    }
#endif

    /*init all PduRouteDest enable state*/
#if (0u < SOAD_PDU_ROUTE_DEST_MAX)
    SoAd_InnerPduRouteDestManagerType* pduRouteDestMPtr;
    SoAd_RoutingGroupIdType            txRoutGroupRefNum;

    for (SoAd_uintx cnt2 = 0u; cnt2 < SOAD_PDU_ROUTE_DEST_NUM; cnt2++)
    {
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION) && (SOAD_PARTITION_NUM > 1u)
        uint16 partitionId = SoAd_PduRouteDestCfgPtr[cnt2].PartitionId;
        if (partitionId != curAppId)
        {
            continue;
        }
        uint16 partIndex = SoAd_GetPartitionIndex(partitionId);
        pduRouteDestMPtr =
            &SoAd_PduRouteDestManagerPtr[partIndex][SoAd_PduRouteDestCfgPtr[cnt2].PartitionPduRouteDestId];
#else
        pduRouteDestMPtr = &SoAd_PduRouteDestManagerPtr[cnt2];
#endif
        pduRouteDestMPtr->TxPendingLength = 0u;
        if (0u == SoAd_PduRouteDestCfgPtr[cnt2].TxRoutingGroupRefNum)
        {
            pduRouteDestMPtr->PduRouteDestEnable = TRUE;
        }
#if (0u < SOAD_ROUTING_GROUP_MAX)
        else
        {
            txRoutGroupRefNum = SoAd_PduRouteDestCfgPtr[cnt2].TxRoutingGroupRefNum;
            for (SoAd_RoutingGroupIdType cnt0 = 0u;
                 (cnt0 < txRoutGroupRefNum) && (!pduRouteDestMPtr->PduRouteDestEnable);
                 cnt0++)
            {
                routeGroupId = SoAd_PduRouteDestCfgPtr[cnt2].TxRoutingGroupRefPtr[cnt0];
#if (SOAD_PARTITION_NUM > 1u)
                SoAd_RoutingGroupIdType groupId = SoAd_RouteGroupCfgPtr[routeGroupId].PartitionRoutGroupId;
                if (SoAd_RoutGroupManagerPtr[partIndex][groupId].RoutGroupEnable)
#else
                if (SoAd_RoutGroupManagerPtr[routeGroupId].RoutGroupEnable)
#endif
                {
                    pduRouteDestMPtr->PduRouteDestEnable = TRUE;
                }
            }
        }
#endif
    }
#endif
}

#if ((0u < SOAD_MAX_IF_PDU_SIZE) && (SOAD_PDU_ROUTE_DEST_MAX > 0))
/**
 * @brief         Gets the IF PDU data.
 */
SOAD_LOCAL Std_ReturnType SoAd_GetIfPduData(
    const SoAd_PduRouteType*     pduRouteCfgPtr,
    SoAd_InnerPduRoutManageType* pduRouteMgmtPtr,
    const PduInfoType*           pduInfoPtr)
{
    Std_ReturnType result = E_NOT_OK;
    PduLengthType  sduLength;
#if (STD_ON == SOAD_PDU_ROUTE_METADATA_SUPPORT)
    uint8* metaDataPtr  = NULL_PTR;
    uint8  metadata[2u] = {0xFF, 0xFF}; /* PRQA S 3132 */ /* VL_QAC_MagicNum */
#endif
    /* obtain data form up layer */
    if ((NULL_PTR == pduInfoPtr) || (NULL_PTR == pduInfoPtr->SduDataPtr))
    {
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
        PduInfoType pduInfo;

        pduInfo.SduDataPtr  = &SoAd_IfPduBuffer[SOAD_HEADER_SIZE];
        pduInfo.SduLength   = (SOAD_MAX_IF_PDU_SIZE - SOAD_HEADER_SIZE);
        pduInfo.MetaDataPtr = NULL_PTR;
#if (STD_ON == SOAD_PDU_ROUTE_METADATA_SUPPORT)
        if (pduRouteCfgPtr->MetaDataEnable)
        {
            pduInfo.MetaDataPtr = &metadata[0];
            metaDataPtr         = &metadata[0];
        }
#endif
        SoAd_UpLayerModuleType upModule = pduRouteCfgPtr->RefUpModule;
        result    = SoAd_UpperLayerPCCfgPtr[upModule].IfTriggerTransmitFunc(pduRouteCfgPtr->UpPduId, &pduInfo);
        sduLength = pduInfo.SduLength;
#endif
    }
    else
    {
        /* store data to buffer */
        if (pduInfoPtr->SduLength <= (SOAD_MAX_IF_PDU_SIZE - SOAD_HEADER_SIZE))
        {
            SchM_Enter_SoAd_ExclusiveArea();
            (void)IStdLib_MemCpy(&SoAd_IfPduBuffer[SOAD_HEADER_SIZE], pduInfoPtr->SduDataPtr, pduInfoPtr->SduLength);
            SchM_Exit_SoAd_ExclusiveArea();
            sduLength = pduInfoPtr->SduLength;
            result    = E_OK;
#if (STD_ON == SOAD_PDU_ROUTE_METADATA_SUPPORT)
            metaDataPtr = pduInfoPtr->MetaDataPtr;
#endif
        }
    }

    if (E_OK == result)
    {
        pduRouteMgmtPtr->PduDataPtr = &SoAd_IfPduBuffer[SOAD_HEADER_SIZE];
        pduRouteMgmtPtr->PduLength  = sduLength;
        pduRouteMgmtPtr->TxSoConId  = SOAD_NO_TXMETADATA;
#if (STD_ON == SOAD_PDU_ROUTE_METADATA_SUPPORT)
        pduRouteMgmtPtr->TxSoConId = SoAd_GetSoConIdFromTxMetaData(pduRouteCfgPtr->MetaDataEnable, metaDataPtr);
#endif
    }
    else
    {
        pduRouteMgmtPtr->PduDataPtr = NULL_PTR;
    }
    return result;
}
#endif

/**
 * @brief         Checks if the remote address of a socket connection is locked.
 */
SOAD_LOCAL boolean
    SoAd_SoConRemoteAddrLocked(SoAd_SoConIdType SoConId, const SoAd_InnerSocketConManageType* soConMgmtPtr)
{
    boolean remoteAddrLocked = FALSE;
#if ((STD_ON == SOAD_SUPPORT_RX_BUFFER) || (STD_ON == SOAD_SUPPORT_TX_BUFFER))
    SoAd_uintx bufferId;
#if (SOAD_PARTITION_NUM > 1u)
    uint16 partIndex = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(SoConId)].PartitionId);
#endif
#endif
    const SoAd_SocketConnectionType* soConCfgPtr = &SoAd_SoConCfgPtr[SoConId];
    /*SWS_SoAd_00532*/
    if (SOAD_SOCON_OFFLINE == soConMgmtPtr->SoConMode)
    {
        remoteAddrLocked = FALSE;
    }
    else if (SOAD_UDP == soConCfgPtr->TcpUdpProtocol)
    {
        if (soConMgmtPtr->RxIfProcessing)
        {
            remoteAddrLocked = TRUE;
        }
#if (STD_ON == SOAD_SUPPORT_RX_BUFFER)
        /*if the Rx PDU is Receiving(UDP),not set the remote addr*/
        if ((NULL_PTR != soConCfgPtr->RxBuffCfgIndexPtr) && !remoteAddrLocked)
        {
            const SoAd_SoRxBufferManType* rxBuffMgmtPtr;
            bufferId = (soConCfgPtr->RxBuffCfgIndexPtr[0u] - SoAd_TxBufferNum);
#if (SOAD_PARTITION_NUM > 1u)
            SoAd_uintx id = bufferId - SOAD_MULTI_PART_INFO.SocketRxBuffStartId;
            rxBuffMgmtPtr = &SoAd_SoRxBuffManagerPtr[partIndex][id];
#else
            rxBuffMgmtPtr = &SoAd_SoRxBuffManagerPtr[bufferId];
#endif
            /*the Rx Buffer is empty and the Residue Rx Pdu length is 0*/
            if ((0u < rxBuffMgmtPtr->ExitPduResidueLength)
                || (rxBuffMgmtPtr->EnterPosition != rxBuffMgmtPtr->ExitPosition))
            {
                remoteAddrLocked = TRUE;
            }
        }
#endif
#if (STD_ON == SOAD_SUPPORT_TX_BUFFER)
        /*if the Tx PDU is transmitting(UDP),not set the remote addr*/
        if ((NULL_PTR != soConCfgPtr->TxBuffCfgIndexPtr) && !remoteAddrLocked)
        {
            bufferId = soConCfgPtr->TxBuffCfgIndexPtr[0u];
#if (SOAD_PARTITION_NUM > 1u)
            SoAd_uintx id = bufferId - SOAD_MULTI_PART_INFO.SocketTxBuffStartId;
            if (0u < SoAd_SoTxBuffManagerPtr[partIndex][id].TxPduTotalLength)
#else
            if (0u < SoAd_SoTxBuffManagerPtr[bufferId].TxPduTotalLength)
#endif
            {
                remoteAddrLocked = TRUE;
            }
        }
#endif
    }
    else
    {
        remoteAddrLocked = TRUE;
    }
    return remoteAddrLocked;
}

/**
 * @brief         Releases the remote address for a socket connection.
 */
SOAD_LOCAL void SoAd_ReleaseRemoteAddrHandle(
    SoAd_SoConIdType               SoConId,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    SoAd_uintx                     SoConGroupId)
{
    if (NULL_PTR != SOAD_SOCONREMOTEADDR_CFG(SoConId))
    {
        soConMgmtPtr->RemoteAddrIsSet = TRUE;
        soConMgmtPtr->RemoteAddr      = *SOAD_SOCONREMOTEADDR_CFG(SoConId);
    }
    /*Standard undefined,release to wildcards*/
    else
    {
        soConMgmtPtr->RemoteAddr.addr[0u] = 0x00000000u;
        soConMgmtPtr->RemoteAddr.port     = TCPIP_PORT_ANY;
    }
    /*SWS_SoAd_00687*/
    if ((SOAD_SOCON_ONLINE == soConMgmtPtr->SoConMode)
        && (SOAD_IS_IPADDR_ANY(soConMgmtPtr->RemoteAddr) || (TCPIP_PORT_ANY == soConMgmtPtr->RemoteAddr.port)))
    {
        soConMgmtPtr->SoConMode = SOAD_SOCON_RECONNECT;
        SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, soConMgmtPtr, SoConGroupId);
    }
}

/**
 * @brief         Releases the remote address in the main function for a socket connection.
 */
SOAD_LOCAL void SoAd_ReleaseRemoteAddrMainFunctionHandle(SoAd_SoConIdType SoConId)
{
    SoAd_uintx                     soConGroup = SOAD_SOCON_REFGORUP(SoConId);
    SoAd_InnerSocketConManageType* soConMgmtPtr;

#if (SOAD_PARTITION_NUM > 1u)
    uint16 partIndex = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[soConGroup].PartitionId);
    soConMgmtPtr     = &SoAd_SoConManagerPtr[partIndex][SoAd_SoConCfgPtr[SoConId].PartitionSoConId];
#else
            soConMgmtPtr = &SoAd_SoConManagerPtr[SoConId];
#endif
    if ((soConMgmtPtr->RemoteAddrNeedRelease) && (!SoAd_SoConRemoteAddrLocked(SoConId, soConMgmtPtr)))
    {
        SoAd_ReleaseRemoteAddrHandle(SoConId, soConMgmtPtr, soConGroup);
        soConMgmtPtr->RemoteAddrNeedRelease = FALSE;
    }
}

#if (STD_ON == SOAD_SUPPORT_RX_BUFFER)

#if (STD_ON == SOAD_SUPPORT_HEADERID)

/**
 * @brief         Handles the new PDU for header enable.
 */
/* PRQA S 6070 ++*/ /* VL_MTR_SoAd_STCAL */
SOAD_LOCAL void SoAd_HeaderEnableNewPduHandle(
    SoAd_SoConIdType               SoConId,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    SoAd_uintx                     BufferManageId,
    SoAd_SoRxBufferManType*        soRxBuffMgmtPtr)
/* PRQA S 6070 --*/
{
    PduLengthType enterPos       = soRxBuffMgmtPtr->EnterPosition;
    PduLengthType exitPos        = soRxBuffMgmtPtr->ExitPosition;
    PduLengthType bufferLength   = SoAd_SocketBufferCfgPtr[BufferManageId + SoAd_TxBufferNum].BuffLen;
    PduLengthType bufferStartPos = SoAd_SocketBufferCfgPtr[BufferManageId + SoAd_TxBufferNum].BuffPos;

    PduLengthType bufferedDataLength = SoAd_CalculateU16ValueByCondition(
        (exitPos < enterPos),
        (enterPos - exitPos),
        (bufferLength + enterPos - exitPos));

    if (bufferedDataLength >= SOAD_HEADER_SIZE)
    {
        PduLengthType bufStartId         = bufferStartPos + exitPos;
        PduLengthType destHeaderLocation = exitPos + SOAD_HEADER_SIZE;
        uint32        header;
        PduLengthType pduLength;
        /*the header data is overturn of the buffer*/
        if (destHeaderLocation > bufferLength)
        {
            uint8         headerData[SOAD_HEADER_SIZE];
            PduLengthType copyLen;
            destHeaderLocation -= bufferLength;
            copyLen = SOAD_HEADER_SIZE - destHeaderLocation;
            (void)IStdLib_MemCpy(&headerData[0u], &SoAd_PduBuffer[bufStartId], copyLen);
            (void)IStdLib_MemCpy(&headerData[copyLen], &SoAd_PduBuffer[bufferStartPos], destHeaderLocation);
            header    = SOAD_GET_HEADER(headerData, 0u);
            pduLength = (PduLengthType)(SOAD_GET_LENGTH(headerData, 0u));
        }
        else
        {
            header    = SOAD_GET_HEADER(SoAd_PduBuffer, bufStartId);
            pduLength = (PduLengthType)(SOAD_GET_LENGTH(SoAd_PduBuffer, bufStartId));
        }

        boolean    findValidSocketRoute = FALSE;
        SoAd_uintx socketRouteId;
        findValidSocketRoute = SoAd_FindHeaderIDAndSocketRouteId(SoConId, header, &socketRouteId);
        /*find the valid socket route(dest),and the socket route dest is enabled(according to the RouteGroup Manager)*/
#if (SOAD_PARTITION_NUM > 1u)
        uint16    partIndex  = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(SoConId)].PartitionId);
        PduIdType id         = SoAd_SoRouteDestCfgPtr[socketRouteId].PartitionSocketRouteDestId;
        boolean   destEnable = SoAd_SoRouteDestManagerPtr[partIndex][id].SocketRouteDestEnable;
#else
        boolean destEnable = SoAd_SoRouteDestManagerPtr[socketRouteId].SocketRouteDestEnable;
#endif
        if (findValidSocketRoute && destEnable)
        {
            soRxBuffMgmtPtr->SocketRouteId        = socketRouteId;
            soRxBuffMgmtPtr->ExitPduResidueLength = pduLength;
            exitPos += SOAD_HEADER_SIZE;
            exitPos = SoAd_CalculateU16ValueByCondition((exitPos >= bufferLength), (exitPos - bufferLength), exitPos);
            soRxBuffMgmtPtr->ExitPosition = exitPos;

            SoAd_UpdateTcpReceived(SoConId, soConMgmtPtr->TcpIpSocketId, SOAD_HEADER_SIZE);
#if (0u < SOAD_MAX_IF_PDU_SIZE)
            /*IF PDU data are in the SoAd Rx Buffer,just is TCP(Header Enable)*/
            if (SOAD_UPPER_LAYER_IF == SoAd_SoRouteDestCfgPtr[socketRouteId].RxUpperLayerType)
            {
                SoAd_HeaderEnableIfPduHandle(SoConId, soConMgmtPtr, BufferManageId, soRxBuffMgmtPtr);
            }
            else
#endif
            {
                SoAd_RxTpPduHeaderEnableHandle(pduLength, SoConId, socketRouteId, soConMgmtPtr, soRxBuffMgmtPtr);
            }
        }
        else
        {
            if (bufferedDataLength >= (pduLength + SOAD_HEADER_SIZE))
            {
                soRxBuffMgmtPtr->ExitPosition += (pduLength + SOAD_HEADER_SIZE);
                soRxBuffMgmtPtr->ExitPosition = SoAd_CalculateU16ValueByCondition(
                    (soRxBuffMgmtPtr->ExitPosition >= bufferLength),
                    (soRxBuffMgmtPtr->ExitPosition - bufferLength),
                    soRxBuffMgmtPtr->ExitPosition);
                uint32 tempLen = (uint32)(pduLength) + SOAD_HEADER_DATA_SIZE + SOAD_HEADER_LENGTH_SIZE;
                SoAd_UpdateTcpReceived(SoConId, soConMgmtPtr->TcpIpSocketId, tempLen);
                (void)Det_ReportRuntimeError(
                    SOAD_MODULE_ID,
                    SOAD_INSTANCE,
                    SOAD_SID_MAINFUNCTION,
                    SOAD_E_INV_PDUHEADER_ID);
            }

            /*received TCP FIN, never receive data*/
            if ((soConMgmtPtr->SocketNeedClose == SOAD_SOCKET_CLOSE_DEFERRED)
                && (SOAD_TCP == SoAd_SoConCfgPtr[SoConId].TcpUdpProtocol))
            {
                soRxBuffMgmtPtr->ExitPosition = soRxBuffMgmtPtr->EnterPosition;
            }
        }
    }
    else
    {
        /*received TCP FIN, never receive data*/
        if ((soConMgmtPtr->SocketNeedClose == SOAD_SOCKET_CLOSE_DEFERRED)
            && (SOAD_TCP == SoAd_SoConCfgPtr[SoConId].TcpUdpProtocol))
        {
            soRxBuffMgmtPtr->ExitPosition = soRxBuffMgmtPtr->EnterPosition;
        }
    }
}

/**
 * @brief         Handles the old PDU for header enable.
 */
SOAD_LOCAL void SoAd_HeaderEnableOldPduHandle(
    SoAd_SoConIdType               SoConId,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    SoAd_uintx                     BufferManageId,
    SoAd_SoRxBufferManType*        soRxBuffMgmtPtr)
{
    SoAd_uintx                      socketRouteId        = soRxBuffMgmtPtr->SocketRouteId;
    const SoAd_SocketRouteDestType* socketRoutDestCfgPtr = &SoAd_SoRouteDestCfgPtr[socketRouteId];

#if (0u < SOAD_MAX_IF_PDU_SIZE)
    if (SOAD_UPPER_LAYER_IF == socketRoutDestCfgPtr->RxUpperLayerType)
    {
        SoAd_HeaderEnableIfPduHandle(SoConId, soConMgmtPtr, BufferManageId, soRxBuffMgmtPtr);
    }
    else
#endif
    {
        PduInfoType pduInfo;

        pduInfo.SduDataPtr  = NULL_PTR;
        pduInfo.SduLength   = 0u;
        pduInfo.MetaDataPtr = NULL_PTR;
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
        uint8 metaData[2u]; /* PRQA S 3132 */ /* VL_QAC_MagicNum */
        pduInfo.MetaDataPtr = SoAd_MetaDataFillHandle(SoConId, socketRoutDestCfgPtr->MetaDataEnable, &metaData[0u]);
#endif
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
        PduLengthType          upBufferSize;
        PduIdType              upPduId  = socketRoutDestCfgPtr->UpRxPduId;
        SoAd_UpLayerModuleType upModule = socketRoutDestCfgPtr->RefUpModule;

        if (BUFREQ_OK == SoAd_UpperLayerPCCfgPtr[upModule].TpCopyRxDataFunc(upPduId, &pduInfo, &upBufferSize))
        {
            SoAd_HeaderEnableTpPduHandle(SoConId, soConMgmtPtr, upBufferSize, soRxBuffMgmtPtr);
        }
        else
        {
            SoAd_UpperLayerPCCfgPtr[upModule].TpRxindicationFunc(upPduId, E_NOT_OK);
            soConMgmtPtr->SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
            soConMgmtPtr->SetSocketNeedCloseLine = __LINE__;
#endif
        }
#endif
    }
}

/**
 * @brief         Handles the TP PDU for header enable.
 */
SOAD_LOCAL void SoAd_HeaderEnableTpPduHandle(
    SoAd_SoConIdType               SoConId,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    PduLengthType                  UpBufferSize,
    SoAd_SoRxBufferManType*        soRxBuffMgmtPtr)
{
    PduLengthType                   copyToUpLength;
    SoAd_uintx                      bufferId             = SoAd_SoConCfgPtr[SoConId].RxBuffCfgIndexPtr[0u];
    PduLengthType                   bufferLength         = SoAd_SocketBufferCfgPtr[bufferId].BuffLen;
    PduLengthType                   bufferStartPos       = SoAd_SocketBufferCfgPtr[bufferId].BuffPos;
    SoAd_uintx                      socketRouteId        = soRxBuffMgmtPtr->SocketRouteId;
    const SoAd_SocketRouteDestType* socketRoutDestCfgPtr = &SoAd_SoRouteDestCfgPtr[socketRouteId];

    if (soRxBuffMgmtPtr->ExitPosition < soRxBuffMgmtPtr->EnterPosition)
    {
        copyToUpLength = soRxBuffMgmtPtr->EnterPosition - soRxBuffMgmtPtr->ExitPosition;
    }
    else
    {
        copyToUpLength = bufferLength - soRxBuffMgmtPtr->ExitPosition;
    }
    if (copyToUpLength > UpBufferSize)
    {
        copyToUpLength = UpBufferSize;
    }
    if (copyToUpLength >= soRxBuffMgmtPtr->ExitPduResidueLength)
    {
        copyToUpLength = soRxBuffMgmtPtr->ExitPduResidueLength;
    }
    if (copyToUpLength > 0u)
    {
        PduInfoType pduInfo;
        pduInfo.SduDataPtr  = &SoAd_PduBuffer[bufferStartPos + soRxBuffMgmtPtr->ExitPosition];
        pduInfo.SduLength   = copyToUpLength;
        pduInfo.MetaDataPtr = NULL_PTR;
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
        uint8 metaData[2u]; /* PRQA S 3132 */ /* VL_QAC_MagicNum */
        pduInfo.MetaDataPtr = SoAd_MetaDataFillHandle(SoConId, socketRoutDestCfgPtr->MetaDataEnable, &metaData[0u]);
#endif

#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
        PduIdType              upPduId = socketRoutDestCfgPtr->UpRxPduId;
        PduLengthType          upBufferSize;
        SoAd_UpLayerModuleType upModule = socketRoutDestCfgPtr->RefUpModule;
        if (BUFREQ_OK == SoAd_UpperLayerPCCfgPtr[upModule].TpCopyRxDataFunc(upPduId, &pduInfo, &upBufferSize))
        {
            SoAd_UpdateTcpReceived(SoConId, soConMgmtPtr->TcpIpSocketId, copyToUpLength);
            soRxBuffMgmtPtr->ExitPosition += copyToUpLength;
            if (soRxBuffMgmtPtr->ExitPosition >= bufferLength)
            {
                soRxBuffMgmtPtr->ExitPosition -= bufferLength;
            }
            soRxBuffMgmtPtr->ExitPduResidueLength -= copyToUpLength;
            if (0u == soRxBuffMgmtPtr->ExitPduResidueLength)
            {
                SoAd_UpperLayerPCCfgPtr[upModule].TpRxindicationFunc(upPduId, E_OK);
            }
        }
        else
        {
            SoAd_UpperLayerPCCfgPtr[upModule].TpRxindicationFunc(upPduId, E_NOT_OK);
            soConMgmtPtr->SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
            soConMgmtPtr->SetSocketNeedCloseLine = __LINE__;
#endif
        }
#endif
    }
}
#endif

#if ((0u < SOAD_MAX_IF_PDU_SIZE) && (STD_ON == SOAD_SUPPORT_HEADERID))

/**
 * @brief         Handles the IF PDU for header enable.
 */
SOAD_LOCAL void SoAd_HeaderEnableIfPduHandle(
    SoAd_SoConIdType                     SoConId,
    const SoAd_InnerSocketConManageType* soConMgmtPtr,
    SoAd_uintx                           BufferManageId,
    SoAd_SoRxBufferManType*              soRxBuffMgmtPtr)
{
    PduLengthType                   bufferedDataLength;
    PduLengthType                   enterPos          = soRxBuffMgmtPtr->EnterPosition;
    PduLengthType                   exitPos           = soRxBuffMgmtPtr->ExitPosition;
    PduLengthType                   exitPduResidueLen = soRxBuffMgmtPtr->ExitPduResidueLength;
    PduLengthType                   bufferLength   = SoAd_SocketBufferCfgPtr[BufferManageId + SoAd_TxBufferNum].BuffLen;
    PduLengthType                   bufferStartPos = SoAd_SocketBufferCfgPtr[BufferManageId + SoAd_TxBufferNum].BuffPos;
    SoAd_uintx                      socketRouteId  = soRxBuffMgmtPtr->SocketRouteId;
    const SoAd_SocketRouteDestType* socketRoutDestCfgPtr = &SoAd_SoRouteDestCfgPtr[socketRouteId];

    if (exitPos < enterPos)
    {
        bufferedDataLength = enterPos - exitPos;
    }
    else
    {
        bufferedDataLength = bufferLength + enterPos - exitPos;
    }

    if (bufferedDataLength >= exitPduResidueLen)
    {
        PduInfoType   pduInfo;
        PduLengthType srcPos  = bufferStartPos + exitPos;
        PduLengthType destPos = exitPos + exitPduResidueLen;
        pduInfo.SduLength     = exitPduResidueLen;
        pduInfo.MetaDataPtr   = NULL_PTR;
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
        uint8 metaData[2u]; /* PRQA S 3132 */ /* VL_QAC_MagicNum */
        pduInfo.MetaDataPtr = SoAd_MetaDataFillHandle(SoConId, socketRoutDestCfgPtr->MetaDataEnable, &metaData[0u]);
#endif
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
        PduIdType              upPduId  = socketRoutDestCfgPtr->UpRxPduId;
        SoAd_UpLayerModuleType upModule = socketRoutDestCfgPtr->RefUpModule;
        if (destPos > bufferLength)
        {
            SchM_Enter_SoAd_ExclusiveArea();
            /*SoAd_IfPduBuffer is used for IF TCP receive buffer*/
            PduLengthType copyLength = bufferLength - exitPos;
            (void)IStdLib_MemCpy(&SoAd_IfPduBuffer[0u], &SoAd_PduBuffer[srcPos], copyLength);
            (void)IStdLib_MemCpy(
                &SoAd_IfPduBuffer[copyLength],
                &SoAd_PduBuffer[bufferStartPos],
                (PduLengthType)(destPos - bufferLength));
            pduInfo.SduDataPtr = &SoAd_IfPduBuffer[0u];
            SchM_Exit_SoAd_ExclusiveArea();
            SoAd_UpperLayerPCCfgPtr[upModule].IfRxIndicationFunc(upPduId, &pduInfo);
        }
        else
        {
            pduInfo.SduDataPtr = &SoAd_PduBuffer[srcPos];
            SoAd_UpperLayerPCCfgPtr[upModule].IfRxIndicationFunc(upPduId, &pduInfo);
        }
#endif
        /*IF PDU used Rx Buffer must be TCP.
        SWS_SoAd_00564:SoAd shall confirm the reception of all data*/
        SoAd_UpdateTcpReceived(SoConId, soConMgmtPtr->TcpIpSocketId, exitPduResidueLen);

        soRxBuffMgmtPtr->ExitPosition = destPos;
        if (soRxBuffMgmtPtr->ExitPosition >= bufferLength)
        {
            soRxBuffMgmtPtr->ExitPosition -= bufferLength;
        }
        soRxBuffMgmtPtr->ExitPduResidueLength = 0u;
    }
}
#endif

/**
 * @brief         Handles the reception data in the main function with header disable.
 */
SOAD_LOCAL void SoAd_RxDataMainFunctionHeaderDisableHandle(
    SoAd_SoConIdType               SoConId,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    SoAd_uintx                     BufferManageId,
    SoAd_SoRxBufferManType*        soRxBuffMgmtPtr)
{
    PduInfoType                     pduInfo;
    SoAd_uintx                      socketRouteId        = SoAd_SoConCfgPtr[SoConId].RefSocketRoutePtr[0u];
    const SoAd_SocketRouteDestType* socketRoutDestCfgPtr = &SoAd_SoRouteDestCfgPtr[socketRouteId];

    pduInfo.SduDataPtr  = NULL_PTR;
    pduInfo.SduLength   = 0u;
    pduInfo.MetaDataPtr = NULL_PTR;
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
    uint8 metaData[2u]; /* PRQA S 3132 */ /* VL_QAC_MagicNum */
    pduInfo.MetaDataPtr = SoAd_MetaDataFillHandle(SoConId, socketRoutDestCfgPtr->MetaDataEnable, &metaData[0u]);
#endif

#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
    PduIdType              upPduId  = socketRoutDestCfgPtr->UpRxPduId;
    SoAd_UpLayerModuleType upModule = socketRoutDestCfgPtr->RefUpModule;
    PduLengthType          upBufferSize;

    /* obtain uplayer available buffer size */
    if (BUFREQ_OK == SoAd_UpperLayerPCCfgPtr[upModule].TpCopyRxDataFunc(upPduId, &pduInfo, &upBufferSize))
    {
        PduLengthType copyToUpLength;
        PduLengthType bufferLength   = SoAd_SocketBufferCfgPtr[BufferManageId + SoAd_TxBufferNum].BuffLen;
        PduLengthType bufferStartPos = SoAd_SocketBufferCfgPtr[BufferManageId + SoAd_TxBufferNum].BuffPos;

        if (soRxBuffMgmtPtr->ExitPosition < soRxBuffMgmtPtr->EnterPosition)
        {
            copyToUpLength = soRxBuffMgmtPtr->EnterPosition - soRxBuffMgmtPtr->ExitPosition;
        }
        else
        {
            copyToUpLength = (bufferLength - soRxBuffMgmtPtr->ExitPosition);
        }
        if (copyToUpLength > upBufferSize)
        {
            copyToUpLength = upBufferSize;
        }
        if (copyToUpLength > 0u)
        {
            pduInfo.SduDataPtr = &SoAd_PduBuffer[bufferStartPos + soRxBuffMgmtPtr->ExitPosition];
            pduInfo.SduLength  = copyToUpLength;
            if (BUFREQ_OK == SoAd_UpperLayerPCCfgPtr[upModule].TpCopyRxDataFunc(upPduId, &pduInfo, &upBufferSize))
            {
                SoAd_UpdateTcpReceived(SoConId, soConMgmtPtr->TcpIpSocketId, copyToUpLength);
                soRxBuffMgmtPtr->ExitPosition += copyToUpLength;
                if (soRxBuffMgmtPtr->ExitPosition >= bufferLength)
                {
                    soRxBuffMgmtPtr->ExitPosition -= bufferLength;
                }
            }
            else
            {
                soConMgmtPtr->SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
                soConMgmtPtr->SetSocketNeedCloseLine = __LINE__;
#endif
            }
        }
    }
    else
    {
        soConMgmtPtr->SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
        soConMgmtPtr->SetSocketNeedCloseLine = __LINE__;
#endif
    }
#endif
}

/**
 * @brief         Handles the reception data in the main function for a socket connection.
 */
SOAD_LOCAL void SoAd_SoConRxDataMainFunctionHandle(SoAd_SoConIdType SoConId)
{
    const SoAd_SocketConnectionType* soConCfgPtr = &SoAd_SoConCfgPtr[SoConId];

    if ((NULL_PTR != soConCfgPtr->RxBuffCfgIndexPtr) && (NULL_PTR != soConCfgPtr->RefSocketRoutePtr))
    {
        SoAd_InnerSocketConManageType* soConMgmtPtr;
        SoAd_uintx                     bufferId = soConCfgPtr->RxBuffCfgIndexPtr[0u] - SoAd_TxBufferNum;
#if (SOAD_PARTITION_NUM > 1u)
        uint16     partIndex = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(SoConId)].PartitionId);
        SoAd_uintx id        = bufferId - SOAD_MULTI_PART_INFO.SocketRxBuffStartId;
        SoAd_SoRxBufferManType* soRxBuffMgmtPtr = &SoAd_SoRxBuffManagerPtr[partIndex][id];
        soConMgmtPtr = &SoAd_SoConManagerPtr[partIndex][SoAd_SoConCfgPtr[SoConId].PartitionSoConId];
#else
        SoAd_SoRxBufferManType* soRxBuffMgmtPtr = &SoAd_SoRxBuffManagerPtr[bufferId];
        soConMgmtPtr                            = &SoAd_SoConManagerPtr[SoConId];
#endif

        /*the EnterPosition is not equal to ExitPosition:the Rx Buffer have store receive data*/
        if (soRxBuffMgmtPtr->EnterPosition != soRxBuffMgmtPtr->ExitPosition)
        {
#if (STD_ON == SOAD_SUPPORT_HEADERID)
            SoAd_uintx soConGroupId  = soConCfgPtr->SocketConnectionGroupRef;
            boolean    headerEnabled = SoAd_SoConGroupCfgPtr[soConGroupId].PduHeaderEnable;
            /*header enabled handle*/
            if (headerEnabled)
            {
                /*need to analysis the newly PDU header*/
                if (0u == soRxBuffMgmtPtr->ExitPduResidueLength)
                {
                    SoAd_HeaderEnableNewPduHandle(SoConId, soConMgmtPtr, bufferId, soRxBuffMgmtPtr);
                }
                /*don't need to analysis the newly PDU header*/
                else
                {
                    SoAd_HeaderEnableOldPduHandle(SoConId, soConMgmtPtr, bufferId, soRxBuffMgmtPtr);
                }
            }
            /*header disabled handle:just TP PDU may use the Rx Buffer*/
            else
#endif
            {
                SoAd_RxDataMainFunctionHeaderDisableHandle(SoConId, soConMgmtPtr, bufferId, soRxBuffMgmtPtr);
            }
        }
        else
        {
            if (soConMgmtPtr->SocketNeedClose == SOAD_SOCKET_CLOSE_DEFERRED)
            {
                soConMgmtPtr->SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
                soConMgmtPtr->SetSocketNeedCloseLine = __LINE__;
#endif
            }
        }
    }
}
#endif

#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
/**
 * @brief     Fill metadata to SDU
 */
SOAD_LOCAL uint8* SoAd_MetaDataFillHandle(SoAd_SoConIdType SoConId, boolean metaDataEnable, uint8* DataPtr)
{
    uint8* metaDataPtr = NULL_PTR;

    if (metaDataEnable)
    {
#if (0x100u > SOAD_SO_CON_MAX)
        DataPtr[0u] = SoConId;
#else
        if (LOW_BYTE_FIRST == CPU_BYTE_ORDER)
        {
            DataPtr[0u] = (uint8)SoConId;
            DataPtr[1u] = (uint8)(SoConId >> 8u);
        }
        else
        {
            DataPtr[0u] = (uint8)(SoConId >> 8u);
            DataPtr[1u] = (uint8)SoConId;
        }
#endif
        metaDataPtr = DataPtr;
    }

    return metaDataPtr;
}
#endif
#if (0u < SOAD_SOCKET_ROUTE_DEST_MAX)
#if (STD_ON == SOAD_SUPPORT_UDP)
#if (STD_ON == SOAD_SUPPORT_HEADERID)
#if (STD_ON == SOAD_SUPPORT_RX_BUFFER)
/**
 * @brief         Handle received TP PDU for a UDP Header enable socket connection.
 */
/* PRQA S 6030 ++ */ /* VL_MTR_SoAd_STMIF */
SOAD_LOCAL boolean SoAd_HeaderEnableUdpSoConRxTpPduHandle(
    SoAd_SoConIdType               SoConId,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    SoAd_uintx                     SocketRouteId,
    PduLengthType*                 DataPosition,
    uint8*                         DataPtr)
/* PRQA S 6030 -- */
{
    boolean       validPduData = TRUE;
    PduLengthType dataPos      = *DataPosition;
    PduLengthType pduLength    = (PduLengthType)(SOAD_GET_LENGTH(DataPtr, dataPos));

    if (0u < pduLength)
    {
        SoAd_uintx bufferId = SoAd_SoConCfgPtr[SoConId].RxBuffCfgIndexPtr[0] - SoAd_TxBufferNum;
#if (SOAD_PARTITION_NUM > 1u)
        uint16     partIndex = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(SoConId)].PartitionId);
        SoAd_uintx id        = bufferId - SOAD_MULTI_PART_INFO.SocketRxBuffStartId;
        SoAd_SoRxBufferManType* soRxBuffMgmtPtr = &SoAd_SoRxBuffManagerPtr[partIndex][id];
#else
        SoAd_SoRxBufferManType* soRxBuffMgmtPtr = &SoAd_SoRxBuffManagerPtr[bufferId];
#endif
        /*check the Rx Buffer is empty or not*/
        if ((soRxBuffMgmtPtr->EnterPosition == soRxBuffMgmtPtr->ExitPosition)
            && (0u == soRxBuffMgmtPtr->ExitPduResidueLength))
        {
            PduInfoType                     pduInfo;
            const SoAd_SocketRouteDestType* socketRoutDestCfgPtr = &SoAd_SoRouteDestCfgPtr[SocketRouteId];

            pduInfo.SduDataPtr  = NULL_PTR;
            pduInfo.SduLength   = 0U;
            pduInfo.MetaDataPtr = NULL_PTR;
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
            uint8 metaData[2u]; /* PRQA S 3132 */ /* VL_QAC_MagicNum */
            pduInfo.MetaDataPtr = SoAd_MetaDataFillHandle(SoConId, socketRoutDestCfgPtr->MetaDataEnable, &metaData[0u]);
#endif

#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
            PduLengthType          upBufferSize;
            PduIdType              upPduId  = socketRoutDestCfgPtr->UpRxPduId;
            SoAd_UpLayerModuleType upModule = socketRoutDestCfgPtr->RefUpModule;

            if (BUFREQ_OK
                == SoAd_UpperLayerPCCfgPtr[upModule]
                       .TpStartOfReceptionFunc(upPduId, &pduInfo, pduLength, &upBufferSize))
            {
                if ((upBufferSize >= pduLength) || (upBufferSize > 0u))
                {
                    pduInfo.SduDataPtr = &DataPtr[dataPos + SOAD_HEADER_SIZE];
                    pduInfo.SduLength  = (upBufferSize >= pduLength) ? pduLength : upBufferSize;
                    PduLengthType copyUpBufferSize;
                    if (BUFREQ_OK
                        == SoAd_UpperLayerPCCfgPtr[upModule].TpCopyRxDataFunc(upPduId, &pduInfo, &copyUpBufferSize))
                    {
                        if (upBufferSize >= pduLength)
                        {
                            SoAd_UpperLayerPCCfgPtr[upModule].TpRxindicationFunc(upPduId, E_OK);
                        }
                        else
                        {
                            /*copy the remainder data(Length-pduInfo.SduLength) to SoAd Rx Buffer */
                            boolean ret = SoAd_CopyDataToRxBuffer(
                                SoConId,
                                (pduLength - pduInfo.SduLength),
                                &DataPtr[dataPos + SOAD_HEADER_SIZE + pduInfo.SduLength],
                                soConMgmtPtr,
                                soRxBuffMgmtPtr);
                            if (ret)
                            {
                                soRxBuffMgmtPtr->ExitPduResidueLength = pduLength - pduInfo.SduLength;
                                soRxBuffMgmtPtr->SocketRouteId        = SocketRouteId;
                            }
                        }
                    }
                    else
                    {
                        SoAd_UpperLayerPCCfgPtr[upModule].TpRxindicationFunc(upPduId, E_NOT_OK);
                    }
                }
                else
                {
                    /*copy the remainder data(Length) to SoAd Rx Buffer */
                    boolean ret = SoAd_CopyDataToRxBuffer(
                        SoConId,
                        pduLength,
                        &DataPtr[dataPos + SOAD_HEADER_SIZE],
                        soConMgmtPtr,
                        soRxBuffMgmtPtr);
                    if (ret)
                    {
                        soRxBuffMgmtPtr->ExitPduResidueLength = pduLength;
                        soRxBuffMgmtPtr->SocketRouteId        = SocketRouteId;
                    }
                }
            }
#endif
        }
        else
        {
            /*copy the data(Length+8) to Rx Buffer*/
            (void)SoAd_CopyDataToRxBuffer(
                SoConId,
                (SOAD_HEADER_SIZE + pduLength),
                &DataPtr[dataPos],
                soConMgmtPtr,
                soRxBuffMgmtPtr);
        }
    }
    else
    {
        validPduData = FALSE;
    }

    *DataPosition += (SOAD_HEADER_SIZE + pduLength);
    return validPduData;
}
#endif

/**
 * @brief         Handles the reception of a UDP socket connection.
 */
/* PRQA S 6030 ++ */ /* VL_MTR_SoAd_STMIF */
SOAD_LOCAL boolean SoAd_HeaderEnableUdpSoConRxHandle(
    SoAd_SoConIdType               SoConId,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    uint16                         RxMessageLength,
    uint8*                         DataPtr)
/* PRQA S 6030 -- */
{
    boolean                         validPduData = TRUE;
    boolean                         findValidSocketRoute;
    uint32                          header;
    SoAd_UpLayerModuleType          upModule;
    PduInfoType                     pduInfo;
    SoAd_uintx                      socketRouteId;
    PduLengthType                   dataPosition = 0u;
    PduLengthType                   pduLength;
    const SoAd_SocketRouteDestType* soRouteDestCfgPtr;
    PduLengthType                   remainLen;
    SoAd_uintx                      soConGroupId = SOAD_SOCON_REFGORUP(SoConId);

    if (SoAd_UdpStrictHeaderLenCheck(soConGroupId, DataPtr, RxMessageLength))
    {
        while (dataPosition < RxMessageLength)
        {
            header    = SOAD_GET_HEADER(DataPtr, dataPosition);
            pduLength = (PduLengthType)(SOAD_GET_LENGTH(DataPtr, dataPosition));
            remainLen = dataPosition + SOAD_HEADER_SIZE + pduLength;
            if (remainLen <= RxMessageLength)
            {
                findValidSocketRoute = SoAd_FindHeaderIDAndSocketRouteId(SoConId, header, &socketRouteId);

                /*findValidSocketRoute and socket dest routing group is enabled*/
                if (findValidSocketRoute)
                {
                    /*SWS_SoAd_00600*/
#if (SOAD_PARTITION_NUM > 1u)
                    uint16 partIndex =
                        SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(SoConId)].PartitionId);
                    uint16  id         = SoAd_SoRouteDestCfgPtr[socketRouteId].PartitionSocketRouteDestId;
                    boolean destEnable = SoAd_SoRouteDestManagerPtr[partIndex][id].SocketRouteDestEnable;
#else
                    boolean destEnable = SoAd_SoRouteDestManagerPtr[socketRouteId].SocketRouteDestEnable;
#endif
                    if (destEnable)
                    {
                        soRouteDestCfgPtr = &SoAd_SoRouteDestCfgPtr[socketRouteId];
                        upModule          = soRouteDestCfgPtr->RefUpModule;
                        if (SOAD_UPPER_LAYER_IF == soRouteDestCfgPtr->RxUpperLayerType)
                        {
                            pduInfo.SduDataPtr  = &DataPtr[dataPosition + SOAD_HEADER_SIZE];
                            pduInfo.SduLength   = pduLength;
                            pduInfo.MetaDataPtr = NULL_PTR;
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
                            uint8 metaData[2u]; /* PRQA S 3132 */ /* VL_QAC_MagicNum */
                            pduInfo.MetaDataPtr =
                                SoAd_MetaDataFillHandle(SoConId, soRouteDestCfgPtr->MetaDataEnable, &metaData[0u]);
#endif
                            /* maybe Multiple header for one udp packet */
                            if ((remainLen + SOAD_HEADER_SIZE + pduLength) <= RxMessageLength)
                            {
                                soConMgmtPtr->RxIfProcessing = TRUE;
                            }
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
                            SoAd_UpperLayerPCCfgPtr[upModule].IfRxIndicationFunc(
                                soRouteDestCfgPtr->UpRxPduId,
                                &pduInfo);
#endif
                            dataPosition = remainLen;
                        }
                        else
                        {
#if (STD_ON == SOAD_SUPPORT_RX_BUFFER)
                            validPduData = SoAd_HeaderEnableUdpSoConRxTpPduHandle(
                                SoConId,
                                soConMgmtPtr,
                                socketRouteId,
                                &dataPosition,
                                DataPtr);
#endif
                        }
                    }
                    else
                    {
                        /* just for break */
                        dataPosition = RxMessageLength;
                    }
                }
                else
                {
                    dataPosition = remainLen;
                    (void)Det_ReportRuntimeError(
                        SOAD_MODULE_ID,
                        SOAD_INSTANCE,
                        SOAD_SID_RXINDICATION,
                        SOAD_E_INV_PDUHEADER_ID);
                }
            }
            else
            {
                break;
            }
        }
        if (soConMgmtPtr->RxIfProcessing)
        {
            soConMgmtPtr->RxIfProcessing = FALSE;
        }
    }
    else
    {
        validPduData = FALSE;
    }

    return validPduData;
}
#endif
#endif

#if (STD_ON == SOAD_SUPPORT_RX_BUFFER)
#if (STD_ON == SOAD_SUPPORT_HEADERID)
/**
 * @brief         Handles the reception of a TCP socket connection for a TP PDU.
 */
/* PRQA S 6030 ++ */ /* VL_MTR_SoAd_STMIF */
SOAD_LOCAL void SoAd_HeaderEnableTcpSoConRxTpPduHandle(
    SoAd_SoConIdType               SoConId,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    SoAd_uintx                     SocketRouteId,
    PduLengthType                  RxMessageLength,
    uint8*                         DataPtr)
/* PRQA S 6030 -- */
{
    PduInfoType                     pduInfo;
    const SoAd_SocketRouteDestType* soRouteDestCfgPtr = &SoAd_SoRouteDestCfgPtr[SocketRouteId];
    PduLengthType                   pduLength         = (PduLengthType)SOAD_GET_LENGTH(DataPtr, 0u);
    SoAd_uintx                      bufferId          = SoAd_SoConCfgPtr[SoConId].RxBuffCfgIndexPtr[0];
    PduLengthType                   bufferLength      = SoAd_SocketBufferCfgPtr[bufferId].BuffLen;
    bufferId                                          = bufferId - SoAd_TxBufferNum;

    pduInfo.SduDataPtr  = NULL_PTR;
    pduInfo.SduLength   = 0u;
    pduInfo.MetaDataPtr = NULL_PTR;
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
    uint8 metaData[2u]; /* PRQA S 3132 */ /* VL_QAC_MagicNum */
    pduInfo.MetaDataPtr = SoAd_MetaDataFillHandle(SoConId, soRouteDestCfgPtr->MetaDataEnable, &metaData[0u]);
#endif

#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
    PduLengthType          upBufferSize = 0u;
    PduIdType              upPduId      = soRouteDestCfgPtr->UpRxPduId;
    SoAd_UpLayerModuleType upModule     = soRouteDestCfgPtr->RefUpModule;

    if (BUFREQ_OK
        == SoAd_UpperLayerPCCfgPtr[upModule].TpStartOfReceptionFunc(upPduId, &pduInfo, pduLength, &upBufferSize))
    {
        PduLengthType rxDataLength = RxMessageLength - SOAD_HEADER_SIZE;
        /*if not all data can be processed*/
        if ((bufferLength + upBufferSize) < rxDataLength)
        {
            SoAd_UpperLayerPCCfgPtr[upModule].TpRxindicationFunc(upPduId, E_NOT_OK);
            soConMgmtPtr->SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
            soConMgmtPtr->SetSocketNeedCloseLine = __LINE__;
#endif
        }
        else
        {
#if (SOAD_PARTITION_NUM > 1u)
            uint16 partIndex = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(SoConId)].PartitionId);
            SoAd_uintx              startId         = bufferId - SOAD_MULTI_PART_INFO.SocketRxBuffStartId;
            SoAd_SoRxBufferManType* soRxBuffMgmtPtr = &SoAd_SoRxBuffManagerPtr[partIndex][startId];
#else
            SoAd_SoRxBufferManType* soRxBuffMgmtPtr = &SoAd_SoRxBuffManagerPtr[bufferId];
#endif
            /*the message's data belong to more than one PDU*/
            if (pduLength <= rxDataLength)
            {
                SoAd_RxFullTpPduHandleType parameter;

                parameter.DataPtr       = DataPtr;
                parameter.PduLength     = pduLength;
                parameter.RxDataLength  = RxMessageLength;
                parameter.SoConId       = SoConId;
                parameter.SocketRouteId = SocketRouteId;
                parameter.UpBufferSize  = upBufferSize;
                parameter.UpModuleId    = upModule;
                parameter.RxBufferId    = bufferId;
                parameter.TcpIpSocketId = soConMgmtPtr->TcpIpSocketId;
                SoAd_RxFullTpPduHandle(&parameter, soConMgmtPtr, soRxBuffMgmtPtr);
            }
            /*the message's all data is part of the PDU*/
            else
            {
                PduLengthType copyToUpLength = upBufferSize;

                if (upBufferSize > rxDataLength)
                {
                    copyToUpLength = rxDataLength;
                }
                if (0u < copyToUpLength)
                {
                    pduInfo.SduDataPtr = &DataPtr[SOAD_HEADER_SIZE];
                    pduInfo.SduLength  = copyToUpLength;
                    if (BUFREQ_OK
                        == SoAd_UpperLayerPCCfgPtr[upModule].TpCopyRxDataFunc(upPduId, &pduInfo, &upBufferSize))
                    {
                        SoAd_UpdateTcpReceived(SoConId, soConMgmtPtr->TcpIpSocketId, copyToUpLength);
                        /*copy the remainder data(RxMessageLength-8-copyToUpLength) to SoAd Rx Buffer*/
                        if (SoAd_CopyDataToRxBuffer(
                                SoConId,
                                (rxDataLength - copyToUpLength),
                                &DataPtr[SOAD_HEADER_SIZE + copyToUpLength],
                                soConMgmtPtr,
                                soRxBuffMgmtPtr))
                        {
                            soRxBuffMgmtPtr->ExitPduResidueLength = (pduLength - copyToUpLength);
                            soRxBuffMgmtPtr->SocketRouteId        = SocketRouteId;
                        }
                    }
                    else
                    {
                        SoAd_UpperLayerPCCfgPtr[upModule].TpRxindicationFunc(upPduId, E_NOT_OK);
                        soConMgmtPtr->SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
                        soConMgmtPtr->SetSocketNeedCloseLine = __LINE__;
#endif
                    }
                }
                else
                {
                    /*copy the remainder data(RxMessageLength-8) to SoAd Rx Buffer*/
                    if (SoAd_CopyDataToRxBuffer(
                            SoConId,
                            rxDataLength,
                            &DataPtr[SOAD_HEADER_SIZE],
                            soConMgmtPtr,
                            soRxBuffMgmtPtr))
                    {
                        soRxBuffMgmtPtr->ExitPduResidueLength = pduLength;
                        soRxBuffMgmtPtr->SocketRouteId        = SocketRouteId;
                    }
                }
            }
        }
    }
    else
    {
        soConMgmtPtr->SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
        soConMgmtPtr->SetSocketNeedCloseLine = __LINE__;
#endif
    }
#endif
}

/**
 * @brief         Handles the full TP PDU reception.
 */
SOAD_LOCAL void SoAd_RxFullTpPduHandle(
    const SoAd_RxFullTpPduHandleType* Parameter,
    SoAd_InnerSocketConManageType*    soConMgmtPtr,
    SoAd_SoRxBufferManType*           soRxBuffMgmtPtr)
{
    PduInfoType                     pduInfo;
    SoAd_UpLayerModuleType          upModule          = Parameter->UpModuleId;
    uint8*                          DataPtr           = Parameter->DataPtr;
    PduLengthType                   upBufferSize      = Parameter->UpBufferSize;
    const SoAd_SocketRouteDestType* soRouteDestCfgPtr = &SoAd_SoRouteDestCfgPtr[Parameter->SocketRouteId];
    PduIdType                       upPduId           = soRouteDestCfgPtr->UpRxPduId;

    pduInfo.MetaDataPtr = NULL_PTR;
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
    uint8 metaData[2u]; /* PRQA S 3132 */ /* VL_QAC_MagicNum */
    pduInfo.MetaDataPtr = SoAd_MetaDataFillHandle(Parameter->SoConId, soRouteDestCfgPtr->MetaDataEnable, &metaData[0u]);
#endif

    /*the UpBuffer is large enough to copy the total pdu data*/
    Std_ReturnType result        = E_OK;
    PduLengthType  copyLength    = Parameter->RxDataLength - SOAD_HEADER_SIZE;
    PduLengthType  remainLength  = Parameter->PduLength;
    PduLengthType  dataPos       = SOAD_HEADER_SIZE;
    boolean        residueLength = TRUE;

    if (upBufferSize > 0u)
    {
        boolean upSufficientbuffer = upBufferSize >= Parameter->PduLength;
        pduInfo.SduDataPtr         = &DataPtr[SOAD_HEADER_SIZE];
        pduInfo.SduLength          = (upSufficientbuffer) ? Parameter->PduLength : upBufferSize;
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
        if (BUFREQ_OK == SoAd_UpperLayerPCCfgPtr[upModule].TpCopyRxDataFunc(upPduId, &pduInfo, &upBufferSize))
        {
            /*SWS_SoAd_00564:SoAd shall confirm the reception of all data*/
            SoAd_UpdateTcpReceived(Parameter->SoConId, Parameter->TcpIpSocketId, pduInfo.SduLength);

            if (upSufficientbuffer)
            {
                SoAd_UpperLayerPCCfgPtr[upModule].TpRxindicationFunc(upPduId, E_OK);
                residueLength = FALSE;
            }
            else
            {
                remainLength -= pduInfo.SduLength;
            }
            copyLength -= pduInfo.SduLength;
            dataPos += pduInfo.SduLength;
        }
        else
        {
            SoAd_UpperLayerPCCfgPtr[upModule].TpRxindicationFunc(upPduId, E_NOT_OK);
            soConMgmtPtr->SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
            soConMgmtPtr->SetSocketNeedCloseLine = __LINE__;
#endif
            result = E_NOT_OK;
        }
#endif
    }

    if (E_OK == result)
    {
        /*copy the remainder data(include the next pdu's header and data) to SoAd Rx Buffer*/
        boolean ret =
            SoAd_CopyDataToRxBuffer(Parameter->SoConId, copyLength, &DataPtr[dataPos], soConMgmtPtr, soRxBuffMgmtPtr);
        if ((residueLength) && (ret))
        {
            soRxBuffMgmtPtr->ExitPduResidueLength = remainLength;
            soRxBuffMgmtPtr->SocketRouteId        = Parameter->SocketRouteId;
        }
    }
}
#endif

/**
 * @brief     Handle received TP PDU with disabled header
 */
SOAD_LOCAL void SoAd_HeaderDisableSoConRxTpPduHandle(
    SoAd_SoConIdType               SoConId,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    SoAd_uintx                     SocketRouteId,
    PduLengthType                  RxMessageLength,
    uint8*                         DataPtr)
{
    SoAd_uintx bufferId = SoAd_SoConCfgPtr[SoConId].RxBuffCfgIndexPtr[0] - SoAd_TxBufferNum;
#if (SOAD_PARTITION_NUM > 1u)
    uint16     partIndex = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(SoConId)].PartitionId);
    SoAd_uintx id        = bufferId - SOAD_MULTI_PART_INFO.SocketRxBuffStartId;
    SoAd_SoRxBufferManType* soRxBuffMgmtPtr = &SoAd_SoRxBuffManagerPtr[partIndex][id];
#else
    SoAd_SoRxBufferManType* soRxBuffMgmtPtr = &SoAd_SoRxBuffManagerPtr[bufferId];
#endif
    /** SWS_SoAd_00568:
     * if the SoAd receive buffer does not contain any TP data for this socket connection
     */
    if (soRxBuffMgmtPtr->ExitPosition == soRxBuffMgmtPtr->EnterPosition)
    {
        PduInfoType pduInfo;
        pduInfo.SduDataPtr  = NULL_PTR;
        pduInfo.SduLength   = 0u;
        pduInfo.MetaDataPtr = NULL_PTR;
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
        uint8 metaData[2u]; /* PRQA S 3132 */ /* VL_QAC_MagicNum */
        pduInfo.MetaDataPtr =
            SoAd_MetaDataFillHandle(SoConId, SoAd_SoRouteDestCfgPtr[SocketRouteId].MetaDataEnable, &metaData[0u]);
#endif

#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
        PduLengthType          upBufferSize;
        PduIdType              upPduId  = SoAd_SoRouteDestCfgPtr[SocketRouteId].UpRxPduId;
        SoAd_UpLayerModuleType upModule = SoAd_SoRouteDestCfgPtr[SocketRouteId].RefUpModule;

        if (BUFREQ_OK == SoAd_UpperLayerPCCfgPtr[upModule].TpCopyRxDataFunc(upPduId, &pduInfo, &upBufferSize))
        {
            if (upBufferSize >= RxMessageLength)
            {
                pduInfo.SduLength = RxMessageLength;
            }
            else
            {
                pduInfo.SduLength = upBufferSize;
            }
            pduInfo.SduDataPtr = DataPtr;
            if (BUFREQ_OK == SoAd_UpperLayerPCCfgPtr[upModule].TpCopyRxDataFunc(upPduId, &pduInfo, &upBufferSize))
            {
                SoAd_UpdateTcpReceived(SoConId, soConMgmtPtr->TcpIpSocketId, pduInfo.SduLength);
                /*copy data to SoAd Rx Buffer*/
                if (pduInfo.SduLength < RxMessageLength)
                {
                    (void)SoAd_CopyDataToRxBuffer(
                        SoConId,
                        (RxMessageLength - pduInfo.SduLength),
                        &DataPtr[pduInfo.SduLength],
                        soConMgmtPtr,
                        soRxBuffMgmtPtr);
                }
            }
            else
            {
                soConMgmtPtr->SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
                soConMgmtPtr->SetSocketNeedCloseLine = __LINE__;
#endif
            }
        }
        else
        {
            soConMgmtPtr->SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
            soConMgmtPtr->SetSocketNeedCloseLine = __LINE__;
#endif
        }
#endif
    }
    /*SWS_SoAd_00568:
      if the SoAd receive buffer already contains TP data for this socket connection
      and is able to store all (newley) received data*/
    else
    {
        (void)SoAd_CopyDataToRxBuffer(SoConId, RxMessageLength, &DataPtr[0u], soConMgmtPtr, soRxBuffMgmtPtr);
    }
}

#if (STD_ON == SOAD_SUPPORT_HEADERID)

/**
 * @brief     Handle received PDU with enabled header for TCP socket connection
 */
/* PRQA S 6030 ++ */ /* VL_MTR_SoAd_STMIF */
SOAD_LOCAL void SoAd_HeaderEnableTcpSoConRxHandle(
    SoAd_SoConIdType               SoConId,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    PduLengthType                  RxMessageLength,
    uint8*                         DataPtr)
/* PRQA S 6030 -- */
{
    if (NULL_PTR != SoAd_SoConCfgPtr[SoConId].RxBuffCfgIndexPtr)
    {
        SoAd_uintx bufferId = SoAd_SoConCfgPtr[SoConId].RxBuffCfgIndexPtr[0u] - SoAd_TxBufferNum;
        /*used for SoAd_RxBuffer Manage*/

#if (SOAD_PARTITION_NUM > 1u)
        uint16     partIndex = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(SoConId)].PartitionId);
        SoAd_uintx id        = bufferId - SOAD_MULTI_PART_INFO.SocketRxBuffStartId;
        SoAd_SoRxBufferManType* soRxBuffMgmtPtr = &SoAd_SoRxBuffManagerPtr[partIndex][id];
#else
        SoAd_SoRxBufferManType* soRxBuffMgmtPtr = &SoAd_SoRxBuffManagerPtr[bufferId];
#endif
        /*the newly pdu:copy data to up layer,copy residue data to SoAd Rx Buffer*/
        if ((soRxBuffMgmtPtr->EnterPosition == soRxBuffMgmtPtr->ExitPosition)
            && (0u == soRxBuffMgmtPtr->ExitPduResidueLength) && (RxMessageLength >= SOAD_HEADER_SIZE))
        {
            PduLengthType rxDataLength = RxMessageLength - SOAD_HEADER_SIZE;
            uint32        header       = SOAD_GET_HEADER(DataPtr, 0u);
            PduLengthType pduLength    = (PduLengthType)SOAD_GET_LENGTH(DataPtr, 0u);
            SoAd_uintx    socketRouteId;
            boolean       findValidSocketRoute = FALSE;

            findValidSocketRoute = SoAd_FindHeaderIDAndSocketRouteId(SoConId, header, &socketRouteId);
#if (SOAD_PARTITION_NUM > 1u)
            uint16 partIndex = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(SoConId)].PartitionId);
            id               = SoAd_SoRouteDestCfgPtr[socketRouteId].PartitionSocketRouteDestId;
            boolean destEnable = SoAd_SoRouteDestManagerPtr[partIndex][id].SocketRouteDestEnable;
#else
            boolean destEnable = SoAd_SoRouteDestManagerPtr[socketRouteId].SocketRouteDestEnable;
#endif
            /*findValidSocketRoute and socket dest routing group is enabled.*/
            if (destEnable)
            {
                if (findValidSocketRoute)
                {
                    const SoAd_SocketRouteDestType* soRouteDestCfgPtr = &SoAd_SoRouteDestCfgPtr[socketRouteId];
                    SoAd_UpdateTcpReceived(SoConId, soConMgmtPtr->TcpIpSocketId, SOAD_HEADER_SIZE);
                    if (SOAD_UPPER_LAYER_TP == soRouteDestCfgPtr->RxUpperLayerType)
                    {
                        /*Skip further processing if PDU length is 0 and (SoAdPduHeaderEnable is FALSE or
                         * SoAdRxUpperLayerType is TP)*/
                        if (0u < pduLength)
                        {
                            SoAd_HeaderEnableTcpSoConRxTpPduHandle(
                                SoConId,
                                soConMgmtPtr,
                                socketRouteId,
                                RxMessageLength,
                                DataPtr);
                        }
                    }
#if (0u < SOAD_MAX_IF_PDU_SIZE)
                    /*IF PDU Rx handle*/
                    else
                    {
                        /*the message's data belong to more than one PDU*/
                        if (pduLength <= rxDataLength)
                        {
                            PduInfoType pduInfo;
                            pduInfo.SduDataPtr  = &DataPtr[SOAD_HEADER_SIZE];
                            pduInfo.SduLength   = pduLength;
                            pduInfo.MetaDataPtr = NULL_PTR;
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
                            uint8 metaData[2u]; /* PRQA S 3132 */ /* VL_QAC_MagicNum */
                            pduInfo.MetaDataPtr =
                                SoAd_MetaDataFillHandle(SoConId, soRouteDestCfgPtr->MetaDataEnable, &metaData[0u]);

#endif

#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
                            SoAd_UpLayerModuleType upModule = soRouteDestCfgPtr->RefUpModule;
                            PduIdType              upPduId  = soRouteDestCfgPtr->UpRxPduId;
                            SoAd_UpperLayerPCCfgPtr[upModule].IfRxIndicationFunc(upPduId, &pduInfo);
#endif
                            /*SWS_SoAd_00564:SoAd shall confirm the reception of all data*/
                            SoAd_UpdateTcpReceived(SoConId, soConMgmtPtr->TcpIpSocketId, pduLength);
                            /*copy the remainder data(Length-8-pduLength) to SoAd Rx Buffer*/
                            (void)SoAd_CopyDataToRxBuffer(
                                SoConId,
                                (rxDataLength - pduLength),
                                &DataPtr[SOAD_HEADER_SIZE + pduLength],
                                soConMgmtPtr,
                                soRxBuffMgmtPtr);
                        }
                        /*the message is part data of one PDU*/
                        else
                        {
                            /*copy the remainder data(Length-8) to SoAd Rx Buffer*/
                            if (SoAd_CopyDataToRxBuffer(
                                    SoConId,
                                    rxDataLength,
                                    &DataPtr[SOAD_HEADER_SIZE],
                                    soConMgmtPtr,
                                    soRxBuffMgmtPtr))
                            {
                                soRxBuffMgmtPtr->ExitPduResidueLength = pduLength;
                                soRxBuffMgmtPtr->SocketRouteId        = socketRouteId;
                            }
                        }
                    }
#endif
                }
                else
                {
                    if (rxDataLength >= pduLength)
                    {
                        (void)Det_ReportRuntimeError(
                            SOAD_MODULE_ID,
                            SOAD_INSTANCE,
                            SOAD_SID_RXINDICATION,
                            SOAD_E_INV_PDUHEADER_ID);
                        /*discard the all pdu data*/
                        PduLengthType length = pduLength + SOAD_HEADER_SIZE;
                        SoAd_UpdateTcpReceived(SoConId, soConMgmtPtr->TcpIpSocketId, (uint32)(length));
                        (void)SoAd_CopyDataToRxBuffer(
                            SoConId,
                            (rxDataLength - pduLength),
                            &DataPtr[length],
                            soConMgmtPtr,
                            soRxBuffMgmtPtr);
                    }
                    /*need copy the part pdu data to buffer,then discard the all data of the pdu.*/
                    else
                    {
                        (void)SoAd_CopyDataToRxBuffer(SoConId, RxMessageLength, DataPtr, soConMgmtPtr, soRxBuffMgmtPtr);
                    }
                }
            }
        }
        /*copy the total data to SoAd Rx Buffer*/
        else
        {
            (void)SoAd_CopyDataToRxBuffer(SoConId, RxMessageLength, DataPtr, soConMgmtPtr, soRxBuffMgmtPtr);
        }
    }
    else
    {
        (void)Det_ReportRuntimeError(SOAD_MODULE_ID, SOAD_INSTANCE, SOAD_SID_RXINDICATION, SOAD_E_INV_PDUHEADER_ID);
        /*discard the all pdu data*/
        SoAd_UpdateTcpReceived(SoConId, soConMgmtPtr->TcpIpSocketId, (uint32)(RxMessageLength));
    }
}
#endif

/**
 * @brief     Copy received data to SoAd Rx Buffer
 */
SOAD_LOCAL boolean SoAd_CopyDataToRxBuffer(
    SoAd_SoConIdType               SoConId,
    PduLengthType                  RxMessageLength,
    const uint8*                   DataPtr,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    SoAd_SoRxBufferManType*        soRxBuffMgmtPtr)
{
    boolean copyDataOk = TRUE;

    if (0u < RxMessageLength)
    {
        SoAd_uintx    bufferId       = SoAd_SoConCfgPtr[SoConId].RxBuffCfgIndexPtr[0u];
        PduLengthType bufferPosition = SoAd_SocketBufferCfgPtr[bufferId].BuffPos;
        PduLengthType bufferLength   = SoAd_SocketBufferCfgPtr[bufferId].BuffLen;
        /*used for SoAd_RxBuffer Manage*/
        PduLengthType srcPos   = soRxBuffMgmtPtr->EnterPosition;
        PduLengthType destPos  = srcPos + RxMessageLength;
        PduLengthType startPos = bufferPosition + srcPos;

        if (soRxBuffMgmtPtr->ExitPosition <= soRxBuffMgmtPtr->EnterPosition)
        {
            if (destPos >= bufferLength)
            {
                if ((destPos - bufferLength) < soRxBuffMgmtPtr->ExitPosition)
                {
                    PduLengthType copyLength = bufferLength - srcPos;
                    (void)IStdLib_MemCpy(&SoAd_PduBuffer[startPos], DataPtr, copyLength);
                    (void)IStdLib_MemCpy(
                        &SoAd_PduBuffer[bufferPosition],
                        &DataPtr[copyLength],
                        (PduLengthType)(destPos - bufferLength));
                    soRxBuffMgmtPtr->EnterPosition = destPos - bufferLength;
                }
                /*SWS_SoAd_00693:larger than the remaining available buffer size */
                else
                {
                    copyDataOk = FALSE;
                }
            }
            else
            {
                (void)IStdLib_MemCpy(&SoAd_PduBuffer[startPos], DataPtr, RxMessageLength);
                soRxBuffMgmtPtr->EnterPosition = destPos;
            }
        }
        else
        {
            if (destPos < soRxBuffMgmtPtr->ExitPosition)
            {
                (void)IStdLib_MemCpy(&SoAd_PduBuffer[startPos], DataPtr, RxMessageLength);
                soRxBuffMgmtPtr->EnterPosition = destPos;
            }
            else
            {
                copyDataOk = FALSE;
            }
        }
        if (!copyDataOk)
        {
            soConMgmtPtr->SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
            soConMgmtPtr->SetSocketNeedCloseLine = __LINE__;
#endif
            (void)Det_ReportRuntimeError(SOAD_MODULE_ID, SOAD_INSTANCE, SOAD_SID_RXINDICATION, SOAD_E_NOBUFS);
        }
    }
    return copyDataOk;
}
#endif

/**
 * @brief     Handle received IF PDU with disabled header for socket connection
 */
SOAD_LOCAL void SoAd_HeaderDisableSoConRxIfPduHandle(
    SoAd_SoConIdType SoConId,
    SoAd_uintx       SocketRouteId,
    PduLengthType    RxMessageLength,
    uint8*           DataPtr)
{
    PduInfoType pduInfo;

    pduInfo.SduDataPtr  = DataPtr;
    pduInfo.SduLength   = RxMessageLength;
    pduInfo.MetaDataPtr = NULL_PTR;
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
    uint8 metaData[2u]; /* PRQA S 3132 */ /* VL_QAC_MagicNum */
    pduInfo.MetaDataPtr =
        SoAd_MetaDataFillHandle(SoConId, SoAd_SoRouteDestCfgPtr[SocketRouteId].MetaDataEnable, &metaData[0u]);
#endif

#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
    SoAd_UpLayerModuleType upModule = SoAd_SoRouteDestCfgPtr[SocketRouteId].RefUpModule;

    SoAd_UpperLayerPCCfgPtr[upModule].IfRxIndicationFunc(SoAd_SoRouteDestCfgPtr[SocketRouteId].UpRxPduId, &pduInfo);

#endif

    if (SOAD_TCP == SoAd_SoConCfgPtr[SoConId].TcpUdpProtocol)
    {
        /*SWS_SoAd_00564:SoAd shall confirm the reception of all data*/
#if (SOAD_PARTITION_NUM > 1u)
        uint16 partIndex    = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(SoConId)].PartitionId);
        SoAd_SoConIdType id = SoAd_SoConCfgPtr[SoConId].PartitionSoConId;
        SoAd_UpdateTcpReceived(SoConId, SoAd_SoConManagerPtr[partIndex][id].TcpIpSocketId, RxMessageLength);
#else
        SoAd_UpdateTcpReceived(SoConId, SoAd_SoConManagerPtr[SoConId].TcpIpSocketId, RxMessageLength);
#endif
    }
}
#endif

#if (0u < SOAD_SOCKET_ROUTE_DEST_MAX)

/**
 * @brief         Finds the valid socket connection based on the socket ID and remote address.
 */
SOAD_LOCAL boolean SoAd_FindTheValidSoCon(
    TcpIp_SocketIdType        SocketId,
    const TcpIp_SockAddrType* RemoteAddrPtr,
#if (STD_ON == SOAD_SUPPORT_UDP)
    SoAd_FindTheValidSoConType* UdpParaPtr,
#endif
    SoAd_SoConIdType* SoConId)
{
    boolean                                   socketIdValid = FALSE;
    const SoAd_InnerSocketConManageType*      soConMgmtPtr;
    const SoAd_InnerSocketConGroupManageType* soConGroupMgmtPtr;
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION) && (SOAD_PARTITION_NUM > 1u)
    ApplicationType curAppId = GetApplicationID();
#endif
#if (STD_ON == SOAD_SUPPORT_UDP)
    const SoAd_SocketUdpType* soUdpCfgPtr;
    UdpParaPtr->RemoteAddrChanged = FALSE;

    /*find a udp socket connection*/
    for (SoAd_uintx groupId = 0u; (groupId < SOAD_SO_CON_GROUP_NUM) && (!socketIdValid); groupId++)
    {
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION) && (SOAD_PARTITION_NUM > 1u)
        uint16 partitionIdCfg = SoAd_SoConGroupCfgPtr[groupId].PartitionId;
        if (partitionIdCfg != curAppId)
        {
            continue;
        }
        uint16 partIndex  = SoAd_GetPartitionIndex(partitionIdCfg);
        soConGroupMgmtPtr = &SoAd_SoConGroupManagerPtr[partIndex][SoAd_SoConGroupCfgPtr[groupId].PartitionSoConGroupId];
#else
        soConGroupMgmtPtr = &SoAd_SoConGroupManagerPtr[groupId];
#endif
        soUdpCfgPtr = SOAD_UDPTYPE_CFG(groupId);
        if (soConGroupMgmtPtr->TcpIpSocketValid && (SocketId == soConGroupMgmtPtr->TcpIpSocketId)
            && (NULL_PTR != soUdpCfgPtr))
        {
            socketIdValid = SoAd_FindUdpValidSoCon(groupId, RemoteAddrPtr, UdpParaPtr, SoConId);
        }
    }

#endif

#if (STD_ON == SOAD_SUPPORT_TCP)
    SoAd_uintx soConGroupId;

    /*find a tcp socket connection*/
    for (SoAd_SoConIdType soConCnt = 0u; (soConCnt < SoAd_SoConNum) && (!socketIdValid); soConCnt++)
    {
        soConGroupId = SOAD_SOCON_REFGORUP(soConCnt);
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION) && (SOAD_PARTITION_NUM > 1u)
        uint16 partitionIdCfg = SoAd_SoConGroupCfgPtr[soConGroupId].PartitionId;
        if (partitionIdCfg != curAppId)
        {
            continue;
        }
        uint16 partIndex = SoAd_GetPartitionIndex(partitionIdCfg);
        soConMgmtPtr     = &SoAd_SoConManagerPtr[partIndex][SoAd_SoConCfgPtr[soConCnt].PartitionSoConId];
#else
        soConMgmtPtr = &SoAd_SoConManagerPtr[soConCnt];
#endif

#if (SOAD_PARTITION_NUM > 1u)
        soConGroupMgmtPtr =
            &SoAd_SoConGroupManagerPtr[partIndex][SoAd_SoConGroupCfgPtr[soConGroupId].PartitionSoConGroupId];
#else
        soConGroupMgmtPtr = &SoAd_SoConGroupManagerPtr[soConGroupId];
#endif
        if ((!soConGroupMgmtPtr->TcpIpSocketValid || (SocketId != soConGroupMgmtPtr->TcpIpSocketId))
            && soConMgmtPtr->TcpIpSocketValid && (SocketId == soConMgmtPtr->TcpIpSocketId))
        {
            if (SoAd_SoConGroupCfgPtr[soConGroupId].SocketMsgAcceptanceFilterEnabled)
            {
                if ((soConMgmtPtr->RemoteAddr.domain == RemoteAddrPtr->domain)
                    && (SOAD_IS_IPADDR_ANY(soConMgmtPtr->RemoteAddr)
                        || (SOAD_EQ_IPADDR(soConMgmtPtr->RemoteAddr, *RemoteAddrPtr)))
                    && ((soConMgmtPtr->RemoteAddr.port == TCPIP_PORT_ANY)
                        || (soConMgmtPtr->RemoteAddr.port == RemoteAddrPtr->port)))
                {
                    *SoConId = soConCnt;
                }
            }
            else
            {
                *SoConId = soConCnt;
            }
            socketIdValid = TRUE;
        }
    }

#endif

    return socketIdValid;
}
#endif

#if (STD_ON == SOAD_SUPPORT_UDP)
#if (STD_ON == SOAD_SUPPORT_HEADERID)
/**
 * @brief         Performs a strict header length check for UDP.
 */
SOAD_LOCAL boolean SoAd_UdpStrictHeaderLenCheck(SoAd_uintx SoConGroupId, const uint8* BufPtr, uint16 Length)
{
    uint32  dataPosition = 0u;
    uint32  pduLength;
    boolean udpStrictHeaderLenCheck = FALSE;
    if (Length >= SOAD_HEADER_SIZE)
    {
        if (SOAD_UDPTYPE_CFG(SoConGroupId)->SocketUdpStrictHeaderLenCheckEnabled)
        {
            while (dataPosition < Length)
            {
                pduLength    = SOAD_GET_LENGTH(BufPtr, dataPosition);
                dataPosition = dataPosition + SOAD_HEADER_DATA_SIZE + SOAD_HEADER_LENGTH_SIZE + pduLength;
                if (dataPosition == Length)
                {
                    udpStrictHeaderLenCheck = TRUE;
                }
            }
        }
        else
        {
            udpStrictHeaderLenCheck = TRUE;
        }
    }
    return udpStrictHeaderLenCheck;
}
#endif

/**
 * @brief         Handles the UDP alive supervision time for a socket connection.
 */
SOAD_LOCAL void SoAd_UdpAliveSupervisionTimeHandle(SoAd_SoConIdType SoConId)
{
    SoAd_InnerSocketConManageType* soConMgmtPtr;
#if (SOAD_PARTITION_NUM > 1u)
    uint16 partIndex = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(SoConId)].PartitionId);
    soConMgmtPtr     = &SoAd_SoConManagerPtr[partIndex][SoAd_SoConCfgPtr[SoConId].PartitionSoConId];
#else
    soConMgmtPtr = &SoAd_SoConManagerPtr[SoConId];
#endif

    if (0uL < soConMgmtPtr->UdpAliveSupervisionTime)
    {
        soConMgmtPtr->UdpAliveSupervisionTime -= 1uL;
        if (0uL == soConMgmtPtr->UdpAliveSupervisionTime)
        {
            /*SWS_SoAd_00695:alive supervision timer runs out*/
            soConMgmtPtr->SoConMode = SOAD_SOCON_RECONNECT;
            SoAd_uintx soConGroup   = SOAD_SOCON_REFGORUP(SoConId);
            SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, soConMgmtPtr, soConGroup);
            if (NULL_PTR != SOAD_SOCONREMOTEADDR_CFG(SoConId))
            {
                soConMgmtPtr->RemoteAddr = *SOAD_SOCONREMOTEADDR_CFG(SoConId);
            }
        }
    }
}
#endif

/**
 * @brief         Finds the best match algorithm socket connection.
 */
/* PRQA S 6030 ++ */ /* VL_MTR_SoAd_STMIF */
SOAD_LOCAL boolean SoAd_GetBestMatchAlgorithmSoCon(
    SoAd_uintx                SoConGroupId,
    const TcpIp_SockAddrType* RemoteAddrPtr,
    SoAd_SoConIdType*         SoConId,
    SoAd_BestMatchStateType   CheckSoConState)
/* PRQA S 6030 -- */
{
    uint8                                 priority         = 0u;
    const SoAd_SocketConnectionGroupType* soConGroupCfgPtr = &SoAd_SoConGroupCfgPtr[SoConGroupId];
    SoAd_SoConIdType                      socketConNum     = soConGroupCfgPtr->SocketConNum;
    const SoAd_InnerSocketConManageType*  soConMgmtPtr;
    SoAd_SoConIdType                      soConStartId = soConGroupCfgPtr->SocketConStartId;

#if (SOAD_PARTITION_NUM > 1u)
    uint16 partIndex = SoAd_GetPartitionIndex(soConGroupCfgPtr->PartitionId);
    soConMgmtPtr     = SoAd_SoConManagerPtr[partIndex];
#endif

    /*Best Match Algorithm*/
    for (SoAd_SoConIdType cnt = 0u; (cnt < socketConNum) && (SOAD_BEST_MATCH_ALG_PRIORITY_4 != priority); cnt++)
    {
        SoAd_SoConIdType soConIndex = soConStartId + cnt;
#if (SOAD_PARTITION_NUM > 1u)
        soConMgmtPtr = soConMgmtPtr + SoAd_SoConCfgPtr[soConIndex].PartitionSoConId;
#else
                soConMgmtPtr = &SoAd_SoConManagerPtr[soConIndex];
#endif
        if ((SOAD_SOCON_MATCH_REQUEST_NONE == CheckSoConState)
            || ((SOAD_SOCON_MATCH_REQUEST_RECONNECT == CheckSoConState)
                && (soConMgmtPtr->SoConMode == SOAD_SOCON_RECONNECT) && !soConMgmtPtr->TcpIpSocketValid)
            || ((SOAD_SOCON_MATCH_REQUEST_ONLINE_RECONNECT == CheckSoConState)
                && (soConMgmtPtr->SoConMode != SOAD_SOCON_OFFLINE)))
        {
            uint8 currentPrio;

            if (SOAD_EQ_IPADDR(*RemoteAddrPtr, soConMgmtPtr->RemoteAddr)
                && (RemoteAddrPtr->port == soConMgmtPtr->RemoteAddr.port))
            {
                currentPrio = SOAD_BEST_MATCH_ALG_PRIORITY_4;
            }
            else if (
                SOAD_EQ_IPADDR(*RemoteAddrPtr, soConMgmtPtr->RemoteAddr)
                && (TCPIP_PORT_ANY == soConMgmtPtr->RemoteAddr.port))
            {
                currentPrio = SOAD_BEST_MATCH_ALG_PRIORITY_3;
            }
            else if (
                SOAD_IS_IPADDR_ANY(soConMgmtPtr->RemoteAddr) && (RemoteAddrPtr->port == soConMgmtPtr->RemoteAddr.port))
            {
                currentPrio = SOAD_BEST_MATCH_ALG_PRIORITY_2;
            }
            else if (SOAD_IS_IPADDR_ANY(soConMgmtPtr->RemoteAddr) && (TCPIP_PORT_ANY == soConMgmtPtr->RemoteAddr.port))
            {
                currentPrio = SOAD_BEST_MATCH_ALG_PRIORITY_1;
            }
            else
            {
                currentPrio = 0u;
            }

            if (currentPrio > priority)
            {
                *SoConId = soConIndex;
                priority = currentPrio;
            }
        }
    }

    return ((priority > 0u) && (priority <= SOAD_BEST_MATCH_ALG_PRIORITY_4));
}

/**
 * @brief     Change socket parameters
 */
SOAD_LOCAL void SoAd_ChangeSocketParameter(TcpIp_SocketIdType SocketId, SoAd_uintx SoConGroup)
{
    const SoAd_SocketConnectionGroupType* soConGroupCfgPtr = &SoAd_SoConGroupCfgPtr[SoConGroup];

    /*In case of a TCP/UDP socket*/
    if (SOAD_UNUSED_UINT8 != soConGroupCfgPtr->SocketFramePriority)
    {
        (void)TcpIp_ChangeParameter(SocketId, TCPIP_PARAMID_FRAMEPRIO, &soConGroupCfgPtr->SocketFramePriority);
    }

    if (SOAD_UNUSED_UINT8 != soConGroupCfgPtr->SocketDifferentiatedServicesField)
    {
        (void)TcpIp_ChangeParameter(SocketId, TCPIP_PARAMID_DSCP, &soConGroupCfgPtr->SocketDifferentiatedServicesField);
    }

    if (SOAD_UNUSED_UINT32 != soConGroupCfgPtr->SocketFlowLabel)
    {
        uint8 para[4u]; /* PRQA S 3132 */ /* VL_QAC_MagicNum */
        SOAD_TCPIP_CHG_PARA_ENCODE_32(para, soConGroupCfgPtr->SocketFlowLabel);
        (void)TcpIp_ChangeParameter(SocketId, TCPIP_PARAMID_FLOWLABEL, para);
    }

    if (SOAD_UNUSED_UINT8 != soConGroupCfgPtr->SocketPathMTUEnable)
    {
        (void)TcpIp_ChangeParameter(SocketId, TCPIP_PARAMID_PATHMTU_ENABLE, &soConGroupCfgPtr->SocketPathMTUEnable);
    }

#if (SOAD_SOCKET_VLAN == STD_ON)
    if (SOAD_UNUSED_UINT32 != soConGroupCfgPtr->SocketVlan)
    {
        uint8 para[4u]; /* PRQA S 3132 */ /* VL_QAC_MagicNum */
        SOAD_TCPIP_CHG_PARA_ENCODE_32(para, soConGroupCfgPtr->SocketVlan);
        (void)TcpIp_ChangeParameter(SocketId, TCPIP_PARAMID_SOCKET_VLAN, para);
    }
#endif
}
#if (STD_ON == SOAD_SUPPORT_TCP)
/**
 * @brief     Change TCP socket parameters
 */
/* PRQA S 6030,6080 ++ */ /* VL_MTR_SoAd_STMIF,VL_MTR_SoAd_STPTH */
SOAD_LOCAL void SoAd_ChangeTcpSocketParameter(TcpIp_SocketIdType SocketId, SoAd_uintx SoConGroup)
/* PRQA S 6030,6080 -- */
{
    /*In case of a TCP socket*/
    const SoAd_SocketConnectionGroupType* soConGroupCfgPtr = &SoAd_SoConGroupCfgPtr[SoConGroup];
    const SoAd_SocketTcpType*             soTcpCfgPtr      = SOAD_TCPTYPE_CFG(SoConGroup);

    if (SOAD_UNUSED_UINT16 != soConGroupCfgPtr->SocketTpRxBufferMin)
    {
        uint8 para[2u]; /* PRQA S 3132 */ /* VL_QAC_MagicNum */
        SOAD_TCPIP_CHG_PARA_ENCODE_16(para, soConGroupCfgPtr->SocketTpRxBufferMin);
        (void)TcpIp_ChangeParameter(SocketId, TCPIP_PARAMID_TCP_RXWND_MAX, para);
    }

    if (SOAD_UNUSED_UINT8 != soTcpCfgPtr->SocketTcpNoDelay)
    {
        uint8 delayVlu = 0x01u;
        if (soTcpCfgPtr->SocketTcpNoDelay == 0x01u)
        {
            delayVlu = 0x0u;
        }
        (void)TcpIp_ChangeParameter(SocketId, TCPIP_PARAMID_TCP_NAGLE, &delayVlu);
    }

    {
        uint8 tcpKeepAlive = (uint8)SoAd_CalculateU16ValueByCondition((soTcpCfgPtr->SocketTcpKeepAlive), 1u, 0u);
        (void)TcpIp_ChangeParameter(SocketId, TCPIP_PARAMID_TCP_KEEPALIVE, &tcpKeepAlive);
    }

    if (SOAD_UNUSED_UINT32 != soTcpCfgPtr->SocketTcpKeepAliveTime)
    {
        uint8 para[4u]; /* PRQA S 3132 */ /* VL_QAC_MagicNum */
        SOAD_TCPIP_CHG_PARA_ENCODE_32(para, soTcpCfgPtr->SocketTcpKeepAliveTime);
        (void)TcpIp_ChangeParameter(SocketId, TCPIP_PARAMID_TCP_KEEPALIVE_TIME, para);
    }

    if (SOAD_UNUSED_UINT16 != soTcpCfgPtr->SocketTcpKeepAliveProbesMax)
    {
        uint8 para[2u]; /* PRQA S 3132 */ /* VL_QAC_MagicNum */
        SOAD_TCPIP_CHG_PARA_ENCODE_16(para, soTcpCfgPtr->SocketTcpKeepAliveProbesMax);
        (void)TcpIp_ChangeParameter(SocketId, TCPIP_PARAMID_TCP_KEEPALIVE_PROBES_MAX, para);
    }

    if (SOAD_UNUSED_UINT32 != soTcpCfgPtr->SocketTcpKeepAliveInterval)
    {
        uint8 para[4u]; /* PRQA S 3132 */ /* VL_QAC_MagicNum */
        SOAD_TCPIP_CHG_PARA_ENCODE_32(para, soTcpCfgPtr->SocketTcpKeepAliveInterval);
        (void)TcpIp_ChangeParameter(SocketId, TCPIP_PARAMID_TCP_KEEPALIVE_INTERVAL, para);
    }

    if (SOAD_UNUSED_UINT8 != soTcpCfgPtr->SocketTCPOptionFilterId)
    {
        (void)TcpIp_ChangeParameter(SocketId, TCPIP_PARAMID_TCP_OPTIONFILTER, &soTcpCfgPtr->SocketTCPOptionFilterId);
    }

    if (SOAD_UNUSED_UINT16 != soTcpCfgPtr->SocketTcpTlsConnectionId)
    {
        uint8 para[2u]; /* PRQA S 3132 */ /* VL_QAC_MagicNum */
        SOAD_TCPIP_CHG_PARA_ENCODE_16(para, soTcpCfgPtr->SocketTcpTlsConnectionId);
        (void)TcpIp_ChangeParameter(SocketId, TCPIP_PARAMID_TLS_CONNECTION_ASSIGNMENT, para);
    }
}
#endif
#if (STD_ON == SOAD_SUPPORT_UDP)

/**
 * @brief     Open UDP socket connection
 */
SOAD_LOCAL void SoAd_OpenUdpSoConHandle(
    SoAd_SoConIdType                    SoConId,
    SoAd_InnerSocketConManageType*      soConMgmtPtr,
    SoAd_uintx                          SoConGroup,
    SoAd_InnerSocketConGroupManageType* soConGroupMgmtPtr)
{
    TcpIp_SocketIdType socketId;

    if (E_OK == TcpIp_SoAdGetSocket(soConMgmtPtr->RemoteAddr.domain, SOAD_UDP, &socketId))
    {
        /*Change the socket specific parameters according to [SWS_SoAd_00689]*/
        SoAd_ChangeSocketParameter(socketId, SoConGroup);
        /*init the SoAd_SoConManager parameter*/
        soConMgmtPtr->TcpIpSocketValid         = TRUE;
        soConMgmtPtr->TcpIpSocketId            = socketId;
        soConGroupMgmtPtr->TcpIpSocketValid    = TRUE;
        soConGroupMgmtPtr->TcpIpSocketId       = socketId;
        soConGroupMgmtPtr->SoConSocketValidNum = 1u;
        /*Bind the socket to the local address and port*/
        uint16 localPort = SoAd_SoConGroupCfgPtr[SoConGroup].SocketLocalPort;
        if (E_OK == TcpIp_Bind(socketId, SOAD_SOCKETLOCALADDR_CFG(SoConGroup).LocalAddrId, &localPort))
        {
            /*SWS_SoAd_00591*/
            if (SOAD_UDPTYPE_CFG(SoConGroup)->SocketUdpListenOnly
                || ((!SOAD_IS_IPADDR_ANY(soConMgmtPtr->RemoteAddr))
                    && (TCPIP_PORT_ANY != soConMgmtPtr->RemoteAddr.port)))
            {
                if (SOAD_SOCON_ONLINE != soConMgmtPtr->SoConMode)
                {
                    soConMgmtPtr->SoConMode = SOAD_SOCON_ONLINE;
                    SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, soConMgmtPtr, SoConGroup);
                }
            }
            /*SWS_SoAd_00686*/
            else
            {
                if (SOAD_SOCON_RECONNECT != soConMgmtPtr->SoConMode)
                {
                    soConMgmtPtr->SoConMode = SOAD_SOCON_RECONNECT;
                    SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, soConMgmtPtr, SoConGroup);
                }
            }
        }
        else
        {
            /*don't care the return value*/
            (void)TcpIp_Close(socketId, TRUE);
        }
    }
}
#endif

/**
 * @brief     Notify socket connection mode change and TP start reception
 */
SOAD_LOCAL void SoAd_SoConModeChgNotificationAndTpStartOfReception(
    SoAd_SoConIdType               SoConId,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    SoAd_uintx                     SoConGroup)
{
    SoAd_UpLayerModuleType           upModule;
    const SoAd_SocketConnectionType* soConCfgPtr = &SoAd_SoConCfgPtr[SoConId];

    /*if SoAdSocketSoConModeChgNotification is set to TRUE,call <Up>_SoConModeChg()*/
    if (SoAd_SoConGroupCfgPtr[SoConGroup].SocketSoConModeChgNotification)
    {
        uint8 refUpNum = soConCfgPtr->RefUpNum;
        for (uint8 cnt0 = 0u; cnt0 < refUpNum; cnt0++)
        {
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
            upModule = soConCfgPtr->RefUpModulePtr[cnt0];

            SoAd_UpperLayerPCCfgPtr[upModule].SoConModeChgFunc(SoConId, soConMgmtPtr->SoConMode);

#endif
        }
    }

    if (SOAD_SOCON_ONLINE == soConMgmtPtr->SoConMode)
    {
        SoAd_uintx refSocketRoutNum = soConCfgPtr->RefSocketRouteNum;
        SoAd_uintx socketRouteId;
        for (SoAd_uintx cnt1 = 0u; cnt1 < refSocketRoutNum; cnt1++)
        {
            socketRouteId = soConCfgPtr->RefSocketRoutePtr[cnt1];
            /*SWS_SoAd_00595:For socket connection with PDU Header mode disabled
              (SoAdPduHeaderEnable = FALSE) and an upper layer with TP-API*/
            if (
#if (STD_ON == SOAD_SUPPORT_HEADERID)
                (NULL_PTR == SoAd_SocketRouteCfgPtr[socketRouteId].RxPduHeaderId) &&
#endif
                (SOAD_UPPER_LAYER_TP == SoAd_SoRouteDestCfgPtr[socketRouteId].RxUpperLayerType))
            {
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
                PduLengthType bufferSize = 0u;
                upModule                 = SoAd_SoRouteDestCfgPtr[socketRouteId].RefUpModule;

                if (BUFREQ_OK
                    != SoAd_UpperLayerPCCfgPtr[upModule].TpStartOfReceptionFunc(
                        SoAd_SoRouteDestCfgPtr[socketRouteId].UpRxPduId,
                        NULL_PTR,
                        0U,
                        &bufferSize))
                {
                    soConMgmtPtr->SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
                    soConMgmtPtr->SetSocketNeedCloseLine = __LINE__;
#endif
                }

#endif
            }
        }
    }
}

/**
 * @brief     Notify socket connection IP address assignment change
 */
SOAD_LOCAL void
    SoAd_SoConLocalIpAddrAssignmentChg(SoAd_SoConIdType SoConId, SoAd_uintx SoConGroup, TcpIp_IpAddrStateType State)
{
    /*if SoAd_SocketIpAddrAssignmentChgNotification is set to TRUE,call <Up>_LocalIpAddrAssignmentChg*/
    if (SoAd_SoConGroupCfgPtr[SoConGroup].SocketIpAddrAssignmentChgNotification)
    {
        const SoAd_SocketConnectionType* soConCfgPtr = &SoAd_SoConCfgPtr[SoConId];
        uint8                            refUpNum    = soConCfgPtr->RefUpNum;

        for (uint8 cnt = 0u; cnt < refUpNum; cnt++)
        {
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
            SoAd_UpLayerModuleType upModule = soConCfgPtr->RefUpModulePtr[cnt];

            SoAd_UpperLayerPCCfgPtr[upModule].LocalIpAddrAssignmentChgFunc(SoConId, State);

#endif
        }
    }
}
#if (STD_ON == SOAD_SUPPORT_TCP)
/**
 * @brief         Opens a TCP server socket connection.
 */
/* PRQA S 6030 ++ */ /* VL_MTR_SoAd_STMIF */
SOAD_LOCAL void SoAd_OpenTcpServerSoConHandle(
    SoAd_SoConIdType                    SoConId,
    SoAd_uintx                          SoConGroup,
    SoAd_InnerSocketConManageType*      soConMgmtPtr,
    SoAd_InnerSocketConGroupManageType* soConGroupMgmtPtr)
/* PRQA S 6030 -- */
{
    if (!soConGroupMgmtPtr->TcpIpSocketValid)
    {
        TcpIp_SocketIdType socketId;

        if (E_OK == TcpIp_SoAdGetSocket(soConMgmtPtr->RemoteAddr.domain, SOAD_TCP, &socketId))
        {
            /*Change the socket specific parameters according to [SWS_SoAd_00689]*/
            SoAd_ChangeSocketParameter(socketId, SoConGroup);
            SoAd_ChangeTcpSocketParameter(socketId, SoConGroup);
            soConGroupMgmtPtr->TcpIpSocketValid    = TRUE;
            soConGroupMgmtPtr->TcpIpSocketId       = socketId;
            soConGroupMgmtPtr->SoConSocketValidNum = 0u;
            /*Bind the socket to the local address and port*/
            uint16         localPort = SoAd_SoConGroupCfgPtr[SoConGroup].SocketLocalPort;
            Std_ReturnType ret = TcpIp_Bind(socketId, SOAD_SOCKETLOCALADDR_CFG(SoConGroup).LocalAddrId, &localPort);
            if (E_OK == ret)
            {
                ret = TcpIp_TcpListen(socketId, SoAd_SoConGroupCfgPtr[SoConGroup].SocketConNum);
                if (E_OK == ret)
                {
                    /*SWS_SoAd_00686*/
                    if (SOAD_SOCON_RECONNECT != soConMgmtPtr->SoConMode)
                    {
                        soConMgmtPtr->SoConMode = SOAD_SOCON_RECONNECT;
                        SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, soConMgmtPtr, SoConGroup);
                    }
                }
            }
            if (E_OK != ret)
            {
                soConGroupMgmtPtr->TcpIpSocketValid = FALSE;
                soConGroupMgmtPtr->TcpIpSocketId    = (TcpIp_SocketIdType)TCPIP_SOCKETID_INVALID;
                /*don't care the return value*/
                (void)TcpIp_Close(socketId, TRUE);
            }
        }
    }
    else
    {
        /*SWS_SoAd_00686:tcp server may have changed to ONLINE before*/
        if (SOAD_SOCON_OFFLINE == soConMgmtPtr->SoConMode)
        {
            soConMgmtPtr->SoConMode = SOAD_SOCON_RECONNECT;
            SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, soConMgmtPtr, SoConGroup);
        }
    }
}

/**
 * @brief         Opens a TCP client socket connection.
 */
SOAD_LOCAL void SoAd_OpenTcpClientSoConHandle(
    SoAd_SoConIdType               SoConId,
    SoAd_uintx                     SoConGroup,
    SoAd_InnerSocketConManageType* soConMgmtPtr)
{
    TcpIp_SocketIdType socketId;

    if (E_OK == TcpIp_SoAdGetSocket(soConMgmtPtr->RemoteAddr.domain, SOAD_TCP, &socketId))
    {
        /*Change the socket specific parameters according to [SWS_SoAd_00689]*/
        SoAd_ChangeSocketParameter(socketId, SoConGroup);
        SoAd_ChangeTcpSocketParameter(socketId, SoConGroup);
        soConMgmtPtr->TcpIpSocketValid = TRUE;
        soConMgmtPtr->TcpIpSocketId    = socketId;
        /*Bind the socket to the local address and port*/
        uint16         localPort = SoAd_SoConGroupCfgPtr[SoConGroup].SocketLocalPort;
        Std_ReturnType ret       = TcpIp_Bind(socketId, SOAD_SOCKETLOCALADDR_CFG(SoConGroup).LocalAddrId, &localPort);
        if (E_OK == ret)
        {
            ret = TcpIp_TcpConnect(socketId, &soConMgmtPtr->RemoteAddr);
            if (E_OK == ret)
            {
                /*SWS_SoAd_00686*/
                if (SOAD_SOCON_OFFLINE == soConMgmtPtr->SoConMode)
                {
                    soConMgmtPtr->SoConMode = SOAD_SOCON_RECONNECT;
                    SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, soConMgmtPtr, SoConGroup);
                }
            }
        }
        if (E_OK != ret)
        {
            soConMgmtPtr->TcpIpSocketValid = FALSE;
            soConMgmtPtr->TcpIpSocketId    = (TcpIp_SocketIdType)TCPIP_SOCKETID_INVALID;
            /*don't care the return value*/
            (void)TcpIp_Close(socketId, TRUE);
        }
    }
}
#endif

/**
 * @brief     Handle socket connection mode change
 */
/* PRQA S 6030 ++ */ /* VL_MTR_SoAd_STMIF */
SOAD_LOCAL void SoAd_SoConModeChangeHandle(SoAd_SoConIdType SoConId)
/* PRQA S 6030 -- */
{
    const SoAd_SocketConnectionType*    soConCfgPtr = &SoAd_SoConCfgPtr[SoConId];
    SoAd_uintx                          soConGroup  = soConCfgPtr->SocketConnectionGroupRef;
    SoAd_InnerSocketConManageType*      soConMgmtPtr;
    SoAd_InnerSocketConGroupManageType* soConGroupMgmtPtr;

#if (SOAD_PARTITION_NUM > 1u)
    uint16 partIndex  = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(SoConId)].PartitionId);
    soConMgmtPtr      = &SoAd_SoConManagerPtr[partIndex][SoAd_SoConCfgPtr[SoConId].PartitionSoConId];
    soConGroupMgmtPtr = &SoAd_SoConGroupManagerPtr[partIndex][SoAd_SoConGroupCfgPtr[soConGroup].PartitionSoConGroupId];
#else
            soConMgmtPtr      = &SoAd_SoConManagerPtr[SoConId];
            soConGroupMgmtPtr = &SoAd_SoConGroupManagerPtr[soConGroup];
#endif
    if (SOAD_SOCON_REQUEST_OPEN == soConMgmtPtr->SoConRequest[0u])
    {
        if (SOAD_SOCON_OFFLINE != soConMgmtPtr->SoConMode)
        {
            soConMgmtPtr->SoConRequest[0u] = soConMgmtPtr->SoConRequest[1u];
            soConMgmtPtr->SoConRequest[1u] = SOAD_SOCON_REQUEST_NONE;
        }
        /*SWS_SoAd_00589*/
        else if (
            soConMgmtPtr->RemoteAddrIsSet && (TCPIP_IPADDR_STATE_ASSIGNED == soConGroupMgmtPtr->LocalIpAddrState)
            && !soConMgmtPtr->TcpIpSocketValid)
        {
#if (STD_ON == SOAD_SUPPORT_UDP)
            if (SOAD_UDP == soConCfgPtr->TcpUdpProtocol)
            {
                /*SWS_SoAd_00590*/
                if (1u == SoAd_SoConGroupCfgPtr[soConGroup].SocketConNum)
                {
                    SoAd_OpenUdpSoConHandle(SoConId, soConMgmtPtr, soConGroup, soConGroupMgmtPtr);
                }
                /*SWS_SoAd_00639*/
                else
                {
                    SoAd_AttemptOpenUDPSoCon(SoConId, soConGroup, soConMgmtPtr, soConGroupMgmtPtr);
                }
            }
#endif
#if (STD_ON == SOAD_SUPPORT_TCP)
            SoAd_OpenTCPSoConHandle(SoConId, soConGroup, soConMgmtPtr, soConGroupMgmtPtr);
#endif
            /*The Open request is cleared only when the Open request is successfully executed*/
            if (SOAD_SOCON_OFFLINE != soConMgmtPtr->SoConMode)
            {
                soConMgmtPtr->SoConRequest[0u] = soConMgmtPtr->SoConRequest[1u];
                soConMgmtPtr->SoConRequest[1u] = SOAD_SOCON_REQUEST_NONE;
            }
        }
        else
        {
            /*do nothing*/
        }
    }
    else if (SOAD_SOCON_REQUEST_CLOSE == soConMgmtPtr->SoConRequest[0u])
    {
        /*SWS_SoAd_00604*/
        if (SOAD_SOCON_OFFLINE != soConMgmtPtr->SoConMode)
        {
#if (STD_ON == SOAD_SUPPORT_TCP)
            soConMgmtPtr->ChangeModeToOffline =
                SoAd_CalculateBooleanValueByCondition((SOAD_TCP == soConCfgPtr->TcpUdpProtocol), TRUE, FALSE);
#endif
            SoAd_SoConCloseHandle(SoConId, TRUE, soConMgmtPtr, soConGroupMgmtPtr);
#if (STD_ON == SOAD_SUPPORT_UDP)
            if (SOAD_UDP == soConCfgPtr->TcpUdpProtocol)
            {
                soConMgmtPtr->TcpIpSocketValid = FALSE;
                soConMgmtPtr->TcpIpSocketId    = (TcpIp_SocketIdType)TCPIP_SOCKETID_INVALID;
                soConMgmtPtr->SoConMode        = SOAD_SOCON_OFFLINE;
                SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, soConMgmtPtr, soConGroup);
            }
#endif
        }
        soConMgmtPtr->SoConRequest[0u] = soConMgmtPtr->SoConRequest[1u];
        soConMgmtPtr->SoConRequest[1u] = SOAD_SOCON_REQUEST_NONE;
    }
    else
    {
        /*handle the Disconnection And Recovery of SoCon*/
        SoAd_DisconnectionAndRecoveryHandle(SoConId, soConGroup, soConMgmtPtr, soConGroupMgmtPtr);
    }
}

#if (STD_ON == SOAD_SUPPORT_RX_BUFFER)

/**
 * @brief     Handle received TP PDU after socket connection is closed
 */
SOAD_LOCAL void SoAd_CloseRxTpPduHandle(SoAd_SoConIdType SoConId)
{
    SoAd_uintx                       soAdSocketRouteId = SOAD_UNUSED_UINT16;
    const SoAd_SocketConnectionType* soConCfgPtr       = &SoAd_SoConCfgPtr[SoConId];
    SoAd_uintx                       rxBufferId        = soConCfgPtr->RxBuffCfgIndexPtr[0];
    rxBufferId                                         = rxBufferId - SoAd_TxBufferNum;
    uint16 partIndex                                   = 0u;
#if (SOAD_PARTITION_NUM > 1u)
    partIndex = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(SoConId)].PartitionId);
    SoAd_uintx                    id              = rxBufferId - SOAD_MULTI_PART_INFO.SocketRxBuffStartId;
    const SoAd_SoRxBufferManType* soRxBuffMgmtPtr = &SoAd_SoRxBuffManagerPtr[partIndex][id];
#else
    const SoAd_SoRxBufferManType* soRxBuffMgmtPtr = &SoAd_SoRxBuffManagerPtr[rxBufferId];
#endif
    if (SOAD_UNUSED_UINT16 == soRxBuffMgmtPtr->SocketRouteId)
    {
        if (NULL_PTR != soConCfgPtr->RefSocketRoutePtr)
        {
            soAdSocketRouteId = soConCfgPtr->RefSocketRoutePtr[0u];
        }
    }
    else
    {
        soAdSocketRouteId = soRxBuffMgmtPtr->SocketRouteId;
    }
    if ((soAdSocketRouteId < SOAD_SOCKET_ROUTE_DEST_NUM)
        && (SOAD_UPPER_LAYER_TP == SoAd_SoRouteDestCfgPtr[soAdSocketRouteId].RxUpperLayerType))
    {
        PduIdType              pduId    = SoAd_SoRouteDestCfgPtr[soAdSocketRouteId].UpRxPduId;
        SoAd_UpLayerModuleType upModule = SoAd_SoRouteDestCfgPtr[soAdSocketRouteId].RefUpModule;

#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)

#if (STD_ON == SOAD_SUPPORT_HEADERID)
        if (NULL_PTR != SoAd_SocketRouteCfgPtr[soAdSocketRouteId].RxPduHeaderId)
        {
            if (soRxBuffMgmtPtr->ExitPduResidueLength > 0u)
            {
                SoAd_UpperLayerPCCfgPtr[upModule].TpRxindicationFunc(pduId, E_NOT_OK);
            }
        }
        else
#endif
        {
            SoAd_UpperLayerPCCfgPtr[upModule].TpRxindicationFunc(pduId, E_NOT_OK);
        }
#endif
    }
    /*clear the SoCon Rx Buffer*/
    SoAd_RxBufferInit(rxBufferId, partIndex);
}

/**
 * @brief     Check if the received TP PDU has been processed when TCP received FIN
 */
SOAD_LOCAL boolean SoAd_CheckRxTpPduDataHandle(SoAd_SoConIdType SoConId)
{
    boolean                          result      = FALSE;
    const SoAd_SocketConnectionType* soConCfgPtr = &SoAd_SoConCfgPtr[SoConId];

    if (NULL_PTR != soConCfgPtr->RxBuffCfgIndexPtr)
    {
        SoAd_uintx soAdSocketRouteId = SOAD_UNUSED_UINT16;
        SoAd_uintx rxBufferId        = soConCfgPtr->RxBuffCfgIndexPtr[0u];
        rxBufferId                   = rxBufferId - SoAd_TxBufferNum;
#if (SOAD_PARTITION_NUM > 1u)
        uint16     partIndex = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(SoConId)].PartitionId);
        SoAd_uintx id        = rxBufferId - SOAD_MULTI_PART_INFO.SocketRxBuffStartId;
        const SoAd_SoRxBufferManType* soRxBuffMgmtPtr = &SoAd_SoRxBuffManagerPtr[partIndex][id];
#else
        const SoAd_SoRxBufferManType* soRxBuffMgmtPtr = &SoAd_SoRxBuffManagerPtr[rxBufferId];
#endif
        if (SOAD_UNUSED_UINT16 == soRxBuffMgmtPtr->SocketRouteId)
        {
            if (NULL_PTR != soConCfgPtr->RefSocketRoutePtr)
            {
                soAdSocketRouteId = soConCfgPtr->RefSocketRoutePtr[0u];
            }
        }
        else
        {
            soAdSocketRouteId = soRxBuffMgmtPtr->SocketRouteId;
        }

        if ((soAdSocketRouteId < SOAD_SOCKET_ROUTE_DEST_NUM)
            && (SOAD_UPPER_LAYER_TP == SoAd_SoRouteDestCfgPtr[soAdSocketRouteId].RxUpperLayerType))
        {
            /*the EnterPosition is not equal to ExitPosition:the Rx Buffer have store receive data*/
            if (soRxBuffMgmtPtr->EnterPosition != soRxBuffMgmtPtr->ExitPosition)
            {
                result = TRUE;
            }
        }
    }

    return result;
}
#endif

/**
 * @brief     Close socket connection
 */
SOAD_LOCAL void SoAd_SoConCloseHandle(
    SoAd_SoConIdType                    SoConId,
    boolean                             CloseSocket,
    SoAd_InnerSocketConManageType*      soConMgmtPtr,
    SoAd_InnerSocketConGroupManageType* soConGroupMgmtPtr)
{
    const SoAd_SocketConnectionType* soConCfgPtr = &SoAd_SoConCfgPtr[SoConId];
    uint16                           partIndex   = 0u;

#if (SOAD_PARTITION_NUM > 1u)
    partIndex = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[soConCfgPtr->SocketConnectionGroupRef].PartitionId);
#endif

#if (0u < SOAD_PDU_ROUTE_DEST_MAX)
    SoAd_uintx                         refPduRouteDestNum = soConCfgPtr->RefPduRouteDestNum;
    SoAd_InnerPduRouteDestManagerType* pduRouteDestMPtr;
    for (SoAd_uintx cnt = 0u; cnt < refPduRouteDestNum; cnt++)
    {
        SoAd_uintx pduRouteDestId = soConCfgPtr->RefPduRouteDestPtr[cnt];
#if (SOAD_PARTITION_NUM > 1u)
        SoAd_uintx id    = SoAd_PduRouteDestCfgPtr[pduRouteDestId].PartitionPduRouteDestId;
        pduRouteDestMPtr = &SoAd_PduRouteDestManagerPtr[partIndex][id];
#else
        pduRouteDestMPtr = &SoAd_PduRouteDestManagerPtr[pduRouteDestId];
#endif
        if (0u < pduRouteDestMPtr->TxPendingLength)
        {
            /*Init PduRouteDestManager parameter*/
            pduRouteDestMPtr->TxPendingLength = 0u;
        }
        PduIdType pduRouteId = SoAd_PduRouteDestCfgPtr[pduRouteDestId].PduRouteIndex;
        /*Init PduRouteManager parameter*/
        SoAd_PduRouteManagerInit(pduRouteId);
        if (soConMgmtPtr->TpPduTransmitting && (pduRouteId == soConMgmtPtr->ActiveSoAdPduRouteId))
        {
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
            SoAd_UpLayerModuleType upModule = SoAd_PduRouteCfgPtr[pduRouteId].RefUpModule;

            /*TP transmitting,call the up TpTxConfirmation with E_NOT_OK*/
            SoAd_UpperLayerPCCfgPtr[upModule].TpTxConfirmationFunc(SoAd_PduRouteCfgPtr[pduRouteId].UpPduId, E_NOT_OK);

#endif
        }
    }
#if (STD_ON == SOAD_SUPPORT_TX_BUFFER)
    if (NULL_PTR != soConCfgPtr->TxBuffCfgIndexPtr)
    {
        /*Init Tx Buffer parameter*/
        SoAd_TxBufferInit(soConCfgPtr->TxBuffCfgIndexPtr[0], partIndex);
    }
#endif
#endif

#if (STD_ON == SOAD_SUPPORT_RX_BUFFER)
    /* TP PDU is receiving handle*/
    if (NULL_PTR != soConCfgPtr->RxBuffCfgIndexPtr)
    {
        SoAd_CloseRxTpPduHandle(SoConId);
    }
#endif

    SoAd_CloseSoConTcpIpSocket(SoConId, CloseSocket, soConGroupMgmtPtr, soConMgmtPtr);

    /*clear the SoAd_SoConManager parameters*/
    soConMgmtPtr->SoConCloseAbort = FALSE;
    soConMgmtPtr->SocketNeedClose = SOAD_SOCKET_CLOSE_NONE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
    soConGroupMgmtPtr->SetSocketNeedCloseLine = 0;
#endif
    soConMgmtPtr->TpPduTransmitting = FALSE;
    soConMgmtPtr->TxNoAckLength     = 0u;
#if (STD_ON == SOAD_SUPPORT_UDP)
    soConMgmtPtr->UdpAliveSupervisionTime = 0u;
#endif
    /*SWS_SoAd_00527:SoAd shall reset the remote address to the configured remote
      address (or unset the remote address in case no remote address has been
      configured)*/
    if (NULL_PTR != SOAD_SOCONREMOTEADDR_CFG(SoConId))
    {
        soConMgmtPtr->RemoteAddr = *SOAD_SOCONREMOTEADDR_CFG(SoConId);
    }
}

/**
 * @brief     Handle socket connection disconnection and recovery
 */
SOAD_LOCAL void SoAd_DisconnectionAndRecoveryHandle(
    SoAd_SoConIdType                    SoConId,
    SoAd_uintx                          SoConGroup,
    SoAd_InnerSocketConManageType*      soConMgmtPtr,
    SoAd_InnerSocketConGroupManageType* soConGroupMgmtPtr)
{
    /* handle the disconnection */
    if (SOAD_SOCKET_CLOSE_IMMEDIATE == soConMgmtPtr->SocketNeedClose)
    {
        SoAd_SoConCloseHandle(SoConId, TRUE, soConMgmtPtr, soConGroupMgmtPtr);
        /*Notification UP Module SoCon changed*/
        if (soConMgmtPtr->SoConMode == SOAD_SOCON_ONLINE)
        {
            soConMgmtPtr->SoConMode = SOAD_SOCON_RECONNECT;
            SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, soConMgmtPtr, SoConGroup);
        }
    }
    else if (SOAD_SOCON_RECONNECT == soConMgmtPtr->SoConMode)
    {
        if ((TCPIP_IPADDR_STATE_ASSIGNED == soConGroupMgmtPtr->LocalIpAddrState) && soConMgmtPtr->RemoteAddrIsSet)
        {
#if (STD_ON == SOAD_SUPPORT_UDP)
            /*UDP socket connection recovery*/
            SoAd_AttemptOpenUDPSoCon(SoConId, SoConGroup, soConMgmtPtr, soConGroupMgmtPtr);
#endif
#if (STD_ON == SOAD_SUPPORT_TCP)
            SoAd_OpenTCPSoConHandle(SoConId, SoConGroup, soConMgmtPtr, soConGroupMgmtPtr);
#endif
        }
    }
    else
    {
        /*do nothing*/
    }
}

/**
 * control ronting group
 */
#if (0u < SOAD_ROUTING_GROUP_MAX)
/**
 * @brief         Controls the routing group.
 */
SOAD_LOCAL Std_ReturnType SoAd_RoutingGroupControl(SoAd_RoutingGroupIdType id, boolean enFlag)
{
    SoAd_uintx                   routeDestId;
    SoAd_RoutingGroupIdType      routeGroupId, routeGroupCnt;
    boolean                      flagSet;
    const SoAd_RoutingGroupType* routGroupCfgPtr = &SoAd_PbCfgPtr->RoutingGroup[id];

#if (SOAD_PARTITION_NUM > 1u)
    uint16 partIndex = SoAd_GetPartitionIndex(SoAd_RouteGroupCfgPtr[id].PartitionId);
    SoAd_RoutGroupManagerPtr[partIndex][routGroupCfgPtr->PartitionRoutGroupId].RoutGroupEnable = enFlag;
#else
    SoAd_RoutGroupManagerPtr[id].RoutGroupEnable = enFlag;
#endif

#if (0u < SOAD_SOCKET_ROUTE_DEST_MAX)
    const SoAd_SocketRouteDestType* socketRouteDestCfgPtr;
    SoAd_RoutingGroupIdType         rxRoutGroupRefNum;
    SoAd_uintx                      socketRouteDestNum = routGroupCfgPtr->SocketRouteDestNum;

    for (SoAd_uintx cnt = 0u; cnt < socketRouteDestNum; cnt++)
    {
        routeDestId           = routGroupCfgPtr->SocketRouteDestIndexPtr[cnt];
        socketRouteDestCfgPtr = &SoAd_SoRouteDestCfgPtr[routeDestId];
        flagSet               = FALSE;
        rxRoutGroupRefNum     = socketRouteDestCfgPtr->RxRoutingGroupRefNum;
        for (routeGroupCnt = 0u; (routeGroupCnt < rxRoutGroupRefNum) && (!flagSet); routeGroupCnt++)
        {
            routeGroupId = socketRouteDestCfgPtr->RxRoutingGroupRefPtr[routeGroupCnt];
#if (SOAD_PARTITION_NUM > 1u)
            SoAd_RoutingGroupIdType groupId = SoAd_RouteGroupCfgPtr[routeGroupId].PartitionRoutGroupId;
            if (SoAd_RoutGroupManagerPtr[partIndex][groupId].RoutGroupEnable)
#else
            if (SoAd_RoutGroupManagerPtr[routeGroupId].RoutGroupEnable)
#endif
            {
                flagSet = TRUE;
            }
        }
#if (SOAD_PARTITION_NUM > 1u)
        SoAd_uintx startId = socketRouteDestCfgPtr->PartitionSocketRouteDestId;
        SoAd_SoRouteDestManagerPtr[partIndex][startId].SocketRouteDestEnable = flagSet;
#else
        SoAd_SoRouteDestManagerPtr[routeDestId].SocketRouteDestEnable = flagSet;
#endif
    }
#endif

#if (0u < SOAD_PDU_ROUTE_DEST_MAX)
    const SoAd_PduRouteDestType* pduRouteDestCfgPtr;
    SoAd_RoutingGroupIdType      txRoutGroupRefNum;
    SoAd_uintx                   pduRouteDestNum = routGroupCfgPtr->PduRouteDestNum;

    for (SoAd_uintx cnt = 0u; cnt < pduRouteDestNum; cnt++)
    {
        routeDestId        = routGroupCfgPtr->PduRouteDestIndexPtr[cnt];
        pduRouteDestCfgPtr = &SoAd_PduRouteDestCfgPtr[routeDestId];
        flagSet            = FALSE;
        txRoutGroupRefNum  = pduRouteDestCfgPtr->TxRoutingGroupRefNum;
        for (routeGroupCnt = 0u; (routeGroupCnt < txRoutGroupRefNum) && (!flagSet); routeGroupCnt++)
        {
            routeGroupId = pduRouteDestCfgPtr->TxRoutingGroupRefPtr[routeGroupCnt];
#if (SOAD_PARTITION_NUM > 1u)
            SoAd_RoutingGroupIdType groupId = SoAd_RouteGroupCfgPtr[routeGroupId].PartitionRoutGroupId;
            if (SoAd_RoutGroupManagerPtr[partIndex][groupId].RoutGroupEnable)
#else
            if (SoAd_RoutGroupManagerPtr[routeGroupId].RoutGroupEnable)
#endif
            {
                flagSet = TRUE;
            }
        }

#if (SOAD_PARTITION_NUM > 1u)
        SoAd_uintx startId = pduRouteDestCfgPtr->PartitionPduRouteDestId;
        SoAd_PduRouteDestManagerPtr[partIndex][startId].PduRouteDestEnable = flagSet;
#else
        SoAd_PduRouteDestManagerPtr[routeDestId].PduRouteDestEnable = flagSet;
#endif
    }
#endif

    return E_OK;
}
#endif

#if (STD_ON == SOAD_SUPPORT_TX_BUFFER)
/**
 * @brief         Initializes the transmit buffer.
 */
SOAD_LOCAL void SoAd_TxBufferInit(SoAd_uintx TxBufferId, uint16 partIndex)
{
#if (SOAD_PARTITION_NUM > 1u)
    SoAd_uintx           id              = TxBufferId - SOAD_MULTI_PART_INFO.SocketTxBuffStartId;
    SoAd_SoTxBufManType* soTxBuffMgmtPtr = &SoAd_SoTxBuffManagerPtr[partIndex][id];
#else
    SOAD_NOUSED(partIndex);
    SoAd_SoTxBufManType* soTxBuffMgmtPtr = &SoAd_SoTxBuffManagerPtr[TxBufferId];
#endif

#if (STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER)
    soTxBuffMgmtPtr->TriggerTimer = 0u;
#endif
    soTxBuffMgmtPtr->UpCopyLength     = 0u;
    soTxBuffMgmtPtr->TxPduTotalLength = 0u;
    soTxBuffMgmtPtr->TxResidueLength  = SoAd_SocketBufferCfgPtr[TxBufferId].BuffLen;
}
#endif

#if (STD_ON == SOAD_SUPPORT_RX_BUFFER)
/**
 * @brief         Initializes the receive buffer.
 */
SOAD_LOCAL void SoAd_RxBufferInit(SoAd_uintx RxBufferId, uint16 partIndex)
{
#if (SOAD_PARTITION_NUM > 1u)
    SoAd_uintx              id              = RxBufferId - SOAD_MULTI_PART_INFO.SocketRxBuffStartId;
    SoAd_SoRxBufferManType* soRxBuffMgmtPtr = &SoAd_SoRxBuffManagerPtr[partIndex][id];
#else
    SOAD_NOUSED(partIndex);
    SoAd_SoRxBufferManType* soRxBuffMgmtPtr = &SoAd_SoRxBuffManagerPtr[RxBufferId];
#endif

    soRxBuffMgmtPtr->EnterPosition        = 0x0u;
    soRxBuffMgmtPtr->ExitPosition         = 0x0u;
    soRxBuffMgmtPtr->ExitPduResidueLength = 0x0u;
    soRxBuffMgmtPtr->SocketRouteId        = SOAD_UNUSED_UINT16;
}
#endif

#if (STD_ON == SOAD_SUPPORT_TX_BUFFER) || (STD_ON == SOAD_SUPPORT_RX_BUFFER)

/**
 * @brief         Initializes the transmit and receive buffers.
 */
SOAD_LOCAL void SoAd_TxRxBufferInit(uint16 partIndex)
{
    uint16 startId;

#if (STD_ON == SOAD_SUPPORT_TX_BUFFER)
    startId = SOAD_MULTI_PART_INFO.SocketTxBuffStartId;
    for (uint16 cnt = startId; cnt < SoAd_TxBufferNum; cnt++)
    {
        SoAd_TxBufferInit(cnt, partIndex);
    }
#endif

#if (STD_ON == SOAD_SUPPORT_RX_BUFFER)
    startId = SOAD_MULTI_PART_INFO.SocketRxBuffStartId;
    for (uint16 cnt = startId; cnt < SoAd_RxBufferNum; cnt++)
    {
        SoAd_RxBufferInit(cnt, partIndex);
    }
#endif
}
#endif

#if (STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER)
/**
 * @brief     Handle NPDU UDP TX PDU pending
 */
void SoAd_NPduUdpTxPduPendingHandle(SoAd_SoConIdType SoConId)
{
    SoAd_InnerPduRoutManageType* pduRouteMgmtPtr;
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION) && (SOAD_PARTITION_NUM > 1u)
    uint16          partIndex = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(SoConId)].PartitionId);
    ApplicationType curAppId  = GetApplicationID();
#endif
    for (PduIdType txPduId = 0u; txPduId < SOAD_PDU_ROUTE_NUM; txPduId++)
    {
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION) && (SOAD_PARTITION_NUM > 1u)
        if (SoAd_PduRouteCfgPtr[txPduId].PartitionId != curAppId)
        {
            continue;
        }

        pduRouteMgmtPtr = &SoAd_PduRouteManagerPtr[partIndex][SoAd_PduRouteCfgPtr[txPduId].PartitionPduRouteId];
#else
        pduRouteMgmtPtr = &SoAd_PduRouteManagerPtr[txPduId];
#endif
        if ((SoConId == pduRouteMgmtPtr->TxSoConId) && (0u < pduRouteMgmtPtr->TxPendingNum))
        {
            pduRouteMgmtPtr->TxPendingNum = 0u;
        }
    }
}

/**
 * @brief         Handles the main function for NPDU UDP transmission.
 */
SOAD_LOCAL void SoAd_MainFunctionNPduUdpTxHandle(SoAd_SoConIdType SoConId)
{
    if (NULL_PTR != SOAD_SOCKET_TXBUF_INDEX_CFG(SoConId))
    {
        SoAd_uintx txBufferId = *SOAD_SOCKET_TXBUF_INDEX_CFG(SoConId);
        uint16     partIndex  = 0u;
#if (SOAD_PARTITION_NUM > 1u)
        partIndex = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(SoConId)].PartitionId);
        uint16               id              = txBufferId - SOAD_MULTI_PART_INFO.SocketTxBuffStartId;
        SoAd_SoTxBufManType* soTxBuffMgmtPtr = &SoAd_SoTxBuffManagerPtr[partIndex][id];
#else
        SoAd_SoTxBufManType* soTxBuffMgmtPtr = &SoAd_SoTxBuffManagerPtr[txBufferId];
#endif
        if (0u < soTxBuffMgmtPtr->TriggerTimer)
        {
            soTxBuffMgmtPtr->TriggerTimer -= 1u;
            if (0u == soTxBuffMgmtPtr->TriggerTimer)
            {
                SoAd_InnerSocketConManageType* soConMgmtPtr;
#if (SOAD_PARTITION_NUM > 1u)
                soConMgmtPtr = &SoAd_SoConManagerPtr[partIndex][SoAd_SoConCfgPtr[SoConId].PartitionSoConId];
#else
                soConMgmtPtr = &SoAd_SoConManagerPtr[SoConId];
#endif
                (void)TcpIp_UdpTransmit(
                    soConMgmtPtr->TcpIpSocketId,
                    &SoAd_PduBuffer[SoAd_SocketBufferCfgPtr[txBufferId].BuffPos],
                    &soConMgmtPtr->RemoteAddr,
                    (uint16)soTxBuffMgmtPtr->TxPduTotalLength);
                SoAd_TxBufferInit(txBufferId, partIndex);
                SoAd_SocnTransmitAutomaticSoConSetupHandle(SoConId, soConMgmtPtr, SOAD_UDP);
                SoAd_NPduUdpTxPduPendingHandle(SoConId);
            }
        }
    }
}
#endif

#if (SOAD_PDU_ROUTE_DEST_MAX > 0)
/**
 * @brief         Handles the main function for transmission confirmation.
 */
SOAD_LOCAL void SoAd_MainFunctionTxConfirmationHandle(void)
{
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION) && (SOAD_PARTITION_NUM > 1u)
    ApplicationType curAppId = GetApplicationID();
#endif

    for (PduIdType txPduId = 0u; txPduId < SOAD_PDU_ROUTE_NUM; txPduId++)
    {
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION) && (SOAD_PARTITION_NUM > 1u)
        if (SoAd_PduRouteCfgPtr[txPduId].PartitionId != curAppId)
        {
            continue;
        }
        uint16    partIndex    = SoAd_GetPartitionIndex(SoAd_PduRouteCfgPtr[txPduId].PartitionId);
        PduIdType id           = SoAd_PduRouteCfgPtr[txPduId].PartitionPduRouteId;
        uint8     txPendingNum = SoAd_PduRouteManagerPtr[partIndex][id].TxPendingNum;
#else
        uint8 txPendingNum = SoAd_PduRouteManagerPtr[txPduId].TxPendingNum;
#endif
        if (0u == txPendingNum)
        {
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
            SoAd_UpLayerModuleType upModule = SoAd_PduRouteCfgPtr[txPduId].RefUpModule;

            SoAd_UpperLayerPCCfgPtr[upModule].IfTxConfirmationFunc(SoAd_PduRouteCfgPtr[txPduId].UpPduId, E_OK);

#endif
            SoAd_PduRouteManagerInit(txPduId);
        }
    }
}
#endif

#if (STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER)
/**
 * @brief Handles the "trigger always" operation for UDP transmission.
 */
SOAD_LOCAL Std_ReturnType SoAd_NPduUdpTransmitHandleTriggerAlways(
    SoAd_InnerPduRoutManageType*   pduRouteMgmtPtr,
    const SoAd_PduRouteDestType*   pduRouteDestCfgPtr,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    SoAd_SoConIdType               SoConId)
{
    Std_ReturnType result;
    uint16         partIndex  = 0u;
    SoAd_uintx     txBufferId = *SOAD_SOCKET_TXBUF_INDEX_CFG(SoConId);
    PduLengthType  pduLength  = pduRouteMgmtPtr->PduLength + SOAD_HEADER_SIZE;
    SOAD_FILL_HEADER(&SoAd_IfPduBuffer[0], pduRouteDestCfgPtr->TxPduHeaderId[0], pduRouteMgmtPtr->PduLength);
#if (SOAD_PARTITION_NUM > 1u)
    SoAd_uintx           id              = txBufferId - SOAD_MULTI_PART_INFO.SocketTxBuffStartId;
    SoAd_SoTxBufManType* soTxBuffMgmtPtr = &SoAd_SoTxBuffManagerPtr[partIndex][id];
#else
    SoAd_SoTxBufManType* soTxBuffMgmtPtr = &SoAd_SoTxBuffManagerPtr[txBufferId];
#endif
    PduLengthType txBufferPos = SoAd_SocketBufferCfgPtr[txBufferId].BuffPos;

    if (soTxBuffMgmtPtr->TxResidueLength < pduLength)
    {
        (void)TcpIp_UdpTransmit(
            soConMgmtPtr->TcpIpSocketId,
            &SoAd_PduBuffer[txBufferPos],
            &soConMgmtPtr->RemoteAddr,
            (uint16)soTxBuffMgmtPtr->TxPduTotalLength);

        SoAd_TxBufferInit(txBufferId, partIndex);

        result = TcpIp_UdpTransmit(
            soConMgmtPtr->TcpIpSocketId,
            &SoAd_IfPduBuffer[0],
            &soConMgmtPtr->RemoteAddr,
            (uint16)pduLength);
        SoAd_SocnTransmitAutomaticSoConSetupHandle(SoConId, soConMgmtPtr, SOAD_UDP);
    }
    else
    {
        (void)IStdLib_MemCpy(
            &SoAd_PduBuffer[txBufferPos + (soTxBuffMgmtPtr->TxPduTotalLength)],
            &SoAd_IfPduBuffer[0],
            pduLength);
        soTxBuffMgmtPtr->TxPduTotalLength += pduLength;
        soTxBuffMgmtPtr->TxResidueLength -= pduLength;
        result = TcpIp_UdpTransmit(
            soConMgmtPtr->TcpIpSocketId,
            &SoAd_PduBuffer[txBufferPos],
            &soConMgmtPtr->RemoteAddr,
            (uint16)soTxBuffMgmtPtr->TxPduTotalLength);

        SoAd_TxBufferInit(txBufferId, partIndex);
        SoAd_SocnTransmitAutomaticSoConSetupHandle(SoConId, soConMgmtPtr, SOAD_UDP);
    }

    return result;
}
/**
 * @brief Handles the "trigger never" operation for UDP transmission.
 */
SOAD_LOCAL Std_ReturnType SoAd_NPduUdpTransmitHandleTriggerNever(
    SoAd_InnerPduRoutManageType*   pduRouteMgmtPtr,
    const SoAd_PduRouteDestType*   pduRouteDestCfgPtr,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    SoAd_SoConIdType               SoConId)
{
    Std_ReturnType result;
    uint16         partIndex  = 0u;
    SoAd_uintx     txBufferId = *SOAD_SOCKET_TXBUF_INDEX_CFG(SoConId);
    PduLengthType  pduLength  = pduRouteMgmtPtr->PduLength + SOAD_HEADER_SIZE;
    SOAD_FILL_HEADER(&SoAd_IfPduBuffer[0], pduRouteDestCfgPtr->TxPduHeaderId[0], pduRouteMgmtPtr->PduLength);
#if (SOAD_PARTITION_NUM > 1u)
    SoAd_uintx           id              = txBufferId - SOAD_MULTI_PART_INFO.SocketTxBuffStartId;
    SoAd_SoTxBufManType* soTxBuffMgmtPtr = &SoAd_SoTxBuffManagerPtr[partIndex][id];
#else
    SoAd_SoTxBufManType* soTxBuffMgmtPtr = &SoAd_SoTxBuffManagerPtr[txBufferId];
#endif
    PduLengthType txBufferPos = SoAd_SocketBufferCfgPtr[txBufferId].BuffPos;

    if (soTxBuffMgmtPtr->TxResidueLength < pduLength)
    {
        (void)TcpIp_UdpTransmit(
            soConMgmtPtr->TcpIpSocketId,
            &SoAd_PduBuffer[txBufferPos],
            &soConMgmtPtr->RemoteAddr,
            (uint16)soTxBuffMgmtPtr->TxPduTotalLength);

        SoAd_TxBufferInit(txBufferId, partIndex);
        SoAd_NPduUdpTxPduPendingHandle(SoConId);

        (void)IStdLib_MemCpy(
            &SoAd_PduBuffer[txBufferPos + (soTxBuffMgmtPtr->TxPduTotalLength)],
            &SoAd_IfPduBuffer[0],
            pduLength);
        soTxBuffMgmtPtr->TxPduTotalLength += pduLength;
        soTxBuffMgmtPtr->TxResidueLength -= pduLength;
        soTxBuffMgmtPtr->TriggerTimer = pduRouteDestCfgPtr->CfgUdpTriggerTimeout;
        result                        = E_OK;
    }
    else
    {
        (void)IStdLib_MemCpy(
            &SoAd_PduBuffer[txBufferPos + (soTxBuffMgmtPtr->TxPduTotalLength)],
            &SoAd_IfPduBuffer[0],
            pduLength);
        soTxBuffMgmtPtr->TxPduTotalLength += pduLength;
        soTxBuffMgmtPtr->TxResidueLength -= pduLength;
        if ((pduRouteDestCfgPtr->CfgUdpTriggerTimeout < soTxBuffMgmtPtr->TriggerTimer)
            || (soTxBuffMgmtPtr->TriggerTimer == 0u))
        {
            soTxBuffMgmtPtr->TriggerTimer = pduRouteDestCfgPtr->CfgUdpTriggerTimeout;
        }
        result = E_OK;
    }

    return result;
}
/**
 * @brief         Handles the transmission of NPDU UDP.
 */
SOAD_LOCAL Std_ReturnType SoAd_NPduUdpTransmitHandle(
    SoAd_InnerPduRoutManageType*   pduRouteMgmtPtr,
    const SoAd_PduRouteDestType*   pduRouteDestCfgPtr,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    SoAd_SoConIdType               SoConId)
{
    Std_ReturnType result;

    if (SOAD_TRIGGER_ALWAYS == pduRouteDestCfgPtr->TxUdpTriggerMode)
    {
        result = SoAd_NPduUdpTransmitHandleTriggerAlways(pduRouteMgmtPtr, pduRouteDestCfgPtr, soConMgmtPtr, SoConId);
    }
    else
    {
        result = SoAd_NPduUdpTransmitHandleTriggerNever(pduRouteMgmtPtr, pduRouteDestCfgPtr, soConMgmtPtr, SoConId);
    }

    if (E_OK == result)
    {
        if (SOAD_UNUSED_UINT8 == pduRouteMgmtPtr->TxPendingNum)
        {
            SoAd_uintx txBufferId = *SOAD_SOCKET_TXBUF_INDEX_CFG(SoConId);
#if (SOAD_PARTITION_NUM > 1u)
            uint16 partIndex = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(SoConId)].PartitionId);
            uint16 id        = txBufferId - SOAD_MULTI_PART_INFO.SocketTxBuffStartId;
            SoAd_SoTxBufManType* soTxBuffMgmtPtr = &SoAd_SoTxBuffManagerPtr[partIndex][id];
#else
            SoAd_SoTxBufManType* soTxBuffMgmtPtr = &SoAd_SoTxBuffManagerPtr[txBufferId];
#endif
            pduRouteMgmtPtr->TxPendingNum = soTxBuffMgmtPtr->TriggerTimer;
            pduRouteMgmtPtr->TxSoConId    = SoConId;

            if (soTxBuffMgmtPtr->TriggerTimer == 0u)
            {
                SoAd_NPduUdpTxPduPendingHandle(SoConId);
            }
        }
    }
    return result;
}
#endif

#if ((0u < SOAD_MAX_IF_PDU_SIZE) && (SOAD_PDU_ROUTE_DEST_MAX > 0))

/**
 * @brief         Transmits an IF PDU on a specific socket connection.
 */
SOAD_LOCAL Std_ReturnType SoAd_IfTransmitSoCon(
    PduIdType                      TxPduId,
    SoAd_InnerPduRoutManageType*   pduRouteMgmtPtr,
    SoAd_uintx                     PduRouteDestId,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    SoAd_SoConIdType               SoConId)
{
    Std_ReturnType               result;
    const SoAd_PduRouteDestType* pduRouteDestCfgPtr = &SoAd_PduRouteDestCfgPtr[PduRouteDestId];

#if (STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER)
    if (SOAD_TRIGGER_UNSET != pduRouteDestCfgPtr->TxUdpTriggerMode)
    {
        result = SoAd_NPduUdpTransmitHandle(pduRouteMgmtPtr, pduRouteDestCfgPtr, soConMgmtPtr, SoConId);
    }
    else
#endif
    {
#if (STD_ON == SOAD_SUPPORT_TCP) || (STD_ON == SOAD_SUPPORT_UDP)
        PduLengthType pduLength;
        const uint8*  dataPtr;

#if (STD_ON == SOAD_SUPPORT_HEADERID)
        if (NULL_PTR != pduRouteDestCfgPtr->TxPduHeaderId)
        {
            SOAD_FILL_HEADER(&SoAd_IfPduBuffer[0], pduRouteDestCfgPtr->TxPduHeaderId[0], pduRouteMgmtPtr->PduLength);
            pduLength = pduRouteMgmtPtr->PduLength + SOAD_HEADER_SIZE;
            dataPtr   = &SoAd_IfPduBuffer[0];
        }
        else
#endif
        {
            pduLength = pduRouteMgmtPtr->PduLength;
            dataPtr   = pduRouteMgmtPtr->PduDataPtr;
        }
#if (STD_ON == SOAD_SUPPORT_TCP)
        if (SOAD_TCP == SoAd_SoConCfgPtr[SoConId].TcpUdpProtocol)
        {
            /*transmit success manage parameter set*/
            soConMgmtPtr->TxNoAckLength += pduLength;
            SoAd_InnerPduRouteDestManagerType* pduRouteDestMPtr;
#if (SOAD_PARTITION_NUM > 1u)
            uint16 partIndex = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(SoConId)].PartitionId);
            SoAd_uintx destStartId = pduRouteDestCfgPtr->PartitionPduRouteDestId;
            pduRouteDestMPtr       = &SoAd_PduRouteDestManagerPtr[partIndex][destStartId];
#else
            pduRouteDestMPtr = &SoAd_PduRouteDestManagerPtr[PduRouteDestId];
#endif
            pduRouteDestMPtr->TxPendingLength = soConMgmtPtr->TxNoAckLength;
            pduRouteMgmtPtr->TxPendingNum     = (uint8)SoAd_CalculateU16ValueByCondition(
                (SOAD_UNUSED_UINT8 == pduRouteMgmtPtr->TxPendingNum),
                1u,
                (uint16)(pduRouteMgmtPtr->TxPendingNum) + 1u);

            result = TcpIp_TcpTransmit(soConMgmtPtr->TcpIpSocketId, dataPtr, pduLength, TRUE);
            if (E_OK != result)
            {
                soConMgmtPtr->TxNoAckLength -= pduLength;
                pduRouteDestMPtr->TxPendingLength = 0u;

                pduRouteMgmtPtr->TxPendingNum = (uint8)SoAd_CalculateU16ValueByCondition(
                    (1u == pduRouteMgmtPtr->TxPendingNum),
                    SOAD_UNUSED_UINT8,
                    (uint16)(pduRouteMgmtPtr->TxPendingNum) - 1u);
            }
        }
        else
#endif
        {
#if (STD_ON == SOAD_SUPPORT_UDP)
            result =
                TcpIp_UdpTransmit(soConMgmtPtr->TcpIpSocketId, dataPtr, &soConMgmtPtr->RemoteAddr, (uint16)pduLength);
            if (E_OK == result)
            {
                const SoAd_PduRouteType* pduRouteCfgPtr = &SoAd_PduRouteCfgPtr[TxPduId];
#if (STD_ON == SOAD_UDP_SOCON_IMMEDIATE_TXCONFIRMATION)
                SoAd_uintx soConGroupId = SOAD_SOCON_REFGORUP(SoConId);
                if ((SOAD_UDPTYPE_CFG(soConGroupId)->SocketUdpImmediateTxConfirmEnabled)
                    && (pduRouteCfgPtr->UDPImmediateTxConfirmation))
                {
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
                    SoAd_UpLayerModuleType upModule = pduRouteCfgPtr->RefUpModule;

                    SoAd_UpperLayerPCCfgPtr[upModule].IfTxConfirmationFunc(pduRouteCfgPtr->UpPduId, E_OK);

#endif
                    SoAd_PduRouteManagerInit(TxPduId);
                }
                else
#endif
                    if (SOAD_UNUSED_UINT8 == pduRouteMgmtPtr->TxPendingNum)
                {
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
                    SoAd_UpLayerModuleType upModule = pduRouteCfgPtr->RefUpModule;
                    /* Return immediately without configuration TxConfirmation for Udp */
                    if (SoAd_DummyIfTxConfirmation != SoAd_UpperLayerPCCfgPtr[upModule].IfTxConfirmationFunc)
#endif
                    {
                        pduRouteMgmtPtr->TxPendingNum = 0u;
                    }
                }
            }
            SoAd_SocnTransmitAutomaticSoConSetupHandle(SoConId, soConMgmtPtr, SOAD_UDP);
#endif
        }
#endif
    }
    if (E_NOT_OK == result)
    {
        pduRouteMgmtPtr->TxPendingNum = SOAD_UNUSED_UINT8;
    }
    return result;
}
#endif

#if (STD_ON == SOAD_SUPPORT_TX_BUFFER)
/**
 * @brief         Handles the main function for TP PDU transmission.
 */
/* PRQA S 6030 ++ */ /* VL_MTR_SoAd_STMIF */
SOAD_LOCAL void SoAd_MainFunctionTpTransmitHandle(SoAd_SoConIdType SoConId)
/* PRQA S 6030 -- */
{
    SoAd_InnerSocketConManageType* soConMgmtPtr;
    SoAd_uintx                     txBufferId = *SOAD_SOCKET_TXBUF_INDEX_CFG(SoConId);
#if (SOAD_PARTITION_NUM > 1u)
    uint16     partIndex = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(SoConId)].PartitionId);
    SoAd_uintx id        = txBufferId - SOAD_MULTI_PART_INFO.SocketTxBuffStartId;
    SoAd_SoTxBufManType* soTxBuffMgmtPtr = &SoAd_SoTxBuffManagerPtr[partIndex][id];
    soConMgmtPtr                         = &SoAd_SoConManagerPtr[partIndex][SoAd_SoConCfgPtr[SoConId].PartitionSoConId];
#else
    SoAd_SoTxBufManType* soTxBuffMgmtPtr = &SoAd_SoTxBuffManagerPtr[txBufferId];
    soConMgmtPtr                         = &SoAd_SoConManagerPtr[SoConId];
#endif

    if (soConMgmtPtr->TpPduTransmitting && (SOAD_SOCKET_CLOSE_NONE == soConMgmtPtr->SocketNeedClose)
        && (soTxBuffMgmtPtr->UpCopyLength < soTxBuffMgmtPtr->TxPduTotalLength))
    {
        BufReq_ReturnType result     = BUFREQ_E_NOT_OK;
        PduIdType         pduRouteId = soConMgmtPtr->ActiveSoAdPduRouteId;

#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
        SoAd_UpLayerModuleType upModule = SoAd_PduRouteCfgPtr[pduRouteId].RefUpModule;
        PduIdType              upPduId  = SoAd_PduRouteCfgPtr[pduRouteId].UpPduId;

        PduLengthType validDataLength;
        PduInfoType   pduInfo;

        pduInfo.MetaDataPtr = NULL_PTR;
        pduInfo.SduLength   = 0u;
        pduInfo.SduDataPtr  = NULL_PTR;

        result = SoAd_UpperLayerPCCfgPtr[upModule].TpCopyTxDataFunc(upPduId, &pduInfo, NULL_PTR, &validDataLength);
        if ((BUFREQ_OK == result)
            && (soTxBuffMgmtPtr->TxPduTotalLength >= (soTxBuffMgmtPtr->UpCopyLength + validDataLength)))
        {
            if ((SOAD_TCP == SoAd_SoConCfgPtr[SoConId].TcpUdpProtocol)
                && (SOAD_SOCKET_CLOSE_NONE == soConMgmtPtr->SocketNeedClose))
            {
                if (E_NOT_OK == TcpIp_TcpTransmit(soConMgmtPtr->TcpIpSocketId, NULL_PTR, validDataLength, FALSE))
                {
                    result = BUFREQ_E_NOT_OK;
                }
            }
            else
            {
                PduLengthType txBufferPos = SoAd_SocketBufferCfgPtr[txBufferId].BuffPos;
                pduInfo.SduDataPtr        = &SoAd_PduBuffer[txBufferPos + soTxBuffMgmtPtr->UpCopyLength];
                pduInfo.SduLength         = validDataLength;
                result =
                    SoAd_UpperLayerPCCfgPtr[upModule].TpCopyTxDataFunc(upPduId, &pduInfo, NULL_PTR, &validDataLength);
                if (BUFREQ_OK == result)
                {
                    soTxBuffMgmtPtr->UpCopyLength += pduInfo.SduLength;
                    if (soTxBuffMgmtPtr->TxPduTotalLength == soTxBuffMgmtPtr->UpCopyLength)
                    {
                        Std_ReturnType resultLo = TcpIp_UdpTransmit(
                            soConMgmtPtr->TcpIpSocketId,
                            &SoAd_PduBuffer[txBufferPos],
                            &soConMgmtPtr->RemoteAddr,
                            (uint16)soTxBuffMgmtPtr->TxPduTotalLength);
                        if (E_OK == resultLo)
                        {
                            soTxBuffMgmtPtr->TxPduTotalLength = 0u;
                            soTxBuffMgmtPtr->UpCopyLength     = 0u;
                            soConMgmtPtr->TpPduTransmitting   = FALSE;

                            SoAd_UpperLayerPCCfgPtr[upModule].TpTxConfirmationFunc(upPduId, E_OK);
                        }
                        else
                        {
                            result = BUFREQ_E_NOT_OK;
                        }
                        SoAd_SocnTransmitAutomaticSoConSetupHandle(SoConId, soConMgmtPtr, SOAD_UDP);
                    }
                }
            }
        }
        else
        {
            result = BUFREQ_E_NOT_OK;
        }

        if (BUFREQ_E_NOT_OK == result)
        {
            SoAd_TpPduTxErrorHandle(SoConId, txBufferId, upModule, upPduId);
        }
#endif
    }
}
#endif

#if (STD_ON == SOAD_SUPPORT_TX_BUFFER)

/**
 * @brief         Handles the error for a TP PDU transmission.
 */
SOAD_LOCAL void SoAd_TpPduTxErrorHandle(
    SoAd_SoConIdType       SoConId,
    SoAd_uintx             TxBufferId,
    SoAd_UpLayerModuleType UpModule,
    PduIdType              UpPduId)
{
    SoAd_InnerSocketConManageType* soConMgmtPtr;
    uint16                         partIndex = 0u;
#if (SOAD_PARTITION_NUM > 1u)
    partIndex    = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(SoConId)].PartitionId);
    soConMgmtPtr = &SoAd_SoConManagerPtr[partIndex][SoAd_SoConCfgPtr[SoConId].PartitionSoConId];
#else
    soConMgmtPtr = &SoAd_SoConManagerPtr[SoConId];
#endif

    if (SOAD_TCP == SoAd_SoConCfgPtr[SoConId].TcpUdpProtocol)
    {
        soConMgmtPtr->SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
        soConMgmtPtr->SetSocketNeedCloseLine = __LINE__;
#endif
    }
    else
    {
        /* SWS_SoAd_00652 SWS_SoAd_00670 */
        SoAd_TxBufferInit(TxBufferId, partIndex);
        soConMgmtPtr->TpPduTransmitting = FALSE;

#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)

        SoAd_UpperLayerPCCfgPtr[UpModule].TpTxConfirmationFunc(UpPduId, E_NOT_OK);

#endif
    }
}
#endif

#if (STD_ON == SOAD_SUPPORT_TX_BUFFER)

/**
 * @brief         Transmits a TP PDU on a specific socket connection.
 */
SOAD_LOCAL Std_ReturnType SoAd_TpTransmitSoCon(
    SoAd_SoConIdType                   SoConId,
    SoAd_InnerSocketConManageType*     soConMgmtPtr,
    SoAd_uintx                         PduRouteDestId,
    const PduInfoType*                 SoAdSrcPduInfoPtr,
    SoAd_InnerPduRouteDestManagerType* pduRouteDestMPtr)
{
    Std_ReturnType ret        = E_NOT_OK;
    SoAd_uintx     txBufferId = *SOAD_SOCKET_TXBUF_INDEX_CFG(SoConId);
#if (SOAD_PARTITION_NUM > 1u)
    uint16     partIndex = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(SoConId)].PartitionId);
    SoAd_uintx id        = txBufferId - SOAD_MULTI_PART_INFO.SocketTxBuffStartId;
    SoAd_SoTxBufManType* soTxBuffMgmtPtr = &SoAd_SoTxBuffManagerPtr[partIndex][id];
#else
    SoAd_SoTxBufManType* soTxBuffMgmtPtr = &SoAd_SoTxBuffManagerPtr[txBufferId];
#endif
    PduLengthType bufferLen = SoAd_SocketBufferCfgPtr[txBufferId].BuffLen;

    if ((bufferLen >= SoAdSrcPduInfoPtr->SduLength) || (0u == bufferLen))
    {
#if (STD_ON == SOAD_SUPPORT_HEADERID)
        const SoAd_PduRouteDestType* pduRouteDestCfgPtr = &SoAd_PduRouteDestCfgPtr[PduRouteDestId];
        if (NULL_PTR != pduRouteDestCfgPtr->TxPduHeaderId)
        {
            if (SOAD_TCP == SoAd_SoConCfgPtr[SoConId].TcpUdpProtocol)
            {
                uint8 tcpHeaderData[SOAD_HEADER_SIZE];
                SOAD_FILL_HEADER(&tcpHeaderData[0], pduRouteDestCfgPtr->TxPduHeaderId[0], SoAdSrcPduInfoPtr->SduLength);
                /*transmit success manage parameter set*/
                soConMgmtPtr->TxNoAckLength += SOAD_HEADER_SIZE;
                pduRouteDestMPtr->TxPendingLength = soConMgmtPtr->TxNoAckLength;
                soTxBuffMgmtPtr->TxPduTotalLength = SoAdSrcPduInfoPtr->SduLength + SOAD_HEADER_SIZE;
                soTxBuffMgmtPtr->UpCopyLength     = SOAD_HEADER_SIZE;
                soConMgmtPtr->TpPduTransmitting   = TRUE;
                ret = TcpIp_TcpTransmit(soConMgmtPtr->TcpIpSocketId, &tcpHeaderData[0], SOAD_HEADER_SIZE, TRUE);
                if (E_OK != ret)
                {
                    soConMgmtPtr->TxNoAckLength -= SOAD_HEADER_SIZE;
                    pduRouteDestMPtr->TxPendingLength = 0u;
                    soTxBuffMgmtPtr->TxPduTotalLength = 0u;
                    soTxBuffMgmtPtr->UpCopyLength     = 0u;
                    soConMgmtPtr->TpPduTransmitting   = FALSE;
                }
            }
            else
            {
                if (bufferLen >= (SoAdSrcPduInfoPtr->SduLength + SOAD_HEADER_SIZE))
                {
                    SOAD_FILL_HEADER(
                        &SoAd_PduBuffer[SoAd_SocketBufferCfgPtr[txBufferId].BuffPos],
                        pduRouteDestCfgPtr->TxPduHeaderId[0],
                        SoAdSrcPduInfoPtr->SduLength);
                    soTxBuffMgmtPtr->TxPduTotalLength = (SoAdSrcPduInfoPtr->SduLength + SOAD_HEADER_SIZE);
                    soTxBuffMgmtPtr->UpCopyLength     = SOAD_HEADER_SIZE;
                    soConMgmtPtr->TpPduTransmitting   = TRUE;
                    ret                               = E_OK;
                }
            }
        }
        else
#endif
        {
            soTxBuffMgmtPtr->TxPduTotalLength = SoAdSrcPduInfoPtr->SduLength;
            soTxBuffMgmtPtr->UpCopyLength     = 0u;
            soConMgmtPtr->TpPduTransmitting   = TRUE;
            ret                               = E_OK;
        }
    }
    return ret;
}
#endif

#if (STD_ON == SOAD_SUPPORT_UDP)
/**
 * @brief         Closes a UDP socket connection group.Called by SoAd_TcpIpEvent.
 */
SOAD_LOCAL void SoAd_CloseUdpSoConGroup(TcpIp_EventType Event, SoAd_uintx soConGroupId)
{
    if (NULL_PTR != SOAD_UDPTYPE_CFG(soConGroupId))
    {
        /*SWS_SoAd_00643:TCPIP_UDP_CLOSED for a udp socket connection group*/
        if (TCPIP_UDP_CLOSED == Event)
        {
            SoAd_SoConIdType                    soConCnt, soConId;
            SoAd_SoConIdType                    soceketConNum = SoAd_SoConGroupCfgPtr[soConGroupId].SocketConNum;
            SoAd_InnerSocketConManageType*      soConMgmtPtr;
            SoAd_InnerSocketConGroupManageType* soConGroupMgmtPtr;
#if (SOAD_PARTITION_NUM > 1u)
            uint16 partIndex   = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[soConGroupId].PartitionId);
            soConMgmtPtr       = SoAd_SoConManagerPtr[partIndex];
            SoAd_uintx groupId = SoAd_SoConGroupCfgPtr[soConGroupId].PartitionSoConGroupId;
            soConGroupMgmtPtr  = &SoAd_SoConGroupManagerPtr[partIndex][groupId];
#else
            soConGroupMgmtPtr = &SoAd_SoConGroupManagerPtr[soConGroupId];
#endif
            SoAd_SoConIdType soConStartId = SoAd_SoConGroupCfgPtr[soConGroupId].SocketConStartId;
            for (soConCnt = 0u; soConCnt < soceketConNum; soConCnt++)
            {
                soConId = soConStartId + soConCnt;
#if (SOAD_PARTITION_NUM > 1u)
                soConMgmtPtr = soConMgmtPtr + SoAd_SoConCfgPtr[soConId].PartitionSoConId;
#else
                soConMgmtPtr = &SoAd_SoConManagerPtr[soConId];
#endif
                soConMgmtPtr->TcpIpSocketValid = FALSE;
                if (SOAD_SOCON_ONLINE == soConMgmtPtr->SoConMode)
                {
                    SoAd_SoConCloseHandle(soConId, FALSE, soConMgmtPtr, soConGroupMgmtPtr);
                    /*Notification UP Module SoCon changed*/
                    soConMgmtPtr->SoConMode = SOAD_SOCON_RECONNECT;
                    SoAd_SoConModeChgNotificationAndTpStartOfReception(soConId, soConMgmtPtr, soConGroupId);
                }
                soConMgmtPtr->TcpIpSocketId = (TcpIp_SocketIdType)TCPIP_SOCKETID_INVALID;
            }

            soConGroupMgmtPtr->TcpIpSocketValid    = FALSE;
            soConGroupMgmtPtr->SoConSocketValidNum = 0u;
            soConGroupMgmtPtr->TcpIpSocketId       = (TcpIp_SocketIdType)TCPIP_SOCKETID_INVALID;
        }
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
        /*SWS_SoAd_00278*/
        else
        {
            SOAD_DET_REPORT(SOAD_SID_TCPIPEVENT, SOAD_E_INV_ARG);
        }
#endif
    }
}
#endif

/*Called by SoAd_TxConfirmation.
 *Tcp TxConfirmation handle*/
#if (STD_ON == SOAD_SUPPORT_TCP)
#if (SOAD_PDU_ROUTE_DEST_MAX > 0)
/**
 * @brief         Handles the TCP transmission confirmation.
 */
SOAD_LOCAL void SoAd_TxConfirmationHandle(SoAd_SoConIdType soConId, uint16 Length, SoAd_uintx pduRouteDestId)
{
    SoAd_InnerPduRouteDestManagerType* pduRouteDestMPtr;

#if (SOAD_PARTITION_NUM > 1u)
    uint16 partIndex = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(soConId)].PartitionId);
    pduRouteDestMPtr =
        &SoAd_PduRouteDestManagerPtr[partIndex][SoAd_PduRouteDestCfgPtr[pduRouteDestId].PartitionPduRouteDestId];
#else
    pduRouteDestMPtr = &SoAd_PduRouteDestManagerPtr[pduRouteDestId];
#endif

    if (pduRouteDestMPtr->TxPendingLength <= Length)
    {
        PduIdType                      pduRouteId = SoAd_PduRouteDestCfgPtr[pduRouteDestId].PduRouteIndex;
        SoAd_InnerSocketConManageType* soConMgmtPtr;
#if (SOAD_PARTITION_NUM > 1u)
        soConMgmtPtr = &SoAd_SoConManagerPtr[partIndex][SoAd_SoConCfgPtr[soConId].PartitionSoConId];
#else
        soConMgmtPtr = &SoAd_SoConManagerPtr[soConId];
#endif
        pduRouteDestMPtr->TxPendingLength = 0u;
#if (STD_ON == SOAD_SUPPORT_TX_BUFFER)
        if (SOAD_UPPER_LAYER_TP == SoAd_PduRouteCfgPtr[pduRouteId].TxUpperLayerType)
        {
            if (!soConMgmtPtr->TpPduTransmitting)
            {
                /*here is the really TxConfirmation for SoAdSocketTcpImmediateTpTxConfirmation*/
                SoAd_SocnTransmitAutomaticSoConSetupHandle(soConId, soConMgmtPtr, SOAD_TCP);
            }
            else
            {
                /*Tp Pdu TxConfirmation handle*/
                SoAd_uintx txBufferId = *SOAD_SOCKET_TXBUF_INDEX_CFG(soConId);
#if (SOAD_PARTITION_NUM > 1u)
                SoAd_uintx           id              = txBufferId - SOAD_MULTI_PART_INFO.SocketTxBuffStartId;
                SoAd_SoTxBufManType* soTxBuffMgmtPtr = &SoAd_SoTxBuffManagerPtr[partIndex][id];
#else
                SoAd_SoTxBufManType* soTxBuffMgmtPtr = &SoAd_SoTxBuffManagerPtr[txBufferId];
#endif
                if (soTxBuffMgmtPtr->UpCopyLength == soTxBuffMgmtPtr->TxPduTotalLength)
                {
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
                    SoAd_UpLayerModuleType upModule = SoAd_PduRouteCfgPtr[pduRouteId].RefUpModule;

                    SoAd_UpperLayerPCCfgPtr[upModule].TpTxConfirmationFunc(
                        SoAd_PduRouteCfgPtr[pduRouteId].UpPduId,
                        E_OK);

#endif
                    soTxBuffMgmtPtr->TxPduTotalLength = 0u;
                    soTxBuffMgmtPtr->UpCopyLength     = 0u;
                    soConMgmtPtr->TpPduTransmitting   = FALSE;
                    SoAd_SocnTransmitAutomaticSoConSetupHandle(soConId, soConMgmtPtr, SOAD_TCP);
                }
            }
        }
        /*If Pdu TxConfirmation handle*/
        else
#endif
        {
            SoAd_InnerPduRoutManageType* pduRouteMgmtPtr;
#if (SOAD_PARTITION_NUM > 1u)
            pduRouteMgmtPtr = &SoAd_PduRouteManagerPtr[partIndex][SoAd_PduRouteCfgPtr[pduRouteId].PartitionPduRouteId];
#else
            pduRouteMgmtPtr = &SoAd_PduRouteManagerPtr[pduRouteId];
#endif
            if (pduRouteMgmtPtr->TxPendingNum > 0u)
            {
                pduRouteMgmtPtr->TxPendingNum -= 1u;
                if (0u == pduRouteMgmtPtr->TxPendingNum)
                {
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
                    SoAd_UpLayerModuleType upModule = SoAd_PduRouteCfgPtr[pduRouteId].RefUpModule;

                    SoAd_UpperLayerPCCfgPtr[upModule].IfTxConfirmationFunc(
                        SoAd_PduRouteCfgPtr[pduRouteId].UpPduId,
                        E_OK);

#endif
                    SoAd_PduRouteManagerInit(pduRouteId);
                    SoAd_SocnTransmitAutomaticSoConSetupHandle(soConId, soConMgmtPtr, SOAD_TCP);
                }
            }
        }
    }
    else
    {
        pduRouteDestMPtr->TxPendingLength -= Length;
    }
}
#endif
#endif

#if (STD_ON == SOAD_SUPPORT_HEADERID)
/**
 * @brief         Finds the header ID and socket route ID.
 */
SOAD_LOCAL boolean
    SoAd_FindHeaderIDAndSocketRouteId(SoAd_SoConIdType SoConId, uint32 HeaderId, SoAd_uintx* SocketRouteID)
{
    SoAd_uintx                       soRouteId;
    boolean                          findValidHeaderId = FALSE;
    const SoAd_SocketConnectionType* soConCfgPtr       = &SoAd_SoConCfgPtr[SoConId];
    SoAd_uintx                       socketRouteNum    = soConCfgPtr->RefSocketRouteNum;

    for (SoAd_uintx cnt = 0u; cnt < socketRouteNum; cnt++)
    {
        soRouteId = soConCfgPtr->RefSocketRoutePtr[cnt];
        if (HeaderId == *SoAd_SocketRouteCfgPtr[soRouteId].RxPduHeaderId)
        {
            findValidHeaderId = TRUE;
            *SocketRouteID    = soRouteId;
            break;
        }
    }

    return findValidHeaderId;
}
#endif

/**
 * @brief     Match TCP socket ID
 */
SOAD_LOCAL boolean
    SoAd_MatchTCPSocketId(TcpIp_SocketIdType socketId, SoAd_SoConIdType* soConIdPtr, boolean tpPduTransmitting)
{
    boolean                              socketIdValid = FALSE;
    const SoAd_InnerSocketConManageType* soConMgmtPtr;
    boolean                              findFlag = FALSE;
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION) && (SOAD_PARTITION_NUM > 1)
    ApplicationType curAppId = GetApplicationID();
#endif

    for (SoAd_SoConIdType cnt = 0u; (cnt < SoAd_SoConNum) && (!findFlag); cnt++)
    {
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION) && (SOAD_PARTITION_NUM > 1u)
        uint16 partitionIdCfg = SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(cnt)].PartitionId;
        if (partitionIdCfg != curAppId)
        {
            continue;
        }
        uint16 partIndex = SoAd_GetPartitionIndex(partitionIdCfg);
        soConMgmtPtr     = &SoAd_SoConManagerPtr[partIndex][SoAd_SoConCfgPtr[cnt].PartitionSoConId];
#else
                soConMgmtPtr = &SoAd_SoConManagerPtr[cnt];
#endif
        if ((socketId == soConMgmtPtr->TcpIpSocketId) && (SOAD_SOCON_ONLINE == soConMgmtPtr->SoConMode)
            && (SOAD_TCP == SoAd_SoConCfgPtr[cnt].TcpUdpProtocol))
        {
            if ((!tpPduTransmitting) || (soConMgmtPtr->TpPduTransmitting))
            {
                socketIdValid = TRUE;
                *soConIdPtr   = cnt;
                findFlag      = TRUE;
            }
        }
    }
    return socketIdValid;
}
#if (STD_ON == SOAD_SUPPORT_TCP)
/**
 * @brief     Match TCP socket group socket ID
 */
SOAD_LOCAL boolean SoAd_MatchTCPSoConGroupSocketId(TcpIp_SocketIdType socketId, SoAd_uintx* soConGroupIdPtr)
{
    SoAd_uintx                                soConGroupId;
    boolean                                   soConGroupValid = FALSE;
    const SoAd_InnerSocketConGroupManageType* soConGroupMgmtPtr;
    const SoAd_SocketTcpType*                 soTcpCfgPtr;
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION) && (SOAD_PARTITION_NUM > 1u)
    ApplicationType curAppId = GetApplicationID();
#endif

    for (soConGroupId = 0u; soConGroupId < SOAD_SO_CON_GROUP_NUM; soConGroupId++)
    {
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION) && (SOAD_PARTITION_NUM > 1u)
        uint16 partitionIdCfg = SoAd_SoConGroupCfgPtr[soConGroupId].PartitionId;
        if (partitionIdCfg != curAppId)
        {
            continue;
        }
        uint16 partIndex = SoAd_GetPartitionIndex(partitionIdCfg);

        soConGroupMgmtPtr =
            &SoAd_SoConGroupManagerPtr[partIndex][SoAd_SoConGroupCfgPtr[soConGroupId].PartitionSoConGroupId];
#else
        soConGroupMgmtPtr = &SoAd_SoConGroupManagerPtr[soConGroupId];
#endif

        soTcpCfgPtr = SOAD_TCPTYPE_CFG(soConGroupId);
        if (soConGroupMgmtPtr->TcpIpSocketValid && (socketId == soConGroupMgmtPtr->TcpIpSocketId)
            && (NULL_PTR != soTcpCfgPtr) && !soTcpCfgPtr->SocketTcpInitiate)
        {
            soConGroupValid  = TRUE;
            *soConGroupIdPtr = soConGroupId;
            break;
        }
    }

    return soConGroupValid;
}
#endif

#if (STD_ON == SOAD_DEV_ERROR_DETECT)
/**
 * @brief     Check initialization status of the SoAd module.
 */
SOAD_LOCAL boolean SoAd_DetCheckInitStatus(uint8 apiId, ApplicationType* applicationIdPtr)
{
    boolean ret = TRUE;

#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
    uint16          idx;
    ApplicationType curAppId = GetApplicationID();
    for (idx = 0u; idx < SOAD_PARTITION_MAX; idx++)
    {
        if (SoAd_Partition[idx].ApplicationId == curAppId)
        {
            if (*SoAd_MultiPartitionInitStatusPtr[SoAd_Partition[idx].PartitionIndex] != SOAD_STATE_INIT)
            {
                SOAD_DET_REPORT(apiId, SOAD_E_NOTINIT);
                ret = FALSE;
            }
            break;
        }
    }

    if (idx >= SOAD_PARTITION_MAX)
    {
        SOAD_DET_REPORT(apiId, SOAD_E_INVALID_PARTITION);
        ret = FALSE;
    }
    if (ret && (NULL_PTR != applicationIdPtr))
    {
        *applicationIdPtr = curAppId;
    }
#else
    if (SoAd_InitStatus != SOAD_STATE_INIT)
    {
        SOAD_DET_REPORT(apiId, SOAD_E_NOTINIT);
        ret = FALSE;
    }

    if ((ret) && (applicationIdPtr))
    {
        *applicationIdPtr = 0u;
    }
#endif
    return ret;
}

/**
 * @brief     Check TX PDU ID
 */
SOAD_LOCAL boolean SoAd_DetCheckTxPduId(uint8 apiId, PduIdType txPduId, boolean checkULType, SoAd_UpperLayerType type)
{
    boolean ret = TRUE;
    if ((txPduId >= SOAD_PDU_ROUTE_NUM) || (checkULType && (type != SoAd_PduRouteCfgPtr[txPduId].TxUpperLayerType)))
    {
        SOAD_DET_REPORT(apiId, SOAD_E_INV_PDUID);
        ret = FALSE;
    }

    return ret;
}

/**
 * @brief     Check socket route destination ID
 */
SOAD_LOCAL boolean
    SoAd_DetCheckSocketRouteDestId(uint8 apiId, PduIdType pduId, boolean checkULType, SoAd_UpperLayerType type)
{
    boolean ret = TRUE;
    if ((pduId >= SOAD_SOCKET_ROUTE_DEST_NUM)
        || (checkULType && (type != SoAd_SoRouteDestCfgPtr[pduId].RxUpperLayerType)))
    {
        SOAD_DET_REPORT(apiId, SOAD_E_INV_PDUID);
        ret = FALSE;
    }

    return ret;
}

/**
 * @brief     Check routing group ID
 */
SOAD_LOCAL boolean SoAd_DetCheckRoutingGroupId(uint8 apiId, SoAd_RoutingGroupIdType routingGroupId)
{
    boolean ret = TRUE;
    if (routingGroupId >= SOAD_ROUTING_GROUP_NUM)
    {
        SOAD_DET_REPORT(apiId, SOAD_E_INV_ARG);
        ret = FALSE;
    }
    return ret;
}

/**
 * @brief     Check if routing group is transmit triggerable
 */
SOAD_LOCAL boolean SoAd_DetCheckRoutingGroupTxTriggerable(uint8 apiId, SoAd_RoutingGroupIdType routingGroupId)
{
    boolean ret = TRUE;
    if (!SOAD_ROUTGROUP_TXTRIG(routingGroupId))
    {
        SOAD_DET_REPORT(apiId, SOAD_E_INV_ARG);
        ret = FALSE;
    }
    return ret;
}

#if (STD_ON == SOAD_DEV_ERROR_DETECT)
/**
 * @brief     Check socket connection ID
 */
SOAD_LOCAL boolean SoAd_DetCheckSoConId(uint8 apiId, SoAd_SoConIdType soConId)
{
    boolean ret = TRUE;
    if (soConId >= SoAd_SoConNum)
    {
        SOAD_DET_REPORT(apiId, SOAD_E_INV_ARG);
        ret = FALSE;
    }
    return ret;
}
/**
 * @brief Checks if automatic socket connection setup is enabled for a given socket connection.
 */
SOAD_LOCAL boolean SoAd_DetCheckAutoSoConSetup(uint8 apiId, SoAd_SoConIdType soConId)
{
    boolean ret = TRUE;

    if (SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(soConId)].SocketAutomaticSoConSetup)
    {
        SOAD_DET_REPORT(apiId, SOAD_E_INV_ARG);
        ret = FALSE;
    }
    return ret;
}

/**
 * @brief Checks if a pointer parameter is valid (not NULL).
 */
SOAD_LOCAL boolean SoAd_DetCheckPtrParameter(uint8 apiId, const void* parameterPtr)
{
    boolean ret = TRUE;

    if (NULL_PTR == parameterPtr)
    {
        SOAD_DET_REPORT(apiId, SOAD_E_INV_ARG);
        ret = FALSE;
    }
    return ret;
}

/**
 * @brief Checks if a length parameter is valid (not zero).
 */
SOAD_LOCAL boolean SoAd_DetCheckLength(uint8 apiId, uint16 length)
{
    boolean ret = TRUE;

    if (0u == length)
    {
        SOAD_DET_REPORT(apiId, SOAD_E_INV_ARG);
        ret = FALSE;
    }
    return ret;
}

/**
 * @brief Checks if the length of data to be transmitted without acknowledgment is valid.
 */
SOAD_LOCAL boolean SoAd_DetCheckTCPTxNoAckLength(uint8 apiId, uint16 length, PduLengthType txNoAckLength)
{
    boolean ret = TRUE;

    if (txNoAckLength < length)
    {
        SOAD_DET_REPORT(apiId, SOAD_E_INV_ARG);
        ret = FALSE;
    }
    return ret;
}

/**
 * @brief Checks if a TCP event is valid.
 */
SOAD_LOCAL boolean SoAd_DetCheckTCPEvent(uint8 apiId, TcpIp_EventType Event)
{
    boolean ret = TRUE;

    if (Event > TCPIP_TLS_HANDSHAKE_SUCCEEDED)
    {
        SOAD_DET_REPORT(apiId, SOAD_E_INV_ARG);
        ret = FALSE;
    }
    return ret;
}

/**
 * @brief Checks if a socket ID is valid.
 */
SOAD_LOCAL boolean SoAd_DetCheckSocketIdValid(uint8 apiId, boolean socketIdValid)
{
    boolean ret = TRUE;

    if (!socketIdValid)
    {
        SOAD_DET_REPORT(apiId, SOAD_E_INV_SOCKETID);
        ret = FALSE;
    }
    return ret;
}

/**
 * @brief Checks if the domain type of a socket is valid.
 */
SOAD_LOCAL boolean SoAd_DetCheckTcpIpDomainType(uint8 apiId, TcpIp_DomainType domainType, SoAd_SoConIdType soConId)
{
    boolean ret = TRUE;

    if (domainType != SOAD_SOCKETLOCALADDR_CFG(SOAD_SOCON_REFGORUP(soConId)).DomainType)
    {
        SOAD_DET_REPORT(apiId, SOAD_E_INV_ARG);
        ret = FALSE;
    }
    return ret;
}

/**
 * @brief Checks if the partition of a transmit PDU ID is valid.
 */
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
SOAD_LOCAL boolean SoAd_DetCheckTxPduIdPartition(uint8 apiId, PduIdType txPduId, ApplicationType curAppId)
{
    boolean ret = TRUE;

    if (SoAd_PduRouteCfgPtr[txPduId].PartitionId != curAppId)
    {
        SOAD_DET_REPORT(apiId, SOAD_E_INVALID_PARTITION);
        ret = FALSE;
    }

    return ret;
}

/**
 * @brief Checks if the partition of a receive PDU ID is valid.
 */
SOAD_LOCAL boolean SoAd_DetCheckRxPduIdPartition(uint8 apiId, PduIdType pduId, ApplicationType curAppId)
{
    boolean ret = TRUE;

    if (SoAd_SocketRouteCfgPtr[pduId].PartitionId != curAppId)
    {
        SOAD_DET_REPORT(apiId, SOAD_E_INVALID_PARTITION);
        ret = FALSE;
    }

    return ret;
}

/**
 * @brief Checks if the partition of a routing group ID is valid.
 */
SOAD_LOCAL boolean
    SoAd_DetCheckRoutingGroupIdPartition(uint8 apiId, SoAd_RoutingGroupIdType routingGroupId, ApplicationType curAppId)
{
    boolean ret = TRUE;

    if (SoAd_RouteGroupCfgPtr[routingGroupId].PartitionId != curAppId)
    {
        SOAD_DET_REPORT(apiId, SOAD_E_INVALID_PARTITION);
        ret = FALSE;
    }

    return ret;
}
#endif

/**
 * @brief Checks if the partition of a socket connection group ID is valid.
 */
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
SOAD_LOCAL boolean SoAd_DetCheckSoConIdPartition(uint8 apiId, SoAd_uintx soConGroupId, ApplicationType curAppId)
{
    boolean ret = TRUE;

    if (SoAd_SoConGroupCfgPtr[soConGroupId].PartitionId != curAppId)
    {
        SOAD_DET_REPORT(apiId, SOAD_E_INVALID_PARTITION);
        ret = FALSE;
    }

    return ret;
}
#endif
#endif
#endif

#if (STD_ON == SOAD_PDU_ROUTE_METADATA_SUPPORT)
SOAD_LOCAL boolean SoAd_MatchMetaDataSoConId(PduIdType txPduId, SoAd_uintx pduRouteDestId, SoAd_SoConIdType txSoConId)
{
    boolean ret = TRUE;
    if (SoAd_PduRouteCfgPtr[txPduId].MetaDataEnable)
    {
        /*SWS_SoAd_00738 */
        const SoAd_PduRouteDestType* pduRouteDestCfgPtr = &SoAd_PduRouteDestCfgPtr[pduRouteDestId];
        SoAd_SoConIdType             refNum             = pduRouteDestCfgPtr->TxPduRouteDestSocketConRefNum;
        if (refNum > 1u)
        {
            SoAd_SoConIdType conId;
            for (conId = 0u; conId < refNum; conId++)
            {
                if (txSoConId == pduRouteDestCfgPtr->TxSocketConRefIndexPtr[conId])
                {
                    conId = 0u;
                    break;
                }
            }

            if (conId >= refNum)
            {
                ret = FALSE;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
                SOAD_DET_REPORT(SOAD_SID_IFTRANSMIT, SOAD_E_INV_METADATA);
#endif
            }
        }
    }

    return ret;
}
#endif
#if (STD_ON == SOAD_SUPPORT_TCP)
SOAD_LOCAL void SoAd_HandleTcpIpEventTCP(
    SoAd_SoConIdType               soConId,
    TcpIp_EventType                Event,
    SoAd_InnerSocketConManageType* soConMgmtPtr)
{
    /*Adjust the standard SWS_SoAd_00646*/
    if (SOAD_SOCON_OFFLINE != soConMgmtPtr->SoConMode)
    {
        SoAd_uintx                          soConGroupId = SOAD_SOCON_REFGORUP(soConId);
        SoAd_InnerSocketConGroupManageType* soConGroupMgmtPtr;
#if (SOAD_PARTITION_NUM > 1u)
        uint16 partIndex = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(soConId)].PartitionId);
        soConGroupMgmtPtr =
            &SoAd_SoConGroupManagerPtr[partIndex][SoAd_SoConGroupCfgPtr[soConGroupId].PartitionSoConGroupId];
#else
        soConGroupMgmtPtr = &SoAd_SoConGroupManagerPtr[soConGroupId];
#endif

        if (TCPIP_TCP_FIN_RECEIVED == Event)
        {
            if (soConMgmtPtr->SocketNeedClose != SOAD_SOCKET_CLOSE_DEFERRED)
            {
#if (STD_ON == SOAD_SUPPORT_RX_BUFFER)
                if (SoAd_CheckRxTpPduDataHandle(soConId))
                {
                    /*rx tp data not copy to UL, need delay close socket*/
                    soConMgmtPtr->SocketNeedClose = SOAD_SOCKET_CLOSE_DEFERRED;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
                    soConMgmtPtr->SetSocketNeedCloseLine = __LINE__;
#endif
                }
                else
#endif
                {
                    SoAd_SoConCloseHandle(soConId, TRUE, soConMgmtPtr, soConGroupMgmtPtr);
                }
            }
        }
        else
        {
            soConMgmtPtr->TcpIpSocketValid = FALSE;
            soConMgmtPtr->TcpIpSocketId    = (TcpIp_SocketIdType)TCPIP_SOCKETID_INVALID;
            SoAd_SoConCloseHandle(soConId, FALSE, soConMgmtPtr, soConGroupMgmtPtr);
            if (0u < soConGroupMgmtPtr->SoConSocketValidNum)
            {
                (soConGroupMgmtPtr->SoConSocketValidNum) -= 1u;
            }
        }
        /*Notification UP Module SoCon changed*/
        if ((SOAD_SOCON_ONLINE == soConMgmtPtr->SoConMode) && (SOAD_SOCKET_CLOSE_NONE == soConMgmtPtr->SocketNeedClose))
        {
            if (soConMgmtPtr->ChangeModeToOffline)
            {
                soConMgmtPtr->SoConMode           = SOAD_SOCON_OFFLINE;
                soConMgmtPtr->ChangeModeToOffline = FALSE;
            }
            else
            {
                soConMgmtPtr->SoConMode = SOAD_SOCON_RECONNECT;
            }
            SoAd_SoConModeChgNotificationAndTpStartOfReception(soConId, soConMgmtPtr, soConGroupId);
        }
        /*server-side,think about close the"listen socket"*/
        if (!SOAD_TCPTYPE_CFG(soConGroupId)->SocketTcpInitiate && soConGroupMgmtPtr->TcpIpSocketValid
            && (0u == soConGroupMgmtPtr->SoConSocketValidNum))
        {
            (void)TcpIp_Close(soConGroupMgmtPtr->TcpIpSocketId, soConMgmtPtr->SoConCloseAbort);
        }
    }
    else
    {
        soConMgmtPtr->TcpIpSocketValid = FALSE;
    }
}
#endif

#if (STD_ON == SOAD_SUPPORT_HEADERID) && (0u < SOAD_SOCKET_ROUTE_DEST_MAX)
SOAD_LOCAL boolean SoAd_HandleRxIndicationEnableHeader(
    SoAd_SoConIdType               soConId,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    uint8*                         bufPtr,
    uint16                         length)
{
    boolean validPduData = TRUE;

    if (SOAD_TCP == SoAd_SoConCfgPtr[soConId].TcpUdpProtocol)
    {
#if (STD_ON == SOAD_SUPPORT_RX_BUFFER)
        SoAd_HeaderEnableTcpSoConRxHandle(soConId, soConMgmtPtr, length, bufPtr);
#endif
    }
#if (STD_ON == SOAD_SUPPORT_UDP)
    /*header enable and UDP socket connection*/
    else
    {
        validPduData = SoAd_HeaderEnableUdpSoConRxHandle(soConId, soConMgmtPtr, length, bufPtr);
    }
#endif

    return validPduData;
}
#endif

#if (0u < SOAD_SOCKET_ROUTE_DEST_MAX)
SOAD_LOCAL boolean SoAd_HandleRxIndicationDisableHeader(
    SoAd_SoConIdType               soConId,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    uint8*                         bufPtr,
    uint16                         length)
{
    boolean validPduData = TRUE;

    /*Header is disabled,just one socket route ref the SoCon*/
    if (NULL_PTR != SoAd_SoConCfgPtr[soConId].RefSocketRoutePtr)
    {
        SoAd_uintx socketRouteId = SoAd_SoConCfgPtr[soConId].RefSocketRoutePtr[0u];
#if (SOAD_PARTITION_NUM > 1u)
        uint16     partIndex  = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(soConId)].PartitionId);
        SoAd_uintx startId    = SoAd_SoRouteDestCfgPtr[socketRouteId].PartitionSocketRouteDestId;
        boolean    destEnable = SoAd_SoRouteDestManagerPtr[partIndex][startId].SocketRouteDestEnable;
#else
        boolean destEnable = SoAd_SoRouteDestManagerPtr[socketRouteId].SocketRouteDestEnable;
#endif
        if (SOAD_UPPER_LAYER_TP == SoAd_SoRouteDestCfgPtr[socketRouteId].RxUpperLayerType)
        {
#if (STD_ON == SOAD_SUPPORT_RX_BUFFER)
            if (destEnable)
            {
                SoAd_HeaderDisableSoConRxTpPduHandle(soConId, soConMgmtPtr, socketRouteId, length, bufPtr);
            }
#if (STD_ON == SOAD_SUPPORT_UDP)
            else
            {
                validPduData = FALSE;
            }
#endif
#endif
        }
        else
        {
            /*Skip further processing if PDU length is 0 and (SoAdPduHeaderEnable is FALSE or
             * SoAdRxUpperLayerType is TP)*/
            if (destEnable)
            {
                SoAd_HeaderDisableSoConRxIfPduHandle(soConId, socketRouteId, length, bufPtr);
            }
#if (STD_ON == SOAD_SUPPORT_UDP)
            else
            {
                validPduData = FALSE;
            }
#endif
        }
    }

    return validPduData;
}
#endif

#if (STD_ON == SOAD_SUPPORT_UDP)
SOAD_LOCAL boolean SoAd_FindUdpValidSoCon(
    SoAd_uintx                  soConGroupId,
    const TcpIp_SockAddrType*   RemoteAddrPtr,
    SoAd_FindTheValidSoConType* UdpParaPtr,
    SoAd_SoConIdType*           soConIdPtr)
{
    SoAd_InnerSocketConManageType* soConMgmtPtr;
    SoAd_SoConIdType               tempSoConId;
    boolean                        socketIdValid = FALSE;

    if (SoAd_SoConGroupCfgPtr[soConGroupId].SocketMsgAcceptanceFilterEnabled)
    {
        /*SWS_SoAd_00657:Best Match Algorithm*/
        if (SoAd_GetBestMatchAlgorithmSoCon(
                soConGroupId,
                RemoteAddrPtr,
                &tempSoConId,
                SOAD_SOCON_MATCH_REQUEST_ONLINE_RECONNECT))
        {
            const SoAd_SocketUdpType* soUdpCfgPtr = SOAD_UDPTYPE_CFG(soConGroupId);
#if (SOAD_PARTITION_NUM > 1u)
            uint16 partIndex = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[soConGroupId].PartitionId);
            soConMgmtPtr     = &SoAd_SoConManagerPtr[partIndex][SoAd_SoConCfgPtr[tempSoConId].PartitionSoConId];
#else
            soConMgmtPtr = &SoAd_SoConManagerPtr[tempSoConId];
#endif

            UdpParaPtr->RemoteAddr = soConMgmtPtr->RemoteAddr;
            /*SWS_SoAd_00592:(a) overwrite the remote address parts
            specified with wildcards with the related source address
            parts of the received message and (b) change the state of
                the socket connection to SOAD_SOCON_ONLINE */
            if ((SOAD_SOCON_ONLINE != soConMgmtPtr->SoConMode) && !soUdpCfgPtr->SocketUdpListenOnly)
            {
                UdpParaPtr->RemoteAddrChanged = TRUE;
                soConMgmtPtr->RemoteAddr      = *RemoteAddrPtr;
                soConMgmtPtr->SoConMode       = SOAD_SOCON_ONLINE;
                SoAd_SoConModeChgNotificationAndTpStartOfReception(tempSoConId, soConMgmtPtr, soConGroupId);
            }
            /*Start the UdpAliveSupervisionTime*/
            if (UdpParaPtr->RemoteAddrChanged)
            {
                if ((SOAD_UNUSED_UINT32 != soUdpCfgPtr->SocketUdpAliveSupervisionTimeout)
                    && (SOAD_IS_IPADDR_ANY(*SOAD_SOCONREMOTEADDR_CFG(tempSoConId))
                        || (TCPIP_PORT_ANY == SOAD_SOCONREMOTEADDR_CFG(tempSoConId)->port)))
                {
                    soConMgmtPtr->UdpAliveSupervisionTime = soUdpCfgPtr->SocketUdpAliveSupervisionTimeout;
                }
            }
            else
            {
                if (0uL < soConMgmtPtr->UdpAliveSupervisionTime)
                {
                    soConMgmtPtr->UdpAliveSupervisionTime = soUdpCfgPtr->SocketUdpAliveSupervisionTimeout;
                }
            }
            *soConIdPtr   = tempSoConId;
            socketIdValid = TRUE;
        }
    }
    else
    {
        tempSoConId = SoAd_SoConGroupCfgPtr[soConGroupId].SocketConStartId;
#if (SOAD_PARTITION_NUM > 1u)
        uint16 partIndex = SoAd_GetPartitionIndex(SoAd_SoConGroupCfgPtr[soConGroupId].PartitionId);
        soConMgmtPtr     = &SoAd_SoConManagerPtr[partIndex][SoAd_SoConCfgPtr[tempSoConId].PartitionSoConId];
#else
        soConMgmtPtr = &SoAd_SoConManagerPtr[tempSoConId];
#endif
        if (soConMgmtPtr->SoConMode == SOAD_SOCON_ONLINE)
        {
            *soConIdPtr = tempSoConId;
        }
        socketIdValid = TRUE;
    }

    return socketIdValid;
}
#endif

#if (STD_ON == SOAD_SUPPORT_TCP)
SOAD_LOCAL void SoAd_OpenTCPSoConHandle(
    SoAd_SoConIdType                    soConId,
    SoAd_uintx                          soConGroup,
    SoAd_InnerSocketConManageType*      soConMgmtPtr,
    SoAd_InnerSocketConGroupManageType* soConGroupMgmtPtr)
{
    if (SOAD_TCP == SoAd_SoConCfgPtr[soConId].TcpUdpProtocol)
    {
        /*SWS_SoAd_00590*/
        if (SOAD_TCPTYPE_CFG(soConGroup)->SocketTcpInitiate)
        {
            if (!soConMgmtPtr->TcpIpSocketValid)
            {
                SoAd_OpenTcpClientSoConHandle(soConId, soConGroup, soConMgmtPtr);
            }
        }
        /*SWS_SoAd_00638*/
        else
        {
            SoAd_OpenTcpServerSoConHandle(soConId, soConGroup, soConMgmtPtr, soConGroupMgmtPtr);
        }
    }
}
#endif

#if (STD_ON == SOAD_SUPPORT_UDP)
SOAD_LOCAL void SoAd_AttemptOpenUDPSoCon(
    SoAd_SoConIdType                    soConId,
    SoAd_uintx                          soConGroup,
    SoAd_InnerSocketConManageType*      soConMgmtPtr,
    SoAd_InnerSocketConGroupManageType* soConGroupMgmtPtr)
{
    if (SOAD_UDP == SoAd_SoConCfgPtr[soConId].TcpUdpProtocol)
    {
        /*SWS_SoAd_00639*/
        if (!soConGroupMgmtPtr->TcpIpSocketValid)
        {
            SoAd_OpenUdpSoConHandle(soConId, soConMgmtPtr, soConGroup, soConGroupMgmtPtr);
        }
        /*Activate the socket connection for communication */
        else
        {
            if (!soConMgmtPtr->TcpIpSocketValid)
            {
                soConMgmtPtr->TcpIpSocketValid = TRUE;
                soConMgmtPtr->TcpIpSocketId    = soConGroupMgmtPtr->TcpIpSocketId;
                soConGroupMgmtPtr->SoConSocketValidNum += 1u;
            }
            /*SWS_SoAd_00591*/
            if (SOAD_UDPTYPE_CFG(soConGroup)->SocketUdpListenOnly
                || ((!SOAD_IS_IPADDR_ANY(soConMgmtPtr->RemoteAddr))
                    && (TCPIP_PORT_ANY != soConMgmtPtr->RemoteAddr.port)))
            {
                soConMgmtPtr->SoConMode = SOAD_SOCON_ONLINE;
                SoAd_SoConModeChgNotificationAndTpStartOfReception(soConId, soConMgmtPtr, soConGroup);
            }
            /*SWS_SoAd_00686*/
            else
            {
                if (SOAD_SOCON_RECONNECT != soConMgmtPtr->SoConMode)
                {
                    soConMgmtPtr->SoConMode = SOAD_SOCON_RECONNECT;
                    SoAd_SoConModeChgNotificationAndTpStartOfReception(soConId, soConMgmtPtr, soConGroup);
                }
            }
        }
    }
}
#endif

/**
 * close socket connection socket
 */
SOAD_LOCAL void SoAd_CloseSoConTcpIpSocket(
    SoAd_SoConIdType                    soConId,
    boolean                             closeSocket,
    SoAd_InnerSocketConGroupManageType* soConGroupMgmtPtr,
    SoAd_InnerSocketConManageType*      soConMgmtPtr)
{
    const SoAd_SocketConnectionType* soConCfgPtr = &SoAd_SoConCfgPtr[soConId];
#if (STD_ON == SOAD_SUPPORT_TCP)
    /*close the tcpip socket*/
    if (SOAD_TCP == soConCfgPtr->TcpUdpProtocol)
    {
        if (closeSocket)
        {
            if (soConMgmtPtr->TcpIpSocketValid)
            {
                (void)TcpIp_Close(soConMgmtPtr->TcpIpSocketId, soConMgmtPtr->SoConCloseAbort);
            }
            else
            {
                if (soConMgmtPtr->ChangeModeToOffline)
                {
                    soConMgmtPtr->SoConMode           = SOAD_SOCON_OFFLINE;
                    soConMgmtPtr->ChangeModeToOffline = FALSE;
                    SoAd_uintx soConGroup             = SOAD_SOCON_REFGORUP(soConId);
                    SoAd_SoConModeChgNotificationAndTpStartOfReception(soConId, soConMgmtPtr, soConGroup);
                }
                if ((soConGroupMgmtPtr->TcpIpSocketValid) && (soConGroupMgmtPtr->SoConSocketValidNum == 0u))
                {
                    (void)TcpIp_Close(soConGroupMgmtPtr->TcpIpSocketId, soConMgmtPtr->SoConCloseAbort);
                }
            }
        }
    }
#endif
#if (STD_ON == SOAD_SUPPORT_UDP)
    if (SOAD_UDP == soConCfgPtr->TcpUdpProtocol)
    {
        if ((0u < soConGroupMgmtPtr->SoConSocketValidNum) && soConMgmtPtr->TcpIpSocketValid)
        {
            (soConGroupMgmtPtr->SoConSocketValidNum) -= 1u;
        }
        if (0u == soConGroupMgmtPtr->SoConSocketValidNum)
        {
            if (closeSocket)
            {
                (void)TcpIp_Close(soConGroupMgmtPtr->TcpIpSocketId, soConMgmtPtr->SoConCloseAbort);
            }
        }
    }
#endif
}

SOAD_LOCAL Std_ReturnType
    SoAd_RoutingGroupControlSpcificSoCon(SoAd_RoutingGroupIdType id, boolean enFlag, SoAd_SoConIdType soConId)
{
    Std_ReturnType                   result = E_NOT_OK;
    SoAd_uintx                       routeDestId;
    SoAd_RoutingGroupIdType          routeGroupId, routeGroupCnt;
    boolean                          flagSet;
    const SoAd_SocketConnectionType* soConCfgPtr = &SoAd_SoConCfgPtr[soConId];
#if (SOAD_PARTITION_NUM > 1u)
    uint16 partIndex = SoAd_GetPartitionIndex(SoAd_RouteGroupCfgPtr[id].PartitionId);
#endif
#if (0u < SOAD_SOCKET_ROUTE_DEST_MAX)
    const SoAd_SocketRouteDestType* socketRouteDestCfgPtr;
    SoAd_RoutingGroupIdType         rxRouteGroupRefNum;
    SoAd_uintx                      refSocketRouteNum = soConCfgPtr->RefSocketRouteNum;

    for (SoAd_uintx cnt = 0u; cnt < refSocketRouteNum; cnt++)
    {
        routeDestId           = soConCfgPtr->RefSocketRoutePtr[cnt];
        socketRouteDestCfgPtr = &SoAd_SoRouteDestCfgPtr[routeDestId];
        flagSet               = FALSE;
        rxRouteGroupRefNum    = socketRouteDestCfgPtr->RxRoutingGroupRefNum;
        for (routeGroupCnt = 0u; (routeGroupCnt < rxRouteGroupRefNum) && (!flagSet); routeGroupCnt++)
        {
            routeGroupId = socketRouteDestCfgPtr->RxRoutingGroupRefPtr[routeGroupCnt];
            if (id == routeGroupId)
            {
#if (SOAD_PARTITION_NUM > 1u)
                SoAd_uintx startId = socketRouteDestCfgPtr->PartitionSocketRouteDestId;
                SoAd_SoRouteDestManagerPtr[partIndex][startId].SocketRouteDestEnable = enFlag;
#else
                SoAd_SoRouteDestManagerPtr[routeDestId].SocketRouteDestEnable = enFlag;
#endif
                flagSet = TRUE;
                result  = E_OK;
            }
        }
    }

#endif
#if (0u < SOAD_PDU_ROUTE_DEST_MAX)
    const SoAd_PduRouteDestType* pduRouteDestCfgPtr;
    SoAd_RoutingGroupIdType      txRoutGroupRefNum;
    SoAd_uintx                   refPduRouteDestNum = soConCfgPtr->RefPduRouteDestNum;

    for (SoAd_uintx cnt = 0u; cnt < refPduRouteDestNum; cnt++)
    {
        flagSet            = FALSE;
        routeDestId        = soConCfgPtr->RefPduRouteDestPtr[cnt];
        pduRouteDestCfgPtr = &SoAd_PduRouteDestCfgPtr[routeDestId];
        txRoutGroupRefNum  = pduRouteDestCfgPtr->TxRoutingGroupRefNum;
        for (routeGroupCnt = 0u; (routeGroupCnt < txRoutGroupRefNum) && (!flagSet); routeGroupCnt++)
        {
            routeGroupId = pduRouteDestCfgPtr->TxRoutingGroupRefPtr[routeGroupCnt];
            if (id == routeGroupId)
            {
#if (SOAD_PARTITION_NUM > 1u)
                SoAd_uintx startId = pduRouteDestCfgPtr->PartitionPduRouteDestId;
                SoAd_PduRouteDestManagerPtr[partIndex][startId].PduRouteDestEnable = enFlag;
#else
                SoAd_PduRouteDestManagerPtr[routeDestId].PduRouteDestEnable = enFlag;

#endif

                flagSet = TRUE;
                result  = E_OK;
            }
        }
    }
#endif

    return result;
}

#if (STD_ON == SOAD_SUPPORT_TCP)
SOAD_LOCAL void SoAd_CloseTcpSoConGroup(
    TcpIp_EventType                     event,
    SoAd_uintx                          soConGroupId,
    SoAd_InnerSocketConGroupManageType* soConGroupMgmtPtr)
{
    if (NULL_PTR != SOAD_TCPTYPE_CFG(soConGroupId))
    {
        if ((TCPIP_TCP_CLOSED == event) || (TCPIP_TCP_RESET == event))
        {
            /*SWS_SoAd_00645:TCPIP_TCP_CLOSED for a Listen-Socket*/
            soConGroupMgmtPtr->TcpIpSocketValid = FALSE;
        }
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
        /*SWS_SoAd_00278*/
        else
        {
            SOAD_DET_REPORT(SOAD_SID_TCPIPEVENT, SOAD_E_INV_ARG);
        }
#endif
    }
}
#endif

#if (STD_ON == SOAD_SUPPORT_RX_BUFFER)
#if (STD_ON == SOAD_SUPPORT_HEADERID)
SOAD_LOCAL void SoAd_RxTpPduHeaderEnableHandle(
    PduLengthType                  pduLength,
    SoAd_SoConIdType               soConId,
    SoAd_uintx                     socketRouteId,
    SoAd_InnerSocketConManageType* soConMgmtPtr,
    SoAd_SoRxBufferManType*        soRxBuffMgmtPtr)
{
    if (pduLength > 0u)
    {
        PduInfoType pduInfo;
        pduInfo.SduDataPtr  = NULL_PTR;
        pduInfo.SduLength   = 0u;
        pduInfo.MetaDataPtr = NULL_PTR;
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
        uint8 metaData[2u]; /* PRQA S 3132 */ /* VL_QAC_MagicNum */
        pduInfo.MetaDataPtr =
            SoAd_MetaDataFillHandle(soConId, SoAd_SoRouteDestCfgPtr[socketRouteId].MetaDataEnable, &metaData[0u]);
#endif

#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
        SoAd_UpLayerModuleType upModule = SoAd_SoRouteDestCfgPtr[socketRouteId].RefUpModule;
        PduLengthType          upBufferSize;
        if (BUFREQ_OK
            == SoAd_UpperLayerPCCfgPtr[upModule].TpStartOfReceptionFunc(
                SoAd_SoRouteDestCfgPtr[socketRouteId].UpRxPduId,
                &pduInfo,
                pduLength,
                &upBufferSize))
        {
            SoAd_HeaderEnableTpPduHandle(soConId, soConMgmtPtr, upBufferSize, soRxBuffMgmtPtr);
        }
        else
        {
            soConMgmtPtr->SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
            soConMgmtPtr->SetSocketNeedCloseLine = __LINE__;
#endif
        }
#endif
    }
}
#endif
#endif

/**
 * @brief         Initialize socket connection manager.
 */
SOAD_LOCAL void SoAd_SoConManagerInit(void)
{
    SoAd_uintx                     soConGroupId;
    SoAd_InnerSocketConManageType* soConMgmtPtr;
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION) && (SOAD_PARTITION_NUM > 1u)
    ApplicationType curAppId = GetApplicationID();
#endif

    for (SoAd_SoConIdType cnt = 0u; cnt < SoAd_SoConNum; cnt++)
    {
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION) && (SOAD_PARTITION_NUM > 1u)
        uint16 partitionIdCfg = SoAd_SoConGroupCfgPtr[SOAD_SOCON_REFGORUP(cnt)].PartitionId;
        if (partitionIdCfg != curAppId)
        {
            continue;
        }
        uint16 partIndex = SoAd_GetPartitionIndex(partitionIdCfg);
        soConMgmtPtr     = &SoAd_SoConManagerPtr[partIndex][SoAd_SoConCfgPtr[cnt].PartitionSoConId];
#else
                soConMgmtPtr = &SoAd_SoConManagerPtr[cnt];
#endif
        soConMgmtPtr->SoConMode           = SOAD_SOCON_OFFLINE;
        soConMgmtPtr->ChangeModeToOffline = FALSE;
#if (STD_ON == SOAD_SUPPORT_UDP)
        soConMgmtPtr->UdpAliveSupervisionTime = 0u;
#endif
        /*init the socket connection's remote address to configuration parameter*/
        soConGroupId                    = SOAD_SOCON_REFGORUP(cnt);
        soConMgmtPtr->RemoteAddr.domain = SOAD_SOCKETLOCALADDR_CFG(soConGroupId).DomainType;
        if (NULL_PTR == SOAD_SOCONREMOTEADDR_CFG(cnt))
        {
            soConMgmtPtr->RemoteAddrIsSet = FALSE;
            TCPIP_SET_IPADDR_ANY(soConMgmtPtr->RemoteAddr);
            soConMgmtPtr->RemoteAddr.port = TCPIP_PORT_ANY;
        }
        else
        {
            soConMgmtPtr->RemoteAddrIsSet = TRUE;
            soConMgmtPtr->RemoteAddr      = *SOAD_SOCONREMOTEADDR_CFG(cnt);
        }
        /*init socket connection's Open/Close request*/
        if (SoAd_SoConGroupCfgPtr[soConGroupId].SocketAutomaticSoConSetup)
        {
            soConMgmtPtr->SoConRequest[0] = SOAD_SOCON_REQUEST_OPEN;
        }
        else
        {
            soConMgmtPtr->SoConRequest[0] = SOAD_SOCON_REQUEST_NONE;
        }
        soConMgmtPtr->SoConRequest[1] = SOAD_SOCON_REQUEST_NONE;
        /*other parameter init of SoAd_SoConManager*/
        soConMgmtPtr->ActiveSoAdPduRouteId = SOAD_PDUID_INVALID;
        soConMgmtPtr->SoConCloseAbort      = FALSE;
        soConMgmtPtr->SocketNeedClose      = SOAD_SOCKET_CLOSE_NONE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
        soConMgmtPtr->SetSocketNeedCloseLine = __LINE__;
#endif
        soConMgmtPtr->TcpIpSocketValid      = FALSE;
        soConMgmtPtr->TcpIpSocketId         = (TcpIp_SocketIdType)TCPIP_SOCKETID_INVALID;
        soConMgmtPtr->TpPduTransmitting     = FALSE;
        soConMgmtPtr->TxNoAckLength         = 0u;
        soConMgmtPtr->RemoteAddrNeedRelease = FALSE;
        soConMgmtPtr->RxIfProcessing        = FALSE;
    }
}

#define SOAD_STOP_SEC_CODE
#include "SoAd_MemMap.h"
