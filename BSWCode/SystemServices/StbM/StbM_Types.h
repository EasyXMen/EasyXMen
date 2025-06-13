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
**  FILENAME    : StbM_Types.h                                                **
**                                                                            **
**  Created on  :                                                             **
**  Author      : yuzhe.zhang                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Type header for StbM                                        **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

#ifndef STBM_TYPES_H
#define STBM_TYPES_H

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Std_Types.h"
#include "StbM_Cfg.h"
#include "Rte_StbM_Type.h"

#if STBM_TRIGGERED_CUSTOMER_NUM > 0u
#include "Os.h"
#endif
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/

typedef enum
{
    CRC_IGNORED = 0u,
    CRC_NOT_VALIDATED,
    CRC_OPTIONAL,
    CRC_VALIDATED
} CrcValidatedType;

typedef enum
{
    CRC_NOT_SUPPORTED = 0u,
    CRC_SUPPORTED
} CrcSecuredType;

typedef enum
{
    STBM_TBTYPE_SYNCHRONIZED,
    STBM_TBTYPE_OFFSET,
    STBM_TBTYPE_PURELOCAL
} StbM_SynchronizedTimeBaseTypeType;

#ifndef _DEFINED_TYPEDEF_FOR_StbM_SyncRecordTableBlockType_
/* Synchronized Time Base Record Table Block */
typedef struct
{
    uint32 GlbSeconds;
    uint32 GlbNanoSeconds;
    StbM_TimeBaseStatusType TimeBaseStatus;
    uint32 VirtualLocalTimeLow;
    StbM_RateDeviationType RateDeviation;
    uint32 LocSeconds;
    uint32 LocNanoSeconds;
    uint32 PathDelay;
} StbM_SyncRecordTableBlockType;
#endif

#ifndef _DEFINED_TYPEDEF_FOR_StbM_OffsetRecordTableBlockType_
/* Offset Time Base Record Table Block */
typedef struct
{
    uint32 GlbSeconds;
    uint32 GlbNanoSeconds;
    StbM_TimeBaseStatusType TimeBaseStatus;
} StbM_OffsetRecordTableBlockType;
#endif

#ifndef _DEFINED_TYPEDEF_FOR_StbM_TimeBaseNotificationType_
/* The StbM_TimeBaseNotificationType type defines a number of global time related events.  */
#define STBM_STATUS_NTF_GLOBAL_TIME             0x0001u
#define STBM_STATUS_NTF_TIMEOUT_OCCURRED        0x0002u
#define STBM_STATUS_NTF_TIMEOUT_REMOVED         0x0004u
#define STBM_STATUS_NTF_TIMELEAP_FUTURE         0x0008u
#define STBM_STATUS_NTF_TIMELEAP_FUTURE_REMOVED 0x0010u
#define STBM_STATUS_NTF_TIMELEAP_PAST           0x0020u
#define STBM_STATUS_NTF_TIMELEAP_PAST_REMOVED   0x0040u
#define STBM_STATUS_NTF_SYNC_TO_SUBDOMAIN       0x0080u
#define STBM_STATUS_NTF_SYNC_TO_GLOBAL_MASTER   0x0100u
#define STBM_STATUS_NTF_RESYNC                  0x0200u
#define STBM_STATUS_NTF_RATECORRECTION          0x0400u
typedef uint16 StbM_TimeBaseNotificationType;
#endif

#if STBM_TRIGGERED_CUSTOMER_NUM > 0u
/* The triggered customer is directly triggered by the Synchronized Timebase Manager by getting synchronized with the
current (global) definition of time and passage of time. */
typedef struct
{
    uint16 triggeredCustomerPeriod;
    /* Mandatory reference to synchronized OS ScheduleTable, which will be
explicitly synchronized by the StbM. */
    uint16 StbMOSScheduleTableRef;
    uint8 indexOfSynchronizedTimeBase;
    TickType osScheduleTableDuration;
} StbMTriggeredCustomerCfgType;
#endif

typedef enum
{
    CALLBACK,
    CALLBACK_AND_SR_INTERFACE,
    NO_NOTIFICATION,
    SR_INTERFACE
} StbMNotificationInterfaceType;

