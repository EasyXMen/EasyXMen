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
 **  @file               : Dem_OptMacros.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : Implementation file of Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_OPTMACROS_H
#define DEM_OPTMACROS_H

/* =================================================== inclusions =================================================== */
#include "Dcm_Cfg.h"
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
/* PRQA S 3332 ++ */ /* VL_Dem_3332 */
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

/** Users can choose whether to use the Dem_SetWIRStatus api, with the default configuration set to disabled. */
#ifndef DEM_USER_CONTROLLED_WIR
#define DEM_USER_CONTROLLED_WIR STD_OFF
#endif

/** To optimize code memory usage, users can enable or disable Dem_GetEventMemoryOverflow and
 * Dem_GetNumberOfEventMemoryEntries according to the actual project scenario. */
#ifndef DEM_USER_SUPPORT_OVFLIND_API
#define DEM_USER_SUPPORT_OVFLIND_API STD_OFF
#endif

/** To optimize code memory usage, users can enable or disable Dem_SetPtoStatus according to the actual project
 * scenario. */
#ifndef DEM_USER_SUPPORT_SET_PTO_API
#define DEM_USER_SUPPORT_SET_PTO_API STD_OFF
#endif

/** To optimize code memory usage, users can enable or disable Dem_SetDataOfPID21, Dem_SetDataOfPID31,
 * Dem_SetDataOfPID4D, Dem_SetDataOfPID4E according to the actual project scenario. */
#ifndef DEM_USER_SUPPORT_SET_PID_API
#define DEM_USER_SUPPORT_SET_PID_API STD_OFF
#endif

/** To optimize code memory usage, define whether to support the Dem_SelectDTC API. */
#if (                                                                                                                 \
    (STD_ON == DCM_OBD_0X4) || (STD_ON == DCM_UDS_0X14) || (defined(DCM_UDS_0X19_0X4)) || (defined(DCM_UDS_0X19_0X6)) \
    || (defined(DCM_UDS_0X19_0XD)) || (defined(DCM_UDS_0X19_0XE)))
#define DEM_SUPPORT_SELECTDTC_API STD_ON
#else
#define DEM_SUPPORT_SELECTDTC_API STD_OFF
#endif

/** To optimize code memory usage, define whether to support the Dem_GetTranslationType API. */
#if (defined(DCM_UDS_0X19_0X1))
#define DEM_SUPPORT_GETTRANSLATIONTYPE_API STD_ON
#else
#define DEM_SUPPORT_GETTRANSLATIONTYPE_API STD_OFF
#endif

/** To optimize code memory usage, define whether to support the Dem_GetDTCStatusAvailabilityMask API. */
#if (                                                                                           \
    (STD_ON == DCM_UDS_0X19_0X2) || (STD_ON == DCM_UDS_0X19_0XA) || (defined(DCM_UDS_0X19_0X1)) \
    || (defined(DCM_UDS_0X19_0XD)) || (defined(DCM_UDS_0X19_0XE)))
#define DEM_SUPPORT_GETDTCSTATUSAVAILABILITYMASK_API STD_ON
#else
#define DEM_SUPPORT_GETDTCSTATUSAVAILABILITYMASK_API STD_OFF
#endif

/** To optimize code memory usage, define whether to support the Dem_GetStatusOfDTC API. */
#if (                                                                                           \
    (STD_ON == DCM_UDS_0X19_0X4) || (STD_ON == DCM_UDS_0X19_0X6) || (defined(DCM_UDS_0X19_0XD)) \
    || (defined(DCM_UDS_0X19_0XE)))
#define DEM_SUPPORT_GETSTATUSOFDTC_API STD_ON
#else
#define DEM_SUPPORT_GETSTATUSOFDTC_API STD_OFF
#endif

/** To optimize code memory usage, define whether to support the Dem_GetSeverityOfDTC API. */
#if (STD_ON == DCM_UDS_0X19_0X9)
#define DEM_SUPPORT_GETSEVERITYOFDTC_API STD_ON
#else
#define DEM_SUPPORT_GETSEVERITYOFDTC_API STD_OFF
#endif

/** To optimize code memory usage, define whether to support the Dem_SetDTCFilter and Dem_GetNumberOfFilteredDTC API. */
#if (                                                                                                             \
    (STD_ON == DCM_OBD_0X3) || (STD_ON == DCM_OBD_0X7) || (STD_ON == DCM_OBD_0XA) || (STD_ON == DCM_UDS_0X19_0X1) \
    || (STD_ON == DCM_UDS_0X19_0X2) || (STD_ON == DCM_UDS_0X19_0XA) || (STD_ON == DCM_UDS_0X19_0X14))
#define DEM_SUPPORT_SETDTCFILTER_AND_GETNUMBER_API STD_ON
#else
#define DEM_SUPPORT_SETDTCFILTER_AND_GETNUMBER_API STD_OFF
#endif

/** To optimize code memory usage, define whether to support the Dem_GetNextFilteredDTC API. */
#if (                                                                                                             \
    (STD_ON == DCM_OBD_0X3) || (STD_ON == DCM_OBD_0X7) || (STD_ON == DCM_OBD_0XA) || (STD_ON == DCM_UDS_0X19_0X2) \
    || (STD_ON == DCM_UDS_0X19_0XA))
#define DEM_SUPPORT_GETNEXT_FILTEREDDTC_API STD_ON
#else
#define DEM_SUPPORT_GETNEXT_FILTEREDDTC_API STD_OFF
#endif

/** To optimize code memory usage, define whether to support the Dem_GetNextFilteredDTCAndFDC API. */
#if (STD_ON == DCM_UDS_0X19_0X14)
#define DEM_SUPPORT_GETNEXT_FILTEREDDTCANDFDC_API STD_ON
#else
#define DEM_SUPPORT_GETNEXT_FILTEREDDTCANDFDC_API STD_OFF
#endif

/** To optimize code memory usage, define whether to support the Dem_GetNextFilteredDTCAndSeverity API. */
#if ((STD_ON == DCM_UDS_0X19_0X8) || (STD_ON == DCM_UDS_0X19_0X42))
#define DEM_SUPPORT_GETNEXT_FILTEREDDTCANDSEVERITY_API STD_ON
#else
#define DEM_SUPPORT_GETNEXT_FILTEREDDTCANDSEVERITY_API STD_OFF
#endif

/** To optimize code memory usage, define whether to support the Dem_SetFreezeFrameRecordFilter,
 * Dem_GetNumberOfFreezeFrameRecords and Dem_GetNumberOfFreezeFrameRecords API. */
#if (STD_ON == DCM_UDS_0X19_0X3)
#define DEM_SUPPORT_SETFREEZEFRAMERECORD_AND_GET_API STD_ON
#else
#define DEM_SUPPORT_SETFREEZEFRAMERECORD_AND_GET_API STD_OFF
#endif

/** To optimize code memory usage, define whether to support the Dem_GetDTCByOccurrenceTime API. */
#if ((defined(DCM_UDS_0X19_0XE)) || (defined(DCM_UDS_0X19_0XD)))
#define DEM_SUPPORT_GETDTCOCCURRENCETIME_API STD_ON
#else
#define DEM_SUPPORT_GETDTCOCCURRENCETIME_API STD_OFF
#endif

/** To optimize code memory usage, define whether to support the Dem_DisableDTCRecordUpdate and
 * Dem_EnableDTCRecordUpdate API. */
#if ((STD_ON == DCM_UDS_0X19_0X4) || (STD_ON == DCM_UDS_0X19_0X6))
#define DEM_SUPPORT_DISABLE_ENABLE_DTCRECORD_UPDATE_API STD_ON
#else
#define DEM_SUPPORT_DISABLE_ENABLE_DTCRECORD_UPDATE_API STD_OFF
#endif

/** To optimize code memory usage, define whether to support the Dem_SelectExtendedDataRecord,
 * Dem_GetSizeOfExtendedDataRecordSelection and Dem_GetNextExtendedDataRecord API. */
#if (STD_ON == DCM_UDS_0X19_0X6)
#define DEM_SUPPORT_SELECT_GETSIZE_GETNEXT_EXTENDEDDATARECORD_API STD_ON
#else
#define DEM_SUPPORT_SELECT_GETSIZE_GETNEXT_EXTENDEDDATARECORD_API STD_OFF
#endif

/** To optimize code memory usage, define whether to support the Dem_SelectFreezeFrameData,
 * Dem_GetSizeOfFreezeFrameSelection and Dem_GetNextFreezeFrameData API. */
#if (STD_ON == DCM_UDS_0X19_0X4)
#define DEM_SUPPORT_SELECT_GETSIZE_GETNEXT_FREEZEFRAMEDATA_API STD_ON
#else
#define DEM_SUPPORT_SELECT_GETSIZE_GETNEXT_FREEZEFRAMEDATA_API STD_OFF
#endif

/** To optimize code memory usage, define whether to support the Dem_GetDTCSeverityAvailabilityMask API. */
#if (STD_ON == DCM_UDS_0X19_0X42)
#define DEM_SUPPORT_GETDTCSERVERITYAVAILABILITYMASK_API STD_ON
#else
#define DEM_SUPPORT_GETDTCSERVERITYAVAILABILITYMASK_API STD_OFF
#endif

/** To optimize code memory usage, define whether to support the Dem_SetDTCFilterByExtendedDataRecordNumber API. */
#if (STD_ON == DCM_UDS_0X19_0X1A)
#define DEM_SUPPORT_SETDTCFILTERBYEXTENDEDDATARECORDNUMBER_API STD_ON
#else
#define DEM_SUPPORT_SETDTCFILTERBYEXTENDEDDATARECORDNUMBER_API STD_OFF
#endif

/** To optimize code memory usage, define whether to support the Dem_SetDTCFilterByReadinessGroup API. */
#if (STD_ON == DCM_UDS_0X19_0X56)
#define DEM_SUPPORT_SETDTCFILTERBYREADINESSGROUP_API STD_ON
#else
#define DEM_SUPPORT_SETDTCFILTERBYREADINESSGROUP_API STD_OFF
#endif
/* PRQA S 3332 -- */
/** To optimize code memory usage, define whether to support the Dem_ClearDTC API. */
#if ((STD_ON == DCM_OBD_0X4) || (STD_ON == DCM_UDS_0X14))
#define DEM_SUPPORT_CLEARDTC_API STD_ON
#else
#define DEM_SUPPORT_CLEARDTC_API STD_OFF
#endif

#endif
