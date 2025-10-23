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
 **  @file               : Dem_Client.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : Implementation file of Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_CLIENT_H
#define DEM_CLIENT_H

/* =================================================== inclusions =================================================== */
#include "Dem_EventInterface.h"
#include "Dem_DtcInterface.h"
#include "Dem_MemInterface.h"
#include "Dem_FreezeFrame.h"
#include "Dem_Indicator.h"
#include "Dem_ExtendedData.h"
#include "Dem_Error.h"

/* ===================================================== macros ===================================================== */
/**
 * @name DEM_DTC_ORIGIN
 * @{
 * @brief Identification of DTC origin type
 */
#define DEM_DTC_ORIGIN_ZERO_INVALID     (0x0u)    /**< invaild value 0 of DTCOrigin */
#define DEM_DTC_ORIGIN_THREE_INVALID    (0x3u)    /**< invaild value 3 of DTCOrigin */
#define DEM_DTC_ORIGIN_USER_MIN_INVALID (0x00FFu) /**< min invaild value of DTCOrigin */
#define DEM_DTC_ORIGIN_USER_MAX_INVALID (0x0200u) /**< max invaild value of DTCOrigin */
/** @} */

/**
 * @name DEM_ENCODING_INVALID_VALUE
 * @{
 * @brief Macros encoding invalid values for API parameter verification
 */
#define DEM_DTC_MAX_VALUE ((uint32)(0x00FFFFFFu)) /**< Largest 'DTC' value for any format */
#if (DEM_SUPPORTED_OBD_UDS_DTC_SEPARATION == STD_OFF)
#define DEM_DTC_FORMAT_INVALID DEM_DTC_FORMAT_J1939 /**< Last vaild value of Dem_DTCFormatType */
#else
#define DEM_DTC_FORMAT_INVALID DEM_DTC_FORMAT_OBD_3BYTE /**< Last vaild value of Dem_DTCFormatType */
#endif
#define DEM_DTC_KIND_INVALID        DEM_DTC_KIND_EMISSION_REL_DTCS /**< Last vaild value of Dem_DTCKindType */
#define DEM_DTC_REQUEST_FIRST_VALID DEM_FIRST_FAILED_DTC           /**< First vaild value of Dem_DTCRequestType */
#define DEM_DTC_REQUEST_INVALID     DEM_MOST_REC_DET_CONFIRMED_DTC /**< Last vaild value of Dem_DTCRequestType */
#define DEM_J1939DTC_CLEAR_INVALID \
    DEM_J1939DTC_CLEAR_ACTIVE_AND_PREVIOUSLY_ACTIVE /**< Last vaild value of Dem_J1939DcmSetClearFilterType */
#define DEM_J1939DTC_FILTER_INVALID \
    DEM_J1939DTC_CURRENTLY_ACTIVE /**< Last vaild value of Dem_J1939DcmDTCStatusFilterType */
#define DEM_J1939DTC_FFFILTER_INVALID \
    DEM_J1939DCM_SPNS_IN_EXPANDED_FREEZEFRAME /**< Last vaild value of Dem_J1939DcmSetFreezeFrameFilterType */
/** @} */

/**
 * @name DEM_READOUTBUFFER_STATUS
 * @{
 * @brief Initialization value, buffer is unused and contains invalid data
 */
#define DEM_READOUTBUFFER_STATUS_UNUSED                                           \
    0u /**< Buffer contains configuration - waiting MainFunction to copy the data \
        */
#define DEM_READOUTBUFFER_STATUS_QUEUED     1u /**< Buffer contains configuration - buffer is queued */
#define DEM_READOUTBUFFER_STATUS_IN_USE     2u /**< Buffer contains configuration and event data */
#define DEM_READOUTBUFFER_STATUS_NOT_STORED 3u /**< Buffer contains configuration - no data found to copy */
/** @} */

/**
 * @name DEM_DTC_SETTING
 * @{
 * @brief Identification of DTC setting type
 */
#define DEM_DTCSETTING_ACTIVE          (0x1u)  /**< the active state of an DTCSETTING */
#define DEM_DTCSETTING_QUEUE_DISABLE   (0x2u)  /**< the 'queued disable' state of an DTCSETTING */
#define DEM_DTCSETTING_PROCESS_DISABLE (0x4u)  /**< the 'process disable' state of an DTCSETTING */
#define DEM_DTCSETTING_QUEUE_ENABLE    (0x8u)  /**< the 'queued enable' state of an DTCSETTING */
#define DEM_DTCSETTING_PROCESS_ENABLE  (0x10u) /**< the 'process enable' state of an DTCSETTING */
/** @} */

#define DEM_USERDEF_MASK (0xFFu) /**< user def memory mask */

/**
 * @name DEM_RECORD_NUMBER
 * @{
 * @brief Identification of record number type
 */
#define DEM_RECORD_NUMBER_ALL     (0xFFu) /**< recordNumber all */
#define DEM_RECORD_NUMBER_OBDALL  (0xFEu) /**< recordNumber OBD all */
#define DEM_RECORD_NUMBER_SPECIAL (0xF0u) /**< special recordNumber */
/** @} */

#define DEM_BYTE_SIZE (8u) /**< one byte size */

#define DEM_UDS_STATUS_CDTC_PDTC (0x0Cu) /**< bit 2: PendingDTC and bit 3: ConfirmedDTC */

/* ================================================ type definitions ================================================ */
/**
 * @brief Type to encode the state of a clear operation
 */
typedef uint8 Dem_ClearDTCStatusType;
#define DEM_CLEARDTC_STATUS_IDLE     0u /**< No clear operation in progress */
#define DEM_CLEARDTC_STATUS_PENDING  1u /**< Currently clearing of DTCs is in progress */
#define DEM_CLEARDTC_STATUS_DONE     2u /**< ClearDTC has finished sucessfully */
#define DEM_CLEARDTC_STATUS_FAILED   3u /**< ClearDTC has not finished sucessfully */
#define DEM_CLEARDTC_STATUS_NV_ERROR 4u /**< NV write error during ClearDTC */

/**
 * @brief This type is used to encode the result of a DTC selection
 */
typedef uint8 Dem_ClientResultType;
#define DEM_CLIENT_RESULT_INIT         0u /**< Initialization value */
#define DEM_CLIENT_RESULT_DTC          1u /**< A DTC was selected */
#define DEM_CLIENT_RESULT_GROUP        2u /**< A DTC group was selected */
#define DEM_CLIENT_RESULT_ALL_DTCS     3u /**< All DTCs were selected */
#define DEM_CLIENT_RESULT_NO_MATCH     4u /**< Selected number does not map to a DTC or DTC group */
#define DEM_CLIENT_RESULT_WRONG_ORIGIN 5u /**< DTC Origin not supported */

/**
 * @brief This type is used to encode the selected DTC group
 */
typedef uint8 Dem_ClientGroupIdType;
#define DEM_CLIENT_GROUP_INVALID          0xFFu /**< No group */
#define DEM_CLIENT_GROUP_ALL_DTCS         1u    /**< UDS group All DTCs */
#define DEM_CLIENT_GROUP_EMISSION_RELATED 2u    /**< OBD group All DTCs */
#define DEM_CLIENT_GROUP_WWH_OBD_ALL_DTC  3u    /**< WWH-OBD group All DTCs */
#define DEM_CLIENT_GROUP_ALL_J1939_DTCS   4u    /**< J1939 group All DTCs */

/**
 * @brief Type of used filter
 */
typedef uint8 Dem_FilterDataFilterType;
#define DEM_FILTERDATA_FILTER_INIT         0u /**< Initialization value */
#define DEM_FILTERDATA_FILTER_DTC          1u /**< A filter for DTCs was set up */
#define DEM_FILTERDATA_FILTER_J1939DTCLAMP 2u /**< A filter for J1939 DTCs with lamp status was set up */
#define DEM_FILTERDATA_FILTER_J1939DTC     3u /**< A filter for J1939 DTCs was set up */

/**
 * @brief FilterFlags: Macros encoding the filter flags of a filter.
 */
#define DEM_FILTERDATA_FILTERFLAGS_USESTATUSMASK        (0x01u) /**< Status mask is used for filtering */
#define DEM_FILTERDATA_FILTERFLAGS_USEFDC               (0x02u) /**< FDC is used for filtering */
#define DEM_FILTERDATA_FILTERFLAGS_USESEVERITY          (0x04u) /**< Severity is used for filtering */
#define DEM_FILTERDATA_FILTERFLAGS_USEFORMATOBD         (0x08u) /**< DTCFormat OBD is used for filtering */
#define DEM_FILTERDATA_FILTERFLAGS_FILTEREMISSIONRELDTC (0x10u) /**< Filtering for emsission relevant DTCs */
#define DEM_FILTERDATA_FILTERFLAGS_FILTERALLDTC         (0x20u) /**< Filtering for all DTCs */

/**
 * @brief Storage for the original selection request
 */
typedef struct Dem_ClientRequestTag /* PRQA S 1536 */ /* VL_Dem_1536 */
{
    uint32            DTC;       /**< Selected DTC value @range 0..0xFFFFFFFFu */
    Dem_DTCFormatType DTCFormat; /**< Selected DTC format @range 0..3 */
    Dem_DTCOriginType DTCOrigin; /**< Selected DTC origin @range 0..3 */
} Dem_ClientRequestType;

/**
 * @brief Storage for selection data
 */
typedef union Dem_ClientSelectionTag /* PRQA S 1536 */ /* VL_Dem_1536 */
{ /* PRQA S 0750 */                                    /* VL_Dem_0750 */
    Dem_EventIdType       EventId;                     /**< Selected EventId @range 0..0xFFFFu */
    Dem_ClientGroupIdType GroupId;                     /**< Selected GroupId @range 0..0xFFu */
#if (DEM_DTC_GROUP_NUMBER > 0u)
    Dem_GroupOfDTCNumType GroupDTCIndex; /**< Selected GroupDTCIndex @range 0..0xFFFFu */
#endif
} Dem_ClientSelectionType;

/**
 * @brief Storage for the Client request
 */
typedef struct Dem_ClientInfoTag /* PRQA S 1536 */ /* VL_Dem_1536 */
{
    Dem_ClearDTCStatusType  ClearDTCStatus; /**< The status of the clear operation @range 0..0xFFu */
    Dem_ClientRequestType   Request;        /**< Selection Request @range 0..0xFFFFFFFFu */
    Dem_ClientSelectionType Selection;      /**< Selection data @range 0..0xFFFFu */
    Dem_MemoryNumType       MemIndex;       /**< Memory the event or group belongs to @range 0..0xFFu */
    Dem_ClientResultType    ClientResult;   /**< DTC Selection Result @range 0..0xFFu */
    uint8 ActiveAsyncOps; /**< Number of currently active asynchronous operation that require a Dem_SelectDTC()
                             @range 0..0xFFu */
} Dem_ClientInfoType;

#if ((DEM_DCM_CLIENT_NUMBER + DEM_J1939DCM_CLIENT_NUMBER) > 0u)
/**
 * @brief Filtering strategy for DTC filtering
 */
typedef enum
{
    Dem_FilterDataInvalid,                         /**< Invalid filtering @range 0u */
    Dem_FilterDataFilterChronological,             /**< Chronological filtering @range 1u */
    Dem_FilterDataFilterEventBased,                /**< Non-ordered filtering @range 2u */
    Dem_FilterDataFilterEventBasedPermanentMemory, /**< Non-ordered filtering in permanent memory @range 3u */
} Dem_FilterDataDTCFilteringStrategyType;

/**
 * @brief Filter data for Dcm requests
 */
typedef struct Dem_FilterDataInfoTag /* PRQA S 1536 */ /* VL_Dem_1536 */
{
    union IteratorTag
    { /* PRQA S 0750 */                 /* VL_Dem_0750 */
        Dem_EventIdType EventIndex;     /**< Event iterator for linar order filtering @range 0..0xFFFFu */
        uint8           ChronoIndex;    /**< chronology iterator for chronological order filtering @range 0..0xFFu */
        uint8           PermanentIndex; /**< permanent entry iterator for permanent filtering @range 0..0xFFu */
    } Iterator;                         /**< Iterator data @range NA */
    uint8               StatusMask;     /**< Status mask filter @range 0..0xFFu */
    sint8               FaultDetectionCounter; /**< FaultDetectionCounter for FDC filter @range 0..0xFFu */
    Dem_MemoryNumType   MemIndex;              /**< Filtered event memory @range 0..0xFFu */
    Dem_DTCSeverityType SeverityMask;          /**< Severity mask filter @range 0..0xFFu */
    uint8               FilterFlags;           /**< Filter flags @range 0..0xFFu */
    Dem_FilterDataDTCFilteringStrategyType FilterDtcStrategy;   /**< Active DTC filtering strategy @range 0..0xFFu */
    Dem_FilterDataFilterType               FilterState;         /**< Filter state @range 0..0xFFu */
    Dem_J1939DcmDTCStatusFilterType        DTCStatusFilterType; /**< J1939Dcm DTC status filter type @range 0..0xFFu */
} Dem_FilterDataInfoType;

/**
 * @brief Filter data result type
 */
typedef struct Dem_FilterDataResultTag /* PRQA S 1536 */ /* VL_Dem_1536 */
{
    uint32                     DTC;               /**< DTC value @range 0..0xFFFFFFFFu */
    sint8                      FDC;               /**< FaultDetectionCounter @range 0..0xFFu */
    uint8                      DTCStatus;         /**< Status @range 0..0xFFu */
    Dem_DTCSeverityType        Severity;          /**< Severity @range 0..0xFFu */
    uint8                      FunctionalUnit;    /**< FunctionalUnit @range 0..0xFFu */
    uint8                      OccurrenceCounter; /**< OccurrenceCounter @range 0..0xFFu */
    Dem_J1939DcmLampStatusType LampStatus;        /**< LampStatus @range 0..0xFFu */
} Dem_FilterDataResultType;

/* PRQA S 5004 ++ */ /* VL_Dem_5004 */
/**
 * @brief         Function pointer type for event filter functions
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module
 * @param[in]     MemoryInfo: Record memory information
 * @return        Dem_EventIdType: Identification of an event by assigned EventId
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
typedef Dem_EventIdType (*Dem_FilterDataFuncPtrType)(uint8 ClientId, const Dem_MemStateInfoType* MemoryInfo);
/* PRQA S 5004 -- */

#if (DEM_CLIENT_REPORT_EVENT_SIZE > 0u)
/**
 * @brief Reported Events type
 */
typedef Dem_ComProcessorWordType Dem_ReportedEventsType[DEM_CLIENT_REPORT_EVENT_SIZE];
#endif

/* ========================================== internal function declarations ======================================== */
#if (DEM_USER_DEFINED_MEMORY_ENABLE == STD_ON)
/**
 * @brief         Checking User Defined Memory Origin Validity
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[in]     DTCOrigin: The event memory of the requested DTC or group of DTC.
 * @param[out]    MemIndex: Memory Index
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean
    Dem_DTCOriginUserDefinedCheckValid(uint8 ClientId, Dem_DTCOriginType DTCOrigin, Dem_MemoryNumType* MemIndex);
#endif

/**
 * @brief         Checking DTC Origin Validity in Client
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[in]     DTCOrigin: The event memory of the requested DTC or group of DTC.
 * @param[out]    MemIndex: Memory Index
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean
    Dem_DTCOriginCheckValidOnClient(uint8 ClientId, Dem_DTCOriginType DTCOrigin, Dem_MemoryNumType* MemIndex);

/**
 * @brief         Checking if a DTC number encodes a specific DTC.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[in]     DTC: Defines the DTC in respective format that is selected. If the DTC fits to a DTC group
 *                     number, the DTC group is selected.
 * @param[in]     DTCFormat: Defines the input-format of the provided DTC value.
 * @param[in]     DTCOrigin: The event memory of the requested DTC or group of DTC.
 * @return        Dem_EventIdType: Identification of an event by assigned EventId.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Dem_EventIdType
    Dem_ClientCheckSingleDTC(uint8 ClientId, uint32 DTC, Dem_DTCFormatType DTCFormat, Dem_DTCOriginType DTCOrigin);

/**
 * @brief         Selects a DTC or DTC group as target for further operations.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[in]     DTC: Defines the DTC in respective format that is selected. If the DTC fits to a DTC group
 *                     number, the DTC group is selected.
 * @param[in]     DTCFormat: Defines the input-format of the provided DTC value.
 * @param[in]     DTCOrigin: The event memory of the requested DTC or group of DTC.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void
    Dem_ClientSelectDTC(uint8 ClientId, uint32 DTC, Dem_DTCFormatType DTCFormat, Dem_DTCOriginType DTCOrigin);

#if (DEM_DCM_CLIENT_NUMBER > 0u)
/**
 * @brief         Initialize memory entry.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_ClientReadoutBufferInit(uint8 ClientId);

/**
 * @brief         Check state of a ReadoutBuffer entry, save current request if it is new.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[in]     EventId: Event ID
 * @param[in]     MemIndex: Memory index
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType
    Dem_DTCReadoutBufferCheckAndSetState(uint8 ClientId, Dem_EventIdType EventId, Dem_MemoryNumType MemIndex);
#endif

/**
 * @brief         Provides information if the last call to Dem_SelectDTC has selected a valid DTC or group of DTCs.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[out]    ErrorId: Error code ID
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_ClientGetDTCSelectionResult(
    uint8 ClientId
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    ,
    uint8* ErrorId
#endif
);

/**
 * @brief         Provides information if the clear result of a selected valid DTC or group of DTCs.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[out]    ErrorId: Error code ID
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_ClientResultForClearDTCGetDTCSelection(
    uint8 ClientId
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    ,
    uint8* ErrorId
#endif
);

/**
 * @brief         Disables the event memory update of a specific DTC (only one at one time).
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[out]    ErrorId: Error code ID
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_ClientDisableDTCRecordUpdate(
    uint8 ClientId
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    ,
    uint8* ErrorId
#endif
);

#if (DEM_DCM_CLIENT_NUMBER > 0u)
/**
 * @brief         Release a previously prepared memory entry.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_ClientEnableDTCRecordUpdate(uint8 ClientId);

/**
 * @brief         Populate readout buffer with data.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_ReadoutBufferFillData(uint8 ClientId);

/**
 * @brief         Copy the memory entry contents into the ReadoutBuffer.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_FilterEventDataTask(void);

/**
 * @brief         Check the DTC Setting state of Event
 * @param[in]     EventId: Identification of an event by assigned EventId.
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_CheckEventDTCSettingState(Dem_EventIdType EventId);

/**
 * @brief         Enable the DTC setting State
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DTCSettingEnable(uint8 ClientId);

/**
 * @brief         Disable the DTC setting
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DTCSettingDisable(uint8 ClientId);

/**
 * @brief         Processes changes of DTC setting
 * @param[in]     EventId: Identification of an event by assigned EventId.
 * @param[in]     DcmClientId: Identification of a client.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_ProcessQueueEventDTCSetting(Dem_EventIdType EventId, uint8 DcmClientId);

/**
 * @brief         DTC setting Process.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DTCSettingProcess(uint8 ClientId);

/**
 * @brief         DTC setting Task.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DTCSettingTask(void);
#endif

#if (DEM_SUPPRESSION_SUPPORT == DEM_DTC_SUPPRESSION)
/**
 * @brief         Suppress event or release event suppression as requested by the client
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[in]     SuppressionStatus: This parameter specifies whether the respective DTC shall be disabled (TRUE) or
 * enabled (FALSE).
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_ClientSetDTCSuppression(uint8 ClientId, boolean SuppressionStatus);
#endif

#if (DEM_DCM_CLIENT_NUMBER > 0u)
/**
 * @brief         Gets the DTC Status availability mask of the selected fault memory.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[in]     DTCStatusMask: Status-byte mask for DTC status-byte filtering Values: 0x00: Autosar-specific
 *                               value to deactivate the status-byte filtering (different meaning than in ISO 14229-1)
 *                               to report all supported DTCs (used for service 0x19 subfunctions 0x0A/0x15)
 *                               0x01..0xFF: Status-byte mask according to ISO 14229-1 DTCStatusMask (handed over by
 *                               Dcm from service request directly) to filter for DTCs with at least one status bit
 *                               set matching this status-byte mask
 * @param[in]     DTCOrigin: This parameter is used to select the source memory the DTCs shall be read/cleared from.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_ClientGetDTCStatusAvailabilityMask(
    uint8                  ClientId,
    Dem_UdsStatusByteType* DTCStatusMask,
    Dem_DTCOriginType      DTCOrigin);

#if (DEM_DCM_CLIENT_NUMBER > 0u)
/**
 * @brief         Get the current Uds status of a selected DTC
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[out]    ErrorId: The error code ID
 * @param[out]    DTCStatus: This parameter receives the status information of the requested DTC. It follows the
 *                           format as defined in ISO14229-1 If the return value of the function call is other than
 *                           DEM_FILTERED_OK this parameter does not contain valid data.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_ClientGetStatusOfDTC(
    uint8  ClientId,
    uint8* DTCStatus
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    ,
    uint8* ErrorId
#endif
);

#if (DEM_FEATURE_DCM_0X19_0X09_ENABLE == STD_ON)
/**
 * @brief         Get the severity of a selected DTC
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[out]    ErrorId: The error code ID
 * @param[out]    DTCSeverity: This parameter contains the DTCSeverity according to ISO 14229-1.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_ClientGetSeverityOfDTC(
    uint8                ClientId,
    Dem_DTCSeverityType* DTCSeverity
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    ,
    uint8* ErrorId
#endif
);

/**
 * @brief         Get the functional unit of a selected DTC
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[out]    ErrorId: The error code ID
 * @param[out]    DTCFunctionalUnit: Functional unit value of this DTC
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_ClientGetFunctionalUnitOfDTC(
    uint8  ClientId,
    uint8* DTCFunctionalUnit
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    ,
    uint8* ErrorId
#endif
);
#endif

/**
 * @brief         Initialize a filter for a 'filter DTC' request.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[in]     DTCOrigin: The event memory of the requested DTC or group of DTC.
 * @param[in]     MemIndex: Filtered event memory
 * @param[out]    StatusMask: DTCStatusMask The value DTCStatusMask indicates the supported DTC status bits
 *                               from the Dem. All supported information is indicated by setting the corresponding
 *                               status bit to 1. See ISO14229-1.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void
    Dem_FilterDataInit(uint8 ClientId, Dem_DTCOriginType DTCOrigin, Dem_MemoryNumType MemIndex, uint8 StatusMask);

/**
 * @brief         Initialize a filter for a 'filter DTC' request.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[in]     DTCFormat: Defines the output-format of the requested DTC value.
 * @param[in]     DTCOrigin: The event memory of the requested DTC or group of DTC.
 * @param[in]     FilterWithSeverity: This flag defines whether severity information (ref. to parameter below)
 *                                    shall be used for filtering. This is to allow for coexistence of DTCs with and
 *                                    without severity information. TRUE: severity information is used for filtering.
 *                                    FALSE: severity information is not used for filtering.
 * @param[in]     DTCSeverityMask: Contains the DTCSeverityMask according to ISO14229-1.
 * @param[in]     FilterForFaultDetectionCounter: This flag defines whether the fault detection counter
 *                                                information shall be used for filtering. This is to allow for
 *                                                coexistence of DTCs with and without fault detection counter
 *                                                information. If fault detection counter information is filter
 *                                                criteria, only those DTCs with a fault detection counter value
 *                                                between 1 and 0x7E shall be reported. Remark: If the event does not
 *                                                use the debouncing inside Dem, then the Dem must request this
 *                                                information via GetFaultDetectionCounter. TRUE: fault detection
 *                                                counter is used for filtering. FALSE: fault detection counter is
 *                                                not used for filtering.
 * @param[out]    DTCStatusMask: DTCStatusMask The value DTCStatusMask indicates the supported DTC status bits
 *                               from the Dem. All supported information is indicated by setting the corresponding
 *                               status bit to 1. See ISO14229-1.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_ClientSetDTCFilter(
    uint8               ClientId,
    uint8               DTCStatusMask,
    Dem_DTCFormatType   DTCFormat,
    Dem_DTCOriginType   DTCOrigin,
    boolean             FilterWithSeverity,
    Dem_DTCSeverityType DTCSeverityMask,
    boolean             FilterForFaultDetectionCounter);

#if (DEM_OBDII_SUPPORT == STD_ON)
/**
 * @brief         Calculates the number of filtered DTCs for permanent memory.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @return        uint16
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL uint16 Dem_FilterDataCountMatchesInPermanentMemory(uint8 ClientId);
#endif

/**
 * @brief         Test whether a given EventId matches the filter.
 * @param[in]     DcmClientId: Identification of a client.
 * @param[in]     EventId: Identification of an event by assigned EventId.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_FilterDataFilterForEventMatch(uint8 DcmClientId, Dem_EventIdType EventId);

/**
 * @brief         Calculates the number of filtered events in a standard event memory.
 * @param[in]     DcmClientId: Identification of a client.
 * @return        uint16
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL uint16 Dem_FilterDataCountMatchesInEventMemory(uint8 DcmClientId);

/**
 * @brief         Fetch number of DTCs matching the client specific filter settings.
 * @param[in]     ClientId: Identification of a client.
 * @return        uint16
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL uint16 Dem_ClientFilterNumberMemory(uint8 ClientId);

#if (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_ON)
/**
 * @brief         Tests whether an event is already reported.
 * @param[in]     ClientId: Identification of a client.
 * @param[in]     EventId: Identification of an event by assigned EventId.
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_FilterDataCheckEventReported(uint8 ClientId, Dem_EventIdType EventId);
#endif

/**
 * @brief         Iterates events in the event memory matching the Dcm DTC filter.
 * @param[in]     ClientId: Identification of a client.
 * @param[in]     MemoryInfo: Record memory information
 * @return        Dem_EventIdType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Dem_EventIdType Dem_FilterDataFilterMemoryByEvent(uint8 ClientId, const Dem_MemStateInfoType* MemoryInfo);

#if (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_ON)
/**
 * @brief         If chronological reporting is required this function iterates  all events in order of occurrence.
 * @param[in]     ClientId: Identification of a client.
 * @param[in]     MemoryInfo: Record memory information
 * @return        Dem_EventIdType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Dem_EventIdType Dem_FilterDataFilterMemoryChrono(uint8 ClientId, const Dem_MemStateInfoType* MemoryInfo);
#endif

#if (DEM_OBDII_SUPPORT == STD_ON)
/**
 * @brief         Iterates events in permanent memory matching the Dcm DTC filter.
 * @param[in]     ClientId: Identification of a client.
 * @param[in]     MemoryInfo: Record memory information
 * @return        Dem_EventIdType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Dem_EventIdType Dem_FilterDataFilterPermanentByEvent(uint8 ClientId, const Dem_MemStateInfoType* MemoryInfo);
#endif

/* PRQA S 5016 ++ */ /* VL_Dem_5016 */
/**
 * @brief         Get the DTC filter function corresponding to the DTC filtering strategy.
 * @param[in]     DTCFilteringStrategy: Filter strategy
 * @return        Dem_FilterDataFuncPtrType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Dem_FilterDataFuncPtrType
    Dem_FilterDataGetDTCFilterFunction(Dem_FilterDataDTCFilteringStrategyType DTCFilteringStrategy);
/* PRQA S 5016 -- */

