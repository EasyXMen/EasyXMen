/* PRQA S 3108++ */
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
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : Dem_SubExt.c                                                **
**                                                                            **
**  Created on  :                                                             **
**  Author      : tao.yu                                                      **
**  Vendor      : i-soft                                                      **
**  DESCRIPTION : API definitions                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/
#include "Dem_Internal.h"

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
/*
 * Brief               Dem_ExtendedDataStorage
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId && pEventBuffer && Trigger
 * Param-Name[out]     None
 * Param-Name[in/out]  pEntry
 * Return              None
 */
/*************************************************************************/
/* PRQA S 1505,3432++ */ /* MISRA Rule 8.7,20.7 */
DEM_LOCAL FUNC(void, DEM_CODE) Dem_ExtendedDataStorage(
    Dem_EventIdType IntId,
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA) pEntry,
    uint8 Trigger);
/* PRQA S 1505,3432-- */ /* MISRA Rule 8.7,20.7 */
/*******************************************************************************
**                      Private Function Definitions                         **
*******************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#if (DEM_J1939_SUPPORT == STD_ON)
#if (DEM_J1939_READING_DTC_SUPPORT == STD_ON)
#if (DEM_INDICATOR_NUM > 0)
/*************************************************************************/
/*
 * Brief               The function returns the composite lamp status of the filtered DTCs
 * ServiceId           none
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     LampStatus:Receives the commulated lamp status
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
/* PRQA S 1532,3432,3206,3673++ */ /* MISRA Rule 8.7,20.7,2.7,8.13 */
FUNC(void, DEM_CODE)
Dem_SubGetLampStatusOfJ1939DcmSetDTCFilter(P2VAR(Dem_J1939DcmLampStatusType, AUTOMATIC, DEM_APPL_DATA) LampStatus)
/* PRQA S 1532,3432,3206,3673-- */ /* MISRA Rule 8.7,20.7,2.7,8.13 */
{
/*Composite Amber Warning Lamp Status*/
#if (DEM_AMBER_WARING_LAMP_INDICATOR != DemIndicator_INVALID)
    if (DemWIRStatus[DEM_AMBER_WARING_LAMP_INDICATOR] != DEM_INDICATOR_OFF)
    {
        /* req SWS_Dem_00866 */
        *LampStatus |= 0x0400u;
    }
    if ((DemWIRStatus[DEM_AMBER_WARING_LAMP_INDICATOR] == DEM_INDICATOR_OFF)
        || (DemWIRStatus[DEM_AMBER_WARING_LAMP_INDICATOR] == DEM_INDICATOR_CONTINUOUS))
    {
        /* req SWS_Dem_00867 */
        *LampStatus |= 0x000cu;
    }
    if (DemWIRStatus[DEM_AMBER_WARING_LAMP_INDICATOR] == DEM_INDICATOR_SLOW_FLASH)
    {
        /* req SWS_Dem_00868 */
        *LampStatus &= ~0x000cu;
    }
    if (DemWIRStatus[DEM_AMBER_WARING_LAMP_INDICATOR] == DEM_INDICATOR_FAST_FLASH)
    {
        /* req SWS_Dem_00869 */
        *LampStatus &= ~0x0008u;
    }
#endif
/*Composite Protect Lamp Status*/
#if (DEM_PROTECT_LAMP_INDICATOR != DemIndicator_INVALID)
    if (DemWIRStatus[DEM_PROTECT_LAMP_INDICATOR] != DEM_INDICATOR_OFF)
    {
        /* req SWS_Dem_00870 */
        *LampStatus |= 0x0100u;
    }
    if ((DemWIRStatus[DEM_PROTECT_LAMP_INDICATOR] == DEM_INDICATOR_OFF)
        || (DemWIRStatus[DEM_PROTECT_LAMP_INDICATOR] == DEM_INDICATOR_CONTINUOUS))
    {
        /* req SWS_Dem_00871 */
        *LampStatus |= 0x0003u;
    }
    if (DemWIRStatus[DEM_PROTECT_LAMP_INDICATOR] == DEM_INDICATOR_SLOW_FLASH)
    {
        /* req SWS_Dem_00872 */
        *LampStatus &= ~0x0003u;
    }
    if (DemWIRStatus[DEM_PROTECT_LAMP_INDICATOR] == DEM_INDICATOR_FAST_FLASH)
    {
        /* req SWS_Dem_00873 */
        *LampStatus &= ~0x0002u;
    }
#endif
#if ((DEM_AMBER_WARING_LAMP_INDICATOR == DemIndicator_INVALID) && (DEM_PROTECT_LAMP_INDICATOR == DemIndicator_INVALID))
    DEM_UNUSED(LampStatus);
#endif
}
#endif /* DEM_INDICATOR_NUM > 0 */

/*************************************************************************/
/*
 * Brief               Gets the number of currently filtered DTCs set by the function
 * Dem_J1939DcmSetDTCFilter ServiceId           0x91 Sync/Async          Asynchronous Reentrancy Non
 * Reentrant Param-Name[in]      None Param-Name[out]     NumberOfFilteredDTC: The number of DTCs
 * matching the defined status mask. Param-Name[in/out]  none Return              Status of the
 * operation to retrieve a number of DTC from the Dem
 */
/*************************************************************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(void, DEM_CODE)
Dem_SubInterJ1939DcmGetNumberOfFilteredDTC(
    Dem_EventIdType IntId,
    uint8 entryId,
    P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA) findSameDTC,
    P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA) IsFind)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    const Dem_EventRelateInformationType* pEvent = Dem_GetEventInfo(IntId);
    Dem_J1939DTCFilterInfoType* pFilter = &DemJ1939DTCFilter;
    uint8 iloop;
    uint16 dtcRef;
    uint32 tempDtcValue[DEM_MAX_NUMBER_EVENT_ENTRY_PRIMARY];

    dtcRef = DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef;
    uint16 DemObdDTCRef = DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef;
    const uint32 DemJ1939DTCValue = DemPbCfgPtr->DemObdDTC[DemObdDTCRef].DemJ1939DTCValue;
    if (*IsFind == TRUE)
    {
        uint32 J1939DTC;
        *IsFind = FALSE;
        Dem_GetEventDTC(dtcRef, DEM_DTC_FORMAT_J1939, &J1939DTC);
        if (J1939DTC != 0u)
        {
            tempDtcValue[entryId] = J1939DTC;
            for (iloop = 0; iloop < entryId; iloop++)
            {
                if (tempDtcValue[iloop] == DemJ1939DTCValue)
                {
                    /* find same dtc of diffrence event memory */
                    *findSameDTC = TRUE;
                }
            }
            if (*findSameDTC == FALSE)
            {
                pFilter->NumberOfFilteredDTC++;
            }
            else
            {
                *findSameDTC = FALSE;
            }
        }
    }
}
#endif /* DEM_J1939_SUPPORT == STD_ON */
#endif /* DEM_J1939_READING_DTC_SUPPORT == STD_ON */

#if (DEM_INDICATOR_NUM > 0)
/*************************************************************************/
/*
 * Brief               Gets the next filtered J1939 DTC for DM31 including current LampStatus.
 * ServiceId           none
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     LampStatus:Receives the lamp status returned by this function.
 *                      If the return value of the function is other than
                        DEM_FILTERED_OK this parameter does not contain valid data.
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(void, DEM_CODE)
Dem_SubGetLampStatusOfJ1939DcmGetNextDTCwithLampStatus(
    P2CONST(Dem_IndicatorAttributeType, AUTOMATIC, DEM_APPL_DATA) pWIRAttr,
    P2CONST(Dem_EventRelateInformationType, AUTOMATIC, DEM_APPL_DATA) pEvent,
    P2VAR(Dem_J1939DcmLampStatusType, AUTOMATIC, DEM_APPL_DATA) LampStatus)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
#if ((DEM_INDICATOR_NUM > 0) && (DEM_INDICATOR_ATTRIBUTE_TOTAL_NUM > 0))
    uint8 DemIndicatorRef = pWIRAttr->DemIndicatorRef;
    uint8 DemIndicatorBehaviour = pWIRAttr->DemIndicatorBehaviour;

    /* Amber Warning Lamp */
    if (DemIndicatorRef == DEM_AMBER_WARING_LAMP_INDICATOR)
    {
        if (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_WIR))
        {
            /* SWS_Dem_00891 */
            *LampStatus |= 0x0004u;
        }
        if (DemWIRStatus[DemIndicatorRef] != DEM_INDICATOR_OFF)
        {
            if (DemIndicatorBehaviour == DEM_INDICATOR_CONTINUOUS)
            {
                /* SWS_Dem_00892 */
                *LampStatus |= 0x0C00u;
            }
            if (DemIndicatorBehaviour == DEM_INDICATOR_SLOW_FLASH)
            {
                /* SWS_Dem_00893 */
                *LampStatus |= 0x0000u;
            }
            if (DemIndicatorBehaviour == DEM_INDICATOR_FAST_FLASH)
            {
                /* SWS_Dem_00894 */
                *LampStatus |= 0x0400u;
            }
        }
    }

    /* Protect Lamp */
    if (DemIndicatorRef == DEM_PROTECT_LAMP_INDICATOR)
    {
        if (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_WIR))

        {
            /* SWS_Dem_00895 */
            *LampStatus |= 0x0001u;
        }
        if (DemWIRStatus[DemIndicatorRef] != DEM_INDICATOR_OFF)
        {
            if (DemIndicatorBehaviour == DEM_INDICATOR_CONTINUOUS)
            {
                /* SWS_Dem_00896 */
                *LampStatus |= 0x0300u;
            }
            if (DemIndicatorBehaviour == DEM_INDICATOR_SLOW_FLASH)
            {
                /* SWS_Dem_00897 */
                *LampStatus |= 0x0000u;
            }
            if (DemIndicatorBehaviour == DEM_INDICATOR_FAST_FLASH)
            {
                /* SWS_Dem_00898 */
                *LampStatus |= 0x0100u;
            }
        }
    }
#else
    *LampStatus = 0x00FF;
    DEM_UNUSED(pEvent);
    DEM_UNUSED(pWIRAttr);
#endif /*DEM_INDICATOR_NUM > 0) && (DEM_INDICATOR_ATTRIBUTE_TOTAL_NUM > 0 */
}
#endif /* DEM_INDICATOR_NUM > 0 */

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
/*************************************************************************/
/*
 * Brief               Dem_DebouceProcessPrePassedCounter
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
#if (DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
/* PRQA S 1505,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(Dem_EventStatusType, DEM_CODE)
Dem_DebouceProcessPrePassedCounter(
    P2CONST(Dem_DebounceCounterBasedClassType, AUTOMATIC, DEM_CONST) pCfgCounter,
    P2VAR(Dem_DebounceCounterInfoType, AUTOMATIC, DEM_APPL_DATA) pCounter)
/* PRQA S 1505,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    Dem_EventStatusType status = DEM_EVENT_STATUS_PREPASSED;
    sint16 JumpDownValue = pCfgCounter->DemDebounceCounterJumpDownValue;
    sint16* InternalCounter = &pCounter->InternalDebounceCounter;
    sint16 PassedThreshold = pCfgCounter->DemDebounceCounterPassedThreshold;
    sint16 StepSize = (sint16)pCfgCounter->DemDebounceCounterDecrementStepSize;
    if (*InternalCounter > JumpDownValue)
    {
        if (pCfgCounter->DemDebounceCounterJumpDown == TRUE) /*SWS_Dem_00423*/
        {
            *InternalCounter = JumpDownValue;
        }
    }
    if ((PassedThreshold + StepSize) >= *InternalCounter)
    {
        *InternalCounter = PassedThreshold;
        status = DEM_EVENT_STATUS_PASSED;
    }
    else
    {
        *InternalCounter -= StepSize;
    }
    return status;
}

/*************************************************************************/
/*
 * Brief               Dem_DebouceProcessPreFailedCounter
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
/* PRQA S 1505,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(Dem_EventStatusType, DEM_CODE)
Dem_DebouceProcessPreFailedCounter(
    P2CONST(Dem_DebounceCounterBasedClassType, AUTOMATIC, DEM_CONST) pCfgCounter,
    P2VAR(Dem_DebounceCounterInfoType, AUTOMATIC, DEM_APPL_DATA) pCounter)
/* PRQA S 1505,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    Dem_EventStatusType status = DEM_EVENT_STATUS_PREFAILED;
    sint16 JumpUpValue = pCfgCounter->DemDebounceCounterJumpUpValue;
    sint16* InternalCounter = &pCounter->InternalDebounceCounter;
    sint16 FailedThreshold = pCfgCounter->DemDebounceCounterFailedThreshold;
    sint16 StepSize = (sint16)pCfgCounter->DemDebounceCounterIncrementStepSize;
    if (*InternalCounter < JumpUpValue)
    {
        if (pCfgCounter->DemDebounceCounterJumpUp == TRUE) /*SWS_Dem_00425*/
        {
            *InternalCounter = JumpUpValue;
        }
    }
    if ((FailedThreshold - StepSize) <= *InternalCounter)
    {
        *InternalCounter = FailedThreshold;
        status = DEM_EVENT_STATUS_FAILED;
    }
    else
    {
        *InternalCounter += StepSize;
    }
    return status;
}

/*************************************************************************/
/*
 * Brief               Dem_DebounceProcessCounter
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      pEventBuffer && pEventCfg
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Dem_EventStatusType
 */
/*************************************************************************/
/* PRQA S 1505++ */ /* MISRA Rule 8.7 */
FUNC(Dem_EventStatusType, DEM_CODE)
Dem_DebounceProcessCounter(Dem_EventIdType ExtId, Dem_EventStatusType Status)
/* PRQA S 1505-- */ /* MISRA Rule 8.7 */
{
    Dem_EventIdType IntId = Dem_GetEventInternalId(ExtId);
    const Dem_EventParameterType* pEventCfg = &DemPbCfgPtr->DemEventParameter[IntId];
    if (DEM_DEBOUNCE_COUNTER_BASED == pEventCfg->AlgorithmType)
    {
        const Dem_DebounceCounterBasedClassType* pCfgCounter =
            &DemPbCfgPtr->DemDebounceCounterBasedClass[DemPbCfgPtr->DemEventParameter[IntId].AlgorithmRef];
        Dem_DebounceCounterInfoType* pCounter = &DemDebounceCounterInfo[pEventCfg->AlgorithmIndex];
        Dem_EventStatusType lStatus = Status;

        switch (Status)
        {
        case DEM_EVENT_STATUS_PASSED:
            /* req SWS_Dem_00421 */
            pCounter->InternalDebounceCounter = pCfgCounter->DemDebounceCounterPassedThreshold;
            break;
        case DEM_EVENT_STATUS_FAILED:
            /* req SWS_Dem_00420  */
            pCounter->InternalDebounceCounter = pCfgCounter->DemDebounceCounterFailedThreshold;
            break;
        case DEM_EVENT_STATUS_PREPASSED:
            Status = Dem_DebouceProcessPrePassedCounter(pCfgCounter, pCounter);
            break;
        default:
            /* DEM_EVENT_STATUS_PREFAILED */
            Status = Dem_DebouceProcessPreFailedCounter(pCfgCounter, pCounter);
            break;
        }
    }
    return Status;
}
#endif /* DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0 */

#if (DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
/*************************************************************************/
/*
 * Brief               Dem_DebounceProcessTimer
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      pEventBuffer && pEventCfg
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Dem_EventStatusType
 */
/*************************************************************************/
/* PRQA S 1505++ */ /* MISRA Rule 8.7 */
DEM_LOCAL FUNC(Dem_EventStatusType, DEM_CODE) Dem_DebounceProcessTimer(
    Dem_EventDataBufferType pEventBuffer,
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg)
/* PRQA S 1505-- */ /* MISRA Rule 8.7 */
{
    Dem_EventStatusType Status = pEventBuffer.Status;
    Dem_DebounceTimeInfoType* pTimer = &DemDebounceTimerInfo[pEventCfg->AlgorithmIndex];
    const Dem_DebounceTimeBaseClassType* pTimerCfg =
        &DemPbCfgPtr->DemDebounceTimeBaseClass
             [DemPbCfgPtr->DemEventParameter[Dem_GetEventInternalId(pEventBuffer.ExtId)].AlgorithmRef];

    uint8 chkmask = DEM_EVENT_STATUS_ENABLED_CONDICTION | DEM_EVENT_STATUS_DTC_SETTING;
    const Dem_EventRelateInformationType* pEvent;

    pEvent = Dem_GetEventInfo(Dem_GetEventInternalId(pEventBuffer.ExtId));
    /* req SWS_Dem_00656 */
    if ((pTimer->IsFreezing != FALSE) && (chkmask == DEM_FLAGS_ISSET(pEvent->Status, chkmask)))
    {
        pTimer->IsFreezing = FALSE;
    }
    switch (Status)
    {
    case DEM_EVENT_STATUS_PASSED:
        pTimer->CurStatus = DEM_EVENT_STATUS_PASSED;
        /* req SWS_Dem_00435 */
        pTimer->Timeout = pTimerCfg->DemDebounceTimePassedThreshold;
        break;
    case DEM_EVENT_STATUS_FAILED:
        pTimer->CurStatus = DEM_EVENT_STATUS_FAILED;
        /* req SWS_Dem_00431 */
        pTimer->Timeout = pTimerCfg->DemDebounceTimeFailedThreshold;
        break;
    case DEM_EVENT_STATUS_PREPASSED:
        /* req SWS_Dem_00432 req SWS_Dem_00433 */
        if ((pTimer->CurStatus == DEM_EVENT_STATUS_FAILED) || (pTimer->CurStatus == DEM_EVENT_STATUS_PREFAILED)
            || (pTimer->CurStatus == DEM_EVENT_STATUS_FDC_THRESHOLD_REACHED) || (pTimer->Triggered == FALSE))
        {
            pTimer->CurStatus = DEM_EVENT_STATUS_PREPASSED;
            pTimer->Triggered = TRUE;
            pTimer->Timeout = 0;
        }
        break;
    case DEM_EVENT_STATUS_FDC_THRESHOLD_REACHED:
        break;
    case DEM_EVENT_STATUS_PREFAILED:
        /* Prefailed req SWS_Dem_00429*/
        if ((pTimer->CurStatus == DEM_EVENT_STATUS_PASSED) || (pTimer->CurStatus == DEM_EVENT_STATUS_PREPASSED)
            || (pTimer->Triggered == FALSE))
        {
            pTimer->CurStatus = DEM_EVENT_STATUS_PREFAILED;
            pTimer->Triggered = TRUE;
            pTimer->Timeout = 0;
        }
        break;
    default:
        /*idle*/
        break;
    }
    return Status;
}
#endif /* DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0 */

