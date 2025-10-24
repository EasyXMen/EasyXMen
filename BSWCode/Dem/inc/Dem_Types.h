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
 **  @file               : Dem_Types.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : Type definitions of Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_TYPES_H
#define DEM_TYPES_H

/* =================================================== inclusions =================================================== */
#include "Rte_Dem_Type.h"
#include "Dem_OptMacros.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ================================================ type definitions ================================================ */

/**
 * @brief Identification of a DemComponent by assigned ComponentId. The ComponentId is automatically assigned by the
 * Dem. Internal identifier of a monitored component. Remark: 0 is not a valid value. Range: 1..65535
 */
typedef uint16 Dem_ComponentIdType;

/**
 * @brief This type is used to filter DTCs for their kind.
 * Range: 1..2
 */
typedef uint8 Dem_DTCKindType;
#define DEM_DTC_KIND_ALL_DTCS          0x01u /**< Select all DTCs */
#define DEM_DTC_KIND_EMISSION_REL_DTCS 0x02u /**< Select OBD-relevant DTCs */

/**
 * @brief This type is used to request a DTC with specific attributes.
 * Range: 1..4
 */
typedef uint8 Dem_DTCRequestType;
#define DEM_FIRST_FAILED_DTC           0x01u /**< First failed DTC requested */
#define DEM_MOST_RECENT_FAILED_DTC     0x02u /**< Most recent failed DTC requested */
#define DEM_FIRST_DET_CONFIRMED_DTC    0x03u /**< First detected confirmed DTC requested */
#define DEM_MOST_REC_DET_CONFIRMED_DTC 0x04u /**< Most recently detected confirmed DTC requested */

/**
 * @brief DTC translation format as defined in ISO14229-1.
 * Range: 1..5
 */
typedef uint8 Dem_DTCTranslationFormatType;
#define DEM_DTC_TRANSLATION_ISO15031_6        0x00u /**< ISO15031-6 DTC format/SAE J2012-DA_DTCFormat_00 DTC format */
#define DEM_DTC_TRANSLATION_ISO14229_1        0x01u /**< ISO14229-1 DTC format */
#define DEM_DTC_TRANSLATION_SAEJ1939_73       0x02u /**< SAEJ1939-73 DTC format */
#define DEM_DTC_TRANSLATION_ISO11992_4        0x03u /**< ISO11992-4 DTC format */
#define DEM_DTC_TRANSLATION_J2012DA_FORMAT_04 0x04u /**< SAE_J2012-DA_DTCFormat_04 DTC format */

/**
 * @brief Type definition of DTCSeverityMask / DTCSeverity byte containing the DTC severity and DTC class information
 * according to ISO 14229-1 Annex D.3. The upper 3 bits (bit 7-5) are used to represent the DTC severity information.
 * The lower 5 bits (bit 4-0) are used to represent the DTC class information.
 */
typedef uint8 Dem_DTCSeverityType;
#define DEM_SEVERITY_NO_SEVERITY           0x00u /**< No severity information available */
#define DEM_SEVERITY_WWHOBD_CLASS_NO_CLASS 0x01u /**< No class information */
#define DEM_SEVERITY_WWHOBD_CLASS_A        0x02u /**< WWH-OBD Class A */
#define DEM_SEVERITY_WWHOBD_CLASS_B1       0x04u /**< WWH-OBD Class B1 */
#define DEM_SEVERITY_WWHOBD_CLASS_B2       0x08u /**< WWH-OBD Class B2 */
#define DEM_SEVERITY_WWHOBD_CLASS_C        0x10u /**< WWH-OBD Class C */
#define DEM_SEVERITY_MAINTENANCE_ONLY      0x20u /**< Maintenance required */
#define DEM_SEVERITY_CHECK_AT_NEXT_HALT    0x40u /**< Check at next halt */
#define DEM_SEVERITY_CHECK_IMMEDIATELY     0x80u /**< Check immediately */

/**
 * @brief The type to distinguish which DTCs should be filtered.
 * Range: 0..4 */