/**
 * @brief         Get the next matching event the filter criteria set with Dem_ClientSetDTCFilter
 * @param[in]     ClientId: Identification of a client.
 * @param[out]    FilterResult: Result of filter data
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_FilterDataGetNextFilteredDTC(uint8 ClientId, Dem_FilterDataResultType* FilterResult);

/**
 * @brief         Sets a freeze frame record filter with the given criteria.
 * @param[in]     ClientId: Identification of a client.
 * @param[in]     DTCFormat: Output format of the DTC value.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_ClientStartFreezeFrameIterator(uint8 ClientId, Dem_DTCFormatType DTCFormat);

/**
 * @brief         Get a DTC based on occurrence time.
 * @param[in]     ClientId: Identification of a client.
 * @param[in]     DTCRequest: This parameter defines the request type of the DTC.
 * @param[out]    DTC: Receives the DTC value in respective format of the filter returned by this function.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_ClientGetDTCByOccurrenceTime(uint8 ClientId, Dem_DTCRequestType DTCRequest, uint32* DTC);

/**
 * @brief         Select the the given extended data record
 * @param[in]     ClientId: Identification of a client.
 * @param[in]     RecordNumber: Unique identifier for a snapshot record as defined in ISO 14229-1. The value 0xFF
 *                              is a placeholder referencing all snapshot records of the addressed DTC. The value
 *                              0x00 indicates the DTC-specific WWH-OBD snapshot record.
 * @param[out]    ErrorId: Error code ID
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_ClientSelectExtendedDataRecord(
    uint8 ClientId,
    uint8 RecordNumber
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    ,
    uint8* ErrorId
#endif
);

/**
 * @brief         Calculates the size of an extended data record
 * @param[in]     CfgEDIndex: Config extend data index.
 * @param[in]     DcmClientId: Identification of a client.
 * @param[in]     MemoryIndex: Memory index
 * @param[in]     ExtendedEntryIndex: Extend data entry index ID
 * @return        uint32
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL uint32 Dem_GetSizeOfEDCalculateSize(
    Dem_EDRecordRefNumType CfgEDIndex,
    uint8                  DcmClientId,
    Dem_NvBlockNumType     MemoryIndex,
    uint8                  ExtendedEntryIndex);

#if (DEM_EXTENDED_DATA_CLASS_NUMBER > 0u)
/**
 * @brief         Copies an extended data record into the given buffer
 * @param[in]     DcmClientId: Identification of a client.
 * @param[in]     DestinationBuffer: Destination buffer
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType
    Dem_ClientGetNextEDCopyNextRecord(uint8 DcmClientId, Dem_DestinationBufferPtrType DestinationBuffer);

/**
 * @brief         Test whether filter for extended data record(s) matches
 * @param[in]     ClientId: Identification of a client.
 * @param[in]     RecordNumber: Unique identifier for a snapshot record as defined in ISO 14229-1. The value 0xFF
 *                              is a placeholder referencing all snapshot records of the addressed DTC. The value
 *                              0x00 indicates the DTC-specific WWH-OBD snapshot record.
 * @param[in]     RecordNumberFilter: Filter RecordNumber.
 * @return        uint32
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_ClientGetNextEDChecktRecordMatch(uint8 ClientId, uint8 RecordNumberFilter, uint8 RecordNumber);
#endif

/**
 * @brief         Get the size of one or all extended data record(s)
 * @param[in]     ClientId: Identification of a client.
 * @param[out]    SizeOfExtendedDataRecord: Size of the requested extended data record(s) including record number.
 *                                          The format for a single ExtendedDataRecord is:
 *                                          {RecordNumber, data[1], ..., data[N]}
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType
    Dem_ClientGetSizeOfExtendedDataRecordSelection(uint8 ClientId, uint32* SizeOfExtendedDataRecord);

/**
 * @brief         Validates the request to get an extended data record in UDS format and copies the data.
 * @param[in]     ClientId: Identification of a client.
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
DEM_LOCAL Std_ReturnType Dem_ClientGetNextExtendedDataRecord(uint8 ClientId, uint8* DestBuffer, uint16* BufSize);

/**
 * @brief         Initializes the iterator for the selected event.
 * @param[in]     DcmClientId: Identification of a client.
 * @param[in]     RecordNumber: Unique identifier for a snapshot record as defined in ISO 14229-1. The value 0xFF
 *                              is a placeholder referencing all snapshot records of the addressed DTC. The value
 *                              0x00 indicates the DTC-specific WWH-OBD snapshot record.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_ReadoutBufferSelectFF(uint8 DcmClientId, uint8 RecordNumber);

/**
 * @brief         Sets the filter to be used by Dem_GetNextFreezeFrameData and Dem_GetSizeOfFreezeFrameSelection.
 * @param[in]     DcmClientId: Identification of a client.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_GetFFInitFFIterator(uint8 DcmClientId);

/**
 * @brief         Select the first snapshot record source
 * @param[in]     ClientId: Identification of a client.
 * @param[in]     RecordNumber: Unique identifier for a snapshot record as defined in ISO 14229-1. The value 0xFF
 *                              is a placeholder referencing all snapshot records of the addressed DTC. The value
 *                              0x00 indicates the DTC-specific WWH-OBD snapshot record.
 * @param[out]    ErrorId: Error code ID
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_ClientSelectFirstSource(
    uint8 ClientId,
    uint8 RecordNumber
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    ,
    uint8* ErrorId
#endif
);

/**
 * @brief         Get the size of one or all formatted snapshot record() stored for a DTC (19 04).
 * @param[in]     ClientId: Identification of a client.
 * @param[in]     RecordNumber: Unique identifier for a snapshot record as defined in ISO 14229-1. The value 0xFF
 *                              is a placeholder referencing all snapshot records of the addressed DTC. The value
 *                              0x00 indicates the DTC-specific WWH-OBD snapshot record.
 * @param[out]    SizeOfFreezeFrame: Number of bytes in the requested freeze frame record.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_ClientGetSizeOfFFSelection(uint8 ClientId, uint32* SizeOfFreezeFrame);

#if (DEM_OBDII_SUPPORT == STD_ON)
/**
 * @brief         Reads the OBD freeze frame
 * @param[in]     ClientId: Identification of a client.
 * @param[in]     DestinationBuffer: Destination buffer
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_GetNextFFDataObd(uint8 ClientId, Dem_DestinationBufferPtrType DestinationBuffer);
#endif

#if (DEM_MAX_SIZE_FREEZEFRAME > 0u)
/**
 * @brief         Reads the freeze frame
 * @param[in]     ClientId: Identification of a client.
 * @param[in]     DestinationBuffer: Destination buffer
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_GetNextFFDataStd(uint8 ClientId, Dem_DestinationBufferPtrType DestinationBuffer);
#endif

/**
 * @brief         Select the next snapshot record source
 * @param[in]     DcmClientId: Identification of a client.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_ClientSelectNextSource(uint8 DcmClientId);

/**
 * @brief         Copies data into the given buffer.
 * @param[in]     ClientId: Identification of a client.
 * @param[out]    DestBuffer: Destination buffer
 * @param[out]    BufSize: Buffer size
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_ClientGetNextFreezeFrameData(uint8 ClientId, uint8* DestBuffer, uint16* BufSize);
#endif

#if (DEM_SUPPRESSION_SUPPORT == DEM_DTC_SUPPRESSION)
/**
 * @brief         Suppress event or release event suppression as requested by the client
 * @param[in]     ClientId: Identification of a client.
 * @param[out]    SuppressionStatus: Defines whether the respective DTC is suppressed (TRUE) or enabled (FALSE).
 * @param[out]    ErrorId: Error code ID
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Std_ReturnType Dem_ClientGetDTCSuppression(
    uint8    ClientId,
    boolean* SuppressionStatus
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    ,
    uint8* ErrorId
#endif
);
#endif

/* ============================================ internal data definitions =========================================== */
#if (DEM_CLIENT_REPORT_EVENT_SIZE > 0u)
#define DEM_START_SEC_VAR_CLEARED_32
#include "Dem_MemMap.h"
/**
 * @brief Filter reported event array
 * @range 0..4294967295
 */
DEM_LOCAL Dem_ReportedEventsType Dem_ReportedEventsOfFilter[DEM_DCM_CLIENT_NUMBER];
#define DEM_STOP_SEC_VAR_CLEARED_32
#include "Dem_MemMap.h"
#endif

#define DEM_START_SEC_VAR_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
/**
 * @brief Filter data infomation array
 * @range NA
 */
DEM_LOCAL Dem_FilterDataInfoType Dem_FilterDataInfo[DEM_DCM_CLIENT_NUMBER + DEM_J1939DCM_CLIENT_NUMBER];
#define DEM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
#endif

#define DEM_START_SEC_VAR_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
/**
 * @brief Client infomation array
 * @range NA
 */
DEM_LOCAL Dem_ClientInfoType Dem_ClientInfo[DEM_CLIENT_NUMBER];
#define DEM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

/* ========================================== internal function definitions ========================================= */
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Checking Client ID Validity
 * @param[in]     ClientId: Identification of a client.
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_ClientCheckValid(uint8 ClientId)
{
    return (ClientId < DEM_CLIENT_NUMBER); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/**
 * @brief         Checking Dcm Client ID Validity
 * @param[in]     ClientId: Identification of a client.
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_ClientCheckDcmClientIdValid(uint8 ClientId)
{
    boolean ret = FALSE;
    if (ClientId < DEM_CLIENT_NUMBER)
    {
        if ((Dem_GetDcmClientIdOfClient(ClientId) != DEM_CLIENT_NUMBER))
        {
            ret = TRUE;
        }
    }
    return ret;
}

/**
 * @brief         Checking J1939Dcm Client ID Validity
 * @param[in]     ClientId: Identification of a client.
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_ClientCheckJ1939DcmClientIdValid(uint8 ClientId)
{
    boolean ret = FALSE;
    if (ClientId < DEM_CLIENT_NUMBER)
    {
        if ((Dem_GetJ1939DcmClientIdOfClient(ClientId) != DEM_CLIENT_NUMBER))
        {
            ret = TRUE;
        }
    }
    return ret;
}

/**
 * @brief         Checking DTC Origin Validity
 * @param[in]     DTCOrigin: If the Dem supports more than one event memory this parameter is used to select the
 *                           source memory the overflow indication shall be read from.
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_DTCOriginCheckValid(Dem_DTCOriginType DTCOrigin)
{
    boolean ret = TRUE;
    if ((DTCOrigin == DEM_DTC_ORIGIN_ZERO_INVALID) || (DTCOrigin == DEM_DTC_ORIGIN_THREE_INVALID)
        || ((DTCOrigin > DEM_DTC_ORIGIN_OBD_RELEVANT_MEMORY) && (DTCOrigin < DEM_DTC_ORIGIN_USER_MIN_INVALID))
        || (DTCOrigin > DEM_DTC_ORIGIN_USER_MAX_INVALID))
    {
        ret = FALSE;
    }
    return ret;
}

#if (DEM_USER_DEFINED_MEMORY_ENABLE == STD_ON)
/**
 * @brief Checking User Defined Memory Origin Validity
 */
DEM_LOCAL boolean
    Dem_DTCOriginUserDefinedCheckValid(uint8 ClientId, Dem_DTCOriginType DTCOrigin, Dem_MemoryNumType* MemIndex)
{
    boolean           ret          = FALSE;
    Dem_DTCOriginType origin       = DTCOrigin & DEM_USERDEF_MASK;
    uint8             memorySetRef = Dem_GetMemorySetRefOfClient(ClientId);
    for (Dem_MemoryNumType start = Dem_GetUserDefStartOfMemSet(memorySetRef);
         start < Dem_GetUserDefEndOfMemSet(memorySetRef);
         ++start)
    {
        const Dem_MemStateInfoType* memoryInfo = Dem_MemStateInfoInit(start);
        if (origin == memoryInfo->MemoryId)
        {
            *MemIndex = start;
            ret       = TRUE;
            break;
        }
    }
    return ret;
}
#endif

/**
 * @brief Checking DTC Origin Validity in Client
 */
DEM_LOCAL boolean
    Dem_DTCOriginCheckValidOnClient(uint8 ClientId, Dem_DTCOriginType DTCOrigin, Dem_MemoryNumType* MemIndex)
{
    boolean ret          = TRUE;
    uint8   memorySetRef = Dem_GetMemorySetRefOfClient(ClientId);
    if (DTCOrigin == DEM_DTC_ORIGIN_PERMANENT_MEMORY)
    {
#if (DEM_PERMANENT_MEMORY_NUMBER > 0u)
        if (Dem_GetPermanentNumOfMemSet(memorySetRef) == 0u)
#endif
        {
            ret = FALSE;
        }
#if (DEM_PERMANENT_MEMORY_NUMBER > 0u)
        else
        {
            /** DEM_DTC_ORIGIN_PERMANENT_MEMORY */
            *MemIndex = Dem_GetPrimaryRefOfMemSet(memorySetRef);
        }
#endif
    }
    else if (DTCOrigin > DEM_DTC_ORIGIN_USER_MIN_INVALID)
    {
#if (DEM_USER_DEFINED_MEMORY_ENABLE == STD_ON)
        if (Dem_DTCOriginUserDefinedCheckValid(ClientId, DTCOrigin, MemIndex) == FALSE)
#endif
        {
            ret = FALSE;
        }
    }
    else
    {
        /** DEM_DTC_ORIGIN_PRIMARY_MEMORY and DEM_DTC_ORIGIN_OBD_RELEVANT_MEMORY */
        *MemIndex = Dem_GetPrimaryRefOfMemSet(memorySetRef);
    }
    return ret;
}

/**
 * @brief         Set the Client Request Info
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[in]     ClearDTCStatus: The status that DTC cleared.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetClientClearDTCStatus(uint8 ClientId, Dem_ClearDTCStatusType ClearDTCStatus)
{
    Dem_ClientInfo[ClientId].ClearDTCStatus = ClearDTCStatus;
}

/**
 * @brief         Get the Client Request Info
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @return        Dem_ClearDTCStatusType
 * @retval        DEM_CLEARDTC_STATUS_IDLE: No clear operation in progress
 * @retval        DEM_CLEARDTC_STATUS_PENDING: Currently clearing of DTCs is in progress
 * @retval        DEM_CLEARDTC_STATUS_DONE: ClearDTC has finished sucessfully
 * @retval        DEM_CLEARDTC_STATUS_FAILED: ClearDTC has not finished sucessfully
 * @retval        DEM_CLEARDTC_STATUS_NV_ERROR: NV write error during ClearDTC
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_ClearDTCStatusType Dem_GetClientClearDTCStatus(uint8 ClientId)
{
    return Dem_ClientInfo[ClientId].ClearDTCStatus;
}

/**
 * @brief         Set the Client Result
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[in]     ClientResult: Result of the client.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetClientResult(uint8 ClientId, Dem_ClientResultType ClientResult)
{
    Dem_ClientInfo[ClientId].ClientResult = ClientResult;
}

/**
 * @brief         Get the Client Result
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @return        Dem_ClientResultType
 * @retval        DEM_CLIENT_RESULT_INIT: Initialization value
 * @retval        DEM_CLIENT_RESULT_DTC: A DTC was selected
 * @retval        DEM_CLIENT_RESULT_GROUP: A DTC group was selected
 * @retval        DEM_CLIENT_RESULT_ALL_DTCS: All DTCs were selected
 * @retval        DEM_CLIENT_RESULT_NO_MATCH: Selected number does not map to a DTC or DTC group
 * @retval        DEM_CLIENT_RESULT_WRONG_ORIGIN: DTC Origin not supported
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_ClientResultType Dem_GetClientResult(uint8 ClientId)
{
    return Dem_ClientInfo[ClientId].ClientResult;
}

/**
 * @brief         Get the Client Request
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @return        Dem_ClientRequestType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_ClientRequestType Dem_GetClientRequest(uint8 ClientId)
{
    return Dem_ClientInfo[ClientId].Request;
}

/**
 * @brief         Locks access to the API Dem_SelectDTC
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_LockClient(uint8 ClientId)
{
    Dem_ClientInfo[ClientId].ActiveAsyncOps++;
}

/**
 * @brief         Release access to the API Dem_SelectDTC
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_ReleaseClient(uint8 ClientId)
{
    Dem_ClientInfo[ClientId].ActiveAsyncOps--;
}

/**
 * @brief         Check Locks access to the API Dem_SelectDTC
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckClientLocked(uint8 ClientId)
{
    return (Dem_ClientInfo[ClientId].ActiveAsyncOps > 0u); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

#if ((DEM_DCM_CLIENT_NUMBER + DEM_J1939DCM_CLIENT_NUMBER) > 0u)
/**
 * @brief         Set the data of a DTC filter.
 * @param[in]     DcmClientId: Unique client id, assigned to the instance of the calling module.
 * @param[in]     FilterData: Filter data type.
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetFilterData(uint8 DcmClientId, Dem_FilterDataInfoType FilterData)
{
    Dem_FilterDataInfo[DcmClientId] = FilterData;
}

/**
 * @brief         Get the data of a DTC filter.
 * @param[in]     DcmClientId: Unique client id, assigned to the instance of the calling module.
 * @return        Dem_FilterDataInfoType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_FilterDataInfoType Dem_GetFilterData(uint8 DcmClientId)
{
    return Dem_FilterDataInfo[DcmClientId];
}

/**
 * @brief         Get the MemoryId stored in the filter object for DTC filtering.
 * @param[in]     DcmClientId: Unique client id, assigned to the instance of the calling module.
 * @return        Dem_MemoryNumType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_MemoryNumType Dem_FilterDataGetMemIndex(uint8 DcmClientId)
{
    return Dem_FilterDataInfo[DcmClientId].MemIndex;
}

/**
 * @brief         Check the data of a DTC filter.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[in]     FilterState: Filter state type.
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_ClientCheckDTCFilterSet(uint8 ClientId, Dem_FilterDataFilterType FilterState)
{
    uint8 DcmClientId;
    if (DEM_FILTERDATA_FILTER_DTC == FilterState)
    {
        DcmClientId = Dem_GetDcmClientIdOfClient(ClientId);
    }
    else
    {
        DcmClientId = Dem_GetJ1939DcmClientIdOfClient(ClientId) + DEM_DCM_CLIENT_NUMBER;
    }
    return (Dem_FilterDataInfo[DcmClientId].FilterState == FilterState); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/**
 * @brief         Get an enumerator indicating the DTC filtering strategy corresponding to the filter object.
 * @param[in]     DcmClientId: Unique client id, assigned to the instance of the calling module.
 * @return        Dem_FilterDataDTCFilteringStrategyType
 * @retval        Dem_FilterDataInvalid: Invalid filteringSelected DTC value
 * @retval        Dem_FilterDataFilterChronological: Chronological filtering
 * @retval        Dem_FilterDataFilterEventBased: Non-ordered filtering
 * @retval        Dem_FilterDataFilterEventBasedPermanentMemory: Non-ordered filtering in permanent memory
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_FilterDataDTCFilteringStrategyType Dem_FilterDataGetDTCFilteringStrategy(uint8 DcmClientId)
{
    return Dem_FilterDataInfo[DcmClientId].FilterDtcStrategy;
}

/**
 * @brief         Get the Chrono Index stored in the filter object.
 * @param[in]     DcmClientId: Unique client id, assigned to the instance of the calling module.
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_FilterDataGetChronoIndex(uint8 DcmClientId)
{
    return Dem_FilterDataInfo[DcmClientId].Iterator.ChronoIndex;
}

/**
 * @brief         Get the event Id stored in the filter object.
 * @param[in]     DcmClientId: Unique client id, assigned to the instance of the calling module.
 * @return        Dem_EventIdType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventIdType Dem_FilterDataGetEventId(uint8 DcmClientId)
{
    return Dem_FilterDataInfo[DcmClientId].Iterator.EventIndex;
}

/**
 * @brief         Get the UseStatusMask flag of the given filter flags.
 * @param[in]     DcmClientId: Unique client id, assigned to the instance of the calling module.
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_FilterDataCheckUseStatusMask(uint8 DcmClientId)
{
    /* PRQA S 4404 ++ */ /* VL_Dem_4404 */
    boolean ret = DEM_FLAG_ISSET(Dem_FilterDataInfo[DcmClientId].FilterFlags, DEM_FILTERDATA_FILTERFLAGS_USESTATUSMASK);
    /* PRQA S 4404 -- */
    return ret;
}

/**
 * @brief         Get the status mask stored in the filter object.
 * @param[in]     DcmClientId: Unique client id, assigned to the instance of the calling module.
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_FilterDataGetStatusMask(uint8 DcmClientId)
{
    return Dem_FilterDataInfo[DcmClientId].StatusMask;
}

/**
 * @brief         Get the UseSeverity flag of the given filter flags.
 * @param[in]     DcmClientId: Unique client id, assigned to the instance of the calling module.
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_FilterDataCheckUseSeverity(uint8 DcmClientId)
{
    /* PRQA S 4404 ++ */ /* VL_Dem_4404 */
    boolean ret = DEM_FLAG_ISSET(Dem_FilterDataInfo[DcmClientId].FilterFlags, DEM_FILTERDATA_FILTERFLAGS_USESEVERITY);
    /* PRQA S 4404-- */
    return ret;
}

/**
 * @brief 		  Get the UseFdc flag of the given filter flags.
 * @param[in]     DcmClientId: Unique client id, assigned to the instance of the calling module.
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_FilterDataCheckUseFdc(uint8 DcmClientId)
{
    /* PRQA S 4404 ++ */ /* VL_Dem_4404 */
    boolean ret = DEM_FLAG_ISSET(Dem_FilterDataInfo[DcmClientId].FilterFlags, DEM_FILTERDATA_FILTERFLAGS_USEFDC);
    /* PRQA S 4404 -- */
    return ret;
}

#if (DEM_OBDII_SUPPORT == STD_ON)
/**
 * @brief 		  Get the UseFormatOBD flag of the given filter flags.
 * @param[in]     DcmClientId: Unique client id, assigned to the instance of the calling module.
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_FilterDataCheckUseFormatOBD(uint8 DcmClientId)
{
    boolean ret = DEM_FLAG_ISSET(Dem_FilterDataInfo[DcmClientId].FilterFlags, DEM_FILTERDATA_FILTERFLAGS_USEFORMATOBD);
    return ret;
}
#endif

/**
 * @brief 		  Get the severity mask stored in the filter object.
 * @param[in]     DcmClientId: Unique client id, assigned to the instance of the calling module.
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_FilterDataGetSeverityMask(uint8 DcmClientId)
{
    return Dem_FilterDataInfo[DcmClientId].SeverityMask;
}

#if (DEM_FEATURE_DCM_0X19_0X14_ENABLE == STD_ON)
/**
 * @brief 		  Get the fault detection counter in the given filter object.
 * @param[in]     DcmClientId: Unique client id, assigned to the instance of the calling module.
 * @return        sint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE sint8 Dem_FilterDataGetFDC(uint8 DcmClientId)
{
    return Dem_FilterDataInfo[DcmClientId].FaultDetectionCounter;
}
#endif
#endif

/**
 * @brief 		  Set the Client Request Info
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[in]     DTC: Defines the DTC in respective format that is selected. If the DTC fits to a DTC group
 *                     number, the DTC group is selected.
 * @param[in]     DTCFormat: Defines the output-format of the requested DTC value.
 * @param[in]     DTCOrigin: If the Dem supports more than one event memory this parameter is used to select the
 *                           source memory the overflow indication shall be read from.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void
    Dem_SetClientRequestInfo(uint8 ClientId, uint32 DTC, Dem_DTCFormatType DTCFormat, Dem_DTCOriginType DTCOrigin)
{
    Dem_ClientRequestType* clientRequest = &(Dem_ClientInfo[ClientId].Request);
    clientRequest->DTC                   = DTC;
    clientRequest->DTCFormat             = DTCFormat;
    clientRequest->DTCOrigin             = DTCOrigin;
}

/**
 * @brief 		  Initializes the Client Info
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_InitClientInfoByClientId(uint8 ClientId)
{
    Dem_ClientInfoType* clientInfo = &(Dem_ClientInfo[ClientId]);
    /* PRQA S 1258 ++ */ /* VL_Dem_1258 */
    Dem_SetClientRequestInfo(ClientId, DEM_DTC_MAX_VALUE, DEM_DTC_FORMAT_INVALID, DEM_DTC_ORIGIN_USER_MAX_INVALID);
    /* PRQA S 1258 -- */
    Dem_SetClientClearDTCStatus(ClientId, DEM_CLEARDTC_STATUS_IDLE);
    Dem_SetClientResult(ClientId, DEM_CLIENT_RESULT_INIT);
    clientInfo->MemIndex          = DEM_TOTAL_MEMORY_NUMBER;
    clientInfo->Selection.EventId = DEM_EVENT_INVALID;
    clientInfo->ActiveAsyncOps    = 0u;
}

/**
 * @brief 		  Copy the memory entry contents into the ReadoutBuffer.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_InitClientDTCSetting(void)
{
    Dem_MemSet(Dem_GetClientSetting(), 0u, DEM_DCM_CLIENT_NUMBER);
}

/* PRQA S 1258 ++ */ /* VL_Dem_1258 */
/**
 * @brief 		  Test whether a DTC number encodes an 'AllDTC' mask.
 * @param[in]     DTC: Defines the DTC in respective format that is selected. If the DTC fits to a DTC group
 *                     number, the DTC group is selected.
 * @param[in]     DTCFormat: Defines the output-format of the requested DTC value.
 * @return        Dem_ClientGroupIdType
 * @retval        DEM_CLIENT_GROUP_INVALID: No group
 * @retval        DEM_CLIENT_GROUP_ALL_DTCS: UDS group All DTCs
 * @retval        DEM_CLIENT_GROUP_EMISSION_RELATED: OBD group All DTCs
 * @retval        DEM_CLIENT_GROUP_WWH_OBD_ALL_DTC: WWH-OBD group All DTCs
 * @retval        DEM_CLIENT_GROUP_ALL_J1939_DTCS: J1939 group All DTCs
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_ClientGroupIdType Dem_ClientCheckAllDTC(uint32 DTC, Dem_DTCFormatType DTCFormat)
{
    Dem_ClientGroupIdType groupId = DEM_CLIENT_GROUP_INVALID;
    if ((DTCFormat == DEM_DTC_FORMAT_UDS) || (DTCFormat == DEM_DTC_FORMAT_OBD)
#if (DEM_SUPPORTED_OBD_UDS_DTC_SEPARATION == STD_ON)
        || (DTCFormat == DEM_DTC_FORMAT_OBD_3BYTE)
#endif
    )
    {
        if (DTC == DEM_DTC_GROUP_ALL_DTCS)
        {
            groupId = DEM_CLIENT_GROUP_ALL_DTCS;
        }
#if (DEM_OBDII_SUPPORT == STD_ON)
        else if (DTC == DEM_DTC_GROUP_EMISSION_REL_DTCS)
        {
            groupId = DEM_CLIENT_GROUP_EMISSION_RELATED;
        }
        else if (DTC == DEM_DTC_GROUP_WWHOBD_DTCS)
        {
            groupId = DEM_CLIENT_GROUP_WWH_OBD_ALL_DTC;
        }
        else
        {
            /** Misra C */
        }
#endif
    }
#if (DEM_J1939_CLEAR_DTC_SUPPORT == STD_ON)
#if (DEM_J1939_SUPPORT == STD_ON)
    else
    {
        /** DTC Format J1939 */
        if (DTC == DEM_DTC_GROUP_ALL_DTCS)
        {
            groupId = DEM_CLIENT_GROUP_ALL_J1939_DTCS;
        }
    }
#endif
#endif
    return groupId;
}
/* PRQA S 1258 -- */

#if (DEM_DTC_GROUP_NUMBER > 0u)
/**
 * @brief 		  Test if a DTC number encodes a specific DTC group (not ALLDTCs).
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[in]     DTC: Defines the DTC in respective format that is selected. If the DTC fits to a DTC group
 *                     number, the DTC group is selected.
 * @param[in]     DTCFormat: Defines the output-format of the requested DTC value.
 * @return        Dem_GroupOfDTCNumType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_GroupOfDTCNumType Dem_ClientCheckGroupDTC(uint8 ClientId, uint32 DTC, Dem_DTCFormatType DTCFormat)
{
    Dem_GroupOfDTCNumType groupId = DEM_CLIENT_GROUP_INVALID;
    if (DTCFormat == DEM_DTC_FORMAT_UDS)
    {
        Dem_GroupOfDTCNumType start;
        /** first to check primary */
        for (start = 0u; start < DEM_DTC_GROUP_NUMBER; ++start)
        {
            if ((DTC == Dem_GetDTCOfGroupDTCs(start))
                && (Dem_GetRefedMemorySetOfGroupDTC(start) == Dem_GetMemorySetRefOfClient(ClientId)))
            {
                groupId = start;
                break;
            }
        }
    }
    return groupId;
}
#endif

/**
 * @brief 		  Find the event configured to use the DTC number
 * @param[in]     DTC: Defines the DTC in respective format that is selected. If the DTC fits to a DTC group
 *                     number, the DTC group is selected.
 * @param[out]    MemIndex: Memory Index
 * @return        Dem_EventIdType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventIdType Dem_ClientFindEventFromUdsDtc(uint32 DTC, Dem_MemoryNumType MemIndex)
{
    const Dem_MemStateInfoType* memoryInfo = Dem_MemStateInfoInit(MemIndex);
    Dem_EventIdType             eventId    = DEM_EVENT_INVALID;
    Dem_EventIdType             lastEvent  = memoryInfo->LastEvent;
    Dem_EventIdType             start      = memoryInfo->FirstEvent;
    for (; start < lastEvent; ++start)
    {
        if (Dem_GetEventDTC(start) == DTC)
        {
            eventId = start;
            break;
        }
    }
    return eventId;
}

#if (DEM_J1939_SUPPORT == STD_ON)
/**
 * @brief 		  Find the event configured to use the J1939 DTC number
 * @param[in]     DTC: Defines the DTC in respective format that is selected. If the DTC fits to a DTC group
 *                     number, the DTC group is selected.
 * @param[out]    MemIndex: Memory Index
 * @return        Dem_EventIdType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_EventIdType Dem_ClientFindEventFromJ1939Dtc(uint32 DTC, Dem_MemoryNumType MemIndex)
{
    const Dem_MemStateInfoType* memoryInfo = Dem_MemStateInfoInit(MemIndex);
    Dem_EventIdType             eventId    = DEM_EVENT_INVALID;
    Dem_EventIdType             lastEvent  = memoryInfo->LastEvent;
    Dem_EventIdType             start      = memoryInfo->FirstEvent;
    for (; start < lastEvent; ++start)
    {
        if (Dem_GetEventJ1939DTC(start) == DTC)
        {
            eventId = start;
            break;
        }
    }
    return eventId;
}
#endif

/**
 * @brief         Checking if a DTC number encodes a specific DTC.
 */
DEM_LOCAL Dem_EventIdType
    Dem_ClientCheckSingleDTC(uint8 ClientId, uint32 DTC, Dem_DTCFormatType DTCFormat, Dem_DTCOriginType DTCOrigin)
{
    Dem_EventIdType eventId = DEM_EVENT_INVALID; /* PRQA S 1252 */ /* VL_Dem_1252 */
    if (DTC != 0u) /* PRQA S 1252 */                               /* VL_Dem_1252 */
    {
        uint8             memorySetRef = Dem_GetMemorySetRefOfClient(ClientId);
        Dem_MemoryNumType memIndex;
        if (DTCOrigin < DEM_DTC_ORIGIN_USER_MIN_INVALID)
        {
            memIndex = Dem_GetPrimaryRefOfMemSet(memorySetRef);
        }
        else
        {
            memIndex = Dem_ClientInfo[ClientId].MemIndex;
        }

        if (DTCFormat == DEM_DTC_FORMAT_UDS)
        {
            eventId = Dem_ClientFindEventFromUdsDtc(DTC, memIndex);
        }
#if (DEM_J1939_SUPPORT == STD_ON)
        else if (DTCFormat == DEM_DTC_FORMAT_J1939)
        {
            eventId = Dem_ClientFindEventFromJ1939Dtc(DTC, memIndex);
        }
#endif
        else
        {
            /** Mirsa C */
        }
    }
    return eventId;
}

/**
 * @brief Selects a DTC or DTC group as target for further operations.
 */
DEM_LOCAL void Dem_ClientSelectDTC(uint8 ClientId, uint32 DTC, Dem_DTCFormatType DTCFormat, Dem_DTCOriginType DTCOrigin)
{
    Dem_MemoryNumType memIndex;
    Dem_SetClientClearDTCStatus(ClientId, DEM_CLEARDTC_STATUS_IDLE);
    Dem_SetClientRequestInfo(ClientId, DTC, DTCFormat, DTCOrigin);
    if (Dem_DTCOriginCheckValidOnClient(ClientId, DTCOrigin, &memIndex) == FALSE)
    {
        Dem_SetClientResult(ClientId, DEM_CLIENT_RESULT_WRONG_ORIGIN);
    }
    else
    {
        Dem_ClientGroupIdType groupId     = Dem_ClientCheckAllDTC(DTC, DTCFormat);
        Dem_ClientInfo[ClientId].MemIndex = memIndex;
        if (groupId != DEM_CLIENT_GROUP_INVALID)
        {
            Dem_ClientInfo[ClientId].Selection.GroupId = groupId;
            Dem_SetClientResult(ClientId, DEM_CLIENT_RESULT_ALL_DTCS);
        }
        else
        {
#if (DEM_DTC_GROUP_NUMBER > 0u)
            Dem_GroupOfDTCNumType lDTCGroupId = Dem_ClientCheckGroupDTC(ClientId, DTC, DTCFormat);
            if (lDTCGroupId != DEM_CLIENT_GROUP_INVALID)
            {
                Dem_ClientInfo[ClientId].Selection.GroupDTCIndex = lDTCGroupId;
                Dem_SetClientResult(ClientId, DEM_CLIENT_RESULT_GROUP);
            }
            else
#endif
            {
                Dem_EventIdType eventId = Dem_ClientCheckSingleDTC(ClientId, DTC, DTCFormat, DTCOrigin);
                if (eventId != DEM_EVENT_INVALID)
                {
                    Dem_ClientInfo[ClientId].Selection.EventId = eventId;
                    Dem_SetClientResult(ClientId, DEM_CLIENT_RESULT_DTC);
                }
                else
                {
                    Dem_SetClientResult(ClientId, DEM_CLIENT_RESULT_NO_MATCH);
                }
            }
        }
    }
}

#if (DEM_DCM_CLIENT_NUMBER > 0u)
/**
 * @brief 		  Set the readout buffer entry
 * @param[in]     DcmClientId: Identification of a client.
 * @param[in]     ReadoutBuffer: The buffer to store readout data
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_ClientSetReadoutBuffer(uint8 DcmClientId, Dem_ReadoutBufferEntryType ReadoutBuffer)
{
    Dem_GetReadoutBuffer()[DcmClientId] = ReadoutBuffer;
}

/**
 * @brief 		  Returns the readout buffer entry
 * @param[in]     DcmClientId: Identification of a client.
 * @retval        Dem_ReadoutBufferEntryType
 * @retval        State: Internal state of this buffer
 * @retval        MemIndex: MemoryIndex of this Data record
 * @retval        ExtendedIterator: Internal state for reporting ExtendedData record(s)
 * @retval        FFIterator: Internal state for reporting SnapshotData record(s)
 * @retval        EventId: EventId of this Data record
 * @retval        Data: Backed event data
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_ReadoutBufferEntryType Dem_ClientGetReadoutBuffer(uint8 DcmClientId)
{
    return Dem_GetReadoutBuffer()[DcmClientId];
}

#if (DEM_EXTENDED_DATA_CLASS_NUMBER > 0u)
/**
 * @brief 		  Init the readout buffer entry  Extend Data Iterator
 * @param[in]     OldEDIterator: Extend Data Iterator
 * @retval        Dem_ReadoutBufferEDIteratorType
 * @retval        ExtendedRecordSelected: Specifies whether a record is selected or not
 * @retval        ExtendedEntryIndex: Extended entry index
 * @retval        ExtendedDataNumber: The selected extended data number
 * @retval        EDRNStart: Iterator for the extended entries
 * @retval        EDRNEnd: Iterator for the extended entries
 * @retval        MemoryIndex: Memory entry handle
 * @retval        EventId: Event Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_ReadoutBufferEDIteratorType
    Dem_ReadoutBufferEDRecordInit(Dem_ReadoutBufferEDIteratorType OldEDIterator)
{
    Dem_ReadoutBufferEDIteratorType lEDIterator;
    lEDIterator                        = OldEDIterator;
    lEDIterator.EDRNStart              = 0u;
    lEDIterator.EDRNEnd                = 0u;
    lEDIterator.MemoryIndex            = DEM_MEM_INVALID_MEMORY_INDEX;
    lEDIterator.ExtendedEntryIndex     = 0u;
    lEDIterator.EventId                = DEM_EVENT_INVALID;
    lEDIterator.ExtendedDataNumber     = 0u;
    lEDIterator.ExtendedRecordSelected = FALSE;
    return lEDIterator;
}

/**
 * @brief 		  Returns the extended iterator of the given readout buffer
 * @param[in]     DcmClientId: Identification of a client.
 * @retval        Dem_ReadoutBufferEDIteratorType
 * @retval        ExtendedRecordSelected: Specifies whether a record is selected or not
 * @retval        ExtendedEntryIndex: Extended entry index
 * @retval        ExtendedDataNumber: The selected extended data number
 * @retval        EDRNStart: Iterator for the extended entries
 * @retval        EDRNEnd: Iterator for the extended entries
 * @retval        MemoryIndex: Memory entry handle
 * @retval        EventId: Event Id
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_ReadoutBufferEDIteratorType Dem_ReadoutBufferGetEDIterator(uint8 DcmClientId)
{
    return Dem_ClientGetReadoutBuffer(DcmClientId).ExtendedIterator;
}

/**
 * @brief  		  Sets the extended iterator of the given readout buffer
 * @param[in]     DcmClientId: Identification of a client.
 * @param[in]     ExtendedIterator: Extended iterator.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void
    Dem_ReadoutBufferSetEDIterator(uint8 DcmClientId, Dem_ReadoutBufferEDIteratorType ExtendedIterator)
{
    Dem_ReadoutBufferEntryType readoutBuffer = Dem_ClientGetReadoutBuffer(DcmClientId);
    readoutBuffer.ExtendedIterator           = ExtendedIterator;
    Dem_ClientSetReadoutBuffer(DcmClientId, readoutBuffer);
}
#endif

/**
 * @brief         Returns the snapshot iterator of the given readout buffer
 * @param[in]     DcmClientId: Identification of a client.
 * @retval        Dem_ReadoutBufferFFIteratorType
 * @retval        SnapshotSelected: Specifies whether a snapshot is selected or not
 * @retval        Source: The source of the snapshot
 * @retval        RecordNumber: The selected extended data number
 * @retval        FFEntryIter: Iterator for the snapshot entries
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_ReadoutBufferFFIteratorType Dem_ReadoutBufferGetFFIterator(uint8 DcmClientId)
{
    return Dem_ClientGetReadoutBuffer(DcmClientId).FFIterator;
}

/**
 * @brief         Sets the snapshot iterator of the given readout buffer
 * @param[in]     DcmClientId: Identification of a client.
 * @param[in]     FFIterator: Internal state for reporting SnapshotData record(s)
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_ReadoutBufferSetFFIterator(uint8 DcmClientId, Dem_ReadoutBufferFFIteratorType FFIterator)
{
    Dem_ReadoutBufferEntryType readoutBuffer = Dem_ClientGetReadoutBuffer(DcmClientId);
    readoutBuffer.FFIterator                 = FFIterator;
    Dem_ClientSetReadoutBuffer(DcmClientId, readoutBuffer);
}

/**
 * @brief         Gets the snapshot source type
 * @param[in]     DcmClientId: Identification of a client.
 * @return        Dem_ReadoutBuffer_FFType
 * @retval        Dem_ReadoutBuffer_FF_Obd: OBD snapshot
 * @retval        Dem_ReadoutBuffer_FF_Srec: Snapshot record source
 * @retval        Dem_ReadoutBuffer_FF_Invalid: Not initialize or end of list identifier
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_ReadoutBuffer_FFType Dem_ReadoutBufferGetFFIteratorSource(uint8 DcmClientId)
{
    return Dem_ReadoutBufferGetFFIterator(DcmClientId).Source;
}

/**
 * @brief         Sets the snapshot source type
 * @param[in]     DcmClientId: Identification of a client.
 * @param[in]     Source: Source buffer.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_ReadoutBufferSetFFIteratorSource(uint8 DcmClientId, Dem_ReadoutBuffer_FFType Source)
{
    Dem_ReadoutBufferFFIteratorType lFFIterator = Dem_ReadoutBufferGetFFIterator(DcmClientId);
    lFFIterator.Source                          = Source;
    Dem_ReadoutBufferSetFFIterator(DcmClientId, lFFIterator);
}

/**
 * @brief         Returns whether data is stored or not
 * @param[in]     DcmClientId: Identification of a client.
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_ReadoutBufferCheckStoredDataAvailable(uint8 DcmClientId)
{
    /* PRQA S 4404 ++ */ /* VL_Dem_4404 */
    return (Dem_GetReadoutBuffer()[DcmClientId].State == DEM_READOUTBUFFER_STATUS_IN_USE);
    /* PRQA S 4404 -- */
}

#if ((DEM_MAX_SIZE_OBDFREEZEFRAME > 0u) || (DEM_FREEZE_FRAME_CLASS_NUMBER > 0u))

/**
 * @brief         Initializes the snapshot entry iterator
 * @param[in]     OldFFIterator: Internal state for reporting SnapshotData record(s)
 * @retval        Dem_ReadoutBufferFFIteratorType
 * @retval        SnapshotSelected: Specifies whether a snapshot is selected or not
 * @retval        Source: The source of the snapshot
 * @retval        RecordNumber: The selected extended data number
 * @retval        FFEntryIter: Iterator for the snapshot entries
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_ReadoutBufferFFIteratorType Dem_ReadoutBufferFFInit(Dem_ReadoutBufferFFIteratorType OldFFIterator)
{
    Dem_ReadoutBufferFFIteratorType lFFIterator;

    lFFIterator = OldFFIterator;
#if (DEM_MAX_SIZE_FREEZEFRAME > 0u)
    Dem_FFEntryIteratorInit(DEM_EVENT_INVALID, DEM_MEM_INVALID_MEMORY_INDEX, &(lFFIterator.FFEntryIter));
#endif
    lFFIterator.RecordNumber     = 0u;
    lFFIterator.Source           = Dem_ReadoutBuffer_FF_Invalid;
    lFFIterator.SnapshotSelected = FALSE;
    return lFFIterator;
}
#endif

/**
 * @brief Initialize memory entry.
 */
DEM_LOCAL void Dem_ClientReadoutBufferInit(uint8 ClientId)
{
    uint8 dcmClientId = Dem_GetDcmClientIdOfClient(ClientId);
    if (dcmClientId != DEM_CLIENT_NUMBER)
    {
        Dem_ReadoutBufferEntryType readoutBuffer;

        readoutBuffer = Dem_ClientGetReadoutBuffer(dcmClientId);

        readoutBuffer.State = DEM_READOUTBUFFER_STATUS_UNUSED;
#if (DEM_EXTENDED_DATA_CLASS_NUMBER > 0u)
        readoutBuffer.ExtendedIterator = Dem_ReadoutBufferEDRecordInit(readoutBuffer.ExtendedIterator);
#endif
#if ((DEM_MAX_SIZE_OBDFREEZEFRAME > 0u) || (DEM_FREEZE_FRAME_CLASS_NUMBER > 0u))
        readoutBuffer.FFIterator = Dem_ReadoutBufferFFInit(readoutBuffer.FFIterator);
#endif
        Dem_ClientSetReadoutBuffer(dcmClientId, readoutBuffer);
    }
}

/**
 * @brief         Initialize a filter.
 * @param[in]     ClientId: Identification of a client.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_FilterDataInitDefault(uint8 ClientId)
{
    uint8 dcmClientId = Dem_GetDcmClientIdOfClient(ClientId);
    if (dcmClientId != DEM_CLIENT_NUMBER)
    {
        Dem_FilterDataInfoType filterData = Dem_GetFilterData(dcmClientId);
        filterData.FilterState            = DEM_FILTERDATA_FILTER_INIT;
        filterData.MemIndex               = DEM_TOTAL_MEMORY_NUMBER;
        filterData.Iterator.EventIndex    = DEM_EVENT_INVALID;
#if (DEM_DCM_CLIENT_NUMBER > 0u)
        filterData.StatusMask            = 0x00u;
        filterData.SeverityMask          = DEM_SEVERITY_NO_SEVERITY;
        filterData.FaultDetectionCounter = 0;
        filterData.FilterDtcStrategy     = Dem_FilterDataFilterEventBased;
#endif
#if (DEM_J1939_SUPPORT == STD_ON)
        filterData.DTCStatusFilterType = 0u;
#endif
        Dem_SetFilterData(dcmClientId, filterData);
    }
}

/**
 * @brief Check state of a ReadoutBuffer entry, save current request if it is new.
 */
DEM_LOCAL Std_ReturnType
    Dem_DTCReadoutBufferCheckAndSetState(uint8 ClientId, Dem_EventIdType EventId, Dem_MemoryNumType MemIndex)
{
    Std_ReturnType             ret           = E_NOT_OK;
    uint8                      dcmClientId   = Dem_GetDcmClientIdOfClient(ClientId);
    Dem_ReadoutBufferEntryType readoutBuffer = Dem_ClientGetReadoutBuffer(dcmClientId);

    if (readoutBuffer.State == DEM_READOUTBUFFER_STATUS_UNUSED)
    {
        /** new request */
        Dem_LockClient(ClientId);
        readoutBuffer.EventId  = EventId;
        readoutBuffer.MemIndex = MemIndex;
#if (DEM_EXTENDED_DATA_CLASS_NUMBER > 0u)
        readoutBuffer.ExtendedIterator = Dem_ReadoutBufferEDRecordInit(readoutBuffer.ExtendedIterator);
#endif
#if ((DEM_MAX_SIZE_OBDFREEZEFRAME > 0u) || (DEM_FREEZE_FRAME_CLASS_NUMBER > 0u))
        readoutBuffer.FFIterator = Dem_ReadoutBufferFFInit(readoutBuffer.FFIterator);
#endif
        /** The task function could be active already due to another client. Set the state last, otherwise the task
         * could process this entry before it is copmletely initialized */
        readoutBuffer.State = DEM_READOUTBUFFER_STATUS_QUEUED;
        Dem_EnableTaskOnce(Dem_Task_FilterEventData);
        Dem_ClientSetReadoutBuffer(dcmClientId, readoutBuffer);
        ret = DEM_PENDING;
    }
    else if (EventId == readoutBuffer.EventId)
    {
        if (MemIndex == readoutBuffer.MemIndex)
        {
            /** repeated request with identical parameters */
            if (readoutBuffer.State == DEM_READOUTBUFFER_STATUS_QUEUED)
            {
                /** request is still queued, copying of data is not finished */
                ret = DEM_PENDING;
            }
            else
            {
                /** copying of data is finished */
                ret = E_OK;
            }
        }
    }
    else
    {
        /** repeated request with different parameters */
    }
    return ret;
}

/**
 * @brief         Test if the DTC record update is disabled
 * @param[in]     ClientId: Identification of a client.
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_ClientCheckDTCRecordUpdateDisabled(uint8 ClientId)
{
    Dem_ReadoutBufferEntryType readoutBuffer = Dem_ClientGetReadoutBuffer(Dem_GetDcmClientIdOfClient(ClientId));
    /* PRQA S 4404 ++ */ /* VL_Dem_4404 */
    return (
        (readoutBuffer.State == DEM_READOUTBUFFER_STATUS_IN_USE)
        || (readoutBuffer.State == DEM_READOUTBUFFER_STATUS_NOT_STORED));
    /* PRQA S 4404 -- */
}
#endif

/**
 * @brief Provides information if the last call to Dem_SelectDTC has selected a valid DTC or group of DTCs.
 */
DEM_LOCAL Std_ReturnType Dem_ClientGetDTCSelectionResult(
    uint8 ClientId
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    ,
    uint8* ErrorId
#endif
)
{
    Std_ReturnType ret = E_NOT_OK;
    if (Dem_CheckClientLocked(ClientId) == TRUE)
    {
        ret = DEM_BUSY;
    }
    else
    {
        switch (Dem_GetClientResult(ClientId))
        {
        case DEM_CLIENT_RESULT_DTC:
        case DEM_CLIENT_RESULT_GROUP:
        case DEM_CLIENT_RESULT_ALL_DTCS:
            ret = E_OK;
            break;
        case DEM_CLIENT_RESULT_NO_MATCH:
            ret = DEM_WRONG_DTC;
            break;
        case DEM_CLIENT_RESULT_WRONG_ORIGIN:
            ret = DEM_WRONG_DTCORIGIN;
            break;
        case DEM_CLIENT_RESULT_INIT:
            /** E_NOT_OK */
#if (DEM_DEV_ERROR_DETECT == STD_ON)
            *ErrorId = DEM_E_WRONG_CONDITION;
#endif
            break;
        default:
            /** E_NOT_OK */
            break;
        }
    }
    return ret;
}

/**
 * @brief Provides information if the last call to Dem_SelectDTC has selected a valid DTC or group of DTCs.
 */
DEM_LOCAL Std_ReturnType Dem_ClientResultForClearDTCGetDTCSelection(
    uint8 ClientId
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    ,
    uint8* ErrorId
#endif
)
{
    Std_ReturnType ret = E_NOT_OK;
    if (Dem_CheckClientLocked(ClientId) == TRUE)
    {
        ret = DEM_BUSY;
    }
    else
    {
        switch (Dem_GetClientResult(ClientId))
        {
        case DEM_CLIENT_RESULT_DTC:
        case DEM_CLIENT_RESULT_GROUP:
#if (DEM_CLEAR_DTC_LIMITATION == DEM_ONLY_CLEAR_ALL_DTCS)
            ret = DEM_WRONG_DTC;
            break;
#endif
        case DEM_CLIENT_RESULT_ALL_DTCS:
            ret = E_OK;
            break;
        case DEM_CLIENT_RESULT_NO_MATCH:
            ret = DEM_WRONG_DTC;
            break;
        case DEM_CLIENT_RESULT_WRONG_ORIGIN:
            ret = DEM_WRONG_DTCORIGIN;
            break;
        case DEM_CLIENT_RESULT_INIT:
            /** E_NOT_OK */
#if (DEM_DEV_ERROR_DETECT == STD_ON)
            *ErrorId = DEM_E_WRONG_CONDITION;
#endif
            break;
        default:
            /** E_NOT_OK */
            break;
        }
    }
    return ret;
}

/**
 * @brief Disables the event memory update of a specific DTC (only one at one time).
 */
DEM_LOCAL Std_ReturnType Dem_ClientDisableDTCRecordUpdate(
    uint8 ClientId
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    ,
    uint8* ErrorId
#endif
)
{
    Std_ReturnType ret = E_NOT_OK;
    /** missing SelectDTC call or incorrect arguments in that call */
    switch (Dem_GetClientResult(ClientId))
    {
    case DEM_CLIENT_RESULT_DTC:
    {
        /** get event id from selectDTC */
        const Dem_ClientInfoType* clientInfo = &Dem_ClientInfo[ClientId];
        Dem_EventIdType           eventId    = clientInfo->Selection.EventId;

        if (Dem_CheckDTCSuppressed(eventId) == FALSE)
        {
#if (DEM_DCM_CLIENT_NUMBER > 0u)
            ret = Dem_DTCReadoutBufferCheckAndSetState(ClientId, eventId, clientInfo->MemIndex);
#endif
        }
        else
        {
            ret = DEM_WRONG_DTC;
        }
        break;
    }
    case DEM_CLIENT_RESULT_GROUP:
    case DEM_CLIENT_RESULT_ALL_DTCS:
    case DEM_CLIENT_RESULT_NO_MATCH:
        ret = DEM_WRONG_DTC;
        break;
    case DEM_CLIENT_RESULT_WRONG_ORIGIN:
        ret = DEM_WRONG_DTCORIGIN;
        break;
    case DEM_CLIENT_RESULT_INIT:
    default:
#if (DEM_DEV_ERROR_DETECT == STD_ON)
        /** new request with different parameters than ongoing request */
        *ErrorId = DEM_E_WRONG_CONDITION;
#endif
        break;
    }
    return ret;
}

#if (DEM_DCM_CLIENT_NUMBER > 0u)
/**
 * @brief Release a previously prepared memory entry.
 */
DEM_LOCAL Std_ReturnType Dem_ClientEnableDTCRecordUpdate(uint8 ClientId)
{
    Std_ReturnType             ret         = E_NOT_OK;
    uint8                      dcmClientId = Dem_GetDcmClientIdOfClient(ClientId);
    Dem_ReadoutBufferEntryType readoutBuffer;
    if (dcmClientId != DEM_CLIENT_NUMBER)
    {
        readoutBuffer = Dem_ClientGetReadoutBuffer(dcmClientId);
        /** Enter Critical Section: DcmApi */
        Dem_EnterCritical_DcmApi();
        if (readoutBuffer.State == DEM_READOUTBUFFER_STATUS_QUEUED)
        {
            Dem_ReleaseClient(ClientId);
        }
        readoutBuffer.State = DEM_READOUTBUFFER_STATUS_UNUSED;
        Dem_ClientSetReadoutBuffer(dcmClientId, readoutBuffer);
        Dem_LeaveCritical_DcmApi();
        /** Leave Critical Section: DcmApi */
        ret = E_OK;
    }
    return ret;
}

/**
 * @brief Populate readout buffer with data.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
DEM_LOCAL void Dem_ReadoutBufferFillData(uint8 ClientId)
{
    uint8                      dcmClientId   = Dem_GetDcmClientIdOfClient(ClientId);
    Dem_ReadoutBufferEntryType ReadoutBuffer = Dem_ClientGetReadoutBuffer(dcmClientId);
    /** copy memory entry only for readout buffer in state QUEUED */
    if (ReadoutBuffer.State == DEM_READOUTBUFFER_STATUS_QUEUED)
    {
        Dem_EventIdType              eventId     = ReadoutBuffer.EventId;
        Dem_MemStateInfoConstPtrType memoryInfo  = Dem_MemStateInfoInit(ReadoutBuffer.MemIndex);
        Dem_NvBlockNumType           memoryIndex = Dem_MemoryFindIndex(memoryInfo, eventId);
        if (memoryIndex == DEM_MEM_INVALID_MEMORY_INDEX)
        {
            /** Enter Critical Section: DcmApi */
            Dem_EnterCritical_DcmApi();
            ReadoutBuffer = Dem_ClientGetReadoutBuffer(dcmClientId);
            if (ReadoutBuffer.State == DEM_READOUTBUFFER_STATUS_QUEUED)
            {
                ReadoutBuffer.State = DEM_READOUTBUFFER_STATUS_NOT_STORED;
                Dem_ClientSetReadoutBuffer(dcmClientId, ReadoutBuffer);
                Dem_ReleaseClient(ClientId);
            }
            Dem_LeaveCritical_DcmApi();
            /** Leave Critical Section: DcmApi */
        }
        else
        {
            Dem_MemCpy((uint8*)&ReadoutBuffer.Data, (uint8*)&Dem_GetMemoryEntry()[memoryIndex], sizeof(Dem_EntryType));
            /** Enter Critical Section: DcmApi */
            Dem_EnterCritical_DcmApi();
            ReadoutBuffer = Dem_ClientGetReadoutBuffer(dcmClientId);
            if (ReadoutBuffer.State == DEM_READOUTBUFFER_STATUS_QUEUED)
            {
                ReadoutBuffer.State = DEM_READOUTBUFFER_STATUS_IN_USE;
                Dem_ClientSetReadoutBuffer(dcmClientId, ReadoutBuffer);
                Dem_ReleaseClient(ClientId);
            }
            Dem_LeaveCritical_DcmApi();
            /** Leave Critical Section: DcmApi */
        }
    }
}
/* PRQA S 6070 -- */

/**
 * @brief Copy the memory entry contents into the ReadoutBuffer.
 */
DEM_LOCAL void Dem_FilterEventDataTask(void)
{
    for (uint8 clientId = 0u; clientId < DEM_CLIENT_NUMBER; ++clientId)
    {
        if (Dem_GetDcmClientIdOfClient(clientId) != DEM_CLIENT_NUMBER)
        {
            Dem_ReadoutBufferFillData(clientId);
        }
    }
}

/* PRQA S 3473, 4399, 4461 ++ */ /* VL_Dem_3473, VL_Dem_4399, VL_Dem_4461 */
/**
 * @brief         Set the active state of DTC setting
 * @param[in]     DTCSettingState: The DTC setting status
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_SetDTCSettingStateActive(uint8 DTCSettingState)
{
    uint8 state = DTCSettingState;
    DEM_FLAG_SET(state, DEM_DTCSETTING_ACTIVE);
    return state;
}

/**
 * @brief         Reset the active state of DTC setting
 * @param[in]     DTCSettingState: The DTC setting status
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_ResetDTCSettingStateActive(uint8 DTCSettingState)
{
    uint8 state = DTCSettingState;
    DEM_FLAG_RESET(state, DEM_DTCSETTING_ACTIVE);
    return state;
}

/**
 * @brief         Check the active state of DTC setting
 * @param[in]     DTCSettingState: The DTC setting status
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckDTCSettingStateActive(uint8 DTCSettingState)
{
    boolean ret = DEM_FLAG_ISSET(DTCSettingState, DEM_DTCSETTING_ACTIVE); /* PRQA S 4404 */ /* VL_Dem_4404 */
    return ret;
}

/**
 * @brief         Set the 'queued disable' state of DTC setting
 * @param[in]     DTCSettingState: The DTC setting status
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_SetDTCSettingStateQueuedDisable(uint8 DTCSettingState)
{
    uint8 state = DTCSettingState;
    DEM_FLAG_SET(state, DEM_DTCSETTING_QUEUE_DISABLE);
    return state;
}

/**
 * @brief         Reset the 'queued disable' state of DTC setting
 * @param[in]     DTCSettingState: The DTC setting status
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_ResetDTCSettingStateQueuedDisable(uint8 DTCSettingState)
{
    uint8 state = DTCSettingState;
    DEM_FLAG_RESET(state, DEM_DTCSETTING_QUEUE_DISABLE);
    return state;
}

/**
 * @brief         Check the 'queued disable' state of DTC setting
 * @param[in]     DTCSettingState: The DTC setting status
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckDTCSettingStateQueuedDisable(uint8 DTCSettingState)
{
    boolean ret = DEM_FLAG_ISSET(DTCSettingState, DEM_DTCSETTING_QUEUE_DISABLE); /* PRQA S 4404 */ /* VL_Dem_4404 */
    return ret;
}

/**
 * @brief         Set the 'process disable' state of DTC setting
 * @param[in]     DTCSettingState: The DTC setting status
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_SetDTCSettingStateProcessDisable(uint8 DTCSettingState)
{
    uint8 state = DTCSettingState;
    DEM_FLAG_SET(state, DEM_DTCSETTING_PROCESS_DISABLE);
    return state;
}

/**
 * @brief         Reset the 'process disable' state of DTC setting
 * @param[in]     DTCSettingState: The DTC setting status
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_ResetDTCSettingStateProcessDisable(uint8 DTCSettingState)
{
    uint8 state = DTCSettingState;
    DEM_FLAG_RESET(state, DEM_DTCSETTING_PROCESS_DISABLE);
    return state;
}

/**
 * @brief         Check the 'process disable' state of DTC setting
 * @param[in]     DTCSettingState: The DTC setting status
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckDTCSettingStateProcessDisable(uint8 DTCSettingState)
{
    boolean ret = DEM_FLAG_ISSET(DTCSettingState, DEM_DTCSETTING_PROCESS_DISABLE); /* PRQA S 4404 */ /* VL_Dem_4404 */
    return ret;
}

/**
 * @brief         Set the 'queued enable' state of DTC setting
 * @param[in]     DTCSettingState: The DTC setting status
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_SetDTCSettingStateQueuedEnable(uint8 DTCSettingState)
{
    uint8 state = DTCSettingState;
    DEM_FLAG_SET(state, DEM_DTCSETTING_QUEUE_ENABLE);
    return state;
}

/**
 * @brief         Reset the 'queued enable' state of DTC setting
 * @param[in]     DTCSettingState: The DTC setting status
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_ResetDTCSettingStateQueuedEnable(uint8 DTCSettingState)
{
    uint8 state = DTCSettingState;
    DEM_FLAG_RESET(state, DEM_DTCSETTING_QUEUE_ENABLE);
    return state;
}

/**
 * @brief         Check the 'queued enable' state of DTC setting
 * @param[in]     DTCSettingState: The DTC setting status
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckDTCSettingStateQueuedEnable(uint8 DTCSettingState)
{
    boolean ret = DEM_FLAG_ISSET(DTCSettingState, DEM_DTCSETTING_QUEUE_ENABLE); /* PRQA S 4404 */ /* VL_Dem_4404 */
    return ret;
}

/**
 * @brief         Set the 'process enable' state of DTC setting
 * @param[in]     DTCSettingState: The DTC setting status
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_SetDTCSettingStateProcessEnable(uint8 DTCSettingState)
{
    uint8 state = DTCSettingState;
    DEM_FLAG_SET(state, DEM_DTCSETTING_PROCESS_ENABLE);
    return state;
}

/**
 * @brief         Reset the 'process enable' state of DTC setting
 * @param[in]     DTCSettingState: The DTC setting status
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_ResetDTCSettingStateProcessEnable(uint8 DTCSettingState)
{
    uint8 state = DTCSettingState;
    DEM_FLAG_RESET(state, DEM_DTCSETTING_PROCESS_ENABLE);
    return state;
}
/* PRQA S 3473, 4399, 4461 -- */

/**
 * @brief         Check the 'process enable' state of DTC setting
 * @param[in]     DTCSettingState: The DTC setting status
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckDTCSettingStateProcessEnable(uint8 DTCSettingState)
{
    boolean ret = DEM_FLAG_ISSET(DTCSettingState, DEM_DTCSETTING_PROCESS_ENABLE); /* PRQA S 4404 */ /* VL_Dem_4404 */
    return ret;
}

/**
 * @brief         Set DTC state
 * @param[in]     DcmClientId: Identification of a client.
 * @param[in]     DTCSettingState: The DTC setting status
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetDTCSettingState(uint8 DcmClientId, uint8 DTCSettingState)
{
    Dem_GetClientSetting()[DcmClientId] = DTCSettingState;
}

/**
 * @brief         Get DTC state
 * @param[in]     DcmClientId: Identification of a client.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetDTCSettingState(uint8 DcmClientId)
{
    return Dem_GetClientSetting()[DcmClientId];
}

/**
 * @brief Check the DTC Setting state of Event
 */
DEM_LOCAL boolean Dem_CheckEventDTCSettingState(Dem_EventIdType EventId)
{
    boolean                ret         = FALSE;
    Dem_EventClientRefType clientStart = Dem_GetClientStartRefOfEvent(EventId);
    Dem_EventClientRefType clientEnd   = Dem_GetClientEndRefOfEvent(EventId);
    for (; clientStart < clientEnd; ++clientStart)
    {
        uint8 dcmClientId = Dem_GetDcmClientIdOfClient(Dem_GetClientOfEventRef(clientStart));
        if (dcmClientId != DEM_CLIENT_NUMBER)
        {
            uint8 DTCSettingState = Dem_GetDTCSettingState(dcmClientId);
            if (DEM_FLAG_ISSET(DTCSettingState, DEM_DTCSETTING_ACTIVE))
            {
                ret = TRUE;
                break;
            }
        }
    }
    return ret;
}

/**
 * @brief Enable the DTC setting State
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
DEM_LOCAL void Dem_DTCSettingEnable(uint8 ClientId)
{
    uint8   dcmClientId        = Dem_GetDcmClientIdOfClient(ClientId);
    boolean lDTCSettingPending = FALSE;
    uint8   lDTCSettingState;
    /** Enter Critical Section: StateManager */
    Dem_EnterCritical_StateManager();
    lDTCSettingState = Dem_GetDTCSettingState(dcmClientId);
    if (Dem_CheckDTCSettingStateActive(lDTCSettingState) == TRUE)
    {
        lDTCSettingState = Dem_SetDTCSettingStateQueuedEnable(lDTCSettingState);
        lDTCSettingState = Dem_ResetDTCSettingStateActive(lDTCSettingState);
        Dem_SetDTCSettingState(dcmClientId, lDTCSettingState);
        lDTCSettingPending = TRUE;
    }
    Dem_LeaveCritical_StateManager();
    /** Leave Critical Section: StateManager */

    if (lDTCSettingPending == TRUE)
    {
        Dem_EnableTaskOnce(Dem_Task_DTCSetting);
    }
}

/**
 * @brief Disable the DTC setting
 */
DEM_LOCAL void Dem_DTCSettingDisable(uint8 ClientId)
{
    uint8   dcmClientId        = Dem_GetDcmClientIdOfClient(ClientId);
    boolean lDTCSettingPending = FALSE;
    uint8   lDTCSettingState;
    /** Enter Critical Section: StateManager */
    Dem_EnterCritical_StateManager();
    lDTCSettingState = Dem_GetDTCSettingState(dcmClientId);
    if (Dem_CheckDTCSettingStateActive(lDTCSettingState) == FALSE)
    {
        lDTCSettingState = Dem_SetDTCSettingStateQueuedDisable(lDTCSettingState);
        lDTCSettingState = Dem_SetDTCSettingStateActive(lDTCSettingState);
        Dem_SetDTCSettingState(dcmClientId, lDTCSettingState);
        lDTCSettingPending = TRUE;
    }
    Dem_LeaveCritical_StateManager();
    /** Leave Critical Section: StateManager */

    if (lDTCSettingPending == TRUE)
    {
        Dem_EnableTaskOnce(Dem_Task_DTCSetting);
    }
}
/* PRQA S 6070 -- */

/**
 * @brief Processes changes of DTC setting
 */
DEM_LOCAL void Dem_ProcessQueueEventDTCSetting(Dem_EventIdType EventId, uint8 DcmClientId)
{
    uint8 lDTCSettingState = Dem_GetDTCSettingState(DcmClientId);
    /** Reset/Freeze debouncing on DTC setting state change unlocked -> locked */
    if (Dem_CheckDTCSettingStateProcessDisable(lDTCSettingState) == TRUE)
    {
        /** DTC setting become disabled.
         * If the event is configured to freeze: freeze it
         * Otherwise: reset it
         */
        if (Dem_GetDebounceBehaviorByEvent(EventId) == FALSE)
        {
            Dem_SatelliteFreezeDebounceStatus(EventId);
        }
        else
        {
            Dem_SatelliteResetDebounceStatus(EventId);
        }
    }

    if (Dem_CheckDTCSettingStateProcessEnable(lDTCSettingState) == TRUE)
    {
        Dem_NotiInitMonitorForEvent(EventId, DEM_INIT_MONITOR_REENABLED);
    }
}

/**
 * @brief DTC setting Process.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
DEM_LOCAL void Dem_DTCSettingProcess(uint8 ClientId)
{
    Dem_EventIdType eventId;
    Dem_EventIdType lastEvent;
    uint8           lDTCSettingState;

    uint8 dcmClientId = Dem_GetDcmClientIdOfClient(ClientId);
    /** Enter Critical Section: StateManager */
    Dem_EnterCritical_StateManager();
    lDTCSettingState = Dem_GetDTCSettingState(dcmClientId);
    if (Dem_CheckDTCSettingStateQueuedDisable(lDTCSettingState) == TRUE)
    {
        lDTCSettingState = Dem_SetDTCSettingStateProcessDisable(lDTCSettingState);
        lDTCSettingState = Dem_ResetDTCSettingStateQueuedDisable(lDTCSettingState);
    }
    if (Dem_CheckDTCSettingStateQueuedEnable(lDTCSettingState) == TRUE)
    {
        lDTCSettingState = Dem_SetDTCSettingStateProcessEnable(lDTCSettingState);
        lDTCSettingState = Dem_ResetDTCSettingStateQueuedEnable(lDTCSettingState);
    }
    Dem_SetDTCSettingState(dcmClientId, lDTCSettingState);
    Dem_LeaveCritical_StateManager();
    /** Leave Critical Section: StateManager */

    lastEvent = Dem_GetLastEventOfClient(ClientId);
    for (eventId = Dem_GetFirstEventOfClient(ClientId); eventId < lastEvent; ++eventId)
    {
        if ((Dem_GetAvailableInVariantByEvent(eventId) == TRUE)
            && (Dem_GetEventAvailableStatusByEventId(eventId) == TRUE))
        {
            /** Process DTC Setting for every event */
            Dem_ProcessQueueEventDTCSetting(eventId, dcmClientId);
        }
    }

    /** Enter Critical Section: StateManager */
    Dem_EnterCritical_StateManager();
    /** Remove process flags */
    lDTCSettingState = Dem_GetDTCSettingState(dcmClientId);
    lDTCSettingState = Dem_ResetDTCSettingStateProcessDisable(lDTCSettingState);
    lDTCSettingState = Dem_ResetDTCSettingStateProcessEnable(lDTCSettingState);
    Dem_SetDTCSettingState(dcmClientId, lDTCSettingState);
    Dem_LeaveCritical_StateManager();
    /** Leave Critical Section: StateManager */
}
/* PRQA S 6070 -- */

/**
 * @brief DTC setting Task.
 */
DEM_LOCAL void Dem_DTCSettingTask(void)
{
    for (uint8 clientId = 0u; clientId < DEM_CLIENT_NUMBER; ++clientId)
    {
        if (Dem_GetDcmClientIdOfClient(clientId) != DEM_CLIENT_NUMBER)
        {
            Dem_DTCSettingProcess(clientId);
        }
    }
}
#endif

/**
 * @brief         Initializes the Client Info for all clients.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_InitClientInfo(void)
{
    for (uint8 clientId = 0u; clientId < DEM_CLIENT_NUMBER; ++clientId)
    {
        Dem_InitClientInfoByClientId(clientId);
#if (DEM_DCM_CLIENT_NUMBER > 0u)
        Dem_ClientReadoutBufferInit(clientId);
#endif
        Dem_FilterDataInitDefault(clientId);
        Dem_FFIteratorInitIterator(clientId);
    }
#if (DEM_DCM_CLIENT_NUMBER > 0u)
    Dem_InitClientDTCSetting();
#endif
}

#if (DEM_SUPPRESSION_SUPPORT == DEM_DTC_SUPPRESSION)
/**
 * @brief         Suppress event or release event suppression as requested by the client
 */
DEM_LOCAL Std_ReturnType Dem_ClientGetDTCSuppression(
    uint8    ClientId,
    boolean* SuppressionStatus
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    ,
    uint8* ErrorId
#endif
)
{
    Std_ReturnType ret = E_NOT_OK;
    switch (Dem_GetClientResult(ClientId))
    {
    case DEM_CLIENT_RESULT_DTC:
    {
        Dem_EventIdType eventId = Dem_ClientInfo[ClientId].Selection.EventId;
        if (eventId != DEM_EVENT_INVALID)
        {
            *SuppressionStatus = Dem_CheckDTCSuppression(eventId);
            ret                = E_OK;
        }
        break;
    }
    case DEM_CLIENT_RESULT_GROUP:
    case DEM_CLIENT_RESULT_ALL_DTCS:
    case DEM_CLIENT_RESULT_NO_MATCH:
        ret = DEM_WRONG_DTC;
        break;
    case DEM_CLIENT_RESULT_WRONG_ORIGIN:
        ret = DEM_WRONG_DTCORIGIN;
        break;
    case DEM_CLIENT_RESULT_INIT:
        /** E_NOT_OK */
#if (DEM_DEV_ERROR_DETECT == STD_ON)
        *ErrorId = DEM_E_WRONG_CONDITION;
#endif
        break;
    default:
        /** E_NOT_OK */
        break;
    }
    return ret;
}
#endif

/**
 * @brief         Gets the event memory overflow indication status.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module
 * @param[in]     DTCOrigin: The event memory of the requested DTC or group of DTC.
 * @param[out]    OverflowIndication: This parameter returns TRUE if the according event memory was overflowed,
 *                                    otherwise it returns FALSE.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Std_ReturnType
    Dem_ClientGetEventMemoryOverflow(uint8 ClientId, Dem_DTCOriginType DTCOrigin, boolean* OverflowIndication)
{
    Std_ReturnType    ret = E_NOT_OK;
    Dem_MemoryNumType memIndex;
    if (Dem_DTCOriginCheckValidOnClient(ClientId, DTCOrigin, &memIndex) == TRUE)
    {
        *OverflowIndication = Dem_GetMemoryOverflow(memIndex);
        ret                 = E_OK;
    }
    return ret;
}

/**
 * @brief         Returns the number of entries currently stored in the requested event memory.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module
 * @param[in]     DTCOrigin: The event memory of the requested DTC or group of DTC.
 * @param[out]    NumberOfEventMemoryEntries: Number of entries currently stored in the requested event memory.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Std_ReturnType Dem_ClientGetNumberOfEventMemoryEntries(
    uint8             ClientId,
    Dem_DTCOriginType DTCOrigin,
    uint8*            NumberOfEventMemoryEntries)
{
    Std_ReturnType    ret = E_NOT_OK;
    Dem_MemoryNumType memIndex;
    if (Dem_DTCOriginCheckValidOnClient(ClientId, DTCOrigin, &memIndex) == TRUE)
    {
        Dem_MemStateInfoConstPtrType memoryInfo = Dem_MemStateInfoInit(memIndex);
        *NumberOfEventMemoryEntries             = Dem_GetCurSizeOfMemoryInfo(memoryInfo);
        ret                                     = E_OK;
    }
    return ret;
}

#if (DEM_SUPPRESSION_SUPPORT == DEM_DTC_SUPPRESSION)
/**
 * @brief Suppress event or release event suppression as requested by the client
 */
DEM_LOCAL Std_ReturnType Dem_ClientSetDTCSuppression(uint8 ClientId, boolean SuppressionStatus)
{
    Std_ReturnType  ret     = E_NOT_OK;
    Dem_EventIdType eventId = Dem_ClientInfo[ClientId].Selection.EventId;
    if (eventId != DEM_EVENT_INVALID)
    {
        Dem_ChangeDTCSuppression(eventId, SuppressionStatus);
        ret = E_OK;
    }
    return ret;
}
#endif

#if (DEM_DCM_CLIENT_NUMBER > 0u)
/**
 * @brief Gets the DTC Status availability mask of the selected fault memory.
 */
DEM_LOCAL Std_ReturnType Dem_ClientGetDTCStatusAvailabilityMask(
    uint8                  ClientId,
    Dem_UdsStatusByteType* DTCStatusMask,
    Dem_DTCOriginType      DTCOrigin)
{
    Std_ReturnType    ret = E_NOT_OK;
    Dem_MemoryNumType memIndex;
    if (Dem_DTCOriginCheckValidOnClient(ClientId, DTCOrigin, &memIndex) == TRUE)
    {
        *DTCStatusMask = Dem_GetDTCStatusMaskByMem(memIndex);
        ret            = E_OK;
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    else
    {
        /** ***** Development Error Report ***** */
        Dem_DetReportError(DEM_SID_SETDTCFILTER, DEM_E_WRONG_CONFIGURATION);
    }
#endif
    return ret;
}

/**
 * @brief Get the current Uds status of a selected DTC
 */
DEM_LOCAL Std_ReturnType Dem_ClientGetStatusOfDTC(
    uint8  ClientId,
    uint8* DTCStatus
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    ,
    uint8* ErrorId
#endif
)
{
    Std_ReturnType ret = E_NOT_OK;
    switch (Dem_GetClientResult(ClientId))
    {
    case DEM_CLIENT_RESULT_DTC:
    {
        const Dem_ClientInfoType* clientInfo = &(Dem_ClientInfo[ClientId]);
        Dem_MemoryNumType         memIndex   = clientInfo->MemIndex;
        Dem_EventIdType           eventId    = clientInfo->Selection.EventId;
        if (eventId != DEM_EVENT_INVALID)
        {
            if (Dem_CheckDTCSuppressed(eventId) == TRUE)
            {
                ret = DEM_WRONG_DTC;
            }
            else
            {
                Dem_UdsStatusByteType eventStatus = Dem_DTCApplyExternalStatus(eventId, Dem_GetDTCUDSStatus(eventId));
                /** apply status availability mask */
                *DTCStatus = eventStatus & Dem_GetDTCStatusMaskByMem(memIndex);
                ret        = E_OK;
            }
        }
    }
    break;
    case DEM_CLIENT_RESULT_GROUP:
    case DEM_CLIENT_RESULT_ALL_DTCS:
    case DEM_CLIENT_RESULT_NO_MATCH:
        ret = DEM_WRONG_DTC;
        break;
    case DEM_CLIENT_RESULT_WRONG_ORIGIN:
        ret = DEM_WRONG_DTCORIGIN;
        break;
    case DEM_CLIENT_RESULT_INIT:
        /** E_NOT_OK */
#if (DEM_DEV_ERROR_DETECT == STD_ON)
        *ErrorId = DEM_E_WRONG_CONDITION;
#endif
        break;
    default:
        /** E_NOT_OK */
        break;
    }
    return ret;
}

#if (DEM_FEATURE_DCM_0X19_0X09_ENABLE == STD_ON)
/**
 * @brief Get the severity of a selected DTC
 */
DEM_LOCAL Std_ReturnType Dem_ClientGetSeverityOfDTC(
    uint8                ClientId,
    Dem_DTCSeverityType* DTCSeverity
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    ,
    uint8* ErrorId
#endif
)
{
    Std_ReturnType ret = E_NOT_OK;
    switch (Dem_GetClientResult(ClientId))
    {
    case DEM_CLIENT_RESULT_DTC:
    {
        const Dem_ClientInfoType* clientInfo = &(Dem_ClientInfo[ClientId]);
        Dem_EventIdType           eventId    = clientInfo->Selection.EventId;
        if (eventId != DEM_EVENT_INVALID)
        {
            if (Dem_CheckDTCSuppressed(eventId) == TRUE)
            {
                ret = DEM_WRONG_DTC;
            }
            else
            {
                if (Dem_GetDTCRefOfEvent(eventId) != DEM_DTC_NUMBER)
                {
                    *DTCSeverity = Dem_GetCfgSeverityOfDTC(Dem_GetDTCRefOfEvent(eventId));
                    ret          = E_OK;
                }
            }
        }
    }
    break;
    case DEM_CLIENT_RESULT_GROUP:
    case DEM_CLIENT_RESULT_ALL_DTCS:
    case DEM_CLIENT_RESULT_NO_MATCH:
        ret = DEM_WRONG_DTC;
        break;
    case DEM_CLIENT_RESULT_WRONG_ORIGIN:
        ret = DEM_WRONG_DTCORIGIN;
        break;
    case DEM_CLIENT_RESULT_INIT:
        /** E_NOT_OK */
#if (DEM_DEV_ERROR_DETECT == STD_ON)
        *ErrorId = DEM_E_WRONG_CONDITION;
#endif
        break;
    default:
        /** E_NOT_OK */
        break;
    }
    return ret;
}

/**
 * @brief Get the functional unit of a selected DTC
 */
DEM_LOCAL Std_ReturnType Dem_ClientGetFunctionalUnitOfDTC(
    uint8  ClientId,
    uint8* DTCFunctionalUnit
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    ,
    uint8* ErrorId
#endif
)
{
    Std_ReturnType ret = E_NOT_OK;
    switch (Dem_GetClientResult(ClientId))
    {
    case DEM_CLIENT_RESULT_DTC:
    {
        const Dem_ClientInfoType* clientInfo = &(Dem_ClientInfo[ClientId]);
        Dem_EventIdType           eventId    = clientInfo->Selection.EventId;

        if (eventId != DEM_EVENT_INVALID)
        {
            if (Dem_CheckDTCSuppressed(eventId) == TRUE)
            {
                ret = DEM_WRONG_DTC;
            }
            else
            {
                if (Dem_GetDTCRefOfEvent(eventId) != DEM_DTC_NUMBER)
                {
                    *DTCFunctionalUnit = Dem_GetFUOfDTC(Dem_GetDTCRefOfEvent(eventId));
                    ret                = E_OK;
                }
            }
        }
    }
    break;
    case DEM_CLIENT_RESULT_GROUP:
    case DEM_CLIENT_RESULT_ALL_DTCS:
    case DEM_CLIENT_RESULT_NO_MATCH:
        ret = DEM_WRONG_DTC;
        break;
    case DEM_CLIENT_RESULT_WRONG_ORIGIN:
        ret = DEM_WRONG_DTCORIGIN;
        break;
    case DEM_CLIENT_RESULT_INIT:
        /** E_NOT_OK */
#if (DEM_DEV_ERROR_DETECT == STD_ON)
        *ErrorId = DEM_E_WRONG_CONDITION;
#endif
        break;
    default:
        /** E_NOT_OK */
        break;
    }
    return ret;
}
#endif

#if (DEM_CLIENT_REPORT_EVENT_SIZE > 0u)
/**
 * @brief         Clear the bitfield of reported events for the given filter.
 * @param[in]     DcmClientId: Unique client id, assigned to the instance of the calling module
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_FilterDataResetReportedEvents(uint8 DcmClientId)
{
    Dem_MemSet((uint8*)&Dem_ReportedEventsOfFilter[DcmClientId], 0u, sizeof(Dem_ReportedEventsType));
}
#endif

/**
 * @brief Initialize a filter for a 'filter DTC' request.
 */
DEM_LOCAL void
    Dem_FilterDataInit(uint8 ClientId, Dem_DTCOriginType DTCOrigin, Dem_MemoryNumType MemIndex, uint8 StatusMask)
{
    Dem_FilterDataInfoType filterData = Dem_GetFilterData(ClientId);

    if (DTCOrigin != DEM_DTC_ORIGIN_PERMANENT_MEMORY)
    {
        filterData.MemIndex = MemIndex;
#if (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_ON)
        if ((StatusMask == DEM_UDS_STATUS_PDTC) || (StatusMask == DEM_UDS_STATUS_CDTC)
            || (StatusMask == DEM_UDS_STATUS_CDTC_PDTC))
        {
            /** reporting shall be in chronological order */
            filterData.Iterator.ChronoIndex = 0u;
            filterData.FilterDtcStrategy    = Dem_FilterDataFilterChronological;
        }
        else
#endif
        {
            /** report by event id, set first event index */
            filterData.Iterator.EventIndex = Dem_GetMemStateInfo()[MemIndex].FirstEvent;
            filterData.FilterDtcStrategy   = Dem_FilterDataFilterEventBased;
        }
    }
    else
    {
        /** report by event id, set max number of permanent memory entries */
        uint8 eventMemorySetRef            = Dem_GetMemorySetRefOfClient(ClientId);
        filterData.MemIndex                = eventMemorySetRef;
        filterData.Iterator.PermanentIndex = Dem_GetPermanentNumOfMemSet(eventMemorySetRef);
        filterData.FilterDtcStrategy       = Dem_FilterDataFilterEventBasedPermanentMemory;
    }
    Dem_SetFilterData(ClientId, filterData);
}

/* PRQA S 3473, 4399, 4461,6040 ++ */ /* VL_Dem_3473, VL_Dem_4399, VL_Dem_4461,VL_MTR_Dem_STPAR */
/**
 * @brief Initializes a DTC filter with the given criteria.
 */
DEM_LOCAL Std_ReturnType Dem_ClientSetDTCFilter(
    uint8               ClientId,
    uint8               DTCStatusMask,
    Dem_DTCFormatType   DTCFormat,
    Dem_DTCOriginType   DTCOrigin,
    boolean             FilterWithSeverity,
    Dem_DTCSeverityType DTCSeverityMask,
    boolean             FilterForFaultDetectionCounter)
{
    Dem_FilterDataInfoType filterData;
    Std_ReturnType         ret         = E_OK;
    uint8                  dcmClientId = Dem_GetDcmClientIdOfClient(ClientId);
    Dem_MemoryNumType      memIndex;

    if (Dem_DTCOriginCheckValidOnClient(ClientId, DTCOrigin, &memIndex) == TRUE)
    {
        Dem_ClientInfo[ClientId].MemIndex = memIndex;
        Dem_FilterDataInit(dcmClientId, DTCOrigin, memIndex, DTCStatusMask);

#if (DEM_CLIENT_REPORT_EVENT_SIZE > 0u)
        /** Clear list of identified and reported events */
        Dem_FilterDataResetReportedEvents(dcmClientId);
#endif

        filterData = Dem_GetFilterData(dcmClientId);
        /** Set the filter state in the given filter object */
        filterData.FilterState = DEM_FILTERDATA_FILTER_DTC;
        /** Check if filter shall ignore Dtc status or not */
        if (DTCStatusMask == 0x00u)
        {
            DEM_FLAG_RESET(filterData.FilterFlags, DEM_FILTERDATA_FILTERFLAGS_USESTATUSMASK);
        }
        else
        {
            Dem_UdsStatusByteType mask;
            ret = Dem_ClientGetDTCStatusAvailabilityMask(ClientId, &mask, DTCOrigin);
            if (ret == E_OK)
            {
                filterData.StatusMask = DTCStatusMask & mask;
                DEM_FLAG_SET(filterData.FilterFlags, DEM_FILTERDATA_FILTERFLAGS_USESTATUSMASK);
            }
        }

        /** Check if filter shall use severity */
        if (FilterWithSeverity == TRUE)
        {
            filterData.SeverityMask = DTCSeverityMask & DEM_SEVERITY_MASK;
            DEM_FLAG_SET(filterData.FilterFlags, DEM_FILTERDATA_FILTERFLAGS_USESEVERITY);
        }
        else
        {
            DEM_FLAG_RESET(filterData.FilterFlags, DEM_FILTERDATA_FILTERFLAGS_USESEVERITY);
        }

        /** Check if filter shall use fault detection counter */
        if (FilterForFaultDetectionCounter == TRUE)
        {
            DEM_FLAG_SET(filterData.FilterFlags, DEM_FILTERDATA_FILTERFLAGS_USEFDC);
        }
        else
        {
            DEM_FLAG_RESET(filterData.FilterFlags, DEM_FILTERDATA_FILTERFLAGS_USEFDC);
        }

        /** Check if filtering for emission relevant DTCs was requested */
        if (DTCOrigin == DEM_DTC_ORIGIN_OBD_RELEVANT_MEMORY)
        {
#if (DEM_OBDII_SUPPORT == STD_ON)
            DEM_FLAG_SET(filterData.FilterFlags, DEM_FILTERDATA_FILTERFLAGS_FILTEREMISSIONRELDTC);
#else
#if (DEM_DEV_ERROR_DETECT == STD_ON)
            /** ***** Development Error Report ***** */
            Dem_DetReportError(DEM_SID_SETDTCFILTER, DEM_E_WRONG_CONFIGURATION);
#endif
#endif
        }
        else
        {
            DEM_FLAG_RESET(filterData.FilterFlags, DEM_FILTERDATA_FILTERFLAGS_FILTEREMISSIONRELDTC);
        }

        /** Check if output format of DTCs shall be OBD */
        if ((DTCFormat == DEM_DTC_FORMAT_OBD)
#if (DEM_SUPPORTED_OBD_UDS_DTC_SEPARATION == STD_ON)
            || (DTCFormat == DEM_DTC_FORMAT_OBD_3BYTE)
#endif
        )
        {
#if (DEM_OBDII_SUPPORT == STD_ON)
            DEM_FLAG_SET(filterData.FilterFlags, DEM_FILTERDATA_FILTERFLAGS_USEFORMATOBD);
#else
#if (DEM_DEV_ERROR_DETECT == STD_ON)
            /** ***** Development Error Report ***** */
            Dem_DetReportError(DEM_SID_SETDTCFILTER, DEM_E_WRONG_CONFIGURATION);
#endif
#endif
        }
#if (DEM_OBDII_SUPPORT == STD_ON)
        else
        {
            DEM_FLAG_RESET(filterData.FilterFlags, DEM_FILTERDATA_FILTERFLAGS_USEFORMATOBD);
        }
#endif
        Dem_SetFilterData(dcmClientId, filterData);
    }
    return ret;
}
/* PRQA S 3473, 4399, 4461,6040 -- */

#if (DEM_OBDII_SUPPORT == STD_ON)
/**
 * @brief         Calculates the number of filtered DTCs for permanent memory.
 */
DEM_LOCAL uint16 Dem_FilterDataCountMatchesInPermanentMemory(uint8 ClientId)
{
    uint16 numberOfEvents    = 0;
    uint8  eventMemorySetRef = Dem_GetMemorySetRefOfClient(ClientId);
    uint8  permanentIndex    = Dem_GetPermanentStartOfMemSet(eventMemorySetRef);
    uint8  permanentEnd      = Dem_GetPermanentNumOfMemSet(eventMemorySetRef) + permanentIndex;

    /** scan the permanent memory for visible events */
    for (; permanentIndex < permanentEnd; ++permanentIndex)
    {
        if (DEM_FLAG_ISSET(Dem_GetstateOfMemEntry(permanentIndex), DEM_PERMANENT_VISIBLE) == TRUE)
        {
            Dem_EventIdType EventId = Dem_GetEventIdOfMemEntry(permanentIndex);
            if ((EventId != DEM_EVENT_INVALID)
#if (DEM_INDICATOR_MIL_ENABLE == STD_ON)
                && (Dem_EventCheckHaveSpecialIndicator(EventId, Dem_GetMILIndOfMemSet(eventMemorySetRef)) == TRUE)
#endif
            )
            {
                ++numberOfEvents;
            }
        }
    }
    return numberOfEvents;
}
#endif

/* PRQA S 1258,6030,6070 ++ */ /* VL_Dem_1258,VL_MTR_Dem_STMIF,VL_MTR_Dem_STCAL */
/**
 * @brief Test whether a given EventId matches the filter.
 */
DEM_LOCAL Std_ReturnType Dem_FilterDataFilterForEventMatch(uint8 DcmClientId, Dem_EventIdType EventId)
{
    Std_ReturnType ret = E_OK;
    /** Suppressed DTCs are skipped */
    /* PRQA S 1252 ++ */ /* VL_Dem_1252 */
    if ((Dem_GetEventDTC(EventId) != DEM_DTC_UDS_INVALID) && (Dem_CheckDTCSuppressed(EventId) == FALSE))
    /* PRQA S 1252 -- */
    {
        Dem_UdsStatusByteType eventStatus = Dem_DTCApplyExternalStatus(EventId, Dem_GetDTCUDSStatus(EventId));
        /** Skip DTCs not matching the requested status mask */
        if ((Dem_FilterDataCheckUseStatusMask(DcmClientId) == TRUE)
            && ((Dem_FilterDataGetStatusMask(DcmClientId) & eventStatus) == 0u))
        {
            ret = E_NOT_OK;
        }
        else
        {
            /** Skip DTCs not matching the requested severity */
            if ((Dem_FilterDataCheckUseSeverity(DcmClientId) == TRUE)
                && ((
                    (Dem_FilterDataGetSeverityMask(DcmClientId)
                     & Dem_GetCfgSeverityOfDTC(Dem_GetDTCRefOfEvent(EventId)))
                    == 0u)))
            {
                ret = E_NOT_OK;
            }
            else
            {
                if (Dem_FilterDataCheckUseFdc(DcmClientId) == TRUE)
                {
                    sint8 fdc;
                    /** If FDC cannot be retrieved, the DTC does not match */
                    if (Dem_DTCGetFaultDetectionCounter(EventId, &fdc) != E_OK)
                    {
                        fdc = 0;
                    }
                    /** store the matching FDC for later */
                    Dem_FilterDataInfoType filterData = Dem_GetFilterData(DcmClientId);
                    filterData.FaultDetectionCounter  = fdc;
                    Dem_SetFilterData(DcmClientId, filterData);
                }
            }
        }
        /** If all tests pass, keep this DTC for processing */
    }
    else
    {
        ret = E_NOT_OK;
    }
    return ret;
}
/* PRQA S 1258,6030,6070 -- */

/**
 * @brief Calculates the number of filtered events in a standard event memory.
 */
DEM_LOCAL uint16 Dem_FilterDataCountMatchesInEventMemory(uint8 DcmClientId)
{
    uint16                      numberOfEvents = 0u;
    const Dem_MemStateInfoType* memoryInfo     = Dem_MemStateInfoInit(Dem_FilterDataGetMemIndex(DcmClientId));
    if (memoryInfo->FirstEvent != DEM_EVENT_INVALID)
    {
        Dem_EventIdType lastEvent = memoryInfo->LastEvent;
        Dem_EventIdType eventId   = memoryInfo->FirstEvent;
        /** at least one event is available which may match the filter criteria */
        for (; eventId < lastEvent; ++eventId)
        {
            if (Dem_FilterDataFilterForEventMatch(DcmClientId, eventId) == E_OK)
            {
                ++numberOfEvents;
            }
        }
    }
    return numberOfEvents;
}

/**
 * @brief Fetch number of DTCs matching the client specific filter settings.
 */
DEM_LOCAL uint16 Dem_ClientFilterNumberMemory(uint8 ClientId)
{
    uint16 numberOfEvents;
    uint8  dcmClientId = Dem_GetDcmClientIdOfClient(ClientId);
    if (Dem_FilterDataGetDTCFilteringStrategy(dcmClientId) == Dem_FilterDataFilterEventBasedPermanentMemory)
    {
#if (DEM_OBDII_SUPPORT == STD_ON)
        numberOfEvents = Dem_FilterDataCountMatchesInPermanentMemory(ClientId);
#else
        numberOfEvents = 0u;
#endif
    }
    else
    {
        numberOfEvents = Dem_FilterDataCountMatchesInEventMemory(dcmClientId);
    }
    return numberOfEvents;
}

#if (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_ON)
/* PRQA S 1252 ++ */ /* VL_Dem_1252 */
/**
 * @brief Tests whether an event is already reported.
 */
DEM_LOCAL boolean Dem_FilterDataCheckEventReported(uint8 ClientId, Dem_EventIdType EventId)
{
    /** identified an Event which matches the filter criteria, mark event as reported */
    uint8                           dcmClientId = Dem_GetDcmClientIdOfClient(ClientId);
    const Dem_ComProcessorWordType* wordPtr     = &Dem_ReportedEventsOfFilter[dcmClientId][0];
    Dem_EventIdType                 bitPosition = EventId - Dem_GetFirstEventOfClient(ClientId);
    uint16 wordPosition = ((uint16)bitPosition / ((uint16)sizeof(Dem_ComProcessorWordType) * (uint16)DEM_BYTE_SIZE));
    bitPosition = (bitPosition % ((Dem_EventIdType)sizeof(Dem_ComProcessorWordType) * (Dem_EventIdType)DEM_BYTE_SIZE));
    /* PRQA S 4404, 4397, 1891 ++ */ /* VL_Dem_4404, VL_Dem_4397, VL_Dem_1891 */
    boolean ret = DEM_FLAG_ISSET(wordPtr[wordPosition], DEM_BIT(bitPosition));
    /* PRQA S 4404, 4397, 1891 -- */
    return ret;
}
/* PRQA S 1252 -- */
#endif

/**
 * @brief Iterates events in the event memory matching the Dcm DTC filter.
 */
DEM_LOCAL Dem_EventIdType Dem_FilterDataFilterMemoryByEvent(uint8 ClientId, const Dem_MemStateInfoType* MemoryInfo)
{
    uint8           dcmClientId = Dem_GetDcmClientIdOfClient(ClientId);
    Dem_EventIdType eventId     = Dem_FilterDataGetEventId(dcmClientId);
    Dem_EventIdType findEventId = DEM_EVENT_INVALID;

    /** if last EventId is not valid or was still analyzed skip the filtering */
    if (eventId != DEM_EVENT_INVALID)
    {
        Dem_FilterDataInfoType filterData;
        Dem_EventIdType        eventIdLast;
        eventIdLast = MemoryInfo->LastEvent;
        /** iterate through the EventIds and check filter criteria */
        for (; eventId < eventIdLast; eventId++)
        {
#if (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_ON)
            /** take event into account if it was not reported before */
            if (Dem_FilterDataCheckEventReported(ClientId, eventId) == FALSE)
#endif
            {
                /** EventId was not reported before, proceed */
                if (Dem_FilterDataFilterForEventMatch(dcmClientId, eventId) == E_OK)
                {
                    /** identified an Event which matches the filter criteria */
                    findEventId = eventId;
                    break;
                }
            }
        }
        filterData = Dem_GetFilterData(dcmClientId);
        if (eventId >= eventIdLast)
        {
            /** last EventId has been processed */
            filterData.Iterator.EventIndex = DEM_EVENT_INVALID;
        }
        else
        {
            /** write back the next event to process */
            filterData.Iterator.EventIndex = (Dem_EventIdType)(eventId + 1u);
        }
        Dem_SetFilterData(dcmClientId, filterData);
    }
    /** else FindEventId = DEM_EVENT_INVALID initially set */
    return findEventId;
}

#if (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_ON)
/**
 * @brief         Marks an event as reported in the corresponding filter's bitfield.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module
 * @param[in]     EventId: Event ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_FilterDataSetEventReported(uint8 ClientId, Dem_EventIdType EventId)
{
    /** identified an Event which matches the filter criteria, mark event as reported */
    uint8                     dcmClientId = Dem_GetDcmClientIdOfClient(ClientId);
    Dem_ComProcessorWordType* wordPtr     = &Dem_ReportedEventsOfFilter[dcmClientId][0];
    Dem_EventIdType           bitPosition = EventId - Dem_GetFirstEventOfClient(ClientId);
    /* PRQA S 4461 ++ */ /* VL_Dem_4461 */
    uint16 wordPosition = (bitPosition / (sizeof(Dem_ComProcessorWordType) * DEM_BYTE_SIZE));
    bitPosition         = (bitPosition % (sizeof(Dem_ComProcessorWordType) * DEM_BYTE_SIZE));
    /* PRQA S 4461 -- */
    /* PRQA S 3473, 1891, 4397 ++ */ /* VL_Dem_3473, VL_Dem_1891, VL_Dem_4397 */
    DEM_FLAG_SET(wordPtr[wordPosition], DEM_BIT(bitPosition));
    /* PRQA S 3473, 1891, 4397 -- */
}

/**
 * @brief If chronological reporting is required this function iterates  all events in order of occurrence.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
DEM_LOCAL Dem_EventIdType Dem_FilterDataFilterMemoryChrono(uint8 ClientId, const Dem_MemStateInfoType* MemoryInfo)
{
    Dem_EventIdType        eventId = DEM_EVENT_INVALID;
    Dem_FilterDataInfoType filterData;
    uint8                  dcmClientId = Dem_GetDcmClientIdOfClient(ClientId);
    uint8                  chronoIndex = Dem_FilterDataGetChronoIndex(dcmClientId);

    /** index set to 0 indicates that the filter was reset */
    if (chronoIndex == 0u)
    {
        chronoIndex = *MemoryInfo->SizePtr;
    }
    /** else */ /** last stored index initially loaded */

    while (chronoIndex != 0u)
    {
        --chronoIndex;
        /** Get EventId for current index and marker if event was reported before */
        eventId = Dem_GetEventIdOfMemEntry(Dem_GetChronologyOfMemoryInfo(MemoryInfo, chronoIndex));
        /** verify that a valid EventId was returned */
        if (Dem_CheckEventIdInValid(eventId) == FALSE)
        {
            /** EventId was not reported before, proceed */
            if (Dem_FilterDataFilterForEventMatch(dcmClientId, eventId) == E_OK)
            {
                /** identified an Event which matches the filter criteria, mark event as reported */
                Dem_FilterDataSetEventReported(ClientId, eventId);
                break;
            }
            else
            { /** No filter match, continue with loop */
                eventId = DEM_EVENT_INVALID;
            }
        }
    }

    filterData = Dem_GetFilterData(dcmClientId);

    /** If end of chrono stack is reached, switch to filter by event id */
    if (chronoIndex == 0u)
    {
        filterData.Iterator.EventIndex = MemoryInfo->FirstEvent;
        filterData.FilterDtcStrategy   = Dem_FilterDataFilterEventBased;
        Dem_SetFilterData(dcmClientId, filterData);
        if (eventId == DEM_EVENT_INVALID)
        {
            /** no valid match, proceed with filtering by EventId */
            eventId = Dem_FilterDataFilterMemoryByEvent(dcmClientId, MemoryInfo);
        }
    }
    else
    {
        /** store current position of chrono stack */
        filterData.Iterator.ChronoIndex = chronoIndex;
        Dem_SetFilterData(dcmClientId, filterData);
    }
    return eventId;
}
/* PRQA S 6070 -- */
#endif

#if (DEM_OBDII_SUPPORT == STD_ON)
/**
 * @brief         Iterates events in permanent memory matching the Dcm DTC filter.
 */
DEM_LOCAL Dem_EventIdType Dem_FilterDataFilterPermanentByEvent(uint8 ClientId, const Dem_MemStateInfoType* MemoryInfo)
{
    uint8                  dcmClientId    = Dem_GetDcmClientIdOfClient(ClientId);
    Dem_FilterDataInfoType filterData     = Dem_GetFilterData(dcmClientId);
    uint8                  permanentIndex = filterData.Iterator.PermanentIndex;
    Dem_EventIdType        eventId        = DEM_EVENT_INVALID;

    /** if last EventId is not valid or was still analysed skip the filtering */
    while (permanentIndex != 0u)
    {
        --permanentIndex;
        if (DEM_FLAG_ISSET(Dem_GetstateOfMemEntry(permanentIndex), DEM_PERMANENT_VISIBLE) == TRUE)
        {
            /** Visible Event identified, return it and stop processing */
            uint8 eventMemorySetRef = Dem_GetMemorySetRefOfClient(ClientId);
            eventId                 = Dem_GetEventIdOfMemEntry(permanentIndex);
            if ((eventId != DEM_EVENT_INVALID)
#if (DEM_INDICATOR_MIL_ENABLE == STD_ON)
                && (Dem_EventCheckHaveSpecialIndicator(eventId, Dem_GetMILIndOfMemSet(eventMemorySetRef)) == TRUE)
#endif
            )
            {
                break;
            }
        }
    }
    /** store current position in permanent memory */
    filterData.Iterator.PermanentIndex = permanentIndex;
    Dem_SetFilterData(dcmClientId, filterData);
    return eventId;
}
#endif

/**
 * @brief Get the DTC filter function corresponding to the DTC filtering strategy.
 */
DEM_LOCAL Dem_FilterDataFuncPtrType
    Dem_FilterDataGetDTCFilterFunction(Dem_FilterDataDTCFilteringStrategyType DTCFilteringStrategy)
{
    Dem_FilterDataFuncPtrType lDTCFilterFunction; /* PRQA S 3672 */ /* VL_Dem_3672 */
    switch (DTCFilteringStrategy)
    {
#if (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_ON)
    case Dem_FilterDataFilterChronological:
        lDTCFilterFunction = Dem_FilterDataFilterMemoryChrono;
        break;
#endif
    case Dem_FilterDataFilterEventBased:
        lDTCFilterFunction = Dem_FilterDataFilterMemoryByEvent;
        break;
#if (DEM_OBDII_SUPPORT == STD_ON)
    case Dem_FilterDataFilterEventBasedPermanentMemory:
        lDTCFilterFunction = Dem_FilterDataFilterPermanentByEvent;
        break;
#endif
    default:
        lDTCFilterFunction = Dem_FilterDataFilterMemoryByEvent;
        break;
    }
    return lDTCFilterFunction;
}

/**
 * @brief Get the next matching event the filter criteria set with Dem_ClientSetDTCFilter
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
DEM_LOCAL Std_ReturnType Dem_FilterDataGetNextFilteredDTC(uint8 ClientId, Dem_FilterDataResultType* FilterResult)
{
    Std_ReturnType  ret;
    Dem_EventIdType nextEvent;
    uint8           dcmClientId = Dem_GetDcmClientIdOfClient(ClientId);

    nextEvent = Dem_FilterDataGetDTCFilterFunction(Dem_FilterDataGetDTCFilteringStrategy(
        dcmClientId))(ClientId, Dem_MemStateInfoInit(Dem_FilterDataGetMemIndex(dcmClientId)));

    if (nextEvent == DEM_EVENT_INVALID)
    {
        ret = DEM_NO_SUCH_ELEMENT;
    }
    else
    {
        const Dem_ClientInfoType* clientInfo = &(Dem_ClientInfo[ClientId]);
        Dem_MemoryNumType         memIndex   = clientInfo->MemIndex;
#if (DEM_OBDII_SUPPORT == STD_ON)
        if (Dem_FilterDataCheckUseFormatOBD(dcmClientId) == TRUE)
        {
            FilterResult->DTC = Dem_GetEventObdDTC(nextEvent);
        }
        else
#endif
        {
            FilterResult->DTC = Dem_GetEventDTC(nextEvent);
        }
        FilterResult->DTCStatus = (uint8)(Dem_DTCApplyExternalStatus(nextEvent, Dem_GetDTCUDSStatus(nextEvent)));
        FilterResult->DTCStatus &= Dem_GetDTCStatusMaskByMem(memIndex);
#if (DEM_FEATURE_DCM_0X19_0X14_ENABLE == STD_ON)
        if (Dem_FilterDataCheckUseFdc(dcmClientId) == TRUE)
        {
            FilterResult->FDC = Dem_FilterDataGetFDC(dcmClientId);
        }
#endif
#if ((DEM_FEATURE_DCM_0X19_0X08_ENABLE == STD_ON) || (DEM_FEATURE_DCM_0X19_0X42_ENABLE == STD_ON))
        if (Dem_FilterDataCheckUseSeverity(dcmClientId) == TRUE)
        {
            FilterResult->Severity       = Dem_GetCfgSeverityOfDTC(Dem_GetDTCRefOfEvent(nextEvent));
            FilterResult->FunctionalUnit = Dem_GetFUOfDTC(Dem_GetDTCRefOfEvent(nextEvent));
        }
#endif
        ret = E_OK;
    }
    return ret;
}
/* PRQA S 6070 -- */

/**
 * @brief         Get the next matching event the filter criteria set with Dem_ClientSetDTCFilter
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module
 * @param[out]    FilterResult: Result of filter data
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Std_ReturnType Dem_ClientGetNextFilteredDTC(uint8 ClientId, Dem_FilterDataResultType* FilterResult)
{
    return Dem_FilterDataGetNextFilteredDTC(ClientId, FilterResult);
}

#if (DEM_MAX_SIZE_FREEZEFRAME > 0u)

/**
 * @brief         Test if a DTC filter has been set for the given ClientId.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_ClientCheckFreezeFrameFilterSet(uint8 ClientId)
{
    /* PRQA S 4404 ++ */ /* VL_Dem_4404 */
    return (Dem_FFIteratorGetDTCFormat(Dem_GetDcmClientIdOfClient(ClientId)) != DEM_FREEZEFRAMEITERATOR_DTCFORMAT_INIT);
    /* PRQA S 4404 -- */
}

/**
 * @brief Sets a freeze frame record filter with the given criteria.
 */
DEM_LOCAL Std_ReturnType Dem_ClientStartFreezeFrameIterator(uint8 ClientId, Dem_DTCFormatType DTCFormat)
{
    uint8                       dcmClientId = Dem_GetDcmClientIdOfClient(ClientId);
    Dem_FFIteratorDTCFormatType lDtcFormat  = DEM_FREEZEFRAMEITERATOR_DTCFORMAT_INIT;
    /** Count stored snapshot records in primary memory to prevent concurrent modification of the chrono index to
     * have an effect on the iteration */
    Dem_MemStateInfoConstPtrType memoryInfo =
        Dem_MemStateInfoInit(Dem_GetPrimaryRefOfMemSet(Dem_GetMemorySetRefOfClient(ClientId)));
    Dem_NvBlockNumType memoryIndex = Dem_GetMemEntryStartOfMemoryInfo(memoryInfo);

    if (DTCFormat == DEM_DTC_FORMAT_UDS)
    {
        /** Set filter for output format of DTCs to UDS */
        lDtcFormat = DEM_FREEZEFRAMEITERATOR_DTCFORMAT_UDS;
    }
#if (DEM_OBDII_SUPPORT == STD_ON)
    else if (DTCFormat == DEM_DTC_FORMAT_OBD)
    {
        /** Set filter for output format of DTCs to OBD */
        lDtcFormat = DEM_FREEZEFRAMEITERATOR_DTCFORMAT_OBD;
    }
#endif
#if (DEM_J1939_SUPPORT == STD_ON)
    else if (DTCFormat == DEM_DTC_FORMAT_J1939)
    {
        /** Set filter for output format of DTCs to J1939 */
        lDtcFormat = DEM_FREEZEFRAMEITERATOR_DTCFORMAT_J1939;
    }
#endif
    else
    {
        /** Misra C */
    }
    /** Prepare iterations to get the data of the records when later calling Dem_GetNextFilteredRecord. */
    Dem_FFIteratorStartIterator(dcmClientId, lDtcFormat, memoryIndex);
    return E_OK;
}

/**
 * @brief         Gets the total number of stored FreezeFrame records
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module
 * @return        uint16
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint16 Dem_ClientGetNumberOfFreezeFrameRecords(uint8 ClientId)
{
    /** Count stored snapshot records */
    return Dem_FFIteratorGetNumberOfFF(ClientId);
}

/**
 * @brief         Iterate the next snapshot record number matching the filter.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module
 * @param[in]     DTC: Defines the DTC in respective format that is selected. If the DTC fits to a DTC group
 *                     number, the DTC group is selected.
 * @param[in]     RecordNumber: Unique identifier for a snapshot record as defined in ISO 14229-1. The value 0xFF
 *                              is a placeholder referencing all snapshot records of the addressed DTC. The value
 *                              0x00 indicates the DTC-specific WWH-OBD snapshot record.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Std_ReturnType Dem_ClientGetNextFreezeFrameRecord(uint8 ClientId, uint32* DTC, uint8* RecordNumber)
{
    return Dem_FFIteratorGetNext(ClientId, DTC, RecordNumber);
}
#endif

/**
 * @brief Get a DTC based on occurrence time.
 */
DEM_LOCAL Std_ReturnType Dem_ClientGetDTCByOccurrenceTime(uint8 ClientId, Dem_DTCRequestType DTCRequest, uint32* DTC)
{
    Std_ReturnType  ret     = DEM_NO_SUCH_ELEMENT;
    Dem_EventIdType eventId = DEM_EVENT_INVALID;
    switch (DTCRequest)
    {
    case DEM_FIRST_FAILED_DTC:
        eventId = Dem_GetFirstFailedEvent(ClientId);
        break;
    case DEM_MOST_RECENT_FAILED_DTC:
        eventId = Dem_GetMostRecentFailedEvent(ClientId);
        break;
    case DEM_FIRST_DET_CONFIRMED_DTC:
        eventId = Dem_GetFirstConfirmedEvent(ClientId);
        break;
    case DEM_MOST_REC_DET_CONFIRMED_DTC:
        eventId = Dem_GetMostRecentConfirmdEvent(ClientId);
        break;
    default:
        /** Mirsa C */
        break;
    }
    if (eventId != DEM_EVENT_INVALID)
    {
        /** Only EventIds with a valid DTC number should be stored in, so the EventId->DTC mapping will return always a
         * valid DTC number */
        if (Dem_CheckDTCSuppressed(eventId) == FALSE)
        {
            *DTC = Dem_GetEventDTC(eventId);
            ret  = E_OK;
        }
    }
    return ret;
}

/**
 * @brief         Test if the DTC record update was requested
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_ClientCheckDTCRecordUpdateRequested(uint8 ClientId)
{
    Dem_ReadoutBufferEntryType readoutBuffer = Dem_ClientGetReadoutBuffer(Dem_GetDcmClientIdOfClient(ClientId));
    return (readoutBuffer.State != DEM_READOUTBUFFER_STATUS_UNUSED); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/**
 * @brief Select the the given extended data record
 */
DEM_LOCAL Std_ReturnType Dem_ClientSelectExtendedDataRecord(
    uint8 ClientId,
    uint8 RecordNumber
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    ,
    uint8* ErrorId
#endif
)
{
    Std_ReturnType ret = E_NOT_OK;
    switch (Dem_GetClientResult(ClientId))
    {
    case DEM_CLIENT_RESULT_DTC:
        if (Dem_ClientCheckDTCRecordUpdateDisabled(ClientId) == TRUE)
        {
#if (DEM_EXTENDED_DATA_CLASS_NUMBER > 0u)
            uint8                           dcmClientId   = Dem_GetDcmClientIdOfClient(ClientId);
            Dem_ReadoutBufferEntryType      readoutBuffer = Dem_ClientGetReadoutBuffer(dcmClientId);
            Dem_ReadoutBufferEDIteratorType lEDIterator   = readoutBuffer.ExtendedIterator;
            Dem_EventIdType                 eventId       = readoutBuffer.EventId;
            Dem_MemStateInfoConstPtrType    memoryInfo    = Dem_MemStateInfoInit(readoutBuffer.MemIndex);
            Dem_DTCAttRefNumType            lDTCAttr      = Dem_GetDTCAttr(eventId);
            Dem_EDRefNumType                lEDRef        = Dem_GetEDRefOfDTCAttr(lDTCAttr);

            if (lEDRef != DEM_ATTRI_INVALID_EXTENDDATA)
            {
                lEDIterator.EDRNStart = Dem_GetEDRStartOfED(lEDRef);
                lEDIterator.EDRNEnd   = Dem_GetEDREndOfED(lEDRef);
            }
            else
            {
                lEDIterator.EDRNStart = 0u;
                lEDIterator.EDRNEnd   = 0u;
            }
            lEDIterator.MemoryIndex            = Dem_MemoryFindIndex(memoryInfo, eventId);
            lEDIterator.ExtendedEntryIndex     = 0u;
            lEDIterator.EventId                = eventId;
            lEDIterator.ExtendedDataNumber     = RecordNumber;
            lEDIterator.ExtendedRecordSelected = TRUE;
            Dem_ReadoutBufferSetEDIterator(dcmClientId, lEDIterator);
#endif
            ret = E_OK;
        }
        else
        {
            ret = DEM_PENDING;
        }
        break;
    case DEM_CLIENT_RESULT_GROUP:
    case DEM_CLIENT_RESULT_ALL_DTCS:
    case DEM_CLIENT_RESULT_NO_MATCH:
        ret = DEM_WRONG_DTC;
        break;
    case DEM_CLIENT_RESULT_WRONG_ORIGIN:
        ret = DEM_WRONG_DTCORIGIN;
        break;
    case DEM_CLIENT_RESULT_INIT:
        /** E_NOT_OK */
#if (DEM_DEV_ERROR_DETECT == STD_ON)
        *ErrorId = DEM_E_WRONG_CONDITION;
#endif
        break;
    default:
        /** E_NOT_OK */
        break;
    }
    return ret;
}

/**
 * @brief         Test if an extended data record is selected for the given ClientId.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_ClientCheckExtendedRecordSelected(uint8 ClientId)
{
    Dem_ReadoutBufferEntryType readoutBuffer = Dem_ClientGetReadoutBuffer(Dem_GetDcmClientIdOfClient(ClientId));
    return readoutBuffer.ExtendedIterator.ExtendedRecordSelected;
}

/**
 * @brief         Test if an snapshot record is selected for the given ClientId.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_ClientCheckFFRecordSelected(uint8 ClientId)
{
    Dem_ReadoutBufferEntryType readoutBuffer = Dem_ClientGetReadoutBuffer(Dem_GetDcmClientIdOfClient(ClientId));
    return readoutBuffer.FFIterator.SnapshotSelected;
}

/* PRQA S 1252 ++ */ /* VL_Dem_1252 */
/**
 * @brief Calculates the size of an extended data record
 */
DEM_LOCAL uint32 Dem_GetSizeOfEDCalculateSize(
    Dem_EDRecordRefNumType CfgEDIndex,
    uint8                  DcmClientId,
    Dem_NvBlockNumType     MemoryIndex,
    uint8                  ExtendedEntryIndex)
{
    uint32 sizeOfExtRec = 0u;
    /** Test if event has a memory entry */
    if (Dem_ReadoutBufferCheckStoredDataAvailable(DcmClientId) == TRUE)
    {
        if ((Dem_GetHaveUserDataOfEDRC(CfgEDIndex) == FALSE)
            /** Test if requested record is stored for event */
            || (Dem_CheckEDStored(MemoryIndex, ExtendedEntryIndex) == TRUE)) /* PRQA S 3415 */ /* VL_Dem_3415 */
        {
            sizeOfExtRec = (uint32)Dem_GetEDSizeOfEDRC(CfgEDIndex) + 1u;
        }
        /** else sizeOfExtRec is already set to 0 */
    }
    /** else sizeOfExtRec is already set to 0 */

    return sizeOfExtRec;
}
/* PRQA S 1252 -- */

#if (DEM_EXTENDED_DATA_CLASS_NUMBER > 0u)
/**
 * @brief Copies an extended data record into the given buffer
 */
DEM_LOCAL Std_ReturnType
    Dem_ClientGetNextEDCopyNextRecord(uint8 DcmClientId, Dem_DestinationBufferPtrType DestinationBuffer)
{
    Dem_ReadoutBufferEDIteratorType lEDIterator        = Dem_ReadoutBufferGetEDIterator(DcmClientId);
    Std_ReturnType                  ret                = E_NOT_OK;
    Dem_NvBlockNumType              memoryIndex        = lEDIterator.MemoryIndex;
    uint8                           extendedEntryIndex = lEDIterator.ExtendedEntryIndex;
    Dem_EDRecordRefNumType          cfgEDIndex         = Dem_GetValueOfEDRecordRef(lEDIterator.EDRNStart);

    if ((Dem_ReadoutBufferCheckStoredDataAvailable(DcmClientId) == TRUE)
        && ((Dem_GetHaveUserDataOfEDRC(cfgEDIndex) == FALSE)
            || (Dem_CheckEDStored(memoryIndex, extendedEntryIndex) == TRUE))) /* PRQA S 3415 */ /* VL_Dem_3415 */
    {
        /** Check buffer size after stored status - the buffer does not need to be large enough for an empty response */
        if ((uint16)(Dem_GetEDSizeOfEDRC(cfgEDIndex) + 1u)
            > (uint16)(DestinationBuffer->BufferSize - DestinationBuffer->WriteIndex))
        {
            ret = DEM_BUFFER_TOO_SMALL;
        }
        else
        {
            Dem_WriteUint8ToBuffer(DestinationBuffer, (uint8)Dem_GetEDRNumberOfEDRC(cfgEDIndex));
            (void)Dem_DataReportCopyExtendedDataRecord(
                lEDIterator.EventId,
                extendedEntryIndex,
                cfgEDIndex,
                memoryIndex,
                DestinationBuffer);
            ret = E_OK;
        }
    }
    else
    {
        /** records currently not stored are returned with empty content */
        ret                           = E_OK;
        DestinationBuffer->WriteIndex = 0u;
    }
    return ret;
}

/**
 * @brief Test whether filter for extended data record(s) matches
 */
DEM_LOCAL boolean Dem_ClientGetNextEDChecktRecordMatch(uint8 ClientId, uint8 RecordNumberFilter, uint8 RecordNumber)
{
    boolean match;
    uint8   maxRecordNumber;
    uint8   minRecordNumber;

#if (DEM_USER_DEFINED_MEMORY_ENABLE == STD_ON)
    if (Dem_ClientInfo[ClientId].Request.DTCOrigin >= DEM_DTC_ORIGIN_USER)
    {
        maxRecordNumber = DEM_DCM_EXTENDEDDATARECORD_LAST_USER;
        minRecordNumber = DEM_DCM_EXTENDEDDATARECORD_FIRST;
    }
    else
#else
    DEM_PARAM_UNUSED(ClientId);
#endif
    {
        maxRecordNumber = DEM_DCM_EXTENDEDDATARECORD_LAST_PRIMARY;
        minRecordNumber = DEM_DCM_EXTENDEDDATARECORD_FIRST;
    }
    if ((RecordNumberFilter == DEM_DCM_EXTENDEDDATARECORD_ALL) && (RecordNumber >= minRecordNumber)
        && (RecordNumber <= maxRecordNumber))
    {
        match = TRUE;
    }
#if (DEM_OBDII_SUPPORT == STD_ON)
    else if (
        (RecordNumberFilter == DEM_DCM_EXTENDEDDATARECORD_OBD) && (RecordNumber >= DEM_DCM_EXTENDEDDATARECORD_FIRST_OBD)
        && (RecordNumber <= DEM_DCM_EXTENDEDDATARECORD_LAST_OBD))
    {
        match = TRUE;
    }
#endif
    else if (RecordNumberFilter == RecordNumber)
    {
        match = TRUE;
    }
    else
    {
        match = FALSE;
    }
    return match;
}
#endif

/**
 * @brief Get the size of one or all extended data record(s)
 */
