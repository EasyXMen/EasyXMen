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
 **  @file               : Dem_StorageCondition.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : Implementation file of Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_STORAGECONDITION_H
#define DEM_STORAGECONDITION_H

/* =================================================== inclusions =================================================== */
#include "Dem_Lcfg.h"
#if (DEM_STORAGE_CONDITION_GROUP_NUMBER > 0u)

/* ===================================================== macros ===================================================== */
#define DEM_SC_VALID_ALL (0xFFu) /**< Initialization valid state */

/* ========================================== internal function declarations ======================================== */
/**
 * @brief         Sets the storage condition Fulfilled
 * @param[in]     StorageConditionID: Storage condition Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_SetStorageConditionFulfilled(uint8 StorageConditionID);

/**
 * @brief         Reset the storage condition Fulfilled
 * @param[in]     StorageConditionID: Storage condition Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_ResetStorageConditionFulfilled(uint8 StorageConditionID);

/**
 * @brief         Initializes the storage conditions and storage condition groups
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_StorageConditionInit(void);
/* ========================================== internal function definitions ========================================= */
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"

/**
 * @brief         Get Init Status of storage condition
 * @param[in]     StorageConditionID: Storage condition Id
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_GetStorageConditionInitStatus(uint8 StorageConditionID)
{
    return DEM_BITS_ISSET(Dem_GetSCInitStatus(), StorageConditionID); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/**
 * @brief         Get Status of storage condition
 * @param[in]     StorageConditionID: Storage condition Id
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_GetStorageConditionStatus(uint8 StorageConditionID)
{
    return DEM_BITS_ISSET(Dem_GetSCStatus(), StorageConditionID); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/**
 * @brief         Set Status of storage condition
 * @param[in]     StorageConditionID: Storage condition Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetStorageConditionStatus(uint8 StorageConditionID)
{
    DEM_BITS_SET(Dem_GetSCStatus(), StorageConditionID);
}

/**
 * @brief         Reset Status of storage condition
 * @param[in]     StorageConditionID: Storage condition Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_ResetStorageConditionStatus(uint8 StorageConditionID)
{
    DEM_BITS_RESET(Dem_GetSCStatus(), StorageConditionID);
}

/**
 * @brief         Set current internal counter of Status condition group
 * @param[in]     storageConditionGroup: Storage condition group
 * @param[in]     Counter: Storage condition group counter
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetStorageConditionGroupCounter(uint8 storageConditionGroup, uint8 Counter)
{
    Dem_GetSCGCounter()[storageConditionGroup] = Counter;
}

/**
 * @brief         Get current internal counter of Status condition group
 * @param[in]     storageConditionGroup: Storage condition group
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetStorageConditionGroupCounter(uint8 storageConditionGroup)
{
    return Dem_GetSCGCounter()[storageConditionGroup];
}

/**
 * @brief         Enables all storage conditions
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_StorageConditionPreInit(void)
{
    /** Initialize storage conditions (all enabled due to init phase) */
    Dem_MemSet(Dem_GetSCStatus(), DEM_SC_VALID_ALL, DEM_STORAGE_CONDITION_ID_BYTE);
    Dem_MemSet(Dem_GetSCGCounter(), 0u, DEM_STORAGE_CONDITION_GROUP_NUMBER);
}

/**
 * @brief Sets the storage condition Fulfilled
 */
