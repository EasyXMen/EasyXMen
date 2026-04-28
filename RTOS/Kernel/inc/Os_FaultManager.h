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
 **  @file               : Os_FaultManager.h
 **  @author             : tong.yin
 **  @date               : 2024/02/17
 **  @vendor             : isoft
 **  @description        : Os header file for FaultManager api declarations
 **
 ***********************************************************************************************************************/

#ifndef OS_FAULTMANAGER_H
#define OS_FAULTMANAGER_H

/* =================================================== inclusions =================================================== */
#include "Os_Debug_CfgData.h"

/* ========================================== external function definitions ========================================= */
#if (TRUE == CFG_FAULT_MANAGEMENT_ENABLE)
/**
 * @brief              Init the Fault location module.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_InitFaultManagement(void);

/**
 * @brief              Save hardware exceptions and system information.
 * @param[in]          errType: Trap error type.
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern void Os_FaultSaveInformation(StatusType errType);

/**
 * @brief              Saving system information.
 * @param[in]          pScb: Points to the system control block.
 * @param[in]          err: Error code.
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern void Os_FalultRecordSysRuntimeInform(const Os_SCBType *pScb, StatusType err);

/**
 * @brief              Getter function for the exception context.
 * @param[out]         context: Current exception context.
 * @return             StatusType
 * @retval             E_OK : No error
 * @retval             E_OS_ILLEGAL_ADDRESS: The parameter pointer is NULL
 * @retval             E_OS_CALLEVEL: Call level at wrong context
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern StatusType Os_GetExceptionContext(Os_ExceptionContextRefType context);

/**
 * @brief              Setter function for the exception context.
 * @param[in]          context: Context to set.
 * @return             StatusType
 * @retval             E_OK : No error
 * @retval             E_OS_ILLEGAL_ADDRESS: The parameter pointer is NULL
 * @retval             E_OS_CALLEVEL: Call level at wrong context
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern StatusType Os_SetExceptionContext(const Os_ExceptionContextType * context);

#endif

/**
 * @brief              Exception handler.
 * @param[in]          errMsg: Exception message.
 * @param[in]          errType: Exception type.
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern void Os_ExceptionHandler(const void *errMsg, StatusType errType);

#endif
