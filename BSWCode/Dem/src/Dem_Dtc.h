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
 **  @file               : Dem_Dtc.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : Implementation file of Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_DTC_H
#define DEM_DTC_H

/* =================================================== inclusions =================================================== */
#include "Dem_J1939.h"
#if (DEM_MAX_NUMBER_PRESTORED_FF > 0u)
#include "Dem_PreStore.h"
#endif
#include "Dem_OperationCycle.h"

/* ===================================================== macros ===================================================== */
#if (                                                                                                    \
    (DEM_OBD_CALLBACK_DTC_STATUS_CHANGED == STD_ON) || (DEM_J1939_CALLBACK_DTC_STATUS_CHANGED == STD_ON) \
    || (DEM_CLIENT_CALLBACK_DTC_STATUS_CHANGED == STD_ON))
#define DEM_CALLBACK_DTC_STATUS_CHANGED STD_ON /**< Disable DTC status changed callback */
#else
#define DEM_CALLBACK_DTC_STATUS_CHANGED STD_OFF /**< Enable DTC status changed callback */
#endif

/* ========================================== internal function declarations ======================================== */
#if (DEM_AGING_ENABLE == STD_ON)
/**
 * @brief         Starts aging of an event.In case the event already ages (or does not support aging), this function has
 * no effect.
 * @param[in]     EventId: The event ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DTCAgingStart(Dem_EventIdType EventId);
#endif

#if (DEM_AGING_ENABLE == STD_ON)
#if (DEM_STATUS_BIT_HANDLING_TEST_FAILED_SINCE_LAST_CLEAR == STD_ON)
/**
 * @brief         DemAgingCycleCounterThreshold or DemAgingCycleCounterThresholdForTFSLC (depending on which value is
 * higher) defines the number of completed aging cycles, after which the event memory entry shall be deleted(aged) from
 * the event memory.
 * @param[in]     EventId: The event ID
 * @param[out]    CycleCounter: Cycle Counter
 * @param[in]     MemoryIndex: Memory index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void
    Dem_DTCCheckAgingCycleThreshold(Dem_EventIdType EventId, uint16 CycleCounter, Dem_NvBlockNumType MemoryIndex);
#endif
#endif

#if (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_OFF)
/**
 * @brief         If configuration parameter DemResetConfirmedBitOnOverflow is set to false and an event memory entry
 * aging occurs the Dem module shall check for other events having UDS status bit 3 set to 1 and UDS status bit 0 set to
 * 0. If such an event is found, the Dem shall allocate an event memory location to get an aging counter.
 * @param[in]     MemIndex: Memory index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DTCAgedTryAgingOther(Dem_MemoryNumType MemIndex);
#endif

#if (DEM_AGING_ENABLE == STD_ON)
/**
 * @brief         Completes aging of a DTC.
 * @param[in]     EventId: The event ID
 * @param[in]     MemoryIndex: Memory index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DTCAged(Dem_EventIdType EventId, Dem_NvBlockNumType MemoryIndex);
#endif

#if (DEM_AVAILABILITY_SUPPORT == DEM_EVENT_AVAILABILITY)
#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
/**
 * @brief Marks a combined event unavailable
 * @param[in]     EventId: The event ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DTCDisableCombined(Dem_EventIdType EventId);

/**
 * @brief Marks a combined event available
 * @param[in]     EventId: The event ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DTCEnableCombined(Dem_EventIdType EventId);
#endif
#endif

/**
 * @brief         Read the DTC number for the given EventId
 * @param[in]     EventId: The event ID
 * @param[in]     DTCFormat: Defines the output-format of the requested DTC value.
 * @param[out]    DTCOfEvent: Receives the DTC value in respective format returned by this function. If the return
 *                            value of the function is other than E_OK this parameter does not contain valid data.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_GetDTCByEvent(Dem_EventIdType EventId, Dem_DTCFormatType DTCFormat, uint32* DTCOfEvent);

/**
 * @brief         Set DTC WIRS tatus
 * @param[in]     EventId: The event ID
 * @param[in]     WIRStatus: Requested status of event related WIR-bit (regarding to the current status of
 *                           function inhibition) WIRStatus = TRUE -> WIR-bit shall be set to "1" WIRStatus = FALSE ->
 *                           WIR-bit shall be set to "0"
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_DTCSetWIRStatus(Dem_EventIdType EventId, boolean WIRStatus);

/* ========================================== internal function definitions ========================================= */
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Get DemDTCAttributesRef by event
 */
DEM_LOCAL_INLINE Dem_DTCAttRefNumType Dem_GetDTCAttr(Dem_EventIdType EventId)
{
    return Dem_GetAttrRefOfEvent(EventId);
}

/**
 * @brief         Check DemDTCAttributesRef by event
 */
DEM_LOCAL_INLINE boolean Dem_CheckDTCAttr(Dem_DTCAttRefNumType DTCAttr)
{
    return (DTCAttr != DEM_DTC_ATTRIBUTES_NUMBER); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/**
 * @brief         Get Memory Destination reference
 * @param[in]     DTCAttr: The reference DTC attribute
 * @return        Dem_MemoryNumType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_MemoryNumType Dem_GetMemRef(Dem_DTCAttRefNumType DTCAttr)
{
    return Dem_GetMemRefOfDTCAttr(DTCAttr);
}

/**
 * @brief         Get Event DTC NvStorageStrategy
 */
DEM_LOCAL_INLINE boolean Dem_GetEventDTCNvImmediate(Dem_EventIdType EventId)
{
    boolean Immediate = FALSE;
#if (DEM_IMMEDIATE_STORAGE == STD_ON)
    if (Dem_GetDTCRefOfEvent(EventId) != DEM_DTC_NUMBER)
    {
        Immediate = Dem_GetStorageOfDTC(Dem_GetDTCRefOfEvent(EventId));
    }
#else
    DEM_PARAM_UNUSED(EventId);
#endif
    return Immediate;
}

#if (DEM_DTC_GROUP_NUMBER > 0u)
/**
 * @brief         Get Dem Group Of DTC
 * @param[in]     GroupId: Identification of DTC Group.
 * @return        uint32
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint32 Dem_GetDTCOfGroupDTCs(Dem_GroupOfDTCNumType GroupId)
{
    return Dem_GlobalConfigPtr->GroupOfDTC[GroupId];
}
#endif

/**
 * @brief         Get Event DTC value
 */
DEM_LOCAL_INLINE uint32 Dem_GetEventDTC(Dem_EventIdType EventId)
{
    uint32 lDtcValue = DEM_DTC_UDS_INVALID; /* PRQA S 1258 */ /* VL_Dem_1258 */
    if (Dem_GetDTCRefOfEvent(EventId) != DEM_DTC_NUMBER)
    {
        lDtcValue = Dem_GlobalConfigPtr->DtcValue[Dem_GetDTCRefOfEvent(EventId)];
    }
    return lDtcValue;
}

#if (DEM_OBDII_SUPPORT == STD_ON)
/**
 * @brief         Get Event Obd DTC value
 */
DEM_LOCAL_INLINE uint32 Dem_GetEventObdDTC(Dem_EventIdType EventId)
{
    uint32 lOBDDtcValue = DEM_DTC_OBD_INVALID;
    if (Dem_GetObdDTCRefOfEvent(EventId) != DEM_OBD_DTC_NUMBER)
    {
        lOBDDtcValue = Dem_GlobalConfigPtr->PBObdDTC[Dem_GetObdDTCRefOfEvent(EventId)].DtcValue;
    }
    return lOBDDtcValue;
}
#endif

#if (DEM_J1939_SUPPORT == STD_ON)
/**
 * @brief         Get Event J1939 DTC value
 */
DEM_LOCAL_INLINE uint32 Dem_GetEventJ1939DTC(Dem_EventIdType EventId)
{
    uint32 lJ1939DtcValue = DEM_DTC_J1939_INVALID; /* PRQA S 1258 */ /* VL_Dem_1258 */
    if (Dem_GetObdDTCRefOfEvent(EventId) != DEM_OBD_DTC_NUMBER)
    {
        lJ1939DtcValue = Dem_GlobalConfigPtr->PBObdDTC[Dem_GetObdDTCRefOfEvent(EventId)].J1939DTCValue;
    }
    return lJ1939DtcValue;
}

/**
 * @brief         Check Event J1939 Related
 */
DEM_LOCAL_INLINE boolean Dem_CheckEventJ1939Related(Dem_EventIdType EventId)
{
    /* PRQA S 4404, 1252 ++ */ /* VL_Dem_4404, VL_QAC_1252 */
    return (Dem_GetEventJ1939DTC(EventId) != DEM_J1939_DTC_INVALID_VALUE);
    /* PRQA S 4404, 1252 -- */
}
#endif

/**
 * @brief         Check whether a memory entry exists for the DTC
 */
DEM_LOCAL_INLINE boolean Dem_DTCCheckStoredStatus(Dem_EventIdType EventId)
{
    boolean ret = FALSE;
    if (Dem_GetEventstoreStatus(Dem_GetDtcInterStatus(EventId)) != Dem_InternalStoredStatus_None)
    {
        ret = TRUE;
    }
    return ret;
}

/**
 * @brief         Initialization DTC context
 */
DEM_LOCAL_INLINE void Dem_InitDTCContext(Dem_EventIdType EventId, Dem_DTCContextType* const DTCContext)
{
    DTCContext->EventId      = EventId;
    DTCContext->OldDtcStatus = Dem_GetEventUDSStatus(EventId);
#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
    DTCContext->GroupId = Dem_GetGroupIdOfEvent(EventId);
#endif
    DTCContext->Dtctriggers = DEM_TRIGGER_NONE;
}

#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
/**
 * @brief         Calculate Combine DTC Status
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_CalculateCombineUdsStatus(Dem_CombinationNumType GroupId)
{
    Dem_UdsStatusByteType udsStatus = 0;
    for (Dem_ComDTCEventRefNumType start = Dem_GetStartRefOfCombinationDTC(GroupId);
         start < Dem_GetEndRefOfCombinationDTC(GroupId);
         ++start)
    {
        Dem_EventIdType EventId = Dem_GetventIdOfCombinationDTC(start);
        if ((Dem_GetAvailableInVariantByEvent(EventId) == TRUE)
            && (Dem_GetEventAvailableStatusByEventId(EventId) == TRUE))
        {
            udsStatus |= Dem_GetEventUDSStatus(EventId);
        }
    }
    return udsStatus;
}

/**
 * @brief         Calculates the fault detection counter of a combined event
 * @param[in]     GroupId: Identification of DTC Group.
 * @param[out]    FaultDetectionCounter: This parameter receives the Fault Detection Counter information of the
 *                                       requested EventId. If the return value of the function call is other
 *                                       than E_OK this parameter does not contain valid data. -128dec...127dec
 *                                       PASSED...FAILED according to ISO 14229-1
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_DTCCombinedGroupGetFDC(Dem_CombinationNumType GroupId, sint8* FaultDetectionCounter)
{
    Std_ReturnType ret    = TRUE;
    sint8          maxFdc = -128;
    for (Dem_ComDTCEventRefNumType start = Dem_GetStartRefOfCombinationDTC(GroupId);
         start < Dem_GetEndRefOfCombinationDTC(GroupId);
         ++start)
    {
        Dem_EventIdType eventId = Dem_GetventIdOfCombinationDTC(start);
        if ((Dem_GetAvailableInVariantByEvent(eventId) == TRUE)
            && (Dem_GetEventAvailableStatusByEventId(eventId) == TRUE))
        {
            sint8 fdc;
            ret = Dem_SatelliteGetFaultDetectionCounter(eventId, &fdc);
            if ((ret == E_OK) && (maxFdc < fdc))
            {
                maxFdc = fdc;
            }
        }
        if (ret != E_OK)
        {
            break;
        }
    }
    if (ret == E_OK)
    {
        *FaultDetectionCounter = maxFdc;
    }
    return ret;
}
#endif

/**
 * @brief         Calculate DTC Status
 * @param[in]     DTCContext: The DTC context
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_CalculateDTCStatus(Dem_DTCContextType* const DTCContext)
{
#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
    if (DTCContext->GroupId != DEM_COMBINATION_NUMBER)
    {
        DTCContext->NewDtcStatus = Dem_CalculateCombineUdsStatus(DTCContext->GroupId);
        Dem_SetCombinedEventUDSStatus(DTCContext->GroupId, DTCContext->NewDtcStatus);
    }
    else
#endif
    {
        DTCContext->NewDtcStatus = Dem_GetEventUDSStatus(DTCContext->EventId);
    }
}

/**
 * @brief         Calculate DTC triggers
 * @param[in]     DTCContext: The DTC context
 * @param[in]     Eventtriggers: The event trigger type
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_DTCCalculateFailedEffects(Dem_DTCContextType* const DTCContext, uint8 Eventtriggers)
{
#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
    if (DTCContext->GroupId != DEM_COMBINATION_NUMBER)
    {
        /** Derive the triggers for the event from the changes of the status bits */
        if (Dem_UDSStatusCheckTF(DTCContext->OldDtcStatus) == FALSE)
        {
            DEM_FLAG_SET(DTCContext->Dtctriggers, DEM_TRIGGER_TESTFAILED);
        }
        if (Dem_UDSStatusCheckTFTOC(DTCContext->OldDtcStatus) == FALSE)
        {
            DEM_FLAG_SET(DTCContext->Dtctriggers, DEM_TRIGGER_TESTFAILEDTHISCYCLE);
        }
    }
    else
