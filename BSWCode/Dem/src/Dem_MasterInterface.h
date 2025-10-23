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
/*
 ************************************************************************************************************************
 **
 **  @file               : Dem_MasterInterface.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : Declaration file of Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_MASTER_INTERFACE_H
#define DEM_MASTER_INTERFACE_H

/* =================================================== inclusions =================================================== */
#include "Dem_Lcfg.h"

/* ================================================ type definitions ================================================ */
/**
 * @brief Task Handle, in order of priority
 */
typedef enum
{
#if ((DEM_MAX_NUMBER_PRESTORED_FF > 0u) && (DEM_EVENT_PARTITION_NUMBER > 1u))
    Dem_Task_Prestorage, /**< Prestorage task @range 0u */
#endif
    Dem_Task_ClearTask, /**< Clear task @range 1u */
#if (DEM_AVAILABILITY_SUPPORT == DEM_EVENT_AVAILABILITY)
    Dem_Task_Event, /**< Event task @range 2u */
#endif
#if (DEM_DCM_CLIENT_NUMBER > 0u)
    Dem_Task_FilterEventData, /**< Filter Event Data task @range 3u */
    Dem_Task_DTCSetting,      /**< DTC Setting task @range 4u */
#endif
#if (DEM_ENABLE_CONDITION_GROUP_NUMBER > 0u)
    Dem_Task_EnableCondition, /**< Enable Condition task @range 5u */
#endif
    Dem_Task_EventQueue,     /**< Event Queue task @range 6u */
    Dem_Task_OperationCycle, /**< Operation Cycle task @range 7u */
#if ((DEM_IMMEDIATE_STORAGE == STD_ON) || (DEM_CLEAR_DTC_BEHAVIOR != DEM_CLRRESP_VOLATILE))
    Dem_Task_NvM, /**< NvM task @range 8u */
#endif
    Dem_Task_Invalid /**< Invalid task @range 9u */
} Dem_TaskIdType;

/* ========================================== internal function declarations ======================================== */
/**
 * @brief         Scheduler Task enable
 * @param[in]     TaskId: Task Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_EnableTask(Dem_TaskIdType TaskId);

/**
 * @brief         Scheduler once Task enable
 * @param[in]     TaskId: Task Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_EnableTaskOnce(Dem_TaskIdType TaskId);

/**
 * @brief         Scheduler Task disable
 * @param[in]     TaskId: Task Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_DisableTask(Dem_TaskIdType TaskId);

#endif
