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
 ***********************************************************************************************************************
 **
 **  @file               : CryIf_Cbk.h
 **  @author             : jie.gu
 **  @date               : 2025/03/31
 **  @vendor             : isoft
 **  @description        : CryIf callback Implementation header
 **
 **********************************************************************************************************************/

#ifndef CRYIF_CBK_H
#define CRYIF_CBK_H

/* =================================================== inclusions =================================================== */
#include "CryIf.h"

#ifdef __cplusplus
extern "C" {
#endif
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief          Callback function for Crypto Job notification.
 *                 This function is called by the Crypto driver to notify the
 *                 application about the completion of a Crypto Job.
 * @param[in]      job : Pointer to a Crypto Job structure.
 *                 This parameter is a pointer to a structure that contains
 *                 information about the Crypto Job, such as the job ID, the
 *                 input data, and the output data.
 * @param[in]      result : Result of the Crypto Job.
 *                 This parameter indicates whether the Crypto Job was successful
 *                 or not. It can be one of the following values:
 *                 - CRYPTO_E_OK: The Crypto Job was successful.
 *                 - CRYPTO_E_NOT_OK: The Crypto Job failed.
 * @return         None
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace          CPD-71588
 */
void CryIf_CallbackNotification(Crypto_JobType* job, Crypto_ResultType result);

#ifdef __cplusplus
}
#endif

#endif /* CryIf_CBK_H */
