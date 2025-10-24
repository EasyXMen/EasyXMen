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
 **  @file               : Dem_Satellite.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : Implementation file of Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_SATELLITE_H
#define DEM_SATELLITE_H

/* =================================================== inclusions =================================================== */
#include "Dem_MasterSatelliteCom.h"
#include "Dem_Debounce.h"
#include "Dem_Queue.h"
#include "Dem_InitState.h"
#include "Dem_OperationCycle.h"
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
#include "Os.h"
#endif
#if (DEM_TRIGGER_FIM_REPORTS == STD_ON)
#include "FiM_Dem.h"
#endif

/* ===================================================== macros ===================================================== */
/**
 * @name  DEM_SATELLITEINFO_ENCODE
 * @{
 * @brief SatelliteStatus of satellite info value
 */
#define DEM_SATELLITEINFO_EVENTQUEUE_STATUS    (0x01u) /**< queue status */
#define DEM_SATELLITEINFO_PRESTOREQUEUE_STATUS (0x02u) /**< prestore queue status */

/* ========================================== internal function declarations ======================================== */
/**
 * @brief         Get Satellite partition current id
 * @return        Dem_SatelliteIdType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Dem_SatelliteIdType Dem_GetCurrentSatelliteId(void);

/**
 * @brief         Monitor Status Changed notification
 * @param[in]     EventId: Event Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_NotiMonitorStatusChanged(Dem_EventIdType EventId);

/**
 * @brief         Calculate Monitor status based on event status
 * @param[in]     MonitorStatusValue: Monitor status value
 * @param[in]     MonitorTrigger: Monitor trigger
 * @return        Dem_MonitorStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Dem_MonitorStatusType
    Dem_CalculateSatelliteMonitorStatus(Dem_MonitorStatusType MonitorStatusValue, Dem_EventStatusType MonitorTrigger);

/**
 * @brief         Resets the debouncing, the monitor status and all queued actions for an event.
 * @param[in]     EventId: Event Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_SatelliteClearEvent(Dem_EventIdType EventId);

/**
 * @brief         Resets the debouncing and all queued actions for an event.
 * Sets the TestNotCompletedThisCycle flag of the monitor status.
 * @param[in]     EventId: Event Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_SatelliteRestartEvent(Dem_EventIdType EventId);

/**
 * @brief         Update the corresponding satellite data information according to the debounceing result if the queue
 * is not overflowed
 * @param[in]     OldSatelliteData: Old satellite data
 * @param[in]     BaseInfoPtr: Pointer to base information
 * @return        Dem_SatelliteDataType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Dem_SatelliteDataType
    Dem_SatelliteUpdateSatelliteData(Dem_SatelliteDataType OldSatelliteData, Dem_BaseInfoConstPtrConstType BaseInfoPtr);

/**
 * @brief         Set Satellite monitor status information
 * @param[in]     EventId: Event Id
 * @param[in]     MonitorTrigger: Monitor trigger
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_SatelliteSetMonitorStatus(Dem_EventIdType EventId, Dem_EventStatusType MonitorTrigger);

/**
 * @brief         report Satellite monitor status information to the Dem
 * @param[in]     EventId: Event Id
 * @param[in]     EventStatus: Event status
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_SatelliteSetEventStatus(Dem_EventIdType EventId, Dem_EventStatusType EventStatus);

#if (DEM_EVENT_REPORTING_WITH_MONITOR == STD_ON)
/**
 * @brief         report Satellite monitor Data information to the Dem
 * @param[in]     EventId: Event Id
 * @param[in]     monitorData0: Monitor data 0
 * @param[in]     monitorData1: Monitor data 1
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_SatelliteSetMonitorData(
    Dem_EventIdType     EventId,
    Dem_MonitorDataType monitorData0,
    Dem_MonitorDataType monitorData1);
#endif

#if (defined(DEM_EVENT_TIME_DEBOUNCCE))
/**
 * @brief         Processes all time debounce of satellite
 * @param[in]     SatelliteId: Satellite Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_SatelliteMainFunctionDebounceTime(Dem_SatelliteIdType SatelliteId);
#endif

/**
 * @brief         Gets the monitor status of the given event.
 * @param[in]     EventId: Event Id
 * @param[out]    MonitorStatus: Pointer to monitor Status
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_MonitorGetStatus(Dem_EventIdType EventId, Dem_MonitorStatusType* MonitorStatus);

/**
 * @brief         Calculates the debouncing state of an event.
 * @param[in]     EventId: Event Id
 * @param[out]    DebouncingState: Pointer to debounce Status
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType
    Dem_SatelliteGetDebouncingState(Dem_EventIdType EventId, Dem_DebouncingStateType* DebouncingState);

/**
 * @brief         Reads the fault detection counter of an event.
 * @param[in]     EventId: Event Id
 * @param[out]    FaultDetectionCounter: Pointer to Fault detection counter
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_SatelliteGetFaultDetectionCounter(Dem_EventIdType EventId, sint8* FaultDetectionCounter);

/**
 * @brief         Reset the event status of an event.
 * @param[in]     EventId: Event Id
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_SatelliteResetEventStatus(Dem_EventIdType EventId);

/**
 * @brief         Reset the Monitor status of an event.
 * @param[in]     EventId: Event Id
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_SatelliteResetMonitorStatus(Dem_EventIdType EventId);

/* ========================================== internal function definitions ========================================= */
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
/**
 * @brief         Get ApplicationId of Satellite Info by SatelliteId
 * @param[in]     SatelliteId: Satellite Id
 * @return        ApplicationType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE ApplicationType Dem_GetSatelliteApplicationId(Dem_SatelliteIdType SatelliteId)
{
    return Dem_GetAppIdOfSatellite(SatelliteId);
}
#endif

/* PRQA S 0759 ++ */ /* VL_Dem_0759 */
/****************Satellite sync*******************/
/**
 * @brief         Synchronized satellite information
 */
DEM_LOCAL_INLINE boolean Dem_SyncSetSatelliteInfo(
    Dem_SatelliteIdType   SatelliteId,
    Dem_SatelliteInfoType OldSatelliteInfo,
    Dem_SatelliteInfoType NewSatelliteInfo)
{
    boolean ret;
    ret = Dem_ComSyncCompareAndSwap(Dem_GetStatusOfSatellite(SatelliteId), OldSatelliteInfo.Raw, NewSatelliteInfo.Raw);
    return ret;
}