#endif
    {
        DTCContext->Dtctriggers = Eventtriggers;
    }
}

/**
 * @brief         Calculate storage dependent DTC triggers on 'Failed' status report.
 * @param[in]     DTCContext: The DTC context
 * @param[in]     Eventtriggers: The event trigger type
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_DTCCalculateFailedEffectsOnStorage(Dem_DTCContextType* const DTCContext, uint8 Eventtriggers)
{
#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
    if (DTCContext->GroupId != DEM_COMBINATION_NUMBER)
    {
        if (DEM_FLAG_ISSET(Eventtriggers, DEM_TRIGGER_PENDING))
        {
            if (Dem_UDSStatusCheckPDTC(DTCContext->OldDtcStatus) == FALSE)
            {
                DEM_FLAG_SET(DTCContext->Dtctriggers, DEM_TRIGGER_PENDING);
            }
        }
        if (DEM_FLAG_ISSET(Eventtriggers, DEM_TRIGGER_CONFIRMED))
        {
            if (Dem_UDSStatusCheckCDTC(DTCContext->OldDtcStatus) == FALSE)
            {
                DEM_FLAG_SET(DTCContext->Dtctriggers, DEM_TRIGGER_CONFIRMED);
            }
        }
        if (DEM_FLAG_ISSET(Eventtriggers, DEM_TRIGGER_INDICATOR))
        {
            if (Dem_UDSStatusCheckWIR(DTCContext->OldDtcStatus) == FALSE)
            {
                DEM_FLAG_SET(DTCContext->Dtctriggers, DEM_TRIGGER_INDICATOR);
            }
        }
    }
    else
#endif
    {
        DTCContext->Dtctriggers = Eventtriggers;
    }
}

/**
 * @brief         Check Combine DTC Status
 * @param[in]     DtcStatus: The DTC status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_CheckCombineUdsStatus(Dem_UdsStatusByteType DtcStatus)
{
    Dem_UdsStatusByteType newDtcStatus = DtcStatus;
#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
    if (Dem_UDSStatusCheckTFSLC(newDtcStatus) == TRUE)
    {
        newDtcStatus = Dem_UDSStatusResetTNCSLC(newDtcStatus);
    }
    if (Dem_UDSStatusCheckTFTOC(newDtcStatus) == TRUE)
    {
        newDtcStatus = Dem_UDSStatusResetTNCTOC(newDtcStatus);
    }
#endif
    return newDtcStatus;
}

/**
 * @brief         This function applies a filter to the DTC status byte to set bits that are forced to true by different
 * mechanisms than event status modification. Also, status bits not contained in the status availability mask are masked
 * out.
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType
    Dem_DTCApplyExternalStatus(Dem_EventIdType EventId, Dem_UdsStatusByteType DtcStatus)
{
    Dem_UdsStatusByteType newDtcStatus = DtcStatus;
#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
    Dem_CombinationNumType groupId = Dem_GetGroupIdOfEvent(EventId);
    if (groupId != DEM_COMBINATION_NUMBER)
    {
        boolean wirActive = FALSE;
        for (Dem_ComDTCEventRefNumType start = Dem_GetStartRefOfCombinationDTC(groupId);
             start < Dem_GetEndRefOfCombinationDTC(groupId);
             ++start)
        {
            Dem_EventIdType subEventId = Dem_GetventIdOfCombinationDTC(start);
            if ((Dem_GetAvailableInVariantByEvent(subEventId) == TRUE)
                && (Dem_GetEventAvailableStatusByEventId(subEventId) == TRUE)
                && (Dem_GetEventWirActiveStatus(subEventId) == TRUE))
            {
                wirActive = TRUE;
                break;
            }
        }
        if (wirActive == TRUE)
        {
            newDtcStatus = Dem_UDSStatusSetWIR(newDtcStatus);
        }
        newDtcStatus = Dem_CheckCombineUdsStatus(newDtcStatus);
    }
    else
#endif
    {
        if (Dem_GetEventWirActiveStatus(EventId) == TRUE)
        {
            newDtcStatus = Dem_UDSStatusSetWIR(newDtcStatus);
        }
    }
    return newDtcStatus;
}

/**
 * @brief         This function retrieves the event's internal status byte for normal events, and the group's internal
 * status byte for combined events.
 */
DEM_LOCAL_INLINE Dem_EventInternalStatusType Dem_GetDtcInterStatus(Dem_EventIdType EventId)
{
    Dem_EventInternalStatusType ret;
#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
    Dem_CombinationNumType groupId = Dem_GetGroupIdOfEvent(EventId);
    if (groupId != DEM_COMBINATION_NUMBER)
    {
        ret = Dem_GetCombinedEventStatusByGroupId(groupId);
    }
    else
#endif
    {
        ret = Dem_GetEventInternalStatusByEvent(EventId);
    }
    return ret;
}

/**
 * @brief         This function stores the event's internal status byte for normal events, and the group's internal
 * status byte for combined events.
 */
DEM_LOCAL_INLINE void Dem_SetDtcInterStatus(Dem_EventIdType EventId, Dem_EventInternalStatusType Status)
{
#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
    Dem_CombinationNumType groupId = Dem_GetGroupIdOfEvent(EventId);
    if (groupId != DEM_COMBINATION_NUMBER)
    {
        Dem_SetCombinedEventStatusByGroupId(groupId, Status);
    }
    else
#endif
    {
        Dem_SetEventInternalStatusByEvent(EventId, Status);
    }
}

