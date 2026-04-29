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
 **  @file               : Eep_62_Cbk.h
 **  @author             : peng.wu
 **  @date               : 2024/12/23
 **  @vendor             : isoft
 **  @description        : Callback API declaration of Eep
 **
 ***********************************************************************************************************************/

#ifndef EEP_62_CBK_H
#define EEP_62_CBK_H

/* =================================================== inclusions =================================================== */
#include "Std_Types.h"
#include "Eep_62_Cfg.h"

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
 * @brief       This callback function provided by the module user is called when a job has been completed with a
 * positive result.
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76987
 */
extern void Eep_62_JobEndNotification(void);

/**
 * @brief       This callback function provided by the module user is called when a job has been canceled or finished
 * with negative result.
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76988
 */
extern void Eep_62_JobErrorNotification(void);

#ifdef __cplusplus
}
#endif
#endif
