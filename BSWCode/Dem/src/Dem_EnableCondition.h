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
 **  @file               : Dem_EnableCondition.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : Implementation file of Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_ENABLECONDITION_H
#define DEM_ENABLECONDITION_H

/* =================================================== inclusions =================================================== */
#include "Dem_Lcfg.h"
#if (DEM_ENABLE_CONDITION_GROUP_NUMBER > 0u)
#include "Dem_SatelliteInterface.h"
#include "Dem_MasterInterface.h"

/* ===================================================== macros ===================================================== */
/**
 * @name DEM_ENABLECONDITIONGROUP_ENCODE
 * @{
 * @brief Identification of enable condition group
 */
#define DEM_ENABLECONDITIONGROUP_ACTIVE        (0x1u) /**< the active state of an enable condition group */
#define DEM_ENABLECONDITIONGROUP_QUEUE_DISABLE (0x2u) /**< the 'queued disable' state of an enable condition group */
#define DEM_ENABLECONDITIONGROUP_QUEUE_ENABLE  (0x4u) /**< the 'queued enable' state of an enable condition group */
#define DEM_ENABLECONDITIONGROUP_PROCESS_DISABLE                         \
    (0x8u) /**< the 'process disable' state of an enable condition group \
            */
#define DEM_ENABLECONDITIONGROUP_PROCESS_ENABLE                          \
    (0x10u) /**< the 'process enable' state of an enable condition group \
             */
/** @} */

#define DEM_EC_VALID_ALL (0xFFu) /**< Initialization valid state */

