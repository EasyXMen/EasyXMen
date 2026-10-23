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
 **  @file               : StbM_Clock.c
 **  @author             : xiaojian.liang
 **  @date               : 2025/12/15
 **  @vendor             : isoft
 **  @description        : Synchronized TimeBase Manager Clock Source File
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "StbM_Clock.h"

#if STBM_GPT_TIMESTAMP_SUPPORT == STD_ON
#include "SchM_StbM.h"
#endif

#if STBM_GPT_TIMESTAMP_SUPPORT == STD_ON || STBM_OS_TIMESTAMP_SUPPORT == STD_ON
#if STBM_PRE_INIT_API == STD_ON
#include "IStdLib.h"
#endif

#include "StbM_Internal.h"
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */
#if STBM_GPT_TIMESTAMP_SUPPORT == STD_ON
/**
 * @brief Structure for GPT clock variables
 * */
typedef struct
{
    boolean       Running;   /**< Running status @range 0...1 */
    Gpt_ValueType PreValue;  /**< Previous GPT value @range 0...0xFFFFFFFF */
    uint64        FracValue; /**< Fractional time value @range 0...0xFFFFFFFFFFFFFFFF */
    uint64        Time;      /**< Current time value @range 0...0xFFFFFFFFFFFFFFFF */
} StbM_ClockGptVarType;
#endif

#if STBM_OS_TIMESTAMP_SUPPORT == STD_ON
/**
 * @brief Structure for OS clock variables
 * */
typedef struct
{
    TickType PreValue;  /**< Previous OS tick value @range 0...0xFFFFFFFF */
    uint64   FracValue; /**< Fractional time value @range 0...0xFFFFFFFFFFFFFFFF */
    uint64   Time;      /**< Current time value @range 0...0xFFFFFFFFFFFFFFFF */
} StbM_ClockOsVarType;
#endif

/* ========================================= internal function declarations ========================================= */
#if STBM_GPT_TIMESTAMP_SUPPORT == STD_ON || STBM_OS_TIMESTAMP_SUPPORT == STD_ON
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL void StbM_ClockUpdateTime(
    uint64  elapsedValue,
    uint64* fracValue,
    uint64* clockTime,
    uint32  clockFrequency,
    uint32  clockPrescaler);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

/* ============================================ internal data definitions =========================================== */
#if STBM_GPT_TIMESTAMP_SUPPORT == STD_ON
#define STBM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "StbM_MemMap.h"
/**
 * @brief GPT clock variables
 * */
STBM_LOCAL StbM_ClockGptVarType StbM_ClockGptVars[STBM_CLOCK_GPT_COUNT]; /**< @range NA */
#define STBM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "StbM_MemMap.h"
#endif

#if STBM_OS_TIMESTAMP_SUPPORT == STD_ON
#define STBM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "StbM_MemMap.h"
/**
 * @brief OS clock variables
 * */
STBM_LOCAL StbM_ClockOsVarType StbM_ClockOsVars[STBM_CLOCK_OS_COUNT]; /**< @range NA */
#define STBM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "StbM_MemMap.h"
#endif

/* ============================================ external data definition ============================================ */

/* ========================================== external function definitions ========================================= */
#if STBM_GPT_TIMESTAMP_SUPPORT == STD_ON || STBM_OS_TIMESTAMP_SUPPORT == STD_ON
#if STBM_PRE_INIT_API == STD_ON
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
void StbM_ClockPreInit(void)
{
#if STBM_GPT_TIMESTAMP_SUPPORT == STD_ON
    (void)IStdLib_MemSet(StbM_ClockGptVars, 0u, sizeof(StbM_ClockGptVars));
#endif
#if STBM_OS_TIMESTAMP_SUPPORT == STD_ON
    (void)IStdLib_MemSet(StbM_ClockOsVars, 0u, sizeof(StbM_ClockOsVars));
#endif
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif
#endif

#if STBM_GPT_TIMESTAMP_SUPPORT == STD_ON
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
void StbM_ClockGptInit(uint8 channelIdx)
{
    const StbM_ClockGptConfigType* clockConfigPtr = &StbM_ClockGptConfigs[channelIdx];
    StbM_ClockGptVarType*          clockVarPtr    = &StbM_ClockGptVars[channelIdx];

    /* This prevents race conditions between partitions accessing shared clock resources. */
    SchM_Enter_StbM_Context();
    if (!clockVarPtr->Running)
    {
        Gpt_StartTimer(clockConfigPtr->GptChannelId, clockConfigPtr->GptChannelTickValueMax);
        clockVarPtr->Running = TRUE;
    }
    SchM_Exit_StbM_Context();
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
void StbM_ClockGptGetCurrentTime(uint8 channelIdx, uint64* timePtr)
{
    const StbM_ClockGptConfigType* clockConfigPtr = &StbM_ClockGptConfigs[channelIdx];
    StbM_ClockGptVarType*          clockVarPtr    = &StbM_ClockGptVars[channelIdx];
    Gpt_ValueType                  value          = Gpt_GetTimeElapsed(clockConfigPtr->GptChannelId);
    Gpt_ValueType                  elapsedValue   = (clockVarPtr->PreValue <= value)
                                                        ? (value - clockVarPtr->PreValue)
                                                        : (clockConfigPtr->GptChannelTickValueMax - clockVarPtr->PreValue + value + 1u);
    clockVarPtr->PreValue                         = value;
    StbM_ClockUpdateTime(
        elapsedValue,
        &clockVarPtr->FracValue,
        &clockVarPtr->Time,
        clockConfigPtr->ClockFrequency,
        clockConfigPtr->ClockPrescaler);
    *timePtr = clockVarPtr->Time;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#if STBM_OS_TIMESTAMP_SUPPORT == STD_ON
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
void StbM_ClockOsGetCurrentTime(uint8 counterIdx, uint64* timePtr)
{
    const StbM_ClockOsConfigType* clockConfigPtr = &StbM_ClockOsConfigs[counterIdx];
    StbM_ClockOsVarType*          clockVarPtr    = &StbM_ClockOsVars[counterIdx];
    TickType                      elapsedValue;
    if (GetElapsedValue(clockConfigPtr->OsCounterID, &clockVarPtr->PreValue, &elapsedValue) == E_OK)
    {
        StbM_ClockUpdateTime(
            elapsedValue,
            &clockVarPtr->FracValue,
            &clockVarPtr->Time,
            clockConfigPtr->ClockFrequency,
            clockConfigPtr->ClockPrescaler);
        *timePtr = clockVarPtr->Time;
    }
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

/* ========================================== internal function definitions ========================================= */
#if STBM_GPT_TIMESTAMP_SUPPORT == STD_ON || STBM_OS_TIMESTAMP_SUPPORT == STD_ON
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief Updates the fractional and time values based on elapsed ticks, clock frequency, and prescaler.
 * @param[in] elapsedValue The elapsed value in ticks.
 * @param[in,out] fracValue Pointer to the fractional value to be updated.
 * @param[in,out] time Pointer to the time value to be updated.
 * @param[in] clockFrequency The clock frequency.
 * @param[in] clockPrescaler The clock prescaler.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace CPD-86509
 */
STBM_LOCAL void StbM_ClockUpdateTime(
    uint64  elapsedValue,
    uint64* fracValue,
    uint64* clockTime,
    uint32  clockFrequency,
    uint32  clockPrescaler)
{
    uint64 tFracValue = *fracValue;

    /* PRQA S 1252, 2834 ++ */ /* VL_StbM_1252, VL_StbM_2834 */
    tFracValue += (elapsedValue * STBM_NS_PER_S * clockPrescaler) % clockFrequency;
    /* PRQA S 1252, 2834 -- */

    if (tFracValue >= clockFrequency)
    {
        tFracValue -= clockFrequency;
        (*clockTime)++;
    }
    *fracValue = tFracValue;

    /* PRQA S 1252, 2834 ++ */ /* VL_StbM_1252, VL_StbM_2834 */
    *clockTime += (elapsedValue * STBM_NS_PER_S * clockPrescaler) / clockFrequency;
    /* PRQA S 1252, 2834 -- */
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif
