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
 **  @file               : Dem_DtcInterface.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : Declaration file of Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_DTC_INTERFACE_H
#define DEM_DTC_INTERFACE_H

/* =================================================== inclusions =================================================== */
#include "Dem_PBcfg.h"

/* ========================================== internal function declarations ======================================== */
/**
 * @brief         Get DemDTCAttributesRef by event
 * @param[in]     EventId: Event ID
 * @return        Dem_DTCAttRefNumType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_DTCAttRefNumType Dem_GetDTCAttr(Dem_EventIdType EventId);

/**
 * @brief         Check DemDTCAttributesRef by event
 * @param[in]     DTCAttr: The reference DTC attribute
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckDTCAttr(Dem_DTCAttRefNumType DTCAttr);

/**
 * @brief         Get Memory Destination reference
 * @param[in]     DTCAttr: The reference DTC attribute
 * @return        Dem_MemoryNumType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_MemoryNumType Dem_GetMemRef(Dem_DTCAttRefNumType DTCAttr);

#if (DEM_OBDII_SUPPORT == STD_ON)
/**
 * @brief         Get Event Obd DTC value
 * @param[in]     EventId: Event ID
 * @return        uint32
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint32 Dem_GetEventObdDTC(Dem_EventIdType EventId);
#endif

#if (DEM_J1939_SUPPORT == STD_ON)
/**
 * @brief         Get Event J1939 DTC value
 * @param[in]     EventId: Event ID
 * @return        uint32
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint32 Dem_GetEventJ1939DTC(Dem_EventIdType EventId);

/**
 * @brief         Check Event J1939 Related
 * @param[in]     EventId: Event ID
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckEventJ1939Related(Dem_EventIdType EventId);
#endif

/**
 * @brief         Initialization DTC context
 * @param[in]     EventId: Event ID
 * @param[in]     DTCContext: The DTC context
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_InitDTCContext(Dem_EventIdType EventId, Dem_DTCContextType* const DTCContext);
/**
 * @brief Calculate DTC Status
 */
DEM_LOCAL_INLINE void Dem_CalculateDTCStatus(Dem_DTCContextType* const DTCContext);

/**
 * @brief         This function applies a filter to the DTC status byte to set bits that are forced to true by different
 * mechanisms than event status modification. Also, status bits not contained in the status availability mask are masked
 * out.
 * @param[in]     EventId: Event ID
 * @param[in]     DtcStatus: The DTC status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType
    Dem_DTCApplyExternalStatus(Dem_EventIdType EventId, Dem_UdsStatusByteType DtcStatus);

/**
 * @brief         The following callbacks are called here: - DtcStatusChanged(1..*) - DcmDtcStatusChanged(1) -
 * J1939DcmDtcStatusChanged(1)
 * @param[in]     EventId: Event ID
 * @param[in]     DtcStatusOld: The old DTC status
 * @param[in]     DtcStatusNew: The old DTC status
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_NotiDtcStatusChanged(
    Dem_EventIdType       EventId,
    Dem_UdsStatusByteType DtcStatusOld,
    Dem_UdsStatusByteType DtcStatusNew);

/**
 * @brief Tests all options due to which an event or combined event group has to be has to be excluded from reporting
 API, based on the combined event group for combined events. Any event of a combined group can be passed to this
 function.
 * @param[in]     EventId: Event ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckDTCSuppressed(Dem_EventIdType EventId);

#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
/**
 * @brief         Calculate Combine DTC Status
 * @param[in]     GroupId: Group ID
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_CalculateCombineUdsStatus(Dem_CombinationNumType GroupId);
#endif

/**
 * @brief         This function retrieves the event's status byte for normal events, and the combined event group's
 * status byte for combined events.
 * @param[in]     EventId: Event ID
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_GetDTCUDSStatus(Dem_EventIdType EventId);

#if (DEM_GENERAL_DISPLACEMENT_NONE == STD_OFF)
/**
 * @brief         Updates the status of a displaced DTC
 * @param[in]     EventId: Event ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_DTCDisplaced(Dem_EventIdType EventId);
#endif

/**
 * @brief         Get Event DTC NvStorageStrategy
 * @param[in]     EventId: Event ID
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_GetEventDTCNvImmediate(Dem_EventIdType EventId);

/**
 * @brief         Check Combine DTC Status
 * @param[in]     EventId: Event ID
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_CheckCombineUdsStatus(Dem_UdsStatusByteType DtcStatus);

/**
 * @brief         This function retrieves the event's internal status byte for normal events, and the group's internal
 * status byte for combined events.
 * @param[in]     EventId: Event ID
 * @return        Dem_EventInternalStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventInternalStatusType Dem_GetDtcInterStatus(Dem_EventIdType EventId);

/**
 * @brief         This function stores the event's internal status byte for normal events, and the group's internal
 * status byte for combined events.
 * @param[in]     EventId: Event ID
 * @param[in]     Status: The event status
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetDtcInterStatus(Dem_EventIdType EventId, Dem_EventInternalStatusType Status);

/**
 * @brief         Get Event DTC value
 * @param[in]     EventId: Event ID
 * @return        uint32
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint32 Dem_GetEventDTC(Dem_EventIdType EventId);

#if (                                                                                                     \
    (DEM_ENTRY_TRIGGER_ON_FDC_THRESHOLD == STD_ON) || (DEM_GENERAL_ED_TRIGGER_ON_FDC_THRESHOLD == STD_ON) \
    || (DEM_GENERAL_FF_TRIGGER_ON_FDC_THRESHOLD == STD_ON))
/**
 * @brief         Updates event data due to exceeding a FDC threshold
 * @param[in]     EventId: Event ID
 * @param[in]     FirstThisCycle: First time occurs in this opration Cycle
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DTCFdcTrip(Dem_EventIdType EventId, boolean FirstThisCycle);
#endif

#if (defined(DEM_MAX_FDC_SINCE_LAST_CLEAR))
/**
 * @brief         Update event data 'maximum FDC since last clear'
 * @param[in]     EventId: Event ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DTCUpdateFdcMax(Dem_EventIdType EventId);
#endif

/**
 * @brief         Process DTC triggers on 'Passed' status report.
 * @param[in]     DTCContext: The DTC context
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DTCPassedProcessDTCEffects(const Dem_DTCContextType* DTCContext);

/**
 * @brief         Calculate DTC triggers
 * @param[in]     DTCContext: The DTC context
 * @param[in]     Eventtriggers: The event trigger type
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_DTCCalculateFailedEffects(Dem_DTCContextType* const DTCContext, uint8 Eventtriggers);

/**
 * @brief Calculate storage dependent DTC triggers on 'Failed' status report.
 * @param[in]     DTCContext: The DTC context
 * @param[in]     Eventtriggers: The event trigger type
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_DTCCalculateFailedEffectsOnStorage(Dem_DTCContextType* const DTCContext, uint8 Eventtriggers);

/**
 * @brief         Process Dtc storage into event memory
 * @param[in]     EventContext: The event context
 * @param[in]     DTCContext: The DTC context
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DTCProcessStorage(Dem_EventContextType* EventContext, Dem_DTCContextType* DTCContext);

/**
 * @brief         Due to excessive code duplication it was necessary to fall back to higher order functions. This
 * function takes a function pointer to an operation to be executed for all sub-events of a combined event. For normal
 * events, the operation is executed for that event only, so the caller does not need to distinguish between combined
 * and non-combined events too much. The caller is still reponsible to call this function only once per combined event,
 * e.g. by calling it only for the master event.
 * @param[in]     EventId: Event ID
 * @param[in]     FuncPtr: Point to function
 * @param[in]     ArgPtr: Point to argument
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_CombinedEvent(
    Dem_EventIdType                EventId,
    const Dem_UtilCombinedFuncType FuncPtr,
    const Dem_CombinedArgPtrType   ArgPtr);

/**
 * @brief         For normal events, this function simply looks up the event configuration. A combined event group is
 * available only if at least one sub-event is available in the active variant. Any event of a combined group can be
 * passed to this function.
 * @param[in]     EventId: Event ID
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckDTCAvailableInVariant(Dem_EventIdType EventId);

#if (DEM_AVAILABILITY_SUPPORT == DEM_EVENT_AVAILABILITY)
/**
 * @brief         Unavailable the DTC's event. If the DTC has a combined group, disconnect all its subevents.
 * @param[in]     EventId: Event ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_DTCDisable(Dem_EventIdType EventId);

/**
 * @brief         Available the DTC's event. If the DTC has a combined group, reconnect all its subevents.
 * @param[in]     EventId: Event ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_DTCEnable(Dem_EventIdType EventId);
#endif

#if (DEM_AGING_ENABLE == STD_ON)
/**
 * @brief Processes the aging cycle for a DTC
 * @param[in]     EventId: Event ID
 * @param[in]     CycleCounter: Cycle Counter
 * @param[in]     MemoryIndex: Memory index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DTCAgingCycle(Dem_EventIdType EventId, uint16 CycleCounter, Dem_NvBlockNumType MemoryIndex);

/**
 * @brief Processes aging at the end of the DTC's operation cycle
 * @param[in]     EventId: Event ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_DTCOperationCyclEndAging(Dem_EventIdType EventId);
#endif

/**
 * @brief         Update the UDS status of DTC due to operation cycle state change
 * @param[in]     EventId: Event ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_DTCOperationCycle(Dem_EventIdType EventId);

#if (DEM_SUPPRESSION_SUPPORT == DEM_DTC_SUPPRESSION)
/**
 * @brief         Tests if an event or combined event group is suppressed for reporting API. Any event of a combined
 * group can be passed to this function.
 * @param[in]     EventId: Event ID
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckDTCSuppression(Dem_EventIdType EventId);

/**
 * @brief         Change suppression of a DTC
 * @param[in]     EventId: Event ID
 * @param[out]    SuppressionStatus: Defines whether the respective DTC is suppressed (TRUE) or enabled (FALSE).
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_ChangeDTCSuppression(Dem_EventIdType EventId, boolean SuppressionStatus);
#endif

/**
 * @brief       Gets the FaultDetectionCounter (FDC) of a DTC
 * @param[in]     EventId: Event ID
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
DEM_LOCAL_INLINE Std_ReturnType Dem_DTCGetFaultDetectionCounter(Dem_EventIdType EventId, sint8* FaultDetectionCounter);

#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
/**
 * @brief         Calculates the maximum fault detection counter this cycle of a combined event
 * @param[in]     GroupId: Group ID
 * @return        sint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL sint8 Dem_GetFDCMaxOfDTCCombined(Dem_CombinationNumType GroupId);
#endif

/**
 * @brief         Check whether a memory entry exists for the DTC
 * @param[in]     EventId: Event ID
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_DTCCheckStoredStatus(Dem_EventIdType EventId);
#if (DEM_DTC_GROUP_NUMBER > 0u)
/**
 * @brief Get Dem Group Of DTC
 */
DEM_LOCAL_INLINE uint32 Dem_GetDTCOfGroupDTCs(Dem_GroupOfDTCNumType GroupId);
#endif

#endif