typedef enum
{
    ETHTSYN_GLOBALTIME_DOMAIN,
    GPT_CHANNEL_CONFIGURATION,
    OS_COUNTER
} StbMLocalTimeHardwareCfgType;
/* Collects the information relevant for the rate- and offset correction of a Time Base. */
typedef struct
{
    /* This attribute describes whether the rate correction value of a Time Base
can be set by StbM_SetRateCorrection() */
    boolean StbMAllowMasterRateCorrection;
    /* This attribute describes the maximum allowed absolute value of the rate
deviation value to be set by StbM_SetRateCorrection() */
    uint16 StbMMasterRateDeviationMax;
    /* Defines the interval during which the adaptive rate correction cancels out
the rate- and time deviation */
    uint64 StbMOffsetCorrectionAdaptionInterval;
    /* Threshold for the correction method. Deviations below this value will be
corrected by a linear reduction over a defined timespan. */
    uint64 StbMOffsetCorrectionJumpThreshold;
    /* Definition of the time span [s] which is used to calculate the rate deviation */
    uint64 StbMRateCorrectionMeasurementDuration;
    /* Number of simultaneous rate measurements to determine the current rate deviation */
    uint16 StbMRateCorrectionsPerMeasurementDuration;
} StbMTimeCorrectionCfgType;
/* References the hardware reference clock of this Synchronized Time Base. */
typedef struct
{
    /* Represents the frequency [Hz] of the HW reference clock used by the StbM. */
    uint32 StbMClockFrequency;
    /* Represents the prescaler to calculate the resulting frequency of the HW
reference clock used by the StbM. */
    uint32 StbMClockPrescaler;
    /* Reference to the local time hardware. */
    uint8 StbMLocalTimeHardware;
    StbMLocalTimeHardwareCfgType StbMLocalTimeHardwareType;
    uint32 LocalTimeMax;
} StbMLocalTimeClockCfgType;
/* Collects the information relevant for configuration of the precision
measurement of a Time Base. */
#if (STBM_TIME_RECORDING_SUPPORT == STD_ON)
typedef struct
{
    /* Name of the customer specific callback function, which shall be called, if a
measurement data for a Offset Time Base are available. */
    P2FUNC(void, TYPEDEF, StbMOffsetTimeRecordBlockCallback)
    (P2CONST(StbM_OffsetRecordTableBlockType, AUTOMATIC, STBM_APPL_DATA) offsetRecordTableBlock);
    /* Represents the number of Blocks used for queing time measurement
events for the Offset Time Base Record Table. */
    uint16 StbMOffsetTimeRecordTableBlockCount;
    /* Name of the customer specific callback function, which shall be called, if a
    measurement data for a Synchronized Time Base are available. */
    P2FUNC(void, TYPEDEF, StbMSyncTimeRecordBlockCallback)
    (P2CONST(StbM_SyncRecordTableBlockType, AUTOMATIC, STBM_APPL_DATA) syncRecordTableBlock);
    /* Represents the number of Blocks used for queing time measurement
events for the Synchronized Time Base Record Table. */
    uint16 StbMSyncTimeRecordTableBlockCount;
} StbMTimeRecordingCfgType;
#endif

typedef struct
{
    StbM_SynchronizedTimeBaseType timeBaseId;
    /* Name of the customer specific notification callback function, which shall be
called, if the time previously set by the customer is reached. */
    void (*StbMTimeNotificationCallback)(StbM_TimeDiffType deviationTime);
} StbMNotificationCustomerCfgType;

/* Synchronized time.base collects the information about a specific time-base
provider within the system. */
typedef struct
{
    /* This attribute describes the required number of updates to the Time Base */
    uint16 StbMClearTimeleapCount;
    /* This parameter shall be set to true for a Global Time Master that acts as a
system-wide source of time information with respect to Global Time. */
    boolean StbMIsSystemWideGlobalTimeMaster;
#if STBM_STATUS_NOTIFICATIONS_ENABLED == STD_ON
    /* The parameter defines what type of interface shall be used to notify a customer of a
status event. */
    StbMNotificationInterfaceType StbMNotificationInterface;
    /* Name of the customer specific status notification callback function, which
shall be called, if a non-masked status event occurs. */
    void (*StbMStatusNotificationCallback)(StbM_TimeBaseNotificationType eventNotification);
    /* The parameter defines the initial value for NotificationMask mask, which defines the
     *  events for which the event notification callback function shall be called. */
    StbM_TimeBaseNotificationType statusNotificationMask;
#endif
    /* Identification of a Synchronized TimeBase via a unique identifier. */
    uint16 StbMSynchronizedTimeBaseIdentifier;
    /* This attribute describes the timeout for the situation that the time
synchronization gets lost in the scope of the time domain */
    uint64 StbMSyncLossTimeout;
    /* This represents the maximum allowed positive difference between a newly
received Global Time Base value and the current Local Time Base value */
    uint64 StbMTimeLeapFutureThreshold;
    /* This represents the maximum allowed negative difference between the
current Local Time Base value and a newly received Global Time Base value */
    uint64 StbMTimeLeapPastThreshold;
    /* This is the reference to the Synchronized Time-Base this Offset Time-Base
is based on. */
    const uint16* StbMOffsetTimeBase;
    const StbMTimeCorrectionCfgType* StbMTimeCorrection;
    const StbMLocalTimeClockCfgType* StbMLocalTimeClock;
#if (STBM_TIME_RECORDING_SUPPORT == STD_ON)
    const StbMTimeRecordingCfgType* StbMTimeRecording;
#endif
    /* This container holds the configuration of a notification customer, which is
notified is informed about the occurance of a Time-base related event. */
    uint16 StbMNotificationCustomerNum;
    const StbMNotificationCustomerCfgType* StbMNotificationCustomer;
    StbM_SynchronizedTimeBaseTypeType synchronizedTimeBaseType;
    boolean StbMisTimeGateway;
} StbMSynchronizedTimeBaseCfgType;

