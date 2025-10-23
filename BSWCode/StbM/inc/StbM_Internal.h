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
#include "Rte_StbM_Type.h"
#include "StbM_Cfg.h"
#include "StbM_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */

/* ===================================================== macros ===================================================== */
#define STBM_UNUSED(a) (void)(a)

#ifndef STBM_LOCAL
#define STBM_LOCAL static /* PRQA S 3414 */ /* VL_StbM_3414 */
#endif

#ifndef STBM_LOCAL_INLINE
#define STBM_LOCAL_INLINE static inline
#endif

#define STBM_TIMEBASE_ID_RESERVE     128U
#define STBM_NS_PER_S                (1000000000UL) /* 0x3B9ACA00u */
#define STBM_NS_PER_US               (1000ULL)
#define STBM_RATE_PPM                (1000000)
#define STBM_GPT_TIMER_TICK_DURATION (1ULL) /* 1us */

/* ================================================ type definitions ================================================ */
typedef void (*StbMTimeNtfCallbackType)(StbM_TimeDiffType diff);

typedef uint64 StbM_HwCounterType;
typedef uint64 StbM_LocalTimeType;

typedef struct StbM_MainTimeTupleTag
{
    uint64 GlobalTime;
    uint64 VirtualLocalTime;
} StbM_MainTimeTupleType;

typedef struct StbM_RateCorrectionTupleTag
{
    uint64 TGstart;
    uint64 TVstart;
} StbM_RateCorrectionTupleType;

typedef struct StbM_RateCorrectionTag
{
    StbM_RateCorrectionTupleType* Array;
    uint32                        StartIndex;
    uint32                        StopIndex;
} StbM_RateCorrectionType;

typedef struct StbM_LocalClockTimeTag
{
    StbM_HwCounterType PreHwCounter;
    StbM_LocalTimeType LocalTimeFraction;
    StbM_LocalTimeType LocalTime;
} StbM_LocalClockTimeType;

typedef struct StbM_CurrentRateTag
{
    sint32  RrcValue;
    sint32  RocValue;
    boolean IsSet;
} StbM_CurrentRateType;

#if (STBM_NOTIFICATION_CUSTOMER_NUM > 0)
typedef struct StbM_NotificationTag
{
    uint64  CustomerTimerExpireTime;
    boolean IsSet;
    boolean GPTtimercalled;
    boolean Isrunning;
} StbM_NotificationType;
#endif

typedef struct StbM_TimeBaseTag
{
    StbM_TimeBaseStatusType TimeBaseStatus;
    uint8                   TimeBaseUpdateCounter;
#if STBM_STATUS_NOTIFICATIONS_ENABLED == STD_ON
    StbM_TimeBaseNotificationType NotificationEvents;
#endif
    StbM_UserDataType       UserData;
    StbM_MainTimeTupleType  MainTimeTuple;
    uint16                  ClearTimeleapCount;
    StbM_TimeDiffType       TimeLeap;
    boolean                 TimeLeapValid;
    boolean                 BusNotFirstSet;
    uint64                  LastBusSettime;
    StbM_LocalClockTimeType LocalClockTime;
    StbM_CurrentRateType    RateDeviation;
#if (STBM_TIME_CORRECTION_SUPPORT == STD_ON)
    StbM_RateCorrectionType RateCorrection;
#endif
#if (STBM_TIME_RECORDING_SUPPORT == STD_ON)
    StbM_SyncRecordTableBlockType* SyncTimeRecordBlock;
    uint16                         RecordBlockIndex;
    uint16                         RecordBlockCount;
#endif
} StbM_TimeBaseType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */
extern StbM_TimeBaseType* const StbM_TimeBasePtr[STBM_TIME_BASE_NUM];
#if (STBM_TIME_RECORDING_SUPPORT == STD_ON)
extern StbM_SyncRecordTableBlockType* const StbM_SyncTimeRecordBlockPtr[STBM_TIME_BASE_NUM];
#endif

#if (STBM_TIME_CORRECTION_SUPPORT == STD_ON)
extern StbM_RateCorrectionTupleType* const StbM_RateCorrectionPtr[STBM_TIME_BASE_NUM];
#endif

#if (STBM_NOTIFICATION_CUSTOMER_NUM > 0u)
extern StbM_NotificationType* const StbM_NotificationPtr[STBM_NOTIFICATION_CUSTOMER_NUM];
#endif

#if STBM_MULTIPLE_PARTITION_ENABLED == STD_ON
extern const ApplicationType StbM_OsApplicationList[STBM_PARTITION_NUMBER];
#endif

#if (STBM_NOTIFICATION_CUSTOMER_NUM > 0)
extern const StbM_NotificationCustomerCfgType StbM_NotificationCustomer[STBM_NOTIFICATION_CUSTOMER_NUM];
#endif

/* ========================================= external function declarations ========================================= */
/**
 * @brief        This function needs to be called periodically to perform the AUTOSAR StbM activities
 * @reentrant    TRUE (Reentrant for different partitions. Non reentrant for the same partition.)
 * @synchronous  FALSE
 * @trace        CPD-71872
 */
void StbM_MainFunction(void);

#ifdef __cplusplus
}
#endif

#endif
