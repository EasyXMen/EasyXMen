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

#if (STBM_TRIGGERED_CUSTOMER_NUM > 0u) || (STBM_MULTIPLE_PARTITION_ENABLED == STD_ON)
#include "Os.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */
/**
 * @brief defines Synchronized Time Base Type
 */
typedef enum
{
    STBM_TBTYPE_PURELOCAL,    /**< Pure local time base type @range 0 */
    STBM_TBTYPE_SYNCHRONIZED, /**< Synchronized time base type @range 1 */
} StbM_SynchronizedTimeBaseTypeType;

/**
 * @brief defines CRC validated type
 */
typedef enum
{
    CRC_IGNORED,       /**< CRC is ignored @range 0 */
    CRC_NOT_VALIDATED, /**< CRC is not validated @range 1 */
    CRC_OPTIONAL,      /**< CRC is optional @range 2 */
    CRC_VALIDATED      /**< CRC is validated @range 3 */
} CrcValidatedType;

/**
 * @brief defines CRC secured type
 */
typedef enum
{
    CRC_NOT_SUPPORTED, /**< CRC is not supported @range 0 */
    CRC_SUPPORTED      /**< CRC is supported @range 1 */
} CrcSecuredType;

#if STBM_TRIGGERED_CUSTOMER_NUM > 0u
/**
 * @brief defines Configuration of The triggered customer.
 */
typedef struct
{
    uint16   TriggeredCustomerPeriod; /**< The triggering period of the triggered customer @range 0..65535 */
    uint16   OSScheduleTableRef;      /**< Mandatory reference to synchronized OS ScheduleTable @range 0..65535 */
    uint16   SynchronizedTimeBaseRef; /**< Mandatory reference to the required synchronized time-base @range 0..65535 */
    TickType OsScheduleTableDuration; /**< OS schedule table duration @range 0..0xFFFFFFFFFFFFFFFF */
} StbM_TriggeredCustomerCfgType;
#endif

/**
 * @brief defines Notification Interface Type
 */
typedef enum
{
    CALLBACK,                  /**< Callback interface only @range 0 */
    CALLBACK_AND_SR_INTERFACE, /**< Callback and SR interface @range 1 */
    NO_NOTIFICATION,           /**< No notification @range 2 */
    SR_INTERFACE               /**< SR interface only @range 3 */
} StbM_NotificationInterfaceType;

/**
 * @brief defines Local Time Hardware Configuration Type
 */
typedef enum
{
    CANTSYN_GLOBALTIME_DOMAIN, /**< CAN time sync global time domain @range 0 */
    ETHTSYN_GLOBALTIME_DOMAIN, /**< ETH time sync global time domain @range 1 */
    GPT_CHANNEL_CONFIGURATION, /**< GPT channel configuration @range 2 */
    OS_COUNTER                 /**< OS counter configuration @range 3 */
} StbM_LocalTimeHardwareCfgType;

/**
 * @brief defines Rate Source Type
 */
typedef enum
{
    RATE_SOURCE_STBM,            /**< Rate source from STBM @range 0 */
    RATE_SOURCE_TIMESYNC_MODULE, /**< Rate source from time sync module @range 1 */
    RATE_SOURCE_NOT_CONFIG,      /**< Rate source not configured @range 2 */
} StbM_RateSourceType;

/**
 * @brief defines Configuration of the rate and offset correction.
 */
typedef struct
{
    boolean AllowMasterRateCorrection; /**< This attribute describes whether the rate correction value of a Time Base
                                          can be set by StbM_SetRateCorrection() @range FALSE..TRUE */
    uint16 MasterRateDeviationMax;     /**< This attribute describes the maximum allowed absolute value of the rate
                                          deviation value to be set by StbM_SetRateCorrection() @range 0..65535 */
    uint64 OffsetCorrectionAdaptionInterval; /**< Defines the interval during which the adaptive rate correction cancels
                                                out the rate- and time deviation @range 0..0xFFFFFFFFFFFFFFFF */
    uint64 OffsetCorrectionJumpThreshold; /**< Threshold for the correction method. Deviations below this value will be
                                             corrected by a linear reduction over a defined timespan.
                                             @range 0..0xFFFFFFFFFFFFFFFF */
    uint64 RateCorrectionMeasurementDuration; /**< Definition of the time span [s] which is used to calculate the rate
                                                 deviation @range 0..0xFFFFFFFFFFFFFFFF */
    uint16 RateCorrectionsPerMeasurementDuration; /**< Number of simultaneous rate measurements to determine the current
                                                     rate deviation @range 0..65535 */
    uint16              RateCorrectionThreshold;  /**< Threshold for rate correction calculation @range 0..65535 */
    StbM_RateSourceType RateSource;               /**< Selects which module does the rate measurement. @range NA */
} StbM_TimeCorrectionCfgType;

#if STBM_CLONE_TIMEBASE_USED == STD_ON
typedef struct
{
    uint64               TimeOffset; /**< Offset value for cloning @range 0..0xFFFFFFFFFFFFFFFF */
    StbM_CloneConfigType CloneCfg;   /**< Refines how source Time Base is cloned to destination @range NA */
    boolean              Pending;    /**< Clone request is pending or not @range FALSE..TRUE */
} StbM_CloneRequestType;
#endif

/**
 * @brief defines Configuration of the hardware reference clock for StbMLocalTimeClock and StbMFallbackTimeClock.
 */
typedef struct
{
    uint8 LocalTimeHardware; /**< Reference to the local time hardware. @range 0..255 */
#if STBM_ETHIF_TIMESTAMP_SUPPORT == STD_ON
    uint8 ClkUnitIdx; /**< Clock unit index @range 0..255 */
#endif
    StbM_LocalTimeHardwareCfgType LocalTimeHardwareType; /**< Local time hardware type @range NA */
} StbM_LocalTimeClockCfgType;

#if STBM_DISCIPLINED_CLOCK_USED == STD_ON
/**
 * @brief defines Configuration of the disciplined clock.
 */
typedef struct
{
    uint8 CtrlIdx;    /**< Control index @range 0..255 */
    uint8 ClkUnitIdx; /**< Clock unit index @range 0..255 */
} StbM_DisciplinedClockCfgType;
#endif

#if (STBM_TIME_RECORDING_SUPPORT == STD_ON)
/**
 * @brief defines Configuration of the time recording.
 */
typedef struct
{
    void (*SyncTimeRecordBlockCallback)(
        const StbM_SyncRecordTableBlockType*
            syncRecordTableBlock); /**< Name of the customer specific callback function, which shall be called, if a
                                      measurement data for a Synchronized Time Base are available. @range NA */
    uint16 SyncTimeRecordTableBlockCount; /**< Represents the number of Blocks used for queing time measurement events
                                             for the Synchronized Time Base Record Table. @range 0..65535 */
} StbM_TimeRecordingCfgType;
#endif

/**
 * @brief defines Configuration of the Notification Customer.
 */
typedef struct
{
    StbM_SynchronizedTimeBaseType TimeBaseId; /**< Time base identifier @range NA */
    void (*TimeNotificationCallback)(
        StbM_TimeDiffType
            deviationTime); /**< Name of the customer specific notification callback function, which shall be called, if
                               the time previously set by the customer is reached. @range NA */
} StbM_NotificationCustomerCfgType;

#if STBM_CAN_SLAVE_TIME_VALIDATION_USED == STD_ON
typedef struct
{
    StbM_CanTimeSlaveMeasurementType* Data; /**< Data pointer @range NA */
    Std_ReturnType (*SetSlaveTimingData)(const StbM_CanTimeSlaveMeasurementType*
                                             measurementData); /**< Set slave timing data function pointer @range NA */
} StbM_CanSlaveMeasurementQueueType;
#endif

#if STBM_CAN_MASTER_TIME_VALIDATION_USED == STD_ON
typedef struct
{
    StbM_CanTimeMasterMeasurementType* Data; /**< Data pointer @range NA */
    Std_ReturnType (*SetMasterTimingData)(
        const StbM_CanTimeMasterMeasurementType*
            measurementData); /**< Set master timing data function pointer @range NA */
} StbM_CanMasterMeasurementQueueType;
#endif

#if STBM_ETH_SLAVE_TIME_VALIDATION_USED == STD_ON
typedef struct
{
    StbM_EthTimeSlaveMeasurementType* Data; /**< Data pointer @range NA */
    Std_ReturnType (*SetSlaveTimingData)(const StbM_EthTimeSlaveMeasurementType*
                                             measurementData); /**< Set slave timing data function pointer @range NA */
} StbM_EthSlaveMeasurementQueueType;

typedef struct
{
    StbM_PdelayInitiatorMeasurementType* Data; /**< Data pointer @range NA */
    Std_ReturnType (*SetPdelayInitiatorData)(
        const StbM_PdelayInitiatorMeasurementType*
            measurementData); /**< Set Pdelay initiator data function pointer @range NA */
} StbM_PdelayInitiatorMeasurementQueueType;
#endif

