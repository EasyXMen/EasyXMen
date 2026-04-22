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
 **  @file               : Dem_Debounce.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : Implementation file of Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_DEBOUNCE_H
#define DEM_DEBOUNCE_H

/* =================================================== inclusions =================================================== */
#include "Dem_EventInterface.h"
#include "Dem_SatelliteInterface.h"
#include "Dem_Error.h"
#include "Dem_EnableCondition.h"
#include "Dem_StorageCondition.h"
#include "Dem_Client.h"

/* ===================================================== macros ===================================================== */
/**
 * @name DEM_DEBOUNCSTATE_ENCODE
 * @{
 * @brief Macros encoding the state of a debouncing object
 */
#define DEM_DEBOUNCSTATE_INITIAL              (0x00u) /**< Initial state value */
#define DEM_DEBOUNCSTATE_DEBOUNCINGCHANGED    (0x01u) /**< Status change indication */
#define DEM_DEBOUNCSTATE_REQUESTFDCPROCESSING (0x02u) /**< FDC Status change indication */
#define DEM_DEBOUNCSTATE_MAXIMUMFDCEXCEEDED   (0x04u) /**< FDC maximum exceeded */
#define DEM_DEBOUNCSTATE_FDCTHRESHOLDEXCEEDED (0x08u) /**< FDC threshold exceeded */
/** @} */

/**
 * @name DEM_DEBOUNCETIME_ENCODE
 * @{
 * @brief Macros encoding debounce status states
 */
#define DEM_DEBOUNCETIME_NONE            (0x0u) /**< Debouncing none */
#define DEM_DEBOUNCETIME_RESETFAILED     (0x1u) /**< Debouncing started, counter initialized towards failed threshold */
#define DEM_DEBOUNCETIME_RESETPASSED     (0x2u) /**< Debouncing started, counter initialized towards passed threshold */
#define DEM_DEBOUNCETIME_COUNTFAILED     (0x3u) /**< Debouncing in progress, counting towards failed threshold */
#define DEM_DEBOUNCETIME_COUNTPASSED     (0x4u) /**< Debouncing in progress, counting towards passed threshold */
#define DEM_DEBOUNCETIME_FROZENFAILED    (0x5u) /**< Debouncing paused while counting towards failed threshold */
#define DEM_DEBOUNCETIME_FROZENPASSED    (0x6u) /**< Debouncing paused while counting towards passed threshold */
#define DEM_DEBOUNCETIME_COUNTFAILEDFDC  (0x7u) /**< Debouncing in progress, counting towards failed threshold, FDC */
#define DEM_DEBOUNCETIME_FROZENFAILEDFDC (0x9u) /**< Debouncing paused while counting towards failed threshold, FDC */
#define DEM_DEBOUNCETIME_DONEPASSED      (0xau) /**< Debouncing completed, passed threshold reached */
#define DEM_DEBOUNCETIME_DONEFAILED      (0xbu) /**< Debouncing completed, failed threshold reached */
/** @} */

/**
 * @name DEM_DEBOUNCEMONITORINTERNAL_ENCODE
 * @{
 * @brief Macros encoding qualified monitor status reports
 */
#define DEM_DEBOUNCEMONITORINTERNAL_NONE                (0x00u) /**< No result */
#define DEM_DEBOUNCEMONITORINTERNAL_FAILED              (0x01u) /**< Failed result */
#define DEM_DEBOUNCEMONITORINTERNAL_PASSED              (0x02u) /**< Passed result */
#define DEM_DEBOUNCEMONITORINTERNAL_FDCTHRESHOLDREACHED (0x03u) /**< Fdc threshold reached result */
/** @} */

/**
 * @name DEM_FDC_ENCODE
 * @{
 * @brief Macros encoding FDC
 */
#define DEM_FDC_INIT_VALUE ((sint8)0)      /**< Init FDC */
#define DEM_FDC_MAX_VALUE  ((sint8)127)    /**< Max FDC */
#define DEM_FDC_MIN_VALUE  ((sint8)(-128)) /**< Min FDC */
/** @} */

#define DEM_DEBOUNCE_INIT_VALUE ((sint16)0) /**< Init debounce value */

/* PRQA S 5004 ++ */ /* VL_Dem_5004 */
/* ================================================ type definitions ================================================ */
/**
 * @brief Debouncing function pointer table
 */
typedef struct Dem_DebounceFunctionTableType_s Dem_DebounceFunctionTableType;
/**
 * @brief Non-writable pointer to debouncing function pointer table
 */
typedef const Dem_DebounceFunctionTableType* Dem_DebounceFunctionTablePtrType;

/**
 * @brief Debounce algorithm management data.
 * Data common to all debounce algorithms.
 */
typedef struct
{
    Dem_DebounceFunctionTablePtrType FunctionTable; /**< Function pointer table for the debouncing type @range NA */
    Dem_EventIdType                  EventId;       /**< debounced event id @range 0..65535 */
    Dem_EventStatusType MonitorTrigger; /**< Reported/Effective status of the application monitor @range 0..255 */
    uint8 State; /**< flag definitions: see Macros encoding the state of a debouncing object. @range 0..255 */
} Dem_BaseInfoType;

/**
 * @brief Pointer of debounce algorithm management data.
 * Data common to all debounce algorithms.
 */
typedef Dem_BaseInfoType* Dem_BaseInfoPtrType;

/**
 * @brief Const pointer of debounce algorithm management data.
 * Data common to all debounce algorithms.
 */
typedef const Dem_BaseInfoType* Dem_BaseInfoConstPtrType;

/**
 * @brief Const pointer of debounce algorithm management data.
 * Data common to all debounce algorithms.
 */
typedef const Dem_BaseInfoType* const Dem_BaseInfoConstPtrConstType;

/**
 * @brief         Debouncing function pointer for functions without return value
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
typedef void (*Dem_DebounceBase_VoidFunctionPtr)(Dem_BaseInfoConstPtrConstType BaseInfoPtr);

#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
/**
 * @brief         Debouncing function pointer for functions without return value
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
typedef void (*Dem_DebounceBase_UpdateMaxFunctionPtr)(Dem_BaseInfoConstPtrConstType BaseInfoPtr);
#endif

/**
 * @brief         Debouncing function pointer for functions with boolean return value
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
typedef boolean (*Dem_DebounceBase_BoolFunctionPtr)(Dem_BaseInfoPtrType BaseInfoPtr);

/**
 * @brief         Debouncing function pointer to commit debouncing result
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @param[in]     SatelliteData: Satellite Data
 * @return        Dem_SatelliteDataType
 * @retval        Data: The detailed data type.
 * @retval        Raw: The raw data type intended for queue communication.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
typedef Dem_SatelliteDataType (*Dem_DebounceBase_ResultFunctionPtr)(
    Dem_BaseInfoConstPtrConstType BaseInfoPtr,
    Dem_SatelliteDataType         SatelliteData);

/**
 * @brief         Debouncing function pointer for functions with debouncing state return value
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @return        Dem_DebouncingStateType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
typedef Dem_DebouncingStateType (*Dem_DebounceBase_StateFunctionPtr)(Dem_BaseInfoConstPtrConstType BaseInfoPtr);

/**
 * @brief         Debouncing function pointer for functions returning an FDC value
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @param[out]    FDC: Pointer to the FDC
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
typedef Std_ReturnType (*Dem_DebounceBase_GetFDCFunctionPtr)(Dem_BaseInfoConstPtrConstType BaseInfoPtr, sint8* FDC);

/**
 * @brief         Debouncing function pointer to restoring a debounce value
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @param[in]     SatelliteData: Satellite Data
 * @param[in]     Value: Process value
 * @param[in]     ExpectedThreshold: The expect threshold
 * @return        Dem_SatelliteDataType
 * @retval        Data: The detailed data type.
 * @retval        Raw: The raw data type intended for queue communication.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
typedef Dem_SatelliteDataType (*Dem_DebounceBase_RestoreFunctionPtr)(
    Dem_BaseInfoConstPtrConstType BaseInfoPtr,
    Dem_SatelliteDataType         SatelliteData,
    sint16                        Value,
    boolean                       ExpectedThreshold);

/**
 * @brief Debouncing function pointer table
 */
struct Dem_DebounceFunctionTableType_s
{
    Dem_DebounceBase_BoolFunctionPtr  Calculate;          /**< Debouncing calculation function @range NA */
    Dem_DebounceBase_VoidFunctionPtr  Freeze;             /**< freeze debouncing function @range NA */
    Dem_DebounceBase_VoidFunctionPtr  Reset;              /**< reset debouncing function @range NA */
    Dem_DebounceBase_StateFunctionPtr GetDebouncingState; /**< get the current debouncing state @range NA */
#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
    Dem_DebounceBase_UpdateMaxFunctionPtr
        UpdateDebounceValueMax;                                  /**< updates the maximum debouncing value. @range NA */
    Dem_DebounceBase_UpdateMaxFunctionPtr ResetDebounceValueMax; /**< resets the maximum debouncing value. @range NA */
#endif
    Dem_DebounceBase_GetFDCFunctionPtr GetFaultDetectionCounter; /**< Returns the fault detection counter. @range NA */
#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
    Dem_DebounceBase_GetFDCFunctionPtr
        GetMaxFaultDetectionCounter; /**< Returns the maximum fault detection counter. @range NA */
#endif
    Dem_DebounceBase_ResultFunctionPtr  GetDebouncingResult;  /**< Update and return debouncing result @range NA */
    Dem_DebounceBase_RestoreFunctionPtr RestoreDebounceValue; /**< restoring debouncing result function @range NA */
};

/**
 * @brief Data for counter-based debouncing
 */
typedef struct
{
    Dem_BaseInfoType Base;             /**< Base class object. This attribute must come first! @range NA */
    sint16           OldDebounceValue; /**< [in] debounce counter before processing @range -32768..32767 */
    sint16           NewDebounceValue; /**< [out] debounce counter after processing @range -32768..32767 */
} Dem_CounterBasedInfoType;

/**
 * @brief Data for monitor internal debouncing
 */
typedef struct
{
    Dem_BaseInfoType Base;             /**< Base class object. This attribute must come first! @range NA */
    uint8            OldMonitorStatus; /**< [in] Last monitor result before processing @range 0..255 */
    uint8            NewMonitorStatus; /**< [out] Last monitor result after processing @range 0..255 */
} Dem_MonitorInternalInfoType;

/**
 * @brief Data for time-based debouncing
 */
typedef struct
{
    Dem_BaseInfoType Base;          /**< Base class object. This attribute must come first! @range NA */
    sint16           OldTimerValue; /**< [in] debounce timer before processing @range -32768..32767 */
    sint16           NewTimerValue; /**< [out] debounce timer after processing @range -32768..32767 */
    uint8            OldTimerState; /**< [in] debounce state before processing @range 0..255 */
    uint8            NewTimerState; /**< [out] debounce state after processing @range 0..255 */
} Dem_TimeBasedInfoType;

/**
 * @brief General debounce type.
 */
typedef union
{ /* PRQA S 0750 */                              /* VL_Dem_0750 */
    Dem_CounterBasedInfoType    CounterBased;    /**< counter based debouncing. @range NA */
    Dem_MonitorInternalInfoType MonitorInternal; /**< monitor internal debouncing. @range NA */
    Dem_TimeBasedInfoType       TimeBased;       /**< time based debouncing. @range NA */
} Dem_DebounceInfoType;

/**
 * @brief Pointer to debounce Counter algorithm management data
 */
typedef Dem_CounterBasedInfoType* Dem_CounterBasedInfoPtrType;

/**
 * @brief Pointer to debounce Monitor Internal algorithm management data
 */
typedef Dem_MonitorInternalInfoType* Dem_MonitorInternalInfoPtrType;

/**
 * @brief Pointer to debounce Time algorithm management data
 * */
typedef Dem_TimeBasedInfoType* Dem_TimeBasedInfoPtrType;

/**
 * @brief Const Pointer to debounce Counter algorithm management data
 */
typedef const Dem_CounterBasedInfoType* const Dem_CounterBasedInfoConstPtrType;

/**
 * @brief Const Pointer to debounce Monitor Internal algorithm management data
 */
typedef const Dem_MonitorInternalInfoType* const Dem_MonitorInternalInfoConstPtrType;

/**
 * @brief Const Pointer to debounce Time algorithm management data
 * */
typedef const Dem_TimeBasedInfoType* const Dem_TimeBasedInfoConstPtrType;

/**
 * @brief         Function pointer type for Debounce Time Calcul Function
 * @param[in]     DebounceTimePtr: Pointer to the debounce time information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
typedef void (*Dem_DebounceTimeCalculFunctionPtr)(Dem_TimeBasedInfoPtrType DebounceTimePtr);

/**
 * @brief Function pointer type for Debounce Counter Calcul Function
 * @param[in]     DebounceCounterPtr: Pointer to the debounce counter information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
typedef void (*Dem_DebounceCounterCalculFunctionPtr)(Dem_CounterBasedInfoPtrType DebounceCounterPtr);
/* PRQA S 5004 -- */

