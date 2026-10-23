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
 **  @file               : StbM_CanTSyn.h
 **  @author             : xiaojian.liang
 **  @date               : 2025/11/05
 **  @vendor             : isoft
 **  @description        : Function of StbM Implementation header for CanTSyn
 **
 ***********************************************************************************************************************/

#ifndef STBM_CAN_TSYN_H_
#define STBM_CAN_TSYN_H_

/* =================================================== inclusions =================================================== */
#include "StbM_Cfg.h"

#if STBM_CAN_SLAVE_TIME_VALIDATION_USED == STD_ON || STBM_CAN_MASTER_TIME_VALIDATION_USED == STD_ON
#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
#if STBM_CAN_SLAVE_TIME_VALIDATION_USED == STD_ON
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief        Allows the CanTSyn Module to forward CAN specific details to the StbM.
 * @param[in]    timeBaseId      Time Base reference
 * @param[in]    measureDataPtr  New measurement data
 * @retval       E_OK            successful
 * @retval       E_NOT_OK        failed
 * @reentrant    FALSE
 * @synchronous  TRUE
 * @trace   CPD-85945
 */
Std_ReturnType StbM_CanSetSlaveTimingData(
    StbM_SynchronizedTimeBaseType           timeBaseId,
    const StbM_CanTimeSlaveMeasurementType* measureDataPtr);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#if STBM_CAN_MASTER_TIME_VALIDATION_USED == STD_ON
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/**
 * @brief        Provides CAN Timesyn module specific data for a Time Master to the StbM.
 * @param[in]    timeBaseId      Time Base reference
 * @param[out]   measureDataPtr  New measurement data
 * @retval       E_OK            successful
 * @retval       E_NOT_OK        failed
 * @reentrant    FALSE
 * @synchronous  TRUE
 * @trace   CPD-85449
 */
Std_ReturnType StbM_CanSetMasterTimingData(
    StbM_SynchronizedTimeBaseType            timeBaseId,
    const StbM_CanTimeMasterMeasurementType* measureDataPtr);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif

#endif

#ifdef __cplusplus
}
#endif

#endif
