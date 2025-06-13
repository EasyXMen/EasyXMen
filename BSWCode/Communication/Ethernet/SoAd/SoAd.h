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
**  FILENAME    : SoAd.h                                                     **
**                                                                           **
**  Created on  :                                                            **
**  Author      : zhengfei.li                                                **
**  Vendor      :                                                            **
**  DESCRIPTION : SoAd header description                                    **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2 and R19_11           **
**                                                                           **
**************************************************************************** */
/*============================================================================
==============================================================================*/
#ifndef SOAD_H_
#define SOAD_H_
/*==============================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/
#include "SoAd_Types.h"
#include "SchM_SoAd.h"
/*==============================================================================
*                              SOURCE FILE VERSION INFORMATION
==============================================================================*/
#define SOAD_VENDOR_ID          62u
#define SOAD_MODULE_ID          56u
#define SOAD_INSTANCE           0u
#define SOAD_H_AR_MAJOR_VERSION 4u
#define SOAD_H_AR_MINOR_VERSION 5u
#define SOAD_H_AR_PATCH_VERSION 0u

/*==============================================================================
*                                     FILE VERSION CHECKS
==============================================================================*/

/*==============================================================================
*                                          CONSTANTS
==============================================================================*/
#define SOAD_NOUSED(paramter) (void)(paramter)

/* Service IDs definition */
#define SOAD_SID_INIT                           0x01u
#define SOAD_SID_GETVERSIONINFO                 0x02u
#define SOAD_SID_IFTRANSMIT                     0x03u
#define SOAD_SID_IFROUTINGGROUPTRANSMIT         0x1Du
#define SOAD_SID_IFSPECIFICROUTINGGROUPTRANSMIT 0x1Fu
#define SOAD_SID_TPTRANSMIT                     0x04u
#define SOAD_SID_TPCANCELTRANSMIT               0x05u
#define SOAD_SID_TPCANCELRECEIVE                0x06u
#define SOAD_SID_GETSOCONID                     0x07u
#define SOAD_SID_OPENSOCON                      0x08u
#define SOAD_SID_CLOSESOCON                     0x09u
#define SOAD_SID_REQUESTIPADDRASSIGNMENT        0x0Au
#define SOAD_SID_RELEASEIPADDRASSIGNMENT        0x0Bu
#define SOAD_SID_GETLOCALADDR                   0x0Cu
#define SOAD_SID_GETPHYSADDR                    0x0Du
#define SOAD_SID_GETREMOTEADDR                  0x1Cu
#define SOAD_SID_ENABLEROUTING                  0x0Eu
#define SOAD_SID_ENABLESPECIFICROUTING          0x20u
#define SOAD_SID_DISABLEROUTING                 0x0Fu
#define SOAD_SID_DISABLESPECIFICROUTING         0x21u
#define SOAD_SID_SETREMOTEADDR                  0x10u
#define SOAD_SID_SETUNIQUEREMOTEADDR            0x1Eu
#define SOAD_SID_TPCHANGEPARAMETER              0x11u
#define SOAD_SID_READDHCPHOSTNAMEOPTION         0x1Au
#define SOAD_SID_WRITEDHCPHOSTNAMEOPTION        0x1Bu
#define SOAD_SID_RXINDICATION                   0x12u
#define SOAD_SID_COPYTXDATA                     0x13u
#define SOAD_SID_TXCONFIRMATION                 0x14u
#define SOAD_SID_TCPACCEPTED                    0x15u
#define SOAD_SID_TCPCONNECTED                   0x16u
#define SOAD_SID_TCPIPEVENT                     0x17u
#define SOAD_SID_LOCALIPADDRASSIGNMENTCHG       0x18u
#define SOAD_SID_MAINFUNCTION                   0x19u
#define SOAD_SID_GETSOCONMODE                   0x22u
#define SOAD_SID_RELEASEREMOTEADDR              0x23u

/* DET error codes */
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
#define SOAD_E_NOTINIT       0x01u
#define SOAD_E_PARAM_POINTER 0x02u
#define SOAD_E_INV_ARG       0x03u
#define SOAD_E_INV_PDUID     0x06u
#define SOAD_E_INV_SOCKETID  0x07u
#define SOAD_E_INIT_FAILED   0x08u
#endif /*STD_ON == SOAD_DEV_ERROR_DETECT*/
#define SOAD_E_NOBUFS           0x04u
#define SOAD_E_INV_PDUHEADER_ID 0x05u

#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)

extern CONST(SoAd_PcCfgType, SOAD_CONST) SoAd_PcCfg;

#endif /* SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u */

extern CONST(SoAd_ConfigType, SOAD_CONST) SoAd_Config;

/*==============================================================================
*                                    FUNCTION PROTOTYPES
==============================================================================*/

/*============================================================================*/
extern FUNC(void, SOAD_CODE) SoAd_Init(P2CONST(SoAd_ConfigType, AUTOMATIC, SOAD_APPL_CONST) SoAdConfigPtr);
/*============================================================================*/
#if (STD_ON == SOAD_VERSION_INFO_API)
extern FUNC(void, SOAD_CODE) SoAd_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, SOAD_APPL_DATA) versioninfo);
#endif /*STD_ON == SOAD_VERSION_INFO_API*/
/*============================================================================*/
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_IfRoutingGroupTransmit(VAR(SoAd_RoutingGroupIdType, AUTOMATIC) id);
/*============================================================================*/
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_IfSpecificRoutingGroupTransmit(
    VAR(SoAd_RoutingGroupIdType, AUTOMATIC) id,
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId);
/*============================================================================*/
extern FUNC(Std_ReturnType, SOAD_CODE)
    SoAd_GetSoConId(VAR(PduIdType, AUTOMATIC) TxPduId, P2VAR(SoAd_SoConIdType, AUTOMATIC, SOAD_APPL_DATA) SoConIdPtr);
/*============================================================================*/
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_OpenSoCon(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId);
/*============================================================================*/
extern FUNC(Std_ReturnType, SOAD_CODE)
    SoAd_CloseSoCon(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId, VAR(boolean, AUTOMATIC) Abort);
/*============================================================================*/
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_RequestIpAddrAssignment(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(TcpIp_IpAddrAssignmentType, AUTOMATIC) Type,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_CONST) LocalIpAddrPtr,
    VAR(uint8, AUTOMATIC) Netmask,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_CONST) DefaultRouterPtr);
/*============================================================================*/
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_ReleaseIpAddrAssignment(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId);
/*============================================================================*/
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_GetLocalAddr(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    P2VAR(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) LocalAddrPtr,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) NetmaskPtr,
    P2VAR(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) DefaultRouterPtr);
/*============================================================================*/
extern FUNC(Std_ReturnType, SOAD_CODE)
    SoAd_GetPhysAddr(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId, P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) PhysAddrPtr);
/*============================================================================*/
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_GetRemoteAddr(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    P2VAR(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) IpAddrPtr);
/*============================================================================*/
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_EnableRouting(VAR(SoAd_RoutingGroupIdType, AUTOMATIC) id);
/*============================================================================*/
extern FUNC(Std_ReturnType, SOAD_CODE)
    SoAd_EnableSpecificRouting(VAR(SoAd_RoutingGroupIdType, AUTOMATIC) id, VAR(SoAd_SoConIdType, AUTOMATIC) SoConId);
/*============================================================================*/
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_DisableRouting(VAR(SoAd_RoutingGroupIdType, AUTOMATIC) id);
/*============================================================================*/
extern FUNC(Std_ReturnType, SOAD_CODE)
    SoAd_DisableSpecificRouting(VAR(SoAd_RoutingGroupIdType, AUTOMATIC) id, VAR(SoAd_SoConIdType, AUTOMATIC) SoConId);
/*============================================================================*/
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_SetRemoteAddr(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_CONST) RemoteAddrPtr);
/*============================================================================*/
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_SetUniqueRemoteAddr(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_CONST) RemoteAddrPtr,
    P2VAR(SoAd_SoConIdType, AUTOMATIC, SOAD_APPL_DATA) AssignedSoConIdPtr);
/*============================================================================*/
extern FUNC(void, SOAD_CODE) SoAd_ReleaseRemoteAddr(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId);
/*============================================================================*/
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_TpChangeParameter(
    VAR(PduIdType, AUTOMATIC) id,
    VAR(TPParameterType, AUTOMATIC) parameter,
    VAR(uint16, AUTOMATIC) value);
/*============================================================================*/
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_ReadDhcpHostNameOption(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) length,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) data);
/*============================================================================*/
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_WriteDhcpHostNameOption(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(uint8, AUTOMATIC) length,
    P2CONST(uint8, AUTOMATIC, SOAD_APPL_CONST) data);
/*============================================================================*/
extern FUNC(void, SOAD_CODE) SoAd_GetSoConMode(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    P2VAR(SoAd_SoConModeType, AUTOMATIC, SOAD_APPL_DATA) ModePtr);
/*============================================================================*/

#endif /* SOAD_H_ */
