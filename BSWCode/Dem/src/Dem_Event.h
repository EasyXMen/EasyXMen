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
 **  @file               : Dem_Event.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : Implementation file of Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_EVENT_H
#define DEM_EVENT_H

/* =================================================== inclusions =================================================== */
#include "Dem_InitState.h"
#include "Dem_EventInterface.h"
#include "Dem_DtcInterface.h"
#include "Dem_MemInterface.h"
#if (DEM_INDICATOR_NUMBER > 0u)
#include "Dem_Indicator.h"
#endif
#include "Dem_ExtendedData.h"
#include "Dem_SatelliteInterface.h"

/* ===================================================== macros ===================================================== */
/**
 * @name  DEM_UDS_STATUS_ENCODE
 * @{
 * @brief Macros encoding status masks to reset UDS status bits
 */
#define DEM_UDS_STATUS_TF_MASK     DEM_BIT_MASK(0, DEM_1BYTE) /**< reset test failed mask */
#define DEM_UDS_STATUS_TFTOC_MASK  DEM_BIT_MASK(1, DEM_1BYTE) /**< reset test failed this operating cycle mask */
#define DEM_UDS_STATUS_PDTC_MASK   DEM_BIT_MASK(2, DEM_1BYTE) /**< reset pending dtc mask */
#define DEM_UDS_STATUS_CDTC_MASK   DEM_BIT_MASK(3, DEM_1BYTE) /**< reset confirmed dtc mask */
#define DEM_UDS_STATUS_TNCSLC_MASK DEM_BIT_MASK(4, DEM_1BYTE) /**< reset test not completed since last clear mask */
#define DEM_UDS_STATUS_TFSLC_MASK  DEM_BIT_MASK(5, DEM_1BYTE) /**< reset test failed since last clear mask */
#define DEM_UDS_STATUS_TNCTOC_MASK                                                                                 \
    DEM_BIT_MASK(6, DEM_1BYTE)                             /**< reset test not completed this operating cycle mask \
                                                            */
#define DEM_UDS_STATUS_WIR_MASK DEM_BIT_MASK(7, DEM_1BYTE) /**< reset warning indicator requested mask */

#define DEM_PRIORITY_INVALID        (0xFFu) /**< invalid priority */
#define DEM_HEALING_COUNTER_INVALID (0xFFu) /**< invalid healing Counter */
#define DEM_FAILURE_COUNTER_MASK    (0xFFu) /**< Failure Counter mask */
/** @} */

/* ========================================== internal function declarations ======================================== */
/**
 * @brief         For aging events, reset in the UDS status the relevant bits and perform or start healing.
 * @param[in]     EventId: Event ID
 * @param[in]     EventStatus: Event status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Dem_UdsStatusByteType
    Dem_EventInitStatusAgingEvent(Dem_EventIdType EventId, Dem_UdsStatusByteType EventStatus);

/**
 * @brief         Initializes the event status of a given event
 * @param[in]     EventId: Event ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_InitEventStatus(Dem_EventIdType EventId);

/**
 * @brief         Event Status Changed notification
 * @param[in]     EventId: Event ID
 * @param[in]     EventUdsStatusOld: Old UDS status
 * @param[in]     EventUdsStatusNew: new UDS status
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_NotiEventStatusChanged(
    Dem_EventIdType       EventId,
    Dem_UdsStatusByteType EventUdsStatusOld,
    Dem_UdsStatusByteType EventUdsStatusNew);

/**
 * @brief         Tests whether an event may age
 * @param[in]     UdsStatus: UDS status
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_CheckEventMayAge(Dem_UdsStatusByteType UdsStatus);

#if (DEM_AGING_ENABLE == STD_ON)
/**
 * @brief         Updates an event due to aging
 * @param[in]     EventId: Event ID
 * @param[in]     ArgPtr: Pointer to argument
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_EventAged(Dem_EventIdType EventId, const Dem_CombinedArgPtrType ArgPtr);
#endif

/**
 * @brief         Soft filter when pass,if TF/TNCLSC or TNCTOC is flaged then no deal with uds status
 * @param[in]     EventContext: The event context
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_EventNeedPassedCheck(const Dem_EventContextType* const EventContext);

#if (DEM_INDICATOR_NUMBER > 0u)
/**
 * @brief         Process event healing for events with healing target 0.
 * @param[in]     EventContext: The event context
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_EventPassedProcessHealing(Dem_EventContextType* EventContext);
#endif

/**
 * @brief         Calculate storage depending event triggers on 'Failed' status report.
 * @param[in]     EventContext: The event context
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_EventCalculateFailedEffectsOnStorage(Dem_EventContextType* EventContext);

/**
 * @brief         Update of event's storage dependent UDS status bits on 'Failed' status report
 * @param[in]     EventContext: The event context
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_EventCalculateStatusOnFailedOnStorage(Dem_EventContextType* EventContext);

/**
 * @brief         Process DTC triggers on 'Failed' status report.
 * @param[in]     DTCContext: The DTC context
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DTCProcessFailedEffects(const Dem_DTCContextType* DTCContext);

#if (DEM_EVENT_CONFIRMATION_THRESHOLD_ADAPTABLE == STD_ON)
/**
 * @brief         If Dem_SetEventConfirmationThresholdCounter is called and the corresponding event is already stored in
 * event memory and the new failure threshold is less or equal to the current failure counter, then  the event will get
 * confirmed. This implies, that the event related data will also be updated according their defined triggers.
 * @param[in]     EventId: Event ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_EventConfirmed(Dem_EventIdType EventId);
#endif

#if (DEM_AVAILABILITY_SUPPORT == DEM_EVENT_AVAILABILITY)
#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
#if (DEM_COMBINATION_NUMBER > 0u)
/**
 * @brief         For Stored DTCs having a combined group, test if another event is available beside the event to set
 * unavailable.
 * @param[in]     EventId: Event ID
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_TestRemainingEventforDtc(Dem_EventIdType EventId);
#endif
#endif

/**
 * @brief         Marks an event available
 * @param[in]     EventId: Event ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_EventEnable(Dem_EventIdType EventId);

/**
 * @brief         Marks an event unavailable
 * @param[in]     EventId: Event ID
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_EventDisable(Dem_EventIdType EventId);

/**
 * @brief         Process event availability changes
 * @param[in]     EventId: Event ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_ProcessEventAvailable(Dem_EventIdType EventId);
#endif

/**
 * @brief         Processes an event's healing conditions
 * @param[in]     EventId: Event ID
 * @param[in]     EventStatus: Event status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Dem_UdsStatusByteType Dem_EventProcessHealing(Dem_EventIdType EventId, Dem_UdsStatusByteType EventStatus);

/**
 * @brief         Processes healing at the end of the event's operation cycle
 * @param[in]     EventId: Event ID
 * @param[in]     EventStatus: Event status
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_EventOperationCyclEndHealing(Dem_EventIdType EventId, Dem_UdsStatusByteType EventStatus);

/**
 * @brief         Processes the start of the event's operation cycle
 * @param[in]     EventId: Event ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_EventOperationCyclStart(Dem_EventIdType EventId);

/**
 * @brief         Updates an event's state due to operation cycle change
 * @param[in]     EventId: Event ID
 * @param[in]     ArgPtr: Pointer to argument
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_EventOperationCycle(Dem_EventIdType EventId, const Dem_CombinedArgPtrType ArgPtr);

/* ========================================== internal function definitions ========================================= */
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Get the number of all events, including invalid events #0
 * @return        Dem_EventIdType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventIdType Dem_GetEventNumber(void)
{
    return (Dem_EventIdType)DEM_EVENT_NUMBER;
}

/**
 * @brief         This function tests whether a handle is in the valid range of EventIds. This function does not
 * consider calibration or runtime-availability.
 * @param[in]     EventId: Event ID
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckEventIdInValid(Dem_EventIdType EventId)
{
    boolean ret = ((EventId == DEM_EVENT_INVALID) || (EventId >= Dem_GetEventNumber())) ? TRUE : FALSE;
    return ret;
}

/**
 * @brief         Get DemEventAvailableInVariant information based on event ID
 * @param[in]     EventId: Event ID
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_GetAvailableInVariantByEvent(Dem_EventIdType EventId)
{
    return Dem_GlobalConfigPtr->PBEvent[EventId].EventAvailableInVariant;
}

/**
 * @brief         Get DemEventAvailable information based on event ID
 * @param[in]     EventId: Event ID
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_GetAvailableByEvent(Dem_EventIdType EventId)
{
    return Dem_GlobalConfigPtr->PBEvent[EventId].EventAvailable;
}

#if (defined(DEM_EVENT_ASSOCIATED_IDENTIFICATION))
/**
 * @brief         Get DemEventAssociatedIdentification information based on event ID
 * @param[in]     EventId: Event ID
 * @return        Dem_EventIdType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventIdType Dem_GetAssociatedIdByEvent(Dem_EventIdType EventId)
{
    return Dem_GlobalConfigPtr->PBEvent[EventId].EventAssociatedIdentification;
}
#endif

/**
 * @brief         Get DemEventConfirmationThreshold information based on event ID
 * @param[in]     EventId: Event ID
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetConfirmationThresholdByEvent(Dem_EventIdType EventId)
{
    return Dem_GlobalConfigPtr->PBEvent[EventId].EventConfirmationThreshold;
}

/**
 * @brief         Get Event Confirmation Threshold
 * @param[in]     EventId: Event ID
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetEventConfirmationThreshold(Dem_EventIdType EventId)
{
#if (DEM_EVENT_CONFIRMATION_THRESHOLD_ADAPTABLE == STD_ON)
    return Dem_EventConfirmationThreshold[EventId];
#else
    return Dem_GetConfirmationThresholdByEvent(EventId);
#endif
}

#if (DEM_AGING_ENABLE == STD_ON)
/**
 * @brief         Get Event Aging Cycle
 * @param[in]     EventId: Event ID
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetEventAgingCycle(Dem_EventIdType EventId)
{
    return Dem_GetAgingCycleRefOfDTCAttr(Dem_GetAttrRefOfEvent(EventId));
}

/**
 * @brief         Get Event Aging Threshold
 * @param[in]     EventId: Event ID
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetEventAgingThreshold(Dem_EventIdType EventId)
{
    return Dem_GetAgeThrOfDTCAttr(Dem_GetAttrRefOfEvent(EventId));
}

#if (DEM_STATUS_BIT_HANDLING_TEST_FAILED_SINCE_LAST_CLEAR == STD_ON)
/**
 * @brief         Get Event Aging Threshold TFSLC
 * @param[in]     EventId: Event ID
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetEventAgingThresholdTFSLC(Dem_EventIdType EventId)
{
    return Dem_GetAgeThrTFSLCOfDTCAttr(Dem_GetAttrRefOfEvent(EventId));
}
#endif
#endif

#if (DEM_AGING_ENABLE == STD_ON)
/**
 * @brief         Get Event max Aging Threshold
 * @param[in]     EventId: Event ID
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetEventMaxAgingThreshold(Dem_EventIdType EventId)
{
#if (DEM_STATUS_BIT_HANDLING_TEST_FAILED_SINCE_LAST_CLEAR == STD_ON)
    uint8 ageThr      = Dem_GetEventAgingThreshold(EventId);
    uint8 ageThrTFSLC = Dem_GetEventAgingThresholdTFSLC(EventId);
    return ((ageThr >= ageThrTFSLC) ? ageThr : ageThrTFSLC);
#else
    return Dem_GetEventAgingThreshold(EventId);
#endif
}
#endif

#if (DEM_EVENT_CONFIRMATION_THRESHOLD_ADAPTABLE == STD_ON)
/**
 * @brief         Set Event Confirmation Threshold
 * @param[in]     EventId: Event ID
 * @param[in]     ConfirmationThreshold: Event confirmation threshold
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetEventConfirmationThreshold(Dem_EventIdType EventId, uint8 ConfirmationThreshold)
{
    Dem_EventConfirmationThreshold[EventId] = ConfirmationThreshold;
}
#endif

/* PRQA S 4404 ++ */ /* VL_Dem_4404 */
/**
 * @brief         check if DemEventRecoverableInSameOperationCycle is enable and FTOC is set based on event ID
 * @param[in]     EventId: Event ID
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckLatchedTestFailed(Dem_EventIdType EventId)
{
    /** If the configuration parameter DemEventRecoverableInSameOperationCycle is set to FALSE, a report of PASSED
     * (directly via Dem_SetEventStatus or indirectly after Debouncing) will be ignored if the event status byte has the
     * TestFailedThisOperationCycle flag is set to 1. */
    return (
        (Dem_UDSStatusCheckTFTOC(Dem_GetEventUDSStatus(EventId)) == TRUE)
        && (Dem_GlobalConfigPtr->PBEvent[EventId].EventRecoverableInSameOperationCycle == FALSE));
}

