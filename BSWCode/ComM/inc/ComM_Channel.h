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
 ***********************************************************************************************************************
 **
 **  @file               : ComM_Channel.h
 **  @author             : xiaojian.liang
 **  @date               : 2024/08/21
 **  @vendor             : isoft
 **  @description        : ComM Channel Implementation header
 **
 **********************************************************************************************************************/

#ifndef COMM_CHANNEL_H_
#define COMM_CHANNEL_H_

/* =================================================== inclusions =================================================== */
#include "ComM_Internal.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */
#define COMM_NOT_USED_CHANNEL_ID ((NetworkHandleType)255)

/* ================================================ type definitions ================================================ */
typedef enum
{
    COMM_NM_VARIANT_FULL,
    COMM_NM_VARIANT_LIGHT,
    COMM_NM_VARIANT_NONE,
    COMM_NM_VARIANT_PASSIVE,
} ComM_NmVariantType;

typedef enum
{
    COMM_GATEWAY_TYPE_NONE,
    COMM_GATEWAY_TYPE_ACTIVE,
    COMM_GATEWAY_TYPE_PASSIVE,
} ComM_PncGatewayTypeType;

#if COMM_NM_ENABLED == STD_ON
typedef enum
{
    COMM_NM_MODE_EVENT_NONE,
    COMM_NM_MODE_EVENT_BUS_SLEEP,
    COMM_NM_MODE_EVENT_PREPARE_BUS_SLEEP,
    COMM_NM_MODE_EVENT_NETWORK,
} ComM_NmModeEventType;
#endif

/** @ingroup ComMChannelTypes */
typedef struct ComM_ChannelConfigType_Tag
{
    Std_ReturnType (*BusSMRequestComMode)(NetworkHandleType Channel, ComM_ModeType ComMode);
    uint32             NmLightTimeout;
    ComM_NmVariantType NmVariant;
    NetworkHandleType  ManagingChannel;
#if COMM_USER_NUMBER > 0u
#if COMM_CURRENT_CHANNEL_REQUEST_NOTIFICATION_ENABLED == STD_ON
    Std_ReturnType (*CurrentChannelRequest)(uint8 numberOfRequesters, ComM_UserHandleType* handleArray);
    ComM_UserHandleType* const UserRequesters;
#endif
    const ComM_UserHandleType* Users;
    uint8                      NumberOfUsers;
#endif
#if COMM_PNC_SUPPORT == STD_ON
    uint8* const PncBitVectorIRA;
    uint8        PncBitVectorLength;
    boolean      PncNmRequest;
    boolean      WakeupSleepRequestEnabled;
#if COMM_PNC_GATEWAY_ENABLED == STD_ON
    ComM_PncGatewayTypeType PncGatewayType;
#endif
#endif
#if COMM_MULTIPLE_PARTITION_ENABLED == STD_ON
    ApplicationType Application;
#endif
} ComM_ChannelConfigType;

typedef struct
{
    ComM_StateType State;
#if COMM_NM_ENABLED == STD_ON
    /* Set and CmpAndSet are mutually exclusive */
    ComM_NmModeEventType NmModeEvent;
#endif
    boolean Allowed;
    uint32  NmLightTimeout;
#if COMM_CURRENT_CHANNEL_REQUEST_NOTIFICATION_ENABLED == STD_ON
    uint8                NumberOfUserRequesters;
    ComM_UserHandleType* UserRequesters;
#endif
#if COMM_PNC_SUPPORT == STD_ON
    uint8* PncBitVectorIRA;
#endif
} ComM_ChannelLocalVarType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */
extern ComM_ChannelLocalVarType* const ComM_ChannelLocalVarPtrs[COMM_CHANNEL_NUMBER];

/* ========================================= external function declarations ========================================= */
/**
 * @brief Initializes the global variables for all channels that are used across multiple partitions.
 * @param[in]     ChannelConfigs  Pointer to post-build configuration data for all channels
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-61604
 */
void ComM_ChannelGlobalInit(const ComM_ChannelConfigType* ChannelConfigs);

/**
 * @brief Initializes the local variables for all channels assigned to the current partition.
 * @reentrant     TRUE (Reentrant for different partitions. Non reentrant for the same partition.)
 * @synchronous   TRUE
 * @trace         CPD-61605
 */
void ComM_ChannelLocalInit(void);

#ifdef COMM_TEST
/**
 * @brief The function returns the current state of the corresponding channel.
 * @param[in]    Channel  See NetworkHandleType
 * @return       See ComM_StateType
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
ComM_StateType ComM_ChannelGetState(NetworkHandleType Channel);
#endif

/**
 * @brief Function to query the maximum allowed Communication Mode of the corresponding channel.
 * @param[in]    Channel  See NetworkHandleType
 * @return       See ComM_ModeType
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-61606
 */
