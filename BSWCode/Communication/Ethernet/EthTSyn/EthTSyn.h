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
/*********************************************************************************
**                                                                            **
**  FILENAME    : EthTSyn.h                                                   **
**                                                                            **
**  Created on  :                                                             **
**  Author      : yuzhe.zhang                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation header for EthTSyn                           **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/******************************************************************************
**                      Revision Control History                             **
******************************************************************************/
#ifndef ETHTSYN_H
#define ETHTSYN_H
/*******************************************************************************
**                      Include                                               **
*******************************************************************************/
#include "EthTSyn_Types.h"
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define ETHTSYN_VENDOR_ID          62U
#define ETHTSYN_MODULE_ID          169U
#define ETHTSYN_INSTANCE_ID        0U
#define ETHTSYN_H_AR_MAJOR_VERSION 4U
#define ETHTSYN_H_AR_MINOR_VERSION 5U
#define ETHTSYN_H_AR_PATCH_VERSION 0U
#define ETHTSYN_H_SW_MAJOR_VERSION 2U
#define ETHTSYN_H_SW_MINOR_VERSION 0U
#define ETHTSYN_H_SW_PATCH_VERSION 18U

#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
/*Development errors used by the EthTSyn module*/
#define ETHTSYN_E_UNINIT              ((uint8)0x20)
#define ETHTSYN_E_INIT_FAILED         ((uint8)0x21)
#define ETHTSYN_E_CTRL_IDX            ((uint8)0x22)
#define ETHTSYN_E_PARAM_POINTER       ((uint8)0x23)
#define ETHTSYN_E_PARAM               ((uint8)0x24)
#define ETHTSYN_E_ALREADY_INITIALIZED ((uint8)0x25)
#endif

/*service id*/
#define ETHTSYN_SID_INIT                ((uint8)0x01)
#define ETHTSYN_SID_GETVERSIONINFO      ((uint8)0x02)
#define ETHTSYN_SID_SETTRANSMISSIONMODE ((uint8)0x05)
#define ETHTSYN_SID_RXINDICATION        ((uint8)0x06)
#define ETHTSYN_SID_TXCONFIRMATION      ((uint8)0x07)
#define ETHTSYN_SID_TRCVLINKSTATECHG    ((uint8)0x08)
#define ETHTSYN_SID_MAINFUNCTION        ((uint8)0x09)

#if (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON)
/* Time Master conflict */
#define ETHTSYN_E_TMCONFLICT ((Std_ReturnType)0x01)
/* Time Slave conflict */
#define ETHTSYN_E_TSCONFLICT ((Std_ReturnType)0x02)
#endif
/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
/* Handles the enabling and disabling of the transmission mode */
extern const uint8 EthTSynDestPhyAddr[6];

extern const EthTSyn_ConfigType EthTSyn_Config;

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

/**
 * This function initializes the Time Synchronization over Ethernet.
 * Service ID: 0x01
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): @ConfigPtr, Pointer to the selected configuration set.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
void EthTSyn_Init(const EthTSyn_ConfigType* configPtr);
#if (ETHTSYN_VERSION_INFO_API == STD_ON)
/**
 * Returns the version information of this module.
 * Service ID: 0x02
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): @versioninfo, Pointer to the memory location holding the version information of
the module.
 * Return value: NA
 */
void EthTSyn_GetVersionInfo(Std_VersionInfoType* versioninfo);
#endif

/**
 * This API is used to turn on and off the TX capabilities of the EthTSyn.
 * Service ID: 0x05
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): @CtrlIdx Index of the Ethernet controller
                   @Mode ETHTSYN_TX_OFF ETHTSYN_TX_ON
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:    NA
 */
void EthTSyn_SetTransmissionMode(uint8 CtrlIdx, EthTSyn_TransmissionModeType Mode);

/**
 * By this API service the EthTSyn gets an indication and the data of a received frame.
 * Service ID: 0x06
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): @CtrlIdx, Index of the Ethernet controller
                   @FrameType, frame type of received Ethernet frame
                   @IsBroadcast, parameter to indicate a broadcast frame
                   @PhysAddrPtr, pointer to Physical source address (MAC address in network byte
                    order) of received Ethernet frame
                   @DataPtr Pointer to payload of the received Ethernet frame (i.e. Ethernet
                    header is not provided).
                   @LenByte Length of received data
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:    NA
 */
void EthTSyn_RxIndication(
    uint8 CtrlIdx,
    Eth_FrameType FrameType,
    boolean IsBroadcast,
    const uint8* PhysAddrPtr,
    const uint8* DataPtr,
    uint16 LenByte);

/**
 * @brief         Confirms the transmission of an Ethernet frame
 * @id            0x07
 * @synchronous   Synchronous
 * @reentrancy    Non Reentrant Dont care
 * @param[in]     CtrlIdx  Index of the Ethernet controller within the context of the Ethernet Interface
 * @param[in]     BufIdx   Index of the buffer resource
 * @retval        E_OK     The transmission was successful
 * @retval        E_NOT_OK The transmission failed
 */
void EthTSyn_TxConfirmation(uint8 CtrlIdx, Eth_BufIdxType BufIdx, Std_ReturnType Result);

/**
 * Allows resetting state machine in case of unexpected Link loss to avoid
   inconsistent Sync and Follow_Up sequences
 * Service ID: 0x08
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN):  @CtrlIdx Index of the Ethernet controller
                    @TrcvLinkState ETHTRCV_LINK_STATE_DOWN ETHTRCV_LINK_STATE_ACTIVE
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType EthTSyn_TrcvLinkStateChg(uint8 CtrlIdx, EthTrcv_LinkStateType TrcvLinkState);
#endif