typedef uint8 Dem_J1939DcmDTCStatusFilterType;
#define DEM_J1939DTC_ACTIVE            0x00u /**< Active DTCs */
#define DEM_J1939DTC_PREVIOUSLY_ACTIVE 0x01u /**< Previously active DTCs */
#define DEM_J1939DTC_PENDING           0x02u /**< Pending DTCs */
#define DEM_J1939DTC_PERMANENT         0x03u /**< Permanent DTCs */
#define DEM_J1939DTC_CURRENTLY_ACTIVE  0x04u /**< Currently active DTC */

/**
 * @brief The type to distinguish which DTCs gets cleared.
 * Range: 0..2
 */
typedef uint8 Dem_J1939DcmSetClearFilterType;
#define DEM_J1939DTC_CLEAR_ACTIVE                       0x00u /**< Active DTCs */
#define DEM_J1939DTC_CLEAR_PREVIOUSLY_ACTIVE            0x01u /**< Previously active DTCs */
#define DEM_J1939DTC_CLEAR_ACTIVE_AND_PREVIOUSLY_ACTIVE 0x02u /**< Active and previously active DTCs */

/**
 * @brief The type to distinguish which Freeze Frame should be filtered.
 * Range: 0..2
 */
typedef uint8 Dem_J1939DcmSetFreezeFrameFilterType;
#define DEM_J1939DCM_FREEZEFRAME                  0x00u /**< FreezeFrame DM04 */
#define DEM_J1939DCM_EXPANDED_FREEZEFRAME         0x01u /**< ExpandedFreezeFrame DM25 */
#define DEM_J1939DCM_SPNS_IN_EXPANDED_FREEZEFRAME 0x02u /**< SPNs in ExpandedFreezeFrame DM24 */

/**
 * @brief For details refer SAE J1939-73.
 */
typedef struct Dem_J1939DcmLampStatusTag /* PRQA S 1536  */ /* VL_Dem_1536 */
{
    uint8 LampStatus;      /**< Lamp status @range 0..3 */
    uint8 FlashLampStatus; /**< Flash lamp status @range 0..3 */
} Dem_J1939DcmLampStatusType;

/**
 * @brief This structure represents all data elemets of the DM05 message. The encoding shall be done acording SAE
 * J1939-73
 */
typedef struct Dem_J1939DcmDiagnosticReadiness1Tag /* PRQA S 1536  */ /* VL_Dem_1536 */
{
    uint8 ActiveTroubleCodes;                         /**< Number of active DTCs @range 0..255 */
    uint8 PreviouslyActiveDiagnosticTroubleCodes;     /**< Number of previously active DTCs @range 0..255 */
    uint8 OBDCompliance;                              /**< OBD Compliance @range 0..255 */
    uint8 ContinuouslyMonitoredSystemsSupport_Status; /**< Identifies the continuously monitored system support and
                                                         status @range 0..255 */
    uint8 NonContinuouslyMonitoredSystemsSupport5;    /**< Identifies the non-continuously monitored systems support
                                                       * (byte5) @range 0..255 */
    uint8 NonContinuouslyMonitoredSystemsSupport6;    /**< Identifies the non-continuously monitored systems support
                                                        (byte6) @range 0..255 */
    uint8 NonContinuouslyMonitoredSystemsStatus7; /**< Identifies the non-continuously monitored systems status (byte7)
                                                     @range 0..255 */
    uint8 NonContinuouslyMonitoredSystemsStatus8; /**< Identifies the non-continuously monitored systems status (byte8)
                                                     @range 0..255 */
} Dem_J1939DcmDiagnosticReadiness1Type;

/**
 * @brief This structure represents all data elemets of the DM21 message. The encoding shall be done acording SAE
 * J1939-73
 */
typedef struct Dem_J1939DcmDiagnosticReadiness2Tag /* PRQA S 1536  */ /* VL_Dem_1536 */
{
    uint16 DistanceTraveledWhileMILisActivated; /**< The kilometers accumulated while the MIL is activated
                                                    @range 0..65535 */
    uint16
        DistanceSinceDTCsCleared; /**< Distance accumulated since emission related DTCs were cleared @range 0..65535 */
    uint16 MinutesRunbyEngineWhileMILisActivated;  /**< Accumulated count (in minutes) while the MIL is activated (on)
                                                      @range 0..65535 */
    uint16 TimeSinceDiagnosticTroubleCodesCleared; /**< Engine running time accumulated since emission related DTCs were
                                                      cleared @range 0..65535 */
} Dem_J1939DcmDiagnosticReadiness2Type;

