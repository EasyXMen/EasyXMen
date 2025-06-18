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
**************************************************************************** **
**                                                                           **
**  FILENAME    : SoAd.c                                                     **
**                                                                           **
**  Created on  :                                                            **
**  Author      : zhengfei.li                                                **
**  Vendor      :                                                            **
**  DESCRIPTION : none                                                       **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2 and R19_11           **
**                                                                           **
**************************************************************************** */
/*******************************************************************************
**                      REVISION   HISTORY                                    **
*******************************************************************************/
/* <VERSION>  <DATE>    <AUTHOR>      <REVISION LOG>
 *  V2.0.0    20200507  zhengfei.li   Initial version
 *                                    (Upgrade according to the R19_11 standards)
 *  V2.0.1    20211108  zhengfei.li   Set up administrative data before sending
 *                                    TCP message(Avoid TxConfirmation coming early)
 *  V2.0.2    20220104  zhengfei.li   Udp status management parameters are set first
 *                                    and then notified to the upper layer.
 *                                    SoAdCopyTxData management variables plus
 *                                    interrupt protection.
 *  V2.0.3    20220106  zhengfei.li   SoAd_TxConfirmation invalid SoCon protection.
 *  V2.0.4    20220610  darren.zhang  SoAd_TcpIpEvent reentrt modify,
 *                                    reslove bsd tcp server socket repeat close
 *  V2.0.5    20230803  fupeng.yu     SoAd_HeaderEnableIfPduHandle modify the logic
 *                                    of copying SoAd_PduBuffer to SoAd_IfPduBuffer.
 *                                    resolve data of rxindication is not a correct
 *  V2.0.6    20230821  fupeng.yu     When the received IF UDP data with headerid is
 *                                    processing, the release of the remote address
 *                                    should be locked
 *  V2.0.7    20231124  fupeng.yu     Performance optimization
 *  V2.0.8    20231225  fupeng.yu     add logic of event with TCPIP_TCP_RESET for socked connect
 *                                    group in SoAd_TcpIpEvent
 *  V2.0.9    20240202  fupeng.yu     In SoAd_TcpIpEvent modify close_socket logic for
 *                                    received TCPIP_TCP_FIN_RECEIVED of TCP when TP data of received
 *                                    not all copy to upper layer
 *  V2.0.10   20240229  fupeng.yu     1. resolve QAC
 *                                    2. used istd_lib API
 *                                    3. In SoAd_HeaderEnableNewPduHandle, resolve TCP connection cannot close
 *                                       when TCP receives TCPIP-TCP-FIN and the part of PDU
 *  V2.0.11   20240320  fupeng.yu     resolve running exception in SoAd_FindTheValidSoCon
 *  V2.0.12   20240501  fupeng.yu     1.add feature of udp immediate txconfirmation when The same pdu is sent multiple
 *                                      times.
 *                                    2.When the received IF UDP data with multiple headerid is processing, the
 *                                      release of the remote address should be locked.
 *  V2.0.13   20240726  fupeng.yu     1.add feature of udp which release state of txconfirmation when The same pdu is
 *                                      sent multiple times and <up_>txconfirmation is not configured.
 *  V2.0.14   20240828  fupeng.yu     Add exception handling for SoAd_RxBufCfgIndexPtr in
 *                                    SoAd_HeaderEnableTcpSoConRxHandle
 *  V2.0.15   20240830  fupeng.yu     Resolve compilation errors when no SocketRoute is configured.
 *  V2.0.16   20250110  fupeng.yu     Resolve compilation errors.CPT-12341
 *  V2.0.17   20250113  fupeng.yu     add SoAdPduRouteDest reference SoAdSocketConnectionGroup
 *  V2.0.18   20250331  fupeng.yu     Add specific soConId handling for SoAd_PduRouteDestControl
 *  V2.0.19   20250417  fupeng.yu     To resolve the transmission failure issue of SocketConnectionGroup referencing
 *                                    PduRouteDest in SoAd_IfSpecificRoutingGroupTransmit.
 * ******************************************************************************/
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:SoAd<br>
  RuleSorce:puhua-rule2024-2.rcf 2.3.5

    \li PRQA S 2991 MISRA Rule 14.3 .<br>
    Reason:No impact was assessed and the status quo was maintained.

    \li PRQA S 2995 MISRA Rule 2.2 .<br>
    Reason:No impact was assessed and the status quo was maintained.

    \li PRQA S 3469 MISRA Dir 4.9 .<br>
    Reason: Function-like macro are used to Improve maintainability code.

 */
/*===========================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
============================================================================*/
#include "istd_lib.h"
#include "SoAd.h"
#include "SoAd_Cbk.h"
#include "Det.h"

/*===========================================================================
*                              SOURCE FILE VERSION INFORMATION
============================================================================*/
#define SOAD_VENDOR_ID_C        62u
#define SOAD_C_AR_MAJOR_VERSION 4u
#define SOAD_C_AR_MINOR_VERSION 5u
#define SOAD_C_AR_PATCH_VERSION 0u
#define SOAD_C_SW_MAJOR_VERSION 2u
#define SOAD_C_SW_MINOR_VERSION 0u
#define SOAD_C_SW_PATCH_VERSION 17u
/*==========================================================================
*                                     FILE VERSION CHECKS
===========================================================================*/
/* Check if current file and SOAD header file are of the same vendor */
#if (SOAD_VENDOR_ID_C != SOAD_VENDOR_ID)
#error "SoAd.c and SoAd.h have different vendor ids"
#endif /* SOAD_VENDOR_ID_C != SOAD_VENDOR_ID */
/* Check if current file and SOAD header file are of
 the same Autosar version */
#if (                                                                                                            \
    (SOAD_C_AR_MAJOR_VERSION != SOAD_H_AR_MAJOR_VERSION) || (SOAD_C_AR_MINOR_VERSION != SOAD_H_AR_MINOR_VERSION) \
    || (SOAD_C_AR_PATCH_VERSION != SOAD_H_AR_PATCH_VERSION))
#error "AutoSar Version Numbers of SoAd.c and SoAd.h are different"
#endif

/* line of socket need to close for debug */
#define SOAD_SET_SOCKET_NEED_CLOSE_DEBUG STD_OFF

/*==============================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS).
==============================================================================*/
typedef enum
{
    SOAD_STATE_UNINIT = 0u,
    SOAD_STATE_INIT
} SoAd_Module_StateType;

typedef enum
{
    SOAD_SOCON_REQUEST_NONE = 0u,
    SOAD_SOCON_REQUEST_OPEN,
    SOAD_SOCON_REQUEST_CLOSE
} SoAd_SoConRequestType;

typedef enum
{
    SOAD_SOCKET_CLOSE_NONE = 0u,
    SOAD_SOCKET_CLOSE_IMMEDIATE,
    SOAD_SOCKET_CLOSE_DEFERRED
} SoAd_SocketCloseType;

typedef struct
{
    PduLengthType txPduTotalLength;
    PduLengthType txResidueLength;
    PduLengthType upCopyLength;
#if (STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER)
    uint32 tirgTimer;
#endif /* STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER */
} SoAd_SoTxBufManType;

typedef struct
{
    PduLengthType EnterPosition;
    PduLengthType ExitPosition;
    PduLengthType ExitPduResidueLength;
    SoAd_uintx SoAd_SocketRouteId;
} SoAd_SoRxBufferManType;

typedef struct
{
    boolean SoConCloseAbort;
    boolean RemoteAddrIsSet;
    TcpIp_SockAddrType RemoteAddr;
    boolean TcpIpSocketValid;
    SoAd_SocketCloseType SocketNeedClose;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
    uint32 SetSocketNeedCloseLine;
#endif /* STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG */
    SoAd_SoConModeType SoAd_SoConMode;
    SoAd_SoConRequestType SoCon_Request[2u];
    PduIdType ActiveSoAdPduRouteId;
#if (STD_ON == SOAD_SUPPORT_UDP)
    uint32 UdpAliveSupervisionTime;
#endif /* STD_ON == SOAD_SUPPORT_UDP */
    TcpIp_SocketIdType TcpIpSocketId;
    PduLengthType TxNoAckLength;
    boolean TpPduTransmitting;
    boolean RemoteAddrNeedRelease;
    boolean ChangeModeToOffline;
    boolean RxIfProcessing;
} SoAd_InnerSocketManageType;

typedef struct
{
    boolean TcpIpSocketValid;
    TcpIp_IpAddrStateType LocalIpAddrState;
    TcpIp_SocketIdType TcpIpSocketId;
    SoAd_SoConIdType SoConSocketValidNum;
} SoAd_InnerSocketGroupManageType;

typedef struct
{
    uint8 TxPendingNum;
    uint8* PduDataPtr;
    PduLengthType PduLength;
    SoAd_SoConIdType TxSoConId;
} SoAd_PduRoutInnerManageType;

typedef struct
{
    boolean RoutGroupEnable;
} SoAd_RoutingGroupInnerManageType;

typedef struct
{
    uint8 PduRouteDestRefSoConEnable[SOAD_SO_CON_MAX / 8u + 1];
    /*the PduRouteDest TCP not ACK position of all TCP not ACK data*/
    PduLengthType TxPendingLength;
} SoAd_PduRouteDestInnerManagerType;

typedef struct
{
    boolean SocketRouteDestEnable;
} SoAd_SocketRouteDestInnerManagerType;

/*used by SoAd_RxFullTpPduHandle*/
typedef struct
{
    SoAd_SoConIdType SoConId;
    SoAd_uintx SocketRouteId;
    PduLengthType RxDataLength;
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) DataPtr;
    PduLengthType UpBufferSize;
    PduLengthType PduLength;
    SoAd_UpLayerModuleType UpModuleId;
    SoAd_uintx RxBufferId;
    TcpIp_SocketIdType TcpIpSocketId;
} SoAd_RxFullTpPduHandleType;

/*used by SoAd_RxFullTpPduHandle*/
#if (STD_ON == SOAD_SUPPORT_UDP)
typedef struct
{
    P2VAR(boolean, AUTOMATIC, SOAD_APPL_DATA) RemoteAddrChanged;
    P2VAR(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) RemoteAddr;
} SoAd_FindTheValidSoConType;
#endif
/*==============================================================================
*                                       LOCAL MACROS
==============================================================================*/
#if !defined(SOAD_LOCAL_INLINE)
#define SOAD_LOCAL_INLINE static inline
#endif

#if (STD_ON == SOAD_DEV_ERROR_DETECT)
SOAD_LOCAL_INLINE void SOAD_DET_REPORT(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE, ApiId, ErrorId);
}
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
#define SOAD_UNUSED_UINT8  (0xFFu)
#define SOAD_UNUSED_UINT32 (0xFFFFFFFFu)
#define SOAD_UNUSED_UINT16 (0xFFFFu)
#define SOAD_HEADER_SIZE   (8u)
/*configuration parameter addressing capability*/
#define SOAD_SOCKET_TXBUF_INDEX_CFG(soId) SoAd_PbCfgPtr->SoAd_SocketConnection[soId].SoAd_TxBufCfgIndexPtr
#define SOAD_SOCKET_LINKGORUP(soId)       SoAd_PbCfgPtr->SoAd_SocketConnection[soId].SoAd_SocketConnectionGroupRef
#define SOAD_PDUROUT_CFG(pduId)           SoAd_PbCfgPtr->SoAd_PduRoute[pduId]
#define SOAD_PDUROT_DESTINDEX_CFG(pduId, destIndex) \
    SoAd_PbCfgPtr->SoAd_PduRoute[pduId].SoAd_PduRouteDestIndex[destIndex]
#define SOAD_PDUROTDEST_CFG(pduRouDestIndex) SoAd_PbCfgPtr->SoAd_PduRouteDestTotalPtr[pduRouDestIndex]
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
#define SOAD_UPPERLAY_PCCFG(upModule) SoAd_PcCfg.SoAd_UpLayerFuncTalbe[upModule]
#endif /* SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u */
#define SOAD_ROUTGROUP_CFG(routGroupId)    SoAd_PbCfgPtr->SoAd_RoutingGroup[routGroupId]
#define SOAD_ROUTGROUP_TXTRIG(routGroupId) SoAd_PbCfgPtr->SoAd_RoutingGroup[routGroupId].SoAd_RoutingGroupTxTriggerable
/* BigEndian */
#if (STD_ON == SOAD_SUPPORT_HEADERID)
SOAD_LOCAL_INLINE void SOAD_FILL_HEADER(uint8* buffer, uint32 headerId, uint16 length)
{
    (buffer)[0u] = (uint8)(((uint32)(headerId)) >> 24u);
    (buffer)[1u] = (uint8)(((uint32)(headerId)) >> 16u);
    (buffer)[2u] = (uint8)(((uint32)(headerId)) >> 8u);
    (buffer)[3u] = (uint8)((uint32)(headerId));
    (buffer)[4u] = (uint8)(((uint32)(length)) >> 24u);
    (buffer)[5u] = (uint8)(((uint32)(length)) >> 16u);
    (buffer)[6u] = (uint8)(((uint32)(length)) >> 8u);
    (buffer)[7u] = (uint8)((uint32)(length));
}
#endif /* STD_ON == SOAD_SUPPORT_HEADERID */

#define SOAD_PDU_ROUTE_DEST_NUM           (SoAd_PbCfgPtr->SoAd_PduRouteDestTotalNum)
#define SOAD_SOCKET_ROUTE_DEST_NUM        (SoAd_PbCfgPtr->SoAd_SocketRouteDestTotalNum)
#define SOAD_SO_CON_GROUP_NUM             (SoAd_PbCfgPtr->SoAd_SocketConnectionGroupNum)
#define SOAD_PDU_ROUTE_NUM                (SoAd_PbCfgPtr->SoAd_PduRouteNum)
#define SOAD_ROUTING_GROUP_NUM            (SoAd_PbCfgPtr->SoAd_RoutingGroupNum)
#define SOAD_SOCON_NUM                    (SoAd_PbCfgPtr->SoAd_SocketConnectionNum)

#define SOAD_SOCON_CFG(soConId)           SoAd_PbCfgPtr->SoAd_SocketConnection[soConId]
#define SOAD_SOCONGROUP_CFG(soConGroupId) SoAd_PbCfgPtr->SoAd_SocketConnectionGroup[soConGroupId]
#define SOAD_SOCKETLOCALADDR_CFG(soConGroupId) \
    SoAd_PbCfgPtr->SoAd_SocketConnectionGroup[soConGroupId].SoAd_SocketLocalAddressRef
#define SOAD_SOCONREMOTEADDR_CFG(soConId) SoAd_PbCfgPtr->SoAd_SocketConnection[soConId].SoAd_SocketRemoteAddress
#if (STD_ON == SOAD_SUPPORT_TCP)
#define SOAD_TCPTYPE_CFG(soConGroupId) \
    SoAd_PbCfgPtr->SoAd_SocketConnectionGroup[soConGroupId].SoAd_SocketProtocol.SoAd_SocketTcpPtr
#define SOAD_SOCONGROUP_TCPIMME_TXCONF(soConGop)        \
    SoAd_PbCfgPtr->SoAd_SocketConnectionGroup[soConGop] \
        .SoAd_SocketProtocol.SoAd_SocketTcpPtr->SoAd_SocketTcpImmediateTpTxConfirmation
#endif /* STD_ON == SOAD_SUPPORT_TCP */
#if (STD_ON == SOAD_SUPPORT_UDP)
#define SOAD_UDPTYPE_CFG(soConGroupId) \
    SoAd_PbCfgPtr->SoAd_SocketConnectionGroup[soConGroupId].SoAd_SocketProtocol.SoAd_SocketUdpPtr
#endif /* STD_ON == SOAD_SUPPORT_UDP */
#define SOAD_SRDEST_CFG(socketRouteId)      SoAd_PbCfgPtr->SoAd_SocketRouteDestTotalPtr[socketRouteId]
#define SOAD_SOCKETBUFFER_CFG(bufferId)     SoAd_PbCfgPtr->SoAd_SocketBufferCfgPtr[bufferId]
#define SOAD_SOCKETROUTE_CFG(socketRouteId) SoAd_PbCfgPtr->SoAd_SocketRoute[socketRouteId]
#define SOAD_PDUROUTE_CFG(pduRouteId)       SoAd_PbCfgPtr->SoAd_PduRoute[pduRouteId]
SOAD_LOCAL_INLINE uint32 SOAD_GET_HEADER(const uint8* dataPtr, uint32 bufStartId)
{
    return (
        ((uint32)(dataPtr)[(bufStartId)] << 24u) | ((uint32)(dataPtr)[(bufStartId) + 1u] << 16u)
        | ((uint32)(dataPtr)[(bufStartId) + 2u] << 8u) | ((uint32)(dataPtr)[(bufStartId) + 3u]));
}
SOAD_LOCAL_INLINE uint32 SOAD_GET_LENGTH(const uint8* dataPtr, uint32 bufStartId)
{
    return (
        ((uint32)(dataPtr)[(bufStartId) + 4u] << 24u) | ((uint32)(dataPtr)[(bufStartId) + 5u] << 16u)
        | ((uint32)(dataPtr)[(bufStartId) + 6u] << 8u) | ((uint32)(dataPtr)[(bufStartId) + 7u]));
}

SOAD_LOCAL_INLINE boolean SOAD_IS_IPADDR_ANY(TcpIp_SockAddrType IpAddr)
{
    return (IpAddr.addr[0u] == 0x00000000u);
}
SOAD_LOCAL_INLINE boolean SOAD_EQ_IPADDR(TcpIp_SockAddrType IpAddr1, TcpIp_SockAddrType IpAddr2)
{
    return (IpAddr1.addr[0u] == IpAddr2.addr[0u]);
}

#if (0x100u > SOAD_SO_CON_MAX)
#define SOAD_METADATATOSOCONID(SoConId, MetaDataPtr) (SoConId) = (MetaDataPtr)[0u]
#else
#if (LOW_BYTE_FIRST == CPU_BYTE_ORDER)
#define SOAD_METADATATOSOCONID(SoConId, MetaDataPtr)  (  \
        (SoConId) = (SoAd_SoConIdType)((MetaDataPtr)[0u]) | \
        (SoAd_SoConIdType)(MetaDataPtr[1u]) << 8u)      \
        )
#else
#define SOAD_METADATATOSOCONID(SoConId, MetaDataPtr)  (  \
        (SoConId) = (SoAd_SoConIdType)((MetaDataPtr)[1u]) | \
        (SoAd_SoConIdType)((MetaDataPtr)[0u]) << 8u)      \
        )
#endif
#endif
/*==============================================================================
*                                      LOCAL CONSTANTS
==============================================================================*/

/*==============================================================================
*                                      LOCAL VARIABLES
==============================================================================*/
#define SOAD_START_SEC_VAR_INIT_PTR
#include "SoAd_MemMap.h"
static P2CONST(SoAd_ConfigType, AUTOMATIC, SOAD_APPL_CONST) SoAd_PbCfgPtr = NULL_PTR;
#define SOAD_STOP_SEC_VAR_INIT_PTR
#include "SoAd_MemMap.h"

#define SOAD_START_SEC_VAR_INIT_8
#include "SoAd_MemMap.h"
static VAR(SoAd_Module_StateType, SOAD_VAR_POWER_ON_INIT) SoAd_Module_Status = SOAD_STATE_UNINIT;
#define SOAD_STOP_SEC_VAR_INIT_8
#include "SoAd_MemMap.h"

#define SOAD_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "SoAd_MemMap.h"
static VAR(SoAd_InnerSocketManageType, SOAD_VAR) SoAd_SoConManager[SOAD_SO_CON_MAX];
#define SOAD_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "SoAd_MemMap.h"

#define SOAD_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "SoAd_MemMap.h"
static VAR(SoAd_InnerSocketGroupManageType, SOAD_VAR) SoAd_SoConGroupManager[SOAD_SO_CON_GROUP_MAX];
#define SOAD_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "SoAd_MemMap.h"

#if (0u < SOAD_ROUTING_GROUP_MAX)
#define SOAD_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "SoAd_MemMap.h"
static VAR(SoAd_RoutingGroupInnerManageType, SOAD_VAR) SoAd_RoutGroupManager[SOAD_ROUTING_GROUP_MAX];
#define SOAD_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "SoAd_MemMap.h"
#endif /*0u < SOAD_ROUTING_GROUP_MAX*/

#if (0u < SOAD_PDU_ROUTE_DEST_MAX)
#define SOAD_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "SoAd_MemMap.h"
static VAR(SoAd_PduRouteDestInnerManagerType, SOAD_VAR) SoAd_PduRouteDestManager[SOAD_PDU_ROUTE_DEST_MAX];
#define SOAD_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "SoAd_MemMap.h"
#endif /* 0u< SOAD_PDU_ROUTE_DEST_MAX */

#if (0u < SOAD_SOCKET_ROUTE_DEST_MAX)
#define SOAD_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "SoAd_MemMap.h"
static VAR(SoAd_SocketRouteDestInnerManagerType, SOAD_VAR) SoAd_SocketRouteDestManager[SOAD_SOCKET_ROUTE_DEST_MAX];
#define SOAD_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "SoAd_MemMap.h"
#endif /* 0u < SOAD_SOCKET_ROUTE_DEST_MAX */

#if (0u < SOAD_PDU_ROUTE_MAX)
#define SOAD_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "SoAd_MemMap.h"
static VAR(SoAd_PduRoutInnerManageType, SOAD_VAR) SoAd_PduRoutManager[SOAD_PDU_ROUTE_MAX];
#define SOAD_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "SoAd_MemMap.h"
#endif /* 0u < SOAD_PDU_ROUTE_MAX */

#if (0u < SOAD_MAX_BUFFER_SIZE)
#define SOAD_START_SEC_VAR_NO_INIT_8
#include "SoAd_MemMap.h"
static VAR(uint8, SOAD_VAR) SoAd_PduBuffer[SOAD_MAX_BUFFER_SIZE];
#define SOAD_STOP_SEC_VAR_NO_INIT_8
#include "SoAd_MemMap.h"
#endif /* 0u < SOAD_MAX_BUFFER_SIZE */

#if (0u < SOAD_MAX_IF_PDU_SIZE)
#define SOAD_START_SEC_VAR_NO_INIT_8
#include "SoAd_MemMap.h"
static VAR(uint8, SOAD_VAR) SoAd_IfPduBuffer[SOAD_MAX_IF_PDU_SIZE];
#define SOAD_STOP_SEC_VAR_NO_INIT_8
#include "SoAd_MemMap.h"
#endif /* 0u < SOAD_MAX_IF_PDU_SIZE */

#if (0u < SOAD_TXBUFFER_NUM)
/* socket link buffer management data */
#define SOAD_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "SoAd_MemMap.h"
static VAR(SoAd_SoTxBufManType, SOAD_VAR) SoAd_SoTxBufMan[SOAD_TXBUFFER_NUM];
#define SOAD_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "SoAd_MemMap.h"
#endif /* 0U< SOAD_TXBUFFER_NUM */

#if (0u < SOAD_RXBUFFER_NUM)
/* socket link buffer management data */
#define SOAD_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "SoAd_MemMap.h"
static VAR(SoAd_SoRxBufferManType, SOAD_VAR) SoAd_SoRxBufMan[SOAD_RXBUFFER_NUM];
#define SOAD_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "SoAd_MemMap.h"
#endif /* 0u < SOAD_RXBUFFER_NUM */
/*==============================================================================
*                                      GLOBAL CONSTANTS
==============================================================================*/

/*==============================================================================
*                                      GLOBAL VARIABLES
==============================================================================*/

/*==============================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==============================================================================*/
#define SOAD_START_SEC_CODE
#include "SoAd_MemMap.h"
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
static FUNC(void, SOAD_CODE)
    SoAd_MetaDataFillHandle(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId, P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) DataPtr);
#endif /* STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT */

static FUNC(void, SOAD_CODE) SoAd_SocnTransmitAutomaticSoConSetupHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(TcpIp_ProtocolType, AUTOMATIC) Protocol);

static FUNC(void, SOAD_CODE)
    SoAd_OpenTcpServerSoConHandle(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId, VAR(SoAd_uintx, AUTOMATIC) SoConGroup);

static FUNC(void, SOAD_CODE)
    SoAd_OpenTcpClientSoConHandle(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId, VAR(SoAd_uintx, AUTOMATIC) SoConGroup);

static FUNC(void, SOAD_CODE) SoAd_MainFunctionTxConfirmationHandle(void);

#if (STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER)
static FUNC(void, SOAD_CODE) SoAd_MainFunctionNPduUdpTxHandle(SoAd_SoConIdType SoConId);
#endif

#if (0u < SOAD_MAX_IF_PDU_SIZE)
static FUNC(Std_ReturnType, SOAD_CODE) SoAd_GetIfPduData(PduIdType TxPduId);
#endif

#if (STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER)
static FUNC(Std_ReturnType, SOAD_CODE)
    SoAd_NPduUdpTransmitHandle(PduIdType TxPduId, SoAd_uintx PduRouteDestId, SoAd_SoConIdType SoConId);
#endif

#if (0u < SOAD_MAX_IF_PDU_SIZE)
static FUNC(Std_ReturnType, SOAD_CODE)
    SoAd_IfTransmitSoCon(PduIdType TxPduId, SoAd_uintx PduRouteDestId, SoAd_SoConIdType SoConId);
#endif

#if (0u < SOAD_TXBUFFER_NUM)
static FUNC(void, SOAD_CODE) SoAd_TpPduTxErrorHandle(
    SoAd_SoConIdType SoConId,
    SoAd_uintx TxBufferId,
    SoAd_UpLayerModuleType UpModule,
    PduIdType UpPduId);
#endif

static FUNC(boolean, SOAD_CODE) SoAd_SoConRemoteAddrLocked(SoAd_SoConIdType SoConId);

#if (0u < SOAD_TXBUFFER_NUM)
static FUNC(void, SOAD_CODE) SoAd_MainFunctionTpTransmitHandle(SoAd_SoConIdType SoConId);
#endif

static FUNC(void, SOAD_CODE) SoAd_ReleaseRemoteAddrHandle(SoAd_SoConIdType SoConId, SoAd_uintx SoConGroupId);
static FUNC(void, SOAD_CODE) SoAd_PduRouteManagerInit(PduIdType TxPduId);

static FUNC(void, SOAD_CODE) SoAd_ReleaseRemoteAddrMainFunctionHandle(SoAd_SoConIdType SoConId);

#if (0u < SOAD_TXBUFFER_NUM)
static FUNC(void, SOAD_CODE) SoAd_TxBufferInit(SoAd_uintx TxBufferId);
#endif

#if (0u < SOAD_RXBUFFER_NUM)
static FUNC(void, SOAD_CODE) SoAd_RxBufferInit(SoAd_uintx RxBufferId);
#endif

#if (0u < SOAD_SOCKET_ROUTE_DEST_MAX)
#if (0u < SOAD_RXBUFFER_NUM)
#if (STD_ON == SOAD_SUPPORT_HEADERID)
static FUNC(void, SOAD_CODE)
    SoAd_HeaderEnableNewPduHandle(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId, VAR(SoAd_uintx, AUTOMATIC) BufferManageId);

static FUNC(void, SOAD_CODE)
    SoAd_HeaderEnableOldPduHandle(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId, VAR(SoAd_uintx, AUTOMATIC) BufferManageId);

static FUNC(void, SOAD_CODE)
    SoAd_HeaderEnableTpPduHandle(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId, VAR(PduLengthType, AUTOMATIC) UpBufferSize);
#endif /*STD_ON == SOAD_SUPPORT_HEADERID*/

#if (0u < SOAD_MAX_IF_PDU_SIZE)
#if (STD_ON == SOAD_SUPPORT_HEADERID)
static FUNC(void, SOAD_CODE)
    SoAd_HeaderEnableIfPduHandle(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId, VAR(SoAd_uintx, AUTOMATIC) BufferManageId);
#endif /*STD_ON == SOAD_SUPPORT_HEADERID*/
#endif /*0u < SOAD_MAX_IF_PDU_SIZE*/
#endif /*0u < SOAD_RXBUFFER_NUM*/

#if (0u < SOAD_RXBUFFER_NUM)
static FUNC(void, SOAD_CODE) SoAd_RxDataMainFunctionHeaderDisableHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(SoAd_uintx, AUTOMATIC) BufferManageId);

static FUNC(void, SOAD_CODE) SoAd_SoConRxDataMainFunctionHandle(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId);
#endif /*0u < SOAD_RXBUFFER_NUM*/

#if (STD_ON == SOAD_SUPPORT_UDP)
#if (STD_ON == SOAD_SUPPORT_HEADERID)
#if (0u < SOAD_RXBUFFER_NUM)
static FUNC(boolean, SOAD_CODE) SoAd_HeaderEnableUdpSoConRxTpPduHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(SoAd_uintx, AUTOMATIC) SocketRouteId,
    P2VAR(PduLengthType, AUTOMATIC, SOAD_APPL_DATA) DataPosition,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) DataPtr);
#endif /*0u < SOAD_RXBUFFER_NUM*/

static FUNC(boolean, SOAD_CODE) SoAd_HeaderEnableUdpSoConRxHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(uint16, AUTOMATIC) RxMessageLength,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) DataPtr);
#endif /*STD_ON == SOAD_SUPPORT_HEADERID*/
#endif /* STD_ON == SOAD_SUPPORT_UDP */

#if (0u < SOAD_RXBUFFER_NUM)
#if (STD_ON == SOAD_SUPPORT_HEADERID)
static FUNC(void, SOAD_CODE) SoAd_HeaderEnableTcpSoConRxTpPduHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(SoAd_uintx, AUTOMATIC) SocketRouteId,
    VAR(PduLengthType, AUTOMATIC) RxMessageLength,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) DataPtr);

static FUNC(void, SOAD_CODE)
    SoAd_RxFullTpPduHandle(P2CONST(SoAd_RxFullTpPduHandleType, AUTOMATIC, SOAD_APPL_DATA) Parameter);
#endif /*STD_ON == SOAD_SUPPORT_HEADERID*/

static FUNC(void, SOAD_CODE) SoAd_HeaderDisableSoConRxTpPduHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(SoAd_uintx, AUTOMATIC) SocketRouteId,
    VAR(PduLengthType, AUTOMATIC) RxMessageLength,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) DataPtr);

#if (STD_ON == SOAD_SUPPORT_HEADERID)
static FUNC(void, SOAD_CODE) SoAd_HeaderEnableTcpSoConRxHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(PduLengthType, AUTOMATIC) RxMessageLength,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) DataPtr);
#endif /*STD_ON == SOAD_SUPPORT_HEADERID*/

/*copy data to SoAd Rx Buffer*/
static FUNC(boolean, SOAD_CODE) SoAd_CopyDataToRxBuffer(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(PduLengthType, AUTOMATIC) RxMessageLength,
    P2CONST(uint8, AUTOMATIC, SOAD_APPL_DATA) DataPtr);
#endif /*0u < SOAD_RXBUFFER_NUM*/

static FUNC(void, SOAD_CODE) SoAd_HeaderDisableSoConRxIfPduHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(SoAd_uintx, AUTOMATIC) SocketRouteId,
    VAR(PduLengthType, AUTOMATIC) RxMessageLength,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) DataPtr);
#endif /*0u < SOAD_SOCKET_ROUTE_DEST_MAX*/

#if (0u < SOAD_SOCKET_ROUTE_DEST_MAX)
static FUNC(boolean, SOAD_CODE) SoAd_FindTheValidSoCon(
    VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_CONST) RemoteAddrPtr,
#if (STD_ON == SOAD_SUPPORT_UDP)
    VAR(SoAd_FindTheValidSoConType, AUTOMATIC) Parameter,
#endif
    P2VAR(SoAd_SoConIdType, AUTOMATIC, SOAD_APPL_DATA) SoConId);
#endif /*0u < SOAD_SOCKET_ROUTE_DEST_MAX*/

#if (STD_ON == SOAD_SUPPORT_UDP)
#if (STD_ON == SOAD_SUPPORT_HEADERID)
static FUNC(boolean, SOAD_CODE) SoAd_UdpStrictHeaderLenCheck(
    VAR(SoAd_uintx, AUTOMATIC) SoConGroupId,
    P2CONST(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
    VAR(uint16, AUTOMATIC) Length);
#endif /*STD_ON == SOAD_SUPPORT_HEADERID*/

static FUNC(void, SOAD_CODE) SoAd_UdpAliveSupervisionTimeHandle(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId);
#endif /* STD_ON == SOAD_SUPPORT_UDP */

static FUNC(boolean, SOAD_CODE) SoAd_GetBestMatchAlgorithmSoCon(
    VAR(SoAd_uintx, AUTOMATIC) SoConGroupId,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_CONST) RemoteAddrPtr,
    P2VAR(SoAd_SoConIdType, AUTOMATIC, SOAD_APPL_CONST) SoConId,
    VAR(SoAd_BestMatchStateType, AUTOMATIC) CheckSoConState);

static FUNC(void, SOAD_CODE) SoAd_ChangeSocketParameter(
#if (STD_ON == SOAD_SUPPORT_TCP)
    VAR(boolean, AUTOMATIC) SocketProtocolIsTcp,
#endif
    VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId,
    VAR(SoAd_uintx, AUTOMATIC) SoConGroup);

#if (STD_ON == SOAD_SUPPORT_UDP)
static FUNC(void, SOAD_CODE)
    SoAd_OpenUdpSoConHandle(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId, VAR(SoAd_uintx, AUTOMATIC) SoConGroup);
#endif /* STD_ON == SOAD_SUPPORT_UDP */

static FUNC(void, SOAD_CODE) SoAd_SoConModeChgNotificationAndTpStartOfReception(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(SoAd_uintx, AUTOMATIC) SoConGroup);

static FUNC(void, SOAD_CODE)
    SoAd_SoConLocalIpAddrAssignmentChg(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId, VAR(SoAd_uintx, AUTOMATIC) SoConGroup);

static FUNC(void, SOAD_CODE) SoAd_SoConModeChangeHandle(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId);

#if (0u < SOAD_RXBUFFER_NUM)
static FUNC(void, SOAD_CODE) SoAd_CloseRxTpPduHandle(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId);
static FUNC(boolean, SOAD_CODE) SoAd_CheckRxTpPduDataHandle(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId);
#endif

static FUNC(void, SOAD_CODE) SoAd_SoConCloseHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(SoAd_uintx, AUTOMATIC) SoConGroup,
    VAR(boolean, AUTOMATIC) CloseSocket);

static FUNC(void, SOAD_CODE) SoAd_DisconnectionAndRecoveryHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(SoAd_uintx, AUTOMATIC) SoConGroup);

/* routing group init  and calculte SoAdSocketRouteDest ,SoAdPduRouteDest
routing control variable*/
static FUNC(void, SOAD_CODE) SoAd_RoutingGroupInit(void);

/* id: routing group identifier specifying the routing group
enFlag: group control enable shall be set TRUE otherwise is FALSE
soConIdPtr:socket connection index specifying the socket connection on
which the routing group  */
#if (0u < SOAD_ROUTING_GROUP_MAX)
static FUNC(Std_ReturnType, SOAD_CODE) SoAd_RoutingGroupControl(
    VAR(SoAd_RoutingGroupIdType, AUTOMATIC) id,
    VAR(boolean, AUTOMATIC) enFlag,
    P2CONST(SoAd_SoConIdType, AUTOMATIC, SOAD_APPL_DATA) soConIdPtr);
#endif /*0u < SOAD_ROUTING_GROUP_MAX*/

/* TpTransmit to specification socket connection id */
#if (0u < SOAD_TXBUFFER_NUM)
static FUNC(Std_ReturnType, SOAD_CODE) SoAd_TpTransmitSoCon(
    SoAd_SoConIdType SoConId,
    SoAd_uintx PduRouteDestId,
    P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_CONST) SoAdSrcPduInfoPtr);
#endif /*0u < SOAD_TXBUFFER_NUM*/

/*Called by SoAd_TcpIpEvent.
 *Close the Udp SoCon Group*/
#if (STD_ON == SOAD_SUPPORT_UDP)
static FUNC(void, SOAD_CODE)
    SoAd_CloseUdpSoConGroup(VAR(TcpIp_EventType, AUTOMATIC) Event, VAR(SoAd_uintx, AUTOMATIC) SoConGroupId);
#endif

/*Called by SoAd_TxConfirmation.
 *Tcp TxConfirmation handle*/
#if (STD_ON == SOAD_SUPPORT_TCP)
static FUNC(void, SOAD_CODE) SoAd_TxConfirmationHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) soConId,
    VAR(uint16, AUTOMATIC) Length,
    VAR(SoAd_uintx, AUTOMATIC) pduRouteDestId);
#endif

#if (STD_ON == SOAD_SUPPORT_HEADERID)
static boolean SoAd_FindHeaderIDAndSocketRouteId(SoAd_SoConIdType SoConId, uint32 HeaderId, SoAd_uintx* SocketRouteID);
#endif /* STD_ON == SOAD_SUPPORT_HEADERID */

static void SoAd_PduRouteDestControl(
    SoAd_PduRouteDestInnerManagerType* pduRouteDestMPtr,
    SoAd_uintx pduRouteDestId,
    boolean enFlag,
    SoAd_SoConIdType specificSoConId);
