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
 **  @file               : Os_Spinlock.h
 **  @author             : i-soft-os
 **  @date               : 2024/02/10
 **  @vendor             : isoft
 **  @description        : Os header file for Spinlock api declarations
 **
 ***********************************************************************************************************************/

#ifndef OS_SPINLOCK_H
#define OS_SPINLOCK_H

/* =================================================== inclusions =================================================== */
#include "Os_Spinlock_CfgData.h"

/* ===================================================== macros ===================================================== */
/**
 * @brief Spinlock state.
 */
#define OS_SPINLOCK_UNLOCK 0x0u
#define OS_SPINLOCK_LOCK 0x1u

/**
 * @brief Spinlock occupy state.
 */
#define OS_SPINLOCK_OCCUPY_SUCCESS 0x0u
#define OS_SPINLOCK_OCCUPY_NO_SUCCESS 0x1u

/* =========================================== external data declarations =========================================== */
#if (OS_AUTOSAR_CORES > 1U)
/**
 * @brief Spinlock used by the kernel.
 */
extern Os_SpinlockType Os_SpinlockSync;
#endif

/* ========================================= external function declarations ========================================= */
#if (CFG_SPINLOCK_MAX > 0U)
/**
 * @brief              Initialize the spin lock
 * @param[in]          coreId: Core Id.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_InitSpinlock(Os_CoreIdType coreId);

/**
 * @brief              Check if there are any outstanding spinlocks
 *                     when scheduling is required
 * @param[in]          runningTaskId: The TaskId that is currently running.
 * @return             StatusType
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern StatusType Os_SpinlockSafetyCheck(Os_TaskType runningTaskId);

/**
 * @brief              Internal implementation of OS service:ReleaseSpinlock
 * @param[in]          pScb: Points to the system control block.
 * @param[in]          spinlockId: Spinlock Id.
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern void Os_ReleaseSpinlock(Os_SCBType *pScb, SpinlockIdType spinlockId);
#endif

#if ((OS_AUTOSAR_CORES > 1U) || (CFG_SPINLOCK_MAX > 0U))

/**
 * @brief              The get spin lock function inside the OS.
 * @param[in]          spinlockId: Spinlock Id.
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern void Os_GetInternalSpinlock(Os_SpinlockRefType spinlock);

/**
 * @brief              The release spin lock function inside the OS.
 * @param[in]          spinlockId: Spinlock Id.
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern void Os_ReleaseInternalSpinlock(Os_SpinlockRefType spinlock);
#endif

#endif

/*=======[E N D   O F   F I L E]==============================================*/
