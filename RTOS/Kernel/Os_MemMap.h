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
**  FILENAME    : Os_MemMap.h                                                 **
**                                                                            **
**  Created on  : 2018-08-12                                                  **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : Memory mapping abstraction declaration of OS                **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/

/*=======[M A C R O S]========================================================*/
#define MEMMAP_ERROR

#if defined(START_WITH_IF)

/*Index:0  Section name :CODE*/
#elif defined OS_START_SEC_CODE
#undef OS_START_SEC_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#define OS_START_SEC_CODE_NO_MATCH
#define START_SEC_CODE

#elif defined OS_STOP_SEC_CODE
#undef OS_STOP_SEC_CODE
#undef MEMMAP_ERROR
#ifdef OS_START_SEC_CODE_NO_MATCH
#undef OS_START_SEC_CODE_NO_MATCH
#else
#error "Os_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CODE

/*Index:1  Section name :CONST_PTR*/
#elif defined OS_START_SEC_CONST_PTR
#undef OS_START_SEC_CONST_PTR
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#define OS_START_SEC_CONST_PTR_NO_MATCH
#define START_SEC_CONST_PTR

#elif defined OS_STOP_SEC_CONST_PTR
#undef OS_STOP_SEC_CONST_PTR
#undef MEMMAP_ERROR
#ifdef OS_START_SEC_CONST_PTR_NO_MATCH
#undef OS_START_SEC_CONST_PTR_NO_MATCH
#else
#error "Os_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CONST_PTR

/*Index:2  Section name :CONST_UNSPECIFIED*/
#elif defined OS_START_SEC_CONST_UNSPECIFIED
#undef OS_START_SEC_CONST_UNSPECIFIED
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#define OS_START_SEC_CONST_UNSPECIFIED_NO_MATCH
#define START_SEC_CONST_UNSPECIFIED

#elif defined OS_STOP_SEC_CONST_UNSPECIFIED
#undef OS_STOP_SEC_CONST_UNSPECIFIED
#undef MEMMAP_ERROR
#ifdef OS_START_SEC_CONST_UNSPECIFIED_NO_MATCH
#undef OS_START_SEC_CONST_UNSPECIFIED_NO_MATCH
#else
#error "Os_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CONST_UNSPECIFIED

/*Index:3  Section name :VAR_CLEARED_GLOBAL_32*/
#elif defined OS_START_SEC_VAR_CLEARED_GLOBAL_32
#undef OS_START_SEC_VAR_CLEARED_GLOBAL_32
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#define OS_START_SEC_VAR_CLEARED_GLOBAL_32_NO_MATCH
#define START_SEC_VAR_CLEARED_GLOBAL_32

#elif defined OS_STOP_SEC_VAR_CLEARED_GLOBAL_32
#undef OS_STOP_SEC_VAR_CLEARED_GLOBAL_32
#undef MEMMAP_ERROR
#ifdef OS_START_SEC_VAR_CLEARED_GLOBAL_32_NO_MATCH
#undef OS_START_SEC_VAR_CLEARED_GLOBAL_32_NO_MATCH
#else
#error "Os_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLEARED_GLOBAL_32

/*Index:4  Section name :VAR_CLEARED_GLOBAL_UNSPECIFIED*/
#elif defined OS_START_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#undef OS_START_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#define OS_START_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED_NO_MATCH
#define START_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED

#elif defined OS_STOP_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#undef OS_STOP_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#undef MEMMAP_ERROR
#ifdef OS_START_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED_NO_MATCH
#undef OS_START_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED_NO_MATCH
#else
#error "Os_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED

/*Index:5  Section name :VAR_CLEARED_CLONE_PTR*/
#elif defined OS_START_SEC_VAR_CLONE_PTR
#undef OS_START_SEC_VAR_CLONE_PTR
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#define OS_START_SEC_VAR_CLONE_PTR_NO_MATCH
#define START_SEC_VAR_CLONE_PTR

#elif defined OS_STOP_SEC_VAR_CLONE_PTR
#undef OS_STOP_SEC_VAR_CLONE_PTR
#undef MEMMAP_ERROR
#ifdef OS_START_SEC_VAR_CLONE_PTR_NO_MATCH
#undef OS_START_SEC_VAR_CLONE_PTR_NO_MATCH
#else
#error "Os_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLONE_PTR

/*Index:6  Section name :VAR_CLEARED_CLONE_32*/
#elif defined OS_START_SEC_VAR_CLONE_32
#undef OS_START_SEC_VAR_CLONE_32
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#define OS_START_SEC_VAR_CLONE_32_NO_MATCH
#define START_SEC_VAR_CLONE_32

#elif defined OS_STOP_SEC_VAR_CLONE_32
#undef OS_STOP_SEC_VAR_CLONE_32
#undef MEMMAP_ERROR
#ifdef OS_START_SEC_VAR_CLONE_32_NO_MATCH
#undef OS_START_SEC_VAR_CLONE_32_NO_MATCH
#else
#error "Os_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLONE_32

/*Index:7  Section name :VAR_CLEARED_CLONE_16*/
#elif defined OS_START_SEC_VAR_CLONE_16
#undef OS_START_SEC_VAR_CLONE_16
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#define OS_START_SEC_VAR_CLONE_16_NO_MATCH
#define START_SEC_VAR_CLONE_16

#elif defined OS_STOP_SEC_VAR_CLONE_16
#undef OS_STOP_SEC_VAR_CLONE_16
#undef MEMMAP_ERROR
#ifdef OS_START_SEC_VAR_CLONE_16_NO_MATCH
#undef OS_START_SEC_VAR_CLONE_16_NO_MATCH
#else
#error "Os_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLONE_16

/*Index:8  Section name :VAR_CLEARED_CLONE_8*/
#elif defined OS_START_SEC_VAR_CLONE_8
#undef OS_START_SEC_VAR_CLONE_8
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#define OS_START_SEC_VAR_CLONE_8_NO_MATCH
#define START_SEC_VAR_CLONE_8

#elif defined OS_STOP_SEC_VAR_CLONE_8
#undef OS_STOP_SEC_VAR_CLONE_8
#undef MEMMAP_ERROR
#ifdef OS_START_SEC_VAR_CLONE_8_NO_MATCH
#undef OS_START_SEC_VAR_CLONE_8_NO_MATCH
#else
#error "Os_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLONE_8

/*Index:9  Section name :VAR_CONFIG_DATA_UNSPECIFIED*/
#elif defined OS_START_SEC_VAR_CONFIG_DATA_UNSPECIFIED
#undef OS_START_SEC_VAR_CONFIG_DATA_UNSPECIFIED
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#define OS_START_SEC_VAR_CONFIG_DATA_UNSPECIFIED_NO_MATCH
#define START_SEC_VAR_CONFIG_DATA_UNSPECIFIED

#elif defined OS_STOP_SEC_VAR_CONFIG_DATA_UNSPECIFIED
#undef OS_STOP_SEC_VAR_CONFIG_DATA_UNSPECIFIED
#undef MEMMAP_ERROR
#ifdef OS_START_SEC_VAR_CONFIG_DATA_UNSPECIFIED_NO_MATCH
#undef OS_START_SEC_VAR_CONFIG_DATA_UNSPECIFIED_NO_MATCH
#else
#error "Os_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CONFIG_DATA_UNSPECIFIED

/*Index:28  Section name :VAR_CONFIG_DATA_UNSPECIFIED*/
#elif defined OS_START_SEC_VAR_CLONE_UNSPECIFIED
#undef OS_START_SEC_VAR_CLONE_UNSPECIFIED
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#define OS_START_SEC_VAR_CLONE_UNSPECIFIED_NO_MATCH
#define START_SEC_VAR_CLONE_UNSPECIFIED

#elif defined OS_STOP_SEC_VAR_CLONE_UNSPECIFIED
#undef OS_STOP_SEC_VAR_CLONE_UNSPECIFIED
#undef MEMMAP_ERROR
#ifdef OS_START_SEC_VAR_CLONE_UNSPECIFIED_NO_MATCH
#undef OS_START_SEC_VAR_CLONE_UNSPECIFIED_NO_MATCH
#else
#error "Os_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLONE_UNSPECIFIED

/*Index:29  Section name :VAR_CONFIG_DATA_UNSPECIFIED*/
#elif defined OS_START_SEC_CODE_FAST
#undef OS_START_SEC_CODE_FAST
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#define OS_START_SEC_CODE_FAST_NO_MATCH
#define START_SEC_CODE_FAST

#elif defined OS_STOP_SEC_CODE_FAST
#undef OS_STOP_SEC_CODE_FAST
#undef MEMMAP_ERROR
#ifdef OS_START_SEC_CODE_FAST_NO_MATCH
#undef OS_START_SEC_CODE_FAST_NO_MATCH
#else
#error "Os_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CODE_FAST

/*Index:30  Section name :VAR_POWER_ON_CLEARED*/
#elif defined OS_START_SEC_VAR_POWER_ON_CLEARED
#undef OS_START_SEC_VAR_POWER_ON_CLEARED
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#define OS_START_SEC_VAR_POWER_ON_CLEARED_NO_MATCH
#define START_SEC_VAR_POWER_ON_CLEARED

#elif defined OS_STOP_SEC_VAR_POWER_ON_CLEARED
#undef OS_STOP_SEC_VAR_POWER_ON_CLEARED
#undef MEMMAP_ERROR
#ifdef OS_START_SEC_VAR_POWER_ON_CLEARED_NO_MATCH
#undef OS_START_SEC_VAR_POWER_ON_CLEARED_NO_MATCH
#else
#error "Os_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_POWER_ON_CLEARED

#endif /* START_WITH_IF */
#include "Os_MemmapImp.h"
/*=====[E N D   O F   F I L E]=====*//* PRQA S 0883 */ /* MISRA  Dir-4.10*/ /* OS_MEMMAP_INCLUDE_001*/
