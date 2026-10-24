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
#include "SchM_StbM.h"
#include "StbM_Internal.h"
#include "StbM.h"
#include "IStdLib.h"

#if STBM_DEV_ERROR_DETECT == STD_ON
#include "Det.h"
#endif

/* SWS_StbM_00065 */
#if STBM_TRIGGERED_CUSTOMER_NUM > 0
#include "Os.h"
#endif
/* SWS_StbM_00426 */
#if STBM_NOTIFICATION_CUSTOMER_NUM > 0
#include "Gpt.h"
#endif

/* SWS_StbM_00246 */
#if (STBM_ETHIF_TIMESTAMP_SUPPORT == STD_ON) || (STBM_DISCIPLINED_CLOCK_USED == STD_ON)
#include "EthIf.h"
#endif

/* SWS_StbM_00538 */
#if STBM_CANIF_TIMESTAMP_SUPPORT == STD_ON
#include "CanIf.h"
#endif

#if STBM_ETHTSYN_ENABLE == STD_ON
#include "EthTSyn.h"
#endif

#if STBM_GPT_TIMESTAMP_SUPPORT == STD_ON || STBM_OS_TIMESTAMP_SUPPORT == STD_ON
#include "StbM_Clock.h"
#endif

#if (STBM_CAN_SLAVE_TIME_VALIDATION_USED == STD_ON) || (STBM_CAN_MASTER_TIME_VALIDATION_USED == STD_ON)
#include "StbM_CanTSyn.h"
#endif

#if (STBM_ETH_SLAVE_TIME_VALIDATION_USED == STD_ON) || (STBM_ETH_MASTER_TIME_VALIDATION_USED == STD_ON)
#include "StbM_EthTSyn.h"
#endif

#if STBM_QUERY_FRESHNESS_VALUE != STBM_QUERY_FRESHNESS_VALUE_NONE
#include "StbM_Freshness.h"
#endif

#include "StbM_Arti.h"

/* ===================================================== macros ===================================================== */
#define TRIIGGERCUSTOMER_CFG(index) StbM_ConfigPtr->TriggeredCustomer[index]
#define TIMEBASE_CFG(index)         StbM_ConfigPtr->SynchronizedTimeBase[index]

#define STBM_SINT32_MIN       (-2147483648LL)
#define STBM_SINT32_MAX       (2147483647LL)
#define STBM_USERDATA_LEN_MAX (3U)
#define STBM_SHIFT_BIT32      (32U)
#define STBM_POW2_41          (sint64)(1LLU << 41U)

#define STBM_USER_DATA_BYTE_0 0U
#define STBM_USER_DATA_BYTE_1 1U
#define STBM_USER_DATA_BYTE_2 2U

#define STBM_ROUND_VALUE (0.5)

#if STBM_TIME_CORRECTION_SUPPORT == STD_ON
#define STBM_RATE_CORRECTION_INVALID_TIMEBASE_STATUS                                                          \
    (STBM_TIMEBASE_STATUS_TIMELEAP_FUTURE | STBM_TIMEBASE_STATUS_TIMELEAP_PAST | STBM_TIMEBASE_STATUS_TIMEOUT \
     | STBM_TIMEBASE_STATUS_TIME_PROGRESSION_INCONSISTENCY | STBM_TIMEBASE_STATUS_FALLBACK_TIME_EXTRAPOLATION)
#endif

#if (STBM_NOTIFICATION_CUSTOMER_NUM > 0)
/**
 * @brief       Convert nanosecond time to microsecond time.
 * @param[in]   ns  nanosecond time
 * @return      uint64
 * @retval      microsecond time
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace   CPD-74329
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
 * @trace   CPD-74330
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
 * @trace   CPD-74331
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
 * @trace   CPD-74332
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
 * @trace   CPD-74333
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

#if (STBM_DISCIPLINED_CLOCK_USED == STD_ON) && (STBM_TIME_CORRECTION_SUPPORT == STD_ON)
/**
 * @brief       Clamp a value between a minimum and maximum value.
 * @param[in]   val     The value to be clamped
 * @param[in]   min     The minimum value
 * @param[in]   max     The maximum value
 * @return      sint64
 * @retval      The clamped value
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace   CPD-85957
 */
STBM_LOCAL_INLINE sint64 StbM_Clamp(sint64 val, sint64 min, sint64 max)
{
    return (val < min) ? min : ((val > max) ? max : val);
}
#endif

#if STBM_TIME_VALIDATION_USED == STD_ON || STBM_ETH_SLAVE_TIME_VALIDATION_USED == STD_ON
/**
 * @brief       Convert a time value from a time base to a virtual local time.
 * @param[in]   tvPtr   The time value to be converted
 * @param[out]  virtualLocalTimePtr  The converted virtual local time
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace   CPD-85958
 */
STBM_LOCAL_INLINE void
    StbM_TvTime64ToVirtualLocalTime(const uint64* tvPtr, StbM_VirtualLocalTimeType* virtualLocalTimePtr)
{
    virtualLocalTimePtr->nanosecondsLo = (uint32)*tvPtr;
    virtualLocalTimePtr->nanosecondsHi = (uint32)(*tvPtr >> STBM_SHIFT_BIT32);
}
#endif

#if STBM_TIME_VALIDATION_USED == STD_ON
/**
 * @brief        Convert a time value from a time stamp to a time stamp short.
 * @param[in]    tlPtr   The time stamp to be converted
 * @param[inout] timeStampShortPtr The converted time stamp short
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace   CPD-85959
 */
STBM_LOCAL_INLINE void StbM_TlTime64ToTimeStampShort(const uint64* tlPtr, StbM_TimeStampShortType* timeStampShortPtr)
{
    timeStampShortPtr->nanoseconds = (uint32)(*tlPtr % STBM_NS_PER_S);
    timeStampShortPtr->seconds     = (uint32)(*tlPtr / STBM_NS_PER_S);
}
#endif

#if STBM_CAN_SLAVE_TIME_VALIDATION_USED == STD_ON || STBM_ETH_SLAVE_TIME_VALIDATION_USED == STD_ON
/**
 * @brief       Calculate the absolute value of a float64 value.
 * @param[in]   x   The value to be calculated
 * @return      float64
 * @retval      The absolute value
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace   CPD-85960
 */
STBM_LOCAL_INLINE float64 StbM_Float64Abs(float64 x)
{
    return (x < 0.0) ? -x : x;
}

/**
 * @brief       Get the status bit of a time base status.
 * @param[in]   timeBaseStatusPtr   The time base status
 * @param[in]   bit                 The status bit to be checked
 * @return      boolean
 * @retval      TRUE : if the status bit is set
 * @retval      FALSE : otherwise
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace   CPD-85961
 */
STBM_LOCAL_INLINE boolean
    StbM_TimeBaseStatusBitGet(const StbM_TimeBaseStatusType* timeBaseStatusPtr, StbM_TimeBaseStatusType bit)
{
    return (*timeBaseStatusPtr & bit) != 0u;
}
#endif

#if STBM_TIME_VALIDATION_USED == STD_ON
/** Notify the time base provider record table. */
#define StbM_TimeBaseProviderNotificationBusRole(                                                             \
    measurementQueuePtr,                                                                                      \
    operation,                                                                                                \
    timeValidationDataInfoPtr,                                                                                \
    blockCount)                                                                                               \
    do                                                                                                        \
    {                                                                                                         \
        if (measurementQueuePtr != NULL_PTR)                                                                  \
        {                                                                                                     \
            SchM_Enter_StbM_TimeValidation();                                                                 \
            for (uint16 i = 0u; i < timeValidationDataInfoPtr->Size; i++)                                     \
            {                                                                                                 \
                (void)measurementQueuePtr->operation(                                                         \
                    &measurementQueuePtr->Data[((uint32)i + timeValidationDataInfoPtr->Front) % blockCount]); \
            }                                                                                                 \
            timeValidationDataInfoPtr->Front = 0u;                                                            \
            timeValidationDataInfoPtr->Size  = 0u;                                                            \
            SchM_Exit_StbM_TimeValidation();                                                                  \
        }                                                                                                     \
    } while (0)

#if STBM_CAN_SLAVE_TIME_VALIDATION_USED == STD_ON
STBM_LOCAL_INLINE void StbM_TimeBaseProviderNotificationCanSlave(
    StbM_CanSlaveMeasurementQueueType* measurementQueuePtr,
    StbM_TimeValidataionDataInfoType*  timeValidationDataInfoPtr,
    uint16                             blockCount)
{
    StbM_TimeBaseProviderNotificationBusRole(
        measurementQueuePtr,
        SetSlaveTimingData,
        timeValidationDataInfoPtr,
        blockCount);
}
#endif

#if STBM_CAN_MASTER_TIME_VALIDATION_USED == STD_ON
STBM_LOCAL_INLINE void StbM_TimeBaseProviderNotificationCanMaster(
    StbM_CanMasterMeasurementQueueType* measurementQueuePtr,
    StbM_TimeValidataionDataInfoType*   timeValidationDataInfoPtr,
    uint16                              blockCount)
{
    StbM_TimeBaseProviderNotificationBusRole(
        measurementQueuePtr,
        SetMasterTimingData,
        timeValidationDataInfoPtr,
        blockCount);
}
#endif

#if STBM_ETH_SLAVE_TIME_VALIDATION_USED == STD_ON
STBM_LOCAL_INLINE void StbM_TimeBaseProviderNotificationEthSlave(
    StbM_EthSlaveMeasurementQueueType* measurementQueuePtr,
    StbM_TimeValidataionDataInfoType*  timeValidationDataInfoPtr,
    uint16                             blockCount)
{
    StbM_TimeBaseProviderNotificationBusRole(
        measurementQueuePtr,
        SetSlaveTimingData,
        timeValidationDataInfoPtr,
        blockCount);
}

STBM_LOCAL_INLINE void StbM_TimeBaseProviderNotificationPdelayInitiator(
    StbM_PdelayInitiatorMeasurementQueueType* measurementQueuePtr,
    StbM_TimeValidataionDataInfoType*         timeValidationDataInfoPtr,
    uint16                                    blockCount)
{
    StbM_TimeBaseProviderNotificationBusRole(
        measurementQueuePtr,
        SetPdelayInitiatorData,
        timeValidationDataInfoPtr,
        blockCount);
}
#endif

#if STBM_ETH_MASTER_TIME_VALIDATION_USED == STD_ON
STBM_LOCAL_INLINE void StbM_TimeBaseProviderNotificationEthMaster(
    StbM_EthMasterMeasurementQueueType* measurementQueuePtr,
    StbM_TimeValidataionDataInfoType*   timeValidationDataInfoPtr,
    uint16                              blockCount)
{
    StbM_TimeBaseProviderNotificationBusRole(
        measurementQueuePtr,
        SetMasterTimingData,
        timeValidationDataInfoPtr,
        blockCount);
}

STBM_LOCAL_INLINE void StbM_TimeBaseProviderNotificationPdelayResponder(
    StbM_PdelayResponderMeasurementQueueType* measurementQueuePtr,
    StbM_TimeValidataionDataInfoType*         timeValidationDataInfoPtr,
    uint16                                    blockCount)
{
    StbM_TimeBaseProviderNotificationBusRole(
        measurementQueuePtr,
        SetPdelayResponderData,
        timeValidationDataInfoPtr,
        blockCount);
}
#endif
#endif

/* ================================================ type definitions ================================================ */
#if STBM_STATUS_NOTIFICATIONS_ENABLED == STD_ON
/* clang-format off */
enum
{
    STBM_TIME_BASE_STATUS_MASK = STBM_TIMEBASE_STATUS_TIMEOUT
                                 | STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY
                                 | STBM_TIMEBASE_STATUS_GLOBAL_TIME_BASE
                                 | STBM_TIMEBASE_STATUS_TIMELEAP_FUTURE
                                 | STBM_TIMEBASE_STATUS_TIMELEAP_PAST
                                 | STBM_TIMEBASE_STATUS_RATE_CORRECTED
                                 | STBM_TIMEBASE_STATUS_RATE_EXCEEDED
#if STBM_TIME_VALIDATION_USED == STD_ON
                                 | STBM_TIMEBASE_STATUS_PDELAY_EXCEEDED
                                 | STBM_TIMEBASE_STATUS_RATEJITTERWANDER_EXCEEDED
                                 | STBM_TIMEBASE_STATUS_TIME_PROGRESSION_INCONSISTENCY
#endif
#if STBM_FALLBACK_VIRTUAL_LOCAL_TIME_USED == STD_ON
                                 | STBM_TIMEBASE_STATUS_FALLBACK_TIME_EXTRAPOLATION
#endif
};
/* clang-format on */

typedef struct
{
    StbM_TimeBaseNotificationType SetEvent;
    StbM_TimeBaseNotificationType ClrEvent;
} StbM_TimeBaseStatusNotificationType;
#endif

/* ========================================= internal function declarations ========================================= */
#if (STBM_DEV_ERROR_DETECT == STD_ON)
#ifdef QAC_ANALYZE
#pragma PRQA_NO_SIDE_EFFECTS StbM_ValidatePointer
#pragma PRQA_NO_SIDE_EFFECTS StbM_ValidateCommon
#pragma PRQA_NO_SIDE_EFFECTS StbM_ValidatePartition
#pragma PRQA_NO_SIDE_EFFECTS StbM_ValidateNanoseconds
#pragma PRQA_NO_SIDE_EFFECTS StbM_ValidateUserData
#pragma PRQA_NO_SIDE_EFFECTS StbM_ValidateGlobalTimeMaster
#pragma PRQA_NO_SIDE_EFFECTS StbM_ValidateSynchronizedTimeBase
#pragma PRQA_NO_SIDE_EFFECTS StbM_ValidateAllowMasterRateCorrection
#endif
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL boolean StbM_ValidateInit(const StbM_ConfigType* configPtr);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL boolean StbM_ValidatePointer(uint8 apiId, const void* pointer);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL boolean StbM_ValidateCommon(uint8 apiId, StbM_SynchronizedTimeBaseType timeBaseId, uint16 timeBaseIndex);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL boolean StbM_ValidatePartition(uint8 apiId, uint16 timeBaseIndex);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL boolean StbM_ValidateNanoseconds(uint8 apiId, uint32 nanoseconds);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL boolean StbM_ValidateUserData(uint8 apiId, const StbM_UserDataType* userDataPtr);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL boolean StbM_ValidateGlobalTimeMaster(uint8 apiId, uint16 timeBaseIndex);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL boolean StbM_ValidateSynchronizedTimeBase(uint8 apiId, uint16 timeBaseIndex);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#if STBM_CLONE_TIMEBASE_USED == STD_ON
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL boolean StbM_ValidateTimeBaseCloning(uint8 apiId, uint16 timeBaseIdx);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#if STBM_TIME_CORRECTION_SUPPORT == STD_ON
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL boolean StbM_ValidateAllowMasterRateCorrection(uint8 apiId, uint16 timeBaseIndex);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#if STBM_NOTIFICATION_CUSTOMER_NUM > 0
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL
boolean StbM_ValidateNotificationCustomer(StbM_SynchronizedTimeBaseType timeBaseId, StbM_CustomerIdType customerId);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#if STBM_QUERY_FRESHNESS_VALUE != STBM_QUERY_FRESHNESS_VALUE_NONE
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL boolean StbM_ValidateFreshnessValueId(uint8 apiId, uint16 freshnessValueId);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif
#endif

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL Std_ReturnType StbM_ProcessValidBusTimeUpdate(
    StbM_SynchronizedTimeBaseType timeBaseId,
    StbM_Time64TupleType*         time64Tuple,
    const StbM_MeasurementType*   measureDataPtr,
    const StbM_TimeTupleType*     timeTuplePtr,
    const StbM_UserDataType*      userDataPtr);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL Std_ReturnType StbM_ApplyTimeBaseUpdate(
    StbM_SynchronizedTimeBaseType timeBaseId,
    const StbM_TimeTupleType*     timeTuplePtr,
    StbM_Time64TupleType*         time64Tuple,
    uint64                        tLURx,
    const StbM_UserDataType*      userDataPtr);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL uint16 StbM_GetPartitionIndex(void);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL uint16 StbM_FindSynchronizedTimeBase(StbM_SynchronizedTimeBaseType timeBaseId);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL void StbM_TimeBaseInit(uint16 index);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#if (STBM_NOTIFICATION_CUSTOMER_NUM > 0u)
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL void StbM_NotificationCustomerInit(void);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL void
    StbM_LocalClockToLocalTime(const StbM_LocalTimeClockCfgType* localClockCfg, uint64* tVPtr, const uint64* tLPtr);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL void StbM_GetCurrentTimeTuple(uint16 timeBaseIndex, StbM_Time64TupleType* time64TuplePtr);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL uint64 StbM_GetGlobalTime64(uint16 timeBaseIndex, uint64 tV, uint64 tVFallback);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL void StbM_UpdateMainTimeTuple(uint16 timeBaseIndex, const StbM_Time64TupleType* time64TuplePtr);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL uint64 StbM_GTTimeTo64Time(const StbM_TimeStampType* timeStamp);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL void StbM_64TimeToGTTime(uint64 time64, StbM_TimeStampType* timeStamp);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL uint64 StbM_LTTimeTo64Time(StbM_VirtualLocalTimeType localTime);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL void StbM_CheckTimeleap(uint16 timeBaseIndex, uint64 TL_Sync, uint64 TG_URx);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL void StbM_CheckTimeout(uint16 timeBaseIndex, const uint64* tvPtr);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL void StbM_CycleTaskHandle(uint16 timeBaseIndex, const StbM_Time64TupleType* time64TuplePtr);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#if STBM_TIME_CORRECTION_SUPPORT == STD_ON
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL void StbM_CalculateRrc(
    uint16                  timeBaseIndex,
    const uint64*           tGPtr,
    const uint64*           tVPtr,
    StbM_TimeBaseStatusType timeBaseStatus);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL void StbM_CalculateRoc(uint16 timeBaseIndex, uint64* tGPtr, uint64 tLSync);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#if (STBM_TIME_RECORDING_SUPPORT == STD_ON)
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL void StbM_TimeRecord(uint16 timeBaseIndex, uint64 TLsync, uint64 TGrx, uint64 TVrx, uint32 pathDelay);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL void StbM_TimeRecordback(uint16 timeBaseIndex);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL uint64 StbM_ProcessBusTimeCorrection(
    StbM_SynchronizedTimeBaseType timeBaseId,
    StbM_Time64TupleType          time64Tuple,
    const StbM_TimeTupleType*     timeTuplePtr,
    uint64                        tGRx,
    const StbM_MeasurementType*   measureDataPtr);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL void StbM_SetUserDataIn(uint16 timeBaseIndex, const StbM_UserDataType* userDataPtr);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL Std_ReturnType StbM_SetGlobalTimePrivate(
    StbM_SynchronizedTimeBaseType timeBaseId,
    const StbM_TimeStampType*     timeStamp,
    const StbM_UserDataType*      userData,
    uint8                         serviceId,
    boolean                       isUpdateCounter);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL void StbM_TimeBaseStatusBitSet(StbM_TimeBaseType* timeBaseVarPtr, StbM_TimeBaseStatusType bit, boolean set);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#if STBM_NOTIFICATION_CUSTOMER_NUM > 0
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL void StbM_TimerNotificationCustomer(void);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#if STBM_TRIGGERED_CUSTOMER_NUM > 0
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL boolean StbM_CheckTimeBaseStatus(StbM_TimeBaseStatusType timeBaseStatus);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL boolean StbM_CheckScheduleTableStatus(ScheduleTableType scheduleTableID);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL void StbM_TriggeredCustomer(void);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#if STBM_STORE_TIMEBASE_NONVOLATILE_USED == STD_ON
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL void StbM_ReadBackupTimeHandle(uint16 timeBaseIndex, uint64* backupTimeValuePtr);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL void StbM_WriteBackupTimeHandle(uint16 timeBaseIndex, const uint64* backupTimeValuePtr);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#if STBM_TIME_VALIDATION_USED == STD_ON
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL void StbM_TimeBaseProviderNotification(uint16 timeBaseIndex);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#if STBM_CAN_SLAVE_TIME_VALIDATION_USED == STD_ON || STBM_ETH_SLAVE_TIME_VALIDATION_USED == STD_ON
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL void StbM_PdelayAndRateValidationHandler(
    const StbM_SynchronizedTimeBaseCfgType*  timeBaseCfgPtr,
    StbM_TimeBaseType*                       timeBaseVarPtr,
    uint32                                   pathDelay,
    const StbM_RateValidationTimeTripleType* timeTriplePtr);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#if STBM_STATUS_NOTIFICATIONS_ENABLED == STD_ON
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL void StbM_CheckStatusNotification(uint16 timeBaseIndex);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#if STBM_CLONE_TIMEBASE_USED == STD_ON
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL void StbM_CloneTimeBaseCopy(uint16 destTimeBaseIdx, uint16 srcTimeBaseIdx);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL void StbM_CloneTimeBaseDeferCopyHandler(uint16 srcTimeBaseIdx);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