/* ========================================== internal function declarations ======================================== */
/**
 * @brief         Sets the enable condition
 * @param[in]     EnableConditionID: This parameter identifies the enable condition.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_SetEnableConditionFulfilled(uint8 EnableConditionID);

/**
 * @brief         Clears the enable condition
 * @param[in]     EnableConditionID: This parameter identifies the enable condition.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_ResetEnableConditionFulfilled(uint8 EnableConditionID);

/**
 * @brief         Processes changes of enable condition states
 * @param[in]     EventId: Identification of an event by assigned EventId.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_ProcessQueueEventEnableConditions(Dem_EventIdType EventId);

/**
 * @brief         Process enable condition changes
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_EnableConditionTask(void);

/**
 * @brief         Initializes the enable conditions and enable condition groups
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_EnableConditionInit(void);

/* ========================================== internal function definitions ========================================= */
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/* PRQA S 3473 ++ */ /* VL_Dem_3473 */
/**
 * @brief         Set the active state of an enable condition group
 * @param[in]     ECGState: State of enable condition group
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_SetECGStateActive(uint8 ECGState)
{
    uint8 state = ECGState;
    DEM_FLAG_SET(state, DEM_ENABLECONDITIONGROUP_ACTIVE);
    return state;
}

/* PRQA S 4399, 4461 ++ */ /* VL_Dem_4399, VL_Dem_4461 */
/**
 * @brief Reset the active state of an enable condition group
 * @param[in]     ECGState: State of enable condition group
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_ResetECGStateActive(uint8 ECGState)
{
    uint8 state = ECGState;
    DEM_FLAG_RESET(state, DEM_ENABLECONDITIONGROUP_ACTIVE);
    return state;
}

/**
 * @brief         Check the active state of an enable condition group
 * @param[in]     ECGState: State of enable condition group
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckECGStateActive(uint8 ECGState)
{
    return DEM_FLAG_ISSET(ECGState, DEM_ENABLECONDITIONGROUP_ACTIVE); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/**
 * @brief         Set the 'queued disable' state of an enable condition group
 * @param[in]     ECGState: State of enable condition group
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_SetECGStateQueuedDisable(uint8 ECGState)
{
    uint8 state = ECGState;
    DEM_FLAG_SET(state, DEM_ENABLECONDITIONGROUP_QUEUE_DISABLE);
    return state;
}

/**
 * @brief         Reset the 'queued disable' state of an enable condition group
 * @param[in]     ECGState: State of enable condition group
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_ResetECGStateQueuedDisable(uint8 ECGState)
{
    uint8 state = ECGState;
    DEM_FLAG_RESET(state, DEM_ENABLECONDITIONGROUP_QUEUE_DISABLE);
    return state;
}

/**
 * @brief         Check the 'queued disable' state of an enable condition group
 * @param[in]     ECGState: State of enable condition group
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckECGStateQueuedDisable(uint8 ECGState)
{
    return DEM_FLAG_ISSET(ECGState, DEM_ENABLECONDITIONGROUP_QUEUE_DISABLE); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/**
 * @brief         Set the 'queued enable' state of an enable condition group
 * @param[in]     ECGState: State of enable condition group
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_SetECGStateQueuedEnable(uint8 ECGState)
{
    uint8 state = ECGState;
    DEM_FLAG_SET(state, DEM_ENABLECONDITIONGROUP_QUEUE_ENABLE);
    return state;
}

/**
 * @brief         Reset the 'queued enable' state of an enable condition group
 * @param[in]     ECGState: State of enable condition group
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_ResetECGStateQueuedEnable(uint8 ECGState)
{
    uint8 state = ECGState;
    DEM_FLAG_RESET(state, DEM_ENABLECONDITIONGROUP_QUEUE_ENABLE);
    return state;
}

/**
 * @brief         Check the 'queued enable' state of an enable condition group
 * @param[in]     ECGState: State of enable condition group
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckECGStateQueuedEnable(uint8 ECGState)
{
    return DEM_FLAG_ISSET(ECGState, DEM_ENABLECONDITIONGROUP_QUEUE_ENABLE); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/**
 * @brief         Set the 'process disable' state of an enable condition group
 * @param[in]     ECGState: State of enable condition group
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_SetECGStateProcessDisable(uint8 ECGState)
{
    uint8 state = ECGState;
    DEM_FLAG_SET(state, DEM_ENABLECONDITIONGROUP_PROCESS_DISABLE);
    return state;
}

/**
 * @brief         Reset the 'process disable' state of an enable condition group
 * @param[in]     ECGState: State of enable condition group
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_ResetECGStateProcessDisable(uint8 ECGState)
{
    uint8 state = ECGState;
    DEM_FLAG_RESET(state, DEM_ENABLECONDITIONGROUP_PROCESS_DISABLE);
    return state;
}

/**
 * @brief         Check the 'process disable' state of an enable condition group
 * @param[in]     ECGState: State of enable condition group
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckECGStateProcessDisable(uint8 ECGState)
{
    return DEM_FLAG_ISSET(ECGState, DEM_ENABLECONDITIONGROUP_PROCESS_DISABLE); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/**
 * @brief         Set the 'process enable' state of an enable condition group
 * @param[in]     ECGState: State of enable condition group
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_SetECGStateProcessEnable(uint8 ECGState)
{
    uint8 state = ECGState;
    DEM_FLAG_SET(state, DEM_ENABLECONDITIONGROUP_PROCESS_ENABLE);
    return state;
}

/**
 * @brief         Reset the 'process enable' state of an enable condition group
 * @param[in]     ECGState: State of enable condition group
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_ResetECGStateProcessEnable(uint8 ECGState)
{
    uint8 state = ECGState;
    DEM_FLAG_RESET(state, DEM_ENABLECONDITIONGROUP_PROCESS_ENABLE);
    return state;
}
/* PRQA S 4399, 4461 -- */
/* PRQA S 3473 -- */

/**
 * @brief         Check the 'process enable' state of an enable condition group
 * @param[in]     ECGState: State of enable condition group
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckECGStateProcessEnable(uint8 ECGState)
{
    return DEM_FLAG_ISSET(ECGState, DEM_ENABLECONDITIONGROUP_PROCESS_ENABLE); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/**
 * @brief         Get current activation state of enable condition
 * @param[in]     EnableConditionID: Enable condition ID
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_GetEnableConditionActive(uint8 EnableConditionID)
{
    return DEM_BITS_ISSET(Dem_GetECStatus(), EnableConditionID); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/**
 * @brief         Set current activation state of enable condition
 * @param[in]     EnableConditionID: Enable condition ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetEnableConditionActive(uint8 EnableConditionID)
{
    DEM_BITS_SET(Dem_GetECStatus(), EnableConditionID);
}

/**
 * @brief         Reset current activation state of enable condition
 * @param[in]     EnableConditionID: Enable condition ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_ResetEnableConditionActive(uint8 EnableConditionID)
{
    DEM_BITS_RESET(Dem_GetECStatus(), EnableConditionID);
}

/**
 * @brief         Get Init Status of enable condition
 * @param[in]     EnableConditionID: Enable condition ID
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_GetEnableConditionInitStatus(uint8 EnableConditionID)
{
    return DEM_BITS_ISSET(Dem_GetECInitStatus(), EnableConditionID); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/**
 * @brief         Set current internal counter of enable condition group
 * @param[in]     ECG: Enable condition group
 * @param[in]     Counter: Enable condition group counter
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetEnableConditionGroupCounter(uint8 ECG, uint8 Counter)
{
    Dem_GetECGCounter()[ECG] = Counter;
}

/**
 * @brief         Get current internal counter of enable condition group
 * @param[in]     ECG: Enable condition group
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetEnableConditionGroupCounter(uint8 ECG)
{
    return Dem_GetECGCounter()[ECG];
}

/**
 * @brief         Get current internal state of enable condition group
 * @param[in]     ECG: Enable condition group
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetEnableConditionGroupState(uint8 ECG)
{
    return Dem_GetECGStatus()[ECG];
}

/**
 * @brief         Set current internal state of enable condition group
 * @param[in]     ECG: Enable condition group
 * @param[in]     ECGState: State of enable condition group
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetEnableConditionGroupState(uint8 ECG, uint8 ECGState)
{
    Dem_GetECGStatus()[ECG] = ECGState;
}

/**
 * @brief Sets the enable condition
 */
DEM_LOCAL void Dem_SetEnableConditionFulfilled(uint8 EnableConditionID)
{
    boolean lECGStatePending = FALSE;

    /** Enter Critical Section: StateManager */
    Dem_EnterCritical_StateManager();
    if (Dem_GetEnableConditionActive(EnableConditionID) == FALSE)
    {
        for (Dem_ECGRefNumType start = Dem_GetStartOfECGRef(EnableConditionID);
             start < Dem_GetEndOfECGRef(EnableConditionID);
             ++start)
        {
            uint8 enableConditionGroup = Dem_GetECG(start);
            uint8 enableConditionGroupState;

            Dem_SetEnableConditionGroupCounter(
                enableConditionGroup,
                (Dem_GetEnableConditionGroupCounter(enableConditionGroup) - 1u));
            enableConditionGroupState = Dem_GetEnableConditionGroupState(enableConditionGroup);
            enableConditionGroupState = Dem_SetECGStateQueuedEnable(enableConditionGroupState);
            Dem_SetEnableConditionGroupState(enableConditionGroup, enableConditionGroupState);
            lECGStatePending = TRUE;
        }
        Dem_SetEnableConditionActive(EnableConditionID);
    }
    Dem_LeaveCritical_StateManager();
    /** Leave Critical Section: StateManager */

    if (lECGStatePending == TRUE)
    {
        Dem_EnableTaskOnce(Dem_Task_EnableCondition);
    }
}

/**
 * @brief Clears the enable condition
 */
DEM_LOCAL void Dem_ResetEnableConditionFulfilled(uint8 EnableConditionID)
{
    boolean lECGStatePending = FALSE;

    /** Enter Critical Section: StateManager */
    Dem_EnterCritical_StateManager();
    if (Dem_GetEnableConditionActive(EnableConditionID) == TRUE)
    {
        for (Dem_ECGRefNumType start = Dem_GetStartOfECGRef(EnableConditionID);
             start < Dem_GetEndOfECGRef(EnableConditionID);
             ++start)
        {
            uint8 enableConditionGroup = Dem_GetECG(start);
            uint8 enableConditionGroupState;
            Dem_SetEnableConditionGroupCounter(
                enableConditionGroup,
                (Dem_GetEnableConditionGroupCounter(enableConditionGroup) + 1u));

            enableConditionGroupState = Dem_GetEnableConditionGroupState(enableConditionGroup);
            enableConditionGroupState = Dem_ResetECGStateActive(enableConditionGroupState);
            enableConditionGroupState = Dem_SetECGStateQueuedDisable(enableConditionGroupState);
            enableConditionGroupState = Dem_ResetECGStateQueuedEnable(enableConditionGroupState);
            Dem_SetEnableConditionGroupState(enableConditionGroup, enableConditionGroupState);
            lECGStatePending = TRUE;
        }
        Dem_ResetEnableConditionActive(EnableConditionID);
    }
    Dem_LeaveCritical_StateManager();
    /** Leave Critical Section: StateManager */

    if (lECGStatePending == TRUE)
    {
        Dem_EnableTaskOnce(Dem_Task_EnableCondition);
    }
}

/**
 * @brief Processes changes of enable condition states
 */
DEM_LOCAL void Dem_ProcessQueueEventEnableConditions(Dem_EventIdType EventId)
{
    uint8 enableConditionGroup = Dem_GetECGRefOfEvent(EventId);
    if (DEM_ENABLE_CONDITION_GROUP_NUMBER != enableConditionGroup)
    {
        uint8 enableConditionGroupState = Dem_GetEnableConditionGroupState(enableConditionGroup);

        /** Reset/Freeze debouncing on enable condition state change unlocked -> locked */
        if (Dem_CheckECGStateProcessDisable(enableConditionGroupState) == TRUE)
        {
            if (Dem_GetEnableConditionGroupCounter(enableConditionGroup) == 1u)
            {
                /** Enable conditions become disabled.
                 * If the event is configured to freeze: freeze it
                 * Otherwise: reset it
                 */
                if (Dem_GetDebounceBehaviorByEvent(EventId) == FALSE)
                {
                    Dem_SatelliteFreezeDebounceStatus(EventId);
                }
                else
                {
                    Dem_SatelliteResetDebounceStatus(EventId);
                }
            }
        }

        if (Dem_CheckECGStateProcessEnable(enableConditionGroupState) == TRUE)
        {
            if (Dem_GetEnableConditionGroupCounter(enableConditionGroup) == 0u)
            {
                Dem_NotiInitMonitorForEvent(EventId, DEM_INIT_MONITOR_REENABLED);
            }
        }
    }
}

/**
 * @brief Process enable condition changes
 */
DEM_LOCAL void Dem_EnableConditionTask(void)
{
    Dem_EventIdType eventId;
    uint8           enableConditionGroup;

    /** Enter Critical Section: StateManager */
    Dem_EnterCritical_StateManager();
    for (enableConditionGroup = 0u; enableConditionGroup < DEM_ENABLE_CONDITION_GROUP_NUMBER; ++enableConditionGroup)
    {
        uint8 enableConditionGroupState = Dem_GetEnableConditionGroupState(enableConditionGroup);
        if (Dem_CheckECGStateQueuedDisable(enableConditionGroupState) == TRUE)
        {
            enableConditionGroupState = Dem_SetECGStateProcessDisable(enableConditionGroupState);
            enableConditionGroupState = Dem_ResetECGStateQueuedDisable(enableConditionGroupState);
        }
        if (Dem_CheckECGStateQueuedEnable(enableConditionGroupState) == TRUE)
        {
            enableConditionGroupState = Dem_SetECGStateProcessEnable(enableConditionGroupState);
            enableConditionGroupState = Dem_ResetECGStateQueuedEnable(enableConditionGroupState);
        }
        Dem_SetEnableConditionGroupState(enableConditionGroup, enableConditionGroupState);
    }
    Dem_LeaveCritical_StateManager();
    /** Leave Critical Section: StateManager */

    for (eventId = DEM_EVENT_FIRST; eventId < Dem_GetEventNumber(); ++eventId)
    {
        if ((Dem_GetAvailableInVariantByEvent(eventId) == TRUE)
            && (Dem_GetEventAvailableStatusByEventId(eventId) == TRUE))
        {
            /** Process enable condition groups for every event */
            Dem_ProcessQueueEventEnableConditions(eventId);
        }
    }

    /** Enter Critical Section: StateManager */
    Dem_EnterCritical_StateManager();
    /** Remove process flags from group state of all enable condition groups */
    for (enableConditionGroup = 0u; enableConditionGroup < DEM_ENABLE_CONDITION_GROUP_NUMBER; ++enableConditionGroup)
    {
        uint8 enableConditionGroupState = Dem_GetEnableConditionGroupState(enableConditionGroup);
        if (Dem_CheckECGStateProcessEnable(enableConditionGroupState) == TRUE)
        {
            if (Dem_GetEnableConditionGroupCounter(enableConditionGroup) == 0u)
            {
                enableConditionGroupState = Dem_SetECGStateActive(enableConditionGroupState);
            }
        }
        enableConditionGroupState = Dem_ResetECGStateProcessDisable(enableConditionGroupState);
        enableConditionGroupState = Dem_ResetECGStateProcessEnable(enableConditionGroupState);
        Dem_SetEnableConditionGroupState(enableConditionGroup, enableConditionGroupState);
    }
    Dem_LeaveCritical_StateManager();
    /** Leave Critical Section: StateManager */
}

