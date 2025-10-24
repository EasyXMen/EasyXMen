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
 ***********************************************************************************************************************
 **
 **  @file               : Csm_Cbk.h
 **  @author             : jie.gu
 **  @date               : 2025/03/31
 **  @vendor             : isoft
 **  @description        : Csm callback Implementation header
 **
 **********************************************************************************************************************/

#ifndef CSM_CBK_H
#define CSM_CBK_H

/* =================================================== inclusions =================================================== */
#include "Csm.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief the CSM that a job has finished. This function is used by the
 * underlying layer (CRYIF).
 * @synchronous    TRUE
 * @reentrant Reentrant
 * @param[in]    job: Holds a pointer to the job, which has finished.
 * @param[in]    result: Contains the result of the cryptographic operation
 * @trace       CPD-71204
 * @implements CPD-49641
 */
/* PRQA S 3432 ++ */ /* VL_Csm_3432 */
void Csm_CallbackNotification(Crypto_JobType* job, Crypto_ResultType result);
/* PRQA S 3432 -- */

#ifdef __cplusplus
}
#endif

#endif /* CSM_CBK_H */
