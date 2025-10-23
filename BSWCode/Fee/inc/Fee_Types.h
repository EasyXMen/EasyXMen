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
 **  @file               : Fee_Types.h
 **  @author             : peng.wu
 **  @date               : 2024/09/15
 **  @vendor             : isoft
 **  @description        : Type definitions of FEE
 **
 ***********************************************************************************************************************/
#ifndef FEE_TYPES_H
#define FEE_TYPES_H

/* =================================================== inclusions =================================================== */
#include "Std_Types.h"
#include "Fee_InternalTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */

/* ===================================================== macros ===================================================== */
/** Service ID */
#define FEE_INIT_ID                0x00u /** Fee_Init */
#define FEE_SETMODE_ID             0x01u /** Fee_SetMode */
#define FEE_READ_ID                0x02u /** Fee_Read */
#define FEE_WRITE_ID               0x03u /** Fee_Write */
#define FEE_CANCEL_ID              0x04u /** Fee_Cancel */
#define FEE_GETSTATUS_ID           0x05u /** Fee_GetStatus */
#define FEE_GETJOBRESULT_ID        0x06u /** Fee_GetJobResult */
#define FEE_INVALIDATEBLOCK_ID     0x07u /** Fee_InvalidateBlock */
#define FEE_GETVERSIONINFO_ID      0x08u /** Fee_GetVersionInfo */
#define FEE_ERASEIMMEDIATEBLOCK_ID 0x09u /** Fee_EraseImmediateBlock */
#define FEE_JOBENDNOTIFICATION_ID  0x10u /** Fee_JobEndNotification */
#define FEE_MAINFUNCTION_ID        0x12u /** Fee_MainFunction */

/** Development Errors */
#define FEE_E_NO_ERROR          0x0u  /** API function called with no det error */
#define FEE_E_UNINIT            0x01u /** API service called when module was not initialized */
#define FEE_E_INVALID_BLOCK_NO  0x02u /** API service called with invalid block number */
#define FEE_E_INVALID_BLOCK_OFS 0x03u /** API service called with invalid block offset */
#define FEE_E_PARAM_POINTER     0x04u /** API service called with invalid data pointer */
#define FEE_E_INVALID_BLOCK_LEN 0x05u /** API service called with invalid length information */

/** Runtime Errors */
#define FEE_E_BUSY           0x06u /** API service called while module is busy processing a user request */
#define FEE_E_INVALID_CANCEL 0x08u /** Fee_Cancel called while no job was pending */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

#ifdef __cplusplus
}
#endif
#endif