/*******************************************************************************
**                      Private Function Definitions                         **
*******************************************************************************/
#if (DEM_RATIO_NUM > 0)
/*************************************************************************/
/*
 * Brief               Get the IUMPR Group Denomitor data.
 * ServiceId           Internal Function
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(uint16, DEM_CODE)
Dem_GetIUMPRDenValueByGroup(Dem_IUMPRGroupType Dem_IUMPRGroup)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    uint16 iloop;
    uint16 ret = 0;
    for (iloop = 0; iloop < DEM_RATIO_NUM; iloop++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
    {
        if (DemPbCfgPtr->DemRatio[iloop].DemIUMPRGroup == Dem_IUMPRGroup)
        {
            ret = IUMPRValue[iloop].Denominator;
            break;
        }
    }
    return ret;
}
/*************************************************************************/
/*
 * Brief               Get the IUMPR calculated Numerator data.
 * ServiceId           Internal Function
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(uint16, DEM_CODE)
Dem_GetIUMPRNumValueByGroup(Dem_IUMPRGroupType Dem_IUMPRGroup)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    uint16 iloop = 0;
    uint16 ret = 0;
    while (iloop < DEM_RATIO_NUM) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
    {
        if (DemPbCfgPtr->DemRatio[iloop].DemIUMPRGroup == Dem_IUMPRGroup)
        {
            ret = IUMPRValue[iloop].Numerator;
            break;
        }
        iloop++;
    }
    return ret;
}
#endif

/*************************************************************************/
/*
 * Brief               Dem_UpdateInternalData
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
FUNC(void, DEM_CODE) Dem_UpdateInternalData(uint16 IntId) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    const Dem_EventParameterType* pEventCfg = &DemPbCfg.DemEventParameter[IntId];
    const Dem_EventRelateInformationType* pEvent = &DemEventRelateInformation[IntId];
    const Dem_DTCAttributesType* pDTCAttrCfg;
    const uint8* pMemDest;
    uint16 DTCRef = pEventCfg->DemDTCRef;
    sint8 FDC;
    Dem_EventIdType EventId = Dem_GetEventExternalId(IntId);

    Dem_UpdateFDC(IntId);
    (void)Dem_DTCGetFaultDetectionCounter(EventId, &FDC);
    /* req SWS_Dem_00471 */
    DemInternalData.Occctr = pEvent->OccurrenceCounter;
    if (DTCRef != DEM_DTC_REF_INVALID)
    {
        pDTCAttrCfg = &DemPbCfg.DemDTCAttributes[DemPbCfg.DemDTC[DTCRef].DemDTCAttributesRef];

        if (pDTCAttrCfg->DemAgingAllowed == FALSE)
        {
            /* req SWS_Dem_01044 */
            DemInternalData.AgingUpCnt = 0;
            /* req SWS_Dem_01043 */
            DemInternalData.AgingDownCnt = pDTCAttrCfg->DemAgingCycleCounterThreshold;
        }
        else
        {
            /* req SWS_Dem_00643 */
            DemInternalData.AgingUpCnt = pEvent->AgingCounter;
            /* req SWS_Dem_00673 */
            DemInternalData.AgingDownCnt = pDTCAttrCfg->DemAgingCycleCounterThreshold - pEvent->AgingCounter;
        }

        pMemDest = pDTCAttrCfg->DemMemoryDestinationRef;
        DemInternalData.Ovflind = 0;
#if (DEM_MEM_DEST_MAX_NUM_OF_DTC > 1u)
        for (uint8 iloop = 0; iloop < DEM_MEM_DEST_MAX_NUM_OF_DTC; iloop++)
#else
        uint8 iloop = 0;
#endif /* DEM_MEM_DEST_MAX_NUM_OF_DTC > 1u */
        {
            if (pMemDest[iloop] != DEM_MEM_DEST_INVALID)
            {
                if (DemMemDestInfo[pMemDest[iloop]].OverFlow == TRUE)
                {
                    /*if the memDest have one OverFlow is true, then the internal OverFlow is true
                     * req SWS_Dem_00473 */
                    DemInternalData.Ovflind = 1;
                }
            }
        }
        /* req SWS_Dem_01084 */
        DemInternalData.Significance = pDTCAttrCfg->DemDTCSignificance;
        const FDCInfoType* pFDCInfo = &FDCInfo[DTCRef];

        /* req SWS_Dem_00819 */
        DemInternalData.MaxFDCDuringCurrentCycle = pFDCInfo->MaxFDCDuringCurrentCycle;
#if (DEM_MAXFDCSINCELASTCLEAR_EANLE == STD_ON)
        /* req SWS_Dem_00818 */
        DemInternalData.MaxFDCSinceLastClear = pFDCInfo->MaxFDCSinceLastClear;
#endif /* DEM_MAXFDCSINCELASTCLEAR_EANLE == STD_ON */
        /* req SWS_Dem_01084 */
        if (FDC < 0)
        {
            /* PRQA S 4393++ */ /* MISRA Rule 10.8 */
            DemInternalData.CurrentFDC = (uint8)(0xFFu + (uint8)FDC + 1u);
            /* PRQA S 4393-- */ /* MISRA Rule 10.8 */
        }
        else
        {
            DemInternalData.CurrentFDC = (uint8)FDC;
        }
    }
#if (DEM_CYCLESSINCELASTFAILEDEANLE == STD_ON)
    /* SWS_Dem_00820 */
    DemInternalData.CyclesSinceLastFailed = pEvent->CyclesSinceLastFailed;
#endif /* DEM_CYCLESSINCELASTFAILEDEANLE == STD_ON */
    /* SWS_Dem_00821 */
#if (DEM_CYCLESSINCEFRISTFAILEDEANLE == STD_ON)
    DemInternalData.CyclesSinceFirstFailed = pEvent->CyclesSinceFirstFailed;
#endif /* DEM_CYCLESSINCEFRISTFAILEDEANLE == STD_ON */
    /* SWS_Dem_00822 */
    DemInternalData.FailedCycles = pEvent->FailureCounter;
#if (DEM_OCC6EANLE == STD_ON)
    DemInternalData.OCC6 = pEvent->OCC6;
#endif /* DEM_OCC6EANLE == STD_ON */
#if (DEM_OCC4EANLE == STD_ON)
    DemInternalData.OCC4 = pEvent->OCC4;
#endif /* DEM_OCC4EANLE == STD_ON */
#if (DEM_STATUSINDICATOR30ENABLE == STD_ON)
    DemInternalData.StatusIndicator30 = pEvent->StatusIndicator30;
#endif /* DEM_STATUSINDICATOR30ENABLE == STD_ON */
#if (DEM_FAILURE_PENGDING_SAIC == STD_ON)
    DemInternalData.FaultPendingCounter = DEM_GET_BIT(pEvent->UdsStatus, (uint8)DEM_UDS_STATUS_PDTC);
#endif /* DEM_FAILURE_PENGDING_SAIC == STD_ON */
#if (DEM_AGEDCOUNT_SAIC == STD_ON)
    DemInternalData.AgedCounter = pEvent->AgedCounter;
#endif /* DEM_AGEDCOUNT_SAIC == STD_ON */
}

#if ((DEM_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_PID_CLASS_NUM > 0))
/*************************************************************************/
/*
 * Brief               Dem_DcmGetFreezeFrameDataByDTCProcess
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
/* PRQA S 1505,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(void, DEM_CODE)
Dem_DcmGetFreezeFrameDataByDTCProcess(
    P2CONST(uint8, AUTOMATIC, DEM_APPL_DATA) pFFData,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize,
    Dem_EventIdType IntId,
    boolean isGlobalFF)
/* PRQA S 1505,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    uint16 didRefNum;
    uint8 csIndex;
    uint8 dataSize;
    const Dem_DidClassType* pDid;
    const Dem_DTCAttributesType* pDTCAttr = Dem_EventDTCAttributesCfg(IntId);
    uint16 offset = 0;
    uint16 cnt;
    uint16 FFClassRef;
    const Dem_FreezeFrameClassType* pFFClass;
    uint8 RefNum;
    const uint16* pDidIndex;

    if (pDTCAttr != NULL_PTR)
    {
#if (DEM_GLOBAL_FREEZE_FRAME_SUPPORT == STD_ON)
        if (isGlobalFF == TRUE)
        {
            FFClassRef = DemGlobalFreezeFrame.DemFreezeFrameClassRef;
        }
        else
#endif
        {
            FFClassRef = pDTCAttr->DemFreezeFrameClassRef;
        }
        pFFClass = &DemFreezeFrameClass[FFClassRef];
        RefNum = pFFClass->RefNum;
        pDidIndex = &DemDidClassRef[pFFClass->StartIndex];

        DestBuffer[offset] = RefNum;
        offset++;
        (*BufSize) += 1u;
        cnt = 0;
        for (didRefNum = 0u; didRefNum < RefNum; didRefNum++)
        {
            pDid = &DemDidClass[*pDidIndex];
            dataSize = 0u;
            /* DemIdentifier */
            DestBuffer[offset] = (uint8)((pDid->DemDidIdentifier) >> 8u);
            offset++;
            DestBuffer[offset] = (uint8)(pDid->DemDidIdentifier);
            offset++;
#if (DEM_DATA_ELEMENT_CLASS_NUM > 0u)
            Dem_UpdateInternalData(IntId);
            for (csIndex = 0; csIndex < pDid->RefNum; csIndex++)
            {
                const DemDataElementClassTypes* pDemDataElementClass =
                    &DemPbCfgPtr->DemDataElementClass[pDid->StartIndex + csIndex];

                if (pDemDataElementClass->DemDataElementReadFnc != NULL_PTR)
                {
                    uint8 ElementDataSize = pDemDataElementClass->DemDataElementDataSize;
                    if (FALSE == pDemDataElementClass->DemInterFlag)
                    {
                        Dem_MemCopy(&DestBuffer[offset], &pFFData[cnt], ElementDataSize);
                    }
                    else
                    {
                        /* get inter data now */
                        pDemDataElementClass->DemDataElementReadFnc(&DestBuffer[offset]);
                    }
                    offset += ElementDataSize;
                    dataSize += ElementDataSize;
                    cnt += ElementDataSize;
                }
            }
#endif
            (*BufSize) += (2u + (uint16)dataSize);
            pDidIndex++;
        }
#if (DEM_GLOBAL_FREEZE_FRAME_SUPPORT != STD_ON)
        DEM_UNUSED(isGlobalFF);
#endif /* DEM_GLOBAL_FREEZE_FRAME_SUPPORT != STD_ON */
    }
}
#endif /* DEM_FREEZE_FRAME_CLASS_NUM > 0 || DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0 || \
          DEM_PID_CLASS_NUM > 0 */
#if (DEM_EXTENDED_DATA_CLASS_NUM > 0)
/*************************************************************************/
/*
 * Brief               Dem_SetDTCSettingProcess
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant/Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
/* PRQA S 1505,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(void, DEM_CODE)
Dem_DcmGetSizeOfExtDataRecLoop(
    uint16 dtcRef,
    uint8 memDest,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) SizeOfExtendedDataRecord,
    P2CONST(Dem_ExtendedDataRecordClassType, AUTOMATIC, DEM_CONST) pExtDataCfg)
/* PRQA S 1505,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    const Dem_DTCType* pDemDTC = &DemPbCfgPtr->DemDTC[dtcRef];
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
    const Dem_EventIdType EventRefNum = pDemDTC->EventRefNum;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
    const Dem_EventIdType EventRefStart = pDemDTC->EventRefStart;
    const Dem_EventIdType* pDTCMapping = &DemPbCfgPtr->DemDTCMapping[EventRefStart];

#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
    for (uint16 iloop = 0u; iloop < EventRefNum; iloop++)
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
    {
        const Dem_EventMemEntryType* pEntry = Dem_MemEntryGet(Dem_GetEventExternalId(*pDTCMapping), memDest);
        if (pEntry != NULL_PTR)
        {
            *SizeOfExtendedDataRecord += pExtDataCfg->DataSize;
        }
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
        pDTCMapping++;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
    }
}
#endif
#if ((DEM_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_PID_CLASS_NUM > 0))
/*************************************************************************/
/*
 * Brief               Dem_FreezeFrameStorage
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId && pEventBuffer && Trigger
 * Param-Name[out]     None
 * Param-Name[in/out]  pEntry
 * Return              None
 */
/*************************************************************************/
/* PRQA S 1505,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(void, DEM_CODE)
Dem_FreezeFrameStorage(
    Dem_EventIdType IntId,
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA) pEntry,
    uint8 Trigger)
/* PRQA S 1505,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    Dem_FreezeFrameInfoType* pFF;
    const Dem_DTCAttributesType* pDTCAttrCfg = Dem_EventDTCAttributesCfg(IntId);
#if (DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)
#if (DEM_FREEZE_FRAME_RECORD_CLASS_NUM > 0u)
    const Dem_FreezeFrameRecordClassType* pFFRecordCfg;
    uint8 pFFRecordIndex;
#endif /* DEM_FREEZE_FRAME_RECORD_CLASS_NUM > 0u */
    uint8 iloop;
#endif /* DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED */
#if (                                                                      \
    (DEM_PID_CLASS_NUM > 0) && (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT) \
    && (DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED))
    uint16 obdDtcRef;
#endif /* DEM_PID_CLASS_NUM > 0 && DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT && \
    DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED */
#if (DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)
    uint8 FFRecNumClassRef = pDTCAttrCfg->DemFreezeFrameRecNumClassRef;
#if (DEM_GLOBAL_FREEZE_FRAME_SUPPORT == STD_ON)
    Dem_EventRelateInformationType* pEvent = Dem_GetEventInfo(IntId);
    Dem_GlobalFreezeFrameInfoType* pGFF;
    pGFF = &(pEntry->GlobalFFList);
    if ((DemGlobalFreezeFrame.DemFreezeFrameRecordTrigger == Trigger)
        && ((DEM_EVENT_STATUS_FDC_TRIGGERED != Trigger)
            || (DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_FDC_TRIGGERED) == 0x00u))
        && (pGFF->RecordNum == 0xFFu))
    {
        uint16 tempRef = DemGlobalFreezeFrame.DemFreezeFrameClassRef;
        pGFF->RecordNum = DemGlobalFreezeFrame.DemFreezeFrameRecordNumber;
        /* FreezeFrameRef */
        Dem_GetGeneralFreezeFrame(tempRef, pGFF->Data);
    }
#endif

    if ((pDTCAttrCfg != NULL_PTR) && (FFRecNumClassRef != DEM_FREEZE_FRAME_REC_NUM_INVALID))
    {
#if (DEM_FREEZE_FRAME_RECORD_CLASS_NUM > 0u)
        const Dem_FreezeFrameRecNumClassType* pDemFFRecNumClass = &DemFreezeFrameRecNumClass[FFRecNumClassRef];
        uint8 RefNum = pDemFFRecNumClass->RefNum;
        for (iloop = 0; iloop < RefNum; iloop++)
        {
            pFFRecordIndex = DemFreezeFrameRecordClassRef[pDemFFRecNumClass->StartIndex + iloop];
            if (pFFRecordIndex != DEM_FREEZE_FRAME_RECORD_INVALID)
            {
                /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
                const Dem_EventRelateInformationType* pEvent = Dem_GetEventInfo(IntId);
                /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
                pFFRecordCfg = &DemFreezeFrameRecordClass[pFFRecordIndex];
                if ((pFFRecordCfg->DemFreezeFrameRecordTrigger == Trigger)
                    && ((DEM_EVENT_STATUS_FDC_TRIGGERED != Trigger)
                        || (DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_FDC_TRIGGERED) == 0x00u)))
                {
#endif /* DEM_FREEZE_FRAME_RECORD_CLASS_NUM > 0u */
                    /* req SWS_Dem_00797 */
                    pFF = &(pEntry->FFList[iloop]);
                    if (pFF->RecordNum == 0xFFu)
                    {
                        pEntry->FFNum++;
                    }
#if (DEM_FREEZE_FRAME_RECORD_CLASS_NUM > 0u)
                    /* req SWS_Dem_00804  req SWS_Dem_00803 */
                    if (((pFF->RecordNum == 0xFFu)
                         && (pFFRecordCfg->DemFreezeFrameRecordUpdate == DEM_UPDATE_RECORD_NO))
                        || (pFFRecordCfg->DemFreezeFrameRecordUpdate == DEM_UPDATE_RECORD_YES))
                    {
#endif /* DEM_FREEZE_FRAME_RECORD_CLASS_NUM > 0u */
#if ((DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT) && (DEM_PID_CLASS_NUM > 0))
                        obdDtcRef = DemPbCfgPtr->DemDTC[DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef].DemObdDTCRef;
                        if (((obdDtcRef != DEM_OBD_DTC_INVALID)
                             && ((DemPbCfgPtr->DemObdDTC[obdDtcRef].DemDtcValue != DEM_OBD_DTC_CFG_INVALID)))
                            && (pDTCAttrCfg->DemFreezeFrameClassRef == DEM_FREEZE_FRAME_INVALID)
                            && (pDTCAttrCfg->DemWWHOBDFreezeFrameClassRef == DEM_WWHOBD_FREEZE_FRAME_INVALID)
                            && (pDTCAttrCfg->DemJ1939FreezeFrameClassRef == DEM_J1939_FREEZE_FRAME_INVALID)
                            && (pDTCAttrCfg->DemJ1939ExpandedFreezeFrameClassRef == DEM_J1939_FREEZE_FRAME_INVALID))
                        {
                            Dem_FreezeFrameCopy(IntId, pFF->Data);
                            pFF->RecordNum = 0;
                        }
                        else
#endif /* DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT && DEM_PID_CLASS_NUM > 0 */
                        {
#if (DEM_FREEZE_FRAME_RECORD_CLASS_NUM > 0u)
                            /*RecordNum the initialized value should be 0xff  req SWS_Dem_00581 */
                            pFF->RecordNum = pFFRecordCfg->DemFreezeFrameRecordNumber;
#endif /* DEM_FREEZE_FRAME_RECORD_CLASS_NUM > 0 */
                            Dem_FreezeFrameCopy(IntId, pFF->Data);
                            pEntry->LatestFF = iloop;
                        }
#if (DEM_FREEZE_FRAME_RECORD_CLASS_NUM > 0u)
                    }
                }
            }
        }
#endif /* DEM_FREEZE_FRAME_RECORD_CLASS_NUM > 0 */
    }

#else  /* DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED */
    if ((pDTCAttrCfg != NULL_PTR) && (pDTCAttrCfg->DemMaxNumberFreezeFrameRecords != 0u))
    {
        if (pDTCAttrCfg->DemMaxNumberFreezeFrameRecords == 1u)
        {
            pEntry->FFNum = 1u;
            pFF = &pEntry->FFList[0u];
            pEntry->LatestFF = pEntry->FFNum;
            pFF->RecordNum = pEntry->LatestFF;
            Dem_FreezeFrameCopy(IntId, pFF->Data);
        }
        else
        {
            if (pEntry->FFNum < pDTCAttrCfg->DemMaxNumberFreezeFrameRecords)
            {
                /* req SWS_Dem_00585 */
                pFF = &pEntry->FFList[pEntry->FFNum];
                pEntry->LatestFF = pEntry->FFNum;
                pEntry->FFNum++;
                pFF->RecordNum = pEntry->FFNum;
                Dem_FreezeFrameCopy(IntId, pFF->Data);
            }
        }
    }
#endif /* DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED */
}

/*************************************************************************/
/*
 * Brief               Dem_FreezeFrameGetByRecordNum
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      pEntry && RecordNum
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Dem_FreezeFrameInfoType*
 */
/*************************************************************************/
/* PRQA S 1505,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(P2VAR(Dem_FreezeFrameInfoType, AUTOMATIC, DEM_APPL_DATA), DEM_CODE)
Dem_FreezeFrameGetByRecordNum(P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA) pEntry, uint8 RecordNum)
/* PRQA S 1505,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    Dem_FreezeFrameInfoType* ret = NULL_PTR;
    Dem_FreezeFrameInfoType* pFF = pEntry->FFList;

    if (pEntry->FFNum != 0u)
    {
        if (RecordNum == 0xFFu)
        {
            ret = &(pFF[pEntry->LatestFF]);
        }
        else
        {
#if (DEM_MAX_NUMBER_FF_RECORDS > 1u)
            for (uint8 iloop = 0; iloop < DEM_MAX_NUMBER_FF_RECORDS; iloop++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
            {
#endif /* DEM_MAX_NUMBER_FF_RECORDS > 1u */
                if (pFF->RecordNum == RecordNum)
                {
                    ret = pFF;
                }
#if (DEM_MAX_NUMBER_FF_RECORDS > 1u)
                pFF++; /* PRQA S 2983 */ /* MISRA Rule 2.2 */
            }
#endif /* DEM_MAX_NUMBER_FF_RECORDS > 1u */
        }
    }
    return ret;
}
#endif /* DEM_FREEZE_FRAME_CLASS_NUM > 0 || DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0 || \
          DEM_PID_CLASS_NUM > 0 */

/*************************************************************************/
/*
 * Brief               Dem_EventQueueProcessWithDebounce
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(void, DEM_CODE) Dem_EventQueueProcessWithDebounce(Dem_EventIdType IntId)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    const Dem_EventParameterType* pEventCfg = &DemPbCfgPtr->DemEventParameter[IntId];
    uint16 AlgorithmIndex = pEventCfg->AlgorithmIndex;
    /*debounce counter shall be reset to zero*/
    switch (pEventCfg->AlgorithmType)
    {
#if (DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
    case DEM_DEBOUNCE_COUNTER_BASED:
        if (AlgorithmIndex < DEM_DEBOUNCE_COUNTER_BASED_EVENT_NUM)
        {
            DemDebounceCounterInfo[AlgorithmIndex].InternalDebounceCounter = 0;
        }
        break;
#endif
#if (DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
    case DEM_DEBOUNCE_TIME_BASE:
        if (AlgorithmIndex < DEM_DEBOUNCE_TIME_BASED_EVENT_NUM)
        {
            DemDebounceTimerInfo[AlgorithmIndex].Timeout = 0u;
            DemDebounceTimerInfo[AlgorithmIndex].Triggered = FALSE;
            DemDebounceTimerInfo[AlgorithmIndex].CurStatus = DEM_EVENT_STATUS_PREPASSED;
        }
        break;
#endif
    default:
        /*idle*/
        break;
    }
}

/*************************************************************************/
/*
 * Brief               Dem_EventTestFailedProcessOfComponent
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(void, DEM_CODE)
Dem_EventTestFailedProcessOfComponent(
#if (DEM_COMPONENT_NUM > 0)
    P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_APPL_DATA) pEvent,
#else
    P2CONST(Dem_EventRelateInformationType, AUTOMATIC, DEM_APPL_DATA) pEvent,
#endif
    /* PRQA S 3673 */ /* MISRA Rule 8.13 */
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
#if (DEM_COMPONENT_NUM > 0)
    uint16 ComponentID;
    uint16 iloop;
    const uint16* pComponentIndex;

    ComponentID = pEventCfg->DemComponentClassRef;
    /*check whether event ref the component, check the component availability*/
    if ((ComponentID != DEM_COMPONENT_INVALID) && (DemComponentStatus[ComponentID].availability == TRUE))
    {
        uint16 ExtComponentID = Dem_GetEventExternalId(ComponentID);
        const DemComponentTypes* pComponent = DemPbCfgPtr->DemComponent;
        const DemComponentTypes* OldpComponent = &pComponent[ComponentID];
        DemComponentStatusType* pComponentStatus;
        if (OldpComponent->DemParentComponentRef != DEM_COMPONENT_INVALID)
        {
            uint16 NewComponentID = ComponentID;
            for (; NewComponentID != DEM_COMPONENT_INVALID;)
            {
                const DemComponentTypes* NewpComponent = &pComponent[NewComponentID];
                pComponentStatus = &DemComponentStatus[NewComponentID];
                /*SWS_Dem_01126 SWS_Dem_01127 SWS_Dem_01128*/
                if (((pComponentStatus->Status == TRUE)
                     && (pComponentStatus->FailurePriority < pEventCfg->DemComponentPriority)
                     && (NewpComponent->DemComponentIgnoresPriority == FALSE))
                    || ((NewpComponent->DemParentComponentRef != DEM_COMPONENT_INVALID)
                        && (DemComponentStatus[NewpComponent->DemParentComponentRef].Status == TRUE)))
                {
                    DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_CONSECUTIVE_FAULT);
                }
                /* next parent component*/
                /* PRQA S 2469++ */ /* MISRA Rule 14.2 */
                NewComponentID = pComponent[NewComponentID].DemParentComponentRef;
                /* PRQA S 2469-- */ /* MISRA Rule 14.2 */
            }
            pComponentStatus = &DemComponentStatus[ComponentID];
            if (pComponentStatus->FailurePriority > pEventCfg->DemComponentPriority)
            {
                pComponentStatus->FailurePriority = pEventCfg->DemComponentPriority;
            }
            if (pComponentStatus->Status == FALSE)
            {
                DemComponentFailedCallbackFnc ComponentFailedFnc = OldpComponent->DemComponentFailedCallbackFnc;
                DemComponentStatus[ComponentID].Status = TRUE;
                /* req SWS_Dem_01133 */
                if (ComponentFailedFnc != NULL_PTR)
                {
                    (*ComponentFailedFnc)(ExtComponentID, TRUE);
                }
#if (DEM_TRIGGER_FIM_REPORTS == STD_ON)
                /* req SWS_Dem_01132 */
                (void)FiM_DemTriggerOnComponentStatus(ExtComponentID, TRUE);
#endif
            }
        }
        else
        {
            pComponentStatus = &DemComponentStatus[ComponentID];
            /* req SWS_Dem_01126 req SWS_Dem_01128 */
            if ((pComponentStatus->Status == TRUE)
                && (pComponentStatus->FailurePriority < pEventCfg->DemComponentPriority)
                && (OldpComponent->DemComponentIgnoresPriority == FALSE))
            {
                DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_CONSECUTIVE_FAULT);
                pComponentStatus->Status = TRUE;
            }
            else
            {
                if (pComponentStatus->Status == FALSE)
                {
                    pComponentStatus->Status = TRUE;
                    DemComponentFailedCallbackFnc ComponentFailedFnc = OldpComponent->DemComponentFailedCallbackFnc;
                    /* req SWS_Dem_01133 */
                    if (ComponentFailedFnc != NULL_PTR)
                    {
                        (*ComponentFailedFnc)(ExtComponentID, TRUE);
                    }
#if (DEM_TRIGGER_FIM_REPORTS == STD_ON)
                    /* req SWS_Dem_01132 */
                    (void)FiM_DemTriggerOnComponentStatus(ExtComponentID, TRUE);
#endif
                }
            }
            if (pComponentStatus->FailurePriority > pEventCfg->DemComponentPriority)
            {
                pComponentStatus->FailurePriority = pEventCfg->DemComponentPriority;
            }
        }
/* req SWS_Dem_01124 */
#if (DEM_IMMEDITE_CHILD_COMPONENT_MAX_NUM > 0)
        pComponentIndex = OldpComponent->DemImmediateChildComponentRef;
        for (iloop = 0;
             /* PRQA S 2877++ */ /* MISRA Dir 4.1 */
             (iloop < DEM_IMMEDITE_CHILD_COMPONENT_MAX_NUM) && (pComponentIndex[iloop] != DEM_COMPONENT_INVALID);
             /* PRQA S 2877-- */ /* MISRA Dir 4.1 */

             iloop++)
        {
            DemComponentStatus[pComponentIndex[iloop]].Status = TRUE;
        }
#endif /* DEM_IMMEDITE_CHILD_COMPONENT_MAX_NUM > 0 */
    }
#else
    DEM_UNUSED(pEvent);
    DEM_UNUSED(pEventCfg);
#endif
}

/*************************************************************************/
/*
 * Brief               Dem_CheckWIROn
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      pEvent && pEventCfg
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/*************************************************************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(boolean, DEM_CODE)
Dem_CheckWIROn(
    P2CONST(Dem_EventRelateInformationType, AUTOMATIC, DEM_APPL_DATA) pEvent,
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    boolean ret = FALSE;
#if ((DEM_INDICATOR_NUM > 0) && (DEM_INDICATOR_ATTRIBUTE_TOTAL_NUM > 0))
    uint8 iloop;
    const Dem_IndicatorAttributeType* pWIRAttr;
#if (DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)
    const Dem_DTCType* pDTCCfg = DemPbCfgPtr->DemDTC;
    const DemObdDTCType* pObdDTC = DemPbCfgPtr->DemObdDTC;
#endif /* DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU */
    for (iloop = 0; iloop < pEventCfg->AttrNum; iloop++)
    {
        uint16 Ref = DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemOBDGroupingAssociativeEventsRef;
        pWIRAttr = &DemIndicatorAttribute[pEventCfg->AttrStartIndex + iloop];
        uint8 FailureThreshold = pWIRAttr->DemIndicatorFailureCycleCounterThreshold;
        if ((FailureThreshold <= pEvent->FailureCounter)
            || ((Ref != DEM_EVENT_PARAMETER_INVALID)
                && (FailureThreshold <= DemEventRelateInformation[Ref].FailureCounter)))
        {
#if (DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)
            uint16 DemDTCRef = DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef;
            uint16 DemObdDTCRef = pDTCCfg[DemDTCRef].DemObdDTCRef;
#endif
            /*reach the threshold activate the mil req SWS_Dem_00967 req SWS_Dem_00501*/
            ret = TRUE;
            DemWIRStatus[pWIRAttr->DemIndicatorRef] = pWIRAttr->DemIndicatorBehaviour;
#if (DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)
            if ((((DemDTCRef != DEM_DTC_REF_INVALID) && (pDTCCfg[DemDTCRef].DemDtcValue < 0xFFFF33UL)
                  && (pDTCCfg[DemDTCRef].DemDtcValue > 0UL))
                 || ((DEM_OBD_DTC_INVALID != DemObdDTCRef)
                     && ((pObdDTC[DemObdDTCRef].DemDtcValue != DEM_OBD_DTC_CFG_INVALID)
                         || (pObdDTC[DemObdDTCRef].DemJ1939DTCValue != DEM_J1939_DTC_CFG_INVALID)))))
            {
                /* req SWS_Dem_01139  req SWS_Dem_00703 */
                Dem_UpdateOBDMilStatus(pWIRAttr->DemIndicatorRef, pEvent->IntId);
            }
#endif
        }
    }
#else
    DEM_UNUSED(pEvent);
    DEM_UNUSED(pEventCfg);
#endif
    return ret;
}

/*************************************************************************/
/*
 * Brief               Dem_CheckWIROff
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      pEvent && pEventCfg
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/*************************************************************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(boolean, DEM_CODE)
Dem_CheckWIROff(
    P2CONST(Dem_EventRelateInformationType, AUTOMATIC, DEM_APPL_DATA) pEvent,
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    boolean ret = FALSE;
#if ((DEM_INDICATOR_NUM > 0) && (DEM_INDICATOR_ATTRIBUTE_TOTAL_NUM > 0))
    uint8 iloop;
    const Dem_IndicatorAttributeType* pWIRAttr;
    uint16 index;
    uint8 cnt;
    boolean Find = FALSE;
#if (DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)
    uint16 DemDTCRef;
    uint16 DemObdDTCRef;
#endif

    for (iloop = 0; iloop < pEventCfg->AttrNum; iloop++)
    {
        pWIRAttr = &DemIndicatorAttribute[pEventCfg->AttrStartIndex + iloop];
        /* req SWS_Dem_00503 */
        if (pWIRAttr->DemIndicatorHealingCycleCounterThreshold <= pEvent->HealingCounter)
        {
            for (index = 0; ((index < DEM_EVENT_PARAMETER_NUM) && (Find == FALSE)); index++)
            {
                for (cnt = 0; ((cnt < DemPbCfgPtr->DemEventParameter[index].AttrNum) && (Find == FALSE)); cnt++)
                {
                    if ((pWIRAttr->DemIndicatorRef
                         == DemIndicatorAttribute[DemPbCfgPtr->DemEventParameter[index].AttrStartIndex + cnt]
                                .DemIndicatorRef)
                        && (DemEventRelateInformation[index].HealingCounter
                            < DemIndicatorAttribute[DemPbCfgPtr->DemEventParameter[index].AttrStartIndex + cnt]
                                  .DemIndicatorHealingCycleCounterThreshold)
                        && (0x00u != DEM_FLAGS_ISSET(DemEventRelateInformation[index].UdsStatus, DEM_UDS_STATUS_WIR)))
                    {
                        Find = TRUE;
                    }
                }
            }
            if (Find == FALSE)
            {
                DemWIRStatus[pWIRAttr->DemIndicatorRef] = DEM_INDICATOR_OFF;
                ret = TRUE;
            }
#if (DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)
            DemDTCRef = DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef;
            DemObdDTCRef = DemPbCfgPtr->DemDTC[DemDTCRef].DemObdDTCRef;
            if ((((DemDTCRef != DEM_DTC_REF_INVALID) && (DemPbCfgPtr->DemDTC[DemDTCRef].DemDtcValue < 0xFFFF33UL)
                  && (DemPbCfgPtr->DemDTC[DemDTCRef].DemDtcValue > 0UL))
                 || ((DEM_OBD_DTC_INVALID != DemObdDTCRef)
                     && ((DemPbCfgPtr->DemObdDTC[DemObdDTCRef].DemDtcValue != DEM_OBD_DTC_CFG_INVALID)
                         || (DemPbCfgPtr->DemObdDTC[DemObdDTCRef].DemJ1939DTCValue != DEM_J1939_DTC_CFG_INVALID)))))
            {
                /* req SWS_Dem_01139  req SWS_Dem_00703 */
                Dem_UpdateOBDMilStatus(pWIRAttr->DemIndicatorRef, pEvent->IntId);
            }
#endif
        }
    }
#else
    DEM_UNUSED(pEvent);
    DEM_UNUSED(pEventCfg);
#endif /*DEM_INDICATOR_NUM > 0) && (DEM_INDICATOR_ATTRIBUTE_TOTAL_NUM > 0 */
    return ret;
}

/*************************************************************************/
/*
 * Brief               Dem_ResponseOnDtcStatusChange
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
FUNC(void, DEM_CODE) Dem_ResponseOnDtcStatusChange(Dem_EventIdType IntId)
{
    uint16 dtcRef = DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef;
#if (DEM_J1939_NODE_NUM > 0)
    uint8 iloop;
    uint8 Nodeindex;
    uint8 NodeNum;
    uint16 ObdDTCRef;
    uint8 NodeRef;
#endif
    Dem_EventRelateInformationType* pEvent = Dem_GetEventInfo(IntId);

    if ((dtcRef != DEM_DTC_REF_INVALID) && (pEvent->OldUdsStatus != pEvent->UdsStatus))
    {
        /* req SWS_Dem_01050 */
        Dem_TriggerOnEventStatus(IntId, pEvent->OldUdsStatus, pEvent->UdsStatus);
#if (DEM_J1939_NODE_NUM > 0)
        const DemObdDTCType* pDemObdDTC = DemPbCfgPtr->DemObdDTC;
        ObdDTCRef = DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef;
        if ((ObdDTCRef != DEM_OBD_DTC_INVALID) && (DEM_J1939_DTC_CFG_INVALID != pDemObdDTC[ObdDTCRef].DemJ1939DTCValue)
            && ((pEvent->OldUdsStatus & DEM_UDS_STATUS_TF)
                != (DemDTCGeneralStatus[dtcRef].CbUdsStatus & DEM_UDS_STATUS_TF)))
        {
            NodeRef =
                DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].DemDTCAttributesRef].DemJ1939DTC_J1939NodeRef;
            NodeNum = DemPbCfgPtr->J1939Node[NodeRef].NodeNum;
            for (iloop = 0; iloop < NodeNum; iloop++)
            {
                Nodeindex = DemPbCfgPtr->J1939Node[NodeRef].NodeIDRef[iloop];
                /* req SWS_Dem_00971 */
                J1939Dcm_DemTriggerOnDTCStatus(pDemObdDTC[ObdDTCRef].DemJ1939DTCValue, Nodeindex);
            }
        }
#endif
        pEvent->OldUdsStatus = pEvent->UdsStatus;
    }
}

/*************************************************************************/
/*
 * Brief               Dem_InterEventTestPassed
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId && pEventBuffer
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(void, DEM_CODE)
Dem_InterEventTestPassed(Dem_EventIdType IntId)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    Dem_EventRelateInformationType* pEvent = &DemEventRelateInformation[IntId];
    const Dem_EventParameterType* pEventCfg = &(DemPbCfgPtr->DemEventParameter[IntId]);

#if (DEM_COMPONENT_NUM > 0)
    uint16 ComponentID = pEventCfg->DemComponentClassRef;
    boolean allParentComponentpass = TRUE;

    /*check whether event ref the component , check the component availability*/
    if ((ComponentID != DEM_COMPONENT_INVALID) && (DemComponentStatus[ComponentID].availability == TRUE))
    {
        DEM_FLAGS_CLR(pEvent->Status, DEM_EVENT_STATUS_CONSECUTIVE_FAULT);
        const DemComponentTypes* pComponent = DemPbCfgPtr->DemComponent;
        if (pComponent[ComponentID].DemParentComponentRef != DEM_COMPONENT_INVALID)
        {
            uint16 LComponentID = ComponentID;
            uint16 DemParentComponentRef = pComponent[LComponentID].DemParentComponentRef;
            while ((DemParentComponentRef != DEM_COMPONENT_INVALID) && (allParentComponentpass == TRUE))
            {
                if (DemComponentStatus[DemParentComponentRef].Status == TRUE)
                {
                    allParentComponentpass = FALSE;
                }
                LComponentID = DemParentComponentRef; /* next parent component*/
                DemParentComponentRef = pComponent[LComponentID].DemParentComponentRef;
            }
        }
        if (allParentComponentpass == TRUE)
        {
            ComponentID = pEventCfg->DemComponentClassRef;
            /* req SWS_Dem_01125 */
            DemComponentStatus[ComponentID].Status = FALSE;
            DemComponentStatus[ComponentID].FailurePriority = 0xFF;
#if (STD_ON == DEM_TRIGGER_FIM_REPORTS)
            (void)FiM_DemTriggerOnComponentStatus(ComponentID + 1u, FALSE);
#endif
        }
    }
