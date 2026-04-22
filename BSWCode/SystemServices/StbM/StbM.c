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
/*********************************************************************************
**                                                                            **
**  FILENAME    : StbM.c                                                      **
**                                                                            **
**  Created on  :                                                             **
**  Author      : yuzhe.zhang                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation for StbM                                     **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/******************************************************************************
**                      Revision Control History                             **
******************************************************************************/
/*  <VERSION>    <DATE>    <AUTHOR>        <REVISION LOG>
 *  V2.0.0      20200716  yuzhe.zhang      Initial version
 *  V2.0.1      20201202  yuzhe.zhang      UserDatalength check
 *  V2.0.2      20210630  Xinrun.Wang      TimeCorrection Relatede Issues Fixed
 *  V2.0.3      20230105  Xinrun.Wang      1. Correct RateCorrection algorithm
 *                                         2. Check for RateAdaption Interval equal to zero
 *                                         3. Add period for triggered customers
 *                                         4. Adjust mainfunction index processing for TimeCorrction
 *                                         5. Initialize RecordBlockPtr, Add static RecordBlock Array
 *                                         6. Adjust RecordBlockIndex index counting
 *                                         7. rateDeviation Handle ppm unit for TimeRecord
 *  V2.0.4      20230307  Xinrun.Wang      1. Add VLT access at mainfunction to avoid HW clock recording overflow
 *                                         2. Record current VLT when Set/UpdateGlobalTime to reset VLT
 *  V2.0.5      20230421  Xinrun.Wang      1. Adjust offset time base getCurrentTime to show offset
 *                                         2. Seperate Offset/Rate Correction, correct perMeasurement index
 *  V2.0.6      20230516  Xinrun.Wang      fixed RateCorrection duration
 *  V2.0.7      20230615  Xinrun.Wang      Correct timeSyncLoss algorithm
 *  V2.0.8      20230815  Xiaojian.liang   Fixed CPT-6511, Loop error for StbM_Init
 *  V2.0.9      20231108  Xiaojian.liang   Fixed CPT-7410, StbMTriggeredCustomer does not work
 *  V2.0.10     20231206  Xiaojian.liang   CPD-33590 Performance optimization
 *                                         Fixed CPT-7751, Fixed rate deviation error due to integer arithmetic
 *  V2.0.11     20240320  Xiaojian.liang   CPT-7684 fixed error in converting clock ticks to local time
 *                                         StbM_Init starts the GPT timer.
 *              20240409  Xiaojian.liang   CPT-8583 Can't trigger the status notification callback if a timeout occurs
 *  V2.0.12     20240510  Han.li           CPT-8508 Remove StbMStoreTimebaseNonVolatile related code
 *              20240620  Han.li           CPT-9255 Modify the initialization of "ret" in StbM_StartTimer()
 *              20241127  xiongfei.shi     Fixed CPT-8763, Accuracy loss problem of StbM_GetRateDeviation()
 *                                         Fixed CPT-9091, Miscalculation of the entry parameter of Gpt_StartTimer()
 *  V2.0.13     20250613  shuangyang.fu    Fixed CPT-9349 CPT-9350,correct the logic of rate correction
 */

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "StbM.h"
#if (STBM_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif
/* SWS_StbM_00065 */
#if ((STBM_TRIGGERED_CUSTOMER_NUM > 0) || (STBM_OS_TIMESTAMP_SUPPORT))
#include "Os.h"
#endif
/* SWS_StbM_00426 */
#if ((STBM_GPT_TIMESTAMP_SUPPORT == STD_ON) || (STBM_NOTIFICATION_CUSTOMER_NUM > 0))
#include "Gpt.h"
#endif
#include "SchM_StbM.h"
#include "StbM_Cfg.h"
/* SWS_StbM_00246 */
#if (STBM_ETHIF_TIMESTAMP_SUPPORT == STD_ON)
#include "EthIf.h"
#endif
/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/

/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
#ifndef STBM_LOCAL
#define STBM_LOCAL static
#endif

#ifndef STBM_LOCAL_INLINE
#define STBM_LOCAL_INLINE static inline
#endif

#define STBM_NS_PER_S 1000000000u

#define StbM_GetTriggerCustomerConfig(index) (&StbM_ConfigData->StbMTriggeredCustomer[index])
#define TIMEBASE_CFG(timebasecfgid)          StbM_ConfigData->StbMSynchronizedTimeBase[timebasecfgid]
#define STBM_UNUSED(a)                       (void)(a)

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/
typedef uint16 StbM_TimeBaseIndexType;
typedef void (*StbMTimeNtfCallbackType)(StbM_TimeDiffType diff);

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
#if (STBM_DEV_ERROR_DETECT == STD_ON)
static Std_ReturnType CheckTimeBaseIdType(StbM_SynchronizedTimeBaseType timeBaseId, StbM_TimeBaseIdAllowedType type);
static Std_ReturnType
    Stbm_TimeBaseIdDetCheck(StbM_SynchronizedTimeBaseType timeBaseId, uint8 serviceid, StbM_TimeBaseIdAllowedType type);
#endif
STBM_LOCAL StbM_TimeBaseIndexType StbM_FindSynchronizedTimeBase(StbM_SynchronizedTimeBaseType timeBaseId);
STBM_LOCAL Std_ReturnType StbM_GetCurrentLocalTime(StbM_TimeBaseIndexType timeBaseIndex, StbM_LocalTimeType* localTime);
STBM_LOCAL void           StbM_GetGlobalTime64(uint16 timeBaseIndex, uint64* timeStamp);
static void               StbM_UpdateMainTimeTuple(uint16 timebasecfgid, uint64 TL, uint64 TV);
static uint64             StbM_GTTimeTo64Time(StbM_TimeStampType timeStamp);
static StbM_TimeStampType StbM_64TimeToGTTime(uint64 time64);
static void               StbM_CheckTimeleap(uint16 timebasecfgid, uint64 timenow, uint64 timereceived);
static void               StbM_CheckTimeout(uint16 timebasecfgid, uint64 timenow);

#if (STBM_TIME_CORRECTION_SUPPORT == STD_ON)
STBM_LOCAL void StbM_CalculateRrc(
    uint16                  timeBaseIndex,
    const uint64*           TGPtr,
    const uint64*           TVPtr,
    StbM_TimeBaseStatusType timeBaseStatus);
STBM_LOCAL void StbM_CalculateRoc(uint16 timeBaseIndex, uint64* TGPtr, const uint64* TVPtr);
STBM_LOCAL void StbM_TimeCorrection(
    uint16                  timeBaseIndex,
    uint64*                 TGPtr,
    const uint64*           TVPtr,
    StbM_TimeBaseStatusType timeBaseStatus);
STBM_LOCAL_INLINE uint64 StbM_Abs(sint64 value);
#endif

#if STBM_STATUS_NOTIFICATIONS_ENABLED == STD_ON
STBM_LOCAL void StbM_OnChangeTimeBaseStatus(
    StbM_TimeBaseNotificationType* notificationEvents,
    StbM_TimeBaseStatusType        oldTimeBaseStatus,
    StbM_TimeBaseStatusType        newTimeBaseStatus);
STBM_LOCAL void StbM_CheckStatusNotification(StbM_TimeBaseIndexType timeBaseIndex);
#endif

#if (STBM_TIME_RECORDING_SUPPORT == STD_ON)
static void
    StbM_TimeRecord(StbM_SynchronizedTimeBaseType timeBaseId, uint64 TLreceived, uint32 TVLoreceived, uint32 pathDelay);
static void StbM_TimeRecordback(StbM_SynchronizedTimeBaseType timeBaseId);
#endif
static void StbM_SetUserDataIn(uint16 timebasecfgid, StbM_UserDataType userData);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
#if (STBM_NOTIFICATION_CUSTOMER_NUM > 0)
extern const StbMNotificationCustomerCfgType StbMNotificationCustomer[STBM_NOTIFICATION_CUSTOMER_NUM];
#endif
#define STBM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "StbM_MemMap.h"
STBM_LOCAL StbM_TimeBaseType StbM_TimeBase[STBM_SYNCHRONIZED_TIME_BASE_NUM];
#if (STBM_MAX_SYNC_TIMERECORDTABLE_BLOCK_COUNT > 0)
STBM_LOCAL StbM_SyncRecordTableBlockType
    StbM_SyncTimeRecordBlock[STBM_SYNCHRONIZED_TIME_BASE_NUM][STBM_MAX_SYNC_TIMERECORDTABLE_BLOCK_COUNT];
#endif
#if (STBM_MAX_OFFSET_TIMERECORDTABLE_BLOCK_COUNT > 0)
STBM_LOCAL StbM_OffsetRecordTableBlockType
    StbM_OffsetTimeRecordBlock[STBM_SYNCHRONIZED_TIME_BASE_NUM][STBM_MAX_OFFSET_TIMERECORDTABLE_BLOCK_COUNT];
#endif

#if (STBM_NOTIFICATION_CUSTOMER_NUM > 0)
STBM_LOCAL StbM_NotificationType StbM_Notification[STBM_NOTIFICATION_CUSTOMER_NUM];
#endif

STBM_LOCAL const StbM_ConfigType* StbM_ConfigData;

#if (STBM_TIME_CORRECTION_SUPPORT == STD_ON)
STBM_LOCAL StbM_RateCorrectionTupleType
    StbM_RateCorrection[STBM_SYNCHRONIZED_TIME_BASE_NUM][STBM_MAX_PER_MEASUREMENT_DURATION + 1];
#endif
#define STBM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "StbM_MemMap.h"

/*Module initialize status, TRUE initialized, FALSE not initialized*/
#define STBM_START_SEC_VAR_CLEARED_BOOLEAN
#include "StbM_MemMap.h"
STBM_LOCAL boolean StbM_InitStatus = FALSE;
#define STBM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "StbM_MemMap.h"

#if (STBM_TRIGGERED_CUSTOMER_NUM > 0)
#define STBM_START_SEC_VAR_CLEARED_16
#include "StbM_MemMap.h"
STBM_LOCAL uint16 StbM_TriggerRemainPeriod[STBM_TRIGGERED_CUSTOMER_NUM];
#define STBM_STOP_SEC_VAR_CLEARED_16
#include "StbM_MemMap.h"
#endif

/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * Initializes the Synchronized Time-base Manager
 * Service ID: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): @ConfigPtr, Pointer to the selected configuration set.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
void StbM_Init(const StbM_ConfigType* ConfigPtr)
{
#if ((STBM_TIME_CORRECTION_SUPPORT == STD_ON) || (STBM_NOTIFICATION_CUSTOMER_NUM > 0u))
    uint16 subIndex;
#endif

#if (STBM_DEV_ERROR_DETECT == STD_ON)
    if (ConfigPtr == NULL_PTR)
    {
        (void)Det_ReportError(STBM_MODULE_ID, STBM_INSTANCE_ID, STBM_SID_INIT, STBM_E_INIT_FAILED);
    }
    else
#endif
    {
        StbM_ConfigData = ConfigPtr;
        for (uint16 index = 0u; index < STBM_SYNCHRONIZED_TIME_BASE_NUM; index++)
        {
            StbM_TimeBaseType* timeBasePtr = &StbM_TimeBase[index];
            /* SWS_StbM_00170 */
            timeBasePtr->timeBaseStatus = 0x00u;
            /* SWS_StbM_00345 */
#if STBM_STATUS_NOTIFICATIONS_ENABLED == STD_ON
            timeBasePtr->notificationEvents = 0u;
#endif
            /* SWS_StbM_00344 */
            timeBasePtr->StbMTimeBaseUpdateCounter = 0u;
            /* SWS_StbM_00427 */
            timeBasePtr->UserData.userDataLength = 0u;
            timeBasePtr->UserData.userByte0      = 0u;
            timeBasePtr->UserData.userByte1      = 0u;
            timeBasePtr->UserData.userByte2      = 0u;
            /* Initialize */
            timeBasePtr->BusNotFirstSet                     = (boolean)FALSE;
            timeBasePtr->lastBusSettime                     = 0u;
            timeBasePtr->StbMMainTimeTuple.GlobalTime       = 0u;
            timeBasePtr->StbMMainTimeTuple.VirtualLocalTime = 0u;
            timeBasePtr->Offset.nanoseconds                 = 0u;
            timeBasePtr->Offset.seconds                     = 0u;
            timeBasePtr->Offset.secondsHi                   = 0u;
            timeBasePtr->TimeLeap                           = 0;
            timeBasePtr->localClockTime                     = (StbM_LocalClockTimeType){0u, 0u, 0u};
            timeBasePtr->StbMClearTimeleapCount             = 0u;

/* SWS_StbM_00306 */
#if (STBM_TIME_RECORDING_SUPPORT == STD_ON)
            timeBasePtr->RecordBlockIndex = 0u;
            timeBasePtr->RecordBlockCount = 0u;
#if (STBM_MAX_SYNC_TIMERECORDTABLE_BLOCK_COUNT > 0)
            timeBasePtr->StbMSyncTimeRecordBlock = &StbM_SyncTimeRecordBlock[index][0];
#else
            timeBasePtr->StbMSyncTimeRecordBlock = NULL_PTR;
#endif
#if (STBM_MAX_OFFSET_TIMERECORDTABLE_BLOCK_COUNT > 0)
            timeBasePtr->StbMOffsetTimeRecordBlock = &StbM_OffsetTimeRecordBlock[index][0];
#else
            timeBasePtr->StbMOffsetTimeRecordBlock = NULL_PTR;
#endif
#endif

            const StbM_SynchronizedTimeBaseCfgType* timeBaseCfgPtr = &TIMEBASE_CFG(index);

#if STBM_GPT_TIMESTAMP_SUPPORT == STD_ON
            const StbM_LocalTimeClockCfgType* localTimeClock = timeBaseCfgPtr->StbMLocalTimeClock;
            if (localTimeClock->StbMLocalTimeHardwareType == GPT_CHANNEL_CONFIGURATION)
            {
                Gpt_StartTimer(localTimeClock->StbMLocalTimeHardware, localTimeClock->LocalTimeMax);
            }
#endif

#if (STBM_TIME_CORRECTION_SUPPORT == STD_ON)
            timeBasePtr->RateDeviation.IsSet                      = FALSE;
            timeBasePtr->RateDeviation.RrcValue                   = STBM_RATE_PPM;
            timeBasePtr->RateDeviation.RocValue                   = 0;
            const StbMTimeCorrectionCfgType* timeCorrectionCfgPtr = timeBaseCfgPtr->StbMTimeCorrection;
            if (NULL_PTR != timeCorrectionCfgPtr)
            {
                timeBasePtr->RateCorrection.Array      = &StbM_RateCorrection[index][0u];
                timeBasePtr->RateCorrection.StartIndex = 0UL;
                timeBasePtr->RateCorrection.StopIndex  = 0UL;
                for (subIndex = 0; subIndex < timeCorrectionCfgPtr->RateCorrectionsPerMeasurementDuration; subIndex++)
                {
                    timeBasePtr->RateCorrection.Array[subIndex].TGstart = 0ULL;
                    timeBasePtr->RateCorrection.Array[subIndex].TVstart = 0ULL;
                }
            }
#endif
        }
#if (STBM_NOTIFICATION_CUSTOMER_NUM > 0u)
        for (uint16 index = 0u; index < STBM_NOTIFICATION_CUSTOMER_NUM; index++)
        {
            StbM_NotificationType* notificationPtr = &StbM_Notification[index];
            notificationPtr->GPTtimercalled        = FALSE;
            notificationPtr->isrunning             = FALSE;
        }
#endif
#if (STBM_TRIGGERED_CUSTOMER_NUM > 0u)
        for (uint16 index = 0u; index < STBM_TRIGGERED_CUSTOMER_NUM; index++)
        {
            StbM_TriggerRemainPeriod[index] = StbM_GetTriggerCustomerConfig(index)->triggeredCustomerPeriod;
        }
#endif
        StbM_InitStatus = TRUE;
    }
}

