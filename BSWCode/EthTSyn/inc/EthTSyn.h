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
 *  V2.1.0    2025-01-01 shuangyang.fu  1. Support multi-partition distribution.
 *  V2.1.1    2026-06-03 xiaojian.liang CPTASK-1133: Add ARTI stub functions and ARTI_TRACE_N instrumentation.
 *            2026-07-21 junwei.he      [CPT-19245] [EthTSyn] Message length bytes used for CRC calculation do not match
 *                                      requirements
 *
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

    \li VL_EthTSyn_clockIdentity
      Reason: The array indices 0-7 and the literal values 0xFFU, 0xFEU are required by IEEE 802.1AS-2011 clause 8.5.2.2
              to map a 6-byte MAC address to an 8-byte EUI-64 clockIdentity. The positions and fixed bytes are
              explicitly defined in the standard; using them directly ensures traceability to the specification and
              avoids unnecessary abstraction that would obfuscate the mandated layout.
      Risk: No risk.
      Prevention: No prevention.

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

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */
/**
 * @name Published Information
 * @{
 * @brief EthTSyn Publisted Infomation.
 */
#define ETHTSYN_VENDOR_ID   ((uint8)62u)  /**< AUTOSAR Vendor ID assigned to the module developer. */
#define ETHTSYN_MODULE_ID   ((uint8)164u) /**< Unique module identifier within AUTOSAR specifications. */
#define ETHTSYN_INSTANCE_ID ((uint8)0u)   /**< Instance ID for multi-instance module support (0 for single instance). */
#define ETHTSYN_AR_RELEASE_MAJOR_VERSION \
    ((uint8)4u) /**< Major version of the AUTOSAR specification this module complies with. */
#define ETHTSYN_AR_RELEASE_MINOR_VERSION \
    ((uint8)9u) /**< Minor version of the AUTOSAR specification this module complies with. */
#define ETHTSYN_AR_RELEASE_REVISION_VERSION \
    ((uint8)0u) /**< Revision version of the AUTOSAR specification this module complies with. */
#define ETHTSYN_SW_MAJOR_VERSION ((uint8)2u) /**< Major version of the EthTSyn module software implementation. */
#define ETHTSYN_SW_MINOR_VERSION ((uint8)1u) /**< Minor version of the EthTSyn module software implementation. */
#define ETHTSYN_SW_PATCH_VERSION \
    ((uint8)0u) /**< Patch/Revision version of the EthTSyn module software implementation. */
/** @} */

/* ===================================================== macros ===================================================== */
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
/** Development errors used by the EthTSyn module*/

/**
 * @name EthTSyn_DevelopmentErrorCodes
 * @{
 * @brief Development error codes used by the EthTSyn module for error detection and reporting.
 */
#define ETHTSYN_E_UNINIT                    ((uint8)0x20u) /**< API service called before module initialization. */
#define ETHTSYN_E_INIT_FAILED               ((uint8)0x21u) /**< Module initialization failed. */
#define ETHTSYN_E_CTRL_IDX                  ((uint8)0x22u) /**< Invalid controller index parameter provided. */
#define ETHTSYN_E_PARAM_POINTER             ((uint8)0x23u) /**< NULL pointer parameter where valid pointer expected. */
#define ETHTSYN_E_PARAM                     ((uint8)0x24u) /**< Invalid parameter value (out of range or not allowed). */
#define ETHTSYN_E_ALREADY_INITIALIZED       ((uint8)0x25u) /**< Module initialization attempted when already initialized. */
#define ETHTSYN_E_INVALID_PARTITION_CONTEXT ((uint8)0x26u) /**< Service called from wrong partition context. */
#define ETHTSYN_E_UNRECOGNIZED_TLV          ((uint8)0x27u) /**< Received TLV (Type-Length-Value) format not recognized. */
#endif
/** @} */

/** magic number*/
#define ETHTSYN_TIME_SYNCHRONIZATION_FRAMETYPE ((uint16)0x88F7u) /**< PTP frame type. */

/**
 * @name ETHTSYN_SEQUENCEID_BYTE_OFFSET_{{HIGH or LOW}}
 * @{
 * @brief Protocol-specific constants for Time Protocol implementation.
 */
#define ETHTSYN_SEQUENCEID_BYTE_OFFSET_HIGH \
    ((uint8)0x1Eu) /**< Byte offset to the high byte of SequenceID in PTP message. */