/**
 * @brief This structure represents all data elemets of the DM26 message. The encoding shall be done acording SAE
 * J1939-73
 */
typedef struct Dem_J1939DcmDiagnosticReadiness3Tag /* PRQA S 1536  */ /* VL_Dem_1536 */
{
    uint16 TimeSinceEngineStart; /**< Time since key-on that the engine has been running. @range 0..65535 */
    uint8
        NumberofWarmupsSinceDTCsCleared; /**< Number of OBD warm-up cycles since all DTCs were cleared @range 0..255 */
    uint8 ContinuouslyMonitoredSystemsEnableCompletedStatus; /**< Identifies the continuously monitored system
                                                                Enable/completed support and status. @range 0..255 */
    uint8 NonContinuouslyMonitoredSystemsEnableStatus5; /**< Enable status of non-continuous monitors this monitoring
                                                           cycle (byte5) @range 0..255 */
    uint8 NonContinuouslyMonitoredSystemsEnableStatus6; /**< Enable status of non-continuous monitors this monitoring
                                                           cycle (byte6) @range 0..255 */
    uint8 NonContinuouslyMonitoredSystems7; /**< Completion status of non-continuous monitors this monitoring cycle
                                               (byte7) @range 0..255 */
    uint8 NonContinuouslyMonitoredSystems8; /**< Completion status of non-continuous monitors this monitoring cycle
                                               (byte8) @range 0..255 */
} Dem_J1939DcmDiagnosticReadiness3Type;

/**
 * @brief This Data type corresponds to the readiness group defined by SAE J1979-DA.
 * Range: 0..255
 */
typedef uint8 Dem_EventOBDReadinessGroupType;

/**<  */
/**
 * @name DEM_OPERATION_ERROR_CODE
 * @{
 * @brief ServiceInterfaceErrorCode AUTOSAR R23-11 Possible Error codes for Service Interfaces (APIs)
 */
#define DEM_PENDING            (4u) /**< Operation is asynchronous processed (typically needs to be requested again) */
#define DEM_CLEAR_BUSY         (5u) /**< Operation ClearDTC reports "Busy" */
#define DEM_CLEAR_MEMORY_ERROR (6u) /**< Operation ClearDTC has "Memory Error" */
#define DEM_CLEAR_FAILED       (7u) /**< DTC clearing failed */
#define DEM_WRONG_DTC \
    (8u) /**< Selected DTC value in selected format does not exist or operation is restricted by configuration */
#define DEM_WRONG_DTCORIGIN    (9u)  /**< Selected DTCOrigin does not exist */
#define DEM_E_NO_DTC_AVAILABLE (10u) /**< There is no DTC configured in the requested format */
#define DEM_E_NO_FDC_AVAILABLE (14u) /**< There is no fault detection counter available for the requested event */
#define DEM_BUFFER_TOO_SMALL   (21u) /**< The provided buffer size is too small */
#define DEM_BUSY                                                                                                       \
    (22u) /**< A different Dem_SelectDTC dependent operation according to SWS_Dem_01253 of this client is currently in \
             progress. */
#define DEM_NO_SUCH_ELEMENT                                                                                         \
    (48u) /**< The requested event data is not currently stored (but the request was valid) OR The requested record \
             number is not supported by the event OR The requested DID is not supported by the freeze frame         \
             (GetEventFreezeFrameDataEx) */

/**
 * @name DEM_DTC_GROUP_ENCODE
 * @{
 * @brief Predefined DTC group encoding
 */
#define DEM_DTC_GROUP_ALL_DTCS          ((uint32)0xFFFFFFu) /**< All DTCs */
#define DEM_DTC_GROUP_EMISSION_REL_DTCS ((uint32)0x0u)      /**< Emmission related DTCs for OBD-II */
#define DEM_DTC_GROUP_WWHOBD_DTCS       ((uint32)0xFFFF33u) /**< Emmission related DTCs for WWH-OBD */

#ifdef __cplusplus
}
#endif
#endif