/* ========================================== internal function declarations ======================================== */
#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
/**
 * @brief         Call function to Update Debounce Value Max
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DebounceUpdateDebounceValueMax(Dem_BaseInfoConstPtrConstType BaseInfoPtr);

/**
 * @brief         Call function to resets the maximum debouncing value
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DebounceResetDebounceValueMax(Dem_BaseInfoConstPtrConstType BaseInfoPtr);
#endif

/**
 * @brief         Call function to Returns the fault detection counter.
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @param[out]    FDC: Pointer to the FDC
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_DebounceGetFaultDetectionCounter(Dem_BaseInfoConstPtrConstType BaseInfoPtr, sint8* FDC);

#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
/**
 * @brief         Call function to Update Debounce Value Max
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @param[out]    FDC: Pointer to the FDC
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_DebounceGetMaxFaultDetectionCounter(Dem_BaseInfoConstPtrConstType BaseInfoPtr, sint8* FDC);
#endif

#if (defined(DEM_EVENT_COUNTER_DEBOUNCCE))
/**
 * @brief         Debounce counter initialization
 * @param[in]     EventId: The event ID
 * @param[in]     DebounceValue: Debounce value
 * @return        sint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL sint8 Dem_CalculateCounterFDC(Dem_EventIdType EventId, sint16 DebounceValue);

/**
 * @brief         Debounce counter Calculate of Prepassed function
 * @param[in]     DebounceCounterPtr: Pointer to the debounce counter information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DebCounterCalculatePrepassed(Dem_CounterBasedInfoPtrType DebounceCounterPtr);

/**
 * @brief         Debounce counter Calculate of passed function
 * @param[in]     DebounceCounterPtr: Pointer to the debounce counter information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DebCounterCalculatePassed(Dem_CounterBasedInfoPtrType DebounceCounterPtr);

/**
 * @brief         Debounce counter Calculate of Prefailed function
 * @param[in]     DebounceCounterPtr: Pointer to the debounce counter information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DebCounterCalculatePrefailed(Dem_CounterBasedInfoPtrType DebounceCounterPtr);

/**
 * @brief         Debounce counter Calculate of failed function
 * @param[in]     DebounceCounterPtr: Pointer to the debounce counter information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DebCounterCalculateFailed(Dem_CounterBasedInfoPtrType DebounceCounterPtr);

/* PRQA S 5016 ++ */ /* VL_Dem_5016 */
/**
 * @brief         Debounce counter initialization
 * @param[in]     DebounceCounterPtr: Pointer to the debounce counter information
 * @param[in]     EventId: The event ID
 * @param[in]     SatelliteData: Satellite data
 * @return        Dem_BaseInfoPtrType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_BaseInfoPtrType Dem_DebounceCounterInit(
    Dem_CounterBasedInfoType* const DebounceCounterPtr,
    Dem_EventIdType                 EventId,
    Dem_SatelliteDataType           SatelliteData);

/**
 * @brief         Debounce counter Calculate function
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_DebounceCounterCalculate(Dem_BaseInfoPtrType BaseInfoPtr);

/**
 * @brief         Debounce counter Freeze function
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DebounceCounterFreeze(Dem_BaseInfoConstPtrConstType BaseInfoPtr);

/**
 * @brief         Debounce counter Reset function
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DebounceCounterReset(Dem_BaseInfoConstPtrConstType BaseInfoPtr);

/**
 * @brief         Debounce counter Get Debouncing State function
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @return        Dem_DebouncingStateType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Dem_DebouncingStateType Dem_DebounceCounterGetDebouncingState(Dem_BaseInfoConstPtrConstType BaseInfoPtr);

#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
/**
 * @brief         Debounce counter Update Debounce Value Max function
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DebounceCounterUpdateDebounceValueMax(Dem_BaseInfoConstPtrConstType BaseInfoPtr);

/**
 * @brief       Debounce counter Reset Debounce Value Max function
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DebounceCounterResetDebounceValueMax(Dem_BaseInfoConstPtrConstType BaseInfoPtr);
#endif

/**
 * @brief         Debounce counter Get Fault Detection Counter function
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @param[out]    FDC: Pointer to the FDC
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType
    Dem_DebounceCounterGetFaultDetectionCounter(Dem_BaseInfoConstPtrConstType BaseInfoPtr, sint8* FDC);

#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
/**
 * @brief         Debounce counter Get Max Fault Detection Counter function
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @param[out]    FDC: Pointer to the FDC
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType
    Dem_DebounceCounterGetMaxFaultDetectionCounter(Dem_BaseInfoConstPtrConstType BaseInfoPtr, sint8* FDC);
#endif

/**
 * @brief         Debounce counter Get Debouncing Result function
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @param[in]     SatelliteData: Satellite Data
 * @return        Dem_SatelliteDataType
 * @retval        Data: The detailed data type.
 * @retval        Raw: The raw data type intended for queue communication.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Dem_SatelliteDataType Dem_DebounceCounterGetDebouncingResult(
    Dem_BaseInfoConstPtrConstType BaseInfoPtr,
    Dem_SatelliteDataType         SatelliteData);

/**
 * @brief         Debounce counter Restore Debounce Value function
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @param[in]     SatelliteData: Satellite Data
 * @param[in]     Value: Process value
 * @param[in]     ExpectedThreshold: The expect threshold
 * @return        Dem_SatelliteDataType
 * @retval        Data: The detailed data type.
 * @retval        Raw: The raw data type intended for queue communication.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Dem_SatelliteDataType Dem_DebounceCounterRestoreDebounceValue(
    Dem_BaseInfoConstPtrConstType BaseInfoPtr,
    Dem_SatelliteDataType         SatelliteData,
    sint16                        Value,
    boolean                       ExpectedThreshold);
#endif

#if (defined(DEM_EVENT_TIME_DEBOUNCCE))
/**
 * @brief         Debounce time Calculate Fault Detection Counter
 * @param[in]     EventId: The event ID
 * @param[in]     DebounceValue: Debounce value
 * @param[in]     DebounceStatus: Debounce status value
 * @return        sint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL sint8 Dem_CalculateTimeFDC(Dem_EventIdType EventId, sint16 DebounceValue, uint8 DebounceStatus);

/**
 * @brief         Debounce time Calculate of Prepassed function
 * @param[in]     DebounceTimePtr: Pointer to the debounce time information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DebTimeCalculatePrepassed(Dem_TimeBasedInfoPtrType DebounceTimePtr);

/**
 * @brief         Debounce time Calculate of passed function
 * @param[in]     DebounceTimePtr: Pointer to the debounce time information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DebTimeCalculatePrefailed(Dem_TimeBasedInfoPtrType DebounceTimePtr);

/**
 * @brief         Debounce time Calculate of Prefailed function
 * @param[in]     DebounceTimePtr: Pointer to the debounce time information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DebTimeCalculatePassed(Dem_TimeBasedInfoPtrType DebounceTimePtr);

/**
 * @brief         Debounce time Calculate of failed function
 * @param[in]     DebounceTimePtr: Pointer to the debounce time information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DebTimeCalculateFailed(Dem_TimeBasedInfoPtrType DebounceTimePtr);

/**
 * @brief         Debounce time initialization
 * @param[in]     DebounceTimePtr: Pointer to the debounce time information
 * @param[in]     EventId: The event ID
 * @param[in]     SatelliteData: Satellite Data
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_BaseInfoPtrType Dem_DebounceTimeInit(
    Dem_TimeBasedInfoType* const DebounceTimePtr,
    Dem_EventIdType              EventId,
    Dem_SatelliteDataType        SatelliteData);

/**
 * @brief         Debounce Time Calculate function
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_DebounceTimeCalculate(Dem_BaseInfoPtrType BaseInfoPtr);

/**
 * @brief         Debounce Time Freeze function
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DebounceTimeFreeze(Dem_BaseInfoConstPtrConstType BaseInfoPtr);

/**
 * @brief         Debounce Time Reset function
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DebounceTimeReset(Dem_BaseInfoConstPtrConstType BaseInfoPtr);

/**
 * @brief         Debounce Time Reset function
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @return        Dem_DebouncingStateType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Dem_DebouncingStateType Dem_DebounceTimeGetDebouncingState(Dem_BaseInfoConstPtrConstType BaseInfoPtr);

#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
/**
 * @brief         Debounce Time Update Debounce Value Max function
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DebounceTimeUpdateDebounceValueMax(Dem_BaseInfoConstPtrConstType BaseInfoPtr);

/**
 * @brief         Debounce Time Reset Debounce Value Max function
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DebounceTimeResetDebounceValueMax(Dem_BaseInfoConstPtrConstType BaseInfoPtr);
#endif
/**
 * @brief         Debounce Time Get Fault Detection Counter function
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @param[out]    FDC: Pointer to the FDC
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType
    Dem_DebounceTimeGetFaultDetectionCounter(Dem_BaseInfoConstPtrConstType BaseInfoPtr, sint8* FDC);

#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
/**
 * @brief         Debounce Time Get Max Fault Detection Counter function
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @param[out]    FDC: Pointer to the FDC
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType
    Dem_DebounceTimeGetMaxFaultDetectionCounter(Dem_BaseInfoConstPtrConstType BaseInfoPtr, sint8* FDC);
#endif

/**
 * @brief         Debounce Time Get Debouncing Result function
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @param[in]     SatelliteData: Satellite Data
 * @return        Dem_SatelliteDataType
 * @retval        Data: The detailed data type.
 * @retval        Raw: The raw data type intended for queue communication.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Dem_SatelliteDataType
    Dem_DebounceTimeGetDebouncingResult(Dem_BaseInfoConstPtrConstType BaseInfoPtr, Dem_SatelliteDataType SatelliteData);

/**
 * @brief         Debounce Time Counting Towards Passed
 * @param[in]     DebounceTimePtr: Pointer to the debounce time information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DebounceTimeCountingTowardsPassed(const Dem_TimeBasedInfoPtrType DebounceTimePtr);

/**
 * @brief         Debounce Time Counting Towards Failed
 * @param[in]     DebounceTimePtr: Pointer to the debounce time information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DebounceTimeCountingTowardsFailed(const Dem_TimeBasedInfoPtrType DebounceTimePtr);

/**
 * @brief         Debounce Time Counting Towards Failed With FDC Reached
 * @param[in]     DebounceTimePtr: Pointer to the debounce time information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DebounceTimeCountingTowardsFailedWithFDCReached(const Dem_TimeBasedInfoPtrType DebounceTimePtr);

/**
 * @brief         Debounce Time Process Debounce
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DebounceTimeProcessDebounce(Dem_BaseInfoConstPtrConstType BaseInfoPtr);

/**
 * @brief         Check Debounce Time is active
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_DebounceTimeIsTimerActive(Dem_BaseInfoConstPtrConstType BaseInfoPtr);
#endif

#if (defined(DEM_EVENT_MONOTOR_DEBOUNCCE))
/**************Debounce Monitor Internal***************/
/**
 * @brief Debounce Monitor Internal initialization
 * @param[in]     MonitorInternalPtr: Pointer to the monitor internal information
 * @param[in]     EventId: The event ID
 * @param[in]     SatelliteData: Satellite Data
 * @return        Dem_BaseInfoPtrType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_BaseInfoPtrType Dem_DebounceMonitorInternalInit(
    Dem_MonitorInternalInfoType* const MonitorInternalPtr,
    Dem_EventIdType                    EventId,
    Dem_SatelliteDataType              SatelliteData);

/**
 * @brief         Debounce Monitor Internal Calculate function
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_DebounceMonitorInternalCalculate(Dem_BaseInfoPtrType BaseInfoPtr);

/**
 * @brief         Debounce Monitor Internal Freeze function
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DebounceMonitorInternalFreeze(Dem_BaseInfoConstPtrConstType BaseInfoPtr);
/**
 * @brief         Debounce Monitor Internal Reset function
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DebounceMonitorInternalReset(Dem_BaseInfoConstPtrConstType BaseInfoPtr);

/**
 * @brief         Debounce Monitor Internal Get Debouncing State function
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @return        Dem_DebouncingStateType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Dem_DebouncingStateType
    Dem_DebounceMonitorInternalGetDebouncingState(Dem_BaseInfoConstPtrConstType BaseInfoPtr);

#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
/**
 * @brief         Debounce Monitor Internal Update Debounce Value Max function
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DebounceMonitorInternalUpdateDebounceValueMax(Dem_BaseInfoConstPtrConstType BaseInfoPtr);

/**
 * @brief         Debounce Monitor Internal Reset Debounce Value Max function
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DebounceMonitorInternalResetDebounceValueMax(Dem_BaseInfoConstPtrConstType BaseInfoPtr);
#endif

/**
 * @brief         Debounce Monitor Internal Get Fault Detection Counter function
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @param[out]    FDC: Pointer to the FDC
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType
    Dem_DebounceMonitorInternalGetFaultDetectionCounter(Dem_BaseInfoConstPtrConstType BaseInfoPtr, sint8* FDC);

#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
/**
 * @brief         Debounce Monitor Internal Get Max Fault Detection Counter function
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @param[out]    FDC: Pointer to the FDC
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType
    Dem_DebounceMonitorInternalGetMaxFaultDetectionCounter(Dem_BaseInfoConstPtrConstType BaseInfoPtr, sint8* FDC);
#endif

/**
 * @brief         Debounce Monitor Internal Get Debouncing Result function
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @param[in]     SatelliteData: Satellite Data
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Dem_SatelliteDataType Dem_DebounceMonitorInternalGetDebouncingResult(
    Dem_BaseInfoConstPtrConstType BaseInfoPtr,
    Dem_SatelliteDataType         SatelliteData);
#endif

/* ============================================ internal data definitions =========================================== */
#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
/* PRQA S 3218 ++ */ /* VL_Dem_3218 */
#if (defined(DEM_EVENT_COUNTER_DEBOUNCCE))
/**
 * @brief Debounce Counter Function Table
 * @range NA
 */
DEM_LOCAL const Dem_DebounceFunctionTableType Dem_DebounceCounterFunctionTable = {
    Dem_DebounceCounterCalculate,
    Dem_DebounceCounterFreeze,
    Dem_DebounceCounterReset,
    Dem_DebounceCounterGetDebouncingState,
#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
    Dem_DebounceCounterUpdateDebounceValueMax,
    Dem_DebounceCounterResetDebounceValueMax,
#endif
    Dem_DebounceCounterGetFaultDetectionCounter,
#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
    Dem_DebounceCounterGetMaxFaultDetectionCounter,
#endif
    Dem_DebounceCounterGetDebouncingResult,
    Dem_DebounceCounterRestoreDebounceValue,
};
#endif

#if (defined(DEM_EVENT_TIME_DEBOUNCCE))
/**
 * @brief Debounce Time Function Table
 * @range NA
 */
DEM_LOCAL const Dem_DebounceFunctionTableType Dem_DebounceTimeFunctionTable = {
    Dem_DebounceTimeCalculate,
    Dem_DebounceTimeFreeze,
    Dem_DebounceTimeReset,
    Dem_DebounceTimeGetDebouncingState,
#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
    Dem_DebounceTimeUpdateDebounceValueMax,
    Dem_DebounceTimeResetDebounceValueMax,
#endif
    Dem_DebounceTimeGetFaultDetectionCounter,
#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
    Dem_DebounceTimeGetMaxFaultDetectionCounter,
#endif
    Dem_DebounceTimeGetDebouncingResult,
    NULL_PTR,
};
#endif

#if (defined(DEM_EVENT_MONOTOR_DEBOUNCCE))
/**
 * @brief Debounce Monitor Internal Function Table
 * @range NA
 */
DEM_LOCAL const Dem_DebounceFunctionTableType Dem_DebounceMonitorInternalFunctionTable = {
    Dem_DebounceMonitorInternalCalculate,
    Dem_DebounceMonitorInternalFreeze,
    Dem_DebounceMonitorInternalReset,
    Dem_DebounceMonitorInternalGetDebouncingState,
#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
    Dem_DebounceMonitorInternalUpdateDebounceValueMax,
    Dem_DebounceMonitorInternalResetDebounceValueMax,
#endif
    Dem_DebounceMonitorInternalGetFaultDetectionCounter,
#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
    Dem_DebounceMonitorInternalGetMaxFaultDetectionCounter,
#endif
    Dem_DebounceMonitorInternalGetDebouncingResult,
    NULL_PTR,
};
#endif
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
/* PRQA S 3218 -- */

/* ========================================== internal function definitions ========================================= */
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"

#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
/**
 * @brief         Set Satellite data
 * @param[in]     SatelliteId: Satellite ID
 * @param[in]     MonitorId: Monitor ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_MaxDebValPreInit(Dem_SatelliteIdType SatelliteId, Dem_MonitorIdType MonitorId)
{
    Dem_GetMaxDebValOfSatellite(SatelliteId)[MonitorId] = 0u;
}

/**
 * @brief         Get Satellite info Debounce Value Max
 * @param[in]     EventId: Event ID
 * @return        sint16
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE sint16 Dem_GetDebounceValueMaxByEvent(Dem_EventIdType EventId)
{
    Dem_SatelliteIdType satelliteId = Dem_GetSatelliteIdByEventId(EventId);
    return (sint16)Dem_GetMaxDebValOfSatellite(satelliteId)[Dem_GetMonitorIdByEventId(EventId)];
}

/**
 * @brief         Set Satellite info Debounce Value Max
 * @param[in]     EventId: Event ID
 * @param[in]     DebounceValue: Debounce value
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetDebounceValueMaxByEvent(Dem_EventIdType EventId, sint16 DebounceValue)
{
    Dem_SatelliteIdType satelliteId                                              = Dem_GetSatelliteIdByEventId(EventId);
    Dem_GetMaxDebValOfSatellite(satelliteId)[Dem_GetMonitorIdByEventId(EventId)] = (uint16)DebounceValue;
}
#endif

/* PRQA S 4404 ++ */ /* VL_Dem_4404 */
#if (DEM_DEBOUNCE_COUNTER_STORAGE_NUM > 0u)
/**
 * @brief         Get DemDebounceCounterStorage based on event ID
 * @param[in]     EventId: Event ID
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_GetDebounceStorageByEvent(Dem_EventIdType EventId)
{
    return (boolean)Dem_GetStorageOfDB(Dem_GetDebounceRefOfEvent(EventId));
}
#endif

/**
 * @brief Get Debounce based type on event ID
 * @param[in]     EventId: Event ID
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetDebounceTypeByEventId(Dem_EventIdType EventId)
{
    return (uint8)(Dem_GetTypeOfDB(Dem_GetDebounceRefOfEvent(EventId)));
}

/**
 * @brief         Check Debouncing Changed
 * @param[in]     DebounceInfoPtr: Pointer to debounce information
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckDebouncingChanged(Dem_BaseInfoConstPtrConstType DebounceInfoPtr)
{
    return DEM_FLAG_ISSET(DebounceInfoPtr->State, DEM_DEBOUNCSTATE_DEBOUNCINGCHANGED);
}

/**
 * @brief         Set Debouncing Changed
 * @param[in]     DebounceInfoPtr: Pointer to debounce information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetDebouncingChanged(Dem_BaseInfoPtrType DebounceInfoPtr)
{
    DEM_FLAG_SET(DebounceInfoPtr->State, DEM_DEBOUNCSTATE_DEBOUNCINGCHANGED); /* PRQA S 3473 */ /* VL_Dem_3473 */
}

/**
 * @brief         Check Request Fdc Processing
 * @param[in]     DebounceInfoPtr: Pointer to debounce information
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckRequestFdcProcessing(Dem_BaseInfoConstPtrConstType DebounceInfoPtr)
{
    return DEM_FLAG_ISSET(DebounceInfoPtr->State, DEM_DEBOUNCSTATE_REQUESTFDCPROCESSING);
}

/**
 * @brief         Set Request Fdc Processing
 * @param[in]     DebounceInfoPtr: Pointer to debounce information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetRequestFdcProcessing(Dem_BaseInfoPtrType DebounceInfoPtr)
{
    DEM_FLAG_SET(DebounceInfoPtr->State, DEM_DEBOUNCSTATE_REQUESTFDCPROCESSING); /* PRQA S 3473 */ /* VL_Dem_3473 */
}

/**
 * @brief         Check Maximum Fdc Exceeded
 * @param[in]     DebounceInfoPtr: Pointer to debounce information
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckMaximumFdcExceeded(Dem_BaseInfoConstPtrConstType DebounceInfoPtr)
{
    return DEM_FLAG_ISSET(DebounceInfoPtr->State, DEM_DEBOUNCSTATE_MAXIMUMFDCEXCEEDED);
}

/**
 * @brief         Set Maximum Fdc Exceeded
 * @param[in]     DebounceInfoPtr: Pointer to debounce information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetMaximumFdcExceeded(Dem_BaseInfoPtrType DebounceInfoPtr)
{
    DEM_FLAG_SET(DebounceInfoPtr->State, DEM_DEBOUNCSTATE_MAXIMUMFDCEXCEEDED); /* PRQA S 3473 */ /* VL_Dem_3473 */
}

/**
 * @brief         Check Fdc Threshold Exceeded
 * @param[in]     DebounceInfoPtr: Pointer to debounce information
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckFdcThresholdExceeded(Dem_BaseInfoConstPtrConstType DebounceInfoPtr)
{
    return DEM_FLAG_ISSET(DebounceInfoPtr->State, DEM_DEBOUNCSTATE_FDCTHRESHOLDEXCEEDED);
}

/**
 * @brief         Set Fdc Threshold Exceeded
 * @param[in]     DebounceInfoPtr: Pointer to debounce information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetFdcThresholdExceeded(Dem_BaseInfoPtrType DebounceInfoPtr)
{
    DEM_FLAG_SET(DebounceInfoPtr->State, DEM_DEBOUNCSTATE_FDCTHRESHOLDEXCEEDED); /* PRQA S 3473 */ /* VL_Dem_3473 */
}

/**
 * @brief         Set Temporarily Defective
 * @param[in]     DebounceStatus: Debounce status value
 * @return        Dem_DebouncingStateType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_DebouncingStateType Dem_SetTemporarilyDefective(Dem_DebouncingStateType DebounceStatus)
{
    Dem_DebouncingStateType state = DebounceStatus;
    DEM_FLAG_SET(state, DEM_TEMPORARILY_DEFECTIVE); /* PRQA S 3473 */ /* VL_Dem_3473 */
    return state;
}

/**
 * @brief         Set finally Defective
 * @param[in]     DebounceStatus: Debounce status value
 * @return        Dem_DebouncingStateType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_DebouncingStateType Dem_SetFinallyDefective(Dem_DebouncingStateType DebounceStatus)
{
    Dem_DebouncingStateType state = DebounceStatus;
    DEM_FLAG_SET(state, DEM_FINALLY_DEFECTIVE); /* PRQA S 3473 */ /* VL_Dem_3473 */
    return state;
}

/**
 * @brief         Set temporarily healed
 * @param[in]     DebounceStatus: Debounce status value
 * @return        Dem_DebouncingStateType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_DebouncingStateType Dem_SetTemporarilyHealed(Dem_DebouncingStateType DebounceStatus)
{
    Dem_DebouncingStateType state = DebounceStatus;
    DEM_FLAG_SET(state, DEM_TEMPORARILY_HEALED); /* PRQA S 3473 */ /* VL_Dem_3473 */
    return state;
}

/**
 * @brief         Set Test complete
 * @param[in]     DebounceStatus: Debounce status value
 * @return        Dem_DebouncingStateType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_DebouncingStateType Dem_SetTestComplete(Dem_DebouncingStateType DebounceStatus)
{
    Dem_DebouncingStateType state = DebounceStatus;
    DEM_FLAG_SET(state, DEM_TEST_COMPLETE); /* PRQA S 3473 */ /* VL_Dem_3473 */
    return state;
}
/* PRQA S 4404 -- */

/**
 * @brief         Set DTR Update
 * @param[in]     DebounceStatus: Debounce status value
 * @return        Dem_DebouncingStateType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_DebouncingStateType Dem_SetDtrUpdate(Dem_DebouncingStateType DebounceStatus)
{
    Dem_DebouncingStateType state = DebounceStatus;
    DEM_FLAG_SET(state, DEM_DTR_UPDATE); /* PRQA S 3473 */ /* VL_Dem_3473 */
    return state;
}

/* PRQA S 0759 ++ */ /* VL_Dem_0759 */
/**
 * @brief         Debounce Info initialization
 * @param[in]     EventId: The event ID
 * @param[in]     DebounceInfoData: Pointer to debounce data
 * @param[in]     SatelliteData: Satellite Data
 * @return        Dem_BaseInfoPtrType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_BaseInfoPtrType Dem_DebounceInfoInit(
    Dem_EventIdType             EventId,
    Dem_DebounceInfoType* const DebounceInfoData,
    Dem_SatelliteDataType       SatelliteData)
{
    Dem_BaseInfoPtrType base;

    switch (Dem_GetDebounceTypeByEventId(EventId))
    {
#if (defined(DEM_EVENT_COUNTER_DEBOUNCCE))
    case DEM_EVENT_COUNTER_DEBOUNCCE:
        base = Dem_DebounceCounterInit(&(DebounceInfoData->CounterBased), EventId, SatelliteData);
        break;
#endif
#if (defined(DEM_EVENT_TIME_DEBOUNCCE))
    case DEM_EVENT_TIME_DEBOUNCCE:
        base = Dem_DebounceTimeInit(&(DebounceInfoData->TimeBased), EventId, SatelliteData);
        break;
#endif
#if (defined(DEM_EVENT_MONOTOR_DEBOUNCCE))
    case DEM_EVENT_MONOTOR_DEBOUNCCE:
        base = Dem_DebounceMonitorInternalInit(&(DebounceInfoData->MonitorInternal), EventId, SatelliteData);
        break;
#endif
    default:
        /** Invalid debounce configurations that don't appear in the tool generation case */
#if (DEM_DEV_ERROR_DETECT == STD_ON)
        Dem_DetReportError(DEM_SID_INTERNALAPIID, DEM_E_WRONG_CONFIGURATION);
#endif
        break;
    }
    return base;
}

/**
 * @brief         Debounce Base Info initialization
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @param[in]     FunctionTable: Pointer to function table
 * @param[in]     EventId: The event ID
 * @return        Dem_BaseInfoPtrType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_BaseInfoPtrType Dem_DebounceBaseInfoInit(
    const Dem_BaseInfoPtrType        BaseInfoPtr,
    Dem_DebounceFunctionTablePtrType FunctionTable,
    Dem_EventIdType                  EventId)
{
    BaseInfoPtr->FunctionTable  = FunctionTable;
    BaseInfoPtr->EventId        = EventId;
    BaseInfoPtr->MonitorTrigger = DEM_EVENT_STATUS_INVALID;
    BaseInfoPtr->State          = DEM_DEBOUNCSTATE_INITIAL;
    return BaseInfoPtr;
}
/* PRQA S 5016 -- */

/**
 * @brief         Set Debounce Base Info MonitorTrigger
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @param[in]     MonitorTrigger: Monitor trigger
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_DebounceSetMonitorTrigger(Dem_BaseInfoPtrType BaseInfoPtr, Dem_EventStatusType MonitorTrigger)
{
    BaseInfoPtr->MonitorTrigger = MonitorTrigger;
}

/**
 * @brief         Call function to Perform the Debouncing calculation process
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_DebounceCalculate(Dem_BaseInfoPtrType BaseInfoPtr)
{
    return BaseInfoPtr->FunctionTable->Calculate(BaseInfoPtr);
}

/**
 * @brief         Call function to freeze debouncing
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_DebounceFreeze(Dem_BaseInfoConstPtrConstType BaseInfoPtr)
{
    BaseInfoPtr->FunctionTable->Freeze(BaseInfoPtr);
}

/**
 * @brief         Call function to reset debouncing
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_DebounceReset(Dem_BaseInfoConstPtrConstType BaseInfoPtr)
{
    BaseInfoPtr->FunctionTable->Reset(BaseInfoPtr);
}

/**
 * @brief         Call function to get the current debouncing state
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @return        Dem_DebouncingStateType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_DebouncingStateType Dem_DebounceGetDebouncingState(Dem_BaseInfoConstPtrConstType BaseInfoPtr)
{
    return BaseInfoPtr->FunctionTable->GetDebouncingState(BaseInfoPtr);
}

#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
/**
 * @brief Call function to Update Debounce Value Max
 */
DEM_LOCAL void Dem_DebounceUpdateDebounceValueMax(Dem_BaseInfoConstPtrConstType BaseInfoPtr)
{
    BaseInfoPtr->FunctionTable->UpdateDebounceValueMax(BaseInfoPtr);
}

/**
 * @brief Call function to resets the maximum debouncing value
 */
DEM_LOCAL void Dem_DebounceResetDebounceValueMax(Dem_BaseInfoConstPtrConstType BaseInfoPtr)
{
    BaseInfoPtr->FunctionTable->ResetDebounceValueMax(BaseInfoPtr);
}
#endif

/**
 * @brief Call function to Returns the fault detection counter.
 */
DEM_LOCAL Std_ReturnType Dem_DebounceGetFaultDetectionCounter(Dem_BaseInfoConstPtrConstType BaseInfoPtr, sint8* FDC)
{
    Std_ReturnType ret;
    ret = BaseInfoPtr->FunctionTable->GetFaultDetectionCounter(BaseInfoPtr, FDC);
    return ret;
}

#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
/**
 * @brief Call function to Update Debounce Value Max
 */
DEM_LOCAL Std_ReturnType Dem_DebounceGetMaxFaultDetectionCounter(Dem_BaseInfoConstPtrConstType BaseInfoPtr, sint8* FDC)
{
    Std_ReturnType ret;
    ret = BaseInfoPtr->FunctionTable->GetMaxFaultDetectionCounter(BaseInfoPtr, FDC);
    return ret;
}
#endif

/**
 * @brief         Call function to Get Debouncing Result
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @param[in]     SatelliteData: Satellite Data
 * @return        Dem_SatelliteDataType
 * @retval        Data: The detailed data type.
 * @retval        Raw: The raw data type intended for queue communication.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_SatelliteDataType
    Dem_DebounceGetDebouncingResult(Dem_BaseInfoConstPtrConstType BaseInfoPtr, Dem_SatelliteDataType SatelliteData)
{
    return BaseInfoPtr->FunctionTable->GetDebouncingResult(BaseInfoPtr, SatelliteData);
}

/**
 * @brief         Call function to restoring debouncing result
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @param[in]     SatelliteData: Satellite Data
 * @param[in]     Value: Process value
 * @param[in]     ExpectedThreshold: The expect threshold
 * @return        Dem_SatelliteDataType
 * @retval        Data: The detailed data type.
 * @retval        Raw: The raw data type intended for queue communication.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_SatelliteDataType Dem_DebounceRestoreDebounceValue(
    Dem_BaseInfoConstPtrConstType BaseInfoPtr,
    Dem_SatelliteDataType         SatelliteData,
    sint16                        Value,
    boolean                       ExpectedThreshold)
{
    Dem_SatelliteDataType lSatelliteData = SatelliteData;
    if (BaseInfoPtr->FunctionTable->RestoreDebounceValue != NULL_PTR)
    {
        lSatelliteData =
            BaseInfoPtr->FunctionTable->RestoreDebounceValue(BaseInfoPtr, SatelliteData, Value, ExpectedThreshold);
    }
    return lSatelliteData;
}

/**
 * @brief         Reset the debouncing status and data.
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @param[in]     SatelliteData: Satellite Data
 * @return        Dem_SatelliteDataType
 * @retval        Data: The detailed data type.
 * @retval        Raw: The raw data type intended for queue communication.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_SatelliteDataType
    Dem_ResetDebouncing(Dem_BaseInfoConstPtrConstType BaseInfoPtr, Dem_SatelliteDataType SatelliteData)
{
    Dem_DebounceReset(BaseInfoPtr);
    return Dem_DebounceGetDebouncingResult(BaseInfoPtr, SatelliteData);
}

/**
 * @brief         Freeze the debouncing status and data.
 * @param[in]     BaseInfoPtr: Pointer to the debounce information
 * @param[in]     SatelliteData: Satellite Data
 * @return        Dem_SatelliteDataType
 * @retval        Data: The detailed data type.
 * @retval        Raw: The raw data type intended for queue communication.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_SatelliteDataType
    Dem_FreezeDebouncing(Dem_BaseInfoConstPtrConstType BaseInfoPtr, Dem_SatelliteDataType SatelliteData)
{
    Dem_DebounceFreeze(BaseInfoPtr);
    return Dem_DebounceGetDebouncingResult(BaseInfoPtr, SatelliteData);
}

#if (defined(DEM_EVENT_COUNTER_DEBOUNCCE))
/**************Debounce Counter************************/
/**
 * @brief Debounce counter fault detection counter Calculate
 */
/* PRQA S 6030 ++ */ /* VL_MTR_Dem_STMIF */
DEM_LOCAL sint8 Dem_CalculateCounterFDC(Dem_EventIdType EventId, sint16 DebounceValue)
{
    sint8 FDC;

    if ((sint32)DebounceValue == (sint32)DEM_DEBOUNCE_INIT_VALUE)
    {
        FDC = DEM_FDC_INIT_VALUE;
    }
    else
    {
        Dem_DebounceAlgorithmNumType debounceRef = Dem_GetDebounceRefOfEvent(EventId);
        sint16                       failedThr   = Dem_GetFailedThrOfDB(debounceRef);
        sint16                       passedThr   = Dem_GetPassedThrOfDB(debounceRef);
        if ((sint32)DebounceValue == (sint32)failedThr)
        {
            FDC = DEM_FDC_MAX_VALUE;
        }
        else if ((sint32)DebounceValue == (sint32)passedThr)
        {
            FDC = DEM_FDC_MIN_VALUE;
        }
        else
        {
            /** current debounce counter value is somewhere in range of passed threshold and failed threshold but not 0
             */
            /* PRQA S 3103, 2152 ++ */ /* VL_Dem_3103, VL_Dem_2152 */
            if ((sint32)DebounceValue > (sint32)DEM_DEBOUNCE_INIT_VALUE)
            {
                FDC = (sint8)(((sint32)DebounceValue * (sint32)DEM_FDC_MAX_VALUE) / (sint32)failedThr);
                /** result has to be greater than 0 */
                if ((sint32)FDC == (sint32)DEM_FDC_INIT_VALUE)
                {
                    FDC = 1;
                }
            }
            else
            {
                FDC = (sint8)(((sint32)DebounceValue * (sint32)DEM_FDC_MIN_VALUE) / (sint32)passedThr);
                /** result has to be smaller than 0 */
                if ((sint32)FDC == (sint32)DEM_FDC_INIT_VALUE)
                {
                    FDC = -1;
                }
            }
            /* PRQA S 3103, 2152 -- */
        }
    }
    return FDC;
}
/* PRQA S 6030 -- */

/**
 * @brief         Debounce counter initialization
 */
DEM_LOCAL_INLINE Dem_BaseInfoPtrType Dem_DebounceCounterInit(
    Dem_CounterBasedInfoType* const DebounceCounterPtr,
    Dem_EventIdType                 EventId,
    Dem_SatelliteDataType           SatelliteData)
{
    Dem_BaseInfoPtrType ret =
        Dem_DebounceBaseInfoInit(&(DebounceCounterPtr->Base), &Dem_DebounceCounterFunctionTable, EventId);
    DebounceCounterPtr->OldDebounceValue = (sint16)Dem_GetSatelliteDebounceData(SatelliteData);
    DebounceCounterPtr->NewDebounceValue = (sint16)Dem_GetSatelliteDebounceData(SatelliteData);
    return ret;
}

/**
 * @brief Debounce counter Calculate of Prepassed function
 */
DEM_LOCAL void Dem_DebCounterCalculatePrepassed(Dem_CounterBasedInfoPtrType DebounceCounterPtr)
{
    Dem_EventIdType              eventId     = DebounceCounterPtr->Base.EventId;
    Dem_DebounceAlgorithmNumType debounceRef = Dem_GetDebounceRefOfEvent(eventId);
    sint16                       passedThr   = Dem_GetPassedThrOfDB(debounceRef);
    sint16                       decreStep   = Dem_GetDecreOfDB(debounceRef);
#if (DEM_JUMP_DOWN_DEBOUNCE_COUNTER == STD_ON)
    sint16 jumpDownValue = Dem_GetDownValueOfDB(debounceRef);

    if (((sint32)DebounceCounterPtr->OldDebounceValue > (sint32)jumpDownValue) && (Dem_GetJumpDownOfDB(debounceRef)))
    {
        /** JumpDown */
        DebounceCounterPtr->NewDebounceValue = jumpDownValue;
    }
#endif

    /** No overflow possible */
    if ((sint32)DebounceCounterPtr->NewDebounceValue <= (sint32)((sint32)passedThr - (sint32)decreStep))
    {
        DebounceCounterPtr->NewDebounceValue = passedThr;
        /** qualified result, set result and final de-bounce counter */
        DebounceCounterPtr->Base.MonitorTrigger = DEM_EVENT_STATUS_PASSED;
    }
    else
    {
        DebounceCounterPtr->NewDebounceValue =
            (sint16)((sint32)DebounceCounterPtr->NewDebounceValue + (sint32)decreStep);
    }
}

/**
 * @brief Debounce counter Calculate of passed function
 */
DEM_LOCAL void Dem_DebCounterCalculatePassed(Dem_CounterBasedInfoPtrType DebounceCounterPtr)
{
    Dem_EventIdType              eventId     = DebounceCounterPtr->Base.EventId;
    Dem_DebounceAlgorithmNumType debounceRef = Dem_GetDebounceRefOfEvent(eventId);
    sint16                       passedThr   = Dem_GetPassedThrOfDB(debounceRef);

    /** Qualified result - directly load threshold de-bounce counter from configuration */
    /** This also reduces exceeded counter values to match the passed threshold exactly (latch at bottom) */
    DebounceCounterPtr->NewDebounceValue    = passedThr;
    DebounceCounterPtr->Base.MonitorTrigger = DEM_EVENT_STATUS_PASSED;
}

/**
 * @brief Debounce counter Calculate of Prefailed function
 */
DEM_LOCAL void Dem_DebCounterCalculatePrefailed(Dem_CounterBasedInfoPtrType DebounceCounterPtr)
{
    Dem_EventIdType              eventId     = DebounceCounterPtr->Base.EventId;
    Dem_DebounceAlgorithmNumType debounceRef = Dem_GetDebounceRefOfEvent(eventId);
    sint16                       failedThr   = Dem_GetFailedThrOfDB(debounceRef);
    sint16                       IncreStep   = Dem_GetIncreOfDB(debounceRef);
#if (DEM_JUMP_UP_DEBOUNCE_COUNTER == STD_ON)
    sint16 jumpUpValue = Dem_GetUpValueOfDB(debounceRef);

    if (((sint32)DebounceCounterPtr->OldDebounceValue < (sint32)jumpUpValue) && (Dem_GetJumpUpOfDB(debounceRef)))
    {
        /** JumpUp */
        DebounceCounterPtr->NewDebounceValue = jumpUpValue;
    }
#endif

    /** No overflow possible */
    if ((sint32)DebounceCounterPtr->NewDebounceValue >= (sint32)((sint32)failedThr - (sint32)IncreStep))
    {
        /** qualified result, set result and final de-bounce counter */
        DebounceCounterPtr->NewDebounceValue    = failedThr;
        DebounceCounterPtr->Base.MonitorTrigger = DEM_EVENT_STATUS_FAILED;
    }
    else
    {
        DebounceCounterPtr->NewDebounceValue =
            (sint16)((sint32)DebounceCounterPtr->NewDebounceValue + (sint32)IncreStep);
    }
}

/**
 * @brief Debounce counter Calculate of failed function
 */
DEM_LOCAL void Dem_DebCounterCalculateFailed(Dem_CounterBasedInfoPtrType DebounceCounterPtr)
{
    Dem_EventIdType              eventId     = DebounceCounterPtr->Base.EventId;
    Dem_DebounceAlgorithmNumType debounceRef = Dem_GetDebounceRefOfEvent(eventId);
    sint16                       failedThr   = Dem_GetFailedThrOfDB(debounceRef);

    /** Qualified result - directly load threshold de-bounce counter from configuration */
    /** This also reduces counter values to match the failed threshold exactly (latch at ceiling) */
    DebounceCounterPtr->NewDebounceValue    = failedThr;
    DebounceCounterPtr->Base.MonitorTrigger = DEM_EVENT_STATUS_FAILED;
}

/* PRQA S 0310 ++ */ /* VL_Dem_0310 */
/**
 * @brief Debounce counter Calculate function
 */
DEM_LOCAL boolean Dem_DebounceCounterCalculate(Dem_BaseInfoPtrType BaseInfoPtr)
{
    Dem_EventIdType             eventId            = BaseInfoPtr->EventId;
    Dem_CounterBasedInfoPtrType debounceCounterPtr = (Dem_CounterBasedInfoPtrType)BaseInfoPtr;
    boolean                     ret                = FALSE;
    Dem_EventStatusType         monitorTrigger     = BaseInfoPtr->MonitorTrigger;

    /** Monitor status for this event not supported */
    if (monitorTrigger == DEM_EVENT_STATUS_FDC_THRESHOLD_REACHED)
    {
#if (DEM_DEV_ERROR_DETECT == STD_ON)
        Dem_DetReportError(DEM_SID_INTERNALAPIID, DEM_E_PARAM_DATA);
#endif
    }
    else
    {
        if ((Dem_CheckECFulfilled(eventId) == TRUE) && (Dem_CheckEventDTCSettingState(eventId) == FALSE))
        {
            const Dem_DebounceCounterCalculFunctionPtr
                Dem_DebounceCounterCalcul[DEM_EVENT_STATUS_FDC_THRESHOLD_REACHED] = {
                    Dem_DebCounterCalculatePassed,
                    Dem_DebCounterCalculateFailed,
                    Dem_DebCounterCalculatePrepassed,
                    Dem_DebCounterCalculatePrefailed,
                };
            Dem_DebounceAlgorithmNumType debounceRef = Dem_GetDebounceRefOfEvent(eventId);
            sint16                       fdcThr      = Dem_GetFdcThrOfDB(debounceRef);
            sint16                       newDebounceValue;
            sint16                       oldDebounceValue = debounceCounterPtr->OldDebounceValue;
            (*Dem_DebounceCounterCalcul[monitorTrigger])(debounceCounterPtr);

            newDebounceValue = debounceCounterPtr->NewDebounceValue;
            if (((sint32)oldDebounceValue != (sint32)newDebounceValue)
#if (DEM_GENERAL_FF_TRIGGER_ON_EVERY_TEST_FAILED == STD_ON)
                || (monitorTrigger == DEM_EVENT_STATUS_FAILED)
#endif
            )
            {
                Dem_SetDebouncingChanged(BaseInfoPtr);
                if (((sint32)newDebounceValue >= (sint32)fdcThr) && ((sint32)oldDebounceValue < (sint32)fdcThr))
                {
                    Dem_SetFdcThresholdExceeded(BaseInfoPtr);
                    Dem_SetRequestFdcProcessing(BaseInfoPtr);
                }
                /** Possible update of SLC based on update of TOC */
#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
                if ((sint32)newDebounceValue > (sint32)Dem_GetDebounceValueMaxByEvent(eventId))
                {
                    Dem_SetMaximumFdcExceeded(BaseInfoPtr);
                    Dem_SetRequestFdcProcessing(BaseInfoPtr);
                }
#endif
            }
            ret = TRUE;
        }
    }
    return ret;
}

/**
 * @brief Debounce counter Freeze function
 */
DEM_LOCAL void Dem_DebounceCounterFreeze(Dem_BaseInfoConstPtrConstType BaseInfoPtr)
{
    /** No action necessary */
    DEM_PARAM_UNUSED(BaseInfoPtr);
}

/* PRQA S 0311 ++ */ /* VL_Dem_0311 */
/**
 * @brief Debounce counter Reset function
 */
DEM_LOCAL void Dem_DebounceCounterReset(Dem_BaseInfoConstPtrConstType BaseInfoPtr)
{
    Dem_CounterBasedInfoPtrType debounceCounterPtr = (Dem_CounterBasedInfoPtrType)BaseInfoPtr;
    debounceCounterPtr->NewDebounceValue           = DEM_DEBOUNCE_INIT_VALUE;
}

/**
 * @brief Debounce counter Get Debouncing State function
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
DEM_LOCAL Dem_DebouncingStateType Dem_DebounceCounterGetDebouncingState(Dem_BaseInfoConstPtrConstType BaseInfoPtr)
{
    Dem_EventIdType                  eventId            = BaseInfoPtr->EventId;
    Dem_DebouncingStateType          debouncingState    = 0u;
    Dem_DebounceAlgorithmNumType     debounceRef        = Dem_GetDebounceRefOfEvent(eventId);
    Dem_CounterBasedInfoConstPtrType debounceCounterPtr = (Dem_CounterBasedInfoPtrType)BaseInfoPtr;
    sint16                           newDebounceValue   = debounceCounterPtr->NewDebounceValue;

    if ((sint32)newDebounceValue == (sint32)Dem_GetFailedThrOfDB(debounceRef))
    {
        debouncingState = Dem_SetFinallyDefective(debouncingState);
        debouncingState = Dem_SetTestComplete(debouncingState);
        if ((Dem_CheckECFulfilled(eventId) == TRUE) && (Dem_CheckEventDTCSettingState(eventId) == FALSE)
            && (Dem_CheckSCFulfilled(eventId) == TRUE))
        {
            debouncingState = Dem_SetDtrUpdate(debouncingState);
        }
    }
    else if ((sint32)newDebounceValue == (sint32)Dem_GetPassedThrOfDB(debounceRef))
    {
        debouncingState = Dem_SetTestComplete(debouncingState);
        if ((Dem_CheckECFulfilled(eventId) == TRUE) && (Dem_CheckEventDTCSettingState(eventId) == FALSE)
            && (Dem_CheckSCFulfilled(eventId) == TRUE))
        {
            debouncingState = Dem_SetDtrUpdate(debouncingState);
        }
    }
    else if ((sint32)newDebounceValue > (sint32)DEM_DEBOUNCE_INIT_VALUE)
    {
        debouncingState = Dem_SetTemporarilyDefective(debouncingState);
    }
    else if ((sint32)newDebounceValue < (sint32)DEM_DEBOUNCE_INIT_VALUE)
    {
        debouncingState = Dem_SetTemporarilyHealed(debouncingState);
    }
    else
    {
        /** de-bouncing state 0 */
    }
    return debouncingState;
}
/* PRQA S 6070 -- */

#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
/**
 * @brief Debounce counter Update Debounce Value Max function
 */
DEM_LOCAL void Dem_DebounceCounterUpdateDebounceValueMax(Dem_BaseInfoConstPtrConstType BaseInfoPtr)
{
    Dem_CounterBasedInfoConstPtrType debounceCounterPtr = (Dem_CounterBasedInfoPtrType)BaseInfoPtr;
    Dem_SetDebounceValueMaxByEvent(BaseInfoPtr->EventId, debounceCounterPtr->NewDebounceValue);
}

/**
 * @brief Debounce counter Reset Debounce Value Max function
 */
DEM_LOCAL void Dem_DebounceCounterResetDebounceValueMax(Dem_BaseInfoConstPtrConstType BaseInfoPtr)
{
    Dem_SetDebounceValueMaxByEvent(BaseInfoPtr->EventId, DEM_DEBOUNCE_INIT_VALUE);
}
#endif

/**
 * @brief Debounce counter Get Fault Detection Counter function
 */
DEM_LOCAL Std_ReturnType
    Dem_DebounceCounterGetFaultDetectionCounter(Dem_BaseInfoConstPtrConstType BaseInfoPtr, sint8* FDC)
{
    Dem_EventIdType                  eventId            = BaseInfoPtr->EventId;
    Dem_CounterBasedInfoConstPtrType debounceCounterPtr = (Dem_CounterBasedInfoPtrType)BaseInfoPtr;

    *FDC = Dem_CalculateCounterFDC(eventId, debounceCounterPtr->NewDebounceValue);
    return E_OK;
}

#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
/**
 * @brief Debounce counter Get Max Fault Detection Counter function
 */
DEM_LOCAL Std_ReturnType
    Dem_DebounceCounterGetMaxFaultDetectionCounter(Dem_BaseInfoConstPtrConstType BaseInfoPtr, sint8* FDC)
{
    Dem_EventIdType eventId = BaseInfoPtr->EventId;
    *FDC                    = Dem_CalculateCounterFDC(eventId, Dem_GetDebounceValueMaxByEvent(eventId));
    return E_OK;
}
#endif

/**
 * @brief Debounce counter Get Debouncing Result function
 */
DEM_LOCAL Dem_SatelliteDataType Dem_DebounceCounterGetDebouncingResult(
    Dem_BaseInfoConstPtrConstType BaseInfoPtr,
    Dem_SatelliteDataType         SatelliteData)
{
    Dem_SatelliteDataType            ret;
    Dem_CounterBasedInfoConstPtrType debounceCounterPtr = (Dem_CounterBasedInfoPtrType)BaseInfoPtr;
    ret = Dem_SetSatelliteDebounceData(SatelliteData, (Dem_DebounceDataType)debounceCounterPtr->NewDebounceValue);
    return ret;
}
/* PRQA S 0311 -- */

/**
 * @brief Debounce counter Restore Debounce Value function
 */
DEM_LOCAL Dem_SatelliteDataType Dem_DebounceCounterRestoreDebounceValue(
    Dem_BaseInfoConstPtrConstType BaseInfoPtr,
    Dem_SatelliteDataType         SatelliteData,
    sint16                        Value,
    boolean                       ExpectedThreshold)
{
    Dem_EventIdType              eventId = BaseInfoPtr->EventId;
    Dem_SatelliteDataType        ret;
    Dem_DebounceAlgorithmNumType debounceRef = Dem_GetDebounceRefOfEvent(eventId);
    sint16                       failedThr   = Dem_GetFailedThrOfDB(debounceRef);
    sint16                       passedThr   = Dem_GetPassedThrOfDB(debounceRef);

    if (/** Unqualified de-bounce state is ok */
        (((sint32)failedThr > (sint32)Value) && ((sint32)passedThr < (sint32)Value))
        /** If de-bouncing counter has reached qualified threshold, the event status must match */
        || (((sint32)failedThr == (sint32)Value) && (ExpectedThreshold == TRUE))
        || (((sint32)passedThr == (sint32)Value) && (ExpectedThreshold == FALSE)))
    {
        ret = Dem_SetSatelliteDebounceData(SatelliteData, (Dem_DebounceDataType)Value);
    }
    else
    {
        ret = SatelliteData;
    }

    return ret;
}
/* PRQA S 0310 -- */
#endif

#if (defined(DEM_EVENT_TIME_DEBOUNCCE))
/* PRQA S 0310 ++ */ /* VL_Dem_0310 */
/**************Debounce Time***************************/
/**
 * @brief         Debounce time initialization
 */
DEM_LOCAL_INLINE Dem_BaseInfoPtrType Dem_DebounceTimeInit(
    Dem_TimeBasedInfoType* const DebounceTimePtr,
    Dem_EventIdType              EventId,
    Dem_SatelliteDataType        SatelliteData)
{
    Dem_BaseInfoPtrType ret =
        Dem_DebounceBaseInfoInit(&(DebounceTimePtr->Base), &Dem_DebounceTimeFunctionTable, EventId);
    DebounceTimePtr->OldTimerState = Dem_GetSatelliteDebounceStatus(SatelliteData);
    DebounceTimePtr->NewTimerState = DebounceTimePtr->OldTimerState;
    DebounceTimePtr->OldTimerValue = (sint16)Dem_GetSatelliteDebounceData(SatelliteData);
    DebounceTimePtr->NewTimerValue = DebounceTimePtr->OldTimerValue;
    return ret;
}

/**
 * @brief Debounce time Calculate Fault Detection Counter
 */
DEM_LOCAL sint8 Dem_CalculateTimeFDC(Dem_EventIdType EventId, sint16 DebounceValue, uint8 DebounceStatus)
{
    sint8 lFDC;

    switch (DebounceStatus)
    {
    case DEM_DEBOUNCETIME_NONE:
        lFDC = DEM_FDC_INIT_VALUE;
        break;
    case DEM_DEBOUNCETIME_DONEFAILED:
        lFDC = DEM_FDC_MAX_VALUE;
        break;
    case DEM_DEBOUNCETIME_DONEPASSED:
        lFDC = DEM_FDC_MIN_VALUE;
        break;
    case DEM_DEBOUNCETIME_COUNTFAILED:
    case DEM_DEBOUNCETIME_FROZENFAILED:
    case DEM_DEBOUNCETIME_COUNTFAILEDFDC:
    case DEM_DEBOUNCETIME_FROZENFAILEDFDC:
    {
        Dem_DebounceAlgorithmNumType debounceRef = Dem_GetDebounceRefOfEvent(EventId);
        sint16                       failedThr   = Dem_GetFailedThrOfDB(debounceRef);

        /* PRQA S 3103, 2152 ++ */ /* VL_Dem_3103, VL_Dem_2152 */
        lFDC = (sint8)(((sint32)((sint32)failedThr - (sint32)DebounceValue) * (sint32)DEM_FDC_MAX_VALUE)
                       / (sint32)failedThr);
        /** Already counting, assert an FDC of at least 1 */
        if ((sint32)lFDC == (sint32)DEM_FDC_INIT_VALUE)
        {
            lFDC = 1;
        }
        break;
    }
    case DEM_DEBOUNCETIME_RESETFAILED:
        lFDC = 1;
        break;
    case DEM_DEBOUNCETIME_COUNTPASSED:
    case DEM_DEBOUNCETIME_FROZENPASSED:
    {
        Dem_DebounceAlgorithmNumType debounceRef = Dem_GetDebounceRefOfEvent(EventId);
        sint16                       passedThr   = Dem_GetPassedThrOfDB(debounceRef);

        lFDC = (sint8)(((sint32)((sint32)passedThr - (sint32)DebounceValue) * (sint32)DEM_FDC_MIN_VALUE)
                       / (sint32)passedThr);
        /** Already counting, assert an FDC of at most -1 */
        if ((sint32)lFDC == (sint32)DEM_FDC_INIT_VALUE)
        {
            lFDC = -1;
        }
        /* PRQA S 3103, 2152 -- */
        break;
    }
    case DEM_DEBOUNCETIME_RESETPASSED:
        lFDC = -1;
        break;
    default:
        lFDC = DEM_FDC_INIT_VALUE;
        break;
    }
    return lFDC;
}

/**
 * @brief Debounce time Calculate of Prepassed function
 */
DEM_LOCAL void Dem_DebTimeCalculatePrepassed(Dem_TimeBasedInfoPtrType DebounceTimePtr)
{
    switch (DebounceTimePtr->OldTimerState)
    {
    case DEM_DEBOUNCETIME_NONE:
    case DEM_DEBOUNCETIME_RESETFAILED:
    case DEM_DEBOUNCETIME_COUNTFAILED:
    case DEM_DEBOUNCETIME_FROZENFAILED:
    case DEM_DEBOUNCETIME_COUNTFAILEDFDC:
    case DEM_DEBOUNCETIME_FROZENFAILEDFDC:
    case DEM_DEBOUNCETIME_DONEFAILED:
        DebounceTimePtr->NewTimerState = DEM_DEBOUNCETIME_RESETPASSED;
        break;
    case DEM_DEBOUNCETIME_FROZENPASSED:
        DebounceTimePtr->NewTimerState = DEM_DEBOUNCETIME_COUNTPASSED;
        break;
    case DEM_DEBOUNCETIME_DONEPASSED:
        /** Rare case with continuous debouncing in which a qualified passed result should has been reached before
         * but not been processed since enable conditions were disabled. Allow for proccessing the passed result
         * qualification with next 'Prepassed'*/
        DebounceTimePtr->Base.MonitorTrigger = DEM_EVENT_STATUS_PASSED;
        break;
    case DEM_DEBOUNCETIME_RESETPASSED:
    case DEM_DEBOUNCETIME_COUNTPASSED:
        /** Nothing to do */
        break;
    default:
        /** idle*/
        break;
    }
}

/**
 * @brief Debounce time Calculate of passed function
 */
DEM_LOCAL void Dem_DebTimeCalculatePrefailed(Dem_TimeBasedInfoPtrType DebounceTimePtr)
{
    switch (DebounceTimePtr->OldTimerState)
    {
    case DEM_DEBOUNCETIME_NONE:
    case DEM_DEBOUNCETIME_RESETPASSED:
    case DEM_DEBOUNCETIME_COUNTPASSED:
    case DEM_DEBOUNCETIME_FROZENPASSED:
    case DEM_DEBOUNCETIME_DONEPASSED:
        DebounceTimePtr->NewTimerState = DEM_DEBOUNCETIME_RESETFAILED;
        break;
    case DEM_DEBOUNCETIME_FROZENFAILED:
        DebounceTimePtr->NewTimerState = DEM_DEBOUNCETIME_COUNTFAILED;
        break;
    case DEM_DEBOUNCETIME_FROZENFAILEDFDC:
        DebounceTimePtr->NewTimerState = DEM_DEBOUNCETIME_COUNTFAILEDFDC;
        break;
    case DEM_DEBOUNCETIME_DONEFAILED:
        /** Rare case with continuous debouncing in which a qualified failed result should has been reached but not been
         * processed since enable conditions were disabled. Allow for proccessing the failed result qualification with
         * next 'Prefailed'*/
        DebounceTimePtr->Base.MonitorTrigger = DEM_EVENT_STATUS_FAILED;
        break;
    case DEM_DEBOUNCETIME_RESETFAILED:
    case DEM_DEBOUNCETIME_COUNTFAILED:
    case DEM_DEBOUNCETIME_COUNTFAILEDFDC:
        /** No change */
        break;
    default:
        /** idle */
        break;
    }
}

/**
 * @brief Debounce time Calculate of Prefailed function
 */
DEM_LOCAL void Dem_DebTimeCalculatePassed(Dem_TimeBasedInfoPtrType DebounceTimePtr)
{
    DebounceTimePtr->NewTimerState = DEM_DEBOUNCETIME_DONEPASSED;
    DebounceTimePtr->NewTimerValue = DEM_DEBOUNCE_INIT_VALUE;
}

/**
 * @brief Debounce time Calculate of failed function
 */
DEM_LOCAL void Dem_DebTimeCalculateFailed(Dem_TimeBasedInfoPtrType DebounceTimePtr)
{
    Dem_BaseInfoPtrType baseInfoPtr = &DebounceTimePtr->Base;
    DebounceTimePtr->NewTimerState  = DEM_DEBOUNCETIME_DONEFAILED;
    DebounceTimePtr->NewTimerValue  = DEM_DEBOUNCE_INIT_VALUE;
    if (DebounceTimePtr->OldTimerState != DEM_DEBOUNCETIME_DONEFAILED)
    {
        Dem_SetMaximumFdcExceeded(baseInfoPtr);
        Dem_SetRequestFdcProcessing(baseInfoPtr);
    }
    if ((DebounceTimePtr->OldTimerState != DEM_DEBOUNCETIME_COUNTFAILEDFDC)
        && (DebounceTimePtr->OldTimerState != DEM_DEBOUNCETIME_FROZENFAILEDFDC)
        && (DebounceTimePtr->OldTimerState != DEM_DEBOUNCETIME_DONEFAILED))
    {
        Dem_SetFdcThresholdExceeded(baseInfoPtr);
        Dem_SetRequestFdcProcessing(baseInfoPtr);
    }
}

/**
 * @brief Debounce Time Calculate function
 */
DEM_LOCAL boolean Dem_DebounceTimeCalculate(Dem_BaseInfoPtrType BaseInfoPtr)
{
    Dem_EventIdType          eventId         = BaseInfoPtr->EventId;
    Dem_TimeBasedInfoPtrType debounceTimePtr = (Dem_TimeBasedInfoPtrType)BaseInfoPtr;
    boolean                  ret             = FALSE;
    Dem_EventStatusType      monitorTrigger  = BaseInfoPtr->MonitorTrigger;

    /** Monitor status for this event not supported */
    if (monitorTrigger == DEM_EVENT_STATUS_FDC_THRESHOLD_REACHED)
    {
#if (DEM_DEV_ERROR_DETECT == STD_ON)
        Dem_DetReportError(DEM_SID_INTERNALAPIID, DEM_E_PARAM_DATA);
#endif
    }
    else
    {
        if ((Dem_CheckECFulfilled(eventId) == TRUE) && (Dem_CheckEventDTCSettingState(eventId) == FALSE))
        {
            const Dem_DebounceTimeCalculFunctionPtr Dem_DebounceTimeCalcul[DEM_EVENT_STATUS_FDC_THRESHOLD_REACHED] = {
                Dem_DebTimeCalculatePassed,
                Dem_DebTimeCalculateFailed,
                Dem_DebTimeCalculatePrepassed,
                Dem_DebTimeCalculatePrefailed,
            };

            (*Dem_DebounceTimeCalcul[monitorTrigger])(debounceTimePtr);
            if ((debounceTimePtr->OldTimerState != debounceTimePtr->NewTimerState)
#if (DEM_GENERAL_FF_TRIGGER_ON_EVERY_TEST_FAILED == STD_ON)
                || (monitorTrigger == DEM_EVENT_STATUS_FAILED)
#endif
            )
            {
                Dem_SetDebouncingChanged(BaseInfoPtr);
            }
            ret = TRUE;
        }
    }
    return ret;
}

/* PRQA S 0311 ++ */ /* VL_Dem_0311 */
/**
 * @brief Debounce Time Freeze function
 */
DEM_LOCAL void Dem_DebounceTimeFreeze(Dem_BaseInfoConstPtrConstType BaseInfoPtr)
{
    Dem_EventIdType              eventId         = BaseInfoPtr->EventId;
    Dem_DebounceAlgorithmNumType debounceRef     = Dem_GetDebounceRefOfEvent(eventId);
    Dem_TimeBasedInfoPtrType     debounceTimePtr = (Dem_TimeBasedInfoPtrType)BaseInfoPtr;

    switch (debounceTimePtr->OldTimerState)
    {
    case DEM_DEBOUNCETIME_RESETFAILED:
        debounceTimePtr->NewTimerValue = Dem_GetFailedThrOfDB(debounceRef);
        debounceTimePtr->NewTimerState = DEM_DEBOUNCETIME_FROZENFAILED;
        break;
    case DEM_DEBOUNCETIME_RESETPASSED:
        debounceTimePtr->NewTimerValue = Dem_GetPassedThrOfDB(debounceRef);
        debounceTimePtr->NewTimerState = DEM_DEBOUNCETIME_FROZENPASSED;
        break;
    case DEM_DEBOUNCETIME_COUNTFAILED:
        debounceTimePtr->NewTimerState = DEM_DEBOUNCETIME_FROZENFAILED;
        break;
    case DEM_DEBOUNCETIME_COUNTPASSED:
        debounceTimePtr->NewTimerState = DEM_DEBOUNCETIME_FROZENPASSED;
        break;
    case DEM_DEBOUNCETIME_COUNTFAILEDFDC:
        debounceTimePtr->NewTimerState = DEM_DEBOUNCETIME_FROZENFAILEDFDC;
        break;
    default:
        /** Nothing to do */
        break;
    }
}

/**
 * @brief Debounce Time Reset function
 */
DEM_LOCAL void Dem_DebounceTimeReset(Dem_BaseInfoConstPtrConstType BaseInfoPtr)
{
    Dem_TimeBasedInfoPtrType debounceTimePtr = (Dem_TimeBasedInfoPtrType)BaseInfoPtr;
    debounceTimePtr->NewTimerState           = DEM_DEBOUNCETIME_NONE;
    debounceTimePtr->NewTimerValue           = DEM_DEBOUNCE_INIT_VALUE;
}

/**
 * @brief Debounce Time Get Debouncing State function
 */
DEM_LOCAL Dem_DebouncingStateType Dem_DebounceTimeGetDebouncingState(Dem_BaseInfoConstPtrConstType BaseInfoPtr)
{
    Dem_DebouncingStateType       debouncingState = 0u;
    Dem_EventIdType               eventId         = BaseInfoPtr->EventId;
    Dem_TimeBasedInfoConstPtrType debounceTimePtr = (Dem_TimeBasedInfoPtrType)BaseInfoPtr;

    switch (debounceTimePtr->OldTimerState)
    {
    case DEM_DEBOUNCETIME_DONEFAILED:
        debouncingState = Dem_SetFinallyDefective(debouncingState);
        debouncingState = Dem_SetTestComplete(debouncingState);
        if ((Dem_CheckECFulfilled(eventId) == TRUE) && (Dem_CheckEventDTCSettingState(eventId) == FALSE)
            && (Dem_CheckSCFulfilled(eventId) == TRUE))
        {
            debouncingState = Dem_SetDtrUpdate(debouncingState);
        }
        break;
    case DEM_DEBOUNCETIME_DONEPASSED:
        debouncingState = Dem_SetTestComplete(debouncingState);
        if ((Dem_CheckECFulfilled(eventId) == TRUE) && (Dem_CheckEventDTCSettingState(eventId) == FALSE)
            && (Dem_CheckSCFulfilled(eventId) == TRUE))
        {
            debouncingState = Dem_SetDtrUpdate(debouncingState);
        }
        break;
    case DEM_DEBOUNCETIME_COUNTFAILED:
    case DEM_DEBOUNCETIME_FROZENFAILED:
    case DEM_DEBOUNCETIME_COUNTFAILEDFDC:
    case DEM_DEBOUNCETIME_FROZENFAILEDFDC:
    case DEM_DEBOUNCETIME_RESETFAILED:
        debouncingState = Dem_SetTemporarilyDefective(debouncingState);
        break;
    case DEM_DEBOUNCETIME_COUNTPASSED:
    case DEM_DEBOUNCETIME_FROZENPASSED:
    case DEM_DEBOUNCETIME_RESETPASSED:
        debouncingState = Dem_SetTemporarilyHealed(debouncingState);
        break;
    case DEM_DEBOUNCETIME_NONE:
    default:
        /** debouncing state 0 */
        break;
    }
    return debouncingState;
}

#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
/**
 * @brief Debounce Time Update Debounce Value Max function
 */
DEM_LOCAL void Dem_DebounceTimeUpdateDebounceValueMax(Dem_BaseInfoConstPtrConstType BaseInfoPtr)
{
    Dem_EventIdType               eventId         = BaseInfoPtr->EventId;
    Dem_TimeBasedInfoConstPtrType debounceTimePtr = (Dem_TimeBasedInfoPtrType)BaseInfoPtr;

    if ((sint32)debounceTimePtr->NewTimerValue < (sint32)Dem_GetDebounceValueMaxByEvent(eventId))
    {
        Dem_SetDebounceValueMaxByEvent(eventId, debounceTimePtr->NewTimerValue);
    }
}

/**
 * @brief Debounce Time Reset Debounce Value Max function
 */
DEM_LOCAL void Dem_DebounceTimeResetDebounceValueMax(Dem_BaseInfoConstPtrConstType BaseInfoPtr)
{
    Dem_EventIdType              eventId     = BaseInfoPtr->EventId;
    Dem_DebounceAlgorithmNumType debounceRef = Dem_GetDebounceRefOfEvent(eventId);
    Dem_SetDebounceValueMaxByEvent(eventId, Dem_GetFailedThrOfDB(debounceRef));
}
#endif

/**
 * @brief Debounce Time Get Fault Detection Counter function
 */
DEM_LOCAL Std_ReturnType Dem_DebounceTimeGetFaultDetectionCounter(Dem_BaseInfoConstPtrConstType BaseInfoPtr, sint8* FDC)
{
    Dem_TimeBasedInfoConstPtrType debounceTimePtr = (Dem_TimeBasedInfoPtrType)BaseInfoPtr;

    *FDC = Dem_CalculateTimeFDC(BaseInfoPtr->EventId, debounceTimePtr->OldTimerValue, debounceTimePtr->OldTimerState);
    return E_OK;
}

#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))

/**
 * @brief Debounce Time Get Max Fault Detection Counter function
 */
DEM_LOCAL Std_ReturnType
    Dem_DebounceTimeGetMaxFaultDetectionCounter(Dem_BaseInfoConstPtrConstType BaseInfoPtr, sint8* FDC)
{
    Dem_EventIdType              eventId     = BaseInfoPtr->EventId;
    Dem_DebounceAlgorithmNumType debounceRef = Dem_GetDebounceRefOfEvent(eventId);

    if ((sint32)Dem_GetDebounceValueMaxByEvent(eventId) != (sint32)Dem_GetFailedThrOfDB(debounceRef))
    {
        *FDC = Dem_CalculateTimeFDC(eventId, Dem_GetDebounceValueMaxByEvent(eventId), DEM_DEBOUNCETIME_COUNTFAILED);
    }
    else
    {
        *FDC = DEM_FDC_INIT_VALUE;
    }
    return E_OK;
}
#endif

/**
 * @brief Debounce Time Get Debouncing Result function
 */
DEM_LOCAL Dem_SatelliteDataType
    Dem_DebounceTimeGetDebouncingResult(Dem_BaseInfoConstPtrConstType BaseInfoPtr, Dem_SatelliteDataType SatelliteData)
{
    Dem_SatelliteDataType         ret             = SatelliteData;
    Dem_TimeBasedInfoConstPtrType debounceTimePtr = (Dem_TimeBasedInfoPtrType)BaseInfoPtr;

    ret = Dem_SetSatelliteDebounceStatus(ret, (Dem_DebounceStatusType)debounceTimePtr->NewTimerState);
    ret = Dem_SetSatelliteDebounceData(ret, (Dem_DebounceDataType)debounceTimePtr->NewTimerValue);
    return ret;
}

/* PRQA S 1259 ++ */ /* VL_Dem_1259 */
/**
 * @brief Debounce Time Counting Towards Passed
 */
DEM_LOCAL void Dem_DebounceTimeCountingTowardsPassed(const Dem_TimeBasedInfoPtrType DebounceTimePtr)
{
    /** process event timer */
    DebounceTimePtr->NewTimerValue = (sint16)((sint32)DebounceTimePtr->OldTimerValue - (sint32)1);
    if ((sint32)DebounceTimePtr->NewTimerValue == (sint32)DEM_DEBOUNCE_INIT_VALUE)
    {
        DebounceTimePtr->NewTimerState = DEM_DEBOUNCETIME_DONEPASSED;
        if ((Dem_CheckECFulfilled(DebounceTimePtr->Base.EventId) == TRUE)
            && (Dem_CheckEventDTCSettingState(DebounceTimePtr->Base.EventId) == FALSE))
        {
            DebounceTimePtr->Base.MonitorTrigger = DEM_EVENT_STATUS_PASSED;
        }
    }
}

/**
 * @brief Debounce Time Counting Towards Failed
 */
DEM_LOCAL void Dem_DebounceTimeCountingTowardsFailed(const Dem_TimeBasedInfoPtrType DebounceTimePtr)
{
    Dem_BaseInfoPtrType baseInfoPtr = &DebounceTimePtr->Base;
    Dem_EventIdType     eventId     = baseInfoPtr->EventId;
    /* PRQA S 4404 ++ */ /* VL_Dem_4404 */
    boolean lECSatisfied =
        ((Dem_CheckECFulfilled(eventId) == TRUE) && (Dem_CheckEventDTCSettingState(eventId) == FALSE));
    /* PRQA S 4404 -- */
    Dem_DebounceAlgorithmNumType debounceRef = Dem_GetDebounceRefOfEvent(eventId);
    sint16                       fdcThr      = Dem_GetFdcThrOfDB(debounceRef);
    sint16                       newTimerValue;

    /** process event timer */
    DebounceTimePtr->NewTimerValue = (sint16)((sint32)DebounceTimePtr->OldTimerValue - (sint32)1);
    newTimerValue                  = DebounceTimePtr->NewTimerValue;
    if ((sint32)newTimerValue <= (sint32)fdcThr)
    {
        if ((sint32)newTimerValue == (sint32)DEM_DEBOUNCE_INIT_VALUE)
        {
            DebounceTimePtr->NewTimerState = DEM_DEBOUNCETIME_DONEFAILED;
            if (lECSatisfied == TRUE)
            {
                baseInfoPtr->MonitorTrigger = DEM_EVENT_STATUS_FAILED;
            }
        }
        else
        {
            DebounceTimePtr->NewTimerState = DEM_DEBOUNCETIME_COUNTFAILEDFDC;
        }
        if (lECSatisfied == TRUE)
        {
            Dem_SetFdcThresholdExceeded(baseInfoPtr);
            Dem_SetRequestFdcProcessing(baseInfoPtr);
        }
    }
#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
    if ((lECSatisfied == TRUE) && ((sint32)Dem_GetDebounceValueMaxByEvent(eventId) > (sint32)newTimerValue))
    {
        Dem_SetMaximumFdcExceeded(baseInfoPtr);
        Dem_SetRequestFdcProcessing(baseInfoPtr);
    }
#endif
}

/**
 * @brief Debounce Time Counting Towards Failed With FDC Reached
 */
DEM_LOCAL void Dem_DebounceTimeCountingTowardsFailedWithFDCReached(const Dem_TimeBasedInfoPtrType DebounceTimePtr)
{
    Dem_BaseInfoPtrType baseInfoPtr = &DebounceTimePtr->Base;
    Dem_EventIdType     eventId     = baseInfoPtr->EventId;
    /* PRQA S 4404 ++ */ /* VL_Dem_4404 */
    boolean lECSatisfied =
        ((Dem_CheckECFulfilled(eventId) == TRUE) && (Dem_CheckEventDTCSettingState(eventId) == FALSE));
    /* PRQA S 4404 -- */
    sint16 newTimerValue;

    /** process event timer */
    DebounceTimePtr->NewTimerValue = (sint16)((sint32)DebounceTimePtr->OldTimerValue - (sint32)1);
    newTimerValue                  = DebounceTimePtr->NewTimerValue;
    if ((sint32)newTimerValue == (sint32)DEM_DEBOUNCE_INIT_VALUE)
    {
        DebounceTimePtr->NewTimerState = DEM_DEBOUNCETIME_DONEFAILED;
        if (lECSatisfied == TRUE)
        {
            baseInfoPtr->MonitorTrigger = DEM_EVENT_STATUS_FAILED;
        }
    }

#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
    if ((lECSatisfied == TRUE) && ((sint32)Dem_GetDebounceValueMaxByEvent(eventId) > (sint32)newTimerValue))
    {
        Dem_SetMaximumFdcExceeded(baseInfoPtr);
        Dem_SetRequestFdcProcessing(baseInfoPtr);
    }
#endif
}
/* PRQA S 1259 -- */

/**
 * @brief Debounce Time Process Debounce
 */
DEM_LOCAL void Dem_DebounceTimeProcessDebounce(Dem_BaseInfoConstPtrConstType BaseInfoPtr)
{
    Dem_TimeBasedInfoPtrType     debounceTimePtr = (Dem_TimeBasedInfoPtrType)BaseInfoPtr;
    Dem_EventIdType              eventId         = BaseInfoPtr->EventId;
    Dem_DebounceAlgorithmNumType debounceRef     = Dem_GetDebounceRefOfEvent(eventId);

    switch (debounceTimePtr->OldTimerState)
    {
    case DEM_DEBOUNCETIME_RESETPASSED:
    {
        debounceTimePtr->NewTimerValue = Dem_GetPassedThrOfDB(debounceRef);
        debounceTimePtr->NewTimerState = DEM_DEBOUNCETIME_COUNTPASSED;
        break;
    }
    case DEM_DEBOUNCETIME_RESETFAILED:
    {
        debounceTimePtr->NewTimerValue = Dem_GetFailedThrOfDB(debounceRef);
        debounceTimePtr->NewTimerState = DEM_DEBOUNCETIME_COUNTFAILED;
        break;
    }
    case DEM_DEBOUNCETIME_COUNTPASSED:
        Dem_DebounceTimeCountingTowardsPassed(debounceTimePtr);
        break;
    case DEM_DEBOUNCETIME_COUNTFAILED:
        Dem_DebounceTimeCountingTowardsFailed(debounceTimePtr);
        break;
    case DEM_DEBOUNCETIME_COUNTFAILEDFDC:
        Dem_DebounceTimeCountingTowardsFailedWithFDCReached(debounceTimePtr);
        break;
    default:
        /** idle */
        break;
    }
}

/**
 * @brief Check Debounce Time is active
 */
DEM_LOCAL boolean Dem_DebounceTimeIsTimerActive(Dem_BaseInfoConstPtrConstType BaseInfoPtr)
{
    Dem_TimeBasedInfoConstPtrType debounceTimePtr = (Dem_TimeBasedInfoPtrType)BaseInfoPtr;
    boolean                       ret             = FALSE;

    switch (debounceTimePtr->OldTimerState)
    {
    case DEM_DEBOUNCETIME_FROZENPASSED:
    case DEM_DEBOUNCETIME_FROZENFAILED:
    case DEM_DEBOUNCETIME_FROZENFAILEDFDC:
    case DEM_DEBOUNCETIME_DONEPASSED:
    case DEM_DEBOUNCETIME_DONEFAILED:
    case DEM_DEBOUNCETIME_NONE:
        break;
    case DEM_DEBOUNCETIME_RESETPASSED:
    case DEM_DEBOUNCETIME_RESETFAILED:
    case DEM_DEBOUNCETIME_COUNTPASSED:
    case DEM_DEBOUNCETIME_COUNTFAILED:
    case DEM_DEBOUNCETIME_COUNTFAILEDFDC:
        ret = TRUE;
        break;
    default:
        /** idle */
        break;
    }
    return ret;
}
/* PRQA S 0311 -- */
/* PRQA S 0310 -- */
#endif

#if (defined(DEM_EVENT_MONOTOR_DEBOUNCCE))
/* PRQA S 0310 ++ */ /* VL_Dem_0310 */
/**************Debounce Monitor Internal***************/
/**
 * @brief         Debounce Monitor Internal initialization
 * @param[in]     MonitorInternalPtr: Pointer to the monitor internal information
 * @param[in]     EventId: The event ID
 * @param[in]     SatelliteData: Satellite Data
 * @return        Dem_BaseInfoPtrType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_BaseInfoPtrType Dem_DebounceMonitorInternalInit(
    Dem_MonitorInternalInfoType* const MonitorInternalPtr,
    Dem_EventIdType                    EventId,
    Dem_SatelliteDataType              SatelliteData)
{
    Dem_BaseInfoPtrType ret =
        Dem_DebounceBaseInfoInit(&(MonitorInternalPtr->Base), &Dem_DebounceMonitorInternalFunctionTable, EventId);
    MonitorInternalPtr->OldMonitorStatus = (uint8)Dem_GetSatelliteDebounceData(SatelliteData);
    MonitorInternalPtr->NewMonitorStatus = MonitorInternalPtr->OldMonitorStatus;
    return ret;
}

/**
 * @brief Debounce Monitor Internal Calculate function
 */
/* PRQA S 6070,6030 ++ */ /* VL_MTR_Dem_STCAL,VL_MTR_Dem_STMIF */
DEM_LOCAL boolean Dem_DebounceMonitorInternalCalculate(Dem_BaseInfoPtrType BaseInfoPtr)
{
    boolean                        ret                = FALSE;
    boolean                        debouncingChanged  = FALSE;
    Dem_MonitorInternalInfoPtrType monitorInternalPtr = (Dem_MonitorInternalInfoPtrType)BaseInfoPtr;
    Dem_EventStatusType            monitorTrigger     = BaseInfoPtr->MonitorTrigger;

    /** No debouncing for this event supported */
    if ((monitorTrigger == DEM_EVENT_STATUS_PREPASSED) || (monitorTrigger == DEM_EVENT_STATUS_PREFAILED))
    {
#if (DEM_DEV_ERROR_DETECT == STD_ON)
        Dem_DetReportError(DEM_SID_INTERNALAPIID, DEM_E_PARAM_DATA);
#endif
    }
    else
    {
        if ((Dem_CheckECFulfilled(BaseInfoPtr->EventId) == TRUE)
            && (Dem_CheckEventDTCSettingState(BaseInfoPtr->EventId) == FALSE))
        {
            switch (monitorTrigger)
            {
            case DEM_EVENT_STATUS_FAILED:
                monitorInternalPtr->NewMonitorStatus = DEM_DEBOUNCEMONITORINTERNAL_FAILED;
                if (monitorInternalPtr->OldMonitorStatus != DEM_DEBOUNCEMONITORINTERNAL_FAILED)
                {
                    Dem_SetMaximumFdcExceeded(BaseInfoPtr);
                    if (monitorInternalPtr->OldMonitorStatus != DEM_DEBOUNCEMONITORINTERNAL_FDCTHRESHOLDREACHED)
                    {
                        Dem_SetFdcThresholdExceeded(BaseInfoPtr);
                        Dem_SetRequestFdcProcessing(BaseInfoPtr);
                        debouncingChanged = TRUE;
                    }
                }
                ret = TRUE;
                break;
            case DEM_EVENT_STATUS_PASSED:
                monitorInternalPtr->NewMonitorStatus = DEM_DEBOUNCEMONITORINTERNAL_PASSED;
                ret                                  = TRUE;
                break;
            case DEM_EVENT_STATUS_FDC_THRESHOLD_REACHED:
                monitorInternalPtr->NewMonitorStatus = DEM_DEBOUNCEMONITORINTERNAL_FDCTHRESHOLDREACHED;
                Dem_SetFdcThresholdExceeded(BaseInfoPtr);
                Dem_SetRequestFdcProcessing(BaseInfoPtr);
                ret               = TRUE;
                debouncingChanged = TRUE;
                break;
            default:
                /** idle */
                break;
            }

            if (ret == TRUE)
            {
                if (((debouncingChanged == TRUE)
                     || (monitorInternalPtr->NewMonitorStatus != monitorInternalPtr->OldMonitorStatus))
#if (DEM_GENERAL_FF_TRIGGER_ON_EVERY_TEST_FAILED == STD_ON)
                    || (monitorTrigger == DEM_EVENT_STATUS_FAILED)
#endif
                )
                {
                    Dem_SetDebouncingChanged(BaseInfoPtr);
                }
            }
        }
    }
    return ret;
}
/* PRQA S 6070,6030 -- */

/**
 * @brief Debounce Monitor Internal Freeze function
 */
DEM_LOCAL void Dem_DebounceMonitorInternalFreeze(Dem_BaseInfoConstPtrConstType BaseInfoPtr)
{
    /** No action necessary */
    DEM_PARAM_UNUSED(BaseInfoPtr);
}

/* PRQA S 0311 ++ */ /* VL_Dem_0311 */
/**
 * @brief Debounce Monitor Internal Reset function
 */
DEM_LOCAL void Dem_DebounceMonitorInternalReset(Dem_BaseInfoConstPtrConstType BaseInfoPtr)
{
    Dem_MonitorInternalInfoPtrType monitorInternalPtr = (Dem_MonitorInternalInfoPtrType)BaseInfoPtr;
    monitorInternalPtr->NewMonitorStatus              = DEM_DEBOUNCEMONITORINTERNAL_NONE;
}

/**
 * @brief Debounce Monitor Internal Get Debouncing State function
 */
/* PRQA S 6030,6070 ++ */ /* VL_MTR_Dem_STMIF,VL_MTR_Dem_STCAL */
DEM_LOCAL Dem_DebouncingStateType
    Dem_DebounceMonitorInternalGetDebouncingState(Dem_BaseInfoConstPtrConstType BaseInfoPtr)
{
    Dem_DebouncingStateType debouncingState = 0u;
    Dem_EventIdType         eventId         = BaseInfoPtr->EventId;

    if (Dem_GetDebounceGetFDC(eventId) != NULL_PTR)
    {
        sint8 lFDC;
        if ((*Dem_GetDebounceGetFDC(eventId))(&lFDC) == E_OK)
        {
            /* PRQA S 2152 ++ */ /* VL_Dem_2152 */
            if ((sint32)lFDC == (sint32)DEM_FDC_MAX_VALUE)
            {
                debouncingState = Dem_SetFinallyDefective(debouncingState);
                debouncingState = Dem_SetTestComplete(debouncingState);
                if ((Dem_CheckECFulfilled(eventId) == TRUE) && (Dem_CheckEventDTCSettingState(eventId) == FALSE)
                    && (Dem_CheckSCFulfilled(eventId) == TRUE))
                {
                    debouncingState = Dem_SetDtrUpdate(debouncingState);
                }
            }
            else if ((sint32)lFDC == (sint32)DEM_FDC_MIN_VALUE)
            {
                debouncingState = Dem_SetTestComplete(debouncingState);
                if ((Dem_CheckECFulfilled(eventId) == TRUE) && (Dem_CheckEventDTCSettingState(eventId) == FALSE)
                    && (Dem_CheckSCFulfilled(eventId) == TRUE))
                {
                    debouncingState = Dem_SetDtrUpdate(debouncingState);
                }
            }
            else if ((sint32)lFDC > (sint32)DEM_FDC_INIT_VALUE)
            {
                debouncingState = Dem_SetTemporarilyDefective(debouncingState);
            }
            else if ((sint32)lFDC < (sint32)DEM_FDC_INIT_VALUE)
            {
                debouncingState = Dem_SetTemporarilyHealed(debouncingState);
            }
            else
            {
                /** debouncing state 0 */
            }
            /* PRQA S 2152 -- */
        }
    }
    return debouncingState;
}
/* PRQA S 6030,6070 -- */

#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
/**
 * @brief Debounce Monitor Internal Update Debounce Value Max function
 */
DEM_LOCAL void Dem_DebounceMonitorInternalUpdateDebounceValueMax(Dem_BaseInfoConstPtrConstType BaseInfoPtr)
{
    Dem_MonitorInternalInfoConstPtrType monitorInternalPtr = (Dem_MonitorInternalInfoPtrType)BaseInfoPtr;

    if (monitorInternalPtr->NewMonitorStatus == DEM_DEBOUNCEMONITORINTERNAL_FAILED)
    {
        /* PRQA S 2152 ++ */ /* VL_Dem_2152 */
        Dem_SetDebounceValueMaxByEvent(BaseInfoPtr->EventId, (sint16)DEM_FDC_MAX_VALUE);
        /* PRQA S 2152 -- */
    }
}

/**
 * @brief Debounce Monitor Internal Reset Debounce Value Max function
 */
DEM_LOCAL void Dem_DebounceMonitorInternalResetDebounceValueMax(Dem_BaseInfoConstPtrConstType BaseInfoPtr)
{
    Dem_SetDebounceValueMaxByEvent(BaseInfoPtr->EventId, DEM_DEBOUNCE_INIT_VALUE);
}
#endif

/**
 * @brief Debounce Monitor Internal Get Fault Detection Counter function
 */
DEM_LOCAL Std_ReturnType
    Dem_DebounceMonitorInternalGetFaultDetectionCounter(Dem_BaseInfoConstPtrConstType BaseInfoPtr, sint8* FDC)
{
    Dem_EventIdType eventId = BaseInfoPtr->EventId;
    Std_ReturnType  ret;

    if (Dem_GetDebounceGetFDC(eventId) != NULL_PTR)
    {
        sint8 lFdc;
        ret = (*Dem_GetDebounceGetFDC(eventId))(&lFdc);
        if (ret == E_OK)
        {
            *FDC = lFdc;
        }
    }
    else
    {
        ret = DEM_E_NO_FDC_AVAILABLE;
    }
    return ret;
}

#if ((defined(DEM_MAX_FDC_SINCE_LAST_CLEAR)) || (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE)))
/**
 * @brief Debounce Monitor Internal Get Max Fault Detection Counter function
 */
DEM_LOCAL Std_ReturnType
    Dem_DebounceMonitorInternalGetMaxFaultDetectionCounter(Dem_BaseInfoConstPtrConstType BaseInfoPtr, sint8* FDC)
{
    *FDC = (sint8)Dem_GetDebounceValueMaxByEvent(BaseInfoPtr->EventId);
    return E_OK;
}
#endif

/**
 * @brief Debounce Monitor Internal Get Debouncing Result function
 */
DEM_LOCAL Dem_SatelliteDataType Dem_DebounceMonitorInternalGetDebouncingResult(
    Dem_BaseInfoConstPtrConstType BaseInfoPtr,
    Dem_SatelliteDataType         SatelliteData)
{
    Dem_MonitorInternalInfoConstPtrType monitorInternalPtr = (Dem_MonitorInternalInfoPtrType)BaseInfoPtr;
    Dem_SatelliteDataType               ret =
        Dem_SetSatelliteDebounceData(SatelliteData, (Dem_DebounceDataType)monitorInternalPtr->NewMonitorStatus);
    return ret;
}
/* PRQA S 0311 -- */
/* PRQA S 0310 -- */
#endif
/* PRQA S 0759 -- */

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif
