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
 **  @file               : Com_Types.h
 **  @author             : shengnan.sun
 **  @date               : 2024/09/01
 **  @vendor             : isoft
 **  @description        : Type definitions of Com
 **
 ***********************************************************************************************************************/
#ifndef COM_TYPES_H_
#define COM_TYPES_H_

/* =================================================== inclusions =================================================== */
#include "Com_Cfg.h"
#include "Com_PBcfg.h"
#include "ComStack_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */
#if ((COM_MAX_RXIPDUGROUP_NUMBER <= 0xFFu) && (COM_MAX_RXIPDUGROUP_NUMBER >= 0u))
typedef uint8 Com_RxIpduGroupIdType;
#elif ((COM_MAX_RXIPDUGROUP_NUMBER <= 0xFFFFu) && (COM_MAX_RXIPDUGROUP_NUMBER > 0xFFu))
typedef uint16 Com_RxIpduGroupIdType;
#elif ((COM_MAX_RXIPDUGROUP_NUMBER <= 0xFFFFFFFFu) && (COM_MAX_RXIPDUGROUP_NUMBER > 0xFFFFu))
typedef uint32 Com_RxIpduGroupIdType;
#else
#error "Error Type"
#endif

#if ((COM_MAX_TXIPDUGROUP_NUMBER <= 0xFFu) && (COM_MAX_TXIPDUGROUP_NUMBER >= 0u))
typedef uint8 Com_TxIpduGroupIdType;
#elif ((COM_MAX_TXIPDUGROUP_NUMBER <= 0xFFFFu) && (COM_MAX_TXIPDUGROUP_NUMBER > 0xFFu))
typedef uint16 Com_TxIpduGroupIdType;
#elif ((COM_MAX_TXIPDUGROUP_NUMBER <= 0xFFFFFFFFu) && (COM_MAX_TXIPDUGROUP_NUMBER > 0xFFFFu))
typedef uint32 Com_TxIpduGroupIdType;
#else
#error "Error Type"
#endif

/* PRQA S 1252 ++ */ /* VL_QAC_1252 */
#if (                                                                                \
    defined(COM_MAX_RXSIG_TIMEOUT_NUMBER) && (COM_MAX_RXSIG_TIMEOUT_NUMBER <= 0xFFu) \
    && (COM_MAX_RXSIG_TIMEOUT_NUMBER >= 0u))
typedef uint8 Com_RxSigTimeoutIdType;
#elif (                                                                                \
    defined(COM_MAX_RXSIG_TIMEOUT_NUMBER) && (COM_MAX_RXSIG_TIMEOUT_NUMBER <= 0xFFFFu) \
    && (COM_MAX_RXSIG_TIMEOUT_NUMBER > 0xFFu))
typedef uint16 Com_RxSigTimeoutIdType;
#elif (                                                                                    \
    defined(COM_MAX_RXSIG_TIMEOUT_NUMBER) && (COM_MAX_RXSIG_TIMEOUT_NUMBER <= 0xFFFFFFFFu) \
    && (COM_MAX_RXSIG_TIMEOUT_NUMBER > 0xFFFFu))
typedef uint32 Com_RxSigTimeoutIdType;
#endif

#if (                                                                                      \
    defined(COM_MAX_RXSIGGRP_TIMEOUT_NUMBER) && (COM_MAX_RXSIGGRP_TIMEOUT_NUMBER <= 0xFFu) \
    && (COM_MAX_RXSIGGRP_TIMEOUT_NUMBER >= 0u))
typedef uint8 Com_RxSigGrpTimeoutIdType;
#elif (                                                                                      \
    defined(COM_MAX_RXSIGGRP_TIMEOUT_NUMBER) && (COM_MAX_RXSIGGRP_TIMEOUT_NUMBER <= 0xFFFFu) \
    && (COM_MAX_RXSIGGRP_TIMEOUT_NUMBER > 0xFFu))
typedef uint16 Com_RxSigGrpTimeoutIdType;
#elif (                                                                                          \
    defined(COM_MAX_RXSIGGRP_TIMEOUT_NUMBER) && (COM_MAX_RXSIGGRP_TIMEOUT_NUMBER <= 0xFFFFFFFFu) \
    && (COM_MAX_RXSIGGRP_TIMEOUT_NUMBER > 0xFFFFu))
typedef uint32 Com_RxSigGrpTimeoutIdType;
#endif
/* PRQA S 1252 -- */

#if ((COM_MAX_GW_SOURCE_DESCRIPTION_NUMBER <= 0xFFu) && (COM_MAX_GW_SOURCE_DESCRIPTION_NUMBER >= 0u))
typedef uint8 Com_GwSourceDescriptionIdType;
#elif ((COM_MAX_GW_SOURCE_DESCRIPTION_NUMBER <= 0xFFFFu) && (COM_MAX_GW_SOURCE_DESCRIPTION_NUMBER > 0xFFu))
typedef uint16 Com_GwSourceDescriptionIdType;
#elif ((COM_MAX_GW_SOURCE_DESCRIPTION_NUMBER <= 0xFFFFFFFFu) && (COM_MAX_GW_SOURCE_DESCRIPTION_NUMBER > 0xFFFFu))
typedef uint32 Com_GwSourceDescriptionIdType;
#else
#error "Error Type"
#endif

#if ((COM_MAX_RXSIGNAL_NUMBER <= 0xFFu) && (COM_MAX_RXSIGNAL_NUMBER >= 0u))
typedef uint8 Com_RxSignalIdType;
#elif ((COM_MAX_RXSIGNAL_NUMBER <= 0xFFFFu) && (COM_MAX_RXSIGNAL_NUMBER > 0xFFu))
typedef uint16 Com_RxSignalIdType;
#elif ((COM_MAX_RXSIGNAL_NUMBER <= 0xFFFFFFFFu) && (COM_MAX_RXSIGNAL_NUMBER > 0xFFFFu))
typedef uint32 Com_RxSignalIdType;
#else
#error "Error Type"
#endif

#if ((COM_MAX_RXSIGNALGROUP_NUMBER <= 0xFFu) && (COM_MAX_RXSIGNALGROUP_NUMBER >= 0u))
typedef uint8 Com_RxSignalGroupIdType;
#elif ((COM_MAX_RXSIGNALGROUP_NUMBER <= 0xFFFFu) && (COM_MAX_RXSIGNALGROUP_NUMBER > 0xFFu))
typedef uint16 Com_RxSignalGroupIdType;
#elif ((COM_MAX_RXSIGNALGROUP_NUMBER <= 0xFFFFFFFFu) && (COM_MAX_RXSIGNALGROUP_NUMBER > 0xFFFFu))
typedef uint32 Com_RxSignalGroupIdType;
#else
#error "Error Type"
#endif

#if ((COM_MAX_GW_DESTINATION_DESCRIPTION_NUMBER <= 0xFFu) && (COM_MAX_GW_DESTINATION_DESCRIPTION_NUMBER >= 0u))
typedef uint8 Com_GwDestinationDescriptionIdType;
#elif ((COM_MAX_GW_DESTINATION_DESCRIPTION_NUMBER <= 0xFFFFu) && (COM_MAX_GW_DESTINATION_DESCRIPTION_NUMBER > 0xFFu))
typedef uint16 Com_GwDestinationDescriptionIdType;
#elif (                                                        \
    (COM_MAX_GW_DESTINATION_DESCRIPTION_NUMBER <= 0xFFFFFFFFu) \
    && (COM_MAX_GW_DESTINATION_DESCRIPTION_NUMBER > 0xFFFFu))
typedef uint32 Com_GwDestinationDescriptionIdType;
#else
#error "Error Type"
#endif

#if ((COM_MAX_TXSIGNAL_NUMBER <= 0xFFu) && (COM_MAX_TXSIGNAL_NUMBER >= 0u))
typedef uint8 Com_TxSignalIdType;
#elif ((COM_MAX_TXSIGNAL_NUMBER <= 0xFFFFu) && (COM_MAX_TXSIGNAL_NUMBER > 0xFFu))
typedef uint16 Com_TxSignalIdType;
#elif ((COM_MAX_TXSIGNAL_NUMBER <= 0xFFFFFFFFu) && (COM_MAX_TXSIGNAL_NUMBER > 0xFFFFu))
typedef uint32 Com_TxSignalIdType;
#else
#error "Error Type"
#endif

