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
**************************************************************************** **
**                                                                           **
**  FILENAME    : SoAd_Types.h                                               **
**                                                                           **
**  Created on  :                                                            **
**  Author      : zhengfei.li                                                **
**  Vendor      :                                                            **
**  DESCRIPTION : SoAd header description                                    **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2 and R19_11           **
**                                                                           **
**************************************************************************** */
#ifndef SOAD_TYPES_H_
#define SOAD_TYPES_H_

#include "SoAd_Cfg.h"
#include "ComStack_Types.h"
#include "TcpIp.h"
/*==============================================================================
*                                             ENUMS
==============================================================================*/
/*AUTOSAR standard definition type*/
#if (0x100u > SOAD_SO_CON_MAX)
typedef uint8 SoAd_SoConIdType;
#define NO_TXMETADATA 0xFFu
#else
typedef uint16 SoAd_SoConIdType;
#define NO_TXMETADATA 0xFFFFu
#endif /* 0x100u > SOAD_SO_CON_MAX */

#if (0x100u > SOAD_ROUTING_GROUP_MAX)
typedef uint8 SoAd_RoutingGroupIdType;
#else
typedef uint16 SoAd_RoutingGroupIdType;
#endif /* 0x100u > SOAD_ROUTING_GROUP_MAX */

typedef enum
{
    SOAD_SOCON_ONLINE = 0u,
    SOAD_SOCON_RECONNECT,
    SOAD_SOCON_OFFLINE
} SoAd_SoConModeType;

typedef enum
{
    SOAD_SOCON_MATCH_REQUEST_NONE = 0u,
    SOAD_SOCON_MATCH_REQUEST_RECONNECT,
    SOAD_SOCON_MATCH_REQUEST_ONLINE_RECONNECT
} SoAd_BestMatchStateType;

/*SoAd Internal unknown type definition*/
typedef uint16 SoAd_uintx;
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
/*PC configuration type definition*/
typedef struct
{
    P2FUNC(void, SOAD_APPL_CODE, If_RxIndication_FuncPtr)(PduIdType RxPduId, const PduInfoType* PduInfoPtr);
    P2FUNC(Std_ReturnType, SOAD_APPL_CODE, If_TriggerTransmit_FuncPtr)(PduIdType TxPduId, PduInfoType* PduInfoPtr);
    P2FUNC(void, SOAD_APPL_CODE, If_TxConfirmation_FuncPtr)(PduIdType TxPduId);
    P2FUNC(BufReq_ReturnType, SOAD_APPL_CODE, Tp_StartOfReception_FuncPtr)
    (PduIdType RxPduId, const PduInfoType* info, PduLengthType TpSduLength, PduLengthType* bufferSizePtr);
    P2FUNC(BufReq_ReturnType, SOAD_APPL_CODE, Tp_CopyRxData_FuncPtr)
    (PduIdType id, const PduInfoType* info, PduLengthType* bufferSizePtr);
    P2FUNC(void, SOAD_APPL_CODE, Tp_RxIndication_FuncPtr)(PduIdType RxPduId, Std_ReturnType result);
    P2FUNC(BufReq_ReturnType, SOAD_APPL_CODE, Tp_CopyTxData_FuncPtr)
    (PduIdType id, const PduInfoType* info, const RetryInfoType* retry, PduLengthType* availableDataPtr);
    P2FUNC(void, SOAD_APPL_CODE, Tp_TxConfirmation_FuncPtr)(PduIdType TxPduId, Std_ReturnType result);
    P2FUNC(void, SOAD_APPL_CODE, SoConModeChg_FuncPtr)(SoAd_SoConIdType SoConId, SoAd_SoConModeType Mode);
    P2FUNC(void, SOAD_APPL_CODE, LocalIpAddrAssignmentChg_FuncPtr)
    (SoAd_SoConIdType SoConId, TcpIp_IpAddrStateType State);
} SoAd_UpLayerFuncType;
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
typedef struct
{
    SoAd_UpLayerFuncType SoAd_UpLayerFuncTalbe[SOAD_SUPPORT_UPPERLAYMODULE_MAX];
} SoAd_PcCfgType;
#endif /* SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u */

