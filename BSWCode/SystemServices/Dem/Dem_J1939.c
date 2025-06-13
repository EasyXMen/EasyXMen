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
**  FILENAME    : Dem_J1939.c                                                 **
**                                                                            **
**  Created on  :                                                             **
**  Author      : tao.yu                                                      **
**  Vendor      : i-soft                                                      **
**  DESCRIPTION : API definitions of DEM for J1939                            **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Dem_Internal.h"

#if (DEM_J1939_SUPPORT == STD_ON)
/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
/* PRQA S 1514++ */ /* MISRA Rule 8.9 */
#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
VAR(Dem_J1939DcmFreezeFrameFilterType, AUTOMATIC) DemJ1939DcmFreezeFrameFilter;
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
VAR(Dem_J1939DcmRatioFilterType, AUTOMATIC) DemJ1939DcmRatioFilter;
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
VAR(Dem_J1939DTCFilterInfoType, AUTOMATIC) DemJ1939DTCFilter;
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

#if (DEM_J1939_DM31_SUPPORT == STD_ON)
#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
VAR(DemJ1939DTCGetNodeType, AUTOMATIC) DemJ1939DTCGetNode;
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
#endif
/* PRQA S 1514-- */ /* MISRA Rule 8.9 */
/*******************************************************************************
**                      Private Function Definitions                         **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/*************************************************************************/
/*
 * Brief               Init the J1939 related data.
 * ServiceId           Internal Function
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
FUNC(void, DEM_CODE) Dem_J1939Init(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    Dem_MemSet((uint8*)&DemJ1939DTCFilter, 0x00u, sizeof(Dem_J1939DTCFilterInfoType));
    Dem_MemSet((uint8*)&DemJ1939DcmFreezeFrameFilter, 0x00u, sizeof(Dem_J1939DcmFreezeFrameFilterType));
    Dem_MemSet((uint8*)&DemJ1939DcmRatioFilter, 0x00u, sizeof(Dem_J1939DcmRatioFilterType));
}

#if (DEM_J1939_NODE_NUM > 0u)
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
FUNC(boolean, DEM_CODE) Dem_J1939CeckNode(uint16 dtcRef, uint8 node)
{
    boolean ret = FALSE;
    uint8 J1939NodeRef =
        DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].DemDTCAttributesRef].DemJ1939DTC_J1939NodeRef;
    const J1939NodeType* pJ1939Node = &DemPbCfgPtr->J1939Node[J1939NodeRef];
    const uint8* nodeIndex = pJ1939Node->NodeIDRef;
    uint8 nodeNum = pJ1939Node->NodeNum;
    for (uint8 index = 0; index < nodeNum; index++)
    {
        if (node == *nodeIndex)
        {
            /* find the node */
            ret = TRUE;
        }
        nodeIndex++;
    }
    return ret;
}
#endif /* DEM_J1939_NODE_NUM > 0u */

#if (DEM_J1939_READING_DTC_SUPPORT == STD_ON)
/*************************************************************************/
/*
 * Brief               The function sets the DTC filter for a specific node and returns the
 composite lamp status of the filtered DTCs
 * ServiceId           0x90
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTCStatusFilter: The following types are available:
                        DEM_J1939DTC_ACTIVE
                        DEM_J1939DTC_PREVIOUSLY_ACTIVE
                        DEM_J1939DTC_PENDING
                        DEM_J1939DTC_PERMANENT
                        DEM_J1939DTC_CURRENTLY_ACTIVE
 *                      DTCKind: Defines the functional group of DTCs to be reported
 *                      node:Nm node Id
 * Param-Name[out]     LampStatus:Receives the commulated lamp status
 * Param-Name[in/out]  none
 * Return              Status of the operation to (re-)set a DTC filter.
 */
