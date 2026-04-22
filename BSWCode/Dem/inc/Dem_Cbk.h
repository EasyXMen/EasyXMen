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
 **  @file               : Dem_Cbk.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : Implementation file of Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_CBK_H
#define DEM_CBK_H

/* =================================================== inclusions =================================================== */
#include "Dem_Lcfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================= external function declarations ========================================= */
/**
 * @brief         Initializes the NvBlock for administrative data
 *                supposed to be called by the NvM in order to (re)initialize the data in case the non-volatile
 *                memory has never been stored, or was corrupted. (See NvMBlockDescriptor/NvMInitBlockCallback) It
 *                can also be used to force a reinitialization of the Dem data triggered by the application (e.g.
 *                after a new software version has been flashed to the ECU). In the latter case, make sure the
 *                function is not called while the Dem is active.
 * @param[in]     InitBlockRequest: The request type of the currentlyprocessed block
 * @return        Std_ReturnType
 * @retval        E_OK: Processing success
 * @retval        E_NOT_OK: Processing Failure
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-67518
 */
/* PRQA S 1512 EOF */ /* VL_Dem_1512 */
extern Std_ReturnType Dem_NvMInitAdminData(NvM_InitBlockRequestType InitBlockRequest);

/**
 * @brief         Initializes the NvBlock for event status data
 *                supposed to be called by the NvM in order to (re)initialize the data in case the non-volatile
 *                memory has never been stored, or was corrupted. (See NvMBlockDescriptor/NvMInitBlockCallback) It
 *                can also be used to force a reinitialization of the Dem data triggered by the application (e.g.
 *                after a new software version has been flashed to the ECU). In the latter case, make sure the
 *                function is not called while the Dem is active.
 * @param[in]     InitBlockRequest: The request type of the currentlyprocessed block
 * @return        Std_ReturnType
 * @retval        E_OK: Processing success
 * @retval        E_NOT_OK: Processing Failure
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-67519
 */
extern Std_ReturnType Dem_NvMInitStatusData(NvM_InitBlockRequestType InitBlockRequest);

#if (DEM_DEBOUNCE_COUNTER_STORAGE_NUM > 0u)
/**
 * @brief         Initializes the NvBlock for persisted debounce values
 *                supposed to be called by the NvM in order to (re)initialize the data in case the non-volatile
 *                memory has never been stored, or was corrupted. (See NvMBlockDescriptor/NvMInitBlockCallback) It
 *                can also be used to force a reinitialization of the Dem data triggered by the application (e.g.
 *                after a new software version has been flashed to the ECU). In the latter case, make sure the
 *                function is not called while the Dem is active.
 * @param[in]     InitBlockRequest: The request type of the currentlyprocessed block
 * @return        Std_ReturnType
 * @retval        E_OK: Processing success
 * @retval        E_NOT_OK: Processing Failure
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-67522
 */
extern Std_ReturnType Dem_NvMInitDebounceData(NvM_InitBlockRequestType InitBlockRequest);
#endif

/**
 * @brief         Notifies the Dem module about a completed NV operation
 *                This function has to be called by the NvM after a write operation has finished.
 *                (See NvMBlockDescriptor/NvMSingleBlockCallback)
 * @param[in]     ServiceId: Service identifier
 * @param[in]     JobResult: Result of the NV job
 * @return        Std_ReturnType
 * @retval        E_OK: Processing success
 * @retval        E_NOT_OK: Processing Failure
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-67541
 */
extern Std_ReturnType Dem_NvMJobFinished(NvM_BlockRequestType ServiceId, NvM_RequestResultType JobResult);

#ifdef __cplusplus
}
#endif

#endif
