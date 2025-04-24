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
**  FILENAME    : Dem_Dcm.h                                                   **
**                                                                            **
**  Created on  :                                                             **
**  Author      : tao.yu                                                      **
**  Vendor      : i-soft                                                      **
**  DESCRIPTION : API declaration of DEM for DCM                              **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/

#ifndef DEM_DCM_H_
#define DEM_DCM_H_

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Dcm_Types.h"
#include "Dem.h"

#if (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_ON)
typedef struct
{
    uint8 DtcAbsTime;
    uint16 DtcRef;
} Dem_DTCReportByTimeOrderType;
#endif
/* DTCByOccurrenceTime */
typedef struct
{
    Dem_EventIdType FirstFailed;
    Dem_EventIdType MostRecentFailed;
    Dem_EventIdType FirstDtcConfirmed;
    Dem_EventIdType MostRecDtcConfirmed;
} Dem_DTCByOccurrenceTimeType;

/* DTC Filter */
typedef struct
{
    /* Filter */
    Dem_UdsStatusByteType DTCStatusMask;
    Dem_UdsStatusByteType RequestDTCStatusMask;
    Dem_DTCKindType DTCKind;
    Dem_DTCFormatType DTCFormat;
    Dem_DTCOriginType DTCOrigin;
    boolean FilterWithSeverity;
    Dem_DTCSeverityType DTCSeverityMask;
    boolean FilterForFaultDetectionCounter;
    boolean TimeOrder;
#if (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_ON)
    Dem_DTCReportByTimeOrderType DemDTCReportByTimeOrder[DEM_DTC_NUM];
    uint16 DemDTCTimeOrderNum;
#endif
    uint16 DemDemDTCReportNum;
    uint16 DemDTCReport[DEM_DTC_NUM];

    /* IsSet */
    boolean IsSet;
    /* Result */
    uint16 NumberOfFilteredDTC;
    uint16 CurrentIndex;
} Dem_DTCFilterInfoType;

typedef uint8 Dem_FilterForFDCType;
#define DEM_FILTER_FOR_FDC_NO  FALSE
#define DEM_FILTER_FOR_FDC_YES TRUE

typedef uint8 Dem_FilterWithSeverityType;
#define DEM_FILTER_WITH_SEVERITY_NO  FALSE
#define DEM_FILTER_WITH_SEVERITY_YES TRUE

typedef uint8 Dem_ReturnSetDTCFilterType;

typedef Dem_ReturnGetNextFilteredElementType Dem_ReturnGetNextFilteredDTCType;
#define DEM_FILTERED_NO_MATCHING_DTC DEM_FILTERED_NO_MATCHING_ELEMENT
#define DEM_FILTERED_WRONG_DTCKIND   0x04

typedef uint8 Dem_ReturnGetDTCOfFreezeFrameRecordType;
#define DEM_GET_DTCOFFF_OK                                                   0x00
#define DEM_GET_DTCOFFF_WRONG_RECORD                                         0x01
#define DEM_GET_DTCOFFF_NO_DTC_FOR_RECORD                                    0x02
#define DEM_GET_DTCOFFF_WRONG_DTCKIND                                        0x03

#define Dem_GetDTCOfFreezeFrameRecord(RecordNumber, DTCOrigin, DTCKind, DTC) DEM_GET_DTCOFFF_NO_DTC_FOR_RECORD

typedef uint8 Dem_ReturnGetFreezeFrameDataIdentifierByDTCType;
#define DEM_GET_ID_OK              0x00
#define DEM_GET_ID_WRONG_DTC       0x01
#define DEM_GET_ID_WRONG_DTCORIGIN 0x02
#define DEM_GET_ID_WRONG_DTCKIND   0x03
#define DEM_GET_ID_WRONG_FF_TYPE   0x04

#define Dem_GetFreezeFrameDataIdentifierByDTC(DTC, DTCKind, DTCOrigin, RecordNumber, ArraySize, DataId) \
    DEM_GET_ID_WRONG_FF_TYPE

typedef uint8 Dem_ReturnGetSizeOfFreezeFrameType;
#define DEM_GET_SIZEOFFF_OK            DEM_GETSIZEBYDTC_OK
#define DEM_GET_SIZEOFFF_WRONG_DTC     DEM_GETSIZEBYDTC_WRONG_DTC
#define DEM_GET_SIZEOFFF_WRONG_DTCOR   DEM_GETSIZEBYDTC_WRONG_DTCORIGIN
#define DEM_GET_SIZEOFFF_WRONG_DTCKIND DEM_GETSIZEBYDTC_WRONG_DTCORIGIN
#define DEM_GET_SIZEOFFF_WRONG_RNUM    DEM_GETSIZEBYDTC_WRONG_RECNUM
#define DEM_GET_SIZEOFFF_PENDING       DEM_GETSIZEBYDTC_PENDING

typedef uint32 Dem_DTCGroupType;

typedef uint8 Dem_ReturnControlDTCStorageType;
#define DEM_CONTROL_DTC_STORAGE_OK   DEM_CONTROL_DTC_SETTING_OK
#define DEM_CONTROL_DTC_STORAGE_N_OK DEM_CONTROL_DTC_SETTING_N_OK

typedef uint8 Dem_ReturnControlEventUpdateType;
#define DEM_CONTROL_EVENT_UPDATE_OK      DEM_CONTROL_DTC_SETTING_OK
#define DEM_CONTROL_EVENT_UPDATE_N_OK    DEM_CONTROL_DTC_SETTING_N_OK
#define DEM_CONTROL_EVENT_WRONG_DTCGROUP DEM_CONTROL_DTC_WRONG_DTCGROUP

typedef uint8 Dem_ReturnGetSizeOfExtendedDataRecordByDTCType;
#define DEM_GET_SIZEOFEDRBYDTC_OK      DEM_GETSIZEBYDTC_OK
#define DEM_GET_SIZEOFEDRBYDTC_W_DTC   DEM_GETSIZEBYDTC_WRONG_DTC
#define DEM_GET_SIZEOFEDRBYDTC_W_DTCOR DEM_GETSIZEBYDTC_WRONG_DTCORIGIN
#define DEM_GET_SIZEOFEDRBYDTC_W_DTCKI DEM_GET_SIZEOFEDRBYDTC_W_RNUM
#define DEM_GET_SIZEOFEDRBYDTC_W_RNUM  DEM_GETSIZEBYDTC_WRONG_RECNUM
#define DEM_GET_SIZEOFEDRBYDTC_PENDING DEM_GETSIZEBYDTC_PENDING

extern VAR(Dem_DTCFilterInfoType, AUTOMATIC) DemDTCFilterInfo;

extern VAR(Dem_DTCByOccurrenceTimeType, AUTOMATIC) DemDTCByOccurrenceTimeInfo;

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
extern FUNC(Dem_DTCTranslationFormatType, DEM_CODE) Dem_DcmGetTranslationType(void);

/*************************************************************************/
/*
 * Brief               Gets the DTC Status availability mask.
 * ServiceId           0x16
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]     DTCStatusMask:The value DTCStatusMask indicates the supported DTC status
 *                              bits from the Dem.
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              E_OK: get of DTC status mask was successful
 *                       E_NOT_OK: get of DTC status mask failed
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    Dem_DcmGetDTCStatusAvailabilityMask(P2VAR(Dem_UdsStatusByteType, AUTOMATIC, DEM_APPL_DATA) DTCStatusMask);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Gets the status of a DTC.
 * ServiceId           0x15
 * Sync/Async          Synchronous/Asynchronous
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
extern FUNC(Dem_ReturnGetStatusOfDTCType, DEM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    Dem_DcmGetStatusOfDTC(uint32 DTC, Dem_DTCOriginType DTCOrigin, P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DTCStatus);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Gets the severity of the requested DTC.
 * ServiceId           0x0e
 * Sync/Async          Synchronous/Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTC : Diagnostic Trouble Code in UDS format.
 * Param-Name[out]     DTCSeverity:This parameter contains the DTCSeverity according to ISO 14229-1.
 * Param-Name[in/out]  none
 * Return              Status of the operation of type Dem_ReturnGetSeverityOfDTCType.
 */
/*************************************************************************/
extern FUNC(Dem_ReturnGetSeverityOfDTCType, DEM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    Dem_DcmGetSeverityOfDTC(uint32 DTC, P2VAR(Dem_DTCSeverityType, AUTOMATIC, DEM_APPL_DATA) DTCSeverity);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

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
extern FUNC(Dem_ReturnGetFunctionalUnitOfDTCType, DEM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    Dem_DcmGetFunctionalUnitOfDTC(uint32 DTC, P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DTCFunctionalUnit);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

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
extern FUNC(Dem_ReturnSetFilterType, DEM_CODE) Dem_DcmSetDTCFilter(
    Dem_UdsStatusByteType DTCStatusMask,
    Dem_DTCKindType DTCKind,
    Dem_DTCFormatType DTCFormat,
    Dem_DTCOriginType DTCOrigin,
    boolean FilterWithSeverity,
    Dem_DTCSeverityType DTCSeverityMask,
    boolean FilterForFaultDetectionCounter);

/*************************************************************************/
/*
 * Brief               Gets the number of a filtered DTC.
 * ServiceId           0x17
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     NumberOfFilteredDTC:The number of DTCs matching the defined status mask.>
 * Param-Name[in/out]  none
 * Return              Status of the operation to retrieve a number of DTC from the Dem
 */
/*************************************************************************/
extern FUNC(Dem_ReturnGetNumberOfFilteredDTCType, DEM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    Dem_DcmGetNumberOfFilteredDTC(P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) NumberOfFilteredDTC);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Gets the next filtered DTC matching the filter criteria.
 * ServiceId           0x18
 * Sync/Async          Synchronous/Asynchronous
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
extern FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE) Dem_DcmGetNextFilteredDTC(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
    P2VAR(Dem_UdsStatusByteType, AUTOMATIC, DEM_APPL_DATA) DTCStatus);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

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
extern FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE) Dem_DcmGetNextFilteredDTCAndFDC(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
    P2VAR(sint8, AUTOMATIC, DEM_APPL_DATA) DTCFaultDetectionCounter);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Gets the next filtered DTC and its associated Severity matching the filter
                        criteria. The interface has an asynchronous behavior, because a large
                        number of DTCs has to be processed.
 * ServiceId           0x3d
 * Sync/Async          Asynchronous
 * Reentrancy          Re-entrant for different ClientIDs, Non re-entrant for same ClientId.
 * Param-Name[in]      ClientId: Unique client id, assigned to the instance of the calling module.
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
extern FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE) Dem_DcmGetNextFilteredDTCAndSeverity(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
    P2VAR(Dem_UdsStatusByteType, AUTOMATIC, DEM_APPL_DATA) DTCStatus,
    P2VAR(Dem_DTCSeverityType, AUTOMATIC, DEM_APPL_DATA) DTCSeverity,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DTCFunctionalUnit);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Sets a freeze frame record filter.
 * ServiceId           0x3f
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different ClientIds, non reentrant for the same ClientId.
 * Param-Name[in]      DTCFormat: Defines the output-format of the requested DTC values for the
 *                           sub-sequent API calls.
 * Param-Name[out]     NumberOfFilteredRecords:Number of freeze frame records currently stored
 *                           in the event memory.
 * Param-Name[in/out]  none
 * Return              Status of the operation to (re-)set a freeze frame record filter.
 */
/*************************************************************************/
extern FUNC(Dem_ReturnSetFilterType, DEM_CODE) Dem_DcmSetFreezeFrameRecordFilter(
    Dem_DTCFormatType DTCFormat,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) NumberOfFilteredRecords);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

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
extern FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE) Dem_DcmGetNextFilteredRecord(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) RecordNumber);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

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
extern FUNC(Dem_ReturnGetDTCByOccurrenceTimeType, DEM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    Dem_DcmGetDTCByOccurrenceTime(Dem_DTCRequestType DTCRequest, P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

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
extern FUNC(void, DEM_CODE) Dem_DcmControlDTCStatusChangedNotification(boolean TriggerNotification);

/*******************************************************************************
**       Access extended data records and FreezeFrame data                    **
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
extern FUNC(Dem_ReturnDisableDTCRecordUpdateType, DEM_CODE)
    Dem_DcmDisableDTCRecordUpdate(uint32 DTC, Dem_DTCOriginType DTCOrigin);

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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_DcmEnableDTCRecordUpdate(void);

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
extern FUNC(Dem_ReturnGetFreezeFrameDataByDTCType, DEM_CODE) Dem_DcmGetFreezeFrameDataByDTC(
    uint32 DTC,
    Dem_DTCOriginType DTCOrigin,
    uint8 RecordNumber,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Get the size of frozen frame data using DTC
 * ServiceId           0x1f
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTC : Diagnostic Trouble Code in UDS format
 *                       DTCOrigin : If the Dem supports more than one event memory,
 *                          this parameter is used to select the source memory
 *                          the DTCs shall be read from.
 *                       RecordNumber: This parameter is a unique identifier for a freeze
 *                          frame record as defined in ISO 15031-5 and ISO 14229-1.
 * Param-Name[out]     SizeOfFreezeFrame:The number of bytes in the requested freeze frame record
 * Param-Name[in/out]  none
 * Return              Gets the operation status of freezing frame data size
 */
/*************************************************************************/
extern FUNC(Dem_ReturnGetSizeOfDataByDTCType, DEM_CODE) Dem_DcmGetSizeOfFreezeFrameByDTC(
    uint32 DTC,
    Dem_DTCOriginType DTCOrigin,
    uint8 RecordNumber,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) SizeOfFreezeFrame);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

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
extern FUNC(Dem_ReturnGetExtendedDataRecordByDTCType, DEM_CODE) Dem_DcmGetExtendedDataRecordByDTC(
    uint32 DTC,
    Dem_DTCOriginType DTCOrigin,
    uint8 ExtendedDataNumber,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

#define DEM_RECORD_WRONG_DTCKIND 0x06
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
extern FUNC(Dem_ReturnGetSizeOfDataByDTCType, DEM_CODE) Dem_DcmGetSizeOfExtendedDataRecordByDTC(
    uint32 DTC,
    Dem_DTCOriginType DTCOrigin,
    uint8 ExtendedDataNumber,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) SizeOfExtendedDataRecord);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

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
extern FUNC(Dem_ReturnClearDTCType, DEM_CODE)
    Dem_DcmCheckClearParameter(uint32 DTC, Dem_DTCFormatType DTCFormat, Dem_DTCOriginType DTCOrigin);

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
/* PRQA S 0624,3449,3451++ */ /* MISRA Rule 8.3,8.5,8.5 */
FUNC(Dem_ReturnClearDTCType, DEM_CODE)
Dem_DcmClearDTC(uint32 DTC, Dem_DTCFormatType DTCFormat, Dem_DTCOriginType DTCOrigin);

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
FUNC(Dem_ReturnControlDTCSettingType, DEM_CODE)
Dem_DcmEnableDTCSetting(uint32 DTCGroup, Dem_DTCKindType DTCKind);
/* PRQA S 0624,3449,3451-- */ /* MISRA Rule 8.3,8.5,8.5 */

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
extern FUNC(Dem_ReturnControlDTCSettingType, DEM_CODE)
    Dem_DcmDisableDTCSetting(uint32 DTCGroup, Dem_DTCKindType DTCKind);

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
extern FUNC(Dem_ReturnClearDTCType, DEM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    Dem_DcmGetDTCSeverityAvailabilityMask(P2VAR(Dem_DTCSeverityType, AUTOMATIC, DEM_APPL_DATA) DTCSeverityMask);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

#endif /* DEM_DCM_H_ */

/*******************************************************************************
**                      end of file                                           **
*******************************************************************************/
