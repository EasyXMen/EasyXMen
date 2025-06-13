/* PRQA S 3108++ */
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
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : Com_Types.h                                                 **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Type definitions of COM                                     **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/
#ifndef COM_TYPES_H
#define COM_TYPES_H

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Com_Cfg.h"
#include "Com_PBcfg.h"
#include "ComStack_Types.h"

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/* Published information */
#if !defined(COM_PUBLISHED_INFORMATION)
#define COM_PUBLISHED_INFORMATION
#define COM_MODULE_ID                   50u
#define COM_VENDOR_ID                   62u
#define COM_AR_RELEASE_MAJOR_VERSION    4u
#define COM_AR_RELEASE_MINOR_VERSION    5u
#define COM_AR_RELEASE_REVISION_VERSION 0u
#define COM_SW_MAJOR_VERSION            2u
#define COM_SW_MINOR_VERSION            1u
#define COM_SW_PATCH_VERSION            25u
#elif ((COM_SW_MAJOR_VERSION != 2u) || (COM_SW_MINOR_VERSION != 1u))
#error "Com: Mismatch in Software Version"
#endif

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
#if ((COM_RXIPDU_COUNTER_NUMBER <= 0xFFu) && (COM_RXIPDU_COUNTER_NUMBER >= 0u))
typedef uint8 Com_RxIPduCounterIdType;
#elif ((COM_RXIPDU_COUNTER_NUMBER <= 0xFFFFu) && (COM_RXIPDU_COUNTER_NUMBER > 0xFFu))
typedef uint16 Com_RxIPduCounterIdType;
#elif ((COM_RXIPDU_COUNTER_NUMBER <= 0xFFFFFFu) && (COM_RXIPDU_COUNTER_NUMBER > 0xFFFFu))
typedef uint32 Com_RxIPduCounterIdType;
#else
#error "Error Type"
#endif

#if ((COM_RXIPDUGROUP_NUMBER <= 0xFFu) && (COM_RXIPDUGROUP_NUMBER >= 0u))
typedef uint8 Com_RxIpduGroupIdType;
#elif ((COM_RXIPDUGROUP_NUMBER <= 0xFFFFu) && (COM_RXIPDUGROUP_NUMBER > 0xFFu))
typedef uint16 Com_RxIpduGroupIdType;
#elif ((COM_RXIPDUGROUP_NUMBER <= 0xFFFFFFu) && (COM_RXIPDUGROUP_NUMBER > 0xFFFFu))
typedef uint32 Com_RxIpduGroupIdType;
#else
#error "Error Type"
#endif

#if ((COM_TXIPDUGROUP_NUMBER <= 0xFFu) && (COM_TXIPDUGROUP_NUMBER >= 0u))
typedef uint8 Com_TxIpduGroupIdType;
#elif ((COM_TXIPDUGROUP_NUMBER <= 0xFFFFu) && (COM_TXIPDUGROUP_NUMBER > 0xFFu))
typedef uint16 Com_TxIpduGroupIdType;
#elif ((COM_TXIPDUGROUP_NUMBER <= 0xFFFFFFu) && (COM_TXIPDUGROUP_NUMBER > 0xFFFFu))
typedef uint32 Com_TxIpduGroupIdType;
#else
#error "Error Type"
#endif

#if ((COM_RXSIG_TIMEOUT_NUMBER <= 0xFFu) && (COM_RXSIG_TIMEOUT_NUMBER >= 0u))
typedef uint8 Com_RxSigTimeoutIdType;
#elif ((COM_RXSIG_TIMEOUT_NUMBER <= 0xFFFFu) && (COM_RXSIG_TIMEOUT_NUMBER > 0xFFu))
typedef uint16 Com_RxSigTimeoutIdType;
#elif ((COM_RXSIG_TIMEOUT_NUMBER <= 0xFFFFFFu) && (COM_RXSIG_TIMEOUT_NUMBER > 0xFFFFu))
typedef uint32 Com_RxSigTimeoutIdType;
#else
#error "Error Type"
#endif

#if ((COM_RXSIGGRP_TIMEOUT_NUMBER <= 0xFFu) && (COM_RXSIGGRP_TIMEOUT_NUMBER >= 0u))
typedef uint8 Com_RxSigGrpTimeoutIdType;
#elif ((COM_RXSIGGRP_TIMEOUT_NUMBER <= 0xFFFFu) && (COM_RXSIGGRP_TIMEOUT_NUMBER > 0xFFu))
typedef uint16 Com_RxSigGrpTimeoutIdType;
#elif ((COM_RXSIGGRP_TIMEOUT_NUMBER <= 0xFFFFFFu) && (COM_RXSIGGRP_TIMEOUT_NUMBER > 0xFFFFu))
typedef uint32 Com_RxSigGrpTimeoutIdType;
#else
#error "Error Type"
#endif

#if ((COM_GW_SOURCE_DESCRIPTION_NUMBER <= 0xFFu) && (COM_GW_SOURCE_DESCRIPTION_NUMBER >= 0u))
typedef uint8 Com_GwSourceDescriptionIdType;
#elif ((COM_GW_SOURCE_DESCRIPTION_NUMBER <= 0xFFFFu) && (COM_GW_SOURCE_DESCRIPTION_NUMBER > 0xFFu))
typedef uint16 Com_GwSourceDescriptionIdType;
#elif ((COM_GW_SOURCE_DESCRIPTION_NUMBER <= 0xFFFFFFu) && (COM_GW_SOURCE_DESCRIPTION_NUMBER > 0xFFFFu))
typedef uint32 Com_GwSourceDescriptionIdType;
#else
#error "Error Type"
#endif

#if ((COM_RXSIGNAL_NUMBER <= 0xFFu) && (COM_RXSIGNAL_NUMBER >= 0u))
typedef uint8 Com_RxSignalIdType;
#elif ((COM_RXSIGNAL_NUMBER <= 0xFFFFu) && (COM_RXSIGNAL_NUMBER > 0xFFu))
typedef uint16 Com_RxSignalIdType;
#elif ((COM_RXSIGNAL_NUMBER <= 0xFFFFFFu) && (COM_RXSIGNAL_NUMBER > 0xFFFFu))
typedef uint32 Com_RxSignalIdType;
#else
#error "Error Type"
#endif

#if ((COM_RXSIGNALGROUP_NUMBER <= 0xFFu) && (COM_RXSIGNALGROUP_NUMBER >= 0u))
typedef uint8 Com_RxSignalGroupIdType;
#elif ((COM_RXSIGNALGROUP_NUMBER <= 0xFFFFu) && (COM_RXSIGNALGROUP_NUMBER > 0xFFu))
typedef uint16 Com_RxSignalGroupIdType;
#elif ((COM_RXSIGNALGROUP_NUMBER <= 0xFFFFFFu) && (COM_RXSIGNALGROUP_NUMBER > 0xFFFFu))
typedef uint32 Com_RxSignalGroupIdType;
#else
#error "Error Type"
#endif

#if ((COM_GW_DESTINATION_DESCRIPTION_NUMBER <= 0xFFu) && (COM_GW_DESTINATION_DESCRIPTION_NUMBER >= 0u))
typedef uint8 Com_GwDestinationDescriptionIdType;
#elif ((COM_GW_DESTINATION_DESCRIPTION_NUMBER <= 0xFFFFu) && (COM_GW_DESTINATION_DESCRIPTION_NUMBER > 0xFFu))
typedef uint16 Com_GwDestinationDescriptionIdType;
#elif ((COM_GW_DESTINATION_DESCRIPTION_NUMBER <= 0xFFFFFFu) && (COM_GW_DESTINATION_DESCRIPTION_NUMBER > 0xFFFFu))
typedef uint32 Com_GwDestinationDescriptionIdType;
#else
#error "Error Type"
#endif

#if ((COM_TXSIGNAL_NUMBER <= 0xFFu) && (COM_TXSIGNAL_NUMBER >= 0u))
typedef uint8 Com_TxSignalIdType;
#elif ((COM_TXSIGNAL_NUMBER <= 0xFFFFu) && (COM_TXSIGNAL_NUMBER > 0xFFu))
typedef uint16 Com_TxSignalIdType;
#elif ((COM_TXSIGNAL_NUMBER <= 0xFFFFFFu) && (COM_TXSIGNAL_NUMBER > 0xFFFFu))
typedef uint32 Com_TxSignalIdType;
#else
#error "Error Type"
#endif

#if ((COM_TXSIGNALGROUP_NUMBER <= 0xFFu) && (COM_TXSIGNALGROUP_NUMBER >= 0u))
typedef uint8 Com_TxSignalGroupIdType;
#elif ((COM_TXSIGNALGROUP_NUMBER <= 0xFFFFu) && (COM_TXSIGNALGROUP_NUMBER > 0xFFu))
typedef uint16 Com_TxSignalGroupIdType;
#elif ((COM_TXSIGNALGROUP_NUMBER <= 0xFFFFFFu) && (COM_TXSIGNALGROUP_NUMBER > 0xFFFFu))
typedef uint32 Com_TxSignalGroupIdType;
#else
#error "Error Type"
#endif

#if ((COM_TXIPDU_COUNTER_NUMBER <= 0xFFu) && (COM_TXIPDU_COUNTER_NUMBER >= 0u))
typedef uint8 Com_TxIPduCounterIdType;
#elif ((COM_TXIPDU_COUNTER_NUMBER <= 0xFFFFu) && (COM_TXIPDU_COUNTER_NUMBER > 0xFFu))
typedef uint16 Com_TxIPduCounterIdType;
#elif ((COM_TXIPDU_COUNTER_NUMBER <= 0xFFFFFFu) && (COM_TXIPDU_COUNTER_NUMBER > 0xFFFFu))
typedef uint32 Com_TxIPduCounterIdType;
#else
#error "Error Type"
#endif

#if ((COM_RXSIGNAL_INVALID_NUMBER <= 0xFFu) && (COM_RXSIGNAL_INVALID_NUMBER >= 0u))
typedef uint8 Com_RxSignalInvalidIdType;
#elif ((COM_RXSIGNAL_INVALID_NUMBER <= 0xFFFFu) && (COM_RXSIGNAL_INVALID_NUMBER > 0xFFu))
typedef uint16 Com_RxSignalInvalidIdType;
#elif ((COM_RXSIGNAL_INVALID_NUMBER <= 0xFFFFFFu) && (COM_RXSIGNAL_INVALID_NUMBER > 0xFFFFu))
typedef uint32 Com_RxSignalInvalidIdType;
#else
#error "Error Type"
#endif

#if ((COM_RXSIGNAL_FILTERTYPE_MAX_NUMBER <= 0xFFu) && (COM_RXSIGNAL_FILTERTYPE_MAX_NUMBER >= 0u))
typedef uint8 Com_RxSignalFilterIdType;
#elif ((COM_RXSIGNAL_FILTERTYPE_MAX_NUMBER <= 0xFFFFu) && (COM_RXSIGNAL_FILTERTYPE_MAX_NUMBER > 0xFFu))
typedef uint16 Com_RxSignalFilterIdType;
#elif ((COM_RXSIGNAL_FILTERTYPE_MAX_NUMBER <= 0xFFFFFFu) && (COM_RXSIGNAL_FILTERTYPE_MAX_NUMBER > 0xFFFFu))
typedef uint32 Com_RxSignalFilterIdType;
#else
#error "Error Type"
#endif

typedef PduLengthType Com_SignalPositionType;

#if ((COM_TXSIGNAL_INVALID_NUMBER <= 0xFFu) && (COM_TXSIGNAL_INVALID_NUMBER >= 0u))
typedef uint8 Com_TxSignalInvalidIdType;
#elif ((COM_TXSIGNAL_INVALID_NUMBER <= 0xFFFFu) && (COM_TXSIGNAL_INVALID_NUMBER > 0xFFu))
typedef uint16 Com_TxSignalInvalidIdType;
#elif ((COM_TXSIGNAL_INVALID_NUMBER <= 0xFFFFFFu) && (COM_TXSIGNAL_INVALID_NUMBER > 0xFFFFu))
typedef uint32 Com_TxSignalInvalidIdType;
#else
#error "Error Type"
#endif

#if ((COM_TXSIGNAL_FILTERTYPE_MAX_NUMBER <= 0xFFu) && (COM_TXSIGNAL_FILTERTYPE_MAX_NUMBER >= 0u))
typedef uint8 Com_TxSignalFilterIdType;
#elif ((COM_TXSIGNAL_FILTERTYPE_MAX_NUMBER <= 0xFFFFu) && (COM_TXSIGNAL_FILTERTYPE_MAX_NUMBER > 0xFFu))
typedef uint16 Com_TxSignalFilterIdType;
#elif ((COM_TXSIGNAL_FILTERTYPE_MAX_NUMBER <= 0xFFFFFFu) && (COM_TXSIGNAL_FILTERTYPE_MAX_NUMBER > 0xFFFFu))
typedef uint32 Com_TxSignalFilterIdType;
#else
#error "Error Type"
#endif

#if ((COM_TXSIG_ERROR_AND_NOTIFY_NUMBER <= 0xFFu) && (COM_TXSIG_ERROR_AND_NOTIFY_NUMBER >= 0u))
typedef uint8 Com_TxSigErrAndNotifyIdType;
#elif ((COM_TXSIG_ERROR_AND_NOTIFY_NUMBER <= 0xFFFFu) && (COM_TXSIG_ERROR_AND_NOTIFY_NUMBER > 0xFFu))
typedef uint16 Com_TxSigErrAndNotifyIdType;
#elif ((COM_TXSIG_ERROR_AND_NOTIFY_NUMBER <= 0xFFFFFFu) && (COM_TXSIG_ERROR_AND_NOTIFY_NUMBER > 0xFFFFu))
typedef uint32 Com_TxSigErrAndNotifyIdType;
#else
#error "Error Type"
#endif

#if ((COM_TXSIGNAL_TIMEOUT_NUMBER <= 0xFFu) && (COM_TXSIGNAL_TIMEOUT_NUMBER >= 0u))
typedef uint8 Com_TxSignalTimeoutIdType;
#elif ((COM_TXSIGNAL_TIMEOUT_NUMBER <= 0xFFFFu) && (COM_TXSIGNAL_TIMEOUT_NUMBER > 0xFFu))
typedef uint16 Com_TxSignalTimeoutIdType;
#elif ((COM_TXSIGNAL_TIMEOUT_NUMBER <= 0xFFFFFFu) && (COM_TXSIGNAL_TIMEOUT_NUMBER > 0xFFFFu))
typedef uint32 Com_TxSignalTimeoutIdType;
#else
#error "Error Type"
#endif

#if ((COM_RXGROUPSIGNAL_NUMBER <= 0xFFu) && (COM_RXGROUPSIGNAL_NUMBER >= 0u))
typedef uint8 Com_RxGroupSignalIdType;
#elif ((COM_RXGROUPSIGNAL_NUMBER <= 0xFFFFu) && (COM_RXGROUPSIGNAL_NUMBER > 0xFFu))
typedef uint16 Com_RxGroupSignalIdType;
#elif ((COM_RXGROUPSIGNAL_NUMBER <= 0xFFFFFFu) && (COM_RXGROUPSIGNAL_NUMBER > 0xFFFFu))
typedef uint32 Com_RxGroupSignalIdType;
#else
#error "Error Type"
#endif

#if ((COM_TXGROUPSIGNAL_NUMBER <= 0xFFu) && (COM_TXGROUPSIGNAL_NUMBER >= 0u))
typedef uint8 Com_TxGroupSignalIdType;
#elif ((COM_TXGROUPSIGNAL_NUMBER <= 0xFFFFu) && (COM_TXGROUPSIGNAL_NUMBER > 0xFFu))
typedef uint16 Com_TxGroupSignalIdType;
#elif ((COM_TXGROUPSIGNAL_NUMBER <= 0xFFFFFFu) && (COM_TXGROUPSIGNAL_NUMBER > 0xFFFFu))
typedef uint32 Com_TxGroupSignalIdType;
#else
#error "Error Type"
#endif

#if ((COM_TXSIGGRP_ERROR_AND_NOTIFY_NUMBER <= 0xFFu) && (COM_TXSIGGRP_ERROR_AND_NOTIFY_NUMBER >= 0u))
typedef uint8 Com_TxSigGrpErrAndNotifyIdType;
#elif ((COM_TXSIGGRP_ERROR_AND_NOTIFY_NUMBER <= 0xFFFFu) && (COM_TXSIGGRP_ERROR_AND_NOTIFY_NUMBER > 0xFFu))
typedef uint16 Com_TxSigGrpErrAndNotifyIdType;
#elif ((COM_TXSIGGRP_ERROR_AND_NOTIFY_NUMBER <= 0xFFFFFFu) && (COM_TXSIGGRP_ERROR_AND_NOTIFY_NUMBER > 0xFFFFu))
typedef uint32 Com_TxSigGrpErrAndNotifyIdType;
#else
#error "Error Type"
#endif

#if ((COM_TXSIGGRP_TIMEOUT_NUMBER <= 0xFFu) && (COM_TXSIGGRP_TIMEOUT_NUMBER >= 0u))
typedef uint8 Com_TxSigGrpTimeoutIdType;
#elif ((COM_TXSIGGRP_TIMEOUT_NUMBER <= 0xFFFFu) && (COM_TXSIGGRP_TIMEOUT_NUMBER > 0xFFu))
typedef uint16 Com_TxSigGrpTimeoutIdType;
#elif ((COM_TXSIGGRP_TIMEOUT_NUMBER <= 0xFFFFFFu) && (COM_TXSIGGRP_TIMEOUT_NUMBER > 0xFFFFu))
typedef uint32 Com_TxSigGrpTimeoutIdType;
#else
#error "Error Type"
#endif

#if ((COM_RXGRPSIG_FILTERTYPE_MAX_NUMBER <= 0xFFu) && (COM_RXGRPSIG_FILTERTYPE_MAX_NUMBER >= 0u))
typedef uint8 Com_RxGrpSigFilterIdType;
#elif ((COM_RXGRPSIG_FILTERTYPE_MAX_NUMBER <= 0xFFFFu) && (COM_RXGRPSIG_FILTERTYPE_MAX_NUMBER > 0xFFu))
typedef uint16 Com_RxGrpSigFilterIdType;
#elif ((COM_RXGRPSIG_FILTERTYPE_MAX_NUMBER <= 0xFFFFFFu) && (COM_RXGRPSIG_FILTERTYPE_MAX_NUMBER > 0xFFFFu))
typedef uint32 Com_RxGrpSigFilterIdType;
#else
#error "Error Type"
#endif

#if ((COM_TXGRPSIG_FILTERTYPE_MAX_NUMBER <= 0xFFu) && (COM_TXGRPSIG_FILTERTYPE_MAX_NUMBER >= 0u))
typedef uint8 Com_TxGrpSigFilterIdType;
#elif ((COM_TXGRPSIG_FILTERTYPE_MAX_NUMBER <= 0xFFFFu) && (COM_TXGRPSIG_FILTERTYPE_MAX_NUMBER > 0xFFu))
typedef uint16 Com_TxGrpSigFilterIdType;
#elif ((COM_TXGRPSIG_FILTERTYPE_MAX_NUMBER <= 0xFFFFFFu) && (COM_TXGRPSIG_FILTERTYPE_MAX_NUMBER > 0xFFFFu))
typedef uint32 Com_TxGrpSigFilterIdType;
#else
#error "Error Type"
#endif

#if ((COM_DESTSIG_FILTERTYPE_MAX_NUMBER <= 0xFFu) && (COM_DESTSIG_FILTERTYPE_MAX_NUMBER >= 0u))
typedef uint8 Com_DestSignalFilterIdType;
#elif ((COM_DESTSIG_FILTERTYPE_MAX_NUMBER <= 0xFFFFu) && (COM_DESTSIG_FILTERTYPE_MAX_NUMBER > 0xFFu))
typedef uint16 Com_DestSignalFilterIdType;
#elif ((COM_DESTSIG_FILTERTYPE_MAX_NUMBER <= 0xFFFFFFu) && (COM_DESTSIG_FILTERTYPE_MAX_NUMBER > 0xFFFFu))
typedef uint32 Com_DestSignalFilterIdType;
#else
#error "Error Type"
#endif

#if ((COM_RXIPDUBUFF_SIZE <= 0xFFu) && (COM_RXIPDUBUFF_SIZE >= 0u))
typedef uint8 Com_RxIpduBufIdType;
#elif ((COM_RXIPDUBUFF_SIZE <= 0xFFFFu) && (COM_RXIPDUBUFF_SIZE > 0xFFu))
typedef uint16 Com_RxIpduBufIdType;
#elif ((COM_RXIPDUBUFF_SIZE <= 0xFFFFFFu) && (COM_RXIPDUBUFF_SIZE > 0xFFFFu))
typedef uint32 Com_RxIpduBufIdType;
#else
#error "Error Type"
#endif

#if ((COM_TXIPDUBUFF_SIZE <= 0xFFu) && (COM_TXIPDUBUFF_SIZE >= 0u))
typedef uint8 Com_TxIpduBufIdType;
#elif ((COM_TXIPDUBUFF_SIZE <= 0xFFFFu) && (COM_TXIPDUBUFF_SIZE > 0xFFu))
typedef uint16 Com_TxIpduBufIdType;
#elif ((COM_TXIPDUBUFF_SIZE <= 0xFFFFFFu) && (COM_TXIPDUBUFF_SIZE > 0xFFFFu))
typedef uint32 Com_TxIpduBufIdType;
#else
#error "Error Type"
#endif

#if ((COM_SIGNALGROUPARRY_NUMBER <= 0xFFu) && (COM_SIGNALGROUPARRY_NUMBER >= 0u))
typedef uint8 Com_SignalGroupArryIdType;
#elif ((COM_SIGNALGROUPARRY_NUMBER <= 0xFFFFu) && (COM_SIGNALGROUPARRY_NUMBER > 0xFFu))
typedef uint16 Com_SignalGroupArryIdType;
#elif ((COM_SIGNALGROUPARRY_NUMBER <= 0xFFFFFFu) && (COM_SIGNALGROUPARRY_NUMBER > 0xFFFFu))
typedef uint32 Com_SignalGroupArryIdType;
#else
#error "Error Type"
#endif

/* AutoSAR COM Com_Module_Status_Type, Range COM_UNINIT/COM_INIT */
typedef enum
{
    COM_UNINIT = 0u,
    COM_INIT
} Com_StatusType;

/* Defines the endianness of the signal's network representation */
typedef enum
{
    COM_BIG_ENDIAN = 0u,
    COM_LITTLE_ENDIAN,
    COM_OPAQUE
} Com_SignalEndiannessType;

/*The AUTOSAR type of the signal*/
typedef enum
{
    COM_BOOLEAN = 0u,
    COM_FLOAT32,
    COM_FLOAT64,
    COM_SINT16,
    COM_SINT32,
    COM_SINT64,
    COM_SINT8,
    COM_UINT16,
    COM_UINT32,
    COM_UINT64,
    COM_UINT8,
    COM_UINT8_DYN,
    COM_UINT8_N
} Com_SignalType;

/*Defines if a write access to this signal can trigger the transmission of the corresponding I-PDU*/
typedef enum
{
    COM_PENDING = 0u,
    COM_TRIGGERED,
    COM_TRIGGERED_ON_CHANGE,
    COM_TRIGGERED_ON_CHANGE_WITHOUT_REPETITION,
    COM_TRIGGERED_WITHOUT_REPETITION,
    COM_GROUPSIGNAL_TMC_NONE
} Com_TransferPropertyType;

/*Defines if a write access to this signal can trigger the transmission of the corresponding I-PDU*/
typedef enum
{
    COM_FILTER_NONE = 0u,
    COM_ALWAYS,
    COM_MASKED_NEW_DIFFERS_MASKED_OLD,
    COM_MASKED_NEW_DIFFERS_X,
    COM_MASKED_NEW_EQUALS_X,
    COM_NEVER,
    COM_NEW_IS_OUTSIDE,
    COM_NEW_IS_WITHIN,
    COM_ONE_EVERY_N
} Com_FilterAlgorithmType;

typedef enum
{
    COM_TX_MODE_DIRECT = 0u,
    COM_TX_MODE_DIRECT_WITHOUT_REPETITION,
    COM_TX_MODE_MIXED,
    COM_TX_MODE_MIXED_WITHOUT_REPETITION,
    COM_TX_MODE_NONE,
    COM_TX_MODE_PERIODIC
} Com_TxModeModeType;

typedef enum
{
    COM_CONFIRMATION = 0u,
    COM_TRANSMIT,
    COM_TRIGGER_TRANSMIT,
    COM_CLEAR_UP_UNUSED
} Com_TxIPduClearUpdateBitType;

typedef enum
{
    COM_DEFERRED = 0u,
    COM_IMMEDIATE
} Com_IPduSignalProcessingType;

typedef enum
{
    COM_PDU_NORMAL = 0u,
    COM_PDU_TP
} Com_IPduType;

typedef enum
{
    COM_INVALID_ACTION_NOTIFY = 0u,
    COM_INVALID_ACTION_REPLACE,
    COM_INVALID_ACTION_NONE
} Com_DataInvalidActionType;

typedef enum
{
    COM_TIMEOUT_ACTION_NONE = 0u,
    COM_TIMEOUT_ACTION_REPLACE,
    COM_TIMEOUT_ACTION_SUBSTITUTE
} Com_RxDataTimeoutActionType;

typedef enum
{
    COM_TX_SIGNAL = 0u,
    COM_TX_GROUP_SIGNAL,
    COM_TX_DESCRIPTION,
    COM_RX_SIGNAL,
    COM_RX_GROUP_SIGNAL,
    COM_RX_DESCRIPTION,
} Com_SignalConfigType;

/* AUTOSAR COM signal object identifier, Range 0-SingalIdMax */
typedef uint16 Com_SignalIdType;

/* AUTOSAR COM signal group object identifier, Range 0-SingalGroupIdMax */
typedef uint16 Com_SignalGroupIdType;

/* AUTOSAR COM IPdu group object identifier, Range 0-PduGroupIdMax */
typedef uint16 Com_IpduGroupIdType;

typedef uint8 Com_MainFunctionType;

#if (COM_IPDUGROUP_MAX > 0u)
typedef uint8 Com_IpduGroupVector[((COM_IPDUGROUP_MAX - 1u) / 8u) + 1u];
#else
typedef uint8 Com_IpduGroupVector[1u];
#endif

/*the configuration parameters of PDU Counter*/
typedef struct
{
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2FUNC(void, COM_APPL_CODE, ComIPduCounterErrorNotification)
    (PduIdType ComPduId, uint8 ExpectedCounter, uint8 ReceivedCounter);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    uint32 ComIPduCounterStartPosition;
    uint8 ComIPduCounterSize; /*all bits in same byte*/
    uint8 ComIPduCounterThreshold;
    uint8 ComIPduMaxCounter;
} Com_IPduCounterType;

/*the information needed for each I-PDU replicated*/
typedef struct
{
    uint8 ComIPduReplicationQuorum;
} Com_IPduReplicationType;

typedef struct
{
    Com_SignalIdType ComGwDestSignalId;
    Com_MainFunctionType ComMainFunctionId;
    Com_SignalConfigType ComGwDestSignalType;
} Com_GwDestType;

/*Each instance of this container defines one mapping of the integrated Signal Gateway*/
typedef struct
{
    Com_SignalIdType ComGwSourceSignalId;
    Com_SignalIdType ComGwDestSignalStartId;
    Com_SignalIdType ComGwDestNumber;
    Com_SignalConfigType ComGwSourceSignalType;
} Com_GwMappingType;

/*Description of a gateway source*/
typedef struct
{
    uint16 ComSigLsbBytePos;
    uint16 ComSignalLength;
    uint16 ComGwIPduRef;
    uint16 GWSignalBufferId;
#if (STD_ON == COM_GW_SRC_SIG_UPDATE_BIT_ENABLE)
    Com_SignalPositionType ComUpdateLsbBytePos;
    uint8 ComUpdateBitMask;
#endif
    uint8 ComSignalPosInByte;
    uint8 ComBitSize;
    uint8 ComSignalByteLength;
    Com_SignalEndiannessType ComSignalEndianness;
    Com_SignalType ComSignalType;
#if (COM_GWMAPPING_NUMBER > 0u)
    Com_GwMappingIdType GwMappingId;
#endif
} Com_GwSourceSignalType;

/*This container contains the configuration parameters of the AUTOSAR COM module's Filters*/

typedef uint64 Com_FilterMaskType;

typedef uint64 Com_FilterXType;

typedef struct
{
    Com_FilterMaskType ComFilterMask;
    Com_FilterXType ComFilterX;
} Com_FilterMaskNewDifEquXType;

typedef uint64 Com_FilterMaxType;
typedef uint64 Com_FilterMinType;

typedef struct
{
    Com_FilterMaxType ComFilterMax;
    Com_FilterMinType ComFilterMin;
} Com_FilterNewIsWithinOutsideType;

typedef struct
{
    Com_SignalIdType ComFilterOffset;
    Com_SignalIdType ComFilterPeriod;
} Com_FilterONEEVERYNType;

/*Description of a gateway destination*/
typedef struct
{
    uint16 ComGwIPduRef;
    uint16 ComSignalLength;
    uint16 ComSignalInitValueId;
    uint16 ComSignalDataInitValueLength;
#if (COM_DESTSIG_FILTERTYPE_MAX_NUMBER > 0u)
    Com_SignalIdType ComTMCBufferId;
#endif
    Com_SignalPositionType ComSigLsbBytePos;
#if (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
    Com_SignalPositionType ComUpdateLsbBytePos;
#endif
#if (COM_DESTSIG_FILTERTYPE_MAX_NUMBER > 0u)
    Com_DestSignalFilterIdType ComFilterIndex;
#endif
    uint8 ComSignalPosInByte;
#if (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
    uint8 ComUpdateBitMask;
#endif
    uint8 ComBitSize;
    uint8 ComSignalByteLength;
    Com_SignalEndiannessType ComSignalEndianness;
    Com_TransferPropertyType ComTransferProperty;
#if (COM_DESTSIG_FILTERTYPE_MAX_NUMBER > 0u)
    Com_FilterAlgorithmType ComFilterAlgorithm;
#endif
    Com_SignalType ComSignalType;
} Com_GwDestSignalType;

/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
typedef P2FUNC(void, COM_APPL_CODE, Com_RxTimeoutNotificationType)(void);
typedef P2FUNC(void, COM_APPL_CODE, Com_RxInvalidNotificationType)(void);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

typedef struct
{
#if (STD_ON == COM_RX_IPDU_CALLOUT_ENABLE)
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2FUNC(boolean, COM_APPL_CODE, ComIPduCallout)
    (PduIdType PduId, P2CONST(PduInfoType, AUTOMATIC, COM_APPL_CONST) PduInfoPtr);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
#endif
    PduLengthType IPduNoDynSignalLength;
    PduLengthType IPduMaxDynSignalLength;
    uint16 ComRxIPduDMFirstTimeout;
    uint16 ComRxIPduDMTimeout;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    uint16 ComRxIpduPartitionId;
#endif
    Com_RxIpduBufIdType ComRxIPduBufIndex;
    Com_RxSignalIdType ComIpduSignalRefStartId;
    Com_RxSignalIdType ComIPduSignalsRefNumber;
    Com_RxIpduGroupIdType ComIPduGroupsRefStartId;
    Com_RxIpduGroupIdType ComIPduGroupsRefNumber;
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE)
    Com_RxSigTimeoutIdType ComIPduSignalsTimeoutRefStartId;
    Com_RxSigTimeoutIdType ComIPduSignalsTimeoutRefNumber;
#endif
    Com_RxSignalGroupIdType ComIPduSignalGroupsRefStartId;
    Com_RxSignalGroupIdType ComIPduSignalGroupsRefNumber;
#if (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)
    Com_RxSigGrpTimeoutIdType ComIPduSignalGroupsTimeoutRefStartId;
    Com_RxSigGrpTimeoutIdType ComIPduSignalGroupsTimeoutRefNumber;
#if (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)
    Com_RxGroupSignalIdType ComGrpSigSubstituteStartId;
#endif
#endif
#if (COM_GW_SOURCE_DESCRIPTION_NUMBER > 0u)
    Com_GwSourceDescriptionIdType ComIPduSourceSignalsRefStartId;
    Com_GwSourceDescriptionIdType ComIPduSourceSignalsRefNumber;
#endif
#if (STD_ON == COM_RX_IPDU_COUNTER_ENABLE)
    Com_RxIPduCounterIdType ComIPduCounterIndex;
#endif
    uint8 MetaDataLength;
#if (STD_ON == COM_CANCELLATION_SUPPORT)
    boolean ComIPduCancellationSupport;
#endif
    Com_IPduSignalProcessingType ComIPduSignalProcessing;
    Com_IPduType ComIPduType;
} Com_RxIPduType;

/*This container contains the configuration parameters of the AUTOSAR COM module's transmission modes*/
typedef uint8 Com_TxModeNumberOfRepetitionsType;
typedef uint16 Com_TxModeRepetitionPeriodType;
typedef uint16 Com_TxModeTimeOffsetType;
typedef uint16 Com_TxModeTimePeriodType;

typedef struct
{
    Com_TxModeTimeOffsetType ComTxModeTimeOffset;
    Com_TxModeTimePeriodType ComTxModeTimePeriod;
} Com_TxModePeriodType;

typedef struct
{
    Com_TxModeNumberOfRepetitionsType ComTxModeNumberOfRepetitions;
    Com_TxModeRepetitionPeriodType ComTxModeRepetitionPeriod;
} Com_TxModeDirectRepetitionsType;

typedef struct
{
    Com_TxModeTimeOffsetType ComTxModeTimeOffset;
    Com_TxModeTimePeriodType ComTxModeTimePeriod;
    Com_TxModeRepetitionPeriodType ComTxModeRepetitionPeriod;
    Com_TxModeNumberOfRepetitionsType ComTxModeNumberOfRepetitions;
} Com_TxModeMixedRepetitionsType;

typedef struct
{
#if (STD_ON == COM_TX_IPDU_CALLOUT_ENABLE)
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2FUNC(boolean, COM_APPL_CODE, ComIPduCallout)
    (PduIdType PduId, P2VAR(PduInfoType, AUTOMATIC, COM_APPL_CONST) PduInfoPtr);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
#endif
#if (COM_TXIPDU_TIGGERTRANSMIT_CALLOUT_ENABLE > 0u)
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2FUNC(boolean, COM_APPL_CODE, ComIPduTriggerTransmitCallout)
    (PduIdType PduId, P2VAR(PduInfoType, AUTOMATIC, COM_APPL_CONST) PduInfoPtr);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
#endif
    PduLengthType IPduNoDynSignalLength;
    PduLengthType IPduMaxDynSignalLength;
    uint16 ComTxIpduDM;
    uint16 ComMinimumDelayTime;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    uint16 ComTxIpduPartitionId;
#endif
    Com_TxIpduBufIdType ComTxIPduBufIndex;
    PduIdType ComPduIdRef;
    PduIdType ComTxModeTrueRefId;
    PduIdType ComTxModeFalseRefId;
    Com_TxSignalIdType ComIpduSignalRefStartId;
    Com_TxSignalIdType ComIPduSignalsRefNumber;
    Com_TxSignalGroupIdType ComIPduSignalGroupsRefStartId;
    Com_TxSignalGroupIdType ComIPduSignalGroupsRefNumber;
    Com_TxIpduGroupIdType ComIPduGroupsRefStartId;
    Com_TxIpduGroupIdType ComIPduGroupsRefNumber;
#if (COM_GW_DESTINATION_DESCRIPTION_NUMBER > 0u)
    Com_GwDestinationDescriptionIdType ComIPduDespSignalsRefStartId;
    Com_GwDestinationDescriptionIdType ComIPduDespSignalsRefNumber;
#endif
#if (STD_ON == COM_TX_IPDU_COUNTER_ENABLE)
    Com_TxIPduCounterIdType ComIPduCounterIndex;
#endif
    Com_TxModeModeType ComTxTrueModeMode;
    Com_TxModeModeType ComTxFalseModeMode;
    uint8 MetaDataLength;
#if (STD_ON == COM_CANCELLATION_SUPPORT)
    boolean ComIPduCancellationSupport;
#endif
#if (                                                                                             \
    (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE) || (STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE) \
    || (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE))
    Com_TxIPduClearUpdateBitType ComTxIPduClearUpdateBit;
#endif
    Com_IPduSignalProcessingType ComIPduSignalProcessing;
    Com_IPduType ComIPduType;
} Com_TxIPduType;

typedef struct
{
#if (STD_ON == COM_RX_SIGNAL_NOTIFICATION_ENABLE)
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2FUNC(void, COM_APPL_CODE, ComNotification)(void);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
#endif
    uint16 ComSignalInitValueId;
    uint16 ComSignalLength;
    uint16 ComSignalDataInitValueLength;
    uint16 GWSignalBufferId;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    uint16 ComRxSignalPartitionId;
#endif
    Com_SignalPositionType ComSigLsbBytePos;
#if (STD_ON == COM_RX_SIGNAL_UPDATE_BIT_ENABLE)
    Com_SignalPositionType ComUpdateLsbBytePos;
#endif
    PduIdType ComIpduRefIndex;
#if (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ENABLE)
    Com_RxSignalInvalidIdType ComSignalInvalidRefId;
#endif
#if (COM_RXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)
    Com_RxSignalFilterIdType ComFilterIndex;
#endif
#if (STD_ON == COM_RX_SIGNAL_UPDATE_BIT_ENABLE)
    uint8 ComUpdateBitMask;
#endif
    uint8 ComSignalPosInByte;
    uint8 ComBitSize;
    uint8 ComSignalByteLength;
    Com_SignalType ComSignalType;
    Com_SignalEndiannessType ComSignalEndianness;
#if (COM_RXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)
    Com_FilterAlgorithmType ComFilterAlgorithm;
#endif
#if (COM_GWMAPPING_NUMBER > 0u)
    Com_GwMappingIdType GwMappingId;
#endif
} Com_RxSignalType;

/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
typedef P2FUNC(void, COM_APPL_CODE, Com_TxTimeoutNotificationType)(void);
typedef P2FUNC(void, COM_APPL_CODE, Com_ErrorAndNotificationType)(void);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
typedef uint8 Com_TxSignalNotifyType;

typedef struct
{
    uint16 ComSignalInitValueId;
    uint16 ComSignalLength;
    uint16 ComSignalDataInitValueLength;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    uint16 ComTxSignalPartitionId;
#endif
#if (COM_TXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)
    Com_SignalIdType ComTMCBufferId;
#endif
    Com_SignalPositionType ComSigLsbBytePos;
#if (STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE)
    Com_SignalPositionType ComUpdateLsbBytePos;
#endif
    PduIdType ComIpduRefIndex;
#if (STD_ON == COM_TX_SIGNAL_INVALID_DATA_ENABLE)
    Com_TxSignalInvalidIdType ComSignalInvalidRefId;
#endif
#if (COM_TXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)
    Com_TxSignalFilterIdType ComFilterIndex;
#endif
#if (STD_ON == COM_TX_SIGNAL_TIMEOUT_ENABLE)
    Com_TxSignalTimeoutIdType ComTimeoutIndex;
#endif
#if (COM_TXSIG_ERROR_AND_NOTIFY_NUMBER > 0u)
    Com_TxSigErrAndNotifyIdType ComTxSignalErrAndNotifyFncId;
#endif
    uint8 ComSignalPosInByte;
    uint8 ComBitSize;
    uint8 ComSignalByteLength;
#if (STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE)
    uint8 ComUpdateBitMask;
#endif
    boolean ComInitialValueOnly;
    Com_SignalEndiannessType ComSignalEndianness;
    Com_SignalType ComSignalType;
    Com_TransferPropertyType ComTransferProperty;
#if (COM_TXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)
    Com_FilterAlgorithmType ComFilterAlgorithm;
#endif
#if (COM_TXSIG_ERROR_AND_NOTIFY_NUMBER > 0u)
    Com_TxSignalNotifyType ComTxSignalNotificationType;
#endif
} Com_TxSignalType;

typedef struct
{
    uint16 ComIPduStartBufIndex;
    uint16 SignalGroupArrayLength;
} Com_SignalGroupArrayType;

typedef struct
{
#if (STD_ON == COM_RX_SIG_GROUP_NOTIFICATION_ENABLE)
    P2FUNC(void, COM_APPL_CODE, ComNotification)(void); /* PRQA S 3432 */ /* MISRA Rule 20.7 */
#endif
#if (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE)
    P2FUNC(void, COM_APPL_CODE, ComInvalidNotification)(void); /* PRQA S 3432 */ /* MISRA Rule 20.7 */
#endif
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    uint16 ComRxSigGrpPartitionId;
#endif
#if (STD_ON == COM_RX_SIG_GROUP_UPDATE_BIT_ENABLE)
    Com_SignalPositionType ComUpdateLsbBytePos;
#endif
    Com_RxGroupSignalIdType ComGroupSignalRefStartId;
    Com_RxGroupSignalIdType ComGroupSignalRefNumber;
    PduIdType ComIpduRefIndex;
#if (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE)
    Com_DataInvalidActionType ComDataInvalidAction;
#endif
#if (STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API)
    Com_SignalGroupArryIdType ComSignalGroupArrayId;
#endif
#if (STD_ON == COM_RX_SIG_GROUP_UPDATE_BIT_ENABLE)
    uint8 ComUpdateBitMask;
#endif
} Com_RxSignalGroupType;

typedef struct
{
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    uint16 ComTxSigGrpPartitionId;
#endif
#if (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE)
    Com_SignalPositionType ComUpdateLsbBytePos;
#endif
    Com_TxGroupSignalIdType ComGroupSignalRefStartId;
    Com_TxGroupSignalIdType ComGroupSignalRefNumber;
    PduIdType ComIpduRefIndex;
#if (STD_ON == COM_TX_SIG_GROUP_TIMEOUT_ENABLE)
    Com_TxSigGrpTimeoutIdType ComTimeoutIndex;
#endif
#if (COM_TXSIGGRP_ERROR_AND_NOTIFY_NUMBER > 0u)
    Com_TxSigGrpErrAndNotifyIdType ComTxSigGrpErrAndNotifyFncId;
#endif
#if (STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API)
    Com_SignalGroupArryIdType ComSignalGroupArrayId;
#endif
#if (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE)
    uint8 ComUpdateBitMask;
#endif
#if (COM_TXSIGGRP_ERROR_AND_NOTIFY_NUMBER > 0u)
    Com_TxSignalNotifyType ComTxSignalNotificationType;
#endif
    boolean ComInitialValueOnly;
    Com_TransferPropertyType ComTransferProperty;
} Com_TxSignalGroupType;

typedef struct
{
    uint16 ComSignalInitValueId;
    uint16 ComSignalLength;
    uint16 ComSignalDataInitValueLength;
    uint16 GWSignalBufferId;
#if (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE)
    uint16 ComSignalDataInvalidValueLength;
    uint16 ComSignalInvalidBuffRefId;
#endif
    Com_SignalPositionType ComSigLsbBytePos;
    Com_RxSignalGroupIdType ComSignalGroupRef;
#if (COM_RXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
    Com_RxGrpSigFilterIdType ComFilterIndex;
#endif
    uint8 ComSignalPosInByte;
    uint8 ComBitSize;
    uint8 ComSignalByteLength;
    Com_SignalEndiannessType ComSignalEndianness;
    Com_SignalType ComSignalType;
#if (COM_RXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
    Com_FilterAlgorithmType ComFilterAlgorithm;
#endif
#if (COM_GWMAPPING_NUMBER > 0u)
    Com_GwMappingIdType GwMappingId;
#endif
} Com_RxGroupSignalType;
typedef struct
{
    uint16 ComSignalInitValueId;
    uint16 ComSignalLength;
    uint16 ComSignalDataInitValueLength;
#if (STD_ON == COM_TX_GRP_SIGNAL_INVALID_DATA_ENABLE)
    uint16 ComSignalDataInvalidValueLength;
    uint16 ComSignalInvalidBuffRefId;
#endif
    Com_SignalPositionType ComSigLsbBytePos;
#if (COM_TXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
    Com_SignalIdType ComTMCBufferId;
#endif
    Com_TxSignalGroupIdType ComSignalGroupRef;
#if (COM_TXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
    Com_TxGrpSigFilterIdType ComFilterIndex;
#endif
    uint8 ComSignalPosInByte;
    uint8 ComBitSize;
    uint8 ComSignalByteLength;
    Com_SignalEndiannessType ComSignalEndianness;
    Com_SignalType ComSignalType;
#if (COM_TXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
    Com_FilterAlgorithmType ComFilterAlgorithm;
#endif
    Com_TransferPropertyType ComTransferProperty;
} Com_TxGroupSignalType;
typedef struct
{
    uint16 RxIpduLength;
    uint16 RxIpduBaseDMTimeout;
    PduLengthType RxOffset;
    /*
     * bit0 ActiveEnable;
     * bit1 DMEnable;
     * bit2 Receiving;
     * bit3 RxIndication;
     * bit4 RxAnyCounterPdu;
     */
    uint8 RxIpduRTStFlag;
    uint8 RxIpduDMFlag;
#if (STD_ON == COM_RX_IPDU_COUNTER_ENABLE)
    uint8 RxIpduCounter;
#endif
#if (STD_ON == COM_IPDU_REPLICATION_ENABLE) && (STD_ON == COM_RX_IPDU_COUNTER_ENABLE)
    uint8 RxReplicationNumber;
#endif
} Com_RxIPduRunTimeStateType;

typedef struct
{
    uint16 TxIpduLength;
#if (STD_ON == COM_MDT_ENABLE)
    uint16 MDTCnt;
#endif
    uint16 RptNum;
    uint16 NTimeCnt;
    uint16 DMCnt;
    uint16 PeriodCnt;
    PduLengthType TxOffset;
#if (COM_TX_IPDU_COUNTER_ENABLE > 0u)
    uint8 TxIpduCounter;
#endif
    /*
     * bit0 ActiveEnable;
     * bit1 TxConfirm;
     * bit2 TMS;
     * bit3 Transmiting;
     * bit4 TxDelay;
     */
    uint8 TxIpduRTStFlag;
    Com_TxModeModeType ipduTxMode;
} Com_TxIPduRunTimeStateType;

typedef struct
{
    uint16 DynamicSignalLength;
    /*
     * bit0 NTimesTransmitNeed;
     * bit1 WithoutRepetition;
     */
    uint8 TxSigGrpRTStFlag;
} Com_TxSignalGroupRunTimeStateType;

typedef struct
{
    const Com_RxIPduType* ComRxIPdu;
    const Com_TxIPduType* ComTxIPdu;
    const Com_RxSignalType* ComRxSignal;
    const Com_TxSignalType* ComTxSignal;
    const Com_RxSignalGroupType* ComRxSignalGroup;
    const Com_TxSignalGroupType* ComTxSignalGroup;
    const Com_RxGroupSignalType* ComRxGroupSignal;
    const Com_TxGroupSignalType* ComTxGroupSignal;
    const Com_GwMappingType* ComGwMapping;
    const Com_GwSourceSignalType* ComSourceSignal;
    const Com_GwDestSignalType* ComDestSignal;
    const uint16* MainFunctionRxRange;
    const uint16* MainFunctionTxRange;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    const uint16* ComIPduGroupPartition;
    const uint16* ComMainFuncRxPartitionRange;
    const uint16* ComMainFuncTxPartitionRange;
#endif
    Com_SignalIdType ComTxSignalNum;
    Com_SignalIdType ComRxSignalNum;
    PduIdType ComTxPduNum;
    PduIdType ComRxPduNum;
    Com_SignalIdType ComTxGroupSignalNum;
    Com_SignalIdType ComRxGroupSignalNum;
    Com_SignalGroupIdType ComTxSignalGroupNum;
    Com_SignalGroupIdType ComRxSignalGroupNum;
    Com_IpduGroupIdType ComIpduGroupNum;
    Com_MainFunctionType MainFunctionRouteSignalsNum;
    Com_MainFunctionType MainFunctionRxNum;
    Com_MainFunctionType MainFunctionTxNum;
} Com_ConfigType;

#endif /* end of COM_TYPES_H */