/**
 * @brief         check if DemEventReportingType is STANDARD_REPORTING_WITH_MONITOR_DATA based on event ID
 * @param[in]     EventId: Event ID
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckEventReportingType(Dem_EventIdType EventId)
{
    return DEM_FLAG_ISSET(Dem_GetCfgFlagOfEvent(EventId), Dem_EventReportingTypePos);
}

/**
 * @brief         check if DemEventConfirmationThresholdCounterAdaptable is enable based on event ID
 * @param[in]     EventId: Event ID
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckEventThrAdapt(Dem_EventIdType EventId)
{
    return DEM_FLAG_ISSET(Dem_GetCfgFlagOfEvent(EventId), Dem_EventCDThrCounterAdaptPos);
}

#if (DEM_INDICATOR_NUMBER > 0u)
/**
 * @brief         check if all DemIndicatorHealingCycleCounterThreshold are equal to 0 based on event ID
 * @param[in]     EventId: Event ID
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckEventAllHealingequal0(Dem_EventIdType EventId)
{
    return DEM_FLAG_ISSET(Dem_GetCfgFlagOfEvent(EventId), Dem_EventAllHealingEqual0);
}
#endif

/**
 * @brief         check if  Event have Special Indicator based on event ID
 */
DEM_LOCAL_INLINE boolean Dem_CheckEventHaveSpecialIndicator(Dem_EventIdType EventId)
{
    return DEM_FLAG_ISSET(Dem_GetCfgFlagOfEvent(EventId), Dem_EventHaveSpecialIndicator);
}
/* PRQA S 4404 -- */

/**
 * @brief         Get the priority of the event based on event ID
 * @param[in]     EventId: Event ID
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetEventPriority(Dem_EventIdType EventId)
{
    uint8                priority = DEM_PRIORITY_INVALID;
    Dem_DTCAttRefNumType lDTCAttr = Dem_GetDTCAttr(EventId);
    if (Dem_CheckDTCAttr(lDTCAttr) == TRUE)
    {
        priority = Dem_GetPriorityOfDTCAttr(lDTCAttr);
    }
    return priority;
}

/**
 * @brief         Get event internal status based on event ID
 * @param[in]     EventId: Event ID
 * @return        Dem_EventInternalStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventInternalStatusType Dem_GetEventInternalStatusByEvent(Dem_EventIdType EventId)
{
    return Dem_EventInternalStatus[EventId];
}

/**
 * @brief         Set event internal status based on event ID
 * @param[in]     EventId: Event ID
 * @param[in]     Status: Event status
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetEventInternalStatusByEvent(Dem_EventIdType EventId, Dem_EventInternalStatusType Status)
{
    Dem_EventInternalStatus[EventId] = Status;
}

/* PRQA S 3473, 4399, 4461 ++ */ /* VL_Dem_3473, VL_Dem_4399, VL_Dem_4461 */
/**
 * @brief         Set event stored internal status based on event ID
 */
DEM_LOCAL_INLINE Dem_EventInternalStatusType
    Dem_SetEventstoredStatus(Dem_EventInternalStatusType Status, Dem_InternalStoredStatusType StoredStatus)
{
    Dem_EventInternalStatusType lStatus = Status;
    DEM_FLAG_RESET(lStatus, DEM_INTERNAL_STATUS_STORED);
    DEM_FLAG_SET(lStatus, (Dem_EventInternalStatusType)StoredStatus);
    return lStatus;
}

/**
 * @brief         Get event stored internal status based on event ID
 */
DEM_LOCAL_INLINE Dem_InternalStoredStatusType Dem_GetEventstoreStatus(Dem_EventInternalStatusType Status)
{
    Dem_EventInternalStatusType ret = Status & DEM_INTERNAL_STATUS_STORED;
    return (Dem_InternalStoredStatusType)ret; /* PRQA S 4342 */ /* VL_Dem_4342 */
}

/**
 * @brief         check event stored based on event ID
 */
DEM_LOCAL_INLINE boolean Dem_GetEventstored(Dem_EventIdType EventId)
{
    /* PRQA S 4404 ++ */ /* VL_Dem_4404 */
    return (Dem_GetEventstoreStatus(Dem_GetEventInternalStatusByEvent(EventId)) != Dem_InternalStoredStatus_None);
    /* PRQA S 4404 -- */
}

/**
 * @brief         Set event available internal status
 * @param[in]     Status: Event status
 * @return        Dem_EventInternalStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventInternalStatusType Dem_SetEventAvailableStatus(Dem_EventInternalStatusType Status)
{
    Dem_EventInternalStatusType lStatus = Status;
    DEM_FLAG_SET(lStatus, DEM_INTERNAL_STATUS_AVAILABLE);
    return lStatus;
}

/**
 * @brief         Reset event available internal status
 * @param[in]     Status: Event status
 * @return        Dem_EventInternalStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventInternalStatusType Dem_ResetEventAvailableStatus(Dem_EventInternalStatusType Status)
{
    Dem_EventInternalStatusType lStatus = Status;
    DEM_FLAG_RESET(lStatus, DEM_INTERNAL_STATUS_AVAILABLE);
    return lStatus;
}

/**
 * @brief         Get event available internal status
 * @param[in]     Status: Event status
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_GetEventAvailableStatus(Dem_EventInternalStatusType Status)
{
    return DEM_FLAG_ISSET(Status, DEM_INTERNAL_STATUS_AVAILABLE); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/**
 * @brief         Get Debounce Behavior based on event ID
 * @param[in]     EventId: Event ID
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_GetDebounceBehaviorByEvent(Dem_EventIdType EventId)
{
    return (boolean)Dem_GetBehaviorResetOfDB(Dem_GetDebounceRefOfEvent(EventId));
}

#if (DEM_AVAILABILITY_SUPPORT == DEM_EVENT_AVAILABILITY)
/**
 * @brief         Set event available change internal status
 * @param[in]     Status: Event status
 * @return        Dem_EventInternalStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventInternalStatusType Dem_SetEventAvailableChangedStatus(Dem_EventInternalStatusType Status)
{
    Dem_EventInternalStatusType lStatus = Status;
    DEM_FLAG_SET(lStatus, DEM_INTERNAL_STATUS_AVAILABLE_CHANGED);
    return lStatus;
}

/**
 * @brief         Reset event available change internal status
 * @param[in]     Status: Event status
 * @return        Dem_EventInternalStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventInternalStatusType Dem_ResetEventAvailableChangedStatus(Dem_EventInternalStatusType Status)
{
    Dem_EventInternalStatusType lStatus = Status;
    DEM_FLAG_RESET(lStatus, DEM_INTERNAL_STATUS_AVAILABLE_CHANGED);
    return lStatus;
}

/**
 * @brief         Get event available change internal status
 * @param[in]     Status: Event status
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_GetEventAvailableChangedStatus(Dem_EventInternalStatusType Status)
{
    return DEM_FLAG_ISSET(Status, DEM_INTERNAL_STATUS_AVAILABLE_CHANGED); /* PRQA S 4404 */ /* VL_Dem_4404 */
}
#endif

#if (                                                                                                     \
    (DEM_ENTRY_TRIGGER_ON_FDC_THRESHOLD == STD_ON) || (DEM_GENERAL_ED_TRIGGER_ON_FDC_THRESHOLD == STD_ON) \
    || (DEM_GENERAL_FF_TRIGGER_ON_FDC_THRESHOLD == STD_ON))
/**
 * @brief         Set event fdc max this cycle in the internal status
 * @param[in]     Status: Event status
 * @return        Dem_EventInternalStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventInternalStatusType Dem_SetEventFdcTocStatus(Dem_EventInternalStatusType Status)
{
    Dem_EventInternalStatusType lStatus = Status;
    DEM_FLAG_SET(lStatus, DEM_INTERNAL_STATUS_FDCTOC);
    return lStatus;
}

/**
 * @brief         Reset event fdc max this cycle in the internal status
 * @param[in]     Status: Event status
 * @return        Dem_EventInternalStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventInternalStatusType Dem_ResetEventFdcTocStatus(Dem_EventInternalStatusType Status)
{
    Dem_EventInternalStatusType lStatus = Status;
    DEM_FLAG_RESET(lStatus, DEM_INTERNAL_STATUS_FDCTOC);
    return lStatus;
}

/**
 * @brief         Get event fdc max this cycle in the internal status
 * @param[in]     Status: Event status
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_GetEventFdcTocStatus(Dem_EventInternalStatusType Status)
{
    return DEM_FLAG_ISSET(Status, DEM_INTERNAL_STATUS_FDCTOC); /* PRQA S 4404 */ /* VL_Dem_4404 */
}
#endif

/**
 * @brief         Get event available internal status based on event ID
 * @param[in]     EventId: Event ID
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_GetEventAvailableStatusByEventId(Dem_EventIdType EventId)
{
    return Dem_GetEventAvailableStatus(Dem_GetEventInternalStatusByEvent(EventId));
}

#if (DEM_SUPPRESSION_SUPPORT == DEM_DTC_SUPPRESSION)
/**
 * @brief         Set event Suppressed status
 * @param[in]     Status: Event status
 * @return        Dem_EventInternalStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventInternalStatusType Dem_SetEventSuppressed(Dem_EventInternalStatusType Status)
{
    Dem_EventInternalStatusType lStatus = Status;
    DEM_FLAG_SET(lStatus, DEM_INTERNAL_STATUS_SUPPRESSED);
    return lStatus;
}

/**
 * @brief         Reset event Suppressed status
 * @param[in]     Status: Event status
 * @return        Dem_EventInternalStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventInternalStatusType Dem_ResetEventSuppressed(Dem_EventInternalStatusType Status)
{
    Dem_EventInternalStatusType lStatus = Status;
    DEM_FLAG_RESET(lStatus, DEM_INTERNAL_STATUS_SUPPRESSED);
    return lStatus;
}

/**
 * @brief         Get event Suppressed status
 * @param[in]     Status: Event status
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckEventSuppressed(Dem_EventInternalStatusType Status)
{
    return DEM_FLAG_ISSET(Status, DEM_INTERNAL_STATUS_SUPPRESSED); /* PRQA S 4404 */ /* VL_Dem_4404 */
}
#endif

#if (DEM_COMBINATION_NUMBER > 0u)
/**
 * @brief         Set Combined event Variant status
 * @param[in]     Status: Event status
 * @return        Dem_EventInternalStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventInternalStatusType Dem_SetCombinedEventVariant(Dem_EventInternalStatusType Status)
{
    Dem_EventInternalStatusType lStatus = Status;
    DEM_FLAG_SET(lStatus, DEM_INTERNAL_STATUS_AVAILVARIANT);
    return lStatus;
}

/**
 * @brief         Get Combined event Variant status
 * @param[in]     Status: Event status
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckCombinedEventVariant(Dem_EventInternalStatusType Status)
{
    return DEM_FLAG_ISSET(Status, DEM_INTERNAL_STATUS_AVAILVARIANT);
}
#endif

/**
 * @brief         Set event user controlled warning indicator request internal status based on event ID
 * @param[in]     Status: Event status
 * @return        Dem_EventInternalStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventInternalStatusType Dem_SetEventWirActiveStatus(Dem_EventInternalStatusType Status)
{
    Dem_EventInternalStatusType lStatus = Status;
    DEM_FLAG_SET(lStatus, DEM_INTERNAL_STATUS_WIRCONTROLL);
    return lStatus;
}

/**
 * @brief         Reset event user controlled warning indicator request internal status based on event ID
 * @param[in]     Status: Event status
 * @return        Dem_EventInternalStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventInternalStatusType Dem_ResetEventWirActiveStatus(Dem_EventInternalStatusType Status)
{
    Dem_EventInternalStatusType lStatus = Status;
    DEM_FLAG_RESET(lStatus, DEM_INTERNAL_STATUS_WIRCONTROLL);
    return lStatus;
}

/**
 * @brief         Get event user controlled warning indicator request internal status based on event ID
 * @param[in]     EventId: Event ID
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_GetEventWirActiveStatus(Dem_EventIdType EventId)
{
    Dem_EventInternalStatusType lStatus = Dem_GetEventInternalStatusByEvent(EventId);
    return DEM_FLAG_ISSET(lStatus, DEM_INTERNAL_STATUS_WIRCONTROLL); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

#if (defined(DEM_EVENT_MONOTOR_DEBOUNCCE))
/**
 * @brief         Get event Get Fault Detection Counter fnc based on event ID
 */
DEM_LOCAL_INLINE DemGetFaultDetectionCounterType Dem_GetDebounceGetFDC(Dem_EventIdType EventId)
{
#if (DEM_EVENT_CALLBACK_GET_FDC > 0u)
    DemGetFaultDetectionCounterType fnc               = (DemGetFaultDetectionCounterType)NULL_PTR;
    Dem_DebounceGetFDCNumType       debounceGetFDCRef = Dem_GetDebounceGetFDCRefOfEvent(EventId);
    if (DEM_EVENT_CALLBACK_GET_FDC != debounceGetFDCRef)
    {
        fnc = Dem_GetGetFDCFncOfEvent(debounceGetFDCRef);
    }
#else
    const DemGetFaultDetectionCounterType fnc = (DemGetFaultDetectionCounterType)NULL_PTR;
    DEM_PARAM_UNUSED(EventId);
#endif
    return fnc;
}
#endif

