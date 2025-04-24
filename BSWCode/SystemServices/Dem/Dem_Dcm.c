/* PRQA S 3108++ */
/**
 * Copyright (C) 2024 Isoft Infrastructure Software Co., Ltd.
 * SPDX-License-Identifier: LGPL-2.1-only-with-exception OR  LicenseRef-Commercial-License
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the
 * GNU Lesser General Public License as published by the Free Software Foundation; version 2.1.
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License along with this library;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * or see <https://www.gnu.org/licenses/>.
 *
 * Alternatively, this file may be used under the terms of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License, in which case the provisions of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License shall apply instead of those of the GNU Lesser General Public License.
 *
 * You should have received a copy of the Isoft Infrastructure Software Co., Ltd.  Commercial License
 * along with this program. If not, please find it at <https://EasyXMen.com/xy/reference/permissions.html>
 */
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : Dem_Dcm.c                                                   **
**                                                                            **
**  Created on  :                                                             **
**  Author      : tao.yu                                                      **
**  Vendor      : i-soft                                                      **
**  DESCRIPTION : API definitions of DEM for DCM                              **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Dem_Internal.h"

/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/
typedef struct
{
    uint16 DTCIndex;
    Dem_DTCOriginType DTCOrigin;
} Dem_DTCRecordUpdateStatusInfoType;

#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
VAR(Dem_DTCFilterInfoType, AUTOMATIC) DemDTCFilterInfo;
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
VAR(Dem_DTCSettingInfoType, AUTOMATIC) DemDTCSettingInfo;
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
VAR(Dem_DTCByOccurrenceTimeType, AUTOMATIC) DemDTCByOccurrenceTimeInfo;
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
DEM_LOCAL VAR(Dem_DTCRecordUpdateStatusInfoType, AUTOMATIC) DTCRecordUpdateStatusInfo;
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
VAR(Dem_FreezeFrameRecordFilterInfoType, AUTOMATIC) DemFreezeFrameRecordFilterInfo;
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_POWER_ON_INIT_BOOLEAN
#include "Dem_MemMap.h"
VAR(boolean, AUTOMATIC) Dem_DTCStatusChangedInd = FALSE;
#define DEM_STOP_SEC_VAR_POWER_ON_INIT_BOOLEAN
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_POWER_ON_INIT_BOOLEAN
#include "Dem_MemMap.h"
DEM_LOCAL VAR(boolean, AUTOMATIC) Dem_DTCDisable = FALSE;
#define DEM_STOP_SEC_VAR_POWER_ON_INIT_BOOLEAN
#include "Dem_MemMap.h"

/*******************************************************************************
**                      Private Function Definitions                         **
*******************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"

#if (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_ON)
DEM_LOCAL FUNC(uint8, DEM_CODE) Dem_CheckEventInternalMemDest(Dem_DTCOriginType DTCOrigin, uint16 IntId);
#endif
DEM_LOCAL FUNC(boolean, DEM_CODE) Dem_CheckDTCFormat(uint8 dtcFormat, uint16 dtcRef);
DEM_LOCAL FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE) Dem_DcmCheckDTCFilterStatus(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2CONST(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
    P2CONST(Dem_UdsStatusByteType, AUTOMATIC, DEM_APPL_DATA) DTCStatus);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
DEM_LOCAL FUNC(Dem_ReturnClearDTCType, DEM_CODE)
    Dem_DcmSubClearDTC(uint32 DTC, Dem_DTCFormatType DTCFormat, Dem_DTCOriginType DTCOrigin);

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/

/*************************************************************************/
/*
 * Brief               Gets the supported DTC formats of the ECU.
 * ServiceId           0x3c
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Returns the configured DTC translation format. Acombination of
 *                              different DTC formats is not possible.
 */
/*************************************************************************/
FUNC(Dem_DTCTranslationFormatType, DEM_CODE) Dem_DcmGetTranslationType(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETTRANSLATIONTYPE, DEM_E_UNINIT);
    }
    return DEM_TYPE_OF_DTCSUPPORTED;
}

/*************************************************************************/
/*
 * Brief               Gets the DTC Status availability mask.
 * ServiceId           0x16
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     DTCStatusMask:The value DTCStatusMask indicates the supported DTC status
 *                              bits from the Dem.
 * Param-Name[in/out]  none
 * Return              E_OK: get of DTC status mask was successful
 *                       E_NOT_OK: get of DTC status mask failed
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
/* PRQA S 3432,1532++ */ /* MISRA Rule 20.7,8.7 */
Dem_DcmGetDTCStatusAvailabilityMask(P2VAR(Dem_UdsStatusByteType, AUTOMATIC, DEM_APPL_DATA) DTCStatusMask)
/* PRQA S 3432,1532-- */ /* MISRA Rule 20.7,8.7 */
{
    Std_ReturnType retVal = E_NOT_OK;

    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETDTCSTATUSAVAILABILITYMASK, DEM_E_UNINIT);
    }
    else if (DTCStatusMask == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETDTCSTATUSAVAILABILITYMASK, DEM_E_PARAM_POINTER);
    }
    else
    {
        *DTCStatusMask = DEM_DTC_STATUS_AVAILABILITY_MASK;
        retVal = E_OK;
    }
    return retVal;
}

/*************************************************************************/
/*
 * Brief               Gets the status of a DTC.
 * ServiceId           0x15
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTC:Diagnostic Trouble Code in UDS format.
 *                       DTCOrigin: If the Dem supports more than one event memory this
 *                              parameter is used to select the source memory
 *                              the DTCs shall be read from.
 * Param-Name[out]     DTCStatus: This parameter receives the status information of the
 *                          requested DTC. If the return value of the function call is
 *                          other than DEM_STATUS_OK this parameter does not contain valid
 *                          data. 0x00...0xFF match DTCStatusMask as defined in ISO14229-1
 * Param-Name[in/out]  none
 * Return              Status of the operation of type Dem_ReturnGetStatusOfDTCType.
 */
/*************************************************************************/
FUNC(Dem_ReturnGetStatusOfDTCType, DEM_CODE)
/* PRQA S 3432,1532++ */ /* MISRA Rule 20.7,8.7 */
Dem_DcmGetStatusOfDTC(uint32 DTC, Dem_DTCOriginType DTCOrigin, P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DTCStatus)
/* PRQA S 3432,1532-- */ /* MISRA Rule 20.7,8.7 */
{
    uint8 memDest;
    uint16 dtcIndex;
    Dem_ReturnGetStatusOfDTCType ret = DEM_STATUS_FAILED;

    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETSTATUSOFDTC, DEM_E_UNINIT);
    }
    else if (DTCStatus == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETSTATUSOFDTC, DEM_E_PARAM_POINTER);
    }
    else
    {
        /* req SWS_Dem_01100 */
        dtcIndex = Dem_GetDTCIndex(DTC, DEM_DTC_FORMAT_UDS);
        if (dtcIndex == DEM_DTC_REF_INVALID)
        {
            ret = DEM_STATUS_WRONG_DTC;
        }
        else if (DemDTCGeneralStatus[dtcIndex].SuppressionStatus == TRUE)
        {
            ret = DEM_STATUS_WRONG_DTC;
        }
        else
        {
            memDest = Dem_GetInternalMemDest(DTCOrigin);
            if (memDest == DEM_MEM_DEST_INVALID)
            {
                ret = DEM_STATUS_WRONG_DTCORIGIN;
            }
            else
            {
                /* PRQA S 2985++ */ /* MISRA Rule 2.2 */
                *DTCStatus = (DemDTCGeneralStatus[dtcIndex].CbUdsStatus & DEM_DTC_STATUS_AVAILABILITY_MASK);
                /* PRQA S 2985-- */ /* MISRA Rule 2.2 */
                ret = DEM_STATUS_OK;
            }
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets the severity of the requested DTC.
                        For large configurations and DTC-calibration, the interface behavior can
                        be asynchronous (splitting the DTC-search into segments).
 * ServiceId           0x0e
 * Sync/Async          Synchronous or Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTC : Diagnostic Trouble Code in UDS format.
 * Param-Name[out]     DTCSeverity:This parameter contains the DTCSeverity according to ISO 14229-1.
 * Param-Name[in/out]  none
 * Return              Status of the operation of type Dem_ReturnGetSeverityOfDTCType.
 */
/*************************************************************************/
FUNC(Dem_ReturnGetSeverityOfDTCType, DEM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
Dem_DcmGetSeverityOfDTC(uint32 DTC, P2VAR(Dem_DTCSeverityType, AUTOMATIC, DEM_APPL_DATA) DTCSeverity)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint16 dtcIndex;
    Dem_ReturnGetSeverityOfDTCType ret = DEM_GET_SEVERITYOFDTC_WRONG_DTC;
    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETSEVERITYOFDTC, DEM_E_UNINIT);
    }
    else if (DTCSeverity == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETSEVERITYOFDTC, DEM_E_PARAM_POINTER);
    }
    else
    {
        /* req SWS_Dem_01100 */
        dtcIndex = Dem_GetDTCIndex(DTC, DEM_DTC_FORMAT_UDS);
        if ((dtcIndex != DEM_DTC_REF_INVALID) && (DemDTCGeneralStatus[dtcIndex].SuppressionStatus == FALSE))
        {
            *DTCSeverity = DemPbCfgPtr->DemDTC[dtcIndex].DemDTCSeverity;
            ret = DEM_GET_SEVERITYOFDTC_OK;
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets the functional unit of the requested DTC.
 * ServiceId           0x34
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTC:Diagnostic Trouble Code in UDS format.
 * Param-Name[out]     DTCFunctionalUnit:Functional unit value of this DTC
 * Param-Name[in/out]  none
 * Return              Status of the operation of type Dem_ReturnGetFunctionalUnitOfDTCType.
 */
/*************************************************************************/
FUNC(Dem_ReturnGetFunctionalUnitOfDTCType, DEM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
Dem_DcmGetFunctionalUnitOfDTC(uint32 DTC, P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DTCFunctionalUnit)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint16 dtcIndex;
    Dem_ReturnGetFunctionalUnitOfDTCType ret = DEM_GET_FUNCTIONALUNITOFDTC_WRONG_DTC;
    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETFUNCTIONALUNITOFDTC, DEM_E_UNINIT);
    }
    else if (DTCFunctionalUnit == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETFUNCTIONALUNITOFDTC, DEM_E_PARAM_POINTER);
    }
    else
    {
        /* req SWS_Dem_01100 */
        dtcIndex = Dem_GetDTCIndex(DTC, DEM_DTC_FORMAT_UDS);
        if ((dtcIndex != DEM_DTC_REF_INVALID) && (DemDTCGeneralStatus[dtcIndex].SuppressionStatus == FALSE))
        {
            /* req SWS_Dem_00593 */
            *DTCFunctionalUnit = DemPbCfgPtr->DemDTC[dtcIndex].DemDTCFunctionalUnit;
            ret = DEM_GET_FUNCTIONALUNITOFDTC_OK;
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Sets the DTC Filter.
 * ServiceId           0x13
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTCStatusMask:Status-byte mask for DTC status-byte filtering Values
 *                       DTCKind: Defines the functional group of DTCs to be reported
 *                       DTCFormat: Defines the output-format of the requested DTC values for the
 *                              sub-sequent API calls.
 *                       DTCOrigin: If the Dem supports more than one event memory this parameter
 *                              is used to select the source memory the DTCs shall be read from.
 *                       FilterWithSeverity: This flag defines whether severity information (ref.
 *                              to parameter below) shall be used for filtering
 *                       DTCSeverityMask: This parameter contains the DTCSeverityMask according
 *                              to ISO14229-1
 *                       FilterForFaultDetectionCounter: This flag defines whether the fault
 *                              detection counter information shall be used for filtering.
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Status of the operation to (re-)set a DTC filter.
 */
/*************************************************************************/
FUNC(Dem_ReturnSetFilterType, DEM_CODE)
Dem_DcmSetDTCFilter(
    Dem_UdsStatusByteType DTCStatusMask,
    Dem_DTCKindType DTCKind,
    Dem_DTCFormatType DTCFormat,
    Dem_DTCOriginType DTCOrigin,
    boolean FilterWithSeverity,
    Dem_DTCSeverityType DTCSeverityMask,
    boolean FilterForFaultDetectionCounter)
{
    Dem_ReturnSetFilterType ret = DEM_WRONG_FILTER;
    uint8 memDest = Dem_GetInternalMemDest(DTCOrigin);
    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_DCMSETDTCFILTER, DEM_E_UNINIT);
    }
    else if (
        ((DEM_DTC_FORMAT_OBD == DTCFormat) && (DEM_OBD_SUPPORT == DEM_OBD_NO_OBD_SUPPORT))
        || ((DEM_DTC_FORMAT_J1939 == DTCFormat) && (DEM_J1939_SUPPORT == STD_OFF))
        || ((DEM_DTC_REF_EMISSION_NUM == 0u) && (DTCKind == DEM_DTC_KIND_EMISSION_REL_DTCS))
        || (memDest == DEM_MEM_DEST_INVALID))
    {
        DEM_DET_REPORT(DEM_SID_DCMSETDTCFILTER, DEM_E_WRONG_CONFIGURATION);
    }
    else
    {
        /* req SWS_Dem_01058 */
        /* PRQA S 2985++ */ /* MISRA Rule 2.2 */
        DemDTCFilterInfo.DTCStatusMask = (0u == DTCStatusMask) ? (0xFFu & DEM_DTC_STATUS_AVAILABILITY_MASK)
                                                               : (DTCStatusMask & DEM_DTC_STATUS_AVAILABILITY_MASK);
        /* PRQA S 2985-- */ /* MISRA Rule 2.2 */
        DemDTCFilterInfo.RequestDTCStatusMask = DTCStatusMask;
        DemDTCFilterInfo.DTCKind = DTCKind;
        DemDTCFilterInfo.DTCFormat = DTCFormat;
        DemDTCFilterInfo.DTCOrigin = memDest;
        DemDTCFilterInfo.FilterWithSeverity = FilterWithSeverity;
        DemDTCFilterInfo.DTCSeverityMask = DTCSeverityMask;
        DemDTCFilterInfo.FilterForFaultDetectionCounter = FilterForFaultDetectionCounter;
        DemDTCFilterInfo.CurrentIndex = 0;
        DemDTCFilterInfo.NumberOfFilteredDTC = 0;
        DemDTCFilterInfo.DemDemDTCReportNum = 0;
#if (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_ON)
        DemDTCFilterInfo.DemDTCTimeOrderNum = 0;
        Dem_MemSet(
            (uint8*)DemDTCFilterInfo.DemDTCReportByTimeOrder,
            0x00u,
            (sizeof(Dem_DTCReportByTimeOrderType) * DEM_DTC_NUM));
#endif
        Dem_MemSet((uint8*)DemDTCFilterInfo.DemDTCReport, 0x00u, DEM_DTC_NUM);
        DemDTCFilterInfo.IsSet = TRUE;
        ret = DEM_FILTER_ACCEPTED;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets the number of a filtered DTC.
 *                      (((statusOfDTC & DTCStatusMask) != 0) && ((severity & DTCSeverityMask) !=
 * 0)) == TRUE ServiceId           0x17 Sync/Async          Asynchronous Reentrancy          Non
 * Reentrant Param-Name[in]      none Param-Name[out]     NumberOfFilteredDTC:The number of DTCs
 * matching the defined status mask. Param-Name[in/out]  none Return              Status of the
 * operation to retrieve a number of DTC from the Dem
 */
/*************************************************************************/
FUNC(Dem_ReturnGetNumberOfFilteredDTCType, DEM_CODE)
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
Dem_DcmGetNumberOfFilteredDTC(P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) NumberOfFilteredDTC)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    Dem_ReturnGetNumberOfFilteredDTCType ret = DEM_NUMBER_FAILED;
    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETNUMBEROFFILTEREDDTC, DEM_E_UNINIT);
    }
    else if (NumberOfFilteredDTC == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETNUMBEROFFILTEREDDTC, DEM_E_PARAM_POINTER);
    }
    else if (DemDTCFilterInfo.IsSet == FALSE)
    {
        /* idle */
    }
    else
    {
        Dem_DTCFilterInfoType* pFilter = &DemDTCFilterInfo;

        if (DemDTCFilterInfo.IsSet == TRUE)
        {
            const Dem_DTCType* pDemDTC = DemPbCfgPtr->DemDTC;
            /*SWS_Dem_00410*/
            for (uint16 iloop = 0; iloop < DEM_DTC_NUM; iloop++)
            {
                const Dem_DTCGeneralStatusType* pDemDTCGeneralStatus = &DemDTCGeneralStatus[iloop];
                /* PRQA S 3415++ */ /* MISRA Rule 13.5 */
                if (((0x00u != (pDemDTCGeneralStatus->CbUdsStatus & pFilter->DTCStatusMask))
                     || (0u == pFilter->RequestDTCStatusMask))
                    && (TRUE == Dem_CheckDTCFormat(pFilter->DTCFormat, iloop))
                    && ((pFilter->FilterWithSeverity == FALSE)
                        || ((0x00u != (pFilter->DTCSeverityMask & pDemDTC[iloop].DemDTCSeverity))))
                    && (pFilter->FilterForFaultDetectionCounter == FALSE)
                    && (FALSE == pDemDTCGeneralStatus->SuppressionStatus))
                /* PRQA S 3415-- */ /* MISRA Rule 13.5 */
                {
                    pFilter->NumberOfFilteredDTC++;
#if (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_ON)
                    /* SWS_Dem_00410 */
                    boolean dtcflag = FALSE;
                    uint8 EntryNum = DemMemDestCfg[pFilter->DTCOrigin].EntryNum;
                    const Dem_EventMemEntryType* pEntry = DemMemDestCfg[pFilter->DTCOrigin].EntryList;
                    Dem_UdsStatusByteType DtcStatus =
                        (pDemDTCGeneralStatus->CbUdsStatus & DEM_DTC_STATUS_AVAILABILITY_MASK);
                    if (((DtcStatus & 0x0Cu) != 0u) && ((DtcStatus & 0xF3u) == 0u))
                    {
                        for (uint8 index = 0; index < EntryNum; index++)
                        {
                            if (pEntry->EventId != 0u)
                            {
                                const Dem_EventRelateInformationType* pEvent =
                                    Dem_GetEventInfo(Dem_GetEventInternalId(pEntry->EventId));
                                uint16 DTCRef = DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef;
                                if ((DEM_MEM_DEST_INVALID
                                     != Dem_CheckEventInternalMemDest(pFilter->DTCOrigin, pEvent->IntId))
                                    && (iloop == DTCRef))
                                {
                                    dtcflag = TRUE;
                                    Dem_DTCReportByTimeOrderType* pDTCTimeOrder =
                                        &pFilter->DemDTCReportByTimeOrder[pFilter->DemDTCTimeOrderNum];
                                    pDTCTimeOrder->DtcRef = iloop;
                                    /* get max Abstime */
                                    if (pDTCTimeOrder->DtcAbsTime < pEntry->AbsTime)
                                    {
                                        pDTCTimeOrder->DtcAbsTime = pEntry->AbsTime;
                                    }
#if ((DEM_EVENT_COMBINATION_SUPPORT == DEM_EVCOMB_ONSTORAGE) || (DEM_EVENT_COMBINATION_SUPPORT == DEM_EVCOMB_DISABLED))
                                    break;
#endif
                                }
                            }
                            pEntry++;
                        }
                        if (dtcflag == TRUE)
                        {
                            pFilter->DemDTCTimeOrderNum++;
                            dtcflag = FALSE;
                        }
                        else
                        {
                            pFilter->DemDTCReport[pFilter->DemDemDTCReportNum] = iloop;
                            pFilter->DemDemDTCReportNum++;
                        }
                    }
                    else
#endif
                    {
                        pFilter->DemDTCReport[pFilter->DemDemDTCReportNum] = iloop;
                        pFilter->DemDemDTCReportNum++;
                    }
                }
            }
#if (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_ON)
            /* SWS_Dem_00411 */
            /* Arrange Dtc index in chronological order */ /* insertion sort */
            for (uint16 index = 1u; index < pFilter->DemDTCTimeOrderNum; index++)
            {
                Dem_DTCReportByTimeOrderType* pDTCTimeOrder = pFilter->DemDTCReportByTimeOrder;
                if (pDTCTimeOrder[index].DtcAbsTime > pDTCTimeOrder[index - 1u].DtcAbsTime)
                {
                    Dem_DTCReportByTimeOrderType tmp = pDTCTimeOrder[index];
                    pDTCTimeOrder[index] = pDTCTimeOrder[index - 1u];
                    pDTCTimeOrder[index - 1u] = tmp;
                    for (uint16 iloop = index - 1u;
                         (iloop > 0u) && (pDTCTimeOrder[iloop].DtcAbsTime > pDTCTimeOrder[iloop - 1u].DtcAbsTime);
                         iloop--)
                    {
                        pFilter->DemDTCReportByTimeOrder[iloop] = pFilter->DemDTCReportByTimeOrder[iloop - 1u];
                        pFilter->DemDTCReportByTimeOrder[iloop - 1u] = tmp;
                    }
                }
            }
#endif
            DemDTCFilterInfo.IsSet = FALSE;
            *NumberOfFilteredDTC = pFilter->NumberOfFilteredDTC;
            ret = DEM_NUMBER_OK;
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets the next filtered DTC matching the filter criteria.
 * ServiceId           0x18
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     DTC : Receives the DTC value in respective format of the filter
 *                          returned by this function.
 *                       DTCStatus: This parameter receives the status information of
 *                          the requested DTC.
 * Param-Name[in/out]  none
 * Return              Status of the operation to retrieve a DTC from the Dem.
 */
/*************************************************************************/
FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE)
Dem_DcmGetNextFilteredDTC(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
    P2VAR(Dem_UdsStatusByteType, AUTOMATIC, DEM_APPL_DATA) DTCStatus)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Dem_ReturnGetNextFilteredElementType ret;
    Dem_DTCFilterInfoType* pFilter = &DemDTCFilterInfo;

    ret = Dem_DcmCheckDTCFilterStatus(DTC, DTCStatus);
    if (ret == DEM_FILTERED_OK)
    {
        ret = DEM_FILTERED_NO_MATCHING_ELEMENT;
#if (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_ON)
        for (; pFilter->CurrentIndex < pFilter->DemDTCTimeOrderNum; pFilter->CurrentIndex++)
        {
            if (ret != DEM_FILTERED_OK)
            {
                uint16 dtcRef = pFilter->DemDTCReportByTimeOrder[pFilter->CurrentIndex].DtcRef;
                /*get DTCValue */
                Dem_GetEventDTC(dtcRef, pFilter->DTCFormat, DTC);
                *DTCStatus = DemDTCGeneralStatus[dtcRef].CbUdsStatus & DEM_DTC_STATUS_AVAILABILITY_MASK;
                if (0u != *DTC)
                {
                    ret = DEM_FILTERED_OK;
                }
            }
            else
            {
                break;
            }
        }
        if (pFilter->CurrentIndex >= pFilter->DemDTCTimeOrderNum)
#endif
        {
            for (; pFilter->CurrentIndex < pFilter->NumberOfFilteredDTC; pFilter->CurrentIndex++)
            {
                if (ret != DEM_FILTERED_OK)
                {
#if (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_ON)
                    uint16 index = pFilter->CurrentIndex - pFilter->DemDTCTimeOrderNum;
#else
                    uint16 index = pFilter->CurrentIndex;
#endif
                    /*get DTCValue */
                    Dem_GetEventDTC(pFilter->DemDTCReport[index], pFilter->DTCFormat, DTC);
                    *DTCStatus = DemDTCGeneralStatus[pFilter->DemDTCReport[index]].CbUdsStatus
                                 & DEM_DTC_STATUS_AVAILABILITY_MASK;
                    if (0u != *DTC)
                    {
                        ret = DEM_FILTERED_OK;
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets the next filtered DTC and its associated Fault Detection Counter
 *                      (FDC) matching the filter criteria.
 * ServiceId           0x3b
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     DTC: Receives the DTC value in respective format of the filter
 *                          returned by this function.
 *                       DTCFaultDetectionCounter:This parameter receives the Fault Detection
 *                          Counter information of the requested DTC.
 * Param-Name[in/out]  none
 * Return              Status of the operation to retrieve a DTC from the Dem.
 */
/*************************************************************************/
FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE)
Dem_DcmGetNextFilteredDTCAndFDC(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
    P2VAR(sint8, AUTOMATIC, DEM_APPL_DATA) DTCFaultDetectionCounter)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Dem_ReturnGetNextFilteredElementType ret = DEM_FILTERED_NO_MATCHING_ELEMENT;
    Dem_DTCFilterInfoType* pFilter = &DemDTCFilterInfo;
    uint16 dtcRef;
    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETNEXTFILTEREDDTCANDFDC, DEM_E_UNINIT);
    }
    else if ((DTCFaultDetectionCounter == NULL_PTR) || (DTC == NULL_PTR))
    {
        DEM_DET_REPORT(DEM_SID_DCMGETNEXTFILTEREDDTCANDFDC, DEM_E_PARAM_POINTER);
    }
    else
    {
        /* PRQA S 2463++ */ /* MISRA Rule 14.2 */
        for (; (ret != DEM_FILTERED_OK) && (pFilter->CurrentIndex < DEM_DTC_NUM); pFilter->CurrentIndex++)
        /* PRQA S 2463-- */ /* MISRA Rule 14.2 */
        {
            dtcRef = pFilter->CurrentIndex;
            if (dtcRef != DEM_DTC_REF_INVALID)
            {
                /* req SWS_Dem_01101 */
                /* PRQA S 3415++ */ /* MISRA Rule 13.5 */
                if ((DemDTCGeneralStatus[dtcRef].SuppressionStatus == TRUE)
                    || (FALSE == Dem_CheckDTCFormat(pFilter->DTCFormat, dtcRef)))
                /* PRQA S 3415-- */ /* MISRA Rule 13.5 */
                {
                    ret = DEM_FILTERED_NO_MATCHING_ELEMENT; /* PRQA S 2469 */ /* MISRA Rule 14.2 */
                }
                else
                {
                    /* guarantee the event index the new dtc */
                    Dem_GetEventDTC(dtcRef, pFilter->DTCFormat, DTC);
                    /* req SWS_Dem_00788 req SWS_Dem_00789 req SWS_Dem_00792 req SWS_Dem_00793*/
                    *DTCFaultDetectionCounter = FDCInfo[dtcRef].FDC;
                    /* PRQA S 2985++ */ /* MISRA Rule 2.2 */
                    if ((0u
                         != (DemDTCGeneralStatus[dtcRef].CbUdsStatus & DEM_DTC_STATUS_AVAILABILITY_MASK
                             & pFilter->DTCStatusMask))
                        && (0u != *DTC))
                    /* PRQA S 2985-- */ /* MISRA Rule 2.2 */
                    {
                        ret = DEM_FILTERED_OK; /* PRQA S 2469 */ /* MISRA Rule 14.2 */
                    }
                }
            }
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets the next filtered DTC and its associated Severity matching the filter
                        criteria. The interface has an asynchronous behavior, because a large
                        number of DTCs has to be processed.
 * ServiceId           0x3d
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     DTC:Receives the DTC value in respective format of the
                            filter returned by this function. If the return value of
                            the function is other than DEM_FILTERED_OK this
                            parameter does not contain valid data.
 *                       DTCStatus:This parameter receives the status information of
                            the requested DTC.
                            It follows the format as defined in ISO14229-1
                            If the return value of the function call is other than
                            DEM_FILTERED_OK this parameter does not contain valid data.
 *                       DTCSeverity: Receives the severity value returned by the function.
 *                          If the return value of the function is other than
                            DEM_FILTERED_OK this parameter does not contain valid data
 *                       DTCFunctionalUnit: Receives the functional unit value returned by the
                            function. If the return value of the function is other
                            than DEM_FILTERED_OK this parameter does not
                            contain valid data.
 * Param-Name[in/out]  none
 * Return              Status of the operation to retrieve a DTC from the Dem.
 */
/*************************************************************************/
FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE)
Dem_DcmGetNextFilteredDTCAndSeverity(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
    P2VAR(Dem_UdsStatusByteType, AUTOMATIC, DEM_APPL_DATA) DTCStatus,
    P2VAR(Dem_DTCSeverityType, AUTOMATIC, DEM_APPL_DATA) DTCSeverity,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DTCFunctionalUnit)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Dem_ReturnGetNextFilteredElementType ret = DEM_FILTERED_NO_MATCHING_ELEMENT;
    Dem_DTCFilterInfoType* pFilter = &DemDTCFilterInfo;
    uint16 dtcRef;

    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETNEXTFILTEREDDTCANDSEVERITY, DEM_E_UNINIT);
    }
    else if (
        (DTC == NULL_PTR) || (DTCStatus == NULL_PTR) || (DTCSeverity == NULL_PTR) || (DTCFunctionalUnit == NULL_PTR))
    {
        DEM_DET_REPORT(DEM_SID_DCMGETNEXTFILTEREDDTCANDSEVERITY, DEM_E_PARAM_POINTER);
    }
    else
    {
        /* PRQA S 2463++ */ /* MISRA Rule 14.2 */
        for (; (ret != DEM_FILTERED_OK) && (pFilter->CurrentIndex < DEM_DTC_NUM); pFilter->CurrentIndex++)
        /* PRQA S 2463-- */ /* MISRA Rule 14.2 */
        {
            dtcRef = pFilter->CurrentIndex;
            if (dtcRef != DEM_DTC_REF_INVALID)
            {
                /* req SWS_Dem_01101 */
                /* PRQA S 3415++ */ /* MISRA Rule 13.5 */
                if ((DemDTCGeneralStatus[dtcRef].SuppressionStatus == TRUE)
                    || (FALSE == Dem_CheckDTCFormat(pFilter->DTCFormat, dtcRef)))
                /* PRQA S 3415-- */ /* MISRA Rule 13.5 */
                {
                    ret = DEM_FILTERED_NO_MATCHING_ELEMENT; /* PRQA S 2469 */ /* MISRA Rule 14.2 */
                }
                else
                {
                    /* guarantee the event index the new dtc */
                    Dem_GetEventDTC(dtcRef, pFilter->DTCFormat, DTC);
                    /* PRQA S 2985++ */ /* MISRA Rule 2.2 */
                    *DTCStatus = (DemDTCGeneralStatus[dtcRef].CbUdsStatus & DEM_DTC_STATUS_AVAILABILITY_MASK);
                    /* PRQA S 2985-- */ /* MISRA Rule 2.2 */
                    *DTCSeverity = DemPbCfgPtr->DemDTC[dtcRef].DemDTCSeverity;
                    *DTCFunctionalUnit = DemPbCfgPtr->DemDTC[dtcRef].DemDTCFunctionalUnit;
                    if ((0u != (*DTCStatus & pFilter->DTCStatusMask)) && (0u != *DTC))
                    {
                        ret = DEM_FILTERED_OK; /* PRQA S 2469 */ /* MISRA Rule 14.2 */
                    }
                }
            }
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Sets a freeze frame record filter.
 * ServiceId           0x3f
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
Dem_DcmSetFreezeFrameRecordFilter(
    Dem_DTCFormatType DTCFormat,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) NumberOfFilteredRecords)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Dem_ReturnSetFilterType ret = DEM_WRONG_FILTER;

    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_DCMSETFREEZEFRAMERECORDFILTER, DEM_E_UNINIT);
    }
    else if (DEM_DTC_FORMAT_UDS != DTCFormat)
    {
        DEM_DET_REPORT(DEM_SID_DCMSETFREEZEFRAMERECORDFILTER, DEM_E_WRONG_CONFIGURATION);
    }
    else if (NumberOfFilteredRecords == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_DCMSETFREEZEFRAMERECORDFILTER, DEM_E_PARAM_POINTER);
    }
    else
    {
        ret = Dem_InterDcmSetFreezeFrameRecordFilter(DTCFormat, NumberOfFilteredRecords);
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets the next freeze frame record number and its associated DTC
 *                          stored in the event memory.
 * ServiceId           0x3a
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
Dem_DcmGetNextFilteredRecord(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) RecordNumber)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Dem_ReturnGetNextFilteredElementType ret = DEM_FILTERED_NO_MATCHING_ELEMENT;
    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETNEXTFILTEREDRECORD, DEM_E_UNINIT);
    }
    else if (DTC == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETNEXTFILTEREDRECORD, DEM_E_PARAM_POINTER);
    }
    else if (RecordNumber == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETNEXTFILTEREDRECORD, DEM_E_PARAM_POINTER);
    }
    else
    {
        ret = Dem_InterDcmGetNextFilteredRecord(DTC, RecordNumber);
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets the DTC by occurrence time.
 * ServiceId           0x19
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTCRequest:This parameter defines the request type of the DTC.
 * Param-Name[out]     DTC: Receives the DTC value in UDS format returned by the function.
 * Param-Name[in/out]  none
 * Return              Status of the operation of type Dem_ReturnGetDTCByOccurrenceTimeType.
 */
/*************************************************************************/
/* PRQA S 3432,1532++ */ /* MISRA Rule 20.7,8.7 */
FUNC(Dem_ReturnGetDTCByOccurrenceTimeType, DEM_CODE)
Dem_DcmGetDTCByOccurrenceTime(Dem_DTCRequestType DTCRequest, P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC)
/* PRQA S 3432,1532-- */ /* MISRA Rule 20.7,8.7 */
{
    Dem_EventIdType IntId;
    Dem_ReturnGetDTCByOccurrenceTimeType ret = DEM_OCCURR_NOT_AVAILABLE;

    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETDTCBYOCCURRENCETIME, DEM_E_UNINIT);
    }
    else if (DTC == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETDTCBYOCCURRENCETIME, DEM_E_PARAM_POINTER);
    }
    else
    {
        switch (DTCRequest)
        {
        case DEM_FIRST_FAILED_DTC:
            IntId = DemDTCByOccurrenceTimeInfo.FirstFailed;
            break;
        case DEM_MOST_RECENT_FAILED_DTC:
            IntId = DemDTCByOccurrenceTimeInfo.MostRecentFailed;
            break;
        case DEM_FIRST_DET_CONFIRMED_DTC:
            IntId = DemDTCByOccurrenceTimeInfo.FirstDtcConfirmed;
            break;
        case DEM_MOST_REC_DET_CONFIRMED_DTC:
            IntId = DemDTCByOccurrenceTimeInfo.MostRecDtcConfirmed;
            break;
        default:
            IntId = DEM_EVENT_PARAMETER_INVALID;
            break;
        }
        if ((IntId != DEM_EVENT_PARAMETER_INVALID)
            && (DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef != DEM_DTC_REF_INVALID))
        {
            uint16 DTCRef = DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef;
            /* req SWS_Dem_01101 */
            if (DemDTCGeneralStatus[DTCRef].SuppressionStatus == FALSE)
            {
                Dem_GetEventDTC(DTCRef, DEM_DTC_FORMAT_UDS, DTC);
            }
            ret = DEM_OCCURR_OK;
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Controls the triggering of Dcm_DemTriggerOnDTCStatus.
 * ServiceId           0xb0
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TriggerNotification:This parameter specifies whether the triggering of
 *                          the notification shall be enabled (TRUE) or disabled (FALSE).
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
FUNC(void, DEM_CODE) Dem_DcmControlDTCStatusChangedNotification(boolean TriggerNotification)
{
    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_DCMCONTROLDTCSTATUSCHANGEDNOTIFICATION, DEM_E_UNINIT);
    }
    Dem_DTCStatusChangedInd = TriggerNotification;
}

