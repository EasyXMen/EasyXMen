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
 **  @file               : TcpIp_Internal.h
 **  @author             : darren.zhang
 **  @date               : 2018/12/03
 **  @vendor             : isoft
 **  @description        : TcpIp header file for internal API declarations
 **
 ***********************************************************************************************************************/

#ifndef TCPIP_INTERNAL_H_
#define TCPIP_INTERNAL_H_

/* =================================================== inclusions =================================================== */
#include "lwip/opt.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/netif.h"
#include "lwip/timeouts.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/ip.h"
#include "lwip/pbuf.h"
#if IP_REASSEMBLY
#include "lwip/ip4_frag.h"
#endif
#if (STD_ON == TCPIP_TCP_ENABLED)
#include "lwip/tcp.h"
#include "lwip/priv/tcp_priv.h"
#endif
#if (STD_ON == TCPIP_UDP_ENABLED)
#include "lwip/udp.h"
#endif
#if CHECKSUM_CHECK_ICMP
#include "lwip/inet_chksum.h"
#endif
#if LWIP_RAW
#include "lwip/raw.h"
#endif
#if LWIP_TCP_IPERF
#include "lwip/apps/lwiperf.h"
#endif
#if (STD_ON == TCPIP_ARP_ENABLED)
#include "lwip/etharp.h"
#endif
#if (STD_ON == TCPIP_AUTOIP_ENABLED)
#include "lwip/autoip.h"
#endif
#if (STD_ON == TCPIP_DHCP_CLIENT_ENABLED)
#include "lwip/dhcp.h"
#endif

#include "Os_Types.h"
#include "ComStack_Types.h"
#include "TcpIp_Cfg.h"
#include "TcpIp_PBcfg.h"
#include "TcpIp_Types.h"
#include "TcpIp_CfgTypes.h"
#include "TcpIp.h"

#if (STD_ON == TCPIP_TCP_TLS_ENABLED)
#include "TcpIp_TlsBase.h"
#endif
#include "Eth_GeneralTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */
#if !defined(TCPIP_LOCAL)
/** Represents an internal function */
#define TCPIP_LOCAL static /* PRQA S 3414 */ /* VL_QAC_KeyWord */
#endif

#if !defined(TCPIP_LOCAL_INLINE)
/** Represents an internal inline function */
#define TCPIP_LOCAL_INLINE static inline
#endif

#if !defined(TCPIP_UNUSED_ARG)
/** Represents an unused parameter */
#define TCPIP_UNUSED_ARG(x) (void)(x)
#endif

/** Represents maximum value of uint8 */
#define TCPIP_UNUSED_UINT8 ((uint8)0xFFu)

/** Represents maximum value of uint16 */
#define TCPIP_UNUSED_UINT16 ((uint16)0xFFFFu)

/** Represents maximum value of uint32 */
#define TCPIP_UNUSED_UINT32 ((uint32)0xFFFFFFFFuL)

/** Represents unused function */
#define TCPIP_UNUSED_FUNCTION 0

#if !defined(TCPIP_FASTTX_TCP)
/** unused macro */
#define TCPIP_FASTTX_TCP 0
#endif

#if !defined(TCPIP_SUPPORT_AUTO_CLOSE_ALL_SOCKET)
/** unused macro */
#define TCPIP_SUPPORT_AUTO_CLOSE_ALL_SOCKET 0
#endif

#if !defined(TCPIP_SUPPORT_WRITE_MORE)
/** unused macro */
#define TCPIP_SUPPORT_WRITE_MORE 0
#endif

#if !defined(TCPIP_LWIP_HOOKS)
/** unused macro */
#define TCPIP_LWIP_HOOKS 0
#endif

/**
 * TCPIP_REPEAT_GET_NETIF_MAC =1 :switch comm mode shall be regain get mac addr write to netif.
 * normal used eth_drv is asynchronous initialization mac addr
 */
#if !defined(TCPIP_REPEAT_GET_NETIF_MAC)
/** unused macro */
#define TCPIP_REPEAT_GET_NETIF_MAC 1
#endif

/*
 * TCPIP_ALLOW_RX_INVALID_LOCAL_IP =1:local ip is invalid allow receiver ip packets
 * application scene: local ip release happend TCP closes the socket four times with a wave,if
 * TCPIP_ALLOW_RX_INVALID_LOCAL_IP=0,ware packet shall be destory in function TcpIp_HookIp4InputAccept
 */
#if !defined(TCPIP_ALLOW_RX_INVALID_LOCAL_IP)
/** unused macro */
#define TCPIP_ALLOW_RX_INVALID_LOCAL_IP 1
#endif

#if !defined(TCPIP_RXIND_FILTER_VLAN)
#if (STD_ON == TCPIP_SOCKET_VLAN)
/** unused macro */
#define TCPIP_RXIND_FILTER_VLAN 1
#else
/** unused macro */
#define TCPIP_RXIND_FILTER_VLAN 0
#endif
#endif

#if !defined(TCPIP_HOOK_VLAN_SET)
/** unused macro */
#define TCPIP_HOOK_VLAN_SET 1
#endif

#if !defined(TCPIP_TCP_IPERF_SUPPORT)
/** tcp iperf, need lwip support */
#define TCPIP_TCP_IPERF_SUPPORT 0
#endif

#if !defined(TCPIP_TCP_IPERF_PORT_DEFAULT)
/** iperf used default tcp port */
#define TCPIP_TCP_IPERF_PORT_DEFAULT LWIPERF_TCP_PORT_DEFAULT
#endif

#if defined(TCPIP_SPPORT_TC8_TEST)
#if !defined(TCPIP_ARP_SUPPORT_DYNAMIC_TIMEOUT_CHANGE)
/** unused macro */
#define TCPIP_ARP_SUPPORT_DYNAMIC_TIMEOUT_CHANGE TCPIP_LWIP_ARP_DYNAMIC_TIMEOUT
#endif
#endif

#if defined(TCPIP_SPPORT_TC8_TEST)
#if !defined(TCPIP_ARP_SUPPORT_STATIC_ENTRIES)
/** unused macro */
#define TCPIP_ARP_SUPPORT_STATIC_ENTRIES ETHARP_SUPPORT_STATIC_ENTRIES
#endif
#endif

#if defined(TCPIP_FUNC_CHECK_OUTPUT_SUPPORT)
#include "func_trace.h"
#define TCPIP_CHECK
/** unused macro */
#define TCPIP_CHECK_OUTPUT(...) FuncCheckOutput(__VA_ARGS__)
#else
#define TCPIP_CHECK_OUTPUT(...)
#endif

#if !defined(TCPIP_INNER_CHECK_TAG)
#define TCPIP_INNER_CHECK
/** unused macro */
#define TCPIP_INNER_CHECK_OUTPUT(...) TCPIP_CHECK_OUTPUT(__VA_ARGS__)
#endif

/** Represents the number of sockets on each partition */
#define TCPIP_SOCKET_NUM (TCPIP_UDP_SOCKET_MAX + TCPIP_TCP_SOCKET_MAX)

/**
 * @name TcpIpIPAddressFormatConversionMacros
 * @{
 * @brief A set of utility macros for IP address format conversion
 */
#if (TCPIP_SC1 == TCPIP_SCALABILITY_CLASS)
/* PRQA S 3472, 3473 ++ */                                        /* VL_TcpIp_3472, VL_TcpIp_3473 */
#define TCPIP_LOCAL_TO_LWIP_IPADDR(lip, uip) ((lip) = ((uip)[0])) /**< local to lwIp format */
#define TCPIP_LWIP_TO_LOCAL_IPADDR(uip, lip) ((uip)[0] = (lip))   /**< lwIp to local format */
/* PRQA S 3472, 3473 -- */
#else
#define TCPIP_LOCAL_TO_LWIP_IPADDR(lip, uip) \
    do                                       \
    {                                        \
        (lip)[0] = (uip)[0];                 \
        (lip)[1] = (uip)[1];                 \
        (lip)[2] = (uip)[2];                 \
        (lip)[3] = (uip)[3];                 \
    } while (0);
#define TCPIP_LWIP_TO_LOCAL_IPADDR(uip, lip) \
    do                                       \
    {                                        \
        (uip)[0] = (lip)[0];                 \
        (uip)[1] = (lip)[1];                 \
        (uip)[2] = (lip)[2];                 \
        (uip)[3] = (lip)[3];                 \
    } while (0)
#endif
/**
 * @}
 */

#if (STD_ON == TCPIP_SUPPORT_AUTOIP_INITTIMER)
#if !defined(TCPIP_SUUPORT_CTRL_MAINHANDLE)
/** unused macro */
#define TCPIP_SUUPORT_CTRL_MAINHANDLE
#endif
#endif

/**
 * @name TcpIpPeriodTimerSupportMacros
 * @{
 * @brief A set of switch macros for period timers
 */
#if (                                                                                                      \
    (STD_ON == TCPIP_AUTOIP_ENABLED) || (STD_ON == TCPIP_IPV6_ENABLED) || (STD_ON == TCPIP_AUTOIP_ENABLED) \
    || (LWIP_IPV6_MLD))
#define TCPIP_SUPPORT_TMR_100ms STD_ON /** 100ms timer */
#else
#define TCPIP_SUPPORT_TMR_100ms STD_OFF /** 100ms timer */
#endif

#if (STD_ON == TCPIP_TCP_ENABLED)
#define TCPIP_SUPPORT_TMR_250ms STD_ON /** 250ms timer */
#else
#define TCPIP_SUPPORT_TMR_250ms STD_OFF /** 250ms timer */
#endif