static boolean SoAd_PduRouteDestIsEnable(SoAd_uintx pduRouteDestId, SoAd_SoConIdType soConId);
#define SOAD_STOP_SEC_CODE
#include "SoAd_MemMap.h"
/*===============================================================================
*                                       GLOBAL FUNCTIONS
==============================================================================*/
#define SOAD_START_SEC_CODE
#include "SoAd_MemMap.h"
/*============================================================================*/
/*
Service ID[hex]: 0x01
Sync/Async: Synchronous
Reentrancy: Non Reentrant
Parameters (in): SoAdConfigPtr Pointer to the configuration data of the SoAd module.
Parameters (inout):None
Parameters (out): None
Return value: None
Description: Initializes the Socket Adaptor.
*/
FUNC(void, SOAD_CODE)
SoAd_Init(P2CONST(SoAd_ConfigType, AUTOMATIC, SOAD_APPL_CONST) SoAdConfigPtr)
{
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /*SWS_SoAd_00216*/
    if (NULL_PTR == SoAdConfigPtr)
    {
        SOAD_DET_REPORT(SOAD_SID_INIT, SOAD_E_INIT_FAILED);
    }
    else
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        SoAd_uintx cnt;
        SoAd_uintx soConGroupId;
        SoAd_InnerSocketManageType* soadSoConMPtr;
        SoAd_InnerSocketGroupManageType* soadSoConGroupMPtr;

        /*SWS_SoAd_00211*/
        SoAd_PbCfgPtr = SoAdConfigPtr;
        /*SWS_SoAd_00723*/
        for (cnt = 0u; cnt < SOAD_SOCON_NUM; cnt++)
        {
            soadSoConMPtr = &SoAd_SoConManager[cnt];
            soadSoConMPtr->SoAd_SoConMode = SOAD_SOCON_OFFLINE;
            soadSoConMPtr->ChangeModeToOffline = FALSE;
#if (STD_ON == SOAD_SUPPORT_UDP)
            soadSoConMPtr->UdpAliveSupervisionTime = 0u;
#endif /* STD_ON == SOAD_SUPPORT_UDP */
            /*init the socket connection's remote address to configuration parameter*/
            soConGroupId = SOAD_SOCON_CFG(cnt).SoAd_SocketConnectionGroupRef;
            soadSoConMPtr->RemoteAddr.domain = SOAD_SOCKETLOCALADDR_CFG(soConGroupId).DomainType;
            if (NULL_PTR == SOAD_SOCON_CFG(cnt).SoAd_SocketRemoteAddress)
            {
                soadSoConMPtr->RemoteAddrIsSet = FALSE;
                TCPIP_SET_IPADDR_ANY(soadSoConMPtr->RemoteAddr); /* PRQA S 3469 */ /* MISRA Dir-4.9 */
                soadSoConMPtr->RemoteAddr.port = TCPIP_PORT_ANY;
            }
            else
            {
                soadSoConMPtr->RemoteAddrIsSet = TRUE;
                soadSoConMPtr->RemoteAddr = *SOAD_SOCONREMOTEADDR_CFG(cnt);
            }
            /*init socket connection's Open/Close request*/
            if (SOAD_SOCONGROUP_CFG(soConGroupId).SoAd_SocketAutomaticSoConSetup)
            {
                soadSoConMPtr->SoCon_Request[0] = SOAD_SOCON_REQUEST_OPEN;
            }
            else
            {
                soadSoConMPtr->SoCon_Request[0] = SOAD_SOCON_REQUEST_NONE;
            }
            soadSoConMPtr->SoCon_Request[1] = SOAD_SOCON_REQUEST_NONE;
            /*other parameter init of SoAd_SoConManager*/
            soadSoConMPtr->ActiveSoAdPduRouteId = SOAD_UNUSED_UINT16;
            soadSoConMPtr->SoConCloseAbort = FALSE;
            soadSoConMPtr->SocketNeedClose = SOAD_SOCKET_CLOSE_NONE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
            soadSoConMPtr->SetSocketNeedCloseLine = __LINE__;
#endif /* STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG */
            soadSoConMPtr->TcpIpSocketValid = FALSE;
            soadSoConMPtr->TcpIpSocketId = (TcpIp_SocketIdType)SOAD_UNUSED_UINT16;
            soadSoConMPtr->TpPduTransmitting = FALSE;
            soadSoConMPtr->TxNoAckLength = 0u;
            soadSoConMPtr->RemoteAddrNeedRelease = FALSE;
            soadSoConMPtr->RxIfProcessing = FALSE;
        }
        /*init SoAd_SoConGroupManage parameter*/
        for (cnt = 0u; cnt < SOAD_SO_CON_GROUP_NUM; cnt++)
        {
            soadSoConGroupMPtr = &SoAd_SoConGroupManager[cnt];
            soadSoConGroupMPtr->LocalIpAddrState = TCPIP_IPADDR_STATE_UNASSIGNED;
            soadSoConGroupMPtr->SoConSocketValidNum = 0u;
            soadSoConGroupMPtr->TcpIpSocketId = (TcpIp_SocketIdType)SOAD_UNUSED_UINT16;
            soadSoConGroupMPtr->TcpIpSocketValid = FALSE;
        }
        /* routing group init */
        SoAd_RoutingGroupInit();
        /* socket connection link buffer management data init */
#if (0u < SOAD_TXBUFFER_NUM)
        for (cnt = 0u; cnt < SOAD_TXBUFFER_NUM; cnt++)
        {
            SoAd_TxBufferInit(cnt);
        }
#endif /* 0u < SOAD_TXBUFFER_NUM */
#if (0u < SOAD_RXBUFFER_NUM)
        for (cnt = 0u; cnt < SOAD_RXBUFFER_NUM; cnt++)
        {
            SoAd_RxBufferInit(cnt);
        }
#endif /* 0u < SOAD_RXBUFFER_NUM */
#if (0u < SOAD_PDU_ROUTE_MAX)
        for (cnt = 0u; cnt < SOAD_PDU_ROUTE_NUM; cnt++)
        {
            SoAd_PduRouteManagerInit(cnt);
        }
#endif /* 0u < SOAD_PDU_ROUTE_MAX */
        SoAd_Module_Status = SOAD_STATE_INIT;
    }
    return;
}
/*============================================================================*/
/*
Service ID[hex]: 0x07
Sync/Async: Synchronous
Reentrancy: Reentrant
Parameters (in): TxPduId Transmit PduId specifying the SoAd socket connection for which
the socket connection index shall be returned.
Parameters (inout): None
Parameters (out): SoConIdPtr Pointer to memory receiving the socket connection index asked
for.
Return value: Std_ReturnType Result of operation
E_OK The request was successful
E_NOT_OK The request was not successful
Description: Returns socket connection index related to the specified TxPduId.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_GetSoConId(VAR(PduIdType, AUTOMATIC) TxPduId, P2VAR(SoAd_SoConIdType, AUTOMATIC, SOAD_APPL_DATA) SoConIdPtr)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00609*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_GETSOCONID, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00610*/
    if ((TRUE == detNoErr) && (TxPduId >= SOAD_PDU_ROUTE_NUM))
    {
        SOAD_DET_REPORT(SOAD_SID_GETSOCONID, SOAD_E_INV_PDUID);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        /*SWS_SoAd_00724*/
        if (1u == SOAD_PDUROUTE_CFG(TxPduId).SoAd_PduRouteDestNum)
        {
            SoAd_uintx pduRouteDestId;
            pduRouteDestId = SOAD_PDUROUTE_CFG(TxPduId).SoAd_PduRouteDestIndex[0u];
            *SoConIdPtr = SOAD_PDUROTDEST_CFG(pduRouteDestId).SoAd_TxSocketConRefIndexPtr[0];
            result = E_OK;
        }
    }
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x02
Sync/Async: Synchronous
Reentrancy: Reentrant
Parameters (in): None
Parameters (inout): None
Parameters (out): versioninfo Pointer to where to store the version information of this module.
Return value: None
Description: Returns the version information.
 */
#if (STD_ON == SOAD_VERSION_INFO_API)
FUNC(void, SOAD_CODE)
SoAd_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, SOAD_APPL_DATA) versioninfo)
{
#if (STD_ON == SOAD_DEV_ERROR_DETECT)

    if (NULL_PTR == versioninfo)
    {
        SOAD_DET_REPORT(SOAD_SID_GETVERSIONINFO, SOAD_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        versioninfo->moduleID = (VAR(uint16, AUTOMATIC))SOAD_MODULE_ID;
        versioninfo->vendorID = (VAR(uint16, AUTOMATIC))SOAD_VENDOR_ID;
        versioninfo->sw_major_version = (VAR(uint8, AUTOMATIC))SOAD_C_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = (VAR(uint8, AUTOMATIC))SOAD_C_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = (VAR(uint8, AUTOMATIC))SOAD_C_SW_PATCH_VERSION;
    }

    return;
}
#endif /*STD_ON == SOAD_VERSION_INFO_API*/
/*============================================================================*/
/*
Service ID[hex]: 0x03
Sync/Async: Synchronous
Reentrancy: Reentrant for different PduIds. Non reentrant for the same PduId.
Parameters (in): SoAdSrcPduId This parameter contains a unique identifier referencing to the
PDU Routing Table and thereby specifiying the socket to be
used for tranmission of the data.
SoAdSrcPduInfoPtr A pointer to a structure with socket related data: data length
and pointer to a data buffer.
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType E_OK: The request has been accepted
E_NOT_OK: The request has not been accepted, e.g. due to a
still ongoing transmission in the corresponding socket or the to
be transmitted message is too long.
Description: Requests transmission of an I-PDU.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_IfTransmit(
    VAR(PduIdType, AUTOMATIC) SoAdSrcPduId,
    P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_CONST) SoAdSrcPduInfoPtr)
{
    Std_ReturnType result = E_NOT_OK;
#if (0u < SOAD_MAX_IF_PDU_SIZE)
#if (STD_ON == SOAD_DEV_ERROR_DETECT)

    /*SWS_SoAd_00213*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_IFTRANSMIT, SOAD_E_NOTINIT);
    }
    /*SWS_SoAd_00214*/
    else if (((SoAdSrcPduId >= SOAD_PDU_ROUTE_NUM)
              || (SOAD_UPPER_LAYER_IF != SOAD_PDUROUTE_CFG(SoAdSrcPduId).SoAd_TxUpperLayerType)))
    {
        SOAD_DET_REPORT(SOAD_SID_IFTRANSMIT, SOAD_E_INV_PDUID);
    }
    else
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        SoAd_PduRoutInnerManageType* soadPduRoutMPtr = &SoAd_PduRoutManager[SoAdSrcPduId];

        /*the TxPdu is transmitting,refuse the new transmit request*/
        if (SOAD_UNUSED_UINT8 == soadPduRoutMPtr->TxPendingNum)
        {
            /*1.get pdu data from up module*/
            if ((NULL_PTR == SoAdSrcPduInfoPtr) || (NULL_PTR == SoAdSrcPduInfoPtr->SduDataPtr))
            {
                result = SoAd_GetIfPduData(SoAdSrcPduId);
            }
            else
            {
                if (SoAdSrcPduInfoPtr->SduLength <= (SOAD_MAX_IF_PDU_SIZE - SOAD_HEADER_SIZE))
                {
                    SchM_Enter_SoAd_ExclusiveArea();
                    (void)ILib_memcpy(
                        &SoAd_IfPduBuffer[SOAD_HEADER_SIZE],
                        SoAdSrcPduInfoPtr->SduDataPtr,
                        SoAdSrcPduInfoPtr->SduLength);
                    SchM_Exit_SoAd_ExclusiveArea();
                    soadPduRoutMPtr->PduDataPtr = &SoAd_IfPduBuffer[SOAD_HEADER_SIZE];
                    soadPduRoutMPtr->PduLength = SoAdSrcPduInfoPtr->SduLength;
#if (STD_ON == SOAD_PDU_ROUTE_METADATA_SUPPORT)
                    if (SOAD_PDUROUT_CFG(SoAdSrcPduId).SoAd_MetaDataEnable
                        && (NULL_PTR != SoAdSrcPduInfoPtr->MetaDataPtr))
                    {
                        SOAD_METADATATOSOCONID(soadPduRoutMPtr->TxSoConId, SoAdSrcPduInfoPtr->MetaDataPtr);
                    }
                    else
#endif /* STD_ON == SOAD_PDU_ROUT_METADATA_SUPPORT */
                    {
                        soadPduRoutMPtr->TxSoConId = NO_TXMETADATA;
                    }
                    result = E_OK;
                }
            }
            if (E_OK == result)
            {
                /*2.check the PduRouteDest/SoCon transmit condition and transmit the pdu on related SoCon(s)*/
                SoAd_uintx cnt, pduRouteDestNum, pduRouteDestId;
                SoAd_uintx txSuccessCnt = 0u;
                SoAd_uintx txFailCnt = 0u;
                const SoAd_InnerSocketManageType* soadSoConMPtr;
                SoAd_SoConIdType soceketConNum;

                pduRouteDestNum = SOAD_PDUROUT_CFG(SoAdSrcPduId).SoAd_PduRouteDestNum;
                for (cnt = 0u; (cnt < pduRouteDestNum) && (0u == txFailCnt); cnt++)
                {
                    pduRouteDestId = SOAD_PDUROT_DESTINDEX_CFG(SoAdSrcPduId, cnt);
                    soceketConNum = SOAD_PDUROTDEST_CFG(pduRouteDestId).SoAd_TxPduRouteDestSocketConRefNum;
                    /* SWS_SoAd_00722 */
                    for (SoAd_SoConIdType soConCnt = 0u; (soConCnt < soceketConNum) && (0u == txFailCnt); soConCnt++)
                    {
                        SoAd_SoConIdType soConId =
                            SOAD_PDUROTDEST_CFG(pduRouteDestId).SoAd_TxSocketConRefIndexPtr[soConCnt];

                        if (SoAd_PduRouteDestIsEnable(pduRouteDestId, soConId))
                        {
                            boolean tpPduTcpTransmitting = FALSE;

                            soadSoConMPtr = &SoAd_SoConManager[soConId];
                            if ((TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(soConId).SoAd_TcpUdpProtocol)
                                && soadSoConMPtr->TpPduTransmitting)
                            {
                                tpPduTcpTransmitting = TRUE;
                            }
                            if (((NO_TXMETADATA == soadPduRoutMPtr->TxSoConId)
                                 || (soConId == soadPduRoutMPtr->TxSoConId))
                                && ((SOAD_SOCON_ONLINE == soadSoConMPtr->SoAd_SoConMode)
                                    && (SOAD_SOCKET_CLOSE_NONE == soadSoConMPtr->SocketNeedClose)
                                    && (!tpPduTcpTransmitting)))
                            {
                                if (E_OK == SoAd_IfTransmitSoCon(SoAdSrcPduId, pduRouteDestId, soConId))
                                {
                                    txSuccessCnt++;
                                }
                                else
                                {
                                    txFailCnt++;
                                }
                            }
                        }
                    }
                }

                if ((txSuccessCnt > 0u) && (0u == txFailCnt))
                {
                    result = E_OK;
                }
                else
                {
                    result = E_NOT_OK;
                }
            }
        }
    }
