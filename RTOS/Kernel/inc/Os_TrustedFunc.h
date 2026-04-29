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
 **  @file               : Os_TrustedFunc.h
 **  @author             : i-soft-os
 **  @date               : 2024/02/10
 **  @vendor             : isoft
 **  @description        : Os header file for TrustedFunc api declarations
 **
 ***********************************************************************************************************************/

#ifndef OS_TRUSTED_FUNCTION_H
#define OS_TRUSTED_FUNCTION_H

#define OS_TRUSTED_FUNCTION_ISR2_BIT (0x1000U)
/* =================================================== inclusions =================================================== */
#include "Os_TrustedFunc_CfgData.h"

/* ========================================= external function declarations ========================================= */

#if (CFG_TRUSTED_SYSTEM_SERVICE_MAX > 0U)
/**
 * @brief              Init the TrustedFunction.
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern void Os_InitTrustedFunction(void);

/**
 * @brief              To check if the current error is caused by TrustedFunc and if the error is fatal.
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern void Os_TrustedFuncCallerCheck(StatusType errType, uint32 whoHook, Os_SCBType *pScb, Os_ProtectionReturnType *Ret);
#endif

#endif

/*=======[E N D   O F   F I L E]==============================================*/