/*******************************************************************************
**      Access extended data records and FreezeFrame data                     **
*******************************************************************************/
/*************************************************************************/
/*
 * Brief               Disables the event memory update of a specific DTC
 *                          (only one at one time).
 * ServiceId           0x1a
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTC:Selects the DTC in UDS format, for which DTC record update
 *                          shall be disabled.
 *                       DTCOrigin: If the Dem supports more than one event memory,
 *                          this parameter is used to select the source memory
 *                          for which DTC record update shall be disabled.
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Status of the operation to disable the event memory
 *                          update of a specific DTC.
 */
/*************************************************************************/
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(Dem_ReturnDisableDTCRecordUpdateType, DEM_CODE)
Dem_DcmDisableDTCRecordUpdate(uint32 DTC, Dem_DTCOriginType DTCOrigin)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    uint16 dtcIndex;
    Dem_ReturnDisableDTCRecordUpdateType ret = DEM_DISABLE_DTCRECUP_WRONG_DTC;
    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_DCMDISABLEDTCRECORDUPDATE, DEM_E_UNINIT);
    }
    else if (Dem_DTCDisable == TRUE)
    {
        DEM_DET_REPORT(DEM_SID_DCMDISABLEDTCRECORDUPDATE, DEM_E_WRONG_CONDITION);
        ret = E_NOT_OK;
    }
    else
    {
        dtcIndex = Dem_GetDTCIndex(DTC, DEM_DTC_FORMAT_UDS);
        uint8 memDest = Dem_GetInternalMemDest(DTCOrigin);
        if (dtcIndex == DEM_DTC_REF_INVALID)
        {
            /*idle*/
        }
        else if (memDest == DEM_MEM_DEST_INVALID)
        {
            ret = DEM_DISABLE_DTCRECUP_WRONG_DTCORIGIN;
        }
        else
        {
            Dem_DTCDisable = TRUE;
            DTCRecordUpdateStatusInfo.DTCIndex = dtcIndex;
            DTCRecordUpdateStatusInfo.DTCOrigin = memDest;
            DEM_BITS_SET(DemMemDestInfo[memDest].DisableDTCRecordUpdate, dtcIndex);
            ret = DEM_DISABLE_DTCRECUP_OK;
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Enables the event memory update of the DTC disabled by
 *                          Dem_DcmDisableDTCRecordUpdate() before.
 * ServiceId           0x1b
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Always E_OK is returned.
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE) Dem_DcmEnableDTCRecordUpdate(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_DCMENABLEDTCRECORDUPDATE, DEM_E_UNINIT);
    }
    else
    {
        Dem_DTCDisable = FALSE;
        DEM_BITS_CLR(
            DemMemDestInfo[DTCRecordUpdateStatusInfo.DTCOrigin].DisableDTCRecordUpdate,
            DTCRecordUpdateStatusInfo.DTCIndex);
        ret = E_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets freeze frame data by DTC. The function stores the data
 *                          in the provided DestBuffer.
 * ServiceId           0x1d
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
/* PRQA S 3432,1532++ */ /* MISRA Rule 20.7,8.7 */
FUNC(Dem_ReturnGetFreezeFrameDataByDTCType, DEM_CODE)
Dem_DcmGetFreezeFrameDataByDTC(
    uint32 DTC,
    Dem_DTCOriginType DTCOrigin,
    uint8 RecordNumber,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize)
/* PRQA S 3432,1532-- */ /* MISRA Rule 20.7,8.7 */
{
    uint8 memDest;
    uint16 dtcIndex;
    Dem_ReturnGetFreezeFrameDataByDTCType ret = DEM_GET_FFDATABYDTC_WRONG_DTC;
    uint16 memDestAndRecordNumber;

    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETFREEZEFRAMEDATABYDTC, DEM_E_UNINIT);
    }
    else
    {
        dtcIndex = Dem_GetDTCIndex(DTC, DEM_DTC_FORMAT_UDS);
        memDest = Dem_GetInternalMemDest(DTCOrigin);
        if ((DestBuffer == NULL_PTR) || (BufSize == NULL_PTR))
        {
            DEM_DET_REPORT(DEM_SID_DCMGETFREEZEFRAMEDATABYDTC, DEM_E_PARAM_POINTER);
        }
        else if (RecordNumber == 0xFFu)
        {
            DEM_DET_REPORT(DEM_SID_DCMGETFREEZEFRAMEDATABYDTC, DEM_E_PARAM_DATA);
            ret = DEM_GET_FFDATABYDTC_WRONG_RECORDNUMBER;
        }
        /* req SWS_Dem_01100 req SWS_Dem_01101*/
        else if ((dtcIndex == DEM_DTC_REF_INVALID) || (DemDTCGeneralStatus[dtcIndex].SuppressionStatus == TRUE))
        {
            /*idle*/
        }
        else if (memDest == DEM_MEM_DEST_INVALID)
        {
            ret = DEM_GET_FFDATABYDTC_WRONG_DTCORIGIN;
        }
        else
        {
            memDestAndRecordNumber = (uint16)(((uint16)memDest << 8u) | (uint16)RecordNumber);
            ret = Dem_InterDcmGetFreezeFrameDataByDTC(DTC, memDestAndRecordNumber, DestBuffer, BufSize);
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets the size of freeze frame data by DTC.
 * ServiceId           0x1f
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
Dem_DcmGetSizeOfFreezeFrameByDTC(
    uint32 DTC,
    Dem_DTCOriginType DTCOrigin,
    uint8 RecordNumber,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) SizeOfFreezeFrame)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Dem_ReturnGetSizeOfDataByDTCType ret = DEM_GETSIZEBYDTC_WRONG_DTC;
    uint16 dtcRef;
    uint8 memDest;

    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETSIZEOFFREEZEFRAMEBYDTC, DEM_E_UNINIT);
    }
    else if (SizeOfFreezeFrame == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETSIZEOFFREEZEFRAMEBYDTC, DEM_E_PARAM_POINTER);
    }
    else
    {
        /*SWS_Dem_01100 SWS_Dem_01101 */
        dtcRef = Dem_GetDTCIndex(DTC, DEM_DTC_FORMAT_UDS);
        memDest = Dem_GetInternalMemDest(DTCOrigin);
        if ((dtcRef == DEM_DTC_REF_INVALID) || (DemDTCGeneralStatus[dtcRef].SuppressionStatus == TRUE))
        {
            /*idle*/
        }
        else if (memDest == DEM_MEM_DEST_INVALID)
        {
            ret = DEM_GETSIZEBYDTC_WRONG_DTCORIGIN;
        }
        else
        {
            *SizeOfFreezeFrame = 0u;
            ret = Dem_InterDcmGetSizeOfFreezeFrameByDTC(DTC, DTCOrigin, RecordNumber, SizeOfFreezeFrame);
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets extended data by DTC. The function stores the data in
 *                          the provided DestBuffer.
 * ServiceId           0x20
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTC:Diagnostic Trouble Code in UDS format.
 *                       DTCOrigin:If the Dem supports more than one event memory,
 *                          this parameter is used to select the source memory
 *                          the DTCs shall be read from.
 *                       ExtendedDataNumber:dentification/Number of requested extended data
 *                       record. The values 0xFE and 0xFF are not allowed.
 * Param-Name[out]     DestBuffer:This parameter contains a byte pointer that points to
 *                          the buffer, to which the extended data record shall
 *                          be written to.
 * Param-Name[in/out]  BufSize:When the function is called this parameter contains
 *                          the maximum number of data bytes that can be written to the buffer.
 * Return              Status of the operation to retrieve extended data by DTC.
 */
/*************************************************************************/
/* PRQA S 3432,1532++ */ /* MISRA Rule 20.7,8.7 */
FUNC(Dem_ReturnGetExtendedDataRecordByDTCType, DEM_CODE)
Dem_DcmGetExtendedDataRecordByDTC(
    uint32 DTC,
    Dem_DTCOriginType DTCOrigin,
    uint8 ExtendedDataNumber,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize)
/* PRQA S 3432,1532++ */ /* MISRA Rule 20.7,8.7 */
{
    uint8 memDest;
    Dem_ReturnGetExtendedDataRecordByDTCType ret = DEM_RECORD_WRONG_DTC;
    const Dem_ExtendedDataRecordClassType* pExtDataCfg;
    uint16 dtcRef;
    const Dem_DTCAttributesType* pDTCAttrCfg;
    uint16 memDestAndExtendedDataNumber;

    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETEXTENDEDDATARECORDBYDTC, DEM_E_UNINIT);
    }
    else if ((DestBuffer == NULL_PTR) || (BufSize == NULL_PTR))
    {
        DEM_DET_REPORT(DEM_SID_DCMGETEXTENDEDDATARECORDBYDTC, DEM_E_PARAM_POINTER);
    }
    else if (ExtendedDataNumber >= 0xFEu)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETEXTENDEDDATARECORDBYDTC, DEM_E_PARAM_DATA);
        ret = DEM_RECORD_WRONG_NUMBER;
    }
    else
    {
        memDest = Dem_GetInternalMemDest(DTCOrigin);
        pExtDataCfg = Dem_GetExtendedDataRecordCfg(ExtendedDataNumber);
        /* req SWS_Dem_01100 req SWS_Dem_01101 */
        dtcRef = Dem_GetDTCIndex(DTC, DEM_DTC_FORMAT_UDS);
        pDTCAttrCfg = &DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].DemDTCAttributesRef];
        if (memDest == DEM_MEM_DEST_INVALID)
        {
            /* req SWS_Dem_00171 */
            ret = DEM_RECORD_WRONG_DTCORIGIN;
        }
        else if (pExtDataCfg == NULL_PTR)
        {
            ret = DEM_RECORD_WRONG_NUMBER;
        }
        else if (*BufSize < pExtDataCfg->DataSize)
        {
            ret = DEM_RECORD_WRONG_BUFFERSIZE;
        }
        else if (DEM_EXTENDED_DATA_INVALID == pDTCAttrCfg->DemExtendedDataClassRef)
        {
            ret = DEM_RECORD_WRONG_NUMBER;
        }
        else if ((dtcRef != DEM_DTC_REF_INVALID) && (DemDTCGeneralStatus[dtcRef].SuppressionStatus == FALSE))
        {
            *BufSize = 0u;
            memDestAndExtendedDataNumber = (uint16)(((uint16)memDest << 8u) | (uint16)ExtendedDataNumber);
            ret = Dem_GetExtendedRecordDataLoop(DTC, memDestAndExtendedDataNumber, DestBuffer, BufSize);
        }
        else
        {
            /*idle*/
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets the size of extended data by DTC.
 * ServiceId           0x21
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
Dem_DcmGetSizeOfExtendedDataRecordByDTC(
    uint32 DTC,
    Dem_DTCOriginType DTCOrigin,
    uint8 ExtendedDataNumber,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) SizeOfExtendedDataRecord)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
{
    Dem_ReturnGetSizeOfDataByDTCType ret = DEM_GETSIZEBYDTC_WRONG_DTC;
    uint8 memDest;
    uint16 dtcRef;

    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETSIZEOFEXTENDEDDATARECORDBYDTC, DEM_E_UNINIT);
    }
    else if (SizeOfExtendedDataRecord == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETSIZEOFEXTENDEDDATARECORDBYDTC, DEM_E_PARAM_POINTER);
    }
    else
    {
        dtcRef = Dem_GetDTCIndex(DTC, DEM_DTC_FORMAT_UDS);
        memDest = Dem_GetInternalMemDest(DTCOrigin);
        /* req SWS_Dem_01100 req SWS_Dem_01101*/
        if ((dtcRef == DEM_DTC_REF_INVALID) || (DemDTCGeneralStatus[dtcRef].SuppressionStatus == TRUE))
        {
            /*idle*/
        }
        else if (memDest == DEM_MEM_DEST_INVALID)
        {
            ret = DEM_GETSIZEBYDTC_WRONG_DTCORIGIN;
        }
        else
        {
            ret = Dem_InterDcmGetSizeOfExtendedDataRecordByDTC(
                DTC,
                DTCOrigin,
                ExtendedDataNumber,
                SizeOfExtendedDataRecord);
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Performs a parameter check and gives the result which would also be
                        returned by calling clear with same parameters.
 * ServiceId           0x7b
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      DTC:Defines the DTC in respective format, that shall be
 *                          cleared from the event memory. If the DTC fits to a
 *                          DTC group number, all DTCs of the group shall be cleared.
 *                       DTCFormat:Defines the input-format of the provided DTC value.
 *                       DTCOrigin:If the Dem supports more than one event memory
 *                          this parameter is used to select the source memory
 *                          the DTCs shall be read from.
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Returns the result of the parameter check when performing a
 *                      clear command with same parameters (refer to Dem_DcmClearDTC).
                        Only the following return values will be used:
                        DEM_CLEAR_OK: DTC successfully cleared
                        DEM_CLEAR_WRONG_DTC: DTC value not existing (in this format)
                        DEM_CLEAR_WRONG_DTCORIGIN: Wrong DTC origin
                        DEM_CLEAR_FAILED: In case of general errors
with clear parameters
 */
/*************************************************************************/
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(Dem_ReturnClearDTCType, DEM_CODE)
Dem_DcmCheckClearParameter(uint32 DTC, Dem_DTCFormatType DTCFormat, Dem_DTCOriginType DTCOrigin)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    Dem_ReturnClearDTCType ret = DEM_CLEAR_FAILED;
#if (DEM_CLEAR_DTCBEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
    Dem_ClearDTCInfoType* pClr = &DemClearDTCInfo;
#endif
    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_DCMCHECKCLEARPARAMETER, DEM_E_UNINIT);
    }
    else if (Dem_ClearDTCLock == DEM_CLEAR_LOCK)
    {
        ret = DEM_CLEAR_PENDING;
    }
    else if (DEM_DTC_ORIGIN_PRIMARY_MEMORY != DTCOrigin)
    {
        ret = DEM_CLEAR_WRONG_DTCORIGIN;
    }
    /* PRQA S 3415++ */ /* MISRA Rule 13.5 */
    else if (
        (DTC != DEM_DTC_GROUP_ALL_DTCS)
#if (DEM_CLEAR_DTCLIMITATION != DEM_ONLY_CLEAR_ALL_DTCS)
        && (Dem_GetDTCIndex(DTC, DTCFormat) == DEM_DTC_REF_INVALID)
#if (DEM_GROUP_OF_DTC_NUM > 0u)
        && (DEM_GROUP_OF_DTC_INVALID == Dem_GetDTCGroupIndex(DTC))
#endif /* DEM_GROUP_OF_DTC_NUM > 0u */
#endif /* DEM_CLEAR_DTCLIMITATION != DEM_ONLY_CLEAR_ALL_DTCS */
    )
    /* PRQA S 3415-- */ /* MISRA Rule 13.5 */
    {
        ret = DEM_CLEAR_WRONG_DTC;
    }
    else
    {
        ret = DEM_CLEAR_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Clears single DTCs, as well as groups of DTCs. This API is intended
                       for the Dcm. It can only be used through the RTE (due to work-around
                       described below SWS_Dem_00009), and therefore no declaration is exported via
 Dem_Dcm.h.
 * ServiceId           0x22
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      DTC:Defines the DTC in respective format, that shall be
 *                          cleared from the event memory. If the DTC fits to a
 *                          DTC group number, all DTCs of the group shall be cleared.
 *                       DTCFormat:Defines the input-format of the provided DTC value.
 *                       DTCOrigin:If the Dem supports more than one event memory
 *                          this parameter is used to select the source memory
 *                          the DTCs shall be read from.
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Status of the operation of type Dem_ReturnClearDTCType.
 */
/*************************************************************************/
/* PRQA S 0624++ */ /* MISRA Rule 8.3 */
FUNC(Dem_ReturnClearDTCType, DEM_CODE)
Dem_DcmClearDTC(uint32 DTC, Dem_DTCFormatType DTCFormat, Dem_DTCOriginType DTCOrigin)
/* PRQA S 0624-- */ /* MISRA Rule 8.3 */
{
    Dem_ClearDTCInfoType* pClr = &DemClearDTCInfo;
    Dem_ReturnClearDTCType ret = DEM_CLEAR_FAILED;
    uint16 dtcRef = Dem_GetDTCIndex(DTC, DTCFormat);

    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_DCMCLEARDTC, DEM_E_UNINIT);
    }
    else if (DEM_DTC_ORIGIN_PRIMARY_MEMORY != DTCOrigin)
    {
        ret = DEM_CLEAR_WRONG_DTCORIGIN;
    }
    else
    {
        if ((DTC == DEM_DTC_GROUP_ALL_DTCS) && (DEM_DTC_FORMAT_OBD == DTCFormat))
        {
            Dem_ClearDTRInfoByEventId(DEM_EVENT_PARAMETER_INVALID);
        }
        if (Dem_ClearDTCLock == DEM_CLEAR_LOCK)
        {
            if ((pClr->SaveDTCIndex == dtcRef) && (pClr->SaveDTCFormat == DTCFormat)
                && (pClr->SaveDTCOrigin == DTCOrigin) && (pClr->SID == DEM_SID_DCMCLEARDTC))
            {
#if (DEM_CLEAR_DTCBEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
                if (Dem_ClearNonvolatileStatus == DEM_ClEAR_NONVOLATILE_FINISHED)
                {
                    /* SWS_Dem_00572 */
                    Dem_ClearNonvolatileStatus = DEM_ClEAR_NONVOLATILE_INIT;
                    Dem_ResetDemClearDTCInfo();
                    Dem_ClearDTCLock = DEM_CLEAR_NOT_LOCK;
                    ret = DEM_CLEAR_OK;
                }
                else if (Dem_ClearNonvolatileStatus == DEM_ClEAR_NONVOLATILE_FAILED)
                {
                    /*SWS_Dem_01057*/
                    Dem_ClearNonvolatileStatus = DEM_ClEAR_NONVOLATILE_INIT;
                    Dem_ResetDemClearDTCInfo();
                    Dem_ClearDTCLock = DEM_CLEAR_NOT_LOCK;
                    ret = DEM_CLEAR_MEMORY_ERROR;
                }
                else
                {
                    /* req SWS_Dem_00663 */
                    ret = DEM_CLEAR_PENDING;
                }
#endif
            }
            else
            {
                /* req SWS_Dem_00662 req SWS_Dem_00664*/
                ret = DEM_CLEAR_BUSY;
            }
        }
        else if (Dem_ClearDTCLock == DEM_CLEAR_NOT_LOCK)
        {
            /* req SWS_Dem_00661 */
            Dem_ClearDTCLock = DEM_CLEAR_LOCK;
            pClr->DTCIndex = dtcRef;
            pClr->DTCFormat = DTCFormat;
            pClr->DTCOrigin = DTCOrigin;
            pClr->SaveDTCIndex = dtcRef;
            pClr->SaveDTCFormat = DTCFormat;
            pClr->SaveDTCOrigin = DTCOrigin;
            pClr->SID = DEM_SID_DCMCLEARDTC;
        }
        else
        {
            /*idle*/
        }
        /* req SWS_Dem_00670 */
#if (DEM_CLEAR_DTCLIMITATION == DEM_ONLY_CLEAR_ALL_DTCS)
        if ((ret == DEM_CLEAR_FAILED) && (DTCFormat != DEM_DTC_FORMAT_UDS || (DTC & 0xFFFFFFUL) != 0xFFFFFFUL))
        {
            ret = DEM_CLEAR_WRONG_DTC;
        }
#endif
        uint8 memDest = Dem_GetInternalMemDest(DTCOrigin);
        if ((ret == DEM_CLEAR_FAILED) && (memDest == DEM_MEM_DEST_INVALID))
        {
            ret = DEM_CLEAR_WRONG_DTCORIGIN;
        }
        if (ret == DEM_CLEAR_FAILED)
        {
            ret = Dem_DcmSubClearDTC(DTC, DTCFormat, memDest);
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Disables the DTC setting for a DTC group.
 * ServiceId           0x24
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
Dem_DcmDisableDTCSetting(uint32 DTCGroup, Dem_DTCKindType DTCKind)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    Dem_ReturnControlDTCSettingType ret = DEM_CONTROL_DTC_SETTING_N_OK;
    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_DCMDISABLEDTCSETTING, DEM_E_UNINIT);
    }
    else
    {
        ret = Dem_InterDcmDisableDTCSetting(DTCGroup, DTCKind);
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Enables the DTC setting for a DTC group.
                        This API is intended for the Dcm. It can only be used through the RTE
                        (due to work-around described below SWS_Dem_00035), and therefore
                        no declaration is exported via Dem_Dcm.h.
 * ServiceId           0x25
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
/* PRQA S 1532,0624++ */ /* MISRA Rule 8.7,8.3 */
FUNC(Dem_ReturnControlDTCSettingType, DEM_CODE)
Dem_DcmEnableDTCSetting(uint32 DTCGroup, Dem_DTCKindType DTCKind)
/* PRQA S 1532,0624-- */ /* MISRA Rule 8.7,8.3 */
{
    Dem_ReturnControlDTCSettingType ret = DEM_CONTROL_DTC_SETTING_N_OK;
    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_DCMENABLEDTCSETTING, DEM_E_UNINIT);
    }
    else
    {
        ret = Dem_InterDcmEnableDTCSetting(DTCGroup, DTCKind);
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets the DTC Severity availability mask.
 * ServiceId           0xb2
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     DTCSeverityMask:The value DTCSeverityMask indicates the
 *                      supported DTC severity bits from the Dem.
 * Param-Name[in/out]  none
 * Return              E_OK: get of DTC severity mask was successful
                        E_NOT_OK: get of DTC severity mask failed
 */
/*************************************************************************/
FUNC(Dem_ReturnClearDTCType, DEM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
Dem_DcmGetDTCSeverityAvailabilityMask(P2VAR(Dem_DTCSeverityType, AUTOMATIC, DEM_VAR) DTCSeverityMask)
/* PRQA S 3432-- */ /* MISRA Rule 8207 */
{
    Dem_ReturnClearDTCType ret = E_NOT_OK;
    if (DEM_STATE_INIT != Dem_InitState)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETDTCSEVERITYAVAILABILITYMASK, DEM_E_UNINIT);
    }
    else if (NULL_PTR == DTCSeverityMask)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETDTCSEVERITYAVAILABILITYMASK, DEM_E_PARAM_POINTER);
    }
    else
    {
        *DTCSeverityMask = DemDTCFilterInfo.DTCSeverityMask;
        ret = E_OK;
    }
    return ret;
}

