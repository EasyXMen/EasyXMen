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
 **  @file               : Dem_Queue.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : Implementation file of Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_QUEUE_H
#define DEM_QUEUE_H

/* =================================================== inclusions =================================================== */
#include "Dem_SatelliteInterface.h"
#include "Dem_PreStore.h"
#include "Dem_EventInterface.h"

/* ===================================================== macros ===================================================== */
/**
 * @name  DEM_EVENTQUEUE_ENCODE
 * @{
 * @brief Dem_EventQueueStatusType: Macros encoding queue states
 */
#define DEM_EVENTQUEUESTATE_INITIAL         (0x0u)  /**< Initial queue value. */
#define DEM_EVENTQUEUE_PASSED               (0x1u)  /**< Queued trigger: Passed */
#define DEM_EVENTQUEUE_PASSED_FAILED        (0x2u)  /**< Queued trigger: Passed and Failed */
#define DEM_EVENTQUEUE_PASSED_FAILED_PASSED (0x3u)  /**< Queued trigger: Passed, Failed and Passed */
#define DEM_EVENTQUEUE_FAILED               (0x4u)  /**< Queued trigger: Failed */
#define DEM_EVENTQUEUE_FAILED_PASSED        (0x5u)  /**< Queued trigger: Failed and Passed */
#define DEM_EVENTQUEUE_FAILED_PASSED_FAILED (0x6u)  /**< Queued trigger: Failed, Passed and Failed */
#define DEM_EVENTQUEUE_ACTION               (0x07u) /**< Queued trigger status mask */
#define DEM_EVENTQUEUE_ACTION_MASK          (0xf8u) /**< other status except Queued trigger status mask */
#define DEM_EVENTQUEUE_FDC_TRIP             (0x08U) /**< Fdc Threshold Exceeded */
#define DEM_EVENTQUEUE_FDC_MAX_SLC          (0x10U) /**< Fdc Maximum Exceeded */
#define DEM_EVENTQUEUE_FDC_PROCESSING       (0x20U) /**< Fdc Processing Request */
#define DEM_EVENTQUEUE_RESET_TF             (0x40U) /**< Reset Test Failed Requested */
#define DEM_EVENTQUEUE_RSERVE               (0x80U) /**< rserve */
/** @} */

/* ========================================== internal function declarations ======================================== */
/**
 * @brief         Check Queue Overflow
 * @param[in]     QueueStatus: status of queue
 * @param[in]     MonitorTrigger: Monitor trigger
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_QueueCheckQueueOverflow(Dem_EventQueueStatusType QueueStatus, Dem_EventStatusType MonitorTrigger);

/**
 * @brief         Calculate queue status based on event status
 * @param[in]     QueueStatus: status of queue
 * @param[in]     MonitorTrigger: Monitor trigger
 * @return        Dem_EventQueueStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Dem_EventQueueStatusType
    Dem_QueueCalculateQueueStatus(Dem_EventQueueStatusType QueueStatus, Dem_EventStatusType MonitorTrigger);

/**
 * @brief         Queued Event process by the action type
 * @param[in]     EventId: Event Id
 * @param[in]     QueueStatus: status of queue
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_ProcessQueuedEventActions(Dem_EventIdType EventId, Dem_EventQueueStatusType QueueStatus);

/**
 * @brief         Queued Event process in Satellite
 * @param[in]     SatelliteId: Satellite Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_EventQueuedProcess(Dem_SatelliteIdType SatelliteId);

/**
 * @brief         Event Queue Task process
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_EventQueueTask(void);

/**
 * @brief         Initialize the subcomponent Event Queue
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_EventQueueInit(void);

#if (DEM_PROCESS_EVENT_LIMIT_NUMBER > 0u)
/* ============================================ internal data definitions =========================================== */
#define DEM_START_SEC_VAR_CLEARED_16
#include "Dem_MemMap.h"
/**
 * @brief Last SatelliteId
 * @range 0..65535
 */