#if (STBM_VERSION_INFO_API == STD_ON)
/**
 * Returns the version information of this module.
 * Service ID: 0x05
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): @versioninfo, Pointer to the memory location holding the version information of
the module.
 * Return value: NA
 */
void StbM_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, STBM_APPL_DATA) versioninfo)
{
#if (STBM_DEV_ERROR_DETECT == STD_ON)
    if (versioninfo == NULL_PTR)
    {
        (void)Det_ReportError(STBM_MODULE_ID, STBM_INSTANCE_ID, STBM_SID_GETVERSIONINFO, STBM_E_PARAM_POINTER);
    }
    else
#endif
    {
        versioninfo->moduleID         = STBM_MODULE_ID;
        versioninfo->vendorID         = STBM_VENDOR_ID;
        versioninfo->sw_major_version = STBM_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = STBM_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = STBM_SW_PATCH_VERSION;
    }
}
#endif

/**
 * Returns a time value(Local Time Base derived from Global Time Base) in standard format.
 * Service ID: 0x07
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): @timeBaseId, time base reference
 * Parameters(INOUT): NA
 * Parameters(OUT): @timeStamp, Current time stamp that is valid at this time
 *                  @userData, User data of the Time Base
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType StbM_GetCurrentTime(
    StbM_SynchronizedTimeBaseType timeBaseId,
    StbM_TimeStampType*           timeStamp,
    StbM_UserDataType*            userData)
{
    Std_ReturnType ret = E_OK;
#if (STBM_DEV_ERROR_DETECT == STD_ON)
    ret = Stbm_TimeBaseIdDetCheck(timeBaseId, STBM_SID_GETCURRENTTIME, RESERVED_NOT_ALLOWED);
    if (ret == (Std_ReturnType)E_NOT_OK)
    {
        /* nothing */
    }
    else if ((timeStamp == NULL_PTR) || (userData == NULL_PTR))
    {
        (void)Det_ReportError(STBM_MODULE_ID, STBM_INSTANCE_ID, STBM_SID_GETCURRENTTIME, STBM_E_PARAM_POINTER);
        ret = E_NOT_OK;
    }
    else
#endif
    {
        uint16 timebasecfgid = StbM_FindSynchronizedTimeBase(timeBaseId);
        uint64 offsettime64  = 0u;
        uint64 time64        = 0u;

        if ((timeBaseId >= STBM_SYNC_TIMEBASE_RANGE) && (timeBaseId < STBM_OFFSET_TIMEBASE_RANGE))
        {
            if (TIMEBASE_CFG(timebasecfgid).StbMOffsetTimeBase != NULL_PTR)
            {
                SchM_Enter_StbM_Context();
                offsettime64 = StbM_GTTimeTo64Time(StbM_TimeBase[timebasecfgid].Offset);
                SchM_Exit_StbM_Context();
                timebasecfgid = *(TIMEBASE_CFG(timebasecfgid).StbMOffsetTimeBase);
            }
            else
            {
                ret = E_NOT_OK;
            }
        }

        /* SWS_StbM_00173 */
        /* SWS_StbM_00434 */
        if (ret == (Std_ReturnType)E_OK)
        {
            ret = StbM_GetCurrentLocalTime(timebasecfgid, &time64);
        }

        if (ret == (Std_ReturnType)E_OK)
        {
            const StbM_TimeBaseType* timeBasePtr = &StbM_TimeBase[timebasecfgid];
            StbM_GetGlobalTime64(timebasecfgid, &time64);
            time64 += offsettime64;
            SchM_Enter_StbM_Context();
            *timeStamp                = StbM_64TimeToGTTime(time64);
            *userData                 = timeBasePtr->UserData;
            timeStamp->timeBaseStatus = timeBasePtr->timeBaseStatus;
            SchM_Exit_StbM_Context();
        }
    }
    return ret;
}

/**
 * Returns the Virtual Local Time of the referenced Time Base.
 * Service ID: 0x1e
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): @timeBaseId, time base reference
 * Parameters(INOUT): NA
 * Parameters(OUT): @localTimePtr, Current Virtual Local Time value
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType
    StbM_GetCurrentVirtualLocalTime(StbM_SynchronizedTimeBaseType timeBaseId, StbM_VirtualLocalTimeType* localTimePtr)
{
    uint64         time64 = 0u;
    Std_ReturnType ret    = E_OK;

#if (STBM_DEV_ERROR_DETECT == STD_ON)
    ret = Stbm_TimeBaseIdDetCheck(timeBaseId, STBM_SID_GETCURRENTVIRTUALLOCALTIME, RESERVED_NOT_ALLOWED);
    if (ret == (Std_ReturnType)E_NOT_OK)
    {
        /* nothing */
    }
    else if (localTimePtr == NULL_PTR)
    {
        (void)Det_ReportError(
            STBM_MODULE_ID,
            STBM_INSTANCE_ID,
            STBM_SID_GETCURRENTVIRTUALLOCALTIME,
            STBM_E_PARAM_POINTER);
        ret = E_NOT_OK;
    }
    else
#endif
    {
        uint16 timebasecfgid = StbM_FindSynchronizedTimeBase(timeBaseId);
        if ((timeBaseId >= STBM_SYNC_TIMEBASE_RANGE) && (timeBaseId < STBM_OFFSET_TIMEBASE_RANGE))
        {
            if (TIMEBASE_CFG(timebasecfgid).StbMOffsetTimeBase != NULL_PTR)
            {
                timebasecfgid = *(TIMEBASE_CFG(timebasecfgid).StbMOffsetTimeBase);
            }
            else
            {
                ret = E_NOT_OK;
            }
        }

        /* SWS_StbM_00437 */
        if (ret == (Std_ReturnType)E_OK)
        {
            ret = StbM_GetCurrentLocalTime(timebasecfgid, &time64);
        }
        if (ret == (Std_ReturnType)E_OK)
        {
            localTimePtr->nanosecondsHi = (uint32)(time64 >> 32);
            localTimePtr->nanosecondsLo = (uint32)time64;
        }
    }
    return ret;
}

/**
 * Allows the Customers to set the new global time that has to be valid for the system,
   which will be sent to the busses. This function will be used if a Time Master is
   present in this ECU.
 * Service ID: 0x0b
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN):  @timeBaseId, time base reference
 *                  @timeStamp, New time stamp
 *                  @userData, New user data(if not NULL)
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType StbM_SetGlobalTime(
    StbM_SynchronizedTimeBaseType timeBaseId,
    const StbM_TimeStampType*     timeStamp,
    const StbM_UserDataType*      userData)
{
    Std_ReturnType ret    = E_NOT_OK;
    uint64         time64 = 0u;
#if (STBM_DEV_ERROR_DETECT == STD_ON)
    ret = Stbm_TimeBaseIdDetCheck(timeBaseId, STBM_SID_SETGLOBALTIME, RESERVED_NOT_ALLOWED);
    if (ret == (Std_ReturnType)E_NOT_OK)
    {
        /* nothing */
    }
    else if ((timeStamp == NULL_PTR) || (userData == NULL_PTR))
    {
        (void)Det_ReportError(STBM_MODULE_ID, STBM_INSTANCE_ID, STBM_SID_SETGLOBALTIME, STBM_E_PARAM_POINTER);
        ret = E_NOT_OK;
    }
    else if (timeStamp->nanoseconds >= STBM_NANOSECOND_TO_SECOND)
    {
        (void)Det_ReportError(STBM_MODULE_ID, STBM_INSTANCE_ID, STBM_SID_SETGLOBALTIME, STBM_E_PARAM_TIMESTAMP);
        ret = E_NOT_OK;
    }
    else if (userData->userDataLength > 3u)
    {
        (void)Det_ReportError(STBM_MODULE_ID, STBM_INSTANCE_ID, STBM_SID_SETGLOBALTIME, STBM_E_PARAM_USERDATA);
        ret = E_NOT_OK;
    }
    else
#endif
    {
        uint16             timebasecfgid = StbM_FindSynchronizedTimeBase(timeBaseId);
        StbM_TimeBaseType* timeBasePtr   = &StbM_TimeBase[timebasecfgid];
        if ((timeBaseId < STBM_OFFSET_TIMEBASE_RANGE) && (timeBaseId >= STBM_SYNC_TIMEBASE_RANGE)
            && (TIMEBASE_CFG(timebasecfgid).StbMOffsetTimeBase != NULL_PTR))
        {
            uint16 synctimebasecfgid = *(TIMEBASE_CFG(timebasecfgid).StbMOffsetTimeBase);
            if ((StbM_TimeBase[synctimebasecfgid].timeBaseStatus & STBM_TIMEBASE_STATUS_GLOBAL_TIME_BASE) != 0u)
            {
                ret = StbM_GetCurrentLocalTime(synctimebasecfgid, &time64);
                if (ret == E_OK)
                {
                    uint64 receivedtime64 = StbM_GTTimeTo64Time(*timeStamp);
                    if (receivedtime64 >= time64)
                    {
                        time64 = receivedtime64 - time64;
                        SchM_Enter_StbM_Context();
                        timeBasePtr->Offset = StbM_64TimeToGTTime(time64);
                        SchM_Exit_StbM_Context();
                    }
                    else
                    {
                        ret = E_NOT_OK;
                    }
                }
            }
        }
        else
        {
            ret       = StbM_GetCurrentLocalTime(timebasecfgid, &time64);
            uint64 TL = StbM_GTTimeTo64Time(*timeStamp);
            /* SWS_StbM_00342 */
            /* SWS_StbM_00436 */
            /* SWS_StbM_00433 */
            StbM_UpdateMainTimeTuple(timebasecfgid, TL, time64);
        }
        if (ret == E_OK)
        {
            StbM_TimeBaseStatusType newTimeBaseStatus = 0x00u | STBM_TIMEBASE_STATUS_GLOBAL_TIME_BASE;
            SchM_Enter_StbM_Context();
#if STBM_STATUS_NOTIFICATIONS_ENABLED == STD_ON
            StbM_TimeBaseStatusType oldTimeBaseStatus = timeBasePtr->timeBaseStatus;
            StbM_OnChangeTimeBaseStatus(&timeBasePtr->notificationEvents, oldTimeBaseStatus, newTimeBaseStatus);
            timeBasePtr->notificationEvents |= STBM_STATUS_NTF_RESYNC;
#endif
            timeBasePtr->timeBaseStatus = newTimeBaseStatus;
            StbM_SetUserDataIn(timebasecfgid, *userData);
            ++timeBasePtr->StbMTimeBaseUpdateCounter; /* Free-Running Counter */
            SchM_Exit_StbM_Context();
        }
    }
    return ret;
}

