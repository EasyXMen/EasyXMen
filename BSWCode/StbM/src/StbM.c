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
 **  @file               : StbM.c
 **  @author             : xiongfei.shi
 **  @date               : 2024/12/26
 **  @vendor             : isoft
 **  @description        : Synchronized TimeBase Manager
 **
 ***********************************************************************************************************************/
/* PRQA S 6060, 6520 EOF */ /* VL_MTR_StbM_STM19, VL_MTR_StbM_STVAR */
/* PRQA S 1503, 2889 ++ */  /* VL_QAC_NoUsedApi, VL_QAC_MultiReturn */
/* =================================================== inclusions =================================================== */
#include "StbM_Cfg.h"
#include "Std_Types.h"
#include "ComStack_Types.h"
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
/* SWS_StbM_00246 */
#if (STBM_ETHIF_TIMESTAMP_SUPPORT == STD_ON)
#include "EthIf.h"
#endif
#include "SchM_StbM.h"
#include "StbM_Internal.h"
#include "StbM.h"

/* ===================================================== macros ===================================================== */
#define TRIIGGERCUSTOMER_CFG(index) StbM_ConfigPtr->TriggeredCustomer[index]
#define TIMEBASE_CFG(index)         StbM_ConfigPtr->SynchronizedTimeBase[index]

#define STBM_USERDATA_LEN_MAX (3U)
#define STBM_SHIFT_BIT32      (32U)

#define STBM_ROUND_VALUE (0.5)

#if (STBM_NOTIFICATION_CUSTOMER_NUM > 0)
/**
 * @brief       Convert nanosecond time to microsecond time.
 * @param[in]   ns  nanosecond time
 * @return      uint64
 * @retval      microsecond time
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-74329
 */
STBM_LOCAL_INLINE uint64 StbM_GptTimer_Ns2us(uint64 ns)
{
    return ns / STBM_NS_PER_US;
}
#endif

/**
 * @brief       Shift a 64bit integer to the left by 32 bits.
 * @param[in]   x  Integer to be operated on
 * @return      uint64
 * @retval      The result after the shift
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-74330
 */
STBM_LOCAL_INLINE uint64 StbM_Shift_Left32(uint64 x)
{
    return x << STBM_SHIFT_BIT32;
}

/**
 * @brief       Shift a 64bit integer to the right by 32 bits.
 * @param[in]   x  Integer to be operated on
 * @return      uint64
 * @retval      The result after the shift
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-74331
 */
STBM_LOCAL_INLINE uint64 StbM_Shift_Right32(uint64 x)
{
    return x >> STBM_SHIFT_BIT32;
}

#if (STBM_DEV_ERROR_DETECT == STD_ON)
/**
 * @brief       Report a StbM diagnostic error.
 * @param[in]   apiId   The application ID
 * @param[in]   errorId The error ID
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-74332
 */
STBM_LOCAL_INLINE void StbM_Det_Report(uint8 apiId, uint8 errorId)
{
    (void)Det_ReportError(STBM_MODULE_ID, STBM_INSTANCE_ID, apiId, errorId);
}
#endif

#if (STBM_TIME_CORRECTION_SUPPORT == STD_ON)
/**
 * @brief       Calculate the absolute value.
 * @param[in]   value  The value to be calculated
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-74333
 */
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

/* ================================================ type definitions ================================================ */

/* ========================================= internal function declarations ========================================= */
#if (STBM_DEV_ERROR_DETECT == STD_ON)
#ifdef QAC_ANALYZE
#pragma PRQA_NO_SIDE_EFFECTS StbM_ValidateInit
#pragma PRQA_NO_SIDE_EFFECTS StbM_ValidatePointer
#pragma PRQA_NO_SIDE_EFFECTS StbM_ValidateCommom
#pragma PRQA_NO_SIDE_EFFECTS StbM_ValidatePartition
#pragma PRQA_NO_SIDE_EFFECTS StbM_ValidateNanoseconds
#pragma PRQA_NO_SIDE_EFFECTS StbM_ValidateUserData
#pragma PRQA_NO_SIDE_EFFECTS StbM_ValidateGlobalTimeMaster
#pragma PRQA_NO_SIDE_EFFECTS StbM_ValidateSynchronizedTimeBase
#if (STBM_TIME_CORRECTION_SUPPORT == STD_ON)
#pragma PRQA_NO_SIDE_EFFECTS StbM_ValidateAllowMasterRateCorrection
#endif
#if (STBM_NOTIFICATION_CUSTOMER_NUM > 0)
#pragma PRQA_NO_SIDE_EFFECTS StbM_ValidateNotificationCustomer
#endif
#endif
STBM_LOCAL boolean StbM_ValidateInit(const StbM_ConfigType* configPtr);
STBM_LOCAL boolean StbM_ValidatePointer(uint8 apiId, const void* pointer);
STBM_LOCAL boolean StbM_ValidateCommom(uint8 apiId, StbM_SynchronizedTimeBaseType timeBaseId, uint16 timeBaseIndex);
STBM_LOCAL boolean StbM_ValidatePartition(uint8 apiId, uint16 timeBaseIndex);
STBM_LOCAL boolean StbM_ValidateNanoseconds(uint8 apiId, uint32 nanoseconds);
STBM_LOCAL boolean StbM_ValidateUserData(uint8 apiId, const StbM_UserDataType* userDataPtr);
STBM_LOCAL boolean StbM_ValidateGlobalTimeMaster(uint8 apiId, uint16 timeBaseIndex);
STBM_LOCAL boolean StbM_ValidateSynchronizedTimeBase(uint8 apiId, uint16 timeBaseIndex);
#if (STBM_TIME_CORRECTION_SUPPORT == STD_ON)
STBM_LOCAL boolean StbM_ValidateAllowMasterRateCorrection(uint8 apiId, uint16 timeBaseIndex);
#endif
#if (STBM_NOTIFICATION_CUSTOMER_NUM > 0)
STBM_LOCAL
boolean StbM_ValidateNotificationCustomer(StbM_SynchronizedTimeBaseType timeBaseId, StbM_CustomerIdType customerId);
#endif
#endif
STBM_LOCAL uint16 StbM_GetPartitionIndex(void);
STBM_LOCAL uint16 StbM_GetTimebasePartitionIndex(uint16 timeBaseIndex);
STBM_LOCAL uint16 StbM_FindSynchronizedTimeBase(StbM_SynchronizedTimeBaseType timeBaseId);
STBM_LOCAL void   StbM_TimeBaseInit(uint16 index);
#if (STBM_NOTIFICATION_CUSTOMER_NUM > 0u)
STBM_LOCAL void StbM_NotificationCustomerInit(void);
#endif
#if (STBM_TRIGGERED_CUSTOMER_NUM > 0u)
STBM_LOCAL void StbM_TriiggerCustomerInit(void);
#endif
STBM_LOCAL Std_ReturnType StbM_LocalClockToLocalTime(
    const StbM_LocalTimeClockCfgType* localClockCfg,
    StbM_LocalClockTimeType*          localClockTime,
    uint64*                           timeStamp);
STBM_LOCAL Std_ReturnType     StbM_GetCurrentLocalTime(uint16 timeBaseIndex, StbM_LocalTimeType* localTime);
STBM_LOCAL void               StbM_GetGlobalTime64(uint16 timeBaseIndex, uint64* timeStamp);
STBM_LOCAL void               StbM_UpdateMainTimeTuple(uint16 timeBaseIndex, uint64 TL, uint64 TV);
STBM_LOCAL uint64             StbM_GTTimeTo64Time(StbM_TimeStampType timeStamp);
STBM_LOCAL StbM_TimeStampType StbM_64TimeToGTTime(uint64 time64);
STBM_LOCAL uint64             StbM_LTTimeTo64Time(StbM_VirtualLocalTimeType localTime);
STBM_LOCAL void               StbM_CheckTimeleap(uint16 timeBaseIndex, uint64 TL_Sync, uint64 TG_URx);
STBM_LOCAL void               StbM_CheckTimeout(uint16 timeBaseIndex, uint64 TV);

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
#endif

#if STBM_STATUS_NOTIFICATIONS_ENABLED == STD_ON
STBM_LOCAL void StbM_OnChangeTimeBaseStatus(
    StbM_TimeBaseNotificationType* notificationEvents,
    StbM_TimeBaseStatusType        oldTimeBaseStatus,
    StbM_TimeBaseStatusType        newTimeBaseStatus);
STBM_LOCAL void StbM_CheckStatusNotification(uint16 timeBaseIndex);
#endif

#if (STBM_TIME_RECORDING_SUPPORT == STD_ON)
STBM_LOCAL void StbM_TimeRecord(uint16 timeBaseIndex, uint64 TLsync, uint64 TGrx, uint64 TVrx, uint32 pathDelay);
STBM_LOCAL void StbM_TimeRecordback(uint16 timeBaseIndex);
#endif

STBM_LOCAL void StbM_SetUserDataIn(uint16 timeBaseIndex, const StbM_UserDataType* userDataPtr);

STBM_LOCAL Std_ReturnType StbM_SetGlobalTimePrivate(
    StbM_SynchronizedTimeBaseType timeBaseId,
    const StbM_TimeStampType*     timeStamp,
    const StbM_UserDataType*      userData,
    uint8                         serviceId,
    boolean                       isUpdateCounter);

#if (STBM_NOTIFICATION_CUSTOMER_NUM > 0)
STBM_LOCAL void StbM_TimerNotificationCustomer(void);
#endif

#if (STBM_TRIGGERED_CUSTOMER_NUM > 0u)
STBM_LOCAL void StbM_TriggeredCustomer(void);
#endif

/* ============================================ internal data definitions =========================================== */
#define STBM_START_SEC_VAR_CLEARED_BOOLEAN
#include "StbM_MemMap.h"
STBM_LOCAL boolean StbM_InitStatus[STBM_PARTITION_NUMBER];
#define STBM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "StbM_MemMap.h"

#if (STBM_TRIGGERED_CUSTOMER_NUM > 0)
#define STBM_START_SEC_VAR_CLEARED_16
#include "StbM_MemMap.h"
STBM_LOCAL uint16 StbM_TriggerRemainPeriod[STBM_TRIGGERED_CUSTOMER_NUM];
#define STBM_STOP_SEC_VAR_CLEARED_16
#include "StbM_MemMap.h"
#endif

#define STBM_START_SEC_VAR_CLEARED_PTR
#include "StbM_MemMap.h"
STBM_LOCAL const StbM_ConfigType* StbM_ConfigPtr;
#define STBM_STOP_SEC_VAR_CLEARED_PTR
#include "StbM_MemMap.h"

/* ============================================ external data definition ============================================ */

/* ========================================== external function definitions ========================================= */
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"

/**
 * The implementation of StbM_Init.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void StbM_Init(const StbM_ConfigType* configPtr)
/* PRQA S 1532 -- */
{
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateInit(configPtr))
#endif
    {
        if (StbM_ConfigPtr == NULL_PTR)
        {
            StbM_ConfigPtr = configPtr;
        }

        for (uint16 index = 0u; index < STBM_TIME_BASE_NUM; index++)
        {
#if STBM_MULTIPLE_PARTITION_ENABLED == STD_ON
            if (GetApplicationID() != TIMEBASE_CFG(index).ApplicationId)
            {
                continue; /* PRQA S 0770 */ /* VL_StbM_0770 */
            }
#endif
            StbM_TimeBaseInit(index);
        }

#if (STBM_NOTIFICATION_CUSTOMER_NUM > 0u)
        StbM_NotificationCustomerInit();
#endif

#if (STBM_TRIGGERED_CUSTOMER_NUM > 0u)
        StbM_TriiggerCustomerInit();
#endif

        StbM_InitStatus[StbM_GetPartitionIndex()] = TRUE;
    }
}

#if (STBM_VERSIONINFO_API == STD_ON)
/**
 * The implementation of StbM_GetVersionInfo.
 */
void StbM_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidatePointer(STBM_SID_GETVERSIONINFO, versioninfo))
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
 * The implementation of StbM_GetCurrentTime.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_StbM_STCAL */
Std_ReturnType StbM_GetCurrentTime(
    StbM_SynchronizedTimeBaseType timeBaseId,
    StbM_TimeTupleType*           timeTuple,
    StbM_UserDataType*            userData)
/* PRQA S 6070 -- */
{
    Std_ReturnType ret           = E_NOT_OK;
    uint16         timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommom(STBM_SID_GETCURRENTTIME, timeBaseId, timeBaseIndex)
        && StbM_ValidatePointer(STBM_SID_GETCURRENTTIME, timeTuple)
        && StbM_ValidatePointer(STBM_SID_GETCURRENTTIME, userData)
        && StbM_ValidatePartition(STBM_SID_GETCURRENTTIME, timeBaseIndex))
#endif
    {
        uint64 TV = 0u;
        /* SWS_StbM_00173 SWS_StbM_00434 */
        ret = StbM_GetCurrentLocalTime(timeBaseIndex, &TV);
        if (ret == E_OK)
        {
            const StbM_TimeBaseType* timeBasePtr = StbM_TimeBasePtr[timeBaseIndex];
            uint64                   TL          = TV;
            StbM_GetGlobalTime64(timeBaseIndex, &TL);

            SchM_Enter_StbM_Context();
            timeTuple->globalTime                     = StbM_64TimeToGTTime(TL);
            timeTuple->timeBaseStatus                 = timeBasePtr->TimeBaseStatus;
            timeTuple->virtualLocalTime.nanosecondsHi = (uint32)StbM_Shift_Right32(TV);
            timeTuple->virtualLocalTime.nanosecondsLo = (uint32)TV;
            *userData                                 = timeBasePtr->UserData;
            SchM_Exit_StbM_Context();
        }
    }

    return ret;
}

/**
 * The implementation of StbM_GetCurrentVirtualLocalTime.
 */
Std_ReturnType
    StbM_GetCurrentVirtualLocalTime(StbM_SynchronizedTimeBaseType timeBaseId, StbM_VirtualLocalTimeType* localTimePtr)
{
    Std_ReturnType ret           = E_NOT_OK;
    uint16         timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommom(STBM_SID_GETCURRENTVIRTUALLOCALTIME, timeBaseId, timeBaseIndex)
        && StbM_ValidatePointer(STBM_SID_GETCURRENTVIRTUALLOCALTIME, localTimePtr))
#endif
    {
        /* SWS_StbM_00437 */
        uint64 time64 = 0u;
        ret           = StbM_GetCurrentLocalTime(timeBaseIndex, &time64);
        if (ret == E_OK)
        {
            localTimePtr->nanosecondsHi = (uint32)StbM_Shift_Right32(time64);
            localTimePtr->nanosecondsLo = (uint32)time64;
        }
    }

    return ret;
}

/**
 * The implementation of StbM_SetGlobalTime.
 */
Std_ReturnType StbM_SetGlobalTime(
    StbM_SynchronizedTimeBaseType timeBaseId,
    const StbM_TimeStampType*     timeStamp,
    const StbM_UserDataType*      userData)
{
    return StbM_SetGlobalTimePrivate(timeBaseId, timeStamp, userData, STBM_SID_SETGLOBALTIME, TRUE);
}

/**
 * The implementation of StbM_UpdateGlobalTime.
 */
Std_ReturnType StbM_UpdateGlobalTime(
    StbM_SynchronizedTimeBaseType timeBaseId,
    const StbM_TimeStampType*     timeStamp,
    const StbM_UserDataType*      userData)
{
    return StbM_SetGlobalTimePrivate(timeBaseId, timeStamp, userData, STBM_SID_UPDATEGLOBALTIME, FALSE);
}

/**
 * The implementation of StbM_SetUserData.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_StbM_STCAL */
Std_ReturnType StbM_SetUserData(StbM_SynchronizedTimeBaseType timeBaseId, const StbM_UserDataType* userData)
/* PRQA S 6070 -- */
{
    Std_ReturnType ret           = E_NOT_OK;
    uint16         timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommom(STBM_SID_SETUSERDATA, timeBaseId, timeBaseIndex)
        && StbM_ValidatePointer(STBM_SID_SETUSERDATA, userData) && StbM_ValidateUserData(STBM_SID_SETUSERDATA, userData)
        && StbM_ValidateGlobalTimeMaster(STBM_SID_SETUSERDATA, timeBaseIndex) /* SWS_StbM_00630 */
        && StbM_ValidatePartition(STBM_SID_SETUSERDATA, timeBaseIndex))
#endif
    {
        SchM_Enter_StbM_Context();
        StbM_SetUserDataIn(timeBaseIndex, userData);
        SchM_Exit_StbM_Context();
        ret = E_OK;
    }

    return ret;
}

/**
 * The implementation of StbM_BusSetGlobalTime.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_StbM_STCAL */
Std_ReturnType StbM_BusSetGlobalTime(
    StbM_SynchronizedTimeBaseType timeBaseId,
    const StbM_TimeTupleType*     timeTuplePtr,
    const StbM_UserDataType*      userDataPtr,
    const StbM_MeasurementType*   measureDataPtr)
/* PRQA S 6070 -- */
{
    Std_ReturnType ret           = E_NOT_OK;
    uint16         timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommom(STBM_SID_BUSSETGLOBALTIME, timeBaseId, timeBaseIndex)
        && StbM_ValidatePointer(STBM_SID_BUSSETGLOBALTIME, timeTuplePtr)
        && StbM_ValidatePointer(STBM_SID_BUSSETGLOBALTIME, measureDataPtr)
        && StbM_ValidateNanoseconds(STBM_SID_BUSSETGLOBALTIME, timeTuplePtr->globalTime.nanoseconds)
        && StbM_ValidateUserData(STBM_SID_BUSSETGLOBALTIME, userDataPtr)
        && StbM_ValidateSynchronizedTimeBase(STBM_SID_BUSSETGLOBALTIME, timeBaseIndex)
        && StbM_ValidatePartition(STBM_SID_BUSSETGLOBALTIME, timeBaseIndex))
#endif
    {
        StbM_TimeBaseType* timeBasePtr = StbM_TimeBasePtr[timeBaseIndex];
#if STBM_STATUS_NOTIFICATIONS_ENABLED == STD_ON
        StbM_TimeBaseStatusType oldTimeBaseStatus = timeBasePtr->TimeBaseStatus;
#endif
        uint64 TV_Sync = 0u;
        ret            = StbM_GetCurrentLocalTime(timeBaseIndex, &TV_Sync);
        if (ret == E_OK)
        {
            SchM_Enter_StbM_Context();
            timeBasePtr->TimeBaseStatus |= STBM_TIMEBASE_STATUS_GLOBAL_TIME_BASE;
            timeBasePtr->LastBusSettime = TV_Sync;
            SchM_Exit_StbM_Context();

            /* SWS_StbM_00438 */
            uint64 TL_Sync = TV_Sync;
            StbM_GetGlobalTime64(timeBaseIndex, &TL_Sync);

            /* SWS_StbM_00529 */
            uint64 TV_Rx  = StbM_LTTimeTo64Time(timeTuplePtr->virtualLocalTime);
            uint64 T_SPD  = TV_Sync - TV_Rx;
            uint64 TG_Rx  = StbM_GTTimeTo64Time(timeTuplePtr->globalTime);
            uint64 TV_URx = TV_Sync;
            /* PRQA S 4119, 4395 ++ */ /* VL_StbM_4119, VL_StbM_4395 */
            uint64 TL_URx =
                TG_Rx
                + (uint64)(((float64)(timeBasePtr->RateDeviation.RrcValue) / (float64)STBM_RATE_PPM) * (float64)T_SPD);
            /* PRQA S 4119, 4395 -- */

            StbM_CheckTimeleap(timeBaseIndex, TL_Sync, TL_URx);

            if (timeBasePtr->BusNotFirstSet == FALSE)
            {
                timeBasePtr->BusNotFirstSet = TRUE;
            }
#if (STBM_TIME_RECORDING_SUPPORT == STD_ON)
            StbM_TimeRecord(
                timeBaseId,
                TL_Sync,
                TL_URx,
                TV_URx,
                measureDataPtr->pathDelay); /* SWS_StbM_00313 SWS_StbM_00314 */
#else
            STBM_UNUSED(measureDataPtr);
#endif

#if (STBM_TIME_CORRECTION_SUPPORT == STD_ON)
            StbM_TimeCorrection(timeBaseIndex, &TL_URx, &TV_URx, timeTuplePtr->timeBaseStatus);
#endif
            /* SWS_StbM_00184 */
            SchM_Enter_StbM_Context();
            if (((timeTuplePtr->timeBaseStatus) & STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY) != 0u)
            {
                timeBasePtr->TimeBaseStatus |= STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY;
            }
            else
            {
                timeBasePtr->TimeBaseStatus &=
                    (StbM_TimeBaseStatusType)(~(StbM_TimeBaseStatusType)STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY);
            }
            SchM_Exit_StbM_Context();
            /* SWS_StbM_00436 SWS_StbM_00433 */
            StbM_UpdateMainTimeTuple(timeBaseIndex, TL_URx, TV_URx);

            /* SWS_StbM_00183 */
            SchM_Enter_StbM_Context();
            timeBasePtr->TimeBaseStatus &=
                (StbM_TimeBaseStatusType)(~(StbM_TimeBaseStatusType)STBM_TIMEBASE_STATUS_TIMEOUT);
            SchM_Exit_StbM_Context();
#if STBM_STATUS_NOTIFICATIONS_ENABLED == STD_ON
            StbM_OnChangeTimeBaseStatus(
                &timeBasePtr->NotificationEvents,
                oldTimeBaseStatus,
                timeBasePtr->TimeBaseStatus);
            SchM_Enter_StbM_Context();
            timeBasePtr->NotificationEvents |= STBM_TIMEBASE_EV_RESYNC;
            SchM_Exit_StbM_Context();
#endif
            /* SWS_StbM_00350 */
            SchM_Enter_StbM_Context();
            ++timeBasePtr->TimeBaseUpdateCounter;           /* Free-Running Counter */
            StbM_SetUserDataIn(timeBaseIndex, userDataPtr); /* SWS_StbM_00179 */
            SchM_Exit_StbM_Context();
        }
    }

    return ret;
}

/**
 * The implementation of StbM_GetRateDeviation.
 */
Std_ReturnType StbM_GetRateDeviation(
    StbM_SynchronizedTimeBaseType timeBaseId,
    StbM_RateDeviationType*       rateDeviation) /* PRQA S 3673 */ /* VL_StbM_3673 */
{
    Std_ReturnType ret = E_NOT_OK;
#if STBM_TIME_CORRECTION_SUPPORT == STD_ON
    uint16 timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommom(STBM_SID_GETRATEDEVIATION, timeBaseId, timeBaseIndex)
        && StbM_ValidatePointer(STBM_SID_GETRATEDEVIATION, rateDeviation)
        && StbM_ValidatePartition(STBM_SID_GETRATEDEVIATION, timeBaseIndex))
#endif
    {
        const StbM_TimeBaseType* const timeBasePtr = StbM_TimeBasePtr[timeBaseIndex];
        SchM_Enter_StbM_Context();
        if (timeBasePtr->RateDeviation.IsSet)
        {
            *rateDeviation = (StbM_RateDeviationType)(timeBasePtr->RateDeviation.RrcValue - (sint32)STBM_RATE_PPM);
            ret            = E_OK;
        }
        SchM_Exit_StbM_Context();
    }
#else
    STBM_UNUSED(timeBaseId);
    STBM_UNUSED(rateDeviation);
#endif

    return ret;
}

/**
 * The implementation of StbM_SetRateCorrection.
 */
Std_ReturnType StbM_SetRateCorrection(StbM_SynchronizedTimeBaseType timeBaseId, StbM_RateDeviationType rateDeviation)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STBM_TIME_CORRECTION_SUPPORT == STD_ON)
    uint16 timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommom(STBM_SID_SETRATECORRECTION, timeBaseId, timeBaseIndex)
        && StbM_ValidatePointer(STBM_SID_SETRATECORRECTION, TIMEBASE_CFG(timeBaseIndex).TimeCorrection)
        && StbM_ValidateAllowMasterRateCorrection(STBM_SID_SETRATECORRECTION, timeBaseIndex)
        && StbM_ValidateGlobalTimeMaster(STBM_SID_SETRATECORRECTION, timeBaseIndex)
        && StbM_ValidatePartition(STBM_SID_SETRATECORRECTION, timeBaseIndex))
#endif
    {
        const StbM_SynchronizedTimeBaseCfgType* timeBaseCfg = &TIMEBASE_CFG(timeBaseIndex);
        StbM_RateDeviationType                  rate        = 0;
        uint16 RateDeviationMax = timeBaseCfg->TimeCorrection->MasterRateDeviationMax; /* Range: 0 .. 32000  */
        if ((sint32)rateDeviation >= (sint32)RateDeviationMax)
        {
            rate = (StbM_RateDeviationType)RateDeviationMax;
        }
        else if ((sint32)rateDeviation <= -(sint32)RateDeviationMax)
        {
            rate = (StbM_RateDeviationType)(-(sint32)RateDeviationMax);
        }
        else
        {
            rate = rateDeviation;
        }

        StbM_CurrentRateType* rateDeviationPtr = &StbM_TimeBasePtr[timeBaseIndex]->RateDeviation;

        SchM_Enter_StbM_Context();
        rateDeviationPtr->RrcValue = (sint32)rate + (sint32)STBM_RATE_PPM;
        rateDeviationPtr->RocValue = 0;
        rateDeviationPtr->IsSet    = TRUE;
        /* SWS_StbM_00568 */
        StbM_TimeBasePtr[timeBaseIndex]->TimeBaseStatus |= (StbM_TimeBaseStatusType)STBM_TIMEBASE_STATUS_RATE_CORRECTED;
        SchM_Exit_StbM_Context();

        /* SWS_StbM_00583 */
        uint64 TV = 0u;
        ret       = StbM_GetCurrentLocalTime(timeBaseIndex, &TV);
        if (ret == E_OK)
        {
            uint64 TL = TV;
            StbM_GetGlobalTime64(timeBaseIndex, &TL);
            StbM_UpdateMainTimeTuple(timeBaseIndex, TL, TV);
        }
    }
#else
    STBM_UNUSED(timeBaseId);
    STBM_UNUSED(rateDeviation);
#endif

    return ret;
}

/**
 * The implementation of StbM_GetTimeLeap.
 */
Std_ReturnType StbM_GetTimeLeap(StbM_SynchronizedTimeBaseType timeBaseId, StbM_TimeDiffType* timeJump)
{
    Std_ReturnType ret           = E_NOT_OK;
    uint16         timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommom(STBM_SID_GETTIMELEAP, timeBaseId, timeBaseIndex)
        && StbM_ValidateSynchronizedTimeBase(STBM_SID_GETTIMELEAP, timeBaseIndex)
        && StbM_ValidatePointer(STBM_SID_GETTIMELEAP, timeJump)
        && StbM_ValidatePartition(STBM_SID_GETTIMELEAP, timeBaseIndex))
#endif
    {
        SchM_Enter_StbM_Context();
        if (StbM_TimeBasePtr[timeBaseIndex]->TimeLeapValid) /* SWS_StbM_00425 */
        {
            *timeJump = StbM_TimeBasePtr[timeBaseIndex]->TimeLeap;
            ret       = E_OK;
        }
        SchM_Exit_StbM_Context();
    }

    return ret;
}

/**
 * The implementation of StbM_GetTimeBaseStatus.
 */
Std_ReturnType StbM_GetTimeBaseStatus(StbM_SynchronizedTimeBaseType timeBaseId, StbM_TimeBaseStatusType* timeBaseStatus)
{
    Std_ReturnType ret           = E_NOT_OK;
    uint16         timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommom(STBM_SID_GETTIMEBASESTATUS, timeBaseId, timeBaseIndex)
        && StbM_ValidatePointer(STBM_SID_GETTIMEBASESTATUS, timeBaseStatus)
        && StbM_ValidatePartition(STBM_SID_GETTIMEBASESTATUS, timeBaseIndex))
#endif
    {
        SchM_Enter_StbM_Context();
        *timeBaseStatus = StbM_TimeBasePtr[timeBaseIndex]->TimeBaseStatus;
        SchM_Exit_StbM_Context();
        ret = E_OK;
    }

    return ret;
}

#if (STBM_NOTIFICATION_CUSTOMER_NUM > 0)
/**
 * The implementation of StbM_StartTimer.
 */
Std_ReturnType StbM_StartTimer(
    StbM_SynchronizedTimeBaseType timeBaseId,
    StbM_CustomerIdType           customerId,
    const StbM_TimeStampType*     expireTime)
{
    Std_ReturnType ret           = E_NOT_OK;
    uint16         timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommom(STBM_SID_STARTTIMER, timeBaseId, timeBaseIndex)
        && StbM_ValidateNotificationCustomer(timeBaseId, customerId)
        && StbM_ValidatePointer(STBM_SID_STARTTIMER, expireTime)
        && StbM_ValidateNanoseconds(STBM_SID_STARTTIMER, expireTime->nanoseconds)
        && StbM_ValidatePartition(STBM_SID_STARTTIMER, timeBaseIndex))
#endif
    {
        StbM_NotificationType* notificationPtr = StbM_NotificationPtr[customerId];
        SchM_Enter_StbM_Context();
        if (!notificationPtr->Isrunning)
        {
            notificationPtr->CustomerTimerExpireTime = StbM_GTTimeTo64Time(*expireTime);
            notificationPtr->IsSet                   = TRUE;
            ret                                      = E_OK;
        }
        SchM_Exit_StbM_Context();
    }
    return ret;
}
#endif

#if (STBM_TIME_RECORDING_SUPPORT == STD_ON)
/**
 * The implementation of StbM_GetSyncTimeRecordHead.
 */
Std_ReturnType StbM_GetSyncTimeRecordHead(
    StbM_SynchronizedTimeBaseType timeBaseId,
    StbM_SyncRecordTableHeadType* syncRecordTableHead)
{
    Std_ReturnType ret           = E_NOT_OK;
    uint16         timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommom(STBM_SID_GETSYNCTIMERECORDHEAD, timeBaseId, timeBaseIndex)
        && StbM_ValidateSynchronizedTimeBase(STBM_SID_GETSYNCTIMERECORDHEAD, timeBaseIndex)
        && StbM_ValidatePointer(STBM_SID_GETSYNCTIMERECORDHEAD, syncRecordTableHead))
#endif
    {
        syncRecordTableHead->SynchronizedTimeDomain = (uint8)timeBaseId;
        syncRecordTableHead->HWfrequency            = TIMEBASE_CFG(timeBaseIndex).LocalTimeClock->ClockFrequency;
        syncRecordTableHead->HWprescaler            = TIMEBASE_CFG(timeBaseIndex).LocalTimeClock->ClockPrescaler;
    }
    return ret;
}
#endif

/**
 * The implementation of StbM_TriggerTimeTransmission.
 */
Std_ReturnType StbM_TriggerTimeTransmission(StbM_SynchronizedTimeBaseType timeBaseId)
{
    Std_ReturnType ret           = E_NOT_OK;
    uint16         timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommom(STBM_SID_TRIGGERTIMETRANSMISSION, timeBaseId, timeBaseIndex)
        && StbM_ValidateSynchronizedTimeBase(STBM_SID_TRIGGERTIMETRANSMISSION, timeBaseIndex)
        && StbM_ValidatePartition(STBM_SID_TRIGGERTIMETRANSMISSION, timeBaseIndex))
#endif
    {
        SchM_Enter_StbM_Context();
        ++StbM_TimeBasePtr[timeBaseIndex]->TimeBaseUpdateCounter;
        SchM_Exit_StbM_Context();
    }
    return ret;
}

/**
 * The implementation of StbM_GetTimeBaseUpdateCounter.
 */
uint8 StbM_GetTimeBaseUpdateCounter(StbM_SynchronizedTimeBaseType timeBaseId)
{
    uint8  updateCounter = 0u;
    uint16 timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommom(STBM_SID_GETTIMEBASEUPDATECOUNTER, timeBaseId, timeBaseIndex)
        && StbM_ValidateSynchronizedTimeBase(STBM_SID_GETTIMEBASEUPDATECOUNTER, timeBaseIndex)
        && StbM_ValidatePartition(STBM_SID_GETTIMEBASEUPDATECOUNTER, timeBaseIndex))
#endif
    {
        SchM_Enter_StbM_Context();
        updateCounter = StbM_TimeBasePtr[timeBaseIndex]->TimeBaseUpdateCounter;
        SchM_Exit_StbM_Context();
    }

    return updateCounter;
}

#if (STBM_ALLOW_SYSTEM_WIDE_GLOBAL_TIME_MASTER == STD_ON)
/**
 * The implementation of StbM_GetMasterConfig.
 */
Std_ReturnType StbM_GetMasterConfig(StbM_SynchronizedTimeBaseType timeBaseId, StbM_MasterConfigType* masterConfig)
{
    Std_ReturnType ret           = E_NOT_OK;
    uint16         timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommom(STBM_SID_GETMASTERCONFIG, timeBaseId, timeBaseIndex)
        && StbM_ValidatePointer(STBM_SID_GETMASTERCONFIG, masterConfig))
#endif
    {
        /* SWS_StbM_00408 */
        if (TIMEBASE_CFG(timeBaseIndex).IsSystemWideGlobalTimeMaster)
        {
            *masterConfig = STBM_SYSTEM_WIDE_MASTER_ENABLED;
        }
        else
        {
            *masterConfig = STBM_SYSTEM_WIDE_MASTER_DISABLED;
        }
        ret = E_OK;
    }

    return ret;
}
#endif

#if (STBM_NOTIFICATION_CUSTOMER_NUM > 0)
/**
 * @brief       When the timer time is up, perform a callback.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-71870
 */
