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

/* PRQA S 6520 EOF */ /* VL_MTR_Dem_STVAR */
/* PRQA S 1520 EOF */ /* VL_Dem_1520 */
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

#if (DEM_EXTENDED_DATA_CLASS_NUM > 0)
DEM_LOCAL FUNC(void, DEM_CODE) Dem_ExtendedDataStorage(
    Dem_EventIdType IntId,
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA) pEntry,
    uint8 Trigger);
#endif

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
static FUNC(void, DEM_CODE) Dem_GetFncData(DemDataElementReadFncType DataReadFnc, uint8* pBuffer, uint8 DataSize);
#endif /* DEM_DATA_ELEMENT_CLASS_NUM > 0u */

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
static FUNC(void, DEM_CODE) Dem_FreezeFrameStorage(
    Dem_EventIdType IntId,
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA) pEntry,
    uint8 Trigger);

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
static FUNC(void, DEM_CODE) Dem_GetGeneralFreezeFrame(uint16 tempRef, uint8* pBuffer);

#if (DEM_SPN_CLASS_NUM > 0u)
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
static FUNC(void, DEM_CODE) Dem_GetJ1939FreezeFrame(uint16 tempRef, P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) pBuffer);
#endif

#if (DEM_PID_CLASS_NUM > 0u)
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
static FUNC(void, DEM_CODE) Dem_GetObdFreezeFrame(P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) pBuffer);
#endif
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
static FUNC(void, DEM_CODE)
    Dem_FreezeFrameCopy(Dem_EventIdType IntId, P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) pFFBuffer);

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
static FUNC(P2VAR(Dem_FreezeFrameInfoType, AUTOMATIC, DEM_APPL_DATA), DEM_CODE)
    Dem_FreezeFrameGetByRecordNum(P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA) pEntry, uint8 RecordNum);
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

static FUNC(Std_ReturnType, DEM_CODE)
    Dem_DTCGetFaultDetectionCounter(Dem_EventIdType EventId, sint8* FaultDetectionCounter);

#if ((DEM_MAX_NUMBER_EVENT_ENTRY_PERMANENT > 0) && (DEM_MALFUNCTION_LAMP_INDICATOR != DemIndicator_INVALID))
/*************************************************************************/
/*
 * Brief               Dem_PermanentEntryFind
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      EventId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint8
 */
/*************************************************************************/
static uint8 Dem_PermanentEntryFind(Dem_EventIdType EventId);

/*************************************************************************/
/*
 * Brief               Dem_PermanentEntryRemove
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      EventId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
static void Dem_PermanentEntryRemove(Dem_EventIdType EventId);
#endif

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
/*************************************************************************/
/*
 * Brief               Gets the number of currently filtered DTCs set by the function
 * Dem_J1939DcmSetDTCFilter ServiceId           0x91 Sync/Async          Asynchronous Reentrancy Non
 * Reentrant Param-Name[in]      None Param-Name[out]     NumberOfFilteredDTC: The number of DTCs
 * matching the defined status mask. Param-Name[in/out]  none Return              Status of the
 * operation to retrieve a number of DTC from the Dem
 */
/*************************************************************************/
FUNC(void, DEM_CODE)
Dem_SubInterJ1939DcmGetNumberOfFilteredDTC(
    Dem_EventIdType IntId,
    uint8           entryId,
    P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA) findSameDTC,
    P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA) IsFind)
{
    const Dem_EventRelateInformationType* pEvent  = Dem_GetEventInfo(IntId);
    Dem_J1939DTCFilterInfoType*           pFilter = &DemJ1939DTCFilter;
    uint8                                 iloop;
    uint16                                dtcRef;
    uint32                                tempDtcValue[DEM_MAX_NUMBER_EVENT_ENTRY_PRIMARY];

    dtcRef                        = DemPbCfgPtr->pDemEventParameter[pEvent->IntId].DemDTCRef;
    uint16       DemObdDTCRef     = DemPbCfgPtr->DemCfgDTC[dtcRef].DemObdDTCRef;
    const uint32 DemJ1939DTCValue = DemPbCfgPtr->ObdDTC[DemObdDTCRef].DemJ1939DTCValue;
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
FUNC(void, DEM_CODE)
Dem_SubGetLampStatusOfJ1939DcmGetNextDTCwithLampStatus(
    P2CONST(Dem_IndicatorAttributeType, AUTOMATIC, DEM_APPL_DATA) pWIRAttr,
    P2CONST(Dem_EventRelateInformationType, AUTOMATIC, DEM_APPL_DATA) pEvent,
    P2VAR(Dem_J1939DcmLampStatusType, AUTOMATIC, DEM_APPL_DATA) LampStatus)
{
#if ((DEM_INDICATOR_NUM > 0) && (DEM_INDICATOR_ATTRIBUTE_TOTAL_NUM > 0))
    uint8 DemIndicatorRef       = pWIRAttr->DemIndicatorRef;
    uint8 DemIndicatorBehaviour = pWIRAttr->DemIndicatorBehaviour;
    uint8 lampStatus            = 0u;
    uint8 flashLampStatus       = (uint8)(*LampStatus);

    /* Amber Warning Lamp */
    /* PRQA S 2986 ++ */ /* VL_Dem_2986 */
    if (DemIndicatorRef == DEM_AMBER_WARING_LAMP_INDICATOR)
    {
        if (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_WIR))
        {
            /* SWS_Dem_00891 */
            lampStatus |= (Dem_J1939LampStatus(DemIndicatorBehaviour) << DEM_J1939_LAMP_AWL_OFFSET);
            flashLampStatus |= (Dem_J1939FlashLampStatus(DemIndicatorBehaviour) << DEM_J1939_LAMP_AWL_OFFSET);
        }
        else
        {
            lampStatus |= DEM_J1939_LAMP_AWL_NA;
            flashLampStatus |= DEM_J1939_FLASHLAMP_AWL_NO_FLASH;
        }
    }

    /* Protect Lamp */
    if (DemIndicatorRef == DEM_PROTECT_LAMP_INDICATOR)
    {
        if (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_WIR))
        {
            /* SWS_Dem_00895 */
            lampStatus |= Dem_J1939LampStatus(DemIndicatorBehaviour);
            flashLampStatus |= (Dem_J1939FlashLampStatus(DemIndicatorBehaviour));
        }
        else
        {
            lampStatus |= DEM_J1939_LAMP_PL_NA;
            flashLampStatus |= DEM_J1939_FLASHLAMP_PL_NO_FLASH;
        }
    }
    *LampStatus = ((uint16)lampStatus << 8u) | flashLampStatus;
    /* PRQA S 2986 -- */
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
static FUNC(Dem_EventStatusType, DEM_CODE) Dem_DebouceProcessPrePassedCounter(
    P2CONST(Dem_DebounceCounterBasedClassType, AUTOMATIC, DEM_CONST) pCfgCounter,
    P2VAR(Dem_DebounceCounterInfoType, AUTOMATIC, DEM_APPL_DATA) pCounter)
{
    Dem_EventStatusType status          = DEM_EVENT_STATUS_PREPASSED;
    sint16              JumpDownValue   = pCfgCounter->DemDebounceCounterJumpDownValue;
    sint16*             InternalCounter = &pCounter->InternalDebounceCounter;
    sint16              PassedThreshold = pCfgCounter->DemDebounceCounterPassedThreshold;
    sint16              StepSize        = (sint16)pCfgCounter->DemDebounceCounterDecrementStepSize;
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
        status           = DEM_EVENT_STATUS_PASSED;
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
static FUNC(Dem_EventStatusType, DEM_CODE) Dem_DebouceProcessPreFailedCounter(
    P2CONST(Dem_DebounceCounterBasedClassType, AUTOMATIC, DEM_CONST) pCfgCounter,
    P2VAR(Dem_DebounceCounterInfoType, AUTOMATIC, DEM_APPL_DATA) pCounter)
{
    Dem_EventStatusType status          = DEM_EVENT_STATUS_PREFAILED;
    sint16              JumpUpValue     = pCfgCounter->DemDebounceCounterJumpUpValue;
    sint16*             InternalCounter = &pCounter->InternalDebounceCounter;
    sint16              FailedThreshold = pCfgCounter->DemDebounceCounterFailedThreshold;
    sint16              StepSize        = (sint16)pCfgCounter->DemDebounceCounterIncrementStepSize;
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
        status           = DEM_EVENT_STATUS_FAILED;
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
FUNC(Dem_EventStatusType, DEM_CODE)
Dem_DebounceProcessCounter(Dem_EventIdType ExtId, Dem_EventStatusType Status)
{
    Dem_EventIdType               IntId     = Dem_GetEventInternalId(ExtId);
    const Dem_EventParameterType* pEventCfg = &DemPbCfgPtr->pDemEventParameter[IntId];
    Dem_EventStatusType           lStatus   = Status;
    if (DEM_DEBOUNCE_COUNTER_BASED == pEventCfg->AlgorithmType)
    {
        const Dem_DebounceCounterBasedClassType* pCfgCounter =
            &DemPbCfgPtr->DebounceCounterBasedClass[DemPbCfgPtr->pDemEventParameter[IntId].AlgorithmRef];
        Dem_DebounceCounterInfoType* pCounter = &DemDebounceCounterInfo[pEventCfg->AlgorithmIndex];

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
            lStatus = Dem_DebouceProcessPrePassedCounter(pCfgCounter, pCounter);
            break;
        default:
            /* DEM_EVENT_STATUS_PREFAILED */
            lStatus = Dem_DebouceProcessPreFailedCounter(pCfgCounter, pCounter);
            break;
        }
    }
    return lStatus;
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
DEM_LOCAL FUNC(Dem_EventStatusType, DEM_CODE) Dem_DebounceProcessTimer(
    Dem_EventDataBufferType pEventBuffer,
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg)
{
    Dem_EventStatusType                  Status = pEventBuffer.Status;
    Dem_DebounceTimeInfoType*            pTimer = &DemDebounceTimerInfo[pEventCfg->AlgorithmIndex];
    const Dem_DebounceTimeBaseClassType* pTimerCfg =
        &DemPbCfgPtr->DebounceTimeBaseClass[DemPbCfgPtr->pDemEventParameter[Dem_GetEventInternalId(pEventBuffer.ExtId)]
                                                .AlgorithmRef];

    uint8                                 chkmask = DEM_EVENT_STATUS_ENABLED_CONDICTION | DEM_EVENT_STATUS_DTC_SETTING;
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
            pTimer->Timeout   = 0;
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
            pTimer->Timeout   = 0;
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
FUNC(uint16, DEM_CODE)
Dem_GetIUMPRDenValueByGroup(Dem_IUMPRGroupType Dem_IUMPRGroup)
{
    uint16 iloop;
    uint16 ret = 0;
    for (iloop = 0; iloop < DEM_RATIO_NUM; iloop++)
    {
        if (DemPbCfgPtr->pDemRatio[iloop].DemIUMPRGroup == Dem_IUMPRGroup)
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
FUNC(uint16, DEM_CODE)
Dem_GetIUMPRNumValueByGroup(Dem_IUMPRGroupType Dem_IUMPRGroup)
{
    uint16 iloop = 0;
    uint16 ret   = 0;
    while (iloop < DEM_RATIO_NUM)
    {
        if (DemPbCfgPtr->pDemRatio[iloop].DemIUMPRGroup == Dem_IUMPRGroup)
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
DEM_LOCAL FUNC(void, DEM_CODE) Dem_UpdateInternalData(uint16 IntId)
{
    const Dem_EventParameterType*         pEventCfg = &DemPbCfg.pDemEventParameter[IntId];
    const Dem_EventRelateInformationType* pEvent    = &DemEventRelateInformation[IntId];
    const Dem_DTCAttributesType*          pDTCAttrCfg;
    const uint8*                          pMemDest;
    uint16                                DTCRef = pEventCfg->DemDTCRef;
    sint8                                 FDC;
    Dem_EventIdType                       EventId = Dem_GetEventExternalId(IntId);

    Dem_UpdateFDC(IntId);
    (void)Dem_DTCGetFaultDetectionCounter(EventId, &FDC);
    /* req SWS_Dem_00471 */
    DemInternalData.Occctr = pEvent->OccurrenceCounter;
    if (DTCRef != DEM_DTC_REF_INVALID)
    {
        pDTCAttrCfg = &DemPbCfg.pDemDTCAttributes[DemPbCfg.DemCfgDTC[DTCRef].DemDTCAttributesRef];

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

        pMemDest                = pDTCAttrCfg->DemMemoryDestinationRef;
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
        const FDCInfoType* pFDCInfo  = &FDCInfo[DTCRef];

        /* req SWS_Dem_00819 */
        DemInternalData.MaxFDCDuringCurrentCycle = pFDCInfo->MaxFDCDuringCurrentCycle;
#if (DEM_MAXFDCSINCELASTCLEAR_EANLE == STD_ON)
        /* req SWS_Dem_00818 */
        DemInternalData.MaxFDCSinceLastClear = pFDCInfo->MaxFDCSinceLastClear;
#endif /* DEM_MAXFDCSINCELASTCLEAR_EANLE == STD_ON */
        /* req SWS_Dem_01084 */
        if (FDC < 0)
        {
            DemInternalData.CurrentFDC = (uint8)(0xFFu + (uint8)FDC + 1u); /* PRQA S 2896 */ /* VL_Dem_2896 */
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
static FUNC(void, DEM_CODE) Dem_DcmGetFreezeFrameDataByDTCProcess(
    P2CONST(uint8, AUTOMATIC, DEM_APPL_DATA) pFFData,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize,
    Dem_EventIdType IntId,
    boolean         isGlobalFF)
{
    uint16                          didRefNum;
    uint8                           csIndex;
    uint8                           dataSize;
    const Dem_DidClassType*         pDid;
    const Dem_DTCAttributesType*    pDTCAttr = Dem_EventDTCAttributesCfg(IntId);
    uint16                          offset   = 0;
    uint16                          cnt;
    uint16                          FFClassRef;
    const Dem_FreezeFrameClassType* pFFClass;
    uint8                           RefNum;
    const uint16*                   pDidIndex;

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
        pFFClass  = &DemFreezeFrameClass[FFClassRef];
        RefNum    = pFFClass->RefNum;
        pDidIndex = &DemDidClassRef[pFFClass->StartIndex];

        DestBuffer[offset] = RefNum;
        offset++;
        (*BufSize) += 1u;
        cnt = 0;
        for (didRefNum = 0u; didRefNum < RefNum; didRefNum++)
        {
            pDid     = &DemDidClass[*pDidIndex];
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
                    &DemPbCfgPtr->pDemDataElementClass[pDid->StartIndex + csIndex];

                if (pDemDataElementClass->DemDataElementReadFnc != NULL_PTR)
                {
                    uint8 ElementDataSize = pDemDataElementClass->DemDataElementDataSize;
                    Dem_MemCopy(&DestBuffer[offset], &pFFData[cnt], ElementDataSize);
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
static FUNC(void, DEM_CODE) Dem_DcmGetSizeOfExtDataRecLoop(
    uint16 dtcRef,
    uint8  memDest,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) SizeOfExtendedDataRecord,
    P2CONST(Dem_ExtendedDataRecordClassType, AUTOMATIC, DEM_CONST) pExtDataCfg)
{
    const Dem_DTCType* pDemDTC = &DemPbCfgPtr->DemCfgDTC[dtcRef];
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
    const Dem_EventIdType EventRefNum = pDemDTC->EventRefNum;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
    const Dem_EventIdType  EventRefStart = pDemDTC->EventRefStart;
    const Dem_EventIdType* pDTCMapping   = &DemPbCfgPtr->DTCMapping[EventRefStart];

#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
    for (uint16 iloop = 0u; iloop < EventRefNum; iloop++)
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
    {
        const Dem_EventMemEntryType* pEntry = Dem_MemEntryGet(Dem_GetEventExternalId(*pDTCMapping), memDest);
        if (pEntry != NULL_PTR)
        {
            *SizeOfExtendedDataRecord += pExtDataCfg->DataSize;
            *SizeOfExtendedDataRecord += 1u;
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
            break;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
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
static FUNC(void, DEM_CODE) Dem_FreezeFrameStorage(
    Dem_EventIdType IntId,
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA) pEntry,
    uint8 Trigger)
{
    Dem_FreezeFrameInfoType*     pFF;
    const Dem_DTCAttributesType* pDTCAttrCfg = Dem_EventDTCAttributesCfg(IntId);
#if (DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)
#if (DEM_FREEZE_FRAME_RECORD_CLASS_NUM > 0u)
    const Dem_FreezeFrameRecordClassType* pFFRecordCfg;
    uint8                                 pFFRecordIndex;
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
#if (DEM_GLOBAL_FREEZE_FRAME_SUPPORT == STD_ON)
    Dem_EventRelateInformationType* pEvent = Dem_GetEventInfo(IntId);
    Dem_GlobalFreezeFrameInfoType*  pGFF;
    pGFF = &(pEntry->GlobalFFList);
    if ((DemGlobalFreezeFrame.DemFreezeFrameRecordTrigger == Trigger)
        && ((DEM_EVENT_STATUS_FDC_TRIGGERED != Trigger)
            || (DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_FDC_TRIGGERED) == 0x00u))
        && (pGFF->RecordNum == 0xFFu))
    {
        uint16 tempRef  = DemGlobalFreezeFrame.DemFreezeFrameClassRef;
        pGFF->RecordNum = DemGlobalFreezeFrame.DemFreezeFrameRecordNumber;
        /* FreezeFrameRef */
        Dem_GetGeneralFreezeFrame(tempRef, pGFF->Data);
    }
#endif

    if ((pDTCAttrCfg != NULL_PTR) && (pDTCAttrCfg->DemFreezeFrameRecNumClassRef != DEM_FREEZE_FRAME_REC_NUM_INVALID))
    {
#if (DEM_FREEZE_FRAME_RECORD_CLASS_NUM > 0u)
        const Dem_FreezeFrameRecNumClassType* pDemFFRecNumClass =
            &DemFreezeFrameRecNumClass[pDTCAttrCfg->DemFreezeFrameRecNumClassRef];
        uint8 RefNum = pDemFFRecNumClass->RefNum;
        for (iloop = 0; iloop < RefNum; iloop++)
        {
            pFFRecordIndex = DemFreezeFrameRecordClassRef[pDemFFRecNumClass->StartIndex + iloop];
            if (pFFRecordIndex != DEM_FREEZE_FRAME_RECORD_INVALID)
            {
                const Dem_EventRelateInformationType* pEvent = Dem_GetEventInfo(IntId);
                pFFRecordCfg                                 = &DemFreezeFrameRecordClass[pFFRecordIndex];
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
                        obdDtcRef =
                            DemPbCfgPtr->DemCfgDTC[DemPbCfgPtr->pDemEventParameter[IntId].DemDTCRef].DemObdDTCRef;
                        if (((obdDtcRef != DEM_OBD_DTC_INVALID)
                             && ((DemPbCfgPtr->ObdDTC[obdDtcRef].DemDtcValue != DEM_OBD_DTC_CFG_INVALID)))
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
            pEntry->FFNum    = 1u;
            pFF              = &pEntry->FFList[0u];
            pEntry->LatestFF = pEntry->FFNum;
            pFF->RecordNum   = pEntry->LatestFF;
            Dem_FreezeFrameCopy(IntId, pFF->Data);
        }
        else
        {
            if (pEntry->FFNum < pDTCAttrCfg->DemMaxNumberFreezeFrameRecords)
            {
                /* req SWS_Dem_00585 */
                pFF              = &pEntry->FFList[pEntry->FFNum];
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
static FUNC(P2VAR(Dem_FreezeFrameInfoType, AUTOMATIC, DEM_APPL_DATA), DEM_CODE)
    Dem_FreezeFrameGetByRecordNum(P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA) pEntry, uint8 RecordNum)
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
            for (uint8 iloop = 0; iloop < DEM_MAX_NUMBER_FF_RECORDS; iloop++)
            {
#endif /* DEM_MAX_NUMBER_FF_RECORDS > 1u */
                if (pFF->RecordNum == RecordNum)
                {
                    ret = pFF;
                }
#if (DEM_MAX_NUMBER_FF_RECORDS > 1u)
                pFF++;
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
FUNC(void, DEM_CODE) Dem_EventQueueProcessWithDebounce(Dem_EventIdType IntId)
{
    const Dem_EventParameterType* pEventCfg      = &DemPbCfgPtr->pDemEventParameter[IntId];
    uint16                        AlgorithmIndex = pEventCfg->AlgorithmIndex;
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
            DemDebounceTimerInfo[AlgorithmIndex].Timeout   = 0u;
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
FUNC(void, DEM_CODE)
Dem_EventTestFailedProcessOfComponent(
#if (DEM_COMPONENT_NUM > 0)
    P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_APPL_DATA) pEvent,
#else
    P2CONST(Dem_EventRelateInformationType, AUTOMATIC, DEM_APPL_DATA) pEvent,
#endif
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg)
{
#if (DEM_COMPONENT_NUM > 0)
    uint16        ComponentID;
    uint16        iloop;
    const uint16* pComponentIndex;

    ComponentID = pEventCfg->DemComponentClassRef;
    /*check whether event ref the component, check the component availability*/
    if ((ComponentID != DEM_COMPONENT_INVALID) && (DemComponentStatus[ComponentID].availability == TRUE))
    {
        uint16                   ExtComponentID = Dem_GetEventExternalId(ComponentID);
        const DemComponentTypes* pComponent     = DemPbCfgPtr->pDemComponent;
        const DemComponentTypes* OldpComponent  = &pComponent[ComponentID];
        DemComponentStatusType*  pComponentStatus;
        if (OldpComponent->DemParentComponentRef != DEM_COMPONENT_INVALID)
        {
            uint16 NewComponentID = ComponentID;
            for (; NewComponentID != DEM_COMPONENT_INVALID;)
            {
                const DemComponentTypes* NewpComponent = &pComponent[NewComponentID];
                pComponentStatus                       = &DemComponentStatus[NewComponentID];
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
                NewComponentID = pComponent[NewComponentID].DemParentComponentRef; /* PRQA S 2469 */ /* VL_Dem_2469 */
            }
            pComponentStatus = &DemComponentStatus[ComponentID];
            if (pComponentStatus->FailurePriority > pEventCfg->DemComponentPriority)
            {
                pComponentStatus->FailurePriority = pEventCfg->DemComponentPriority;
            }
            if (pComponentStatus->Status == FALSE)
            {
                pDemComponentFailedCallbackFnc ComponentFailedFnc = OldpComponent->DemComponentFailedCallbackFnc;
                DemComponentStatus[ComponentID].Status            = TRUE;
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
                    pComponentStatus->Status                          = TRUE;
                    pDemComponentFailedCallbackFnc ComponentFailedFnc = OldpComponent->DemComponentFailedCallbackFnc;
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
             (iloop < DEM_IMMEDITE_CHILD_COMPONENT_MAX_NUM) && (pComponentIndex[iloop] != DEM_COMPONENT_INVALID);

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
FUNC(boolean, DEM_CODE)
Dem_CheckWIROn(
    P2CONST(Dem_EventRelateInformationType, AUTOMATIC, DEM_APPL_DATA) pEvent,
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg)
{
    boolean ret = FALSE;
#if ((DEM_INDICATOR_NUM > 0) && (DEM_INDICATOR_ATTRIBUTE_TOTAL_NUM > 0))
    uint8                             iloop;
    const Dem_IndicatorAttributeType* pWIRAttr;
#if (DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)
    const Dem_DTCType*   pDTCCfg = DemPbCfgPtr->DemCfgDTC;
    const DemObdDTCType* pObdDTC = DemPbCfgPtr->ObdDTC;
#endif /* DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU */
    for (iloop = 0; iloop < pEventCfg->AttrNum; iloop++)
    {
        Dem_EventIdType intId  = pEvent->IntId;
        uint16          Ref    = DemPbCfgPtr->pDemEventParameter[intId].DemOBDGroupingAssociativeEventsRef;
        pWIRAttr               = &DemIndicatorAttribute[pEventCfg->AttrStartIndex + iloop];
        uint8 FailureThreshold = pWIRAttr->DemIndicatorFailureCycleCounterThreshold;
        if ((FailureThreshold <= pEvent->FailureCounter)
            || ((Ref != DEM_EVENT_PARAMETER_INVALID)
                && (FailureThreshold <= DemEventRelateInformation[Ref].FailureCounter)))
        {
#if (DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)
            uint16 DemDTCRef    = DemPbCfgPtr->pDemEventParameter[intId].DemDTCRef;
            uint16 DemObdDTCRef = pDTCCfg[DemDTCRef].DemObdDTCRef;
#endif
            /*reach the threshold activate the mil req SWS_Dem_00967 req SWS_Dem_00501*/
            ret                                     = TRUE;
            DemWIRStatus[pWIRAttr->DemIndicatorRef] = pWIRAttr->DemIndicatorBehaviour;
#if ((DEM_MAX_NUMBER_EVENT_ENTRY_PERMANENT > 0) && (DEM_MALFUNCTION_LAMP_INDICATOR != DemIndicator_INVALID))
            if (pWIRAttr->DemIndicatorRef == DEM_MALFUNCTION_LAMP_INDICATOR)
            {
                Dem_EventIdType eventId = Dem_GetEventExternalId(intId);
                if (Dem_PermanentEntryFind(eventId) == DEM_MAX_NUMBER_EVENT_ENTRY_PERMANENT)
                {
                    uint8 permanentStart = Dem_PermanentEntryFind(0u);
                    if (permanentStart != DEM_MAX_NUMBER_EVENT_ENTRY_PERMANENT)
                    {
                        DemPermanentMemory[permanentStart] = eventId;
                    }
                }
            }
#endif
#if (DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)
            if ((((DemDTCRef != DEM_DTC_REF_INVALID) && (pDTCCfg[DemDTCRef].DemDtcValue < 0xFFFF33UL)
                  && (pDTCCfg[DemDTCRef].DemDtcValue > 0UL))
                 || ((DEM_OBD_DTC_INVALID != DemObdDTCRef)
                     && ((pObdDTC[DemObdDTCRef].DemDtcValue != DEM_OBD_DTC_CFG_INVALID)
                         || (pObdDTC[DemObdDTCRef].DemJ1939DTCValue != DEM_J1939_DTC_CFG_INVALID)))))
            {
                /* req SWS_Dem_01139  req SWS_Dem_00703 */
                Dem_UpdateOBDMilStatus(pWIRAttr->DemIndicatorRef, intId);
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

#if ((DEM_MAX_NUMBER_EVENT_ENTRY_PERMANENT > 0) && (DEM_MALFUNCTION_LAMP_INDICATOR != DemIndicator_INVALID))
/*************************************************************************/
/*
 * Brief               Dem_PermanentEntryFind
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      EventId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint8
 */
/*************************************************************************/
static uint8 Dem_PermanentEntryFind(Dem_EventIdType EventId)
{
    uint8 permanentStart = 0u;
    uint8 permanentlast  = DEM_MAX_NUMBER_EVENT_ENTRY_PERMANENT;

    while (permanentStart < permanentlast)
    {
        if (DemPermanentMemory[permanentStart] != EventId)
        {
            ++permanentStart;
        }
        else
        {
            /** Found */
            break;
        }
    }
    /** if not found, return DEM_MEM_INVALID_MEMORY_INDEX */
    if (permanentStart >= permanentlast)
    {
        permanentStart = DEM_MAX_NUMBER_EVENT_ENTRY_PERMANENT;
    }
    return permanentStart;
}

/*************************************************************************/
/*
 * Brief               Dem_PermanentEntryRemove
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      EventId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
static void Dem_PermanentEntryRemove(Dem_EventIdType EventId)
{
    uint8 permanentStart = Dem_PermanentEntryFind(EventId);
    if (permanentStart != DEM_MAX_NUMBER_EVENT_ENTRY_PERMANENT)
    {
        DemPermanentMemory[permanentStart] = 0u;
    }
}
#endif

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
FUNC(boolean, DEM_CODE)
Dem_CheckWIROff(
    P2CONST(Dem_EventRelateInformationType, AUTOMATIC, DEM_APPL_DATA) pEvent,
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg)
{
    boolean ret = FALSE;
#if ((DEM_INDICATOR_NUM > 0) && (DEM_INDICATOR_ATTRIBUTE_TOTAL_NUM > 0))
    uint8                             iloop;
    const Dem_IndicatorAttributeType* pWIRAttr;
    uint16                            index;
    uint8                             cnt;
    boolean                           Find = FALSE;
#if (DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)
    uint16 DemDTCRef;
    uint16 DemObdDTCRef;
#endif
#if ((DEM_MAX_NUMBER_EVENT_ENTRY_PERMANENT > 0) && (DEM_MALFUNCTION_LAMP_INDICATOR != DemIndicator_INVALID))
    boolean checkFlag = FALSE;
#endif

    for (iloop = 0; iloop < pEventCfg->AttrNum; iloop++)
    {
        pWIRAttr = &DemIndicatorAttribute[pEventCfg->AttrStartIndex + iloop];
        /* req SWS_Dem_00503 */
        if (pWIRAttr->DemIndicatorHealingCycleCounterThreshold <= pEvent->HealingCounter)
        {
            for (index = 0; ((index < DEM_EVENT_PARAMETER_NUM) && (Find == FALSE)); index++)
            {
                for (cnt = 0; ((cnt < DemPbCfgPtr->pDemEventParameter[index].AttrNum) && (Find == FALSE)); cnt++)
                {
                    if ((pWIRAttr->DemIndicatorRef
                         == DemIndicatorAttribute[DemPbCfgPtr->pDemEventParameter[index].AttrStartIndex + cnt]
                                .DemIndicatorRef)
                        && (DemEventRelateInformation[index].HealingCounter
                            < DemIndicatorAttribute[DemPbCfgPtr->pDemEventParameter[index].AttrStartIndex + cnt]
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
                ret                                     = TRUE;
#if ((DEM_MAX_NUMBER_EVENT_ENTRY_PERMANENT > 0) && (DEM_MALFUNCTION_LAMP_INDICATOR != DemIndicator_INVALID))
                if (pWIRAttr->DemIndicatorRef == DEM_MALFUNCTION_LAMP_INDICATOR)
                {
                    checkFlag = TRUE;
                }
#endif
            }
#if (DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)
            DemDTCRef    = DemPbCfgPtr->pDemEventParameter[pEvent->IntId].DemDTCRef;
            DemObdDTCRef = DemPbCfgPtr->DemCfgDTC[DemDTCRef].DemObdDTCRef;
            if ((((DemDTCRef != DEM_DTC_REF_INVALID) && (DemPbCfgPtr->DemCfgDTC[DemDTCRef].DemDtcValue < 0xFFFF33UL)
                  && (DemPbCfgPtr->DemCfgDTC[DemDTCRef].DemDtcValue > 0UL))
                 || ((DEM_OBD_DTC_INVALID != DemObdDTCRef)
                     && ((DemPbCfgPtr->ObdDTC[DemObdDTCRef].DemDtcValue != DEM_OBD_DTC_CFG_INVALID)
                         || (DemPbCfgPtr->ObdDTC[DemObdDTCRef].DemJ1939DTCValue != DEM_J1939_DTC_CFG_INVALID)))))
            {
                /* req SWS_Dem_01139  req SWS_Dem_00703 */
                Dem_UpdateOBDMilStatus(pWIRAttr->DemIndicatorRef, pEvent->IntId);
            }
#endif
        }
    }
#if ((DEM_MAX_NUMBER_EVENT_ENTRY_PERMANENT > 0) && (DEM_MALFUNCTION_LAMP_INDICATOR != DemIndicator_INVALID))
    if (checkFlag == TRUE)
    {
        Dem_PermanentEntryRemove(Dem_GetEventExternalId(pEvent->IntId));
    }
#endif
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
    uint16 dtcRef = DemPbCfgPtr->pDemEventParameter[IntId].DemDTCRef;
#if (DEM_J1939_NODE_NUM > 0)
    uint8  iloop;
    uint8  Nodeindex;
    uint8  NodeNum;
    uint16 ObdDTCRef;
    uint8  NodeRef;
#endif
    Dem_EventRelateInformationType* pEvent = Dem_GetEventInfo(IntId);

    if ((dtcRef != DEM_DTC_REF_INVALID) && (pEvent->OldUdsStatus != pEvent->UdsStatus))
    {
        /* req SWS_Dem_01050 */
        Dem_TriggerOnEventStatus(IntId, pEvent->OldUdsStatus, pEvent->UdsStatus);
#if (DEM_J1939_NODE_NUM > 0)
        const DemObdDTCType* pDemObdDTC = DemPbCfgPtr->ObdDTC;
        ObdDTCRef                       = DemPbCfgPtr->DemCfgDTC[dtcRef].DemObdDTCRef;
        if ((ObdDTCRef != DEM_OBD_DTC_INVALID) && (DEM_J1939_DTC_CFG_INVALID != pDemObdDTC[ObdDTCRef].DemJ1939DTCValue)
            && ((pEvent->OldUdsStatus & DEM_UDS_STATUS_TF)
                != (DemDTCGeneralStatus[dtcRef].CbUdsStatus & DEM_UDS_STATUS_TF)))
        {
            NodeRef = DemPbCfgPtr->pDemDTCAttributes[DemPbCfgPtr->DemCfgDTC[dtcRef].DemDTCAttributesRef]
                          .DemJ1939DTC_J1939NodeRef;
            NodeNum = DemPbCfgPtr->pJ1939Node[NodeRef].NodeNum;
            for (iloop = 0; iloop < NodeNum; iloop++)
            {
                Nodeindex = DemPbCfgPtr->pJ1939Node[NodeRef].NodeIDRef[iloop];
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
FUNC(void, DEM_CODE)
Dem_InterEventTestPassed(Dem_EventIdType IntId)
{
    Dem_EventRelateInformationType* pEvent    = &DemEventRelateInformation[IntId];
    const Dem_EventParameterType*   pEventCfg = &(DemPbCfgPtr->pDemEventParameter[IntId]);

#if (DEM_COMPONENT_NUM > 0)
    uint16  ComponentID            = pEventCfg->DemComponentClassRef;
    boolean allParentComponentpass = TRUE;

    /*check whether event ref the component , check the component availability*/
    if ((ComponentID != DEM_COMPONENT_INVALID) && (DemComponentStatus[ComponentID].availability == TRUE))
    {
        DEM_FLAGS_CLR(pEvent->Status, DEM_EVENT_STATUS_CONSECUTIVE_FAULT);
        const DemComponentTypes* pComponent = DemPbCfgPtr->pDemComponent;
        if (pComponent[ComponentID].DemParentComponentRef != DEM_COMPONENT_INVALID)
        {
            uint16 LComponentID          = ComponentID;
            uint16 DemParentComponentRef = pComponent[LComponentID].DemParentComponentRef;
            while ((DemParentComponentRef != DEM_COMPONENT_INVALID) && (allParentComponentpass == TRUE))
            {
                if (DemComponentStatus[DemParentComponentRef].Status == TRUE)
                {
                    allParentComponentpass = FALSE;
                }
                LComponentID          = DemParentComponentRef; /* next parent component*/
                DemParentComponentRef = pComponent[LComponentID].DemParentComponentRef;
            }
        }
        if (allParentComponentpass == TRUE)
        {
            ComponentID = pEventCfg->DemComponentClassRef;
            /* req SWS_Dem_01125 */
            DemComponentStatus[ComponentID].Status          = FALSE;
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
        boolean                           checkFlag      = TRUE;
        uint8                             AttrNum        = pEventCfg->AttrNum;
        uint16                            AttrStartIndex = pEventCfg->AttrStartIndex;
        for (uint8 iloop = 0; iloop < AttrNum; iloop++)
        {
            pWIRAttr = &DemIndicatorAttribute[AttrStartIndex + iloop];
            if (0x00u != pWIRAttr->DemIndicatorHealingCycleCounterThreshold)
            {
                checkFlag = FALSE;
                break;
            }
        }
        if (TRUE == checkFlag) /* R19-11: SWS_Dem_01233*/
        {
            DEM_FLAGS_CLR(pEvent->UdsStatus, DEM_UDS_STATUS_WIR);
#if ((DEM_MAX_NUMBER_EVENT_ENTRY_PERMANENT > 0) && (DEM_MALFUNCTION_LAMP_INDICATOR != DemIndicator_INVALID))
            checkFlag = FALSE;
#endif
            for (uint8 iloop = 0; iloop < AttrNum; iloop++)
            {
                pWIRAttr                                = &DemIndicatorAttribute[AttrStartIndex + iloop];
                DemWIRStatus[pWIRAttr->DemIndicatorRef] = DEM_INDICATOR_OFF;
#if ((DEM_MAX_NUMBER_EVENT_ENTRY_PERMANENT > 0) && (DEM_MALFUNCTION_LAMP_INDICATOR != DemIndicator_INVALID))
                if (pWIRAttr->DemIndicatorRef == DEM_MALFUNCTION_LAMP_INDICATOR)
                {
                    checkFlag = TRUE;
                }
#endif
            }
#if ((DEM_MAX_NUMBER_EVENT_ENTRY_PERMANENT > 0) && (DEM_MALFUNCTION_LAMP_INDICATOR != DemIndicator_INVALID))
            if (checkFlag == TRUE)
            {
                Dem_PermanentEntryRemove(Dem_GetEventExternalId(IntId));
            }
#endif
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
    Dem_EventRelateInformationType* pEvent  = Dem_GetEventInfo(IntId);
    uint16                          TempRef = DemPbCfgPtr->pDemEventParameter[IntId].DemDTCRef;
    if (TempRef != DEM_DTC_REF_INVALID)
    {
        const uint8* pMemDest =
            DemPbCfgPtr->pDemDTCAttributes[DemPbCfgPtr->DemCfgDTC[TempRef].DemDTCAttributesRef].DemMemoryDestinationRef;
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
                    Dem_MemDestInfoType*         pDemMemDestInfo = &DemMemDestInfo[MemDest];
                    uint8                        EntryNum        = DemMemDestCfg[MemDest].EntryNum;
                    const Dem_MemDestConfigType* pDemMemDestCfg  = &DemMemDestCfg[MemDest];
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
FUNC(void, DEM_CODE) Dem_InterOperationCycleStart(Dem_EventIdType IntId)
{
#if ((DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0) || (DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0))
    uint8  AlgorithmType  = DemPbCfgPtr->pDemEventParameter[IntId].AlgorithmType;
    uint16 AlgorithmIndex = DemPbCfgPtr->pDemEventParameter[IntId].AlgorithmIndex;
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
        DemDebounceTimerInfo[AlgorithmIndex].Timeout   = 0;
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
FUNC(void, DEM_CODE)
Dem_CallDataChangedOfMemEntryAllocate(Dem_EventIdType IntId)
{
#if (DEM_EXTERAL_DATA_ELEMENT_CLASS_NUM > 0)
    /* req SWS_Dem_00475 */
    Dem_CallbackEventDataChangedType CallbackEventDataChanged =
        DemPbCfgPtr->pDemEventParameter[IntId].DemCallbackEventDataChanged;
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
Dem_GetDemWIRStatus(uint8 DemIndicatorRef)
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
void Dem_ClearEventRelateInformation(P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_VAR) pEvent)
{
    pEvent->FailureCounter = 0;
#if (DEM_OCC6EANLE == STD_ON)
    pEvent->OCC6 = 0;
#endif /* DEM_OCC6EANLE == STD_ON */
#if (DEM_OCC4EANLE == STD_ON)
    pEvent->OCC4 = 0;
#endif /* DEM_OCC4EANLE == STD_ON */
    pEvent->OccurrenceCounter = 0;
    pEvent->AgingCounter      = 0;
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
Dem_DeleteRelatedData(P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA) pEntry)
{
    Dem_EventRelateInformationType* pEvent;
#if ((DEM_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_PID_CLASS_NUM > 0))
    pEntry->FFNum    = 0x00;
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
    uint16                 TempRef = DemPbCfgPtr->pDemEventParameter[IntId].DemDTCRef;
    Dem_EventMemEntryType* pEntry;

    if (TempRef != DEM_DTC_REF_INVALID)
    {
        Dem_Pending                           = TRUE;
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
                if ((MemDest != DEM_MEM_DEST_INVALID)
                    && (DEM_BITS_ISNSET(DemMemDestInfo[MemDest].DisableDTCRecordUpdate, TempRef)))
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
FUNC(void, DEM_CODE) Dem_DebounceInit(void)
{
    uint16 iloop;

#if (DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
    for (iloop = 0; iloop < DEM_DEBOUNCE_COUNTER_BASED_EVENT_NUM; iloop++)
    {
        Dem_DebounceCounterInfoType* pDemDebounceCounterInfo = &DemDebounceCounterInfo[iloop];
        pDemDebounceCounterInfo->InternalDebounceCounter     = 0;
    }
#endif
#if (DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
    for (iloop = 0; iloop < DEM_DEBOUNCE_TIME_BASED_EVENT_NUM; iloop++)
    {
        Dem_DebounceTimeInfoType* pDemDebounceTimerInfo = &DemDebounceTimerInfo[iloop];
        pDemDebounceTimerInfo->Timeout                  = 0x00;
        pDemDebounceTimerInfo->Triggered                = FALSE;
        pDemDebounceTimerInfo->CurStatus                = DEM_EVENT_STATUS_PREPASSED;
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
FUNC(void, DEM_CODE) Dem_DebounceReset(Dem_EventIdType IntId)
{
    const Dem_EventParameterType* pEventCfg = &DemPbCfgPtr->pDemEventParameter[IntId];
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
            pCounter                          = &DemDebounceCounterInfo[AlgorithmIndex];
            pCounter->InternalDebounceCounter = 0;
        }
        break;
#endif
#if (DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
    case DEM_DEBOUNCE_TIME_BASE:
        if (AlgorithmIndex < DEM_DEBOUNCE_TIME_BASED_EVENT_NUM)
        {
            pTimer            = &DemDebounceTimerInfo[AlgorithmIndex];
            pTimer->Timeout   = 0;
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
Dem_GetInteralFDC(Dem_EventIdType IntId, P2VAR(sint8, AUTOMATIC, DEM_APPL_DATA) FaultDetectionCounter)
{
    const Dem_EventParameterType* pEventCfg = &DemPbCfgPtr->pDemEventParameter[IntId];
    Std_ReturnType                ret       = E_NOT_OK;
#if (DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
    const Dem_DebounceCounterInfoType*       pCounter;
    const Dem_DebounceCounterBasedClassType* pCfgCounter;
#endif
#if (DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
    const Dem_DebounceTimeInfoType*      pTimer;
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
        if (((DemPbCfgPtr->pDemEventParameter[IntId].AlgorithmRef) < DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM)
            && ((pEventCfg->AlgorithmIndex) < DEM_DEBOUNCE_COUNTER_BASED_EVENT_NUM))
        {
            pCfgCounter = &DemPbCfgPtr->DebounceCounterBasedClass[DemPbCfgPtr->pDemEventParameter[IntId].AlgorithmRef];
            pCounter    = &DemDebounceCounterInfo[pEventCfg->AlgorithmIndex];
            if (pCounter->InternalDebounceCounter > 0)
            {
                IFDC                   = (sint64)pCounter->InternalDebounceCounter * (sint64)127;
                *FaultDetectionCounter = (sint8)(IFDC / (sint32)pCfgCounter->DemDebounceCounterFailedThreshold);
            }
            else
            {
                IFDC                   = (sint64)pCounter->InternalDebounceCounter * ((sint64)(-128));
                *FaultDetectionCounter = (sint8)(IFDC / (sint64)pCfgCounter->DemDebounceCounterPassedThreshold);
            }
            ret = E_OK;
        }
        break;
#endif
#if (DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
    case DEM_DEBOUNCE_TIME_BASE: /*SWS_Dem_00427*/
        if (((DemPbCfgPtr->pDemEventParameter[IntId].AlgorithmRef) < DEM_DEBOUNCE_TIME_BASE_CLASS_NUM)
            && ((pEventCfg->AlgorithmIndex) < DEM_DEBOUNCE_TIME_BASED_EVENT_NUM))
        {
            pTimer    = &DemDebounceTimerInfo[pEventCfg->AlgorithmIndex];
            pTimerCfg = &DemPbCfgPtr->DebounceTimeBaseClass[DemPbCfgPtr->pDemEventParameter[IntId].AlgorithmRef];
            switch (pTimer->CurStatus)
            {
            case DEM_EVENT_STATUS_FAILED:
            case DEM_EVENT_STATUS_PREFAILED:
                IFDC                   = (sint64)pTimer->Timeout * (sint64)127;
                *FaultDetectionCounter = (sint8)(IFDC / (sint64)pTimerCfg->DemDebounceTimeFailedThreshold);
                break;
            default:
                IFDC                   = (sint64)pTimer->Timeout * ((sint64)(-128));
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
        if ((DemPbCfgPtr->pDemEventParameter[IntId].AlgorithmRef) < DEM_DEBOUNCE_MONITOR_INTERNAL_EVENT_NUM)
        {
            GetFnc                 = DemGetFDCFncs[DemPbCfgPtr->pDemEventParameter[IntId].AlgorithmRef];
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
FUNC(Dem_EventStatusType, DEM_CODE)
Dem_DebounceProcess(Dem_EventDataBufferType pEventBuffer)
{
    Dem_EventIdType                 IntId         = Dem_GetEventInternalId(pEventBuffer.ExtId);
    const Dem_EventParameterType*   pEventCfg     = &DemPbCfgPtr->pDemEventParameter[IntId];
    Dem_EventStatusType             Status        = pEventBuffer.Status;
    Dem_EventRelateInformationType* pEvent        = Dem_GetEventInfo(IntId);
    uint8                           AlgorithmType = pEventCfg->AlgorithmType;

#if (DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
    if (AlgorithmType == DEM_DEBOUNCE_TIME_BASE)
    {
        Status = Dem_DebounceProcessTimer(pEventBuffer, pEventCfg);
    }
#endif

#if (DEM_STATUSINDICATOR30ENABLE == STD_ON)
    if ((DEM_EVENT_STATUS_FAILED != Status) && (0x00u != DEM_FLAGS_ISSET(pEvent->StatusIndicator30, DEM_SI30_DTC_AGED)))
    {
        /*bit 6: A DTC is not emission related as long as the FDC10 has not reached the value
         * +127 since DTC information was last cleared and when it is aged*/
        uint16             DemDTCRef = pEventCfg->DemDTCRef;
        const Dem_DTCType* pDemDTC   = &DemPbCfgPtr->DemCfgDTC[DemDTCRef];
        if ((pDemDTC->DTCKind != DEM_DTC_KIND_EMISSION_REL_DTCS))
        {
            DEM_FLAGS_SET(pEvent->StatusIndicator30, DEM_SI30_DTC_ER);
        }
    }
#endif /* DEM_STATUSINDICATOR30ENABLE == STD_ON */

    /* if event failed clear anging counter*/
    pEvent->AgingCounter = (DEM_EVENT_STATUS_FAILED == Status) ? 0u : pEvent->AgingCounter;
    Dem_UpdateFDC(IntId);
#if (DEM_TRIGGERONFDCTHRESHOLDEANLE == STD_ON)
    uint16 DemDTCRef = DemPbCfgPtr->pDemEventParameter[IntId].DemDTCRef;
    /* req SWS_Dem_00786 req SWS_Dem_01068 req SWS_Dem_00811 req SWS_Dem_01069*/
    if ((DemDTCRef != DEM_DTC_REF_INVALID) && (FDCInfo[DemDTCRef].FDC > 0)
        && (DemPbCfgPtr->pDemDTCAttributes[DemPbCfgPtr->DemCfgDTC[DemDTCRef].DemDTCAttributesRef]
                .DemEventMemoryEntryFdcThresholdStorageValue
            <= (uint8)FDCInfo[DemDTCRef].FDC)
        && ((DEM_DEBOUNCE_COUNTER_BASED == AlgorithmType)
#if (DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
            || ((DEM_DEBOUNCE_TIME_BASE == AlgorithmType)
                && (DemDebounceTimerInfo[pEventCfg->AlgorithmIndex].Triggered == FALSE))
#endif
                ))
    {
        /* req SWS_Dem_00799 */
        DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_FDC_TRIGGERED);
        Dem_EventDataStorageTrigger(IntId, DEM_TRIGGER_ON_FDC_THRESHOLD);
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
FUNC(void, DEM_CODE) Dem_DebounceTimerMain(void)
{
#if (DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
    uint16                               DemDTCRef;
    uint16                               iloop;
    Dem_EventIdType                      IntId;
    Dem_EventRelateInformationType*      pEvent;
    const Dem_EventParameterType*        pEventCfg;
    const Dem_DebounceTimeBaseClassType* pTimerCfg;
    Dem_DebounceTimeInfoType*            pTimer = DemDebounceTimerInfo;
#if (DEM_TRIGGER_FIM_REPORTS == STD_ON)
    uint8 OldStatus;
#endif

    for (iloop = 0; (iloop < DEM_DEBOUNCE_TIME_BASED_EVENT_NUM)
                    && (DemPbCfgPtr->pDemEventParameter[pTimer->IntId].AlgorithmRef < DEM_DEBOUNCE_TIME_BASE_CLASS_NUM);
         iloop++)
    {
        pTimer = &DemDebounceTimerInfo[iloop];
        IntId  = pTimer->IntId;
        if ((pTimer->Triggered == TRUE) && (pTimer->IsFreezing == FALSE))
        {
#if (DEM_TRIGGER_FIM_REPORTS == STD_ON)
            OldStatus = pTimer->CurStatus;
#endif
            if ((pTimer->CurStatus != DEM_EVENT_STATUS_FAILED) && (pTimer->CurStatus != DEM_EVENT_STATUS_PASSED))
            {
                pTimer->Timeout += DEM_TASK_TIME;
            }
            pTimerCfg = &DemPbCfgPtr->DebounceTimeBaseClass[DemPbCfgPtr->pDemEventParameter[IntId].AlgorithmRef];
            pEventCfg = &DemPbCfgPtr->pDemEventParameter[IntId];
            if ((pTimer->Timeout >= pTimerCfg->DemDebounceTimeFailedThreshold)
                && ((pTimer->CurStatus == DEM_EVENT_STATUS_PREFAILED)))
            {
                pTimer->CurStatus = DEM_EVENT_STATUS_FAILED;
                pTimer->Triggered = FALSE;
                if ((DEM_BITS_ISSET(DemOperationCycleStatus, pEventCfg->DemOperationCycleRef))
                    || (DemPbCfgPtr->OperationCycle[pEventCfg->DemOperationCycleRef].DemOperationCycleType
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
                    || (DemPbCfgPtr->OperationCycle[pEventCfg->DemOperationCycleRef].DemOperationCycleType
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
            pEventCfg = &DemPbCfgPtr->pDemEventParameter[IntId];
            DemDTCRef = pEventCfg->DemDTCRef;
            pEvent    = Dem_GetEventInfo(IntId);
            if ((DemDTCRef != DEM_DTC_REF_INVALID) && (FDCInfo[DemDTCRef].FDC > 0)
                && (DemPbCfgPtr->pDemDTCAttributes[DemPbCfgPtr->DemCfgDTC[DemDTCRef].DemDTCAttributesRef]
                        .DemEventMemoryEntryFdcThresholdStorageValue
                    <= (uint8)FDCInfo[DemDTCRef].FDC))
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
FUNC(void, DEM_CODE) Dem_UpdateFDC(Dem_EventIdType IntId) /* PRQA S 1505 */ /* VL_Dem_1505 */
{
    const Dem_EventParameterType* pEventCfg = &DemPbCfg.pDemEventParameter[IntId];
#if (DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
    const Dem_DebounceCounterInfoType*       pCounter;
    const Dem_DebounceCounterBasedClassType* pCfgCounter;
#endif /* DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0 */
#if (DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
    const Dem_DebounceTimeInfoType*      pTimer;
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
        if (((DemPbCfg.pDemEventParameter[IntId].AlgorithmRef) < DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM)
            && ((pEventCfg->AlgorithmIndex) < DEM_DEBOUNCE_COUNTER_BASED_EVENT_NUM))
        {
            pCfgCounter = &DemPbCfg.DebounceCounterBasedClass[DemPbCfg.pDemEventParameter[IntId].AlgorithmRef];
            pCounter    = &DemDebounceCounterInfo[pEventCfg->AlgorithmIndex];
            if (pCounter->InternalDebounceCounter == 0)
            {
                FDC = 0;
            }
            else if (pCounter->InternalDebounceCounter > 0)
            {
                IFDC = (sint64)pCounter->InternalDebounceCounter * (sint64)127;
                FDC  = (sint8)(IFDC / (sint64)pCfgCounter->DemDebounceCounterFailedThreshold);
            }
            else
            {
                IFDC = (sint64)pCounter->InternalDebounceCounter * ((sint64)(-128));
                FDC  = (sint8)(IFDC / (sint64)pCfgCounter->DemDebounceCounterPassedThreshold);
            }
        }
        break;
#endif /* DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0 */
#if (DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
    case DEM_DEBOUNCE_TIME_BASE:
        if (((DemPbCfg.pDemEventParameter[IntId].AlgorithmRef) < DEM_DEBOUNCE_TIME_BASE_CLASS_NUM)
            && ((pEventCfg->AlgorithmIndex) < DEM_DEBOUNCE_TIME_BASED_EVENT_NUM))
        {
            pTimer    = &DemDebounceTimerInfo[pEventCfg->AlgorithmIndex];
            pTimerCfg = &DemPbCfg.DebounceTimeBaseClass[DemPbCfg.pDemEventParameter[IntId].AlgorithmRef];
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
                FDC  = (sint8)(IFDC / (sint64)pTimerCfg->DemDebounceTimeFailedThreshold);
                break;
            default:
                IFDC = (sint64)pTimer->Timeout * ((sint64)(-128));
                FDC  = (sint8)(IFDC / (sint64)pTimerCfg->DemDebounceTimePassedThreshold);
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
    Dem_EventIdType        EventRefStart;
    Dem_EventIdType        EventId;
    Std_ReturnType         ret = E_NOT_OK;
    sint8                  Fdc;
    sint8                  MaxFdc = -128;

    EventRefStart = pDemDTC->EventRefStart;
    pDTCMapping   = &DemPbCfg.DTCMapping[EventRefStart];

    for (uint8 loop = 0u; loop < pDemDTC->EventRefNum; loop++)
    {
        EventId = Dem_GetEventExternalId(*pDTCMapping);
        ret     = Dem_GetFaultDetectionCounter(EventId, &Fdc);
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
static FUNC(Std_ReturnType, DEM_CODE)
    Dem_DTCGetFaultDetectionCounter(Dem_EventIdType EventId, sint8* FaultDetectionCounter)
{
    Std_ReturnType ret;

#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
    Dem_EventIdType       IntId       = Dem_GetEventInternalId(EventId);
    uint16                DTCRef      = DemPbCfg.pDemEventParameter[IntId].DemDTCRef;
    const Dem_DTCType*    pDemDTC     = &DemPbCfg.DemCfgDTC[DTCRef];
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
#if (DEM_DEBOUNCE_COUNTER_STORAGE_NUM > 0)
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
FUNC(void, DEM_CODE)
Dem_InterIntWriteNvRAM(
    uint16 iloop,
    P2VAR(Dem_EventRelateInformationStorageType, AUTOMATIC, DEM_APPL_DATA) InformationStorage)
{
    /*SWS_Dem_00674*/
    if (DemPbCfgPtr->DebounceCounterBasedClass[DemPbCfgPtr->pDemEventParameter[iloop].AlgorithmRef]
            .DemDebounceCounterStorage
        == TRUE)
    {
        InformationStorage->DebounceCounter[iloop] =
            (sint16)DemDebounceCounterInfo[DemPbCfgPtr->pDemEventParameter[iloop].AlgorithmIndex]
                .InternalDebounceCounter;
    }
}
#endif

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
FUNC(void, DEM_CODE)
Dem_InterIntReadNvRAM(
    uint16 iloop,
    P2CONST(Dem_EventRelateInformationStorageType, AUTOMATIC, DEM_APPL_DATA) InformationStorage)
{
#if (DEM_DEBOUNCE_COUNTER_STORAGE_NUM > 0)
    /* req SWS_Dem_00674 */
    if (DemPbCfgPtr->DebounceCounterBasedClass[DemPbCfgPtr->pDemEventParameter[iloop].AlgorithmRef]
            .DemDebounceCounterStorage
        == TRUE)
    {
        DemDebounceCounterInfo[DemPbCfgPtr->pDemEventParameter[iloop].AlgorithmIndex].InternalDebounceCounter =
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
FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE)
Dem_InterDcmGetNextFilteredRecord(
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) RecordNumber)
{
    Dem_ReturnGetNextFilteredElementType ret = DEM_FILTERED_NO_MATCHING_ELEMENT;
#if (DEM_FREEZE_FRAME_CLASS_NUM > 0)
    const Dem_EventMemEntryType*         pEntry;
    Dem_FreezeFrameRecordFilterInfoType* pFilter = &DemFreezeFrameRecordFilterInfo;
    Dem_DTCOriginType                    DTCOrigin;

    DTCOrigin = Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY);
    for (; (ret != DEM_FILTERED_OK) && (pFilter->GetNum < pFilter->NumberOfFilteredRecords);)
    {
        /* req SWS_Dem_00210 */
        if (pFilter->EntryIndex >= DemMemDestCfg[DTCOrigin].EntryNum)
        {
            pFilter->EntryIndex = 0;
            pFilter->FFIndex    = 0;
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
                    Dem_EventIdType IntId  = Dem_GetEventInternalId(pEntry->EventId);
                    uint16          DTCRef = DemPbCfgPtr->pDemEventParameter[IntId].DemDTCRef;
                    if ((DTCRef != DEM_DTC_REF_INVALID) && (DemDTCGeneralStatus[DTCRef].SuppressionStatus == FALSE))
                    {
#if (DEM_GLOBAL_FREEZE_FRAME_SUPPORT == STD_ON)
                        if (pEntry->GlobalFFList.RecordNum != 0xFFu)
                        {
                            Dem_GetEventDTC(DTCRef, pFilter->DTCFormat, DTC);
                            *RecordNumber = pEntry->GlobalFFList.RecordNum;
                            ret           = DEM_FILTERED_OK;
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
                                ret           = DEM_FILTERED_OK; /* PRQA S 2469 */ /* VL_Dem_2469 */
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
FUNC(Dem_ReturnSetFilterType, DEM_CODE)
Dem_InterDcmSetFreezeFrameRecordFilter(
    Dem_DTCFormatType DTCFormat,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) NumberOfFilteredRecords)
{
    Dem_ReturnSetFilterType ret;
#if (DEM_FREEZE_FRAME_CLASS_NUM > 0)
    uint8                                entryNum;
    const Dem_EventMemEntryType*         pEntry;
    Dem_FreezeFrameRecordFilterInfoType* pFilter   = &DemFreezeFrameRecordFilterInfo;
    Dem_DTCOriginType                    DTCOrigin = DEM_DTC_ORIGIN_PRIMARY_MEMORY;

    DTCOrigin                = Dem_GetInternalMemDest(DTCOrigin);
    pFilter->DTCFormat       = DTCFormat;
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
    pFilter->GetNum                  = 0;
    pFilter->EntryIndex              = 0;
    pFilter->FFIndex                 = 0;
    pFilter->NumberOfFilteredRecords = *NumberOfFilteredRecords;
    ret                              = DEM_FILTER_ACCEPTED;
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
FUNC(Dem_ReturnGetFreezeFrameDataByDTCType, DEM_CODE)
Dem_InterDcmGetFreezeFrameDataByDTC(
    uint32 DTC,
    uint16 memDestAndRecordNumber,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize)
{
    uint8  memDest;
    uint16 dtcIndex;
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
    uint16 iloop = 0;
#endif
    boolean                               getData = FALSE;
    boolean                               Find    = FALSE;
    Dem_ReturnGetFreezeFrameDataByDTCType ret;
    const uint8*                          pFFData;
    Dem_EventMemEntryType*                pEntry;
    const Dem_FreezeFrameInfoType*        pFF;
    const Dem_DTCAttributesType*          pDTCAttr;
    uint8                                 RecordNumber = (uint8)(memDestAndRecordNumber & 0xFFu);
    boolean                               isGlobalFF   = FALSE;

    dtcIndex                   = Dem_GetDTCIndex(DTC, DEM_DTC_FORMAT_UDS);
    memDest                    = (uint8)(memDestAndRecordNumber >> 8u);
    *BufSize                   = 0u;
    const Dem_DTCType* pDemDTC = &DemPbCfgPtr->DemCfgDTC[dtcIndex];
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
    const Dem_EventIdType EventRefNum = pDemDTC->EventRefNum;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
    const Dem_EventIdType  EventRefStart = pDemDTC->EventRefStart;
    const Dem_EventIdType* pDTCMapping   = &DemPbCfgPtr->DTCMapping[EventRefStart];

#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
    for (; iloop < EventRefNum; iloop++)
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
    {
#if (DEM_GLOBAL_FREEZE_FRAME_SUPPORT == STD_ON)
        if ((RecordNumber == DemGlobalFreezeFrame.DemFreezeFrameRecordNumber) || (RecordNumber == 0xFFu))
        {
            Find       = TRUE;
            isGlobalFF = TRUE;
            pEntry     = Dem_MemEntryGet(Dem_GetEventExternalId(*pDTCMapping), memDest);
            if ((pEntry != NULL_PTR) && (pEntry->GlobalFFList.RecordNum != 0xFFu))
            {
                getData = TRUE;
                pFFData = pEntry->GlobalFFList.Data;
                Dem_DcmGetFreezeFrameDataByDTCProcess(pFFData, &DestBuffer[1], BufSize, *pDTCMapping, isGlobalFF);
                DestBuffer[0] = DemGlobalFreezeFrame.DemFreezeFrameRecordNumber;
                (*BufSize) += 1u;
            }
        }
#endif
        {
            const Dem_DTCAttributesType* pDTCAttrCfg = Dem_EventDTCAttributesCfg(*pDTCMapping);
            uint16                       FFClassRef  = pDTCAttrCfg->DemFreezeFrameClassRef;
            if ((FFClassRef != DEM_FREEZE_FRAME_INVALID)
#if (DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)
                && (DEM_FREEZE_FRAME_REC_NUM_INVALID != pDTCAttrCfg->DemFreezeFrameRecNumClassRef)
#endif
            )
            {
                uint8 record;
#if (DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)
                const Dem_FreezeFrameRecNumClassType* PDemFFRecNumClass =
                    &DemFreezeFrameRecNumClass[pDTCAttrCfg->DemFreezeFrameRecNumClassRef];
                const uint8  RefNum         = PDemFFRecNumClass->RefNum;
                const uint8* pFFRecordIndex = &DemFreezeFrameRecordClassRef[PDemFFRecNumClass->StartIndex];
                for (uint8 index = 0u; index < RefNum; index++)
                {
                    if (((*pFFRecordIndex) != DEM_FREEZE_FRAME_RECORD_INVALID)
                        && ((DemFreezeFrameRecordClass[*pFFRecordIndex].DemFreezeFrameRecordNumber == RecordNumber)
                            || (RecordNumber == 0xFFu)))
                    {
                        record = DemFreezeFrameRecordClass[*pFFRecordIndex].DemFreezeFrameRecordNumber;
                        Find   = TRUE;
#else
                for (uint8 index = 0u; index < DEM_MAX_NUMBER_FF_RECORDS; index++)
                {
                    if ((index == RecordNumber) || (RecordNumber == 0xFFu))
                    {
                        record = index;

#endif
                        pEntry = Dem_MemEntryGet(Dem_GetEventExternalId(*pDTCMapping), memDest);
                        if (pEntry != NULL_PTR)
                        {
#if ((DEM_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_PID_CLASS_NUM > 0))
                            pFF = Dem_FreezeFrameGetByRecordNum(pEntry, record);
                            if (pFF != NULL_PTR)
                            {
                                getData  = TRUE;
                                pFFData  = pFF->Data;
                                pDTCAttr = Dem_EventDTCAttributesCfg(*pDTCMapping);
                                if ((pDTCAttr != NULL_PTR)
                                    && (pDTCAttr->DemFreezeFrameClassRef != DEM_FREEZE_FRAME_INVALID))
                                {
                                    DestBuffer[*BufSize] = record;
                                    Dem_DcmGetFreezeFrameDataByDTCProcess(
                                        pFFData,
                                        &DestBuffer[(*BufSize) + 1u],
                                        BufSize,
                                        *pDTCMapping,
                                        isGlobalFF);
                                    (*BufSize) += 1u;
                                }
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
                                break;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
                            }
#endif
                        }
                    }
#if (DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)
                    pFFRecordIndex++;
#endif
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
FUNC(Dem_ReturnGetSizeOfDataByDTCType, DEM_CODE)
Dem_InterDcmGetSizeOfFreezeFrameByDTC(
    uint32 DTC,
    uint8  RecordNumber,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) SizeOfFreezeFrame)
{
    Dem_ReturnGetSizeOfDataByDTCType ret = DEM_GETSIZEBYDTC_WRONG_DTC;
#if (DEM_FREEZE_FRAME_CLASS_NUM > 0u)
    uint8                        index;
    uint16                       dtcRef;
    const Dem_DTCAttributesType* pDTCAttr;
    /*SWS_Dem_01100 SWS_Dem_01101 */
    dtcRef                     = Dem_GetDTCIndex(DTC, DEM_DTC_FORMAT_UDS);
    const Dem_DTCType* pDemDTC = &DemPbCfgPtr->DemCfgDTC[dtcRef];
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
    const Dem_EventIdType EventRefNum = pDemDTC->EventRefNum;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
    const Dem_EventIdType  EventRefStart = pDemDTC->EventRefStart;
    const Dem_EventIdType* pDTCMapping   = &DemPbCfgPtr->DTCMapping[EventRefStart];
    *SizeOfFreezeFrame                   = 0u;
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
    for (uint16 iloop = 0u; iloop < EventRefNum; iloop++)
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
    {
#if (DEM_GLOBAL_FREEZE_FRAME_SUPPORT == STD_ON)
        uint16 Size = DemFreezeFrameClass[DemGlobalFreezeFrame.DemFreezeFrameClassRef].DataSize;
        if ((DemGlobalFreezeFrame.DemFreezeFrameRecordNumber == RecordNumber) || (RecordNumber == 0xFFu))
        {
            *SizeOfFreezeFrame += Size;
            *SizeOfFreezeFrame += 1u;
        }
#endif
        pDTCAttr = Dem_EventDTCAttributesCfg(*pDTCMapping);
        if (pDTCAttr != NULL_PTR)
        {
            uint16 DataSize = DemFreezeFrameClass[pDTCAttr->DemFreezeFrameClassRef].DataSize;
#if (DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)
            if (DEM_FREEZE_FRAME_REC_NUM_INVALID != pDTCAttr->DemFreezeFrameRecNumClassRef)
#endif
            {
#if (DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)
                const uint8 RefNum     = DemFreezeFrameRecNumClass[pDTCAttr->DemFreezeFrameRecNumClassRef].RefNum;
                uint16      StartIndex = DemFreezeFrameRecNumClass[pDTCAttr->DemFreezeFrameRecNumClassRef].StartIndex;
                for (index = 0; index < RefNum; index++)
#else
                for (index = 0; index < pDTCAttr->DemMaxNumberFreezeFrameRecords; index++)
#endif
                {
#if (DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)
                    uint8 pFFRecordIndex = DemFreezeFrameRecordClassRef[StartIndex + index];
                    if (pFFRecordIndex != DEM_FREEZE_FRAME_RECORD_INVALID)
                    {
                        const Dem_FreezeFrameRecordClassType* pFFRecordCfg = &DemFreezeFrameRecordClass[pFFRecordIndex];
                        if ((pFFRecordCfg->DemFreezeFrameRecordNumber == RecordNumber) || (RecordNumber == 0xFFu))
#else

                    if ((index == RecordNumber) || (RecordNumber == 0xFFu))
#endif
                        {
                            *SizeOfFreezeFrame += DataSize;
                            *SizeOfFreezeFrame += 1u;
                        }
                    }
                    ret = DEM_GETSIZEBYDTC_OK;
                }
            }
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
            break;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
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

    for (iloop = 0; iloop < DEM_EXTENDED_DATA_RECORD_CLASS_NUM; iloop++)
    {
        if ((DemExtendedDataRecordClass[iloop].DemExtendedDataRecordNumber == ExtendedDataNumber)
            || (0xFEu == ExtendedDataNumber) || (0xFFu == ExtendedDataNumber))
        {
            ret = &(DemExtendedDataRecordClass[iloop]);
            break;
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
FUNC(Dem_ReturnGetExtendedDataRecordByDTCType, DEM_CODE)
Dem_GetExtendedRecordDataLoop(
    uint32 DTC,
    uint16 memDestAndExtendedDataNumber,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize)
{
    Dem_ReturnGetExtendedDataRecordByDTCType ret = DEM_RECORD_OK;
#if (DEM_EXTENDED_DATA_CLASS_NUM > 0u)
    uint16                           dtcRef = Dem_GetDTCIndex(DTC, DEM_DTC_FORMAT_UDS);
    uint16                           index;
    boolean                          Find                = FALSE;
    uint8                            memDest             = (uint8)(memDestAndExtendedDataNumber >> 8u);
    uint8                            ExtendedDataNumber  = (uint8)memDestAndExtendedDataNumber;
    const uint16                     DemDTCAttributesRef = DemPbCfgPtr->DemCfgDTC[dtcRef].DemDTCAttributesRef;
    const Dem_DTCAttributesType*     pDTCAttrCfg         = &DemPbCfgPtr->pDemDTCAttributes[DemDTCAttributesRef];
    const Dem_ExtendedDataClassType* pDemExtendedDataClass =
        &DemExtendedDataClass[pDTCAttrCfg->DemExtendedDataClassRef];
    const uint16 StartIndex = pDemExtendedDataClass->StartIndex;
    const uint8* pExtIndex;

    for (index = 0; index < pDemExtendedDataClass->RefNum; index++)
    {
        pExtIndex = &DemExtendedDataRecordClassRef[StartIndex + index];
        if (*pExtIndex != DEM_EXTENDED_DATA_RECORD_INVALID)
        {
            uint8 cfgNumber = DemExtendedDataRecordClass[*pExtIndex].DemExtendedDataRecordNumber;
            if ((cfgNumber == ExtendedDataNumber)
                || ((ExtendedDataNumber == 0xFFu) && (cfgNumber >= 1u) && (cfgNumber <= 0xEFu))
                || ((ExtendedDataNumber == 0xFEu) && (cfgNumber >= 0x90u) && (cfgNumber <= 0xEFu)))
            {
                Find                       = TRUE;
                const Dem_DTCType* pDemDTC = &DemPbCfgPtr->DemCfgDTC[dtcRef];
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
                const Dem_EventIdType EventRefNum = pDemDTC->EventRefNum;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
                const Dem_EventIdType  EventRefStart = pDemDTC->EventRefStart;
                const Dem_EventIdType* pDTCMapping   = &DemPbCfgPtr->DTCMapping[EventRefStart];
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
                for (uint16 iloop = 0u; iloop < EventRefNum; iloop++)
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
                {
                    const Dem_EventMemEntryType* pEntry =
                        Dem_MemEntryGet(Dem_GetEventExternalId(*pDTCMapping), memDest);
                    if (pEntry != NULL_PTR)
                    {
                        ret = Dem_ExtendedDataGetFromEntry(pEntry, cfgNumber, FALSE, DestBuffer, BufSize);
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
                        break;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
                    }
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
                    pDTCMapping++;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
                }
            }
        }
    }
    if (Find == FALSE)
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
FUNC(Dem_ReturnGetSizeOfDataByDTCType, DEM_CODE)
Dem_InterDcmGetSizeOfExtendedDataRecordByDTC(
    uint32            DTC,
    Dem_DTCOriginType DTCOrigin,
    uint8             ExtendedDataNumber,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) SizeOfExtendedDataRecord)
{
    Dem_ReturnGetSizeOfDataByDTCType ret = DEM_GETSIZEBYDTC_WRONG_RECNUM;
#if (DEM_EXTENDED_DATA_CLASS_NUM > 0)
    uint8  memDest;
    uint16 dtcRef;

    dtcRef                    = Dem_GetDTCIndex(DTC, DEM_DTC_FORMAT_UDS);
    memDest                   = Dem_GetInternalMemDest(DTCOrigin);
    *SizeOfExtendedDataRecord = 0;
    uint8 Start;
    uint8 End;
    if (ExtendedDataNumber == 0xFEu)
    {
        Start = 0x90u;
        End   = 0xEFu;
    }
    else if (ExtendedDataNumber == 0xFFu)
    {
        Start = 0x01u;
        End   = 0xEFu;
    }
    else
    {
        Start = ExtendedDataNumber;
        End   = ExtendedDataNumber + 1u;
    }
    for (; Start < End; Start++)
    {
        const Dem_ExtendedDataRecordClassType* pExtDataCfg = Dem_GetExtendedDataRecordCfg(Start);
        if (pExtDataCfg != NULL_PTR)
        {
            Dem_DcmGetSizeOfExtDataRecLoop(dtcRef, memDest, SizeOfExtendedDataRecord, pExtDataCfg);
            ret = DEM_GETSIZEBYDTC_OK;
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
FUNC(Dem_ReturnControlDTCSettingType, DEM_CODE)
Dem_InterDcmDisableDTCSetting(uint32 DTCGroup, Dem_DTCKindType DTCKind)
{
    Dem_ReturnControlDTCSettingType ret;
    boolean                         ProcessOn = TRUE;
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
            ret       = DEM_CONTROL_DTC_WRONG_DTCGROUP;
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
        ret       = DEM_CONTROL_DTC_WRONG_DTCGROUP;
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
FUNC(Dem_ReturnControlDTCSettingType, DEM_CODE)
Dem_InterDcmEnableDTCSetting(uint32 DTCGroup, Dem_DTCKindType DTCKind)
{
    Dem_ReturnControlDTCSettingType ret;
    boolean                         ProcessOn = TRUE;
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
            ret       = DEM_CONTROL_DTC_WRONG_DTCGROUP;
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
        ret       = DEM_CONTROL_DTC_WRONG_DTCGROUP;
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
static FUNC(void, DEM_CODE) Dem_GetFncData(DemDataElementReadFncType DataReadFnc, uint8* pBuffer, uint8 DataSize)
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
static FUNC(void, DEM_CODE) Dem_GetGeneralFreezeFrame(uint16 tempRef, uint8* pBuffer)
{
#if (DEM_DID_CLASS_NUM > 0)
    uint8         offset    = 0;
    uint16        didRefNum = DemFreezeFrameClass[tempRef].RefNum;
    const uint16* pIndex    = &DemDidClassRef[DemFreezeFrameClass[tempRef].StartIndex];

    for (uint16 iloop = 0; iloop < didRefNum; iloop++)
    {
        const Dem_DidClassType* pDid    = &DemDidClass[*pIndex];
        uint8                   csNum   = pDid->RefNum;
        uint16                  csStart = pDid->StartIndex;
        for (uint8 csIndex = 0; csIndex < csNum; csIndex++)
        {
#if (DEM_DATA_ELEMENT_CLASS_NUM > 0)
            uint8                     DataSize    = DemPbCfg.pDemDataElementClass[csStart].DemDataElementDataSize;
            uint8*                    DestBuffer  = &pBuffer[offset];
            DemDataElementReadFncType DataReadFnc = DemPbCfg.pDemDataElementClass[csStart].DemDataElementReadFnc;
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

#if (DEM_SPN_CLASS_NUM > 0u)
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
static FUNC(void, DEM_CODE) Dem_GetJ1939FreezeFrame(uint16 tempRef, P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) pBuffer)
{
    uint8         offset    = 0;
    uint16        SpnRefNum = DemJ1939FreezeFrameClass[tempRef].RefNum;
    const uint16* SpnIndex  = &DemSPNClassRef[DemJ1939FreezeFrameClass[tempRef].StartIndex];

    for (uint16 iloop = 0; iloop < SpnRefNum; iloop++)
    {
        uint16 index = DemSPNClass[*SpnIndex].StartIndex;
#if (DEM_DATA_ELEMENT_CLASS_NUM > 0)
        uint8                     DataSize    = DemPbCfg.pDemDataElementClass[index].DemDataElementDataSize;
        uint8*                    DestBuffer  = &pBuffer[offset];
        DemDataElementReadFncType DataReadFnc = DemPbCfg.pDemDataElementClass[index].DemDataElementReadFnc;
        Dem_GetFncData(DataReadFnc, DestBuffer, DataSize);
        offset += DataSize;
#endif
        SpnIndex++;
    }
}
#endif

#if (DEM_PID_CLASS_NUM > 0u)
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
static FUNC(void, DEM_CODE) Dem_GetObdFreezeFrame(P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) pBuffer)
{
    uint8 offset = 0;

    for (uint16 pPidIndex = 0u; pPidIndex < DEM_PID_CLASS_NUM; pPidIndex++)
    {
        const Dem_PidClassType* pPid      = &DemPbCfg.PidClass[pPidIndex];
        uint16                  pidRefNum = pPid->StartIndex;
        for (uint16 index = 0; index < pPid->RefNum; index++)
        {
#if (DEM_DATA_ELEMENT_CLASS_NUM > 0)
            uint8                     DataSize    = DemPbCfg.pDemDataElementClass[pidRefNum].DemDataElementDataSize;
            uint8*                    DestBuffer  = &pBuffer[offset];
            DemDataElementReadFncType DataReadFnc = DemPbCfg.pDemDataElementClass[pidRefNum].DemDataElementReadFnc;
            Dem_GetFncData(DataReadFnc, DestBuffer, DataSize);
            offset += DataSize;
#endif
            pidRefNum++;
        }
    }
}
#endif

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
FUNC(boolean, DEM_CODE)
Dem_CheckFreezeFrameIsStorage(Dem_EventIdType IntId, uint8 Trigger)
{
    boolean res = FALSE;
#if (                                                                        \
    (DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED) \
    && (DEM_FREEZE_FRAME_RECORD_CLASS_NUM > 0))
    const Dem_DTCAttributesType* pDTCAttrCfg = Dem_EventDTCAttributesCfg(IntId);
    uint8                        pFFRecordIndex;
    uint8                        iloop;
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
static FUNC(void, DEM_CODE) Dem_FreezeFrameCopy(Dem_EventIdType IntId, P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) pFFBuffer)
{
#if (DEM_MAX_NUMBER_PRESTORED_FF > 0)
    Dem_PreStoreFFInfoType* pPreFF = NULL_PTR;
    if (DemPbCfgPtr->pDemEventParameter[IntId].DemFFPrestorageSupported == TRUE)
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
/* PRQA S 1505 ++ */ /* VL_Dem_1505 */
FUNC(void, DEM_CODE)
Dem_GetFreezeFrame(Dem_EventIdType IntId, P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) pBuffer)
/* PRQA S 1505 -- */
{
    uint16       tempRef;
    uint16       DTCAttributesRef;
    uint16       obdDtcRef;
    const uint16 DemDTCRef = DemPbCfg.pDemEventParameter[IntId].DemDTCRef;

    Dem_UpdateInternalData(IntId);
    /* DTCAttributesRef */
    DTCAttributesRef = DemPbCfg.DemCfgDTC[DemDTCRef].DemDTCAttributesRef;
    obdDtcRef        = DemPbCfg.DemCfgDTC[DemDTCRef].DemObdDTCRef;
    /* FreezeFrameRef */
    tempRef = DemPbCfg.pDemDTCAttributes[DTCAttributesRef].DemFreezeFrameClassRef;
    if (tempRef != DEM_FREEZE_FRAME_INVALID)
    {
        Dem_GetGeneralFreezeFrame(tempRef, pBuffer);
    }
    else
    {
        /* WWHOBDFreezeFrameRef */
        tempRef = DemPbCfg.pDemDTCAttributes[DTCAttributesRef].DemWWHOBDFreezeFrameClassRef;
        if (tempRef != DEM_WWHOBD_FREEZE_FRAME_INVALID)
        {
            Dem_GetGeneralFreezeFrame(tempRef, pBuffer);
        }
        else
        {
            /* J1939FreezeFrameRef */
            tempRef = DemPbCfg.pDemDTCAttributes[DTCAttributesRef].DemJ1939FreezeFrameClassRef;
            if ((tempRef != DEM_J1939_FREEZE_FRAME_INVALID) && (obdDtcRef != DEM_OBD_DTC_INVALID)
                && (DemPbCfg.ObdDTC[obdDtcRef].DemJ1939DTCValue != DEM_J1939_DTC_CFG_INVALID))
            {
#if (DEM_SPN_CLASS_NUM > 0u)
                Dem_GetJ1939FreezeFrame(tempRef, pBuffer);
#endif
            }
            else
            {
                /* J1939EXFreezeFrameRef */
                tempRef = DemPbCfg.pDemDTCAttributes[DTCAttributesRef].DemJ1939ExpandedFreezeFrameClassRef;
                if ((tempRef != DEM_J1939_FREEZE_FRAME_INVALID) && (obdDtcRef != DEM_OBD_DTC_INVALID)
                    && (DemPbCfg.ObdDTC[obdDtcRef].DemJ1939DTCValue != DEM_J1939_DTC_CFG_INVALID))
                {
#if (DEM_SPN_CLASS_NUM > 0u)
                    Dem_GetJ1939FreezeFrame(tempRef, pBuffer);
#endif
                }
                else
                {
#if (DEM_PID_CLASS_NUM > 0u)
                    if ((obdDtcRef != DEM_OBD_DTC_INVALID)
                        && (DemPbCfg.ObdDTC[obdDtcRef].DemDtcValue != DEM_OBD_DTC_CFG_INVALID))
                    {
                        /*no other FreezeFrameRef ,take the obd FreezeFrame*/
                        Dem_GetObdFreezeFrame(pBuffer);
                    }
#endif
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
/* PRQA S 6030 ++ */ /* VL_MTR_Dem_STMIF */
FUNC(Std_ReturnType, DEM_CODE)
Dem_ExtendedDataGetFromEntry(
    P2CONST(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA) pEntry,
    uint8   RecordNum,
    boolean ExInterface,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) pBuffer,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize)
/* PRQA S 6030 -- */
{
#if (DEM_EXTENDED_DATA_CLASS_NUM > 0)
    Std_ReturnType               ret         = DEM_E_WRONG_RECORDNUMBER;
    const Dem_DTCAttributesType* pDTCAttrCfg = Dem_EventDTCAttributesCfg(Dem_GetEventInternalId(pEntry->EventId));
    const Dem_ExtendedDataRecordClassType* pExtRecord;
    uint8                                  iloop;
    uint8                                  cnt = 0;
#if (DEM_EXTENDED_DATA_MAX_LEN > 0u)
    uint8        ExtOffset = 0;
    const uint8* pExt      = pEntry->ExtData;
#endif

    if (pDTCAttrCfg != NULL_PTR)
    {
        const uint16 EDClassRef = pDTCAttrCfg->DemExtendedDataClassRef;
        if (DEM_EXTENDED_DATA_INVALID != EDClassRef)
        {
            const Dem_ExtendedDataClassType* pEDClass  = &DemExtendedDataClass[EDClassRef];
            uint8                            RefNum    = pEDClass->RefNum;
            const uint8*                     pExtIndex = &DemExtendedDataRecordClassRef[pEDClass->StartIndex];
            for (iloop = 0; (iloop < RefNum) && (ret == DEM_E_WRONG_RECORDNUMBER); iloop++)
            {
                if (*pExtIndex != DEM_EXTENDED_DATA_RECORD_INVALID)
                {
                    pExtRecord = &DemExtendedDataRecordClass[*pExtIndex];
                    if (pExtRecord->DemExtendedDataRecordNumber == RecordNum)
                    {
                        if (ExInterface == FALSE)
                        {
                            pBuffer[*BufSize] = RecordNum;
                            *BufSize += 1u;
                        }
                        if (DEM_BITS_ISNSET(pEntry->ExtStatus, iloop))
                        {
                            ret = DEM_E_NODATAAVAILABLE;
                        }
                        else
                        {
#if (DEM_DATA_ELEMENT_CLASS_NUM > 0u)
                            uint8  DataElementNum = pExtRecord->DemDataElementClassNum;
                            uint16 offset         = *BufSize;
                            Dem_UpdateInternalData(Dem_GetEventInternalId(pEntry->EventId));
                            for (uint8 index = 0; index < DataElementNum; index++)
                            {
                                const DemDataElementClassTypes* pDemDataElementClass =
                                    &DemPbCfgPtr
                                         ->pDemDataElementClass[pExtRecord->DemDataElementClassIndex + (uint16)index];

                                if (pDemDataElementClass->DemDataElementReadFnc != NULL_PTR)
                                {
                                    uint8 ElementDataSize = pDemDataElementClass->DemDataElementDataSize;
                                    if (FALSE == pDemDataElementClass->DemInterFlag)
                                    {
#if (DEM_EXTENDED_DATA_MAX_LEN > 0u)
                                        Dem_MemCopy(&pBuffer[offset], &pExt[cnt + ExtOffset], ElementDataSize);
                                        ExtOffset += ElementDataSize;
#endif
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
                            *BufSize += pExtRecord->DataSize;
                            ret = E_OK;
                        }
                    }
                    else
                    {
                        uint8 dataSize = 0;
#if (DEM_DATA_ELEMENT_CLASS_NUM > 0u)
                        uint8 DataElementNum = pExtRecord->DemDataElementClassNum;
                        for (uint8 index = 0; index < DataElementNum; index++)
                        {
                            const DemDataElementClassTypes* pDemDataElementClass =
                                &DemPbCfgPtr
                                     ->pDemDataElementClass[pExtRecord->DemDataElementClassIndex + (uint16)index];

                            if (pDemDataElementClass->DemDataElementReadFnc != NULL_PTR)
                            {
                                uint8 ElementDataSize = pDemDataElementClass->DemDataElementDataSize;
                                if (FALSE == pDemDataElementClass->DemInterFlag)
                                {
                                    dataSize += ElementDataSize;
                                }
                            }
                        }
#endif
                        cnt += dataSize;
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
#if (DEM_EXTENDED_DATA_MAX_LEN > 0u)
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
DEM_LOCAL FUNC(void, DEM_CODE) Dem_GetExtendedData(const Dem_ExtendedDataRecordClassType* pExtRecord, uint8* pBuffer)
{
#if (DEM_DATA_ELEMENT_CLASS_NUM > 0u)
    uint16 DataIndex = pExtRecord->DemDataElementClassIndex;
    uint8  cnt       = pExtRecord->DemDataElementClassNum;
    uint8  offset    = 0;

    for (uint16 index = 0; index < cnt; index++)
    {
        const DemDataElementClassTypes* pDataElementClass = &DemPbCfg.pDemDataElementClass[DataIndex];
        if (pDataElementClass->DemInterFlag == FALSE)
        {
            uint8                     DataSize    = pDataElementClass->DemDataElementDataSize;
            uint8*                    DestBuffer  = &pBuffer[offset];
            DemDataElementReadFncType DataReadFnc = pDataElementClass->DemDataElementReadFnc;
            Dem_GetFncData(DataReadFnc, DestBuffer, DataSize);
            offset += DataSize;
        }
        DataIndex++;
    }
#endif /* DEM_DATA_ELEMENT_CLASS_NUM > 0u */
}
#endif

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
FUNC(boolean, DEM_CODE)
Dem_CheckExtendedDataIsStorage(Dem_EventIdType IntId, uint8 Trigger)
{
    boolean                      res         = FALSE;
    const Dem_DTCAttributesType* pDTCAttrCfg = Dem_EventDTCAttributesCfg(IntId);
    uint8                        pExtIndex;
    uint8                        iloop;

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
DEM_LOCAL FUNC(void, DEM_CODE) Dem_ExtendedDataStorage(
    Dem_EventIdType IntId,
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA) pEntry,
    uint8 Trigger)
{
    const Dem_DTCAttributesType*           pDTCAttrCfg = Dem_EventDTCAttributesCfg(IntId);
    const Dem_ExtendedDataRecordClassType* pExtRecord;
    uint8                                  pExtIndex;
    uint8                                  iloop;
    uint8                                  Offset = 0;

    if ((pDTCAttrCfg != NULL_PTR) && (pDTCAttrCfg->DemExtendedDataClassRef != DEM_EXTENDED_DATA_INVALID))
    {
        uint16 DemExtendedDataClassRef = pDTCAttrCfg->DemExtendedDataClassRef;
        Dem_UpdateInternalData(IntId);
        for (iloop = 0; iloop < DemExtendedDataClass[DemExtendedDataClassRef].RefNum; iloop++)
        {
            pExtIndex = DemExtendedDataRecordClassRef[DemExtendedDataClass[DemExtendedDataClassRef].StartIndex + iloop];
            if (pExtIndex != DEM_EXTENDED_DATA_RECORD_INVALID)
            {
                const Dem_EventRelateInformationType* pEvent = Dem_GetEventInfo(IntId);
                pExtRecord                                   = &DemExtendedDataRecordClass[pExtIndex];
                uint8 RecordTrigger                          = pExtRecord->DemExtendedDataRecordTrigger;
                uint8 Update                                 = pExtRecord->DemExtendedDataRecordUpdate;
                if ((RecordTrigger == Trigger)
                    && ((DEM_EVENT_STATUS_FDC_TRIGGERED != Trigger)
                        || (DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_FDC_TRIGGERED) == 0x00u))
                    && ((Update == DEM_UPDATE_RECORD_YES) || (DEM_BITS_ISNSET(pEntry->ExtStatus, iloop))))
                {
                    /* req SWS_Dem_00815  req SWS_Dem_00816 */
                    /*set the corresponding bit*/
                    DEM_BITS_SET(pEntry->ExtStatus, iloop);
#if (DEM_DATA_ELEMENT_CLASS_NUM > 0u)
#if (DEM_EXTENDED_DATA_MAX_LEN > 0u)
                    Dem_GetExtendedData(pExtRecord, &(pEntry->ExtData[Offset]));
#endif
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
                                    DemPbCfgPtr->pDemEventParameter[IntId].DemDTCRef)))
                        {
                            /*set the corresponding bit*/
                            DEM_BITS_SET(pEntry->ExtStatus, iloop);
#if (DEM_DATA_ELEMENT_CLASS_NUM > 0u)
#if (DEM_EXTENDED_DATA_MAX_LEN > 0u)
                            Dem_GetExtendedData(pExtRecord, &(pEntry->ExtData[Offset]));
#endif
#endif /* DEM_DATA_ELEMENT_CLASS_NUM > 0u */
                        }
                    }
                }
#endif /* DEM_MAX_NUMBER_EVENT_ENTRY_MIRROR > 0u */
#endif /* DEM_TRIGGERONMIRROREANLE == STD_ON */
#if (DEM_DATA_ELEMENT_CLASS_NUM > 0u)
#if (DEM_EXTENDED_DATA_MAX_LEN > 0u)
                {
                    uint16 DataIndex = pExtRecord->DemDataElementClassIndex;
                    uint8  cnt       = pExtRecord->DemDataElementClassNum;
                    for (uint16 index = 0; index < cnt; index++)
                    {
                        const DemDataElementClassTypes* pDataElementClass = &DemPbCfg.pDemDataElementClass[DataIndex];
                        if (pDataElementClass->DemInterFlag == FALSE)
                        {
                            uint8 DataSize = pDataElementClass->DemDataElementDataSize;
                            Offset += DataSize;
                        }
                        DataIndex++;
                    }
                }
#endif
#endif /* DEM_DATA_ELEMENT_CLASS_NUM > 0u */
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
/* PRQA S 6030 ++ */ /* VL_MTR_Dem_STMIF */
FUNC(Std_ReturnType, DEM_CODE)
Dem_FreezeFrameGetFromEntry(
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA) pEntry,
    uint8  RecordNum,
    uint16 Did,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) pBuffer,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize)
/* PRQA S 6030 -- */
{
#if ((DEM_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_PID_CLASS_NUM > 0))
    const Dem_FreezeFrameInfoType* pFF;
#endif
#if (DEM_DID_CLASS_NUM > 0)
    uint16                  Ref;
    uint8                   cnt = 0;
    uint16                  didRefNum;
    uint16                  pDidIndex;
    const Dem_DidClassType* pDid;
    const uint8*            pData;
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
            ret      = E_OK;
        }
        else
        {
            ret = E_NOT_OK;
        }
#else /* DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CALCULATED */
#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
        if ((pEntry->EventId != 0u)
            && (DemPbCfgPtr
                    ->DemCfgDTC[DemPbCfgPtr->pDemEventParameter[Dem_GetEventInternalId(pEntry->EventId)].DemDTCRef]
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
                    Ref = DemPbCfgPtr->pDemEventParameter[Dem_GetEventInternalId(pEntry->EventId)].DemDTCRef;
                    /* DTCAttributesRef */
                    Ref = DemPbCfgPtr->DemCfgDTC[Ref].DemDTCAttributesRef;
                    /* FreezeFrameRef */
                    Ref = DemPbCfgPtr->pDemDTCAttributes[Ref].DemFreezeFrameClassRef;

                    for (didRefNum = 0; (ret != E_OK) && (didRefNum < DemFreezeFrameClass[Ref].RefNum); didRefNum++)
                    {
                        pDidIndex = DemDidClassRef[DemFreezeFrameClass[Ref].StartIndex + didRefNum];
                        pDid      = &DemDidClass[pDidIndex];
                        if (pDid->DemDidIdentifier == Did)
                        {
#if (DEM_DATA_ELEMENT_CLASS_NUM > 0)
                            uint8  offset  = 0;
                            uint16 csStart = pDid->StartIndex;
                            for (uint8 csIndex = 0; csIndex < pDid->RefNum; csIndex++)
                            {
                                const DemDataElementClassTypes* lDemDataElementClass =
                                    &DemPbCfgPtr->pDemDataElementClass[csStart];
                                if (lDemDataElementClass->DemDataElementReadFnc != NULL_PTR)
                                {
                                    uint8 ElementDataSize = lDemDataElementClass->DemDataElementDataSize;
                                    Dem_MemCopy(&pBuffer[offset], &pData[offset + cnt], ElementDataSize);
                                    offset += ElementDataSize;
                                }
                                csStart++;
                            }
#endif
                            *BufSize = pDid->DataSize;
                            ret      = E_OK;
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
