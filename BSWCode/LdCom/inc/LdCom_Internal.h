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
 **  @file               : LdCom_Internal.h
 **  @author             : fupeng.yu
 **  @date               : 2024/08/26
 **  @vendor             : isoft
 **  @description        : Large Data Communication internal header
 **
 ***********************************************************************************************************************/

#ifndef LDCOM_INTERNAL_H_
#define LDCOM_INTERNAL_H_

/* =================================================== inclusions =================================================== */
#include "LdCom.h"

#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
#include "Det.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */
/** LdCom local var or function */
#if !defined(LDCOM_LOCAL)
#define LDCOM_LOCAL static /* PRQA S 3414 */ /* VL_QAC_KeyWord */
#endif

/** LdCom inline function */
#if !defined(LDCOM_LOCAL_INLINE)
#define LDCOM_LOCAL_INLINE static inline
#endif

/** Invalid PduId for LdCom */
#define LDCOM_PDU_ID_INVALID COMSTACK_PDUID_INVALID

/**
 * @brief Unused function parameters
 */
#define LDCOM_NOUSED(paramter) (void)(paramter)

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
/**
 * report detect of the LdCom module
 */
LDCOM_LOCAL_INLINE void LDCOM_DET_REPORT(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportError(LDCOM_MODULE_ID, LDCOM_INSTANCE_ID, (ApiId), (ErrorId));
}
#endif

/**
 * report detect of the LdCom module
 */
LDCOM_LOCAL_INLINE PduIdType LdCom_GetIpduIdFromUserId(PduIdType Id, const PduIdType* MapPtr)
{
    return MapPtr[Id];
}
/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

#ifdef __cplusplus
}
#endif

#endif