#if (defined TCPIP_TCP_SYNISN_RECALC) && (STD_ON == TCPIP_TCP_SYNISN_RECALC) && (4u > TCPIP_MAINFUNCTION_PERIOD)
#define TCPIP_SUPPORT_TMR_4ms STD_ON /** 4ms timer */
#else
#define TCPIP_SUPPORT_TMR_4ms STD_OFF /** 4ms timer */
#endif

#if (STD_ON == TCPIP_DHCP_CLIENT_ENABLED)
#define TCPIP_SUPPORT_TMR_500ms   STD_ON /** 500ms timer */
#define TCPIP_SUPPORT_TMR_60000ms STD_ON /** 60s timer */
#else
#define TCPIP_SUPPORT_TMR_500ms   STD_OFF /** 500ms timer */
#define TCPIP_SUPPORT_TMR_60000ms STD_OFF /** 60s timer */
#endif

#if ((STD_ON == TCPIP_ARP_ENABLED) || (STD_ON == TCPIP_IPV6_ENABLED) || (IP_REASSEMBLY) || (LWIP_IPV6_REASS))
#define TCPIP_SUPPORT_TMR_1000ms STD_ON /** 1s timer */
#else
#define TCPIP_SUPPORT_TMR_1000ms STD_OFF /** 1s timer */
#endif
/**
 * @}
 */

#if (STD_ON == TCPIP_PARTITION_USED)
/** get partition index of a structure */
#define TCPIP_PARTITION_INDEX(cfgPtr) (cfgPtr)->PartitionIndex
#else
#define TCPIP_PARTITION_INDEX(cfgPtr) 0u
#endif

/**
 * @name TcpIpInformationRetrievalMacros
 * @{
 * @brief A set of utility macros for information retrieval
 */
/* PRQA S 3472 ++ */ /* VL_TcpIp_3472 */
/** module runtime */
#define TCPIP_RUNTIME(partitionIndex) TcpIp_Runtime[(partitionIndex)]

/** controller configuration */
#define TCPIP_CTRL_CONFIG(ctrlIndex) TcpIp_PbCfgPtr->CtrlPtr[(ctrlIndex)]
/** controller ID to index */
#define TCPIP_CTRL_INDEX(ctrlId) TcpIp_PbCfgPtr->EthIfCtrlId2IndexPtr[(ctrlId)]

/** local address configuration */
#define TCPIP_LOCALADDR_CONFIG(localAddrId) TcpIp_PbCfgPtr->LocalAddrPtr[(localAddrId)]
/** pointer to local address ID to index */
#define TCPIP_LOCALADDR_VARINDEX_REF TcpIp_PbCfgPtr->LocalAddrIdVariantIndexRef
/** local address ID to index */
#define TCPIP_LOCALADDR_VARINDEX(localAddrId) TCPIP_LOCALADDR_VARINDEX_REF[(localAddrId)]

/** socket runtime */
#define TCPIP_SOCKET(partitionIndex, socketIndex) TcpIp_SocketRuntime[(partitionIndex)][(socketIndex)]
/** socket index to ID */
#define TCPIP_SOCKET_ID(partitionIndex, socketIndex) \
    ((TcpIp_SocketIdType)(((partitionIndex) * TCPIP_SOCKET_NUM) + (socketIndex)))
/** socket index w.r.t. controller */
#define TCPIP_SOCKET_INDEX(partitionIndex, socketId) \
    ((TcpIp_SocketIdType)((socketId) - ((partitionIndex) * TCPIP_SOCKET_NUM)))
/** socket index by address */
#define TCPIP_SOCKET_INDEX_BY_ADDR(partitionIndex, socketRtPtr)                                   \
    (TcpIp_SocketIdType)(                                                                         \
        (size_t)((const uint8*)(socketRtPtr) - (const uint8*)(&TCPIP_SOCKET(partitionIndex, 0u))) \
        / sizeof(TcpIp_SocketRuntimeType))

/** period timer runtime */
#define TCPIP_PERIODTIMER(partitionIndex) TcpIp_PeriodTimer[(partitionIndex)]
/* PRQA S 3472 -- */
/**
 * @}
 */

#define TcpIp_EnterExclusiveArea SchM_Enter_TcpIp_ExclusiveArea
#define TcpIp_ExitExclusiveArea  SchM_Exit_TcpIp_ExclusiveArea

/* ================================================ type definitions ================================================ */
/**
 * @name TcpIpStateTypeEnum.
 * @{
 * @brief Type definition for initialization state.
 */
typedef uint8 TcpIp_InitStateType;
#define TCPIP_STATE_UNINIT ((TcpIp_InitStateType)0x00u) /**< not initialized */
#define TCPIP_STATE_INIT   ((TcpIp_InitStateType)0x01u) /**< initialized */
/**
 * @}
 */

/**
 * @brief TcpIp Up notification info.
 */
