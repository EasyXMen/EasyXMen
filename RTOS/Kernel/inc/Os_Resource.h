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
 **  @file               : Os_Resource.h
 **  @author             : i-soft-os
 **  @date               : 2024/02/10
 **  @vendor             : isoft
 **  @description        : Os header file for resource api declarations
 **
 ***********************************************************************************************************************/

#ifndef OS_RESOURCE_H
#define OS_RESOURCE_H

/* =================================================== inclusions =================================================== */
#include "Os_Resource_CfgData.h"

/* ===================================================== macros ===================================================== */

/* ========================================== external function definitions ========================================= */
#if (CFG_STD_RESOURCE_MAX > 0U)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Init the resource control block.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_InitResource(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Internal implementation of OS service:GetResource
 * @param[in]          pScb: Points to the system control block.
 * @param[in]          resId: Resource Id.
 * @return             StatusType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern StatusType Os_GetResource(Os_SCBType* pScb, ResourceType resId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              task or ISR release a resource occupied before.
 *                     ReleaseResource is the counterpart of  GetResource and
 *                     serves to leave critical sections in the code that are
 *                     assigned to the resource referenced by <resId>.
 * @param[in]          pScb: Points to the system control block.
 * @param[in]          resId: Resource Id.
 * @return             StatusType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern StatusType Os_ReleaseResource(Os_SCBType* pScb, ResourceType resId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /*CFG_RESOURCE_MAX > 0U*/

#if (CFG_INTERNAL_RESOURCE_MAX > 0U)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Get the internal resource.
 * @param[in]          pScb: Points to the system control block.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_GetInternalResource(Os_SCBType* pScb);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Release the internal resource.
 * @param[in]          pScb: Points to the system control block.
 * @param[in]          taskId: Task Id.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_ReleaseInternalResource(const Os_SCBType* pScb, Os_TaskType taskId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif

#endif

/*=======[E N D   O F   F I L E]==============================================*/
