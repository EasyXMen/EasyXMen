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
 **  @file               : Dem_EventInterface.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : Declaration file of Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_EVENT_INTERFACE_H
#define DEM_EVENT_INTERFACE_H

/* =================================================== inclusions =================================================== */
#include "Dem_Lcfg.h"

/* ===================================================== macros ===================================================== */
/**
 * @name  DEM_EVENT_ID_STATUS
 * @{
 * @brief Macros encoding status of an event
 */
#define DEM_EVENT_INVALID        ((Dem_EventIdType)0x0000u)   /**< EventId which is not used */
#define DEM_EVENT_FIRST          ((Dem_EventIdType)0x0001u)   /**< EventId which is the first valid event */
#define DEM_EVENT_STATUS_INVALID ((Dem_EventStatusType)0x05u) /**< EventStatus which is not used */
/** @} */

/**
 * @name  DEM_INTERNAL_STATUS_ENCODE
 * @{
 * @brief Event internal status mask
 */
#define DEM_INTERNAL_STATUS_STORED            ((Dem_EventInternalStatusType)0x03u) /**< stored status bit */
#define DEM_INTERNAL_STATUS_SUPPRESSED        ((Dem_EventInternalStatusType)0x04u) /**< event(DTC) suppressed bit */
#define DEM_INTERNAL_STATUS_AVAILABLE         ((Dem_EventInternalStatusType)0x08u) /**< event availavle bit */
#define DEM_INTERNAL_STATUS_AVAILABLE_CHANGED ((Dem_EventInternalStatusType)0x10u) /**< event changed availavle bit */
#define DEM_INTERNAL_STATUS_FDCTOC            ((Dem_EventInternalStatusType)0x20u) /**< FDC this cycle bit */
#define DEM_INTERNAL_STATUS_AVAILVARIANT      ((Dem_EventInternalStatusType)0x40u) /**< Combined event available bit */
#define DEM_INTERNAL_STATUS_WIRCONTROLL       ((Dem_EventInternalStatusType)0x80u) /**< user controlled WIR bit */
/** @} */

#define DEM_UDS_STATUS_INITIALIZE (0x50u) /**< UDS status after re-initialization */

/* ================================================ type definitions ================================================ */
/**
 * @brief Enum to encode stored status states
 */
typedef enum
{
    Dem_InternalStoredStatus_None,   /**< Not stored @range 0u */
    Dem_InternalStoredStatus_Active, /**< Active memory entry exists @range 1u */
    Dem_InternalStoredStatus_Aging,  /**< Aging memory entry exists @range 2u */
    Dem_InternalStoredStatus_Aged,   /**< Aged memory entry exists @range 3u */
} Dem_InternalStoredStatusType;

/* ========================================== internal function declarations ======================================== */
/**
 * @brief         Get the number of all events, including invalid events #0
 * @return        Dem_EventIdType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventIdType Dem_GetEventNumber(void);
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
DEM_LOCAL_INLINE boolean Dem_CheckEventIdInValid(Dem_EventIdType EventId);

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
DEM_LOCAL_INLINE boolean Dem_GetAvailableInVariantByEvent(Dem_EventIdType EventId);

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
DEM_LOCAL_INLINE boolean Dem_GetEventAvailableStatusByEventId(Dem_EventIdType EventId);

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
DEM_LOCAL_INLINE uint8 Dem_GetEventPriority(Dem_EventIdType EventId);

#if (defined(DEM_EVENT_MONOTOR_DEBOUNCCE))
/* PRQA S 5016 ++ */ /* VL_Dem_5016 */
/**
 * @brief         Get event Get Fault Detection Counter Fnc based on event ID
 * @param[in]     EventId: Event ID
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE DemGetFaultDetectionCounterType Dem_GetDebounceGetFDC(Dem_EventIdType EventId);
/* PRQA S 5016 -- */
#endif

/**
 * @brief         Pre-initialization event available state
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_EventPreInit(void);

/**
 * @brief         Reset the Uds Status bit of the event
 * @param[in]     EventId: Event ID
 * @param[in]     UdsStatus: UDS status
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_EventResetUdsStatus(Dem_EventIdType EventId, Dem_UdsStatusByteType UdsStatus);

#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
/**
 * @brief         Check Event Status Block need to store
 * @param[in]     OldEventStatus: Old event status
 * @param[in]     NewEventStatus: New event status
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean
    Dem_CheckNvRelevantStatus(Dem_UdsStatusByteType OldEventStatus, Dem_UdsStatusByteType NewEventStatus);
#endif

#if (DEM_COMBINATION_NUMBER > 0u)
/**
 * @brief         Pre-initialization Combined event
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_CombinedEventPreInit(void);

/**
 * @brief         Get Combined event internal status based on Group ID
 * @param[in]     GroupId: Group Id
 * @return        Dem_EventInternalStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventInternalStatusType Dem_GetCombinedEventStatusByGroupId(Dem_CombinationNumType GroupId);

/**
 * @brief         Set Combined event internal status based on Group ID
 * @param[in]     GroupId: Group Id
 * @param[in]     Status: Event status
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetCombinedEventStatusByGroupId(Dem_CombinationNumType GroupId, uint8 Status);

/**
 * @brief         Initialization Combined event
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_CombinedEventInit(void);
#endif

/**
 * @brief         Check event stored based on event ID
 * @param[in]     EventId: Event ID
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_GetEventstored(Dem_EventIdType EventId);

#if (DEM_GENERAL_DISPLACEMENT_NONE == STD_OFF)
/**
 * @brief         Updates an event's status due to displacement.Do not call this function directly, it is part of
 * Dem_DTCDisplaced
 * @param[in]     EventId: Event ID
 * @param[in]     ArgPtr: Pointer to argument
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_EventDisplaced(Dem_EventIdType EventId, const Dem_CombinedArgPtrType ArgPtr);
#endif

/******************UDS Status*******************/
/**
 * @brief         Get Event UDS Status
 * @param[in]     EventId: Event ID
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_GetEventUDSStatus(Dem_EventIdType EventId);

/**
 * @brief         Check whether the TF bit of the UDS status bit is set or not
 * @param[in]     Status: UDS status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_UDSStatusCheckTF(Dem_UdsStatusByteType Status);

/**
 * @brief         Set the TF bit of the UDS status bit
 * @param[in]     Status: UDS status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_UDSStatusSetTF(Dem_UdsStatusByteType Status);

/**
 * @brief         Reset the TF bit of the UDS status bit
 * @param[in]     Status: UDS status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_UDSStatusResetTF(Dem_UdsStatusByteType Status);

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
DEM_LOCAL_INLINE boolean Dem_UDSStatusCheckTFTOC(Dem_UdsStatusByteType Status);

/**
 * @brief         Set the TFTOC bit of the UDS status bit
 * @param[in]     Status: UDS status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_UDSStatusSetTFTOC(Dem_UdsStatusByteType Status);

/**
 * @brief         Reset the TFTOC bit of the UDS status bit
 * @param[in]     Status: UDS status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_UDSStatusResetTFTOC(Dem_UdsStatusByteType Status);

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
DEM_LOCAL_INLINE boolean Dem_UDSStatusCheckPDTC(Dem_UdsStatusByteType Status);
/**
 * @brief         Set the PDTC bit of the UDS status bit
 * @param[in]     Status: UDS status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_UDSStatusSetPDTC(Dem_UdsStatusByteType Status);

/**
 * @brief         Reset the PDTC bit of the UDS status bit
 * @param[in]     Status: UDS status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_UDSStatusResetPDTC(Dem_UdsStatusByteType Status);

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
DEM_LOCAL_INLINE boolean Dem_UDSStatusCheckCDTC(Dem_UdsStatusByteType Status);

/**
 * @brief         Set the CDTC bit of the UDS status bit
 * @param[in]     Status: UDS status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_UDSStatusSetCDTC(Dem_UdsStatusByteType Status);

/**
 * @brief         Reset the CDTC bit of the UDS status bit
 * @param[in]     Status: UDS status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_UDSStatusResetCDTC(Dem_UdsStatusByteType Status);

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
DEM_LOCAL_INLINE boolean Dem_UDSStatusCheckTNCSLC(Dem_UdsStatusByteType Status);

/**
 * @brief         Set the TNCSLC bit of the UDS status bit
 * @param[in]     Status: UDS status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_UDSStatusSetTNCSLC(Dem_UdsStatusByteType Status);

/**
 * @brief         Reset the TNCSLC bit of the UDS status bit
 * @param[in]     Status: UDS status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_UDSStatusResetTNCSLC(Dem_UdsStatusByteType Status);

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
DEM_LOCAL_INLINE boolean Dem_UDSStatusCheckTFSLC(Dem_UdsStatusByteType Status);

/**
 * @brief         Set the TFSLC bit of the UDS status bit
 * @param[in]     Status: UDS status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_UDSStatusSetTFSLC(Dem_UdsStatusByteType Status);

/**
 * @brief         Reset the TFSLC bit of the UDS status bit
 * @param[in]     Status: UDS status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_UDSStatusResetTFSLC(Dem_UdsStatusByteType Status);

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
DEM_LOCAL_INLINE boolean Dem_UDSStatusCheckTNCTOC(Dem_UdsStatusByteType Status);

/**
 * @brief         Set the TNCTOC bit of the UDS status bit
 * @param[in]     Status: UDS status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_UDSStatusSetTNCTOC(Dem_UdsStatusByteType Status);

/**
 * @brief         Reset the TNCTOC bit of the UDS status bit
 * @param[in]     Status: UDS status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_UDSStatusResetTNCTOC(Dem_UdsStatusByteType Status);

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
DEM_LOCAL_INLINE boolean Dem_UDSStatusCheckWIR(Dem_UdsStatusByteType Status);

/**
 * @brief         Set the WIR bit of the UDS status bit
 * @param[in]     Status: UDS status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_UDSStatusSetWIR(Dem_UdsStatusByteType Status);

/**
 * @brief         Reset the WIR bit of the UDS status bit
 * @param[in]     Status: UDS status
 * @return        Dem_UdsStatusByteType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_UDSStatusResetWIR(Dem_UdsStatusByteType Status);

/**************Dem_EventStatusData******************/
/**
 * @brief         Get First Failed Event
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module
 * @return        Dem_EventIdType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventIdType Dem_GetFirstFailedEvent(uint8 ClientId);

/**
 * @brief         Set First Failed Event
 * @param[in]     EventId: Event ID
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetFirstFailedEvent(Dem_EventIdType EventId, uint8 ClientId);

/**
 * @brief         Get First Confirmed Event
 * @param[in]     EventId: Event ID
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module
 * @return        Dem_EventIdType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventIdType Dem_GetFirstConfirmedEvent(uint8 ClientId);

/**
 * @brief         Set First Confirmed Event
 * @param[in]     EventId: Event ID
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetFirstConfirmedEvent(Dem_EventIdType EventId, uint8 ClientId);

/**
 * @brief         Get Most Recent Failed Event
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module
 * @return        Dem_EventIdType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventIdType Dem_GetMostRecentFailedEvent(uint8 ClientId);

/**
 * @brief         Set Most Recent Failed Event
 * @param[in]     EventId: Event ID
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetMostRecentFailedEvent(Dem_EventIdType EventId, uint8 ClientId);

/**
 * @brief         Get Most Recent Confmd Event
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module
 * @return        Dem_EventIdType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventIdType Dem_GetMostRecentConfirmdEvent(uint8 ClientId);

/**
 * @brief         Set Most Recent Confmd Event
 * @param[in]     EventId: Event ID
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetMostRecentConfirmdEvent(Dem_EventIdType EventId, uint8 ClientId);

/**
 * @brief         processes the stored triggers from debounce values passing the FDC trip threshold.
 * @param[in]     EventId: Event ID
 * @param[in]     FdcThrExceeded: FDC threshold exceed
 * @param[in]     FdcMaximumExceeded: FDC max exceed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_EventProcessFdc(Dem_EventIdType EventId, boolean FdcThrExceeded, boolean FdcMaximumExceeded);

/**
 * @brief         Process the event's queued qualified passed result
 * @param[in]     EventId: Event ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_EventPassed(Dem_EventIdType EventId);

/**
 * @brief         Process the event's queued qualified failed result
 * @param[in]     EventId: Event ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_EventFailed(Dem_EventIdType EventId);

/**
 * @brief         initialization event state
 * @param[in]     EventId: Event ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_EventInit(void);

/**
 * @brief         Set event stored internal status based on event ID
 * @param[in]     Status: Event status
 * @param[in]     StoredStatus: Stored status
 * @return        Dem_EventInternalStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventInternalStatusType
    Dem_SetEventstoredStatus(Dem_EventInternalStatusType Status, Dem_InternalStoredStatusType StoredStatus);

#if (DEM_EVENT_FAILURE_COUNTER == STD_ON)
/**
 * @brief         Get Event Failure Counter
 * @param[in]     EventId: Event ID
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetEventFailureCounter(Dem_EventIdType EventId);

/**
 * @brief         Set Event Failure Counter
 * @param[in]     EventId: Event ID
 * @param[in]     FailureCounter: Failure counter
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetEventFailureCounter(Dem_EventIdType EventId, uint8 FailureCounter);
#endif

/**
 * @brief         Event Data Changed notification
 * @param[in]     EventId: Event ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_NotiEventDataChanged(Dem_EventIdType EventId);

#if (DEM_EVENT_CALLBACK_CLEAR_ALLOWED > 0u)
/**
 * @brief         If the out-parameter Allowed of the callback DemClearEventAllowed returns false and the configuration
 * parameter DemClearEventAllowedBehavior is set to DEM_ONLY_THIS_CYCLE_AND_READINESS, the related UDS status bits 1
 * (TestFailedThisOperationCycle), 4 (TestNotCompletedSinceLastClear), 5 (TestFailedSinceLastClear), and 6
 * (TestNotCompletedThisOperationCycle) shall be reset.
 * @param[in]     EventId: Event ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_EventClearReset(Dem_EventIdType EventId);
#endif

/**
 * @brief         Calls all configured 'init monitor for event' callbacks.
 * @param[in]     EventId: Event ID
 * @param[in]     InitReason: Init monitor reason
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_NotiInitMonitorForEvent(Dem_EventIdType EventId, Dem_InitMonitorReasonType InitReason);

/**
 * @brief         Clears an event's state in volatile RAM
 * @param[in]     EventId: Event ID
 * @param[in]     ArgPtr: Pointer to argument
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_EventCleared(Dem_EventIdType EventId, const Dem_CombinedArgPtrType ArgPtr);

#if (DEM_AVAILABILITY_SUPPORT == DEM_EVENT_AVAILABILITY)
/**
 * @brief         Process queued event availability changes
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_EventTask(void);

/**
 * @brief         Check precondition for event unavailable
 * @param[in]     EventId: Event ID
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_EventMayDisable(Dem_EventIdType EventId);
#endif

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
DEM_LOCAL_INLINE boolean Dem_GetEventAvailableStatus(Dem_EventInternalStatusType Status);

/**
 * @brief         Get event stored internal status based on event ID
 * @param[in]     Status: Event status
 * @return        boolean
 * @retval        Dem_InternalStoredStatus_None: Not stored
 * @retval        Dem_InternalStoredStatus_Active: Active memory entry exists
 * @retval        Dem_InternalStoredStatus_Aging: Aging memory entry exists
 * @retval        Dem_InternalStoredStatus_Aged: Aged memory entry exists
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_InternalStoredStatusType Dem_GetEventstoreStatus(Dem_EventInternalStatusType Status);

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
DEM_LOCAL_INLINE boolean Dem_GetDebounceBehaviorByEvent(Dem_EventIdType EventId);

#if (DEM_AGING_ENABLE == STD_ON)
/**
 * @brief         Get Event Aging Cycle
 * @param[in]     EventId: Event ID
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetEventAgingCycle(Dem_EventIdType EventId);
#endif

/**
 * @brief         Initializes the event status
 * @param[in]     EventId: Event ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_EventInitNvData(Dem_EventIdType EventId);

#if (DEM_EVENT_HEALING_COUNTER == STD_ON)
/**
 * @brief         Set Event Healing Counter
 * @param[in]     EventId: Event ID
 * @param[in]     HealingCounter: Healing counter
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetEventHealingCounter(Dem_EventIdType EventId, uint8 HealingCounter);

#endif
/**
 * @brief         Set Event UDS Status
 * @param[in]     EventId: Event ID
 * @param[in]     Status: Event status
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetEventUDSStatus(Dem_EventIdType EventId, Dem_UdsStatusByteType Status);

#if (DEM_AGING_ENABLE == STD_ON)
/**
 * @brief         Get Event Aging Threshold
 * @param[in]     EventId: Event ID
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetEventAgingThreshold(Dem_EventIdType EventId);

/**
 * @brief         Get Event max Aging Threshold
 * @param[in]     EventId: Event ID
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetEventMaxAgingThreshold(Dem_EventIdType EventId);
#endif

#if (defined(DEM_EVENT_ASSOCIATED_IDENTIFICATION))
/**
 * @brief         Get DemEventAssociatedIdentification information based on event ID
 * @param[in]     EventId: Event ID
 * @return        Dem_EventIdType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventIdType Dem_GetAssociatedIdByEvent(Dem_EventIdType EventId);
#endif

/**
 * @brief         Get Event Confirmation Threshold
 * @param[in]     EventId: Event ID
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetEventConfirmationThreshold(Dem_EventIdType EventId);

#if (                                                                                                     \
    (DEM_ENTRY_TRIGGER_ON_FDC_THRESHOLD == STD_ON) || (DEM_GENERAL_ED_TRIGGER_ON_FDC_THRESHOLD == STD_ON) \
    || (DEM_GENERAL_FF_TRIGGER_ON_FDC_THRESHOLD == STD_ON))
/**
 * @brief         Reset event fdc max this cycle in the internal status
 * @param[in]     Status: Event status
 * @return        Dem_EventInternalStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventInternalStatusType Dem_ResetEventFdcTocStatus(Dem_EventInternalStatusType Status);
#endif
/**
 * @brief         Get event internal status based on event ID
 * @param[in]     EventId: Event ID
 * @return        Dem_EventInternalStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventInternalStatusType Dem_GetEventInternalStatusByEvent(Dem_EventIdType EventId);

/**
 * @brief         Set event internal status based on event ID
 * @param[in]     EventId: Event ID
 * @param[in]     Status: Event status
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetEventInternalStatusByEvent(Dem_EventIdType EventId, Dem_EventInternalStatusType Status);

/**
 * @brief         Check if  Event have Special Indicator based on event ID
 * @param[in]     EventId: Event ID
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckEventHaveSpecialIndicator(Dem_EventIdType EventId);
#endif
