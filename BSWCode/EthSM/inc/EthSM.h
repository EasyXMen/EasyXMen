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
 **  @file               : EthSM.h
 **  @author             : xiaojian.liang
 **  @date               : 2024/12/23
 **  @vendor             : isoft
 **  @description        : Implementation header of EthSM
 **
 **********************************************************************************************************************/

/** ===================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date       Author         Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V2.1.0    2024-12-23 xiaojian.liang
 *      1. Support multi-partition distribution.
 *
 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C: 2012 Compliance Exceptions
    ModeName:ComM<br>
  RuleSorce:puhua_rules-2024.12.rcf

    \todo
    \li VL_EthSM_FalsePositive
      Reason: QAC false positives
      Risk: No risk
      Prevention: No action required.

    \li VL_MTR_EthSM_STM19
      Reason: Multiple exit points can contribute to improved code performance in specific scenarios.
      Risk: Increases the difficulty of debugging the function.
      Prevention: Adequate testing ensures correct functionality.
 */

#ifndef ETHSM_H_
#define ETHSM_H_

/* =================================================== inclusions =================================================== */
#include "EthSM_Cfg.h"

#include "Std_Types.h"        /* Import Std_VersionInfoType, Std_ReturnType */
#include "ComStack_Types.h"   /* Import NetworkHandleType */
#include "ComM.h"             /* Import ComM_ModeType */
#include "Eth_GeneralTypes.h" /* Import Eth_ModeType, EthTrcv_LinkStateType */

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */
/** @name Published Information
 *  @trace CPD-73900
 *  @{ */
#define ETHSM_VENDOR_ID                   (62u)
#define ETHSM_MODULE_ID                   (143u)
#define ETHSM_AR_RELEASE_MAJOR_VERSION    (4u)
#define ETHSM_AR_RELEASE_MINOR_VERSION    (9u)
#define ETHSM_AR_RELEASE_REVISION_VERSION (0u)
#define ETHSM_SW_MAJOR_VERSION            (2u)
#define ETHSM_SW_MINOR_VERSION            (1u)
#define ETHSM_SW_PATCH_VERSION            (0u)
/** @} */

/* ===================================================== macros ===================================================== */
/** @name Development Errors
 *  @trace CPD-73901
 *  @{ */
#define ETHSM_E_INVALID_NETWORK_MODE        0x01U
#define ETHSM_E_UNINIT                      0x02U
#define ETHSM_E_PARAM_POINTER               0x03U
#define ETHSM_E_INVALID_NETWORK_HANDLE      0x04U
#define ETHSM_E_INVALID_TCP_IP_MODE         0x05U
#define ETHSM_E_PARAM_CONTROLLER            0x07U
#define ETHSM_E_INVALID_ETH_MODE            0x08U
#define ETHSM_E_INVALID_ETH_TRCV_LINK_STATE 0x09U
#define ETHSM_E_INVALID_PARTITION_CONTEXT   0x0AU
/** @} */

/** @name Service ID
 *  @trace CPD-73902
 *  @{ */
#define ETHSM_SID_INIT                0x07U
#define ETHSM_SID_GETVERSIONINFO      0x02U
#define ETHSM_SID_REQUESTCOMMODE      0x05U
#define ETHSM_SID_GETCURRENTCOMMODE   0x04U
#define ETHSM_SID_CTRLMODEINDICATION  0x09U
#define ETHSM_SID_TRCVLINKSTATECHG    0x06U
#define ETHSM_SID_TCPIPMODEINDICATION 0x08U
#define ETHSM_SID_MAINFUNCTION        0x01U
/** @} */

/* ================================================ type definitions ================================================ */
/* clang-format off */
/** This type shall define the states of the network mode state machine.
 *  @trace CPD-69967
 */
