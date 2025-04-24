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
********************************************************************************
**                                                                            **
**  FILENAME    : EthIf.h                                                     **
**                                                                            **
**  Created on  :                                                             **
**  Author      : YangBo                                                      **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
* V2.0.5 [2022/05/17] [zhiqiang.huang] Add null pointer check for eth trcv API.
*******************************************************************************/

#ifndef ETHIF_H
#define ETHIF_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "EthIf_Types.h"
#include "EthIf_Cfg.h"
#include "SchM_EthIf.h"
#if (STD_ON == ETHIF_WAKEUP_SUPPORT)
#include "EcuM_Types.h"
#endif
#include "EthSM.h"
#include "ComStack_Types.h"

extern const EthIf_ConfigType EthIf_ConfigData;
/*TODO EthIf_RxIndication*/
extern struct netif g_netif;

extern const EthIf_EthDriverApiType Eth_DriverApi[ETHIF_ETH_DRIVER_API_NUM];

#if (STD_ON == ETHIF_USE_ETHTRCV)
extern const EthIf_EthTrcvDriverApiType EthTrcv_DriverApi[ETHIF_ETHTRCV_DRIVER_API_NUM];
#endif /* STD_ON == ETHIF_USE_ETHTRCV */

#if (STD_ON == ETHIF_USE_ETHSWT)
extern const EthIf_EthSwtDriverApiType EthSwt_DriverApi[ETHIF_ETHTRCV_DRIVER_API_NUM];
#endif /* STD_ON == ETHIF_USE_ETHSWT */

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/* @req SWS_EthIf_00006 */
#define ETHIF_H_AR_RELEASE_MAJOR_VERSION    (4u)
#define ETHIF_H_AR_RELEASE_MINOR_VERSION    (5u)
#define ETHIF_H_AR_RELEASE_REVISION_VERSION (0u)

#define ETHIF_H_SW_MAJOR_VERSION            (2u)
#define ETHIF_H_SW_MINOR_VERSION            (0u)
#define ETHIF_H_SW_PATCH_VERSION            (14u)

#define ETHIF_MODULE_ID                     (65u)
#define ETHIF_INSTANCE_ID                   (0u)
#define ETHIF_VENDOR_ID                     (62u)
/*Error IDs*/
#define ETHIF_E_INV_CTRL_IDX       0x01u
#define ETHIF_E_INV_TRCV_IDX       0x02u
#define ETHIF_E_INV_SWT_IDX        0x03u
#define ETHIF_E_INV_PORT_GROUP_IDX 0x04u
#define ETHIF_E_UNINIT             0x05u
#define ETHIF_E_PARAM_POINTER      0x06u
#define ETHIF_E_INV_PARAM          0x07u
#define ETHIF_E_INIT_FAILED        0x08u
#define ETHIF_E_INV_PORT_IDX       0x09u

/** Service IDs */
#define ETHIF_SERVICE_ID_INIT                                       0x01u
#define ETHIF_SERVICE_ID_SET_CTRL_MODE                              0x03u
#define ETHIF_SERVICE_ID_GET_CTRL_MODE                              0x04u
#define ETHIF_SERVICE_ID_SET_SWITCH_PORTGROUP_MODE                  0x06u
#define ETHIF_SERVICE_ID_START_ALL_PORTS                            0x07u
#define ETHIF_SERVICE_ID_GET_PHY_ADDR                               0x08u
#define ETHIF_SERVICE_ID_PROVIDE_TX_BUFFER                          0x09u
#define ETHIF_SERVICE_ID_TRANSMIT                                   0x0Au
#define ETHIF_SERVICE_ID_GET_VERSION_INFO                           0x0Bu
#define ETHIF_SERVICE_ID_UPDATE_PHY_ADDR_FILTER                     0x0Cu
#define ETHIF_SERVICE_ID_SET_PHY_ADDR                               0x0Du
#define ETHIF_SERVICE_ID_GET_SWITCH_PORT_SIGNAL_QUALITY             0x1Au
#define ETHIF_SERVICE_ID_CLEAR_SWITCH_PORT_SIGNAL_QUALITY           0x1Bu
#define ETHIF_SERVICE_ID_GET_CURRENT_TIME                           0x22u
#define ETHIF_SERVICE_ID_ENABLE_EGRESS_TIME_STAMP                   0x23u
#define ETHIF_SERVICE_ID_GET_EGRESS_TIME_STAMP                      0x24u
#define ETHIF_SERVICE_ID_GET_INGRESS_TIME_STAMP                     0x25u
#define ETHIF_SERVICE_ID_SET_CORRECTION_TIME                        0x26u
#define ETHIF_SERVICE_ID_SET_GLOBAL_TIME                            0x27u
#define ETHIF_SERVICE_ID_GET_PORT_MAC_ADDR                          0x28u
#define ETHIF_SERVICE_ID_GET_ARL_TABEL                              0x29u
#define ETHIF_SERVICE_ID_GET_BUF_LEVEL                              0x2Au
#define ETHIF_SERVICE_ID_GET_DROP_COUNT                             0x2Bu
#define ETHIF_SERVICE_ID_STORE_CONFIG                               0x2Cu
#define ETHIF_SERVICE_ID_RESET_CONFIG                               0x2Du
#define ETHIF_SERVICE_ID_SET_TRCV_WAKEUP_MODE                       0x2Eu
#define ETHIF_SERVICE_ID_GET_TRCV_WAKEUP_MODE                       0x2Fu
#define ETHIF_SERVICE_ID_CHECK_WAKE_UP                              0x30u
#define ETHIF_SERVICE_ID_SET_SWITCH_MGMT_INFO                       0x38u
#define ETHIF_SERVICE_ID_SWITCH_ENABLE_TIME_STAMPING                0x39u
#define ETHIF_SERVICE_ID_VERIFY_CONFIG                              0x40u
#define ETHIF_SERVICE_ID_SET_FORWARDING_MODE                        0x41u
#define ETHIF_SERVICE_ID_GET_VLANID                                 0x43u
#define ETHIF_SERVICE_ID_GET_CTRL_IDXLIST                           0x44u
#define ETHIF_SERVICE_ID_GET_AND_RESET_MEASUREMENT_DATA             0x45u
#define ETHIF_SERVICE_ID_GET_RX_MGMT_OBJECT                         0x47u
#define ETHIF_SERVICE_ID_GET_TX_MGMT_OBJECT                         0x48u
#define ETHIF_SERVICE_ID_GET_SWITCH_PORT_MODE                       0x49u
#define ETHIF_SERVICE_ID_GET_TRCV_MODE                              0x4Au
#define ETHIF_SERVICE_ID_SWITCH_PORT_GET_LINK_STATE                 0x4Bu
#define ETHIF_SERVICE_ID_SWITCH_PORT_GET_BAUD_RATE                  0x4Du
#define ETHIF_SERVICE_ID_SWITCH_PORT_GET_DUPLEX_MODE                0x4Fu
#define ETHIF_SERVICE_ID_SWITCH_PORT_GET_COUNTER_VALUE              0x51u
#define ETHIF_SERVICE_ID_SWITCH_PORT_GET_RX_STATS                   0x52u
#define ETHIF_SERVICE_ID_SWITCH_PORT_GET_TX_STATS                   0x53u
#define ETHIF_SERVICE_ID_SWITCH_PORT_GET_TX_ERROR_COUNTER_VALUE     0x54u
#define ETHIF_SERVICE_ID_SWITCH_PORT_GET_MAC_LEARNING_MODE          0x55u
#define ETHIF_SERVICE_ID_GET_SWITCH_PORT_IDENTIFIER                 0x56u
#define ETHIF_SERVICE_ID_GET_SWITCH_IDENTIFIER                      0x57u
#define ETHIF_SERVICE_ID_WRITE_PORT_MIRROR_CONFIGURATION            0x58u
#define ETHIF_SERVICE_ID_READ_PORT_MIRROR_CONFIGURATION             0x59u
#define ETHIF_SERVICE_ID_DELETE_PORT_MIRROR_CONFIGURATION           0x5Au
#define ETHIF_SERVICE_ID_GET_PORT_MIRROR_STATE                      0x5Bu
#define ETHIF_SERVICE_ID_SET_PORT_MIRROR_STATE                      0x5Cu
#define ETHIF_SERVICE_ID_SET_PORT_TEST_MODE                         0x5Du
#define ETHIF_SERVICE_ID_SET_PORT_LOOPBACK_MODE                     0x5Eu
#define ETHIF_SERVICE_ID_SET_PORT_TX_MODE                           0x5Fu
#define ETHIF_SERVICE_ID_GET_PORT_CABLE_DIAGNOSTICS_RESULT          0x60u
#define ETHIF_SERVICE_ID_RUN_CABLE_DIAGNOSTIC                       0x61u
#define ETHIF_SERVICE_ID_SWITCH_GET_CFG_DATA_RAW                    0x63u
#define ETHIF_SERVICE_ID_SWITCH_GET_CFG_DATA_INFO                   0x64u
#define ETHIF_SERVICE_ID_SWITCH_PORT_GET_MAX_FIFO_BUFFER_FILL_LEVEL 0X65u

/* Callback notifications function service Ids */
#define ETHIF_SERVICE_ID_RX_INDICATION        0x10u
#define ETHIF_SERVICE_ID_TX_CONFIRMATION      0x11u
#define ETHIF_SERVICE_ID_CTRL_MODE_INDICATION 0x0Eu
#define ETHIF_SERVICE_ID_TRCV_MODE_INDICATION 0x0Fu

/* Scheduled functions service IDs */
#define ETHIF_SERVICE_ID_MAIN_FUNCTION_RX    0x20u
#define ETHIF_SERVICE_ID_MAIN_FUNCTION_TX    0x21u
#define ETHIF_SERVICE_ID_MAIN_FUNCTION_STATE 0x05u

#define ETHIF_NOUSED(paramter)               (void)(paramter)
/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
/* @req SWS_EthIf_00024 */
FUNC(void, ETHIF_CODE)
EthIf_Init(P2CONST(EthIf_ConfigType, ETHIF_APPL_DATA, ETHIF_VAR) CfgPtr); /** Init function for ETHIF */
/* @req SWS_EthIf_00034 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SetControllerMode(VAR(uint8, ETHIF_VAR) CtrlIdx, VAR(Eth_ModeType, ETHIF_VAR) CtrlMode);
/* @req SWS_EthIf_00039 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_GetControllerMode(VAR(uint8, ETHIF_VAR) CtrlIdx, P2VAR(Eth_ModeType, ETHIF_APPL_DATA, ETHIF_VAR) CtrlModePtr);
/* @req SWS_EthIf_00055 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_GetTransceiverMode(VAR(uint8, ETHIF_VAR) TrcvIdx, P2VAR(Eth_ModeType, ETHIF_APPL_DATA, ETHIF_VAR) TrcvModePtr);
#if (STD_ON == ETHIF_WAKEUP_SUPPORT)
/* @req SWS_EthIf_00233 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SetTransceiverWakeupMode(VAR(uint8, ETHIF_VAR) TrcvIdx, VAR(EthTrcv_WakeupModeType, ETHIF_VAR) TrcvWakeupMode);
/* @req SWS_EthIf_00238 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_GetTransceiverWakeupMode(
    VAR(uint8, ETHIF_VAR) TrcvIdx,
    P2VAR(EthTrcv_WakeupModeType, ETHIF_APPL_DATA, ETHIF_VAR) TrcvWakeupModePtr);
/* @req SWS_EthIf_00244 */
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_CheckWakeup(VAR(EcuM_WakeupSourceType, ETHIF_VAR) WakeupSource);
#endif
/* @req SWS_EthIf_00061 */
FUNC(void, ETHIF_CODE)
EthIf_GetPhysAddr(VAR(uint8, ETHIF_VAR) CtrlIdx, P2VAR(uint8, ETHIF_APPL_DATA, ETHIF_VAR) PhysAddrPtr);
/* @req SWS_EthIf_00132 */
FUNC(void, ETHIF_CODE)
EthIf_SetPhysAddr(VAR(uint8, ETHIF_VAR) CtrlIdx, P2CONST(uint8, ETHIF_APPL_DATA, ETHIF_VAR) PhysAddrPtr);
/* @req SWS_EthIf_00139 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_UpdatePhysAddrFilter(
    VAR(uint8, ETHIF_VAR) CtrlIdx,
    P2CONST(uint8, ETHIF_APPL_DATA, ETHIF_VAR) PhysAddrPtr,
    VAR(Eth_FilterActionType, ETHIF_VAR) Action);
/* @req SWS_EthIf_00154 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_GetCurrentTime(
    VAR(uint8, ETHIF_VAR) CtrlIdx,
    P2VAR(Eth_TimeStampQualType, ETHIF_APPL_DATA, ETHIF_VAR) timeQualPtr,
    P2VAR(Eth_TimeStampType, ETHIF_APPL_DATA, ETHIF_VAR) timeStampPtr);
/* @req SWS_EthIf_00160 */
FUNC(void, ETHIF_CODE)
EthIf_EnableEgressTimeStamp(VAR(uint8, ETHIF_VAR) CtrlIdx, VAR(Eth_BufIdxType, ETHIF_VAR) BufIdx);
/* @req SWS_EthIf_00166 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_GetEgressTimeStamp(
    VAR(uint8, ETHIF_VAR) CtrlIdx,
    VAR(Eth_BufIdxType, ETHIF_VAR) BufIdx,
    P2VAR(Eth_TimeStampQualType, ETHIF_APPL_DATA, ETHIF_VAR) timeQualPtr,
    P2VAR(Eth_TimeStampType, ETHIF_APPL_DATA, ETHIF_VAR) timeStampPtr);
/* @req SWS_EthIf_00172 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_GetIngressTimeStamp(
    VAR(uint8, ETHIF_VAR) CtrlIdx,
    P2CONST(Eth_DataType, ETHIF_APPL_DATA, ETHIF_VAR) DataPtr,
    P2VAR(Eth_TimeStampQualType, ETHIF_APPL_DATA, ETHIF_VAR) timeQualPtr,
    P2VAR(Eth_TimeStampType, ETHIF_APPL_DATA, ETHIF_VAR) timeStampPtr);
/* @req SWS_EthIf_00067 */
FUNC(BufReq_ReturnType, ETHIF_CODE)
EthIf_ProvideTxBuffer(
    VAR(uint8, ETHIF_VAR) CtrlIdx,
    VAR(Eth_FrameType, ETHIF_VAR) FrameType,
    VAR(uint8, ETHIF_VAR) Priority,
    P2VAR(Eth_BufIdxType, ETHIF_APPL_DATA, ETHIF_VAR) BufIdxPtr,
    P2VAR(uint8*, ETHIF_APPL_DATA, ETHIF_VAR) BufPtr,
    P2VAR(uint16, ETHIF_APPL_DATA, ETHIF_VAR) LenBytePtr);
/* @req SWS_EthIf_00075 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_Transmit(
    VAR(uint8, ETHIF_VAR) CtrlIdx,
    VAR(Eth_BufIdxType, ETHIF_VAR) BufIdx,
    VAR(Eth_FrameType, ETHIF_VAR) FrameType,
    VAR(boolean, ETHIF_VAR) TxConfirmation,
    VAR(uint16, ETHIF_VAR) LenByte,
    P2CONST(uint8, ETHIF_APPL_DATA, ETHIF_VAR) PhysAddrPtr);
#if (ETHIF_VERSION_INFO_API == STD_ON)
FUNC(void, ETHIF_CODE) EthIf_GetVersionInfo(Std_VersionInfoType* VersionInfoPtr);
#endif
#if (STD_ON == ETH_GET_CTRLIDX_LIST)
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_GetCtrlIdxList(
    P2VAR(uint8, ETHIF_APPL_DATA, ETHIF_VAR) NumberOfCtrlIdx,
    P2VAR(uint8, ETHIF_APPL_DATA, ETHIF_VAR) CtrlIdxListPtr);
#endif
#if (ETHIF_ENABLE_RX_INTERRUPT == STD_OFF)
/* @req SWS_EthIf_00097 */
FUNC(void, ETHIF_CODE) EthIf_MainFunctionRx(void);
#endif
#if (ETHIF_ENABLE_TX_INTERRUPT == STD_OFF)
/* @req  SWS_EthIf_00113 */
FUNC(void, ETHIF_CODE) EthIf_MainFunctionTx(void);
#endif

FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_GetVlanId(VAR(uint8, ETHIF_VAR) CtrlIdx, P2VAR(uint16, ETHIF_APPL_DATA, ETHIF_VAR) VlanIdPtr);

#if (STD_ON == ETHIF_USE_ETHSWT)
#if (STD_ON == ETHIF_GET_PORT_MAC_ADDR_API)
FUNC(BufReq_ReturnType, ETHIF_CODE)
EthIf_GetPortMacAddr(
    P2CONST(uint8, ETHIF_APPL_DATA, ETHIF_VAR) MacAddrPtr,
    P2VAR(uint8, ETHIF_APPL_DATA, ETHIF_VAR) SwtichIdxPtr,
    P2VAR(uint8, ETHIF_APPL_DATA, ETHIF_VAR) PortIdxPtr);
#endif /* STD_ON == ETHIF_GET_PORT_MAC_ADDR_API */

#if (STD_ON == ETHIF_GET_ARL_TABLE)
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_GetArlTable(
    VAR(uint8, ETHIF_VAR) switchIdx,
    P2VAR(uint16, ETHIF_APPL_DATA, ETHIF_VAR) numberOfElements,
    P2VAR(Eth_MacVlanType, ETHIF_APPL_DATA, ETHIF_VAR) arlTableListPointer);
#endif /* STD_ON == ETHIF_GET_ARL_TABLE */

#if (STD_ON == ETHIF_STORE_CONFIGURATION_API)
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_StoreConfiguration(VAR(uint8, ETHIF_VAR) SwitchIdx);
#endif /* STD_ON == ETHIF_STORE_CONFIGURATION_API */

#if (STD_ON == ETHIF_RESET_CONFIGURATION_API)
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_ResetConfiguration(VAR(uint8, ETHIF_VAR) SwitchIdx);
#endif /* STD_ON == ETHIF_RESET_CONFIGURATION_API */

FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SwitchPortGroupRequestMode(
    VAR(EthIf_SwitchPortGroupIdxType, ETHIF_VAR) PortGroupIdx,
    VAR(Eth_ModeType, ETHIF_VAR) PortMode);

FUNC(Std_ReturnType, ETHIF_CODE) EthIf_StartAllPorts(void);

#if (STD_ON == ETHIF_SWITCH_MANAGEMENT_SUPPORT)
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SetSwitchMgmtInfo(
    VAR(uint8, ETHIF_VAR) CtrlIdx,
    VAR(Eth_BufIdxType, ETHIF_VAR) BufIdx,
    P2VAR(EthSwt_MgmtInfoType, ETHIF_APPL_DATA, ETHIF_VAR) MgmtInfoPtr);
#endif /* STD_ON == ETHIF_SWITCH_MANAGEMENT_SUPPORT */

FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_GetRxMgmtObject(
    VAR(uint8, ETHIF_VAR) CtrlIdx,
    P2VAR(Eth_DataType, ETHIF_APPL_DATA, ETHIF_VAR) DataPtr,
    EthSwt_MgmtObjectType** MgmtObjectPtr);

FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_GetTxMgmtObject(
    VAR(uint8, ETHIF_VAR) CtrlIdx,
    VAR(Eth_BufIdxType, ETHIF_VAR) BufIdx,
    EthSwt_MgmtObjectType** MgmtObjectPtr);

#if (STD_ON == ETHIF_GLOBAL_TIME_SUPPORT)
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SwitchEnableTimeStamping(
    VAR(uint8, ETHIF_VAR) CtrlIdx,
    VAR(Eth_BufIdxType, ETHIF_VAR) BufIdx,
    P2VAR(EthSwt_MgmtInfoType, ETHIF_APPL_DATA, ETHIF_VAR) MgmtInfo);
#endif /* STD_ON == ETHIF_GLOBAL_TIME_SUPPORT */

#if (STD_ON == ETHIF_VERIFY_CONFIG_API)
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_VerifyConfig(VAR(uint8, ETHIF_VAR) SwitchIdx, P2VAR(boolean, ETHIF_APPL_DATA, ETHIF_VAR) Result);
#endif /* STD_ON == ETHIF_VERIFY_CONFIG_API */

#if (STD_ON == ETHIF_SET_FORWARDINGD_MODE_API)
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SetForwardingMode(VAR(uint8, ETHIF_VAR) SwitchIdx, VAR(boolean, ETHIF_VAR) mode);
#endif /* STD_ON == ETHIF_SET_FORWARDINGD_MODE_API */

FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_GetSwitchPortSignalQuality(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    VAR(uint8, ETHIF_VAR) SwitchPortIdx,
    P2VAR(EthIf_SignalQualityResultType, ETHIF_APPL_DATA, ETHIF_VAR) ResultPtr);

FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_ClearSwitchPortSignalQuality(VAR(uint8, ETHIF_VAR) SwitchIdx, VAR(uint8, ETHIF_VAR) SwitchPortIdx);

FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_GetSwitchPortMode(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    VAR(uint8, ETHIF_VAR) SwitchPortIdx,
    P2VAR(Eth_ModeType, ETHIF_APPL_DATA, ETHIF_VAR) PortModePtr);

FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SwitchPortGetLinkState(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    VAR(uint8, ETHIF_VAR) SwitchPortIdx,
    P2VAR(EthTrcv_LinkStateType, ETHIF_APPL_DATA, ETHIF_VAR) LinkStatePtr);

FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SwitchPortGetBaudRate(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    VAR(uint8, ETHIF_VAR) SwitchPortIdx,
    P2VAR(EthTrcv_BaudRateType, ETHIF_APPL_DATA, ETHIF_VAR) BaudRatePtr);

FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SwitchPortGetDuplexMode(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    VAR(uint8, ETHIF_VAR) SwitchPortIdx,
    P2VAR(EthTrcv_DuplexModeType, ETHIF_APPL_DATA, ETHIF_VAR) DuplexModePtr);

FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SwitchPortGetCounterValue(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    VAR(uint8, ETHIF_VAR) SwitchPortIdx,
    P2VAR(Eth_CounterType, ETHIF_APPL_DATA, ETHIF_VAR) CounterPtr);

FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SwitchPortGetRxStats(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    VAR(uint8, ETHIF_VAR) SwitchPortIdx,
    P2VAR(Eth_RxStatsType, ETHIF_APPL_DATA, ETHIF_VAR) RxStatsPtr);

FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SwitchPortGetTxStats(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    VAR(uint8, ETHIF_VAR) SwitchPortIdx,
    P2VAR(Eth_TxStatsType, ETHIF_APPL_DATA, ETHIF_VAR) TxStatsPtr);

FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SwitchPortGetTxErrorCounterValues(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    VAR(uint8, ETHIF_VAR) SwitchPortIdx,
    P2VAR(Eth_TxErrorCounterValuesType, ETHIF_APPL_DATA, ETHIF_VAR) TxStatsPtr);

FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SwitchPortGetMacLearningMode(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    VAR(uint8, ETHIF_VAR) SwitchPortIdx,
    P2VAR(EthSwt_MacLearningType, ETHIF_APPL_DATA, ETHIF_VAR) MacLearningModePtr);

FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_GetSwitchPortIdentifier(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    VAR(uint8, ETHIF_VAR) SwitchPortIdx,
    P2VAR(uint32, ETHIF_APPL_DATA, ETHIF_VAR) OrgUniqueIdPtr,
    P2VAR(uint8, ETHIF_APPL_DATA, ETHIF_VAR) ModelNrPtr,
    P2VAR(uint8, ETHIF_APPL_DATA, ETHIF_VAR) RevisionNrPtr);

FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_GetSwitchIdentifier(VAR(uint8, ETHIF_VAR) SwitchIdx, P2VAR(uint32, ETHIF_APPL_DATA, ETHIF_VAR) OrgUniqueIdPtr);

FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_WritePortMirrorConfiguration(
    VAR(uint8, ETHIF_VAR) MirroredSwitchIdx,
    P2CONST(EthSwt_PortMirrorCfgType, ETHIF_APPL_DATA, ETHIF_VAR) PortMirrorConfigurationPtr);

FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_ReadPortMirrorConfiguration(
    VAR(uint8, ETHIF_VAR) MirroredSwitchIdx,
    P2VAR(EthSwt_PortMirrorCfgType, ETHIF_APPL_DATA, ETHIF_VAR) PortMirrorConfigurationPtr);

FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_DeletePortMirrorConfiguration(VAR(uint8, ETHIF_VAR) MirroredSwitchIdx);

FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_GetPortMirrorState(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    VAR(uint8, ETHIF_VAR) PortIdx,
    P2VAR(EthSwt_PortMirrorStateType, ETHIF_APPL_DATA, ETHIF_VAR) PortMirrorStatePtr);

FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SetPortMirrorState(
    VAR(uint8, ETHIF_VAR) MirroredSwitchIdx,
    VAR(uint8, ETHIF_VAR) PortIdx,
    VAR(EthSwt_PortMirrorStateType, ETHIF_VAR) PortMirrorState);

FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SetPortTestMode(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    VAR(uint8, ETHIF_VAR) PortIdx,
    VAR(EthTrcv_PhyTestModeType, ETHIF_VAR) Mode);

FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SetPortLoopbackMode(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    VAR(uint8, ETHIF_VAR) PortIdx,
    VAR(EthTrcv_PhyLoopbackModeType, ETHIF_VAR) Mode);

FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SetPortTxMode(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    VAR(uint8, ETHIF_VAR) PortIdx,
    VAR(EthTrcv_PhyTxModeType, ETHIF_VAR) Mode);

FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_GetPortCableDiagnosticsResult(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    VAR(uint8, ETHIF_VAR) PortIdx,
    P2VAR(EthTrcv_CableDiagResultType, ETHIF_APPL_DATA, ETHIF_VAR) ResultPtr);

FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_RunPortCableDiagnostic(VAR(uint8, ETHIF_VAR) SwitchIdx, VAR(uint8, ETHIF_VAR) PortIdx);

FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SwitchGetCfgDataRaw(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    VAR(uint32, ETHIF_VAR) Offset,
    VAR(uint16, ETHIF_VAR) Length,
    P2VAR(uint8, ETHIF_APPL_DATA, ETHIF_VAR) BufferPtr);

FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SwitchGetCfgDataInfo(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    P2VAR(uint32, ETHIF_APPL_DATA, ETHIF_VAR) DataSizePtr,
    P2VAR(uint32, ETHIF_APPL_DATA, ETHIF_VAR) DataAdressPtr);

FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SwitchPortGetMaxFIFOBufferFillLevel(
    VAR(uint8, ETHIF_VAR) SwitchPortIdx,
    VAR(uint8, ETHIF_VAR) PortIdx,
    VAR(uint8, ETHIF_VAR) SwitchPortEgressFifoIdx,
    P2VAR(uint32, ETHIF_APPL_DATA, ETHIF_VAR) SwitchPortEgressFifoBufferLevelPtr);
#endif /* STD_ON == ETHIF_USE_ETHSWT */

#endif
