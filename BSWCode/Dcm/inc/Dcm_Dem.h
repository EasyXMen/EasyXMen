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
 **  @file               : Dcm_Dem.h
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm Public Functions Declaration For Dem
 **
 ***********************************************************************************************************************/
#ifndef DCM_DEM_H_
#define DCM_DEM_H_

/* =================================================== inclusions =================================================== */
#include "Rte_Dem_Type.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================= external function declarations ========================================= */
/**
 * @brief         Triggers on changes of the UDS status byte. Allows to trigger on ROE Event for subservice On
 *                DTCStatusChanged.
 * @param[in]     DTC          : This is the DTC the change trigger is assigned to.
 * @param[in]     DTCStatusOld : DTC status before change
 * @param[in]     DTCStatusNew : DTC status after change
 * @return        Std_ReturnType
 * @retval        E_OK : this value is always returned.
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-60695
 */
Std_ReturnType
    Dcm_DemTriggerOnDTCStatus(uint32 DTC, Dem_UdsStatusByteType DTCStatusOld, Dem_UdsStatusByteType DTCStatusNew);

#ifdef __cplusplus
}
#endif
#endif
