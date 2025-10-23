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
 **  @file               : SecOC_Externals.h
 **  @author             : HuRongbo
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : API declaration  of SecOC
 **
 ***********************************************************************************************************************/

/* ================================================ misar justifications ============================================ */

#ifndef SECOC_EXTERNALS_H
#define SECOC_EXTERNALS_H

#include "SecOC_Internal.h"
/* =================================================== inclusions =================================================== */

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */
/* PRQA S 1336, 5004 ++ */ /* VL_SecOC_1336, VL_SecOC_5004 */
/**
 * @brief    The verification status of the SecOC module.
 */
typedef void (*SecOC_VerificationStatusCalloutType)(SecOC_VerificationStatusType);
/* PRQA S 1336, 5004 -- */
/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief          This function is called by the SecOC module to notify the BSW module about the verification status.
 * @param[in]      verificationStatus: The verification status of the SecOC module.
 * @reentrant      TRUE
 * @synchronous    TRUE
 * @trace       CPD-70354
 */
void SecOC_VerifyStatus(SecOC_VerificationStatusType verificationStatus);

#ifdef __cplusplus
}
#endif
#endif /* SECOC_EXTERNALS_H */