/*************************************************************************/
FUNC(Dem_ReturnSetFilterType, DEM_CODE)
Dem_J1939DcmSetDTCFilter(
    Dem_J1939DcmDTCStatusFilterType DTCStatusFilter,
    Dem_DTCKindType DTCKind,
    uint8 node,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dem_J1939DcmLampStatusType, AUTOMATIC, DEM_APPL_DATA) LampStatus)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Dem_ReturnSetFilterType ret = DEM_WRONG_FILTER;
    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMSETDTCFILTER, DEM_E_UNINIT);
    }
    else if (LampStatus == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMSETDTCFILTER, DEM_E_PARAM_POINTER);
    }
    else
    {
        DemJ1939DTCFilter.DTCStatusFilter = DTCStatusFilter;
        DemJ1939DTCFilter.DemJ1939DTCFilterInfo = DTCKind;
        DemJ1939DTCFilter.node = node;
        /* find the DTCOrigin */
        if (DTCStatusFilter == DEM_J1939DTC_PERMANENT)
        {
            DemJ1939DTCFilter.DTCOrigin = Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PERMANENT_MEMORY);
        }
        else
        {
            DemJ1939DTCFilter.DTCOrigin = Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY);
        }
        DemJ1939DTCFilter.CurrentIndex = 0;
        DemJ1939DTCFilter.NumberOfFilteredDTC = 0;
        DemJ1939DTCFilter.GetNum = 0;
        DemJ1939DTCFilter.IsSet = TRUE;
        Dem_GetLampStatusOfJ1939DcmSetDTCFilter(LampStatus);
        ret = DEM_FILTER_ACCEPTED;
    }
    return ret;
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
FUNC(Dem_ReturnGetNumberOfFilteredDTCType, DEM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
Dem_J1939DcmGetNumberOfFilteredDTC(P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) NumberOfFilteredDTC)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Dem_ReturnGetNumberOfFilteredDTCType ret = DEM_NUMBER_FAILED;
    const Dem_EventMemEntryType* pEntry;
    const Dem_EventRelateInformationType* pEvent;
    const Dem_J1939DTCFilterInfoType* pFilter = &DemJ1939DTCFilter;
    uint8 iloop;
    uint8 entryNum;
    uint16 dtcRef;
    boolean findSameDTC = FALSE;
    boolean IsFind = FALSE;

    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMGETNUMBEROFFILTEREDDTC, DEM_E_UNINIT);
    }
    else if (NumberOfFilteredDTC == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMGETNUMBEROFFILTEREDDTC, DEM_E_PARAM_POINTER);
    }
    else if (pFilter->IsSet == FALSE)
    {
        /*idle*/
    }
    else
    {
        entryNum = DemMemDestCfg[pFilter->DTCOrigin].EntryNum;
        for (iloop = 0; iloop < entryNum; iloop++)
        {
            pEntry = &DemMemDestCfg[pFilter->DTCOrigin].EntryList[iloop];
            if (pEntry->EventId != 0x00u)
            {
                pEvent = Dem_GetEventInfo(Dem_GetEventInternalId(pEntry->EventId));
                dtcRef = DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef;
                const Dem_DTCType* pDTC = &DemPbCfgPtr->DemDTC[dtcRef];
                const DemObdDTCType* pObdDTC = DemPbCfgPtr->DemObdDTC;
                uint16 DemObdDTCRef = pDTC->DemObdDTCRef;
                /*match kind and J1939DTC*/
                if ((DEM_DTC_REF_INVALID != dtcRef) && (DemObdDTCRef != DEM_OBD_DTC_INVALID)
                    && (DemDTCGeneralStatus[dtcRef].SuppressionStatus == FALSE) /* SWS_Dem_01101 */
                    && (pObdDTC[DemObdDTCRef].DemJ1939DTCValue != DEM_J1939_DTC_CFG_INVALID)
                    && ((DEM_DTC_KIND_ALL_DTCS == pFilter->DemJ1939DTCFilterInfo)
                        || (pFilter->DemJ1939DTCFilterInfo == pDTC->DTCKind)))
                {
                    Dem_InterJ1939DcmGetNumberOfFilteredDTC(
                        Dem_GetEventInternalId(pEntry->EventId),
                        iloop,
                        &findSameDTC,
                        &IsFind);
                }
            }
        }
        *NumberOfFilteredDTC = pFilter->NumberOfFilteredDTC;
        ret = DEM_NUMBER_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets the next filtered J1939 DTC.
 * ServiceId           0x92
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
FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE)
Dem_J1939DcmGetNextFilteredDTC(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) J1939DTC,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) OccurenceCounter)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Dem_ReturnGetNextFilteredElementType ret = DEM_FILTERED_NO_MATCHING_ELEMENT;
    Dem_J1939DTCFilterInfoType* pFilter = &DemJ1939DTCFilter;
    uint8 entryNum;
    const Dem_EventMemEntryType* pEntry;

    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMGETNEXTFILTEREDDTC, DEM_E_UNINIT);
    }
    else if ((OccurenceCounter == NULL_PTR) || (J1939DTC == NULL_PTR))
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMGETNEXTFILTEREDDTC, DEM_E_PARAM_POINTER);
    }
    else if (Dem_Pending == TRUE)
    {
        /* req SWS_Dem_00877 */
        ret = DEM_FILTERED_PENDING;
    }
    else
    {
        entryNum = DemMemDestCfg[pFilter->DTCOrigin].EntryNum;

        /* PRQA S 2472++ */ /* MISRA Rule 14.2 */
        for (; (ret != DEM_FILTERED_OK) && (pFilter->CurrentIndex < entryNum); pFilter->CurrentIndex++)
        /* PRQA S 2472-- */ /* MISRA Rule 14.2 */
        {
            pEntry = &DemMemDestCfg[pFilter->DTCOrigin].EntryList[pFilter->CurrentIndex];
            if (pEntry->EventId != 0x00u)
            {
                ret = Dem_InterJ1939DcmGetNextFilteredDTC(J1939DTC, OccurenceCounter);
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
 * response for a specific node ServiceId           0x93 Sync/Async          Synchronous Reentrancy
 * Non Reentrant Param-Name[in] Nm node Id of requesting client Param-Name[out]     None
 * Param-Name[in/out]  none Return              Status of the operation to retrieve a DTC from the
 * Dem.
 */
/*************************************************************************/
FUNC(void, DEM_CODE) Dem_J1939DcmFirstDTCwithLampStatus(uint8 node)
{
    const Dem_EventMemEntryType* pEntry;
    uint8 entryNum = DemMemDestCfg[Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY)].EntryNum;
    const Dem_EventRelateInformationType* pEvent;
    uint16 dtcRef;
    uint8 iloop;
    boolean IsFind = FALSE;

    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMFIRSTDTCWITHLAMPSTATUS, DEM_E_UNINIT);
    }
    else
    {
        DemJ1939DTCGetNode.node = node;
        DemJ1939DTCGetNode.IsSet = TRUE;
        for (iloop = 0; (iloop < entryNum) && (IsFind == FALSE); iloop++)
        {
            pEntry = &DemMemDestCfg[Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY)].EntryList[iloop];
            if (pEntry->EventId != 0x00u)
            {
                pEvent = Dem_GetEventInfo(Dem_GetEventInternalId(pEntry->EventId));
                dtcRef = DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef;
                const DemObdDTCType* pObdDTC = DemPbCfgPtr->DemObdDTC;
                uint16 DemObdDTCRef = DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef;
                /*match kind and J1939DTC*/
                if ((DEM_DTC_REF_INVALID != dtcRef) && (DemObdDTCRef != DEM_OBD_DTC_INVALID)
                    && (pObdDTC[DemObdDTCRef].DemJ1939DTCValue != DEM_J1939_DTC_CFG_INVALID)
                    && (DemDTCGeneralStatus[dtcRef].SuppressionStatus == FALSE)) /* SWS_Dem_01101 */
                {
                    if (TRUE == Dem_InterJ1939DcmFirstDTCwithLampStatus(DemJ1939DTCGetNode.node, dtcRef))
                    {
                        DemJ1939DTCGetNode.CurrentIndex = iloop;
                        IsFind = TRUE;
                    }
                }
            }
        }
    }
}

/*************************************************************************/
/*
 * Brief               Gets the next filtered J1939 DTC for DM31 including current LampStatus.
 * ServiceId           0x94
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     LampStatus:Receives the lamp status returned by this function.
 *                      If the return value of the function is other than
                        DEM_FILTERED_OK this parameter does not contain valid data.
                        J1939DTC:Receives the J1939DTC value. If the return value of
                        the function is other than DEM_FILTERED_OK this
                        parameter does not contain valid data.
                        OccurenceCounter:This parameter receives the corresponding occurrence
 counter. If the return value of the function call is other than DEM_FILTERED_OK this parameter does
 not contain valid data.
 * Param-Name[in/out]  none
 * Return              Status of the operation to retrieve a DTC from the Dem.
 */
/*************************************************************************/
FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE)
Dem_J1939DcmGetNextDTCwithLampStatus(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dem_J1939DcmLampStatusType, AUTOMATIC, DEM_APPL_DATA) LampStatus,
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) J1939DTC,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) OccurenceCounter)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Dem_ReturnGetNextFilteredElementType ret = DEM_FILTERED_NO_MATCHING_ELEMENT;
    uint8 entryNum = DemMemDestCfg[Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY)].EntryNum;
    uint16 dtcRef;
    Dem_EventIdType IntId;
    const Dem_EventMemEntryType* pEntry;
    const Dem_EventRelateInformationType* pEvent = NULL_PTR;

    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMGETNEXTDTCWITHLAMPSTATUS, DEM_E_UNINIT);
    }
    else if ((OccurenceCounter == NULL_PTR) || (J1939DTC == NULL_PTR) || (LampStatus == NULL_PTR))
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMGETNEXTDTCWITHLAMPSTATUS, DEM_E_PARAM_POINTER);
    }
    else if (Dem_Pending == TRUE)
    {
        /* req SWS_Dem_00882 */
        ret = DEM_FILTERED_PENDING;
    }
    else
    {
        /* PRQA S 2461++ */ /* MISRA Rule 14.2 */
        for (; (ret != DEM_FILTERED_OK) && (DemJ1939DTCGetNode.IsSet == TRUE)
               && (DemJ1939DTCGetNode.CurrentIndex < entryNum);
             DemJ1939DTCGetNode.CurrentIndex++)
        /* PRQA S 2461-- */ /* MISRA Rule 14.2 */
        {
            pEvent = NULL_PTR;
            pEntry = &DemMemDestCfg[Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY)]
                          .EntryList[DemJ1939DTCGetNode.CurrentIndex];
            if (pEntry->EventId != 0x00u)
            {
                IntId = Dem_GetEventInternalId(pEntry->EventId);
                pEvent = Dem_GetEventInfo(IntId);
                dtcRef = DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef;
                const DemObdDTCType* pObdDTC = DemPbCfgPtr->DemObdDTC;
                uint16 DemObdDTCRef = DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef;
                /*match kind and J1939DTC*/
                if ((DEM_DTC_REF_INVALID != dtcRef) && (DemObdDTCRef != DEM_OBD_DTC_INVALID)
                    && (pObdDTC[DemObdDTCRef].DemJ1939DTCValue != DEM_J1939_DTC_CFG_INVALID))
                {
                    ret = Dem_InterJ1939DcmGetNextDTCwithLampStatus(
                        DemJ1939DTCGetNode.node,
                        J1939DTC,
                        OccurenceCounter,
                        IntId);
                }
            }
        }
        *LampStatus = 0u;
        if (pEvent != NULL_PTR)
        {
            Dem_GetLampStatusOfJ1939DcmGetNextDTCwithLampStatus(pEvent, LampStatus);
        }
    }
    return ret;
}
#endif /* DEM_J1939_DM31_SUPPORT == STD_ON */

#if (DEM_J1939_CLEAR_DTC_SUPPORT == STD_ON)
/*************************************************************************/
/*
 * Brief               Clears active DTCs as well as previously active DTCs
 * ServiceId           0x95
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTCTypeFilter:The following types are available:
                        DEM_J1939DTC_CLEAR_ALL
                        DEM_J1939DTC_CLEAR_PREVIOUSLY_ACTIVE
 *                      node: Nm node Id of requesting client
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Status of the operation of type Dem_ReturnClearDTCType.
 */
