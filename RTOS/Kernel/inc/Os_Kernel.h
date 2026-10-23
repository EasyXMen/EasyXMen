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
#include "Os_Hal_Define.h"
#include "Os_Appl.h"
#include "Os_CfgData.h"

/* ===================================================== macros ===================================================== */
#if (CFG_ALARM_MAX > 0)
/**
 * @brief SCB and CoreId are obtained by alarmId.
 */
/* PRQA S 3472 ++ */ /* VL_Os_3472 */
#define OS_ALARM_GET_COREID(alarmId) (Os_AlarmCfg[alarmId].HostCore)
/* PRQA S 3472 -- */

#if (TRUE == ENABLE_CLONE_TECH)
#define OS_ALARM_GET_SCB(alarmId) (&Os_SCB_FA_Inf[0])
#else
#define OS_ALARM_GET_SCB(alarmId) (Os_SCB_FA_Inf[OS_ALARM_GET_COREID(alarmId)])
#endif

#endif

#if (CFG_COUNTER_MAX > 0U)
/**
 * @brief SCB and CoreId are obtained by counterId.
 */
/* PRQA S 3472 ++ */ /* VL_Os_3472 */
#define OS_COUNTER_GET_COREID(counterId) (Os_CounterCfg[counterId].HostCore)
/* PRQA S 3472 -- */
#if (TRUE == ENABLE_CLONE_TECH)
#define OS_COUNTER_GET_SCB(counterId) (&Os_SCB_FA_Inf[0])
#else
#define OS_COUNTER_GET_SCB(counterId) (Os_SCB_FA_Inf[OS_COUNTER_GET_COREID(counterId)])
#endif

#endif

#if (CFG_ISR_MAX > 0U)
/**
 * @brief SCB and CoreId are obtained by isrId.
 */
/* PRQA S 3472 ++ */ /* VL_Os_3472 */
#define OS_ISR_GET_COREID(isrId) (Os_IsrCfg[isrId].HostCore)
/* PRQA S 3472 -- */
#if (TRUE == ENABLE_CLONE_TECH)
#define OS_ISR_GET_SCB(isrId) (&Os_SCB_FA_Inf[0])
#else
#define OS_ISR_GET_SCB(isrId) (Os_SCB_FA_Inf[OS_ISR_GET_COREID(isrId)])
#endif

/**
 * @brief Check the validity of the isrId.
 *
 * Macro parameters:
 *  @param[in] isrId: The Id of the Isr to be checked.
 */
/* PRQA S 3472 ++ */ /* VL_Os_3472 */
#define Os_CheckIsr2Id(isrId)                                             \
    (((isrId) >= (Os_IsrIdRange[OS_ISR_GET_COREID(isrId)].Isr2.IsrStart)) \
     && ((isrId) < (Os_IsrIdRange[OS_ISR_GET_COREID(isrId)].Isr2.IsrEnd)))
/* PRQA S 3472 -- */

#endif

#if (CFG_STD_RESOURCE_MAX > 0U)
/**
 * @brief SCB and CoreId are obtained by resId.
 */
#define OS_RESOURCE_GET_COREID(resId) (Os_ResourceCfg[resId].HostCore) /* PRQA S 3472 */ /* VL_Os_3472 */
#if (TRUE == ENABLE_CLONE_TECH)
#define OS_RESOURCE_GET_SCB(resId) (&Os_SCB_FA_Inf[0])
#else
#define OS_RESOURCE_GET_SCB(resId) (Os_SCB_FA_Inf[OS_RESOURCE_GET_COREID(resId)])
#endif

#endif

#if (CFG_SCHEDTBL_MAX > 0U)
/**
 * @brief SCB and CoreId are obtained by scheduleTableId.
 */
/* PRQA S 3472 ++ */ /* VL_Os_3472 */
#define OS_SCHEDTBL_GET_COREID(scheduleTableId) (Os_SchedTblCfg[scheduleTableId].HostCore)
/* PRQA S 3472 -- */