DEM_LOCAL Dem_SatelliteIdType Dem_LastSatelliteId;

/**
 * @brief Last MonitorId
 * @range 0..65535
 */
DEM_LOCAL Dem_MonitorIdType Dem_LastMonitorId;

/**
 * @brief Processed Event Number in this mainfunction
 * @range 0..65535
 */
DEM_LOCAL Dem_EventIdType Dem_ProcessedEventNum;
#define DEM_STOP_SEC_VAR_CLEARED_16
#include "Dem_MemMap.h"
#endif

/* ========================================== internal function definitions ========================================= */
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/* PRQA S 0759 ++ */ /* VL_Dem_0759 */
/**
 * @brief         Reset QueueStatus
 * @param[in]     SatelliteData: Satellite Data
 * @return        Dem_SatelliteDataType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_SatelliteDataType Dem_QueueResetQueueStatus(Dem_SatelliteDataType SatelliteData)
{
    return Dem_SetSatelliteQueueStatus(SatelliteData, DEM_EVENTQUEUESTATE_INITIAL);
}

/**
 * @brief         Get QueueStatus
 * @param[in]     SatelliteData: Satellite Data
 * @return        Dem_EventQueueStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventQueueStatusType Dem_QueueGetQueueStatus(Dem_SatelliteDataType SatelliteData)
{
    return Dem_GetSatelliteQueueStatus(SatelliteData);
}
/* PRQA S 0759 -- */

/**
 * @brief         Check FdcTrip Status of QueueStatus
 * @param[in]     QueueStatus: status of queue
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_QueueCheckFdcTripStatus(Dem_EventQueueStatusType QueueStatus)
{
    return DEM_FLAG_ISSET(QueueStatus, DEM_EVENTQUEUE_FDC_TRIP); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/* PRQA S 3473 ++ */ /* VL_Dem_3473 */
/**
 * @brief         Set FdcTrip Status of QueueStatus
 * @param[in]     QueueStatus: status of queue
 * @return        Dem_EventQueueStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventQueueStatusType Dem_QueueSetFdcTripStatus(Dem_EventQueueStatusType QueueStatus)
{
    Dem_EventQueueStatusType state = QueueStatus;
    DEM_FLAG_SET(state, DEM_EVENTQUEUE_FDC_TRIP);
    return state;
}

/**
 * @brief         Check Fdc Max SLC Status of QueueStatus
 * @param[in]     QueueStatus: status of queue
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_QueueCheckFdcMaxSLCStatus(Dem_EventQueueStatusType QueueStatus)
{
    return DEM_FLAG_ISSET(QueueStatus, DEM_EVENTQUEUE_FDC_MAX_SLC); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/**
 * @brief         Set Fdc Max SLC Status of QueueStatus
 * @param[in]     QueueStatus: status of queue
 * @return        Dem_EventQueueStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventQueueStatusType Dem_QueueSetFdcMaxSLCStatus(Dem_EventQueueStatusType QueueStatus)
{
    Dem_EventQueueStatusType state = QueueStatus;
    DEM_FLAG_SET(state, DEM_EVENTQUEUE_FDC_MAX_SLC);
    return state;
}

/**
 * @brief         Check Fdc Processing Status of QueueStatus
 * @param[in]     QueueStatus: status of queue
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_QueueCheckFdcProcessingStatus(Dem_EventQueueStatusType QueueStatus)
{
    return DEM_FLAG_ISSET(QueueStatus, DEM_EVENTQUEUE_FDC_PROCESSING); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/**
 * @brief         Set Fdc Processing Status of QueueStatus
 * @param[in]     QueueStatus: status of queue
 * @return        Dem_EventQueueStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventQueueStatusType Dem_QueueSetFdcProcessingStatus(Dem_EventQueueStatusType QueueStatus)
{
    Dem_EventQueueStatusType state = QueueStatus;
    DEM_FLAG_SET(state, DEM_EVENTQUEUE_FDC_PROCESSING);
    return state;
}

/**
 * @brief         Check Reset Test Failed Requested of QueueStatus
 * @param[in]     QueueStatus: status of queue
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_QueueCheckResetTFRequested(Dem_EventQueueStatusType QueueStatus)
{
    return DEM_FLAG_ISSET(QueueStatus, DEM_EVENTQUEUE_RESET_TF); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/**
 * @brief         Set Reset Test Failed Requested of QueueStatus
 * @param[in]     QueueStatus: status of queue
 * @return        Dem_EventQueueStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventQueueStatusType Dem_QueueSetResetTFRequested(Dem_EventQueueStatusType QueueStatus)
{
    Dem_EventQueueStatusType state = QueueStatus;
    DEM_FLAG_SET(state, DEM_EVENTQUEUE_RESET_TF);
    return state;
}
/* PRQA S 3473 -- */

/**
 * @brief         Get Queued trigger status of QueueStatus
 * @param[in]     QueueStatus: status of queue
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_QueueGetQueueAction(Dem_EventQueueStatusType QueueStatus)
{
    return (uint8)(QueueStatus & DEM_EVENTQUEUE_ACTION);
}

/**
 * @brief         Set Queued trigger status of QueueStatus
 * @param[in]     QueueStatus: status of queue
 * @param[in]     Action: Action of queue
 * @return        Dem_EventQueueStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventQueueStatusType Dem_QueueSetQueueAction(Dem_EventQueueStatusType QueueStatus, uint8 Action)
{
    return (uint8)((QueueStatus & DEM_EVENTQUEUE_ACTION_MASK) | (Action & DEM_EVENTQUEUE_ACTION));
}

/**
 * @brief Check Queue Overflow
 */
DEM_LOCAL boolean Dem_QueueCheckQueueOverflow(Dem_EventQueueStatusType QueueStatus, Dem_EventStatusType MonitorTrigger)
{
    boolean ret = FALSE;
    uint8   queueAction;

    queueAction = Dem_QueueGetQueueAction(QueueStatus);
    if (((MonitorTrigger == DEM_EVENT_STATUS_FAILED) && (queueAction == DEM_EVENTQUEUE_PASSED_FAILED_PASSED))
        || ((MonitorTrigger == DEM_EVENT_STATUS_PASSED) && (queueAction == DEM_EVENTQUEUE_FAILED_PASSED_FAILED)))
    {
        ret = TRUE;
    }
    return ret;
}

/**
 * @brief Calculate queue status based on event status
 */
DEM_LOCAL Dem_EventQueueStatusType
    Dem_QueueCalculateQueueStatus(Dem_EventQueueStatusType QueueStatus, Dem_EventStatusType MonitorTrigger)
{
    Dem_EventQueueStatusType ret;

    if (MonitorTrigger == DEM_EVENT_STATUS_FAILED)
    {
        switch (Dem_QueueGetQueueAction(QueueStatus))
        {
        case DEM_EVENTQUEUESTATE_INITIAL:
            ret = Dem_QueueSetQueueAction(QueueStatus, DEM_EVENTQUEUE_FAILED);
            break;
        case DEM_EVENTQUEUE_PASSED:
        case DEM_EVENTQUEUE_FAILED_PASSED:
            ret = (Dem_EventQueueStatusType)(QueueStatus + 1u);
            break;
        default:
            ret = QueueStatus;
            break;
        }
    }
    else if (MonitorTrigger == DEM_EVENT_STATUS_PASSED)
    {
        switch (Dem_QueueGetQueueAction(QueueStatus))
        {
        case DEM_EVENTQUEUESTATE_INITIAL:
            ret = Dem_QueueSetQueueAction(QueueStatus, DEM_EVENTQUEUE_PASSED);
            break;
        case DEM_EVENTQUEUE_FAILED:
        case DEM_EVENTQUEUE_PASSED_FAILED:
            ret = (Dem_EventQueueStatusType)(QueueStatus + 1u);
            break;
        default:
            ret = QueueStatus;
            break;
        }
    }
    else
    {
        /** Not a qualified result */
        ret = QueueStatus;
    }
    return ret;
}