/*************************************************************************/
FUNC(Dem_ReturnClearDTCType, DEM_CODE)
Dem_J1939DcmClearDTC(Dem_J1939DcmSetClearFilterType DTCTypeFilter, uint8 node)
{
    Dem_ReturnClearDTCType ret = DEM_CLEAR_FAILED;
    Dem_ClearDTCInfoType* pClr;
    boolean ProcessOn = TRUE;

    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMCLEARDTC, DEM_E_UNINIT);
    }
    else
    {
        pClr = &DemClearDTCInfo;
        if (Dem_ClearDTCLock == DEM_CLEAR_LOCK)
        {
            if ((pClr->DTCTypeFilter == DTCTypeFilter) && (pClr->SID == DEM_SID_J1939DCMCLEARDTC)
                && (pClr->node == node))
            {
                /* req SWS_Dem_00663 */
                ret = DEM_CLEAR_PENDING;
                ProcessOn = FALSE;
            }
            else
            {
                /* req SWS_Dem_00662 req SWS_Dem_00664 */
                ret = DEM_CLEAR_BUSY;
                ProcessOn = FALSE;
            }
        }
        else if (Dem_ClearDTCLock == DEM_CLEAR_NOT_LOCK)
        {
            /* req SWS_Dem_00661 */
            Dem_ClearDTCLock = DEM_CLEAR_LOCK;
            pClr->DTCTypeFilter = DTCTypeFilter;
            pClr->SID = DEM_SID_J1939DCMCLEARDTC;
            pClr->node = node;
        }
        else
        {
            /*idle*/
        }
        if (ProcessOn == TRUE)
        {
            pClr->ClearAllGroup = TRUE;
            pClr->memDest = Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY);
            Dem_Pending = TRUE;
            Dem_ClearDTCProcess();
            Dem_ResetDemClearDTCInfo();
            ret = DEM_CLEAR_OK;
        }
    }
    return ret;
}
#endif /* DEM_J1939_CLEAR_DTC_SUPPORT == STD_ON */

#if (DEM_J1939_EXPANDED_FREEZE_FRAME_SUPPORT == STD_ON)
#if (DEM_J1939_FREEZE_FRAME_SUPPORT == STD_ON)
/*************************************************************************/
/*
 * Brief               The function sets the FreezeFrame filter for a specific node
 * ServiceId           0x96
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      FreezeFrameKind: The following types are available:
                        DEM_J1939DCM_FREEZEFRAME
                        DEM_J1939DCM_EXPANDED_FREEZEFRAME
                        DEM_J1939DCM_SPNS_IN_EXPANDED_
                        FREEZEFRAME
                        node:Nm node Id of requesting client
 * Param-Name[out]     None
 * Param-Name[in/out]  none
 * Return              Status of the operation to (re-)set a FreezeFrame filter.
 */
/*************************************************************************/
FUNC(Dem_ReturnSetFilterType, DEM_CODE)
Dem_J1939DcmSetFreezeFrameFilter(Dem_J1939DcmSetFreezeFrameFilterType FreezeFrameKind, uint8 node)
{
    Dem_ReturnSetFilterType ret = DEM_WRONG_FILTER;

    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMSETFREEZEFRAMEFILTER, DEM_E_UNINIT);
    }
    else
    {
        DemJ1939DcmFreezeFrameFilter.IsSet = TRUE;
        DemJ1939DcmFreezeFrameFilter.FreezeFrameKind = FreezeFrameKind;
        DemJ1939DcmFreezeFrameFilter.node = node;
        DemJ1939DcmFreezeFrameFilter.SPNIndex = 0;
        DemJ1939DcmFreezeFrameFilter.CurrentIndex = 0;
        ret = DEM_FILTER_ACCEPTED;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets next freeze frame data. The function stores the data in the provided
 DestBuffer
 * ServiceId           0x97
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     J1939DTC:Receives the J1939DTC value. If the return value of
                        the function is other than DEM_FILTERED_OK this
                        parameter does not contain valid data
                        OccurenceCounter:This parameter receives the corresponding occurrence
 counter. If the return value of the function call is other than DEM_FILTERED_OK this parameter does
 not contain valid data.
 * Param-Name[in/out]  DestBuffer:This parameter contains a byte pointer that points
                        to the buffer, to which the freeze frame data record
                        shall be written to.
                        BufSize:When the function is called this parameter contains
                        the maximum number of data bytes that can be written to the buffer.
 * Return              Status of the operation to retrieve freeze frame data by DTC.
 */
/*************************************************************************/
FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE)
Dem_J1939DcmGetNextFreezeFrame(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) J1939DTC,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) OccurenceCounter,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Dem_ReturnGetNextFilteredElementType ret = DEM_FILTERED_NO_MATCHING_ELEMENT;
    uint8 entryNum = DemMemDestCfg[Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY)].EntryNum;
    uint16 dtcRef;
    Dem_EventIdType eventId;
    const Dem_EventMemEntryType* pEntry;
    const Dem_EventRelateInformationType* pEvent;

    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMGETNEXTFREEZEFRAME, DEM_E_UNINIT);
    }
    else if (
        (OccurenceCounter == NULL_PTR) || (J1939DTC == NULL_PTR) || (DestBuffer == NULL_PTR) || (BufSize == NULL_PTR))
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMGETNEXTFREEZEFRAME, DEM_E_PARAM_POINTER);
    }
    else if (Dem_Pending == TRUE)
    {
        /* req SWS_Dem_00902 */
        ret = DEM_FILTERED_PENDING;
    }
    else
    {
        Dem_MemSet(DestBuffer, 0u, *BufSize);
        /* PRQA S 2461++ */ /* MISRA Rule 14.2 */
        for (; (ret == DEM_FILTERED_NO_MATCHING_ELEMENT) && (DemJ1939DcmFreezeFrameFilter.IsSet == TRUE)
               && (DemJ1939DcmFreezeFrameFilter.CurrentIndex < entryNum);
             DemJ1939DcmFreezeFrameFilter.CurrentIndex++)
        /* PRQA S 2461-- */ /* MISRA Rule 14.2 */
        {
            pEntry = &DemMemDestCfg[Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY)]
                          .EntryList[DemJ1939DcmFreezeFrameFilter.CurrentIndex];
            if (pEntry->EventId != 0x00u)
            {
                if (pEntry->FFNum == 0u)
                {
                    ret = DEM_E_NODATAAVAILABLE;
                }
                eventId = Dem_GetEventInternalId(pEntry->EventId);
                pEvent = Dem_GetEventInfo(eventId);
                dtcRef = DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef;
                const DemObdDTCType* pObdDTC = DemPbCfgPtr->DemObdDTC;
                uint16 DemObdDTCRef = DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef;
                /* match J1939DTC */
                if ((DEM_DTC_REF_INVALID != dtcRef) && (DemObdDTCRef != DEM_OBD_DTC_INVALID)
                    && (pObdDTC[DemObdDTCRef].DemJ1939DTCValue != DEM_J1939_DTC_CFG_INVALID))
                {
                    ret = Dem_InterJ1939DcmGetNextFreezeFrame(J1939DTC, OccurenceCounter, DestBuffer, BufSize);
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
 * ServiceId           0x98
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     SPNSupported: This parameter contains the next SPN in the ExpandedFreezeFrame
                        SPNDataLength: This parameter contains the corresponding dataLength of the
 SPN
 * Param-Name[in/out]  None
 * Return              Status of the operation to retrieve freeze frame data by DTC.
 */
/*************************************************************************/
FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE)
Dem_J1939DcmGetNextSPNInFreezeFrame(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) SPNSupported,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) SPNDataLength)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Dem_ReturnGetNextFilteredElementType ret = DEM_FILTERED_NO_MATCHING_ELEMENT;
    uint8 entryNum = DemMemDestCfg[Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY)].EntryNum;
    uint16 dtcRef;
    Dem_EventIdType eventId;
    const Dem_EventMemEntryType* pEntry;
    const Dem_EventRelateInformationType* pEvent;

    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMGETNEXTSPNINFREEZEFRAME, DEM_E_UNINIT);
    }
    else if ((SPNSupported == NULL_PTR) || (SPNDataLength == NULL_PTR))
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMGETNEXTSPNINFREEZEFRAME, DEM_E_PARAM_POINTER);
    }
    else if (Dem_Pending == TRUE)
    {
        /* req SWS_Dem_00905 */
        ret = DEM_FILTERED_PENDING;
    }
    else
    {
        *SPNSupported = 0;
        *SPNDataLength = 0;
        /* PRQA S 2461++ */ /* MISRA Rule 14.2 */
        for (; (ret == DEM_FILTERED_NO_MATCHING_ELEMENT) && (DemJ1939DcmFreezeFrameFilter.IsSet == TRUE)
               && (DemJ1939DcmFreezeFrameFilter.CurrentIndex < entryNum);
             DemJ1939DcmFreezeFrameFilter.CurrentIndex++)
        /* PRQA S 2461-- */ /* MISRA Rule 14.2 */
        {
            pEntry = &DemMemDestCfg[Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY)]
                          .EntryList[DemJ1939DcmFreezeFrameFilter.CurrentIndex];
            if (pEntry->EventId != 0x00u)
            {
                if (pEntry->FFNum == 0u)
                {
                    ret = DEM_E_NODATAAVAILABLE;
                }
                eventId = Dem_GetEventInternalId(pEntry->EventId);
                pEvent = Dem_GetEventInfo(eventId);
                dtcRef = DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef;
                const DemObdDTCType* pObdDTC = DemPbCfgPtr->DemObdDTC;
                uint16 DemObdDTCRef = DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef;
                /* match J1939DTC */
                if ((DEM_DTC_REF_INVALID != dtcRef) && (DemObdDTCRef != DEM_OBD_DTC_INVALID)
                    && (pObdDTC[DemObdDTCRef].DemJ1939DTCValue != DEM_J1939_DTC_CFG_INVALID))
                {
                    ret = Dem_InterJ1939DcmGetNextSPNInFreezeFrame(dtcRef, SPNSupported, SPNDataLength);
                }
            }
            DemJ1939DcmFreezeFrameFilter.IsFind = FALSE;
            DemJ1939DcmFreezeFrameFilter.IsOK = FALSE;
            DemJ1939DcmFreezeFrameFilter.SPNCounter = 0;
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
 * ServiceId           0x99
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      node:Nm node Id of requesting client
 * Param-Name[out]     IgnitionCycleCounter: Ignition Cycle Counter
                        OBDMonitoringConditionsEncountered: OBD Monitoring Conditions Encountered
 * Param-Name[in/out]  None
 * Return              Status of the operation to (re-)set a DTC filter.
 */
/*************************************************************************/
FUNC(Dem_ReturnSetFilterType, DEM_CODE)
Dem_J1939DcmSetRatioFilter(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) IgnitionCycleCounter,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) OBDMonitoringConditionsEncountered,
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    uint8 node)
{
    Dem_ReturnSetFilterType ret = DEM_WRONG_FILTER;

    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMSETRATIOFILTER, DEM_E_UNINIT);
    }
    else if ((IgnitionCycleCounter == NULL_PTR) || (OBDMonitoringConditionsEncountered == NULL_PTR))
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMSETRATIOFILTER, DEM_E_PARAM_POINTER);
    }
    else
    {
#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
        *IgnitionCycleCounter = IgnUpCycleCounter;
#endif
#if (DEM_RATIO_NUM > 0)
        *OBDMonitoringConditionsEncountered = IUMPRValue[0].General_Denominator;
#endif
        DemJ1939DcmRatioFilter.node = node;
        DemJ1939DcmRatioFilter.IsSet = TRUE;
        DemJ1939DcmRatioFilter.CurrentIndex = 0;
        ret = DEM_FILTER_ACCEPTED;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               The function sets the Ratio filter for a specific node and returns the
 * corresponding Ignition Cycle Counter and General Denominator.
 * ServiceId           0x9a
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     SPN:Receives the SPN of the applicaple system monitor. If the return value of
 the function is other than DEM_FILTERED_OK this parameter does not contain valid data.
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
FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE)
Dem_J1939DcmGetNextFilteredRatio(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) SPN,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) Numerator,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) Denominator)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    const Dem_EventMemEntryType* pEntry;
    uint8 entryNum = DemMemDestCfg[Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY)].EntryNum;
    const Dem_EventRelateInformationType* pEvent;
    uint16 dtcRef;
    Dem_ReturnGetNextFilteredElementType ret = DEM_FILTERED_NO_MATCHING_ELEMENT;

    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMGETNEXTFILTEREDRATIO, DEM_E_UNINIT);
    }
    else if ((SPN == NULL_PTR) || (Numerator == NULL_PTR) || (Denominator == NULL_PTR))
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMGETNEXTFILTEREDRATIO, DEM_E_PARAM_POINTER);
    }
    else if (Dem_Pending == TRUE)
    {
        /* req SWS_Dem_00913 */
        ret = DEM_FILTERED_PENDING;
    }
    else
    {
        /* PRQA S 2461++ */ /* MISRA Rule 14.2 */
        for (; (DemJ1939DcmRatioFilter.CurrentIndex < entryNum) && (ret != DEM_FILTERED_OK)
               && (DemJ1939DcmRatioFilter.IsSet == TRUE);
             DemJ1939DcmRatioFilter.CurrentIndex++)
        /* PRQA S 2461-- */ /* MISRA Rule 14.2 */
        {
            pEntry = &DemMemDestCfg[Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY)]
                          .EntryList[DemJ1939DcmRatioFilter.CurrentIndex];
            if (pEntry->EventId != 0x00u)
            {
                pEvent = Dem_GetEventInfo(Dem_GetEventInternalId(pEntry->EventId));
                dtcRef = DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef;
                const DemObdDTCType* pObdDTC = DemPbCfgPtr->DemObdDTC;
                uint16 DemObdDTCRef = DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef;
                /*match kind and J1939DTC*/
                if ((DEM_DTC_REF_INVALID != dtcRef) && (DemObdDTCRef != DEM_OBD_DTC_INVALID)
                    && (pObdDTC[DemObdDTCRef].DemJ1939DTCValue != DEM_J1939_DTC_CFG_INVALID))
                {
                    /* req SWS_Dem_01101 */
                    if (DemDTCGeneralStatus[dtcRef].SuppressionStatus == FALSE)
                    {
                        ret = Dem_InterJ1939DcmGetNextFilteredRatio(pEvent->IntId, SPN, Numerator, Denominator);
                    }
                }
            }
        }
    }
    return ret;
}
#endif /* DEM_J1939_RATIO_SUPPORT == STD_ON */

#if (DEM_J1939_READINESS1_SUPPORT == STD_ON)
/*************************************************************************/
/*
 * Brief               Service to report the value of Diagnostic Readiness 1 (DM05) computed by the
 Dem.
 * ServiceId           0x9b
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      node:Nm node Id of requesting client
 * Param-Name[out]     DataValue:Buffer of 8 bytes containing the contents of Diagnostic Readiness 1
 (DM05) computed by the Dem.
 * Param-Name[in/out]  None
 * Return              E_OK: Operation was successful
                        E_NOT_OK: Operation failed
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_J1939DcmReadDiagnosticReadiness1(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dem_J1939DcmDiagnosticReadiness1Type, AUTOMATIC, DEM_APPL_DATA) DataValue,
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    uint8 node)
{
    const Dem_EventMemEntryType* pEntry;
    const Dem_EventRelateInformationType* pEvent;
    uint8 iloop;
    uint8 entryNum;
    uint16 dtcRef;
    uint8 ReadinessGroupIndex;
    uint8 supportedGroups[3];
    uint8 notCompletedGroups[3];
    Std_ReturnType ret = E_NOT_OK;

    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMREADDIAGNOSTICREADINESS1, DEM_E_UNINIT);
    }
    else if (DataValue == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMREADDIAGNOSTICREADINESS1, DEM_E_PARAM_POINTER);
    }
    else
    {
        Dem_MemSet(supportedGroups, 0u, (uint8)3);
        Dem_MemSet(notCompletedGroups, 0u, (uint8)3);
        DataValue->ActiveTroubleCodes = 0;
        DataValue->ContinuouslyMonitoredSystemsSupport_Status = 0;
        DataValue->NonContinuouslyMonitoredSystemsStatus = 0;
        DataValue->NonContinuouslyMonitoredSystemsSupport = 0;
        DataValue->PreviouslyActiveDiagnosticTroubleCodes = 0;
#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
        DataValue->OBDCompliance = DemGeneralOBD.DemOBDCompliancy;
#else
        DataValue->OBDCompliance = 0xff;
#endif
        entryNum = DemMemDestCfg[Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY)].EntryNum;
        for (iloop = 0; iloop < entryNum; iloop++)
        {
            pEntry = &DemMemDestCfg[Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY)].EntryList[iloop];
            if (pEntry->EventId != 0x00u)
            {
                pEvent = Dem_GetEventInfo(Dem_GetEventInternalId(pEntry->EventId));
                dtcRef = DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef;
                const DemObdDTCType* pObdDTC = DemPbCfgPtr->DemObdDTC;
                uint16 DemObdDTCRef = DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef;
                /* match J1939DTC */
                if ((DEM_DTC_REF_INVALID != dtcRef) && (DemObdDTCRef != DEM_OBD_DTC_INVALID)
                    && (pObdDTC[DemObdDTCRef].DemJ1939DTCValue != DEM_J1939_DTC_CFG_INVALID))
                {
                    /* req SWS_Dem_01101 */
                    if (DemDTCGeneralStatus[dtcRef].SuppressionStatus != TRUE)
                    {
                        Dem_InterOneJ1939DcmReadDiagnosticReadiness1(pEvent->IntId, DataValue, node);
                    }
                }
            }
        }
        for (ReadinessGroupIndex = 0; ReadinessGroupIndex < DEM_NUMBER_OF_READINESS_GROUPS_FORJ1939;
             ReadinessGroupIndex++)
        {
            Dem_InterTwoJ1939DcmReadDiagnosticReadiness1(
                node,
                ReadinessGroupIndex,
                supportedGroups,
                notCompletedGroups);
        }
        DataValue->ContinuouslyMonitoredSystemsSupport_Status =
            ((supportedGroups[0u] & 0xFu) | ((uint8)((notCompletedGroups[0u] & 0xFu) << 4u)));
        DataValue->NonContinuouslyMonitoredSystemsSupport =
            (((uint16)(((uint16)supportedGroups[0] & 0xF0u) >> 4u)) | (uint16)(((uint16)supportedGroups[1u]) << 4u)
             | (uint16)((((uint16)supportedGroups[2u]) << 12u)));
        DataValue->NonContinuouslyMonitoredSystemsStatus =
            ((((uint16)notCompletedGroups[0u] & 0xF0u) >> 4u) | (uint16)(((uint16)notCompletedGroups[1u]) << 4u)
             | ((uint16)(((uint16)notCompletedGroups[2u]) << 12u)));
        ret = E_OK;
    }
    return ret;
}
#endif /* DEM_J1939_READINESS1_SUPPORT == STD_ON */

#if (DEM_J1939_READINESS2_SUPPORT == STD_ON)
/*************************************************************************/
/*
 * Brief               Service to report the value of Diagnostic Readiness 2 (DM21) computed by the
 Dem.
 * ServiceId           0x9c
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      node:Nm node Id of requesting client
 * Param-Name[out]     DataValue:Buffer of 8 bytes containing the contents of Diagnostic Readiness 2
 (DM21) computed by the Dem.
 * Param-Name[in/out]  None
 * Return              E_OK: Operation was successful
                        E_NOT_OK: Operation failed
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_J1939DcmReadDiagnosticReadiness2(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dem_J1939DcmDiagnosticReadiness2Type, AUTOMATIC, DEM_APPL_DATA) DataValue,
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    uint8 node)
{
#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
    uint16 DistWithMilOn = 0U;
    uint16 currentDistanceInformation;
    uint16 TimeWithMilOn = 0U;
    uint16 CurrentTimeInformation;
#endif
    Std_ReturnType ret = E_NOT_OK;

    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMREADDIAGNOSTICREADINESS2, DEM_E_UNINIT);
    }
    else if (DataValue == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMREADDIAGNOSTICREADINESS2, DEM_E_PARAM_POINTER);
    }
    else
    {
        DataValue->DistanceSinceDTCsCleared = 0;
        DataValue->DistanceTraveledWhileMILisActivated = 0;
        DataValue->MinutesRunbyEngineWhileMILisActivated = 0;
        DataValue->TimeSinceDiagnosticTroubleCodesCleared = 0;
        DEM_UNUSED(node);
#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
        /* MinutesRunbyEngineWhileMILisActivated */
        /*the indicator is on*/
        if (DEM_INDICATOR_OFF != OBDMilStatus)
        {
            CurrentTimeInformation = Dem_ReadTimeInformation();
            if (CurrentTimeInformation > OBDTimeMILLastOn)
            {
                TimeWithMilOn = CurrentTimeInformation - OBDTimeMILLastOn;
                OBDTimeMILLastOn = CurrentTimeInformation;
            }
        }
        if ((OBDTimeMILOn + TimeWithMilOn) < 0xFFFFu)
        {
            OBDTimeMILOn += TimeWithMilOn;
        }
        else
        {
            OBDTimeMILOn = 0xFFFFu;
        }
        /*J1939-73*/
        if (OBDTimeMILOn < 0xFAFFu)
        {
            DataValue->MinutesRunbyEngineWhileMILisActivated = OBDTimeMILOn;
        }
        else
        {
            DataValue->MinutesRunbyEngineWhileMILisActivated = 0xFAFFu;
        }
        /*TimeSinceDiagnosticTroubleCodesCleared*/
        CurrentTimeInformation = Dem_ReadDistanceInformation();
        if (CurrentTimeInformation > OBDTimeDTCLastClear)
        {
            OBDTimeDTCClear = CurrentTimeInformation - OBDTimeDTCLastClear;
        }
        if (OBDTimeDTCClear < 0xFAFFu)
        {
            DataValue->TimeSinceDiagnosticTroubleCodesCleared = OBDTimeDTCClear;
        }
        else
        {
            DataValue->TimeSinceDiagnosticTroubleCodesCleared = 0xFAFFu;
        }
        /*DistanceSinceDTCsCleared*/
        currentDistanceInformation = Dem_ReadDistanceInformation();
        if (currentDistanceInformation > OBDDistanceDTCClear)
        {
            DistSinceDtcCleared = currentDistanceInformation - OBDDistanceDTCClear;
        }
        if (DistSinceDtcCleared < 0xFAFFu)
        {
            DataValue->DistanceSinceDTCsCleared = DistSinceDtcCleared;
        }
        else
        {
            DataValue->DistanceSinceDTCsCleared = 0xFAFFu;
        }
        /*DistanceTraveledWhileMILisActivated*/
        /*the indicator is on*/
        if (DEM_INDICATOR_OFF != OBDMilStatus)
        {
            currentDistanceInformation = Dem_ReadDistanceInformation();
            if (currentDistanceInformation > OBDDistanceMILLastOn)
            {
                DistWithMilOn = currentDistanceInformation - OBDDistanceMILLastOn;
                OBDDistanceMILLastOn = currentDistanceInformation;
            }
        }
        if ((OBDDistanceMILOn + DistWithMilOn) < 0xFFFFu)
        {
            OBDDistanceMILOn += DistWithMilOn;
        }
        else
        {
            OBDDistanceMILOn = 0xFFFFu;
        }
        if (OBDDistanceMILOn < 0xFAFFu)
        {
            DataValue->DistanceTraveledWhileMILisActivated = OBDDistanceMILOn;
        }
        else
        {
            DataValue->DistanceTraveledWhileMILisActivated = 0xFAFFu;
        }
        ret = E_OK;
#endif
    }
    return ret;
}
#endif /* DEM_J1939_READINESS2_SUPPORT == STD_ON */

#if (DEM_J1939_READINESS3_SUPPORT == STD_ON)
/*************************************************************************/
/*
 * Brief               Service to report the value of Diagnostic Readiness 3 (DM26) computed by the
 Dem.
 * ServiceId           0x9d
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      node:Nm node Id of requesting client
 * Param-Name[out]     DataValue:Buffer of 8 bytes containing the contents of Diagnostic Readiness 3
 (DM26) computed by the Dem.
 * Param-Name[in/out]  None
 * Return              E_OK: Operation was successful
                        E_NOT_OK: Operation failed
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_J1939DcmReadDiagnosticReadiness3(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dem_J1939DcmDiagnosticReadiness3Type, AUTOMATIC, DEM_APPL_DATA) DataValue,
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    uint8 node)
{
    uint16 iloop;
    uint16 ReadinessGroupIndex;
    uint8 enabledGroups[3];
    uint8 notCompletedGroups[3];
    uint32 J1939Dtc;
    uint16 obdDtcRef;
    uint16 tempRef;
    const Dem_EventRelateInformationType* pEvent;
    Std_ReturnType ret = E_NOT_OK;
    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMREADDIAGNOSTICREADINESS3, DEM_E_UNINIT);
    }
    else if (DataValue == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMREADDIAGNOSTICREADINESS3, DEM_E_PARAM_POINTER);
    }
    else
    {
        DEM_UNUSED(node);
        Dem_MemSet(enabledGroups, 0xffu, (uint8)3);
        Dem_MemSet(notCompletedGroups, 0u, (uint8)3);
#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
        DataValue->TimeSinceEngineStart = Dem_ReadTimeInformation();
        DataValue->NumberofWarmupsSinceDTCsCleared = WarmUpCycleCounter;
#else
        DataValue->NumberofWarmupsSinceDTCsCleared = 0;
        DataValue->TimeSinceEngineStart = 0;
#endif
        for (ReadinessGroupIndex = 0; ReadinessGroupIndex < DEM_NUMBER_OF_READINESS_GROUPS_FORJ1939;
             ReadinessGroupIndex++)
        {
            for (iloop = 0; iloop < DemPbCfgPtr->DemEventNum; iloop++)
            {
                tempRef = DemPbCfgPtr->DemEventParameter[iloop].DemDTCRef;
                if (DEM_DTC_REF_INVALID != tempRef)
                {
                    obdDtcRef = DemPbCfgPtr->DemDTC[tempRef].DemObdDTCRef;
                    const DemObdDTCType* pObdDTC = &DemPbCfgPtr->DemObdDTC[obdDtcRef];
                    /* req SWS_Dem_01101 */
                    if ((DemDTCGeneralStatus[tempRef].SuppressionStatus != TRUE) && (obdDtcRef != DEM_OBD_DTC_INVALID)
                        && ((pObdDTC->DemEventOBDReadinessGroup == ReadinessGroupIndex)))
                    {
                        /* Only include emission related DTCs */
                        J1939Dtc = pObdDTC->DemJ1939DTCValue;
                        if (J1939Dtc != 0UL)
                        {
                            pEvent = Dem_GetEventInfo(iloop);
                            uint8 DemOperationCycleRef = DemPbCfgPtr->DemEventParameter[iloop].DemOperationCycleRef;
                            if ((DEM_BITS_ISSET(DemOperationCycleStatus, DemOperationCycleRef))
                                && (DemPbCfgPtr->DemOperationCycle[DemOperationCycleRef].DemOperationCycleType
                                    == DEM_OPCYC_POWER))
                            {
                                if (0x00u == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_ENABLED_CONDICTION))
                                {
                                    /* req SWS_Dem_00356  req SWS_Dem_00348 */
                                    DEM_BITS_CLR(enabledGroups, ReadinessGroupIndex);
                                }
                            }
                            if (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TNCTOC))
                            {
                                /* req SWS_Dem_00355 */
                                DEM_BITS_SET(notCompletedGroups, ReadinessGroupIndex);
                            }
                        }
                    }
                }
            }
        }
        DataValue->ContinuouslyMonitoredSystemsEnableCompletedStatus =
            ((enabledGroups[0u] & 0xFu) | (uint8)((notCompletedGroups[0u] & 0xFu) << 4u));
        DataValue->NonContinuouslyMonitoredSystemsEnableStatus =
            ((uint16)(((uint16)enabledGroups[0u] & 0xF0u) >> 4u) | (uint16)((((uint16)enabledGroups[1u]) << 4u))
             | (uint16)((((uint16)enabledGroups[2u]) << 12u)));
        DataValue->NonContinuouslyMonitoredSystems =
            ((uint16)(((uint16)notCompletedGroups[0u] & 0xF0u) >> 4u) | (uint16)(((uint16)notCompletedGroups[1u]) << 4u)
             | (uint16)(((uint16)notCompletedGroups[2u]) << 12u));
        ret = E_OK;
    }
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif /* DEM_J1939_READINESS3_SUPPORT == STD_ON */

#endif /* DEM_J1939_SUPPORT == STD_ON */
/*******************************************************************************
**                      end of file                                           **
*******************************************************************************/