DEM_LOCAL void Dem_SetStorageConditionFulfilled(uint8 StorageConditionID)
{
    /** Enter Critical Section: StateManager */
    Dem_EnterCritical_StateManager();

    if (Dem_GetStorageConditionStatus(StorageConditionID) == FALSE)
    {
        for (Dem_SCGRefNumType start = Dem_GetStartOfSCGRef(StorageConditionID);
             start < Dem_GetEndOfSCGRef(StorageConditionID);
             ++start)
        {
            uint8 storageConditionGroup = Dem_GetSCGIndex(start);
            Dem_SetStorageConditionGroupCounter(
                storageConditionGroup,
                (Dem_GetStorageConditionGroupCounter(storageConditionGroup) - 1u));
            if (Dem_GetStorageConditionGroupCounter(storageConditionGroup) == 0u)
            {
                for (Dem_EventIdType eventId = DEM_EVENT_FIRST; eventId < Dem_GetEventNumber(); ++eventId)
                {
                    if ((Dem_GetAvailableInVariantByEvent(eventId) == TRUE)
                        && (Dem_GetEventAvailableStatusByEventId(eventId) == TRUE))
                    {
                        Dem_NotiInitMonitorForEvent(eventId, DEM_INIT_MONITOR_REENABLED);
                    }
                }
            }
        }
        Dem_SetStorageConditionStatus(StorageConditionID);
    }

    Dem_LeaveCritical_StateManager();
    /** Leave Critical Section: StateManager */
}

/**
 * @brief Reset the storage condition Fulfilled
 */
DEM_LOCAL void Dem_ResetStorageConditionFulfilled(uint8 StorageConditionID)
{
    /** Enter Critical Section: StateManager */
    Dem_EnterCritical_StateManager();

    if (Dem_GetStorageConditionStatus(StorageConditionID) == TRUE)
    {
        for (Dem_SCGRefNumType start = Dem_GetStartOfSCGRef(StorageConditionID);
             start < Dem_GetEndOfSCGRef(StorageConditionID);
             ++start)
        {
            uint8 storageConditionGroup = Dem_GetSCGIndex(start);
            Dem_SetStorageConditionGroupCounter(
                storageConditionGroup,
                (Dem_GetStorageConditionGroupCounter(storageConditionGroup) + 1u));
        }
        Dem_ResetStorageConditionStatus(StorageConditionID);
    }

    Dem_LeaveCritical_StateManager();
    /** Leave Critical Section: StateManager */
}

/**
 * @brief Initializes the storage conditions and storage condition groups
 */
DEM_LOCAL void Dem_StorageConditionInit(void)
{
    /** Initialize enable condition group counter and set enable condition group state active */
    Dem_MemSet(Dem_GetSCGCounter(), 0u, DEM_STORAGE_CONDITION_GROUP_NUMBER);
    Dem_MemSet(Dem_GetSCStatus(), DEM_SC_VALID_ALL, DEM_STORAGE_CONDITION_ID_BYTE);

    for (uint8 storageConditionID = 0u; storageConditionID < DEM_STORAGE_CONDITION_ID_NUMBER; ++storageConditionID)
    {
        if (Dem_GetStorageConditionInitStatus(storageConditionID) == FALSE)
        {
            for (Dem_SCGRefNumType start = Dem_GetStartOfSCGRef(storageConditionID);
                 start < Dem_GetEndOfSCGRef(storageConditionID);
                 ++start)
            {
                uint8 storageConditionGroup = Dem_GetSCGIndex(start);
                Dem_SetStorageConditionGroupCounter(
                    storageConditionGroup,
                    (Dem_GetStorageConditionGroupCounter(storageConditionGroup) + 1u));
                Dem_ResetStorageConditionStatus(storageConditionID);
            }
        }
    }
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Get the current state of the request operation cycle
 * @param[in]     EventId: Event Id
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckSCFulfilled(Dem_EventIdType EventId)
{
    boolean ret = TRUE;
#if (DEM_STORAGE_CONDITION_GROUP_NUMBER > 0u)
    if (Dem_GetSCGRefOfEvent(EventId) != DEM_STORAGE_CONDITION_GROUP_NUMBER)
    {
        if (Dem_GetStorageConditionGroupCounter(Dem_GetSCGRefOfEvent(EventId)) > 0u)
        {
            ret = FALSE;
        }
    }
#else
    DEM_PARAM_UNUSED(EventId);
#endif
    return ret;
}

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