/**
 * @brief Queued Event process by the action type
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
DEM_LOCAL void Dem_ProcessQueuedEventActions(Dem_EventIdType EventId, Dem_EventQueueStatusType QueueStatus)
/* PRQA S 6070 -- */
{
    if (Dem_QueueCheckResetTFRequested(QueueStatus) == TRUE)
    {
#if (                                                                                                     \
    (DEM_ENTRY_TRIGGER_ON_FDC_THRESHOLD == STD_ON) || (DEM_GENERAL_ED_TRIGGER_ON_FDC_THRESHOLD == STD_ON) \
    || (DEM_GENERAL_FF_TRIGGER_ON_FDC_THRESHOLD == STD_ON))
        Dem_EventInternalStatusType eventInternalStatus;

        /**  Enter Critical Section: DiagMonitor */
        Dem_EnterCritical_DiagMonitor();
        eventInternalStatus = Dem_GetEventInternalStatusByEvent(EventId);
        eventInternalStatus = Dem_ResetEventFdcTocStatus(eventInternalStatus);
        Dem_SetEventInternalStatusByEvent(EventId, eventInternalStatus);
        Dem_LeaveCritical_DiagMonitor();
        /**  Leave Critical Section: DiagMonitor */

#endif

        Dem_EventResetUdsStatus(EventId, DEM_UDS_STATUS_TF);
    }

    if (Dem_QueueCheckFdcProcessingStatus(QueueStatus) == TRUE)
    {
        Dem_EventProcessFdc(
            EventId,
            Dem_QueueCheckFdcTripStatus(QueueStatus),
            Dem_QueueCheckFdcMaxSLCStatus(QueueStatus));
    }

    switch (Dem_QueueGetQueueAction(QueueStatus))
    {
    case DEM_EVENTQUEUE_PASSED:
        Dem_EventPassed(EventId);
        break;
    case DEM_EVENTQUEUE_PASSED_FAILED:
        Dem_EventPassed(EventId);
        Dem_EventFailed(EventId);
        break;
    case DEM_EVENTQUEUE_PASSED_FAILED_PASSED:
        Dem_EventPassed(EventId);
        Dem_EventFailed(EventId);
        Dem_EventPassed(EventId);
        break;

    case DEM_EVENTQUEUE_FAILED:
        Dem_EventFailed(EventId);
        break;
    case DEM_EVENTQUEUE_FAILED_PASSED:
        Dem_EventFailed(EventId);
        Dem_EventPassed(EventId);
        break;
    case DEM_EVENTQUEUE_FAILED_PASSED_FAILED:
        Dem_EventFailed(EventId);
        Dem_EventPassed(EventId);
        Dem_EventFailed(EventId);
        break;

    default:
        /** No action */
        break;
    }
}

/**
 * @brief         Queued Event process
 * @param[in]     QueueStatus: status of queue
 * @param[in]     EventId: Event Id
 * @return        Dem_EventQueueStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_ProcessEventQueuedEvent(Dem_EventIdType EventId, Dem_EventQueueStatusType QueueStatus)
{
#if (DEM_MAX_NUMBER_PRESTORED_FF > 0u)
    Dem_EventPrestorageLock(EventId);
#endif

    Dem_ProcessQueuedEventActions(EventId, QueueStatus);

#if (DEM_MAX_NUMBER_PRESTORED_FF > 0u)
    Dem_EventPrestorageRelease();
#endif
}

/**
 * @brief Queued Event process in Satellite
 */
DEM_LOCAL void Dem_EventQueuedProcess(Dem_SatelliteIdType SatelliteId)
{
    Dem_MonitorIdType monitorId = 0u;
#if (DEM_PROCESS_EVENT_LIMIT_NUMBER > 0u)
    monitorId = Dem_LastMonitorId;
#endif
    for (; monitorId < Dem_GetEventCountOfSatellite(SatelliteId); ++monitorId)
    {
        /* PRQA S 0759 ++ */ /* VL_Dem_0759 */
        Dem_SatelliteDataType satelliteData = Dem_GetSatelliteData(SatelliteId, monitorId);
        /* PRQA S 0759 -- */
        if (Dem_QueueGetQueueStatus(satelliteData) != DEM_EVENTQUEUESTATE_INITIAL)
        {
            Dem_EventQueueStatusType queueStatus;
            Dem_EventIdType          eventId = Dem_SatelliteGetEventId(SatelliteId, monitorId);

            satelliteData = Dem_ConsumeEventQueueStatus(SatelliteId, monitorId);
            queueStatus   = Dem_QueueGetQueueStatus(satelliteData);
            if (queueStatus != DEM_EVENTQUEUESTATE_INITIAL)
            {
                Dem_ProcessEventQueuedEvent(eventId, queueStatus);
#if (DEM_PROCESS_EVENT_LIMIT_NUMBER > 0u)
                Dem_ProcessedEventNum++;
                if (Dem_ProcessedEventNum >= DEM_PROCESS_EVENT_LIMIT_NUMBER)
                {
                    Dem_LastMonitorId   = monitorId + 1u;
                    Dem_LastSatelliteId = SatelliteId;
                    break;
                }
#endif
            }
        }
    }
#if (DEM_PROCESS_EVENT_LIMIT_NUMBER > 0u)
    if (monitorId >= Dem_GetEventCountOfSatellite(SatelliteId))
    {
        /** If this is the last monitor of the current satellite, reset the monitor index. */
        Dem_LastMonitorId = 0u;
        /** If this is the last monitor of the current satellite, increase the SatelliteId. */
        Dem_LastSatelliteId = SatelliteId + 1u;
    }
#endif
}

/**
 * @brief Event Queue Task process
 */
DEM_LOCAL void Dem_EventQueueTask(void)
{
    Dem_SatelliteIdType satelliteId = 0u;
#if (DEM_PROCESS_EVENT_LIMIT_NUMBER > 0u)
    Dem_ProcessedEventNum = 0u;
    satelliteId           = Dem_LastSatelliteId;
#endif
    for (; satelliteId < Dem_GetNumOfSatellite(); ++satelliteId)
    {
#if (DEM_PROCESS_EVENT_LIMIT_NUMBER == 0u)
        if (Dem_CheckAndResetSatelliteInfoEventQueue(satelliteId) == TRUE)
#else
        if (Dem_CheckSatelliteInfoEventQueue(satelliteId) == TRUE)
#endif
        {
            Dem_EventQueuedProcess(satelliteId);
#if (DEM_PROCESS_EVENT_LIMIT_NUMBER > 0u)
            if (Dem_ProcessedEventNum >= DEM_PROCESS_EVENT_LIMIT_NUMBER)
            {
                Dem_ProcessedEventNum = 0u;
                break;
            }
            (void)Dem_CheckAndResetSatelliteInfoEventQueue(satelliteId);
#endif
        }
    }
#if (DEM_PROCESS_EVENT_LIMIT_NUMBER > 0u)
    if (satelliteId >= Dem_GetNumOfSatellite())
    {
        /** If the current satellite is the last one and the monitor is also the last one, then reset the satellite
         * index. */
        Dem_LastSatelliteId = 0u;
        Dem_LastMonitorId   = 0u;
    }
#endif
}

/**
 * @brief Initialize the subcomponent Event Queue
 */
DEM_LOCAL void Dem_EventQueueInit(void)
{
#if (DEM_PROCESS_EVENT_LIMIT_NUMBER > 0u)
    Dem_LastSatelliteId   = 0u;
    Dem_LastMonitorId     = 0u;
    Dem_ProcessedEventNum = 0u;
#endif
    Dem_EnableTask(Dem_Task_EventQueue);
}

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif
