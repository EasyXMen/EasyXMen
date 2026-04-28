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
 **  @file               : Os_Kernel.h
 **  @author             : i-soft-os
 **  @date               : 2024/02/10
 **  @vendor             : isoft
 **  @description        : Os header file for kernel api declarations
 **
 ***********************************************************************************************************************/

#ifndef OS_KERNEL_H
#define OS_KERNEL_H

/* =================================================== inclusions =================================================== */
#include "Os_CfgData.h"
#include "Os_Arch_Processor.h"

/* ========================================== external function definitions ========================================= */
/**
 * @brief              Internal implementation of OS service:ShutdownOS.
 * @param[in]          error: Error code.
 * @param[in]          action: The action of the ShutdownOS.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_ShutdownOS(StatusType error, Os_ShutdownAction action);

/**
 * @brief              StartOS synchronizes all cores twice. The first
 *                     synchronization point is located before the StartupHooks
 *                     are executed, the second after the OS-Application specific
 *                     StartupHooks have finished and before the scheduler is started.
 * @param[in]          point: Which synchronization point.
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern void Os_SynPoint(uint8 point);

/**
 * @brief              Clear the CoreCB info.
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern void Os_DeInitCoreInfo(void);

/**
 * @brief         Os panic.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         -
 */
extern void Os_Panic(void);
/* ========================================== internal function definitions ========================================= */
/**
 * @brief           Gets the ID of the local core
 * @return          Os_CoreIdType
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE Os_CoreIdType Os_GetCoreIdLocal(void)
{
#if (FALSE == ENABLE_CLONE_TECH)
    return Os_Hal_GetCoreID();
#else
    return Os_SCB_FA_Inf[0].SysCore;
#endif
}

/**
 * @brief           Gets the system control block for a specific core
 * @param[in]       coreId: Core identifier
 * @return          Os_SCBType*
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE Os_SCBType *Os_GetSystemContext(Os_CoreIdType coreId)
{
    Os_SCBType *pScb = NULL_PTR;

#if (TRUE == ENABLE_CLONE_TECH)
    pScb = &Os_SCB_FA_Inf[0];
#else
    pScb = Os_SCB_FA_Inf[coreId];
#endif

    (void)coreId;
    return pScb;
}

/**
 * @brief           Gets the system control block of the current core
 * @return          Os_SCBType*
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE Os_SCBType *Os_GetCurrentContext(void)
{
    Os_SCBType *pScb = NULL_PTR;
#if (TRUE == ENABLE_CLONE_TECH)
    pScb = &Os_SCB_FA_Inf[0];
#elif (OS_AUTOSAR_CORES == 1)
    pScb = Os_SCB_FA_Inf[0];
#else
    Os_CoreIdType coreId = Os_GetCoreIdLocal();
    pScb = Os_SCB_FA_Inf[coreId];
#endif

    return pScb;
}

/**
 * @brief           Validates if an object ID is within valid range for its type
 * @param[in]       objId: Object identifier
 * @param[in]       type: Object type
 * @return          boolean
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE boolean Os_ObjectIDCheck(ObjectType objId, uint8 type)
{
    boolean status = FALSE;
    if (objId < Os_ObjectIDMaxTable[type])
    {
        status = TRUE;
    }

    return status;
}

#endif /* OS_KERNEL_H_*/

/*=======[E N D   O F   F I L E]==============================================*/
