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
 **  @file               : Dem_J1939.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : Implementation file of Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_J1939_H
#define DEM_J1939_H

/* =================================================== inclusions =================================================== */
#include "Dem_Lcfg.h"
#if (DEM_J1939_SUPPORT == STD_ON)
#include "J1939Dcm_Dem.h"
#include "Dem_Client.h"
#include "Dem_InitState.h"
#if (DEM_J1939_CLEAR_DTC_SUPPORT == STD_ON)
#include "Dem_ClearDTC.h"
#endif
#include "Dem_Indicator.h"

/* ===================================================== macros ===================================================== */
/**
 * @name  DEM_FILTERDATA_ENCODE
 * @{
 * @brief Macros encoding DTC status filter types for J1939Dcm filter requests
 */
#define DEM_FILTERDATA_J1939FILTER_ACTIVE            (1u) /**< Filter active DTCs */
#define DEM_FILTERDATA_J1939FILTER_PREVIOUSLY_ACTIVE (2u) /**< Filter previously active DTCs */
#define DEM_FILTERDATA_J1939FILTER_PENDING           (3u) /**< Filter pending DTCs */
#define DEM_FILTERDATA_J1939FILTER_CURRENTLY_ACTIVE  (4u) /**< Filter currently active DTCs */
/** @} */

/**
 * @name  DEM_J1939_OPRATION_ENCODE
 * @{
 * @brief Macros encoding for J1939 operation value
 */
#define DEM_J1939_OC_MAX                     (0x7Eu) /**< Occurrence Counter max value in j1939 */
#define DEM_J1939_OC_INVALID                 (0xFFu) /**< Occurrence Counter invalid value in j1939 */
#define DEM_J1939_OBD_COMPLIANCY_INVALID     (0x05u) /**< OBDCompliancy invalid value in j1939 */
#define DEM_J1939_READINESS_MAX_MATCH_NUMBER (250u)  /**< max number Of Filtered DTC in Readiness */
/** @} */

/**
 * @name  DEM_J1939_LAMP_ENCODE
 * @{
 * @brief Macros encoding J1939 lamp states
 */
#define DEM_J1939_LAMP_NA  (0x03u) /**< Indicator not configured */
#define DEM_J1939_LAMP_OFF (0x00u) /**< Indicator off */
#define DEM_J1939_LAMP_ON  (0x01u) /**< Indicator on */
/** @} */

/**
 * @name  DEM_J1939_LAMP_OFFSET_ENCODE
 * @{
 * @brief Macros encoding J1939 lamp offset
 */
#define DEM_J1939_LAMP_MIL_OFFSET (6u) /**< offset for MIL indicator */
#define DEM_J1939_LAMP_RSL_OFFSET (4u) /**< offset for RSL indicator */
#define DEM_J1939_LAMP_AWL_OFFSET (2u) /**< offset for AWL indicator */
/** @} */

/**
 * @name  DEM_J1939_BIT_MASK_ENCODE
 * @{
 * @brief Macros encoding J1939 bit mask
 */
#define DEM_J1939_LAMP_MIL_NA             (DEM_J1939_LAMP_NA << 6u)  /**< Bitmask for MIL indicator */
#define DEM_J1939_LAMP_RSL_NA             (DEM_J1939_LAMP_NA << 4u)  /**< Bitmask for RSL indicator */
#define DEM_J1939_LAMP_AWL_NA             (DEM_J1939_LAMP_NA << 2u)  /**< Bitmask for AWL indicator */
#define DEM_J1939_LAMP_PL_NA              (DEM_J1939_LAMP_NA)        /**< Bitmask for PL indicator */
#define DEM_J1939_GLOBAL_LAMP_MIL_NO_LAMP (DEM_J1939_LAMP_OFF << 6u) /**< Bitmask for MIL indicator */
#define DEM_J1939_GLOBAL_LAMP_RSL_NO_LAMP (DEM_J1939_LAMP_OFF << 4u) /**< Bitmask for RSL indicator */
#define DEM_J1939_GLOBAL_LAMP_AWL_NO_LAMP (DEM_J1939_LAMP_OFF << 2u) /**< Bitmask for AWL indicator */
#define DEM_J1939_GLOBAL_LAMP_PL_NO_LAMP  (DEM_J1939_LAMP_OFF)       /**< Bitmask for PL indicator */
/** @} */

/**
 * @name  DEM_J1939_FLASHLAMP_ENCODE
 * @{
 * @brief Macros encoding J1939 flash lamp states
 */
#define DEM_J1939_FLASHLAMP_FAST_FLASH   (0x01u) /**< Indicator fast flashing */
#define DEM_J1939_FLASHLAMP_SLOW_FLASH   (0x00u) /**< Indicator slow flashing */
#define DEM_J1939_FLASHLAMP_NO_FLASH     (0x03u) /**< Indicator not flashing (either not configured, off or continuous) */
#define DEM_J1939_FLASHLAMP_MIL_NO_FLASH (DEM_J1939_FLASHLAMP_NO_FLASH << 6u) /**< Bitmask for MIL flashing */
#define DEM_J1939_FLASHLAMP_RSL_NO_FLASH (DEM_J1939_FLASHLAMP_NO_FLASH << 4u) /**< Bitmask for RSL flashing */
#define DEM_J1939_FLASHLAMP_AWL_NO_FLASH (DEM_J1939_FLASHLAMP_NO_FLASH << 2u) /**< Bitmask for AWL flashing */
#define DEM_J1939_FLASHLAMP_PL_NO_FLASH  (DEM_J1939_FLASHLAMP_NO_FLASH)       /**< Bitmask for PL flashing */
/** @} */

#define DEM_J1939_GLOBAL_FLASHLAMP_NO_LAMP                                                                   \
    ((DEM_J1939_FLASHLAMP_MIL_NO_FLASH | DEM_J1939_FLASHLAMP_RSL_NO_FLASH | DEM_J1939_FLASHLAMP_AWL_NO_FLASH \
      | DEM_J1939_FLASHLAMP_PL_NO_FLASH)) /**< Global bitmask for MIL flashing */

#define DEM_J1939_GLOBAL_LAMP_NO_LAMP                                                                           \
    ((DEM_J1939_GLOBAL_LAMP_MIL_NO_LAMP | DEM_J1939_GLOBAL_LAMP_RSL_NO_LAMP | DEM_J1939_GLOBAL_LAMP_AWL_NO_LAMP \
      | DEM_J1939_GLOBAL_LAMP_PL_NO_LAMP)) /**< Global bitmask for RSL flashing */

#define DEM_J1939_EVENT_FLASHLAMP_NO_LAMP                                                                    \
    ((DEM_J1939_FLASHLAMP_MIL_NO_FLASH | DEM_J1939_FLASHLAMP_RSL_NO_FLASH | DEM_J1939_FLASHLAMP_AWL_NO_FLASH \
      | DEM_J1939_FLASHLAMP_PL_NO_FLASH)) /**< Global bitmask for AWL flashing */

#define DEM_J1939_EVENT_LAMP_NO_LAMP                                        \
    ((DEM_J1939_LAMP_MIL_NA | DEM_J1939_LAMP_RSL_NA | DEM_J1939_LAMP_AWL_NA \
      | DEM_J1939_LAMP_PL_NA)) /**< Global bitmask for PL flashing */

/* PRQA S 3472 ++ */ /* VL_Dem_3472 */
/** Status 'ActiveDTC' */
#define DEM_DTC_CHECK_J1939_STATUS_ADTC(Status)             \
    (((Status) & (DEM_UDS_STATUS_CDTC | DEM_UDS_STATUS_TF)) \
     == (DEM_UDS_STATUS_CDTC | DEM_UDS_STATUS_TF)) /**< Test active DTC state of UDS status */

/** Status 'PreviouslyActiveDTC' - */
#define DEM_DTC_CHECK_J1939_STATUS_PADTC(Status)            \
    (((Status) & (DEM_UDS_STATUS_CDTC | DEM_UDS_STATUS_TF)) \
     == (DEM_UDS_STATUS_CDTC)) /**< Test previously active DTC state of UDS status */
/* PRQA S 3472 -- */

/* ========================================== internal function declarations ======================================== */
/* PRQA S 1512 EOF */ /* VL_Dem_1512 */
#if (                                                                                                     \
    ((DEM_OBD_CALLBACK_DTC_STATUS_CHANGED == STD_ON) || (DEM_J1939_CALLBACK_DTC_STATUS_CHANGED == STD_ON) \
     || (DEM_CLIENT_CALLBACK_DTC_STATUS_CHANGED == STD_ON))                                               \
    && (DEM_J1939_SUPPORT == STD_ON))
