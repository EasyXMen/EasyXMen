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
 **  @file               : TcpIp_Internal.c
 **  @author             : darren.zhang
 **  @date               : 2018/12/03
 **  @vendor             : isoft
 **  @description        : TcpIp source file for internal API implementations
 **
 ***********************************************************************************************************************/
/* PRQA S 3120, 3123 EOF */             /* VL_QAC_MagicNum, VL_TcpIp_3123 */
/* PRQA S 1503 EOF */                   /* VL_QAC_NoUsedApi */
/* PRQA S 1505 EOF */                   /* VL_TcpIp_1505 */
/* PRQA S 1532 EOF */                   /* VL_QAC_OneFunRef */
/* PRQA S 2916, 2982, 2983 EOF */       /* VL_TcpIp_2916, VL_TcpIp_2982, VL_TcpIp_2983 */
/* PRQA S 2991, 2992, 2995, 2996 EOF */ /* VL_TcpIp_2991, VL_TcpIp_2992, VL_TcpIp_2995, VL_TcpIp_2996 */
/* PRQA S 0314, 0316 EOF */             /* VL_TcpIp_0314, VL_QAC_0316 */
/* PRQA S 0488, 0499 EOF */             /* VL_TcpIp_0488, VL_TcpIp_0499 */
/* PRQA S 1258, 1259, 1290, 2144 EOF */ /* VL_TcpIp_1258, VL_TcpIp_1259, VL_TcpIp_1290, VL_TcpIp_2144 */
/* PRQA S 1338 EOF */                   /* VL_TcpIp_1338 */
/* PRQA S 3673 EOF */                   /* VL_QAC_3673 */
/* PRQA S 1277 EOF */                   /* VL_TcpIp_1277 */
/* PRQA S 3631 EOF */                   /* VL_TcpIp_3631 */
/* PRQA S 4391, 4393, 4397 EOF */       /* VL_TcpIp_4391, VL_TcpIp_4393, VL_TcpIp_4397 */
/* PRQA S 6030 EOF */                   /* VL_MTR_TcpIp_STMIF */
/* PRQA S 6040 EOF */                   /* VL_MTR_TcpIp_STPAR */
/* PRQA S 6070 EOF */                   /* VL_MTR_TcpIp_STCAL */
/* PRQA S 6510 EOF */                   /* VL_MTR_TcpIp_CONF */
/* PRQA S 6520 EOF */                   /* VL_MTR_TcpIp_STVAR */
/* PRQA S 1501 EOF */                   /* VL_TcpIp_1501 */
/* PRQA S 1753 EOF */                   /* VL_TcpIp_1753 */
/* =================================================== inclusions =================================================== */
#include "Os_Types.h"
#include "ComStack_Types.h"
#include "TcpIp_Cfg.h"
#include "TcpIp_PBcfg.h"
#include "TcpIp_Types.h"
#include "TcpIp_CfgTypes.h"
#include "TcpIp.h"
#include "TcpIp_Internal.h"
#include "TcpIp_DetError.h"

#if (STD_ON == TCPIP_PARTITION_USED)
#include "Os.h"
#endif
#include "IStdLib.h"
#include "EthIf.h"
#include "SchM_TcpIp.h"
#if (STD_ON == TCPIP_ETHSM_SUPPORT)
#include "EthSM_TcpIp.h"
#endif

#include "lwip/etharp.h"
#if TCPIP_LWIP_HOOKS
#include "hooks/lwip_hooks.h"
#endif
#if TCPIP_TCP_IPERF_SUPPORT && LWIP_TCP_IPERF && defined(LWIP_HOOK_IPERF_GET_TIME_100US)
#include "Tm.h"
#endif

/* ===================================================== macros ===================================================== */
#define ERR_ENUM_MAX ((err_t)ERR_ARG - 1)

#define TCPIP_CHANGEPARAMETER_DECODE_U16(value) (uint16)((((value)[0]) << 8u) | ((value)[1]))
#define TCPIP_CHANGEPARAMETER_DECODE_U32(value) \
    (uint32)((((value)[0]) << 24u) | (((value)[1]) << 16u) | (((value)[2]) << 8u) | ((value)[3]))

#define TCPIP_MASK_U32_BYTE_POS(x) ((x) / 32u)
#define TCPIP_MASK_U32_BIT_POS(x)  ((uint8)((x) & 0x1Fu))

#define TCPIP_NETIF_NSC_REASON_IPCHANGED                                                                         \
    (netif_nsc_reason_t)(                                                                                        \
        (netif_nsc_reason_t)LWIP_NSC_IPV4_ADDRESS_CHANGED | (netif_nsc_reason_t)LWIP_NSC_IPV4_GATEWAY_CHANGED    \
        | (netif_nsc_reason_t)LWIP_NSC_IPV4_NETMASK_CHANGED | (netif_nsc_reason_t)LWIP_NSC_IPV4_SETTINGS_CHANGED \
        | (netif_nsc_reason_t)LWIP_NSC_IPV6_SET | (netif_nsc_reason_t)LWIP_NSC_IPV6_ADDR_STATE_CHANGED)

#if TCPIP_SUPPORT_WRITE_MORE
#define TCPIP_TCP_WRITE_API_FLAG (u8_t)((u8_t)TCP_WRITE_FLAG_COPY | (u8_t)TCP_WRITE_FLAG_MORE)
#else
#define TCPIP_TCP_WRITE_API_FLAG (u8_t)(TCP_WRITE_FLAG_COPY)
#endif

/* PRQA S 3472 ++ */ /* VL_TcpIp_3472 */
#define TCPIP_TIMER_PERIOD(x) (((uint32)(x)) / (uint32)TCPIP_MAINFUNCTION_PERIOD)
/* PRQA S 3472 -- */

#define TCPIP_TCPEXTARG_CALLBACK_INDEX_DISABLED 0u
#define TCPIP_TCPEXTARG_CALLBACK_INDEX_ENABLED  1u

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
#if (STD_ON == TCPIP_CTRL_MULTI_LOCALADDR)
/**
 * @brief           Check the usage of local address.
 * @param[in]       ctrlRtPtr: pointer to controller runtime
 * @return          boolean
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL_INLINE boolean TcpIp_CtrlLocalAddrIsAnyUsed(const TcpIp_CtrlRuntimeType* ctrlRtPtr)
{
    boolean isSet = FALSE;

#if (1u >= TCPIP_CTRL_LOCALADDR_NUMBER_32BITS)
    isSet = (0uL != ctrlRtPtr->LocalAddrUsed[0u]);
#else
    for (TcpIp_LocalAddrIdType index = 0u; index < TCPIP_CTRL_LOCALADDR_NUMBER_32BITS; ++index)
    {
        if (0uL != ctrlRtPtr->LocalAddrUsed[index])
        {
            isSet = TRUE;
            break;
        }
    }
#endif

    return isSet;
}

/**
 * @brief           Set the usage of local address.
 * @param[in]       ctrlRtPtr: pointer to controller runtime
 * @param[in]       localAddrIndex: index of local address
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL_INLINE void
    TcpIp_CtrlLocalAddrSetUsed(TcpIp_CtrlRuntimeType* ctrlRtPtr, TcpIp_LocalAddrIdType localAddrIndex)
{
    TCPIP_FLAG_SET(
        uint32,
        ctrlRtPtr,
        LocalAddrUsed[TCPIP_MASK_U32_BYTE_POS(localAddrIndex)],
        TCPIP_CTRL_LOCALADDR_FLAG(localAddrIndex));
}

/**
 * @brief           Reset the usage of local address.
 * @param[in]       ctrlRtPtr: pointer to controller runtime
 * @param[in]       localAddrIndex: index of local address
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL_INLINE void
    TcpIp_CtrlLocalAddrResetUsed(TcpIp_CtrlRuntimeType* ctrlRtPtr, TcpIp_LocalAddrIdType localAddrIndex)
{
    TCPIP_FLAG_RESET(
        uint32,
        ctrlRtPtr,
        LocalAddrUsed[TCPIP_MASK_U32_BYTE_POS(localAddrIndex)],
        TCPIP_CTRL_LOCALADDR_FLAG(localAddrIndex));
}
#endif

/**
 * @brief           Check pending state.
 * @param[in]       ctrlRtPtr: pointer to controller runtime
 * @param[in]       state: pending state
 * @return          boolean
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL_INLINE boolean TcpIp_CtrlStateIsPending(const TcpIp_CtrlRuntimeType* ctrlRtPtr, TcpIp_StateType state)
{
    return TCPIP_FLAG_IS_SET(uint8, ctrlRtPtr, StateFlag, TCPIP_CTRL_STATE_FLAG(state));
}

/**
 * @brief           Set pending state.
 * @param[in]       ctrlRtPtr: pointer to controller runtime
 * @param[in]       state: pending state
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL_INLINE void TcpIp_CtrlStateSetPending(TcpIp_CtrlRuntimeType* ctrlRtPtr, TcpIp_StateType state)
{
    TCPIP_FLAG_SET(uint8, ctrlRtPtr, StateFlag, TCPIP_CTRL_STATE_FLAG(state));
}

/**
 * @brief           Reset pending state.
 * @param[in]       ctrlRtPtr: pointer to controller runtime
 * @param[in]       state: pending state
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL_INLINE void TcpIp_CtrlStateResetPending(TcpIp_CtrlRuntimeType* ctrlRtPtr, TcpIp_StateType state)
{
    TCPIP_FLAG_RESET(uint8, ctrlRtPtr, StateFlag, TCPIP_CTRL_STATE_FLAG(state));
}

/**
 * @brief           Check the flag of local address.
 * @param[in]       localAddrRtPtr: pointer to local address
 * @param[in]       mask: flag mask
 * @return          boolean
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL_INLINE boolean TcpIp_LocalAddrFlagIsSet(const TcpIp_LocalAddrRuntimeType* localAddrRtPtr, uint8 mask)
{
    return TCPIP_FLAG_IS_SET(uint8, localAddrRtPtr, Flag, mask);
}

/**
 * @brief           Set the flag of local address.
 * @param[in]       localAddrRtPtr: pointer to local address
 * @param[in]       mask: flag mask
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL_INLINE void TcpIp_LocalAddrFlagSet(TcpIp_LocalAddrRuntimeType* localAddrRtPtr, uint8 mask)
{
    TCPIP_FLAG_SET(uint8, localAddrRtPtr, Flag, mask);
}

/**
 * @brief           Reset the flag of local address.
 * @param[in]       localAddrRtPtr: pointer to local address
 * @param[in]       mask: flag mask
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL_INLINE void TcpIp_LocalAddrFlagReset(TcpIp_LocalAddrRuntimeType* localAddrRtPtr, uint8 mask)
{
    TCPIP_FLAG_RESET(uint8, localAddrRtPtr, Flag, mask);
}

/**
 * @brief           Check the flag of socket.
 * @param[in]       socketRtPtr: pointer to socket runtime
 * @param[in]       mask: flag mask
 * @return          boolean
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL_INLINE boolean TcpIp_SocketFlagIsSet(const TcpIp_SocketRuntimeType* socketRtPtr, uint8 mask)
{
    return TCPIP_FLAG_IS_SET(uint8, socketRtPtr, SocketFlag, mask);
}

/**
 * @brief           Set the flag of socket.
 * @param[in]       socketRtPtr: pointer to socket runtime
 * @param[in]       mask: flag mask
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL_INLINE void TcpIp_SocketFlagSet(TcpIp_SocketRuntimeType* socketRtPtr, uint8 mask)
{
    TCPIP_FLAG_SET(uint8, socketRtPtr, SocketFlag, mask);
}

/**
 * @brief           Reset the flag of socket.
 * @param[in]       socketRtPtr: pointer to socket runtime
 * @param[in]       mask: flag mask
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL_INLINE void TcpIp_SocketFlagReset(TcpIp_SocketRuntimeType* socketRtPtr, uint8 mask)
{
    TCPIP_FLAG_RESET(uint8, socketRtPtr, SocketFlag, mask);
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#if (STD_ON == TCPIP_TCP_ENABLED)
/**
 * @brief           Notify TCP accepted event to upper layer.
 * @param[in]       listenSocketRtPtr: pointer to listening socket runtime
 * @param[in]       socketRtPtr: pointer to accepted socket runtime
 * @return          Std_ReturnType
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL Std_ReturnType TcpIp_NotifyUpTcpAccepted(
    const TcpIp_SocketRuntimeType* listenSocketRtPtr,
    const TcpIp_SocketRuntimeType* socketRtPtr);

/**
 * @brief           Notify TCP connected event to upper layer.
 * @param[in]       socketRtPtr: pointer to connected socket runtime
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL void TcpIp_NotifyUpTcpConnected(const TcpIp_SocketRuntimeType* socketRtPtr);

/**
 * @brief           Notify TCP RX indication event to upper layer.
 * @param[in]       socketRtPtr: pointer to socket runtime
 * @param[in]       pbufPtrPtr: pointer to pbuf of received data
 * @return          Std_ReturnType
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL Std_ReturnType
    TcpIp_TcpNotifyUpRxIndication(const TcpIp_SocketRuntimeType* socketRtPtr, struct pbuf** pbufPtrPtr);

/**
 * @brief           Notify TX confirmation event to upper layer.
 * @param[in]       socketRtPtr: pointer to socket runtime
 * @param[in]       len: length of confirmed data
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL void TcpIp_TcpNotifyUpTxConfirmation(const TcpIp_SocketRuntimeType* socketRtPtr, uint16 len);

/**
 * @brief           Notify TcpIp event to upper layer.
 * @param[in]       socketRtPtr: pointer to socket runtime
 * @param[in]       event: type of event
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL void TcpIp_NotifyUpTcpIpEvent(const TcpIp_SocketRuntimeType* socketRtPtr, TcpIp_EventType event);
#endif

#if (STD_ON == TCPIP_UDP_ENABLED)
/**
 * @brief           Notify UDP RX indication event to upper layer.
 * @param[in]       socketRtPtr: pointer to socket runtime
 * @param[in]       pbufPtrPtr: pointer to pbuf of received data
 * @param[in]       remoteIpPtr: pointer to remote ip address
 * @param[in]       port: port number
 * @return          Std_ReturnType
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL Std_ReturnType TcpIp_UdpNotifyUpRxIndication(
    const TcpIp_SocketRuntimeType* socketRtPtr,
    struct pbuf**                  pbufPtrPtr,
    const ip_addr_t*               remoteIpPtr,
    uint16                         port);
#endif

/**
 * @brief           Notify local ip address assignment event to upper layer.
 * @param[in]       localAddrRtPtr: pointer to local address
 * @param[in]       ipAddrState: type of ip address state
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL void TcpIp_NotfiyUpLocalIpAddrAssignmentChg(
    const TcpIp_LocalAddrRuntimeType* localAddrRtPtr,
    TcpIp_IpAddrStateType             ipAddrState);

#if (STD_ON == TCPIP_TCP_ENABLED)
/**
 * @brief           Callback of TCP accept event.
 * @param[in]       arg: pointer to socket runtime
 * @param[in]       newpcb: pointer to pcb
 * @param[in]       err: error code
 * @return          err_t
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL err_t TcpIp_TcpAcceptCallback(void* arg, struct tcp_pcb* newpcb, err_t err);

/**
 * @brief           Callback of TCP connect event.
 * @param[in]       arg: pointer to socket runtime
 * @param[in]       tpcb: pointer to pcb
 * @param[in]       err: error code
 * @return          err_t
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL err_t TcpIp_TcpConnnectCallback(void* arg, struct tcp_pcb* tpcb, err_t err);

/**
 * @brief           Callback of TCP received event.
 * @param[in]       arg: pointer to socket runtime
 * @param[in]       tpcb: pointer to pcb
 * @param[in]       p: pointer to pbuf
 * @param[in]       err: error code
 * @return          err_t
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL err_t TcpIp_TcpRecvCallback(void* arg, struct tcp_pcb* tpcb, struct pbuf* p, err_t err);

/**
 * @brief           Callback of TCP sent event.
 * @param[in]       arg: pointer to socket runtime
 * @param[in]       tpcb: pointer to pcb
 * @param[in]       len: length of sent data
 * @return          err_t
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL err_t TcpIp_TcpSentCallback(void* arg, struct tcp_pcb* tpcb, u16_t len);

/**
 * @brief           Callback of TCP error event.
 * @param[in]       arg: pointer to socket runtime
 * @param[in]       err: error code
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL void TcpIp_TcpErrCallback(void* arg, err_t err);

/**
 * @brief           Callback of TCP pcb destroyed event.
 * @param[in]       id: index of extra argument
 * @param[in]       data: pointer to socket runtime
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL void TcpIp_TcpPcbDestroyedCallback(u8_t id, void* data);
#endif

#if (STD_ON == TCPIP_UDP_ENABLED)
/**
 * @brief           Callback of UDP received event.
 * @param[in]       arg: pointer to socket runtime
 * @param[in]       pcb: pointer to pcb
 * @param[in]       p: pointer to pbuf
 * @param[in]       addr: pointer to ip address
 * @param[in]       port: port number
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL void
    TcpIp_UdpRecvCallback(void* arg, struct udp_pcb* pcb, struct pbuf* p, const ip_addr_t* addr, u16_t port);
#endif

/**
 * @brief           Callback of NetIf ip changed event.
 * @param[in]       netif: pointer to netif
 * @param[in]       reason: reason of ip changed
 * @param[in]       args: pointer to extra arguments
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL void
    TcpIp_NetifIpChangedCallback(struct netif* netif, netif_nsc_reason_t reason, const netif_ext_callback_args_t* args);

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
/**
 * @brief           Notify mode indication to EthSM.
 * @param[in]       ctrlCfgPtr: pointer to controller configuration
 * @param[in]       state: controller state to notify
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL_INLINE void TcpIp_EthSmModeIndicationNotifyForCtrl(const TcpIp_CtrlType* ctrlCfgPtr, TcpIp_StateType state)
{
#if (STD_ON == TCPIP_ETHSM_SUPPORT)
    EthSM_TcpIpModeIndication(ctrlCfgPtr->EthIfCtrlId, state);
#else
    TCPIP_UNUSED_ARG(ctrlCfgPtr);
    TCPIP_UNUSED_ARG(state);
#endif
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

/**
 * @brief           Handle local ip address change of a controller.
 * @param[in]       ctrlCfgPtr: pointer to controller configuration
 * @param[in]       ctrlRtPtr: pointer to controller runtime
 * @param[in]       ipAddrState: type of ip address state
 * @param[in]       forceNotify: force to notify
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL void TcpIp_LocalIpAddrAssignChgHandleForCtrl(
    const TcpIp_CtrlType*  ctrlCfgPtr,
    TcpIp_CtrlRuntimeType* ctrlRtPtr,
    TcpIp_IpAddrStateType  ipAddrState,
    boolean                forceNotify);

/**
 * @brief           Handle TcpIp event.
 * @param[in]       socketRtPtr: pointer to socket runtime
 * @param[in]       event: type of event
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL void TcpIp_UpTcpIpEventHandle(const TcpIp_SocketRuntimeType* socketRtPtr, TcpIp_EventType event);

/**
 * @brief           Notify TcpIp event.
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL void TcpIp_UpTcpIpEventNotify(void);

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
#if defined(TCPIP_INNER_CHECK)
/**
 * @brief           Check controller index of NetIf.
 * @param[in]       netifPtr: pointer to NetIf
 * @return          boolean
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL_INLINE boolean TcpIp_NetifIsSetCtrlIndex(const struct netif* netifPtr)
{
    return (
        ((char)'I' == netifPtr->name[0]) /* PRQA S 2106, 2140 */ /* VL_TcpIp_2106, VL_TcpIp_2140 */
        && (TCPIP_CTRL_INDEX_INVALID > (uint8)(netifPtr->name[1]))
#if defined(TCPIP_INNER_CHECK)
        && (NULL_PTR != TcpIp_PbCfgPtr) && (TcpIp_PbCfgPtr->CtrlNum > (uint8)(netifPtr->name[1]))
#endif
    );
}
#endif

