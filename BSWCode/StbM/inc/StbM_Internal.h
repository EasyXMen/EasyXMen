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
 **  @file               : StbM_Internal.h
 **  @author             : xiongfei.shi
 **  @date               : 2024/12/26
 **  @vendor             : isoft
 **  @description        : internal data types of the StbM module
 **
 ***********************************************************************************************************************/

#ifndef STBM_INTERNAL_H_
#define STBM_INTERNAL_H_

/* =================================================== inclusions =================================================== */
#include "StbM_Cfg.h"
#include "StbM_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */

/* ===================================================== macros ===================================================== */
#define STBM_UNUSED(a) (void)(a) /**< Macro to mark a parameter as unused */

#ifndef STBM_LOCAL_INLINE
#define STBM_LOCAL_INLINE static inline /**< Macro for defining local inline functions */
#endif

#define STBM_TIMEBASE_ID_RESERVE     128U                    /**< Maximum value for timebase id reserve */
#define STBM_NS_PER_S                (1000000000UL)          /**< Nanoseconds per second constant */
#define STBM_NS_PER_US               (1000ULL)               /**<  Nanoseconds per microsecond constant */
#define STBM_RATE_PPM                (1000000)               /**< Parts per million for rate calculations */
#define STBM_GPT_TIMER_TICK_DURATION (1ULL)                  /**< GPT timer tick duration in nanoseconds. 1us */
#define STBM_TIME_64_INVALID         (0xFFFFFFFFFFFFFFFFULL) /**< Invalid 64-bit time value */
#define STBM_UINT32_MAX              (0xFFFFFFFFU)           /**< Maximum value for 32-bit unsigned integer */

/* ================================================ type definitions ================================================ */
/**
 * @brief Type definition for time notification callback function
 */
typedef void (*StbMTimeNtfCallbackType)(StbM_TimeDiffType diff);

/**
 * @brief Type definition for hardware counter values
 */
typedef uint64 StbM_HwCounterType;

/**
 * @brief Type definition for local time values
 */
typedef uint64 StbM_LocalTimeType;

/**
 * @brief Structure for main time tuple
 */
typedef struct
{
    uint64 TL; /**< Local time value @range 0..0xFFFFFFFFFFFFFFFF */
    uint64 TV; /**< Virtual time value @range 0..0xFFFFFFFFFFFFFFFF */
#if STBM_FALLBACK_VIRTUAL_LOCAL_TIME_USED == STD_ON
    uint64 TVFallback; /**< Fallback virtual time value @range 0..0xFFFFFFFFFFFFFFFF */
#endif
} StbM_Time64TupleType;

/**
 * @brief Structure for rate correction tuple
 */
typedef struct
{
    uint64 TGstart; /**< Start time for global time @range 0..0xFFFFFFFFFFFFFFFF */
    uint64 TVstart; /**< Start time for virtual time @range 0..0xFFFFFFFFFFFFFFFF */
} StbM_RateCorrectionTupleType;

/**
 * @brief Structure for rate correction data
 */
typedef struct
{
    StbM_RateCorrectionTupleType* Array;      /**< Pointer to rate correction tuple array @range NA */
    uint32                        StartIndex; /**< Start index in the array @range 0..STBM_UINT32_MAX */
    uint32                        StopIndex;  /**< Stop index in the array @range 0..STBM_UINT32_MAX */
} StbM_RateCorrectionType;

/**
 * @brief Structure for current rate information
 */
typedef struct
{
    sint32  RrcValue; /**< Rate correction value @range -2147483648..2147483647 */
    sint32  RocValue; /**< Rate offset correction value @range -2147483648..2147483647 */
    boolean IsSet;    /**< Flag indicating if rate values are set @range FALSE/TRUE */
} StbM_CurrentRateType;

#if (STBM_NOTIFICATION_CUSTOMER_NUM > 0)
/**
 * @brief Structure for notification data
 */
typedef struct
{
    uint64  CustomerTimerExpireTime; /**< Time when customer timer expires @range 0..0xFFFFFFFFFFFFFFFF */
    boolean IsSet;                   /**< Flag indicating if notification is set @range FALSE..TRUE */
    boolean GPTtimercalled;          /**< Flag indicating if GPT timer was called @range FALSE..TRUE */
    boolean Isrunning;               /**< Flag indicating if notification is running @range FALSE/TRUE */
} StbM_NotificationType;
#endif

#if STBM_CAN_SLAVE_TIME_VALIDATION_USED == STD_ON || STBM_ETH_SLAVE_TIME_VALIDATION_USED == STD_ON
/**
 * @brief Structure for rate validation time triple
 */
typedef struct
{
    uint64 TGRx;   /**< Received global time @range 0..0xFFFFFFFFFFFFFFFF */
    uint64 TVRx;   /**< Received virtual time @range 0..0xFFFFFFFFFFFFFFFF */
    uint64 TLSync; /**< Synchronization local time @range 0..0xFFFFFFFFFFFFFFFF */
} StbM_RateValidationTimeTripleType;
#endif

#if STBM_TIME_VALIDATION_USED == STD_ON
/**
 * @brief Structure for time validation data information
 */
typedef struct
{
    uint16 Front;
    uint16 Size;
} StbM_TimeValidataionDataInfoType;
#endif

/**
 * @brief Structure for time base data
 */
typedef struct
{
    StbM_TimeBaseStatusType TimeBaseStatus;        /**< Status of the time base @range NA */
    uint8                   TimeBaseUpdateCounter; /**< Counter for time base updates @range 0..255 */
#if STBM_STATUS_NOTIFICATIONS_ENABLED == STD_ON
    StbM_TimeBaseNotificationType NotificationEvents; /**< Notification events for the time base @range NA */
#endif
    StbM_UserDataType    UserData;      /**< User data associated with the time base @range NA */
    StbM_Time64TupleType MainTimeTuple; /**< Main time tuple for the time base @range NA */
#if STBM_TIME_VALIDATION_USED == STD_ON
    StbM_Time64TupleType PreMainTimeTuple; /**< Previous main time tuple for validation @range NA */
#endif
    uint16            ClearTimeleapCount; /**< Count of time leap clear operations @range 0..65535 */
    StbM_TimeDiffType TimeLeap;           /**< Time leap value @range NA */
    boolean           TimeLeapValid;      /**< Flag indicating if time leap is valid @range FALSE..TRUE */
    boolean           BusNotFirstSet;     /**< Flag indicating if bus time was not first set @range FALSE..TRUE */
    uint64            LastBusSettime;     /**< Time of last bus time setting @range 0..0xFFFFFFFFFFFFFFFF */
    uint64            MainTupleUpdateCnt; /**< Count of main tuple updates @range 0..0xFFFFFFFFFFFFFFFF */
#if STBM_STORE_TIMEBASE_NONVOLATILE_USED == STD_ON
    uint64 BackupTimeCnt; /**< Backup time count @range 0..0xFFFFFFFFFFFFFFFF */
#endif
    StbM_CurrentRateType RateDeviation; /**< Rate deviation information @range NA */
#if (STBM_TIME_CORRECTION_SUPPORT == STD_ON)
    StbM_RateCorrectionType RateCorrection; /**< Rate correction information @range NA */
#endif
#if (STBM_TIME_RECORDING_SUPPORT == STD_ON)
    StbM_SyncRecordTableBlockType* SyncTimeRecordBlock; /**< Pointer to sync time record block @range NA */
    uint16                         RecordBlockIndex;    /**< Index in the record block @range 0..65535 */
    uint16                         RecordBlockCount;    /**< Count of record blocks @range 0..65535 */
#endif

#if STBM_CAN_SLAVE_TIME_VALIDATION_USED == STD_ON || STBM_ETH_SLAVE_TIME_VALIDATION_USED == STD_ON
    StbM_RateValidationTimeTripleType RateValidationTimeTriple0; /**< First rate validation time triple @range NA */
    StbM_RateValidationTimeTripleType
          RateValidationTimeTriplePrev; /**< Previous rate validation time triple @range NA */
    uint8 RateValidationInterval;       /**< Rate validation interval @range 0..255 */
#endif
#if STBM_CAN_SLAVE_TIME_VALIDATION_USED == STD_ON
    StbM_TimeValidataionDataInfoType
        CanSlaveTimeValidationDataInfo; /**< Time validation data information for CAN slave time validation @range NA */
#endif
#if STBM_CAN_MASTER_TIME_VALIDATION_USED == STD_ON
    StbM_TimeValidataionDataInfoType CanMasterTimeValidationDataInfo; /**< Time validation data information for CAN
                                                                         master time validation @range NA */
#endif
#if STBM_ETH_SLAVE_TIME_VALIDATION_USED == STD_ON
    StbM_TimeValidataionDataInfoType
        EthSlaveTimeValidationDataInfo; /**< Time validation data information for ETH slave time validation @range NA */
    StbM_TimeValidataionDataInfoType PdelayInitiatorValidationDataInfo; /**< Time validation data information for Pdelay
                                                                           initiator validation @range NA */
#endif
#if STBM_ETH_MASTER_TIME_VALIDATION_USED == STD_ON
    StbM_TimeValidataionDataInfoType EthMasterTimeValidationDataInfo;   /**< Time validation data information for ETH
                                                                           master time validation @range NA */
    StbM_TimeValidataionDataInfoType PdelayResponderValidationDataInfo; /**< Time validation data information for Pdelay
                                                                           responder validation @range NA */
#endif
} StbM_TimeBaseType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */
#if (STBM_TIME_RECORDING_SUPPORT == STD_ON)
#define STBM_START_SEC_CONST_PTR
#include "StbM_MemMap.h"
/**
 * @brief External declaration for sync time record block pointer array
 */
