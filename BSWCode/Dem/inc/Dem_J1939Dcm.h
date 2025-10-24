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
 **  @file               : Dem_J1939Dcm.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : API declaration of Dem used by J1939Dcm
 **
 ***********************************************************************************************************************/

#ifndef DEM_J1939DCM_H
#define DEM_J1939DCM_H

/* =================================================== inclusions =================================================== */
#include "Dem_Lcfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================= external function declarations ========================================= */

#if (DEM_J1939_READING_DTC_SUPPORT == STD_ON)
/* PRQA S 1512 ++ */ /* VL_Dem_1512 */
/**
 * @brief         The function sets the DTC filter for a specific node and returns the composite lamp status of
 *                the filtered DTCs.
 * @param[in]     DTCStatusFilter: The following types are available: DEM_J1939DTC_ACTIVE
 *                                 DEM_J1939DTC_PREVIOUSLY_ACTIVE DEM_J1939DTC_PENDING DEM_J1939DTC_PERMANENT
 *                                 DEM_J1939DTC_CURRENTLY_ACTIVE
 * @param[in]     DTCKind: Defines the functional group of DTCs to be reported (e.g. all DTC, OBD-relevant DTC)
 * @param[in]     DTCOrigin: This parameter is used to select the source memory the DTCs shall be read/cleared from.
 * @param[in]     ClientId: ClientId to address the J1939 event memory
 * @param[out]    LampStatus: Receives the lamp status returned by this function. If the return value of the
 *                            function is other than DEM_FILTERED_OK this parameter does not contain valid data.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation successful
 * @retval        E_NOT_OK: Filter could not be set
 * @reentrant     Re-entrant for different ClientIDs, Non re-entrant for same ClientId.
 * @synchronous   TRUE
 * @trace         CPD-61077
 */
extern Std_ReturnType Dem_J1939DcmSetDTCFilter(
    Dem_J1939DcmDTCStatusFilterType DTCStatusFilter,
    Dem_DTCKindType                 DTCKind,
    Dem_DTCOriginType               DTCOrigin,
    uint8                           ClientId,
    Dem_J1939DcmLampStatusType*     LampStatus);

/**
 * @brief         Gets the number of currently filtered DTCs set by the function Dem_J1939DcmSetDTCFilter.
 * @param[in]     ClientId: ClientId to address the J1939 event memory
 * @param[out]    NumberOfFilteredDTC: The number of DTCs matching the defined status mask.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation successful
 * @retval        E_NOT_OK: Operation successful and result pending.
 * @reentrant     Non Reentrant
 * @synchronous   FALSE
 * @trace         CPD-61079
 */
extern Std_ReturnType Dem_J1939DcmGetNumberOfFilteredDTC(uint16* NumberOfFilteredDTC, uint8 ClientId);

/**
 * @brief         Gets the next filtered J1939 DTC.
 * @param[in]     ClientId: ClientId to address the J1939 event memory
 * @param[out]    J1939DTC: Receives the J1939DTC value. If the return value of the function is other than
 *                          DEM_FILTERED_OK this parameter does not contain valid data.
 * @param[out]    OccurenceCounter: This parameter receives the corresponding occurrence counter. If the
 *                                  return value of the function call is other than DEM_FILTERED_OK this parameter
 *                                  does not contain valid data.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation successful
 * @retval        DEM_NO_SUCH_ELEMENT: The requested element is not available
 * @retval        DEM_PENDING: Operation successful and result pending.
 * @retval        DEM_BUFFER_TOO_SMALL: The provided buffer is too small
 * @reentrant     Non Reentrant
 * @synchronous   FALSE
 * @trace         CPD-61081
 */
extern Std_ReturnType Dem_J1939DcmGetNextFilteredDTC(uint32* J1939DTC, uint8* OccurenceCounter, uint8 ClientId);
/* PRQA S 1512 -- */
#endif

#if ((DEM_J1939_READING_DTC_SUPPORT == STD_ON) || (DEM_J1939_DM31_SUPPORT == STD_ON))
/**
 * @brief         The function sets the filter to the first applicable DTC for the DM31 response for a specific node.
 * @param[in]     ClientId: ClientId to address the J1939 event memory
 * @reentrant     Re-entrant for different ClientIDs, Non re-entrant for same ClientId.
 * @synchronous   TRUE
 * @trace         CPD-61078
 */
extern void Dem_J1939DcmFirstDTCwithLampStatus(uint8 ClientId);

/**
 * @brief         Gets the next filtered J1939 DTC for DM31 including current LampStatus.
 * @param[in]     ClientId: ClientId to address the J1939 event memory
 * @param[out]    LampStatus: Receives the lamp status returned by this function. If the return value of the
 *                            function is other than DEM_FILTERED_OK this parameter does not contain valid data.
 * @param[out]    J1939DTC: Receives the J1939DTC value. If the return value of the function is other than
 *                          DEM_FILTERED_OK this parameter does not contain valid data.
 * @param[out]    OccurenceCounter: This parameter receives the corresponding occurrence counter. If the
 *                                  return value of the function call is other than DEM_FILTERED_OK this parameter
 *                                  does not contain valid data.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation successful
 * @retval        DEM_NO_SUCH_ELEMENT: The requested element is not available
 * @retval        DEM_PENDING: Operation successful and result pending.
 * @retval        DEM_BUFFER_TOO_SMALL: The provided buffer is too small
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61083
 */
extern Std_ReturnType Dem_J1939DcmGetNextDTCwithLampStatus(
    Dem_J1939DcmLampStatusType* LampStatus,
    uint32*                     J1939DTC,
    uint8*                      OccurenceCounter,
    uint8                       ClientId);
#endif

#if (DEM_J1939_CLEAR_DTC_SUPPORT == STD_ON)
/**
 * @brief         Clears the status of all event(s) related to the specified DTC(s), as well as all associated
                  event memory entries for these event(s).
 * @param[in]     DTCTypeFilter: Defines the type of DTCs to be cleared.
 * @param[in]     DTCOrigin: This parameter is used to select the source memory the DTCs shall be read/cleared from.
 * @param[in]     ClientId: ClientId to address the J1939 event memory
 * @return        Std_ReturnType
 * @retval        E_OK: DTC successfully cleared
 * @retval        DEM_WRONG_DTC: Selected DTC value in selected format does not exist or clearing is restricted by
 *                               configuration to group of all DTCs only.
 * @retval        DEM_WRONG_DTCORIGIN: Selected DTCOrigin does not exist
 * @retval        DEM_CLEAR_FAILED: DTC clearing failed
 * @retval        DEM_CLEAR_BUSY: Another client is currently clearing DTCs. The requested operation will
 *                                not be started and the caller shall try again at a later moment.
 * @retval        DEM_CLEAR_MEMORY_ERROR: An error occurred during erasing a memory location (e.g. if
 *                                        DemClearDTCBehavior is set to DEM_CLRRESP_NON-VOLATILE_FINISH and
 *                                        erasing of non-volatile-block failed).
 * @retval        DEM_PENDING: Clearing the DTCs is currently in progress. The caller shall call this
 *                             function again at a later moment.
 * @reentrant     Re-entrant for different ClientIDs, Non re-entrant for same ClientId.
 * @synchronous   FALSE
 * @trace         CPD-61076
 */
extern Std_ReturnType
    Dem_J1939DcmClearDTC(Dem_J1939DcmSetClearFilterType DTCTypeFilter, Dem_DTCOriginType DTCOrigin, uint8 ClientId);
#endif

#if ((DEM_J1939_READING_DTC_SUPPORT == STD_ON) || (DEM_J1939_DM31_SUPPORT == STD_ON))
/* PRQA S 1753,1501 ++ */ /* VL_Dem_1753,VL_Dem_1501 */
/**
 * @brief         The function sets the FreezeFrame filter for a specific node.
 * @param[in]     FreezeFrameKind: The following types are available: DEM_J1939DCM_FREEZEFRAME
 *                                 DEM_J1939DCM_EXPANDED_FREEZEFRAME DEM_J1939DCM_SPNS_IN_EXPANDED_FREEZEFRAME
 * @param[in]     ClientId: ClientId to address the J1939 event memory
 * @return        Std_ReturnType
 * @retval        E_OK: Operation successful
 * @retval        E_NOT_OK: Filter could not be set
 * @reentrant     Re-entrant for different ClientIDs, Non re-entrant for same ClientId.
 * @synchronous   TRUE
 * @trace         CPD-61073
 */
extern Std_ReturnType
    Dem_J1939DcmSetFreezeFrameFilter(Dem_J1939DcmSetFreezeFrameFilterType FreezeFrameKind, uint8 ClientId);

/**
 * @brief         Gets next freeze frame data. The function stores the data in the provided DestBuffer.
 * @param[in]     ClientId: ClientId to address the J1939 event memory
 * @param[out]    J1939DTC: Receives the J1939DTC value. If the return value of the function is other than
 *                          DEM_FILTERED_OK this parameter does not contain valid data.
 * @param[out]    OccurenceCounter: This parameter receives the corresponding occurrence counter. If the
 *                                  return value of the function call is other than DEM_FILTERED_OK this
 *                                  parameter does not contain valid data.
 * @param[inout]  DestBuffer: This parameter contains a byte pointer that points to the buffer, to
 *                            swhich the freeze frame data record shall be written to.
 * @param[inout]  BufSize: When the function is called this parameter contains the maximum number of data
 *                         bytes that can be written to the buffer. The function returns the actual number of writtens
 *                         data bytes in Dest Buffer
 * @return        Std_ReturnType
 * @retval        E_OK: Operation successful
 * @retval        DEM_NO_SUCH_ELEMENT: The requested element is not available
 * @retval        DEM_PENDING: Operation successful and result pending.
 * @retval        DEM_BUFFER_TOO_SMALL: The provided buffer is too small
 * @reentrant     Non Reentrant
 * @synchronous   FALSE
 * @trace         CPD-61075
 */
extern Std_ReturnType Dem_J1939DcmGetNextFreezeFrame(
    uint32* J1939DTC,
    uint8*  OccurenceCounter,
    uint8*  DestBuffer,
    uint16* BufSize,
    uint8   ClientId);

/**
 * @brief         Gets next SPN.
 * @param[in]     ClientId: ClientId to address the J1939 event memory
 * @param[out]    SPNSupported: This parameter contains the next SPN in the ExpandedFreezeFrame
 * @param[out]    SPNDataLength: The SPN data length
 * @return        Std_ReturnType
 * @retval        E_OK: Operation successful
 * @retval        DEM_NO_SUCH_ELEMENT: The requested element is not available
 * @retval        DEM_PENDING: Operation successful and result pending.
 * @reentrant     Non Reentrant
 * @synchronous   FALSE
 * @trace         CPD-61073
 */
extern Std_ReturnType Dem_J1939DcmGetNextSPNInFreezeFrame(uint32* SPNSupported, uint8* SPNDataLength, uint8 ClientId);
/* PRQA S 1753,1501 -- */
#endif

#if (DEM_J1939_RATIO_SUPPORT == STD_ON)
/**
 * @brief         The function sets the Ratio filter for a specific node and returns the corresponding
 *                Ignition Cycle Counter and General Denominator.
 * @param[in]     ClientId: ClientId to address the J1939 event memory
 * @param[out]    IgnitionCycleCounter: Ignition Cycle Counter
 * @param[out]    OBDMonitoringConditionsEncountered: OBD Monitoring Conditions Encountered
 * @return        Std_ReturnType
 * @retval        E_OK: Operation successful
 * @retval        E_NOT_OK: Filter could not be set
 * @reentrant     Re-entrant for different ClientIDs, Non re-entrant for same ClientId.
 * @synchronous   TRUE
 * @trace         CPD-61072
 */
extern Std_ReturnType Dem_J1939DcmSetRatioFilter(
    uint16* IgnitionCycleCounter,
    uint16* OBDMonitoringConditionsEncountered,
    uint8   ClientId);

/**
 * @brief         Gets the next filtered Ratio.
 * @param[in]     ClientId: ClientId to address the J1939 event memory
 * @param[out]    SPN: Receives the SPN of the applicaple system monitor. If the return value of the function
 *                     is other than DEM_FILTERED_OK this parameter does not contain valid data.
 * @param[out]    Numerator: Receives the Numerator of the applicable system monitor. If the return value of
 *                           the function is other than DEM_FILTERED_OK this parameter does not contain valid data.
 * @param[out]    Denominator: Receives the Denominator of the applicable system monitor. If the return value
 *                             of the function is other than DEM_FILTERED_OK this parameter does not contain
 *                             valid data.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation successful
 * @retval        DEM_NO_SUCH_ELEMENT: The requested element is not available
 * @retval        DEM_PENDING: Operation successful and result pending.
 * @retval        DEM_BUFFER_TOO_SMALL: The provided buffer is too small
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61071
 */
extern Std_ReturnType
    Dem_J1939DcmGetNextFilteredRatio(uint32* SPN, uint16* Numerator, uint16* Denominator, uint8 ClientId);
#endif

#if (DEM_J1939_READINESS1_SUPPORT == STD_ON)
/**
 * @brief         Service to report the value of Diagnostic Readiness 1 (DM05) computed by the Dem.
 * @param[in]     ClientId: ClientId to address the J1939 event memory
 * @param[out]    DataValue: Buffer of 8 bytes containing the contents of Diagnostic Readiness1 (DM05)
 *                           computed by the Dem.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     Re-entrant for different ClientIDs, Non re-entrant for same ClientId.
 * @synchronous   TRUE
 * @trace         CPD-61070
 */
extern Std_ReturnType
    Dem_J1939DcmReadDiagnosticReadiness1(Dem_J1939DcmDiagnosticReadiness1Type* DataValue, uint8 ClientId);
#endif

#if (DEM_J1939_READINESS2_SUPPORT == STD_ON)
/**
 * @brief         Service to report the value of Diagnostic Readiness 2 (DM21) computed by the Dem.
 * @param[in]     ClientId: ClientId to address the J1939 event memory
 * @param[out]    DataValue: Buffer of 8 bytes containing the contents of Diagnostic Readiness2 (DM21)
 *                           computed by the Dem.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     Re-entrant for different ClientIDs, Non re-entrant for same ClientId.
 * @synchronous   TRUE
 * @trace         CPD-61068
 */
extern Std_ReturnType
    Dem_J1939DcmReadDiagnosticReadiness2(Dem_J1939DcmDiagnosticReadiness2Type* DataValue, uint8 ClientId);
#endif

#if (DEM_J1939_READINESS3_SUPPORT == STD_ON)
/**
 * @brief         Service to report the value of Diagnostic Readiness 3 (DM26) computed by the Dem.
 * @param[in]     ClientId: ClientId to address the J1939 event memory
 * @param[out]    DataValue: Buffer of 8 bytes containing the contents of Diagnostic Readiness3 (DM26)
 *                           computed by the Dem.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     Re-entrant for different ClientIDs, Non re-entrant for same ClientId.
 * @synchronous   TRUE
 * @trace         CPD-61069
 */
extern Std_ReturnType
    Dem_J1939DcmReadDiagnosticReadiness3(Dem_J1939DcmDiagnosticReadiness3Type* DataValue, uint8 ClientId);
#endif

#ifdef __cplusplus
}
#endif
#endif
