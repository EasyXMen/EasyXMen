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
 **  @file               : Os_ECode.h
 **  @author             : i-soft-os
 **  @date               : 2024/02/10
 **  @vendor             : isoft
 **  @description        : Os header file for error code definitions
 **
 ***********************************************************************************************************************/

#ifndef OS_ECODE_H
#define OS_ECODE_H
/* ===================================================== macros ===================================================== */
/**
 * @brief API services error type of STD OSEK.
 */
/* E_NOT_OK shall only be used for internal judgment within the OS, cannot be return to the user. */
#define E_OS_ACCESS 1U
#define E_OS_CALLEVEL 2U
#define E_OS_ID 3U
#define E_OS_LIMIT 4U
#define E_OS_NOFUNC 5U
#define E_OS_RESOURCE 6U
#define E_OS_STATE 7U
#define E_OS_VALUE 8U

/**
 * @brief API services error type of AUTOSAR, see 7.10.
 */
#define E_OS_SERVICEID 9U
#define E_OS_ILLEGAL_ADDRESS 11U
#define E_OS_MISSINGEND 12U
#define E_OS_DISABLEDINT 13U
#define E_OS_STACKFAULT 14U
#define E_OS_PROTECTION_MEMORY 15U
#define E_OS_PROTECTION_TIME 16U
#define E_OS_PROTECTION_LOCKED 17U
#define E_OS_PROTECTION_EXCEPTION 18U
#define E_OS_PROTECTION_ARRIVAL 20U
#define E_OS_CORE 21U
#define E_OS_INTERFERENCE_DEADLOCK 22U
#define E_OS_NESTING_DEADLOCK 23U
#define E_OS_SPINLOCK 24U

/**
 * @brief API services error type of IOC.
 */
#define IOC_E_OK 0U
#define IOC_E_LOST_DATA 64U
#define IOC_E_LIMIT 130U
#define IOC_E_NO_DATA 131U
#define IOC_E_LENGTH 132U

/**
 * @brief RPC added.
 */
#define E_OS_TIMEOUT 133U

/**
 * @brief Barrier added.
 */
#define E_OS_DEADLOCK  134U
#define E_OS_SYS_NO_BARRIER_PARTICIPANT  135U

#endif

/*=======[E N D   O F   F I L E]============================================*/