/*******************************************************************************
**                      Internal                                              **
*******************************************************************************/
typedef uint64 StbM_HwCounterType;
typedef uint64 StbM_LocalTimeType;

typedef struct
{
    uint64 GlobalTime;
    uint64 VirtualLocalTime;
} StbM_MainTimeTupleType;

typedef struct
{
    uint64 TGstart;
    uint64 TVstart;
} StbM_RateCorrectionType;

typedef struct
{
    StbM_HwCounterType preHwCounter;
    StbM_LocalTimeType localTimeFraction;
    StbM_LocalTimeType localTime;
} StbM_LocalClockTimeType;

typedef struct
{
#if (STBM_TIME_CORRECTION_SUPPORT == STD_ON)
    float64 ratio;
    float64 roc;
#endif
    boolean ratioSet;
} StbM_CurrentRateType;

#if (STBM_NOTIFICATION_CUSTOMER_NUM > 0)
typedef struct
{
    uint64 CustomerTimerExpireTime;
    boolean GPTtimercalled;
    boolean isrunning;
} StbM_NotificationType;
#endif

typedef enum
{
    RESERVED_NOT_ALLOWED,
    PURELOCAL_TIMEBASE_NOT_ALLOWED,
    OFFSET_TIMEBASE_NOT_ALLOWED,
    SYNC_TIMEBASE_NOT_ALLOWED,
    SYNC_AND_PURELOCAL_TIMEBASE_NOT_ALLOWED,
    OFFSET_AND_PURELOCAL_TIMEBASE_NOT_ALLOWED
} StbM_TimeBaseIdAllowedType;

typedef struct
{
    StbM_TimeBaseStatusType timeBaseStatus;
    uint8 StbMTimeBaseUpdateCounter;
#if STBM_STATUS_NOTIFICATIONS_ENABLED == STD_ON
    StbM_TimeBaseNotificationType notificationEvents;
#endif
    StbM_UserDataType UserData;
    StbM_TimeStampType Offset;
    StbM_MainTimeTupleType StbMMainTimeTuple;
    uint16 StbMClearTimeleapCount;
    StbM_TimeDiffType TimeLeap;
    boolean BusNotFirstSet;
    uint64 lastBusSettime;
    StbM_LocalClockTimeType localClockTime;
    StbM_CurrentRateType RateDeviation;
    StbM_RateCorrectionType* RateCorrection;
#if (STBM_TIME_CORRECTION_SUPPORT == STD_ON)
    sint64 TimeOffsetForRateAdaption;
    uint64 RemainTimeOffsetForRateAdaption;
#endif
#if (STBM_TIME_RECORDING_SUPPORT == STD_ON)
    P2VAR(StbM_SyncRecordTableBlockType, AUTOMATIC, STBM_APPL_DATA) StbMSyncTimeRecordBlock;
    P2VAR(StbM_OffsetRecordTableBlockType, AUTOMATIC, STBM_APPL_DATA) StbMOffsetTimeRecordBlock;
    uint16 RecordBlockIndex;
    uint16 RecordBlockCount;
#endif
} StbM_TimeBaseType;

#define STBM_SYNC_TIMEBASE_RANGE      16U
#define STBM_OFFSET_TIMEBASE_RANGE    32U
#define STBM_PURELOCAL_TIMEBASE_RANGE 128U
#define STBM_NANOSECOND_TO_SECOND     0x3B9ACA00u

#endif