/**
 * Allows the Customers to set the Global Time that will be sent to the buses. This
   function will be used if a Time Master is present in this ECU. Using UpdateGlobal
   Time will not lead to an immediate transmission of the Global Time.
 * Service ID: 0x10
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN):  @timeBaseId, time base reference
 *                  @timeStamp, New time stamp
 *                  @userData, New user data(if not NULL)
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType StbM_UpdateGlobalTime(
    StbM_SynchronizedTimeBaseType timeBaseId,
    const StbM_TimeStampType*     timeStamp,
    const StbM_UserDataType*      userData)
{
    Std_ReturnType ret    = E_NOT_OK;
    uint64         time64 = 0u;
#if (STBM_DEV_ERROR_DETECT == STD_ON)
    ret = Stbm_TimeBaseIdDetCheck(timeBaseId, STBM_SID_SETGLOBALTIME, RESERVED_NOT_ALLOWED);
    if (ret == (Std_ReturnType)E_NOT_OK)
    {
        /* nothing */
    }
    else if ((timeStamp == NULL_PTR) || (userData == NULL_PTR))
    {
        (void)Det_ReportError(STBM_MODULE_ID, STBM_INSTANCE_ID, STBM_SID_SETGLOBALTIME, STBM_E_PARAM_POINTER);
        ret = E_NOT_OK;
    }
    else if (timeStamp->nanoseconds >= STBM_NANOSECOND_TO_SECOND)
    {
        (void)Det_ReportError(STBM_MODULE_ID, STBM_INSTANCE_ID, STBM_SID_SETGLOBALTIME, STBM_E_PARAM_TIMESTAMP);
        ret = E_NOT_OK;
    }
    else if (userData->userDataLength > 3u)
    {
        (void)Det_ReportError(STBM_MODULE_ID, STBM_INSTANCE_ID, STBM_SID_SETGLOBALTIME, STBM_E_PARAM_USERDATA);
        ret = E_NOT_OK;
    }
    else
#endif
    {
        uint16             timebasecfgid = StbM_FindSynchronizedTimeBase(timeBaseId);
        StbM_TimeBaseType* timeBasePtr   = &StbM_TimeBase[timebasecfgid];
        if ((timeBaseId < STBM_OFFSET_TIMEBASE_RANGE) && (timeBaseId >= STBM_SYNC_TIMEBASE_RANGE)
            && (TIMEBASE_CFG(timebasecfgid).StbMOffsetTimeBase != NULL_PTR))
        {
            uint16 synctimebasecfgid = *(TIMEBASE_CFG(timebasecfgid).StbMOffsetTimeBase);
            if ((StbM_TimeBase[synctimebasecfgid].timeBaseStatus & STBM_TIMEBASE_STATUS_GLOBAL_TIME_BASE) != 0u)
            {
                ret = StbM_GetCurrentLocalTime(synctimebasecfgid, &time64);
                if (ret == E_OK)
                {
                    uint64 receivedtime64 = StbM_GTTimeTo64Time(*timeStamp);
                    if (receivedtime64 >= time64)
                    {
                        time64 = receivedtime64 - time64;
                        SchM_Enter_StbM_Context();
                        timeBasePtr->Offset = StbM_64TimeToGTTime(time64);
                        SchM_Exit_StbM_Context();
                    }
                    else
                    {
                        ret = E_NOT_OK;
                    }
                }
            }
        }
        else
        {
            ret       = StbM_GetCurrentLocalTime(timebasecfgid, &time64);
            uint64 TL = StbM_GTTimeTo64Time(*timeStamp);
            /* SWS_StbM_00342 */
            /* SWS_StbM_00436 */
            /* SWS_StbM_00433 */
            StbM_UpdateMainTimeTuple(timebasecfgid, TL, time64);
        }
        if (ret == E_OK)
        {
            StbM_TimeBaseStatusType newTimeBaseStatus = 0x00u | STBM_TIMEBASE_STATUS_GLOBAL_TIME_BASE;
            SchM_Enter_StbM_Context();
#if STBM_STATUS_NOTIFICATIONS_ENABLED == STD_ON
            StbM_TimeBaseStatusType oldTimeBaseStatus = timeBasePtr->timeBaseStatus;
            StbM_OnChangeTimeBaseStatus(&timeBasePtr->notificationEvents, oldTimeBaseStatus, newTimeBaseStatus);
            timeBasePtr->notificationEvents |= STBM_STATUS_NTF_RESYNC;
#endif
            timeBasePtr->timeBaseStatus = newTimeBaseStatus;
            StbM_SetUserDataIn(timebasecfgid, *userData);
            SchM_Exit_StbM_Context();
        }
    }
    return ret;
}

/**
 * Allows the Customers to set the new User Data that has to be valid for the
   system, which will be sent to the busses.
 * Service ID: 0x0c
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN):  @timeBaseId, time base reference
 *                  @userData, New user data
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType StbM_SetUserData(StbM_SynchronizedTimeBaseType timeBaseId, const StbM_UserDataType* userData)
{
    Std_ReturnType ret = E_OK;
#if (STBM_DEV_ERROR_DETECT == STD_ON)
    ret = Stbm_TimeBaseIdDetCheck(timeBaseId, STBM_SID_SETUSERDATA, RESERVED_NOT_ALLOWED);
    if (ret == (Std_ReturnType)E_NOT_OK)
    {
        /* nothing */
    }
    else if (userData == NULL_PTR)
    {
        (void)Det_ReportError(STBM_MODULE_ID, STBM_INSTANCE_ID, STBM_SID_SETUSERDATA, STBM_E_PARAM_POINTER);
        ret = E_NOT_OK;
    }
    else if (userData->userDataLength > 3u)
    {
        (void)Det_ReportError(STBM_MODULE_ID, STBM_INSTANCE_ID, STBM_SID_SETUSERDATA, STBM_E_PARAM_USERDATA);
        ret = E_NOT_OK;
    }
    else
#endif
    {
        uint16 timebasecfgid = StbM_FindSynchronizedTimeBase(timeBaseId);
        SchM_Enter_StbM_Context();
        StbM_SetUserDataIn(timebasecfgid, *userData);
        SchM_Exit_StbM_Context();
    }
    return ret;
}

/**
 * Allows the Customers and the Timesync Modules to set the Offset Time and the User Data.
 * Service ID: 0x0d
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN):  @timeBaseId, time base reference
 *                  @timeStamp, New time stamp
 *                  @userData, New user data(Or NULL if no new user data is provided)
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType StbM_SetOffset(
    StbM_SynchronizedTimeBaseType timeBaseId,
    const StbM_TimeStampType*     timeStamp,
    const StbM_UserDataType*      userData)
{
    Std_ReturnType ret;
#if (STBM_DEV_ERROR_DETECT == STD_ON)
    ret = Stbm_TimeBaseIdDetCheck(timeBaseId, STBM_SID_SETOFFSET, SYNC_AND_PURELOCAL_TIMEBASE_NOT_ALLOWED);
    if (ret == (Std_ReturnType)E_NOT_OK)
    {
        /* nothing */
    }
    else if ((timeStamp == NULL_PTR) || (userData == NULL_PTR))
    {
        (void)Det_ReportError(STBM_MODULE_ID, STBM_INSTANCE_ID, STBM_SID_SETOFFSET, STBM_E_PARAM_POINTER);
        ret = E_NOT_OK;
    }
    else if (timeStamp->nanoseconds >= STBM_NANOSECOND_TO_SECOND)
    {
        (void)Det_ReportError(STBM_MODULE_ID, STBM_INSTANCE_ID, STBM_SID_SETOFFSET, STBM_E_PARAM_TIMESTAMP);
        ret = E_NOT_OK;
    }
    else if (userData->userDataLength > 3u)
    {
        (void)Det_ReportError(STBM_MODULE_ID, STBM_INSTANCE_ID, STBM_SID_SETOFFSET, STBM_E_PARAM_USERDATA);
        ret = E_NOT_OK;
    }
    else
#endif
    {
        uint16             timebasecfgid = StbM_FindSynchronizedTimeBase(timeBaseId);
        StbM_TimeBaseType* timeBasePtr   = &StbM_TimeBase[timebasecfgid];
        SchM_Enter_StbM_Context();
        timeBasePtr->Offset                       = *timeStamp;
        StbM_TimeBaseStatusType newTimeBaseStatus = 0x00u | STBM_TIMEBASE_STATUS_GLOBAL_TIME_BASE;
#if STBM_STATUS_NOTIFICATIONS_ENABLED == STD_ON
        StbM_TimeBaseStatusType oldTimeBaseStatus = timeBasePtr->timeBaseStatus;
        StbM_OnChangeTimeBaseStatus(&timeBasePtr->notificationEvents, oldTimeBaseStatus, newTimeBaseStatus);
        timeBasePtr->notificationEvents |= STBM_STATUS_NTF_RESYNC;
#endif
        timeBasePtr->timeBaseStatus = newTimeBaseStatus;

        StbM_SetUserDataIn(timebasecfgid, *userData);
        ++timeBasePtr->StbMTimeBaseUpdateCounter; /* Free-Running Counter */
        SchM_Exit_StbM_Context();

        uint64 TV = 0u;
        ret       = StbM_GetCurrentLocalTime(timebasecfgid, &TV);
        if (ret == (Std_ReturnType)E_OK)
        {
            uint64 TL = StbM_GTTimeTo64Time(*timeStamp);
            /* SWS_StbM_00436 */
            /* SWS_StbM_00433 */
            StbM_UpdateMainTimeTuple(timebasecfgid, TL, TV);
        }
    }
    return ret;
}

/**
 * Allows the Timesync Modules to get the current Offset Time and User Data.
 * Service ID: 0x0e
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): @timeBaseId, time base reference
 * Parameters(INOUT): NA
 * Parameters(OUT): @timeStamp, Current Offset Time value
 *                  @userData, Current user data
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType
    StbM_GetOffset(StbM_SynchronizedTimeBaseType timeBaseId, StbM_TimeStampType* timeStamp, StbM_UserDataType* userData)
{
    Std_ReturnType ret = E_OK;
#if (STBM_DEV_ERROR_DETECT == STD_ON)
    ret = Stbm_TimeBaseIdDetCheck(timeBaseId, STBM_SID_GETOFFSET, SYNC_AND_PURELOCAL_TIMEBASE_NOT_ALLOWED);
    if (ret == (Std_ReturnType)E_NOT_OK)
    {
        /* nothing */
    }
    else if ((timeStamp == NULL_PTR) || (userData == NULL_PTR))
    {
        (void)Det_ReportError(STBM_MODULE_ID, STBM_INSTANCE_ID, STBM_SID_GETOFFSET, STBM_E_PARAM_POINTER);
        ret = E_NOT_OK;
    }
    else
#endif
    {
        uint16                   timebasecfgid = StbM_FindSynchronizedTimeBase(timeBaseId);
        const StbM_TimeBaseType* timeBasePtr   = &StbM_TimeBase[timebasecfgid];

        SchM_Enter_StbM_Context();
        timeStamp->nanoseconds    = timeBasePtr->Offset.nanoseconds;
        timeStamp->seconds        = timeBasePtr->Offset.seconds;
        timeStamp->secondsHi      = timeBasePtr->Offset.secondsHi;
        timeStamp->timeBaseStatus = timeBasePtr->timeBaseStatus;
        *userData                 = timeBasePtr->UserData;
        SchM_Exit_StbM_Context();

        uint64 time64VL = 0u;
        (void)StbM_GetCurrentLocalTime(timebasecfgid, &time64VL);
    }
    return ret;
}
/**
 * Returns the current Time Tuple, status and User Data of the Time Base.
 * Service ID: 0x1f
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): @timeBaseId, time base reference
 *
 * Parameters(INOUT): NA
 * Parameters(OUT): @globalTimePtr Value of the local instance of the Global Time, which is sampled
                    when the function is called
                    @localTimePtr, Value of the Virtual Local Time, which is sampled when the function is called
                    @userDataPtr, User data of the Time Base
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType StbM_BusGetCurrentTime(
    StbM_SynchronizedTimeBaseType timeBaseId,
    StbM_TimeStampType*           globalTimePtr,
    StbM_VirtualLocalTimeType*    localTimePtr,
    StbM_UserDataType*            userDataPtr)
{
    Std_ReturnType ret;
#if (STBM_DEV_ERROR_DETECT == STD_ON)
    ret = Stbm_TimeBaseIdDetCheck(timeBaseId, STBM_SID_BUSGETCURRENTTIME, OFFSET_TIMEBASE_NOT_ALLOWED);
    if (ret == (Std_ReturnType)E_NOT_OK)
    {
        /* nothing */
    }
    else if ((globalTimePtr == NULL_PTR) || (localTimePtr == NULL_PTR) || (userDataPtr == NULL_PTR))
    {
        (void)Det_ReportError(STBM_MODULE_ID, STBM_INSTANCE_ID, STBM_SID_BUSGETCURRENTTIME, STBM_E_PARAM_POINTER);
        ret = E_NOT_OK;
    }
    else
#endif
    {
        uint16 timebasecfgid = StbM_FindSynchronizedTimeBase(timeBaseId);
        uint64 localTime     = 0u;

        /* SWS_StbM_00435 */
        ret = StbM_GetCurrentLocalTime(timebasecfgid, &localTime);
        if (ret == (Std_ReturnType)E_OK)
        {
            localTimePtr->nanosecondsHi = (uint32)(localTime >> 32);
            localTimePtr->nanosecondsLo = (uint32)localTime;
            (void)StbM_GetGlobalTime64(timebasecfgid, &localTime);
            *globalTimePtr = StbM_64TimeToGTTime(localTime);

            const StbM_TimeBaseType* timeBasePtr = &StbM_TimeBase[timebasecfgid];
            SchM_Enter_StbM_Context();
            *userDataPtr                  = timeBasePtr->UserData;
            globalTimePtr->timeBaseStatus = timeBasePtr->timeBaseStatus;
            SchM_Exit_StbM_Context();
        }
    }
    return ret;
}

/**
 * Allows the Time Base Provider Modules to forward a new Global Time tuple(i.e.,
   the Received Time Tuple) to the StbM.
 * Service ID: 0x0f
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): @timeBaseId, time base reference
 *                 @globalTimePtr New Global Time value
 *                 @userDataPtr New User Data(if not NULL)
                   @measureDataPtr New measurement data
                   @localTimePtr Value of the Virtual Local Time associated to the new Global Time
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType StbM_BusSetGlobalTime(
    StbM_SynchronizedTimeBaseType    timeBaseId,
    const StbM_TimeStampType*        globalTimePtr,
    const StbM_UserDataType*         userDataPtr,
    const StbM_MeasurementType*      measureDataPtr,
    const StbM_VirtualLocalTimeType* localTimePtr)
{
    Std_ReturnType ret;

#if (STBM_DEV_ERROR_DETECT == STD_ON)
    ret = Stbm_TimeBaseIdDetCheck(timeBaseId, STBM_SID_BUSSETGLOBALTIME, PURELOCAL_TIMEBASE_NOT_ALLOWED);
    if (ret == (Std_ReturnType)E_NOT_OK)
    {
        /* nothing */
    }
    else if ((globalTimePtr == NULL_PTR) || (measureDataPtr == NULL_PTR) || (localTimePtr == NULL_PTR))
    {
        (void)Det_ReportError(STBM_MODULE_ID, STBM_INSTANCE_ID, STBM_SID_BUSSETGLOBALTIME, STBM_E_PARAM_POINTER);
        ret = E_NOT_OK;
    }
    else if (globalTimePtr->nanoseconds >= STBM_NANOSECOND_TO_SECOND)
    {
        (void)Det_ReportError(STBM_MODULE_ID, STBM_INSTANCE_ID, STBM_SID_BUSSETGLOBALTIME, STBM_E_PARAM_TIMESTAMP);
        ret = E_NOT_OK;
    }
    else if ((userDataPtr != NULL_PTR) && (userDataPtr->userDataLength > 3u))
    {
        (void)Det_ReportError(STBM_MODULE_ID, STBM_INSTANCE_ID, STBM_SID_BUSSETGLOBALTIME, STBM_E_PARAM_USERDATA);
        ret = E_NOT_OK;
    }
    else
#endif
    {
        /* SWS_StbM_00179 */
        uint64             time64 = 0u;
        uint64             tempTime64;
        uint16             timebasecfgid = StbM_FindSynchronizedTimeBase(timeBaseId);
        StbM_TimeBaseType* timeBasePtr   = &StbM_TimeBase[timebasecfgid];
        uint64             time64received;
#if STBM_STATUS_NOTIFICATIONS_ENABLED == STD_ON
        StbM_TimeBaseStatusType oldTimeBaseStatus = timeBasePtr->timeBaseStatus;
#endif
        ret        = StbM_GetCurrentLocalTime(timebasecfgid, &time64);
        tempTime64 = time64;

        if (ret == (Std_ReturnType)E_OK)
        {
            SchM_Enter_StbM_Context();
            timeBasePtr->timeBaseStatus |= STBM_TIMEBASE_STATUS_GLOBAL_TIME_BASE;
            timeBasePtr->lastBusSettime = time64;
            SchM_Exit_StbM_Context();
            time64received = StbM_GTTimeTo64Time(*globalTimePtr);
            StbM_GetGlobalTime64(timebasecfgid, &tempTime64);
            StbM_CheckTimeleap(timebasecfgid, tempTime64, time64received);

            if (timeBasePtr->BusNotFirstSet == (boolean)FALSE)
            {
                timeBasePtr->BusNotFirstSet = (boolean)TRUE;
            }
#if (STBM_TIME_RECORDING_SUPPORT == STD_ON)
            StbM_TimeRecord(timeBaseId, time64received, localTimePtr->nanosecondsLo, measureDataPtr->pathDelay);
#else
            STBM_UNUSED(measureDataPtr);
            STBM_UNUSED(localTimePtr);
#endif
#if (STBM_TIME_CORRECTION_SUPPORT == STD_ON)
            StbM_TimeCorrection(timebasecfgid, &time64received, &time64, globalTimePtr->timeBaseStatus);
#endif
            /* SWS_StbM_00184 */
            SchM_Enter_StbM_Context();
            if (((globalTimePtr->timeBaseStatus) & STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY) != 0u)
            {
                timeBasePtr->timeBaseStatus |= STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY;
            }
            else
            {
                timeBasePtr->timeBaseStatus &= ~STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY;
            }
            SchM_Exit_StbM_Context();
            /* SWS_StbM_00436 */
            /* SWS_StbM_00433 */
            StbM_UpdateMainTimeTuple(timebasecfgid, time64received, time64);
        }
        SchM_Enter_StbM_Context();
        timeBasePtr->timeBaseStatus &= ~STBM_TIMEBASE_STATUS_TIMEOUT;
#if STBM_STATUS_NOTIFICATIONS_ENABLED == STD_ON
        StbM_OnChangeTimeBaseStatus(&timeBasePtr->notificationEvents, oldTimeBaseStatus, timeBasePtr->timeBaseStatus);
        timeBasePtr->notificationEvents |= STBM_STATUS_NTF_RESYNC;
#endif
        /* SWS_StbM_00350 */
        ++timeBasePtr->StbMTimeBaseUpdateCounter; /* Free-Running Counter */

        if (userDataPtr != NULL_PTR)
        {
            StbM_SetUserDataIn(timebasecfgid, *userDataPtr);
        }
        SchM_Exit_StbM_Context();
    }
    return ret;
}

/**
 * Returns value of the current rate deviation of a Time Base
 * Service ID: 0x11
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): @timeBaseId, time base reference
 * Parameters(INOUT): NA
 * Parameters(OUT): @rateDeviation, Value of the current rate deviation of a Time Base
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
/* PRQA S 3673 ++ */ /* VL_StbM_3673 */
Std_ReturnType StbM_GetRateDeviation(StbM_SynchronizedTimeBaseType timeBaseId, StbM_RateDeviationType* rateDeviation)
/* PRQA S 3673 -- */ /* VL_StbM_3673 */
{
    Std_ReturnType ret = E_NOT_OK;
#if (STBM_DEV_ERROR_DETECT == STD_ON)
    if (Stbm_TimeBaseIdDetCheck(timeBaseId, STBM_SID_GETRATEDEVIATION, RESERVED_NOT_ALLOWED) == E_NOT_OK)
    {
        /* nothing */
    }
    else if (rateDeviation == NULL_PTR)
    {
        (void)Det_ReportError(STBM_MODULE_ID, STBM_INSTANCE_ID, STBM_SID_GETRATEDEVIATION, STBM_E_PARAM_POINTER);
    }
    else
#endif
    {
#if STBM_TIME_CORRECTION_SUPPORT == STD_ON
        StbM_TimeBaseIndexType timebasecfgid = StbM_FindSynchronizedTimeBase(timeBaseId);
        StbM_TimeBaseType*     timeBasePtr   = &StbM_TimeBase[timebasecfgid];
        SchM_Enter_StbM_Context();
        if (timeBasePtr->RateDeviation.IsSet)
        {
            *rateDeviation = (StbM_RateDeviationType)(timeBasePtr->RateDeviation.RrcValue - (sint32)STBM_RATE_PPM);
            ret            = E_OK;
        }
        SchM_Exit_StbM_Context();
#endif
    }

#if (STBM_DEV_ERROR_DETECT == STD_OFF) && (STBM_TIME_CORRECTION_SUPPORT == STD_OFF)
    STBM_UNUSED(timeBaseId);
    STBM_UNUSED(rateDeviation);
#endif

    return ret;
}

/**
 * Allows to set the rate of a Synchronized Time Base(being either a Pure Local Time Base or not)
 * Service ID: 0x12
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): @timeBaseId, time base reference
 *                 @rateDeviation, Value of the applied rate deviation
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType StbM_SetRateCorrection(StbM_SynchronizedTimeBaseType timeBaseId, StbM_RateDeviationType rateDeviation)
{
    Std_ReturnType ret;
#if (STBM_DEV_ERROR_DETECT == STD_ON)
    ret = Stbm_TimeBaseIdDetCheck(timeBaseId, STBM_SID_SETRATECORRECTION, RESERVED_NOT_ALLOWED);
    if (ret == (Std_ReturnType)E_OK)
#endif
    {
#if (STBM_TIME_CORRECTION_SUPPORT == STD_ON)
        uint16                                  timebasecfgid        = StbM_FindSynchronizedTimeBase(timeBaseId);
        const StbM_SynchronizedTimeBaseCfgType* synchronizedTimeBase = &TIMEBASE_CFG(timebasecfgid);
        if ((synchronizedTimeBase->StbMTimeCorrection != NULL_PTR)
            && synchronizedTimeBase->StbMTimeCorrection->AllowMasterRateCorrection)
        {
            StbM_RateDeviationType rate;
            StbM_RateDeviationType RateDeviationMax = /* Range: 0 .. 32000  */
                (StbM_RateDeviationType)synchronizedTimeBase->StbMTimeCorrection->MasterRateDeviationMax;
            if (rateDeviation >= RateDeviationMax)
            {
                rate = RateDeviationMax;
            }
            else if (rateDeviation <= -RateDeviationMax)
            {
                rate = -RateDeviationMax;
            }
            else
            {
                rate = rateDeviation;
            }
            StbM_CurrentRateType* rateDeviationPtr = &StbM_TimeBase[timebasecfgid].RateDeviation;

            SchM_Enter_StbM_Context();
            rateDeviationPtr->RrcValue = (sint32)rate + (sint32)STBM_RATE_PPM;
            rateDeviationPtr->RocValue = 0;
            rateDeviationPtr->IsSet    = TRUE;
            SchM_Exit_StbM_Context();

            uint64 TV = 0u;
            ret       = StbM_GetCurrentLocalTime(timebasecfgid, &TV);
            if (ret == (Std_ReturnType)E_OK)
            {
                uint64 TL = TV;
                (void)StbM_GetGlobalTime64(timebasecfgid, &TL);
                /* SWS_StbM_00436 */
                /* SWS_StbM_00433 */
                StbM_UpdateMainTimeTuple(timebasecfgid, TL, TV);
            }
        }
        else
#else
        STBM_UNUSED(timeBaseId);
        STBM_UNUSED(rateDeviation);
#endif
        {
#if (STBM_DEV_ERROR_DETECT == STD_ON)
            (void)
                Det_ReportError(STBM_MODULE_ID, STBM_INSTANCE_ID, STBM_SID_SETRATECORRECTION, STBM_E_SERVICE_DISABLED);
#endif
            ret = E_NOT_OK;
        }
    }
    return ret;
}

/**
 * Returns value of Time Leap.
 * Service ID: 0x13
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): @timeBaseId, time base reference
 * Parameters(INOUT): NA
 * Parameters(OUT): @timeJump, Time leap value
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType StbM_GetTimeLeap(StbM_SynchronizedTimeBaseType timeBaseId, StbM_TimeDiffType* timeJump)
{
    Std_ReturnType ret;
#if (STBM_DEV_ERROR_DETECT == STD_ON)
    ret = Stbm_TimeBaseIdDetCheck(timeBaseId, STBM_SID_GETTIMELEAP, PURELOCAL_TIMEBASE_NOT_ALLOWED);
    if (ret == (Std_ReturnType)E_NOT_OK)
    {
        /* nothing */
    }
    else if (timeJump == NULL_PTR)
    {
        (void)Det_ReportError(STBM_MODULE_ID, STBM_INSTANCE_ID, STBM_SID_GETTIMELEAP, STBM_E_PARAM_POINTER);
        ret = E_NOT_OK;
    }
    else
#endif
    {
        uint16 timebasecfgid = StbM_FindSynchronizedTimeBase(timeBaseId);
        SchM_Enter_StbM_Context();
        if (StbM_TimeBase[timebasecfgid].BusNotFirstSet)
        {
            *timeJump = StbM_TimeBase[timebasecfgid].TimeLeap;
            ret       = E_OK;
        }
        else
        {
            ret = E_NOT_OK;
        }
        SchM_Exit_StbM_Context();
    }
    return ret;
}

/**
 * Returns detailed status information for a Synchronized(or Pure Local) Time Base
   and, if called for an Offset Time Base, for the Offset Time Base and the underlying
   Synchronized Time Base.
 * Service ID: 0x14
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): @timeBaseId, time base reference
 * Parameters(INOUT): NA
 * Parameters(OUT): @syncTimeBaseStatus Status of the Synchronized(or Pure Local) Time Base
                    @offsetTimeBaseStatus Status of the Offset Time Base
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType StbM_GetTimeBaseStatus(
    StbM_SynchronizedTimeBaseType timeBaseId,
    StbM_TimeBaseStatusType*      syncTimeBaseStatus,
    StbM_TimeBaseStatusType*      offsetTimeBaseStatus)
{
    Std_ReturnType ret;
#if (STBM_DEV_ERROR_DETECT == STD_ON)
    ret = Stbm_TimeBaseIdDetCheck(timeBaseId, STBM_SID_GETTIMEBASESTATUS, RESERVED_NOT_ALLOWED);
    if (ret == (Std_ReturnType)E_NOT_OK)
    {
        /* nothing */
    }
    else if ((syncTimeBaseStatus == NULL_PTR) || (offsetTimeBaseStatus == NULL_PTR))
    {
        (void)Det_ReportError(STBM_MODULE_ID, STBM_INSTANCE_ID, STBM_SID_GETTIMEBASESTATUS, STBM_E_PARAM_POINTER);
        ret = E_NOT_OK;
    }
    else
