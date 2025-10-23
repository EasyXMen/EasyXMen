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
 **  @file               : Dcm_ComM.h
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm Public Functions Declaration For ComM
 **
 ***********************************************************************************************************************/
#ifndef DCM_COMM_H_
#define DCM_COMM_H_

/* =================================================== inclusions =================================================== */
#include "Std_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================= external function declarations ========================================= */
/**
 * @brief         This call informs the Dcm module about a ComM mode change to COMM_NO_COMMUNICATION.
 * @param[in]     NetworkId : Identifier of the network concerned by the mode change
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-60717
 */
void Dcm_ComM_NoComModeEntered(uint8 NetworkId);

/**
 * @brief         This call informs the Dcm module about a ComM mode change to COMM_SILENT_COMMUNICATION.
 * @param[in]     NetworkId : Identifier of the network concerned by the mode change
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-60719
 */
void Dcm_ComM_SilentComModeEntered(uint8 NetworkId);

/**
 * @brief         This call informs the Dcm module about a ComM mode change to COMM_FULL_COMMUNICATION.
 * @param[in]     NetworkId : Identifier of the network concerned by the mode change
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-60720
 */
void Dcm_ComM_FullComModeEntered(uint8 NetworkId);

#ifdef __cplusplus
}
#endif
#endif