/**
 * @brief           Get controller index of NetIf.
 * @param[in]       netifPtr: pointer to NetIf
 * @return          uint8
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL_INLINE uint8 TcpIp_NetifGetCtrlIndex(const struct netif* netifPtr)
{
    return (uint8)(netifPtr->name[1]);
}

/**
 * @brief           Set controller index of NetIf.
 * @param[in]       netifPtr: pointer to NetIf
 * @param[in]       ctrlIndex: index of controller
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL_INLINE void TcpIp_NetifSetCtrlIndex(struct netif* netifPtr, uint8 ctrlIndex)
{
    netifPtr->name[0] = (char)'I';
    netifPtr->name[1] = (char)ctrlIndex;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

/**
 * @brief           Get physical address of EthIf.
 * @param[in]       netifPtr: pointer to NetIf
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL void TcpIp_NetIfGetPhyAddrFromEthIf(struct netif* netifPtr);

/**
 * @brief           Initialize NetIf.
 * @param[in]       netif: pointer to NetIf
 * @return          err_t
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL err_t TcpIp_InitNetif(struct netif* netif);

/**
 * @brief           Initialize local address runtime.
 * @param[in]       ctrlCfgPtr: pointer to controller configuration
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL void TcpIp_InitLocalAddrRuntime(const TcpIp_CtrlType* ctrlCfgPtr);

/**
 * @brief           Bind ip address and port number to a socket.
 * @param[in]       socketRtPtr: pointer to socket runtime
 * @param[in]       ctrlRtPtr: pointer to controller runtime
 * @param[in]       localIpPtr: pointer to local ip address
 * @param[inout]    portPtr: pointer to port number
 * @return          err_t
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL err_t TcpIp_DoBind(
    TcpIp_SocketRuntimeType*     socketRtPtr,
    const TcpIp_CtrlRuntimeType* ctrlRtPtr,
    const ip_addr_t*             localIpPtr,
    uint16*                      portPtr);

/**
 * @brief           Close all socket of a controller.
 * @param[in]       ctrlRtPtr: pointer to controller runtime
 * @return          TcpIp_SocketIdType
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL TcpIp_SocketIdType TcpIp_CloseAllSocket(const TcpIp_CtrlRuntimeType* ctrlRtPtr);

/**
 * @brief           Handle multicast ip address assignment.
 * @param[in]       ctrlRtPtr: pointer to controller runtime
 * @param[in]       localAddrCfgPtr: pointer to local address configuration
 * @param[in]       isAssign: assign or release
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL void TcpIp_MultCastIpAssigmentHandle(
    TcpIp_CtrlRuntimeType*     ctrlRtPtr,
    const TcpIp_LocalAddrType* localAddrCfgPtr,
    boolean                    isAssign);

/**
 * @brief           Set local address with static method.
 * @param[in]       ctrlRtPtr: pointer to controller runtime
 * @param[in]       localAddrCfgPtr: pointer to local address configuration
 * @param[in]       ipAddrCfgPtr: pointer to static ip address configuration
 * @return          Std_ReturnType
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL Std_ReturnType TcpIp_SetLocalAddrStatic(
    TcpIp_CtrlRuntimeType*                 ctrlRtPtr,
    const TcpIp_LocalAddrType*             localAddrCfgPtr,
    const TcpIp_StaticIpAddressConfigType* ipAddrCfgPtr);

/**
 * @brief           Clear local address with static method.
 * @param[in]       ctrlRtPtr: pointer to controller runtime
 * @param[in]       localAddrCfgPtr: pointer to local address configuration
 * @return          Std_ReturnType
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL Std_ReturnType
    TcpIp_ClearLocalAddrStatic(TcpIp_CtrlRuntimeType* ctrlRtPtr, const TcpIp_LocalAddrType* localAddrCfgPtr);

/**
 * @brief           Handle ip address assignment request.
 * @param[in]       ctrlCfgPtr: pointer to controller configuration
 * @param[in]       ctrlRtPtr: pointer to controller runtime
 * @param[in]       localAddrCfgPtr: pointer to local address configuration
 * @param[in]       assignType: type of assignment
 * @param[in]       ipAddrCfgPtr: pointer to static ip address configuration
 * @return          Std_ReturnType
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL Std_ReturnType TcpIp_DoRequestIpAddrAssignment(
    const TcpIp_CtrlType*                  ctrlCfgPtr,
    TcpIp_CtrlRuntimeType*                 ctrlRtPtr,
    const TcpIp_LocalAddrType*             localAddrCfgPtr,
    TcpIp_IpAddrAssignmentType             assignType,
    const TcpIp_StaticIpAddressConfigType* ipAddrCfgPtr);

/**
 * @brief           Handle ip address assignment release.
 * @param[in]       ctrlCfgPtr: pointer to controller configuration
 * @param[in]       ctrlRtPtr: pointer to controller runtime
 * @param[in]       localAddrCfgPtr: pointer to local address configuration
 * @param[in]       assignType: type of assignment
 * @return          Std_ReturnType
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL Std_ReturnType TcpIp_DoReleaseIpAddrAssignment(
    const TcpIp_CtrlType*      ctrlCfgPtr,
    TcpIp_CtrlRuntimeType*     ctrlRtPtr,
    const TcpIp_LocalAddrType* localAddrCfgPtr,
    TcpIp_IpAddrAssignmentType assignType);

/**
 * @brief           Handle ip address assignment request with automatic method.
 * @param[in]       ctrlCfgPtr: pointer to controller configuration
 * @param[in]       ctrlRtPtr: pointer to controller runtime
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL void
    TcpIp_RequestIpAddrAssignmentAutomaticForCtrl(const TcpIp_CtrlType* ctrlCfgPtr, TcpIp_CtrlRuntimeType* ctrlRtPtr);

/**
 * @brief           Handle ip address assignment release.
 * @param[in]       ctrlCfgPtr: pointer to controller configuration
 * @param[in]       ctrlRtPtr: pointer to controller runtime
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL void
    TcpIp_ReleaseIpAddrAssignmentForCtrl(const TcpIp_CtrlType* ctrlCfgPtr, TcpIp_CtrlRuntimeType* ctrlRtPtr);

#if (STD_ON == TCPIP_TCP_ENABLED)
/**
 * @brief           Obtain TCP data via <Up>_CopyTxData().
 * @param[in]       socketRtPtr: pointer to socket runtime
 * @return          uint16
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL uint16 TcpIp_TcpTransmitCopyTxData(TcpIp_SocketRuntimeType* socketRtPtr);

/**
 * @brief           Pass TCP data to lwIp.
 * @param[in]       socketRtPtr: pointer to socket runtime
 * @param[in]       copyLen: length of data
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL void TcpIp_TcpTransmitCopyWrite(TcpIp_SocketRuntimeType* socketRtPtr, uint16 copyLen);

/* PRQA S 5016 ++ */ /* VL_TcpIp_5016 */
/**
 * @brief           Malloc TCP socket with a pcb.
 * @param[in]       socketRtPtr: pointer to socket runtime
 * @param[in]       pcbPtr: pointer to pcb
 * @return          TcpIp_SocketRuntimeType
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL TcpIp_SocketRuntimeType*
    TcpIp_MallocTcpSocketByPcb(const TcpIp_SocketRuntimeType* socketRtPtr, struct tcp_pcb* pcbPtr);
/* PRQA S 5016 -- */

/**
 * @brief           Free TCP socket.
 * @param[in]       socketRtPtr: pointer to socket runtime
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL void TcpIp_FreeTcpSocket(TcpIp_SocketRuntimeType* socketRtPtr);
#endif

#if (STD_ON == TCPIP_UDP_ENABLED)
/**
 * @brief           Free UDP socket.
 * @param[in]       socketRtPtr: pointer to socket runtime
 * @synchronous     TRUE
 * @reentrant       Non reentrant
 * @trace           CPD-PLACEHOLDER
 */
TCPIP_LOCAL void TcpIp_FreeUdpSocket(TcpIp_SocketRuntimeType* socketRtPtr);
#endif

/* ============================================ internal data definitions =========================================== */
#define TCPIP_START_SEC_VAR_CLEARED_PTR
#include "TcpIp_MemMap.h"
const TcpIp_ConfigType* TcpIp_PbCfgPtr;
#define TCPIP_STOP_SEC_VAR_CLEARED_PTR
#include "TcpIp_MemMap.h"

#define TCPIP_START_SEC_VAR_CLONE_8
#include "TcpIp_MemMap.h"
uint8 TcpIp_MulticoreIndex;
#define TCPIP_STOP_SEC_VAR_CLONE_8
#include "TcpIp_MemMap.h"

#if TCPIP_LWIP_HOOKS
#if (defined TCPIP_TCP_SYNISN_RECALC) && (STD_ON == TCPIP_TCP_SYNISN_RECALC)
#define TCPIP_START_SEC_VAR_CLEARED_32
#include "TcpIp_MemMap.h"
/**
 * @brief initial sequence number counter
 */
TCPIP_LOCAL uint32 TcpIp_TcpIsnCount;
#define TCPIP_STOP_SEC_VAR_CLEARED_32
#include "TcpIp_MemMap.h"
#endif
#endif

#if (STD_ON == TCPIP_TCP_ENABLED)
#define TCPIP_START_SEC_CONST_UNSPECIFIED
#include "TcpIp_MemMap.h"
/**
 * @brief Callbacks for tcp_ext_arg_set_callbacks()
 */
TCPIP_LOCAL const struct tcp_ext_arg_callbacks TcpIp_TcpExtArgCallbackTable[] = {
    {
        NULL_PTR,
        NULL_PTR,
    },
    {
        TcpIp_TcpPcbDestroyedCallback,
        NULL_PTR,
    },
};
#define TCPIP_STOP_SEC_CONST_UNSPECIFIED
#include "TcpIp_MemMap.h"
#endif

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"

#if (STD_ON == TCPIP_PARTITION_USED)
#if (0u < TCPIP_PARTITION_ID2INDEX_NUMBER)
ApplicationType TcpIp_GetCurrentPartition(void)
{
    ApplicationType partitionId = GetApplicationID();
    ApplicationType partitionIndex;

    if (TCPIP_PARTITION_ID2INDEX_NUMBER > partitionId)
    {
        partitionIndex = TcpIp_PartitionId2Index[partitionId];
    }
    else
    {
        partitionIndex = TCPIP_PARTITION_NUMBER; /* invalid index */
    }

    return partitionIndex;
}
#endif
#endif

void TcpIp_NetMaskExtToInner(uint8 netmaskNum, TcpIp_DomainType domainType, uint32* netmaskPtr)
{
    TCPIP_UNUSED_ARG(domainType);

#if (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS)
    if (TCPIP_AF_INET == domainType)
#endif
    {
#if (TCPIP_SC1 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS)
        /* ipv4 32bit*/
        netmaskPtr[0] = (uint32)(TCPIP_UNUSED_UINT32 << (32u - netmaskNum));
#endif
    }
#if (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS)
    else
#endif
    {
#if (TCPIP_SC2 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS)
        /* ipv6 128bit*/
        /* (128u - netmaskNum)>96u?32-netmaskNum:0  */
        netmaskPtr[3] = (uint32)(TCPIP_UNUSED_UINT32 << ((netmaskNum < 32u) ? (32u - netmaskNum) : 0u));
        /* (128u - netmaskNum)>64u?(64-netmaskNum)%32:0  */
        netmaskPtr[2] = (uint32)(TCPIP_UNUSED_UINT32 << ((netmaskNum < 64u) ? (64u - netmaskNum) & 0x1Fu : 0u));
        /* (128u - netmaskNum)>96u?(96-netmaskNum)%32:0  */
        netmaskPtr[1] = (uint32)(TCPIP_UNUSED_UINT32 << ((netmaskNum < 64u) ? (64u - netmaskNum) & 0x1Fu : 0u));
        /* (128u - netmaskNum)%32  */
        netmaskPtr[0] = (uint32)(TCPIP_UNUSED_UINT32 << ((128u - netmaskNum) & 0x1Fu));
#endif
    }
}

void TcpIp_NetMaskInnerToExt(TcpIp_DomainType domainType, const uint32* netmaskPtr, uint8* netmaskNumPtr)
{
    uint8 cnt = 0u;

    TCPIP_UNUSED_ARG(domainType);

#if (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS)
    if (TCPIP_AF_INET == domainType)
#endif
    {
#if (TCPIP_SC1 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS)
        uint32 vlu = netmaskPtr[0];

        while (0uL < vlu)
        {
            if (1uL == (vlu & 1uL))
            {
                cnt++;
            }
            vlu >>= 1u;
        }
#endif
    }
#if (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS)
    else
#endif
    {
#if (TCPIP_SC2 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS)
        uint32 vlu = netmaskPtr[0];

        for (uint8 i = 0u; (4u > i) && (0uL < vlu); ++i)
        {
            vlu = netmaskPtr[i];

            while (0uL < vlu)
            {
                if (1uL == (vlu & 1uL))
                {
                    cnt++;
                }
                vlu >>= 1u;
            }
        }
#endif
    }

    *netmaskNumPtr = cnt;
}

void TcpIp_InitCtrlRuntime(ApplicationType partitionIndex)
{
    for (uint8 ctrlIndex = 0u; ctrlIndex < TcpIp_PbCfgPtr->CtrlNum; ++ctrlIndex)
    {
        const TcpIp_CtrlType* ctrlCfgPtr = &TCPIP_CTRL_CONFIG(ctrlIndex);

        if (TCPIP_PARTITION_INDEX(ctrlCfgPtr) == partitionIndex)
        {
            TcpIp_CtrlRuntimeType* ctrlRtPtr = ctrlCfgPtr->RuntimePtr;
            struct netif*          netifPtr  = &(ctrlRtPtr->Netif);

            (void)IStdLib_MemSet(ctrlRtPtr, 0u, sizeof(TcpIp_CtrlRuntimeType));

            TcpIp_NetifSetCtrlIndex(netifPtr, ctrlIndex);
            netifPtr = netif_add_noaddr(netifPtr, NULL_PTR, TcpIp_InitNetif, NULL_PTR);

#if defined(TCPIP_INNER_CHECK)
            if ((NULL_PTR != netifPtr) && (&(ctrlRtPtr->Netif) == netifPtr))
#endif
            {
#if LWIP_IPV6
                netif_create_ip6_linklocal_address(netifPtr, 1);
#endif
#if (STD_ON == TCPIP_AUTOIP_ENABLED)
                autoip_set_struct(netifPtr, &(ctrlRtPtr->netif_autoip));
#endif
#if (STD_ON == TCPIP_DHCP_CLIENT_ENABLED)
                dhcp_set_struct(netifPtr, &(ctrlRtPtr->netif_dhcp));
#endif

                netif_set_down(netifPtr);
                netif_set_link_down(netifPtr);

                netif_add_ext_callback(&(ctrlRtPtr->NetifExtCallback), TcpIp_NetifIpChangedCallback, netifPtr);
                netif_set_default(netifPtr);
            }

            ctrlRtPtr->CtrlStatus = TCPIP_STATE_OFFLINE;

#if ((STD_ON == TCPIP_ARP_ENABLED) && (STD_ON == TCPIP_ARP_STATIC_ENABLED))
            if (NULL_PTR != ctrlCfgPtr->ArpCfgPtr)
            {
                const TcpIp_ArpConfigType* arpCfgPtr = ctrlCfgPtr->ArpCfgPtr;

                for (uint8 entryIndex = 0u; entryIndex < arpCfgPtr->ArpTableStaticEntryNum; ++entryIndex)
                {
                    const TcpIp_ArpStaticEntryType* entryPtr   = &arpCfgPtr->ArpTableStaticEntryPtr[entryIndex];
                    const TcpIp_SockAddrType        remoteAddr = {
                               .addr = {entryPtr->IpAddr[0]},
                    };

                    (void)TcpIp_ArpAddStaticEntry(ctrlIndex, &remoteAddr, entryPtr->PhysAddr);
                }
            }
#endif

            TcpIp_InitLocalAddrRuntime(ctrlCfgPtr);
        }
    }
}

void TcpIp_InitSocketRuntime(ApplicationType partitionIndex)
{
    for (TcpIp_SocketIdType socketIndex = 0u; TCPIP_SOCKET_NUM > socketIndex; ++socketIndex)
    {
        TcpIp_SocketRuntimeType* socketRtPtr = &TCPIP_SOCKET(partitionIndex, socketIndex);

        (void)IStdLib_MemSet(socketRtPtr, 0u, sizeof(TcpIp_SocketRuntimeType));
        socketRtPtr->SocketId      = TCPIP_SOCKET_ID(partitionIndex, socketIndex);
        socketRtPtr->NextFreeIndex = socketIndex + 1u;
    }
}

void TcpIp_InitPeriodTimer(ApplicationType partitionIndex)
{
    TcpIP_PeriodTimerType* timerRtPtr = TCPIP_PERIODTIMER(partitionIndex);

    (void)IStdLib_MemSet(timerRtPtr, 0u, sizeof(TcpIP_PeriodTimerType));
}

Std_ReturnType TcpIp_InnerClose(TcpIp_SocketRuntimeType* socketRtPtr, boolean isAbort)
{
    Std_ReturnType ret = E_NOT_OK;

    TcpIp_EnterExclusiveArea();
#if defined(TCPIP_INNER_CHECK)
    if (NULL_PTR != socketRtPtr->PcbPtr)
#endif
    {
#if (STD_ON == TCPIP_TCP_ENABLED)
        if (TCPIP_IPPROTO_TCP == socketRtPtr->Protocol)
        {
            err_t           err;
            struct tcp_pcb* pcbPtr = (struct tcp_pcb*)socketRtPtr->PcbPtr;

            if ((LISTEN == pcbPtr->state) || (!isAbort))
            {
                TcpIp_SocketFlagSet(socketRtPtr, TCPIP_SOCKET_FLAG_TCP_PENDING_EVENT);

                err = tcp_close(pcbPtr);
            }
            else
            {
                tcp_abort(pcbPtr);
                err = (err_t)ERR_OK;
            }

            if ((err_t)ERR_OK == err)
            {
                ret = E_OK;
            }
        }
#endif

#if (STD_ON == TCPIP_UDP_ENABLED)
        if (TCPIP_IPPROTO_UDP == socketRtPtr->Protocol)
        {
            struct udp_pcb* pcbPtr = (struct udp_pcb*)socketRtPtr->PcbPtr;

            udp_disconnect(pcbPtr);
            udp_remove(pcbPtr);

            TcpIp_UpTcpIpEventHandle(socketRtPtr, TCPIP_UDP_CLOSED);
            TcpIp_FreeUdpSocket(socketRtPtr);

            ret = E_OK;
        }
#endif
    }
#if defined(TCPIP_INNER_CHECK)
    else
    {
        ret = E_OK;
    }
#endif
    TcpIp_ExitExclusiveArea();

    TcpIp_UpTcpIpEventNotify();

    return ret;
}

Std_ReturnType TcpIp_InnerBind(TcpIp_SocketRuntimeType* socketRtPtr, TcpIp_LocalAddrIdType localAddrId, uint16* portPtr)
{
    Std_ReturnType         ret        = E_NOT_OK;
    uint8                  ctrlIndex  = TCPIP_CTRL_INDEX_INVALID;
    const TcpIp_CtrlType*  ctrlCfgPtr = NULL_PTR;
    TcpIp_CtrlRuntimeType* ctrlRtPtr  = NULL_PTR;
    const ip_addr_t*       localIpPtr = NULL_PTR;
    boolean                toBindFlag = FALSE;

    if (TCPIP_LOCALADDR_NUMBER > localAddrId)
    {
        const TcpIp_LocalAddrType*        localAddrCfgPtr = &TCPIP_LOCALADDR_CONFIG(localAddrId);
        const TcpIp_LocalAddrRuntimeType* localAddrRtPtr  = localAddrCfgPtr->RuntimePtr;

        ctrlIndex  = localAddrCfgPtr->CtrlIndex;
        ctrlCfgPtr = &TCPIP_CTRL_CONFIG(ctrlIndex);
        ctrlRtPtr  = ctrlCfgPtr->RuntimePtr;
        localIpPtr = &(localAddrRtPtr->LocalIp);

        TcpIp_EnterExclusiveArea();
        toBindFlag =
            ((TCPIP_STATE_ONLINE == ctrlRtPtr->CtrlStatus)
             && TcpIp_LocalAddrFlagIsSet(localAddrRtPtr, TCPIP_LOCALADDR_FLAG_IP_IS_VALID));
    }
    else /* TODO: to listen to all EthIf controller, TCPIP_LOCALADDRID_ANY has to be specified as LocalAddrId. */
    {
        toBindFlag = FALSE;
        /* TODO assign ctrlIndex to EthIf controller of current partition */

        TcpIp_EnterExclusiveArea();
    }

    if (
#if defined(TCPIP_INNER_CHECK)
        (NULL_PTR != socketRtPtr->PcbPtr) &&
#endif
        toBindFlag)
    {
        err_t err = TcpIp_DoBind(socketRtPtr, ctrlRtPtr, localIpPtr, portPtr);

        if ((err_t)ERR_OK == err)
        {
            socketRtPtr->CtrlIndex = ctrlIndex;
            ctrlRtPtr->SocketUsedNum += 1u;
            TcpIp_ExitExclusiveArea();

            ret = E_OK;
        }
        else
        {
            TcpIp_ExitExclusiveArea();

#if (STD_ON == TCPIP_DEV_ERROR_DETECT)
            if ((err_t)ERR_USE == err)
            {
                TcpIp_DetReportError(TCPIP_SID_BIND, TCPIP_E_ADDRINUSE);
            }
            else
            {
                TcpIp_DetReportError(TCPIP_SID_BIND, TCPIP_E_ADDRNOTAVAIL);
            }
#endif
        }
    }
    else
    {
        TcpIp_ExitExclusiveArea();
    }

    return ret;
}

#if (STD_ON == TCPIP_TCP_ENABLED)
Std_ReturnType
    TcpIp_InnerTcpConnect(const TcpIp_SocketRuntimeType* socketRtPtr, const TcpIp_SockAddrType* remoteAddrPtr)
{
    Std_ReturnType ret      = E_NOT_OK;
    err_t          err      = (err_t)ERR_ENUM_MAX;
    ip_addr_t      remoteIp = {0};
    uint16         port     = remoteAddrPtr->port;

    TCPIP_LOCAL_TO_LWIP_IPADDR(remoteIp.addr, remoteAddrPtr->addr); /* PRQA S 3473 */ /* VL_TcpIp_3473 */

    TcpIp_EnterExclusiveArea();
#if defined(TCPIP_INNER_CHECK)
    if ((NULL_PTR != socketRtPtr->PcbPtr) && (TCPIP_IPPROTO_TCP == socketRtPtr->Protocol)
        && (TCPIP_CTRL_INDEX_INVALID > socketRtPtr->CtrlIndex))
#endif
    {
        uint8                        ctrlIndex  = socketRtPtr->CtrlIndex;
        const TcpIp_CtrlType*        ctrlCfgPtr = &TCPIP_CTRL_CONFIG(ctrlIndex);
        const TcpIp_CtrlRuntimeType* ctrlRtPtr  = ctrlCfgPtr->RuntimePtr;

        if (TCPIP_STATE_ONLINE == ctrlRtPtr->CtrlStatus)
        {
            struct tcp_pcb* pcbPtr = (struct tcp_pcb*)socketRtPtr->PcbPtr;

#if (STD_ON == TCPIP_TCP_TLS_ENABLED)
            if (NULL_PTR == socketRtPtr->tlsPtr)
            {
                TcpIp_TlsBaseStaticConnect(socketRtPtr, remoteAddrPtr, TLS_CLIENT);
            }
            if (NULL_PTR != socketRtPtr->tlsPtr)
            {
                err = TcpIp_TlsConnet(pcbPtr, &remoteIp, port);
            }
            else
#endif
            {
                err = tcp_connect(pcbPtr, &remoteIp, (u16_t)port, TcpIp_TcpConnnectCallback);
            }
        }
    }
    TcpIp_ExitExclusiveArea();

    if ((err_t)ERR_OK == err)
    {
        ret = E_OK;
    }

    return ret;
}