#endif
    {
        uint16 timebasecfgid = StbM_FindSynchronizedTimeBase(timeBaseId);
        uint64 localTime     = 0u;
        ret                  = StbM_GetCurrentLocalTime(timebasecfgid, &localTime);

        SchM_Enter_StbM_Context();
        if (timeBaseId < STBM_SYNC_TIMEBASE_RANGE)
        {
            *syncTimeBaseStatus   = StbM_TimeBase[timebasecfgid].timeBaseStatus;
            *offsetTimeBaseStatus = 0x00u;
        }
        else if (timeBaseId < STBM_OFFSET_TIMEBASE_RANGE)
        {
            *offsetTimeBaseStatus = StbM_TimeBase[timebasecfgid].timeBaseStatus;
            if (TIMEBASE_CFG(timebasecfgid).StbMOffsetTimeBase != NULL_PTR)
            {
                timebasecfgid       = *(TIMEBASE_CFG(timebasecfgid).StbMOffsetTimeBase);
                *syncTimeBaseStatus = StbM_TimeBase[timebasecfgid].timeBaseStatus;
            }
        }
        else
        {
            *syncTimeBaseStatus   = StbM_TimeBase[timebasecfgid].timeBaseStatus;
            *offsetTimeBaseStatus = 0x00u;
        }
        SchM_Exit_StbM_Context();
    }
    return ret;
}
#if (STBM_NOTIFICATION_CUSTOMER_NUM > 0)
/**
 * Sets a time value, which the Time Base value is compared against
 * Service ID: 0x15
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): @timeBaseId, time base reference
 *                 @customerId, Status of the Synchronized Time Base
                   @expireTime, Time value relative to current Time Base value of the Notification
                    Customer, when the Timer shall expire
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType StbM_StartTimer(
    StbM_SynchronizedTimeBaseType timeBaseId,
    StbM_CustomerIdType           customerId,
    const StbM_TimeStampType*     expireTime)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STBM_DEV_ERROR_DETECT == STD_ON)
    if (Stbm_TimeBaseIdDetCheck(timeBaseId, STBM_SID_STARTTIMER, RESERVED_NOT_ALLOWED) == E_NOT_OK)
    {
        /* nothing */
    }
    else if (expireTime == NULL_PTR)
    {
        (void)Det_ReportError(STBM_MODULE_ID, STBM_INSTANCE_ID, STBM_SID_STARTTIMER, STBM_E_PARAM_POINTER);
    }
    else if (
        (customerId >= STBM_NOTIFICATION_CUSTOMER_NUM)
        || (timeBaseId != StbMNotificationCustomer[customerId].timeBaseId))
    {
        (void)Det_ReportError(STBM_MODULE_ID, STBM_INSTANCE_ID, STBM_SID_STARTTIMER, STBM_E_PARAM);
    }
    else
#endif
    {
        StbM_NotificationType* notificationPtr = &StbM_Notification[customerId];
        SchM_Enter_StbM_Context();
        if (!notificationPtr->isrunning)
        {
            notificationPtr->CustomerTimerExpireTime = StbM_GTTimeTo64Time(*expireTime);
            notificationPtr->isrunning               = TRUE;
            ret                                      = E_OK;
        }
        SchM_Exit_StbM_Context();
    }
    return ret;
}
#endif
#if (STBM_TIME_RECORDING_SUPPORT == STD_ON)
/**
 * Accesses to the recorded snapshot data Header of the table belonging to the Synchronized Time Base.
 * Service ID: 0x16
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): @timeBaseId, time base reference
 * Parameters(INOUT): NA
 * Parameters(OUT): @syncRecordTableHead, Header of the table
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType StbM_GetSyncTimeRecordHead(
    StbM_SynchronizedTimeBaseType timeBaseId,
    P2VAR(StbM_SyncRecordTableHeadType, AUTOMATIC, STBM_APPL_DATA) syncRecordTableHead)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STBM_DEV_ERROR_DETECT == STD_ON)
    ret =
        Stbm_TimeBaseIdDetCheck(timeBaseId, STBM_SID_GETSYNCTIMERECORDHEAD, OFFSET_AND_PURELOCAL_TIMEBASE_NOT_ALLOWED);
    if (ret == (Std_ReturnType)E_NOT_OK)
    {
        /* nothing */
    }
    else if (syncRecordTableHead == NULL_PTR)
    {
        (void)Det_ReportError(STBM_MODULE_ID, STBM_INSTANCE_ID, STBM_SID_GETSYNCTIMERECORDHEAD, STBM_E_PARAM_POINTER);
        ret = E_NOT_OK;
    }
    else
#endif
    {
        syncRecordTableHead->SynchronizedTimeDomain = timeBaseId;
        const StbM_LocalTimeClockCfgType* localTimeClockCfgPtr =
            TIMEBASE_CFG(StbM_FindSynchronizedTimeBase(timeBaseId)).StbMLocalTimeClock;
        syncRecordTableHead->HWfrequency = localTimeClockCfgPtr->StbMClockFrequency;
        syncRecordTableHead->HWprescaler = localTimeClockCfgPtr->StbMClockPrescaler;
    }
    return ret;
}

/**
 * Accesses to the recorded snapshot data Header of the table belonging to the Offset Time Base.
 * Service ID: 0x17
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): @timeBaseId, time base reference
 * Parameters(INOUT): NA
 * Parameters(OUT): @offsetRecordTableHead, Header of the table
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType StbM_GetOffsetTimeRecordHead(
    StbM_SynchronizedTimeBaseType timeBaseId,
    P2VAR(StbM_OffsetRecordTableHeadType, AUTOMATIC, STBM_APPL_DATA) offsetRecordTableHead)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STBM_DEV_ERROR_DETECT == STD_ON)
    ret =
        Stbm_TimeBaseIdDetCheck(timeBaseId, STBM_SID_GETOFFSETTIMERECORDHEAD, SYNC_AND_PURELOCAL_TIMEBASE_NOT_ALLOWED);
    if (ret == (Std_ReturnType)E_NOT_OK)
    {
        /* nothing */
    }
    else if (offsetRecordTableHead == NULL_PTR)
    {
        STBM_DET_REPORT(STBM_SID_GETOFFSETTIMERECORDHEAD, STBM_E_PARAM_POINTER);
        ret = E_NOT_OK;
    }
    else
#endif
    {
        offsetRecordTableHead->OffsetTimeDomain = timeBaseId;
    }
    return ret;
}
#endif
/**
 * Called by the <Upper Layer> to force the Timesync Modules to transmit the current
   Time Base again due to an incremented timeBaseUpdateCounter[timeBaseId]
 * Service ID: 0x1c
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): @timeBaseId, time base reference
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType StbM_TriggerTimeTransmission(StbM_SynchronizedTimeBaseType timeBaseId)
{
    Std_ReturnType ret = E_OK;
#if (STBM_DEV_ERROR_DETECT == STD_ON)
    ret = Stbm_TimeBaseIdDetCheck(timeBaseId, STBM_SID_TRIGGERTIMETRANSMISSION, PURELOCAL_TIMEBASE_NOT_ALLOWED);
    if (ret == (Std_ReturnType)E_OK)
#endif
    {
        uint16 timebasecfgid;
        timebasecfgid = StbM_FindSynchronizedTimeBase(timeBaseId);
        SchM_Enter_StbM_Context();
        ++StbM_TimeBase[timebasecfgid].StbMTimeBaseUpdateCounter; /* Free-Running Counter */
        SchM_Exit_StbM_Context();
    }
    return ret;
}

/**
 * Allows the Timesync Modules to detect, whether a Time Base should be
   transmitted immediately in the subsequent <Bus>TSyn_MainFunction() cycle.
 * Service ID: 0x1b
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): @timeBaseId, time base reference
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:    Counter value belonging to the Time Base, that indicates a Time Base
                    update to the Timesync Modules
 */
uint8 StbM_GetTimeBaseUpdateCounter(StbM_SynchronizedTimeBaseType timeBaseId)
{
    uint8 StbM_TimeBaseUpdateCounter;
#if (STBM_DEV_ERROR_DETECT == STD_ON)
    Std_ReturnType ret;
    ret = Stbm_TimeBaseIdDetCheck(timeBaseId, STBM_SID_GETTIMEBASEUPDATECOUNTER, PURELOCAL_TIMEBASE_NOT_ALLOWED);
    if ((Std_ReturnType)E_OK != ret)
    {
        StbM_TimeBaseUpdateCounter = 0u;
    }
    else
#endif
    {
        uint16 timebasecfgid;
        timebasecfgid = StbM_FindSynchronizedTimeBase(timeBaseId);
        SchM_Enter_StbM_Context();
        StbM_TimeBaseUpdateCounter = StbM_TimeBase[timebasecfgid].StbMTimeBaseUpdateCounter;
        SchM_Exit_StbM_Context();
    }
    return StbM_TimeBaseUpdateCounter;
}

#if (STBM_IS_SYSTEM_WIDE_GLOBAL_TIME_MASTER == STD_ON)
/**
 * Indicates if the functionality for a system wide master(e.g. StbM_SetGlobalTime)
   for a given Time Base is available or not.
 * Service ID: 0x1d
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): @timeBaseId, time base reference
 * Parameters(INOUT): NA
 * Parameters(OUT): @masterConfig, Indicates, if system wide master functionality is supported
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType StbM_GetMasterConfig(StbM_SynchronizedTimeBaseType timeBaseId, StbM_MasterConfigType* masterConfig)
{
    Std_ReturnType ret = E_OK;
#if (STBM_DEV_ERROR_DETECT == STD_ON)
    ret = Stbm_TimeBaseIdDetCheck(timeBaseId, STBM_SID_GETMASTERCONFIG, RESERVED_NOT_ALLOWED);
    if (ret == (Std_ReturnType)E_NOT_OK)
    {
        /* nothing */
    }
    else if (masterConfig == NULL_PTR)
    {
        (void)Det_ReportError(STBM_MODULE_ID, STBM_INSTANCE_ID, STBM_SID_GETMASTERCONFIG, STBM_E_PARAM_POINTER);
        ret = E_NOT_OK;
    }
    else
#endif
    {
        uint16 timebasecfgid = StbM_FindSynchronizedTimeBase(timeBaseId);
        /* SWS_StbM_00408 */
        *masterConfig = TIMEBASE_CFG(timebasecfgid).StbMIsSystemWideGlobalTimeMaster ? STBM_SYSTEM_WIDE_MASTER_ENABLED
                                                                                     : STBM_SYSTEM_WIDE_MASTER_DISABLED;
    }
    return ret;
}
#endif

/**
 * @brief         This function will be called cyclically by a task body provided by the BSW Schedule. It will invoke
 *                the triggered customers and synchronize the referenced OS ScheduleTables..
 * @id            0x04
 * @synchronous   Synchronous
 * @reentrancy    Non Reentrant
 */
void StbM_MainFunction(void)
{
    if (StbM_InitStatus == (boolean)TRUE)
    {
        uint16 index;
        uint64 TV = 0u;
        for (index = 0u; index < STBM_SYNCHRONIZED_TIME_BASE_NUM; index++)
        {
            (void)StbM_GetCurrentLocalTime(index, &TV);
            StbM_CheckTimeout(index, TV);
#if STBM_STATUS_NOTIFICATIONS_ENABLED == STD_ON
            StbM_CheckStatusNotification(index);
#endif
        }
#if (STBM_NOTIFICATION_CUSTOMER_NUM > 0u)
        for (index = 0u; index < STBM_NOTIFICATION_CUSTOMER_NUM; index++)
        {
            StbM_NotificationType* notificationPtr = &StbM_Notification[index];
            if (notificationPtr->isrunning)
            {
                uint64 TL;
                uint16 timeBaseCfgId = StbM_FindSynchronizedTimeBase(StbMNotificationCustomer[index].timeBaseId);
                (void)StbM_GetCurrentLocalTime(timeBaseCfgId, &TL);
                (void)StbM_GetGlobalTime64(timeBaseCfgId, &TL);
                sint64 timeDiff;
                timeDiff = (sint64)(notificationPtr->CustomerTimerExpireTime - TL);
                if (timeDiff <= 0)
                {
                    timeDiff = (timeDiff < StbM_TimeDiffType_LowerLimit) ? StbM_TimeDiffType_LowerLimit : timeDiff;
                    StbMTimeNtfCallbackType StbMTimeNtfCallback =
                        StbMNotificationCustomer[index].StbMTimeNotificationCallback;
                    if (NULL_PTR != StbMTimeNtfCallback)
                    {
                        StbMTimeNtfCallback((StbM_TimeDiffType)timeDiff);
                    }
                    SchM_Enter_StbM_Context();
                    notificationPtr->isrunning      = FALSE;
                    notificationPtr->GPTtimercalled = FALSE;
                    SchM_Exit_StbM_Context();
                }
                else if (timeDiff < (sint64)(STBM_TIMER_START_THRESHOLD))
                {
                    /* SWS_StbM_00336 */
                    uint32 tickDiff =
                        (uint32)((timeDiff * (sint64)(STBM_GPT_TIMER_FREQUENCY)) / (sint64)(STBM_NS_PER_S));
                    Gpt_StartTimer(STBM_GPT_TIMER_REF, tickDiff);
                    SchM_Enter_StbM_Context();
                    notificationPtr->isrunning      = FALSE;
                    notificationPtr->GPTtimercalled = TRUE;
                    SchM_Exit_StbM_Context();
                }
                else
                {
                    /* do nothing */
                }
            }
        }
#endif
#if (STBM_TRIGGERED_CUSTOMER_NUM > 0u)
        for (index = 0u; index < STBM_TRIGGERED_CUSTOMER_NUM; index++)
        {
            --StbM_TriggerRemainPeriod[index];
            if (StbM_TriggerRemainPeriod[index] == 0u)
            {
                const StbMTriggeredCustomerCfgType* triggerCustomerConfig = StbM_GetTriggerCustomerConfig(index);
                StbM_TriggerRemainPeriod[index] = triggerCustomerConfig->triggeredCustomerPeriod;

                ScheduleTableStatusType scheduleTableStatus;
                if (GetScheduleTableStatus(triggerCustomerConfig->StbMOSScheduleTableRef, &scheduleTableStatus) != E_OK)
                {
                    continue;
                }

                if ((scheduleTableStatus != SCHEDULETABLE_WAITING) && (scheduleTableStatus != SCHEDULETABLE_RUNNING)
                    && (scheduleTableStatus != SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS))
                {
                    continue;
                }

                /* SWS_StbM_00302 */
                /* SWS_StbM_00303 */
                uint64 TL;
                if (StbM_GetCurrentLocalTime(triggerCustomerConfig->indexOfSynchronizedTimeBase, &TL) != E_OK)
                {
                    continue;
                }
                StbM_GetGlobalTime64(triggerCustomerConfig->indexOfSynchronizedTimeBase, &TL);
                TL = TL / 1000u % triggerCustomerConfig->osScheduleTableDuration;
                (void)SyncScheduleTable(triggerCustomerConfig->StbMOSScheduleTableRef, (TickType)TL);
            }
        }
#endif

#if (STBM_TIME_RECORDING_SUPPORT == STD_ON)
        for (index = 0u; index < STBM_SYNCHRONIZED_TIME_BASE_NUM; index++)
        {
            if (TIMEBASE_CFG(index).StbMIsSystemWideGlobalTimeMaster == FALSE)
            {
                StbM_TimeRecordback(index);
            }
        }
#endif
    }
}