extern StbM_SyncRecordTableBlockType* const StbM_SyncTimeRecordBlockPtr[STBM_TIME_BASE_NUM]; /**< @range NA */
#define STBM_STOP_SEC_CONST_PTR
#include "StbM_MemMap.h"
#endif

#if (STBM_TIME_CORRECTION_SUPPORT == STD_ON)
#define STBM_START_SEC_CONST_PTR
#include "StbM_MemMap.h"
/**
 * @brief External declaration for rate correction pointer array
 */
extern StbM_RateCorrectionTupleType* const StbM_RateCorrectionPtr[STBM_TIME_BASE_NUM]; /**< @range NA */
#define STBM_STOP_SEC_CONST_PTR
#include "StbM_MemMap.h"
#endif

#if (STBM_NOTIFICATION_CUSTOMER_NUM > 0u)
#define STBM_START_SEC_CONST_PTR
#include "StbM_MemMap.h"
/**
 * @brief External declaration for notification pointer array
 */
extern StbM_NotificationType* const StbM_NotificationPtr[STBM_NOTIFICATION_CUSTOMER_NUM]; /**< @range NA */
#define STBM_STOP_SEC_CONST_PTR
#include "StbM_MemMap.h"
#endif

#if STBM_MULTIPLE_PARTITION_ENABLED == STD_ON
#define STBM_START_SEC_CONFIG_DATA_32
#include "StbM_MemMap.h"
/**
 * @brief External declaration for OS application list
 */
extern const ApplicationType StbM_OsApplicationList[STBM_PARTITION_NUMBER]; /**< @range NA */
#define STBM_STOP_SEC_CONFIG_DATA_32
#include "StbM_MemMap.h"
#endif

#if (STBM_NOTIFICATION_CUSTOMER_NUM > 0)
#define STBM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "StbM_MemMap.h"
/**
 * @brief External declaration for notification customer configuration
 */
extern const StbM_NotificationCustomerCfgType
    StbM_NotificationCustomer[STBM_NOTIFICATION_CUSTOMER_NUM]; /**< @range NA */
#define STBM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "StbM_MemMap.h"
#endif

#ifdef __cplusplus
}
#endif

#endif
