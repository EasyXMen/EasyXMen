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
 **  @file               : Dem_OperationCycle.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : Implementation file of Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_OPERATIONCYCLE_H
#define DEM_OPERATIONCYCLE_H

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Dem_MemInterface.h"
#include "Dem_MasterInterface.h"
#include "Dem_EventInterface.h"
#include "Dem_DtcInterface.h"
#include "Dem_InitState.h"

/* ===================================================== macros ===================================================== */
#define DEM_OPERATION_CYCLE_BYTE_SIZE   (0x8u)  /**< byte size of restart Flag */
#define DEM_OPERATION_CYCLE_COUNTER_MAX (0xFFu) /**< max value of counter */

/* ========================================== internal function declarations ======================================== */
#if (                                                                                                          \
    (defined(DEM_CYCLES_SINCE_FIRST_FAILED)) || (defined(DEM_CYCLES_SINCE_LAST_FAILED)) || (defined(DEM_OCC1)) \
    || (defined(DEM_OCC3)) || (defined(DEM_AGINGCTR_DOWNCNT)) || (defined(DEM_AGINGCTR_UPCNT_FIRST_ACTIVE))    \
    || (defined(DEM_AGINGCTR_UPCNT)))
/**
 * @brief         Calculates the distance between cycle counter values
 * @param[in]     CurrentCounter: The current counter
 * @param[in]     TargetCounter: The target counter
 * @return        uint16
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL uint16 Dem_CycleCountDistance(uint16 CurrentCounter, uint16 TargetCounter);
#endif

/**
 * @brief         Processes cycle update for stored entries.
 * This function processes the aging cycle updates for all events currently stored in the event memory. This has effect
 * only on events using the given aging cycle, when their aging  condition is fulfilled for this cycle. This function
 * also processes data updates on operation cycle change.
 * @param[in]     OperationCycleId: Identification of operation cycle, like power cycle, driving cycle.
 * @param[in]     CycleCounter: Cycle Counter
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_OperationCycleStored(uint8 OperationCycleId, uint16 CycleCounter);

/**
 * @brief         Checks if passed event is affected by cycle update
 * @param[in]     OperationCycleId: Identification of operation cycle, like power cycle, driving cycle.
 * @param[in]     EventId: Event Id
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_UpdateCheckEventAffected(Dem_EventIdType EventId, uint8 OperationCycleId);

/**
 * @brief         Process all queued operation cycle changes
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_OperationCycleTask(void);

/* ========================================== internal function definitions ========================================= */
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"

/**
 * @brief         Init the state of the operation cycle
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_OperationCyclePreInit(void)
{
    Dem_MemSet(Dem_GetOCRestartFlag(), 0x0u, DEM_SIZE_OPERATION_CYCLE_NUMBER);
}

/**
 * @brief         Get current counter of the cycle
 * @param[in]     OperationCycleId: Identification of operation cycle, like power cycle, driving cycle.
 * @return        uint16
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint16 Dem_GetCycleCounter(uint8 OperationCycleId)
{
    return Dem_GetAdminData().CycleCounter[OperationCycleId];
}

/**
 * @brief         Set current counter value of the cycle
 * @param[in]     OperationCycleId: Identification of operation cycle, like power cycle, driving cycle.
 * @param[in]     CycleCounter: Cycle Counter
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetCycleCounter(uint8 OperationCycleId, uint16 CycleCounter)
{
    Dem_GetAdminData().CycleCounter[OperationCycleId] = CycleCounter;
}

/**
 * @brief         Add current counter value of the cycle
 * @param[in]     CurrentCounter: The current counter
 * @param[in]     DeltaValue: The delta value
 * @return        uint16
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint16 Dem_AddCycleCount(uint16 CurrentCounter, uint8 DeltaValue)
{
    uint16 targetCycle = (uint16)(CurrentCounter + DeltaValue);

    if (targetCycle > DEM_OPERATIONCYCLE_CYCLECOUNT_MAX)
    {
        /** Overflow needs to wrap around at DEM_OPERATIONCYCLE_CYCLECOUNT_MAX */
        targetCycle = (uint16)(targetCycle - DEM_OPERATIONCYCLE_CYCLECOUNT_MAX);
    }
    return targetCycle;
}

