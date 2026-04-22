/**
 * Copyright (C) 2024 Isoft Infrastructure Software Co., Ltd.
 * SPDX-License-Identifier: LGPL-2.1-only-with-exception OR  LicenseRef-Commercial-License
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the
 * GNU Lesser General Public License as published by the Free Software Foundation; version 2.1.
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License along with this library;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * or see <https://www.gnu.org/licenses/>.
 *
 * Alternatively, this file may be used under the terms of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License, in which case the provisions of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License shall apply instead of those of the GNU Lesser General Public License.
 *
 * You should have received a copy of the Isoft Infrastructure Software Co., Ltd.  Commercial License
 * along with this program. If not, please find it at <https://EasyXMen.com/xy/reference/permissions.html>
 *
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
/**
 * @brief SCB and CoreId are obtained by resId.
 */
#define OS_RESOURCE_GET_COREID(resId) (Os_ResourceCfg[resId].HostCore) /* PRQA S 3472 */ /* VL_Os_3472 */
#if (TRUE == ENABLE_CLONE_TECH)
#define OS_RESOURCE_GET_SCB(resId) (&Os_SCB_FA_Inf[0])
#else
#define OS_RESOURCE_GET_SCB(resId) (Os_SCB_FA_Inf[OS_RESOURCE_GET_COREID(resId)])
#endif

/**
 * @brief Check if the resId on a certain core is valid.
 *
 * Macro parameters:
 *  @param[in] resId: The resId that needs to be checked.
 *  @param[in] coreId: The core to which the checked Resource belongs.
 */
/* PRQA S 3410 ++ */ /* VL_Os_3410 */
#define Os_CheckResourceId(resId, coreId) (               \
    (resId >= Os_StdResIdRange[coreId].StdResStart) && \
    (resId < Os_StdResIdRange[coreId].StdResEnd))
/* PRQA S 3410 -- */

/* ========================================== external function definitions ========================================= */
#if (CFG_STD_RESOURCE_MAX > 0U)
/**
 * @brief              Init the resource control block.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_InitResource(void);

/**
 * @brief              Internal implementation of OS service:GetResource
 * @param[in]          pScb: Points to the system control block.
 * @param[in]          resId: Resource Id.
 * @return             StatusType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern StatusType Os_GetResource(Os_SCBType *pScb, ResourceType resId);

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
extern StatusType Os_ReleaseResource(Os_SCBType *pScb, ResourceType resId);
#endif /*CFG_RESOURCE_MAX > 0U*/

#if (CFG_INTERNAL_RESOURCE_MAX > 0U)
/**
 * @brief              Get the internal resource.
 * @param[in]          pScb: Points to the system control block.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_GetInternalResource(Os_SCBType *pScb);

/**
 * @brief              Release the internal resource.
 * @param[in]          pScb: Points to the system control block.
 * @param[in]          taskId: Task Id.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_ReleaseInternalResource(const Os_SCBType *pScb, Os_TaskType taskId);

#endif

#endif

/*=======[E N D   O F   F I L E]==============================================*/