Std_ReturnType TcpIp_InnerTcpListen(TcpIp_SocketRuntimeType* socketRtPtr, uint8 maxChannNum)
{
    Std_ReturnType ret = E_NOT_OK;

    TcpIp_EnterExclusiveArea();
#if defined(TCPIP_INNER_CHECK)
    if ((NULL_PTR != socketRtPtr->PcbPtr) && (TCPIP_IPPROTO_TCP == socketRtPtr->Protocol)
        && (TCPIP_CTRL_INDEX_INVALID > socketRtPtr->CtrlIndex))
#endif
    {
        uint8                        ctrlIndex  = socketRtPtr->CtrlIndex;
        const TcpIp_CtrlType*        ctrlCfgPtr = &TCPIP_CTRL_CONFIG(ctrlIndex);
        const TcpIp_CtrlRuntimeType* ctrlRtPtr  = ctrlCfgPtr->RuntimePtr;

        if (TCPIP_STATE_ONLINE == ctrlRtPtr->CtrlStatus)
        {
            err_t           err    = (err_t)ERR_ENUM_MAX;
            struct tcp_pcb* pcbPtr = (struct tcp_pcb*)socketRtPtr->PcbPtr;

            /* listen pcb shall be remalloc, so lwip pcb free handle not release local val */
            tcp_ext_arg_set(pcbPtr, 0u, NULL_PTR);
            tcp_ext_arg_set_callbacks(
                pcbPtr,
                0u,
                &TcpIp_TcpExtArgCallbackTable[TCPIP_TCPEXTARG_CALLBACK_INDEX_DISABLED]);

            socketRtPtr->PcbPtr = tcp_listen_with_backlog_and_err(pcbPtr, (u8_t)maxChannNum, &err);

            if ((NULL_PTR != socketRtPtr->PcbPtr)
#if defined(TCPIP_INNER_CHECK)
                && (((err_t)ERR_ALREADY == err) || ((err_t)ERR_OK == err))
#endif
            )
            {
                TcpIp_SocketFlagSet(socketRtPtr, TCPIP_SOCKET_FLAG_TCP_SERVERSOCKET);

                pcbPtr = (struct tcp_pcb*)socketRtPtr->PcbPtr;
                tcp_ext_arg_set(pcbPtr, 0u, socketRtPtr);
                tcp_ext_arg_set_callbacks(
                    pcbPtr,
                    0u,
                    &TcpIp_TcpExtArgCallbackTable[TCPIP_TCPEXTARG_CALLBACK_INDEX_ENABLED]);
                tcp_arg(pcbPtr, socketRtPtr);
                tcp_accept(pcbPtr, TcpIp_TcpAcceptCallback);

                ret = E_OK;
            }
            else
#if defined(TCPIP_INNER_CHECK)
                if ((NULL_PTR == socketRtPtr->PcbPtr) && (((err_t)ERR_USE == err) || ((err_t)ERR_MEM == err)))
#endif
            {
                socketRtPtr->PcbPtr = pcbPtr;
            }
#if defined(TCPIP_INNER_CHECK)
            else
            {
                /* Do nothing */
            }
#endif
        }
    }
    TcpIp_ExitExclusiveArea();

    return ret;
}

Std_ReturnType TcpIp_InnerTcpReceived(const TcpIp_SocketRuntimeType* socketRtPtr, uint16 recvLen)
{
    Std_ReturnType ret = E_NOT_OK;

    TcpIp_EnterExclusiveArea();
#if defined(TCPIP_INNER_CHECK)
    if ((NULL_PTR != socketRtPtr->PcbPtr) && (TCPIP_IPPROTO_TCP == socketRtPtr->Protocol))
#endif
    {
#if (STD_ON == TCPIP_TCP_TLS_ENABLED)
        if (NULL_PTR != socketRtPtr->tlsPtr)
        {
            ret = TcpIp_TlsRecved(socketRtPtr, recvLen);
        }
        else
#endif
        {
            tcp_recved((struct tcp_pcb*)socketRtPtr->PcbPtr, (u16_t)recvLen);

            ret = E_OK;
        }
    }
    TcpIp_ExitExclusiveArea();

    return ret;
}
#endif

Std_ReturnType TcpIp_InnerRequestComModeOnline(
    const TcpIp_CtrlType*  ctrlCfgPtr,
    TcpIp_CtrlRuntimeType* ctrlRtPtr,
    TcpIp_StateType        reqState)
{
    Std_ReturnType ret = E_NOT_OK;

    TcpIp_EnterExclusiveArea();
#if defined(TCPIP_INNER_CHECK)
    if (TCPIP_STATE_ONLINE == ctrlRtPtr->CtrlStatus)
#endif
    {
        if (TCPIP_STATE_ONHOLD == reqState)
        {
            TcpIp_LocalIpAddrAssignChgHandleForCtrl(ctrlCfgPtr, ctrlRtPtr, TCPIP_IPADDR_STATE_ONHOLD, TRUE);

            ctrlRtPtr->CtrlStatus = TCPIP_STATE_ONHOLD;
            TcpIp_EthSmModeIndicationNotifyForCtrl(ctrlCfgPtr, TCPIP_STATE_ONHOLD);
            TcpIp_ExitExclusiveArea();

            ret = E_OK;
        }
        else if (TCPIP_STATE_OFFLINE == reqState)
        {
            TcpIp_SocketIdType pendingSocketNum = TcpIp_CloseAllSocket(ctrlRtPtr);

            ctrlRtPtr->CtrlStatus = TCPIP_STATE_SHUTDOWN;
            TcpIp_EthSmModeIndicationNotifyForCtrl(ctrlCfgPtr, TCPIP_STATE_SHUTDOWN);
            if (0u < pendingSocketNum)
            {
                TcpIp_CtrlStateSetPending(ctrlRtPtr, TCPIP_STATE_SHUTDOWN);
            }

            TcpIp_ReleaseIpAddrAssignmentForCtrl(ctrlCfgPtr, ctrlRtPtr);

            if (0u == pendingSocketNum)
            {
                struct netif* netifPtr = &(ctrlRtPtr->Netif);

                ctrlRtPtr->CtrlStatus = TCPIP_STATE_OFFLINE;
                TcpIp_EthSmModeIndicationNotifyForCtrl(ctrlCfgPtr, TCPIP_STATE_OFFLINE);

                netif_set_down(netifPtr);
                netif_set_link_down(netifPtr);
            }
            TcpIp_ExitExclusiveArea();

            TcpIp_UpTcpIpEventNotify();

            ret = E_OK;
        }
        else
        {
            TcpIp_ExitExclusiveArea();
        }
    }
#if defined(TCPIP_INNER_CHECK)
    else
    {
        TcpIp_ExitExclusiveArea();
    }
#endif

    return ret;
}

Std_ReturnType TcpIp_InnerRequestComModeOnhold(
    const TcpIp_CtrlType*  ctrlCfgPtr,
    TcpIp_CtrlRuntimeType* ctrlRtPtr,
    TcpIp_StateType        reqState)
{
    Std_ReturnType ret = E_NOT_OK;

    TcpIp_EnterExclusiveArea();
#if defined(TCPIP_INNER_CHECK)
    if (TCPIP_STATE_ONHOLD == ctrlRtPtr->CtrlStatus)
#endif
    {
        if (TCPIP_STATE_ONLINE == reqState)
        {
            TcpIp_LocalIpAddrAssignChgHandleForCtrl(ctrlCfgPtr, ctrlRtPtr, TCPIP_IPADDR_STATE_ASSIGNED, TRUE);

            ctrlRtPtr->CtrlStatus = TCPIP_STATE_ONLINE;
            TcpIp_EthSmModeIndicationNotifyForCtrl(ctrlCfgPtr, TCPIP_STATE_ONLINE);
            TcpIp_ExitExclusiveArea();

            ret = E_OK;
        }
        else if (TCPIP_STATE_OFFLINE == reqState)
        {
            TcpIp_SocketIdType pendingSocketNum = TcpIp_CloseAllSocket(ctrlRtPtr);

            ctrlRtPtr->CtrlStatus = TCPIP_STATE_SHUTDOWN;
            TcpIp_EthSmModeIndicationNotifyForCtrl(ctrlCfgPtr, TCPIP_STATE_SHUTDOWN);
            if (0u < pendingSocketNum)
            {
                TcpIp_CtrlStateSetPending(ctrlRtPtr, TCPIP_STATE_SHUTDOWN);
            }

            TcpIp_ReleaseIpAddrAssignmentForCtrl(ctrlCfgPtr, ctrlRtPtr);

            if (0u == pendingSocketNum)
            {
                struct netif* netifPtr = &(ctrlRtPtr->Netif);

                ctrlRtPtr->CtrlStatus = TCPIP_STATE_OFFLINE;
                TcpIp_EthSmModeIndicationNotifyForCtrl(ctrlCfgPtr, TCPIP_STATE_OFFLINE);

                netif_set_down(netifPtr);
                netif_set_link_down(netifPtr);
            }
            TcpIp_ExitExclusiveArea();

            TcpIp_UpTcpIpEventNotify();

            ret = E_OK;
        }
        else
        {
            TcpIp_ExitExclusiveArea();
        }
    }
#if defined(TCPIP_INNER_CHECK)
    else
    {
        TcpIp_ExitExclusiveArea();
    }
#endif

    return ret;
}

Std_ReturnType TcpIp_InnerRequestComModeOffline(
    const TcpIp_CtrlType*  ctrlCfgPtr,
    TcpIp_CtrlRuntimeType* ctrlRtPtr,
    TcpIp_StateType        reqState)
{
    Std_ReturnType ret = E_NOT_OK;

    if (TCPIP_STATE_ONLINE == reqState)
    {
        struct netif* netifPtr = &(ctrlRtPtr->Netif);

#if TCPIP_REPEAT_GET_NETIF_MAC
        TcpIp_NetIfGetPhyAddrFromEthIf(netifPtr);
#endif

        TcpIp_EnterExclusiveArea();
#if defined(TCPIP_INNER_CHECK)
        if (TCPIP_STATE_OFFLINE == ctrlRtPtr->CtrlStatus)
#endif
        {
            netif_set_up(netifPtr);

            ctrlRtPtr->CtrlStatus = TCPIP_STATE_STARTUP;
            TcpIp_EthSmModeIndicationNotifyForCtrl(ctrlCfgPtr, TCPIP_STATE_STARTUP);

            TcpIp_RequestIpAddrAssignmentAutomaticForCtrl(ctrlCfgPtr, ctrlRtPtr);

            netif_set_link_up(netifPtr);
            TcpIp_ExitExclusiveArea();

            ret = E_OK;
        }
#if defined(TCPIP_INNER_CHECK)
        else
        {
            TcpIp_ExitExclusiveArea();
        }
#endif
    }

    return ret;
}

Std_ReturnType TcpIp_InnerRequestComModeStartup(
    const TcpIp_CtrlType*  ctrlCfgPtr,
    TcpIp_CtrlRuntimeType* ctrlRtPtr,
    TcpIp_StateType        reqState)
{
    Std_ReturnType ret = E_NOT_OK;

    if (TCPIP_STATE_OFFLINE == reqState)
    {
        struct netif* netifPtr = &(ctrlRtPtr->Netif);

        TcpIp_EnterExclusiveArea();
#if defined(TCPIP_INNER_CHECK)
        if (TCPIP_STATE_STARTUP == ctrlRtPtr->CtrlStatus)
#endif
        {
            ctrlRtPtr->CtrlStatus = TCPIP_STATE_OFFLINE;
            TcpIp_EthSmModeIndicationNotifyForCtrl(ctrlCfgPtr, TCPIP_STATE_OFFLINE);

            TcpIp_ReleaseIpAddrAssignmentForCtrl(ctrlCfgPtr, ctrlRtPtr);

            netif_set_down(netifPtr);
            netif_set_link_down(netifPtr);
            TcpIp_ExitExclusiveArea();

            ret = E_OK;
        }
#if defined(TCPIP_INNER_CHECK)
        else
        {
            TcpIp_ExitExclusiveArea();
        }
#endif
    }

    return ret;
}

Std_ReturnType TcpIp_InnerRequestIpAddrAssignment(
    const TcpIp_CtrlType*      ctrlCfgPtr,
    TcpIp_CtrlRuntimeType*     ctrlRtPtr,
    const TcpIp_LocalAddrType* localAddrCfgPtr,
    TcpIp_IpAddrAssignmentType Type,
    const TcpIp_SockAddrType*  LocalIpAddrPtr,
    uint8                      Netmask,
    const TcpIp_SockAddrType*  DefaultRouterPtr)
{
    Std_ReturnType ret = E_NOT_OK;

    for (uint8 addrAssignIndex = 0u; addrAssignIndex < localAddrCfgPtr->AddrAssignNum; ++addrAssignIndex)
    {
        const TcpIp_AddrAssignmentType* addrAssignCfgPtr = &(localAddrCfgPtr->AddrAssignPtr[addrAssignIndex]);
        TcpIp_IpAddrAssignmentType      assignType       = addrAssignCfgPtr->AssignMethod;

        if (((assignType == Type) || (TCPIP_IPADDR_ASSIGNMENT_ALL == Type))
            && (TCPIP_ASSIGN_TRIGGER_MANUAL == addrAssignCfgPtr->AssignTrigger))
        {
            const TcpIp_StaticIpAddressConfigType* ipAddrCfgPtr = NULL_PTR;
            TcpIp_StaticIpAddressConfigType        ipAddrCfg    = {0};
#if (TCPIP_SC1 == TCPIP_SCALABILITY_CLASS)
            uint32 netmask[1] = {0};
#else
            uint32 netmask[4] = {0};
#endif

            if ((TCPIP_IPADDR_ASSIGNMENT_STATIC == assignType) && (NULL_PTR == localAddrCfgPtr->IpAddrCfgPtr)
                && (NULL_PTR != LocalIpAddrPtr))
            {
                if (NULL_PTR != DefaultRouterPtr)
                {
                    ipAddrCfg.DefaultRouterPtr = DefaultRouterPtr->addr;
                }
                TcpIp_NetMaskExtToInner(Netmask, localAddrCfgPtr->DomainType, netmask);
                ipAddrCfg.NetmaskPtr      = netmask;
                ipAddrCfg.StaticIpAddrPtr = LocalIpAddrPtr->addr;
                ipAddrCfgPtr              = &ipAddrCfg;
            }

            TcpIp_EnterExclusiveArea();
#if defined(TCPIP_INNER_CHECK)
            if (TCPIP_STATE_OFFLINE != ctrlRtPtr->CtrlStatus)
#endif
            {
                ret = TcpIp_DoRequestIpAddrAssignment(ctrlCfgPtr, ctrlRtPtr, localAddrCfgPtr, assignType, ipAddrCfgPtr);
            }
            TcpIp_ExitExclusiveArea();
            break;
        }
    }

    return ret;
}

Std_ReturnType TcpIp_InnerReleaseIpAddrAssignment(
    const TcpIp_CtrlType*      ctrlCfgPtr,
    TcpIp_CtrlRuntimeType*     ctrlRtPtr,
    const TcpIp_LocalAddrType* localAddrCfgPtr)
{
    Std_ReturnType ret = E_NOT_OK;

    for (uint8 addrAssignIndex = 0u; addrAssignIndex < localAddrCfgPtr->AddrAssignNum; ++addrAssignIndex)
    {
        const TcpIp_AddrAssignmentType* addrAssignCfgPtr = &(localAddrCfgPtr->AddrAssignPtr[addrAssignIndex]);

        if (TCPIP_ASSIGN_TRIGGER_MANUAL == addrAssignCfgPtr->AssignTrigger)
        {
            TcpIp_EnterExclusiveArea();
#if defined(TCPIP_INNER_CHECK)
            if (TCPIP_STATE_OFFLINE != ctrlRtPtr->CtrlStatus)
#endif
            {
                ret = TcpIp_DoReleaseIpAddrAssignment(
                    ctrlCfgPtr,
                    ctrlRtPtr,
                    localAddrCfgPtr,
                    addrAssignCfgPtr->AssignMethod);
            }
            TcpIp_ExitExclusiveArea();
        }

        if (E_OK != ret)
        {
            break;
        }
    }

    return ret;
}

#if (STD_ON == TCPIP_ICMP_ENABLED)
#if ((TCPIP_SC1 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS))
Std_ReturnType TcpIp_InnerIcmpTransmit(TcpIp_LocalAddrIdType localAddrId, const TcpIp_IcmpTransDataType* icmpParamPtr)
{
    Std_ReturnType                    ret             = E_NOT_OK;
    const TcpIp_LocalAddrType*        localAddrCfgPtr = &TCPIP_LOCALADDR_CONFIG(localAddrId);
    uint8                             ctrlIndex       = localAddrCfgPtr->CtrlIndex;
    const TcpIp_CtrlType*             ctrlCfgPtr      = &TCPIP_CTRL_CONFIG(ctrlIndex);
    TcpIp_CtrlRuntimeType*            ctrlRtPtr       = ctrlCfgPtr->RuntimePtr;
    const TcpIp_LocalAddrRuntimeType* localAddrRtPtr  = localAddrCfgPtr->RuntimePtr;

    TcpIp_EnterExclusiveArea();
    if (TCPIP_STATE_ONLINE == ctrlRtPtr->CtrlStatus)
    {
        err_t err = (err_t)ERR_ENUM_MAX;

        if (NULL_PTR == ctrlRtPtr->IcmpPcbPtr)
        {
            ctrlRtPtr->IcmpPcbPtr = raw_new((u8_t)IP_PROTO_ICMP);
        }
        if (NULL_PTR != ctrlRtPtr->IcmpPcbPtr)
        {
            struct raw_pcb*     pcbPtr   = ctrlRtPtr->IcmpPcbPtr;
            const struct netif* netifPtr = &(ctrlRtPtr->Netif);

            err = raw_bind(pcbPtr, &(localAddrRtPtr->LocalIp));
            raw_bind_netif(pcbPtr, netifPtr);

#if defined(TCPIP_INNER_CHECK)
            if ((err_t)ERR_OK == err)
#endif
            {
                uint16       len     = (uint16)sizeof(struct icmp_echo_hdr) + icmpParamPtr->DataLength;
                struct pbuf* pbufPtr = pbuf_alloc(PBUF_IP, (u16_t)len, PBUF_RAM);

#if defined(TCPIP_INNER_CHECK)
                if (NULL_PTR != pbufPtr)
#endif
                {
                    struct icmp_echo_hdr* icmphdr = (struct icmp_echo_hdr*)pbufPtr->payload;

                    icmphdr->type   = icmpParamPtr->Type;
                    icmphdr->code   = icmpParamPtr->Code;
                    icmphdr->chksum = 0u;
                    icmphdr->id     = 0u;
                    icmphdr->seqno  = 0u;

                    pcbPtr->ttl = icmpParamPtr->Ttl;

                    err = pbuf_take_at(
                        pbufPtr,
                        icmpParamPtr->DataPtr,
                        icmpParamPtr->DataLength,
                        (uint16)sizeof(struct icmp_echo_hdr));

#if CHECKSUM_GEN_ICMP
                    IF__NETIF_CHECKSUM_ENABLED(netifPtr, NETIF_CHECKSUM_GEN_ICMP)
                    {
                        icmphdr->chksum = inet_chksum(icmphdr, pbufPtr->len);
                    }
#endif
                }
#if defined(TCPIP_INNER_CHECK)
                else
                {
                    err = (err_t)ERR_MEM;
                }
#endif

                if ((err_t)ERR_OK == err)
                {
                    err = raw_sendto(pcbPtr, pbufPtr, &(icmpParamPtr->RemoteIp));
                }
                if (NULL_PTR != pbufPtr)
                {
                    (void)pbuf_free(pbufPtr);
                    pbufPtr = NULL_PTR;
                }
            }
        }

        if ((err_t)ERR_OK == err)
        {
            ret = E_OK;
        }
    }
    TcpIp_ExitExclusiveArea();

    return ret;
}
#endif

#if ((TCPIP_SC2 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS))
Std_ReturnType TcpIp_InnerIcmpV6Transmit(void)
{
    Std_ReturnType ret = E_NOT_OK;

    return ret;
}
#endif
#endif

Std_ReturnType TcpIp_InnerChangeParameterFramePrio(
    TcpIp_SocketRuntimeType* socketRtPtr,
    TcpIp_ParamIdType        paramId,
    const uint8*             paramValue)
{
    Std_ReturnType ret    = E_NOT_OK;
    uint8          vluTos = (uint8)(((*(const uint8*)paramValue) & 0x07u) << 5u);

    TCPIP_UNUSED_ARG(paramId);

    TcpIp_EnterExclusiveArea();
#if defined(TCPIP_INNER_CHECK)
    if (NULL_PTR != socketRtPtr->PcbPtr)
#endif
    {
#if (STD_ON == TCPIP_TCP_ENABLED)
        if (TCPIP_IPPROTO_TCP == socketRtPtr->Protocol)
        {
            struct tcp_pcb* pcbPtr = (struct tcp_pcb*)socketRtPtr->PcbPtr;

            pcbPtr->tos &= 0x1Fu;
            pcbPtr->tos |= vluTos;

            ret = E_OK;
        }
#endif

#if (STD_ON == TCPIP_UDP_ENABLED)
        if (TCPIP_IPPROTO_UDP == socketRtPtr->Protocol)
        {
            struct udp_pcb* pcbPtr = (struct udp_pcb*)socketRtPtr->PcbPtr;

            pcbPtr->tos &= 0x1Fu;
            pcbPtr->tos |= vluTos;

            ret = E_OK;
        }
#endif
    }
    TcpIp_ExitExclusiveArea();

    return ret;
}

#if (STD_ON == TCPIP_TCP_ENABLED)
Std_ReturnType TcpIp_InnerChangeParameterTcpNagle(
    TcpIp_SocketRuntimeType* socketRtPtr,
    TcpIp_ParamIdType        paramId,
    const uint8*             paramValue)
{
    Std_ReturnType ret = E_NOT_OK;

    TCPIP_UNUSED_ARG(paramId);

    TcpIp_EnterExclusiveArea();
#if defined(TCPIP_INNER_CHECK)
    if ((NULL_PTR != socketRtPtr->PcbPtr) && (TCPIP_IPPROTO_TCP == socketRtPtr->Protocol))
#endif
    {
        struct tcp_pcb* pcbPtr = (struct tcp_pcb*)(socketRtPtr->PcbPtr);

        if (LISTEN != pcbPtr->state)
        {
            if (0u < *((const uint8*)paramValue))
            {
                tcp_nagle_enable(pcbPtr); /* PRQA S 2743 */ /* VL_TcpIp_2743 */
            }
            else
            {
                tcp_nagle_disable(pcbPtr); /* PRQA S 2743 */ /* VL_TcpIp_2743 */
            }

            ret = E_OK;
        }
    }
    TcpIp_ExitExclusiveArea();

    return ret;
}

Std_ReturnType TcpIp_InnerChangeParameterTcpKeepAlive(
    TcpIp_SocketRuntimeType* socketRtPtr,
    TcpIp_ParamIdType        paramId,
    const uint8*             paramValue)
{
    Std_ReturnType ret = E_NOT_OK;

    TcpIp_EnterExclusiveArea();
#if defined(TCPIP_INNER_CHECK)
    if ((NULL_PTR != socketRtPtr->PcbPtr) && (TCPIP_IPPROTO_TCP == socketRtPtr->Protocol))
#endif
    {
        struct tcp_pcb* pcbPtr = (struct tcp_pcb*)(socketRtPtr->PcbPtr);

        switch (paramId)
        {
        case TCPIP_PARAMID_TCP_KEEPALIVE:
            if (0u < *((const uint8*)paramValue))
            {
                ip_set_option(pcbPtr, SOF_KEEPALIVE);
            }
            else
            {
                ip_reset_option(pcbPtr, SOF_KEEPALIVE);
            }

            ret = E_OK;
            break;

        case TCPIP_PARAMID_TCP_KEEPALIVE_TIME:
            pcbPtr->keep_idle = TCPIP_CHANGEPARAMETER_DECODE_U32(paramValue);

            ret = E_OK;
            break;

#if LWIP_TCP_KEEPALIVE
        case TCPIP_PARAMID_TCP_KEEPALIVE_PROBES_MAX:
            pcbPtr->keep_cnt = TCPIP_CHANGEPARAMETER_DECODE_U16(paramValue);

            ret = E_OK;
            break;

        case TCPIP_PARAMID_TCP_KEEPALIVE_INTERVAL:
            pcbPtr->keep_intvl = TCPIP_CHANGEPARAMETER_DECODE_U32(paramValue);

            ret = E_OK;
            break;
#endif

        default:
            /* do nothing */
            break;
        }
    }
    TcpIp_ExitExclusiveArea();

    return ret;
}
#endif

Std_ReturnType TcpIp_InnerChangeParameterTtl(
    TcpIp_SocketRuntimeType* socketRtPtr,
    TcpIp_ParamIdType        paramId,
    const uint8*             paramValue)
{
    Std_ReturnType ret = E_NOT_OK;

    TCPIP_UNUSED_ARG(paramId);

    TcpIp_EnterExclusiveArea();
#if defined(TCPIP_INNER_CHECK)
    if (NULL_PTR != socketRtPtr->PcbPtr)
#endif
    {
#if (STD_ON == TCPIP_TCP_ENABLED)
        if (TCPIP_IPPROTO_TCP == socketRtPtr->Protocol)
        {
            struct tcp_pcb* pcbPtr = (struct tcp_pcb*)socketRtPtr->PcbPtr;

            pcbPtr->ttl = *((const uint8*)paramValue);

            ret = E_OK;
        }
#endif

#if (STD_ON == TCPIP_UDP_ENABLED)
        if (TCPIP_IPPROTO_UDP == socketRtPtr->Protocol)
        {
            struct udp_pcb* pcbPtr = (struct udp_pcb*)socketRtPtr->PcbPtr;

            pcbPtr->ttl = *((const uint8*)paramValue);

            ret = E_OK;
        }
#endif
    }
    TcpIp_ExitExclusiveArea();

    return ret;
}

#if (STD_ON == TCPIP_SOCKET_VLAN)
Std_ReturnType TcpIp_InnerChangeParameterSocketVlan(
    TcpIp_SocketRuntimeType* socketRtPtr,
    TcpIp_ParamIdType        paramId,
    const uint8*             paramValue)
{
    Std_ReturnType ret = E_NOT_OK;

    TCPIP_UNUSED_ARG(paramId);

    TcpIp_EnterExclusiveArea();
#if defined(TCPIP_INNER_CHECK)
    if (NULL_PTR != socketRtPtr->PcbPtr)
#endif
    {
#if (STD_ON == TCPIP_TCP_ENABLED)
        if (TCPIP_IPPROTO_TCP == socketRtPtr->Protocol)
        {
            struct tcp_pcb* pcbPtr = (struct tcp_pcb*)socketRtPtr->PcbPtr;

            pcbPtr->netif_hints.tci = TCPIP_CHANGEPARAMETER_DECODE_U32(paramValue);

            ret = E_OK;
        }
#endif

#if (STD_ON == TCPIP_UDP_ENABLED)
        if (TCPIP_IPPROTO_UDP == socketRtPtr->Protocol)
        {
            struct udp_pcb* pcbPtr = (struct udp_pcb*)socketRtPtr->PcbPtr;

            pcbPtr->netif_hints.tci = TCPIP_CHANGEPARAMETER_DECODE_U32(paramValue);

            ret = E_OK;
        }
#endif
    }
    TcpIp_ExitExclusiveArea();

    return ret;
}
#endif

TcpIp_ReturnType TcpIp_InnerGetRemotePhysAddrByArp(
    TcpIp_CtrlRuntimeType*    ctrlRtPtr,
    const TcpIp_SockAddrType* IpAddrPtr,
    uint8*                    PhysAddrPtr,
    boolean                   initRes)
{
    TcpIp_ReturnType ret      = TCPIP_E_PHYS_ADDR_MISS;
    struct netif*    netifPtr = &(ctrlRtPtr->Netif);
    ip4_addr_t       remoteIp = {
              .addr = IpAddrPtr->addr[0],
    };
    struct eth_addr*  ethAddrPtr = NULL_PTR;
    const ip4_addr_t* ipPtr      = NULL_PTR;

    if (!ip4_addr_net_eq(&remoteIp, netif_ip4_addr(netifPtr), netif_ip4_netmask(netifPtr)))
    {
        ret = TCPIP_E_NOT_OK;
    }
    else if (0 <= etharp_find_addr(netifPtr, &remoteIp, &ethAddrPtr, &ipPtr))
    {
        (void)IStdLib_MemCpy(PhysAddrPtr, ethAddrPtr->addr, (uint32)ETH_HWADDR_LEN);

        ret = TCPIP_E_OK;
    }
    else if (initRes)
    {
        if ((err_t)ERR_OK != etharp_request(netifPtr, &remoteIp))
        {
            ret = TCPIP_E_NOT_OK;
        }
    }
    else
    {
        ret = TCPIP_E_NOT_OK;
    }

    return ret;
}

TcpIp_ReturnType
    TcpIp_InnerIsConnectionReady(const TcpIp_SocketRuntimeType* socketRtPtr, const TcpIp_SockAddrType* RemoteAddrPtr)
{
    TcpIp_ReturnType ret = TCPIP_E_NOT_OK;

    TcpIp_EnterExclusiveArea();
    if ((NULL_PTR != socketRtPtr->PcbPtr) && (TCPIP_CTRL_INDEX_INVALID > socketRtPtr->CtrlIndex))
    {
        const TcpIp_CtrlType*  ctrlCfgPtr = &TCPIP_CTRL_CONFIG(socketRtPtr->CtrlIndex);
        TcpIp_CtrlRuntimeType* ctrlRtPtr  = ctrlCfgPtr->RuntimePtr;

        if (TCPIP_STATE_ONLINE == ctrlRtPtr->CtrlStatus)
        {
            struct netif* netifPtr = &(ctrlRtPtr->Netif);
            ip4_addr_t    remoteIp = {
                   .addr = RemoteAddrPtr->addr[0],
            };
            struct eth_addr*  ethAddrPtr = NULL_PTR;
            const ip4_addr_t* ipPtr      = NULL_PTR;

#if (STD_ON == TCPIP_TCP_ENABLED)
            if (TCPIP_IPPROTO_TCP == socketRtPtr->Protocol)
            {
                const struct tcp_pcb* pcbPtr = (struct tcp_pcb*)socketRtPtr->PcbPtr;

                if (0 <= etharp_find_addr(netifPtr, &remoteIp, &ethAddrPtr, &ipPtr))
                {
                    ret = TCPIP_E_OK;
                }
                if ((SYN_SENT == pcbPtr->state) || (SYN_RCVD == pcbPtr->state))
                {
                    ret = TCPIP_E_PENDING;
                }
            }
#endif

#if (STD_ON == TCPIP_UDP_ENABLED)
            if (TCPIP_IPPROTO_UDP == socketRtPtr->Protocol)
            {
                if (0 <= etharp_find_addr(netifPtr, &remoteIp, &ethAddrPtr, &ipPtr))
                {
                    ret = TCPIP_E_OK;
                }
            }
#endif
        }
    }
    TcpIp_ExitExclusiveArea();

    return ret;
}

#if (STD_ON == TCPIP_UDP_ENABLED)
Std_ReturnType TcpIp_InnerUdpTransmitDirect(
    const TcpIp_SocketRuntimeType* socketRtPtr,
    const uint8*                   dataPtr,
    const TcpIp_SockAddrType*      remoteAddrPtr,
    uint16                         totalLength)
{
    Std_ReturnType ret      = E_NOT_OK;
    err_t          err      = (err_t)ERR_ENUM_MAX;
    ip_addr_t      destIp   = {0};
    u16_t          destPort = remoteAddrPtr->port;

    TCPIP_LOCAL_TO_LWIP_IPADDR(destIp.addr, remoteAddrPtr->addr); /* PRQA S 3473 */ /* VL_TcpIp_3473 */

    TcpIp_EnterExclusiveArea();
#if defined(TCPIP_INNER_CHECK)
    if ((NULL_PTR != socketRtPtr->PcbPtr) && (TCPIP_IPPROTO_UDP == socketRtPtr->Protocol))
#endif
    {
        struct pbuf* pbufPtr = pbuf_alloc(PBUF_TRANSPORT, (u16_t)totalLength, PBUF_REF);

#if defined(TCPIP_INNER_CHECK)
        if (NULL_PTR != pbufPtr)
#endif
        {
            struct udp_pcb* pcbPtr = (struct udp_pcb*)socketRtPtr->PcbPtr;

            pbufPtr->payload = (void*)dataPtr; /* PRQA S 0311 */ /* VL_TcpIp_0311 */
            err              = udp_sendto(pcbPtr, pbufPtr, &destIp, destPort);

            (void)pbuf_free(pbufPtr);
            pbufPtr = NULL_PTR;
        }
    }
    TcpIp_ExitExclusiveArea();

    if ((err_t)ERR_OK == err)
    {
        ret = E_OK;
    }

    return ret;
}

Std_ReturnType TcpIp_InnerUdpTransmitCopy(
    const TcpIp_SocketRuntimeType* socketRtPtr,
    const TcpIp_SockAddrType*      remoteAddrPtr,
    uint16                         totalLength)
{
    Std_ReturnType ret = E_NOT_OK;
    err_t          err = (err_t)ERR_ENUM_MAX;

    TcpIp_EnterExclusiveArea();
    struct udp_pcb*                  pcbPtr      = (struct udp_pcb*)socketRtPtr->PcbPtr;
    const TcpIp_UpCopyTxDataFuncType copyFuncPtr = socketRtPtr->OwnerCfgPtr->Up_CopyTxDataFuncPtr;

    if (
#if defined(TCPIP_INNER_CHECK)
        (NULL_PTR != pcbPtr) && (TCPIP_IPPROTO_UDP == socketRtPtr->Protocol) &&
#endif
        (NULL_PTR != copyFuncPtr))
    {
        struct pbuf* pbufPtr = pbuf_alloc(PBUF_TRANSPORT, (u16_t)totalLength, PBUF_RAM);
        TcpIp_ExitExclusiveArea();

#if defined(TCPIP_INNER_CHECK)
        if ((NULL_PTR != pbufPtr) && (totalLength == (uint16)pbufPtr->len) && (pbufPtr->len == pbufPtr->tot_len)
            && (NULL_PTR == pbufPtr->next))
#endif
        {
            BufReq_ReturnType bufRet =
                copyFuncPtr(socketRtPtr->SocketId, (uint8*)pbufPtr->payload, (uint16)pbufPtr->len);
            ip_addr_t destIp   = {0};
            u16_t     destPort = remoteAddrPtr->port;

            TCPIP_LOCAL_TO_LWIP_IPADDR(destIp.addr, remoteAddrPtr->addr); /* PRQA S 3473 */ /* VL_TcpIp_3473 */

            TcpIp_EnterExclusiveArea();
            if ((BUFREQ_OK == bufRet)
#if defined(TCPIP_INNER_CHECK)
                && (pcbPtr == socketRtPtr->PcbPtr)
#endif
            )
            {
                err = udp_sendto(pcbPtr, pbufPtr, &destIp, destPort);
            }
        }
#if defined(TCPIP_INNER_CHECK)
        else
        {
            TcpIp_EnterExclusiveArea();
        }

        if (NULL_PTR != pbufPtr)
#endif
        {
            (void)pbuf_free(pbufPtr);
            pbufPtr = NULL_PTR;
        }
    }
    TcpIp_ExitExclusiveArea();

    if ((err_t)ERR_OK == err)
    {
        ret = E_OK;
    }

    return ret;
}
#endif

#if (STD_ON == TCPIP_TCP_ENABLED)
Std_ReturnType
    TcpIp_InnerTcpTransmitDirect(const TcpIp_SocketRuntimeType* socketRtPtr, const uint8* dataPtr, uint16 totalLength)
{
    Std_ReturnType ret = E_NOT_OK;
    err_t          err = (err_t)ERR_ENUM_MAX;

    TcpIp_EnterExclusiveArea();
#if defined(TCPIP_INNER_CHECK)
    if ((NULL_PTR != socketRtPtr->PcbPtr) && (TCPIP_IPPROTO_TCP == socketRtPtr->Protocol))
#endif
    {
#if (STD_ON == TCPIP_TCP_TLS_ENABLED)
        if (NULL_PTR != socketRtPtr->tlsPtr)
        {
            err = (err_t)TcpIp_TlsWrite(socketRtPtr, dataPtr, totalLength);
        }
        else
#endif
        {
            struct tcp_pcb* pcbPtr = (struct tcp_pcb*)socketRtPtr->PcbPtr;

            err = tcp_write(pcbPtr, dataPtr, (u16_t)totalLength, TCPIP_TCP_WRITE_API_FLAG);
#if TCPIP_FASTTX_TCP
            err += tcp_output(pcbPtr);
#endif
        }
    }
    TcpIp_ExitExclusiveArea();

    if ((err_t)ERR_OK == err)
    {
        ret = E_OK;
    }

    return ret;
}

Std_ReturnType
    TcpIp_InnerTcpTransmitCopy(TcpIp_SocketRuntimeType* socketRtPtr, uint16 totalLength, boolean forceRetrieve)
{
    Std_ReturnType ret = E_NOT_OK;

    TcpIp_EnterExclusiveArea();
    const TcpIp_UpCopyTxDataFuncType copyFuncPtr = socketRtPtr->OwnerCfgPtr->Up_CopyTxDataFuncPtr;

    if (
#if defined(TCPIP_INNER_CHECK)
        (NULL_PTR != socketRtPtr->PcbPtr) && (TCPIP_IPPROTO_TCP == socketRtPtr->Protocol) &&
#endif
        (NULL_PTR != copyFuncPtr)
        && ((NULL_PTR == socketRtPtr->UpCopyTxPbufPtr) && (0u == socketRtPtr->UpCopyTxRemainLen)))
    {
        err_t           err    = (err_t)ERR_ENUM_MAX;
        struct tcp_pcb* pcbPtr = (struct tcp_pcb*)socketRtPtr->PcbPtr;
        uint16 copyLen = ((tcpwnd_size_t)totalLength <= pcbPtr->snd_buf) ? totalLength : ((uint16)pcbPtr->snd_buf);
        struct pbuf* pbufPtr = pbuf_alloc(PBUF_RAW, (u16_t)copyLen, PBUF_RAM);
        TcpIp_ExitExclusiveArea();

#if defined(TCPIP_INNER_CHECK)
        if ((NULL_PTR != pbufPtr) && (copyLen == (uint16)pbufPtr->len) && (pbufPtr->len == pbufPtr->tot_len)
            && (NULL_PTR == pbufPtr->next))
#endif
        {
            BufReq_ReturnType bufRet =
                copyFuncPtr(socketRtPtr->SocketId, (uint8*)pbufPtr->payload, (uint16)pbufPtr->len);

            TcpIp_EnterExclusiveArea();
            if ((BUFREQ_OK == bufRet)
#if defined(TCPIP_INNER_CHECK)
                && (pcbPtr == socketRtPtr->PcbPtr)
#endif
            )
            {
                if ((copyLen == totalLength) || (!forceRetrieve))
                {
#if (STD_ON == TCPIP_TCP_TLS_ENABLED)
                    if (NULL_PTR != socketRtPtr->tlsPtr)
                    {
                        err = TcpIp_TlsWrite(socketRtPtr, pbufPtr->payload, pbufPtr->len);
                    }
                    else
#endif
                    {
                        err = tcp_write(pcbPtr, pbufPtr->payload, pbufPtr->len, TCPIP_TCP_WRITE_API_FLAG);
#if TCPIP_FASTTX_TCP
                        err += tcp_output(pcbPtr);
#endif
                    }

                    if ((err_t)ERR_MEM == err)
                    {
                        err = (err_t)ERR_OK;
                    }
                    else
                    {
                        (void)pbuf_free(pbufPtr);
                        pbufPtr = NULL_PTR;
                    }
                }
            }
            else
            {
                (void)pbuf_free(pbufPtr);
                pbufPtr = NULL_PTR;
            }
        }
#if defined(TCPIP_INNER_CHECK)
        else
        {
            TcpIp_EnterExclusiveArea();
            if (pcbPtr == socketRtPtr->PcbPtr)
            {
                err = (err_t)ERR_OK;
            }

            if (NULL_PTR != pbufPtr)
            {
                (void)pbuf_free(pbufPtr);
                pbufPtr = NULL_PTR;
            }
        }
#endif

        if ((err_t)ERR_OK == err)
        {
            /* pbufPtr != NULL_PTR only if tcp_write() == ERR_MEM */
            socketRtPtr->UpCopyTxPbufPtr   = pbufPtr;
            socketRtPtr->UpCopyTxRemainLen = totalLength - copyLen;
            if (forceRetrieve)
            {
                TcpIp_SocketFlagSet(socketRtPtr, TCPIP_SOCKET_FLAG_TCP_FORCERETRIEVE);
            }
            else
            {
                TcpIp_SocketFlagReset(socketRtPtr, TCPIP_SOCKET_FLAG_TCP_FORCERETRIEVE);
            }

            ret = E_OK;
        }
    }
    TcpIp_ExitExclusiveArea();

    return ret;
}
#endif

void TcpIp_InnerRxIndication(
    const TcpIp_CtrlType* ctrlCfgPtr,
    Eth_FrameType         frameType,
    uint8                 flag,
    const uint8*          dataPtr,
    uint16                lenByte)
{
#if defined(TCPIP_RXIND_PBUF_REF)
    struct pbuf* p = pbuf_alloc(PBUF_RAW, lenByte, PBUF_REF);
    p->payload     = dataPtr;
#else
    struct pbuf* p = pbuf_alloc(PBUF_RAW, lenByte, PBUF_RAM);
#endif

    TcpIp_UpTcpIpEventNotify();

#if defined(TCPIP_INNER_CHECK)
    if (NULL_PTR != p)
#endif
    {
#if defined(TCPIP_INNER_CHECK)
#if !defined(TCPIP_RXIND_PBUF_REF)
        err_t err = pbuf_take(p, dataPtr, lenByte);

        if ((err_t)ERR_OK == err)
#endif
#else
        (void)pbuf_take(p, dataPtr, lenByte);
#endif
        {
            TcpIp_CtrlRuntimeType* ctrlRtPtr = ctrlCfgPtr->RuntimePtr;
            struct netif*          netifPtr  = &(ctrlRtPtr->Netif);

            p->if_idx = netif_get_index(netifPtr); /* PRQA S 1860 */ /* VL_TcpIp_1860 */
            p->flags |= flag;

            switch (frameType)
            {
#if (STD_ON == TCPIP_IPV4_ENABLED) && (STD_ON == TCPIP_ARP_ENABLED)
            case ((uint16)ETHTYPE_IP):
                if (0u != (netifPtr->flags & (uint8)NETIF_FLAG_ETHARP))
                {
                    (void)ip4_input(p, netifPtr);
                }
                else
                {
                    (void)pbuf_free(p);
                    p = NULL_PTR;
                }
                break;
            case ((uint16)ETHTYPE_ARP):
                if (0u != (netifPtr->flags & (uint8)NETIF_FLAG_ETHARP))
                {
                    etharp_input(p, netifPtr);
                }
                else
                {
                    (void)pbuf_free(p);
                    p = NULL_PTR;
                }
                break;
#endif

#if (STD_ON == TCPIP_IPV6_ENABLED)
            case ((uint16)ETHTYPE_IPV6):
                ip6_input(p, netifPtr);
                break;
#endif

            default:
                (void)pbuf_free(p);
                p = NULL_PTR;
                break;
            }

            TcpIp_UpTcpIpEventNotify();
        }
    }
}

#if defined(TCPIP_SUUPORT_CTRL_MAINHANDLE)
void TcpIp_ControlPollMainHandle(ApplicationType partitionIndex)
{
    for (uint8 ctrlIndex = 0u; ctrlIndex < TcpIp_PbCfgPtr->CtrlNum; ++ctrlIndex)
    {
        const TcpIp_CtrlType* ctrlCfgPtr = &TCPIP_CTRL_CONFIG(ctrlIndex);

        if (TCPIP_PARTITION_INDEX(ctrlCfgPtr) == partitionIndex)
        {
            TcpIp_CtrlRuntimeType* ctrlRtPtr = ctrlCfgPtr->RuntimePtr;

#if (STD_ON == TCPIP_SUPPORT_AUTOIP_INITTIMER)
            if (TCPIP_CTRL_IS_AUTOIP_PENDING(ctrlRtPtr))
            {
                TCPIP_CTRL_CLR_AUTOIP_PENDING(ctrlRtPtr);

                if (0u < ctrlRtPtr->autoipTimer)
                {
                    --ctrlRtPtr->autoipTimer;
                }
                if (0u == ctrlRtPtr->autoipTimer)
                {
                    struct netif* netifPtr = &(ctrlRtPtr->Netif);

                    (void)autoip_start(netifPtr);
                }
            }
#endif
        }
    }
}
#endif

void TcpIp_SocketPollMainHandle(ApplicationType partitionIndex)
{
#if (STD_ON == TCPIP_TCP_ENABLED)
    for (TcpIp_SocketIdType socketIndex = 0u; TCPIP_SOCKET_NUM > socketIndex; ++socketIndex)
    {
        TcpIp_SocketRuntimeType* socketRtPtr = &TCPIP_SOCKET(partitionIndex, socketIndex);

        TcpIp_EnterExclusiveArea();
        if ((NULL_PTR != socketRtPtr->PcbPtr) && (TCPIP_IPPROTO_TCP == socketRtPtr->Protocol)
            && ((NULL_PTR != socketRtPtr->UpCopyTxPbufPtr) || (0u < socketRtPtr->UpCopyTxRemainLen))
#if defined(TCPIP_INNER_CHECK)
            && (NULL_PTR != socketRtPtr->OwnerCfgPtr->Up_CopyTxDataFuncPtr)
#endif
        )
        {
            uint16 copyLen = TcpIp_TcpTransmitCopyTxData(socketRtPtr);

            TcpIp_TcpTransmitCopyWrite(socketRtPtr, copyLen);
        }
        TcpIp_ExitExclusiveArea();
    }
#endif
}

void TcpIp_PeriodTimerMainHandle(ApplicationType partitionIndex)
{
    TcpIP_PeriodTimerType* timerRtPtr = TCPIP_PERIODTIMER(partitionIndex);

    TcpIp_EnterExclusiveArea();
#if (defined TCPIP_TCP_SYNISN_RECALC) && (STD_ON == TCPIP_TCP_SYNISN_RECALC)
#if (4u <= TCPIP_MAINFUNCTION_PERIOD)
    TcpIp_TcpIsnCount += (TCPIP_MAINFUNCTION_PERIOD / 4u);
#else
    if (0uL < timerRtPtr->tmr_4ms)
    {
        --timerRtPtr.tmr_4ms;
    }
    if (0uL == timerRtPtr->tmr_4ms)
    {
        timerRtPtr->tmr_4ms = TCPIP_TIMER_PERIOD(4u);
        TcpIp_TcpIsnCount += 1uL;
    }
#endif
#endif

#if (STD_ON == TCPIP_SUPPORT_TMR_100ms)
    if (0uL < timerRtPtr->tmr_100ms)
    {
        --timerRtPtr->tmr_100ms;
    }
    if (0uL == timerRtPtr->tmr_100ms)
    {
        timerRtPtr->tmr_100ms = TCPIP_TIMER_PERIOD(100u);
#if (STD_ON == TCPIP_AUTOIP_ENABLED)
        autoip_tmr();
#endif
#if LWIP_IPV6_MLD
        mld6_tmr();
#endif
    }
#endif

#if (STD_ON == TCPIP_SUPPORT_TMR_250ms)
    if (0uL < timerRtPtr->tmr_250ms)
    {
        --timerRtPtr->tmr_250ms;
    }
    if (0uL == timerRtPtr->tmr_250ms)
    {
        timerRtPtr->tmr_250ms = TCPIP_TIMER_PERIOD(250u);
#if (STD_ON == TCPIP_TCP_ENABLED)
        tcp_tmr();
#endif
    }
#endif

#if (STD_ON == TCPIP_SUPPORT_TMR_500ms)
    if (0uL < timerRtPtr->tmr_500ms)
    {
        --timerRtPtr->tmr_500ms;
    }
    if (0uL == timerRtPtr->tmr_500ms)
    {
        timerRtPtr->tmr_500ms = TCPIP_TIMER_PERIOD(500u);
#if (STD_ON == TCPIP_DHCP_CLIENT_ENABLED)
        dhcp_fine_tmr();
#endif
    }
#endif

#if (STD_ON == TCPIP_SUPPORT_TMR_1000ms)
    if (0uL < timerRtPtr->tmr_1000ms)
    {
        --timerRtPtr->tmr_1000ms;
    }
    if (0uL == timerRtPtr->tmr_1000ms)
    {
        timerRtPtr->tmr_1000ms = TCPIP_TIMER_PERIOD(1000u);
#if (STD_ON == TCPIP_ARP_ENABLED)
        etharp_tmr();
#endif
#if IP_REASSEMBLY
        ip_reass_tmr();
#endif
#if (STD_ON == TCPIP_IPV6_ENABLED)
        nd6_tmr();
#endif
#if LWIP_IPV6_REASS
        ip6_reass_tmr();
#endif
    }
#endif

#if (STD_ON == TCPIP_SUPPORT_TMR_60000ms)
    if (0uL < timerRtPtr->tmr_60000ms)
    {
        --timerRtPtr->tmr_60000ms;
    }
    if (0uL == timerRtPtr->tmr_60000ms)
    {
        timerRtPtr->tmr_60000ms = TCPIP_TIMER_PERIOD(60000u);
#if (STD_ON == TCPIP_DHCP_CLIENT_ENABLED)
        dhcp_coarse_tmr();
#endif
    }
#endif
    TcpIp_ExitExclusiveArea();

    TcpIp_UpTcpIpEventNotify();
}

#if (STD_ON == TCPIP_TCP_ENABLED)
Std_ReturnType TcpIp_MallocTcpSocket(
    TcpIp_DomainType             domain,
    const TcpIp_SocketOwnerType* socketOwnerCfgPtr,
    TcpIp_SocketIdType*          socketIdPtr)
{
    Std_ReturnType     ret            = E_NOT_OK;
    ApplicationType    partitionIndex = TcpIp_MulticoreIndex;
    TcpIp_RuntimeType* runtimePtr     = TCPIP_RUNTIME(partitionIndex);

    TCPIP_UNUSED_ARG(domain);

    TcpIp_EnterExclusiveArea();
    TcpIp_SocketIdType socketLastIndex = runtimePtr->SocketLastIndex;

    if ((TCPIP_TCP_SOCKET_MAX > runtimePtr->TcpSocketUsedNum) && (TCPIP_SOCKET_NUM > socketLastIndex))
    {
        struct tcp_pcb* pcbPtr = NULL_PTR;

#if ((TCPIP_SC1 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC2 == TCPIP_SCALABILITY_CLASS))
        pcbPtr = tcp_new();
#else
        if (TCPIP_AF_INET == domain)
        {
            pcbPtr = tcp_new_ip_type(IPADDR_TYPE_V4);
        }
        else
        {
            pcbPtr = tcp_new_ip_type(IPADDR_TYPE_V6);
        }
#endif

#if defined(TCPIP_INNER_CHECK)
        if (NULL_PTR != pcbPtr)
#endif
        {
            TcpIp_SocketRuntimeType* socketRtPtr = &TCPIP_SOCKET(partitionIndex, socketLastIndex);

            runtimePtr->SocketLastIndex = socketRtPtr->NextFreeIndex;
            runtimePtr->TcpSocketUsedNum += 1u;
            TcpIp_ExitExclusiveArea();

            *socketIdPtr = TCPIP_SOCKET_ID(partitionIndex, socketLastIndex);

            (void)IStdLib_MemSet(socketRtPtr, 0u, sizeof(TcpIp_SocketRuntimeType));
            socketRtPtr->SocketId      = TCPIP_SOCKET_ID(partitionIndex, socketLastIndex);
            socketRtPtr->NextFreeIndex = TCPIP_SOCKET_NUM;
            socketRtPtr->PcbPtr        = (void*)pcbPtr;
            socketRtPtr->Protocol      = TCPIP_IPPROTO_TCP;
            socketRtPtr->OwnerCfgPtr   = socketOwnerCfgPtr;
            socketRtPtr->CtrlIndex     = TCPIP_CTRL_INDEX_INVALID;
#if (STD_ON == TCPIP_TCP_TLS_ENABLED)
            socketRtPtr->listenSocketId = TCPIP_SOCKET_NUM;
#endif

            tcp_ext_arg_set(pcbPtr, 0u, socketRtPtr);
            tcp_ext_arg_set_callbacks(
                pcbPtr,
                0u,
                &TcpIp_TcpExtArgCallbackTable[TCPIP_TCPEXTARG_CALLBACK_INDEX_ENABLED]);
            tcp_arg(pcbPtr, socketRtPtr);
            tcp_recv(pcbPtr, TcpIp_TcpRecvCallback);
            tcp_sent(pcbPtr, TcpIp_TcpSentCallback);
            tcp_err(pcbPtr, TcpIp_TcpErrCallback);

            ret = E_OK;
        }
#if defined(TCPIP_INNER_CHECK)
        else
        {
            TcpIp_ExitExclusiveArea();
        }
#endif
    }
    else
    {
        TcpIp_ExitExclusiveArea();
    }

    return ret;
}
#endif

#if (STD_ON == TCPIP_UDP_ENABLED)
Std_ReturnType TcpIp_MallocUdpSocket(
    TcpIp_DomainType             domain,
    const TcpIp_SocketOwnerType* socketOwnerCfgPtr,
    TcpIp_SocketIdType*          socketIdPtr)
{
    Std_ReturnType     ret            = E_NOT_OK;
    ApplicationType    partitionIndex = TcpIp_MulticoreIndex;
    TcpIp_RuntimeType* runtimePtr     = TCPIP_RUNTIME(partitionIndex);

    TCPIP_UNUSED_ARG(domain);

    TcpIp_EnterExclusiveArea();
    TcpIp_SocketIdType socketLastIndex = runtimePtr->SocketLastIndex;

    if ((TCPIP_UDP_SOCKET_MAX > runtimePtr->UdpSocketUsedNum) && (TCPIP_SOCKET_NUM > socketLastIndex))
    {
        struct udp_pcb* pcbPtr = NULL_PTR;

#if ((TCPIP_SC1 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC2 == TCPIP_SCALABILITY_CLASS))
        pcbPtr = udp_new();
#else
        if (TCPIP_AF_INET == domain)
        {
            pcbPtr = udp_new_ip_type(IPADDR_TYPE_V4);
        }
        else
        {
            pcbPtr = udp_new_ip_type(IPADDR_TYPE_V6);
        }
#endif

#if defined(TCPIP_INNER_CHECK)
        if (NULL_PTR != pcbPtr)
#endif
        {
            TcpIp_SocketRuntimeType* socketRtPtr = &TCPIP_SOCKET(partitionIndex, socketLastIndex);

            runtimePtr->SocketLastIndex = socketRtPtr->NextFreeIndex;
            runtimePtr->UdpSocketUsedNum += 1u;
            TcpIp_ExitExclusiveArea();

            *socketIdPtr = TCPIP_SOCKET_ID(partitionIndex, socketLastIndex);

            (void)IStdLib_MemSet(socketRtPtr, 0u, sizeof(TcpIp_SocketRuntimeType));
            socketRtPtr->SocketId      = TCPIP_SOCKET_ID(partitionIndex, socketLastIndex);
            socketRtPtr->NextFreeIndex = TCPIP_SOCKET_NUM;
            socketRtPtr->PcbPtr        = (void*)pcbPtr;
            socketRtPtr->Protocol      = TCPIP_IPPROTO_UDP;
            socketRtPtr->OwnerCfgPtr   = socketOwnerCfgPtr;
            socketRtPtr->CtrlIndex     = TCPIP_CTRL_INDEX_INVALID;

            ret = E_OK;
        }
#if defined(TCPIP_INNER_CHECK)
        else
        {
            TcpIp_ExitExclusiveArea();
        }
#endif
    }
    else
    {
        TcpIp_ExitExclusiveArea();
    }

    return ret;
}
#endif

#if ((STD_ON == TCPIP_ARP_ENABLED) && (STD_ON == TCPIP_ARP_STATIC_ENABLED))
Std_ReturnType
    TcpIp_ArpAddStaticEntry(uint32 ctrlIndex, const TcpIp_SockAddrType* remoteAddrPtr, const uint8* physAddrPtr)
{
    Std_ReturnType         ret        = E_NOT_OK;
    const TcpIp_CtrlType*  ctrlCfgPtr = &TCPIP_CTRL_CONFIG(ctrlIndex);
    TcpIp_CtrlRuntimeType* ctrlRtPtr  = ctrlCfgPtr->RuntimePtr;
    ip4_addr_t             remoteIp   = {0};
    struct eth_addr        ethAddr    = {0};
    struct netif*          netifPtr   = &(ctrlRtPtr->Netif);

    TCPIP_LOCAL_TO_LWIP_IPADDR(remoteIp.addr, remoteAddrPtr->addr); /* PRQA S 3473 */ /* VL_TcpIp_3473 */
    (void)IStdLib_MemCpy(ethAddr.addr, physAddrPtr, ETH_HWADDR_LEN);

    if ((err_t)ERR_OK == etharp_add_static_entry(&remoteIp, &ethAddr, netifPtr))
    {
        ret = E_OK;
    }

    return ret;
}

Std_ReturnType TcpIp_ArpRemoveStaticEntry(uint32 ctrlIndex, const TcpIp_SockAddrType* remoteAddrPtr)
{
    Std_ReturnType         ret        = E_NOT_OK;
    const TcpIp_CtrlType*  ctrlCfgPtr = &TCPIP_CTRL_CONFIG(ctrlIndex);
    TcpIp_CtrlRuntimeType* ctrlRtPtr  = ctrlCfgPtr->RuntimePtr;
    ip4_addr_t             remoteIp   = {0};
    struct netif*          netifPtr   = &(ctrlRtPtr->Netif);

    TCPIP_LOCAL_TO_LWIP_IPADDR(remoteIp.addr, remoteAddrPtr->addr); /* PRQA S 3473 */ /* VL_TcpIp_3473 */

    if ((err_t)ERR_OK == etharp_remove_static_entry(&remoteIp, netifPtr))
    {
        ret = E_OK;
    }

    return ret;
}
#endif

#if TCPIP_LWIP_HOOKS

#if defined(LWIP_HOOK_IPERF_GET_TIME_100US)
/**
 * @brief              implementation for TcpIp_HookGetTimeNow100us()
 * @return             uint32
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
uint32 TcpIp_HookGetTimeNow100us(void)
{
    Tm_PredefTimer100us32bitType time_vlu = {0};
    (void)Tm_ResetTimer100us32bit(&time_vlu);
    return time_vlu.ui32RefTime;
}
#endif

#if (defined TCPIP_TCP_SYNISN_RECALC) && (STD_ON == TCPIP_TCP_SYNISN_RECALC)
/**
 * @brief              implementation for TcpIp_HookTcpIsn()
 * @param[in]          local_ip
 * @param[in]          local_port
 * @param[in]          remote_ip
 * @param[in]          remote_port
 * @return             uint32
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
uint32 TcpIp_HookTcpIsn(const ip_addr_t* local_ip, u16_t local_port, const ip_addr_t* remote_ip, u16_t remote_port)
{
    TCPIP_UNUSED_ARG(local_ip);
    TCPIP_UNUSED_ARG(local_port);
    TCPIP_UNUSED_ARG(remote_ip);
    TCPIP_UNUSED_ARG(remote_port);

    return TcpIp_TcpIsnCount;
}
#endif

#if (STD_ON == TCPIP_CTRL_MULTI_LOCALADDR)
/**
 * @brief              implementation for TcpIp_HookIp4InputAccept()
 * @param[in]          netifPtr
 * @param[in]          destIpPtr
 * @return             struct netif*
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
struct netif* TcpIp_HookIp4InputAccept(struct netif* netifPtr, const ip_addr_t* destIpPtr)
{
    struct netif* retNetifPtr = NULL_PTR;

#if defined(TCPIP_INNER_CHECK)
    if ((NULL_PTR != netifPtr) && TcpIp_NetifIsSetCtrlIndex(netifPtr) && (NULL_PTR != destIpPtr))
#endif
    {
        uint8                 ctrlIndex  = TcpIp_NetifGetCtrlIndex(netifPtr);
        const TcpIp_CtrlType* ctrlCfgPtr = &TCPIP_CTRL_CONFIG(ctrlIndex);

        for (TcpIp_LocalAddrIdType localAddrId = ctrlCfgPtr->LocalAddrIdMin;
             localAddrId < ctrlCfgPtr->LocalAddrIdMaxExcluded;
             ++localAddrId)
        {
            const TcpIp_LocalAddrType*        localAddrCfgPtr = &TCPIP_LOCALADDR_CONFIG(localAddrId);
            const TcpIp_LocalAddrRuntimeType* localAddrRtPtr  = localAddrCfgPtr->RuntimePtr;

#if !TCPIP_ALLOW_RX_INVALID_LOCAL_IP
            if (TcpIp_LocalAddrFlagIsSet(localAddrRtPtr, TCPIP_LOCALADDR_FLAG_IP_IS_VALID)
                && (ip_addr_eq(&(localAddrRtPtr->LocalIp), destIpPtr)))
#else
            if ((!ip_addr_isany_val(localAddrRtPtr->LocalIp)) && (ip_addr_eq(&(localAddrRtPtr->LocalIp), destIpPtr)))
#endif
            {
                retNetifPtr = netifPtr;
                break;
            }
        }
    }

    return retNetifPtr;
}

/**
 * @brief              implementation for TcpIp_HookEthArpInputForUs()
 * @param[in]          netifPtr
 * @param[in]          destIpPtr
 * @param[in]          srcIpPtr
 * @return             uint8
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
uint8 TcpIp_HookEthArpInputForUs(const struct netif* netifPtr, const ip_addr_t* destIpPtr, ip_addr_t* srcIpPtr)
{
    uint8 ret = 0u;

#if defined(TCPIP_INNER_CHECK)
    if ((NULL_PTR != netifPtr) && TcpIp_NetifIsSetCtrlIndex(netifPtr) && (NULL_PTR != destIpPtr)
        && (NULL_PTR != srcIpPtr))
#endif
    {
        uint8                 ctrlIndex  = TcpIp_NetifGetCtrlIndex(netifPtr);
        const TcpIp_CtrlType* ctrlCfgPtr = &TCPIP_CTRL_CONFIG(ctrlIndex);

        for (TcpIp_LocalAddrIdType localAddrId = ctrlCfgPtr->LocalAddrIdMin;
             localAddrId < ctrlCfgPtr->LocalAddrIdMaxExcluded;
             ++localAddrId)
        {
            const TcpIp_LocalAddrType*        localAddrCfgPtr = &TCPIP_LOCALADDR_CONFIG(localAddrId);
            const TcpIp_LocalAddrRuntimeType* localAddrRtPtr  = localAddrCfgPtr->RuntimePtr;

            if (TcpIp_LocalAddrFlagIsSet(localAddrRtPtr, TCPIP_LOCALADDR_FLAG_IP_IS_VALID)
                && (ip_addr_eq(&(localAddrRtPtr->LocalIp), destIpPtr)))
            {
                *srcIpPtr = *destIpPtr;
                ret       = 1u;
                break;
            }
        }
    }

    return ret;
}

/**
 * @brief              implementation for TcpIp_HookEthArpGetGw()
 * @param[in]          netifPtr
 * @param[in]          destIpPtr
 * @return             const ip4_addr_t*
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
const ip4_addr_t* TcpIp_HookEthArpGetGw(struct netif* netifPtr, const ip_addr_t* destIpPtr)
{
    const ip4_addr_t* retIpPtr = NULL_PTR;

#if defined(TCPIP_INNER_CHECK)
    if ((NULL_PTR != netifPtr) && TcpIp_NetifIsSetCtrlIndex(netifPtr) && (NULL_PTR != destIpPtr))
#endif
    {
        uint8                 ctrlIndex  = TcpIp_NetifGetCtrlIndex(netifPtr);
        const TcpIp_CtrlType* ctrlCfgPtr = &TCPIP_CTRL_CONFIG(ctrlIndex);

        for (TcpIp_LocalAddrIdType localAddrId = ctrlCfgPtr->LocalAddrIdMin;
             localAddrId < ctrlCfgPtr->LocalAddrIdMaxExcluded;
             ++localAddrId)
        {
            const TcpIp_LocalAddrType*        localAddrCfgPtr = &TCPIP_LOCALADDR_CONFIG(localAddrId);
            const TcpIp_LocalAddrRuntimeType* localAddrRtPtr  = localAddrCfgPtr->RuntimePtr;

            if (TcpIp_LocalAddrFlagIsSet(localAddrRtPtr, TCPIP_LOCALADDR_FLAG_IP_IS_VALID)
                && (!ip4_addr_isany_val(localAddrRtPtr->LocalIp)) && (!ip4_addr_isany_val(localAddrRtPtr->Netmask))
                && (ip4_addr_net_eq(destIpPtr, &(localAddrRtPtr->LocalIp), &(localAddrRtPtr->Netmask))))
            {
                retIpPtr = &(localAddrRtPtr->RouterIp);
                break;
            }
        }
    }

    return retIpPtr;
}

/**
 * @brief              implementation for TcpIp_HookGratuitousArp()
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
void TcpIp_HookGratuitousArp(struct netif* netifPtr)
{
#if defined(TCPIP_INNER_CHECK)
    if ((NULL_PTR != netifPtr) && TcpIp_NetifIsSetCtrlIndex(netifPtr))
#endif
    {
        uint8                 ctrlIndex  = TcpIp_NetifGetCtrlIndex(netifPtr);
        const TcpIp_CtrlType* ctrlCfgPtr = &TCPIP_CTRL_CONFIG(ctrlIndex);
        struct eth_addr       ethAddr    = {
                     .addr =
                {
                    netifPtr->hwaddr[0],
                    netifPtr->hwaddr[1],
                    netifPtr->hwaddr[2],
                    netifPtr->hwaddr[3],
                    netifPtr->hwaddr[4],
                    netifPtr->hwaddr[5],
                },
        };
        struct eth_addr local_ethbroadcast = {
            .addr = {0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu},
        };
        struct eth_addr local_ethzero = {.addr = {0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u}};

        for (TcpIp_LocalAddrIdType localAddrId = ctrlCfgPtr->LocalAddrIdMin;
             localAddrId < ctrlCfgPtr->LocalAddrIdMaxExcluded;
             ++localAddrId)
        {
            const TcpIp_LocalAddrType*        localAddrCfgPtr = &TCPIP_LOCALADDR_CONFIG(localAddrId);
            const TcpIp_LocalAddrRuntimeType* localAddrRtPtr  = localAddrCfgPtr->RuntimePtr;

            if (TcpIp_LocalAddrFlagIsSet(localAddrRtPtr, TCPIP_LOCALADDR_FLAG_IP_IS_VALID)
                && (!ip4_addr_isany_val(localAddrRtPtr->LocalIp)))
            {
                (void)etharp_raw(
                    netifPtr,
                    &ethAddr,
                    &local_ethbroadcast,
                    &ethAddr,
                    &(localAddrRtPtr->LocalIp),
                    &local_ethzero,
                    &(localAddrRtPtr->LocalIp),
                    (u16_t)ARP_REQUEST);
            }
        }
    }
}

#if (STD_ON == TCPIP_UDP_ENABLED)
/**
 * @brief              implementation for TcpIp_UdpTransmitLocalIpCompare()
 * @param[in]          localIpPtr
 * @param[in]          netifPtr
 * @return             uint8
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
uint8 TcpIp_UdpTransmitLocalIpCompare(const ip_addr_t* localIpPtr, const struct netif* netifPtr)
{
    uint8 ret = 0u;

#if defined(TCPIP_INNER_CHECK)
    if ((NULL_PTR != netifPtr) && TcpIp_NetifIsSetCtrlIndex(netifPtr) && (NULL_PTR != localIpPtr))
#endif
    {
        uint8                 ctrlIndex  = TcpIp_NetifGetCtrlIndex(netifPtr);
        const TcpIp_CtrlType* ctrlCfgPtr = &TCPIP_CTRL_CONFIG(ctrlIndex);

        for (TcpIp_LocalAddrIdType localAddrId = ctrlCfgPtr->LocalAddrIdMin;
             localAddrId < ctrlCfgPtr->LocalAddrIdMaxExcluded;
             ++localAddrId)
        {
            const TcpIp_LocalAddrType*        localAddrCfgPtr = &TCPIP_LOCALADDR_CONFIG(localAddrId);
            const TcpIp_LocalAddrRuntimeType* localAddrRtPtr  = localAddrCfgPtr->RuntimePtr;

            if (TcpIp_LocalAddrFlagIsSet(localAddrRtPtr, TCPIP_LOCALADDR_FLAG_IP_IS_VALID)
                && (ip_addr_eq(&(localAddrRtPtr->LocalIp), localIpPtr)))
            {
                ret = 1u;
                break;
            }
        }
    }

    return ret;
}
#endif
#endif

#endif

uint8 TcpIp_GetCtrlIpFramePriority(uint8 ctrlIndex)
{
    uint8 priority = 0u;

    if ((NULL_PTR != TcpIp_PbCfgPtr) && (TcpIp_PbCfgPtr->CtrlNum > ctrlIndex))
    {
        const TcpIp_CtrlType* ctrlCfgPtr = &TCPIP_CTRL_CONFIG(ctrlIndex);

        priority = ctrlCfgPtr->IpFramePrioDefault;
    }

    return priority;
}

/* ========================================== internal function definitions ========================================= */
#if (STD_ON == TCPIP_TCP_ENABLED)
/**
 * called in rx indication
 */
TCPIP_LOCAL Std_ReturnType TcpIp_NotifyUpTcpAccepted(
    const TcpIp_SocketRuntimeType* listenSocketRtPtr,
    const TcpIp_SocketRuntimeType* socketRtPtr)
{
    Std_ReturnType                    ret        = E_NOT_OK;
    const TcpIp_UpTcpAcceptedFuncType acceptFunc = socketRtPtr->OwnerCfgPtr->Up_TcpAcceptedFuncPtr;

    if (NULL_PTR != acceptFunc)
    {
        const struct tcp_pcb* pcbPtr     = (struct tcp_pcb*)socketRtPtr->PcbPtr;
        TcpIp_SockAddrType    remoteAddr = {0};

#if (TCPIP_SC1 == TCPIP_SCALABILITY_CLASS)
        remoteAddr.domain = TCPIP_AF_INET;
#elif (TCPIP_SC2 == TCPIP_SCALABILITY_CLASS)
        remoteAddr.domain = TCPIP_AF_INET6;
#else
        remoteAddr.domain = IP_IS_V4_VAL(pcbPtr->remote_ip) ? TCPIP_AF_INET : TCPIP_AF_INET6;
#endif
        remoteAddr.port = pcbPtr->remote_port;
        TCPIP_LWIP_TO_LOCAL_IPADDR(remoteAddr.addr, pcbPtr->remote_ip.addr);

        ret = acceptFunc(listenSocketRtPtr->SocketId, socketRtPtr->SocketId, &remoteAddr);
    }

    return ret;
}

/**
 * called in rx indication
 */
TCPIP_LOCAL void TcpIp_NotifyUpTcpConnected(const TcpIp_SocketRuntimeType* socketRtPtr)
{
    const TcpIp_UpTcpConnectedFuncType connectPtr = socketRtPtr->OwnerCfgPtr->Up_TcpConnectedFuncPtr;

    if (NULL_PTR != connectPtr)
    {
        connectPtr(socketRtPtr->SocketId);
    }
}

/**
 * called in rx indication
 */
TCPIP_LOCAL Std_ReturnType
    TcpIp_TcpNotifyUpRxIndication(const TcpIp_SocketRuntimeType* socketRtPtr, struct pbuf** pbufPtrPtr)
{
    Std_ReturnType                     ret          = E_NOT_OK;
    const TcpIp_UpRxIndicationFuncType rxIndFuncPtr = socketRtPtr->OwnerCfgPtr->Up_RxIndicationFuncPtr;
    struct pbuf*                       pbufPtr      = *pbufPtrPtr;

    if (NULL_PTR != rxIndFuncPtr)
    {
        err_t              err;
        struct tcp_pcb*    pcbPtr     = (struct tcp_pcb*)socketRtPtr->PcbPtr;
        TcpIp_SockAddrType remoteAddr = {0};
        ip_addr_t          remoteIp;

        err = tcp_tcp_get_tcp_addrinfo(pcbPtr, 0, &remoteIp, (u16_t*)&remoteAddr.port);
#if defined(TCPIP_INNER_CHECK)
        if ((err_t)ERR_OK == err)
#endif
        {
#if (TCPIP_SC1 == TCPIP_SCALABILITY_CLASS)
            remoteAddr.domain = TCPIP_AF_INET;
#elif (TCPIP_SC2 == TCPIP_SCALABILITY_CLASS)
            remoteAddr.domain = TCPIP_AF_INET6;
#else
            remoteAddr.domain = IP_IS_V4_VAL(remoteIp) ? TCPIP_AF_INET : TCPIP_AF_INET6;
#endif
            TCPIP_LWIP_TO_LOCAL_IPADDR(remoteAddr.addr, remoteIp.addr);

            pbufPtr = pbuf_coalesce(pbufPtr, PBUF_RAW);
            for (const struct pbuf* p = pbufPtr; NULL_PTR != p; p = p->next)
            {
                rxIndFuncPtr(socketRtPtr->SocketId, &remoteAddr, (uint8*)(p->payload), (uint16)(p->len));
            }

            ret = E_OK;
        }
    }
    else
    {
        ret = E_OK;
    }

    *pbufPtrPtr = pbufPtr;

    return ret;
}

/**
 * called in rx indication
 */
TCPIP_LOCAL void TcpIp_TcpNotifyUpTxConfirmation(const TcpIp_SocketRuntimeType* socketRtPtr, uint16 len)
{
    const TcpIp_UpTxConfirmationFuncType txConfFuncPtr = socketRtPtr->OwnerCfgPtr->Up_TxConfirmationFuncPtr;

    if (NULL_PTR != txConfFuncPtr)
    {
        txConfFuncPtr(socketRtPtr->SocketId, len);
    }
}

/**
 * called in rx indication
 */
TCPIP_LOCAL void TcpIp_NotifyUpTcpIpEvent(const TcpIp_SocketRuntimeType* socketRtPtr, TcpIp_EventType event)
{
    const TcpIp_UpTcpIpEventFuncType eventFuncPtr = socketRtPtr->OwnerCfgPtr->Up_TcpIpEventFuncPtr;

    if (NULL_PTR != eventFuncPtr)
    {
        eventFuncPtr(socketRtPtr->SocketId, event);
    }
}
#endif

#if (STD_ON == TCPIP_UDP_ENABLED)
/**
 * called in rx indication
 */
TCPIP_LOCAL Std_ReturnType TcpIp_UdpNotifyUpRxIndication(
    const TcpIp_SocketRuntimeType* socketRtPtr,
    struct pbuf**                  pbufPtrPtr,
    const ip_addr_t*               remoteIpPtr,
    uint16                         port)
{
    Std_ReturnType                     ret          = E_OK;
    const TcpIp_UpRxIndicationFuncType rxIndFuncPtr = socketRtPtr->OwnerCfgPtr->Up_RxIndicationFuncPtr;
    struct pbuf*                       pbufPtr      = *pbufPtrPtr;

    if (NULL_PTR != rxIndFuncPtr)
    {
        TcpIp_SockAddrType remoteAddr = {0};

#if (TCPIP_SC1 == TCPIP_SCALABILITY_CLASS)
        remoteAddr.domain = TCPIP_AF_INET;
#elif (TCPIP_SC2 == TCPIP_SCALABILITY_CLASS)
        remoteAddr.domain = TCPIP_AF_INET6;
#else
        remoteAddr.domain = IP_IS_V4_VAL(*remoteIpPtr) ? TCPIP_AF_INET : TCPIP_AF_INET6;
#endif
        remoteAddr.port = port;
        TCPIP_LWIP_TO_LOCAL_IPADDR(remoteAddr.addr, remoteIpPtr->addr);

        pbufPtr = pbuf_coalesce(pbufPtr, PBUF_RAW);
        for (const struct pbuf* p = pbufPtr; NULL_PTR != p; p = p->next)
        {
            rxIndFuncPtr(socketRtPtr->SocketId, &remoteAddr, (uint8*)(p->payload), (uint16)(p->len));
        }
    }

    *pbufPtrPtr = pbufPtr;

    return ret;
}
#endif

/**
 * called in non-exclusive area
 */
TCPIP_LOCAL void TcpIp_NotfiyUpLocalIpAddrAssignmentChg(
    const TcpIp_LocalAddrRuntimeType* localAddrRtPtr,
    TcpIp_IpAddrStateType             ipAddrState)
{
    for (uint8 socketOwnerIndex = 0u; socketOwnerIndex < TcpIp_LConfig.SocketOwnerNum; ++socketOwnerIndex)
    {
        const TcpIp_SocketOwnerType*                   ownerCfgPtr = &TcpIp_LConfig.SocketOwnerPtr[socketOwnerIndex];
        const TcpIp_UpLocalIpAddrAssignmentChgFuncType localAssignFuncPtr =
            ownerCfgPtr->Up_LocalIpAddrAssignmentChgFuncPtr;

        if (NULL_PTR != localAssignFuncPtr)
        {
            localAssignFuncPtr(localAddrRtPtr->LocalAddrId, ipAddrState);
        }
    }
}

#if (STD_ON == TCPIP_TCP_ENABLED)
/**
 * called in rx indication
 */
TCPIP_LOCAL err_t TcpIp_TcpAcceptCallback(void* arg, struct tcp_pcb* newpcb, err_t err)
{
    err_t                          retErr            = (err_t)ERR_ABRT;
    const TcpIp_SocketRuntimeType* listenSocketRtPtr = (TcpIp_SocketRuntimeType*)arg;

    if (((err_t)ERR_OK == err)
#if defined(TCPIP_INNER_CHECK)
        && (NULL_PTR != listenSocketRtPtr) && (NULL_PTR != listenSocketRtPtr->PcbPtr)
        && (TCPIP_CTRL_INDEX_INVALID > listenSocketRtPtr->CtrlIndex) && (NULL_PTR != newpcb)
#endif
    )
    {
        const TcpIp_SocketRuntimeType* socketRtPtr = TcpIp_MallocTcpSocketByPcb(listenSocketRtPtr, newpcb);

        if ((NULL_PTR != socketRtPtr)
#if defined(TCPIP_INNER_CHECK)
            && (TCPIP_CTRL_INDEX_INVALID > socketRtPtr->CtrlIndex)
#endif
        )
        {
#if (STD_ON == TCPIP_TCP_TLS_ENABLED)
            if (NULL_PTR == socketRtPtr->tlsPtr)
            {
                TcpIp_SockAddrType remoteAddr;
                ip_addr_t          remoteIp;

                retErr = tcp_tcp_get_tcp_addrinfo(newpcb, 1, &remoteIp, &remoteAddr.port);
                if ((err_t)ERR_OK == retErr)
                {
                    TCPIP_LWIP_TO_LOCAL_IPADDR(remoteAddr.addr, remoteIp.addr);
#if (TCPIP_SC1 == TCPIP_SCALABILITY_CLASS)
                    remoteAddr.domain = TCPIP_AF_INET;
#elif (TCPIP_SC2 == TCPIP_SCALABILITY_CLASS)
                    remoteAddr.domain = TCPIP_AF_INET6;
#else
                    remoteAddr.domain = IP_IS_V4_VAL(remoteIp) ? TCPIP_AF_INET : TCPIP_AF_INET6;
#endif
                    /* static tls connection assignment */
                    TcpIp_TlsBaseStaticConnect(socketRtPtr, &remoteAddr, TLS_SERVER);
                }
            }

            /* if tls connect not assignment, this socket is normal */
            if (NULL_PTR == socketRtPtr->tlsPtr)
#endif
            {
                Std_ReturnType               ret        = E_NOT_OK;
                uint8                        ctrlIndex  = socketRtPtr->CtrlIndex;
                const TcpIp_CtrlType*        ctrlCfgPtr = &TCPIP_CTRL_CONFIG(ctrlIndex);
                const TcpIp_CtrlRuntimeType* ctrlRtPtr  = ctrlCfgPtr->RuntimePtr;

                if (TCPIP_STATE_ONLINE == ctrlRtPtr->CtrlStatus)
                {
                    ret = TcpIp_NotifyUpTcpAccepted(listenSocketRtPtr, socketRtPtr);
                }
                if (E_OK == ret)
                {
                    retErr = (err_t)ERR_OK;
                }
                else
                {
                    /* if accept is not ok, do not continue to notify upper layer event */
                    tcp_err(newpcb, NULL_PTR);
                }
            }
        }
    }
    if ((err_t)ERR_ABRT == retErr)
    {
        tcp_abort(newpcb);
    }

    return retErr;
}

/**
 * called in rx indication
 */
TCPIP_LOCAL err_t TcpIp_TcpConnnectCallback(void* arg, struct tcp_pcb* tpcb, err_t err)
{
    err_t                          retErr      = (err_t)ERR_ABRT;
    const TcpIp_SocketRuntimeType* socketRtPtr = (TcpIp_SocketRuntimeType*)arg;

#if defined(TCPIP_INNER_CHECK)
    if (((err_t)ERR_OK == err)
        && ((NULL_PTR != socketRtPtr) && (NULL_PTR != socketRtPtr->PcbPtr) && (tpcb == socketRtPtr->PcbPtr)))
#endif
    {
        TcpIp_NotifyUpTcpConnected(socketRtPtr);

        retErr = (err_t)ERR_OK;
    }
#if defined(TCPIP_INNER_CHECK)
    if ((err_t)ERR_ABRT == retErr)
    {
        tcp_abort(tpcb);
    }
#endif

    return retErr;
}

/**
 * called in rx indication
 */
TCPIP_LOCAL err_t TcpIp_TcpRecvCallback(void* arg, struct tcp_pcb* tpcb, struct pbuf* p, err_t err)
{
    err_t                          retErr      = (err_t)ERR_ABRT;
    const TcpIp_SocketRuntimeType* socketRtPtr = (TcpIp_SocketRuntimeType*)arg;

#if defined(TCPIP_INNER_CHECK)
    if (((err_t)ERR_OK == err) && (NULL_PTR != socketRtPtr) && (NULL_PTR != socketRtPtr->PcbPtr)
        && (tpcb == socketRtPtr->PcbPtr))
#endif
    {
        if (NULL_PTR != p)
        {
            if (E_OK == TcpIp_TcpNotifyUpRxIndication(socketRtPtr, &p)) /* PRQA S 1339 */ /* VL_TcpIp_1339 */
            {
                retErr = (err_t)ERR_OK;
            }
        }
        else
        {
            if (!TcpIp_SocketFlagIsSet(socketRtPtr, TCPIP_SOCKET_FLAG_TCP_PENDING_EVENT))
            {
                TcpIp_NotifyUpTcpIpEvent(socketRtPtr, TCPIP_TCP_FIN_RECEIVED);
            }
            retErr = (err_t)ERR_OK;
        }
    }
    if (NULL_PTR != p)
    {
        (void)pbuf_free(p);
        p = NULL_PTR;
    }

#if defined(TCPIP_INNER_CHECK)
    /* only if TcpIp_TcpNotifyUpRxIndication() fails */
    if ((err_t)ERR_ABRT == retErr)
    {
        tcp_abort(tpcb);
    }
#endif

    return retErr;
}

/**
 * called in rx indication
 */
TCPIP_LOCAL err_t TcpIp_TcpSentCallback(void* arg, struct tcp_pcb* tpcb, u16_t len)
{
    err_t                          retErr      = (err_t)ERR_ABRT;
    const TcpIp_SocketRuntimeType* socketRtPtr = (TcpIp_SocketRuntimeType*)arg;

#if defined(TCPIP_INNER_CHECK)
    if ((NULL_PTR != socketRtPtr) && (NULL_PTR != socketRtPtr->PcbPtr) && (tpcb == socketRtPtr->PcbPtr) && (0u < len))
#endif
    {
        TcpIp_TcpNotifyUpTxConfirmation(socketRtPtr, len);

        retErr = (err_t)ERR_OK;
    }
#if defined(TCPIP_INNER_CHECK)
    if ((err_t)ERR_ABRT == retErr)
    {
        tcp_abort(tpcb);
    }
#endif

    return retErr;
}

/**
 * called in rx indication or in exclusive area
 */
TCPIP_LOCAL void TcpIp_TcpErrCallback(void* arg, err_t err)
{
    TcpIp_SocketRuntimeType* socketRtPtr = (TcpIp_SocketRuntimeType*)arg;

    if ((NULL_PTR != socketRtPtr) && (NULL_PTR != socketRtPtr->PcbPtr))
    {
        /* ERR_RST, ERR_CLSD, ERR_ABRT */
        TcpIp_EventType event = ((err_t)ERR_CLSD != err) ? TCPIP_TCP_RESET : TCPIP_TCP_CLOSED;

        TcpIp_SocketFlagReset(socketRtPtr, TCPIP_SOCKET_FLAG_TCP_PENDING_EVENT);

        TcpIp_UpTcpIpEventHandle(socketRtPtr, event);
    }
}

/**
 * called in rx indication or in exclusive area
 */
TCPIP_LOCAL void TcpIp_TcpPcbDestroyedCallback(u8_t id, void* data)
{
    TcpIp_SocketRuntimeType* socketRtPtr = (TcpIp_SocketRuntimeType*)data;

    TCPIP_UNUSED_ARG(id);

    if ((NULL_PTR != socketRtPtr)
#if defined(TCPIP_INNER_CHECK)
        && (NULL_PTR != socketRtPtr->PcbPtr)
#endif
    )
    {
        struct tcp_pcb* pcbPtr = (struct tcp_pcb*)socketRtPtr->PcbPtr;

        tcp_ext_arg_set(pcbPtr, 0u, NULL_PTR);
        tcp_ext_arg_set_callbacks(pcbPtr, 0u, &TcpIp_TcpExtArgCallbackTable[TCPIP_TCPEXTARG_CALLBACK_INDEX_DISABLED]);
        tcp_arg(pcbPtr, NULL_PTR);
        if (TcpIp_SocketFlagIsSet(socketRtPtr, TCPIP_SOCKET_FLAG_TCP_SERVERSOCKET))
        {
            tcp_accept(pcbPtr, NULL_PTR);

            TcpIp_SocketFlagReset(socketRtPtr, TCPIP_SOCKET_FLAG_TCP_SERVERSOCKET);
        }
        else
        {
            tcp_recv(pcbPtr, NULL_PTR);
            tcp_sent(pcbPtr, NULL_PTR);
            tcp_err(pcbPtr, NULL_PTR);
        }

#if (STD_ON == TCPIP_TCP_TLS_ENABLED)
        if (NULL_PTR != socketRtPtr->tlsPtr)
        {
            TcpIp_TlsSocketDeInit(socketRtPtr);
        }
#endif

        if (TcpIp_SocketFlagIsSet(socketRtPtr, TCPIP_SOCKET_FLAG_TCP_PENDING_EVENT))
        {
            TcpIp_UpTcpIpEventHandle(socketRtPtr, TCPIP_TCP_CLOSED);
        }
        TcpIp_FreeTcpSocket(socketRtPtr);
    }
}
#endif

#if (STD_ON == TCPIP_UDP_ENABLED)
/**
 * called in rx indication
 */
