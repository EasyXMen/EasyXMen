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
 **  @file               : Dem_Error.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : Implementation file of Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_ERROR_H
#define DEM_ERROR_H

/* =================================================== inclusions =================================================== */
#include "Dem_Cfg.h"
#include "Det.h"
#if (DEM_FEATURE_MEMCOPY_USED_ILIB == STD_ON)
#include "IStdLib.h"
#endif

/* ===================================================== macros ===================================================== */
/**
 * @name DEM_SID_ENCODE
 * @{
 * @brief API IDs
 */
#define DEM_SID_GETVERSIONINFO                         (0x00u) /**< Dem_GetVersionInfo */
#define DEM_SID_PREINIT                                (0x01u) /**< Dem_PreInit */
#define DEM_SID_INIT                                   (0x02u) /**< Dem_Init */
#define DEM_SID_SHUTDOWN                               (0x03u) /**< Dem_Shutdown */
#define DEM_SID_SETEVENTSTATUS                         (0x04u) /**< Dem_SetEventStatus */
#define DEM_SID_RESETEVENTSTATUS                       (0x05u) /**< Dem_ResetEventStatus */
#define DEM_SID_PRESTOREFREEZEFRAME                    (0x06u) /**< Dem_PrestoreFreezeFrame */
#define DEM_SID_CLEARPRESTOREDFREEZEFRAME              (0x07u) /**< Dem_ClearPrestoredFreezeFrame */
#define DEM_SID_RESTARTOPERATIONCYCLE                  (0x08u) /**< Dem_RestartOperationCycle */
#define DEM_SID_RESETEVENTDEBOUNCESTATUS               (0x09u) /**< Dem_ResetEventDebounceStatus */
#define DEM_SID_GETDTCOFEVENT                          (0x0Du) /**< Dem_GetDTCOfEvent */
#define DEM_SID_GETSEVERITYOFDTC                       (0x0Eu) /**< Dem_GetSeverityOfDTC */
#define DEM_SID_SETDTCFILTER                           (0x13u) /**< Dem_SetDTCFilter */
#define DEM_SID_GETSTATUSOFDTC                         (0x15u) /**< Dem_GetStatusOfDTC */
#define DEM_SID_GETDTCSTATUSAVAILABILITYMASK           (0x16u) /**< Dem_GetDTCStatusAvailabilityMask */
#define DEM_SID_GETNUMBEROFFILTEREDDTC                 (0x17u) /**< Dem_GetNumberOfFilteredDTC */
#define DEM_SID_GETNEXTFILTEREDDTC                     (0x18u) /**< Dem_GetNextFilteredDTC */
#define DEM_SID_GETDTCBYOCCURRENCETIME                 (0x19u) /**< Dem_GetDTCByOccurrenceTime */
#define DEM_SID_DISABLEDTCRECORDUPDATE                 (0x1Au) /**< Dem_DisableDTCRecordUpdate */
#define DEM_SID_ENABLEDTCRECORDUPDATE                  (0x1Bu) /**< Dem_EnableDTCRecordUpdate */
#define DEM_SID_GETNEXTFREEZEFRAMEDATA                 (0x1Du) /**< Dem_GetNextFreezeFrameData */
#define DEM_SID_GETSIZEOFFREEZEFRAMESELECTION          (0x1Fu) /**< Dem_GetSizeOfFreezeFrameSelection */
#define DEM_SID_GETNEXTEXTENDEDDATARECORD              (0x20u) /**< Dem_GetNextExtendedDataRecord */
#define DEM_SID_GETSIZEOFEXTENDEDDATARECORDSELECTION   (0x21u) /**< Dem_GetSizeOfExtendedDataRecordSelection */
#define DEM_SID_CLEARDTC                               (0x23u) /**< Dem_ClearDTC */
#define DEM_SID_DISABLEDTCSETTING                      (0x24u) /**< Dem_DisableDTCSetting */
#define DEM_SID_ENABLEDTCSETTING                       (0x25u) /**< Dem_EnableDTCSetting */
#define DEM_SID_GETINDICATORSTATUS                     (0x29u) /**< Dem_GetIndicatorStatus */
#define DEM_SID_GETCOMPONENTFAILED                     (0x2Au) /**< Dem_GetComponentFailed */
#define DEM_SID_SETCOMPONENTAVAILABLE                  (0x2Bu) /**< Dem_SetComponentAvailable */
#define DEM_SID_GETEVENTMEMORYOVERFLOW                 (0x32u) /**< Dem_GetEventMemoryOverflow */
#define DEM_SID_SETDTCSUPPRESSION                      (0x33u) /**< Dem_SetDTCSuppression */
#define DEM_SID_GETFUNCTIONALUNITOFDTC                 (0x34u) /**< Dem_GetFunctionalUnitOfDTC */
#define DEM_SID_GETNUMBEROFEVENTMEMORYENTRIES          (0x35u) /**< Dem_GetNumberOfEventMemoryEntries */
#define DEM_SID_SETEVENTAVAILABLE                      (0x37u) /**< Dem_SetEventAvailable */
#define DEM_SID_SETSTORAGECONDITION                    (0x38u) /**< Dem_SetStorageCondition */
#define DEM_SID_SETENABLECONDITION                     (0x39u) /**< Dem_SetEnableCondition */
#define DEM_SID_GETNEXTFILTEREDRECORD                  (0x3Au) /**< Dem_GetNextFilteredRecord */
#define DEM_SID_GETNEXTFILTEREDDTCANDFDC               (0x3Bu) /**< Dem_GetNextFilteredDTCAndFDC */
#define DEM_SID_GETTRANSLATIONTYPE                     (0x3Cu) /**< Dem_GetTranslationType */
#define DEM_SID_GETNEXTFILTEREDDTCANDSEVERITY          (0x3Du) /**< Dem_GetNextFilteredDTCAndSeverity */
#define DEM_SID_GETFAULTDETECTIONCOUNTER               (0x3Eu) /**< Dem_GetFaultDetectionCounter */
#define DEM_SID_SETFREEZEFRAMERECORDFILTER             (0x3Fu) /**< Dem_SetFreezeFrameRecordFilter */
#define DEM_SID_SETEVENTDISABLED                       (0x51u) /**< Dem_SetEventDisabled */
#define DEM_SID_DCMREADDATAOFOBDFREEZEFRAME            (0x52u) /**< Dem_DcmReadDataOfOBDFreezeFrame */
#define DEM_SID_DCMGETDTCOFOBDFREEZEFRAME              (0x53u) /**< Dem_DcmGetDTCOfOBDFreezeFrame */
#define DEM_SID_MAINFUNCTION                           (0x55u) /**< Dem_MainFunction */
#define DEM_SID_SETCYCLEQUALIFIED                      (0x56u) /**< Dem_SetCycleQualified */
#define DEM_SID_SETEVENTCONFIRMATIONTHRESHOLDCOUNTER   (0x57u) /**< Dem_SetEventConfirmationThresholdCounter */
#define DEM_SID_GETNUMBEROFFREEZEFRAMERECORDS          (0x5Au) /**< Dem_GetNumberOfFreezeFrameRecords */
#define DEM_SID_DCMREADDATAOFPID01                     (0x61u) /**< Dem_DcmReadDataOfPID01 */
#define DEM_SID_DCMREADDATAOFPID1C                     (0x63u) /**< Dem_DcmReadDataOfPID1C */
#define DEM_SID_DCMREADDATAOFPID21                     (0x64u) /**< Dem_DcmReadDataOfPID21 */
#define DEM_SID_DCMREADDATAOFPID30                     (0x65u) /**< Dem_DcmReadDataOfPID30 */
#define DEM_SID_DCMREADDATAOFPID31                     (0x66u) /**< Dem_DcmReadDataOfPID31 */
#define DEM_SID_DCMREADDATAOFPID41                     (0x67u) /**< Dem_DcmReadDataOfPID41 */
#define DEM_SID_DCMREADDATAOFPID4D                     (0x68u) /**< Dem_DcmReadDataOfPID4D */
#define DEM_SID_DCMREADDATAOFPID4E                     (0x69u) /**< Dem_DcmReadDataOfPID4E */
#define DEM_SID_DCMREADDATAOFPID91                     (0x6Au) /**< Dem_DcmReadDataOfPID91 */
#define DEM_SID_DCMGETINFOTYPEVALUE08                  (0x6Bu) /**< Dem_DcmGetInfoTypeValue08 */
#define DEM_SID_DCMGETINFOTYPEVALUE0B                  (0x6Cu) /**< Dem_DcmGetInfoTypeValue0B */
#define DEM_SID_GETEVENTEXTENDEDDATARECORDEX           (0x6Du) /**< Dem_GetEventExtendedDataRecordEx */
#define DEM_SID_GETEVENTFREEZEFRAMEDATAEX              (0x6Eu) /**< Dem_GetEventFreezeFrameDataEx */
#define DEM_SID_REPIUMPRDENRELEASE                     (0x72u) /**< Dem_RepIUMPRDenRelease */
#define DEM_SID_REPIUMPRFAULTDETECT                    (0x73u) /**< Dem_RepIUMPRFaultDetect */
#define DEM_SID_SETPTOSTATUS                           (0x79u) /**< Dem_SetPtoStatus */
#define DEM_SID_SETWIRSTATUS                           (0x7Au) /**< Dem_SetWIRStatus */
#define DEM_SID_J1939DCMSETDTCFILTER                   (0x90u) /**< Dem_J1939DcmSetDTCFilter */
#define DEM_SID_J1939DCMGETNUMBEROFFILTEREDDTC         (0x91u) /**< Dem_J1939DcmGetNumberOfFilteredDTC */
#define DEM_SID_J1939DCMGETNEXTOFFILTEREDDTC           (0x92u) /**< Dem_J1939DcmGetNextFilteredDTC */
#define DEM_SID_J1939DCMFIRSTDTCWITHLAMPSTATUS         (0x93u) /**< Dem_J1939DcmFirstDTCwithLampStatus */
#define DEM_SID_J1939DCMGETNEXTDTCWITHLAMPSTATUS       (0x94u) /**< Dem_J1939DcmGetNextDTCwithLampStatus */
#define DEM_SID_J1939DCMCLEARDTC                       (0x95u) /**< Dem_J1939DcmClearDTC */
#define DEM_SID_J1939DCMSETFREEZEFRAMEFITER            (0x96u) /**< Dem_J1939DcmSetFreezeFrameFilter */
#define DEM_SID_J1939DCMGETNEXTFREEZEFRAME             (0x97u) /**< Dem_J1939DcmGetNextFreezeFrame */
#define DEM_SID_J1939DCMGETNEXTSPNINFREEZEFRAME        (0x98u) /**< Dem_J1939DcmGetNextSPNInFreezeFrame */
#define DEM_SID_J1939DCMSETRATIOFILTER                 (0x99u) /**< Dem_J1939DcmSetRatioFilter */
#define DEM_SID_J1939DCMGETNEXTFILTEREDRATIO           (0x9Au) /**< Dem_J1939DcmGetNextFilteredRatio */
#define DEM_SID_J1939DCMREADDIAGNOSTICREADINESS1       (0x9Bu) /**< Dem_J1939DcmReadDiagnosticReadiness1 */
#define DEM_SID_J1939DCMREADDIAGNOSTICREADINESS2       (0x9Cu) /**< Dem_J1939DcmReadDiagnosticReadiness2 */
#define DEM_SID_J1939DCMREADDIAGNOSTICREADINESS3       (0x9Du) /**< Dem_J1939DcmReadDiagnosticReadiness3 */
#define DEM_SID_GETDEBOUNCINGEVENT                     (0x9Fu) /**< Dem_GetDebouncingOfEvent */
#define DEM_SID_SETDTR                                 (0xA2u) /**< Dem_SetDTR */
#define DEM_SID_DCMGETAVAILABLEOBDMIDS                 (0xA3u) /**< Dem_DcmGetAvailableOBDMIDs */
#define DEM_SID_DCMGETNUMTIDSOFOBDMID                  (0xA4u) /**< Dem_DcmGetNumTIDsOfOBDMID */
#define DEM_SID_DCMGETDTRDATA                          (0xA5u) /**< Dem_DcmGetDTRData */
#define DEM_SID_SETDATAOFPID21                         (0xA6u) /**< Dem_SetDataOfPID21 */
#define DEM_SID_SETDATAOFPID31                         (0xA7u) /**< Dem_SetDataOfPID31 */
#define DEM_SID_SETDATAOFPID4D                         (0xA8u) /**< Dem_SetDataOfPID4D */
#define DEM_SID_SETDATAOFPID4E                         (0xA9u) /**< Dem_SetDataOfPID4E */
#define DEM_SID_GETCYCLEQUALIFIED                      (0xABu) /**< Dem_GetCycleQualified */
#define DEM_SID_SETIUMPRDENCONDITION                   (0xAEu) /**< Dem_SetIUMPRDenCondition */
#define DEM_SID_GETIUMPRDENCONDITION                   (0xAFu) /**< Dem_GetIUMPRDenCondition */
#define DEM_SID_GETDATAOFPID21                         (0xB1u) /**< Dem_GetDataOfPID21 */
#define DEM_SID_GETDTCSEVERITYAVAILABILITYMASK         (0xB2u) /**< Dem_GetDTCSeverityAvailabilityMask */
#define DEM_SID_READDATAOFPID01                        (0xB3u) /**< Dem_ReadDataOfPID01 */
#define DEM_SID_GETB1COUNTER                           (0xB4u) /**< Dem_GetB1Counter */
#define DEM_SID_GETMONOTORSTATUS                       (0xB5u) /**< Dem_GetMonitorStatus */
#define DEM_SID_GETEVENTUDSSTATUS                      (0xB6u) /**< Dem_GetEventUdsStatus */
#define DEM_SID_SELECTDTC                              (0xB7u) /**< Dem_SelectDTC */
#define DEM_SID_GETDTCSELECTIONRESULT                  (0xB8u) /**< Dem_GetDTCSelectionResult */
#define DEM_SID_SELECTFREEZEFRAMEDATA                  (0xB9u) /**< Dem_SelectFreezeFrameData */
#define DEM_SID_SELSECTEXTENDEDDATARECORD              (0xBAu) /**< Dem_SelectExtendedDataRecord */
#define DEM_SID_GETDTCSELECTIONRESULTFORCLEARDTC       (0xBBu) /**< Dem_GetDTCSelectionResultForClearDTC */
#define DEM_SID_GETDTCSUPPRESSION                      (0xBCu) /**< Dem_GetDTCSuppression */
#define DEM_SID_SETEVENTSTATUSWITHMONITORDATA          (0xBDu) /**< Dem_SetEventStatusWithMonitorData */
#define DEM_SID_GETEVENTAVAILABLE                      (0xBEu) /**< Dem_GetEventAvailable */
#define DEM_SID_SETDTCFILTERBYEXTENDEDDATARECORDNUMBER (0xC1u) /**< Dem_SetDTCFilterByExtendedDataRecordNumber */
#define DEM_SID_SETDTCFILTERBYREADINESSGROUP           (0xC2u) /**< Dem_SetDTCFilterByReadinessGroup */
#define DEM_SID_DCMGETINFOTYPEVALUE79                  (0xC3u) /**< Dem_DcmGetInfoTypeValue79 */
#define DEM_SID_DCMREADDATAOFPIDF501                   (0xC4u) /**< Dem_DcmReadDataOfPIDF501 */
#define DEM_SID_RESETMONITORSTATUS                     (0xC5u) /**< Dem_ResetMonitorStatus */
#define DEM_SID_INTERNALAPIID                          (0xffU) /**< Internal Functions */
/** @} */

/**
 * @name DEM_DEVELOP_ERROR_ENCODE
 * @{
 * @brief Development Errors
 */
#define DEM_E_NO_ERROR (0x0u) /**< API function called with no det error */
#define DEM_E_WRONG_CONFIGURATION \
    (0x10u) /**< API function called with a parameter value, which is not allowed by active configuration */
#define DEM_E_PARAM_POINTER (0x11u) /**< API function called with a NULL pointer */
#define DEM_E_PARAM_DATA    (0x12u) /**< API function called with wrong parameter value */
#define DEM_E_PARAM_LENGTH  (0x13u) /**< API function called with wrong length parameter value */
#define DEM_E_INIT_FAILED   (0x14u) /**< Dem initialisation failed (refer to SWS_BSW_00151) */
#define DEM_E_UNINIT                                                                                                  \
    (0x20u) /**< API function called before the Dem module has been full initialized (refer to                        \
               [SWS_Dem_00124],[SWS_Dem_00364]) or after the Dem module has been shut down (refer to [SWS_Dem_00368]) \
             */
#define DEM_E_WRONG_CONDITION                                                                                         \
    (0x40u) /**< Required conditions for the respective API call are not fulfilled (e.g. an invalid status change was \
               initiated, or a filter was not set correctly, etc. - refer to [SWS_Dem_00518]). */
#define DEM_E_INVALID_OBDMID (0x50u) /**< Dem_DcmGetAvailableOBDMIDs called with invalid OBDMID. */
/** @} */

/**<  */
/**
 * @name DEM_RUNTIME_ERROR_ENCODE
 * @{
 * @brief Runtime Errors
 */
#define DEM_E_UDS_STATUS_PROCESSING_FAILED \
    (0x21u) /**< The UDS status corresponding to a changed monitor status could not be processed. */
#define DEM_E_NODATAAVAILABLE (0x30u) /**< No valid data for data element available by SW-C or BSW call */
/** @} */

/* ========================================== internal function definitions ========================================= */
#if (STD_ON == DEM_DEV_ERROR_DETECT)
/**
 * @brief       Dem Report Error to Det
 * @param[in]   ApiId: Specifies which API reported the error.
 * @param[in]   ErrorId: Specify which error was reported.
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_DetReportError(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportError(DEM_MODULE_ID, DEM_INSTANCE_ID, ApiId, ErrorId);
}
#else
#define Dem_DetReportError(ApiId, ErrorId)
#endif

/**
 * @brief       Dem Report Runtime Error to Det
 * @param[in]   ApiId: Specifies which API reported the error.
 * @param[in]   ErrorId: Specify which error was reported.
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_DetReportRuntimeError(uint8 ApiId, uint8 ErrorId)
{
    ((void)Det_ReportRuntimeError(DEM_MODULE_ID, DEM_INSTANCE_ID, ApiId, ErrorId));
}

#if (DEM_FEATURE_MEMCOPY_USED_ILIB == STD_OFF)
/**
 * @brief       Default implementation of memcopy. Can be used instead of external function call. 'dst' and 'src' MUST
 * have the type 'uint8 *'.
 * @param[in]   src: Pointer to source buffer
 * @param[in]   len: The process size
 * @param[out]  dst: Pointer to destination buffer
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_MemCpy(uint8* dst, const uint8* src, uint32 len)
{
    for (uint32 Index = 0x00u; Index < len; ++Index)
    {
        dst[Index] = src[Index];
    }
}

/**
 * @brief       Implementation of memset. Can be used instead of external function call. 'dst' MUST have the type
 * 'uint8 *'.
 * @param[in]   val: The process value
 * @param[in]   len: The process size
 * @param[out]  dst: Pointer to destination buffer
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_MemSet(uint8* dst, uint8 val, uint32 len)
{
    for (uint32 Index = 0x00u; Index < len; ++Index)
    {
        dst[Index] = val;
    }
}
#else
#define Dem_MemCpy(dst, src, len) IStdLib_MemCpy(dst, src, len) /**< Wrapper macro for memcopy */
#define Dem_MemSet(dst, val, len) IStdLib_MemSet(dst, val, len) /**< Wrapper macro for memset */
#endif

#endif