#endif /*0u < SOAD_MAX_IF_PDU_SIZE*/
    SOAD_NOUSED(SoAdSrcPduId);
    SOAD_NOUSED(SoAdSrcPduInfoPtr);
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x1D
Sync/Async: Asynchronous
Reentrancy: Reentrant
Parameters (in): id routing group identifier indirectly specifying PDUs to be
transmitted (after requesting the newest data from the related upper layer).
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType Result of operation
E_OK The request was successful
E_NOT_OK The request was not successful.
Description: Triggers the transmission of all If-TxPDUs identified by the parameter id after
requesting the data from the related upper layer.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_IfRoutingGroupTransmit(VAR(SoAd_RoutingGroupIdType, AUTOMATIC) id)
{
    Std_ReturnType result = E_NOT_OK;
#if ((0u < SOAD_ROUTING_GROUP_MAX) && (0u < SOAD_MAX_IF_PDU_SIZE))
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00661*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_IFROUTINGGROUPTRANSMIT, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00658*/
    if ((TRUE == detNoErr) && ((id >= SOAD_ROUTING_GROUP_NUM) || (TRUE != SOAD_ROUTGROUP_TXTRIG(id))))
    {
        SOAD_DET_REPORT(SOAD_SID_IFROUTINGGROUPTRANSMIT, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        SoAd_uintx cnt, pduRouteDestId, pduRouteDestNum;
        PduIdType txPduId = 0xFFFFu;
        const SoAd_PduRoutInnerManageType* soadPduRoutMPtr;
        const SoAd_InnerSocketManageType* soadSoConMPtr;
        const SoAd_RoutingGroupType* soadRoutGroupPtr = &SOAD_ROUTGROUP_CFG(id);

        pduRouteDestNum = soadRoutGroupPtr->SoAd_PduRouteDestNum;
        SchM_Enter_SoAd_ExclusiveArea();
        for (cnt = 0u; cnt < pduRouteDestNum; cnt++)
        {
            pduRouteDestId = soadRoutGroupPtr->SoAd_PduRouteDestIndexPtr[cnt];
            PduIdType pduRouteId = SOAD_PDUROTDEST_CFG(pduRouteDestId).SoAd_PduRouteIndex;
            soadPduRoutMPtr = &SoAd_PduRoutManager[pduRouteId];

            if ((SOAD_UPPER_LAYER_IF == SOAD_PDUROUT_CFG(pduRouteId).SoAd_TxUpperLayerType)
                && (SOAD_UNUSED_UINT8 == soadPduRoutMPtr->TxPendingNum))
            {
                SoAd_SoConIdType socketConNum;
                SoAd_SoConIdType soConId;
                /* SWS_SoAd_00722 */
                socketConNum = SOAD_PDUROTDEST_CFG(pduRouteDestId).SoAd_TxPduRouteDestSocketConRefNum;
                for (SoAd_SoConIdType soConCnt = 0u; soConCnt < socketConNum; soConCnt++)
                {
                    soConId = SOAD_PDUROTDEST_CFG(pduRouteDestId).SoAd_TxSocketConRefIndexPtr[soConCnt];
                    if (SoAd_PduRouteDestIsEnable(pduRouteDestId, soConId))
                    {
                        boolean tpPduTcpTransmitting = FALSE;
                        soadSoConMPtr = &SoAd_SoConManager[soConId];
                        if ((TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(soConId).SoAd_TcpUdpProtocol)
                            && soadSoConMPtr->TpPduTransmitting)
                        {
                            tpPduTcpTransmitting = TRUE;
                        }
                        if ((SOAD_SOCON_ONLINE == soadSoConMPtr->SoAd_SoConMode)
                            && (SOAD_SOCKET_CLOSE_NONE == soadSoConMPtr->SocketNeedClose) && (!tpPduTcpTransmitting))
                        {
                            if (txPduId != pduRouteId)
                            {
                                (void)SoAd_GetIfPduData(pduRouteId);
                                txPduId = pduRouteId;
                            }
                            if ((NULL_PTR != soadPduRoutMPtr->PduDataPtr)
                                && ((NO_TXMETADATA == soadPduRoutMPtr->TxSoConId)
                                    || (soConId == soadPduRoutMPtr->TxSoConId)))
                            {
                                (void)SoAd_IfTransmitSoCon(pduRouteId, pduRouteDestId, soConId);
                            }
                        }
                    }
                }
            }
        }

        SchM_Exit_SoAd_ExclusiveArea();
        if (0xFFFFu != txPduId)
        {
            result = E_OK;
        }
    }
#endif /* 0u < SOAD_ROUTING_GROUP_MAX && 0u < SOAD_MAX_IF_PDU_SIZE */
    SOAD_NOUSED(id);
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x1f
Sync/Async: Asynchronous
Reentrancy: Reentrant
Parameters (in):id routing group identifier indirectly specifying PDUs to be
transmitted (after requesting the newest data from the related
upper layer).
SoConId socket connection index specifying the socket connection on
which the PDUs shall be transmitted
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType E_OK The request was successful.
E_NOT_OK The request was not successful.
Description: Triggers the transmission of all If-TxPDUs identified by the parameter id on the
socket connection specified by SoConId after requesting the data from the related
upper layer.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_IfSpecificRoutingGroupTransmit(
    VAR(SoAd_RoutingGroupIdType, AUTOMATIC) id,
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId)
{
    Std_ReturnType result = E_NOT_OK;
#if ((0u < SOAD_ROUTING_GROUP_MAX) && (0u < SOAD_MAX_IF_PDU_SIZE))
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00712*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_IFSPECIFICROUTINGGROUPTRANSMIT, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00713*/
    if ((TRUE == detNoErr)
        && ((id >= SOAD_ROUTING_GROUP_NUM) || (TRUE != SOAD_ROUTGROUP_TXTRIG(id)) || (SoConId >= SOAD_SOCON_NUM)))
    {
        SOAD_DET_REPORT(SOAD_SID_IFSPECIFICROUTINGGROUPTRANSMIT, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        SoAd_uintx cnt, pduRouteDestId, pduRouteDestNum;
        const SoAd_PduRoutInnerManageType* soadPduRoutMPtr;
        const SoAd_InnerSocketManageType* soadSoConMPtr;
        const SoAd_RoutingGroupType* soadRoutGroupPtr = &SOAD_ROUTGROUP_CFG(id);
        const SoAd_PduRouteDestType* soadPduRouteDestCfgPtr;

        pduRouteDestNum = soadRoutGroupPtr->SoAd_PduRouteDestNum;
        SchM_Enter_SoAd_ExclusiveArea();
        for (cnt = 0u; cnt < pduRouteDestNum; cnt++)
        {
            PduIdType pduRouteId;
            pduRouteDestId = soadRoutGroupPtr->SoAd_PduRouteDestIndexPtr[cnt];
            soadPduRouteDestCfgPtr = &SOAD_PDUROTDEST_CFG(pduRouteDestId);
            pduRouteId = soadPduRouteDestCfgPtr->SoAd_PduRouteIndex;
            soadPduRoutMPtr = &SoAd_PduRoutManager[pduRouteId];
            if ((SOAD_UPPER_LAYER_IF == SOAD_PDUROUT_CFG(pduRouteId).SoAd_TxUpperLayerType)
                && (SOAD_UNUSED_UINT8 == soadPduRoutMPtr->TxPendingNum)
                && SoAd_PduRouteDestIsEnable(pduRouteDestId, SoConId))
            {
                SoAd_SoConIdType soConId;
                boolean tpPduTcpTransmitting = FALSE;

                SoAd_SoConIdType refConNum = soadPduRouteDestCfgPtr->SoAd_TxPduRouteDestSocketConRefNum;
                boolean vaild = FALSE;
                for (SoAd_SoConIdType index = 0u; index < refConNum; index++)
                {
                    soConId = soadPduRouteDestCfgPtr->SoAd_TxSocketConRefIndexPtr[index];
                    if (SoConId == soConId)
                    {
                        vaild = TRUE;
                        break;
                    }
                }
                if (vaild)
                {
                    soadSoConMPtr = &SoAd_SoConManager[soConId];
                    if ((TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(soConId).SoAd_TcpUdpProtocol)
                        && (soadSoConMPtr->TpPduTransmitting))
                    {
                        tpPduTcpTransmitting = TRUE;
                    }
                    if ((SoConId == soConId) && (SOAD_SOCON_ONLINE == soadSoConMPtr->SoAd_SoConMode)
                        && (SOAD_SOCKET_CLOSE_NONE == soadSoConMPtr->SocketNeedClose) && (!tpPduTcpTransmitting))
                    {
                        (void)SoAd_GetIfPduData(pduRouteId);
                        if ((NULL_PTR != soadPduRoutMPtr->PduDataPtr)
                            && ((NO_TXMETADATA == soadPduRoutMPtr->TxSoConId)
                                || (soConId == soadPduRoutMPtr->TxSoConId)))
                        {
                            (void)SoAd_IfTransmitSoCon(pduRouteId, pduRouteDestId, soConId);
                        }
                        if (E_NOT_OK == result)
                        {
                            result = E_OK;
                        }
                    }
                }
            }
        }
        SchM_Exit_SoAd_ExclusiveArea();
    }
#endif /* 0u < SOAD_ROUTING_GROUP_MAX && 0u < SOAD_MAX_IF_PDU_SIZE */
    SOAD_NOUSED(id);
    SOAD_NOUSED(SoConId);
    return result;
}

/*============================================================================*/
/*
Service ID[hex]: 0x05
Sync/Async: Synchronous
Reentrancy: Reentrant for different PduIds. Non reentrant for the same PduId.
Parameters (in): PduId Identifiaction of the I-PDU to be cancelled.
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType Result of operation:
E_OK: request accepted (but not yet performed).
E_NOT_OK: request not accepted (e.g. cancellation not
possible).
Description: Requests cancellation of the transmission via TP for a specific I-PDU.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_TpCancelTransmit(VAR(PduIdType, AUTOMATIC) PduId)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00605*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_TPCANCELTRANSMIT, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00606*/
    if ((TRUE == detNoErr)
        && ((PduId >= SOAD_PDU_ROUTE_NUM) || (SOAD_UPPER_LAYER_TP != SOAD_PDUROUTE_CFG(PduId).SoAd_TxUpperLayerType)))
    {
        SOAD_DET_REPORT(SOAD_SID_TPCANCELTRANSMIT, SOAD_E_INV_PDUID);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
#if (0u < SOAD_TXBUFFER_NUM)
        SoAd_uintx pduRotDestIndex;
        SoAd_SoConIdType soConId;
        SoAd_InnerSocketManageType* soadSoConMPtr;

        /*TP PDU just reference one PduRouteDest(one SoCon),TP transmit just support 1:1*/
        pduRotDestIndex = SOAD_PDUROT_DESTINDEX_CFG(PduId, 0u);
        soConId = SOAD_PDUROTDEST_CFG(pduRotDestIndex).SoAd_TxSocketConRefIndexPtr[0];
        soadSoConMPtr = &SoAd_SoConManager[soConId];
        if (soadSoConMPtr->TpPduTransmitting && (PduId == soadSoConMPtr->ActiveSoAdPduRouteId))
        {
            result = E_OK;
            soadSoConMPtr->SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
            SoAd_SoConManager[soConId].SetSocketNeedCloseLine = __LINE__;
#endif /* STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG */
        }
#endif /*0u < SOAD_TXBUFFER_NUM*/
    }
    SOAD_NOUSED(PduId);
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x06
Sync/Async: Synchronous
Reentrancy: Reentrant for different PduIds. Non reentrant for the same PduId.
Parameters (in): PduId Identifiaction of the I-PDU to be cancelled.
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType Result of operation:
E_OK: request accepted (but not yet performed).
E_NOT_OK: request not accepted (e.g. cancellation not
possible).
Description: Requests cancellation of the reception via TP for a specific I-PDU.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_TpCancelReceive(VAR(PduIdType, AUTOMATIC) PduId)
{
    Std_ReturnType result = E_NOT_OK;
#if (0u < SOAD_RXBUFFER_NUM)
    SoAd_SoConIdType soConId;
    SoAd_uintx bufferId;
    SoAd_uintx soConGroupId;
    boolean headerEnabled;
#endif /*0u < SOAD_RXBUFFER_NUM*/
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00607*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_TPCANCELRECEIVE, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00608*/
    if ((TRUE == detNoErr)
        && ((PduId >= SOAD_SOCKET_ROUTE_DEST_NUM)
            || (SOAD_UPPER_LAYER_TP != SOAD_SRDEST_CFG(PduId).SoAd_RxUpperLayerType)))
    {
        SOAD_DET_REPORT(SOAD_SID_TPCANCELRECEIVE, SOAD_E_INV_PDUID);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
#if (0u < SOAD_RXBUFFER_NUM)
        SoAd_InnerSocketManageType* soadSoConMPtr;

        /*TP socket route can't reference SoConGroup*/
        soConId = *SOAD_SOCKETROUTE_CFG(PduId).SoAd_RxSocketConnRefIndexPtr;
        soadSoConMPtr = &SoAd_SoConManager[soConId];
        bufferId = SOAD_SOCON_CFG(soConId).SoAd_RxBufCfgIndexPtr[0u];
        bufferId = bufferId - SOAD_TXBUFFER_NUM;
        if (SOAD_SOCON_ONLINE == soadSoConMPtr->SoAd_SoConMode)
        {
            soConGroupId = SOAD_SOCON_CFG(soConId).SoAd_SocketConnectionGroupRef;
            headerEnabled = SOAD_SOCONGROUP_CFG(soConGroupId).SoAd_PduHeaderEnable;
            if (headerEnabled)
            {
                if ((PduId == SoAd_SoRxBufMan[bufferId].SoAd_SocketRouteId)
                    && (SoAd_SoRxBufMan[bufferId].ExitPduResidueLength > 0u))
                {
                    soadSoConMPtr->SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
                    SoAd_SoConManager[soConId].SetSocketNeedCloseLine = __LINE__;
#endif /* STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG */
                    result = E_OK;
                }
            }
            else
            {
                soadSoConMPtr->SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
                SoAd_SoConManager[soConId].SetSocketNeedCloseLine = __LINE__;
#endif /* STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG */
                result = E_OK;
            }
        }
#endif /*0u < SOAD_RXBUFFER_NUM*/
    }
    SOAD_NOUSED(PduId);
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x04
Sync/Async: Asynchronous
Reentrancy: Reentrant
Parameters (in):
SoAdSrcPduId This parameter contains a unique identifier referencing to the
PDU Routing Table and thereby specifiying the socket to be
used for tranmission of the data.
SoAdSrcPduInfoPtr A pointer to a structure with socket related data. Only the
length data is valid.
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType E_OK: The request has been accepted
E_NOT_OK: The request has not been accepted, e.g. due to a
still ongoing transmission in the corresponding socket or the to
be transmitted message is too long.
Description: Requests transmission of an I-PDU.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_TpTransmit(
    VAR(PduIdType, AUTOMATIC) SoAdSrcPduId,
    P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_CONST) SoAdSrcPduInfoPtr)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00224*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_TPTRANSMIT, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00237*/
    if ((TRUE == detNoErr)
        && ((SoAdSrcPduId >= SOAD_PDU_ROUTE_NUM)
            || (SOAD_UPPER_LAYER_TP != SOAD_PDUROUTE_CFG(SoAdSrcPduId).SoAd_TxUpperLayerType)))
    {
        SOAD_DET_REPORT(SOAD_SID_TPTRANSMIT, SOAD_E_INV_PDUID);
        detNoErr = FALSE;
    }
    /* add det check input parameter point */
    if ((TRUE == detNoErr) && (NULL_PTR == SoAdSrcPduInfoPtr))
    {
        SOAD_DET_REPORT(SOAD_SID_TPTRANSMIT, SOAD_E_PARAM_POINTER);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
#if (0u < SOAD_TXBUFFER_NUM)
        SoAd_InnerSocketManageType* soadSoConMPtr;

        if ((SOAD_UNUSED_UINT8 == SoAd_PduRoutManager[SoAdSrcPduId].TxPendingNum)
            && (0u < SoAdSrcPduInfoPtr->SduLength))
        {
            SoAd_uintx pduRouteDestId = SOAD_PDUROT_DESTINDEX_CFG(SoAdSrcPduId, 0u);
            SoAd_SoConIdType soConId = SOAD_PDUROTDEST_CFG(pduRouteDestId).SoAd_TxSocketConRefIndexPtr[0];
            if (SoAd_PduRouteDestIsEnable(pduRouteDestId, soConId))
            {
                soadSoConMPtr = &SoAd_SoConManager[soConId];
                if ((SOAD_SOCON_ONLINE == soadSoConMPtr->SoAd_SoConMode)
                    && (SOAD_SOCKET_CLOSE_NONE == soadSoConMPtr->SocketNeedClose)
                    && (!soadSoConMPtr->TpPduTransmitting))
                {
                    result = SoAd_TpTransmitSoCon(soConId, pduRouteDestId, SoAdSrcPduInfoPtr);
                    if (E_OK == result)
                    {
                        soadSoConMPtr->ActiveSoAdPduRouteId = SoAdSrcPduId;
                    }
                }
            }
        }
#endif /*0u < SOAD_TXBUFFER_NUM*/
    }
    SOAD_NOUSED(SoAdSrcPduId);
    SOAD_NOUSED(SoAdSrcPduInfoPtr);
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x08
Sync/Async: Asynchronous
Reentrancy: Reentrant
Parameters (in): SoConId socket connection index specifying the socket connection which
shall be opened
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType Result of operation
E_OK The request was successful
E_NOT_OK The request was not successful.
Description: This service opens the socket connection specified by SoConId.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_OpenSoCon(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    SoAd_uintx soConGroup;
    /*SWS_SoAd_00615*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_OPENSOCON, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00611*/
    if ((TRUE == detNoErr) && (SoConId >= SOAD_SOCON_NUM))
    {
        SOAD_DET_REPORT(SOAD_SID_OPENSOCON, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00528*/
    if (TRUE == detNoErr)
    {
        soConGroup = SOAD_SOCON_CFG(SoConId).SoAd_SocketConnectionGroupRef;
        if (TRUE == SOAD_SOCONGROUP_CFG(soConGroup).SoAd_SocketAutomaticSoConSetup)
        {
            SOAD_DET_REPORT(SOAD_SID_OPENSOCON, SOAD_E_INV_ARG);
            detNoErr = FALSE;
        }
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        SoAd_InnerSocketManageType* soadSoConMPtr = &SoAd_SoConManager[SoConId];

        if (SOAD_SOCON_REQUEST_NONE == soadSoConMPtr->SoCon_Request[0u])
        {
            soadSoConMPtr->SoCon_Request[0u] = SOAD_SOCON_REQUEST_OPEN;
        }
        else
        {
            if (SOAD_SOCON_REQUEST_NONE == soadSoConMPtr->SoCon_Request[1u])
            {
                if (SOAD_SOCON_REQUEST_CLOSE == soadSoConMPtr->SoCon_Request[0u])
                {
                    soadSoConMPtr->SoCon_Request[1u] = SOAD_SOCON_REQUEST_OPEN;
                }
            }
            else if (SOAD_SOCON_REQUEST_CLOSE == soadSoConMPtr->SoCon_Request[1u])
            {
                soadSoConMPtr->SoCon_Request[1u] = SOAD_SOCON_REQUEST_NONE;
            }
            else
            {
                ; /* do nothing */
            }
        }
        result = E_OK;
    }
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x09
Sync/Async: Asynchronous
Reentrancy: Reentrant
Parameters (in): SoConId socket connection index specifying the socket connection which
shall be closed
abort TRUE: socket connection will immediately be terminated.
FALSE: socket connection will be terminated if no other upper
layer is using this socket connection.
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType Result of operation
E_OK The request was successful
E_NOT_OK The request was not successful.
Description: This service closes the socket connection specified by SoConId.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_CloseSoCon(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId, VAR(boolean, AUTOMATIC) Abort)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    SoAd_uintx soConGroup;
    /*SWS_SoAd_00616*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_CLOSESOCON, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00612*/
    if ((TRUE == detNoErr) && (SoConId >= SOAD_SOCON_NUM))
    {
        SOAD_DET_REPORT(SOAD_SID_CLOSESOCON, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00529*/
    if (TRUE == detNoErr)
    {
        soConGroup = SOAD_SOCON_CFG(SoConId).SoAd_SocketConnectionGroupRef;
        if (TRUE == SOAD_SOCONGROUP_CFG(soConGroup).SoAd_SocketAutomaticSoConSetup)
        {
            SOAD_DET_REPORT(SOAD_SID_CLOSESOCON, SOAD_E_INV_ARG);
            detNoErr = FALSE;
        }
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        SoAd_InnerSocketManageType* soadSoConMPtr = &SoAd_SoConManager[SoConId];

        soadSoConMPtr->SoConCloseAbort = Abort;
        if (FALSE == Abort)
        {
            if (SOAD_SOCON_REQUEST_NONE == soadSoConMPtr->SoCon_Request[0u])
            {
                soadSoConMPtr->SoCon_Request[0u] = SOAD_SOCON_REQUEST_CLOSE;
            }
            else
            {
                if (SOAD_SOCON_REQUEST_NONE == soadSoConMPtr->SoCon_Request[1u])
                {
                    if (SOAD_SOCON_REQUEST_OPEN == soadSoConMPtr->SoCon_Request[0u])
                    {
                        soadSoConMPtr->SoCon_Request[1u] = SOAD_SOCON_REQUEST_CLOSE;
                    }
                }
                else if (SOAD_SOCON_REQUEST_OPEN == soadSoConMPtr->SoCon_Request[1u])
                {
                    soadSoConMPtr->SoCon_Request[1u] = SOAD_SOCON_REQUEST_NONE;
                }
                else
                {
                    ; /* do nothing */
                }
            }
        }
        else
        {
            soadSoConMPtr->SoCon_Request[0u] = SOAD_SOCON_REQUEST_CLOSE;
            soadSoConMPtr->SoCon_Request[1u] = SOAD_SOCON_REQUEST_NONE;
        }
        result = E_OK;
    }
    return result;
}
/*============================================================================*/
/*
Service ID [hex]: 0x22
Sync/Async: Synchronous
Reentrancy: Reentrant
Parameters (in):
SoConId: socket connection index specifying the socket connection
         for which the state shall be returned.
Parameters (inout): None
Parameters (out):
ModePtr: Pointer to memory where the socket connection state shall be stored.
Return value: None
Description: Returns current state of the socket connection specified by SoConId.
 */
FUNC(void, SOAD_CODE)
SoAd_GetSoConMode(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    P2VAR(SoAd_SoConModeType, AUTOMATIC, SOAD_APPL_DATA) ModePtr)
{
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_GETSOCONMODE, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr) && ((SoConId >= SOAD_SOCON_NUM) || (NULL_PTR == ModePtr)))
    {
        SOAD_DET_REPORT(SOAD_SID_GETSOCONMODE, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        *ModePtr = SoAd_SoConManager[SoConId].SoAd_SoConMode;
    }
    return;
}
/*============================================================================*/
/*
Service ID[hex]: 0x0A
Sync/Async: Asynchronous
Reentrancy: Reentrant for different SoConIds. Non reentrant for the same SoConId.
Parameters (in): SoConId Socket connection index specifying the socket connection for
which the IP address shall be set
Type Type of IP address assignment which shall be initiated.
LocalIpAddrPtr Pointer to structure containing the IP address which shall be
assigned to the EthIf controller indirectly specified via SoConId.
Note: This parameter is only used in case the parameter Type is
set to TCPIP_IPADDR_ASSIGNMENT_STATIC, can be set to
NULL_PTR otherwise.
Netmask Network mask of IPv4 address or address prefix of IPv6 address
in CIDR Notation.
Note: This parameter is only used in case the parameter Type is
set to TCPIP_IPADDR_ASSIGNMENT_STATIC.
DefaultRouterPtr Pointer to structure containing the IP address of the default router
(gateway) which shall be assigned.
Note: This parameter is only used in case the parameter Type is
set to TCPIP_IPADDR_ASSIGNMENT_STATIC, can be set to
NULL_PTR otherwise.
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType E_OK: The request has been accepted
E_NOT_OK: The request has not been accepted
Description: By this API service the local IP address assignment which shall be used for the
socket connection specified by SoConId is initiated.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_RequestIpAddrAssignment(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(TcpIp_IpAddrAssignmentType, AUTOMATIC) Type,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_CONST) LocalIpAddrPtr,
    VAR(uint8, AUTOMATIC) Netmask,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_CONST) DefaultRouterPtr)
{
    TcpIp_LocalAddrIdType localAddrId;
    Std_ReturnType result = E_NOT_OK;
    SoAd_uintx soConGroup;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00613*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_REQUESTIPADDRASSIGNMENT, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00617*/
    if ((TRUE == detNoErr) && (SoConId >= SOAD_SOCON_NUM))
    {
        SOAD_DET_REPORT(SOAD_SID_REQUESTIPADDRASSIGNMENT, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        soConGroup = SOAD_SOCON_CFG(SoConId).SoAd_SocketConnectionGroupRef;
        localAddrId = SOAD_SOCKETLOCALADDR_CFG(soConGroup).LocalAddrId;
        result = TcpIp_RequestIpAddrAssignment(localAddrId, Type, LocalIpAddrPtr, Netmask, DefaultRouterPtr);
    }
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x0B
Sync/Async: Asynchronous
Reentrancy: Reentrant
Parameters (in): SoConId socket connection index specifying the socket connection for
which the IP address shall be released
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType E_OK: The request has been accepted
E_NOT_OK: The request has not been accepted
Description: By this API service the local IP address assignment used for the socket
connection specified by SoConId is released.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_ReleaseIpAddrAssignment(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId)
{
    TcpIp_LocalAddrIdType localAddrId;
    Std_ReturnType result = E_NOT_OK;
    SoAd_uintx soConGroup;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00618*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_RELEASEIPADDRASSIGNMENT, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00619*/
    if ((TRUE == detNoErr) && (SoConId >= SOAD_SOCON_NUM))
    {
        SOAD_DET_REPORT(SOAD_SID_RELEASEIPADDRASSIGNMENT, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        soConGroup = SOAD_SOCON_CFG(SoConId).SoAd_SocketConnectionGroupRef;
        localAddrId = SOAD_SOCKETLOCALADDR_CFG(soConGroup).LocalAddrId;
        result = TcpIp_ReleaseIpAddrAssignment(localAddrId);
    }
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x0C
Sync/Async: Synchronous
Reentrancy: Reentrant
Parameters (in): SoConId socket connection index representing the SoAd socket
connection for which the actual local IP address shall be
obtained.
Parameters (inout): LocalAddrPtr Pointer to a struct where the local address (IP address and port)
is stored.
The struct member domain shall be set by the caller of the API to
the desired TcpIp_DomainType and it shall be ensured by the
caller that the struct is large enough to store an address of the
selected type (INET or INET6).
DefaultRouterPtr Pointer to struct where the IP address of the default router
(gateway) is stored (struct member "port" is not used and of
arbitrary value).
The struct must be of the same type and size as LocalAddrPtr.
Parameters (out): NetmaskPtr Pointer to memory where Network mask of IPv4 address or
address prefix of IPv6 address in CIDR Notation is stored
Return value: Std_ReturnType Result of operation
E_OK The request was successful
E_NOT_OK The request was not successful.
Description: Retrieves the local address (IP address and port) actually used for the SoAd
socket connection specified by SoConId, the netmask and default router
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_GetLocalAddr(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    P2VAR(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) LocalAddrPtr,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) NetmaskPtr,
    P2VAR(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) DefaultRouterPtr)
{
    TcpIp_LocalAddrIdType localAddrId;
    Std_ReturnType result = E_NOT_OK;
    SoAd_uintx soConGroup;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00621*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_GETLOCALADDR, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00620*/
    if ((TRUE == detNoErr) && (SoConId >= SOAD_SOCON_NUM))
    {
        SOAD_DET_REPORT(SOAD_SID_GETLOCALADDR, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        soConGroup = SOAD_SOCON_CFG(SoConId).SoAd_SocketConnectionGroupRef;
        LocalAddrPtr->port = SOAD_SOCONGROUP_CFG(soConGroup).SoAd_SocketLocalPort;
        localAddrId = SOAD_SOCKETLOCALADDR_CFG(soConGroup).LocalAddrId;
        result = TcpIp_GetIpAddr(localAddrId, LocalAddrPtr, NetmaskPtr, DefaultRouterPtr);
    }
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x0D
Sync/Async: Synchronous
Reentrancy: Reentrant
Parameters (in): SoConId socket connection index representing the SoAd socket
connection for which the physical source address of the related
EthIf controller shall be obtained.
Parameters (inout): None
Parameters (out): PhysAddrPtr Pointer to the memory where the physical source address (MAC
address) in network byte order is stored
Return value: Std_ReturnType Result of operation
E_OK The request was successful
E_NOT_OK The request was not successful.
Description: Retrieves the physical source address of the EthIf controller used by the SoAd
socket connection specified by SoConId.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_GetPhysAddr(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId, P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) PhysAddrPtr)
{
    TcpIp_LocalAddrIdType localAddrId;
    Std_ReturnType result = E_NOT_OK;
    SoAd_uintx soConGroup;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00623*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_GETPHYSADDR, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00622*/
    if ((TRUE == detNoErr) && (SoConId >= SOAD_SOCON_NUM))
    {
        SOAD_DET_REPORT(SOAD_SID_GETPHYSADDR, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        soConGroup = SOAD_SOCON_CFG(SoConId).SoAd_SocketConnectionGroupRef;
        localAddrId = SOAD_SOCKETLOCALADDR_CFG(soConGroup).LocalAddrId;
        result = TcpIp_GetPhysAddr(localAddrId, PhysAddrPtr);
    }
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x1C
Sync/Async: Synchronous
Reentrancy: Reentrant
Parameters (in): SoConId socket connection index representing the SoAd socket
connection for which the actually specified remote address shall
be obtained.
Parameters (inout): None
Parameters (out): IpAddrPtr Pointer to a struct where the retrieved remote address (IP
address and port) is stored.
Return value: Std_ReturnType Result of operation
E_OK The request was successful
E_NOT_OK The request was not successful.
Description: Retrieves the remote address (IP address and port) actually used for the SoAd
socket connection specified by SoConId
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_GetRemoteAddr(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    P2VAR(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) IpAddrPtr)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00659*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_GETREMOTEADDR, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00660*/
    if ((TRUE == detNoErr) && (SoConId >= SOAD_SOCON_NUM))
    {
        SOAD_DET_REPORT(SOAD_SID_GETREMOTEADDR, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        const SoAd_InnerSocketManageType* soadSoConMPtr = &SoAd_SoConManager[SoConId];

        if (IpAddrPtr->domain == soadSoConMPtr->RemoteAddr.domain)
        {
            if (soadSoConMPtr->RemoteAddrIsSet)
            {
                /*SWS_SoAd_00664*/

                *IpAddrPtr = soadSoConMPtr->RemoteAddr;
                result = E_OK;
            }
            /*SWS_SoAd_00666*/
            else
            {
                ;
            }
        }
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
        /*SWS_SoAd_00698*/
        else
        {
            SOAD_DET_REPORT(SOAD_SID_GETREMOTEADDR, SOAD_E_INV_ARG);
        }
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    }
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x0E
Sync/Async: Synchronous
Reentrancy: Reentrant
Parameters (in): id routing group identifier specifying the routing group to be
enabled
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType Result of operation
E_OK The request was successful
E_NOT_OK The request was not successful.
Description: Enables routing of a group of PDUs in the SoAd related to the RoutingGroup
specified by parameter id.
Routing of PDUs can be either forwarding of PDUs from the upper layer to a TCP
or UDP socket of the TCP/IP stack specified by a PduRoute or the other way
around specified by a SocketRoute.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_EnableRouting(VAR(SoAd_RoutingGroupIdType, AUTOMATIC) id)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00624*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_ENABLEROUTING, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00625*/
    if ((TRUE == detNoErr) && (id >= SOAD_ROUTING_GROUP_NUM))
    {
        SOAD_DET_REPORT(SOAD_SID_ENABLEROUTING, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
#if (0u < SOAD_ROUTING_GROUP_MAX)
        result = SoAd_RoutingGroupControl(id, TRUE, NULL_PTR);
#endif /*0u < SOAD_ROUTING_GROUP_MAX*/
    }
    SOAD_NOUSED(id);
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x20
Sync/Async: Synchronous
Reentrancy: Reentrant
Parameters (in): id routing group identifier specifying the routing group to be
enabled
SoConId socket connection index specifying the socket connection on
which the routing group shall be enabled
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType E_OK The request was successful.
E_NOT_OK The request was not successful.
Description: Enables routing of a group of PDUs in the SoAd related to the RoutingGroup
specified by parameter id only on the socket connection identified by SoConId.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_EnableSpecificRouting(VAR(SoAd_RoutingGroupIdType, AUTOMATIC) id, VAR(SoAd_SoConIdType, AUTOMATIC) SoConId)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00715*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_ENABLESPECIFICROUTING, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00716*/
    if ((TRUE == detNoErr) && (id >= SOAD_ROUTING_GROUP_NUM))
    {
        SOAD_DET_REPORT(SOAD_SID_ENABLESPECIFICROUTING, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
#if (0u < SOAD_ROUTING_GROUP_MAX)
        SoAd_SoConIdType localSoConId = SoConId;
        result = SoAd_RoutingGroupControl(id, TRUE, &localSoConId);
#endif /*0u < SOAD_ROUTING_GROUP_MAX*/
    }
    SOAD_NOUSED(id);
    SOAD_NOUSED(SoConId);
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x0F
Sync/Async: Synchronous
Reentrancy: Reentrant
Parameters (in): id routing group identifier specifying the routing group to be
disabled
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType Result of operation
E_OK The request was successful
E_NOT_OK The request was not successful.
Description: Disables routing of a group of PDUs in the SoAd related to the RoutingGroup
specified by parameter id.
Routing of PDUs can be either forwarding of PDUs from the upper layer to a TCP
or UDP socket of the TCP/IP stack specified by a PduRoute or the other way
around specified by a SocketRoute.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_DisableRouting(VAR(SoAd_RoutingGroupIdType, AUTOMATIC) id)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00627*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_DISABLEROUTING, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00626*/
    if ((TRUE == detNoErr) && (id >= SOAD_ROUTING_GROUP_NUM))
    {
        SOAD_DET_REPORT(SOAD_SID_DISABLEROUTING, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
#if (0u < SOAD_ROUTING_GROUP_MAX)
        result = SoAd_RoutingGroupControl(id, FALSE, NULL_PTR);
#endif /*0u < SOAD_ROUTING_GROUP_MAX*/
    }
    SOAD_NOUSED(id);
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x21
Sync/Async: Synchronous
Reentrancy: Reentrant
Parameters (in): id routing group identifier specifying the routing group to be
disabled
SoConId socket connection index specifying the socket connection on
which the routing group shall be disabled
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType E_OK The request was successful.
E_NOT_OK The request was not successful.
Description: Disables routing of a group of PDUs in the SoAd related to the RoutingGroup
specified by parameter id only on the socket connection identified by SoConId.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_DisableSpecificRouting(VAR(SoAd_RoutingGroupIdType, AUTOMATIC) id, VAR(SoAd_SoConIdType, AUTOMATIC) SoConId)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00718*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_DISABLESPECIFICROUTING, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00719*/
    if ((TRUE == detNoErr) && (id >= SOAD_ROUTING_GROUP_NUM))
    {
        SOAD_DET_REPORT(SOAD_SID_DISABLESPECIFICROUTING, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
#if (0u < SOAD_ROUTING_GROUP_MAX)
        SoAd_SoConIdType localSoConId = SoConId;
        result = SoAd_RoutingGroupControl(id, FALSE, &localSoConId);
#endif /*0u < SOAD_ROUTING_GROUP_MAX*/
    }
    SOAD_NOUSED(id);
    SOAD_NOUSED(SoConId);
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x10
Sync/Async: Synchronous
Reentrancy: Reentrant for different SoConIds. Non reentrant for the same SoConId.
Parameters (in): SoConId socket connection index specifying the socket connection for
which the remote address shall be set
RemoteAddrPtr Struct containint the IP address and port to be set.
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType E_OK: The request has been accepted
E_NOT_OK: The request has not been accepted
Description: By this API service the remote address (IP address and port) of the specified
socket connection shall be set.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_SetRemoteAddr(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_CONST) RemoteAddrPtr)
{
    Std_ReturnType result = E_NOT_OK;
    SoAd_uintx soConGroup = SOAD_SOCON_CFG(SoConId).SoAd_SocketConnectionGroupRef;
    boolean remoteAddrLocked;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00628*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_SETREMOTEADDR, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr) && ((SoConId >= SOAD_SOCON_NUM) || (NULL_PTR == RemoteAddrPtr)))
    {
        SOAD_DET_REPORT(SOAD_SID_SETREMOTEADDR, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00531*/
    if (TRUE == detNoErr)
    {
        if (TRUE == SOAD_SOCONGROUP_CFG(soConGroup).SoAd_SocketAutomaticSoConSetup)
        {
            SOAD_DET_REPORT(SOAD_SID_SETREMOTEADDR, SOAD_E_INV_ARG);
            detNoErr = FALSE;
        }
    }
    /*SWS_SoAd_00699*/
    if (TRUE == detNoErr)
    {
        if (RemoteAddrPtr->domain != SoAd_SoConManager[SoConId].RemoteAddr.domain)
        {
            SOAD_DET_REPORT(SOAD_SID_SETREMOTEADDR, SOAD_E_INV_ARG);
            detNoErr = FALSE;
        }
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        remoteAddrLocked = SoAd_SoConRemoteAddrLocked(SoConId);
        if (!remoteAddrLocked)
        {
            SoAd_InnerSocketManageType* soadSoConMPtr = &SoAd_SoConManager[SoConId];
            /*SWS_SoAd_00533*/
            soadSoConMPtr->RemoteAddrIsSet = TRUE;
            soadSoConMPtr->RemoteAddr = *RemoteAddrPtr;
            /*SWS_SoAd_00687*/
            if ((SOAD_SOCON_ONLINE == soadSoConMPtr->SoAd_SoConMode)
                && (SOAD_IS_IPADDR_ANY(*RemoteAddrPtr) || (TCPIP_PORT_ANY == RemoteAddrPtr->port)))
            {
                soadSoConMPtr->SoAd_SoConMode = SOAD_SOCON_RECONNECT;
                SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, soConGroup);
            }
            else
            {
                SoAd_DisconnectionAndRecoveryHandle(SoConId, soConGroup);
            }
            result = E_OK;
        }
    }
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x1e
Sync/Async: Synchronous
Reentrancy: Reentrant for different SoConIds. Non reentrant for the same SoConId.
Parameters (in): SoConId Index of any socket connection that is part of the
SoAdSocketConnectionGroup.
RemoteAddrPtr Pointer to the structure containing the requested remote IP
address and port.
Parameters (inout): None
Parameters (out): AssignedSoConIdPtr Pointer to the SoAd_SoConIdType where the index of the
socket connection configured with the remote address
(RemoteAddrPtr) shall be stored.
Return value: Std_ReturnType
E_OK: The request was accepted.
E_NOT_OK: The request was rejected,AssignedSoConIdPtr remains unchanged.
Description: This API service shall either return the socket connection index of the
SoAdSocketConnectionGroup where the specified remote address (IP address
and port) is set or assign the remote address to an unused socket connection from
the same SoAdSocketConnectionGroup.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_SetUniqueRemoteAddr(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_CONST) RemoteAddrPtr,
    P2VAR(SoAd_SoConIdType, AUTOMATIC, SOAD_APPL_DATA) AssignedSoConIdPtr)
{
    Std_ReturnType result = E_NOT_OK;
    SoAd_uintx soConGroup = SOAD_SOCON_CFG(SoConId).SoAd_SocketConnectionGroupRef;
    SoAd_SoConIdType soConId = 0u;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00672*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_SETUNIQUEREMOTEADDR, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00673*/
    if ((TRUE == detNoErr) && ((SoConId >= SOAD_SOCON_NUM) || (NULL_PTR == RemoteAddrPtr)))
    {
        SOAD_DET_REPORT(SOAD_SID_SETUNIQUEREMOTEADDR, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    /*similar to SWS_SoAd_00531*/
    if (TRUE == detNoErr)
    {
        if (TRUE == SOAD_SOCONGROUP_CFG(soConGroup).SoAd_SocketAutomaticSoConSetup)
        {
            SOAD_DET_REPORT(SOAD_SID_SETUNIQUEREMOTEADDR, SOAD_E_INV_ARG);
            detNoErr = FALSE;
        }
    }
    /*SWS_SoAd_00700*/
    if (TRUE == detNoErr)
    {
        if (RemoteAddrPtr->domain != SoAd_SoConManager[SoConId].RemoteAddr.domain)
        {
            SOAD_DET_REPORT(SOAD_SID_SETUNIQUEREMOTEADDR, SOAD_E_INV_ARG);
            detNoErr = FALSE;
        }
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        /*SWS_SoAd_00675,SWS_SoAd_00676*/
        if (TRUE == SoAd_GetBestMatchAlgorithmSoCon(soConGroup, RemoteAddrPtr, &soConId, SOAD_SOCON_MATCH_REQUEST_NONE))
        {
            SoAd_SoConManager[soConId].RemoteAddrIsSet = TRUE;
            SoAd_SoConManager[soConId].RemoteAddr = *RemoteAddrPtr;
            result = E_OK;
            *AssignedSoConIdPtr = soConId;
            SoAd_DisconnectionAndRecoveryHandle(soConId, soConGroup);
        }
        /*SWS_SoAd_00678*/
        else
        {
            result = E_NOT_OK;
        }
    }
    return result;
}
/*============================================================================*/
/*
Service ID [hex]: 0x23
Sync/Async: Synchronous
Reentrancy: Reentrant for different SoConIds. Non reentrant for the same SoConId.
Parameters(in):
SoConId: Index of the socket connection for which the remote address shall be released.
Parameters (inout): None
Parameters (out): None
Return value: None
Description: By this API service the remote address (IP address and port) of the specified
socket connection shall be released, i.e. set back to the configured remote address setting.
 */
FUNC(void, SOAD_CODE)
SoAd_ReleaseRemoteAddr(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId)
{
    SoAd_uintx soConGroup = SOAD_SOCON_CFG(SoConId).SoAd_SocketConnectionGroupRef;
    boolean remoteAddrLocked;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_RELEASEREMOTEADDR, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr) && (SoConId >= SOAD_SOCON_NUM))
    {
        SOAD_DET_REPORT(SOAD_SID_RELEASEREMOTEADDR, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    /*similar to SWS_SoAd_00744*/
    if (TRUE == detNoErr)
    {
        if (TRUE == SOAD_SOCONGROUP_CFG(soConGroup).SoAd_SocketAutomaticSoConSetup)
        {
            SOAD_DET_REPORT(SOAD_SID_RELEASEREMOTEADDR, SOAD_E_INV_ARG);
            detNoErr = FALSE;
        }
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        remoteAddrLocked = SoAd_SoConRemoteAddrLocked(SoConId);
        if (!remoteAddrLocked)
        {
            SoAd_ReleaseRemoteAddrHandle(SoConId, soConGroup);
        }
        else
        {
            SoAd_SoConManager[SoConId].RemoteAddrNeedRelease = TRUE;
        }
    }
    return;
}
/*============================================================================*/
/*
Service ID[hex]: 0x11
Sync/Async: Synchronous
Reentrancy: Reentrant for different SoConIds. Non reentrant for the same SoConId.
Parameters (in): id Identification of the I-PDU which the parameter change
shall affect parameter Identifier of the parameter to be changed
value New parameter value
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType E_OK: The request has been accepted
E_NOT_OK: The request has not been accepted
Description: By this API service the SoAd or TCP/IP stack is requested to change a connection
parameter. E.g. the Nagle algorithm may be controlled by this API.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_TpChangeParameter(
    VAR(PduIdType, AUTOMATIC) id,
    VAR(TPParameterType, AUTOMATIC) parameter,
    VAR(uint16, AUTOMATIC) value)
{
    /*follow AUTOSAR_R19_11:SWS_SoAd_00730
      not follow AUTOSAR4.2.2:SWS_SoAd_00629,SWS_SoAd_00630,SWS_SoAd_00631
    */
    SOAD_NOUSED(id);
    SOAD_NOUSED(parameter);
    SOAD_NOUSED(value);
    return E_NOT_OK;
}
/*============================================================================*/
/*
Service ID[hex]: 0x1A
Sync/Async: Synchronous
Reentrancy: Reentrant for different SoConIds. Non reentrant for the same SoConId.
Parameters (in): SoConId socket connection index specifying the socket connection for
which the hostname shall be read
Parameters (inout): length As input parameter, contains the length of the provided data
buffer.Will be overwritten with the length of the actual data.
Parameters (out): data Pointer to provided memory buffer the hostname, i.e. the Fully
Qualified Domain Name (FQDN) according to IETF RFC
4702/IETF RFC 4704 will be copied to.
Return value: Std_ReturnType E_OK: The request has been accepted
E_NOT_OK: The request has not been accepted
Description: By this API service an upper layer of the SoAd can read the currently configured
hostname, i.e. FQDN option in the DHCP submodule of the TCP/IP stack.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_ReadDhcpHostNameOption(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) length,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) data)
{
    Std_ReturnType result = E_NOT_OK;
    SoAd_uintx soConGroup;
    TcpIp_LocalAddrIdType localAddrId;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00701*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_READDHCPHOSTNAMEOPTION, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00702*/
    if ((TRUE == detNoErr) && (SoConId >= SOAD_SOCON_NUM))
    {
        SOAD_DET_REPORT(SOAD_SID_READDHCPHOSTNAMEOPTION, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        soConGroup = SOAD_SOCON_CFG(SoConId).SoAd_SocketConnectionGroupRef;
        localAddrId = SOAD_SOCKETLOCALADDR_CFG(soConGroup).LocalAddrId;
        if (TCPIP_AF_INET == SOAD_SOCKETLOCALADDR_CFG(soConGroup).DomainType)
        {
            /*SWS_SoAd_00703*/
            result = TcpIp_DhcpReadOption(localAddrId, 81u, length, data);
        }
#if (STD_ON == SOAD_IPV6_ADDRESS_ENABLED)
        else
        {
            uint16 dataLength;
            dataLength = (uint16)(*length);
            /*SWS_SoAd_00704*/
            result = TcpIp_DhcpV6ReadOption(localAddrId, 39u, &dataLength, data);
        }
#endif /*STD_ON == SOAD_IPV6_ADDRESS_ENABLED*/
    }
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x1B
Sync/Async: Synchronous
Reentrancy: Reentrant for different SoConIds. Non reentrant for the same SoConId.
Parameters (in): SoConId socket connection index specifying the socket connection for
which the hostname shall be changed length Length of hostname to be set.
data Pointer to memory containing the hostname, i.e. the Fully
Qualified Domain Name (FQDN) according to IETF RFC
4702/IETF RFC 4704.
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType E_OK: The request has been accepted
E_NOT_OK: The request has not been accepted
Description: By this API service an upper layer of the SoAd can set the hostname, i.e. FQDN
option in the DHCP submodule of the TCP/IP stack.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_WriteDhcpHostNameOption(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(uint8, AUTOMATIC) length,
    P2CONST(uint8, AUTOMATIC, SOAD_APPL_CONST) data)
{
    Std_ReturnType result = E_NOT_OK;
    SoAd_uintx soConGroup;
    TcpIp_LocalAddrIdType localAddrId;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00705*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_WRITEDHCPHOSTNAMEOPTION, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00706*/
    if ((TRUE == detNoErr) && (SoConId >= SOAD_SOCON_NUM))
    {
        SOAD_DET_REPORT(SOAD_SID_WRITEDHCPHOSTNAMEOPTION, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        soConGroup = SOAD_SOCON_CFG(SoConId).SoAd_SocketConnectionGroupRef;
        localAddrId = SOAD_SOCKETLOCALADDR_CFG(soConGroup).LocalAddrId;
        if (TCPIP_AF_INET == SOAD_SOCKETLOCALADDR_CFG(soConGroup).DomainType)
        {
            /*SWS_SoAd_00707*/
            result = TcpIp_DhcpWriteOption(localAddrId, 81u, length, data);
        }
#if (STD_ON == SOAD_IPV6_ADDRESS_ENABLED)
        else
        {
            /*SWS_SoAd_00708*/
            result = TcpIp_DhcpV6WriteOption(localAddrId, 39u, (uint16)length, data);
        }
#endif /*STD_ON == SOAD_IPV6_ADDRESS_ENABLED*/
    }
    return result;
}
#define SOAD_STOP_SEC_CODE
#include "SoAd_MemMap.h"
/*============================================================================*/
/*
Service ID[hex]: 0x12
Sync/Async: Synchronous
Reentrancy: Reentrant for different SocketIds. Non reentrant for the same SocketId.
Parameters (in): SocketId Socket identifier of the related local socket resource.
RemoteAddrPtr Pointer to memory containing IP address and port of the
remote host which sent the data.
BufPtr Pointer to the received data.
Length Data length of the received TCP segment or UDP datagram.
Parameters (inout): None
Parameters (out): None
Return value: None
Description: The TCP/IP stack calls this primitive after the reception of data on a socket. The
socket identifier along with configuration information determines which module is
to be called.
 */
#define SOAD_START_SEC_SOADRXINDICATION_CALLBACK_CODE
#include "SoAd_MemMap.h"
FUNC(void, SOAD_CODE)
SoAd_RxIndication(
    VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_CONST) RemoteAddrPtr,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
    VAR(uint16, AUTOMATIC) Length)
{
#if (0u < SOAD_SOCKET_ROUTE_DEST_MAX)
    boolean socketIdValid;
    SoAd_uintx soConGroupId;
    SoAd_SoConIdType soConId = NO_TXMETADATA;
#if (STD_ON == SOAD_SUPPORT_UDP)
    /*used to revert the remote address change*/
    SoAd_FindTheValidSoConType parameter;
    TcpIp_SockAddrType oldRemoteAddr;
    boolean remoteAddrChanged = FALSE;
#endif
    boolean validPduData = TRUE;
    SoAd_uintx socketRouteId;
#endif /*0u < SOAD_SOCKET_ROUTE_DEST_MAX*/
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00264*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_RXINDICATION, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00268*/
    if ((TRUE == detNoErr) && (NULL_PTR == RemoteAddrPtr))
    {
        SOAD_DET_REPORT(SOAD_SID_RXINDICATION, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr) && ((0u == Length) || (NULL_PTR == BufPtr)))
    {
        SOAD_DET_REPORT(SOAD_SID_RXINDICATION, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
#if (0u < SOAD_SOCKET_ROUTE_DEST_MAX)
#if (STD_ON == SOAD_SUPPORT_UDP)
        parameter.RemoteAddr = &oldRemoteAddr;
        parameter.RemoteAddrChanged = &remoteAddrChanged;
#endif
        /*find the valid Socket Connection*/
        socketIdValid = SoAd_FindTheValidSoCon(
            SocketId,
            RemoteAddrPtr,
#if (STD_ON == SOAD_SUPPORT_UDP)
            parameter,
#endif
            &soConId);
        /*handle the received TCP segment and UDP message */
        if (socketIdValid)
        {
            SoAd_InnerSocketManageType* soadSoConMPtr = &SoAd_SoConManager[soConId];

            if ((NO_TXMETADATA != soConId) && (SOAD_SOCKET_CLOSE_NONE == soadSoConMPtr->SocketNeedClose))
            {
                soConGroupId = SOAD_SOCON_CFG(soConId).SoAd_SocketConnectionGroupRef;
#if (STD_ON == SOAD_SUPPORT_HEADERID)
                if (SOAD_SOCONGROUP_CFG(soConGroupId).SoAd_PduHeaderEnable)
                {
                    if (TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(soConId).SoAd_TcpUdpProtocol)
                    {
#if (0u < SOAD_RXBUFFER_NUM)
                        SoAd_HeaderEnableTcpSoConRxHandle(soConId, Length, BufPtr);
#endif /*0u < SOAD_RXBUFFER_NUM*/
                    }
#if (STD_ON == SOAD_SUPPORT_UDP)
                    /*header enable and UDP socket connection*/
                    else
                    {
                        validPduData = SoAd_HeaderEnableUdpSoConRxHandle(soConId, Length, BufPtr);
                    }
#endif /* STD_ON == SOAD_SUPPORT_UDP */
                }
                /*Header is disabled,just one socket route ref the SoCon*/
                else
#endif /*STD_ON == SOAD_SUPPORT_HEADERID*/
                {
                    if (NULL_PTR != SOAD_SOCON_CFG(soConId).SoAd_RefSocketRoutePtr)
                    {
                        socketRouteId = SOAD_SOCON_CFG(soConId).SoAd_RefSocketRoutePtr[0u];
                        if (SOAD_UPPER_LAYER_TP == SOAD_SRDEST_CFG(socketRouteId).SoAd_RxUpperLayerType)
                        {
#if (0u < SOAD_RXBUFFER_NUM)
                            if (SoAd_SocketRouteDestManager[socketRouteId].SocketRouteDestEnable)
                            {
                                SoAd_HeaderDisableSoConRxTpPduHandle(soConId, socketRouteId, Length, BufPtr);
                            }
#if (STD_ON == SOAD_SUPPORT_UDP)
                            else
                            {
                                validPduData = FALSE;
                            }
#endif /*STD_ON == SOAD_SUPPORT_UDP*/
#endif /*0u < SOAD_RXBUFFER_NUM*/
                        }
                        else
                        {
                            /*Skip further processing if PDU length is 0 and (SoAdPduHeaderEnable is FALSE or
                             * SoAdRxUpperLayerType is TP)*/
                            if (SoAd_SocketRouteDestManager[socketRouteId].SocketRouteDestEnable)
                            {
                                SoAd_HeaderDisableSoConRxIfPduHandle(soConId, socketRouteId, Length, BufPtr);
                            }
#if (STD_ON == SOAD_SUPPORT_UDP)
                            else
                            {
                                validPduData = FALSE;
                            }
#endif /*STD_ON == SOAD_SUPPORT_UDP*/
                        }
                    }
                }
#if (STD_ON == SOAD_SUPPORT_UDP)
                /*SWS_SoAd_00710*/
                if (!validPduData && remoteAddrChanged)
                {
                    soadSoConMPtr->RemoteAddr = oldRemoteAddr;
                    soadSoConMPtr->UdpAliveSupervisionTime = 0u;
                    soadSoConMPtr->SoAd_SoConMode = SOAD_SOCON_RECONNECT;
                    SoAd_SoConModeChgNotificationAndTpStartOfReception(soConId, soConGroupId);
                }
#endif /*STD_ON == SOAD_SUPPORT_UDP*/
            }
        }
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
        else
        {
            /*SWS_SoAd_00267*/
            SOAD_DET_REPORT(SOAD_SID_RXINDICATION, SOAD_E_INV_SOCKETID);
        }
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
#endif /*0u < SOAD_SOCKET_ROUTE_DEST_MAX*/
    }
    SOAD_NOUSED(SocketId);
    SOAD_NOUSED(RemoteAddrPtr);
    SOAD_NOUSED(BufPtr);
    SOAD_NOUSED(Length);
    return;
}
#define SOAD_STOP_SEC_SOADRXINDICATION_CALLBACK_CODE
#include "SoAd_MemMap.h"
/*============================================================================*/
/*
Service ID[hex]: 0x13
Sync/Async: Synchronous
Reentrancy: Reentrant for different SocketIds. Non reentrant for the same SocketId.
Parameters (in): SocketId Socket identifier of the related local socket resource.
BufPtr Pointer to buffer for transmission data.
BufLength Length of provided data buffer.
Parameters (inout): None
Parameters (out): None
Return value: BufReq_ReturnType BUFREQ_OK: Data has been copied to the transmit buffer
completely as requested.
BUFREQ_E_NOT_OK: Data has not been copied. Request
failed. (No further action for TcpIp required. Later the upper
layer might either close the socket or retry the transmit request)
Description: This service requests to copy data for transmission to the buffer indicated.
This call is triggered by TcpIp_Transmit(). Note: The call to <Up>_CopyTxData()
may happen in the context of TcpIp_Transmit().
 */
#define SOAD_START_SEC_SOADCOPYTXDATA_CALLBACK_CODE
#include "SoAd_MemMap.h"
FUNC(BufReq_ReturnType, SOAD_CODE)
SoAd_CopyTxData(
    VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
    VAR(uint16, AUTOMATIC) BufLength)
{
    BufReq_ReturnType ret = BUFREQ_E_NOT_OK;
#if (0u < SOAD_TXBUFFER_NUM)
    SoAd_SoConIdType soConId;
#endif
#if ((0u < SOAD_TXBUFFER_NUM) || (STD_ON == SOAD_DEV_ERROR_DETECT))
    SoAd_SoConIdType cnt;
    boolean socketIdValid = FALSE;
    SoAd_InnerSocketManageType* soadSoConMPtr;

    for (cnt = 0u; cnt < SOAD_SOCON_NUM; cnt++)
    {
        soadSoConMPtr = &SoAd_SoConManager[cnt];
        if ((SocketId == soadSoConMPtr->TcpIpSocketId) && (SOAD_SOCON_ONLINE == soadSoConMPtr->SoAd_SoConMode)
            && soadSoConMPtr->TpPduTransmitting && (TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(cnt).SoAd_TcpUdpProtocol))
        {
            socketIdValid = TRUE;
#if (0u < SOAD_TXBUFFER_NUM)
            soConId = cnt;
#endif
            break;
        }
    }
#endif /* 0u < SOAD_TXBUFFER_NUM || STD_ON == SOAD_DEV_ERROR_DETECT */
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_COPYTXDATA, SOAD_E_NOTINIT);
    }
    else if (TRUE != socketIdValid)
    {
        SOAD_DET_REPORT(SOAD_SID_COPYTXDATA, SOAD_E_INV_SOCKETID);
    }
    else
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
#if (0u < SOAD_TXBUFFER_NUM)
        if (socketIdValid && (NULL_PTR != BufPtr) && (0u < BufLength)) /* PRQA S 2995 */ /* MISRA Rule 2.2 */
        {
            PduInfoType PduInfo;
            PduIdType pduRouteId;
            SoAd_UpLayerModuleType upModule;
            PduLengthType validDataLength;
            SoAd_uintx txBufferId, pduRouteDestId;
            const SoAd_PduRouteType* soadPduRoutePtr;
            SoAd_SoTxBufManType* soadSoTxBufMPrt;

            txBufferId = *SOAD_SOCKET_TXBUF_INDEX_CFG(soConId);
            PduInfo.MetaDataPtr = NULL_PTR;
            PduInfo.SduLength = BufLength;
            PduInfo.SduDataPtr = BufPtr;
            soadSoConMPtr = &SoAd_SoConManager[soConId];
            pduRouteId = soadSoConMPtr->ActiveSoAdPduRouteId;
            soadPduRoutePtr = &SOAD_PDUROUT_CFG(pduRouteId);
            upModule = soadPduRoutePtr->SoAd_RefModule;
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
            if (NULL_PTR != SOAD_UPPERLAY_PCCFG(upModule).Tp_CopyTxData_FuncPtr)
            {
                ret = SOAD_UPPERLAY_PCCFG(upModule)
                          .Tp_CopyTxData_FuncPtr(soadPduRoutePtr->SoAd_UpPduId, &PduInfo, NULL_PTR, &validDataLength);
                if (BUFREQ_OK == ret)
                {
                    SchM_Enter_SoAd_ExclusiveArea();
                    soadSoTxBufMPrt = &SoAd_SoTxBufMan[txBufferId];
                    soadSoTxBufMPrt->upCopyLength += BufLength;
                    if (SOAD_SOCONGROUP_TCPIMME_TXCONF(SOAD_SOCKET_LINKGORUP(soConId))
                        && (soadSoTxBufMPrt->txPduTotalLength == soadSoTxBufMPrt->upCopyLength))
                    {
                        soadSoTxBufMPrt->txPduTotalLength = 0u;
                        soadSoTxBufMPrt->upCopyLength = 0u;
                        soadSoConMPtr->TpPduTransmitting = FALSE;
                        if (NULL_PTR != SOAD_UPPERLAY_PCCFG(upModule).Tp_TxConfirmation_FuncPtr)
                        {
                            SOAD_UPPERLAY_PCCFG(upModule).Tp_TxConfirmation_FuncPtr(
                                soadPduRoutePtr->SoAd_UpPduId,
                                E_OK);
                        }
                    }
                    soadSoConMPtr->TxNoAckLength += BufLength;
                    pduRouteDestId = soadPduRoutePtr->SoAd_PduRouteDestIndex[0];
                    SoAd_PduRouteDestManager[pduRouteDestId].TxPendingLength = soadSoConMPtr->TxNoAckLength;
                    SchM_Exit_SoAd_ExclusiveArea();
                }
                else
                {
                    SoAd_TpPduTxErrorHandle(soConId, txBufferId, upModule, soadPduRoutePtr->SoAd_UpPduId);
                    ret = BUFREQ_E_NOT_OK;
                }
            }
#endif /* SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u */
        }
#endif /*0u < SOAD_TXBUFFER_NUM*/
    }
    SOAD_NOUSED(SocketId);
    SOAD_NOUSED(BufPtr);
    SOAD_NOUSED(BufLength);
    return ret;
}
#define SOAD_STOP_SEC_SOADCOPYTXDATA_CALLBACK_CODE
#include "SoAd_MemMap.h"
/*============================================================================*/
/*
Service ID[hex]: 0x14
Sync/Async: Synchronous
Reentrancy: Reentrant for different SocketIds. Non reentrant for the same SocketId.
Parameters (in): SocketId Socket identifier of the related local socket resource.
Length Number of transmitted data bytes.
Parameters (inout): None
Parameters (out): None
Return value: None
Description: The TCP/IP stack calls this function after the data has been acknowledged by the
peer for TCP.
Caveats: The upper layer might not be able to determine exactly which data bytes
have been confirmed.
 */
#define SOAD_START_SEC_SOADTXCONFIRMATION_CALLBACK_CODE
#include "SoAd_MemMap.h"
FUNC(void, SOAD_CODE)
SoAd_TxConfirmation(VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId, VAR(uint16, AUTOMATIC) Length)
{
#if (STD_ON == SOAD_SUPPORT_TCP)
    SoAd_SoConIdType soConcnt;
    SoAd_SoConIdType soConId = NO_TXMETADATA;
    SoAd_InnerSocketManageType* soadSoConMPtr;

    for (soConcnt = 0u; soConcnt < SOAD_SOCON_NUM; soConcnt++)
    {
        soadSoConMPtr = &SoAd_SoConManager[soConcnt];
        if ((SocketId == soadSoConMPtr->TcpIpSocketId) && (SOAD_SOCON_ONLINE == soadSoConMPtr->SoAd_SoConMode)
            && (TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(soConcnt).SoAd_TcpUdpProtocol))
        {
            soConId = soConcnt;
            soadSoConMPtr = &SoAd_SoConManager[soConId];
            break;
        }
    }
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_TXCONFIRMATION, SOAD_E_NOTINIT);
    }
    else if (NO_TXMETADATA == soConId)
    {
        SOAD_DET_REPORT(SOAD_SID_TXCONFIRMATION, SOAD_E_INV_SOCKETID);
    }
    /* SWS_SoAd_00271 */
    else if ((0u == Length) || (soadSoConMPtr->TxNoAckLength < Length))
    {
        SOAD_DET_REPORT(SOAD_SID_TXCONFIRMATION, SOAD_E_INV_ARG);
    }
    else
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        if (NO_TXMETADATA != soConId) /* PRQA S 2991, 2995 */ /* MISRA Rule 2.2, 14.3 */
        {
            SoAd_uintx cnt, pduRouteDestId;
            const SoAd_SocketConnectionType* soadSoConCfgPtr = &SOAD_SOCON_CFG(soConId);
            SoAd_uintx refPduRouteDestNum = soadSoConCfgPtr->SoAd_RefPduRouteDestNum;

            soadSoConMPtr->TxNoAckLength -= Length;
            for (cnt = 0u; cnt < refPduRouteDestNum; cnt++)
            {
                pduRouteDestId = soadSoConCfgPtr->SoAd_RefPduRouteDestPtr[cnt];
                if (0u < SoAd_PduRouteDestManager[pduRouteDestId].TxPendingLength)
                {
                    SoAd_TxConfirmationHandle(soConId, Length, pduRouteDestId);
                }
            }
        }
    }
#endif /*STD_ON == SOAD_SUPPORT_TCP*/
    SOAD_NOUSED(SocketId);
    SOAD_NOUSED(Length);
    return;
}
#define SOAD_STOP_SEC_SOADTXCONFIRMATION_CALLBACK_CODE
#include "SoAd_MemMap.h"
/*============================================================================*/
/*
Service ID[hex]: 0x15
Sync/Async: Synchronous
Reentrancy: Non Reentrant
Parameters (in): SocketId Socket identifier of the related local socket resource which
has been used at TcpIp_Bind()
SocketIdConnected Socket identifier of the local socket resource used for the
established connection.
RemoteAddrPtr IP address and port of the remote host.
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType Result of operation
E_OK upper layer accepts the established connection
E_NOT_OK upper layer refuses the established
connection, TcpIp stack shall close the connection.
Description: This service gets called if the stack put a socket into the listen mode before (as
server) and a peer connected to it (as client).
In detail: The TCP/IP stack calls this function after a socket was set into the listen
state with TcpIp_TcpListen() and a TCP connection is requested by the peer.
 */
#define SOAD_START_SEC_SOADTCPACCEPTED_CALLBACK_CODE
#include "SoAd_MemMap.h"
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_TcpAccepted(
    VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId,
    VAR(TcpIp_SocketIdType, AUTOMATIC) SocketIdConnected,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_CONST) RemoteAddrPtr)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == SOAD_SUPPORT_TCP)
    SoAd_SoConIdType soConId = 0u;
    SoAd_uintx soConGroupId;
    boolean soConGroupValid = FALSE;
    boolean soConValid = FALSE;
    SoAd_InnerSocketManageType* soadSoConMPtr;
    SoAd_InnerSocketGroupManageType* soadSoConGruMPtr;
    const SoAd_SocketTcpType* soadSoTcpCfgPtr;

#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /*SWS_SoAd_00272*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_TCPACCEPTED, SOAD_E_NOTINIT);
    }
    else if (NULL_PTR == RemoteAddrPtr)
    {
        SOAD_DET_REPORT(SOAD_SID_TCPACCEPTED, SOAD_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        for (soConGroupId = 0u; (soConGroupId < SOAD_SO_CON_GROUP_NUM) && (FALSE == soConGroupValid); soConGroupId++)
        {
            soadSoConGruMPtr = &SoAd_SoConGroupManager[soConGroupId];
            soadSoTcpCfgPtr = SOAD_TCPTYPE_CFG(soConGroupId);
            if (soadSoConGruMPtr->TcpIpSocketValid && (SocketId == soadSoConGruMPtr->TcpIpSocketId)
                && (NULL_PTR != soadSoTcpCfgPtr) && !soadSoTcpCfgPtr->SoAd_SocketTcpInitiate)
            {
                if (SOAD_SOCONGROUP_CFG(soConGroupId).SoAd_SocketMsgAcceptanceFilterEnabled)
                {
                    if (TRUE
                        == SoAd_GetBestMatchAlgorithmSoCon(
                            soConGroupId,
                            RemoteAddrPtr,
                            &soConId,
                            SOAD_SOCON_MATCH_REQUEST_RECONNECT))
                    {
                        soadSoConMPtr = &SoAd_SoConManager[soConId];
                        soConValid = TRUE;
                    }
                }
                else
                {
                    /*if SoAd_SocketMsgAcceptanceFilterEnabled is FALSE:the SoConGroup just have only one SoCon*/
                    soConId = SOAD_SOCONGROUP_CFG(soConGroupId).SoAd_SoConId[0u];
                    soadSoConMPtr = &SoAd_SoConManager[soConId];
                    if (!soadSoConMPtr->TcpIpSocketValid && (soadSoConMPtr->SoAd_SoConMode == SOAD_SOCON_RECONNECT))
                    {
                        soConValid = TRUE;
                    }
                }
                if (soConValid)
                {
                    /*Set the new tcp communication socket NoDelay flag*/
                    if (NULL_PTR != soadSoTcpCfgPtr->SoAd_SocketTcpNoDelay)
                    {
                        uint8 delayVlu = 0x01u;
                        if (soadSoTcpCfgPtr->SoAd_SocketTcpNoDelay[0])
                        {
                            delayVlu = 0x0u;
                        }
                        (void)TcpIp_ChangeParameter(SocketIdConnected, TCPIP_PARAMID_TCP_NAGLE, &delayVlu);
                    }
                    /*SWS_SoAd_00636:At SoAd_TcpAccepted(), SoAd shall perform the following
                    actions if the TCP SoAdSocketConnectionGroup related to SocketId has both
                    SoAdSocketTcpInitiate and SoAdSocketMsgAcceptanceFilterEnabled set to FALSE
                    and is not online (i.e. current connection state not SOAD_SOCON_ONLINE)*/
                    soadSoConMPtr->RemoteAddrIsSet = TRUE;
                    soadSoConMPtr->RemoteAddr = *RemoteAddrPtr;
                    soadSoConMPtr->TcpIpSocketValid = TRUE;
                    soadSoConMPtr->TcpIpSocketId = SocketIdConnected;
                    soadSoConMPtr->SoAd_SoConMode = SOAD_SOCON_ONLINE;
                    SoAd_SoConModeChgNotificationAndTpStartOfReception(soConId, soConGroupId);
                    soadSoConGruMPtr->SoConSocketValidNum += 1u;
                    result = E_OK;
                }
                soConGroupValid = TRUE;
            }
        }
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
        /*SWS_SoAd_00273*/
        if (TRUE != soConGroupValid)
        {
            SOAD_DET_REPORT(SOAD_SID_TCPACCEPTED, SOAD_E_INV_SOCKETID);
        }
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    }
#endif /* STD_ON == SOAD_SUPPORT_TCP */
    SOAD_NOUSED(SocketId);
    SOAD_NOUSED(SocketIdConnected);
    SOAD_NOUSED(RemoteAddrPtr);
    return result;
}
#define SOAD_STOP_SEC_SOADTCPACCEPTED_CALLBACK_CODE
#include "SoAd_MemMap.h"
/*============================================================================*/
/*
Service ID[hex]: 0x16
Sync/Async: Synchronous
Reentrancy: Non Reentrant
Parameters (in): SocketId Socket identifier of the related local socket resource.
Parameters (inout): None
Parameters (out): None
Return value: None
Description: This service gets called if the stack initiated a TCP connection
before (as client) and the peer (the server) acknowledged the connection set up.
In detail:
The TCP/IP stack calls this function after a socket was requested to connect with
TcpIp_TcpConnect() and a TCP connection is confirmed by the peer.
The parameter value of SocketId equals the SocketId value of the preceeding
TcpIp_TcpConnect() call.
 */
#define SOAD_START_SEC_SOADTCPCONNECTED_CALLBACK_CODE
#include "SoAd_MemMap.h"
FUNC(void, SOAD_CODE)
SoAd_TcpConnected(VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId)
{
#if (STD_ON == SOAD_SUPPORT_TCP)
    SoAd_SoConIdType soConId;
    SoAd_uintx soConGroupId;

    SoAd_InnerSocketManageType* soadSoConMPtr;
    const SoAd_SocketTcpType* soadSoTcpCfgPtr;

#if (STD_ON == SOAD_DEV_ERROR_DETECT)

    boolean socketIdValid = FALSE;

    /*SWS_SoAd_00274*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_TCPCONNECTED, SOAD_E_NOTINIT);
    }
    else
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        for (soConId = 0u; soConId < SOAD_SOCON_NUM; soConId++)
        {
            soConGroupId = SOAD_SOCON_CFG(soConId).SoAd_SocketConnectionGroupRef;
            soadSoConMPtr = &SoAd_SoConManager[soConId];
            soadSoTcpCfgPtr = SOAD_TCPTYPE_CFG(soConGroupId);
            /*SWS_SoAd_00593:Within SoAd_TcpConnected() SoAd shall change
             * the state of the socket connection to SOAD_SOCON_ONLINE*/
            if (soadSoConMPtr->TcpIpSocketValid && (SocketId == soadSoConMPtr->TcpIpSocketId)
                && (TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(soConId).SoAd_TcpUdpProtocol)
                && soadSoTcpCfgPtr->SoAd_SocketTcpInitiate)
            {
                if (SOAD_SOCON_ONLINE != soadSoConMPtr->SoAd_SoConMode)
                {
                    soadSoConMPtr->SoAd_SoConMode = SOAD_SOCON_ONLINE;
                    SoAd_SoConModeChgNotificationAndTpStartOfReception(soConId, soConGroupId);
                }
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
                socketIdValid = TRUE;
#endif
                break;
            }
        }
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
        if (FALSE == socketIdValid)
        {
            SOAD_DET_REPORT(SOAD_SID_TCPCONNECTED, SOAD_E_INV_SOCKETID);
        }
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    }
#endif /* STD_ON == SOAD_SUPPORT_TCP */
    SOAD_NOUSED(SocketId);
    return;
}
#define SOAD_STOP_SEC_SOADTCPCONNECTED_CALLBACK_CODE
#include "SoAd_MemMap.h"
/*============================================================================*/
/*
Service ID[hex]: 0x17
Sync/Async: Synchronous
Reentrancy: Non Reentrant
Parameters (in): SocketId Socket identifier of the related local socket resource.
Event This parameter contains a description of the event just encountered.
Parameters (inout): None
Parameters (out): None
Return value: None
Description: This service gets called if the stack encounters a condition described by the
values in Event.
 */
#define SOAD_START_SEC_SOADTCPIPEVENT_CALLBACK_CODE
#include "SoAd_MemMap.h"
FUNC(void, SOAD_CODE)
SoAd_TcpIpEvent(VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId, VAR(TcpIp_EventType, AUTOMATIC) Event)
{
    SoAd_uintx cnt;
#if (STD_ON == SOAD_SUPPORT_TCP)
    SoAd_uintx soConGroupId;
    SoAd_SoConIdType soConId;
    SoAd_InnerSocketManageType* soadSoConMPtr;
#endif /*STD_ON == SOAD_SUPPORT_TCP*/
    boolean socketIdValid = FALSE;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00276*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_TCPIPEVENT, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00278*/
    if ((TRUE == detNoErr) && (Event > TCPIP_TLS_HANDSHAKE_SUCCEEDED))
    {
        SOAD_DET_REPORT(SOAD_SID_TCPIPEVENT, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        if (TCPIP_TLS_HANDSHAKE_SUCCEEDED != Event)
        {
            SoAd_InnerSocketGroupManageType* soadSoConGruMPtr;

            /*find a udp SoCon Group or a tcp SoCon Group(listen socket)*/
            for (cnt = 0u; (cnt < SOAD_SO_CON_GROUP_NUM) && (FALSE == socketIdValid); cnt++)
            {
                soadSoConGruMPtr = &SoAd_SoConGroupManager[cnt];
                if (soadSoConGruMPtr->TcpIpSocketValid && (SocketId == soadSoConGruMPtr->TcpIpSocketId))
                {
#if (STD_ON == SOAD_SUPPORT_TCP)
                    if (NULL_PTR != SOAD_TCPTYPE_CFG(cnt))
                    {
                        if ((TCPIP_TCP_CLOSED == Event) || (TCPIP_TCP_RESET == Event))
                        {
                            /*SWS_SoAd_00645:TCPIP_TCP_CLOSED for a Listen-Socket*/
                            soadSoConGruMPtr->TcpIpSocketValid = FALSE;
                        }
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
                        /*SWS_SoAd_00278*/
                        else
                        {
                            SOAD_DET_REPORT(SOAD_SID_TCPIPEVENT, SOAD_E_INV_ARG);
                        }
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
                    }
#endif /* STD_ON == SOAD_SUPPORT_TCP */
#if (STD_ON == SOAD_SUPPORT_UDP)
                    if (NULL_PTR != SOAD_UDPTYPE_CFG(cnt))
                    {
                        SoAd_CloseUdpSoConGroup(Event, cnt);
                    }
#endif /* STD_ON == SOAD_SUPPORT_UDP */
                    socketIdValid = TRUE;
                }
            }

/*find a tcp socket connection*/
#if (STD_ON == SOAD_SUPPORT_TCP)
            for (soConId = 0u; (soConId < SOAD_SOCON_NUM) && (FALSE == socketIdValid); soConId++)
            {
                /*SWS_SoAd_00646,SWS_SoAd_00688*/
                soadSoConMPtr = &SoAd_SoConManager[soConId];
                if (soadSoConMPtr->TcpIpSocketValid && (SocketId == soadSoConMPtr->TcpIpSocketId)
                    && (TCPIP_UDP_CLOSED != Event))
                {
                    /*Adjust the standard SWS_SoAd_00646*/
                    if (SOAD_SOCON_OFFLINE != soadSoConMPtr->SoAd_SoConMode)
                    {
                        soConGroupId = SOAD_SOCON_CFG(soConId).SoAd_SocketConnectionGroupRef;
                        soadSoConGruMPtr = &SoAd_SoConGroupManager[soConGroupId];
                        if (TCPIP_TCP_FIN_RECEIVED == Event)
                        {
                            if (soadSoConMPtr->SocketNeedClose != SOAD_SOCKET_CLOSE_DEFERRED)
                            {
#if (0u < SOAD_RXBUFFER_NUM)
                                if (SoAd_CheckRxTpPduDataHandle(soConId))
                                {
                                    /*rx tp data not copy to UL, need delay close socket*/
                                    soadSoConMPtr->SocketNeedClose = SOAD_SOCKET_CLOSE_DEFERRED;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
                                    soadSoConMPtr->SetSocketNeedCloseLine = __LINE__;
#endif /* STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG */
                                }
                                else
#endif
                                {
                                    SoAd_SoConCloseHandle(soConId, soConGroupId, TRUE);
                                }
                            }
                        }
                        else
                        {
                            soadSoConMPtr->TcpIpSocketValid = FALSE;
                            SoAd_SoConCloseHandle(soConId, soConGroupId, FALSE);
                            if (0u < soadSoConGruMPtr->SoConSocketValidNum)
                            {
                                (soadSoConGruMPtr->SoConSocketValidNum) -= 1u;
                            }
                        }
                        /*Notification UP Module SoCon changed*/
                        if ((SOAD_SOCON_ONLINE == soadSoConMPtr->SoAd_SoConMode)
                            && (SOAD_SOCKET_CLOSE_NONE == soadSoConMPtr->SocketNeedClose))
                        {
                            if (soadSoConMPtr->ChangeModeToOffline)
                            {
                                soadSoConMPtr->SoAd_SoConMode = SOAD_SOCON_OFFLINE;
                                soadSoConMPtr->ChangeModeToOffline = FALSE;
                            }
                            else
                            {
                                soadSoConMPtr->SoAd_SoConMode = SOAD_SOCON_RECONNECT;
                            }
                            SoAd_SoConModeChgNotificationAndTpStartOfReception(soConId, soConGroupId);
                        }
                        /*server-side,think about close the"listen socket"*/
                        if (!SOAD_TCPTYPE_CFG(soConGroupId)->SoAd_SocketTcpInitiate
                            && soadSoConGruMPtr->TcpIpSocketValid && (0u == soadSoConGruMPtr->SoConSocketValidNum))
                        {
                            (void)TcpIp_Close(soadSoConGruMPtr->TcpIpSocketId, soadSoConMPtr->SoConCloseAbort);
                        }
                    }
                    else
                    {
                        soadSoConMPtr->TcpIpSocketValid = FALSE;
                    }
                    socketIdValid = TRUE;
                }
            }
#endif /*SOAD_SUPPORT_TCP*/
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
            /*SWS_SoAd_00277*/
            if (FALSE == socketIdValid)
            {
                SOAD_DET_REPORT(SOAD_SID_TCPIPEVENT, SOAD_E_INV_SOCKETID);
            }
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
        }
    }
    return;
}
#define SOAD_STOP_SEC_SOADTCPIPEVENT_CALLBACK_CODE
#include "SoAd_MemMap.h"
/*============================================================================*/
/*
Service ID[hex]: 0x18
Sync/Async: Synchronous
Reentrancy: Non Reentrant
Parameters (in): IpAddrId IP address Identifier, representing an IP address specified in the TcpIp
module configuraiton (e.g. static IPv4 address on EthIf controller 0).
State state of IP address assignment
Parameters (inout): None
Parameters (out): None
Return value: None
Description: This service gets called by the TCP/IP stack if an IP address assignment changes
(i.e. new address assigned or assigned address becomes invalid).
 */
#define SOAD_START_SEC_SOADLOCALIPADDRASSIGNMENTCHG_CALLBACK_CODE
#include "SoAd_MemMap.h"
FUNC(void, SOAD_CODE)
SoAd_LocalIpAddrAssignmentChg(
    VAR(TcpIp_LocalAddrIdType, AUTOMATIC) IpAddrId,
    VAR(TcpIp_IpAddrStateType, AUTOMATIC) State)
{
    SoAd_uintx cnt0;
    SoAd_SoConIdType soConId, cnt1;
    SoAd_uintx socketConNum;

#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00279*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_LOCALIPADDRASSIGNMENTCHG, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        /*SWS_SoAd_00729*/
        for (cnt0 = 0u; (cnt0 < SOAD_SO_CON_GROUP_NUM); cnt0++)
        {
            if (IpAddrId == SOAD_SOCKETLOCALADDR_CFG(cnt0).LocalAddrId)
            {
                SoAd_SoConGroupManager[cnt0].LocalIpAddrState = State;
                socketConNum = SOAD_SOCONGROUP_CFG(cnt0).SoAd_SoConNum;
                for (cnt1 = 0u; cnt1 < socketConNum; cnt1++)
                {
                    soConId = SOAD_SOCONGROUP_CFG(cnt0).SoAd_SoConId[cnt1];
                    if ((SOAD_SOCON_OFFLINE != SoAd_SoConManager[soConId].SoAd_SoConMode)
                        && (TCPIP_IPADDR_STATE_UNASSIGNED == State))
                    {
                        SoAd_SoConManager[soConId].SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
                        SoAd_SoConManager[soConId].SetSocketNeedCloseLine = __LINE__;
#endif /* STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG */
                    }
                    SoAd_SoConLocalIpAddrAssignmentChg(soConId, cnt0);
                }
            }
        }
    }
    return;
}
#define SOAD_STOP_SEC_SOADLOCALIPADDRASSIGNMENTCHG_CALLBACK_CODE
#include "SoAd_MemMap.h"
/*============================================================================*/
/*
Service ID[hex]: 0x19
Description: Schedules the Socket Adaptor. (Entry point for scheduling)
 */
#define SOAD_START_SEC_CODE
#include "SoAd_MemMap.h"
FUNC(void, SOAD_CODE)
SoAd_MainFunction(VAR(void, AUTOMATIC))
{
    SoAd_SoConIdType soConCnt;
    if (SOAD_STATE_INIT == SoAd_Module_Status)
    {
        for (soConCnt = 0u; soConCnt < SOAD_SOCON_NUM; soConCnt++)
        {
#if (0u < SOAD_TXBUFFER_NUM)
            if (NULL_PTR != SOAD_SOCKET_TXBUF_INDEX_CFG(soConCnt))
            {
                SoAd_MainFunctionTpTransmitHandle(soConCnt);
            }
#endif /*0u < SOAD_TXBUFFER_NUM*/
#if (STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER)
            SoAd_MainFunctionNPduUdpTxHandle(soConCnt);
#endif /*STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER*/
            SoAd_ReleaseRemoteAddrMainFunctionHandle(soConCnt);
#if (STD_ON == SOAD_SUPPORT_UDP)
            SoAd_UdpAliveSupervisionTimeHandle(soConCnt);
#endif /* STD_ON == SOAD_SUPPORT_UDP */
            /*handle the Open/Close of requested(SoAd_OpenSoCon,SoAd_CloseSoCon,
             * SoAdSocketAutomaticSoConSetup) socket connections;
             * handle the Disconnection and recovery of SoCon*/
            SoAd_SoConModeChangeHandle(soConCnt);
#if ((0u < SOAD_SOCKET_ROUTE_DEST_MAX) && (0u < SOAD_RXBUFFER_NUM))
            /*copy the PDU data(in the Rx Buffer) to the Up module*/
            SoAd_SoConRxDataMainFunctionHandle(soConCnt);
#endif /*0u < SOAD_RXBUFFER_NUM*/
        }
        SoAd_MainFunctionTxConfirmationHandle();
    }

    return;
}
#define SOAD_STOP_SEC_CODE
#include "SoAd_MemMap.h"
/*==============================================================================
*                                       LOCAL FUNCTIONS
==============================================================================*/
#define SOAD_START_SEC_CODE
#include "SoAd_MemMap.h"
static FUNC(void, SOAD_CODE) SoAd_SocnTransmitAutomaticSoConSetupHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(TcpIp_ProtocolType, AUTOMATIC) Protocol)
{
    SoAd_uintx soConGroup;
    /* socket connection of type automatic (i.e.
    configuration parameter SoAdSocketAutomaticSoConSetup set to TRUE)
    which uses a wildcard in the configured remote address */
    soConGroup = SOAD_SOCON_CFG(SoConId).SoAd_SocketConnectionGroupRef;
    if (SOAD_SOCONGROUP_CFG(soConGroup).SoAd_SocketAutomaticSoConSetup)
    {
        P2CONST(TcpIp_SockAddrType, TYPEDEF, SOAD_APPL_CONST)
        soConRemoteAdrCfgPtr = SOAD_SOCON_CFG(SoConId).SoAd_SocketRemoteAddress;
        if ((NULL_PTR != soConRemoteAdrCfgPtr)
            && ((SOAD_IS_IPADDR_ANY(*soConRemoteAdrCfgPtr)) || (TCPIP_PORT_ANY == soConRemoteAdrCfgPtr->port)))
        {
#if (STD_ON == SOAD_SUPPORT_TCP)
            if (TCPIP_IPPROTO_TCP == Protocol)
            {
                SoAd_SoConManager[SoConId].SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
                SoAd_SoConManager[SoConId].SetSocketNeedCloseLine = __LINE__;
#endif /* STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG */
            }
#endif /* STD_ON == SOAD_SUPPORT_TCP */
#if (STD_ON == SOAD_SUPPORT_UDP)
            if (TCPIP_IPPROTO_UDP == Protocol)
            {
                SoAd_SoConManager[SoConId].RemoteAddr = *soConRemoteAdrCfgPtr;
                SoAd_SoConManager[SoConId].SoAd_SoConMode = SOAD_SOCON_RECONNECT;
                SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, soConGroup);
            }
#endif /* STD_ON == SOAD_SUPPORT_UDP */
        }
    }
    return;
}

static FUNC(void, SOAD_CODE) SoAd_PduRouteManagerInit(PduIdType TxPduId)
{
    SoAd_PduRoutInnerManageType* soadPduRoutMPtr = &SoAd_PduRoutManager[TxPduId];

    soadPduRoutMPtr->PduDataPtr = NULL_PTR;
    soadPduRoutMPtr->PduLength = 0u;
    soadPduRoutMPtr->TxPendingNum = SOAD_UNUSED_UINT8;
    soadPduRoutMPtr->TxSoConId = NO_TXMETADATA;
    return;
}

static FUNC(void, SOAD_CODE) SoAd_RoutingGroupInit(void)
{
    SoAd_RoutingGroupIdType routeGroupId;
    SoAd_RoutingGroupIdType cnt0;

    /* init all RouteGroup enable state */
#if (0u < SOAD_ROUTING_GROUP_MAX)
    for (cnt0 = 0u; cnt0 < SOAD_ROUTING_GROUP_NUM; cnt0++)
    {
        SoAd_RoutGroupManager[cnt0].RoutGroupEnable = SOAD_ROUTGROUP_CFG(cnt0).SoAd_RoutingGroupIsEnabledAtInit;
    }
#endif /*0u < SOAD_ROUTING_GROUP_MAX*/

    /* init all SocketRouteDest enable state */
#if (0u < SOAD_SOCKET_ROUTE_DEST_MAX)
    SoAd_SocketRouteDestInnerManagerType* socketRoutDestMPtr;

    for (SoAd_uintx cnt1 = 0u; cnt1 < SOAD_SOCKET_ROUTE_DEST_NUM; cnt1++)
    {
        socketRoutDestMPtr = &SoAd_SocketRouteDestManager[cnt1];
        if (0u == SOAD_SRDEST_CFG(cnt1).SoAd_RxRoutingGroupRefNum)
        {
            socketRoutDestMPtr->SocketRouteDestEnable = TRUE;
        }
#if (0u < SOAD_ROUTING_GROUP_MAX)
        else
        {
            SoAd_RoutingGroupIdType rxRoutingGroupRefNum = SOAD_SRDEST_CFG(cnt1).SoAd_RxRoutingGroupRefNum;

            for (cnt0 = 0u; (cnt0 < rxRoutingGroupRefNum) && (FALSE == socketRoutDestMPtr->SocketRouteDestEnable);
                 cnt0++)
            {
                routeGroupId = SOAD_SRDEST_CFG(cnt1).SoAd_RxRoutingGroupRefPtr[cnt0];
                if (SoAd_RoutGroupManager[routeGroupId].RoutGroupEnable)
                {
                    socketRoutDestMPtr->SocketRouteDestEnable = TRUE;
                }
            }
        }
#endif /*0u < SOAD_ROUTING_GROUP_MAX*/
    }
#endif /*0u < SOAD_SOCKET_ROUTE_DEST_MAX*/

    /*init all PduRouteDest enable state*/
#if (0u < SOAD_PDU_ROUTE_DEST_MAX)
    SoAd_PduRouteDestInnerManagerType* pduRouteDestMPtr;
    SoAd_RoutingGroupIdType txRoutGroupRefNum;

    for (SoAd_uintx cnt2 = 0u; cnt2 < SOAD_PDU_ROUTE_DEST_NUM; cnt2++)
    {
        pduRouteDestMPtr = &SoAd_PduRouteDestManager[cnt2];
        pduRouteDestMPtr->TxPendingLength = 0u;
        if (0u == SOAD_PDUROTDEST_CFG(cnt2).SoAd_TxRoutingGroupRefNum)
        {
            SoAd_PduRouteDestControl(pduRouteDestMPtr, cnt2, TRUE, NO_TXMETADATA);
        }
#if (0u < SOAD_ROUTING_GROUP_MAX)
        else
        {
            txRoutGroupRefNum = SOAD_PDUROTDEST_CFG(cnt2).SoAd_TxRoutingGroupRefNum;

            SoAd_SoConIdType refSoConNum = SOAD_PDUROTDEST_CFG(cnt2).SoAd_TxPduRouteDestSocketConRefNum;
            boolean pduRouteDestEnable = TRUE;
            for (SoAd_SoConIdType cnt = 0u; cnt < refSoConNum; cnt++)
            {
                SoAd_SoConIdType soConId = SOAD_PDUROTDEST_CFG(cnt2).SoAd_TxSocketConRefIndexPtr[cnt];
                if (!SoAd_PduRouteDestIsEnable(cnt2, soConId))
                {
                    pduRouteDestEnable = FALSE;
                    break;
                }
            }

            for (cnt0 = 0u; (cnt0 < txRoutGroupRefNum) && (FALSE == pduRouteDestEnable); cnt0++)
            {
                routeGroupId = SOAD_PDUROTDEST_CFG(cnt2).SoAd_TxRoutingGroupRefPtr[cnt0];
                if (SoAd_RoutGroupManager[routeGroupId].RoutGroupEnable)
                {
                    SoAd_PduRouteDestControl(pduRouteDestMPtr, cnt2, TRUE, NO_TXMETADATA);
                }
            }
        }
#endif /*0u < SOAD_ROUTING_GROUP_MAX*/
    }
#endif /*0u < SOAD_PDU_ROUTE_DEST_MAX*/
}

#if (0u < SOAD_MAX_IF_PDU_SIZE)
static FUNC(Std_ReturnType, SOAD_CODE) SoAd_GetIfPduData(PduIdType TxPduId)
{
    Std_ReturnType result = E_NOT_OK;
    SoAd_UpLayerModuleType upModule;
    PduInfoType pduInfo;
#if (STD_ON == SOAD_PDU_ROUTE_METADATA_SUPPORT)
    uint8 metadata[2] = {0xFF, 0xFF};
#endif /* STD_ON == SOAD_PDU_ROUTE_METADATA_SUPPORT */
    const SoAd_PduRouteType* soadPduRouteCfgPtr = &SOAD_PDUROUT_CFG(TxPduId);

    upModule = soadPduRouteCfgPtr->SoAd_RefModule;
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
    if (NULL_PTR != SOAD_UPPERLAY_PCCFG(upModule).If_TriggerTransmit_FuncPtr)
    {
        pduInfo.SduDataPtr = &SoAd_IfPduBuffer[SOAD_HEADER_SIZE];
        pduInfo.SduLength = (SOAD_MAX_IF_PDU_SIZE - SOAD_HEADER_SIZE);
#if (STD_ON == SOAD_PDU_ROUTE_METADATA_SUPPORT)
        if (soadPduRouteCfgPtr->SoAd_MetaDataEnable)
        {
            pduInfo.MetaDataPtr = &metadata[0];
        }
        else
#endif /* STD_ON == SOAD_PDU_ROUTE_METADATA_SUPPORT */
        {
            pduInfo.MetaDataPtr = NULL_PTR;
        }
        result = SOAD_UPPERLAY_PCCFG(upModule).If_TriggerTransmit_FuncPtr(soadPduRouteCfgPtr->SoAd_UpPduId, &pduInfo);
    }
#endif /* SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u */
    SoAd_PduRoutInnerManageType* soadPduRoutMPtr = &SoAd_PduRoutManager[TxPduId];
    if (E_OK == result)
    {
        soadPduRoutMPtr->PduDataPtr = &SoAd_IfPduBuffer[SOAD_HEADER_SIZE];
        soadPduRoutMPtr->PduLength = pduInfo.SduLength;
#if (STD_ON == SOAD_PDU_ROUTE_METADATA_SUPPORT)
        if (soadPduRouteCfgPtr->SoAd_MetaDataEnable)
        {
            SOAD_METADATATOSOCONID(soadPduRoutMPtr->TxSoConId, pduInfo.MetaDataPtr);
        }
        else
#endif /* STD_ON == SOAD_PDU_ROUTE_METADATA_SUPPORT */
        {
            soadPduRoutMPtr->TxSoConId = NO_TXMETADATA;
        }
    }
    else
    {
        soadPduRoutMPtr->PduDataPtr = NULL_PTR;
    }
    return result;
}
#endif /*0u < SOAD_MAX_IF_PDU_SIZE*/

static FUNC(boolean, SOAD_CODE) SoAd_SoConRemoteAddrLocked(SoAd_SoConIdType SoConId)
{
    boolean remoteAddrLocked = FALSE;
#if ((0u < SOAD_RXBUFFER_NUM) || (0u < SOAD_TXBUFFER_NUM))
    SoAd_uintx bufferId;
#endif
    /*SWS_SoAd_00532*/
    if (SOAD_SOCON_OFFLINE == SoAd_SoConManager[SoConId].SoAd_SoConMode)
    {
        remoteAddrLocked = FALSE;
    }
    else if (TCPIP_IPPROTO_UDP == SOAD_SOCON_CFG(SoConId).SoAd_TcpUdpProtocol)
    {
        if (SoAd_SoConManager[SoConId].RxIfProcessing)
        {
            remoteAddrLocked = TRUE;
        }
#if (0u < SOAD_RXBUFFER_NUM)
        /*if the Rx PDU is Receiving(UDP),not set the remote addr*/
        if ((NULL_PTR != SOAD_SOCON_CFG(SoConId).SoAd_RxBufCfgIndexPtr) && !remoteAddrLocked)
        {
            bufferId = (SOAD_SOCON_CFG(SoConId).SoAd_RxBufCfgIndexPtr[0u] - SOAD_TXBUFFER_NUM);
            /*the Rx Buffer is empty and the Residue Rx Pdu length is 0*/
            if ((0u < SoAd_SoRxBufMan[bufferId].ExitPduResidueLength)
                || (SoAd_SoRxBufMan[bufferId].EnterPosition != SoAd_SoRxBufMan[bufferId].ExitPosition))
            {
                remoteAddrLocked = TRUE;
            }
        }
#endif /*0u < SOAD_RXBUFFER_NUM*/
#if (0u < SOAD_TXBUFFER_NUM)
        /*if the Tx PDU is transmitting(UDP),not set the remote addr*/
        if ((NULL_PTR != SOAD_SOCON_CFG(SoConId).SoAd_TxBufCfgIndexPtr) && !remoteAddrLocked)
        {
            bufferId = SOAD_SOCON_CFG(SoConId).SoAd_TxBufCfgIndexPtr[0u];
            if (0u < SoAd_SoTxBufMan[bufferId].txPduTotalLength)
            {
                remoteAddrLocked = TRUE;
            }
        }
#endif /*0u < SOAD_TXBUFFER_NUM*/
    }
    else
    {
        remoteAddrLocked = TRUE;
    }
    return remoteAddrLocked;
}

static FUNC(void, SOAD_CODE) SoAd_ReleaseRemoteAddrHandle(SoAd_SoConIdType SoConId, SoAd_uintx SoConGroupId)
{
    SoAd_InnerSocketManageType* soadSoConMPtr = &SoAd_SoConManager[SoConId];

    if (NULL_PTR != SOAD_SOCON_CFG(SoConId).SoAd_SocketRemoteAddress)
    {
        soadSoConMPtr->RemoteAddrIsSet = TRUE;
        soadSoConMPtr->RemoteAddr = *SOAD_SOCONREMOTEADDR_CFG(SoConId);
    }
    /*Standard undefined,release to wildcards*/
    else
    {
        soadSoConMPtr->RemoteAddr.addr[0u] = 0x00000000u;
        soadSoConMPtr->RemoteAddr.port = TCPIP_PORT_ANY;
    }
    /*SWS_SoAd_00687*/
    if ((SOAD_SOCON_ONLINE == soadSoConMPtr->SoAd_SoConMode)
        && (SOAD_IS_IPADDR_ANY(soadSoConMPtr->RemoteAddr) || (TCPIP_PORT_ANY == soadSoConMPtr->RemoteAddr.port)))
    {
        soadSoConMPtr->SoAd_SoConMode = SOAD_SOCON_RECONNECT;
        SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, SoConGroupId);
    }
    return;
}

static FUNC(void, SOAD_CODE) SoAd_ReleaseRemoteAddrMainFunctionHandle(SoAd_SoConIdType SoConId)
{
    SoAd_uintx soConGroup = SOAD_SOCON_CFG(SoConId).SoAd_SocketConnectionGroupRef;
    SoAd_InnerSocketManageType* soadSoConMPtr = &SoAd_SoConManager[SoConId];

    if (soadSoConMPtr->RemoteAddrNeedRelease)
    {
        if (FALSE == SoAd_SoConRemoteAddrLocked(SoConId))
        {
            SoAd_ReleaseRemoteAddrHandle(SoConId, soConGroup);
            soadSoConMPtr->RemoteAddrNeedRelease = FALSE;
        }
    }
    return;
}
#if (0u < SOAD_SOCKET_ROUTE_DEST_MAX)
#if (0u < SOAD_RXBUFFER_NUM)

#if (STD_ON == SOAD_SUPPORT_HEADERID)
static FUNC(void, SOAD_CODE)
    SoAd_HeaderEnableNewPduHandle(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId, VAR(SoAd_uintx, AUTOMATIC) BufferManageId)
{
    PduLengthType bufferedDataLength, bufStartId;
    PduLengthType bufferLength, bufferStartPos;
    boolean findValidSocketRoute;
    uint32 header;
    PduLengthType pduLength;
    SoAd_uintx socketRouteId;
    PduLengthType upBufferSize;
    SoAd_UpLayerModuleType upModule;
    PduInfoType pduInfo;
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
    uint8 metaData[2u];
#endif /* STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT */
    SoAd_SoRxBufferManType* soadSoRxBuffMPtr = &SoAd_SoRxBufMan[BufferManageId];
    PduLengthType enterPos = soadSoRxBuffMPtr->EnterPosition;
    PduLengthType exitPos = soadSoRxBuffMPtr->ExitPosition;

    bufferLength = SOAD_SOCKETBUFFER_CFG(BufferManageId + SOAD_TXBUFFER_NUM).bufLen;
    bufferStartPos = SOAD_SOCKETBUFFER_CFG(BufferManageId + SOAD_TXBUFFER_NUM).bufPos;
    if (exitPos < enterPos)
    {
        bufferedDataLength = enterPos - exitPos;
    }
    else
    {
        bufferedDataLength = (bufferLength + enterPos - exitPos);
    }
    if (bufferedDataLength >= SOAD_HEADER_SIZE)
    {
        PduLengthType destHeaderLocation;
        findValidSocketRoute = FALSE;
        bufStartId = bufferStartPos + exitPos;
        destHeaderLocation = exitPos + SOAD_HEADER_SIZE;
        /*the header data is overturn of the buffer*/
        if (destHeaderLocation > bufferLength)
        {
            uint8 headerData[8];
            PduLengthType copyLen;
            destHeaderLocation -= bufferLength;
            copyLen = SOAD_HEADER_SIZE - destHeaderLocation;
            (void)ILib_memcpy(&headerData[0u], &SoAd_PduBuffer[bufStartId], copyLen);
            (void)ILib_memcpy(&headerData[copyLen], &SoAd_PduBuffer[bufferStartPos], destHeaderLocation);
            header = SOAD_GET_HEADER(headerData, 0u);
            pduLength = (PduLengthType)(SOAD_GET_LENGTH(headerData, 0u));
        }
        else
        {
            header = SOAD_GET_HEADER(SoAd_PduBuffer, bufStartId);
            pduLength = (PduLengthType)(SOAD_GET_LENGTH(SoAd_PduBuffer, bufStartId));
        }

        findValidSocketRoute = SoAd_FindHeaderIDAndSocketRouteId(SoConId, header, &socketRouteId);
        /*find the valid socket route(dest),and the socket route dest is enabled(according to the RouteGroup Manager)*/
        if (findValidSocketRoute && SoAd_SocketRouteDestManager[socketRouteId].SocketRouteDestEnable)
        {
            soadSoRxBuffMPtr->SoAd_SocketRouteId = socketRouteId;
            soadSoRxBuffMPtr->ExitPduResidueLength = pduLength;
            exitPos += SOAD_HEADER_SIZE;
            if (exitPos >= bufferLength)
            {
                exitPos -= bufferLength;
            }
            soadSoRxBuffMPtr->ExitPosition = exitPos;
            if (TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(SoConId).SoAd_TcpUdpProtocol)
            {
                (void)TcpIp_TcpReceived(SoAd_SoConManager[SoConId].TcpIpSocketId, SOAD_HEADER_SIZE);
            }
#if (0u < SOAD_MAX_IF_PDU_SIZE)
            /*IF PDU data are in the SoAd Rx Buffer,just is TCP(Header Enable)*/
            if (SOAD_UPPER_LAYER_IF == SOAD_SRDEST_CFG(socketRouteId).SoAd_RxUpperLayerType)
            {
                SoAd_HeaderEnableIfPduHandle(SoConId, BufferManageId);
            }
            else
#endif /*0u < SOAD_MAX_IF_PDU_SIZE*/
            {
                if (pduLength > 0u)
                {
                    pduInfo.SduDataPtr = NULL_PTR;
                    pduInfo.SduLength = 0u;
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
                    if (SOAD_SRDEST_CFG(socketRouteId).SoAd_MetaDataEnable)
                    {
                        SoAd_MetaDataFillHandle(SoConId, &metaData[0u]);
                        pduInfo.MetaDataPtr = &metaData[0u];
                    }
                    else
#endif /* STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT */
                    {
                        pduInfo.MetaDataPtr = NULL_PTR;
                    }
                    upModule = SOAD_SRDEST_CFG(socketRouteId).SoAd_RefModule;
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
                    if (BUFREQ_OK
                        == SOAD_UPPERLAY_PCCFG(upModule).Tp_StartOfReception_FuncPtr(
                            SOAD_SRDEST_CFG(socketRouteId).SoAd_UpPduId,
                            &pduInfo,
                            pduLength,
                            &upBufferSize))
                    {
                        SoAd_HeaderEnableTpPduHandle(SoConId, upBufferSize);
                    }
                    else
                    {
                        SoAd_SoConManager[SoConId].SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
                        SoAd_SoConManager[SoConId].SetSocketNeedCloseLine = __LINE__;
#endif /* STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG */
                    }
#endif /* SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u */
                }
            }
        }
        else
        {
            if (bufferedDataLength >= (pduLength + SOAD_HEADER_SIZE))
            {
                soadSoRxBuffMPtr->ExitPosition += (pduLength + SOAD_HEADER_SIZE);
                if (soadSoRxBuffMPtr->ExitPosition >= bufferLength)
                {
                    soadSoRxBuffMPtr->ExitPosition -= bufferLength;
                }
                if (TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(SoConId).SoAd_TcpUdpProtocol)
                {
                    (void)TcpIp_TcpReceived(
                        SoAd_SoConManager[SoConId].TcpIpSocketId,
                        (uint32)pduLength + (uint32)SOAD_HEADER_SIZE);
                }
                (void)Det_ReportRuntimeError(
                    SOAD_MODULE_ID,
                    SOAD_INSTANCE,
                    SOAD_SID_MAINFUNCTION,
                    SOAD_E_INV_PDUHEADER_ID);
            }

            /*received TCP FIN, never receive data*/
            if ((SoAd_SoConManager[SoConId].SocketNeedClose == SOAD_SOCKET_CLOSE_DEFERRED)
                && (TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(SoConId).SoAd_TcpUdpProtocol))
            {
                soadSoRxBuffMPtr->ExitPosition = soadSoRxBuffMPtr->EnterPosition;
            }
        }
    }
    else
    {
        /*received TCP FIN, never receive data*/
        if ((SoAd_SoConManager[SoConId].SocketNeedClose == SOAD_SOCKET_CLOSE_DEFERRED)
            && (TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(SoConId).SoAd_TcpUdpProtocol))
        {
            soadSoRxBuffMPtr->ExitPosition = soadSoRxBuffMPtr->EnterPosition;
        }
    }
    return;
}

static FUNC(void, SOAD_CODE)
    SoAd_HeaderEnableOldPduHandle(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId, VAR(SoAd_uintx, AUTOMATIC) BufferManageId)
{
    SoAd_uintx socketRouteId;
    PduInfoType pduInfo;
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
    uint8 metaData[2u];
#endif /* STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT */
    PduLengthType upBufferSize;
    SoAd_UpLayerModuleType upModule;

    socketRouteId = SoAd_SoRxBufMan[BufferManageId].SoAd_SocketRouteId;
    upModule = SOAD_SRDEST_CFG(socketRouteId).SoAd_RefModule;
#if (0u < SOAD_MAX_IF_PDU_SIZE)
    if (SOAD_UPPER_LAYER_IF == SOAD_SRDEST_CFG(socketRouteId).SoAd_RxUpperLayerType)
    {
        SoAd_HeaderEnableIfPduHandle(SoConId, BufferManageId);
    }
    else
#endif /*0u < SOAD_MAX_IF_PDU_SIZE*/
    {
        pduInfo.SduDataPtr = NULL_PTR;
        pduInfo.SduLength = 0u;
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
        if (SOAD_SRDEST_CFG(socketRouteId).SoAd_MetaDataEnable)
        {
            SoAd_MetaDataFillHandle(SoConId, &metaData[0u]);
            pduInfo.MetaDataPtr = &metaData[0u];
        }
        else
#endif /* STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT */
        {
            pduInfo.MetaDataPtr = NULL_PTR;
        }
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
        if (BUFREQ_OK
            == SOAD_UPPERLAY_PCCFG(upModule)
                   .Tp_CopyRxData_FuncPtr(SOAD_SRDEST_CFG(socketRouteId).SoAd_UpPduId, &pduInfo, &upBufferSize))
        {
            SoAd_HeaderEnableTpPduHandle(SoConId, upBufferSize);
        }
        else
        {
            SOAD_UPPERLAY_PCCFG(upModule).Tp_RxIndication_FuncPtr(
                SOAD_SRDEST_CFG(socketRouteId).SoAd_UpPduId,
                E_NOT_OK);
            SoAd_SoConManager[SoConId].SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
            SoAd_SoConManager[SoConId].SetSocketNeedCloseLine = __LINE__;
#endif /* STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG */
        }
#endif /* SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u */
    }
    return;
}

static FUNC(void, SOAD_CODE)
    SoAd_HeaderEnableTpPduHandle(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId, VAR(PduLengthType, AUTOMATIC) UpBufferSize)
{
    PduLengthType copyToUpLength;
    SoAd_uintx bufferId;
    PduLengthType bufferLength;
    PduLengthType bufferStartPos;
    SoAd_uintx socketRouteId;
    SoAd_UpLayerModuleType upModule;
    PduInfoType pduInfo;
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
    uint8 metaData[2];
#endif /* STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT */
    PduLengthType upBufferSize;
    const SoAd_SocketRouteDestType* saodSocketRoutDestCfgPtr;

    bufferId = SOAD_SOCON_CFG(SoConId).SoAd_RxBufCfgIndexPtr[0u];
    bufferLength = SOAD_SOCKETBUFFER_CFG(bufferId).bufLen;
    bufferStartPos = SOAD_SOCKETBUFFER_CFG(bufferId).bufPos;
    bufferId = bufferId - SOAD_TXBUFFER_NUM;
    SoAd_SoRxBufferManType* soadSoRxBuffMPtr = &SoAd_SoRxBufMan[bufferId];

    socketRouteId = soadSoRxBuffMPtr->SoAd_SocketRouteId;
    saodSocketRoutDestCfgPtr = &SOAD_SRDEST_CFG(socketRouteId);
    upModule = saodSocketRoutDestCfgPtr->SoAd_RefModule;

    if (soadSoRxBuffMPtr->ExitPosition < soadSoRxBuffMPtr->EnterPosition)
    {
        copyToUpLength = soadSoRxBuffMPtr->EnterPosition - soadSoRxBuffMPtr->ExitPosition;
    }
    else
    {
        copyToUpLength = bufferLength - soadSoRxBuffMPtr->ExitPosition;
    }
    if (copyToUpLength > UpBufferSize)
    {
        copyToUpLength = UpBufferSize;
    }
    if (copyToUpLength >= soadSoRxBuffMPtr->ExitPduResidueLength)
    {
        copyToUpLength = soadSoRxBuffMPtr->ExitPduResidueLength;
    }
    if (copyToUpLength > 0u)
    {
        pduInfo.SduDataPtr = &SoAd_PduBuffer[bufferStartPos + soadSoRxBuffMPtr->ExitPosition];
        pduInfo.SduLength = copyToUpLength;
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
        if (saodSocketRoutDestCfgPtr->SoAd_MetaDataEnable)
        {
            SoAd_MetaDataFillHandle(SoConId, &metaData[0u]);
            pduInfo.MetaDataPtr = &metaData[0u];
        }
        else
#endif /* STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT */
        {
            pduInfo.MetaDataPtr = NULL_PTR;
        }
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
        PduIdType upPduId = saodSocketRoutDestCfgPtr->SoAd_UpPduId;

        if (BUFREQ_OK == SOAD_UPPERLAY_PCCFG(upModule).Tp_CopyRxData_FuncPtr(upPduId, &pduInfo, &upBufferSize))
        {
            if (TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(SoConId).SoAd_TcpUdpProtocol)
            {
                /*SWS_SoAd_00564:SoAd shall confirm the reception of all data*/
                (void)TcpIp_TcpReceived(SoAd_SoConManager[SoConId].TcpIpSocketId, copyToUpLength);
            }
            soadSoRxBuffMPtr->ExitPosition += copyToUpLength;
            if (soadSoRxBuffMPtr->ExitPosition >= bufferLength)
            {
                soadSoRxBuffMPtr->ExitPosition -= bufferLength;
            }
            soadSoRxBuffMPtr->ExitPduResidueLength -= copyToUpLength;
            if (0u == soadSoRxBuffMPtr->ExitPduResidueLength)
            {
                SOAD_UPPERLAY_PCCFG(upModule).Tp_RxIndication_FuncPtr(upPduId, E_OK);
            }
        }
        else
        {
            SOAD_UPPERLAY_PCCFG(upModule).Tp_RxIndication_FuncPtr(upPduId, E_NOT_OK);
            SoAd_SoConManager[SoConId].SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
            SoAd_SoConManager[SoConId].SetSocketNeedCloseLine = __LINE__;
#endif /* STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG */
        }
#endif /* SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u */
    }
    return;
}
#endif /*STD_ON == SOAD_SUPPORT_HEADERID*/

#if (0u < SOAD_MAX_IF_PDU_SIZE)
#if (STD_ON == SOAD_SUPPORT_HEADERID)
static FUNC(void, SOAD_CODE)
    SoAd_HeaderEnableIfPduHandle(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId, VAR(SoAd_uintx, AUTOMATIC) BufferManageId)
{
    PduLengthType srcPos;
    PduLengthType destPos;
    PduLengthType bufferedDataLength;
    PduLengthType bufferLength;
    PduLengthType bufferStartPos;
    SoAd_uintx socketRouteId;
    SoAd_UpLayerModuleType upModule;
    PduInfoType pduInfo;
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
    uint8 metaData[2];
#endif /* STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT */
    SoAd_SoRxBufferManType* soadSoRxBuffMPtr = &SoAd_SoRxBufMan[BufferManageId];
    PduLengthType enterPos = soadSoRxBuffMPtr->EnterPosition;
    PduLengthType exitPos = soadSoRxBuffMPtr->ExitPosition;
    PduLengthType exitPduResidueLen = soadSoRxBuffMPtr->ExitPduResidueLength;
    const SoAd_SocketRouteDestType* saodSocketRoutDestCfgPtr;

    bufferLength = SOAD_SOCKETBUFFER_CFG(BufferManageId + SOAD_TXBUFFER_NUM).bufLen;
    bufferStartPos = SOAD_SOCKETBUFFER_CFG(BufferManageId + SOAD_TXBUFFER_NUM).bufPos;
    socketRouteId = soadSoRxBuffMPtr->SoAd_SocketRouteId;
    saodSocketRoutDestCfgPtr = &SOAD_SRDEST_CFG(socketRouteId);
    upModule = saodSocketRoutDestCfgPtr->SoAd_RefModule;

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
        srcPos = bufferStartPos + exitPos;
        destPos = exitPos + exitPduResidueLen;
        pduInfo.SduLength = exitPduResidueLen;
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
        if (saodSocketRoutDestCfgPtr->SoAd_MetaDataEnable)
        {
            SoAd_MetaDataFillHandle(SoConId, &metaData[0u]);
            pduInfo.MetaDataPtr = &metaData[0u];
        }
        else
#endif /* STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT */
        {
            pduInfo.MetaDataPtr = NULL_PTR;
        }
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
        PduIdType upPduId = saodSocketRoutDestCfgPtr->SoAd_UpPduId;

        if (destPos > bufferLength)
        {
            SchM_Enter_SoAd_ExclusiveArea();
            /*SoAd_IfPduBuffer is used for IF TCP receive buffer*/
            PduLengthType copyLength = bufferLength - exitPos;
            (void)ILib_memcpy(&SoAd_IfPduBuffer[0u], &SoAd_PduBuffer[srcPos], copyLength);
            (void)ILib_memcpy(
                &SoAd_IfPduBuffer[copyLength],
                &SoAd_PduBuffer[bufferStartPos],
                (PduLengthType)(destPos - bufferLength));
            pduInfo.SduDataPtr = &SoAd_IfPduBuffer[0u];
            SOAD_UPPERLAY_PCCFG(upModule).If_RxIndication_FuncPtr(upPduId, &pduInfo);
            SchM_Exit_SoAd_ExclusiveArea();
        }
        else
        {
            pduInfo.SduDataPtr = &SoAd_PduBuffer[srcPos];
            SOAD_UPPERLAY_PCCFG(upModule).If_RxIndication_FuncPtr(upPduId, &pduInfo);
        }
#endif /* SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u */
        /*IF PDU used Rx Buffer must be TCP.
        SWS_SoAd_00564:SoAd shall confirm the reception of all data*/
        (void)TcpIp_TcpReceived(SoAd_SoConManager[SoConId].TcpIpSocketId, exitPduResidueLen);
        soadSoRxBuffMPtr->ExitPosition = destPos;
        if (soadSoRxBuffMPtr->ExitPosition >= bufferLength)
        {
            soadSoRxBuffMPtr->ExitPosition -= bufferLength;
        }
        soadSoRxBuffMPtr->ExitPduResidueLength = 0u;
    }
    return;
}
#endif /*STD_ON == SOAD_SUPPORT_HEADERID*/
#endif /*0u < SOAD_MAX_IF_PDU_SIZE*/

static FUNC(void, SOAD_CODE) SoAd_RxDataMainFunctionHeaderDisableHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(SoAd_uintx, AUTOMATIC) BufferManageId)
{
    SoAd_uintx socketRouteId;
    PduInfoType pduInfo;
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
    uint8 metaData[2u];
#endif /* STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT */
    SoAd_UpLayerModuleType upModule;
    PduLengthType upBufferSize;
    PduLengthType copyToUpLength;
    PduLengthType bufferLength;
    PduLengthType bufferStartPos;
    SoAd_SoRxBufferManType* soadSoRxBuffMPtr = &SoAd_SoRxBufMan[BufferManageId];
    const SoAd_SocketRouteDestType* saodSocketRoutDestCfgPtr;

    bufferLength = SOAD_SOCKETBUFFER_CFG(BufferManageId + SOAD_TXBUFFER_NUM).bufLen;
    bufferStartPos = SOAD_SOCKETBUFFER_CFG(BufferManageId + SOAD_TXBUFFER_NUM).bufPos;
    socketRouteId = SOAD_SOCON_CFG(SoConId).SoAd_RefSocketRoutePtr[0u];
    saodSocketRoutDestCfgPtr = &SOAD_SRDEST_CFG(socketRouteId);
    upModule = saodSocketRoutDestCfgPtr->SoAd_RefModule;
    pduInfo.SduDataPtr = NULL_PTR;
    pduInfo.SduLength = 0u;
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
    if (saodSocketRoutDestCfgPtr->SoAd_MetaDataEnable)
    {
        SoAd_MetaDataFillHandle(SoConId, &metaData[0u]);
        pduInfo.MetaDataPtr = &metaData[0u];
    }
    else
#endif /* STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT */
    {
        pduInfo.MetaDataPtr = NULL_PTR;
    }
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
    PduIdType upPduId = saodSocketRoutDestCfgPtr->SoAd_UpPduId;

    if (BUFREQ_OK == SOAD_UPPERLAY_PCCFG(upModule).Tp_CopyRxData_FuncPtr(upPduId, &pduInfo, &upBufferSize))
    {
        if (soadSoRxBuffMPtr->ExitPosition < soadSoRxBuffMPtr->EnterPosition)
        {
            copyToUpLength = soadSoRxBuffMPtr->EnterPosition - soadSoRxBuffMPtr->ExitPosition;
        }
        else
        {
            copyToUpLength = (bufferLength - soadSoRxBuffMPtr->ExitPosition);
        }
        if (copyToUpLength > upBufferSize)
        {
            copyToUpLength = upBufferSize;
        }
        if (copyToUpLength > 0u)
        {
            pduInfo.SduDataPtr = &SoAd_PduBuffer[bufferStartPos + soadSoRxBuffMPtr->ExitPosition];
            pduInfo.SduLength = copyToUpLength;
            if (BUFREQ_OK == SOAD_UPPERLAY_PCCFG(upModule).Tp_CopyRxData_FuncPtr(upPduId, &pduInfo, &upBufferSize))
            {
                if (TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(SoConId).SoAd_TcpUdpProtocol)
                {
                    /*SWS_SoAd_00564:SoAd shall confirm the reception of all data*/
                    (void)TcpIp_TcpReceived(SoAd_SoConManager[SoConId].TcpIpSocketId, copyToUpLength);
                }
                soadSoRxBuffMPtr->ExitPosition += copyToUpLength;
                if (soadSoRxBuffMPtr->ExitPosition >= bufferLength)
                {
                    soadSoRxBuffMPtr->ExitPosition -= bufferLength;
                }
            }
            else
            {
                SoAd_SoConManager[SoConId].SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
                SoAd_SoConManager[SoConId].SetSocketNeedCloseLine = __LINE__;
#endif /* STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG */
            }
        }
    }
    else
    {
        SoAd_SoConManager[SoConId].SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
        SoAd_SoConManager[SoConId].SetSocketNeedCloseLine = __LINE__;
#endif /* STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG */
    }
#endif /* SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u */
    return;
}

static FUNC(void, SOAD_CODE) SoAd_SoConRxDataMainFunctionHandle(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId)
{
    SoAd_uintx bufferId;
#if (STD_ON == SOAD_SUPPORT_HEADERID)
    boolean headerEnabled;
    uint16 soConGroupId;
#endif /*STD_ON == SOAD_SUPPORT_HEADERID*/
    const SoAd_SoRxBufferManType* soadSoRxBuffMPtr;
    const SoAd_SocketConnectionType* soadSoConCfgPtr = &SOAD_SOCON_CFG(SoConId);

    if ((NULL_PTR != soadSoConCfgPtr->SoAd_RxBufCfgIndexPtr) && (NULL_PTR != soadSoConCfgPtr->SoAd_RefSocketRoutePtr))
    {
        bufferId = soadSoConCfgPtr->SoAd_RxBufCfgIndexPtr[0u];
        bufferId = bufferId - SOAD_TXBUFFER_NUM;
        soadSoRxBuffMPtr = &SoAd_SoRxBufMan[bufferId];
        /*the EnterPosition is not equal to ExitPosition:the Rx Buffer have store receive data*/
        if (soadSoRxBuffMPtr->EnterPosition != soadSoRxBuffMPtr->ExitPosition)
        {
#if (STD_ON == SOAD_SUPPORT_HEADERID)
            soConGroupId = soadSoConCfgPtr->SoAd_SocketConnectionGroupRef;
            headerEnabled = SOAD_SOCONGROUP_CFG(soConGroupId).SoAd_PduHeaderEnable;
            /*header enabled handle*/
            if (headerEnabled)
            {
                /*need to analysis the newly PDU header*/
                if (0u == soadSoRxBuffMPtr->ExitPduResidueLength)
                {
                    SoAd_HeaderEnableNewPduHandle(SoConId, bufferId);
                }
                /*don't need to analysis the newly PDU header*/
                else
                {
                    SoAd_HeaderEnableOldPduHandle(SoConId, bufferId);
                }
            }
            /*header disabled handle:just TP PDU may use the Rx Buffer*/
            else
#endif /*STD_ON == SOAD_SUPPORT_HEADERID*/
            {
                SoAd_RxDataMainFunctionHeaderDisableHandle(SoConId, bufferId);
            }
        }
        else
        {
            if (SoAd_SoConManager[SoConId].SocketNeedClose == SOAD_SOCKET_CLOSE_DEFERRED)
            {
                SoAd_SoConManager[SoConId].SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
                SoAd_SoConManager[SoConId].SetSocketNeedCloseLine = __LINE__;
#endif /* STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG */
            }
        }
    }
    return;
}
#endif /*0u < SOAD_RXBUFFER_NUM*/
#endif /*0u < SOAD_SOCKET_ROUTE_DEST_MAX)*/
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
static FUNC(void, SOAD_CODE)
    SoAd_MetaDataFillHandle(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId, P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) DataPtr)
{
#if (0x100u > SOAD_SO_CON_MAX)
    DataPtr[0u] = SoConId;
#else  /*0x100u > SOAD_SO_CON_MAX*/
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
#endif /*0x100u > SOAD_SO_CON_MAX*/
    return;
}
#endif /* STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT */

#if (0u < SOAD_SOCKET_ROUTE_DEST_MAX)
#if (STD_ON == SOAD_SUPPORT_UDP)
#if (STD_ON == SOAD_SUPPORT_HEADERID)
#if (0u < SOAD_RXBUFFER_NUM)
static FUNC(boolean, SOAD_CODE) SoAd_HeaderEnableUdpSoConRxTpPduHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(SoAd_uintx, AUTOMATIC) SocketRouteId,
    P2VAR(PduLengthType, AUTOMATIC, SOAD_APPL_DATA) DataPosition,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) DataPtr)
{
    boolean validPduData = TRUE;
    SoAd_uintx bufferId;
    PduInfoType pduInfo;
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
    uint8 metaData[2];
#endif /* STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT */
    SoAd_UpLayerModuleType upModule;
    PduLengthType upBufferSize;
    PduLengthType pduLength;
    SoAd_SoRxBufferManType* soadSoRxBuffMPtr;
    const SoAd_SocketRouteDestType* saodSocketRoutDestCfgPtr = &SOAD_SRDEST_CFG(SocketRouteId);
    PduIdType upPduId = saodSocketRoutDestCfgPtr->SoAd_UpPduId;

    pduLength = (PduLengthType)(SOAD_GET_LENGTH(DataPtr, (*DataPosition)));
    if (0u < pduLength)
    {
        bufferId = *SOAD_SOCON_CFG(SoConId).SoAd_RxBufCfgIndexPtr;
        bufferId = bufferId - SOAD_TXBUFFER_NUM;
        soadSoRxBuffMPtr = &SoAd_SoRxBufMan[bufferId];
        upModule = saodSocketRoutDestCfgPtr->SoAd_RefModule;
        /*check the Rx Buffer is empty or not*/
        if ((soadSoRxBuffMPtr->EnterPosition == soadSoRxBuffMPtr->ExitPosition)
            && (0u == soadSoRxBuffMPtr->ExitPduResidueLength))
        {
            pduInfo.SduDataPtr = NULL_PTR;
            pduInfo.SduLength = 0;
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
            if (saodSocketRoutDestCfgPtr->SoAd_MetaDataEnable)
            {
                SoAd_MetaDataFillHandle(SoConId, &metaData[0u]);
                pduInfo.MetaDataPtr = &metaData[0u];
            }
            else
#endif /* STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT */
            {
                pduInfo.MetaDataPtr = NULL_PTR;
            }
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
            if (BUFREQ_OK
                == SOAD_UPPERLAY_PCCFG(upModule)
                       .Tp_StartOfReception_FuncPtr(upPduId, &pduInfo, pduLength, &upBufferSize))
            {
                if (upBufferSize >= pduLength)
                {
                    pduInfo.SduDataPtr = &DataPtr[(*DataPosition) + SOAD_HEADER_SIZE];
                    pduInfo.SduLength = pduLength;
                    if (BUFREQ_OK
                        == SOAD_UPPERLAY_PCCFG(upModule).Tp_CopyRxData_FuncPtr(upPduId, &pduInfo, &upBufferSize))
                    {
                        SOAD_UPPERLAY_PCCFG(upModule).Tp_RxIndication_FuncPtr(upPduId, E_OK);
                    }
                    else
                    {
                        SOAD_UPPERLAY_PCCFG(upModule).Tp_RxIndication_FuncPtr(upPduId, E_NOT_OK);
                    }
                }
                else if (upBufferSize > 0u)
                {
                    pduInfo.SduDataPtr = &DataPtr[(*DataPosition) + SOAD_HEADER_SIZE];
                    pduInfo.SduLength = upBufferSize;

                    if (BUFREQ_OK
                        == SOAD_UPPERLAY_PCCFG(upModule).Tp_CopyRxData_FuncPtr(upPduId, &pduInfo, &upBufferSize))
                    {
                        /*copy the remainder data(Length-pduInfo.SduLength) to SoAd Rx Buffer */
                        if (TRUE
                            == SoAd_CopyDataToRxBuffer(
                                SoConId,
                                (pduLength - pduInfo.SduLength),
                                &DataPtr[(*DataPosition) + SOAD_HEADER_SIZE + pduInfo.SduLength]))
                        {
                            soadSoRxBuffMPtr->ExitPduResidueLength = pduLength - pduInfo.SduLength;
                            soadSoRxBuffMPtr->SoAd_SocketRouteId = SocketRouteId;
                        }
                    }
                    else
                    {
                        SOAD_UPPERLAY_PCCFG(upModule).Tp_RxIndication_FuncPtr(upPduId, E_NOT_OK);
                    }
                }
                else
                {
                    /*copy the remainder data(Length) to SoAd Rx Buffer */
                    if (TRUE
                        == SoAd_CopyDataToRxBuffer(SoConId, pduLength, &DataPtr[(*DataPosition) + SOAD_HEADER_SIZE]))
                    {
                        soadSoRxBuffMPtr->ExitPduResidueLength = pduLength;
                        soadSoRxBuffMPtr->SoAd_SocketRouteId = SocketRouteId;
                    }
                }
            }
#endif /* SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u */
        }
        else
        {
            /*copy the data(Length+8) to Rx Buffer*/
            (void)SoAd_CopyDataToRxBuffer(SoConId, (SOAD_HEADER_SIZE + pduLength), &DataPtr[*DataPosition]);
        }
    }
    else
    {
        validPduData = FALSE;
    }

    *DataPosition += (SOAD_HEADER_SIZE + pduLength);
    return validPduData;
}
#endif /*0u < SOAD_RXBUFFER_NUM*/

static FUNC(boolean, SOAD_CODE) SoAd_HeaderEnableUdpSoConRxHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(uint16, AUTOMATIC) RxMessageLength,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) DataPtr)
{
    boolean validPduData = TRUE;
    SoAd_uintx soConGroupId;
    boolean findValidSocketRoute;
    uint32 header;
    SoAd_UpLayerModuleType upModule;
    PduInfoType pduInfo;
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
    uint8 metaData[2];
#endif /* STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT */
    SoAd_uintx socketRouteId;
    PduLengthType dataPosition = 0u;
    PduLengthType pduLength;
    const SoAd_SocketRouteDestType* soadSoRouteDestPtr;
    PduLengthType remainLen;

    soConGroupId = SOAD_SOCON_CFG(SoConId).SoAd_SocketConnectionGroupRef;
    if (SoAd_UdpStrictHeaderLenCheck(soConGroupId, DataPtr, RxMessageLength))
    {
        while (dataPosition < RxMessageLength)
        {
            findValidSocketRoute = FALSE;
            header = SOAD_GET_HEADER(DataPtr, dataPosition);
            pduLength = (PduLengthType)(SOAD_GET_LENGTH(DataPtr, dataPosition));
            remainLen = dataPosition + SOAD_HEADER_SIZE + pduLength;
            if (remainLen <= RxMessageLength)
            {
                findValidSocketRoute = SoAd_FindHeaderIDAndSocketRouteId(SoConId, header, &socketRouteId);
                /*findValidSocketRoute and socket dest routing group is enabled*/
                if (findValidSocketRoute && SoAd_SocketRouteDestManager[socketRouteId].SocketRouteDestEnable)
                {
                    soadSoRouteDestPtr = &SOAD_SRDEST_CFG(socketRouteId);
                    upModule = soadSoRouteDestPtr->SoAd_RefModule;
                    if (SOAD_UPPER_LAYER_IF == soadSoRouteDestPtr->SoAd_RxUpperLayerType)
                    {
                        pduInfo.SduDataPtr = &DataPtr[dataPosition + SOAD_HEADER_SIZE];
                        pduInfo.SduLength = pduLength;
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
                        if (soadSoRouteDestPtr->SoAd_MetaDataEnable)
                        {
                            SoAd_MetaDataFillHandle(SoConId, &metaData[0u]);
                            pduInfo.MetaDataPtr = &metaData[0u];
                        }
                        else
#endif /* STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT */
                        {
                            pduInfo.MetaDataPtr = NULL_PTR;
                        }
                        /* maybe Multiple header for one udp packet */
                        if ((remainLen + SOAD_HEADER_SIZE + pduLength) <= RxMessageLength)
                        {
                            SoAd_SoConManager[SoConId].RxIfProcessing = TRUE;
                        }
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
                        SOAD_UPPERLAY_PCCFG(upModule).If_RxIndication_FuncPtr(
                            soadSoRouteDestPtr->SoAd_UpPduId,
                            &pduInfo);
#endif /* SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u */
                        dataPosition = remainLen;
                    }
                    else
                    {
#if (0u < SOAD_RXBUFFER_NUM)
                        validPduData =
                            SoAd_HeaderEnableUdpSoConRxTpPduHandle(SoConId, socketRouteId, &dataPosition, DataPtr);
#endif /*0u < SOAD_RXBUFFER_NUM*/
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

        if (SoAd_SoConManager[SoConId].RxIfProcessing)
        {
            SoAd_SoConManager[SoConId].RxIfProcessing = FALSE;
        }
    }
    else
    {
        validPduData = FALSE;
    }

    return validPduData;
}
#endif /*STD_ON == SOAD_SUPPORT_HEADERID*/
#endif /* STD_ON == SOAD_SUPPORT_UDP */

#if (0u < SOAD_RXBUFFER_NUM)
#if (STD_ON == SOAD_SUPPORT_HEADERID)
static FUNC(void, SOAD_CODE) SoAd_HeaderEnableTcpSoConRxTpPduHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(SoAd_uintx, AUTOMATIC) SocketRouteId,
    VAR(PduLengthType, AUTOMATIC) RxMessageLength,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) DataPtr)
{
    PduInfoType pduInfo;
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
    uint8 metaData[2];
#endif /* STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT */
    PduLengthType copyToUpLength, bufferLength;
    PduLengthType upBufferSize = 0u;
    SoAd_uintx bufferId;
    SoAd_UpLayerModuleType upModule;
    PduLengthType pduLength;
    SoAd_RxFullTpPduHandleType parameter;
    PduLengthType rxDataLength;
    const SoAd_SocketRouteDestType* soadSoRouteDestPtr = &SOAD_SRDEST_CFG(SocketRouteId);

    pduLength = (PduLengthType)SOAD_GET_LENGTH(DataPtr, 0u);
    upModule = soadSoRouteDestPtr->SoAd_RefModule;
    bufferId = *SOAD_SOCON_CFG(SoConId).SoAd_RxBufCfgIndexPtr;
    bufferLength = SOAD_SOCKETBUFFER_CFG(bufferId).bufLen;
    bufferId = bufferId - SOAD_TXBUFFER_NUM;
    SoAd_SoRxBufferManType* soadSoRxBuffMPtr = &SoAd_SoRxBufMan[bufferId];

    pduInfo.SduDataPtr = NULL_PTR;
    pduInfo.SduLength = 0u;
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
    if (soadSoRouteDestPtr->SoAd_MetaDataEnable)
    {
        SoAd_MetaDataFillHandle(SoConId, &metaData[0u]);
        pduInfo.MetaDataPtr = &metaData[0u];
    }
    else
#endif /* STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT */
    {
        pduInfo.MetaDataPtr = NULL_PTR;
    }
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
    PduIdType upPduId = soadSoRouteDestPtr->SoAd_UpPduId;

    if (BUFREQ_OK
        == SOAD_UPPERLAY_PCCFG(upModule).Tp_StartOfReception_FuncPtr(upPduId, &pduInfo, pduLength, &upBufferSize))
    {
        rxDataLength = RxMessageLength - SOAD_HEADER_SIZE;
        /*if not all data can be processed*/
        if ((bufferLength + upBufferSize) < rxDataLength)
        {
            SOAD_UPPERLAY_PCCFG(upModule).Tp_RxIndication_FuncPtr(upPduId, E_NOT_OK);
            SoAd_SoConManager[SoConId].SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
            SoAd_SoConManager[SoConId].SetSocketNeedCloseLine = __LINE__;
#endif /* STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG */
        }
        else
        {
            /*the message's data belong to more than one PDU*/
            if (pduLength <= rxDataLength)
            {
                parameter.DataPtr = DataPtr;
                parameter.PduLength = pduLength;
                parameter.RxDataLength = RxMessageLength;
                parameter.SoConId = SoConId;
                parameter.SocketRouteId = SocketRouteId;
                parameter.UpBufferSize = upBufferSize;
                parameter.UpModuleId = upModule;
                parameter.RxBufferId = bufferId;
                parameter.TcpIpSocketId = SoAd_SoConManager[SoConId].TcpIpSocketId;
                SoAd_RxFullTpPduHandle(&parameter);
            }
            /*the message's all data is part of the PDU*/
            else
            {
                copyToUpLength = upBufferSize;
                if (upBufferSize > rxDataLength)
                {
                    copyToUpLength = rxDataLength;
                }
                if (0u < copyToUpLength)
                {
                    pduInfo.SduDataPtr = &DataPtr[SOAD_HEADER_SIZE];
                    pduInfo.SduLength = copyToUpLength;
                    if (BUFREQ_OK
                        == SOAD_UPPERLAY_PCCFG(upModule).Tp_CopyRxData_FuncPtr(upPduId, &pduInfo, &upBufferSize))
                    {
                        /*SWS_SoAd_00564:SoAd shall confirm the reception of all data*/
                        (void)TcpIp_TcpReceived(SoAd_SoConManager[SoConId].TcpIpSocketId, copyToUpLength);
                        /*copy the remainder data(RxMessageLength-8-copyToUpLength) to SoAd Rx Buffer*/
                        if (TRUE
                            == SoAd_CopyDataToRxBuffer(
                                SoConId,
                                (rxDataLength - copyToUpLength),
                                &DataPtr[SOAD_HEADER_SIZE + copyToUpLength]))
                        {
                            soadSoRxBuffMPtr->ExitPduResidueLength = (pduLength - copyToUpLength);
                            soadSoRxBuffMPtr->SoAd_SocketRouteId = SocketRouteId;
                        }
                    }
                    else
                    {
                        SOAD_UPPERLAY_PCCFG(upModule).Tp_RxIndication_FuncPtr(upPduId, E_NOT_OK);
                        SoAd_SoConManager[SoConId].SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
                        SoAd_SoConManager[SoConId].SetSocketNeedCloseLine = __LINE__;
#endif /* STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG */
                    }
                }
                else
                {
                    /*copy the remainder data(RxMessageLength-8) to SoAd Rx Buffer*/
                    if (TRUE == SoAd_CopyDataToRxBuffer(SoConId, rxDataLength, &DataPtr[SOAD_HEADER_SIZE]))
                    {
                        soadSoRxBuffMPtr->ExitPduResidueLength = pduLength;
                        soadSoRxBuffMPtr->SoAd_SocketRouteId = SocketRouteId;
                    }
                }
            }
        }
    }
    else
    {
        SoAd_SoConManager[SoConId].SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
        SoAd_SoConManager[SoConId].SetSocketNeedCloseLine = __LINE__;
#endif /* STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG */
    }
#endif /* SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u */
    return;
}

static FUNC(void, SOAD_CODE)
    SoAd_RxFullTpPduHandle(P2CONST(SoAd_RxFullTpPduHandleType, AUTOMATIC, SOAD_APPL_DATA) Parameter)
{
    PduInfoType pduInfo;
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
    uint8 metaData[2];
#endif /* STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT */
    SoAd_UpLayerModuleType upModule;
    SoAd_uintx bufferId;
    PduLengthType upBufferSize;
    uint8* DataPtr;

    bufferId = Parameter->RxBufferId;
    upModule = Parameter->UpModuleId;
    DataPtr = Parameter->DataPtr;
    upBufferSize = Parameter->UpBufferSize;
    SoAd_SoRxBufferManType* soadSoRxBuffMPtr = &SoAd_SoRxBufMan[bufferId];
    const SoAd_SocketRouteDestType* soadSoRouteDestPtr = &SOAD_SRDEST_CFG(Parameter->SocketRouteId);
    PduIdType upPduId = soadSoRouteDestPtr->SoAd_UpPduId;

#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
    if (soadSoRouteDestPtr->SoAd_MetaDataEnable)
    {
        SoAd_MetaDataFillHandle(Parameter->SoConId, &metaData[0u]);
        pduInfo.MetaDataPtr = &metaData[0u];
    }
    else
#endif /* STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT */
    {
        pduInfo.MetaDataPtr = NULL_PTR;
    }
    /*the UpBuffer is large enough to copy the total pdu data*/
    if (upBufferSize >= Parameter->PduLength)
    {
        pduInfo.SduDataPtr = &DataPtr[SOAD_HEADER_SIZE];
        pduInfo.SduLength = Parameter->PduLength;
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
        if (BUFREQ_OK == SOAD_UPPERLAY_PCCFG(upModule).Tp_CopyRxData_FuncPtr(upPduId, &pduInfo, &upBufferSize))
        {
            /*SWS_SoAd_00564:SoAd shall confirm the reception of all data*/
            (void)TcpIp_TcpReceived(Parameter->TcpIpSocketId, Parameter->PduLength);
            SOAD_UPPERLAY_PCCFG(upModule).Tp_RxIndication_FuncPtr(upPduId, E_OK);
            /*copy the remainder data(include the next pdu's header and data) to SoAd Rx Buffer*/
            (void)SoAd_CopyDataToRxBuffer(
                Parameter->SoConId,
                (Parameter->RxDataLength - SOAD_HEADER_SIZE - Parameter->PduLength),
                &DataPtr[SOAD_HEADER_SIZE + Parameter->PduLength]);
        }
        else
        {
            SOAD_UPPERLAY_PCCFG(upModule).Tp_RxIndication_FuncPtr(upPduId, E_NOT_OK);
            SoAd_SoConManager[Parameter->SoConId].SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
            SoAd_SoConManager[Parameter->SoConId].SetSocketNeedCloseLine = __LINE__;
#endif /* STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG */
        }
#endif /* SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u */
    }
    else if (upBufferSize > 0u)
    {
        pduInfo.SduDataPtr = &DataPtr[SOAD_HEADER_SIZE];
        pduInfo.SduLength = upBufferSize;
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)

        if (BUFREQ_OK == SOAD_UPPERLAY_PCCFG(upModule).Tp_CopyRxData_FuncPtr(upPduId, &pduInfo, &upBufferSize))
        {
            /*SWS_SoAd_00564:SoAd shall confirm the reception of all data*/
            (void)TcpIp_TcpReceived(Parameter->TcpIpSocketId, upBufferSize);
            /*copy the remainder data to SoAd Rx Buffer*/
            if (TRUE
                == SoAd_CopyDataToRxBuffer(
                    Parameter->SoConId,
                    (Parameter->RxDataLength - SOAD_HEADER_SIZE - pduInfo.SduLength),
                    &DataPtr[SOAD_HEADER_SIZE + pduInfo.SduLength]))
            {
                soadSoRxBuffMPtr->ExitPduResidueLength = (Parameter->PduLength - pduInfo.SduLength);
                soadSoRxBuffMPtr->SoAd_SocketRouteId = Parameter->SocketRouteId;
            }
        }
        else
        {
            SOAD_UPPERLAY_PCCFG(upModule).Tp_RxIndication_FuncPtr(upPduId, E_NOT_OK);
            SoAd_SoConManager[Parameter->SoConId].SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
            SoAd_SoConManager[Parameter->SoConId].SetSocketNeedCloseLine = __LINE__;
#endif /* STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG */
        }
#endif /* SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u */
    }
    else
    {
        /*copy the remainder data to SoAd Rx Buffer*/
        if (TRUE
            == SoAd_CopyDataToRxBuffer(
                Parameter->SoConId,
                (Parameter->RxDataLength - SOAD_HEADER_SIZE),
                &DataPtr[SOAD_HEADER_SIZE]))
        {
            soadSoRxBuffMPtr->ExitPduResidueLength = Parameter->PduLength;
            soadSoRxBuffMPtr->SoAd_SocketRouteId = Parameter->SocketRouteId;
        }
    }
    return;
}
#endif /*STD_ON == SOAD_SUPPORT_HEADERID*/

static FUNC(void, SOAD_CODE) SoAd_HeaderDisableSoConRxTpPduHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(SoAd_uintx, AUTOMATIC) SocketRouteId,
    VAR(PduLengthType, AUTOMATIC) RxMessageLength,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) DataPtr)
{
    SoAd_uintx bufferId;
    PduInfoType pduInfo;
    PduLengthType upBufferSize;
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
    uint8 metaData[2u];
#endif /* STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT */

    bufferId = *SOAD_SOCON_CFG(SoConId).SoAd_RxBufCfgIndexPtr;
    bufferId = bufferId - SOAD_TXBUFFER_NUM;
    SoAd_UpLayerModuleType upModule = SOAD_SRDEST_CFG(SocketRouteId).SoAd_RefModule;
    const SoAd_SoRxBufferManType* soadSoRxBuffMPtr = &SoAd_SoRxBufMan[bufferId];

    /*SWS_SoAd_00568:
      if the SoAd receive buffer does not contain any TP data for this socket connection*/
    if (soadSoRxBuffMPtr->ExitPosition == soadSoRxBuffMPtr->EnterPosition)
    {
        pduInfo.SduDataPtr = NULL_PTR;
        pduInfo.SduLength = 0u;
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
        if (SOAD_SRDEST_CFG(SocketRouteId).SoAd_MetaDataEnable)
        {
            SoAd_MetaDataFillHandle(SoConId, &metaData[0u]);
            pduInfo.MetaDataPtr = &metaData[0u];
        }
        else
#endif /* STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT */
        {
            pduInfo.MetaDataPtr = NULL_PTR;
        }
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
        PduIdType upPduId = SOAD_SRDEST_CFG(SocketRouteId).SoAd_UpPduId;

        if (BUFREQ_OK == SOAD_UPPERLAY_PCCFG(upModule).Tp_CopyRxData_FuncPtr(upPduId, &pduInfo, &upBufferSize))
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
            if (BUFREQ_OK == SOAD_UPPERLAY_PCCFG(upModule).Tp_CopyRxData_FuncPtr(upPduId, &pduInfo, &upBufferSize))
            {
                if (TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(SoConId).SoAd_TcpUdpProtocol)
                {
                    /*SWS_SoAd_00564:SoAd shall confirm the reception of all data*/
                    (void)TcpIp_TcpReceived(SoAd_SoConManager[SoConId].TcpIpSocketId, pduInfo.SduLength);
                }
                /*copy data to SoAd Rx Buffer*/
                if (pduInfo.SduLength < RxMessageLength)
                {
                    (void)SoAd_CopyDataToRxBuffer(
                        SoConId,
                        (RxMessageLength - pduInfo.SduLength),
                        &DataPtr[pduInfo.SduLength]);
                }
            }
            else
            {
                SoAd_SoConManager[SoConId].SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
                SoAd_SoConManager[SoConId].SetSocketNeedCloseLine = __LINE__;
#endif /* STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG */
            }
        }
        else
        {
            SoAd_SoConManager[SoConId].SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
            SoAd_SoConManager[SoConId].SetSocketNeedCloseLine = __LINE__;
#endif /* STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG */
        }
#endif /* SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u */
    }
    /*SWS_SoAd_00568:
      if the SoAd receive buffer already contains TP data for this socket connection
      and is able to store all (newley) received data*/
    else
    {
        (void)SoAd_CopyDataToRxBuffer(SoConId, RxMessageLength, &DataPtr[0u]);
    }
    return;
}