/**
 * @brief Initializes the enable conditions and enable condition groups
 */
DEM_LOCAL void Dem_EnableConditionInit(void)
{
    uint8 enableConditionGroupState;
    /** Initialize enable condition group counter and set enable condition group state active */
    Dem_MemSet(Dem_GetECGCounter(), 0u, DEM_ENABLE_CONDITION_GROUP_NUMBER);

    enableConditionGroupState = 0u;
    enableConditionGroupState = Dem_SetECGStateActive(enableConditionGroupState);
    Dem_MemSet(Dem_GetECGStatus(), enableConditionGroupState, DEM_ENABLE_CONDITION_GROUP_NUMBER);

    for (uint8 EnableConditionID = 0u; EnableConditionID < DEM_ENABLE_CONDITION_ID_NUMBER; ++EnableConditionID)
    {
        if (Dem_GetEnableConditionInitStatus(EnableConditionID) == FALSE)
        {
            for (Dem_ECGRefNumType start = Dem_GetStartOfECGRef(EnableConditionID);
                 start < Dem_GetEndOfECGRef(EnableConditionID);
                 ++start)
            {
                uint8 enableConditionGroup = Dem_GetECG(start);
                Dem_SetEnableConditionGroupCounter(
                    enableConditionGroup,
                    (Dem_GetEnableConditionGroupCounter(enableConditionGroup) + 1u));
                enableConditionGroupState =
                    Dem_ResetECGStateActive(Dem_GetEnableConditionGroupState(enableConditionGroup));
                Dem_SetEnableConditionGroupState(enableConditionGroup, enableConditionGroupState);
            }
            Dem_ResetEnableConditionActive(EnableConditionID);
        }
    }
}

/**
 * @brief         Enables all enable conditions
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_EnableConditionPreInit(void)
{
    uint8 enableConditionGroupState;
    /** Initialize enable conditions (all enabled due to init phase) */
    Dem_MemSet(Dem_GetECStatus(), DEM_EC_VALID_ALL, DEM_ENABLE_CONDITION_ID_BYTE);
    Dem_MemSet(Dem_GetECGCounter(), 0u, DEM_ENABLE_CONDITION_GROUP_NUMBER);
    enableConditionGroupState = 0u;
    enableConditionGroupState = Dem_SetECGStateActive(enableConditionGroupState);
    Dem_MemSet(Dem_GetECGStatus(), enableConditionGroupState, DEM_ENABLE_CONDITION_GROUP_NUMBER);
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Get the current state of the request operation cycle
 * @param[in]     EventId: Identification of an event by assigned EventId.
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckECFulfilled(Dem_EventIdType EventId)
{
    boolean ret = TRUE;
#if (DEM_ENABLE_CONDITION_GROUP_NUMBER > 0u)
    if (Dem_GetECGRefOfEvent(EventId) != DEM_ENABLE_CONDITION_GROUP_NUMBER)
    {
        ret = Dem_CheckECGStateActive(Dem_GetEnableConditionGroupState(Dem_GetECGRefOfEvent(EventId)));
    }
#else
    DEM_PARAM_UNUSED(EventId);
#endif
    return ret;
}

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