#define ETHTSYN_SEQUENCEID_BYTE_OFFSET_LOW \
    ((uint8)0x1Fu) /**< Byte offset to the low byte of SequenceID in PTP message. */
/** @} */

/**
 * @name EthTSyn_PTPProtocolConstants
 * @{
 * @brief Protocol-specific constants for Time Protocol implementation.
 */
#define ETHTSYN_TIMEDOMAINID_BYTE_OFFSET \
    ((uint8)0x04u) /**< Byte offset to the TimeDomainID field in PTP message header. */
#define ETHTSYN_MAX_HYSTERESIS_COUNTER ((uint8)0xFFu) /**< Maximum value for hysteresis counter . */
/** @} */
/**
 * @name ETHTSYN_BIT_SHIFT_{{value}}
 * @{
 * @brief the value of bit offset
 */
#define ETHTSYN_BIT_SHIFT_4            ((uint8)4u)
#define ETHTSYN_BIT_SHIFT_8            ((uint8)8u)
#define ETHTSYN_BIT_SHIFT_16           ((uint8)16u)
#define ETHTSYN_BIT_SHIFT_24           ((uint8)24u)
#define ETHTSYN_BIT_SHIFT_32           ((uint8)32u)
#define ETHTSYN_BIT_SHIFT_40           ((uint8)40u)
/** @} */

/**
 * @name ETHTSYN_TimeExtractionParameters
 * @{
 * @brief Time extraction parameters for timestamp processing in PTP messages.
 */
#define ETHTSYN_NS_BYTES_TO_EXTRACT   ((uint8)6u)  /**< Number of bytes to extract for nanosecond timestamp. */
#define ETHTSYN_CORRECTION_FIELD_SIZE ((uint8)8u)  /**< Size of correction field in PTP messages . */
#define ETHTSTN_NS_BITS_TO_EXTRACT    ((uint8)48u) /**< Number of bits to extract for nanosecond precision. */
#define ETHTSYN_BITS_PER_BYTE         ((uint8)8u)  /**< Number of bits per byte. */
/** @} */

/**
 * @name EthTSyn_ProtocolConstants
 * @{
 * @brief Protocol-specific constants for Ethernet Time Synchronization operations.
 */
#define ETHTSYN_LOW_4_BITS_MASK ((uint8)0x0fu) /**< Mask to extract the low 4 bits of a byte . */
#define ETHTSYN_PDELAY_REQ_SOURCE_PORTID_BYTE_OFFSET \
    ((uint8)20u) /**< Byte offset to source port identity in Pdelay_Req message. */
#define ETHTSYN_PDELAY_REQ_SOURCE_PORTID_BYTE_LENGTH                 \
    ((uint8)10u) /**< Length of source port identity field in bytes. \
                  */
/** @} */

#define ETHTSYN_PTP_VERSION ((uint8)0x02u) /**< the value PTP version. */

/**
 * @name ETHTSYN_TimeSynchronizationParameters
 * @{
 * @brief Time synchronization parameters and field size definitions for PTP messages.
 */
#define ETHTSYN_MAX_SEQUENCEID     ((uint16)0xFFFFu) /**< Maximum sequence ID value . */
#define ETHTSYN_SECONDS_SIZE       ((uint8)6u)       /**< Size of seconds field in timestamp. */
#define ETHTSYN_NANOSECONDS_SIZE   ((uint8)4u)       /**< Size of nanoseconds field in timestamp . */
#define ETHTSYN_BIT_SHIFT_PER_BYTE ((uint8)8u)       /**< Bit shift value per byte (8 bits). */
#define ETHTSYN_NANOSECONDS_BYTE_INDEX_START                                                                        \
    ((uint8)6u)                                         /**< Starting byte index of nanoseconds field in timestamp. \
                                                         */
#define ETHTSYN_NANOSECONDS_BYTE_INDEX_END ((uint8)10u) /**< Ending byte index of nanoseconds field in timestamp . */
#define ETHTSYN_SYNCHRONIZE_TIMEBASE_MAX_INDEX \
    ((uint8)16u) /**< Maximum index for timebase synchronization operations. */
/** @} */

#define ETHTSYN_CORRECTIONFIELD_BYTE_INDEX ((uint8)8u) /**< Byte index of correction field in PTP message. */

/**
 * @name ETHTSYN_CorrectionField_Indices
 * @{
 * @brief Correction field byte indices for PTP message processing.
 */
#define ETHTSYN_CORRECTIONFIELD_DEFAULT_VALUE_INDEX0 \
    ((uint8)6u) /**< Byte index 0 for default correction field value. */
#define ETHTSYN_CORRECTIONFIELD_DEFAULT_VALUE_INDEX1 \
    ((uint8)7u) /**< Byte index 1 for default correction field value. */
/** @} */

#define ETHTSYN_ORIGINAL_TIMESTAMP_BYTE_INDEX \
    ((uint8)34u) /**< the byte index of Original Timestamp in received buffer.*/

/**
 * @name ETHTSYN_MessageProcessingConstants
 * @{
 * @brief Message processing constants and field definitions for Ethernet Time Synchronization.
 */
#define ETHTSYN_DELAY_HALF_PERIOD ((uint16)2UL) /**< Half period value for delay calculation. */
#define ETHTSYN_BUFFER_TO_TRANSSMIT_LENGTH \
    ((uint16)ETHTSYN_ICV_CIRCULAR_BUFFER_SIZE) /**< Length of buffer for transmission . */
#define ETHTSYN_TX_ON_MAX_VALUE ((uint8)1u)    /**< Maximum value for transmission enable flag . */
#define ETHTSYN_CRC_DATA_LENGTH ((uint8)17u)   /**< Data length for CRC calculation in bytes. */
#define ETHTSYN_HEADER_SOURCE_PORT_IDENTITY_OFFSET \
    ((uint8)20u) /**< Offset to source port identity in PTP message header. */
#define ETHTSYN_PDELAY_RESP_REQUESTING_PORT_IDENTITY_OFFSET \
    ((uint8)44u) /**< Offset to requesting port identity in Pdelay_Resp message. */
#define ETHTSYN_PDELAY_RESP_FOLLOW_UP_REQUESTING_PORT_IDENTITY_OFFSET \
    ((uint8)44u) /**< Offset to requesting port identity in Pdelay_Resp_Follow_Up message. */
#define ETHTSYN_FUP_MESSAGE_TOTAL_LENGTH_OFFSET ((uint8)2u)   /**< Offset to total length field in Follow_Up message. */
#define ETHTSYN_MAX_TIMEBASE_ID                 ((uint8)127u) /**< Maximum timebase ID value . */
#define ETHTSYN_CONST_3U                        ((uint8)0x03u)        /**< Constant value 3 for various calculations. */
#define ETHTSYN_CONST_7U                        ((uint8)0x07u)        /**< Constant value 7 for various calculations. */
#define ETHTSYN_MAX_JOBID                       ((uint32)0xFFFFFFFFu) /**< Maximum job ID value . */
#define ETHTSYN_LOCAL_TIME_MAX_SECONDS          ((uint64)18446744073LLu) /**< Maximum local time in seconds . */
#define ETHTSYN_PDELAY_DIVISOR                  ((uint64)2LLu)           /**< Divisor for path delay calculation . */
/** @} */

/* RateRatio */
/**
 * @name EthTSyn_CumulativeScaledRateConstants
 * @{
 * @brief Constants for cumulative scaled rate field byte offsets in PTP messages.
 */
#define ETHTSYN_CUMULATIVE_SCALED_RATE_OFFSET_BTE0 \
    ((uint8)54u) /**< Byte offset 0 for cumulative scaled rate field in PTP message. */
#define ETHTSYN_CUMULATIVE_SCALED_RATE_OFFSET_BTE1 \
    ((uint8)55u) /**< Byte offset 1 for cumulative scaled rate field in PTP message. */
#define ETHTSYN_CUMULATIVE_SCALED_RATE_OFFSET_BTE2 \
    ((uint8)56u) /**< Byte offset 2 for cumulative scaled rate field in PTP message. */
#define ETHTSYN_CUMULATIVE_SCALED_RATE_OFFSET_BTE3 \
    ((uint8)57u) /**< Byte offset 3 for cumulative scaled rate field in PTP message. */
/** @} */

/**
 * @name EthTSyn_RateCalculationConstants
 * @{
 * @brief Constants for rate ratio and floating point calculations.
 */
#define ETHTSYN_ROUND_VALUE           (0.5)           /**< Rounding constant for floating point calculations. */
#define ETHTSYN_RATE_RATIO_MULTIPLIER ((1uLL << 41u)) /**< Multiplier for rate ratio calculation . */
/** @} */

/**
 * @name EthTSyn_RateRatioLimitConstants
 * @{
 * @brief Constants defining limits for rate ratio calculations.
 */
#define ETHTSYN_MAX_LIMIT_RATE_RATIO_VALUE ((sint32)2147483647) /**< Maximum positive limit for rate ratio value. */
#define ETHTSYN_MINIMAL_LIMIT_RATE_RATIO_VALUE_INT \
    ((sint32) - 2147483648LL) /**< Minimum negative limit for rate ratio integer value. */
#define ETHTSYN_MINIMAL_LIMIT_RATE_RATIO_VALUE_FLOAT \
    (-2147483648.0) /**< Minimum negative limit for rate ratio floating point value. */
/** @} */

/**
 * @name EthTSyn_FollowUpSubTLVConstants
 * @{
 * @brief Constants for Follow_Up message sub-TLV handling (when MESSAGE_COMPLIANCE is STD_OFF).
 */
#if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
#define ETHTSYN_FUP_SUBTLV_HEADER_SIZE            ((uint8)2u) /**< Header size for Follow_Up sub-TLV. */
#define ETHTSYN_FUP_SUBTLV_LENGTH_OFFSET          ((uint8)2u) /**< Offset to length field within sub-TLV. */
#define ETHTSYN_FUP_SUBTLV_TIME_LENGTH            ((uint8)3u) /**< Length of time sub-TLV field. */
#define ETHTSYN_FUP_SUBTLV_STATUS_LENGTH          ((uint8)2u) /**< Length of status sub-TLV field. */
#define ETHTSYN_FUP_SUBTLV_USERDATA_LENGTH        ((uint8)5u) /**< Length of user data sub-TLV field. */
/** @} */
/**
 * @name ETHTSYN_SUB_TLV_TYPE_{{type}}_{{SECURED OR NOT_SECURED}}
 * @{
 * @brief the sub tlv type.
 */
#define ETHTSYN_SUB_TLV_TYPE_OFS_NOT_SECURED      ((uint8)0x34u)
#define ETHTSYN_SUB_TLV_TYPE_OFS_SECURED          ((uint8)0x44u)
#define ETHTSYN_SUB_TLV_TYPE_USERDATA_NOT_SECURED ((uint8)0x61u)
#define ETHTSYN_SUB_TLV_TYPE_USERDATA_SECURED     ((uint8)0x60u)
#define ETHTSYN_SUB_TLV_TYPE_STATUS_NOT_SECURED   ((uint8)0x51u)
#define ETHTSYN_SUB_TLV_TYPE_STATUS_SECURED       ((uint8)0x50u)
#define ETHTSYN_SUB_TLV_TYPE_TIME_SECURED         ((uint8)0x28u)
/** @} */
/**
 * @name EthTSYN_CRCMaskConstants
 * @{
 * @brief Bit masks for CRC field validation in PTP messages.
 */
#define ETHTSYN_MESSAGE_LENGTH_BIT_MASK           ((uint8)0x01u) /**< Bit mask for message length field validation. */
#define ETHTSYN_DOMAIN_NUMBER_BIT_MASK            ((uint8)0x02u) /**< Bit mask for domain number field validation. */
#define ETHTSYN_CORRECTION_FIELD_BIT_MASK         ((uint8)0x04u) /**< Bit mask for correction field validation. */
#define ETHTSYN_SOURCE_PORT_IDENTITY_BIT_MASK                                                                \
    ((uint8)0x08u)                                  /**< Bit mask for source port identity field validation. \
                                                     */
#define ETHTSYN_SEQUENCE_ID_BIT_MASK ((uint8)0x10u) /**< Bit mask for sequence ID field validation. */
#define ETHTSYN_PRECISE_ORIGIN_TIMESTAMP_BIT_MASK \
    ((uint8)0x20u)                                   /**< Bit mask for precise origin timestamp field validation. */
#define ETHTSYN_MAX_MASK_32BIT ((uint32)0xFFFFFFFFu) /**< 32-bit maximum mask. */
/** @} */

/**
 * @name EthTSyn_SyncToGatewayConstants
 * @{
 * @brief Constants for synchronization to gateway masks and sub-TLV parameters.
 */
#define ETHTSYN_SYNC_TO_GATEWAY_MASK_SUBTLV      ((uint8)0x01u) /**< Sub-TLV mask for synchronization to gateway. */
#define ETHTSYN_SYNC_TO_GATEWAY_MASK_STBM        ((uint8)0x04u) /**< StbM mask for synchronization to gateway. */
#define ETHTSYN_SUBTLV_STATUS_STATUS             ((uint8)0x02u) /**< Status field within sub-TLV status. */
#define ETHTSYN_SUBTLV_USERDATA_USER_DATA_LENGTH ((uint8)2u)    /**< User data length within user data sub-TLV. */
#define ETHTSYN_SUBTLV_USERDATA_USER_BYTE_0      ((uint8)3u)    /**< Byte offset 0 for user data within sub-TLV. */
#define ETHTSYN_SUBTLV_USERDATA_USER_BYTE_1      ((uint8)4u)    /**< Byte offset 1 for user data within sub-TLV. */
#define ETHTSYN_SUBTLV_USERDATA_USER_BYTE_2      ((uint8)5u)    /**< Byte offset 2 for user data within sub-TLV. */
#define ETHTSYN_TLV_LENGTH_FIELD_OFFSET          ((uint8)78u)   /**< Offset to length field in TLV structure. */
/** @} */

/**
 * @name EthTSyn_AUTOSARTLVConstants
 * @{
 * @brief Constants for AUTOSAR-specific TLV handling in Follow_Up messages.
 */
#define ETHTSYN_FUP_AUTOSAR_SUBTLV_START_ADDR \
    ((uint8)86u) /**< Start address for AUTOSAR-specific sub-TLV in Follow_Up message. */
#define ETHTSYN_FUP_AUTOSAR_TLV_HEADER_START_ADDR \
    ((uint8)76u)                                    /**< Start address for AUTOSAR TLV header in Follow_Up message. */
#define ETHTSYN_FUP_TLV_TYPE_1       ((uint8)0x0u)  /**< TLV type value 1 for AUTOSAR extension. */
#define ETHTSYN_FUP_TLV_TYPE_0       ((uint8)0x01u) /**< TLV type value 0 for AUTOSAR extension. */
#define ETHTSYN_FUP_EXT_TLV_LENGTH_0 ((uint8)0x06u) /**< Extended TLV length value 0. */
#define ETHTSYN_FUP_TLV_ORGID_2      ((uint8)0x04u) /**< Organization ID byte 2 for TLV. */
#define ETHTSYN_FUP_TLV_ORGID_1      ((uint8)0x05u) /**< Organization ID byte 1 for TLV. */
#define ETHTSYN_FUP_TLV_ORGID_0      ((uint8)0x06u) /**< Organization ID byte 0 for TLV. */
/** @} */

/**
 * @name ETHTSYN_FUP_ExtendedTLV_OrganizationIDs
 * @{
 * @brief Extended TLV organization ID definitions for AUTOSAR Follow_Up message extension.
 */
#define ETHTSYN_FUP_EXT_TLV_ORGID_2 ((uint8)0x1AU) /**< Extended TLV organization ID byte 2. */
#define ETHTSYN_FUP_EXT_TLV_ORGID_1 ((uint8)0x75U) /**< Extended TLV organization ID byte 1. */
#define ETHTSYN_FUP_EXT_TLV_ORGID_0 ((uint8)0xFBU) /**< Extended TLV organization ID byte 0 . */
/** @} */

/**
 * @name ETHTSYN_FUP_TLV_OrganizationSubtypes
 * @{
 * @brief TLV organization subtype definitions for AUTOSAR Follow_Up messages.
 */
#define ETHTSYN_FUP_TLV_ORG_SUBTYPE_2 ((uint8)0x7u) /**< TLV organization subtype byte 2. */
#define ETHTSYN_FUP_TLV_ORG_SUBTYPE_1 ((uint8)0x8u) /**< TLV organization subtype byte 1. */
#define ETHTSYN_FUP_TLV_ORG_SUBTYPE_0 ((uint8)0x9u) /**< TLV organization subtype byte 0. */
/** @} */

/**
 * @name ETHTSYN_FUP_ExtendedTLV_OrganizationSubtypes
 * @{
 * @brief Extended TLV organization subtype definitions for AUTOSAR Follow_Up message extension.
 */