TCPIP_LOCAL void
    TcpIp_UdpRecvCallback(void* arg, struct udp_pcb* pcb, struct pbuf* p, const ip_addr_t* addr, u16_t port)
{
    const TcpIp_SocketRuntimeType* socketRtPtr = (TcpIp_SocketRuntimeType*)arg;

#if defined(TCPIP_INNER_CHECK)
    if ((NULL_PTR != socketRtPtr) && (NULL_PTR != socketRtPtr->PcbPtr) && (pcb == socketRtPtr->PcbPtr)
        && (NULL_PTR != p) && (NULL_PTR != addr))
#endif
    {
        (void)TcpIp_UdpNotifyUpRxIndication(socketRtPtr, &p, addr, (uint16)port); /* PRQA S 1339 */ /* VL_TcpIp_1339 */
    }
    if (NULL_PTR != p)
    {
        (void)pbuf_free(p);
        p = NULL_PTR;
    }
}
#endif

/**
 * called in rx indication or in exclusive area
 */
TCPIP_LOCAL void
    TcpIp_NetifIpChangedCallback(struct netif* netif, netif_nsc_reason_t reason, const netif_ext_callback_args_t* args)
{
    TCPIP_UNUSED_ARG(args);

    if (
#if defined(TCPIP_INNER_CHECK)
        TcpIp_NetifIsSetCtrlIndex(netif) &&
#endif
        (0u != (reason & TCPIP_NETIF_NSC_REASON_IPCHANGED)))
    {
        uint8                  ctrlIndex   = TcpIp_NetifGetCtrlIndex(netif);
        const TcpIp_CtrlType*  ctrlCfgPtr  = &TCPIP_CTRL_CONFIG(ctrlIndex);
        TcpIp_CtrlRuntimeType* ctrlRtPtr   = ctrlCfgPtr->RuntimePtr;
        TcpIp_IpAddrStateType  ipAddrState = TCPIP_IPADDR_STATE_ASSIGNED;

#if (TCPIP_SC1 == TCPIP_SCALABILITY_CLASS)
        if (!ip4_addr_isany_val(*netif_ip4_addr(netif)))
        {
            if (TCPIP_STATE_STARTUP == ctrlRtPtr->CtrlStatus)
            {
                ctrlRtPtr->CtrlStatus = TCPIP_STATE_ONLINE;
                TcpIp_EthSmModeIndicationNotifyForCtrl(ctrlCfgPtr, TCPIP_STATE_ONLINE);
            }
        }
        else
        {
            ipAddrState = TCPIP_IPADDR_STATE_UNASSIGNED;
        }
#endif

        TcpIp_LocalIpAddrAssignChgHandleForCtrl(ctrlCfgPtr, ctrlRtPtr, ipAddrState, FALSE);
    }
}

/**
 * called in rx indication or in exclusive area
 */
TCPIP_LOCAL void TcpIp_LocalIpAddrAssignChgHandleForCtrl(
    const TcpIp_CtrlType*  ctrlCfgPtr,
    TcpIp_CtrlRuntimeType* ctrlRtPtr,
    TcpIp_IpAddrStateType  ipAddrState,
    boolean                forceNotify)
{
    for (TcpIp_LocalAddrIdType localAddrId = ctrlCfgPtr->LocalAddrIdMin;
         localAddrId < ctrlCfgPtr->LocalAddrIdMaxExcluded;
         ++localAddrId)
    {
        const TcpIp_LocalAddrType*  localAddrCfgPtr = &TCPIP_LOCALADDR_CONFIG(localAddrId);
        TcpIp_LocalAddrRuntimeType* localAddrRtPtr  = localAddrCfgPtr->RuntimePtr;
        boolean                     needNotify      = forceNotify;

        if (forceNotify && (TCPIP_IPADDR_STATE_ASSIGNED == ipAddrState)
            && (!TcpIp_LocalAddrFlagIsSet(localAddrRtPtr, TCPIP_LOCALADDR_FLAG_IP_IS_VALID)))
        {
            needNotify = FALSE;
        }

        if (TcpIp_LocalAddrFlagIsSet(localAddrRtPtr, TCPIP_LOCALADDR_FLAG_REQUEST_IP))
        {
            TcpIp_LocalAddrFlagReset(localAddrRtPtr, TCPIP_LOCALADDR_FLAG_REQUEST_IP);

            if (TCPIP_IPADDR_STATE_ASSIGNED == ipAddrState)
            {
                if (TCPIP_ADDRESS_UNICAST == localAddrCfgPtr->AddrType)
                {
                    const struct netif* netifPtr = &(ctrlRtPtr->Netif);

                    localAddrRtPtr->RouterIp = netifPtr->gw;
                    localAddrRtPtr->Netmask  = netifPtr->netmask;
                    localAddrRtPtr->LocalIp  = netifPtr->ip_addr;
                }
                TcpIp_LocalAddrFlagSet(localAddrRtPtr, TCPIP_LOCALADDR_FLAG_IP_IS_VALID);
            }
            else if (TCPIP_IPADDR_STATE_UNASSIGNED == ipAddrState)
            {
                TcpIp_LocalAddrFlagReset(localAddrRtPtr, TCPIP_LOCALADDR_FLAG_IP_IS_VALID);
            }
            else
            {
                /* do not switch ip valid flag in TCPIP_IPADDR_STATE_ONHOLD state */
            }

            needNotify = TRUE;
        }

        if (needNotify)
        {
            TcpIp_NotfiyUpLocalIpAddrAssignmentChg(localAddrRtPtr, ipAddrState);
        }
    }
}

/**
 * called in rx indication or in exclusive area
 */
TCPIP_LOCAL void TcpIp_UpTcpIpEventHandle(const TcpIp_SocketRuntimeType* socketRtPtr, TcpIp_EventType event)
{
    const TcpIp_UpTcpIpEventFuncType eventFuncPtr = socketRtPtr->OwnerCfgPtr->Up_TcpIpEventFuncPtr;

    if (NULL_PTR != eventFuncPtr)
    {
        ApplicationType          partitionIndex = TcpIp_MulticoreIndex;
        TcpIp_RuntimeType*       runtimePtr     = TCPIP_RUNTIME(partitionIndex);
        TcpIp_SocketIdType       socketIndex    = TCPIP_SOCKET_INDEX_BY_ADDR(partitionIndex, socketRtPtr);
        TcpIp_UpTcpIpNotifyType* notifyPtr      = &(runtimePtr->UpNotify[socketIndex]);

#if defined(TCPIP_INNER_CHECK)
        if ((NULL_PTR == notifyPtr->EventFuncPtr) && (TCPIP_SOCKET_NUM > runtimePtr->UpNotifyNum))
#endif
        {
            runtimePtr->UpNotifyNum += 1u;
        }

        notifyPtr->EventFuncPtr = eventFuncPtr;
        notifyPtr->PendingEvent = event;
    }
}

/**
 * called in non-exclusive area
 */
TCPIP_LOCAL void TcpIp_UpTcpIpEventNotify(void)
{
    ApplicationType    partitionIndex = TcpIp_MulticoreIndex;
    TcpIp_RuntimeType* runtimePtr     = TCPIP_RUNTIME(partitionIndex);

    for (TcpIp_SocketIdType socketIndex = 0u;
         (0u < runtimePtr->UpNotifyNum /* unreliable check */) && (TCPIP_SOCKET_NUM > socketIndex);
         ++socketIndex)
    {
        TcpIp_UpTcpIpNotifyType* notifyPtr = &(runtimePtr->UpNotify[socketIndex]);

        TcpIp_EnterExclusiveArea();
        const TcpIp_UpTcpIpEventFuncType eventFuncPtr = notifyPtr->EventFuncPtr;

        if (NULL_PTR != eventFuncPtr)
        {
            const TcpIp_SocketRuntimeType* socketRtPtr = &TCPIP_SOCKET(partitionIndex, socketIndex);
            TcpIp_EventType                event       = notifyPtr->PendingEvent;

            notifyPtr->EventFuncPtr = NULL_PTR;

#if defined(TCPIP_INNER_CHECK)
            if (0u < runtimePtr->UpNotifyNum)
#endif
            {
                runtimePtr->UpNotifyNum -= 1u;
            }
            TcpIp_ExitExclusiveArea();

            eventFuncPtr(socketRtPtr->SocketId, event);
        }
        else
        {
            TcpIp_ExitExclusiveArea();
        }
    }
}

TCPIP_LOCAL void TcpIp_NetIfGetPhyAddrFromEthIf(struct netif* netifPtr)
{
    uint8                 ctrlIndex  = TcpIp_NetifGetCtrlIndex(netifPtr);
    const TcpIp_CtrlType* ctrlCfgPtr = &TCPIP_CTRL_CONFIG(ctrlIndex);
    uint8                 physAddr[NETIF_MAX_HWADDR_LEN];

    EthIf_GetPhysAddr(ctrlCfgPtr->EthIfCtrlId, physAddr);

    netifPtr->hwaddr_len = NETIF_MAX_HWADDR_LEN;
    (void)IStdLib_MemCpy(netifPtr->hwaddr, physAddr, NETIF_MAX_HWADDR_LEN);
}

TCPIP_LOCAL err_t TcpIp_InitNetif(struct netif* netif)
{
    err_t err = (err_t)ERR_OK;

#if defined(TCPIP_INNER_CHECK)
    if (TcpIp_NetifIsSetCtrlIndex(netif))
#endif
    {
#if LWIP_IPV4
        netif->output = etharp_output; /* PRQA S 1334 */ /* VL_TcpIp_1334 */
#endif
#if LWIP_IPV6
        netif->output_ip6 = ethip6_output;
#endif
        netif->linkoutput = NULL_PTR;

        TcpIp_NetIfGetPhyAddrFromEthIf(netif);
        netif->mtu   = 1500u;
        netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;
#if LWIP_NETIF_HOSTNAME
        netif->hostname = "DoIP-ISoft";
#endif

#if (LWIP_IPV6 && LWIP_IPV6_MLD)
        /*
         * For hardware/netifs that implement MAC filtering.
         * All-nodes link-local is handled by default, so we must let the hardware know
         * to allow multicast packets in.
         * Should set mld_mac_filter previously. */
        if (netif->mld_mac_filter != NULL_PTR)
        {
            ip6_addr_t ip6_allnodes_ll;
            ip6_addr_set_allnodes_linklocal(&ip6_allnodes_ll);
            netif->mld_mac_filter(netif, &ip6_allnodes_ll, NETIF_ADD_MAC_FILTER);
        }
#endif
    }
#if defined(TCPIP_INNER_CHECK)
    else
    {
        err = (err_t)ERR_IF;
    }
#endif

    return err;
}

TCPIP_LOCAL void TcpIp_InitLocalAddrRuntime(const TcpIp_CtrlType* ctrlCfgPtr)
{
    for (TcpIp_LocalAddrIdType localAddrId = ctrlCfgPtr->LocalAddrIdMin;
         localAddrId < ctrlCfgPtr->LocalAddrIdMaxExcluded;
         ++localAddrId)
    {
        const TcpIp_LocalAddrType*  localAddrCfgPtr = &TCPIP_LOCALADDR_CONFIG(localAddrId);
        TcpIp_LocalAddrRuntimeType* localAddrRtPtr  = localAddrCfgPtr->RuntimePtr;

        (void)IStdLib_MemSet(localAddrRtPtr, 0u, sizeof(TcpIp_LocalAddrRuntimeType));
        localAddrRtPtr->LocalAddrId   = localAddrCfgPtr->LocalAddrId;
        localAddrRtPtr->RouterIp.addr = TCPIP_IPADDR_ANY;
        localAddrRtPtr->Netmask.addr  = TCPIP_IPADDR_ANY;
        localAddrRtPtr->LocalIp.addr  = TCPIP_IPADDR_ANY;
#if LWIP_NETIF_HOSTNAME
        localAddrRtPtr->HostNameLen = 10u;
#endif
    }
}

/**
 * called in exclusive area
 */
TCPIP_LOCAL err_t TcpIp_DoBind(
    TcpIp_SocketRuntimeType*     socketRtPtr,
    const TcpIp_CtrlRuntimeType* ctrlRtPtr,
    const ip_addr_t*             localIpPtr,
    uint16*                      portPtr)
{
    err_t err = (err_t)ERR_ENUM_MAX;

#if (STD_ON == TCPIP_TCP_ENABLED)
    if (TCPIP_IPPROTO_TCP == socketRtPtr->Protocol)
    {
        struct tcp_pcb* pcbPtr = (struct tcp_pcb*)socketRtPtr->PcbPtr;

        err = tcp_bind(pcbPtr, localIpPtr, (u16_t)*portPtr);
        if (((err_t)ERR_OK == err) && (TCPIP_PORT_ANY == *portPtr))
        {
            *portPtr = (uint16)pcbPtr->local_port;
        }
        if (NULL_PTR != ctrlRtPtr)
        {
            const struct netif* netifPtr = &(ctrlRtPtr->Netif);

            /* preprocess to avoid calling ip_route */
            tcp_bind_netif(pcbPtr, netifPtr);
        }
    }
#endif

#if (STD_ON == TCPIP_UDP_ENABLED)
    if (TCPIP_IPPROTO_UDP == socketRtPtr->Protocol)
    {
        struct udp_pcb* pcbPtr = (struct udp_pcb*)socketRtPtr->PcbPtr;

        err = udp_bind(pcbPtr, localIpPtr, (u16_t)*portPtr);
        if ((err_t)ERR_OK == err)
        {
            if (TCPIP_PORT_ANY == *portPtr)
            {
                *portPtr = (uint16)pcbPtr->local_port;
            }
            udp_recv(pcbPtr, TcpIp_UdpRecvCallback, socketRtPtr);
        }
        if (NULL_PTR != ctrlRtPtr)
        {
            const struct netif* netifPtr = &(ctrlRtPtr->Netif);

            /* preprocess to avoid calling ip_route */
            udp_bind_netif(pcbPtr, netifPtr);
        }
    }
#endif

    return err;
}

/**
 * called in exclusive area
 */
TCPIP_LOCAL TcpIp_SocketIdType TcpIp_CloseAllSocket(const TcpIp_CtrlRuntimeType* ctrlRtPtr)
{
#if TCPIP_SUPPORT_AUTO_CLOSE_ALL_SOCKET
    ApplicationType partitionIndex = TcpIp_MulticoreIndex;

    for (TcpIp_SocketIdType socketIndex = 0u; TCPIP_SOCKET_NUM > socketIndex; ++socketIndex)
    {
        TcpIp_SocketRuntimeType* socketRtPtr = &TCPIP_SOCKET(partitionIndex, socketIndex);

        if ((NULL_PTR != socketRtPtr->PcbPtr) && (ctrlRtPtr->CtrlIndex == socketRtPtr->CtrlIndex))
        {
#if (STD_ON == TCPIP_TCP_ENABLED)
            if (TCPIP_IPPROTO_TCP == socketRtPtr->Protocol)
            {
                err_t           err;
                struct tcp_pcb* pcbPtr = (struct tcp_pcb*)socketRtPtr->PcbPtr;

                TcpIp_SocketFlagSet(socketRtPtr, TCPIP_SOCKET_FLAG_TCP_PENDING_EVENT);

                err = tcp_close(pcbPtr);
                if ((err_t)ERR_OK != err)
                {
                    tcp_abort(pcbPtr);
                }
                pcbPtr = NULL_PTR;
            }
#endif

#if (STD_ON == TCPIP_UDP_ENABLED)
            if (TCPIP_IPPROTO_UDP == socketRtPtr->Protocol)
            {
                struct udp_pcb* pcbPtr = (struct udp_pcb*)socketRtPtr->PcbPtr;

                udp_disconnect(pcbPtr);
                udp_remove(pcbPtr);
                pcbPtr = NULL_PTR;

                TcpIp_UpTcpIpEventHandle(socketRtPtr, TCPIP_UDP_CLOSED);
                TcpIp_FreeUdpSocket(socketRtPtr);
            }
#endif
        }
    }
#endif

    return ctrlRtPtr->SocketUsedNum;
}

/**
 * called in exclusive area
 */
TCPIP_LOCAL void TcpIp_MultCastIpAssigmentHandle(
    TcpIp_CtrlRuntimeType*     ctrlRtPtr,
    const TcpIp_LocalAddrType* localAddrCfgPtr,
    boolean                    isAssign)
{
    TcpIp_LocalAddrRuntimeType* localAddrRtPtr = localAddrCfgPtr->RuntimePtr;

#if (TCPIP_SC1 == TCPIP_SCALABILITY_CLASS)
    uint8      ethIfCtrlId = localAddrCfgPtr->EthIfCtrlId;
    ip4_addr_t localIp     = {
            .addr = localAddrRtPtr->LocalIp.addr,
    };
    const uint8 macAddr[] = {
        LL_IP4_MULTICAST_ADDR_0,
        LL_IP4_MULTICAST_ADDR_1,
        LL_IP4_MULTICAST_ADDR_2,
        ip4_addr2(&localIp) & (uint8)0x7Fu,
        ip4_addr3(&localIp),
        ip4_addr4(&localIp),
    };

    (void)EthIf_UpdatePhysAddrFilter(ethIfCtrlId, macAddr, (isAssign ? ETH_ADD_TO_FILTER : ETH_REMOVE_FROM_FILTER));
#endif

    if (isAssign)
    {
        const struct netif* netifPtr = &(ctrlRtPtr->Netif);

        /* multcast need to make sure that the netif has a valid ip */
        if (!ip4_addr_isany_val(*netif_ip4_addr(netifPtr)))
        {
            TcpIp_LocalAddrFlagReset(localAddrRtPtr, TCPIP_LOCALADDR_FLAG_REQUEST_IP);
            TcpIp_LocalAddrFlagSet(localAddrRtPtr, TCPIP_LOCALADDR_FLAG_IP_IS_VALID);

            /* The current design of multicast IP allows sockets to be associated, with the upper module SOAD */
            TcpIp_NotfiyUpLocalIpAddrAssignmentChg(localAddrRtPtr, TCPIP_IPADDR_STATE_ASSIGNED);
        }
    }
    else
    {
        TcpIp_LocalAddrFlagReset(localAddrRtPtr, TCPIP_LOCALADDR_FLAG_REQUEST_IP);
        TcpIp_LocalAddrFlagReset(localAddrRtPtr, TCPIP_LOCALADDR_FLAG_IP_IS_VALID);

        /* The current design of multicast IP allows sockets to be unassociated, with the upper module SOAD */
        TcpIp_NotfiyUpLocalIpAddrAssignmentChg(localAddrRtPtr, TCPIP_IPADDR_STATE_UNASSIGNED);
    }
}

/**
 * called in exclusive area
 */
TCPIP_LOCAL Std_ReturnType TcpIp_SetLocalAddrStatic(
    TcpIp_CtrlRuntimeType*                 ctrlRtPtr,
    const TcpIp_LocalAddrType*             localAddrCfgPtr,
    const TcpIp_StaticIpAddressConfigType* ipAddrCfgPtr)
{
    Std_ReturnType              ret            = E_NOT_OK;
    TcpIp_LocalAddrRuntimeType* localAddrRtPtr = localAddrCfgPtr->RuntimePtr;
#if (STD_ON == TCPIP_CTRL_MULTI_LOCALADDR)
    TcpIp_LocalAddrIdType localAddrIndex = localAddrCfgPtr->LocalAddrIndex;
#endif

    /* PRQA S 3473 ++ */ /* VL_TcpIp_3473 */
    if (NULL_PTR != ipAddrCfgPtr->DefaultRouterPtr)
    {
        TCPIP_LOCAL_TO_LWIP_IPADDR(localAddrRtPtr->RouterIp.addr, ipAddrCfgPtr->DefaultRouterPtr);
    }
    if (NULL_PTR != ipAddrCfgPtr->NetmaskPtr)
    {
        TCPIP_LOCAL_TO_LWIP_IPADDR(localAddrRtPtr->Netmask.addr, ipAddrCfgPtr->NetmaskPtr);
    }
    TCPIP_LOCAL_TO_LWIP_IPADDR(localAddrRtPtr->LocalIp.addr, ipAddrCfgPtr->StaticIpAddrPtr);
    /* PRQA S 3473 -- */

    switch (localAddrCfgPtr->AddrType)
    {
    case TCPIP_ADDRESS_UNICAST:
        /* multiple ip only first local addr write to netif, other local addr direct notify upper layer */
#if (STD_ON == TCPIP_CTRL_MULTI_LOCALADDR)
        if (!TcpIp_CtrlLocalAddrIsAnyUsed(ctrlRtPtr))
        {
            TcpIp_CtrlLocalAddrSetUsed(ctrlRtPtr, localAddrIndex);
#endif
            TcpIp_LocalAddrFlagSet(localAddrRtPtr, TCPIP_LOCALADDR_FLAG_REQUEST_IP);

            struct netif* netifPtr = &(ctrlRtPtr->Netif);

            netif_set_addr(
                netifPtr,
                (ip4_addr_t*)&(localAddrRtPtr->LocalIp),
                (ip4_addr_t*)&(localAddrRtPtr->Netmask),
                (ip4_addr_t*)&(localAddrRtPtr->RouterIp));
#if (STD_ON == TCPIP_CTRL_MULTI_LOCALADDR)
        }
        else
        {
            TcpIp_CtrlLocalAddrSetUsed(ctrlRtPtr, localAddrIndex);
            TcpIp_LocalAddrFlagSet(localAddrRtPtr, TCPIP_LOCALADDR_FLAG_IP_IS_VALID);

            TcpIp_NotfiyUpLocalIpAddrAssignmentChg(localAddrRtPtr, TCPIP_IPADDR_STATE_ASSIGNED);
        }
#endif

        ret = E_OK;
        break;

    case TCPIP_ADDRESS_MULTICAST:
        TcpIp_LocalAddrFlagSet(localAddrRtPtr, TCPIP_LOCALADDR_FLAG_REQUEST_IP);

        TcpIp_MultCastIpAssigmentHandle(ctrlRtPtr, localAddrCfgPtr, TRUE);

        ret = E_OK;
        break;

    default:
        /* do nothing */
        break;
    }

    return ret;
}

/**
 * called in exclusive area
 */
TCPIP_LOCAL Std_ReturnType
    TcpIp_ClearLocalAddrStatic(TcpIp_CtrlRuntimeType* ctrlRtPtr, const TcpIp_LocalAddrType* localAddrCfgPtr)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == TCPIP_CTRL_MULTI_LOCALADDR)
    TcpIp_LocalAddrIdType localAddrIndex = localAddrCfgPtr->LocalAddrIndex;