typedef struct TcpIp_UpTcpIpNotifyTag
{
    /** pending event @range NA */
    TcpIp_EventType PendingEvent;
    /** notify callback @range NA */
    TcpIp_UpTcpIpEventFuncType EventFuncPtr;
} TcpIp_UpTcpIpNotifyType;

/**
 * @brief TcpIp module runtime.
 */
typedef struct TcpIp_RuntimeTag
{
    /** inited state @range NA */
    TcpIp_InitStateType InitedState;
#if (0u < TCPIP_TCP_SOCKET_MAX)
    /** TCP socket allocated number @range NA */
    TcpIp_SocketIdType TcpSocketUsedNum;
#endif
#if (0u < TCPIP_UDP_SOCKET_MAX)
    /** UDP socket allocated number @range NA */
    TcpIp_SocketIdType UdpSocketUsedNum;
#endif
#if (0u < TCPIP_SOCKET_NUM)
    /** index of next free socket @range NA */
    TcpIp_SocketIdType SocketLastIndex;
    /** number of pending notify events @range NA */
    TcpIp_SocketIdType UpNotifyNum;
    /** pending notify events @range NA */
    TcpIp_UpTcpIpNotifyType UpNotify[TCPIP_SOCKET_NUM];
#endif
} TcpIp_RuntimeType;

/**
 * @brief TcpIp socket runtime.
 */
typedef struct TcpIp_SocketRuntimeTag
{
    /** controller index @range NA */
    uint8 CtrlIndex;
#if (STD_ON == TCPIP_TCP_ENABLED)
    /** socket flag @range NA */
    uint8 SocketFlag;
#endif
    /** protocol @range NA */
    TcpIp_ProtocolType Protocol;
    /** socket ID @range NA */
    TcpIp_SocketIdType SocketId;
    /** index of next free socket @range NA */
    TcpIp_SocketIdType NextFreeIndex;
#if (STD_ON == TCPIP_TCP_ENABLED)
    /** TCP transmit remaining length @range NA */
    uint16 UpCopyTxRemainLen;
    /** TCP transmit remaining data @range NA */
    struct pbuf* UpCopyTxPbufPtr;
#endif
    /** pointer to lwIp pcb variable @range NA */
    void* PcbPtr;
    /** pointer to socket owner configuration @range NA */
    const TcpIp_SocketOwnerType* OwnerCfgPtr;
#if (STD_ON == TCPIP_TCP_TLS_ENABLED)
    /** listening socket id @range NA */
    TcpIp_SocketIdType listenSocketId;
    /** pointer to TLS socket variable @range NA */
    TcpIp_TlsSocketDataType* tlsPtr;
#endif
} TcpIp_SocketRuntimeType;

#if (STD_ON == TCPIP_ICMP_ENABLED)
/**
 * @brief TcpIp ICMP transmit data.
 */
typedef struct TcpIp_IcmpTransDataTag
{
    /** TTL @range NA */
    uint8 Ttl;
    /** hop limit @range NA */
    uint8 HopLimit;
    /** type @range NA */
    uint8 Type;
    /** code @range NA */
    uint8 Code;
    /** data length @range NA */
    uint16 DataLength;
    /** pointer to data @range NA */
    const uint8* DataPtr;
    /** remote IP address @range NA */
    ip_addr_t RemoteIp;
} TcpIp_IcmpTransDataType;
#endif

/**
 * @brief TcpIp period timer runtime.
 */
typedef struct TcpIP_PeriodTimerTag
{
#if (STD_ON == TCPIP_SUPPORT_TMR_4ms)
    uint32 tmr_4ms; /**< counter for 4ms timer @range NA */
#endif
#if (STD_ON == TCPIP_SUPPORT_TMR_100ms)
    uint32 tmr_100ms; /**< counter for 100ms timer @range NA */
#endif
#if (STD_ON == TCPIP_SUPPORT_TMR_250ms)
    uint32 tmr_250ms; /**< counter for 250ms timer @range NA */
#endif
#if (STD_ON == TCPIP_SUPPORT_TMR_500ms)
    uint32 tmr_500ms; /**< counter for 500ms timer @range NA */
#endif
#if (STD_ON == TCPIP_SUPPORT_TMR_1000ms)
    uint32 tmr_1000ms; /**< counter for 1s timer @range NA */
#endif
#if (STD_ON == TCPIP_SUPPORT_TMR_60000ms)
    uint32 tmr_60000ms; /**< counter for 60s timer @range NA */
#endif
} TcpIP_PeriodTimerType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */
/**
 * @brief Pointer to current PB configuration.
 * @range NA
 */
extern const TcpIp_ConfigType* TcpIp_PbCfgPtr;

#if (0u < TCPIP_PARTITION_ID2INDEX_NUMBER)
/**
 * @brief Partition ID to index table.
 * @range NA
 */
extern const ApplicationType TcpIp_PartitionId2Index[TCPIP_PARTITION_ID2INDEX_NUMBER];
#endif

/**
 * @brief Module runtime variables.
 * @range NA
 */
extern TcpIp_RuntimeType* const TcpIp_Runtime[TCPIP_PARTITION_NUMBER];

/**
 * @brief Socket runtime variables
 * @range NA
 */
extern TcpIp_SocketRuntimeType* const TcpIp_SocketRuntime[TCPIP_PARTITION_NUMBER];

/**
 * @brief Period timer runtime variables.
 * @range NA
 */
extern TcpIP_PeriodTimerType* const TcpIp_PeriodTimer[TCPIP_PARTITION_NUMBER];

/* ========================================= external function declarations ========================================= */
#if (STD_ON == TCPIP_PARTITION_USED)
#if (0u < TCPIP_PARTITION_ID2INDEX_NUMBER)
/**
 * @brief           Get current partition index.
 * @return          ApplicationType
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern ApplicationType TcpIp_GetCurrentPartition(void);
#else
#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
TCPIP_LOCAL_INLINE ApplicationType TcpIp_GetCurrentPartition(void)
{
    ApplicationType partitionIndex = GetApplicationID();

    return partitionIndex;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"
#endif
#else
#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
TCPIP_LOCAL_INLINE ApplicationType TcpIp_GetCurrentPartition(void)
{
    ApplicationType partitionIndex = 0u;

    return partitionIndex;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"
#endif

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
/**
 * @brief              Local address ID to index conversion.
 * @param[in]          localAddrId: local address ID
 * @return             TcpIp_LocalAddrIdType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
TCPIP_LOCAL_INLINE TcpIp_LocalAddrIdType TcpIp_LocalAddrId2Var(TcpIp_LocalAddrIdType localAddrId)
{
    TcpIp_LocalAddrIdType localAddrIdVar = localAddrId;

#if (1u < TCPIP_VARIANT_NUMBER)
    if (NULL_PTR != TCPIP_LOCALADDR_VARINDEX_REF)
    {
        localAddrIdVar = TCPIP_LOCALADDR_VARINDEX(localAddrId);
    }
#endif

    return localAddrIdVar;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

/**
 * @brief           Netmask ext to inner.
 * @param[in]       netmaskNum: number of netmask
 * @param[in]       domainType: type of domain
 * @param[out]      netmaskPtr: pointer to netmask
 * @synchronous     TRUE
 * @reentrant       Reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern void TcpIp_NetMaskExtToInner(uint8 netmaskNum, TcpIp_DomainType domainType, uint32* netmaskPtr);

/**
 * @brief           Netmask inner to ext.
 * @param[in]       domainType: type of domain
 * @param[in]       netmaskPtr: pointer to netmask
 * @param[out]      netmaskNumPtr: number to netmask
 * @synchronous     TRUE
 * @reentrant       Reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern void TcpIp_NetMaskInnerToExt(TcpIp_DomainType domainType, const uint32* netmaskPtr, uint8* netmaskNumPtr);

/**
 * @brief           Initialize runtime variables of controllers.
 * @param[in]       partitionIndex: index of partition
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern void TcpIp_InitCtrlRuntime(ApplicationType partitionIndex);

/**
 * @brief           Initialize runtime variables of sockets.
 * @param[in]       partitionIndex: index of partition
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern void TcpIp_InitSocketRuntime(ApplicationType partitionIndex);

/**
 * @brief           Initialize runtime variables of period timers.
 * @param[in]       partitionIndex: index of partition
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern void TcpIp_InitPeriodTimer(ApplicationType partitionIndex);

/**
 * @brief           Close socket.
 * @param[in]       socketRtPtr: pointer to socket runtime
 * @param[in]       isAbort: whether connection will immediately be terminated
 * @return          Std_ReturnType
 * @synchronous     FALSE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_InnerClose(TcpIp_SocketRuntimeType* socketRtPtr, boolean isAbort);

/**
 * @brief           Bind socket.
 * @param[in]       socketRtPtr: pointer to socket runtime
 * @param[in]       localAddrId: local address ID
 * @param[in]       portPtr: pointer to port number
 * @return          Std_ReturnType
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern Std_ReturnType
    TcpIp_InnerBind(TcpIp_SocketRuntimeType* socketRtPtr, TcpIp_LocalAddrIdType localAddrId, uint16* portPtr);

#if (STD_ON == TCPIP_TCP_ENABLED)
/**
 * @brief           Connect TCP socket.
 * @param[in]       socketRtPtr: pointer to socket runtime
 * @param[in]       remoteAddrPtr: pointer to remote address
 * @return          Std_ReturnType
 * @synchronous     FALSE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern Std_ReturnType
    TcpIp_InnerTcpConnect(const TcpIp_SocketRuntimeType* socketRtPtr, const TcpIp_SockAddrType* remoteAddrPtr);

/**
 * @brief           Listen TCP socket.
 * @param[in]       socketRtPtr: pointer to socket runtime
 * @param[in]       maxChannNum: number of channels
 * @return          Std_ReturnType
 * @synchronous     FALSE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_InnerTcpListen(TcpIp_SocketRuntimeType* socketRtPtr, uint8 maxChannNum);

/**
 * @brief           Accept TCP data.
 * @param[in]       socketRtPtr: pointer to socket runtime
 * @param[in]       recvLen: number of received bytes
 * @return          Std_ReturnType
 * @synchronous     FALSE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_InnerTcpReceived(const TcpIp_SocketRuntimeType* socketRtPtr, uint16 recvLen);
#endif

/**
 * @brief           Request state change in online state.
 * @param[in]       ctrlCfgPtr: pointer to controller configuration
 * @param[in]       ctrlRtPtr: pointer to controller runtime
 * @param[in]       reqState: requested state
 * @return          Std_ReturnType
 * @synchronous     FALSE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_InnerRequestComModeOnline(
    const TcpIp_CtrlType*  ctrlCfgPtr,
    TcpIp_CtrlRuntimeType* ctrlRtPtr,
    TcpIp_StateType        reqState);

/**
 * @brief           Request state change in onhold state.
 * @param[in]       ctrlCfgPtr: pointer to controller configuration
 * @param[in]       ctrlRtPtr: pointer to controller runtime
 * @param[in]       reqState: requested state
 * @return          Std_ReturnType
 * @synchronous     FALSE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_InnerRequestComModeOnhold(
    const TcpIp_CtrlType*  ctrlCfgPtr,
    TcpIp_CtrlRuntimeType* ctrlRtPtr,
    TcpIp_StateType        reqState);

/**
 * @brief           Request state change in offline state.
 * @param[in]       ctrlCfgPtr: pointer to controller configuration
 * @param[in]       ctrlRtPtr: pointer to controller runtime
 * @param[in]       reqState: requested state
 * @return          Std_ReturnType
 * @synchronous     FALSE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_InnerRequestComModeOffline(
    const TcpIp_CtrlType*  ctrlCfgPtr,
    TcpIp_CtrlRuntimeType* ctrlRtPtr,
    TcpIp_StateType        reqState);

/**
 * @brief           Request state change in startup state.
 * @param[in]       ctrlCfgPtr: pointer to controller configuration
 * @param[in]       ctrlRtPtr: pointer to controller runtime
 * @param[in]       reqState: requested state
 * @return          Std_ReturnType
 * @synchronous     FALSE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_InnerRequestComModeStartup(
    const TcpIp_CtrlType*  ctrlCfgPtr,
    TcpIp_CtrlRuntimeType* ctrlRtPtr,
    TcpIp_StateType        reqState);

/**
 * @brief           Request ip assignment.
 * @param[in]       ctrlCfgPtr: pointer to controller configuration
 * @param[in]       ctrlRtPtr: pointer to controller runtime
 * @param[in]       localAddrCfgPtr: pointer to local address configuration
 * @param[in]       Type: assignment type
 * @param[in]       LocalIpAddrPtr: pointer to local ip address
 * @param[in]       Netmask: netmask
 * @param[in]       DefaultRouterPtr: pointer to default router address
 * @return          Std_ReturnType
 * @synchronous     FALSE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_InnerRequestIpAddrAssignment(
    const TcpIp_CtrlType*      ctrlCfgPtr,
    TcpIp_CtrlRuntimeType*     ctrlRtPtr,
    const TcpIp_LocalAddrType* localAddrCfgPtr,
    TcpIp_IpAddrAssignmentType Type,
    const TcpIp_SockAddrType*  LocalIpAddrPtr,
    uint8                      Netmask,
    const TcpIp_SockAddrType*  DefaultRouterPtr);

/**
 * @brief           Release ip assignment.
 * @param[in]       ctrlCfgPtr: pointer to controller configuration
 * @param[in]       ctrlRtPtr: pointer to controller runtime
 * @param[in]       localAddrCfgPtr: pointer to local address configuration
 * @return          Std_ReturnType
 * @synchronous     FALSE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_InnerReleaseIpAddrAssignment(
    const TcpIp_CtrlType*      ctrlCfgPtr,
    TcpIp_CtrlRuntimeType*     ctrlRtPtr,
    const TcpIp_LocalAddrType* localAddrCfgPtr);

#if (STD_ON == TCPIP_ICMP_ENABLED)
#if ((TCPIP_SC1 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS))
/**
 * @brief           Transmit ICMP message.
 * @param[in]       localAddrId: local address ID
 * @param[in]       icmpParamPtr: pointer to ICMP data
 * @return          Std_ReturnType
 * @synchronous     FALSE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern Std_ReturnType
    TcpIp_InnerIcmpTransmit(TcpIp_LocalAddrIdType localAddrId, const TcpIp_IcmpTransDataType* icmpParamPtr);
#endif

#if ((TCPIP_SC2 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS))
/**
 * @brief           Transmit ICMPv6 message.
 * @return          Std_ReturnType
 * @synchronous     FALSE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_InnerIcmpV6Transmit(void);
#endif
#endif

/**
 * @brief           Change parameter of frame priority.
 * @param[in]       socketRtPtr: pointer to socket runtime
 * @param[in]       paramId: parameter ID
 * @param[in]       paramValue: parameter value
 * @return          Std_ReturnType
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_InnerChangeParameterFramePrio(
    TcpIp_SocketRuntimeType* socketRtPtr,
    TcpIp_ParamIdType        paramId,
    const uint8*             paramValue);

#if (STD_ON == TCPIP_TCP_ENABLED)
/**
 * @brief           Change parameter of TCP nagle algorithm.
 * @param[in]       socketRtPtr: pointer to socket runtime
 * @param[in]       paramId: parameter ID
 * @param[in]       paramValue: parameter value
 * @return          Std_ReturnType
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_InnerChangeParameterTcpNagle(
    TcpIp_SocketRuntimeType* socketRtPtr,
    TcpIp_ParamIdType        paramId,
    const uint8*             paramValue);

/**
 * @brief           Change parameter of TCP keep alive.
 * @param[in]       socketRtPtr: pointer to socket runtime
 * @param[in]       paramId: parameter ID
 * @param[in]       paramValue: parameter value
 * @return          Std_ReturnType
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_InnerChangeParameterTcpKeepAlive(
    TcpIp_SocketRuntimeType* socketRtPtr,
    TcpIp_ParamIdType        paramId,
    const uint8*             paramValue);
#endif

/**
 * @brief           Change parameter of TTL.
 * @param[in]       socketRtPtr: pointer to socket runtime
 * @param[in]       paramId: parameter ID
 * @param[in]       paramValue: parameter value
 * @return          Std_ReturnType
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_InnerChangeParameterTtl(
    TcpIp_SocketRuntimeType* socketRtPtr,
    TcpIp_ParamIdType        paramId,
    const uint8*             paramValue);

#if (STD_ON == TCPIP_SOCKET_VLAN)
/**
 * @brief           Change parameter of VLAN.
 * @param[in]       socketRtPtr: pointer to socket runtime
 * @param[in]       paramId: parameter ID
 * @param[in]       paramValue: parameter value
 * @return          Std_ReturnType
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_InnerChangeParameterSocketVlan(
    TcpIp_SocketRuntimeType* socketRtPtr,
    TcpIp_ParamIdType        paramId,
    const uint8*             paramValue);
#endif

/**
 * @brief           Get remote physical address.
 * @param[in]       ctrlRtPtr: pointer to controller runtime
 * @param[in]       IpAddrPtr: pointer to ip address
 * @param[out]      PhysAddrPtr: pointer ot physical address
 * @param[in]       initRes: whether ARP request shall be initiated
 * @return          Std_ReturnType
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern TcpIp_ReturnType TcpIp_InnerGetRemotePhysAddrByArp(
    TcpIp_CtrlRuntimeType*    ctrlRtPtr,
    const TcpIp_SockAddrType* IpAddrPtr,
    uint8*                    PhysAddrPtr,
    boolean                   initRes);

/**
 * @brief           Check if connection is ready.
 * @param[in]       socketRtPtr: pointer to socket runtime
 * @param[in]       RemoteAddrPtr: pointer to remote address
 * @return          Std_ReturnType
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern TcpIp_ReturnType
    TcpIp_InnerIsConnectionReady(const TcpIp_SocketRuntimeType* socketRtPtr, const TcpIp_SockAddrType* RemoteAddrPtr);

#if (STD_ON == TCPIP_UDP_ENABLED)
/**
 * @brief           Transmit UDP data which is obtained as argument.
 * @param[in]       socketRtPtr: pointer to socket runtime
 * @param[in]       dataPtr: pointer to data
 * @param[in]       remoteAddrPtr: pointer to remote address
 * @param[in]       totalLength: length of data
 * @return          Std_ReturnType
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_InnerUdpTransmitDirect(
    const TcpIp_SocketRuntimeType* socketRtPtr,
    const uint8*                   dataPtr,
    const TcpIp_SockAddrType*      remoteAddrPtr,
    uint16                         totalLength);

/**
 * @brief           Transmit UDP data which is obtained via <Up>_CopyTxData().
 * @param[in]       socketRtPtr: pointer to socket runtime
 * @param[in]       remoteAddrPtr: pointer to remote address
 * @param[in]       totalLength: length of data
 * @return          Std_ReturnType
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_InnerUdpTransmitCopy(
    const TcpIp_SocketRuntimeType* socketRtPtr,
    const TcpIp_SockAddrType*      remoteAddrPtr,
    uint16                         totalLength);
#endif

#if (STD_ON == TCPIP_TCP_ENABLED)
/**
 * @brief           Transmit TCP data which is obtained as argument.
 * @param[in]       socketRtPtr: pointer to socket runtime
 * @param[in]       dataPtr: pointer to data
 * @param[in]       totalLength: length of data
 * @return          Std_ReturnType
 * @synchronous     FALSE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern Std_ReturnType
    TcpIp_InnerTcpTransmitDirect(const TcpIp_SocketRuntimeType* socketRtPtr, const uint8* dataPtr, uint16 totalLength);

/**
 * @brief           Transmit TCP data which is obtained via <Up>_CopyTxData().
 * @param[in]       socketRtPtr: pointer to socket runtime
 * @param[in]       totalLength: length of data
 * @param[in]       forceRetrieve: retrieves all data via single or multiple <Up>_CopyTxData() calls
 * @return          Std_ReturnType
 * @synchronous     FALSE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern Std_ReturnType
    TcpIp_InnerTcpTransmitCopy(TcpIp_SocketRuntimeType* socketRtPtr, uint16 totalLength, boolean forceRetrieve);
#endif

/**
 * @brief           Indication of received data.
 * @param[in]       ctrlCfgPtr: pointer to controller configuration
 * @param[in]       frameType: type of frame
 * @param[in]       flag: pbuf flag
 * @param[in]       dataPtr: pointer to data
 * @param[in]       lenByte: length of received data
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern void TcpIp_InnerRxIndication(
    const TcpIp_CtrlType* ctrlCfgPtr,
    Eth_FrameType         frameType,
    uint8                 flag,
    const uint8*          dataPtr,
    uint16                lenByte);

#if defined(TCPIP_SUUPORT_CTRL_MAINHANDLE)
/**
 * @brief           Main function to handle controllers.
 * @param[in]       partitionIndex: index of partition
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern void TcpIp_ControlPollMainHandle(ApplicationType partitionIndex);
#endif

/**
 * @brief           Main function to handle sockets.
 * @param[in]       partitionIndex: index of partition
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern void TcpIp_SocketPollMainHandle(ApplicationType partitionIndex);

/**
 * @brief           Main function to handle period timers.
 * @param[in]       partitionIndex: index of partition
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern void TcpIp_PeriodTimerMainHandle(ApplicationType partitionIndex);

#if (STD_ON == TCPIP_TCP_ENABLED)
/**
 * @brief           Malloc TCP socket.
 * @param[in]       domain: type of domain
 * @param[in]       socketOwnerCfgPtr: pointer to configuration of socket owner
 * @param[out]      socketIdPtr: pointer to socket ID
 * @return          Std_ReturnType
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_MallocTcpSocket(
    TcpIp_DomainType             domain,
    const TcpIp_SocketOwnerType* socketOwnerCfgPtr,
    TcpIp_SocketIdType*          socketIdPtr);
#endif

#if (STD_ON == TCPIP_UDP_ENABLED)
/**
 * @brief           Malloc UDP socket.
 * @param[in]       domain: type of domain
 * @param[in]       socketOwnerCfgPtr: pointer to configuration of socket owner
 * @param[out]      socketIdPtr: pointer to socket ID
 * @return          Std_ReturnType
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_MallocUdpSocket(
    TcpIp_DomainType             domain,
    const TcpIp_SocketOwnerType* socketOwnerCfgPtr,
    TcpIp_SocketIdType*          socketIdPtr);
#endif

#if ((STD_ON == TCPIP_ARP_ENABLED) && (STD_ON == TCPIP_ARP_STATIC_ENABLED))
/**
 * @brief           Add static entry into ARP table.
 * @param[in]       ctrlIndex: index of controller
 * @param[in]       remoteAddrPtr: pointer to remote address
 * @param[out]      physAddrPtr: pointer to physical address
 * @return          Std_ReturnType
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern Std_ReturnType
    TcpIp_ArpAddStaticEntry(uint32 ctrlIndex, const TcpIp_SockAddrType* remoteAddrPtr, const uint8* physAddrPtr);

/**
 * @brief           Remove static entry from ARP table.
 * @param[in]       ctrlIndex: index of controller
 * @param[in]       remoteAddrPtr: pointer to remote address
 * @return          Std_ReturnType
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_ArpRemoveStaticEntry(uint32 ctrlIndex, const TcpIp_SockAddrType* remoteAddrPtr);
#endif

/**
 * @brief           Get IP frame priority of controller.
 * @param[in]       ctrlIndex: index of controller
 * @return          uint8
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
extern uint8 TcpIp_GetCtrlIpFramePriority(uint8 ctrlIndex);

#ifdef __cplusplus
}
#endif

#endif