/*PB configuration type definition*/
typedef enum
{
#if (STD_ON == SOAD_PDUR)
    SOAD_UP_MODULE_PDUR,
#endif /* STD_ON == SOAD_PDUR */
#if (STD_ON == SOAD_UDPNM)
    SOAD_UP_MODULE_UDPNM,
#endif /* STD_ON == SOAD_UDPNM */
#if (STD_ON == SOAD_XCP)
    SOAD_UP_MODULE_XCP,
#endif /* STD_ON == SOAD_XCP */
#if (STD_ON == SOAD_SD)
    SOAD_UP_MODULE_SD,
#endif /* STD_ON == SOAD_SD */
#if (STD_ON == SOAD_DOIP)
    SOAD_UP_MODULE_DOIP,
#endif /* STD_ON == SOAD_DOIP */
#if (STD_ON == SOAD_CDD)
    SOAD_UP_MODULE_CDD,
#endif /* STD_ON == SOAD_CDD */
    SOAD_UP_MODULE_NONE
} SoAd_UpLayerModuleType;

typedef struct
{
    PduLengthType bufLen;
    PduLengthType bufPos;
} SoAd_SoBufferCfgType;

#if (STD_ON == SOAD_SUPPORT_UDP)
typedef struct
{
    boolean SoAd_SocketUdpListenOnly;
    boolean SoAd_SocketUdpStrictHeaderLenCheckEnabled;
    CONSTP2CONST(uint32, TYPEDEF, SOAD_CONST_PBCFG)
    SoAd_SocketUdpAliveSupervisionTimeout;
#if (SOAD_UDP_SOCON_IMMEDIATE_TXCONFIRMATION == STD_ON)
    boolean SoAd_SocketUdpImmediateTxConfirmEnabled;
#endif /* SOAD_UDP_SOCON_IMMEDIATE_TXCONFIRMATION == STD_ON */
} SoAd_SocketUdpType;
#endif /* STD_ON == SOAD_SUPPORT_UDP */

#if (STD_ON == SOAD_SUPPORT_TCP)
typedef struct
{
    boolean SoAd_SocketTcpImmediateTpTxConfirmation;
    boolean SoAd_SocketTcpInitiate;
    boolean SoAd_SocketTcpKeepAlive;
    CONSTP2CONST(uint32, TYPEDEF, SOAD_CONST_PBCFG)
    SoAd_SocketTcpKeepAliveInterval;
    CONSTP2CONST(uint16, TYPEDEF, SOAD_CONST_PBCFG)
    SoAd_SocketTcpKeepAliveProbesMax;
    CONSTP2CONST(uint32, TYPEDEF, SOAD_CONST_PBCFG)
    SoAd_SocketTcpKeepAliveTime;
    CONSTP2CONST(boolean, TYPEDEF, SOAD_CONST_PBCFG)
    SoAd_SocketTcpNoDelay;
    CONSTP2CONST(uint32, TYPEDEF, SOAD_CONST_PBCFG)
    SoAd_SocketTcpTxQuota;
    CONSTP2CONST(uint8, TYPEDEF, SOAD_CONST_PBCFG)
    SoAdSocketTCPOptionFilterId;
    CONSTP2CONST(uint16, TYPEDEF, SOAD_CONST_PBCFG)
    SoAdSocketTcpTlsConnectionId;
} SoAd_SocketTcpType;
#endif /* STD_ON == SOAD_SUPPORT_TCP */

typedef struct
{
#if (STD_ON == SOAD_SUPPORT_TCP)
    CONSTP2CONST(SoAd_SocketTcpType, TYPEDEF, SOAD_CONST_PBCFG)
    SoAd_SocketTcpPtr;
#endif /* STD_ON == SOAD_SUPPORT_TCP */
#if (STD_ON == SOAD_SUPPORT_UDP)
    CONSTP2CONST(SoAd_SocketUdpType, TYPEDEF, SOAD_CONST_PBCFG)
    SoAd_SocketUdpPtr;
#endif /* STD_ON == SOAD_SUPPORT_UDP */
} SoAd_SocketProtocolType;

typedef struct
{
    TcpIp_DomainType DomainType;
    TcpIp_LocalAddrIdType LocalAddrId;
} SoAd_SocketLocalAddressRefType;