#if (STD_ON == SOAD_SUPPORT_HEADERID)
static FUNC(void, SOAD_CODE) SoAd_HeaderEnableTcpSoConRxHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(PduLengthType, AUTOMATIC) RxMessageLength,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) DataPtr)
{
    uint32 header;
    PduLengthType pduLength;
#if (0u < SOAD_MAX_IF_PDU_SIZE)
    SoAd_UpLayerModuleType upModule;
    PduInfoType pduInfo;
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
    uint8 metaData[2];
#endif /* STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT */
#endif /* 0u < SOAD_MAX_IF_PDU_SIZE */
    SoAd_uintx socketRouteId;
    boolean findValidSocketRoute = FALSE;
    SoAd_uintx bufferId;
    PduLengthType rxDataLength;

    if (NULL_PTR != SOAD_SOCON_CFG(SoConId).SoAd_RxBufCfgIndexPtr)
    {
        bufferId = SOAD_SOCON_CFG(SoConId).SoAd_RxBufCfgIndexPtr[0u];
        /*used for SoAd_RxBuffer Manage*/
        bufferId = bufferId - SOAD_TXBUFFER_NUM;
        SoAd_SoRxBufferManType* soadSoRxBuffMPtr = &SoAd_SoRxBufMan[bufferId];
        rxDataLength = RxMessageLength - SOAD_HEADER_SIZE;

        /*the newly pdu:copy data to up layer,copy residue data to SoAd Rx Buffer*/
        if ((soadSoRxBuffMPtr->EnterPosition == soadSoRxBuffMPtr->ExitPosition)
            && (0u == soadSoRxBuffMPtr->ExitPduResidueLength) && (RxMessageLength >= SOAD_HEADER_SIZE))
        {
            header = SOAD_GET_HEADER(DataPtr, 0u);
            pduLength = (PduLengthType)SOAD_GET_LENGTH(DataPtr, 0u);
            findValidSocketRoute = SoAd_FindHeaderIDAndSocketRouteId(SoConId, header, &socketRouteId);
            /*findValidSocketRoute and socket dest routing group is enabled.*/
            if (findValidSocketRoute && SoAd_SocketRouteDestManager[socketRouteId].SocketRouteDestEnable)
            {
                const SoAd_SocketRouteDestType* soadSoRouteDestPtr = &SOAD_SRDEST_CFG(socketRouteId);

                (void)TcpIp_TcpReceived(SoAd_SoConManager[SoConId].TcpIpSocketId, SOAD_HEADER_SIZE);
                if (SOAD_UPPER_LAYER_TP == soadSoRouteDestPtr->SoAd_RxUpperLayerType)
                {
                    /*Skip further processing if PDU length is 0 and (SoAdPduHeaderEnable is FALSE or
                     * SoAdRxUpperLayerType is TP)*/
                    if (0u < pduLength)
                    {
                        SoAd_HeaderEnableTcpSoConRxTpPduHandle(SoConId, socketRouteId, RxMessageLength, DataPtr);
                    }
                }
#if (0u < SOAD_MAX_IF_PDU_SIZE)
                /*IF PDU Rx handle*/
                else
                {
                    /*the message's data belong to more than one PDU*/
                    if (pduLength <= rxDataLength)
                    {
                        pduInfo.SduDataPtr = &DataPtr[SOAD_HEADER_SIZE];
                        pduInfo.SduLength = pduLength;
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
                        if (soadSoRouteDestPtr->SoAd_MetaDataEnable)
                        {
                            SoAd_MetaDataFillHandle(SoConId, &metaData[0u]);
                            pduInfo.MetaDataPtr = &metaData[0u];
                        }
                        else
#endif /* STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT */
                        {
                            pduInfo.MetaDataPtr = NULL_PTR;
                        }
                        upModule = soadSoRouteDestPtr->SoAd_RefModule;
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)

                        SOAD_UPPERLAY_PCCFG(upModule).If_RxIndication_FuncPtr(
                            soadSoRouteDestPtr->SoAd_UpPduId,
                            &pduInfo);
#endif /* SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u */

                        /*SWS_SoAd_00564:SoAd shall confirm the reception of all data*/
                        (void)TcpIp_TcpReceived(SoAd_SoConManager[SoConId].TcpIpSocketId, pduLength);
                        /*copy the remainder data(Length-8-pduLength) to SoAd Rx Buffer*/
                        (void)SoAd_CopyDataToRxBuffer(
                            SoConId,
                            (rxDataLength - pduLength),
                            &DataPtr[SOAD_HEADER_SIZE + pduLength]);
                    }
                    /*the message is part data of one PDU*/
                    else
                    {
                        /*copy the remainder data(Length-8) to SoAd Rx Buffer*/
                        if (TRUE == SoAd_CopyDataToRxBuffer(SoConId, rxDataLength, &DataPtr[SOAD_HEADER_SIZE]))
                        {
                            soadSoRxBuffMPtr->ExitPduResidueLength = pduLength;
                            soadSoRxBuffMPtr->SoAd_SocketRouteId = socketRouteId;
                        }
                    }
                }
#endif /*0u < SOAD_MAX_IF_PDU_SIZE*/
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
                    (void)TcpIp_TcpReceived(
                        SoAd_SoConManager[SoConId].TcpIpSocketId,
                        (uint32)pduLength + (uint32)SOAD_HEADER_SIZE);
                    (void)SoAd_CopyDataToRxBuffer(
                        SoConId,
                        (rxDataLength - pduLength),
                        &DataPtr[pduLength + SOAD_HEADER_SIZE]);
                }
                /*need copy the part pdu data to buffer,then discard the all data of the pdu.*/
                else
                {
                    (void)SoAd_CopyDataToRxBuffer(SoConId, RxMessageLength, DataPtr);
                }
            }
        }
        /*copy the total data to SoAd Rx Buffer*/
        else
        {
            (void)SoAd_CopyDataToRxBuffer(SoConId, RxMessageLength, DataPtr);
        }
    }
    else
    {
        (void)Det_ReportRuntimeError(SOAD_MODULE_ID, SOAD_INSTANCE, SOAD_SID_RXINDICATION, SOAD_E_INV_PDUHEADER_ID);
        /*discard the all pdu data*/
        (void)TcpIp_TcpReceived(SoAd_SoConManager[SoConId].TcpIpSocketId, (uint32)RxMessageLength);
    }
    return;
}
#endif /*STD_ON == SOAD_SUPPORT_HEADERID*/

/*copy data to SoAd Rx Buffer*/
static FUNC(boolean, SOAD_CODE) SoAd_CopyDataToRxBuffer(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(PduLengthType, AUTOMATIC) RxMessageLength,
    P2CONST(uint8, AUTOMATIC, SOAD_APPL_DATA) DataPtr)
{
    PduLengthType srcPos;
    PduLengthType destPos;
    boolean copyDataOk = TRUE;
    SoAd_uintx bufferId = SOAD_SOCON_CFG(SoConId).SoAd_RxBufCfgIndexPtr[0u];
    PduLengthType bufferPosition = SOAD_SOCKETBUFFER_CFG(bufferId).bufPos;
    PduLengthType bufferLength = SOAD_SOCKETBUFFER_CFG(bufferId).bufLen;
    SoAd_SoRxBufferManType* soadSoRxBuffMPtr;
    PduLengthType startPos;

    if (0u < RxMessageLength)
    {
        /*used for SoAd_RxBuffer Manage*/
        bufferId = bufferId - SOAD_TXBUFFER_NUM;
        soadSoRxBuffMPtr = &SoAd_SoRxBufMan[bufferId];
        srcPos = soadSoRxBuffMPtr->EnterPosition;
        destPos = srcPos + RxMessageLength;
        startPos = bufferPosition + srcPos;
        if (soadSoRxBuffMPtr->ExitPosition <= soadSoRxBuffMPtr->EnterPosition)
        {
            if (destPos >= bufferLength)
            {
                if ((destPos - bufferLength) < soadSoRxBuffMPtr->ExitPosition)
                {
                    PduLengthType copyLength = bufferLength - srcPos;
                    (void)ILib_memcpy(&SoAd_PduBuffer[startPos], DataPtr, copyLength);
                    (void)ILib_memcpy(
                        &SoAd_PduBuffer[bufferPosition],
                        &DataPtr[copyLength],
                        (PduLengthType)(destPos - bufferLength));
                    soadSoRxBuffMPtr->EnterPosition = destPos - bufferLength;
                }
                /*SWS_SoAd_00693:larger than the remaining available buffer size */
                else
                {
                    copyDataOk = FALSE;
                }
            }
            else
            {
                (void)ILib_memcpy(&SoAd_PduBuffer[startPos], DataPtr, RxMessageLength);
                soadSoRxBuffMPtr->EnterPosition = destPos;
            }
        }
        else
        {
            if (destPos < soadSoRxBuffMPtr->ExitPosition)
            {
                (void)ILib_memcpy(&SoAd_PduBuffer[startPos], DataPtr, RxMessageLength);
                soadSoRxBuffMPtr->EnterPosition = destPos;
            }
            else
            {
                copyDataOk = FALSE;
            }
        }
        if (!copyDataOk)
        {
            SoAd_SoConManager[SoConId].SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
            SoAd_SoConManager[SoConId].SetSocketNeedCloseLine = __LINE__;
#endif /* STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG */
            (void)Det_ReportRuntimeError(SOAD_MODULE_ID, SOAD_INSTANCE, SOAD_SID_RXINDICATION, SOAD_E_NOBUFS);
        }
    }
    return copyDataOk;
}
#endif /*0u < SOAD_RXBUFFER_NUM*/

static FUNC(void, SOAD_CODE) SoAd_HeaderDisableSoConRxIfPduHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(SoAd_uintx, AUTOMATIC) SocketRouteId,
    VAR(PduLengthType, AUTOMATIC) RxMessageLength,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) DataPtr)
{
    SoAd_UpLayerModuleType upModule;
    PduInfoType pduInfo;
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
    uint8 metaData[2u];
#endif /* STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT */

    upModule = SOAD_SRDEST_CFG(SocketRouteId).SoAd_RefModule;
    pduInfo.SduDataPtr = DataPtr;
    pduInfo.SduLength = RxMessageLength;
#if (STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT)
    if (SOAD_SRDEST_CFG(SocketRouteId).SoAd_MetaDataEnable)
    {
        SoAd_MetaDataFillHandle(SoConId, &metaData[0u]);
        pduInfo.MetaDataPtr = &metaData[0u];
    }
    else
#endif /* STD_ON == SOAD_SOCKET_ROUTE_METADATA_SUPPORT */
    {
        pduInfo.MetaDataPtr = NULL_PTR;
    }
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)

    if (NULL_PTR != SOAD_UPPERLAY_PCCFG(upModule).If_RxIndication_FuncPtr)
    {
        SOAD_UPPERLAY_PCCFG(upModule).If_RxIndication_FuncPtr(SOAD_SRDEST_CFG(SocketRouteId).SoAd_UpPduId, &pduInfo);
    }
#endif /* SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u */

    if (TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(SoConId).SoAd_TcpUdpProtocol)
    {
        /*SWS_SoAd_00564:SoAd shall confirm the reception of all data*/
        (void)TcpIp_TcpReceived(SoAd_SoConManager[SoConId].TcpIpSocketId, RxMessageLength);
    }
    return;
}

#if (0u < SOAD_SOCKET_ROUTE_DEST_MAX)
static FUNC(boolean, SOAD_CODE) SoAd_FindTheValidSoCon(
    VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_CONST) RemoteAddrPtr,
#if (STD_ON == SOAD_SUPPORT_UDP)
    VAR(SoAd_FindTheValidSoConType, AUTOMATIC) Parameter,
#endif
    P2VAR(SoAd_SoConIdType, AUTOMATIC, SOAD_APPL_DATA) SoConId)
{
    SoAd_uintx cnt;
    boolean socketIdValid = FALSE;
    boolean soConValid = FALSE;
    SoAd_SoConIdType soConId;
#if (STD_ON == SOAD_SUPPORT_TCP)
    uint16 soConGroupId;
#endif
#if (STD_ON == SOAD_SUPPORT_UDP)
    *(Parameter.RemoteAddrChanged) = FALSE;
#endif
    SoAd_InnerSocketManageType* soadSoConMPtr;
    const SoAd_InnerSocketGroupManageType* soadSoConGruMPtr;

#if (STD_ON == SOAD_SUPPORT_UDP)
    const SoAd_SocketUdpType* soadSocketUdpPtr;
    /*find a udp socket connection*/
    for (cnt = 0u; (cnt < SOAD_SO_CON_GROUP_NUM) && (FALSE == socketIdValid); cnt++)
    {
        soadSocketUdpPtr = SOAD_UDPTYPE_CFG(cnt);
        soadSoConGruMPtr = &SoAd_SoConGroupManager[cnt];
        if (soadSoConGruMPtr->TcpIpSocketValid && (SocketId == soadSoConGruMPtr->TcpIpSocketId)
            && (NULL_PTR != soadSocketUdpPtr))
        {
            if (SOAD_SOCONGROUP_CFG(cnt).SoAd_SocketMsgAcceptanceFilterEnabled)
            {
                /*SWS_SoAd_00657:Best Match Algorithm*/
                if (TRUE
                    == SoAd_GetBestMatchAlgorithmSoCon(
                        cnt,
                        RemoteAddrPtr,
                        &soConId,
                        SOAD_SOCON_MATCH_REQUEST_ONLINE_RECONNECT))
                {
                    soadSoConMPtr = &SoAd_SoConManager[soConId];
                    *(Parameter.RemoteAddr) = soadSoConMPtr->RemoteAddr;
                    /*SWS_SoAd_00592:(a) overwrite the remote address parts
                    specified with wildcards with the related source address
                    parts of the received message and (b) change the state of
                      the socket connection to SOAD_SOCON_ONLINE */
                    if ((SOAD_SOCON_ONLINE != soadSoConMPtr->SoAd_SoConMode)
                        && !soadSocketUdpPtr->SoAd_SocketUdpListenOnly)
                    {
                        *(Parameter.RemoteAddrChanged) = TRUE;
                        soadSoConMPtr->RemoteAddr = *RemoteAddrPtr;
                        soadSoConMPtr->SoAd_SoConMode = SOAD_SOCON_ONLINE;
                        SoAd_SoConModeChgNotificationAndTpStartOfReception(soConId, cnt);
                    }
                    /*Start the UdpAliveSupervisionTime*/
                    if ((*(Parameter.RemoteAddrChanged)) == TRUE)
                    {
                        if ((NULL_PTR != soadSocketUdpPtr->SoAd_SocketUdpAliveSupervisionTimeout)
                            && (SOAD_IS_IPADDR_ANY(*SOAD_SOCON_CFG(soConId).SoAd_SocketRemoteAddress)
                                || (TCPIP_PORT_ANY == SOAD_SOCON_CFG(soConId).SoAd_SocketRemoteAddress->port)))
                        {
                            soadSoConMPtr->UdpAliveSupervisionTime =
                                soadSocketUdpPtr->SoAd_SocketUdpAliveSupervisionTimeout[0u];
                        }
                    }
                    else
                    {
                        if (0u < soadSoConMPtr->UdpAliveSupervisionTime)
                        {
                            soadSoConMPtr->UdpAliveSupervisionTime =
                                soadSocketUdpPtr->SoAd_SocketUdpAliveSupervisionTimeout[0u];
                        }
                    }
                    soConValid = TRUE;
                }
            }
            else
            {
                soConId = SOAD_SOCONGROUP_CFG(cnt).SoAd_SoConId[0u];
                soadSoConMPtr = &SoAd_SoConManager[soConId];
                if (soadSoConMPtr->SoAd_SoConMode == SOAD_SOCON_ONLINE)
                {
                    soConValid = TRUE;
                }
            }
            socketIdValid = TRUE;
        }
    }

#endif /* STD_ON == SOAD_SUPPORT_UDP */
#if (STD_ON == SOAD_SUPPORT_TCP)
    /*find a tcp socket connection*/
    for (cnt = 0u; (cnt < SOAD_SOCON_NUM) && (FALSE == socketIdValid); cnt++)
    {
        soadSoConMPtr = &SoAd_SoConManager[cnt];
        soConGroupId = SOAD_SOCON_CFG(cnt).SoAd_SocketConnectionGroupRef;
        soadSoConGruMPtr = &SoAd_SoConGroupManager[soConGroupId];
        if ((!soadSoConGruMPtr->TcpIpSocketValid || (SocketId != soadSoConGruMPtr->TcpIpSocketId))
            && soadSoConMPtr->TcpIpSocketValid && (SocketId == soadSoConMPtr->TcpIpSocketId))
        {
            if (SOAD_SOCONGROUP_CFG(soConGroupId).SoAd_SocketMsgAcceptanceFilterEnabled)
            {
                if ((soadSoConMPtr->RemoteAddr.domain == RemoteAddrPtr->domain)
                    && (SOAD_IS_IPADDR_ANY(soadSoConMPtr->RemoteAddr)
                        || (SOAD_EQ_IPADDR(soadSoConMPtr->RemoteAddr, *RemoteAddrPtr)))
                    && ((soadSoConMPtr->RemoteAddr.port == TCPIP_PORT_ANY)
                        || (soadSoConMPtr->RemoteAddr.port == RemoteAddrPtr->port)))
                {
                    soConValid = TRUE;
                    soConId = (SoAd_SoConIdType)cnt;
                }
            }
            else
            {
                soConValid = TRUE;
                soConId = (SoAd_SoConIdType)cnt;
            }
            socketIdValid = TRUE;
        }
    }

#endif /* STD_ON == SOAD_SUPPORT_TCP */

    if (soConValid)
    {
        *SoConId = soConId;
    }
    return socketIdValid;
}
#endif /*0u < SOAD_SOCKET_ROUTE_DEST_MAX*/
#endif /*0u < SOAD_SOCKET_ROUTE_DEST_MAX */

#if (STD_ON == SOAD_SUPPORT_UDP)
#if (STD_ON == SOAD_SUPPORT_HEADERID)
static FUNC(boolean, SOAD_CODE) SoAd_UdpStrictHeaderLenCheck(
    VAR(SoAd_uintx, AUTOMATIC) SoConGroupId,
    P2CONST(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
    VAR(uint16, AUTOMATIC) Length)
{
    uint32 dataPosition = 0u;
    uint32 pduLength;
    boolean udpStrictHeaderLenCheck = FALSE;
    if (Length >= SOAD_HEADER_SIZE)
    {
        if (SOAD_UDPTYPE_CFG(SoConGroupId)->SoAd_SocketUdpStrictHeaderLenCheckEnabled)
        {
            while (dataPosition < Length)
            {
                pduLength = SOAD_GET_LENGTH(BufPtr, dataPosition);
                dataPosition = dataPosition + SOAD_HEADER_SIZE + pduLength;
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
#endif /*STD_ON == SOAD_SUPPORT_HEADERID*/

static FUNC(void, SOAD_CODE) SoAd_UdpAliveSupervisionTimeHandle(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId)
{
    SoAd_uintx soConGroup;
    SoAd_InnerSocketManageType* soadSoConMPtr = &SoAd_SoConManager[SoConId];

    if (0u < soadSoConMPtr->UdpAliveSupervisionTime)
    {
        (soadSoConMPtr->UdpAliveSupervisionTime) -= 1u;
        if (0u == soadSoConMPtr->UdpAliveSupervisionTime)
        {
            /*SWS_SoAd_00695:alive supervision timer runs out*/
            soadSoConMPtr->SoAd_SoConMode = SOAD_SOCON_RECONNECT;
            soConGroup = SOAD_SOCON_CFG(SoConId).SoAd_SocketConnectionGroupRef;
            SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, soConGroup);
            if (NULL_PTR != SOAD_SOCON_CFG(SoConId).SoAd_SocketRemoteAddress)
            {
                soadSoConMPtr->RemoteAddr = *SOAD_SOCONREMOTEADDR_CFG(SoConId);
            }
        }
    }
    return;
}
#endif /* STD_ON == SOAD_SUPPORT_UDP */

static FUNC(boolean, SOAD_CODE) SoAd_GetBestMatchAlgorithmSoCon(
    VAR(SoAd_uintx, AUTOMATIC) SoConGroupId,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_CONST) RemoteAddrPtr,
    P2VAR(SoAd_SoConIdType, AUTOMATIC, SOAD_APPL_CONST) SoConId,
    VAR(SoAd_BestMatchStateType, AUTOMATIC) CheckSoConState)
{
    boolean result;
    SoAd_SoConIdType cnt, soConIndex;
    uint8 priority = 0u;
    const SoAd_InnerSocketManageType* soadSoConMPtr;
    SoAd_SoConIdType socketConNum = SOAD_SOCONGROUP_CFG(SoConGroupId).SoAd_SoConNum;

    /*Best Match Algorithm*/
    for (cnt = 0u; (cnt < socketConNum) && (4u != priority); cnt++)
    {
        soConIndex = SOAD_SOCONGROUP_CFG(SoConGroupId).SoAd_SoConId[cnt];
        soadSoConMPtr = &SoAd_SoConManager[soConIndex];
        if ((SOAD_SOCON_MATCH_REQUEST_NONE == CheckSoConState)
            || ((SOAD_SOCON_MATCH_REQUEST_RECONNECT == CheckSoConState)
                && (soadSoConMPtr->SoAd_SoConMode == SOAD_SOCON_RECONNECT) && !soadSoConMPtr->TcpIpSocketValid)
            || ((SOAD_SOCON_MATCH_REQUEST_ONLINE_RECONNECT == CheckSoConState)
                && (soadSoConMPtr->SoAd_SoConMode != SOAD_SOCON_OFFLINE)))
        {
            if (SOAD_IS_IPADDR_ANY(soadSoConMPtr->RemoteAddr) && (TCPIP_PORT_ANY == soadSoConMPtr->RemoteAddr.port))
            {
                if (priority < 1u)
                {
                    *SoConId = soConIndex;
                    priority = 1u;
                }
            }
            else if (
                SOAD_IS_IPADDR_ANY(soadSoConMPtr->RemoteAddr)
                && (RemoteAddrPtr->port == soadSoConMPtr->RemoteAddr.port))
            {
                if (priority < 2u)
                {
                    *SoConId = soConIndex;
                    priority = 2u;
                }
            }
            else if (
                SOAD_EQ_IPADDR(*RemoteAddrPtr, soadSoConMPtr->RemoteAddr)
                && (TCPIP_PORT_ANY == soadSoConMPtr->RemoteAddr.port))
            {
                if (priority < 3u)
                {
                    *SoConId = soConIndex;
                    priority = 3u;
                }
            }
            else if (
                SOAD_EQ_IPADDR(*RemoteAddrPtr, soadSoConMPtr->RemoteAddr)
                && (RemoteAddrPtr->port == soadSoConMPtr->RemoteAddr.port))
            {
                if (priority < 4u)
                {
                    *SoConId = soConIndex;
                    priority = 4u;
                }
            }
            else
            {
                /*do nothing*/
            }
        }
    }

    if ((0u == priority) || (4u < priority))
    {
        result = FALSE;
    }
    else
    {
        result = TRUE;
    }

    return result;
}

static FUNC(void, SOAD_CODE) SoAd_ChangeSocketParameter(
#if (STD_ON == SOAD_SUPPORT_TCP)
    VAR(boolean, AUTOMATIC) SocketProtocolIsTcp,
#endif
    VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId,
    VAR(SoAd_uintx, AUTOMATIC) SoConGroup)
{
    const SoAd_SocketConnectionGroupType* soadSoConGruCfgPtr = &SOAD_SOCONGROUP_CFG(SoConGroup);

    /*In case of a TCP/UDP socket*/
    if (NULL_PTR != soadSoConGruCfgPtr->SoAd_SocketFramePriority)
    {
        (void)TcpIp_ChangeParameter(SocketId, TCPIP_PARAMID_FRAMEPRIO, soadSoConGruCfgPtr->SoAd_SocketFramePriority);
    }
    if (NULL_PTR != soadSoConGruCfgPtr->SoAdSocketDifferentiatedServicesField)
    {
        (void)TcpIp_ChangeParameter(
            SocketId,
            TCPIP_PARAMID_DSCP,
            soadSoConGruCfgPtr->SoAdSocketDifferentiatedServicesField);
    }
    if (NULL_PTR != soadSoConGruCfgPtr->SoAdSocketFlowLabel)
    {
        (void)TcpIp_ChangeParameter(SocketId, TCPIP_PARAMID_FLOWLABEL, soadSoConGruCfgPtr->SoAdSocketFlowLabel);
    }
    if (NULL_PTR != soadSoConGruCfgPtr->SoAdSocketPathMTUEnable)
    {
        (void)
            TcpIp_ChangeParameter(SocketId, TCPIP_PARAMID_PATHMTU_ENABLE, soadSoConGruCfgPtr->SoAdSocketPathMTUEnable);
    }
#if (SOAD_SOCKET_VLAN == STD_ON)
    if (NULL_PTR != soadSoConGruCfgPtr->SoAdSocketVlan)
    {
        (void)TcpIp_ChangeParameter(SocketId, TCPIP_PARAMID_SOCKET_VLAN, soadSoConGruCfgPtr->SoAdSocketVlan);
    }
#endif /* SOAD_SOCKET_VLAN == STD_ON */
#if (STD_ON == SOAD_SUPPORT_TCP)
    /*In case of a TCP socket*/
    if (SocketProtocolIsTcp)
    {
        const SoAd_SocketTcpType* soadSocketTcpPtr = SOAD_TCPTYPE_CFG(SoConGroup);
        if (NULL_PTR != soadSoConGruCfgPtr->SoAd_SocketTpRxBufferMin)
        {
            (void)TcpIp_ChangeParameter(
                SocketId,
                TCPIP_PARAMID_TCP_RXWND_MAX,
                soadSoConGruCfgPtr->SoAd_SocketTpRxBufferMin);
        }
        if (NULL_PTR != soadSocketTcpPtr->SoAd_SocketTcpNoDelay)
        {
            uint8 delayVlu = 0x01u;
            if (soadSocketTcpPtr->SoAd_SocketTcpNoDelay[0])
            {
                delayVlu = 0x0u;
            }
            (void)TcpIp_ChangeParameter(SocketId, TCPIP_PARAMID_TCP_NAGLE, &delayVlu);
        }
        (void)TcpIp_ChangeParameter(SocketId, TCPIP_PARAMID_TCP_KEEPALIVE, &soadSocketTcpPtr->SoAd_SocketTcpKeepAlive);
        if (NULL_PTR != soadSocketTcpPtr->SoAd_SocketTcpKeepAliveTime)
        {
            (void)TcpIp_ChangeParameter(
                SocketId,
                TCPIP_PARAMID_TCP_KEEPALIVE_TIME,
                soadSocketTcpPtr->SoAd_SocketTcpKeepAliveTime);
        }
        if (NULL_PTR != soadSocketTcpPtr->SoAd_SocketTcpKeepAliveProbesMax)
        {
            (void)TcpIp_ChangeParameter(
                SocketId,
                TCPIP_PARAMID_TCP_KEEPALIVE_PROBES_MAX,
                soadSocketTcpPtr->SoAd_SocketTcpKeepAliveProbesMax);
        }
        if (NULL_PTR != soadSocketTcpPtr->SoAd_SocketTcpKeepAliveInterval)
        {
            (void)TcpIp_ChangeParameter(
                SocketId,
                TCPIP_PARAMID_TCP_KEEPALIVE_INTERVAL,
                soadSocketTcpPtr->SoAd_SocketTcpKeepAliveInterval);
        }
        if (NULL_PTR != soadSocketTcpPtr->SoAdSocketTCPOptionFilterId)
        {
            (void)TcpIp_ChangeParameter(
                SocketId,
                TCPIP_PARAMID_TCP_OPTIONFILTER,
                soadSocketTcpPtr->SoAdSocketTCPOptionFilterId);
        }
        if (NULL_PTR != soadSocketTcpPtr->SoAdSocketTcpTlsConnectionId)
        {
            (void)TcpIp_ChangeParameter(
                SocketId,
                TCPIP_PARAMID_TLS_CONNECTION_ASSIGNMENT,
                soadSocketTcpPtr->SoAdSocketTcpTlsConnectionId);
        }
    }
#endif /*STD_ON == SOAD_SUPPORT_TCP*/
    return;
}
#if (STD_ON == SOAD_SUPPORT_UDP)
static FUNC(void, SOAD_CODE)
    SoAd_OpenUdpSoConHandle(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId, VAR(SoAd_uintx, AUTOMATIC) SoConGroup)
{
    uint16 localPort;
    TcpIp_SocketIdType socketId;
    SoAd_InnerSocketManageType* soadSoConMPtr = &SoAd_SoConManager[SoConId];
    SoAd_InnerSocketGroupManageType* soadSoConGruMPtr = &SoAd_SoConGroupManager[SoConGroup];

    if (E_OK == TcpIp_SoAdGetSocket(soadSoConMPtr->RemoteAddr.domain, TCPIP_IPPROTO_UDP, &socketId))
    {
        /*Change the socket specific parameters according to [SWS_SoAd_00689]*/
        SoAd_ChangeSocketParameter(
#if (STD_ON == SOAD_SUPPORT_TCP)
            FALSE,
#endif
            socketId,
            SoConGroup);
        /*init the SoAd_SoConManager parameter*/
        soadSoConMPtr->TcpIpSocketValid = TRUE;
        soadSoConMPtr->TcpIpSocketId = socketId;
        soadSoConGruMPtr->TcpIpSocketValid = TRUE;
        soadSoConGruMPtr->TcpIpSocketId = socketId;
        soadSoConGruMPtr->SoConSocketValidNum = 1u;
        /*Bind the socket to the local address and port*/
        localPort = SOAD_SOCONGROUP_CFG(SoConGroup).SoAd_SocketLocalPort;
        if (E_OK == TcpIp_Bind(socketId, SOAD_SOCKETLOCALADDR_CFG(SoConGroup).LocalAddrId, &localPort))
        {
            /*SWS_SoAd_00591*/
            if (SOAD_UDPTYPE_CFG(SoConGroup)->SoAd_SocketUdpListenOnly
                || ((!SOAD_IS_IPADDR_ANY(soadSoConMPtr->RemoteAddr))
                    && (TCPIP_PORT_ANY != soadSoConMPtr->RemoteAddr.port)))
            {
                if (SOAD_SOCON_ONLINE != soadSoConMPtr->SoAd_SoConMode)
                {
                    soadSoConMPtr->SoAd_SoConMode = SOAD_SOCON_ONLINE;
                    SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, SoConGroup);
                }
            }
            /*SWS_SoAd_00686*/
            else
            {
                if (SOAD_SOCON_RECONNECT != soadSoConMPtr->SoAd_SoConMode)
                {
                    soadSoConMPtr->SoAd_SoConMode = SOAD_SOCON_RECONNECT;
                    SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, SoConGroup);
                }
            }
        }
        else
        {
            /*don't care the return value*/
            (void)TcpIp_Close(socketId, TRUE);
        }
    }
    return;
}
#endif /* STD_ON == SOAD_SUPPORT_UDP */

static FUNC(void, SOAD_CODE) SoAd_SoConModeChgNotificationAndTpStartOfReception(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(SoAd_uintx, AUTOMATIC) SoConGroup)
{
    uint8 cnt0;
    SoAd_uintx cnt1, socketRouteId;
    SoAd_UpLayerModuleType upModule;
    PduLengthType bufferSize = 0u;
    const SoAd_SocketConnectionType* soadSoConCfgPtr = &SOAD_SOCON_CFG(SoConId);
    SoAd_InnerSocketManageType* soadSoConMPtr = &SoAd_SoConManager[SoConId];

    /*if SoAdSocketSoConModeChgNotification is set to TRUE,call <Up>_SoConModeChg()*/
    if (SOAD_SOCONGROUP_CFG(SoConGroup).SoAd_SocketSoConModeChgNotification)
    {
        uint8 refUpNum = soadSoConCfgPtr->SoAd_RefUpNum;
        for (cnt0 = 0u; cnt0 < refUpNum; cnt0++)
        {
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
            upModule = soadSoConCfgPtr->SoAd_RefUpModulePtr[cnt0];
            if (NULL_PTR != SOAD_UPPERLAY_PCCFG(upModule).SoConModeChg_FuncPtr)
            {
                SOAD_UPPERLAY_PCCFG(upModule).SoConModeChg_FuncPtr(SoConId, soadSoConMPtr->SoAd_SoConMode);
            }
#endif /* SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u */
        }
    }
    if (SOAD_SOCON_ONLINE == soadSoConMPtr->SoAd_SoConMode)
    {
        SoAd_uintx refSocketRoutNum = soadSoConCfgPtr->SoAd_RefSocketRouteNum;
        for (cnt1 = 0u; cnt1 < refSocketRoutNum; cnt1++)
        {
            socketRouteId = soadSoConCfgPtr->SoAd_RefSocketRoutePtr[cnt1];
            /*SWS_SoAd_00595:For socket connection with PDU Header mode disabled
              (SoAdPduHeaderEnable = FALSE) and an upper layer with TP-API*/
            if (
#if (STD_ON == SOAD_SUPPORT_HEADERID)
                (NULL_PTR == SOAD_SOCKETROUTE_CFG(socketRouteId).SoAd_RxPduHeaderId) &&
#endif /*STD_ON == SOAD_SUPPORT_HEADERID*/
                (SOAD_UPPER_LAYER_TP == SOAD_SRDEST_CFG(socketRouteId).SoAd_RxUpperLayerType))
            {
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)

                upModule = SOAD_SRDEST_CFG(socketRouteId).SoAd_RefModule;
                if (NULL_PTR != SOAD_UPPERLAY_PCCFG(upModule).Tp_StartOfReception_FuncPtr)
                {
                    if (BUFREQ_OK
                        != SOAD_UPPERLAY_PCCFG(upModule).Tp_StartOfReception_FuncPtr(
                            SOAD_SRDEST_CFG(socketRouteId).SoAd_UpPduId,
                            NULL_PTR,
                            0,
                            &bufferSize))
                    {
                        soadSoConMPtr->SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
                        SoAd_SoConManager[SoConId].SetSocketNeedCloseLine = __LINE__;
#endif /* STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG */
                    }
                }
                else
                {
                    soadSoConMPtr->SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
                    SoAd_SoConManager[SoConId].SetSocketNeedCloseLine = __LINE__;
#endif /* STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG */
                }
#endif /* SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u */
            }
        }
    }
    return;
}

static FUNC(void, SOAD_CODE)
    SoAd_SoConLocalIpAddrAssignmentChg(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId, VAR(SoAd_uintx, AUTOMATIC) SoConGroup)
{
    const SoAd_SocketConnectionType* soadSoConCfgPtr = &SOAD_SOCON_CFG(SoConId);

    /*if SoAd_SocketIpAddrAssignmentChgNotification is set to TRUE,call <Up>_LocalIpAddrAssignmentChg*/
    if (SOAD_SOCONGROUP_CFG(SoConGroup).SoAd_SocketIpAddrAssignmentChgNotification)
    {
        uint8 cnt;
        SoAd_UpLayerModuleType upModule;
        uint8 refUpNum = soadSoConCfgPtr->SoAd_RefUpNum;

        for (cnt = 0u; cnt < refUpNum; cnt++)
        {
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)

            upModule = soadSoConCfgPtr->SoAd_RefUpModulePtr[cnt];
            if (NULL_PTR != SOAD_UPPERLAY_PCCFG(upModule).LocalIpAddrAssignmentChg_FuncPtr)
            {
                SOAD_UPPERLAY_PCCFG(upModule).LocalIpAddrAssignmentChg_FuncPtr(
                    SoConId,
                    SoAd_SoConGroupManager[SoConGroup].LocalIpAddrState);
            }
#endif /* SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u */
        }
    }
    return;
}

static FUNC(void, SOAD_CODE)
    SoAd_OpenTcpServerSoConHandle(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId, VAR(SoAd_uintx, AUTOMATIC) SoConGroup)
{
    TcpIp_SocketIdType socketId;
    uint16 localPort;
    SoAd_InnerSocketManageType* soadSoConMPtr = &SoAd_SoConManager[SoConId];
    SoAd_InnerSocketGroupManageType* soadSoConGruMPtr = &SoAd_SoConGroupManager[SoConGroup];

    if (!soadSoConGruMPtr->TcpIpSocketValid)
    {
        if (E_OK == TcpIp_SoAdGetSocket(soadSoConMPtr->RemoteAddr.domain, TCPIP_IPPROTO_TCP, &socketId))
        {
            /*Change the socket specific parameters according to [SWS_SoAd_00689]*/
            SoAd_ChangeSocketParameter(
#if (STD_ON == SOAD_SUPPORT_TCP)
                TRUE,
#endif
                socketId,
                SoConGroup);
            soadSoConGruMPtr->TcpIpSocketValid = TRUE;
            soadSoConGruMPtr->TcpIpSocketId = socketId;
            soadSoConGruMPtr->SoConSocketValidNum = 0u;
            /*Bind the socket to the local address and port*/
            localPort = SOAD_SOCONGROUP_CFG(SoConGroup).SoAd_SocketLocalPort;
            if (E_OK == TcpIp_Bind(socketId, SOAD_SOCKETLOCALADDR_CFG(SoConGroup).LocalAddrId, &localPort))
            {
                if (E_OK == TcpIp_TcpListen(socketId, SOAD_SOCONGROUP_CFG(SoConGroup).SoAd_SoConNum))
                {
                    /*SWS_SoAd_00686*/
                    if (SOAD_SOCON_RECONNECT != soadSoConMPtr->SoAd_SoConMode)
                    {
                        soadSoConMPtr->SoAd_SoConMode = SOAD_SOCON_RECONNECT;
                        SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, SoConGroup);
                    }
                }
                else
                {
                    /*don't care the return value*/
                    (void)TcpIp_Close(socketId, TRUE);
                }
            }
            else
            {
                /*don't care the return value*/
                (void)TcpIp_Close(socketId, TRUE);
            }
        }
    }
    else
    {
        /*SWS_SoAd_00686:tcp server may have changed to ONLINE before*/
        if (SOAD_SOCON_OFFLINE == soadSoConMPtr->SoAd_SoConMode)
        {
            soadSoConMPtr->SoAd_SoConMode = SOAD_SOCON_RECONNECT;
            SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, SoConGroup);
        }
    }
    return;
}

static FUNC(void, SOAD_CODE)
    SoAd_OpenTcpClientSoConHandle(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId, VAR(SoAd_uintx, AUTOMATIC) SoConGroup)
{
    TcpIp_SocketIdType socketId;
    uint16 localPort;
    SoAd_InnerSocketManageType* soadSoConMPtr = &SoAd_SoConManager[SoConId];

    if (E_OK == TcpIp_SoAdGetSocket(soadSoConMPtr->RemoteAddr.domain, TCPIP_IPPROTO_TCP, &socketId))
    {
        /*Change the socket specific parameters according to [SWS_SoAd_00689]*/
        SoAd_ChangeSocketParameter(
#if (STD_ON == SOAD_SUPPORT_TCP)
            TRUE,
#endif
            socketId,
            SoConGroup);
        soadSoConMPtr->TcpIpSocketValid = TRUE;
        soadSoConMPtr->TcpIpSocketId = socketId;
        /*Bind the socket to the local address and port*/
        localPort = SOAD_SOCONGROUP_CFG(SoConGroup).SoAd_SocketLocalPort;
        if (E_OK == TcpIp_Bind(socketId, SOAD_SOCKETLOCALADDR_CFG(SoConGroup).LocalAddrId, &localPort))
        {
            if (E_OK == TcpIp_TcpConnect(socketId, &(soadSoConMPtr->RemoteAddr)))
            {
                /*SWS_SoAd_00686*/
                if (SOAD_SOCON_OFFLINE == soadSoConMPtr->SoAd_SoConMode)
                {
                    soadSoConMPtr->SoAd_SoConMode = SOAD_SOCON_RECONNECT;
                    SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, SoConGroup);
                }
            }
            else
            {
                /*don't care the return value*/
                (void)TcpIp_Close(socketId, TRUE);
            }
        }
        else
        {
            /*don't care the return value*/
            (void)TcpIp_Close(socketId, TRUE);
        }
    }
    return;
}

static FUNC(void, SOAD_CODE) SoAd_SoConModeChangeHandle(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId)
{
    const SoAd_SocketConnectionType* soadSoConCfgPtr = &SOAD_SOCON_CFG(SoConId);
    SoAd_uintx soConGroup = soadSoConCfgPtr->SoAd_SocketConnectionGroupRef;
    SoAd_InnerSocketManageType* soadSoConMPtr = &SoAd_SoConManager[SoConId];
    SoAd_InnerSocketGroupManageType* soadSoConGruMPtr = &SoAd_SoConGroupManager[soConGroup];

    if (SOAD_SOCON_REQUEST_OPEN == soadSoConMPtr->SoCon_Request[0u])
    {
        if (SOAD_SOCON_OFFLINE != soadSoConMPtr->SoAd_SoConMode)
        {
            soadSoConMPtr->SoCon_Request[0u] = soadSoConMPtr->SoCon_Request[1u];
            soadSoConMPtr->SoCon_Request[1u] = SOAD_SOCON_REQUEST_NONE;
        }
        /*SWS_SoAd_00589*/
        else if (
            soadSoConMPtr->RemoteAddrIsSet && (TCPIP_IPADDR_STATE_ASSIGNED == soadSoConGruMPtr->LocalIpAddrState)
            && !soadSoConMPtr->TcpIpSocketValid)
        {
#if (STD_ON == SOAD_SUPPORT_UDP)
            if (TCPIP_IPPROTO_UDP == soadSoConCfgPtr->SoAd_TcpUdpProtocol)
            {
                /*SWS_SoAd_00590*/
                if (1u == SOAD_SOCONGROUP_CFG(soConGroup).SoAd_SoConNum)
                {
                    SoAd_OpenUdpSoConHandle(SoConId, soConGroup);
                }
                /*SWS_SoAd_00639*/
                else
                {
                    if (!soadSoConGruMPtr->TcpIpSocketValid)
                    {
                        SoAd_OpenUdpSoConHandle(SoConId, soConGroup);
                    }
                    /*Activate the socket connection for communication */
                    else
                    {
                        soadSoConMPtr->TcpIpSocketValid = TRUE;
                        soadSoConMPtr->TcpIpSocketId = soadSoConGruMPtr->TcpIpSocketId;
                        (soadSoConGruMPtr->SoConSocketValidNum) += 1u;
                        /*SWS_SoAd_00591*/
                        if (SOAD_UDPTYPE_CFG(soConGroup)->SoAd_SocketUdpListenOnly
                            || ((!SOAD_IS_IPADDR_ANY(soadSoConMPtr->RemoteAddr))
                                && (TCPIP_PORT_ANY != soadSoConMPtr->RemoteAddr.port)))
                        {
                            if (SOAD_SOCON_ONLINE != soadSoConMPtr->SoAd_SoConMode)
                            {
                                soadSoConMPtr->SoAd_SoConMode = SOAD_SOCON_ONLINE;
                                SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, soConGroup);
                            }
                        }
                        /*SWS_SoAd_00686*/
                        else
                        {
                            if (SOAD_SOCON_RECONNECT != soadSoConMPtr->SoAd_SoConMode)
                            {
                                soadSoConMPtr->SoAd_SoConMode = SOAD_SOCON_RECONNECT;
                                SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, soConGroup);
                            }
                        }
                    }
                }
            }
#endif /* STD_ON == SOAD_SUPPORT_UDP */
#if (STD_ON == SOAD_SUPPORT_TCP)
            if (TCPIP_IPPROTO_TCP == soadSoConCfgPtr->SoAd_TcpUdpProtocol)
            {
                /*SWS_SoAd_00590*/
                if (SOAD_TCPTYPE_CFG(soConGroup)->SoAd_SocketTcpInitiate)
                {
                    SoAd_OpenTcpClientSoConHandle(SoConId, soConGroup);
                }
                /*SWS_SoAd_00638*/
                else
                {
                    SoAd_OpenTcpServerSoConHandle(SoConId, soConGroup);
                }
            }
#endif /* STD_ON == SOAD_SUPPORT_TCP */
            /*The Open request is cleared only when the Open request is successfully executed*/
            if (SOAD_SOCON_OFFLINE != soadSoConMPtr->SoAd_SoConMode)
            {
                soadSoConMPtr->SoCon_Request[0u] = soadSoConMPtr->SoCon_Request[1u];
                soadSoConMPtr->SoCon_Request[1u] = SOAD_SOCON_REQUEST_NONE;
            }
        }
        else
        {
            /*do nothing*/
        }
    }
    else if (SOAD_SOCON_REQUEST_CLOSE == soadSoConMPtr->SoCon_Request[0u])
    {
        /*SWS_SoAd_00604*/
        if (SOAD_SOCON_OFFLINE != soadSoConMPtr->SoAd_SoConMode)
        {
#if (STD_ON == SOAD_SUPPORT_TCP)
            if (TCPIP_IPPROTO_TCP == soadSoConCfgPtr->SoAd_TcpUdpProtocol)
            {
                soadSoConMPtr->ChangeModeToOffline = TRUE;
            }
#endif /*STD_ON == SOAD_SUPPORT_TCP*/
            SoAd_SoConCloseHandle(SoConId, soConGroup, TRUE);
#if (STD_ON == SOAD_SUPPORT_UDP)
            if (TCPIP_IPPROTO_UDP == soadSoConCfgPtr->SoAd_TcpUdpProtocol)
            {
                soadSoConMPtr->TcpIpSocketValid = FALSE;
                soadSoConMPtr->TcpIpSocketId = (TcpIp_SocketIdType)SOAD_UNUSED_UINT16;
                soadSoConMPtr->SoAd_SoConMode = SOAD_SOCON_OFFLINE;
                SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, soConGroup);
            }
#endif /*STD_ON == SOAD_SUPPORT_UDP*/
        }
        soadSoConMPtr->SoCon_Request[0u] = soadSoConMPtr->SoCon_Request[1u];
        soadSoConMPtr->SoCon_Request[1u] = SOAD_SOCON_REQUEST_NONE;
    }
    else
    {
        /*handle the Disconnection And Recovery of SoCon*/
        SoAd_DisconnectionAndRecoveryHandle(SoConId, soConGroup);
    }
    return;
}

#if (0u < SOAD_RXBUFFER_NUM)
static FUNC(void, SOAD_CODE) SoAd_CloseRxTpPduHandle(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId)
{
    SoAd_uintx rxBufferId;
    SoAd_uintx soAdSocketRouteId = SOAD_UNUSED_UINT16;
    PduIdType pduId;
    SoAd_UpLayerModuleType upModule;
    const SoAd_SocketConnectionType* soadSoConCfgPtr = &SOAD_SOCON_CFG(SoConId);

    rxBufferId = soadSoConCfgPtr->SoAd_RxBufCfgIndexPtr[0];
    rxBufferId = rxBufferId - SOAD_TXBUFFER_NUM;
    if (SOAD_UNUSED_UINT16 == SoAd_SoRxBufMan[rxBufferId].SoAd_SocketRouteId)
    {
        if (NULL_PTR != soadSoConCfgPtr->SoAd_RefSocketRoutePtr)
        {
            soAdSocketRouteId = soadSoConCfgPtr->SoAd_RefSocketRoutePtr[0u];
        }
    }
    else
    {
        soAdSocketRouteId = SoAd_SoRxBufMan[rxBufferId].SoAd_SocketRouteId;
    }
    if ((soAdSocketRouteId < SOAD_SOCKET_ROUTE_DEST_NUM)
        && (SOAD_UPPER_LAYER_TP == SOAD_SRDEST_CFG(soAdSocketRouteId).SoAd_RxUpperLayerType))
    {
        upModule = SOAD_SRDEST_CFG(soAdSocketRouteId).SoAd_RefModule;
        pduId = SOAD_SRDEST_CFG(soAdSocketRouteId).SoAd_UpPduId;
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)

#if (STD_ON == SOAD_SUPPORT_HEADERID)
        if (NULL_PTR != SOAD_SOCKETROUTE_CFG(soAdSocketRouteId).SoAd_RxPduHeaderId)
        {
            if (SoAd_SoRxBufMan[rxBufferId].ExitPduResidueLength > 0u)
            {
                SOAD_UPPERLAY_PCCFG(upModule).Tp_RxIndication_FuncPtr(pduId, E_NOT_OK);
            }
        }
        else
#endif /*STD_ON == SOAD_SUPPORT_HEADERID*/
        {
            SOAD_UPPERLAY_PCCFG(upModule).Tp_RxIndication_FuncPtr(pduId, E_NOT_OK);
        }
#endif /* SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u */
    }
    /*clear the SoCon Rx Buffer*/
    SoAd_RxBufferInit(rxBufferId);
    return;
}

static FUNC(boolean, SOAD_CODE) SoAd_CheckRxTpPduDataHandle(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId)
{
    boolean result = FALSE;
    SoAd_uintx rxBufferId;
    SoAd_uintx soAdSocketRouteId = SOAD_UNUSED_UINT16;
    const SoAd_SocketConnectionType* soadSoConCfgPtr = &SOAD_SOCON_CFG(SoConId);

    if (NULL_PTR != soadSoConCfgPtr->SoAd_RxBufCfgIndexPtr)
    {
        rxBufferId = *soadSoConCfgPtr->SoAd_RxBufCfgIndexPtr;
        rxBufferId = rxBufferId - SOAD_TXBUFFER_NUM;
        if (SOAD_UNUSED_UINT16 == SoAd_SoRxBufMan[rxBufferId].SoAd_SocketRouteId)
        {
            if (NULL_PTR != soadSoConCfgPtr->SoAd_RefSocketRoutePtr)
            {
                soAdSocketRouteId = soadSoConCfgPtr->SoAd_RefSocketRoutePtr[0u];
            }
        }
        else
        {
            soAdSocketRouteId = SoAd_SoRxBufMan[rxBufferId].SoAd_SocketRouteId;
        }

        if ((soAdSocketRouteId < SOAD_SOCKET_ROUTE_DEST_NUM)
            && (SOAD_UPPER_LAYER_TP == SOAD_SRDEST_CFG(soAdSocketRouteId).SoAd_RxUpperLayerType))
        {
            /*the EnterPosition is not equal to ExitPosition:the Rx Buffer have store receive data*/
            if (SoAd_SoRxBufMan[rxBufferId].EnterPosition != SoAd_SoRxBufMan[rxBufferId].ExitPosition)
            {
                result = TRUE;
            }
        }
    }

    return result;
}
#endif /* 0u < SOAD_RXBUFFER_NUM */

static FUNC(void, SOAD_CODE) SoAd_SoConCloseHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(SoAd_uintx, AUTOMATIC) SoConGroup,
    VAR(boolean, AUTOMATIC) CloseSocket)
{
#if (0u < SOAD_PDU_ROUTE_DEST_MAX)
    SoAd_uintx cnt, pduRouteDestId;
    PduIdType pduRouteId;
#endif
#if ((0u < SOAD_RXBUFFER_NUM) || (0u < SOAD_PDU_ROUTE_DEST_MAX))
    SoAd_UpLayerModuleType upModule;
#endif
    const SoAd_SocketConnectionType* soadSoConCfgPtr = &SOAD_SOCON_CFG(SoConId);
    SoAd_InnerSocketManageType* soadSoConMPtr = &SoAd_SoConManager[SoConId];
    SoAd_InnerSocketGroupManageType* soadSoConGruMPtr = &SoAd_SoConGroupManager[SoConGroup];

#if (0u < SOAD_PDU_ROUTE_DEST_MAX)
    SoAd_uintx refPduRouteDestNum = soadSoConCfgPtr->SoAd_RefPduRouteDestNum;

    for (cnt = 0u; cnt < refPduRouteDestNum; cnt++)
    {
        pduRouteDestId = soadSoConCfgPtr->SoAd_RefPduRouteDestPtr[cnt];
        if (0u < SoAd_PduRouteDestManager[pduRouteDestId].TxPendingLength)
        {
            /*Init PduRouteDestManager parameter*/
            SoAd_PduRouteDestManager[pduRouteDestId].TxPendingLength = 0u;
        }
        pduRouteId = SOAD_PDUROTDEST_CFG(pduRouteDestId).SoAd_PduRouteIndex;
        /*Init PduRouteManager parameter*/
        SoAd_PduRouteManagerInit(pduRouteId);
        if (soadSoConMPtr->TpPduTransmitting && (pduRouteId == soadSoConMPtr->ActiveSoAdPduRouteId))
        {
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)

            upModule = SOAD_PDUROUT_CFG(pduRouteId).SoAd_RefModule;
            if (NULL_PTR != SOAD_UPPERLAY_PCCFG(upModule).Tp_TxConfirmation_FuncPtr)
            {
                /*TP transmitting,call the up TpTxConfirmation with E_NOT_OK*/
                SOAD_UPPERLAY_PCCFG(upModule).Tp_TxConfirmation_FuncPtr(
                    SOAD_PDUROUT_CFG(pduRouteId).SoAd_UpPduId,
                    E_NOT_OK);
            }
#endif /* SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u */
        }
    }
#if (0u < SOAD_TXBUFFER_NUM)
    if (NULL_PTR != soadSoConCfgPtr->SoAd_TxBufCfgIndexPtr)
    {
        /*Init Tx Buffer parameter*/
        SoAd_TxBufferInit(soadSoConCfgPtr->SoAd_TxBufCfgIndexPtr[0]);
    }
#endif /*0u < SOAD_TXBUFFER_NUM*/
#endif /*0u < SOAD_PDU_ROUTE_DEST_MAX*/

#if (0u < SOAD_RXBUFFER_NUM)
    /* TP PDU is receiving handle*/
    if (NULL_PTR != soadSoConCfgPtr->SoAd_RxBufCfgIndexPtr)
    {
        SoAd_CloseRxTpPduHandle(SoConId);
    }
#endif /*0u < SOAD_RXBUFFER_NUM*/

#if (STD_ON == SOAD_SUPPORT_TCP)
    /*close the tcpip socket*/
    if (TCPIP_IPPROTO_TCP == soadSoConCfgPtr->SoAd_TcpUdpProtocol)
    {
        if (CloseSocket)
        {
            if (soadSoConMPtr->TcpIpSocketValid)
            {
                (void)TcpIp_Close(soadSoConMPtr->TcpIpSocketId, soadSoConMPtr->SoConCloseAbort);
            }
            else
            {
                if (soadSoConMPtr->ChangeModeToOffline)
                {
                    soadSoConMPtr->SoAd_SoConMode = SOAD_SOCON_OFFLINE;
                    soadSoConMPtr->ChangeModeToOffline = FALSE;
                    SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, SoConGroup);
                }
                if ((soadSoConGruMPtr->TcpIpSocketValid) && (soadSoConGruMPtr->SoConSocketValidNum == 0u))
                {
                    (void)TcpIp_Close(soadSoConGruMPtr->TcpIpSocketId, soadSoConMPtr->SoConCloseAbort);
                }
            }
        }
    }
#endif /* STD_ON == SOAD_SUPPORT_TCP */
#if (STD_ON == SOAD_SUPPORT_UDP)
    if (TCPIP_IPPROTO_UDP == soadSoConCfgPtr->SoAd_TcpUdpProtocol)
    {
        if ((0u < soadSoConGruMPtr->SoConSocketValidNum) && soadSoConMPtr->TcpIpSocketValid)
        {
            (soadSoConGruMPtr->SoConSocketValidNum) -= 1u;
        }
        if (0u == soadSoConGruMPtr->SoConSocketValidNum)
        {
            if (CloseSocket)
            {
                (void)TcpIp_Close(soadSoConGruMPtr->TcpIpSocketId, soadSoConMPtr->SoConCloseAbort);
            }
        }
    }
#endif /* STD_ON == SOAD_SUPPORT_UDP */
    /*clear the SoAd_SoConManager parameters*/
    soadSoConMPtr->SoConCloseAbort = FALSE;
    soadSoConMPtr->SocketNeedClose = SOAD_SOCKET_CLOSE_NONE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
    SoAd_SoConManager[SoConId].SetSocketNeedCloseLine = 0;
#endif /* STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG */
    soadSoConMPtr->TpPduTransmitting = FALSE;
    soadSoConMPtr->TxNoAckLength = 0u;
#if (STD_ON == SOAD_SUPPORT_UDP)
    soadSoConMPtr->UdpAliveSupervisionTime = 0u;
#endif /*STD_ON == SOAD_SUPPORT_UDP*/
    /*SWS_SoAd_00527:SoAd shall reset the remote address to the configured remote
      address (or unset the remote address in case no remote address has been
      configured)*/
    if (NULL_PTR != SOAD_SOCONREMOTEADDR_CFG(SoConId))
    {
        soadSoConMPtr->RemoteAddr = *SOAD_SOCONREMOTEADDR_CFG(SoConId);
    }
    return;
}

static FUNC(void, SOAD_CODE)
    SoAd_DisconnectionAndRecoveryHandle(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId, VAR(SoAd_uintx, AUTOMATIC) SoConGroup)
{
    SoAd_InnerSocketManageType* soadSoConMPtr = &SoAd_SoConManager[SoConId];
    SoAd_InnerSocketGroupManageType* soadSoConGruMPtr = &SoAd_SoConGroupManager[SoConGroup];
    const SoAd_SocketConnectionType* soadSoConCfgPtr = &SOAD_SOCON_CFG(SoConId);

    /* handle the disconnection */
    if (SOAD_SOCKET_CLOSE_IMMEDIATE == soadSoConMPtr->SocketNeedClose)
    {
        SoAd_SoConCloseHandle(SoConId, SoConGroup, TRUE);
        /*Notification UP Module SoCon changed*/
        if (soadSoConMPtr->SoAd_SoConMode == SOAD_SOCON_ONLINE)
        {
            soadSoConMPtr->SoAd_SoConMode = SOAD_SOCON_RECONNECT;
            SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, SoConGroup);
        }
    }
    else if (SOAD_SOCON_RECONNECT == soadSoConMPtr->SoAd_SoConMode)
    {
        if ((TCPIP_IPADDR_STATE_ASSIGNED == soadSoConGruMPtr->LocalIpAddrState) && soadSoConMPtr->RemoteAddrIsSet)
        {
#if (STD_ON == SOAD_SUPPORT_UDP)
            /*UDP socket connection recovery*/
            if (TCPIP_IPPROTO_UDP == soadSoConCfgPtr->SoAd_TcpUdpProtocol)
            {
                if (!soadSoConGruMPtr->TcpIpSocketValid)
                {
                    SoAd_OpenUdpSoConHandle(SoConId, SoConGroup);
                }
                else
                {
                    /*SWS_SoAd_00686*/
                    if (SOAD_UDPTYPE_CFG(SoConGroup)->SoAd_SocketUdpListenOnly
                        || ((!SOAD_IS_IPADDR_ANY(soadSoConMPtr->RemoteAddr))
                            && (TCPIP_PORT_ANY != soadSoConMPtr->RemoteAddr.port)))
                    {
                        soadSoConMPtr->SoAd_SoConMode = SOAD_SOCON_ONLINE;
                        SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, SoConGroup);
                    }
                    if (!soadSoConMPtr->TcpIpSocketValid)
                    {
                        soadSoConMPtr->TcpIpSocketValid = TRUE;
                        soadSoConMPtr->TcpIpSocketId = soadSoConGruMPtr->TcpIpSocketId;
                        (soadSoConGruMPtr->SoConSocketValidNum) += 1u;
                    }
                }
            }
#endif /* STD_ON == SOAD_SUPPORT_UDP */
#if (STD_ON == SOAD_SUPPORT_TCP)
            if (TCPIP_IPPROTO_TCP == soadSoConCfgPtr->SoAd_TcpUdpProtocol)
            {
                /*SWS_SoAd_00590*/
                if (SOAD_TCPTYPE_CFG(SoConGroup)->SoAd_SocketTcpInitiate)
                {
                    if (!soadSoConMPtr->TcpIpSocketValid)
                    {
                        SoAd_OpenTcpClientSoConHandle(SoConId, SoConGroup);
                    }
                }
                /*SWS_SoAd_00638*/
                else
                {
                    SoAd_OpenTcpServerSoConHandle(SoConId, SoConGroup);
                }
            }
#endif /* STD_ON == SOAD_SUPPORT_TCP */
        }
    }
    else
    {
        /*do nothing*/
    }
    return;
}

/* id: routing group identifier specifying the routing group
enFlag: group control enable shall be set TRUE otherwise is FALSE
soConIdPtr:socket connection index specifying the socket connection on
which the routing group  */
#if (0u < SOAD_ROUTING_GROUP_MAX)
static FUNC(Std_ReturnType, SOAD_CODE) SoAd_RoutingGroupControl(
    VAR(SoAd_RoutingGroupIdType, AUTOMATIC) id,
    VAR(boolean, AUTOMATIC) enFlag,
    P2CONST(SoAd_SoConIdType, AUTOMATIC, SOAD_APPL_DATA) soConIdPtr)
{
    SoAd_uintx cnt, routeDestId;
    SoAd_RoutingGroupIdType routeGroupId, routeGroupCnt;
    boolean flagSet;
#if (0u < SOAD_SOCKET_ROUTE_DEST_MAX)
    const SoAd_SocketRouteDestType* soRuDestCfgPtr;
#endif
#if (0u < SOAD_PDU_ROUTE_DEST_MAX)
    const SoAd_PduRouteDestType* pduRouDestCfgPtr;
    SoAd_RoutingGroupIdType txRoutGroupRefNum;
#endif

    if (NULL_PTR != soConIdPtr)
    {
        const SoAd_SocketConnectionType* soadSoConCfgPtr = &SOAD_SOCON_CFG(*soConIdPtr);
#if (0u < SOAD_SOCKET_ROUTE_DEST_MAX)
        SoAd_RoutingGroupIdType rxRouteGroupRefNum;
        SoAd_uintx refSocketRouteNum = soadSoConCfgPtr->SoAd_RefSocketRouteNum;

        for (cnt = 0u; cnt < refSocketRouteNum; cnt++)
        {
            routeDestId = soadSoConCfgPtr->SoAd_RefSocketRoutePtr[cnt];
            soRuDestCfgPtr = &SOAD_SRDEST_CFG(routeDestId);
            flagSet = FALSE;
            rxRouteGroupRefNum = soRuDestCfgPtr->SoAd_RxRoutingGroupRefNum;
            for (routeGroupCnt = 0u; (routeGroupCnt < rxRouteGroupRefNum) && (FALSE == flagSet); routeGroupCnt++)
            {
                routeGroupId = soRuDestCfgPtr->SoAd_RxRoutingGroupRefPtr[routeGroupCnt];
                if (id == routeGroupId)
                {
                    SoAd_SocketRouteDestManager[routeDestId].SocketRouteDestEnable = enFlag;
                    flagSet = TRUE;
                }
            }
        }
#endif
#if (0u < SOAD_PDU_ROUTE_DEST_MAX)
        SoAd_uintx SoAd_RefPduRouteDestNum = soadSoConCfgPtr->SoAd_RefPduRouteDestNum;

        for (cnt = 0u; cnt < SoAd_RefPduRouteDestNum; cnt++)
        {
            routeDestId = soadSoConCfgPtr->SoAd_RefPduRouteDestPtr[cnt];
            pduRouDestCfgPtr = &SOAD_PDUROTDEST_CFG(routeDestId);
            flagSet = FALSE;
            txRoutGroupRefNum = pduRouDestCfgPtr->SoAd_TxRoutingGroupRefNum;
            for (routeGroupCnt = 0u; (routeGroupCnt < txRoutGroupRefNum) && (FALSE == flagSet); routeGroupCnt++)
            {
                routeGroupId = pduRouDestCfgPtr->SoAd_TxRoutingGroupRefPtr[routeGroupCnt];
                if (id == routeGroupId)
                {
                    SoAd_PduRouteDestControl(&SoAd_PduRouteDestManager[routeDestId], routeDestId, enFlag, *soConIdPtr);
                    flagSet = TRUE;
                }
            }
        }
#endif
    }
    else
    {
        const SoAd_RoutingGroupType* routGroupCfgPtr = &SoAd_PbCfgPtr->SoAd_RoutingGroup[id];
        SoAd_RoutGroupManager[id].RoutGroupEnable = enFlag;
#if (0u < SOAD_SOCKET_ROUTE_DEST_MAX)
        SoAd_RoutingGroupIdType rxRoutGroupRefNum;
        SoAd_uintx socketRouteDestNum = routGroupCfgPtr->SoAd_SocketRouteDestNum;

        for (cnt = 0u; cnt < socketRouteDestNum; cnt++)
        {
            routeDestId = routGroupCfgPtr->SoAd_SocketRouteDestIndexPtr[cnt];
            soRuDestCfgPtr = &SOAD_SRDEST_CFG(routeDestId);
            flagSet = FALSE;
            rxRoutGroupRefNum = soRuDestCfgPtr->SoAd_RxRoutingGroupRefNum;
            for (routeGroupCnt = 0u; (routeGroupCnt < rxRoutGroupRefNum) && (FALSE == flagSet); routeGroupCnt++)
            {
                routeGroupId = soRuDestCfgPtr->SoAd_RxRoutingGroupRefPtr[routeGroupCnt];
                if (SoAd_RoutGroupManager[routeGroupId].RoutGroupEnable)
                {
                    flagSet = TRUE;
                }
            }
            SoAd_SocketRouteDestManager[routeDestId].SocketRouteDestEnable = flagSet;
        }
#endif
#if (0u < SOAD_PDU_ROUTE_DEST_MAX)
        SoAd_uintx pduRouteDestNum = routGroupCfgPtr->SoAd_PduRouteDestNum;

        for (cnt = 0u; cnt < pduRouteDestNum; cnt++)
        {
            routeDestId = routGroupCfgPtr->SoAd_PduRouteDestIndexPtr[cnt];
            pduRouDestCfgPtr = &SOAD_PDUROTDEST_CFG(routeDestId);
            flagSet = FALSE;
            txRoutGroupRefNum = pduRouDestCfgPtr->SoAd_TxRoutingGroupRefNum;
            for (routeGroupCnt = 0u; (routeGroupCnt < txRoutGroupRefNum) && (FALSE == flagSet); routeGroupCnt++)
            {
                routeGroupId = pduRouDestCfgPtr->SoAd_TxRoutingGroupRefPtr[routeGroupCnt];
                if (SoAd_RoutGroupManager[routeGroupId].RoutGroupEnable)
                {
                    flagSet = TRUE;
                }
            }
            SoAd_PduRouteDestControl(&SoAd_PduRouteDestManager[routeDestId], routeDestId, flagSet, NO_TXMETADATA);
        }
#endif
    }
    return E_OK;
}
#endif /*0u < SOAD_ROUTING_GROUP_MAX*/

#if (0u < SOAD_TXBUFFER_NUM)
static FUNC(void, SOAD_CODE) SoAd_TxBufferInit(SoAd_uintx TxBufferId)
{
    SoAd_SoTxBufManType* soadSoTxBuffMptr = &SoAd_SoTxBufMan[TxBufferId];

#if (STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER)
    soadSoTxBuffMptr->tirgTimer = 0u;
#endif /*STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER*/
    soadSoTxBuffMptr->upCopyLength = 0u;
    soadSoTxBuffMptr->txPduTotalLength = 0u;
    soadSoTxBuffMptr->txResidueLength = SOAD_SOCKETBUFFER_CFG(TxBufferId).bufLen;
    return;
}
#endif /*0u < SOAD_TXBUFFER_NUM*/

#if (0u < SOAD_RXBUFFER_NUM)
static FUNC(void, SOAD_CODE) SoAd_RxBufferInit(SoAd_uintx RxBufferId)
{
    SoAd_SoRxBufferManType* soadSoRxBuffMPtr = &SoAd_SoRxBufMan[RxBufferId];

    soadSoRxBuffMPtr->EnterPosition = 0x0u;
    soadSoRxBuffMPtr->ExitPosition = 0x0u;
    soadSoRxBuffMPtr->ExitPduResidueLength = 0x0u;
    soadSoRxBuffMPtr->SoAd_SocketRouteId = SOAD_UNUSED_UINT16;
    return;
}
#endif /*0u < SOAD_RXBUFFER_NUM*/

#if (STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER)
static FUNC(void, SOAD_CODE) SoAd_MainFunctionNPduUdpTxHandle(SoAd_SoConIdType SoConId)
{
    SoAd_uintx txBufferId;
    SoAd_InnerSocketManageType* soadSoConMPtr = &SoAd_SoConManager[SoConId];

    if (NULL_PTR != SOAD_SOCKET_TXBUF_INDEX_CFG(SoConId))
    {
        txBufferId = *SOAD_SOCKET_TXBUF_INDEX_CFG(SoConId);
        if (0u < SoAd_SoTxBufMan[txBufferId].tirgTimer)
        {
            SoAd_SoTxBufMan[txBufferId].tirgTimer -= 1u;
            if (0u == SoAd_SoTxBufMan[txBufferId].tirgTimer)
            {
                (void)TcpIp_UdpTransmit(
                    soadSoConMPtr->TcpIpSocketId,
                    &SoAd_PduBuffer[SOAD_SOCKETBUFFER_CFG(txBufferId).bufPos],
                    &soadSoConMPtr->RemoteAddr,
                    (uint16)SoAd_SoTxBufMan[txBufferId].txPduTotalLength);
                SoAd_TxBufferInit(txBufferId);
                SoAd_SocnTransmitAutomaticSoConSetupHandle(SoConId, TCPIP_IPPROTO_UDP);
            }
        }
    }
    return;
}
#endif

static FUNC(void, SOAD_CODE) SoAd_MainFunctionTxConfirmationHandle(void)
{
    PduIdType txPduId;
    SoAd_UpLayerModuleType upModule;
    for (txPduId = 0u; txPduId < SOAD_PDU_ROUTE_NUM; txPduId++)
    {
        if (0u == SoAd_PduRoutManager[txPduId].TxPendingNum)
        {
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)

            upModule = SOAD_PDUROUT_CFG(txPduId).SoAd_RefModule;
            if (NULL_PTR != SOAD_UPPERLAY_PCCFG(upModule).If_TxConfirmation_FuncPtr)
            {
                SOAD_UPPERLAY_PCCFG(upModule).If_TxConfirmation_FuncPtr(SOAD_PDUROUT_CFG(txPduId).SoAd_UpPduId);
            }
#endif /* SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u */

            SoAd_PduRouteManagerInit(txPduId);
        }
    }
    return;
}

#if (STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER)
static FUNC(Std_ReturnType, SOAD_CODE)
    SoAd_NPduUdpTransmitHandle(PduIdType TxPduId, SoAd_uintx PduRouteDestId, SoAd_SoConIdType SoConId)
{
    Std_ReturnType result;
    SoAd_uintx txBufferId;
    PduLengthType pduLength;
    SoAd_SoTxBufManType* soadSoTxBufMPtr;
    PduLengthType txBufferPos;
    SoAd_InnerSocketManageType* soadSoConMPtr = &SoAd_SoConManager[SoConId];
    const SoAd_PduRouteDestType* soadPduRouteDestCfgPtr = &SOAD_PDUROTDEST_CFG(PduRouteDestId);

    txBufferId = *SOAD_SOCKET_TXBUF_INDEX_CFG(SoConId);
    pduLength = SoAd_PduRoutManager[TxPduId].PduLength + SOAD_HEADER_SIZE;
    SOAD_FILL_HEADER(
        &SoAd_IfPduBuffer[0],
        soadPduRouteDestCfgPtr->SoAd_TxPduHeaderId[0],
        SoAd_PduRoutManager[TxPduId].PduLength);
    soadSoTxBufMPtr = &SoAd_SoTxBufMan[txBufferId];
    txBufferPos = SOAD_SOCKETBUFFER_CFG(txBufferId).bufPos;
    if (SOAD_TRIGGER_ALWAYS == *(soadPduRouteDestCfgPtr->SoAd_TxUdpTriggerMode))
    {
        if (soadSoTxBufMPtr->txResidueLength < pduLength)
        {
            (void)TcpIp_UdpTransmit(
                soadSoConMPtr->TcpIpSocketId,
                &SoAd_PduBuffer[txBufferPos],
                &soadSoConMPtr->RemoteAddr,
                (uint16)soadSoTxBufMPtr->txPduTotalLength);

            SoAd_TxBufferInit(txBufferId);

            result = TcpIp_UdpTransmit(
                soadSoConMPtr->TcpIpSocketId,
                &SoAd_IfPduBuffer[0],
                &soadSoConMPtr->RemoteAddr,
                (uint16)pduLength);
            SoAd_SocnTransmitAutomaticSoConSetupHandle(SoConId, TCPIP_IPPROTO_UDP);
        }
        else
        {
            (void)ILib_memcpy(
                &SoAd_PduBuffer[txBufferPos + (soadSoTxBufMPtr->txPduTotalLength)],
                &SoAd_IfPduBuffer[0],
                pduLength);
            soadSoTxBufMPtr->txPduTotalLength += pduLength;
            soadSoTxBufMPtr->txResidueLength -= pduLength;
            result = TcpIp_UdpTransmit(
                soadSoConMPtr->TcpIpSocketId,
                &SoAd_PduBuffer[txBufferPos],
                &soadSoConMPtr->RemoteAddr,
                (uint16)soadSoTxBufMPtr->txPduTotalLength);

            SoAd_TxBufferInit(txBufferId);
            SoAd_SocnTransmitAutomaticSoConSetupHandle(SoConId, TCPIP_IPPROTO_UDP);
        }
    }
    else
    {
        if (soadSoTxBufMPtr->txResidueLength < pduLength)
        {
            (void)TcpIp_UdpTransmit(
                soadSoConMPtr->TcpIpSocketId,
                &SoAd_PduBuffer[txBufferPos],
                &soadSoConMPtr->RemoteAddr,
                (uint16)soadSoTxBufMPtr->txPduTotalLength);

            SoAd_TxBufferInit(txBufferId);

            (void)ILib_memcpy(
                &SoAd_PduBuffer[txBufferPos + (soadSoTxBufMPtr->txPduTotalLength)],
                &SoAd_IfPduBuffer[0],
                pduLength);
            soadSoTxBufMPtr->txPduTotalLength += pduLength;
            soadSoTxBufMPtr->txResidueLength -= pduLength;
            soadSoTxBufMPtr->tirgTimer = soadPduRouteDestCfgPtr->SoAd_CfgUdpTriggerTimeout;
            result = E_OK;
        }
        else
        {
            (void)ILib_memcpy(
                &SoAd_PduBuffer[txBufferPos + (soadSoTxBufMPtr->txPduTotalLength)],
                &SoAd_IfPduBuffer[0],
                pduLength);
            soadSoTxBufMPtr->txPduTotalLength += pduLength;
            soadSoTxBufMPtr->txResidueLength -= pduLength;
            if ((soadPduRouteDestCfgPtr->SoAd_CfgUdpTriggerTimeout < soadSoTxBufMPtr->tirgTimer)
                || (soadSoTxBufMPtr->tirgTimer == 0u))
            {
                soadSoTxBufMPtr->tirgTimer = soadPduRouteDestCfgPtr->SoAd_CfgUdpTriggerTimeout;
            }
            result = E_OK;
        }
    }
    if (E_OK == result)
    {
        if (SOAD_UNUSED_UINT8 == SoAd_PduRoutManager[TxPduId].TxPendingNum)
        {
            SoAd_PduRoutManager[TxPduId].TxPendingNum = 0u;
        }
    }
    return result;
}
#endif /*STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER*/

#if (0u < SOAD_MAX_IF_PDU_SIZE)
static FUNC(Std_ReturnType, SOAD_CODE)
    SoAd_IfTransmitSoCon(PduIdType TxPduId, SoAd_uintx PduRouteDestId, SoAd_SoConIdType SoConId)
{
    Std_ReturnType result;
    SoAd_InnerSocketManageType* soadSoConMPtr = &SoAd_SoConManager[SoConId];
    const SoAd_PduRouteDestType* soadPduRouteDestCfgPtr = &SOAD_PDUROTDEST_CFG(PduRouteDestId);
    SoAd_PduRoutInnerManageType* soadPduRouteMPtr = &SoAd_PduRoutManager[TxPduId];

#if (STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER)
    if (NULL_PTR != soadPduRouteDestCfgPtr->SoAd_TxUdpTriggerMode)
    {
        result = SoAd_NPduUdpTransmitHandle(TxPduId, PduRouteDestId, SoConId);
    }
    else
#endif /*STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER*/
    {
        PduLengthType pduLength;
        const uint8* dataPtr;

#if (STD_ON == SOAD_SUPPORT_HEADERID)
        if (NULL_PTR != soadPduRouteDestCfgPtr->SoAd_TxPduHeaderId)
        {
            SOAD_FILL_HEADER(
                &SoAd_IfPduBuffer[0],
                soadPduRouteDestCfgPtr->SoAd_TxPduHeaderId[0],
                soadPduRouteMPtr->PduLength);
            pduLength = soadPduRouteMPtr->PduLength + SOAD_HEADER_SIZE;
            dataPtr = &SoAd_IfPduBuffer[0];
        }
        else
#endif /*STD_ON == SOAD_SUPPORT_HEADERID*/
        {
            pduLength = soadPduRouteMPtr->PduLength;
            dataPtr = soadPduRouteMPtr->PduDataPtr;
        }
        if (TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(SoConId).SoAd_TcpUdpProtocol)
        {
            /*transmit success manage parameter set*/
            soadSoConMPtr->TxNoAckLength += pduLength;
            SoAd_PduRouteDestManager[PduRouteDestId].TxPendingLength = soadSoConMPtr->TxNoAckLength;
            if (SOAD_UNUSED_UINT8 == soadPduRouteMPtr->TxPendingNum)
            {
                soadPduRouteMPtr->TxPendingNum = 1u;
            }
            else
            {
                soadPduRouteMPtr->TxPendingNum += 1u;
            }
            result = TcpIp_TcpTransmit(soadSoConMPtr->TcpIpSocketId, dataPtr, pduLength, TRUE);
            if (E_OK != result)
            {
                soadSoConMPtr->TxNoAckLength -= pduLength;
                SoAd_PduRouteDestManager[PduRouteDestId].TxPendingLength = 0u;
                if (1u == soadPduRouteMPtr->TxPendingNum)
                {
                    soadPduRouteMPtr->TxPendingNum = SOAD_UNUSED_UINT8;
                }
                else
                {
                    soadPduRouteMPtr->TxPendingNum -= 1u;
                }
            }
        }
        else
        {
            result =
                TcpIp_UdpTransmit(soadSoConMPtr->TcpIpSocketId, dataPtr, &soadSoConMPtr->RemoteAddr, (uint16)pduLength);
            if (E_OK == result)
            {
#if (STD_ON == SOAD_UDP_SOCON_IMMEDIATE_TXCONFIRMATION)
                SoAd_uintx soConGroup = SOAD_SOCON_CFG(SoConId).SoAd_SocketConnectionGroupRef;
                if ((SOAD_UDPTYPE_CFG(soConGroup)->SoAd_SocketUdpImmediateTxConfirmEnabled)
                    && (SOAD_PDUROUTE_CFG(TxPduId).SoAd_UDPImmediateTxConfirmation))
                {
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
                    SoAd_UpLayerModuleType upModule = SOAD_PDUROUT_CFG(TxPduId).SoAd_RefModule;
                    if (NULL_PTR != SOAD_UPPERLAY_PCCFG(upModule).If_TxConfirmation_FuncPtr)
                    {
                        SOAD_UPPERLAY_PCCFG(upModule).If_TxConfirmation_FuncPtr(SOAD_PDUROUT_CFG(TxPduId).SoAd_UpPduId);
                    }
#endif /* SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u */

                    SoAd_PduRouteManagerInit(TxPduId);
                }
                else
#endif /* STD_ON == SOAD_UDP_SOCON_IMMEDIATE_TXCONFIRMATION */
                    if (SOAD_UNUSED_UINT8 == soadPduRouteMPtr->TxPendingNum)
                    {
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
                        SoAd_UpLayerModuleType upModule = SOAD_PDUROUT_CFG(TxPduId).SoAd_RefModule;
                        /* Return immediately without configuration TxConfirmation for Udp */
                        if (NULL_PTR != SOAD_UPPERLAY_PCCFG(upModule).If_TxConfirmation_FuncPtr)
#endif
                        {
                            soadPduRouteMPtr->TxPendingNum = 0u;
                        }
                    }
            }
            SoAd_SocnTransmitAutomaticSoConSetupHandle(SoConId, TCPIP_IPPROTO_UDP);
        }
    }
    if (E_NOT_OK == result)
    {
        soadPduRouteMPtr->TxPendingNum = SOAD_UNUSED_UINT8;
    }
    return result;
}
#endif /*0u < SOAD_MAX_IF_PDU_SIZE*/

#if (0u < SOAD_TXBUFFER_NUM)
static FUNC(void, SOAD_CODE) SoAd_MainFunctionTpTransmitHandle(SoAd_SoConIdType SoConId)
{
    SoAd_uintx txBufferId;
    SoAd_SoTxBufManType* soadSoTxBufMPtr;
    SoAd_InnerSocketManageType* soadSoConMPtr = &SoAd_SoConManager[SoConId];

    txBufferId = *SOAD_SOCKET_TXBUF_INDEX_CFG(SoConId);
    soadSoTxBufMPtr = &SoAd_SoTxBufMan[txBufferId];
    if (soadSoConMPtr->TpPduTransmitting && (SOAD_SOCKET_CLOSE_NONE == soadSoConMPtr->SocketNeedClose)
        && (soadSoTxBufMPtr->upCopyLength < soadSoTxBufMPtr->txPduTotalLength))
    {
        PduInfoType PduInfo;
        PduIdType pduRouteId, upPduId;
        SoAd_UpLayerModuleType upModule;
        PduLengthType validDataLength;
        BufReq_ReturnType result = BUFREQ_E_NOT_OK;
        Std_ReturnType resultLo;
        PduLengthType txBufferPos = SOAD_SOCKETBUFFER_CFG(txBufferId).bufPos;

        PduInfo.MetaDataPtr = NULL_PTR;
        PduInfo.SduLength = 0u;
        PduInfo.SduDataPtr = NULL_PTR;
        pduRouteId = soadSoConMPtr->ActiveSoAdPduRouteId;
        upPduId = SOAD_PDUROUT_CFG(pduRouteId).SoAd_UpPduId;
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
        upModule = SOAD_PDUROUT_CFG(pduRouteId).SoAd_RefModule;
        if (NULL_PTR != SOAD_UPPERLAY_PCCFG(upModule).Tp_CopyTxData_FuncPtr)
        {
            result = SOAD_UPPERLAY_PCCFG(upModule).Tp_CopyTxData_FuncPtr(upPduId, &PduInfo, NULL_PTR, &validDataLength);
            if ((BUFREQ_OK == result)
                && (soadSoTxBufMPtr->txPduTotalLength >= (soadSoTxBufMPtr->upCopyLength + validDataLength)))
            {
                SchM_Enter_SoAd_ExclusiveArea();
                if ((TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(SoConId).SoAd_TcpUdpProtocol)
                    && (SOAD_SOCKET_CLOSE_NONE == soadSoConMPtr->SocketNeedClose))
                {
                    if (E_NOT_OK == TcpIp_TcpTransmit(soadSoConMPtr->TcpIpSocketId, NULL_PTR, validDataLength, FALSE))
                    {
                        result = BUFREQ_E_NOT_OK;
                    }
                }
                else
                {
                    PduInfo.SduDataPtr = &SoAd_PduBuffer[txBufferPos + soadSoTxBufMPtr->upCopyLength];
                    PduInfo.SduLength = validDataLength;
                    result = SOAD_UPPERLAY_PCCFG(upModule)
                                 .Tp_CopyTxData_FuncPtr(upPduId, &PduInfo, NULL_PTR, &validDataLength);
                    if (BUFREQ_OK == result)
                    {
                        soadSoTxBufMPtr->upCopyLength += PduInfo.SduLength;
                        if (soadSoTxBufMPtr->txPduTotalLength == soadSoTxBufMPtr->upCopyLength)
                        {
                            resultLo = TcpIp_UdpTransmit(
                                soadSoConMPtr->TcpIpSocketId,
                                &SoAd_PduBuffer[txBufferPos],
                                &soadSoConMPtr->RemoteAddr,
                                (uint16)soadSoTxBufMPtr->txPduTotalLength);
                            if (E_OK == resultLo)
                            {
                                soadSoTxBufMPtr->txPduTotalLength = 0u;
                                soadSoTxBufMPtr->upCopyLength = 0u;
                                soadSoConMPtr->TpPduTransmitting = FALSE;
                                if (NULL_PTR != SOAD_UPPERLAY_PCCFG(upModule).Tp_TxConfirmation_FuncPtr)
                                {
                                    SOAD_UPPERLAY_PCCFG(upModule).Tp_TxConfirmation_FuncPtr(upPduId, E_OK);
                                }
                            }
                            else
                            {
                                result = BUFREQ_E_NOT_OK;
                            }
                            SoAd_SocnTransmitAutomaticSoConSetupHandle(SoConId, TCPIP_IPPROTO_UDP);
                        }
                    }
                }
                SchM_Exit_SoAd_ExclusiveArea();
            }
            else
            {
                result = BUFREQ_E_NOT_OK;
            }
        }
        if (BUFREQ_E_NOT_OK == result)
        {
            SoAd_TpPduTxErrorHandle(SoConId, txBufferId, upModule, upPduId);
        }
#endif /* SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u */
    }
    return;
}
#endif /*0u < SOAD_TXBUFFER_NUM*/

#if (0u < SOAD_TXBUFFER_NUM)
static FUNC(void, SOAD_CODE) SoAd_TpPduTxErrorHandle(
    SoAd_SoConIdType SoConId,
    SoAd_uintx TxBufferId,
    SoAd_UpLayerModuleType UpModule,
    PduIdType UpPduId)
{
    if (TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(SoConId).SoAd_TcpUdpProtocol)
    {
        SoAd_SoConManager[SoConId].SocketNeedClose = SOAD_SOCKET_CLOSE_IMMEDIATE;
#if (STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG)
        SoAd_SoConManager[SoConId].SetSocketNeedCloseLine = __LINE__;
#endif /* STD_ON == SOAD_SET_SOCKET_NEED_CLOSE_DEBUG */
    }
    else
    {
        SoAd_TxBufferInit(TxBufferId);
        SoAd_SoConManager[SoConId].TpPduTransmitting = FALSE;
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)

        if (NULL_PTR != SOAD_UPPERLAY_PCCFG(UpModule).Tp_TxConfirmation_FuncPtr)
        {
            SOAD_UPPERLAY_PCCFG(UpModule).Tp_TxConfirmation_FuncPtr(UpPduId, E_NOT_OK);
        }
#endif /* SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u */
    }
    return;
}
#endif /*0u < SOAD_TXBUFFER_NUM*/

#if (0u < SOAD_TXBUFFER_NUM)
static FUNC(Std_ReturnType, SOAD_CODE) SoAd_TpTransmitSoCon(
    SoAd_SoConIdType SoConId,
    SoAd_uintx PduRouteDestId,
    P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_CONST) SoAdSrcPduInfoPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    SoAd_uintx txBufferId = *SOAD_SOCKET_TXBUF_INDEX_CFG(SoConId);
    SoAd_SoTxBufManType* soadSoTxBufMPtr = &SoAd_SoTxBufMan[txBufferId];
    PduLengthType bufferLen = SOAD_SOCKETBUFFER_CFG(txBufferId).bufLen;

    if ((bufferLen >= SoAdSrcPduInfoPtr->SduLength) || (0u == bufferLen))
    {
        SoAd_InnerSocketManageType* soadSoConMPtr = &SoAd_SoConManager[SoConId];

#if (STD_ON == SOAD_SUPPORT_HEADERID)
        if (NULL_PTR != SOAD_PDUROTDEST_CFG(PduRouteDestId).SoAd_TxPduHeaderId)
        {
            if (TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(SoConId).SoAd_TcpUdpProtocol)
            {
                uint8 tcpHeaderData[SOAD_HEADER_SIZE];
                SOAD_FILL_HEADER(
                    &tcpHeaderData[0],
                    *SOAD_PDUROTDEST_CFG(PduRouteDestId).SoAd_TxPduHeaderId,
                    SoAdSrcPduInfoPtr->SduLength);
                /*transmit success manage parameter set*/
                soadSoConMPtr->TxNoAckLength += SOAD_HEADER_SIZE;
                SoAd_PduRouteDestManager[PduRouteDestId].TxPendingLength = soadSoConMPtr->TxNoAckLength;
                soadSoTxBufMPtr->txPduTotalLength = (SoAdSrcPduInfoPtr->SduLength + SOAD_HEADER_SIZE);
                soadSoTxBufMPtr->upCopyLength = SOAD_HEADER_SIZE;
                soadSoConMPtr->TpPduTransmitting = TRUE;
                ret = TcpIp_TcpTransmit(soadSoConMPtr->TcpIpSocketId, &tcpHeaderData[0], SOAD_HEADER_SIZE, TRUE);
                if (E_OK != ret)
                {
                    soadSoConMPtr->TxNoAckLength -= SOAD_HEADER_SIZE;
                    SoAd_PduRouteDestManager[PduRouteDestId].TxPendingLength = 0u;
                    soadSoTxBufMPtr->txPduTotalLength = 0u;
                    soadSoTxBufMPtr->upCopyLength = 0u;
                    soadSoConMPtr->TpPduTransmitting = FALSE;
                }
            }
            else
            {
                if (bufferLen >= (SoAdSrcPduInfoPtr->SduLength + SOAD_HEADER_SIZE))
                {
                    SOAD_FILL_HEADER(
                        &SoAd_PduBuffer[SOAD_SOCKETBUFFER_CFG(txBufferId).bufPos],
                        *SOAD_PDUROTDEST_CFG(PduRouteDestId).SoAd_TxPduHeaderId,
                        SoAdSrcPduInfoPtr->SduLength);
                    soadSoTxBufMPtr->txPduTotalLength = (SoAdSrcPduInfoPtr->SduLength + SOAD_HEADER_SIZE);
                    soadSoTxBufMPtr->upCopyLength = SOAD_HEADER_SIZE;
                    soadSoConMPtr->TpPduTransmitting = TRUE;
                    ret = E_OK;
                }
            }
        }
        else
#endif /*STD_ON == SOAD_SUPPORT_HEADERID*/
        {
            soadSoTxBufMPtr->txPduTotalLength = SoAdSrcPduInfoPtr->SduLength;
            soadSoTxBufMPtr->upCopyLength = 0u;
            soadSoConMPtr->TpPduTransmitting = TRUE;
            ret = E_OK;
        }
    }
    return ret;
}
#endif /*0u < SOAD_TXBUFFER_NUM*/

/*Called by SoAd_TcpIpEvent.
 *Close the Udp SoCon Group*/
#if (STD_ON == SOAD_SUPPORT_UDP)
static FUNC(void, SOAD_CODE)
    SoAd_CloseUdpSoConGroup(VAR(TcpIp_EventType, AUTOMATIC) Event, VAR(SoAd_uintx, AUTOMATIC) SoConGroupId)
{
    SoAd_SoConIdType soConCnt, soConId;
    /*SWS_SoAd_00643:TCPIP_UDP_CLOSED for a udp socket connection group*/
    if (TCPIP_UDP_CLOSED == Event)
    {
        SoAd_SoConIdType soceketConNum = SOAD_SOCONGROUP_CFG(SoConGroupId).SoAd_SoConNum;

        for (soConCnt = 0u; soConCnt < soceketConNum; soConCnt++)
        {
            soConId = SOAD_SOCONGROUP_CFG(SoConGroupId).SoAd_SoConId[soConCnt];
            SoAd_SoConManager[soConId].TcpIpSocketValid = FALSE;
            if (SOAD_SOCON_ONLINE == SoAd_SoConManager[soConId].SoAd_SoConMode)
            {
                SoAd_SoConCloseHandle(soConId, SoConGroupId, FALSE);
                /*Notification UP Module SoCon changed*/
                SoAd_SoConManager[soConId].SoAd_SoConMode = SOAD_SOCON_RECONNECT;
                SoAd_SoConModeChgNotificationAndTpStartOfReception(soConId, SoConGroupId);
            }
        }
        SoAd_SoConGroupManager[SoConGroupId].TcpIpSocketValid = FALSE;
        SoAd_SoConGroupManager[SoConGroupId].SoConSocketValidNum = 0u;
    }
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /*SWS_SoAd_00278*/
    else
    {
        SOAD_DET_REPORT(SOAD_SID_TCPIPEVENT, SOAD_E_INV_ARG);
    }
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    return;
}
#endif /* STD_ON == SOAD_SUPPORT_UDP */

/*Called by SoAd_TxConfirmation.
 *Tcp TxConfirmation handle*/
#if (STD_ON == SOAD_SUPPORT_TCP)
static FUNC(void, SOAD_CODE) SoAd_TxConfirmationHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) soConId,
    VAR(uint16, AUTOMATIC) Length,
    VAR(SoAd_uintx, AUTOMATIC) pduRouteDestId)
{
    PduIdType pduRouteId;
    if (SoAd_PduRouteDestManager[pduRouteDestId].TxPendingLength <= Length)
    {
        SoAd_PduRouteDestManager[pduRouteDestId].TxPendingLength = 0u;
        pduRouteId = SOAD_PDUROTDEST_CFG(pduRouteDestId).SoAd_PduRouteIndex;
#if (0u < SOAD_TXBUFFER_NUM)
        if (SOAD_UPPER_LAYER_TP == SOAD_PDUROUT_CFG(pduRouteId).SoAd_TxUpperLayerType)
        {
            if (!SoAd_SoConManager[soConId].TpPduTransmitting)
            {
                /*here is the really TxConfirmation for SoAdSocketTcpImmediateTpTxConfirmation*/
                SoAd_SocnTransmitAutomaticSoConSetupHandle(soConId, TCPIP_IPPROTO_TCP);
            }
            else
            {
                /*Tp Pdu TxConfirmation handle*/
                SoAd_uintx txBufferId;
                txBufferId = *SOAD_SOCKET_TXBUF_INDEX_CFG(soConId);
                if (SoAd_SoTxBufMan[txBufferId].upCopyLength == SoAd_SoTxBufMan[txBufferId].txPduTotalLength)
                {
                    SoAd_UpLayerModuleType upModule;
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
                    upModule = SOAD_PDUROUT_CFG(pduRouteId).SoAd_RefModule;
                    if (NULL_PTR != SOAD_UPPERLAY_PCCFG(upModule).Tp_TxConfirmation_FuncPtr)
                    {
                        SOAD_UPPERLAY_PCCFG(upModule).Tp_TxConfirmation_FuncPtr(
                            SOAD_PDUROUT_CFG(pduRouteId).SoAd_UpPduId,
                            E_OK);
                    }
#endif /* SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u */
                    SoAd_SoTxBufMan[txBufferId].txPduTotalLength = 0u;
                    SoAd_SoTxBufMan[txBufferId].upCopyLength = 0u;
                    SoAd_SoConManager[soConId].TpPduTransmitting = FALSE;
                    SoAd_SocnTransmitAutomaticSoConSetupHandle(soConId, TCPIP_IPPROTO_TCP);
                }
            }
        }
        /*If Pdu TxConfirmation handle*/
        else
#endif /*0u < SOAD_TXBUFFER_NUM*/
        {
            if (SoAd_PduRoutManager[pduRouteId].TxPendingNum > 0u)
            {
                SoAd_PduRoutManager[pduRouteId].TxPendingNum -= 1u;
                if (0u == SoAd_PduRoutManager[pduRouteId].TxPendingNum)
                {
                    SoAd_UpLayerModuleType upModule;
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
                    upModule = SOAD_PDUROUT_CFG(pduRouteId).SoAd_RefModule;
                    if (NULL_PTR != SOAD_UPPERLAY_PCCFG(upModule).If_TxConfirmation_FuncPtr)
                    {
                        SOAD_UPPERLAY_PCCFG(upModule).If_TxConfirmation_FuncPtr(
                            SOAD_PDUROUT_CFG(pduRouteId).SoAd_UpPduId);
                    }
#endif /* SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u */
                    SoAd_PduRouteManagerInit(pduRouteId);
                    SoAd_SocnTransmitAutomaticSoConSetupHandle(soConId, TCPIP_IPPROTO_TCP);
                }
            }
        }
    }
    else
    {
        SoAd_PduRouteDestManager[pduRouteDestId].TxPendingLength -= Length;
    }
    return;
}
#endif

#if (STD_ON == SOAD_SUPPORT_HEADERID)
static boolean SoAd_FindHeaderIDAndSocketRouteId(SoAd_SoConIdType SoConId, uint32 HeaderId, SoAd_uintx* SocketRouteID)
{
    SoAd_uintx cnt;
    SoAd_uintx soRouteId;
    boolean findValidHeaderId = FALSE;
    SoAd_uintx socketRouteNum = SOAD_SOCON_CFG(SoConId).SoAd_RefSocketRouteNum;

    for (cnt = 0u; cnt < socketRouteNum; cnt++)
    {
        soRouteId = SOAD_SOCON_CFG(SoConId).SoAd_RefSocketRoutePtr[cnt];
        if (HeaderId == *SOAD_SOCKETROUTE_CFG(soRouteId).SoAd_RxPduHeaderId)
        {
            findValidHeaderId = TRUE;
            *SocketRouteID = soRouteId;
            break;
        }
    }

    return findValidHeaderId;
}
#endif /* SOAD_SUPPORT_HEADERID */

static void SoAd_PduRouteDestControl(
    SoAd_PduRouteDestInnerManagerType* pduRouteDestMPtr,
    SoAd_uintx pduRouteDestId,
    boolean enFlag,
    SoAd_SoConIdType specificSoConId)
{
    SoAd_SoConIdType refSoConNum = SOAD_PDUROTDEST_CFG(pduRouteDestId).SoAd_TxPduRouteDestSocketConRefNum;
    for (SoAd_SoConIdType cnt = 0u; cnt < refSoConNum; cnt++)
    {
        SoAd_SoConIdType soConId = SOAD_PDUROTDEST_CFG(pduRouteDestId).SoAd_TxSocketConRefIndexPtr[cnt];
        if ((specificSoConId == NO_TXMETADATA) || ((specificSoConId != NO_TXMETADATA) && (specificSoConId == soConId)))
        {
            if (enFlag)
            {
                pduRouteDestMPtr->PduRouteDestRefSoConEnable[soConId / 8] |= (uint8)(0x01 << (soConId % 8u));
            }
            else
            {
                pduRouteDestMPtr->PduRouteDestRefSoConEnable[soConId / 8] &= (uint8)(~(0x01 << (soConId % 8u)));
            }
        }
    }
}

static boolean SoAd_PduRouteDestIsEnable(SoAd_uintx pduRouteDestId, SoAd_SoConIdType soConId)
{
    uint8 enable = SoAd_PduRouteDestManager[pduRouteDestId].PduRouteDestRefSoConEnable[soConId / 8];
    return ((enable & (uint8)(0x01 << (soConId % 8))) > 0);
}
#define SOAD_STOP_SEC_CODE
#include "SoAd_MemMap.h"
