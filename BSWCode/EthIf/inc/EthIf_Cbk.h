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
 **  @file               : EthIf.c
 **  @author             : bo.yang
 **  @date               : 2020/07/20
 **  @vendor             : isoft
 **  @description        : Ethernet Interface
 **
 ***********************************************************************************************************************/

#ifndef ETHIF_CBK_H
#define ETHIF_CBK_H
/* PRQA S 1501,1753 EOF */ /* VL_EthIf_1501,VL_EthIf_1753 */
/* =================================================== inclusions =================================================== */
#include "ComStack_Types.h"
#include "EthIf_Types.h"
#include "EthIf_Cfg.h"

#ifdef __cplusplus
extern "C" {
#endif
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

/**
 * @brief           Receive indication of an Ethernet frame which was received by the indexed controller.
 * @param[in]       TrcvIdx :Index of the physical Ethernet controller within the context of the Ethernet Interface.
 * @param[in]       FrameType :Frame type of received Ethernet frame.
 * @param[in]       IsBroadcast :parameter to indicate a broadcast frame.
 * @param[in]       PhysAddrPtr :Pointer to Physical source address (MAC address in network byte order) of received
 * Ethernet frame.
 * @param[in]       DataPtr :Pointer to payload of received Ethernet frame.
 * @param[in]       DataLen :Length (bytes) of the payload in received frame.
 * @param[in]       IngressTimeTuplePtr :Pointer to ingress timestamp provided as time tuple.
 * @param[in]       RxHandleId :Unique receive handle id provided by the Ethernet Driver, to identify the ingress queue
 * element per hysical Ethernet controller.
 * @return          None
 * @reentrant       FALSE
 * @synchronous     TRUE
 * @trace           CPD-63699
 */
extern void EthIf_RxIndication(
    uint8               CtrlIdx,
    Eth_FrameType       FrameType,
    boolean             IsBroadcast,
    const uint8*        PhysAddrPtr,
    const Eth_DataType* DataPtr,
    uint16              DataLen
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_R23_11)
    ,
    TimeTupleType* IngressTimeTuplePtr,
    Eth_BufIdxType RxHandleId
#endif
);

/**
 * @brief           Confirms frame transmission by the indexed controller.
 * @param[in]       CtrlIdx :Index of the physical Ethernet controller within the context of the Ethernet Interface.
 * @param[in]       BufIdx :Index of the transmitted buffer.
 * @param[in]       Result :E_OK: The transmission was successful, E_NOT_OK: The transmission failed.
 * @return          None
 * @reentrant       FALSE
 * @synchronous     TRUE
 * @trace           CPD-63700
 */

extern void EthIf_TxConfirmation(
    uint8          CtrlIdx,
    Eth_BufIdxType BufIdx
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431)
    ,
    Std_ReturnType Result
#endif
);

/**
 * @brief           Called asynchronously when mode has been read out.
 * @param[in]       CtrlIdx :Index of the physical Ethernet controller within the context of the Ethernet Interface.
 * @param[in]       CtrlMode :Notified Ethernet controller mode.
 * @return          None
 * @reentrant       Non Reentrant for the same CtrlIdx, reentrant for different
 * @synchronous     TRUE
 * @trace           CPD-63701
 */
extern void EthIf_CtrlModeIndication(uint8 CtrlIdx, Eth_ModeType CtrlMode);

/**
 * @brief           Called asynchronously when a mode change has been read out.
 * @param[in]       TrcvIdx :Index of the Ethernet transceiver within the context of the Ethernet Interface.
 * @param[in]       TrcvMode :Notified Ethernet transceiver mode.
 * @return          None
 * @reentrant       Non Reentrant for the same CtrlIdx, reentrant for different
 * @synchronous     TRUE
 * @trace           CPD-63702
 */
extern void EthIf_TrcvModeIndication(uint8 TrcvIdx, Eth_ModeType TrcvMode);

/**
 * @brief           The EthIf shall determine the expected notifications based on the EthSwtPort configuration. In
 *                  case the EthSwtPort references an EthTrcv the EthIf expects a notification from the EthTrcv via
 *                  API EthIf_TrcvModeIndication(). Otherwise the EthIf expects a notification from the EthSwt via
 *                  API EthIf_SwitchPortModeIndication().
 * @param[in]       SwtichIdxPtr : Pointer to the switch index.
 * @param[in]       PortIdxPtr : Pointer to the port index.
 * @param[in]       PortMode :Notified Ethernet Switch port mode..
 * @return          None
 * @reentrant       FALSE
 * @synchronous     FALSE
 * @trace           CPD-63703
 */
void EthIf_SwitchPortModeIndication(uint8 SwitchIdx, uint8 SwitchPortIdx, Eth_ModeType PortMode);

/**
 * @brief           This API is called by the corresponding EthTrcv, if a sleep indication was detected on the network.
 * @param[in]       TrcvIdx : Index of the Ethernet transceiver within the context of the Ethernet Interface.
 * @return          None
 * @reentrant       TRUE
 * @synchronous     TRUE
 * @trace           CPD-63704
 */
void EthIf_SleepIndication(uint8 TrcvIdx);

#ifdef __cplusplus
}
#endif
#endif
