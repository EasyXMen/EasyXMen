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
 **  @file               : Dcm_Dsd.h
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : DcmDsd Functions/Types Declaration
 **
 ***********************************************************************************************************************/
#ifndef DCM_DSD_H_
#define DCM_DSD_H_

/* =================================================== inclusions =================================================== */
#include "Dcm_Type.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================= external function declarations ========================================= */
/**
 * @brief         Called by Dcm_TpRxIndication to check service request validity
 * @param[in]     protocolId : current protocol Id
 * @return        void
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void DsdInternal_RxIndication(uint8 protocolId);

/**
 * @brief         Called by DslInternal_SetStateIdle do serviceRequest notification confirmation
 * @param[in]     protocolId         : current protocol Id
 * @param[in]     confirmationStatus : curruent confirmation status
 * @return        void
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void DsdInternal_HandleConfirmation(uint8 protocolId, Dcm_ConfirmationStatusType confirmationStatus);

/**
 * @brief         This function sends diagnostic response
 * @param[in]     ProtocolId : The index of the protocol
 * @param[in]     errorCode  : The NRC error code
 * @return        void
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void DsdInternal_SendResponse(uint8 ProtocolId, Dcm_NegativeResponseCodeType errorCode);

/**
 * @brief         Called by DsdInternal_RxIndication to check service validity and output configuration index
 * @param[in]     protocolId : current protocol Id
 * @param[in]     Sid        : the requested ServiceId(if have)
 * @param[out]    servIndex  : the configuration index
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */

Std_ReturnType DsdInternal_CheckService(uint8 protocolId, uint16* servIndex, const uint8* Sid);

/**
 * @brief         Called by DsdInternal_RxIndication to check subService validity and output configuration index
 * @param[in]     protocolId   : current protocol Id
 * @param[in]     servIndex    : the configuration index of service
 * @param[in]     SubServiceId : the requested subServiceId(if have)
 * @param[out]    subServIndex : the configuration index
 * @param[in]     ErrorCode  : The NRC error code
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType DsdInternal_CheckSubService(
    uint8                         protocolId,
    uint16                        servIndex,
    uint16*                       subServIndex,
    const uint8*                  SubServiceId,
    Dcm_NegativeResponseCodeType* ErrorCode);

#ifdef __cplusplus
}
#endif
#endif
