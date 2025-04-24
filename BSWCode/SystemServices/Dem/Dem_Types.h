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
**  FILENAME    : Dem_Types.h                                                 **
**                                                                            **
**  Created on  :                                                             **
**  Author      : tao.yu                                                      **
**  Vendor      : i-soft                                                      **
**  DESCRIPTION : Type definitions of DEM                                     **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/

#ifndef DEM_TYPES_H_
#define DEM_TYPES_H_

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Dem_Cfg.h"
#include "Rte_Dem_Type.h"

/*******************************************************************************
**                      macros                                                **
*******************************************************************************/
#ifndef DEM_UNUSED
#define DEM_UNUSED(a) (void)(a)
#endif

#define DEM_DTC_GROUP_ALL_DTCS 0xFFFFFFUL

/*Internal identifier of a monitored component. Remark: 0 is not a valid value*/
typedef uint16 Dem_ComponentIdType;

/*OBD specific ratio Id (related to a specific event, a FID, and an IUMPR group). This type depends
 * on the Dem configuration.*/
typedef uint16 Dem_RatioIdType;

typedef uint8 Dem_DTCRequestType;
#define DEM_FIRST_FAILED_DTC           0x01u
#define DEM_MOST_RECENT_FAILED_DTC     0x02u
#define DEM_FIRST_DET_CONFIRMED_DTC    0x03u
#define DEM_MOST_REC_DET_CONFIRMED_DTC 0x04u

typedef uint8 Dem_DTCTranslationFormatType;
#define DEM_DTC_TRANSLATION_ISO15031_6        0x00u
#define DEM_DTC_TRANSLATION_ISO14229_1        0x01u
#define DEM_DTC_TRANSLATION_SAEJ1939_73       0x02u
#define DEM_DTC_TRANSLATION_ISO11992_4        0x03u
#define DEM_DTC_TRANSLATION_J2012DA_FORMAT_04 0x04u

typedef uint8 Dem_DTCSeverityType;
#define DEM_SEVERITY_NO_SEVERITY           0x00u
#define DEM_SEVERITY_WWHOBD_CLASS_NO_CLASS 0x01u
#define DEM_SEVERITY_WWHOBD_CLASS_A        0x02u
#define DEM_SEVERITY_WWHOBD_CLASS_B1       0x04u
#define DEM_SEVERITY_WWHOBD_CLASS_B2       0x08u
#define DEM_SEVERITY_WWHOBD_CLASS_C        0x10u
/*Selects one of the defined DTC classes (or "NO_CLASS")*/
#define DTC_CLASS                       0x1fu
#define DEM_SEVERITY_MAINTENANCE_ONLY   0x20u
#define DEM_SEVERITY_CHECK_AT_NEXT_HALT 0x40u
#define DEM_SEVERITY_CHECK_IMMEDIATELY  0x80u

/* DemIUMPRDenGroup */
typedef uint8 Dem_IUMPRDenGroupType;
#define DEM_IUMPR_DEN_500MILL   0u
#define DEM_IUMPR_DEN_COLDSTART 1u
#define DEM_IUMPR_DEN_EVAP      2u
#define DEM_IUMPR_DEN_NONE      3u
#define DEM_IUMPR_DEN_PHYS_API  4u

/* DemIUMPRGroup */
typedef uint8 Dem_IUMPRGroupType;
#define DEM_IUMPR_BOOSTPRS  1u
#define DEM_IUMPR_CAT1      2u
#define DEM_IUMPR_CAT2      3u
#define DEM_IUMPR_EGR       4u
#define DEM_IUMPR_EGSENSOR  5u
#define DEM_IUMPR_EVAP      6u
#define DEM_IUMPR_FLSYS     7u
#define DEM_IUMPR_NMHCCAT   8u
#define DEM_IUMPR_NOXADSORB 9u
#define DEM_IUMPR_NOXCAT    10u
#define DEM_IUMPR_OXS1      11u
#define DEM_IUMPR_OXS2      12u
#define DEM_IUMPR_PMFILTER  13u
#define DEM_IUMPR_PRIVATE   14u
#define DEM_IUMPR_SAIR      15u
#define DEM_IUMPR_SECOXS1   16u
#define DEM_IUMPR_SECOXS2   17u

/* DemRatioKind */
typedef uint8 Dem_RatioKindType;
#define DEM_RATIO_API      0u
#define DEM_RATIO_OBSERVER 1u

typedef uint8 Dem_J1939DcmDTCStatusFilterType;
#define DEM_J1939DTC_ACTIVE            0x00u /*active DTCs*/
#define DEM_J1939DTC_PREVIOUSLY_ACTIVE 0x01u /*previously active DTCs*/
#define DEM_J1939DTC_PENDING           0x02u /*pending DTCs*/
#define DEM_J1939DTC_PERMANENT         0x03u /*permanent DTCs*/
#define DEM_J1939DTC_CURRENTLY_ACTIVE  0x04u /*currently active DTC*/