typedef struct
{
    SoAd_SoConIdType SoAd_SocketId;
    SoAd_uintx SoAd_SocketConnectionGroupRef;
    CONSTP2CONST(TcpIp_SockAddrType, TYPEDEF, SOAD_CONST_PBCFG)
    SoAd_SocketRemoteAddress;
    uint8 SoAd_RefUpNum;
    CONSTP2CONST(SoAd_UpLayerModuleType, TYPEDEF, SOAD_CONST_PBCFG)
    SoAd_RefUpModulePtr;
    SoAd_uintx SoAd_RefSocketRouteNum;
    CONSTP2CONST(SoAd_uintx, TYPEDEF, SOAD_CONST_PBCFG)
    SoAd_RefSocketRoutePtr;
    CONSTP2CONST(SoAd_uintx, TYPEDEF, SOAD_CONST_PBCFG)
    SoAd_TxBufCfgIndexPtr;
    CONSTP2CONST(SoAd_uintx, TYPEDEF, SOAD_CONST_PBCFG)
    SoAd_RxBufCfgIndexPtr;
    TcpIp_ProtocolType SoAd_TcpUdpProtocol;
    SoAd_uintx SoAd_RefPduRouteDestNum;
    CONSTP2CONST(SoAd_uintx, TYPEDEF, SOAD_CONST_PBCFG)
    SoAd_RefPduRouteDestPtr;
} SoAd_SocketConnectionType;

typedef struct
{
    boolean SoAd_PduHeaderEnable;
    boolean SoAd_ResourceManagementEnable;
    boolean SoAd_SocketAutomaticSoConSetup;
    boolean SoAd_SocketMsgAcceptanceFilterEnabled;
    boolean SoAd_SocketIpAddrAssignmentChgNotification;
    boolean SoAd_SocketSoConModeChgNotification;
    uint16 SoAd_SocketLocalPort;
    CONSTP2CONST(uint8, TYPEDEF, SOAD_CONST_PBCFG)
    SoAd_SocketFramePriority;
    CONSTP2CONST(uint8, TYPEDEF, SOAD_CONST_PBCFG)
    SoAdSocketDifferentiatedServicesField;
    CONSTP2CONST(boolean, TYPEDEF, SOAD_CONST_PBCFG)
    SoAdSocketPathMTUEnable;
    CONSTP2CONST(uint32, TYPEDEF, SOAD_CONST_PBCFG)
    SoAdSocketFlowLabel;
    CONSTP2CONST(SoAd_uintx, TYPEDEF, SOAD_CONST_PBCFG)
    SoAd_SocketTpRxBufferMin;
    SoAd_SocketLocalAddressRefType SoAd_SocketLocalAddressRef;
    SoAd_SocketProtocolType SoAd_SocketProtocol;
    SoAd_SoConIdType SoAd_SoConNum;
    CONSTP2CONST(SoAd_SoConIdType, TYPEDEF, SOAD_CONST_PBCFG)
    SoAd_SoConId;
#if (SOAD_SOCKET_VLAN == STD_ON)
    CONSTP2CONST(uint32, TYPEDEF, SOAD_CONST_PBCFG)
    SoAdSocketVlan;
#endif /* SOAD_SOCKET_VLAN == STD_ON */
} SoAd_SocketConnectionGroupType;

typedef enum
{
    SOAD_UPPER_LAYER_IF,
    SOAD_UPPER_LAYER_TP
} SoAd_UpperLayerType;

typedef struct
{
    PduIdType SoAd_RxPduId;
    SoAd_UpperLayerType SoAd_RxUpperLayerType;
    SoAd_RoutingGroupIdType SoAd_RxRoutingGroupRefNum;
    CONSTP2CONST(SoAd_RoutingGroupIdType, TYPEDEF, SOAD_CONST_PBCFG)
    SoAd_RxRoutingGroupRefPtr;
    SoAd_UpLayerModuleType SoAd_RefModule;
    PduIdType SoAd_UpPduId;
    boolean SoAd_MetaDataEnable;
} SoAd_SocketRouteDestType;

typedef struct
{
#if (STD_ON == SOAD_SUPPORT_HEADERID)
    CONSTP2CONST(uint32, TYPEDEF, SOAD_CONST_PBCFG)
    SoAd_RxPduHeaderId;
#endif /* STD_ON == SOAD_SUPPORT_HEADERID */
    SoAd_SoConIdType SoAd_RxSocketConnRefNum;
    CONSTP2CONST(SoAd_SoConIdType, TYPEDEF, SOAD_CONST_PBCFG)
    SoAd_RxSocketConnRefIndexPtr;
} SoAd_SocketRouteType;

typedef enum
{
    SOAD_TRIGGER_ALWAYS,
    SOAD_TRIGGER_NEVER
} SoAd_TriggerModeType;

/* Container which specifies the PDU route destination. */
typedef struct
{
#if (STD_ON == SOAD_SUPPORT_HEADERID)
    CONSTP2CONST(uint32, TYPEDEF, SOAD_CONST_PBCFG)
    SoAd_TxPduHeaderId;
#endif /* STD_ON == SOAD_SUPPORT_HEADERID */
#if (STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER)
    CONSTP2CONST(SoAd_TriggerModeType, TYPEDEF, SOAD_CONST_PBCFG)
    SoAd_TxUdpTriggerMode;
    uint32 SoAd_CfgUdpTriggerTimeout;
#endif /* STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER */
    SoAd_RoutingGroupIdType SoAd_TxRoutingGroupRefNum;
    CONSTP2CONST(SoAd_RoutingGroupIdType, TYPEDEF, SOAD_CONST_PBCFG)
    SoAd_TxRoutingGroupRefPtr;
    SoAd_SoConIdType SoAd_TxPduRouteDestSocketConRefNum;
    CONSTP2CONST(SoAd_SoConIdType, TYPEDEF, SOAD_CONST_PBCFG)
    SoAd_TxSocketConRefIndexPtr;
    PduIdType SoAd_PduRouteIndex;
} SoAd_PduRouteDestType;

typedef struct
{
    PduIdType SoAd_TxPduId;
    SoAd_UpperLayerType SoAd_TxUpperLayerType;
    SoAd_uintx SoAd_PduRouteDestNum;
    CONSTP2CONST(SoAd_uintx, TYPEDEF, SOAD_CONST_PBCFG)
    SoAd_PduRouteDestIndex;
    SoAd_UpLayerModuleType SoAd_RefModule;
    PduIdType SoAd_UpPduId;
    boolean SoAd_MetaDataEnable;
#if (SOAD_UDP_SOCON_IMMEDIATE_TXCONFIRMATION == STD_ON)
    boolean SoAd_UDPImmediateTxConfirmation;
#endif /* SOAD_UDP_SOCON_IMMEDIATE_TXCONFIRMATION == STD_ON */
} SoAd_PduRouteType;

typedef struct
{
    SoAd_RoutingGroupIdType SoAd_RoutingGroupId;
    boolean SoAd_RoutingGroupIsEnabledAtInit;
    boolean SoAd_RoutingGroupTxTriggerable;
    SoAd_uintx SoAd_PduRouteDestNum;
    CONSTP2CONST(SoAd_uintx, TYPEDEF, SOAD_CONST_PBCFG)
    SoAd_PduRouteDestIndexPtr;
    SoAd_uintx SoAd_SocketRouteDestNum;
    CONSTP2CONST(SoAd_uintx, TYPEDEF, SOAD_CONST_PBCFG)
    SoAd_SocketRouteDestIndexPtr;
} SoAd_RoutingGroupType;

typedef struct
{
    SoAd_uintx SoAd_PduRouteNum;
    CONSTP2CONST(SoAd_PduRouteType, TYPEDEF, SOAD_CONST_PBCFG)
    SoAd_PduRoute;
    SoAd_RoutingGroupIdType SoAd_RoutingGroupNum;
    CONSTP2CONST(SoAd_RoutingGroupType, TYPEDEF, SOAD_CONST_PBCFG)
    SoAd_RoutingGroup;
    SoAd_SoConIdType SoAd_SocketConnectionNum;
    CONSTP2CONST(SoAd_SocketConnectionType, TYPEDEF, SOAD_CONST_PBCFG)
    SoAd_SocketConnection;
    SoAd_uintx SoAd_SocketConnectionGroupNum;
    CONSTP2CONST(SoAd_SocketConnectionGroupType, TYPEDEF, SOAD_CONST_PBCFG)
    SoAd_SocketConnectionGroup;
    SoAd_uintx SoAd_SocketRouteNum;
    CONSTP2CONST(SoAd_SocketRouteType, TYPEDEF, SOAD_CONST_PBCFG)
    SoAd_SocketRoute;
    SoAd_uintx SoAd_SocketRouteDestTotalNum;
    CONSTP2CONST(SoAd_SocketRouteDestType, TYPEDEF, SOAD_CONST_PBCFG)
    SoAd_SocketRouteDestTotalPtr;
    SoAd_uintx SoAd_PduRouteDestTotalNum;
    CONSTP2CONST(SoAd_PduRouteDestType, TYPEDEF, SOAD_CONST_PBCFG)
    SoAd_PduRouteDestTotalPtr;
    SoAd_uintx SoAd_SocketBufferCfgNum;
    CONSTP2CONST(SoAd_SoBufferCfgType, TYPEDEF, SOAD_CONST_PBCFG)
    SoAd_SocketBufferCfgPtr;
} SoAd_ConfigType;
/*==============================================================================
*                                             ENUMS
==============================================================================*/

#endif /* SOAD_TYPES_H_ */
