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
 **  @file               : Dcm_Dsp.h
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm Dsp Functions Declaration
 **
 ***********************************************************************************************************************/
#ifndef DCM_DSP_H_
#define DCM_DSP_H_
/* =================================================== inclusions =================================================== */
#include "Std_Types.h"
#include "Dcm.h"
#if (                                                                                                        \
    (STD_ON == DCM_OBD_0X3) || (STD_ON == DCM_OBD_0X7) || (STD_ON == DCM_OBD_0XA) || (STD_ON == DCM_OBD_0X4) \
    || (STD_ON == DCM_UDS_0X14) || (STD_ON == DCM_UDS_0X19) || (STD_ON == DCM_UDS_0X85))
#include "Dem.h"
#include "Rte_Dem_Type.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================= external function declarations ========================================= */
/**
 * @brief         Called by DslInternal_SetStateIdle to notify Dsp for confirmation
 * @param[in]     protocolId   : current protocol Id
 * @param[in]     connectionId : current connection Id
 * @return        void
 * @reentrant     TRUE for different protocolId
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void DspInternal_DcmConfirmation(uint8 protocolId, uint16 connectionId);

#if (STD_ON == DCM_UDS_0X86)
/**
 * @brief         Called by Dcm_DemTriggerOnDTCStatus to process roeEvent related to DTC
 * @param[in]     DTCStatusOld : DTC status before change
 * @param[in]     DTCStatusNew : DTC status after change
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void DspInternal_handleDTCRoe(Dem_UdsStatusByteType DTCStatusOld, Dem_UdsStatusByteType DTCStatusNew);
#endif
#ifdef __cplusplus
}
#endif
#endif