typedef uint8 Dem_J1939DcmSetClearFilterType;
#define DEM_J1939DTC_CLEAR_ALL               0x00u /*active DTCs*/
#define DEM_J1939DTC_CLEAR_PREVIOUSLY_ACTIVE 0x01u /*previously active DTCs*/

typedef uint8 Dem_J1939DcmSetFreezeFrameFilterType;
#define DEM_J1939DCM_FREEZEFRAME                  0x00u
#define DEM_J1939DCM_EXPANDED_FREEZEFRAME         0x01u
#define DEM_J1939DCM_SPNS_IN_EXPANDED_FREEZEFRAME 0x02u

typedef uint16 Dem_J1939DcmLampStatusType;
/* bits 8-7    Malfunction Indicator Lamp Status
 * bits 6-5    Red Stop Lamp Status
 * bits 4-3    Amber Warning Lamp Status
 * bits 2-1    Protect Lamp Status
 * bits 8-7    Flash Malfunction Indicator Lamp
 * bits 6-5    Flash Red Stop Lamp
 * bits 4-3    Flash Amber Warning Lamp
 * bits 2-1    Flash Protect Lamp
 */

/*This structure represents all data elemets of the DM05 message*/
typedef struct
{
    uint8 ActiveTroubleCodes;                     /*Number of active DTCs*/
    uint8 PreviouslyActiveDiagnosticTroubleCodes; /*Number of previously active DTCs*/
    uint8 OBDCompliance;                          /*OBD Compliance*/
    uint8
        ContinuouslyMonitoredSystemsSupport_Status; /*Identifies the continuously monitored system support and status*/
    uint16 NonContinuouslyMonitoredSystemsSupport;  /*Identifies the non-continuously monitored systems support*/
    /*Identifies the non-continuously monitored systems status*/
    uint16 NonContinuouslyMonitoredSystemsStatus; /* PRQA S 0779 */ /* MISRA Rule 5.2 */
} Dem_J1939DcmDiagnosticReadiness1Type; /* PRQA S 0779 */           /* MISRA Rule 5.2 */

/*This structure represents all data elemets of the DM21 message*/
typedef struct
{
    uint16 DistanceTraveledWhileMILisActivated;    /*The kilometers accumulated while the MIL is activated*/
    uint16 DistanceSinceDTCsCleared;               /*Distance accumulated since emission related DTCs were cleared*/
    uint16 MinutesRunbyEngineWhileMILisActivated;  /*Accumulated count (in minutes) while the MIL is activated (on)*/
    uint16 TimeSinceDiagnosticTroubleCodesCleared; /*Engine running time accumulated since emission related DTCs were
                                                      cleared*/
} Dem_J1939DcmDiagnosticReadiness2Type; /* PRQA S 0779 */ /* MISRA Rule 5.2 */

/*This structure represents all data elemets of the DM26 message*/
typedef struct
{
    uint16 TimeSinceEngineStart;                             /*Time since key-on that the engine has been running*/
    uint8 NumberofWarmupsSinceDTCsCleared;                   /*Number of OBD warmup cycles since all DTCs were cleared*/
    uint8 ContinuouslyMonitoredSystemsEnableCompletedStatus; /*Identifies the continuously monitored system
                                                                enable/completed support and status*/
    uint16
        NonContinuouslyMonitoredSystemsEnableStatus; /*Enable status of noncontinuous monitors this monitoring cycle*/
    /*Completion status of noncontinuous monitors this monitoring cycle*/
    uint16 NonContinuouslyMonitoredSystems; /* PRQA S 0779 */ /* MISRA Rule 5.2 */
} Dem_J1939DcmDiagnosticReadiness3Type; /* PRQA S 0779 */     /* MISRA Rule 5.2 */

/*******************************************************************************
**                      Dem return types                                      **
*******************************************************************************/

typedef uint8 Dem_ReturnGetStatusOfDTCType; /*Used to return the status of Dem_DcmGetStatusOfDTC.*/
#define DEM_STATUS_OK              0x00u    /*Status of DTC is OK*/
#define DEM_STATUS_WRONG_DTC       0x01u    /*DTC value not existing (in this format)*/
#define DEM_STATUS_WRONG_DTCORIGIN 0x02u    /*Wrong DTC origin*/
#define DEM_STATUS_FAILED          0x03u    /*DTC failed*/
#define DEM_STATUS_PENDING \
    0x04u /*The requested value is calculated asynchronously and currently not available. The caller can retry later*/

typedef uint8 Dem_ReturnGetSeverityOfDTCType;  /*Used to return the status of Dem_DcmGetSeverityOfDTC*/
#define DEM_GET_SEVERITYOFDTC_OK         0x00u /*Severity successfully returned*/
#define DEM_GET_SEVERITYOFDTC_WRONG_DTC  0x01u /*DTC value not existing (in UDS format)*/
#define DEM_GET_SEVERITYOFDTC_NOSEVERITY 0x02u /*Severity information is not available*/
#define DEM_GET_SEVERITYOFDTC_PENDING                                                                          \
    0x03u /*The requested value is calculated asynchronously and currently not available. The caller can retry \
             later.*/

typedef uint8 Dem_ReturnGetFunctionalUnitOfDTCType; /*Used to return the status of Dem_DcmGetFunctionalUnitOfDTC*/
#define DEM_GET_FUNCTIONALUNITOFDTC_OK        0x00u /*Functional unit successfully returned*/
#define DEM_GET_FUNCTIONALUNITOFDTC_WRONG_DTC 0x01u /*DTC value not existing (in UDS format)*/

typedef uint8 Dem_ReturnSetFilterType; /*Used to return the status of (re-)setting a specific filter*/
#define DEM_FILTER_ACCEPTED 0x00u      /*Filter was accepted*/
#define DEM_WRONG_FILTER    0x01u      /*Wrong filter selected*/

typedef uint8 Dem_ReturnGetNumberOfFilteredDTCType; /*Used to return the status of Dem_DcmGetNumberOfFilteredDTC*/
#define DEM_NUMBER_OK     0x00u                     /*Getting number of filtered DTCs was successful*/
#define DEM_NUMBER_FAILED 0x01u                     /*Getting number of filtered DTCs failed.*/
#define DEM_NUMBER_PENDING \
    0x02u /*The requested value is calculated asynchronously and currently not available. The caller can retry later*/

typedef uint8 Dem_ReturnGetNextFilteredElementType; /*Used to return the status of Dem_DcmGetNextFilteredXxx and
                                                       Dem_J1939DcmGetNextFilteredXxx.*/
#define DEM_FILTERED_OK                  0x00u      /*Returned next filtered element*/
#define DEM_FILTERED_NO_MATCHING_ELEMENT 0x01u      /*No further element (matching the filter criteria) found*/
#define DEM_FILTERED_PENDING                                                                                          \
    0x02u /*The requested value is calculated asynchronously and currently not available. The caller can retry later. \
             Only used by asynchronous interfaces.*/
#define DEM_FILTERED_BUFFER_TOO_SMALL 0x03u /*Buffer in the BufSize parameter is not huge enough*/

typedef uint8 Dem_ReturnGetDTCByOccurrenceTimeType; /*Used to return the status of Dem_DtcGetDTCByOccurrenceTime*/
#define DEM_OCCURR_OK            0x00u              /*matching DTC available*/
#define DEM_OCCURR_NOT_AVAILABLE 0x01u              /*no DTC is matching the requested occurrence time*/

typedef uint8 Dem_ReturnDisableDTCRecordUpdateType; /*Used to return the status of Dem_DcmDisableDTCRecordUpdate*/
#define DEM_DISABLE_DTCRECUP_OK              0x00u  /*Event memory update of DTC successfully disabled*/
#define DEM_DISABLE_DTCRECUP_WRONG_DTC       0x01u  /*DTC value not existing (in UDS format)*/
#define DEM_DISABLE_DTCRECUP_WRONG_DTCORIGIN 0x02u  /*Wrong DTC origin*/
#define DEM_DISABLE_DTCRECUP_PENDING         0x03u  /*Disabling is currently not possible. The caller can retry later*/

typedef uint8 Dem_ReturnGetFreezeFrameDataByDTCType; /*Used to return the status of Dem_DcmGetFreezeFrameDataByDTC*/
#define DEM_GET_FFDATABYDTC_OK              0x00u    /*Size successfully returned*/
#define DEM_GET_FFDATABYDTC_WRONG_DTC       0x01u    /*DTC value not existing (in UDS format)*/
#define DEM_GET_FFDATABYDTC_WRONG_DTCORIGIN 0x02u    /*Wrong DTC origin*/
#define DEM_GET_FFDATABYDTC_WRONG_RECORDNUMBER \
    0x03u /*Record number is not supported by configuration and therefore invalid*/
#define DEM_GET_FFDATABYDTC_WRONG_BUFFERSIZE 0x04u /*provided buffer size to small*/
#define DEM_GET_FFDATABYDTC_PENDING                                                                            \
    0x05u /*The requested value is calculated asynchronously and currently not available. The caller can retry \
             later.*/
#define DEM_GET_FFDATABYDTC_FAILED 0x06u /*failed to get ,because there is no event failed */

typedef uint8
    Dem_ReturnGetExtendedDataRecordByDTCType; /*Used to return the status of Dem_DcmGetExtendedDataRecordByDTC*/
#define DEM_RECORD_OK               0x00u     /*Extended data record successfully returned*/
#define DEM_RECORD_WRONG_DTC        0x01u     /*DTC value not existing (in UDS format)*/
#define DEM_RECORD_WRONG_DTCORIGIN  0x02u     /*Origin wrong*/
#define DEM_RECORD_WRONG_NUMBER     0x03u     /*Record number is not supported by configuration and therefore invalid*/
#define DEM_RECORD_WRONG_BUFFERSIZE 0x04u     /*Provided buffer to small*/
#define DEM_RECORD_PENDING                                                                                     \
    0x05u /*The requested value is calculated asynchronously and currently not available. The caller can retry \
             later.*/
#define DEM_RECORD_FAILED 0x06u /*failed to get ,because there is no event failed */

typedef uint8 Dem_ReturnGetSizeOfDataByDTCType; /*Used to return the status of
                                                   Dem_DcmGetSizeOfFreezeFrameByDTC and
                                                   Dem_DcmGetSizeOfExtendedDataRecordByDTC.*/
#define DEM_GETSIZEBYDTC_OK              0x00u  /*Size successfully returned*/
#define DEM_GETSIZEBYDTC_WRONG_DTC       0x01u  /*DTC value not existing (in UDS format)*/
#define DEM_GETSIZEBYDTC_WRONG_DTCORIGIN 0x02u  /*Wrong DTC origin*/
#define DEM_GETSIZEBYDTC_WRONG_RECNUM    0x03u /*Record number is not supported by configuration and therefore invalid*/
#define DEM_GETSIZEBYDTC_PENDING                                                              \
    0x04u /*The requested value is calculated asynchronously and currently not available. The \
             caller can retry later*/

typedef uint8 Dem_ReturnClearDTCType; /*Used to return the status of Dem_<...>ClearDTC.*/
#define DEM_CLEAR_OK 0x00u            /*DTC successfully cleared*/
#ifndef DEM_CLEAR_WRONG_DTC
#define DEM_CLEAR_WRONG_DTC 0x01u /*DTC value not existing (in this format)*/
#endif                            /* DEM_CLEAR_WRONG_DTC */
#ifndef DEM_CLEAR_WRONG_DTCORIGIN
#define DEM_CLEAR_WRONG_DTCORIGIN 0x02u /*Wrong DTC origin*/
#endif                                  /* DEM_CLEAR_WRONG_DTCORIGIN */
#ifndef DEM_CLEAR_FAILED
#define DEM_CLEAR_FAILED 0x03u /*DTC clearing failed*/
#endif                         /* DEM_CLEAR_FAILED */
#ifndef DEM_CLEAR_PENDING
#define DEM_CLEAR_PENDING \
    0x04u /*The DTC clearing is performed asynchronously and still pending. The caller can retry later*/
#endif    /* DEM_CLEAR_PENDING */
#ifndef DEM_CLEAR_BUSY
#define DEM_CLEAR_BUSY                                                                        \
    0x05u /*DTC not cleared, as another clearing process is in progress. The caller can retry \
             later*/
#endif    /* DEM_CLEAR_BUSY */
#ifndef DEM_CLEAR_MEMORY_ERROR
#define DEM_CLEAR_MEMORY_ERROR                                                                        \
    0x06u /*An error occurred during erasing a memory location (e.g. if DemClearDTCBehavior in Dem is \
           set to DEM_CLRRESP_NONVOLATILE_FINISH and erasing of non-volatile-block failed)*/
#endif    /* DEM_CLEAR_MEMORY_ERROR */

typedef uint8 Dem_ReturnControlDTCSettingType; /*Used to return the status of Dem_DcmDisableDTCSetting and
                                                  Dem_DcmEnableDTCSetting*/
#define DEM_CONTROL_DTC_SETTING_OK 0x00u       /*DTC setting control successful*/
#ifndef DEM_CONTROL_DTC_SETTING_N_OK
#define DEM_CONTROL_DTC_SETTING_N_OK 0x01u /*DTC setting control not successful*/
#endif                                     /* DEM_CONTROL_DTC_SETTING_N_OK */
#ifndef DEM_CONTROL_DTC_WRONG_DTCGROUP
#define DEM_CONTROL_DTC_WRONG_DTCGROUP 0x02u /*DTC setting control not successful because group of DTC was wrong*/
#endif                                       /* DEM_CONTROL_DTC_WRONG_DTCGROUP */

#ifndef DEM_E_NO_DTC_AVAILABLE
#define DEM_E_NO_DTC_AVAILABLE 0x02u
#endif /* DEM_E_NO_DTC_AVAILABLE */
#ifndef DEM_E_NO_FDC_AVAILABLE
#define DEM_E_NO_FDC_AVAILABLE 0x03u
#endif /* DEM_E_NO_FDC_AVAILABLE */
#define DEM_E_WRONG_CONFIGURATION 0x10u
#define DEM_E_PARAM_POINTER       0x11u
#define DEM_E_PARAM_DATA          0x12u
#define DEM_E_PARAM_LENGTH        0x13u
#define DEM_E_INIT_FAILED         0x14u
#ifndef DEM_E_WRONG_BUFFERSIZE
#define DEM_E_WRONG_BUFFERSIZE 0x15u
#endif /* DEM_E_WRONG_BUFFERSIZE */
#define DEM_E_UNINIT 0x20u
#ifndef DEM_E_NODATAAVAILABLE
#define DEM_E_NODATAAVAILABLE 0x30u
#endif /* DEM_E_NODATAAVAILABLE */
#ifndef DEM_E_WRONG_RECORDNUMBER
#define DEM_E_WRONG_RECORDNUMBER 0x31u
#endif /* DEM_E_WRONG_RECORDNUMBER */
#ifndef DEM_E_WRONG_DIDNUMBER
#define DEM_E_WRONG_DIDNUMBER 0x32u
#endif /* DEM_E_WRONG_DIDNUMBER */
#define DEM_E_WRONG_CONDITION 0x40u

/* Service ID */
#define DEM_SID_GETVERSIONINFO                         0x00u
#define DEM_SID_PREINIT                                0x01u
#define DEM_SID_INIT                                   0x02u
#define DEM_SID_SHUTDOWN                               0x03u
#define DEM_SID_REPORTERRORSTATUS                      0x0fu
#define DEM_SID_SETEVENTAVAILABLE                      0x37u
#define DEM_SID_SETEVENTSTATUS                         0x04u
#define DEM_SID_RESETEVENTDEBOUNCESTATUS               0x09u
#define DEM_SID_RESETEVENTSTATUS                       0x05u
#define DEM_SID_PRESTOREFREEZEFRAME                    0x06u
#define DEM_SID_CLEARPRESTOREDFREEZEFRAME              0x07u
#define DEM_SID_SETOPERATIONCYCLESTATE                 0x08u
#define DEM_SID_GETOPERATIONCYCLESTATE                 0x9eu
#define DEM_SID_SETAGINGCYCLESTATE                     0x11u
#define DEM_SID_SETWIRSTATUS                           0x7au
#define DEM_SID_GETCOMPONENTFAILED                     0x2au
#define DEM_SID_GETEVENTSTATUS                         0x0au
#define DEM_SID_GETEVENTFAILED                         0x0bu
#define DEM_SID_GETEVENTTESTED                         0x0cu
#define DEM_SID_GETDEBOUNCINGOFEVENT                   0x9fu
#define DEM_SID_GETDTCOFEVENT                          0x0du
#define DEM_SID_SETENABLECONDITION                     0x39u
#define DEM_SID_SETSTORAGECONDITION                    0x38u
#define DEM_SID_GETFAULTDETECTIONCOUNTER               0x3eu
#define DEM_SID_GETINDICATORSTATUS                     0x29u
#define DEM_SID_SETINDICATORSTATUS                     0xa1u
#define DEM_SID_GETEVENTFREEZEFRAMEDATAEX              0x6eu
#define DEM_SID_GETEVENTEXTENDEDDATARECORDEX           0x6du
#define DEM_SID_GETEVENTMEMORYOVERFLOW                 0x32u
#define DEM_SID_GETNUMBEROFEVENTMEMORYENTRIES          0x35u
#define DEM_SID_SETCOMPONENTAVAILABLE                  0x2bu
#define DEM_SID_SETDTCSUPPRESSION                      0x33u
#define DEM_SID_CLEARDTC                               0x23u

#define DEM_SID_MAINFUNCTION                           0x55u

#define DEM_SID_DCMGETTRANSLATIONTYPE                  0x3cu
#define DEM_SID_DCMGETDTCSTATUSAVAILABILITYMASK        0x16u
#define DEM_SID_DCMGETSTATUSOFDTC                      0x15u
#define DEM_SID_DCMGETSEVERITYOFDTC                    0x0eu
#define DEM_SID_DCMGETFUNCTIONALUNITOFDTC              0x34u
#define DEM_SID_DCMSETDTCFILTER                        0x13u
#define DEM_SID_DCMGETNUMBEROFFILTEREDDTC              0x17u
#define DEM_SID_DCMGETNEXTFILTEREDDTC                  0x18u
#define DEM_SID_DCMGETNEXTFILTEREDDTCANDFDC            0x3bu
#define DEM_SID_DCMGETNEXTFILTEREDDTCANDSEVERITY       0x3du /* PRQA S 0791 */ /* MISRA Rule 5.4 */
#define DEM_SID_DCMSETFREEZEFRAMERECORDFILTER          0x3fu
#define DEM_SID_DCMGETNEXTFILTEREDRECORD               0x3au
#define DEM_SID_DCMGETDTCBYOCCURRENCETIME              0x19u
#define DEM_SID_DCMCONTROLDTCSTATUSCHANGEDNOTIFICATION 0xb0u
#define DEM_SID_DCMDISABLEDTCRECORDUPDATE              0x1au
#define DEM_SID_DCMENABLEDTCRECORDUPDATE               0x1bu
#define DEM_SID_DCMGETFREEZEFRAMEDATABYDTC             0x1du
#define DEM_SID_DCMGETSIZEOFFREEZEFRAMEBYDTC           0x1fu
#define DEM_SID_DCMGETEXTENDEDDATARECORDBYDTC          0x20u
#define DEM_SID_DCMGETSIZEOFEXTENDEDDATARECORDBYDTC    0x21u
#define DEM_SID_DCMCHECKCLEARPARAMETER                 0x7bu
#define DEM_SID_DCMCLEARDTC                            0x22u
#define DEM_SID_DCMDISABLEDTCSETTING                   0x24u
#define DEM_SID_DCMENABLEDTCSETTING                    0x25u
#define DEM_SID_DCMGETINFOTYPEVALUE08                  0x6bu
#define DEM_SID_DCMGETINFOTYPEVALUE0B                  0x6cu
#define DEM_SID_DCMREADDATDOFPID01                     0x61u
#define DEM_SID_DCMREADDATDOFPID1C                     0x63u
#define DEM_SID_DCMREADDATDOFPID21                     0x64u
#define DEM_SID_DCMREADDATDOFPID30                     0x65u
#define DEM_SID_DCMREADDATDOFPID31                     0x66u
#define DEM_SID_DCMREADDATDOFPID41                     0x67u
#define DEM_SID_DCMREADDATDOFPID4D                     0x68u
#define DEM_SID_DCMREADDATDOFPID4E                     0x69u
#define DEM_SID_DCMREADDATDOFPID91                     0x6Au
#define DEM_SID_DCMREADDATAOFOBDFREEZEFRAME            0x52u
#define DEM_SID_DCMGETDTCOFOBDFREEZEFRAME              0x53u
#define DEM_SID_DCMGETAVAILABLEOBDMIDS                 0xa3u
#define DEM_SID_DCMGETNUMTIDSOFOBDMID                  0xa4u
#define DEM_SID_DCMGETDTRDATA                          0xa5u

#define DEM_SID_J1939DCMSETDTCFILTER                   0x90u
#define DEM_SID_J1939DCMGETNUMBEROFFILTEREDDTC         0x91u
#define DEM_SID_J1939DCMGETNEXTFILTEREDDTC             0x92u
#define DEM_SID_J1939DCMFIRSTDTCWITHLAMPSTATUS         0x93u
#define DEM_SID_J1939DCMGETNEXTDTCWITHLAMPSTATUS       0x94u
#define DEM_SID_J1939DCMCLEARDTC                       0x95u
#define DEM_SID_J1939DCMSETFREEZEFRAMEFILTER           0x96u
#define DEM_SID_J1939DCMGETNEXTFREEZEFRAME             0x97u
#define DEM_SID_J1939DCMGETNEXTSPNINFREEZEFRAME        0x98u
#define DEM_SID_J1939DCMSETRATIOFILTER                 0x99u
#define DEM_SID_J1939DCMGETNEXTFILTEREDRATIO           0x9au /* PRQA S 0791 */ /* MISRA Rule 5.4 */
#define DEM_SID_J1939DCMREADDIAGNOSTICREADINESS1       0x9bu
#define DEM_SID_J1939DCMREADDIAGNOSTICREADINESS2       0x9cu /* PRQA S 0791 */ /* MISRA Rule 5.4 */
#define DEM_SID_J1939DCMREADDIAGNOSTICREADINESS3       0x9du /* PRQA S 0791 */ /* MISRA Rule 5.4 */

#define DEM_SID_DLTGETMOSTRECENTFREEZEFRAMERECORDDATA  0x41u
#define DEM_SID_DLTGETALLEXTENDEDDATARECORDS           0x40u