/*******************************************************************************
**                      Private Function Definitions                         **
*******************************************************************************/

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
FUNC(void, DEM_CODE) Dem_SetDTCSettingProcess(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint16 iloop;
    Dem_EventRelateInformationType* pEvent;
    const Dem_EventParameterType* pEventCfg;

    for (iloop = 0; iloop < DemPbCfgPtr->DemEventNum; iloop++)
    {
        pEventCfg = &DemPbCfgPtr->DemEventParameter[iloop];
        pEvent = Dem_GetEventInfo(iloop);
        if ((pEventCfg->DemDTCRef != DEM_DTC_REF_INVALID)
            && (DemDTCSettingInfo.DTCKind == DemPbCfgPtr->DemDTC[pEventCfg->DemDTCRef].DTCKind))
        {
            if (DemPbCfgPtr->DemDTC[pEventCfg->DemDTCRef].GroupRef != DEM_GROUP_OF_DTC_INVALID)
            {
#if (DEM_GROUP_OF_DTC_NUM > 0)
                if (DEM_BITS_ISNSET(
                        DemDTCSettingInfo.DTCGroupStatus,
                        DemPbCfgPtr->DemDTC[pEventCfg->DemDTCRef].GroupRef))
                {
                    DEM_FLAGS_CLR(pEvent->Status, DEM_EVENT_STATUS_DTC_SETTING);
                    /* req SWS_Dem_00678 */
                    Dem_DebounceFreeze(iloop);
                }
                else
#endif
                {
                    if (0x00u == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_DTC_SETTING))
                    {
                        DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_DTC_SETTING);
                        /* req SWS_Dem_00682 Monitor re-initialization  */
                        if (DemPbCfgPtr->DemEventParameter[iloop].DemCallbackInitMForE != NULL_PTR)
                        {
                            DemPbCfgPtr->DemEventParameter[iloop].DemCallbackInitMForE(DEM_INIT_MONITOR_REENABLED);
                        }
                    }
                }
            }
            else
            {
                if (DemDTCSettingInfo.AllGroupIsEnabled == FALSE)
                {
                    /* ControlDTCSetting is disabled*/
                    DEM_FLAGS_CLR(pEvent->Status, DEM_EVENT_STATUS_DTC_SETTING);
                    /* req SWS_Dem_00678 */
                    Dem_DebounceFreeze(iloop);
                }
                else
                {
                    if (0x00u == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_DTC_SETTING))
                    {
                        DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_DTC_SETTING);
                        /* req SWS_Dem_00682 Monitor re-initialization  */
                        if (DemPbCfgPtr->DemEventParameter[iloop].DemCallbackInitMForE != NULL_PTR)
                        {
                            DemPbCfgPtr->DemEventParameter[iloop].DemCallbackInitMForE(DEM_INIT_MONITOR_REENABLED);
                        }
                    }
                }
            }
        }
    }
}

/*************************************************************************/
/*
 * Brief               BRIEF DESCRIPTION
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant/Non Reentrant
 * Param-Name[in]      IntId && DTCFormat
 * Param-Name[out]     DTC
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
FUNC(void, DEM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
Dem_GetEventDTC(uint16 DTCRef, Dem_DTCFormatType DTCFormat, P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint16 DemObdDTCRef;
    const Dem_DTCType* pDemDTC = DemPbCfgPtr->DemDTC;
    const DemObdDTCType* pDemObdDTC = DemPbCfgPtr->DemObdDTC;
    uint32 DemDtcValue;
    *DTC = 0x00UL;
    if (DTCRef != DEM_DTC_REF_INVALID)
    {
        switch (DTCFormat)
        {
        case DEM_DTC_FORMAT_UDS:
            DemDtcValue = pDemDTC[DTCRef].DemDtcValue;
            if ((DemDtcValue != DEM_DTC_CFG_INVALID))
            {
                *DTC = DemDtcValue;
            }
            else
            {
                *DTC = 0x00UL;
            }
            break;
        case DEM_DTC_FORMAT_J1939:
            DemObdDTCRef = pDemDTC[DTCRef].DemObdDTCRef;
            if ((DemObdDTCRef != DEM_OBD_DTC_INVALID)
                && (pDemObdDTC[DemObdDTCRef].DemJ1939DTCValue != DEM_J1939_DTC_CFG_INVALID))
            {
                *DTC = pDemObdDTC[DemObdDTCRef].DemJ1939DTCValue;
            }
            else
            {
                *DTC = 0x00UL;
            }
            break;
        case DEM_DTC_FORMAT_OBD:
            DemObdDTCRef = pDemDTC[DTCRef].DemObdDTCRef;
            if ((DemObdDTCRef != DEM_OBD_DTC_INVALID)
                && (pDemObdDTC[DemObdDTCRef].DemDtcValue != DEM_OBD_DTC_CFG_INVALID))
            {
                *DTC = (uint32)pDemObdDTC[DemObdDTCRef].DemDtcValue;
            }
            else
            {
                *DTC = 0x00UL;
            }
            break;
        default:
            /*idle*/
            break;
        }
    }
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
DEM_LOCAL FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE) Dem_DcmCheckDTCFilterStatus(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2CONST(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
    P2CONST(Dem_UdsStatusByteType, AUTOMATIC, DEM_APPL_DATA) DTCStatus)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Dem_ReturnGetNextFilteredElementType ret = DEM_FILTERED_NO_MATCHING_ELEMENT;
    const Dem_DTCFilterInfoType* pFilter = &DemDTCFilterInfo;

    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETNEXTFILTEREDDTC, DEM_E_UNINIT);
    }
    else if ((DTCStatus == NULL_PTR) || (DTC == NULL_PTR))
    {
        DEM_DET_REPORT(DEM_SID_DCMGETNEXTFILTEREDDTC, DEM_E_PARAM_POINTER);
    }
    /* req SWS_Dem_00653 */
    else if (
        (pFilter->DTCKind == DEM_DTC_KIND_EMISSION_REL_DTCS)
        && ((pFilter->DTCStatusMask == DEM_UDS_STATUS_CDTC) || (pFilter->DTCStatusMask == DEM_UDS_STATUS_PDTC)
            || (pFilter->DTCOrigin == DEM_DTC_ORIGIN_PERMANENT_MEMORY)))
    {
        ret = DEM_FILTERED_OK;
    }
    else
    {
        if (Dem_Pending == TRUE)
        {
            ret = DEM_FILTERED_PENDING;
        }
        else
        {
            ret = DEM_FILTERED_OK;
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Clears single DTCs, as well as groups of DTCs. This API is intended
                       for the Dcm. It can only be used through the RTE (due to work-around
                       described below SWS_Dem_00009), and therefore no declaration is exported via
 Dem_Dcm.h.
 * ServiceId           none
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      DTC:Defines the DTC in respective format, that shall be
 *                          cleared from the event memory. If the DTC fits to a
 *                          DTC group number, all DTCs of the group shall be cleared.
 *                       DTCFormat:Defines the input-format of the provided DTC value.
 *                       DTCOrigin:If the Dem supports more than one event memory
 *                          this parameter is used to select the source memory
 *                          the DTCs shall be read from.
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Status of the operation of type Dem_ReturnClearDTCType.
 */
/*************************************************************************/
DEM_LOCAL FUNC(Dem_ReturnClearDTCType, DEM_CODE)
    Dem_DcmSubClearDTC(uint32 DTC, Dem_DTCFormatType DTCFormat, Dem_DTCOriginType DTCOrigin)
{
    Dem_ClearDTCInfoType* pClr = &DemClearDTCInfo;
    uint16 dtcRef;
    Dem_ReturnClearDTCType ret = DEM_CLEAR_FAILED;

    if (0xFFFFFFUL == (DTC & 0xFFFFFFUL))
    {
        pClr->ClearAllGroup = TRUE;
    }
#if (DEM_CLEAR_DTCLIMITATION == DEM_ALL_SUPPORTED_DTCS)
    else
    {
        /* req SWS_Dem_01100 */
        dtcRef = Dem_GetDTCIndex(DTC, DTCFormat);
        /* PRQA S 3415++ */ /* MISRA Rule 13.5 */
        if (DEM_DTC_REF_INVALID == dtcRef)
        /* PRQA S 3415-- */ /* MISRA Rule 13.5 */
        {
#if (DEM_GROUP_OF_DTC_NUM > 0)
            if (DEM_GROUP_OF_DTC_INVALID == Dem_GetDTCGroupIndex(DTC))
#endif
            {
                Dem_ResetDemClearDTCInfo();
                ret = DEM_CLEAR_WRONG_DTC;
            }
        }
        else
        {
            if (DemDTCGeneralStatus[dtcRef].SuppressionStatus == TRUE)
            {
                Dem_ResetDemClearDTCInfo();
                ret = DEM_CLEAR_WRONG_DTC;
            }
        }
        if (DEM_CLEAR_FAILED == ret)
        {
            pClr->ClearAllGroup = FALSE;
            pClr->DTCGroupIndex = Dem_GetDTCGroupIndex(DTC);
            if (pClr->DTCGroupIndex == DEM_GROUP_OF_DTC_INVALID)
            {
                pClr->DTCIndex = Dem_GetDTCIndex(DTC, DTCFormat);
                if (pClr->DTCIndex == DEM_DTC_REF_INVALID)
                {
                    Dem_ResetDemClearDTCInfo();
                    ret = DEM_CLEAR_WRONG_DTC;
                }
            }
        }
    }
#endif
    if (DEM_CLEAR_FAILED == ret)
    {
        pClr->memDest = DTCOrigin;
        Dem_Pending = TRUE;
        Dem_ClearDTCProcess();
#if (DEM_CLEAR_DTCBEHAVIOR == DEM_CLRRESP_VOLATILE)
        /*req SWS_Dem_00570 */
        Dem_ResetDemClearDTCInfo();
        ret = DEM_CLEAR_OK;
#elif (DEM_CLEAR_DTCBEHAVIOR == DEM_CLRRESP_NONVOLATILE_TRIGGER)
        /* req SWS_Dem_00571 */
        Dem_ClearNonvolatileStatus = DEM_CLEAR_NONVOLATILE_START;
        Dem_ResetDemClearDTCInfo();
        ret = DEM_CLEAR_OK;
#elif (DEM_CLEAR_DTCBEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
        /* req SWS_Dem_00663 */
        Dem_ClearNonvolatileStatus = DEM_CLEAR_NONVOLATILE_START;
        ret = DEM_CLEAR_PENDING;
#endif
    }
    return ret;
}

#if (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_ON)
/*************************************************************************/
/*
 * Brief               Dem_CheckEventInternalMemDest
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      DTCOrigin
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              uint8
 */
/*************************************************************************/
DEM_LOCAL FUNC(uint8, DEM_CODE) Dem_CheckEventInternalMemDest(Dem_DTCOriginType DTCOrigin, uint16 IntId)
{
    uint8 iloop = 0;
    Std_ReturnType ret = DEM_MEM_DEST_INVALID;
    P2CONST(Dem_DTCAttributesType, AUTOMATIC, DEM_CONST)
    pDTCAttrCfg = Dem_EventDTCAttributesCfg(IntId);

    if (pDTCAttrCfg != NULL_PTR)
    {
        while (iloop < DEM_MEM_DEST_MAX_NUM_OF_DTC) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
        {
            if (pDTCAttrCfg->DemMemoryDestinationRef[iloop] == DTCOrigin)
            {
                ret = iloop;
                break;
            }
            iloop++;
        }
    }

    return ret;
}
#endif

/*************************************************************************/
/*
 * Brief               Dem_CheckDTCFormat
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      DTCOrigin
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              uint8
 */
/*************************************************************************/
DEM_LOCAL FUNC(boolean, DEM_CODE) Dem_CheckDTCFormat(uint8 dtcFormat, uint16 dtcRef)
{
    boolean ret = FALSE;
    const Dem_DTCType* pDemDTC = DemPbCfgPtr->DemDTC;
    const DemObdDTCType* pDemObdDTC = DemPbCfgPtr->DemObdDTC;
    uint16 obdDtcRef = pDemDTC[dtcRef].DemObdDTCRef;

    if ((DEM_DTC_FORMAT_UDS == dtcFormat) && (dtcRef != DEM_DTC_REF_INVALID)
        && (pDemDTC[dtcRef].DemDtcValue != DEM_DTC_CFG_INVALID))
    {
        ret = TRUE;
    }
    else if (
        (DEM_DTC_FORMAT_OBD == dtcFormat)
        && ((dtcRef != DEM_DTC_REF_INVALID) && (obdDtcRef != DEM_OBD_DTC_INVALID)
            && (pDemObdDTC[obdDtcRef].DemDtcValue != DEM_OBD_DTC_CFG_INVALID)))
    {
        ret = TRUE;
    }
    else if (
        (DEM_DTC_FORMAT_J1939 == dtcFormat)
        && ((dtcRef != DEM_DTC_REF_INVALID) && (obdDtcRef != DEM_OBD_DTC_INVALID)
            && (pDemObdDTC[obdDtcRef].DemJ1939DTCValue != DEM_J1939_DTC_CFG_INVALID)))
    {
        ret = TRUE;
    }
    else
    {
        /* idle */
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Dem_EventTestFailed
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId && pEventBuffer
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
/* PRQA S 3432,1532++ */ /* MISRA Rule 20.7,8.7 */
FUNC(void, DEM_CODE) Dem_EventTestFailed(Dem_EventIdType IntId)
/* PRQA S 3432,1532-- */ /* MISRA Rule 20.7,8.7 */
{
    const Dem_EventParameterType* pEventCfg = &DemPbCfgPtr->DemEventParameter[IntId];
    Dem_EventRelateInformationType* pEvent = &DemEventRelateInformation[IntId];
    uint16 DTCRef = DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef;
    const Dem_DTCAttributesType* pDTCAttr = Dem_EventDTCAttributesCfg(IntId);
#if (DEM_OCCURRENCE_COUNTER_PROCESSING == DEM_PROCESS_OCCCTR_CDTC)
    Dem_EventMemEntryType* Entry;
#endif
    uint8 MemDestMaxNumOfDtc = Dem_GetMemDestMaxNumOfDtc();

    if (0x00u == (pEvent->UdsStatus & DEM_UDS_STATUS_TF))
    {
        for (uint8 iloop = 0; iloop < MemDestMaxNumOfDtc; iloop++)
        {
            uint8 pMemDest = pDTCAttr->DemMemoryDestinationRef[iloop];
            if ((DEM_MEM_DEST_INVALID != pMemDest) && (DemMemDestCfg[pMemDest].ExtId == DEM_DTC_ORIGIN_PRIMARY_MEMORY))
            {
                /* req SWS_Dem_00219 */
                Dem_SetOccurrenceEvent(pEvent->IntId, DEM_UDS_STATUS_TF);
            }
        }
        DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_TFBITCHANGE);
    }
    /* req SWS_Dem_00036 */
    /* bit 0 1 5*/
    DEM_FLAGS_SET(pEvent->UdsStatus, DEM_UDS_STATUS_TF | DEM_UDS_STATUS_TFTOC | DEM_UDS_STATUS_TFSLC);
    /*bit4 6 */
    DEM_FLAGS_CLR(pEvent->UdsStatus, DEM_UDS_STATUS_TNCTOC | DEM_UDS_STATUS_TNCSLC);
    DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_ACTIVE);

#if (DEM_STATUSINDICATOR30ENABLE == STD_ON)
    /*The bit is set to 1 when the FDC10 reach a value in the range[unconfirmedDTCLimit, +127]*/
    DEM_FLAGS_SET(
        pEvent->StatusIndicator30,
        DEM_SI30_DTC_UC | DEM_SI30_DTC_UCTOC | DEM_SI30_DTC_UCSLC | DEM_SI30_DTC_SSLC); /*bit 0,1,2,4*/
    /*bit 7: The bit is set to 1 when the FDC10 reach the value +127.*/
    DEM_FLAGS_SET(pEvent->StatusIndicator30, DEM_SI30_DTC_TSFLC);
#endif /* DEM_STATUSINDICATOR30ENABLE == STD_ON */

#if ((DEM_OCC6EANLE == STD_ON) || (DEM_OCC4EANLE == STD_ON))
    if (0x00u == DEM_FLAGS_ISSET(pEvent->EventStatusFlag, DEM_EVENT_FLAGS_THISOPISFAILED))
    {
        DEM_FLAGS_SET(pEvent->EventStatusFlag, DEM_EVENT_FLAGS_THISOPISFAILED);
#if (DEM_OCC6EANLE == STD_ON)
        /* In each operation cycle the counter shall be incremented as soon as FDC10 has reached the value +127 for the
         * ﬁrst time. */
        if (pEvent->OCC6 < 0xFFu)
        {
            pEvent->OCC6++;
        }
        /* Do not clear OCC6 in this cycle if fail occured. */
        DEM_FLAGS_CLR(pEvent->EventStatusFlag, DEM_EVENT_FLAGS_OCC6CLR);
#endif /* DEM_OCC6EANLE == STD_ON */
#if (DEM_OCC4EANLE == STD_ON)
        /* In each operaƟon cycle the counter shall be incremented as soon as FDC10 has reached a value that is equal to
         * or greater than its unconﬁrmedDTCLimit for the ﬁrst Ɵme. */
        if (pEvent->OCC4 < 0xFFu)
        {
            pEvent->OCC4++;
        }
#endif /* DEM_OCC4EANLE == STD_ON */
    }
#endif /*DEM_OCC6EANLE == STD_ON) || (DEM_OCC4EANLE == STD_ON */

    Dem_EventTestFailedProcessOfComponent(pEvent, pEventCfg);
#if (DEM_TRIGGERONTESTFAILEDEANLE == STD_ON)
    if (DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_TFBITCHANGE) != 0x00u)
    {
        Dem_EventDataStorageTrigger(IntId, DEM_TRIGGER_ON_TEST_FAILED);
    }
#endif /* DEM_TRIGGERONTESTFAILEDEANLE == STD_ON */

#if (DEM_OCCURRENCE_COUNTER_PROCESSING == DEM_PROCESS_OCCCTR_TF)
    /*SWS_Dem_00524 SWS_Dem_00625 */
    if (((NULL_PTR != Dem_CheckEventMemEntryExistsAlready(IntId)) && (pEvent->OccurrenceCounter < 0xFFu))
        && ((DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF) != 0u)
            && (DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_TFBITCHANGE) != 0u)))
    {
        pEvent->OccurrenceCounter++;
    }
#elif (DEM_OCCURRENCE_COUNTER_PROCESSING == DEM_PROCESS_OCCCTR_TFTOC)
    if ((pEvent->OccurrenceCounter < 0xFFu) && (NULL_PTR != Dem_CheckEventMemEntryExistsAlready(IntId))
        && ((DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TFTOC) != 0u)
            && (DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_TFBITCHANGE) != 0u)))
    {
        pEvent->OccurrenceCounter++;
    }
#endif
    /* if event failed clear anging counter*/
    pEvent->AgingCounter = 0;
#if (DEM_STATUSINDICATOR30ENABLE == STD_ON)
    /*The bit is reset to 0 when the aged DTC reoccurs*/
    if (0x00u != DEM_FLAGS_ISSET(pEvent->StatusIndicator30, DEM_SI30_DTC_AGED))
    {
        DEM_FLAGS_CLR(pEvent->StatusIndicator30, DEM_SI30_DTC_AGED);
    }
#endif /* DEM_STATUSINDICATOR30ENABLE == STD_ON */
    /* Check PendingDTC bit */
    if (0x00u == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_PDTC))
    {
        /* req SWS_Dem_00784 req SWS_Dem_00801 req SWS_Dem_00813  bit 2*/
        DEM_FLAGS_SET(pEvent->UdsStatus, DEM_UDS_STATUS_PDTC);

#if (DEM_TRIGGERONPENDINGEDEANLE == STD_ON)
        Dem_EventDataStorageTrigger(IntId, DEM_TRIGGER_ON_PENDING);
#endif /* DEM_TRIGGERONPENDINGEDEANLE == STD_ON */
    }
#if (DEM_TRIGGER_ON_PENDING == DEM_EVENT_MEMORY_ENTRY_STORAGE_TRIGGER)
#if (DEM_TRIGGERONPENDINGEDEANLE == STD_ON)
    else
    {
        /* req SWS_Dem_00922 */
        if (DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_TFBITCHANGE) != 0x00u)
        {
            Dem_EventDataStorageTrigger(IntId, DEM_TRIGGER_ON_PENDING);
        }
    }
#endif /* DEM_TRIGGERONPENDINGEDEANLE == STD_ON */
#endif /* DEM_TRIGGER_ON_PENDING == DEM_EVENT_MEMORY_ENTRY_STORAGE_TRIGGER */
    /* confirmedLevel reached? */
    if (0x00u == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_CDTC))
    {
        /* req SWS_Dem_00528 */
        if ((pEvent->FailureCounter + 1u) >= pEventCfg->DemEventFailureCycleCounterThreshold)
        {
#if (DEM_EVENT_COMBINATION_SUPPORT == DEM_EVCOMB_ONSTORAGE)
            boolean Find = FALSE;
            const Dem_EventParameterType* pEventParameter = DemPbCfgPtr->DemEventParameter;
            if (DEM_DTC_REF_INVALID != DTCRef)
            {
                const Dem_DTCType* pDemDTC = &DemPbCfgPtr->DemDTC[DTCRef];
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
                const Dem_EventIdType EventRefNum = pDemDTC->EventRefNum;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
                const Dem_EventIdType EventRefStart = pDemDTC->EventRefStart;
                const Dem_EventIdType* pDTCMapping = &DemPbCfgPtr->DemDTCMapping[EventRefStart];
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
                for (uint16 iloop = 0u; (iloop < EventRefNum) && (Find == FALSE); iloop++)
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
                {
                    if ((DTCRef == pEventParameter[*pDTCMapping].DemDTCRef)
                        && (DEM_FLAGS_ISSET(DemEventRelateInformation[*pDTCMapping].UdsStatus, DEM_UDS_STATUS_CDTC)
                            != 0x00u))
                    {
                        Find = TRUE;
                    }
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
                    pDTCMapping++;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
                }
            }
            if (Find == FALSE)
#endif /* DEM_EVENT_COMBINATION_SUPPORT == DEM_EVCOMB_ONSTORAGE */
            {
                if ((DEM_DTC_REF_INVALID != DTCRef) && (pDTCAttr != NULL_PTR))
                {
                    const uint8* pMemDest = pDTCAttr->DemMemoryDestinationRef;
#if (DEM_MEM_DEST_MAX_NUM_OF_DTC > 1u)
                    for (uint8 index = 0; index < DEM_MEM_DEST_MAX_NUM_OF_DTC; index++)
#else
                    uint8 index = 0;
#endif /* DEM_MEM_DEST_MAX_NUM_OF_DTC > 1u */
                    {
                        if ((pMemDest[index] != DEM_MEM_DEST_INVALID)
                            && (DemMemDestCfg[pMemDest[index]].ExtId == DEM_DTC_ORIGIN_PRIMARY_MEMORY))
                        {
                            Dem_SetOccurrenceEvent(IntId, DEM_UDS_STATUS_CDTC); /* SWS_Dem_00219 */
                        }
                    }
                }
                /* req SWS_Dem_00700 */
                if ((DEM_BITS_ISNSET(DemOperationCycleStatus, pEventCfg->DemOperationCycleRef))
                    && (DemPbCfgPtr->DemOperationCycle[pEventCfg->DemOperationCycleRef].DemOperationCycleType
                        == DEM_OPCYC_OBD_DCY))
                {
                    /*idle*/
                }
                else
                {
                    /* bit 3 */
                    DEM_FLAGS_SET(pEvent->UdsStatus, DEM_UDS_STATUS_CDTC);
#if (DEM_OCC6EANLE == STD_ON)
                    /*  The counter may also be cleared when the conﬁrmedDTCLimit is reached. */
                    pEvent->OCC6 = 0;
#endif /* DEM_OCC6EANLE == STD_ON */
                }
#if (DEM_TRIGGERONCONFIRMEDEDEANLE == STD_ON)
                /* req SWS_Dem_00785 SWS_Dem_00802 SWS_Dem_00814*/
                Dem_EventDataStorageTrigger(IntId, DEM_TRIGGER_ON_CONFIRMED);
#endif /* DEM_TRIGGERONCONFIRMEDEDEANLE == STD_ON */
            }
        }
    }
    else
    {
#if (DEM_TRIGGERONCONFIRMEDEDEANLE == STD_ON)
#if (                                                                    \
    (DEM_TRIGGER_ON_CONFIRMED == DEM_EVENT_MEMORY_ENTRY_STORAGE_TRIGGER) \
    && (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_OFF))
        /* req SWS_Dem_00923 */
        if (DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_TFBITCHANGE) != 0x00u)
        {
            Dem_EventDataStorageTrigger(IntId, DEM_TRIGGER_ON_CONFIRMED);
        }
#endif
#endif /* DEM_TRIGGERONCONFIRMEDEDEANLE == STD_ON */
    }
#if (DEM_OCCURRENCE_COUNTER_PROCESSING == DEM_PROCESS_OCCCTR_CDTC)
    /* req SWS_Dem_00580 req SWS_Dem_00625 */
    if ((pEvent->OccurrenceCounter < 0xFFu) && (NULL_PTR != Dem_CheckEventMemEntryExistsAlready(IntId))
        && ((DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF) != 0)
            && (DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_TFBITCHANGE) != 0))
        && (DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_CDTC) != 0))
    {
        /*  occurrence counter counter by one ,trigger by TF bit transition from 0 to 1
         * if CDTC is set */
        pEvent->OccurrenceCounter++;
    }
#endif /* DEM_OCCURRENCE_COUNTER_PROCESSING == DEM_PROCESS_OCCCTR_CDTC */
    /* Check WarningIndicatorOnCondition */
    if (TRUE == Dem_CheckWIROn(pEvent, pEventCfg))
    {
        /* bit 7 */
        DEM_FLAGS_SET(pEvent->UdsStatus, DEM_UDS_STATUS_WIR);
#if (DEM_STATUSINDICATOR30ENABLE == STD_ON)
        /*The bit is set to 1 when DTC status bit no 7 is set to 1. In all cases, FDC10 has reached the value +127,
         * since DTC information was latest cleared, before the bit is set to 1 and warring indicator is requested*/
        if (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF))
        {
            DEM_FLAGS_SET(pEvent->StatusIndicator30, DEM_SI30_DTC_WIRSLC); /*bit 5*/
        }
#endif /* DEM_STATUSINDICATOR30ENABLE == STD_ON */
    }
    if (DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_TFBITCHANGE) != 0x00u)
    {
        DEM_FLAGS_CLR(pEvent->Status, DEM_EVENT_STATUS_TFBITCHANGE);
    }
    /* update the combination event dtc status*/
    Dem_UpdateCombinedDtcStatus(IntId);
    Dem_ResponseOnDtcStatusChange(IntId);
}

/*************************************************************************/
/*
 * Brief               Dem_EventTestPassed
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId && pEventBuffer
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
/* PRQA S 3432,1532++ */ /* MISRA Rule 20.7,8.7 */
FUNC(void, DEM_CODE) Dem_EventTestPassed(Dem_EventIdType IntId)
/* PRQA S 3432,1532-- */ /* MISRA Rule 20.7,8.7 */
{
    Dem_EventRelateInformationType* pEvent = &DemEventRelateInformation[IntId];
    uint16 TempRef = DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef;

    if (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF))
    {
        DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_TFBITCHANGE);
    }
#if (DEM_OCC6EANLE == STD_ON)
    if (0x00u == DEM_FLAGS_ISSET(pEvent->EventStatusFlag, DEM_EVENT_FLAGS_THISOPISFAILED))
    {
        if (0x00u != DEM_FLAGS_ISSET(pEvent->EventStatusFlag, DEM_EVENT_FLAGS_OCC6CLR))
        {
            /* The counter shall be cleared in the beginning of the operation cycle following an operation
             * cycle where FDC10 did not reach the value +127 at any time but -128 (the counter shall
             * maintain its value if did not reach the value -128). */
            pEvent->OCC6 = 0;
        }
    }
#endif /* DEM_OCC6EANLE == STD_ON */
#if (DEM_STATUSINDICATOR30ENABLE == STD_ON)
    /*The bit is reset to 0 when the FDC10 reach the value -128.*/
    DEM_FLAGS_CLR(pEvent->StatusIndicator30, DEM_SI30_DTC_UC); /*bit 0*/
#endif                                                         /* DEM_STATUSINDICATOR30ENABLE == STD_ON */
    /* bit 0 6 4*/
    DEM_FLAGS_CLR(pEvent->UdsStatus, DEM_UDS_STATUS_TF | DEM_UDS_STATUS_TNCTOC | DEM_UDS_STATUS_TNCSLC);
    DEM_FLAGS_CLR(pEvent->Status, DEM_EVENT_STATUS_ACTIVE);
    Dem_InterEventTestPassed(IntId);
    if (TempRef != DEM_DTC_REF_INVALID)
    {
        /* req SWS_Dem_00698 */
#if (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_OFF)
        Dem_CDTCOverFlowCheck(IntId);
#endif
        /* update the combination event dtc status*/
        Dem_UpdateCombinedDtcStatus(IntId);
        Dem_ResponseOnDtcStatusChange(IntId);
#if (DEM_MAX_NUMBER_PRESTORED_FF > 0)
        /* req SWS_Dem_00465 */
        (void)Dem_InterClearPrestoredFreezeFrame(IntId);
#endif
    }
}

/*************************************************************************/
/*
 * Brief               Dem_EntryChronologicalOrderUpdate
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
/* PRQA S 3432,1532++ */ /* MISRA Rule 20.7,8.7 */
DEM_LOCAL FUNC(void, DEM_CODE) Dem_EntryChronologicalOrderUpdate(
    Dem_EventIdType IntId,
    uint8 memDest,
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA) pEntry)
/* PRQA S 3432,1532-- */ /* MISRA Rule 20.7,8.7 */
{
    uint8 index;
    uint8 iloop;
    uint8 EntryNum = DemMemDestCfg[memDest].EntryNum;
    Dem_EventMemEntryType* EntryList = DemMemDestCfg[memDest].EntryList;

    for (iloop = 0; iloop < EntryNum; iloop++)
    {
#if (DEM_EVENT_COMBINATION_SUPPORT == DEM_EVCOMB_ONSTORAGE)
        const Dem_EventParameterType* pEventParameter = DemPbCfgPtr->DemEventParameter;
        if ((EntryList[iloop].EventId != 0u)
            && (pEventParameter[Dem_GetEventInternalId(EntryList[iloop].EventId)].DemDTCRef
                == pEventParameter[Dem_GetEventInternalId(pEntry->EventId)].DemDTCRef))
#else
        if (pEntry->EventId == EntryList[iloop].EventId)
#endif
        {
            for (index = 0; index < EntryNum; index++)
            {
                if (pEntry->AbsTime < EntryList[index].AbsTime)
                {
                    EntryList[index].AbsTime--;
                }
            }
            pEntry->AbsTime = DemMemDestInfo[memDest].RecordNum;
#if (DEM_EVENT_COMBINATION_SUPPORT == DEM_EVCOMB_ONSTORAGE)
            /*Update the DEM_EVENT_COMBINATION event new ID*/
            pEntry->EventId = Dem_GetEventExternalId(IntId);
#endif
            break;
        }
    }
#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_ONSTORAGE)
    DEM_UNUSED(IntId);
#endif
}

/*************************************************************************/
/*
 * Brief               Dem_MemEntryAllocate
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId && MemDestIndex
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Dem_EventMemEntryType*
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
FUNC(P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA), DEM_CODE)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
Dem_MemEntryAllocate(Dem_EventIdType IntId, uint8 memDest)
{
    Dem_EventMemEntryType* pEntry = NULL_PTR;
    Dem_MemDestInfoType* pMem = &DemMemDestInfo[memDest];
    Dem_EventRelateInformationType* pEvent = &DemEventRelateInformation[IntId];

    /* req SWS_Dem_01130 */
    if ((E_OK != Dem_CheckEntryAlreadyAllocated(IntId, memDest, &pEntry))
        || (DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_CONSECUTIVE_FAULT) != 0x00u))
    {
        pEntry = NULL_PTR;
    }
    else if (
        (0x00u == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_STORAGE_CONDICTION))
        && (DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_TFBITCHANGE) != 0x00u))
    {
        /*SWS_Dem_00458 SWS_Dem_00455 SWS_Dem_00591 */
        DEM_FLAGS_CLR(pEvent->Status, DEM_EVENT_STATUS_TFBITCHANGE);
        pEntry = NULL_PTR;
    }
    /* req SWS_Dem_00242 Defines the group of DTC that shall be disabled to store in event memory*/
    else if (0x00u == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_DTC_SETTING))
    {
        /* idle */
    }
    /* Entry already stored */
    else if (pEntry == NULL_PTR)
    {
        /*return NULL_PTR means no idle memory*/
        pEntry = Dem_MemEntryGet(0x00, memDest);
        if (pEntry == NULL_PTR)
        {
            pMem->OverFlow = TRUE;
#if (DEM_EVENT_DISPLACEMENT_STRATEGY != DEM_DISPLACEMENT_NONE)
            /* SWS_Dem_00401 SWS_Dem_00402 SWS_Dem_00407 */
            pEntry = Dem_EventDisplacementProcess(IntId, memDest);
#endif
        }
        else
        {
            /* req SWS_Dem_00475 */
            Dem_CallDataChangedOfMemEntryAllocate(IntId);
        }
        if (pEntry != NULL_PTR)
        {
            if (pMem->RecordNum < DemMemDestCfg[memDest].EntryNum)
            {
                pMem->RecordNum++;
            }
            pEntry->EventId = Dem_GetEventExternalId(IntId);
            /* SWS_Dem_00412 search the time order,give the newer time order*/
            pEntry->AbsTime = pMem->RecordNum;
#if ((DEM_FREEZE_FRAME_CLASS_NUM > 0u) || (DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0u) || (DEM_PID_CLASS_NUM > 0u))
            pEntry->FFNum = 0;
            pEntry->LatestFF = 0;
#endif /* DEM_FREEZE_FRAME_CLASS_NUM > 0u || DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0u || DEM_PID_CLASS_NUM > 0u */
            /* req SWS_Dem_00523 */
            pEvent->OccurrenceCounter = 0;
#if (DEM_CYCLESSINCEFRISTFAILEDEANLE == STD_ON)
            pEvent->CyclesSinceFirstFailed = 0; /* SWS_Dem_00776 */
#endif                                          /* DEM_CYCLESSINCEFRISTFAILEDEANLE == STD_ON */
#if ((DEM_CYCLESSINCEFRISTFAILEDEANLE == STD_ON) || (DEM_CYCLESSINCELASTFAILEDEANLE == STD_ON))
            DEM_FLAGS_SET(pEvent->EventStatusFlag, DEM_EVENT_FLAGS_CYCLECOUNTER);
#endif /* DEM_CYCLESSINCEFRISTFAILEDEANLE == STD_ON|| DEM_CYCLESSINCELASTFAILEDEANLE == STD_ON */
#if (DEM_CYCLESSINCELASTFAILEDEANLE == STD_ON)
            pEvent->CyclesSinceLastFailed = 0; /* SWS_Dem_00771 */
#endif
            pEvent->FailureCounter = 0; /*SWS_Dem_00780*/
        }
    }
    else
    {
        /* req SWS_Dem_00787 */
        if (DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_TFBITCHANGE) != 0x00u)
        {
#if (DEM_CYCLESSINCELASTFAILEDEANLE == STD_ON)
            /* SWS_Dem_00772 */
            pEvent->CyclesSinceLastFailed = 0;
#endif /* DEM_CYCLESSINCELASTFAILEDEANLE == STD_ON */
            /*Update the time order*/
            Dem_EntryChronologicalOrderUpdate(IntId, memDest, pEntry);
        }
    }
    return pEntry;
}

#if (DEM_NVRAM_BLOCKID_NUM > 0)
/*************************************************************************/
/*
 * Brief               Dem_CopyInfoFromInfo
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  pEvent
 * Return              None
 */
/*************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE) Dem_CopyInfoFromInfo(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2CONST(Dem_EventRelateInformationType, AUTOMATIC, DEM_APPL_DATA) Info,
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA) Entry)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Entry->UdsStatus = Info->UdsStatus;
    Entry->AgingCounter = Info->AgingCounter;
    Entry->OccurrenceCounter = Info->OccurrenceCounter;
    Entry->HealingCounter = Info->HealingCounter;
    Entry->FailureCounter = Info->FailureCounter;
#if (DEM_OCC6EANLE == STD_ON)
    Entry->OCC6 = Info->OCC6;
#endif /* DEM_OCC6EANLE == STD_ON */
#if (DEM_OCC4EANLE == STD_ON)
    Entry->OCC4 = Info->OCC4;
#endif /* DEM_OCC4EANLE == STD_ON */
#if (DEM_CYCLESSINCEFRISTFAILEDEANLE == STD_ON)
    Entry->CyclesSinceFirstFailed = Info->CyclesSinceFirstFailed;
#endif /* DEM_CYCLESSINCEFRISTFAILEDEANLE == STD_ON */
#if (DEM_CYCLESSINCELASTFAILEDEANLE == STD_ON)
    Entry->CyclesSinceLastFailed = Info->CyclesSinceLastFailed;
#endif /* DEM_CYCLESSINCELASTFAILEDEANLE == STD_ON */
#if ((DEM_CYCLESSINCEFRISTFAILEDEANLE == STD_ON) || (DEM_CYCLESSINCELASTFAILEDEANLE == STD_ON))
    Entry->EventStatusFlag = Info->EventStatusFlag;
#endif /* DEM_CYCLESSINCEFRISTFAILEDEANLE == STD_ON || DEM_CYCLESSINCELASTFAILEDEANLE == STD_ON */
#if (DEM_STATUSINDICATOR30ENABLE == STD_ON)
    Entry->StatusIndicator30 = Info->StatusIndicator30;
#endif /* DEM_STATUSINDICATOR30ENABLE == STD_ON */
#if (DEM_AGEDCOUNT_SAIC == STD_ON)
    Entry->AgedCounter = Info->AgedCounter;
#endif /* DEM_AGEDCOUNT_SAIC == STD_ON */
}

#if (DEM_NVRAM_DIVADED == STD_ON)
/*************************************************************************/
/*
 * Brief               Dem_IntWriteOneNvRAM
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  pEvent
 * Return              None
 */
/*************************************************************************/
FUNC(void, DEM_CODE) Dem_IntWriteOneNvRAM(uint16 IntId)
{
    uint16 blockRef;
    uint8 DemMemDestindex;
    uint16 iloop = 0;
    uint16 tempEventId = 0;
    uint16 blockIndex = 0;
    for (DemMemDestindex = 0; DemMemDestindex < DEM_MEM_DEST_TOTAL_NUM; DemMemDestindex++)
    {
        if (DemMemDestCfg[DemMemDestindex].ExtId == DEM_DTC_ORIGIN_PRIMARY_MEMORY)
        {
            for (iloop = 0; iloop < DEM_MAX_NUMBER_EVENT_ENTRY_PRIMARY; iloop++)
            {
                tempEventId = DemMemDestCfg[DemMemDestindex].EntryList[iloop].EventId;
                if (tempEventId != 0u)
                {
                    tempEventId = tempEventId - 1u;
                    if (IntId == tempEventId)
                    {
                        Dem_CopyInfoFromInfo(
                            &DemEventRelateInformation[tempEventId],
                            &DemMemDestCfg[DemMemDestindex].EntryList[iloop]);
                        blockRef = DemNvRamBlockId[blockIndex];
                        Dem_MemCopy(
                            NvM_BlockDescriptor[blockRef - 1u].NvmRamBlockDataAddress,
                            (const uint8*)&DemMemDestCfg[DemMemDestindex].EntryList[iloop],
                            ENTRY_STORAGE_LEN);
                        (void)NvM_CancelJobs(blockRef);
                        (void)NvM_WriteBlock(blockRef, NULL_PTR);
                        break;
                    }
                }
                blockIndex++;
            }
            break;
        }
    }
#if (DEM_MAX_NUMBER_EVENT_ENTRY_PERMANENT > 0)
    for (DemMemDestindex = 0; DemMemDestindex < DEM_MEM_DEST_TOTAL_NUM; DemMemDestindex++)
    {
        if (DemMemDestCfg[DemMemDestindex].ExtId == DEM_DTC_ORIGIN_PERMANENT_MEMORY)
        {
            for (iloop = 0; iloop < DEM_MAX_NUMBER_EVENT_ENTRY_PERMANENT; iloop++)
            {
                tempEventId = DemMemDestCfg[DemMemDestindex].EntryList[iloop].EventId;
                if (tempEventId != 0u)
                {
                    tempEventId = tempEventId - 1u;
                    if (IntId == tempEventId)
                    {
                        Dem_CopyInfoFromInfo(
                            &DemEventRelateInformation[tempEventId],
                            &DemMemDestCfg[DemMemDestindex].EntryList[iloop]);
                        blockRef = DemNvRamBlockId[blockIndex];
                        Dem_MemCopy(
                            NvM_BlockDescriptor[blockRef - 1u].NvmRamBlockDataAddress,
                            (const uint8*)&DemMemDestCfg[DemMemDestindex].EntryList[iloop],
                            ENTRY_STORAGE_LEN);
                        (void)NvM_CancelJobs(blockRef);
                        (void)NvM_WriteBlock(blockRef, NULL_PTR);
                        break;
                    }
                }
                blockIndex++;
            }
            break;
        }
    }
#endif
}
#endif /* DEM_NVRAM_DIVADED == STD_ON */
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
FUNC(void, DEM_CODE) Dem_IntWriteNvRAM(void)
{
    uint16 blockRef;
    uint8 DemMemDestindex;
    uint16 iloop;
    uint16 tempEventId = 0;
    uint16 blockIndex = 0;
#if (DEM_NVRAM_DIVADED == STD_ON)
    for (DemMemDestindex = 0; DemMemDestindex < DEM_MEM_DEST_TOTAL_NUM; DemMemDestindex++)
    {
        if (DemMemDestCfg[DemMemDestindex].ExtId == DEM_DTC_ORIGIN_PRIMARY_MEMORY)
        {
            for (iloop = 0; iloop < DEM_MAX_NUMBER_EVENT_ENTRY_PRIMARY; iloop++)
            {
                tempEventId = DemMemDestCfg[DemMemDestindex].EntryList[iloop].EventId;
                if (tempEventId != 0u)
                {
                    tempEventId = tempEventId - 1u;
                    Dem_CopyInfoFromInfo(
                        &DemEventRelateInformation[tempEventId],
                        &DemMemDestCfg[DemMemDestindex].EntryList[iloop]);
                }
                blockRef = DemNvRamBlockId[blockIndex];
                blockIndex++;
                Dem_MemCopy(
                    NvM_BlockDescriptor[blockRef - 1u].NvmRamBlockDataAddress,
                    (const uint8*)&DemMemDestCfg[DemMemDestindex].EntryList[iloop],
                    ENTRY_STORAGE_LEN);
                (void)NvM_CancelJobs(blockRef);
                (void)NvM_WriteBlock(blockRef, NULL_PTR);
            }
            break;
        }
    }
#if (DEM_MAX_NUMBER_EVENT_ENTRY_PERMANENT > 0)
    for (DemMemDestindex = 0; DemMemDestindex < DEM_MEM_DEST_TOTAL_NUM; DemMemDestindex++)
    {
        if (DemMemDestCfg[DemMemDestindex].ExtId == DEM_DTC_ORIGIN_PERMANENT_MEMORY)
        {
            for (iloop = 0; iloop < DEM_MAX_NUMBER_EVENT_ENTRY_PERMANENT; iloop++)
            {
                tempEventId = DemMemDestCfg[DemMemDestindex].EntryList[iloop].EventId;
                if (tempEventId != 0u)
                {
                    tempEventId = tempEventId - 1u;
                    Dem_CopyInfoFromInfo(
                        &DemEventRelateInformation[tempEventId],
                        &DemMemDestCfg[DemMemDestindex].EntryList[iloop]);
                }
                blockRef = DemNvRamBlockId[blockIndex];
                blockIndex++;
                Dem_MemCopy(
                    NvM_BlockDescriptor[blockRef - 1u].NvmRamBlockDataAddress,
                    (const uint8*)&DemMemDestCfg[DemMemDestindex].EntryList[iloop],
                    ENTRY_STORAGE_LEN);
                (void)NvM_CancelJobs(blockRef);
                (void)NvM_WriteBlock(blockRef, NULL_PTR);
            }
            break;
        }
    }
#endif
#else
    for (iloop = 0; iloop < DEM_MAX_NUMBER_EVENT_ENTRY_PRIMARY; iloop++)
    {
        /* PRQA S 2877++ */ /* MISRA Dir 4.1 */
        for (DemMemDestindex = 0; DemMemDestindex < DEM_MEM_DEST_TOTAL_NUM; DemMemDestindex++)
        /* PRQA S 2877-- */ /* MISRA Dir 4.1 */
        {
            if (DemMemDestCfg[DemMemDestindex].ExtId == DEM_DTC_ORIGIN_PRIMARY_MEMORY)
            {
                DemEventMemoryEntryStorage[iloop] = DemMemDestCfg[DemMemDestindex].EntryList[iloop];
                tempEventId = DemMemDestCfg[DemMemDestindex].EntryList[iloop].EventId;
            }
        }
        if (tempEventId != 0u)
        {
            tempEventId = tempEventId - 1u;
            Dem_CopyInfoFromInfo(&DemEventRelateInformation[tempEventId], &DemEventMemoryEntryStorage[iloop]);
        }
    }
#if (DEM_MAX_NUMBER_EVENT_ENTRY_PERMANENT > 0)
    for (; iloop < ENTRY_STORAGE_NUM; iloop++)
    {
        for (DemMemDestindex = 0; DemMemDestindex < DEM_MEM_DEST_TOTAL_NUM; DemMemDestindex++)
        {
            if (DemMemDestCfg[DemMemDestindex].ExtId == DEM_DTC_ORIGIN_PERMANENT_MEMORY)
            {
                DemEventMemoryEntryStorage[iloop] =
                    DemMemDestCfg[DemMemDestindex].EntryList[iloop - DEM_MAX_NUMBER_EVENT_ENTRY_PRIMARY];
                tempEventId =
                    DemMemDestCfg[DemMemDestindex].EntryList[iloop - DEM_MAX_NUMBER_EVENT_ENTRY_PRIMARY].EventId;
            }
        }
        if (tempEventId != 0u)
        {
            tempEventId = tempEventId - 1u;
            Dem_CopyInfoFromInfo(&DemEventRelateInformation[tempEventId], &DemEventMemoryEntryStorage[iloop]);
        }
    }
#endif
    blockRef = DemNvRamBlockId[blockIndex];
    blockIndex++;
    (void)NvM_CancelJobs(blockRef);
    (void)NvM_WriteBlock(blockRef, NULL_PTR);
#endif

    /* req SWS_Dem_00525 SWS_Dem_00389 SWS_Dem_01183 SWS_Dem_00391 SWS_Dem_00392 SWS_Dem_00393
     * SWS_Dem_00394 SWS_Dem_00395*/
    for (iloop = 0; iloop < DemPbCfgPtr->DemEventNum; iloop++)
    {
        DemEventRelateInformationStorage.UdsStatus[iloop] = DemEventRelateInformation[iloop].UdsStatus;
#if (DEM_STATUSINDICATOR30ENABLE == STD_ON)
        DemEventRelateInformationStorage.StatusIndicator30[iloop] = DemEventRelateInformation[iloop].StatusIndicator30;
#endif /* DEM_STATUSINDICATOR30ENABLE == STD_ON */
#if (DEM_AGEDCOUNT_SAIC == STD_ON)
        DemEventRelateInformationStorage.AgedCounter[iloop] = DemEventRelateInformation[iloop].AgedCounter;
#endif /* DEM_AGEDCOUNT_SAIC == STD_ON */
        Dem_InterIntWriteNvRAM(iloop, &DemEventRelateInformationStorage);
    }
#if (DEM_MAXFDCSINCELASTCLEAR_EANLE == STD_ON)
    for (iloop = 0; iloop < DEM_DTC_NUM; iloop++)
    {
        DemEventRelateInformationStorage.MaxFDCSinceLastClear[iloop] = FDCInfo[iloop].MaxFDCSinceLastClear;
    }
#endif /*DEM_MAXFDCSINCELASTCLEAR_EANLE == STD_ON  */
#if (DEM_OPERATION_CYCLE_STATUS_STORAGE == STD_ON)
    /* req SWS_Dem_00577*/
    for (iloop = 0; iloop < DEM_BITS_TO_BYTE(DemPbCfgPtr->DemOperationCycleNum); iloop++)
    {
        DemEventRelateInformationStorage.DemOperationCycleStatus[iloop] = DemOperationCycleStatus[iloop];
    }
#endif
    DemEventRelateInformationStorage.FirstDtcConfirmed = DemDTCByOccurrenceTimeInfo.FirstDtcConfirmed;
    DemEventRelateInformationStorage.FirstFailed = DemDTCByOccurrenceTimeInfo.FirstFailed;
    DemEventRelateInformationStorage.MostRecDtcConfirmed = DemDTCByOccurrenceTimeInfo.MostRecDtcConfirmed;
    DemEventRelateInformationStorage.MostRecentFailed = DemDTCByOccurrenceTimeInfo.MostRecentFailed;

    blockRef = DemNvRamBlockId[blockIndex];
    (void)NvM_CancelJobs(blockRef);
    (void)NvM_WriteBlock(blockRef, NULL_PTR);

#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
    DemOBDDataStorage.IgnUpCycleCounter = IgnUpCycleCounter;
    DemOBDDataStorage.ContinuousMICounter = ContinuousMICounter;
    DemOBDDataStorage.WarmUpCycleCounter = WarmUpCycleCounter;
    DemOBDDataStorage.OBDDistanceMILOn = OBDDistanceMILOn;
    DemOBDDataStorage.DistSinceDtcCleared = DistSinceDtcCleared;
    DemOBDDataStorage.OBDTimeMILOn = OBDTimeMILOn;
    DemOBDDataStorage.OBDTimeDTCClear = OBDTimeDTCClear;
    DemOBDDataStorage.OBDB1Counter = OBDB1Counter;
#if (DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)
    DemOBDDataStorage.MasterContinuousMICounter = MasterContinuousMICounter;
#endif
    blockIndex++;
    blockRef = DemNvRamBlockId[blockIndex];
    (void)NvM_CancelJobs(blockRef);
    (void)NvM_WriteBlock(blockRef, NULL_PTR);
#endif
}
#endif /* DEM_NVRAM_BLOCKID_NUM > 0 */

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
