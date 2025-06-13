/* PRQA S 3108++ */
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
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : Os_ECode.h                                                  **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/

#ifndef OS_ECODE_H
#define OS_ECODE_H
/*=======[M A C R O S]========================================================*/
/* API services error type of STD OSEK */
#define E_OS_CALLEVEL 2U
#define E_OS_ID       3U
#define E_OS_LIMIT    4U
#define E_OS_NOFUNC   5U
#define E_OS_RESOURCE 6U
#define E_OS_STATE    7U
#define E_OS_VALUE    8U

/* API services error type of AUTOSAR, see 7.10 */
#define E_OS_SERVICEID             9U
#define E_OS_RATE                  10U
#define E_OS_ILLEGAL_ADDRESS       11U
#define E_OS_MISSINGEND            12U
#define E_OS_DISABLEDINT           13U
#define E_OS_STACKFAULT            14U
#define E_OS_PROTECTION_MEMORY     15U
#define E_OS_PROTECTION_TIME       16U
#define E_OS_PROTECTION_LOCKED     17U
#define E_OS_PROTECTION_EXCEPTION  18U
#define E_OS_PROTECTION_RATE       19U
#define E_OS_PROTECTION_ARRIVAL    20U
#define E_OS_CORE                  21U
#define E_OS_INTERFERENCE_DEADLOCK 22U
#define E_OS_NESTING_DEADLOCK      23U
#define E_OS_SPINLOCK              24U
#define E_OS_PARAM_POINTER         25U
#define E_OS_ACCESS                49U

/* API services error type of IOC */
#define IOC_E_OK        0U
#define IOC_E_NOK       1U
#define IOC_E_LOST_DATA 64U
#define IOC_E_LIMIT     130U
#define IOC_E_NO_DATA   131U
#define IOC_E_LENGTH    132U
#define E_OS_TIMEOUT    133U

#endif /* OS_ECODE_H */
/*=======[E N D   O F   F I L E]==============================================*/
