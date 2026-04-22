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
 **  @file               : FiM.c
 **  @author             : tao.yu
 **  @date               : 2024/12/04
 **  @vendor             : isoft
 **  @description        : Implementation file of FiM
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Dem.h"
#include "FiM_Cfg.h"
#include "FiM_Dem.h"
#include "FiM.h"
#include "SchM_FiM.h"
#if (STD_ON == FIM_DEV_ERROR_DETECT)
#include "Det.h"
#endif

/* ===================================================== macros ===================================================== */
#if (STD_ON == FIM_DEV_ERROR_DETECT)
#define FIM_DET_REPORT(ApiId, ErrorId) (void)Det_ReportError(FIM_MODULE_ID, FIM_INSTANCE_ID, ApiId, ErrorId)
#endif

#define FIM_EVENT_ID_INVALID    (0u) /** Symbolic name of invalid event ID (derived from Dem SWS) */
#define FIM_FUNCTION_ID_INVALID (0u) /** Symbolic name of invalid Function ID (derived from FiM SWS) */

#define FIM_UNINITIALIZED  (0u) /** Initialization state directly after system startup */
#define FIM_PREINITIALIZED (1u) /** Initialization state after FiM_Init */
#define FIM_INITIALIZED    (2u) /** Initialization state after FiM_DemInit */

#define FIM_INHCODE_NUMBER (4u) /** inhibition mask number */

#define FIM_LAST_FAILED_MASK         (Dem_MonitorStatusType)0x01u /** TF mask */
#define FIM_LAST_FAILED_RESULT       (Dem_MonitorStatusType)0x01u /** TF set */
#define FIM_TESTED_MASK              (Dem_MonitorStatusType)0x02u /** TNCTOC mask */
#define FIM_TESTED_RESULT            (Dem_MonitorStatusType)0x00u /** TNCTOC not set */
#define FIM_NOT_TESTED_MASK          (Dem_MonitorStatusType)0x02u /** TNCTOC mask */
#define FIM_NOT_TESTED_RESULT        (Dem_MonitorStatusType)0x02u /** TNCTOC set */
#define FIM_TESTED_AND_FAILED_MASK   (Dem_MonitorStatusType)0x03u /** TNCTOC | TF mask */
#define FIM_TESTED_AND_FAILED_RESULT (Dem_MonitorStatusType)0x01u /** TNCTOC not set, TF set */

/** Whether an event or an summary event is configured to be handled */
#if (FIM_DEM_EVENT_NUMBER > 0u)
#define FIM_EVENT_INHBITION STD_ON
#else
#define FIM_EVENT_INHBITION STD_OFF
#endif

/** Whether an component is configured to be handled */
#if (FIM_DEM_COMPONENT_NUMBER > 0u)
#define FIM_COMPONENT_INHBITION STD_ON
#else
#define FIM_COMPONENT_INHBITION STD_OFF
#endif

/* PRQA S 3472 ++ */ /* VL_Dem_3472 */
/** Status Bits Functions */
/** _p = &uint8_array[0] */
/** _n = pos */
#define FIM_BITS_SET(_p, _n)    ((_p)[((_n) >> 3u)] |= ((uint8)(1u << ((_n) & 7u))))
#define FIM_BITS_RESET(_p, _n)  ((_p)[((_n) >> 3u)] &= ((uint8)(~((uint8)(1u << ((_n) & 7u))))))
#define FIM_BITS_ISSET(_p, _n)  (((_p)[((_n) >> 3u)] & ((uint8)(1u << ((_n) & 7u)))) != 0u)
#define FIM_BITS_ISNSET(_p, _n) (((_p)[((_n) >> 3u)] & ((uint8)(1u << ((_n) & 7u)))) == 0u)
/* PRQA S 3472 -- */

/** Enter critical section  */
#define FiM_EnterCritical() SchM_Enter_FiM_FIM_EXCLUSIVE_AREA_0() /* PRQA S 3472 */ /* VL_FiM_3472 */
/** Leave critical section  */
#define FiM_LeaveCritical() SchM_Exit_FiM_FIM_EXCLUSIVE_AREA_0() /* PRQA S 3472 */ /* VL_FiM_3472 */

/* ================================================ type definitions ================================================ */
/**
 * @brief info for the calculation of a FID depending on the inhibition configuration.
 */
typedef struct
{
    Dem_MonitorStatusType Mask;   /** Status information of concern */
    Dem_MonitorStatusType Result; /** Desired state results */
} FiM_InhMaskInfoType;

/* ========================================== internal function declarations ======================================== */
#if (FIM_EVENT_UPDATE_TRIGGERED_BY_DEM == STD_ON)
#if (FIM_EVENT_INHBITION == STD_ON)

/**
 * @brief         Calculate the corresponding FID state based on the inhIndex
 * @param[in]     FidContext: Fid processing data information
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71132
 */
FIM_LOCAL void FiM_DoFidCalculateByInhIndex(FiM_FidContextType FidContext);

/**
 * @brief         Calculate the corresponding FID state based on the EventTableRef
 * @param[in]     EventTableRef: Event Table Ref value
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71133
 */
FIM_LOCAL void FiM_CalculateByEvent(FiM_EventNumType EventTableRef);

/**
 * @brief         get the EventTableRef by Event Id.
 * @param[in]     EventId: Event Id
 * @param[out]    EventTableRef: Event Table Ref value
 * @return        boolean
 * @retval        TRUE: Event Table Found
 * @retval        FALSE: Event Table not Found
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71134
 */
FIM_LOCAL boolean FiM_SearchEventTable(Dem_EventIdType EventId, FiM_EventNumType* EventTableRef);
#endif

#if (FIM_COMPONENT_INHBITION == STD_ON)
/**
 * @brief         Dem_GetComponentFailed returns E_OK, the FIM shall consider this event in its inhibition mask
 *                calculation
 * @param[in]     FID: Identification of a functionality by assigned FID.
 * @param[in]     ComponentStatusStart: Component Status after the Dem_GetComponentFailed call
 * @param[in]     OldComponentStatus: Component Status before the Dem_GetComponentFailed call
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71135
 */
FIM_LOCAL void
    FiM_ComponentResultCalculate(FiM_FunctionIdType FID, boolean ComponentStatusStart, boolean OldComponentStatus);

/**
 * @brief         Calculate the corresponding FID state based on the ComponentRef
 * @param[in]     ComponentRef: Component Ref value
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71136
 */
FIM_LOCAL void FiM_CalculateByComponent(FiM_ComponentNumType ComponentRef);

/**
 * @brief         get the ComponentRef by Component Id.
 * @param[in]     ComponentId: Component Id
 * @param[out]    ComponentRef: Component Ref value
 * @return        boolean
 * @retval        TRUE: component Found
 * @retval        FALSE: component not Found
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71137
 */
FIM_LOCAL boolean FiM_SearchComponentTable(Dem_ComponentIdType ComponentId, FiM_ComponentNumType* ComponentRef);
#endif

/**
 * @brief         get the permission state by Fid Counter.
 * @param[in]     FID: Identification of a functionality by assigned FID.
 * @return        boolean
 * @retval        TRUE: FID has permission to run
 * @retval        FALSE: FID has no permission to run, i.e. shall not be executed
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71138
 */
FIM_LOCAL boolean FiM_GetFidPermission(FiM_FunctionIdType FID);
#endif

#if (FIM_EVENT_UPDATE_TRIGGERED_BY_DEM == STD_OFF)
#if (FIM_EVENT_INHBITION == STD_ON)
/**
 * @brief         calculates the permission of a FID by looping through all associated events
 * @param[in]     FidInhTable: This container includes a InhibitionConfiguration all information
 * @return        boolean
 * @retval        TRUE: FID has permission to run
 * @retval        FALSE: FID has no permission to run, i.e. shall not be executed
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71139
 */
FIM_LOCAL boolean FiM_CalculateFIDEventInhibition(const FiM_FidInhTableType* FidInhTable);
#endif

#if (FIM_COMPONENT_INHBITION == STD_ON)
/**
 * @brief         calculates the permission of a FID by looping through all associated Component
 * @param[in]     FidInhTable: This container includes a InhibitionConfiguration all information
 * @return        boolean
 * @retval        TRUE: FID has permission to run
 * @retval        FALSE: FID has no permission to run, i.e. shall not be executed
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71140
 */
FIM_LOCAL boolean FiM_CalculateFIDComponentInhibition(const FiM_FidInhTableType* FidInhTable);
#endif

/**
 * @brief         calculates the permission of a FID by InhibitionConfiguration
 * @param[in]     FidInhTable: This container includes a InhibitionConfiguration all information
 * @return        boolean
 * @retval        TRUE: FID has permission to run
 * @retval        FALSE: FID has no permission to run, i.e. shall not be executed
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71141
 */
FIM_LOCAL boolean FiM_CalculateFIDInhibition(const FiM_FidInhTableType* FidInhTable);

/**
 * @brief         calculates the permission of a FID
 * @param[in]     FID: Identification of a functionality by assigned FID.
 * @return        boolean
 * @retval        TRUE: FID has permission to run
 * @retval        FALSE: FID has no permission to run, i.e. shall not be executed
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71142
 */
FIM_LOCAL boolean FiM_CalculateFIDPermission(FiM_FunctionIdType Fid);

#if (FIM_FEATURE_POLLING_MODE_ASY == STD_ON)
/**
 * @brief         Handles FID permission
 * @param[in]     FID: Identification of a functionality by assigned FID.
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71143
 */
FIM_LOCAL void FiM_HandleFid(FiM_FunctionIdType FID);
#endif
#endif

/* ============================================ internal data definitions =========================================== */
/**
 * @brief store nm post build configuration data pointer
 */
#define FIM_START_SEC_VAR_CLEARED_PTR
#include "FiM_MemMap.h"
FIM_LOCAL const FiM_ConfigType* FiM_PBcfgPtr;
#define FIM_STOP_SEC_VAR_CLEARED_PTR
#include "FiM_MemMap.h"

/**
 * @brief used in calculation of effects of event status changes
 */
#define FIM_START_SEC_CONST_UNSPECIFIED
#include "FiM_MemMap.h"
/* PRQA S 3218 ++ */ /* VL_FiM_3218 */
FIM_LOCAL const FiM_InhMaskInfoType FiM_InhMaskInfo[FIM_INHCODE_NUMBER] = {
    {
        /* FIM_LAST_FAILED */
        FIM_LAST_FAILED_MASK,
        FIM_LAST_FAILED_RESULT,
    },
    {
        /* FIM_NOT_TESTED */
        FIM_NOT_TESTED_MASK,
        FIM_NOT_TESTED_RESULT,
    },
    {
        /* FIM_TESTED */
        FIM_TESTED_MASK,
        FIM_TESTED_RESULT,
    },
    {
        /* FIM_TESTED_AND_FAILED */
        FIM_TESTED_AND_FAILED_MASK,
        FIM_TESTED_AND_FAILED_RESULT,
    },
};
/* PRQA S 3218 -- */
#define FIM_STOP_SEC_CONST_UNSPECIFIED
#include "FiM_MemMap.h"

/* ========================================== internal function declarations ======================================== */

/* ========================================== internal function definitions ========================================= */
#define FIM_START_SEC_CODE
#include "FiM_MemMap.h"

#if (FIM_USER_DEFINED_SYNC_API == STD_OFF)
/**
 * @brief         Default function to synchronize variable information between partitions
 * @param[in]     AddressPtr: Satellite id
 * @param[in]     OldValue: Old Value
 * @param[in]     NewValue: New Value
 * @return        boolean
 * @retval        TRUE: Sync Compare And Swap successful
 * @retval        FALSE: Sync Compare And Swap failed
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71144
 */
FIM_LOCAL_INLINE boolean FiM_DefaultSyncCompareAndSwap(
    FiM_ProcessorWordType* const AddressPtr,
    FiM_ProcessorWordType        OldValue,
    FiM_ProcessorWordType        NewValue)
{
    boolean ret = FALSE;
    /** Enter Critical Section */
    FiM_EnterCritical();
    if (*AddressPtr == OldValue)
    {
        *AddressPtr = NewValue;
        ret         = TRUE;
    }
    FiM_LeaveCritical();
    /** Leave Critical Section */
    return ret;
}
#endif

/**
 * @brief         Inter-partition variable information synchronization
 * @param[in]     AddressPtr: Satellite id
 * @param[in]     OldValue: Old Value
 * @param[in]     NewValue: New Value
 * @return        boolean
 * @retval        TRUE: Sync Compare And Swap successful
 * @retval        FALSE: Sync Compare And Swap failed
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71145
 */
FIM_LOCAL_INLINE boolean FiM_ComSyncCompareAndSwap(
    FiM_ProcessorWordType* const AddressPtr,
    FiM_ProcessorWordType        OldValue,
    FiM_ProcessorWordType        NewValue)
{
#if (FIM_USER_DEFINED_SYNC_API == STD_ON)
    return FiM_UserDefineSyncCompareAndSwap(AddressPtr, OldValue, NewValue);
#else
    return FiM_DefaultSyncCompareAndSwap(AddressPtr, OldValue, NewValue);
#endif
}

/**
 * @brief         get Satellite Init Status
 * @param[in]     SatelliteId: Satellite id
 * @return        FiM_ProcessorWordType: Satellite Init Status
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71146
 */
FIM_LOCAL_INLINE FiM_ProcessorWordType FiM_GetSatelliteInitStatus(FiM_SatelliteIdType SatelliteId)
{
    return *(FiM_GetInitStatusOfSatellite(SatelliteId));
}

/**
 * @brief         set Satellite Init Status
 * @param[in]     SatelliteId: Satellite id
 * @param[in]     InitStatus: Init Status
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71147
 */
FIM_LOCAL_INLINE void FiM_SetSatelliteInitStatus(FiM_SatelliteIdType SatelliteId, FiM_ProcessorWordType InitStatus)
{
    FiM_ProcessorWordType oldInitStatus;
    FiM_ProcessorWordType newInitStatus;
    do
    {
        oldInitStatus = FiM_GetSatelliteInitStatus(SatelliteId);
        newInitStatus = InitStatus;
    } while (FiM_ComSyncCompareAndSwap(FiM_GetInitStatusOfSatellite(SatelliteId), oldInitStatus, newInitStatus)
             == FALSE);
}

/**
 * @brief         Get Satellite partition current id of FID
 * @param[in]     CurrentApplication: Application id
 * @return        FiM_SatelliteIdType: Satellite Id
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71148
 */
FIM_LOCAL_INLINE FiM_SatelliteIdType FiM_GetFidCurrentSatelliteId(ApplicationType CurrentApplication)
{
    FiM_SatelliteIdType satelliteId;
#if (FIM_SATELLITE_NUMBER > 1u)
    for (satelliteId = 0u; satelliteId < FIM_SATELLITE_NUMBER; ++satelliteId)
    {
        if (CurrentApplication == FiM_GetApplicationIdOfSatellite(satelliteId))
        {
            break;
        }
    }
#else
    FIM_PARAM_UNUSED(CurrentApplication);
    satelliteId = FIM_SATELLITE_APPLICATION_ZERO;
#endif
    return satelliteId;
}

#if (FIM_EVENT_UPDATE_TRIGGERED_BY_DEM == STD_ON)
/**
 * @brief         get Satellite Fid Counter By Fid
 * @param[in]     SatelliteId: Satellite id
 * @param[in]     FID: Identification of a functionality by assigned FID.
 * @return        FiM_FidCounterType: FID counter.
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71149
 */
FIM_LOCAL_INLINE FiM_FidCounterType FiM_GetFidCounterByFid(FiM_SatelliteIdType SatelliteId, FiM_FunctionIdType Fid)
{
    FiM_FunctionIdType FidIndex = Fid - FiM_GetFIDStartOfSatellite(SatelliteId);
    return FiM_GetFidCounterOfSatellite(SatelliteId)[FidIndex];
}

/**
 * @brief         set Satellite Fid Counter
 * @param[in]     SatelliteId: Satellite id
 * @param[in]     FID: Identification of a functionality by assigned FID.
 * @param[in]     FidCounter: FID counter.
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71150
 */
FIM_LOCAL_INLINE void
    FiM_SetFidCounter(FiM_SatelliteIdType SatelliteId, FiM_FunctionIdType Fid, FiM_FidCounterType FidCounter)
{
    FiM_FunctionIdType FidIndex = Fid - FiM_GetFIDStartOfSatellite(SatelliteId);
#if (FIM_SATELLITE_NUMBER > 1u)
    FiM_FidCounterType oldFidCounter;
    FiM_FidCounterType newFidCounter;
    do
    {
        oldFidCounter = FiM_GetFidCounterByFid(SatelliteId, Fid);
        newFidCounter = FidCounter;
    } while (
        FiM_ComSyncCompareAndSwap((&FiM_GetFidCounterOfSatellite(SatelliteId)[FidIndex]), oldFidCounter, newFidCounter)
        == FALSE);
#else

    *(&FiM_GetFidCounterOfSatellite(SatelliteId)[FidIndex]) = FidCounter;

#endif
}

/**
 * @brief         Decrease counter
 * @param[in]     FID: Identification of a functionality by assigned FID.
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71151
 */
FIM_LOCAL_INLINE void FiM_DecreaseFidCounter(FiM_FunctionIdType Fid)
{
    FiM_SatelliteIdType satelliteId = FiM_GetSatelliteIdOfFID(Fid);
    FiM_FidCounterType  counter     = FiM_GetFidCounterByFid(satelliteId, Fid);
    if (counter > 0u)
    {
        counter--;
        FiM_SetFidCounter(satelliteId, Fid, counter);
    }
}

/**
 * @brief         Increase counter
 * @param[in]     FID: Identification of a functionality by assigned FID.
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71152
 */
FIM_LOCAL_INLINE void FiM_IncreaseFidCounter(FiM_FunctionIdType Fid)
{
    FiM_SatelliteIdType satelliteId = FiM_GetSatelliteIdOfFID(Fid);
    FiM_FidCounterType  counter     = FiM_GetFidCounterByFid(satelliteId, Fid);
    if (counter < FIM_FID_COUNTER_MAX_VALUE)
    {
        counter++;
        FiM_SetFidCounter(satelliteId, Fid, counter);
    }
}

/**
 * @brief         Get Satellite partition current id of Event
 * @param[in]     CurrentApplication: Application id
 * @return        FiM_EventSatelliteIdType: event Satellite Id
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71153
 */
FIM_LOCAL_INLINE FiM_EventSatelliteIdType FiM_GetEventCurrentSatelliteId(ApplicationType CurrentApplication)
{
    FiM_EventSatelliteIdType           satelliteId;
    const FiM_EventSatelliteTableType* eventSatelliteTable = FiM_PBcfgPtr->EventSatelliteTable;
    FiM_EventSatelliteIdType           eventSatelliteNum   = FiM_PBcfgPtr->EventSatelliteNum;
    for (satelliteId = 0u; satelliteId < eventSatelliteNum; ++satelliteId)
    {
        if (CurrentApplication == eventSatelliteTable->ApplicationIdOfSatellite)
        {
            break;
        }
        eventSatelliteTable++;
    }
    return satelliteId;
}

#if (FIM_EVENT_INHBITION == STD_ON)
/**
 * @brief         save MonitorStatus by EventTableRef, used for next trigger to compare
 * @param[in]     EventTableRef: Event Table Ref value
 * @param[in]     MonitorStatus: Monitor Status
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71154
 */
FIM_LOCAL_INLINE void
    FiM_SetMonitorStatusByEventTableRef(Dem_EventIdType EventTableRef, Dem_MonitorStatusType MonitorStatus)
{
    *(FiM_PBcfgPtr->EventTable[EventTableRef].MonitorStatus) = MonitorStatus;
}

/**
 * @brief         Calculate the corresponding FID state based on the inhIndex
 * @param[in]     FidContext: Fid processing data information
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71132
 */
FIM_LOCAL void FiM_DoFidCalculateByInhIndex(FiM_FidContextType FidContext)
{
    boolean                                newInhibStat;
    boolean                                oldInhibStat;
    FiM_InhCfgNumType                      inhStart         = FidContext.InhStart;
    FiM_InhCfgNumType                      inhEnd           = FidContext.InhEnd;
    Dem_MonitorStatusType                  neWMonitorStatus = FidContext.NeWMonitorStatus;
    Dem_MonitorStatusType                  oldMonitorStatus = FidContext.OldMonitorStatus;
    const FiM_InhibitionConfigurationType* inhCfgPtr        = FiM_PBcfgPtr->InhibitionConfiguration;
    const FiM_InhRefNumType*               InhRef           = FiM_PBcfgPtr->InhRefTable;
    for (FiM_InhCfgNumType inhIndex = inhStart; inhIndex < inhEnd; ++inhIndex)
    {
        const FiM_InhibitionConfigurationType inhCfg    = inhCfgPtr[InhRef[inhIndex]];
        FiMInhInhibitionMaskType              maskIndex = inhCfg.InhInhibitionMask;
        FiM_FunctionIdType                    fid       = inhCfg.InhFunctionIdRef;
        Dem_MonitorStatusType                 mask      = FiM_InhMaskInfo[maskIndex].Mask;
        Dem_MonitorStatusType                 result    = FiM_InhMaskInfo[maskIndex].Result;

        newInhibStat = ((mask & neWMonitorStatus) == result);
        oldInhibStat = ((mask & oldMonitorStatus) == result);
        if (newInhibStat == TRUE)
        {
            FiM_SatelliteIdType satelliteId = FiM_GetSatelliteIdOfFID(fid);
            if ((oldInhibStat == FALSE) || (FIM_PREINITIALIZED == FiM_GetSatelliteInitStatus(satelliteId)))
            {
                /** Increase counter */
                FiM_IncreaseFidCounter(fid);
            }
        }
        else
        {
            if (oldInhibStat == TRUE)
            {
                /** Decrease Counter */
                FiM_DecreaseFidCounter(fid);
            }
        }
    }
}

/**
 * @brief         Calculate the corresponding FID state based on the EventTableRef
 * @param[in]     satelliteId: satellite Id
 * @param[in]     EventTableRef: Event Table Ref value
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71133
 */
FIM_LOCAL void FiM_CalculateByEvent(FiM_EventNumType EventTableRef)
{
    const FiM_EventTableType eventTable       = FiM_PBcfgPtr->EventTable[EventTableRef];
    Dem_MonitorStatusType    oldMonitorStatus = *(eventTable.MonitorStatus);
    Dem_EventIdType          eventId          = eventTable.EventId;
    Std_ReturnType           retStart, retEnd;
    Dem_MonitorStatusType    monitorStatusStart, monitorStatusEnd;
#if (FIM_EVENT_INHBITION == STD_ON)
    FiM_InhRefNumType eventInhStart = eventTable.EventInhStart;
    FiM_InhRefNumType eventInhEnd   = eventTable.EventInhEnd;
#endif
    FiM_FidContextType FidContext;

    /** get monitor status for event from DEM   */
    retEnd = Dem_GetMonitorStatus(eventId, &monitorStatusEnd);

    /** Repeat the process until the return value matches the monitor state, to prevent inconsistencies in state updates
     * due to interruptions during processing */
    do
    {
        /** if repeat again, updata monitor information at begin */
        retStart           = retEnd;
        monitorStatusStart = monitorStatusEnd;
        /** If Dem_GetMonitorStatus returns E_NOT_OK, the FIM shall not consider this event in its inhibition mask
         * calculation */
        if (retStart == E_OK)
        {
            FidContext.NeWMonitorStatus = monitorStatusStart;
            FidContext.OldMonitorStatus = oldMonitorStatus;
#if (FIM_EVENT_INHBITION == STD_ON)
            /** do FiMInhibitionConfiguration FiMInhEventRef */
            FidContext.InhStart = eventInhStart;
            FidContext.InhEnd   = eventInhEnd;
            FiM_DoFidCalculateByInhIndex(FidContext);
#endif
        }
        /** get monitor status for event from DEM   */
        retEnd = Dem_GetMonitorStatus(eventId, &monitorStatusEnd);
    } while ((monitorStatusStart != monitorStatusEnd) || (retStart != retEnd));

    if (retStart == E_OK)
    {
        FiM_SetMonitorStatusByEventTableRef(EventTableRef, monitorStatusEnd);
    }
}

/**
 * @brief         get the EventTableRef by Event Id.
 * @param[in]     EventId: Event Id
 * @param[out]    EventTableRef: Event Table Ref value
 * @return        boolean
 * @retval        TRUE: Event Table Found
 * @retval        FALSE: Event Table not Found
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71134
 */
FIM_LOCAL boolean FiM_SearchEventTable(Dem_EventIdType EventId, FiM_EventNumType* EventTableRef)
{
    boolean                   eventFound    = FALSE;
    FiM_EventNumType          eventTableEnd = FiM_PBcfgPtr->EventRefNum;
    const FiM_EventTableType* eventTable    = FiM_PBcfgPtr->EventTable;
    for (FiM_EventNumType eventTableStart = 0u; eventTableStart < eventTableEnd; ++eventTableStart)
    {
        if (eventTable->EventId == EventId)
        {
            *EventTableRef = eventTableStart;
            eventFound     = TRUE;
            break;
        }
        eventTable++;
    }

    return eventFound;
}
#endif

#if (FIM_COMPONENT_INHBITION == STD_ON)
/**
 * @brief         save Component Status by ComponentRef, used for next trigger to compare
 * @param[in]     ComponentRef: Component Ref value
 * @param[in]     Status: component Failed Status
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71155
 */
FIM_LOCAL_INLINE void FiM_SetComponentStatus(FiM_ComponentNumType ComponentRef, boolean Status)
{
    if (TRUE == Status)
    {
        FIM_BITS_SET(FiM_PBcfgPtr->ComponentStatus, ComponentRef);
    }
    else
    {
        FIM_BITS_RESET(FiM_PBcfgPtr->ComponentStatus, ComponentRef);
    }
}

/**
 * @brief         get Component Status by ComponentRef
 * @param[in]    ComponentRef: Component Ref value
 * @return        boolean
 * @retval        TRUE: component Failed
 * @retval        FALSE: component not Failed
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71156
 */
FIM_LOCAL_INLINE boolean FiM_GetComponentStatus(FiM_ComponentNumType ComponentRef)
{
    return FIM_BITS_ISSET(FiM_PBcfgPtr->ComponentStatus, ComponentRef);
}

/**
 * @brief         Dem_GetComponentFailed returns E_OK, the FIM shall consider this event in its inhibition mask
 *                calculation
 * @param[in]     FID: Identification of a functionality by assigned FID.
 * @param[in]     ComponentStatusStart: Component Status after the Dem_GetComponentFailed call
 * @param[in]     OldComponentStatus: Component Status before the Dem_GetComponentFailed call
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71135
 */
FIM_LOCAL void
    FiM_ComponentResultCalculate(FiM_FunctionIdType FID, boolean ComponentStatusStart, boolean OldComponentStatus)
{
    if (ComponentStatusStart == TRUE)
    {
        FiM_SatelliteIdType satelliteId = FiM_GetSatelliteIdOfFID(FID);
        if ((OldComponentStatus == FALSE) || (FIM_PREINITIALIZED == FiM_GetSatelliteInitStatus(satelliteId)))
        {
            /** Increase counter */
            FiM_IncreaseFidCounter(FID);
        }
    }
    else
    {
        if (OldComponentStatus == TRUE)
        {
            /** Decrease Counter */
            FiM_DecreaseFidCounter(FID);
        }
    }
}

/**
 * @brief         Calculate the corresponding FID state based on the ComponentRef
 * @param[in]     ComponentRef: Component Ref value
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71136
 */
FIM_LOCAL void FiM_CalculateByComponent(FiM_ComponentNumType ComponentRef)
{
    FiM_ComponentTableType componentTable = FiM_PBcfgPtr->ComponentTable[ComponentRef];
    Dem_ComponentIdType    componentId    = componentTable.ComponentId;
    Std_ReturnType         retStart, retEnd;
    boolean                componentStatusStart, componentStatusEnd, oldComponentStatus;
    FiM_InhRefNumType      componentInhStart = componentTable.ComponentInhStart;
    FiM_InhRefNumType      componentInhEnd   = componentTable.ComponentInhEnd;

    oldComponentStatus = FiM_GetComponentStatus(ComponentRef);
    /** get status for component from DEM   */
    retEnd = Dem_GetComponentFailed(componentId, &componentStatusEnd);

    /** Repeat the process until the return value matches the component state, to prevent inconsistencies in state
     * updates due to interruptions during processing */
    do
    {
        /** if repeat again, updata component information at begin */
        retStart             = retEnd;
        componentStatusStart = componentStatusEnd;
        /** If Dem_GetComponentFailed returns E_NOT_OK, the FIM shall not consider this event in its inhibition mask
         * calculation */
        if (retStart == E_OK)
        {
            const FiM_InhibitionConfigurationType* inhCfgPtr = FiM_PBcfgPtr->InhibitionConfiguration;
            const FiM_InhRefNumType*               InhRef    = FiM_PBcfgPtr->InhRefTable;
            for (FiM_InhCfgNumType inhIndex = componentInhStart; inhIndex < componentInhEnd; ++inhIndex)
            {
                const FiM_InhibitionConfigurationType inhCfg = inhCfgPtr[InhRef[inhIndex]];
                FiM_FunctionIdType                    fid    = inhCfg.InhFunctionIdRef;
                FiM_ComponentResultCalculate(fid, componentStatusStart, oldComponentStatus);
            }
        }
        /** get status for component from DEM   */
        retEnd = Dem_GetComponentFailed(componentId, &componentStatusEnd);
    } while ((componentStatusStart != componentStatusEnd) || (retStart != retEnd));

    if (retStart == E_OK)
    {
        FiM_SetComponentStatus(ComponentRef, componentStatusEnd);
    }
}

/**
 * @brief         get the ComponentRef by Component Id.
 * @param[in]     ComponentId: Component Id
 * @param[out]    ComponentRef: Component Ref value
 * @return        boolean
 * @retval        TRUE: component Found
 * @retval        FALSE: component not Found
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71137
 */
FIM_LOCAL boolean FiM_SearchComponentTable(Dem_ComponentIdType ComponentId, FiM_ComponentNumType* ComponentRef)
{
    boolean                       componentFound    = FALSE;
    FiM_ComponentNumType          componentTableEnd = FiM_PBcfgPtr->ComponentRefNum;
    const FiM_ComponentTableType* componentTable    = FiM_PBcfgPtr->ComponentTable;
    for (FiM_ComponentNumType componentTableStart = 0u; componentTableStart < componentTableEnd; ++componentTableStart)
    {
        if (componentTable->ComponentId == ComponentId)
        {
            *ComponentRef  = componentTableStart;
            componentFound = TRUE;
            break;
        }
        componentTable++;
    }

    return componentFound;
}
#endif

/**
 * @brief         get the permission state by Fid Counter.
 * @param[in]     FID: Identification of a functionality by assigned FID.
 * @return        boolean
 * @retval        TRUE: FID has permission to run
 * @retval        FALSE: FID has no permission to run, i.e. shall not be executed
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71138
 */
FIM_LOCAL boolean FiM_GetFidPermission(FiM_FunctionIdType FID)
{
    boolean             fidPermission;
    FiM_SatelliteIdType satelliteId = FiM_GetSatelliteIdOfFID(FID);
    FiM_FidCounterType  counter     = FiM_GetFidCounterByFid(satelliteId, FID);
    if (0u == counter)
    {
        fidPermission = TRUE;
    }
    else
    {
        fidPermission = FALSE;
    }
    return fidPermission;
}
#endif

#if (FIM_EVENT_UPDATE_TRIGGERED_BY_DEM == STD_OFF)
#if (FIM_EVENT_INHBITION == STD_ON)
/**
 * @brief         calculates the permission of a FID by looping through all associated events
 * @param[in]     FidInhTable: This container includes a InhibitionConfiguration all information
 * @return        boolean
 * @retval        TRUE: FID has permission to run
 * @retval        FALSE: FID has no permission to run, i.e. shall not be executed
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71139
 */
FIM_LOCAL boolean FiM_CalculateFIDEventInhibition(const FiM_FidInhTableType* FidInhTable)
{
    boolean                 permission    = TRUE;
    const Dem_EventIdType*  eventIdTable  = FiM_PBcfgPtr->EventIdTable;
    const FiM_EventNumType* eventRefTable = FiM_PBcfgPtr->EventRefTable;
    FiM_EventRefNumType     eventRefStart = FidInhTable->EventRefStart;
    FiM_EventRefNumType     eventRefEnd   = FidInhTable->EventRefEnd;
    for (; ((eventRefStart < eventRefEnd) && (permission == TRUE)); ++eventRefStart)
    {
        FiM_EventNumType      eventRef = eventRefTable[eventRefStart];
        Dem_EventIdType       eventId  = eventIdTable[eventRef];
        Dem_MonitorStatusType monitorStatus;
        if (Dem_GetMonitorStatus(eventId, &monitorStatus) == E_OK)
        {
            FiMInhInhibitionMaskType maskIndex = FidInhTable->InhInhibitionMask;
            Dem_MonitorStatusType    mask      = FiM_InhMaskInfo[maskIndex].Mask;
            Dem_MonitorStatusType    result    = FiM_InhMaskInfo[maskIndex].Result;
            if ((mask & monitorStatus) == result)
            {
                permission = FALSE;
            }
        }
    }
    return permission;
}
#endif

#if (FIM_COMPONENT_INHBITION == STD_ON)
/**
 * @brief         calculates the permission of a FID by looping through all associated Component
 * @param[in]     FidInhTable: This container includes a InhibitionConfiguration all information
 * @return        boolean
 * @retval        TRUE: FID has permission to run
 * @retval        FALSE: FID has no permission to run, i.e. shall not be executed
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71140
 */
FIM_LOCAL boolean FiM_CalculateFIDComponentInhibition(const FiM_FidInhTableType* FidInhTable)
{
    boolean                     permission        = TRUE;
    const Dem_ComponentIdType*  componentIdTable  = FiM_PBcfgPtr->ComponentIdTable;
    const FiM_ComponentNumType* componentRefTable = FiM_PBcfgPtr->ComponentRefTable;
    FiM_ComponentRefNumType     componentRefStart = FidInhTable->ComponentRefStart;
    FiM_ComponentRefNumType     componentRefEnd   = FidInhTable->ComponentRefEnd;
    for (; ((componentRefStart < componentRefEnd) && (permission == TRUE)); ++componentRefStart)
    {
        FiM_ComponentNumType componentRef = componentRefTable[componentRefStart];
        Dem_ComponentIdType  componentId  = componentIdTable[componentRef];
        boolean              componentStatus;
        if (Dem_GetComponentFailed(componentId, &componentStatus) == E_OK)
        {
            if (componentStatus == TRUE)
            {
                permission = FALSE;
            }
        }
    }
    return permission;
}
#endif

/**
 * @brief         calculates the permission of a FID by InhibitionConfiguration
 * @param[in]     FidInhTable: This container includes a InhibitionConfiguration all information
 * @return        boolean
 * @retval        TRUE: FID has permission to run
 * @retval        FALSE: FID has no permission to run, i.e. shall not be executed
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71141
 */
FIM_LOCAL boolean FiM_CalculateFIDInhibition(const FiM_FidInhTableType* FidInhTable)
{
    boolean permission = TRUE;
#if (FIM_EVENT_INHBITION == STD_ON)
    permission = FiM_CalculateFIDEventInhibition(FidInhTable);
#endif
#if (FIM_COMPONENT_INHBITION == STD_ON)
#if (FIM_EVENT_INHBITION == STD_ON)
    if (permission == TRUE)
#endif
    {
        permission = FiM_CalculateFIDComponentInhibition(FidInhTable);
    }
#endif
    return permission;
}

/**
 * @brief         calculates the permission of a FID
 * @param[in]     FID: Identification of a functionality by assigned FID.
 * @return        boolean
 * @retval        TRUE: FID has permission to run
 * @retval        FALSE: FID has no permission to run, i.e. shall not be executed
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71142
 */
FIM_LOCAL boolean FiM_CalculateFIDPermission(FiM_FunctionIdType Fid)
{
    boolean                    permission     = TRUE;
    const FiM_FidTableType     fidTable       = FiM_PBcfgPtr->FidTable[Fid];
    FiM_InhCfgNumType          inhCfgRefStart = fidTable.InhCfgRefStart;
    FiM_InhCfgNumType          inhCfgRefEnd   = fidTable.InhCfgRefEnd;
    const FiM_FidInhTableType* fidInhTable    = &FiM_PBcfgPtr->FidInhTable[inhCfgRefStart];

    for (; ((inhCfgRefStart < inhCfgRefEnd) && (permission == TRUE)); ++inhCfgRefStart)
    {
        permission = FiM_CalculateFIDInhibition(fidInhTable);
        fidInhTable++;
    }
    return permission;
}

#if (FIM_FEATURE_POLLING_MODE_ASY == STD_ON)
/**
 * @brief         get the permission of an inhibition for a Fid.
 * @param[in]     FID: Identification of a functionality by assigned FID.
 * @return        boolean
 * @retval        TRUE: FID has permission to run
 * @retval        FALSE: FID has no permission to run, i.e. shall not be executed
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71157
 */
FIM_LOCAL_INLINE boolean FiM_GetFidPermissionFlag(FiM_FunctionIdType Fid)
{
    FiM_SatelliteIdType satelliteId = FiM_GetSatelliteIdOfFID(Fid);
    FiM_FunctionIdType  FidIndex    = Fid - FiM_GetFIDStartOfSatellite(satelliteId);
    return FIM_BITS_ISNSET((uint8*)FiM_GetInhStatusOfSatellite(satelliteId), FidIndex);
}

/**
 * @brief         set the permission of an inhibition for a Fid.
 * @param[in]     FID: Identification of a functionality by assigned FID.
 * @param[in]     Permission: TRUE: FID has permission to run
 *                            FALSE: FID has no permission to run, i.e. shall not be executed
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71158
 */
FIM_LOCAL_INLINE void FiM_SetFidPermissionFlag(FiM_FunctionIdType Fid, boolean Permission)
{
    FiM_SatelliteIdType satelliteId = FiM_GetSatelliteIdOfFID(Fid);
    FiM_FunctionIdType  FidIndex    = Fid - FiM_GetFIDStartOfSatellite(satelliteId);
    if (Permission == FALSE)
    {
        FIM_BITS_SET((uint8*)FiM_GetInhStatusOfSatellite(satelliteId), FidIndex);
    }
    else
    {
        FIM_BITS_RESET((uint8*)FiM_GetInhStatusOfSatellite(satelliteId), FidIndex);
    }
}

/**
 * @brief         Handles FID permission
 * @param[in]     FID: Identification of a functionality by assigned FID.
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71143
 */
FIM_LOCAL void FiM_HandleFid(FiM_FunctionIdType FID)
{
    boolean oldPermission = FiM_GetFidPermissionFlag(FID);
    boolean permission    = FiM_CalculateFIDPermission(FID);

    /* chekc permission has changed */
    if (permission != oldPermission)
    {
        /* Set Fid permission */
        FiM_SetFidPermissionFlag(FID, permission);
    }
}
#endif
#endif

#if (STD_ON == FIM_AVAILABILITY_SUPPORT)
/**
 * @brief         get the Availability of a Fid.
 * @param[in]     FID: Identification of a functionality by assigned FID.
 * @return        boolean
 * @retval        TRUE: Function is available.
 * @retval        FALSE: Function is not available.
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71159
 */
FIM_LOCAL_INLINE boolean FiM_GetFidAvailabilityFlag(FiM_FunctionIdType Fid)
{
    FiM_SatelliteIdType satelliteId = FiM_GetSatelliteIdOfFID(Fid);
    FiM_FunctionIdType  FidIndex    = Fid - FiM_GetFIDStartOfSatellite(satelliteId);
    return FIM_BITS_ISNSET(FiM_GetAvailabilityStatusOfSatellite(satelliteId), FidIndex);
}

/**
 * @brief         set the Availability of a Fid.
 * @param[in]     FID: Identification of a functionality by assigned FID.
 * @param[in]     Availability: The permission of the requested FID: TRUE: Function is available. FALSE: Function is
 *                              not available.
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace       CPD-71160
 */
FIM_LOCAL_INLINE void FiM_SetFidAvailabilityFlag(FiM_FunctionIdType Fid, boolean Availability)
{
    FiM_SatelliteIdType satelliteId = FiM_GetSatelliteIdOfFID(Fid);
    FiM_FunctionIdType  FidIndex    = Fid - FiM_GetFIDStartOfSatellite(satelliteId);
    if (Availability == FALSE)
    {
        FIM_BITS_SET(FiM_GetAvailabilityStatusOfSatellite(satelliteId), FidIndex);
    }
    else
    {
        FIM_BITS_RESET(FiM_GetAvailabilityStatusOfSatellite(satelliteId), FidIndex);
    }
}
#endif

#define FIM_STOP_SEC_CODE
#include "FiM_MemMap.h"

/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
/* ========================================== external function definitions ========================================= */
#define FIM_START_SEC_CODE
#include "FiM_MemMap.h"
/**
 * @brief         This service initializes the FIM.Initialization needs to be called separately for each partition.
 * @param[in]     FiMConfigPtr: Pointer to the configuration.
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace         CPD-69573
 */
void FiM_Init(const FiM_ConfigType* FiMConfigPtr)
{
#if (FIM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = FIM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (NULL_PTR == FiMConfigPtr)
    {
        errorId = FIM_E_PARAM_POINTER;
    }
    else
#endif
    {
        FiM_PBcfgPtr = FiMConfigPtr;
        /** set all Satellite Init Status: if dem trigger set to off, poll set to FIM_INITIALIZED */
        /** permission state  Availability Status use CLEARED, so not need to init again */

#if (FIM_SATELLITE_NUMBER <= 1u)
        ApplicationType applicationId = FIM_DEM_MASTER_APPLICATION;
#else
        ApplicationType applicationId = GetApplicationID();
#endif
        FiM_SatelliteIdType satelliteId = FiM_GetFidCurrentSatelliteId(applicationId);

        if (satelliteId < FIM_SATELLITE_NUMBER) /* PRQA S 2991,2995 */ /* VL_QAC_2991,VL_FiM_2995 */
        {
            /** permission state  Availability Status use CLEARED,The following lines of code can be removed */
            FiM_FunctionIdType fIdStart = FiM_GetFIDStartOfSatellite(satelliteId);
            FiM_FunctionIdType fIdEnd   = FiM_GetFIDEndOfSatellite(satelliteId);
            for (; fIdStart < fIdEnd; ++fIdStart)
            {
#if (STD_ON == FIM_AVAILABILITY_SUPPORT)
                FiM_SetFidAvailabilityFlag(fIdStart, TRUE);
#endif
#if (FIM_EVENT_UPDATE_TRIGGERED_BY_DEM == STD_ON)
                FiM_SetFidCounter(satelliteId, fIdStart, 0u);
#else
#if (FIM_FEATURE_POLLING_MODE_ASY == STD_ON)
                FiM_SetFidPermissionFlag(fIdStart, TRUE);
#endif
#endif
            }

#if (FIM_EVENT_UPDATE_TRIGGERED_BY_DEM == STD_OFF)
#if (FIM_FEATURE_POLLING_MODE_ASY == STD_ON)
            fIdStart = FiM_GetFIDStartOfSatellite(satelliteId);
            for (; fIdStart < fIdEnd; ++fIdStart)
            {
                FiM_HandleFid(fIdStart);
            }
#endif
#endif
#if (FIM_EVENT_UPDATE_TRIGGERED_BY_DEM == STD_ON)
            FiM_SetSatelliteInitStatus(satelliteId, FIM_PREINITIALIZED);
#else
            FiM_SetSatelliteInitStatus(satelliteId, FIM_INITIALIZED);
#endif
        }
#if (FIM_DEV_ERROR_DETECT == STD_ON)
        else
        {
            FIM_DET_REPORT(FIM_SID_INIT, FIM_E_PARTITION_ERROR);
        }
#endif
    }

#if (FIM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != FIM_E_NO_ERROR)
    {
        FIM_DET_REPORT(FIM_SID_INIT, errorId);
    }
#endif
}

#if (FIM_EVENT_UPDATE_TRIGGERED_BY_DEM == STD_ON)
/**
 * @brief         This service re-initializes the FIM Satellite.
 * @param[in]     ApplicationId: Partition (OsApplication identifier) of the satellite.
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace         CPD-69581
 */
/* PRQA S 1505 ++ */ /* VL_FiM_1505 */
void FiM_DemInitSatellite(ApplicationType ApplicationId)
/* PRQA S 1505 -- */
{
    FiM_SatelliteIdType satelliteId = FiM_GetFidCurrentSatelliteId(ApplicationId);
#if (FIM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = FIM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (satelliteId >= FIM_SATELLITE_NUMBER)
    {
        /** In the trigger mode, the FID can only reference and must reference the events of the corresponding
         * partition.It is not allowed to reference events from other partitions. */
        /** There may be no FID in this partition, so no processing will be carried out. */
    }
    else if (FIM_UNINITIALIZED == FiM_GetSatelliteInitStatus(satelliteId))
    {
        errorId = FIM_E_UNINIT;
    }
    else
#endif
    {
#if (FIM_EVENT_INHBITION == STD_ON)
        FiM_EventSatelliteIdType eventSatelliteNum = FiM_PBcfgPtr->EventSatelliteNum;
        FiM_EventSatelliteIdType eventSatelliteId  = FiM_GetEventCurrentSatelliteId(ApplicationId);
        if (eventSatelliteNum > eventSatelliteId)
        {
            FiM_EventSatelliteTableType eventSatelliteTable = FiM_PBcfgPtr->EventSatelliteTable[eventSatelliteId];
            FiM_EventNumType            eventSatStart       = eventSatelliteTable.EventSatelliteStart;
            FiM_EventNumType            eventSatEnd         = eventSatelliteTable.EventSatelliteEnd;
            const FiM_EventNumType*     eventSatRefTable    = FiM_PBcfgPtr->EventSatelliteRefTable;
            for (; eventSatStart < eventSatEnd; ++eventSatStart)
            {
                FiM_EventNumType eventTableRef = eventSatRefTable[eventSatStart];
                FiM_CalculateByEvent(eventTableRef);
            }
        }
#endif
#if (FIM_COMPONENT_INHBITION == STD_ON)
        if (FIM_DEM_MASTER_APPLICATION == ApplicationId)
        {
            FiM_ComponentNumType componentRefNum = FiM_PBcfgPtr->ComponentRefNum;
            for (FiM_ComponentNumType componentRef = 0u; componentRef < componentRefNum; ++componentRef)
            {
                FiM_CalculateByComponent(componentRef);
            }
        }
#endif
        if (satelliteId < FIM_SATELLITE_NUMBER)
        {
            FiM_SetSatelliteInitStatus(satelliteId, FIM_INITIALIZED);
        }
    }

#if (FIM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != FIM_E_NO_ERROR)
    {
        FIM_DET_REPORT(FIM_SID_DEMINIT, errorId);
    }
#endif
}

/**
 * @brief         This service re-initializes the FIM.
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace         CPD-69578
 */
void FiM_DemInit(void)
{
#if (FIM_SATELLITE_NUMBER <= 1u)
    ApplicationType currentApplication = FIM_DEM_MASTER_APPLICATION;
#else
    ApplicationType currentApplication = GetApplicationID();
#endif
    FiM_DemInitSatellite(currentApplication);
}

/**
 * @brief         This service is provided to be called by the Dem in order to inform the Fim about monitor status
 *                changes.
 * @param[in]     EventId: Identification of an Event by assigned event number. The Event Number is configured in the
 *                         DEM. Min.: 1 (0: Indication of no Event or Failure) Max.: Result of configuration of Event
 *                         Numbers in DEM (Max is either 255 or 65535)
 * @reentrant     Reentrant
 * @synchronous   Synchronous
 * @trace         CPD-69576
 */
void FiM_DemTriggerOnMonitorStatus(Dem_EventIdType EventId)
{
#if (FIM_SATELLITE_NUMBER <= 1u)
    ApplicationType applicationId = FIM_DEM_MASTER_APPLICATION;
#else
    ApplicationType applicationId = GetApplicationID();
#endif
#if (FIM_DEV_ERROR_DETECT == STD_ON)
    uint8               errorId     = FIM_E_NO_ERROR;
    FiM_SatelliteIdType satelliteId = FiM_GetFidCurrentSatelliteId(applicationId);
    /** ***** Development Error Checks ***** */
    if (satelliteId >= FIM_SATELLITE_NUMBER)
    {
        errorId = FIM_E_PARTITION_ERROR;
    }
    else if (FIM_INITIALIZED != FiM_GetSatelliteInitStatus(satelliteId))
    {
        errorId = FIM_E_UNINIT;
    }
    else if ((FIM_DEM_EVENT_NUMBER < EventId) || (FIM_EVENT_ID_INVALID == EventId))
    {
        errorId = FIM_E_EVENTID_OUT_OF_RANGE;
    }
    else
#endif
    {
#if (FIM_EVENT_INHBITION == STD_ON)
        FiM_EventNumType eventTableRef;
        if (TRUE == FiM_SearchEventTable(EventId, &eventTableRef))
        {
            FiM_CalculateByEvent(eventTableRef);
        }
#endif
    }

#if (FIM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != FIM_E_NO_ERROR)
    {
        FIM_DET_REPORT(FIM_SID_DEMTRIGGERONMONITORSTATUS, errorId);
    }
#endif
}

/**
 * @brief         Triggers on changes of the component failed status.
 * @param[in]     ComponentId: Identification of a DemComponent.
 * @param[in]     ComponentFailedStatus: New FAILED status of the component.
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace         CPD-69577
 */
void FiM_DemTriggerOnComponentStatus(Dem_ComponentIdType ComponentId, boolean ComponentFailedStatus)
{
#if (FIM_SATELLITE_NUMBER <= 1u)
    ApplicationType applicationId = FIM_DEM_MASTER_APPLICATION;
#else
    ApplicationType applicationId = GetApplicationID();
#endif
#if (FIM_DEV_ERROR_DETECT == STD_ON)
    uint8               errorId     = FIM_E_NO_ERROR;
    FiM_SatelliteIdType satelliteId = FiM_GetFidCurrentSatelliteId(applicationId);

    /** ***** Development Error Checks ***** */
    if (satelliteId >= FIM_SATELLITE_NUMBER)
    {
        errorId = FIM_E_PARTITION_ERROR;
    }
    else if (FIM_INITIALIZED != FiM_GetSatelliteInitStatus(satelliteId))
    {
        errorId = FIM_E_UNINIT;
    }
    else if ((FIM_DEM_COMPONENT_NUMBER <= ComponentId))
    {
        errorId = FIM_E_COMPONENT_OUT_OF_RANGE;
    }
    else
#endif
    {
#if (FIM_COMPONENT_INHBITION == STD_ON)
        if (FIM_DEM_MASTER_APPLICATION == applicationId)
        {
            FiM_ComponentNumType componentRef;
            if (TRUE == FiM_SearchComponentTable(ComponentId, &componentRef))
            {
                FiM_CalculateByComponent(componentRef);
            }
        }
#endif
        FIM_PARAM_UNUSED(ComponentFailedStatus);
    }

#if (FIM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != FIM_E_NO_ERROR)
    {
        FIM_DET_REPORT(FIM_SID_DEMTRIGGERONCOMPONENTSTATUS, errorId);
    }
#endif
}
#endif

/**
 * @brief         This service reports the permission state to the functionality.
 * @param[in]     FID: Identification of a functionality by assigned FID. The FunctionId is configured in the FIM.
 *                     Min.: 1 (0: Indication of no functionality)
 *                     Max.: Result of configuration of FIDs in FIM (Max is either 255 or 65535)
 * @param[out]    Permission: TRUE: FID has permission to run
 *                            FALSE: FID has no permission to run, i.e. shall not be executed
 * @return        Std_ReturnType
 * @retval        E_OK: The request is accepted
 * @retval        E_NOT_OK: The request is not accepted, ie. initialization of FIM not completed
 * @reentrant     Reentrant
 * @synchronous   Synchronous
 * @trace         CPD-69574
 */
/* PRQA S 6030 ++ */ /* VL_MTR_FiM_STMIF */
Std_ReturnType FiM_GetFunctionPermission(FiM_FunctionIdType FID, boolean* Permission)
{
    Std_ReturnType ret;
#if (FIM_SATELLITE_NUMBER <= 1u)
    ApplicationType applicationId = FIM_DEM_MASTER_APPLICATION;
#else
    ApplicationType applicationId = GetApplicationID();
#endif
#if (FIM_DEV_ERROR_DETECT == STD_ON)
    uint8               errorId     = FIM_E_NO_ERROR;
    FiM_SatelliteIdType satelliteId = FiM_GetFidCurrentSatelliteId(applicationId);
    ret                             = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    if (NULL_PTR == Permission)
    {
        errorId = FIM_E_PARAM_POINTER;
    }
    else if ((FIM_FID_NUM < FID) || (FIM_FUNCTION_ID_INVALID == FID))
    {
        errorId     = FIM_E_FID_OUT_OF_RANGE;
        *Permission = FALSE;
    }
    /* PRQA S 2992,2996 ++ */ /* VL_FiM_2992,VL_FiM_2996 */
    else if ((satelliteId >= FIM_SATELLITE_NUMBER) || (satelliteId != FiM_GetSatelliteIdOfFID(FID)))
    /* PRQA S 2992,2996 -- */
    {
        errorId = FIM_E_PARTITION_ERROR;
    }
    else if (FIM_INITIALIZED != FiM_GetSatelliteInitStatus(satelliteId)) /* PRQA S 1252 */ /* VL_QAC_1252 */
    {
        errorId     = FIM_E_UNINIT;
        *Permission = FALSE;
    }
    else
#endif
#if (STD_ON == FIM_AVAILABILITY_SUPPORT)
        if (FiM_GetFidAvailabilityFlag(FID) == FALSE)
    {
        *Permission = FALSE;
        ret         = E_OK;
    }
    else
#endif
    {
#if (FIM_EVENT_UPDATE_TRIGGERED_BY_DEM == STD_ON)
        *Permission = FiM_GetFidPermission(FID);
#else
#if (FIM_FEATURE_POLLING_MODE_ASY == STD_ON)
        *Permission = FiM_GetFidPermissionFlag(FID);
#else
        *Permission = FiM_CalculateFIDPermission(FID);
#endif
#endif
        ret = E_OK;
    }

#if (FIM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != FIM_E_NO_ERROR)
    {
        FIM_DET_REPORT(FIM_SID_GETFUNCTIONPERMISSION, errorId);
    }
#endif
    return ret;
}
/* PRQA S 6030 -- */

#if (STD_ON == FIM_AVAILABILITY_SUPPORT)
/**
 * @brief         This service sets the availability of a function. The function is only available if FiMAvailability
 *                Support is configured as True.
 * @param[in]     FID: Identification of a functionality by assigned FID.
 * @param[in]     Availability: The permission of the requested FID. TRUE: Function is available. FALSE: Function is
 *                              not available.
 * @return        Std_ReturnType
 * @retval        E_OK: The request is accepted
 * @retval        E_NOT_OK: Request is not accepted (e.g. invalid FID is given)
 * @reentrant     Reentrant
 * @synchronous   Synchronous
 * @trace         CPD-69575
 */
Std_ReturnType FiM_SetFunctionAvailable(FiM_FunctionIdType FID, boolean Availability)
{
    Std_ReturnType ret;
#if (FIM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = FIM_E_NO_ERROR;
#if (FIM_SATELLITE_NUMBER <= 1u)
    ApplicationType applicationId = FIM_DEM_MASTER_APPLICATION;
#else
    ApplicationType applicationId = GetApplicationID();
#endif
    FiM_SatelliteIdType satelliteId = FiM_GetFidCurrentSatelliteId(applicationId);
    ret                             = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    /* PRQA S 2992,2996 ++ */ /* VL_FiM_2992,VL_FiM_2996 */
    if ((satelliteId >= FIM_SATELLITE_NUMBER) || (satelliteId != FiM_GetSatelliteIdOfFID(FID)))
    /* PRQA S 2992,2996 -- */
    {
        errorId = FIM_E_PARTITION_ERROR;
    }
    else if ((FIM_FID_NUM < FID) || (FIM_FUNCTION_ID_INVALID == FID))
    {
        errorId = FIM_E_FID_OUT_OF_RANGE;
    }
    else if (FIM_INITIALIZED != FiM_GetSatelliteInitStatus(satelliteId)) /* PRQA S 1252 */ /* VL_QAC_1252 */
    {
        errorId = FIM_E_UNINIT;
    }
    else
#endif
    {
        FiM_SetFidAvailabilityFlag(FID, Availability);
        ret = E_OK;
    }

#if (FIM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != FIM_E_NO_ERROR)
    {
        FIM_DET_REPORT(FIM_SID_SETFUNCTIONAVAILABLE, errorId);
    }
#endif
    return ret;
}
#endif /* STD_ON == FIM_AVAILABILITY_SUPPORT */

#if (FIM_VERSION_INFO_API == STD_ON)
/**
 * @brief         This service returns the version information of this module.
 * @param[out]    versioninfo: Pointer to where to store the version information of this module.
 * @reentrant     Reentrant
 * @synchronous   Synchronous
 * @trace         CPD-69579
 */
void FiM_GetVersionInfo(Std_VersionInfoType* versionInfo) /* PRQA S 1330 */ /* VL_FiM_1330 */
{
#if (FIM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Checks ***** */
    if (NULL_PTR == versionInfo)
    {
        FIM_DET_REPORT(FIM_SID_GETVERSIONINFO, FIM_E_PARAM_POINTER);
    }
    else
#endif
    {
        versionInfo->vendorID         = FIM_VENDOR_ID;
        versionInfo->moduleID         = FIM_MODULE_ID;
        versionInfo->sw_major_version = FIM_SW_MAJOR_VERSION;
        versionInfo->sw_minor_version = FIM_SW_MINOR_VERSION;
        versionInfo->sw_patch_version = FIM_SW_PATCH_VERSION;
    }
}
#endif

#if (STD_OFF == FIM_EVENT_UPDATE_TRIGGERED_BY_DEM)
/**
 * @brief         This function is used to evaluate permission states cyclically.
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace         CPD-69580
 */
void FiM_MainFunction(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
#if (FIM_FEATURE_POLLING_MODE_ASY == STD_ON)
#if (FIM_SATELLITE_NUMBER <= 1u)
    ApplicationType applicationId = FIM_DEM_MASTER_APPLICATION;
#else
    ApplicationType applicationId = GetApplicationID();
#endif
    FiM_SatelliteIdType satelliteId = FiM_GetFidCurrentSatelliteId(applicationId);
    if (satelliteId >= FIM_SATELLITE_NUMBER) /* PRQA S 2992,2996 */ /* VL_FiM_2992,VL_FiM_2996 */
    {
#if (FIM_DEV_ERROR_DETECT == STD_ON)
        FIM_DET_REPORT(FIM_SID_MAINFUNCTION, FIM_E_PARTITION_ERROR);
#endif
    }
    else
    {
        if (FIM_INITIALIZED == FiM_GetSatelliteInitStatus(satelliteId)) /* PRQA S 1252 */ /* VL_QAC_1252 */
        {
            FiM_FunctionIdType fIdStart = FiM_GetFIDStartOfSatellite(satelliteId);
            FiM_FunctionIdType fIdEnd   = FiM_GetFIDEndOfSatellite(satelliteId);
            for (; fIdStart < fIdEnd; ++fIdStart)
            {
                FiM_HandleFid(fIdStart);
            }
        }
    }
#endif
}
#endif
/* PRQA S 1503 -- */

#define FIM_STOP_SEC_CODE
#include "FiM_MemMap.h"
