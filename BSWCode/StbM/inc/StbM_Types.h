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
 **  @file               : StbM_Types.h
 **  @author             : xiongfei.shi
 **  @date               : 2024/12/26
 **  @vendor             : isoft
 **  @description        : The type definition of the StbM module
 **
 ***********************************************************************************************************************/

#ifndef STBM_TYPES_H
#define STBM_TYPES_H

/* =================================================== inclusions =================================================== */
#include "Std_Types.h"
#include "StbM_Cfg.h"
#include "Rte_StbM_Type.h"

#if (STBM_TRIGGERED_CUSTOMER_NUM > 0u) || (STBM_MULTIPLE_PARTITION_ENABLED == STD_ON)
#include "Os.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */
typedef enum
{
    STBM_TBTYPE_PURELOCAL,
    STBM_TBTYPE_SYNCHRONIZED,
} StbM_SynchronizedTimeBaseTypeType;
typedef enum
{
    CRC_IGNORED,
    CRC_NOT_VALIDATED,
    CRC_OPTIONAL,
    CRC_VALIDATED
} CrcValidatedType;

typedef enum
{
    CRC_NOT_SUPPORTED,
    CRC_SUPPORTED
} CrcSecuredType;

#if STBM_TRIGGERED_CUSTOMER_NUM > 0u
/**
 * @brief defines Configuration of The triggered customer.
 */
typedef struct StbM_TriggeredCustomerCfgTag
{
    /* The triggering period of the triggered customer */
    uint16 TriggeredCustomerPeriod;
    /* Mandatory reference to synchronized OS ScheduleTable */
    uint16 OSScheduleTableRef;
    /* Mandatory reference to the required synchronized time-base */
    uint16   SynchronizedTimeBaseRef;
    TickType OsScheduleTableDuration;
} StbM_TriggeredCustomerCfgType;
#endif

typedef enum
{
    CALLBACK,
    CALLBACK_AND_SR_INTERFACE,
    NO_NOTIFICATION,
    SR_INTERFACE
} StbM_NotificationInterfaceType;

typedef enum
{
    ETHTSYN_GLOBALTIME_DOMAIN,
    GPT_CHANNEL_CONFIGURATION,
    OS_COUNTER
} StbM_LocalTimeHardwareCfgType;

/**
 * @brief defines Configuration of the rate and offset correction.
 */
typedef struct StbM_TimeCorrectionCfgTag
{
    /* This attribute describes whether the rate correction value of a Time Base
can be set by StbM_SetRateCorrection() */
    boolean AllowMasterRateCorrection;
    /* This attribute describes the maximum allowed absolute value of the rate
deviation value to be set by StbM_SetRateCorrection() */
    uint16 MasterRateDeviationMax;
    /* Defines the interval during which the adaptive rate correction cancels out
the rate- and time deviation */
    uint64 OffsetCorrectionAdaptionInterval;
    /* Threshold for the correction method. Deviations below this value will be
corrected by a linear reduction over a defined timespan. */
    uint64 OffsetCorrectionJumpThreshold;
    /* Definition of the time span [s] which is used to calculate the rate deviation */
    uint64 RateCorrectionMeasurementDuration;
    /* Number of simultaneous rate measurements to determine the current rate deviation */
    uint16 RateCorrectionsPerMeasurementDuration;
    /* Threshold for rate correction calculation */
    uint16 RateCorrectionThreshold;
} StbM_TimeCorrectionCfgType;

/**
 * @brief defines Configuration of the hardware reference clock.
 */
typedef struct StbM_LocalTimeClockCfgTag
{
    /* Represents the frequency [Hz] of the HW reference clock used by the StbM. */
    uint32 ClockFrequency;
    /* Represents the prescaler to calculate the resulting frequency of the HW
reference clock used by the StbM. */
    uint32 ClockPrescaler;
    /* Reference to the local time hardware. */
    uint8                         LocalTimeHardware;
    StbM_LocalTimeHardwareCfgType LocalTimeHardwareType;
    uint32                        LocalTimeMax;
} StbM_LocalTimeClockCfgType;

#if (STBM_TIME_RECORDING_SUPPORT == STD_ON)
/**
 * @brief defines Configuration of the time recording.
 */
typedef struct StbM_TimeRecordingCfgTag
{
    /* Name of the customer specific callback function, which shall be called, if a
    measurement data for a Synchronized Time Base are available. */
    void (*SyncTimeRecordBlockCallback)(const StbM_SyncRecordTableBlockType* syncRecordTableBlock);
    /* Represents the number of Blocks used for queing time measurement
events for the Synchronized Time Base Record Table. */
    uint16 SyncTimeRecordTableBlockCount;
} StbM_TimeRecordingCfgType;
#endif

/**
 * @brief defines Configuration of the Notification Customer.
 */
typedef struct StbM_NotificationCustomerCfgTag
{
    StbM_SynchronizedTimeBaseType TimeBaseId;
    /* Name of the customer specific notification callback function, which shall be
called, if the time previously set by the customer is reached. */
    void (*TimeNotificationCallback)(StbM_TimeDiffType deviationTime);
} StbM_NotificationCustomerCfgType;

/**
 * @brief defines the information about a specific time-base.
 */
typedef struct StbM_SynchronizedTimeBaseCfgTag
{
    /* This attribute describes the required number of updates to the Time Base */
    uint16 ClearTimeleapCount;
    /* This parameter shall be set to true for a Global Time Master that acts as a
system-wide source of time information with respect to Global Time. */
    boolean IsSystemWideGlobalTimeMaster;
#if STBM_STATUS_NOTIFICATIONS_ENABLED == STD_ON
    /* The parameter defines what type of interface shall be used to notify a customer of a
status event. */
    StbM_NotificationInterfaceType NotificationInterface;
    /* Name of the customer specific status notification callback function, which
shall be called, if a non-masked status event occurs. */
    void (*StatusNotificationCallback)(StbM_TimeBaseNotificationType eventNotification);
    /* The parameter defines the initial value for NotificationMask mask, which defines the
     *  events for which the event notification callback function shall be called. */
    StbM_TimeBaseNotificationType StatusNotificationMask;
#endif
    /* Identification of a Synchronized TimeBase via a unique identifier. */
    uint16 SynchronizedTimeBaseIdentifier;
    /* This attribute describes the timeout for the situation that the time
synchronization gets lost in the scope of the time domain */
    uint64 SyncLossTimeout;
    /* This represents the maximum allowed positive difference between a newly
received Global Time Base value and the current Local Time Base value */
    uint64 TimeLeapFutureThreshold;
    /* This represents the maximum allowed negative difference between the
current Local Time Base value and a newly received Global Time Base value */
    uint64                            TimeLeapPastThreshold;
    const StbM_TimeCorrectionCfgType* TimeCorrection;
    const StbM_LocalTimeClockCfgType* LocalTimeClock;
#if (STBM_TIME_RECORDING_SUPPORT == STD_ON)
    const StbM_TimeRecordingCfgType* TimeRecording;
#endif
    /* This container holds the configuration of a notification customer, which is
notified is informed about the occurance of a Time-base related event. */
    uint16                                  NotificationCustomerNum;
    const StbM_NotificationCustomerCfgType* NotificationCustomer;
    StbM_SynchronizedTimeBaseTypeType       SynchronizedTimeBaseType;
    boolean                                 IsTimeGateway;
#if STBM_MULTIPLE_PARTITION_ENABLED == STD_ON
    ApplicationType ApplicationId;
    boolean         CrossCoreAccess;
#endif
} StbM_SynchronizedTimeBaseCfgType;

typedef struct StbM_ConfigTag
{
#if STBM_TRIGGERED_CUSTOMER_NUM > 0u
    const StbM_TriggeredCustomerCfgType* TriggeredCustomer;
#endif
    const StbM_SynchronizedTimeBaseCfgType* SynchronizedTimeBase;
} StbM_ConfigType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

#ifdef __cplusplus
}
#endif

#endif