/****************SatelliteInfo*******************/
/**
 * @brief         Get Satellite Info
 * @param[in]     SatelliteId: Satellite Id
 * @return        Dem_SatelliteInfoType
 * @return        Data: The detailed data type
 * @return        Raw: The raw data type intended for queue communication
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_SatelliteInfoType Dem_GetSatelliteInfo(Dem_SatelliteIdType SatelliteId)
{
    Dem_SatelliteInfoType satelliteInfo;
    satelliteInfo.Raw = *(Dem_GetStatusOfSatellite(SatelliteId));
    return satelliteInfo;
}

/**
 * @brief         Get Satellite Init Status
 * @param[in]     SatelliteId: Satellite Id
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetSatelliteInitStatus(Dem_SatelliteIdType SatelliteId)
{
    Dem_SatelliteInfoType satelliteInfo;
    satelliteInfo.Raw = *(Dem_GetStatusOfSatellite(SatelliteId));
    return satelliteInfo.Data.InitStatus;
}

/**
 * @brief         Set SatelliteInfo Init Status
 * @param[in]     SatelliteInfo: Satellite infomation
 * @param[in]     InitStatus: Init Status
 * @return        Dem_SatelliteInfoType
 * @return        Data: The detailed data type
 * @return        Raw: The raw data type intended for queue communication
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_SatelliteInfoType
    Dem_SetSatelliteInfoInitStatus(Dem_SatelliteInfoType SatelliteInfo, uint8 InitStatus)
{
    Dem_SatelliteInfoType lSatelliteInfo = SatelliteInfo;
    lSatelliteInfo.Data.InitStatus       = InitStatus;
    return lSatelliteInfo;
}

/**
 * @brief         Set Satellite Init Status
 * @param[in]     SatelliteId: Satellite Id
 * @param[in]     InitStatus: Init Status
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetSatelliteInitStatus(Dem_SatelliteIdType SatelliteId, uint8 InitStatus)
{
    Dem_SatelliteInfoType oldSatelliteInfo;
    Dem_SatelliteInfoType newSatelliteInfo;
    do
    {
        oldSatelliteInfo = Dem_GetSatelliteInfo(SatelliteId);
        newSatelliteInfo = Dem_SetSatelliteInfoInitStatus(oldSatelliteInfo, InitStatus);
    } while ((Dem_SyncSetSatelliteInfo(SatelliteId, oldSatelliteInfo, newSatelliteInfo) == FALSE));
}

/* PRQA S 2120 ++ */ /* VL_Dem_2120 */
/* PRQA S 3473 ++ */ /* VL_Dem_3473 */
/**
 * @brief         Set Satellite queue Status
 * @param[in]     SatelliteId: Satellite Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetSatelliteInfoEventQueue(Dem_SatelliteIdType SatelliteId)
{
    Dem_SatelliteInfoType oldSatelliteInfo;
    Dem_SatelliteInfoType newSatelliteInfo;
    do
    {
        oldSatelliteInfo = Dem_GetSatelliteInfo(SatelliteId);
        newSatelliteInfo = oldSatelliteInfo;
        DEM_FLAG_SET(newSatelliteInfo.Data.SatelliteStatus, DEM_SATELLITEINFO_EVENTQUEUE_STATUS);
    } while (Dem_SyncSetSatelliteInfo(SatelliteId, oldSatelliteInfo, newSatelliteInfo) == FALSE);
}

/**
 * @brief         Check and Reset Satellite queue Status
 * @param[in]     SatelliteId: Satellite Id
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckAndResetSatelliteInfoEventQueue(Dem_SatelliteIdType SatelliteId)
{
    Dem_SatelliteInfoType oldSatelliteInfo;
    Dem_SatelliteInfoType newSatelliteInfo;
    do
    {
        oldSatelliteInfo = Dem_GetSatelliteInfo(SatelliteId);
        newSatelliteInfo = oldSatelliteInfo;
        /* PRQA S 4399, 4461 ++ */ /* VL_Dem_4399, VL_Dem_4461 */
        DEM_FLAG_RESET(newSatelliteInfo.Data.SatelliteStatus, DEM_SATELLITEINFO_EVENTQUEUE_STATUS);
        /* PRQA S 4399, 4461 -- */
    } while (Dem_SyncSetSatelliteInfo(SatelliteId, oldSatelliteInfo, newSatelliteInfo) == FALSE);
    /* PRQA S 4404 ++ */ /* VL_Dem_4404 */
    return DEM_FLAG_ISSET(oldSatelliteInfo.Data.SatelliteStatus, DEM_SATELLITEINFO_EVENTQUEUE_STATUS);
    /* PRQA S 4404-- */
}

/**
 * @brief         Check Satellite queue Status
 * @param[in]     SatelliteId: Satellite Id
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckSatelliteInfoEventQueue(Dem_SatelliteIdType SatelliteId)
{
    Dem_SatelliteInfoType oldSatelliteInfo = Dem_GetSatelliteInfo(SatelliteId);
    /* PRQA S 4404 ++ */ /* VL_Dem_4404 */
    return DEM_FLAG_ISSET(oldSatelliteInfo.Data.SatelliteStatus, DEM_SATELLITEINFO_EVENTQUEUE_STATUS);
    /* PRQA S 4404-- */
}

#if ((DEM_MAX_NUMBER_PRESTORED_FF > 0u) && (DEM_EVENT_PARTITION_NUMBER > 1u))
/**
 * @brief         Set Satellite prestore queue Status
 * @param[in]     SatelliteInfo: Satellite infomation
 * @return        Dem_SatelliteInfoType
 * @return        Data: The detailed data type
 * @return        Raw: The raw data type intended for queue communication
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_SatelliteInfoType Dem_SetSatelliteInfoPrestoreQueue(Dem_SatelliteInfoType SatelliteInfo)
{
    Dem_SatelliteInfoType ret = SatelliteInfo;
    DEM_FLAG_SET(ret.Data.SatelliteStatus, DEM_SATELLITEINFO_PRESTOREQUEUE_STATUS);
    return ret;
}

/**
 * @brief         Reset Satellite prestore queue Status
 * @param[in]     SatelliteInfo: Satellite infomation
 * @return        Dem_SatelliteInfoType
 * @return        Data: The detailed data type
 * @return        Raw: The raw data type intended for queue communication
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_SatelliteInfoType Dem_SatelliteInfoResetPrestoreQueue(Dem_SatelliteInfoType SatelliteInfo)
{
    Dem_SatelliteInfoType ret = SatelliteInfo;
    /* PRQA S 4399, 4461 ++ */ /* VL_Dem_4399, VL_Dem_4461 */
    DEM_FLAG_RESET(ret.Data.SatelliteStatus, DEM_SATELLITEINFO_PRESTOREQUEUE_STATUS);
    /* PRQA S 4399, 4461 -- */
    return ret;
}

/**
 * @brief         Check Satellite prestore queue Status
 * @param[in]     SatelliteInfo: Satellite infomation
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckSatelliteInfoPrestoreQueue(Dem_SatelliteInfoType SatelliteInfo)
{
    /* PRQA S 4404 ++ */ /* VL_Dem_4404 */
    return DEM_FLAG_ISSET(SatelliteInfo.Data.SatelliteStatus, DEM_SATELLITEINFO_PRESTOREQUEUE_STATUS);
    /* PRQA S 4404-- */
}
#endif

/****************Satellite InitStatus*******************/
/**
 * @brief         Check Satellite Init Status is the same
 * @param[in]     SatelliteId: Satellite Id
 * @param[in]     InitStatus: Init Status
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckSatelliteInitStatus(Dem_SatelliteIdType SatelliteId, uint8 InitStatus)
{
    uint8 status = Dem_GetSatelliteInitStatus(SatelliteId);
    return (status == InitStatus); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/**
 * @brief         Check Satellite Init Status is at least
 * @param[in]     SatelliteId: Satellite Id
 * @param[in]     InitStatus: Init Status
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckLeastSatelliteInitStatus(Dem_SatelliteIdType SatelliteId, uint8 InitStatus)
{
    uint8 status = Dem_GetSatelliteInitStatus(SatelliteId);
    return (status >= InitStatus); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/**
 * @brief         Check All Satellite Init Status is at least
 * @param[in]     InitStatus: Init Status
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckAllLeastSatelliteInitStatus(uint8 InitStatus)
{
    boolean ret = TRUE;
    for (Dem_SatelliteIdType satelliteId = 0u; satelliteId < Dem_GetNumOfSatellite(); ++satelliteId)
    {
        if (Dem_CheckLeastSatelliteInitStatus(satelliteId, InitStatus) == FALSE)
        {
            /** There is at least one satellite initialization state that is not greater than the state of the requested
             * inspection */
            ret = FALSE;
        }
    }
    return ret;
}

/****************Satellite Data*******************/
/**
 * @brief         Set Satellite monitor Status
 * @param[in]     SatelliteDataValue: Satellite data value
 * @param[out]    MonitorStatus: Monitor Status
 * @return        Dem_SatelliteDataType
 * @return        Data: The detailed data type
 * @return        Raw: The raw data type intended for queue communication
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
/* PRQA S 4397 ++ */ /* VL_Dem_4397 */
DEM_LOCAL_INLINE Dem_SatelliteDataType
    Dem_SetSatelliteMonitorStatus(Dem_SatelliteDataType SatelliteDataValue, Dem_MonitorStatusType MonitorStatus)
{
    Dem_SatelliteDataType satelliteData = SatelliteDataValue;
    /* PRQA S 4399, 4461 ++ */ /* VL_Dem_4399, VL_Dem_4461 */
    DEM_FLAG_RESET(satelliteData.Data.SharedStatus, DEM_MONITOR_STATUS);
    /* PRQA S 4399, 4461 -- */
    DEM_FLAG_SET(satelliteData.Data.SharedStatus, (MonitorStatus & DEM_MONITOR_STATUS));
    return satelliteData;
}

/**
 * @brief         reset Satellite monitor initialization Status
 * @param[in]     SatelliteData: Satellite data
 * @return        Dem_SatelliteDataType
 * @return        Data: The detailed data type
 * @return        Raw: The raw data type intended for queue communication
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_SatelliteDataType Dem_ResetSatelliteMonitorInitStatus(Dem_SatelliteDataType SatelliteData)
{
    return Dem_SetSatelliteMonitorStatus(SatelliteData, DEM_MONITOR_STATUS_INITIAL);
}

/**
 * @brief         Get Satellite monitor initialization Status
 * @return        Dem_SatelliteDataType
 * @return        Data: The detailed data type
 * @return        Raw: The raw data type intended for queue communication
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_SatelliteDataType Dem_GetSatelliteMonitorInitStatus(void)
{
    Dem_SatelliteDataType satelliteData;
    satelliteData.Raw = 0u;
    satelliteData     = Dem_SetSatelliteMonitorStatus(satelliteData, DEM_MONITOR_STATUS_INITIAL);
    return satelliteData;
}

/**
 * @brief         Get Satellite monitor Status
 * @param[in]     SatelliteDataValue: Satellite data value
 * @return        Dem_MonitorStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_MonitorStatusType Dem_GetSatelliteMonitorStatus(Dem_SatelliteDataType SatelliteDataValue)
{
    return (Dem_MonitorStatusType)(SatelliteDataValue.Data.SharedStatus & DEM_MONITOR_STATUS);
}

/**
 * @brief         Get Satellite data
 * @param[in]     SatelliteId: Satellite Id
 * @param[in]     MonitorId: Monitor Id
 * @return        Dem_SatelliteDataType
 * @return        Data: The detailed data type
 * @return        Raw: The raw data type intended for queue communication
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_SatelliteDataType
    Dem_GetSatelliteData(Dem_SatelliteIdType SatelliteId, Dem_MonitorIdType MonitorId)
{
    return Dem_GetDataOfSatellite(SatelliteId)[MonitorId];
}

/**
 * @brief         Set Satellite data
 * @param[in]     SatelliteId: Satellite Id
 * @param[in]     MonitorId: Monitor Id
 * @param[in]     SatelliteData: Satellite data
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetSatelliteData(
    Dem_SatelliteIdType   SatelliteId,
    Dem_MonitorIdType     MonitorId,
    Dem_SatelliteDataType SatelliteData)
{
    Dem_GetDataOfSatellite(SatelliteId)[MonitorId] = SatelliteData;
}

/**
 * @brief         Set DebounceData of Satellite data
 */
DEM_LOCAL_INLINE Dem_SatelliteDataType
    Dem_SetSatelliteDebounceData(Dem_SatelliteDataType SatelliteData, Dem_DebounceDataType DebounceData)
{
    Dem_SatelliteDataType ret = SatelliteData;
    ret.Data.DebounceData     = DebounceData;
    return ret;
}

/**
 * @brief         Get DebounceData of Satellite data
 * @param[in]     SatelliteData: Satellite data
 * @return        Dem_DebounceDataType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_DebounceDataType Dem_GetSatelliteDebounceData(Dem_SatelliteDataType SatelliteData)
{
    return SatelliteData.Data.DebounceData;
}

/**
 * @brief         Set QueueStatus of Satellite data
 */
DEM_LOCAL_INLINE Dem_SatelliteDataType
    Dem_SetSatelliteQueueStatus(Dem_SatelliteDataType SatelliteData, Dem_EventQueueStatusType QueueStatus)
{
    Dem_SatelliteDataType ret = SatelliteData;
    ret.Data.QueueStatus      = QueueStatus;
    return ret;
}

/**
 * @brief         Get QueueStatus of Satellite data
 * @param[in]     SatelliteData: Satellite data
 * @return        Dem_EventQueueStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventQueueStatusType Dem_GetSatelliteQueueStatus(Dem_SatelliteDataType SatelliteData)
{
    return SatelliteData.Data.QueueStatus;
}

/**
 * @brief         Set DebounceStatus of Satellite data
 */
DEM_LOCAL_INLINE Dem_SatelliteDataType
    Dem_SetSatelliteDebounceStatus(Dem_SatelliteDataType SatelliteData, Dem_DebounceStatusType DebounceStatus)
{
    Dem_SatelliteDataType ret = SatelliteData;
    /* PRQA S 4399, 4461 ++ */ /* VL_Dem_4399, VL_Dem_4461 */
    DEM_FLAG_RESET(ret.Data.SharedStatus, DEM_DEBOUNCE_STATUS);
    /* PRQA S 4399, 4461 -- */
    DEM_FLAG_SET(ret.Data.SharedStatus, (DebounceStatus << DEM_DEBOUNCE_STATUS_POS));
    return ret;
}

/**
 * @brief         Get DebounceStatus of Satellite data
 */
DEM_LOCAL_INLINE Dem_DebounceStatusType Dem_GetSatelliteDebounceStatus(Dem_SatelliteDataType SatelliteData)
{
    /* PRQA S 4399 ++ */ /* VL_Dem_4399 */
    return (Dem_DebounceStatusType)((SatelliteData.Data.SharedStatus & DEM_DEBOUNCE_STATUS) >> DEM_DEBOUNCE_STATUS_POS);
    /* PRQA S 4399 -- */
}
/* PRQA S 4397 -- */
/* PRQA S 2120 -- */

/**
 * @brief         Sync Satellite data
 * @param[in]     SatelliteId: Satellite Id
 * @param[in]     MonitorId: Monitor Id
 * @param[in]     OldSatelliteData: Old satellite data
 * @param[in]     NewSatelliteData: New satellite data
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_SyncSetSatelliteData(
    Dem_SatelliteIdType   SatelliteId,
    Dem_MonitorIdType     MonitorId,
    Dem_SatelliteDataType OldSatelliteData,
    Dem_SatelliteDataType NewSatelliteData)
{
    /** Swap the new data into the buffer if the buffer's value is identical to the old data. Return whether operation
     * succeeded. */
    boolean ret = Dem_ComSyncCompareAndSwap(
        &((Dem_GetDataOfSatellite(SatelliteId))[MonitorId].Raw),
        OldSatelliteData.Raw,
        NewSatelliteData.Raw);
    return ret;
}

/**
 * @brief         Get SatelliteData and Reset QueueStatus
 * @param[in]     SatelliteId: Satellite Id
 * @param[in]     MonitorId: Monitor Id
 * @return        Dem_SatelliteDataType
 * @return        Data: The detailed data type
 * @return        Raw: The raw data type intended for queue communication
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_SatelliteDataType
    Dem_ConsumeEventQueueStatus(Dem_SatelliteIdType SatelliteId, Dem_MonitorIdType MonitorId)
{
    Dem_SatelliteDataType oldSatelliteData;
    Dem_SatelliteDataType newSatelliteData;
    do
    {
        oldSatelliteData = Dem_GetSatelliteData(SatelliteId, MonitorId);
        newSatelliteData = Dem_QueueResetQueueStatus(oldSatelliteData);
    } while (Dem_SyncSetSatelliteData(SatelliteId, MonitorId, oldSatelliteData, newSatelliteData) == FALSE);
    return oldSatelliteData;
}

/****************Satellite Extended Data*******************/

#if ((DEM_MAX_NUMBER_PRESTORED_FF > 0u) && (DEM_EVENT_PARTITION_NUMBER > 1u))
/**
 * @brief         Get Satellite Extended data
 * @param[in]     SatelliteId: Satellite Id
 * @param[in]     MonitorId: Monitor Id
 * @return        Dem_SatelliteExtendedDataType
 * @return        Data: The detailed data type
 * @return        Raw: The raw data type intended for queue communication
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_SatelliteExtendedDataType
    Dem_GetSatelliteExtendedData(Dem_SatelliteIdType SatelliteId, Dem_MonitorIdType MonitorId)
{
    return Dem_GetEDataOfSatellite(SatelliteId)[MonitorId];
}

/**
 * @brief         Set Satellite Extended data
 * @param[in]     SatelliteId: Satellite Id
 * @param[in]     MonitorId: Monitor Id
 * @param[in]     SatelliteExtendedData: Satellite extended data
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetSatelliteExtendedData(
    Dem_SatelliteIdType           SatelliteId,
    Dem_MonitorIdType             MonitorId,
    Dem_SatelliteExtendedDataType SatelliteExtendedData)
{
    Dem_GetEDataOfSatellite(SatelliteId)[MonitorId] = SatelliteExtendedData;
}

/**
 * @brief         Sync Satellite Extended data
 * @param[in]     SatelliteId: Satellite Id
 * @param[in]     MonitorId: Monitor Id
 * @param[in]     OldSatelliteEData: Old satellite extended data
 * @param[in]     NewSatelliteEData: New satellite extended data
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_SyncSetSatelliteExtendedData(
    Dem_SatelliteIdType           SatelliteId,
    Dem_MonitorIdType             MonitorId,
    Dem_SatelliteExtendedDataType OldSatelliteEData,
    Dem_SatelliteExtendedDataType NewSatelliteEData)
{
    /** Swap the new data into the buffer if the buffer's value is identical to the old data. Return whether operation
     * succeeded. */
    boolean ret = Dem_ComSyncCompareAndSwap(
        &((Dem_GetEDataOfSatellite(SatelliteId))[MonitorId].Raw),
        OldSatelliteEData.Raw,
        NewSatelliteEData.Raw);
    return ret;
}
#endif

#if (DEM_EVENT_REPORTING_WITH_MONITOR == STD_ON)
/**
 * @brief         Get Satellite monitorData0
 * @param[in]     SatelliteId: Satellite Id
 * @param[in]     MonitorId: Monitor Id
 * @return        Dem_MonitorDataType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_MonitorDataType
    Dem_GetSatelliteMonitorData0(Dem_SatelliteIdType SatelliteId, Dem_MonitorIdType MonitorId)
{
    return Dem_GetMonitorData0OfSatellite(SatelliteId)[MonitorId];
}

/**
 * @brief         Set Satellite monitorData0
 * @param[in]     SatelliteId: Satellite Id
 * @param[in]     MonitorId: Monitor Id
 * @param[in]     OldMonitorData0: Old monitor data 0
 * @param[in]     NewMonitorData0: New monitor data 0
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_SyncSatelliteMonitorData0(
    Dem_SatelliteIdType SatelliteId,
    Dem_MonitorIdType   MonitorId,
    Dem_MonitorDataType OldMonitorData0,
    Dem_MonitorDataType NewMonitorData0)
{
    /** Swap the new data into the buffer if the buffer's value is identical to the old data. Return whether operation
     * succeeded. */
    boolean ret = Dem_ComSyncCompareAndSwap(
        (Dem_ComProcessorWordType* const)&((Dem_GetMonitorData0OfSatellite(SatelliteId))[MonitorId]),
        OldMonitorData0,
        NewMonitorData0);
    return ret;
}

/**
 * @brief         Get Satellite monitorData1
 * @param[in]     SatelliteId: Satellite Id
 * @param[in]     MonitorId: Monitor Id
 * @return        Dem_MonitorDataType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_MonitorDataType
    Dem_GetSatelliteMonitorData1(Dem_SatelliteIdType SatelliteId, Dem_MonitorIdType MonitorId)
{
    return Dem_GetMonitorData1OfSatellite(SatelliteId)[MonitorId];
}

/**
 * @brief         Set Satellite monitorData1
 * @param[in]     SatelliteId: Satellite Id
 * @param[in]     MonitorId: Monitor Id
 * @param[in]     OldMonitorData1: Old monitor data 1
 * @param[in]     NewMonitorData1: New monitor data 1
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_SyncSatelliteMonitorData1(
    Dem_SatelliteIdType SatelliteId,
    Dem_MonitorIdType   MonitorId,
    Dem_MonitorDataType OldMonitorData1,
    Dem_MonitorDataType NewMonitorData1)
{
    /** Swap the new data into the buffer if the buffer's value is identical to the old data. Return whether operation
     * succeeded. */
    boolean ret = Dem_ComSyncCompareAndSwap(
        (Dem_ComProcessorWordType* const)&((Dem_GetMonitorData1OfSatellite(SatelliteId))[MonitorId]),
        OldMonitorData1,
        NewMonitorData1);
    return ret;
}
#endif

/****************Satellite Function*******************/
/**
 * @brief         Get SatelliteId by EventId
 * @param[in]     EventId: Event Id
 * @return        Dem_SatelliteIdType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_SatelliteIdType Dem_GetSatelliteIdByEventId(Dem_EventIdType EventId)
{
#if (DEM_EVENT_PARTITION_NUMBER == 1u)
    DEM_PARAM_UNUSED(EventId);
#endif
    return Dem_GetSatelliteIdxOfEvent(EventId);
}

/**
 * @brief         Get MonitorId by EventId
 * @param[in]     EventId: Event Id
 * @return        Dem_MonitorIdType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_MonitorIdType Dem_GetMonitorIdByEventId(Dem_EventIdType EventId)
{
    return Dem_GetSatelliteEventIdxOfEvent(EventId); /* PRQA S 3432,4461 */ /* VL_Dem_3432,VL_Dem_4461 */
}

/**
 * @brief         Get EventId by SatelliteId and MonitorId
 * @param[in]     SatelliteId: Satellite Id
 * @param[in]     MonitorId: Monitor Id
 * @return        Dem_EventIdType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventIdType Dem_SatelliteGetEventId(Dem_SatelliteIdType SatelliteId, Dem_MonitorIdType MonitorId)
{
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    return Dem_GetEventId(Dem_GetEventStartOfSatellite(SatelliteId) + MonitorId);
#else
    return Dem_GetEventId(Dem_GetEventStartOfSatellite(SatelliteId) + MonitorId + 1u);
#endif
}

/* PRQA S 3406, 1505 ++ */ /* VL_Dem_3406, VL_Dem_1505 */
/**
 * @brief Satellite partition pre-initialization
 * @param[in]     SatelliteId: Satellite Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
void Dem_SatelliteSatPreInit(Dem_SatelliteIdType SatelliteId)
{
    Dem_SatelliteDataType satelliteData = Dem_GetSatelliteMonitorInitStatus();
#if ((DEM_MAX_NUMBER_PRESTORED_FF > 0u) && (DEM_EVENT_PARTITION_NUMBER > 1u))
    Dem_SatelliteExtendedDataType satelliteExtendedData;
    satelliteExtendedData.Raw = 0u;
#endif
    for (Dem_MonitorIdType monitorId = 0u; monitorId < Dem_GetEventCountOfSatellite(SatelliteId); ++monitorId)
    {
        Dem_SetSatelliteData(SatelliteId, monitorId, satelliteData);
#if ((DEM_MAX_NUMBER_PRESTORED_FF > 0u) && (DEM_EVENT_PARTITION_NUMBER > 1u))
        Dem_SetSatelliteExtendedData(SatelliteId, monitorId, satelliteExtendedData);
#endif
#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
        Dem_MaxDebValPreInit(SatelliteId, monitorId);
#endif
    }
    Dem_SetSatelliteInitStatus(SatelliteId, DEM_PREINITIALIZED);
}

#if (DEM_EVENT_PARTITION_NUMBER > 1u)
/**
 * @brief         Satellite partition check
 * @param[in]     EventId: Event Id
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_SatelliteCheckEventPartition(Dem_EventIdType EventId)
{
    /* PRQA S 4404 ++ */ /* VL_Dem_4404 */
    return (GetApplicationID() == Dem_GetSatelliteApplicationId(Dem_GetSatelliteIdByEventId(EventId)));
    /* PRQA S 4404-- */
}
#endif

/**
 * @brief Get Satellite partition current id
 */
DEM_LOCAL Dem_SatelliteIdType Dem_GetCurrentSatelliteId(void)
{
    Dem_SatelliteIdType satelliteId;
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    ApplicationType currentApplication = GetApplicationID();
    for (satelliteId = 0u; satelliteId < Dem_GetNumOfSatellite(); ++satelliteId)
    {
        if (currentApplication == Dem_GetSatelliteApplicationId(satelliteId))
        {
            break;
        }
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    if (satelliteId >= Dem_GetNumOfSatellite())
    {
        Dem_DetReportError(DEM_SID_INTERNALAPIID, DEM_E_WRONG_CONFIGURATION);
    }
#endif
#else
    satelliteId = DEM_SATELLITE_APPLICATION_ZERO;
#endif
    return satelliteId;
}

#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
/**
 * @brief         Gets the maximum fault detection counter of an event.
 * @param[in]     SatelliteId: Satellite Id
 * @param[in]     MonitorId: Monitor Id
 * @return        sint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE sint8
    Dem_SatelliteGetMaxFaultDetectionCounter(Dem_SatelliteIdType SatelliteId, Dem_MonitorIdType MonitorId)
{
    sint8                         fdc;
    Dem_EventIdType               eventId       = Dem_SatelliteGetEventId(SatelliteId, MonitorId);
    Dem_SatelliteDataType         satelliteData = Dem_GetSatelliteData(SatelliteId, MonitorId);
    Dem_DebounceInfoType          debounceInfo;
    Dem_BaseInfoConstPtrConstType baseInfo = Dem_DebounceInfoInit(eventId, &debounceInfo, satelliteData);

    (void)Dem_DebounceGetMaxFaultDetectionCounter(baseInfo, &fdc);
    return fdc;
}
#endif

/**
 * @brief Monitor Status Changed notification
 */
DEM_LOCAL void Dem_NotiMonitorStatusChanged(Dem_EventIdType EventId)
{
#if (                                                                                \
    (DEM_GENERAL_INTERFACE_SUPPORT == STD_ON) || (DEM_TRIGGER_FIM_REPORTS == STD_ON) \
    || (DEM_EVENT_CALLBACK_MONITOR_STATUS_CHANGED > 0u))
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == TRUE)
    {
#if (DEM_GENERAL_INTERFACE_SUPPORT == STD_ON)
        if (Dem_GeneralTriggerOnMonitorStatus != NULL_PTR)
        {
            (void)Dem_GeneralTriggerOnMonitorStatus(EventId);
        }
#endif
#if (DEM_TRIGGER_FIM_REPORTS == STD_ON)
        (void)FiM_DemTriggerOnMonitorStatus(EventId);
#endif
#if (DEM_EVENT_CALLBACK_MONITOR_STATUS_CHANGED > 0u)
        for (Dem_MonitorStatusChangedFncNumType lMSCRefStart = Dem_GetMSCRefStartOfEvent(EventId);
             lMSCRefStart < Dem_GetMSCRefEndOfEvent(EventId);
             ++lMSCRefStart)
        {
            const DemTriggerOnMonitorStatusType fnc = Dem_GetTriggerOnMonitorStatusFncOfEvent(lMSCRefStart);
            if (fnc != NULL_PTR)
            {
                (void)(fnc());
            }
        }
#endif
    }
#else
    DEM_PARAM_UNUSED(EventId);
#endif
}

/**
 * @brief Calculate Monitor status based on event status
 */
DEM_LOCAL Dem_MonitorStatusType
    Dem_CalculateSatelliteMonitorStatus(Dem_MonitorStatusType MonitorStatusValue, Dem_EventStatusType MonitorTrigger)
{
    Dem_MonitorStatusType monitorStatus = MonitorStatusValue;
    switch (MonitorTrigger)
    {
        /* PRQA S 4399, 4461 ++ */ /* VL_Dem_4399, VL_Dem_4461 */
    case DEM_EVENT_STATUS_FAILED:
        /** Set Monitor Status TF */
        DEM_FLAG_SET(monitorStatus, DEM_MONITOR_STATUS_TF);
        /** Reset Monitor Status TNCTOC */
        DEM_FLAG_RESET(monitorStatus, DEM_MONITOR_STATUS_TNCTOC);
        break;
    case DEM_EVENT_STATUS_PASSED:
        /** Reset Monitor Status TF */
        DEM_FLAG_RESET(monitorStatus, DEM_MONITOR_STATUS_TF);
        /** Reset Monitor Status TNCTOC */
        DEM_FLAG_RESET(monitorStatus, DEM_MONITOR_STATUS_TNCTOC);
        /* PRQA S 4399, 4461 -- */
        break;
    default:
        /** idle */
        break;
    }
    return monitorStatus;
}

/**
 * @brief Resets the debouncing, the monitor status and all queued actions for an event.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
DEM_LOCAL void Dem_SatelliteClearEvent(Dem_EventIdType EventId)
/* PRQA S 6070 -- */
{
    Dem_SatelliteIdType      satelliteId = Dem_GetSatelliteIdByEventId(EventId);
    Dem_MonitorIdType        monitorId   = Dem_GetMonitorIdByEventId(EventId);
    Dem_SatelliteDataType    oldSatelliteData;
    Dem_SatelliteDataType    newSatelliteData;
    Dem_DebounceInfoType     debounceInfo;
    Dem_BaseInfoConstPtrType baseInfo;
    do
    {
        oldSatelliteData = Dem_GetSatelliteData(satelliteId, monitorId);
        newSatelliteData = oldSatelliteData;
        baseInfo         = Dem_DebounceInfoInit(EventId, &debounceInfo, oldSatelliteData);

        newSatelliteData = Dem_ResetDebouncing(baseInfo, newSatelliteData);
        newSatelliteData = Dem_QueueResetQueueStatus(newSatelliteData);
        newSatelliteData = Dem_ResetSatelliteMonitorInitStatus(newSatelliteData);
    } while (Dem_SyncSetSatelliteData(satelliteId, monitorId, oldSatelliteData, newSatelliteData) == FALSE);

#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
    Dem_DebounceResetDebounceValueMax(baseInfo);
#endif
    /** Notify monitor status change */
    if (Dem_GetSatelliteMonitorStatus(oldSatelliteData) != Dem_GetSatelliteMonitorStatus(newSatelliteData))
    {
        Dem_NotiMonitorStatusChanged(EventId);
    }
}

/**
 * @brief Resets the debouncing and all queued actions for an event.
 * Sets the TestNotCompletedThisCycle flag of the monitor status.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
DEM_LOCAL void Dem_SatelliteRestartEvent(Dem_EventIdType EventId)
/* PRQA S 6070 -- */
{
    Dem_SatelliteIdType      satelliteId = Dem_GetSatelliteIdByEventId(EventId);
    Dem_MonitorIdType        monitorId   = Dem_GetMonitorIdByEventId(EventId);
    Dem_SatelliteDataType    oldSatelliteData;
    Dem_SatelliteDataType    newSatelliteData;
    Dem_DebounceInfoType     debounceInfo;
    Dem_BaseInfoConstPtrType baseInfo;
    Dem_MonitorStatusType    monitorStatus;
    do
    {
        oldSatelliteData = Dem_GetSatelliteData(satelliteId, monitorId);
        newSatelliteData = oldSatelliteData;
        baseInfo         = Dem_DebounceInfoInit(EventId, &debounceInfo, oldSatelliteData);

        newSatelliteData = Dem_ResetDebouncing(baseInfo, newSatelliteData);
        newSatelliteData = Dem_QueueResetQueueStatus(newSatelliteData);
        monitorStatus    = Dem_GetSatelliteMonitorStatus(oldSatelliteData);
        /** Set Monitor Status TNCTOC */
        DEM_FLAG_SET(monitorStatus, DEM_MONITOR_STATUS_TNCTOC);
        newSatelliteData = Dem_SetSatelliteMonitorStatus(newSatelliteData, monitorStatus);
    } while (Dem_SyncSetSatelliteData(satelliteId, monitorId, oldSatelliteData, newSatelliteData) == FALSE);

#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
    Dem_DebounceResetDebounceValueMax(baseInfo);
#endif
    /** Notify monitor status change */
    if (Dem_GetSatelliteMonitorStatus(oldSatelliteData) != Dem_GetSatelliteMonitorStatus(newSatelliteData))
    {
        Dem_NotiMonitorStatusChanged(EventId);
    }
}
/* PRQA S 3473 -- */

/**
 * @brief Resets the debouncing of an event.
 */
DEM_LOCAL void Dem_SatelliteResetDebounceStatus(Dem_EventIdType EventId)
{
    Dem_SatelliteIdType      satelliteId = Dem_GetSatelliteIdByEventId(EventId);
    Dem_MonitorIdType        monitorId   = Dem_GetMonitorIdByEventId(EventId);
    Dem_SatelliteDataType    oldSatelliteData;
    Dem_SatelliteDataType    newSatelliteData;
    Dem_DebounceInfoType     debounceInfo;
    Dem_BaseInfoConstPtrType baseInfo;
    do
    {
        oldSatelliteData = Dem_GetSatelliteData(satelliteId, monitorId);
        newSatelliteData = oldSatelliteData;
        baseInfo         = Dem_DebounceInfoInit(EventId, &debounceInfo, oldSatelliteData);

        newSatelliteData = Dem_ResetDebouncing(baseInfo, newSatelliteData);
    } while (Dem_SyncSetSatelliteData(satelliteId, monitorId, oldSatelliteData, newSatelliteData) == FALSE);
}

/**
 * @brief Freeze the debouncing of an event.
 */
DEM_LOCAL void Dem_SatelliteFreezeDebounceStatus(Dem_EventIdType EventId)
{
    Dem_SatelliteIdType      satelliteId = Dem_GetSatelliteIdByEventId(EventId);
    Dem_MonitorIdType        monitorId   = Dem_GetMonitorIdByEventId(EventId);
    Dem_SatelliteDataType    oldSatelliteData;
    Dem_SatelliteDataType    newSatelliteData;
    Dem_DebounceInfoType     debounceInfo;
    Dem_BaseInfoConstPtrType baseInfo;
    do
    {
        oldSatelliteData = Dem_GetSatelliteData(satelliteId, monitorId);
        newSatelliteData = oldSatelliteData;
        baseInfo         = Dem_DebounceInfoInit(EventId, &debounceInfo, oldSatelliteData);

        newSatelliteData = Dem_FreezeDebouncing(baseInfo, newSatelliteData);
    } while (Dem_SyncSetSatelliteData(satelliteId, monitorId, oldSatelliteData, newSatelliteData) == FALSE);
}

/**
 * @brief Update the corresponding satellite data information according to the debounceing result if the queue is not
 * overflowed
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
DEM_LOCAL Dem_SatelliteDataType
    Dem_SatelliteUpdateSatelliteData(Dem_SatelliteDataType OldSatelliteData, Dem_BaseInfoConstPtrConstType BaseInfoPtr)
/* PRQA S 6070 -- */
{
    Dem_SatelliteDataType    newSatelliteData = OldSatelliteData;
    Dem_EventStatusType      monitorTrigger   = BaseInfoPtr->MonitorTrigger;
    Dem_EventQueueStatusType oldQueueStatus   = Dem_QueueGetQueueStatus(OldSatelliteData);
    Dem_EventQueueStatusType newQueueStatus   = Dem_QueueCalculateQueueStatus(oldQueueStatus, monitorTrigger);
    Dem_MonitorStatusType    monitorStatus    = Dem_GetSatelliteMonitorStatus(OldSatelliteData);

    monitorStatus = Dem_CalculateSatelliteMonitorStatus(monitorStatus, monitorTrigger);
    if (Dem_CheckRequestFdcProcessing(BaseInfoPtr) == TRUE)
    {
        /** update event entry with a new max value > pre-confirmation limit on task level */
        newQueueStatus = Dem_QueueSetFdcProcessingStatus(newQueueStatus);
    }
    if (Dem_CheckFdcThresholdExceeded(BaseInfoPtr) == TRUE)
    {
        newQueueStatus = Dem_QueueSetFdcTripStatus(newQueueStatus);
    }
    if (Dem_CheckMaximumFdcExceeded(BaseInfoPtr) == TRUE)
    {
        newQueueStatus = Dem_QueueSetFdcMaxSLCStatus(newQueueStatus);
    }
    newSatelliteData = Dem_DebounceGetDebouncingResult(BaseInfoPtr, newSatelliteData);
    newSatelliteData = Dem_SetSatelliteMonitorStatus(newSatelliteData, monitorStatus);
    newSatelliteData = Dem_SetSatelliteQueueStatus(newSatelliteData, newQueueStatus);
    return newSatelliteData;
}

/**
 * @brief Set Satellite monitor status information
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
DEM_LOCAL Std_ReturnType Dem_SatelliteSetMonitorStatus(Dem_EventIdType EventId, Dem_EventStatusType MonitorTrigger)
/* PRQA S 6070 -- */
{
    Std_ReturnType        ret = E_OK;
    Dem_DebounceInfoType  debounceInfo;
    Dem_SatelliteIdType   satelliteId = Dem_GetSatelliteIdByEventId(EventId);
    Dem_MonitorIdType     monitorId   = Dem_GetMonitorIdByEventId(EventId);
    Dem_SatelliteDataType oldSatelliteData;
    Dem_SatelliteDataType newSatelliteData;
    Dem_BaseInfoPtrType   baseInfo;

    do
    {
        oldSatelliteData = Dem_GetSatelliteData(satelliteId, monitorId);
        newSatelliteData = oldSatelliteData;
        baseInfo         = Dem_DebounceInfoInit(EventId, &debounceInfo, oldSatelliteData);
        Dem_DebounceSetMonitorTrigger(baseInfo, MonitorTrigger);
        /** do Debounce Calculate */
        if (Dem_DebounceCalculate(baseInfo) == TRUE)
        {
            /** check debouncing result is changed */
            if (Dem_CheckDebouncingChanged(baseInfo) == TRUE)
            {
                /** check queue is over flow */
                if (Dem_QueueCheckQueueOverflow(Dem_QueueGetQueueStatus(oldSatelliteData), baseInfo->MonitorTrigger)
                    == FALSE)
                {
                    /** Update the corresponding satellite data information according to the debounceing result if the
                     * queue is not overflowed */
                    newSatelliteData = Dem_SatelliteUpdateSatelliteData(oldSatelliteData, baseInfo);
                    ret              = E_OK;
                }
                else
                {
                    /** Event update must be discarded due to queue overflow */
                    ret = E_NOT_OK;
                    break;
                }
            }
        }
        else
        {
            ret = E_NOT_OK;
        }
    } while (Dem_SyncSetSatelliteData(satelliteId, monitorId, oldSatelliteData, newSatelliteData) == FALSE);

#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
    if (Dem_CheckMaximumFdcExceeded(baseInfo) == TRUE)
    {
        Dem_DebounceUpdateDebounceValueMax(baseInfo);
    }
#endif

    if ((Dem_QueueGetQueueStatus(oldSatelliteData) == DEM_EVENTQUEUESTATE_INITIAL)
        && (Dem_QueueGetQueueStatus(newSatelliteData) != DEM_EVENTQUEUESTATE_INITIAL))
    {
        Dem_SetSatelliteInfoEventQueue(satelliteId);
    }

    /** Notify monitor status change */
    if (Dem_GetSatelliteMonitorStatus(oldSatelliteData) != Dem_GetSatelliteMonitorStatus(newSatelliteData))
    {
        Dem_NotiMonitorStatusChanged(EventId);
        if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
        {
            Dem_DetReportRuntimeError(DEM_SID_INTERNALAPIID, DEM_E_UDS_STATUS_PROCESSING_FAILED);
        }
    }
    return ret;
}

#if (DEM_DEBOUNCE_COUNTER_STORAGE_NUM > 0u)
/**
 * @brief Restores the given debouncing value of an event.
 * Value is restored, if
 * - the given debouncing value is not qualified
 * - the debouncing value is qualified and the reached threshold equals the ExpectedThreshold.
 */
DEM_LOCAL void Dem_SatelliteRestoreDebounceValue(Dem_EventIdType EventId, sint16 Value, boolean ExpectedThreshold)
{
    Dem_SatelliteIdType      satelliteId = Dem_GetSatelliteIdByEventId(EventId);
    Dem_MonitorIdType        monitorId   = Dem_GetMonitorIdByEventId(EventId);
    Dem_SatelliteDataType    oldSatelliteData;
    Dem_SatelliteDataType    newSatelliteData;
    Dem_DebounceInfoType     debounceInfo;
    Dem_BaseInfoConstPtrType baseInfo;
    do
    {
        oldSatelliteData = Dem_GetSatelliteData(satelliteId, monitorId);
        newSatelliteData = oldSatelliteData;
        baseInfo         = Dem_DebounceInfoInit(EventId, &debounceInfo, oldSatelliteData);

        newSatelliteData = Dem_DebounceRestoreDebounceValue(baseInfo, newSatelliteData, Value, ExpectedThreshold);
        newSatelliteData = Dem_QueueResetQueueStatus(newSatelliteData);
    } while (Dem_SyncSetSatelliteData(satelliteId, monitorId, oldSatelliteData, newSatelliteData) == FALSE);
}
#endif

/**
 * @brief report Satellite monitor status information to the Dem
 */