#if STBM_ETH_MASTER_TIME_VALIDATION_USED == STD_ON
typedef struct
{
    StbM_EthTimeMasterMeasurementType* Data; /**< Data pointer @range NA */
    Std_ReturnType (*SetMasterTimingData)(
        const StbM_EthTimeMasterMeasurementType*
            measurementData); /**< Set master timing data function pointer @range NA */
} StbM_EthMasterMeasurementQueueType;

typedef struct
{
    StbM_PdelayResponderMeasurementType* Data; /**< Data pointer @range NA */
    Std_ReturnType (*SetPdelayResponderData)(
        const StbM_PdelayResponderMeasurementType*
            measurementData); /**< Set Pdelay responder data function pointer @range NA */
} StbM_PdelayResponderMeasurementQueueType;
#endif

#if STBM_CAN_SLAVE_TIME_VALIDATION_USED == STD_ON || STBM_ETH_SLAVE_TIME_VALIDATION_USED == STD_ON
/**
 * @brief defines Configuration of the Pdelay and rate validation for CAN slave time validation and ETH slave time
 * validation.
 */
typedef struct
{
    boolean AllowPdelayValidation;     /**< This attribute describes whether the Pdelay validation is enabled for a Time
                                          Base @range FALSE..TRUE */
    uint32  PdelayValidationThreshold; /**< Pdelay validation threshold @range 0..0xFFFFFFFF */
    boolean AllowRateValidation; /**< This attribute describes whether the rate validation is enabled for a Time Base
                                    @range FALSE..TRUE */
    float64 RateJitterThreshold; /**< Rate jitter threshold @range NA */
    boolean AllowRateWanderValidation; /**< This attribute describes whether the rate wander validation is enabled for a
                                          Time Base. Only set TRUE if both StbMRateWanderThreshold and
                                          StbMRateWanderIntervalWindow are configured, and Window is non-zero @range
                                          FALSE..TRUE */
    float64 RateWanderThreshold;       /**< Rate wander threshold @range NA */
    uint8   RateWanderIntervalWindow;  /**< Rate wander interval window @range 0..255 */
} StbM_PdelayAndRateValidationType;
#endif

/**
 * @brief defines the information about a specific time-base.
 */
typedef struct
{
    uint16 ClearTimeleapCount;            /**< This attribute describes the required number of updates to the Time Base
                                               @range 0..65535 */
    boolean IsSystemWideGlobalTimeMaster; /**< This parameter shall be set to true for a Global Time Master that acts as
                                             a system-wide source of time information with respect to Global Time.
                                             @range FALSE..TRUE */
#if STBM_STATUS_NOTIFICATIONS_ENABLED == STD_ON
    StbM_NotificationInterfaceType NotificationInterface; /**< The parameter defines what type of interface shall be
                                                             used to notify a customer of a status event. @range NA */
    void (*StatusNotificationCallback)(
        StbM_TimeBaseNotificationType
            eventNotification); /**< Name of the customer specific status notification callback function, which shall be
                                   called, if a non-masked status event occurs. @range NA */
    StbM_TimeBaseNotificationType
        StatusNotificationMask; /**< The parameter defines the initial value for NotificationMask mask, which defines
                                   the events for which the event notification callback function shall be called.
                                   @range NA */
#endif
    uint16 SynchronizedTimeBaseIdentifier; /**< Identification of a Synchronized TimeBase via a unique identifier.
                                              @range 0..65535 */
    uint64 SyncLossTimeout; /**< This attribute describes the timeout for the situation that the time synchronization
                               gets lost in the scope of the time domain @range 0..0xFFFFFFFFFFFFFFFF */
    uint64 TimeLeapFutureThreshold; /**< This represents the maximum allowed positive difference between a newly
                                       received Global Time Base value and the current Local Time Base value
                                       @range 0..0xFFFFFFFFFFFFFFFF */
    uint64 TimeLeapPastThreshold; /**< This represents the maximum allowed negative difference between the current Local
                                     Time Base value and a newly received Global Time Base value
                                     @range 0..0xFFFFFFFFFFFFFFFF */
    const StbM_TimeCorrectionCfgType* TimeCorrection; /**< Time correction configuration pointer @range NA */
#if STBM_CLONE_TIMEBASE_USED == STD_ON
    StbM_CloneRequestType* const CloneRequestPtr; /**< Clone request pointer @range NA */
    const uint16                 SrcTimeBaseIdx;  /**< Index of source Time Base @range 0..65535 */
#endif
    const StbM_LocalTimeClockCfgType* LocalTimeClock; /**< Local time clock configuration pointer @range NA */
#if STBM_DISCIPLINED_CLOCK_USED == STD_ON
    const StbM_DisciplinedClockCfgType* DisciplinedClock; /**< Disciplined clock configuration pointer @range NA */
#endif
#if STBM_FALLBACK_VIRTUAL_LOCAL_TIME_USED == STD_ON
    const StbM_LocalTimeClockCfgType* FallbackTimeClock; /**< Fallback time clock configuration pointer @range NA */
#endif
#if (STBM_TIME_RECORDING_SUPPORT == STD_ON)
    const StbM_TimeRecordingCfgType* TimeRecording; /**< Time recording configuration pointer @range NA */
#endif
    uint16 NotificationCustomerNum; /**< This container holds the configuration of a notification customer, which is
                                       notified is informed about the occurance of a Time-base related event.
                                       @range 0..65535 */
    const StbM_NotificationCustomerCfgType*
        NotificationCustomer; /**< Notification customer configuration pointer @range NA */
    StbM_SynchronizedTimeBaseTypeType SynchronizedTimeBaseType; /**< Synchronized time base type @range NA */
    boolean                           IsTimeGateway;            /**< Is time gateway flag @range FALSE..TRUE */
#if STBM_MULTIPLE_PARTITION_ENABLED == STD_ON
    ApplicationType ApplicationId; /**< Application identifier @range NA */
#endif
#if STBM_STORE_TIMEBASE_NONVOLATILE_USED == STD_ON
    uint64  CyclicBackupInterval; /**< Cyclic backup interval @range 0..0xFFFFFFFFFFFFFFFF */
    uint64* BackupTimePtr;        /**< Backup time pointer @range NA */
#endif
#if STBM_TIME_VALIDATION_USED == STD_ON
    uint64 MaxProgressionMismatchThreshold; /**< Maximum progression mismatch threshold @range 0..0xFFFFFFFFFFFFFFFF */
#if STBM_CAN_SLAVE_TIME_VALIDATION_USED == STD_ON
    StbM_CanSlaveMeasurementQueueType*
        CanSlaveMeasurementQueuePtr; /**< CAN slave measurement queue pointer @range NA */
#endif
#if STBM_CAN_MASTER_TIME_VALIDATION_USED == STD_ON
    StbM_CanMasterMeasurementQueueType*
        CanMasterMeasurementQueuePtr; /**< CAN master measurement queue pointer @range NA */
#endif
#if STBM_ETH_SLAVE_TIME_VALIDATION_USED == STD_ON
    StbM_EthSlaveMeasurementQueueType*
        EthSlaveMeasurementQueuePtr; /**< ETH slave measurement queue pointer @range NA */
    StbM_PdelayInitiatorMeasurementQueueType*
        PdelayInitiatorMeasurementQueuePtr; /**< Pdelay initiator measurement queue pointer @range NA */
#endif
#if STBM_ETH_MASTER_TIME_VALIDATION_USED == STD_ON
    StbM_EthMasterMeasurementQueueType*
        EthMasterMeasurementQueuePtr; /**< ETH master measurement queue pointer @range NA */
    StbM_PdelayResponderMeasurementQueueType*
        PdelayResponderMeasurementQueuePtr; /**< Pdelay responder measurement queue pointer @range NA */
#endif
    uint16 TimeValidationRecordTableBlockCount; /**< Time validation record table block count @range 0..65535 */
#endif

#if STBM_CAN_SLAVE_TIME_VALIDATION_USED == STD_ON || STBM_ETH_SLAVE_TIME_VALIDATION_USED == STD_ON
    StbM_PdelayAndRateValidationType*
        PdelayAndRateValidationPtr; /**< Pdelay and rate validation configuration @range NA */
#endif
} StbM_SynchronizedTimeBaseCfgType;

/**
 * @brief defines Configuration of the StbM module.
 */
typedef struct
{
#if STBM_TRIGGERED_CUSTOMER_NUM > 0u
    const StbM_TriggeredCustomerCfgType* TriggeredCustomer; /**< Triggered customer configuration pointer @range NA */
#endif
    const StbM_SynchronizedTimeBaseCfgType*
        SynchronizedTimeBase; /**< Synchronized time base configuration pointer @range NA */
} StbM_ConfigType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

#ifdef __cplusplus
}
#endif

#endif