#define ETHTSYN_FUP_EXT_TLV_ORG_SUBTYPE_2 ((uint8)0x60u) /**< Extended TLV organization subtype byte 2. */
#define ETHTSYN_FUP_EXT_TLV_ORG_SUBTYPE_1 ((uint8)0x56u) /**< Extended TLV organization subtype byte 1. */
#define ETHTSYN_FUP_EXT_TLV_ORG_SUBTYPE_0 ((uint8)0x76u) /**< Extended TLV organization subtype byte 0. */
/** @} */
#endif

#if (ETHTSYN_CRC_SUPPORT == STD_ON)
/**
 * @name ETHTSYN_CRC_CalculationParameters
 * @{
 * @brief CRC (Cyclic Redundancy Check) calculation parameters and offsets.
 */
#define ETHTSYN_CRC_TIME0_OFFSET                ((uint8)3u)  /**< Time field offset 0 for CRC calculation. */
#define ETHTSYN_CRC_TIME1_OFFSET                ((uint8)4u)  /**< Time field offset 1 for CRC calculation. */
#define ETHTSYN_CRC_DATA_0_LENGTH               ((uint8)24u) /**< Length of data block 0 for CRC calculation. */
#define ETHTSYN_CRC_DATA_1_LENGTH               ((uint8)16u) /**< Length of data block 1 for CRC calculation. */
#define ETHTSYN_CRC_DATA_TO_BE_CACULATED_LENGTH ((uint8)17u) /**< Total data length to be used for CRC calculation. */
/** @} */
#endif

/**
 * @name Service ID
 * @{
 * @brief Service ID of EthTSyn
 */
/* clang-format off */
#define ETHTSYN_SID_INIT                      0x01u /**< Service ID for EthTSyn module initialization. */
#define ETHTSYN_SID_GETVERSIONINFO            0x02u /**< Service ID for retrieving module version information. */
#define ETHTSYN_SID_SETTRANSMISSIONMODE       0x05u /**< Service ID for setting transmission mode. */
#define ETHTSYN_SID_RXINDICATION              0x06u /**< Service ID for receive indication callback. */
#define ETHTSYN_SID_TXCONFIRMATION            0x07u /**< Service ID for transmit confirmation callback. */
#define ETHTSYN_SID_TRCVLINKSTATECHG          0x08u /**< Service ID for transceiver link state change notification. */
#define ETHTSYN_SID_MAINFUNCTION              0x09u /**< Service ID for main function periodic execution. */
#define ETHTSYN_SID_SETPROTOCOLPARAM          0x0au /**< Service ID for setting protocol parameters. */
#define ETHTSYN_SID_GETPROTOCOLPARAM          0x0bu /**< Service ID for getting protocol parameters. */
#define ETHTSYN_SID_ICVGENERATIONINDICATION   0x0cu /**< Service ID for Integrity Check Value (ICV) generation indication. */
#define ETHTSYN_SID_ICVVERIFICATIONINDICATION 0x0du /**< Service ID for Integrity Check Value (ICV) verification indication. */
/* clang-format on */
/** @} */

#define ETHTSYN_MAC_ADDRESS_LENGTH ((uint8)0x06u) /**< Length of MAC address in bytes (48 bits). */

#if (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON)

#define ETHTSYN_E_TMCONFLICT ((uint8)0x01u) /**< Time Master conflict */

#define ETHTSYN_E_TSCONFLICT ((uint8)0x02u) /**< Time Master conflict */
#endif

#if (ETHTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)

#define ETHTSYN_SUB_TLV_TYPE_TIME_AUTHENTICATED ((uint8)0x70) /**< Sub-TLV type for time authenticated field. */

/**
 * @name ETHTSYN_ICV_Flags
 * @{
 * @brief Integrity Check Value (ICV) flag definitions for security processing.
 */
#define ETHTSYN_ICV_WITH_FV              ((uint8)0x01u) /**< Flag indicating ICV includes Freshness Value (FV). */
#define ETHTSYN_ICV_FV_GENERATION_FAILED ((uint8)0x02u) /**< Flag indicating Freshness Value generation failed. */
#define ETHTSYN_ICV_IN_MULTIPLE_SUB_TLV \
    ((uint8)0x04u) /**< Flag indicating ICV is distributed across multiple sub-TLVs. */
/** @} */

/**
 * @name ETHTSYN_TimeAuthenticated_ICVLengths
 * @{
 * @brief Maximum ICV length definitions for time authenticated message fragments.
 */
#define ETHTSYN_TIME_AUTHENTICATED_FIRST_FRAGMENT_MAX_ICV_LENGTH \
    ((uint8)0xCDu) /**< Maximum ICV length for first fragment of time authenticated message. */
#define ETHTSYN_TIME_AUTHENTICATED_REMAIN_FRAGMENT_MAX_ICV_LENGTH \
    ((uint8)0xD6u) /**< Maximum ICV length for remaining fragments of time authenticated message. */
/** @} */

#define ETHTSYN_ICV_TIME_AUTHENTICATED_WITHOUT_FV_ICV_LENGTH \
    ((uint8)0x04u) /**< ICV length for time authenticated message without Freshness Value. */

/**
 * @name ETHTSYN_ICV_FieldOffsets
 * @{
 * @brief Field offset definitions within Integrity Check Value (ICV) structure.
 */
#define ETHTSYN_ICV_LENGTH_OFFSET ((uint8)0x01u) /**< Offset to ICV length field within ICV structure. */
#define ETHTSYN_ICV_FLAG_OFFSET   ((uint8)0x02u) /**< Offset to ICV flag field within ICV structure. */
#define ETHTSYN_ICV_SEQUENCE_NUMBER_OFFSET \
    ((uint8)0x03u) /**< Offset to ICV sequence number field within ICV structure. */
#define ETHTSYN_ICV_FRESHNESS_VALUE_LENGTH_OFFSET \
    ((uint8)0x04u) /**< Offset to Freshness Value length field within ICV structure. */
#define ETHTSYN_ICV_FRESHNESS_VALUE_OFFSET                                    \
    ((uint8)0x05u) /**< Offset to Freshness Value field within ICV structure. \
                    */
/** @} */

/**
 * @name ETHTSYN_ICV_TX_ProgressStatus
 * @{
 * @brief ICV transmission progress status definitions.
 */
#define ETHTSYN_ICV_TX_PROGRESS_STATUS_BUILDING ((uint8)0x01u) /**< ICV transmission progress: building ICV. */
#define ETHTSYN_ICV_TX_PROGRESS_STATUS_WAITING_INDICATION \
    ((uint8)0x02u) /**< ICV transmission progress: waiting for generation indication. */
#define ETHTSYN_ICV_TX_PROGRESS_STATUS_COMFIRMATION \
    ((uint8)0x03u)                                          /**< ICV transmission progress: confirmation received. */
#define ETHTSYN_ICV_TX_PROGRESS_STATUS_RETRY ((uint8)0x04u) /**< ICV transmission progress: retry required. */
/** @} */

/**
 * @name ETHTSYN_ICV_RX_ProgressStatus
 * @{
 * @brief ICV reception progress status definitions.
 */
#define ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_INITIAL \
    ((uint8)0x04u) /**< ICV reception progress: initial validation state. */
#define ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_START \
    ((uint8)0x05u) /**< ICV reception progress: starting validation. */
#define ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_WAITING_INDICATION \
    ((uint8)0x06u) /**< ICV reception progress: waiting for verification indication. */
#define ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_COMFIRMATION \
    ((uint8)0x07u) /**< ICV reception progress: confirmation received. */
#define ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_FAILED                                                               \
    ((uint8)0x08u)                                                   /**< ICV reception progress: validation failed. \
                                                                      */
#define ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_RETRY ((uint8)0x09u) /**< ICV reception progress: retry validation. */
/** @} */

#if !defined(ETHTSYN_E_VALIDATE_ICV)
#define ETHTSYN_E_VALIDATE_ICV 0x02u /**< Development error code: ICV validation failed. */
#endif

#define ETHTSYN_E_FRESHNESSFAILURE ((uint8)0x03u) /**< No FV available from the FVM */

#endif

#if !defined(ETHTSYN_LOCAL)
#define ETHTSYN_LOCAL static /* PRQA S 3414 */ /* VL_QAC_KeyWord */
#endif

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief       This function initializes the Time Synchronization over Ethernet.
 * @param[in]   configPtr   : Pointer to the selected configuration set.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70660
 */
