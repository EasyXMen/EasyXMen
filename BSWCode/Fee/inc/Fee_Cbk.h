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
 **  @file               : Fee_Cbk.h
 **  @author             : peng.wu
 **  @date               : 2024/09/15
 **  @vendor             : isoft
 **  @description        : Callback API declaration of FEE
 **
 ***********************************************************************************************************************/
#ifndef FEE_CBK_H
#define FEE_CBK_H

/* =================================================== inclusions =================================================== */
#include "NvM.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief       Service to report to this module the successful end of an asynchronous operation
 * @reentrant   Non Reentrant
 * @synchronous Synchronous
 * @trace       CPD-61300
 */
extern void Fee_JobEndNotification(void);

#ifdef __cplusplus
}
#endif
#endif