/**
 * @brief         Pre-initialization event state
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_EventPreInit(void)
{
    for (Dem_EventIdType eventId = DEM_EVENT_FIRST; eventId < Dem_GetEventNumber(); ++eventId)
    {
        Dem_EventInternalStatusType status = 0u;
        if (Dem_GetAvailableByEvent(eventId) == TRUE)
        {
            status = Dem_SetEventAvailableStatus(status);
        }
        Dem_SetEventInternalStatusByEvent(eventId, status);
#if (DEM_EVENT_CONFIRMATION_THRESHOLD_ADAPTABLE == STD_ON)
        Dem_SetEventConfirmationThreshold(eventId, 0u);
#endif
    }
}

/**
 * @brief         Initializes the internal event status of a given event
 * @param[in]     EventId: Event ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_InitEventInternalStatus(Dem_EventIdType EventId)
{
    Dem_EventInternalStatusType status = Dem_GetEventInternalStatusByEvent(EventId);
    if ((Dem_GetAvailableByEvent(EventId) == TRUE)
#if (DEM_AVAILABILITY_SUPPORT == DEM_EVENT_AVAILABILITY)
        && (Dem_EventMayDisable(EventId) == FALSE)
#endif
    )
    {
        status = Dem_SetEventAvailableStatus(status);
    }
    Dem_SetEventInternalStatusByEvent(EventId, status);
}

/**
 * @brief         Initializes the internal event status of Stored Bit
 * @param[in]     EventStatus: Event status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_EventInitStatusResetStoredBits(Dem_UdsStatusByteType EventStatus)
{
    Dem_UdsStatusByteType newStatus = EventStatus;
    /** Note: this relies on the compiler to optimize the different bit operations into a single mask operation.
     * The intention is to find a dedicated access for each Bit modification for review purposes. */
#if (DEM_RESET_PENDING_BIT_ON_OVERFLOW == STD_ON)
    newStatus = Dem_UDSStatusResetPDTC(newStatus);
#endif
#if (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_ON)
    newStatus = Dem_UDSStatusResetCDTC(newStatus);
#endif
#if (DEM_STATUS_BIT_HANDLING_TEST_FAILED_SINCE_LAST_CLEAR == STD_ON)
    newStatus = Dem_UDSStatusResetTFSLC(newStatus);
#endif
    return newStatus;
}

/**
 * @brief For aging events, reset in the UDS status the relevant bits and perform or start healing.
 */
DEM_LOCAL Dem_UdsStatusByteType
    Dem_EventInitStatusAgingEvent(Dem_EventIdType EventId, Dem_UdsStatusByteType EventStatus)
{
    Dem_UdsStatusByteType newStatus = EventStatus;
    if (Dem_UDSStatusCheckPDTC(EventStatus) == TRUE)
    {
        newStatus = Dem_UDSStatusResetPDTC(newStatus);
#if (DEM_INDICATOR_NUMBER > 0u)
        if (Dem_CheckEventAllHealingequal0(EventId) == TRUE)
        {
            if (Dem_UDSStatusCheckWIR(EventStatus) == TRUE)
            {
                newStatus = Dem_UDSStatusResetWIR(newStatus);
            }
        }
#endif
    }
#if (DEM_INDICATOR_NUMBER > 0u)
#if (DEM_EVENT_HEALING_COUNTER == STD_ON)
    if (Dem_UDSStatusCheckPDTC(EventStatus) == TRUE)
    {
        if ((Dem_CheckEventAllHealingequal0(EventId) == FALSE)
            || ((Dem_CheckEventAllHealingequal0(EventId) == TRUE) && (Dem_UDSStatusCheckWIR(EventStatus) == TRUE)))
        {
            Dem_SetEventHealingCounter(EventId, 0u);
        }
    }
#endif
#endif
    return newStatus;
}

/**
 * @brief Initializes the event status of a given event
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
DEM_LOCAL void Dem_InitEventStatus(Dem_EventIdType EventId)
{
    Dem_UdsStatusByteType eventStatus = Dem_GetEventUDSStatus(EventId);
    if (Dem_GetAvailableInVariantByEvent(EventId) == FALSE)
    {
        eventStatus = 0x00u;
    }
    else
    {
        switch (Dem_GetEventstoreStatus(Dem_GetDtcInterStatus(EventId)))
        {
        case Dem_InternalStoredStatus_None:
            eventStatus = Dem_EventInitStatusResetStoredBits(eventStatus);
            break;
        case Dem_InternalStoredStatus_Aging:
            eventStatus = Dem_EventInitStatusAgingEvent(EventId, eventStatus);
            break;
        default:
            /** Mirsa C */
            break;
        }
    }

#if (DEM_STATUS_BIT_STORAGE_TEST_FAILED == STD_OFF)
    eventStatus = Dem_UDSStatusResetTF(eventStatus);
#endif

    if ((eventStatus & (DEM_UDS_STATUS_TFTOC | DEM_UDS_STATUS_TNCTOC))
        == (DEM_UDS_STATUS_TFTOC | DEM_UDS_STATUS_TNCTOC))
    {
        /** Test not completed, but failed this cycle is not a consistent state.
         *  Reset TFTOC to restore consistency. This also makes sure a new test result is processed */
        eventStatus = Dem_UDSStatusResetTFTOC(eventStatus);
    }

    if ((eventStatus & (DEM_UDS_STATUS_TFSLC | DEM_UDS_STATUS_TNCSLC))
        == (DEM_UDS_STATUS_TFSLC | DEM_UDS_STATUS_TNCSLC))
    {
        /** Test not completed, but failed since last clear is not a consistent state.
         *  Reset TNCSLC to restore consistency. Keep TFSLC since it is sometimes set by correction code */
        eventStatus = Dem_UDSStatusResetTNCSLC(eventStatus);
    }

#if (DEM_INDICATOR_NUMBER > 0u)
    if (Dem_CheckHaveIndicator(EventId) == FALSE)
    {
        eventStatus = Dem_UDSStatusResetWIR(eventStatus);
    }
#endif

    Dem_SetEventUDSStatus(EventId, eventStatus);

#if (DEM_INDICATOR_NUMBER > 0u)
    if (Dem_UDSStatusCheckWIR(eventStatus) == TRUE)
    {
        (void)Dem_EventCheckWIROnFulfilled(EventId);
    }
#endif
}
/* PRQA S 6070 -- */

/**
 * @brief         initialization event state
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_EventInit(void)
{
    for (Dem_EventIdType eventId = DEM_EVENT_FIRST; eventId < Dem_GetEventNumber(); ++eventId)
    {
        Dem_InitEventInternalStatus(eventId);
        Dem_InitEventStatus(eventId);
#if (DEM_EVENT_CONFIRMATION_THRESHOLD_ADAPTABLE == STD_ON)
        if (Dem_GetEventConfirmationThreshold(eventId) == 0u)
        {
            Dem_SetEventConfirmationThreshold(eventId, Dem_GetConfirmationThresholdByEvent(eventId));
        }
#endif
    }
#if (DEM_INDICATOR_NUMBER > 0u)
    Dem_InitIndicatorStates();
#endif
}

/******************UDS Status*******************/
/**
 * @brief         Check whether the TF bit of the UDS status bit is set or not
 * @param[in]     EventStatus: Event status
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_UDSStatusCheckTF(Dem_UdsStatusByteType Status)
{
    return DEM_FLAG_ISSET(Status, DEM_UDS_STATUS_TF); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/**
 * @brief         Set the TF bit of the UDS status bit
 * @param[in]     EventStatus: Event status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_UDSStatusSetTF(Dem_UdsStatusByteType Status)
{
    Dem_UdsStatusByteType lStatus = Status;
    DEM_FLAG_SET(lStatus, DEM_UDS_STATUS_TF);
    return lStatus;
}

/**
 * @brief         Reset the TF bit of the UDS status bit
 * @param[in]     Status: UDS status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_UDSStatusResetTF(Dem_UdsStatusByteType Status)
{
    Dem_UdsStatusByteType lStatus = Status;
    DEM_FLAG_RESET(lStatus, DEM_UDS_STATUS_TF);
    return lStatus;
}

/**
 * @brief         Check whether the TFTOC bit of the UDS status bit is set or not
 * @param[in]     Status: UDS status
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_UDSStatusCheckTFTOC(Dem_UdsStatusByteType Status)
{
    return DEM_FLAG_ISSET(Status, DEM_UDS_STATUS_TFTOC); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/**
 * @brief         Set the TFTOC bit of the UDS status bit
 * @param[in]     Status: UDS status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_UDSStatusSetTFTOC(Dem_UdsStatusByteType Status)
{
    Dem_UdsStatusByteType lStatus = Status;
    DEM_FLAG_SET(lStatus, DEM_UDS_STATUS_TFTOC);
    return lStatus;
}

/**
 * @brief         Reset the TFTOC bit of the UDS status bit
 * @param[in]     Status: UDS status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_UDSStatusResetTFTOC(Dem_UdsStatusByteType Status)
{
    Dem_UdsStatusByteType lStatus = Status;
    DEM_FLAG_RESET(lStatus, DEM_UDS_STATUS_TFTOC);
    return lStatus;
}

/**
 * @brief         Check whether the PDTC bit of the UDS status bit is set or not
 * @param[in]     Status: UDS status
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_UDSStatusCheckPDTC(Dem_UdsStatusByteType Status)
{
    return DEM_FLAG_ISSET(Status, DEM_UDS_STATUS_PDTC); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/**
 * @brief         Set the PDTC bit of the UDS status bit
 * @param[in]     Status: UDS status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_UDSStatusSetPDTC(Dem_UdsStatusByteType Status)
{
    Dem_UdsStatusByteType lStatus = Status;
    DEM_FLAG_SET(lStatus, DEM_UDS_STATUS_PDTC);
    return lStatus;
}

/**
 * @brief         Reset the PDTC bit of the UDS status bit
 * @param[in]     Status: UDS status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_UDSStatusResetPDTC(Dem_UdsStatusByteType Status)
{
    Dem_UdsStatusByteType lStatus = Status;
    DEM_FLAG_RESET(lStatus, DEM_UDS_STATUS_PDTC);
    return lStatus;
}

/**
 * @brief         Check whether the CDTC bit of the UDS status bit is set or not
 * @param[in]     Status: UDS status
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_UDSStatusCheckCDTC(Dem_UdsStatusByteType Status)
{
    return DEM_FLAG_ISSET(Status, DEM_UDS_STATUS_CDTC); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/**
 * @brief         Set the CDTC bit of the UDS status bit
 * @param[in]     Status: UDS status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_UDSStatusSetCDTC(Dem_UdsStatusByteType Status)
{
    Dem_UdsStatusByteType lStatus = Status;
    DEM_FLAG_SET(lStatus, DEM_UDS_STATUS_CDTC);
    return lStatus;
}

/**
 * @brief         Reset the CDTC bit of the UDS status bit
 * @param[in]     Status: UDS status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_UDSStatusResetCDTC(Dem_UdsStatusByteType Status)
{
    Dem_UdsStatusByteType lStatus = Status;
    DEM_FLAG_RESET(lStatus, DEM_UDS_STATUS_CDTC);
    return lStatus;
}

/**
 * @brief         Check whether the TNCSLC bit of the UDS status bit is set or not
 * @param[in]     Status: UDS status
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_UDSStatusCheckTNCSLC(Dem_UdsStatusByteType Status)
{
    return DEM_FLAG_ISSET(Status, DEM_UDS_STATUS_TNCSLC); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/**
 * @brief         Set the TNCSLC bit of the UDS status bit
 * @param[in]     Status: UDS status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_UDSStatusSetTNCSLC(Dem_UdsStatusByteType Status)
{
    Dem_UdsStatusByteType lStatus = Status;
    DEM_FLAG_SET(lStatus, DEM_UDS_STATUS_TNCSLC);
    return lStatus;
}

/**
 * @brief         Reset the TNCSLC bit of the UDS status bit
 * @param[in]     Status: UDS status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_UDSStatusResetTNCSLC(Dem_UdsStatusByteType Status)
{
    Dem_UdsStatusByteType lStatus = Status;
    DEM_FLAG_RESET(lStatus, DEM_UDS_STATUS_TNCSLC);
    return lStatus;
}

/**
 * @brief         Check whether the TFSLC bit of the UDS status bit is set or not
 * @param[in]     Status: UDS status
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_UDSStatusCheckTFSLC(Dem_UdsStatusByteType Status)
{
    return DEM_FLAG_ISSET(Status, DEM_UDS_STATUS_TFSLC); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/**
 * @brief         Set the TFSLC bit of the UDS status bit
 * @param[in]     Status: UDS status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_UDSStatusSetTFSLC(Dem_UdsStatusByteType Status)
{
    Dem_UdsStatusByteType lStatus = Status;
    DEM_FLAG_SET(lStatus, DEM_UDS_STATUS_TFSLC);
    return lStatus;
}

/**
 * @brief         Reset the TFSLC bit of the UDS status bit
 * @param[in]     Status: UDS status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_UDSStatusResetTFSLC(Dem_UdsStatusByteType Status)
{
    Dem_UdsStatusByteType lStatus = Status;
    DEM_FLAG_RESET(lStatus, DEM_UDS_STATUS_TFSLC);
    return lStatus;
}

/**
 * @brief         Check whether the TNCTOC bit of the UDS status bit is set or not
 * @param[in]     Status: UDS status
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_UDSStatusCheckTNCTOC(Dem_UdsStatusByteType Status)
{
    return DEM_FLAG_ISSET(Status, DEM_UDS_STATUS_TNCTOC); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/**
 * @brief         Set the TNCTOC bit of the UDS status bit
 * @param[in]     Status: UDS status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_UDSStatusSetTNCTOC(Dem_UdsStatusByteType Status)
{
    Dem_UdsStatusByteType lStatus = Status;
    DEM_FLAG_SET(lStatus, DEM_UDS_STATUS_TNCTOC);
    return lStatus;
}

/**
 * @brief         Reset the TNCTOC bit of the UDS status bit
 * @param[in]     Status: UDS status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_UDSStatusResetTNCTOC(Dem_UdsStatusByteType Status)
{
    Dem_UdsStatusByteType lStatus = Status;
    DEM_FLAG_RESET(lStatus, DEM_UDS_STATUS_TNCTOC);
    return lStatus;
}

/**
 * @brief         Check whether the WIR bit of the UDS status bit is set or not
 * @param[in]     Status: UDS status
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_UDSStatusCheckWIR(Dem_UdsStatusByteType Status)
{
    return DEM_FLAG_ISSET(Status, DEM_UDS_STATUS_WIR); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/**
 * @brief         Set the WIR bit of the UDS status bit
 * @param[in]     Status: UDS status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_UDSStatusSetWIR(Dem_UdsStatusByteType Status)
{
    Dem_UdsStatusByteType lStatus = Status;
    DEM_FLAG_SET(lStatus, DEM_UDS_STATUS_WIR);
    return lStatus;
}

/**
 * @brief         Reset the WIR bit of the UDS status bit
 * @param[in]     Status: UDS status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_UDSStatusResetWIR(Dem_UdsStatusByteType Status)
{
    Dem_UdsStatusByteType lStatus = Status;
    DEM_FLAG_RESET(lStatus, DEM_UDS_STATUS_WIR);
    return lStatus;
}

#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
/**
 * @brief         check Event Status Block need to store
 * @param[in]     Status: UDS status
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean
    Dem_CheckNvRelevantStatus(Dem_UdsStatusByteType OldEventStatus, Dem_UdsStatusByteType NewEventStatus)
{
    /* PRQA S 4404 ++ */ /* VL_Dem_4404 */
#if (DEM_STATUS_BIT_STORAGE_TEST_FAILED == STD_ON)
    return (OldEventStatus != NewEventStatus);
#else
    /** Ignore TF bit */
    return (((OldEventStatus ^ NewEventStatus) & DEM_UDS_STATUS_TF_MASK) != 0);
#endif
    /* PRQA S 4404 -- */
}
#endif

/**************Dem_EventStatusData******************/
/**
 * @brief         Get First Failed Event
 */
DEM_LOCAL_INLINE Dem_EventIdType Dem_GetFirstFailedEvent(uint8 ClientId)
{
    return Dem_GetEventStatusData().FirstFailedEvent[ClientId];
}

/**
 * @brief         Set First Failed Event
 */
DEM_LOCAL_INLINE void Dem_SetFirstFailedEvent(Dem_EventIdType EventId, uint8 ClientId)
{
    Dem_GetEventStatusData().FirstFailedEvent[ClientId] = EventId;
}

/**
 * @brief         Get First Confirmed Event
 */
DEM_LOCAL_INLINE Dem_EventIdType Dem_GetFirstConfirmedEvent(uint8 ClientId)
{
    return Dem_GetEventStatusData().FirstConfirmedEvent[ClientId];
}

/**
 * @brief         Set First Confirmed Event
 */
DEM_LOCAL_INLINE void Dem_SetFirstConfirmedEvent(Dem_EventIdType EventId, uint8 ClientId)
{
    Dem_GetEventStatusData().FirstConfirmedEvent[ClientId] = EventId;
}

/**
 * @brief         Get Most Recent Failed Event
 */
DEM_LOCAL_INLINE Dem_EventIdType Dem_GetMostRecentFailedEvent(uint8 ClientId)
{
    return Dem_GetEventStatusData().MostRecentFailedEvent[ClientId];
}

/**
 * @brief         Set Most Recent Failed Event
 */
DEM_LOCAL_INLINE void Dem_SetMostRecentFailedEvent(Dem_EventIdType EventId, uint8 ClientId)
{
    Dem_GetEventStatusData().MostRecentFailedEvent[ClientId] = EventId;
}

/**
 * @brief         Get Most Recent Confmd Event
 */
DEM_LOCAL_INLINE Dem_EventIdType Dem_GetMostRecentConfirmdEvent(uint8 ClientId)
{
    return Dem_GetEventStatusData().MostRecentConfmdEvent[ClientId];
}

/**
 * @brief         Set Most Recent Confmd Event
 */
DEM_LOCAL_INLINE void Dem_SetMostRecentConfirmdEvent(Dem_EventIdType EventId, uint8 ClientId)
{
    Dem_GetEventStatusData().MostRecentConfmdEvent[ClientId] = EventId;
}

/**
 * @brief         Get Event UDS Status
 * @param[in]     ClientId: Client Id
 * @return        Dem_EventIdType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_GetEventUDSStatus(Dem_EventIdType EventId)
{
    return Dem_GetEventStatusData().EventStatus[EventId];
}

/**
 * @brief         Set Event UDS Status
 */
DEM_LOCAL_INLINE void Dem_SetEventUDSStatus(Dem_EventIdType EventId, Dem_UdsStatusByteType Status)
{
    Dem_GetEventStatusData().EventStatus[EventId] = Status;
}

#if (DEM_COMBINATION_NUMBER > 0u)
/**
 * @brief         Get Combined Event UDS Status
 * @param[in]     GroupId: Group Id
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_GetCombinedEventUDSStatus(Dem_CombinationNumType GroupId)
{
    return Dem_GetEventStatusData().CombinedEventStatus[GroupId];
}

/**
 * @brief         Set Combined Event UDS Status
 * @param[in]     GroupId: Group Id
 * @param[in]     Status: UDS status
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetCombinedEventUDSStatus(Dem_CombinationNumType GroupId, Dem_UdsStatusByteType Status)
{
    Dem_GetEventStatusData().CombinedEventStatus[GroupId] = Status;
}
#endif

#if (DEM_EVENT_HEALING_COUNTER == STD_ON)
/**
 * @brief         Get Event Healing Counter
 * @param[in]     EventId: Event ID
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetEventHealingCounter(Dem_EventIdType EventId)
{
    return Dem_GetEventStatusData().HealingCounter[EventId];
}

/**
 * @brief         Set Event Healing Counter
 * @param[in]     EventId: Event ID
 * @param[in]     HealingCounter: Healing counter
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetEventHealingCounter(Dem_EventIdType EventId, uint8 HealingCounter)
{
    Dem_GetEventStatusData().HealingCounter[EventId] = HealingCounter;
}
#endif

#if (DEM_EVENT_FAILURE_COUNTER == STD_ON)
/**
 * @brief         Get Event Failure Counter
 * @param[in]     EventId: Event ID
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetEventFailureCounter(Dem_EventIdType EventId)
{
    return Dem_GetEventStatusData().FailureCounter[EventId];
}

/**
 * @brief         Set Event Failure Counter
 * @param[in]     EventId: Event ID
 * @param[in]     FailureCounter: Failure counter
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetEventFailureCounter(Dem_EventIdType EventId, uint8 FailureCounter)
{
    Dem_GetEventStatusData().FailureCounter[EventId] = FailureCounter;
}
#endif

/******************Combined event*******************/
#if (DEM_COMBINATION_NUMBER > 0u)

/**
 * @brief         Get Combined event internal status based on Group ID
 */
DEM_LOCAL_INLINE Dem_EventInternalStatusType Dem_GetCombinedEventStatusByGroupId(Dem_CombinationNumType GroupId)
{
    return Dem_CombinedEventStatus[GroupId];
}

/**
 * @brief         Set Combined event internal status based on Group ID
 * @param[in]     GroupId: Group ID
 * @param[in]     Status: Event status
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void
    Dem_SetCombinedEventStatusByGroupId(Dem_CombinationNumType GroupId, Dem_EventInternalStatusType Status)
{
    Dem_CombinedEventStatus[GroupId] = Status;
}

/**
 * @brief Pre-initialization Combined event
 */
DEM_LOCAL void Dem_CombinedEventPreInit(void)
{
    for (Dem_CombinationNumType groupId = 0; groupId < DEM_COMBINATION_NUMBER; ++groupId)
    {
        Dem_EventInternalStatusType internalStatus = Dem_ResetEventAvailableStatus(0);
        for (Dem_ComDTCEventRefNumType start = Dem_GetStartRefOfCombinationDTC(groupId);
             start < Dem_GetEndRefOfCombinationDTC(groupId);
             ++start)
        {
            Dem_EventIdType eventId = Dem_GetventIdOfCombinationDTC(start);
            if (Dem_GetAvailableInVariantByEvent(eventId) == TRUE)
            {
                internalStatus = Dem_SetCombinedEventVariant(internalStatus);
            }
            if (Dem_GetAvailableByEvent(eventId) == TRUE)
            {
                internalStatus = Dem_SetEventAvailableStatus(internalStatus);
            }
        }
        Dem_SetCombinedEventStatusByGroupId(groupId, internalStatus);
    }
}

/**
 * @brief initialization Combined event
 */
DEM_LOCAL void Dem_CombinedEventInit(void)
{
    for (Dem_CombinationNumType groupId = 0; groupId < DEM_COMBINATION_NUMBER; ++groupId)
    {
        Dem_EventInternalStatusType internalStatus = Dem_GetCombinedEventStatusByGroupId(groupId);
        internalStatus                             = Dem_ResetEventAvailableStatus(internalStatus);
        for (Dem_ComDTCEventRefNumType start = Dem_GetStartRefOfCombinationDTC(groupId);
             start < Dem_GetEndRefOfCombinationDTC(groupId);
             ++start)
        {
            if (Dem_GetAvailableByEvent(Dem_GetventIdOfCombinationDTC(start)) == TRUE)
            {
                internalStatus = Dem_SetEventAvailableStatus(internalStatus);
            }
        }
        Dem_SetCombinedEventStatusByGroupId(groupId, internalStatus);

#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
        Dem_SetCombinedEventUDSStatus(groupId, Dem_CalculateCombineUdsStatus(groupId));
#endif
    }
}
#endif

/**
 * @brief         initialization event context
 * @param[in]     EventId: Event ID
 * @param[in]     EventContext: The event context
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_InitEventContext(Dem_EventIdType EventId, Dem_EventContextType* const EventContext)
{
    EventContext->EventId        = EventId;
    EventContext->OldEventStatus = Dem_GetEventUDSStatus(EventId);
    EventContext->NewEventStatus = EventContext->OldEventStatus;
    EventContext->DataModified   = FALSE;
    EventContext->Eventtriggers  = DEM_TRIGGER_NONE;
}

/**
 * @brief Event Status Changed notification
 * @param[in]     EventId: Event ID
 * @param[in]     EventUdsStatusOld: Old UDS status
 * @param[in]     EventUdsStatusNew: Old UDS status
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_NotiEventStatusChanged(
    Dem_EventIdType       EventId,
    Dem_UdsStatusByteType EventUdsStatusOld,
    Dem_UdsStatusByteType EventUdsStatusNew)
{
#if ((DEM_GENERAL_INTERFACE_SUPPORT == STD_ON) || (DEM_EVENT_CALLBACK_EVENT_UDS_STATUS_CHANGED > 0u))
    if (EventUdsStatusOld != EventUdsStatusNew)
#else
    DEM_PARAM_UNUSED(EventUdsStatusOld);
    DEM_PARAM_UNUSED(EventUdsStatusNew);
#endif
    {
#if (DEM_EVENT_CALLBACK_EVENT_UDS_STATUS_CHANGED > 0u)
        /** Call all configured status change callbacks for this event */
        for (Dem_UdsStatusChangedFncNumType start = Dem_GetEUSCRefStartOfEvent(EventId);
             start < Dem_GetEUSCRefEndOfEvent(EventId);
             ++start)
        {
            const DemEventUdsStatusChangedType fnc = Dem_GetEventUdsStatusChangedFncOfEvent(start);
            if (fnc != NULL_PTR)
            {
                (void)(fnc(EventUdsStatusOld, EventUdsStatusNew));
            }
        }
#else
        DEM_PARAM_UNUSED(EventId);
#endif

#if (DEM_GENERAL_INTERFACE_SUPPORT == STD_ON)
        /** Call general event status changed callback */
        if (Dem_GeneralTriggerOnEventUdsStatus != NULL_PTR)
        {
            (void)Dem_GeneralTriggerOnEventUdsStatus(EventId, EventUdsStatusOld, EventUdsStatusNew);
        }
#endif
    }
}

/**
 * @brief Event Data Changed notification
 */
DEM_LOCAL void Dem_NotiEventDataChanged(Dem_EventIdType EventId)
{
#if (DEM_EVENT_CALLBACK_DATA_CHANGED > 0u)
    Dem_DataChangedFncNumType lEDCRef = Dem_GetEDCRefOfEvent(EventId);
    if (lEDCRef != DEM_EVENT_CALLBACK_DATA_CHANGED)
    {
        const DemTriggerOnEventDataType fnc = Dem_GetTriggerOnEventDataFncOfEvent(lEDCRef);
        if (fnc != NULL_PTR)
        {
            (void)fnc();
        }
    }
#endif

#if (DEM_GENERAL_INTERFACE_SUPPORT == STD_ON)
    /** Call general event data changed callback */
    if (Dem_GeneralTriggerOnEventData != NULL_PTR)
    {
        (void)Dem_GeneralTriggerOnEventData(EventId);
    }
#endif
#if ((DEM_GENERAL_INTERFACE_SUPPORT == STD_OFF) && (DEM_EVENT_CALLBACK_DATA_CHANGED == 0u))
    DEM_PARAM_UNUSED(EventId);
#endif
}

/**
 * @brief  Calls all configured 'init monitor for event' callbacks.
 */
DEM_LOCAL void Dem_NotiInitMonitorForEvent(Dem_EventIdType EventId, Dem_InitMonitorReasonType InitReason)
{
#if (DEM_EVENT_CALLBACK_INIT_MFORE > 0u)
    Dem_InitMForEFncNumType lIMFERef = Dem_GetIMFERefOfEvent(EventId);
    if (DEM_EVENT_CALLBACK_INIT_MFORE != lIMFERef)
    {
        const DemInitMonitorForType fnc = Dem_GetInitMonitorForFncOfEvent(lIMFERef);
        if (fnc != NULL_PTR)
        {
            (void)fnc(InitReason);
        }
    }
#else
    DEM_PARAM_UNUSED(EventId);
    DEM_PARAM_UNUSED(InitReason);
#endif
}

/**
 * @brief Tests whether an event may age
 */
DEM_LOCAL boolean Dem_CheckEventMayAge(Dem_UdsStatusByteType UdsStatus)
{
    boolean ret = TRUE;
    if (Dem_UDSStatusCheckPDTC(UdsStatus) == TRUE)
    {
        ret = FALSE;
    }
    else
#if (DEM_INDICATOR_NUMBER > 0u)
        if (Dem_UDSStatusCheckWIR(UdsStatus) == TRUE)
    {
        ret = FALSE;
    }
    else
#endif
#if (DEM_AGING_REQUIRES_TESTED_CYCLE == STD_ON)
        if (Dem_UDSStatusCheckTNCTOC(UdsStatus) == TRUE)
    {
        ret = FALSE;
    }
    else
#endif
#if (DEM_AGING_REQUIRES_NOT_FAILED_CYCLE == STD_ON)
        if (Dem_UDSStatusCheckTFTOC(UdsStatus) == TRUE)
    {
        ret = FALSE;
    }
    else
#endif
    {
        /** MirSa C*/
    }
    return ret;
}

/**
 * @brief Reset the Uds Status bit of the event
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
DEM_LOCAL void Dem_EventResetUdsStatus(Dem_EventIdType EventId, Dem_UdsStatusByteType UdsStatus)
{
    Dem_EventContextType eventContext;
    Dem_DTCContextType   lDTCContext;

    Dem_InitEventContext(EventId, &eventContext);
    Dem_InitDTCContext(EventId, &lDTCContext);

    eventContext.NewEventStatus = eventContext.OldEventStatus;
    DEM_FLAG_RESET(eventContext.NewEventStatus, (uint32)UdsStatus);
    Dem_SetEventUDSStatus(EventId, eventContext.NewEventStatus);
    Dem_CalculateDTCStatus(&lDTCContext);

#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
    Dem_CheckEventStatusBlock(eventContext.OldEventStatus, eventContext.NewEventStatus);
#endif

    Dem_NotiEventStatusChanged(EventId, eventContext.OldEventStatus, eventContext.NewEventStatus);

    Dem_NotiDtcStatusChanged(
        EventId,
        lDTCContext.OldDtcStatus,
        Dem_DTCApplyExternalStatus(EventId, lDTCContext.NewDtcStatus));
}
/* PRQA S 3473, 4399, 4461 -- */
/* PRQA S 6070 -- */
#if (DEM_EVENT_CALLBACK_CLEAR_ALLOWED > 0u)
/**
 * @brief If the out-parameter Allowed of the callback DemClearEventAllowed returns false and the configuration
 * parameter DemClearEventAllowedBehavior is set to DEM_ONLY_THIS_CYCLE_AND_READINESS, the related UDS status bits 1
 * (TestFailedThisOperationCycle), 4 (TestNotCompletedSinceLastClear), 5 (TestFailedSinceLastClear), and 6
 * (TestNotCompletedThisOperationCycle) shall be reset.
 */
DEM_LOCAL void Dem_EventClearReset(Dem_EventIdType EventId)
{
    Dem_EventContextType eventContext;
    Dem_DTCContextType   lDTCContext;

    Dem_InitEventContext(EventId, &eventContext);
    Dem_InitDTCContext(EventId, &lDTCContext);

    eventContext.NewEventStatus = Dem_UDSStatusResetTFTOC(eventContext.OldEventStatus);
    eventContext.NewEventStatus = Dem_UDSStatusSetTNCSLC(eventContext.NewEventStatus);
    eventContext.NewEventStatus = Dem_UDSStatusResetTFSLC(eventContext.NewEventStatus);
    eventContext.NewEventStatus = Dem_UDSStatusSetTNCTOC(eventContext.NewEventStatus);
    Dem_SetEventUDSStatus(EventId, eventContext.NewEventStatus);
    Dem_CalculateDTCStatus(&lDTCContext);

#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
    Dem_CheckEventStatusBlock(eventContext.OldEventStatus, eventContext.NewEventStatus);
#endif

    Dem_NotiEventStatusChanged(EventId, eventContext.OldEventStatus, eventContext.NewEventStatus);

    Dem_NotiDtcStatusChanged(
        EventId,
        lDTCContext.OldDtcStatus,
        Dem_DTCApplyExternalStatus(EventId, lDTCContext.NewDtcStatus));
}
#endif

/**
 * @brief processes the stored triggers from debounce values passing the FDC trip threshold.
 */
DEM_LOCAL void Dem_EventProcessFdc(Dem_EventIdType EventId, boolean FdcThrExceeded, boolean FdcMaximumExceeded)
{
#if (                                                                                         \
    (defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (DEM_ENTRY_TRIGGER_ON_FDC_THRESHOLD == STD_ON) \
    || (DEM_GENERAL_ED_TRIGGER_ON_FDC_THRESHOLD == STD_ON) || (DEM_GENERAL_FF_TRIGGER_ON_FDC_THRESHOLD == STD_ON))
#if (                                                                                                     \
    (DEM_ENTRY_TRIGGER_ON_FDC_THRESHOLD == STD_ON) || (DEM_GENERAL_ED_TRIGGER_ON_FDC_THRESHOLD == STD_ON) \
    || (DEM_GENERAL_FF_TRIGGER_ON_FDC_THRESHOLD == STD_ON))
    Dem_EventInternalStatusType oldEventInternalStatus;
    Dem_EventInternalStatusType newEventInternalStatus;
    /**  Enter Critical Section: DiagMonitor */
    Dem_EnterCritical_DiagMonitor();
    oldEventInternalStatus = Dem_GetEventInternalStatusByEvent(EventId);
    newEventInternalStatus = oldEventInternalStatus;
    if (FdcThrExceeded == TRUE)
    {
        newEventInternalStatus = Dem_SetEventFdcTocStatus(newEventInternalStatus);
    }
    Dem_SetEventInternalStatusByEvent(EventId, newEventInternalStatus);
    Dem_LeaveCritical_DiagMonitor();
    /**  Leave Critical Section: DiagMonitor */
#endif

    if ((Dem_GetEventDTC(EventId) != DEM_DTC_UDS_INVALID) /* PRQA S 1252 */ /* VL_Dem_1252 */
        /** Don't store with disabled storage conditions */
        && (Dem_CheckSCFulfilled(EventId) == TRUE))
    {
#if (                                                                                                     \
    (DEM_ENTRY_TRIGGER_ON_FDC_THRESHOLD == STD_ON) || (DEM_GENERAL_ED_TRIGGER_ON_FDC_THRESHOLD == STD_ON) \
    || (DEM_GENERAL_FF_TRIGGER_ON_FDC_THRESHOLD == STD_ON))
        if (FdcThrExceeded == TRUE)
        {
            Dem_DTCFdcTrip(EventId, (boolean)(Dem_GetEventFdcTocStatus(oldEventInternalStatus) == FALSE));
        }
#endif
#if (defined(DEM_MAX_FDC_SINCE_LAST_CLEAR))
        if (FdcMaximumExceeded == TRUE)
        {
            Dem_DTCUpdateFdcMax(EventId);
        }
#endif
    }
#else
    DEM_PARAM_UNUSED(EventId);
    DEM_PARAM_UNUSED(FdcThrExceeded);
    DEM_PARAM_UNUSED(FdcMaximumExceeded);
#endif
}

#if (DEM_GENERAL_DISPLACEMENT_NONE == STD_OFF)
/**
 * @brief Updates an event's status due to displacement.Do not call this function directly, it is part of
 * Dem_DTCDisplaced
 */
/* PRQA S 3673,6070 ++ */ /* VL_QAC_3673,VL_MTR_Dem_STCAL */
DEM_LOCAL void Dem_EventDisplaced(Dem_EventIdType EventId, const Dem_CombinedArgPtrType ArgPtr)
/* PRQA S 3673,6070 -- */
{
#if (                                                                                                \
    (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_ON) || (DEM_RESET_PENDING_BIT_ON_OVERFLOW == STD_ON) \
    || (DEM_STATUS_BIT_HANDLING_TEST_FAILED_SINCE_LAST_CLEAR == STD_ON))
    Dem_UdsStatusByteType oldEventStatus = Dem_GetEventUDSStatus(EventId);
    Dem_UdsStatusByteType newEventStatus = oldEventStatus;
#endif

    /** Enter Critical Section: DiagMonitor */
    Dem_EnterCritical_DiagMonitor();
    Dem_EventInternalStatusType internalStatus = Dem_GetEventInternalStatusByEvent(EventId);
    internalStatus = Dem_SetEventstoredStatus(internalStatus, Dem_InternalStoredStatus_None);
    Dem_SetEventInternalStatusByEvent(EventId, internalStatus);
    Dem_LeaveCritical_DiagMonitor();
    /** Leave Critical Section: DiagMonitor */

    /** Update the status bit */
#if (DEM_RESET_PENDING_BIT_ON_OVERFLOW == STD_ON)
    newEventStatus = Dem_UDSStatusResetPDTC(newEventStatus);
#endif
#if (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_ON)
    newEventStatus = Dem_UDSStatusResetCDTC(newEventStatus);
#endif
#if (DEM_RESET_PENDING_BIT_ON_OVERFLOW == STD_ON)
#if (DEM_STATUS_BIT_HANDLING_TEST_FAILED_SINCE_LAST_CLEAR == STD_ON)
    newEventStatus = Dem_UDSStatusResetTFSLC(newEventStatus);
#endif
#endif

#if (                                                                                                \
    (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_ON) || (DEM_RESET_PENDING_BIT_ON_OVERFLOW == STD_ON) \
    || (DEM_STATUS_BIT_HANDLING_TEST_FAILED_SINCE_LAST_CLEAR == STD_ON))

    Dem_SetEventUDSStatus(EventId, newEventStatus);
    /** Displacement follows an event status change. No need to mark Nv modification again. */
    Dem_NotiEventStatusChanged(EventId, oldEventStatus, newEventStatus);
#endif
    DEM_PARAM_UNUSED(ArgPtr);
}
#endif

