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
**  FILENAME    : Dem_Ext.c                                                   **
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
#if (DEM_STORAGE_CONDITION_NUM > 0)
/* Storage Conditions Status Define */
#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
DEM_LOCAL VAR(uint8, AUTOMATIC) DemStorageConditionStatus[DEM_STORAGE_CONDITION_NUM_BYTE];
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_INIT_8
#include "Dem_MemMap.h"
DEM_LOCAL VAR(boolean, AUTOMATIC) Dem_CheckStorageCondition = FALSE;
#define DEM_STOP_SEC_VAR_INIT_8
#include "Dem_MemMap.h"
#endif /* DEM_STORAGE_CONDITION_NUM > 0 */

#if (DEM_ENABLE_CONDITION_NUM > 0)
/* Enable Conditions Status Define */
#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
DEM_LOCAL VAR(uint8, AUTOMATIC) DemEnableConditionStatus[DEM_ENABLE_CONDITION_NUM_BYTE];
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_INIT_8
#include "Dem_MemMap.h"
DEM_LOCAL VAR(boolean, AUTOMATIC) Dem_CheckEnableCondition = FALSE;
#define DEM_STOP_SEC_VAR_INIT_8
#include "Dem_MemMap.h"
#endif /* DEM_ENABLE_CONDITION_NUM > 0 */

#if (DEM_RATIO_NUM > 0)
#define DEM_START_SEC_VAR_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
DEM_LOCAL Dem_IumprDenomCondStatusType ConditionStatu[5] = {3, 3, 3, 3, 3};
#define DEM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
#endif /* DEM_RATIO_NUM > 0 */

#if (DEM_COMPONENT_NUM > 0)
#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
VAR(DemComponentStatusType, AUTOMATIC) DemComponentStatus[DEM_COMPONENT_NUM];
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
#endif

#if (DEM_INDICATOR_NUM > 0)
#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
VAR(Dem_IndicatorStatusType, AUTOMATIC) DemWIRStatus[DEM_INDICATOR_NUM];
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
#endif

#if (DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
/* Debounce Counter Base Info Define */
#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
VAR(Dem_DebounceCounterInfoType, AUTOMATIC)
DemDebounceCounterInfo[DEM_DEBOUNCE_COUNTER_BASED_EVENT_NUM];
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
#endif /* DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0 */
/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/
/* OperationCycleStatus */
#define DEM_START_SEC_VAR_NO_INIT_8
#include "Dem_MemMap.h"
VAR(uint8, AUTOMATIC) DemOperationCycleStatus[DEM_OPERATION_CYCLE_NUM_BYTE];
#define DEM_STOP_SEC_VAR_NO_INIT_8
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
#if (DEM_DTR_NUM > 0)
DTRInfoType DTRInfo[DEM_DTR_NUM]; /* PRQA S 1504 */ /* MISRA Rule 8.7 */
#endif                                              /* DEM_DTR_NUM > 0 */

#if (DEM_RATIO_NUM > 0)
IUMPRType IUMPRValue[DEM_RATIO_NUM];
#endif /* DEM_RATIO_NUM > 0 */
#endif /* DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT */

VAR(Dem_InternalDataType, AUTOMATIC) DemInternalData; /* PRQA S 1533 */ /* MISRA Rule 8.9 */

VAR(Dem_EventQueueType, AUTOMATIC) DemEventQueue;

#if (DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
VAR(Dem_DebounceTimeInfoType, AUTOMATIC)
DemDebounceTimerInfo[DEM_DEBOUNCE_TIME_BASED_EVENT_NUM];
#endif /* DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0 */

VAR(FDCInfoType, AUTOMATIC) FDCInfo[DEM_DTC_NUM];

VAR(Dem_EventRelateInformationType, AUTOMATIC) DemEventRelateInformation[DEM_EVENT_PARAMETER_NUM];

VAR(Dem_DTCGeneralStatusType, AUTOMATIC) DemDTCGeneralStatus[DEM_DTC_NUM];
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

/*******************************************************************************
**                      Private Function Definitions                         **
*******************************************************************************/
#if (DEM_EVENT_COMBINATION_SUPPORT == DEM_EVCOMB_ONSTORAGE)
DEM_LOCAL Dem_EventMemEntryType* Dem_GetEVCOMBEntry(const uint8* pMemDest, uint16 dtcRef)
{
    const Dem_EventParameterType* pDemEventParameter = DemPbCfgPtr->DemEventParameter;
    Dem_EventMemEntryType* pEntry = NULL_PTR;
#if (DEM_MEM_DEST_MAX_NUM_OF_DTC > 1u)
    for (uint8 index = 0; (index < DEM_MEM_DEST_MAX_NUM_OF_DTC) && (pEntry = NULL_PTR); index++)
#else
    uint8 index = 0;
#endif /* DEM_MEM_DEST_MAX_NUM_OF_DTC > 1u */
    {
        const uint8 MemDest = pMemDest[index];
        if (MemDest != DEM_MEM_DEST_INVALID)
        {
            const Dem_EventMemEntryType* Entry = DemMemDestCfg[MemDest].EntryList;
            const uint8 EntryNum = DemMemDestCfg[MemDest].EntryNum;
            for (uint8 iloop = 0; (iloop < EntryNum) && (pEntry == NULL_PTR); iloop++)
            {
                Dem_EventIdType MemEventId = Entry[iloop].EventId;
                if ((MemEventId != 0u) && (pDemEventParameter[Dem_GetEventInternalId(MemEventId)].DemDTCRef == dtcRef))
                {
                    pEntry = Dem_MemEntryGet(MemEventId, MemDest);
                }
            }
        }
    }
    return pEntry;
}
#endif /* DEM_EVENT_COMBINATION_SUPPORT == DEM_EVCOMB_ONSTORAGE */

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
/*************************************************************************/
/*
 * Brief               Sets an enable condition.
 * ServiceId           None
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      EnableConditionID:This parameter identifies the enable condition.
 *                       ConditionFulfilled: This parameter specifies whether the enable condition
 *                       assigned to the EnableConditionID is fulfilled
 *                       (TRUE) or not fulfilled (FALSE).
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              In case the enable condition could be set successfully the API call returns
 * E_OK. If the setting of the enable condition failed the return value of the function is E_NOT_OK.
 */
/*************************************************************************/
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_InterSetEnableCondition(uint8 EnableConditionID, boolean ConditionFulfilled)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_ENABLE_CONDITION_NUM > 0)
    if (EnableConditionID >= DEM_ENABLE_CONDITION_NUM)
    {
        DEM_DET_REPORT(DEM_SID_SETENABLECONDITION, DEM_E_WRONG_CONFIGURATION);
    }
    else
    {
        if (ConditionFulfilled == FALSE)
        {
            DEM_BITS_CLR(DemEnableConditionStatus, EnableConditionID);
        }
        else
        {
            DEM_BITS_SET(DemEnableConditionStatus, EnableConditionID);
        }
        Dem_CheckEnableCondition = TRUE;
        ret = E_OK;
    }
#else
    DEM_UNUSED(EnableConditionID);
    DEM_UNUSED(ConditionFulfilled);
#endif
    return ret;
}

/*************************************************************************/
/*
 * Brief               BRIEF DESCRIPTION
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
FUNC(void, DEM_CODE) Dem_SetEnableCondictionProcess(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (DEM_ENABLE_CONDITION_NUM > 0)
    uint16 iloop = 0;
    Dem_EventRelateInformationType* pEvent;
    boolean IsFulfilled;
    const uint8* pGroup;

    if (Dem_CheckEnableCondition != FALSE)
    {
        Dem_CheckEnableCondition = FALSE;
        while (iloop < DemPbCfgPtr->DemEventNum)
        {
            pEvent = Dem_GetEventInfo(iloop);
            const Dem_EventParameterType* pDemEventParameter = DemPbCfgPtr->DemEventParameter;
            const uint8 DemEnableConditionGroupRef = pDemEventParameter[iloop].DemEnableConditionGroupRef;
            IsFulfilled = TRUE;
            if (DemEnableConditionGroupRef != DEM_ENABLE_CONDITION_GROUP_INVALID)
            {
                pGroup = &DemEnableConditionGroup[DemEnableConditionGroupRef][0];
                IsFulfilled =
                    Dem_CheckCondictionFulfilled(DemEnableConditionStatus, pGroup, DEM_ENABLE_CONDITION_NUM_BYTE);
            }
            if (IsFulfilled == TRUE)
            {
                if (0x00u == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_ENABLED_CONDICTION))
                {
                    DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_ENABLED_CONDICTION);
                    /* req SWS_Dem_00681 Monitor re-initialization  */
                    const Dem_CallbackInitMForEType InitMForEFnc = pDemEventParameter[iloop].DemCallbackInitMForE;
                    if (InitMForEFnc != NULL_PTR)
                    {
                        (void)InitMForEFnc(DEM_INIT_MONITOR_REENABLED);
                    }
                }
            }
            else
            {
                DEM_FLAGS_CLR(pEvent->Status, DEM_EVENT_STATUS_ENABLED_CONDICTION);
                /* req SWS_Dem_00655 */
                Dem_DebounceFreeze(iloop);
            }
            iloop++;
        }
    }
#endif
}

/*************************************************************************/
/*
 * Brief               Sets a storage condition.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      StorageConditionID: This parameter identifies the storage condition.
 *                       ConditionFulfilled: This parameter specifies whether the storage condition
 *                       assigned to the StorageConditionID is fulfilled (TRUE) or not
 *                       fulfilled (FALSE).
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              In case the storage condition could be set successfully the API call
 *                      returns E_OK. If the setting of the storage condition failed the
 *                      return value of the function is E_NOT_OK.
 */
/*************************************************************************/
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_InterSetStorageCondition(uint8 StorageConditionID, boolean ConditionFulfilled)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_STORAGE_CONDITION_NUM > 0)
    if (StorageConditionID >= DEM_STORAGE_CONDITION_NUM)
    {
        DEM_DET_REPORT(DEM_SID_SETSTORAGECONDITION, DEM_E_WRONG_CONFIGURATION);
    }
    else
    {
        if (ConditionFulfilled == FALSE)
        {
            DEM_BITS_CLR(DemStorageConditionStatus, StorageConditionID);
        }
        else
        {
            DEM_BITS_SET(DemStorageConditionStatus, StorageConditionID);
        }
        Dem_CheckStorageCondition = TRUE;
        ret = E_OK;
    }
#else
    DEM_UNUSED(StorageConditionID);
    DEM_UNUSED(ConditionFulfilled);
#endif
    return ret;
}

/*************************************************************************/
/*
 * Brief               Dem_SetStorageCondictionProcess
 * ServiceId           --
 * Sync/Async          Synchronous-
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
FUNC(void, DEM_CODE) Dem_SetStorageCondictionProcess(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (DEM_STORAGE_CONDITION_NUM > 0)
    uint16 iloop = 0;
    Dem_EventRelateInformationType* pEvent;
    boolean IsFulfilled;
    boolean IsReplacement;
    const uint8* pGroup;
    uint8 index;
    uint8 Conditionindex;
    uint8 Counter;
    uint8 Mask;
    if (Dem_CheckStorageCondition != FALSE)
    {
        Dem_CheckStorageCondition = FALSE;
        while (iloop < DemPbCfgPtr->DemEventNum)
        {
            pEvent = Dem_GetEventInfo(iloop);
            IsReplacement = FALSE;
            IsFulfilled = TRUE;
            const Dem_EventParameterType* pDemEventParameter = DemPbCfgPtr->DemEventParameter;
            const uint8 GroupRef = pDemEventParameter[iloop].DemStorageConditionGroupRef;
            if (GroupRef != DEM_STORAGE_CONDITION_GROUP_INVALID)
            {
                pGroup = &DemStorageConditionGroup[GroupRef][0];
                IsFulfilled =
                    Dem_CheckCondictionFulfilled(DemStorageConditionStatus, pGroup, DEM_STORAGE_CONDITION_NUM_BYTE);
            }
            if (IsFulfilled == TRUE)
            {
                if (0x00u == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_STORAGE_CONDICTION))
                {
                    DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_STORAGE_CONDICTION);
                    /*SWS_Dem_01089 */
                    index = 0;
                    Conditionindex = 0;
                    /* PRQA S 2877++ */ /* MISRA Dir 4.1 */
                    while ((index < DEM_STORAGE_CONDITION_NUM_BYTE) && (IsReplacement == FALSE))
                    /* PRQA S 2877-- */ /* MISRA Dir 4.1 */
                    {
                        Counter = 0;
                        Mask = 0x01;
                        /* PRQA S 2877++ */ /* MISRA Dir 4.1 */
                        while ((Conditionindex < DEM_STORAGE_CONDITION_NUM) && (IsReplacement == FALSE)
                               && (Counter < 8u))
                        /* PRQA S 2877-- */ /* MISRA Dir 4.1 */
                        {
                            const uint16 EventRef =
                                DemStorageCondition[Conditionindex].DemStorageConditionReplacementEventRef;
                            if (((DemStorageConditionGroup[pDemEventParameter[iloop].DemStorageConditionGroupRef][index]
                                  & Mask)
                                 != 0u)
                                && (EventRef != DEM_EVENT_PARAMETER_INVALID))
                            {
                                const Dem_CallbackInitMForEType InitMForE =
                                    pDemEventParameter[iloop].DemCallbackInitMForE;
                                (void)Dem_EventQueueAdd(Dem_GetEventExternalId(EventRef), DEM_EVENT_STATUS_PASSED);
                                /* req SWS_Dem_01113  Monitor re-initialization  */
                                if (InitMForE != NULL_PTR)
                                {
                                    (void)InitMForE(DEM_INIT_MONITOR_STORAGE_REENABLED);
                                }
                                IsReplacement = TRUE; /* PRQA S 2982 */ /* MISRA Rule 2.2 */
                            }
                            Counter++; /* PRQA S 2982 */ /* MISRA Rule 2.2 */
                            Conditionindex++;
                            Mask = Mask << 1u; /* PRQA S 2982 */ /* MISRA Rule 2.2 */
                        }
                        index++;
                    }
                }
            }
            else
            {
                DEM_FLAGS_CLR(pEvent->Status, DEM_EVENT_STATUS_STORAGE_CONDICTION);
            }
            iloop++;
        }
    }
#endif
}

/******************************************************************************/
/*
 * Brief               Initializes the internal states necessary to process events reported by
 * BSW-modules.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return None
 */
/******************************************************************************/
FUNC(void, DEM_CODE) Dem_InterPreInit(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint16 iloop;
    Dem_EventQueueType* pQueue = &DemEventQueue;
    Dem_EventDataBufferType* pBuffer;
    Dem_EventRelateInformationType* pEvent; /* PRQA S 3678 */ /* MISRA Rule 8.13 */

    /* OperationCycleStatus */
    Dem_MemSet(DemOperationCycleStatus, 0x00u, DEM_OPERATION_CYCLE_NUM_BYTE);
#if (DEM_ENABLE_CONDITION_NUM > 0)
    /* Init Enable Conditions */
    Dem_MemCopy(DemEnableConditionStatus, DemEnableCondition, DEM_ENABLE_CONDITION_NUM_BYTE);
#endif
#if (DEM_STORAGE_CONDITION_NUM > 0)
    /* Init Storage Conditions */
    Dem_MemSet(DemStorageConditionStatus, 0x00u, DEM_STORAGE_CONDITION_NUM_BYTE);
    for (iloop = 0; iloop < DEM_STORAGE_CONDITION_NUM; iloop++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
    {
        if (DemStorageCondition[iloop].DemStorageConditionStatus == TRUE)
        {
            DEM_BITS_SET(DemStorageConditionStatus, iloop);
        }
    }
#endif
    /* req SWS_Dem_00180 */
    for (iloop = 0; iloop < DEM_EVENT_PARAMETER_NUM; iloop++)
    {
        pEvent = &DemEventRelateInformation[iloop];
        Dem_MemSet((uint8*)pEvent, 0x00u, sizeof(Dem_EventRelateInformationType));
    }
#if ((DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT) || (DEM_J1939_SUPPORT == STD_ON))
#if (DEM_RATIO_NUM > 0)
#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
    IgnUpCycleCounter = 0;
#endif
    for (iloop = 0; iloop < DEM_RATIO_NUM; iloop++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
    {
        IUMPRValue[iloop].IUMPRDenStatus = TRUE;
        IUMPRValue[iloop].IUMPRNumStatus = FALSE;
        IUMPRValue[iloop].Denominator = 0;
        IUMPRValue[iloop].Numerator = 0;
        IUMPRValue[iloop].General_Denominator = 0;
    }
    ConditionStatu[0] = 3;
    ConditionStatu[1] = 3;
    ConditionStatu[2] = 3;
    ConditionStatu[3] = 3;
    ConditionStatu[4] = 3;
#endif
#if (DEM_DTR_NUM > 0)
    Dem_MemSet((uint8*)DTRInfo, 0u, (sizeof(DTRInfoType) * DEM_DTR_NUM));
#endif
#endif
    /* req SWS_Dem_00167*/
    pQueue->ReadIndex = 0x00;
    pQueue->WriteIndex = 0x00;
    for (iloop = 0; iloop < DEM_EVENT_QUEUE_SIZE; iloop++)
    {
        pBuffer = &pQueue->Queue[iloop];
        pBuffer->ExtId = 0x00;
        pBuffer->Status = 0x00;
    }
}

/*************************************************************************/
/*
 * Brief               Initializes or reinitializes this module.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ConfigPtr:Pointer to the configuration set in VARIANT-POSTBUILD.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
FUNC(void, DEM_CODE) Dem_InterInit(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint16 iloop;
    Dem_EventRelateInformationType* pEvent;
#if (DEM_MAX_NUMBER_PRESTORED_FF > 0u)
    Dem_PreStoreFFInfoType* pPreFF;
#endif

#if (DEM_GROUP_OF_DTC_NUM > 0u)
    Dem_MemSet((uint8*)&DemDTCSettingInfo.DTCGroupStatus, 0xFFu, DEM_GROUP_OF_DTC_NUM_BYTE);
#endif
#if (DEM_INDICATOR_NUM > 0u)
    Dem_MemSet(DemWIRStatus, DEM_INDICATOR_OFF, DEM_INDICATOR_NUM);
#endif
    for (iloop = 0u; iloop < DemPbCfgPtr->DemEventNum; iloop++)
    {
#if ((DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0u) || (DEM_ENABLE_CONDITION_NUM > 0u) || (DEM_STORAGE_CONDITION_NUM > 0u))
        const Dem_EventParameterType* pEventCfg = &DemPbCfgPtr->DemEventParameter[iloop];
#endif
        pEvent = &DemEventRelateInformation[iloop];
#if (DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0u)
        if ((pEventCfg->AlgorithmType == DEM_DEBOUNCE_TIME_BASE)
            && (pEventCfg->AlgorithmIndex < DEM_DEBOUNCE_TIME_BASED_EVENT_NUM))
        {
            DemDebounceTimerInfo[pEventCfg->AlgorithmIndex].IntId = iloop;
        }
#endif
#if (DEM_ENABLE_CONDITION_NUM > 0u)
        uint8 DemEnableConditionGroupRef = pEventCfg->DemEnableConditionGroupRef;
        if (DemEnableConditionGroupRef != DEM_ENABLE_CONDITION_GROUP_INVALID)
        {
            if (Dem_CheckCondictionFulfilled(
                    DemEnableConditionStatus,
                    &DemEnableConditionGroup[DemEnableConditionGroupRef][0],
                    DEM_ENABLE_CONDITION_NUM_BYTE)
                == TRUE)
            {
                DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_ENABLED_CONDICTION);
            }
        }
        else
#endif
        {
            DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_ENABLED_CONDICTION);
        }
#if (DEM_STORAGE_CONDITION_NUM > 0u)
        uint8 DemStorageConditionGroupRef = pEventCfg->DemStorageConditionGroupRef;
        if (DemStorageConditionGroupRef != DEM_STORAGE_CONDITION_GROUP_INVALID)
        {
            if (Dem_CheckCondictionFulfilled(
                    DemStorageConditionStatus,
                    &DemStorageConditionGroup[DemStorageConditionGroupRef][0],
                    DEM_STORAGE_CONDITION_NUM_BYTE)
                == TRUE)
            {
                DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_STORAGE_CONDICTION);
            }
        }
        else
#endif
        {
            DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_STORAGE_CONDICTION);
        }
    }
#if (DEM_COMPONENT_NUM > 0u)
    for (iloop = 0u; iloop < DEM_COMPONENT_NUM; iloop++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
    {
        DemComponentStatus[iloop].FailurePriority = 0xFFu;
        DemComponentStatus[iloop].Status = FALSE;
        DemComponentStatus[iloop].availability = TRUE;
    }
#endif
#if (DEM_MAX_NUMBER_PRESTORED_FF > 0u)
    for (iloop = 0u; iloop < DEM_MAX_NUMBER_PRESTORED_FF; iloop++)
    {
        pPreFF = &DemPreStoreFFInfo[iloop];
        pPreFF->ExtId = 0x00u;
#if (DEM_FREEZE_FRAME_MAX_LEN > 0u)
        Dem_MemSet(pPreFF->FFData, 0xFFu, DEM_FREEZE_FRAME_MAX_LEN);
#endif
    }
#endif
}

/*************************************************************************/
/*
 * Brief               Processes the events reported by SW-Cs via RTE.
 * ServiceId           None
 * Sync/Async          Synchronous/Asynchronous
 * Reentrancy          Reentrant for different EventIds.
 *                       Non reentrant for the same EventId.
 * Param-Name[in]      EventId:Identification of an event by assigned EventId.
 *                       EventStatus: Monitor test result
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: set of event status was successful
 *                       E_NOT_OK: set of event status failed or could not be accepted
 */
/*************************************************************************/
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_InterSetEventStatus(Dem_EventIdType* IntId, Dem_EventStatusType EventStatus)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
#if (DEM_STORAGE_CONDITION_NUM > 0)
    const Dem_EventRelateInformationType* pEvent = Dem_GetEventInfo(*IntId);
    Std_ReturnType ret = E_OK;
    uint8 index;
    uint8 Conditionindex;
    uint8 Counter;
    uint8 Mask;
    boolean IsReplacement = FALSE;
    const uint8 DemStorageConditionGroupRef = DemPbCfgPtr->DemEventParameter[*IntId].DemStorageConditionGroupRef;

    if (0x00u == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_STORAGE_CONDICTION))
    {
        /* PRQA S 2877++ */ /* MISRA Dir 4.1 */
        for (index = 0; (index < DEM_STORAGE_CONDITION_NUM_BYTE) && (IsReplacement == FALSE); index++)
        /* PRQA S 2877-- */ /* MISRA Dir 4.1 */
        {
            Counter = 0;
            Mask = 0x01;
            /* PRQA S 2877++ */ /* MISRA Dir 4.1 */
            for (Conditionindex = 0;
                 (Conditionindex < DEM_STORAGE_CONDITION_NUM) && (IsReplacement == FALSE) && (Counter < 8u);
                 Conditionindex++)
            /* PRQA S 2877-- */ /* MISRA Dir 4.1 */
            {
                /*req SWS_Dem_01086 */
                if (((DemStorageConditionGroup[DemStorageConditionGroupRef][index] & Mask) != 0u)
                    && (DemStorageCondition[Conditionindex].DemStorageConditionReplacementEventRef
                        != DEM_EVENT_PARAMETER_INVALID)
                    && (EventStatus == DEM_EVENT_STATUS_FAILED))
                {
                    *IntId = DemStorageCondition[Conditionindex].DemStorageConditionReplacementEventRef;
                    /* req SWS_Dem_01086 */
                    IsReplacement = TRUE;
                }
                Counter++; /* PRQA S 2983 */         /* MISRA Rule 2.2 */
                Mask = Mask << 1u; /* PRQA S 2983 */ /* MISRA Rule 2.2 */
            }
        }
        if (IsReplacement == FALSE)
        {
            ret = E_NOT_OK;
        }
    }
    return ret;
#else
    DEM_UNUSED(IntId);
    DEM_UNUSED(EventStatus);
    return E_OK;
#endif
}

/*************************************************************************/
/*
 * Brief               Control the internal debounce counter/timer by BSW modules and SW-Cs.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different EventIds.
 *                       Non reentrant for the same EventId.
 * Param-Name[in]      EventId:Identification of an event by assigned EventId.
 *                       DebounceResetStatus :  Freeze or reset the internal debounce counter
 *                       /timer of the specified event.
 * Param-Name[out]     None...
 * Param-Name[in/out]  None...
 * Return              None...
 */
/*************************************************************************/
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(void, DEM_CODE)
Dem_InterResetEventDebounceStatus(Dem_EventIdType EventId, Dem_DebounceResetStatusType DebounceResetStatus)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    const uint8 AlgorithmType = DemPbCfgPtr->DemEventParameter[EventId].AlgorithmType;
    const uint16 AlgorithmIndex = DemPbCfgPtr->DemEventParameter[EventId].AlgorithmIndex;
    if (DebounceResetStatus == DEM_DEBOUNCE_STATUS_RESET)
    {
        /* req SWS_Dem_00684*/
        if (AlgorithmType == DEM_DEBOUNCE_COUNTER_BASED)
        {
#if (DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
            if (AlgorithmIndex < DEM_DEBOUNCE_COUNTER_BASED_EVENT_NUM)
            {
                Dem_DebounceCounterInfoType* pDebounceCounter = &DemDebounceCounterInfo[AlgorithmIndex];
                pDebounceCounter->InternalDebounceCounter = 0;
            }
#endif
        }
        else if (AlgorithmType == DEM_DEBOUNCE_TIME_BASE)
        {
#if (DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
            if (AlgorithmIndex < DEM_DEBOUNCE_TIME_BASED_EVENT_NUM)
            {
                Dem_DebounceTimeInfoType* pDebounceTimer = &DemDebounceTimerInfo[AlgorithmIndex];
                pDebounceTimer->Timeout = 0u;
                pDebounceTimer->Triggered = FALSE;
                pDebounceTimer->CurStatus = DEM_EVENT_STATUS_PREPASSED;
            }
#endif
        }
        else
        {
            /*idle*/
        }
    }
    else if (DebounceResetStatus == DEM_DEBOUNCE_STATUS_FREEZE)
    {
        /* req SWS_Dem_00685 */
#if (DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
        if (AlgorithmIndex < DEM_DEBOUNCE_TIME_BASED_EVENT_NUM)
        {
            DemDebounceTimerInfo[AlgorithmIndex].IsFreezing = TRUE;
        }
#endif
    }
    else
    {
        /*idle*/
    }
}

/*************************************************************************/
/*
 * Brief               Captures the freeze frame data for a specific event.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different EventIds.
 *                       Non reentrant for the sameEventId.
 * Param-Name[in]      EventId:Identification of an event by assigned EventId
 * Param-Name[out]     None...
 * Param-Name[in/out]  None...
 * Return              None...
 */
/*************************************************************************/
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(Std_ReturnType, DEM_CODE) Dem_InterPrestoreFreezeFrame(Dem_EventIdType IntId)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
#if (DEM_FFPRESTORAGE_SUPPORT == STD_ON)
    Std_ReturnType ret = E_OK;
    const Dem_EventRelateInformationType* pEvent;
    Dem_PreStoreFFInfoType* pPreFF;

    pEvent = Dem_GetEventInfo(IntId);
    /* req SWS_Dem_01108*/
    if (0x00u != DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_AVAILABLE))
    {
        pPreFF = Dem_PreStoreFFGet(Dem_GetEventExternalId(IntId));
        if (pPreFF == NULL_PTR)
        {
            pPreFF = Dem_PreStoreFFGet(0x00);
            if (pPreFF == NULL_PTR)
            {
                /* req SWS_Dem_00808 */
                ret = E_NOT_OK;
            }
            else
            {
                pPreFF->ExtId = Dem_GetEventExternalId(IntId);
            }
        }
        /* req SWS_Dem_00807 */
        if (ret == E_OK)
        {
            Dem_GetFreezeFrame(IntId, pPreFF->FFData);
        }
    }
    return ret;
#else
    DEM_UNUSED(IntId);
    return E_NOT_OK;
#endif
}

/*************************************************************************/
/*
 * Brief               Clears a prestored freeze frame of a specific event.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different EventIds.
 *                       Non reentrant for the same EventId.
 * Param-Name[in]      EventId : Identification of an event by assigned EventId.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_InterClearPrestoredFreezeFrame(Dem_EventIdType IntId)
{
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_FFPRESTORAGE_SUPPORT == STD_ON)
    Dem_PreStoreFFInfoType* pPreFF = Dem_PreStoreFFGet(Dem_GetEventExternalId(IntId));
    if (pPreFF != NULL_PTR)
    {
        pPreFF->ExtId = 0x00;
#if (DEM_FREEZE_FRAME_MAX_LEN > 0u)
        Dem_MemSet(pPreFF->FFData, 0xFFu, DEM_FREEZE_FRAME_MAX_LEN);
#endif
        ret = E_OK;
    }
    return ret;
#else
    DEM_UNUSED(IntId);
    return ret;
#endif
}

/*************************************************************************/
/*
 * Brief               Gets the failed status of a DemComponent.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ComponentId:Identification of a DemComponent
 * Param-Name[out]     ComponentFailed: TRUE: failed FALSE: not failed
 * Param-Name[in/out]  None
 * Return              E_OK: getting "ComponentFailed" was successful
 *                       E_NOT_OK: getting "ComponentFailed" was not successful
 */
/*************************************************************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_InterGetComponentFailed(
    Dem_ComponentIdType ComponentId,
#if (DEM_COMPONENT_NUM > 0)
    P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA) ComponentFailed
#else
    P2CONST(boolean, AUTOMATIC, DEM_APPL_DATA) ComponentFailed
#endif
)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_COMPONENT_NUM > 0)
    Dem_ComponentIdType ComponentIntId;
#endif

    if (ComponentId > DEM_COMPONENT_NUM)
    {
        DEM_DET_REPORT(DEM_SID_GETCOMPONENTFAILED, DEM_E_WRONG_RECORDNUMBER);
    }
    else if (ComponentFailed == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_GETCOMPONENTFAILED, DEM_E_PARAM_POINTER);
    }
    else
    {
#if (DEM_COMPONENT_NUM > 0)
        ComponentIntId = Dem_GetEventInternalId(ComponentId);
        /* req SWS_Dem_01134 */
        if (DemComponentStatus[ComponentIntId].availability == TRUE)
        {
            /* req SWS_Dem_01131 */
            *ComponentFailed = DemComponentStatus[ComponentIntId].Status;
            ret = E_OK;
        }
#else
        DEM_UNUSED(ComponentId);
        DEM_UNUSED(ComponentFailed);
#endif
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets the indicator status derived from the event status.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      IndicatorId: Number of indicator
 * Param-Name[out]     IndicatorStatus: Status of the indicator, like off, on, or blinking.
 * Param-Name[in/out]  None
 * Return              E_OK: Operation was successful E_NOT_OK: Operation failed
 */
/*************************************************************************/
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_InterGetIndicatorStatus(
    /* PRQA S 1532-- */ /* MISRA Rule 8.7 */
    uint8 IndicatorId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dem_IndicatorStatusType, AUTOMATIC, DEM_APPL_DATA) IndicatorStatus)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_NOT_OK;

#if (DEM_INDICATOR_NUM > 0u)
    if (IndicatorId >= DEM_INDICATOR_NUM)
    {
#endif /* DEM_INDICATOR_NUM > 0u */
        DEM_DET_REPORT(DEM_SID_GETINDICATORSTATUS, DEM_E_WRONG_CONFIGURATION);
#if (DEM_INDICATOR_NUM > 0u)
    }
    else if (IndicatorStatus == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_GETINDICATORSTATUS, DEM_E_PARAM_POINTER);
    }
    else
    {
        *IndicatorStatus = DemWIRStatus[IndicatorId];
        ret = E_OK;
    }
#else  /* DEM_INDICATOR_NUM > 0u */
    DEM_UNUSED(IndicatorId);
    DEM_UNUSED(IndicatorStatus);
#endif /* DEM_INDICATOR_NUM > 0u */
    return ret;
}

/*************************************************************************/
/*
 * Brief               Sets the indicator status included in the event status.
 * ServiceId           None
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      IndicatorId : Number of indicator
 *                      IndicatorStatus Status of the indicator, like off, on, or blinking.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Operation was successful
 *                       E_NOT_OK: Operation failed or is not supported
 */
/*************************************************************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_InterSetIndicatorStatus(
    uint8 IndicatorId,
    P2CONST(Dem_IndicatorStatusType, AUTOMATIC, DEM_APPL_DATA) IndicatorStatus)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    Std_ReturnType ret = E_NOT_OK;

#if (DEM_INDICATOR_NUM > 0)
    if (IndicatorId >= DEM_INDICATOR_NUM)
    {
#endif /* DEM_INDICATOR_NUM > 0u */
        DEM_DET_REPORT(DEM_SID_SETINDICATORSTATUS, DEM_E_WRONG_CONFIGURATION);
#if (DEM_INDICATOR_NUM > 0)
    }
    else if (IndicatorStatus == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_SETINDICATORSTATUS, DEM_E_PARAM_POINTER);
    }
    else
    {
        DemWIRStatus[IndicatorId] = *IndicatorStatus;
        ret = E_OK;
    }
#else  /* DEM_INDICATOR_NUM > 0u */
    DEM_UNUSED(IndicatorId);
    DEM_UNUSED(IndicatorStatus);
#endif /* DEM_INDICATOR_NUM > 0u */
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets the data of a freeze frame by event.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      EventId: Identification of an event by assigned EventId.
 *                       RecordNumber: This parameter is a unique identifier for a freeze
 *                       frame record as defined in ISO15031-5 and ISO14229-1.
 *                       0xFF means most recent freeze frame record is returned.
 *                       DataId : This parameter specifies the PID (ISO15031-5 mapped in UDS range
 *                       0xF400u - 0xF4FFu) or DID (ISO14229-1) that shall be copied to
 *                       the destination buffer.
 * Param-Name[out]     DestBuffer : This parameter contains a byte pointer that points to the
 *                       buffer, to which the freeze frame data record shall be written to.
 *                       The format is raw hexadecimal values and contains no header-information.
 * Param-Name[in/out]   BufSize: When the function is called this parameter contains
                            the maximum number of data bytes that can be written to the buffer.
                            The function returns the actual number of written
                            data bytes in this parameter
 * Return              E_OK: Operation was successful
 *                       DEM_E_NODATAAVAILABLE: The requested event data is not currently stored
 *                                              (but the request was valid)
 *                       DEM_E_WRONG_RECORDNUMBER: The requested record number is not supported
 *                                              by the event
 *                       DEM_E_WRONG_DIDNUMBER: The requested DID is not supported by the freeze
 frame
 */
/*************************************************************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_InterGetEventFreezeFrameDataEx(
    Dem_EventIdType IntId,
    uint8 RecordNumber,
    uint16 DataId,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    Std_ReturnType ret = E_NOT_OK;
#if ((DEM_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_PID_CLASS_NUM > 0))
    Dem_EventMemEntryType* pEntry = NULL_PTR;
    uint8 MemDest;
    uint16 dtcRef = DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef;
#if (DEM_EVENT_COMBINATION_SUPPORT == DEM_EVCOMB_ONSTORAGE)
    if (dtcRef != DEM_DTC_REF_INVALID)
    {
        const uint8* pMemDest =
            DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].DemDTCAttributesRef].DemMemoryDestinationRef;
        pEntry = Dem_GetEVCOMBEntry(pMemDest, dtcRef);
    }
#else
    pEntry = Dem_CheckEventMemEntryExistsAlready(IntId);
#endif /* DEM_EVENT_COMBINATION_SUPPORT == DEM_EVCOMB_ONSTORAGE */
    /* req SWS_Dem_00996 */
    if (pEntry != NULL_PTR)
    {
#if (DEM_MEM_DEST_MAX_NUM_OF_DTC > 1u)
        for (uint8 iloop = 0; iloop < DEM_MEM_DEST_MAX_NUM_OF_DTC; iloop++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
#else
        uint8 iloop = 0;
#endif /* DEM_MEM_DEST_MAX_NUM_OF_DTC > 1u */
        {
            /* req SWS_Dem_01062 */
            MemDest = DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].DemDTCAttributesRef]
                          .DemMemoryDestinationRef[iloop];
            if ((MemDest != DEM_MEM_DEST_INVALID) && (MemDest != DEM_DTC_ORIGIN_PERMANENT_MEMORY)
                && (MemDest != DEM_DTC_ORIGIN_MIRROR_MEMORY))
            {
                ret = Dem_FreezeFrameGetFromEntry(pEntry, RecordNumber, DataId, DestBuffer, BufSize);
#if (DEM_MEM_DEST_MAX_NUM_OF_DTC > 1u)
                break;
#endif /* DEM_MEM_DEST_MAX_NUM_OF_DTC > 1u */
            }
        }
    }
#else
    DEM_UNUSED(IntId);
    DEM_UNUSED(RecordNumber);
    DEM_UNUSED(DataId);
    DEM_UNUSED(DestBuffer);
    DEM_UNUSED(BufSize);
#endif /* DEM_FREEZE_FRAME_CLASS_NUM > 0 || \
          DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0 || (DEM_PID_CLASS_NUM > 0 */
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets the data of an extended data record by event.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      EventId : Identification of an event by assigned EventId.
 *                       RecordNumber: Identification of requested Extended data record.
 *                       Valid values are between 0x01 and 0xEF as defined in ISO14229-1.
 * Param-Name[out]     DestBuffer: This parameter contains a byte pointer that points
 *                          to the buffer, to which the extended data shall bewritten to.
 *                          The format is raw hexadecimal values and contains no header-information.
 * Param-Name[in/out]  None
 * Return              E_OK: Operation was successful
 *                       DEM_E_NODATAAVAILABLE: The requested event data is not currently
 *                              stored (but the request was valid)
 *                       DEM_E_WRONG_RECORDNUMBER: The requested record number is not supported by
 * the event
 */
/*************************************************************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_InterGetEventExtendedDataRecordEx(
    Dem_EventIdType IntId,
    uint8 RecordNumber,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_EXTENDED_DATA_CLASS_NUM > 0)
    const Dem_EventMemEntryType* pEntry = NULL_PTR;
    uint8 MemDest;
    uint16 dtcRef = DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef;

#if (DEM_EVENT_COMBINATION_SUPPORT == DEM_EVCOMB_ONSTORAGE)
    if (dtcRef != DEM_DTC_REF_INVALID)
    {
        const uint8* pMemDest =
            DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].DemDTCAttributesRef].DemMemoryDestinationRef;
        pEntry = Dem_GetEVCOMBEntry(pMemDest, dtcRef);
    }
#else
    pEntry = Dem_CheckEventMemEntryExistsAlready(IntId);
#endif /* DEM_EVENT_COMBINATION_SUPPORT == DEM_EVCOMB_ONSTORAGE */
    /* req SWS_Dem_00997 */
    if (pEntry != NULL_PTR)
    {
#if (DEM_MEM_DEST_MAX_NUM_OF_DTC > 1u)
        for (uint8 iloop = 0; iloop < DEM_MEM_DEST_MAX_NUM_OF_DTC; iloop++)
#else
        uint8 iloop = 0;
#endif /* DEM_MEM_DEST_MAX_NUM_OF_DTC > 1u */
        {
            MemDest = DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].DemDTCAttributesRef]
                          .DemMemoryDestinationRef[iloop];
            if (MemDest != DEM_MEM_DEST_INVALID)
            {
                /* req SWS_Dem_01062 */
                if ((MemDest != DEM_DTC_ORIGIN_PERMANENT_MEMORY) && (MemDest != DEM_DTC_ORIGIN_MIRROR_MEMORY))
                {
                    ret = Dem_ExtendedDataGetFromEntry(pEntry, RecordNumber, DestBuffer, BufSize);
#if (DEM_MEM_DEST_MAX_NUM_OF_DTC > 1u)
                    break;
#endif /* DEM_MEM_DEST_MAX_NUM_OF_DTC > 1u */
                }
            }
        }
    }
#else
    DEM_UNUSED(IntId);
    DEM_UNUSED(RecordNumber);
    DEM_UNUSED(DestBuffer);
    DEM_UNUSED(BufSize);
#endif /* DEM_EXTENDED_DATA_CLASS_NUM > 0 */
    return ret;
}

/*************************************************************************/
/*
 * Brief               Set the availability of a specific DemComponent.
 * ServiceId           none
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ComponentId: Identification of a DemComponent.
 *                       AvailableStatus: This parameter specifies whether the respective
 *                          Component shall be available (TRUE) or not (FALSE).
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              E_OK: Operation was successful
 */
/*************************************************************************/
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_InterSetComponentAvailable(Dem_ComponentIdType ComponentId, boolean AvailableStatus)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_COMPONENT_NUM > 0)
    Dem_ComponentIdType ComponentIntId;
    uint16 iloop;
    Dem_EventRelateInformationType* pEvent;
    const Dem_EventParameterType* pEventCfg;
#endif

    if ((ComponentId > DEM_COMPONENT_NUM) || (ComponentId == 0u)) /* PRQA S 2995,2991 */ /* MISRA Rule 2.2,14.3 */
    {
        DEM_DET_REPORT(DEM_SID_SETCOMPONENTAVAILABLE, DEM_E_WRONG_RECORDNUMBER);
        DEM_UNUSED(AvailableStatus);
    }
    else
    {
#if (DEM_COMPONENT_NUM > 0)
        ComponentIntId = Dem_GetEventInternalId(ComponentId);
        /* req SWS_Dem_01135 */
        DemComponentStatus[ComponentIntId].availability = AvailableStatus;
        /* req SWS_Dem_01136 */
        if (AvailableStatus == FALSE)
        {
            for (iloop = 0; iloop < DemPbCfgPtr->DemEventNum; iloop++)
            {
                pEventCfg = &DemPbCfgPtr->DemEventParameter[iloop];
                pEvent = Dem_GetEventInfo(iloop);
                if (pEventCfg->DemComponentClassRef == ComponentIntId)
                {
                    if ((NULL_PTR != Dem_CheckEventMemEntryExistsAlready(iloop))
                        || (DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF) == 0x01u))
                    {
                        /*idle*/
                    }
                    else
                    {
                        DEM_FLAGS_CLR(pEvent->Status, DEM_EVENT_STATUS_AVAILABLE);
                    }
                }
            }
        }
        ret = E_OK;
#else
        DEM_UNUSED(ComponentId);
        DEM_UNUSED(AvailableStatus);
#endif /* DEM_COMPONENT_NUM > 0u */
    }
    return ret;
}

#if ((DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0u) || (DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0u))
/*************************************************************************/
/*
 * Brief               Dem_DebounceFreeze
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
FUNC(void, DEM_CODE) Dem_DebounceFreeze(Dem_EventIdType IntId)
{
    const Dem_EventParameterType* pEventCfg = &DemPbCfg.DemEventParameter[IntId];
    uint8 AlgorithmType = pEventCfg->AlgorithmType;
    uint16 AlgorithmRef = pEventCfg->AlgorithmRef;
    uint16 AlgorithmIndex = pEventCfg->AlgorithmIndex;
#if (DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
    /*SWS_Dem_00654,SWS_Dem_00677 */
    if ((AlgorithmType == DEM_DEBOUNCE_TIME_BASE) && (AlgorithmRef < DEM_DEBOUNCE_TIME_BASE_CLASS_NUM)
        && (AlgorithmIndex < DEM_DEBOUNCE_TIME_BASED_EVENT_NUM))
    {
        Dem_DebounceTimeInfoType* pDemDebounceTimerInfo = &DemDebounceTimerInfo[AlgorithmIndex];
        if (DemPbCfg.DemDebounceTimeBaseClass[AlgorithmRef].DemDebounceBehavior == DEM_DEBOUNCE_FREEZE)
        {
            DemDebounceTimerInfo[AlgorithmIndex].IsFreezing = TRUE;
        }
        else
        {
            pDemDebounceTimerInfo->CurStatus = DEM_EVENT_STATUS_PREPASSED;
            pDemDebounceTimerInfo->Timeout = 0;
            pDemDebounceTimerInfo->Triggered = FALSE;
        }
    }
    else
