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
 **  @file               : Eep_62_Cbk.c
 **  @author             : peng.wu
 **  @date               : 2024/12/23
 **  @vendor             : isoft
 **  @description        : Implementation for Eep Callback
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Eep_62_Cbk.h"

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define EEP_62_START_SEC_CODE
#include "Eep_62_MemMap.h"
/**
 * @brief Interface for job end notification
 */
/* PRQA S 1503 ++ */ /* VL_Eep_62_1503 */
void Eep_62_JobEndNotification(void)
{
    return;
}

/**
 * @brief Interface for job error notification
 */
void Eep_62_JobErrorNotification(void)
{
    return;
}
/* PRQA S 1503 -- */
#define EEP_62_STOP_SEC_CODE
#include "Eep_62_MemMap.h"

/* ========================================== internal function definitions ========================================= */