#if (TRUE == ENABLE_CLONE_TECH)
#define OS_SCHEDTBL_GET_SCB(scheduleTableId) (&Os_SCB_FA_Inf[0])
#else
#define OS_SCHEDTBL_GET_SCB(scheduleTableId) (Os_SCB_FA_Inf[OS_SCHEDTBL_GET_COREID(scheduleTableId)])
#endif

/**
 * @brief Check if the scheduleTableId on a certain core is valid.
 *
 * Macro parameters:
 *  @param[in] scheduleTableId: The scheduleTableId that needs to be checked.
 *  @param[in] coreId: The core to which the checked scheduleTable belongs.
 */
/* PRQA S 3410 ++ */ /* VL_Os_3410 */
#define Os_CheckSchedTblId(scheduleTableId, coreId)                          \
    ((scheduleTableId >= Os_CfgScheduleTableIndex_Inf[coreId].SchedTblStart) \
     && (scheduleTableId < Os_CfgScheduleTableIndex_Inf[coreId].SchedTblEnd))
/* PRQA S 3410 -- */

#endif

#if (CFG_TASK_MAX > 0U)
/**
 * @brief SCB and CoreId are obtained by taskId.
 */
#define OS_TASK_GET_COREID(taskId) (Os_TaskCfg[taskId].HostCore) /* PRQA S 3472 */ /* VL_Os_3472 */
#if (TRUE == ENABLE_CLONE_TECH)
#define OS_TASK_GET_SCB(taskId) (&Os_SCB_FA_Inf[0])
#else
#define OS_TASK_GET_SCB(taskId) (Os_SCB_FA_Inf[OS_TASK_GET_COREID(taskId)])
#endif

/**
 * @brief Check if the external taskId on a certain core is valid.
 *
 * Macro parameters:
 *  @param[in] taskId: The external taskId that needs to be checked.
 *  @param[in] coreId: The core to which the checked external task belongs.
 */
/* PRQA S 3472 ++ */ /* VL_Os_3472 */
#define Os_CheckExternalTaskId(taskId, coreId) \
    (((taskId) >= (Os_TaskIdRange[coreId].ExtendTask.Start)) && ((taskId) < (Os_TaskIdRange[coreId].ExtendTask.End)))
/* PRQA S 3472 -- */

/**
 * @brief Check if the taskId on a certain core is valid.
 *
 * Macro parameters:
 *  @param[in] taskId: The taskId that needs to be checked.
 *  @param[in] coreId: The core to which the checked task belongs.
 */
/* PRQA S 3472 ++ */ /* VL_Os_3472 */
#define CheckCoreTaskId(taskId, coreId) \
    (((taskId) >= (Os_TaskIdRange[coreId].AllTask.Start)) && ((taskId) < (Os_TaskIdRange[coreId].AllTask.End)))
/* PRQA S 3472 -- */

#endif

/* ========================================== external function definitions ========================================= */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Internal implementation of OS service:ShutdownOS.
 * @param[in]          error: Error code.
 * @param[in]          action: The action of the ShutdownOS.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_ShutdownOS(StatusType error, Os_ShutdownAction action);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Clear the CoreCB info.
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern void Os_DeInitCoreInfo(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief         Os panic.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         -
 */
extern void Os_Panic(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
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
OS_LOCAL_INLINE OS_ALWAYS_INLINE Os_SCBType* Os_GetSystemContext(Os_CoreIdType coreId)
{
    Os_SCBType* pScb = NULL_PTR;

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
OS_LOCAL_INLINE OS_ALWAYS_INLINE Os_SCBType* Os_GetCurrentContext(void)
{
    Os_SCBType* pScb = NULL_PTR;
#if (TRUE == ENABLE_CLONE_TECH)
    pScb = &Os_SCB_FA_Inf[0];
#elif (OS_AUTOSAR_CORES == 1)
    pScb = Os_SCB_FA_Inf[0];
#else
    Os_CoreIdType coreId = Os_GetCoreIdLocal();
    pScb                 = Os_SCB_FA_Inf[coreId];
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
