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
 **  @file               : EthTSyn.c
 **  @author             : yuzhe.zhang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Implementation header for EthTSyn
 **
 ***********************************************************************************************************************/

/** ===================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date       Author         Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V2.1.0    2025-01-01 shuangyang.fu
 *      1. Support multi-partition distribution.
 *
 ==================================================================================================================== */
/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:EthTSyn<br>
  RuleSorce:puhua_rules-2024.12.rcf

    \li VL_EthTSyn_5016
      Reason:Mutable Object is returned to an untrusted caller.
      Risk:caller might modify the returned object.
      Prevention:Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_MTR_EthTSyn_STM19
      Reason:While a single exit point per function is generally mandated by coding standards, strategically
      allowing multiple returns in specific scenarios can enhance performance.
      Risk:may increase the difficulty of debugging.
      Prevention:Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_MTR_EthTSyn_STVAR
      Reason:While the total variable count is constrained by user configuration, dynamic code may permit violations
      of this rule under extreme conditions to prioritize runtime adaptability.
      Risk:The code exhibits poor maintainability.
      Prevention:Design and Code Review with Clear Structure and Well-Commented Code.

    \li VL_MTR_EthTSyn_STMIF
      Reason:Functions handle specific tasks in the behavior of the entire component; Task has different scenarios that
      need to be covered, which depends on local conditions - this leads to deep nesting of the control structure. Since
      there are more common codes than different codes, a higher level of nesting can be accepted to keep the code
      footprint small.
      Risk:The code exhibits poor maintainability.
      Prevention:Design and Code Review with Clear Structure
      and Well-Commented Code.

    \li VL_MTR_EthTSyn_STPAR
      Reason:The API is defined by the AUTOSAR standard.The standard compliance vote is higher than
      the measurement threshold.
      Risk:For the target uC,the stack usage and runtime are too high.
      Prevention:When testing the result code on the target uC,the user must check the stack usage in
      the project context.

    \li VL_MTR_EthTSyn_STCAL
      Reason:The software structure is defined in accordance with the AUTOSAR standard. The standard
      compliance vote is higher than the measurement threshold. Furthermore, a typical way to reduce
      STCAL is to deepen function nesting,which increases the usage and runtime of the call stack.
      Risk:Because the sectors are distributed to many functions,understandability and testability
      may become overly complex.
      Prevention:Design and Code Review with Clear Structure and Well-Commented Code.

    \li VL_MTR_EthTSyn_STPTH
      Reason:The functions were not separated into sub-functions because there were higher requirements
      for minimizing the stack and applying them to the runtime usage of the code.
      Risk:Because the sectors are distributed to many functions, understandability and testability may
      become overly complex.
      Prevention:Design and Code Review with Clear Structure and Well-Commented Code.

    \li VL_MTR_EthTSyn_STLIN
      Reason:Some function logics are overly complex. Splitting and implementing them will lead to reduced performance.
      Risk:The code exhibits poor maintainability.
      Prevention:Design and Code Review with Clear Structure and Well-Commented Code.

    \li VL_MTR_EthTSyn_STST3
      Reason:Some function logics are overly complex. Splitting and implementing them will lead to reduced performance
      Risk:The code exhibits poor maintainability.
      Prevention:Design and Code Review with Clear Structure and Well-Commented Code.
 */
#ifndef ETHTSYN_H
#define ETHTSYN_H
/* =================================================== inclusions =================================================== */
#include "EthTSyn_Types.h"
#include "EthTSyn_PBcfg.h"
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif

#if (ETHTSYN_MULTIPLE_PARTITION_USED == STD_ON)
#include "Os.h"
#endif
/* =============================================== version information ============================================== */
/** Published Information */
#define ETHTSYN_VENDOR_ID                   ((uint8)62u)
#define ETHTSYN_MODULE_ID                   ((uint8)164u)
#define ETHTSYN_INSTANCE_ID                 ((uint8)0u)
#define ETHTSYN_AR_RELEASE_MAJOR_VERSION    ((uint8)4u)
#define ETHTSYN_AR_RELEASE_MINOR_VERSION    ((uint8)9u)
#define ETHTSYN_AR_RELEASE_REVISION_VERSION ((uint8)0u)
#define ETHTSYN_SW_MAJOR_VERSION            ((uint8)2u)
#define ETHTSYN_SW_MINOR_VERSION            ((uint8)1u)
#define ETHTSYN_SW_PATCH_VERSION            ((uint8)0u)
/* ===================================================== macros ===================================================== */
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
/** Development errors used by the EthTSyn module*/
#define ETHTSYN_E_UNINIT                    ((uint8)0x20u)
#define ETHTSYN_E_INIT_FAILED               ((uint8)0x21u)
#define ETHTSYN_E_CTRL_IDX                  ((uint8)0x22u)
#define ETHTSYN_E_PARAM_POINTER             ((uint8)0x23u)
#define ETHTSYN_E_PARAM                     ((uint8)0x24u)
#define ETHTSYN_E_ALREADY_INITIALIZED       ((uint8)0x25u)
#define ETHTSYN_E_INVALID_PARTITION_CONTEXT ((uint8)0x26u)
#endif

/** magic number*/
#define ETHTSYN_TIME_SYNCHRONIZATION_FRAMETYPE                        ((uint16)0x88F7u)
#define ETHTSYN_TIMEDOMAINID_BYTE_OFFSET                              ((uint8)0x04u)
#define ETHTSYN_SEQUENCEID_BYTE_OFFSET_HIGH                           ((uint8)0x1Eu)
#define ETHTSYN_SEQUENCEID_BYTE_OFFSET_LOW                            ((uint8)0x1Fu)
#define ETHTSYN_MAX_HYSTERESIS_COUNTER                                ((uint8)0xFFu)
#define ETHTSYN_BIT_SHIFT_4                                           ((uint8)4u)
#define ETHTSYN_BIT_SHIFT_8                                           ((uint8)8u)
#define ETHTSYN_BIT_SHIFT_16                                          ((uint8)16u)
#define ETHTSYN_BIT_SHIFT_24                                          ((uint8)24u)
#define ETHTSYN_BIT_SHIFT_32                                          ((uint8)32u)
#define ETHTSYN_BIT_SHIFT_40                                          ((uint8)40u)
#define ETHTSYN_NS_BYTES_TO_EXTRACT                                   ((uint8)6u)
#define ETHTSYN_CORRECTION_FIELD_SIZE                                 ((uint8)8u)
#define ETHTSTN_NS_BITS_TO_EXTRACT                                    ((uint8)48u)
#define ETHTSYN_BITS_PER_BYTE                                         ((uint8)8u)
#define ETHTSYN_MAC_ADDR_BYTE0_OFFSET                                 ((uint8)0u)
#define ETHTSYN_MAC_ADDR_BYTE1_OFFSET                                 ((uint8)1u)
#define ETHTSYN_MAC_ADDR_BYTE2_OFFSET                                 ((uint8)2u)
#define ETHTSYN_MAC_ADDR_BYTE3_OFFSET                                 ((uint8)3u)
#define ETHTSYN_MAC_ADDR_BYTE4_OFFSET                                 ((uint8)4u)
#define ETHTSYN_MAC_ADDR_BYTE5_OFFSET                                 ((uint8)5u)
#define ETHTSYN_PORT_IDENTITY_DEFAULT_VALUE0                          ((uint8)0xffu)
#define ETHTSYN_PORT_IDENTITY_DEFAULT_VALUE1                          ((uint8)0xfeu)
#define ETHTSYN_LOW_4_BITS_MASK                                       ((uint8)0x0fu)
#define ETHTSYN_PDELAY_REQ_SOURCE_PORTID_BYTE_OFFSET                  ((uint8)20u)
#define ETHTSYN_PDELAY_REQ_SOURCE_PORTID_BYTE_LENGTH                  ((uint8)10u)
#define ETHTSYN_PTP_VERSION                                           ((uint8)0x02u)
#define ETHTSYN_MAX_SEQUENCEID                                        ((uint16)0xFFFFu)
#define ETHTSYN_SECONDS_SIZE                                          ((uint8)6u)
#define ETHTSYN_NANOSECONDS_SIZE                                      ((uint8)4u)
#define ETHTSYN_BIT_SHIFT_PER_BYTE                                    ((uint8)8u)
#define ETHTSYN_NANOSECONDS_BYTE_INDEX_START                          ((uint8)6u)
#define ETHTSYN_NANOSECONDS_BYTE_INDEX_END                            ((uint8)10u)
#define ETHTSYN_SYNCHRONIZE_TIMEBASE_MAX_INDEX                        ((uint8)16u)
#define ETHTSYN_CORRECTIONFIELD_BYTE_INDEX                            ((uint8)8u)
#define ETHTSYN_CORRECTIONFIELD_DEFAULT_VALUE_INDEX0                  ((uint8)6u)
#define ETHTSYN_CORRECTIONFIELD_DEFAULT_VALUE_INDEX1                  ((uint8)7u)
#define ETHTSYN_ORIGINAL_TIMESTAMP_BYTE_INDEX                         ((uint8)34u)
#define ETHTSYN_DELAY_HALF_PERIOD                                     ((uint16)2UL)
#define ETHTSYN_BUFFER_TO_TRANSSMIT_LENGTH                            ((uint8)200U)
#define ETHTSYN_TX_ON_MAX_VALUE                                       ((uint8)1u)
#define ETHTSYN_CRC_DATA_LENGTH                                       ((uint8)17u)
#define ETHTSYN_HEADER_SOURCE_PORT_IDENTITY_OFFSET                    ((uint8)20u)
#define ETHTSYN_PDELAY_RESP_REQUESTING_PORT_IDENTITY_OFFSET           ((uint8)44u)
#define ETHTSYN_PDELAY_RESP_FOLLOW_UP_REQUESTING_PORT_IDENTITY_OFFSET ((uint8)44u)
#define ETHTSYN_PHYS_ADDR_LENGTH                                      ((uint8)6u)
#define ETHTSYN_FUP_MESSAGE_TOTAL_LENGTH_BYTE0                        ((uint8)2u)
#define ETHTSYN_FUP_MESSAGE_TOTAL_LENGTH_BYTE1                        ((uint8)3u)

#if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
#define ETHTSYN_FUP_SUBTLV_HEADER_SIZE            ((uint8)2u)
#define ETHTSYN_FUP_SUBTLV_LENGTH_OFFSET          ((uint8)2u)
#define ETHTSYN_FUP_SUBTLV_TIME_LENGTH            ((uint8)3u)
#define ETHTSYN_FUP_SUBTLV_STATUS_LENGTH          ((uint8)2u)
#define ETHTSYN_FUP_SUBTLV_USERDATA_LENGTH        ((uint8)5u)
#define ETHTSYN_SUB_TLV_TYPE_OFS_NOT_SECURED      ((uint8)0x34u)
#define ETHTSYN_SUB_TLV_TYPE_OFS_SECURED          ((uint8)0x44u)
#define ETHTSYN_SUB_TLV_TYPE_USERDATA_NOT_SECURED ((uint8)0x61u)
#define ETHTSYN_SUB_TLV_TYPE_USERDATA_SECURED     ((uint8)0x60u)
#define ETHTSYN_SUB_TLV_TYPE_STATUS_NOT_SECURED   ((uint8)0x51u)
#define ETHTSYN_SUB_TLV_TYPE_STATUS_SECURED       ((uint8)0x50u)
#define ETHTSYN_SUB_TLV_TYPE_TIME_SECURED         ((uint8)0x28u)

#define ETHTSYN_MESSAGE_LENGTH_BIT_MASK           ((uint8)0x01u)
#define ETHTSYN_DOMAIN_NUMBER_BIT_MASK            ((uint8)0x02u)
#define ETHTSYN_CORRECTION_FIELD_BIT_MASK         ((uint8)0x04u)
#define ETHTSYN_SOURCE_PORT_IDENTITY_BIT_MASK     ((uint8)0x08u)
#define ETHTSYN_SEQUENCE_ID_BIT_MASK              ((uint8)0x10u)
#define ETHTSYN_PRECISE_ORIGIN_TIMESTAMP_BIT_MASK ((uint8)0x20u)

#define ETHTSYN_SYNC_TO_GATEWAY_MASK_SUBTLV      ((uint8)0x01u)
#define ETHTSYN_SYNC_TO_GATEWAY_MASK_STBM        ((uint8)0x04u)
#define ETHTSYN_SUBTLV_STATUS_STATUS             ((uint8)0x02u)
#define ETHTSYN_SUBTLV_USERDATA_USER_DATA_LENGTH ((uint8)2u)
#define ETHTSYN_SUBTLV_USERDATA_USER_BYTE_0      ((uint8)3u)
#define ETHTSYN_SUBTLV_USERDATA_USER_BYTE_1      ((uint8)4u)
#define ETHTSYN_SUBTLV_USERDATA_USER_BYTE_2      ((uint8)5u)

#define ETHTSYN_FUP_AUTOSAR_SUBTLV_START_ADDR     ((uint8)86u)
#define ETHTSYN_FUP_AUTOSAR_TLV_HEADER_START_ADDR ((uint8)76u)
#define ETHTSYN_FUP_TLV_TYPE_1                    ((uint8)0x0u)
#define ETHTSYN_FUP_TLV_TYPE_0                    ((uint8)0x01u)
#define ETHTSYN_FUP_EXT_TLV_LENGTH_0              ((uint8)0x06u)
#define ETHTSYN_FUP_TLV_ORGID_2                   ((uint8)0x04u)
#define ETHTSYN_FUP_TLV_ORGID_1                   ((uint8)0x05u)
#define ETHTSYN_FUP_TLV_ORGID_0                   ((uint8)0x06u)

#define ETHTSYN_FUP_EXT_TLV_ORGID_2 ((uint8)0x1AU)
#define ETHTSYN_FUP_EXT_TLV_ORGID_1 ((uint8)0x75U)
#define ETHTSYN_FUP_EXT_TLV_ORGID_0 ((uint8)0xFBU)

#define ETHTSYN_FUP_TLV_ORG_SUBTYPE_2 ((uint8)0x7u)
#define ETHTSYN_FUP_TLV_ORG_SUBTYPE_1 ((uint8)0x8u)
#define ETHTSYN_FUP_TLV_ORG_SUBTYPE_0 ((uint8)0x9u)

#define ETHTSYN_FUP_EXT_TLV_ORG_SUBTYPE_2 ((uint8)0x60u)
#define ETHTSYN_FUP_EXT_TLV_ORG_SUBTYPE_1 ((uint8)0x56u)
#define ETHTSYN_FUP_EXT_TLV_ORG_SUBTYPE_0 ((uint8)0x76u)

#define ETHTSYN_CRC_TIME0_OFFSET                ((uint8)3u)
#define ETHTSYN_CRC_TIME1_OFFSET                ((uint8)4u)
#define ETHTSYN_CRC_DATA_0_LENGTH               ((uint8)24u)
#define ETHTSYN_CRC_DATA_1_LENGTH               ((uint8)16u)
#define ETHTSYN_CRC_DATA_TO_BE_CACULATED_LENGTH ((uint8)17u)
#endif

/** service id*/
#define ETHTSYN_SID_INIT                ((uint8)0x01u)
#define ETHTSYN_SID_GETVERSIONINFO      ((uint8)0x02u)
#define ETHTSYN_SID_SETTRANSMISSIONMODE ((uint8)0x05u)
#define ETHTSYN_SID_RXINDICATION        ((uint8)0x06u)
#define ETHTSYN_SID_TXCONFIRMATION      ((uint8)0x07u)
#define ETHTSYN_SID_TRCVLINKSTATECHG    ((uint8)0x08u)
#define ETHTSYN_SID_MAINFUNCTION        ((uint8)0x09u)
#define ETHTSYN_MAC_ADDRESS_LENGTH      ((uint8)0x06u)

#if (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON)
/** Time Master conflict */
#define ETHTSYN_E_TMCONFLICT ((uint8)0x01u)
/** Time Slave conflict */
#define ETHTSYN_E_TSCONFLICT ((uint8)0x02u)
#endif

#if !defined(ETHTSYN_LOCAL)
#ifdef ETHTSYN_GT_EXTERN
#define ETHTSYN_LOCAL
#else
#define ETHTSYN_LOCAL static /* PRQA S 3414 */ /* VL_QAC_KeyWord */
#endif
#endif
/* ================================================ type definitions ================================================ */
/* ========================================== internal function definitions ========================================= */
/* =========================================== external data declarations =========================================== */

extern const uint8 EthTSynDestPhyAddr[ETHTSYN_MAC_ADDRESS_LENGTH];

/* ========================================= external function declarations ========================================= */

/**
 * @brief       This function initializes the Time Synchronization over Ethernet.
 * @param[in]   configPtr   : Pointer to the selected configuration set.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70660
 */
void EthTSyn_Init(const EthTSyn_ConfigType* configPtr);
#if (ETHTSYN_VERSION_INFO_API == STD_ON)
/**
 * @brief       Returns the version information of this module.
 * @param[in]   versioninfo   : Pointer to the memory location holding the version information of the module.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70661
 */
void EthTSyn_GetVersionInfo(Std_VersionInfoType* versioninfo);
#endif

/**
 * @brief       This API is used to turn on and off the TX capabilities of the EthTSyn.
 * @param[in]   ctrlIdx: Index of the Ethernet controller.
 * @param[in]   mode: the mode of Transmission.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70662
 */
void EthTSyn_SetTransmissionMode(uint8 ctrlIdx, EthTSyn_TransmissionModeType mode);
/**
 * @brief       By this API service the EthTSyn gets an indication and the data of a received frame.
 * @param[in]   ctrlIdx: Index of the Ethernet controller.
 * @param[in]   frameType: frame type of received Ethernet frame
 * @param[in]   isBroadcast: parameter to indicate a broadcast frame
 * @param[in]   physAddrPtr: pointer to Physical source address (MAC address in network byte
 * @param[in]   dataPtr: Pointer to payload of the received Ethernet frame (i.e. Ethernet header is not provided).
 * @param[in]   lenByte: Length of received data.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70665
 */
void EthTSyn_RxIndication(
    uint8         ctrlIdx,
    Eth_FrameType frameType,
    boolean       isBroadcast,
    const uint8*  physAddrPtr,
    const uint8*  dataPtr,
    uint16        lenByte);
/**
 * @brief         Confirms the transmission of an Ethernet frame.
 * @param[in]     ctrlIdx: Index of the Ethernet controller within the context of the Ethernet Interface.
 * @param[in]     bufIdx: Index of the buffer resource.
 * @param[in]     result: E_OK: The transmission was successful, E_NOT_OK: The transmission failed.
 * @synchronous   TRUE
 * @reentrancy    FALSE
 * @trace         CPD-70666
 */
void EthTSyn_TxConfirmation(uint8 ctrlIdx, Eth_BufIdxType bufIdx, Std_ReturnType result);
/**
 * @brief         Allows resetting state machine in case of unexpected Link loss to avoid inconsistent Sync and
 * Follow_Up sequences.
 * @param[in]     ctrlIdx: Index of the Ethernet controller within the context of the Ethernet Interface.
 * @param[in]     trcvLinkState: ETHTRCV_LINK_STATE_DOWN ETHTRCV_LINK_STATE_ACTIVE
 * @synchronous   TRUE
 * @reentrancy    FALSE
 * @trace         CPD-70667
 */
void EthTSyn_TrcvLinkStateChg(uint8 ctrlIdx, EthTrcv_LinkStateType trcvLinkState);
/**
 * @brief         Main function for cyclic call/resp. Sync, Follow_Up and Pdelay_Req transmissions.
 * @param[in]     portIndex: Index of the configured Port.
 * @synchronous   TRUE
 * @reentrancy    FALSE
 * @trace         CPD-70670
 */
void EthTSyn_MainFunction(uint8 portIndex);
#endif