#if (                                                                                                          \
    (defined(DEM_CYCLES_SINCE_FIRST_FAILED)) || (defined(DEM_CYCLES_SINCE_LAST_FAILED)) || (defined(DEM_OCC1)) \
    || (defined(DEM_OCC3)) || (defined(DEM_AGINGCTR_DOWNCNT)) || (defined(DEM_AGINGCTR_UPCNT_FIRST_ACTIVE))    \
    || (defined(DEM_AGINGCTR_UPCNT)))
/**
 * @brief         Calculates the distance between cycle counter values
 */
DEM_LOCAL uint16 Dem_CycleCountDistance(uint16 CurrentCounter, uint16 TargetCounter)
{
    uint16 ret;
    if (TargetCounter < CurrentCounter)
    {
        ret = (uint16)((DEM_OPERATIONCYCLE_CYCLECOUNT_MAX - CurrentCounter) + TargetCounter);
    }
    else
    {
        ret = (uint16)(TargetCounter - CurrentCounter);
    }
    return ret;
}
#endif

#if (                                                                                                          \
    (defined(DEM_CYCLES_SINCE_FIRST_FAILED)) || (defined(DEM_CYCLES_SINCE_LAST_FAILED)) || (defined(DEM_OCC1)) \
    || (defined(DEM_OCC3)))
/**
 * @brief         Latches overflowing operation cycle counters
 * @param[in]     MemoryIndex: Memory Index
 * @param[in]     CycleCounter: Cycle Counter
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_MemCheckCycleCounterLatched(Dem_NvBlockNumType MemoryIndex, uint16 CycleCounter)
{
    boolean ret = FALSE;
#if (defined(DEM_CYCLES_SINCE_FIRST_FAILED) || (defined(DEM_OCC3)))
    if ((Dem_GetCyclesSinceFirstFailedOfMemEntry(MemoryIndex) < DEM_OPERATIONCYCLE_CYCLECOUNT_MAX)
        && (Dem_CycleCountDistance(Dem_GetCyclesSinceFirstFailedOfMemEntry(MemoryIndex), CycleCounter)
            >= DEM_OPERATION_CYCLE_COUNTER_MAX))
    {
        Dem_SetCyclesSinceFirstFailedOfMemEntry(MemoryIndex, DEM_OPERATIONCYCLE_CYCLECOUNT_LATCHED);
        ret = TRUE;
    }
#endif
#if ((defined(DEM_CYCLES_SINCE_LAST_FAILED)) || (defined(DEM_OCC1)))
    if ((Dem_GetCyclesSinceLastFailedOfMemEntry(MemoryIndex) < DEM_OPERATIONCYCLE_CYCLECOUNT_MAX)
        && (Dem_CycleCountDistance(Dem_GetCyclesSinceLastFailedOfMemEntry(MemoryIndex), CycleCounter)
            >= DEM_OPERATION_CYCLE_COUNTER_MAX))
    {
        Dem_SetCyclesSinceLastFailedOfMemEntry(MemoryIndex, DEM_OPERATIONCYCLE_CYCLECOUNT_LATCHED);
        ret = TRUE;
    }
#endif
    return ret;
}
#endif

/**
 * @brief         Processes cycle update for stored entries.
 * This function processes the aging cycle updates for all events currently stored in the event memory. This has effect
 * only on events using the given aging cycle, when their aging  condition is fulfilled for this cycle. This function
 * also processes data updates on operation cycle change.
 */
DEM_LOCAL void Dem_OperationCycleStored(uint8 OperationCycleId, uint16 CycleCounter)
{
    Dem_NvBlockNumType memoryIndex;
    Dem_NvBlockNumType MemoryIndexEnd;
    /** Iterate through the primary indices - the chronological index becomes invalid in case an event ages */
    MemoryIndexEnd = DEM_NV_ADMINBLOCKINDEX;
    for (memoryIndex = DEM_NV_PRIMARYBLOCKINDEX; memoryIndex < MemoryIndexEnd; ++memoryIndex)
    {
        Dem_EventIdType eventId = Dem_GetEventIdOfMemEntry(memoryIndex);
        if (Dem_CheckEventIdInValid(eventId) == FALSE)
        {
#if (DEM_AGING_ENABLE == STD_ON)
            if (Dem_GetEventAgingCycle(eventId) == OperationCycleId)
            {
                Dem_DTCAgingCycle(eventId, CycleCounter, memoryIndex);
            }
#endif
#if (                                                                                                          \
    (defined(DEM_CYCLES_SINCE_FIRST_FAILED)) || (defined(DEM_CYCLES_SINCE_LAST_FAILED)) || (defined(DEM_OCC1)) \
    || (defined(DEM_OCC3)))
            if (Dem_GetOCRefOfEvent(eventId) == OperationCycleId)
            {
                if (Dem_MemCheckCycleCounterLatched(memoryIndex, CycleCounter) == TRUE)
                {
#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
                    Dem_NvmSetSingleBlockState(memoryIndex, DEM_NVM_BLOCKSTATE_DIRTY);
#endif
                }
            }
#endif
#if (                                                                                                             \
    (!defined(DEM_CYCLES_SINCE_FIRST_FAILED)) && (!defined(DEM_CYCLES_SINCE_LAST_FAILED)) && (!defined(DEM_OCC1)) \
    && (!defined(DEM_OCC3)) && (DEM_AGING_ENABLE == STD_OFF))
            DEM_PARAM_UNUSED(CycleCounter);
            DEM_PARAM_UNUSED(OperationCycleId);
#endif
        }
    }
}

/**
 * @brief         Processes global cycle counter and aging cycles
 * @param[in]     OperationCycleId: Identification of operation cycle, like power cycle, driving cycle.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_OperationCycleEnd(uint8 OperationCycleId)
{
    /** Tick the cycle if necessary */
    uint16 cycleCounter = Dem_GetCycleCounter(OperationCycleId);
    cycleCounter        = Dem_AddCycleCount(cycleCounter, 1U);
    Dem_SetCycleCounter(OperationCycleId, cycleCounter);

    /** Aging cycle start */
    Dem_OperationCycleStored(OperationCycleId, cycleCounter);
}

/**
 * @brief         Checks if passed event is affected by cycle update
 */
DEM_LOCAL boolean Dem_UpdateCheckEventAffected(Dem_EventIdType EventId, uint8 OperationCycleId)
{
    boolean ret = TRUE;
    if (Dem_GetOCRefOfEvent(EventId) != OperationCycleId)
    {
        /** Event needs to depend on the modified cycle */
        ret = FALSE;
    }

    if (Dem_GetAvailableInVariantByEvent(EventId) == FALSE)
    {
        /** Don't process unavailable DTCs */
        ret = FALSE;
    }

    if (Dem_GetEventAvailableStatusByEventId(EventId) == FALSE)
    {
        /** Don't process unavailable DTCs */
        ret = FALSE;
    }

    return ret;
}

/**
 * @brief         Processes the operation cycle update for the passed event
 * @param[in]     EventId: Event Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_UpdateProcessEvent(Dem_EventIdType EventId)
{
    Dem_UdsStatusByteType oldDtcStatus;
    Dem_UdsStatusByteType newDtcStatus;

    oldDtcStatus = Dem_GetDTCUDSStatus(EventId);
    oldDtcStatus = Dem_DTCApplyExternalStatus(EventId, oldDtcStatus);
    Dem_DTCOperationCycle(EventId);
    newDtcStatus = Dem_GetDTCUDSStatus(EventId);
#if (defined(DEM_OCC6))
    /** The counter shall be cleared in the beginning of the operation cycle following an operation cycle where FDC10
     * did not reach the value +127 at any time but -128 (the counter shall maintain its value if did not reach the
     * value -128).  */
    if ((Dem_UDSStatusCheckTFTOC(oldDtcStatus) == FALSE) && (Dem_UDSStatusCheckTNCTOC(oldDtcStatus) == FALSE))
    {
        Dem_DTCAttRefNumType lDTCAttr = Dem_GetDTCAttr(EventId);
        if (Dem_CheckDTCAttr(lDTCAttr) == TRUE)
        {
            if (Dem_GetEventstored(EventId) == TRUE)
            {
                Dem_MemStateInfoConstPtrType memoryInfo  = Dem_MemStateInfoInit(Dem_GetMemRef(lDTCAttr));
                Dem_NvBlockNumType           memoryIndex = Dem_MemoryFindIndex(memoryInfo, EventId);
                Dem_SetOCC6OfMemEntry(memoryIndex, 0u);
#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
                Dem_NvmSetSingleBlockState(memoryIndex, DEM_NVM_BLOCKSTATE_DIRTY);
#endif
            }
        }
    }
#endif
    Dem_NotiDtcStatusChanged(EventId, oldDtcStatus, Dem_DTCApplyExternalStatus(EventId, newDtcStatus));
}

/**
 * @brief         Processes operation cycle updates
 * This function processes the operation cycle updates for all events using that cycle as operation cycle.
 * @param[in]     OperationCycleId: Identification of operation cycle, like power cycle, driving cycle.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_OperationCycleReadiness(uint8 OperationCycleId)
{
    for (Dem_EventIdType eventId = DEM_EVENT_FIRST; eventId < Dem_GetEventNumber(); ++eventId)
    {
        if (Dem_UpdateCheckEventAffected(eventId, OperationCycleId) == TRUE)
        {
            Dem_UpdateProcessEvent(eventId);
        }
    }
}

/**
 * @brief         Process queued operation cycle changes
 * @param[in]     OperationCycleId: Identification of operation cycle, like power cycle, driving cycle.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_OperationCycleRestart(uint8 OperationCycleId)
{
    Dem_OperationCycleEnd(OperationCycleId);
    /** Operation Cycle start / end */
    Dem_OperationCycleReadiness(OperationCycleId);
}

/**
 * @brief         Process queued operation cycle changes
 * @param[in]     RestartFlag: Flag of restart
 * @param[in]     OperationCycleId: Identification of operation cycle, like power cycle, driving cycle.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_OperationCycleProcess(uint8 RestartFlag, uint8 OperationCycleId)
{
    uint8 cycleId = OperationCycleId;
    uint8 flag    = RestartFlag;
    while ((flag != 0u) && (cycleId < DEM_OPERATION_CYCLE_NUMBER))
    {
        if ((flag & 0x1u) != 0u)
        {
            Dem_OperationCycleRestart(cycleId);
        }
        ++cycleId;
        flag = flag >> 1u;
    }
}

/**
 * @brief Process all queued operation cycle changes
 */
DEM_LOCAL void Dem_OperationCycleTask(void)
{
    uint8 operationCycleId = 0u;
    for (uint8 restartFlagSize = 0u; restartFlagSize < DEM_SIZE_OPERATION_CYCLE_NUMBER; ++restartFlagSize)
    {
        if (Dem_GetOCRestartFlag()[restartFlagSize] != 0u)
        {
            uint8 restartFlag;

            /** Enter Critical Section: StateManager */
            Dem_EnterCritical_StateManager();
            restartFlag                             = Dem_GetOCRestartFlag()[restartFlagSize];
            Dem_GetOCRestartFlag()[restartFlagSize] = 0u;
            Dem_LeaveCritical_StateManager();
            /** Leave Critical Section: StateManager */

            Dem_OperationCycleProcess(restartFlag, operationCycleId);
        }
        operationCycleId += DEM_OPERATION_CYCLE_BYTE_SIZE;
    }
}

/**
 * @brief         Initializes the OperationCycle
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_OperationCycleInit(void)
{
    Dem_EnableTask(Dem_Task_OperationCycle);
}

/**
 * @brief         Sets an operation cycle state.
 * @param[in]     OperationCycleId: Identification of operation cycle, like power cycle, driving cycle.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_OperationCycleSetReStart(uint8 OperationCycleId)
{
    DEM_BITS_SET(Dem_GetOCRestartFlag(), OperationCycleId);
    Dem_EnableTaskOnce(Dem_Task_OperationCycle);
}

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
