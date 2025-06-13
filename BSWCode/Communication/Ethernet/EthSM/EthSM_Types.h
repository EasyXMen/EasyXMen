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
**  FILENAME    : EthSM_Types.h                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : HuRongbo                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION : Type definitions of EthSM                                   **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*  <VERSION>    <DATE>      <AUTHOR>    <REVISION LOG>
 *  V2.0.0       2020-05-27  HuRongbo    R19_11 EthSM initial version.
 */

#ifndef ETHSM_TYPES_H
#define ETHSM_TYPES_H

#include "EthSM_Cfg.h"
#include "ComStack_Types.h"
#if (STD_ON == ETHSM_DEM_SUPPORT)
#include "Rte_Dem_Type.h"
#endif

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
/* This type shall define the states of the network mode state machine  */
/* @req SWS_EthSM_00041 */
typedef enum
{
    ETHSM_STATE_OFFLINE,
    ETHSM_STATE_WAIT_TRCVLINK,
    ETHSM_STATE_WAIT_ONLINE,
    ETHSM_STATE_ONLINE,
    ETHSM_STATE_ONHOLD,
    ETHSM_STATE_WAIT_OFFLINE
} EthSM_NetworkModeStateType;

/*
 * Reference to configured DEM event to report busoff errors for this ETH
 * network.
 */
#if (STD_ON == ETHSM_DEM_SUPPORT)
typedef struct
{
    /*Reference to configured DEM event*/
    Dem_EventIdType EthSMDemEventId;
} EthSM_DemEventParameterRefType;
#endif

/*
 * This container contains the Ethernet network-specific parameters of each
 * Ethernet network. It also contains the controller and transceiver IDs
 * assigned to a Ethernet network.
 */
typedef struct
{
    /* Channel ID assigned by EthSM */
    NetworkHandleType EthSMChannelId;

    /* Unique handle to identify one certain Eth network*/
    NetworkHandleType EthSMComMNetworkHandleRef;

    /* Reference to one of the Eth controllers managed by the EthIf module*/
    uint8 EthSMEthIfControllerRef;

#if (STD_ON == ETHSM_DEM_SUPPORT)
    /*references to DemEventParameter elements*/
    P2CONST(EthSM_DemEventParameterRefType, TYPEDEF, ETHSM_CONST)
    EthSMDemEventParameterRefs;
#endif
} EthSM_NetworkType;

/* Configuration of the Ethernet State Manager */
typedef struct
{
    P2CONST(EthSM_NetworkType, TYPEDEF, ETHSM_CONST) EthSMNetwork;
} EthSM_ConfigType;

#endif /* ETHSM_TYPES_H */
