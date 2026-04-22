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
 **  @file               : Ea_Types.h
 **  @author             : peng.wu
 **  @date               : 2024/12/11
 **  @vendor             : isoft
 **  @description        : Type definitions of Ea
 **
 ***********************************************************************************************************************/
#ifndef EA_TYPES_H
#define EA_TYPES_H

/* =================================================== inclusions =================================================== */
#include "Std_Types.h"
#include "Ea_Cfg.h" /** to get Eep_AddressType and Eep_LengthType from Eep */

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */

/* ===================================================== macros ===================================================== */
/**
 * @name EA_DEVELOP_ERROR_ID
 * @{
 * @brief Develop Error Definition
 */
#define EA_E_NO_ERROR          (0x0u)  /**< API function called with no det error */
#define EA_E_UNINIT            (0x01u) /**< API service called while module is not (yet) initialized */
#define EA_E_INVALID_BLOCK_NO  (0x02u) /**< API service called with invalid block number */
#define EA_E_INVALID_BLOCK_OFS (0x03u) /**< API service called with invalid block offset */
#define EA_E_PARAM_POINTER     (0x04u) /**< API service called with invalid pointer argument */
#define EA_E_INVALID_BLOCK_LEN (0x05u) /**< API service called with invalid block length information */

/**
 * @name EA_RUNTIME_ERROR_ID
 * @{
 * @brief Runtime Error Definition
 */
#define EA_E_BUSY           (0x06u) /**< API service called while module is busy */
#define EA_E_INVALID_CANCEL (0x08u) /**< Ea_Cancel called while no job was pending */

/**
 * @name EA_SERVICE_ID
 * @{
 * @brief SERVICE ID Definition
 */
#define EA_SID_INIT                 (0x00u) /**< Ea_Init */
#define EA_SID_SET_MODE             (0x01u) /**< Ea_SetMode */
#define EA_SID_READ                 (0x02u) /**< Ea_Read */
#define EA_SID_WRITE                (0x03u) /**< Ea_Write */
#define EA_SID_CANCEL               (0x04u) /**< Ea_Cancel */
#define EA_SID_GETSTATUS            (0x05u) /**< Ea_GetStatus */
#define EA_SID_GETJOBRESULT         (0x06u) /**< Ea_GetJobResult */
#define EA_SID_INVALIDATEBLOCK      (0x07u) /**< Ea_InvalidateBlock */
#define EA_SID_GETVERSIONINFO       (0x08u) /**< Ea_GetVersionInfo */
#define EA_SID_ERASEIMMEDIATE_BLOCK (0x09u) /**< Ea_EraseImmediateBlock */
#define EA_SID_JOBENDNOTIFICATION   (0x10u) /**< Ea_JobEndNotification */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

#ifdef __cplusplus
}
#endif
#endif
