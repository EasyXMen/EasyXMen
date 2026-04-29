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
 **  @file               : Dem_SatelliteInterface.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : Declaration file of Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_SATELLITE_INTERFACE_H
#define DEM_SATELLITE_INTERFACE_H

/* =================================================== inclusions =================================================== */
#include "Dem_Lcfg.h"

/* ===================================================== macros ===================================================== */
/** First satellite partition, used only in a single partition */
#define DEM_SATELLITE_APPLICATION_ZERO 0u

/** Dem Monitor Status macros encoding monitor states. Macros encoding monitor states. */
#define DEM_MONITOR_STATUS_INITIAL ((Dem_MonitorStatusType)DEM_MONITOR_STATUS_TNCTOC) /**< Initial value */
/** Bit mapping within SharedStatus in SatelliteData */
/** The first debounce status bit within SharedStatus in SatelliteData  */
#define DEM_DEBOUNCE_STATUS_POS 4u
/** Bit mask for monitor status bit(s) */
#define DEM_MONITOR_STATUS (0x3u)
/** Bit mask for debounce status bit(s) */
#define DEM_DEBOUNCE_STATUS (0xF0u)
/** Bit mask for prestore freeze frame status bit */
#define DEM_PRESTOREFF_STATUS (0x3u)

/* ========================================== internal function declarations ======================================== */
/**
 * @brief         Get DebounceData of Satellite data
 * @param[in]     SatelliteData: Satellite data
 * @return        Dem_DebounceDataType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_DebounceDataType Dem_GetSatelliteDebounceData(Dem_SatelliteDataType SatelliteData);

/**
 * @brief         Get DebounceStatus of Satellite data
 * @param[in]     SatelliteData: Satellite data
 * @return        Dem_DebounceStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_DebounceStatusType Dem_GetSatelliteDebounceStatus(Dem_SatelliteDataType SatelliteData);

/**
 * @brief         Get SatelliteId by EventId
 * @param[in]     EventId: Event Id
 * @return        Dem_SatelliteIdType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_SatelliteIdType Dem_GetSatelliteIdByEventId(Dem_EventIdType EventId);

/**
 * @brief         Get MonitorId by EventId
 * @param[in]     EventId: Event Id
 * @return        Dem_MonitorIdType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_MonitorIdType Dem_GetMonitorIdByEventId(Dem_EventIdType EventId);

/**
 * @brief         Set DebounceData of Satellite data
 * @param[in]     SatelliteData: Satellite data
 * @param[in]     DebounceData: Debounce data
 * @return        Dem_SatelliteDataType
 * @return        Data: The detailed data type
 * @return        Raw: The raw data type intended for queue communication
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_SatelliteDataType
    Dem_SetSatelliteDebounceData(Dem_SatelliteDataType SatelliteData, Dem_DebounceDataType DebounceData);

/**
 * @brief         Set DebounceStatus of Satellite data
 * @param[in]     SatelliteData: Satellite data
 * @param[in]     DebounceStatus: Debounce status
 * @return        Dem_SatelliteDataType
 * @return        Data: The detailed data type
 * @return        Raw: The raw data type intended for queue communication
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_SatelliteDataType
    Dem_SetSatelliteDebounceStatus(Dem_SatelliteDataType SatelliteData, Dem_DebounceStatusType DebounceStatus);

/**
 * @brief         Set QueueStatus of Satellite data
 * @param[in]     SatelliteData: Satellite data
 * @param[in]     QueueStatus: Queue status
 * @return        Dem_SatelliteDataType
 * @return        Data: The detailed data type
 * @return        Raw: The raw data type intended for queue communication
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_SatelliteDataType
    Dem_SetSatelliteQueueStatus(Dem_SatelliteDataType SatelliteData, Dem_EventQueueStatusType QueueStatus);

/**
 * @brief         Get QueueStatus of Satellite data
 * @param[in]     SatelliteData: Satellite data
 * @return        Dem_EventQueueStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventQueueStatusType Dem_GetSatelliteQueueStatus(Dem_SatelliteDataType SatelliteData);

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
DEM_LOCAL_INLINE Dem_SatelliteInfoType Dem_GetSatelliteInfo(Dem_SatelliteIdType SatelliteId);

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
DEM_LOCAL_INLINE Dem_SatelliteInfoType Dem_SetSatelliteInfoPrestoreQueue(Dem_SatelliteInfoType SatelliteInfo);

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
DEM_LOCAL_INLINE Dem_SatelliteInfoType Dem_SatelliteInfoResetPrestoreQueue(Dem_SatelliteInfoType SatelliteInfo);

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
DEM_LOCAL_INLINE boolean Dem_CheckSatelliteInfoPrestoreQueue(Dem_SatelliteInfoType SatelliteInfo);
#endif

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
    Dem_GetSatelliteData(Dem_SatelliteIdType SatelliteId, Dem_MonitorIdType MonitorId);

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
DEM_LOCAL_INLINE boolean Dem_CheckAndResetSatelliteInfoEventQueue(Dem_SatelliteIdType SatelliteId);

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
    Dem_ConsumeEventQueueStatus(Dem_SatelliteIdType SatelliteId, Dem_MonitorIdType MonitorId);

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
    Dem_GetSatelliteExtendedData(Dem_SatelliteIdType SatelliteId, Dem_MonitorIdType MonitorId);

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
    Dem_SatelliteExtendedDataType SatelliteExtendedData);

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
    Dem_SatelliteExtendedDataType NewSatelliteEData);
#endif

/**
 * @brief         Get SatelliteId by EventId
 * @param[in]     EventId: Event Id
 * @return        Dem_SatelliteIdType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_SatelliteIdType Dem_GetSatelliteIdByEventId(Dem_EventIdType EventId);

/**
 * @brief         Get MonitorId by EventId
 * @param[in]     EventId: Event Id
 * @return        Dem_MonitorIdType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_MonitorIdType Dem_GetMonitorIdByEventId(Dem_EventIdType EventId);

/**
 * @brief         Get EventId by SatelliteId and MonitorId
 * @param[in]     SatelliteId: Satellite Id
 * @param[in]     MonitorId: Monitor Id
 * @return        Dem_EventIdType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventIdType Dem_SatelliteGetEventId(Dem_SatelliteIdType SatelliteId, Dem_MonitorIdType MonitorId);

/**
 * @brief         Resets the debouncing of an event.
 * @param[in]     EventId: Event Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_SatelliteResetDebounceStatus(Dem_EventIdType EventId);

/**
 * @brief         Freeze the debouncing of an event.
 * @param[in]     EventId: Event Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_SatelliteFreezeDebounceStatus(Dem_EventIdType EventId);

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
    Dem_SatelliteGetMaxFaultDetectionCounter(Dem_SatelliteIdType SatelliteId, Dem_MonitorIdType MonitorId);
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
    Dem_GetSatelliteMonitorData0(Dem_SatelliteIdType SatelliteId, Dem_MonitorIdType MonitorId);

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
    Dem_GetSatelliteMonitorData1(Dem_SatelliteIdType SatelliteId, Dem_MonitorIdType MonitorId);
#endif

/**
 * @brief         Synchronized satellite information
 * @param[in]     SatelliteId: Satellite Id
 * @param[in]     OldSatelliteInfo: Old satellite information
 * @param[in]     NewSatelliteInfo: Old satellite information
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_SyncSetSatelliteInfo(
    Dem_SatelliteIdType   SatelliteId,
    Dem_SatelliteInfoType OldSatelliteInfo,
    Dem_SatelliteInfoType NewSatelliteInfo);

#if ((defined(DEM_J1939LAMP_STATUS)) && (DEM_INDICATOR_NUMBER > 0u))
/**
 * @brief         calculate a 'DTC with lamp status' filter for the given client
 * Declaration for Dem_J1939.h
 * @param[in]     ClientId: Client Id
 * @param[in]     EventId: Event Id
 * @return        Dem_J1939DcmLampStatusType
 * @return        LampStatus: lamp status
 * @return        FlashLampStatus: flash lamp status
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Dem_J1939DcmLampStatusType Dem_CalculateEventJ1939LampStatus(uint8 ClientId, Dem_EventIdType EventId);
#endif

#if (DEM_DEBOUNCE_COUNTER_STORAGE_NUM > 0u)
/**
 * @brief         Restores the given debouncing value of an event.
 * Value is restored, if
 * - the given debouncing value is not qualified
 * - the debouncing value is qualified and the reached threshold equals the ExpectedThreshold.
 * @param[in]     EventId: Event Id
 * @param[in]     Value: FDC value
 * @param[in]     ExpectedThreshold: The expect threshold
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_SatelliteRestoreDebounceValue(Dem_EventIdType EventId, sint16 Value, boolean ExpectedThreshold);
#endif

#if (DEM_EVENT_PARTITION_NUMBER > 1u)
/**
 * @brief         Get ApplicationId of Satellite Info by SatelliteId
 * @param[in]     SatelliteId: Satellite Id
 * @return        ApplicationType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE ApplicationType Dem_GetSatelliteApplicationId(Dem_SatelliteIdType SatelliteId);
#endif
#endif