/**
 * @brief         call the function J1939Dcm_DemTriggerOnDTCStatus
 * @param[in]     EventId: Event ID
 * @param[in]     DtcStatusOld: The old DTC status
 * @param[in]     DtcStatusNew: The old DTC status
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_NotiJ1939DtcStatusChanged(
    Dem_EventIdType       EventId,
    Dem_UdsStatusByteType DtcStatusOld,
    Dem_UdsStatusByteType DtcStatusNew);
#endif

#if (DEM_J1939_CLEAR_DTC_SUPPORT == STD_ON)
/**
 * @brief         Clear the selected J1939 DTCs
 * @param[in]     DTCTypeFilter: Defines the type of DTCs to be cleared.
 * @param[in]     DTCOrigin: This parameter is used to select the source memory the DTCs shall be read/cleared from.
 * @param[in]     ClientId: ClientId to address the J1939 event memory
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType
    Dem_J1939ClearDTC(Dem_J1939DcmSetClearFilterType DTCTypeFilter, Dem_DTCOriginType DTCOrigin, uint8 ClientId);
#endif

#if ((DEM_J1939_READING_DTC_SUPPORT == STD_ON) || (DEM_J1939_DM31_SUPPORT == STD_ON))
/**
 * @brief         Gets an event's occurrence counter for J1939 reporting.
 * @param[in]     EventId: Event ID
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL uint8 Dem_J1939OccurrenceCounter(Dem_EventIdType EventId);

/**
 * @brief         Filters the next J1939 DTC with lamp status.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module
 * @param[out]    FilterResult: Result of filter data
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType
    Dem_ClientJ1939DcmGetNextDTCwithLampStatus(uint8 ClientId, Dem_FilterDataResultType* FilterResult);
#endif

#if ((DEM_J1939_READING_DTC_SUPPORT == STD_ON) || (DEM_J1939_READINESS1_SUPPORT == STD_ON))
/**
 * @brief         Checks if the passed EventId matches the J1939 DTC filter criteria.
 * @param[in]     EventId: Event ID
 * @param[in]     DTCStatusFilterType: The status of filter DTCs.
 * @param[in]     FilterFlags: Flag of filter data
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_FilterDataCheckEventMatchesJ1939DTCFilter(
    Dem_J1939DcmDTCStatusFilterType DTCStatusFilterType,
    uint8                           FilterFlags,
    Dem_EventIdType                 EventId);
#endif

#if (DEM_J1939_READING_DTC_SUPPORT == STD_ON)

#if (DEM_INDICATOR_NUMBER > 0u)
/**
 * @brief         Calculate the node specific J1939 lamp status based on red stop lamp, amber warning lamp, malfunction
 * @param[in]     ClientId: ClientId to address the J1939 event memory
 * @return        Dem_J1939DcmLampStatusType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Dem_J1939DcmLampStatusType Dem_CalculateJ1939GlobalLampStatus(uint8 ClientId);
#endif

/**
 * @brief         Initializes a J1939 DTC filter with the given criteria.
 * @param[in]     DTCStatusFilter: The following types are available: DEM_J1939DTC_ACTIVE
 *                                 DEM_J1939DTC_PREVIOUSLY_ACTIVE DEM_J1939DTC_PENDING DEM_J1939DTC_PERMANENT
 *                                 DEM_J1939DTC_CURRENTLY_ACTIVE
 * @param[in]     DTCKind: Defines the functional group of DTCs to be reported (e.g. all DTC, OBD-relevant DTC)
 * @param[in]     DTCOrigin: This parameter is used to select the source memory the DTCs shall be read/cleared from.
 * @param[in]     ClientId: ClientId to address the J1939 event memory
 * @param[out]    LampStatus: Receives the lamp status returned by this function. If the return value of the
 *                            function is other than DEM_FILTERED_OK this parameter does not contain valid data.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_ClientJ1939DcmSetDTCFilter(
    Dem_J1939DcmDTCStatusFilterType DTCStatusFilter,
    Dem_DTCKindType                 DTCKind,
    Dem_DTCOriginType               DTCOrigin,
    uint8                           ClientId,
    Dem_J1939DcmLampStatusType*     LampStatus);

/**
 * @brief         Fetch number of DTCs matching the client specific J1939 DTC filter settings.
 * @param[in]     ClientId: ClientId to address the J1939 event memory
 * @return        uint16
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL uint16 Dem_ClientJ1939DcmGetNumberOfFilteredDTC(uint8 ClientId);

/**
 * @brief         Get the next matching event.
 * @param[in]     DcmClientId: ClientId to address the J1939 event memory
 * @param[out]    FilterResult: Result of filter data
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType
    Dem_FilterDataGetNextFilteredJ1939DTC(uint8 DcmClientId, Dem_FilterDataResultType* FilterResult);
#endif

#if ((DEM_J1939_EXPANDED_FREEZE_FRAME_SUPPORT == STD_ON) || (DEM_J1939_FREEZE_FRAME_SUPPORT == STD_ON))
/**
 * @brief         Sets a J1939 freeze frame record filter with the given criteria.
 * @param[in]     ClientId: ClientId to address the J1939 event memory
 * @param[in]     FreezeFrameKind: The following types are available: DEM_J1939DCM_FREEZEFRAME
 *                                 DEM_J1939DCM_EXPANDED_FREEZEFRAME DEM_J1939DCM_SPNS_IN_EXPANDED_FREEZEFRAME
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void
    Dem_ClientJ1939DcmSetFreezeFrameFilter(uint8 ClientId, Dem_J1939DcmSetFreezeFrameFilterType FreezeFrameKind);

/**
 * @brief         Filters the next J1939 DTC
 * @param[in]     ClientId: ClientId to address the J1939 event memory
 * @param[in]     LastEventId: Last Event Id
 * @param[in]     FreezeFrameKind: The following types are available: DEM_J1939DCM_FREEZEFRAME
 *                                 DEM_J1939DCM_EXPANDED_FREEZEFRAME DEM_J1939DCM_SPNS_IN_EXPANDED_FREEZEFRAME
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Dem_EventIdType Dem_FreezeFrameIteratorFindNextJ1939DTC(
    uint8                                ClientId,
    Dem_EventIdType                      LastEventId,
    Dem_J1939DcmSetFreezeFrameFilterType FreezeFrameKind);

/**
 * @brief         Get the next matching J1939 freeze frame record event.
 * @param[in]     ClientId: ClientId to address the J1939 event memory
 * @param[out]    FFIteratorFilterData: The filter FF iterator
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean
    Dem_FreezeFrameIteratorGetNextJ1939event(uint8 ClientId, Dem_FFIteratorFilterType* FFIteratorFilterData);

/**
 * @brief         Get the next matching J1939 freeze frame record.
 * @param[in]     ClientId: ClientId to address the J1939 event memory
 * @param[out]    J1939DTC: Receives the J1939DTC value. If the return value of the function is other than
 *                          DEM_FILTERED_OK this parameter does not contain valid data.
 * @param[out]    OccurrenceCounter: This parameter receives the corresponding occurrence counter. If the
 *                                  return value of the function call is other than DEM_FILTERED_OK this parameter
 *                                  does not contain valid data.
 * @param[out]    DestBuffer: This parameter contains a byte pointer that points to the buffer, to which the
 *                            freeze frame data record shall be written to. The format is raw hexadecimal values
 *                            and contains no header-information.
 * @param[inout]  BufSize: When the function is called this parameter contains the maximum number of data bytes
 *                         that can be written to the buffer. The function returns the actual number of written data
 *                         bytes in this parameter.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_FreezeFrameIteratorGetNextJ1939FreezeFrame(
    uint8   ClientId,
    uint32* J1939DTC,
    uint8*  OccurrenceCounter,
    uint8*  DestBuffer,
    uint16* BufSize);

#if (DEM_MAX_SIZE_J1939EXFREEZEFRAME > 0u)
/**
 * @brief         Get the next matching SPN.
 * @param[out]    FFIteratorFilterData: The filter FF iterator
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_FreezeFrameIteratorGetNextSPN(Dem_FFIteratorFilterType* FFIteratorFilterData);

/**
 * @brief         Get the next matching J1939 freeze frame record event.
 * @param[in]     ClientId: ClientId to address the J1939 event memory
 * @param[out]    FFIteratorFilterData: The filter FF iterator
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_FreezeFrameIteratorGetNextEvent(uint8 ClientId, Dem_FFIteratorFilterType* FFIteratorFilterData);

/**
 * @brief         Get the next matching J1939 freeze frame record.
 * @param[in]     ClientId: ClientId to address the J1939 event memory
 * @param[out]    SPNSupported: This parameter contains the next SPN in the ExpandedFreezeFrame
 * @param[out]    SPNDataLength: The SPN data length
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType
    Dem_FreezeFrameIteratorGetNextSPNInFreezeFrame(uint8 ClientId, uint32* SPNSupported, uint8* SPNDataLength);
#endif
#endif

#if (DEM_J1939_READINESS1_SUPPORT == STD_ON)
/**
 * @brief         Get the diagnostic readiness for the given client
 * @param[in]     ClientId: ClientId to address the J1939 event memory
 * @param[out]    DataValue: Buffer of 8 bytes containing the contents of Diagnostic Readiness1 (DM05)
 *                           computed by the Dem.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void
    Dem_ClientJ1939DcmReadDiagnosticReadiness1(Dem_J1939DcmDiagnosticReadiness1Type* DataValue, uint8 ClientId);
#endif

/* ========================================== internal function definitions ========================================= */
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"

/* PRQA S 4404 ++ */ /* VL_Dem_4404 */
/**
 * @brief         check the data of a DTC filter.
 * @param[in]     ClientId: ClientId to address the J1939 event memory
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_ClientCheckJ1939NextFreezeFrameFilterSet(uint8 ClientId)
{
    Dem_FFIteratorFilterType lFFIteratorFilter =
        Dem_FFIteratorGetFreezeFrameFilterData(Dem_GetJ1939DcmClientIdOfClient(ClientId) + DEM_DCM_CLIENT_NUMBER);
    return (lFFIteratorFilter.FilterState != DEM_FREEZEFRAMEITERATOR_J1939STATE_INIT)
           && ((lFFIteratorFilter.FreezeFrameKind == DEM_J1939DCM_FREEZEFRAME)
               || (lFFIteratorFilter.FreezeFrameKind == DEM_J1939DCM_EXPANDED_FREEZEFRAME));
}

/**
 * @brief         check the data of a DTC filter.
 * @param[in]     ClientId: ClientId to address the J1939 event memory
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_ClientCheckJ1939SPNFreezeFrameFilterSet(uint8 ClientId)
{
    Dem_FFIteratorFilterType lFFIteratorFilter =
        Dem_FFIteratorGetFreezeFrameFilterData(Dem_GetJ1939DcmClientIdOfClient(ClientId) + DEM_DCM_CLIENT_NUMBER);
    return (lFFIteratorFilter.FilterState != DEM_FREEZEFRAMEITERATOR_J1939STATE_INIT)
           && (lFFIteratorFilter.FreezeFrameKind == DEM_J1939DCM_SPNS_IN_EXPANDED_FREEZEFRAME);
}
/* PRQA S 4404 -- */

#if (                                                                                                     \
    ((DEM_OBD_CALLBACK_DTC_STATUS_CHANGED == STD_ON) || (DEM_J1939_CALLBACK_DTC_STATUS_CHANGED == STD_ON) \
     || (DEM_CLIENT_CALLBACK_DTC_STATUS_CHANGED == STD_ON))                                               \
    && (DEM_J1939_SUPPORT == STD_ON))
/**
 * @brief call the function J1939Dcm_DemTriggerOnDTCStatus
 */
DEM_LOCAL void Dem_NotiJ1939DtcStatusChanged(
    Dem_EventIdType       EventId,
    Dem_UdsStatusByteType DtcStatusOld,
    Dem_UdsStatusByteType DtcStatusNew)
{
    uint32 lJ1939Dtc = Dem_GetEventJ1939DTC(EventId);
    if (lJ1939Dtc != DEM_J1939_DTC_INVALID_VALUE) /* PRQA S 1252 */ /* VL_Dem_1252 */
    {
#if (DEM_J1939_CALLBACK_DTC_STATUS_CHANGED == STD_ON)
        for (Dem_DTCStatusChangedNumType start = Dem_GetCallbackStartOfJ1939(); start < Dem_GetCallbackEndOfJ1939();
             ++start)
        {
            const DemTriggerOnDTCStatusType pDtcStatusFunc = Dem_GetDTCStatusFnc(start);
            if (pDtcStatusFunc != NULL_PTR)
            {
                (void)pDtcStatusFunc(lJ1939Dtc, DtcStatusOld, DtcStatusNew);
            }
        }
#endif
        /** In case the TestFailed bit of an event within an primary event memory changes and a J1939 DTC number is
         * assigned to that event, the function J1939Dcm_DemTriggerOnDTCStatus shall be called for each DemClient
         * referencing this DemEventMemorySet */
        if (Dem_UDSStatusCheckTF(DtcStatusOld ^ DtcStatusNew) == TRUE)
        {
            Dem_EventClientRefType clientStart = Dem_GetClientStartRefOfEvent(EventId);
            Dem_EventClientRefType clientEnd   = Dem_GetClientEndRefOfEvent(EventId);
            for (; clientStart < clientEnd; ++clientStart)
            {
                J1939Dcm_DemTriggerOnDTCStatus(lJ1939Dtc, Dem_GetClientOfEventRef(clientStart));
            }
        }
    }
}
#endif