/**
 * @brief         This function retrieves the event's status byte for normal events, and the combined event group's
 * status byte for combined events.
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_GetDTCUDSStatus(Dem_EventIdType EventId)
{
    Dem_UdsStatusByteType dtcStatus;
#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
    Dem_CombinationNumType groupId = Dem_GetGroupIdOfEvent(EventId);
    if (groupId != DEM_COMBINATION_NUMBER)
    {
        dtcStatus = Dem_GetCombinedEventUDSStatus(groupId);
    }
    else
#endif
    {
        dtcStatus = Dem_GetEventUDSStatus(EventId);
    }
    return dtcStatus;
}

#if (DEM_SUPPRESSION_SUPPORT == DEM_DTC_SUPPRESSION)
/**
 * @brief         Tests if an event or combined event group is suppressed for reporting API. Any event of a combined
 * group can be passed to this function.
 */
DEM_LOCAL_INLINE boolean Dem_CheckDTCSuppression(Dem_EventIdType EventId)
{
    boolean ret;
#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
    Dem_CombinationNumType groupId = Dem_GetGroupIdOfEvent(EventId);
    if (groupId != DEM_COMBINATION_NUMBER)
    {
        ret = Dem_CheckEventSuppressed(Dem_GetCombinedEventStatusByGroupId(groupId));
    }
    else
#endif
    {
        ret = Dem_CheckEventSuppressed(Dem_GetEventInternalStatusByEvent(EventId));
    }
    return ret;
}

/**
 * @brief         Marks an event's DTC suppressed
 * @param[in]     EventId: The event ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_DTCSetSuppression(Dem_EventIdType EventId)
{
    Dem_EventInternalStatusType internalStatus;
#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
    Dem_CombinationNumType groupId = Dem_GetGroupIdOfEvent(EventId);
    if (groupId != DEM_COMBINATION_NUMBER)
    {
        /** Enter Critical Section: DiagMonitor */
        Dem_EnterCritical_DiagMonitor();
        internalStatus = Dem_GetCombinedEventStatusByGroupId(groupId);
        internalStatus = Dem_SetEventSuppressed(internalStatus);
        Dem_SetCombinedEventStatusByGroupId(groupId, internalStatus);
        Dem_LeaveCritical_DiagMonitor();
        /** Leave Critical Section: DiagMonitor */
    }
    else
#endif
    {
        /** Enter Critical Section: DiagMonitor */
        Dem_EnterCritical_DiagMonitor();
        internalStatus = Dem_GetEventInternalStatusByEvent(EventId);
        internalStatus = Dem_SetEventSuppressed(internalStatus);
        Dem_SetEventInternalStatusByEvent(EventId, internalStatus);
        Dem_LeaveCritical_DiagMonitor();
        /** Leave Critical Section: DiagMonitor */
    }
}

/**
 * @brief         Marks an event's DTC un-suppressed
 * @param[in]     EventId: The event ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_DTCResetSuppression(Dem_EventIdType EventId)
{
    Dem_EventInternalStatusType internalStatus;
#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
    Dem_CombinationNumType groupId = Dem_GetGroupIdOfEvent(EventId);
    if (groupId != DEM_COMBINATION_NUMBER)
    {
        /** Enter Critical Section: DiagMonitor */
        Dem_EnterCritical_DiagMonitor();
        internalStatus = Dem_GetCombinedEventStatusByGroupId(groupId);
        internalStatus = Dem_ResetEventSuppressed(internalStatus);
        Dem_SetCombinedEventStatusByGroupId(groupId, internalStatus);
        Dem_LeaveCritical_DiagMonitor();
        /** Leave Critical Section: DiagMonitor */
    }
    else
#endif
    {
        /** Enter Critical Section: DiagMonitor */
        Dem_EnterCritical_DiagMonitor();
        internalStatus = Dem_GetEventInternalStatusByEvent(EventId);
        internalStatus = Dem_ResetEventSuppressed(internalStatus);
        Dem_SetEventInternalStatusByEvent(EventId, internalStatus);
        Dem_LeaveCritical_DiagMonitor();
        /** Leave Critical Section: DiagMonitor */
    }
}

/**
 * @brief         Change suppression of a DTC
 */
DEM_LOCAL_INLINE void Dem_ChangeDTCSuppression(Dem_EventIdType EventId, boolean SuppressionStatus)
{
    if (SuppressionStatus == TRUE)
    {
        Dem_DTCSetSuppression(EventId);
    }
    else
    {
        Dem_DTCResetSuppression(EventId);
    }
}

#endif

/**
 * @brief         For normal events, this function simply looks up the event configuration. A combined event group is
 * available only if at least one sub-event is available in the active variant. Any event of a combined group can be
 * passed to this function.
 */
DEM_LOCAL_INLINE boolean Dem_CheckDTCAvailableInVariant(Dem_EventIdType EventId)
{
    boolean ret;
#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
    Dem_CombinationNumType groupId = Dem_GetGroupIdOfEvent(EventId);
    if (groupId != DEM_COMBINATION_NUMBER)
    {
        ret = Dem_CheckCombinedEventVariant(Dem_GetCombinedEventStatusByGroupId(groupId));
    }
    else
#endif
    {
        ret = Dem_GetAvailableInVariantByEvent(EventId);
    }
    return ret;
}

/**
 * @brief         Tests if a event or combined event group is available based on runtime calibration. Any event of a
 * combined group can be passed to this function.
 * @param[in]     EventId: The event ID
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckDTCAvailable(Dem_EventIdType EventId)
{
    boolean ret;
#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
    Dem_CombinationNumType groupId = Dem_GetGroupIdOfEvent(EventId);
    if (groupId != DEM_COMBINATION_NUMBER)
    {
        ret = Dem_GetEventAvailableStatus(Dem_GetCombinedEventStatusByGroupId(groupId));
    }
    else
#endif
    {
        ret = Dem_GetEventAvailableStatusByEventId(EventId);
    }
    return ret;
}

/**
 * @brief         Gets the FaultDetectionCounter (FDC) of a DTC
 */
DEM_LOCAL_INLINE Std_ReturnType Dem_DTCGetFaultDetectionCounter(Dem_EventIdType EventId, sint8* FaultDetectionCounter)
{
    Std_ReturnType ret;
#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
    Dem_CombinationNumType groupId = Dem_GetGroupIdOfEvent(EventId);
    if (groupId != DEM_COMBINATION_NUMBER)
    {
        ret = Dem_DTCCombinedGroupGetFDC(groupId, FaultDetectionCounter);
    }
    else
#endif
    {
        ret = Dem_SatelliteGetFaultDetectionCounter(EventId, FaultDetectionCounter);
    }
    return ret;
}

#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
/**
 * @brief Calculates the maximum fault detection counter this cycle of a combined event
 */
DEM_LOCAL sint8 Dem_GetFDCMaxOfDTCCombined(Dem_CombinationNumType GroupId)
{
    sint8 maxFdc = 0;
#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
    for (Dem_ComDTCEventRefNumType start = Dem_GetStartRefOfCombinationDTC(GroupId);
         start < Dem_GetEndRefOfCombinationDTC(GroupId);
         ++start)
    {
        Dem_EventIdType subEventId = Dem_GetventIdOfCombinationDTC(start);
        if ((Dem_GetAvailableInVariantByEvent(subEventId) == TRUE)
            && (Dem_GetEventAvailableStatusByEventId(subEventId) == TRUE))
        {
            sint8 fdc;
            fdc = Dem_SatelliteGetMaxFaultDetectionCounter(
                Dem_GetSatelliteIdByEventId(subEventId),
                Dem_GetMonitorIdByEventId(subEventId));
            if (maxFdc < fdc)
            {
                maxFdc = fdc;
            }
        }
    }
#endif
    return maxFdc;
}
#endif

/**
 * @brief         Tests all options due to which an event or combined event group has to be has to be excluded from
 * reporting API, based on the combined event group for combined events. Any event of a combined group can be passed to
 * this function.
 */
DEM_LOCAL_INLINE boolean Dem_CheckDTCSuppressed(Dem_EventIdType EventId)
{
    boolean ret = (
#if (DEM_SUPPRESSION_SUPPORT == DEM_DTC_SUPPRESSION)
        (Dem_CheckDTCSuppression(EventId) == (boolean)TRUE) || /* PRQA S 4404 */ /* VL_Dem_4404 */
#endif
        (Dem_CheckDTCAvailableInVariant(EventId) == FALSE)
        || ((Dem_CheckDTCAvailable(EventId) == FALSE) && (Dem_GetEventstored(EventId) == FALSE)));
    return ret;
}

/**
 * @brief         The following callbacks are called here:
 *                - DtcStatusChanged(1..*)
 *                - DcmDtcStatusChanged(1)
 *                - J1939DcmDtcStatusChanged(1)
 */