#define DEM_SID_SETEVENTDISABLED                       0x51u
#define DEM_SID_REPIUMPRFAULTDETECT                    0x73u
#define DEM_SID_SETIUMPRDENCONDITION                   0xaeu
#define DEM_SID_GETIUMPRDENCONDITION                   0xafu
#define DEM_SID_REPIUMPRDENLOCK                        0x71u
#define DEM_SID_REPIUMPRDENRELEASE                     0x72u
#define DEM_SID_SETPTOSTATUS                           0x79u
#define DEM_SID_READDATAOFPID01                        0xb3u
#define DEM_SID_GETDATAOFPID21                         0xb1u
#define DEM_SID_SETDATAOFPID21                         0xa6u
#define DEM_SID_SETDATAOFPID31                         0xa7u
#define DEM_SID_SETDATAOFPID4D                         0xa8u
#define DEM_SID_SETDATAOFPID4E                         0xa9u
#define DEM_SID_SETPFCCYCLEQUALIFIED                   0xaau
#define DEM_SID_GETPFCCYCLEQUALIFIED                   0xabu
#define DEM_SID_SETCLEARDTC                            0xacu
#define DEM_SID_DCMGETDTCSEVERITYAVAILABILITYMASK      0xb2u
#define DEM_SID_GETB1COUNTER                           0xb4u
#define DEM_SID_SETDTR                                 0xa2u

/* DemTypeOfFreezeFrameRecordNumeration  */
#define DEM_FF_RECNUM_CALCULATED 1u
#define DEM_FF_RECNUM_CONFIGURED 2u

/*******************************************************************************
**                      Global Variable declaration                           **
*******************************************************************************/
typedef struct
{
    Dem_EventIdType IntId;
    uint8 Status;
    uint8 EventStatusFlag;
    uint8 UdsStatus;
    uint8 OldUdsStatus;
    /* Counters */
    uint8 OccurrenceCounter; /*SWS_Dem_00011] */
    uint8 FailureCounter;
#if (DEM_OCC6EANLE == STD_ON)
    uint8 OCC6;
#endif /* DEM_OCC6EANLE == STD_ON */
#if (DEM_OCC4EANLE == STD_ON)
    uint8 OCC4;
#endif /* DEM_OCC4EANLE == STD_ON */
    uint8 AgingCounter;
    uint8 HealingCounter;
#if (DEM_AGEDCOUNT_SAIC == STD_ON)
    uint8 AgedCounter;
#endif /* DEM_AGEDCOUNT_SAIC == STD_ON */
    /* Operation Cycle Counter */
#if (DEM_CYCLESSINCEFRISTFAILEDEANLE == STD_ON)
    uint8 CyclesSinceFirstFailed;
#endif /* DEM_CYCLESSINCEFRISTFAILEDEANLE == STD_ON */
#if (DEM_CYCLESSINCELASTFAILEDEANLE == STD_ON)
    uint8 CyclesSinceLastFailed;
#endif /* DEM_CYCLESSINCELASTFAILEDEANLE == STD_ON */
#if (DEM_STATUSINDICATOR30ENABLE == STD_ON)
    uint8 StatusIndicator30;
#endif /* DEM_STATUSINDICATOR30ENABLE == STD_ON */
} Dem_EventRelateInformationType;

typedef struct
{
    boolean IUMPRDenStatus;
    boolean IUMPRNumStatus;
    uint16 Denominator; /*the number of times the vehicle operation conditions have been fulfilled */
    uint16 Numerator;   /*the number of times a fault could have been found*/
    uint16 General_Denominator;
} IUMPRType;

typedef struct
{
    sint32 TestResult;
    sint32 LowerLimit;
    sint32 UpperLimit;
} DTRInfoType;

/* DTC Filter */
typedef struct
{
    Dem_J1939DcmDTCStatusFilterType DTCStatusFilter;
    Dem_DTCKindType DemJ1939DTCFilterInfo;
    uint8 node;
    Dem_DTCOriginType DTCOrigin;
    /* Result */
    boolean IsSet;
    uint16 NumberOfFilteredDTC;
    uint8 GetNum;
    uint8 CurrentIndex;
} Dem_J1939DTCFilterInfoType;

/* Freeze Frame Filter */
typedef struct
{
    Dem_J1939DcmSetFreezeFrameFilterType FreezeFrameKind;
    uint8 node;
    uint8 CurrentIndex;
    boolean IsSet;
    uint16 SPNIndex;
    uint8 SPNCounter;
    boolean IsFind;
    boolean IsOK;
} Dem_J1939DcmFreezeFrameFilterType;

typedef struct
{
    uint8 node;
    uint8 CurrentIndex;
    boolean IsSet;
} Dem_J1939DcmRatioFilterType;

/* Dem InitState Define  */
typedef enum
{
    DEM_STATE_UNINIT = 0u,
    DEM_STATE_PRE_INIT,
    DEM_STATE_INIT,
    DEM_STATE_SHUTDOWN
} Dem_StateType;

typedef enum
{
    DEM_CLEAR_LOCK = 0u,
    DEM_CLEAR_NOT_LOCK
} Dem_ClearLockStatus;

typedef enum
{
    DEM_ClEAR_NONVOLATILE_FINISHED = 0u,
    DEM_ClEAR_NONVOLATILE_FAILED,
    DEM_ClEAR_NONVOLATILE_INIT,
    DEM_CLEAR_NONVOLATILE_PROCESSING,
    DEM_CLEAR_NONVOLATILE_START
} Dem_ClearNonvolatileStatusType;

typedef struct
{
    uint16 DTCIndex;
    uint16 SaveDTCIndex;
    boolean ClearAllGroup;
    uint8 DTCGroupIndex;
    uint8 memDest;
    uint32 DTC;
    Dem_DTCFormatType DTCFormat;
    Dem_DTCFormatType SaveDTCFormat;
    Dem_DTCOriginType DTCOrigin;
    Dem_DTCOriginType SaveDTCOrigin;
    uint16 SID;
    Dem_J1939DcmSetClearFilterType DTCTypeFilter;
    uint8 node;
} Dem_ClearDTCInfoType;

typedef struct
{
    uint8 Occctr;
    uint8 AgingUpCnt;
    uint8 AgingDownCnt;
    uint8 Ovflind;
    uint8 Significance;
    uint8 CurrentFDC;
#if (DEM_MAXFDCSINCELASTCLEAR_EANLE == STD_ON)
    uint8 MaxFDCSinceLastClear;
#endif /* DEM_MAXFDCSINCELASTCLEAR_EANLE == STD_ON */
    uint8 MaxFDCDuringCurrentCycle;
#if (DEM_CYCLESSINCELASTFAILEDEANLE == STD_ON)
    uint8 CyclesSinceLastFailed;
#endif /* DEM_CYCLESSINCELASTFAILEDEANLE == STD_ON */
#if (DEM_CYCLESSINCEFRISTFAILEDEANLE == STD_ON)
    uint8 CyclesSinceFirstFailed;
#endif /* DEM_CYCLESSINCEFRISTFAILEDEANLE == STD_ON */
    uint8 FailedCycles;
#if (DEM_OCC6EANLE == STD_ON)
    uint8 OCC6;
#endif /* DEM_OCC6EANLE == STD_ON */
#if (DEM_OCC4EANLE == STD_ON)
    uint8 OCC4;
#endif /* DEM_OCC4EANLE == STD_ON */
#if (DEM_STATUSINDICATOR30ENABLE == STD_ON)
    uint8 StatusIndicator30;
#endif /* DEM_STATUSINDICATOR30ENABLE == STD_ON */
#if (DEM_FAILURE_PENGDING_SAIC == STD_ON)
    uint8 FaultPendingCounter;
#endif /* DEM_FAILURE_PENGDING_SAIC == STD_ON */
#if (DEM_AGEDCOUNT_SAIC == STD_ON)
    uint8 AgedCounter;
#endif /* DEM_AGEDCOUNT_SAIC == STD_ON */
} Dem_InternalDataType;

typedef struct
{
    uint8 CbUdsStatus;
    boolean SuppressionStatus;
} Dem_DTCGeneralStatusType;

typedef struct
{
    boolean Status;
    uint8 FailurePriority;
    boolean availability;
} DemComponentStatusType;

/* Debounce Counter Base Info Define */
typedef struct
{
    sint16 InternalDebounceCounter;
    Dem_EventStatusType CurStatus;
} Dem_DebounceCounterInfoType;

typedef struct
{
    sint8 FDC;
    uint8 MaxFDCSinceLastClear;
    uint8 MaxFDCDuringCurrentCycle;
} FDCInfoType;

/* FreezeFrameRecordFilter */
typedef struct
{
    /* Filter */
    Dem_DTCFormatType DTCFormat;
    /* Result */
    uint16 NumberOfFilteredRecords;
    uint16 GetNum;
    uint8 EntryIndex;
    uint8 FFIndex;
} Dem_FreezeFrameRecordFilterInfoType;

typedef struct
{
    uint8 node;
    uint8 CurrentIndex;
    boolean IsSet;
} DemJ1939DTCGetNodeType;
/*******************************************************************************
**                      Global Function declaration                           **
*******************************************************************************/

#endif /* DEM_TYPES_H_ */

/*******************************************************************************
**                      end of file                                           **
*******************************************************************************/
