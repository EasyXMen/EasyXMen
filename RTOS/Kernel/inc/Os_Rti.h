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
 **  @file               : Os_Rti.h
 **  @author             : i-soft-os
 **  @date               : 2024/02/10
 **  @vendor             : isoft
 **  @description        : Os header file for Rti api declarations
 **
 ***********************************************************************************************************************/

#ifndef OS_RTI_H
#define OS_RTI_H

/* =================================================== inclusions =================================================== */
#include "Os_Types.h"

/* ===================================================== macros ===================================================== */
#define OS_RESOURCE_LOCKER_ISR2_BIT      (0x1000U)
#define OS_RESOURCE_LOCKER_INVALID_ID    (0xFFFFU)
#define OS_API_ENTER_BIT                 (1U)

#if (TRUE == CFG_RTI_ENABLE)
/* PRQA S 3412, 3410 ++ */ /* VL_Os_3412, VL_Os_3410 */
#define OSRtiSaveTaskLockerId(resId, taskId)        \
        Os_RCB[resId]->ResLockerId = (uint32)(taskId);

#define OSRtiSaveIsr2LockerId(resId, IsrId)         \
        Os_RCB[resId]->ResLockerId = (uint32)(IsrId & OS_RESOURCE_LOCKER_ISR2_BIT);

#define OSRtiClearLockerId(resId)                   \
        Os_RCB[resId]->ResLockerId = OS_RESOURCE_LOCKER_INVALID_ID;

#define OSRtiEnterApi(pScb, ApiId)                  \
        pScb->CurrentApiId = (ApiId) + OS_API_ENTER_BIT;

#define OSRtiExitApi(pScb, ApiId)                   \
        pScb->CurrentApiId = (ApiId);
/* PRQA S 3412, 3410 -- */
#else
#define OSRtiSaveTaskLockerId(resId, taskId)
#define OSRtiSaveIsr2LockerId(resId, IsrId)
#define OSRtiClearLockerId(resId)
#define OSRtiEnterApi(pScb, ApiId)
#define OSRtiExitApi(pScb, ApiId)
#endif

#if ((TRUE == CFG_RTI_ENABLE)||(TRUE == CFG_USEGETSERVICEID))
/* PRQA S 3412 ++ */ /* VL_Os_3412 */
#define OSRtiSaveErrorStatus(err)                   \
        pScb->SysOsError = (err);
/* PRQA S 3412 -- */
#else
#define OSRtiSaveErrorStatus(err)
#endif

#endif /* OS_RTI_H */
/*=======[E N D   O F   F I L E]============================================*/