DEM_LOCAL_INLINE void Dem_NotiDtcStatusChanged(
    Dem_EventIdType       EventId,
    Dem_UdsStatusByteType DtcStatusOld,
    Dem_UdsStatusByteType DtcStatusNew)
{
#if (DEM_CALLBACK_DTC_STATUS_CHANGED == STD_ON)
    if (Dem_CheckDTCSuppressed(EventId) == FALSE)
    {
        Dem_DTCAttRefNumType lDTCAttr = Dem_GetDTCAttr(EventId);
        if (Dem_CheckDTCAttr(lDTCAttr) == TRUE)
        {
            uint8 statusMask    = Dem_GetDTCStatusMaskByMem(Dem_GetMemRef(lDTCAttr));
            uint8 lDtcStatusOld = DtcStatusOld & statusMask;
            uint8 lDtcStatusNew = DtcStatusNew & statusMask;
            if (lDtcStatusOld != lDtcStatusNew)
            {
#if (DEM_CLIENT_CALLBACK_DTC_STATUS_CHANGED == STD_ON)
                uint32 UdsDtc = Dem_GetEventDTC(EventId);
                if (UdsDtc != DEM_DTC_UDS_INVALID) /* PRQA S 1252 */ /* VL_Dem_1252 */
                {
                    Dem_EventClientRefType clientStart = Dem_GetClientStartRefOfEvent(EventId);
                    Dem_EventClientRefType clientEnd   = Dem_GetClientEndRefOfEvent(EventId);
                    for (; clientStart < clientEnd; ++clientStart)
                    {
                        uint8 ClientId = Dem_GetClientOfEventRef(clientStart);
                        for (Dem_DTCStatusChangedNumType start = Dem_GetCallbackStartOfClient(ClientId);
                             start < Dem_GetCallbackEndOfClient(ClientId);
                             ++start)
                        {
                            const DemTriggerOnDTCStatusType pDtcStatusFunc = Dem_GetDTCStatusFnc(start);
                            if (pDtcStatusFunc != NULL_PTR)
                            {
                                /** Call Dcm Dtc status change callback, Dcm_DemTriggerOnDTCStatus is in there */
                                (void)pDtcStatusFunc(UdsDtc, lDtcStatusOld, lDtcStatusNew);
                            }
                        }
                    }
                }
#endif
#if (DEM_J1939_SUPPORT == STD_ON)
                Dem_NotiJ1939DtcStatusChanged(EventId, lDtcStatusOld, lDtcStatusNew);
#endif
            }
        }
    }
#else
    DEM_PARAM_UNUSED(EventId);
    DEM_PARAM_UNUSED(DtcStatusOld);
    DEM_PARAM_UNUSED(DtcStatusNew);
#endif
}

/**
 * @brief         Due to excessive code duplication it was necessary to fall back to higher order functions. This
 * function takes a function pointer to an operation to be executed for all sub-events of a combined event. For normal
 * events, the operation is executed for that event only, so the caller does not need to distinguish between combined
 * and non-combined events too much. The caller is still reponsible to call this function only once per combined event,
 * e.g. by calling it only for the master event.
 */
DEM_LOCAL_INLINE void Dem_CombinedEvent(
    Dem_EventIdType                EventId,
    const Dem_UtilCombinedFuncType FuncPtr,
    const Dem_CombinedArgPtrType   ArgPtr)
{
#if (DEM_EVENT_COMBINATION_SUPPORT == DEM_EVCOMB_ONSTORAGE)
    Dem_CombinationNumType groupId = Dem_GetGroupIdOfEvent(EventId);
    if (groupId != DEM_COMBINATION_NUMBER)
    {
        /** DTC has combined group */
        Dem_UdsStatusByteType DtcStatus = 0; /** neutral value */
        ArgPtr->OldDtcStatus            = Dem_GetCombinedEventUDSStatus(groupId);
        /** Iterate through all active sub-events */
        for (Dem_ComDTCEventRefNumType start = Dem_GetStartRefOfCombinationDTC(groupId);
             start < Dem_GetEndRefOfCombinationDTC(groupId);
             ++start)
        {
            Dem_EventIdType subEventId = Dem_GetventIdOfCombinationDTC(start);
            if ((Dem_GetAvailableInVariantByEvent(subEventId) == TRUE)
                && (Dem_GetEventAvailableStatusByEventId(subEventId) == TRUE))
            {
                (*FuncPtr)(subEventId, ArgPtr);
                /** Combined event status created by ORing the status bits or all sub-events */
                DtcStatus = (Dem_UdsStatusByteType)(DtcStatus | Dem_GetEventUDSStatus(subEventId));
            }
        }
        Dem_SetCombinedEventUDSStatus(groupId, DtcStatus);
        ArgPtr->NewDtcStatus = DtcStatus;
    }
    else
#endif
    {
        /** EventId is not a combined event */
        ArgPtr->OldDtcStatus = Dem_GetEventUDSStatus(EventId);
        (*FuncPtr)(EventId, ArgPtr);
        ArgPtr->NewDtcStatus = Dem_GetEventUDSStatus(EventId);
    }
}

#if (DEM_GENERAL_DISPLACEMENT_NONE == STD_OFF)
/**
 * @brief         Updates the status of a displaced DTC
 */
DEM_LOCAL_INLINE void Dem_DTCDisplaced(Dem_EventIdType EventId)
{
    Dem_UdsStatusByteType oldDtcStatus;
    Dem_CombinedArgType   args;

    oldDtcStatus = Dem_DTCApplyExternalStatus(EventId, Dem_GetDTCUDSStatus(EventId));
    Dem_CombinedEvent(EventId, Dem_EventDisplaced, &args);

#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
    {
        Dem_CombinationNumType groupId = Dem_GetGroupIdOfEvent(EventId);
        if (groupId != DEM_COMBINATION_NUMBER)
        {
            Dem_SetCombinedEventUDSStatus(groupId, args.NewDtcStatus);
            /** Enter Critical Section: DiagMonitor */
            Dem_EnterCritical_DiagMonitor();
            {
                Dem_EventInternalStatusType internalStatus = Dem_GetCombinedEventStatusByGroupId(groupId);
                internalStatus = Dem_SetEventstoredStatus(internalStatus, Dem_InternalStoredStatus_None);
                Dem_SetCombinedEventStatusByGroupId(groupId, internalStatus);
            }
            Dem_LeaveCritical_DiagMonitor();
            /** Leave Critical Section: DiagMonitor */
        }
    }
#endif

    /** Call status changed notifications (there is always a change) */
    Dem_NotiDtcStatusChanged(EventId, oldDtcStatus, Dem_DTCApplyExternalStatus(EventId, args.NewDtcStatus));
}
#endif

#if (                                                                                                     \
    (DEM_ENTRY_TRIGGER_ON_FDC_THRESHOLD == STD_ON) || (DEM_GENERAL_ED_TRIGGER_ON_FDC_THRESHOLD == STD_ON) \
    || (DEM_GENERAL_FF_TRIGGER_ON_FDC_THRESHOLD == STD_ON))
/**
 * @brief Updates event data due to exceeding a FDC threshold
 */
DEM_LOCAL void Dem_DTCFdcTrip(Dem_EventIdType EventId, boolean FirstThisCycle)
{
    Dem_NvBlockNumType   memoryIndex = DEM_MEM_INVALID_MEMORY_INDEX;
    Dem_DTCAttRefNumType lDTCAttr    = Dem_GetDTCAttr(EventId);
    if (Dem_CheckDTCAttr(lDTCAttr) == TRUE)
    {
        Dem_MemoryNumType            memRef     = Dem_GetMemRef(lDTCAttr);
        Dem_MemStateInfoConstPtrType memoryInfo = Dem_MemStateInfoInit(memRef);

        if (Dem_DTCCheckStoredStatus(EventId) == TRUE)
        {
            memoryIndex = Dem_MemoryFindIndex(memoryInfo, EventId);
        }
#if (DEM_ENTRY_TRIGGER_ON_FDC_THRESHOLD == STD_ON)
        else
        {
            if (DEM_FLAG_ISSET(Dem_GetStorageTriggerOfMemory(memRef), DEM_TRIGGER_FDC))
            {
                /** FdcTrip is the storage trigger and the event is not yet stored */
                memoryIndex = Dem_AllocateMemory(EventId);
            }
        }
#endif
        if ((memoryIndex != DEM_MEM_INVALID_MEMORY_INDEX) && (FirstThisCycle == TRUE))
        {
            uint8 blockModification = Dem_MemUpdateEventEntry(EventId, DEM_TRIGGER_FDC, memoryIndex);
#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
            if (blockModification != DEM_DATA_NO_CHANGES)
            {
                Dem_NvmSetSingleBlockState(memoryIndex, DEM_NVM_BLOCKSTATE_DIRTY);
            }
#endif
            if (DEM_FLAG_ISSET(blockModification, DEM_DATA_USERDATA_CHANGED))
            {
                Dem_NotiEventDataChanged(EventId);
            }
        }
    }
#if (DEM_MAX_NUMBER_PRESTORED_FF > 0u)
    Dem_SetPrestoreStateDiscard();
#endif
}
#endif

#if (defined(DEM_MAX_FDC_SINCE_LAST_CLEAR))
/**
 * @brief  Update event data 'maximum FDC since last clear'
 */
DEM_LOCAL void Dem_DTCUpdateFdcMax(Dem_EventIdType EventId)
{
    if (Dem_GetEventstored(EventId) == TRUE)
    {
        /** Already stored - Find, *but don't update* the memory entry */
        Dem_DTCAttRefNumType lDTCAttr = Dem_GetDTCAttr(EventId);
        if (Dem_CheckDTCAttr(lDTCAttr) == TRUE)
        {
#if (defined(DEM_MAX_FDC_SINCE_LAST_CLEAR))
            Dem_MemoryNumType            memRef      = Dem_GetMemRef(lDTCAttr);
            Dem_MemStateInfoConstPtrType memoryInfo  = Dem_MemStateInfoInit(memRef);
            Dem_NvBlockNumType           memoryIndex = Dem_MemoryFindIndex(memoryInfo, EventId);
            /** synchronize the current max FDC with the stored FDC max value */
            sint8 oldFdc = (sint8)Dem_GetMaxDebounceValueOfMemEntry(memoryIndex);
#endif
#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
            sint8 newFdc;
#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
            Dem_CombinationNumType groupId = Dem_GetGroupIdOfEvent(EventId);
            if (groupId != DEM_COMBINATION_NUMBER)
            {
                newFdc = Dem_GetFDCMaxOfDTCCombined(groupId);
            }
            else
#endif
            {
                newFdc = Dem_SatelliteGetMaxFaultDetectionCounter(
                    Dem_GetSatelliteIdByEventId(EventId),
                    Dem_GetMonitorIdByEventId(EventId));
            }
#endif
#if (defined(DEM_MAX_FDC_SINCE_LAST_CLEAR))
            /* PRQA S 2152 ++ */ /* VL_Dem_2152 */
            if ((sint32)oldFdc < (sint32)newFdc)
            {
                Dem_SetMaxDebounceValueOfMemEntry(memoryIndex, (sint16)newFdc);
#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
                Dem_NvmSetSingleBlockState(memoryIndex, DEM_NVM_BLOCKSTATE_DIRTY);
#endif
            }
            /* PRQA S 2152 -- */
#endif
        }
    }
}
#endif

