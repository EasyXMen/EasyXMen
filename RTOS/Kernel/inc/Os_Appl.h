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
 **  @file               : Os_Appl.h
 **  @author             : i-soft-os
 **  @date               : 2024/02/10
 **  @vendor             : isoft
 **  @description        : Os header file for application api declarations
 **
 ***********************************************************************************************************************/

#ifndef OS_APPL_H
#define OS_APPL_H

/* =================================================== inclusions =================================================== */
#include "Os_Appl_CfgData.h"
#include "Os_Alarm.h"
#include "Os_Task.h"
#include "Os_Counter.h"
#include "Os_Interrupt.h"
#include "Os_ScheduleTable.h"

/* ===================================================== macros ===================================================== */
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
/**
 * @brief The size of the minimum access bit unit for one application.
 */
/* DD_1_0176 */
#define OS_APPACCBITUNIT_SIZE (sizeof(ApplicationType) * CHAR_BIT)

/**
 * @brief The access application bit position based on the access bit unit size.
 */
#define OS_APPGETACCESS_BP(appId) ((appId) % OS_APPACCBITUNIT_SIZE)
/**
 * @brief The access application bit group.
 */
/* PRQA S 3472 ++ */ /* VL_Os_3472 */
#define OS_APPGETACCESS_GP(appId) ((appId) >> 4u)
/**
 * @brief The access mask of the application.
 */
#define OS_APPACCESS_MASK(appId) ((uint16)1u << OS_APPGETACCESS_BP(appId))
/* PRQA S 3472 -- */
#endif
/**
 * @brief Check if the appId on a certain core is valid.
 *
 * Macro parameters:
 *  @param[in] appId: The appId that needs to be checked.
 *  @param[in] coreId: The core to which the checked APP belongs.
 */
/* PRQA S 3472 ++ */ /* VL_Os_3472 */
#define Os_CheckCoreAppId(appId, coreId) (     \
    ((appId) >= (uint16)(Os_AppIdRange[coreId].start)) && \
    ((appId) < (uint16)(Os_AppIdRange[coreId].end)))
/* PRQA S 3472 -- */

/* ========================================== external function definitions ========================================= */
#if (CFG_OSAPPLICATION_MAX > 0U)
/**
 * @brief         Init the Application control block
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         -
 */
extern void Os_InitApplication(void);

/**
 * @brief              Internal implementation of OS service:TerminateApplication
 * @param[in]          pScb: Points to the system control block.
 * @param[in]          application: The appId to be terminated.
 * @param[in]          restartOption: The action to restart the APP.
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
/* PRQA S 1753, 1501 ++ */ /* VL_Os_1753, VL_Os_1501 */
extern void Os_TerminateApplication(
    Os_SCBType *pScb,
    ApplicationType application,
    RestartType restartOption);
/* PRQA S 1753, 1501 -- */
/**
 * @brief              Internal implementation of OS service:CheckObjectAccess
 * @param[in]          pScb: Points to the system control block.
 * @param[in]          applId: The access permissions of the objectId for this app.
 * @param[in]          objectType: The type of the object.
 * @param[in]          objectId: The ID of the Object for which permission needs to be checked.
 * @return             Os_ObjectAccessType
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
/* PRQA S 1753, 1501 ++ */ /* VL_Os_1753, VL_Os_1501 */
extern Os_ObjectAccessType Os_CheckObjectAccess(
    const Os_SCBType *pScb,
    ApplicationType applId,
    ObjectTypeType objectType,
    AppObjectId objectId);
/* PRQA S 1753, 1501 -- */

/* ========================================== internal function definitions ========================================= */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Retrieve the appId to which it belongs through the objectId and type.
 * @param[in]          objectType: The type of the object.
 * @param[in]          objectId: ObjectId.
 * @return             ApplicationType
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE ApplicationType Os_GetObjectAppID(ObjectTypeType objectType, AppObjectId objectId)
{
    ApplicationType appId = 0U;

    switch (objectType)
    {
#if (CFG_TASK_MAX > 0U)
    case OS_OBJECT_TASK:
        appId = Os_TaskCfg[objectId].ObjAppCfg->HostApp;
        break;
#endif
#if (CFG_ISR_MAX > 0U)
    case OS_OBJECT_ISR:
        appId = Os_IsrCfg[objectId].ObjAppCfg->HostApp;
        break;
#endif
#if (CFG_ALARM_MAX > 0U)
    case OS_OBJECT_ALARM:
        appId = Os_AlarmCfg[objectId].ObjAppCfg->HostApp;
        break;
#endif
#if (CFG_COUNTER_MAX > 0U)
    case OS_OBJECT_COUNTER:
        appId = Os_CounterCfg[objectId].ObjAppCfg->HostApp;
        break;
#endif
#if (CFG_SCHEDTBL_MAX > 0U)
    case OS_OBJECT_SCHEDULETABLE:
        appId = Os_SchedTblCfg[objectId].ObjAppCfg->HostApp;
        break;
#endif
    default: /* Nothing to do. */
        break;
    }
    return appId;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              This service returns the current state of an OS-Application.
 * @param[in]          application: Application ID.
 * @return             ApplicationStateType
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE ApplicationStateType Os_GetAppStateInternal(ApplicationType application)
{
    return Os_AppCB[application].AppState;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#endif

/*=======[E N D   O F   F I L E]============================================*/
