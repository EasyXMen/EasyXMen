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
 **  @file               : StbM_Clock.h
 **  @author             : xiaojian.liang
 **  @date               : 2025/12/15
 **  @vendor             : isoft
 **  @description        : Synchronized TimeBase Manager Clock Header File
 **
 ***********************************************************************************************************************/

#ifndef STBM_CLOCK_H_
#define STBM_CLOCK_H_

/* =================================================== inclusions =================================================== */
#include "StbM_Cfg.h"

#if STBM_GPT_TIMESTAMP_SUPPORT == STD_ON
#include "Gpt.h"
#endif

#if STBM_OS_TIMESTAMP_SUPPORT == STD_ON
#include "Os.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */
#if STBM_GPT_TIMESTAMP_SUPPORT == STD_ON
/**
 * @brief Configuration structure for GPT clock
 */
typedef struct
{
    Gpt_ChannelType GptChannelId;           /**< GPT channel identifier @range NA */
    Gpt_ValueType   GptChannelTickValueMax; /**< Maximum tick value for the GPT channel @range 0..0xFFFFFFFF */
    /** 1 .. 4294967295, see TCPP-3482 TCPP-3488 */
    uint32 ClockFrequency; /**< Clock frequency in Hz @range 0..0xFFFFFFFF */
    uint32 ClockPrescaler; /**< Clock prescaler value @range 0..0xFFFFFFFF */
} StbM_ClockGptConfigType;
#endif

#if STBM_OS_TIMESTAMP_SUPPORT == STD_ON
/**
 * @brief Configuration structure for OS clock
 */
typedef struct
{
    CounterType OsCounterID; /**< OS counter identifier @range NA */
    /** 1 .. 4294967295, see TCPP-3482 TCPP-3488 */
    uint32 ClockFrequency; /**< Clock frequency in Hz @range 0..0xFFFFFFFF */
    uint32 ClockPrescaler; /**< Clock prescaler value @range 0..0xFFFFFFFF */
} StbM_ClockOsConfigType;
#endif

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */
#if STBM_GPT_TIMESTAMP_SUPPORT == STD_ON
#define STBM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "StbM_MemMap.h"
/**
 * @brief Array of GPT clock configurations
 */
extern const StbM_ClockGptConfigType StbM_ClockGptConfigs[STBM_CLOCK_GPT_COUNT]; /**< @range NA */
#define STBM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "StbM_MemMap.h"
#endif

#if STBM_OS_TIMESTAMP_SUPPORT == STD_ON
#define STBM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "StbM_MemMap.h"
/**
 * @brief Array of OS clock configurations
 */
extern const StbM_ClockOsConfigType StbM_ClockOsConfigs[STBM_CLOCK_OS_COUNT]; /**< @range NA */
#define STBM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "StbM_MemMap.h"
#endif

/* ========================================= external function declarations ========================================= */
#if STBM_GPT_TIMESTAMP_SUPPORT == STD_ON || STBM_OS_TIMESTAMP_SUPPORT == STD_ON
#if STBM_PRE_INIT_API == STD_ON
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief Pre-initializes the clock module before the main initialization.
 * @return void
 * @reentrant    FALSE
 * @synchronous  TRUE
 * @trace   CPD-85946
 */
void StbM_ClockPreInit(void);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif
#endif

#if STBM_GPT_TIMESTAMP_SUPPORT == STD_ON
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief Initializes the specified GPT channel for clock functionality.
 * @param[in] channelIdx Index of the GPT channel to initialize
 * @return void
 * @reentrant    TRUE (for different channelIdx values)
 * @synchronous  TRUE
 * @trace   CPD-85947
 */
void StbM_ClockGptInit(uint8 channelIdx);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief Gets the current time from the specified GPT channel.
 * @param[in] channelIdx Index of the GPT channel to get time from
 * @param[out] timePtr Pointer to store the current time value
 * @return void
 * @reentrant    TRUE (for different channelIdx values)
 * @synchronous  TRUE
 * @trace   CPD-85948
 */
void StbM_ClockGptGetCurrentTime(uint8 channelIdx, uint64* timePtr);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#if STBM_OS_TIMESTAMP_SUPPORT == STD_ON
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief Gets the current time from the specified OS counter.
 * @param[in] counterIdx Index of the OS counter to get time from
 * @param[out] timePtr Pointer to store the current time value
 * @return void
 * @reentrant    TRUE (for different counterIdx values)
 * @synchronous  TRUE
 * @trace   CPD-85949
 */
void StbM_ClockOsGetCurrentTime(uint8 counterIdx, uint64* timePtr);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#ifdef __cplusplus
}
#endif
#endif