#if (DEM_J1939_CLEAR_DTC_SUPPORT == STD_ON)
/**
 * @brief Clear the selected J1939 DTCs
 */
DEM_LOCAL Std_ReturnType
    Dem_J1939ClearDTC(Dem_J1939DcmSetClearFilterType DTCTypeFilter, Dem_DTCOriginType DTCOrigin, uint8 ClientId)
{
    Std_ReturnType ret = DEM_CLEAR_FAILED;
    switch (Dem_GetClientClearDTCStatus(ClientId))
    {
    case DEM_CLEARDTC_STATUS_IDLE:
        /* PRQA S 1258 ++ */ /* VL_Dem_1258 */
        Dem_ClientSelectDTC(ClientId, DEM_DTC_GROUP_ALL_DTCS, DEM_DTC_FORMAT_J1939, DTCOrigin);
        /* PRQA S 1258 -- */
        if (Dem_GetClientResult(ClientId) == DEM_CLIENT_RESULT_ALL_DTCS)
        {
            ret = Dem_ClientStartClearJ1939(ClientId, DTCTypeFilter);
        }
        else
        {
            ret = DEM_WRONG_DTCORIGIN;
        }
        break;
    case DEM_CLEARDTC_STATUS_PENDING:
        ret = DEM_PENDING;
        break;
    case DEM_CLEARDTC_STATUS_DONE:
        Dem_SetClientClearDTCStatus(ClientId, DEM_CLEARDTC_STATUS_IDLE);
        ret = E_OK;
        break;
    case DEM_CLEARDTC_STATUS_FAILED:
        Dem_SetClientClearDTCStatus(ClientId, DEM_CLEARDTC_STATUS_IDLE);
        break;
    default:
        /** return DEM_CLEAR_FAILED */
        break;
    }
    return ret;
}
#endif

#if (((DEM_J1939_DM31_SUPPORT == STD_ON) || (DEM_J1939_READING_DTC_SUPPORT == STD_ON)) && (DEM_INDICATOR_NUMBER > 0u))
/**
 * @brief         Maps indicator behavior to J1939 lamp status
 * @param[in]     IndicatorState: Statue of indicator
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_J1939LampStatus(uint8 IndicatorState)
{
    uint8 ret;
    switch (IndicatorState)
    {
    case DEM_INDICATOR_OFF:
        ret = DEM_J1939_LAMP_OFF;
        break;
    case DEM_INDICATOR_FAST_FLASH:
    case DEM_INDICATOR_SLOW_FLASH:
    case DEM_INDICATOR_CONTINUOUS:
        ret = DEM_J1939_LAMP_ON;
        break;
    default:
        ret = DEM_J1939_LAMP_OFF;
        break;
    }
    return ret;
}

/**
 * @brief         Maps indicator behavior to J1939 flash lamp status
 * @param[in]     IndicatorState: Statue of indicator
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_J1939FlashLampStatus(uint8 IndicatorState)
{
    uint8 ret;
    switch (IndicatorState)
    {
    case DEM_INDICATOR_OFF:
        ret = DEM_J1939_FLASHLAMP_NO_FLASH;
        break;
    case DEM_INDICATOR_FAST_FLASH:
        ret = DEM_J1939_FLASHLAMP_FAST_FLASH;
        break;
    case DEM_INDICATOR_SLOW_FLASH:
        ret = DEM_J1939_FLASHLAMP_SLOW_FLASH;
        break;
    case DEM_INDICATOR_CONTINUOUS:
        ret = DEM_J1939_FLASHLAMP_NO_FLASH;
        break;
    default:
        ret = DEM_J1939_FLASHLAMP_NO_FLASH;
        break;
    }
    return ret;
}
#endif

/* PRQA S 4397 ++ */ /* VL_Dem_4397 */
#if ((DEM_J1939_READING_DTC_SUPPORT == STD_ON) || (DEM_J1939_DM31_SUPPORT == STD_ON))
/**
 * @brief Gets an event's occurrence counter for J1939 reporting.
 */
DEM_LOCAL uint8 Dem_J1939OccurrenceCounter(Dem_EventIdType EventId)
{
    uint8                ret;
    Dem_DTCAttRefNumType lDTCAttr = Dem_GetDTCAttr(EventId);
    if ((Dem_CheckDTCAttr(lDTCAttr) == TRUE) && (Dem_DTCCheckStoredStatus(EventId) == TRUE))
    {
        /** Stored DTC, so occurrence counter is available in primary memory */
        Dem_MemStateInfoConstPtrType memoryInfo = Dem_MemStateInfoInit(Dem_GetMemRef(lDTCAttr));
        uint8 lOccurrenceCounter                = Dem_GetOCCOfMemEntry(Dem_MemoryFindIndex(memoryInfo, EventId));
        /** Latch the counter value at 126 */
        if (lOccurrenceCounter > DEM_J1939_OC_MAX)
        {
            lOccurrenceCounter = DEM_J1939_OC_MAX;
        }
        ret = (uint8)lOccurrenceCounter;
    }
    else
    {
        ret = DEM_J1939_OC_INVALID;
    }
    return ret;
}

/**
 * @brief         Initializes a 'First DTC with lamp status' filter for the given client
 * @param[in]     ClientId: ClientId to address the J1939 event memory
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_ClientJ1939DcmFirstDTCwithLampStatus(uint8 ClientId)
{
    uint8                  dcmClientId = Dem_GetJ1939DcmClientIdOfClient(ClientId) + DEM_DCM_CLIENT_NUMBER;
    Dem_FilterDataInfoType filterData  = Dem_GetFilterData(dcmClientId);
    filterData.Iterator.EventIndex =
        Dem_GetMemStateInfo()[Dem_GetPrimaryRefOfMemSet(Dem_GetMemorySetRefOfClient(ClientId))].FirstEvent;
    filterData.FilterState = DEM_FILTERDATA_FILTER_J1939DTCLAMP;
    Dem_SetFilterData(dcmClientId, filterData);
}

/**
 * @brief calculate a 'DTC with lamp status' filter for the given client
 */
/* PRQA S 3450 ++ */ /* VL_Dem_3450 */
DEM_LOCAL Dem_J1939DcmLampStatusType Dem_CalculateEventJ1939LampStatus(uint8 ClientId, Dem_EventIdType EventId)
/* PRQA S 3450 -- */
{
    Dem_J1939DcmLampStatusType status;
    if (Dem_CheckEventHaveSpecialIndicator(EventId) == TRUE)
    {
#if (                                                                            \
    (DEM_INDICATOR_MIL_ENABLE == STD_ON) || (DEM_INDICATOR_RSL_ENABLE == STD_ON) \
    || (DEM_INDICATOR_AWL_ENABLE == STD_ON) || (DEM_INDICATOR_PL_ENABLE == STD_ON))
        uint8 eventMemorySetRef = Dem_GetMemorySetRefOfClient(ClientId);
#endif
#if (DEM_INDICATOR_MIL_ENABLE == STD_ON)
        uint8 milState;
#endif
#if (DEM_INDICATOR_RSL_ENABLE == STD_ON)
        uint8 rslState;
#endif
#if (DEM_INDICATOR_AWL_ENABLE == STD_ON)
        uint8 awlState;
#endif
#if (DEM_INDICATOR_PL_ENABLE == STD_ON)
        uint8 plState;
#endif
        uint8 lampStatus = 0u;

#if (DEM_INDICATOR_MIL_ENABLE == STD_ON)
        if ((Dem_EventCheckHaveSpecialIndicator(EventId, Dem_GetMILIndOfMemSet(eventMemorySetRef)) == TRUE))
        {
            milState = Dem_GetStatusOfIndicator(Dem_GetMILIndOfMemSet(ClientId));
            lampStatus |= (Dem_J1939LampStatus(milState) << DEM_J1939_LAMP_MIL_OFFSET);
        }
        else
        {
            milState = DEM_INDICATOR_OFF;
            lampStatus |= DEM_J1939_LAMP_MIL_NA;
        }
#else
        lampStatus |= DEM_J1939_LAMP_MIL_NA;
#endif
#if (DEM_INDICATOR_RSL_ENABLE == STD_ON)
        if ((Dem_EventCheckHaveSpecialIndicator(EventId, Dem_GetRSLIndOfMemSet(eventMemorySetRef)) == TRUE))
        {
            rslState = Dem_GetStatusOfIndicator(Dem_GetRSLIndOfMemSet(ClientId));
            lampStatus |= (Dem_J1939LampStatus(rslState) << DEM_J1939_LAMP_RSL_OFFSET);
        }
        else
        {
            rslState = DEM_INDICATOR_OFF;
            lampStatus |= DEM_J1939_LAMP_RSL_NA;
        }
#else
        lampStatus |= DEM_J1939_LAMP_RSL_NA;
#endif
#if (DEM_INDICATOR_AWL_ENABLE == STD_ON)
        if ((Dem_EventCheckHaveSpecialIndicator(EventId, Dem_GetAWLIndOfMemSet(eventMemorySetRef)) == TRUE))
        {
            awlState = Dem_GetStatusOfIndicator(Dem_GetAWLIndOfMemSet(ClientId));
            lampStatus |= (Dem_J1939LampStatus(awlState) << DEM_J1939_LAMP_AWL_OFFSET);
        }
        else
        {
            awlState = DEM_INDICATOR_OFF;
            lampStatus |= DEM_J1939_LAMP_AWL_NA;
        }
#else
        lampStatus |= DEM_J1939_LAMP_AWL_NA;
#endif
#if (DEM_INDICATOR_PL_ENABLE == STD_ON)
        if ((Dem_EventCheckHaveSpecialIndicator(EventId, Dem_GetPLIndOfMemSet(eventMemorySetRef)) == TRUE))
        {
            plState = Dem_GetStatusOfIndicator(Dem_GetPLIndOfMemSet(ClientId));
            lampStatus |= Dem_J1939LampStatus(plState);
        }
        else
        {
            plState = DEM_INDICATOR_OFF;
            lampStatus |= DEM_J1939_LAMP_PL_NA;
        }
#else
        lampStatus |= DEM_J1939_LAMP_PL_NA;
#endif

        /** calculate node related lamp status */
        status.LampStatus = lampStatus;

        /** calculate node related flash lamp status */
        status.FlashLampStatus = 0u
#if (DEM_INDICATOR_MIL_ENABLE == STD_ON)
                                 | (Dem_J1939FlashLampStatus(milState) << DEM_J1939_LAMP_MIL_OFFSET)
#else
                                 | DEM_J1939_FLASHLAMP_MIL_NO_FLASH
#endif
#if (DEM_INDICATOR_RSL_ENABLE == STD_ON)
                                 | (Dem_J1939FlashLampStatus(rslState) << DEM_J1939_LAMP_RSL_OFFSET)
#else
                                 | DEM_J1939_FLASHLAMP_RSL_NO_FLASH
#endif
#if (DEM_INDICATOR_AWL_ENABLE == STD_ON)
                                 | (Dem_J1939FlashLampStatus(awlState) << DEM_J1939_LAMP_AWL_OFFSET)
#else
                                 | DEM_J1939_FLASHLAMP_AWL_NO_FLASH
#endif
#if (DEM_INDICATOR_PL_ENABLE == STD_ON)
                                 | (Dem_J1939FlashLampStatus(plState))
#else
                                 | DEM_J1939_FLASHLAMP_PL_NO_FLASH
#endif
            ;
    }
    else
    {
        /** No special indicator available for this event, so all lamps are set to no applicable */
        status.LampStatus =
            (uint8)(DEM_J1939_LAMP_MIL_NA | DEM_J1939_LAMP_RSL_NA | DEM_J1939_LAMP_AWL_NA | DEM_J1939_LAMP_PL_NA);
        status.FlashLampStatus = (uint8)(DEM_J1939_FLASHLAMP_MIL_NO_FLASH | DEM_J1939_FLASHLAMP_RSL_NO_FLASH
                                         | DEM_J1939_FLASHLAMP_AWL_NO_FLASH | DEM_J1939_FLASHLAMP_PL_NO_FLASH);
    }
    DEM_PARAM_UNUSED(ClientId);
    return status;
}