void EthTSyn_Init(const EthTSyn_ConfigType* configPtr);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#if (ETHTSYN_VERSION_INFO_API == STD_ON)
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief       Returns the version information of this module.
 * @param[out]  versioninfo Pointer to the memory location holding the version information of the module.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70661
 */
void EthTSyn_GetVersionInfo(Std_VersionInfoType* versioninfo);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#endif

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief       This API is used to turn on and off the TX capabilities of the EthTSyn.
 * @param[in]   ctrlIdx: Index of the Ethernet controller.
 * @param[in]   mode: the mode of Transmission.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70662
 */
void EthTSyn_SetTransmissionMode(uint8 ctrlIdx, EthTSyn_TransmissionModeType mode);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
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
 * @trace       CPD-85827
 */
void EthTSyn_RxIndication(
    uint8         ctrlIdx,
    Eth_FrameType frameType,
    boolean       isBroadcast,
    const uint8*  physAddrPtr,
    const uint8*  dataPtr,
    uint16        lenByte);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         Confirms the transmission of an Ethernet frame.
 * @param[in]     ctrlIdx: Index of the Ethernet controller within the context of the Ethernet Interface.
 * @param[in]     bufIdx: Index of the buffer resource.
 * @param[in]     result: E_OK: The transmission was successful, E_NOT_OK: The transmission failed.
 * @synchronous   TRUE
 * @reentrant     FALSE
 * @trace         CPD-85828
 */
void EthTSyn_TxConfirmation(uint8 ctrlIdx, Eth_BufIdxType bufIdx, Std_ReturnType result);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         Allows resetting state machine in case of unexpected Link loss to avoid inconsistent Sync and
 * Follow_Up sequences.
 * @param[in]     ctrlIdx: Index of the Ethernet controller within the context of the Ethernet Interface.
 * @param[in]     trcvLinkState: ETHTRCV_LINK_STATE_DOWN ETHTRCV_LINK_STATE_ACTIVE
 * @synchronous   TRUE
 * @reentrant     FALSE
 * @trace         CPD-70667
 */
void EthTSyn_TrcvLinkStateChg(uint8 ctrlIdx, EthTrcv_LinkStateType trcvLinkState);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         Main function for cyclic call/resp. Sync, Follow_Up and Pdelay_Req transmissions.
 * @param[in]     portIndex: Index of the configured Port.
 * @synchronous   TRUE
 * @reentrant     FALSE
 * @trace         CPD-86494
 */
void EthTSyn_MainFunction(uint8 portIndex);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         This API is used to set FollowUp information TLV parameters of a Follow_Up message prior transmission.
 * @param[in]     timeBaseId: ID of the synchronized time base.
 * @param[in]     protocolParam: structure with Follow_Up information TLV parameters.
 * @synchronous   TRUE
 * @reentrant     FALSE
 * @trace         CPD-83052
 */
Std_ReturnType
    EthTSyn_SetProtocolParam(StbM_SynchronizedTimeBaseType timeBaseId, const StbM_ProtocolParamType* protocolParam);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         This API is used to read FollowUp information TLV parameters from received Follow_Upmessage.
 * @param[in]     timeBaseId: ID of the synchronized time base.
 * @param[out]    protocolParam: structure to store received Follow_Up information TLV parameters.
 * @synchronous   TRUE
 * @reentrant     FALSE
 * @trace         CPD-83053
 */
Std_ReturnType
    EthTSyn_GetProtocolParam(StbM_SynchronizedTimeBaseType timeBaseId, StbM_ProtocolParamType* protocolParam);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#if (ETHTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         By this API service the EthTSyn gets an indication and the result of ICV generation.
 * @param[in]     job: Job of the operation that caused the callback.
 * @param[in]     result: Contains the result of the cryptographic operation.
 * @synchronous   TRUE
 * @reentrant     TRUE
 * @trace         CPD-83054
 */
void EthTSyn_IcvGenerationIndication(Crypto_JobType* job, Crypto_ResultType result);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         By this API service the EthTSyn gets an indication and the result of ICV verification.
 * @param[in]     job: Job of the operation that caused the callback.
 * @param[in]     result: Contains the result of the cryptographic operation.
 * @synchronous   TRUE
 * @reentrant     TRUE
 * @trace         CPD-85829
 */
void EthTSyn_IcvVerificationIndication(Crypto_JobType* job, Crypto_ResultType result);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#endif

#ifdef __cplusplus
}
#endif

#endif
