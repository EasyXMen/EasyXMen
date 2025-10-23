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
 **  @file               : Dem_MasterSatelliteCom.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : Implementation file of Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_MASTERSATELLITECOM_H
#define DEM_MASTERSATELLITECOM_H

/* =================================================== inclusions =================================================== */
#include "Dem_Lcfg.h"

/* ===================================================== macros ===================================================== */
#define DEM_COM_BITS_PER_WORD (sizeof(Dem_ComProcessorWordType) * 8u) /**< Number of bits per word */

/* ========================================== internal function definitions ========================================= */
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"

#if (DEM_USER_DEFINED_SYNC_API == STD_OFF)
/**
 * @brief         Default function to synchronize variable information between partitions
 * @param[in]     AddressPtr: Satellite id
 * @param[in]     OldValue: Old Value
 * @param[in]     NewValue: New Value
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_DefaultSyncCompareAndSwap(
    Dem_ComProcessorWordType* const AddressPtr,
    Dem_ComProcessorWordType        OldValue,
    Dem_ComProcessorWordType        NewValue)
{
    boolean ret = FALSE;
    /** Enter Critical Section: CrossCoreComm */
    Dem_EnterCritical_CrossCoreComm();
    if (*AddressPtr == OldValue)
    {
        *AddressPtr = NewValue;
        ret         = TRUE;
    }
    Dem_LeaveCritical_CrossCoreComm();
    /** Leave Critical Section: CrossCoreComm */
    return ret;
}
#endif

/**
 * @brief         Inter-partition variable information synchronization
 * @param[in]     AddressPtr: Satellite id
 * @param[in]     OldValue: Old Value
 * @param[in]     NewValue: New Value
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_ComSyncCompareAndSwap(
    Dem_ComProcessorWordType* const AddressPtr,
    Dem_ComProcessorWordType        OldValue,
    Dem_ComProcessorWordType        NewValue)
{
#if (DEM_USER_DEFINED_SYNC_API == STD_ON)
    return Dem_UserDefineSyncCompareAndSwap(AddressPtr, OldValue, NewValue);
#else
    return Dem_DefaultSyncCompareAndSwap(AddressPtr, OldValue, NewValue);
#endif
}

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