/* ============================================ internal data definitions =========================================== */
#define STBM_START_SEC_VAR_CLEARED_BOOLEAN
#include "StbM_MemMap.h"
/** @brief Initialization status of each partition */
STBM_LOCAL boolean StbM_InitStatus[STBM_PARTITION_NUMBER]; /**< @range FALSE..TRUE */
#define STBM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "StbM_MemMap.h"

#if (STBM_TRIGGERED_CUSTOMER_NUM > 0)
#define STBM_START_SEC_VAR_CLEARED_16
#include "StbM_MemMap.h"
/** @brief Remaining period for each triggered customer */
STBM_LOCAL uint16 StbM_TriggerRemainPeriod[STBM_TRIGGERED_CUSTOMER_NUM]; /**< @range 0..65535 */
#define STBM_STOP_SEC_VAR_CLEARED_16
#include "StbM_MemMap.h"
#endif

#define STBM_START_SEC_VAR_CLEARED_PTR
#include "StbM_MemMap.h"
/** @brief Pointer to the configuration structure */
STBM_LOCAL const StbM_ConfigType* StbM_ConfigPtr; /**< @range NA */
#define STBM_STOP_SEC_VAR_CLEARED_PTR
#include "StbM_MemMap.h"

#define STBM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "StbM_MemMap.h"
STBM_LOCAL StbM_TimeBaseType StbM_TimeBaseRuntime[STBM_TIME_BASE_NUM];
#define STBM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "StbM_MemMap.h"

#if STBM_STATUS_NOTIFICATIONS_ENABLED == STD_ON
#define STBM_START_SEC_CONST_32
#include "StbM_MemMap.h"
/**
 * @brief Status notification configuration
 */
/* clang-format off */
STBM_LOCAL const StbM_TimeBaseStatusNotificationType StbM_TimeBaseStatusNotifications[] = {
    /*             SetEvent                                         ClrEvent                                                     Status */
    {STBM_TIMEBASE_EV_TIMEOUT_OCCURRED,               STBM_TIMEBASE_EV_TIMEOUT_REMOVED},                        /* STBM_TIMEBASE_STATUS_TIMEOUT */
    {STBM_TIMEBASE_EV_SYNC_TO_SUBDOMAIN,              STBM_TIMEBASE_EV_SYNC_TO_GLOBAL_MASTER},                  /* STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY */
    {STBM_TIMEBASE_EV_GLOBAL_TIME,                    0u},                                                      /* STBM_TIMEBASE_STATUS_GLOBAL_TIME_BASE */
    {STBM_TIMEBASE_EV_TIMELEAP_FUTURE,                STBM_TIMEBASE_EV_TIMELEAP_FUTURE_REMOVED},                /* STBM_TIMEBASE_STATUS_TIMELEAP_FUTURE */
    {STBM_TIMEBASE_EV_TIMELEAP_PAST,                  STBM_TIMEBASE_EV_TIMELEAP_PAST_REMOVED},                  /* STBM_TIMEBASE_STATUS_TIMELEAP_PAST */
    {STBM_TIMEBASE_EV_RATECORRECTION,                 0u},                                                      /* STBM_TIMEBASE_STATUS_RATE_CORRECTED */
    {STBM_TIMEBASE_EV_RATE_EXCEEDED,                  STBM_TIMEBASE_EV_RATECORRECTION},                         /* STBM_TIMEBASE_STATUS_RATE_EXCEEDED */
#if STBM_TIME_VALIDATION_USED == STD_ON
    {STBM_TIMEBASE_EV_PDELAY_EXCEEDED,                STBM_TIMEBASE_EV_PDELAY_EXCEEDED_REMOVED},                /* STBM_TIMEBASE_STATUS_PDELAY_EXCEEDED */
    {STBM_TIMEBASE_EV_RATEJITTERWANDER_EXCEEDED,      STBM_TIMEBASE_EV_RATEJITTERWANDER_EXCEEDED_REMOVED},      /* STBM_TIMEBASE_STATUS_RATEJITTERWANDER_EXCEEDED */
    {STBM_TIMEBASE_EV_TIME_PROGRESSION_INCONSISTENCY, STBM_TIMEBASE_EV_TIME_PROGRESSION_INCONSISTENCY_REMOVED}, /* STBM_TIMEBASE_STATUS_TIME_PROGRESSION_INCONSISTENCY */
#endif
#if STBM_FALLBACK_VIRTUAL_LOCAL_TIME_USED == STD_ON
    {STBM_TIMEBASE_EV_FALLBACK_TIME_EXTRAPOLATION,    STBM_TIMEBASE_EV_FALLBACK_TIME_EXTRAPOLATION_REMOVED},    /* STBM_TIMEBASE_STATUS_FALLBACK_TIME_EXTRAPOLATION */
#endif
}; /**< @range NA */
/* clang-format on */
#define STBM_STOP_SEC_CONST_32
#include "StbM_MemMap.h"
#endif

/* ============================================ external data definition ============================================ */

/* ========================================== external function definitions ========================================= */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * The implementation of StbM_Init.
 */
void StbM_Init(const StbM_ConfigType* configPtr)
/* PRQA S 1532 -- */
{
    StbM_ArtiApiStart(2, STBM_SID_INIT, configPtr);
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

        StbM_InitStatus[StbM_GetPartitionIndex()] = TRUE;
    }
    StbM_ArtiApiReturn(1, STBM_SID_INIT);
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#if (STBM_VERSIONINFO_API == STD_ON)
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * The implementation of StbM_GetVersionInfo.
 */
void StbM_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
    StbM_ArtiApiStart(2, STBM_SID_GETVERSIONINFO, versioninfo);
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
    StbM_ArtiApiReturn(2, STBM_SID_GETVERSIONINFO, versioninfo);
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/* PRQA S 6070, 1510 ++ */ /* VL_MTR_StbM_STCAL, VL_StbM_Redeclaration */
/**
 * The implementation of StbM_GetCurrentTime.
 */
Std_ReturnType StbM_GetCurrentTime(
    StbM_SynchronizedTimeBaseType timeBaseId,
    StbM_TimeTupleType*           timeTuple,
    StbM_UserDataType*            userData)
/* PRQA S 6070, 1510 -- */
{
    Std_ReturnType ret = E_NOT_OK;
    StbM_ArtiApiStart(4, STBM_SID_GETCURRENTTIME, timeBaseId, timeTuple, userData);
    uint16 timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommon(STBM_SID_GETCURRENTTIME, timeBaseId, timeBaseIndex)
        && StbM_ValidatePointer(STBM_SID_GETCURRENTTIME, timeTuple)
        && StbM_ValidatePointer(STBM_SID_GETCURRENTTIME, userData)
        && StbM_ValidatePartition(STBM_SID_GETCURRENTTIME, timeBaseIndex))
#endif
    {
        /* SWS_StbM_00173 SWS_StbM_00434 */
        StbM_Time64TupleType time64Tuple;
        StbM_GetCurrentTimeTuple(timeBaseIndex, &time64Tuple);

        /* SWS_StbM_00608 */
#if STBM_FALLBACK_VIRTUAL_LOCAL_TIME_USED == STD_ON
        if (time64Tuple.TV == STBM_TIME_64_INVALID)
        {
            time64Tuple.TV = time64Tuple.TVFallback;
        }
#endif

        if ((time64Tuple.TL != STBM_TIME_64_INVALID) && (time64Tuple.TV != STBM_TIME_64_INVALID))
        {
            const StbM_TimeBaseType* timeBasePtr = &StbM_TimeBaseRuntime[timeBaseIndex];
            SchM_Enter_StbM_Context();
            StbM_64TimeToGTTime(time64Tuple.TL, &timeTuple->globalTime);
            timeTuple->timeBaseStatus                 = timeBasePtr->TimeBaseStatus;
            timeTuple->virtualLocalTime.nanosecondsHi = (uint32)StbM_Shift_Right32(time64Tuple.TV);
            timeTuple->virtualLocalTime.nanosecondsLo = (uint32)time64Tuple.TV;
            *userData                                 = timeBasePtr->UserData;
            SchM_Exit_StbM_Context();

            ret = E_OK;
        }
    }

    StbM_ArtiApiReturn(2, STBM_SID_GETCURRENTTIME, ret);
    return ret;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#if STBM_TIME_VALIDATION_USED == STD_ON
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * The implementation of StbM_GetCurrentSafeTime.
 */
Std_ReturnType StbM_GetCurrentSafeTime(
    StbM_SynchronizedTimeBaseType timeBaseId,
    StbM_TimeTripleType*          timeTriple,
    StbM_UserDataType*            userData)
{
    Std_ReturnType ret = E_NOT_OK;
    StbM_ArtiApiStart(4, STBM_SID_GETCURRENTSAFETIME, timeBaseId, timeTriple, userData);
    uint16 timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommon(STBM_SID_GETCURRENTSAFETIME, timeBaseId, timeBaseIndex)
        && StbM_ValidatePointer(STBM_SID_GETCURRENTSAFETIME, timeTriple)
        && StbM_ValidatePointer(STBM_SID_GETCURRENTSAFETIME, userData))
#endif
    {
        StbM_Time64TupleType time64Tuple;
        StbM_GetCurrentTimeTuple(timeBaseIndex, &time64Tuple);

        if ((time64Tuple.TL != STBM_TIME_64_INVALID) && (time64Tuple.TV != STBM_TIME_64_INVALID)
            && (time64Tuple.TVFallback != STBM_TIME_64_INVALID))
        {
            const StbM_TimeBaseType* timeBasePtr = &StbM_TimeBaseRuntime[timeBaseIndex];
            SchM_Enter_StbM_Context();
            StbM_TvTime64ToVirtualLocalTime(&time64Tuple.TV, &timeTriple->virtualLocalTime);
            StbM_TvTime64ToVirtualLocalTime(&time64Tuple.TVFallback, &timeTriple->fallbackVirtualLocalTime);
            StbM_64TimeToGTTime(time64Tuple.TL, &timeTriple->globalTime);
            timeTriple->timeBaseStatus = timeBasePtr->TimeBaseStatus;
            *userData                  = timeBasePtr->UserData;
            SchM_Exit_StbM_Context();

            ret = E_OK;
        }
    }
    StbM_ArtiApiReturn(2, STBM_SID_GETCURRENTSAFETIME, ret);
    return ret;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * The implementation of StbM_GetCurrentVirtualLocalTime.
 */
Std_ReturnType
    StbM_GetCurrentVirtualLocalTime(StbM_SynchronizedTimeBaseType timeBaseId, StbM_VirtualLocalTimeType* localTimePtr)
{
    Std_ReturnType ret = E_NOT_OK;
    StbM_ArtiApiStart(3, STBM_SID_GETCURRENTVIRTUALLOCALTIME, timeBaseId, localTimePtr);
    uint16 timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommon(STBM_SID_GETCURRENTVIRTUALLOCALTIME, timeBaseId, timeBaseIndex)
        && StbM_ValidatePointer(STBM_SID_GETCURRENTVIRTUALLOCALTIME, localTimePtr))
#endif
    {
        /* SWS_StbM_00437 */
        uint64 tV = STBM_TIME_64_INVALID;
        StbM_LocalClockToLocalTime(TIMEBASE_CFG(timeBaseIndex).LocalTimeClock, &tV, NULL_PTR);
        if (tV != STBM_TIME_64_INVALID)
        {
            localTimePtr->nanosecondsHi = (uint32)StbM_Shift_Right32(tV);
            localTimePtr->nanosecondsLo = (uint32)tV;
            ret                         = E_OK;
        }
    }
    StbM_ArtiApiReturn(2, STBM_SID_GETCURRENTVIRTUALLOCALTIME, ret);
    return ret;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#if STBM_FALLBACK_VIRTUAL_LOCAL_TIME_USED == STD_ON

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * The implementation of StbM_GetFallbackVirtualLocalTime.
 */
Std_ReturnType
    StbM_GetFallbackVirtualLocalTime(StbM_SynchronizedTimeBaseType timeBaseId, StbM_VirtualLocalTimeType* localTimePtr)
{
    Std_ReturnType ret = E_NOT_OK;
    StbM_ArtiApiStart(3, STBM_SID_GETFALLBACKVIRTUALLOCALTIME, timeBaseId, localTimePtr);
    uint16 timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommon(STBM_SID_GETFALLBACKVIRTUALLOCALTIME, timeBaseId, timeBaseIndex)
        && StbM_ValidatePointer(STBM_SID_GETFALLBACKVIRTUALLOCALTIME, localTimePtr))
#endif
    {
        /* SWS_StbM_00595 */
        const StbM_LocalTimeClockCfgType* fallbackTimeClock = TIMEBASE_CFG(timeBaseIndex).FallbackTimeClock;
        if (fallbackTimeClock != NULL_PTR)
        {
            uint64 tVFallback = STBM_TIME_64_INVALID;
            StbM_LocalClockToLocalTime(fallbackTimeClock, &tVFallback, NULL_PTR);
            if (tVFallback != STBM_TIME_64_INVALID)
            {
                localTimePtr->nanosecondsHi = (uint32)StbM_Shift_Right32(tVFallback);
                localTimePtr->nanosecondsLo = (uint32)tVFallback;
                ret                         = E_OK;
            }
        }
    }
    StbM_ArtiApiReturn(2, STBM_SID_GETFALLBACKVIRTUALLOCALTIME, ret);
    return ret;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * The implementation of StbM_SetGlobalTime.
 */
Std_ReturnType StbM_SetGlobalTime(
    StbM_SynchronizedTimeBaseType timeBaseId,
    const StbM_TimeStampType*     timeStamp,
    const StbM_UserDataType*      userData)
{
    StbM_ArtiApiStart(4, STBM_SID_SETGLOBALTIME, timeBaseId, timeStamp, userData);
    Std_ReturnType ret = StbM_SetGlobalTimePrivate(timeBaseId, timeStamp, userData, STBM_SID_SETGLOBALTIME, TRUE);
    StbM_ArtiApiReturn(2, STBM_SID_SETGLOBALTIME, ret);
    return ret;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * The implementation of StbM_UpdateGlobalTime.
 */
Std_ReturnType StbM_UpdateGlobalTime(
    StbM_SynchronizedTimeBaseType timeBaseId,
    const StbM_TimeStampType*     timeStamp,
    const StbM_UserDataType*      userData)
{
    StbM_ArtiApiStart(4, STBM_SID_UPDATEGLOBALTIME, timeBaseId, timeStamp, userData);
    Std_ReturnType ret = StbM_SetGlobalTimePrivate(timeBaseId, timeStamp, userData, STBM_SID_UPDATEGLOBALTIME, FALSE);
    StbM_ArtiApiReturn(2, STBM_SID_UPDATEGLOBALTIME, ret);
    return ret;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/* PRQA S 6070 ++ */ /* VL_MTR_StbM_STCAL */
/**
 * The implementation of StbM_SetUserData.
 */
Std_ReturnType StbM_SetUserData(StbM_SynchronizedTimeBaseType timeBaseId, const StbM_UserDataType* userData)
/* PRQA S 6070 -- */
{
    Std_ReturnType ret = E_NOT_OK;
    StbM_ArtiApiStart(3, STBM_SID_SETUSERDATA, timeBaseId, userData);
    uint16 timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommon(STBM_SID_SETUSERDATA, timeBaseId, timeBaseIndex)
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

    StbM_ArtiApiReturn(2, STBM_SID_SETUSERDATA, ret);
    return ret;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/* PRQA S 6070 ++ */ /* VL_MTR_StbM_STCAL */
/**
 * The implementation of StbM_BusSetGlobalTime.
 */
Std_ReturnType StbM_BusSetGlobalTime(
    StbM_SynchronizedTimeBaseType timeBaseId,
    const StbM_TimeTupleType*     timeTuplePtr,
    const StbM_UserDataType*      userDataPtr,
    const StbM_MeasurementType*   measureDataPtr)
/* PRQA S 6070 -- */
{
    Std_ReturnType ret = E_NOT_OK;
    StbM_ArtiApiStart(5, STBM_SID_BUSSETGLOBALTIME, timeBaseId, timeTuplePtr, userDataPtr, measureDataPtr);
    uint16 timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommon(STBM_SID_BUSSETGLOBALTIME, timeBaseId, timeBaseIndex)
        && StbM_ValidatePointer(STBM_SID_BUSSETGLOBALTIME, timeTuplePtr)
        && StbM_ValidatePointer(STBM_SID_BUSSETGLOBALTIME, measureDataPtr)
        && StbM_ValidateNanoseconds(STBM_SID_BUSSETGLOBALTIME, timeTuplePtr->globalTime.nanoseconds)
        && StbM_ValidateUserData(STBM_SID_BUSSETGLOBALTIME, userDataPtr)
        && StbM_ValidateSynchronizedTimeBase(STBM_SID_BUSSETGLOBALTIME, timeBaseIndex)
        && StbM_ValidatePartition(STBM_SID_BUSSETGLOBALTIME, timeBaseIndex))
#endif
    {
        /* If Primary Virtual Local Time could not be determined, set tV to invalid value.
         * If Fallback Virtual Local Time not used or could not be determined, set tVFallback to invalid value.
         */
        StbM_Time64TupleType time64Tuple;
        StbM_GetCurrentTimeTuple(timeBaseIndex, &time64Tuple);

        const StbM_SynchronizedTimeBaseCfgType* timeBaseCfgPtr = &TIMEBASE_CFG(timeBaseIndex);

#if STBM_CAN_SLAVE_TIME_VALIDATION_USED == STD_ON
        StbM_CanSlaveMeasurementQueueType* canSlaveMeasurementQueuePtr = timeBaseCfgPtr->CanSlaveMeasurementQueuePtr;
        if (canSlaveMeasurementQueuePtr != NULL_PTR)
        {
            StbM_CanTimeSlaveMeasurementType* canSlaveSentinelMeasureDataPtr =
                &canSlaveMeasurementQueuePtr->Data[timeBaseCfgPtr->TimeValidationRecordTableBlockCount];
            SchM_Enter_StbM_TimeValidation();
            StbM_TvTime64ToVirtualLocalTime(&time64Tuple.TV, &canSlaveSentinelMeasureDataPtr->referenceLocalTimestamp);
            StbM_TlTime64ToTimeStampShort(&time64Tuple.TL, &canSlaveSentinelMeasureDataPtr->referenceGlobalTimestamp);
            SchM_Exit_StbM_TimeValidation();
        }
#endif
#if STBM_ETH_SLAVE_TIME_VALIDATION_USED == STD_ON
        StbM_EthSlaveMeasurementQueueType* ethSlaveMeasurementQueuePtr = timeBaseCfgPtr->EthSlaveMeasurementQueuePtr;
        if (ethSlaveMeasurementQueuePtr != NULL_PTR)
        {
            StbM_EthTimeSlaveMeasurementType* ethSlaveSentinelMeasureDataPtr =
                &ethSlaveMeasurementQueuePtr->Data[timeBaseCfgPtr->TimeValidationRecordTableBlockCount];
            SchM_Enter_StbM_TimeValidation();
            StbM_TvTime64ToVirtualLocalTime(&time64Tuple.TV, &ethSlaveSentinelMeasureDataPtr->referenceLocalTimestamp);
            StbM_TlTime64ToTimeStampShort(&time64Tuple.TL, &ethSlaveSentinelMeasureDataPtr->referenceGlobalTimestamp);
            SchM_Exit_StbM_TimeValidation();
        }
#endif
#if STBM_CAN_SLAVE_TIME_VALIDATION_USED == STD_ON || STBM_ETH_SLAVE_TIME_VALIDATION_USED == STD_ON
        if (timeBaseCfgPtr->PdelayAndRateValidationPtr != NULL_PTR)
        {
            StbM_TimeBaseType* timeBasePtr = &StbM_TimeBaseRuntime[timeBaseIndex];
            uint64             tVRx        = StbM_LTTimeTo64Time(timeTuplePtr->virtualLocalTime);
            uint64             tGRx        = StbM_GTTimeTo64Time(&timeTuplePtr->globalTime);

            StbM_PdelayAndRateValidationHandler(
                timeBaseCfgPtr,
                timeBasePtr,
                measureDataPtr->pathDelay,
                &(StbM_RateValidationTimeTripleType){.TGRx = tGRx, .TVRx = tVRx, .TLSync = time64Tuple.TL});
        }
#endif

        /* Do not use fallback virtual local time to replace primary virtual local time, because <Bus>TSync only use
         * primary virtual local time, using fallback virtual local time  would result in inconsistency between the
         * clock sources used by StbM and <Bus>TSync. */
        if (time64Tuple.TV != STBM_TIME_64_INVALID)
        {
            ret = StbM_ProcessValidBusTimeUpdate(timeBaseId, &time64Tuple, measureDataPtr, timeTuplePtr, userDataPtr);
        }
        else
        {
            /* The purpose is to set TVMain to an invalid value without updating TLMain, while TVFallbackMain may be
               either valid or invalid. */
            SchM_Enter_StbM_Context();
            StbM_UpdateMainTimeTuple(timeBaseIndex, &time64Tuple);
            SchM_Exit_StbM_Context();
        }
        STBM_UNUSED(timeBaseCfgPtr);
    }

    StbM_ArtiApiReturn(2, STBM_SID_BUSSETGLOBALTIME, ret);
    return ret;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * The implementation of StbM_GetRateDeviation.
 */
Std_ReturnType StbM_GetRateDeviation(
    StbM_SynchronizedTimeBaseType timeBaseId,
    StbM_RateDeviationType*       rateDeviation) /* PRQA S 3673 */ /* VL_StbM_3673 */
{
    Std_ReturnType ret = E_NOT_OK;
    StbM_ArtiApiStart(3, STBM_SID_GETRATEDEVIATION, timeBaseId, rateDeviation);
#if STBM_TIME_CORRECTION_SUPPORT == STD_ON
    uint16 timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommon(STBM_SID_GETRATEDEVIATION, timeBaseId, timeBaseIndex)
        && StbM_ValidatePointer(STBM_SID_GETRATEDEVIATION, rateDeviation)
        && StbM_ValidatePartition(STBM_SID_GETRATEDEVIATION, timeBaseIndex))
#endif
    {
        const StbM_TimeBaseType* const timeBasePtr = &StbM_TimeBaseRuntime[timeBaseIndex];
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

    StbM_ArtiApiReturn(2, STBM_SID_GETRATEDEVIATION, ret);
    return ret;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * The implementation of StbM_SetRateCorrection.
 */
Std_ReturnType StbM_SetRateCorrection(StbM_SynchronizedTimeBaseType timeBaseId, StbM_RateDeviationType rateDeviation)
{
    Std_ReturnType ret = E_NOT_OK;
    StbM_ArtiApiStart(3, STBM_SID_SETRATECORRECTION, timeBaseId, rateDeviation);
#if (STBM_TIME_CORRECTION_SUPPORT == STD_ON)
    uint16 timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommon(STBM_SID_SETRATECORRECTION, timeBaseId, timeBaseIndex)
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

#if STBM_DISCIPLINED_CLOCK_USED == STD_ON
        const StbM_DisciplinedClockCfgType* disciplinedClock = timeBaseCfg->DisciplinedClock;
        if (disciplinedClock != NULL_PTR)
        {
            /* SWS_StbM_00580 */
            sint64 ethRateDeviation = rate * STBM_POW2_41 / STBM_RATE_PPM;
            if ((ethRateDeviation >= STBM_SINT32_MIN) && (ethRateDeviation <= STBM_SINT32_MAX))
            {
                ret = EthIf_SetPhcCorrection(
                    disciplinedClock->CtrlIdx,
                    disciplinedClock->ClkUnitIdx,
                    ethRateDeviation,
                    0);
            }
        }
        else
#endif
        {
            /* SWS_StbM_00583 */
            StbM_Time64TupleType time64Tuple;
            StbM_GetCurrentTimeTuple(timeBaseIndex, &time64Tuple);
            if (time64Tuple.TL != STBM_TIME_64_INVALID)
            {
                SchM_Enter_StbM_Context();
                StbM_UpdateMainTimeTuple(timeBaseIndex, &time64Tuple);
                SchM_Exit_StbM_Context();
                ret = E_OK;
            }
        }

        if (ret == E_OK)
        {
            StbM_TimeBaseType*    timeBaseVarPtr   = &StbM_TimeBaseRuntime[timeBaseIndex];
            StbM_CurrentRateType* rateDeviationPtr = &timeBaseVarPtr->RateDeviation;

            SchM_Enter_StbM_Context();
            rateDeviationPtr->RrcValue = (sint32)rate + (sint32)STBM_RATE_PPM;
            rateDeviationPtr->RocValue = 0;
            rateDeviationPtr->IsSet    = TRUE;
            /* SWS_StbM_00568 */
            StbM_TimeBaseStatusBitSet(timeBaseVarPtr, STBM_TIMEBASE_STATUS_RATE_CORRECTED, TRUE);
            SchM_Exit_StbM_Context();
        }
    }
#else
    STBM_UNUSED(timeBaseId);
    STBM_UNUSED(rateDeviation);
#endif

    StbM_ArtiApiReturn(2, STBM_SID_SETRATECORRECTION, ret);
    return ret;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * The implementation of StbM_GetTimeLeap.
 */
Std_ReturnType StbM_GetTimeLeap(StbM_SynchronizedTimeBaseType timeBaseId, StbM_TimeDiffType* timeJump)
{
    Std_ReturnType ret = E_NOT_OK;
    StbM_ArtiApiStart(3, STBM_SID_GETTIMELEAP, timeBaseId, timeJump);
    uint16 timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommon(STBM_SID_GETTIMELEAP, timeBaseId, timeBaseIndex)
        && StbM_ValidateSynchronizedTimeBase(STBM_SID_GETTIMELEAP, timeBaseIndex)
        && StbM_ValidatePointer(STBM_SID_GETTIMELEAP, timeJump)
        && StbM_ValidatePartition(STBM_SID_GETTIMELEAP, timeBaseIndex))
#endif
    {
        SchM_Enter_StbM_Context();
        if (StbM_TimeBaseRuntime[timeBaseIndex].TimeLeapValid) /* SWS_StbM_00425 */
        {
            *timeJump = StbM_TimeBaseRuntime[timeBaseIndex].TimeLeap;
            ret       = E_OK;
        }
        SchM_Exit_StbM_Context();
    }

    StbM_ArtiApiReturn(2, STBM_SID_GETTIMELEAP, ret);
    return ret;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * The implementation of StbM_GetTimeBaseStatus.
 */
Std_ReturnType StbM_GetTimeBaseStatus(StbM_SynchronizedTimeBaseType timeBaseId, StbM_TimeBaseStatusType* timeBaseStatus)
{
    Std_ReturnType ret = E_NOT_OK;
    StbM_ArtiApiStart(3, STBM_SID_GETTIMEBASESTATUS, timeBaseId, timeBaseStatus);
    uint16 timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommon(STBM_SID_GETTIMEBASESTATUS, timeBaseId, timeBaseIndex)
        && StbM_ValidatePointer(STBM_SID_GETTIMEBASESTATUS, timeBaseStatus)
        && StbM_ValidatePartition(STBM_SID_GETTIMEBASESTATUS, timeBaseIndex))
#endif
    {
        SchM_Enter_StbM_Context();
        *timeBaseStatus = StbM_TimeBaseRuntime[timeBaseIndex].TimeBaseStatus;
        SchM_Exit_StbM_Context();
        ret = E_OK;
    }

    StbM_ArtiApiReturn(2, STBM_SID_GETTIMEBASESTATUS, ret);
    return ret;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#if (STBM_NOTIFICATION_CUSTOMER_NUM > 0)
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * The implementation of StbM_StartTimer.
 */
Std_ReturnType StbM_StartTimer(
    StbM_SynchronizedTimeBaseType timeBaseId,
    StbM_CustomerIdType           customerId,
    const StbM_TimeStampType*     expireTime)
{
    Std_ReturnType ret = E_NOT_OK;
    StbM_ArtiApiStart(4, STBM_SID_STARTTIMER, timeBaseId, customerId, expireTime);
    uint16 timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommon(STBM_SID_STARTTIMER, timeBaseId, timeBaseIndex)
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
            notificationPtr->CustomerTimerExpireTime = StbM_GTTimeTo64Time((const StbM_TimeStampType*)expireTime);
            notificationPtr->IsSet                   = TRUE;
            ret                                      = E_OK;
        }
        SchM_Exit_StbM_Context();
    }
    StbM_ArtiApiReturn(2, STBM_SID_STARTTIMER, ret);
    return ret;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#if (STBM_TIME_RECORDING_SUPPORT == STD_ON)
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * The implementation of StbM_GetSyncTimeRecordHead.
 */
Std_ReturnType StbM_GetSyncTimeRecordHead(
    StbM_SynchronizedTimeBaseType timeBaseId,
    StbM_SyncRecordTableHeadType* syncRecordTableHead)
{
    Std_ReturnType ret = E_NOT_OK;
    StbM_ArtiApiStart(3, STBM_SID_GETSYNCTIMERECORDHEAD, timeBaseId, syncRecordTableHead);
    uint16 timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommon(STBM_SID_GETSYNCTIMERECORDHEAD, timeBaseId, timeBaseIndex)
        && StbM_ValidateSynchronizedTimeBase(STBM_SID_GETSYNCTIMERECORDHEAD, timeBaseIndex)
        && StbM_ValidatePointer(STBM_SID_GETSYNCTIMERECORDHEAD, syncRecordTableHead))
#endif
    {
        syncRecordTableHead->SynchronizedTimeDomain = (uint8)timeBaseId;

        const StbM_LocalTimeClockCfgType* localClockCfgPtr = TIMEBASE_CFG(timeBaseIndex).LocalTimeClock;
        switch (localClockCfgPtr->LocalTimeHardwareType)
        {
#if STBM_CANIF_TIMESTAMP_SUPPORT == STD_ON || STBM_ETHIF_TIMESTAMP_SUPPORT == STD_ON
        case CANTSYN_GLOBALTIME_DOMAIN:
        case ETHTSYN_GLOBALTIME_DOMAIN:
            syncRecordTableHead->HWfrequency = STBM_NS_PER_S;
            syncRecordTableHead->HWprescaler = 1u;
            break;
#endif

#if STBM_GPT_TIMESTAMP_SUPPORT == STD_ON
        case GPT_CHANNEL_CONFIGURATION:
            syncRecordTableHead->HWfrequency = StbM_ClockGptConfigs[localClockCfgPtr->LocalTimeHardware].ClockFrequency;
            syncRecordTableHead->HWprescaler = StbM_ClockGptConfigs[localClockCfgPtr->LocalTimeHardware].ClockPrescaler;
            break;
#endif

#if STBM_OS_TIMESTAMP_SUPPORT == STD_ON
        case OS_COUNTER:
            syncRecordTableHead->HWfrequency = StbM_ClockOsConfigs[localClockCfgPtr->LocalTimeHardware].ClockFrequency;
            syncRecordTableHead->HWprescaler = StbM_ClockOsConfigs[localClockCfgPtr->LocalTimeHardware].ClockPrescaler;
            break;
#endif
        default:
            break;
        }
        ret = E_OK;
    }
    StbM_ArtiApiReturn(2, STBM_SID_GETSYNCTIMERECORDHEAD, ret);
    return ret;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * The implementation of StbM_TriggerTimeTransmission.
 */
Std_ReturnType StbM_TriggerTimeTransmission(StbM_SynchronizedTimeBaseType timeBaseId)
{
    Std_ReturnType ret = E_NOT_OK;
    StbM_ArtiApiStart(2, STBM_SID_TRIGGERTIMETRANSMISSION, timeBaseId);
    uint16 timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommon(STBM_SID_TRIGGERTIMETRANSMISSION, timeBaseId, timeBaseIndex)
        && StbM_ValidateSynchronizedTimeBase(STBM_SID_TRIGGERTIMETRANSMISSION, timeBaseIndex)
        && StbM_ValidatePartition(STBM_SID_TRIGGERTIMETRANSMISSION, timeBaseIndex))
#endif
    {
        SchM_Enter_StbM_Context();
        ++StbM_TimeBaseRuntime[timeBaseIndex].TimeBaseUpdateCounter;
        SchM_Exit_StbM_Context();
    }
    StbM_ArtiApiReturn(2, STBM_SID_TRIGGERTIMETRANSMISSION, ret);
    return ret;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * The implementation of StbM_GetTimeBaseUpdateCounter.
 */
uint8 StbM_GetTimeBaseUpdateCounter(StbM_SynchronizedTimeBaseType timeBaseId)
{
    uint8 updateCounter = 0u;
    StbM_ArtiApiStart(2, STBM_SID_GETTIMEBASEUPDATECOUNTER, timeBaseId);
    uint16 timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommon(STBM_SID_GETTIMEBASEUPDATECOUNTER, timeBaseId, timeBaseIndex)
        && StbM_ValidateSynchronizedTimeBase(STBM_SID_GETTIMEBASEUPDATECOUNTER, timeBaseIndex)
        && StbM_ValidatePartition(STBM_SID_GETTIMEBASEUPDATECOUNTER, timeBaseIndex))
#endif
    {
        SchM_Enter_StbM_Context();
        updateCounter = StbM_TimeBaseRuntime[timeBaseIndex].TimeBaseUpdateCounter;
        SchM_Exit_StbM_Context();
    }

    StbM_ArtiApiReturn(2, STBM_SID_GETTIMEBASEUPDATECOUNTER, updateCounter);
    return updateCounter;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#if (STBM_ALLOW_SYSTEM_WIDE_GLOBAL_TIME_MASTER == STD_ON)
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * The implementation of StbM_GetMasterConfig.
 */
Std_ReturnType StbM_GetMasterConfig(StbM_SynchronizedTimeBaseType timeBaseId, StbM_MasterConfigType* masterConfig)
{
    Std_ReturnType ret = E_NOT_OK;
    StbM_ArtiApiStart(3, STBM_SID_GETMASTERCONFIG, timeBaseId, masterConfig);
    uint16 timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommon(STBM_SID_GETMASTERCONFIG, timeBaseId, timeBaseIndex)
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

    StbM_ArtiApiReturn(2, STBM_SID_GETMASTERCONFIG, ret);
    return ret;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#if STBM_QUERY_FRESHNESS_VALUE != STBM_QUERY_FRESHNESS_VALUE_NONE
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * The implementation of StbM_GetTxFreshness.
 */
Std_ReturnType
    StbM_GetTxFreshness(uint16 StbMFreshnessValueId, uint8* StbMFreshnessValue, uint32* StbMFreshnessValueLength)
{
    StbM_ArtiApiStart(4, STBM_SID_GETTXFRESHNESS, StbMFreshnessValueId, StbMFreshnessValue, StbMFreshnessValueLength);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateFreshnessValueId(STBM_SID_GETTXFRESHNESS, StbMFreshnessValueId)
        && StbM_ValidatePointer(STBM_SID_GETTXFRESHNESS, StbMFreshnessValue)
        && StbM_ValidatePointer(STBM_SID_GETTXFRESHNESS, StbMFreshnessValueLength))
    {
        Std_ReturnType ret =
            StbM_FreshnessGetTxData(StbMFreshnessValueId, StbMFreshnessValue, StbMFreshnessValueLength);
        StbM_ArtiApiReturn(2, STBM_SID_GETTXFRESHNESS, ret);
        return ret;
    }
#endif

    StbM_ArtiApiReturn(2, STBM_SID_GETTXFRESHNESS, E_NOT_OK);
    return E_NOT_OK;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * The implementation of StbM_GetTxFreshnessTruncData.
 */
Std_ReturnType StbM_GetTxFreshnessTruncData(
    uint16  StbMFreshnessValueId,
    uint8*  StbMFreshnessValue,
    uint32* StbMFreshnessValueLength,
    uint8*  StbMTruncatedFreshnessValue,
    uint32* StbMTruncatedFreshnessValueLength)
{
    StbM_ArtiApiStart(
        6,
        STBM_SID_GETTXFRESHNESSTRUNCDATA,
        StbMFreshnessValueId,
        StbMFreshnessValue,
        StbMFreshnessValueLength,
        StbMTruncatedFreshnessValue,
        StbMTruncatedFreshnessValueLength);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateFreshnessValueId(STBM_SID_GETTXFRESHNESSTRUNCDATA, StbMFreshnessValueId)
        && StbM_ValidatePointer(STBM_SID_GETTXFRESHNESSTRUNCDATA, StbMFreshnessValue)
        && StbM_ValidatePointer(STBM_SID_GETTXFRESHNESSTRUNCDATA, StbMFreshnessValueLength)
        && StbM_ValidatePointer(STBM_SID_GETTXFRESHNESSTRUNCDATA, StbMTruncatedFreshnessValue)
        && StbM_ValidatePointer(STBM_SID_GETTXFRESHNESSTRUNCDATA, StbMTruncatedFreshnessValueLength))
    {
        Std_ReturnType ret = StbM_FreshnessGetTxTruncData(
            StbMFreshnessValueId,
            StbMFreshnessValue,
            StbMFreshnessValueLength,
            StbMTruncatedFreshnessValue,
            StbMTruncatedFreshnessValueLength);
        StbM_ArtiApiReturn(2, STBM_SID_GETTXFRESHNESSTRUNCDATA, ret);
        return ret;
    }
#endif

    StbM_ArtiApiReturn(2, STBM_SID_GETTXFRESHNESSTRUNCDATA, E_NOT_OK);
    return E_NOT_OK;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * The implementation of StbM_SPduTxConfirmation.
 */
void StbM_SPduTxConfirmation(uint16 StbMFreshnessValueId)
{
    StbM_ArtiApiStart(2, STBM_SID_SPDUTXCONFIRMATION, StbMFreshnessValueId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateFreshnessValueId(STBM_SID_SPDUTXCONFIRMATION, StbMFreshnessValueId))
    {
        StbM_FreshnessSPduTxConfirmation(StbMFreshnessValueId);
    }
#endif

    StbM_ArtiApiReturn(1, STBM_SID_SPDUTXCONFIRMATION);
    return;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

/**
 * The implementation of StbM_GetRxFreshness.
 */
/* PRQA S 6040 ++ */ /* VL_MTR_StbM_STPAR */
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
Std_ReturnType StbM_GetRxFreshness(
    uint16       StbMFreshnessValueId,
    const uint8* StbMTruncatedFreshnessValue,
    uint32       StbMTruncatedFreshnessValueLength,
    uint16       StbMAuthVerifyAttempts,
    uint8*       StbMFreshnessValue,
    uint32*      StbMFreshnessValueLength)
{
    StbM_ArtiApiStart(
        7,
        STBM_SID_GETRXFRESHNESS,
        StbMFreshnessValueId,
        StbMTruncatedFreshnessValue,
        StbMTruncatedFreshnessValueLength,
        StbMAuthVerifyAttempts,
        StbMFreshnessValue,
        StbMFreshnessValueLength);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateFreshnessValueId(STBM_SID_GETRXFRESHNESS, StbMFreshnessValueId)
        && StbM_ValidatePointer(STBM_SID_GETRXFRESHNESS, StbMFreshnessValue)
        && StbM_ValidatePointer(STBM_SID_GETRXFRESHNESS, StbMFreshnessValueLength))
    {
        Std_ReturnType ret = StbM_FreshnessGetRxData(
            StbMFreshnessValueId,
            StbMTruncatedFreshnessValue,
            StbMTruncatedFreshnessValueLength,
            StbMAuthVerifyAttempts,
            StbMFreshnessValue,
            StbMFreshnessValueLength);
        StbM_ArtiApiReturn(2, STBM_SID_GETRXFRESHNESS, ret);
        return ret;
    }
#endif

    StbM_ArtiApiReturn(2, STBM_SID_GETRXFRESHNESS, E_NOT_OK);
    return E_NOT_OK;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
/* PRQA S 6040 -- */
#endif

#if STBM_CAN_SLAVE_TIME_VALIDATION_USED == STD_ON
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/** The implementation of StbM_CanSetSlaveTimingData. */
Std_ReturnType StbM_CanSetSlaveTimingData(
    StbM_SynchronizedTimeBaseType           timeBaseId,
    const StbM_CanTimeSlaveMeasurementType* measureDataPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    StbM_ArtiApiStart(3, STBM_SID_CANSETSLAVETIMINGDATA, timeBaseId, measureDataPtr);
    uint16 timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommon(STBM_SID_CANSETSLAVETIMINGDATA, timeBaseId, timeBaseIndex)
        && StbM_ValidatePointer(STBM_SID_CANSETSLAVETIMINGDATA, measureDataPtr))
#endif
    {
        const StbM_SynchronizedTimeBaseCfgType* timeBaseCfgPtr      = &TIMEBASE_CFG(timeBaseIndex);
        StbM_CanSlaveMeasurementQueueType*      measurementQueuePtr = timeBaseCfgPtr->CanSlaveMeasurementQueuePtr;
        StbM_TimeValidataionDataInfoType*       timevalidationDataInfoPtr =
            &StbM_TimeBaseRuntime[timeBaseIndex].CanSlaveTimeValidationDataInfo;
        if (measurementQueuePtr != NULL_PTR)
        {
            StbM_CanTimeSlaveMeasurementType* sentinelMeasureDataPtr =
                &measurementQueuePtr->Data[timeBaseCfgPtr->TimeValidationRecordTableBlockCount];
            SchM_Enter_StbM_TimeValidation();
            StbM_CanTimeSlaveMeasurementType* nextMeasureDataPtr =
                &measurementQueuePtr->Data
                     [(timevalidationDataInfoPtr->Front + timevalidationDataInfoPtr->Size)
                      % timeBaseCfgPtr->TimeValidationRecordTableBlockCount];

            (void)IStdLib_MemCpy(nextMeasureDataPtr, measureDataPtr, sizeof(StbM_CanTimeSlaveMeasurementType));
            nextMeasureDataPtr->referenceLocalTimestamp  = sentinelMeasureDataPtr->referenceLocalTimestamp;
            nextMeasureDataPtr->referenceGlobalTimestamp = sentinelMeasureDataPtr->referenceGlobalTimestamp;

            if (timevalidationDataInfoPtr->Size < timeBaseCfgPtr->TimeValidationRecordTableBlockCount)
            {
                timevalidationDataInfoPtr->Size = timevalidationDataInfoPtr->Size + 1u;
            }
            else
            {
                timevalidationDataInfoPtr->Front =
                    (timevalidationDataInfoPtr->Front + 1u) % timeBaseCfgPtr->TimeValidationRecordTableBlockCount;
            }
            SchM_Exit_StbM_TimeValidation();
            ret = E_OK;
        }
    }
    StbM_ArtiApiReturn(2, STBM_SID_CANSETSLAVETIMINGDATA, ret);
    return ret;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#if STBM_CAN_MASTER_TIME_VALIDATION_USED == STD_ON
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/** The implementation of StbM_CanSetMasterTimingData. */
Std_ReturnType StbM_CanSetMasterTimingData(
    StbM_SynchronizedTimeBaseType            timeBaseId,
    const StbM_CanTimeMasterMeasurementType* measureDataPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    StbM_ArtiApiStart(3, STBM_SID_CANSETMASTERTIMINGDATA, timeBaseId, measureDataPtr);
    uint16 timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommon(STBM_SID_CANSETMASTERTIMINGDATA, timeBaseId, timeBaseIndex)
        && StbM_ValidatePointer(STBM_SID_CANSETMASTERTIMINGDATA, measureDataPtr))
#endif
    {
        const StbM_SynchronizedTimeBaseCfgType* timeBaseCfgPtr      = &TIMEBASE_CFG(timeBaseIndex);
        StbM_CanMasterMeasurementQueueType*     measurementQueuePtr = timeBaseCfgPtr->CanMasterMeasurementQueuePtr;
        StbM_TimeValidataionDataInfoType*       timevalidationDataInfoPtr =
            &StbM_TimeBaseRuntime[timeBaseIndex].CanMasterTimeValidationDataInfo;

        if (measurementQueuePtr != NULL_PTR)
        {
            SchM_Enter_StbM_TimeValidation();
            StbM_CanTimeMasterMeasurementType* nextMeasureDataPtr =
                &measurementQueuePtr->Data
                     [(timevalidationDataInfoPtr->Front + timevalidationDataInfoPtr->Size)
                      % timeBaseCfgPtr->TimeValidationRecordTableBlockCount];

            (void)IStdLib_MemCpy(nextMeasureDataPtr, measureDataPtr, sizeof(StbM_CanTimeMasterMeasurementType));

            if (timevalidationDataInfoPtr->Size < timeBaseCfgPtr->TimeValidationRecordTableBlockCount)
            {
                timevalidationDataInfoPtr->Size = timevalidationDataInfoPtr->Size + 1u;
            }
            else
            {
                timevalidationDataInfoPtr->Front =
                    (timevalidationDataInfoPtr->Front + 1u) % timeBaseCfgPtr->TimeValidationRecordTableBlockCount;
            }
            SchM_Exit_StbM_TimeValidation();
            ret = E_OK;
        }
    }
    StbM_ArtiApiReturn(2, STBM_SID_CANSETMASTERTIMINGDATA, ret);
    return ret;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#if STBM_ETH_SLAVE_TIME_VALIDATION_USED == STD_ON
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/** The implementation of StbM_EthSetSlaveTimingData. */
Std_ReturnType StbM_EthSetSlaveTimingData(
    StbM_SynchronizedTimeBaseType           timeBaseId,
    const StbM_EthTimeSlaveMeasurementType* measureDataPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    StbM_ArtiApiStart(3, STBM_SID_ETHSETSLAVETIMINGDATA, timeBaseId, measureDataPtr);
    uint16 timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommon(STBM_SID_ETHSETSLAVETIMINGDATA, timeBaseId, timeBaseIndex)
        && StbM_ValidatePointer(STBM_SID_ETHSETSLAVETIMINGDATA, measureDataPtr))
#endif
    {
        const StbM_SynchronizedTimeBaseCfgType* timeBaseCfgPtr      = &TIMEBASE_CFG(timeBaseIndex);
        StbM_EthSlaveMeasurementQueueType*      measurementQueuePtr = timeBaseCfgPtr->EthSlaveMeasurementQueuePtr;
        StbM_TimeValidataionDataInfoType*       timevalidationDataInfoPtr =
            &StbM_TimeBaseRuntime[timeBaseIndex].EthSlaveTimeValidationDataInfo;
        if (measurementQueuePtr != NULL_PTR)
        {
            StbM_EthTimeSlaveMeasurementType* sentinelMeasureDataPtr =
                &measurementQueuePtr->Data[timeBaseCfgPtr->TimeValidationRecordTableBlockCount];
            SchM_Enter_StbM_TimeValidation();
            StbM_EthTimeSlaveMeasurementType* nextMeasureDataPtr =
                &measurementQueuePtr->Data
                     [(timevalidationDataInfoPtr->Front + timevalidationDataInfoPtr->Size)
                      % timeBaseCfgPtr->TimeValidationRecordTableBlockCount];

            (void)IStdLib_MemCpy(nextMeasureDataPtr, measureDataPtr, sizeof(StbM_EthTimeSlaveMeasurementType));
            nextMeasureDataPtr->referenceLocalTimestamp  = sentinelMeasureDataPtr->referenceLocalTimestamp;
            nextMeasureDataPtr->referenceGlobalTimestamp = sentinelMeasureDataPtr->referenceGlobalTimestamp;

            if (timevalidationDataInfoPtr->Size < timeBaseCfgPtr->TimeValidationRecordTableBlockCount)
            {
                timevalidationDataInfoPtr->Size = timevalidationDataInfoPtr->Size + 1u;
            }
            else
            {
                timevalidationDataInfoPtr->Front =
                    (timevalidationDataInfoPtr->Front + 1u) % timeBaseCfgPtr->TimeValidationRecordTableBlockCount;
            }
            SchM_Exit_StbM_TimeValidation();
            ret = E_OK;
        }
    }
    StbM_ArtiApiReturn(2, STBM_SID_ETHSETSLAVETIMINGDATA, ret);
    return ret;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/** The implementation of StbM_EthSetPdelayInitiatorData. */
Std_ReturnType StbM_EthSetPdelayInitiatorData(
    StbM_SynchronizedTimeBaseType              timeBaseId,
    const StbM_PdelayInitiatorMeasurementType* measureDataPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    StbM_ArtiApiStart(3, STBM_SID_ETHTSETPDELAYINITIATORDATA, timeBaseId, measureDataPtr);
    uint16 timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommon(STBM_SID_ETHTSETPDELAYINITIATORDATA, timeBaseId, timeBaseIndex)
        && StbM_ValidatePointer(STBM_SID_ETHTSETPDELAYINITIATORDATA, measureDataPtr))
#endif
    {
        const StbM_SynchronizedTimeBaseCfgType*   timeBaseCfgPtr = &TIMEBASE_CFG(timeBaseIndex);
        StbM_PdelayInitiatorMeasurementQueueType* measurementQueuePtr =
            timeBaseCfgPtr->PdelayInitiatorMeasurementQueuePtr;
        StbM_TimeValidataionDataInfoType* timevalidationDataInfoPtr =
            &StbM_TimeBaseRuntime[timeBaseIndex].PdelayInitiatorValidationDataInfo;
        if (measurementQueuePtr != NULL_PTR)
        {
            SchM_Enter_StbM_TimeValidation();
            StbM_PdelayInitiatorMeasurementType* nextMeasureDataPtr =
                &measurementQueuePtr->Data
                     [(timevalidationDataInfoPtr->Front + timevalidationDataInfoPtr->Size)
                      % timeBaseCfgPtr->TimeValidationRecordTableBlockCount];

            (void)IStdLib_MemCpy(nextMeasureDataPtr, measureDataPtr, sizeof(StbM_PdelayInitiatorMeasurementType));

            if (timevalidationDataInfoPtr->Size < timeBaseCfgPtr->TimeValidationRecordTableBlockCount)
            {
                timevalidationDataInfoPtr->Size = timevalidationDataInfoPtr->Size + 1u;
            }
            else
            {
                timevalidationDataInfoPtr->Front =
                    (timevalidationDataInfoPtr->Front + 1u) % timeBaseCfgPtr->TimeValidationRecordTableBlockCount;
            }
            SchM_Exit_StbM_TimeValidation();
            ret = E_OK;
        }
    }
    StbM_ArtiApiReturn(2, STBM_SID_ETHTSETPDELAYINITIATORDATA, ret);
    return ret;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#if STBM_ETH_MASTER_TIME_VALIDATION_USED == STD_ON
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/** The implementation of StbM_EthSetMasterTimingData. */
Std_ReturnType StbM_EthSetMasterTimingData(
    StbM_SynchronizedTimeBaseType            timeBaseId,
    const StbM_EthTimeMasterMeasurementType* measureDataPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    StbM_ArtiApiStart(3, STBM_SID_ETHTSETMASTERTIMINGDATA, timeBaseId, measureDataPtr);
    uint16 timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommon(STBM_SID_ETHTSETMASTERTIMINGDATA, timeBaseId, timeBaseIndex)
        && StbM_ValidatePointer(STBM_SID_ETHTSETMASTERTIMINGDATA, measureDataPtr))
#endif
    {
        const StbM_SynchronizedTimeBaseCfgType* timeBaseCfgPtr      = &TIMEBASE_CFG(timeBaseIndex);
        StbM_EthMasterMeasurementQueueType*     measurementQueuePtr = timeBaseCfgPtr->EthMasterMeasurementQueuePtr;
        StbM_TimeValidataionDataInfoType*       timevalidationDataInfoPtr =
            &StbM_TimeBaseRuntime[timeBaseIndex].EthMasterTimeValidationDataInfo;
        if (measurementQueuePtr != NULL_PTR)
        {
            SchM_Enter_StbM_TimeValidation();
            StbM_EthTimeMasterMeasurementType* nextMeasureDataPtr =
                &measurementQueuePtr->Data
                     [(timevalidationDataInfoPtr->Front + timevalidationDataInfoPtr->Size)
                      % timeBaseCfgPtr->TimeValidationRecordTableBlockCount];

            (void)IStdLib_MemCpy(nextMeasureDataPtr, measureDataPtr, sizeof(StbM_EthTimeMasterMeasurementType));

            if (timevalidationDataInfoPtr->Size < timeBaseCfgPtr->TimeValidationRecordTableBlockCount)
            {
                timevalidationDataInfoPtr->Size = timevalidationDataInfoPtr->Size + 1u;
            }
            else
            {
                timevalidationDataInfoPtr->Front =
                    (timevalidationDataInfoPtr->Front + 1u) % timeBaseCfgPtr->TimeValidationRecordTableBlockCount;
            }
            SchM_Exit_StbM_TimeValidation();
            ret = E_OK;
        }
    }
    StbM_ArtiApiReturn(2, STBM_SID_ETHTSETMASTERTIMINGDATA, ret);
    return ret;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/** The implementation of StbM_EthSetPdelayResponderData. */
Std_ReturnType StbM_EthSetPdelayResponderData(
    StbM_SynchronizedTimeBaseType              timeBaseId,
    const StbM_PdelayResponderMeasurementType* measureDataPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    StbM_ArtiApiStart(3, STBM_SID_ETHTSETPDELAYRESPONDERDATA, timeBaseId, measureDataPtr);
    uint16 timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommon(STBM_SID_ETHTSETPDELAYRESPONDERDATA, timeBaseId, timeBaseIndex)
        && StbM_ValidatePointer(STBM_SID_ETHTSETPDELAYRESPONDERDATA, measureDataPtr))
#endif
    {
        const StbM_SynchronizedTimeBaseCfgType*   timeBaseCfgPtr = &TIMEBASE_CFG(timeBaseIndex);
        StbM_PdelayResponderMeasurementQueueType* measurementQueuePtr =
            timeBaseCfgPtr->PdelayResponderMeasurementQueuePtr;
        StbM_TimeValidataionDataInfoType* timevalidationDataInfoPtr =
            &StbM_TimeBaseRuntime[timeBaseIndex].PdelayResponderValidationDataInfo;
        if (measurementQueuePtr != NULL_PTR)
        {
            SchM_Enter_StbM_TimeValidation();
            StbM_PdelayResponderMeasurementType* nextMeasureDataPtr =
                &measurementQueuePtr->Data
                     [(timevalidationDataInfoPtr->Front + timevalidationDataInfoPtr->Size)
                      % timeBaseCfgPtr->TimeValidationRecordTableBlockCount];

            (void)IStdLib_MemCpy(nextMeasureDataPtr, measureDataPtr, sizeof(StbM_PdelayResponderMeasurementType));

            if (timevalidationDataInfoPtr->Size < timeBaseCfgPtr->TimeValidationRecordTableBlockCount)
            {
                timevalidationDataInfoPtr->Size = timevalidationDataInfoPtr->Size + 1u;
            }
            else
            {
                timevalidationDataInfoPtr->Front =
                    (timevalidationDataInfoPtr->Front + 1u) % timeBaseCfgPtr->TimeValidationRecordTableBlockCount;
            }
            SchM_Exit_StbM_TimeValidation();
            ret = E_OK;
        }
    }
    StbM_ArtiApiReturn(2, STBM_SID_ETHTSETPDELAYRESPONDERDATA, ret);
    return ret;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#if (STBM_NOTIFICATION_CUSTOMER_NUM > 0)
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief       When the timer time is up, perform a callback.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace   CPD-71870
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
            StbM_TimeDiffType timeDiff;
            uint16            timeBaseIndex = StbM_FindSynchronizedTimeBase(customerPtr->TimeBaseId);

            StbM_Time64TupleType time64Tuple;
            StbM_GetCurrentTimeTuple(timeBaseIndex, &time64Tuple);

            /* if TL is not available, timeDiff may be StbM_TimeDiffType_LowerLimit. */
            if (notificationPtr->CustomerTimerExpireTime >= time64Tuple.TL)
            {
                timeDiff = ((notificationPtr->CustomerTimerExpireTime - time64Tuple.TL)
                            >= (uint64)StbM_TimeDiffType_UpperLimit)
                               ? StbM_TimeDiffType_UpperLimit
                               : (StbM_TimeDiffType)(notificationPtr->CustomerTimerExpireTime - time64Tuple.TL);
            }
            else
            {
                timeDiff = ((time64Tuple.TL - notificationPtr->CustomerTimerExpireTime)
                            >= (uint64)(-StbM_TimeDiffType_LowerLimit))
                               ? StbM_TimeDiffType_LowerLimit
                               : -(StbM_TimeDiffType)(time64Tuple.TL - notificationPtr->CustomerTimerExpireTime);
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
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
/**
 * The implementation of StbM_MainFunction.
 */
void StbM_MainFunction(void)
/* PRQA S 1532 -- */
{
    StbM_ArtiApiStart(1, STBM_SID_MAINFUNCTION);
    if (StbM_InitStatus[StbM_GetPartitionIndex()] == TRUE)
    {
#if STBM_MULTIPLE_PARTITION_ENABLED == STD_ON
        ApplicationType applicationID = GetApplicationID();
#endif
        for (uint16 index = 0u; index < STBM_TIME_BASE_NUM; index++)
        {
#if STBM_MULTIPLE_PARTITION_ENABLED == STD_ON
            if (applicationID == TIMEBASE_CFG(index).ApplicationId)
            {
#endif
                /* Periodic reading of tV and tVFallback prevents GPT counter or OS counter overflow.
                 * For timebases using ETH or CAN timestamp, this read operation has no practical significance and is
                 * solely for keeping the code clean.
                 */
                StbM_Time64TupleType time64Tuple;
                StbM_GetCurrentTimeTuple(index, &time64Tuple);
                if (time64Tuple.TV != STBM_TIME_64_INVALID)
                {
                    StbM_CheckTimeout(index, &time64Tuple.TV); /* SWS_StbM_00540 */
                }

                StbM_CycleTaskHandle(index, &time64Tuple);

#if STBM_MULTIPLE_PARTITION_ENABLED == STD_ON
            }
#endif

#if STBM_MULTIPLE_PARTITION_ENABLED == STD_ON
            if (applicationID == STBM_ECUC_PARTITION)
            {
#endif
#if STBM_STATUS_NOTIFICATIONS_ENABLED == STD_ON
                StbM_CheckStatusNotification(index);
#endif

#if (STBM_TIME_RECORDING_SUPPORT == STD_ON)
                StbM_TimeRecordback(index);
#endif

#if STBM_TIME_VALIDATION_USED == STD_ON
                StbM_TimeBaseProviderNotification(index);
#endif

#if STBM_MULTIPLE_PARTITION_ENABLED == STD_ON
            }
#endif
        }

#if STBM_MULTIPLE_PARTITION_ENABLED == STD_ON
        if (applicationID == STBM_ECUC_PARTITION)
        {
#endif

#if (STBM_NOTIFICATION_CUSTOMER_NUM > 0u)
            StbM_TimerNotificationCustomer();
#endif

#if (STBM_TRIGGERED_CUSTOMER_NUM > 0u)
            StbM_TriggeredCustomer();
#endif

#if STBM_MULTIPLE_PARTITION_ENABLED == STD_ON
        }
#endif
    }
    StbM_ArtiApiReturn(1, STBM_SID_MAINFUNCTION);
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#if STBM_ETHTSYN_ENABLE == STD_ON
/* PRQA S 1510 ++ */ /* VL_StbM_Redeclaration */
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
Std_ReturnType
    StbM_GetBusProtocolParam(StbM_SynchronizedTimeBaseType timeBaseId, StbM_ProtocolParamType* protocolParams)
/* PRQA S 1510 -- */
{
    Std_ReturnType ret = E_NOT_OK;
    StbM_ArtiApiStart(3, STBM_SID_GETPROTOCOLPARAM, timeBaseId, protocolParams);
    uint16 timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommon(STBM_SID_GETPROTOCOLPARAM, timeBaseId, timeBaseIndex)
        && StbM_ValidateSynchronizedTimeBase(STBM_SID_GETPROTOCOLPARAM, timeBaseIndex)
        && StbM_ValidatePointer(STBM_SID_GETPROTOCOLPARAM, protocolParams)
        && StbM_ValidatePartition(STBM_SID_GETPROTOCOLPARAM, timeBaseIndex))
#endif
    {
        if ((TIMEBASE_CFG(timeBaseIndex).LocalTimeClock->LocalTimeHardwareType == ETHTSYN_GLOBALTIME_DOMAIN)
            && (protocolParams->protocolType == STBM_TIMESYNC_ETHERNET))
        {
            ret = EthTSyn_GetProtocolParam(timeBaseId, protocolParams);
        }
    }
    StbM_ArtiApiReturn(2, STBM_SID_GETPROTOCOLPARAM, ret);
    return ret;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#if STBM_ETHTSYN_ENABLE == STD_ON
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
Std_ReturnType
    StbM_SetBusProtocolParam(StbM_SynchronizedTimeBaseType timeBaseId, const StbM_ProtocolParamType* protocolParams)
{
    Std_ReturnType ret = E_NOT_OK;
    StbM_ArtiApiStart(3, STBM_SID_SETPROTOCOLPARAM, timeBaseId, protocolParams);
    uint16 timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommon(STBM_SID_SETPROTOCOLPARAM, timeBaseId, timeBaseIndex)
        && StbM_ValidateSynchronizedTimeBase(STBM_SID_SETPROTOCOLPARAM, timeBaseIndex)
        && StbM_ValidatePointer(STBM_SID_SETPROTOCOLPARAM, protocolParams)
        && StbM_ValidatePartition(STBM_SID_SETPROTOCOLPARAM, timeBaseIndex))
#endif
    {
        if ((TIMEBASE_CFG(timeBaseIndex).LocalTimeClock->LocalTimeHardwareType == ETHTSYN_GLOBALTIME_DOMAIN)
            && (protocolParams->protocolType == STBM_TIMESYNC_ETHERNET))
        {
            ret = EthTSyn_SetProtocolParam(timeBaseId, protocolParams);
        }
    }

    StbM_ArtiApiReturn(2, STBM_SID_SETPROTOCOLPARAM, ret);
    return ret;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#if STBM_CLONE_TIMEBASE_USED == STD_ON
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
Std_ReturnType StbM_SetCloningOffset(StbM_SynchronizedTimeBaseType timeBaseId, const StbM_TimeStampType* timeOffset)
{
    Std_ReturnType ret = E_NOT_OK;
    StbM_ArtiApiStart(3, STBM_SID_SETCLONINGOFFSET, timeBaseId, timeOffset);
    uint16 timeBaseIdx = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommon(STBM_SID_SETCLONINGOFFSET, timeBaseId, timeBaseIdx)
        && StbM_ValidateTimeBaseCloning(STBM_SID_SETCLONINGOFFSET, timeBaseIdx)
        && StbM_ValidatePointer(STBM_SID_SETCLONINGOFFSET, timeOffset))
#endif
    {
        const StbM_SynchronizedTimeBaseCfgType* timeBaseCfgPtr = &TIMEBASE_CFG(timeBaseIdx);
        StbM_CloneRequestType*                  cloneReqestPtr = timeBaseCfgPtr->CloneRequestPtr;

        cloneReqestPtr->TimeOffset = StbM_GTTimeTo64Time(timeOffset);
        ret                        = E_OK;
    }
    StbM_ArtiApiReturn(2, STBM_SID_SETCLONINGOFFSET, ret);
    return ret;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
Std_ReturnType StbM_CloneTimeBase(
    StbM_SynchronizedTimeBaseType timeBaseId,
    StbM_CloneConfigType          cloneCfg,
    StbM_TimeBaseStatusType       statusMask,
    StbM_TimeBaseStatusType       statusValue)
{
    Std_ReturnType ret = E_NOT_OK;
    StbM_ArtiApiStart(5, STBM_SID_CLONETIMEBASE, timeBaseId, cloneCfg, statusMask, statusValue);
    uint16 timeBaseIdx = StbM_FindSynchronizedTimeBase(timeBaseId);

#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommon(STBM_SID_CLONETIMEBASE, timeBaseId, timeBaseIdx)
        && StbM_ValidateTimeBaseCloning(STBM_SID_CLONETIMEBASE, timeBaseIdx)
        && StbM_ValidatePartition(STBM_SID_CLONETIMEBASE, timeBaseIdx))
#endif
    {
        const StbM_SynchronizedTimeBaseCfgType* timeBaseCfgPtr = &TIMEBASE_CFG(timeBaseIdx);
        StbM_TimeBaseType*                      timeBaseVarPtr = &StbM_TimeBaseRuntime[timeBaseIdx];
        if ((timeBaseVarPtr->TimeBaseStatus & statusMask) == statusValue)
        {
            StbM_CloneRequestType* cloneRequestPtr = timeBaseCfgPtr->CloneRequestPtr;

            cloneRequestPtr->Pending  = TRUE;
            cloneRequestPtr->CloneCfg = cloneCfg;

            /*SWS_StbM_00533, SWS_StbM_00530*/
            if ((cloneCfg & STBM_CLONE_CONFIG_DEFERRED_COPY) == 0u)
            {
                StbM_CloneTimeBaseCopy(timeBaseIdx, timeBaseCfgPtr->SrcTimeBaseIdx);
            }
            ret = E_OK;
        }
    }
    StbM_ArtiApiReturn(2, STBM_SID_CLONETIMEBASE, ret);
    return ret;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief Copy the timebase data from source to destination
 * @param[in] destTimeBaseIdx Destination timebase index
 * @param[in] srcTimeBaseIdx Source timebase index
 * @return void
 * @reentrant    FALSE
 * @synchronous  TRUE
 * @trace   CPD-85969
 */
STBM_LOCAL void StbM_CloneTimeBaseCopy(uint16 destTimeBaseIdx, uint16 srcTimeBaseIdx)
{
    const StbM_SynchronizedTimeBaseCfgType* destTimeBaseCfgPtr = &TIMEBASE_CFG(destTimeBaseIdx);
    StbM_CloneRequestType*                  cloneRequestPtr    = destTimeBaseCfgPtr->CloneRequestPtr;

    StbM_TimeBaseType* destTimeBaseVarPtr = &StbM_TimeBaseRuntime[destTimeBaseIdx];

    if (cloneRequestPtr->Pending)
    {
        StbM_TimeBaseType* srcTimeBaseVarPtr = &StbM_TimeBaseRuntime[srcTimeBaseIdx];
        if ((srcTimeBaseVarPtr->TimeBaseStatus & STBM_TIMEBASE_STATUS_TIMEOUT) == 0u)
        {
            uint64 timeOffset = 0ULL;
            if ((cloneRequestPtr->CloneCfg & STBM_CLONE_CONFIG_APPLY_OFFSET) != 0u)
            {
                timeOffset = cloneRequestPtr->TimeOffset;
            }

            StbM_Time64TupleType srcTime64Tuple;
            StbM_GetCurrentTimeTuple(srcTimeBaseIdx, &srcTime64Tuple);
            if (srcTime64Tuple.TL != STBM_TIME_64_INVALID)
            {
                StbM_TimeStampType timeStamp;
                StbM_64TimeToGTTime(srcTime64Tuple.TL + timeOffset, &timeStamp);
                StbM_SetGlobalTimePrivate(
                    destTimeBaseCfgPtr->SynchronizedTimeBaseIdentifier,
                    &timeStamp,
                    &srcTimeBaseVarPtr->UserData,
                    STBM_SID_CLONETIMEBASE,
                    (cloneRequestPtr->CloneCfg & STBM_CLONE_CONFIG_IMMEDIATE_TX) != 0u);

                /* SWS_StbM_00535 */
                if (((cloneRequestPtr->CloneCfg & STBM_CLONE_CONFIG_APPLY_RATE) != 0u)
                    && (destTimeBaseCfgPtr->TimeCorrection != NULL_PTR)
                    && (destTimeBaseCfgPtr->TimeCorrection->AllowMasterRateCorrection))
                {
                    SchM_Enter_StbM_Context();
                    if ((srcTimeBaseVarPtr->TimeBaseStatus & STBM_TIMEBASE_STATUS_RATE_CORRECTED) != 0u)
                    {
                        destTimeBaseVarPtr->RateDeviation.IsSet    = srcTimeBaseVarPtr->RateDeviation.IsSet;
                        destTimeBaseVarPtr->RateDeviation.RrcValue = srcTimeBaseVarPtr->RateDeviation.RrcValue;
                        destTimeBaseVarPtr->RateDeviation.RocValue = srcTimeBaseVarPtr->RateDeviation.RocValue;
                        StbM_TimeBaseStatusBitSet(destTimeBaseVarPtr, STBM_TIMEBASE_STATUS_RATE_CORRECTED, TRUE);
                    }
                    else
                    {
                        destTimeBaseVarPtr->RateDeviation.IsSet    = FALSE;
                        destTimeBaseVarPtr->RateDeviation.RrcValue = STBM_RATE_PPM;
                        destTimeBaseVarPtr->RateDeviation.RocValue = 0;
                        StbM_TimeBaseStatusBitSet(destTimeBaseVarPtr, STBM_TIMEBASE_STATUS_RATE_CORRECTED, FALSE);
                    }
                    SchM_Exit_StbM_Context();
                }
            }
        }

        if ((cloneRequestPtr->CloneCfg & STBM_CLONE_CONFIG_RETRIGGER_DEFERRED_COPY) == 0u)
        {
            cloneRequestPtr->Pending = FALSE;
        }
    }
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

/* ========================================== internal function definitions ========================================= */
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL uint64 StbM_ProcessBusTimeCorrection(
    StbM_SynchronizedTimeBaseType timeBaseId,
    StbM_Time64TupleType          time64Tuple,
    const StbM_TimeTupleType*     timeTuplePtr,
    uint64                        tGRx,
    const StbM_MeasurementType*   measureDataPtr)
{
    uint16             timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
    StbM_TimeBaseType* timeBasePtr   = &StbM_TimeBaseRuntime[timeBaseIndex];
    uint64             tVRx          = StbM_LTTimeTo64Time(timeTuplePtr->virtualLocalTime);
    /* SWS_StbM_00529 */
    uint64 tSPD = time64Tuple.TV - tVRx;
#if (STBM_TIME_RECORDING_SUPPORT == STD_ON) || (STBM_TIME_CORRECTION_SUPPORT == STD_ON)
    uint64 tVURx = time64Tuple.TV;
#endif
    /* PRQA S 4119, 4395 ++ */ /* VL_StbM_4119, VL_StbM_4395 */
    uint64 tLURx =
        tGRx + (uint64)(((float64)(timeBasePtr->RateDeviation.RrcValue) / (float64)STBM_RATE_PPM) * (float64)tSPD);
    (void)timeTuplePtr;
    /* SWS_StbM_00438 SWS_StbM_00609
     * If TVMain, tVFallback, and TVFallbackMain are all invalid, the calculated TL_Sync will be an invalid
     * value, which will result in a time leap past.
     */
    StbM_CheckTimeleap(timeBaseIndex, time64Tuple.TL, tLURx);

    if (timeBasePtr->BusNotFirstSet == FALSE)
    {
        timeBasePtr->BusNotFirstSet = TRUE;
    }
#if (STBM_TIME_RECORDING_SUPPORT == STD_ON)
    StbM_TimeRecord(
        timeBaseId,
        time64Tuple.TL,
        tLURx,
        tVURx,
        measureDataPtr->pathDelay); /* SWS_StbM_00313 SWS_StbM_00314 */
#else
    STBM_UNUSED(measureDataPtr);
#endif

#if (STBM_TIME_CORRECTION_SUPPORT == STD_ON)
    if (TIMEBASE_CFG(timeBaseIndex).TimeCorrection != NULL_PTR)
    {
        StbM_RateSourceType rateSource = TIMEBASE_CFG(timeBaseIndex).TimeCorrection->RateSource;
        if (rateSource == RATE_SOURCE_TIMESYNC_MODULE)
        {
            if (measureDataPtr->rateDeviation.rateDeviationStatus == RATE_OK)
            {
                /** SWS_StbM_00576 */
                timeBasePtr->RateDeviation.RrcValue =
                    (sint32)((measureDataPtr->rateDeviation.rateDeviationValue / STBM_POW2_41) + 1LL);
                timeBasePtr->RateDeviation.IsSet = TRUE;
                timeBasePtr->TimeBaseStatus |= (StbM_TimeBaseStatusType)STBM_TIMEBASE_STATUS_RATE_CORRECTED;
            }
        }
        else if (rateSource == RATE_SOURCE_STBM)
        {
            /** SWS_StbM_00377 */
            StbM_CalculateRrc(timeBaseIndex, &tLURx, &tVURx, timeTuplePtr->timeBaseStatus);
        }
        StbM_CalculateRoc(timeBaseIndex, &tLURx, time64Tuple.TL);
    }
#endif
    return tLURx;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL Std_ReturnType StbM_ProcessValidBusTimeUpdate(
    StbM_SynchronizedTimeBaseType timeBaseId,
    StbM_Time64TupleType*         time64Tuple,
    const StbM_MeasurementType*   measureDataPtr,
    const StbM_TimeTupleType*     timeTuplePtr,
    const StbM_UserDataType*      userDataPtr)
{
    uint16 timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);

    StbM_TimeBaseType* timeBasePtr = &StbM_TimeBaseRuntime[timeBaseIndex];
#if STBM_DISCIPLINED_CLOCK_USED == STD_ON
    const StbM_SynchronizedTimeBaseCfgType* timeBaseCfgPtr = &TIMEBASE_CFG(timeBaseIndex);
#endif
    uint64 tGRx = StbM_GTTimeTo64Time((const StbM_TimeStampType*)&timeTuplePtr->globalTime);
    SchM_Enter_StbM_Context();
    StbM_TimeBaseStatusBitSet(timeBasePtr, STBM_TIMEBASE_STATUS_GLOBAL_TIME_BASE, TRUE);
    timeBasePtr->LastBusSettime = time64Tuple->TV;
    SchM_Exit_StbM_Context();

    /* PRQA S 4119, 4395 -- */
    uint64 tLURx = StbM_ProcessBusTimeCorrection(timeBaseId, *time64Tuple, timeTuplePtr, tGRx, measureDataPtr);

    return StbM_ApplyTimeBaseUpdate(timeBaseId, timeTuplePtr, time64Tuple, tLURx, userDataPtr);
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL Std_ReturnType StbM_ApplyTimeBaseUpdate(
    StbM_SynchronizedTimeBaseType timeBaseId,
    const StbM_TimeTupleType*     timeTuplePtr,
    StbM_Time64TupleType*         time64Tuple,
    uint64                        tLURx,
    const StbM_UserDataType*      userDataPtr)
{
    uint16             timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
    StbM_TimeBaseType* timeBasePtr   = &StbM_TimeBaseRuntime[timeBaseIndex];
    Std_ReturnType     ret           = E_OK;
    /* SWS_StbM_00184 */
    SchM_Enter_StbM_Context();
    StbM_TimeBaseStatusBitSet(
        timeBasePtr,
        STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY,
        ((timeTuplePtr->timeBaseStatus) & STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY) != 0u);

#if STBM_DISCIPLINED_CLOCK_USED == STD_ON
    const StbM_SynchronizedTimeBaseCfgType* timeBaseCfgPtr   = &TIMEBASE_CFG(timeBaseIndex);
    const StbM_DisciplinedClockCfgType*     disciplinedClock = timeBaseCfgPtr->DisciplinedClock;
    if (disciplinedClock != NULL_PTR)
    {
        TimeStampType timeStamp;
        StbM_64TimeToGTTime(tLURx, &timeStamp);
        ret = EthIf_SetPhcTime(disciplinedClock->CtrlIdx, disciplinedClock->ClkUnitIdx, &timeStamp);
    }
    else
#endif
    {
        ret = E_OK;
    }
    /* SWS_StbM_00436 SWS_StbM_00433 */
    time64Tuple->TL = tLURx;
    StbM_UpdateMainTimeTuple(timeBaseIndex, time64Tuple);

    /* SWS_StbM_00183 */
    StbM_TimeBaseStatusBitSet(timeBasePtr, STBM_TIMEBASE_STATUS_TIMEOUT, FALSE);
#if STBM_STATUS_NOTIFICATIONS_ENABLED == STD_ON
    timeBasePtr->NotificationEvents |= STBM_TIMEBASE_EV_RESYNC;
#endif
    /* SWS_StbM_00350 */
    ++timeBasePtr->TimeBaseUpdateCounter;           /* Free-Running Counter */
    StbM_SetUserDataIn(timeBaseIndex, userDataPtr); /* SWS_StbM_00179 */
    SchM_Exit_StbM_Context();

#if STBM_CLONE_TIMEBASE_USED == STD_ON
    /* SWS_StbM_00532 */
    StbM_CloneTimeBaseDeferCopyHandler(timeBaseIndex);
#endif
    return ret;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#if (STBM_DEV_ERROR_DETECT == STD_ON)
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief Development error validation of StbM_Init
 * @param[in]    configPtr   StbM config data
 * @return       boolean
 * @retval       TRUE    Development error not occurred
 * @retval       FALSE   Development error occurred
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace   CPD-76917
 */
STBM_LOCAL boolean StbM_ValidateInit(const StbM_ConfigType* configPtr)
{
    uint16 partitionIndex = StbM_GetPartitionIndex();
#if STBM_MULTIPLE_PARTITION_ENABLED == STD_ON
    if (partitionIndex >= STBM_PARTITION_NUMBER)
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

    if (StbM_InitStatus[partitionIndex])
    {
        StbM_Det_Report(STBM_SID_INIT, STBM_E_ALREADY_INITIALIZED);
        return FALSE;
    }

    return TRUE;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief Development error validation of null pointer
 * @param[in]    apiId   ID of API service in which error is detected
 * @param[in]    pointer pointer
 * @return       boolean
 * @retval       TRUE    Development error not occurred
 * @retval       FALSE   Development error occurred
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace   CPD-75407
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
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
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
 * @trace   CPD-71834
 */
STBM_LOCAL boolean StbM_ValidateCommon(uint8 apiId, StbM_SynchronizedTimeBaseType timeBaseId, uint16 timeBaseIndex)
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

    uint16 partitionIndex = StbM_GetPartitionIndex();
#if STBM_MULTIPLE_PARTITION_ENABLED == STD_ON
    if (partitionIndex >= STBM_PARTITION_NUMBER)
    {
        StbM_Det_Report(apiId, STBM_E_INVALID_PARTITION);
        return FALSE;
    }
#endif

    if (!StbM_InitStatus[partitionIndex])
    {
        StbM_Det_Report(apiId, STBM_E_UNINIT);
        return FALSE;
    }

    return TRUE;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief        Check if this partition is allowed to be accessed
 * @param[in]    apiId      ID of API service in which error is detected
 * @param[in]    timeBaseIndex The index of the time base
 * @return       boolean
 * @retval       TRUE    Development error not occurred
 * @retval       FALSE   Development error occurred
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace   CPD-71832
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
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief       whether a nanosecond value is reasonable
 * @param[in]   apiId   ID of API service in which error is detected
 * @param[in]   nanoseconds    Nanosecond
 * @return      boolean
 * @retval      TRUE    Development error not occurred
 * @retval      FALSE   Development error occurred
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace   CPD-75408
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
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief       whether user data is reasonable
 * @param[in]   apiId       ID of API service in which error is detected
 * @param[in]   userDataPtr User data
 * @return      boolean
 * @retval      TRUE    Development error not occurred
 * @retval      FALSE   Development error occurred
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace   CPD-75409
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
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief       Whether is a wide global time master
 * @param[in]   apiId       ID of API service in which error is detected
 * @param[in]   timeBaseIndex The index of the time base
 * @return      boolean
 * @retval      TRUE    Development error not occurred
 * @retval      FALSE   Development error occurred
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace   CPD-75410
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
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief       Whether is a synchronized time base
 * @param[in]   apiId       ID of API service in which error is detected
 * @param[in]   timeBaseIndex The index of the time base
 * @return      boolean
 * @retval      TRUE    Development error not occurred
 * @retval      FALSE   Development error occurred
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace   CPD-75411
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
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#if STBM_QUERY_FRESHNESS_VALUE != STBM_QUERY_FRESHNESS_VALUE_NONE
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief       Whether is a identifier of the freshness value
 * @param[in]   apiId             ID of API service in which error is detected
 * @param[in]   freshnessValueId  Holds the identifier of the freshness value
 * @retval      TRUE    Development error not occurred
 * @retval      FALSE   Development error occurred
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace   CPD-85963
 */
STBM_LOCAL boolean StbM_ValidateFreshnessValueId(uint8 apiId, uint16 freshnessValueId)
{
    if (StbM_FreshnessGetConfig(freshnessValueId) == NULL_PTR)
    {
        StbM_Det_Report(apiId, STBM_E_PARAM);
        return FALSE;
    }
    return TRUE;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#if STBM_CLONE_TIMEBASE_USED == STD_ON
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief       Whether is a no clone source time base
 * @param[in]   apiId       ID of API service in which error is detected
 * @param[in]   timeBaseIdx The index of the time base
 * @return      boolean
 * @retval      TRUE    Development error not occurred
 * @retval      FALSE   Development error occurred
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace   CPD-85962
 */
STBM_LOCAL boolean StbM_ValidateTimeBaseCloning(uint8 apiId, uint16 timeBaseIdx)
{
    const StbM_SynchronizedTimeBaseCfgType* timeBaseCfgPtr = &TIMEBASE_CFG(timeBaseIdx);
    if (timeBaseCfgPtr->CloneRequestPtr == NULL_PTR)
    {
        StbM_Det_Report(apiId, STBM_E_SERVICE_NOT_SUPPORTED);
        return FALSE;
    }
    return TRUE;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#if (STBM_TIME_CORRECTION_SUPPORT == STD_ON)
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief       Whether allow master time base set a rate correction
 * @param[in]   apiId       ID of API service in which error is detected
 * @param[in]   timeBaseIndex The index of the time base
 * @return      boolean
 * @retval      TRUE    Development error not occurred
 * @retval      FALSE   Development error occurred
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace   CPD-75412
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
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#if (STBM_NOTIFICATION_CUSTOMER_NUM > 0)
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
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
 * @trace   CPD-75413
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
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif
#endif

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief       Returns the index of current partition in the CanTSyn configuration.
 * @return      uint16
 * @retval      The index of current partition in the CanTSyn configuration
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace   CPD-71831
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
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief       Find the index of the time base according to it's ID.
 * @param[in]   timeBaseId  Identification of a Time Base
 * @return      uint16
 * @retval      The time base index in the configuration
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace   CPD-71833
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
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief       initialize the time base refer by index.
 * @param[in]   index  index of a Time Base
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace   CPD-76918
 */
STBM_LOCAL void StbM_TimeBaseInit(uint16 index)
{
    StbM_TimeBaseType* timeBasePtr = &StbM_TimeBaseRuntime[index];
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
    timeBasePtr->TimeLeapValid      = FALSE;
    timeBasePtr->BusNotFirstSet     = FALSE;
    timeBasePtr->LastBusSettime     = 0ULL;
    timeBasePtr->MainTupleUpdateCnt = STBM_MAINTUPLE_HASNOT_UPDATE;

    uint64 backupTimeValue = 0ULL;
#if STBM_STORE_TIMEBASE_NONVOLATILE_USED == STD_ON
    timeBasePtr->BackupTimeCnt = 0ULL;
    StbM_ReadBackupTimeHandle(index, &backupTimeValue);
#endif

#if STBM_FALLBACK_VIRTUAL_LOCAL_TIME_USED == STD_ON
    timeBasePtr->MainTimeTuple.TL         = backupTimeValue;
    timeBasePtr->MainTimeTuple.TV         = 0ULL;
    timeBasePtr->MainTimeTuple.TVFallback = 0ULL;
#else
    timeBasePtr->MainTimeTuple.TL = backupTimeValue;
    timeBasePtr->MainTimeTuple.TV = 0ULL;
#endif
    timeBasePtr->TimeLeap           = 0;
    timeBasePtr->ClearTimeleapCount = 0u;

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
        StbM_ClockGptInit(localClockCfg->LocalTimeHardware);
    }
#if STBM_FALLBACK_VIRTUAL_LOCAL_TIME_USED == STD_ON
    const StbM_LocalTimeClockCfgType* fallbackClockCfg = timeBaseCfgPtr->FallbackTimeClock;
    if ((fallbackClockCfg != NULL_PTR) && (fallbackClockCfg->LocalTimeHardwareType == GPT_CHANNEL_CONFIGURATION))
    {
        StbM_ClockGptInit(fallbackClockCfg->LocalTimeHardware);
    }
#endif
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
        if (timeCorrectionCfgPtr->RateSource == RATE_SOURCE_STBM)
        {
            for (uint16 i = 0U; i <= timeCorrectionCfgPtr->RateCorrectionsPerMeasurementDuration; i++)
            {
                timeBasePtr->RateCorrection.Array[i].TGstart = 0ULL;
                timeBasePtr->RateCorrection.Array[i].TVstart = 0ULL;
            }
        }
    }
#endif

#if STBM_CAN_SLAVE_TIME_VALIDATION_USED == STD_ON || STBM_ETH_SLAVE_TIME_VALIDATION_USED == STD_ON
    /* RateValidationTimeTriple0 and RateValidationTimeTriplePrev will be assigned upon the first invocation of
     * StbM_BusSetGlobalTime. */
    timeBasePtr->RateValidationInterval = 0u; /* reset the calculation of rate validation. */
#endif

#if STBM_CLONE_TIMEBASE_USED == STD_ON
    StbM_CloneRequestType* cloneRequestPtr = timeBaseCfgPtr->CloneRequestPtr;
    if (cloneRequestPtr != NULL_PTR)
    {
        cloneRequestPtr->TimeOffset = 0ULL;
        cloneRequestPtr->CloneCfg   = 0U;
        cloneRequestPtr->Pending    = FALSE;
    }
#endif
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#if (STBM_NOTIFICATION_CUSTOMER_NUM > 0u)
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief       initialize the notification customers.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-76919
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
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief       Get the local time and disciplined clock time (if clock hardware is eth timestamp).
 * @param[in]   localClockCfg  The local hardware clock
 * @param[out]  tVPtr  Pointer to store the virtual time
 * @param[out]  tLPtr  Pointer to store the local Instance of the global time (disciplined clock time)
 * @return      The local time stamp, if the hardware counter is valid, otherwise STBM_TIME_64_INVALID
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace   CPD-71835
 */
STBM_LOCAL void
    StbM_LocalClockToLocalTime(const StbM_LocalTimeClockCfgType* localClockCfg, uint64* tVPtr, const uint64* tLPtr)
{
    switch (localClockCfg->LocalTimeHardwareType)
    {
#if STBM_CANIF_TIMESTAMP_SUPPORT == STD_ON
    /* SWS_StbM_00539 SWS_StbM_00594 */
    case CANTSYN_GLOBALTIME_DOMAIN:
    {
        Can_TimeStampType canTimeStamp;
        uint8             controller = localClockCfg->LocalTimeHardware;
        if (CanIf_GetCurrentTime(controller, &canTimeStamp) == E_OK)
        {
            /* Can_TimeStampType has no member named secondsHi, secondsHi of stbmTimeStamp is implicitly initialized to
             * 0 */
            StbM_TimeStampType timeStamp = {.nanoseconds = canTimeStamp.nanoseconds, .seconds = canTimeStamp.seconds};
            *tVPtr                       = StbM_GTTimeTo64Time(&timeStamp);
        }
        break;
    }
#endif

#if STBM_ETHIF_TIMESTAMP_SUPPORT == STD_ON
    /* SWS_StbM_00178 */
    case ETHTSYN_GLOBALTIME_DOMAIN:
    {
        uint8         ctrlIdx = localClockCfg->LocalTimeHardware;
        TimeTupleType ethTimeTuple;
        if ((EthIf_GetCurrentTimeTuple(ctrlIdx, localClockCfg->ClkUnitIdx, &ethTimeTuple) == E_OK)
            && (ethTimeTuple.timeQuality == TSQ_VALID))
        {
            StbM_TimeStampType timeStamp;
            timeStamp.nanoseconds = ethTimeTuple.timestampClockValue.nanoseconds;
            timeStamp.seconds     = ethTimeTuple.timestampClockValue.seconds;
            timeStamp.secondsHi   = ethTimeTuple.timestampClockValue.secondsHi;
            *tVPtr                = StbM_GTTimeTo64Time(&timeStamp);
#if STBM_DISCIPLINED_CLOCK_USED == STD_ON
            uint64* tLPtrTemp = (uint64*)tLPtr;
            if (tLPtrTemp != NULL_PTR)
            {
                timeStamp.nanoseconds = ethTimeTuple.disciplinedClockValue.nanoseconds;
                timeStamp.seconds     = ethTimeTuple.disciplinedClockValue.seconds;
                timeStamp.secondsHi   = ethTimeTuple.disciplinedClockValue.secondsHi;
                *tLPtrTemp            = StbM_GTTimeTo64Time(&timeStamp);
            }
#endif
        }
        break;
    }
#endif

#if STBM_GPT_TIMESTAMP_SUPPORT == STD_ON
    /* SWS_StbM_00512 */
    case GPT_CHANNEL_CONFIGURATION:
        StbM_ClockGptGetCurrentTime(localClockCfg->LocalTimeHardware, tVPtr);
        break;
#endif

#if STBM_OS_TIMESTAMP_SUPPORT == STD_ON
    case OS_COUNTER:
        StbM_ClockOsGetCurrentTime(localClockCfg->LocalTimeHardware, tVPtr);
        break;
#endif

    default:
        /* unreachable */
        break;
    }
    STBM_UNUSED(tLPtr);
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#if STBM_DISCIPLINED_CLOCK_USED == STD_ON
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL void StbM_GetDisciplinedClockTime(uint16 timeBaseIndex, uint64* tL, uint64* tV, uint64* tVFallback)
{
    const StbM_SynchronizedTimeBaseCfgType* timeBaseCfgPtr = &TIMEBASE_CFG(timeBaseIndex);

    const StbM_LocalTimeClockCfgType*   localTimeClock   = timeBaseCfgPtr->LocalTimeClock;
    const StbM_DisciplinedClockCfgType* disciplinedClock = timeBaseCfgPtr->DisciplinedClock;
    StbM_TimeBaseType*                  timeBaseVarPtr   = &StbM_TimeBaseRuntime[timeBaseIndex];

    if ((localTimeClock->LocalTimeHardwareType == ETHTSYN_GLOBALTIME_DOMAIN)
        && (localTimeClock->ClkUnitIdx == disciplinedClock->ClkUnitIdx))
    {
        /* SWS_StbM_00574 */
        StbM_LocalClockToLocalTime(localTimeClock, tV, tL);
    }
    else
    {
        /* SWS_StbM_00575
         * The sequence of the 2 calls, StbM_LocalClockToLocalTime and EthIf_GetPhcTime, shall not be interrupted.
         */
        StbM_LocalClockToLocalTime(localTimeClock, tV, NULL_PTR);
        StbM_TimeStampType timeStamp;
        TimeStampQualType  timeStampQual;
        if ((EthIf_GetPhcTime(disciplinedClock->CtrlIdx, disciplinedClock->ClkUnitIdx, &timeStampQual, &timeStamp)
             == E_OK)
            && (timeStampQual == TSQ_VALID))
        {
            *tL = StbM_GTTimeTo64Time(&timeStamp);
        }
    }
#if STBM_TIME_CORRECTION_SUPPORT == STD_ON
    if (timeBaseCfgPtr->TimeCorrection != NULL_PTR)
    {
        if ((*tV != STBM_TIME_64_INVALID) && (timeBaseVarPtr->MainTimeTuple.TV != STBM_TIME_64_INVALID))
        {
            uint64 diff     = *tV - timeBaseVarPtr->MainTimeTuple.TV;
            uint64 interval = timeBaseCfgPtr->TimeCorrection->OffsetCorrectionAdaptionInterval;
            /* when the Offset Correction Adaption Interval expires */
            if ((timeBaseVarPtr->RateDeviation.RocValue != 0L) && (diff >= interval))
            {
                sint64 ethRateDeviation =
                    (timeBaseVarPtr->RateDeviation.RrcValue - STBM_RATE_PPM) * STBM_POW2_41 / STBM_RATE_PPM;
                ethRateDeviation = StbM_Clamp(ethRateDeviation, STBM_SINT32_MIN, STBM_SINT32_MAX);
                (void)EthIf_SetPhcCorrection(
                    disciplinedClock->CtrlIdx,
                    disciplinedClock->ClkUnitIdx,
                    ethRateDeviation,
                    0);
                timeBaseVarPtr->RateDeviation.RocValue = 0;

                StbM_Time64TupleType time64Tuple = {
                    *tL,
                    *tV,
#if STBM_FALLBACK_VIRTUAL_LOCAL_TIME_USED == STD_ON
                    *tVFallback
#endif
                };
                StbM_UpdateMainTimeTuple(timeBaseIndex, &time64Tuple);
            }
        }
    }
#endif
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief         Reads the current time tuple [TL, TV, TVFallback] in an atomic operation
 * @param[in]     timeBaseIndex   The time base index in the configuration
 * @param[out]    time64TuplePtr  Time tuple [TL, TV, TVFallback]
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace   CPD-84910
 */
STBM_LOCAL void StbM_GetCurrentTimeTuple(uint16 timeBaseIndex, StbM_Time64TupleType* time64TuplePtr)
{
    const StbM_SynchronizedTimeBaseCfgType* timeBaseCfgPtr = &TIMEBASE_CFG(timeBaseIndex);
#if (STBM_FALLBACK_VIRTUAL_LOCAL_TIME_USED == STD_ON)
    StbM_TimeBaseType* timeBaseVarPtr = &StbM_TimeBaseRuntime[timeBaseIndex];
#endif
    uint64                            tL             = STBM_TIME_64_INVALID;
    uint64                            tV             = STBM_TIME_64_INVALID;
    const StbM_LocalTimeClockCfgType* localTimeClock = timeBaseCfgPtr->LocalTimeClock;

#if STBM_DISCIPLINED_CLOCK_USED == STD_ON
    const StbM_DisciplinedClockCfgType* disciplinedClock = timeBaseCfgPtr->DisciplinedClock;
#endif

#if STBM_FALLBACK_VIRTUAL_LOCAL_TIME_USED == STD_ON
    uint64                            tVFallback        = STBM_TIME_64_INVALID;
    const StbM_LocalTimeClockCfgType* fallbackTimeClock = timeBaseCfgPtr->FallbackTimeClock;
#endif

    /* Get Current Time Tuple */
#if STBM_TIME_VALIDATION_USED == STD_ON
    uint64 tLMonitor = STBM_TIME_64_INVALID;
#endif
    SchM_Enter_StbM_Context();
#if STBM_FALLBACK_VIRTUAL_LOCAL_TIME_USED == STD_ON
    if (fallbackTimeClock != NULL_PTR)
    {
        StbM_LocalClockToLocalTime(fallbackTimeClock, &tVFallback, NULL_PTR);
#if STBM_TIME_VALIDATION_USED == STD_ON
        StbM_Time64TupleType* preMainTimeTuple = &timeBaseVarPtr->PreMainTimeTuple;
        if ((tVFallback != STBM_TIME_64_INVALID) && (preMainTimeTuple->TVFallback != STBM_TIME_64_INVALID))
        {
            tLMonitor = preMainTimeTuple->TL + (tVFallback - preMainTimeTuple->TVFallback);
        }
#endif
        tL = StbM_GetGlobalTime64(timeBaseIndex, STBM_TIME_64_INVALID, tVFallback);
    }
#endif

#if STBM_DISCIPLINED_CLOCK_USED == STD_ON
    if (disciplinedClock != NULL_PTR)
    {
        StbM_GetDisciplinedClockTime(timeBaseIndex, &tL, &tV, &tVFallback);
    }
    else
#endif
    {
        StbM_LocalClockToLocalTime(localTimeClock, &tV, NULL_PTR);
        tL = StbM_GetGlobalTime64(timeBaseIndex, tV, STBM_TIME_64_INVALID);
    }

#if STBM_FALLBACK_VIRTUAL_LOCAL_TIME_USED == STD_ON
    if (tL == STBM_TIME_64_INVALID)
    {
        tL = StbM_GetGlobalTime64(timeBaseIndex, STBM_TIME_64_INVALID, tVFallback);
    }
#endif

    time64TuplePtr->TL = tL;
    time64TuplePtr->TV = tV;
#if STBM_FALLBACK_VIRTUAL_LOCAL_TIME_USED == STD_ON
    time64TuplePtr->TVFallback = tVFallback;

#if STBM_TIME_VALIDATION_USED == STD_ON
    if ((tLMonitor != STBM_TIME_64_INVALID) && (tL != STBM_TIME_64_INVALID))
    {
        uint64 progressionMismatch = (tL >= tLMonitor) ? (tL - tLMonitor) : (tLMonitor - tL);
        StbM_TimeBaseStatusBitSet(
            timeBaseVarPtr,
            STBM_TIMEBASE_STATUS_TIME_PROGRESSION_INCONSISTENCY,
            progressionMismatch > timeBaseCfgPtr->MaxProgressionMismatchThreshold);
    }
#endif
    /* SWS_StbM_00607 */
    if ((tV == STBM_TIME_64_INVALID) && (tVFallback != STBM_TIME_64_INVALID))
    {
        StbM_TimeBaseStatusBitSet(timeBaseVarPtr, STBM_TIMEBASE_STATUS_FALLBACK_TIME_EXTRAPOLATION, TRUE);
    }
#endif
    SchM_Exit_StbM_Context();
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief         Get the current local time
 * @param[in]     timeBaseIndex  The time base index in the configuration
 * @param[in]     tV             Input the primary local time and get the global time
 * @param[in]     tVFallback     Input the fallback local time and get the global time
 * @return        Local Instance of the Global Time
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace   CPD-71837
 */
STBM_LOCAL uint64 StbM_GetGlobalTime64(uint16 timeBaseIndex, uint64 tV, uint64 tVFallback)
{
    uint64             tL          = STBM_TIME_64_INVALID;
    StbM_TimeBaseType* timeBasePtr = &StbM_TimeBaseRuntime[timeBaseIndex];

    if ((tV != STBM_TIME_64_INVALID) && (timeBasePtr->MainTimeTuple.TV != STBM_TIME_64_INVALID))
    {
        uint64 diff = tV - timeBasePtr->MainTimeTuple.TV;

        if (TIMEBASE_CFG(timeBaseIndex).TimeCorrection == NULL_PTR)
        {
            tL = timeBasePtr->MainTimeTuple.TL + diff;
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
                StbM_Time64TupleType time64Tuple = {
                    timeBasePtr->MainTimeTuple.TL + (uint64)((float64)interval * rc),
                    timeBasePtr->MainTimeTuple.TV + interval,
#if STBM_FALLBACK_VIRTUAL_LOCAL_TIME_USED == STD_ON
                    tVFallback
#endif
                };
                StbM_UpdateMainTimeTuple(timeBaseIndex, &time64Tuple);
                timeBasePtr->RateDeviation.RocValue = 0;
                diff                                = diff - interval;
                tL =
                    timeBasePtr->MainTimeTuple.TL
                    + (uint64)(((float64)timeBasePtr->RateDeviation.RrcValue / (float64)STBM_RATE_PPM) * (float64)diff);
            }
            else
            {
                tL = timeBasePtr->MainTimeTuple.TL + (uint64)(rc * (float64)diff);
            }
            /* PRQA S 4119, 4395 -- */
        }
    }
#if STBM_FALLBACK_VIRTUAL_LOCAL_TIME_USED == STD_ON
    else if (tVFallback != STBM_TIME_64_INVALID && (timeBasePtr->MainTimeTuple.TVFallback != STBM_TIME_64_INVALID))
    {
        uint64 diff = tVFallback - timeBasePtr->MainTimeTuple.TVFallback;
        tL          = timeBasePtr->MainTimeTuple.TL + diff;
    }
    else
    {
        ; /* tL is STBM_TIME_64_INVALID */
    }
#else
    STBM_UNUSED(tVFallback);
#endif

    return tL;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief       Update the time tuple or triple of the specified time base.
 * @param[in]   timeBaseIndex   The time base index in the configuration
 * @param[in]   time64TuplePtr  Current time tuple [TL, TV, TVFallback]
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace   CPD-71838
 */
STBM_LOCAL void StbM_UpdateMainTimeTuple(uint16 timeBaseIndex, const StbM_Time64TupleType* time64TuplePtr)
{
    StbM_TimeBaseType*    timeBasePtr      = &StbM_TimeBaseRuntime[timeBaseIndex];
    StbM_Time64TupleType* mainTimeTuplePtr = &timeBasePtr->MainTimeTuple;

    timeBasePtr->MainTupleUpdateCnt = STBM_MAINTUPLE_HASNOT_UPDATE;
#if STBM_TIME_VALIDATION_USED == STD_ON /* STBM_FALLBACK_VIRTUAL_LOCAL_TIME_USED is STD_ON */
    StbM_Time64TupleType* preMainTimeTuplePtr = &timeBasePtr->PreMainTimeTuple;

    if ((time64TuplePtr->TV != STBM_TIME_64_INVALID) && (time64TuplePtr->TVFallback != STBM_TIME_64_INVALID))
    {
        *preMainTimeTuplePtr = *mainTimeTuplePtr;
    }
#endif

    if (time64TuplePtr->TL != STBM_TIME_64_INVALID)
    {
        mainTimeTuplePtr->TL = time64TuplePtr->TL;
    }
    mainTimeTuplePtr->TV = time64TuplePtr->TV;
#if STBM_FALLBACK_VIRTUAL_LOCAL_TIME_USED == STD_ON
    mainTimeTuplePtr->TVFallback = time64TuplePtr->TVFallback;

    if ((time64TuplePtr->TV != STBM_TIME_64_INVALID) && (time64TuplePtr->TVFallback != STBM_TIME_64_INVALID))
    {
        StbM_TimeBaseType* timeBaseVarPtr = &StbM_TimeBaseRuntime[timeBaseIndex];
        StbM_TimeBaseStatusBitSet(timeBaseVarPtr, STBM_TIMEBASE_STATUS_FALLBACK_TIME_EXTRAPOLATION, FALSE);
    }
#endif
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief       Convert StbM_TimeStampType time value to uint64 type.
 * @param[in]   timeStamp  The time value to be converted
 * @return      uint64
 * @retval      64-bit time value
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace   CPD-71839
 */
STBM_LOCAL uint64 StbM_GTTimeTo64Time(const StbM_TimeStampType* timeStamp)
{
    uint64 time64 = StbM_Shift_Left32((uint64)timeStamp->secondsHi) + (uint64)timeStamp->seconds;
    time64 *= STBM_NS_PER_S; /* PRQA S 1252 */ /* VL_StbM_1252 */
    time64 += timeStamp->nanoseconds;
    return time64;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief        Convert uint64 time value to TimeStampType type.
 * @param[in]    time64  The time value to be converted
 * @param[inout] timeStamp TimeStampType time value
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace   CPD-71840
 */
STBM_LOCAL void StbM_64TimeToGTTime(uint64 time64, StbM_TimeStampType* timeStamp)
{
    timeStamp->nanoseconds = (uint32)(time64 % STBM_NS_PER_S); /* PRQA S 1252 */ /* VL_StbM_1252 */
    uint64 t               = time64 / STBM_NS_PER_S; /* PRQA S 1252 */           /* VL_StbM_1252 */
    timeStamp->secondsHi   = (uint16)StbM_Shift_Right32(t);
    timeStamp->seconds     = (uint32)(t);
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief       Convert StbM_VirtualLocalTimeType time value to uint64 type.
 * @param[in]   localTime  The time value to be converted
 * @return      uint64
 * @retval      64-bit time value
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace   CPD-71841
 */
STBM_LOCAL uint64 StbM_LTTimeTo64Time(StbM_VirtualLocalTimeType localTime)
{
    uint64 time64 = StbM_Shift_Left32((uint64)localTime.nanosecondsHi);
    time64 |= (uint64)localTime.nanosecondsLo;
    return time64;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief       Calculate the difference between local time and global time.
 * @param[in]   timeBaseIndex The time base index in the configuration
 * @param[in]   TL_Sync       Global Time part of the Synclocal Time Tuple
 * @param[in]   TG_URx        Global Time part of the Updated Rx Time Tuple
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace   CPD-71842
 */
STBM_LOCAL void StbM_CheckTimeleap(uint16 timeBaseIndex, uint64 TL_Sync, uint64 TG_URx)
{
    StbM_TimeBaseType* timeBasePtr = &StbM_TimeBaseRuntime[timeBaseIndex];
    SchM_Enter_StbM_Context();
    if (timeBasePtr->BusNotFirstSet == TRUE)
    {
        StbM_TimeBaseStatusType leapState =
            (TL_Sync > TG_URx) ? STBM_TIMEBASE_STATUS_TIMELEAP_PAST : STBM_TIMEBASE_STATUS_TIMELEAP_FUTURE;
        uint64 leapThreshold = (TL_Sync > TG_URx) ? (TIMEBASE_CFG(timeBaseIndex).TimeLeapPastThreshold)
                                                  : (TIMEBASE_CFG(timeBaseIndex).TimeLeapFutureThreshold);
        uint64 timeLeap      = (TL_Sync > TG_URx) ? (TL_Sync - TG_URx) : (TG_URx - TL_Sync);
        /* SWS_StbM_00182 SWS_StbM_00305 */
        if (leapThreshold != 0ULL)
        {
            if (timeLeap > leapThreshold)
            {
                StbM_TimeBaseStatusBitSet(timeBasePtr, leapState, TRUE);
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
                    StbM_TimeBaseStatusBitSet(timeBasePtr, STBM_TIMEBASE_STATUS_TIMELEAP_PAST, FALSE);
                    StbM_TimeBaseStatusBitSet(timeBasePtr, STBM_TIMEBASE_STATUS_TIMELEAP_FUTURE, FALSE);
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
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief       Check if there has been a timeout in time synchronization.
 * @param[in]   timeBaseIndex The time base index in the configuration
 * @param[in]   tvPtr         Virtual Local Time
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace   CPD-71843
 */
STBM_LOCAL void StbM_CheckTimeout(uint16 timeBaseIndex, const uint64* tvPtr)
{
    const StbM_SynchronizedTimeBaseCfgType* timeBaseCfgPtr = &TIMEBASE_CFG(timeBaseIndex);

    /* SWS_StbM_00183 */
    if ((!timeBaseCfgPtr->IsSystemWideGlobalTimeMaster) && (timeBaseCfgPtr->SyncLossTimeout != 0ULL))
    {
        StbM_TimeBaseType* timeBasePtr = &StbM_TimeBaseRuntime[timeBaseIndex];

        SchM_Enter_StbM_Context();
        if (timeBasePtr->BusNotFirstSet == TRUE)
        {
            if ((*tvPtr - timeBasePtr->LastBusSettime) > timeBaseCfgPtr->SyncLossTimeout)
            {
                StbM_TimeBaseStatusBitSet(timeBasePtr, STBM_TIMEBASE_STATUS_TIMEOUT, TRUE);
                /* SWS_StbM_00187 */
                if (timeBaseCfgPtr->IsTimeGateway)
                {
                    StbM_TimeBaseStatusBitSet(timeBasePtr, STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY, TRUE);
                }
            }
        }
        SchM_Exit_StbM_Context();
    }
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief       Handle the cycle task.
 * @param[in]   timeBaseIndex The time base index in the configuration
 * @param[in]   time64TuplePtr The time tuple to be used
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace   CPD-85964
 */
STBM_LOCAL void StbM_CycleTaskHandle(uint16 timeBaseIndex, const StbM_Time64TupleType* time64TuplePtr)
{
    StbM_TimeBaseType* timeBasePtr = &StbM_TimeBaseRuntime[timeBaseIndex];
    /**
     * If there has been no update for more than 3s, the StbM may also update the Main Time Tuple.
     */

    if (timeBasePtr->MainTupleUpdateCnt > 0ULL)
    {
        SchM_Enter_StbM_Context();
        timeBasePtr->MainTupleUpdateCnt--;
        if (timeBasePtr->MainTupleUpdateCnt == 0ULL)
        {
            StbM_UpdateMainTimeTuple(timeBaseIndex, time64TuplePtr);
        }
        SchM_Exit_StbM_Context();
    }

#if STBM_STORE_TIMEBASE_NONVOLATILE_USED == STD_ON
    /** SWS_StbM_00172 */
    if (timeBasePtr->BackupTimeCnt > 0ULL)
    {
        timeBasePtr->BackupTimeCnt--;
        if (timeBasePtr->BackupTimeCnt == 0ULL)
        {
            StbM_WriteBackupTimeHandle(timeBaseIndex, &time64TuplePtr->TL);
        }
    }
#endif
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#if STBM_TIME_CORRECTION_SUPPORT == STD_ON
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief       Rate Correction measurement.
 * @param[in]   timeBaseIndex  The time base index in the configuration
 * @param[in]   tGPtr          Global Time part of the Updated Rx Time Tuple
 * @param[in]   tVPtr          Virtual Local Time part of the Updated Rx Time Tuple
 * @param[in]   timeBaseStatus The time base status
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace   CPD-71844
 */
STBM_LOCAL void StbM_CalculateRrc(
    uint16                  timeBaseIndex,
    const uint64*           tGPtr,
    const uint64*           tVPtr,
    StbM_TimeBaseStatusType timeBaseStatus)
{
    const StbM_TimeCorrectionCfgType* cfgPtr      = TIMEBASE_CFG(timeBaseIndex).TimeCorrection;
    StbM_TimeBaseType*                timeBasePtr = &StbM_TimeBaseRuntime[timeBaseIndex];

    const StbM_RateCorrectionType* const rataCorrection = &timeBasePtr->RateCorrection;

    if ((cfgPtr == NULL_PTR) || (cfgPtr->RateCorrectionMeasurementDuration == 0u))
    {
        return;
    }

    uint16 num = cfgPtr->RateCorrectionsPerMeasurementDuration;

    SchM_Enter_StbM_Context();
    if ((timeBasePtr->TimeBaseStatus & STBM_RATE_CORRECTION_INVALID_TIMEBASE_STATUS) != 0u)
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

    uint64 tVStop = *tVPtr;
    uint64 tGStop = *tGPtr;

    if ((timeBaseStatus & STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY)
        != (timeBasePtr->TimeBaseStatus & STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY))
    {
        rataCorrection->Array[0].TGstart = tGStop;
        rataCorrection->Array[0].TVstart = tVStop;
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
        rataCorrection->Array[0].TGstart       = tGStop;
        rataCorrection->Array[0].TVstart       = tVStop;
        timeBasePtr->RateCorrection.StartIndex = 0UL;
        timeBasePtr->RateCorrection.StopIndex  = 1UL;
    }
    else
    {
        uint64 perDuration = cfgPtr->RateCorrectionMeasurementDuration / num;
        uint32 stopIndex   = timeBasePtr->RateCorrection.StopIndex;

        if ((tVStop - rataCorrection->Array[(stopIndex - 1UL) % arraySize].TVstart) >= perDuration)
        {
            rataCorrection->Array[stopIndex % arraySize].TGstart = tGStop;
            rataCorrection->Array[stopIndex % arraySize].TVstart = tVStop;
            timeBasePtr->RateCorrection.StopIndex++;
        }
    }

    uint32 startIndex = timeBasePtr->RateCorrection.StartIndex;
    if ((timeBasePtr->RateCorrection.StopIndex - startIndex) > num)
    {
        uint64 TGdiff = tGStop - rataCorrection->Array[startIndex % arraySize].TGstart;
        uint64 TVdiff = tVStop - rataCorrection->Array[startIndex % arraySize].TVstart;
        /* PRQA S 4119, 4395 ++ */ /* VL_StbM_4119, VL_StbM_4395 */
        sint32 rrcValue = (sint32)(((float64)TGdiff / (float64)TVdiff * (float64)STBM_RATE_PPM) + STBM_ROUND_VALUE);
        /* PRQA S 4119, 4395 -- */
        timeBasePtr->RateCorrection.StartIndex++;

        if ((cfgPtr->RateCorrectionThreshold > 0U)
            && ((rrcValue > (STBM_RATE_PPM + (sint32)cfgPtr->RateCorrectionThreshold))
                || (rrcValue < (STBM_RATE_PPM - (sint32)cfgPtr->RateCorrectionThreshold))))
        {
            StbM_TimeBaseStatusBitSet(timeBasePtr, STBM_TIMEBASE_STATUS_RATE_EXCEEDED, TRUE);
        }
        else
        {
            timeBasePtr->RateDeviation.RrcValue = rrcValue;
            timeBasePtr->RateDeviation.IsSet    = TRUE;
            StbM_TimeBaseStatusBitSet(timeBasePtr, STBM_TIMEBASE_STATUS_RATE_EXCEEDED, FALSE);
            StbM_TimeBaseStatusBitSet(timeBasePtr, STBM_TIMEBASE_STATUS_RATE_CORRECTED, TRUE);
        }
    }
    SchM_Exit_StbM_Context();
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief         Calculate a new value for the Offset Correction by Rate Adaption.
 * @param[in]     timeBaseIndex  The time base index in the configuration
 * @param[in]     tLSync         Local Instance of the Global Time at re-synchronization
 * @param[in,out] tGPtr          Global Time part of the Updated Rx Time Tuple
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace   CPD-71845
 */
STBM_LOCAL void StbM_CalculateRoc(uint16 timeBaseIndex, uint64* tGPtr, uint64 tLSync)
{
    const StbM_SynchronizedTimeBaseCfgType* timeBaseCfgPtr = &TIMEBASE_CFG(timeBaseIndex);
    const StbM_TimeCorrectionCfgType*       cfgPtr         = timeBaseCfgPtr->TimeCorrection;
    StbM_TimeBaseType*                      timeBasePtr    = &StbM_TimeBaseRuntime[timeBaseIndex];

    /* If TVMain is invalid and either TVFallbackSync or TVFallbackMain is also invalid, then TLSync cannot be
       calculated and the calculation of roc will be skipped. */
    if ((cfgPtr == NULL_PTR) || (tLSync == STBM_TIME_64_INVALID))
    {
        return;
    }

    uint64 tGStop   = *tGPtr;
    uint64 interval = cfgPtr->OffsetCorrectionAdaptionInterval;
    sint64 odiff    = (sint64)tGStop - (sint64)tLSync;

    SchM_Enter_StbM_Context();
    if (cfgPtr->OffsetCorrectionJumpThreshold == 0u)
    {
        timeBasePtr->RateDeviation.RocValue = 0;
    }
    else if (StbM_Abs(odiff) >= cfgPtr->OffsetCorrectionJumpThreshold)
    {
        timeBasePtr->RateDeviation.RocValue = 0;
    }
    else if (interval == 0u)
    {
        timeBasePtr->RateDeviation.RocValue = 0;
    }
    else if (StbM_Abs(odiff) < cfgPtr->OffsetCorrectionJumpThreshold) /* SWS_StbM_00356 */
    {
        /* PRQA S 4119, 4395 ++ */ /* VL_StbM_4119, VL_StbM_4395 */
        float64 temp = (float64)STBM_RATE_PPM * (float64)odiff / (float64)interval;
        if (temp > 0.0)
        {
            timeBasePtr->RateDeviation.RocValue = (sint32)(temp + STBM_ROUND_VALUE);
        }
        else
        {
            timeBasePtr->RateDeviation.RocValue = (sint32)(temp - STBM_ROUND_VALUE);
        }
        /* PRQA S 4119, 4395 -- */
        *tGPtr = tLSync;
    }
    else
    {
        /* do nothing */
    }
    SchM_Exit_StbM_Context();

#if STBM_DISCIPLINED_CLOCK_USED == STD_ON
    const StbM_DisciplinedClockCfgType* disciplinedClock = timeBaseCfgPtr->DisciplinedClock;
    if (disciplinedClock != NULL_PTR)
    {
        sint64 ethRateDeviation = (timeBasePtr->RateDeviation.RrcValue - STBM_RATE_PPM) * STBM_POW2_41 / STBM_RATE_PPM;
        ethRateDeviation        = StbM_Clamp(ethRateDeviation, STBM_SINT32_MIN, STBM_SINT32_MAX);
        sint32 offset           = timeBasePtr->RateDeviation.RocValue;
        (void)EthIf_SetPhcCorrection(disciplinedClock->CtrlIdx, disciplinedClock->ClkUnitIdx, ethRateDeviation, offset);
    }
#endif
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#if STBM_STATUS_NOTIFICATIONS_ENABLED == STD_ON
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief       Check the notification event and take the appropriate action.
 * @param[in]   timeBaseIndex The time base index in the configuration
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace   CPD-71848
 */
STBM_LOCAL void StbM_CheckStatusNotification(uint16 timeBaseIndex)
{
    const StbM_SynchronizedTimeBaseCfgType* timeBaseCfg = &TIMEBASE_CFG(timeBaseIndex);

    StbM_TimeBaseNotificationType* notificationEvents = &StbM_TimeBaseRuntime[timeBaseIndex].NotificationEvents;
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
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#if (STBM_TIME_RECORDING_SUPPORT == STD_ON)
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief       Record time information.
 * @param[in]   timeBaseIndex The time base index in the configuration
 * @param[in]   TLsync        The Local Time Base directly before synchronization with the Global Time Base
 * @param[in]   TGrx          The Local Time Base directly after synchronization with the Global Time Base
 * @param[in]   TVrx          Virtual Local Time
 * @param[in]   pathDelay     Current propagation delay in nanoseconds
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace   CPD-71849
 */
STBM_LOCAL void StbM_TimeRecord(uint16 timeBaseIndex, uint64 TLsync, uint64 TGrx, uint64 TVrx, uint32 pathDelay)
{
    const StbM_SynchronizedTimeBaseCfgType* timeBaseCfgPtr = &TIMEBASE_CFG(timeBaseIndex);
    if (timeBaseCfgPtr->TimeRecording != NULL_PTR)
    {
        StbM_TimeBaseType* timeBasePtr = &StbM_TimeBaseRuntime[timeBaseIndex];

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
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief       Name of the customer specific callback function, which shall be called, if a
 *              measurement data for a Synchronized Time Base are available.
 * @param[in]   timeBaseIndex The time base index in the configuration
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace   CPD-71850
 */
STBM_LOCAL void StbM_TimeRecordback(uint16 timeBaseIndex)
{
    const StbM_TimeRecordingCfgType* timeRecordingCfgPtr = TIMEBASE_CFG(timeBaseIndex).TimeRecording;
    if (timeRecordingCfgPtr != NULL_PTR)
    {
        if (timeRecordingCfgPtr->SyncTimeRecordTableBlockCount > 0u)
        {
            StbM_TimeBaseType* timeBasePtr = &StbM_TimeBaseRuntime[timeBaseIndex];
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
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief       Set the user data for a Synchronized time base.
 * @param[in]   timeBaseIndex The time base index in the configuration
 * @param[in]   userDataPtr   User data of the time base
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace   CPD-71851
 */
STBM_LOCAL void StbM_SetUserDataIn(uint16 timeBaseIndex, const StbM_UserDataType* userDataPtr)
{
    if (userDataPtr != NULL_PTR)
    {
        StbM_TimeBaseType* timeBasePtr       = &StbM_TimeBaseRuntime[timeBaseIndex];
        timeBasePtr->UserData.userDataLength = userDataPtr->userDataLength;

        if (STBM_USER_DATA_BYTE_0 < userDataPtr->userDataLength)
        {
            timeBasePtr->UserData.userByte0 = userDataPtr->userByte0;
            if (STBM_USER_DATA_BYTE_1 < userDataPtr->userDataLength)
            {
                timeBasePtr->UserData.userByte1 = userDataPtr->userByte1;
                if (STBM_USER_DATA_BYTE_2 < userDataPtr->userDataLength)
                {
                    timeBasePtr->UserData.userByte2 = userDataPtr->userByte2;
                }
            }
        }
    }
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/* PRQA S 6070 ++ */ /* VL_MTR_StbM_STCAL */
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
 * @trace   CPD-71855
 */
STBM_LOCAL Std_ReturnType StbM_SetGlobalTimePrivate(
    StbM_SynchronizedTimeBaseType timeBaseId,
    const StbM_TimeStampType*     timeStamp,
    const StbM_UserDataType*      userData,
    uint8                         serviceId,
    boolean                       isUpdateCounter)
/* PRQA S 6070 -- */
{
    STBM_UNUSED(serviceId);
    Std_ReturnType ret           = E_NOT_OK;
    uint16         timeBaseIndex = StbM_FindSynchronizedTimeBase(timeBaseId);
#if STBM_DEV_ERROR_DETECT == STD_ON
    if (StbM_ValidateCommon(serviceId, timeBaseId, timeBaseIndex) && StbM_ValidatePointer(serviceId, timeStamp)
        && StbM_ValidateNanoseconds(serviceId, timeStamp->nanoseconds) && StbM_ValidateUserData(serviceId, userData)
        && StbM_ValidatePartition(serviceId, timeBaseIndex))
#endif
    {
#if STBM_DISCIPLINED_CLOCK_USED == STD_ON
        const StbM_DisciplinedClockCfgType* disciplinedClock = TIMEBASE_CFG(timeBaseIndex).DisciplinedClock;
        if (disciplinedClock != NULL_PTR)
        {
            ret = EthIf_SetPhcTime(disciplinedClock->CtrlIdx, disciplinedClock->ClkUnitIdx, timeStamp);
        }
#endif
        /* SWS_StbM_00342 SWS_StbM_00436 */
        StbM_Time64TupleType time64Tuple;
        StbM_GetCurrentTimeTuple(timeBaseIndex, &time64Tuple);

        time64Tuple.TL = StbM_GTTimeTo64Time(timeStamp);
        SchM_Enter_StbM_Context();
        StbM_UpdateMainTimeTuple(timeBaseIndex, &time64Tuple);
        SchM_Exit_StbM_Context();

        StbM_TimeBaseType* timeBasePtr = &StbM_TimeBaseRuntime[timeBaseIndex];
        /* SWS_StbM_00571 SWS_StbM_00181 */
        SchM_Enter_StbM_Context();
#if STBM_STATUS_NOTIFICATIONS_ENABLED == STD_ON
        timeBasePtr->NotificationEvents |= STBM_TIMEBASE_EV_RESYNC;
#endif
        StbM_TimeBaseStatusBitSet(timeBasePtr, STBM_TIMEBASE_STATUS_GLOBAL_TIME_BASE, TRUE);
        StbM_SetUserDataIn(timeBaseIndex, userData);
        if (isUpdateCounter)
        {
            ++timeBasePtr->TimeBaseUpdateCounter;
        }
        SchM_Exit_StbM_Context();

        ret = E_OK;
    }

    return ret;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief        Set the Global Time information
 * @param[inout] timeBaseVarPtr Time base variable pointer
 * @param[in]    bit             Bit to set
 * @param[in]    set             TRUE to set, FALSE to clear
 * @reentrant    FALSE
 * @synchronous  TRUE
 * @trace   CPD-85965
 */
STBM_LOCAL void StbM_TimeBaseStatusBitSet(StbM_TimeBaseType* timeBaseVarPtr, StbM_TimeBaseStatusType bit, boolean set)
{
    StbM_TimeBaseStatusType* timeBaseStatusPtr = &timeBaseVarPtr->TimeBaseStatus;
    if (((*timeBaseStatusPtr & bit) != 0u) != set)
    {
        *timeBaseStatusPtr = set ? (*timeBaseStatusPtr | bit) : (*timeBaseStatusPtr & (StbM_TimeBaseStatusType)(~bit));

#if STBM_STATUS_NOTIFICATIONS_ENABLED == STD_ON
        /* Clear all bits left of the specified bit */
        StbM_TimeBaseStatusType statusMask = STBM_TIME_BASE_STATUS_MASK & (bit | bit - 1u);

        /* Calculate the index of this bit as the nth '1' in the mask (index of TimeBaseStatusNotifications) */
        uint8 index = 0u;
        while ((statusMask &= statusMask - 1u) != 0u)
        {
            index++;
        }

        timeBaseVarPtr->NotificationEvents |=
            set ? StbM_TimeBaseStatusNotifications[index].SetEvent : StbM_TimeBaseStatusNotifications[index].ClrEvent;
#endif
    }
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#if (STBM_NOTIFICATION_CUSTOMER_NUM > 0)
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief       Notify when the customer's time is up.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace   CPD-71869
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
            StbM_Time64TupleType time64Tuple;
            StbM_GetCurrentTimeTuple(timeBaseIndex, &time64Tuple);
            if (time64Tuple.TL != STBM_TIME_64_INVALID)
            {
                sint64 timeDiff = (sint64)notificationPtr->CustomerTimerExpireTime - (sint64)time64Tuple.TL;
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
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#if STBM_TRIGGERED_CUSTOMER_NUM > 0
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL boolean StbM_CheckTimeBaseStatus(StbM_TimeBaseStatusType timeBaseStatus)
{
    return ((timeBaseStatus & STBM_TIMEBASE_STATUS_GLOBAL_TIME_BASE) != 0u)
           && ((timeBaseStatus & STBM_TIMEBASE_STATUS_TIMEOUT) == 0u)
           && ((timeBaseStatus & STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY) == 0u)
           && ((timeBaseStatus & STBM_TIMEBASE_STATUS_TIMELEAP_FUTURE) == 0u)
           && ((timeBaseStatus & STBM_TIMEBASE_STATUS_TIMELEAP_PAST) == 0u);
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL boolean StbM_CheckScheduleTableStatus(ScheduleTableType scheduleTableID)
{
    ScheduleTableStatusType scheduleTableStatus;
    if (GetScheduleTableStatus(scheduleTableID, &scheduleTableStatus) != E_OK)
    {
        return FALSE;
    }
    return (scheduleTableStatus == SCHEDULETABLE_WAITING) || (scheduleTableStatus == SCHEDULETABLE_RUNNING)
           || (scheduleTableStatus == SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS);
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief       The triggered customer is directly triggered by the Synchronized Time-base Manager
 *              by getting synchronized with the current (global) definition of time and passage of time.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace   CPD-71871
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
        StbM_TriggerRemainPeriod[index]++;
        if (StbM_TriggerRemainPeriod[index] >= triggerCustomerConfig->TriggeredCustomerPeriod)
        {
            StbM_TriggerRemainPeriod[index] = 0u;

            /* SWS_StbM_00077 */
            if (!StbM_CheckTimeBaseStatus(
                    StbM_TimeBaseRuntime[triggerCustomerConfig->SynchronizedTimeBaseRef].TimeBaseStatus))
            {
                continue; /* PRQA S 0770 */ /* VL_StbM_0770 */
            }

            /* SWS_StbM_00092 */
            if (!StbM_CheckScheduleTableStatus(triggerCustomerConfig->OSScheduleTableRef))
            {
                continue; /* PRQA S 0770 */ /* VL_StbM_0770 */
            }

            /* SWS_StbM_00302 SWS_StbM_00303 */
            StbM_Time64TupleType time64Tuple;
            StbM_GetCurrentTimeTuple(triggerCustomerConfig->SynchronizedTimeBaseRef, &time64Tuple);
            if (time64Tuple.TL != STBM_TIME_64_INVALID)
            {
                TickType value =
                    (TickType)((time64Tuple.TL / STBM_NS_PER_US) % triggerCustomerConfig->OsScheduleTableDuration);
                (void)SyncScheduleTable(triggerCustomerConfig->OSScheduleTableRef, value);
            }
        }
    }
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#if STBM_STORE_TIMEBASE_NONVOLATILE_USED == STD_ON
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief       Read the backup time handle from the non-volatile memory
 * @param[in]   timeBaseIndex   Index of the time base
 * @param[out]  backupTimeValuePtr   Pointer to the backup time value
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace   CPD-85444
 */
STBM_LOCAL void StbM_ReadBackupTimeHandle(uint16 timeBaseIndex, uint64* backupTimeValuePtr)
{
    const StbM_SynchronizedTimeBaseCfgType* timeBaseCfgPtr = &TIMEBASE_CFG(timeBaseIndex);
    if (timeBaseCfgPtr->BackupTimePtr != NULL_PTR)
    {
        uint64 backupTimeValue = *timeBaseCfgPtr->BackupTimePtr;
        *backupTimeValuePtr    = backupTimeValue;

        /** SWS_StbM_00171 */
        StbM_WriteBackupTimeHandle(timeBaseIndex, &backupTimeValue);
        StbM_TimeBaseRuntime[timeBaseIndex].BackupTimeCnt =
            timeBaseCfgPtr->CyclicBackupInterval / STBM_MAIN_FUNCTION_PERIOD;
        /** SWS_StbM_00578 */
#if STBM_DISCIPLINED_CLOCK_USED == STD_ON
        const StbM_DisciplinedClockCfgType* disciplinedClock = TIMEBASE_CFG(timeBaseIndex).DisciplinedClock;
        if (disciplinedClock != NULL_PTR)
        {
            StbM_TimeStampType timeStamp;
            StbM_64TimeToGTTime(backupTimeValue, &timeStamp);
            (void)EthIf_SetPhcTime(disciplinedClock->CtrlIdx, disciplinedClock->ClkUnitIdx, &timeStamp);
        }
#endif
    }
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief       Write the backup time handle to the non-volatile memory
 * @param[in]   timeBaseIndex   Index of the time base
 * @param[in]   backupTimeValuePtr   Pointer to the backup time value
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace   CPD-85445
 */
STBM_LOCAL void StbM_WriteBackupTimeHandle(uint16 timeBaseIndex, const uint64* backupTimeValuePtr)
{
    const StbM_SynchronizedTimeBaseCfgType* timeBaseCfgPtr = &TIMEBASE_CFG(timeBaseIndex);
    if (timeBaseCfgPtr->BackupTimePtr != NULL_PTR)
    {
        if (*backupTimeValuePtr != STBM_TIME_64_INVALID)
        {
            uint64  cyclicBackupInterval = timeBaseCfgPtr->CyclicBackupInterval;
            uint64* backupTimePtr        = timeBaseCfgPtr->BackupTimePtr;

            /** SWS_StbM_00555 */
            *backupTimePtr = *backupTimeValuePtr + cyclicBackupInterval;
            StbM_TimeBaseRuntime[timeBaseIndex].BackupTimeCnt =
                timeBaseCfgPtr->CyclicBackupInterval / STBM_MAIN_FUNCTION_PERIOD;
        }
        else
        {
            StbM_TimeBaseRuntime[timeBaseIndex].BackupTimeCnt = 1ULL;
        }
    }
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#if STBM_TIME_VALIDATION_USED == STD_ON
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief       Notify the time base provider
 * @param[in]   timeBaseIndex   Index of the time base
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace   CPD-85967
 */
STBM_LOCAL void StbM_TimeBaseProviderNotification(uint16 timeBaseIndex)
{
    const StbM_SynchronizedTimeBaseCfgType* timeBaseCfgPtr = &TIMEBASE_CFG(timeBaseIndex);
    uint16                                  blockCount     = timeBaseCfgPtr->TimeValidationRecordTableBlockCount;
    StbM_TimeBaseType*                      timeBasePtr    = &StbM_TimeBaseRuntime[timeBaseIndex];

#if STBM_CAN_SLAVE_TIME_VALIDATION_USED == STD_ON
    StbM_TimeBaseProviderNotificationCanSlave(
        timeBaseCfgPtr->CanSlaveMeasurementQueuePtr,
        &timeBasePtr->CanSlaveTimeValidationDataInfo,
        blockCount);
#endif

#if STBM_CAN_MASTER_TIME_VALIDATION_USED == STD_ON
    StbM_TimeBaseProviderNotificationCanMaster(
        timeBaseCfgPtr->CanMasterMeasurementQueuePtr,
        &timeBasePtr->CanMasterTimeValidationDataInfo,
        blockCount);
#endif

#if STBM_ETH_SLAVE_TIME_VALIDATION_USED == STD_ON
    StbM_TimeBaseProviderNotificationEthSlave(
        timeBaseCfgPtr->EthSlaveMeasurementQueuePtr,
        &timeBasePtr->EthSlaveTimeValidationDataInfo,
        blockCount);
    StbM_TimeBaseProviderNotificationPdelayInitiator(
        timeBaseCfgPtr->PdelayInitiatorMeasurementQueuePtr,
        &timeBasePtr->PdelayInitiatorValidationDataInfo,
        blockCount);
#endif

#if STBM_ETH_MASTER_TIME_VALIDATION_USED == STD_ON
    StbM_TimeBaseProviderNotificationEthMaster(
        timeBaseCfgPtr->EthMasterMeasurementQueuePtr,
        &timeBasePtr->EthMasterTimeValidationDataInfo,
        blockCount);
    StbM_TimeBaseProviderNotificationPdelayResponder(
        timeBaseCfgPtr->PdelayResponderMeasurementQueuePtr,
        &timeBasePtr->PdelayResponderValidationDataInfo,
        blockCount);
#endif
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#if STBM_CAN_SLAVE_TIME_VALIDATION_USED == STD_ON || STBM_ETH_SLAVE_TIME_VALIDATION_USED == STD_ON
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief       Pdelay and rate validation handler
 * @param[in]   timeBaseCfgPtr   Pointer to the time base configuration
 * @param[in]   timeBaseVarPtr   Pointer to the time base variable
 * @param[in]   pathDelay   Path delay
 * @param[in]   timeTriplePtr   Pointer to the time triple
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace   CPD-85968
 */
STBM_LOCAL void StbM_PdelayAndRateValidationHandler(
    const StbM_SynchronizedTimeBaseCfgType*  timeBaseCfgPtr,
    StbM_TimeBaseType*                       timeBaseVarPtr,
    uint32                                   pathDelay,
    const StbM_RateValidationTimeTripleType* timeTriplePtr)
{
    StbM_TimeBaseStatusType* timeBaseStatusPtr = &timeBaseVarPtr->TimeBaseStatus;
    boolean                  rateJitterWanderExceeded =
        StbM_TimeBaseStatusBitGet(timeBaseStatusPtr, STBM_TIMEBASE_STATUS_RATEJITTERWANDER_EXCEEDED);
    const StbM_PdelayAndRateValidationType* pdelayAndRateValidationCfgPtr = timeBaseCfgPtr->PdelayAndRateValidationPtr;

    if ((timeTriplePtr->TGRx != STBM_TIME_64_INVALID) && (timeTriplePtr->TVRx != STBM_TIME_64_INVALID)
        && (timeTriplePtr->TLSync != STBM_TIME_64_INVALID))
    {
        if (timeBaseVarPtr->RateValidationInterval == 0u)
        {
            if (pdelayAndRateValidationCfgPtr->AllowRateValidation)
            {
                timeBaseVarPtr->RateValidationTimeTriplePrev = *timeTriplePtr;
            }
            if (pdelayAndRateValidationCfgPtr->AllowRateWanderValidation)
            {
                timeBaseVarPtr->RateValidationTimeTriple0 = *timeTriplePtr;
            }
        }
        else
        {
            if (pdelayAndRateValidationCfgPtr->AllowRateValidation)
            {
                /* check for rate jitter. */
                uint64 deltaTG = timeTriplePtr->TGRx - timeBaseVarPtr->RateValidationTimeTriplePrev.TGRx;
                uint64 deltaTV = timeTriplePtr->TVRx - timeBaseVarPtr->RateValidationTimeTriplePrev.TVRx;
                uint64 deltaTL = timeTriplePtr->TLSync - timeBaseVarPtr->RateValidationTimeTriplePrev.TLSync;

                /* rate jitter detected. */
                rateJitterWanderExceeded = StbM_Float64Abs((float64)deltaTG / deltaTL - 1.0)
                                           > pdelayAndRateValidationCfgPtr->RateJitterThreshold;
                rateJitterWanderExceeded |= StbM_Float64Abs((float64)deltaTV / deltaTL - 1.0)
                                            > pdelayAndRateValidationCfgPtr->RateJitterThreshold;

                timeBaseVarPtr->RateValidationTimeTriplePrev = *timeTriplePtr;
            }
            if (pdelayAndRateValidationCfgPtr->AllowRateWanderValidation
                && (timeBaseVarPtr->RateValidationInterval == pdelayAndRateValidationCfgPtr->RateWanderIntervalWindow))
            {
                /* check for rate wander. */
                uint64 deltaTG = timeTriplePtr->TGRx - timeBaseVarPtr->RateValidationTimeTriple0.TGRx;
                uint64 deltaTV = timeTriplePtr->TVRx - timeBaseVarPtr->RateValidationTimeTriple0.TVRx;
                uint64 deltaTL = timeTriplePtr->TLSync - timeBaseVarPtr->RateValidationTimeTriple0.TLSync;

                /* rate wander detected. */
                rateJitterWanderExceeded |= StbM_Float64Abs((float64)deltaTG / deltaTL - 1.0)
                                            > pdelayAndRateValidationCfgPtr->RateWanderThreshold;
                rateJitterWanderExceeded |= StbM_Float64Abs((float64)deltaTV / deltaTL - 1.0)
                                            > pdelayAndRateValidationCfgPtr->RateWanderThreshold;

                timeBaseVarPtr->RateValidationTimeTriple0 = *timeTriplePtr;
                timeBaseVarPtr->RateValidationInterval    = 0u;
            }
            else if (pdelayAndRateValidationCfgPtr->RateWanderIntervalWindow == 0u)
            {
                timeBaseVarPtr->RateValidationInterval = 0u;
            }
        }
        timeBaseVarPtr->RateValidationInterval++;
    }
    else /* reset the calculation of rate validation. */
    {
        timeBaseVarPtr->RateValidationInterval = 0u;
    }

    if (pdelayAndRateValidationCfgPtr->AllowPdelayValidation)
    {
        SchM_Enter_StbM_Context();
        StbM_TimeBaseStatusBitSet(
            timeBaseVarPtr,
            STBM_TIMEBASE_STATUS_PDELAY_EXCEEDED,
            pathDelay > pdelayAndRateValidationCfgPtr->PdelayValidationThreshold);
        SchM_Exit_StbM_Context();
    }

    if (pdelayAndRateValidationCfgPtr->AllowRateValidation || pdelayAndRateValidationCfgPtr->AllowRateWanderValidation)
    {
        SchM_Enter_StbM_Context();
        StbM_TimeBaseStatusBitSet(
            timeBaseVarPtr,
            STBM_TIMEBASE_STATUS_RATEJITTERWANDER_EXCEEDED,
            rateJitterWanderExceeded);
        SchM_Exit_StbM_Context();
    }
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#if STBM_CLONE_TIMEBASE_USED == STD_ON
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief       Clone time base defer copy handler
 * @param[in]   srcTimeBaseIdx   Source time base index
 * @return void
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace   CPD-85970
 */
STBM_LOCAL void StbM_CloneTimeBaseDeferCopyHandler(uint16 srcTimeBaseIdx)
{
    /*SWS_StbM_00532, SWS_StbM_00531*/
    for (uint8 destTimeBaseIdx = 0u; destTimeBaseIdx < STBM_TIME_BASE_NUM; destTimeBaseIdx++)
    {
        const StbM_SynchronizedTimeBaseCfgType* destTimeBaseCfgPtr = &TIMEBASE_CFG(destTimeBaseIdx);
        if ((destTimeBaseCfgPtr->CloneRequestPtr != NULL_PTR) && (destTimeBaseCfgPtr->SrcTimeBaseIdx == srcTimeBaseIdx))
        {
            StbM_CloneTimeBaseCopy(destTimeBaseIdx, srcTimeBaseIdx);
        }
    }
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif
/* PRQA S 1503, 2889 -- */