#if ((COM_MAX_TXSIGNALGROUP_NUMBER <= 0xFFu) && (COM_MAX_TXSIGNALGROUP_NUMBER >= 0u))
typedef uint8 Com_TxSignalGroupIdType;
#elif ((COM_MAX_TXSIGNALGROUP_NUMBER <= 0xFFFFu) && (COM_MAX_TXSIGNALGROUP_NUMBER > 0xFFu))
typedef uint16 Com_TxSignalGroupIdType;
#elif ((COM_MAX_TXSIGNALGROUP_NUMBER <= 0xFFFFFFFFu) && (COM_MAX_TXSIGNALGROUP_NUMBER > 0xFFFFu))
typedef uint32 Com_TxSignalGroupIdType;
#else
#error "Error Type"
#endif

#if ((COM_MAX_RXSIGNAL_INVALID_NUMBER <= 0xFFu) && (COM_MAX_RXSIGNAL_INVALID_NUMBER >= 0u))
typedef uint8 Com_RxSignalInvalidIdType;
#elif ((COM_MAX_RXSIGNAL_INVALID_NUMBER <= 0xFFFFu) && (COM_MAX_RXSIGNAL_INVALID_NUMBER > 0xFFu))
typedef uint16 Com_RxSignalInvalidIdType;
#elif ((COM_MAX_RXSIGNAL_INVALID_NUMBER <= 0xFFFFFFFFu) && (COM_MAX_RXSIGNAL_INVALID_NUMBER > 0xFFFFu))
typedef uint32 Com_RxSignalInvalidIdType;
#else
#error "Error Type"
#endif

#if ((COM_MAX_RXSIGNAL_FILTERTYPE_MAX_NUMBER <= 0xFFu) && (COM_MAX_RXSIGNAL_FILTERTYPE_MAX_NUMBER >= 0u))
typedef uint8 Com_RxSignalFilterIdType;
#elif ((COM_MAX_RXSIGNAL_FILTERTYPE_MAX_NUMBER <= 0xFFFFu) && (COM_MAX_RXSIGNAL_FILTERTYPE_MAX_NUMBER > 0xFFu))
typedef uint16 Com_RxSignalFilterIdType;
#elif ((COM_MAX_RXSIGNAL_FILTERTYPE_MAX_NUMBER <= 0xFFFFFFFFu) && (COM_MAX_RXSIGNAL_FILTERTYPE_MAX_NUMBER > 0xFFFFu))
typedef uint32 Com_RxSignalFilterIdType;
#else
#error "Error Type"
#endif

typedef PduLengthType Com_SignalPositionType;

#if ((COM_MAX_TXSIGNAL_INVALID_NUMBER <= 0xFFu) && (COM_MAX_TXSIGNAL_INVALID_NUMBER >= 0u))
typedef uint8 Com_TxSignalInvalidIdType;
#elif ((COM_MAX_TXSIGNAL_INVALID_NUMBER <= 0xFFFFu) && (COM_MAX_TXSIGNAL_INVALID_NUMBER > 0xFFu))
typedef uint16 Com_TxSignalInvalidIdType;
#elif ((COM_MAX_TXSIGNAL_INVALID_NUMBER <= 0xFFFFFFFFu) && (COM_MAX_TXSIGNAL_INVALID_NUMBER > 0xFFFFu))
typedef uint32 Com_TxSignalInvalidIdType;
#else
#error "Error Type"
#endif

#if ((COM_MAX_TXSIGNAL_FILTERTYPE_MAX_NUMBER <= 0xFFu) && (COM_MAX_TXSIGNAL_FILTERTYPE_MAX_NUMBER >= 0u))
typedef uint8 Com_TxSignalFilterIdType;
#elif ((COM_MAX_TXSIGNAL_FILTERTYPE_MAX_NUMBER <= 0xFFFFu) && (COM_MAX_TXSIGNAL_FILTERTYPE_MAX_NUMBER > 0xFFu))
typedef uint16 Com_TxSignalFilterIdType;
#elif ((COM_MAX_TXSIGNAL_FILTERTYPE_MAX_NUMBER <= 0xFFFFFFFFu) && (COM_MAX_TXSIGNAL_FILTERTYPE_MAX_NUMBER > 0xFFFFu))
typedef uint32 Com_TxSignalFilterIdType;
#else
#error "Error Type"
#endif

#if ((COM_MAX_TXSIG_ERROR_AND_NOTIFY_NUMBER <= 0xFFu) && (COM_MAX_TXSIG_ERROR_AND_NOTIFY_NUMBER >= 0u))
typedef uint8 Com_TxSigErrAndNotifyIdType;
#elif ((COM_MAX_TXSIG_ERROR_AND_NOTIFY_NUMBER <= 0xFFFFu) && (COM_MAX_TXSIG_ERROR_AND_NOTIFY_NUMBER > 0xFFu))
typedef uint16 Com_TxSigErrAndNotifyIdType;
#elif ((COM_MAX_TXSIG_ERROR_AND_NOTIFY_NUMBER <= 0xFFFFFFFFu) && (COM_MAX_TXSIG_ERROR_AND_NOTIFY_NUMBER > 0xFFFFu))
typedef uint32 Com_TxSigErrAndNotifyIdType;
#else
#error "Error Type"
#endif

#if ((COM_MAX_TXSIGNAL_TIMEOUT_NUMBER <= 0xFFu) && (COM_MAX_TXSIGNAL_TIMEOUT_NUMBER >= 0u))
typedef uint8 Com_TxSignalTimeoutIdType;
#elif ((COM_MAX_TXSIGNAL_TIMEOUT_NUMBER <= 0xFFFFu) && (COM_MAX_TXSIGNAL_TIMEOUT_NUMBER > 0xFFu))
typedef uint16 Com_TxSignalTimeoutIdType;
#elif ((COM_MAX_TXSIGNAL_TIMEOUT_NUMBER <= 0xFFFFFFFFu) && (COM_MAX_TXSIGNAL_TIMEOUT_NUMBER > 0xFFFFu))
typedef uint32 Com_TxSignalTimeoutIdType;
#else
#error "Error Type"
#endif

#if ((COM_MAX_RXGROUPSIGNAL_NUMBER <= 0xFFu) && (COM_MAX_RXGROUPSIGNAL_NUMBER >= 0u))
typedef uint8 Com_RxGroupSignalIdType;
#elif ((COM_MAX_RXGROUPSIGNAL_NUMBER <= 0xFFFFu) && (COM_MAX_RXGROUPSIGNAL_NUMBER > 0xFFu))
typedef uint16 Com_RxGroupSignalIdType;
#elif ((COM_MAX_RXGROUPSIGNAL_NUMBER <= 0xFFFFFFFFu) && (COM_MAX_RXGROUPSIGNAL_NUMBER > 0xFFFFu))
typedef uint32 Com_RxGroupSignalIdType;
#else
#error "Error Type"
#endif

#if ((COM_MAX_TXGROUPSIGNAL_NUMBER <= 0xFFu) && (COM_MAX_TXGROUPSIGNAL_NUMBER >= 0u))
typedef uint8 Com_TxGroupSignalIdType;
#elif ((COM_MAX_TXGROUPSIGNAL_NUMBER <= 0xFFFFu) && (COM_MAX_TXGROUPSIGNAL_NUMBER > 0xFFu))
typedef uint16 Com_TxGroupSignalIdType;
#elif ((COM_MAX_TXGROUPSIGNAL_NUMBER <= 0xFFFFFFFFu) && (COM_MAX_TXGROUPSIGNAL_NUMBER > 0xFFFFu))
typedef uint32 Com_TxGroupSignalIdType;
#else
#error "Error Type"
#endif

#if ((COM_MAX_TXSIGGRP_ERROR_AND_NOTIFY_NUMBER <= 0xFFu) && (COM_MAX_TXSIGGRP_ERROR_AND_NOTIFY_NUMBER >= 0u))
typedef uint8 Com_TxSigGrpErrAndNotifyIdType;
#elif ((COM_MAX_TXSIGGRP_ERROR_AND_NOTIFY_NUMBER <= 0xFFFFu) && (COM_MAX_TXSIGGRP_ERROR_AND_NOTIFY_NUMBER > 0xFFu))
typedef uint16 Com_TxSigGrpErrAndNotifyIdType;
#elif ( \
    (COM_MAX_TXSIGGRP_ERROR_AND_NOTIFY_NUMBER <= 0xFFFFFFFFu) && (COM_MAX_TXSIGGRP_ERROR_AND_NOTIFY_NUMBER > 0xFFFFu))
