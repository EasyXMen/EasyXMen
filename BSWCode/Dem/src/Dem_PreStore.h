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
 **  @file               : Dem_PreStore.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : Implementation file of Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_PRESTORE_H
#define DEM_PRESTORE_H

/* =================================================== inclusions =================================================== */
#include "Dem_Lcfg.h"
#if (DEM_MAX_NUMBER_PRESTORED_FF > 0u)
#include "Dem_SatelliteInterface.h"
#include "Dem_MasterInterface.h"
#include "Dem_Obd.h"
#include "Dem_FreezeFrame.h"
#include "Dem_EventInterface.h"
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
#include "Os.h"
#endif

/* ===================================================== macros ===================================================== */
/**
 * @name  DEM_PRESTORE_REQUEST_ENCODE
 * @{
 * @brief Handle for prestorage actions set on satellite, read on master
 */
#define DEM_PRESTORE_REQUEST_INITIAL    (0x0u) /**< Prestored request is initial */
#define DEM_PRESTORE_REQUEST_PRESTOREFF (0x1u) /**< Prestored request is prestore FF */
#define DEM_PRESTORE_REQUEST_CLEAR      (0x2u) /**< Prestored request is clear */
/** @} */

/**
 * @name  DEM_PRESTORE_STATUS_ENCODE
 * @{
 * @brief Macros encoding prestored status states
 */
#define DEM_PRESTORE_STATUS_FREE      (0x00u) /**< Prestored entry is free */
#define DEM_PRESTORE_STATUS_READY     (0x01u) /**< Prestored entry contains valid data */
#define DEM_PRESTORE_STATUS_UPDATING  (0x02u) /**< Prestored entry needs to update data */
#define DEM_PRESTORE_STATUS_LOCKED    (0x03U) /**< Prestored entry is locked */
#define DEM_PRESTORE_STATUS_DISCARDED (0x04u) /**< Prestored entry needs to be freed */
/** @} */

/* ========================================== internal function declarations ======================================== */
/**
 * @brief         Attempt to allocate an unused pre-stored freeze frame space
 * @param[in]     EventId: Event Id
 * @return        Dem_PrestorageIndexType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Dem_PrestorageIndexType Dem_AllocatePrestorageEntry(Dem_EventIdType EventId);

/**
 * @brief         Get freeze frame data into pre-stored freeze frame space
 * @param[in]     EventId: Event Id
 * @param[in]     PrestorEntryIndex: Prestor entry index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_PrestorageEntryUpdate(Dem_EventIdType EventId, Dem_PrestorageIndexType PrestorEntryIndex);

/**
 * @brief         Execute event pre-storage requests to update data
 * @param[in]     EventId: Event Id
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_PrestorageUpdate(Dem_EventIdType EventId);

/**
 * @brief         Execute event pre-storage requests to clear data
 * @param[in]     EventId: Event Id
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_PrestorageClear(Dem_EventIdType EventId);

/**
 * @brief         Locks pre-stored freeze frames to prevent them from being deleted on request during processing
 * @param[in]     EventId: Event Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_EventPrestorageLock(Dem_EventIdType EventId);

/**
 * @brief         Release locked pre-stored entries
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_EventPrestorageRelease(void);

#if ((DEM_MAX_NUMBER_PRESTORED_FF > 0u) && (DEM_EVENT_PARTITION_NUMBER > 1u))
/**
 * @brief         update current monitor prestore FF flag
 * @param[in]     EventId: Event Id
 * @param[in]     NewPrestoreFlag: Flag of new prestor
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_UpdatePrestoreStatus(Dem_EventIdType EventId, uint32 NewPrestoreFlag);

/**
 * @brief         prestore process
 * @param[in]     SatelliteId: satellite Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_PrestoreProcess(Dem_SatelliteIdType SatelliteId);

/**
 * @brief         Prestorage Task process
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_PrestorageTask(void);
#endif

/**
 * @brief         Processing event pre-storage requests
 * @param[in]     EventId: Event Id
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_EventPrestoreFF(Dem_EventIdType EventId);

/**
 * @brief         clear Processed event pre-storage Freeze frame
 * @param[in]     EventId: Event Id
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_EventClearPrestoreFF(Dem_EventIdType EventId);

#if (DEM_FREEZE_FRAME_CLASS_NUMBER > 0u)
/**
 * @brief         Copy a prestored snapshot for an event
 * @param[in]     DestinationBuffer: Pointer to destination buffer
 * @param[in]     BufferSize: Buffer size
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_PrestoreApplyFreezeFrame(uint8* DestinationBuffer, uint16 BufferSize);
#endif

#if (DEM_MAX_SIZE_J1939FREEZEFRAME > 0u)
/**
 * @brief         Copy a prestored J1939 snapshot for an event
 * @param[in]     DestinationBuffer: Pointer to destination buffer
 * @param[in]     BufferSize: Buffer size
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_PrestoreApplyJ1939FreezeFrame(uint8* DestinationBuffer, uint16 BufferSize);
#endif

#if (DEM_MAX_SIZE_J1939EXFREEZEFRAME > 0u)
/**
 * @brief         Copy a prestored J1939Ex snapshot for an event
 * @param[in]     DestinationBuffer: Pointer to destination buffer
 * @param[in]     BufferSize: Buffer size
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_PrestoreApplyJ1939ExFreezeFrame(uint8* DestinationBuffer, uint16 BufferSize);
#endif

#if ((DEM_OBDII_SUPPORT == STD_ON) && (DEM_MAX_SIZE_OBDFREEZEFRAME > 0u))
/**
 * @brief         Copy a prestored OBD snapshot for an event
 * @param[in]     DestinationBuffer: Pointer to destination buffer
 * @param[in]     BufferSize: Buffer size
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_PrestoreApplyOBDFreezeFrame(uint8* DestinationBuffer, uint16 BufferSize);
#endif

/* ============================================ internal data definitions =========================================== */
#define DEM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dem_MemMap.h"
/**
 * @brief        Pre store Free List Head
 * @range 0..255
 */
DEM_LOCAL Dem_PrestorageIndexType Dem_PrestoreFreeListHead;

/**
 * @brief        Pre store Locked List Head
 * @range 0..255
 */
DEM_LOCAL Dem_PrestorageIndexType Dem_PrestoreLockedList;
#define DEM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dem_MemMap.h"

/* ========================================== internal function definitions ========================================= */
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         get the maximum number for prestored freeze frames
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_PreFFSize(void)
{
    return DEM_MAX_NUMBER_PRESTORED_FF;
}

/**
 * @brief         get the number for prestored freeze frames event
 * @return        Dem_EventIdType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventIdType Dem_PreFFEventSize(void)
{
    return DEM_PRESTORED_EVENT_NUM;
}

/* PRQA S 5016 ++ */ /* VL_Dem_5016 */
#if (DEM_MAX_SIZE_FREEZEFRAME > 0u)
/**
 * @brief         Get Prestorage Entry Freeze frame buffer pointer
 * @param[in]     PrestorageIndex: Prestorage entry index
 * @return        uint8*
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8* Dem_GetPrestorageFFPtr(Dem_PrestorageIndexType PrestorageIndex)
{
    return Dem_GetPrestorage()[PrestorageIndex].FreezeFrame;
}
#endif

#if (DEM_MAX_SIZE_OBDFREEZEFRAME > 0u)
/**
 * @brief         Get Prestorage Entry Freeze frame buffer pointer
 * @param[in]     PrestorageIndex: Prestorage entry index
 * @return        uint8*
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8* Dem_GetPrestorageOBDFFPtr(Dem_PrestorageIndexType PrestorageIndex)
{
    return Dem_GetPrestorage()[PrestorageIndex].OBDFreezeFrame;
}
#endif

#if (DEM_MAX_SIZE_J1939FREEZEFRAME > 0u)
/**
 * @brief         Get Prestorage Entry Freeze frame buffer pointer
 * @param[in]     PrestorageIndex: Prestorage entry index
 * @return        uint8*
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8* Dem_GetPrestorageJ1939FFPtr(Dem_PrestorageIndexType PrestorageIndex)
{
    return Dem_GetPrestorage()[PrestorageIndex].J1939FreezeFrame;
}
#endif

#if (DEM_MAX_SIZE_J1939EXFREEZEFRAME > 0u)
/**
 * @brief         Get Prestorage Entry Freeze frame buffer pointer
 * @param[in]     PrestorageIndex: Prestorage entry index
 * @return        uint8*
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8* Dem_GetPrestorage1939ExFFPtr(Dem_PrestorageIndexType PrestorageIndex)
{
    return Dem_GetPrestorage()[PrestorageIndex].J1939ExFreezeFrame;
}
#endif
/* PRQA S 5016 -- */

/**
 * @brief         Set Prestorage Entry EventId
 * @param[in]     PrestorageIndex: Prestorage entry index
 * @param[in]     EventId: Event Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetPrestorageEntryEventId(Dem_PrestorageIndexType PrestorageIndex, Dem_EventIdType EventId)
{
    Dem_GetPrestorage()[PrestorageIndex].EventId = EventId;
}

/**
 * @brief         Set Prestorage Entry State
 * @param[in]     PrestorageIndex: Prestorage entry index
 * @param[in]     State: Prestorage state
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetPrestorageEntryState(Dem_PrestorageIndexType PrestorageIndex, uint8 State)
{
    Dem_GetPrestorage()[PrestorageIndex].State = State;
}

/**
 * @brief         Get Prestorage Entry State
 * @param[in]     PrestorageIndex: Prestorage entry index
 * @param[in]     State: Prestorage state
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetPrestorageEntryState(Dem_PrestorageIndexType PrestorageIndex)
{
    return Dem_GetPrestorage()[PrestorageIndex].State;
}

/**
 * @brief         Set pre-stored freeze frame space to point to the next unused space
 * @param[in]     PrestorageIndex: Prestorage entry index
 * @param[in]     NextPrestorageIndex: Next prestorage entry index
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void
    Dem_FreePrestorageListSetNext(Dem_PrestorageIndexType PrestorageIndex, Dem_PrestorageIndexType NextPrestorageIndex)
{
    Dem_GetPrestorage()[PrestorageIndex].EventId = (Dem_EventIdType)NextPrestorageIndex;
}

/**
 * @brief         Get an index to the next unused pre-stored freeze frame space
 * @param[in]     PrestorageIndex: Prestorage entry index
 * @return        Dem_PrestorageIndexType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_PrestorageIndexType Dem_GetNextPrestoreFreeList(Dem_PrestorageIndexType PrestorageIndex)
{
    return (Dem_PrestorageIndexType)Dem_GetPrestorage()[PrestorageIndex].EventId;
}

/**
 * @brief         Get EventId by the pre-stored freeze frame space
 * @param[in]     PrestorageIndex: Prestorage entry index
 * @return        Dem_EventIdType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventIdType Dem_PrestorageEntryGetEventId(Dem_PrestorageIndexType PrestorageIndex)
{
    return Dem_GetPrestorage()[PrestorageIndex].EventId;
}

/**
 * @brief         Set Prestorage Index of event
 * @param[in]     PreFFEventIndex: Prestorage FF event index
 * @param[in]     PrestorageIndex: Prestorage entry index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetPrestorageIndex(Dem_EventIdType PreFFEventIndex, uint8 PrestorageIndex)
{
    Dem_GetCfgPrestorageIndex()[PreFFEventIndex] = PrestorageIndex;
}

/**
 * @brief         Get Prestorage Index of event
 * @param[in]     PreFFEventIndex: Prestorage FF event index
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetPrestorageIndex(Dem_EventIdType PreFFEventIndex)
{
    return Dem_GetCfgPrestorageIndex()[PreFFEventIndex];
}

/**
 * @brief         prestored freeze frames initialization
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_PrestoreInit(void)
{
    /** initialize the temporary element too, so Dem_PreFFSize() is the last valid index */
    for (Dem_PrestorageIndexType prestorageIndex = 0u; prestorageIndex < Dem_PreFFSize(); ++prestorageIndex)
    {
        /** Chain the blocks */
        Dem_SetPrestorageEntryState(prestorageIndex, DEM_PRESTORE_STATUS_FREE);
        Dem_FreePrestorageListSetNext(prestorageIndex, (Dem_PrestorageIndexType)(prestorageIndex + 1u));
    }
    /** Last block points to end, head points to first block */
    Dem_FreePrestorageListSetNext(Dem_PreFFSize(), Dem_PreFFSize());
    Dem_PrestoreFreeListHead = 0u;
    /** no event has currantly allocated a buffer */
    for (Dem_EventIdType preFFEventIndex = 0u; preFFEventIndex < Dem_PreFFEventSize(); ++preFFEventIndex)
    {
        Dem_SetPrestorageIndex(preFFEventIndex, Dem_PreFFSize());
    }
    /** Initialize the prestorage lock state */
    Dem_PrestoreLockedList = Dem_PreFFSize();
#if ((DEM_MAX_NUMBER_PRESTORED_FF > 0u) && (DEM_EVENT_PARTITION_NUMBER > 1u))
    /** Set prestorage tasks on scheduler to enabled*/
    Dem_EnableTask(Dem_Task_Prestorage);
#endif
}

/**
 * @brief Attempt to allocate an unused pre-stored freeze frame space
 */
DEM_LOCAL Dem_PrestorageIndexType Dem_AllocatePrestorageEntry(Dem_EventIdType EventId)
{
    Dem_PrestorageIndexType firstUnusedIndex = Dem_PrestoreFreeListHead;
    if (firstUnusedIndex != Dem_PreFFSize())
    {
        /** shorten the free list by removing its first element */
        Dem_PrestoreFreeListHead = Dem_GetNextPrestoreFreeList(firstUnusedIndex);
        /** connect the event with the buffer slot */
        Dem_SetPrestorageIndex(Dem_GetPreFFIndexOfEvent(EventId), firstUnusedIndex);
        /** fill the buffer slot with the EventId */
        Dem_SetPrestorageEntryEventId(firstUnusedIndex, EventId);
    }
    return firstUnusedIndex;
}

/**
 * @brief         Attempt to free an used pre-stored freeze frame space
 * @param[in]     PrestorEntryIndex: Prestor entry index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_FreePrestorageEntry(Dem_PrestorageIndexType PrestorEntryIndex)
{
    /** un-connect the event from the buffer slot */
    Dem_EventIdType eventId = Dem_GetNextPrestoreFreeList(PrestorEntryIndex);
    Dem_SetPrestorageIndex(Dem_GetPreFFIndexOfEvent(eventId), Dem_PreFFSize());
    /** enqueue the buffer slot as first element in the free list */
    Dem_FreePrestorageListSetNext(PrestorEntryIndex, Dem_PrestoreFreeListHead);
    Dem_PrestoreFreeListHead = PrestorEntryIndex;
}

/**
 * @brief Get freeze frame data into pre-stored freeze frame space
 */
DEM_LOCAL void Dem_PrestorageEntryUpdate(Dem_EventIdType EventId, Dem_PrestorageIndexType PrestorEntryIndex)
{
#if (DEM_MAX_SIZE_FREEZEFRAME > 0u)
    /** Sample a UDS snapshot into the prestorage buffer */
    Dem_GetFFFromEnv(EventId, Dem_GetPrestorageFFPtr(PrestorEntryIndex), DEM_MAX_SIZE_FREEZEFRAME);
#endif

#if ((DEM_OBDII_SUPPORT == STD_ON) && (DEM_MAX_SIZE_OBDFREEZEFRAME > 0u))
    if (Dem_CheckEventObdRelated(EventId) == TRUE)
    {
        /** If the event is OBD relevant, sample an OBD freeze frame into the prestorage buffer */
        Dem_GetOBDFFFromEnv(EventId, Dem_GetPrestorageOBDFFPtr(PrestorEntryIndex), DEM_MAX_SIZE_OBDFREEZEFRAME);
    }
#endif

#if ((DEM_J1939_SUPPORT == STD_ON) && ((DEM_MAX_SIZE_J1939FREEZEFRAME > 0u) || (DEM_MAX_SIZE_J1939EXFREEZEFRAME > 0u)))
    /** If the event has a J1939 DTC, sample the SPNs into the prestorage buffer */
    if (Dem_CheckEventJ1939Related(EventId) == TRUE)
    {
#if (DEM_MAX_SIZE_J1939FREEZEFRAME > 0u)
        Dem_GetJ1939FFFromEnv(EventId, Dem_GetPrestorageJ1939FFPtr(PrestorEntryIndex), DEM_MAX_SIZE_J1939FREEZEFRAME);
#endif
#if (DEM_MAX_SIZE_J1939EXFREEZEFRAME > 0u)
        Dem_GetJ1939ExFFFromEnv(
            EventId,
            Dem_GetPrestorage1939ExFFPtr(PrestorEntryIndex),
            DEM_MAX_SIZE_J1939EXFREEZEFRAME);
#endif
    }
#endif
}

/**
 * @brief Execute event pre-storage requests to update data
 */
DEM_LOCAL Std_ReturnType Dem_PrestorageUpdate(Dem_EventIdType EventId)
{
    Std_ReturnType          ret = E_NOT_OK;
    Dem_PrestorageIndexType prestorEntryIndex;

    /** Enter Critical Section: DiagMonitor */
    Dem_EnterCritical_DiagMonitor();
    prestorEntryIndex = Dem_GetPrestorageIndex(Dem_GetPreFFIndexOfEvent(EventId));
    if (prestorEntryIndex < Dem_PreFFSize())
    {
        /** there already is an entry allocated to this event */
        Dem_SetPrestorageEntryState(prestorEntryIndex, DEM_PRESTORE_STATUS_UPDATING);
    }
    else
    {
        /** try to find a free entry for this event */
        prestorEntryIndex = Dem_AllocatePrestorageEntry(EventId);
        if (prestorEntryIndex != Dem_PreFFSize())
        {
            Dem_SetPrestorageEntryState(prestorEntryIndex, DEM_PRESTORE_STATUS_UPDATING);
        }
    }
    Dem_LeaveCritical_DiagMonitor();
    /** Leave Critical Section: DiagMonitor */

    if (prestorEntryIndex < Dem_PreFFSize())
    {
        Dem_PrestorageEntryUpdate(EventId, prestorEntryIndex);
        Dem_SetPrestorageEntryState(prestorEntryIndex, DEM_PRESTORE_STATUS_READY);
        ret = E_OK;
    }
    return ret;
}

/**
 * @brief Execute event pre-storage requests to clear data
 */
DEM_LOCAL Std_ReturnType Dem_PrestorageClear(Dem_EventIdType EventId)
{
    Std_ReturnType          ret               = E_NOT_OK;
    Dem_PrestorageIndexType prestorEntryIndex = Dem_GetPrestorageIndex(Dem_GetPreFFIndexOfEvent(EventId));

    if (prestorEntryIndex < Dem_PreFFSize())
    {
        /** there already is an entry allocated to this event */
        /** Enter Critical Section: DiagMonitor */
        Dem_EnterCritical_DiagMonitor();
        if (Dem_GetPrestorageEntryState(prestorEntryIndex) != DEM_PRESTORE_STATUS_LOCKED)
        {
            Dem_SetPrestorageEntryState(prestorEntryIndex, DEM_PRESTORE_STATUS_FREE);
            Dem_FreePrestorageEntry(prestorEntryIndex);
            ret = E_OK;
        }
        Dem_LeaveCritical_DiagMonitor();
        /** Leave Critical Section: DiagMonitor */
    }
    return ret;
}

/**
 * @brief Locks pre-stored freeze frames to prevent them from being deleted on request during processing
 */
DEM_LOCAL void Dem_EventPrestorageLock(Dem_EventIdType EventId)
{
    if (Dem_GetPreFFIndexOfEvent(EventId) != DEM_PRESTORED_EVENT_NUM)
    {
        Dem_PrestorageIndexType prestorEntryIndex = Dem_GetPrestorageIndex(Dem_GetPreFFIndexOfEvent(EventId));
        if (prestorEntryIndex < Dem_PreFFSize())
        {
            /** Enter Critical Section: DiagMonitor */
            Dem_EnterCritical_DiagMonitor();
            if (Dem_GetPrestorageEntryState(prestorEntryIndex) == DEM_PRESTORE_STATUS_READY)
            {
                Dem_SetPrestorageEntryState(prestorEntryIndex, DEM_PRESTORE_STATUS_LOCKED);
                Dem_SetPrestorageIndex(Dem_GetPreFFIndexOfEvent(EventId), Dem_PreFFSize());
                Dem_PrestoreLockedList = prestorEntryIndex;
            }
            Dem_LeaveCritical_DiagMonitor();
            /** Leave Critical Section: DiagMonitor */
        }
    }
}

/**
 * @brief         Prestored data (if available) is valid throughout the whole event processing step. This function marks
 * the current data to be discarded after event processing completes.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetPrestoreStateDiscard(void)
{
    if (Dem_PrestoreLockedList != Dem_PreFFSize())
    {
        Dem_SetPrestorageEntryState(Dem_PrestoreLockedList, DEM_PRESTORE_STATUS_DISCARDED);
    }
}

/**
 * @brief Release locked pre-stored entries
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
DEM_LOCAL void Dem_EventPrestorageRelease(void)
/* PRQA S 6070 -- */
{
    if (Dem_PrestoreLockedList != Dem_PreFFSize())
    {
        Dem_PrestorageIndexType prestorageEntry = Dem_PrestoreLockedList;
        Dem_EventIdType         eventId         = Dem_PrestorageEntryGetEventId(prestorageEntry);

        /** Enter Critical Section: DiagMonitor */
        Dem_EnterCritical_DiagMonitor();
        if (Dem_GetPrestorageEntryState(prestorageEntry) == DEM_PRESTORE_STATUS_LOCKED)
        {
            /** Entry unused, return it to the event */
            Dem_PrestorageIndexType prestorEntryIndex = Dem_GetPrestorageIndex(Dem_GetPreFFIndexOfEvent(eventId));
            if (prestorEntryIndex < Dem_PreFFSize())
            {
                /** Event has created a new prestored slot since locking the old, so free the locked entry */
                Dem_SetPrestorageEntryState(prestorageEntry, DEM_PRESTORE_STATUS_FREE);
                Dem_FreePrestorageEntry(prestorageEntry);
                /** FreeEntry removes the prestored block from the event, repair the link */
                Dem_SetPrestorageIndex(Dem_GetPreFFIndexOfEvent(eventId), prestorEntryIndex);
            }
            else
            {
                /** Return the unused prestorage entry to the event*/
                Dem_SetPrestorageEntryState(prestorageEntry, DEM_PRESTORE_STATUS_READY);
                Dem_SetPrestorageIndex(Dem_GetPreFFIndexOfEvent(eventId), prestorageEntry);
            }
        }
        else
        {
            /** Prestorage entry has been discarded */
            Dem_SetPrestorageEntryState(prestorageEntry, DEM_PRESTORE_STATUS_FREE);
            Dem_FreePrestorageEntry(prestorageEntry);
        }
        Dem_LeaveCritical_DiagMonitor();
        /** Leave Critical Section: DiagMonitor */

        Dem_PrestoreLockedList = Dem_PreFFSize();
    }
}

#if ((DEM_MAX_NUMBER_PRESTORED_FF > 0u) && (DEM_EVENT_PARTITION_NUMBER > 1u))
/* PRQA S 1258 ++ */ /* VL_Dem_1258 */
/* PRQA S 0759 ++ */ /* VL_Dem_0759 */
/**
 * @brief         get current monitor prestore flag
 * @param[in]     SatelliteExtendedData: Satellite extended data
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetPrestoreStatus(Dem_SatelliteExtendedDataType SatelliteExtendedData)
{
    uint32 sharedData = SatelliteExtendedData.Data.SharedData;
    return (uint8)(sharedData & (uint32)DEM_PRESTOREFF_STATUS);
}

/* PRQA S 3473 ++ */ /* VL_Dem_3473 */
/**
 * @brief         set current monitor prestore FF flag
 * @param[in]     SatelliteExtendedData: Satellite extended data
 * @return        Dem_SatelliteExtendedDataType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_SatelliteExtendedDataType Dem_SetPrestoreFF(Dem_SatelliteExtendedDataType SatelliteExtendedData)
{
    Dem_SatelliteExtendedDataType ret        = SatelliteExtendedData;
    uint32                        sharedData = ret.Data.SharedData;
    DEM_FLAG_RESET(sharedData, (uint32)DEM_PRESTOREFF_STATUS);
    DEM_FLAG_SET(sharedData, DEM_PRESTORE_REQUEST_PRESTOREFF); /* PRQA S 1252 */ /* VL_Dem_1252 */
    ret.Data.SharedData = sharedData;
    return ret;
}

/**
 * @brief         reset current monitor prestore flag
 * @param[in]     SatelliteExtendedData: Satellite extended data
 * @return        Dem_SatelliteExtendedDataType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_SatelliteExtendedDataType
    Dem_ResetPrestoreStatus(Dem_SatelliteExtendedDataType SatelliteExtendedData)
{
    Dem_SatelliteExtendedDataType ret        = SatelliteExtendedData;
    uint32                        sharedData = ret.Data.SharedData;
    DEM_FLAG_RESET(sharedData, (uint32)DEM_PRESTOREFF_STATUS);
    ret.Data.SharedData = sharedData;
    return ret;
}

/**
 * @brief         set current monitor prestore clear flag
 * @param[in]     SatelliteExtendedData: Satellite extended data
 * @return        Dem_SatelliteExtendedDataType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_SatelliteExtendedDataType Dem_SetPrestoreClear(Dem_SatelliteExtendedDataType SatelliteExtendedData)
{
    Dem_SatelliteExtendedDataType ret        = SatelliteExtendedData;
    uint32                        sharedData = ret.Data.SharedData;
    DEM_FLAG_RESET(sharedData, (uint32)DEM_PRESTOREFF_STATUS);
    DEM_FLAG_SET(sharedData, (uint32)DEM_PRESTORE_REQUEST_CLEAR);
    ret.Data.SharedData = sharedData;
    return ret;
}
/* PRQA S 1258 -- */
/* PRQA S 3473 -- */
/**
 * @brief update current monitor prestore FF flag
 */
DEM_LOCAL void Dem_UpdatePrestoreStatus(Dem_EventIdType EventId, uint32 NewPrestoreFlag)
{
    boolean             ProcessingDone;
    Dem_SatelliteIdType satelliteId = Dem_GetSatelliteIdByEventId(EventId);
    Dem_MonitorIdType   monitorId   = Dem_GetMonitorIdByEventId(EventId);
    do
    {
        Dem_SatelliteExtendedDataType oldSatelliteEData = Dem_GetSatelliteExtendedData(satelliteId, monitorId);
        Dem_SatelliteExtendedDataType newSatelliteEData;
        uint32                        oldPrestoreFlag = (uint32)Dem_GetPrestoreStatus(oldSatelliteEData);
        if (oldPrestoreFlag != NewPrestoreFlag)
        {
            if (NewPrestoreFlag == DEM_PRESTORE_REQUEST_PRESTOREFF) /* PRQA S 1252 */ /* VL_Dem_1252 */
            {
                newSatelliteEData = Dem_SetPrestoreFF(oldSatelliteEData);
            }
            else
            {
                newSatelliteEData = Dem_SetPrestoreClear(oldSatelliteEData);
            }
            ProcessingDone =
                Dem_SyncSetSatelliteExtendedData(satelliteId, monitorId, oldSatelliteEData, newSatelliteEData);
        }
        else
        {
            /** Prestorage of freeze frame is already queued */
            ProcessingDone = TRUE;
        }
    } while (ProcessingDone == FALSE);
}

/**
 * @brief         get and clear current monitor prestore FF flag
 * @param[in]     SatelliteId: satellite Id
 * @param[in]     MonitorId: Monitor ID
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetAndClearPrestoreStatus(Dem_SatelliteIdType SatelliteId, Dem_MonitorIdType MonitorId)
{
    Dem_SatelliteExtendedDataType oldSatelliteEData;
    Dem_SatelliteExtendedDataType newSatelliteEData;
    do
    {
        oldSatelliteEData = Dem_GetSatelliteExtendedData(SatelliteId, MonitorId);
        newSatelliteEData = Dem_ResetPrestoreStatus(oldSatelliteEData);
    } while (Dem_SyncSetSatelliteExtendedData(SatelliteId, MonitorId, oldSatelliteEData, newSatelliteEData) == FALSE);
    return Dem_GetPrestoreStatus(oldSatelliteEData);
}

/**
 * @brief         check and clear Satellite prestore flag
 * @param[in]     SatelliteId: satellite Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetPrestoreQueueUpdated(Dem_SatelliteIdType SatelliteId)
{
    Dem_SatelliteInfoType oldSatelliteInfo;
    Dem_SatelliteInfoType newSatelliteInfo;
    do
    {
        oldSatelliteInfo = Dem_GetSatelliteInfo(SatelliteId);
        newSatelliteInfo = Dem_SetSatelliteInfoPrestoreQueue(oldSatelliteInfo);
    } while (Dem_SyncSetSatelliteInfo(SatelliteId, oldSatelliteInfo, newSatelliteInfo) == FALSE);
}

/**
 * @brief         set Satellite prestore flag
 * @param[in]     SatelliteId: satellite Id
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckAndClearPrestoreQueueUpdated(Dem_SatelliteIdType SatelliteId)
{
    Dem_SatelliteInfoType oldSatelliteInfo;
    Dem_SatelliteInfoType newSatelliteInfo;
    do
    {
        oldSatelliteInfo = Dem_GetSatelliteInfo(SatelliteId);
        newSatelliteInfo = Dem_SatelliteInfoResetPrestoreQueue(oldSatelliteInfo);
    } while (Dem_SyncSetSatelliteInfo(SatelliteId, oldSatelliteInfo, newSatelliteInfo) == FALSE);
    return Dem_CheckSatelliteInfoPrestoreQueue(oldSatelliteInfo);
}

/**
 * @brief prestore process
 */
DEM_LOCAL void Dem_PrestoreProcess(Dem_SatelliteIdType SatelliteId)
{
    for (Dem_MonitorIdType monitorId = 0u; monitorId < Dem_GetEventCountOfSatellite(SatelliteId); ++monitorId)
    {
        Dem_SatelliteExtendedDataType satelliteExtendedData = Dem_GetSatelliteExtendedData(SatelliteId, monitorId);
        if (Dem_GetPrestoreStatus(satelliteExtendedData) != DEM_PRESTORE_REQUEST_INITIAL)
        {
            uint8           prestoreStatus = Dem_GetAndClearPrestoreStatus(SatelliteId, monitorId);
            Dem_EventIdType eventId        = Dem_SatelliteGetEventId(SatelliteId, monitorId);

            if (prestoreStatus == DEM_PRESTORE_REQUEST_PRESTOREFF)
            {
                (void)Dem_PrestorageUpdate(eventId);
            }
            else
            {
                (void)Dem_PrestorageClear(eventId);
            }
        }
    }
}
/* PRQA S 0759 -- */

/**
 * @brief Prestorage Task process
 */
DEM_LOCAL void Dem_PrestorageTask(void)
{
    for (Dem_SatelliteIdType satelliteId = 0u; satelliteId < Dem_GetNumOfSatellite(); ++satelliteId)
    {
        if (Dem_CheckAndClearPrestoreQueueUpdated(satelliteId) == TRUE)
        {
            Dem_PrestoreProcess(satelliteId);
        }
    }
}
#endif

/**
 * @brief Processing event pre-storage requests
 */
DEM_LOCAL Std_ReturnType Dem_EventPrestoreFF(Dem_EventIdType EventId)
{
    Std_ReturnType ret = E_NOT_OK;
    /** Events without configured DTCs are not allowed to enable pre-stored freeze frames, as guaranteed by the tool
     * calibration */
    if ((Dem_GetPreFFIndexOfEvent(EventId) != DEM_PRESTORED_EVENT_NUM)
        && (Dem_GetAvailableInVariantByEvent(EventId) == TRUE)
        && (Dem_GetEventAvailableStatusByEventId(EventId) == TRUE))
    {
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
        if (GetApplicationID() != DEM_MASTER_APPLICATION_ID) /* PRQA S 1252 */ /* VL_Dem_1252 */
        {
            Dem_UpdatePrestoreStatus(EventId, DEM_PRESTORE_REQUEST_PRESTOREFF);
            Dem_SetPrestoreQueueUpdated(Dem_GetSatelliteIdByEventId(EventId));
            ret = E_OK;
        }
        else
#endif
        {
            ret = Dem_PrestorageUpdate(EventId);
        }
    }
    return ret;
}

/**
 * @brief clear Processed event pre-storage Freeze frame
 */
DEM_LOCAL Std_ReturnType Dem_EventClearPrestoreFF(Dem_EventIdType EventId)
{
    Std_ReturnType ret = E_NOT_OK;
    /** Events without configured DTCs are not allowed to enable pre-stored freeze frames, as guaranteed by the tool
     * calibration */
    if ((Dem_GetPreFFIndexOfEvent(EventId) != DEM_PRESTORED_EVENT_NUM)
        && (Dem_GetAvailableInVariantByEvent(EventId) == TRUE)
        && (Dem_GetEventAvailableStatusByEventId(EventId) == TRUE))
    {
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
        if (GetApplicationID() != DEM_MASTER_APPLICATION_ID) /* PRQA S 1252 */ /* VL_Dem_1252 */
        {
            Dem_UpdatePrestoreStatus(EventId, DEM_PRESTORE_REQUEST_CLEAR);
            Dem_SetPrestoreQueueUpdated(Dem_GetSatelliteIdByEventId(EventId));
            ret = E_OK;
        }
        else
#endif
        {
            ret = Dem_PrestorageClear(EventId);
        }
    }
    return ret;
}

#if (DEM_FREEZE_FRAME_CLASS_NUMBER > 0u)
/**
 * @brief Copy a prestored snapshot for an event
 */
DEM_LOCAL boolean Dem_PrestoreApplyFreezeFrame(uint8* DestinationBuffer, uint16 BufferSize)
{
    boolean                 ret = FALSE;
    Dem_PrestorageIndexType prestoreLockedIndex;
    prestoreLockedIndex = Dem_PrestoreLockedList;
    if (Dem_PreFFSize() != prestoreLockedIndex)
    {
        Dem_MemCpy(DestinationBuffer, Dem_GetPrestorageFFPtr(prestoreLockedIndex), BufferSize);
        ret = TRUE;
    }
    return ret;
}
#endif

#if (DEM_MAX_SIZE_J1939FREEZEFRAME > 0u)
/**
 * @brief Copy a prestored J1939 snapshot for an event
 */
DEM_LOCAL boolean Dem_PrestoreApplyJ1939FreezeFrame(uint8* DestinationBuffer, uint16 BufferSize)
{
    boolean                 ret = FALSE;
    Dem_PrestorageIndexType prestoreLockedIndex;
    prestoreLockedIndex = Dem_PrestoreLockedList;
    if (Dem_PreFFSize() != prestoreLockedIndex)
    {
        Dem_MemCpy(DestinationBuffer, Dem_GetPrestorageJ1939FFPtr(prestoreLockedIndex), BufferSize);
        ret = TRUE;
    }
    return ret;
}
#endif

#if (DEM_MAX_SIZE_J1939EXFREEZEFRAME > 0u)
/**
 * @brief Copy a prestored J1939Ex snapshot for an event
 */
DEM_LOCAL boolean Dem_PrestoreApplyJ1939ExFreezeFrame(uint8* DestinationBuffer, uint16 BufferSize)
{
    boolean                 ret = FALSE;
    Dem_PrestorageIndexType prestoreLockedIndex;
    prestoreLockedIndex = Dem_PrestoreLockedList;
    if (Dem_PreFFSize() != prestoreLockedIndex)
    {
        Dem_MemCpy(DestinationBuffer, Dem_GetPrestorage1939ExFFPtr(prestoreLockedIndex), BufferSize);
        ret = TRUE;
    }
    return ret;
}
#endif

#if ((DEM_OBDII_SUPPORT == STD_ON) && (DEM_MAX_SIZE_OBDFREEZEFRAME > 0u))
/**
 * @brief Copy a prestored OBD snapshot for an event
 */
DEM_LOCAL boolean Dem_PrestoreApplyOBDFreezeFrame(uint8* DestinationBuffer, uint16 BufferSize)
{
    boolean                 ret = FALSE;
    Dem_PrestorageIndexType prestoreLockedIndex;
    prestoreLockedIndex = Dem_PrestoreLockedList;
    if (Dem_PreFFSize() != prestoreLockedIndex)
    {
        Dem_MemCpy(DestinationBuffer, Dem_GetPrestorageOBDFFPtr(prestoreLockedIndex), BufferSize);
        ret = TRUE;
    }
    return ret;
}
#endif

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

#endif
