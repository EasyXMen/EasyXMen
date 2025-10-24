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
/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author       Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V02.00.00 2020-07-20  bo.yang           Initial Vertion.
 *  V02.00.01 2020-11-10  bo.yang           QAC fix.
 *  V02.00.02 2020-01-08  bo.yang           Unit test fix.
 *  V02.00.03 2020-01-28  bo.yang           add surport for Eth and EthTrcv multi api.
 *  V02.00.04 2020-03-31  bo.yang           Det method change.
 *  V02.00.05 2022-05-17  zhiqiang.huang    Add null pointer check for eth trcv API.
 *  V02.00.06 2022-08-02  zhiqiang.huang    In EthIf_ProvideTxBuffer, check that controlmode is ACTIVE and then provide.
 *  V02.00.07 2023-03-02  fupeng.yu         Add "Result" parameter to the EthIf_TxConfirmation for Autosar 4.4,R19.
 *  V02.00.08 2023-04-07  fupeng.yu         In EthIf_SetControllerMode, Modify logic when CtrlMode is ETH_MODE_DOWN.
 *  V02.00.09 2023-07-31  rongbo.hu         Modify precompile condition in EthIf_GetTransceiverMode,Resolve compile
 error.
 *  V02.00.10 2023-08-02  fupeng.yu         Add ETHIF_ETH_MAIJORVERSION==19u precompile condition for FifoIdx in
 *                                          EthIf_MainFunctionRx. Modify precompile condition in EthIf_TxConfirmation.
 *  V02.00.11 2023-08-28  fupeng.yu         1. In EthIf_SetControllerMode, Resolve Incorrect function return value.
 *                                          2. In EthIf_MainFunctionState, modify the logic notified to SM when the link
 *                                          state of the same transceiver changes.
 *                                          3. Delete EthIf_SearchEthifCrlIndexFmTrcv().
 *  V02.00.12 2023-10-17  fupeng.yu         1. Add link state change handling.
 *                                          2. In EthIf_RxIndication, delete ethernet time sync frame handling.
 *                                          3. Delete EthIf_UpdateTrcvLinkStateToEthSM().
 *  V02.00.13 2023-10-07  tong.zhao         Adaptation of different AutoSar versions of eth driver, currently supporting
 *                                          versions 4.2.2, 4.3.1, 4.4.0, 4.5.0.
 *  V02.00.14 2023-11-14  fupeng.yu         1.In EthIf_CtrlModeIndication, add Invalid value handling for transceiver
 index.
 *  V02.00.15 2023-12-12  fupeng.yu         add switch feature.
 *  V02.00.16 2024-03-13  fupeng.yu         Compatible with Upper Layer defined TxConfirmation callback.
 *  V02.00.17 2024-04-25  fupeng.yu         add the judgment of module initialization fo EthIf_MainFunction<XX>.
 *  V02.00.18 2024-08-06  fupeng.yu         In EthIf_SetCtrlModeDown, update the correct inner state.
 *  V02.00.19 2024-08-13  fupeng.yu         Modify EthIfULTxConfirmationType.
 *  V02.01.00 2025-04-29  jianyu.yang       Developed according to R23-11 specification.
 *  V02.01.01 2025-08-19  jianyu.yang       Fix condiational compilation using CtrlIndexMap.
 *  V02.01.02 2025-09-05  fupeng.yu         Fix compilation error when ETHIF_TRANSCEIVER_TRANSCEIVERS_NUM = 0.

 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:Com<br>
  RuleSource:puhua-rule.rcf 3.0.0

    \li VL_EthIf_TypeBeingCastToDifferentEnumType
      Reason: Use Eth_GetCurrentTime to support EthIf_GetCurrentTimeTuple and EthTrcv_SetTransceiverMode has different
      parameter type in different AUTOSAR version . Risk:Incorrect Type cast . Prevention: Different enumerator have
  same value.

    \li VL_EthIf_SwitchStatementDefaultClauseEmpty
      Reason: There is nothing need to be implement in case default of EthIf_SetControllerMode
      Risk: No risk
      Prevention: On default case function will reture E_NOT_OK.

    \li VL_EthIf_CastingToDifferentObjectPointerType
      Reason: Use Eth_GetCurrentTime to support EthIf_GetCurrentTimeTuple
      Risk: No risk
      Prevention: Two deffirent struct types have same menbers and sequence.

    \li VL_EthIf_FileScopOnlyAccessedInOneFunction
      Reason: Use global variable to controller the interval for polling link change
      Risk: No risk
      Prevention: The variable name is identifiable.

    \li VL_EthIf_TypeOfPointerCouldbePointToConst
      Reason: Keep prototype aligned with SWS
      Risk: No risk
      Prevention:  Variable is not changed by EthIf.

    \li VL_EthIf_prototypesOfFunctionPointerAreDifferent
      Reason: Different modules use different prototype
      Risk: The names of parameters in this function pointer are different from those in another function declarator.
      Prevention: will keep same prototype in later version.

    \li VL_EthIf_MacroDefineSpecifierKeyword
      Reason: Make it easy for code test.
      Risk:  Sometimes it not easy to notice what it really means.
      Prevention: None.

    \li VL_MTR_EthIf_QacSTPAR
      Reason: Keep prototype align with AUTOSAR SWS.
      Risk:  No risk
      Prevention: None

    \li VL_MTR_EthIf_QacSTMIF
      Reason: Special condition to ensure better performance.
      Risk:  Not easy to maintain
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_MTR_EthIf_QacSTVAR
      Reason: Implement all functions in one source file.
      Risk:  Not easy to maintain
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_MTR_EthIf_QacSTCONF
      Reason: To trace AUTOSAR SWS requirements.
      Risk:  Not easy to maintain
      Prevention: None
 */
#ifndef ETHIF_H
#define ETHIF_H

/* =================================================== inclusions =================================================== */

#include "EthIf_Cfg.h"
#include "EthIf_Types.h"
#include "SchM_EthIf.h"
#if (STD_ON == ETHIF_WAKEUP_SUPPORT)
#include "EcuM_Types.h"
#endif
#include "EthSM.h"
#include "ComStack_Types.h"
#include "EthIf_PBcfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */

/* @SWS_EthIf_00006 version number */
#define ETHIF_AR_RELEASE_MAJOR_VERSION    (4u)
#define ETHIF_AR_RELEASE_MINOR_VERSION    (9u)
#define ETHIF_AR_RELEASE_REVISION_VERSION (0u)

#define ETHIF_H_SW_MAJOR_VERSION (2u)
#define ETHIF_H_SW_MINOR_VERSION (0u)
#define ETHIF_H_SW_PATCH_VERSION (14u)

#define ETHIF_MODULE_ID   (65u)
#define ETHIF_INSTANCE_ID (0u)
#define ETHIF_VENDOR_ID   (62u)

/* Error IDs */
#define ETHIF_E_INV_CTRL_IDX       0x01u
#define ETHIF_E_INV_TRCV_IDX       0x02u
#define ETHIF_E_INV_SWT_IDX        0x03u
#define ETHIF_E_INV_PORT_GROUP_IDX 0x04u
#define ETHIF_E_UNINIT             0x05u
#define ETHIF_E_PARAM_POINTER      0x06u
#define ETHIF_E_INV_PARAM          0x07u
#define ETHIF_E_INIT_FAILED        0x08u
#define ETHIF_E_INV_PORT_IDX       0x09u
#define ETHIF_E_INV_CLKUNIT_IDX    (0x0Au)
#define ETHIF_E_INVALID_PARTITION  (0x0Bu)
#define ETHIF_E_PARTITION_MISMATCH (0x0Bu)

/* Service IDs */
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
#define ETHIF_SERVICE_ID_RUN_PORT_CABLE_DIAGNOSTIC                  0x61u
#define ETHIF_SERVICE_ID_GET_PORT_CABLE_DIAGNOSTICS_RESULT          0x60u
#define ETHIF_SERVICE_ID_RUN_CABLE_DIAGNOSTIC                       0x62u
#define ETHIF_SERVICE_ID_SWITCH_GET_CFG_DATA_RAW                    0x63u
#define ETHIF_SERVICE_ID_SWITCH_GET_CFG_DATA_INFO                   0x64u
#define ETHIF_SERVICE_ID_SWITCH_PORT_GET_MAX_FIFO_BUFFER_FILL_LEVEL 0X65u

#define ETHIF_SERVICE_ID_GETSWITCHPORTWAKEUPREASON    0x67u
#define ETHIF_SERVICE_ID_GET_PHY_WAKEUP_REASON        0x69u
#define ETHIF_SERVICE_ID_GET_TRCV_SIGNAL_QUALITY      0x18u
#define ETHIF_SERVICE_ID_CLEAR_TRCV_SIGNAL_QUALITY    0x19u
#define ETHIF_SERVICE_ID_SET_PHY_TEST_MODE            0x17u
#define ETHIF_SERVICE_ID_SET_PHY_LOOPBACK_MODE        0x12u
#define ETHIF_SERVICE_ID_SET_PHY_TX_MODE              0x13u
#define ETHIF_SERVICE_ID_GET_CABLE_DIAGNOSTICS_RESULT 0x78u
#define ETHIF_SERVICE_ID_GET_PHY_IDENTIFIER           0x15u
#define ETHIF_SERVICE_ID_TRANSCEIVER_GET_LINK_STATE   0x4Cu
#define ETHIF_SERVICE_ID_TRANSCEIVER_GET_BAUD_RATE    0x4Eu
#define ETHIF_SERVICE_ID_TRANSCEIVER_GET_DUPLEX_MODE  0x50u
#define ETHIF_SERVICE_ID_TRANSCEIVER_GET_MAC_METHOD   0x66u
#define ETHIF_SERVICE_ID_ETH_GET_SPI_STATUS           0x6Au
#define ETHIF_SERVICE_ID_SWITCH_PORT_MODE_INDICATION  0x46u
#define ETHIF_SERVICE_ID_SLEEP_INDICATION             0x68u

#define ETHIF_SERVICE_ID_GET_CURRENT_TIME_TUPLE 0x95u
#define ETHIF_SERVICE_ID_SET_PHC_TIME           0x96u
#define ETHIF_SERVICE_ID_SET_PHC_CORRECTION     0x97u
#define ETHIF_SERVICE_ID_GET_PHC_TIME           0x98u
#define ETHIF_SERVICE_ID_SET_PPS_SIGNAL_MODE    0x99u

/* Callback notifications function service Ids */
#define ETHIF_SERVICE_ID_RX_INDICATION        0x10u
#define ETHIF_SERVICE_ID_TX_CONFIRMATION      0x11u
#define ETHIF_SERVICE_ID_CTRL_MODE_INDICATION 0x0Eu
#define ETHIF_SERVICE_ID_TRCV_MODE_INDICATION 0x0Fu

/* Scheduled functions service IDs */
#define ETHIF_SERVICE_ID_MAIN_FUNCTION_RX    0x20u
#define ETHIF_SERVICE_ID_MAIN_FUNCTION_TX    0x21u
#define ETHIF_SERVICE_ID_MAIN_FUNCTION_STATE 0x05u

#define ETHIF_NOUSED(paramter) (void)(paramter)

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

/**
 * @brief           Initializes the Ethernet Interface.
 * @param[in]       CfgPtr :Points to the implementation specific structure.
 * @return          None
 * @reentrant       FALSE
 * @synchronous     TRUE
 * @trace           CPD-63576
 */
void EthIf_Init(const EthIf_ConfigType* CfgPtr); /** Init function for ETHIF */

/**
 * @brief           Enables / disables the indexed controller.
 * @param[in]       CtrlIdx :Index of Ethernet Controller.
 * @param[in]       CtrlMode :Mode for Controller.
 * @return          Std_ReturnType
 * @retval          E_OK     : success.
 * @retval          E_NOT_OK : controller mode could not be changed.
 * @reentrant       FALSE.
 * @synchronous     FALSE
 * @trace           CPD-63577
 */
Std_ReturnType EthIf_SetControllerMode(uint8 CtrlIdx, Eth_ModeType CtrlMode);

/**
 * @brief           Enables / disables the indexed controller.
 * @param[in]       CtrlIdx :Index of Ethernet Controller.
 * @param[out]      CtrlModePtr :Mode of Controller.
 * @return          Std_ReturnType
 * @retval          E_OK     : success.
 * @retval          E_NOT_OK : controller could not be initialized.
 * @reentrant       FALSE.
 * @synchronous     TRUE
 * @trace           CPD-63578
 */
Std_ReturnType EthIf_GetControllerMode(uint8 CtrlIdx, Eth_ModeType* CtrlModePtr);

/**
 * @brief           Obtain state of the indexed transceiver.
 * @param[in]       TrcvIdx :Index of the transceiver within the context of the Ethernet Transceiver Driver.
 * @param[out]      TrcvModePtr :ETH_MODE_DOWN: the transceiver is disabled ETH_MODE_ACTIVE: the transceiver is enable.
 * @return          Std_ReturnType
 * @retval          E_OK     : success.
 * @retval          E_NOT_OK : transceiver could not be initialized.
 * @reentrant       FALSE
 * @synchronous     TRUE
 * @trace           CPD-63629
 */
Std_ReturnType EthIf_GetTransceiverMode(uint8 TrcvIdx, Eth_ModeType* TrcvModePtr);

/**
 * @brief         This API request the affected Ethernet hardware to check for a signaled wake-up.
 * @param[in]     WakeupSource : Source device which initiated the wake up event.
 * @reentrant     TRUE
 * @synchronous   FALSE
 * @trace         CPD-63579
 */
Std_ReturnType EthIf_CheckWakeup(EcuM_WakeupSourceType WakeupSource);

/**
 * @brief         obtains the wake up reasons of the indexed Ethernet Transceiver (PHY).
 * @param[in]     TrcvIdx : Index of the transceiver.
 * @param[out]    WakeupReasonPtr : Pointer to structure of least recent wakeup event,which was
 *                                  detected by the Ethernet PHY.
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-63580
 */
Std_ReturnType EthIf_GetPhyWakeupReason(uint8 TrcvIdx, EthTrcv_WakeupReasonType* WakeupReasonPtr);

/**
 * @brief         This function obtains the wake up reasons of the indexed Ethernet switch port.
 * @param[in]     SwitchIdx : Index of the Ethernet switch.
 * @param[in]     SwitchPortIdx : Index of the Ethernet switch port.
 * @param[out]    WakeupReasonPtr : Pointer to structure of least recent wakeup event,which was
 *                                  detected by the Ethernet switch port.
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-63581
 */
Std_ReturnType
    EthIf_GetSwitchPortWakeupReason(uint8 SwitchIdx, uint8 SwitchPortIdx, EthTrcv_WakeupReasonType* WakeupReasonPtr);

/**
 * @brief         Obtains the physical source address used by the indexed controller.
 * @param[in]     CtrlIdx : Index of the Ethernet controller.
 * @param[out]    PhysAddrPtr : Physical source address (MAC address) in network byte order.
 * @reentrant     TRUE
 * @synchronous   FALSE
 * @trace         CPD-63582
 */
void EthIf_GetPhysAddr(uint8 CtrlIdx, uint8* PhysAddrPtr);

/**
 * @brief         Sets the physical source address used by the indexed controller.
 * @param[in]     CtrlIdx : Index of the Ethernet controller.
 * @param[out]    PhysAddrPtr : Pointer to memory containing the physical source address (MAC
 *                              address) in network byte order.
 * @reentrant     Non Reentrant for the same CtrlIdx, reentrant for different
 * @synchronous   TRUE
 * @trace         CPD-63583
 */
void EthIf_SetPhysAddr(uint8 CtrlIdx, const uint8* PhysAddrPtr);

/**
 * @brief         Update the physical source address to/from the indexed controller filter.
 * @param[in]     CtrlIdx : Index of the Ethernet controller.
 * @param[in]    PhysAddrPtr : Pointer to memory containing the physical destination address
                                (MAC address) in network byte order.
 * @param[in]     Action : Add or remove the address from the Ethernet controllers filter.
 * @reentrant     TRUE
 * @synchronous   FALSE
 * @trace         CPD-63584
 */
Std_ReturnType EthIf_UpdatePhysAddrFilter(uint8 CtrlIdx, const uint8* PhysAddrPtr, Eth_FilterActionType Action);

/* @req SWS_EthIf_00154 */
Std_ReturnType EthIf_GetCurrentTime(uint8 CtrlIdx, Eth_TimeStampQualType* timeQualPtr, Eth_TimeStampType* timeStampPtr);

/**
 * @brief           Reads the current time of the timestamp clock and the current time of the PHC in an atomic
 * operation.
 * @param[in]       CtrlIdx :Index of Ethernet Controller.
 * @param[in]       ClkUnitIdx :Index of the Clock Unit.
 * @param[out]      currentTimeTuplePtr :Current time tuple with the: value of the free-running clock used for
 * timestamping and value of the adjustable PHC
 * @return          Std_ReturnType
 * @retval          E_OK     : Current time successfully retrieved.
 * @retval          E_NOT_OK : Current time could not be retrieved.
 * @reentrant       FALSE.
 * @synchronous     TRUE
 * @trace           CPD-63593
 */
Std_ReturnType EthIf_GetCurrentTimeTuple(uint8 CtrlIdx, uint8 ClkUnitIdx, TimeTupleType* currentTimeTuplePtr);

/**
 * @brief           Sets the absolute time of the PHC.
 * @param[in]       CtrlIdx :Index of Ethernet Controller.
 * @param[in]       ClkUnitIdx :Index of the Clock Unit.
 * @param[out]      timeStampPtr :Time value to which the PHC shall be set.
 * @return          Std_ReturnType
 * @retval          E_OK     : Current time successfully retrieved.
 * @retval          E_NOT_OK : Current time could not be retrieved.
 * @reentrant       FALSE.
 * @synchronous     TRUE
 * @trace           CPD-63594
 */
Std_ReturnType EthIf_SetPhcTime(uint8 CtrlIdx, uint8 ClkUnitIdx, const TimeStampType* timeStampPtr);

/**
 * @brief           Sets the absolute time of the PHC.
 * @param[in]       CtrlIdx :Index of Ethernet Controller.
 * @param[in]       ClkUnitIdx :Index of the Clock Unit.
 * @param[in]       rateDeviation :Rate deviation (resolution: 2-41), by which the PHC is requested to be corrected.
 * @param[in]       offset :Time offset, by which the PHC is requested to be updated.
 * @return          Std_ReturnType
 * @retval          E_OK     : Current time successfully retrieved.
 * @retval          E_NOT_OK : Current time could not be retrieved.
 * @reentrant       FALSE.
 * @synchronous     TRUE
 * @trace           CPD-63595
 */
Std_ReturnType EthIf_SetPhcCorrection(uint8 CtrlIdx, uint8 ClkUnitIdx, sint32 rateDeviation, sint32 offset);

/**
 * @brief           Returns the current time value out of the HW registers of the PHC.
 * @param[in]       CtrlIdx :Index of Ethernet Controller.
 * @param[in]       ClkUnitIdx :Index of the Clock Unit.
 * @param[out]      timeQualPtr :quality of HW time stamp, e.g. based on current drift.
 * @param[out]      timeStampPtr :current time stamp.
 * @return          Std_ReturnType
 * @retval          E_OK     : PHC value successfully retrieved.
 * @retval          E_NOT_OK : PHC value could not be retrieved.
 * @reentrant       FALSE.
 * @synchronous     TRUE
 * @trace           CPD-63596
 */
Std_ReturnType
    EthIf_GetPhcTime(uint8 CtrlIdx, uint8 ClkUnitIdx, TimeStampQualType* timeQualPtr, TimeStampType* timeStampPtr);

/**
 * @brief           Enables/disables the generation of a PPS signal.
 * @param[in]       CtrlIdx :Index of Ethernet Controller.
 * @param[in]       ClkUnitIdx :Index of the Clock Unit.
 * @param[out]      signalMode :quality of HW time stamp, e.g. based on current drift.
 * @return          Std_ReturnType
 * @retval          E_OK     : PPS signal generation successfully enabled/disabled.
 * @retval          E_NOT_OK : Failed to enable/disable PPS signal generation.
 * @reentrant       FALSE.
 * @synchronous     TRUE
 * @trace           CPD-63597
 */
Std_ReturnType EthIf_SetPpsSignalMode(uint8 CtrlIdx, uint8 ClkUnitIdx, boolean signalMode);

/**
 * @brief           Activates egress time stamping on a dedicated message object.
 * @param[in]       CtrlIdx :Index of Ethernet Controller.
 * @param[in]       BufIdx :Index of the message buffer, where Application expects egress time stamping.
 * @return          None
 * @reentrant       FALSE.
 * @synchronous     TRUE
 * @trace           CPD-63598
 */
void EthIf_EnableEgressTimeStamp(uint8 CtrlIdx, Eth_BufIdxType BufIdx);

/**
 * @brief           Reads back the egress time stamp on a dedicated message object.
 * @param[in]       CtrlIdx :Index of Ethernet Controller.
 * @param[in]       BufIdx :Index of the message buffer, where the Upper Layer expects egress time stamping.
 * @param[out]      timeQualPtr :quality of HW time stamp, e.g. based on current drift.
 * @param[out]      timeStampPtr :current time stamp.
 * @return          Std_ReturnType
 * @retval          E_OK     : PPS signal generation successfully enabled/disabled.
 * @retval          E_NOT_OK : Failed to enable/disable PPS signal generation.
 * @reentrant       FALSE.
 * @synchronous     TRUE
 * @trace           CPD-63599
 */
Std_ReturnType EthIf_GetEgressTimeStamp(
    uint8                  CtrlIdx,
    Eth_BufIdxType         BufIdx,
    Eth_TimeStampQualType* timeQualPtr,
    Eth_TimeStampType*     timeStampPtr);

/**
 * @brief           Reads back the ingress time stamp on a dedicated message object.
 * @param[in]       CtrlIdx :Index of Ethernet Controller.
 * @param[in]       DataPtr :Pointer to the message buffer, where Application expects ingress time stamping.
 * @param[out]      timeQualPtr :quality of HW time stamp, e.g. based on current drift.
 * @param[out]      timeStampPtr :current time stamp.
 * @return          Std_ReturnType
 * @retval          E_OK     : PPS signal generation successfully enabled/disabled.
 * @retval          E_NOT_OK : Failed to enable/disable PPS signal generation.
 * @reentrant       FALSE.
 * @synchronous     TRUE
 * @trace           CPD-63600
 */
Std_ReturnType EthIf_GetIngressTimeStamp(
    uint8                  CtrlIdx,
    const Eth_DataType*    DataPtr,
    Eth_TimeStampQualType* timeQualPtr,
    Eth_TimeStampType*     timeStampPtr);

/**
 * @brief           Provides access to a transmit buffer of the specified Ethernet controller.
 * @param[in]       CtrlIdx :Index of the transceiver within the context of the Ethernet Interface.
 * @param[out]      FrameType :Ethernet Frame Type (EtherType)
 * @param[out]      Priority :Priority value which shall be used for the 3-bit PCP field of the VLAN tag
 * @param[out]      BufIdxPtr :Index to the granted buffer resource. To be used for subsequent requests.
 * @param[out]      BufPtr :Pointer to the granted buffer
 * @param[inout]    LenBytePtr : in: desired length in bytes, out: granted length in bytes
 * @return          BufReq_ReturnType
 * @retval          BUFREQ_OK: success
 * @retval          BUFREQ_E_NOT_OK: development error detected
 * @retval          BUFREQ_E_BUSY: all buffers in use
 * @retval          BUFREQ_E_OVFL: requested buffer too large
 * @reentrant       TRUE
 * @synchronous     TRUE
 * @trace           CPD-63571
 */
BufReq_ReturnType EthIf_ProvideTxBuffer(
    uint8           CtrlIdx,
    Eth_FrameType   FrameType,
    uint8           Priority,
    Eth_BufIdxType* BufIdxPtr,
    uint8**         BufPtr,
    uint16*         LenBytePtr);

/**
 * @brief           Triggers transmission of a previously filled transmit buffer.
 * @param[in]       CtrlIdx :Index of the transceiver within the context of the Ethernet Interface.
 * @param[in]       BufIdx :Index of the buffer resource
 * @param[in]       FrameType :Ethernet Frame Type (EtherType)
 * @param[in]       TxConfirmation :Activates transmission confirmation.
 * @param[in]       LenByte :Data length in byte.
 * @param[in]       PhysAddrPtr :Physical target address (MAC address) in network byte order
 * @return          Std_ReturnType
 * @retval          E_OK: success
 * @retval          E_NOT_OK: transmission failed
 * @reentrant       Reentrant for different buffer indexes and Ctrl indexes
 * @synchronous     TRUE
 * @trace           CPD-63570
 */
Std_ReturnType EthIf_Transmit(
    uint8          CtrlIdx,
    Eth_BufIdxType BufIdx,
    Eth_FrameType  FrameType,
    boolean        TxConfirmation,
    uint16         LenByte,
    const uint8*   PhysAddrPtr);

#if (ETHIF_VERSION_INFO_API == STD_ON)
/**
 * @brief           Returns the version information of this module.
 * @param[out]      VersionInfoPtr :Version information of this module.
 * @return          None
 * @reentrant       TRUE
 * @synchronous     TRUE
 * @trace           CPD-63627
 */
void EthIf_GetVersionInfo(Std_VersionInfoType* VersionInfoPtr);
#endif

#if (STD_ON == ETHIF_GET_CTRL_IDX_LIST)
/**
 * @brief         Returns the number and index of all active Ethernet controllers..
 * @param[inout] NumberOfCtrlIdx :in: maximum number of controllers in CtrlIdxListPtr, 0 to return
 *                                  the number of controllers but without filling CtrlIdxListPtr. out:
 *                                  number of active controllers.
 * @param[out]    CtrlIdxListPtr : List of active controller indexes.
 * @return        Std_ReturnType
 * @retval        E_OK     : success.
 * @retval        E_NOT_OK : failure.
 * @reentrant     FALSE.
 * @synchronous   TRUE
 * @trace         CPD-63587
 */
Std_ReturnType EthIf_GetCtrlIdxList(uint8* NumberOfCtrlIdx, uint8* CtrlIdxListPtr);
#endif
#if (ETHIF_ENABLE_RX_INTERRUPT == STD_OFF)
/**
 * @brief     The function checks for new received frames and issues reception indications in polling mode..
 * @param[in]     ethIfPartition :Index of the Ethernet partition.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   FALSE
 * @trace         CPD-63707
 */
void EthIf_MainFunctionRx(uint8 ethIfPartition);
#endif

#if (ETHIF_ENABLE_TX_INTERRUPT == STD_OFF)
/**
 * @brief     The function issues transmission confirmations in polling mode. It checks also for transceiver state
 * changes.
 * @param[in]     ethIfPartition :Index of the Ethernet partition.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   FALSE
 * @trace         CPD-63710
 */
void EthIf_MainFunctionTx(uint8 ethIfPartition);
#endif

#if (ETHIF_MAIN_FUNCTION_STATE_PERIOD > 0u)
/**
 * @brief     The function is polling different communication hardware (Ethernet transceiver, Ethernet switch ports)
 *            related information, e.g. link state,signal quality.
 * @param[in]     ethIfPartition :Index of the Ethernet partition.
 * @return        None
 * @reentrant     FALSE
 * @synchronous   FALSE
 * @trace         CPD-63711
 */
void EthIf_MainFunctionState(uint8 ethIfPartition);
#endif

/**
 * @brief         Returns the VLAN identifier of the requested Ethernet controller..
 * @param[in]     CtrlIdx :Index of the Ethernet controller.
 * @param[out]    VlanIdPtr : Pointer to store the VLAN identifier (VID) of the Ethernet controller.
 * @return        Std_ReturnType
 * @retval        E_OK     : success.
 * @retval        E_NOT_OK : failure.
 * @reentrant     FALSE.
 * @synchronous   TRUE
 * @trace         CPD-63588
 */
Std_ReturnType EthIf_GetVlanId(uint8 CtrlIdx, uint16* VlanIdPtr);

/**
 * @brief         Returns the VLAN identifier of the requested Ethernet controller..
 * @param[in]     MeasurementIdx :Data index of measurement data.
 * @param[in]     MeasurementResetNeeded :Flag to trigger a reset of the measurement data.
 * @param[out]    MeasurementDataPtr : Reference to data buffer, where to copy measurement data.
 * @return        Std_ReturnType
 * @retval        E_OK     : success.
 * @retval        E_NOT_OK : failure.
 * @reentrant     TRUE.
 * @synchronous   TRUE
 * @trace         CPD-63589
 */
Std_ReturnType EthIf_GetAndResetMeasurementData(
    EthIf_MeasurementIdxType MeasurementIdx,
    boolean                  MeasurementResetNeeded,
    uint32*                  MeasurementDataPtr);

/**
 * @brief         Obtains the port over which this MAC-address can be reached.
 * @param[in]     MacAddrPtr    : ID of the SDU that is requested to be transmitted.
 * @param[out]    SwtichIdxPtr : Pointer to the switch index.
 * @param[out]    PortIdxPtr : Pointer to the port index.
 * @return        Std_ReturnType
 * @retval        E_OK     : success.
 * @retval        E_NOT_OK : an error occurred, e.g. multiple ports were found.
 * @reentrant     FALSE.
 * @synchronous   TRUE
 * @trace         CPD-63585
 */
Std_ReturnType EthIf_GetPortMacAddr(const uint8* MacAddrPtr, uint8* SwtichIdxPtr, uint8* PortIdxPtr);

/**
 * @brief         Obtains the address resolution table of a switch and copies the list into a user provided buffer.
 * @param[in]     switchIdx : Index of the switch.
 * @param[inout] numberOfElements :In: Maximum number of elements which can be written into the
 *                                  arlTable Out: Number of elements which are currently available in
 *                                  the EthSwitch module.
 * @param[out]    arlTableListPointer : Returns a pointer to the memory where the ARL table of the switch.
 * @return        Std_ReturnType
 * @retval        E_OK     : success.
 * @retval        E_NOT_OK : requested switchIdx is not valid or inactive.
 * @reentrant     FALSE.
 * @synchronous   TRUE
 * @trace         CPD-63586
 */
Std_ReturnType EthIf_GetArlTable(uint8 switchIdx, uint16* numberOfElements, Eth_MacVlanType* arlTableListPointer);

#if (STD_ON == ETHIF_STORE_CONFIGURATION_API)
/**
 * @brief         Trigger the storage/reset of the configuration of the learned MAC/Port tables of a switch in a
 * persistent manner and will be used by e.g. CDD.
 * @param[in]     SwitchIdx :Index of the switch.
 * @return        Std_ReturnType
 * @retval        E_OK     : Storage/Reset request accepted.
 * @retval        E_NOT_OK : Storage/Reset request not accepted.
 * @reentrant     FALSE.
 * @synchronous   TRUE
 * @trace         CPD-63590
 */
Std_ReturnType EthIf_StoreConfiguration(uint8 SwitchIdx);
#endif

#if (STD_ON == ETHIF_RESET_CONFIGURATION_API)
/**
 * @brief        The function shall request to reset the configuration of the learned MAC/Port tables of a Ethernet
 * switch in a persistent manner.
 * @param[in]     SwitchIdx :Index of the switch.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request to persistently reset the MAC/Port table was accepted.
 * @retval        E_NOT_OK : Request to persistently reset the MAC/Port table was not accepted.
 * @reentrant     FALSE.
 * @synchronous   TRUE
 * @trace         CPD-63591
 */
Std_ReturnType EthIf_ResetConfiguration(uint8 SwitchIdx);
#endif

/**
 * @brief         Request a mode for the EthIfSwtPortGroup.
 * @param[in]     PortGroupIdx :Index of the switch.
 * @param[in]     PortMode :port group mode for EthIfSwtPortGroup.
 * @return        Std_ReturnType
 * @retval        E_OK     : success.
 * @retval        E_NOT_OK : group mode could not be changed.
 * @reentrant     FALSE.
 * @synchronous   TRUE
 * @trace         CPD-63601
 */
Std_ReturnType EthIf_SwitchPortGroupRequestMode(EthIf_SwitchPortGroupIdxType PortGroupIdx, Eth_ModeType PortMode);

/**
 * @brief         Request to set all configured and affected EthSwtPorts to ETH_MODE_ACTIVE.
 * @param[in]     SwitchIdx :Index of the switch.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was accepted.
 * @retval        E_NOT_OK : Request was rejected.
 * @reentrant     FALSE.
 * @synchronous   FALSE
 * @trace         CPD-63602
 */
Std_ReturnType EthIf_StartAllPorts(void);

#if (STD_ON == ETHIF_SWITCH_MANAGEMENT_SUPPORT)
/**
 * @brief         Provides additional management information along to an Ethernet frame that requires special treatment
 * within the Switch.
 * @param[in]     CtrlIdx :Index of an Ethernet Interface controller.
 * @param[in]     BufIdx :Ethernet Tx Buffer index.
 * @param[in]     MgmtInfoPtr :Pointer to the management information.
 * @return        Std_ReturnType
 * @retval        E_OK     : Management infos successfully set
 * @retval        E_NOT_OK : Setting of management infos failed.
 * @reentrant     FALSE.
 * @synchronous   TRUE
 * @trace         CPD-63603
 */
Std_ReturnType EthIf_SetSwitchMgmtInfo(uint8 CtrlIdx, Eth_BufIdxType BufIdx, EthSwt_MgmtInfoType* MgmtInfoPtr);
#endif

/**
 * @brief         Request the MgmtObject of the (in this context) unique DataPtr.
 * @param[in]     CtrlIdx :Index of an Ethernet Interface controller.
 * @param[in]     DataPtr :Ethernet data pointer.
 * @param[out]    MgmtObjectPtr :Pointer to the management object.
 * @return        Std_ReturnType
 * @retval        E_OK     : success.
 * @retval        E_NOT_OK : management object could not be obtained.
 * @reentrant     TRUE.
 * @synchronous   TRUE
 * @trace         CPD-63604
 */
Std_ReturnType EthIf_GetRxMgmtObject(uint8 CtrlIdx, Eth_DataType* DataPtr, EthSwt_MgmtObjectType** MgmtObjectPtr);

/**
 * @brief         Request the MgmtObject of the (in this context) unique BufIdx.
 * @param[in]     CtrlIdx :Index of an Ethernet Interface controller.
 * @param[in]     BufIdx :Ethernet Rx Buffer index.
 * @param[out]    MgmtObjectPtr : Pointer to the management object.
 * @return        Std_ReturnType
 * @retval        E_OK     : success.
 * @retval        E_NOT_OK : management object could not be obtained.
 * @reentrant     TRUE.
 * @synchronous   TRUE
 * @trace         CPD-63605
 */
Std_ReturnType EthIf_GetTxMgmtObject(uint8 CtrlIdx, Eth_BufIdxType BufIdx, EthSwt_MgmtObjectType** MgmtObjectPtr);

#if (STD_ON == ETHIF_GLOBAL_TIME_SUPPORT)
/**
 * @brief         Activates egress time stamping on a dedicated message object, addressed by CtrlIdx and BufIdx.
 * @param[in]     CtrlIdx :Index of an Ethernet Interface controller.
 * @param[in]     BufIdx :Index of the message buffer, where Application expects egress time stamping.
 * @param[out]    MgmtInfo :Management information.
 * @return        Std_ReturnType
 * @retval        E_OK     : Time stamping on egress successfully enabled.
 * @retval        E_NOT_OK : Enabling of time stamping on egress has been failed.
 * @reentrant     FALSE.
 * @synchronous   TRUE
 * @trace         CPD-63606
 */
Std_ReturnType EthIf_SwitchEnableTimeStamping(uint8 CtrlIdx, Eth_BufIdxType BufIdx, EthSwt_MgmtInfoType* MgmtInfo);
#endif

#if (STD_ON == ETHIF_VERIFY_CONFIG_API)
/**
 * @brief           Forwarded to EthSwt_VerifyConfig. EthSwt_VerifyConfig verifies the Switch Configuration
                    depending on the HW-Architecture, HW-capability and the intended accuracy of this verification.
 * @param[in]       SwitchIdx :Index of the switch within the context of the Ethernet Interface.
 * @param[out]      Result :Result of verification, TRUE: configureation verified ok, FALSE:configuraton values found
 corrupted
 * @return          Std_ReturnType
 * @retval          E_OK     : Configuration verificaton succeeded.
 * @retval          E_NOT_OK : Configuration verification not succeeded.
 * @reentrant       FALSE.
 * @synchronous     TRUE
 * @trace           CPD-63607
 */
Std_ReturnType EthIf_VerifyConfig(uint8 SwitchIdx, boolean* Result);
#endif

#if (STD_ON == ETHIF_SET_FORWARDING_MODE_API)
/**
 * @brief           Control of switch frame forwarding.
 * @param[in]       SwitchIdx :Index of the switch within the context of the Ethernet Interface.
 * @param[in]       mode :True Forwarding enabled, False Forwarding disabled.
 * @return          Std_ReturnType
 * @retval          E_OK     : request was accepted.
 * @retval          E_NOT_OK : request was rejected.
 * @reentrant       FALSE.
 * @synchronous     TRUE
 * @trace           CPD-63608
 */
Std_ReturnType EthIf_SetForwardingMode(uint8 SwitchIdx, boolean mode);
#endif

#if (ETHIF_ETHTRCV_AUTOSAR_VERSION >= ETHIF_ETHTRCV_AUTOSAR_431)
/**
 * @brief           Retrieves the signal quality of the link of the given Ethernet transceiver.
 * @param[in]       TrcvIdx :Index of the transceiver within the context of the Ethernet Interface.
 * @param[out]      ResultPtr :Pointer to the memory where the signal quality in percent shall be stored..
 * @return          Std_ReturnType
 * @retval          E_OK     : The signal quality retrieved successfully.
 * @retval          E_NOT_OK : The signal quality not retrieved successfully.
 * @reentrant       Reentrant for different TrcvIdx. Non reentrant for the same TrcvIdx.
 * @synchronous     TRUE
 * @trace           CPD-63609
 */
Std_ReturnType EthIf_GetTrcvSignalQuality(uint8 TrcvIdx, EthIf_SignalQualityResultType* ResultPtr);
#endif

/**
 * @brief           Retrieves the signal quality of the link of the given Ethernet switch port.
 * @param[in]       SwitchIdx :Index of the Ethernet switch within the context of the Ethernet Interface.
 * @param[in]       SwitchPortIdx :Index of the Ethernet switch port within the context of the Ethernet Interface.
 * @param[out]      ResultPtr :Pointer to the memory where the signal quality in percent shall be stored..
 * @return          Std_ReturnType
 * @retval          E_OK     : The signal quality retrieved successfully.
 * @retval          E_NOT_OK : The signal quality not retrieved successfully.
 * @reentrant       Reentrant for different TrcvIdx. Non reentrant for the same TrcvIdx.
 * @synchronous     TRUE
 * @trace           CPD-63610
 */
Std_ReturnType
    EthIf_GetSwitchPortSignalQuality(uint8 SwitchIdx, uint8 SwitchPortIdx, EthIf_SignalQualityResultType* ResultPtr);

/**
 * @brief           Clear the stored signal quality of the link of the given Ethernet transceiver.
 * @param[in]       TrcvIdx :Index of the transceiver within the context of the Ethernet Interface.
 * @return          Std_ReturnType
 * @retval          E_OK     : The signal quality cleared successfully.
 * @retval          E_NOT_OK : The signal quality cleared not successfully.
 * @reentrant       Reentrant for different TrcvIdx. Non reentrant for the same TrcvIdx.
 * @synchronous     TRUE
 * @trace           CPD-63611
 */
Std_ReturnType EthIf_ClearTrcvSignalQuality(uint8 TrcvIdx);

/**
 * @brief           Clear the stored signal quality of the link of the given Ethernet switch port.
 * @param[in]       SwitchIdx :Index of the Ethernet switch within the context of the Ethernet Interface.
 * @param[in]       SwitchPortIdx :Index of the Ethernet switch port within the context of the Ethernet Interface.
 * @return          Std_ReturnType
 * @retval          E_OK     : The signal quality cleared successfully.
 * @retval          E_NOT_OK : The signal quality cleared not successfully.
 * @reentrant       Reentrant for different Ethernet switch indexes and Ethernet Switch port indexes. Non reentrant for
 * the same SwitchPortIdx.
 * @synchronous     TRUE
 * @trace           CPD-63612
 */
Std_ReturnType EthIf_ClearSwitchPortSignalQuality(uint8 SwitchIdx, uint8 SwitchPortIdx);

/**
 * @brief           Activates a given test mode.
 * @param[in]       TrcvIdx :Index of the transceiver within the context of the Ethernet Interface.
 * @param[in]       Mode :Test mode to be activated.
 * @return          Std_ReturnType
 * @retval          E_OK     : The request has been accepted.
 * @retval          E_NOT_OK : The request has not been accepted.
 * @reentrant       Reentrant for different TrcvIdx. Non reentrant for the same TrcvIdx.
 * @synchronous     TRUE
 * @trace           CPD-63613
 */
Std_ReturnType EthIf_SetPhyTestMode(uint8 TrcvIdx, EthTrcv_PhyTestModeType Mode);

/**
 * @brief           Activates a given loopback mode.
 * @param[in]       TrcvIdx :Index of the transceiver within the context of the Ethernet Interface.
 * @param[in]       Mode :Loopback mode to be activated.
 * @return          Std_ReturnType
 * @retval          E_OK     : The request has been accepted.
 * @retval          E_NOT_OK : The request has not been accepted.
 * @reentrant       Reentrant for different TrcvIdx. Non reentrant for the same TrcvIdx.
 * @synchronous     TRUE
 * @trace           CPD-63614
 */
Std_ReturnType EthIf_SetPhyLoopbackMode(uint8 TrcvIdx, EthTrcv_PhyLoopbackModeType Mode);

/**
 * @brief           Activates a given transmission mode.
 * @param[in]       TrcvIdx :Index of the transceiver within the context of the Ethernet Interface.
 * @param[in]       Mode :Transmission mode to be activated.
 * @return          Std_ReturnType
 * @retval          E_OK     : The request has been accepted.
 * @retval          E_NOT_OK : The request has not been accepted.
 * @reentrant       Reentrant for different TrcvIdx. Non reentrant for the same TrcvIdx.
 * @synchronous     TRUE
 * @trace           CPD-63615
 */
Std_ReturnType EthIf_SetPhyTxMode(uint8 TrcvIdx, EthTrcv_PhyTxModeType Mode);

/**
 * @brief           Retrieves the cable diagnostics result of a given transceiver.
 * @param[in]       TrcvIdx :Index of the transceiver within the context of the Ethernet Interface.
 * @param[out]      ResultPtr :Pointer to the location where the cable diagnostic result shall be stored.
 * @return          Std_ReturnType
 * @retval          E_OK     : The request has been accepted.
 * @retval          E_NOT_OK : The request has not been accepted.
 * @reentrant       Reentrant for different TrcvIdx. Non reentrant for the same TrcvIdx.
 * @synchronous     TRUE
 * @trace           CPD-63616
 */
Std_ReturnType EthIf_GetCableDiagnosticsResult(uint8 TrcvIdx, EthTrcv_CableDiagResultType* ResultPtr);

/**
 * @brief           Obtains the PHY identifier of the Ethernet Interface according to IEEE 802.3-2015.
 * @param[in]       TrcvIdx :Index of the transceiver within the context of the Ethernet Interface.
 * @param[out]      OrgUniqueIdPtr :Pointer to the memory where the Organizationally Unique Identifier shall be stored.
 * @param[out]      ModelNrPtr :Pointer to the memory where the Manufacturer Model Number shall be stored.
 * @param[out]      RevisionNrPtr :Pointer to the memory where the Revision Number shall be stored.
 * @return          Std_ReturnType
 * @retval          E_OK     : The request has been accepted.
 * @retval          E_NOT_OK : The request has not been accepted.
 * @reentrant       Reentrant for different TrcvIdx. Non reentrant for the same TrcvIdx.
 * @synchronous     TRUE
 * @trace           CPD-63617
 */
Std_ReturnType EthIf_GetPhyIdentifier(uint8 TrcvIdx, uint32* OrgUniqueIdPtr, uint8* ModelNrPtr, uint8* RevisionNrPtr);

/**
 * @brief           Obtains the mode of the indexed switch port.
 * @param[in]       SwitchIdx :Index of the switch within the context of the Ethernet Switch Driver.
 * @param[in]       SwitchPortIdx :Index of the port at the addressed switch.
 * @param[out]      PortModePtr :ETH_MODE_DOWN: The Ethernet switch port of the given Ethernet switch is disabled
 *                               ETH_MODE_ACTIVE: The Ethernet switch port of the given Ethernet switch is enabled
 * @return          Std_ReturnType
 * @retval          E_OK     : success.
 * @retval          E_NOT_OK : The mode of the indexed switch port could not be obtained, or the function is called in
 * state ETHSWT_STATE_UNINIT or ETHSWT_STATE_INIT..
 * @reentrant       FALSE
 * @synchronous     TRUE
 * @trace           CPD-63628
 */
Std_ReturnType EthIf_GetSwitchPortMode(uint8 SwitchIdx, uint8 SwitchPortIdx, Eth_ModeType* PortModePtr);

/**
 * @brief           Obtains the link state of the indexed switch port.
 * @param[in]       SwitchIdx :Index of the switch within the context of the Ethernet Switch Driver.
 * @param[in]       SwitchPortIdx :Index of the port at the addressed switch.
 * @param[in]       LinkStatePtr :ETHTRCV_LINK_STATE_DOWN: Switch port is disconnected
 *                                ETHTRCV_LINK_STATE_ACTIVE: Switch port is connected
 * @return          Std_ReturnType
 * @retval          E_OK     : success.
 * @retval          E_NOT_OK : Link state of the indexed switch port could not be obtained, or the function is called in
 state ETHSWT_STATE_UNINIT or ETHSWT_STATE_INIT.
 * @reentrant       FALSE
 * @synchronous     TRUE
 * @trace           CPD-63630
 */
Std_ReturnType EthIf_SwitchPortGetLinkState(uint8 SwitchIdx, uint8 SwitchPortIdx, EthTrcv_LinkStateType* LinkStatePtr);

/**
 * @brief           Obtains the link state of the indexed transceiver.
 * @param[in]       TrcvIdx :Index of the transceiver within the context of the Ethernet Transceiver Driver.
 * @param[out]      LinkStatePtr :ETHTRCV_LINK_STATE_DOWN: transceiver is disconnected
 *                                ETHTRCV_LINK_STATE_ACTIVE: transceiver is connected
 * @return          Std_ReturnType
 * @retval          E_OK     : success.
 * @retval          E_NOT_OK : transceiver could not be initialized.
 * @reentrant       FALSE
 * @synchronous     TRUE
 * @trace           CPD-63631
 */
Std_ReturnType EthIf_TransceiverGetLinkState(uint8 TrcvIdx, EthTrcv_LinkStateType* LinkStatePtr);

/**
 * @brief           Obtains the baud rate of the indexed switch port.
 * @param[in]       SwitchIdx :Index of the switch within the context of the Ethernet Switch Driver.
 * @param[in]       SwitchPortIdx :Index of the port at the addressed switch.
 * @param[out]      BaudRatePtr :ETHTRCV_BAUD_RATE_10MBIT: 10MBit connection
 *                               ETHTRCV_BAUD_RATE_100MBIT: 100MBit connection
 *                               ETHTRCV_BAUD_RATE_1000MBIT: 1000MBit connection
 *                               ETHTRCV_BAUD_RATE_2500MBIT: 2500MBit connection
 * @return          Std_ReturnType
 * @retval          E_OK     : success.
 * @retval          E_NOT_OK : Baud rate of the indexed switch port could not be obtained, or the function is called in
 * state ETHSWT_STATE_UNINIT or ETHSWT_STATE_INIT
 * @reentrant       FALSE
 * @synchronous     TRUE
 * @trace           CPD-63632
 */
Std_ReturnType EthIf_SwitchPortGetBaudRate(uint8 SwitchIdx, uint8 SwitchPortIdx, EthTrcv_BaudRateType* BaudRatePtr);

/**
 * @brief           Obtains the baud rate of the indexed transceiver.
 * @param[in]       TrcvIdx :Index of the transceiver within the context of the Ethernet Transceiver Driver.
 * @param[out]      BaudRatePtr :ETHTRCV_BAUD_RATE_10MBIT: 10MBit connection
 *                               ETHTRCV_BAUD_RATE_100MBIT: 100MBit connection
 *                               ETHTRCV_BAUD_RATE_1000MBIT: 1000MBit connection
 *                               ETHTRCV_BAUD_RATE_2500MBIT: 2500MBit connection
 * @return          Std_ReturnType
 * @retval          E_OK     : success.
 * @retval          E_NOT_OK : transceiver could not be initialized.
 * @reentrant       FALSE
 * @synchronous     TRUE
 * @trace           CPD-63633
 */
Std_ReturnType EthIf_TransceiverGetBaudRate(uint8 TrcvIdx, EthTrcv_BaudRateType* BaudRatePtr);

/**
 * @brief           Obtains the duplex mode of the indexed switch port.
 * @param[in]       SwitchIdx :Index of the switch within the context of the Ethernet Switch Driver.
 * @param[in]       SwitchPortIdx :Index of the port at the addressed switch.
 * @param[out]      DuplexModePtr :ETHTRCV_DUPLEX_MODE_HALF: half duplex connections
 *                                 ETHTRCV_DUPLEXMODE_FULL: full duplex connection
 * @return          Std_ReturnType
 * @retval          E_OK     : success.
 * @retval          E_NOT_OK : duplex mode of the indexed switch port could not be obtained, or the function is called
 * in state ETHSWT_STATE_UNINIT or ETHSWT_STATE_INIT.
 * @reentrant       FALSE
 * @synchronous     TRUE
 * @trace           CPD-63634
 */
Std_ReturnType
    EthIf_SwitchPortGetDuplexMode(uint8 SwitchIdx, uint8 SwitchPortIdx, EthTrcv_DuplexModeType* DuplexModePtr);

/**
 * @brief           Obtains the duplex mode of the indexed transceiver.
 * @param[in]       TrcvIdx :Index of the transceiver within the context of the Ethernet Transceiver Driver.
 * @param[out]      DuplexModePtr :ETHTRCV_DUPLEX_MODE_HALF: half duplex connections
 *                                 ETHTRCV_DUPLEXMODE_FULL: full duplex connection
 * @return          Std_ReturnType
 * @retval          E_OK     : success.
 * @retval          E_NOT_OK : transceiver could not be initialized
 * @reentrant       FALSE
 * @synchronous     TRUE
 * @trace           CPD-63635
 */
Std_ReturnType EthIf_TransceiverGetDuplexMode(uint8 TrcvIdx, EthTrcv_DuplexModeType* DuplexModePtr);

/**
 * @brief           Reads a list with drop counter values of the corresponding port of the switch.
 * @param[in]       SwitchIdx :Index of the switch within the context of the Ethernet Switch Driver.
 * @param[in]       SwitchPortIdx :Index of the port at the addressed switch.
 * @param[out]      CounterPtr :counter values according to IETF RFC 1757, RFC 1643 and RFC 2233
 * @return          Std_ReturnType
 * @retval          E_OK     : success.
 * @retval          E_NOT_OK : counter values read failure
 * @reentrant       FALSE
 * @synchronous     TRUE
 * @trace           CPD-63636
 */
Std_ReturnType EthIf_SwitchPortGetCounterValues(uint8 SwitchIdx, uint8 SwitchPortIdx, Eth_CounterType* CounterPtr);

/**
 * @brief           Returns a list of statistic counters defined with Eth_RxTatsType. The majority of these Counters are
 * derived from the IETF RFC2819.
 * @param[in]       SwitchIdx :Index of the switch within the context of the Ethernet Switch Driver.
 * @param[in]       SwitchPortIdx :Index of the port at the addressed switch.
 * @param[out]      RxStatsPtr :List of values according to IETF RFC 2819 (Remote Network Monitoring Management
 * Information Base)
 * @return          Std_ReturnType
 * @retval          E_OK     : success.
 * @retval          E_NOT_OK : drop counter could not be obtained
 * @reentrant       FALSE
 * @synchronous     TRUE
 * @trace           CPD-63637
 */
Std_ReturnType EthIf_SwitchPortGetRxStats(uint8 SwitchIdx, uint8 SwitchPortIdx, Eth_RxStatsType* RxStatsPtr);

/**
 * @brief           List of values to read statistic values for transmission.
 * @param[in]       SwitchIdx :Index of the switch within the context of the Ethernet Switch Driver.
 * @param[in]       SwitchPortIdx :Index of the port at the addressed switch.
 * @param[out]      TxStatsPtr :List of values to read statistic values for transmission.
 * @return          Std_ReturnType
 * @retval          E_OK     : success.
 * @retval          E_NOT_OK : Tx-statistics could not be obtained
 * @reentrant       FALSE
 * @synchronous     TRUE
 * @trace           CPD-63638
 */
Std_ReturnType EthIf_SwitchPortGetTxStats(uint8 SwitchIdx, uint8 SwitchPortIdx, Eth_TxStatsType* TxStatsPtr);

/**
 * @brief           List of values to read statistic error counter values for transmission from.
 * @param[in]       SwitchIdx :Index of the switch within the context of the Ethernet Switch Driver.
 * @param[in]       SwitchPortIdx :Index of the port at the addressed switch.
 * @param[out]      TxStatsPtr :List of values to read statistic error counter values for transmission.
 * @return          Std_ReturnType
 * @retval          E_OK     : success.
 * @retval          E_NOT_OK : Tx-statistics could not be obtained
 * @reentrant       FALSE
 * @synchronous     TRUE
 * @trace           CPD-63639
 */
Std_ReturnType EthIf_SwitchPortGetTxErrorCounterValues(
    uint8                         SwitchIdx,
    uint8                         SwitchPortIdx,
    Eth_TxErrorCounterValuesType* TxStatsPtr);

/**
 * @brief           Returns the MAC learning mode.
 * @param[in]       SwitchIdx :Index of the switch within the context of the Ethernet Switch Driver.
 * @param[in]       SwitchPortIdx :Index of the port at the addressed switch.
 * @param[out]      MacLearningModePtr :Defines whether MAC addresses shall be learned and if they shall be learned in
 * software or hardware.
 * @return          Std_ReturnType
 * @retval          E_OK     : success.
 * @retval          E_NOT_OK : configuration could be persistently reset
 * @reentrant       FALSE
 * @synchronous     TRUE
 * @trace           CPD-63640
 */
Std_ReturnType EthIf_SwitchPortGetMacLearningMode(
    uint8                   SwitchIdx,
    uint8                   SwitchPortIdx,
    EthSwt_MacLearningType* MacLearningModePtr);

/**
 * @brief           Returns the MAC learning mode.
 * @param[in]       SwitchIdx :Index of the switch within the context of the Ethernet Switch Driver.
 * @param[in]       SwitchPortIdx :Index of the port at the addressed switch.
 * @param[out]      OrgUniqueIdPtr :Pointer to the memory where the Organizationally Unique Identifier (OUI) shall be
 * stored.
 * @param[out]      ModelNrPtr :Pointer to the memory where the Manufacturers Model Number shall be stored.
 * @param[out]      RevisionNrPtr :Pointer to the memory where the Revision Number shall be stored.
 * @return          Std_ReturnType
 * @retval          E_OK     : organizationally unique identifier of the Ethernet transceiver could be read.
 * @retval          E_NOT_OK : organizationally unique identifier of the Ethernet transceiver could not be obtained
 * (i.e. OUI is not available).
 * @reentrant       FALSE
 * @synchronous     TRUE
 * @trace           CPD-63641
 */
Std_ReturnType EthIf_GetSwitchPortIdentifier(
    uint8   SwitchIdx,
    uint8   SwitchPortIdx,
    uint32* OrgUniqueIdPtr,
    uint8*  ModelNrPtr,
    uint8*  RevisionNrPtr);

/**
 * @brief            Obtain the Organizationally Unique Identifier that is given by the IEEE of the indexed Ethernet
 * switch.
 * @param[in]       SwitchIdx :Index of the switch within the context of the Ethernet Switch Driver.
 * @param[out]      OrgUniqueIdPtr :Pointer to the memory where the Organizationally Unique Identifier (OUI) shall be
 * stored.
 * @return          Std_ReturnType
 * @retval          E_OK     : organizationally unique identifier of the Ethernet switch could be read..
 * @retval          E_NOT_OK : organizationally unique identifier of the Ethernet switch could not be read (i.e. no OUI
 * is available for this Ethernet switch).
 * @reentrant       FALSE
 * @synchronous     TRUE
 * @trace           CPD-63642
 */
Std_ReturnType EthIf_GetSwitchIdentifier(uint8 SwitchIdx, uint32* OrgUniqueIdPtr);

/**
 * @brief           Store the given port mirror configuration in a shadow buffer in the Ethernet switch driver for the
 * given MirroredSwitchIdx.
 * @param[in]       MirroredSwitchIdx :Index of the switch within the context of the Ethernet Switch Driver, where the
 * Ethernet switch port is located, that has to be mirrored.
 * @param[out]      PortMirrorConfigurationPtr :Pointer to the memory where the port configuration shall be used.
 * @return          Std_ReturnType
 * @retval          E_OK     : the port mirror configuration for the indexed Ethernet switch port was written.
 * @retval          E_NOT_OK : the port mirror configuration for the indexed Ethernet switch port was not written. (i.e.
 * indexed ethernet switch is not available).
 * @reentrant       FALSE
 * @synchronous     TRUE
 * @trace           CPD-63643
 */
Std_ReturnType EthIf_WritePortMirrorConfiguration(
    uint8                           MirroredSwitchIdx,
    const EthSwt_PortMirrorCfgType* PortMirrorConfigurationPtr);

/**
 * @brief           Obtain the port mirror configuration of the given Ethernet switch.
 * @param[in]       MirroredSwitchIdx :Index of the switch within the context of the Ethernet Switch Driver, where the
 * Ethernet switch port is located, that has to be mirrored.
 * @param[out]      PortMirrorConfigurationPtr :Pointer to the memory where the port configuration shall be stored.
 * @return          Std_ReturnType
 * @retval          E_OK     : the port mirror configuration for the indexed Ethernet switch port was red successfully.
 * @retval          E_NOT_OK : the port mirror configuration for the indexed Ethernet switch was not red successfully.
 * (i.e. indexed Ethernet switch is not available.
 * @reentrant       FALSE
 * @synchronous     TRUE
 * @trace           CPD-63644
 */
Std_ReturnType
    EthIf_ReadPortMirrorConfiguration(uint8 MirroredSwitchIdx, EthSwt_PortMirrorCfgType* PortMirrorConfigurationPtr);

/**
 * @brief           Delete the stored port mirror configuration of the given MirroredSwitchIdx.
 * @param[in]       MirroredSwitchIdx :Index of the switch within the context of the Ethernet Switch Driver.
 * @return          Std_ReturnType
 * @retval          E_OK     : Port mirror configuration was deleted successfully.
 * @retval          E_NOT_OK : Port mirror configuration was not deleted successfully. (e.g. the port mirroring is
 * enabled)
 * @reentrant       Reentrant Reentrant for different MirroredSwitchIdx. Non reentrant for the same SwitchIdx.
 * @synchronous     TRUE
 * @trace           CPD-63645
 */
Std_ReturnType EthIf_DeletePortMirrorConfiguration(uint8 MirroredSwitchIdx);

/**
 * @brief           Obtain the current status of the port mirroring for the indexed Ethernet switch port.
 * @param[in]       SwitchIdx :Index of the switch within the context of the Ethernet Switch Driver.
 * @param[in]       PortIdx :Index of the port at the addressed switch.
 * @param[out]      PortMirrorStatePtr :Pointer to the memory where the port mirroring state (either
 * PORT_MIRRORING_ENABLED or PORT_MIRRORING_ DISABLED)of the given Ethernet switch port shall be stored.
 * @return          Std_ReturnType
 * @retval          E_OK     : the port mirroring state for the indexed Ethernet switch port returned successfully..
 * @retval          E_NOT_OK : the port mirror configuration for the indexed Ethernet switch returned not successfully.
 * (i.e. indexed ethernet switch port is not available)
 * @reentrant       FALSE
 * @synchronous     TRUE
 * @trace           CPD-63646
 */
Std_ReturnType EthIf_GetPortMirrorState(uint8 SwitchIdx, uint8 PortIdx, EthSwt_PortMirrorStateType* PortMirrorStatePtr);

/**
 * @brief           Request to set the given port mirroring state of the port mirror configuration for the given
 * Ethernet switch.
 * @param[in]       SwitchIdx :Index of the switch within the context of the Ethernet Switch Driver.
 * @param[in]       PortIdx :Index of the port at the addressed switch.
 * @param[out]      PortMirrorState :Contain the requested port mirroring state either PORT_MIRRORING_ENABLED or
 * PORT_MIRRORING_DISABLED.
 * @return          Std_ReturnType
 * @retval          E_OK     : the requested port mirroring state for the indexed Ethernet switch port was set
 * successfully.
 * @retval          E_NOT_OK : the requested port mirroring state for the indexed Ethernet switch was not set
 * successfully.
 * @reentrant       FALSE
 * @synchronous     TRUE
 * @trace           CPD-63647
 */
Std_ReturnType
    EthIf_SetPortMirrorState(uint8 MirroredSwitchIdx, uint8 PortIdx, EthSwt_PortMirrorStateType PortMirrorState);

/**
 * @brief           Activates a given test mode of the indexed Ethernet switch port.
 * @param[in]       SwitchIdx :Index of the switch within the context of the Ethernet Switch Driver.
 * @param[in]       PortIdx :Index of the port at the addressed switch.
 * @param[out]      Mode :Test mode to be activated.
 * @return          Std_ReturnType
 * @retval          E_OK     : the port test mode for the indexed Ethernet switch port was set successfully.
 * @retval          E_NOT_OK : the port test mode for the indexed Ethernet switch was not set successfully.
 * @reentrant       FALSE
 * @synchronous     TRUE
 * @trace           CPD-63648
 */
Std_ReturnType EthIf_SetPortTestMode(uint8 SwitchIdx, uint8 PortIdx, EthTrcv_PhyTestModeType Mode);

/**
 * @brief           Activates a given test loop-back mode of the indexed Ethernet switch port..
 * @param[in]       SwitchIdx :Index of the switch within the context of the Ethernet Switch Driver.
 * @param[in]       PortIdx :Index of the port at the addressed switch.
 * @param[out]      Mode :Loop-back mode to be activated.
 * @return          Std_ReturnType
 * @retval          E_OK     : the port mirroring loop-back back mode for the indexed Ethernet switch port was activated
 * successfully.
 * @retval          E_NOT_OK : the port mirroring loop-back back mode for the indexed Ethernet switch port was not
 * activated successfully.
 * @reentrant       FALSE
 * @synchronous     TRUE
 * @trace           CPD-63649
 */
Std_ReturnType EthIf_SetPortLoopbackMode(uint8 SwitchIdx, uint8 PortIdx, EthTrcv_PhyLoopbackModeType Mode);

/**
 * @brief           Activates a given transmission mode of the indexed Ethernet switch port.
 * @param[in]       SwitchIdx :Index of the switch within the context of the Ethernet Switch Driver.
 * @param[in]       PortIdx :Index of the port at the addressed switch.
 * @param[out]      Mode :Transmission mode to be activated.
 * @return          Std_ReturnType
 * @retval          E_OK     : the port Tx mode for the indexed Ethernet switch port was activated successfully.
 * @retval          E_NOT_OK : the port Tx mode for the indexed Ethernet switch port was not activated successfully.
 * @reentrant       FALSE
 * @synchronous     TRUE
 * @trace           CPD-63650
 */
Std_ReturnType EthIf_SetPortTxMode(uint8 SwitchIdx, uint8 PortIdx, EthTrcv_PhyTxModeType Mode);

/**
 * @brief            Retrieves the cable diagnostics result of the indexed Ethernet switch port respectively the
 *                   referenced Ethernet Transceiver Driver.
 * @param[in]       SwitchIdx :Index of the switch within the context of the Ethernet Switch Driver.
 * @param[in]       PortIdx :Index of the port at the addressed switch.
 * @param[out]      ResultPtr :Pointer to the location where the cable diagnostics result shall be stored.
 * @return          Std_ReturnType
 * @retval          E_OK     : the port cable diagnostic result for the indexed Ethernet switch port was obtained
 * successfully.
 * @retval          E_NOT_OK : the port cable diagnostic result for the indexed Ethernet switch port was not obtained
 * successfully.
 * @reentrant       FALSE
 * @synchronous     TRUE
 * @trace           CPD-63651
 */
Std_ReturnType
    EthIf_GetPortCableDiagnosticsResult(uint8 SwitchIdx, uint8 PortIdx, EthTrcv_CableDiagResultType* ResultPtr);

/**
 * @brief            Trigger the cable diagnostics of the given Ethernet Switch port (PortIdx) by calling EthTrcv_Run
 *                    CableDiagnostic of the referenced Ethernet transceiver.
 * @param[in]       SwitchIdx :Index of the switch within the context of the Ethernet Switch Driver.
 * @param[in]       PortIdx :Index of the port at the addressed switch.
 * @return          Std_ReturnType
 * @retval          E_OK     : The trigger to run the cable diagnostic has been accepted.
 * @retval          E_NOT_OK : The trigger to run the cable diagnostic has not been accepted.
 * @reentrant       Reentrant Reentrant for different SwitchIdx and PortIdx. Non reentrant for the same SwitchIdx and
 * PortIdx.
 * @synchronous     TRUE
 * @trace           CPD-63652
 */
Std_ReturnType EthIf_RunPortCableDiagnostic(uint8 SwitchIdx, uint8 PortIdx);

/**
 * @brief           Trigger the cable diagnostics for the given Ethernet transceiver..
 * @param[in]       TrcvIdx :Index of the switch within the context of the Ethernet Switch Driver.
 * @return          Std_ReturnType
 * @retval          E_OK     : The trigger has been accepted..
 * @retval          E_NOT_OK : The trigger has not been accepted.
 * @reentrant       Reentrant Reentrant for different TrcvIdx. Non reentrant for the same TrcvIdx..
 * @synchronous     TRUE
 * @trace           CPD-63653
 */
Std_ReturnType EthIf_RunCableDiagnostic(uint8 TrcvIdx);

/**
 * @brief           Retrieves the data in memory of the indexed Ethernet switch in variable length.
 * @param[in]       SwitchIdx :Index of the Ethernet switch within the context of the Ethernet Switch Driver.
 * @param[in]       Offset :Offset of the Ethernet switch memory from where the reading starts.
 * @param[in]       Length :Length of data in bytes that shall be copied.
 * @param[out]       BufferPtr :Pointer to the location where the data shall be copied.
 * @return          Std_ReturnType
 * @retval          E_OK     : The trigger has been accepted..
 * @retval          E_NOT_OK : The trigger has not been accepted.
 * @reentrant       FALSE
 * @synchronous     FALSE
 * @trace           CPD-63654
 */
Std_ReturnType EthIf_SwitchGetCfgDataRaw(uint8 SwitchIdx, uint32 Offset, uint16 Length, uint8* BufferPtr);

/**
 * @brief           Retrieves the data in memory of the indexed Ethernet switch in variable length.
 * @param[in]       SwitchIdx :Index of the Ethernet switch within the context of the Ethernet Switch Driver.
 * @param[out]      DataSizePtr :Pointer to the location where the total size of the configuration data shall be copied.
 * @param[out]      DataAdressPtr :Pointer to the location where the start address of the configuration registers shall
 * be copied.
 * @return          Std_ReturnType
 * @retval          E_OK     : the data was obtained successfully.
 * @retval          E_NOT_OK : the data was not obtained successfully. (i.e. indexedEthernet switch is not available).
 * @reentrant       TRUE
 * @synchronous     FALSE
 * @trace           CPD-63655
 */
Std_ReturnType EthIf_SwitchGetCfgDataInfo(uint8 SwitchIdx, uint32* DataSizePtr, uint32* DataAdressPtr);

/**
 * @brief           The function retrieves the maximum amount of allocated FIFO buffer of the indexed Ethernet switch
 *                    egress port. If the Ethernet switch hardware does not support Ethernet switch port based maximal
 *                    FIFO buffer level, the content of SwitchPortEgressFifoBufferLevelPtr shall be set to 0xFFFFFFFF.
 *                    This API may be called by e.g. a CDD.
 * @param[in]       SwitchIdx :Index of the switch within the context of the Ethernet Switch Driver.
 * @param[in]       PortIdx :Index of the port at the addressed switch.
 * @param[out]      SwitchPortEgressFifoIdx :Index of the egress FIFO of the addressed Ethernet switch port.
 * @param[out]      SwitchPortEgressFifoBufferLevelPtr :Pointer to a memory location, where the maximum amount of
 *                                                      allocated FIFO buffer (in bytes) since the last read out shall
 * be stored.
 * @return          Std_ReturnType
 * @retval          E_OK     : success.
 * @retval          E_NOT_OK : The maximal FIFO buffer level could not be obtained.
 * @reentrant       Reentrant Reentrant for different SwitchIdx and PortIdx. Non reentrant for the same SwitchIdx and
 * PortIdx.
 * @synchronous     FALSE
 * @trace           CPD-63655
 */
Std_ReturnType EthIf_SwitchPortGetMaxFIFOBufferFillLevel(
    uint8   SwitchPortIdx,
    uint8   PortIdx,
    uint8   SwitchPortEgressFifoIdx,
    uint32* SwitchPortEgressFifoBufferLevelPtr);

#if (ETHIF_ETHTRCV_AUTOSAR_VERSION >= ETHIF_ETHTRCV_AUTOSAR_R20_11)
/**
 * @brief           Obtains the media access mode of the transceiver..
 * @param[in]       TrcvIdx :Index of the transceiver within the context of the Ethernet Transceiver Driver.
 * @param[out]      MacModePtr :ETHTRCV_MAC_TYPE_CSMA_CD: Carrier-sense multiple access with collicion detection.
 *                              ETHTRCV_MAC_TYPE_PLCA: Physical layer collision avoidance.
 * @return          Std_ReturnType
 * @retval          E_OK     : success..
 * @retval          E_NOT_OK : transceiver request has not been accepted.
 * @reentrant       FALSE
 * @synchronous     TRUE
 * @trace           CPD-63657
 */

Std_ReturnType EthIf_TransceiverGetMacMethod(uint8 TrcvIdx, EthTrcv_MacMethodType* MacModePtr);
#endif

#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_R23_11)
/**
 * @brief           When MACPHY controller are used, obtains the SPI interface status..
 * @param[in]       CtrlIdx :Index of the controller within the context of the Ethernet controller Driver.
 * @param[out]      SpiStatusPtr :Status of the SPI interface.
 * @return          Std_ReturnType
 * @retval          E_OK     : success.
 * @retval          E_NOT_OK : Controller request has not been accepted..
 * @reentrant       FALSE
 * @synchronous     TRUE
 * @trace           CPD-63658
 */
Std_ReturnType EthIf_EthGetSpiStatus(uint8 CtrlIdx, Eth_SpiStatusType* SpiStatusPtr);
#endif

#ifdef __cplusplus
}
#endif

#endif