#endif
#if ((DEM_INDICATOR_NUM > 0) && (DEM_INDICATOR_ATTRIBUTE_TOTAL_NUM > 0))
    /* R19-11: req SWS_Dem_01233 */
    if ((0x00u == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TFTOC))
        && (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_WIR)))
    {
        const Dem_IndicatorAttributeType* pWIRAttr;
        boolean allThreshold = FALSE;
        uint8 AttrNum = pEventCfg->AttrNum;
        uint16 AttrStartIndex = pEventCfg->AttrStartIndex;
        for (uint8 iloop = 0; iloop < AttrNum; iloop++)
        {
            pWIRAttr = &DemIndicatorAttribute[AttrStartIndex + iloop];
            if (0x00u != pWIRAttr->DemIndicatorHealingCycleCounterThreshold)
            {
                allThreshold = TRUE;
                break;
            }
        }
        if (FALSE == allThreshold) /* R19-11: SWS_Dem_01233*/
        {
            DEM_FLAGS_CLR(pEvent->UdsStatus, DEM_UDS_STATUS_WIR);
            for (uint8 iloop = 0; iloop < AttrNum; iloop++)
            {
                pWIRAttr = &DemIndicatorAttribute[AttrStartIndex + iloop];
                DemWIRStatus[pWIRAttr->DemIndicatorRef] = DEM_INDICATOR_OFF;
            }
        }
    }
#endif

    if (DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_TFBITCHANGE) != 0x00u)
    {
#if (DEM_TRIGGERONPASSEDEANLE == STD_ON)
#if (DEM_EXTENDED_DATA_CLASS_NUM > 0)
        /*req SWS_Dem_01070 */
        Dem_EventDataStorageTrigger(IntId, DEM_TRIGGER_ON_PASSED);
#endif /* DEM_EXTENDED_DATA_CLASS_NUM > 0u */
#endif /* DEM_TRIGGERONPASSEDEANLE == STD_ON */
        DEM_FLAGS_CLR(pEvent->Status, DEM_EVENT_STATUS_TFBITCHANGE);
    }

#if (                                                                                                  \
    ((DEM_INDICATOR_NUM == 0) || (DEM_INDICATOR_ATTRIBUTE_TOTAL_NUM == 0)) && (DEM_COMPONENT_NUM == 0) \
    && (DEM_EXTENDED_DATA_CLASS_NUM == 0))
    DEM_UNUSED(pEvent);
#endif
#if (((DEM_INDICATOR_NUM == 0) || (DEM_INDICATOR_ATTRIBUTE_TOTAL_NUM == 0)) && (DEM_COMPONENT_NUM == 0))
    DEM_UNUSED(pEventCfg);
#endif
}

#if (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_OFF)
void Dem_CDTCOverFlowCheck(Dem_EventIdType IntId)
{
    /* insert a entry that been displaced into entry memory as first entry, so its absolute time is minimum of all
     * entry. And update other absolute time of other entry by increment one  */
    Dem_EventRelateInformationType* pEvent = Dem_GetEventInfo(IntId);
    uint16 TempRef = DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef;
    if (TempRef != DEM_DTC_REF_INVALID)
    {
        const uint8* pMemDest =
            DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[TempRef].DemDTCAttributesRef].DemMemoryDestinationRef;
#if (DEM_MEM_DEST_MAX_NUM_OF_DTC > 1u)
        for (uint8 iloop = 0; iloop < DEM_MEM_DEST_MAX_NUM_OF_DTC; iloop++)
#else
        uint8 iloop = 0;
#endif /* DEM_MEM_DEST_MAX_NUM_OF_DTC > 1u */
        {
            uint8 MemDest = pMemDest[iloop];
            if ((DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_CDTC) != 0x00u)
                && (Dem_MemEntryGet(Dem_GetEventExternalId(IntId), MemDest) == NULL_PTR))
            {
                /*not stored in an event memory entry  UDS status bit 3 (ConfirmedDTC) set   gets qualified as passed*/
                Dem_EventMemEntryType* pEntry = Dem_MemEntryGet(0x00, MemDest);
                /*return NULL_PTR means no idle memory*/
                if (pEntry != NULL_PTR)
                {
                    /*SWS_Dem_00823] */
                    Dem_MemDestInfoType* pDemMemDestInfo = &DemMemDestInfo[MemDest];
                    uint8 EntryNum = DemMemDestCfg[MemDest].EntryNum;
                    const Dem_MemDestConfigType* pDemMemDestCfg = &DemMemDestCfg[MemDest];
                    if (pDemMemDestInfo->RecordNum < EntryNum)
                    {
                        pDemMemDestInfo->RecordNum++;
                    }
                    pEntry->EventId = Dem_GetEventExternalId(IntId);
                    for (uint8 jloop = 0; jloop < EntryNum; jloop++)
                    {
                        Dem_EventMemEntryType* pEntryList = &pDemMemDestCfg->EntryList[jloop];
                        /* Update absolute time of other entry */
                        pEntryList->AbsTime += 1;
                    }
                    pEntry->AbsTime = 1;
                }
            }
        }
    }
}
#endif /* DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_OFF */

/*************************************************************************/
/*
 * Brief               Dem_InterOperationCycleStart
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(void, DEM_CODE) Dem_InterOperationCycleStart(Dem_EventIdType IntId)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
#if ((DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0) || (DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0))
    uint8 AlgorithmType = DemPbCfgPtr->DemEventParameter[IntId].AlgorithmType;
    uint16 AlgorithmIndex = DemPbCfgPtr->DemEventParameter[IntId].AlgorithmIndex;
#endif
#if (DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
    if ((AlgorithmType == DEM_DEBOUNCE_COUNTER_BASED) && (AlgorithmIndex < DEM_DEBOUNCE_COUNTER_BASED_EVENT_NUM))
    {
        DemDebounceCounterInfo[AlgorithmIndex].InternalDebounceCounter = 0;
    }
#endif
#if (DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
    if ((AlgorithmType == DEM_DEBOUNCE_TIME_BASE) && (AlgorithmIndex < DEM_DEBOUNCE_TIME_BASED_EVENT_NUM))
    {
        DemDebounceTimerInfo[AlgorithmIndex].Timeout = 0;
        DemDebounceTimerInfo[AlgorithmIndex].Triggered = FALSE;
        DemDebounceTimerInfo[AlgorithmIndex].CurStatus = DEM_EVENT_STATUS_PREPASSED;
    }
#endif
}

/*************************************************************************/
/*
 * Brief               Dem_CallDataChangedOfMemEntryAllocate
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(void, DEM_CODE)
Dem_CallDataChangedOfMemEntryAllocate(Dem_EventIdType IntId)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
#if (DEM_EXTERAL_DATA_ELEMENT_CLASS_NUM > 0)
    /* req SWS_Dem_00475 */
    Dem_CallbackEventDataChangedType CallbackEventDataChanged =
        DemPbCfgPtr->DemEventParameter[IntId].DemCallbackEventDataChanged;
    if (CallbackEventDataChanged != NULL_PTR)
    {
        (void)CallbackEventDataChanged(Dem_GetEventExternalId(IntId));
    }
#else
    DEM_UNUSED(IntId);
#endif
}

/*************************************************************************/
/*
 * Brief               Dem_GetMemDestMaxNumOfDtc
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint8
 */
/*************************************************************************/
FUNC(uint8, DEM_CODE) Dem_GetMemDestMaxNumOfDtc(void)
{
    return DEM_MEM_DEST_MAX_NUM_OF_DTC;
}

/*************************************************************************/
/*
 * Brief               Dem_GetDemWIRStatus
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint8
 */
/*************************************************************************/
FUNC(Dem_IndicatorStatusType, DEM_CODE)
Dem_GetDemWIRStatus(uint8 DemIndicatorRef) /* PRQA S 1532 */ /* MISRA Rule 2.1 */
{
#if (DEM_INDICATOR_NUM > 0)
    return DemWIRStatus[DemIndicatorRef];
#else
    DEM_UNUSED(DemIndicatorRef);
    return DEM_INDICATOR_OFF;
#endif
}

/*************************************************************************/
/*
 * Brief               Dem_ClearEventRelateInformation
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      pEvent
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
void Dem_ClearEventRelateInformation(P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_VAR) pEvent)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    pEvent->FailureCounter = 0;
#if (DEM_OCC6EANLE == STD_ON)
    pEvent->OCC6 = 0;
#endif /* DEM_OCC6EANLE == STD_ON */
#if (DEM_OCC4EANLE == STD_ON)
    pEvent->OCC4 = 0;
#endif /* DEM_OCC4EANLE == STD_ON */
    pEvent->OccurrenceCounter = 0;
    pEvent->AgingCounter = 0;
#if (DEM_CYCLESSINCEFRISTFAILEDEANLE == STD_ON)
    pEvent->CyclesSinceFirstFailed = 0;
#endif /* DEM_CYCLESSINCEFRISTFAILEDEANLE == STD_ON */
#if (DEM_CYCLESSINCELASTFAILEDEANLE == STD_ON)
    pEvent->CyclesSinceLastFailed = 0;
#endif /* DEM_CYCLESSINCELASTFAILEDEANLE == STD_ON */
#if ((DEM_CYCLESSINCEFRISTFAILEDEANLE == STD_ON) || (DEM_CYCLESSINCELASTFAILEDEANLE == STD_ON))
    DEM_FLAGS_CLR(pEvent->EventStatusFlag, DEM_EVENT_FLAGS_CYCLECOUNTER);
#endif /*DEM_CYCLESSINCEFRISTFAILEDEANLE == STD_ON)|| (DEM_CYCLESSINCELASTFAILEDEANLE == STD_ON */
    pEvent->HealingCounter = 0;
    DEM_FLAGS_CLR(pEvent->EventStatusFlag, DEM_EVENT_FLAGS_FDC_TRIGGER);
}

/*************************************************************************/
/*
 * Brief               Dem_DeleteRelatedData
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint8
 */
/*************************************************************************/
FUNC(void, DEM_CODE)
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
Dem_DeleteRelatedData(P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA) pEntry)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    Dem_EventRelateInformationType* pEvent;
#if ((DEM_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_PID_CLASS_NUM > 0))
    pEntry->FFNum = 0x00;
    pEntry->LatestFF = 0x00;
    Dem_MemSet((uint8*)pEntry->FFList, 0xFFu, sizeof(Dem_FreezeFrameInfoType) * DEM_MAX_NUMBER_FF_RECORDS);
#if (DEM_GLOBAL_FREEZE_FRAME_SUPPORT == STD_ON)
    Dem_MemSet((uint8*)&pEntry->GlobalFFList, 0xFFu, sizeof(Dem_GlobalFreezeFrameInfoType));
#endif /* DEM_GLOBAL_FREEZE_FRAME_SUPPORT == STD_ON */
#endif
#if (DEM_EXTENDED_DATA_CLASS_NUM > 0)
#if (DEM_EXTENDED_DATA_MAX_LEN > 0u)
    Dem_MemSet(pEntry->ExtData, 0xFFu, DEM_EXTENDED_DATA_MAX_LEN);
#endif
    Dem_MemSet(pEntry->ExtStatus, 0x00u, DEM_EXTENDED_DATA_MAX_REF_NUM_BYTE);
#endif
    if ((pEntry->EventId != 0u) && (pEntry->EventId <= DEM_EVENT_PARAMETER_NUM))
    {
        pEvent = &DemEventRelateInformation[Dem_GetEventInternalId(pEntry->EventId)];
        Dem_ClearEventRelateInformation(pEvent);
    }
}

/*************************************************************************/
/*
 * Brief               Dem_EventDataStorageTrigger
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId && pEventBuffer && Trigger
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
FUNC(void, DEM_CODE)
Dem_EventDataStorageTrigger(Dem_EventIdType IntId, uint8 Trigger)
{
#if (                                                                                                        \
    (((DEM_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_PID_CLASS_NUM > 0)) \
     && (DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED))                            \
    || (DEM_EXTENDED_DATA_CLASS_NUM > 0))
    uint16 TempRef = DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef;
    Dem_EventMemEntryType* pEntry;

    if (TempRef != DEM_DTC_REF_INVALID)
    {
        Dem_Pending = TRUE;
        const Dem_DTCAttributesType* pDTCAttr = Dem_EventDTCAttributesCfg(IntId);
        if (pDTCAttr != NULL_PTR)
        {
            const uint8* pMemDest = pDTCAttr->DemMemoryDestinationRef;

#if (DEM_MEM_DEST_MAX_NUM_OF_DTC > 1u)
            for (uint8 iloop = 0; iloop < DEM_MEM_DEST_MAX_NUM_OF_DTC; iloop++)
#else
            uint8 iloop = 0;
#endif /* DEM_MEM_DEST_MAX_NUM_OF_DTC > 1u */
            {
                uint8 MemDest = pMemDest[iloop];
                /* PRQA S 3415++ */ /* MISRA Rule 13.5 */
                if ((MemDest != DEM_MEM_DEST_INVALID)
                    && (DEM_BITS_ISNSET(DemMemDestInfo[MemDest].DisableDTCRecordUpdate, TempRef)))
                /* PRQA S 3415-- */ /* MISRA Rule 13.5 */
                {
                    if (Trigger == DEM_EVENT_MEMORY_ENTRY_STORAGE_TRIGGER)
                    {
                        pEntry = Dem_MemEntryAllocate(IntId, MemDest);
                    }
                    else
                    {
                        pEntry = Dem_MemEntryGet(Dem_GetEventExternalId(IntId), MemDest);
                    }
                    /*whether get the entry to store*/
                    if (pEntry != NULL_PTR)
                    {
#if (                                                                                                       \
    ((DEM_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_PID_CLASS_NUM > 0)) \
    && (DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED))
                        Dem_FreezeFrameStorage(IntId, pEntry, Trigger);
#endif
#if (DEM_EXTENDED_DATA_CLASS_NUM > 0)
                        Dem_ExtendedDataStorage(IntId, pEntry, Trigger);
#endif
#if (DEM_NVRAM_BLOCKID_NUM > 0u)
                        if ((pDTCAttr->DemImmediateNvStorage == TRUE)
                            && (DemEventRelateInformation[IntId].OccurrenceCounter < DEM_IMMEDIATE_NV_STORAGE_LIMIT))
                        {
#if (DEM_NVRAM_DIVADED == STD_ON)
                            Dem_IntWriteOneNvRAM(IntId);
#else
                            Dem_IntWriteNvRAM();
#endif /* DEM_NVRAM_DIVADED == STD_ON */
                        }
#endif /* DEM_NVRAM_BLOCKID_NUM > 0u */
                    }
                }
            }
        }
        Dem_Pending = FALSE;
    }
#else
    DEM_UNUSED(IntId);
    DEM_UNUSED(Trigger);
#endif
}

/*************************************************************************/
/*
 * Brief               Dem_DebounceInit
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
FUNC(void, DEM_CODE) Dem_DebounceInit(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint16 iloop;

#if (DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
    for (iloop = 0; iloop < DEM_DEBOUNCE_COUNTER_BASED_EVENT_NUM; iloop++)
    {
        Dem_DebounceCounterInfoType* pDemDebounceCounterInfo = &DemDebounceCounterInfo[iloop];
        pDemDebounceCounterInfo->InternalDebounceCounter = 0;
    }
#endif
#if (DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
    for (iloop = 0; iloop < DEM_DEBOUNCE_TIME_BASED_EVENT_NUM; iloop++)
    {
        Dem_DebounceTimeInfoType* pDemDebounceTimerInfo = &DemDebounceTimerInfo[iloop];
        pDemDebounceTimerInfo->Timeout = 0x00;
        pDemDebounceTimerInfo->Triggered = FALSE;
        pDemDebounceTimerInfo->CurStatus = DEM_EVENT_STATUS_PREPASSED;
    }
#endif
    Dem_MemSet((uint8*)FDCInfo, 0x00u, (sizeof(FDCInfoType) * DEM_DTC_NUM));
}

/*************************************************************************/
/*
 * Brief               Dem_DebounceReset
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
FUNC(void, DEM_CODE) Dem_DebounceReset(Dem_EventIdType IntId) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    const Dem_EventParameterType* pEventCfg = &DemPbCfgPtr->DemEventParameter[IntId];
#if (DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
    Dem_DebounceCounterInfoType* pCounter;
#endif
#if (DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
    Dem_DebounceTimeInfoType* pTimer;
#endif
#if ((DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0u) || (DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0u))
    const uint16 AlgorithmIndex = pEventCfg->AlgorithmIndex;
#endif /*DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0u)||(DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0u */

    switch (pEventCfg->AlgorithmType)
    {
#if (DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
    case DEM_DEBOUNCE_COUNTER_BASED:
        if (AlgorithmIndex < DEM_DEBOUNCE_COUNTER_BASED_EVENT_NUM)
        {
            pCounter = &DemDebounceCounterInfo[AlgorithmIndex];
            pCounter->InternalDebounceCounter = 0;
        }
        break;
#endif
#if (DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
    case DEM_DEBOUNCE_TIME_BASE:
        if (AlgorithmIndex < DEM_DEBOUNCE_TIME_BASED_EVENT_NUM)
        {
            pTimer = &DemDebounceTimerInfo[AlgorithmIndex];
            pTimer->Timeout = 0;
            pTimer->Triggered = FALSE;
            pTimer->CurStatus = DEM_EVENT_STATUS_PREPASSED;
        }
        break;
#endif
    default:
        /*idle*/
        break;
    }
}

/*************************************************************************/
/*
 * Brief               Dem_GetInteralFDC
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              sint8
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
Dem_GetInteralFDC(Dem_EventIdType IntId, P2VAR(sint8, AUTOMATIC, DEM_APPL_DATA) FaultDetectionCounter)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    const Dem_EventParameterType* pEventCfg = &DemPbCfgPtr->DemEventParameter[IntId];
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
    const Dem_DebounceCounterInfoType* pCounter;
    const Dem_DebounceCounterBasedClassType* pCfgCounter;
#endif
#if (DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
    const Dem_DebounceTimeInfoType* pTimer;
    const Dem_DebounceTimeBaseClassType* pTimerCfg;
#endif
#if (DEM_DEBOUNCE_MONITOR_INTERNAL_EVENT_NUM > 0)
    Dem_GetFDCFncType GetFnc;
#endif
#if ((DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0) || (DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0))
    sint64 IFDC;
#endif /*DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0) || (DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0 */
    *FaultDetectionCounter = 0;
    switch (pEventCfg->AlgorithmType)
    {
#if (DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
    case DEM_DEBOUNCE_COUNTER_BASED:
        /* req SWS_Dem_00415 */
        if (((DemPbCfgPtr->DemEventParameter[IntId].AlgorithmRef) < DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM)
            && ((pEventCfg->AlgorithmIndex) < DEM_DEBOUNCE_COUNTER_BASED_EVENT_NUM))
        {
            pCfgCounter =
                &DemPbCfgPtr->DemDebounceCounterBasedClass[DemPbCfgPtr->DemEventParameter[IntId].AlgorithmRef];
            pCounter = &DemDebounceCounterInfo[pEventCfg->AlgorithmIndex];
            if (pCounter->InternalDebounceCounter > 0)
            {
                IFDC = (sint64)pCounter->InternalDebounceCounter * (sint64)127;
                *FaultDetectionCounter = (sint8)(IFDC / (sint32)pCfgCounter->DemDebounceCounterFailedThreshold);
            }
            else
            {
                IFDC = (sint64)pCounter->InternalDebounceCounter * ((sint64)(-128));
                *FaultDetectionCounter = (sint8)(IFDC / (sint64)pCfgCounter->DemDebounceCounterPassedThreshold);
            }
            ret = E_OK;
        }
        break;
#endif
#if (DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
    case DEM_DEBOUNCE_TIME_BASE: /*SWS_Dem_00427*/
        if (((DemPbCfgPtr->DemEventParameter[IntId].AlgorithmRef) < DEM_DEBOUNCE_TIME_BASE_CLASS_NUM)
            && ((pEventCfg->AlgorithmIndex) < DEM_DEBOUNCE_TIME_BASED_EVENT_NUM))
        {
            pTimer = &DemDebounceTimerInfo[pEventCfg->AlgorithmIndex];
            pTimerCfg = &DemPbCfgPtr->DemDebounceTimeBaseClass[DemPbCfgPtr->DemEventParameter[IntId].AlgorithmRef];
            switch (pTimer->CurStatus)
            {
            case DEM_EVENT_STATUS_FAILED:
            case DEM_EVENT_STATUS_PREFAILED:
                IFDC = (sint64)pTimer->Timeout * (sint64)127;
                *FaultDetectionCounter = (sint8)(IFDC / (sint64)pTimerCfg->DemDebounceTimeFailedThreshold);
                break;
            default:
                IFDC = (sint64)pTimer->Timeout * ((sint64)(-128));
                *FaultDetectionCounter = (sint8)(IFDC / (sint64)pTimerCfg->DemDebounceTimePassedThreshold);
                break;
            }
            ret = E_OK;
        }
        break;
#endif
#if (DEM_DEBOUNCE_MONITOR_INTERNAL_EVENT_NUM > 0)
    case DEM_DEBOUNCE_MONITOR_INTERNAL:
        /* req SWS_Dem_00264 */
        if ((DemPbCfgPtr->DemEventParameter[IntId].AlgorithmRef) < DEM_DEBOUNCE_MONITOR_INTERNAL_EVENT_NUM)
        {
            GetFnc = DemGetFDCFncs[DemPbCfgPtr->DemEventParameter[IntId].AlgorithmRef];
            *FaultDetectionCounter = 0;
            if (GetFnc != NULL_PTR)
            {
                /* req SWS_Dem_00439 */
                ret = GetFnc(FaultDetectionCounter);
                if (E_OK != ret)
                {
                    *FaultDetectionCounter = 0;
                }
            }
            else
            {
                ret = DEM_E_NO_FDC_AVAILABLE;
            }
        }
        break;
#endif
    default:
        *FaultDetectionCounter = 0;
        break;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Dem_DebounceProcess
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      pEventBuffer
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Dem_EventStatusType
 */
/*************************************************************************/
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(Dem_EventStatusType, DEM_CODE)
Dem_DebounceProcess(Dem_EventDataBufferType pEventBuffer)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    Dem_EventIdType IntId = Dem_GetEventInternalId(pEventBuffer.ExtId);
    const Dem_EventParameterType* pEventCfg = &DemPbCfgPtr->DemEventParameter[IntId];
    Dem_EventStatusType Status = pEventBuffer.Status;
    Dem_EventRelateInformationType* pEvent = Dem_GetEventInfo(IntId);
    uint8 AlgorithmType = pEventCfg->AlgorithmType;

    switch (AlgorithmType)
    {
#if (DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
    case DEM_DEBOUNCE_TIME_BASE:
        Status = Dem_DebounceProcessTimer(pEventBuffer, pEventCfg);
        break;
#endif
    default:
        /*idle,debounce counter deal in seteventstatus */
        break;
    }

#if (DEM_STATUSINDICATOR30ENABLE == STD_ON)
    if ((DEM_EVENT_STATUS_FAILED != Status) && (0x00u != DEM_FLAGS_ISSET(pEvent->StatusIndicator30, DEM_SI30_DTC_AGED)))
    {
        /*bit 6: A DTC is not emission related as long as the FDC10 has not reached the value
         * +127 since DTC information was last cleared and when it is aged*/
        uint16 DemDTCRef = pEventCfg->DemDTCRef;
        const Dem_DTCType* pDemDTC = &DemPbCfgPtr->DemDTC[DemDTCRef];
        if ((pDemDTC->DTCKind != DEM_DTC_KIND_EMISSION_REL_DTCS))
        {
            DEM_FLAGS_SET(pEvent->StatusIndicator30, DEM_SI30_DTC_ER);
        }
    }
#endif /* DEM_STATUSINDICATOR30ENABLE == STD_ON */

    /* if event failed clear anging counter*/
    pEvent->AgingCounter = (DEM_EVENT_STATUS_FAILED == Status) ? 0u : pEvent->AgingCounter;
#if (DEM_TRIGGERONFDCTHRESHOLDEANLE == STD_ON)
    uint16 DemDTCRef = DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef;
    /* req SWS_Dem_00786 req SWS_Dem_01068 req SWS_Dem_00811 req SWS_Dem_01069*/
    Dem_UpdateFDC(IntId);
    if ((DemDTCRef != DEM_DTC_REF_INVALID) && (FDCInfo[DemDTCRef].FDC > 0)
        && (DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[DemDTCRef].DemDTCAttributesRef]
                .DemEventMemoryEntryFdcThresholdStorageValue
            <= (uint8)FDCInfo[DemDTCRef].FDC)
        && ((DEM_DEBOUNCE_COUNTER_BASED == AlgorithmType)
#if (DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
            || ((DEM_DEBOUNCE_TIME_BASE == AlgorithmType)
                && (&DemDebounceTimerInfo[pEventCfg->AlgorithmIndex].Triggered == FALSE))
#endif
                ))
    {
        /* req SWS_Dem_00799 */
        DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_FDC_TRIGGERED);
        /* PRQA S 0431++ */ /* MISRA Rule 1.1 */
        Dem_EventDataStorageTrigger(IntId, DEM_TRIGGER_ON_FDC_THRESHOLD);
        /* PRQA S 0431-- */ /* MISRA Rule 1.1 */
    }
#endif /* DEM_TRIGGERONFDCTHRESHOLDEANLE == STD_ON */
    return Status;
}

/*************************************************************************/
/*
 * Brief               Dem_DebounceTimerMain
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
FUNC(void, DEM_CODE) Dem_DebounceTimerMain(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
    sint8 FDC;
    uint16 DemDTCRef;
    uint16 iloop;
    Dem_EventIdType IntId;
    Dem_EventRelateInformationType* pEvent;
    const Dem_EventParameterType* pEventCfg;
    const Dem_DebounceTimeBaseClassType* pTimerCfg;
    Dem_DebounceTimeInfoType* pTimer = DemDebounceTimerInfo;
#if (DEM_TRIGGER_FIM_REPORTS == STD_ON)
    uint8 OldStatus;
#endif

    for (iloop = 0; (iloop < DEM_DEBOUNCE_TIME_BASED_EVENT_NUM)
                    && (DemPbCfgPtr->DemEventParameter[pTimer->IntId].AlgorithmRef < DEM_DEBOUNCE_TIME_BASE_CLASS_NUM);
         iloop++)
    {
        pTimer = &DemDebounceTimerInfo[iloop];
        IntId = pTimer->IntId;
        if ((pTimer->Triggered == TRUE) && (pTimer->IsFreezing == FALSE))
        {
#if (DEM_TRIGGER_FIM_REPORTS == STD_ON)
            OldStatus = pTimer->CurStatus;
#endif
            if ((pTimer->CurStatus != DEM_EVENT_STATUS_FAILED) && (pTimer->CurStatus != DEM_EVENT_STATUS_PASSED))
            {
                pTimer->Timeout += DEM_TASK_TIME;
            }
            pTimerCfg = &DemPbCfgPtr->DemDebounceTimeBaseClass[DemPbCfgPtr->DemEventParameter[IntId].AlgorithmRef];
            pEventCfg = &DemPbCfgPtr->DemEventParameter[IntId];
            if ((pTimer->Timeout >= pTimerCfg->DemDebounceTimeFailedThreshold)
                && ((pTimer->CurStatus == DEM_EVENT_STATUS_PREFAILED)))
            {
                pTimer->CurStatus = DEM_EVENT_STATUS_FAILED;
                pTimer->Triggered = FALSE;
                if ((DEM_BITS_ISSET(DemOperationCycleStatus, pEventCfg->DemOperationCycleRef))
                    || (DemPbCfgPtr->DemOperationCycle[pEventCfg->DemOperationCycleRef].DemOperationCycleType
                        == DEM_OPCYC_OBD_DCY))
                {
                    Dem_EventTestFailed(IntId);
                }
            }
            else if (
                (pTimer->Timeout >= pTimerCfg->DemDebounceTimePassedThreshold)
                && ((pTimer->CurStatus == DEM_EVENT_STATUS_PREPASSED)))
            {
                pTimer->CurStatus = DEM_EVENT_STATUS_PASSED;
                pTimer->Triggered = FALSE;
                if ((DEM_BITS_ISSET(DemOperationCycleStatus, pEventCfg->DemOperationCycleRef))
                    || (DemPbCfgPtr->DemOperationCycle[pEventCfg->DemOperationCycleRef].DemOperationCycleType
                        == DEM_OPCYC_OBD_DCY))
                {
                    Dem_EventTestPassed(IntId);
                }
            }
            else
            {
                /*idle*/
            }
            Dem_UpdateFDC(IntId);
            pEventCfg = &DemPbCfgPtr->DemEventParameter[IntId];
            DemDTCRef = pEventCfg->DemDTCRef;
            FDC = FDCInfo[DemDTCRef].FDC;
            pEvent = Dem_GetEventInfo(IntId);
            if ((DemDTCRef != DEM_DTC_REF_INVALID) && (FDC > 0)
                && (DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[DemDTCRef].DemDTCAttributesRef]
                        .DemEventMemoryEntryFdcThresholdStorageValue
                    <= (uint8)FDC))
            {
                /*SWS_Dem_00799]*/
                DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_FDC_TRIGGERED);
                Dem_EventDataStorageTrigger(IntId, DEM_TRIGGER_ON_FDC_THRESHOLD);
            }
#if (DEM_TRIGGER_FIM_REPORTS == STD_ON)
            if (OldStatus != pTimer->CurStatus)
            {
                (void)FiM_DemTriggerOnEventStatus(IntId + 1u, OldStatus, pTimer->CurStatus);
            }
#endif
        }
    }
#endif
}

/*************************************************************************/
/*
 * Brief               Dem_UpdateFDC
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Dem_EventStatusType
 */
/*************************************************************************/
FUNC(void, DEM_CODE) Dem_UpdateFDC(Dem_EventIdType IntId) /* PRQA S 2755 */ /* MISRA CMN 0.4 */
{
    const Dem_EventParameterType* pEventCfg = &DemPbCfg.DemEventParameter[IntId];
#if (DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
    const Dem_DebounceCounterInfoType* pCounter;
    const Dem_DebounceCounterBasedClassType* pCfgCounter;
#endif /* DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0 */
#if (DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
    const Dem_DebounceTimeInfoType* pTimer;
    const Dem_DebounceTimeBaseClassType* pTimerCfg;
#endif /* DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0 */
#if ((DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0) || (DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0))
    sint64 IFDC;
#endif /*DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0) || (DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0 */
    sint8 FDC = 0;

    switch (pEventCfg->AlgorithmType)
    {
#if (DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
    case DEM_DEBOUNCE_COUNTER_BASED:
        if (((DemPbCfg.DemEventParameter[IntId].AlgorithmRef) < DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM)
            && ((pEventCfg->AlgorithmIndex) < DEM_DEBOUNCE_COUNTER_BASED_EVENT_NUM))
        {
            pCfgCounter = &DemPbCfg.DemDebounceCounterBasedClass[DemPbCfg.DemEventParameter[IntId].AlgorithmRef];
            pCounter = &DemDebounceCounterInfo[pEventCfg->AlgorithmIndex];
            if (pCounter->InternalDebounceCounter == 0)
            {
                FDC = 0;
            }
            else if (pCounter->InternalDebounceCounter > 0)
            {
                IFDC = (sint64)pCounter->InternalDebounceCounter * (sint64)127;
                FDC = (sint8)(IFDC / (sint64)pCfgCounter->DemDebounceCounterFailedThreshold);
            }
            else
            {
                IFDC = (sint64)pCounter->InternalDebounceCounter * ((sint64)(-128));
                FDC = (sint8)(IFDC / (sint64)pCfgCounter->DemDebounceCounterPassedThreshold);
            }
        }
        break;
#endif /* DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0 */
#if (DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
    case DEM_DEBOUNCE_TIME_BASE:
        if (((DemPbCfg.DemEventParameter[IntId].AlgorithmRef) < DEM_DEBOUNCE_TIME_BASE_CLASS_NUM)
            && ((pEventCfg->AlgorithmIndex) < DEM_DEBOUNCE_TIME_BASED_EVENT_NUM))
        {
            pTimer = &DemDebounceTimerInfo[pEventCfg->AlgorithmIndex];
            pTimerCfg = &DemPbCfg.DemDebounceTimeBaseClass[DemPbCfg.DemEventParameter[IntId].AlgorithmRef];
            switch (pTimer->CurStatus)
            {
            case DEM_EVENT_STATUS_PASSED:
                FDC = -128;
                break;
            case DEM_EVENT_STATUS_FAILED:
                FDC = 127;
                break;
            case DEM_EVENT_STATUS_PREFAILED:
                IFDC = (sint64)pTimer->Timeout * (sint64)127;
                FDC = (sint8)(IFDC / (sint64)pTimerCfg->DemDebounceTimeFailedThreshold);
                break;
            default:
                IFDC = (sint64)pTimer->Timeout * ((sint64)(-128));
                FDC = (sint8)(IFDC / (sint64)pTimerCfg->DemDebounceTimePassedThreshold);
                break;
            }
        }
        break;
#endif /* DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0 */
    default:
        /*idle*/
        break;
    }

    if (pEventCfg->DemDTCRef != DEM_DTC_REF_INVALID)
    {
        /* req SWS_Dem_00795 */
        if (FDC > 0)
        {
            if ((uint8)FDC > FDCInfo[pEventCfg->DemDTCRef].MaxFDCSinceLastClear)
            {
                FDCInfo[pEventCfg->DemDTCRef].MaxFDCSinceLastClear = (uint8)FDC;
            }
            if ((uint8)FDC > FDCInfo[pEventCfg->DemDTCRef].MaxFDCDuringCurrentCycle)
            {
                FDCInfo[pEventCfg->DemDTCRef].MaxFDCDuringCurrentCycle = (uint8)FDC;
            }
        }
        FDCInfo[pEventCfg->DemDTCRef].FDC = FDC;
    }
}

#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
/*************************************************************************/
/*
 * Brief               Dem_DTCCombinedGroupGetFDC
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      pDemDTC
 * Param-Name[out]     FaultDetectionCounter
 * Param-Name[in/out]  none
 * Return              Std_ReturnType
 */
/*************************************************************************/
DEM_LOCAL FUNC(Std_ReturnType, DEM_CODE)
    Dem_DTCCombinedGroupGetFDC(const Dem_DTCType* pDemDTC, sint8* FaultDetectionCounter)
{
    const Dem_EventIdType* pDTCMapping;
    Dem_EventIdType EventRefStart;
    Dem_EventIdType EventId;
    Std_ReturnType ret;
    sint8 Fdc;
    sint8 MaxFdc = -128;

    EventRefStart = pDemDTC->EventRefStart;
    pDTCMapping = &DemPbCfg.DemDTCMapping[EventRefStart];

    for (uint8 loop = 0u; loop < pDemDTC->EventRefNum; loop++)
    {
        EventId = Dem_GetEventExternalId(*pDTCMapping);
        ret = Dem_GetFaultDetectionCounter(EventId, &Fdc);
        if ((ret == E_OK) && (MaxFdc < Fdc))
        {
            MaxFdc = Fdc;
        }

        if (ret != E_OK)
        {
            break;
        }
        else
        {
            pDTCMapping++;
        }
    }
    if (ret == E_OK)
    {
        *FaultDetectionCounter = MaxFdc;
    }
    return ret;
}

#endif

/*************************************************************************/
/*
 * Brief               Dem_DTCGetFaultDetectionCounter
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      EventId
 * Param-Name[out]     FaultDetectionCounter
 * Param-Name[in/out]  none
 * Return              Std_ReturnType
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE) Dem_DTCGetFaultDetectionCounter(Dem_EventIdType EventId, sint8* FaultDetectionCounter)
{
    Std_ReturnType ret;

#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
    Dem_EventIdType IntId = Dem_GetEventInternalId(EventId);
    uint16 DTCRef = DemPbCfg.DemEventParameter[IntId].DemDTCRef;
    const Dem_DTCType* pDemDTC = &DemPbCfg.DemDTC[DTCRef];
    const Dem_EventIdType EventRefNum = pDemDTC->EventRefNum;
    if (EventRefNum != 1u)
    {
        ret = Dem_DTCCombinedGroupGetFDC(pDemDTC, FaultDetectionCounter);
    }
    else
#endif /* DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED */
    {
        ret = Dem_GetFaultDetectionCounter(EventId, FaultDetectionCounter);
    }
    return ret;
}

#if (DEM_NVRAM_BLOCKID_NUM > 0)
/*************************************************************************/
/*
 * Brief               Dem_IntWriteNvRAM
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  pEvent
 * Return              None
 */
/*************************************************************************/
/* PRQA S 1532,3432,3673++ */ /* MISRA Rule 8.7,8.13,20.7 */
FUNC(void, DEM_CODE)
Dem_InterIntWriteNvRAM(
    uint16 iloop,
    P2VAR(Dem_EventRelateInformationStorageType, AUTOMATIC, DEM_APPL_DATA) InformationStorage)
/* PRQA S 1532,3432,3673-- */ /* MISRA Rule 8.7,8.13,20.7 */
{
#if (DEM_DEBOUNCE_COUNTER_STORAGE_NUM > 0)
    /*SWS_Dem_00674*/
    if (DemPbCfgPtr->DemDebounceCounterBasedClass[DemPbCfgPtr->DemEventParameter[iloop].AlgorithmRef]
            .DemDebounceCounterStorage
        == TRUE)
    {
        InformationStorage->DebounceCounter[iloop] =
            (sint16)DemDebounceCounterInfo[DemPbCfgPtr->DemEventParameter[iloop].AlgorithmIndex]
                .InternalDebounceCounter;
    }
#else  /* DEM_DEBOUNCE_COUNTER_STORAGE_NUM > 0 */
    DEM_UNUSED(InformationStorage);
    DEM_UNUSED(iloop);
#endif /* DEM_DEBOUNCE_COUNTER_STORAGE_NUM > 0 */
}

/*************************************************************************/
/*
 * Brief               Dem_InterIntReadNvRAM
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  pEvent
 * Return              None
 */
/*************************************************************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(void, DEM_CODE)
Dem_InterIntReadNvRAM(
    uint16 iloop,
    P2CONST(Dem_EventRelateInformationStorageType, AUTOMATIC, DEM_APPL_DATA) InformationStorage)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
#if (DEM_DEBOUNCE_COUNTER_STORAGE_NUM > 0)
    /* req SWS_Dem_00674 */
    if (DemPbCfgPtr->DemDebounceCounterBasedClass[DemPbCfgPtr->DemEventParameter[iloop].AlgorithmRef]
            .DemDebounceCounterStorage
        == TRUE)
    {
        DemDebounceCounterInfo[DemPbCfgPtr->DemEventParameter[iloop].AlgorithmIndex].InternalDebounceCounter =
            (sint32)InformationStorage->DebounceCounter[iloop];
    }
#else  /* DEM_DEBOUNCE_COUNTER_STORAGE_NUM > 0 */
    DEM_UNUSED(InformationStorage);
    DEM_UNUSED(iloop);
#endif /* DEM_DEBOUNCE_COUNTER_STORAGE_NUM > 0 */
}
#endif /* DEM_NVRAM_BLOCKID_NUM > 0 */

/*************************************************************************/
/*
 * Brief               Gets the next freeze frame record number and its associated DTC
 *                          stored in the event memory.
 * ServiceId           none
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     DTC:Receives the DTC value in respective format of the filter
 *                          returned by this function.
 *                       RecordNumber:Freeze frame record number of the reported DTC
 *                          (relative addressing).
 * Param-Name[in/out]  none
 * Return              Status of the operation to retrieve a DTC and its associated
 *                          snapshot record number from the Dem.
 */
/*************************************************************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE)
Dem_InterDcmGetNextFilteredRecord(
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) RecordNumber)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    Dem_ReturnGetNextFilteredElementType ret = DEM_FILTERED_NO_MATCHING_ELEMENT;
#if (DEM_FREEZE_FRAME_CLASS_NUM > 0)
    const Dem_EventMemEntryType* pEntry;
    Dem_FreezeFrameRecordFilterInfoType* pFilter = &DemFreezeFrameRecordFilterInfo;
    Dem_DTCOriginType DTCOrigin;

    DTCOrigin = Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY);
    for (; (ret != DEM_FILTERED_OK) && (pFilter->GetNum < pFilter->NumberOfFilteredRecords);)
    {
        /* req SWS_Dem_00210 */
        if (pFilter->EntryIndex >= DemMemDestCfg[DTCOrigin].EntryNum)
        {
            pFilter->EntryIndex = 0;
            pFilter->FFIndex = 0;
        }
        else
        {
            pEntry = &DemMemDestCfg[DTCOrigin].EntryList[pFilter->EntryIndex];
            if (pFilter->FFIndex >= DEM_MAX_NUMBER_FF_RECORDS)
            {
                pFilter->FFIndex = 0;
                pFilter->EntryIndex++;
            }
            else
            {
                if (pEntry->EventId != 0u)
                {
                    Dem_EventIdType IntId = Dem_GetEventInternalId(pEntry->EventId);
                    uint16 DTCRef = DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef;
                    if ((DTCRef != DEM_DTC_REF_INVALID) && (DemDTCGeneralStatus[DTCRef].SuppressionStatus == FALSE))
                    {
#if (DEM_GLOBAL_FREEZE_FRAME_SUPPORT == STD_ON)
                        if (pEntry->GlobalFFList.RecordNum != 0xFFu)
                        {
                            Dem_GetEventDTC(DTCRef, pFilter->DTCFormat, DTC);
                            *RecordNumber = pEntry->GlobalFFList.RecordNum;
                            ret = DEM_FILTERED_OK; /* PRQA S 2469 */ /* MISRA Rule 14.2 */
                            pFilter->GetNum++;
                        }
                        else
#endif
                        {
                            uint8 index = pFilter->FFIndex;
                            if (pEntry->FFList[index].RecordNum != 0xFFu)
                            {
                                Dem_GetEventDTC(DTCRef, pFilter->DTCFormat, DTC);
                                *RecordNumber = pEntry->FFList[index].RecordNum;
                                ret = DEM_FILTERED_OK; /* PRQA S 2469 */ /* MISRA Rule 14.2 */
                                pFilter->GetNum++;
                            }
                            pFilter->FFIndex++;
                        }
                    }
                    else
                    {
                        pFilter->EntryIndex++;
                    }
                }
            }
        }
    }
#endif
    return ret;
}

/*************************************************************************/
/*
 * Brief               Sets a freeze frame record filter.
 * ServiceId           none
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      DTCFormat: Defines the output-format of the requested DTC values for the
 *                           sub-sequent API calls.
 * Param-Name[out]     NumberOfFilteredRecords:Number of freeze frame records currently stored
 *                           in the event memory.
 * Param-Name[in/out]  none
 * Return              Status of the operation to (re-)set a freeze frame record filter.
 */
/*************************************************************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(Dem_ReturnSetFilterType, DEM_CODE)
Dem_InterDcmSetFreezeFrameRecordFilter(
    Dem_DTCFormatType DTCFormat,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) NumberOfFilteredRecords)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    Dem_ReturnSetFilterType ret;
#if (DEM_FREEZE_FRAME_CLASS_NUM > 0)
    uint8 entryNum;
    const Dem_EventMemEntryType* pEntry;
    Dem_FreezeFrameRecordFilterInfoType* pFilter = &DemFreezeFrameRecordFilterInfo;
    Dem_DTCOriginType DTCOrigin = DEM_DTC_ORIGIN_PRIMARY_MEMORY;

    DTCOrigin = Dem_GetInternalMemDest(DTCOrigin);
    pFilter->DTCFormat = DTCFormat;
    *NumberOfFilteredRecords = 0;
    /* req SWS_Dem_00210 */
    for (entryNum = 0; entryNum < DemMemDestCfg[DTCOrigin].EntryNum; entryNum++)
    {
        pEntry = &DemMemDestCfg[DTCOrigin].EntryList[entryNum];
        if (pEntry->EventId != 0x00u)
        {
#if (DEM_GLOBAL_FREEZE_FRAME_SUPPORT == STD_ON)
            if (pEntry->GlobalFFList.RecordNum != 0xFFu)
            {
                *NumberOfFilteredRecords += pEntry->FFNum + 1u;
            }
            else
#endif
            {
                *NumberOfFilteredRecords += pEntry->FFNum;
            }
        }
    }
    pFilter->GetNum = 0;
    pFilter->EntryIndex = 0;
    pFilter->FFIndex = 0;
    pFilter->NumberOfFilteredRecords = *NumberOfFilteredRecords;
    ret = DEM_FILTER_ACCEPTED;
#else
    ret = DEM_WRONG_FILTER;
#endif
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets freeze frame data by DTC. The function stores the data
 *                          in the provided DestBuffer.
 * ServiceId           Non
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTC : Diagnostic Trouble Code in UDS format.
 *                       DTCOrigin:If the Dem supports more than one event memory,
 *                          this parameter is used to select the source memory
 *                          the DTCs shall be read from.
 *                       RecordNumber:This parameter is a unique identifier for a freeze
 *                          frame record as defined in ISO 15031-5 and ISO 14229-1.
 * Param-Name[out]     DestBuffer:This parameter contains a byte pointer that points
 *                          to the buffer, to which the freeze frame data record
 *                          shall be written to.
 * Param-Name[in/out]  BufSize:When the function is called this parameter contains
 *                          the maximum number of data bytes that can be written to the buffer.
 * Return              Status of the operation to retrieve freeze frame data by DTC.
 */
/*************************************************************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(Dem_ReturnGetFreezeFrameDataByDTCType, DEM_CODE)
Dem_InterDcmGetFreezeFrameDataByDTC(
    uint32 DTC,
    uint16 memDestAndRecordNumber,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    uint8 memDest;
    uint16 dtcIndex;
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
    uint16 iloop = 0;
#endif
    boolean getData = FALSE;
    boolean Find = FALSE;
    Dem_ReturnGetFreezeFrameDataByDTCType ret;
    const uint8* pFFData;
    Dem_EventMemEntryType* pEntry;
    const Dem_FreezeFrameInfoType* pFF;
    const Dem_DTCAttributesType* pDTCAttr;
    uint8 RecordNumber = (uint8)(memDestAndRecordNumber & 0xFFu);
    boolean isGlobalFF = FALSE;

    dtcIndex = Dem_GetDTCIndex(DTC, DEM_DTC_FORMAT_UDS);
    memDest = (uint8)(memDestAndRecordNumber >> 8u);
    *BufSize = 0u;
    const Dem_DTCType* pDemDTC = &DemPbCfgPtr->DemDTC[dtcIndex];
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
    const Dem_EventIdType EventRefNum = pDemDTC->EventRefNum;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
    const Dem_EventIdType EventRefStart = pDemDTC->EventRefStart;
    const Dem_EventIdType* pDTCMapping = &DemPbCfgPtr->DemDTCMapping[EventRefStart];

#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
    for (; iloop < EventRefNum; iloop++)
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
    {
#if (DEM_GLOBAL_FREEZE_FRAME_SUPPORT == STD_ON)
        if (RecordNumber == DemGlobalFreezeFrame.DemFreezeFrameRecordNumber)
        {
            Find = TRUE;
            isGlobalFF = TRUE;
            pEntry = Dem_MemEntryGet(Dem_GetEventExternalId(*pDTCMapping), memDest);
            if ((pEntry != NULL_PTR) && (pEntry->GlobalFFList.RecordNum != 0xFFu))
            {
                getData = TRUE;
                pFFData = pEntry->GlobalFFList.Data;
                Dem_DcmGetFreezeFrameDataByDTCProcess(pFFData, DestBuffer, BufSize, *pDTCMapping, isGlobalFF);
            }
        }
        if (Find != TRUE)
#endif
        {
            const Dem_DTCAttributesType* pDTCAttrCfg = Dem_EventDTCAttributesCfg(*pDTCMapping);
            uint16 FFClassRef = pDTCAttrCfg->DemFreezeFrameClassRef;
            if ((FFClassRef != DEM_FREEZE_FRAME_INVALID)
#if (DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)
                && (DEM_FREEZE_FRAME_REC_NUM_INVALID != pDTCAttrCfg->DemFreezeFrameRecNumClassRef)
#endif
            )
            {
#if (DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)

                const Dem_FreezeFrameRecNumClassType* PDemFFRecNumClass =
                    &DemFreezeFrameRecNumClass[pDTCAttrCfg->DemFreezeFrameRecNumClassRef];
                const uint8 RefNum = PDemFFRecNumClass->RefNum;
                const uint8* pFFRecordIndex = &DemFreezeFrameRecordClassRef[PDemFFRecNumClass->StartIndex];
                for (uint8 index = 0u; (index < RefNum) && (Find == FALSE); index++)
                {
                    if (((*pFFRecordIndex) != DEM_FREEZE_FRAME_RECORD_INVALID)
                        && (DemFreezeFrameRecordClass[*pFFRecordIndex].DemFreezeFrameRecordNumber == RecordNumber))
                    {
                        Find = TRUE;
                    }
                    pFFRecordIndex++;
                }
                if (Find == TRUE)
#endif
                {
                    pEntry = Dem_MemEntryGet(Dem_GetEventExternalId(*pDTCMapping), memDest);
                    if (pEntry != NULL_PTR)
                    {
#if ((DEM_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_PID_CLASS_NUM > 0))
                        pFF = Dem_FreezeFrameGetByRecordNum(pEntry, RecordNumber);
                        if (pFF != NULL_PTR)
                        {
                            getData = TRUE;
                            pFFData = pFF->Data;
                            pDTCAttr = Dem_EventDTCAttributesCfg(*pDTCMapping);
                            if ((pDTCAttr != NULL_PTR)
                                && (pDTCAttr->DemFreezeFrameClassRef != DEM_FREEZE_FRAME_INVALID))
                            {
                                Dem_DcmGetFreezeFrameDataByDTCProcess(
                                    pFFData,
                                    DestBuffer,
                                    BufSize,
                                    *pDTCMapping,
                                    isGlobalFF);
                            }
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
                            break;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
                        }
#endif
                    }
                }
            }
        }
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
        pDTCMapping++;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
    }
#if (DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)
    if (Find == FALSE)
    {
        ret = DEM_GET_FFDATABYDTC_WRONG_RECORDNUMBER;
    }
    else
#endif
        if (getData == FALSE)
    {
        ret = DEM_GET_FFDATABYDTC_FAILED;
    }
    else
    {
        ret = DEM_GET_FFDATABYDTC_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets the size of freeze frame data by DTC.
 * ServiceId           none
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTC : Diagnostic Trouble Code in UDS format
 *                       DTCOrigin : If the Dem supports more than one event memory,
 *                          this parameter is used to select the source memory
 *                          the DTCs shall be read from.
 *                       RecordNumber: This parameter is a unique identifier for a freeze
 *                          frame record as defined in ISO 15031-5 and ISO 14229-1.
 * Param-Name[out]     SizeOfFreezeFrame:
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(Dem_ReturnGetSizeOfDataByDTCType, DEM_CODE)
Dem_InterDcmGetSizeOfFreezeFrameByDTC(
    uint32 DTC,
    Dem_DTCOriginType DTCOrigin,
    uint8 RecordNumber,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) SizeOfFreezeFrame)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    Dem_ReturnGetSizeOfDataByDTCType ret = DEM_GETSIZEBYDTC_WRONG_DTC;
#if (DEM_FREEZE_FRAME_CLASS_NUM > 0u)
    uint8 index;
    uint16 dtcRef;
    uint8 memDest;
    const Dem_DTCAttributesType* pDTCAttr;
    const Dem_FreezeFrameInfoType* FFList;
    /*SWS_Dem_01100 SWS_Dem_01101 */
    dtcRef = Dem_GetDTCIndex(DTC, DEM_DTC_FORMAT_UDS);
    memDest = Dem_GetInternalMemDest(DTCOrigin);
    const Dem_DTCType* pDemDTC = &DemPbCfgPtr->DemDTC[dtcRef];
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
    const Dem_EventIdType EventRefNum = pDemDTC->EventRefNum;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
    const Dem_EventIdType EventRefStart = pDemDTC->EventRefStart;
    const Dem_EventIdType* pDTCMapping = &DemPbCfgPtr->DemDTCMapping[EventRefStart];
    *SizeOfFreezeFrame = 0u;
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
    for (uint16 iloop = 0u; iloop < EventRefNum; iloop++)
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
    {
        const Dem_EventMemEntryType* pEntry = Dem_MemEntryGet(Dem_GetEventExternalId(*pDTCMapping), memDest);
        if (pEntry != NULL_PTR)
        {
#if (DEM_GLOBAL_FREEZE_FRAME_SUPPORT == STD_ON)
            uint16 Size = DemFreezeFrameClass[DemGlobalFreezeFrame.DemFreezeFrameClassRef].DataSize;
            if (pEntry->GlobalFFList.RecordNum != 0xFFu)
            {
                *SizeOfFreezeFrame += Size;
            }
#endif
            pDTCAttr = Dem_EventDTCAttributesCfg(pEntry->EventId);
            if (pDTCAttr != NULL_PTR)
            {
                uint16 DataSize = DemFreezeFrameClass[pDTCAttr->DemFreezeFrameClassRef].DataSize;
                if ((RecordNumber == 0xFFu)
#if (DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)
                    && (DEM_FREEZE_FRAME_REC_NUM_INVALID != pDTCAttr->DemFreezeFrameRecNumClassRef)
#endif
                )
                {
#if (DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)
                    const uint8 RefNum = DemFreezeFrameRecNumClass[pDTCAttr->DemFreezeFrameRecNumClassRef].RefNum;
                    for (index = 0; index < RefNum; index++)
#else
                    for (index = 0; index < pDTCAttr->DemMaxNumberFreezeFrameRecords; index++)
#endif
                    {
                        FFList = &pEntry->FFList[index];
                        if (FFList->RecordNum != 0xFFu)
                        {
                            *SizeOfFreezeFrame += DataSize;
                        }
                    }
                }
                else
                {
                    *SizeOfFreezeFrame += DataSize;
                }
                ret = DEM_GETSIZEBYDTC_OK;
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
                break;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
            }
        }
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
        pDTCMapping++;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
    }
#endif
    return ret;
}

/*************************************************************************/
/*
 * Brief               BRIEF DESCRIPTION
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ExtendedDataNumber
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              const Dem_ExtendedDataRecordClassType*
 */
/*************************************************************************/
FUNC(P2CONST(Dem_ExtendedDataRecordClassType, AUTOMATIC, DEM_CONST), DEM_CODE)
Dem_GetExtendedDataRecordCfg(uint8 ExtendedDataNumber)
{
    const Dem_ExtendedDataRecordClassType* ret = (const Dem_ExtendedDataRecordClassType*)NULL_PTR;
#if (DEM_EXTENDED_DATA_RECORD_CLASS_NUM > 0)
    uint8 iloop;

    for (iloop = 0; iloop < DEM_EXTENDED_DATA_RECORD_CLASS_NUM; iloop++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
    {
        if (DemExtendedDataRecordClass[iloop].DemExtendedDataRecordNumber == ExtendedDataNumber)
        {
            ret = &(DemExtendedDataRecordClass[iloop]);
        }
    }
#endif
    return ret;
}

/*************************************************************************/
/*
 * Brief               BRIEF DESCRIPTION
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant/Non Reentrant
 * Param-Name[in]      IntId && Status
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(Dem_ReturnGetExtendedDataRecordByDTCType, DEM_CODE)
Dem_GetExtendedRecordDataLoop(
    uint32 DTC,
    uint16 memDestAndExtendedDataNumber,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    Dem_ReturnGetExtendedDataRecordByDTCType ret = DEM_RECORD_OK;
#if (DEM_EXTENDED_DATA_CLASS_NUM > 0u)
    uint16 dtcRef = Dem_GetDTCIndex(DTC, DEM_DTC_FORMAT_UDS);
    uint16 index;
    boolean Find = FALSE;
    uint8 memDest = (uint8)(memDestAndExtendedDataNumber >> 8u);
    uint8 ExtendedDataNumber = ((uint8)memDestAndExtendedDataNumber & 0xFFu);
    const uint16 DemDTCAttributesRef = DemPbCfgPtr->DemDTC[dtcRef].DemDTCAttributesRef;
    const Dem_DTCAttributesType* pDTCAttrCfg = &DemPbCfgPtr->DemDTCAttributes[DemDTCAttributesRef];
    const Dem_ExtendedDataClassType* pDemExtendedDataClass =
        &DemExtendedDataClass[pDTCAttrCfg->DemExtendedDataClassRef];
    const uint16 StartIndex = pDemExtendedDataClass->StartIndex;
    const uint8* pExtIndex;

    for (index = 0; index < pDemExtendedDataClass->RefNum; index++)
    {
        pExtIndex = &DemExtendedDataRecordClassRef[StartIndex + index];
        if ((*pExtIndex != DEM_EXTENDED_DATA_RECORD_INVALID)
            && (DemExtendedDataRecordClass[*pExtIndex].DemExtendedDataRecordNumber == ExtendedDataNumber))
        {
            Find = TRUE;
        }
    }
    if (Find == TRUE)
    {
        const Dem_DTCType* pDemDTC = &DemPbCfgPtr->DemDTC[dtcRef];
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
        const Dem_EventIdType EventRefNum = pDemDTC->EventRefNum;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
        const Dem_EventIdType EventRefStart = pDemDTC->EventRefStart;
        const Dem_EventIdType* pDTCMapping = &DemPbCfgPtr->DemDTCMapping[EventRefStart];
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
        for (uint16 iloop = 0u; (iloop < EventRefNum) && (ret == DEM_RECORD_OK); iloop++)
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
        {
            const Dem_EventMemEntryType* pEntry = Dem_MemEntryGet(Dem_GetEventExternalId(*pDTCMapping), memDest);
            if (pEntry != NULL_PTR)
            {
                ret = Dem_ExtendedDataGetFromEntry(pEntry, ExtendedDataNumber, DestBuffer, BufSize);
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
                break;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
            }
            else
            {
                *BufSize = 0u;
                ret = DEM_RECORD_OK;
            }
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
            pDTCMapping++;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
        }
    }
    else
    {
        ret = DEM_RECORD_WRONG_NUMBER;
    }
#else
    DEM_UNUSED(DTC);
    DEM_UNUSED(memDestAndExtendedDataNumber);
    DEM_UNUSED(DestBuffer);
    DEM_UNUSED(BufSize);
    ret = DEM_RECORD_WRONG_NUMBER;
#endif
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets the size of extended data by DTC.
 * ServiceId           none
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTC:Diagnostic Trouble Code in UDS format.
 *                      DTCOrigin:If the Dem supports more than one event memory ,
 *                          this parameter is used to select the source memory
 *                          the DTCs shall be read from.
 *                      ExtendedDataNumber:Identification/Number of requested extended
 *                          data record.
 * Param-Name[out]     SizeOfExtendedDataRecord:Size of the requested extended data record(s)
 *                          including record number size
 * Param-Name[in/out]  none
 * Return              Status of the operation to retrieve the size of extended data.
 */
/*************************************************************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(Dem_ReturnGetSizeOfDataByDTCType, DEM_CODE)
Dem_InterDcmGetSizeOfExtendedDataRecordByDTC(
    uint32 DTC,
    Dem_DTCOriginType DTCOrigin,
    uint8 ExtendedDataNumber,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) SizeOfExtendedDataRecord)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    Dem_ReturnGetSizeOfDataByDTCType ret = DEM_GETSIZEBYDTC_OK;
#if (DEM_EXTENDED_DATA_CLASS_NUM > 0)
    uint8 memDest;
    uint16 dtcRef;

    dtcRef = Dem_GetDTCIndex(DTC, DEM_DTC_FORMAT_UDS);
    memDest = Dem_GetInternalMemDest(DTCOrigin);
    *SizeOfExtendedDataRecord = 0;
    uint8 Start;
    uint8 End;
    if (ExtendedDataNumber == 0xFEu)
    {
        Start = 0x90u;
        End = 0xEFu;
    }
    else if (ExtendedDataNumber == 0xFFu)
    {
        Start = 0x01u;
        End = 0xEFu;
    }
    else
    {
        Start = ExtendedDataNumber;
        End = ExtendedDataNumber + 1u;
    }
    for (; (Start < End) && (DEM_GETSIZEBYDTC_OK == ret); Start++)
    {
        const Dem_ExtendedDataRecordClassType* pExtDataCfg = NULL_PTR;
        pExtDataCfg = Dem_GetExtendedDataRecordCfg(ExtendedDataNumber);
        if (pExtDataCfg == NULL_PTR)
        {
            ret = DEM_GETSIZEBYDTC_WRONG_RECNUM;
        }
        else
        {
            Dem_DcmGetSizeOfExtDataRecLoop(dtcRef, memDest, SizeOfExtendedDataRecord, pExtDataCfg);
        }
    }
#else
    DEM_UNUSED(DTC);
    DEM_UNUSED(DTCOrigin);
    DEM_UNUSED(ExtendedDataNumber);
    DEM_UNUSED(SizeOfExtendedDataRecord);
#endif
    return ret;
}

/*************************************************************************/
/*
 * Brief               Disables the DTC setting for a DTC group.
 * ServiceId           none
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTCGroup:Defines the group of DTC that shall be disabled to
 *                          store in event memory.
 *                       DTCKind:This parameter defines the requested DTC kind, either
 *                          only OBD-relevant DTCs or all DTCs>
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Returns status of the operation>
 */
/*************************************************************************/
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(Dem_ReturnControlDTCSettingType, DEM_CODE)
Dem_InterDcmDisableDTCSetting(uint32 DTCGroup, Dem_DTCKindType DTCKind)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    Dem_ReturnControlDTCSettingType ret;
    boolean ProcessOn = TRUE;
#if (DEM_GROUP_OF_DTC_NUM > 0)
    uint8 Groupindex;

    if ((DTCGroup & 0xFFFFFFUL) == 0xFFFFFFUL)
    {
        DemDTCSettingInfo.AllGroupIsEnabled = FALSE;
        Dem_MemSet(DemDTCSettingInfo.DTCGroupStatus, 0x00u, DEM_GROUP_OF_DTC_NUM_BYTE);
    }
    else
    {
        Groupindex = Dem_GetDTCGroupIndex(DTCGroup);
        if (Groupindex == DEM_GROUP_OF_DTC_INVALID)
        {
            ret = DEM_CONTROL_DTC_WRONG_DTCGROUP;
            ProcessOn = FALSE;
        }
        else
        {
            DEM_BITS_CLR(DemDTCSettingInfo.DTCGroupStatus, DTCGroup);
        }
    }
#else
    if ((DTCGroup & 0xFFFFFFUL) != 0xFFFFFFUL)
    {
        DEM_DET_REPORT(DEM_SID_DCMDISABLEDTCSETTING, DEM_E_WRONG_CONFIGURATION);
        ret = DEM_CONTROL_DTC_WRONG_DTCGROUP;
        ProcessOn = FALSE;
    }
    else
    {
        DemDTCSettingInfo.AllGroupIsEnabled = FALSE;
    }
#endif
    if (ProcessOn == TRUE)
    {
        DemDTCSettingInfo.DTCKind = DTCKind;
        Dem_SetDTCSettingProcess();
        ret = DEM_CONTROL_DTC_SETTING_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Enables the DTC setting for a DTC group.
                        This API is intended for the Dcm. It can only be used through the RTE
                        (due to work-around described below SWS_Dem_00035), and therefore
                        no declaration is exported via Dem_Dcm.h.
 * ServiceId           none
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTCGroup:Defines the group of DTC that shall be enabled to
 *                          store in event memory.
 *                       DTCKind:This parameter defines the requested DTC kind,
 *                          either only OBD-relevant DTCs or all DTCs
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Returns the status of the operation
 */
/*************************************************************************/
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(Dem_ReturnControlDTCSettingType, DEM_CODE)
Dem_InterDcmEnableDTCSetting(uint32 DTCGroup, Dem_DTCKindType DTCKind)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    Dem_ReturnControlDTCSettingType ret;
    boolean ProcessOn = TRUE;
#if (DEM_GROUP_OF_DTC_NUM > 0)
    uint8 GroupIndex;

    if ((DTCGroup & 0xFFFFFFUL) == 0xFFFFFFUL)
    {
        DemDTCSettingInfo.AllGroupIsEnabled = TRUE;
        Dem_MemSet(DemDTCSettingInfo.DTCGroupStatus, 0xFFu, DEM_GROUP_OF_DTC_NUM_BYTE);
    }
    else
    {
        GroupIndex = (Dem_GetDTCGroupIndex(DTCGroup));
        if ((uint8)GroupIndex == DEM_GROUP_OF_DTC_INVALID)
        {
            ret = DEM_CONTROL_DTC_WRONG_DTCGROUP;
            ProcessOn = FALSE;
        }
        else
        {
            DEM_BITS_SET(DemDTCSettingInfo.DTCGroupStatus, GroupIndex);
        }
    }
#else
    if ((DTCGroup & 0xFFFFFFUL) != 0xFFFFFFUL)
    {
        DEM_DET_REPORT(DEM_SID_DCMENABLEDTCSETTING, DEM_E_WRONG_CONFIGURATION);
        ret = DEM_CONTROL_DTC_WRONG_DTCGROUP;
        ProcessOn = FALSE;
    }
    else
    {
        DemDTCSettingInfo.AllGroupIsEnabled = TRUE;
    }
#endif
    if (ProcessOn == TRUE)
    {
        DemDTCSettingInfo.DTCKind = DTCKind;
        Dem_SetDTCSettingProcess();
        ret = DEM_CONTROL_DTC_SETTING_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Dem_GetDemMalfunctionLampIndicator
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint8
 */
/*************************************************************************/
FUNC(uint8, DEM_CODE) Dem_GetDemMalfunctionLampIndicator(void)
{
    return DEM_MALFUNCTION_LAMP_INDICATOR;
}

#if (DEM_DATA_ELEMENT_CLASS_NUM > 0u)
/*************************************************************************/
/*
 * Brief               Dem_GetFncData>
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      DataReadFnc,DataSize
 * Param-Name[out]     pBuffer
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
FUNC(void, DEM_CODE)
Dem_GetFncData(DemDataElementReadFncType DataReadFnc, uint8* pBuffer, uint8 DataSize)
{
    if (DataReadFnc != NULL_PTR)
    {
        if (DataReadFnc(pBuffer) != E_OK)
        {
            Dem_MemSet(pBuffer, 0xFFu, DataSize);
            DEM_DET_REPORT(DEM_SID_MAINFUNCTION, DEM_E_NODATAAVAILABLE);
        }
    }
}
#endif /* DEM_DATA_ELEMENT_CLASS_NUM > 0u */

#if ((DEM_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_PID_CLASS_NUM > 0))
/*************************************************************************/
/*
 * Brief               Dem_GetGeneralFreezeFrame
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      tempRef
 * Param-Name[out]     pBuffer
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
FUNC(void, DEM_CODE)
Dem_GetGeneralFreezeFrame(uint16 tempRef, uint8* pBuffer) /* PRQA S 1505 */ /* MISRA Rule 8.7 */
{
#if (DEM_DID_CLASS_NUM > 0)
    uint8 offset = 0;
    uint16 didRefNum = DemFreezeFrameClass[tempRef].RefNum;
    const uint16* pIndex = &DemDidClassRef[DemFreezeFrameClass[tempRef].StartIndex];

    for (uint16 iloop = 0; iloop < didRefNum; iloop++)
    {
        const Dem_DidClassType* pDid = &DemDidClass[*pIndex];
        uint8 csNum = pDid->RefNum;
        uint16 csStart = pDid->StartIndex;
        for (uint8 csIndex = 0; csIndex < csNum; csIndex++)
        {
#if (DEM_DATA_ELEMENT_CLASS_NUM > 0)
            uint8 DataSize = DemPbCfg.DemDataElementClass[csStart].DemDataElementDataSize;
            uint8* DestBuffer = &pBuffer[offset];
            DemDataElementReadFncType DataReadFnc = DemPbCfg.DemDataElementClass[csStart].DemDataElementReadFnc;
            Dem_GetFncData(DataReadFnc, DestBuffer, DataSize);
            offset += DataSize;
#endif
            csStart++;
        }
        pIndex++;
    }
#else
    DEM_UNUSED(tempRef);
    DEM_UNUSED(pBuffer);
#endif
}

/*************************************************************************/
/*
 * Brief               Dem_GetJ1939FreezeFrame
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      tempRef
 * Param-Name[out]     pBuffer
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
FUNC(void, DEM_CODE)
/* PRQA S 1505,3432,3673++ */ /* MISRA Rule 8.7,20.7,8.13 */
Dem_GetJ1939FreezeFrame(uint16 tempRef, P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) pBuffer)
/* PRQA S 1505,3432,3673-- */ /* MISRA Rule 8.7,20.7,8.13 */
{
#if (DEM_SPN_CLASS_NUM > 0u)
    uint8 offset = 0;
    uint16 SpnRefNum = DemJ1939FreezeFrameClass[tempRef].RefNum;
    const uint16* SpnIndex = &DemSPNClassRef[DemJ1939FreezeFrameClass[tempRef].StartIndex];

    for (uint16 iloop = 0; iloop < SpnRefNum; iloop++)
    {
        uint16 index = DemSPNClass[*SpnIndex].StartIndex;
#if (DEM_DATA_ELEMENT_CLASS_NUM > 0)
        uint8 DataSize = DemPbCfg.DemDataElementClass[index].DemDataElementDataSize;
        uint8* DestBuffer = &pBuffer[offset];
        DemDataElementReadFncType DataReadFnc = DemPbCfg.DemDataElementClass[index].DemDataElementReadFnc;
        Dem_GetFncData(DataReadFnc, DestBuffer, DataSize);
        offset += DataSize;
#endif
        SpnIndex++;
    }
#else
    DEM_UNUSED(tempRef);
    DEM_UNUSED(pBuffer);
#endif
}

/*************************************************************************/
/*
 * Brief               Dem_GetObdFreezeFrame
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     pBuffer
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
/* PRQA S 1505,3432,3673++ */ /* MISRA Rule 8.7,20.7,8.13 */
FUNC(void, DEM_CODE)
Dem_GetObdFreezeFrame(P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) pBuffer)
/* PRQA S 1505,3432,3673-- */ /* MISRA Rule 8.7,20.7,8.13 */
{
#if (DEM_PID_CLASS_NUM > 0u)
    uint8 offset = 0;

    for (uint16 pPidIndex = 0u; pPidIndex < DEM_PID_CLASS_NUM; pPidIndex++)
    {
        const Dem_PidClassType* pPid = &DemPbCfg.DemPidClass[pPidIndex];
        uint16 pidRefNum = pPid->StartIndex;
        for (uint16 index = 0; index < pPid->RefNum; index++)
        {
#if (DEM_DATA_ELEMENT_CLASS_NUM > 0)
            uint8 DataSize = DemPbCfg.DemDataElementClass[pidRefNum].DemDataElementDataSize;
            uint8* DestBuffer = &pBuffer[offset];
            DemDataElementReadFncType DataReadFnc = DemPbCfg.DemDataElementClass[pidRefNum].DemDataElementReadFnc;
            Dem_GetFncData(DataReadFnc, DestBuffer, DataSize);
            offset += DataSize;
#endif
            pidRefNum++;
        }
    }
#else
    DEM_UNUSED(pBuffer);
#endif
}

/*************************************************************************/
/*
 * Brief               Dem_CheckFreezeFrameIsStorage
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId && Trigger
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/*************************************************************************/
/* PRQA S 1505++ */ /* MISRA Rule 8.7 */
FUNC(boolean, DEM_CODE)
Dem_CheckFreezeFrameIsStorage(Dem_EventIdType IntId, uint8 Trigger)
/* PRQA S 1505-- */ /* MISRA Rule 8.7 */
{
    boolean res = FALSE;
#if (                                                                        \
    (DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED) \
    && (DEM_FREEZE_FRAME_RECORD_CLASS_NUM > 0))
    const Dem_DTCAttributesType* pDTCAttrCfg = Dem_EventDTCAttributesCfg(IntId);
    uint8 pFFRecordIndex;
    uint8 iloop;
    if (pDTCAttrCfg != NULL_PTR)
    {
        for (iloop = 0;
             (DEM_FREEZE_FRAME_REC_NUM_INVALID != pDTCAttrCfg->DemFreezeFrameRecNumClassRef)
             && (iloop < DemFreezeFrameRecNumClass[pDTCAttrCfg->DemFreezeFrameRecNumClassRef].RefNum) && (res == FALSE);
             iloop++)
        {
            pFFRecordIndex = DemFreezeFrameRecordClassRef
                [DemFreezeFrameRecNumClass[pDTCAttrCfg->DemFreezeFrameRecNumClassRef].StartIndex + iloop];
            if ((pFFRecordIndex != DEM_FREEZE_FRAME_RECORD_INVALID)
                && (DemFreezeFrameRecordClass[pFFRecordIndex].DemFreezeFrameRecordTrigger == Trigger))
            {
                res = TRUE;
            }
        }
    }
#endif
    return res;
}

/*************************************************************************/
/*
 * Brief               Dem_FreezeFrameCopy
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId && pEventBuffer
 * Param-Name[out]     pFFBuffer
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
/* PRQA S 1505,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(void, DEM_CODE)
Dem_FreezeFrameCopy(Dem_EventIdType IntId, P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) pFFBuffer)
/* PRQA S 1505,3432-- */ /* MISRA Rule 8.7,20.7 */
{
#if (DEM_MAX_NUMBER_PRESTORED_FF > 0)
    Dem_PreStoreFFInfoType* pPreFF = NULL_PTR;
    if (DemPbCfgPtr->DemEventParameter[IntId].DemFFPrestorageSupported == TRUE)
    {
        pPreFF = Dem_PreStoreFFGet(Dem_GetEventExternalId(IntId));
    }
    if (pPreFF != NULL_PTR)
    {
/* req SWS_Dem_00464 */
#if (DEM_FREEZE_FRAME_MAX_LEN > 0u)
        Dem_MemCopy(pFFBuffer, pPreFF->FFData, DEM_FREEZE_FRAME_MAX_LEN);
        Dem_MemSet(pPreFF->FFData, 0xFFu, DEM_FREEZE_FRAME_MAX_LEN);
#endif
        /* req SWS_Dem_00969 */
        pPreFF->ExtId = 0x00;
    }
    else
#endif /* DEM_MAX_NUMBER_PRESTORED_FF > 0 */
    {
        Dem_GetFreezeFrame(IntId, pFFBuffer);
    }
}

/*************************************************************************/
/*
 * Brief               Dem_GetFreezeFrame
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId
 * Param-Name[out]     pBuffer
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
FUNC(void, DEM_CODE)
/* PRQA S 3432,1505++ */ /* MISRA Rule 20.7,8.7 */
Dem_GetFreezeFrame(Dem_EventIdType IntId, P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) pBuffer)
/* PRQA S 3432,1505-- */ /* MISRA Rule 20.7,8.7 */
{
    uint16 tempRef;
    uint16 DTCAttributesRef;
    uint16 obdDtcRef;
    const uint16 DemDTCRef = DemPbCfg.DemEventParameter[IntId].DemDTCRef;

    Dem_UpdateInternalData(IntId);
    /* DTCAttributesRef */
    DTCAttributesRef = DemPbCfg.DemDTC[DemDTCRef].DemDTCAttributesRef;
    obdDtcRef = DemPbCfg.DemDTC[DemDTCRef].DemObdDTCRef;
    /* FreezeFrameRef */
    tempRef = DemPbCfg.DemDTCAttributes[DTCAttributesRef].DemFreezeFrameClassRef;
    if (tempRef != DEM_FREEZE_FRAME_INVALID)
    {
        Dem_GetGeneralFreezeFrame(tempRef, pBuffer);
    }
    else
    {
        /* WWHOBDFreezeFrameRef */
        tempRef = DemPbCfg.DemDTCAttributes[DTCAttributesRef].DemWWHOBDFreezeFrameClassRef;
        if (tempRef != DEM_WWHOBD_FREEZE_FRAME_INVALID)
        {
            Dem_GetGeneralFreezeFrame(tempRef, pBuffer);
        }
        else
        {
            /* J1939FreezeFrameRef */
            tempRef = DemPbCfg.DemDTCAttributes[DTCAttributesRef].DemJ1939FreezeFrameClassRef;
            if ((tempRef != DEM_J1939_FREEZE_FRAME_INVALID) && (obdDtcRef != DEM_OBD_DTC_INVALID)
                && (DemPbCfg.DemObdDTC[obdDtcRef].DemJ1939DTCValue != DEM_J1939_DTC_CFG_INVALID))
            {
                Dem_GetJ1939FreezeFrame(tempRef, pBuffer);
            }
            else
            {
                /* J1939EXFreezeFrameRef */
                tempRef = DemPbCfg.DemDTCAttributes[DTCAttributesRef].DemJ1939ExpandedFreezeFrameClassRef;
                if ((tempRef != DEM_J1939_FREEZE_FRAME_INVALID) && (obdDtcRef != DEM_OBD_DTC_INVALID)
                    && (DemPbCfg.DemObdDTC[obdDtcRef].DemJ1939DTCValue != DEM_J1939_DTC_CFG_INVALID))
                {
                    Dem_GetJ1939FreezeFrame(tempRef, pBuffer);
                }
                else
                {
                    if ((obdDtcRef != DEM_OBD_DTC_INVALID)
                        && (DemPbCfg.DemObdDTC[obdDtcRef].DemDtcValue != DEM_OBD_DTC_CFG_INVALID))
                    {
                        /*no other FreezeFrameRef ,take the obd FreezeFrame*/
                        Dem_GetObdFreezeFrame(pBuffer);
                    }
                }
            }
        }
    }
}
#endif /* DEM_FREEZE_FRAME_CLASS_NUM > 0 || DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0 || \
          DEM_PID_CLASS_NUM > 0 */

/*************************************************************************/
/*
 * Brief               Dem_ExtendedDataGetFromEntry
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  BufSize
 * Return              None
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_ExtendedDataGetFromEntry(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2CONST(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA) pEntry,
    uint8 RecordNum,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) pBuffer,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
#if (DEM_EXTENDED_DATA_CLASS_NUM > 0)
    Std_ReturnType ret = DEM_E_WRONG_RECORDNUMBER;
    const Dem_DTCAttributesType* pDTCAttrCfg = Dem_EventDTCAttributesCfg(Dem_GetEventInternalId(pEntry->EventId));
    const Dem_ExtendedDataRecordClassType* pExtRecord;
    uint8 iloop;
    uint8 cnt = 0;
    const uint8* pExt = pEntry->ExtData;

    if (pDTCAttrCfg != NULL_PTR)
    {
        const uint16 EDClassRef = pDTCAttrCfg->DemExtendedDataClassRef;
        if (DEM_EXTENDED_DATA_INVALID != EDClassRef)
        {
            const Dem_ExtendedDataClassType* pEDClass = &DemExtendedDataClass[EDClassRef];
            uint8 RefNum = pEDClass->RefNum;
            const uint8* pExtIndex = &DemExtendedDataRecordClassRef[pEDClass->StartIndex];
            for (iloop = 0; (iloop < RefNum) && (ret == DEM_E_WRONG_RECORDNUMBER); iloop++)
            {
                if (*pExtIndex != DEM_EXTENDED_DATA_RECORD_INVALID)
                {
                    pExtRecord = &DemExtendedDataRecordClass[*pExtIndex];
                    if (pExtRecord->DemExtendedDataRecordNumber == RecordNum)
                    {
                        if (DEM_BITS_ISNSET(pEntry->ExtStatus, iloop))
                        {
                            ret = DEM_E_NODATAAVAILABLE;
                        }
                        else
                        {
#if (DEM_DATA_ELEMENT_CLASS_NUM > 0u)
                            uint8 DataElementNum = pExtRecord->DemDataElementClassNum;
                            uint8 offset = 0;
                            Dem_UpdateInternalData(Dem_GetEventInternalId(pEntry->EventId));
                            for (uint8 index = 0; index < DataElementNum; index++)
                            {
                                const DemDataElementClassTypes* pDemDataElementClass =
                                    &DemPbCfgPtr
                                         ->DemDataElementClass[pExtRecord->DemDataElementClassIndex + (uint16)index];

                                if (pDemDataElementClass->DemDataElementReadFnc != NULL_PTR)
                                {
                                    uint8 ElementDataSize = pDemDataElementClass->DemDataElementDataSize;
                                    if (FALSE == pDemDataElementClass->DemInterFlag)
                                    {
                                        Dem_MemCopy(&pBuffer[offset], &pExt[cnt + offset], ElementDataSize);
                                    }
                                    else
                                    {
                                        /* get inter data now */
                                        pDemDataElementClass->DemDataElementReadFnc(&pBuffer[offset]);
                                    }
                                    offset += ElementDataSize;
                                }
                            }
#endif
                            *BufSize = pExtRecord->DataSize;
                            ret = E_OK;
                        }
                    }
                    else
                    {
                        cnt += pExtRecord->DataSize;
                    }
                }
                pExtIndex++;
            }
        }
    }
    return ret;
#else
    DEM_UNUSED(pEntry);
    DEM_UNUSED(RecordNum);
    DEM_UNUSED(pBuffer);
    DEM_UNUSED(BufSize);
    return DEM_E_NODATAAVAILABLE;
#endif
}

#if (DEM_EXTENDED_DATA_CLASS_NUM > 0)
/*************************************************************************/
/*
 * Brief               Dem_GetExtendedData>
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId
 * Param-Name[out]     pBuffer
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
/* PRQA S 1505,3432++ */ /* MISRA Rule 8.7,20.7 */
DEM_LOCAL FUNC(void, DEM_CODE) Dem_GetExtendedData(const Dem_ExtendedDataRecordClassType* pExtRecord, uint8* pBuffer)
/* PRQA S 1505,3432-- */ /* MISRA Rule 8.7,20.7 */
{
#if (DEM_DATA_ELEMENT_CLASS_NUM > 0u)
    uint16 DataIndex = pExtRecord->DemDataElementClassIndex;
    uint8 cnt = pExtRecord->DemDataElementClassNum;
    uint8 offset = 0;

    for (uint16 index = 0; index < cnt; index++)
    {
        const DemDataElementClassTypes* pDataElementClass = &DemPbCfg.DemDataElementClass[DataIndex];
        uint8 DataSize = pDataElementClass->DemDataElementDataSize;
        uint8* DestBuffer = &pBuffer[offset];
        DemDataElementReadFncType DataReadFnc = pDataElementClass->DemDataElementReadFnc;
        Dem_GetFncData(DataReadFnc, DestBuffer, DataSize);
        offset += DataSize;
        DataIndex++;
    }
#endif /* DEM_DATA_ELEMENT_CLASS_NUM > 0u */
}

/*************************************************************************/
/*
 * Brief               Dem_CheckExtendedDataIsStorage
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId && Trigger
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/*************************************************************************/
/* PRQA S 1505++ */ /* MISRA Rule 8.7 */
FUNC(boolean, DEM_CODE)
Dem_CheckExtendedDataIsStorage(Dem_EventIdType IntId, uint8 Trigger)
/* PRQA S 1505-- */ /* MISRA Rule 8.7 */
{
    boolean res = FALSE;
    const Dem_DTCAttributesType* pDTCAttrCfg = Dem_EventDTCAttributesCfg(IntId);
    uint8 pExtIndex;
    uint8 iloop;

    if ((pDTCAttrCfg != NULL_PTR) && (pDTCAttrCfg->DemExtendedDataClassRef != DEM_EXTENDED_DATA_INVALID))
    {
        for (iloop = 0; (iloop < DemExtendedDataClass[pDTCAttrCfg->DemExtendedDataClassRef].RefNum) && (res == FALSE);
             iloop++)
        {
            pExtIndex = DemExtendedDataRecordClassRef
                [DemExtendedDataClass[pDTCAttrCfg->DemExtendedDataClassRef].StartIndex + iloop];
            if (pExtIndex != DEM_EXTENDED_DATA_RECORD_INVALID)
            {
                if (DemExtendedDataRecordClass[pExtIndex].DemExtendedDataRecordTrigger == Trigger)
                {
                    res = TRUE;
                }
            }
        }
    }
    return res;
}

/*************************************************************************/
/*
 * Brief               Dem_ExtendedDataStorage
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId && pEventBuffer && Trigger
 * Param-Name[out]     None
 * Param-Name[in/out]  pEntry
 * Return              None
 */
/*************************************************************************/
/* PRQA S 1505,3432++ */ /* MISRA Rule 8.7,20.7 */
DEM_LOCAL FUNC(void, DEM_CODE) Dem_ExtendedDataStorage(
    Dem_EventIdType IntId,
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA) pEntry,
    uint8 Trigger)
/* PRQA S 1505,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    const Dem_DTCAttributesType* pDTCAttrCfg = Dem_EventDTCAttributesCfg(IntId);
    const Dem_ExtendedDataRecordClassType* pExtRecord;
    uint8 pExtIndex;
    uint8 iloop;
    uint8 Offset = 0;
    uint16 DemExtendedDataClassRef = pDTCAttrCfg->DemExtendedDataClassRef;

    if ((pDTCAttrCfg != NULL_PTR) && (DemExtendedDataClassRef != DEM_EXTENDED_DATA_INVALID))
    {
        Dem_UpdateInternalData(IntId);
        for (iloop = 0; iloop < DemExtendedDataClass[DemExtendedDataClassRef].RefNum; iloop++)
        {
            pExtIndex = DemExtendedDataRecordClassRef[DemExtendedDataClass[DemExtendedDataClassRef].StartIndex + iloop];
            if (pExtIndex != DEM_EXTENDED_DATA_RECORD_INVALID)
            {
                /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
                const Dem_EventRelateInformationType* pEvent = Dem_GetEventInfo(IntId);
                /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
                pExtRecord = &DemExtendedDataRecordClass[pExtIndex];
                uint8 RecordTrigger = pExtRecord->DemExtendedDataRecordTrigger;
                uint8 Update = pExtRecord->DemExtendedDataRecordUpdate;
                /* PRQA S 3415++ */ /* MISRA Rule 13.5 */
                if ((RecordTrigger == Trigger)
                    && ((DEM_EVENT_STATUS_FDC_TRIGGERED != Trigger)
                        || (DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_FDC_TRIGGERED) == 0x00u))
                    && ((Update == DEM_UPDATE_RECORD_YES) || (DEM_BITS_ISNSET(pEntry->ExtStatus, iloop))))
                /* PRQA S 3415-- */ /* MISRA Rule 13.5 */
                {
                    /* req SWS_Dem_00815  req SWS_Dem_00816 */
                    /*set the corresponding bit*/
                    DEM_BITS_SET(pEntry->ExtStatus, iloop);
#if (DEM_DATA_ELEMENT_CLASS_NUM > 0u)
                    Dem_GetExtendedData(pExtRecord, &(pEntry->ExtData[Offset]));
#endif /* DEM_DATA_ELEMENT_CLASS_NUM > 0u */
                }
#if (DEM_TRIGGERONMIRROREANLE == STD_ON)
#if (DEM_MAX_NUMBER_EVENT_ENTRY_MIRROR > 0u)
                if ((RecordTrigger == DEM_TRIGGER_ON_MIRROR)
                    && ((Update == DEM_UPDATE_RECORD_YES)
                        || ((DEM_BITS_ISNSET(pEntry->ExtStatus, iloop)) && (Update == DEM_UPDATE_RECORD_NO))))
                {
                    /* req SWS_Dem_01071 */
                    Dem_DTCOriginType pMemDest = Dem_GetInternalMemDest(DEM_DTC_ORIGIN_MIRROR_MEMORY);
                    if (pMemDest != DEM_MEM_DEST_INVALID)
                    {
                        pEntry = Dem_MemEntryAllocate(IntId, pMemDest);

                        if ((pEntry != NULL_PTR)
                            && (0x00u
                                == DEM_BITS_ISSET(
                                    DemMemDestInfo[pMemDest].DisableDTCRecordUpdate,
                                    DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef)))
                        {
                            /*set the corresponding bit*/
                            DEM_BITS_SET(pEntry->ExtStatus, iloop);
#if (DEM_DATA_ELEMENT_CLASS_NUM > 0u)
                            Dem_GetExtendedData(pExtRecord, &(pEntry->ExtData[Offset]));
#endif /* DEM_DATA_ELEMENT_CLASS_NUM > 0u */
                        }
                    }
                }
#endif /* DEM_MAX_NUMBER_EVENT_ENTRY_MIRROR > 0u */
#endif /* DEM_TRIGGERONMIRROREANLE == STD_ON */
                Offset += pExtRecord->DataSize;
            }
        }
    }
}
#endif /* DEM_EXTENDED_DATA_CLASS_NUM > 0 */

/*************************************************************************/
/*
 * Brief               Dem_FreezeFrameGetFromEntry
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      pEntry && RecordNum && Did
 * Param-Name[out]     None
 * Param-Name[in/out]  pBuffer && BufSize
 * Return              Std_ReturnType
 */
/*************************************************************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_FreezeFrameGetFromEntry(
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA) pEntry,
    uint8 RecordNum,
    uint16 Did,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) pBuffer,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
#if ((DEM_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_PID_CLASS_NUM > 0))
    const Dem_FreezeFrameInfoType* pFF;
#endif
#if (DEM_DID_CLASS_NUM > 0)
    uint16 Ref;
    uint8 cnt = 0;
    uint16 didRefNum;
    uint16 pDidIndex;
    const Dem_DidClassType* pDid;
    const uint8* pData;
#endif /* DEM_DID_CLASS_NUM > 0 */
    Std_ReturnType ret = DEM_E_WRONG_DIDNUMBER;

#if ((DEM_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_PID_CLASS_NUM > 0))
    if (pEntry->FFNum != 0u)
#endif
    {
#if (DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CALCULATED)
        /* req SWS_Dem_00479 */
        if (RecordNum == 0xFFu)
        {
#if (DEM_FREEZE_FRAME_MAX_LEN > 0u)
            Dem_MemCopy(pBuffer, pEntry->FFList[pEntry->LatestFF].Data, DEM_FREEZE_FRAME_MAX_LEN);
#endif
            *BufSize = DEM_FREEZE_FRAME_MAX_LEN;
            ret = E_OK;
        }
        else
        {
            ret = E_NOT_OK;
        }
#else /* DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CALCULATED */
#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
        if ((pEntry->EventId != 0u)
            && (DemPbCfgPtr->DemDTC[DemPbCfgPtr->DemEventParameter[Dem_GetEventInternalId(pEntry->EventId)].DemDTCRef]
                    .DemWWHOBDDTCClass
                == DEM_DTC_WWHOBD_CLASS_NOCLASS)
            && (RecordNum == 0u))
        {
            /* req SWS_Dem_01194 */
            ret = DEM_E_WRONG_RECORDNUMBER;
        }
        else
#endif /* DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT */
        {
#if ((DEM_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_PID_CLASS_NUM > 0))
            pFF = Dem_FreezeFrameGetByRecordNum(pEntry, RecordNum);
            if (pFF == NULL_PTR)
            {
                ret = DEM_E_WRONG_RECORDNUMBER;
            }
#endif
#if (DEM_DID_CLASS_NUM > 0)
            else
            {
                if (pEntry->EventId != 0u)
                {
                    pData = pFF->Data;
                    /* DtcRef */
                    Ref = DemPbCfgPtr->DemEventParameter[Dem_GetEventInternalId(pEntry->EventId)].DemDTCRef;
                    /* DTCAttributesRef */
                    Ref = DemPbCfgPtr->DemDTC[Ref].DemDTCAttributesRef;
                    /* FreezeFrameRef */
                    Ref = DemPbCfgPtr->DemDTCAttributes[Ref].DemFreezeFrameClassRef;

                    for (didRefNum = 0; (ret != E_OK) && (didRefNum < DemFreezeFrameClass[Ref].RefNum); didRefNum++)
                    {
                        pDidIndex = DemDidClassRef[DemFreezeFrameClass[Ref].StartIndex + didRefNum];
                        pDid = &DemDidClass[pDidIndex];
                        if (pDid->DemDidIdentifier == Did)
                        {
#if (DEM_DATA_ELEMENT_CLASS_NUM > 0)
                            uint8 offset = 0;
                            uint16 csStart = pDid->StartIndex;
                            for (uint8 csIndex = 0; csIndex < pDid->RefNum; csIndex++)
                            {
                                const DemDataElementClassTypes* pDemDataElementClass =
                                    &DemPbCfgPtr->DemDataElementClass[csStart];
                                if (pDemDataElementClass->DemDataElementReadFnc != NULL_PTR)
                                {
                                    uint8 ElementDataSize = pDemDataElementClass->DemDataElementDataSize;
                                    if (FALSE == pDemDataElementClass->DemInterFlag)
                                    {
                                        Dem_MemCopy(&pBuffer[offset], &pData[offset + cnt], ElementDataSize);
                                    }
                                    else
                                    {
                                        /* get inter data now */
                                        pDemDataElementClass->DemDataElementReadFnc(&pBuffer[offset]);
                                    }
                                    offset += ElementDataSize;
                                }
                                csStart++;
                            }
#endif
                            *BufSize = pDid->DataSize;
                            ret = E_OK;
                        }
                        else
                        {
                            cnt += pDid->DataSize;
                        }
                    }
                }
            }
#endif /* DEM_DID_CLASS_NUM > 0 */
        }
#endif /* DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CALCULATED */
    }
#if ((DEM_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_PID_CLASS_NUM > 0))
    else
#endif
    {
        ret = DEM_E_NODATAAVAILABLE;
    }
    return ret;
}

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