typedef uint32 Com_TxSigGrpErrAndNotifyIdType;
#else
#error "Error Type"
#endif

#if ((COM_MAX_TXSIGGRP_TIMEOUT_NUMBER <= 0xFFu) && (COM_MAX_TXSIGGRP_TIMEOUT_NUMBER >= 0u))
typedef uint8 Com_TxSigGrpTimeoutIdType;
#elif ((COM_MAX_TXSIGGRP_TIMEOUT_NUMBER <= 0xFFFFu) && (COM_MAX_TXSIGGRP_TIMEOUT_NUMBER > 0xFFu))
typedef uint16 Com_TxSigGrpTimeoutIdType;
#elif ((COM_MAX_TXSIGGRP_TIMEOUT_NUMBER <= 0xFFFFFFFFu) && (COM_MAX_TXSIGGRP_TIMEOUT_NUMBER > 0xFFFFu))
typedef uint32 Com_TxSigGrpTimeoutIdType;
#else
#error "Error Type"
#endif

#if ((COM_MAX_RXGRPSIG_FILTERTYPE_MAX_NUMBER <= 0xFFu) && (COM_MAX_RXGRPSIG_FILTERTYPE_MAX_NUMBER >= 0u))
typedef uint8 Com_RxGrpSigFilterIdType;
#elif ((COM_MAX_RXGRPSIG_FILTERTYPE_MAX_NUMBER <= 0xFFFFu) && (COM_MAX_RXGRPSIG_FILTERTYPE_MAX_NUMBER > 0xFFu))
typedef uint16 Com_RxGrpSigFilterIdType;
#elif ((COM_MAX_RXGRPSIG_FILTERTYPE_MAX_NUMBER <= 0xFFFFFFFFu) && (COM_MAX_RXGRPSIG_FILTERTYPE_MAX_NUMBER > 0xFFFFu))
typedef uint32 Com_RxGrpSigFilterIdType;
#else
#error "Error Type"
#endif

#if ((COM_MAX_TXGRPSIG_FILTERTYPE_MAX_NUMBER <= 0xFFu) && (COM_MAX_TXGRPSIG_FILTERTYPE_MAX_NUMBER >= 0u))
typedef uint8 Com_TxGrpSigFilterIdType;
#elif ((COM_MAX_TXGRPSIG_FILTERTYPE_MAX_NUMBER <= 0xFFFFu) && (COM_MAX_TXGRPSIG_FILTERTYPE_MAX_NUMBER > 0xFFu))
typedef uint16 Com_TxGrpSigFilterIdType;
#elif ((COM_MAX_TXGRPSIG_FILTERTYPE_MAX_NUMBER <= 0xFFFFFFFFu) && (COM_MAX_TXGRPSIG_FILTERTYPE_MAX_NUMBER > 0xFFFFu))
typedef uint32 Com_TxGrpSigFilterIdType;
#else
#error "Error Type"
#endif

#if ((COM_MAX_DESTSIG_FILTERTYPE_MAX_NUMBER <= 0xFFu) && (COM_MAX_DESTSIG_FILTERTYPE_MAX_NUMBER >= 0u))
typedef uint8 Com_DestSignalFilterIdType;
#elif ((COM_MAX_DESTSIG_FILTERTYPE_MAX_NUMBER <= 0xFFFFu) && (COM_MAX_DESTSIG_FILTERTYPE_MAX_NUMBER > 0xFFu))
typedef uint16 Com_DestSignalFilterIdType;
#elif ((COM_MAX_DESTSIG_FILTERTYPE_MAX_NUMBER <= 0xFFFFFFFFu) && (COM_MAX_DESTSIG_FILTERTYPE_MAX_NUMBER > 0xFFFFu))
typedef uint32 Com_DestSignalFilterIdType;
#else
#error "Error Type"
#endif

#if ((COM_MAX_RXIPDUBUFF_SIZE <= 0xFFu) && (COM_MAX_RXIPDUBUFF_SIZE >= 0u))
typedef uint8 Com_RxIpduBufIdType;
#elif ((COM_MAX_RXIPDUBUFF_SIZE <= 0xFFFFu) && (COM_MAX_RXIPDUBUFF_SIZE > 0xFFu))
typedef uint16 Com_RxIpduBufIdType;
#elif ((COM_MAX_RXIPDUBUFF_SIZE <= 0xFFFFFFFFu) && (COM_MAX_RXIPDUBUFF_SIZE > 0xFFFFu))
typedef uint32 Com_RxIpduBufIdType;
#else
#error "Error Type"
#endif

#if ((COM_MAX_TXIPDUBUFF_SIZE <= 0xFFu) && (COM_MAX_TXIPDUBUFF_SIZE >= 0u))
typedef uint8 Com_TxIpduBufIdType;
#elif ((COM_MAX_TXIPDUBUFF_SIZE <= 0xFFFFu) && (COM_MAX_TXIPDUBUFF_SIZE > 0xFFu))
typedef uint16 Com_TxIpduBufIdType;
#elif ((COM_MAX_TXIPDUBUFF_SIZE <= 0xFFFFFFFFu) && (COM_MAX_TXIPDUBUFF_SIZE > 0xFFFFu))
typedef uint32 Com_TxIpduBufIdType;
#else
#error "Error Type"
#endif

#if ((COM_MAX_SIGNALGROUPARRY_NUMBER <= 0xFFu) && (COM_MAX_SIGNALGROUPARRY_NUMBER >= 0u))
typedef uint8 Com_SignalGroupArryIdType;
#elif ((COM_MAX_SIGNALGROUPARRY_NUMBER <= 0xFFFFu) && (COM_MAX_SIGNALGROUPARRY_NUMBER > 0xFFu))
typedef uint16 Com_SignalGroupArryIdType;
#elif ((COM_MAX_SIGNALGROUPARRY_NUMBER <= 0xFFFFFFFFu) && (COM_MAX_SIGNALGROUPARRY_NUMBER > 0xFFFFu))
typedef uint32 Com_SignalGroupArryIdType;
#else
#error "Error Type"
#endif

/**
 * @brief  AutoSAR COM Com_Module_Status_Type, Range COM_UNINIT/COM_INIT
 *
 */
typedef enum
{
    COM_UNINIT = 0,
    COM_INIT   = 1,
} Com_StatusType;

/**
 * @brief Defines the endianness of the signals network representation
 *
 */
typedef enum
{
    COM_BIG_ENDIAN    = 0,
    COM_LITTLE_ENDIAN = 1,
    COM_OPAQUE        = 2,
    COM_ENDIAN_NONE   = 3,
} Com_SignalEndiannessType;

/**
 * @brief The AUTOSAR type of the signal
 *
 */
typedef enum
{
    COM_BOOLEAN   = 0,
    COM_FLOAT32   = 1,
    COM_FLOAT64   = 2,
    COM_SINT16    = 3,
    COM_SINT32    = 4,
    COM_SINT64    = 5,
    COM_SINT8     = 6,
    COM_UINT16    = 7,
    COM_UINT32    = 8,
    COM_UINT64    = 9,
    COM_UINT8     = 10,
    COM_UINT8_DYN = 11,
    COM_UINT8_N   = 12,
} Com_SignalType;

/**
 * @brief Defines if a write access to this signal can trigger the transmission of the corresponding I-PDU
 *
 */
typedef enum
{
    COM_PENDING                                = 0,
    COM_TRIGGERED                              = 1,
    COM_TRIGGERED_ON_CHANGE                    = 2,
    COM_TRIGGERED_ON_CHANGE_WITHOUT_REPETITION = 3,
    COM_TRIGGERED_WITHOUT_REPETITION           = 4,
    COM_GROUPSIGNAL_TMC_NONE                   = 5,
} Com_TransferPropertyType;

/**
 * @brief Defines if a write access to this signal can trigger the transmission of the corresponding I-PDU
 *
 */