DEM_LOCAL Std_ReturnType Dem_SatelliteSetEventStatus(Dem_EventIdType EventId, Dem_EventStatusType EventStatus)
{
    Std_ReturnType ret = E_NOT_OK;
    /** Check event available information */
    if ((Dem_GetAvailableInVariantByEvent(EventId) == TRUE) && (Dem_GetEventAvailableStatusByEventId(EventId) == TRUE))
    {
        ret = Dem_SatelliteSetMonitorStatus(EventId, EventStatus);
    }
    return ret;
}

#if (DEM_EVENT_REPORTING_WITH_MONITOR == STD_ON)
/**
 * @brief report Satellite monitor Data information to the Dem
 */
DEM_LOCAL void Dem_SatelliteSetMonitorData(
    Dem_EventIdType     EventId,
    Dem_MonitorDataType monitorData0,
    Dem_MonitorDataType monitorData1)
{
    Dem_MonitorDataType oldMonitorData;
    Dem_MonitorDataType newMonitorData;
    Dem_SatelliteIdType satelliteId = Dem_GetSatelliteIdByEventId(EventId);
    Dem_MonitorIdType   monitorId   = Dem_GetMonitorIdByEventId(EventId);
    /** deal with monitorData0 */
    do
    {
        oldMonitorData = Dem_GetSatelliteMonitorData0(satelliteId, monitorId);
        newMonitorData = monitorData0;
    } while (Dem_SyncSatelliteMonitorData0(satelliteId, monitorId, oldMonitorData, newMonitorData) == FALSE);

    /** deal with monitorData1 */
    do
    {
        oldMonitorData = Dem_GetSatelliteMonitorData1(satelliteId, monitorId);
        newMonitorData = monitorData1;
    } while (Dem_SyncSatelliteMonitorData1(satelliteId, monitorId, oldMonitorData, newMonitorData) == FALSE);
}
#endif

/**
 * @brief         Satellite partition Memory pre-initialization
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SatelliteMemoryInit(void)
{
    for (Dem_SatelliteIdType satelliteId = 0u; satelliteId < Dem_GetNumOfSatellite(); ++satelliteId)
    {
        Dem_SetSatelliteInitStatus(satelliteId, DEM_UNINITIALIZED);
    }
}

/**
 * @brief         Dem_SatellitePreInit is called directly for pre-initialization in case of single partition, or
 *                executed by the corresponding partition in case of multiple partitions.
 * @reentrant     Non reentrant
 * @synchronous   TRUE
 * @trace         CPD-68116
 */
void Dem_SatellitePreInit(void)
{
    Dem_SatelliteIdType satelliteId = Dem_GetCurrentSatelliteId();
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (DEM_EVENT_PARTITION_NUMBER == satelliteId)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
    else if (Dem_CheckMasterInitStatus(DEM_PREINITIALIZED) == FALSE)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
    else if (Dem_CheckLeastSatelliteInitStatus(satelliteId, DEM_UNINITIALIZED) == FALSE)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
    else
#endif
    {
        Dem_SatelliteSatPreInit(satelliteId);
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
 * @brief         Initializes the satellite.
 * @param[in]     SatelliteId: Satellite Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SatelliteSatInit(Dem_SatelliteIdType SatelliteId)
{
    Dem_SetSatelliteInitStatus(SatelliteId, DEM_INITIALIZED);
#if ((DEM_TRIGGER_FIM_REPORTS == STD_ON) && (DEM_EVENT_PARTITION_NUMBER > 1u))
    FiM_DemInitSatellite(Dem_GetSatelliteApplicationId(SatelliteId));
#endif
}

/**
 * @brief         Dem_SatelliteInit is called directly for initialization in case of single partition, or
 *                executed by the corresponding partition in case of multiple partitions.
 * @reentrant     Non reentrant
 * @synchronous   TRUE
 * @trace         CPD-68118
 */
void Dem_SatelliteInit(void)
{
    Dem_SatelliteIdType satelliteId = Dem_GetCurrentSatelliteId();
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
    else if (DEM_EVENT_PARTITION_NUMBER == satelliteId)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
    else if (Dem_CheckSatelliteInitStatus(satelliteId, DEM_PREINITIALIZED) == FALSE)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
    else
#endif
    {
        Dem_SatelliteSatInit(satelliteId);
    }

#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_INIT, errorId);
    }
#endif
}

#if (defined(DEM_EVENT_TIME_DEBOUNCCE))
/**
 * @brief Processes all time debounce of satellite
 */
DEM_LOCAL void Dem_SatelliteMainFunctionDebounceTime(Dem_SatelliteIdType SatelliteId)
{
    for (Dem_EventIdType iloop = Dem_GetDTEventSrartOfSatellite(SatelliteId);
         iloop < Dem_GetDTEventCountOfSatellite(SatelliteId);
         ++iloop)
    {
        Dem_EventIdType          eventId   = Dem_GetDTEventId(iloop);
        Dem_MonitorIdType        monitorId = Dem_GetMonitorIdByEventId(eventId);
        Dem_DebounceInfoType     debounceInfo;
        Dem_SatelliteDataType    oldSatelliteData;
        Dem_SatelliteDataType    newSatelliteData;
        Dem_BaseInfoConstPtrType baseInfo;

        oldSatelliteData = Dem_GetSatelliteData(SatelliteId, monitorId);
        baseInfo         = Dem_DebounceInfoInit(eventId, &debounceInfo, oldSatelliteData);
        /** Check if timing debouncing is activated */
        if (Dem_DebounceTimeIsTimerActive(baseInfo) == TRUE)
        {
            do
            {
                oldSatelliteData = Dem_GetSatelliteData(SatelliteId, monitorId);
                newSatelliteData = oldSatelliteData;
                baseInfo         = Dem_DebounceInfoInit(eventId, &debounceInfo, oldSatelliteData);
                /** do time debounce */
                Dem_DebounceTimeProcessDebounce(baseInfo);
                /** update Monitor Status, update Queue Status etc. */
                newSatelliteData = Dem_SatelliteUpdateSatelliteData(oldSatelliteData, baseInfo);
            } while (Dem_SyncSetSatelliteData(SatelliteId, monitorId, oldSatelliteData, newSatelliteData) == FALSE);

#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
            if (Dem_CheckMaximumFdcExceeded(baseInfo) == TRUE)
            {
                Dem_DebounceUpdateDebounceValueMax(baseInfo);
            }
#endif

            if ((Dem_QueueGetQueueStatus(oldSatelliteData) == DEM_EVENTQUEUESTATE_INITIAL)
                && (Dem_QueueGetQueueStatus(newSatelliteData) != DEM_EVENTQUEUESTATE_INITIAL))
            {
                Dem_SetSatelliteInfoEventQueue(SatelliteId);
            }

            /** Notify monitor status change */
            if (Dem_GetSatelliteMonitorStatus(oldSatelliteData) != Dem_GetSatelliteMonitorStatus(newSatelliteData))
            {
                Dem_NotiMonitorStatusChanged(eventId);
            }
        }
    }
}
#endif

/**
 * @brief         Cyclic DemSatellite timer task.
 *                Processes all time debounce of satellite
 * @reentrant     Non reentrant
 * @synchronous   TRUE
 * @trace         CPD-68120
 */
void Dem_SatelliteMainFunction(void)
{
#if (defined(DEM_EVENT_TIME_DEBOUNCCE))
    Dem_SatelliteIdType satelliteId = Dem_GetCurrentSatelliteId();
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    if (DEM_EVENT_PARTITION_NUMBER == satelliteId)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
    else if (Dem_CheckLeastSatelliteInitStatus(satelliteId, DEM_PREINITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else
#endif
    {
        if (Dem_CheckSatelliteInitStatus(satelliteId, DEM_INITIALIZED) == TRUE)
        {
            Dem_SatelliteMainFunctionDebounceTime(satelliteId);
        }
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_MAINFUNCTION, errorId);
    }
#endif
#endif
}
/* PRQA S 3406, 1505 -- */

/**
 * @brief  Gets the monitor status of the given event.
 */
DEM_LOCAL Std_ReturnType Dem_MonitorGetStatus(Dem_EventIdType EventId, Dem_MonitorStatusType* MonitorStatus)
{
    Std_ReturnType ret = E_NOT_OK;
    if ((Dem_GetAvailableInVariantByEvent(EventId) == TRUE) && (Dem_GetEventAvailableStatusByEventId(EventId) == TRUE))
    {
        Dem_SatelliteIdType   satelliteId = Dem_GetSatelliteIdByEventId(EventId);
        Dem_MonitorIdType     monitorId   = Dem_GetMonitorIdByEventId(EventId);
        Dem_SatelliteDataType satelliteData;

        satelliteData  = Dem_GetSatelliteData(satelliteId, monitorId);
        *MonitorStatus = Dem_GetSatelliteMonitorStatus(satelliteData);
        ret            = E_OK;
    }
    return ret;
}

/**
 * @brief Calculates the debouncing state of an event.
 */
DEM_LOCAL Std_ReturnType
    Dem_SatelliteGetDebouncingState(Dem_EventIdType EventId, Dem_DebouncingStateType* DebouncingState)
{
    Std_ReturnType ret = E_NOT_OK;
    if ((Dem_GetAvailableInVariantByEvent(EventId) == TRUE) && (Dem_GetEventAvailableStatusByEventId(EventId) == TRUE))
    {
        Dem_SatelliteIdType      satelliteId = Dem_GetSatelliteIdByEventId(EventId);
        Dem_MonitorIdType        monitorId   = Dem_GetMonitorIdByEventId(EventId);
        Dem_SatelliteDataType    satelliteData;
        Dem_BaseInfoConstPtrType baseInfo;
        Dem_DebounceInfoType     debounceInfo;

        satelliteData    = Dem_GetSatelliteData(satelliteId, monitorId);
        baseInfo         = Dem_DebounceInfoInit(EventId, &debounceInfo, satelliteData);
        *DebouncingState = Dem_DebounceGetDebouncingState(baseInfo);
        ret              = E_OK;
    }
    return ret;
}

/**
 * @brief Reads the fault detection counter of an event.
 */
DEM_LOCAL Std_ReturnType Dem_SatelliteGetFaultDetectionCounter(Dem_EventIdType EventId, sint8* FaultDetectionCounter)
{
    Std_ReturnType ret = E_NOT_OK;
    if ((Dem_GetAvailableInVariantByEvent(EventId) == TRUE) && (Dem_GetEventAvailableStatusByEventId(EventId) == TRUE))
    {
        Dem_SatelliteIdType      satelliteId = Dem_GetSatelliteIdByEventId(EventId);
        Dem_MonitorIdType        monitorId   = Dem_GetMonitorIdByEventId(EventId);
        Dem_SatelliteDataType    satelliteData;
        Dem_BaseInfoConstPtrType baseInfo;
        Dem_DebounceInfoType     debounceInfo;

        satelliteData = Dem_GetSatelliteData(satelliteId, monitorId);
        baseInfo      = Dem_DebounceInfoInit(EventId, &debounceInfo, satelliteData);
        ret           = Dem_DebounceGetFaultDetectionCounter(baseInfo, FaultDetectionCounter);
    }
    return ret;
}

/**
 * @brief Reset the event status of an event.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
DEM_LOCAL Std_ReturnType Dem_SatelliteResetEventStatus(Dem_EventIdType EventId)
/* PRQA S 6070 -- */
{
    Std_ReturnType ret = E_NOT_OK;
    if ((Dem_GetAvailableInVariantByEvent(EventId) == TRUE) && (Dem_GetEventAvailableStatusByEventId(EventId) == TRUE)
        && (Dem_UDSStatusCheckTNCTOC(Dem_GetEventUDSStatus(EventId)) == TRUE))
    {
        Dem_SatelliteIdType satelliteId = Dem_GetSatelliteIdByEventId(EventId);
        Dem_MonitorIdType   monitorId   = Dem_GetMonitorIdByEventId(EventId);
        boolean             processingDone;
        do
        {
            Dem_SatelliteDataType    oldSatelliteData;
            Dem_SatelliteDataType    newSatelliteData;
            Dem_EventQueueStatusType queueStatus;

            oldSatelliteData = Dem_GetSatelliteData(satelliteId, monitorId);
            newSatelliteData = oldSatelliteData;

            queueStatus = Dem_QueueGetQueueStatus(oldSatelliteData);
            if (queueStatus == DEM_EVENTQUEUESTATE_INITIAL)
            {
                Dem_DebounceInfoType     debounceInfo;
                Dem_BaseInfoConstPtrType baseInfo;

                baseInfo         = Dem_DebounceInfoInit(EventId, &debounceInfo, oldSatelliteData);
                newSatelliteData = Dem_ResetDebouncing(baseInfo, newSatelliteData);

                queueStatus      = Dem_QueueSetResetTFRequested(queueStatus);
                newSatelliteData = Dem_SetSatelliteQueueStatus(newSatelliteData, queueStatus);
                processingDone   = Dem_SyncSetSatelliteData(satelliteId, monitorId, oldSatelliteData, newSatelliteData);
                if (processingDone == TRUE)
                {
                    ret = E_OK;
                    Dem_SetSatelliteInfoEventQueue(satelliteId);
                }
            }
            else
            {
                /** Event is already queued, so there was a qualified result. Technically, reseteventstatus is not
                 * allowed to interrupt seteventstatus, so the queue status should not change in this loop - better
                 * be paranoid though */
                processingDone = TRUE;
            }
        } while (processingDone == FALSE);
    }
    return ret;
}

/**
 * @brief Reset the Monitor status of an event.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
DEM_LOCAL Std_ReturnType Dem_SatelliteResetMonitorStatus(Dem_EventIdType EventId)
/* PRQA S 6070 -- */
{
    Std_ReturnType ret = E_NOT_OK;
    if ((Dem_GetAvailableInVariantByEvent(EventId) == TRUE) && (Dem_GetEventAvailableStatusByEventId(EventId) == TRUE))
    {
        Dem_SatelliteIdType   satelliteId = Dem_GetSatelliteIdByEventId(EventId);
        Dem_MonitorIdType     monitorId   = Dem_GetMonitorIdByEventId(EventId);
        Dem_SatelliteDataType oldSatelliteData;
        Dem_MonitorStatusType monitorStatus;

        oldSatelliteData = Dem_GetSatelliteData(satelliteId, monitorId);
        monitorStatus    = Dem_GetSatelliteMonitorStatus(oldSatelliteData);
        if (DEM_FLAG_ISSET(monitorStatus, DEM_MONITOR_STATUS_TNCTOC))
        {
            boolean processingDone;
            do
            {
                Dem_SatelliteDataType    newSatelliteData;
                Dem_EventQueueStatusType queueStatus;

                oldSatelliteData = Dem_GetSatelliteData(satelliteId, monitorId);
                newSatelliteData = oldSatelliteData;

                queueStatus = Dem_QueueGetQueueStatus(oldSatelliteData);
                if (queueStatus == DEM_EVENTQUEUESTATE_INITIAL)
                {
                    Dem_DebounceInfoType     debounceInfo;
                    Dem_BaseInfoConstPtrType baseInfo;

                    baseInfo         = Dem_DebounceInfoInit(EventId, &debounceInfo, oldSatelliteData);
                    newSatelliteData = Dem_ResetDebouncing(baseInfo, newSatelliteData);
                    monitorStatus    = Dem_GetSatelliteMonitorStatus(oldSatelliteData);
                    /** Reset Monitor Status TF */
                    /* PRQA S 3473, 4399, 4461 ++ */ /* VL_Dem_3473, VL_Dem_4399, VL_Dem_4461 */
                    DEM_FLAG_RESET(monitorStatus, DEM_MONITOR_STATUS_TF);
                    /* PRQA S 3473, 4399, 4461 -- */
                    newSatelliteData = Dem_SetSatelliteMonitorStatus(newSatelliteData, monitorStatus);
                    processingDone =
                        Dem_SyncSetSatelliteData(satelliteId, monitorId, oldSatelliteData, newSatelliteData);
                }
                else
                {
                    /** Event is already queued, so there was a qualified result. Technically, reseteventstatus is not
                     * allowed to interrupt seteventstatus, so the queue status should not change in this loop - better
                     * be paranoid though */
                    processingDone = TRUE;
                }
            } while (processingDone == FALSE);
            ret = E_OK;
        }
    }
    return ret;
}
/* PRQA S 0759 -- */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
