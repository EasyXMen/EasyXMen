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
 **  @file               : Dem_InitState.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : Implementation file of Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_INITSTATUS_H
#define DEM_INITSTATUS_H

/* =================================================== inclusions =================================================== */
#include "Dem_PBcfg.h"

/* ===================================================== macros ===================================================== */
/**
 * @name  DEM_INITIALIZATION_STATE_ENCODE
 * @{
 * @brief Macros encoding for initialization state
 */
#define DEM_UNINITIALIZED  (0u) /**< Initialization state directly after system startup */
#define DEM_PREINITIALIZED (1u) /**< Initialization state after Dem_PreInit */
#define DEM_SHUTDOWN       (2u) /**< Initialization state after Dem_Shutdown */
#define DEM_INITIALIZED    (3u) /**< Initialization state after Dem_Init */
/** @} */

/* ============================================ internal data definitions =========================================== */
#define DEM_START_SEC_VAR_CLEARED_8
#include "Dem_MemMap.h"
/**
 * @brief     Type definition for master initialization status
 * @range       0 ~ 3
 */
DEM_LOCAL uint8 Dem_MasterInitStatus;
#define DEM_STOP_SEC_VAR_CLEARED_8
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dem_MemMap.h"
/**
 * @brief     Pointer to global configuration parameter
 * @range NA
 */
const Dem_ConfigType* Dem_GlobalConfigPtr; /* PRQA S 3406, 1504 */ /* VL_Dem_3406, VL_Dem_1504 */
#define DEM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dem_MemMap.h"

/* ========================================== internal function definitions ========================================= */
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/****************Master*******************/
/**
 * @brief         Set Master Init Status
 * @param[in]     InitStatus: Init Status
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetMasterInitStatus(uint8 InitStatus)
{
    Dem_MasterInitStatus = InitStatus;
}

/* PRQA S 4304 ++ */ /* VL_Dem_4304 */
/**
 * @brief         Check Master Init Status is the same
 * @param[in]     InitStatus: Init Status
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckMasterInitStatus(uint8 InitStatus)
{
    return (boolean)(Dem_MasterInitStatus == InitStatus);
}

/**
 * @brief         Check Master Init Status is at least
 * @param[in]     InitStatus: Init Status
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckLeastMasterInitStatus(uint8 InitStatus)
{
    return (boolean)(Dem_MasterInitStatus >= InitStatus);
}
/* PRQA S 4304 -- */

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
