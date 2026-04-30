/* PRQA S 3108++ */
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
/* PRQA S 3108-- */
/*
************************************************************************************************************************
**                                                                                                                    **
**  FILENAME    : EthSwt.h                                                                                            **
**                                                                                                                    **
**  Created on  : 2021-10-13                                                                                          **
**  Author      : zhiqiang.huang                                                                                      **
**  Vendor      :                                                                                                     **
**  DESCRIPTION : none                                                                                                **
**                                                                                                                    **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                                                              **
**                                                                                                                    **
***********************************************************************************************************************/
#ifndef ETHSWT_H_
#define ETHSWT_H_

/***********************************************************************************************************************
 *  INCLUDES
 ***********************************************************************************************************************/
#include "EthSwt_Cfg.h"
#include "EthSwt_Types.h"
#include "EthSwt_PBCfg.h"
/***********************************************************************************************************************
 *                              SOURCE FILE VERSION INFORMATION
 ***********************************************************************************************************************/
#define ETHSWT_VENDOR_ID          62u
#define ETHSWT_MODULE_ID          89u
#define ETHSWT_INSTANCE           0u /*This is the swtIdx of the switch*/
#define ETHSWT_H_AR_MAJOR_VERSION 4u
#define ETHSWT_H_AR_MINOR_VERSION 5u
#define ETHSWT_H_AR_PATCH_VERSION 0u

/* Service IDs definition */
#define ETHSWT_SID_INIT                           0x01
#define ETHSWT_SID_SETSWITCHPORTMODE              0x03
#define ETHSWT_SID_GETSWITCHPORTMODE              0x04
#define ETHSWT_SID_STARTSWITCHPORTAUTONEGOTIATION 0x05
#define ETHSWT_SID_CHECKWAKEUP                    0x4C
#define ETHSWT_SID_GETSWITCHPORTWAKEUPREASON      0x4B
#define ETHSWT_SID_GETLINKSTATE                   0x06
#define ETHSWT_SID_GETBAUDRATE                    0x07
#define ETHSWT_SID_GETDUPLEXMODE                  0x08
#define ETHSWT_SID_GETPORTMACADDR                 0x09
#define ETHSWT_SID_GETARLTABLE                    0x0A
#define ETHSWT_SID_GETCOUNTERVALUES               0x0C
#define ETHSWT_SID_GETRXSTATS                     0x0D
#define ETHSWT_SID_GETTXSTATS                     0x20
#define ETHSWT_SID_GETTXERRORCOUNTERVALUES        0x21
#define ETHSWT_SID_GETSWITCHREG                   0x0E
#define ETHSWT_SID_SETSWITCHREG                   0x0F
#define ETHSWT_SID_READTRCVREGISTER               0x10
#define ETHSWT_SID_WRITETRCVREGISTER              0x11
#define ETHSWT_SID_ENABLEVLAN                     0x12
#define ETHSWT_SID_STORECONFIGURATION             0x13
#define ETHSWT_SID_RESETCONFIGURATION             0x14
#define ETHSWT_SID_SETMACLEARNINGMODE             0x15
#define ETHSWT_SID_GETMACLEARNINGMODE             0x16
#define ETHSWT_SID_NVMSINGLEBLOCKCALLBACK         0x17
#define ETHSWT_SID_GETVERSIONINFO                 0x18
#define ETHSWT_SID_ETHRXPROCESSFRAME              0x23
#define ETHSWT_SID_ETHRXFINISHEDINDICATION        0x24
#define ETHSWT_SID_ETHTXPREPAREFRAME              0x25
#define ETHSWT_SID_ETHTXADAPTBUFFERLENGTH         0x26
#define ETHSWT_SID_SETMGMTINFO                    0x27
#define ETHSWT_SID_ETHTXPROCESSFRAME              0x28
#define ETHSWT_SID_ETHTXFINISHEDINDICATION        0x29
#define ETHSWT_SID_PORTENABLETIMESTAMP            0x40
#define ETHSWT_SID_VERIFYCONFIG                   0x31
#define ETHSWT_SID_SETFORWARDINGMODE              0x32
#define ETHSWT_SID_GETPORTSIGNALQUALITY           0x33
#define ETHSWT_SID_GETPORTIDENTIFIER              0x34
#define ETHSWT_SID_GETSWITCHIDENTIFIER            0x35
#define ETHSWT_SID_WRITEPORTMIRRORCONFIGURATION   0x36
#define ETHSWT_SID_READPORTMIRRORCONFIGURATION    0x37
#define ETHSWT_SID_DELETEPORTMIRRORCONFIGURATION  0x4A
#define ETHSWT_SID_GETPORTMIRRORSTATE             0x38
#define ETHSWT_SID_SETPORTMIRRORSTATE             0x39
#define ETHSWT_SID_SETPORTTESTMODE                0x3A
#define ETHSWT_SID_SETPORTLOOPBACKMODE            0x3B
#define ETHSWT_SID_SETPORTTXMODE                  0x3C
#define ETHSWT_SID_RUNPORTCABLEDIAGNOSTIC         0x45
#define ETHSWT_SID_GETPORTCABLEDIAGNOSTICSRESULT  0x3F
#define ETHSWT_SID_GETCFGDATARAW                  0x41
#define ETHSWT_SID_GETCFGDATAINFO                 0x42
#define ETHSWT_SID_PORTLINKSTATEREQUEST           0x49
#define ETHSWT_SID_GETMAXFIFOBUFFERFILLLEVEL      0x48
#define ETHSWT_SID_GETRXMGMTOBJECT                0x47
#define ETHSWT_SID_GETTXMGMTOBJECT                0x44
#define ETHSWT_SID_MAINFUNCTION                   0x1c
#define ETHSWT_SID_BACKGROUNDTASK                 0x46
/*Development Errors*/
#if (STD_ON == ETHSWT_DEV_ERROR_DETECT)
#define ETHSWT_E_INV_SWITCH_IDX     0x01
#define ETHSWT_E_UNINIT             0x02
#define ETHSWT_E_PARAM_POINTER      0x03
#define ETHSWT_E_INV_API            0x05
#define ETHSWT_E_INV_SWITCHPORT_IDX 0x06
#define ETHSWT_E_INV_CTRL_IDX       0x07
#define ETHSWT_E_INV_PARAM          0x08
#define ETHSWT_E_INIT_FAILED        0x09
#endif /*STD_ON == ETHSWT_DEV_ERROR_DETECT*/

/*Runtime Errors*/
#define ETHSWT_INIT_NOT_COMPLETED 0x01

/*Extended Production Errors*/
#define ETHSWT_E_ACCESS
/***********************************************************************************************************************
 *  FUNCTION PROTOTYPES
 ***********************************************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/**
 * @brief         Initializes the Ethernet Switch Driver.
 * @param[in]     CfgPtr: Pointer to the configuration data of the EthSwt module.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-X
 */
void EthSwt_Init(const EthSwt_ConfigType* CfgPtr);

#if (STD_ON == ETHSWT_VERSION_INFO_API)
/**
 * @brief         Returns the version information
 * @param[out]    versioninfo: Pointer to where to store the version information of this module.
 * @return        None
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-X
 */
void EthSwt_GetVersionInfo(Std_VersionInfoType* versioninfo);
#endif /* STD_ON == ETHSWT_VERSION_INFO_API */

#if (STD_ON == ETHSWT_SET_SWITCH_PORT_MODE_API)
/**
 * @brief         Enables/disables the indexed switch port.
 * @param[in]     SwitchIdx: Index of the switch within the context of the Ethernet Switch Driver
 * @param[in]     SwitchPortIdx: Index of the port at the addressed switch
 * @param[in]     PortMode: ETH_MODE_DOWN: Disable the addressed Ethernet switch port at the given Ethernet switch
 *                          ETH_MODE_ACTIVE: Enable the addressed Ethernet switch port at the given Ethernet switch
 * @return        Std_ReturnType
 * @retval        E_OK     : success.
 * @retval        E_NOT_OK : The indexed switch port could not be set to Port Mode, or the function is called instate
 *                           ETHSWT_STATE_UNINIT or ETHSWT_STATE_INIT.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-X
 */
Std_ReturnType EthSwt_SetSwitchPortMode(uint8 SwitchIdx, uint8 SwitchPortIdx, Eth_ModeType PortMode);
#endif /* STD_ON == ETHSWT_SET_SWITCH_PORT_MODE_API */

#if (STD_ON == ETHSWT_GET_SWITCH_PORT_MODE_API)
/**
 * @brief         Obtains the mode of the indexed switch port.
 * @param[in]     SwitchIdx: Index of the switch within the context of the Ethernet Switch Driver
 * @param[in]     SwitchPortIdx: Index of the port at the addressed switch
 * @param[out]    SwitchModePtr: ETH_MODE_DOWN: The Ethernet switch port of the given Ethernet switch is disabled.
 *                               ETH_MODE_ACTIVE: The Ethernet switch port of the given Ethernet switch is enabled
 * @return        Std_ReturnType
 * @retval        E_OK     : success.
 * @retval        E_NOT_OK : The mode of the indexed switch port could not be obtained, or the function is called in
 *                           state ETHSWT_STATE_UNINIT or ETHSWT_STATE_INIT.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-X
 */
Std_ReturnType EthSwt_GetSwitchPortMode(uint8 SwitchIdx, uint8 SwitchPortIdx, Eth_ModeType* SwitchModePtr);
#endif /* STD_ON == ETHSWT_GET_SWITCH_PORT_MODE_API */

#if (STD_ON == ETHSWT_GET_DUPLEX_MODE_API)
/**
 * @brief         Obtains the duplex mode of the indexed switch port.
 * @param[in]     SwitchIdx: Index of the switch within the context of the Ethernet Switch Driver
 * @param[in]     SwitchPortIdx: Index of the port at the addressed switch
 * @param[out]    DuplexModePtr: ETHTRCV_DUPLEX_MODE_HALF: half duplex connections
 *                   ETHTRCV_DUPLEXMODE_FULL: fullduplex connection switch port of the given Ethernet switch is enabled.
 * @return        Std_ReturnType
 * @retval        E_OK     : success.
 * @retval        E_NOT_OK : duplex mode of the indexed switch port could not be obtained, or the function is called
 *                           in state ETHSWT_STATE_UNINIT or ETHSWT_STATE_INIT.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-X
 */
Std_ReturnType EthSwt_GetDuplexMode(uint8 SwitchIdx, uint8 SwitchPortIdx, EthTrcv_DuplexModeType* DuplexModePtr);
#endif /* STD_ON == ETHSWT_GET_DUPLEX_MODE_API */

#if (STD_ON == ETHSWT_GET_LINK_STATE_API)
/**
 * @brief         Obtains the link state of the indexed switch port.
 * @param[in]     SwitchIdx: Index of the switch within the context of the Ethernet Switch Driver
 * @param[in]     SwitchPortIdx: Index of the port at the addressed switch
 * @param[out]    LinkStatePtr: ETHTRCV_LINK_STATE_DOWN: Switch port is disconnected
 *                              ETHTRCV_LINK_STATE_ACTIVE: Switch port is connected.
 * @return        Std_ReturnType
 * @retval        E_OK     : success.
 * @retval        E_NOT_OK : Link state of the indexed switch port could not be obtained, or the function is called in
 *                           state ETHSWT_STATE_UNINIT or ETHSWT_STATE_INIT.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-X
 */
Std_ReturnType EthSwt_GetLinkState(uint8 SwitchIdx, uint8 SwitchPortIdx, EthTrcv_LinkStateType* LinkStatePtr);
#endif /* STD_ON == ETHSWT_GET_LINK_STATE_API */

#if (STD_ON == ETHSWT_GET_BAUD_RATE_API)
/**
 * @brief         Obtains the baud rate of the indexed switch port.
 * @param[in]     SwitchIdx: Index of the switch within the context of the Ethernet Switch Driver
 * @param[in]     SwitchPortIdx: Index of the port at the addressed switch
 * @param[out]    BaudRatePtr: ETHTRCV_BAUD_RATE_10MBIT: 10MBit connection
 *                             ETHTRCV_BAUD_RATE_100MBIT: 100MBit connection
 *                             ETHTRCV_BAUD_RATE_1000MBIT: 1000MBit connection
 *                             ETHTRCV_BAUD_RATE_2500MBIT: 2500MBit connection.
 * @return        Std_ReturnType
 * @retval        E_OK     : success.
 * @retval        E_NOT_OK : Baud rate of the indexed switch port could not be obtained, or the function is called in
 *                           state ETHSWT_STATE_UNINIT or ETHSWT_STATE_INIT.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-X
 */
Std_ReturnType EthSwt_GetBaudRate(uint8 SwitchIdx, uint8 SwitchPortIdx, EthTrcv_BaudRateType* BaudRatePtr);
#endif /* STD_ON == ETHSWT_GET_BAUD_RATE_API */

#if (STD_ON == ETHSWT_START_SWITCH_PORT_AUTO_NEGOTIATION_API)
/**
 * @brief         Starts the auto-negotiation of the indexed switch port.
 * @param[in]     SwitchIdx: Index of the switch within the context of the Ethernet Switch Driver
 * @param[in]     SwitchPortIdx: Index of the port at the addressed switch
 * @return        Std_ReturnType
 * @retval        E_OK     : success.
 * @retval        E_NOT_OK : Automatic negotiation could not be started for the indexed switch port, or the function
 *                           is called in state ETHSWT_STATE_UNINIT or ETHSWT_STATE_INIT.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-X
 */
Std_ReturnType EthSwt_StartSwitchPortAutoNegotiation(uint8 SwitchIdx, uint8 SwitchPortIdx);
#endif /* STD_ON == ETHSWT_START_SWITCH_PORT_AUTO_NEGOTIATION_API */

#if (STD_ON == ETHSWT_CHECK_WAKEUP_API)
/**
 * @brief         API is called by EthIf. The Ethernet switch driver request to check for a wake-up at all Ethernet
 *                switch ports which reference an EthTrcv. For those Ethernet switch ports the call is forwarded to
 *                the referenced EthTrcv. The function could be called in context of an interrupt service routine
 *                or on task leve.
 * @param[in]     SwitchIdx: Index of the switch within the context of the Ethernet Switch Driver
 * @retval        E_OK     : request to check for a wake-up is accepted.
 * @retval        E_NOT_OK : request to check for a wake-up is not accepted.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-X
 */
Std_ReturnType EthSwt_CheckWakeup(uint8 SwitchIdx);
#endif /* STD_ON == ETHSWT_CHECK_WAKEUP_API */

#if (STD_ON == ETHSWT_GET_SWITCH_WAKEUP_REASON_API)
/**
 * @brief         This function obtains the wake up reasons of the the indexed Ethernet switch port by calling Eth
 *                Trcv_GetBusWuReason(...) of the referenced EthTrcv.
 * @param[in]     SwitchIdx: Index of the switch within the context of the Ethernet Switch Driver.
 * @param[in]     SwitchPortIdx: Index of the Ethernet switch port index in the context of the Ethernet switch driver.
 * @param[out]    Reason: Pointer to structure of least recent wakeup event, which was detected by the Ethernet switch
 *                        port.
 * @return        Std_ReturnType
 * @retval        E_OK     : Ethernet switch port wake up reason request has been accepted.
 * @retval        E_NOT_OK : Ethernet switch port wake up reason request has not been accepted.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-X
 */
Std_ReturnType EthSwt_GetSwitchPortWakeupReason(uint8 SwitchIdx, uint8 SwitchPortIdx, EthTrcv_WakeupReasonType* Reason);
#endif /* STD_ON == ETHSWT_GET_SWITCH_WAKEUP_REASON_API */

#if (STD_ON == ETHSWT_GET_PORT_MAC_ADDR_API)
/**
 * @brief         Obtains the port over which this MAC-address at the indexed switch can be reached The result
 *                might be used for a DHCP-server which will need the port/MAC-resolution.If for the PortIdxPtr the
 *                maximal possible value (255) is returned the given MAC address cannot be reached via a port of this
 *                switch. If multiple ports were found the API returns E_NOT_OK.
 * @param[in]     SwitchIdx: Index of the switch within the context of the Ethernet Switch Driver
 * @param[in]     MacAddrPtr: MAC-address for which a switch port is searched over which the node with this MAC-address
 *                can be reached
 * @param[out]    PortIdxPtr: Pointer to the port index.
 * @return        Std_ReturnType
 * @retval        E_OK     : success.
 * @retval        E_NOT_OK : multiple ports were found.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-X
 */
Std_ReturnType EthSwt_GetPortMacAddr(uint8 SwitchIdx, const uint8* MacAddrPtr, uint8* PortIdxPtr);
#endif /* STD_ON == ETHSWT_GET_PORT_MAC_ADDR_API */

#if (STD_ON == ETHSWT_ENABLE_VLAN_API)
/**
 * @brief         Enables or disables a pre-configured VLAN at a certain port of a switch.
 * @param[in]     SwitchIdx: Index of the switch within the context of the Ethernet Switch Driver
 * @param[in]     SwitchPortIdx: Index of the port at the addressed switch
 * @param[in]     VlanId: VLAN-ID to a preconfigured configuration on the given ingress port
 * @param[in]     Enable: 1 = VLAN-configuration enabled 0 = VLAN-configuration disabled(frames with given VLAN-ID
 *                        will be dropped)
 * @return        Std_ReturnType
 * @retval        E_OK     : success.
 * @retval        E_NOT_OK : buffer level could not be obtained.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-X
 */
Std_ReturnType EthSwt_EnableVlan(uint8 SwitchIdx, uint8 SwitchPortIdx, uint16 VlanId, boolean Enable);
#endif /* STD_ON == ETHSWT_ENABLE_VLAN_API */

#if (STD_ON == ETHSWT_SET_MAC_LEARNING_MODE_API)
/**
 * @brief         Sets the MAC learning mode in one of the tree modes
 * @param[in]     SoConId: Socket connection index specifying the socket connection for which the IP address
 * @param[in]     SwitchPortIdx: Index of the port at the addressed switch
 * @param[in]     MacLearningMode: Defines whether MAC addresses shall be learned and if they shall be learned in
 *                                 software or hardware.
 * @retval        E_OK     : success.
 * @retval        E_NOT_OK : configuration could be persistently reset
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-X
 */
Std_ReturnType EthSwt_SetMacLearningMode(uint8 SwitchIdx, uint8 SwitchPortIdx, EthSwt_MacLearningType MacLearningMode);
#endif /* STD_ON == ETHSWT_SET_MAC_LEARNING_MODE_API */

#if (STD_ON == ETHSWT_GET_MAC_LEARNING_MODE_API)
/**
 * @brief         Returns the MAC learning mode
 * @param[in]     SwitchIdx: Index of the switch within the context of the Ethernet Switch Driver
 * @param[in]     SwitchPortIdx: Index of the port at the addressed switch
 * @param[out]    MacLearningMode: Defines whether MAC addresses shall be learned and if they shall be learned in
 *                                 software or hardware.
 * @return        Std_ReturnType
 * @retval        E_OK     : success.
 * @retval        E_NOT_OK : configuration could be persistently reset.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-X
 */
Std_ReturnType EthSwt_GetMacLearningMode(uint8 SwitchIdx, uint8 SwitchPortIdx, EthSwt_MacLearningType* MacLearningMode);
#endif /* STD_ON == ETHSWT_GET_MAC_LEARNING_MODE_API */

#if (STD_ON == ETHSWT_SET_FORWARDING_MODE_API)
/**
 * @brief         Configures switch to start or stop forwarding for all ports.This API call may be used during switch
 *                configuration verification.
 * @param[in]     SwitchIdx: Index of the switch within the context of the Ethernet Switch Driver
 * @param[in]     mode: True Forewarding enabled, False Forwarding disabled
 * @return        Std_ReturnType
 * @retval        E_OK     : stopping of frame forwarding succeeded,
 * @retval        E_NOT_OK : stopping of frame forwarding not succeeded.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-X
 */
Std_ReturnType EthSwt_SetForwardingMode(uint8 SwitchIdx, boolean mode);
#endif /* STD_ON == ETHSWT_SET_FORWARDING_MODE_API */

#if (STD_ON == ETHSWT_SET_PORT_TX_MODE_API)
/**
 * @brief         Activates a given transmission mode of the indexed Ethernet switch port.
 * @param[in]     SwitchIdx: Index of the switch within the context of the Ethernet Switch Driver
 * @param[in]     PortIdx: Index of the port at the addressed switch
 * @param[in]     Mode: Transmission mode to be activated
 * @return        Std_ReturnType
 * @retval        E_OK     : the port Tx mode for the indexed Ethernet switch port was activated successfully.
 * @retval        E_NOT_OK : the port Tx mode for the indexed Ethernet switch port was not activated successfully.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-X
 */
Std_ReturnType EthSwt_SetPortTxMode(uint8 SwitchIdx, uint8 PortIdx, EthTrcv_PhyTxModeType Mode);
#endif /* STD_ON == ETHSWT_SET_PORT_TX_MODE_API */

#if (STD_ON == ETHSWT_WRITE_PORT_MIRROR_CONFIGURATION_API)
/**
 * @brief         Store the given port mirror configuration in a shadow buffer in the Ethernet switch driver for the
 *                given MirroredSwitchIdx.
 * @param[in]     MirroredSwitchIdx: Index of the switch within the context of the Ethernet Switch Driver, where the
 *                Ethernet switch port is located, that has to be mirrored.
 * @param[in]     PortMirrorConfigurationPtr: Pointer of the port configuration, which shall be stored in a shadow
 *                buffer in the Ethernet switch driver.
 * @return        Std_ReturnType
 * @retval        E_OK     : the port mirror configuration for the indexed Ethernet switch port was written.
 * @retval        E_NOT_OK : the port mirror configuration for the indexed Ethernet switch port was not written.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-X
 */
Std_ReturnType EthSwt_WritePortMirrorConfiguration(
    uint8                           MirroredSwitchIdx,
    const EthSwt_PortMirrorCfgType* PortMirrorConfigurationPtr);
#endif /* STD_ON == ETHSWT_WRITE_PORT_MIRROR_CONFIGURATION_API */

#if (STD_ON == ETHSWT_READ_PORT_MIRROR_CONFIGURATION_API)
/**
 * @brief         Obtain the port mirror configuration of the given Ethernet switch.
 * @param[in]     MirroredSwitchIdx: Index of the switch within the context of the Ethernet Switch Driver, where
 *                the Ethernet switch port is located, that has to be mirrored.
 * @param[in]     PortMirrorConfigurationPtr: Pointer to the memory where the port configuration shall be stored.
 * @return        Std_ReturnType
 * @retval        E_OK     : the port mirror configuration for the indexed Ethernet switch port was red successfully.
 * @retval        E_NOT_OK : the port mirror configuration for the indexed Ethernet switch was not red successfully.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-X
 */
Std_ReturnType
    EthSwt_ReadPortMirrorConfiguration(uint8 MirroredSwitchIdx, EthSwt_PortMirrorCfgType* PortMirrorConfigurationPtr);
