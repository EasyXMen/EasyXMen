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
 **  @file               : Dem_Obd.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : Implementation file of Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_OBD_H
#define DEM_OBD_H

/* =================================================== inclusions =================================================== */
#include "Dem_Cfg.h"
#if (DEM_OBDII_SUPPORT == STD_ON)
#include "Dem_DtcInterface.h"

/* ========================================== internal function definitions ========================================= */
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"

#if (DEM_PID_CLASS_NUMBER > 0u)
/**
 * @brief         Get DemPidIdentifier information based on Pid Index
 * @param[in]     PidIndex: PID index
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetPidIdentifier(uint8 PidIndex)
{
    return Dem_GlobalConfigPtr->PBPidClass[PidIndex];
}
#endif

/**
 * @brief         check Event Obd Related
 * @param[in]     EventId: Identification of an event by assigned EventId.
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckEventObdRelated(Dem_EventIdType EventId)
{
    return (Dem_GetEventObdDTC(EventId) != DEM_OBD_DTC_INVALID_VALUE);
}

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif

#endif