typedef enum
{
    COM_FILTER_NONE                   = 0,
    COM_ALWAYS                        = 1,
    COM_MASKED_NEW_DIFFERS_MASKED_OLD = 2,
    COM_MASKED_NEW_DIFFERS_X          = 3,
    COM_MASKED_NEW_EQUALS_X           = 4,
    COM_NEVER                         = 5,
    COM_NEW_IS_OUTSIDE                = 6,
    COM_NEW_IS_WITHIN                 = 7,
    COM_ONE_EVERY_N                   = 8,
} Com_FilterAlgorithmType;

/**
 * @brief Defines TxModeMode for TxIpdu
 *
 */
typedef enum
{
    COM_TX_MODE_DIRECT                    = 0,
    COM_TX_MODE_DIRECT_WITHOUT_REPETITION = 1,
    COM_TX_MODE_MIXED                     = 2,
    COM_TX_MODE_MIXED_WITHOUT_REPETITION  = 3,
    COM_TX_MODE_PERIODIC                  = 4,
    COM_TX_MODE_NONE                      = 5,
    COM_TX_MODE_NULL                      = 6,
} Com_TxModeModeType;

/**
 * @brief Defines of ClearUpdateBit Type for TxIPdu
 *
 */
typedef enum
{
    COM_CONFIRMATION     = 0,
    COM_TRANSMIT         = 1,
    COM_TRIGGER_TRANSMIT = 2,
    COM_CLEAR_UP_UNUSED  = 3,
} Com_TxIPduClearUpdateBitType;

/**
 * @brief Defines of the two modes Immediate and Deferred
 *
 */
typedef enum
{
    COM_DEFERRED  = 0,
    COM_IMMEDIATE = 1,
} Com_IPduSignalProcessingType;

/**
 * @brief Defines if this I-PDU is a normal I-PDU that can be sent unfragmented or if this is a large I-PDU that shall
 *        be sent via the Transport Protocol of the underlying bus.
 *
 */
typedef enum
{
    COM_PDU_NORMAL = 0,
    COM_PDU_TP     = 1,
} Com_IPduType;

/**
 * @brief Defines of the action performed upon reception of an invalid signal
 *
 */
typedef enum
{
    COM_INVALID_ACTION_NOTIFY  = 0,
    COM_INVALID_ACTION_REPLACE = 1,
    COM_INVALID_ACTION_NONE    = 2,
} Com_DataInvalidActionType;

/**
 * @brief Defines of the action performed upon expiration of the reception deadline monitoring timer
 *
 */
typedef enum
{
    COM_TIMEOUT_ACTION_NONE       = 0,
    COM_TIMEOUT_ACTION_REPLACE    = 1,
    COM_TIMEOUT_ACTION_SUBSTITUTE = 2,
} Com_RxDataTimeoutActionType;

/**
 * @brief Defines signal configuration type
 *
 */
typedef enum
{
    COM_TX_SIGNAL       = 0,
    COM_TX_GROUP_SIGNAL = 1,
    COM_TX_DESCRIPTION  = 2,
    COM_RX_SIGNAL       = 3,
    COM_RX_GROUP_SIGNAL = 4,
    COM_RX_DESCRIPTION  = 5,
    COM_RXTX_ERROR_TYPE = 6,
} Com_SignalConfigType;

/**
 * @brief AUTOSAR COM signal object identifier, Range 0-SingalIdMax
 *
 */
typedef uint16 Com_SignalIdType;

/**
 * @brief AUTOSAR COM signal group object identifier, Range 0-SingalGroupIdMax
 *
 */
typedef uint16 Com_SignalGroupIdType;

/**
 * @brief AUTOSAR COM IPdu group object identifier, Range 0-PduGroupIdMax
 *
 */
typedef uint16 Com_IpduGroupIdType;

/**
 * @brief Com mainfunction index of MainFunctionRx or MainFunctionTx or MainFunctionRoutes
 *
 */
typedef uint8 Com_MainFunctionIdType;

/**
 * @brief Each instance of this choice container allows to define one routing destination either by
 *        reference to an already configured COM signal / group signal or by a destination
 *        description container.
 *
 */
typedef struct
{
    Com_SignalIdType       GwDestSignalId;
    Com_MainFunctionIdType MainFunctionId;
    Com_SignalConfigType   GwDestSignalType;
} Com_GwDestType;

/**
 * @brief Each instance of this container defines one mapping of the integrated Signal Gateway
 *
 */
typedef struct
{
    Com_SignalIdType     GwSourceSignalId;
    Com_SignalIdType     GwDestSignalStartId;
    Com_SignalIdType     GwDestSignalEndId;
    Com_SignalConfigType GwSourceSignalType;
} Com_GwMappingType;

/**
 * @brief Description of a gateway source
 *
 */
typedef struct
{
    uint16 SigLsbBytePos;
    uint16 ComSignalLength;
    uint16 ComGwIPduRef;
    uint16 GWSignalBufferId;
#if (STD_ON == COM_GW_SRC_SIG_UPDATE_BIT_ENABLE)
    Com_SignalPositionType UpdateLsbBytePos;
    uint8                  UpdateBitMask;
#endif
    uint8                    SignalPosInByte;
    uint8                    ComBitSize;
    uint8                    ComSignalByteLength;
    Com_SignalEndiannessType ComSignalEndianness;
    Com_SignalType           ComSignalType;
#if (COM_MAX_GWMAPPING_NUMBER > 0u)
    Com_GwMappingIdType GwMappingId;
#endif
} Com_GwSourceSignalType;

/**
 * @brief This container contains the configuration parameters of the AUTOSAR COM modules Filters
 *
 */

typedef uint64 Com_FilterMaskType;

typedef uint64 Com_FilterXType;

typedef struct
{
    Com_FilterMaskType FilterMask;
    Com_FilterXType    FilterX;
} Com_FilterMaskNewDifEquXType;

typedef uint64 Com_FilterMaxType;
typedef uint64 Com_FilterMinType;

typedef struct
{
    Com_FilterMaxType FilterMax;
    Com_FilterMinType FilterMin;
} Com_FilterNewIsWithinOutsideType;

typedef struct
{
    Com_SignalIdType FilterOffset;
    Com_SignalIdType FilterPeriod;
} Com_FilterOneEveryNType;

/**
 * @brief Description of a gateway destination
 *
 */
typedef struct
{
    uint16 ComGwIPduRef;
    uint16 ComSignalLength;
    uint16 SignalInitValueId;
    uint16 SignalDataInitValueLength;
#if (COM_MAX_DESTSIG_FILTERTYPE_MAX_NUMBER > 0u)
    Com_SignalIdType TMCBufferId;
#endif
    Com_SignalPositionType SigLsbBytePos;
#if (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
    Com_SignalPositionType UpdateLsbBytePos;
#endif
#if (COM_MAX_DESTSIG_FILTERTYPE_MAX_NUMBER > 0u)
    Com_DestSignalFilterIdType FilterIndex;
#endif
    uint8 SignalPosInByte;
#if (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
    uint8 UpdateBitMask;
#endif
    uint8                    ComBitSize;
    uint8                    ComSignalByteLength;
    Com_SignalEndiannessType ComSignalEndianness;
    Com_TransferPropertyType ComTransferProperty;
#if (COM_MAX_DESTSIG_FILTERTYPE_MAX_NUMBER > 0u)
    Com_FilterAlgorithmType FilterAlgorithm;
#endif
    Com_SignalType ComSignalType;
} Com_GwDestSignalType;

/**
 * @brief Defines of notification or callout pointer
 *
 */
/* PRQA S 5004 ++ */ /* VL_Com_5004 */
typedef void (*Com_RxTimeoutNotificationType)(void);
typedef void (*Com_RxInvalidNotificationType)(void);
typedef boolean (*Com_RxIpduCalloutType)(PduIdType PduId, const PduInfoType* PduInfoPtr);
typedef boolean (*Com_TxIpduCalloutType)(PduIdType PduId, PduInfoType* PduInfoPtr);
typedef boolean (*Com_IPduTriggerTransmitCalloutType)(PduIdType PduId, PduInfoType* PduInfoPtr);
typedef void (*Com_IpduNotificationType)(void);
/* PRQA S 5004 -- */

/**
 * @brief Descriptions of AUTOSAR COM modules Rx I-PDUs
 *
 */
