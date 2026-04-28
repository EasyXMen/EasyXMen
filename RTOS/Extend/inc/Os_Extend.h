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
 **  @file               : Os_Extened.h
 **  @author             : i-soft-os
 **  @date               : 2024/02/10
 **  @vendor             : isoft
 **  @description        : Os header file for extended function
 **
 ***********************************************************************************************************************/
#ifndef OS_EXTEND_H
#define OS_EXTEND_H

/* =================================================== inclusions =================================================== */
#include "Std_Types.h"

/* ===================================================== macros ===================================================== */
/**
 * Define the vendor ID and module ID.
 */
#define OS_VENDOR_ID (62U)
#define OS_MODULE_ID (1U)

/* Types of stack usage. */
typedef uint32 Os_StackUsageType;

/* Enumeration type of stack object. */
typedef enum
{
    OS_STACK_SYSTEM = 0,
    OS_STACK_TASK = 1,
    OS_STACK_ISR2 = 2
} Os_StackObject;

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[F U N C T I O N   D E C L A R A T I O N S]========*/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief          Provide Version information to user.
 * @param[in]      Versioninfo: Version information.
 * @reentrant      TRUE
 * @synchronous    FALSE
 * @trace          -
 */
void OSGetVersionInfo(Std_VersionInfoType *osVerInfoPtr);

#if (CFG_STACK_USAGE_CHECK == TRUE)
/**
 * @brief          Get max usage of system,task,ISR2 stack.
 * @param[in]      stack: The type of stack.
 * @param[in]      id: The id of stack.
 * @return         Os_StackUsageType
 * @retval         Return the used size of the stack.
 * @reentrant      TRUE
 * @synchronous    FALSE
 * @trace          -
 */
Os_StackUsageType OSGetStackUsage(Os_StackObject stack, uint16 id);
#endif

/**
 * @brief          Check ISR source.
 * @param[in]      source: The Interrupt source.
 * @return         StatusType
 * @retval         Check if the input interrupt source is the current interrupt;
 *                 if it is, return TRUE, otherwise return FALSE.
 * @reentrant      TRUE
 * @synchronous    FALSE
 * @trace          -
 */
extern StatusType OSCheckISRSource(uint32 source);

/**
 * @brief          Check whether CPU information is correct.
 * @reentrant      TRUE
 * @synchronous    FALSE
 * @trace          -
 */
void OSCheckCPUInformation(void);

/**
 * @brief          Shell processing commands.
 * @reentrant      FALSE
 * @synchronous    FALSE
 * @trace          -
 */
extern void OSShellhandler(void); /* PRQA S 1753, 1501 */ /* VL_Os_1753, VL_Os_1501 */

#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

/*=======[E N D   O F   F I L E]==============================================*/