typedef enum
{
    ETHSM_STATE_OFFLINE,       /**< EthSM is initialized in this state. (TcpIp state is OFFLINE) */
    ETHSM_STATE_WAIT_TRCVLINK, /**< ComM requests COMM_FULL_COMMUNICATION in this state. Controller will be set to ACTIVE. EthSM waits for transceiver link state (ACTIVE). */
    ETHSM_STATE_WAIT_ONLINE,   /**< Transceiver link state is ACTIVE EthSM waits for IP communication (TcpIP state is ONLINE) */
    ETHSM_STATE_ONLINE,        /**< IP communication is available ComM state COMM_FULL_COMMUNICATION is reached */
    ETHSM_STATE_ONHOLD,        /**< EthSM lost active transceiver link state, TcpIP state is still ONLINE) */
    ETHSM_STATE_WAIT_OFFLINE,  /**< ComM requests COMM_SILENT_COMMUNICATION (TcpIp state is still ONLINE) and if configured (see ECUC_EthSM_00110), to disable transmission of Ethernet frames. Afterwards ComM requests COMM_NO_COMMUNICATION in this state. (TcpIp state is requested with OFFLINE) */
} EthSM_NetworkModeStateType;
/* clang-format on */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief       This function initialize the EthSM.
 * @reentrant   TRUE (Reentrant for different partitions. Non reentrant for the same partition.)
 * @synchronous TRUE
 * @trace       CPD-69974
 */
void EthSM_Init(void);

/**
 * @brief       This service puts out the version information of this module.
 * @param[out]  versioninfo Pointer where to put out the version information.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-69975
 */
#if ETHSM_VERSION_INFO_API == STD_ON
void EthSM_GetVersionInfo(Std_VersionInfoType* versioninfo);
#endif

/**
 * @brief       Handles the communication mode and sets the Ethernet network active or passive.
 * @param[in]   NetworkHandle   Handle of destinated communication network for request
 * @param[in]   ComM_Mode       Requested communication mode
 * @retval      E_OK            Controller mode request has been accepted
 * @retval      E_NOT_OK        Controller mode request has not been accepted
 * @reentrant   TRUE (Reentrant for different networks. Non reentrant for the same network.)
 * @synchronous FALSE
 * @trace       CPD-69976
 */
Std_ReturnType EthSM_RequestComMode(NetworkHandleType NetworkHandle, ComM_ModeType ComM_Mode);

/**
 * @brief       This service shall put out the current communication mode of a Ethernet network.
 * @param[in]   NetworkHandle   Network handle whose current communication mode shall be put out
 * @param[out]  ComM_ModePtr    Pointer where to put out the current communication mode
 * @retval      E_OK            Service accepted
 * @retval      E_NOT_OK        Service denied
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-69977
 */
Std_ReturnType EthSM_GetCurrentComMode(NetworkHandleType NetworkHandle, ComM_ModeType* ComM_ModePtr);

/**
 * @brief       Called when mode has been read out. Either triggered by previous EthIf_GetControllerMode or by
 *              EthIf_SetControllerMode call. Can directly be called within the trigger functions.
 * @param[in]   CtrlIdx   Ethernet Interface Controller whose mode has changed
 * @param[in]   CtrlMode  Notified Ethernet Interface Controller mode
 * @reentrant   TRUE (only for different Ethernet controllers)
 * @synchronous TRUE
 * @trace       CPD-69978
 */
void EthSM_CtrlModeIndication(uint8 CtrlIdx, Eth_ModeType CtrlMode);

/**
 * @brief       This service is called by the Ethernet Interface to report a transceiver link state change.
 * @param[in]   CtrlIdx               Index of the Ethernet controller within the context of the Ethernet Interface
 * @param[in]   TransceiverLinkState  Actual transceiver link state of the specific network handle
 * @reentrant   TRUE (only for different Ethernet controllers)
 * @synchronous TRUE
 * @trace       CPD-69979
 */
void EthSM_TrcvLinkStateChg(uint8 CtrlIdx, EthTrcv_LinkStateType TransceiverLinkState);

#ifdef __cplusplus
}
#endif

#endif /* ETHSM_H_ */
