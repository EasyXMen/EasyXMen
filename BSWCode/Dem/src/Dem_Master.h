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
 **  @file               : Dem_Master.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : Implementation file of Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_MASTER_H
#define DEM_MASTER_H

/* =================================================== inclusions =================================================== */
#include "Dem_MasterSatelliteCom.h"
#include "Dem_ClearDTC.h"
#include "Dem_OperationCycle.h"
#if (DEM_FEATURE_NVM_IMMEDIATE == STD_ON)
#include "Dem_MemInterface.h"
#endif
#include "Dem_Queue.h"
#include "Dem_Indicator.h"
#if (DEM_TRIGGER_FIM_REPORTS == STD_ON)
#include "FiM_Dem.h"
#endif
#include "Dem_Error.h"
/* ================================================ type definitions ================================================ */
/**
 * @brief         Function pointer type for Task Function
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
typedef void (*Dem_TaskFunctionPtr)(void); /* PRQA S 5004 */ /* VL_Dem_5004 */

/* ========================================== internal function declarations ======================================== */
/**
 * @brief         Scheduler Task process
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_TaskScheduler(void);

/**
 * @brief         Master partition pre-initialization
 * @param[in]     ConfigPtr: Pointer to the configuration set in VARIANT-POST-BUILD.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_MasterPreInit(const Dem_ConfigType* ConfigPtr);

/**
 * @brief         Master partition initialization
 * @param[in]     ConfigPtr: Pointer to the configuration set in VARIANT-POST-BUILD.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_MasterInit(const Dem_ConfigType* ConfigPtr);

/* ============================================ internal data definitions =========================================== */
#define DEM_START_SEC_VAR_CLEARED_32
#include "Dem_MemMap.h"
/**
 * @brief     Type definition for multiple scheduler task
 * @range 0..4294967295
 */
DEM_LOCAL uint32 Dem_MultipleSchedulerTask;

/**
 * @brief     Type definition for single scheduler task
 * @range 0..4294967295
 */
DEM_LOCAL uint32 Dem_SingleSchedulerTask;
#define DEM_STOP_SEC_VAR_CLEARED_32
#include "Dem_MemMap.h"

/* ========================================== internal function definitions ========================================= */
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Scheduler Task initialization
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SchedulerTaskInit(void)
{
    Dem_MultipleSchedulerTask = 0u;
    Dem_SingleSchedulerTask   = 0u;
}

/**
 * @brief         Scheduler Task disable
 * @param[in]     Task: Pointer to the task
 * @param[in]     TaskId: Task Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SyncReSetTask(uint32* Task, Dem_TaskIdType TaskId)
{
    uint32 oldTask;
    uint32 newTask;
    uint32 taskMask = (uint32)(~((uint32)1u << (uint8)TaskId)); /* PRQA S 1258 */ /* VL_Dem_1258 */
    do
    {
        oldTask = *Task;
        newTask = oldTask & taskMask;
    } while (Dem_ComSyncCompareAndSwap(Task, oldTask, newTask) == FALSE);
}

/**
 * @brief         Scheduler Task disable
 * @param[in]     TaskId: Task Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_DisableTask(Dem_TaskIdType TaskId)
{
    Dem_SyncReSetTask(&Dem_MultipleSchedulerTask, TaskId);
}

/**
 * @brief         Scheduler Task enable
 * @param[in]     Task: Pointer to the task
 * @param[in]     TaskId: Task Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SyncSetTask(uint32* Task, Dem_TaskIdType TaskId)
{
    uint32 oldTask;
    uint32 newTask;
    uint32 taskMask = (uint32)1u << (uint8)TaskId; /* PRQA S 1258 */ /* VL_Dem_1258 */
    do
    {
        oldTask = *Task;
        newTask = oldTask | taskMask;
    } while (Dem_ComSyncCompareAndSwap(Task, oldTask, newTask) == FALSE);
}

/**
 * @brief         Scheduler Task enable
 * @param[in]     TaskId: Task Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_EnableTask(Dem_TaskIdType TaskId)
{
    Dem_SyncSetTask(&Dem_MultipleSchedulerTask, TaskId);
}

/**
 * @brief         Scheduler once Task enable
 * @param[in]     TaskId: Task Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_EnableTaskOnce(Dem_TaskIdType TaskId)
{
    Dem_SyncSetTask(&Dem_SingleSchedulerTask, TaskId);
}

/**
 * @brief         Single Scheduler Task get an clear
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint32 Dem_GetAndClearSingleScheduledTask(void)
{
    uint32 singleScheduledTask = Dem_SingleSchedulerTask;
    if (singleScheduledTask > 0u) /* PRQA S 1252 */ /* VL_Dem_1252 */
    {
        do
        {
            /** idle */
        } while (Dem_ComSyncCompareAndSwap(&Dem_SingleSchedulerTask, singleScheduledTask, 0u) == FALSE);
    }
    return singleScheduledTask;
}

/**
 * @brief Scheduler Task process
 */
DEM_LOCAL void Dem_TaskScheduler(void)
{
    uint32                    activeTasks = (uint32)(Dem_MultipleSchedulerTask | Dem_GetAndClearSingleScheduledTask());
    uint8                     currentTask = 0u;
    const Dem_TaskFunctionPtr Dem_TaskFunction[Dem_Task_Invalid] = {
#if ((DEM_MAX_NUMBER_PRESTORED_FF > 0u) && (DEM_EVENT_PARTITION_NUMBER > 1u))
        Dem_PrestorageTask,
#endif
        Dem_ClearTaskTask,
#if (DEM_AVAILABILITY_SUPPORT == DEM_EVENT_AVAILABILITY)
        Dem_EventTask,
#endif
#if (DEM_DCM_CLIENT_NUMBER > 0u)
        Dem_FilterEventDataTask,
        Dem_DTCSettingTask,
#endif
#if (DEM_ENABLE_CONDITION_GROUP_NUMBER > 0u)
        Dem_EnableConditionTask,
#endif
        Dem_EventQueueTask,
        Dem_OperationCycleTask,
#if (DEM_FEATURE_NVM_IMMEDIATE == STD_ON)
        Dem_NvMTask,
#endif
    };

    while (activeTasks != 0u) /* PRQA S 1252 */ /* VL_Dem_1252 */
    {
        while (DEM_FLAG_ISNSET(activeTasks, 1u)) /* PRQA S 1252 */ /* VL_Dem_1252 */
        {
            activeTasks = (uint32)(activeTasks >> 1u);
            ++currentTask;
        }

        (*Dem_TaskFunction[(Dem_TaskIdType)currentTask])(); /* PRQA S 4342 */ /* VL_Dem_4342 */

        activeTasks = (uint32)(activeTasks >> 1u);
        ++currentTask;
    }
}

/**
 * @brief Master partition pre-initialization
 */
DEM_LOCAL void Dem_MasterPreInit(const Dem_ConfigType* ConfigPtr)
{
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if ((Dem_CheckMasterInitStatus(DEM_UNINITIALIZED) == FALSE))
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
    else if (ConfigPtr == NULL_PTR)
    {
        errorId = DEM_E_PARAM_POINTER;
    }
    else
#endif
    {
        Dem_GlobalConfigPtr = ConfigPtr;

        Dem_SatelliteMemoryInit();
#if (DEM_ENABLE_CONDITION_GROUP_NUMBER > 0u)
        Dem_EnableConditionPreInit();
#endif
#if (DEM_STORAGE_CONDITION_GROUP_NUMBER > 0u)
        Dem_StorageConditionPreInit();
#endif
        Dem_OperationCyclePreInit();
        Dem_EventPreInit();
#if (DEM_COMBINATION_NUMBER > 0u)
        Dem_CombinedEventPreInit();
#endif
        Dem_SetMasterInitStatus(DEM_PREINITIALIZED);
    }

#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_PREINIT, errorId);
    }
#endif
}

/**
 * @brief Master partition initialization
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
DEM_LOCAL void Dem_MasterInit(const Dem_ConfigType* ConfigPtr)
/* PRQA S 6070 -- */
{
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if ((Dem_CheckMasterInitStatus(DEM_PREINITIALIZED) == FALSE) && (Dem_CheckMasterInitStatus(DEM_SHUTDOWN) == FALSE))
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
    else if (Dem_CheckAllLeastSatelliteInitStatus(DEM_PREINITIALIZED) == FALSE)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
    else if (ConfigPtr == NULL_PTR)
    {
        errorId = DEM_E_PARAM_POINTER;
    }
    else
#endif
    {
        Dem_GlobalConfigPtr = ConfigPtr;
        Dem_SchedulerTaskInit();
        Dem_InitClientInfo();
        Dem_EventQueueInit();
        Dem_ClearTaskInit();
#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
        Dem_NvmInit();
#endif
        Dem_MemoryInit();
#if (DEM_INDICATOR_NUMBER > 0u)
        Dem_IndicatorInit();
#endif
        Dem_EventInit();
        Dem_OperationCycleInit();
#if (DEM_COMBINATION_NUMBER > 0u)
        Dem_CombinedEventInit();
#endif

        Dem_SetMasterInitStatus(DEM_INITIALIZED);
#if (DEM_TRIGGER_FIM_REPORTS == STD_ON)
        FiM_DemInit();
#endif
    }

#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_INIT, errorId);
    }
#endif
}

/**
 * @brief         Master partition Main Function
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_MasterMainFunction(void)
{
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    if (Dem_CheckLeastMasterInitStatus(DEM_PREINITIALIZED) == FALSE)
    {
        Dem_DetReportError(DEM_SID_MAINFUNCTION, DEM_E_UNINIT);
    }
    else
#endif
    {
        if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == TRUE)
        {
            Dem_TaskScheduler();
        }
    }
}

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