/**
 * @brief         Tests whether an event or combined event may age
 * @param[in]     EventId: The event ID
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckDtcMayAge(Dem_EventIdType EventId)
{
    return Dem_CheckEventMayAge(Dem_GetDTCUDSStatus(EventId));
}

#if (DEM_AGING_ENABLE == STD_ON)
/**
 * @brief Starts aging of an event.In case the event already ages (or does not support aging), this function has no
 * effect.
 */
DEM_LOCAL void Dem_DTCAgingStart(Dem_EventIdType EventId)
{
    Dem_EventInternalStatusType internalStatus = Dem_GetDtcInterStatus(EventId);
    if (Dem_GetEventstoreStatus(internalStatus) < Dem_InternalStoredStatus_Aging)
    {
        /** <=> Dem_DTCInternalStatus_StoredStatus_None || Dem_DTCInternalStatus_StoredStatus_Active */
        Dem_NvBlockNumType           memoryIndex = DEM_MEM_INVALID_MEMORY_INDEX;
        Dem_MemStateInfoConstPtrType memoryInfo  = Dem_MemStateInfoInit(Dem_GetMemRef(Dem_GetDTCAttr(EventId)));

#if (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_OFF)
        /** If configuration parameter DemResetConfirmedBitOnOverflow is set to false and in case an event has UDS
         * status bit 3 set and gets qualified as passed and is not stored in an event memory entry the Dem module shall
         * try to allocate an event memory entry to get an aging counter. */
        if (Dem_GetEventstoreStatus(internalStatus) == Dem_InternalStoredStatus_None)
        {
            Dem_UdsStatusByteType dtcStatus = Dem_GetDTCUDSStatus(EventId);
            /** If necessary allocate an aging counter */
            if ((Dem_UDSStatusCheckCDTC(dtcStatus) == TRUE) && (Dem_UDSStatusCheckTF(dtcStatus) == FALSE))
            {
                memoryIndex = Dem_MemoryAllocateAging(memoryInfo, EventId);
            }
        }
        else
#endif
            if (Dem_GetEventstoreStatus(internalStatus) == Dem_InternalStoredStatus_Active)
        {
            memoryIndex = Dem_MemoryFindIndex(memoryInfo, EventId);
        }
        else
        {
            /** Misra C */
        }

        if (memoryIndex != DEM_MEM_INVALID_MEMORY_INDEX)
        {
            if (1u == Dem_GetEventMaxAgingThreshold(EventId))
            {
                /** Aging cycle reached, so this event has aged */
                Dem_DTCAged(EventId, memoryIndex);
            }
            else
            {
                /** start Aging */
                uint16 agingCounter;
                /** Initialize the aging target of the memory entry */
                agingCounter = Dem_GetCycleCounter(Dem_GetEventAgingCycle(EventId));
                agingCounter = Dem_AddCycleCount(agingCounter, Dem_GetEventMaxAgingThreshold(EventId));
                if (Dem_GetEventAgingCycle(EventId) == Dem_GetOCRefOfEvent(EventId))
                {
                    /** The current operation cycle meets the aging condition. When aging begins, the ISO diagramming
                     * method should be used to count the additional items required. */
                    agingCounter--;
                }
                Dem_SetAgingCounterOfMemEntry(memoryIndex, agingCounter);
#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
                Dem_NvmSetSingleBlockState(memoryIndex, DEM_NVM_BLOCKSTATE_DIRTY);
#endif

                /** Enter Critical Section: DiagMonitor */
                Dem_EnterCritical_DiagMonitor();
                internalStatus = Dem_GetDtcInterStatus(EventId);
                /** In critical section 'DiagMonitor', set the events stored status to 'AGING' */
                internalStatus = Dem_SetEventstoredStatus(internalStatus, Dem_InternalStoredStatus_Aging);
                Dem_SetDtcInterStatus(EventId, internalStatus);
                Dem_LeaveCritical_DiagMonitor();
                /** Leave Critical Section: DiagMonitor */
            }
        }
    }
}
#endif

#if (DEM_AGING_ENABLE == STD_ON)
#if (DEM_STATUS_BIT_HANDLING_TEST_FAILED_SINCE_LAST_CLEAR == STD_ON)
/**
 * @brief DemAgingCycleCounterThreshold or DemAgingCycleCounterThresholdForTFSLC (depending on which value is higher)
 * defines the number of completed aging cycles, after which the event memory entry shall be deleted(aged) from the
 * event memory.
 */
DEM_LOCAL void
    Dem_DTCCheckAgingCycleThreshold(Dem_EventIdType EventId, uint16 CycleCounter, Dem_NvBlockNumType MemoryIndex)
{
    uint8 ageThr      = Dem_GetEventAgingThreshold(EventId);
    uint8 ageThrTFSLC = Dem_GetEventAgingThresholdTFSLC(EventId);
    /** Upon event aging counter reach threshold DemAgingCycleCounterThresholdForTFSLC, the UDS status bit 5
     * (TestFailedSinceLastClear) shall be set to 0 if DemStatusBitHandlingTestFailedSinceLastClear is set to
     * DEM_STATUS_BIT_AGING_AND_DISPLACEMENT */
    if (ageThr > ageThrTFSLC)
    {
        /** In case parameter DemAgingCycleCounterThresholdForTFSLC is not configured, TestFailedSinceLastClear will not
         * be aged. */
        if (ageThrTFSLC != 0u)
        {
            if ((uint16)((uint16)ageThr - (uint16)ageThrTFSLC)
                == (Dem_GetAgingCounterOfMemEntry(MemoryIndex) - CycleCounter))
            {
                Dem_EventResetUdsStatus(EventId, DEM_UDS_STATUS_TFSLC);
            }
        }
    }
    else if (ageThr < ageThrTFSLC)
    {
        /** Upon event aging counter reach threshold DemAgingCycleCounterThreshold, the UDS status bit 3 shall be set to
         * 0. */
        if ((uint16)((uint16)ageThrTFSLC - (uint16)ageThr)
            == (Dem_GetAgingCounterOfMemEntry(MemoryIndex) - CycleCounter))
        {
            Dem_EventResetUdsStatus(EventId, DEM_UDS_STATUS_CDTC);
        }
    }
    else
    {
        /** Still counting */
    }
}
#endif
#endif

#if ((DEM_AGING_REQUIRES_TESTED_CYCLE == STD_ON) || (DEM_AGING_REQUIRES_NOT_FAILED_CYCLE == STD_ON))
/**
 * @brief         Tests whether the current cycle fulfills the conditions for aging
 * @param[in]     UdsStatus: Status of UDS
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_DTCheckStatusForAgingCycle(Dem_UdsStatusByteType UdsStatus)
{
    boolean ret = TRUE;
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
#endif

#if (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_OFF)
/**
 * @brief If configuration parameter DemResetConfirmedBitOnOverflow is set to false and an event memory entry aging
 * occurs the Dem module shall check for other events having UDS status bit 3 set to 1 and UDS status bit 0
 * set to 0. If such an event is found, the Dem shall allocate an event memory location to get an aging counter.
 */
DEM_LOCAL void Dem_DTCAgedTryAgingOther(Dem_MemoryNumType MemIndex)
{
    Dem_MemStateInfoPtrType memoryInfo = Dem_MemStateInfoInit(MemIndex);
    for (Dem_EventIdType eventId = memoryInfo->FirstEvent; eventId < memoryInfo->LastEvent; ++eventId)
    {
        if ((Dem_CheckDTCAttr(eventId) == TRUE) && (Dem_GetMemRef(Dem_GetDTCAttr(eventId)) == MemIndex))
        {
            Dem_EventInternalStatusType internalStatus = Dem_GetDtcInterStatus(eventId);
            Dem_UdsStatusByteType       dtcStatus      = Dem_GetDTCUDSStatus(eventId);
            if ((Dem_GetEventstoreStatus(internalStatus) == Dem_InternalStoredStatus_None)
                && (Dem_UDSStatusCheckCDTC(dtcStatus) == TRUE) && (Dem_UDSStatusCheckTF(dtcStatus) == FALSE))
            {
                /** If necessary allocate an aging counter */
                Dem_NvBlockNumType memoryIndex = Dem_MemoryAllocateAging(memoryInfo, eventId);
                if (memoryIndex != DEM_MEM_INVALID_MEMORY_INDEX)
                {
                    /** start Aging */
                    uint16 agingCounter;
                    /** Initialize the aging target of the memory entry */
                    agingCounter = Dem_GetCycleCounter(Dem_GetEventAgingCycle(eventId));
                    agingCounter = Dem_AddCycleCount(agingCounter, Dem_GetEventMaxAgingThreshold(eventId));
                    Dem_SetAgingCounterOfMemEntry(memoryIndex, agingCounter);
#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
                    Dem_NvmSetSingleBlockState(memoryIndex, DEM_NVM_BLOCKSTATE_DIRTY);
#endif
                    /** Enter Critical Section: DiagMonitor */
                    Dem_EnterCritical_DiagMonitor();
                    internalStatus = Dem_GetDtcInterStatus(eventId);
                    /** In critical section 'DiagMonitor', set the events stored status to 'AGING' */
                    internalStatus = Dem_SetEventstoredStatus(internalStatus, Dem_InternalStoredStatus_Aging);
                    Dem_SetDtcInterStatus(eventId, internalStatus);
                    Dem_LeaveCritical_DiagMonitor();
                    /** Leave Critical Section: DiagMonitor */
                }
            }
        }
    }
}
#endif

#if (DEM_AGING_ENABLE == STD_ON)
/**
 * @brief Completes aging of a DTC.
 */
DEM_LOCAL void Dem_DTCAged(Dem_EventIdType EventId, Dem_NvBlockNumType MemoryIndex)
{
    Dem_UdsStatusByteType oldDtcStatus;
    Dem_CombinedArgType   args;

    /** Process aging for each sub-event status */
    oldDtcStatus = Dem_DTCApplyExternalStatus(EventId, Dem_GetDTCUDSStatus(EventId));
    Dem_CombinedEvent(EventId, Dem_EventAged, &args);

    if (MemoryIndex != DEM_MEM_INVALID_MEMORY_INDEX)
    {
        Dem_MemoryNumType            memIndex   = Dem_GetMemRef(Dem_GetDTCAttr(EventId));
        Dem_MemStateInfoConstPtrType memoryInfo = Dem_MemStateInfoInit(memIndex);
        Dem_MemoryFreeIndex(memoryInfo, MemoryIndex);
#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
        Dem_NvmSetSingleBlockState(MemoryIndex, DEM_NVM_BLOCKSTATE_DIRTY);
#endif

#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
        /** For combined events, synchronize the combined internal stored status */
        {
            Dem_CombinationNumType groupId = Dem_GetGroupIdOfEvent(EventId);
            if (groupId != DEM_COMBINATION_NUMBER)
            {
                Dem_SetCombinedEventUDSStatus(groupId, args.NewDtcStatus);
                /** Enter Critical Section: DiagMonitor */
                Dem_EnterCritical_DiagMonitor();
                {
                    Dem_EventInternalStatusType internalStatus = Dem_GetCombinedEventStatusByGroupId(groupId);
                    internalStatus = Dem_SetEventstoredStatus(internalStatus, Dem_InternalStoredStatus_None);
                    Dem_SetCombinedEventStatusByGroupId(groupId, internalStatus);
                }
                Dem_LeaveCritical_DiagMonitor();
                /** Leave Critical Section: DiagMonitor */
            }
        }
#endif

        /** If configured, notify the DTC status change */
        if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == TRUE)
        {
            Dem_NotiDtcStatusChanged(EventId, oldDtcStatus, Dem_DTCApplyExternalStatus(EventId, args.NewDtcStatus));
        }
#if (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_OFF)
        Dem_DTCAgedTryAgingOther(memIndex);
#endif
    }
}
#endif

#if (DEM_AGING_ENABLE == STD_ON)
/**
 * @brief Processes the aging cycle for a DTC
 */
DEM_LOCAL void Dem_DTCAgingCycle(Dem_EventIdType EventId, uint16 CycleCounter, Dem_NvBlockNumType MemoryIndex)
{
    if (Dem_GetEventAgingThreshold(EventId) != 0u)
    {
#if ((DEM_AGING_REQUIRES_TESTED_CYCLE == STD_ON) || (DEM_AGING_REQUIRES_NOT_FAILED_CYCLE == STD_ON))
        if (Dem_DTCheckStatusForAgingCycle(Dem_GetDTCUDSStatus(EventId)) == FALSE)
        {
            /** Untested cycle requires recalculation of aging target */
            uint16 cycleCount = Dem_GetAgingCounterOfMemEntry(MemoryIndex);
            if (cycleCount <= DEM_OPERATIONCYCLE_CYCLECOUNT_MAX)
            {
                /** Ignore maxed (latched) counter */
                cycleCount = Dem_AddCycleCount(cycleCount, 1u);
                Dem_SetAgingCounterOfMemEntry(MemoryIndex, cycleCount);
#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
                Dem_NvmSetSingleBlockState(MemoryIndex, DEM_NVM_BLOCKSTATE_DIRTY);
#endif
            }
        }
        else
#endif
            if (CycleCounter == Dem_GetAgingCounterOfMemEntry(MemoryIndex))
        {
            /** Aging cycle reached, so this event has aged */
            Dem_DTCAged(EventId, MemoryIndex);
        }
        else
        {
            /** Still counting */
#if (DEM_STATUS_BIT_HANDLING_TEST_FAILED_SINCE_LAST_CLEAR == STD_ON)
            Dem_DTCCheckAgingCycleThreshold(EventId, CycleCounter, MemoryIndex);
#endif
        }
    }
    else
    {
        /** Latch the aging counter and set the memory entries NV status to DIRTY */
        Dem_SetAgingCounterOfMemEntry(MemoryIndex, DEM_OPERATIONCYCLE_CYCLECOUNT_LATCHED);
#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
        Dem_NvmSetSingleBlockState(MemoryIndex, DEM_NVM_BLOCKSTATE_DIRTY);
#endif
    }
}
#endif

#if (defined(DEM_OCC2))
/**
 * @brief         In each operation cycle the counter shall be incremented after completing an operation cycle in which
 * the FDC10 reaches the value -128, unless FDC10 has already reached a value that is equal to or greater than its
 * unconfirmedDTCLimit in the current operation cycle.
 * @param[in]     DTCContext: The DTC context
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_DTCcheckPassedThisOperationAndNoFailed(const Dem_DTCContextType* DTCContext)
{
    boolean ret = FALSE;
    if ((Dem_UDSStatusCheckTNCTOC(DTCContext->OldDtcStatus) == TRUE)
        && (Dem_UDSStatusCheckTFTOC(DTCContext->OldDtcStatus) == FALSE))
    {
        ret = TRUE;
    }
    return ret;
}
#endif

/**
 * @brief Process DTC triggers on 'Passed' status report.
 */
DEM_LOCAL void Dem_DTCPassedProcessDTCEffects(const Dem_DTCContextType* DTCContext)
{
#if (((DEM_AGING_ENABLE == STD_ON) && (DEM_AGING_REQUIRES_NOT_FAILED_CYCLE == STD_ON)) || (defined(DEM_OCC2)))
    Dem_EventIdType      eventId  = DTCContext->EventId;
    Dem_DTCAttRefNumType lDTCAttr = Dem_GetDTCAttr(eventId);
    if (Dem_CheckDTCAttr(lDTCAttr) == TRUE)
    {
#if ((defined(DEM_OCC2)) || (defined(DEM_SI30)))
        Dem_MemoryNumType            memRef     = Dem_GetMemRef(lDTCAttr);
        Dem_MemStateInfoConstPtrType memoryInfo = Dem_MemStateInfoInit(memRef);
        if (Dem_GetEventstored(eventId) == TRUE)
        {
            Dem_NvBlockNumType memoryIndex = Dem_MemoryFindIndex(memoryInfo, eventId);
#if (defined(DEM_OCC2))
            if (Dem_DTCcheckPassedThisOperationAndNoFailed(DTCContext) == TRUE)
            {
                if (Dem_MemIncrementOCC2(memoryIndex) == TRUE)
                {
#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
                    Dem_NvmSetSingleBlockState(memoryIndex, DEM_NVM_BLOCKSTATE_DIRTY);
#endif
                }
            }
#endif
#if (defined(DEM_SI30))
            Dem_ResetSI30Bit0FromMemEntry(memoryIndex);
#endif
        }
#endif
    }
#else
    DEM_PARAM_UNUSED(DTCContext);
#endif
}

#if (DEM_AGING_ENABLE == STD_ON)
/**
 * @brief         Processes aging at the end of the DTC's operation cycle
 */
DEM_LOCAL_INLINE void Dem_DTCOperationCyclEndAging(Dem_EventIdType EventId)
{
    if (Dem_CheckDTCAttr(Dem_GetDTCAttr(EventId)) == TRUE)
    {
        if (Dem_GetEventAgingThreshold(EventId) != 0u)
        {
            Dem_DTCAgingStart(EventId);
        }
    }
}
#endif

/**
 * @brief         Process Dtc storage into event memory
 * @param[in]     EventId: The event ID
 * @param[in]     Trigger: The DTC storage trigger type
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_DTCStorageTriggerCheck(Dem_EventIdType EventId, uint8 Trigger)
{
    boolean ret = FALSE;
#if ((DEM_ENTRY_TRIGGER_ON_CONFIRMED == STD_ON) || (DEM_ENTRY_TRIGGER_ON_TEST_FAILED == STD_ON))
    Dem_MemoryNumType memRef   = Dem_GetMemRef(Dem_GetDTCAttr(EventId));
    uint8             lTrigger = Dem_GetStorageTriggerOfMemory(memRef);
#endif
#if (DEM_ENTRY_TRIGGER_ON_CONFIRMED == STD_ON)
    if ((DEM_FLAG_ISSET(Trigger, DEM_TRIGGER_CONFIRMED)) && (lTrigger == DEM_TRIGGER_CONFIRMED))
    {
        ret = TRUE;
    }
#endif
#if (DEM_ENTRY_TRIGGER_ON_TEST_FAILED == STD_ON)
    if ((DEM_FLAG_ISSET(Trigger, DEM_TRIGGER_TESTFAILED)) && (lTrigger == DEM_TRIGGER_TESTFAILED))
    {
        ret = TRUE;
    }
#endif
    return ret;
}

#if (defined(DEM_OCC6))
/**
 * @brief         The counter may also be cleared when the confirmedDTCLimit is reached.
 * @param[in]     DTCContext: The DTC context
 * @param[in]     MemoryIndex: Memory index
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8
    Dem_DtcDoClearOCC6OnConfirmed(const Dem_DTCContextType* DTCContext, Dem_NvBlockNumType MemoryIndex)
{
    uint8 blockModification = DEM_DATA_NO_CHANGES;
    if ((Dem_UDSStatusCheckCDTC(DTCContext->NewDtcStatus) == TRUE) && (MemoryIndex != DEM_MEM_INVALID_MEMORY_INDEX)
        && (Dem_GetOCC6OfMemEntry(MemoryIndex) != 0u))
    {
        Dem_SetOCC6OfMemEntry(MemoryIndex, 0u);
        blockModification = DEM_DATA_STATISTICS_CHANGED;
    }
    return blockModification;
}
#endif

#if (defined(DEM_OCCCTR))
/**
 * @brief         Check to increment the occurrence counter
 * @param[in]     DTCContext: The DTC context
 * @param[in]     MemoryIndex: Memory index
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_CheckIncrementOCCCTR(const Dem_DTCContextType* DTCContext, Dem_NvBlockNumType MemoryIndex)
{
    uint8 blockModification = DEM_DATA_NO_CHANGES;
#if (                                                                                        \
    (DEM_GENERAL_PROCESS_OCCCTR_CDTC == STD_ON) || (DEM_GENERAL_PROCESS_OCCCTR_TF == STD_ON) \
    || (DEM_GENERAL_PROCESS_OCCCTR_TFTOC == STD_ON))
    Dem_EventIdType eventId = DTCContext->EventId;
    if (MemoryIndex != DEM_MEM_INVALID_MEMORY_INDEX)
    {
        Dem_MemoryNumType                   memRef         = Dem_GetMemRef(Dem_GetDTCAttr(eventId));
        Dem_OccurrenceCounterProcessingType lOCCProcessing = Dem_GetOCCProcessingOfMemory(memRef);
        boolean                             lIncrement     = FALSE;
        DEM_PARAM_UNUSED(memRef);

#if (DEM_GENERAL_PROCESS_OCCCTR_CDTC == STD_ON)
        if ((lOCCProcessing == DEM_PROCESS_OCCCTR_CDTC)
            && ((Dem_UDSStatusCheckCDTC(DTCContext->OldDtcStatus) == TRUE)
                || (DEM_FLAG_ISSET(DTCContext->Dtctriggers, DEM_TRIGGER_CONFIRMED)))
            && (Dem_UDSStatusCheckTF(DTCContext->NewDtcStatus) == TRUE)
            && (Dem_UDSStatusCheckTF(DTCContext->OldDtcStatus) == FALSE))
        {
            lIncrement = TRUE;
        }
        else
#endif
#if (DEM_GENERAL_PROCESS_OCCCTR_TF == STD_ON)
            if ((lOCCProcessing == DEM_PROCESS_OCCCTR_TF) && (Dem_UDSStatusCheckTF(DTCContext->NewDtcStatus) == TRUE)
                && (Dem_UDSStatusCheckTF(DTCContext->OldDtcStatus) == FALSE))
        {
            lIncrement = TRUE;
        }
        else
#endif
#if (DEM_GENERAL_PROCESS_OCCCTR_TFTOC == STD_ON)
            if ((lOCCProcessing == DEM_PROCESS_OCCCTR_TFTOC) && (Dem_UDSStatusCheckTF(DTCContext->NewDtcStatus) == TRUE)
                && (Dem_UDSStatusCheckTF(DTCContext->OldDtcStatus) == FALSE)
                && (Dem_UDSStatusCheckTFTOC(DTCContext->NewDtcStatus) == TRUE))
        {
            lIncrement = TRUE;
        }
        else
#endif
        {
            /** Misra C */
        }

        if (lIncrement == TRUE)
        {
            if (Dem_MemIncrementOCC(MemoryIndex) == TRUE)
            {
                DEM_FLAG_SET(blockModification, DEM_DATA_STATISTICS_CHANGED); /* PRQA S 3473 */ /* VL_Dem_3473 */
            }
        }
    }
#endif
    return blockModification;
}
#endif

/**
 * @brief Process Dtc storage into event memory
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
DEM_LOCAL void Dem_DTCProcessStorage(Dem_EventContextType* EventContext, Dem_DTCContextType* DTCContext)
{
    Dem_EventIdType    eventId           = DTCContext->EventId;
    Dem_NvBlockNumType memoryIndex       = DEM_MEM_INVALID_MEMORY_INDEX;
    boolean            isOccurrence      = FALSE;
    uint8              blockModification = DEM_DATA_NO_CHANGES;

    if (Dem_DTCCheckStoredStatus(eventId) == FALSE)
    {
#if ((DEM_ENTRY_TRIGGER_ON_CONFIRMED == STD_ON) || (DEM_ENTRY_TRIGGER_ON_TEST_FAILED == STD_ON))
        /** Not stored yet - If no entry can be created we need to discard some updates */
        if (Dem_DTCStorageTriggerCheck(eventId, DTCContext->Dtctriggers) == TRUE)
        {
            /** We have to store this event in memory */
            memoryIndex = Dem_AllocateMemory(eventId);
        }
        if (memoryIndex == DEM_MEM_INVALID_MEMORY_INDEX)
        {
            /** Storage was not possible, discard changes that require storage */
            /* PRQA S 3473, 4399, 4461 ++ */ /* VL_Dem_3473, VL_Dem_4399, VL_Dem_4461 */
            DEM_FLAG_RESET(EventContext->Eventtriggers, DEM_STORAGE_TRIGGER);
            DEM_FLAG_RESET(DTCContext->Dtctriggers, DEM_STORAGE_TRIGGER);
            /* PRQA S 3473, 4399, 4461 -- */
        }
#endif
    }
    else
    {
        /** Already stored - Update the existing entry if necessary */
        Dem_MemStateInfoConstPtrType memoryInfo = Dem_MemStateInfoInit(Dem_GetMemRef(Dem_GetDTCAttr(eventId)));
        memoryIndex                             = Dem_MemoryFindIndex(memoryInfo, eventId);
        isOccurrence      = (boolean)DEM_FLAG_ISSET(DTCContext->Dtctriggers, DEM_TRIGGER_TESTFAILED);
        blockModification = Dem_UpdateEventMemoryStatistics(eventId, memoryIndex, isOccurrence);
    }

    if (memoryIndex != DEM_MEM_INVALID_MEMORY_INDEX)
    {
#if ((defined(DEM_CYCLES_SINCE_FIRST_FAILED)) || (defined(DEM_OCC3)))
        if (Dem_GetCyclesSinceFirstFailedOfMemEntry(memoryIndex) == DEM_OPERATIONCYCLE_CYCLECOUNT_INVALID)
        {
            Dem_SetCyclesSinceFirstFailedOfMemEntry(memoryIndex, Dem_GetCycleCounter(Dem_GetOCRefOfEvent(eventId)));
            DEM_FLAG_SET(blockModification, DEM_DATA_STATISTICS_CHANGED); /* PRQA S 3473 */ /* VL_Dem_3473 */
        }
#endif
#if (defined(DEM_OCC6))
        blockModification |= Dem_DtcDoClearOCC6OnConfirmed(DTCContext, memoryIndex);
#endif
#if (defined(DEM_OCCCTR))
        blockModification |= Dem_CheckIncrementOCCCTR(DTCContext, memoryIndex);
#endif

        blockModification |= Dem_MemUpdateEventEntry(eventId, DTCContext->Dtctriggers, memoryIndex);
    }
#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
    if (blockModification != DEM_DATA_NO_CHANGES)
    {
        Dem_NvmSetSingleBlockState(memoryIndex, DEM_NVM_BLOCKSTATE_DIRTY);
    }
#endif
    EventContext->DataModified = (boolean)DEM_FLAG_ISSET(blockModification, DEM_DATA_USERDATA_CHANGED);
}
/* PRQA S 6070 -- */

#if (DEM_AVAILABILITY_SUPPORT == DEM_EVENT_AVAILABILITY)
#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
/**
 * @brief Marks a combined event unavailable
 */
