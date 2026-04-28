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
 **  @file               : Os_PORT.h
 **  @author             : i-soft-os
 **  @date               : 2025/11/14
 **  @vendor             : isoft
 **  @description        : Os header file for port api declarations
 **
 ***********************************************************************************************************************/

#ifndef OS_PORT_H
#define OS_PORT_H

/* =================================================== inclusions =================================================== */
#include "Os_CfgData.h"

/* ========================================== external function definitions ========================================= */


/* ========================================== internal function definitions ========================================= */
#if (FALSE == ENABLE_CLONE_TECH)
/**
 * @brief           Gets the system level for a specific core
 * @param[in]       coreId: Core identifier
 * @return          Os_CallLevelType
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
static inline OS_ALWAYS_INLINE Os_CallLevelType Os_Kernel_GetSysLevel(Os_CoreIdType coreId)
{
    return Os_SCB_FA_Inf[coreId]->SysOsLevel;
}

#if (CFG_OSAPPLICATION_MAX > 0U)
/**
 * @brief           Gets the running application ID for a specific core
 * @param[in]       coreId: Core identifier
 * @return          Os_ApplicationType
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
static inline OS_ALWAYS_INLINE Os_ApplicationType Os_Kernel_GetSysRunningAppID(Os_CoreIdType coreId)
{
    return Os_SCB_FA_Inf[coreId]->SysRunningAppId;
}
#endif

/**
 * @brief           Gets the running task ID for a specific core
 * @param[in]       coreId: Core identifier
 * @return          Os_TaskType
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
static inline OS_ALWAYS_INLINE Os_TaskType Os_Kernel_GetSysRunningTaskID(Os_CoreIdType coreId)
{
    return Os_SCB_FA_Inf[coreId]->SysRunningTaskId;
}

/**
 * @brief           Gets the running task state for a specific core
 * @param[in]       coreId: Core identifier
 * @return          Os_TaskStateType
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
static inline OS_ALWAYS_INLINE Os_TaskStateType Os_Kernel_GetSysRunningTaskState(Os_CoreIdType coreId)
{
    return Os_SCB_FA_Inf[coreId]->SysRunningTCB->TaskState;
}

/**
 * @brief           Gets the ISR Category 2 nesting count for a specific core
 * @param[in]       coreId: Core identifier
 * @return          uint8
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
static inline OS_ALWAYS_INLINE uint8 Os_Kernel_GetIntNestNumISR2(Os_CoreIdType coreId)
{
    return Os_SCB_FA_Inf[coreId]->IntNestISR2;
}

/**
 * @brief           Gets the ISR Category 1 nesting count for a specific core
 * @param[in]       coreId: Core identifier
 * @return          uint8
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
static inline OS_ALWAYS_INLINE uint8 Os_Kernel_GetIntNestNumISR1(Os_CoreIdType coreId)
{
    return Os_SCB_FA_Inf[coreId]->IntNestISR1;
}

/**
 * @brief           Compare the target object with the object that ProtectionHook needs to end; if they are equal, return and clear.
 * @return          Os_ProtectionTerminateObjType
 * @param[in]       desObj: Desired Protection Terminate Object
 * @param[in]       coreId: Core identifier
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
static inline OS_ALWAYS_INLINE Os_ProtectionTerminateObjType Os_Kernel_GetAndComparTerminateObject(Os_ProtectionTerminateObjType desObj, Os_CoreIdType coreId)
{
    Os_ProtectionTerminateObjType ret = Os_SCB_FA_Inf[coreId]->ProtectionTerminateObj;

    if (desObj == ret)
    {
        Os_SCB_FA_Inf[coreId]->ProtectionTerminateObj = OS_PROTECTION_TERMINATE_INVALID;
    }
    return ret;
}
#else /* TRUE == ENABLE_CLONE_TECH */
/**
 * @brief           Gets the system level
 * @return          Os_CallLevelType
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
static inline OS_ALWAYS_INLINE Os_CallLevelType Os_Kernel_GetSysLevel(void)
{
    return Os_SCB_FA_Inf[0].SysOsLevel;
}

#if (CFG_OSAPPLICATION_MAX > 0U)
/**
 * @brief           Gets the running application ID
 * @return          Os_ApplicationType
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
static inline OS_ALWAYS_INLINE Os_ApplicationType Os_Kernel_GetSysRunningAppID(void)
{
    return Os_SCB_FA_Inf[0].SysRunningAppId;
}
#endif

/**
 * @brief           Gets the running task ID
 * @return          Os_TaskType
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
static inline OS_ALWAYS_INLINE Os_TaskType Os_Kernel_GetSysRunningTaskID(void)
{
    return Os_SCB_FA_Inf[0].SysRunningTaskId;
}

/**
 * @brief           Gets the running task state
 * @return          Os_TaskStateType
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
static inline OS_ALWAYS_INLINE Os_TaskStateType Os_Kernel_GetSysRunningTaskState(void)
{
    return Os_SCB_FA_Inf[0].SysRunningTCB->TaskState;
}

/**
 * @brief           Gets the ISR Category 2 nesting count
 * @return          uint8
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
static inline OS_ALWAYS_INLINE uint8 Os_Kernel_GetIntNestNumISR2(void)
{
    return Os_SCB_FA_Inf[0].IntNestISR2;
}

/**
 * @brief           Gets the ISR Category 1 nesting count
 * @return          uint8
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
static inline OS_ALWAYS_INLINE uint8 Os_Kernel_GetIntNestNumISR1(void)
{
    return Os_SCB_FA_Inf[0].IntNestISR1;
}

/**
 * @brief           Compare the target object with the object that ProtectionHook needs to end; if they are equal, return and clear.
 * @return          Os_ProtectionTerminateObjType
 * @param[in]       desObj: Desired Protection Terminate Object
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
static inline OS_ALWAYS_INLINE Os_ProtectionTerminateObjType Os_Kernel_GetAndComparTerminateObject(Os_ProtectionTerminateObjType desObj)
{
    Os_ProtectionTerminateObjType ret = Os_SCB_FA_Inf[0].ProtectionTerminateObj;

    if (desObj == ret)
    {
        Os_SCB_FA_Inf[0].ProtectionTerminateObj = OS_PROTECTION_TERMINATE_INVALID;
    }
    return ret;
}

#endif

#endif /* OS_PORT_H*/

/*=======[E N D   O F   F I L E]==============================================*/
