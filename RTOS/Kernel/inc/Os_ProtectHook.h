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
 **  @file               : Os_ProtectHook.h
 **  @author             : i-soft-os
 **  @date               : 2024/02/10
 **  @vendor             : isoft
 **  @description        : Os header file for ProtectHook api declarations
 **
 ***********************************************************************************************************************/

#ifndef OS_PROTECTHOOK_H
#define OS_PROTECTHOOK_H

/* =================================================== inclusions =================================================== */

#include "Os_Types.h"

/* ========================================== external function definitions ========================================= */
#if (TRUE == CFG_PROTECTIONHOOK)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              ProtectionHook called by OS.
 * @param[in]          errType: Error Type.
 * @param[in]          whoHook: The object where the error occurred.
 * @return             ProtectionReturnType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
/* PRQA S 3449, 3451 ++ */ /* VL_Os_3449, VL_Os_3451 */
extern Os_ProtectionReturnType Os_CallProtectionHook(StatusType errType, uint32 whoHook);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
/* PRQA S 3449, 3451 --*/
#endif

#endif

/*=======[E N D   O F   F I L E]==============================================*/
