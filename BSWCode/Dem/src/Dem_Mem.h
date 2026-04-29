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
 **  @file               : Dem_Mem.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : Implementation file of Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_MEM_H
#define DEM_MEM_H

/* =================================================== inclusions =================================================== */
#include "Dem_PreStore.h"
#include "Dem_FreezeFrame.h"
#include "Dem_EnableCondition.h"
#include "Dem_MemInterface.h"
#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
#include "NvM.h"
#endif
#if (DEM_CLEAR_DTC_BEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
#include "Dem_ClearDTC.h"
#endif
#include "Dem_Error.h"

/* ===================================================== macros ===================================================== */
#if (defined(DEM_MAX_FDC_SINCE_LAST_CLEAR))
#define DEM_ENTRY_MAX_FDC_VALUE ((sint16)127) /**< entry store max FDC value */
#endif

/* ========================================== internal function declarations ======================================== */
/* PRQA S 1512 EOF */ /* VL_Dem_1512 */
/**
 * @brief         Check if candidate has an event priority, that qualifies it or disqualifies it for the displacement.
 * Otherwise it needs further tests.
 * @param[in]     BestMatchInfo: Pointer to match information
 * @param[in]     CandidateInfo: Pointer to candidate information
 * @return        Dem_MemDisplaceCheckResultType
 * @retval        Dem_MemDisplace_NeedMoreTests: Candidate is not classified yet, apply further tests on this candidate
 * @retval        Dem_MemDisplace_RatingDone: Candidate is classified, continue the scan with next candidate
 * @retval        Dem_MemDisplace_FoundBestMatch: Candidate is the best match, abort scanning for better matches
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Dem_MemDisplaceCheckResultType Dem_MemDisplaceCheckPriority(
    Dem_MemDisplacementInfoType* const       BestMatchInfo,
    const Dem_MemDisplacementInfoType* const CandidateInfo);

#if (DEM_GENERAL_DISPLACEMENT_FULL == STD_ON)
/**
 * @brief         Check if candidate is passive, and this qualifies it for the displacement. Otherwise it needs further
 * tests.
 * @param[in]     BestMatchInfo: Pointer to match information
 * @param[in]     CandidateInfo: Pointer to candidate information
 * @return        Dem_MemDisplaceCheckResultType
 * @retval        Dem_MemDisplace_NeedMoreTests: Candidate is not classified yet, apply further tests on this candidate
 * @retval        Dem_MemDisplace_RatingDone: Candidate is classified, continue the scan with next candidate
 * @retval        Dem_MemDisplace_FoundBestMatch: Candidate is the best match, abort scanning for better matches
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Dem_MemDisplaceCheckResultType Dem_MemDisplaceCheckPassiveState(
    Dem_MemDisplacementInfoType* const       BestMatchInfo,
    const Dem_MemDisplacementInfoType* const CandidateInfo);

/**
 * @brief         Check if candidate is passive, and this qualifies it for the displacement. Otherwise it needs further
 * tests.
 * @param[in]     BestMatchInfo: Pointer to match information
 * @param[in]     CandidateInfo: Pointer to candidate information
 * @return        Dem_MemDisplaceCheckResultType
 * @retval        Dem_MemDisplace_NeedMoreTests: Candidate is not classified yet, apply further tests on this candidate
 * @retval        Dem_MemDisplace_RatingDone: Candidate is classified, continue the scan with next candidate
 * @retval        Dem_MemDisplace_FoundBestMatch: Candidate is the best match, abort scanning for better matches
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Dem_MemDisplaceCheckResultType Dem_MemDisplaceCheckReadinessState(
    Dem_MemDisplacementInfoType* const       BestMatchInfo,
    const Dem_MemDisplacementInfoType* const CandidateInfo);
#endif

/**
 * @brief         Selects an event to displace in a filled memory
 * @param[in]     MemoryInfo: Record memory information
 * @param[in]     DisplacingEventId: Displac event Id
 * @param[in]     displacementStrategy: Displacement strategy
 * @return        Dem_NvBlockNumType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Dem_NvBlockNumType Dem_MemSelectDisplacedIndex(
    Dem_MemStateInfoConstPtrType      MemoryInfo,
    Dem_EventIdType                   DisplacingEventId,
    Dem_EventDisplacementStrategyType displacementStrategy);

/**
 * @brief         This function manages the event memory storage. If a free slot is available, it is returned directly.
 * In case no free slot is available, the configured displacement algorithm is applied to free a slot.
 * @param[in]     MemoryInfo: Record memory information
 * @param[in]     EventId: Event Id
 * @return        Dem_NvBlockNumType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Dem_NvBlockNumType Dem_MemoryAllocateIndex(Dem_MemStateInfoConstPtrType MemoryInfo, Dem_EventIdType EventId);

/**
 * @brief         This function will look for every snapshot record of the event that needs updating due to the status
 * changes encoded by the trigger flags.
 * @param[in]     MemoryIndex: Memory index
 * @param[in]     EventId: Event Id
 * @param[in]     Trigger: The event update trigger
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL uint8 Dem_MemUpdateEventFreezeFrame(Dem_EventIdType EventId, Dem_NvBlockNumType MemoryIndex, uint8 Trigger);

#if (DEM_MAX_SIZE_J1939EXFREEZEFRAME > 0u)
/**
 * @brief         Collect J1939 Freeze Frame data stored for an event.
 * @param[in]     MemoryIndex: Memory index
 * @param[in]     EventId: Event Id
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL uint8 Dem_MemUpdateEventJ1939FF(Dem_EventIdType EventId, Dem_NvBlockNumType MemoryIndex);
#endif

#if ((DEM_OBDII_SUPPORT == STD_ON) && (DEM_MAX_SIZE_OBDFREEZEFRAME > 0u))
/**
 * @brief         Collect OBD Freeze Frame data stored for an event.
 * @param[in]     MemoryIndex: Memory index
 * @param[in]     EventId: Event Id
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL uint8 Dem_MemUpdateEventOBDFF(Dem_EventIdType EventId, Dem_NvBlockNumType MemoryIndex);
#endif

/**
 * @brief         Update the chronology of one entry in the event memory.
 * @param[in]     MemoryInfo: Record memory information
 * @param[in]     MemoryIndex: Memory index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_MemoryUpdateIndex(Dem_MemStateInfoConstPtrType MemoryInfo, Dem_NvBlockNumType MemoryIndex);

/**
 * @brief         Initializes Event memory
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_MemoryEventInit(void);

#if (DEM_COMBINATION_NUMBER > 0u)
/**
 * @brief         Initializes Combined Event memory
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_MemoryCombinedEventInit(void);
#endif

/**
 * @brief         Check for corrupt memory entry.
 * @param[in]     MemoryInfo: Record memory information
 * @param[in]     MemoryIndex: Memory index
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_MemFixCorruptMemoryEntry(Dem_MemStateInfoConstPtrType MemoryInfo, Dem_NvBlockNumType MemoryIndex);

/**
 * @brief         Check for duplicate memory entry.
 * @param[in]     MemoryInfo: Record memory information
 * @param[in]     MemoryIndex: Memory index
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_MemFixDoubleAllocation(Dem_MemStateInfoConstPtrType MemoryInfo, Dem_NvBlockNumType MemoryIndex);

#if (DEM_ENTRY_TRIGGER_ON_CONFIRMED == STD_ON)
/**
 * @brief         Restore parts of the UDS status, using redundant data, when storage happens on confirmed.
 * @param[in]     EventId: Event Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_MemRestoreStatusBitsOnConfirmed(Dem_EventIdType EventId);
#endif

#if (DEM_ENTRY_TRIGGER_ON_TEST_FAILED == STD_ON)
/**
 * @brief         Restore parts of the UDS status, using redundant data, when storage happens on failed.
 * @param[in]     EventId: Event Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_MemRestoreStatusBitsOnFailed(Dem_EventIdType EventId);
#endif

#if ((DEM_ENTRY_TRIGGER_ON_FDC_THRESHOLD == STD_ON) && (defined(DEM_OCCCTR)))
/**
 * @brief         Restore parts of the UDS status, using redundant data, when storage happens at a FDC threshold.
 * @param[in]     EventId: Event Id
 * @param[in]     MemoryIndex: Memory index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_MemRestoreStatusBitsOnFdc(Dem_NvBlockNumType MemoryIndex, Dem_EventIdType EventId);
#endif

/**
 * @brief         Update the aging status of an event in it's internal status.
 * @param[in]     InternalStatus: Internal status
 * @param[in]     MemoryIndex: Memory index
 * @return        Dem_EventInternalStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Dem_EventInternalStatusType
    Dem_MemRestoreStoredStatusFromEntry(Dem_NvBlockNumType MemoryIndex, Dem_EventInternalStatusType InternalStatus);

/**
 * @brief         Restores the event status and internal status based on the configured storage trigger.
 * @param[in]     EventId: Event Id
 * @param[in]     MemoryIndex: Memory index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_MemRestoreEventStatus(Dem_NvBlockNumType MemoryIndex, Dem_EventIdType EventId);

/**
 * @brief         Sort the memory entry into the chronological list.
 * @param[in]     MemoryInfo: Record memory information
 * @param[in]     MemoryIndex: Memory Index
 * @param[in]     Timestamp: Time stamp
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_MemSortEntryIntoChronologicalList(
    Dem_MemStateInfoConstPtrType MemoryInfo,
    Dem_NvBlockNumType           MemoryIndex,
    uint32                       Timestamp);

/**
 * @brief         verifies the integrity of memory entries and links them up into the runtime control structures.
 * @param[out]    MemIndex: Memory Index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_MemoryInitRestoreMemory(Dem_MemoryNumType MemIndex);

/**
 * @brief         Initializes NvM Memory use in dem
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_NvMMemoryInit(void);

/**

 * @brief         Function updates environmental data of an event on a Test Failed this cycle trigger.
 * @param[in]     MemoryIndex: Memory index
 * @param[in]     EventId: Event Id
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL uint8 Dem_MemUpdateEntryOnTFTOC(Dem_EventIdType EventId, Dem_NvBlockNumType MemoryIndex);

#if (DEM_FEATURE_NVM_IMMEDIATE == STD_ON)
#if (DEM_FEATURE_SUPPORT_NVM_POLLING == STD_ON)
/**
 * @brief         Poll the result of an ongoing write job
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_NvmGetNvMResult(void);
#endif

/**
 * @brief         Find Next Block To Commit
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_NvmFindNextBlockToCommit(void);

/**
 * @brief         writes the memory block marked for immediate synchronization into the NV storage.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_NvmCommitBlock(void);
#endif

/* ============================================ internal data definitions =========================================== */
#define DEM_START_SEC_VAR_CLEARED_32
#include "Dem_MemMap.h"
/**
 * @brief Current time stamp, used to establish a chronological ordering
 * @range 0..4294967295
 */
DEM_LOCAL uint32 Dem_MemCurrentTimestamp;
#define DEM_STOP_SEC_VAR_CLEARED_32
#include "Dem_MemMap.h"

/* ========================================== internal function definitions ========================================= */
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         set Block Status
 * @param[in]     MemoryIndex: Memory index
 * @param[in]     Status: Memory status
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_NvmSetBlockState(Dem_NvBlockNumType MemoryIndex, uint8 Status)
{
#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
    Dem_GetMemoryStatus()[MemoryIndex] = Status;
#endif
}

/**
 * @brief         get Block Status
 * @param[in]     MemoryIndex: Memory index
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_NvmGetBlockState(Dem_NvBlockNumType MemoryIndex)
{
#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
    return Dem_GetMemoryStatus()[MemoryIndex];
#else
    return DEM_NVM_BLOCKSTATE_UNMODIFIED;
#endif
}

/**
 * @brief         get Memory Overflow of Admin Data
 */
DEM_LOCAL_INLINE boolean Dem_GetMemoryOverflow(Dem_MemoryNumType MemIndex)
{
    return DEM_BITS_ISSET(Dem_GetAdminData().MemoryOverflow, MemIndex); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/**
 * @brief         Set Memory Overflow of Admin Data
 * @param[in]     MemIndex: Memory index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetMemoryOverflow(Dem_MemoryNumType MemIndex)
{
    DEM_BITS_SET(Dem_GetAdminData().MemoryOverflow, MemIndex);
}

/**
 * @brief         Reset Memory Overflow of Admin Data
 */
DEM_LOCAL_INLINE void Dem_ResetMemoryOverflow(Dem_MemoryNumType MemIndex)
{
    DEM_BITS_RESET(Dem_GetAdminData().MemoryOverflow, MemIndex);
}

#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
#if (DEM_FEATURE_NVM_IMMEDIATE == STD_ON)
/**
 * @brief         Check Block State Immediate
 * @param[out]    BlockState: Status of the block
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_NvmCheckBlockStateImmediate(uint8 BlockState)
{
    return DEM_FLAG_ISSET(BlockState, DEM_NVM_BLOCKSTATE_IMMEDIATEFLAG); /* PRQA S 4404 */ /* VL_Dem_4404 */
}
#endif

#if (DEM_CLEAR_DTC_BEHAVIOR != DEM_CLRRESP_VOLATILE)
/**
 * @brief         Check Block State Clear
 * @param[out]    BlockState: Status of the block
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_NvmCheckBlockStateClear(uint8 BlockState)
{
    return DEM_FLAG_ISSET(BlockState, DEM_NVM_BLOCKSTATE_CLEARFLAG); /* PRQA S 4404 */ /* VL_Dem_4404 */
}
#endif

#if (DEM_FEATURE_NVM_IMMEDIATE == STD_ON)
/**
 * @brief         Gets the currently processed memory block
 * @return        Dem_NvBlockNumType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_NvBlockNumType Dem_NvmGetCurrentMemoryIndex(void)
{
    return Dem_GetNvmCommitData().MemoryIndex;
}

/**
 * @brief         Sets the currently processed memory block
 * @param[in]     BlockIndex: Block index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_NvmSetCurrentMemoryIndex(Dem_NvBlockNumType BlockIndex)
{
    Dem_GetNvmCommitData().MemoryIndex = BlockIndex;
}

/**
 * @brief         Increment Pending Commits
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_NvmIncrementPendingCommits(void)
{
    Dem_GetNvmCommitData().PendingCount++;
}

/**
 * @brief         Decrement Pending Commits
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_NvmDecrementPendingCommits(void)
{
    Dem_GetNvmCommitData().PendingCount--;
}

/**
 * @brief         Check Pending Commits
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_NvmCheckPendingCommits(void)
{
    return (Dem_GetNvmCommitData().PendingCount > 0u); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/**
 * @brief         Resets the number of to be processed NvRAM blocks
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_NvmResetPendingCommits(void)
{
    Dem_GetNvmCommitData().PendingCount = 0u;
}

/**
 * @brief         Gets the state of the current commit
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_NvmGetCurrentCommitState(void)
{
    return Dem_GetNvmCommitData().CommitState;
}

/**
 * @brief         Sets the state of the current commit
 * @param[in]     CommitState: Commit state
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_NvmSetCurrentCommitState(uint8 CommitState)
{
    Dem_GetNvmCommitData().CommitState = CommitState;
}

#if (DEM_CLEAR_DTC_BEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
/**
 * @brief         Increment Pending Clear Commits
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_NvmIncrementPendingClearCommits(void)
{
    Dem_GetNvmCommitData().PendingClearCount++;
}

/**
 * @brief         Decrement Pending Clear Commits
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_NvmDecrementPendingClearCommits(void)
{
    Dem_GetNvmCommitData().PendingClearCount--;
}

/**
 * @brief         Check Pending Clear Commits
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_NvmCheckPendingClearCommits(void)
{
    return (Dem_GetNvmCommitData().PendingClearCount > 0u); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/**
 * @brief         reset Pending Clear Commits
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_NvmResetPendingClearCommits(void)
{
    Dem_GetNvmCommitData().PendingClearCount = 0u;
}

/**
 * @brief         Check if the currently processed block was cleared
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_NvmCheckCurrentBlockIsCleared(void)
{
    return Dem_GetNvmCommitData().BlockIsCleared;
}
/**
 * @brief         Sets the value of the 'BlockIsCleared' flag for the currently  processed block
 * @param[in]     BlockIsCleared: Flag of cleared
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_NvmSetCurrentBlockIsCleared(boolean BlockIsCleared)
{
    Dem_GetNvmCommitData().BlockIsCleared = BlockIsCleared;
}
#endif
#endif

/**
 * @brief set Single Block Status
 */
DEM_LOCAL void Dem_NvmSetSingleBlockState(Dem_NvBlockNumType MemoryIndex, uint8 Status)
{
#if (DEM_FEATURE_NVM_IMMEDIATE == STD_ON)
    if ((Dem_NvmCheckBlockStateImmediate(Status) == TRUE)
        && (Dem_NvmCheckBlockStateImmediate(Dem_NvmGetBlockState(MemoryIndex)) == FALSE))
    {
        /** Only if Immediate flag is being set, not if it is already set */
        Dem_NvmIncrementPendingCommits();
    }

#if (DEM_CLEAR_DTC_BEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
    if ((Dem_NvmCheckBlockStateClear(Status) == TRUE)
        && (Dem_NvmCheckBlockStateClear(Dem_NvmGetBlockState(MemoryIndex)) == FALSE))
    {
        /** Only if clear flag is being set, not if it is already set */
        Dem_NvmIncrementPendingClearCommits();
    }
#endif

    if (Dem_NvmGetBlockState(MemoryIndex) < Status)
#endif
    {
        Dem_NvmSetBlockState(MemoryIndex, Status);
    }
}

/**
 * @brief         set Event Status Block need to store
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_NvmSetStatusBlockChanged(void)
{
#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
    Dem_NvmSetSingleBlockState(DEM_NV_EVENTSTATUSBLOCKINDEX, DEM_NVM_BLOCKSTATE_DIRTY);
#endif
}

/**
 * @brief         check Event Status Block need to store
 * @param[in]     OldEventStatus: Old event status
 * @param[in]     NewEventStatus: New event status
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void
    Dem_CheckEventStatusBlock(Dem_UdsStatusByteType OldEventStatus, Dem_UdsStatusByteType NewEventStatus)
{
    if (Dem_CheckNvRelevantStatus(OldEventStatus, NewEventStatus) == TRUE)
    {
        Dem_NvmSetStatusBlockChanged();
    }
}

#if (DEM_FEATURE_NVM_IMMEDIATE == STD_ON)
/**
 * @brief         Restore internal state for block marked as 'dirty'
 * processes the NV block according to BlockState:
 * DEM_NVM_BLOCKSTATE_IMMEDIATEFLAG - The block is queued for immediate NV update.
 * DEM_NVM_BLOCKSTATE_CLEARFLAG - The block was cleared.
 * DEM_NVM_BLOCKSTATE_DIRTY - The block should be synchronized to NV-Ram during WriteAll.
 * @param[in]     BlockIndex: Block index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_NvmRestoreBlockStateImmediate(Dem_NvBlockNumType BlockIndex)
{
    if (Dem_NvmCheckBlockStateImmediate(Dem_NvmGetBlockState(BlockIndex)) == TRUE)
    {
        Dem_NvmIncrementPendingCommits();
    }

#if (DEM_CLEAR_DTC_BEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
    if (Dem_NvmCheckBlockStateClear(Dem_NvmGetBlockState(BlockIndex)) == TRUE)
    {
        Dem_NvmIncrementPendingClearCommits();
    }
#endif
}
#endif

/**
 * @brief         Initializes the NvM management on wake up
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_NvmInitWakeup(void)
{
#if (DEM_FEATURE_NVM_IMMEDIATE == STD_ON)
    for (Dem_NvBlockNumType blockIndex = 0u; blockIndex < DEM_NV_RAM_BLOCK_NUMBER; ++blockIndex)
    {
        if (Dem_NvmGetBlockState(blockIndex) != DEM_NVM_BLOCKSTATE_UNMODIFIED)
        {
            Dem_NvmRestoreBlockStateImmediate(blockIndex);
        }
    }
#endif
}

/**
 * @brief         Initializes the NvM management on startup
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_NvmInitStartup(void)
{
    Dem_MemSet((uint8*)&Dem_GetMemoryStatus(), DEM_NVM_BLOCKSTATE_UNMODIFIED, DEM_NV_RAM_BLOCK_NUMBER);
#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
    Dem_NvmSetSingleBlockState(DEM_NV_ADMINBLOCKINDEX, DEM_NVM_BLOCKSTATE_DIRTY);
#endif
#if (DEM_FEATURE_NVM_IMMEDIATE == STD_ON)
    Dem_NvmSetCurrentMemoryIndex(0u);
    Dem_NvmResetPendingCommits();
    Dem_NvmSetCurrentCommitState(DEM_NVM_COMMIT_IDLE);
#if (DEM_CLEAR_DTC_BEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
    Dem_NvmResetPendingClearCommits();
    Dem_NvmSetCurrentBlockIsCleared(FALSE);
#endif
#endif
}

/**
 * @brief         Initializes the NvM management
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_NvmInit(void)
{
    if (Dem_CheckMasterInitStatus(DEM_SHUTDOWN) == TRUE)
    {
        Dem_NvmInitWakeup();
    }
    else
    {
        Dem_NvmInitStartup();
    }

#if (DEM_FEATURE_NVM_IMMEDIATE == STD_ON)
    Dem_EnableTask(Dem_Task_NvM);
#endif
}

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Mark update started for the memory block.
 * @param[in]     MemoryIndex: Memory index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_MemoryUpdateStart(Dem_NvBlockNumType MemoryIndex)
{
#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
    DEM_FLAG_SET(Dem_GetMemoryEntryStatus()[MemoryIndex], DEM_ENTRY_UPDATE_STARTED); /* PRQA S 3473 */ /* VL_Dem_3473 */
#endif
}

/**
 * @brief         Mark update finished for the memory block.
 * @param[in]     MemoryIndex: Memory index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_MemoryUpdateFinish(Dem_NvBlockNumType MemoryIndex)
{
#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
    /* PRQA S 3473, 4399, 4461 ++ */ /* VL_Dem_3473, VL_Dem_4399, VL_Dem_4461 */
    DEM_FLAG_RESET(Dem_GetMemoryEntryStatus()[MemoryIndex], DEM_ENTRY_UPDATE_STARTED);
/* PRQA S 3473, 4399, 4461 -- */
#endif
}

/**
 * @brief         Get the update state of the memory block.
 */
DEM_LOCAL_INLINE boolean Dem_MemoryUpdateGetState(Dem_NvBlockNumType MemoryIndex)
{
#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
    /* PRQA S 4404 ++ */ /* VL_Dem_4404 */
    return DEM_FLAG_ISSET(Dem_GetMemoryEntryStatus()[MemoryIndex], DEM_ENTRY_UPDATE_STARTED);
/* PRQA S 4404 -- */
#else
    return FALSE;
#endif
}

/**
 * @brief         Get the update state of the memory block.
 * @param[in]     MemoryIndex: Memory index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_MemoryUpdateInit(Dem_NvBlockNumType MemoryIndex)
{
#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
    Dem_GetMemoryEntryStatus()[MemoryIndex] = DEM_ENTRY_UPDATE_INIT;
#endif
}

/**
 * @brief         Get DTC Status Mask By Mem
 */
DEM_LOCAL_INLINE uint8 Dem_GetDTCStatusMaskByMem(Dem_MemoryNumType MemIndex)
{
    return Dem_GetDtcStatusMaskOfMemory(MemIndex);
}

/**
 * @brief         Init MemStateInfo
 */
DEM_LOCAL_INLINE Dem_MemStateInfoPtrType Dem_MemStateInfoInit(Dem_MemoryNumType MemIndex)
{
    return &Dem_GetMemStateInfo()[MemIndex];
}

/**
 * @brief         get MemEntry start of MemStateInfo
 * @param[in]     MemoryInfo: Record memory information
 * @return        Dem_NvBlockNumType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_NvBlockNumType Dem_GetMemEntryStartOfMemoryInfo(Dem_MemStateInfoConstPtrType MemoryInfo)
{
    return MemoryInfo->FirstMemoryIndex;
}

/**
 * @brief         get MemEntry End of MemStateInfo
 * @param[in]     MemoryInfo: Record memory information
 * @return        Dem_NvBlockNumType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_NvBlockNumType Dem_GetMemEntryEndOfMemoryInfo(Dem_MemStateInfoConstPtrType MemoryInfo)
{
    return MemoryInfo->FirstMemoryIndex + (Dem_NvBlockNumType)MemoryInfo->MaxSize;
}

/**
 * @brief         get MaxSize of MemStateInfo
 * @param[in]     MemoryInfo: Record memory information
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetMaxSizeOfMemoryInfo(Dem_MemStateInfoConstPtrType MemoryInfo)
{
    return MemoryInfo->MaxSize;
}

/**
 * @brief         get current size of MemStateInfo
 * @param[in]     MemoryInfo: Record memory information
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetCurSizeOfMemoryInfo(Dem_MemStateInfoConstPtrType MemoryInfo)
{
    return *(MemoryInfo->SizePtr);
}

/**
 * @brief         set current size of MemStateInfo
 * @param[in]     MemoryInfo: Record memory information
 * @param[in]     CurSize: Current memory size
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetCurSizeOfMemoryInfo(Dem_MemStateInfoConstPtrType MemoryInfo, uint8 CurSize)
{
    *(MemoryInfo->SizePtr) = CurSize;
}

/**
 * @brief         get the Nth oldest memory index of MemStateInfo. 0 is the oldest index.
 * @param[in]     MemoryInfo: Record memory information
 * @param[in]     ChronoIndex: Chrono index
 * @return        Dem_NvBlockNumType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_NvBlockNumType
    Dem_GetChronologyOfMemoryInfo(const Dem_MemStateInfoType* MemoryInfo, uint8 ChronoIndex)
{
    return MemoryInfo->ChronoPtr[ChronoIndex];
}

/**
 * @brief         Set the Nth oldest memory index of MemStateInfo. 0 is the oldest index.
 * @param[in]     MemoryInfo: Record memory information
 * @param[in]     ChronoIndex: Chrono index
 * @param[in]     MemoryIndex: Memory index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetChronologyOfMemoryInfo(
    Dem_MemStateInfoConstPtrType MemoryInfo,
    uint8                        ChronoIndex,
    Dem_NvBlockNumType           MemoryIndex)
{
    MemoryInfo->ChronoPtr[ChronoIndex] = MemoryIndex;
}

/**
 * @brief         Set current value for time stamp.
 * @param[in]     CurrentTimeStamp: Current time stamp
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_MemSetCurrentTimestamp(uint32 CurrentTimeStamp)
{
    Dem_MemCurrentTimestamp = CurrentTimeStamp;
}

/**
 * @brief         Get current value for time stamp.
 * @return        uint32
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint32 Dem_MemGetCurrentTimestamp(void)
{
    return Dem_MemCurrentTimestamp;
}

/**
 * @brief         get MemIndex of MemStateInfo
 * @param[in]     MemoryInfo: Record memory information
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetMemIndexOfMemoryInfo(Dem_MemStateInfoConstPtrType MemoryInfo)
{
    return MemoryInfo->MemIndex;
}

/**
 * @brief         get EventId of MemEntry by MemoryIndex
 */
DEM_LOCAL_INLINE Dem_EventIdType Dem_GetEventIdOfMemEntry(Dem_NvBlockNumType MemoryIndex)
{
    return Dem_GetMemoryEntry()[MemoryIndex].EventId;
}

/**
 * @brief         Set EventId of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory index
 * @param[in]     EventId: Event Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetEventIdOfMemEntry(Dem_NvBlockNumType MemoryIndex, Dem_EventIdType EventId)
{
    Dem_GetMemoryEntry()[MemoryIndex].EventId = EventId;
}

/**
 * @brief         get Timestamp of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory index
 * @return        uint32
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint32 Dem_GetTimestampOfMemEntry(Dem_NvBlockNumType MemoryIndex)
{
    return Dem_GetMemoryEntry()[MemoryIndex].Timestamp;
}

/**
 * @brief         Set Timestamp of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory index
 * @param[in]     Timestamp: Time stamp
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetTimestampOfMemEntry(Dem_NvBlockNumType MemoryIndex, uint32 Timestamp)
{
    Dem_GetMemoryEntry()[MemoryIndex].Timestamp = Timestamp;
}

#if (defined(DEM_OCCCTR))
/**
 * @brief         get OccurrenceCounter of MemEntry by MemoryIndex
 */
DEM_LOCAL_INLINE uint8 Dem_GetOCCOfMemEntry(Dem_NvBlockNumType MemoryIndex)
{
    return Dem_GetMemoryEntry()[MemoryIndex].OccurrenceCounter;
}

/**
 * @brief         Set OccurrenceCounter of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory index
 * @param[in]     OccurrenceCounter: Occurrence Counter
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetOCCOfMemEntry(Dem_NvBlockNumType MemoryIndex, uint8 OccurrenceCounter)
{
    Dem_GetMemoryEntry()[MemoryIndex].OccurrenceCounter = OccurrenceCounter;
}
#endif

/**
 * @brief         get AgingCounter of MemEntry by MemoryIndex
 */
DEM_LOCAL_INLINE uint16 Dem_GetAgingCounterOfMemEntry(Dem_NvBlockNumType MemoryIndex)
{
    return Dem_GetMemoryEntry()[MemoryIndex].AgingCounter;
}

/**
 * @brief         Set AgingCounter of MemEntry by MemoryIndex
 */
DEM_LOCAL_INLINE void Dem_SetAgingCounterOfMemEntry(Dem_NvBlockNumType MemoryIndex, uint16 AgingCounter)
{
    Dem_GetMemoryEntry()[MemoryIndex].AgingCounter = AgingCounter;
}

#if (defined(DEM_MAX_FDC_SINCE_LAST_CLEAR))
/**
 * @brief         get MaxDebounceValue of MemEntry by MemoryIndex
 */
DEM_LOCAL_INLINE sint16 Dem_GetMaxDebounceValueOfMemEntry(Dem_NvBlockNumType MemoryIndex)
{
    return Dem_GetMemoryEntry()[MemoryIndex].MaxDebounceValue;
}

/**
 * @brief         Set MaxDebounceValue of MemEntry by MemoryIndex
 */
DEM_LOCAL_INLINE void Dem_SetMaxDebounceValueOfMemEntry(Dem_NvBlockNumType MemoryIndex, sint16 MaxDebounceValue)
{
    Dem_GetMemoryEntry()[MemoryIndex].MaxDebounceValue = MaxDebounceValue;
}
#endif

#if (defined(DEM_FAILED_CYCLES))
/**
 * @brief         get FailedCycleCounter of MemEntry by MemoryIndex
 */
DEM_LOCAL_INLINE uint8 Dem_GetFailedCycleCounterOfMemEntry(Dem_NvBlockNumType MemoryIndex)
{
    return Dem_GetMemoryEntry()[MemoryIndex].FailedCycleCounter;
}

/**
 * @brief         Set FailedCycleCounter of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory index
 * @param[in]     FailedCycleCounter: Failed cycle counter
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetFailedCycleCounterOfMemEntry(Dem_NvBlockNumType MemoryIndex, uint8 FailedCycleCounter)
{
    Dem_GetMemoryEntry()[MemoryIndex].FailedCycleCounter = FailedCycleCounter;
}
#endif

#if ((defined(DEM_CYCLES_SINCE_FIRST_FAILED)) || (defined(DEM_OCC3)))
/**
 * @brief         get CyclesSinceFirstFailed of MemEntry by MemoryIndex
 */
DEM_LOCAL_INLINE uint16 Dem_GetCyclesSinceFirstFailedOfMemEntry(Dem_NvBlockNumType MemoryIndex)
{
    return Dem_GetMemoryEntry()[MemoryIndex].CyclesSinceFirstFailed;
}

/**
 * @brief         Set CyclesSinceFirstFailed of MemEntry by MemoryIndex
 */
DEM_LOCAL_INLINE void
    Dem_SetCyclesSinceFirstFailedOfMemEntry(Dem_NvBlockNumType MemoryIndex, uint16 CyclesSinceFirstFailed)
{
    Dem_GetMemoryEntry()[MemoryIndex].CyclesSinceFirstFailed = CyclesSinceFirstFailed;
}
#endif

#if ((defined(DEM_CYCLES_SINCE_LAST_FAILED)) || (defined(DEM_OCC1)))
/**
 * @brief         get CyclesSinceLastFailed of MemEntry by MemoryIndex
 */
DEM_LOCAL_INLINE uint16 Dem_GetCyclesSinceLastFailedOfMemEntry(Dem_NvBlockNumType MemoryIndex)
{
    return Dem_GetMemoryEntry()[MemoryIndex].CyclesSinceLastFailed;
}

/**
 * @brief         Set CyclesSinceLastFailed of MemEntry by MemoryIndex
 */
DEM_LOCAL_INLINE void
    Dem_SetCyclesSinceLastFailedOfMemEntry(Dem_NvBlockNumType MemoryIndex, uint16 CyclesSinceLastFailed)
{
    Dem_GetMemoryEntry()[MemoryIndex].CyclesSinceLastFailed = CyclesSinceLastFailed;
}
#endif

#if (defined(DEM_OCC2))
/**
 * @brief         get OCC2 of MemEntry by MemoryIndex
 */
DEM_LOCAL_INLINE uint8 Dem_GetOCC2OfMemEntry(Dem_NvBlockNumType MemoryIndex)
{
    return Dem_GetMemoryEntry()[MemoryIndex].OCC2;
}

/**
 * @brief         Set OCC2 of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory index
 * @param[in]     OCC2: operation cycle counter #2
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetOCC2OfMemEntry(Dem_NvBlockNumType MemoryIndex, uint8 OCC2)
{
    Dem_GetMemoryEntry()[MemoryIndex].OCC2 = OCC2;
}
#endif

#if (defined(DEM_OCC4))
/**
 * @brief         get OCC4 of MemEntry by MemoryIndex
 */
DEM_LOCAL_INLINE uint8 Dem_GetOCC4OfMemEntry(Dem_NvBlockNumType MemoryIndex)
{
    return Dem_GetMemoryEntry()[MemoryIndex].OCC4;
}

/**
 * @brief         Set OCC4 of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory index
 * @param[in]     OCC4: operation cycle counter #4
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetOCC4OfMemEntry(Dem_NvBlockNumType MemoryIndex, uint8 OCC4)
{
    Dem_GetMemoryEntry()[MemoryIndex].OCC4 = OCC4;
}
#endif

#if (defined(DEM_OCC5))
/**
 * @brief         get OCC5 of MemEntry by MemoryIndex
 */
DEM_LOCAL_INLINE uint8 Dem_GetOCC5OfMemEntry(Dem_NvBlockNumType MemoryIndex)
{
    return Dem_GetMemoryEntry()[MemoryIndex].OCC5;
}

/**
 * @brief         Set OCC5 of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory index
 * @param[in]     OCC5: operation cycle counter #5
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetOCC5OfMemEntry(Dem_NvBlockNumType MemoryIndex, uint8 OCC5)
{
    Dem_GetMemoryEntry()[MemoryIndex].OCC5 = OCC5;
}
#endif

#if (defined(DEM_OCC6))
/**
 * @brief         get OCC6 of MemEntry by MemoryIndex
 */
DEM_LOCAL_INLINE uint8 Dem_GetOCC6OfMemEntry(Dem_NvBlockNumType MemoryIndex)
{
    return Dem_GetMemoryEntry()[MemoryIndex].OCC6;
}

/**
 * @brief         Set OCC6 of MemEntry by MemoryIndex
 */
DEM_LOCAL_INLINE void Dem_SetOCC6OfMemEntry(Dem_NvBlockNumType MemoryIndex, uint8 OCC6)
{
    Dem_GetMemoryEntry()[MemoryIndex].OCC6 = OCC6;
}
#endif

#if (defined(DEM_SI30))
/**
 * @brief         get SI30 of MemEntry by MemoryIndex
 */
DEM_LOCAL_INLINE uint8 Dem_GetSI30FromMemEntry(Dem_NvBlockNumType MemoryIndex)
{
    return Dem_GetMemoryEntry()[MemoryIndex].SI30;
}

/**
 * @brief         Set SI30 of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory index
 * @param[in]     SI30: Status indicator 30
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetSI30FromMemEntry(Dem_NvBlockNumType MemoryIndex, uint8 SI30)
{
    Dem_GetMemoryEntry()[MemoryIndex].SI30 = SI30;
}

/**
 * @brief         Reset SI30 bit 0 of MemEntry by MemoryIndex
 */
DEM_LOCAL_INLINE void Dem_ResetSI30Bit0FromMemEntry(Dem_NvBlockNumType MemoryIndex)
{
    uint8 lSI30 = Dem_GetSI30FromMemEntry(MemoryIndex);
    /* PRQA S 3473, 4399, 4461 ++ */ /* VL_Dem_3473, VL_Dem_4399, VL_Dem_4461 */
    DEM_FLAG_RESET(lSI30, DEM_SI30_STATUS_UC);
    /* PRQA S 3473, 4399, 4461 -- */
    Dem_SetSI30FromMemEntry(MemoryIndex, lSI30);
}

/**
 * @brief         Set SI30 bit 5 of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetSI30Bit5OfMemEntry(Dem_NvBlockNumType MemoryIndex)
{
    uint8 lSI30 = Dem_GetSI30FromMemEntry(MemoryIndex);
    DEM_FLAG_SET(lSI30, DEM_SI30_STATUS_WIRSLC); /* PRQA S 3473 */ /* VL_Dem_3473 */
    Dem_SetSI30FromMemEntry(MemoryIndex, lSI30);
}

/**
 * @brief         Reset SI30 bit 1 of MemEntry by MemoryIndex
 * @param[in]     EventId: Event Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_ResetSI30Bit1OfMemEntry(Dem_EventIdType EventId)
{
    Dem_DTCAttRefNumType lDTCAttr = Dem_GetDTCAttr(EventId);
    if (Dem_CheckDTCAttr(lDTCAttr) == TRUE)
    {
        Dem_MemoryNumType            memRef     = Dem_GetMemRef(lDTCAttr);
        Dem_MemStateInfoConstPtrType memoryInfo = Dem_MemStateInfoInit(memRef);
        if (Dem_GetEventstored(EventId) == TRUE)
        {
            Dem_NvBlockNumType memoryIndex = Dem_MemoryFindIndex(memoryInfo, EventId);
            uint8              SI30        = Dem_GetSI30FromMemEntry(memoryIndex);
            /* PRQA S 3473, 4399, 4461 ++ */ /* VL_Dem_3473, VL_Dem_4399, VL_Dem_4461 */
            DEM_FLAG_RESET(SI30, DEM_SI30_STATUS_UCTOC);
            /* PRQA S 3473, 4399, 4461 -- */
            Dem_SetSI30FromMemEntry(memoryIndex, SI30);
        }
    }
}
#if (DEM_OBD_DTC_NUMBER > 0)
/**
 * @brief         Update SI30 bit 6 of MemEntry by MemoryIndex
 */
DEM_LOCAL_INLINE void Dem_UpdateSI30BitOfMemEntry(Dem_NvBlockNumType MemoryIndex, Dem_EventIdType EventId)
{
    uint8 lSI30 = Dem_GetSI30FromMemEntry(MemoryIndex);
    if (Dem_GetObdDTCRefOfEvent(EventId) == DEM_OBD_DTC_NUMBER)
    {
        DEM_FLAG_SET(lSI30, DEM_SI30_STATUS_ER); /* PRQA S 3473 */ /* VL_Dem_3473 */
    }
    else
    {
        /* PRQA S 3473, 4399, 4461 ++ */ /* VL_Dem_3473, VL_Dem_4399, VL_Dem_4461 */
        DEM_FLAG_RESET(lSI30, DEM_SI30_STATUS_ER);
        /* PRQA S 3473, 4399, 4461 -- */
    }
    /** JL 3.0 SymptomSinceLastClear, condition not know. will do in here */
    Dem_SetSI30FromMemEntry(MemoryIndex, lSI30);
}
#endif
#endif

/* PRQA S 5016 ++ */ /* VL_Dem_5016 */
#if (DEM_EXTENDED_DATA_CLASS_NUMBER > 0u)
/**
 * @brief         get Extended Data Ptr of MemEntry by MemoryIndex
 */
DEM_LOCAL_INLINE uint8* Dem_GetExtendedDataPtrOfMemEntry(Dem_NvBlockNumType MemoryIndex, uint8 EDEntryIndex)
{
#if (DEM_MAX_SIZE_EXTENDED_DATA > 0u)
    return &(Dem_GetMemoryEntry()[MemoryIndex].ExtendedData[EDEntryIndex][0u]);
#else
    DEM_PARAM_UNUSED(MemoryIndex);
    DEM_PARAM_UNUSED(EDEntryIndex);
    return NULL_PTR;
#endif
}

#if (DEM_MAX_SIZE_EXTENDED_DATA > 0u)
/**
 * @brief         get Extended Data Ptr of MemEntry by MemoryIndex and EntryIndex
 */
DEM_LOCAL_INLINE uint8*
    Dem_GetExtendedDataPtrOfMemEntryByEntryIndex(Dem_NvBlockNumType MemoryIndex, uint8 EDEntryIndex, uint8 EntryIndex)
{
    return &(Dem_GetMemoryEntry()[MemoryIndex].ExtendedData[EDEntryIndex][EntryIndex]);
}
#endif

/**
 * @brief         get Extended Data Record Ptr of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory index
 * @return        uint8*
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8* Dem_GetExtendedDataRecordPtrOfMemEntry(Dem_NvBlockNumType MemoryIndex)
{
    return Dem_GetMemoryEntry()[MemoryIndex].EDRecord;
}
#endif

#if (DEM_MAX_SIZE_FREEZEFRAME > 0u)
/**
 * @brief         get Freeze Frame Ptr of MemEntry by MemoryIndex
 */
DEM_LOCAL_INLINE uint8* Dem_GetFreezeFramePtrOfMemEntry(Dem_NvBlockNumType MemoryIndex, uint8 FFEntryIndex)
{
    return &(Dem_GetMemoryEntry()[MemoryIndex].FreezeFrame[FFEntryIndex][0u]);
}

/**
 * @brief         get Freeze Frame Record Ptr of MemEntry by MemoryIndex
 */
DEM_LOCAL_INLINE uint8* Dem_GetFreezeFrameRecordPtrOfMemEntry(Dem_NvBlockNumType MemoryIndex)
{
    return Dem_GetMemoryEntry()[MemoryIndex].FFRecord;
}
#endif

#if (DEM_MAX_SIZE_OBDFREEZEFRAME > 0u)
/**
 * @brief         get OBD Freeze Frame Ptr of MemEntry by MemoryIndex
 */
DEM_LOCAL_INLINE uint8* Dem_GetOBDFreezeFramePtrOfMemEntry(Dem_NvBlockNumType MemoryIndex)
{
    return Dem_GetMemoryEntry()[MemoryIndex].OBDFreezeFrame;
}
#endif

#if (DEM_MAX_SIZE_J1939FREEZEFRAME > 0u)
/**
 * @brief         get J1939 Freeze Frame Ptr of MemEntry by MemoryIndex
 */
DEM_LOCAL_INLINE uint8* Dem_GetJ1939FreezeFramePtrOfMemEntry(Dem_NvBlockNumType MemoryIndex)
{
    return Dem_GetMemoryEntry()[MemoryIndex].J1939FreezeFrame;
}
#endif

#if (DEM_MAX_SIZE_J1939EXFREEZEFRAME > 0u)
/**
 * @brief         get J1939Ex Freeze Frame Ptr of MemEntry by MemoryIndex
 */
DEM_LOCAL_INLINE uint8* Dem_GetJ1939ExFreezeFramePtrOfMemEntry(Dem_NvBlockNumType MemoryIndex)
{
    return Dem_GetMemoryEntry()[MemoryIndex].J1939ExFreezeFrame;
}
#endif

#if (                                                                            \
    (DEM_MAX_SIZE_J1939EXFREEZEFRAME > 0u) || (DEM_MAX_SIZE_OBDFREEZEFRAME > 0u) \
    || (DEM_MAX_SIZE_J1939FREEZEFRAME > 0u))
/**
 * @brief         get Freeze Frame Record ext Ptr of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory index
 * @return        uint8*
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8* Dem_GetFreezeFrameRecordExtPtrOfMemEntry(Dem_NvBlockNumType MemoryIndex)
{
    return &(Dem_GetMemoryEntry()[MemoryIndex].FFRecordExt);
}
#endif
/* PRQA S 5016 -- */

#if ((DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_OFF) || (DEM_OBDII_SUPPORT == STD_ON))
/**
 * @brief         get state of MemEntry by MemoryIndex
 */
DEM_LOCAL_INLINE uint8 Dem_GetstateOfMemEntry(Dem_NvBlockNumType MemoryIndex)
{
    return Dem_GetMemoryEntry()[MemoryIndex].State;
}
#endif

#if (DEM_OBDII_SUPPORT == STD_ON)
/**
 * @brief         Set state of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory index
 * @param[in]     State: Memory entry state
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetstateOfMemEntry(Dem_NvBlockNumType MemoryIndex, uint8 State)
{
    Dem_GetMemoryEntry()[MemoryIndex].State = State;
}
#endif

#if (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_OFF)
/**
 * @brief         get aging only state of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory index
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_GetAgingOnlyOfMemEntry(Dem_NvBlockNumType MemoryIndex)
{
    return DEM_FLAG_ISSET(Dem_GetMemoryEntry()[MemoryIndex].State, DEM_MEM_STATE_AGING_ONLY);
}

/**
 * @brief         Set aging only state of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetAgingOnlyOfMemEntry(Dem_NvBlockNumType MemoryIndex)
{
    DEM_FLAG_SET(Dem_GetMemoryEntry()[MemoryIndex].State, DEM_MEM_STATE_AGING_ONLY);
}
#endif

/**
 * @brief         Initializes an event memory entry.
 * @param[in]     MemoryIndex: Memory index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_MemInitializeEntry(Dem_NvBlockNumType MemoryIndex)
{
#if (defined(DEM_OCCCTR))
    Dem_SetOCCOfMemEntry(MemoryIndex, 0u);
#endif
    Dem_SetAgingCounterOfMemEntry(MemoryIndex, DEM_OPERATIONCYCLE_CYCLECOUNT_INVALID);
#if (defined(DEM_MAX_FDC_SINCE_LAST_CLEAR))
    Dem_SetMaxDebounceValueOfMemEntry(MemoryIndex, 0);
#endif
#if (defined(DEM_FAILED_CYCLES))
    Dem_SetFailedCycleCounterOfMemEntry(MemoryIndex, 0u);
#endif
#if ((defined(DEM_CYCLES_SINCE_FIRST_FAILED)) || (defined(DEM_OCC3)))
    Dem_SetCyclesSinceFirstFailedOfMemEntry(MemoryIndex, DEM_OPERATIONCYCLE_CYCLECOUNT_INVALID);
#endif
#if ((defined(DEM_CYCLES_SINCE_LAST_FAILED)) || (defined(DEM_OCC1)))
    Dem_SetCyclesSinceLastFailedOfMemEntry(MemoryIndex, DEM_OPERATIONCYCLE_CYCLECOUNT_INVALID);
#endif
#if (defined(DEM_OCC2))
    Dem_SetOCC2OfMemEntry(MemoryIndex, 0u);
#endif
#if (defined(DEM_OCC4))
    Dem_SetOCC4OfMemEntry(MemoryIndex, 0u);
#endif
#if (defined(DEM_OCC5))
    Dem_SetOCC5OfMemEntry(MemoryIndex, 0u);
#endif
#if (defined(DEM_OCC6))
    Dem_SetOCC6OfMemEntry(MemoryIndex, 0u);
#endif
#if (defined(DEM_SI30))
    Dem_SetSI30FromMemEntry(MemoryIndex, DEM_SI30_STATUS_DEFALUT);
#endif
#if (DEM_EXTENDED_DATA_CLASS_NUMBER > 0u)
    Dem_MemSet(Dem_GetExtendedDataRecordPtrOfMemEntry(MemoryIndex), 0u, DEM_MAX_NUMBER_EXTENDED_DATA_RECORD);
#endif
#if (DEM_FREEZE_FRAME_REC_NUM_CLASS_NUMBER > 0u)
    Dem_MemSet(Dem_GetFreezeFrameRecordPtrOfMemEntry(MemoryIndex), 0u, DEM_MAX_NUMBER_FREEZEFRAMERECORD);
#endif
#if (                                                                            \
    (DEM_MAX_SIZE_J1939EXFREEZEFRAME > 0u) || (DEM_MAX_SIZE_OBDFREEZEFRAME > 0u) \
    || (DEM_MAX_SIZE_J1939FREEZEFRAME > 0u))
    Dem_MemSet(Dem_GetFreezeFrameRecordExtPtrOfMemEntry(MemoryIndex), 0u, 1u);
#endif
}

/**
 * @brief Find Index in Memory by EventId
 */
DEM_LOCAL Dem_NvBlockNumType Dem_MemoryFindIndex(Dem_MemStateInfoConstPtrType MemoryInfo, Dem_EventIdType EventId)
{
    Dem_NvBlockNumType memoryIndex;
    Dem_NvBlockNumType memoryIndexEnd;
#if (DEM_EVENT_COMBINATION_SUPPORT == DEM_EVCOMB_ONSTORAGE)
    Dem_CombinationNumType groupId = Dem_GetGroupIdOfEvent(EventId);
#endif

    memoryIndex    = Dem_GetMemEntryStartOfMemoryInfo(MemoryInfo);
    memoryIndexEnd = Dem_GetMemEntryEndOfMemoryInfo(MemoryInfo);
    while ((memoryIndex < memoryIndexEnd)
           && ((Dem_GetEventIdOfMemEntry(memoryIndex) != EventId)
#if (DEM_EVENT_COMBINATION_SUPPORT == DEM_EVCOMB_ONSTORAGE)
               || (groupId != Dem_GetGroupIdOfEvent(Dem_GetEventIdOfMemEntry(memoryIndex)))
#endif
                   ))
    {
        ++memoryIndex;
    }
    /** if not found, return DEM_MEM_INVALID_MEMORY_INDEX */
    if (memoryIndex >= memoryIndexEnd)
    {
        memoryIndex = DEM_MEM_INVALID_MEMORY_INDEX;
    }
    return memoryIndex;
}

/**
 * @brief Frees an event memory entry
 */
DEM_LOCAL void Dem_MemoryFreeIndex(Dem_MemStateInfoConstPtrType MemoryInfo, Dem_NvBlockNumType MemoryIndex)
{
    uint8              chronoIterator;
    Dem_NvBlockNumType tempChronoIndex;

    /** reduce count of used elements in the chrono stack */
    chronoIterator = Dem_GetCurSizeOfMemoryInfo(MemoryInfo);
    --chronoIterator;
    Dem_SetCurSizeOfMemoryInfo(MemoryInfo, chronoIterator);

    /** free the primary element */
    Dem_SetEventIdOfMemEntry(MemoryIndex, DEM_EVENT_INVALID);
    Dem_SetTimestampOfMemEntry(MemoryIndex, 0u);

    /** go backwards through the chronological list and move down all entries, until the deleted element is
     * overwritten
     */
    tempChronoIndex = Dem_GetChronologyOfMemoryInfo(MemoryInfo, chronoIterator);
    while (tempChronoIndex != MemoryIndex)
    {
        Dem_NvBlockNumType swap;
        swap = tempChronoIndex;
        --chronoIterator;
        tempChronoIndex = Dem_GetChronologyOfMemoryInfo(MemoryInfo, chronoIterator);
        Dem_SetChronologyOfMemoryInfo(MemoryInfo, chronoIterator, swap);
    }
}

#if (DEM_GENERAL_DISPLACEMENT_NONE == STD_OFF)

/**
 * @brief         Update the best match info by the candidate info.
 * @param[in]     BestMatchInfo: Pointer to match information
 * @param[in]     CandidateInfo: Pointer to candidate information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_MemDisplaceUpdateBestMatch(
    Dem_MemDisplacementInfoType* const       BestMatchInfo,
    const Dem_MemDisplacementInfoType* const CandidateInfo)
{
    *BestMatchInfo = *CandidateInfo;
}

/**
 * @brief         Check if candidate is the invalid entry. If so, it is the best match.
 * @param[in]     BestMatchInfo: Pointer to match information
 * @param[in]     CandidateInfo: Pointer to candidate information
 * @return        Dem_MemDisplaceCheckResultType
 * @retval        Dem_MemDisplace_NeedMoreTests: Candidate is not classified yet, apply further tests on this candidate
 * @retval        Dem_MemDisplace_RatingDone: Candidate is classified, continue the scan with next candidate
 * @retval        Dem_MemDisplace_FoundBestMatch: Candidate is the best match, abort scanning for better matches
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_MemDisplaceCheckResultType Dem_MemDisplaceCheckInvalidEntry(
    Dem_MemDisplacementInfoType* const       BestMatchInfo,
    const Dem_MemDisplacementInfoType* const CandidateInfo)
{
    Dem_MemDisplaceCheckResultType checkResult = Dem_MemDisplace_NeedMoreTests;

    if (Dem_CheckEventIdInValid(CandidateInfo->EventId) == TRUE)
    {
        /** always replace an invalid memory entry */
        Dem_MemDisplaceUpdateBestMatch(BestMatchInfo, CandidateInfo);
        checkResult = Dem_MemDisplace_FoundBestMatch;
    }

    return checkResult;
}

/**
 * @brief Check if candidate has an event priority, that qualifies it or disqualifies it for the displacement. Otherwise
 * it needs further tests.
 */
DEM_LOCAL Dem_MemDisplaceCheckResultType Dem_MemDisplaceCheckPriority(
    Dem_MemDisplacementInfoType* const       BestMatchInfo,
    const Dem_MemDisplacementInfoType* const CandidateInfo)
{
    Dem_MemDisplaceCheckResultType checkResult;

    if (Dem_GetEventPriority(CandidateInfo->EventId) < Dem_GetEventPriority(BestMatchInfo->EventId))
    {
        /** Never displace events with higher priority */
        checkResult = Dem_MemDisplace_RatingDone;
    }
    else if (Dem_GetEventPriority(CandidateInfo->EventId) > Dem_GetEventPriority(BestMatchInfo->EventId))
    {
        /** always prefer an event with lowest priority */
        Dem_MemDisplaceUpdateBestMatch(BestMatchInfo, CandidateInfo);
        checkResult = Dem_MemDisplace_RatingDone;
    }
    else
    {
        /** No candidate identified */
        checkResult = Dem_MemDisplace_NeedMoreTests;
    }
    return checkResult;
}

#if (DEM_GENERAL_DISPLACEMENT_FULL == STD_ON)
/**
 * @brief Check if candidate is passive, and this qualifies it for the displacement. Otherwise it needs further tests.
 */
DEM_LOCAL Dem_MemDisplaceCheckResultType Dem_MemDisplaceCheckPassiveState(
    Dem_MemDisplacementInfoType* const       BestMatchInfo,
    const Dem_MemDisplacementInfoType* const CandidateInfo)
{
    Dem_MemDisplaceCheckResultType checkResult;
    if (Dem_UDSStatusCheckTF(CandidateInfo->DtcStatus) == FALSE)
    {
        /** did not find a passive event before, so candidate is a better match */
        Dem_MemDisplaceUpdateBestMatch(BestMatchInfo, CandidateInfo);
        checkResult = Dem_MemDisplace_RatingDone;
    }
    else
    {
        /** no candidate identified */
        checkResult = Dem_MemDisplace_NeedMoreTests;
    }
    return checkResult;
}

/**
 * @brief Check if candidate is passive, and this qualifies it for the displacement. Otherwise it needs further tests.
 */
DEM_LOCAL Dem_MemDisplaceCheckResultType Dem_MemDisplaceCheckReadinessState(
    Dem_MemDisplacementInfoType* const       BestMatchInfo,
    const Dem_MemDisplacementInfoType* const CandidateInfo)
{
    Dem_MemDisplaceCheckResultType checkResult;

    if ((Dem_UDSStatusCheckTNCTOC(BestMatchInfo->DtcStatus) == FALSE)
        && (Dem_UDSStatusCheckTNCTOC(CandidateInfo->DtcStatus) == TRUE))
    {
        Dem_MemDisplaceUpdateBestMatch(BestMatchInfo, CandidateInfo);
        checkResult = Dem_MemDisplace_RatingDone;
    }
    else
    {
        /** no candidate identified */
        checkResult = Dem_MemDisplace_NeedMoreTests;
    }

    return checkResult;
}
#endif

/**
 * @brief Selects an event to displace in a filled memory
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
DEM_LOCAL Dem_NvBlockNumType Dem_MemSelectDisplacedIndex(
    Dem_MemStateInfoConstPtrType      MemoryInfo,
    Dem_EventIdType                   DisplacingEventId,
    Dem_EventDisplacementStrategyType displacementStrategy)
/* PRQA S 6070 -- */
{
    Dem_MemDisplacementInfoType    bestMatchInfo;
    Dem_MemDisplacementInfoType    candidateInfo;
    Dem_MemDisplaceCheckResultType checkResult;

    /** Initialize Best Candidate to 'displacement not possible' */
    bestMatchInfo.EventId     = DisplacingEventId;
    bestMatchInfo.MemoryIndex = DEM_MEM_INVALID_MEMORY_INDEX;
    bestMatchInfo.DtcStatus   = DEM_UDS_STATUS_TF;

    /** find lowest priority DTC in memory. scan in chronological order, so the first found entry is the older one */
    /* PRQA S 0771 ++ */ /* VL_Dem_0771 */
    for (uint8 chronoIndex = 0u; chronoIndex < Dem_GetCurSizeOfMemoryInfo(MemoryInfo); ++chronoIndex)
    /* PRQA S 0771 -- */
    {
        /** Get next Candidate to be checked */
        candidateInfo.MemoryIndex = Dem_GetChronologyOfMemoryInfo(MemoryInfo, chronoIndex);
        candidateInfo.EventId     = Dem_GetEventIdOfMemEntry(candidateInfo.MemoryIndex);
        candidateInfo.DtcStatus   = Dem_CheckCombineUdsStatus(Dem_GetDTCUDSStatus(candidateInfo.EventId));

        /** Check: Candidate is invalid entry */
        if (Dem_MemDisplaceCheckInvalidEntry(&bestMatchInfo, &candidateInfo) == Dem_MemDisplace_FoundBestMatch)
        {
            /** always replace an invalid memory entry */
            break;
        }

        /** Check: event priorities */
        checkResult = Dem_MemDisplaceCheckPriority(&bestMatchInfo, &candidateInfo);
        if (checkResult == Dem_MemDisplace_RatingDone)
        {
            continue; /* PRQA S 0770 */ /* VL_Dem_0770 */
        }
        else if (checkResult == Dem_MemDisplace_FoundBestMatch)
        {
            break;
        }
        else
        {
            /** check next category */
        }

        /** Check: consider passive for displacement */
#if (DEM_GENERAL_DISPLACEMENT_FULL == STD_ON)
        if (displacementStrategy == DEM_DISPLACEMENT_FULL)
        {
            checkResult = Dem_MemDisplaceCheckPassiveState(&bestMatchInfo, &candidateInfo);
            if (checkResult == Dem_MemDisplace_RatingDone)
            {
                continue; /* PRQA S 0770 */ /* VL_Dem_0770 */
            }
            else if (checkResult == Dem_MemDisplace_FoundBestMatch)
            {
                break;
            }
            else
            {
                /** check next category */
            }

            /** Check: consider readiness for displacement */
            if (Dem_MemDisplaceCheckReadinessState(&bestMatchInfo, &candidateInfo) == Dem_MemDisplace_RatingDone)
            {
                continue; /* PRQA S 0770 */ /* VL_Dem_0770 */
            }
        }
#endif
    }
    return bestMatchInfo.MemoryIndex;
}
#endif

/**
 * @brief This function manages the event memory storage. If a free slot is available, it is returned directly. In case
 * no free slot is available, the configured displacement algorithm is applied to free a slot.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
DEM_LOCAL Dem_NvBlockNumType Dem_MemoryAllocateIndex(Dem_MemStateInfoConstPtrType MemoryInfo, Dem_EventIdType EventId)
/* PRQA S 6070 -- */
{
    Dem_NvBlockNumType memoryIndex;
    if (Dem_GetMaxSizeOfMemoryInfo(MemoryInfo) == Dem_GetCurSizeOfMemoryInfo(MemoryInfo))
    {
        Dem_MemoryNumType memRef = Dem_GetMemRef(Dem_GetDTCAttr(EventId));
#if (DEM_GENERAL_DISPLACEMENT_NONE == STD_OFF)
        Dem_EventDisplacementStrategyType displacementStrategy = Dem_GetDisplacementOfMemory(memRef);
#endif
        /** Memory is filled up */
        Dem_SetMemoryOverflow(memRef);
#if (DEM_GENERAL_DISPLACEMENT_NONE == STD_OFF)
#if (DEM_GENERAL_DISPLACEMENT_HAVE_NONE == STD_ON)
        if (displacementStrategy != DEM_DISPLACEMENT_NONE)
        {
#endif
            memoryIndex = Dem_MemSelectDisplacedIndex(MemoryInfo, EventId, displacementStrategy);
#if (DEM_GENERAL_DISPLACEMENT_HAVE_NONE == STD_ON)
        }
        else
        {
            memoryIndex = DEM_MEM_INVALID_MEMORY_INDEX; /** Always failed */
        }
#endif
#else
        memoryIndex = DEM_MEM_INVALID_MEMORY_INDEX; /** Always failed */
#endif
#if (DEM_GENERAL_DISPLACEMENT_NONE == STD_OFF)
        if (memoryIndex != DEM_MEM_INVALID_MEMORY_INDEX)
        {
            /** a displaced DTC was identified, now remove that DTC */
            Dem_EventIdType displacedEventId = Dem_GetEventIdOfMemEntry(memoryIndex);
            if (Dem_CheckEventIdInValid(displacedEventId) == FALSE)
            {
                Dem_DTCDisplaced(displacedEventId);
            }
            Dem_MemoryFreeIndex(MemoryInfo, memoryIndex);
        }
        /** else: no adequate DTC found, the new entry cannot be added - return an invalid index */
#endif
    }
    else
    {
        /** Memory is not full -> Find first free entry */
        memoryIndex = Dem_MemoryFindIndex(MemoryInfo, DEM_EVENT_INVALID);
        /** if not found : memoryIndex == DEM_MEM_INVALID_MEMORY_INDEX */
    }

    if (memoryIndex != DEM_MEM_INVALID_MEMORY_INDEX)
    {
        uint8 memorySize = Dem_GetCurSizeOfMemoryInfo(MemoryInfo);
        /** fill the chrono record with event id and lock the referenced PriMem slot */
        Dem_SetChronologyOfMemoryInfo(MemoryInfo, memorySize, memoryIndex);
        Dem_SetEventIdOfMemEntry(memoryIndex, EventId);
        Dem_SetTimestampOfMemEntry(memoryIndex, Dem_MemGetCurrentTimestamp());
        Dem_MemInitializeEntry(memoryIndex);
#if ((defined(DEM_SI30)) && (DEM_OBD_DTC_NUMBER > 0))
        Dem_UpdateSI30BitOfMemEntry(memoryIndex, EventId);
#endif
        /** need to clear mementry data? eventid invalid means data invalid */
        Dem_MemSetCurrentTimestamp(Dem_MemGetCurrentTimestamp() + 1u); /* PRQA S 1252 */ /* VL_Dem_1252 */
        Dem_SetCurSizeOfMemoryInfo(MemoryInfo, (memorySize + 1u));
    }
    return memoryIndex;
}

/**
 * @brief Selects a memory entry for an event
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
DEM_LOCAL Dem_NvBlockNumType Dem_AllocateMemory(Dem_EventIdType EventId)
/* PRQA S 6070 -- */
{
    Dem_NvBlockNumType   memoryIndex;
    Dem_DTCAttRefNumType lDTCAttr = Dem_GetDTCAttr(EventId);
    if (Dem_CheckDTCAttr(lDTCAttr) == TRUE)
    {
        Dem_MemStateInfoConstPtrType memoryInfo = Dem_MemStateInfoInit(Dem_GetMemRef(lDTCAttr));
        memoryIndex                             = Dem_MemoryAllocateIndex(memoryInfo, EventId);
        if (memoryIndex != DEM_MEM_INVALID_MEMORY_INDEX)
        {
            /** Created new entry */
            /** Enter Critical Section: DiagMonitor */
            Dem_EnterCritical_DiagMonitor();
            Dem_EventInternalStatusType internalStatus = Dem_GetDtcInterStatus(EventId);
            internalStatus = Dem_SetEventstoredStatus(internalStatus, Dem_InternalStoredStatus_Active);
            Dem_SetDtcInterStatus(EventId, internalStatus);
            Dem_LeaveCritical_DiagMonitor();
            /**  Leave Critical Section: DiagMonitor */

#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
#if (DEM_FEATURE_NVM_IMMEDIATE == STD_ON)
            if (Dem_GetEventDTCNvImmediate(EventId) == TRUE)
            {
                Dem_NvmSetSingleBlockState(memoryIndex, DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
            }
            else
#endif
            {
                Dem_NvmSetSingleBlockState(memoryIndex, DEM_NVM_BLOCKSTATE_DIRTY);
            }
#endif
        }
    }
    return memoryIndex;
}

#if (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_OFF)
/**
 * @brief Selects a memory entry for an event's aging counter
 */
DEM_LOCAL Dem_NvBlockNumType Dem_MemoryAllocateAging(Dem_MemStateInfoConstPtrType MemoryInfo, Dem_EventIdType EventId)
{
    Dem_NvBlockNumType memoryIndex;
    /** Find empty slot if possible, otherwise remove aged slot */
    if (Dem_GetMaxSizeOfMemoryInfo(MemoryInfo) == Dem_GetCurSizeOfMemoryInfo(MemoryInfo))
    {
        /** Memory is filled up - Aged environmental data is not displaced, the remaining slots (if any) are still
         * used
         * -> no chance to allocate an aging counter */
        memoryIndex = DEM_MEM_INVALID_MEMORY_INDEX;
    }
    else
    {
        /** Memory is not full -> Find first free entry */
        memoryIndex = Dem_MemoryFindIndex(MemoryInfo, DEM_EVENT_INVALID);
        /** if not found : memoryIndex == DEM_MEM_INVALID_MEMORY_INDEX */
    }

    if (memoryIndex != DEM_MEM_INVALID_MEMORY_INDEX)
    {
        uint8 memorySize = Dem_GetCurSizeOfMemoryInfo(MemoryInfo);
        /** fill the chrono record with event id and lock the referenced PriMem slot */
        Dem_SetTimestampOfMemEntry(memoryIndex, Dem_MemGetCurrentTimestamp());
        Dem_SetChronologyOfMemoryInfo(MemoryInfo, memorySize, memoryIndex);
        Dem_SetEventIdOfMemEntry(memoryIndex, EventId);
        /** Data */
        Dem_MemInitializeEntry(memoryIndex);
#if ((defined(DEM_SI30)) && (DEM_OBD_DTC_NUMBER > 0))
        Dem_UpdateSI30BitOfMemEntry(memoryIndex, EventId);
#endif
        Dem_SetAgingOnlyOfMemEntry(memoryIndex);
        Dem_MemSetCurrentTimestamp(Dem_MemGetCurrentTimestamp() + 1u);
        Dem_SetCurSizeOfMemoryInfo(MemoryInfo, (memorySize + 1u));
    }
    return memoryIndex;
}
#endif

#if (defined(DEM_OCCCTR))
/**
 * @brief         Increment the occurrence counter in the memory entry, if it is lower than the maximum value (latch at
 * maximum value).
 */
DEM_LOCAL_INLINE boolean Dem_MemIncrementOCC(Dem_NvBlockNumType MemoryIndex)
{
    boolean successfulIncrement = FALSE;
    uint8   occurrenceCounter   = Dem_GetOCCOfMemEntry(MemoryIndex);

    if (occurrenceCounter < DEM_DATA_OCCURRENCE_MAX)
    {
        ++occurrenceCounter;
        Dem_SetOCCOfMemEntry(MemoryIndex, occurrenceCounter);
        successfulIncrement = TRUE;
    }
    return successfulIncrement;
}
#endif

#if (defined(DEM_OCC2))
/**
 * @brief         Increment the occurrence counter # 2 in the memory entry, if it is lower than the maximum value (latch
 * at maximum value).
 */
DEM_LOCAL_INLINE boolean Dem_MemIncrementOCC2(Dem_NvBlockNumType MemoryIndex)
{
    boolean successfulIncrement = FALSE;
    uint8   lOCC2               = Dem_GetOCC2OfMemEntry(MemoryIndex);

    if (lOCC2 < DEM_DATA_OCCURRENCE_MAX)
    {
        ++lOCC2;
        Dem_SetOCC2OfMemEntry(MemoryIndex, lOCC2);
        successfulIncrement = TRUE;
    }
    return successfulIncrement;
}
#endif

#if (defined(DEM_OCC4))
/**
 * @brief         Increment the occurrence counter # 4 in the memory entry, if it is lower than the maximum value (latch
 * at maximum value).
 * @param[in]     MemoryIndex: Memory index
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_MemIncrementOCC4(Dem_NvBlockNumType MemoryIndex)
{
    boolean successfulIncrement = FALSE;
    uint8   lOCC4               = Dem_GetOCC4OfMemEntry(MemoryIndex);

    if (lOCC4 < DEM_DATA_OCCURRENCE_MAX)
    {
        ++lOCC4;
        Dem_SetOCC4OfMemEntry(MemoryIndex, lOCC4);
        successfulIncrement = TRUE;
    }
    return successfulIncrement;
}
#endif

#if (defined(DEM_OCC6))
/**
 * @brief         Increment the occurrence counter # 6 in the memory entry, if it is lower than the maximum value (latch
 * at maximum value).
 * @param[in]     MemoryIndex: Memory index
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_MemIncrementOCC6(Dem_NvBlockNumType MemoryIndex)
{
    boolean successfulIncrement = FALSE;
    uint8   lOCC6               = Dem_GetOCC6OfMemEntry(MemoryIndex);

    if (lOCC6 < DEM_DATA_OCCURRENCE_MAX)
    {
        ++lOCC6;
        Dem_SetOCC6OfMemEntry(MemoryIndex, lOCC6);
        successfulIncrement = TRUE;
    }
    return successfulIncrement;
}
#endif

#if (defined(DEM_FAILED_CYCLES))
/**
 * @brief         Increment the 'Failed Cycle Counter' in the memory entry, if it is lower than the maximum value (latch
 * at maximum value).
 */
DEM_LOCAL_INLINE boolean Dem_MemIncrementFailedCycleCounter(Dem_NvBlockNumType MemoryIndex)
{
    boolean successfulIncrement = FALSE;
    uint8   failedCycleCounter  = Dem_GetFailedCycleCounterOfMemEntry(MemoryIndex);

    if (failedCycleCounter < DEM_DATA_FAILED_CYCLE_COUNTER_MAX)
    {
        ++failedCycleCounter;
        Dem_SetFailedCycleCounterOfMemEntry(MemoryIndex, failedCycleCounter);
        successfulIncrement = TRUE;
    }
    return successfulIncrement;
}
#endif

/**

 * @brief         Function updates environmental data of an event on a Test Failed trigger.
 * @param[in]     MemoryIndex: Memory index
 * @param[in]     EventId: Event Id
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_MemUpdateEntryOnTF(Dem_EventIdType EventId, Dem_NvBlockNumType MemoryIndex)
{
    uint8 blockModification = DEM_DATA_NO_CHANGES;

    Dem_SetAgingCounterOfMemEntry(MemoryIndex, DEM_OPERATIONCYCLE_CYCLECOUNT_INVALID);
    DEM_FLAG_SET(blockModification, DEM_DATA_STATISTICS_CHANGED); /* PRQA S 3473 */ /* VL_Dem_3473 */

#if ((defined(DEM_CYCLES_SINCE_FIRST_FAILED)) || (defined(DEM_OCC3)))
    if (Dem_GetCyclesSinceFirstFailedOfMemEntry(MemoryIndex) == DEM_OPERATIONCYCLE_CYCLECOUNT_INVALID)
    {
        Dem_SetCyclesSinceFirstFailedOfMemEntry(MemoryIndex, Dem_GetCycleCounter(Dem_GetOCRefOfEvent(EventId)));
        DEM_FLAG_SET(blockModification, DEM_DATA_STATISTICS_CHANGED); /* PRQA S 3473 */ /* VL_Dem_3473 */
    }
#else
    DEM_PARAM_UNUSED(EventId);
#endif

#if (defined(DEM_MAX_FDC_SINCE_LAST_CLEAR))
    Dem_SetMaxDebounceValueOfMemEntry(MemoryIndex, DEM_ENTRY_MAX_FDC_VALUE);
    DEM_FLAG_SET(blockModification, DEM_DATA_STATISTICS_CHANGED); /* PRQA S 3473 */ /* VL_Dem_3473 */
#endif
    return blockModification;
}

/**
 * @brief         Function updates environmental data of an event on a Test Failed this cycle trigger.
 */
DEM_LOCAL uint8 Dem_MemUpdateEntryOnTFTOC(Dem_EventIdType EventId, Dem_NvBlockNumType MemoryIndex)
{
    uint8 blockModification = DEM_DATA_NO_CHANGES;
#if ((defined(DEM_CYCLES_SINCE_LAST_FAILED)) || (defined(DEM_OCC1)))
    Dem_SetCyclesSinceLastFailedOfMemEntry(MemoryIndex, Dem_GetCycleCounter(Dem_GetOCRefOfEvent(EventId)));
    DEM_FLAG_SET(blockModification, DEM_DATA_STATISTICS_CHANGED); /* PRQA S 3473 */ /* VL_Dem_3473 */
#else
    DEM_PARAM_UNUSED(EventId);
    DEM_PARAM_UNUSED(MemoryIndex);
#endif
    return blockModification;
}

#if (DEM_FREEZE_FRAME_CLASS_NUMBER > 0u)
/**
 * @brief         Check if freeze frame is Stored
 */
DEM_LOCAL_INLINE boolean Dem_CheckFFStored(uint8 FFEntryIndex, Dem_NvBlockNumType MemoryIndex)
{
    const uint8* ptr = Dem_GetFreezeFrameRecordPtrOfMemEntry(MemoryIndex);
    boolean      ret = DEM_BITS_ISSET(ptr, FFEntryIndex); /* PRQA S 4404 */ /* VL_Dem_4404 */
    return ret;
}

/**
 * @brief         Sets freeze frame is Stored
 * @param[in]     FFEntryIndex: FreezeFrame Entry index
 * @param[in]     MemoryIndex: Memory index
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetFFStored(uint8 FFEntryIndex, Dem_NvBlockNumType MemoryIndex)
{
    uint8* ptr = Dem_GetFreezeFrameRecordPtrOfMemEntry(MemoryIndex);
    DEM_BITS_SET(ptr, FFEntryIndex);
}

/**
 * @brief This function will look for every snapshot record of the event that needs updating due to the status changes
 * encoded by the trigger flags.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
DEM_LOCAL uint8 Dem_MemUpdateEventFreezeFrame(Dem_EventIdType EventId, Dem_NvBlockNumType MemoryIndex, uint8 Trigger)
/* PRQA S 6070 -- */
{
    uint8 blockModification = DEM_DATA_NO_CHANGES;
#if (DEM_MAX_SIZE_FREEZEFRAME > 0u)
#if ((DEM_GENERAL_FF_RECNUM_CONFIGURED == STD_ON) && (DEM_GENERAL_FF_RECNUM_CALCULATED == STD_ON))
    Dem_MemoryNumType memRef = Dem_GetMemRef(Dem_GetDTCAttr(EventId));
#endif
    Dem_TypeOfFreezeFrameRecordNumerationType typeOfFFRecordNumeration = Dem_GetFFRecordOfMemory(memRef);
    Dem_FFEntryIterType                       lFFEntryIter;
#if (DEM_GENERAL_FF_RECNUM_CONFIGURED == STD_ON)
    uint8 lastStoredEntryIndex = DEM_MEM_INVALID_MEMORY_INDEX;
#endif
    uint8 cfgTrigger = DEM_TRIGGER_TESTFAILED;
    Dem_FFEntryIteratorInit(EventId, MemoryIndex, &lFFEntryIter);
    while (Dem_FFEntryIteratorExists(&lFFEntryIter) == TRUE)
    {
        uint8 lFFEntryIndex = lFFEntryIter.FFEntryIndex;
#if (DEM_GENERAL_FF_RECNUM_CONFIGURED == STD_ON)
        Dem_FFRRefNumType lFFRecordRef = Dem_GetValueOfDFFRRef(lFFEntryIter.FFRNStart);
        if (typeOfFFRecordNumeration == DEM_FF_RECNUM_CONFIGURED)
        {
            cfgTrigger = Dem_GetFFRTriggerOfFFRC(lFFRecordRef);
        }
#endif

        /** If an update is needed: Updates are required when the configured trigger matches,
         * and the record is either not stored or configured for updates */
        if ((DEM_FLAG_ISSET(Trigger, cfgTrigger))
            && (
#if (DEM_GENERAL_FF_RECNUM_CONFIGURED == STD_ON)
                (Dem_GetFFRUpdataOfFFRC(lFFRecordRef) == DEM_FF_UPDATE_RECORD_YES) ||
#endif
                (Dem_CheckFFStored(lFFEntryIndex, MemoryIndex) == FALSE))) /* PRQA S 3415 */ /* VL_Dem_3415 */
        {
            Dem_SetFFStored(lFFEntryIndex, MemoryIndex);
#if (DEM_GENERAL_FF_RECNUM_CONFIGURED == STD_ON)
            /** For configured snapshot numbers, if the snapshot is already collected copy its data */
            if (lastStoredEntryIndex != DEM_MEM_INVALID_MEMORY_INDEX)
            {
                if (lFFEntryIndex < DEM_MAX_NUMBER_FREEZEFRAME)
                {
                    Dem_MemCpy(
                        Dem_GetFreezeFramePtrOfMemEntry(MemoryIndex, lFFEntryIndex),
                        Dem_GetFreezeFramePtrOfMemEntry(MemoryIndex, lastStoredEntryIndex),
                        DEM_MAX_SIZE_FREEZEFRAME);
                }
            }
            else
#endif
            {
#if (DEM_MAX_NUMBER_PRESTORED_FF > 0u)
                /** If 'prestore freezeframe' is enabled and data is prestored, copy the prestored data */
                if (Dem_PrestoreApplyFreezeFrame(
                        Dem_GetFreezeFramePtrOfMemEntry(MemoryIndex, lFFEntryIndex),
                        DEM_MAX_SIZE_FREEZEFRAME)
                    == FALSE)
#endif
                {
                    /** Otherwise, collect new samples from the application. */
                    Dem_GetFFFromEnv(
                        EventId,
                        Dem_GetFreezeFramePtrOfMemEntry(MemoryIndex, lFFEntryIndex),
                        DEM_MAX_SIZE_FREEZEFRAME);
#if (DEM_GENERAL_FF_RECNUM_CONFIGURED == STD_ON)
                    lastStoredEntryIndex = lFFEntryIndex;
#endif
                }
                blockModification = DEM_DATA_USERDATA_CHANGED;
            }
#if (DEM_GENERAL_FF_RECNUM_CALCULATED == STD_ON)
            /** For calculated snapshot records, processing is completed */
            if (typeOfFFRecordNumeration == DEM_FF_RECNUM_CALCULATED)
            {
                break;
            }
#endif
        }
        Dem_FFEntryIteratorNext(&lFFEntryIter);
    }
#endif
    return blockModification;
}
#endif

#if (DEM_EXTENDED_DATA_CLASS_NUMBER > 0u)

/**
 * @brief         Check if extended data is Stored
 */
DEM_LOCAL_INLINE boolean Dem_CheckEDStored(Dem_NvBlockNumType MemoryIndex, uint8 EDEntryIndex)
{
    const uint8* ptr = Dem_GetExtendedDataRecordPtrOfMemEntry(MemoryIndex);
    boolean      ret = DEM_BITS_ISSET(ptr, EDEntryIndex); /* PRQA S 4404 */ /* VL_Dem_4404 */
    return ret;
}

/**
 * @brief         Sets extended data is Stored
 * @param[in]     EDEntryIndex: Extend data entry index ID
 * @param[in]     MemoryIndex: Memory index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetEDStored(uint8 EDEntryIndex, Dem_NvBlockNumType MemoryIndex)
{
    uint8* ptr = Dem_GetExtendedDataRecordPtrOfMemEntry(MemoryIndex);
    DEM_BITS_SET(ptr, EDEntryIndex);
}

/**
 * @brief Collects the user provided extended data records stored for an event entry.
 */
/* PRQA S 3415 ++ */ /* VL_Dem_3415 */
DEM_LOCAL uint8 Dem_MemUpdateEventExtendData(Dem_EventIdType EventId, Dem_NvBlockNumType MemoryIndex, uint8 Trigger)
{
    Dem_DTCAttRefNumType   lDTCAttr = Dem_GetDTCAttr(EventId);
    Dem_EDRefNumType       lEDRef   = Dem_GetEDRefOfDTCAttr(lDTCAttr);
    Dem_EDRecordRefNumType lEDRNStart;
    Dem_EDRecordRefNumType lEDRNEnd;
    uint8                  blockModification  = DEM_DATA_NO_CHANGES;
    uint8                  extendedEntryIndex = 0u;

    if (lEDRef != DEM_ATTRI_INVALID_EXTENDDATA)
    {
        lEDRNStart = Dem_GetEDRStartOfED(lEDRef);
        lEDRNEnd   = Dem_GetEDREndOfED(lEDRef);
    }
    else
    {
        lEDRNStart = 0u;
        lEDRNEnd   = 0u;
    }
    for (; lEDRNStart < lEDRNEnd; ++lEDRNStart)
    {
        Dem_EDRecordRefNumType cfgEDIndex = Dem_GetValueOfEDRecordRef(lEDRNStart);
        if ((Dem_GetHaveUserDataOfEDRC(cfgEDIndex) == TRUE)
            && (DEM_FLAG_ISSET(Trigger, Dem_GetEDRTriggerOfEDRC(cfgEDIndex)))
            && ((Dem_GetEDRUpdataOfEDRC(cfgEDIndex) == DEM_ED_UPDATE_RECORD_YES)
                || (Dem_CheckEDStored(MemoryIndex, extendedEntryIndex) == FALSE)))
        {
            Dem_DestinationBufferType destBufferInfo;
            Dem_DataInfoType          dataInfo;

            Dem_DataInfoInit(&dataInfo, &destBufferInfo, EventId);
            Dem_SetEDStored(extendedEntryIndex, MemoryIndex);
            Dem_DestBufferInit(
                &destBufferInfo,
                Dem_GetExtendedDataPtrOfMemEntry(MemoryIndex, extendedEntryIndex),
                DEM_MAX_SIZE_EXTENDED_DATA);
            Dem_GetEnvDataByEDR(&dataInfo, cfgEDIndex);
            blockModification = DEM_DATA_USERDATA_CHANGED;
        }
        extendedEntryIndex++;
    }

    return blockModification;
}
/* PRQA S 3415 -- */
#endif

#if ((DEM_J1939_SUPPORT == STD_ON) && ((DEM_MAX_SIZE_J1939FREEZEFRAME > 0u) || (DEM_MAX_SIZE_J1939EXFREEZEFRAME > 0u)))
#if (DEM_MAX_SIZE_J1939FREEZEFRAME > 0u)
/**
 * @brief         Check if J1939 freeze frame is Stored
 * @param[in]     MemoryIndex: Memory index
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckJ1939FFStored(Dem_NvBlockNumType MemoryIndex)
{
    const uint8* ptr = Dem_GetFreezeFrameRecordExtPtrOfMemEntry(MemoryIndex);
    boolean      ret = DEM_BITS_ISSET(ptr, DEM_J1939FREEZEFRAME_RECORD_INDEX); /* PRQA S 4404 */ /* VL_Dem_4404 */
    return ret;
}

/**
 * @brief         Sets J1939 freeze frame is Stored
 * @param[in]     MemoryIndex: Memory index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetJ1939FFStored(Dem_NvBlockNumType MemoryIndex)
{
    uint8* ptr = Dem_GetFreezeFrameRecordExtPtrOfMemEntry(MemoryIndex);
    DEM_BITS_SET(ptr, DEM_J1939FREEZEFRAME_RECORD_INDEX);
}
#endif

#if (DEM_MAX_SIZE_J1939EXFREEZEFRAME > 0u)
/**
 * @brief         Check if J1939 Ex freeze frame is Stored
 * @param[in]     MemoryIndex: Memory index
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckJ1939ExFFStored(Dem_NvBlockNumType MemoryIndex)
{
    const uint8* ptr = Dem_GetFreezeFrameRecordExtPtrOfMemEntry(MemoryIndex);
    boolean      ret = DEM_BITS_ISSET(ptr, DEM_J1939EXFREEZEFRAME_RECORD_INDEX); /* PRQA S 4404 */ /* VL_Dem_4404 */
    return ret;
}

/**
 * @brief         Sets J1939 Ex freeze frame is Stored
 * @param[in]     MemoryIndex: Memory index
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetJ1939ExFFStored(Dem_NvBlockNumType MemoryIndex)
{
    uint8* ptr = Dem_GetFreezeFrameRecordExtPtrOfMemEntry(MemoryIndex);
    DEM_BITS_SET(ptr, DEM_J1939EXFREEZEFRAME_RECORD_INDEX);
}
#endif

/**
 * @brief Collect J1939 Freeze Frame data stored for an event.
 */
DEM_LOCAL uint8 Dem_MemUpdateEventJ1939FF(Dem_EventIdType EventId, Dem_NvBlockNumType MemoryIndex)
{
    uint8 blockModification;
#if (DEM_MAX_SIZE_J1939FREEZEFRAME > 0u)
    if (Dem_CheckJ1939FFStored(MemoryIndex) == FALSE)
    {
        Dem_SetJ1939FFStored(MemoryIndex);

#if (DEM_MAX_NUMBER_PRESTORED_FF > 0u)
        /** If 'prestore freezeframe' is enabled and data is prestored, copy the prestored data */
        if (Dem_PrestoreApplyJ1939FreezeFrame(
                Dem_GetJ1939FreezeFramePtrOfMemEntry(MemoryIndex),
                DEM_MAX_SIZE_J1939FREEZEFRAME)
            == FALSE)
#endif
        {
            /** Otherwise, collect new samples from the application. */
            Dem_GetJ1939FFFromEnv(
                EventId,
                Dem_GetJ1939FreezeFramePtrOfMemEntry(MemoryIndex),
                DEM_MAX_SIZE_J1939FREEZEFRAME);
        }
        blockModification = DEM_DATA_USERDATA_CHANGED;
    }
#endif
#if (DEM_MAX_SIZE_J1939EXFREEZEFRAME > 0u)
    if (Dem_CheckJ1939ExFFStored(MemoryIndex) == FALSE)
    {
        Dem_SetJ1939ExFFStored(MemoryIndex);
#if (DEM_MAX_NUMBER_PRESTORED_FF > 0u)
        /** If 'prestore freezeframe' is enabled and data is prestored, copy the prestored data */
        if (Dem_PrestoreApplyJ1939ExFreezeFrame(
                Dem_GetJ1939ExFreezeFramePtrOfMemEntry(MemoryIndex),
                DEM_MAX_SIZE_J1939EXFREEZEFRAME)
            == FALSE)
#endif
        {
            /** Otherwise, collect new samples from the application. */
            Dem_GetJ1939ExFFFromEnv(
                EventId,
                Dem_GetJ1939ExFreezeFramePtrOfMemEntry(MemoryIndex),
                DEM_MAX_SIZE_J1939EXFREEZEFRAME);
        }
        blockModification = DEM_DATA_USERDATA_CHANGED;
    }
#endif
    return blockModification;
}
#endif

#if ((DEM_OBDII_SUPPORT == STD_ON) && (DEM_MAX_SIZE_OBDFREEZEFRAME > 0u))
/**
 * @brief         Check if OBD freeze frame is Stored
 */
DEM_LOCAL_INLINE boolean Dem_CheckOBDFFStored(Dem_NvBlockNumType MemoryIndex)
{
    return DEM_BITS_ISSET(Dem_GetFreezeFrameRecordExtPtrOfMemEntry(MemoryIndex), DEM_OBDFREEZEFRAME_RECORD_INDEX);
}

/**
 * @brief         Sets OBD freeze frame is Stored
 * @param[in]     MemoryIndex: Memory index
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetOBDFFStored(Dem_NvBlockNumType MemoryIndex)
{
    DEM_BITS_SET(Dem_GetFreezeFrameRecordExtPtrOfMemEntry(MemoryIndex), DEM_OBDFREEZEFRAME_RECORD_INDEX);
}

/**
 * @brief Collect OBD Freeze Frame data stored for an event.
 */
DEM_LOCAL uint8 Dem_MemUpdateEventOBDFF(Dem_EventIdType EventId, Dem_NvBlockNumType MemoryIndex)
{
    uint8 blockModification;
    if (Dem_CheckOBDFFStored(MemoryIndex) == FALSE)
    {
        Dem_SetOBDFFStored(MemoryIndex);
#if (DEM_MAX_NUMBER_PRESTORED_FF > 0u)
        /** If 'prestore freezeframe' is enabled and data is prestored, copy the prestored data */
        if (Dem_PrestoreApplyOBDFreezeFrame(
                Dem_GetOBDFreezeFramePtrOfMemEntry(MemoryIndex),
                DEM_MAX_SIZE_OBDFREEZEFRAME)
            == FALSE)
#endif
        {
            /** Otherwise, collect new samples from the application. */
            Dem_GetOBDFFFromEnv(EventId, Dem_GetOBDFreezeFramePtrOfMemEntry(MemoryIndex), DEM_MAX_SIZE_OBDFREEZEFRAME);
        }
        blockModification = DEM_DATA_USERDATA_CHANGED;
    }
    return blockModification;
}
#endif

/**
 * @brief Updates all data stored for an event.
 * Depending on configuration of the event data, collect the data to be updated due to the update trigger(s) in
 * Trigger. This includes statistical data (e.g. occurrence counter) as well as environment data collected from
 * callback routines.
 * Statistical data is stored separately in the MemoryEntry and is mapped into snapshot and extended records when
 * reading the data.
 */
DEM_LOCAL uint8 Dem_MemUpdateEventEntry(Dem_EventIdType EventId, uint8 Trigger, Dem_NvBlockNumType MemoryIndex)
{
    uint8 blockModification = DEM_DATA_NO_CHANGES;

    Dem_MemoryUpdateStart(MemoryIndex);
    /** On a TestFailed trigger */
    if (DEM_FLAG_ISSET(Trigger, DEM_TRIGGER_TESTFAILED))
    {
        blockModification |= Dem_MemUpdateEntryOnTF(EventId, MemoryIndex);
    }
    /** On a TestFailedThisCycle trigger */
    if (DEM_FLAG_ISSET(Trigger, DEM_TRIGGER_TESTFAILEDTHISCYCLE))
    {
        blockModification |= Dem_MemUpdateEntryOnTFTOC(EventId, MemoryIndex);
#if (defined(DEM_OCC4))
        /** In each operation cycle the counter shall be incremented as soon as FDC10 has reached a value that is
         * equal to or greater than its unconfirmedDTCLimit for the firsttime. */
        if (Dem_MemIncrementOCC4(MemoryIndex) == TRUE)
        {
            blockModification |= DEM_DATA_STATISTICS_CHANGED;
        }
#endif
#if (defined(DEM_OCC6))
        /** In each operation cycle the counter shall be incremented as soon as FDC10 has reached the value +127
         * for the first time. */
        if (Dem_MemIncrementOCC6(MemoryIndex) == TRUE)
        {
            blockModification |= DEM_DATA_STATISTICS_CHANGED;
        }
#endif
    }
#if (defined(DEM_SI30))
    /** On a WarningIndicatorRequested trigger */
    if (DEM_FLAG_ISSET(Trigger, DEM_TRIGGER_INDICATOR))
    {
        Dem_SetSI30Bit5OfMemEntry(MemoryIndex);
        blockModification |= DEM_DATA_STATISTICS_CHANGED;
    }
#endif

#if (DEM_FREEZE_FRAME_CLASS_NUMBER > 0u)
    {
#if (DEM_GENERAL_FF_RECNUM_CALCULATED == STD_ON)
        Dem_MemoryNumType memRef = Dem_GetMemRef(Dem_GetDTCAttr(EventId));
        if (
#if (DEM_GENERAL_FF_RECNUM_CONFIGURED == STD_ON)
            (DEM_FF_RECNUM_CONFIGURED == Dem_GetFFRecordOfMemory(memRef)) ||
#endif
            ((DEM_FF_RECNUM_CALCULATED == Dem_GetFFRecordOfMemory(memRef))
             && (DEM_FLAG_ISSET(Trigger, DEM_TRIGGER_TESTFAILED))))
#endif
        {
            blockModification |= Dem_MemUpdateEventFreezeFrame(EventId, MemoryIndex, Trigger);
        }
    }
#endif

#if (DEM_EXTENDED_DATA_CLASS_NUMBER > 0u)
    blockModification |= Dem_MemUpdateEventExtendData(EventId, MemoryIndex, Trigger);
#endif

#if ((DEM_J1939_SUPPORT == STD_ON) && ((DEM_MAX_SIZE_J1939FREEZEFRAME > 0u) || (DEM_MAX_SIZE_J1939EXFREEZEFRAME > 0u)))
    if (Dem_CheckEventJ1939Related(EventId) == TRUE)
    {
        blockModification |= Dem_MemUpdateEventJ1939FF(EventId, MemoryIndex);
    }
#endif

#if ((DEM_OBDII_SUPPORT == STD_ON) && (DEM_MAX_SIZE_OBDFREEZEFRAME > 0u))
    if (Dem_CheckEventObdRelated(EventId) == TRUE)
    {
        blockModification |= Dem_MemUpdateEventOBDFF(EventId, MemoryIndex);
    }
#endif
    Dem_MemoryUpdateFinish(MemoryIndex);
    return blockModification;
}

/**
 * @brief  Update the chronology of one entry in the event memory.
 */
DEM_LOCAL void Dem_MemoryUpdateIndex(Dem_MemStateInfoConstPtrType MemoryInfo, Dem_NvBlockNumType MemoryIndex)
{
    uint8 chronoIterator;
    uint8 tempIndex;
    /** Store the global timestamp to the entry, and update it */
    Dem_SetTimestampOfMemEntry(MemoryIndex, Dem_MemGetCurrentTimestamp());
    Dem_MemSetCurrentTimestamp(Dem_MemGetCurrentTimestamp() + 1u); /* PRQA S 1252 */ /* VL_Dem_1252 */
    /** move downwards the chronology moving all entries, until the updated element is overwritten */
    chronoIterator = Dem_GetCurSizeOfMemoryInfo(MemoryInfo) - 1u;
    tempIndex      = Dem_GetChronologyOfMemoryInfo(MemoryInfo, chronoIterator);
    while (tempIndex != MemoryIndex)
    {
        uint8 swap;

        swap = tempIndex;
        --chronoIterator;
        tempIndex = Dem_GetChronologyOfMemoryInfo(MemoryInfo, chronoIterator);
        Dem_SetChronologyOfMemoryInfo(MemoryInfo, chronoIterator, swap);
    }
    Dem_SetChronologyOfMemoryInfo(MemoryInfo, (Dem_GetCurSizeOfMemoryInfo(MemoryInfo) - 1u), MemoryIndex);
}

/**
 * @brief Update the statistics of an an event entry.
 */
DEM_LOCAL uint8
    Dem_UpdateEventMemoryStatistics(Dem_EventIdType EventId, Dem_NvBlockNumType MemoryIndex, boolean Occurrence)
{
    Dem_MemStateInfoConstPtrType memoryInfo        = Dem_MemStateInfoInit(Dem_GetMemRef(Dem_GetDTCAttr(EventId)));
    uint8                        blockModification = DEM_DATA_NO_CHANGES;

#if (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_OFF)
    if (Dem_GetAgingOnlyOfMemEntry(MemoryIndex))
    {
        /** Re-Initialize the memory entry - this will reset special flags like aging only */
        Dem_MemInitializeEntry(MemoryIndex);
#if ((defined(DEM_SI30)) && (DEM_OBD_DTC_NUMBER > 0))
        Dem_UpdateSI30BitOfMemEntry(MemoryIndex, EventId);
#endif
        DEM_FLAG_SET(blockModification, DEM_DATA_STATISTICS_CHANGED);
        /** Prepare expected statistics */
#if (defined(DEM_MAX_FDC_SINCE_LAST_CLEAR))
        Dem_SetMaxDebounceValueOfMemEntry(MemoryIndex, 0u);
#endif
    }
#endif
#if (defined(DEM_OCC2))
    /** The counter shall be cleared as soon as FDC10 reaches its unconfirmedDTCLimit. */
    Dem_SetOCC2OfMemEntry(MemoryIndex, 0u);
#endif
    if (Occurrence == TRUE)
    {
        /** On re-occurrence also update the chronology */
        Dem_MemoryUpdateIndex(memoryInfo, MemoryIndex);
        DEM_FLAG_SET(blockModification, DEM_DATA_STATISTICS_CHANGED); /* PRQA S 3473 */ /* VL_Dem_3473 */
    }
    return blockModification;
}

/**
 * @brief updates Memory failure statistics like first and most recently failed event, first and most recently confirmed
 * event.
 */
DEM_LOCAL void Dem_UpdateMemoryFailureStatistics(Dem_EventIdType EventId, uint8 Trigger)
{
    Dem_EventClientRefType clientStart = Dem_GetClientStartRefOfEvent(EventId);
    Dem_EventClientRefType clientEnd   = Dem_GetClientEndRefOfEvent(EventId);
    for (; clientStart < clientEnd; ++clientStart)
    {
        uint8 clientId = Dem_GetClientOfEventRef(clientStart);
        if ((EventId >= Dem_GetFirstEventOfClient(clientId)) && (EventId < Dem_GetLastEventOfClient(clientId)))
        {
            if ((Trigger & DEM_TRIGGER_CONFIRMED) != 0u)
            {
                if (Dem_GetFirstConfirmedEvent(clientId) == DEM_EVENT_INVALID)
                {
                    Dem_SetFirstConfirmedEvent(EventId, clientId);
                }
                Dem_SetMostRecentConfirmdEvent(EventId, clientId);
            }
            if (Dem_GetFirstFailedEvent(clientId) == DEM_EVENT_INVALID)
            {
                Dem_SetFirstFailedEvent(EventId, clientId);
            }
            Dem_SetMostRecentFailedEvent(EventId, clientId);
        }
    }
}

#if (DEM_DEBOUNCE_COUNTER_STORAGE_NUM > 0u)
/**
 * @brief Copies debounce values from debounce array to NVRAM mirror
 */
DEM_LOCAL void Dem_MemStorageDataDebounce(void)
{
    for (Dem_EventIdType eventId = DEM_EVENT_FIRST; eventId < Dem_GetEventNumber(); ++eventId)
    {
        Dem_DebounceAlgorithmNumType debounceRef = Dem_GetDebounceRefOfEvent(eventId);
        if (Dem_GetStorageOfDB(debounceRef))
        {
            Dem_DebounceCounterStorageRefNumType debounceCounterStorageRefNum = Dem_GetDCStorageRefOfDB(debounceRef);
            if (DEM_DEBOUNCE_COUNTER_STORAGE_NUM != debounceCounterStorageRefNum)
            {
                Dem_GetDebounceData()[debounceCounterStorageRefNum] = (sint16)Dem_GetSatelliteDebounceData(
                    Dem_GetSatelliteData(Dem_GetSatelliteIdByEventId(eventId), Dem_GetMonitorIdByEventId(eventId)));
            }
        }
    }
#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
    Dem_NvmSetSingleBlockState(DEM_NV_DEBOUNCEBLOCKINDEX, DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
#endif
}
#endif

/**
 * @brief Initializes Event memory
 */
DEM_LOCAL void Dem_MemoryEventInit(void)
{
    for (Dem_EventIdType eventId = DEM_EVENT_FIRST; eventId < Dem_GetEventNumber(); ++eventId)
    {
#if (DEM_DEBOUNCE_COUNTER_STORAGE_NUM > 0u)
        Dem_DebounceAlgorithmNumType debounceRef;
#endif

        /** Only reset stored status, since debouncing can have changed since pre-initalization */
        Dem_SetEventInternalStatusByEvent(
            eventId,
            Dem_SetEventstoredStatus(Dem_GetEventInternalStatusByEvent(eventId), Dem_InternalStoredStatus_None));

#if (DEM_DEBOUNCE_COUNTER_STORAGE_NUM > 0u)
        /** If NV storage of debounce values is configured, and the event uses that feature */
        /** Its safe to do it here since supporting events are not allowed to report before Dem_Init() */
        debounceRef = Dem_GetDebounceRefOfEvent(eventId);
        if (Dem_GetStorageOfDB(debounceRef))
        {
            sint16                               debounceValue;
            boolean                              failedThresholdReached;
            Dem_DebounceCounterStorageRefNumType debounceCounterStorageRefNum = Dem_GetDCStorageRefOfDB(debounceRef);
            if (DEM_DEBOUNCE_COUNTER_STORAGE_NUM != debounceCounterStorageRefNum)
            {
                debounceValue          = Dem_GetDebounceData()[debounceCounterStorageRefNum];
                failedThresholdReached = Dem_UDSStatusCheckTF(Dem_GetEventUDSStatus(eventId));
                Dem_SatelliteRestoreDebounceValue(eventId, debounceValue, failedThresholdReached);
            }
        }
#endif
    }
}

#if (DEM_COMBINATION_NUMBER > 0u)
/**
 * @brief Initializes Combined Event memory
 */
DEM_LOCAL void Dem_MemoryCombinedEventInit(void)
{
    for (Dem_CombinationNumType groupId = 0; groupId < DEM_COMBINATION_NUMBER; ++groupId)
    {
        /** Initialize the combination group internal status */
        Dem_EventInternalStatusType InternalStatus = Dem_InternalStoredStatus_None;
        /** For all combination groups, test if at least one sub-events is available by configuration. Initialization
         * in PreInit and Init can have different settings for  event availability, so repeat the calculation for
         * combined DTCs */
        for (Dem_ComDTCEventRefNumType start = Dem_GetStartRefOfCombinationDTC(groupId);
             start < Dem_GetEndRefOfCombinationDTC(groupId);
             ++start)
        {
            Dem_EventIdType eventId = Dem_GetventIdOfCombinationDTC(start);
            if (Dem_GetAvailableInVariantByEvent(eventId) == TRUE)
            {
                /** If so, set the availability in the combination status. */
                InternalStatus = Dem_SetEventAvailableStatus(InternalStatus);
                break;
            }
        }
        Dem_SetCombinedEventUDSStatus(groupId, InternalStatus);
    }
}
#endif

/* PRQA S 3406, 1505 ++ */ /* VL_Dem_3406, VL_Dem_1505 */
/**
 * @brief Initializes the NvBlock for administrative data
 * supposed to be called by the NvM in order to (re)initialize the data in case the non-volatile memory has never
 * been stored, or was corrupted. (See NvMBlockDescriptor/NvMInitBlockCallback) It can also be used to force a
 * reinitialization of the Dem data triggered by the application (e.g. after a new software version has been flashed
 * to the ECU). In the latter case, make sure the function is not called while the Dem is active.
 */
Std_ReturnType Dem_NvMInitAdminData(NvM_InitBlockRequestType InitBlockRequest)
{
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == TRUE)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
    else
#endif
    {
        Dem_GetAdminData().ConfigurationId = Dem_GetConfigurationId();

        for (uint8 operationCycleId = 0U; operationCycleId < DEM_OPERATION_CYCLE_NUMBER; ++operationCycleId)
        {
            Dem_SetCycleCounter(operationCycleId, 0u);
        }
        Dem_MemSet(Dem_GetAdminData().MemoryOverflow, 0u, sizeof(Dem_GetAdminData().MemoryOverflow));
#if (defined(DEM_AGED_COUNTER))
        Dem_MemSet(Dem_GetAdminData().AgedCounter, 0u, sizeof(Dem_GetAdminData().AgedCounter));
#endif
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_INTERNALAPIID, errorId);
    }
#endif
    DEM_PARAM_UNUSED(InitBlockRequest);
    /** The DEM expects that the NVM by itself marks the NVRAM block as dirty */
    return E_OK;
}

/**
 * @brief Initializes the NvBlock for event status data
 * supposed to be called by the NvM in order to (re)initialize the data in case the non-volatile memory has never
 * been stored, or was corrupted. (See NvMBlockDescriptor/NvMInitBlockCallback) It can also be used to force a
 * reinitialization of the Dem data triggered by the application (e.g. after a new software version has been flashed
 * to the ECU). In the latter case, make sure the function is not called while the Dem is active.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
Std_ReturnType Dem_NvMInitStatusData(NvM_InitBlockRequestType InitBlockRequest)
/* PRQA S 6070 -- */
{
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == TRUE)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
    else
#endif
    {
        for (Dem_EventIdType eventId = DEM_EVENT_FIRST; eventId < Dem_GetEventNumber(); ++eventId)
        {
            Dem_EventInitNvData(eventId);
        }

        for (uint8 clientId = 0u; clientId < DEM_CLIENT_NUMBER; ++clientId)
        {
            Dem_SetFirstFailedEvent(DEM_EVENT_INVALID, clientId);
            Dem_SetFirstConfirmedEvent(DEM_EVENT_INVALID, clientId);
            Dem_SetMostRecentFailedEvent(DEM_EVENT_INVALID, clientId);
            Dem_SetMostRecentConfirmdEvent(DEM_EVENT_INVALID, clientId);
        }
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_INTERNALAPIID, errorId);
    }
#endif
    DEM_PARAM_UNUSED(InitBlockRequest);
    /** The DEM expects that the NVM by itself marks the NVRAM block as dirty */
    return E_OK;
}

#if (DEM_DEBOUNCE_COUNTER_STORAGE_NUM > 0u)
/**
 * @brief Initializes the NvBlock for persisted debounce values
 * supposed to be called by the NvM in order to (re)initialize the data in case the non-volatile memory has never
 * been stored, or was corrupted. (See NvMBlockDescriptor/NvMInitBlockCallback) It can also be used to force a
 * reinitialization of the Dem data triggered by the application (e.g. after a new software version has been flashed
 * to the ECU). In the latter case, make sure the function is not called while the Dem is active.
 */
Std_ReturnType Dem_NvMInitDebounceData(NvM_InitBlockRequestType InitBlockRequest)
{
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == TRUE)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
    else
#endif
    {
        Dem_MemSet((uint8*)Dem_GetDebounceData(), 0u, sizeof(Dem_GetDebounceData()));
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_INTERNALAPIID, errorId);
    }
#endif
    DEM_PARAM_UNUSED(InitBlockRequest);
    /** The DEM expects that the NVM by itself marks the NVRAM block as dirty */
    return E_OK;
}
#endif
/* PRQA S 3406, 1505 -- */

/**
 * @brief         Initializes the memory entry
 * @param[in]     MemoryIndex: Memory index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_MemoryEntryInit(Dem_NvBlockNumType MemoryIndex)
{
    Dem_MemSet((uint8*)&Dem_GetMemoryEntry()[MemoryIndex], 0u, sizeof(Dem_EntryType));
}

/**
 * @brief Check for corrupt memory entry.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
DEM_LOCAL boolean Dem_MemFixCorruptMemoryEntry(Dem_MemStateInfoConstPtrType MemoryInfo, Dem_NvBlockNumType MemoryIndex)
/* PRQA S 6070 -- */
{
    boolean         ret     = FALSE;
    Dem_EventIdType eventId = Dem_GetEventIdOfMemEntry(MemoryIndex);

    if (eventId != DEM_EVENT_INVALID)
    {
        if ((eventId >= Dem_GetEventNumber()) || (Dem_GetDTCAttr(eventId) == DEM_DTC_ATTRIBUTES_NUMBER)
            || (Dem_GetMemRef(Dem_GetDTCAttr(eventId)) != Dem_GetMemIndexOfMemoryInfo(MemoryInfo)))
        {
            /** The memory entry is not allocated to the event. Remove it. */
            Dem_SetEventIdOfMemEntry(MemoryIndex, DEM_EVENT_INVALID);
            Dem_SetTimestampOfMemEntry(MemoryIndex, 0U);
#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
            Dem_NvmSetSingleBlockState(MemoryIndex, DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
#endif

#if (DEM_DEV_ERROR_DETECT == STD_ON)
            /** DET: To get this inconsistency, it requires a configuration change without clearing the NV contents
             */
            Dem_DetReportError(DEM_SID_INTERNALAPIID, DEM_E_INIT_FAILED);
#endif

            ret = TRUE; /** caller shall continue with next memory entry */
        }
    }
    return ret;
}

/**
 * @brief Check for duplicate memory entry.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
DEM_LOCAL boolean Dem_MemFixDoubleAllocation(Dem_MemStateInfoConstPtrType MemoryInfo, Dem_NvBlockNumType MemoryIndex)
/* PRQA S 6070 -- */
{
    boolean         ret     = FALSE;
    Dem_EventIdType eventId = Dem_GetEventIdOfMemEntry(MemoryIndex);
    if (eventId != DEM_EVENT_INVALID)
    {
        if (Dem_GetEventstoreStatus(Dem_GetDtcInterStatus(eventId)) != Dem_InternalStoredStatus_None)
        {
            /** Runtime error, no DET. This can happen due to NV errors */
            /** Dem_MemoryFindIndex will always succeed since we are processing a duplicate */
            uint8 otherIndex = Dem_MemoryFindIndex(MemoryInfo, eventId);
            /** Clear the older memory entry */
            if (Dem_GetTimestampOfMemEntry(otherIndex) < Dem_GetTimestampOfMemEntry(MemoryIndex))
            {
                Dem_MemoryFreeIndex(MemoryInfo, otherIndex);
#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
                Dem_NvmSetSingleBlockState(otherIndex, DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
#endif
            }
            else
            {
                Dem_SetEventIdOfMemEntry(MemoryIndex, DEM_EVENT_INVALID);
                Dem_SetTimestampOfMemEntry(MemoryIndex, 0U);
#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
                Dem_NvmSetSingleBlockState(MemoryIndex, DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
#endif
                ret = TRUE; /** caller shall continue with next memory entry */
            }
        }
    }
    return ret;
}

#if (DEM_ENTRY_TRIGGER_ON_CONFIRMED == STD_ON)
/**
 * @brief Restore parts of the UDS status, using redundant data, when storage happens on confirmed.
 */
DEM_LOCAL void Dem_MemRestoreStatusBitsOnConfirmed(Dem_EventIdType EventId)
{
    /** If the event belongs to a combined group, ignore the event */
#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
    if (Dem_GetGroupIdOfEvent(EventId) == DEM_COMBINATION_NUMBER)
#endif
    {
        /** If retaining memory after aging is supported, ignore aged events else process the event */
        uint8 eventStatus = Dem_GetEventUDSStatus(EventId);
        /** Since DTCs are stored only on confirmed, set CDTC and TFSLC bits */
        eventStatus = Dem_UDSStatusSetTFSLC(eventStatus);
        eventStatus = Dem_UDSStatusSetCDTC(eventStatus);
        /** Store the modified bits for the event */
        Dem_SetEventUDSStatus(EventId, eventStatus);
    }
}
#endif

#if (DEM_ENTRY_TRIGGER_ON_TEST_FAILED == STD_ON)
/**
 * @brief Restore parts of the UDS status, using redundant data, when storage happens on failed.
 */
DEM_LOCAL void Dem_MemRestoreStatusBitsOnFailed(Dem_EventIdType EventId)
{
    /** If the event belongs to a combined group, ignore the event */
#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
    if (Dem_GetGroupIdOfEvent(EventId) == DEM_COMBINATION_NUMBER)
#endif
    {
        uint8 eventStatus = Dem_GetEventUDSStatus(EventId);
        /** If retaining memory after aging is supported and memory entry is aged, clear TF bit */
        /** Set TFSLC bit */
        eventStatus = Dem_UDSStatusSetTFSLC(eventStatus);
        /** If event has confirmed target == 1, set CDTC bit */
        if (Dem_GetEventConfirmationThreshold(EventId) == 1u)
        {
            eventStatus = Dem_UDSStatusSetCDTC(eventStatus);
        }
        /** Store the modified bits for the event */
        Dem_SetEventUDSStatus(EventId, eventStatus);
    }
}
#endif

#if ((DEM_ENTRY_TRIGGER_ON_FDC_THRESHOLD == STD_ON) && (defined(DEM_OCCCTR)))
/**
 * @brief Restore parts of the UDS status, using redundant data, when storage happens at a FDC threshold.
 */
DEM_LOCAL void Dem_MemRestoreStatusBitsOnFdc(Dem_NvBlockNumType MemoryIndex, Dem_EventIdType EventId)
{
    /** If the event belongs to a combined group, ignore the event */
#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
    if (Dem_GetGroupIdOfEvent(EventId) == DEM_COMBINATION_NUMBER)
#endif
    {
        uint8 eventStatus = Dem_GetEventUDSStatus(EventId);
        /** If retaining memory after aging is supported and memory entry is aged, clear TF bit */
        /** If there is an occurrence counter > 0, set TFSLC bit, as we know about some TF 0->1 transitions */
#if (defined(DEM_OCCCTR))
        if (Dem_GetOCCOfMemEntry(MemoryIndex) > 0u)
        {
            eventStatus = Dem_UDSStatusSetTFSLC(eventStatus);
        }
#endif
        /** Store the modified bits for the event */
        Dem_SetEventUDSStatus(EventId, eventStatus);
    }
}
#endif

/**
 * @brief Update the aging status of an event in it's internal status.
 */
DEM_LOCAL Dem_EventInternalStatusType
    Dem_MemRestoreStoredStatusFromEntry(Dem_NvBlockNumType MemoryIndex, Dem_EventInternalStatusType InternalStatus)
{
    Dem_EventInternalStatusType ret;
    uint16                      agingCounter = Dem_GetAgingCounterOfMemEntry(MemoryIndex);
    /** If the memory entry is currently aging, mark the event as AGING */
    if ((agingCounter <= DEM_OPERATIONCYCLE_CYCLECOUNT_MAX) || (agingCounter == DEM_OPERATIONCYCLE_CYCLECOUNT_LATCHED))
    {
        ret = Dem_SetEventstoredStatus(InternalStatus, Dem_InternalStoredStatus_Aging);
    }
    else
    /** Otherwise: Mark the event as ACTIVE */
    {
        ret = Dem_SetEventstoredStatus(InternalStatus, Dem_InternalStoredStatus_Active);
    }
    return ret;
}

/**
 * @brief Restores the event status and internal status based on the configured storage trigger.
 */
DEM_LOCAL void Dem_MemRestoreEventStatus(Dem_NvBlockNumType MemoryIndex, Dem_EventIdType EventId)
{
    Dem_EventInternalStatusType internalStatus = Dem_GetDtcInterStatus(EventId);
    Dem_MemoryNumType           memRef         = Dem_GetMemRef(Dem_GetDTCAttr(EventId));
    uint8                       storageTrigger = Dem_GetStorageTriggerOfMemory(memRef);

    /** Dependent on the available redundancy, restore UDS status bits and the internal status */
#if (DEM_ENTRY_TRIGGER_ON_CONFIRMED == STD_ON)
    if (DEM_FLAG_ISSET(storageTrigger, DEM_TRIGGER_CONFIRMED))
    {
        Dem_MemRestoreStatusBitsOnConfirmed(EventId);
    }
    else
#endif
#if (DEM_ENTRY_TRIGGER_ON_TEST_FAILED == STD_ON)
        if (DEM_FLAG_ISSET(storageTrigger, DEM_TRIGGER_TESTFAILED))
    {
        Dem_MemRestoreStatusBitsOnFailed(EventId);
    }
    else
#endif
#if ((DEM_ENTRY_TRIGGER_ON_FDC_THRESHOLD == STD_ON) && (defined(DEM_OCCCTR)))
        if (DEM_FLAG_ISSET(storageTrigger, DEM_TRIGGER_FDC))
    {
        Dem_MemRestoreStatusBitsOnFdc(MemoryIndex, EventId);
    }
    else
#endif
    {
        /** Mirsa C */
    }
    internalStatus = Dem_MemRestoreStoredStatusFromEntry(MemoryIndex, internalStatus);
    /*Store the modified internal status for the event*/
    Dem_SetDtcInterStatus(EventId, internalStatus);
}

/**
 * @brief Sort the memory entry into the chronological list.
 */
DEM_LOCAL void Dem_MemSortEntryIntoChronologicalList(
    Dem_MemStateInfoConstPtrType MemoryInfo,
    Dem_NvBlockNumType           MemoryIndex,
    uint32                       Timestamp)
{
    uint8 sortedIndex;
    for (sortedIndex = Dem_GetCurSizeOfMemoryInfo(MemoryInfo);
         (sortedIndex > 0u)
         && (Dem_GetTimestampOfMemEntry(Dem_GetChronologyOfMemoryInfo(MemoryInfo, (sortedIndex - 1u))) > Timestamp);
         --sortedIndex)
    {
        Dem_SetChronologyOfMemoryInfo(
            MemoryInfo,
            sortedIndex,
            Dem_GetChronologyOfMemoryInfo(MemoryInfo, (sortedIndex - 1u)));
    }
    Dem_SetChronologyOfMemoryInfo(MemoryInfo, sortedIndex, MemoryIndex);
}

/**
 * @brief verifies the integrity of memory entries and links them up into the runtime control structures.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
DEM_LOCAL void Dem_MemoryInitRestoreMemory(Dem_MemoryNumType MemIndex)
/* PRQA S 6070 -- */
{
    /** No reinitialization - restore primary stack */
    Dem_MemStateInfoConstPtrType memoryInfo     = Dem_MemStateInfoInit(MemIndex);
    Dem_NvBlockNumType           memoryIndex    = Dem_GetMemEntryStartOfMemoryInfo(memoryInfo);
    Dem_NvBlockNumType           memoryIndexEnd = Dem_GetMemEntryEndOfMemoryInfo(memoryInfo);

    Dem_SetCurSizeOfMemoryInfo(memoryInfo, 0u);
    /** Iterate through all memory entries. The iteration order must be identical to Dem_MemAccess_MemoryFindIndex.
     */
    for (; memoryIndex < memoryIndexEnd; ++memoryIndex)
    {
        Dem_EventIdType eventId;

        /** Check and fix problems with internal corruption of the memory entry record: */
        if (Dem_MemFixCorruptMemoryEntry(memoryInfo, memoryIndex) == TRUE)
        {
            /** Continue with next memory entry */
            continue; /* PRQA S 0770 */ /* VL_Dem_0770 */
        }

        /** If the memory entry is allocated to an event that already has a memory entry: */
        if (Dem_MemFixDoubleAllocation(memoryInfo, memoryIndex) == TRUE)
        {
            /** Continue with next memory entry */
            continue; /* PRQA S 0770 */ /* VL_Dem_0770 */
        }
        eventId = Dem_GetEventIdOfMemEntry(memoryIndex);
        if (eventId != DEM_EVENT_INVALID)
        {
            uint32 timestamp = Dem_GetTimestampOfMemEntry(memoryIndex);
            Dem_MemRestoreEventStatus(memoryIndex, eventId);
            /** Synchronize the global timestamp with the timestamp stored in the memory entry */
            if (timestamp > Dem_MemGetCurrentTimestamp())
            {
                Dem_MemSetCurrentTimestamp(timestamp);
            }
            /** Sort the memory entry into the chronological list */
            Dem_MemSortEntryIntoChronologicalList(memoryInfo, memoryIndex, timestamp);
            /** Update the number of occupied memory entries */
            Dem_SetCurSizeOfMemoryInfo(memoryInfo, (Dem_GetCurSizeOfMemoryInfo(memoryInfo) + 1u));
        }
    }
    /** Update the global timestamp */
    Dem_MemSetCurrentTimestamp(Dem_MemGetCurrentTimestamp() + 1u); /* PRQA S 1252 */ /* VL_Dem_1252 */
}

/**
 * @brief Initializes NvM Memory use in dem
 */
DEM_LOCAL void Dem_NvMMemoryInit(void)
{
#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
    /** Initialize transactional update entry status */
    Dem_MemSet(Dem_GetMemoryEntryStatus(), 0u, DEM_NV_RAM_BLOCK_NUMBER);
#endif

    if (Dem_GetAdminData().ConfigurationId != Dem_GetConfigurationId())
    {
        /** If the NV data is not compatible to the configuration */
        /** Re-initialize all NV data content and reset the global time stamp */

#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
        /** Set All Blocks Dirty */
        Dem_MemSet((uint8*)&Dem_GetMemoryStatus(), DEM_NVM_BLOCKSTATE_DIRTY, DEM_NV_RAM_BLOCK_NUMBER);
#endif

        (void)Dem_NvMInitAdminData(0u);
        (void)Dem_NvMInitStatusData(0u);
#if (DEM_DEBOUNCE_COUNTER_STORAGE_NUM > 0u)
        (void)Dem_NvMInitDebounceData(0u);
#endif
        Dem_MemSetCurrentTimestamp(1u);
        for (Dem_MemoryNumType memIndex = 0u; memIndex < DEM_TOTAL_MEMORY_NUMBER; ++memIndex)
        {
            Dem_GetMemCounter()[memIndex] = 0u;
        }
        /** Iterate through the primary indices - the chronological index becomes invalid in case an event ages */
        for (Dem_NvBlockNumType memoryIndex = DEM_NV_PRIMARYBLOCKINDEX; memoryIndex < DEM_NV_ADMINBLOCKINDEX;
             ++memoryIndex)
        {
            Dem_MemoryEntryInit(memoryIndex);
            Dem_MemoryUpdateInit(memoryIndex);
        }
    }
    /** Otherwise */
    else
    {
        Dem_MemSetCurrentTimestamp(0u);
        /** Rebuild the event memory control data from NV data contents */
        for (Dem_MemoryNumType memIndex = 0u; memIndex < DEM_TOTAL_MEMORY_NUMBER; ++memIndex)
        {
            Dem_MemoryInitRestoreMemory(memIndex);
        }
    }
}

/**
 * @brief         Initializes backing memory
 * This function controls initialization of the memory. After the function completes, event memories are available.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_MemoryInit(void)
{
    Dem_MemoryEventInit();
#if (DEM_COMBINATION_NUMBER > 0u)
    Dem_MemoryCombinedEventInit();
#endif
#if (DEM_ENABLE_CONDITION_GROUP_NUMBER > 0u)
    Dem_EnableConditionInit();
#endif
#if (DEM_STORAGE_CONDITION_GROUP_NUMBER > 0u)
    Dem_StorageConditionInit();
#endif
    Dem_NvMMemoryInit();
#if (DEM_MAX_NUMBER_PRESTORED_FF > 0u)
    /** If prestore freeze frame is enabled, initialize freeze frame prestorage */
    Dem_PrestoreInit();
#endif
}

#if (DEM_FEATURE_NVM_IMMEDIATE == STD_ON)
#if (DEM_FEATURE_SUPPORT_NVM_POLLING == STD_ON)
/**
 * @brief Poll the result of an ongoing write job
 */
DEM_LOCAL void Dem_NvmGetNvMResult(void)
{
    if (Dem_NvmGetCurrentCommitState() == DEM_NVM_COMMIT_WRITING)
    {
        NvM_RequestResultType requestResult;
        Dem_NvBlockNumType    currentMemoryIndex = Dem_NvmGetCurrentMemoryIndex();
        if (NvM_GetErrorStatus(Dem_GetNvBlockRef(currentMemoryIndex), &requestResult) == E_OK)
        {
            if (requestResult != NVM_REQ_PENDING)
            {
                if (requestResult != NVM_REQ_OK)
                {
                    /** Write request aborted or otherwise failed. */
                    Dem_NvmSetCurrentCommitState(DEM_NVM_COMMIT_FAILED);
                }
                else
                {
                    /** continue with next block */
                    Dem_NvmSetCurrentCommitState(DEM_NVM_COMMIT_FINISHED);
                }
            }
            /** else: NvM is still busy, wait */
        }
        else
        {
            Dem_NvmSetCurrentCommitState(DEM_NVM_COMMIT_FAILED);
        }
    }
}
#endif
#endif

/* PRQA S 3406, 1503,1532 ++ */ /* VL_Dem_3406, VL_QAC_NoUsedApi,VL_QAC_OneFunRef */

/**
 * @brief Notifies the Dem module about a completed NV operation
 */
Std_ReturnType Dem_NvMJobFinished(NvM_BlockRequestType ServiceId, NvM_RequestResultType JobResult)
/* PRQA S 3406, 1503,1532 -- */
{
#if ((DEM_FEATURE_SUPPORT_NVM_POLLING == STD_OFF) && (DEM_FEATURE_NVM_IMMEDIATE == STD_ON))
    if (Dem_NvmGetCurrentCommitState() == DEM_NVM_COMMIT_WRITING)
    {
        if (JobResult != NVM_REQ_OK)
        {
            /** Write request aborted or otherwise failed. */
            Dem_NvmSetCurrentCommitState(DEM_NVM_COMMIT_FAILED);
        }
        else
        {
            /** Continue with next block */
            Dem_NvmSetCurrentCommitState(DEM_NVM_COMMIT_FINISHED);
        }
    }
    /** else: the NvM has finished with some job,  e.g. restoring a block during ReadAll.
       We must ignore these notifications, since the value passed as ServiceId is not specified. */
#else
    DEM_PARAM_UNUSED(JobResult);
#endif
    DEM_PARAM_UNUSED(ServiceId);
    return E_OK;
}

#if (DEM_FEATURE_NVM_IMMEDIATE == STD_ON)
/**
 * @brief Find Next Block To Commit
 */
DEM_LOCAL boolean Dem_NvmFindNextBlockToCommit(void)
{
    boolean foundBlock = FALSE;
    /** Event states first, they are bound to be modified quickly */
    if (Dem_NvmCheckBlockStateImmediate(Dem_NvmGetBlockState(DEM_NV_EVENTSTATUSBLOCKINDEX)) == TRUE)
    {
        Dem_NvmSetCurrentMemoryIndex(DEM_NV_EVENTSTATUSBLOCKINDEX);
        foundBlock = TRUE;
    }
    else
    {
        /** start at last inspected index to write blocks fairly */
        Dem_NvBlockNumType blockIndex;
        Dem_NvBlockNumType endIndex;

        blockIndex = Dem_NvmGetCurrentMemoryIndex();
        endIndex   = blockIndex;
        do
        {
            ++blockIndex;
            if (blockIndex >= DEM_NV_RAM_BLOCK_NUMBER)
            {
                blockIndex = 0u;
            }

            if (Dem_NvmCheckBlockStateImmediate(Dem_NvmGetBlockState(blockIndex)) == TRUE)
            {
                /** Found a block that needs to be committed */
                Dem_NvmSetCurrentMemoryIndex(blockIndex);
                foundBlock = TRUE;
                break;
            }
        } while (blockIndex != endIndex); /** Stop when all slots have been inspected */
    }
    return foundBlock;
}

/**
 * @brief writes the memory block marked for immediate synchronization into the NV storage.
 */
DEM_LOCAL void Dem_NvmCommitBlock(void)
{
    Dem_NvBlockNumType blockIndex = Dem_NvmGetCurrentMemoryIndex();

#if (DEM_CLEAR_DTC_BEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
    Dem_NvmSetCurrentBlockIsCleared(Dem_NvmCheckBlockStateClear(Dem_NvmGetBlockState(blockIndex)));
#endif

    if (blockIndex == DEM_NV_ADMINBLOCKINDEX)
    {
        Dem_NvmSetBlockState(blockIndex, DEM_NVM_BLOCKSTATE_DIRTY);
    }
    else
    {
        Dem_NvmSetBlockState(blockIndex, DEM_NVM_BLOCKSTATE_UNMODIFIED);
    }

    Dem_MemCpy((uint8*)&Dem_NvmCommitBuffer.MRaw[0], Dem_MemoryDataPtr[blockIndex], Dem_MemoryDataSize[blockIndex]);

    /** Job End notification might return immediately - set the commit state before WriteBlock */
    Dem_NvmSetCurrentCommitState(DEM_NVM_COMMIT_WRITING);

    if (NvM_WriteBlock(Dem_GetNvBlockRef(blockIndex), &Dem_NvmCommitBuffer.MRaw[0]) != E_OK)
    {
        /** Write attempt failed - act as if callback had returned failed.
         * This case is handled by Dem_NvMTask */
        Dem_NvmSetCurrentCommitState(DEM_NVM_COMMIT_FAILED);
    }
}

/**
 * @brief Processes NvM Task
 * The function controls NvM commits. Modified blocks are passed to the NvM to write back, and once the write
 * operation has finished the next modified block is processed.
 */
DEM_LOCAL void Dem_NvMTask(void)
{
    /** Check for ongoing NV commit */
    if (Dem_NvmCheckPendingCommits() == TRUE)
    {
#if (DEM_FEATURE_SUPPORT_NVM_POLLING == STD_ON)
        Dem_NvmGetNvMResult();
#endif
        switch (Dem_NvmGetCurrentCommitState())
        {
        case DEM_NVM_COMMIT_FAILED:
        {
            /** Error handling: This implementation will mark the block as dirty in case of failed NvM_WriteBlock.
             * Another immediate update is done only if the block changes again */
            Dem_NvBlockNumType currentMemoryIndex = Dem_NvmGetCurrentMemoryIndex();
            (void)NvM_SetRamBlockStatus(Dem_GetNvBlockRef(currentMemoryIndex), TRUE);
#if (DEM_CLEAR_DTC_BEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
            if (Dem_NvmCheckCurrentBlockIsCleared() == TRUE)
            {
                Dem_SetClearWriteErrors();
            }
#endif
        }
            /** FALL THROUGH */
        case DEM_NVM_COMMIT_FINISHED: /* PRQA S 2003 */ /* VL_Dem_2003 */
            /** Post-processing of NV update */
            Dem_NvmDecrementPendingCommits();
#if (DEM_CLEAR_DTC_BEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
            if (Dem_NvmCheckCurrentBlockIsCleared() == TRUE)
            {
                Dem_NvmSetCurrentBlockIsCleared(FALSE);
                Dem_NvmDecrementPendingClearCommits();
                if (Dem_NvmCheckPendingClearCommits() == FALSE)
                {
                    Dem_NotifyClearCompleted();
                }
            }
#endif
            /** FALL THROUGH */
        case DEM_NVM_COMMIT_IDLE: /* PRQA S 2003 */ /* VL_Dem_2003 */
            /** Look for next dirty/immediate NV block */
            if (Dem_NvmCheckPendingCommits() == TRUE)
            {
                if (Dem_NvmFindNextBlockToCommit() == FALSE)
                {
                    /** We will continue looking for the pending block in the next task */
                    Dem_NvmSetCurrentCommitState(DEM_NVM_COMMIT_IDLE);
                }
                else
                {
                    Dem_NvmCommitBlock();
                }
            }
            else
            {
                Dem_NvmSetCurrentCommitState(DEM_NVM_COMMIT_IDLE);
            }
            break;
        default:
            /** DEM_NVM_COMMIT_WRITING - NvM is still busy, do nothing until this changes (no timeout) */
            break;
        }
    }
}
#endif

#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
/**
 * @brief         All pending Nv updates are reported as modified block for the NvM. (NvM_SetRamBlockStatus). The Dem
 * assumes that all such marked blocks are persisted in NV-Ram using NvM_WriteAll following the Dem shutdown. This is
 * not in scope of the Dem module however. If Dem_Shutdown is ever called without a following NvM_WriteAll, data will be
 * lost.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_NvmShutdown(void)
{
    Dem_NvBlockNumType blockIndex;

    for (blockIndex = 0u; blockIndex < DEM_NV_RAM_BLOCK_NUMBER; ++blockIndex)
    {
        if (Dem_NvmGetBlockState(blockIndex) != DEM_NVM_BLOCKSTATE_UNMODIFIED)
        {
            (void)NvM_SetRamBlockStatus(Dem_GetNvBlockRef(blockIndex), TRUE);
        }
    }

#if (DEM_FEATURE_NVM_IMMEDIATE == STD_ON)
    Dem_NvmSetCurrentMemoryIndex(0u);
    Dem_NvmResetPendingCommits();
    Dem_NvmSetCurrentCommitState(DEM_NVM_COMMIT_IDLE);
#if (DEM_CLEAR_DTC_BEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
    Dem_NvmResetPendingClearCommits();
    Dem_NvmSetCurrentBlockIsCleared(FALSE);
#endif
#endif
}
#endif

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif
