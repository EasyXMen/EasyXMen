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
**  FILENAME    :  Os_Extened.h                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      :  i-soft-os                                                  **
**  Vendor      :                                                             **
**  DESCRIPTION :  Extended functions                                         **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/

#ifndef OS_EXTEND_H
#define OS_EXTEND_H

/*=======[I N C L U D E S]====================================================*/
#include "Os_Types.h"
#include "Os_Internal.h"
#include "Arch_Extend.h"

/*=======[M A C R O S]========================================================*/
/* Types of stack usage. */
typedef uint32 osStackUsageType;

/* Enumeration type of stack object. */
typedef enum
{
    OS_STACK_SYSTEM = 0U,
    OS_STACK_TASK = 1U,
    OS_STACK_ISR2 = 2U
} osStackObject;

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[F U N C T I O N   D E C L A R A T I O N S]========*/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           Provide Version information to user.
 * @param[out]      osVerInfoPtr: pointer for getting OS version
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           -
 */
void OSGetVersionInfo(Std_VersionInfoType* osVerInfoPtr);

#if (CHECK_STACK_USAGE > 0)
/**
 * @brief           Get max usage of system,task,ISR2 stack.
 * @param[in]       stack: Stack type
 * @param[in]       id: Object ID
 * @return          osStackUsageType
 * @retval          uint32: Max usage of stack
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           -
 */
osStackUsageType OSGetStackUsage(osStackObject stack, uint16 id);
#endif

/**
 * @brief           OSCheckISRSource is used to check the interrupt source.
 * @param[in]       Source: Isr source
 * @return          StatusType
 * @retval          TRUE: current triggered interrupt.
 * @retval          FALSE: not current triggered interrupt.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           -
 */
StatusType OSCheckISRSource(uint32 Source);

/**
 * @brief           OSCheckCPUInformation is used to check if the CPU information is correct.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           -
 */
void OSCheckCPUInformation(void);

#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* OS_EXTEND_H */

/*=======[E N D   O F   F I L E]==============================================*/