/**
 * @brief Clears an event's state in volatile RAM
 */
/* PRQA S 3673,6070 ++ */ /* VL_QAC_3673,VL_MTR_Dem_STCAL */
DEM_LOCAL void Dem_EventCleared(Dem_EventIdType EventId, const Dem_CombinedArgPtrType ArgPtr)
/* PRQA S 3673,6070 -- */
{
    Dem_EventInternalStatusType internalStatus;
    Dem_UdsStatusByteType       oldEventStatus;

    Dem_SatelliteClearEvent(EventId);

    /** Enter Critical Section: DiagMonitor */
    Dem_EnterCritical_DiagMonitor();
    internalStatus = Dem_GetEventInternalStatusByEvent(EventId);
    oldEventStatus = Dem_GetEventUDSStatus(EventId);

    internalStatus = Dem_SetEventstoredStatus(internalStatus, Dem_InternalStoredStatus_None);
#if (                                                                                                     \
    (DEM_ENTRY_TRIGGER_ON_FDC_THRESHOLD == STD_ON) || (DEM_GENERAL_ED_TRIGGER_ON_FDC_THRESHOLD == STD_ON) \
    || (DEM_GENERAL_FF_TRIGGER_ON_FDC_THRESHOLD == STD_ON))
    internalStatus = Dem_ResetEventFdcTocStatus(internalStatus);
#endif
    Dem_SetEventInternalStatusByEvent(EventId, internalStatus);
    /** Reset status byte */
    Dem_SetEventUDSStatus(EventId, DEM_UDS_STATUS_INITIALIZE);
    /** EventStatus is marked modified by the caller to reduce runtime overhead */
    Dem_LeaveCritical_DiagMonitor();
    /** Leave Critical Section: DiagMonitor */

#if (DEM_INDICATOR_NUMBER > 0u)
    if (Dem_UDSStatusCheckWIR(oldEventStatus) == TRUE)
    {
        Dem_IndicatorDisable(EventId);
    }
#endif

#if (DEM_MAX_NUMBER_PRESTORED_FF > 0u)
    if (Dem_GetPreFFIndexOfEvent(EventId) != DEM_PRESTORED_EVENT_NUM)
    {
        (void)Dem_PrestorageClear(EventId);
    }
#endif

#if (DEM_EVENT_HEALING_COUNTER == STD_ON)
    Dem_SetEventHealingCounter(EventId, 0u);
#endif

#if (DEM_EVENT_FAILURE_COUNTER == STD_ON)
    Dem_SetEventFailureCounter(EventId, 0u);
#endif

#if (defined(DEM_AGED_COUNTER))
    Dem_GetAdminData().AgedCounter[EventId] = 0u;
#endif

    Dem_NotiEventStatusChanged(EventId, oldEventStatus, DEM_UDS_STATUS_INITIALIZE);
    DEM_PARAM_UNUSED(ArgPtr);
}

#if (DEM_AGING_ENABLE == STD_ON)
/**
 * @brief Updates an event due to aging
 */
/* PRQA S 3673,6070 ++ */ /* VL_QAC_3673,VL_MTR_Dem_STCAL */
DEM_LOCAL void Dem_EventAged(Dem_EventIdType EventId, const Dem_CombinedArgPtrType ArgPtr)
/* PRQA S 3673,6070 -- */
{
    Dem_UdsStatusByteType       oldEventStatus = Dem_GetEventUDSStatus(EventId);
    Dem_UdsStatusByteType       newEventStatus = oldEventStatus;
    Dem_EventInternalStatusType internalStatus;

    /** remove the aged event from the primary memory */
    newEventStatus = Dem_UDSStatusResetCDTC(newEventStatus);
#if (DEM_STATUS_BIT_HANDLING_TEST_FAILED_SINCE_LAST_CLEAR == STD_ON)
    newEventStatus = Dem_UDSStatusResetTFSLC(newEventStatus);
#endif
    Dem_SetEventUDSStatus(EventId, newEventStatus);

    /** Enter Critical Section: DiagMonitor */
    Dem_EnterCritical_DiagMonitor();
    internalStatus = Dem_GetEventInternalStatusByEvent(EventId);
    internalStatus = Dem_SetEventstoredStatus(internalStatus, Dem_InternalStoredStatus_None);
    Dem_SetEventInternalStatusByEvent(EventId, internalStatus);
    Dem_LeaveCritical_DiagMonitor();
    /** Leave Critical Section: DiagMonitor */

#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
    Dem_CheckEventStatusBlock(oldEventStatus, newEventStatus);
#endif

#if (defined(DEM_AGED_COUNTER))
    if (Dem_GetAdminData().AgedCounter[EventId] < DEM_DATAELEMENT_COUNTER_MAX)
    {
        Dem_GetAdminData().AgedCounter[EventId]++;
    }
#endif

    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == TRUE)
    {
        Dem_NotiEventStatusChanged(EventId, oldEventStatus, newEventStatus);
    }
    DEM_PARAM_UNUSED(ArgPtr);
}
#endif

/**
 * @brief Soft filter when pass,if TF/TNCLSC or TNCTOC is flaged then no deal with uds status
 */
DEM_LOCAL boolean Dem_EventNeedPassedCheck(const Dem_EventContextType* const EventContext)
{
    boolean ret = FALSE;
    if ((Dem_UDSStatusCheckTNCTOC(EventContext->OldEventStatus) == TRUE)
        || (Dem_UDSStatusCheckTNCSLC(EventContext->OldEventStatus) == TRUE)
        || (Dem_UDSStatusCheckTF(EventContext->OldEventStatus) == TRUE))
    {
        ret = TRUE;
    }
    return ret;
}

/**
 * @brief         Update of event's UDS status bits on 'Passed' status report
 * @param[in]     EventContext: The event context
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_EventCalculateStatusOnPassed(Dem_EventContextType* EventContext)
{
    EventContext->NewEventStatus = Dem_UDSStatusResetTF(EventContext->NewEventStatus);
    EventContext->NewEventStatus = Dem_UDSStatusResetTNCSLC(EventContext->NewEventStatus);
    EventContext->NewEventStatus = Dem_UDSStatusResetTNCTOC(EventContext->NewEventStatus);
}

/* PRQA S 3473 ++ */ /* VL_Dem_3473 */
/**
 * @brief         Calculate event triggers on 'Passed' status report.
 * @param[in]     EventContext: The event context
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_EventPassedCalcEventtriggers(Dem_EventContextType* EventContext)
{
    if (Dem_UDSStatusCheckTF(EventContext->OldEventStatus) == TRUE)
    {
        DEM_FLAG_SET(EventContext->Eventtriggers, DEM_TRIGGER_TESTPASSED);
    }
}

#if (DEM_INDICATOR_NUMBER > 0u)
/**
 * @brief         Resets all warning indicator requests from the given event. This can modify the global indicator state
 * if the indicator is no longer requested at all.
 * @param[in]     EventId: Event ID
 * @param[in]     EventStatus: Event status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_EventHealed(Dem_EventIdType EventId, Dem_UdsStatusByteType EventStatus)
{
    Dem_UdsStatusByteType newEventStatus = EventStatus;
    if (Dem_UDSStatusCheckWIR(newEventStatus) == TRUE)
    {
        newEventStatus = Dem_UDSStatusResetWIR(newEventStatus);
        Dem_IndicatorDisable(EventId);
    }
    return newEventStatus;
}

/**
 * @brief Process event healing for events with healing target 0.
 */
DEM_LOCAL void Dem_EventPassedProcessHealing(Dem_EventContextType* EventContext)
{
    Dem_EventIdType       eventId        = EventContext->EventId;
    Dem_UdsStatusByteType oldEventStatus = EventContext->OldEventStatus;
    /** If an indicator is configured (via DemIndicatorAttribute) the Dem module shall set when the Event is
     * reported/qualified to PASSED the UDS status bit 7 (WarningIndicatorRequested)to 0, if the following conditions
     * are fulfilled:
     * all DemIndicatorHealingCycleCounterThreshold are equal to 0
     * WIRbit is not enabled by calling the API Dem_SetWIRStatus */
#if (DEM_INDICATOR_NUMBER > 0u)
    if (Dem_CheckEventAllHealingequal0(eventId) == TRUE)
    {
#endif
#if (DEM_EVENT_HEALING_COUNTER == STD_ON)
        if ((Dem_UDSStatusCheckPDTC(oldEventStatus) == TRUE) && (Dem_CheckHaveIndicator(eventId) == TRUE))
        {
            Dem_SetEventHealingCounter(eventId, 0u);
        }
#endif

        if (Dem_UDSStatusCheckWIR(oldEventStatus) == TRUE)
        {
            EventContext->NewEventStatus = Dem_EventHealed(eventId, EventContext->NewEventStatus);
        }
    }
}
#endif

/**
 * @brief         Process event triggers on 'Passed' status report.
 * @param[in]     EventContext: The event context
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_EventProcessPassedEffects(Dem_EventContextType* EventContext)
{
#if (DEM_INDICATOR_NUMBER > 0u)
    Dem_EventPassedProcessHealing(EventContext);
#endif

#if (DEM_MAX_NUMBER_PRESTORED_FF > 0u)
    Dem_SetPrestoreStateDiscard();
#endif
}

/**
 * @brief Process the event's queued qualified passed result
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
DEM_LOCAL void Dem_EventPassed(Dem_EventIdType EventId)
{
    if (Dem_CheckLatchedTestFailed(EventId) == FALSE)
    {
        Dem_EventContextType eventContext;
        Dem_DTCContextType   lDTCContext;

        Dem_InitEventContext(EventId, &eventContext);
        Dem_InitDTCContext(EventId, &lDTCContext);
        if (Dem_EventNeedPassedCheck(&eventContext) == TRUE)
        {
            Dem_EventCalculateStatusOnPassed(&eventContext);
            Dem_EventPassedCalcEventtriggers(&eventContext);
            Dem_EventProcessPassedEffects(&eventContext);
            Dem_SetEventUDSStatus(EventId, eventContext.NewEventStatus);
            Dem_CalculateDTCStatus(&lDTCContext);
            Dem_NotiEventStatusChanged(EventId, eventContext.OldEventStatus, eventContext.NewEventStatus);
            Dem_NotiDtcStatusChanged(
                EventId,
                lDTCContext.OldDtcStatus,
                Dem_DTCApplyExternalStatus(EventId, lDTCContext.NewDtcStatus));
            Dem_DTCPassedProcessDTCEffects(&lDTCContext);

#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
            Dem_CheckEventStatusBlock(eventContext.OldEventStatus, eventContext.NewEventStatus);
#endif
        }

#if ((DEM_GENERAL_ED_TRIGGER_ON_PASSED == STD_ON) && (DEM_EXTENDED_DATA_CLASS_NUMBER > 0u))
        Dem_ExtendedDataPassedUpdate(&eventContext);
#endif
    }
}
/* PRQA S 6070 -- */

/**
 * @brief         Update of event's UDS status bits on 'failed' status report
 * @param[in]     EventContext: The event context
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_EventCalculateStatusOnFailed(Dem_EventContextType* EventContext)
{
    /** Process the deferred 'asynchronous' status bit transitions for the event */
    EventContext->NewEventStatus = Dem_UDSStatusSetTF(EventContext->NewEventStatus);
    EventContext->NewEventStatus = Dem_UDSStatusSetTFTOC(EventContext->NewEventStatus);
    EventContext->NewEventStatus = Dem_UDSStatusSetTFSLC(EventContext->NewEventStatus);
    EventContext->NewEventStatus = Dem_UDSStatusResetTNCSLC(EventContext->NewEventStatus);
    EventContext->NewEventStatus = Dem_UDSStatusResetTNCTOC(EventContext->NewEventStatus);
    Dem_SetEventUDSStatus(EventContext->EventId, EventContext->NewEventStatus);
}

/**
 * @brief         Calculate storage independent event triggers on 'Failed' status report.
 * @param[in]     EventContext: The event context
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_EventCalculateFailedEffects(Dem_EventContextType* EventContext)
{
    /** Derive the triggers for the event from the changes of the status bits */
    if (Dem_UDSStatusCheckTF(EventContext->OldEventStatus) == FALSE)
    {
        DEM_FLAG_SET(EventContext->Eventtriggers, DEM_TRIGGER_TESTFAILED);
    }
    if (Dem_UDSStatusCheckTF(EventContext->NewEventStatus) == TRUE)
    {
        DEM_FLAG_SET(EventContext->Eventtriggers, DEM_TRIGGER_EVERYTESTFAILED);
    }
    if (Dem_UDSStatusCheckTFTOC(EventContext->OldEventStatus) == FALSE)
    {
        DEM_FLAG_SET(EventContext->Eventtriggers, DEM_TRIGGER_TESTFAILEDTHISCYCLE);
    }
}

/**
 * @brief         Calculate event has confirmed
 * @param[in]     EventContext: The event context
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_EventCheckConfirmedFulfilled(const Dem_EventContextType* const EventContext)
{
    boolean ret;
#if (DEM_EVENT_FAILURE_COUNTER == STD_ON)
    Dem_EventIdType eventId = EventContext->EventId;
    ret                     = FALSE;
    if ((Dem_GetEventFailureCounter(eventId) + 1u) >= Dem_GetEventConfirmationThreshold(eventId))
#endif
    {
        ret = TRUE;
    }
    return ret;
}

/**
 * @brief Calculate storage depending event triggers on 'Failed' status report.
 */
DEM_LOCAL void Dem_EventCalculateFailedEffectsOnStorage(Dem_EventContextType* EventContext)
{
    /** Calculate effects for 'Pending' bit */
    if (Dem_UDSStatusCheckPDTC(EventContext->OldEventStatus) == FALSE)
    {
        /** Not yet pending, set pending independently of TFTOC */
        DEM_FLAG_SET(EventContext->Eventtriggers, DEM_TRIGGER_PENDING);
    }

    if (Dem_UDSStatusCheckCDTC(EventContext->OldEventStatus) == FALSE)
    {
        /** Calculate effects for 'Confirmed' */
        if (Dem_EventCheckConfirmedFulfilled(EventContext) == TRUE)
        {
            /** Event has confirmed */
            DEM_FLAG_SET(EventContext->Eventtriggers, DEM_TRIGGER_CONFIRMED);
        }
    }

#if (DEM_INDICATOR_NUMBER > 0u)
    /** Calculate effects for 'WIR' bit */
    if (Dem_EventCheckWIROnFulfilled(EventContext->EventId) == TRUE)
    {
        /** Event has WarningIndicatorOnCriteriaFulfilled */
        if (Dem_UDSStatusCheckWIR(EventContext->OldEventStatus) == FALSE)
        {
            if (Dem_CheckHaveIndicator(EventContext->EventId) == TRUE)
            {
                DEM_FLAG_SET(EventContext->Eventtriggers, DEM_TRIGGER_INDICATOR);
            }
        }
    }
#endif
}

/**
 * @brief Update of event's storage dependent UDS status bits on 'Failed' status report
 */
DEM_LOCAL void Dem_EventCalculateStatusOnFailedOnStorage(Dem_EventContextType* EventContext)
{
    uint8 trigger = EventContext->Eventtriggers;
    if (DEM_FLAG_ISSET(trigger, DEM_TRIGGER_PENDING))
    {
        EventContext->NewEventStatus = Dem_UDSStatusSetPDTC(EventContext->NewEventStatus);
    }
    if (DEM_FLAG_ISSET(trigger, DEM_TRIGGER_CONFIRMED))
    {
        EventContext->NewEventStatus = Dem_UDSStatusSetCDTC(EventContext->NewEventStatus);
    }
    if (DEM_FLAG_ISSET(trigger, DEM_TRIGGER_INDICATOR))
    {
        EventContext->NewEventStatus = Dem_UDSStatusSetWIR(EventContext->NewEventStatus);
    }
    Dem_SetEventUDSStatus(EventContext->EventId, EventContext->NewEventStatus);
}

/**
 * @brief Process DTC triggers on 'Failed' status report.
 */
DEM_LOCAL void Dem_DTCProcessFailedEffects(const Dem_DTCContextType* DTCContext)
{
    Dem_EventInternalStatusType newInternalStatus;
    /** Enter Critical Section: DiagMonitor */
    Dem_EnterCritical_DiagMonitor();
    newInternalStatus = Dem_GetDtcInterStatus(DTCContext->EventId);
    if (Dem_GetEventstoreStatus(newInternalStatus) != Dem_InternalStoredStatus_None)
    {
        newInternalStatus = Dem_SetEventstoredStatus(newInternalStatus, Dem_InternalStoredStatus_Active);
        Dem_SetDtcInterStatus(DTCContext->EventId, newInternalStatus);
    }
    Dem_LeaveCritical_DiagMonitor();
    /** Leave Critical Section: DiagMonitor */
}

/**
 * @brief Process the event's queued qualified failed result
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
DEM_LOCAL void Dem_EventFailed(Dem_EventIdType EventId)
{
    Dem_EventContextType eventContext;
    Dem_DTCContextType   lDTCContext;

    Dem_InitEventContext(EventId, &eventContext);
    Dem_InitDTCContext(EventId, &lDTCContext);
    Dem_EventCalculateStatusOnFailed(&eventContext);
    Dem_CalculateDTCStatus(&lDTCContext);
    Dem_EventCalculateFailedEffects(&eventContext);
    Dem_DTCCalculateFailedEffects(&lDTCContext, eventContext.Eventtriggers);

    if ((eventContext.OldEventStatus != eventContext.NewEventStatus)
#if (DEM_GENERAL_FF_TRIGGER_ON_EVERY_TEST_FAILED == STD_ON)
        || (Dem_UDSStatusCheckTF(eventContext.NewEventStatus) == TRUE)
#endif
#if ((DEM_STORAGE_CONDITION_GROUP_NUMBER > 0u) && (DEM_FEATURE_RETRY_STORAGE == STD_ON))
        || (Dem_DTCCheckStoredStatus(EventId) == FALSE)
#endif
    )
    {
#if (DEM_EVENT_HEALING_COUNTER == STD_ON)
        if (Dem_UDSStatusCheckPDTC(eventContext.OldEventStatus) == TRUE)
        {
            Dem_SetEventHealingCounter(EventId, 0u);
        }
#endif
        {
            Dem_DTCAttRefNumType lDTCAttr = Dem_GetDTCAttr(EventId);
            /* PRQA S 4404 ++ */ /* VL_Dem_4404 */
            boolean storage = (Dem_CheckDTCAttr(lDTCAttr) == TRUE) && (Dem_CheckSCFulfilled(EventId) == TRUE);
            /* PRQA S 4404 -- */
            if (storage == TRUE)
            {
                Dem_EventCalculateFailedEffectsOnStorage(&eventContext);
                Dem_DTCCalculateFailedEffectsOnStorage(&lDTCContext, eventContext.Eventtriggers);

                Dem_DTCProcessStorage(&eventContext, &lDTCContext);
                Dem_EventCalculateStatusOnFailedOnStorage(&eventContext);
                Dem_CalculateDTCStatus(&lDTCContext);

                if (lDTCContext.Dtctriggers != DEM_TRIGGER_NONE)
                {
                    Dem_DTCProcessFailedEffects(&lDTCContext);
                }
                Dem_UpdateMemoryFailureStatistics(EventId, eventContext.Eventtriggers);

#if (DEM_MAX_NUMBER_PRESTORED_FF > 0u)
                Dem_SetPrestoreStateDiscard();
#endif
            }
        }
#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
        Dem_NvmSetStatusBlockChanged();
#endif
    }
    Dem_NotiEventStatusChanged(EventId, eventContext.OldEventStatus, eventContext.NewEventStatus);
    if (eventContext.DataModified == TRUE)
    {
        Dem_NotiEventDataChanged(EventId);
    }
    Dem_NotiDtcStatusChanged(
        EventId,
        lDTCContext.OldDtcStatus,
        Dem_DTCApplyExternalStatus(EventId, lDTCContext.NewDtcStatus));
}
/* PRQA S 6070 -- */

#if (DEM_EVENT_CONFIRMATION_THRESHOLD_ADAPTABLE == STD_ON)
/**
 * @brief If Dem_SetEventConfirmationThresholdCounter is called and the corresponding event is already stored in
 * event memory and the new failure threshold is less or equal to the current failure counter, then  the event will get
 * confirmed. This implies, that the event related data will also be updated according their defined triggers.
 */
DEM_LOCAL void Dem_EventConfirmed(Dem_EventIdType EventId)
{
    Dem_EventContextType eventContext;
    Dem_DTCContextType   lDTCContext;

    Dem_InitEventContext(EventId, &eventContext);
    Dem_InitDTCContext(EventId, &lDTCContext);
    if (Dem_UDSStatusCheckCDTC(eventContext.OldEventStatus) == FALSE)
    {
        /** Event has confirmed */
        DEM_FLAG_SET(eventContext.Eventtriggers, DEM_TRIGGER_CONFIRMED);
    }
    Dem_DTCCalculateFailedEffectsOnStorage(&lDTCContext, eventContext.Eventtriggers);
    Dem_DTCProcessStorage(&eventContext, &lDTCContext);
    Dem_EventCalculateStatusOnFailedOnStorage(&eventContext);
    Dem_CalculateDTCStatus(&lDTCContext);

    if (lDTCContext.Dtctriggers != DEM_TRIGGER_NONE)
    {
        Dem_DTCProcessFailedEffects(&lDTCContext);
    }
    Dem_UpdateMemoryFailureStatistics(EventId, eventContext.Eventtriggers);
#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
    Dem_NvmSetStatusBlockChanged();
#endif
    Dem_NotiEventStatusChanged(EventId, eventContext.OldEventStatus, eventContext.NewEventStatus);
    if (eventContext.DataModified == TRUE)
    {
        Dem_NotiEventDataChanged(EventId);
    }
    Dem_NotiDtcStatusChanged(
        EventId,
        lDTCContext.OldDtcStatus,
        Dem_DTCApplyExternalStatus(EventId, lDTCContext.NewDtcStatus));
}
#endif
/* PRQA S 3473 -- */

#if (DEM_AVAILABILITY_SUPPORT == DEM_EVENT_AVAILABILITY)

#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
#if (DEM_COMBINATION_NUMBER > 0u)
/**
 * @brief For Stored DTCs having a combined group, test if another event is available beside the event to set
 * unavailable.
 * */
DEM_LOCAL boolean Dem_TestRemainingEventforDtc(Dem_EventIdType EventId)
{
    boolean                ret     = FALSE;
    Dem_CombinationNumType groupId = Dem_GetGroupIdOfEvent(EventId);
    if (groupId != DEM_COMBINATION_NUMBER)
    {
        for (Dem_ComDTCEventRefNumType start = Dem_GetStartRefOfCombinationDTC(groupId);
             start < Dem_GetEndRefOfCombinationDTC(groupId);
             ++start)
        {
            Dem_EventIdType subEventId = Dem_GetventIdOfCombinationDTC(start);
            if ((subEventId != EventId)
                && (Dem_GetEventAvailableStatus(Dem_GetEventInternalStatusByEvent(subEventId)) == TRUE))
            {
                ret = TRUE;
                break;
            }
        }
    }
    return ret;
}
#endif
#endif

/**
 * @brief Check precondition for event unavailable
 */
DEM_LOCAL boolean Dem_EventMayDisable(Dem_EventIdType EventId)
{
    boolean               ret        = FALSE;
    Dem_UdsStatusByteType lUDSStatus = Dem_GetEventUDSStatus(EventId);
    /** When the API Dem_SetEventAvailable is called with availableStatus = FALSE, the Dem shall return E_NOT_OK if:
     * for the event an event memory entry already exists, or
     * any of the event status flags testFailed, pending, confirmed or warningIndicatorRequested is set */
    if ((Dem_UDSStatusCheckTF(lUDSStatus) == FALSE) && (Dem_UDSStatusCheckWIR(lUDSStatus) == FALSE)
        && (Dem_UDSStatusCheckPDTC(lUDSStatus) == FALSE) && (Dem_UDSStatusCheckCDTC(lUDSStatus) == FALSE))
    {
        if (Dem_GetEventstored(EventId) == TRUE)
        {
#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
#if (DEM_COMBINATION_NUMBER > 0u)
            ret = Dem_TestRemainingEventforDtc(EventId);
#endif
#endif
        }
        else
        {
            ret = TRUE;
        }
    }

    return ret;
}

/**
 * @brief Marks an event available
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
DEM_LOCAL void Dem_EventEnable(Dem_EventIdType EventId)
{
    Dem_EventInternalStatusType internalStatus = Dem_GetEventInternalStatusByEvent(EventId);

    if ((Dem_GetEventAvailableStatus(internalStatus) == FALSE)
        && (Dem_GetEventAvailableChangedStatus(internalStatus) == FALSE))
    {
        /** Enter Critical Section: DiagMonitor */
        Dem_EnterCritical_DiagMonitor();
        internalStatus = Dem_GetEventInternalStatusByEvent(EventId);
        internalStatus = Dem_SetEventAvailableStatus(internalStatus);
        internalStatus = Dem_SetEventAvailableChangedStatus(internalStatus);
        Dem_SetEventInternalStatusByEvent(EventId, internalStatus);
        Dem_LeaveCritical_DiagMonitor();
        /**  Leave Critical Section: DiagMonitor */

        Dem_EnableTaskOnce(Dem_Task_Event);
    }
}

/**
 * @brief Marks an event unavailable
 */
DEM_LOCAL Std_ReturnType Dem_EventDisable(Dem_EventIdType EventId)
{
    Std_ReturnType              ret = E_OK;
    Dem_EventInternalStatusType internalStatus;

    internalStatus = Dem_GetEventInternalStatusByEvent(EventId);
    if (Dem_GetEventAvailableStatus(internalStatus) == TRUE)
    {
        if (((Dem_CheckMasterInitStatus(DEM_PREINITIALIZED) == TRUE)
#if (DEM_AVAILABILITY_SUPPORT == DEM_EVENT_AVAILABILITY)
             || (Dem_EventMayDisable(EventId) == TRUE)
#endif
                 )
            && (Dem_GetEventAvailableChangedStatus(internalStatus) == FALSE))
        {
            /** Enter Critical Section: DiagMonitor */
            Dem_EnterCritical_DiagMonitor();
            internalStatus = Dem_GetEventInternalStatusByEvent(EventId);
            internalStatus = Dem_ResetEventAvailableStatus(internalStatus);
            internalStatus = Dem_SetEventAvailableChangedStatus(internalStatus);
            Dem_SetEventInternalStatusByEvent(EventId, internalStatus);
            Dem_LeaveCritical_DiagMonitor();
            /**  Leave Critical Section: DiagMonitor */

            Dem_EnableTaskOnce(Dem_Task_Event);
        }
        else
        {
            ret = E_NOT_OK;
        }
    }
    return ret;
}
/* PRQA S 6070 -- */

/**
 * @brief         Marks a normal event unavailable
 * @param[in]     EventId: Event ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_EventDisableNormal(Dem_EventIdType EventId)
{
    Dem_UdsStatusByteType oldEventStatus = Dem_GetEventUDSStatus(EventId);
#if (                                                                                                     \
    (DEM_ENTRY_TRIGGER_ON_FDC_THRESHOLD == STD_ON) || (DEM_GENERAL_ED_TRIGGER_ON_FDC_THRESHOLD == STD_ON) \
    || (DEM_GENERAL_FF_TRIGGER_ON_FDC_THRESHOLD == STD_ON))
    {
        Dem_EventInternalStatusType eventInternalStatus;

        /**  Enter Critical Section: DiagMonitor */
        Dem_EnterCritical_DiagMonitor();
        eventInternalStatus = Dem_GetEventInternalStatusByEvent(EventId);
        eventInternalStatus = Dem_ResetEventFdcTocStatus(eventInternalStatus);
        Dem_SetEventInternalStatusByEvent(EventId, eventInternalStatus);
        Dem_LeaveCritical_DiagMonitor();
        /**  Leave Critical Section: DiagMonitor */
    }
#endif
    Dem_SatelliteClearEvent(EventId);
    Dem_SetEventUDSStatus(EventId, 0x00u);
    Dem_NotiEventStatusChanged(EventId, oldEventStatus, 0x00u);
}

/**
 * @brief         Marks a normal event available
 * @param[in]     EventId: Event ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_EventEnableNormal(Dem_EventIdType EventId)
{
    Dem_UdsStatusByteType oldEventStatus = Dem_GetEventUDSStatus(EventId);
    Dem_SetEventUDSStatus(EventId, DEM_UDS_STATUS_INITIALIZE);
    Dem_NotiEventStatusChanged(EventId, oldEventStatus, DEM_UDS_STATUS_INITIALIZE);
}

/**
 * @brief Process event availability changes
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
DEM_LOCAL void Dem_ProcessEventAvailable(Dem_EventIdType EventId)
{
    if (Dem_GetEventAvailableChangedStatus(Dem_GetEventInternalStatusByEvent(EventId)) == TRUE)
    {
        Dem_EventInternalStatusType internalStatus;
        boolean                     availableStatus;

        /**  Enter Critical Section: DiagMonitor */
        Dem_EnterCritical_DiagMonitor();
        internalStatus = Dem_GetEventInternalStatusByEvent(EventId);
        internalStatus = Dem_ResetEventAvailableChangedStatus(internalStatus);
        Dem_SetEventInternalStatusByEvent(EventId, internalStatus);
        availableStatus = Dem_GetEventAvailableStatus(internalStatus);
        Dem_LeaveCritical_DiagMonitor();
        /** Leave Critical Section: DiagMonitor */

        if (availableStatus == FALSE)
        {
            /** Dtc was unavailable */
            Dem_DTCDisable(EventId);

#if (DEM_MAX_NUMBER_PRESTORED_FF > 0u)
            if (Dem_GetPreFFIndexOfEvent(EventId) != DEM_PRESTORED_EVENT_NUM)
            {
                (void)Dem_PrestorageClear(EventId);
            }
#endif
        }
        else
        {
            /** Event was available */
            Dem_DTCEnable(EventId);
            Dem_NotiInitMonitorForEvent(EventId, DEM_INIT_MONITOR_REENABLED);
        }
    }
}
/* PRQA S 6070 -- */

/**
 * @brief Process queued event availability changes
 */
DEM_LOCAL void Dem_EventTask(void)
{
    for (Dem_EventIdType eventId = DEM_EVENT_FIRST; eventId < Dem_GetEventNumber(); ++eventId)
    {
        Dem_ProcessEventAvailable(eventId);
    }
}
#endif

/**
 * @brief Processes an event's healing conditions
 */
DEM_LOCAL Dem_UdsStatusByteType Dem_EventProcessHealing(Dem_EventIdType EventId, Dem_UdsStatusByteType EventStatus)
{
    uint8 newEventStatus = EventStatus;
#if (DEM_INDICATOR_NUMBER > 0u)
#if (DEM_EVENT_HEALING_COUNTER == STD_ON)
    uint8   healingCounter = Dem_GetEventHealingCounter(EventId);
    boolean healed         = TRUE;
    if (healingCounter < DEM_HEALING_COUNTER_INVALID)
    {
        healingCounter += 1u;
        Dem_SetEventHealingCounter(EventId, healingCounter);
    }

    for (Dem_IndicatorAttrNumType start = Dem_GetIAStartOfEvent(EventId); start < Dem_GetIAEndRefEndOfEvent(EventId);
         ++start)
    {
        uint8 HThrOfInd = Dem_GetHThrOfInd(start);
        if (healingCounter < HThrOfInd)
        {
            healed = FALSE;
        }
    }
    if (healed == TRUE)
    {
        newEventStatus = Dem_EventHealed(EventId, EventStatus);
    }
#else
    if (Dem_UDSStatusCheckWIR(EventStatus) == TRUE)
    {
        /** Healing target reached -> heal all related indicators */
        newEventStatus = Dem_EventHealed(EventId, newEventStatus);
    }
#endif
#endif
    return newEventStatus;
}

/**
 * @brief Processes healing at the end of the event's operation cycle
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
DEM_LOCAL boolean Dem_EventOperationCyclEndHealing(Dem_EventIdType EventId, Dem_UdsStatusByteType EventStatus)
{
    boolean               eventMayAge    = FALSE;
    Dem_UdsStatusByteType newEventStatus = EventStatus;

    if (Dem_UDSStatusCheckTNCTOC(newEventStatus) == FALSE)
    {
        if (Dem_UDSStatusCheckTFTOC(newEventStatus) == FALSE)
        {
#if (DEM_EVENT_HEALING_COUNTER == STD_ON)
            /** Event tested this operation cycle and was not failed, reset pending state */
            if (Dem_UDSStatusCheckPDTC(newEventStatus) == TRUE)
            {
                Dem_SetEventHealingCounter(EventId, 0u);
            }
#endif
#if (DEM_EVENT_FAILURE_COUNTER == STD_ON)
            /** Dem_RestartOperationCycle[(OperationCycleId == DemOperationCycleRef) && (testFailedThisOperationCycle ==
             * 0) && (testNotCompleteThisOperationCycle == 0)***] /failure counter = 0 */
            Dem_SetEventFailureCounter(EventId, 0u);
#endif
            newEventStatus = Dem_UDSStatusResetPDTC(newEventStatus);
            newEventStatus = Dem_EventProcessHealing(EventId, newEventStatus);
            eventMayAge    = Dem_CheckEventMayAge(newEventStatus);
        }
#if (DEM_EVENT_FAILURE_COUNTER == STD_ON)
        else
        {
            if (Dem_UDSStatusCheckPDTC(newEventStatus) == TRUE)
            {
                /** test failed: increment trip counter if trip target has not yet been reached */
                Dem_SetEventFailureCounter(
                    EventId,
                    ((Dem_GetEventFailureCounter(EventId) + 1u) & DEM_FAILURE_COUNTER_MASK));
            }
        }
#endif
        Dem_SetEventUDSStatus(EventId, newEventStatus);
    }
    else
    {
#if (DEM_AGING_REQUIRES_TESTED_CYCLE == STD_OFF)
        if (Dem_UDSStatusCheckWIR(newEventStatus) == FALSE)
        {
            eventMayAge = TRUE;
        }
#endif
    }
    return eventMayAge;
}
/* PRQA S 6070 -- */

/**
 * @brief         Processes the end of the event's operation cycle
 * @param[in]     EventId: Event ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_EventOperationCyclEnd(Dem_EventIdType EventId)
{
    if (Dem_EventOperationCyclEndHealing(EventId, Dem_GetEventUDSStatus(EventId)) == TRUE)
    {
#if (DEM_AGING_ENABLE == STD_ON)
        Dem_DTCOperationCyclEndAging(EventId);
#endif
    }
}

/**
 * @brief         Processes the start of the event's operation cycle
 * @param[in]     EventId: Event ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_EventOperationCyclStart(Dem_EventIdType EventId)
{
    Dem_UdsStatusByteType newEventStatus = Dem_GetEventUDSStatus(EventId);

    newEventStatus = Dem_UDSStatusResetTFTOC(newEventStatus);
    newEventStatus = Dem_UDSStatusSetTNCTOC(newEventStatus);
    Dem_SetEventUDSStatus(EventId, newEventStatus);

    Dem_SatelliteRestartEvent(EventId);
#if (                                                                                                     \
    (DEM_ENTRY_TRIGGER_ON_FDC_THRESHOLD == STD_ON) || (DEM_GENERAL_ED_TRIGGER_ON_FDC_THRESHOLD == STD_ON) \
    || (DEM_GENERAL_FF_TRIGGER_ON_FDC_THRESHOLD == STD_ON))
    {
        Dem_EventInternalStatusType eventInternalStatus;

        /**  Enter Critical Section: DiagMonitor */
        Dem_EnterCritical_DiagMonitor();
        eventInternalStatus = Dem_GetEventInternalStatusByEvent(EventId);
        eventInternalStatus = Dem_ResetEventFdcTocStatus(eventInternalStatus);
        Dem_SetEventInternalStatusByEvent(EventId, eventInternalStatus);
        Dem_LeaveCritical_DiagMonitor();
        /**  Leave Critical Section: DiagMonitor */
    }
#endif
}

/**
 * @brief Updates an event's state due to operation cycle change
 */
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
DEM_LOCAL void Dem_EventOperationCycle(Dem_EventIdType EventId, const Dem_CombinedArgPtrType ArgPtr)
/* PRQA S 3673 -- */
{
    Dem_UdsStatusByteType oldEventStatus = Dem_GetEventUDSStatus(EventId);
    Dem_UdsStatusByteType newEventStatus;

    DEM_PARAM_UNUSED(ArgPtr);
    Dem_EventOperationCyclEnd(EventId);
    Dem_EventOperationCyclStart(EventId);

    newEventStatus = Dem_GetEventUDSStatus(EventId);

#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
    Dem_CheckEventStatusBlock(oldEventStatus, newEventStatus);
#endif

    Dem_NotiEventStatusChanged(EventId, oldEventStatus, newEventStatus);
    /** Call Init Monitor Callbacks */
    Dem_NotiInitMonitorForEvent(EventId, DEM_INIT_MONITOR_RESTART);
}

/**
 * @brief         Initializes the event status
 * @param[in]     EventId: Event ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_EventInitNvData(Dem_EventIdType EventId)
{
    Dem_SetEventUDSStatus(EventId, DEM_UDS_STATUS_INITIALIZE);

#if (DEM_EVENT_HEALING_COUNTER == STD_ON)
    Dem_SetEventHealingCounter(EventId, 0u);
#endif
#if (DEM_EVENT_FAILURE_COUNTER == STD_ON)
    Dem_SetEventFailureCounter(EventId, 0u);
#endif
}

/**
 * @brief         Gets the current event status
 * @param[in]     EventId: Event ID
 * @param[out]    EventUDSStatus: Event UDS Status
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Std_ReturnType Dem_EventGetUDSStatus(Dem_EventIdType EventId, Dem_UdsStatusByteType* EventUDSStatus)
{
    Std_ReturnType ret = E_NOT_OK;
    if ((Dem_GetAvailableInVariantByEvent(EventId) == TRUE) && (Dem_GetEventAvailableStatusByEventId(EventId) == TRUE))
    {
        *EventUDSStatus = Dem_GetEventUDSStatus(EventId);
        ret             = E_OK;
    }
    return ret;
}

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