#endif /* STD_ON == ETHSWT_READ_PORT_MIRROR_CONFIGURATION_API */

#if (STD_ON == ETHSWT_DELETE_PORT_MIRROR_CONFIGURATION_API)
/**
 * @brief         Delete the stored port mirror configuration of the given MirroredSwitchIdx. If no port mirror
 *                configuration was found for the given MirroredSwitchIdx, the return value shall be E_OK.
 * @param[in]     MirroredSwitchIdx: Index of the switch within the context of the Ethernet Switch Driver.
 * @return        Std_ReturnType
 * @retval        E_OK     : Port mirror configuration was deleted successfully.
 * @retval        E_NOT_OK : Port mirror configuration was not deleted successfully. (e.g. the port mirroring is
 * enabled).
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-X
 */
Std_ReturnType EthSwt_DeletePortMirrorConfiguration(uint8 MirroredSwitchIdx);
#endif /* STD_ON == ETHSWT_DELETE_PORT_MIRROR_CONFIGURATION_API */

#if (STD_ON == ETHSWT_GET_PORT_MIRROR_STATE_API)
/**
 * @brief         Obtain the current status of the port mirroring for the indexed Ethernet switch port.
 * @param[in]     SwitchIdx: Index of the switch within the context of the Ethernet Switch Driver
 * @param[in]     SwitchPortIdx: Index of the port at the addressed switch
 * @param[out]    PortMirrorStatePtr: Pointer to the memory where the port mirroring state
 *                (either PORT_MIRRORING_ENABLED or PORT_MIRRORING_ DISABLED)of the given Ethernet switch port shall
 *                be stored.
 * @return        Std_ReturnType
 * @retval        E_OK     : the port mirroring state for the indexed Ethernet switch port returned successfully.
 * @retval        E_NOT_OK : the port mirror configuration for the indexed Ethernet switch returned not successfully.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-X
 */
Std_ReturnType
    EthSwt_GetPortMirrorState(uint8 SwitchIdx, uint8 PortIdx, EthSwt_PortMirrorStateType* PortMirrorStatePtr);
#endif /* STD_ON == ETHSWT_GET_PORT_MIRROR_STATE_API */

#if (STD_ON == ETHSWT_SET_PORT_MIRROR_STATE_API)
/**
 * @brief         Request to set the given port mirroring state of the port mirror configuration for the given
 *                Ethernet switch.
 * @param[in]     MirroredSwitchIdx: Index of the Ethernet switch within the context of the Ethernet Switch Driver,
 *                where the port mirroring configuration is located that has to be enabled anddisabled, repectively.
 * @param[in]     PortMirrorState: Contain the requested port mirroring state either PORT_MIRRORING_ENABLED or
 *                PORT_MIRRORING_DISABLED
 * @return        Std_ReturnType
 * @retval        E_OK    : the requested port mirroring state for the indexed Ethernet switch port was set successfully
 * @retval        E_NOT_OK: the requested port mirroring state for the indexed Ethernet switch was not set successfully.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-X
 */
Std_ReturnType EthSwt_SetPortMirrorState(uint8 MirroredSwitchIdx, EthSwt_PortMirrorStateType PortMirrorState);
#endif /* STD_ON == ETHSWT_SET_PORT_MIRROR_STATE_API */

/**
 * @brief         Service to support asynchronous behavior of API calls.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-X
 */
void EthSwt_MainFunction(void);

/**
 * @brief         Returns the version information.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-X
 */
void EthSwt_BackgroundTask(void);

/***********************************************************************************************************************
 *  GLOBAL data declaration
 **********************************************************************************************************************/
extern CONST(EthSwt_ConfigType, ETHSWT_CONST) EthSwt_Config[ETHSWT_DEVICE_NUM];

#ifdef __cplusplus
}
#endif

#endif /*ifndef ETHSWT_H_*/