#endif /*DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0 */
#if (DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
        if ((AlgorithmType == DEM_DEBOUNCE_COUNTER_BASED) && (AlgorithmRef < DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM)
            && (AlgorithmIndex < DEM_DEBOUNCE_COUNTER_BASED_EVENT_NUM))
    {
        if (DemPbCfg.DemDebounceCounterBasedClass[AlgorithmRef].DemDebounceBehavior == DEM_DEBOUNCE_RESET)
        {
            DemDebounceCounterInfo[AlgorithmIndex].InternalDebounceCounter = 0;
        }
    }
    else
#endif /*DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0 */
    {
        /*idle*/
    }
}
#endif /* DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0u) || (DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0u */

#if ((DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT) || (DEM_J1939_SUPPORT == STD_ON))
#if (DEM_RATIO_NUM > 0)
/*************************************************************************/
/*
 * Brief               In order to communicate the status of the (additional) denominator
                        conditions among the OBD relevant ECUs, the API is used to forward
                        the condition status to a Dem of a particular ECU
 * ServiceId           None
 * Sync/Async          Synchronous /Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ConditionId: Identification of a IUMPR denominator condition ID
                        ConditionStatus:Status of the IUMPR denominator condition
                        (Notreached, reached, not reachable / inhibited)
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: set of IUMPR denominator condition was successful
                        E_NOT_OK: set of IUMPR denominator condition
                        failed or could not be accepted.
 */
/*************************************************************************/
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_InterSetIUMPRDenCondition(Dem_IumprDenomCondIdType ConditionId, Dem_IumprDenomCondStatusType ConditionStatus)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret;
#if (DEM_RATIO_NUM > 0)
    /* req SWS_Dem_00714 */
    ConditionStatu[ConditionId] = ConditionStatus;
    ret = E_OK;
#else
    DEM_UNUSED(ConditionId);
    DEM_UNUSED(ConditionStatus);
    ret = E_NOT_OK;
#endif
    return ret;
}

/*************************************************************************/
/*
 * Brief               In order to communicate the status of the (additional) denominator
                        conditions among the OBD relevant ECUs, the API is used to retrieve
                        the condition status from the Dem of the ECU where the conditions are
                        computed.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ConditionId: Identification of a IUMPR denominator condition ID
 * Param-Name[out]     ConditionStatus:Status of the IUMPR denominator condition
 *                      (Notreached, reached, not reachable / inhibited)
 * Param-Name[in/out]  None
 * Return              E_OK: get of IUMPR denominator condition status was successful
                        E_NOT_OK: get of condition status failed
 */
/*************************************************************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_InterGetIUMPRDenCondition(
    Dem_IumprDenomCondIdType ConditionId,
    P2VAR(Dem_IumprDenomCondStatusType, AUTOMATIC, DEM_APPL_DATA) ConditionStatus)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    Std_ReturnType ret;
#if (DEM_RATIO_NUM > 0)
    /* req SWS_Dem_00966 */
    *ConditionStatus = ConditionStatu[ConditionId];
    ret = E_OK;
#else
    DEM_UNUSED(ConditionId);
    DEM_UNUSED(ConditionStatus);
    ret = E_NOT_OK;
#endif
    return ret;
}
#endif /*DEM_RATIO_NUM > 0 */

/*************************************************************************/
/*
 * Brief               Calculate the OBD related data.
 * ServiceId           Internal Function
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
FUNC(void, DEM_CODE) Dem_InterCalOBDRelatedValue(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (DEM_RATIO_NUM > 0)
    uint16 iloop;
    uint16 DemDTCRef;
    const Dem_EventRelateInformationType* pEvent;
    uint8 eventid;
#if (DEM_TRIGGER_FIM_REPORTS == STD_ON)
    boolean permission;
    uint8 FimFunctionId;
    uint16 index;
#endif

    for (iloop = 0; iloop < DEM_RATIO_NUM; iloop++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
    {
        uint8 DemDiagnosticEventRef = DemPbCfgPtr->DemRatio[iloop].DemDiagnosticEventRef;
        DemDTCRef = DemPbCfgPtr->DemEventParameter[DemDiagnosticEventRef].DemDTCRef;
        pEvent = Dem_GetEventInfo(DemDiagnosticEventRef);
        /* req SWS_Dem_01104 */
        if ((DemDTCRef != DEM_DTC_REF_INVALID) && (DemDTCGeneralStatus[DemDTCRef].SuppressionStatus != TRUE)
            && (0x00u != DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_AVAILABLE)))
        {
            /* req SWS_Dem_00709 */
#if (DEM_TRIGGER_FIM_REPORTS == STD_ON)
            FimFunctionId = DemPbCfgPtr->DemRatio[iloop].DemFunctionIdRef;
            FiM_GetFunctionPermission(FimFunctionId, &permission);
            /*Consider the integration with the FIM  Numerator and Denominator both are forbidden*/
            if (permission == TRUE)
            {
                const DemSecondaryFunctionIdType* pSecondaryFIdRef =
                    DemPbCfgPtr->DemRatio[iloop].DemSecondaryFunctionIdRef;
                if (pSecondaryFIdRef != NULL_PTR)
                {
                    uint8 RefNum = pSecondaryFIdRef->DemSecondaryFunctionIdRefNum;
                    for (index = 0; iloop < RefNum; index++)
                    {
                        FimFunctionId = pSecondaryFIdRef->DemSecondaryFunctionIdRef[index];
                        FiM_GetFunctionPermission(FimFunctionId, &permission);
                        /*Consider the integration with the FIM*/
                        if (permission == FALSE)
                        {
                            /*Numerator and Denominator both are forbidden*/
                            break;
                        }
                    }
                }
            }
#endif
            /*get General Denominator*/
            eventid = DemPbCfgPtr->DemRatio[iloop].DemDiagnosticEventRef;
            pEvent = Dem_GetEventInfo(eventid);
            IUMPRType* pIUMPRValue = &IUMPRValue[iloop];
            if (0u != DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_ENABLED_CONDICTION))
            {
                pIUMPRValue->General_Denominator++;
                /*get Specific Diagnostic Denominator req SWS_Dem_00712 */
                if (pIUMPRValue->IUMPRDenStatus == TRUE)
                {
                    if (pIUMPRValue->Denominator < 0xFFFFu)
                    {
                        pIUMPRValue->Denominator++;
                    }
                }
            }
#if ((DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU) || (DEM_OBD_SUPPORT == DEM_OBD_PRIMARY_ECU))
            /* req SWS_Dem_00359  req SWS_Dem_00361  req SWS_Dem_00710 */
            const DemRatioType* pRatio = &DemPbCfgPtr->DemRatio[iloop];
            if (pRatio->DemRatioKind == DEM_RATIO_OBSERVER)
            {
                if (DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TFTOC) != 0x00u)
                {
                    if (pIUMPRValue->Numerator < 0xFFFFu)
                    {
                        pIUMPRValue->Numerator++;
                    }
                }
            }
            else
            {
                if (pIUMPRValue->IUMPRNumStatus == TRUE)
                {
                    if (pIUMPRValue->Numerator < 0xFFFFu)
                    {
                        pIUMPRValue->Numerator++;
                    }
                    pIUMPRValue->IUMPRNumStatus = FALSE;
                }
            }
#endif
        }
    }
#endif
}

/*************************************************************************/
/*
 * Brief               Service is used for requesting IUMPR data according to InfoType 08.
 * ServiceId           Non
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      OpStatus Only DCM_INITIAL will appear, because this API
                        behaves synchronous.
 * Param-Name[out]     Iumprdata08 Buffer containing the number of data elements (as
                        defined in ISO-15031-5) and contents of InfoType
                        08. The buffer is provided by the Dcm.
 * Param-Name[in/out]  Iumprdata08BufferSize: The maximum number of data bytes that can be
                        written to the Iumprdata08 Buffer.
 * Return              Non
 */
/*************************************************************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(void, DEM_CODE)
Dem_InterDcmGetInfoTypeValue08(
    Dcm_OpStatusType OpStatus,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) Iumprdata08,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) Iumprdata08BufferSize)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
#if ((DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT) || (DEM_RATIO_NUM > 0))
    uint8 tempData[32] = {0};
#else
    const uint8 tempData[32] = {0};
#endif

#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
    tempData[0U] = (uint8)(IgnUpCycleCounter >> 8U);
    tempData[1U] = (uint8)(IgnUpCycleCounter);
#endif
#if (DEM_RATIO_NUM > 0)
    tempData[2U] = (uint8)(IUMPRValue[0U].General_Denominator >> 8U);
    tempData[3U] = (uint8)(IUMPRValue[0U].General_Denominator);

    tempData[4U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_CAT1) >> 8U);
    tempData[5U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_CAT1));
    tempData[6U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_CAT1) >> 8U);
    tempData[7U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_CAT1));

    tempData[8U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_CAT2) >> 8U);
    tempData[9U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_CAT2));
    tempData[10U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_CAT2) >> 8U);
    tempData[11U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_CAT2));

    tempData[12U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_OXS1) >> 8U);
    tempData[13U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_OXS1));
    tempData[14U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_OXS1) >> 8U);
    tempData[15U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_OXS1));

    tempData[16U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_OXS2) >> 8U);
    tempData[17U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_OXS2));
    tempData[18U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_OXS2) >> 8U);
    tempData[19U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_OXS2));

    tempData[20U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_EGR) >> 8U);
    tempData[21U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_EGR));
    tempData[22U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_EGR) >> 8U);
    tempData[23U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_EGR));

    tempData[24U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_SAIR) >> 8U);
    tempData[25U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_SAIR));
    tempData[26U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_SAIR) >> 8U);
    tempData[27U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_SAIR));

    tempData[28U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_EVAP) >> 8U);
    tempData[29U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_EVAP));
    tempData[30U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_EVAP) >> 8U);
    tempData[31U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_EVAP));
#endif
    *Iumprdata08BufferSize = 32U;

    switch (OpStatus)
    {
    case 0x00u:
        Dem_MemCopy(Iumprdata08, tempData, 32U);
        break;
    case 0x01u:
    case 0x02u:
    case 0x03u:
        /*idle*/
        break;
    default:
        /*idle*/
        break;
    }
}

/*************************************************************************/
/*
 * Brief               Service is used for requesting IUMPR data according to InfoType 0B.
 * ServiceId           Non
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      OpStatus Only DCM_INITIAL will appear, because this API
                        behaves synchronous.
 * Param-Name[out]     Iumprdata0B Buffer containing the number of data elements (as
                        defined in ISO-15031-5) and contents of InfoType
                        08. The buffer is provided by the Dcm.
 * Param-Name[in/out]  Iumprdata0BBufferSize: The maximum number of data bytes that can be
                        written to the Iumprdata08 Buffer.
 * Return              Non
 */
/*************************************************************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(void, DEM_CODE)
Dem_InterDcmGetInfoTypeValue0B(
    Dcm_OpStatusType OpStatus,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) Iumprdata0B,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) Iumprdata0BBufferSize)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
#if ((DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT) || (DEM_RATIO_NUM > 0))
    uint8 tempData[32] = {0};
#else
    const uint8 tempData[32] = {0};
#endif

#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
    tempData[0U] = (uint8)(IgnUpCycleCounter >> 8U);
    tempData[1U] = (uint8)(IgnUpCycleCounter);
#endif
#if (DEM_RATIO_NUM > 0)
    tempData[2U] = (uint8)(IUMPRValue[0].General_Denominator >> 8U);
    tempData[3U] = (uint8)(IUMPRValue[0].General_Denominator);

    tempData[4U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_CAT1) >> 8U);
    tempData[5U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_CAT1));
    tempData[6U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_CAT1) >> 8U);
    tempData[7U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_CAT1));

    tempData[8U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_CAT2) >> 8U);
    tempData[9U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_CAT2));
    tempData[10U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_CAT2) >> 8U);
    tempData[11U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_CAT2));

    tempData[12U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_OXS1) >> 8U);
    tempData[13U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_OXS1));
    tempData[14U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_OXS1) >> 8U);
    tempData[15U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_OXS1));

    tempData[16U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_OXS2) >> 8U);
    tempData[17U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_OXS2));
    tempData[18U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_OXS2) >> 8U);
    tempData[19U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_OXS2));

    tempData[20U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_EGR) >> 8U);
    tempData[21U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_EGR));
    tempData[22U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_EGR) >> 8U);
    tempData[23U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_EGR));

    tempData[24U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_SAIR) >> 8U);
    tempData[25U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_SAIR));
    tempData[26U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_SAIR) >> 8U);
    tempData[27U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_SAIR));

    tempData[28U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_EVAP) >> 8U);
    tempData[29U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_EVAP));
    tempData[30U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_EVAP) >> 8U);
    tempData[31U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_EVAP));
#endif
    *Iumprdata0BBufferSize = 32U;

    switch (OpStatus)
    {
    case 0x00u:
        Dem_MemCopy(Iumprdata0B, tempData, 32U);
        break;
    case 0x01u:
    case 0x02u:
    case 0x03u:
        /*idle*/
        break;
    default:
        /*idle*/
        break;
    }
}

/*************************************************************************/
/*
 * Brief               Reports the value of a requested "availability-OBDMID" to the DCM upon a
 * Service 06 request. ServiceId           None Sync/Async          Synchronous Reentrancy Reentrant
 * Param-Name[in]      Obdmid Availablity OBDMID (00,20, 40...)
 * Param-Name[out]     Obdmidvalue Bit coded information on the support of OBDMIDs.
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(void, DEM_CODE)
Dem_InterDcmGetAvailableOBDMIDs(uint8 Obdmid, P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) Obdmidvalue)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
#if (DEM_DTR_NUM > 0)
    uint16 iloop;
    boolean Find = FALSE;

    *Obdmidvalue = 0;
    for (iloop = 0; iloop < DEM_DTR_NUM; iloop++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
    {
        uint8 DemDtrMid = DemPbCfgPtr->DTR[iloop].DemDtrMid;
        if ((DemDtrMid >= (Obdmid + 0x01u)) && (DemDtrMid <= (Obdmid + 0x20u)))
        {
            /* req SWS_Dem_00760 */
            *Obdmidvalue |= (uint32)1u << (0x20u - (DemDtrMid - Obdmid));
        }
        if ((DemDtrMid == Obdmid) || (Obdmid == 0x0u))
        {
            Find = TRUE;
        }
    }
    if (Find == FALSE)
    {
        *Obdmidvalue = 0;
    }
#else
    DEM_UNUSED(Obdmid);
    *Obdmidvalue = 0;
#endif
}

/*************************************************************************/
/*
 * Brief               Gets the number of TIDs per (functional) OBDMID.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Obdmid OBDMID subject of the request to identify the number of assigned TIDs
 * Param-Name[out]     numberOfTIDs Number of assigned TIDs for the requested OBDMID.
 *                      Used as loop value for the DCM to retrieve all OBD/TID result data
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(void, DEM_CODE)
Dem_InterDcmGetNumTIDsOfOBDMID(uint8 Obdmid, P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) numberOfTIDs)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
#if (DEM_DTR_NUM > 0)
    uint16 iloop;
    *numberOfTIDs = 0u;
    for (iloop = 0; iloop < DEM_DTR_NUM; iloop++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
    {
        if (DemPbCfgPtr->DTR[iloop].DemDtrMid == Obdmid)
        {
            /* req SWS_Dem_00761 */
            (*numberOfTIDs) += 1u;
        }
    }
#else
    DEM_UNUSED(Obdmid);
    *numberOfTIDs = 0u;
#endif
}

/*************************************************************************/
/*
 * Brief               Gets data element per PID and index of the most important freeze frame
                        being selected for the output of service 02. The function stores the data
                        in the provided DestBuffer.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      PID: This parameter is an identifier for a PID as defined
                        in ISO15031-5.
                        DataElementIndexOfPID: Data element index of this PID according
                        to the Dcm configuration of service 02. It is zero-based and
                        consecutive, and ordered by the data element  positions
                        (configured in Dcm, refer to SWS_Dem_00597).>
 * Param-Name[out]     None
 * Param-Name[in/out]  DestBuffer: This parameter contains a byte pointer that points to
                        the buffer, to which the data element of the PID shall
                        be written to. The format is raw hexadecimal values
                        and contains no header-information.
                        BufSize: When the function is called this parameter contains
                        the maximum number of data bytes that can be written
                        to the buffer.
                        The function returns the actual number of written
                        data bytes in this parameter.
 * Return              E_OK Freeze frame data was successfully reported
                        E_NOT_OK Freeze frame data was not successfully reported
 */
/*************************************************************************/
/* PRQA S 1532,3432,3206++ */ /* MISRA Rule 8.7,20.7,2.7 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_InterDcmReadDataOfOBDFreezeFrame(
    uint8 PID,
    uint8 DataElementIndexOfPID,
#if ((DEM_PID_CLASS_NUM > 0) && (DEM_DATA_ELEMENT_CLASS_NUM > 0))
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize
#else
    P2CONST(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
    P2CONST(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize
#endif
)
/* PRQA S 1532,3432,3206-- */ /* MISRA Rule 8.7,20.7,2.7 */
{
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_PID_CLASS_NUM > 0)
    const uint8* pBuffer;
    const Dem_EventMemEntryType* pEntry;
    uint16 IntID;
    uint8 DataElementIndex;
    uint8 pid_num;
    const Dem_PidClassType* pPid;
    uint16 obdDtcRef;
    uint16 tempRef;
    uint8 cnt = 0;

    pEntry = Dem_SelectOBDFreezeFrame(&IntID);
    if ((pEntry != NULL_PTR) && (IntID != DemPbCfgPtr->DemEventNum)
        && (DemPbCfgPtr->DemEventParameter[IntID].DemDTCRef != DEM_DTC_REF_INVALID))
    {
        uint16 DTCRef = DemPbCfgPtr->DemEventParameter[IntID].DemDTCRef;
        obdDtcRef = DemPbCfgPtr->DemDTC[DTCRef].DemObdDTCRef;
        tempRef = DemPbCfgPtr->DemDTC[DTCRef].DemDTCAttributesRef;
        const Dem_DTCAttributesType* pDTCAttr = &DemPbCfgPtr->DemDTCAttributes[tempRef];
        if (((obdDtcRef != DEM_OBD_DTC_INVALID)
             && ((DemPbCfgPtr->DemObdDTC[obdDtcRef].DemDtcValue != DEM_OBD_DTC_CFG_INVALID)))
            && (pDTCAttr != NULL_PTR) && (pDTCAttr->DemFreezeFrameClassRef == DEM_FREEZE_FRAME_INVALID)
            && (pDTCAttr->DemWWHOBDFreezeFrameClassRef == DEM_WWHOBD_FREEZE_FRAME_INVALID)
            && (pDTCAttr->DemJ1939FreezeFrameClassRef == DEM_J1939_FREEZE_FRAME_INVALID)
            && (pDTCAttr->DemJ1939ExpandedFreezeFrameClassRef == DEM_J1939_FREEZE_FRAME_INVALID))
        {
            /* req SWS_Dem_01101 */
            if (DemDTCGeneralStatus[DemPbCfgPtr->DemEventParameter[IntID].DemDTCRef].SuppressionStatus == FALSE)
            {
                for (pid_num = 0; (pid_num < DEM_PID_CLASS_NUM) && (ret == E_NOT_OK); pid_num++)
                {
                    pBuffer = &(pEntry->FFList[0]).Data[cnt];
                    pPid = &DemPbCfgPtr->DemPidClass[pid_num];
                    if (PID == pPid->DemPidIdentifier)
                    {
                        for (DataElementIndex = 0; DataElementIndex < pPid->RefNum; DataElementIndex++)
                        {
#if (DEM_DATA_ELEMENT_CLASS_NUM > 0)
                            if (DataElementIndexOfPID == (pPid->StartIndex + DataElementIndex))
                            {
                                const DemDataElementClassTypes* pDemDataElementClass =
                                    &DemPbCfgPtr->DemDataElementClass[pPid->StartIndex + DataElementIndex];
                                uint8 ElementDataSize = pDemDataElementClass->DemDataElementDataSize;
                                if (FALSE == pDemDataElementClass->DemInterFlag)
                                {
                                    Dem_MemCopy(DestBuffer, pBuffer, ElementDataSize);
                                }
                                else
                                {
                                    /* get inter data now */
                                    pDemDataElementClass->DemDataElementReadFnc(DestBuffer);
                                }
                                *BufSize += ElementDataSize;
                                ret = E_OK;
                            }
#endif
                        }
                    }
                    else
                    {
                        cnt += pPid->DataSize;
                    }
                }
            }
        }
    }
#else
    DEM_UNUSED(PID);
    DEM_UNUSED(DataElementIndexOfPID);
    DEM_UNUSED(DestBuffer);
    DEM_UNUSED(BufSize);
#endif
    return ret;
}

/*************************************************************************/
/*
 * Brief               Reports a DTR data along with TID-value, UaSID, test result with lower and
 upper limit.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Obdmid: Identification of a DTR element by assigned DTRId.
                        TIDindex: Index of the TID within the DEM. Runs from 0
                        to "numberOfTIDs" obtained in the call to Dem_
                        DcmGetNumTIDsOfOBDMID()
 * Param-Name[out]     TIDvalue: TID to be put on the tester reponse
                        UaSID: UaSID to be put on the tester reponse
                        Testvalue:Latest test result
                        Lowlimvalue:Lower limit value associated to the latest test result
                        Upplimvalue:Upper limit value associated to the latest test result
 * Param-Name[in/out]  None
 * Return              E_OK: Report of DTR result successful
                        E_NOT_OK: Report of DTR result failed
 */
/*************************************************************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_InterDcmGetDTRData(
    uint8 Obdmid,
    uint8 TIDindex,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) TIDvalue,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) UaSID,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) Testvalue,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) Lowlimvalue,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) Upplimvalue)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_DTR_NUM > 0)
    uint16 iloop;
    const DTRType* pDTR = DemPbCfgPtr->DTR;

    for (iloop = 0; iloop < DEM_DTR_NUM; iloop++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
    {
        if ((Obdmid == pDTR[iloop].DemDtrMid) && (TIDindex == iloop))
        {
            /* req SWS_Dem_00762 */
            *TIDvalue = pDTR[iloop].DemDtrTid;
            *UaSID = pDTR[iloop].DemDtrUasid;
            *Lowlimvalue = (uint16)DTRInfo[iloop].LowerLimit;
            *Testvalue = (uint16)DTRInfo[iloop].TestResult;
            *Upplimvalue = (uint16)DTRInfo[iloop].UpperLimit;
            ret = E_OK;
        }
    }
#else
    DEM_UNUSED(Obdmid);
    DEM_UNUSED(TIDindex);
    *TIDvalue = 0;
    *UaSID = 0;
    *Lowlimvalue = 0;
    *Testvalue = 0;
    *Upplimvalue = 0;
#endif
    return ret;
}

/*************************************************************************/
/*
 * Brief               Service for reporting that faults are possibly found
                        because all conditions are fullfilled.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Ratio Identifier reporting that a respective monitor
                        could have found a fault
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK report of IUMPR result was successfully reported
 */
/*************************************************************************/
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_InterRepIUMPRFaultDetect(Dem_RatioIdType RatioID)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;

    if (RatioID > DEM_RATIO_NUM)
    {
        DEM_DET_REPORT(DEM_SID_REPIUMPRFAULTDETECT, DEM_E_WRONG_CONFIGURATION);
    }
    else
    {
#if (DEM_RATIO_NUM > 0)
        if (DemPbCfgPtr->DemRatio[RatioID].DemRatioKind == DEM_RATIO_API)
        {
            /*malfunction could have been found.*/
            IUMPRValue[RatioID].IUMPRNumStatus = TRUE;
        }
        ret = E_OK;
#endif
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Service is used to lock a denominator of a specific monitor.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Ratio Identifier reporting that specific denominator
                        is locked
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK report of IUMPR denominator status was successfully reported
                        E_NOK report of IUMPR denominator status was
                        not successfully reported
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE) Dem_InterRepIUMPRDenLock(Dem_RatioIdType RatioID) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    if (RatioID > DEM_RATIO_NUM)
    {
        DEM_DET_REPORT(DEM_SID_REPIUMPRDENLOCK, DEM_E_WRONG_CONFIGURATION);
    }
    else
    {
#if (DEM_RATIO_NUM > 0)
        /* req SWS_Dem_00362 */
        if (DEM_IUMPR_DEN_PHYS_API == DemPbCfgPtr->DemRatio[RatioID].DemIUMPRDenGroup)
        {
            IUMPRValue[RatioID].IUMPRDenStatus = FALSE;
        }
        ret = E_OK;
#endif
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Service is used to release a denominator of a
                        specific monitor.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Ratio Identifier reporting that specific denominator
                        is released
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK report of IUMPR denominator status was successfully reported
                        E_NOK report of IUMPR denominator status was
                        not successfully reported
 */
/*************************************************************************/
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(Std_ReturnType, DEM_CODE) Dem_InterRepIUMPRDenRelease(Dem_RatioIdType RatioID)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    if (RatioID > DEM_RATIO_NUM)
    {
        DEM_DET_REPORT(DEM_SID_REPIUMPRDENRELEASE, DEM_E_WRONG_CONFIGURATION);
    }
    else
    {
#if (DEM_RATIO_NUM > 0)
        if (DEM_IUMPR_DEN_PHYS_API == DemPbCfgPtr->DemRatio[RatioID].DemIUMPRDenGroup) /*SWS_Dem_00362] */
        {
            IUMPRValue[RatioID].IUMPRDenStatus = TRUE;
        }
        ret = E_OK;
#endif
    }
    return ret;
}

#if (DEM_DTR_NUM > 0)
/*************************************************************************/
/*
 * Brief              Reports a DTR result with lower and upper limit. The internal eventstatus
                        serves as master whether the DTR values are forwarded or ignored,
                        also taking the DTRUpdateKind into account.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      DTRId:Identification of a DTR element by assigned DTRId
 *                      TestResult:Test result of DTR
 *                      LowerLimit:Lower limit of DTR
 *                      UpperLimit:Upper limit of DTR
 *                      Ctrlval:Control value of the DTR to support its interpretation
 Dem-internally.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Report of DTR result successful
                        E_NOT_OK: Report of DTR result failed
 */
/*************************************************************************/
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_InterSetDTR(uint16 DTRId, sint32 TestResult, sint32 LowerLimit, sint32 UpperLimit, Dem_DTRControlType Ctrlval)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_DTR_NUM > 0)
    const Dem_EventRelateInformationType* pEvent = &DemEventRelateInformation[DemPbCfgPtr->DTR[DTRId].DemDtrEventRef];
    uint8 DemDtrUpdateKind = DemPbCfgPtr->DTR[DTRId].DemDtrUpdateKind;

    if ((DemDtrUpdateKind == DEM_DTR_UPDATE_ALWAYS)
        || ((DemDtrUpdateKind == DEM_DTR_UPDATE_STEADY)
            && (0x00u == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TNCTOC))))
    {
        switch (Ctrlval)
        {
        case DEM_DTR_CTL_NORMAL:
            DTRInfo[DTRId].TestResult = TestResult;
            DTRInfo[DTRId].LowerLimit = LowerLimit;
            DTRInfo[DTRId].UpperLimit = UpperLimit;
            break;
        case DEM_DTR_CTL_NO_MAX:
            DTRInfo[DTRId].TestResult = TestResult;
            DTRInfo[DTRId].LowerLimit = LowerLimit;
            break;
        case DEM_DTR_CTL_NO_MIN:
            DTRInfo[DTRId].TestResult = TestResult;
            DTRInfo[DTRId].UpperLimit = UpperLimit;
            break;
        case DEM_DTR_CTL_RESET:
            DTRInfo[DTRId].TestResult = 0;
            DTRInfo[DTRId].LowerLimit = 0;
            DTRInfo[DTRId].UpperLimit = 0;
            break;
        default:
            /*idle*/
            break;
        }
        ret = E_OK;
    }
#else
    DEM_UNUSED(DTRId);
    DEM_UNUSED(TestResult);
    DEM_UNUSED(LowerLimit);
    DEM_UNUSED(UpperLimit);
    DEM_UNUSED(Ctrlval);
#endif
    return ret;
}
#endif /*DEM_DTR_NUM > 0 */

/*************************************************************************/
/*
 * Brief               Get the Current Distance Information.
 * ServiceId           Internal Function
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
FUNC(uint16, DEM_CODE) Dem_ReadDistanceInformation(void)
{
/*read current distance*/
#if ((DEM_DATA_ELEMENT_CLASS_NUM > 0) && (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT))
    uint8 data[2] = {0};
    uint8 ref = DemGeneralOBD.DemOBDInputDistanceInformation;
    const DemDataElementClassTypes* DemDataElementClass = &DemPbCfgPtr->DemDataElementClass[ref];
    if ((ref != DEM_DATA_ELEMENT_CLASS_NUM_INVALID) && (DemDataElementClass->DemDataElementReadFnc != NULL_PTR))
    {
        DemDataElementClass->DemDataElementReadFnc(data);
    }
#else
    const uint8 data[2] = {0};
#endif
    return ((uint16)data[0u] | (uint16)((uint16)data[1u] << 8u));
}

/*************************************************************************/
/*
 * Brief               Get the Current Time Since Engine Start.
 * ServiceId           Internal Function
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
FUNC(uint16, DEM_CODE) Dem_ReadTimeInformation(void)
{

#if ((DEM_DATA_ELEMENT_CLASS_NUM > 0) && (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT))
    uint8 data[2] = {0};
    uint8 ref = DemGeneralOBD.DemOBDTimeSinceEngineStart;
    const DemDataElementClassTypes* DemDataElementClass = &DemPbCfgPtr->DemDataElementClass[ref];
    if ((ref != DEM_DATA_ELEMENT_CLASS_NUM_INVALID) && (DemDataElementClass->DemDataElementReadFnc != NULL_PTR))
    {
        DemDataElementClass->DemDataElementReadFnc(data);
    }
#else
    const uint8 data[2] = {0};
#endif
    return ((uint16)data[0u] | (uint16)((uint16)data[1u] << 8u));
}
#endif /* DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT */

#if (DEM_J1939_SUPPORT == STD_ON)
#if (DEM_J1939_READING_DTC_SUPPORT == STD_ON)

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
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(void, DEM_CODE)
Dem_GetLampStatusOfJ1939DcmSetDTCFilter(P2VAR(Dem_J1939DcmLampStatusType, AUTOMATIC, DEM_APPL_DATA) LampStatus)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    *LampStatus = 0x00u;
#if (DEM_INDICATOR_NUM > 0)
/*Composite Malfunction Indicator Lamp Status*/
#if (DEM_MALFUNCTION_LAMP_INDICATOR != DemIndicator_INVALID)
    if (DemWIRStatus[DEM_MALFUNCTION_LAMP_INDICATOR] != DEM_INDICATOR_OFF)
    {
        /* req SWS_Dem_00858 */
        *LampStatus |= 0x4000u;
    }
    if ((DemWIRStatus[DEM_MALFUNCTION_LAMP_INDICATOR] == DEM_INDICATOR_OFF)
        || (DemWIRStatus[DEM_MALFUNCTION_LAMP_INDICATOR] == DEM_INDICATOR_CONTINUOUS))
    {
        /* req SWS_Dem_00859 */
        *LampStatus |= 0x00C0u;
    }
    if (DemWIRStatus[DEM_MALFUNCTION_LAMP_INDICATOR] == DEM_INDICATOR_SLOW_FLASH)
    {
        /* req SWS_Dem_00860 */
        *LampStatus &= ~0x00C0u;
    }
    if (DemWIRStatus[DEM_MALFUNCTION_LAMP_INDICATOR] == DEM_INDICATOR_FAST_FLASH)
    {
        /* req SWS_Dem_00861 */
        *LampStatus &= ~0x0080u;
    }
#endif
/*Composite Red Stop Lamp Status*/
#if (DEM_RED_STOP_LAMP_INDICATOR != DemIndicator_INVALID)
    if (DemWIRStatus[DEM_RED_STOP_LAMP_INDICATOR] != DEM_INDICATOR_OFF)
    {
        /* req SWS_Dem_00862 */
        *LampStatus |= 0x1000u;
    }
    if ((DemWIRStatus[DEM_RED_STOP_LAMP_INDICATOR] == DEM_INDICATOR_OFF)
        || (DemWIRStatus[DEM_RED_STOP_LAMP_INDICATOR] == DEM_INDICATOR_CONTINUOUS))
    {
        /* req SWS_Dem_00863 */
        *LampStatus |= 0x0030u;
    }
    if (DemWIRStatus[DEM_RED_STOP_LAMP_INDICATOR] == DEM_INDICATOR_SLOW_FLASH)
    {
        /* req SWS_Dem_00864 */
        *LampStatus &= ~0x0030u;
    }
    if (DemWIRStatus[DEM_RED_STOP_LAMP_INDICATOR] == DEM_INDICATOR_FAST_FLASH)
    {
        /* req SWS_Dem_00865 */
        *LampStatus &= ~0x0020u;
    }
#endif
    Dem_SubGetLampStatusOfJ1939DcmSetDTCFilter(LampStatus);
#endif
}

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
Dem_InterJ1939DcmGetNumberOfFilteredDTC(
    Dem_EventIdType IntId,
    uint8 entryId,
    P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA) findSameDTC,
    P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA) IsFind)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    *IsFind = FALSE;
    const Dem_EventRelateInformationType* pEvent = Dem_GetEventInfo(IntId);
    const Dem_J1939DTCFilterInfoType* pFilter = &DemJ1939DTCFilter;

#if (DEM_J1939_NODE_NUM > 0u)
    uint16 dtcRef = DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef;
    if ((dtcRef != DEM_DTC_REF_INVALID) && (Dem_J1939CeckNode(dtcRef, pFilter->node) == TRUE))
#endif
    {
        *IsFind = FALSE;
        /*match the DTCStatusFilter*/
        switch (pFilter->DTCStatusFilter)
        {
        /*(ConfirmedDTC == 1 AND TestFailed == 1) OR MIL_ON*/
        case DEM_J1939DTC_ACTIVE:
            if (((0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_CDTC))
                 && (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF)))
#if (DEM_MALFUNCTION_LAMP_INDICATOR != DemIndicator_INVALID)
                || ((DemWIRStatus[DEM_MALFUNCTION_LAMP_INDICATOR] != DEM_INDICATOR_OFF))
#endif /* DEM_MALFUNCTION_LAMP_INDICATOR != DemIndicator_INVALID */
            )
            {
                *IsFind = TRUE;
            }
            break;
            /*ConfirmedDTC == 1 AND TestFailed == 0 AND MIL_OFF*/
        case DEM_J1939DTC_PREVIOUSLY_ACTIVE:
            if ((0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_CDTC))
                && (0x00u == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF))
#if (DEM_MALFUNCTION_LAMP_INDICATOR != DemIndicator_INVALID)
                && ((DemWIRStatus[DEM_MALFUNCTION_LAMP_INDICATOR] == DEM_INDICATOR_OFF))
#endif /* DEM_MALFUNCTION_LAMP_INDICATOR != DemIndicator_INVALID */
            )
            {
                *IsFind = TRUE;
            }
            break;
            /*PendingDTC == 1*/
        case DEM_J1939DTC_PENDING:
            if (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_PDTC))
            {
                *IsFind = TRUE;
            }
            break;
        case DEM_J1939DTC_PERMANENT:
            *IsFind = TRUE;
            break;
            /*TestFailed == 1*/
        case DEM_J1939DTC_CURRENTLY_ACTIVE:
            if (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF))
            {
                *IsFind = TRUE;
            }
            break;
        default:
            /*idle*/
            break;
        }
        Dem_SubInterJ1939DcmGetNumberOfFilteredDTC(IntId, entryId, findSameDTC, IsFind);
    }
}

/*************************************************************************/
/*
 * Brief               Gets the next filtered J1939 DTC.
 * ServiceId           none
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     J1939DTC: Receives the J1939DTC value. If the return value of
                        the function is other than DEM_FILTERED_OK this
                        parameter does not contain valid data.
                        OccurenceCounter:This parameter receives the corresponding occurrence
 counter. If the return value of the function call is other than DEM_FILTERED_OK this parameter does
 not contain valid data.
 * Param-Name[in/out]  none
 * Return              Status of the operation to retrieve a DTC from the Dem.
 */
/*************************************************************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE)
Dem_InterJ1939DcmGetNextFilteredDTC(
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) J1939DTC,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) OccurenceCounter)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    Dem_ReturnGetNextFilteredElementType ret = DEM_FILTERED_NO_MATCHING_ELEMENT;
    const Dem_J1939DTCFilterInfoType* pFilter = &DemJ1939DTCFilter;
    uint16 dtcRef;
    Dem_EventIdType eventId;
    const Dem_EventMemEntryType* pEntry;
    const Dem_EventRelateInformationType* pEvent;
    boolean IsFind;

    pEntry = DemMemDestCfg[pFilter->DTCOrigin].EntryList;
    if (pEntry[pFilter->CurrentIndex].EventId != 0u)
    {
        eventId = Dem_GetEventInternalId(pEntry[pFilter->CurrentIndex].EventId);
        pEvent = Dem_GetEventInfo(eventId);
        dtcRef = DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef;
        if (dtcRef != DEM_DTC_REF_INVALID)
        {
#if (DEM_J1939_NODE_NUM > 0)
            if (Dem_J1939CeckNode(dtcRef, pFilter->node) == TRUE)
#endif
            {
                IsFind = FALSE;
                /*match the DTCStatusFilter*/
                switch (pFilter->DTCStatusFilter)
                {
                /*(ConfirmedDTC == 1 AND TestFailed == 1) OR MIL_ON*/
                case DEM_J1939DTC_ACTIVE:
                    if (((0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_CDTC))
                         && (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF)))
#if (DEM_MALFUNCTION_LAMP_INDICATOR != DemIndicator_INVALID)
                        || ((DemWIRStatus[DEM_MALFUNCTION_LAMP_INDICATOR] != DEM_INDICATOR_OFF))
#endif /* DEM_MALFUNCTION_LAMP_INDICATOR != DemIndicator_INVALID */
                    )
                    {
                        IsFind = TRUE;
                    }
                    break;
                    /*ConfirmedDTC == 1 AND TestFailed == 0 AND MIL_OFF*/
                case DEM_J1939DTC_PREVIOUSLY_ACTIVE:
                    if ((0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_CDTC))
                        && (0x00u == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF))
#if (DEM_MALFUNCTION_LAMP_INDICATOR != DemIndicator_INVALID)
                        && ((DemWIRStatus[DEM_MALFUNCTION_LAMP_INDICATOR] == DEM_INDICATOR_OFF))
#endif /* DEM_MALFUNCTION_LAMP_INDICATOR != DemIndicator_INVALID */
                    )
                    {
                        IsFind = TRUE;
                    }
                    break;
                    /*PendingDTC == 1*/
                case DEM_J1939DTC_PENDING:
                    if (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_PDTC))
                    {
                        IsFind = TRUE;
                    }
                    break;
                case DEM_J1939DTC_PERMANENT:
                    IsFind = TRUE;
                    break;
                    /*TestFailed == 1*/
                case DEM_J1939DTC_CURRENTLY_ACTIVE:
                    if (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF))
                    {
                        IsFind = TRUE;
                    }
                    break;
                default:
                    /*idle*/
                    break;
                }
                /* req SWS_Dem_01101 */
                /* PRQA S 2995++ */ /* MISRA Rule 2.2 */
                if ((DemDTCGeneralStatus[dtcRef].SuppressionStatus != TRUE) && (IsFind == TRUE))
                /* PRQA S 2995-- */ /* MISRA Rule 2.2 */
                {
                    /*guarantee the event index the new dtc */
                    Dem_GetEventDTC(dtcRef, DEM_DTC_FORMAT_J1939, J1939DTC);
                    if ((*J1939DTC) != 0u)
                    {
                        *OccurenceCounter = pEvent->OccurrenceCounter;
                        if (*OccurenceCounter > 0x7Eu)
                        {
                            /* req SWS_Dem_00877 */
                            *OccurenceCounter = 0x7Eu;
                        }
                        ret = DEM_FILTERED_OK;
                    }
                }
            }
        }
    }
    return ret;
}
#endif /* DEM_J1939_READING_DTC_SUPPORT == STD_ON */

#if (DEM_J1939_DM31_SUPPORT == STD_ON)
/*************************************************************************/
/*
 * Brief               The function sets the filter to the first applicable DTC for the DM31
 * response for a specific node ServiceId           none Sync/Async          Synchronous
 * Reentrancy Non Reentrant Param-Name[in] Nm node Id of requesting client Param-Name[out] None
 * Param-Name[in/out]  none Return              Status of the operation to retrieve a DTC from
 * the Dem.
 */
/*************************************************************************/
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(boolean, DEM_CODE)
Dem_InterJ1939DcmFirstDTCwithLampStatus(uint8 nodeId, uint16 dtcRef)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    boolean IsFind;
#if (DEM_J1939_NODE_NUM > 0u)
    if (Dem_J1939CeckNode(dtcRef, nodeId) != TRUE)
    {
        IsFind = FALSE;
    }
    else
#endif /* DEM_J1939_NODE_NUM > 0u */
    {
        IsFind = TRUE;
    }
    return IsFind;
}

/*************************************************************************/
/*
 * Brief               Gets the next filtered J1939 DTC for DM31 including current LampStatus.
 * ServiceId           none
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     J1939DTC:Receives the J1939DTC value. If the return value of
                        the function is other than DEM_FILTERED_OK this
                        parameter does not contain valid data.
                        OccurenceCounter:This parameter receives the corresponding occurrence
 counter. If the return value of the function call is other than DEM_FILTERED_OK this parameter
 does not contain valid data.
 * Param-Name[in/out]  none
 * Return              Status of the operation to retrieve a DTC from the Dem.
 */
/*************************************************************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE)
Dem_InterJ1939DcmGetNextDTCwithLampStatus(
    uint8 nodeId,
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) J1939DTC,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) OccurenceCounter,
    Dem_EventIdType IntId)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    Dem_ReturnGetNextFilteredElementType ret = DEM_FILTERED_NO_MATCHING_ELEMENT;
    const Dem_EventRelateInformationType* pEvent;
    uint16 dtcRef;

    pEvent = Dem_GetEventInfo(IntId);
    dtcRef = DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef;
#if (DEM_J1939_NODE_NUM > 0u)
    if ((dtcRef != DEM_DTC_REF_INVALID) && (Dem_J1939CeckNode(dtcRef, nodeId) == TRUE))
#endif /* DEM_J1939_NODE_NUM > 0u */
    {  /* req SWS_Dem_01101 */
        if (DemDTCGeneralStatus[dtcRef].SuppressionStatus == TRUE)
        {
            ret = DEM_FILTERED_NO_MATCHING_ELEMENT;
        }
        else
        {
            /*guarantee the event index the new dtc */
            Dem_GetEventDTC(dtcRef, DEM_DTC_FORMAT_J1939, J1939DTC);
            if ((*J1939DTC) != 0u)
            {
                *OccurenceCounter = pEvent->OccurrenceCounter;
                if (*OccurenceCounter > 0x7Eu)
                {
                    /* req SWS_Dem_00882 */
                    *OccurenceCounter = 0x7Eu;
                }
                ret = DEM_FILTERED_OK;
            }
        }
    }
    return ret;
}

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
Dem_GetLampStatusOfJ1939DcmGetNextDTCwithLampStatus(
    P2CONST(Dem_EventRelateInformationType, AUTOMATIC, DEM_APPL_DATA) pEvent,
    P2VAR(Dem_J1939DcmLampStatusType, AUTOMATIC, DEM_APPL_DATA) LampStatus)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
#if ((DEM_INDICATOR_NUM > 0) && (DEM_INDICATOR_ATTRIBUTE_TOTAL_NUM > 0))
    uint8 iloop;
    const Dem_IndicatorAttributeType* pWIRAttr =
        &DemIndicatorAttribute[DemPbCfgPtr->DemEventParameter[pEvent->IntId].AttrStartIndex];
    for (iloop = 0; iloop < DemPbCfgPtr->DemEventParameter[pEvent->IntId].AttrNum; iloop++)
    {
        uint8 DemIndicatorRef = pWIRAttr->DemIndicatorRef;
        uint8 DemIndicatorBehaviour = pWIRAttr->DemIndicatorBehaviour;
        /*Malfunction Indicator Lamp*/
        if (DemIndicatorRef == DEM_MALFUNCTION_LAMP_INDICATOR)
        {
            if (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_WIR))
            {
                /* SWS_Dem_00883 */
                *LampStatus |= 0x0040u;
            }
            if (DemWIRStatus[DemIndicatorRef] != DEM_INDICATOR_OFF)
            {
                if (DemIndicatorBehaviour == DEM_INDICATOR_CONTINUOUS)
                {
                    /* SWS_Dem_00884 */
                    *LampStatus |= 0xC000u;
                }
                if (DemIndicatorBehaviour == DEM_INDICATOR_SLOW_FLASH)
                {
                    /* SWS_Dem_00860 */
                    *LampStatus |= 0x0000u;
                }
                if (DemIndicatorBehaviour == DEM_INDICATOR_FAST_FLASH)
                {
                    /* SWS_Dem_00861 */
                    *LampStatus |= 0x4000u;
                }
            }
        }
        /* Red Stop Lamp */
        if (DemIndicatorRef == DEM_RED_STOP_LAMP_INDICATOR)
        {
            if (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_WIR))
            {
                /* SWS_Dem_00887 */
                *LampStatus |= 0x0010u;
            }
            if (DemWIRStatus[DemIndicatorRef] != DEM_INDICATOR_OFF)
            {
                if (DemIndicatorBehaviour == DEM_INDICATOR_CONTINUOUS)
                {
                    /* SWS_Dem_00888 */
                    *LampStatus |= 0x3000u;
                }
                if (DemIndicatorBehaviour == DEM_INDICATOR_SLOW_FLASH)
                {
                    /* SWS_Dem_00889 */
                    *LampStatus |= 0x0000u;
                }
                if (DemIndicatorBehaviour == DEM_INDICATOR_FAST_FLASH)
                {
                    /* SWS_Dem_00890 */
                    *LampStatus |= 0x1000u;
                }
            }
        }
        Dem_SubGetLampStatusOfJ1939DcmGetNextDTCwithLampStatus(pWIRAttr, pEvent, LampStatus);
        pWIRAttr++;
    }
#else
    *LampStatus = 0x00FF;
    DEM_UNUSED(pEvent);
#endif /*DEM_INDICATOR_NUM > 0) && (DEM_INDICATOR_ATTRIBUTE_TOTAL_NUM > 0 */
}
#endif /* DEM_J1939_DM31_SUPPORT == STD_ON */

#if (DEM_J1939_EXPANDED_FREEZE_FRAME_SUPPORT == STD_ON)
#if (DEM_J1939_FREEZE_FRAME_SUPPORT == STD_ON)
/*************************************************************************/
/*
 * Brief               Gets next freeze frame data. The function stores the data in the provided
 DestBuffer
 * ServiceId           None
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     J1939DTC:Receives the J1939DTC value. If the return value of
                        the function is other than DEM_FILTERED_OK this
                        parameter does not contain valid data
                        OccurenceCounter:This parameter receives the corresponding occurrence
 counter. If the return value of the function call is other than DEM_FILTERED_OK this parameter
 does not contain valid data.
 * Param-Name[in/out]  DestBuffer:This parameter contains a byte pointer that points
                        to the buffer, to which the freeze frame data record
                        shall be written to.
                        BufSize:When the function is called this parameter contains
                        the maximum number of data bytes that can be written to the buffer.
 * Return              Status of the operation to retrieve freeze frame data by DTC.
 */
/*************************************************************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE)
Dem_InterJ1939DcmGetNextFreezeFrame(
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) J1939DTC,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) OccurenceCounter,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    Dem_ReturnGetNextFilteredElementType ret = DEM_FILTERED_NO_MATCHING_ELEMENT;
    uint16 dtcRef;
    const Dem_EventMemEntryType* pEntry;
    const Dem_EventRelateInformationType* pEvent;

#if (DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0)
    const uint8* pFF;
#endif
    uint8 iloop;

    pEntry = &DemMemDestCfg[Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY)]
                  .EntryList[DemJ1939DcmFreezeFrameFilter.CurrentIndex];
    if (pEntry->EventId != 0u)
    {
        pEvent = Dem_GetEventInfo(Dem_GetEventInternalId(pEntry->EventId));
        dtcRef = DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef;
#if (DEM_J1939_NODE_NUM > 0u)
        if ((dtcRef != DEM_DTC_REF_INVALID) && (Dem_J1939CeckNode(dtcRef, DemJ1939DcmFreezeFrameFilter.node) == TRUE))
#endif /* DEM_J1939_NODE_NUM > 0u */
        {
            /* req SWS_Dem_01101 */
            if (DemDTCGeneralStatus[dtcRef].SuppressionStatus == TRUE)
            {
                DemJ1939DcmFreezeFrameFilter.CurrentIndex++;
            }
            else
            {
                /*guarantee the event index the new dtc */
                Dem_GetEventDTC(dtcRef, DEM_DTC_FORMAT_J1939, J1939DTC);
                *OccurenceCounter = (pEvent->OccurrenceCounter > 0x7Eu) ? 0x7Eu : pEvent->OccurrenceCounter;
                for (iloop = 0; iloop < DEM_MAX_NUMBER_FF_RECORDS; iloop++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
                {
#if (DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0)
                    uint16 DTCAttRef = DemPbCfgPtr->DemDTC[dtcRef].DemDTCAttributesRef;
                    uint8 ClassRef = DemPbCfgPtr->DemDTCAttributes[DTCAttRef].DemJ1939FreezeFrameClassRef;
                    uint8 ExClassRef = DemPbCfgPtr->DemDTCAttributes[DTCAttRef].DemJ1939ExpandedFreezeFrameClassRef;
                    uint8 FreezeRef = 0u;
                    uint16 DataSize;
                    pFF = pEntry->FFList[iloop].Data;
#endif
                    switch (DemJ1939DcmFreezeFrameFilter.FreezeFrameKind)
                    {
#if (DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0)
                    case DEM_J1939DCM_FREEZEFRAME:
                        if (ClassRef != DEM_J1939_FREEZE_FRAME_INVALID)
                        {
                            DataSize = DemJ1939FreezeFrameClass[ClassRef].DataSize;
                            if (*BufSize < DataSize)
                            {
                                ret = DEM_FILTERED_BUFFER_TOO_SMALL;
                            }
                            else
                            {
                                FreezeRef = ClassRef;
                                *BufSize = DataSize;
                                ret = DEM_FILTERED_OK;
                            }
                        }
                        break;
                    case DEM_J1939DCM_EXPANDED_FREEZEFRAME:
                        if (ExClassRef != DEM_J1939_FREEZE_FRAME_INVALID)
                        {
                            DataSize = DemJ1939FreezeFrameClass[ExClassRef].DataSize;
                            if (*BufSize < DataSize)
                            {
                                ret = DEM_FILTERED_BUFFER_TOO_SMALL;
                            }
                            else
                            {
                                FreezeRef = ExClassRef;
                                *BufSize = DataSize;
                                ret = DEM_FILTERED_OK;
                            }
                        }
                        break;
#endif
                    case DEM_J1939DCM_SPNS_IN_EXPANDED_FREEZEFRAME:
                    default:
                        DEM_DET_REPORT(DEM_SID_J1939DCMGETNEXTFREEZEFRAME, DEM_E_WRONG_CONDITION);
                        break;
                    }
                    if (ret == DEM_FILTERED_OK)
                    {
#if (DEM_DATA_ELEMENT_CLASS_NUM > 0)
                        uint8 offset = 0;
                        for (uint8 SpnRefNum = 0; SpnRefNum < DemJ1939FreezeFrameClass[FreezeRef].RefNum; SpnRefNum++)
                        {
                            uint16 SpnIndex =
                                DemSPNClassRef[DemJ1939FreezeFrameClass[FreezeRef].StartIndex + SpnRefNum];
                            uint16 StartIndex = DemSPNClass[SpnIndex].StartIndex;
                            const DemDataElementClassTypes* pDemDataElementClass =
                                &DemPbCfgPtr->DemDataElementClass[StartIndex];
                            if (pDemDataElementClass->DemDataElementReadFnc != NULL_PTR)
                            {
                                uint8 ElementDataSize = pDemDataElementClass->DemDataElementDataSize;
                                if (FALSE == pDemDataElementClass->DemInterFlag)
                                {
                                    Dem_MemCopy(&DestBuffer[offset], &pFF[offset], ElementDataSize);
                                }
                                else
                                {
                                    /* get inter data now */
                                    pDemDataElementClass->DemDataElementReadFnc(&DestBuffer[offset]);
                                }
                                offset += ElementDataSize;
                            }
                        }
#endif
                    }
                }
            }
        }
    }
    return ret;
}
#endif /* DEM_J1939_FREEZE_FRAME_SUPPORT == STD_ON */

/*************************************************************************/
/*
 * Brief               Gets next SPN.
 * ServiceId           None
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     SPNSupported: This parameter contains the next SPN in the
 ExpandedFreezeFrame SPNDataLength: This parameter contains the corresponding dataLength of the
 SPN
 * Param-Name[in/out]  None
 * Return              Status of the operation to retrieve freeze frame data by DTC.
 */
/*************************************************************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE)
Dem_InterJ1939DcmGetNextSPNInFreezeFrame(
    uint16 dtcRef,
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) SPNSupported,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) SPNDataLength)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    Dem_ReturnGetNextFilteredElementType ret = DEM_FILTERED_NO_MATCHING_ELEMENT;
    uint8 iloop;

#if (DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0)
    uint8 DemJ1939ExFFClassRef = DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].DemDTCAttributesRef]
                                     .DemJ1939ExpandedFreezeFrameClassRef;
#endif

#if (DEM_J1939_NODE_NUM > 0)
    if (Dem_J1939CeckNode(dtcRef, DemJ1939DcmFreezeFrameFilter.node) == TRUE)
#endif
    {
        /* req SWS_Dem_01101 */
        if (DemDTCGeneralStatus[dtcRef].SuppressionStatus == FALSE)
        {
            iloop = 0;
#if (DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0)
            while (iloop < DEM_MAX_NUMBER_FF_RECORDS) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
            {
                if (DemJ1939DcmFreezeFrameFilter.IsOK == FALSE)
                {
                    DemJ1939DcmFreezeFrameFilter.SPNIndex = DemJ1939FreezeFrameClass[DemJ1939ExFFClassRef].StartIndex;
                    DemJ1939DcmFreezeFrameFilter.SPNCounter = 0;
                    DemJ1939DcmFreezeFrameFilter.IsOK = TRUE;
                }
#endif
                switch (DemJ1939DcmFreezeFrameFilter.FreezeFrameKind)
                {
#if (DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0)
                case DEM_J1939DCM_SPNS_IN_EXPANDED_FREEZEFRAME:
                    if (DemJ1939ExFFClassRef != DEM_J1939_FREEZE_FRAME_INVALID)
                    {
                        while (DemJ1939DcmFreezeFrameFilter.SPNCounter
                               < DemJ1939FreezeFrameClass[DemJ1939ExFFClassRef].RefNum)
                        {
                            *SPNSupported = DemSPNClass[DemJ1939DcmFreezeFrameFilter.SPNIndex].DemSPNIdentifier;
                            *SPNDataLength = DemSPNClass[DemJ1939DcmFreezeFrameFilter.SPNIndex].DataSize;
                            DemJ1939DcmFreezeFrameFilter.SPNCounter++;
                            DemJ1939DcmFreezeFrameFilter.SPNIndex++;
                            ret = DEM_FILTERED_OK;
                        }
                    }
                    break;
#endif
                case DEM_J1939DCM_FREEZEFRAME:
                case DEM_J1939DCM_EXPANDED_FREEZEFRAME:
                default:
                    DEM_DET_REPORT(DEM_SID_J1939DCMGETNEXTSPNINFREEZEFRAME, DEM_E_WRONG_CONDITION);
                    break;
                }
                iloop++;
#if (DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0)
            }
#endif
            DemJ1939DcmFreezeFrameFilter.IsFind = TRUE;
        }
    }
    return ret;
}
#endif /* DEM_J1939_EXPANDED_FREEZE_FRAME_SUPPORT == STD_ON */

#if (DEM_J1939_RATIO_SUPPORT == STD_ON)
/*************************************************************************/
/*
 * Brief               The function sets the Ratio filter for a specific node and returns the
 * corresponding Ignition Cycle Counter and General Denominator.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     SPN:Receives the SPN of the applicaple system monitor. If the return
 value of the function is other than DEM_FILTERED_OK this parameter does not contain valid data.
 Numerator:Receives the Numerator of the applicable system monitor. If the return value of the
 function is other than DEM_FILTERED_OK this parameter does not contain valid data.
                        Denominator:Receives the Denominator of the applicable system
                        monitor. If the return value of the function is other
                        than DEM_FILTERED_OK this parameter does not
                        contain valid data.
 * Param-Name[in/out]  None
 * Return              Status of the operation to (re-)set a DTC filter.
 */
/*************************************************************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE)
Dem_InterJ1939DcmGetNextFilteredRatio(
    Dem_EventIdType IntId,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) SPN,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) Numerator,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) Denominator)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    const Dem_EventRelateInformationType* pEvent;
    uint16 dtcRef;

    pEvent = Dem_GetEventInfo(IntId);
    dtcRef = DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef;
    Dem_ReturnGetNextFilteredElementType ret = DEM_FILTERED_NO_MATCHING_ELEMENT;

#if (DEM_J1939_NODE_NUM > 0)
    if ((dtcRef != DEM_DTC_REF_INVALID) && (Dem_J1939CeckNode(dtcRef, DemJ1939DcmRatioFilter.node) == TRUE))
#endif
    {
#if (DEM_RATIO_NUM > 0)
        /* PRQA S 2877++ */ /* MISRA Dir 4.1 */
        for (uint16 Ratioindex = 0; (Ratioindex < DEM_RATIO_NUM) && (ret != DEM_FILTERED_OK); Ratioindex++)
        /* PRQA S 2877-- */ /* MISRA Dir 4.1 */
        {
            /* req SWS_Dem_01104 */
            if ((DemPbCfgPtr->DemRatio[Ratioindex].DemDiagnosticEventRef == pEvent->IntId)
                && (0x00u != DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_AVAILABLE)))
            {
                *SPN = (uint16)(((DemPbCfgPtr->DemObdDTC[DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef].DemJ1939DTCValue
                                  & 0xff0000u)
                                 >> 16u)
                                | (DemPbCfgPtr->DemObdDTC[DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef].DemJ1939DTCValue
                                   & 0x00ff00u));
                /*version 4 shall be used TODO:SPN in 1939-73 is 19 bits, and there only
                 * 16bits to reported*/
                *Numerator = IUMPRValue[Ratioindex].Numerator;
                *Denominator = IUMPRValue[Ratioindex].Denominator;
                ret = DEM_FILTERED_OK;
            }
        }
#endif
    }
    return ret;
}
#endif /* DEM_J1939_RATIO_SUPPORT == STD_ON */

#if (DEM_J1939_READINESS1_SUPPORT == STD_ON)
/*************************************************************************/
/*
 * Brief               Service to report the value of Diagnostic Readiness 1 (DM05) computed by
 * the Dem. ServiceId           None Sync/Async          Synchronous Reentrancy          Non
 * Reentrant Param-Name[in]      node:Nm node Id of requesting client Param-Name[out]
 * DataValue:Buffer of 8 bytes containing the contents of Diagnostic Readiness 1 (DM05) computed
 * by the Dem. Param-Name[in/out]  None Return              None
 */
/*************************************************************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(void, DEM_CODE)
Dem_InterOneJ1939DcmReadDiagnosticReadiness1(
    Dem_EventIdType IntId,
    P2VAR(Dem_J1939DcmDiagnosticReadiness1Type, AUTOMATIC, DEM_APPL_DATA) DataValue,
    uint8 node)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    const Dem_EventRelateInformationType* pEvent;
    uint16 dtcRef;

    pEvent = Dem_GetEventInfo(IntId);
    dtcRef = DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef;
#if (DEM_J1939_NODE_NUM > 0)
    if ((dtcRef != DEM_DTC_REF_INVALID) && (Dem_J1939CeckNode(dtcRef, node) == TRUE))
#endif
    {
        if (((0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_CDTC))
             && (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF)))
#if (DEM_MALFUNCTION_LAMP_INDICATOR != DemIndicator_INVALID)
            || ((DemWIRStatus[DEM_MALFUNCTION_LAMP_INDICATOR] != DEM_INDICATOR_OFF))
#endif /* DEM_MALFUNCTION_LAMP_INDICATOR != DemIndicator_INVALID */
        )
        {
            DataValue->ActiveTroubleCodes += 1u;
        }
        if ((0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_CDTC))
            && (0x00u == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF))
#if (DEM_MALFUNCTION_LAMP_INDICATOR != DemIndicator_INVALID)
            && ((DemWIRStatus[DEM_MALFUNCTION_LAMP_INDICATOR] == DEM_INDICATOR_OFF))
#endif /* DEM_MALFUNCTION_LAMP_INDICATOR != DemIndicator_INVALID */
        )
        {
            DataValue->PreviouslyActiveDiagnosticTroubleCodes += 1u;
        }
    }
}

/*************************************************************************/
/*
 * Brief               Service to report the value of Diagnostic Readiness 1 (DM05) computed by
 the Dem.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      node:Nm node Id of requesting client
 * Param-Name[out]     DataValue:Buffer of 8 bytes containing the contents of Diagnostic
 Readiness 1 (DM05) computed by the Dem.
 * Param-Name[in/out]  None
 * Return              E_OK: Operation was successful
                        E_NOT_OK: Operation failed
 */
/*************************************************************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(void, DEM_CODE)
Dem_InterTwoJ1939DcmReadDiagnosticReadiness1(
    uint8 node,
    uint8 ReadinessGroupIndex,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) supportedGroups,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) notCompletedGroups)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    uint16 iloop;
    uint16 dtcRef;
    uint16 obdDtcRef;
    uint32 J1939Dtc;
    const Dem_DTCType* pDTC = DemPbCfgPtr->DemDTC;
    const DemObdDTCType* pObdDTC = DemPbCfgPtr->DemObdDTC;

    for (iloop = 0; iloop < DemPbCfgPtr->DemEventNum; iloop++)
    {
        dtcRef = DemPbCfgPtr->DemEventParameter[iloop].DemDTCRef;
        /* req SWS_Dem_01101 */
        if ((DEM_DTC_REF_INVALID != dtcRef) && (DemDTCGeneralStatus[dtcRef].SuppressionStatus != TRUE)
            && (pDTC[dtcRef].DemObdDTCRef != DEM_OBD_DTC_INVALID))
        {
            obdDtcRef = pDTC[dtcRef].DemObdDTCRef;
            if (pObdDTC[obdDtcRef].DemEventOBDReadinessGroup == ReadinessGroupIndex)
            {
                J1939Dtc = pObdDTC[obdDtcRef].DemJ1939DTCValue;
                /* req SWS_Dem_01101 */
                if (J1939Dtc != 0UL)
                {
#if (DEM_J1939_NODE_NUM > 0)
                    if (Dem_J1939CeckNode(dtcRef, node) == TRUE)
#endif
                    {
                        DEM_BITS_SET(supportedGroups, ReadinessGroupIndex);
                        if (((0x00u == DEM_FLAGS_ISSET(DemEventRelateInformation[iloop].UdsStatus, DEM_UDS_STATUS_TF))
                             && (0x00u
                                 == DEM_FLAGS_ISSET(DemEventRelateInformation[iloop].UdsStatus, DEM_UDS_STATUS_TNCSLC)))
                            || (0x00u
                                != DEM_FLAGS_ISSET(DemEventRelateInformation[iloop].UdsStatus, DEM_UDS_STATUS_WIR)))
                        {
                            /*idle*/
                        }
                        else
                        {
                            DEM_BITS_SET(notCompletedGroups, ReadinessGroupIndex);
                        }
                    }
                }
            }
        }
    }
}
#endif /* DEM_J1939_READINESS1_SUPPORT == STD_ON */
#endif /* DEM_J1939_SUPPORT == STD_ON */

/*************************************************************************/
/*
 * Brief               BRIEF DESCRIPTION
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant/Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(void, DEM_CODE)
Dem_TriggerOnEventStatus(Dem_EventIdType IntId, Dem_UdsStatusByteType OldStatus, Dem_UdsStatusByteType NewStatus)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
#if (DEM_CALLBACK_EVENT_STATUS_CHANGED_TOTAL_NUM > 0)
    const Dem_EventParameterType* pEventCfg = &DemPbCfgPtr->DemEventParameter[IntId];
    const Dem_TriggerOnEventStatusType* pCbk;
#else
    DEM_UNUSED(OldStatus);
    DEM_UNUSED(NewStatus);
#endif

#if (DEM_TRIGGER_DCM_REPORTS == STD_ON)
    uint32 DTC = 0x00;
#else
    DEM_UNUSED(IntId);
#endif
    /*SWS_Dem_00828]*/
    if (Dem_DTCStatusChangedInd == TRUE)
    {
#if (DEM_TRIGGER_DCM_REPORTS == STD_ON)
        uint16 dtcRef = DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef;
        /*SWS_Dem_00284]*/
        Dem_GetEventDTC(dtcRef, DEM_DTC_FORMAT_UDS, &DTC);
        if (DTC != 0x00UL)
        {
            (void)Dcm_DemTriggerOnDTCStatus(DTC, OldStatus, NewStatus);
#if (DEM_CALLBACK_DTC_STATUS_CHANGED_NUM > 0)
            /* PRQA S 2877++ */ /* MISRA Dir 4.1 */
            for (uint8 iloop = 0; iloop < DEM_CALLBACK_DTC_STATUS_CHANGED_NUM; iloop++)
            /* PRQA S 2877-- */ /* MISRA Dir 4.1 */
            {
                if (DemCallbackDTCStatusChanged[iloop] != NULL_PTR)
                {
                    (void)(DemCallbackDTCStatusChanged[iloop])(DTC, OldStatus, NewStatus);
                }
            }
#endif /* DEM_CALLBACK_DTC_STATUS_CHANGED_NUM > 0u */
        }
        /*SWS_Dem_00986]*/
        Dem_GetEventDTC(dtcRef, DEM_DTC_FORMAT_OBD, &DTC);
        if (DTC != 0x00UL)
        {
#if (DEM_CALLBACK_OBD_DTC_STATUS_CHANGED_NUM > 0) /* PRQA S 3332 */ /* MISRA Rule 20.9 */
            for (uint8 iloop = 0; iloop < DEM_CALLBACK_OBD_DTC_STATUS_CHANGED_NUM; iloop++)
            {
                if (DemCallbackObdDTCStatusChanged[iloop] != NULL_PTR)
                {
                    (void)(DemCallbackObdDTCStatusChanged[iloop])(DTC, OldStatus, NewStatus);
                }
            }
#endif
        }
        /*SWS_Dem_00987]*/
        Dem_GetEventDTC(dtcRef, DEM_DTC_FORMAT_J1939, &DTC);
        if (DTC != 0x00UL)
        {
#if (DEM_CALLBACK_J1939_DTC_STATUS_CHANGED_NUM > 0)
            for (uint8 iloop = 0; iloop < DEM_CALLBACK_J1939_DTC_STATUS_CHANGED_NUM; iloop++)
            {
                if (DemCallbackJ1939DTCStatusChanged[iloop] != NULL_PTR)
                {
                    (void)(DemCallbackJ1939DTCStatusChanged[iloop])(DTC, OldStatus, NewStatus);
                }
            }
#endif
        }
#endif
    }
    /*SWS_Dem_00016] */
#if (DEM_CALLBACK_EVENT_STATUS_CHANGED_TOTAL_NUM > 0)
    for (uint8 iloop = 0; iloop < pEventCfg->StatusChangedCbkNum; iloop++)
    {
        pCbk = &DemCallbackEventStatusChanged[pEventCfg->StatusChangedCbkStartIndex + iloop];
        if (*pCbk != NULL_PTR)
        {
            (void)(*pCbk)(IntId + 1u, OldStatus, NewStatus);
        }
    }
#endif
}

/*************************************************************************/
/*
 * Brief               ClearAllDTC
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant/Non Reentrant
 * Param-Name[in]      memDest
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
FUNC(void, DEM_CODE) Dem_ClearAllDTC(uint8 memDest) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint16 iloop;
    uint16 dtcRef;
    Dem_EventRelateInformationType* pEvent;
    const Dem_ClearDTCInfoType* pClr = &DemClearDTCInfo;
    const Dem_EventParameterType* pEventParameter = DemPbCfgPtr->DemEventParameter;
    const DemObdDTCType* pObdDTC = DemPbCfgPtr->DemObdDTC;

    for (iloop = 0; iloop < DemPbCfgPtr->DemEventNum; iloop++)
    {
        boolean DoClear = TRUE;
        pEvent = &DemEventRelateInformation[iloop];
        dtcRef = pEventParameter[pEvent->IntId].DemDTCRef;
        const uint16 ObdDTCRef = DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef;
        if ((ObdDTCRef != DEM_OBD_DTC_INVALID) && (pObdDTC[ObdDTCRef].DemJ1939DTCValue != DEM_J1939_DTC_CFG_INVALID))
        {
            DoClear = FALSE;
#if (DEM_J1939_NODE_NUM > 0u)
            if ((pClr->SID == DEM_SID_J1939DCMCLEARDTC)
                && (((pClr->DTCTypeFilter == DEM_J1939DTC_CLEAR_PREVIOUSLY_ACTIVE)
                     && ((0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_CDTC))
                         && (0x00u == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF))))
                    || ((pClr->DTCTypeFilter == DEM_J1939DTC_CLEAR_ALL)
                        && (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF)))))
            {
                DoClear = Dem_J1939CeckNode(dtcRef, pClr->node);
            }
            else
#endif /* DEM_J1939_NODE_NUM > 0u */
                if (pClr->SID != DEM_SID_J1939DCMCLEARDTC)
                {
                    DoClear = TRUE;
                }
                else
                {
                    /* do nothing */
                }
        }
        if (DoClear == TRUE)
        {
            Dem_Clear(pEvent, memDest);
        }
        /* req SWS_Dem_00399 */
        if (DemMemDestInfo[memDest].RecordNum == 0u)
        {
            DemMemDestInfo[memDest].OverFlow = FALSE;
        }
#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
        /* Clear counters if all OBD DTCs have been cleared */
        WarmUpCycleCounter = 0;
        OBDDistanceDTCClear = Dem_ReadDistanceInformation();
        OBDTimeDTCLastClear = Dem_ReadTimeInformation();
        OBDTimeDTCClear = 0;
        /* req SWS_Dem_01146 */
        ContinuousMICounter = 0;
        OBDB1Counter = 0;
        OBDDistanceMILOn = 0;
#endif
    }
}

#if (DEM_CLEAR_DTCLIMITATION == DEM_ALL_SUPPORTED_DTCS)
/*************************************************************************/
/*
 * Brief               ClearGroupDTC
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant/Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
FUNC(void, DEM_CODE) Dem_ClearGroupDTC(uint8 memDest, uint8 GroupIndex) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (DEM_GROUP_OF_DTC_NUM > 0)
    uint16 iloop;
    uint16 Ref;
    Dem_EventRelateInformationType* pEvent;

    for (iloop = 0; iloop < DemPbCfgPtr->DemEventNum; iloop++)
    {
        Ref = DemPbCfgPtr->DemEventParameter[iloop].DemDTCRef;
        pEvent = &DemEventRelateInformation[iloop];
        if ((Ref != DEM_DTC_REF_INVALID) && (DemPbCfgPtr->DemDTC[Ref].GroupRef == GroupIndex)
            && (0x00u != DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_ENABLED_CONDICTION)))
        {
            Dem_Clear(pEvent, memDest);
        }
    }
    /* req SWS_Dem_00399 */
    if (DemMemDestInfo[memDest].RecordNum == 0u)
    {
        DemMemDestInfo[memDest].OverFlow = FALSE;
    }
#else
    DEM_UNUSED(memDest);
    DEM_UNUSED(GroupIndex);
#endif
}
#endif /* DEM_CLEAR_DTCLIMITATION == DEM_ALL_SUPPORTED_DTCS */

/*************************************************************************/
/*
 * Brief               Dem_ClearDTRInfoByEventId
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
FUNC(void, DEM_CODE) Dem_ClearDTRInfoByEventId(Dem_EventIdType IntId)
{
#if (DEM_DTR_NUM > 0)
    /* req SWS_Dem_00763 */
    const DTRType* pDTR = DemPbCfgPtr->DTR;
    for (uint16 iloop = 0; iloop < DEM_DTR_NUM; iloop++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
    {
        if (IntId == pDTR[iloop].DemDtrEventRef)
        {
            Dem_MemSet((uint8*)&DTRInfo[iloop], 0u, sizeof(DTRInfoType));
        }
    }
#else
    DEM_UNUSED(IntId);
#endif
}

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