ComM_ModeType ComM_ChannelGetMaxComMode(NetworkHandleType Channel);

#ifdef QAC_ANALYZE
#pragma PRQA_NO_SIDE_EFFECTS ComM_ChannelGetCurrentComMode
#endif

/**
 * @brief Function to query the current Communication Mode of the corresponding channel.
 * @param[in]    Channel  See NetworkHandleType
 * @return       See ComM_ModeType
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-61607
 */
ComM_ModeType ComM_ChannelGetCurrentComMode(NetworkHandleType Channel);

/**
 * @brief Change the communication allowed status for the corresponding channel.
 * @param[in]    Channel  See NetworkHandleType
 * @param[in]    Allowed  TRUE: Communication is allowed FALSE: Communication is not allowed
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-61608
 */
void ComM_ChannelCommunicationAllowed(NetworkHandleType Channel, boolean Allowed);

/**
 * @brief Indication of the actual bus mode by the corresponding Bus State Manager.
 * @param[in]    Channel  See NetworkHandleType
 * @param[in]    ComMode  See ComM_ModeType
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-61609
 */
void ComM_ChannelBusSMModeIndication(NetworkHandleType Channel, ComM_ModeType ComMode);

#if COMM_NM_ENABLED == STD_ON
/**
 * @brief Indication of the nm mode event for the corresponding channel.
 * @param[in]    Channel      See NetworkHandleType
 * @param[in]    NmModeEvent  See ComM_NmModeEventType
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-61610
 */
void ComM_ChannelNmModeIndication(NetworkHandleType Channel, ComM_NmModeEventType NmModeEvent);
#endif

/**
 * @brief Notification of a wake up on the corresponding channel
 * @param[in]    Channel            See NetworkHandleType
 * @param[in]    SynchronousWakeUp  TRUE: Synchronous Wake Up Enabled FALSE: Synchronous Wake Up Disabled
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-61612
 */
void ComM_ChannelWakeUpIndication(NetworkHandleType Channel, boolean SynchronousWakeUp);

#if COMM_PNC_SUPPORT == STD_ON
/**
 * @brief Notification of a pnc nm request on the corresponding channel.
 * @param[in]    Channel  See NetworkHandleType
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-61633
 */
void ComM_ChannelSetPncNmRequest(NetworkHandleType Channel);

/**
 * @brief Notification of a wake up sleep request on the corresponding channel.
 * @param[in]    Channel  See NetworkHandleType
 * @retval       TRUE     Successfully
 * @retval       FALSE    WakeupSleepRequestEnabled is disabled on the corresponding channel
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-61634
 */
boolean ComM_ChannelSetWakeupSleepRequest(NetworkHandleType Channel);
#endif

/**
 * @brief Indication of active or inactive diagnostic by the DCM.
 * @param[in]    channel  See NetworkHandleType
 * @param[in]    active   TRUE: Active diagnostic FALSE: Inactive diagnostic
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-61635
 */
void ComM_ChannelActiveDiagnostic(NetworkHandleType channel, boolean active);

#if COMM_PNC_SUPPORT == STD_ON
#if COMM_PNC_GATEWAY_ENABLED == STD_ON
/**
 * @brief Indicate the current external PNC request for the corresponding channel.
 * @param[in]    Channel          See NetworkHandleType
 * @param[in]    PncBitVectorPtr  PNC bit vector which contain the current ERA received on the given channel
 * @reentrant    TRUE (Reentrant for different channels. Non reentrant for the same channel.)
 * @synchronous  TRUE
 * @trace        CPD-61636
 */
void ComM_ChannelUpdateERA(NetworkHandleType Channel, const uint8* PncBitVectorPtr);
#endif
#endif

/**
 * @brief This function shall perform the processing of the ComM channel state activities.
 * @param[in]    Channel  See NetworkHandleType
 * @reentrant    TRUE (Reentrant for different channels. Non reentrant for the same channel.)
 * @synchronous  TRUE
 * @trace        CPD-61637
 */
void ComM_ChannelMainFunction(NetworkHandleType Channel);

#if COMM_MULTIPLE_PARTITION_ENABLED == STD_ON
/**
 * @brief Function to query the application of the corresponding channel.
 * @param[in]    Channel  See NetworkHandleType
 * @return       The application of the corresponding channel
 * @reentrant    TRUE
 * @synchronous  TRUE
 */
ApplicationType ComM_ChannelGetApplication(NetworkHandleType Channel);
#endif
#ifdef __cplusplus
}
#endif

#endif /* COMM_CH_H_ */