DEM_LOCAL void Dem_DTCDisableCombined(Dem_EventIdType EventId)
{
    Dem_CombinationNumType      groupId             = Dem_GetGroupIdOfEvent(EventId);
    Dem_EventInternalStatusType groupInternalStatus = Dem_GetCombinedEventStatusByGroupId(groupId);
    Dem_UdsStatusByteType       oldEventStatus      = Dem_GetEventUDSStatus(EventId);

    /** Remove pending actions: time debouncing, FDC triggers and queue state  */
    Dem_SatelliteClearEvent(EventId);
    Dem_SetEventUDSStatus(EventId, 0x00U);
    Dem_NotiEventStatusChanged(EventId, oldEventStatus, 0x00u);

    if (Dem_GetEventAvailableStatus(groupInternalStatus) == TRUE)
    {
        /** If all Events belonging to a DTC are disconnected, that DTC is disconnected as well */
        /** If all connected events are suppressed or disconnected, the DTC is suppressed as well */
        Dem_UdsStatusByteType combinedGroupStatus = 0x00;
        boolean               doDisable           = TRUE;

        for (Dem_ComDTCEventRefNumType start = Dem_GetStartRefOfCombinationDTC(groupId);
             start < Dem_GetEndRefOfCombinationDTC(groupId);
             ++start)
        {
            Dem_EventIdType subEventId = Dem_GetventIdOfCombinationDTC(start);
            if (Dem_GetAvailableInVariantByEvent(subEventId) == TRUE)
            {
                Dem_EventInternalStatusType internalStatus = Dem_GetEventInternalStatusByEvent(subEventId);
                /** If all events are disconnected now, the combined group is set to disconnected */
                doDisable = (doDisable && (Dem_GetEventAvailableStatusByEventId(internalStatus) == FALSE));

                combinedGroupStatus |= Dem_GetEventUDSStatus(subEventId);
            }
        }
        if (doDisable == TRUE)
        {
            groupInternalStatus = Dem_ResetEventAvailableStatus(groupInternalStatus);
        }
        Dem_SetCombinedEventStatusByGroupId(groupId, groupInternalStatus);
        Dem_SetCombinedEventUDSStatus(groupId, combinedGroupStatus);
    }
}

/**
 * @brief Marks a combined event available
 */
DEM_LOCAL void Dem_DTCEnableCombined(Dem_EventIdType EventId)
{
    Dem_CombinationNumType      groupId             = Dem_GetGroupIdOfEvent(EventId);
    Dem_EventInternalStatusType groupInternalStatus = Dem_GetCombinedEventStatusByGroupId(groupId);
    Dem_UdsStatusByteType       oldEventStatus      = Dem_GetEventUDSStatus(EventId);

    Dem_SetEventUDSStatus(EventId, DEM_UDS_STATUS_INITIALIZE);
    Dem_NotiEventStatusChanged(EventId, oldEventStatus, DEM_UDS_STATUS_INITIALIZE);

    if (Dem_GetEventAvailableStatus(groupInternalStatus) == FALSE)
    {
        Dem_UdsStatusByteType combinedGroupStatus = 0x00;
        /** Reconnecting an event can cause a suppressed DTC to become un-suppressed */
        for (Dem_ComDTCEventRefNumType start = Dem_GetStartRefOfCombinationDTC(groupId);
             start < Dem_GetEndRefOfCombinationDTC(groupId);
             ++start)
        {
            Dem_EventIdType subEventId = Dem_GetventIdOfCombinationDTC(start);
            if (Dem_GetAvailableInVariantByEvent(subEventId) == TRUE)
            {
                /** Calculate the combined event status - logical 'or' of all connected event status bytes (includes
                 * self) */
                combinedGroupStatus |= Dem_GetEventUDSStatus(subEventId);
            }
        }

        /** Since at least one sub-events is connected now, set the combined group to connected */
        groupInternalStatus = Dem_SetEventAvailableStatus(groupInternalStatus);

        Dem_SetCombinedEventStatusByGroupId(groupId, groupInternalStatus);
        Dem_SetCombinedEventUDSStatus(groupId, combinedGroupStatus);
    }
    else
    {
        Dem_SetCombinedEventUDSStatus(groupId, Dem_CalculateCombineUdsStatus(groupId));
    }
}
#endif

/**
 * @brief         Unavailable the DTC's event. If the DTC has a combined group, disconnect all its subevents.
 */
DEM_LOCAL_INLINE void Dem_DTCDisable(Dem_EventIdType EventId)
{
#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
    if (Dem_GetGroupIdOfEvent(EventId) != DEM_COMBINATION_NUMBER)
    {
        Dem_DTCDisableCombined(EventId);
    }
    else
#endif
    {
        Dem_EventDisableNormal(EventId);
    }
}

/**
 * @brief         Available the DTC's event. If the DTC has a combined group, reconnect all its subevents.
 */
DEM_LOCAL_INLINE void Dem_DTCEnable(Dem_EventIdType EventId)
{
#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
    if (Dem_GetGroupIdOfEvent(EventId) != DEM_COMBINATION_NUMBER)
    {
        Dem_DTCEnableCombined(EventId);
    }
    else
#endif
    {
        Dem_EventEnableNormal(EventId);
    }
}
#endif

/**
 * @brief         Update the UDS status of DTC due to operation cycle state change
 */
DEM_LOCAL_INLINE void Dem_DTCOperationCycle(Dem_EventIdType EventId)
{
    Dem_CombinedArgType args;
    Dem_CombinedEvent(EventId, Dem_EventOperationCycle, &args);
#if (defined(DEM_SI30))
    Dem_ResetSI30Bit1OfMemEntry(EventId);
#endif
}

/**
 * @brief Read the DTC number for the given EventId
 */
DEM_LOCAL Std_ReturnType Dem_GetDTCByEvent(Dem_EventIdType EventId, Dem_DTCFormatType DTCFormat, uint32* DTCOfEvent)
{
    Std_ReturnType ret = E_NOT_OK;
    if ((Dem_GetAvailableInVariantByEvent(EventId) == TRUE) && (Dem_GetEventAvailableStatusByEventId(EventId) == TRUE))
    {
        uint32 dtc;
        switch (DTCFormat)
        {
        case DEM_DTC_FORMAT_UDS:
            dtc = Dem_GetEventDTC(EventId);
            if (DEM_DTC_UDS_INVALID != dtc) /* PRQA S 1252,1258 */ /* VL_Dem_1252,VL_Dem_1258 */
            {
                *DTCOfEvent = dtc;
                ret         = E_OK;
            }
            else
            {
                ret = DEM_E_NO_DTC_AVAILABLE;
            }
            break;
#if (DEM_J1939_SUPPORT == STD_ON)
        case DEM_DTC_FORMAT_J1939:
            dtc = Dem_GetEventJ1939DTC(EventId);
            if (DEM_DTC_J1939_INVALID != dtc) /* PRQA S 1252,1258 */ /* VL_Dem_1252,VL_Dem_1258 */
            {
                *DTCOfEvent = dtc;
                ret         = E_OK;
            }
            else
            {
                ret = DEM_E_NO_DTC_AVAILABLE;
            }
            break;
#endif
#if (DEM_OBDII_SUPPORT == STD_ON)
        case DEM_DTC_FORMAT_OBD:
#if (DEM_SUPPORTED_OBD_UDS_DTC_SEPARATION == STD_OFF)
            dtc = Dem_GetEventObdDTC(EventId);
            if (DEM_DTC_OBD_INVALID != dtc) /* PRQA S 1258 */ /* VL_Dem_1258 */
            {
                /** copy valid DTC number to out parameter */
                *DTCOfEvent = (uint32)(dtc << 8);
                ret         = E_OK;
            }
            else
#endif
            {
                ret = DEM_E_NO_DTC_AVAILABLE;
            }
            break;
        case DEM_DTC_FORMAT_OBD_3BYTE:
#if (DEM_SUPPORTED_OBD_UDS_DTC_SEPARATION == STD_ON)
            dtc = Dem_GetEventObdDTC(EventId);
            if (DEM_DTC_OBD_INVALID != dtc) /* PRQA S 1258 */ /* VL_Dem_1258 */
            {
                /** copy valid DTC number to out parameter */
                *DTCOfEvent = (uint32)dtc;
                ret         = E_OK;
            }
            else
#endif
            {
                ret = DEM_E_NO_DTC_AVAILABLE;
            }
            break;
#endif
        default:
            /** MISRA case */
            break;
        }
    }
    return ret;
}

/**
 * @brief Set DTC WIRS tatus
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
DEM_LOCAL Std_ReturnType Dem_DTCSetWIRStatus(Dem_EventIdType EventId, boolean WIRStatus)
{
    Std_ReturnType ret = E_NOT_OK;
    if ((Dem_GetAvailableInVariantByEvent(EventId) == TRUE) && (Dem_GetEventAvailableStatusByEventId(EventId) == TRUE)
        && (Dem_CheckEventDTCSettingState(EventId) == FALSE))
    {
        Dem_EventInternalStatusType internalStatus;
        if (Dem_GetEventWirActiveStatus(EventId) != WIRStatus)
        {
            Dem_UdsStatusByteType oldDtcStatus;
            Dem_UdsStatusByteType newDtcStatus;
            /** override status has to be changed */
            newDtcStatus = Dem_GetDTCUDSStatus(EventId);
            oldDtcStatus = Dem_DTCApplyExternalStatus(EventId, newDtcStatus);

            /**  Enter Critical Section: DiagMonitor */
            Dem_EnterCritical_DiagMonitor();
            /** update internal status bit */
            internalStatus = Dem_GetEventInternalStatusByEvent(EventId);
            if (WIRStatus == FALSE)
            {
                internalStatus = Dem_ResetEventWirActiveStatus(internalStatus);
            }
            else
            {
                internalStatus = Dem_SetEventWirActiveStatus(internalStatus);
            }
            Dem_SetEventInternalStatusByEvent(EventId, internalStatus);
            Dem_LeaveCritical_DiagMonitor();
            /** Leave Critical Section: DiagMonitor */

            newDtcStatus = Dem_DTCApplyExternalStatus(EventId, newDtcStatus);
            Dem_NotiDtcStatusChanged(EventId, oldDtcStatus, newDtcStatus);
        }
        ret = E_OK;
    }
    return ret;
}
/* PRQA S 6070 -- */

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif
