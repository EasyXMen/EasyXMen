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
 **  @file               : Dem_Indicator.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : Implementation file of Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_INDICATOR_H
#define DEM_INDICATOR_H

/* =================================================== inclusions =================================================== */
#include "Dem_Lcfg.h"
#if (DEM_INDICATOR_NUMBER > 0u)

/* ========================================== internal function declarations ======================================== */
/**
 * @brief         Update user-defined indicator counters in enable
 * @param[in]     IndicatorId: Indicator Id
 * @param[in]     IndicatorBehaviour: Behaviour of the indicator
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_EableIndicator(Dem_IndicatorNumType IndicatorId, Dem_IndicatorStatusType IndicatorBehaviour);

/**
 * @brief         Update user-defined indicator counters in disable
 * @param[in]     IndicatorId: Indicator Id
 * @param[in]     IndicatorBehaviour: Behaviour of the indicator
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DisableIndicator(Dem_IndicatorNumType IndicatorId, Dem_IndicatorStatusType IndicatorBehaviour);

/**
 * @brief         Updates all global user indicators for an event
 * @param[in]     IndicatorId: Indicator Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_UpdateNormalIndicator(Dem_IndicatorNumType IndicatorId);

#if ((DEM_EVENT_FAILURE_COUNTER == STD_ON) && (DEM_INDICATOR_MIL_ENABLE == STD_ON))
/**
 * @brief         A DemIndicatorAttribute with a configured DemMILIndicatorRef, shall use the
 * DemEventConfirmationThreshold instead of the DemIndicatorFailureCycleCounterThreshold to calculate the indicator On
 * Criteria (WarningIndicatorOnCriteriaFulfilled).
 * @param[in]     EventId: Event ID
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_EventCheckIndicatorIsMil(Dem_EventIdType EventId);
#endif

/**
 * @brief         Calculate event has Warning Indicator On Criteria Fulfilled
 * @param[in]     EventId: Event ID
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_EventCheckWIROnFulfilled(Dem_EventIdType EventId);

/**
 * @brief         If indicators are configured, initialize all indicator state counters.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_IndicatorInit(void);

/* ========================================== internal function definitions ========================================= */
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Get DemIndicatorBehaviour information based on IndicatorAttributeRef
 * @param[in]     IndicatorAttributeRef: Reference indicator attribute
 * @return        Dem_IndicatorStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_IndicatorStatusType Dem_GetIndicatorBehaviour(Dem_IndicatorAttrNumType IndicatorAttributeRef)
{
    return Dem_GlobalConfigPtr->PBIndicatorAttr[IndicatorAttributeRef];
}

/**
 * @brief         check if the event has configured any indicator.
 * @param[in]     EventId: Event ID
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckHaveIndicator(Dem_EventIdType EventId)
{
    return (Dem_GetIAStartOfEvent(EventId) < Dem_GetIAEndRefEndOfEvent(EventId)); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

#if (DEM_INDICATOR_BLINKING_ENABLE == STD_ON)
/**
 * @brief         Get Indicator Blinking Counter
 * @param[in]     IndicatorId: Indicator Id
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_IndicatorCounterType Dem_GetIndicatorBlinkingCounter(Dem_IndicatorNumType IndicatorId)
{
    return Dem_GetIndicatorStatusInfo()[IndicatorId].BlinkingCounter;
}

/**
 * @brief         Set Indicator Blinking Counter
 * @param[in]     IndicatorId: Indicator Id
 * @param[in]     Counter: Blink counter
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void
    Dem_SetIndicatorBlinkingCounter(Dem_IndicatorNumType IndicatorId, Dem_IndicatorCounterType Counter)
{
    Dem_GetIndicatorStatusInfo()[IndicatorId].BlinkingCounter = Counter;
}
#endif

#if (DEM_INDICATOR_CONTINUOUS_ENABLE == STD_ON)
/**
 * @brief         Get Indicator Continuous Counter
 * @param[in]     IndicatorId: Indicator Id
 * @return        Dem_IndicatorStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_IndicatorCounterType Dem_GetIndicatorContinuousCounter(Dem_IndicatorNumType IndicatorId)
{
    return Dem_GetIndicatorStatusInfo()[IndicatorId].ContinuousCounter;
}

/**
 * @brief         Set Indicator Continuous Counter
 * @param[in]     IndicatorId: Indicator Id
 * @param[in]     Counter: Continuous counter
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void
    Dem_SetIndicatorContinuousCounter(Dem_IndicatorNumType IndicatorId, Dem_IndicatorCounterType Counter)
{
    Dem_GetIndicatorStatusInfo()[IndicatorId].ContinuousCounter = Counter;
}
#endif

#if (DEM_J1939_SUPPORT == STD_ON)
#if (DEM_INDICATOR_FAST_FLASH_ENABLE == STD_ON)
/**
 * @brief         Get Indicator Fast Flash Counter
 * @param[in]     IndicatorId: Indicator Id
 * @return        Dem_IndicatorStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_IndicatorCounterType Dem_GetIndicatorFastFlashCounter(Dem_IndicatorNumType IndicatorId)
{
    return Dem_GetIndicatorStatusInfo()[IndicatorId].FastFlashCounter;
}

/**
 * @brief         Set Indicator Fast Flash Counter
 * @param[in]     IndicatorId: Indicator Id
 * @param[in]     Counter: Fast flash counter
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void
    Dem_SetIndicatorFastFlashCounter(Dem_IndicatorNumType IndicatorId, Dem_IndicatorCounterType Counter)
{
    Dem_GetIndicatorStatusInfo()[IndicatorId].FastFlashCounter = Counter;
}
#endif

#if (DEM_INDICATOR_SLOW_FLASH_ENABLE == STD_ON)
/**
 * @brief         Get Indicator SlowFlash Counter
 * @param[in]     IndicatorId: Indicator Id
 * @return        Dem_IndicatorStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_IndicatorCounterType Dem_GetIndicatorSlowFlashCounter(Dem_IndicatorNumType IndicatorId)
{
    return Dem_GetIndicatorStatusInfo()[IndicatorId].SlowFlashCounter;
}

/**
 * @brief         Set Indicator SlowFlash Counter
 * @param[in]     IndicatorId: Indicator Id
 * @param[in]     Counter: Slow flash counter
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void
    Dem_SetIndicatorSlowFlashCounter(Dem_IndicatorNumType IndicatorId, Dem_IndicatorCounterType Counter)
{
    Dem_GetIndicatorStatusInfo()[IndicatorId].SlowFlashCounter = Counter;
}
#endif
#endif

/**
 * @brief         Get Indicator Status
 * @param[in]     IndicatorId: Indicator Id
 * @return        Dem_IndicatorStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_IndicatorStatusType Dem_GetStatusOfIndicator(Dem_IndicatorNumType IndicatorId)
{
    return Dem_GetIndicatorStatusInfo()[IndicatorId].IndicatorStatus;
}

/**
 * @brief         Set Indicator Status
 * @param[in]     IndicatorId: Indicator Id
 * @param[in]     IndicatorStatus: Status of indicator
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void
    Dem_SetStatusOfIndicator(Dem_IndicatorNumType IndicatorId, Dem_IndicatorStatusType IndicatorStatus)
{
    Dem_GetIndicatorStatusInfo()[IndicatorId].IndicatorStatus = IndicatorStatus;
}

/**
 * @brief Update user-defined indicator counters in enable
 */
DEM_LOCAL void Dem_EableIndicator(Dem_IndicatorNumType IndicatorId, Dem_IndicatorStatusType IndicatorBehaviour)
{
    switch (IndicatorBehaviour)
    {
#if (DEM_INDICATOR_CONTINUOUS_ENABLE == STD_ON)
    case DEM_INDICATOR_CONTINUOUS:
        Dem_SetIndicatorContinuousCounter(IndicatorId, (Dem_GetIndicatorContinuousCounter(IndicatorId) + 1u));
        break;
#endif
#if (DEM_INDICATOR_BLINKING_ENABLE == STD_ON)
    case DEM_INDICATOR_BLINKING:
        Dem_SetIndicatorBlinkingCounter(IndicatorId, (Dem_GetIndicatorBlinkingCounter(IndicatorId) + 1u));
        break;
#endif
#if ((DEM_INDICATOR_CONTINUOUS_ENABLE == STD_ON) && (DEM_INDICATOR_BLINKING_ENABLE == STD_ON))
    case DEM_INDICATOR_BLINK_CONT:
        Dem_SetIndicatorBlinkingCounter(IndicatorId, (Dem_GetIndicatorBlinkingCounter(IndicatorId) + 1u));
        Dem_SetIndicatorContinuousCounter(IndicatorId, (Dem_GetIndicatorContinuousCounter(IndicatorId) + 1u));
        break;
#endif
#if (DEM_J1939_SUPPORT == STD_ON)
#if (DEM_INDICATOR_FAST_FLASH_ENABLE == STD_ON)
    case DEM_INDICATOR_FAST_FLASH:
        Dem_SetIndicatorFastFlashCounter(IndicatorId, (Dem_GetIndicatorFastFlashCounter(IndicatorId) + 1u));
        break;
#endif
#if (DEM_INDICATOR_SLOW_FLASH_ENABLE == STD_ON)
    case DEM_INDICATOR_SLOW_FLASH:
        Dem_SetIndicatorSlowFlashCounter(IndicatorId, (Dem_GetIndicatorSlowFlashCounter(IndicatorId) + 1u));
        break;
#endif
#endif
    default:
        /** Mirsa C */
        break;
    }
}

/**
 * @brief Update user-defined indicator counters in disable
 */
DEM_LOCAL void Dem_DisableIndicator(Dem_IndicatorNumType IndicatorId, Dem_IndicatorStatusType IndicatorBehaviour)
{
    switch (IndicatorBehaviour)
    {
#if (DEM_INDICATOR_CONTINUOUS_ENABLE == STD_ON)
    case DEM_INDICATOR_CONTINUOUS:
        Dem_SetIndicatorContinuousCounter(IndicatorId, (Dem_GetIndicatorContinuousCounter(IndicatorId) - 1u));
        break;
#endif
#if (DEM_INDICATOR_BLINKING_ENABLE == STD_ON)
    case DEM_INDICATOR_BLINKING:
        Dem_SetIndicatorBlinkingCounter(IndicatorId, (Dem_GetIndicatorBlinkingCounter(IndicatorId) - 1u));
        break;
#endif
#if ((DEM_INDICATOR_CONTINUOUS_ENABLE == STD_ON) && (DEM_INDICATOR_BLINKING_ENABLE == STD_ON))
    case DEM_INDICATOR_BLINK_CONT:
        Dem_SetIndicatorBlinkingCounter(IndicatorId, (Dem_GetIndicatorBlinkingCounter(IndicatorId) - 1u));
        Dem_SetIndicatorContinuousCounter(IndicatorId, (Dem_GetIndicatorContinuousCounter(IndicatorId) - 1u));
        break;
#endif
#if (DEM_J1939_SUPPORT == STD_ON)
#if (DEM_INDICATOR_FAST_FLASH_ENABLE == STD_ON)
    case DEM_INDICATOR_SLOW_FLASH:
        Dem_SetIndicatorFastFlashCounter(IndicatorId, (Dem_GetIndicatorFastFlashCounter(IndicatorId) - 1u));
        break;
#endif
#if (DEM_INDICATOR_SLOW_FLASH_ENABLE == STD_ON)
    case DEM_INDICATOR_FAST_FLASH:
        Dem_SetIndicatorSlowFlashCounter(IndicatorId, (Dem_GetIndicatorSlowFlashCounter(IndicatorId) - 1u));
        break;
#endif
#endif
    default:
        /** Mirsa C */
        break;
    }
}

/**
 * @brief         Update user-defined indicator counters
 * Removes the event's contribution to the indicator states. Basically this subtracts 1 from each indicators continuous
 * or blinking count, if the indicator is enabled by the given event
 * @param[in]     EventId: Event ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_DisableNormalIndicatorByEvent(Dem_EventIdType EventId)
{
    for (Dem_IndicatorAttrNumType start = Dem_GetIAStartOfEvent(EventId); start < Dem_GetIAEndRefEndOfEvent(EventId);
         ++start)
    {
        Dem_DisableIndicator(Dem_GetRefOfInd(start), Dem_GetIndicatorBehaviour(start));
    }
}

/**
 * @brief Updates all global user indicators for an event
 */
DEM_LOCAL void Dem_UpdateNormalIndicator(Dem_IndicatorNumType IndicatorId)
{
    Dem_IndicatorStatusType indicatorState = DEM_INDICATOR_OFF;
#if ((DEM_INDICATOR_CONTINUOUS_ENABLE == STD_ON) && (DEM_INDICATOR_BLINKING_ENABLE == STD_ON))
    if ((Dem_GetIndicatorBlinkingCounter(IndicatorId) > 0u) && (Dem_GetIndicatorContinuousCounter(IndicatorId) > 0u))
    {
        indicatorState = DEM_INDICATOR_BLINK_CONT;
    }
    else
#endif
#if (DEM_INDICATOR_CONTINUOUS_ENABLE == STD_ON)
        if (Dem_GetIndicatorContinuousCounter(IndicatorId) > 0u)
    {
        indicatorState = DEM_INDICATOR_CONTINUOUS;
    }
    else
#endif
#if (DEM_INDICATOR_BLINKING_ENABLE == STD_ON)
        if (Dem_GetIndicatorBlinkingCounter(IndicatorId) > 0u)
    {
        indicatorState = DEM_INDICATOR_BLINKING;
    }
    else
#endif
#if (DEM_J1939_SUPPORT == STD_ON)
#if (DEM_INDICATOR_FAST_FLASH_ENABLE == STD_ON)
        if (Dem_GetIndicatorFastFlashCounter(IndicatorId) > 0u)
    {
        indicatorState = DEM_INDICATOR_FAST_FLASH;
    }
    else
#endif
#if (DEM_INDICATOR_SLOW_FLASH_ENABLE == STD_ON)
        if (Dem_GetIndicatorSlowFlashCounter(IndicatorId) > 0u)
    {
        indicatorState = DEM_INDICATOR_SLOW_FLASH;
    }
    else
#endif
#endif
    {
        /** misra c */
    }
    Dem_SetStatusOfIndicator(IndicatorId, indicatorState);
}

/**
 * @brief         Updates all global user indicators for an event
 * @param[in]     EventId: Event ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_UpdateNormalIndicatorByEvent(Dem_EventIdType EventId)
{
    for (Dem_IndicatorAttrNumType start = Dem_GetIAStartOfEvent(EventId); start < Dem_GetIAEndRefEndOfEvent(EventId);
         ++start)
    {
        Dem_IndicatorNumType indicatorId = Dem_GetRefOfInd(start);
        Dem_UpdateNormalIndicator(indicatorId);
    }
}

/**
 * @brief         Decrement indicator counters and update global indicator states
 * @param[in]     EventId: Event ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_IndicatorDisable(Dem_EventIdType EventId)
{
    Dem_DisableNormalIndicatorByEvent(EventId);
    Dem_UpdateNormalIndicatorByEvent(EventId);
}

#if ((DEM_EVENT_FAILURE_COUNTER == STD_ON) && (DEM_INDICATOR_MIL_ENABLE == STD_ON))
/**
 * @brief A DemIndicatorAttribute with a configured DemMILIndicatorRef, shall use the DemEventConfirmationThreshold
 * instead of the DemIndicatorFailureCycleCounterThreshold to calculate the indicator On Criteria
 * (WarningIndicatorOnCriteriaFulfilled).
 */
DEM_LOCAL boolean Dem_EventCheckIndicatorIsMil(Dem_EventIdType EventId)
{
    boolean isMil = FALSE;
    for (Dem_IndicatorAttrNumType start = Dem_GetIAStartOfEvent(EventId); start < Dem_GetIAEndRefEndOfEvent(EventId);
         ++start)
    {
        Dem_IndicatorNumType indicatorId = Dem_GetRefOfInd(start);
        for (uint8 client = Dem_GetClientStartRefOfEvent(EventId); client < Dem_GetClientEndRefOfEvent(EventId);
             ++client)
        {
            if (indicatorId == Dem_GetMILIndOfMemSet(Dem_GetMemorySetRefOfClient(Dem_GetClientOfEventRef(client))))
            {
                isMil = TRUE;
                break;
            }
        }
    }
    return isMil;
}
#endif

/**
 * @brief Calculate event has Warning Indicator On Criteria Fulfilled
 */
DEM_LOCAL boolean Dem_EventCheckWIROnFulfilled(Dem_EventIdType EventId)
{
    boolean ret = FALSE;
#if ((DEM_EVENT_FAILURE_COUNTER == STD_ON) && (DEM_INDICATOR_MIL_ENABLE == STD_ON))
    /** A DemIndicatorAttribute with a configured DemMILIndicatorRef, shall use the DemEventConfirmationThreshold
     * instead of the DemIndicatorFailureCycleCounterThreshold to calculate the indicator On Criteria
     * (WarningIndicatorOnCriteriaFulfilled). */
    boolean isMil = Dem_EventCheckIndicatorIsMil(EventId);
#endif
    for (Dem_IndicatorAttrNumType start = Dem_GetIAStartOfEvent(EventId); start < Dem_GetIAEndRefEndOfEvent(EventId);
         ++start)
    {
#if (DEM_EVENT_FAILURE_COUNTER == STD_ON)
        /* PRQA S 2109 ++ */ /* VL_Dem_2109 */
        uint8 fThrOfInd =
#if (DEM_INDICATOR_MIL_ENABLE == STD_ON)
            (isMil == TRUE) ? Dem_GetEventConfirmationThreshold(EventId) :
#endif
                            Dem_GetFThrOfInd(start);
        /* PRQA S 2109 -- */
        uint8 failureCounter = Dem_GetEventFailureCounter(EventId) + 1u;
        if (failureCounter == fThrOfInd)
#endif
        {
            Dem_IndicatorNumType indicatorId = Dem_GetRefOfInd(start);
            Dem_EableIndicator(indicatorId, Dem_GetIndicatorBehaviour(start));
            Dem_UpdateNormalIndicator(indicatorId);
        }
#if (DEM_EVENT_FAILURE_COUNTER == STD_ON)
        if (failureCounter >= fThrOfInd)
#endif
        {
            ret = TRUE;
        }
    }
    return ret;
}

/**
 * @brief If indicators are configured, initialize all indicator state counters.
 */
DEM_LOCAL void Dem_IndicatorInit(void)
{
    /** If indicators are configured, initialize all indicator state counters */
    for (uint8 indicatorId = 0u; indicatorId < DEM_INDICATOR_NUMBER; ++indicatorId)
    {
#if (DEM_INDICATOR_CONTINUOUS_ENABLE == STD_ON)
        Dem_SetIndicatorContinuousCounter(indicatorId, 0u);
#endif
#if (DEM_INDICATOR_BLINKING_ENABLE == STD_ON)
        Dem_SetIndicatorBlinkingCounter(indicatorId, 0u);
#endif
#if (DEM_J1939_SUPPORT == STD_ON)
#if (DEM_INDICATOR_FAST_FLASH_ENABLE == STD_ON)
        Dem_SetIndicatorFastFlashCounter(indicatorId, 0u);
#endif
#if (DEM_INDICATOR_SLOW_FLASH_ENABLE == STD_ON)
        Dem_SetIndicatorSlowFlashCounter(indicatorId, 0u);
#endif
#endif
    }
}

/**
 * @brief         Initializes indicators States
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_InitIndicatorStates(void)
{
    /** If indicators are configured, initialize all indicator state counters */
    for (uint8 indicatorId = 0u; indicatorId < DEM_INDICATOR_NUMBER; ++indicatorId)
    {
        Dem_UpdateNormalIndicator(indicatorId);
    }
}

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

#if ((DEM_OBDII_SUPPORT == STD_ON) || (DEM_J1939_READING_DTC_SUPPORT == STD_ON) || (DEM_J1939_DM31_SUPPORT == STD_ON))
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Returns if the event supports the Special indicator.
 * @param[in]     EventId: Event ID
 * @param[in]     IndicatorId: Indicator Id
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_EventCheckHaveSpecialIndicator(Dem_EventIdType EventId, uint8 IndicatorId)
{
    boolean ret = FALSE;
#if (DEM_INDICATOR_NUMBER > 0u)
    for (Dem_IndicatorAttrNumType start = Dem_GetIAStartOfEvent(EventId); start < Dem_GetIAEndRefEndOfEvent(EventId);
         ++start)
    {
        if (IndicatorId == Dem_GetRefOfInd(start))
        {
            ret = TRUE;
            break;
        }
    }
#endif
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

#endif
