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
 **  @file               : Dem_MemInterface.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : Declaration file of Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_MEM_INTERFACE_H
#define DEM_MEM_INTERFACE_H

/* =================================================== inclusions =================================================== */
#include "Dem_Lcfg.h"

/* ===================================================== macros ===================================================== */
#if (                                                                                       \
    ((DEM_IMMEDIATE_STORAGE == STD_ON) || (DEM_CLEAR_DTC_BEHAVIOR != DEM_CLRRESP_VOLATILE)) \
    && (DEM_NV_RAM_BLOCK_NUMBER > 0u))
/** Define whether NVM immediate updates are supported */
#define DEM_FEATURE_NVM_IMMEDIATE STD_ON
#else
/** Define whether NVM immediate updates are supported */
#define DEM_FEATURE_NVM_IMMEDIATE STD_OFF
#endif

#define DEM_MEM_INVALID_MEMORY_INDEX                                                \
    DEM_NV_RAM_BLOCK_NUMBER /**< Invalid memory index used for 'no index available' \
                             */

/**
 * @name  DEM_DATA_ENCODE
 * @{
 * @brief Macros encoding changed data while updating DTCs
 */
#define DEM_DATA_NO_CHANGES       (0x00u) /**< No data changed */
#define DEM_DATA_USERDATA_CHANGED (0x01u) /**< User data changed, update NV ram and call data change notification */
#define DEM_DATA_STATISTICS_CHANGED                                                   \
    (0x02u) /**< Internal data changed, update NV ram but no data change notification \
             */
#define DEM_DATA_OCCURRENCE_MAX           (0xFFu) /**< Maximum value for 1 byte occurrence counter */
#define DEM_DATA_FAILED_CYCLE_COUNTER_MAX (0x7Fu) /**< Maximum value for 1 byte Failed Cycle counter */
#define DEM_ENTRY_UPDATE_STARTED          (0x80u) /**< Set flag in memory state which indicate an ongoing commit */
#define DEM_ENTRY_UPDATE_INIT             (0x00u) /**< Initial transaction number of NV transaction state */
/** @} */

#if (DEM_OBDII_SUPPORT == STD_ON)
/**
 * @name  DEM_PERMANENT_ENCODE
 * @{
 * @brief status values for permanent DTCs. Low byte is used for the state and the high byte is used for the visibility
 * and the clear conditions
 */
#define DEM_PERMANENT_STATES           (0x03u) /**< range of valid permanent states */
#define DEM_PERMANENT_NONE             (0x00u) /**< Permanent entry not used */
#define DEM_PERMANENT_ACTIVE           (0x01u) /**< Permanent entry active */
#define DEM_PERMANENT_PASSIVE          (0x02u) /**< Permanent entry not active */
#define DEM_PERMANENT_VISIBLE          (0x80u) /**< Permanent entry visible in mode 0xa */
#define DEM_PERMANENT_CONDITION_TESTED (0x40u) /**< Permanent entry DTC tested this DCY */
#define DEM_PERMANENT_CONDITION_PFC    (0x20u) /**< Permanent entry PFC condition fulfilled */
#define DEM_PERMANENT_CONDITION_DCY    (0x10u) /**< Permanent entry DCY qualified */
/** @} */
#endif

/**
 * @name  DEM_PSI30_STATUS_ENCODE
 * @{
 * @brief The status indicator consists of the following bits: DTC status indicator 30 (SI30)
 * bit 0: UnconfirmedDTC: occurs will be set to 1, pass set to 0
 * bit 1: UnconfirmedDTCThisOperationCyle: occurs will be set to 1, start of an operation cycle set to 0
 * bit 2: UnconfirmedDTCSinceLastClear:  set to 1 when the FDC10 reach. no set to 0
 * bit 3: AgedDTC: because aged will remove entry. reoccurs will be set to 0, so always set to 0
 * bit 4: SymptomSinceLastClear: occurs and condition fulfilled will be set to 1, no set to 0,  not implemented
 * bit 5: WarningIndicatorRequestedSinceLastClear: set to 1 when DTC status bit no 7 is set to 1. no set to 0
 * bit 6: EmissionRelatedDTC: set to 0 when the DTC is not emission related. set to 1 when the DTC is emission related.
 * bit 7: TestFailedSinceLastClear/Aged: because aged will remove entry. reoccurs will be set to 0, so always set to 0
 */
#define DEM_SI30_STATUS_UC      0x01u /**< bit 0: UnconfirmedDTC */
#define DEM_SI30_STATUS_UCTOC   0x02u /**< bit 1: UnconfirmedDTCThisOperationCyle */
#define DEM_SI30_STATUS_UCSLC   0x04u /**< bit 2: UnconfirmedDTCSinceLastClear. */
#define DEM_SI30_STATUS_AGED    0x08u /**< bit 3: AgedDTC */
#define DEM_SI30_STATUS_SSLC    0x10u /**< bit 4: SymptomSinceLastClear */
#define DEM_SI30_STATUS_WIRSLC  0x20u /**< bit 5: WarningIndicatorRequestedSinceLastClear */
#define DEM_SI30_STATUS_ER      0x40u /**< bit 6: EmissionRelatedDTC */
#define DEM_SI30_STATUS_TFSLC   0x80u /**< bit 7: TestFailedSinceLastClear/Aged */
#define DEM_SI30_STATUS_DEFALUT 0x07u /**< Default status */
/** @} */

/* ================================================ type definitions ================================================ */
/**
 * @brief Displacement info data type for selecting the "best" element for displacement
 */
typedef struct Dem_MemDisplacementInfoTag /* PRQA S 1536 */ /* VL_Dem_1536 */
{
    uint16            EventId;     /**< EventId of current element @range 0..65535 */
    Dem_MemoryNumType MemoryIndex; /**< Memory index of current element @range 0..255 */
    uint8             DtcStatus;   /**< DTC status bits, with modifications for combined events @range 0..255 */
} Dem_MemDisplacementInfoType;

/**
 * @brief Result of a displacement check
 */
typedef enum
{
    Dem_MemDisplace_NeedMoreTests, /**< Candidate is not classified yet, apply further tests on this candidate @range 0u
                                    */
    Dem_MemDisplace_RatingDone,    /**< Candidate is classified, continue the scan with next candidate @range 1u */
    Dem_MemDisplace_FoundBestMatch /**< Candidate is the best match, abort scanning for better matches @range 2u */
} Dem_MemDisplaceCheckResultType;

/* ========================================== internal function declarations ======================================== */
#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
/**
 * @brief         Reset Memory Overflow of Admin Data
 * @param[out]    MemIndex: Memory Index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_ResetMemoryOverflow(Dem_MemoryNumType MemIndex);

/**
 * @brief         set Single Block Status
 * @param[in]     MemoryIndex: Memory Index
 * @param[in]     Status: The memory status
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_NvmSetSingleBlockState(Dem_NvBlockNumType MemoryIndex, uint8 Status);

/**
 * @brief         set Event Status Block need to store
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_NvmSetStatusBlockChanged(void);

/**
 * @brief         check Event Status Block need to store
 * @param[in]     OldEventStatus: Old event status
 * @param[in]     NewEventStatus: New event status
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void
    Dem_CheckEventStatusBlock(Dem_UdsStatusByteType OldEventStatus, Dem_UdsStatusByteType NewEventStatus);
#endif

/**
 * @brief         Get DTC Status Mask By Mem
 * @param[out]    MemIndex: Memory Index
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetDTCStatusMaskByMem(Dem_MemoryNumType MemIndex);

/**
 * @brief         Init MemStateInfo
 * @param[out]    MemIndex: Memory Index
 * @return        Dem_MemStateInfoPtrType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
/* PRQA S 5016 ++ */ /* VL_Dem_5016 */
DEM_LOCAL_INLINE Dem_MemStateInfoPtrType Dem_MemStateInfoInit(Dem_MemoryNumType MemIndex);

/**
 * @brief         get EventId of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory Index
 * @return        Dem_EventIdType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventIdType Dem_GetEventIdOfMemEntry(Dem_NvBlockNumType MemoryIndex);

/**
 * @brief         get AgingCounter of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory Index
 * @return        uint16
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint16 Dem_GetAgingCounterOfMemEntry(Dem_NvBlockNumType MemoryIndex);

/**
 * @brief         Set AgingCounter of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory Index
 * @param[in]     AgingCounter: Aging counter
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetAgingCounterOfMemEntry(Dem_NvBlockNumType MemoryIndex, uint16 AgingCounter);

#if (defined(DEM_MAX_FDC_SINCE_LAST_CLEAR))
/**
 * @brief         get MaxDebounceValue of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory Index
 * @return        sint16
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE sint16 Dem_GetMaxDebounceValueOfMemEntry(Dem_NvBlockNumType MemoryIndex);

/**
 * @brief         Set MaxDebounceValue of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory Index
 * @param[in]     MaxDebounceValue: Max debounce value
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetMaxDebounceValueOfMemEntry(Dem_NvBlockNumType MemoryIndex, sint16 MaxDebounceValue);
#endif

#if ((defined(DEM_CYCLES_SINCE_FIRST_FAILED)) || (defined(DEM_OCC3)))
/**
 * @brief         get CyclesSinceFirstFailed of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory Index
 * @return        uint16
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint16 Dem_GetCyclesSinceFirstFailedOfMemEntry(Dem_NvBlockNumType MemoryIndex);

/**
 * @brief         Set CyclesSinceFirstFailed of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory Index
 * @param[in]     CyclesSinceFirstFailed: The cycles since first failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void
    Dem_SetCyclesSinceFirstFailedOfMemEntry(Dem_NvBlockNumType MemoryIndex, uint16 CyclesSinceFirstFailed);
#endif

#if ((defined(DEM_CYCLES_SINCE_LAST_FAILED)) || (defined(DEM_OCC1)))
/**
 * @brief         get CyclesSinceLastFailed of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory Index
 * @return        uint16
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint16 Dem_GetCyclesSinceLastFailedOfMemEntry(Dem_NvBlockNumType MemoryIndex);

/**
 * @brief         Set CyclesSinceLastFailed of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory Index
 * @param[in]     CyclesSinceLastFailed: The cycles since last failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void
    Dem_SetCyclesSinceLastFailedOfMemEntry(Dem_NvBlockNumType MemoryIndex, uint16 CyclesSinceLastFailed);
#endif

/**
 * @brief         Find Index in Memory by EventId
 * @param[in]     MemoryInfo: Record memory information
 * @param[in]     EventId: Event Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Dem_NvBlockNumType Dem_MemoryFindIndex(Dem_MemStateInfoConstPtrType MemoryInfo, Dem_EventIdType EventId);

/**
 * @brief         Frees an event memory entry
 * @param[in]     MemoryInfo: Record memory information
 * @param[in]     MemoryIndex: Memory Index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_MemoryFreeIndex(Dem_MemStateInfoConstPtrType MemoryInfo, Dem_NvBlockNumType MemoryIndex);

/**
 * @brief         Selects a memory entry for an event
 * @param[in]     EventId: Event Id
 * @return        Dem_NvBlockNumType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Dem_NvBlockNumType Dem_AllocateMemory(Dem_EventIdType EventId);

#if (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_OFF)
/**
 * @brief         Selects a memory entry for an event's aging counter
 * @param[in]     MemoryInfo: Record memory information
 * @param[in]     EventId: Event Id
 * @return        Dem_NvBlockNumType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Dem_NvBlockNumType Dem_MemoryAllocateAging(Dem_MemStateInfoConstPtrType MemoryInfo, Dem_EventIdType EventId);
#endif

#if (DEM_EXTENDED_DATA_CLASS_NUMBER > 0u)
/**
 * @brief         Collects the user provided extended data records stored for an event entry.
 * @param[in]     MemoryIndex: Memory Index
 * @param[in]     EventId: Event Id
 * @param[in]     Trigger: The event update trigger
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL uint8 Dem_MemUpdateEventExtendData(Dem_EventIdType EventId, Dem_NvBlockNumType MemoryIndex, uint8 Trigger);
#endif

/**
 * @brief         Updates all data stored for an event.
 * Depending on configuration of the event data, collect the data to be updated due to the update trigger(s) in
 * Trigger. This includes statistical data (e.g. occurrence counter) as well as environment data collected from
 * callback routines.
 * Statistical data is stored separately in the MemoryEntry and is mapped into snapshot and extended records when
 * reading the data.
 * @param[in]     MemoryIndex: Memory Index
 * @param[in]     EventId: Event Id
 * @param[in]     Trigger: The event update trigger
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL uint8 Dem_MemUpdateEventEntry(Dem_EventIdType EventId, uint8 Trigger, Dem_NvBlockNumType MemoryIndex);

/**
 * @brief         Update the statistics of an an event entry.
 * @param[in]     MemoryIndex: Memory Index
 * @param[in]     EventId: Event Id
 * @param[in]     Occurrence: The flag of occurrence
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL uint8
    Dem_UpdateEventMemoryStatistics(Dem_EventIdType EventId, Dem_NvBlockNumType MemoryIndex, boolean Occurrence);

/**
 * @brief         updates Memory failure statistics like first and most recently failed event, first and most recently
 confirmed event.
 * @param[in]     EventId: Event Id
 * @param[in]     Trigger: The event update trigger
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_UpdateMemoryFailureStatistics(Dem_EventIdType EventId, uint8 Trigger);

#if (DEM_DEBOUNCE_COUNTER_STORAGE_NUM > 0u)
/**
 * @brief         Copies debounce values from debounce array to NVRAM mirror
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_MemStorageDataDebounce(void);
#endif

/**
 * @brief         Initializes backing memory
 * This function controls initialization of the memory. After the function completes, event memories are available.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_MemoryInit(void);

#if (DEM_FEATURE_NVM_IMMEDIATE == STD_ON)
/**
 * @brief         Processes NvM Task
 * The function controls NvM commits. Modified blocks are passed to the NvM to write back, and once the write operation
 * has finished the next modified block is processed.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_NvMTask(void);
#endif

/**
 * @brief         Initializes the NvM management
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_NvmInit(void);

#if (DEM_FREEZE_FRAME_REC_NUM_CLASS_NUMBER > 0u)
/**
 * @brief         get Freeze Frame Ptr of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory Index
 * @param[in]     FFEntryIndex: FreezeFrame Entry index
 * @return        uint8*
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8* Dem_GetFreezeFramePtrOfMemEntry(Dem_NvBlockNumType MemoryIndex, uint8 FFEntryIndex);

/**
 * @brief         get Freeze Frame Record Ptr of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory Index
 * @return        uint8*
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8* Dem_GetFreezeFrameRecordPtrOfMemEntry(Dem_NvBlockNumType MemoryIndex);
#endif

/**
 * @brief         Get the update state of the memory block.
 * @param[in]     MemoryIndex: Memory Index
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_MemoryUpdateGetState(Dem_NvBlockNumType MemoryIndex);

#if (DEM_FREEZE_FRAME_CLASS_NUMBER > 0u)
/**
 * @brief         Check if freeze frame is Stored
 * @param[in]     MemoryIndex: Memory Index
 * @param[in]     FFEntryIndex: FreezeFrame Entry index
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckFFStored(uint8 FFEntryIndex, Dem_NvBlockNumType MemoryIndex);
#endif

#if (DEM_EXTENDED_DATA_CLASS_NUMBER > 0u)
/**
 * @brief         get Extended Data Ptr of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory Index
 * @param[in]     EDEntryIndex: Extend data entry index ID
 * @return        uint8*
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8* Dem_GetExtendedDataPtrOfMemEntry(Dem_NvBlockNumType MemoryIndex, uint8 EDEntryIndex);
#endif

#if (defined(DEM_OCC2))
/**
 * @brief         get OCC2 of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory Index
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetOCC2OfMemEntry(Dem_NvBlockNumType MemoryIndex);

#if (defined(DEM_OCC2))
/**
 * @brief         Increment the occurrence counter # 2 in the memory entry, if it is lower than the maximum value (latch
 * at maximum value).
 * @param[in]     MemoryIndex: Memory Index
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_MemIncrementOCC2(Dem_NvBlockNumType MemoryIndex);
#endif
#endif

#if (defined(DEM_OCC4))
/**
 * @brief         get OCC4 of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory Index
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetOCC4OfMemEntry(Dem_NvBlockNumType MemoryIndex);
#endif

#if (defined(DEM_OCC5))
/**
 * @brief         get OCC5 of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory Index
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetOCC5OfMemEntry(Dem_NvBlockNumType MemoryIndex);
#endif

#if (defined(DEM_OCC6))
/**
 * @brief         get OCC6 of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory Index
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetOCC6OfMemEntry(Dem_NvBlockNumType MemoryIndex);

/**
 * @brief         Set OCC6 of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory Index
 * @param[in]     OCC6: operation cycle counter #6
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetOCC6OfMemEntry(Dem_NvBlockNumType MemoryIndex, uint8 OCC6);
#endif

#if (defined(DEM_SI30))
/**
 * @brief         get SI30 of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory Index
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetSI30FromMemEntry(Dem_NvBlockNumType MemoryIndex);

/**
 * @brief         Reset SI30 bit 0 of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory Index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_ResetSI30Bit0FromMemEntry(Dem_NvBlockNumType MemoryIndex);

/**
 * @brief         Reset SI30 bit 1 of MemEntry by MemoryIndex
 * @param[in]     EventId: Event Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_ResetSI30Bit1OfMemEntry(Dem_EventIdType EventId);

#if (DEM_OBD_DTC_NUMBER > 0)
/**
 * @brief         Update SI30 bit 6 of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory Index
 * @param[in]     EventId: Event Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_UpdateSI30BitOfMemEntry(Dem_NvBlockNumType MemoryIndex, Dem_EventIdType EventId);
#endif
#endif

#if (defined(DEM_OCCCTR))
/**
 * @brief         get OccurrenceCounter of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory Index
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetOCCOfMemEntry(Dem_NvBlockNumType MemoryIndex);

/**
 * @brief         Increment the occurrence counter in the memory entry, if it is lower than the maximum value (latch at
 * maximum value).
 * @param[in]     MemoryIndex: Memory Index
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_MemIncrementOCC(Dem_NvBlockNumType MemoryIndex);
#endif

#if (defined(DEM_FAILED_CYCLES))
/**
 * @brief         get FailedCycleCounter of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory Index
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetFailedCycleCounterOfMemEntry(Dem_NvBlockNumType MemoryIndex);
#endif

/**
 * @brief         get Memory Overflow of Admin Data
 * @param[in]     MemIndex: Memory Index
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_GetMemoryOverflow(Dem_MemoryNumType MemIndex);

/**
 * @brief         get current size of MemStateInfo
 * @param[in]     MemoryInfo: Record memory information
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetCurSizeOfMemoryInfo(Dem_MemStateInfoConstPtrType MemoryInfo);

#if ((DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_OFF) || (DEM_OBDII_SUPPORT == STD_ON))
/**
 * @brief         get state of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory Index
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetstateOfMemEntry(Dem_NvBlockNumType MemoryIndex);
#endif

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
    Dem_GetChronologyOfMemoryInfo(const Dem_MemStateInfoType* MemoryInfo, uint8 ChronoIndex);

/**
 * @brief         get MemEntry start of MemStateInfo
 * @param[in]     MemoryInfo: Record memory information
 * @return        Dem_NvBlockNumType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_NvBlockNumType Dem_GetMemEntryStartOfMemoryInfo(Dem_MemStateInfoConstPtrType MemoryInfo);

#if (DEM_EXTENDED_DATA_CLASS_NUMBER > 0u)
/**
 * @brief         Check if extended data is Stored
 * @param[in]     MemoryIndex: Memory Index
 * @param[in]     EDEntryIndex: Extend data entry index ID
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckEDStored(Dem_NvBlockNumType MemoryIndex, uint8 EDEntryIndex);

#if (DEM_MAX_SIZE_EXTENDED_DATA > 0u)
/**
 * @brief         get Extended Data Ptr of MemEntry by MemoryIndex and EntryIndex
 * @param[in]     MemoryIndex: Memory Index
 * @param[in]     EDEntryIndex: Extend data entry index ID
 * @param[in]     EntryIndex: Entry index ID
 * @return        uint8*
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8*
    Dem_GetExtendedDataPtrOfMemEntryByEntryIndex(Dem_NvBlockNumType MemoryIndex, uint8 EDEntryIndex, uint8 EntryIndex);
#endif
#endif

/**
 * @brief         get MemEntry End of MemStateInfo
 * @param[in]     MemoryInfo: Record memory information
 * @return        Dem_NvBlockNumType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_NvBlockNumType Dem_GetMemEntryEndOfMemoryInfo(Dem_MemStateInfoConstPtrType MemoryInfo);

#if ((DEM_OBDII_SUPPORT == STD_ON) && (DEM_MAX_SIZE_OBDFREEZEFRAME > 0u))
/**
 * @brief         Check if OBD freeze frame is Stored
 * @param[in]     MemoryIndex: Memory Index
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckOBDFFStored(Dem_NvBlockNumType MemoryIndex);
#endif

#if (DEM_MAX_SIZE_OBDFREEZEFRAME > 0u)
/**
 * @brief         get OBD Freeze Frame Ptr of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory Index
 * @return        uint8*
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8* Dem_GetOBDFreezeFramePtrOfMemEntry(Dem_NvBlockNumType MemoryIndex);
#endif

#if (DEM_MAX_SIZE_J1939FREEZEFRAME > 0u)
/**
 * @brief         get J1939 Freeze Frame Ptr of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory Index
 * @return        uint8*
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8* Dem_GetJ1939FreezeFramePtrOfMemEntry(Dem_NvBlockNumType MemoryIndex);
#endif

#if (DEM_MAX_SIZE_J1939EXFREEZEFRAME > 0u)
/**
 * @brief         get J1939Ex Freeze Frame Ptr of MemEntry by MemoryIndex
 * @param[in]     MemoryIndex: Memory Index
 * @return        uint8*
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8* Dem_GetJ1939ExFreezeFramePtrOfMemEntry(Dem_NvBlockNumType MemoryIndex);
#endif
/* PRQA S 5016 -- */
#endif
