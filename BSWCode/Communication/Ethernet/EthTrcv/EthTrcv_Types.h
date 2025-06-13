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
********************************************************************************
**                                                                            **
**  FILENAME    : EthTrcv_Types.h                                             **
**                                                                            **
**  Created on  :                                                             **
**  Author      : wanglili                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*  <VERSION>    <DATE>      <AUTHOR>        <REVISION LOG>
 *  V2.0.0       2020-08-18  wanglili        R19_11 initial version.
 */
#ifndef ETHTRCV_TYPES_H_
#define ETHTRCV_TYPES_H_
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Eth_GeneralTypes.h"
#include "EthTrcv_Cfg.h"
#if (ETHTRCV_USED_ICU_MODULE == STD_ON)
#include "Icu.h"
#endif
#if (ETHTRCV_USED_DEM_MODULE == STD_ON)
#include "Dem.h"
#endif
#if (ETHTRCV_WAKEUP_SUPPORT != ETHTRCV_WAKEUP_NOT_SUPPORTED)
#include "EcuM_Types.h"
#endif
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/*the invalid channel id base on the "Icu_ChannelType" type*/
#define ICU_CHANNEL_ID_INVALID (0xFFU)
/*base on the Dem_EventIdType is uint16*/
#define ETHTRCV_NO_DEM_EVENT (0xFFFFU)
/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
/*****************************************************************************
 *  INTERNAL DATA TYPES AND STRUCTURES
 *****************************************************************************/
/* Specifies the connection negotiation of the Ethernet transceiver link */
typedef enum
{
    TRCV_CONN_NEG_AUTO = 0u,
    TRCV_CONN_NEG_MASTER,
    TRCV_CONN_NEG_SLAVE
} EthTrcv_LinkModeType;

/* Specifies the connection negotiation of the Ethernet transceiver link */
typedef enum
{
    TRCV_PHYS_LAYER_TYPE_NONE = 0u,
    TRCV_PHYS_LAYER_TYPE_1000BASE_T,
    TRCV_PHYS_LAYER_TYPE_1000BASE_T1,
    TRCV_PHYS_LAYER_TYPE_100BASE_T1,
    TRCV_PHYS_LAYER_TYPE_100BASE_TX
} EthTrcv_PhysLayerTypeType;

/*Defines the baud rate of the MAC layer*/
typedef enum
{
    ETH_MAC_LAYER_SPEED_NONE = 0u, /*not config*/
    ETH_MAC_LAYER_SPEED_100M,
    ETH_MAC_LAYER_SPEED_10G,
    ETH_MAC_LAYER_SPEED_10M,
    ETH_MAC_LAYER_SPEED_1G,
    ETH_MAC_LAYER_SPEED_2500M
} EthTrcvMacLayerSpeedType;

/*Defines the MAC layer subtype of a switch port*/
typedef enum
{
    NONE = 0u, /*not config*/
    LIGHT,
    REDUCED,
    REVERSED,
    SERIAL,
    STANDARD,
    UNIVERSAL_SERIAL
} EthTrcvMacLayerSubTypeType;

typedef enum
{
    TRCV_MAC_LAYER_TYPE_NONE = 0u, /*not config*/
    TRCV_MAC_LAYER_TYPE_XGMII,
    TRCV_MAC_LAYER_TYPE_XMII,
    TRCV_MAC_LAYER_TYPE_XXGMII
} EthTrcvMacLayerTypeType;

typedef enum
{
    TRCV_SPEED_10 = 0u, /*not config*/
    TRCV_SPEED_100,
    TRCV_SPEED_1000
} EthTrcvSpeedType;

/* Implementation specific structure of the EthTrcv Wakeup Map */
#if (ETHTRCV_WAKEUP_SUPPORT != ETHTRCV_WAKEUP_NOT_SUPPORTED)
typedef struct
{
    EthTrcv_WakeupReasonType EthTrcvWakeupReason;
    EcuM_WakeupSourceType WakeupSource;
} EthTrcv_WakeupMapType;
#endif

/*used to record cable diagnostics state*/
typedef struct
{
    EthTrcv_CableDiagResultType CableDiagState;
    boolean PendingSetTrcvModeReq;
    Eth_ModeType PendingSetTrcvModeReqMode;
    boolean PendingTrcvLinkStateReq;
    EthTrcv_LinkStateType PendingTrcvLinkStateReqMode;
} EthTrcv_CableDiagStateType;

typedef struct
{
    uint8 EthTrcvCtrlIdx; /*Used by EthIf module,may be a macro*/
    uint8 EthTrcvMiiIdx;  /*Used by MII communication(call Eth API)*/
} EthTrcvMiiInterfaceType;

typedef struct
{
    uint8 EthTrcvSwitchPortRef;
    uint8 EthTrcvSwitchRef;
} EthTrcvSwitchInterfaceType;

typedef struct
{
    P2CONST(EthTrcvMiiInterfaceType, TYPEDEF, ETHTRCV_CONFIG_DATA) EthTrcvMiiInterface;
    P2CONST(EthTrcvSwitchInterfaceType, TYPEDEF, ETHTRCV_CONFIG_DATA) EthTrcvSwitchInterface;
} EthTrcvMgmtInterfaceType;

/* Implementation specific structure of the EthTrcv configuration */
typedef struct
{
    uint8 EthTrcvIdx; /*Used by EthIf module,may be a macro*/
    EthTrcv_LinkModeType EthTrcvConnNeg;
    uint8 EthTrcvRefCtrlIdxInEthIf; /*Used by call EthIf API*/
    EthTrcvSpeedType EthTrcvSpeed;
    P2FUNC(void, ETHTRCV_CONFIG_DATA, EthTrcvWakeUpCallout)(uint8 TrcvIdx);
#if (ETHTRCV_USED_ICU_MODULE == STD_ON)
    Icu_ChannelType EthTrcvRefChannelIdx;
#endif
#if (ETHTRCV_USED_DEM_MODULE == STD_ON)
    Dem_EventIdType EthTrcvAccessErrorEventId;
#endif
#if (ETHTRCV_WAKEUP_SUPPORT != ETHTRCV_WAKEUP_NOT_SUPPORTED)
    uint8 EthTrcvWakeupReasonNum;
    CONSTP2CONST(EthTrcv_WakeupMapType, TYPEDEF, ETHTRCV_CONFIG_DATA)
    EthTrcvWakeupMapCfgPtr;
#endif
    EthTrcv_DuplexModeType EthTrcvDuplexMode;
    EthTrcv_PhysLayerTypeType EthTrcvPhysLayerType;
    EthTrcvMacLayerSpeedType EthTrcvMacLayerSpeed;
    EthTrcvMacLayerSubTypeType EthTrcvMacLayerSubType;
    EthTrcvMacLayerTypeType EthTrcvMacLayerType;
    P2CONST(EthTrcvMgmtInterfaceType, TYPEDEF, ETHTRCV_CONFIG_DATA) EthTrcvMgmtInterface;
    P2CONST(uint8, TYPEDEF, ETHTRCV_CONFIG_DATA) EthTrcvConfigEcucPartitionRef;
} EthTrcv_ConfigType;

typedef struct
{
    P2CONST(uint8, TYPEDEF, ETHTRCV_CONFIG_DATA) EthTrcvEcucPartitionRef;
} EthTrcv_GlobalType;
/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/
/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
#endif /* ETHTRCV_TYPES_H_ */