void StbM_TimerCallback(void)
{
    uint8 index;
    for (index = 0u; index < STBM_NOTIFICATION_CUSTOMER_NUM; index++)
    {
        StbM_NotificationType*                  notificationPtr = StbM_NotificationPtr[index];
        const StbM_NotificationCustomerCfgType* customerPtr     = &StbM_NotificationCustomer[index];
        if (notificationPtr->GPTtimercalled)
        {
            uint64            TL;
            StbM_TimeDiffType timeDiff;
            uint16            timeBaseIndex = StbM_FindSynchronizedTimeBase(customerPtr->TimeBaseId);
            (void)StbM_GetCurrentLocalTime(timeBaseIndex, &TL);
            (void)StbM_GetGlobalTime64(timeBaseIndex, &TL);
            sint64 diff;
            if (notificationPtr->CustomerTimerExpireTime >= TL)
            {
                diff = (sint64)(notificationPtr->CustomerTimerExpireTime - TL); /* PRQA S 4394 */ /* VL_StbM_4394 */
            }
            else
            {
                diff = -(sint64)(TL - notificationPtr->CustomerTimerExpireTime); /* PRQA S 4394 */ /* VL_StbM_4394 */
            }

            if (diff > (sint64)StbM_TimeDiffType_UpperLimit)
            {
                timeDiff = StbM_TimeDiffType_UpperLimit;
            }
            else if (diff < (sint64)StbM_TimeDiffType_LowerLimit)
            {
                timeDiff = StbM_TimeDiffType_LowerLimit;
            }
            else
            {
                timeDiff = (StbM_TimeDiffType)diff;
            }

            const StbMTimeNtfCallbackType timeNtfCallback = customerPtr->TimeNotificationCallback;
            if (NULL_PTR != timeNtfCallback)
            {
                timeNtfCallback(timeDiff);
            }
            notificationPtr->GPTtimercalled = FALSE;
            notificationPtr->Isrunning      = FALSE;
            notificationPtr->IsSet          = FALSE;
        }
    }
}
#endif

/**
 * The implementation of StbM_MainFunction.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void StbM_MainFunction(void)
/* PRQA S 1532 -- */
{
    if (StbM_InitStatus[StbM_GetPartitionIndex()] == TRUE)
    {
        uint64 TV = 0u;
        for (uint16 index = 0u; index < STBM_TIME_BASE_NUM; index++)
        {
#if STBM_MULTIPLE_PARTITION_ENABLED == STD_ON
            if (GetApplicationID() != TIMEBASE_CFG(index).ApplicationId)
            {
                continue; /* PRQA S 0770 */ /* VL_StbM_0770 */
            }
#endif
            if (StbM_GetCurrentLocalTime(index, &TV) == E_OK)
            {
                StbM_CheckTimeout(index, TV); /* SWS_StbM_00540 */
            }
#if STBM_STATUS_NOTIFICATIONS_ENABLED == STD_ON
            StbM_CheckStatusNotification(index);
#endif

#if (STBM_TIME_RECORDING_SUPPORT == STD_ON)
            StbM_TimeRecordback(index);
#endif
        }

#if (STBM_NOTIFICATION_CUSTOMER_NUM > 0u)
        StbM_TimerNotificationCustomer();
#endif

#if (STBM_TRIGGERED_CUSTOMER_NUM > 0u)
        StbM_TriggeredCustomer();
#endif
    }
}

#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

/* ========================================== internal function definitions ========================================= */

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"

#if (STBM_DEV_ERROR_DETECT == STD_ON)
/**
 * @brief Development error validation of StbM_Init
 * @param[in]    configPtr   StbM config data
 * @return       boolean
 * @retval       TRUE    Development error not occurred
 * @retval       FALSE   Development error occurred
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
STBM_LOCAL boolean StbM_ValidateInit(const StbM_ConfigType* configPtr)
{
#if STBM_MULTIPLE_PARTITION_ENABLED == STD_ON
    if (StbM_GetPartitionIndex() >= STBM_PARTITION_NUMBER)
    {
        StbM_Det_Report(STBM_SID_INIT, STBM_E_INVALID_PARTITION);
        return FALSE;
    }
#endif

    if (configPtr == NULL_PTR)
    {
        StbM_Det_Report(STBM_SID_INIT, STBM_E_PARAM_POINTER);
        return FALSE;
    }

    if (StbM_InitStatus[StbM_GetPartitionIndex()])
    {
        StbM_Det_Report(STBM_SID_INIT, STBM_E_ALREADY_INITIALIZED);
        return FALSE;
    }

    return TRUE;
}

/**
 * @brief Development error validation of null pointer
 * @param[in]    apiId   ID of API service in which error is detected
 * @param[in]    pointer pointer
 * @return       boolean
 * @retval       TRUE    Development error not occurred
 * @retval       FALSE   Development error occurred
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-75407
 */
STBM_LOCAL boolean StbM_ValidatePointer(uint8 apiId, const void* pointer)
{
    if (pointer == NULL_PTR)
    {
        StbM_Det_Report(apiId, STBM_E_PARAM_POINTER);
        return FALSE;
    }
    return TRUE;
}

/**
 * @brief       Check if the timeBaseId is valid and if the StbM of the current
 *              partition have been initialized
 * @param[in]   apiId   ID of API service in which error is detected
 * @param[in]   timeBaseId    Identification of a Time Base
 * @param[in]   timeBaseIndex The index of the time base with ID timeBaseId
 * @return      boolean
 * @retval      TRUE    Development error not occurred
 * @retval      FALSE   Development error occurred
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71834
 */
STBM_LOCAL boolean StbM_ValidateCommom(uint8 apiId, StbM_SynchronizedTimeBaseType timeBaseId, uint16 timeBaseIndex)
{
    if (timeBaseId >= STBM_TIMEBASE_ID_RESERVE)
    {
        StbM_Det_Report(apiId, STBM_E_PARAM);
        return FALSE;
    }

    if (timeBaseIndex >= STBM_TIME_BASE_NUM)
    {
        StbM_Det_Report(apiId, STBM_E_PARAM);
        return FALSE;
    }

    if (!StbM_InitStatus[StbM_GetTimebasePartitionIndex(timeBaseIndex)])
    {
        StbM_Det_Report(apiId, STBM_E_UNINIT);
        return FALSE;
    }

    return TRUE;
}

/**
 * @brief        Check if this partition is allowed to be accessed
 * @param[in]    apiId      ID of API service in which error is detected
 * @param[in]    timeBaseIndex The index of the time base
 * @return       boolean
 * @retval       TRUE    Development error not occurred
 * @retval       FALSE   Development error occurred
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-71832
 */
STBM_LOCAL boolean StbM_ValidatePartition(uint8 apiId, uint16 timeBaseIndex)
{
#if STBM_MULTIPLE_PARTITION_ENABLED == STD_ON
    if (TIMEBASE_CFG(timeBaseIndex).ApplicationId == GetApplicationID())
    {
        return TRUE;
    }
    else if (TIMEBASE_CFG(timeBaseIndex).IsTimeGateway)
    {
        return TRUE;
    }
    else if (TIMEBASE_CFG(timeBaseIndex).CrossCoreAccess)
    {
        return TRUE;
    }
    else
    {
        StbM_Det_Report(apiId, STBM_E_INVALID_PARTITION);
        return FALSE;
    }
#else
    STBM_UNUSED(apiId);
    STBM_UNUSED(timeBaseIndex);
    return TRUE;
#endif
}

/**
 * @brief       whether a nanosecond value is reasonable
 * @param[in]   apiId   ID of API service in which error is detected
 * @param[in]   nanoseconds    Nanosecond
 * @return      boolean
 * @retval      TRUE    Development error not occurred
 * @retval      FALSE   Development error occurred
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-75408
 */
STBM_LOCAL boolean StbM_ValidateNanoseconds(uint8 apiId, uint32 nanoseconds)
{
    if (nanoseconds >= STBM_NS_PER_S)
    {
        StbM_Det_Report(apiId, STBM_E_PARAM_TIMESTAMP);
        return FALSE;
    }

    return TRUE;
}

/**
 * @brief       whether user data is reasonable
 * @param[in]   apiId       ID of API service in which error is detected
 * @param[in]   userDataPtr User data
 * @return      boolean
 * @retval      TRUE    Development error not occurred
 * @retval      FALSE   Development error occurred
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-75409
 */
STBM_LOCAL boolean StbM_ValidateUserData(uint8 apiId, const StbM_UserDataType* userDataPtr)
{
    if ((userDataPtr != NULL_PTR) && (userDataPtr->userDataLength > STBM_USERDATA_LEN_MAX))
    {
        StbM_Det_Report(apiId, STBM_E_PARAM_USERDATA);
        return FALSE;
    }

    return TRUE;
}

/**
 * @brief       Whether is a wide global time master
 * @param[in]   apiId       ID of API service in which error is detected
 * @param[in]   timeBaseIndex The index of the time base
 * @return      boolean
 * @retval      TRUE    Development error not occurred
 * @retval      FALSE   Development error occurred
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-75410
 */
STBM_LOCAL boolean StbM_ValidateGlobalTimeMaster(uint8 apiId, uint16 timeBaseIndex)
{
    if (!TIMEBASE_CFG(timeBaseIndex).IsSystemWideGlobalTimeMaster)
    {
        StbM_Det_Report(apiId, STBM_E_SERVICE_NOT_SUPPORTED);
        return FALSE;
    }

    return TRUE;
}

/**
 * @brief       Whether is a synchronized time base
 * @param[in]   apiId       ID of API service in which error is detected
 * @param[in]   timeBaseIndex The index of the time base
 * @return      boolean
 * @retval      TRUE    Development error not occurred
 * @retval      FALSE   Development error occurred
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-75411
 */
STBM_LOCAL boolean StbM_ValidateSynchronizedTimeBase(uint8 apiId, uint16 timeBaseIndex)
{
    if (TIMEBASE_CFG(timeBaseIndex).SynchronizedTimeBaseType == STBM_TBTYPE_PURELOCAL)
    {
        StbM_Det_Report(apiId, STBM_E_PARAM);
        return FALSE;
    }

    return TRUE;
}

#if (STBM_TIME_CORRECTION_SUPPORT == STD_ON)
/**
 * @brief       Whether allow master time base set a rate correction
 * @param[in]   apiId       ID of API service in which error is detected
 * @param[in]   timeBaseIndex The index of the time base
 * @return      boolean
 * @retval      TRUE    Development error not occurred
 * @retval      FALSE   Development error occurred
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-75412
 */
STBM_LOCAL boolean StbM_ValidateAllowMasterRateCorrection(uint8 apiId, uint16 timeBaseIndex)
{
    if (!TIMEBASE_CFG(timeBaseIndex).TimeCorrection->AllowMasterRateCorrection)
    {
        StbM_Det_Report(apiId, STBM_E_SERVICE_NOT_SUPPORTED);
        return FALSE;
    }

    return TRUE;
}
#endif

#if (STBM_NOTIFICATION_CUSTOMER_NUM > 0)
/**
 * @brief       Whether the costomer is valid
 * @param[in]   apiId       ID of API service in which error is detected
 * @param[in]   timeBaseId Identification of a Time Base
 * @param[in]   customerId The ID of the customer
 * @return      boolean
 * @retval      TRUE    Development error not occurred
 * @retval      FALSE   Development error occurred
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-75413
 */
STBM_LOCAL boolean
    StbM_ValidateNotificationCustomer(StbM_SynchronizedTimeBaseType timeBaseId, StbM_CustomerIdType customerId)
{
    if ((customerId >= STBM_NOTIFICATION_CUSTOMER_NUM)
        || (timeBaseId != StbM_NotificationCustomer[customerId].TimeBaseId))
    {
        StbM_Det_Report(STBM_SID_STARTTIMER, STBM_E_PARAM);
        return FALSE;
    }

    return TRUE;
}
#endif
#endif

