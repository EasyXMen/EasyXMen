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
 ***********************************************************************************************************************
 **
 **  @file               : EthSM_Internal.h
 **  @author             : xiaojian.liang
 **  @date               : 2024/12/23
 **  @vendor             : isoft
 **  @description        : Internal implementation header of EthSM
 **
 **********************************************************************************************************************/

#ifndef ETHSM_INTERNAL_H_
#define ETHSM_INTERNAL_H_

/* =================================================== inclusions =================================================== */
#include "EthSM_Cfg.h"
#include "EthSM.h" /* Import EthSM_NetworkModeStateType */
                   /* Import Eth_ModeType, EthTrcv_LinkStateType */
                   /* Import NetworkHandleType */
                   /* Import ComM_ModeType */

#if ETHSM_MULTIPLE_PARTITION_ENABLED == STD_ON
#include "Os.h"
#endif

#if ETHSM_TCPIP_ENABLED == STD_ON
#include "TcpIp.h" /* Import TcpIp_StateType */
#endif

#if ETHSM_DEM_ENABLED == STD_ON
#include "Rte_Dem_Type.h" /* Import Dem_EventIdType */
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */
/**
 * @brief EthSM network private state type
 * @trace CPD-69962
 */
typedef enum
{
    ETHSM_PRIVATE_STATE_UNINIT,          /**< EthSM is not initialized in this state. */
    ETHSM_PRIVATE_STATE_NONE,            /**< EthSM shall perform the NetworkModeState state machine transition. */
    ETHSM_PRIVATE_STATE_WAIT_CTRLACTIVE, /**< EthSM waits for controller mode ACTIVE */
    ETHSM_PRIVATE_STATE_WAIT_CTRLDOWN,   /**< EthSM waits for controller mode DOWN */
} EthSM_NetworkPrivateStateType;

/* clang-format off */
/**
 * @brief EthSM network configuration type
 * @trace CPD-73910
 */
typedef struct
{
    NetworkHandleType ComMChannelId;           /**< Reference to one of the network handles configured for the ComM. */
    uint8 EthIfCtrlIdx;                        /**< Reference to EthIfCtrl container where a ETH controller and transceiver (optional) combination is configured. */
    boolean ForwardSilentCommunicationEnabled; /**< Specifies if a request COMM_SILENT_COMMUNICATION shall be forwarded to the corresponding EthIfController. */
#if ETHSM_TCPIP_ENABLED == STD_ON
    boolean TcpIpUsed;                         /**< Specifies if the interaction with a TcpIp module is enabled or disabled for the EthSMNetwork. */
#endif
    boolean WakeupSleepOnDatalineEnabled;      /**< Specifies if an active communication request shall be forwarded to the lower layer to trigger a wake-up on the Ethernet network. */
#if ETHSM_DEM_ENABLED == STD_ON
    Dem_EventIdType DemEventId;                /**< Reference to configured DEM event to report bus off errors for this Eth network, 0 if not used. */
#endif

#if ETHSM_MULTIPLE_PARTITION_ENABLED == STD_ON
    ApplicationType Application;               /**< Denotes in which "OsApplication" the network is executed. */
#endif
} EthSM_NetworkConfigType;

/**
 * @brief EthSM network variable type
 * @trace CPD-69964
 */
typedef struct
{
    ComM_ModeType         ComMMode;                    /**< Current communication mode requested by ComM */
    boolean               WakeupRequest;               /**< There is a pending COMM_FULL_COMMUNICATION_WITH_WAKEUP_REQUEST or not. */
    Eth_ModeType          EthMode;                     /**< Current Ethernet Interface Controller mode. */
    EthTrcv_LinkStateType EthTrcvLinkState;            /**< Current transceiver link state of the specific network handle */
#if ETHSM_TCPIP_ENABLED == STD_ON
    TcpIp_StateType TcpIpState;                        /**< Current TcpIp state of the specific network handle */
#endif
    EthSM_NetworkModeStateType    NetworkModeState;    /**< Current network mode state */
    EthSM_NetworkPrivateStateType NetworkPrivateState; /**< Current network private state */
} EthSM_NetworkVarType;
/* clang-format on */

/**
 * @brief EthSM network type
 * @trace CPD-69966
 */
typedef struct
{
    const EthSM_NetworkConfigType* ConfigPtr; /**< Pointer to the network configuration */
    EthSM_NetworkVarType*          VarPtr;    /**< Pointer to the network variable */
} EthSM_NetworkType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */
/**
 * @brief Array of EthSM network instances
 * @trace CPD-73911
 */
extern const EthSM_NetworkType EthSM_Networks[ETHSM_NETWORK_NUM];

/* ========================================= external function declarations ========================================= */
/**
 * @brief       Cyclic Main Function which is called from the Scheduler.
 * @param[in]   networkPtr  Pointer to the network instance
 * @reentrant   TRUE (only for different Ethernet controllers)
 * @synchronous TRUE
 * @trace       CPD-73929
 */
void EthSM_MainFunction(const EthSM_NetworkType* networkPtr);

#endif /* ETHSM_INTERNAL_H_ */