typedef struct
{
#if (STD_ON == COM_RX_IPDU_CALLOUT_ENABLE)
    Com_RxIpduCalloutType ComIPduCallout;
#endif
    PduLengthType          IPduNoDynSignalLength;
    PduLengthType          IPduMaxDynSignalLength;
    uint16                 RxIPduDMFirstTimeout;
    uint16                 RxIPduDMTimeout;
    uint16                 IpduPartitionId;
    Com_MainFunctionIdType RxMainfunctionId;
    Com_RxIpduBufIdType    RxIPduBufIndex;
    Com_RxSignalIdType     IpduSignalRefStartId;
    Com_RxSignalIdType     IPduSignalsRefEndId;
    Com_RxIpduGroupIdType  IPduGroupsRefStartId;
    Com_RxIpduGroupIdType  IPduGroupsRefEndId;
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE)
    Com_RxSigTimeoutIdType IPduSignalsTimeoutRefStartId;
    Com_RxSigTimeoutIdType IPduSignalsTimeoutRefEndId;
#endif
    Com_RxSignalGroupIdType IPduSignalGroupsRefStartId;
    Com_RxSignalGroupIdType IPduSignalGroupsRefEndId;
#if (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)
    Com_RxSigGrpTimeoutIdType IPduSignalGroupsTimeoutRefStartId;
    Com_RxSigGrpTimeoutIdType IPduSignalGroupsTimeoutRefEndId;
#if (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)
    Com_RxGroupSignalIdType GrpSigSubstituteStartId;
#endif
#endif
#if (COM_MAX_GW_SOURCE_DESCRIPTION_NUMBER > 0u)
    Com_GwSourceDescriptionIdType IPduSourceSignalsRefStartId;
    Com_GwSourceDescriptionIdType IPduSourceSignalsRefEndId;
#endif
    uint8 MetaDataLength;
#if (STD_ON == COM_CANCELLATION_SUPPORT)
    boolean ComIPduCancellationSupport;
#endif
    Com_IPduSignalProcessingType ComIPduSignalProcessing;
    Com_IPduType                 ComIPduType;
} Com_RxIPduType;

/**
 * @brief This container contains the configuration parameters of the AUTOSAR COM modules transmission modes
 *
 */
typedef uint8  Com_TxModeNumberOfRepetitionsType;
typedef uint16 Com_TxModeRepetitionPeriodType;
typedef uint16 Com_TxModeTimeOffsetType;
typedef uint16 Com_TxModeTimePeriodType;

typedef struct
{
    Com_TxModeTimeOffsetType TxModeTimeOffset;
    Com_TxModeTimePeriodType TxModeTimePeriod;
} Com_TxModePeriodType;

typedef struct
{
    Com_TxModeNumberOfRepetitionsType TxModeNumberOfRepetitions;
    Com_TxModeRepetitionPeriodType    TxModeRepetitionPeriod;
} Com_TxModeDirectRepetitionsType;

typedef struct
{
    Com_TxModeTimeOffsetType          TxModeTimeOffset;
    Com_TxModeTimePeriodType          TxModeTimePeriod;
    Com_TxModeRepetitionPeriodType    TxModeRepetitionPeriod;
    Com_TxModeNumberOfRepetitionsType TxModeNumberOfRepetitions;
} Com_TxModeMixedRepetitionsType;

/**
 * @brief Descriptions of AUTOSAR COM modules Tx I-PDUs
 *
 */
typedef struct
{
#if (STD_ON == COM_TX_IPDU_CALLOUT_ENABLE)
    Com_TxIpduCalloutType ComIPduCallout;
#endif
#if (COM_TXIPDU_TIGGERTRANSMIT_CALLOUT_ENABLE > 0u)
    Com_IPduTriggerTransmitCalloutType ComIPduTriggerTransmitCallout;
#endif
    PduLengthType           IPduNoDynSignalLength;
    PduLengthType           IPduMaxDynSignalLength;
    uint16                  TxIpduDM;
    uint16                  ComMinimumDelayTime;
    uint16                  IpduPartitionId;
    Com_MainFunctionIdType  TxMainfunctionId;
    Com_TxIpduBufIdType     TxIPduBufIndex;
    PduIdType               ComPduIdRef;
    PduIdType               TxModeTrueRefId;
    PduIdType               TxModeFalseRefId;
    Com_TxSignalIdType      IpduSignalRefStartId;
    Com_TxSignalIdType      IPduSignalsRefEndId;
    Com_TxSignalGroupIdType IPduSignalGroupsRefStartId;
    Com_TxSignalGroupIdType IPduSignalGroupsRefEndId;
    Com_TxIpduGroupIdType   IPduGroupsRefStartId;
    Com_TxIpduGroupIdType   IPduGroupsRefEndId;
#if (COM_MAX_GW_DESTINATION_DESCRIPTION_NUMBER > 0u)
    Com_GwDestinationDescriptionIdType IPduDespSignalsRefStartId;
    Com_GwDestinationDescriptionIdType IPduDespSignalsRefNumber;
#endif
    Com_TxModeModeType TxTrueModeMode;
    Com_TxModeModeType TxFalseModeMode;
    uint8              MetaDataLength;
#if (STD_ON == COM_CANCELLATION_SUPPORT)
    boolean ComIPduCancellationSupport;
#endif
#if (                                                                                             \
    (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE) || (STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE) \
    || (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE))
    Com_TxIPduClearUpdateBitType ComTxIPduClearUpdateBit;
#endif
    Com_IPduSignalProcessingType ComIPduSignalProcessing;
    Com_IPduType                 ComIPduType;
} Com_TxIPduType;

/**
 * @brief Descriptions of AUTOSAR COM module Rx signals
 *
 */
typedef struct
{
#if (STD_ON == COM_RX_SIGNAL_NOTIFICATION_ENABLE)
    Com_IpduNotificationType ComNotification;
#endif
    uint16                 SignalInitValueId;
    uint16                 ComSignalLength;
    uint16                 SignalDataInitValueLength;
    uint16                 GWSignalBufferId;
    Com_MainFunctionIdType RxMainfunctionId;
    Com_SignalPositionType SigLsbBytePos;
#if (STD_ON == COM_RX_SIGNAL_UPDATE_BIT_ENABLE)
    Com_SignalPositionType UpdateLsbBytePos;
#endif
    PduIdType IpduRefIndex;
#if (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ENABLE)
    Com_RxSignalInvalidIdType SignalInvalidRefId;
#endif
#if (COM_MAX_RXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)
    Com_RxSignalFilterIdType FilterIndex;
#endif
#if (STD_ON == COM_RX_SIGNAL_UPDATE_BIT_ENABLE)
    uint8 UpdateBitMask;
#endif
    uint8                    SignalPosInByte;
    uint8                    ComBitSize;
    uint8                    ComSignalByteLength;
    Com_SignalType           ComSignalType;
    Com_SignalEndiannessType ComSignalEndianness;
#if (COM_MAX_RXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)
    Com_FilterAlgorithmType FilterAlgorithm;
#endif
#if (COM_MAX_GWMAPPING_NUMBER > 0u)
    Com_GwMappingIdType GwMappingId;
#endif
} Com_RxSignalType;

/* PRQA S 5004 ++ */ /* VL_Com_5004 */
typedef void (*Com_TxTimeoutNotificationType)(void);
typedef void (*Com_ErrorAndNotificationType)(void);
/* PRQA S 5004 -- */
typedef uint8 Com_TxSignalNotifyType;

/**
 * @brief Descriptions of AUTOSAR COM modules Tx signals
 *
 */
typedef struct
{
    uint16                 SignalInitValueId;
    uint16                 ComSignalLength;
    uint16                 SignalDataInitValueLength;
    Com_MainFunctionIdType TxMainfunctionId;
#if (COM_MAX_TXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)
    Com_SignalIdType TMCBufferId;
#endif
    Com_SignalPositionType SigLsbBytePos;
#if (STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE)
    Com_SignalPositionType UpdateLsbBytePos;
#endif
    PduIdType IpduRefIndex;
#if (STD_ON == COM_TX_SIGNAL_INVALID_DATA_ENABLE)
    Com_TxSignalInvalidIdType SignalInvalidRefId;
#endif
#if (COM_MAX_TXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)
    Com_TxSignalFilterIdType FilterIndex;
#endif
#if (STD_ON == COM_TX_SIGNAL_TIMEOUT_ENABLE)
    Com_TxSignalTimeoutIdType TimeoutIndex;
#endif
#if (COM_MAX_TXSIG_ERROR_AND_NOTIFY_NUMBER > 0u)
    Com_TxSigErrAndNotifyIdType TxSignalErrAndNotifyFncId;
#endif
    uint8 SignalPosInByte;
    uint8 ComBitSize;
    uint8 ComSignalByteLength;
#if (STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE)
    uint8 UpdateBitMask;
#endif
    boolean                  ComInitialValueOnly;
    Com_SignalEndiannessType ComSignalEndianness;
    Com_SignalType           ComSignalType;
    Com_TransferPropertyType ComTransferProperty;
#if (COM_MAX_TXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)
    Com_FilterAlgorithmType FilterAlgorithm;
#endif
#if (COM_MAX_TXSIG_ERROR_AND_NOTIFY_NUMBER > 0u)
    Com_TxSignalNotifyType TxSignalNotificationType;
#endif
} Com_TxSignalType;

/**
 * @brief The uint8-array shall be used for this ComSignalGroup
 *
 */
typedef struct
{
    uint16 IPduStartBufIndex;
    uint16 SignalGroupArrayLength;
} Com_SignalGroupArrayType;

/**
 * @brief Descriptions of the AUTOSAR COM modules Rx signal groups
 *
 */
typedef struct
{
#if (STD_ON == COM_RX_SIG_GROUP_NOTIFICATION_ENABLE)
    Com_IpduNotificationType ComNotification;
#endif
#if (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE)
    Com_IpduNotificationType ComInvalidNotification;
#endif
    Com_MainFunctionIdType RxMainfunctionId;
#if (STD_ON == COM_RX_SIG_GROUP_UPDATE_BIT_ENABLE)
    Com_SignalPositionType UpdateLsbBytePos;
#endif
    Com_RxGroupSignalIdType GroupSignalRefStartId;
    Com_RxGroupSignalIdType GroupSignalRefEndId;
    PduIdType               IpduRefIndex;
#if (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE)
    Com_DataInvalidActionType ComDataInvalidAction;
#endif
#if (STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API)
    Com_SignalGroupArryIdType SignalGroupArrayId;
#endif
#if (STD_ON == COM_RX_SIG_GROUP_UPDATE_BIT_ENABLE)
    uint8 UpdateBitMask;
#endif
} Com_RxSignalGroupType;

/**
 * @brief Descriptions of the AUTOSAR COM modules Tx signal groups
 *
 */
typedef struct
{
    Com_MainFunctionIdType TxMainfunctionId;
#if (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE)
    Com_SignalPositionType UpdateLsbBytePos;
#endif
    Com_TxGroupSignalIdType GroupSignalRefStartId;
    Com_TxGroupSignalIdType GroupSignalRefEndId;
    PduIdType               IpduRefIndex;
#if (STD_ON == COM_TX_SIG_GROUP_TIMEOUT_ENABLE)
    Com_TxSigGrpTimeoutIdType TimeoutIndex;
#endif
#if (COM_MAX_TXSIGGRP_ERROR_AND_NOTIFY_NUMBER > 0u)
    Com_TxSigGrpErrAndNotifyIdType ComTxSigGrpErrAndNotifyFncId;
#endif
#if (STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API)
    Com_SignalGroupArryIdType SignalGroupArrayId;
#endif
#if (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE)
    uint8 UpdateBitMask;
#endif
#if (COM_MAX_TXSIGGRP_ERROR_AND_NOTIFY_NUMBER > 0u)
    Com_TxSignalNotifyType TxSignalNotificationType;
#endif
    boolean                  ComInitialValueOnly;
    Com_TransferPropertyType ComTransferProperty;
} Com_TxSignalGroupType;

/**
 * @brief Descriptions of the AUTOSAR COM modules Rx group signals
 *
 */
typedef struct
{
    uint16 SignalInitValueId;
    uint16 ComSignalLength;
    uint16 SignalDataInitValueLength;
    uint16 GWSignalBufferId;
#if (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE)
    uint16 ComSignalDataInvalidValueLength;
    uint16 ComSignalInvalidBuffRefId;
#endif
    Com_SignalPositionType  SigLsbBytePos;
    Com_RxSignalGroupIdType ComSignalGroupRef;
#if (COM_MAX_RXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
    Com_RxGrpSigFilterIdType FilterIndex;
#endif
    uint8                    SignalPosInByte;
    uint8                    ComBitSize;
    uint8                    ComSignalByteLength;
    Com_SignalEndiannessType ComSignalEndianness;
    Com_SignalType           ComSignalType;
#if (COM_MAX_RXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
    Com_FilterAlgorithmType FilterAlgorithm;
#endif
#if (COM_MAX_GWMAPPING_NUMBER > 0u)
    Com_GwMappingIdType GwMappingId;
#endif
} Com_RxGroupSignalType;

/**
 * @brief Descriptions of the AUTOSAR COM modules Tx group signals
 *
 */
typedef struct
{
    uint16 SignalInitValueId;
    uint16 ComSignalLength;
    uint16 SignalDataInitValueLength;
#if (STD_ON == COM_TX_GRP_SIGNAL_INVALID_DATA_ENABLE)
    uint16 ComSignalDataInvalidValueLength;
    uint16 ComSignalInvalidBuffRefId;
#endif
    Com_SignalPositionType SigLsbBytePos;
#if (COM_MAX_TXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
    Com_SignalIdType TMCBufferId;
#endif
    Com_TxSignalGroupIdType ComSignalGroupRef;
#if (COM_MAX_TXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
    Com_TxGrpSigFilterIdType FilterIndex;
#endif
    uint8                    SignalPosInByte;
    uint8                    ComBitSize;
    uint8                    ComSignalByteLength;
    Com_SignalEndiannessType ComSignalEndianness;
    Com_SignalType           ComSignalType;
#if (COM_MAX_TXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
    Com_FilterAlgorithmType FilterAlgorithm;
#endif
    Com_TransferPropertyType ComTransferProperty;
} Com_TxGroupSignalType;
typedef struct
{
    uint16        RxIpduLength;
    uint16        RxIpduBaseDMTimeout;
    PduLengthType RxOffset;
    /**
     * bit0 ActiveEnable;
     * bit1 DMEnable;
     * bit2 Receiving;
     * bit3 RxIndication;
     *
     */
    uint8 RxIpduRTStFlag;
    uint8 RxIpduDMFlag;
} Com_RxIPduRunTimeStateType;

typedef struct
{
    uint16 TxIpduLength;
#if (STD_ON == COM_MDT_ENABLE)
    uint16 MDTCnt;
#endif
    uint16        RptNum;
    uint16        NTimeCnt;
    uint16        DMCnt;
    uint16        PeriodCnt;
    PduLengthType TxOffset;
    /**
     *  bit0 ActiveEnable;
     *  bit1 TxConfirm;
     *  bit2 TMS;
     *  bit3 Transmiting;
     *  bit4 TxDelay;
     *
     */
    uint8              TxIpduRTStFlag;
    Com_TxModeModeType IpduTxMode;
} Com_TxIPduRunTimeStateType;

typedef struct
{
    uint16 DynamicSignalLength;
    /**
     * bit0 NTimesTransmitNeed;
     * bit1 WithoutRepetition;
     */
    uint8 TxSigGrpRTStFlag;
} Com_TxSignalGroupRunTimeStateType;

#if (STD_ON == COM_SIGNAL_INITVALUE_SUPPORT)
typedef struct
{
#if (COM_MAX_SIGNAL_BOOLBUFF_SIZE > 0u)
    const boolean* const* SignalBoolInitValue;
#endif
#if (COM_MAX_SIGNAL_8BITBUFF_SIZE > 0u)
    const uint8* const* Signal8BitInitValue;
#endif
#if (COM_MAX_SIGNAL_16BITBUFF_SIZE > 0u)
    const uint16* const* SignalBit16InitValue;
#endif
#if (COM_MAX_SIGNAL_32BITBUFF_SIZE > 0u)
    const uint32* const* Signal32BitInitValue;
#endif
#if (COM_MAX_SIGNAL_64BITBUFF_SIZE > 0u)
    const uint64* const* Signal64BitInitValue;
#endif
} Com_SignalInitValueType;