/**
 * @brief       Returns the index of current partition in the CanTSyn configuration.
 * @return      uint16
 * @retval      The index of current partition in the CanTSyn configuration
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71831
 */
STBM_LOCAL uint16 StbM_GetPartitionIndex(void)
{
    uint16 index = 0u;
#if STBM_MULTIPLE_PARTITION_ENABLED == STD_ON
    ApplicationType applicationID = GetApplicationID();
    for (; index < STBM_PARTITION_NUMBER; ++index)
    {
        if (StbM_OsApplicationList[index] == applicationID)
        {
            break;
        }
    }
#endif
    return index;
}

/**
 * @brief       Returns the the partition index of the time base.
 * @param[in]   timeBaseIndex  Time base index
 * @return      uint16
 * @retval      The index of partition of the time base
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-74334
 */
STBM_LOCAL uint16 StbM_GetTimebasePartitionIndex(uint16 timeBaseIndex)
{
    uint16 index = 0u;
#if STBM_MULTIPLE_PARTITION_ENABLED == STD_ON
    ApplicationType applicationID = TIMEBASE_CFG(timeBaseIndex).ApplicationId;
    for (; index < STBM_PARTITION_NUMBER; ++index)
    {
        if (StbM_OsApplicationList[index] == applicationID)
        {
            break;
        }
    }
#else
    STBM_UNUSED(timeBaseIndex);
#endif
    return index;
}

/**
 * @brief       Find the index of the time base according to it's ID.
 * @param[in]   timeBaseId  Identification of a Time Base
 * @return      uint16
 * @retval      The time base index in the configuration
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71833
 */
STBM_LOCAL uint16 StbM_FindSynchronizedTimeBase(StbM_SynchronizedTimeBaseType timeBaseId)
{
    uint16                                  timeBaseIndex  = 0u;
    const StbM_SynchronizedTimeBaseCfgType* timeBaseCfgPtr = StbM_ConfigPtr->SynchronizedTimeBase;
    for (; timeBaseIndex < STBM_TIME_BASE_NUM; ++timeBaseIndex)
    {
        if (timeBaseId == timeBaseCfgPtr[timeBaseIndex].SynchronizedTimeBaseIdentifier)
        {
            break;
        }
    }
    return timeBaseIndex;
}

/**
 * @brief       initialize the time base refer by index.
 * @param[in]   index  index of a Time Base
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       -
 */
STBM_LOCAL void StbM_TimeBaseInit(uint16 index)
{
    StbM_TimeBaseType* timeBasePtr = StbM_TimeBasePtr[index];
    /* SWS_StbM_00170 */
    timeBasePtr->TimeBaseStatus = 0x00u;
    /* SWS_StbM_00345 */
#if STBM_STATUS_NOTIFICATIONS_ENABLED == STD_ON
    timeBasePtr->NotificationEvents = 0u;
#endif
    /* SWS_StbM_00344 */
    timeBasePtr->TimeBaseUpdateCounter = 0u;
    /* SWS_StbM_00427 */
    timeBasePtr->UserData.userDataLength = 0u;
    timeBasePtr->UserData.userByte0      = 0u;
    timeBasePtr->UserData.userByte1      = 0u;
    timeBasePtr->UserData.userByte2      = 0u;
    /* Initialize */
    timeBasePtr->TimeLeapValid                    = FALSE;
    timeBasePtr->BusNotFirstSet                   = FALSE;
    timeBasePtr->LastBusSettime                   = 0ULL;
    timeBasePtr->MainTimeTuple.GlobalTime         = 0ULL;
    timeBasePtr->MainTimeTuple.VirtualLocalTime   = 0ULL;
    timeBasePtr->TimeLeap                         = 0;
    timeBasePtr->LocalClockTime.PreHwCounter      = 0ULL;
    timeBasePtr->LocalClockTime.LocalTimeFraction = 0ULL;
    timeBasePtr->LocalClockTime.LocalTime         = 0ULL;
    timeBasePtr->ClearTimeleapCount               = 0u;

/* SWS_StbM_00306 */
#if (STBM_TIME_RECORDING_SUPPORT == STD_ON)
    timeBasePtr->RecordBlockIndex    = 0u;
    timeBasePtr->RecordBlockCount    = 0u;
    timeBasePtr->SyncTimeRecordBlock = StbM_SyncTimeRecordBlockPtr[index];
#endif
    const StbM_SynchronizedTimeBaseCfgType* timeBaseCfgPtr = &TIMEBASE_CFG(index);

#if STBM_GPT_TIMESTAMP_SUPPORT == STD_ON
    const StbM_LocalTimeClockCfgType* localClockCfg = timeBaseCfgPtr->LocalTimeClock;
    if (localClockCfg->LocalTimeHardwareType == GPT_CHANNEL_CONFIGURATION)
    {
        Gpt_StartTimer(localClockCfg->LocalTimeHardware, localClockCfg->LocalTimeMax);
    }
#endif

    timeBasePtr->RateDeviation.IsSet    = FALSE;
    timeBasePtr->RateDeviation.RrcValue = STBM_RATE_PPM;
    timeBasePtr->RateDeviation.RocValue = 0;
#if (STBM_TIME_CORRECTION_SUPPORT == STD_ON)
    const StbM_TimeCorrectionCfgType* timeCorrectionCfgPtr = timeBaseCfgPtr->TimeCorrection;
    if (NULL_PTR != timeCorrectionCfgPtr)
    {
        timeBasePtr->RateCorrection.Array      = StbM_RateCorrectionPtr[index];
        timeBasePtr->RateCorrection.StartIndex = 0UL;
        timeBasePtr->RateCorrection.StopIndex  = 0UL;
        for (uint16 i = 0U; i <= timeCorrectionCfgPtr->RateCorrectionsPerMeasurementDuration; i++)
        {
            timeBasePtr->RateCorrection.Array[i].TGstart = 0ULL;
            timeBasePtr->RateCorrection.Array[i].TVstart = 0ULL;
        }
    }
#endif
}

#if (STBM_NOTIFICATION_CUSTOMER_NUM > 0u)
/**
 * @brief       initialize the notification customers.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       -
 */
STBM_LOCAL void StbM_NotificationCustomerInit(void)
{
    for (uint16 index = 0u; index < STBM_NOTIFICATION_CUSTOMER_NUM; index++)
    {
        uint16 timeBaseIndex = StbM_FindSynchronizedTimeBase(StbM_NotificationCustomer[index].TimeBaseId);
#if STBM_MULTIPLE_PARTITION_ENABLED == STD_ON
        if (GetApplicationID() != TIMEBASE_CFG(timeBaseIndex).ApplicationId)
        {
            continue; /* PRQA S 0770 */ /* VL_StbM_0770 */
        }
#endif
        StbM_NotificationType* notificationPtr = StbM_NotificationPtr[index];
        notificationPtr->IsSet                 = FALSE;
        notificationPtr->GPTtimercalled        = FALSE;
        notificationPtr->Isrunning             = FALSE;
    }
}
#endif

#if (STBM_TRIGGERED_CUSTOMER_NUM > 0u)
/**
 * @brief       initialize the triigger customers.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       -
 */
STBM_LOCAL void StbM_TriiggerCustomerInit(void)
{
    for (uint16 index = 0u; index < STBM_TRIGGERED_CUSTOMER_NUM; index++)
    {
        uint16 timeBaseIndex = TRIIGGERCUSTOMER_CFG(index).SynchronizedTimeBaseRef;
#if STBM_MULTIPLE_PARTITION_ENABLED == STD_ON
        if (GetApplicationID() != TIMEBASE_CFG(timeBaseIndex).ApplicationId)
        {
            continue; /* PRQA S 0770 */ /* VL_StbM_0770 */
        }
#endif
        StbM_TriggerRemainPeriod[index] = TRIIGGERCUSTOMER_CFG(index).TriggeredCustomerPeriod;
    }
}
#endif

/**
 * @brief       Get the local time
 * @param[in]   localClockCfg  The local hardware clock
 * @param[in]   localClockTime Used to store intermediate variables of clock time
 * @param[out]  timeStamp      The local time stamp
 * @return      Std_ReturnType
 * @retval      E_OK           Successfully get the local time
 * @retval      E_NOT_OK       Getting the local time failed
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-71835
 */
STBM_LOCAL Std_ReturnType StbM_LocalClockToLocalTime(
    const StbM_LocalTimeClockCfgType* localClockCfg,
    StbM_LocalClockTimeType*          localClockTime,
    uint64*                           timeStamp)
{
    Std_ReturnType     ret          = E_NOT_OK;
    StbM_HwCounterType curHwCounter = 0u;

    SchM_Enter_StbM_Context();
    switch (localClockCfg->LocalTimeHardwareType)
    {
#if STBM_ETHIF_TIMESTAMP_SUPPORT == STD_ON
    /* SWS_StbM_00178 */
    case ETHTSYN_GLOBALTIME_DOMAIN:
    {
        uint8         ctrlIdx = localClockCfg->LocalTimeHardware;
        TimeTupleType ethTimeTuple;
        if ((EthIf_GetCurrentTimeTuple(ctrlIdx, 0u, &ethTimeTuple) == E_OK) && (ethTimeTuple.timeQuality == TSQ_VALID))
        {
            StbM_TimeStampType stbmTimeStamp = {0};
            stbmTimeStamp.nanoseconds        = ethTimeTuple.timestampClockValue.nanoseconds;
            stbmTimeStamp.seconds            = ethTimeTuple.timestampClockValue.seconds;
            stbmTimeStamp.secondsHi          = ethTimeTuple.timestampClockValue.secondsHi;

            *timeStamp = StbM_GTTimeTo64Time(stbmTimeStamp);
            ret        = E_OK;
        }
        break;
    }
#endif

#if STBM_GPT_TIMESTAMP_SUPPORT == STD_ON
    /* SWS_StbM_00512 */
    case GPT_CHANNEL_CONFIGURATION:
    {
        Gpt_ChannelType channel = (Gpt_ChannelType)localClockCfg->LocalTimeHardware;
        Gpt_ValueType   value   = Gpt_GetTimeElapsed(channel);
        curHwCounter            = (StbM_HwCounterType)value;
        ret                     = E_OK;
        break;
    }
#endif

#if STBM_OS_TIMESTAMP_SUPPORT == STD_ON
    case OS_COUNTER:
    {
        CounterType counterID = (CounterType)localClockCfg->LocalTimeHardware;
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

    /* SWS_StbM_00352 */
    if ((ret == E_OK) && (localClockCfg->LocalTimeHardwareType != ETHTSYN_GLOBALTIME_DOMAIN))
    {
        StbM_HwCounterType preHwCounter     = localClockTime->PreHwCounter;
        StbM_HwCounterType elapsedHwCounter = (preHwCounter <= curHwCounter)
                                                  ? (curHwCounter - preHwCounter)
                                                  : (localClockCfg->LocalTimeMax + 1ULL + curHwCounter - preHwCounter);

        localClockTime->PreHwCounter = curHwCounter;

        StbM_LocalTimeType localTimeFraction = localClockTime->LocalTimeFraction;
        /* PRQA S 1252, 2834 ++ */ /* VL_StbM_1252, VL_StbM_2834 */
        localTimeFraction +=
            (elapsedHwCounter * STBM_NS_PER_S * localClockCfg->ClockPrescaler) % localClockCfg->ClockFrequency;
        /* PRQA S 1252, 2834 -- */

        StbM_LocalTimeType localTime = localClockTime->LocalTime;
        if (localTimeFraction >= localClockCfg->ClockFrequency)
        {
            localTimeFraction -= localClockCfg->ClockFrequency;
            ++localTime;
        }
        localClockTime->LocalTimeFraction = localTimeFraction;

        /* PRQA S 1252, 2834 ++ */ /* VL_StbM_1252, VL_StbM_2834 */
        localTime += (elapsedHwCounter * STBM_NS_PER_S * localClockCfg->ClockPrescaler) / localClockCfg->ClockFrequency;
        /* PRQA S 1252, 2834 -- */

        localClockTime->LocalTime = localTime;
        *timeStamp                = localTime;
    }
    else if ((localClockCfg->LocalTimeHardwareType == ETHTSYN_GLOBALTIME_DOMAIN) && (ret == E_OK))
    {
        localClockTime->LocalTime = *timeStamp;
    }
    else
    {
        /* do nothing */
    }
    SchM_Exit_StbM_Context();

    return ret;
}

/**
 * @brief       Get the current local time
 * @param[in]   timeBaseIndex  The time base index in the configuration
 * @param[out]  localTime      The current local time
 * @return      Std_ReturnType
 * @retval      E_OK           Successfully get the current local time
 * @retval      E_NOT_OK       Getting the current local time failed
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-71836
 */
STBM_LOCAL Std_ReturnType StbM_GetCurrentLocalTime(uint16 timeBaseIndex, StbM_LocalTimeType* localTime)
{
    Std_ReturnType                    stdReturn     = E_NOT_OK;
    const StbM_LocalTimeClockCfgType* localClockCfg = TIMEBASE_CFG(timeBaseIndex).LocalTimeClock;
    if (localClockCfg != NULL_PTR)
    {
        StbM_LocalClockTimeType* localClockTime = &StbM_TimeBasePtr[timeBaseIndex]->LocalClockTime;
        stdReturn                               = StbM_LocalClockToLocalTime(localClockCfg, localClockTime, localTime);
    }
    return stdReturn;
}

/**
 * @brief         Get the current local time
 * @param[in]     timeBaseIndex  The time base index in the configuration
 * @param[in,out] timeStamp      Input the local time and get the global time
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-71837
 */
STBM_LOCAL void StbM_GetGlobalTime64(uint16 timeBaseIndex, uint64* timeStamp)
{
    uint64 TV = *timeStamp;

    StbM_TimeBaseType* timeBasePtr = StbM_TimeBasePtr[timeBaseIndex];
    SchM_Enter_StbM_Context();
    uint64 diff = TV - timeBasePtr->MainTimeTuple.VirtualLocalTime;

    if (TIMEBASE_CFG(timeBaseIndex).TimeCorrection == NULL_PTR)
    {
        *timeStamp = timeBasePtr->MainTimeTuple.GlobalTime + diff;
    }
    else
    {
        /* SWS_StbM_00353 */
        uint64  interval = TIMEBASE_CFG(timeBaseIndex).TimeCorrection->OffsetCorrectionAdaptionInterval;
        float64 rc =
            (float64)(((float64)timeBasePtr->RateDeviation.RrcValue + (float64)timeBasePtr->RateDeviation.RocValue)
                      / (float64)STBM_RATE_PPM);

        /* when the Offset Correction Adaption Interval expires */
        /* PRQA S 4119, 4395 ++ */ /* VL_StbM_4119, VL_StbM_4395 */
        if ((timeBasePtr->RateDeviation.RocValue != 0L) && (diff >= interval))
        {
            timeBasePtr->MainTimeTuple.VirtualLocalTime += interval;
            timeBasePtr->MainTimeTuple.GlobalTime += (uint64)((float64)interval * rc);
            timeBasePtr->RateDeviation.RocValue = 0;
            diff                                = diff - interval;
            *timeStamp =
                timeBasePtr->MainTimeTuple.GlobalTime
                + (uint64)(((float64)timeBasePtr->RateDeviation.RrcValue / (float64)STBM_RATE_PPM) * (float64)diff);
        }
        else
        {
            *timeStamp = timeBasePtr->MainTimeTuple.GlobalTime + (uint64)(rc * (float64)diff);
        }
        /* PRQA S 4119, 4395 -- */
    }

    SchM_Exit_StbM_Context();
}

/**
 * @brief       Update the time tuple of the specified time base.
 * @param[in]   timeBaseIndex  The time base index in the configuration
 * @param[in]   TL  Local Instance of the Global Time
 * @param[in]   TV  Virtual Local Time
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71838
 */
STBM_LOCAL void StbM_UpdateMainTimeTuple(uint16 timeBaseIndex, uint64 TL, uint64 TV)
{
    StbM_MainTimeTupleType* mainTimeTuplePtr = &StbM_TimeBasePtr[timeBaseIndex]->MainTimeTuple;
    SchM_Enter_StbM_Context();
    mainTimeTuplePtr->VirtualLocalTime = TV;
    mainTimeTuplePtr->GlobalTime       = TL;
    SchM_Exit_StbM_Context();
}

/**
 * @brief       Convert StbM_TimeStampType time value to uint64 type.
 * @param[in]   timeStamp  The time value to be converted
 * @return      uint64
 * @retval      64-bit time value
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71839
 */
STBM_LOCAL uint64 StbM_GTTimeTo64Time(StbM_TimeStampType timeStamp)
{
    uint64 time64 = StbM_Shift_Left32((uint64)timeStamp.secondsHi) + (uint64)timeStamp.seconds;
    time64 *= STBM_NS_PER_S; /* PRQA S 1252 */ /* VL_StbM_1252 */
    time64 += timeStamp.nanoseconds;
    return time64;
}

/**
 * @brief       Convert uint64 time value to StbM_TimeStampType type.
 * @param[in]   time64  The time value to be converted
 * @return      StbM_TimeStampType
 * @retval      StbM_TimeStampType time value
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71840
 */
STBM_LOCAL StbM_TimeStampType StbM_64TimeToGTTime(uint64 time64)
{
    StbM_TimeStampType timeStamp = {0};
    timeStamp.nanoseconds        = (uint32)(time64 % STBM_NS_PER_S); /* PRQA S 1252 */ /* VL_StbM_1252 */
    uint64 t                     = time64 / STBM_NS_PER_S; /* PRQA S 1252 */           /* VL_StbM_1252 */
    timeStamp.secondsHi          = (uint16)StbM_Shift_Right32(t);
    timeStamp.seconds            = (uint32)(t);
    return timeStamp;
}

/**
 * @brief       Convert StbM_VirtualLocalTimeType time value to uint64 type.
 * @param[in]   localTime  The time value to be converted
 * @return      uint64
 * @retval      64-bit time value
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71841
 */
STBM_LOCAL uint64 StbM_LTTimeTo64Time(StbM_VirtualLocalTimeType localTime)
{
    uint64 time64 = StbM_Shift_Left32((uint64)localTime.nanosecondsHi);
    time64 |= (uint64)localTime.nanosecondsLo;
    return time64;
}

/**
 * @brief       Calculate the difference between local time and global time.
 * @param[in]   timeBaseIndex The time base index in the configuration
 * @param[in]   TL_Sync       Global Time part of the Synclocal Time Tuple
 * @param[in]   TG_URx        Global Time part of the Updated Rx Time Tuple
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-71842
 */
STBM_LOCAL void StbM_CheckTimeleap(uint16 timeBaseIndex, uint64 TL_Sync, uint64 TG_URx)
{
    StbM_TimeBaseType* timeBasePtr = StbM_TimeBasePtr[timeBaseIndex];
    SchM_Enter_StbM_Context();
    if (timeBasePtr->BusNotFirstSet == TRUE)
    {
        uint8 leapState =
            (TL_Sync > TG_URx) ? STBM_TIMEBASE_STATUS_TIMELEAP_PAST : STBM_TIMEBASE_STATUS_TIMELEAP_FUTURE;
        uint64 leapThreshold = (TL_Sync > TG_URx) ? (TIMEBASE_CFG(timeBaseIndex).TimeLeapPastThreshold)
                                                  : (TIMEBASE_CFG(timeBaseIndex).TimeLeapFutureThreshold);
        uint64 timeLeap      = (TL_Sync > TG_URx) ? (TL_Sync - TG_URx) : (TG_URx - TL_Sync);
        /* SWS_StbM_00182 SWS_StbM_00305 */
        if (leapThreshold != 0ULL)
        {
            if (timeLeap > leapThreshold)
            {
                timeBasePtr->TimeBaseStatus |= leapState;
                timeBasePtr->ClearTimeleapCount = TIMEBASE_CFG(timeBaseIndex).ClearTimeleapCount;
            }
            else
            {
                if (timeBasePtr->ClearTimeleapCount > 1u)
                {
                    timeBasePtr->ClearTimeleapCount--;
                }
                else
                {
                    timeBasePtr->TimeBaseStatus &=
                        (StbM_TimeBaseStatusType)(~(StbM_TimeBaseStatusType)STBM_TIMEBASE_STATUS_TIMELEAP_PAST);
                    timeBasePtr->TimeBaseStatus &=
                        (StbM_TimeBaseStatusType)(~(StbM_TimeBaseStatusType)STBM_TIMEBASE_STATUS_TIMELEAP_FUTURE);
                }
            }
        }

        /* SWS_StbM_00425 */
        if (timeLeap > (uint64)StbM_TimeDiffType_UpperLimit) /* PRQA S 1259 */ /* VL_StbM_1259 */
        {
            timeLeap = (uint64)StbM_TimeDiffType_UpperLimit; /* PRQA S 1259 */ /* VL_StbM_1259 */
        }
        timeBasePtr->TimeLeap = (leapState == STBM_TIMEBASE_STATUS_TIMELEAP_FUTURE) ? (StbM_TimeDiffType)timeLeap
                                                                                    : -(StbM_TimeDiffType)timeLeap;

        timeBasePtr->TimeLeapValid = TRUE;
    }
    SchM_Exit_StbM_Context();
}

/**
 * @brief       Check if there has been a timeout in time synchronization.
 * @param[in]   timeBaseIndex The time base index in the configuration
 * @param[in]   TV            Virtual Local Time
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-71843
 */
STBM_LOCAL void StbM_CheckTimeout(uint16 timeBaseIndex, uint64 TV)
{
    const StbM_SynchronizedTimeBaseCfgType* timeBaseCfgPtr = &TIMEBASE_CFG(timeBaseIndex);

    /* SWS_StbM_00183 */
    if ((!timeBaseCfgPtr->IsSystemWideGlobalTimeMaster) && (timeBaseCfgPtr->SyncLossTimeout != 0ULL))
    {
        StbM_TimeBaseType* timeBasePtr = StbM_TimeBasePtr[timeBaseIndex];

        SchM_Enter_StbM_Context();
#if STBM_STATUS_NOTIFICATIONS_ENABLED == STD_ON
        StbM_TimeBaseStatusType oldTimeBaseStatus = timeBasePtr->TimeBaseStatus;
#endif
        if (timeBasePtr->BusNotFirstSet == TRUE)
        {
            if ((TV - timeBasePtr->LastBusSettime) > timeBaseCfgPtr->SyncLossTimeout)
            {
                timeBasePtr->TimeBaseStatus |= STBM_TIMEBASE_STATUS_TIMEOUT;
                /* SWS_StbM_00187 */
                if (timeBaseCfgPtr->IsTimeGateway)
                {
                    timeBasePtr->TimeBaseStatus |= STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY;
                }
            }
        }
        SchM_Exit_StbM_Context();
#if STBM_STATUS_NOTIFICATIONS_ENABLED == STD_ON
        StbM_OnChangeTimeBaseStatus(&timeBasePtr->NotificationEvents, oldTimeBaseStatus, timeBasePtr->TimeBaseStatus);
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
    const StbM_TimeCorrectionCfgType* cfgPtr      = TIMEBASE_CFG(timeBaseIndex).TimeCorrection;
    StbM_TimeBaseType*                timeBasePtr = StbM_TimeBasePtr[timeBaseIndex];

    const StbM_RateCorrectionType* const rataCorrection = &timeBasePtr->RateCorrection;

    if ((cfgPtr == NULL_PTR) || (cfgPtr->RateCorrectionMeasurementDuration == 0u))
    {
        return;
    }

    uint16 num = cfgPtr->RateCorrectionsPerMeasurementDuration;

    SchM_Enter_StbM_Context();
    if (((timeBasePtr->TimeBaseStatus & (StbM_TimeBaseStatusType)STBM_TIMEBASE_STATUS_TIMELEAP_FUTURE) != 0U)
        || ((timeBasePtr->TimeBaseStatus & (StbM_TimeBaseStatusType)STBM_TIMEBASE_STATUS_TIMELEAP_PAST) != 0U))
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

    if (((timeBaseStatus & STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY)
         != (timeBasePtr->TimeBaseStatus & STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY))
        || ((timeBasePtr->TimeBaseStatus & (StbM_TimeBaseStatusType)STBM_TIMEBASE_STATUS_TIMEOUT) != 0U))
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
        /* PRQA S 4119, 4395 ++ */ /* VL_StbM_4119, VL_StbM_4395 */
        sint32 rrcValue = (sint32)(((float64)TGdiff / (float64)TVdiff * (float64)STBM_RATE_PPM) + STBM_ROUND_VALUE);
        /* PRQA S 4119, 4395 -- */
        timeBasePtr->RateCorrection.StartIndex++;

        if ((cfgPtr->RateCorrectionThreshold > 0U)
            && ((rrcValue > (STBM_RATE_PPM + (sint32)cfgPtr->RateCorrectionThreshold))
                || (rrcValue < (STBM_RATE_PPM - (sint32)cfgPtr->RateCorrectionThreshold))))
        {
            timeBasePtr->TimeBaseStatus |= (StbM_TimeBaseStatusType)STBM_TIMEBASE_STATUS_RATE_EXCEEDED;
        }
        else
        {
            timeBasePtr->RateDeviation.RrcValue = rrcValue;
            timeBasePtr->RateDeviation.IsSet    = TRUE;
            timeBasePtr->TimeBaseStatus &=
                (StbM_TimeBaseStatusType)(~(StbM_TimeBaseStatusType)STBM_TIMEBASE_STATUS_RATE_EXCEEDED);
            timeBasePtr->TimeBaseStatus |= (StbM_TimeBaseStatusType)STBM_TIMEBASE_STATUS_RATE_CORRECTED;
        }
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
    const StbM_TimeCorrectionCfgType* cfgPtr      = TIMEBASE_CFG(timeBaseIndex).TimeCorrection;
    StbM_TimeBaseType*                timeBasePtr = StbM_TimeBasePtr[timeBaseIndex];

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
        /* PRQA S 4119, 4395 ++ */ /* VL_StbM_4119, VL_StbM_4395 */
        float64 temp = (float64)STBM_RATE_PPM * (float64)Odiff / (float64)interval;
        if (temp > 0.0)
        {
            timeBasePtr->RateDeviation.RocValue = (sint32)(temp + STBM_ROUND_VALUE);
        }
        else
        {
            timeBasePtr->RateDeviation.RocValue = (sint32)(temp - STBM_ROUND_VALUE);
        }
        /* PRQA S 4119, 4395 -- */
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
 * @brief       Calculate status events based on the status of the time base.
 * @param[out]  notificationEvents  The notification status events
 * @param[in]   oldTimeBaseStatus   The previous time base status
 * @param[in]   newTimeBaseStatus   The current time base status
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-71847
 */
STBM_LOCAL void StbM_OnChangeTimeBaseStatus(
    StbM_TimeBaseNotificationType* notificationEvents,
    StbM_TimeBaseStatusType        oldTimeBaseStatus,
    StbM_TimeBaseStatusType        newTimeBaseStatus)
{
    StbM_TimeBaseStatusType       chgTimeBaseStatus = oldTimeBaseStatus ^ newTimeBaseStatus;
    StbM_TimeBaseNotificationType eventNotification = 0u;

    if (chgTimeBaseStatus == 0u)
    {
        return;
    }

    if ((chgTimeBaseStatus & STBM_TIMEBASE_STATUS_GLOBAL_TIME_BASE) != 0u)
    {
        eventNotification |= STBM_TIMEBASE_EV_GLOBAL_TIME;
    }

    if ((chgTimeBaseStatus & STBM_TIMEBASE_STATUS_TIMEOUT) != 0u)
    {
        eventNotification |= ((newTimeBaseStatus & STBM_TIMEBASE_STATUS_TIMEOUT) != 0u)
                                 ? STBM_TIMEBASE_EV_TIMEOUT_OCCURRED
                                 : STBM_TIMEBASE_EV_TIMEOUT_REMOVED;
    }

    if ((chgTimeBaseStatus & STBM_TIMEBASE_STATUS_TIMELEAP_FUTURE) != 0u)
    {
        eventNotification |= ((newTimeBaseStatus & STBM_TIMEBASE_STATUS_TIMELEAP_FUTURE) != 0u)
                                 ? STBM_TIMEBASE_EV_TIMELEAP_FUTURE
                                 : STBM_TIMEBASE_EV_TIMELEAP_FUTURE_REMOVED;
    }

    if ((chgTimeBaseStatus & STBM_TIMEBASE_STATUS_TIMELEAP_PAST) != 0u)
    {
        eventNotification |= ((newTimeBaseStatus & STBM_TIMEBASE_STATUS_TIMELEAP_PAST) != 0u)
                                 ? STBM_TIMEBASE_EV_TIMELEAP_PAST
                                 : STBM_TIMEBASE_EV_TIMELEAP_PAST_REMOVED;
    }

    if ((chgTimeBaseStatus & STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY) != 0u)
    {
        eventNotification |= ((newTimeBaseStatus & STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY) != 0u)
                                 ? STBM_TIMEBASE_EV_SYNC_TO_SUBDOMAIN
                                 : STBM_TIMEBASE_EV_SYNC_TO_GLOBAL_MASTER;
    }

    SchM_Enter_StbM_Context();
    *notificationEvents |= eventNotification;
    SchM_Exit_StbM_Context();
}

/**
 * @brief       Check the notification event and take the appropriate action.
 * @param[in]   timeBaseIndex The time base index in the configuration
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-71848
 */
STBM_LOCAL void StbM_CheckStatusNotification(uint16 timeBaseIndex)
{
    const StbM_SynchronizedTimeBaseCfgType* timeBaseCfg = &TIMEBASE_CFG(timeBaseIndex);

    StbM_TimeBaseNotificationType* notificationEvents = &StbM_TimeBasePtr[timeBaseIndex]->NotificationEvents;
    StbM_TimeBaseNotificationType  eventNotification  = *notificationEvents & timeBaseCfg->StatusNotificationMask;
    if (eventNotification != 0u)
    {
        /* SWS_StbM_00277 */
        if (timeBaseCfg->StatusNotificationCallback != NULL_PTR)
        {
            timeBaseCfg->StatusNotificationCallback(eventNotification);
        }

        if ((timeBaseCfg->NotificationInterface == SR_INTERFACE)
            || (timeBaseCfg->NotificationInterface == CALLBACK_AND_SR_INTERFACE))
        {
            /* notify the APP via StatusNotification Interface SWS_StbM_00282 */
        }
    }
    *notificationEvents = 0u;
}
#endif

#if (STBM_TIME_RECORDING_SUPPORT == STD_ON)
/**
 * @brief       Record time information.
 * @param[in]   timeBaseIndex The time base index in the configuration
 * @param[in]   TLsync        The Local Time Base directly before synchronization with the Global Time Base
 * @param[in]   TGrx          The Local Time Base directly after synchronization with the Global Time Base
 * @param[in]   TVrx          Virtual Local Time
 * @param[in]   pathDelay     Current propagation delay in nanoseconds
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-71849
 */
STBM_LOCAL void StbM_TimeRecord(uint16 timeBaseIndex, uint64 TLsync, uint64 TGrx, uint64 TVrx, uint32 pathDelay)
{
    const StbM_SynchronizedTimeBaseCfgType* timeBaseCfgPtr = &TIMEBASE_CFG(timeBaseIndex);
    if (timeBaseCfgPtr->TimeRecording != NULL_PTR)
    {
        StbM_TimeBaseType* timeBasePtr = StbM_TimeBasePtr[timeBaseIndex];

        SchM_Enter_StbM_Context();
        uint16 recordBlockIndex = timeBasePtr->RecordBlockIndex;
        if (timeBaseCfgPtr->TimeRecording->SyncTimeRecordTableBlockCount > 0U)
        {
            StbM_SyncRecordTableBlockType* syncRecordTableBlock = &timeBasePtr->SyncTimeRecordBlock[recordBlockIndex];
            syncRecordTableBlock->GlbNanoSeconds                = (uint32)(TGrx % STBM_NS_PER_S);
            syncRecordTableBlock->GlbSeconds                    = (uint32)(TGrx / STBM_NS_PER_S);
            syncRecordTableBlock->LocNanoSeconds                = (uint32)(TLsync % STBM_NS_PER_S);
            syncRecordTableBlock->LocSeconds                    = (uint32)(TLsync / STBM_NS_PER_S);
            syncRecordTableBlock->PathDelay                     = pathDelay;
#if (STBM_TIME_CORRECTION_SUPPORT == STD_ON)
            syncRecordTableBlock->RateDeviation =
                (StbM_RateDeviationType)(timeBasePtr->RateDeviation.RrcValue - (sint32)STBM_RATE_PPM);
#else
            syncRecordTableBlock->RateDeviation = 0;
#endif
            syncRecordTableBlock->TimeBaseStatus      = timeBasePtr->TimeBaseStatus;
            syncRecordTableBlock->VirtualLocalTimeLow = (uint32)TVrx;
            timeBasePtr->RecordBlockIndex =
                (recordBlockIndex + 1u) % timeBaseCfgPtr->TimeRecording->SyncTimeRecordTableBlockCount;
            if (timeBasePtr->RecordBlockCount < timeBaseCfgPtr->TimeRecording->SyncTimeRecordTableBlockCount)
            {
                timeBasePtr->RecordBlockCount++;
            }
        }
        SchM_Exit_StbM_Context();
    }
}

/**
 * @brief       Name of the customer specific callback function, which shall be called, if a
 *              measurement data for a Synchronized Time Base are available.
 * @param[in]   timeBaseIndex The time base index in the configuration
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-71850
 */
STBM_LOCAL void StbM_TimeRecordback(uint16 timeBaseIndex)
{
    const StbM_TimeRecordingCfgType* timeRecordingCfgPtr = TIMEBASE_CFG(timeBaseIndex).TimeRecording;
    if (timeRecordingCfgPtr != NULL_PTR)
    {
        if (timeRecordingCfgPtr->SyncTimeRecordTableBlockCount > 0u)
        {
            StbM_TimeBaseType* timeBasePtr = StbM_TimeBasePtr[timeBaseIndex];
            SchM_Enter_StbM_Context();
            uint16 curIndex = (timeBasePtr->RecordBlockIndex >= timeBasePtr->RecordBlockCount)
                                  ? (timeBasePtr->RecordBlockIndex - timeBasePtr->RecordBlockCount)
                                  : (timeRecordingCfgPtr->SyncTimeRecordTableBlockCount - timeBasePtr->RecordBlockCount
                                     + timeBasePtr->RecordBlockIndex);
            while (timeBasePtr->RecordBlockCount > 0u)
            {
                timeRecordingCfgPtr->SyncTimeRecordBlockCallback(&timeBasePtr->SyncTimeRecordBlock[curIndex]);
                curIndex =
                    (curIndex == (timeRecordingCfgPtr->SyncTimeRecordTableBlockCount - 1u)) ? 0u : (curIndex + 1u);
                timeBasePtr->RecordBlockCount--;
            }
            SchM_Exit_StbM_Context();
        }
    }
}
#endif

/**
 * @brief       Set the user data for a Synchronized time base.
 * @param[in]   timeBaseIndex The time base index in the configuration
 * @param[in]   userDataPtr   User data of the time base
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-71851
 */
STBM_LOCAL void StbM_SetUserDataIn(uint16 timeBaseIndex, const StbM_UserDataType* userDataPtr)
{
    if (userDataPtr != NULL_PTR)
    {
        StbM_TimeBaseType* timeBasePtr       = StbM_TimeBasePtr[timeBaseIndex];
        timeBasePtr->UserData.userDataLength = userDataPtr->userDataLength;
        if (userDataPtr->userDataLength > 0u)
        {
            timeBasePtr->UserData.userByte0 = userDataPtr->userByte0;
            if (userDataPtr->userDataLength > 1u)
            {
                timeBasePtr->UserData.userByte1 = userDataPtr->userByte1;
                if (userDataPtr->userDataLength > 2u) /* PRQA S 3120 */ /* VL_QAC_MagicNum */
                {
                    timeBasePtr->UserData.userByte2 = userDataPtr->userByte2;
                }
            }
        }
    }
}

/**
 * @brief        Set the Global Time information
 * @param[in]    timeBaseId      Identification of a Time Base
 * @param[in]    timeStamp       New time stamp
 * @param[in]    userData        New user data(if not NULL)
 * @param[in]    serviceId       Application ID
 * @param[in]    isUpdateCounter If update the Counter
 * @return       Std_ReturnType
 * @retval       E_OK       Successfully set the global time
 * @retval       E_NOT_OK   setting the global Time failed
 * @reentrant    FALSE
 * @synchronous  TRUE
 * @trace        CPD-71855
 */
/* PRQA S 6070 ++ */ /* VL_MTR_StbM_STCAL */
STBM_LOCAL Std_ReturnType StbM_SetGlobalTimePrivate(
    StbM_SynchronizedTimeBaseType timeBaseId,
    const StbM_TimeStampType*     timeStamp,
    const StbM_UserDataType*      userData,
    uint8                         serviceId,
    boolean                       isUpdateCounter)
/* PRQA S 6070 -- */
{
    Std_ReturnType ret           = E_NOT_OK;
    uint16         timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommom(serviceId, timeBaseId, timeBaseIndex) && StbM_ValidatePointer(serviceId, timeStamp)
        && StbM_ValidateNanoseconds(serviceId, timeStamp->nanoseconds) && StbM_ValidateUserData(serviceId, userData)
        && StbM_ValidatePartition(serviceId, timeBaseIndex))
#endif
    {
        uint64 TV = 0u;

        /* SWS_StbM_00342 SWS_StbM_00436 */
        ret = StbM_GetCurrentLocalTime(timeBaseIndex, &TV);
        if (ret == E_OK)
        {
            uint64 TL = StbM_GTTimeTo64Time(*timeStamp);
            StbM_UpdateMainTimeTuple(timeBaseIndex, TL, TV);

            StbM_TimeBaseType* timeBasePtr = StbM_TimeBasePtr[timeBaseIndex];

            /* SWS_StbM_00571 SWS_StbM_00181 */
#if STBM_STATUS_NOTIFICATIONS_ENABLED == STD_ON
            SchM_Enter_StbM_Context();
            StbM_TimeBaseStatusType oldTimeBaseStatus = timeBasePtr->TimeBaseStatus;
            timeBasePtr->TimeBaseStatus |= STBM_TIMEBASE_STATUS_GLOBAL_TIME_BASE;
            SchM_Exit_StbM_Context();
            StbM_OnChangeTimeBaseStatus(
                &timeBasePtr->NotificationEvents,
                oldTimeBaseStatus,
                timeBasePtr->TimeBaseStatus);
            SchM_Enter_StbM_Context();
            timeBasePtr->NotificationEvents |= STBM_TIMEBASE_EV_RESYNC;
#else
            SchM_Enter_StbM_Context();
            timeBasePtr->TimeBaseStatus |= STBM_TIMEBASE_STATUS_GLOBAL_TIME_BASE;
#endif
            StbM_SetUserDataIn(timeBaseIndex, userData);
            if (isUpdateCounter)
            {
                ++timeBasePtr->TimeBaseUpdateCounter;
            }
            SchM_Exit_StbM_Context();
        }
    }

    return ret;
}

#if (STBM_NOTIFICATION_CUSTOMER_NUM > 0)
/**
 * @brief       Notify when the customer's time is up.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-71869
 */
STBM_LOCAL void StbM_TimerNotificationCustomer(void)
{
    for (uint16 index = 0u; index < STBM_NOTIFICATION_CUSTOMER_NUM; index++)
    {
        uint16 timeBaseIndex = StbM_FindSynchronizedTimeBase(StbM_NotificationCustomer[index].TimeBaseId);

#if STBM_MULTIPLE_PARTITION_ENABLED == STD_ON
        if (GetApplicationID() != TIMEBASE_CFG(timeBaseIndex).ApplicationId)
        {
            continue; /* PRQA S 0770 */ /* VL_StbM_0770 */
        }
#endif
        StbM_NotificationType* notificationPtr = StbM_NotificationPtr[index];

        if (notificationPtr->IsSet)
        {
            uint64 TL;
            if (StbM_GetCurrentLocalTime(timeBaseIndex, &TL) == E_OK)
            {
                StbM_GetGlobalTime64(timeBaseIndex, &TL);
                sint64 timeDiff = (sint64)notificationPtr->CustomerTimerExpireTime - (sint64)TL;
                if (timeDiff <= 0)
                {
                    timeDiff = (timeDiff < StbM_TimeDiffType_LowerLimit) ? StbM_TimeDiffType_LowerLimit : timeDiff;
                    const StbMTimeNtfCallbackType timeNtfCallback =
                        StbM_NotificationCustomer[index].TimeNotificationCallback;
                    if (NULL_PTR != timeNtfCallback)
                    {
                        timeNtfCallback((StbM_TimeDiffType)timeDiff);
                    }
                    SchM_Enter_StbM_Context();
                    notificationPtr->IsSet          = FALSE;
                    notificationPtr->Isrunning      = FALSE;
                    notificationPtr->GPTtimercalled = FALSE;
                    SchM_Exit_StbM_Context();
                }
                else if (timeDiff < (sint64)(STBM_TIMER_START_THRESHOLD))
                {
                    /* SWS_StbM_00336 */
                    uint32 tickDiff = (uint32)(StbM_GptTimer_Ns2us((uint64)timeDiff) / STBM_GPT_TIMER_TICK_DURATION);
                    Gpt_StartTimer(STBM_GPT_TIMER_REF, tickDiff);
                    SchM_Enter_StbM_Context();
                    notificationPtr->Isrunning      = TRUE;
                    notificationPtr->GPTtimercalled = TRUE;
                    SchM_Exit_StbM_Context();
                }
                else
                {
                    /*do nothing */
                }
            }
        }
    }
}
#endif

#if (STBM_TRIGGERED_CUSTOMER_NUM > 0u)
/**
 * @brief       The triggered customer is directly triggered by the Synchronized Time-base Manager
 *              by getting synchronized with the current (global) definition of time and passage of time.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-71871
 */
STBM_LOCAL void StbM_TriggeredCustomer(void)
{
    for (uint16 index = 0u; index < STBM_TRIGGERED_CUSTOMER_NUM; index++)
    {
        const StbM_TriggeredCustomerCfgType* triggerCustomerConfig = &TRIIGGERCUSTOMER_CFG(index);
#if STBM_MULTIPLE_PARTITION_ENABLED == STD_ON
        if (GetApplicationID() != TIMEBASE_CFG(triggerCustomerConfig->SynchronizedTimeBaseRef).ApplicationId)
        {
            continue; /* PRQA S 0770 */ /* VL_StbM_0770 */
        }
#endif
        --StbM_TriggerRemainPeriod[index];
        if (StbM_TriggerRemainPeriod[index] == 0u)
        {
            StbM_TriggerRemainPeriod[index] = triggerCustomerConfig->TriggeredCustomerPeriod;

            SchM_Enter_StbM_Context();
            StbM_TimeBaseStatusType status =
                StbM_TimeBasePtr[triggerCustomerConfig->SynchronizedTimeBaseRef]->TimeBaseStatus;
            SchM_Exit_StbM_Context();

            /* SWS_StbM_00077 */
            if (!((status & STBM_TIMEBASE_STATUS_GLOBAL_TIME_BASE) == STBM_TIMEBASE_STATUS_GLOBAL_TIME_BASE)
                || ((status & STBM_TIMEBASE_STATUS_TIMEOUT) == STBM_TIMEBASE_STATUS_TIMEOUT)
                || ((status & STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY) == STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY)
                || ((status & STBM_TIMEBASE_STATUS_TIMELEAP_FUTURE) == STBM_TIMEBASE_STATUS_TIMELEAP_FUTURE)
                || ((status & STBM_TIMEBASE_STATUS_TIMELEAP_PAST) == STBM_TIMEBASE_STATUS_TIMELEAP_PAST))
            {
                continue; /* PRQA S 0770 */ /* VL_StbM_0770 */
            }

            /* SWS_StbM_00092 */
            ScheduleTableStatusType scheduleTableStatus = SCHEDULETABLE_STOPPED;
            if (GetScheduleTableStatus(triggerCustomerConfig->OSScheduleTableRef, &scheduleTableStatus) != E_OK)
            {
                continue; /* PRQA S 0770 */ /* VL_StbM_0770 */
            }

            if ((scheduleTableStatus != SCHEDULETABLE_WAITING) && (scheduleTableStatus != SCHEDULETABLE_RUNNING)
                && (scheduleTableStatus != SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS))
            {
                continue; /* PRQA S 0770 */ /* VL_StbM_0770 */
            }

            /* SWS_StbM_00302 SWS_StbM_00303 */
            uint64 TL = 0u;
            if (StbM_GetCurrentLocalTime(triggerCustomerConfig->SynchronizedTimeBaseRef, &TL) != E_OK)
            {
                continue; /* PRQA S 0770 */ /* VL_StbM_0770 */
            }
            StbM_GetGlobalTime64(triggerCustomerConfig->SynchronizedTimeBaseRef, &TL);
            TL = (TL / 1000u) % triggerCustomerConfig->OsScheduleTableDuration;
            (void)SyncScheduleTable(triggerCustomerConfig->OSScheduleTableRef, (TickType)TL);
        }
    }
}
#endif

#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
/* PRQA S 1503, 2889 -- */
