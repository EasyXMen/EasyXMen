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
**  @file               : FiM_OptMacros.h
**  @author             : tao.yu
**  @date               : 2024/12/04
**  @vendor             : isoft
**  @description        : Implementation file of FiM
**
***********************************************************************************************************************/

#ifndef FIM_OPTMACROS_H
#define FIM_OPTMACROS_H

/* =================================================== inclusions =================================================== */
#include "Std_Types.h"

/* ===================================================== macros ===================================================== */

/** This macro definition represents an internal function of the FiM module */
#ifndef FIM_LOCAL
#define FIM_LOCAL static /* PRQA S 3414 */ /* VL_FiM_3414 */
#endif

/** This macro definition represents the FiM module inline function */
#ifndef FIM_LOCAL_INLINE
#define FIM_LOCAL_INLINE static inline
#endif

/** This macro definition represents the FiM module unused parameter */
#ifndef FIM_PARAM_UNUSED
#define FIM_PARAM_UNUSED(x) (void)(x)
#endif

/** This macro definition indicates the data copy, setup, and comparison of whether or not the library functions
 * provided by ISoft are used.ST_ON then indicates that they are used, and STD_OFF indicates that they are used with the
 * functions of the per-byte processing of the FiM module. Using the library function provided by ISoft has better
 * performance. */
#ifndef FIM_FEATURE_MEMCOPY_USED_ILIB
#define FIM_FEATURE_MEMCOPY_USED_ILIB STD_OFF
#endif

/** Define whether to poll the Dem status results in FiM_GetFunctionPermission Synchronous.
 * Synchronous(STD_OFF): call Dem_GetMonitorStatus in FiM_GetFunctionPermission to execute the permission state
 * Asynchronous(STD_ON): call Dem_GetMonitorStatus in FiM_MainFunction to execute the permission state
 */
#ifndef FIM_FEATURE_POLLING_MODE_ASY
#define FIM_FEATURE_POLLING_MODE_ASY STD_ON
#endif

/** Implement and use a hardware optimized FiM_UserDefineSyncCompareAndSwap() function.
 * The API FiM_UserDefineSyncCompareAndSwap(uint32* AddressPtr, uint32 OldValue, uint32 NewValue) performs an atomic
 * compare and swap of memory contents. If disabled, the FiM uses an internal implementation, that needs the critical
 * section 'CrossCoreComm'. If enabled, the FiM uses an external implementation. */
#ifndef FIM_USER_DEFINED_SYNC_API
#define FIM_USER_DEFINED_SYNC_API STD_OFF
#endif

#endif

/* ----------------------------------------------------- end of file ------------------------------------------------ */