typedef struct
{
    uint16 bufferSize;
    uint16 rxGroupSignalEndId;
} Com_InitValueSignalIdType;

typedef struct
{
#if (COM_MAX_SIGNAL_BOOLBUFF_SIZE > 0u)
    const Com_InitValueSignalIdType* SignalBoolInitValueSize;
#endif
#if (COM_MAX_SIGNAL_8BITBUFF_SIZE > 0u)
    const Com_InitValueSignalIdType* Signal8BitInitValueSize;
#endif
#if (COM_MAX_SIGNAL_16BITBUFF_SIZE > 0u)
    const Com_InitValueSignalIdType* SignalBit16InitValueSize;
#endif
#if (COM_MAX_SIGNAL_32BITBUFF_SIZE > 0u)
    const Com_InitValueSignalIdType* Signal32BitInitValueSize;
#endif
#if (COM_MAX_SIGNAL_64BITBUFF_SIZE > 0u)
    const Com_InitValueSignalIdType* Signal64BitInitValueSize;
#endif
} Com_SigInitValueSizePerPartitionType;

#endif

#if (STD_ON == COM_SIGNAL_INVALID_VALUE_SUPPORT)
typedef struct
{
#if (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ENABLE)
    const uint16* RxSignalDataInvalidValueId;
#if (0u < COM_MAX_RX_SIGNAL_8BIT_INVALIDVALUE_NUMBER)
    const uint16* RxSignalDataInvalidValueLength;
#endif
    const Com_DataInvalidActionType* RxSignalDataInvalidAction;
#if (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ACTION_NOTIFY_ENABLE)
    const Com_RxInvalidNotificationType* RxInvalidNotificationCfg;
#endif
#endif
#if (STD_ON == COM_TX_SIGNAL_INVALID_DATA_ENABLE)
    const uint16* TxSignalDataInvalidValueId;
#if (0u < COM_MAX_TX_SIGNAL_8BIT_INVALIDVALUE_NUMBER)
    const uint16* TxSignalDataInvalidValueLength;
#endif
#endif
#if (COM_MAX_SIGNAL_BOOL_INVALID_SIZE > 0u)
    const boolean* SignalBoolInvalidValue;
#endif
#if (COM_MAX_SIGNAL_8BIT_INVALID_SIZE > 0u)
    const uint8* Signal8BitInvalidValue;
#endif
#if (COM_MAX_SIGNAL_16BIT_INVALID_SIZE > 0u)
    const uint16* SignalBit16InvalidValue;
#endif
#if (COM_MAX_SIGNAL_32BIT_INVALID_SIZE > 0u)
    const uint32* Signal32BitInvalidValue;
#endif
#if (COM_MAX_SIGNAL_64BIT_INVALID_SIZE > 0u)
    const uint64* Signal64BitInvalidValue;
#endif
} Com_SignalInvalidValueType;
#endif

#if (COM_MAX_TXSIG_ERROR_AND_NOTIFY_NUMBER > 0u) || (COM_MAX_TXSIGGRP_ERROR_AND_NOTIFY_NUMBER > 0u)
typedef struct
{
#if (COM_MAX_TXSIG_ERROR_AND_NOTIFY_NUMBER > 0u)
    const Com_ErrorAndNotificationType* TxSignalErrorAndNotification;
#endif
#if (COM_MAX_TXSIGGRP_ERROR_AND_NOTIFY_NUMBER > 0u)
    const Com_ErrorAndNotificationType* TxSignalGroupErrorAndNotification;
#endif
} Com_Tx_ErrorAndNotificationType;
#endif

#if (STD_ON == COM_SIGNAL_SUBSTITUE_VALUE_SUPPORT)
typedef struct
{
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)
    const uint16* RxSignalSubstituteValueId;
#endif
#if (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)
    const uint16* RxGrpSigSubstituteValueId;
#endif
#if (COM_MAX_SIGNAL_BOOL_SUBSTITUTE_SIZE > 0u)
    const boolean* SignalBoolSubstituteValue;
#endif
#if (COM_MAX_SIGNAL_8BIT_SUBSTITUTE_SIZE > 0u)
    const uint8* Signal8BitSubstituteValue;
#endif
#if (COM_MAX_SIGNAL_16BIT_SUBSTITUTE_SIZE > 0u)
    const uint16* SignalBit16SubstituteValue;
#endif
#if (COM_MAX_SIGNAL_32BIT_SUBSTITUTE_SIZE > 0u)
    const uint32* Signal32BitSubstituteValue;
#endif
#if (COM_MAX_SIGNAL_64BIT_SUBSTITUTE_SIZE > 0u)
    const uint64* Signal64BitSubstituteValue;
#endif
} Com_SignalSubstituteValueType;
#endif

#if (STD_ON == COM_TXMODE_AND_FILTER_SUPPORT)

#if ((COM_MAX_TMC_TX_ALL_TYPE_SIGNAL_NUMBER <= 0xFFu) && (COM_MAX_TMC_TX_ALL_TYPE_SIGNAL_NUMBER >= 0u))
typedef uint8 Com_TxTMCSizeType;
#elif ((COM_MAX_TMC_TX_ALL_TYPE_SIGNAL_NUMBER <= 0xFFFFu) && (COM_MAX_TMC_TX_ALL_TYPE_SIGNAL_NUMBER > 0xFFu))
typedef uint16 Com_TxTMCSizeType;
#elif ((COM_MAX_TMC_TX_ALL_TYPE_SIGNAL_NUMBER <= 0xFFFFFFFFu) && (COM_MAX_TMC_TX_ALL_TYPE_SIGNAL_NUMBER > 0xFFFFFFu))
typedef uint32 Com_TxTMCSizeType;
#else
#error "Error Type"
#endif

typedef struct
{
#if (0u < COM_MAX_TX_MODE_TRUE_PERIOD_NUMBER) || (0u < COM_MAX_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER)
    const Com_TxModePeriodType* TxModeTruePeriod;
#endif
#if (0u < COM_MAX_TX_MODE_FALSE_PERIOD_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER)
    const Com_TxModePeriodType* TxModeFalsePeriod;
#endif
#if (0u < COM_MAX_TX_MODE_TRUE_DIRECT_NUMBER)
    const Com_TxModeDirectRepetitionsType* TxModeTrueDirectRepetitions;
#endif
#if (0u < COM_MAX_TX_MODE_FALSE_DIRECT_NUMBER)
    const Com_TxModeDirectRepetitionsType* TxModeFalseDirectRepetitions;
#endif
#if (0u < COM_MAX_TX_MODE_TRUE_MIXED_NUMBER)
    const Com_TxModeMixedRepetitionsType* TxModeTrueMixedRepetitions;
#endif
#if (0u < COM_MAX_TX_MODE_FALSE_MIXED_NUMBER)
    const Com_TxModeMixedRepetitionsType* TxModeFalseMixedRepetitions;
#endif
#if (0u < COM_MAX_TMC_TX_ALL_TYPE_SIGNAL_NUMBER)
    const boolean* const*    TxSignalTMCInitVale;
    const Com_TxTMCSizeType* TxTMCSize;
#if (0u < COM_MAX_TXMASKNEWDIFFERMASKOLD_NUMBER)
    const Com_FilterMaskType* TxMaskedNewDiffersMaskedOld;
#endif
#if (0u < COM_MAX_TXMASKNEWDIFFERX_NUMBER)
    const Com_FilterMaskNewDifEquXType* TxMaskedNewDiffersX;
#endif
#if (0u < COM_MAX_TXMASKNEWEQUALSX_NUMBER)
    const Com_FilterMaskNewDifEquXType* TxMaskedNewEqualsX;
#endif
#if (0u < COM_MAX_TXNEWISOUTSIDE_NUMBER)
    const Com_FilterNewIsWithinOutsideType* TxNewIsOutside;
#endif
#if (0u < COM_MAX_TXNEWISWITHIN_NUMBER)
    const Com_FilterNewIsWithinOutsideType* TxNewIsWithin;
#endif
#endif
#if (0u < COM_MAX_RXMASKNEWDIFFERMASKOLD_NUMBER)
    const Com_FilterMaskType* RxMaskedNewDiffersMaskedOld;
#endif
#if (0u < COM_MAX_RXMASKNEWDIFFERX_NUMBER)
    const Com_FilterMaskNewDifEquXType* RxMaskedNewDiffersX;
#endif
#if (0u < COM_MAX_RXMASKNEWEQUALSX_NUMBER)
    const Com_FilterMaskNewDifEquXType* RxMaskedNewEqualsX;
#endif
#if (0u < COM_MAX_RXNEWISOUTSIDE_NUMBER)
    const Com_FilterNewIsWithinOutsideType* RxNewIsOutside;
#endif
#if (0u < COM_MAX_RXNEWISWITHIN_NUMBER)
    const Com_FilterNewIsWithinOutsideType* RxNewIsWithin;
#endif
#if (0u < COM_MAX_ONEEVERYNFILTERSIGNAL_NUMBER)
    const Com_FilterOneEveryNType* FilterOneEveryN;
#endif
} Com_TxModeAndSignalFilterPtrType;
#endif

