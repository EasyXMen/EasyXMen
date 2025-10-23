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
 **  @file               : Dem_OptMacros.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : Implementation file of Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_OPTMACROS_H
#define DEM_OPTMACROS_H

/* ===================================================== macros ===================================================== */

/** This macro definition represents an internal function of the Dem module */
#ifndef DEM_LOCAL
#define DEM_LOCAL static /* PRQA S 3414 */ /* VL_Dem_3414 */
#endif

/** This macro definition represents the Dem module inline function */
#ifndef DEM_LOCAL_INLINE
#define DEM_LOCAL_INLINE static inline
#endif

/** This macro definition represents the Dem module unused parameter */
#if !defined(DEM_PARAM_UNUSED)
#define DEM_PARAM_UNUSED(x) (void)(x)
#endif

/** This macro definition indicates the data copy, setup, and comparison of whether or not the library functions
 * provided by ISoft are used.ST_ON then indicates that they are used, and STD_OFF indicates that they are used with the
 * functions of the per-byte processing of the Dem module. Using the library function provided by ISoft has better
 * performance. */
#ifndef DEM_FEATURE_MEMCOPY_USED_ILIB
#define DEM_FEATURE_MEMCOPY_USED_ILIB STD_OFF
#endif

/** Define whether to poll the NvM job results instead of relying on job end notifications. */
#ifndef DEM_FEATURE_SUPPORT_NVM_POLLING
#define DEM_FEATURE_SUPPORT_NVM_POLLING STD_OFF
#endif

/** In order to optimize the code space occupation, a macro definition is added to control whether the interface for SWC
 * to obtain the extended data of the frozen frame is enabled or not. */
#ifndef DEM_FEATURE_SWC_GET_FF_ED
#define DEM_FEATURE_SWC_GET_FF_ED STD_OFF
#endif

/** In order to optimize the code space occupation, a macro definition is added to control whether the interface for Dcm
 * to obtain the 0x19 service 0x09 sub service information is enabled or not. */
#ifndef DEM_FEATURE_DCM_0X19_0X09_ENABLE
#define DEM_FEATURE_DCM_0X19_0X09_ENABLE STD_OFF
#endif

/** In order to optimize the code space occupation, a macro definition is added to control whether the interface for Dcm
 * to obtain the 0x19 service 0x14 sub service information is enabled or not. */
#ifndef DEM_FEATURE_DCM_0X19_0X14_ENABLE
#define DEM_FEATURE_DCM_0X19_0X14_ENABLE STD_OFF
#endif

/** In order to optimize the code space occupation, a macro definition is added to control whether the interface for Dcm
 * to obtain the 0x19 service 0x08 sub service information is enabled or not. */
#ifndef DEM_FEATURE_DCM_0X19_0X08_ENABLE
#define DEM_FEATURE_DCM_0X19_0X08_ENABLE STD_OFF
#endif

/** In order to optimize the code space occupation, a macro definition is added to control whether the interface for Dcm
 * to obtain the 0x19 service 0x42 sub service information is enabled or not. */
#ifndef DEM_FEATURE_DCM_0X19_0X42_ENABLE
#define DEM_FEATURE_DCM_0X19_0X42_ENABLE STD_OFF
#endif

/** Define whether to limitation state for event process.Define the number of events that need to be scanned in one main
 * function cycle when clearing.
 * 0: means not enable
 * 1 to 65535: means enable
 */
#ifndef DEM_PROCESS_EVENT_LIMIT_NUMBER
#define DEM_PROCESS_EVENT_LIMIT_NUMBER 0u
#endif

/** Define whether to limitation state for clear requests.Define the number of events that need to be scanned by the
 * requesting storage source in one main function cycle when clearing.
 * 0: means not enable
 * 1 to 65535: means enable
 */
#ifndef DEM_CLEAR_DTC_LIMIT_NUMBER
#define DEM_CLEAR_DTC_LIMIT_NUMBER 0u
#endif

/** Implement and use a hardware optimized Dem_UserDefineSyncCompareAndSwap() function.
 * The API Dem_UserDefineSyncCompareAndSwap(uint32* AddressPtr, uint32 OldValue, uint32 NewValue) performs an atomic
 * compare and swap of memory contents. If disabled, the DEM uses an internal implementation, that needs the critical
 * section 'CrossCoreComm'. If enabled, the DEM uses an external implementation. */
#ifndef DEM_USER_DEFINED_SYNC_API
#define DEM_USER_DEFINED_SYNC_API STD_OFF
#endif

/** For performance optimization, if an error occurs when the enabling condition for storage is not met, the enabling
 * condition of storage will be restored and the fault will be triggered, which will result in the event not being
 * stored. Unless a pass is triggered during the process, storage checks are performed by default. */
#ifndef DEM_FEATURE_RETRY_STORAGE
#define DEM_FEATURE_RETRY_STORAGE STD_ON
#endif

#endif