/**
 * The implementation of StbM_TimerCallback.
 */
#if (STBM_NOTIFICATION_CUSTOMER_NUM > 0)
void StbM_TimerCallback(void)
{
    uint8             index;
    uint64            time64;
    StbM_TimeDiffType timeDifference;
    for (index = 0u; index < STBM_NOTIFICATION_CUSTOMER_NUM; index++)
    {
        StbM_NotificationType*                 notificationPtr = &StbM_Notification[index];
        const StbMNotificationCustomerCfgType* customerPtr     = &StbMNotificationCustomer[index];
        if (notificationPtr->GPTtimercalled)
        {
            uint16 timeBaseCfgId = StbM_FindSynchronizedTimeBase(customerPtr->timeBaseId);
            (void)StbM_GetCurrentLocalTime(timeBaseCfgId, &time64);
            (void)StbM_GetGlobalTime64(timeBaseCfgId, &time64);
            if (notificationPtr->CustomerTimerExpireTime > time64)
            {
                timeDifference = (notificationPtr->CustomerTimerExpireTime - time64 > StbM_TimeDiffType_UpperLimit)
                                     ? StbM_TimeDiffType_UpperLimit
                                     : (notificationPtr->CustomerTimerExpireTime - time64);
            }
            else
            {
                timeDifference = (time64 - notificationPtr->CustomerTimerExpireTime > StbM_TimeDiffType_UpperLimit)
                                     ? StbM_TimeDiffType_LowerLimit
                                     : -(time64 - notificationPtr->CustomerTimerExpireTime);
            }
            StbMTimeNtfCallbackType StbMTimeNtfCallback = customerPtr->StbMTimeNotificationCallback;
            if (NULL_PTR != StbMTimeNtfCallback)
            {
                StbMTimeNtfCallback(timeDifference);
            }
            notificationPtr->GPTtimercalled = FALSE;
        }
    }
}
#endif
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#if (STBM_TIME_CORRECTION_SUPPORT == STD_ON)
STBM_LOCAL_INLINE uint64 StbM_Abs(sint64 value)
{
    if (value < 0LL)
    {
        return (uint64)-value;
    }
    else
    {
        return (uint64)value;
    }
}
#endif

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
#if (STBM_DEV_ERROR_DETECT == STD_ON)
/**
 * The implementation of CheckTimeBaseIdType.
 * @param[in] timeBaseId The time base identifier to check.
 * @param[in] type The type of time base identifier allowed.
 * @return Returns E_OK if the timeBaseId is of the allowed type, otherwise E_NOT_OK.
 */
static Std_ReturnType CheckTimeBaseIdType(StbM_SynchronizedTimeBaseType timeBaseId, StbM_TimeBaseIdAllowedType type)
{
    Std_ReturnType ret = E_OK;
    switch (type)
    {
    case RESERVED_NOT_ALLOWED:
        if (timeBaseId >= STBM_PURELOCAL_TIMEBASE_RANGE)
        {
            ret = E_NOT_OK;
        }
        break;
    case PURELOCAL_TIMEBASE_NOT_ALLOWED:
        if (timeBaseId >= STBM_OFFSET_TIMEBASE_RANGE)
        {
            ret = E_NOT_OK;
        }
        break;
    case OFFSET_TIMEBASE_NOT_ALLOWED:
        if ((timeBaseId >= STBM_SYNC_TIMEBASE_RANGE) && (timeBaseId < STBM_OFFSET_TIMEBASE_RANGE))
        {
            ret = E_NOT_OK;
        }
        break;
    case SYNC_TIMEBASE_NOT_ALLOWED:
        if (timeBaseId < STBM_SYNC_TIMEBASE_RANGE)
        {
            ret = E_NOT_OK;
        }
        break;
    case SYNC_AND_PURELOCAL_TIMEBASE_NOT_ALLOWED:
        if ((timeBaseId < STBM_SYNC_TIMEBASE_RANGE) || (timeBaseId >= STBM_OFFSET_TIMEBASE_RANGE))
        {
            ret = E_NOT_OK;
        }
        break;
    case OFFSET_AND_PURELOCAL_TIMEBASE_NOT_ALLOWED:
        if (timeBaseId >= STBM_SYNC_TIMEBASE_RANGE)
        {
            ret = E_NOT_OK;
        }
        break;
    default:
        /* do nothing */
        break;
    }
    return ret;
}

/**
 * The implementation of Stbm_TimeBaseIdDetCheck.
 * @param[in] timeBaseId The time base identifier to check.
 * @param[in] serviceid The service ID for error reporting.
 * @param[in] type The type of time base identifier allowed.
 * @return Returns E_OK if the timeBaseId is valid and of the allowed type, otherwise E_NOT_OK.
 */
static Std_ReturnType
    Stbm_TimeBaseIdDetCheck(StbM_SynchronizedTimeBaseType timeBaseId, uint8 serviceid, StbM_TimeBaseIdAllowedType type)
{
    Std_ReturnType ret = E_NOT_OK;
    if (StbM_InitStatus == (boolean)FALSE)
    {
#if (STBM_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError(STBM_MODULE_ID, STBM_INSTANCE_ID, serviceid, STBM_E_UNINIT);
#endif
    }
    else
    {
        StbM_TimeBaseIndexType timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
        if (timeBaseIndex < STBM_SYNCHRONIZED_TIME_BASE_NUM)
        {
            ret = CheckTimeBaseIdType(timeBaseId, type);
        }
        if (ret == E_NOT_OK)
        {
            (void)Det_ReportError(STBM_MODULE_ID, STBM_INSTANCE_ID, serviceid, STBM_E_PARAM);
        }
    }
    return ret;
}
#endif

/**
 * The implementation of StbM_FindSynchronizedTimeBase.
 * @param[in] timeBaseId The time base identifier to find.
 * @return Returns the index of the time base in the StbM_TimeBase array, or STBM_SYNCHRONIZED_TIME_BASE_NUM if not
 * found.
 */
STBM_LOCAL StbM_TimeBaseIndexType StbM_FindSynchronizedTimeBase(StbM_SynchronizedTimeBaseType timeBaseId)
{
    StbM_SynchronizedTimeBaseType           timeBaseIndex;
    const StbM_SynchronizedTimeBaseCfgType* timeBaseCfgPtr = StbM_ConfigData->StbMSynchronizedTimeBase;
    for (timeBaseIndex = 0; timeBaseIndex < STBM_SYNCHRONIZED_TIME_BASE_NUM; ++timeBaseIndex)
    {
        if (timeBaseId == timeBaseCfgPtr[timeBaseIndex].StbMSynchronizedTimeBaseIdentifier)
        {
            break;
        }
    }
    return timeBaseIndex;
}

/**
 * The implementation of StbM_LocalClockToLocalTime.
 * @param[in] localTimeClock The local time clock configuration.
 * @param[out] localClockTime The local clock time.
 * @param[out] timeStamp The time stamp.
 * @return Returns E_OK if successful, otherwise E_NOT_OK.
 */
STBM_LOCAL Std_ReturnType StbM_LocalClockToLocalTime(
    const StbM_LocalTimeClockCfgType* localTimeClock,
    StbM_LocalClockTimeType*          localClockTime,
    uint64*                           timeStamp)
{
    Std_ReturnType     ret = E_NOT_OK;
    StbM_HwCounterType curHwCounter;
    /* SWS_StbM_00180 */
    SchM_Enter_StbM_Context();
    switch (localTimeClock->StbMLocalTimeHardwareType)
    {
#if STBM_ETHIF_TIMESTAMP_SUPPORT == STD_ON
    /* SWS_StbM_00178 */
    case ETHTSYN_GLOBALTIME_DOMAIN:
    {
        uint8                 ctrlIdx = localTimeClock->StbMLocalTimeHardware;
        Eth_TimeStampQualType timeQual;
        Eth_TimeStampType     timeStamp;
        if ((EthIf_GetCurrentTime(ctrlIdx, &timeQual, &timeStamp) == E_OK) && (timeQual == ETH_VALID))
        {
            curHwCounter =
                (((uint64)timeStamp.secondsHi << 32) + timeStamp.seconds) * STBM_NS_PER_S + timeStamp.nanoseconds;
            ret = E_OK;
        }
        break;
    }
#endif
#if STBM_GPT_TIMESTAMP_SUPPORT == STD_ON
    /* SWS_StbM_00512 */
    case GPT_CHANNEL_CONFIGURATION:
    {
        Gpt_ChannelType channel = (Gpt_ChannelType)localTimeClock->StbMLocalTimeHardware;
        Gpt_ValueType   value   = Gpt_GetTimeElapsed(channel);
        curHwCounter            = (StbM_HwCounterType)value;
        ret                     = E_OK;
        break;
    }
#endif
#if STBM_OS_TIMESTAMP_SUPPORT == STD_ON
    case OS_COUNTER:
    {
        CounterType counterID = (CounterType)localTimeClock->StbMLocalTimeHardware;
        TickType    value;
        if (GetCounterValue(counterID, &value) == E_OK)
        {
            curHwCounter = (StbM_HwCounterType)value;
            ret          = E_OK;
        }
        break;
    }
#endif
    default:
        /* unreachable */
        break;
    }
    if (ret == E_OK)
    {
        StbM_HwCounterType preHwCounter     = localClockTime->preHwCounter;
        StbM_HwCounterType elapsedHwCounter = (preHwCounter <= curHwCounter)
                                                  ? (curHwCounter - preHwCounter)
                                                  : (localTimeClock->LocalTimeMax - preHwCounter + curHwCounter + 1u);
        /* SWS_StbM_00352 */
        localClockTime->preHwCounter = curHwCounter;

        StbM_LocalTimeType localTimeFraction = localClockTime->localTimeFraction;
        localTimeFraction += (elapsedHwCounter * STBM_NS_PER_S * localTimeClock->StbMClockPrescaler)
                             % localTimeClock->StbMClockFrequency;

        StbM_LocalTimeType localTime = localClockTime->localTime;
        if (localTimeFraction >= localTimeClock->StbMClockFrequency)
        {
            localTimeFraction -= localTimeClock->StbMClockFrequency;
            ++localTime;
        }
        localClockTime->localTimeFraction = localTimeFraction;

        localTime +=
            elapsedHwCounter * STBM_NS_PER_S * localTimeClock->StbMClockPrescaler / localTimeClock->StbMClockFrequency;

        localClockTime->localTime = localTime;
        *timeStamp                = localTime;
    }
    SchM_Exit_StbM_Context();
    return ret;
}

/**
 * The implementation of StbM_GetCurrentLocalTime.
 * @param[in] timeBaseIndex The index of the time base.
 * @param[out] localTime The local time to be filled.
 * @return Returns E_OK if successful, otherwise E_NOT_OK.
 */
STBM_LOCAL Std_ReturnType StbM_GetCurrentLocalTime(StbM_TimeBaseIndexType timeBaseIndex, StbM_LocalTimeType* localTime)
{
    Std_ReturnType                    stdReturn      = E_NOT_OK;
    const StbM_LocalTimeClockCfgType* localTimeClock = TIMEBASE_CFG(timeBaseIndex).StbMLocalTimeClock;
    if (localTimeClock != NULL_PTR)
    {
        StbM_LocalClockTimeType* localClockTime = &StbM_TimeBase[timeBaseIndex].localClockTime;
        stdReturn                               = StbM_LocalClockToLocalTime(localTimeClock, localClockTime, localTime);
    }
    return stdReturn;
}

/**
 * The implementation of StbM_GetGlobalTime64.
 * @param[in] CfgTimeBaseid The configuration ID of the time base.
 * @param[out] timeStamp The time stamp to be filled with the global time.
 */
STBM_LOCAL void StbM_GetGlobalTime64(uint16 timeBaseIndex, uint64* timeStamp)
{
    uint64 TV = *timeStamp;

    StbM_TimeBaseType* timeBasePtr = &StbM_TimeBase[timeBaseIndex];
    SchM_Enter_StbM_Context();
    uint64 diff = TV - timeBasePtr->StbMMainTimeTuple.VirtualLocalTime;

    if (TIMEBASE_CFG(timeBaseIndex).StbMTimeCorrection == NULL_PTR)
    {
        *timeStamp = timeBasePtr->StbMMainTimeTuple.GlobalTime + diff;
    }
    else
    {
        /* SWS_StbM_00353 */
        uint64  interval = TIMEBASE_CFG(timeBaseIndex).StbMTimeCorrection->OffsetCorrectionAdaptionInterval;
        float64 rc =
            (float64)(((float64)timeBasePtr->RateDeviation.RrcValue + (float64)timeBasePtr->RateDeviation.RocValue)
                      / (float64)STBM_RATE_PPM);

        /* when the Offset Correction Adaption Interval expires */
        /* PRQA S 4395 ++ */ /* VL_StbM_4395 */
        if ((timeBasePtr->RateDeviation.RocValue != 0L) && (diff >= interval))
        {
            timeBasePtr->StbMMainTimeTuple.VirtualLocalTime += interval;
            timeBasePtr->StbMMainTimeTuple.GlobalTime += (uint64)((float64)interval * rc);
            timeBasePtr->RateDeviation.RocValue = 0;
            diff                                = diff - interval;
            *timeStamp =
                timeBasePtr->StbMMainTimeTuple.GlobalTime
                + (uint64)(((float64)timeBasePtr->RateDeviation.RrcValue / (float64)STBM_RATE_PPM) * (float64)diff);
        }
        else
        {
            *timeStamp = timeBasePtr->StbMMainTimeTuple.GlobalTime + (uint64)(rc * (float64)diff);
        }
        /* PRQA S 4395 -- */ /* VL_StbM_4395 */
    }

    SchM_Exit_StbM_Context();
}

/**
 * The implementation of StbM_UpdateMainTimeTuple.
 * @param[in] timebasecfgid The configuration ID of the time base.
 * @param[in] TL The global time to be set.
 */
static void StbM_UpdateMainTimeTuple(uint16 timebasecfgid, uint64 TL, uint64 TV)
{
    StbM_MainTimeTupleType* mainTimeTuplePtr = &StbM_TimeBase[timebasecfgid].StbMMainTimeTuple;
    SchM_Enter_StbM_Context();
    mainTimeTuplePtr->VirtualLocalTime = TV;
    mainTimeTuplePtr->GlobalTime       = TL;
    SchM_Exit_StbM_Context();
}

/**
 * The implementation of StbM_GTTimeTo64Time.
 * @param[in] timeStamp The time stamp to be converted.
 * @return Returns the time stamp in 64-bit format.
 */
static uint64 StbM_GTTimeTo64Time(StbM_TimeStampType timeStamp)
{
    uint64 time64 = (((uint64)timeStamp.secondsHi) << 32) + (uint64)timeStamp.seconds;
    time64 *= STBM_NANOSECOND_TO_SECOND;
    time64 += timeStamp.nanoseconds;
    return time64;
}

/**
 * The implementation of StbM_64TimeToGTTime.
 * @param[in] time64 The 64-bit time to be converted.
 * @return Returns the time stamp in StbM_TimeStampType format.
 */
static StbM_TimeStampType StbM_64TimeToGTTime(uint64 time64)
{
    StbM_TimeStampType timeStamp = {0};
    timeStamp.nanoseconds        = (uint32)(time64 % STBM_NANOSECOND_TO_SECOND);
    uint64 t                     = time64 / STBM_NANOSECOND_TO_SECOND;
    timeStamp.secondsHi          = (uint16)(t >> 32);
    timeStamp.seconds            = (uint32)(t);
    return timeStamp;
}

/**
 * The implementation of StbM_CheckTimeleap.
 * @param[in] timebasecfgid The configuration ID of the time base.
 * @param[in] timenow The current time in 64-bit format.
 * @param[in] timereceived The received time in 64-bit format.
 */
static void StbM_CheckTimeleap(uint16 timebasecfgid, uint64 timenow, uint64 timereceived)
{
    StbM_TimeBaseType* timeBasePtr = &StbM_TimeBase[timebasecfgid];
    SchM_Enter_StbM_Context();
    if (timeBasePtr->BusNotFirstSet == (boolean)TRUE)
    {
        uint8 timeLeapState =
            (timenow > timereceived) ? STBM_TIMEBASE_STATUS_TIMELEAP_PAST : STBM_TIMEBASE_STATUS_TIMELEAP_FUTURE;
        uint64 TimeLeapPastThreshold = (timenow > timereceived)
                                           ? (TIMEBASE_CFG(timebasecfgid).StbMTimeLeapPastThreshold)
                                           : (TIMEBASE_CFG(timebasecfgid).StbMTimeLeapFutureThreshold);
        uint64 time64offset          = (timenow > timereceived) ? (timenow - timereceived) : (timereceived - timenow);
        if (TimeLeapPastThreshold != 0u)
        {
            if (time64offset > TimeLeapPastThreshold)
            {
                timeBasePtr->timeBaseStatus |= timeLeapState;
                timeBasePtr->StbMClearTimeleapCount = TIMEBASE_CFG(timebasecfgid).StbMClearTimeleapCount;
            }
            else
            {
                if (timeBasePtr->StbMClearTimeleapCount < 1u)
                {
                    timeBasePtr->timeBaseStatus &= (~STBM_TIMEBASE_STATUS_TIMELEAP_PAST);
                    timeBasePtr->timeBaseStatus &= (~STBM_TIMEBASE_STATUS_TIMELEAP_FUTURE);
                }
                else
                {
                    timeBasePtr->StbMClearTimeleapCount--;
                }
            }
        }
        if (time64offset > (uint64)StbM_TimeDiffType_UpperLimit)
        {
            time64offset = (uint64)StbM_TimeDiffType_UpperLimit;
        }
        timeBasePtr->TimeLeap = (timeLeapState == STBM_TIMEBASE_STATUS_TIMELEAP_FUTURE)
                                    ? (StbM_TimeDiffType)time64offset
                                    : -(StbM_TimeDiffType)time64offset;
    }
    SchM_Exit_StbM_Context();
}

/**
 * The implementation of StbM_CheckTimeout.
 * @param[in] timebasecfgid The configuration ID of the time base.
 * @param[in] timenow The current time in 64-bit format.
 */
static void StbM_CheckTimeout(uint16 timebasecfgid, uint64 timenow)
{
    const StbM_SynchronizedTimeBaseCfgType* timeBaseCfgPtr = &TIMEBASE_CFG(timebasecfgid);
    if (timeBaseCfgPtr->StbMSyncLossTimeout != 0u)
    {
        StbM_TimeBaseType* timeBasePtr = &StbM_TimeBase[timebasecfgid];

        SchM_Enter_StbM_Context();
#if STBM_STATUS_NOTIFICATIONS_ENABLED == STD_ON
        StbM_TimeBaseStatusType oldTimeBaseStatus = timeBasePtr->timeBaseStatus;
#endif
        if (timeBasePtr->BusNotFirstSet == (boolean)TRUE)
        {
            if ((timenow - timeBasePtr->lastBusSettime) > timeBaseCfgPtr->StbMSyncLossTimeout)
            {
                timeBasePtr->timeBaseStatus |= STBM_TIMEBASE_STATUS_TIMEOUT;
                /* SWS_StbM_00187 */
                if (timeBaseCfgPtr->StbMSynchronizedTimeBaseIdentifier < STBM_OFFSET_TIMEBASE_RANGE)
                {
                    if (timeBaseCfgPtr->StbMisTimeGateway)
                    {
                        timeBasePtr->timeBaseStatus |= STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY;
                    }
                }
            }
            else
            {
                timeBasePtr->timeBaseStatus &= (~STBM_TIMEBASE_STATUS_TIMEOUT);
            }
        }
        SchM_Exit_StbM_Context();
#if STBM_STATUS_NOTIFICATIONS_ENABLED == STD_ON
        StbM_OnChangeTimeBaseStatus(&timeBasePtr->notificationEvents, oldTimeBaseStatus, timeBasePtr->timeBaseStatus);
#endif
    }
}

#if (STBM_TIME_CORRECTION_SUPPORT == STD_ON)
/**
 * @brief       Rate Correction measurement.
 * @param[in]   timeBaseIndex  The time base index in the configuration
 * @param[in]   TGPtr          Global Time part of the Updated Rx Time Tuple
 * @param[in]   TVPtr          Virtual Local Time part of the Updated Rx Time Tuple
 * @param[in]   timeBaseStatus The time base status
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-71844
 */
STBM_LOCAL void StbM_CalculateRrc(
    uint16                  timeBaseIndex,
    const uint64*           TGPtr,
    const uint64*           TVPtr,
    StbM_TimeBaseStatusType timeBaseStatus)
{
    const StbMTimeCorrectionCfgType* cfgPtr      = TIMEBASE_CFG(timeBaseIndex).StbMTimeCorrection;
    StbM_TimeBaseType*               timeBasePtr = &StbM_TimeBase[timeBaseIndex];

    const StbM_RateCorrectionType* const rataCorrection = &timeBasePtr->RateCorrection;

    if ((cfgPtr == NULL_PTR) || (cfgPtr->RateCorrectionMeasurementDuration == 0u))
    {
        return;
    }

    uint16 num = cfgPtr->RateCorrectionsPerMeasurementDuration;

    SchM_Enter_StbM_Context();
    if (((timeBasePtr->timeBaseStatus & (StbM_TimeBaseStatusType)STBM_TIMEBASE_STATUS_TIMELEAP_FUTURE) != 0U)
        || ((timeBasePtr->timeBaseStatus & (StbM_TimeBaseStatusType)STBM_TIMEBASE_STATUS_TIMELEAP_PAST) != 0U))
    {
        for (uint16 i = 0u; i <= num; i++)
        {
            rataCorrection->Array[i].TGstart = 0ULL;
            rataCorrection->Array[i].TVstart = 0ULL;
        }
        timeBasePtr->RateCorrection.StartIndex = 0UL;
        timeBasePtr->RateCorrection.StopIndex  = 0UL;
        SchM_Exit_StbM_Context();
        return;
    }

    uint64 TVstop = *TVPtr;
    uint64 TGstop = *TGPtr;
    /* SWS_StbM_00376 */
    if (((timeBaseStatus & (StbM_TimeBaseStatusType)STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY)
         != (timeBasePtr->timeBaseStatus & (StbM_TimeBaseStatusType)STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY))
        || ((timeBasePtr->timeBaseStatus & (StbM_TimeBaseStatusType)STBM_TIMEBASE_STATUS_TIMEOUT) != 0U))
    {
        rataCorrection->Array[0].TGstart = TGstop;
        rataCorrection->Array[0].TVstart = TVstop;
        for (uint16 i = 1u; i <= num; i++)
        {
            rataCorrection->Array[i].TGstart = 0ULL;
            rataCorrection->Array[i].TVstart = 0ULL;
        }
        timeBasePtr->RateCorrection.StartIndex = 0UL;
        timeBasePtr->RateCorrection.StopIndex  = 1UL;
        SchM_Exit_StbM_Context();
        return;
    }

    uint16 arraySize = num + 1u;

    if ((rataCorrection->Array[0].TGstart == 0ULL) && (rataCorrection->Array[0].TVstart == 0ULL))
    {
        rataCorrection->Array[0].TGstart       = TGstop;
        rataCorrection->Array[0].TVstart       = TVstop;
        timeBasePtr->RateCorrection.StartIndex = 0UL;
        timeBasePtr->RateCorrection.StopIndex  = 1UL;
    }
    else
    {
        uint64 perDuration = cfgPtr->RateCorrectionMeasurementDuration / num;
        uint32 stopIndex   = timeBasePtr->RateCorrection.StopIndex;

        if ((TVstop - rataCorrection->Array[(stopIndex - 1UL) % arraySize].TVstart) >= perDuration)
        {
            rataCorrection->Array[stopIndex % arraySize].TGstart = TGstop;
            rataCorrection->Array[stopIndex % arraySize].TVstart = TVstop;
            timeBasePtr->RateCorrection.StopIndex++;
        }
    }

    uint32 startIndex = timeBasePtr->RateCorrection.StartIndex;
    if ((timeBasePtr->RateCorrection.StopIndex - startIndex) > num)
    {
        uint64 TGdiff = TGstop - rataCorrection->Array[startIndex % arraySize].TGstart;
        uint64 TVdiff = TVstop - rataCorrection->Array[startIndex % arraySize].TVstart;
        /* PRQA S 4395 ++ */ /* VL_StbM_4395 */
        sint32 rrcValue = (sint32)(((float64)TGdiff / (float64)TVdiff * (float64)STBM_RATE_PPM) + STBM_ROUND_VALUE);
        /* PRQA S 4395 -- */ /* VL_StbM_4395 */
        timeBasePtr->RateCorrection.StartIndex++;
        timeBasePtr->RateDeviation.RrcValue = rrcValue;
        timeBasePtr->RateDeviation.IsSet    = TRUE;
    }
    SchM_Exit_StbM_Context();
}

/**
 * @brief         Calculate a new value for the Offset Correction by Rate Adaption.
 * @param[in]     timeBaseIndex  The time base index in the configuration
 * @param[in,out] TGPtr          Global Time part of the Updated Rx Time Tuple
 * @param[in]     TVPtr          Virtual Local Time part of the Updated Rx Time Tuple
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-71845
 */
STBM_LOCAL void StbM_CalculateRoc(uint16 timeBaseIndex, uint64* TGPtr, const uint64* TVPtr)
{
    const StbMTimeCorrectionCfgType* cfgPtr      = TIMEBASE_CFG(timeBaseIndex).StbMTimeCorrection;
    StbM_TimeBaseType*               timeBasePtr = &StbM_TimeBase[timeBaseIndex];

    if (cfgPtr == NULL_PTR)
    {
        return;
    }

    uint64 TGstop   = *TGPtr;
    uint64 interval = cfgPtr->OffsetCorrectionAdaptionInterval;
    uint64 TL       = *TVPtr;

    StbM_GetGlobalTime64(timeBaseIndex, &TL);
    sint64 Odiff = (sint64)TGstop - (sint64)TL;

    SchM_Enter_StbM_Context();
    if (cfgPtr->OffsetCorrectionJumpThreshold == 0u)
    {
        timeBasePtr->RateDeviation.RocValue = 0;
    }
    else if (StbM_Abs(Odiff) >= cfgPtr->OffsetCorrectionJumpThreshold)
    {
        timeBasePtr->RateDeviation.RocValue = 0;
    }
    else if (interval == 0u)
    {
        timeBasePtr->RateDeviation.RocValue = 0;
    }
    else if (StbM_Abs(Odiff) < cfgPtr->OffsetCorrectionJumpThreshold) /* SWS_StbM_00356 */
    {
        /* PRQA S 4395 ++ */ /* VL_StbM_4395 */
        float64 temp = (float64)STBM_RATE_PPM * (float64)Odiff / (float64)interval;
        if (temp > 0.0)
        {
            timeBasePtr->RateDeviation.RocValue = (sint32)(temp + STBM_ROUND_VALUE);
        }
        else
        {
            timeBasePtr->RateDeviation.RocValue = (sint32)(temp - STBM_ROUND_VALUE);
        }
        /* PRQA S 4395 -- */ /* VL_StbM_4395 */
        *TGPtr = TL;
    }
    else
    {
        /* do nothing */
    }
    SchM_Exit_StbM_Context();
}

/**
 * @brief         Do Rate Correction and Offset Correction.
 * @param[in]     timeBaseIndex  The time base index in the configuration
 * @param[in,out] TGPtr          Global Time part of the Updated Rx Time Tuple
 * @param[in]     TVPtr          Virtual Local Time part of the Updated Rx Time Tuple
 * @param[in]     timeBaseStatus The time base status
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-71846
 */
STBM_LOCAL void StbM_TimeCorrection(
    uint16                  timeBaseIndex,
    uint64*                 TGPtr,
    const uint64*           TVPtr,
    StbM_TimeBaseStatusType timeBaseStatus)
{
    StbM_CalculateRrc(timeBaseIndex, TGPtr, TVPtr, timeBaseStatus);
    StbM_CalculateRoc(timeBaseIndex, TGPtr, TVPtr);
}
#endif

#if STBM_STATUS_NOTIFICATIONS_ENABLED == STD_ON
/**
 * The implementation of StbM_OnChangeTimeBaseStatus.
 * @param[in,out] notificationEvents Pointer to the notification events to be updated.
 * @param[in] oldTimeBaseStatus The old time base status.
 * @param[in] newTimeBaseStatus The new time base status.
 */
STBM_LOCAL void StbM_OnChangeTimeBaseStatus(
    StbM_TimeBaseNotificationType* notificationEvents,
    StbM_TimeBaseStatusType        oldTimeBaseStatus,
    StbM_TimeBaseStatusType        newTimeBaseStatus)
{
    StbM_TimeBaseStatusType       chgTimeBaseStatus = oldTimeBaseStatus ^ newTimeBaseStatus;
    StbM_TimeBaseNotificationType eventNotification = 0u;
    if ((chgTimeBaseStatus & STBM_TIMEBASE_STATUS_GLOBAL_TIME_BASE) != 0u)
    {
        eventNotification |= STBM_STATUS_NTF_GLOBAL_TIME;
    }

    if ((chgTimeBaseStatus & STBM_TIMEBASE_STATUS_TIMEOUT) != 0u)
    {
        eventNotification |= ((newTimeBaseStatus & STBM_TIMEBASE_STATUS_TIMEOUT) != 0u)
                                 ? STBM_STATUS_NTF_TIMEOUT_OCCURRED
                                 : STBM_STATUS_NTF_TIMEOUT_REMOVED;
    }

    if ((chgTimeBaseStatus & STBM_TIMEBASE_STATUS_TIMELEAP_FUTURE) != 0u)
    {
        eventNotification |= ((newTimeBaseStatus & STBM_TIMEBASE_STATUS_TIMELEAP_FUTURE) != 0u)
                                 ? STBM_STATUS_NTF_TIMELEAP_FUTURE
                                 : STBM_STATUS_NTF_TIMELEAP_FUTURE_REMOVED;
    }

    if ((chgTimeBaseStatus & STBM_TIMEBASE_STATUS_TIMELEAP_PAST) != 0u)
    {
        eventNotification |= ((newTimeBaseStatus & STBM_TIMEBASE_STATUS_TIMELEAP_PAST) != 0u)
                                 ? STBM_STATUS_NTF_TIMELEAP_PAST
                                 : STBM_STATUS_NTF_TIMELEAP_PAST_REMOVED;
    }

    if ((chgTimeBaseStatus & STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY) != 0u)
    {
        eventNotification |= ((newTimeBaseStatus & STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY) != 0u)
                                 ? STBM_STATUS_NTF_SYNC_TO_SUBDOMAIN
                                 : STBM_STATUS_NTF_SYNC_TO_GLOBAL_MASTER;
    }
    SchM_Enter_StbM_Context();
    *notificationEvents |= eventNotification;
    SchM_Exit_StbM_Context();
}

/**
 * The implementation of StbM_CheckStatusNotification.
 * @param[in] timeBaseIndex The index of the time base to check for notifications.
 */
STBM_LOCAL void StbM_CheckStatusNotification(StbM_TimeBaseIndexType timeBaseIndex)
{
    const StbM_SynchronizedTimeBaseCfgType* timeBaseConfig = &TIMEBASE_CFG(timeBaseIndex);

    StbM_TimeBaseNotificationType* notificationEvents = &StbM_TimeBase[timeBaseIndex].notificationEvents;
    StbM_TimeBaseNotificationType  eventNotification  = *notificationEvents & timeBaseConfig->statusNotificationMask;
    if (eventNotification != 0u)
    {
        if ((timeBaseConfig->StbMNotificationInterface == SR_INTERFACE)
            || (timeBaseConfig->StbMNotificationInterface == CALLBACK_AND_SR_INTERFACE))
        {
            /* notify the APP via StatusNotification Interface */
        }
        if ((timeBaseConfig->StbMNotificationInterface == CALLBACK)
            || (timeBaseConfig->StbMNotificationInterface == CALLBACK_AND_SR_INTERFACE))
        {
            timeBaseConfig->StbMStatusNotificationCallback(eventNotification);
        }
    }
    *notificationEvents = 0u;
}
#endif

/**
 * The implementation of StbM_TimeRecord.
 * @param[in] timeBaseId The time base identifier for which the time record is made.
 * @param[in] TLreceived The global time received.
 * @param[in] TVLoreceived The virtual local time received.
 * @param[in] pathDelay The path delay for the time record.
 */
#if (STBM_TIME_RECORDING_SUPPORT == STD_ON)
static void
    StbM_TimeRecord(StbM_SynchronizedTimeBaseType timeBaseId, uint64 TLreceived, uint32 TVLoreceived, uint32 pathDelay)
{
    uint16                                  timebasecfgid = StbM_FindSynchronizedTimeBase(timeBaseId);
    uint64                                  time64;
    const StbM_SynchronizedTimeBaseCfgType* timeBaseCfgPtr = &TIMEBASE_CFG(timebasecfgid);
    if (!timeBaseCfgPtr->StbMIsSystemWideGlobalTimeMaster)
    {
        StbM_TimeBaseType* timeBasePtr = &StbM_TimeBase[timebasecfgid];
        StbM_GetCurrentLocalTime(timebasecfgid, &time64);
        StbM_GetGlobalTime64(timebasecfgid, &time64);

        SchM_Enter_StbM_Context();
        uint16 recordBlockIndex = timeBasePtr->RecordBlockIndex;
        if ((timeBaseId < STBM_SYNC_TIMEBASE_RANGE)
            && (timeBaseCfgPtr->StbMTimeRecording->StbMSyncTimeRecordTableBlockCount > 0))
        {
            StbM_SyncRecordTableBlockType* syncRecordTableBlock =
                &timeBasePtr->StbMSyncTimeRecordBlock[recordBlockIndex];
            syncRecordTableBlock->GlbNanoSeconds = TLreceived % STBM_NANOSECOND_TO_SECOND;
            syncRecordTableBlock->GlbSeconds     = TLreceived / STBM_NANOSECOND_TO_SECOND;
            syncRecordTableBlock->LocNanoSeconds = time64 % STBM_NANOSECOND_TO_SECOND;
            syncRecordTableBlock->LocSeconds     = time64 / STBM_NANOSECOND_TO_SECOND;
            syncRecordTableBlock->PathDelay      = pathDelay;
#if (STBM_TIME_CORRECTION_SUPPORT == STD_ON)
            syncRecordTableBlock->RateDeviation = (timeBasePtr->RateDeviation.ratio - 1.0) * 1000000.0;
#else
            syncRecordTableBlock->RateDeviation = 0;
#endif
            syncRecordTableBlock->timeBaseStatus      = timeBasePtr->timeBaseStatus;
            syncRecordTableBlock->VirtualLocalTimeLow = TVLoreceived;
            timeBasePtr->RecordBlockIndex =
                (recordBlockIndex + 1u) % timeBaseCfgPtr->StbMTimeRecording->StbMSyncTimeRecordTableBlockCount;
            if (timeBasePtr->RecordBlockCount < timeBaseCfgPtr->StbMTimeRecording->StbMSyncTimeRecordTableBlockCount)
            {
                timeBasePtr->RecordBlockCount++;
            }
        }
        else if (timeBaseCfgPtr->StbMTimeRecording->StbMOffsetTimeRecordTableBlockCount > 0)
        {
            StbM_OffsetRecordTableBlockType* offsetRecordTableBlock =
                &timeBasePtr->StbMOffsetTimeRecordBlock[recordBlockIndex];
            offsetRecordTableBlock->GlbNanoSeconds = TLreceived % STBM_NANOSECOND_TO_SECOND;
            offsetRecordTableBlock->GlbSeconds     = TLreceived / STBM_NANOSECOND_TO_SECOND;
            offsetRecordTableBlock->timeBaseStatus = timeBasePtr->timeBaseStatus;
            timeBasePtr->RecordBlockIndex =
                (recordBlockIndex + 1u) % timeBaseCfgPtr->StbMTimeRecording->StbMOffsetTimeRecordTableBlockCount;

            if (timeBasePtr->RecordBlockCount < timeBaseCfgPtr->StbMTimeRecording->StbMOffsetTimeRecordTableBlockCount)
            {
                timeBasePtr->RecordBlockCount++;
            }
        }
        SchM_Exit_StbM_Context();
    }
}

/* Called when a measurement is prepared, currently not available */
/**
 * The implementation of StbM_TimeRecordback.
 * @param[in] timebasecfgid The configuration ID of the time base for which the time record is processed.
 */
static void StbM_TimeRecordback(StbM_SynchronizedTimeBaseType timebasecfgid)
{
    uint16                          timeBaseId = TIMEBASE_CFG(timebasecfgid).StbMSynchronizedTimeBaseIdentifier;
    const StbMTimeRecordingCfgType* timeRecordingCfgPtr = TIMEBASE_CFG(timebasecfgid).StbMTimeRecording;
    StbM_TimeBaseType*              timeBasePtr         = &StbM_TimeBase[timebasecfgid];
    uint16                          curIndex;
    SchM_Enter_StbM_Context();
    if (timeBaseId < STBM_SYNC_TIMEBASE_RANGE)
    {
        curIndex = (timeBasePtr->RecordBlockIndex >= timeBasePtr->RecordBlockCount)
                       ? (timeBasePtr->RecordBlockIndex - timeBasePtr->RecordBlockCount)
                       : (timeRecordingCfgPtr->StbMSyncTimeRecordTableBlockCount - timeBasePtr->RecordBlockCount
                          + timeBasePtr->RecordBlockIndex);
        while (timeBasePtr->RecordBlockCount > 0)
        {
            timeRecordingCfgPtr->StbMSyncTimeRecordBlockCallback(&timeBasePtr->StbMSyncTimeRecordBlock[curIndex]);
            curIndex = (curIndex == timeRecordingCfgPtr->StbMSyncTimeRecordTableBlockCount - 1) ? 0 : (curIndex + 1);
            timeBasePtr->RecordBlockCount--;
        }
    }
    else if (timeBaseId < STBM_OFFSET_TIMEBASE_RANGE)
    {
        curIndex = (timeBasePtr->RecordBlockIndex >= timeBasePtr->RecordBlockCount)
                       ? (timeBasePtr->RecordBlockIndex - timeBasePtr->RecordBlockCount)
                       : (timeRecordingCfgPtr->StbMOffsetTimeRecordTableBlockCount - timeBasePtr->RecordBlockCount
                          + timeBasePtr->RecordBlockIndex);
        while (timeBasePtr->RecordBlockCount > 0)
        {
            timeRecordingCfgPtr->StbMOffsetTimeRecordBlockCallback(&timeBasePtr->StbMOffsetTimeRecordBlock[curIndex]);
            curIndex = (curIndex == timeRecordingCfgPtr->StbMOffsetTimeRecordTableBlockCount - 1) ? 0 : (curIndex + 1);
            timeBasePtr->RecordBlockCount--;
        }
    }
    else
    {
        /* do nothing */
    }
    SchM_Exit_StbM_Context();
}
#endif

/**
 * The implementation of StbM_SetUserDataIn.
 * @param[in] timebasecfgid The configuration ID of the time base.
 * @param[in] userData The user data to be set in the time base.
 */
static void StbM_SetUserDataIn(uint16 timebasecfgid, StbM_UserDataType userData)
{
    StbM_TimeBaseType* timeBasePtr       = &StbM_TimeBase[timebasecfgid];
    timeBasePtr->UserData.userDataLength = userData.userDataLength;
    if (userData.userDataLength > 0u)
    {
        timeBasePtr->UserData.userByte0 = userData.userByte0;
        if (userData.userDataLength > 1u)
        {
            timeBasePtr->UserData.userByte1 = userData.userByte1;
            if (userData.userDataLength > 2u)
            {
                timeBasePtr->UserData.userByte2 = userData.userByte2;
            }
        }
    }
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