/* PRQA S 6030 ++ */ /* VL_MTR_Dem_STMIF */
DEM_LOCAL Std_ReturnType
    Dem_ClientGetSizeOfExtendedDataRecordSelection(uint8 ClientId, uint32* SizeOfExtendedDataRecord)
{
    Std_ReturnType ret = DEM_NO_SUCH_ELEMENT;
#if (DEM_EXTENDED_DATA_CLASS_NUMBER > 0u)
    uint32                          accumulatedSize = 0u;
    uint8                           dcmClientId     = Dem_GetDcmClientIdOfClient(ClientId);
    uint8                           recordNumber;
    Dem_ReadoutBufferEDIteratorType lEDIterator = Dem_ReadoutBufferGetEDIterator(dcmClientId);
    recordNumber                                = lEDIterator.ExtendedDataNumber;
    Dem_NvBlockNumType memoryIndex              = lEDIterator.MemoryIndex;

    if (memoryIndex != DEM_MEM_INVALID_MEMORY_INDEX)
    {
        if (recordNumber < DEM_RECORD_NUMBER_SPECIAL)
        {
            uint8                  extendedEntryIndex = 0u;
            Dem_EDRecordRefNumType lEDRNStart         = lEDIterator.EDRNStart;
            Dem_EDRecordRefNumType lEDRNEnd           = lEDIterator.EDRNEnd;
            /** Look for one specific record */
            for (; lEDRNStart < lEDRNEnd; ++lEDRNStart)
            {
                Dem_EDRecordRefNumType CfgEDIndex = Dem_GetValueOfEDRecordRef(lEDRNStart);
                if (recordNumber == Dem_GetEDRNumberOfEDRC(CfgEDIndex))
                {
                    /** If the record is supported, the result is always 'OK' */
                    ret = E_OK;
                    accumulatedSize =
                        Dem_GetSizeOfEDCalculateSize(CfgEDIndex, dcmClientId, memoryIndex, extendedEntryIndex);
                    break;
                }
                ++extendedEntryIndex;
            }
        }
        else
        {
            /** Look for all supported records */
            if ((recordNumber == DEM_RECORD_NUMBER_ALL)
#if (DEM_OBDII_SUPPORT == STD_ON)
                || (recordNumber == DEM_RECORD_NUMBER_OBDALL)
#endif
            )
            {
                uint8                  extendedEntryIndex = 0u;
                Dem_EDRecordRefNumType lEDRNStart         = lEDIterator.EDRNStart;
                Dem_EDRecordRefNumType lEDRNEnd           = lEDIterator.EDRNEnd;
                /** Look for one specific record */
                for (; lEDRNStart < lEDRNEnd; ++lEDRNStart)
                {
                    Dem_EDRecordRefNumType cfgEDIndex = Dem_GetValueOfEDRecordRef(lEDRNStart);
                    /** If the extended data number matches */
                    if (Dem_ClientGetNextEDChecktRecordMatch(ClientId, recordNumber, Dem_GetEDRNumberOfEDRC(cfgEDIndex))
                        == TRUE)
                    {
                        /** At least one record found - the result should be negative in case the event doesn't support
                         * any of the requested extended records. */
                        ret = E_OK;
                        accumulatedSize +=
                            Dem_GetSizeOfEDCalculateSize(cfgEDIndex, dcmClientId, memoryIndex, extendedEntryIndex);
                    }
                    extendedEntryIndex += 1u;
                }
            }
        }
    }
    if (ret == E_OK)
    {
        *SizeOfExtendedDataRecord = accumulatedSize;
    }
#endif
    return ret;
}
/* PRQA S 6030 -- */

/**
 * @brief Validates the request to get an extended data record in UDS format and copies the data.
 */
DEM_LOCAL Std_ReturnType Dem_ClientGetNextExtendedDataRecord(uint8 ClientId, uint8* DestBuffer, uint16* BufSize)
{
    Std_ReturnType ret = DEM_NO_SUCH_ELEMENT;
#if (DEM_EXTENDED_DATA_CLASS_NUMBER > 0u)
    uint8                           dcmClientId = Dem_GetDcmClientIdOfClient(ClientId);
    Dem_DestinationBufferType       destinationBuffer;
    Dem_ReadoutBufferEDIteratorType lEDIterator = Dem_ReadoutBufferGetEDIterator(dcmClientId);
    Dem_EDRecordRefNumType          lEDRNStart  = lEDIterator.EDRNStart;
    Dem_EDRecordRefNumType          lEDRNEnd    = lEDIterator.EDRNEnd;

    Dem_DestBufferInit(&destinationBuffer, DestBuffer, *BufSize);
    /** Find next record. */
    while (lEDRNStart < lEDRNEnd)
    {
        Dem_EDRecordRefNumType cfgEDIndex = Dem_GetValueOfEDRecordRef(lEDRNStart);
        /** If the extended data number matches */
        if (Dem_ClientGetNextEDChecktRecordMatch(
                ClientId,
                lEDIterator.ExtendedDataNumber,
                Dem_GetEDRNumberOfEDRC(cfgEDIndex))
            == TRUE)
        {
            ret = Dem_ClientGetNextEDCopyNextRecord(dcmClientId, &destinationBuffer);
        }
        ++lEDRNStart;
        lEDIterator.EDRNStart = lEDRNStart;
        lEDIterator.ExtendedEntryIndex++;
        Dem_ReadoutBufferSetEDIterator(dcmClientId, lEDIterator);

        if (ret != DEM_NO_SUCH_ELEMENT)
        {
            /** record was found */
            if (ret == E_OK)
            {
                /** return number of written bytes */
                *BufSize = destinationBuffer.WriteIndex;
            }
            break;
        }
    }
#endif
    return ret;
}

/**
 * @brief Initializes the iterator for the selected event.
 */
DEM_LOCAL void Dem_ReadoutBufferSelectFF(uint8 DcmClientId, uint8 RecordNumber)
{
    Dem_ReadoutBufferEntryType      readoutBuffer = Dem_ClientGetReadoutBuffer(DcmClientId);
    Dem_ReadoutBufferFFIteratorType lFFIterator   = readoutBuffer.FFIterator;
#if (DEM_MAX_SIZE_FREEZEFRAME > 0u)
    Dem_EventIdType              eventId    = readoutBuffer.EventId;
    Dem_MemStateInfoConstPtrType memoryInfo = Dem_MemStateInfoInit(readoutBuffer.MemIndex);

    Dem_FFEntryIteratorInit(eventId, Dem_MemoryFindIndex(memoryInfo, eventId), &(lFFIterator.FFEntryIter));
#endif
    lFFIterator.RecordNumber     = RecordNumber;
    lFFIterator.Source           = Dem_ReadoutBuffer_FF_Invalid;
    lFFIterator.SnapshotSelected = TRUE;
    Dem_InitFFIterator(&(lFFIterator.FFEntryIter), 0u, 0u);
    Dem_ReadoutBufferSetFFIterator(DcmClientId, lFFIterator);
}

/**
 * @brief Sets the filter to be used by Dem_GetNextFreezeFrameData and Dem_GetSizeOfFreezeFrameSelection.
 */
DEM_LOCAL void Dem_GetFFInitFFIterator(uint8 DcmClientId)
{
    Dem_ReadoutBufferFFIteratorType lFFIterator = Dem_ReadoutBufferGetFFIterator(DcmClientId);
    switch (Dem_ReadoutBufferGetFFIteratorSource(DcmClientId))
    {
#if (DEM_OBDII_SUPPORT == STD_ON)
    case Dem_ReadoutBuffer_FF_Obd:
        Dem_InitFFIterator(&(lFFIterator.FFEntryIter), 0u, 1u);
        break;
#endif
    case Dem_ReadoutBuffer_FF_Srec:
    {
        Dem_ReadoutBufferEntryType readoutBuffer = Dem_ClientGetReadoutBuffer(DcmClientId);
#if (DEM_MAX_SIZE_FREEZEFRAME > 0u)
        Dem_FFEntryIteratorInit(readoutBuffer.EventId, lFFIterator.FFEntryIter.MemoryIndex, &(lFFIterator.FFEntryIter));
#endif
        break;
    }
    default:
        Dem_InitFFIterator(&(lFFIterator.FFEntryIter), 0u, 0u);
        break;
    }
    Dem_ReadoutBufferSetFFIterator(DcmClientId, lFFIterator);
}

/**
 * @brief Select the first snapshot record source
 */
DEM_LOCAL Std_ReturnType Dem_ClientSelectFirstSource(
    uint8 ClientId,
    uint8 RecordNumber
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    ,
    uint8* ErrorId
#endif
)
{
    Std_ReturnType ret = E_NOT_OK;
    switch (Dem_GetClientResult(ClientId))
    {
    case DEM_CLIENT_RESULT_DTC:
        if (Dem_ClientCheckDTCRecordUpdateDisabled(ClientId) == TRUE)
        {
            uint8                    DcmClientId = Dem_GetDcmClientIdOfClient(ClientId);
            Dem_ReadoutBuffer_FFType source;

            Dem_ReadoutBufferSelectFF(DcmClientId, RecordNumber);
#if (DEM_OBDII_SUPPORT == STD_ON)
            if (RecordNumber == DEM_DCM_SNAPSHOTDATARECORD_OBD)
            {
                source = Dem_ReadoutBuffer_FF_Obd;
            }
            else
#endif
#if (DEM_MAX_SIZE_FREEZEFRAME > 0u)
                if (RecordNumber == DEM_DCM_SNAPSHOTDATARECORD_ALL)
            {
                source = Dem_ReadoutBuffer_FF_Srec;
            }
            else
#endif
            {
#if (DEM_MAX_SIZE_FREEZEFRAME > 0u)
                source = Dem_ReadoutBuffer_FF_Srec;
#else
                source = Dem_ReadoutBuffer_FF_Invalid;
#endif
            }
            Dem_ReadoutBufferSetFFIteratorSource(DcmClientId, source);
            Dem_GetFFInitFFIterator(DcmClientId);
            ret = E_OK;
        }
        else
        {
            ret = DEM_PENDING;
        }
        break;
    case DEM_CLIENT_RESULT_GROUP:
    case DEM_CLIENT_RESULT_ALL_DTCS:
    case DEM_CLIENT_RESULT_NO_MATCH:
        ret = DEM_WRONG_DTC;
        break;
    case DEM_CLIENT_RESULT_WRONG_ORIGIN:
        ret = DEM_WRONG_DTCORIGIN;
        break;
    case DEM_CLIENT_RESULT_INIT:
        /** E_NOT_OK */
#if (DEM_DEV_ERROR_DETECT == STD_ON)
        *ErrorId = DEM_E_WRONG_CONDITION;
#endif
        break;
    default:
        /** E_NOT_OK */
        break;
    }
    return ret;
}

/**
 * @brief Get the size of one or all formatted snapshot record() stored for a DTC (19 04).
 */
DEM_LOCAL Std_ReturnType Dem_ClientGetSizeOfFFSelection(uint8 ClientId, uint32* SizeOfFreezeFrame)
{
    Std_ReturnType             ret           = DEM_NO_SUCH_ELEMENT;
    uint8                      dcmClientId   = Dem_GetDcmClientIdOfClient(ClientId);
    Dem_ReadoutBufferEntryType readoutBuffer = Dem_ClientGetReadoutBuffer(dcmClientId);
#if ((DEM_MAX_SIZE_FREEZEFRAME > 0u) || (DEM_OBDII_SUPPORT == STD_ON))
    Dem_ReadoutBufferFFIteratorType lFFIterator  = readoutBuffer.FFIterator;
    uint8                           recordNumber = lFFIterator.RecordNumber;

    if (recordNumber == DEM_DCM_SNAPSHOTDATARECORD_OBD)
    {
#if (DEM_OBDII_SUPPORT == STD_ON)
        Dem_NvBlockNumType memoryIndex = lFFIterator.FFEntryIter.MemoryIndex;
        Dem_EventIdType    eventId     = readoutBuffer.EventId;
        if (Dem_GetPrimaryRefOfMemSet(Dem_GetMemorySetRefOfClient(ClientId)) == readoutBuffer.MemIndex)
        {
            ret = Dem_GetSizeOfFFSelectionCalculateSizeObd(eventId, memoryIndex, SizeOfFreezeFrame);
        }
#endif
    }
    else if (recordNumber == DEM_DCM_SNAPSHOTDATARECORD_ALL)
    {
        ret = Dem_GetSizeOfFFSelectionCalculateSizeAll(readoutBuffer, SizeOfFreezeFrame);
    }
    else
    {
#if (DEM_MAX_SIZE_FREEZEFRAME > 0u)
        ret = Dem_GetSizeOfFFSelectionCalculateSizeStd(readoutBuffer, recordNumber, SizeOfFreezeFrame);
#endif
    }
#endif
    return ret;
}

#if (DEM_OBDII_SUPPORT == STD_ON)
/**
 * @brief Reads the OBD freeze frame
 */
DEM_LOCAL Std_ReturnType Dem_GetNextFFDataObd(uint8 ClientId, Dem_DestinationBufferPtrType DestinationBuffer)
{
    uint8                           dcmClientId   = Dem_GetDcmClientIdOfClient(ClientId);
    Dem_ReadoutBufferEntryType      readoutBuffer = Dem_ClientGetReadoutBuffer(dcmClientId);
    Dem_ReadoutBufferFFIteratorType lFFIterator   = readoutBuffer.FFIterator;
    Dem_FFEntryIterType             lFFEntryIter  = lFFIterator.FFEntryIter;
    Std_ReturnType                  ret           = DEM_NO_SUCH_ELEMENT;

    if (lFFEntryIter.FFRNStart < lFFEntryIter.FFRNEnd)
    {
        if (Dem_GetNextFFDataCheckRecordMatch(lFFIterator.RecordNumber, 0x00) == TRUE)
        {
            if (Dem_GetPrimaryRefOfMemSet(Dem_GetMemorySetRefOfClient(ClientId)) == readoutBuffer.MemIndex)
            {
                ret = Dem_GetNextFFDataCopyNextRecordObd(readoutBuffer, DestinationBuffer);
            }
        }
        lFFEntryIter.FFRNStart++;
    }
    Dem_ReadoutBufferSetFFIterator(dcmClientId, lFFIterator);
    return ret;
}
#endif

#if (DEM_MAX_SIZE_FREEZEFRAME > 0u)
/**
 * @brief Reads the freeze frame
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
DEM_LOCAL Std_ReturnType Dem_GetNextFFDataStd(uint8 ClientId, Dem_DestinationBufferPtrType DestinationBuffer)
{
    uint8                           dcmClientId   = Dem_GetDcmClientIdOfClient(ClientId);
    Dem_ReadoutBufferEntryType      readoutBuffer = Dem_ClientGetReadoutBuffer(dcmClientId);
    Dem_ReadoutBufferFFIteratorType lFFIterator   = readoutBuffer.FFIterator;
    Dem_FFEntryIterType             lFFEntryIter  = lFFIterator.FFEntryIter;
    Std_ReturnType                  ret           = DEM_NO_SUCH_ELEMENT;

    while (Dem_FFEntryIteratorExists(&lFFEntryIter) == TRUE) /* PRQA S 0771 */ /* VL_Dem_0771 */
    {
        uint8                lFFEntryIndex = lFFEntryIter.FFEntryIndex;
        uint8                recordNumber  = lFFIterator.RecordNumber;
        Dem_EventIdType      eventId       = readoutBuffer.EventId;
        Dem_DTCAttRefNumType lDTCAttr      = Dem_GetDTCAttr(eventId);

#if (DEM_GENERAL_FF_RECNUM_CALCULATED == STD_ON)
        if ((Dem_GetFFRecordOfMemory(readoutBuffer.MemIndex) == DEM_FF_RECNUM_CALCULATED)
            && (recordNumber < Dem_GetMaxNumFFROfDTCAttr(lDTCAttr)))
        {
            /** overshoot -> snapshot not supported */
            ret = DEM_NO_SUCH_ELEMENT;
            break;
        }
        else
#endif
        {
            uint8 lRecordNumber;
#if (DEM_GENERAL_FF_RECNUM_CALCULATED == STD_ON)
            Dem_MemoryNumType memRef = Dem_GetMemRef(lDTCAttr);
            if (Dem_GetFFRecordOfMemory(memRef) == DEM_FF_RECNUM_CALCULATED)
            {
                lRecordNumber = lFFEntryIndex + 1u;
            }
            else
#endif
            {
#if (DEM_GENERAL_FF_RECNUM_CONFIGURED == STD_ON)
                Dem_FFRRefNumType lFFRecordRef = Dem_GetValueOfDFFRRef(lFFEntryIter.FFRNStart);
                lRecordNumber                  = Dem_GetFFRNumberOfFFRC(lFFRecordRef);
#endif
            }

            if (Dem_GetNextFFDataCheckRecordMatch(recordNumber, lRecordNumber) == TRUE)
            {
                Dem_FFRefNumType ref = Dem_GetFFRef(lDTCAttr);
                if (DestinationBuffer->BufferSize < (Dem_GetFFSizeOfFF(ref) + 1u))
                {
                    /** supported record, but the buffer is not large enough for all the data */
                    ret = DEM_BUFFER_TOO_SMALL;
                }
                else
                {
                    Dem_NvBlockNumType memoryIndex = lFFEntryIter.MemoryIndex;

                    if (Dem_CheckFFStored(lFFEntryIndex, memoryIndex) == TRUE)
                    {
                        Dem_WriteUint8ToBuffer(DestinationBuffer, lRecordNumber);
                        Dem_DataReportFFCopyData(
                            DestinationBuffer,
                            Dem_GetFreezeFramePtrOfMemEntry(memoryIndex, lFFEntryIndex),
                            eventId,
                            memoryIndex);
                    }
                    ret = E_OK;
                }
                Dem_FFEntryIteratorNext(&lFFEntryIter);
                break;
            }
            /** otherwise next snapshot record */
        }
        Dem_FFEntryIteratorNext(&lFFEntryIter);
    }
    lFFIterator.FFEntryIter = lFFEntryIter;
    Dem_ReadoutBufferSetFFIterator(dcmClientId, lFFIterator);
    return ret;
}
/* PRQA S 6070 -- */
#endif

/**
 * @brief Select the next snapshot record source
 */
DEM_LOCAL Std_ReturnType Dem_ClientSelectNextSource(uint8 DcmClientId)
{
    Std_ReturnType                  ret;
    Dem_ReadoutBufferEntryType      readoutBuffer = Dem_ClientGetReadoutBuffer(DcmClientId);
    Dem_ReadoutBufferFFIteratorType lFFIterator   = readoutBuffer.FFIterator;

    if (lFFIterator.RecordNumber == DEM_DCM_SNAPSHOTDATARECORD_ALL)
    {
        switch (Dem_ReadoutBufferGetFFIteratorSource(DcmClientId))
        {
#if (DEM_OBDII_SUPPORT == STD_ON)
        case Dem_ReadoutBuffer_FF_Obd:
            Dem_ReadoutBufferSetFFIteratorSource(DcmClientId, Dem_ReadoutBuffer_FF_Srec);
            ret = E_OK;
            break;
#endif
        case Dem_ReadoutBuffer_FF_Srec:
            Dem_ReadoutBufferSetFFIteratorSource(DcmClientId, Dem_ReadoutBuffer_FF_Invalid);
            ret = E_OK;
            break;
        default:
            /** Dem_ReadoutBuffer_FF_Invalid */
            ret = DEM_NO_SUCH_ELEMENT;
            break;
        }
    }
    else
    {
        Dem_ReadoutBufferSetFFIteratorSource(DcmClientId, Dem_ReadoutBuffer_FF_Invalid);
        ret = DEM_NO_SUCH_ELEMENT;
    }

    Dem_GetFFInitFFIterator(DcmClientId);

    return ret;
}

/**
 * @brief Copies data into the given buffer.
 */
DEM_LOCAL Std_ReturnType Dem_ClientGetNextFreezeFrameData(uint8 ClientId, uint8* DestBuffer, uint16* BufSize)
{
    Std_ReturnType            ret         = DEM_NO_SUCH_ELEMENT;
    boolean                   processOn   = TRUE;
    uint8                     dcmClientId = Dem_GetDcmClientIdOfClient(ClientId);
    Dem_DestinationBufferType destinationBuffer;

    /** Initialize a DestinationBuffer Context. */
    Dem_DestBufferInit(&destinationBuffer, DestBuffer, *BufSize);
    while (processOn == TRUE)
    {
        Std_ReturnType readNextReturnValue = DEM_NO_SUCH_ELEMENT;
        switch (Dem_ReadoutBufferGetFFIteratorSource(dcmClientId))
        {
#if (DEM_OBDII_SUPPORT == STD_ON)
        case Dem_ReadoutBuffer_FF_Obd:
            readNextReturnValue = Dem_GetNextFFDataObd(ClientId, &destinationBuffer);
            break;
#endif
#if (DEM_MAX_SIZE_FREEZEFRAME > 0u)
        case Dem_ReadoutBuffer_FF_Srec:
            readNextReturnValue = Dem_GetNextFFDataStd(dcmClientId, &destinationBuffer);
            break;
#endif
        default:
            /** Dem_ReadoutBuffer_FF_Invalid */
            break;
        }

        if (readNextReturnValue == DEM_NO_SUCH_ELEMENT)
        {
            Std_ReturnType selectNextReturnValue = Dem_ClientSelectNextSource(dcmClientId);
            if (selectNextReturnValue == DEM_NO_SUCH_ELEMENT)
            {
                ret       = selectNextReturnValue;
                processOn = FALSE;
            }
        }
        else
        {
            ret       = readNextReturnValue;
            processOn = FALSE;
        }
    }
    if (ret == E_OK)
    {
        *BufSize = destinationBuffer.WriteIndex;
    }
    return ret;
}
#endif

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#endif