#endif

#if !TCPIP_ALLOW_RX_INVALID_LOCAL_IP
    TcpIp_LocalAddrRuntimeType* localAddrRtPtr = localAddrCfgPtr->RuntimePtr;

    localAddrRtPtr->RouterIp.addr = TCPIP_IPADDR_ANY;
    localAddrRtPtr->Netmask.addr  = TCPIP_IPADDR_ANY;
    localAddrRtPtr->LocalIp.addr  = TCPIP_IPADDR_ANY;
#endif

    switch (localAddrCfgPtr->AddrType)
    {
    case TCPIP_ADDRESS_UNICAST:
#if (STD_ON == TCPIP_CTRL_MULTI_LOCALADDR)
        TcpIp_CtrlLocalAddrResetUsed(ctrlRtPtr, localAddrIndex);

        /* all local address is release shall be set netif addr is null in this control */
        if (!TcpIp_CtrlLocalAddrIsAnyUsed(ctrlRtPtr))
#endif
        {
            struct netif* netifPtr = &(ctrlRtPtr->Netif);

            netif_set_addr(netifPtr, NULL_PTR, NULL_PTR, NULL_PTR);
        }

        ret = E_OK;
        break;

    case TCPIP_ADDRESS_MULTICAST:
        TcpIp_MultCastIpAssigmentHandle(ctrlRtPtr, localAddrCfgPtr, FALSE);

        ret = E_OK;
        break;

    default:
        /* do nothing */
        break;
    }

    return ret;
}

/**
 * called in exclusive area
 */
TCPIP_LOCAL Std_ReturnType TcpIp_DoRequestIpAddrAssignment(
    const TcpIp_CtrlType*                  ctrlCfgPtr,
    TcpIp_CtrlRuntimeType*                 ctrlRtPtr,
    const TcpIp_LocalAddrType*             localAddrCfgPtr,
    TcpIp_IpAddrAssignmentType             assignType,
    const TcpIp_StaticIpAddressConfigType* ipAddrCfgPtr)
{
    Std_ReturnType ret = E_NOT_OK;

    TCPIP_UNUSED_ARG(ctrlCfgPtr);

    switch (assignType)
    {
    case TCPIP_IPADDR_ASSIGNMENT_STATIC:
        if (NULL_PTR != localAddrCfgPtr->IpAddrCfgPtr)
        {
            ipAddrCfgPtr = localAddrCfgPtr->IpAddrCfgPtr;
        }
        if (NULL_PTR != ipAddrCfgPtr)
        {
            ret = TcpIp_SetLocalAddrStatic(ctrlRtPtr, localAddrCfgPtr, ipAddrCfgPtr);
        }
        break;

    case TCPIP_IPADDR_ASSIGNMENT_DHCP:
#if (STD_ON == TCPIP_DHCP_CLIENT_ENABLED)
        ret = TcpIp_StartDhcp(ctrlRtPtr);
#endif
        break;

    case TCPIP_IPADDR_ASSIGNMENT_LINKLOCAL_DOIP:
    case TCPIP_IPADDR_ASSIGNMENT_LINKLOCAL:
#if (STD_ON == TCPIP_AUTOIP_ENABLED)
        ret = TcpIp_StartAutoIp(ctrlCfgPtr, ctrlRtPtr);
#endif
        break;

    default:
        /* do nothing */
        break;
    }

    return ret;
}

/**
 * called in exclusive area
 */
TCPIP_LOCAL Std_ReturnType TcpIp_DoReleaseIpAddrAssignment(
    const TcpIp_CtrlType*      ctrlCfgPtr,
    TcpIp_CtrlRuntimeType*     ctrlRtPtr,
    const TcpIp_LocalAddrType* localAddrCfgPtr,
    TcpIp_IpAddrAssignmentType assignType)
{
    Std_ReturnType              ret            = E_NOT_OK;
    TcpIp_LocalAddrRuntimeType* localAddrRtPtr = localAddrCfgPtr->RuntimePtr;

    TCPIP_UNUSED_ARG(ctrlCfgPtr);

    if ((TcpIp_LocalAddrFlagIsSet(localAddrRtPtr, TCPIP_LOCALADDR_FLAG_IP_IS_VALID)))
    {
        TcpIp_LocalAddrFlagReset(localAddrRtPtr, TCPIP_LOCALADDR_FLAG_IP_IS_VALID);
        TcpIp_LocalAddrFlagSet(localAddrRtPtr, TCPIP_LOCALADDR_FLAG_REQUEST_IP);
    }

    switch (assignType)
    {
    case TCPIP_IPADDR_ASSIGNMENT_STATIC:
        ret = TcpIp_ClearLocalAddrStatic(ctrlRtPtr, localAddrCfgPtr);
        break;

    case TCPIP_IPADDR_ASSIGNMENT_DHCP:
#if (STD_ON == TCPIP_DHCP_CLIENT_ENABLED)
        ret = TcpIp_StopDhcp(ctrlRtPtr);
#endif
        break;

    case TCPIP_IPADDR_ASSIGNMENT_LINKLOCAL_DOIP:
    case TCPIP_IPADDR_ASSIGNMENT_LINKLOCAL:
#if (STD_ON == TCPIP_AUTOIP_ENABLED)
        ret = TcpIp_StopAutoIp(ctrlCfgPtr, ctrlRtPtr);
#endif
        break;

    default:
        /* do nothing */
        break;
    }

    if (E_OK != ret)
    {
        TcpIp_LocalAddrFlagReset(localAddrRtPtr, TCPIP_LOCALADDR_FLAG_REQUEST_IP);
    }

    return ret;
}

/**
 * called in exclusive area
 */
TCPIP_LOCAL void
    TcpIp_RequestIpAddrAssignmentAutomaticForCtrl(const TcpIp_CtrlType* ctrlCfgPtr, TcpIp_CtrlRuntimeType* ctrlRtPtr)
{
    for (TcpIp_LocalAddrIdType localAddrId = ctrlCfgPtr->LocalAddrIdMin;
         localAddrId < ctrlCfgPtr->LocalAddrIdMaxExcluded;
         ++localAddrId)
    {
        const TcpIp_LocalAddrType* localAddrCfgPtr = &TCPIP_LOCALADDR_CONFIG(localAddrId);

        for (uint8 addrAssignIndex = 0u; addrAssignIndex < localAddrCfgPtr->AddrAssignNum; ++addrAssignIndex)
        {
            const TcpIp_AddrAssignmentType* addrAssignCfgPtr = &(localAddrCfgPtr->AddrAssignPtr[addrAssignIndex]);

            if (TCPIP_ASSIGN_TRIGGER_AUTOMATIC == addrAssignCfgPtr->AssignTrigger)
            {
                (void)TcpIp_DoRequestIpAddrAssignment(
                    ctrlCfgPtr,
                    ctrlRtPtr,
                    localAddrCfgPtr,
                    addrAssignCfgPtr->AssignMethod,
                    NULL_PTR);
            }
        }
    }
}

/**
 * called in exclusive area
 */
TCPIP_LOCAL void
    TcpIp_ReleaseIpAddrAssignmentForCtrl(const TcpIp_CtrlType* ctrlCfgPtr, TcpIp_CtrlRuntimeType* ctrlRtPtr)
{
    for (TcpIp_LocalAddrIdType localAddrId = ctrlCfgPtr->LocalAddrIdMin;
         localAddrId < ctrlCfgPtr->LocalAddrIdMaxExcluded;
         ++localAddrId)
    {
        const TcpIp_LocalAddrType* localAddrCfgPtr = &TCPIP_LOCALADDR_CONFIG(localAddrId);

        for (uint8 addrAssignIndex = 0u; addrAssignIndex < localAddrCfgPtr->AddrAssignNum; ++addrAssignIndex)
        {
            const TcpIp_AddrAssignmentType* addrAssignCfgPtr = &(localAddrCfgPtr->AddrAssignPtr[addrAssignIndex]);

            (void)
                TcpIp_DoReleaseIpAddrAssignment(ctrlCfgPtr, ctrlRtPtr, localAddrCfgPtr, addrAssignCfgPtr->AssignMethod);
        }
    }
}

#if (STD_ON == TCPIP_TCP_ENABLED)
/**
 * called in exclusive area
 */
TCPIP_LOCAL uint16 TcpIp_TcpTransmitCopyTxData(TcpIp_SocketRuntimeType* socketRtPtr)
{
    const struct tcp_pcb* pcbPtr        = (struct tcp_pcb*)socketRtPtr->PcbPtr;
    struct pbuf*          pbufPtr       = socketRtPtr->UpCopyTxPbufPtr;
    uint16                remainLen     = socketRtPtr->UpCopyTxRemainLen;
    boolean               forceRetrieve = TcpIp_SocketFlagIsSet(socketRtPtr, TCPIP_SOCKET_FLAG_TCP_FORCERETRIEVE);
    uint16                copyLen       = 0u;

    if ((NULL_PTR != pbufPtr) && (!forceRetrieve))
    {
        if (((tcpwnd_size_t)pbufPtr->tot_len + (tcpwnd_size_t)remainLen) <= pcbPtr->snd_buf)
        {
            copyLen = remainLen;
        }
        else
        {
            copyLen = (uint16)(pcbPtr->snd_buf - (tcpwnd_size_t)pbufPtr->tot_len);
        }
    }
    else
    {
        if ((tcpwnd_size_t)remainLen <= pcbPtr->snd_buf)
        {
            copyLen = remainLen;
        }
        else
        {
            copyLen = (uint16)pcbPtr->snd_buf;
        }
    }

    if (0u < copyLen)
    {
        const TcpIp_UpCopyTxDataFuncType copyFuncPtr = socketRtPtr->OwnerCfgPtr->Up_CopyTxDataFuncPtr;

        pbufPtr = pbuf_alloc(PBUF_RAW, (u16_t)copyLen, PBUF_RAM);
        TcpIp_ExitExclusiveArea();

#if defined(TCPIP_INNER_CHECK)
        if ((NULL_PTR != pbufPtr) && (copyLen == (uint16)pbufPtr->len) && (pbufPtr->len == pbufPtr->tot_len)
            && (NULL_PTR == pbufPtr->next))
#endif
        {
            BufReq_ReturnType bufRet =
                copyFuncPtr(socketRtPtr->SocketId, (uint8*)pbufPtr->payload, (uint16)pbufPtr->len);

            TcpIp_EnterExclusiveArea();
            if ((BUFREQ_OK == bufRet)
#if defined(TCPIP_INNER_CHECK)
                && (pcbPtr == socketRtPtr->PcbPtr)
#endif
            )
            {
                if (NULL_PTR != socketRtPtr->UpCopyTxPbufPtr)
                {
                    pbuf_cat(socketRtPtr->UpCopyTxPbufPtr, pbufPtr);
                    pbufPtr                      = socketRtPtr->UpCopyTxPbufPtr;
                    socketRtPtr->UpCopyTxPbufPtr = NULL_PTR;
                }
            }
            else
            {
                (void)pbuf_free(pbufPtr);
                pbufPtr = NULL_PTR;

#if defined(TCPIP_INNER_CHECK)
                if (pcbPtr != socketRtPtr->PcbPtr)
                {
                    copyLen = remainLen;
                }
                else
#endif
                {
                    copyLen = 0u;
                }
            }
        }
#if defined(TCPIP_INNER_CHECK)
        else
        {
            TcpIp_EnterExclusiveArea();
            if (NULL_PTR != pbufPtr)
            {
                (void)pbuf_free(pbufPtr);
                pbufPtr = NULL_PTR;
            }

            copyLen = 0u;
        }
#endif
    }

    socketRtPtr->UpCopyTxPbufPtr = pbufPtr;

    return copyLen;
}

/**
 * called in exclusive area
 */
TCPIP_LOCAL void TcpIp_TcpTransmitCopyWrite(TcpIp_SocketRuntimeType* socketRtPtr, uint16 copyLen)
{
    struct pbuf* pbufPtr       = socketRtPtr->UpCopyTxPbufPtr;
    uint16       remainLen     = socketRtPtr->UpCopyTxRemainLen;
    boolean      forceRetrieve = TcpIp_SocketFlagIsSet(socketRtPtr, TCPIP_SOCKET_FLAG_TCP_FORCERETRIEVE);

    if ((NULL_PTR != pbufPtr) && ((copyLen == remainLen) || (!forceRetrieve)))
    {
        err_t err = (err_t)ERR_ENUM_MAX;

        pbufPtr = pbuf_coalesce(pbufPtr, PBUF_RAW);

        if ((NULL_PTR == pbufPtr->next) || (!forceRetrieve))
        {
            struct tcp_pcb* pcbPtr = (struct tcp_pcb*)socketRtPtr->PcbPtr;

#if (STD_ON == TCPIP_TCP_TLS_ENABLED)
            if (NULL_PTR != socketRtPtr->tlsPtr)
            {
                err = TcpIp_TlsWrite(socketRtPtr, pbufPtr->payload, pbufPtr->len);
            }
            else
#endif
            {
                err = tcp_write(pcbPtr, pbufPtr->payload, pbufPtr->len, TCPIP_TCP_WRITE_API_FLAG);
#if TCPIP_FASTTX_TCP
                err += tcp_output(pcbPtr);
#endif
            }
        }
        else
        {
            err = (err_t)ERR_MEM;
        }

        if ((err_t)ERR_OK == err)
        {
            socketRtPtr->UpCopyTxPbufPtr = pbufPtr->next;
            pbufPtr->next                = NULL_PTR;
            (void)pbuf_free(pbufPtr);
            pbufPtr = socketRtPtr->UpCopyTxPbufPtr;
        }
        else if ((err_t)ERR_MEM == err)
        {
            /* try again */
        }
        else
        {
            (void)pbuf_free(pbufPtr);
            pbufPtr = NULL_PTR;

            copyLen = remainLen;
        }
    }

    socketRtPtr->UpCopyTxPbufPtr = pbufPtr;
    socketRtPtr->UpCopyTxRemainLen -= copyLen;
}

/**
 * called in rx indication
 */
TCPIP_LOCAL TcpIp_SocketRuntimeType*
    TcpIp_MallocTcpSocketByPcb(const TcpIp_SocketRuntimeType* socketRtPtr, struct tcp_pcb* pcbPtr)
{
    ApplicationType          partitionIndex  = TcpIp_MulticoreIndex;
    TcpIp_RuntimeType*       runtimePtr      = TCPIP_RUNTIME(partitionIndex);
    TcpIp_SocketIdType       socketLastIndex = runtimePtr->SocketLastIndex;
    TcpIp_SocketRuntimeType* retSocketRtPtr  = NULL_PTR;

    if ((TCPIP_TCP_SOCKET_MAX > runtimePtr->TcpSocketUsedNum) && (TCPIP_SOCKET_NUM > socketLastIndex))
    {
        uint8                  ctrlIndex  = socketRtPtr->CtrlIndex;
        const TcpIp_CtrlType*  ctrlCfgPtr = &TCPIP_CTRL_CONFIG(ctrlIndex);
        TcpIp_CtrlRuntimeType* ctrlRtPtr  = ctrlCfgPtr->RuntimePtr;

        retSocketRtPtr = &TCPIP_SOCKET(partitionIndex, socketLastIndex);

        runtimePtr->SocketLastIndex = retSocketRtPtr->NextFreeIndex;
        runtimePtr->TcpSocketUsedNum += 1u;

        ctrlRtPtr->SocketUsedNum += 1u;

        (void)IStdLib_MemSet(retSocketRtPtr, 0u, sizeof(TcpIp_SocketRuntimeType));
        retSocketRtPtr->SocketId      = TCPIP_SOCKET_ID(partitionIndex, socketLastIndex);
        retSocketRtPtr->NextFreeIndex = TCPIP_SOCKET_NUM;
        retSocketRtPtr->PcbPtr        = (void*)pcbPtr;
        retSocketRtPtr->Protocol      = TCPIP_IPPROTO_TCP;
        retSocketRtPtr->OwnerCfgPtr   = socketRtPtr->OwnerCfgPtr;
        retSocketRtPtr->CtrlIndex     = socketRtPtr->CtrlIndex;
#if (STD_ON == TCPIP_TCP_TLS_ENABLED)
        retSocketRtPtr->listenSocketId = TCPIP_SOCKET_INDEX_BY_ADDR(partitionIndex, socketRtPtr);
#endif

        tcp_ext_arg_set(pcbPtr, 0u, retSocketRtPtr);
        tcp_ext_arg_set_callbacks(pcbPtr, 0u, &TcpIp_TcpExtArgCallbackTable[TCPIP_TCPEXTARG_CALLBACK_INDEX_ENABLED]);
        tcp_arg(pcbPtr, retSocketRtPtr);
        tcp_recv(pcbPtr, TcpIp_TcpRecvCallback);
        tcp_sent(pcbPtr, TcpIp_TcpSentCallback);
        tcp_err(pcbPtr, TcpIp_TcpErrCallback);
    }

    return retSocketRtPtr;
}

/**
 * called in rx indication or in exclusive area
 */
TCPIP_LOCAL void TcpIp_FreeTcpSocket(TcpIp_SocketRuntimeType* socketRtPtr)
{
    ApplicationType    partitionIndex = TcpIp_MulticoreIndex;
    TcpIp_RuntimeType* runtimePtr     = TCPIP_RUNTIME(partitionIndex);
    TcpIp_SocketIdType socketIndex    = TCPIP_SOCKET_INDEX_BY_ADDR(partitionIndex, socketRtPtr);

#if defined(TCPIP_INNER_CHECK)
    if ((TCPIP_SOCKET_NUM > socketIndex) && (NULL_PTR != socketRtPtr->PcbPtr))
#endif
    {
        uint8 ctrlIndex = socketRtPtr->CtrlIndex;

        if (TCPIP_CTRL_INDEX_INVALID > ctrlIndex)
        {
            const TcpIp_CtrlType*  ctrlCfgPtr = &TCPIP_CTRL_CONFIG(ctrlIndex);
            TcpIp_CtrlRuntimeType* ctrlRtPtr  = ctrlCfgPtr->RuntimePtr;

            if (0u < ctrlRtPtr->SocketUsedNum)
            {
                ctrlRtPtr->SocketUsedNum -= 1u;
            }
            if ((0u == ctrlRtPtr->SocketUsedNum) && TcpIp_CtrlStateIsPending(ctrlRtPtr, TCPIP_STATE_SHUTDOWN))
            {
                struct netif* netifPtr = &(ctrlRtPtr->Netif);

                TcpIp_CtrlStateResetPending(ctrlRtPtr, TCPIP_STATE_SHUTDOWN);

                ctrlRtPtr->CtrlStatus = TCPIP_STATE_OFFLINE;
                TcpIp_EthSmModeIndicationNotifyForCtrl(ctrlCfgPtr, TCPIP_STATE_OFFLINE);

                netif_set_down(netifPtr);
                netif_set_link_down(netifPtr);
            }
        }

        socketRtPtr->NextFreeIndex = runtimePtr->SocketLastIndex;
        socketRtPtr->PcbPtr        = NULL_PTR;
        if (NULL_PTR != socketRtPtr->UpCopyTxPbufPtr)
        {
            (void)pbuf_free(socketRtPtr->UpCopyTxPbufPtr);
            socketRtPtr->UpCopyTxPbufPtr = NULL_PTR;
        }
        socketRtPtr->UpCopyTxRemainLen = 0u;
#if (STD_ON == TCPIP_TCP_TLS_ENABLED)
        socketRtPtr->listenSocketId = TCPIP_SOCKET_NUM;
        socketRtPtr->tlsPtr         = NULL_PTR;
#endif

        runtimePtr->SocketLastIndex = socketIndex;
        runtimePtr->TcpSocketUsedNum -= 1u;
    }
}
#endif

#if (STD_ON == TCPIP_UDP_ENABLED)
/**
 * called in exclusive area
 */
TCPIP_LOCAL void TcpIp_FreeUdpSocket(TcpIp_SocketRuntimeType* socketRtPtr)
{
    ApplicationType    partitionIndex = TcpIp_MulticoreIndex;
    TcpIp_RuntimeType* runtimePtr     = TCPIP_RUNTIME(partitionIndex);
    TcpIp_SocketIdType socketIndex    = TCPIP_SOCKET_INDEX_BY_ADDR(partitionIndex, socketRtPtr);

#if defined(TCPIP_INNER_CHECK)
    if ((TCPIP_SOCKET_NUM > socketIndex) && (NULL_PTR != socketRtPtr->PcbPtr))
#endif
    {
        uint8 ctrlIndex = socketRtPtr->CtrlIndex;

        if (TCPIP_CTRL_INDEX_INVALID > ctrlIndex)
        {
            const TcpIp_CtrlType*  ctrlCfgPtr = &TCPIP_CTRL_CONFIG(ctrlIndex);
            TcpIp_CtrlRuntimeType* ctrlRtPtr  = ctrlCfgPtr->RuntimePtr;

            if (0u < ctrlRtPtr->SocketUsedNum)
            {
                ctrlRtPtr->SocketUsedNum -= 1u;
            }
            if ((0u == ctrlRtPtr->SocketUsedNum) && TcpIp_CtrlStateIsPending(ctrlRtPtr, TCPIP_STATE_SHUTDOWN))
            {
                struct netif* netifPtr = &(ctrlRtPtr->Netif);

                TcpIp_CtrlStateResetPending(ctrlRtPtr, TCPIP_STATE_SHUTDOWN);

                ctrlRtPtr->CtrlStatus = TCPIP_STATE_OFFLINE;
                TcpIp_EthSmModeIndicationNotifyForCtrl(ctrlCfgPtr, TCPIP_STATE_OFFLINE);

                netif_set_down(netifPtr);
                netif_set_link_down(netifPtr);
            }
        }

        socketRtPtr->NextFreeIndex = runtimePtr->SocketLastIndex;
        socketRtPtr->PcbPtr        = NULL_PTR;

        runtimePtr->SocketLastIndex = socketIndex;
        runtimePtr->UdpSocketUsedNum -= 1u;
    }
}
#endif

#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"