#if (COM_MAX_RXIPDUGROUP_NUMBER > 0u) || (COM_MAX_TXIPDUGROUP_NUMBER > 0u)
typedef struct
{
    const Com_RxIpduGroupIdType* const RxIpduGroupRef;
    const Com_TxIpduGroupIdType* const TxIpduGroupRef;
} Com_IpduGroupRefType;
#endif

#if (STD_ON == COM_SIGNAL_TIMEOUT_SUPPORT)
typedef struct
{
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE)
    const uint16*                      RxSignalFirstTimeoutVal;
    const uint16*                      RxSignalTimeoutVal;
    const uint16*                      TimeoutRxSignalId;
    const Com_RxDataTimeoutActionType* RxSignalDataTimeoutActionCfg;
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_NOTIFICATION_ENABLE)
    const Com_RxTimeoutNotificationType* RxSignalTimeoutNotificationCfg;
#endif
#endif
#if (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)
    const uint16*                      RxSigGrpFirstTimeoutVal;
    const uint16*                      RxSigGrpTimeoutVal;
    const Com_RxSignalGroupIdType*     TimeoutRxSigGrpId;
    const Com_RxDataTimeoutActionType* RxSigGrpDataTimeoutActionCfg;
#if (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_NOTIFICATION_ENABLE)
    const Com_RxTimeoutNotificationType* RxSigGrpTimeoutNotificationCfg;
#endif
#endif
#if (STD_ON == COM_TX_SIGNAL_TIMEOUT_ENABLE)
    const uint16* TxSignalTimeoutVal;
#if (STD_ON == COM_TX_SIGNAL_TIMEOUT_NOTIFICATION_ENABLE)
    const Com_TxTimeoutNotificationType* TxSignalDataTimeoutNotificationCfg;
#endif
#endif
#if (STD_ON == COM_TX_SIG_GROUP_TIMEOUT_ENABLE)
    const uint16* TxSigGrpTimeoutVal;
#if (STD_ON == COM_TX_SIG_GROUP_TIMEOUT_NOTIFICATION_ENABLE)
    const Com_TxTimeoutNotificationType* TxSigGrpTimeoutNotificationCfg;
#endif
#endif
} Com_SignalTimeoutType;
#endif

#if (COM_MAX_GWMAPPING_NUMBER > 0u)
typedef struct
{
    const Com_GwMappingType* GwMapping;
    const Com_GwDestType*    GwDest;
#if (COM_MAX_GW_SOURCE_DESCRIPTION_NUMBER > 0u)
    const Com_GwSourceSignalType* SourceSignal;
#endif
#if (COM_MAX_GW_DESTINATION_DESCRIPTION_NUMBER > 0u)
    const Com_GwDestSignalType* DestSignal;
#endif
} Com_GwMappingConfigType;
#endif

typedef struct Com_ConfigTypeTag
{
#if (COM_MAX_RXIPDU_NUMBER > 0u)
    const Com_RxIPduType*                    RxIPduPtr;
    const Com_RxIPduRunTimeStateType* const* RxIpduInitStatePtr;
    const uint8* const*                      RxIpduInitValuePtr;
    const Com_RxIpduBufIdType* const         RxIpduBufferSizePerPartition;
#endif
#if (COM_MAX_TXIPDU_NUMBER > 0u)
    const Com_TxIPduType*                    TxIPduPtr;
    const Com_TxIPduRunTimeStateType* const* TxIpduInitStatePtr;
    const uint8* const*                      TxIpduInitValuePtr;
    const Com_TxIpduBufIdType* const         TxIpduBufferSizePerPartition;
#endif
    const Com_RxSignalType*      RxSignalPtr;
    const Com_TxSignalType*      TxSignalPtr;
    const Com_RxSignalGroupType* RxSignalGroupPtr;
    const Com_TxSignalGroupType* TxSignalGroupPtr;
    const Com_RxGroupSignalType* RxGroupSignalPtr;
    const Com_TxGroupSignalType* TxGroupSignalPtr;
    const PduIdType*             MainFunctionRxRangePtr;
    const PduIdType*             MainFunctionTxRangePtr;
#if (COM_MAX_RXIPDUGROUP_NUMBER > 0u) || (COM_MAX_TXIPDUGROUP_NUMBER > 0u)
    const Com_IpduGroupRefType* IpduGroupRefPtr;
#endif
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    const uint16* MainFuncRxPartitionRangePtr;
    const uint16* MainFuncTxPartitionRangePtr;
#endif
#if (COM_MAX_GWMAPPING_NUMBER > 0u)
    const Com_GwMappingConfigType* GwMappingConfigPtr;
#endif
#if (STD_ON == COM_SIGNAL_INITVALUE_SUPPORT)
    const Com_SignalInitValueType*              SignalInitValuePtr;
    const Com_SigInitValueSizePerPartitionType* SignalInitValueSizePtr;
#endif
#if (STD_ON == COM_SIGNAL_INVALID_VALUE_SUPPORT)
    const Com_SignalInvalidValueType* SignalInvalidValuePtr;
#endif
#if (COM_MAX_TXSIG_ERROR_AND_NOTIFY_NUMBER > 0u) || (COM_MAX_TXSIGGRP_ERROR_AND_NOTIFY_NUMBER > 0u)
    const Com_Tx_ErrorAndNotificationType* TxErrorAndNotificationPtr;
#endif
#if (STD_ON == COM_SIGNAL_SUBSTITUE_VALUE_SUPPORT)
    const Com_SignalSubstituteValueType* SignalSubstitueteValuePtr;
#endif
#if (STD_ON == COM_TXMODE_AND_FILTER_SUPPORT)
    const Com_TxModeAndSignalFilterPtrType* TxModeAndSignalFilterPtr;
#endif
#if (STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API)
    const Com_SignalGroupArrayType* SignalGroupArrayPtr;
#endif
#if (STD_ON == COM_SIGNAL_TIMEOUT_SUPPORT)
    const Com_SignalTimeoutType* SignalTimeoutPtr;
#endif
#if (COM_MULITIVARIANT_SUPPORT == STD_ON)
    const Com_IpduGroupIdType*   UserIdMapComIPduGrpIdPtr;      /** User id map to Com Ipdu Group Id */
    const uint16*                UserIdMapComIPduIdPtr;         /** User id map to Com IPdu Id */
    const Com_SignalIdType*      UserIdMapComSigAndGrpSigIdPtr; /** User id map to Com Signal and Com Group Signal Id */
    const Com_SignalGroupIdType* UserIdMapComSigGrpIdPtr;       /** User id map to Com SignalGroup Id */
#endif
    Com_SignalIdType       TxSignalNum;
    Com_SignalIdType       RxSignalNum;
    PduIdType              TxPduNum;
    PduIdType              RxPduNum;
    Com_SignalIdType       TxGroupSignalNum;
    Com_SignalIdType       RxGroupSignalNum;
    Com_SignalGroupIdType  TxSignalGroupNum;
    Com_SignalGroupIdType  RxSignalGroupNum;
    Com_IpduGroupIdType    IpduGroupNum;
    Com_MainFunctionIdType MainFunctionRouteSignalsNum;
    Com_MainFunctionIdType MainFunctionRxNum;
    Com_MainFunctionIdType MainFunctionTxNum;
#if COM_MAX_GWMAPPING_NUMBER > 0u
    Com_SignalIdType GwDestNum;
#endif
} Com_ConfigType;

#ifdef __cplusplus
}
#endif
#endif