/**
 * @brief Filters the next J1939 DTC with lamp status.
 */
DEM_LOCAL Std_ReturnType
    Dem_ClientJ1939DcmGetNextDTCwithLampStatus(uint8 ClientId, Dem_FilterDataResultType* FilterResult)
{
    Std_ReturnType         ret         = DEM_NO_SUCH_ELEMENT;
    uint8                  dcmClientId = Dem_GetJ1939DcmClientIdOfClient(ClientId) + DEM_DCM_CLIENT_NUMBER;
    Dem_FilterDataInfoType filterData  = Dem_GetFilterData(dcmClientId);
    Dem_EventIdType        eventId     = filterData.Iterator.EventIndex;

    /** if last EventId is not valid or was still analyzed skip the filtering */
    if (eventId != DEM_EVENT_INVALID)
    {
        Dem_EventIdType eventIdLast =
            Dem_GetMemStateInfo()[Dem_GetPrimaryRefOfMemSet(Dem_GetMemorySetRefOfClient(ClientId))].LastEvent;
        /** iterate through the EventIds and check filter criteria */
        for (; eventId < eventIdLast; eventId++)
        {
            /* PRQA S 1252,1258 ++ */ /* VL_Dem_1252,VL_Dem_1258 */
            if (Dem_GetEventJ1939DTC(eventId) == DEM_DTC_J1939_INVALID)
            /* PRQA S 1252,1258 -- */
            {
                /** Don't match non-J1939 events */
                continue; /* PRQA S 0770 */ /* VL_Dem_0770 */
            }
            if (Dem_CheckDTCSuppressed(eventId) == TRUE)
            {
                /** Don't match suppressed events */
                continue; /* PRQA S 0770 */ /* VL_Dem_0770 */
            }
#if (DEM_INDICATOR_NUMBER > 0u)
            /*Get lamp status*/
            FilterResult->LampStatus = Dem_CalculateEventJ1939LampStatus(ClientId, eventId);
#else
            FilterResult->LampStatus.FlashLampStatus = DEM_J1939_EVENT_FLASHLAMP_NO_LAMP;
            FilterResult->LampStatus.LampStatus      = DEM_J1939_EVENT_LAMP_NO_LAMP;
#endif
            /** Get OCC and DTC */
            FilterResult->DTC               = Dem_GetEventJ1939DTC(eventId);
            FilterResult->OccurrenceCounter = Dem_J1939OccurrenceCounter(eventId);
            ret                             = E_OK;
            break;
        }

        if (eventId >= eventIdLast)
        {
            /** last EventId has been processed */
            filterData.Iterator.EventIndex = DEM_EVENT_INVALID;
        }
        else
        {
            /** write back the EventId which was raised by 1 for the next repetition */
            filterData.Iterator.EventIndex = (Dem_EventIdType)(eventId + 1u);
        }
        Dem_SetFilterData(dcmClientId, filterData);
    }
    /** else return value DEM_NO_SUCH_ELEMENT initially set */
    return ret;
}
#endif

#if ((DEM_J1939_READING_DTC_SUPPORT == STD_ON) || (DEM_J1939_READINESS1_SUPPORT == STD_ON))
/**
 * @brief Checks if the passed EventId matches the J1939 DTC filter criteria.
 */
DEM_LOCAL boolean Dem_FilterDataCheckEventMatchesJ1939DTCFilter(
    Dem_J1939DcmDTCStatusFilterType DTCStatusFilterType,
    uint8                           FilterFlags,
    Dem_EventIdType                 EventId)
{
    boolean match = FALSE;
    /** Does the event support a lJ1939Dtc */
    if ((Dem_CheckEventJ1939Related(EventId) == TRUE)
        && (Dem_CheckDTCSuppressed(EventId) == FALSE)
        /** check Dem_DTCKindType */
        && ((DEM_FLAG_ISSET(FilterFlags, DEM_FILTERDATA_FILTERFLAGS_FILTERALLDTC))
            || ((DEM_FLAG_ISSET(FilterFlags, DEM_FILTERDATA_FILTERFLAGS_FILTEREMISSIONRELDTC))
                && (Dem_GetObdDTCRefOfEvent(EventId) != DEM_OBD_DTC_NUMBER))))
    {
        Dem_UdsStatusByteType eventStatus = Dem_DTCApplyExternalStatus(EventId, Dem_GetDTCUDSStatus(EventId));
        switch (DTCStatusFilterType)
        {
        case DEM_FILTERDATA_J1939FILTER_ACTIVE:
            if (DEM_DTC_CHECK_J1939_STATUS_ADTC(eventStatus))
            {
                match = TRUE;
            }
            break;
        case DEM_FILTERDATA_J1939FILTER_PREVIOUSLY_ACTIVE:
            if (DEM_DTC_CHECK_J1939_STATUS_PADTC(eventStatus))
            {
                match = TRUE;
            }
            break;
        case DEM_FILTERDATA_J1939FILTER_PENDING:
            if (Dem_UDSStatusCheckPDTC(eventStatus) == TRUE)
            {
                match = TRUE;
            }
            break;
        case DEM_FILTERDATA_J1939FILTER_CURRENTLY_ACTIVE:
            if (Dem_UDSStatusCheckTF(eventStatus) == TRUE)
            {
                match = TRUE;
            }
            break;
        default:
            /** match == FALSE already set */
            break;
        }
    }
    return match;
}
#endif

#if (DEM_J1939_READING_DTC_SUPPORT == STD_ON)

#if (DEM_INDICATOR_NUMBER > 0u)
/**
 * @brief Calculate the node specific J1939 lamp status based on red stop lamp, amber warning lamp, malfunction
 * indicator lamp and the protect lamp.
 */
DEM_LOCAL Dem_J1939DcmLampStatusType Dem_CalculateJ1939GlobalLampStatus(uint8 ClientId)
{
    Dem_J1939DcmLampStatusType status;

#if (DEM_INDICATOR_MIL_ENABLE == STD_ON)
    uint8 milState;
#endif
#if (DEM_INDICATOR_RSL_ENABLE == STD_ON)
    uint8 rslState;
#endif
#if (DEM_INDICATOR_AWL_ENABLE == STD_ON)
    uint8 awlState;
#endif
#if (DEM_INDICATOR_PL_ENABLE == STD_ON)
    uint8 plState;
#endif

#if (DEM_INDICATOR_MIL_ENABLE == STD_ON)
    if (Dem_GetMILIndOfMemSet(ClientId) != DEM_INDICATOR_NUMBER)
    {
        milState = Dem_GetStatusOfIndicator(Dem_GetMILIndOfMemSet(ClientId));
    }
    else
    {
        milState = DEM_INDICATOR_OFF;
    }
#endif
#if (DEM_INDICATOR_RSL_ENABLE == STD_ON)
    if (Dem_GetRSLIndOfMemSet(ClientId) != DEM_INDICATOR_NUMBER)
    {
        rslState = Dem_GetStatusOfIndicator(Dem_GetRSLIndOfMemSet(ClientId));
    }
    else
    {
        rslState = DEM_INDICATOR_OFF;
    }
#endif
#if (DEM_INDICATOR_AWL_ENABLE == STD_ON)
    if (Dem_GetAWLIndOfMemSet(ClientId) != DEM_INDICATOR_NUMBER)
    {
        awlState = Dem_GetStatusOfIndicator(Dem_GetAWLIndOfMemSet(ClientId));
    }
    else
    {
        awlState = DEM_INDICATOR_OFF;
    }
#endif
#if (DEM_INDICATOR_PL_ENABLE == STD_ON)
    if (Dem_GetPLIndOfMemSet(ClientId) != DEM_INDICATOR_NUMBER)
    {
        plState = Dem_GetStatusOfIndicator(Dem_GetPLIndOfMemSet(ClientId));
    }
    else
    {
        plState = DEM_INDICATOR_OFF;
    }
#endif

    /** calculate node related lamp status */
    status.LampStatus = 0u
#if (DEM_INDICATOR_MIL_ENABLE == STD_ON)
                        | (Dem_J1939LampStatus(milState) << DEM_J1939_LAMP_MIL_OFFSET)
#endif
#if (DEM_INDICATOR_RSL_ENABLE == STD_ON)
                        | (Dem_J1939LampStatus(rslState) << DEM_J1939_LAMP_RSL_OFFSET)
#endif
#if (DEM_INDICATOR_AWL_ENABLE == STD_ON)
                        | (Dem_J1939LampStatus(awlState) << DEM_J1939_LAMP_AWL_OFFSET)
#endif
#if (DEM_INDICATOR_PL_ENABLE == STD_ON)
                        | (Dem_J1939LampStatus(plState))
#endif
        ;

    /** calculate node related flash lamp status */
    status.FlashLampStatus = 0u
#if (DEM_INDICATOR_MIL_ENABLE == STD_ON)
                             | (Dem_J1939FlashLampStatus(milState) << DEM_J1939_LAMP_MIL_OFFSET)
#else
                             | DEM_J1939_FLASHLAMP_MIL_NO_FLASH
#endif
#if (DEM_INDICATOR_RSL_ENABLE == STD_ON)
                             | (Dem_J1939FlashLampStatus(rslState) << DEM_J1939_LAMP_RSL_OFFSET)
#else
                             | DEM_J1939_FLASHLAMP_RSL_NO_FLASH
#endif
#if (DEM_INDICATOR_AWL_ENABLE == STD_ON)
                             | (Dem_J1939FlashLampStatus(awlState) << DEM_J1939_LAMP_AWL_OFFSET)
#else
                             | DEM_J1939_FLASHLAMP_AWL_NO_FLASH
#endif
#if (DEM_INDICATOR_PL_ENABLE == STD_ON)
                             | (Dem_J1939FlashLampStatus(plState))
#else
                             | DEM_J1939_FLASHLAMP_PL_NO_FLASH
#endif
        ;
    DEM_PARAM_UNUSED(ClientId);
    return status;
}
#endif
/* PRQA S 4397 -- */

/**
 * @brief Initializes a J1939 DTC filter with the given criteria.
 */
DEM_LOCAL Std_ReturnType Dem_ClientJ1939DcmSetDTCFilter(
    Dem_J1939DcmDTCStatusFilterType DTCStatusFilter,
    Dem_DTCKindType                 DTCKind,
    Dem_DTCOriginType               DTCOrigin,
    uint8                           ClientId,
    Dem_J1939DcmLampStatusType*     LampStatus)
{
    Std_ReturnType         ret         = E_OK;
    uint8                  dcmClientId = Dem_GetJ1939DcmClientIdOfClient(ClientId) + DEM_DCM_CLIENT_NUMBER;
    Dem_FilterDataInfoType filterData;
    uint8                  lDTCStatusFilterType;

    filterData             = Dem_GetFilterData(dcmClientId);
    filterData.FilterFlags = 0u;
    /* PRQA S 3473 ++ */ /* VL_Dem_3473 */
    if (DTCKind == DEM_DTC_KIND_ALL_DTCS)
    {
        DEM_FLAG_SET(filterData.FilterFlags, DEM_FILTERDATA_FILTERFLAGS_FILTERALLDTC);
    }
    else
    {
        DEM_FLAG_SET(filterData.FilterFlags, DEM_FILTERDATA_FILTERFLAGS_FILTEREMISSIONRELDTC);
    }
    /* PRQA S 3473 -- */
    /** Set the first index dependent on the requested DTC status and configuration */
    switch (DTCStatusFilter)
    {
    case DEM_J1939DTC_PENDING:
        lDTCStatusFilterType = DEM_FILTERDATA_J1939FILTER_PENDING;
        break;
    case DEM_J1939DTC_ACTIVE:
        lDTCStatusFilterType = DEM_FILTERDATA_J1939FILTER_ACTIVE;
        break;
    case DEM_J1939DTC_PREVIOUSLY_ACTIVE:
        lDTCStatusFilterType = DEM_FILTERDATA_J1939FILTER_PREVIOUSLY_ACTIVE;
        break;
    case DEM_J1939DTC_CURRENTLY_ACTIVE:
        lDTCStatusFilterType = DEM_FILTERDATA_J1939FILTER_CURRENTLY_ACTIVE;
        break;
    default:
        lDTCStatusFilterType = 0u;
        ret                  = E_NOT_OK;
        break;
    }

    if (ret == E_OK)
    {
        Dem_MemoryNumType memIndex;
        filterData.DTCStatusFilterType = lDTCStatusFilterType;
        if (Dem_DTCOriginCheckValidOnClient(ClientId, DTCOrigin, &memIndex) == TRUE)
        {
            /** Dispatch the Event target */
            filterData.MemIndex            = memIndex;
            filterData.Iterator.EventIndex = Dem_GetMemStateInfo()[memIndex].FirstEvent;
        }
        else
        {
            ret = E_NOT_OK;
        }
/** calculate node related lamp status */
#if (DEM_INDICATOR_NUMBER > 0u)
        *LampStatus = Dem_CalculateJ1939GlobalLampStatus(ClientId);
#else
        LampStatus->FlashLampStatus = DEM_J1939_GLOBAL_FLASHLAMP_NO_LAMP;
        LampStatus->LampStatus      = DEM_J1939_GLOBAL_LAMP_NO_LAMP;
#endif
    }
    filterData.FilterState = DEM_FILTERDATA_FILTER_J1939DTC;
    Dem_SetFilterData(dcmClientId, filterData);
    return ret;
}

/**
 * @brief Fetch number of DTCs matching the client specific J1939 DTC filter settings.
 */
DEM_LOCAL uint16 Dem_ClientJ1939DcmGetNumberOfFilteredDTC(uint8 ClientId)
{
    uint16                          numberOfDTCs = 0u;
    uint8                           dcmClientId  = Dem_GetJ1939DcmClientIdOfClient(ClientId) + DEM_DCM_CLIENT_NUMBER;
    Dem_FilterDataInfoType          filterData   = Dem_GetFilterData(dcmClientId);
    Dem_MemStateInfoConstPtrType    memoryInfo   = Dem_MemStateInfoInit(filterData.MemIndex);
    Dem_EventIdType                 lastEvent    = memoryInfo->LastEvent;
    Dem_EventIdType                 eventId      = memoryInfo->FirstEvent;
    Dem_J1939DcmDTCStatusFilterType lDTCStatusFilterType = filterData.DTCStatusFilterType;
    uint8                           filterFlags          = filterData.FilterFlags;

    /** iterate through the EventIds and check filter criteria */
    for (; eventId < lastEvent; ++eventId)
    {
        if (Dem_FilterDataCheckEventMatchesJ1939DTCFilter(lDTCStatusFilterType, filterFlags, eventId) == TRUE)
        {
            ++numberOfDTCs;
        }
    }
    return numberOfDTCs;
}

/**
 * @brief Get the next matching event.
 */
DEM_LOCAL Std_ReturnType
    Dem_FilterDataGetNextFilteredJ1939DTC(uint8 DcmClientId, Dem_FilterDataResultType* FilterResult)
{
    Std_ReturnType         ret        = DEM_NO_SUCH_ELEMENT;
    Dem_FilterDataInfoType filterData = Dem_GetFilterData(DcmClientId);
    Dem_EventIdType        eventId    = filterData.Iterator.EventIndex;

    /** if last EventId is not valid or was still analyzed skip the filtering */
    if (eventId != DEM_EVENT_INVALID)
    {
        Dem_MemStateInfoConstPtrType    memoryInfo           = Dem_MemStateInfoInit(filterData.MemIndex);
        Dem_EventIdType                 eventIdLast          = memoryInfo->LastEvent;
        Dem_J1939DcmDTCStatusFilterType lDTCStatusFilterType = filterData.DTCStatusFilterType;
        uint8                           filterFlags          = filterData.FilterFlags;

        /** iterate through the EventIds and check filter criteria */
        for (; eventId < eventIdLast; ++eventId)
        {
            if (Dem_FilterDataCheckEventMatchesJ1939DTCFilter(lDTCStatusFilterType, filterFlags, eventId) == TRUE)
            {
                /** identified an Event which matches the filter criteria */
                break;
            }
        }

        if (eventId >= eventIdLast)
        {
            /** last EventId has been processed */
            filterData.Iterator.EventIndex = DEM_EVENT_INVALID;
        }
        else
        {
            /** write back the EventId ( which was raised by 1 for the next repetition) */
            filterData.Iterator.EventIndex = (Dem_EventIdType)(eventId + 1u);
            /** Only EventIds with a valid DTC number should be processed here, so the EventId->DTC mapping will return
             * always a valid DTC number */
            FilterResult->DTC               = Dem_GetEventJ1939DTC(eventId);
            FilterResult->OccurrenceCounter = Dem_J1939OccurrenceCounter(eventId);
            ret                             = E_OK;
        }
        Dem_SetFilterData(DcmClientId, filterData);
    }
    /** else ret = DEM_NO_SUCH_ELEMENT initially set */
    return ret;
}

/**
 * @brief         Get the next matching DTC
 * @param[in]     ClientId: ClientId to address the J1939 event memory
 * @param[out]    J1939DTC: Receives the J1939DTC value. If the return value of the function is other than
 *                          DEM_FILTERED_OK this parameter does not contain valid data.
 * @param[out]    OccurrenceCounter: This parameter receives the corresponding occurrence counter. If the
 *                                  return value of the function call is other than DEM_FILTERED_OK this parameter
 *                                  does not contain valid data.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Std_ReturnType
    Dem_ClientJ1939DcmGetNextFilteredDTC(uint8 ClientId, uint32* J1939DTC, uint8* OccurrenceCounter)
{
    Std_ReturnType           ret;
    uint8                    dcmClientId = Dem_GetJ1939DcmClientIdOfClient(ClientId) + DEM_DCM_CLIENT_NUMBER;
    Dem_FilterDataResultType filterResult;

    ret                = Dem_FilterDataGetNextFilteredJ1939DTC(dcmClientId, &filterResult);
    *J1939DTC          = filterResult.DTC;
    *OccurrenceCounter = filterResult.OccurrenceCounter;
    return ret;
}
#endif

#if ((DEM_J1939_EXPANDED_FREEZE_FRAME_SUPPORT == STD_ON) || (DEM_J1939_FREEZE_FRAME_SUPPORT == STD_ON))
/**
 * @brief Sets a J1939 freeze frame record filter with the given criteria.
 */
DEM_LOCAL void
    Dem_ClientJ1939DcmSetFreezeFrameFilter(uint8 ClientId, Dem_J1939DcmSetFreezeFrameFilterType FreezeFrameKind)
{
    uint8                    dcmClientId       = Dem_GetJ1939DcmClientIdOfClient(ClientId) + DEM_DCM_CLIENT_NUMBER;
    Dem_FFIteratorFilterType lFFIteratorFilter = Dem_FFIteratorGetFreezeFrameFilterData(dcmClientId);
    lFFIteratorFilter.FreezeFrameKind          = FreezeFrameKind;
    lFFIteratorFilter.SPNIter                  = 0u;
    lFFIteratorFilter.EventId =
        Dem_GetMemStateInfo()[Dem_GetPrimaryRefOfMemSet(Dem_GetMemorySetRefOfClient(ClientId))].FirstEvent;
    lFFIteratorFilter.FilterState = DEM_FREEZEFRAMEITERATOR_J1939STATE_USED;
#if (DEM_DCM_CLIENT_NUMBER > 0u)
    lFFIteratorFilter.FilterDTCFormat = DEM_FREEZEFRAMEITERATOR_DTCFORMAT_INIT;
#endif
    Dem_FFIteratorSetFreezeFrameFilterData(dcmClientId, lFFIteratorFilter);
}

/**
 * @brief Filters the next J1939 DTC
 */
DEM_LOCAL Dem_EventIdType Dem_FreezeFrameIteratorFindNextJ1939DTC(
    uint8                                ClientId,
    Dem_EventIdType                      LastEventId,
    Dem_J1939DcmSetFreezeFrameFilterType FreezeFrameKind)
{
    Dem_EventIdType eventId;
    Dem_EventIdType lastEvent =
        Dem_GetMemStateInfo()[Dem_GetPrimaryRefOfMemSet(Dem_GetMemorySetRefOfClient(ClientId))].LastEvent;

    for (eventId = LastEventId; eventId < lastEvent; eventId++)
    {
        Dem_DTCAttRefNumType lDTCAttr = Dem_GetDTCAttr(eventId);
        if (Dem_CheckDTCSuppressed(eventId) == TRUE)
        {
            /** Don't match suppressed events */
            continue; /* PRQA S 0770 */ /* VL_Dem_0770 */
        }
        if (Dem_DTCCheckStoredStatus(eventId) == FALSE)
        {
            /** only consider stored events */
            continue; /* PRQA S 0770 */ /* VL_Dem_0770 */
        }

        if (
#if (DEM_MAX_SIZE_J1939FREEZEFRAME > 0u)
            ((FreezeFrameKind == DEM_J1939DCM_FREEZEFRAME)
             && (Dem_GetJ1939FFRefOfDTCAttr(lDTCAttr) == DEM_ATTRI_INVALID_J1939FREEZEFRAME))
#endif
#if ((DEM_J1939_EXPANDED_FREEZE_FRAME_SUPPORT == STD_ON) && (DEM_J1939_FREEZE_FRAME_SUPPORT == STD_ON))
            ||
#endif
#if (DEM_MAX_SIZE_J1939EXFREEZEFRAME > 0u)
            ((FreezeFrameKind == DEM_J1939DCM_EXPANDED_FREEZEFRAME)
             && (Dem_GetJ1939EFFRefOfDTCAttr(lDTCAttr) == DEM_ATTRI_INVALID_J1939EXFREEZEFRAME))
            || ((FreezeFrameKind == DEM_J1939DCM_SPNS_IN_EXPANDED_FREEZEFRAME)
                && (Dem_GetJ1939EFFRefOfDTCAttr(lDTCAttr) == DEM_ATTRI_INVALID_J1939EXFREEZEFRAME))
#endif
        )
        {
            /** the event does not support the requested data (also: no J1939 DTC) */
            continue; /* PRQA S 0770 */ /* VL_Dem_0770 */
        }
        /** Found a candidate */
        break;
    }

    if (eventId >= lastEvent)
    {
        /** Did not find a candidate */
        eventId = DEM_EVENT_INVALID;
    }

    return eventId;
}

/**
 * @brief Get the next matching J1939 freeze frame record event.
 */
DEM_LOCAL boolean
    Dem_FreezeFrameIteratorGetNextJ1939event(uint8 ClientId, Dem_FFIteratorFilterType* FFIteratorFilterData)
{
    boolean copyData = FALSE;
    if (FFIteratorFilterData->FilterState == DEM_FREEZEFRAMEITERATOR_J1939STATE_PENDING)
    {
        /** process event again if still stored. This event is already the combined group event in case of
         * combination, so no aditional check in this context */
        if (Dem_DTCCheckStoredStatus(FFIteratorFilterData->EventId) == TRUE)
        {
            copyData = TRUE;
        }
        else
        {
            /** proceed with next EventId */
            FFIteratorFilterData->EventId     = FFIteratorFilterData->EventId + 1u;
            FFIteratorFilterData->FilterState = DEM_FREEZEFRAMEITERATOR_J1939STATE_USED;
        }
    }

    if (FFIteratorFilterData->FilterState == DEM_FREEZEFRAMEITERATOR_J1939STATE_USED)
    {
        /** Search the next/first J1939DTC which is currently stored */
        FFIteratorFilterData->EventId = Dem_FreezeFrameIteratorFindNextJ1939DTC(
            ClientId,
            FFIteratorFilterData->EventId,
            FFIteratorFilterData->FreezeFrameKind);

        if (FFIteratorFilterData->EventId == DEM_EVENT_INVALID)
        {
            /** primary EventId range exceeded, finish processing */
            FFIteratorFilterData->EventId     = DEM_EVENT_INVALID;
            FFIteratorFilterData->FilterState = DEM_FREEZEFRAMEITERATOR_J1939STATE_USED;
        }
        else
        {
            copyData = TRUE;
        }
    }
    return copyData;
}

/**
 * @brief Get the next matching J1939 freeze frame record.
 */
DEM_LOCAL Std_ReturnType Dem_FreezeFrameIteratorGetNextJ1939FreezeFrame(
    uint8   ClientId,
    uint32* J1939DTC,
    uint8*  OccurrenceCounter,
    uint8*  DestBuffer,
    uint16* BufSize)
{
    Std_ReturnType           ret                   = DEM_NO_SUCH_ELEMENT;
    uint8                    dcmClientId           = Dem_GetJ1939DcmClientIdOfClient(ClientId) + DEM_DCM_CLIENT_NUMBER;
    Dem_FFIteratorFilterType lFFIteratorFilterData = Dem_FFIteratorGetFreezeFrameFilterData(dcmClientId);

    if (lFFIteratorFilterData.EventId != DEM_EVENT_INVALID)
    {
        if (Dem_FreezeFrameIteratorGetNextJ1939event(ClientId, &lFFIteratorFilterData) == TRUE)
        {
            Dem_MemStateInfoConstPtrType memoryInfo =
                Dem_MemStateInfoInit(Dem_GetPrimaryRefOfMemSet(Dem_GetMemorySetRefOfClient(ClientId)));
            Dem_NvBlockNumType memoryIndex = Dem_MemoryFindIndex(memoryInfo, lFFIteratorFilterData.EventId);
            if (memoryIndex != DEM_MEM_INVALID_MEMORY_INDEX)
            {
                boolean updateState          = Dem_MemoryUpdateGetState(memoryIndex);
                boolean modificationDetected = FALSE;
                /** check for concurrent access to the memory entry */
                if (updateState == FALSE)
                {
                    const uint8*          freezeFramePtr;
                    uint16                size     = 0u;
                    Dem_DTCAttRefNumType  lDTCAttr = Dem_GetDTCAttr(lFFIteratorFilterData.EventId);
                    Dem_J1939FFRefNumType J1939FFRef;

                    /** set the source buffer offset and the size to copy */
                    if (lFFIteratorFilterData.FreezeFrameKind == DEM_J1939DCM_FREEZEFRAME)
                    {
#if (DEM_MAX_SIZE_J1939FREEZEFRAME > 0u)
                        J1939FFRef = Dem_GetJ1939FFRefOfDTCAttr(lDTCAttr);
                        size =
                            (J1939FFRef != DEM_ATTRI_INVALID_J1939FREEZEFRAME) ? Dem_GetSizeOfJ1939FF(J1939FFRef) : 0u;
                        freezeFramePtr = Dem_GetJ1939FreezeFramePtrOfMemEntry(memoryIndex);
#endif
                    }
                    else
                    {
#if (DEM_MAX_SIZE_J1939EXFREEZEFRAME > 0u)
                        J1939FFRef = Dem_GetJ1939EFFRefOfDTCAttr(lDTCAttr);
                        size = (J1939FFRef != DEM_ATTRI_INVALID_J1939EXFREEZEFRAME) ? Dem_GetSizeOfJ1939FF(J1939FFRef)
                                                                                    : 0u;
                        freezeFramePtr = Dem_GetJ1939ExFreezeFramePtrOfMemEntry(memoryIndex);
#endif
                    }

                    if (*BufSize >= size)
                    {
                        uint8 lOccurrenceCounter = Dem_GetOCCOfMemEntry(memoryIndex);

                        Dem_MemCpy(DestBuffer, freezeFramePtr, size);
                        *BufSize  = size;
                        *J1939DTC = Dem_GetEventJ1939DTC(lFFIteratorFilterData.EventId);
                        /** Occurrence counter > 126 shall be reported as 126 */
                        *OccurrenceCounter = (lOccurrenceCounter > DEM_J1939_OC_MAX) ? (uint8)DEM_J1939_OC_MAX
                                                                                     : (uint8)lOccurrenceCounter;
                        if (updateState != Dem_MemoryUpdateGetState(memoryIndex))
                        {
                            /** concurrent access to memory entry during copy process */
                            modificationDetected = TRUE;
                        }
                    }
                    else
                    {
                        ret = DEM_BUFFER_TOO_SMALL;
                    }
                }
                else
                {
                    /** concurrent access detected */
                    modificationDetected = TRUE;
                }

                if (modificationDetected == TRUE)
                {
                    /** retry next call of this interface */
                    lFFIteratorFilterData.FilterState = DEM_FREEZEFRAMEITERATOR_J1939STATE_PENDING;
                    ret                               = DEM_PENDING;
                }
                else if (ret != DEM_BUFFER_TOO_SMALL)
                {
                    /** copy finished */
                    lFFIteratorFilterData.EventId     = lFFIteratorFilterData.EventId + 1u;
                    lFFIteratorFilterData.FilterState = DEM_FREEZEFRAMEITERATOR_J1939STATE_USED;
                    ret                               = E_OK;
                }
                else
                {
                    /** Return DEM_BUFFER_TOO_SMALL */
                }
            }
        }
        Dem_FFIteratorSetFreezeFrameFilterData(dcmClientId, lFFIteratorFilterData);
    }
    /** else processing finished ret = DEM_NO_SUCH_ELEMENT initially set */
    return ret;
}

#if (DEM_MAX_SIZE_J1939EXFREEZEFRAME > 0u)
/**
 * @brief Get the next matching SPN.
 */
DEM_LOCAL void Dem_FreezeFrameIteratorGetNextSPN(Dem_FFIteratorFilterType* FFIteratorFilterData)
{
    Dem_FFIteratorFilterType* lFFIteratorFilterData = FFIteratorFilterData;
    Dem_FFRefNumType          lJ1939ExFFRef = Dem_GetJ1939ExFFRef(Dem_GetDTCAttr(lFFIteratorFilterData->EventId));
    Dem_SPNCRefNumType        lSPNNum =
        Dem_GetSPNCRefEndOfJ1939FF(lJ1939ExFFRef) - Dem_GetSPNCRefStartOfJ1939FF(lJ1939ExFFRef);

    lFFIteratorFilterData->SPNIter++;
    if (lFFIteratorFilterData->SPNIter >= lSPNNum)
    {
        /** proceed with next EventId */
        lFFIteratorFilterData->SPNIter     = 0u;
        lFFIteratorFilterData->EventId     = lFFIteratorFilterData->EventId + 1u;
        lFFIteratorFilterData->FilterState = DEM_FREEZEFRAMEITERATOR_J1939STATE_USED;
    }
}

/**
 * @brief Get the next matching J1939 freeze frame record event.
 */
DEM_LOCAL boolean Dem_FreezeFrameIteratorGetNextEvent(uint8 ClientId, Dem_FFIteratorFilterType* FFIteratorFilterData)
{
    boolean copyData = FALSE;
    if (FFIteratorFilterData->FilterState == DEM_FREEZEFRAMEITERATOR_J1939STATE_PENDING)
    {
        /** process event again if still stored. This event is already the combined group event in case of
         * combination, so no aditional check in this context */
        if (Dem_DTCCheckStoredStatus(FFIteratorFilterData->EventId) == TRUE)
        {
            copyData = TRUE;
        }
        else
        {
            /** proceed with next EventId */
            Dem_FreezeFrameIteratorGetNextSPN(FFIteratorFilterData);
        }
    }

    if (FFIteratorFilterData->FilterState == DEM_FREEZEFRAMEITERATOR_J1939STATE_USED)
    {
        /** Search the next/first J1939DTC which is currently stored */
        FFIteratorFilterData->EventId = Dem_FreezeFrameIteratorFindNextJ1939DTC(
            ClientId,
            FFIteratorFilterData->EventId,
            FFIteratorFilterData->FreezeFrameKind);

        if (FFIteratorFilterData->EventId == DEM_EVENT_INVALID)
        {
            /** primary EventId range exceeded, finish processing */
            FFIteratorFilterData->EventId     = DEM_EVENT_INVALID;
            FFIteratorFilterData->FilterState = DEM_FREEZEFRAMEITERATOR_J1939STATE_USED;
        }
        else
        {
            copyData = TRUE;
        }
    }
    return copyData;
}

/**
 * @brief Get the next matching J1939 freeze frame record.
 */
DEM_LOCAL Std_ReturnType
    Dem_FreezeFrameIteratorGetNextSPNInFreezeFrame(uint8 ClientId, uint32* SPNSupported, uint8* SPNDataLength)
{
    Std_ReturnType           ret                   = DEM_NO_SUCH_ELEMENT;
    uint8                    dcmClientId           = Dem_GetJ1939DcmClientIdOfClient(ClientId) + DEM_DCM_CLIENT_NUMBER;
    Dem_FFIteratorFilterType lFFIteratorFilterData = Dem_FFIteratorGetFreezeFrameFilterData(dcmClientId);

    if (lFFIteratorFilterData.EventId != DEM_EVENT_INVALID)
    {
        if (Dem_FreezeFrameIteratorGetNextEvent(ClientId, &lFFIteratorFilterData) == TRUE)
        {
            Dem_MemStateInfoConstPtrType memoryInfo =
                Dem_MemStateInfoInit(Dem_GetPrimaryRefOfMemSet(Dem_GetMemorySetRefOfClient(ClientId)));
            Dem_NvBlockNumType memoryIndex = Dem_MemoryFindIndex(memoryInfo, lFFIteratorFilterData.EventId);
            if (memoryIndex != DEM_MEM_INVALID_MEMORY_INDEX)
            {
                boolean updateState          = Dem_MemoryUpdateGetState(memoryIndex);
                boolean modificationDetected = FALSE;
                /** check for concurrent access to the memory entry */
                if (updateState == FALSE)
                {
                    Dem_FFRefNumType lJ1939ExFFRef = Dem_GetJ1939ExFFRef(Dem_GetDTCAttr(lFFIteratorFilterData.EventId));
                    Dem_SPNCRefNumType lSPNIndex =
                        Dem_GetSPNCRefStartOfJ1939FF(lJ1939ExFFRef) + lFFIteratorFilterData.SPNIter;

                    /** set the source buffer offset and the size to copy */
                    *SPNSupported  = Dem_GetIDOfSPNC(lSPNIndex);
                    *SPNDataLength = Dem_GetSizeOfSPNC(lSPNIndex);
                    if (updateState != Dem_MemoryUpdateGetState(memoryIndex))
                    {
                        /** concurrent access to memory entry during copy process */
                        modificationDetected = TRUE;
                    }
                }
                else
                {
                    /** concurrent access detected */
                    modificationDetected = TRUE;
                }

                if (modificationDetected == TRUE)
                {
                    /** retry next call of this interface */
                    lFFIteratorFilterData.FilterState = DEM_FREEZEFRAMEITERATOR_J1939STATE_PENDING;
                    ret                               = DEM_PENDING;
                }
                else
                {
                    /** copy finished */
                    Dem_FreezeFrameIteratorGetNextSPN(&lFFIteratorFilterData);
                    ret = E_OK;
                }
            }
        }
        Dem_FFIteratorSetFreezeFrameFilterData(dcmClientId, lFFIteratorFilterData);
    }
    /** else processing finished ret = DEM_NO_SUCH_ELEMENT initially set */
    return ret;
}
#endif
#endif

#if (DEM_J1939_READINESS1_SUPPORT == STD_ON)
/**
 * @brief Get the diagnostic readiness for the given client
 */
DEM_LOCAL void
    Dem_ClientJ1939DcmReadDiagnosticReadiness1(Dem_J1939DcmDiagnosticReadiness1Type* DataValue, uint8 ClientId)
{
    uint8                        dcmClientId = Dem_GetJ1939DcmClientIdOfClient(ClientId) + DEM_DCM_CLIENT_NUMBER;
    Dem_FilterDataInfoType       filterData  = Dem_GetFilterData(dcmClientId);
    uint8                        numberOfFilteredDTC;
    Dem_MemStateInfoConstPtrType memStateInfo =
        &Dem_GetMemStateInfo()[Dem_GetPrimaryRefOfMemSet(Dem_GetMemorySetRefOfClient(ClientId))];
    Dem_EventIdType lastEvent  = memStateInfo->LastEvent;
    Dem_EventIdType firstEvent = memStateInfo->FirstEvent;
#if (DEM_OBDII_SUPPORT == STD_ON)
    DataValue->OBDCompliance = Dem_GetGeneralOBD().OBDCompliancy;
#else
    DataValue->OBDCompliance = DEM_J1939_OBD_COMPLIANCY_INVALID;
#endif
    DataValue->ContinuouslyMonitoredSystemsSupport_Status = 0u;
    DataValue->NonContinuouslyMonitoredSystemsSupport5    = 0u;
    DataValue->NonContinuouslyMonitoredSystemsSupport6    = 0u;
    DataValue->NonContinuouslyMonitoredSystemsStatus7     = 0u;
    DataValue->NonContinuouslyMonitoredSystemsStatus8     = 0u;
    /** set DTC filter for active DTCs */
    filterData.DTCStatusFilterType = DEM_FILTERDATA_J1939FILTER_ACTIVE;
    /** get number of active DTCs */
    numberOfFilteredDTC = 0u;
    /* PRQA S 3473 ++ */ /* VL_Dem_3473 */
    DEM_FLAG_SET(filterData.FilterFlags, DEM_FILTERDATA_FILTERFLAGS_FILTERALLDTC);
    for (Dem_EventIdType eventId = firstEvent; eventId < lastEvent; ++eventId)
    {
        if (Dem_FilterDataCheckEventMatchesJ1939DTCFilter(
                filterData.DTCStatusFilterType,
                filterData.FilterFlags,
                eventId)
            == TRUE)
        {
            numberOfFilteredDTC++;
            if (numberOfFilteredDTC == DEM_J1939_READINESS_MAX_MATCH_NUMBER)
            {
                break;
            }
        }
    }
    DataValue->ActiveTroubleCodes = numberOfFilteredDTC;
    /** set DTC filter for previously active DTCs */
    filterData.DTCStatusFilterType = DEM_FILTERDATA_J1939FILTER_PREVIOUSLY_ACTIVE;
    /** get number of previously active DTCs */
    numberOfFilteredDTC = 0u;
    DEM_FLAG_SET(filterData.FilterFlags, DEM_FILTERDATA_FILTERFLAGS_FILTERALLDTC);
    /* PRQA S 3473 -- */
    for (Dem_EventIdType eventId = firstEvent; eventId < lastEvent; ++eventId)
    {
        if (Dem_FilterDataCheckEventMatchesJ1939DTCFilter(
                filterData.DTCStatusFilterType,
                filterData.FilterFlags,
                eventId)
            == TRUE)
        {
            numberOfFilteredDTC++;
            if (numberOfFilteredDTC == DEM_J1939_READINESS_MAX_MATCH_NUMBER)
            {
                break;
            }
        }
    }
    DataValue->PreviouslyActiveDiagnosticTroubleCodes = numberOfFilteredDTC;
    Dem_SetFilterData(dcmClientId, filterData);
}
#endif

/* ========================================== external function definitions ========================================= */
/* PRQA S 3406, 1503, 1252 ++ */ /* VL_Dem_3406, VL_QAC_NoUsedApi, VL_Dem_1252 */
/* PRQA S 1532 ++ */             /* VL_QAC_OneFunRef */
#if (DEM_J1939_READING_DTC_SUPPORT == STD_ON)
/**
 * @brief The function sets the DTC filter for a specific node and returns the composite lamp status of the filtered
 * DTCs.
 */
/* PRQA S 6030 ++ */ /* VL_MTR_Dem_STMIF */
Std_ReturnType Dem_J1939DcmSetDTCFilter(
    Dem_J1939DcmDTCStatusFilterType DTCStatusFilter,
    Dem_DTCKindType                 DTCKind,
    Dem_DTCOriginType               DTCOrigin,
    uint8                           ClientId,
    Dem_J1939DcmLampStatusType*     LampStatus)
{
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckJ1939DcmClientIdValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
    else if (DTCStatusFilter > DEM_J1939DTC_FILTER_INVALID)
    {
        errorId = DEM_E_PARAM_DATA;
    }
    else if (DTCKind > DEM_DTC_KIND_INVALID)
    {
        errorId = DEM_E_PARAM_DATA;
    }
    else if (Dem_DTCOriginCheckValid(DTCOrigin) == FALSE)
    {
        errorId = DEM_E_PARAM_DATA;
    }
    else if (LampStatus == NULL_PTR)
    {
        errorId = DEM_E_PARAM_POINTER;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        ret = Dem_ClientJ1939DcmSetDTCFilter(DTCStatusFilter, DTCKind, DTCOrigin, ClientId, LampStatus);
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_J1939DCMSETDTCFILTER, errorId);
    }
#endif
    return ret;
}
/* PRQA S 6030 -- */

/**
 * @brief Gets the number of currently filtered DTCs set by the function Dem_J1939DcmSetDTCFilter.
 */
Std_ReturnType Dem_J1939DcmGetNumberOfFilteredDTC(uint16* NumberOfFilteredDTC, uint8 ClientId)
{
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckJ1939DcmClientIdValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
    else if (NumberOfFilteredDTC == NULL_PTR)
    {
        errorId = DEM_E_PARAM_POINTER;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else if (Dem_ClientCheckDTCFilterSet(ClientId, DEM_FILTERDATA_FILTER_J1939DTC) == FALSE)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
    else
#endif
    {
        *NumberOfFilteredDTC = Dem_ClientJ1939DcmGetNumberOfFilteredDTC(ClientId);
        ret                  = E_OK;
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_J1939DCMGETNUMBEROFFILTEREDDTC, errorId);
    }
#endif
    return ret;
}

/**
 * @brief Gets the next filtered J1939 DTC.
 */
Std_ReturnType Dem_J1939DcmGetNextFilteredDTC(uint32* J1939DTC, uint8* OccurenceCounter, uint8 ClientId)
{
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckJ1939DcmClientIdValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
    else if ((J1939DTC == NULL_PTR) || (OccurenceCounter == NULL_PTR))
    {
        errorId = DEM_E_PARAM_POINTER;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else if (Dem_ClientCheckDTCFilterSet(ClientId, DEM_FILTERDATA_FILTER_J1939DTC) == FALSE)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
    else
#endif
    {
        ret = Dem_ClientJ1939DcmGetNextFilteredDTC(ClientId, J1939DTC, OccurenceCounter);
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_J1939DCMGETNEXTOFFILTEREDDTC, errorId);
    }
#endif
    return ret;
}
#endif

#if ((DEM_J1939_READING_DTC_SUPPORT == STD_ON) || (DEM_J1939_DM31_SUPPORT == STD_ON))
/**
 * @brief The function sets the filter to the first applicable DTC for the DM31 response for a specific node.
 */
void Dem_J1939DcmFirstDTCwithLampStatus(uint8 ClientId)
{
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckJ1939DcmClientIdValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        Dem_ClientJ1939DcmFirstDTCwithLampStatus(ClientId);
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_J1939DCMFIRSTDTCWITHLAMPSTATUS, errorId);
    }
#endif
}

/**
 * @brief Gets the next filtered J1939 DTC for DM31 including current LampStatus.
 */
Std_ReturnType Dem_J1939DcmGetNextDTCwithLampStatus(
    Dem_J1939DcmLampStatusType* LampStatus,
    uint32*                     J1939DTC,
    uint8*                      OccurenceCounter,
    uint8                       ClientId)
{
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckJ1939DcmClientIdValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
    else if ((LampStatus == NULL_PTR) || (J1939DTC == NULL_PTR) || (OccurenceCounter == NULL_PTR))
    {
        errorId = DEM_E_PARAM_POINTER;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else if (Dem_ClientCheckDTCFilterSet(ClientId, DEM_FILTERDATA_FILTER_J1939DTCLAMP) == FALSE)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
    else
#endif
    {
        Dem_FilterDataResultType filterResult;
        ret               = Dem_ClientJ1939DcmGetNextDTCwithLampStatus(ClientId, &filterResult);
        *J1939DTC         = filterResult.DTC;
        *OccurenceCounter = filterResult.OccurrenceCounter;
        *LampStatus       = filterResult.LampStatus;
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_J1939DCMGETNEXTDTCWITHLAMPSTATUS, errorId);
    }
#endif
    return ret;
}
#endif

#if (DEM_J1939_CLEAR_DTC_SUPPORT == STD_ON)
/**
 * @brief Clears the status of all event(s) related to the specified DTC(s), as well as all associated event memory
 * entries for these event(s).
 */
Std_ReturnType
    Dem_J1939DcmClearDTC(Dem_J1939DcmSetClearFilterType DTCTypeFilter, Dem_DTCOriginType DTCOrigin, uint8 ClientId)
{
    Std_ReturnType ret = DEM_CLEAR_FAILED;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckValid(ClientId) == FALSE)
    {
        errorId = DEM_E_PARAM_DATA;
    }
    else if (DTCTypeFilter > DEM_J1939DTC_CLEAR_INVALID)
    {
        errorId = DEM_E_PARAM_DATA;
    }
    else if (Dem_DTCOriginCheckValid(DTCOrigin) == FALSE)
    {
        errorId = DEM_E_PARAM_DATA;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        ret = Dem_J1939ClearDTC(DTCTypeFilter, DTCOrigin, ClientId);
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_J1939DCMCLEARDTC, errorId);
    }
#endif
    return ret;
}
#endif

#if ((DEM_J1939_EXPANDED_FREEZE_FRAME_SUPPORT == STD_ON) || (DEM_J1939_FREEZE_FRAME_SUPPORT == STD_ON))
/**
 * @brief The function sets the FreezeFrame filter for a specific node.
 */
Std_ReturnType Dem_J1939DcmSetFreezeFrameFilter(Dem_J1939DcmSetFreezeFrameFilterType FreezeFrameKind, uint8 ClientId)
{
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckJ1939DcmClientIdValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
    else if (FreezeFrameKind > DEM_J1939DTC_FFFILTER_INVALID)
    {
        errorId = DEM_E_PARAM_DATA;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        Dem_ClientJ1939DcmSetFreezeFrameFilter(ClientId, FreezeFrameKind);
        ret = E_OK;
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_J1939DCMSETFREEZEFRAMEFITER, errorId);
    }
#endif
    return ret;
}

/**
 * @brief Gets next freeze frame data. The function stores the data in the provided DestBuffer.
 */
Std_ReturnType Dem_J1939DcmGetNextFreezeFrame(
    uint32* J1939DTC,
    uint8*  OccurenceCounter,
    uint8*  DestBuffer,
    uint16* BufSize,
    uint8   ClientId)
{
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckJ1939DcmClientIdValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
    else if (
        (J1939DTC == NULL_PTR) || (OccurenceCounter == NULL_PTR) || (DestBuffer == NULL_PTR) || (BufSize == NULL_PTR))
    {
        errorId = DEM_E_PARAM_POINTER;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else if (Dem_ClientCheckJ1939NextFreezeFrameFilterSet(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
    else
#endif
    {
#if ((DEM_J1939_EXPANDED_FREEZE_FRAME_SUPPORT == STD_ON) || (DEM_J1939_FREEZE_FRAME_SUPPORT == STD_ON))
        ret = Dem_FreezeFrameIteratorGetNextJ1939FreezeFrame(ClientId, J1939DTC, OccurenceCounter, DestBuffer, BufSize);
#else
        ret = DEM_NO_SUCH_ELEMENT;
#endif
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_J1939DCMGETNEXTFREEZEFRAME, errorId);
    }
#endif
    return ret;
}

/**
 * @brief Gets next SPN.
 */
Std_ReturnType Dem_J1939DcmGetNextSPNInFreezeFrame(uint32* SPNSupported, uint8* SPNDataLength, uint8 ClientId)
{
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckJ1939DcmClientIdValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
    else if ((SPNSupported == NULL_PTR) || (SPNDataLength == NULL_PTR))
    {
        errorId = DEM_E_PARAM_POINTER;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else if (Dem_ClientCheckJ1939SPNFreezeFrameFilterSet(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
    else
#endif
    {
#if (DEM_MAX_SIZE_J1939EXFREEZEFRAME > 0u)
        ret = Dem_FreezeFrameIteratorGetNextSPNInFreezeFrame(ClientId, SPNSupported, SPNDataLength);
#else
        ret = DEM_NO_SUCH_ELEMENT;
#endif
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_J1939DCMGETNEXTSPNINFREEZEFRAME, errorId);
    }
#endif
    return ret;
}
#endif

#if (DEM_J1939_RATIO_SUPPORT == STD_ON)
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
/**
 * @brief The function sets the Ratio filter for a specific node and returns the corresponding Ignition Cycle Counter
 * and General Denominator.
 */
Std_ReturnType
    Dem_J1939DcmSetRatioFilter(uint16* IgnitionCycleCounter, uint16* OBDMonitoringConditionsEncountered, uint8 ClientId)
{
    DEM_PARAM_UNUSED(IgnitionCycleCounter);
    DEM_PARAM_UNUSED(OBDMonitoringConditionsEncountered);
    DEM_PARAM_UNUSED(ClientId);
    return E_NOT_OK;
}

/**
 * @brief Gets the next filtered Ratio.
 */
Std_ReturnType Dem_J1939DcmGetNextFilteredRatio(uint32* SPN, uint16* Numerator, uint16* Denominator, uint8 ClientId)
{
    DEM_PARAM_UNUSED(SPN);
    DEM_PARAM_UNUSED(Numerator);
    DEM_PARAM_UNUSED(Denominator);
    DEM_PARAM_UNUSED(ClientId);
    return E_NOT_OK;
}
/* PRQA S 3673 -- */
#endif

#if (DEM_J1939_READINESS1_SUPPORT == STD_ON)
/**
 * @brief Service to report the value of Diagnostic Readiness 1 (DM05) computed by the Dem.
 */
Std_ReturnType Dem_J1939DcmReadDiagnosticReadiness1(Dem_J1939DcmDiagnosticReadiness1Type* DataValue, uint8 ClientId)
{
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckJ1939DcmClientIdValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
    else if (DataValue == NULL_PTR)
    {
        errorId = DEM_E_PARAM_POINTER;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        Dem_ClientJ1939DcmReadDiagnosticReadiness1(DataValue, ClientId);
        ret = E_OK;
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_J1939DCMREADDIAGNOSTICREADINESS1, errorId);
    }
#endif
    return ret;
}
#endif

#if (DEM_J1939_READINESS2_SUPPORT == STD_ON)
/**
 * @brief Service to report the value of Diagnostic Readiness 2 (DM21) computed by the Dem.
 */
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
Std_ReturnType Dem_J1939DcmReadDiagnosticReadiness2(Dem_J1939DcmDiagnosticReadiness2Type* DataValue, uint8 ClientId)
/* PRQA S 3673 -- */
{
    DEM_PARAM_UNUSED(DataValue);
    DEM_PARAM_UNUSED(ClientId);
    return E_NOT_OK;
}
#endif

#if (DEM_J1939_READINESS3_SUPPORT == STD_ON)
/**
 * @brief Service to report the value of Diagnostic Readiness 3 (DM26) computed by the Dem.
 */
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
Std_ReturnType Dem_J1939DcmReadDiagnosticReadiness3(Dem_J1939DcmDiagnosticReadiness3Type* DataValue, uint8 ClientId)
/* PRQA S 3673 -- */
{
    DEM_PARAM_UNUSED(DataValue);
    DEM_PARAM_UNUSED(ClientId);
    return E_NOT_OK;
}
#endif
/* PRQA S 1532 -- */
/* PRQA S 3406, 1503, 1252 -- */

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif

#endif
