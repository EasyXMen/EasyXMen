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
 **  @file               : Dem_FreezeFrame.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : Implementation file of Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_FREEZEFRAME_H
#define DEM_FREEZEFRAME_H

/* =================================================== inclusions =================================================== */
#include "Dem_DtcInterface.h"
#include "Dem_DataElement.h"
#include "Dem_MemInterface.h"
#include "Dem_Obd.h"

/* ===================================================== macros ===================================================== */
/**
 * @brief This type is used to encode the Dem_DTCFormatType of a FreezeFrameIterator.
 */
typedef uint8 Dem_FFIteratorDTCFormatType;
#define DEM_FREEZEFRAMEITERATOR_DTCFORMAT_INIT  0u /**< Initialization value, filter is unused */
#define DEM_FREEZEFRAMEITERATOR_DTCFORMAT_UDS   1u /**< Selects the UDS DTC format  */
#define DEM_FREEZEFRAMEITERATOR_DTCFORMAT_OBD   2u /**< Selects the OBD DTC format  */
#define DEM_FREEZEFRAMEITERATOR_DTCFORMAT_J1939 3u /**< Selects the  J1939 DTC format */

/**
 * @name  DEM_FREEZEFRAMEITERATOR_J1939STATE_ENCODE
 * @{
 * @brief Macros encoding state for J1939 freeze frame filtering
 */
#define DEM_FREEZEFRAMEITERATOR_J1939STATE_INIT                                                                       \
    (0u)                                             /**< Initialization value, J1939 freeze frame iterator is unused \
                                                      */
#define DEM_FREEZEFRAMEITERATOR_J1939STATE_USED (1u) /**< A J1939 freeze frame filter was set up */
#define DEM_FREEZEFRAMEITERATOR_J1939STATE_PENDING                                                               \
    (2u) /**< A J1939 freeze frame filter was set up, filtering was interrupted by data modification and will be \
          * continued in next iteration */
/** @} */

/**
 * @name  DEM_DCM_SNAPSHOTDATARECORD_ENCODE
 * @{
 * @brief List of special snapshot data record values
 */
#define DEM_DCM_SNAPSHOTDATARECORD_OBD            (0x00u) /**< OBD record */
#define DEM_DCM_SNAPSHOTDATARECORD_STANDARD_FIRST (0x01u) /**< The first vaild standard snapshot data record */
#define DEM_DCM_SNAPSHOTDATARECORD_STANDARD_LAST  (0xFEu) /**< The last valid standard snapshot data record */
#define DEM_DCM_SNAPSHOTDATARECORD_ALL            (0xFFu) /**< All snapshot data records */
/** @} */

#define DEM_MAX_DID_COUNT (0xFFu) /**< the DID count max value */

/* ================================================ type definitions ================================================ */
/**
 * @brief FreezeFrameIterator data for Dcm requests
 */
typedef struct Dem_FFIteratorFilterTag /* PRQA S 1536 */ /* VL_Dem_1536 */
{
    Dem_FFIteratorDTCFormatType FilterDTCFormat; /**< DTCFormat: UDS, OBD, J1939, INIT (no filter) @range 0..3 */
    uint8                       MemoryIndex;     /**< Memory entry type @range 0..255 */
    Dem_FFEntryIterType         FFEntryIter;     /**< Iterator for the snapshot entries @range 0..255 */
    Dem_EventIdType             EventId;         /**< Event Id for J1939 Freeze Frame filtering @range 0..255 */
    Dem_J1939DcmSetFreezeFrameFilterType
          FreezeFrameKind; /**< Freeze Frame Kind for J1939 Freeze Frame filtering @range 0..255 */
    uint8 FilterState;     /**< Filter state for J1939 Freeze Frame filtering @range 0..255 */
#if ((DEM_MAX_SIZE_J1939EXFREEZEFRAME > 0u) || (DEM_MAX_SIZE_J1939FREEZEFRAME > 0u))
    Dem_SPNCRefNumType SPNIter; /**< Iterator for the SPN @range 0..255 */
#endif
} Dem_FFIteratorFilterType;

/* ========================================== internal function declarations ======================================== */
#if (DEM_MAX_SIZE_FREEZEFRAME > 0u)
/**
 * @brief         Sample a UDS snapshot into the prestorage buffer
 * @param[in]     EventId: Event ID
 * @param[in]     DestBuffer: Pointer to the destination buffer
 * @param[out]    BufferSize: Buffer size
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_GetFFFromEnv(Dem_EventIdType EventId, uint8* DestBuffer, uint16 BufferSize);
#endif

/**
 * @brief         Init FF Iterator.
 * @param[in]     FFEntryIter: Pointer to Iterator for the snapshot entries
 * @param[out]    FFRNStart: Start FF reference number
 * @param[out]    FFRNEnd: End FF reference number
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void
    Dem_InitFFIterator(Dem_FFEntryIterType* FFEntryIter, Dem_FFRNRefNumType FFRNStart, Dem_FFRNRefNumType FFRNEnd);

#if ((DEM_OBDII_SUPPORT == STD_ON) && (DEM_MAX_SIZE_OBDFREEZEFRAME > 0u))
/**
 * @brief         Sample a OBD snapshot into the prestorage buffer
 * @param[in]     EventId: Event ID
 * @param[in]     DestBuffer: Pointer to the destination buffer
 * @param[out]    BufferSize: Buffer size
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_GetOBDFFFromEnv(Dem_EventIdType EventId, uint8* DestBuffer, uint16 BufferSize);
#endif

#if (DEM_MAX_SIZE_J1939FREEZEFRAME > 0u)
/**
 * @brief         Sample a j1939 snapshot into the prestorage buffer
 * @param[in]     EventId: Event ID
 * @param[in]     DestBuffer: Pointer to the destination buffer
 * @param[out]    BufferSize: Buffer size
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_GetJ1939FFFromEnv(Dem_EventIdType EventId, uint8* DestBuffer, uint16 BufferSize);
#endif

#if (DEM_MAX_SIZE_J1939EXFREEZEFRAME > 0u)
/**
 * @brief         Sample a j1939 expanded snapshot into the prestorage buffer
 * @param[in]     EventId: Event ID
 * @param[in]     DestBuffer: Pointer to the destination buffer
 * @param[out]    BufferSize: Buffer size
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_GetJ1939ExFFFromEnv(Dem_EventIdType EventId, uint8* DestBuffer, uint16 BufferSize);
#endif

#if (DEM_MAX_SIZE_FREEZEFRAME > 0u)
/**
 * @brief         Initializes the snapshot entry iterator
 * @param[in]     EventId: Event ID
 * @param[in]     MemoryIndex: Memory index
 * @param[in]     FFEntryIter: Pointer to Iterator for the snapshot entries
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void
    Dem_FFEntryIteratorInit(Dem_EventIdType EventId, Dem_NvBlockNumType MemoryIndex, Dem_FFEntryIterType* FFEntryIter);

#if (DEM_FEATURE_SWC_GET_FF_ED == STD_ON)
#if (DEM_GENERAL_FF_RECNUM_CALCULATED == STD_ON)
/**
 * @brief         Move the snapshot entry iterator to the index of the last stored snapshot entry.
 * @param[in]     EventId: Event ID
 * @param[in]     MemoryIndex: Memory index
 * @param[in]     FFEntryIter: Pointer to Iterator for the snapshot entries
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_FFEntryIteratorFindLastStoredEntry(
    Dem_EventIdType      EventId,
    Dem_NvBlockNumType   MemoryIndex,
    Dem_FFEntryIterType* FFEntryIter);
#endif
#endif

/**
 * @brief         Move the snapshot entry iterator to the index of the snapshot entry with requested record number.
 * @param[in]     EventId: Event ID
 * @param[in]     RecordNumber: This parameter is a unique identifier for a freeze frame record as defined in
 *                              ISO14229-1. 0xFF means most recent freeze frame record is returned. 0x00 is only
 *                              supported if the Dem module supports WWH-OBD (refer to DemOBDSupport)
 * @param[in]     MemoryIndex: Memory index
 * @param[in]     FFEntryIter: Pointer to Iterator for the snapshot entries
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_FFEntryIteratorFindEntryWithRecordNumber(
    Dem_EventIdType      EventId,
    uint8                RecordNumber,
    Dem_NvBlockNumType   MemoryIndex,
    Dem_FFEntryIterType* FFEntryIter);

#if (DEM_FEATURE_SWC_GET_FF_ED == STD_ON)
/**
 * @brief         Extracts a snapshot record, or a single DID from it.
 * @param[in]     DataId: This parameter specifies the DID (ISO14229-1) that shall be copied to the destination buffer.
 * @param[in]     SourceBuffer: Pointer to source buffer
 * @param[in]     MemoryIndex: Memory index
 * @param[in]     DestinationBuffer: Pointer to destination buffer
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_DataReportCopyDidFF(
    uint16                       DataId,
    Dem_NvBlockNumType           MemoryIndex,
    const uint8*                 SourceBuffer,
    Dem_DestinationBufferPtrType DestinationBuffer);

/**
 * @brief         Extracts a snapshot record, or a single DID from it
 * @param[in]     EventId: Event ID
 * @param[in]     RecordNumber: This parameter is a unique identifier for a freeze frame record as defined in
 *                              ISO14229-1. 0xFF means most recent freeze frame record is returned. 0x00 is only
 *                              supported if the Dem module supports WWH-OBD (refer to DemOBDSupport)
 * @param[in]     DataId: This parameter specifies the DID (ISO14229-1) that shall be copied to the destination buffer.
 * @param[in]     DestinationBuffer: Pointer to destination buffer
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_EnvDataCopyDidFromFF(
    Dem_EventIdType              EventId,
    uint8                        RecordNumber,
    uint16                       DataId,
    Dem_DestinationBufferPtrType DestinationBuffer);
#endif
#endif

#if (DEM_FEATURE_SWC_GET_FF_ED == STD_ON)
/**
 * @brief         Implementation of SWC API Dem_GetEventFreezeFrameDataEx().
 * @param[in]     EventId: Event ID
 * @param[in]     RecordNumber: This parameter is a unique identifier for a freeze frame record as defined in
 *                              ISO14229-1. 0xFF means most recent freeze frame record is returned. 0x00 is only
 *                              supported if the Dem module supports WWH-OBD (refer to DemOBDSupport)
 * @param[in]     DataId: This parameter specifies the DID (ISO14229-1) that shall be copied to the destination buffer.
 * @param[in]     DestBuffer: Pointer to the destination buffer
 * @param[out]    BufSize: Buffer size
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_EnvDataSwcApiGetEventFreezeFrameDataEx(
    Dem_EventIdType EventId,
    uint8           RecordNumber,
    uint16          DataId,
    uint8*          DestBuffer,
    uint16*         BufSize);
#endif

#if (DEM_DCM_CLIENT_NUMBER > 0u)
/**
 * @brief         Counts the number of snapshot records stored in a memory entry
 * @param[in]     MemoryIndex: Memory index
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL uint8 Dem_FFEntryGetNumberOfFFRecords(Dem_NvBlockNumType MemoryIndex);
#endif

#if ((DEM_DCM_CLIENT_NUMBER > 0u) || (DEM_J1939DCM_CLIENT_NUMBER > 0u))
/**
 * @brief         Move the snapshot entry iterator to the index of the next stored snapshot entry.
 * @param[in]     TypeOfFFRecordNumeration: Type of FF record index
 * @param[in]     FFEntryIter: Pointer to Iterator for the snapshot entries
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_FFEntryIteratorSetNextStoredEntry(
    Dem_TypeOfFreezeFrameRecordNumerationType TypeOfFFRecordNumeration,
    Dem_FFEntryIterType*                      FFEntryIter);

/**
 * @brief         Gets the number of stored snapshot records
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module
 * @return        uint16
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL uint16 Dem_FFIteratorGetNumberOfFF(uint8 ClientId);

/**
 * @brief         Filters the snapshot records of events stored in primary memory.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module
 * @param[in]     RecordNumber: This parameter is a unique identifier for a freeze frame record as defined in
 *                              ISO14229-1. 0xFF means most recent freeze frame record is returned. 0x00 is only
 *                              supported if the Dem module supports WWH-OBD (refer to DemOBDSupport)
 * @param[out]    DTC: Receives the DTC value in respective format of the filter returned by this function. If
 *                     the return value of the function is other than DEM_FILTERED_OK this parameter does not contain
 *                     valid data.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_FFIteratorGetNext(uint8 ClientId, uint32* DTC, uint8* RecordNumber);
#endif

#if (DEM_DCM_CLIENT_NUMBER > 0u)
#if (DEM_OBDII_SUPPORT == STD_ON)
/**
 * @brief         Calculates the total size of the OBD II freeze frame, when formatted as UDS snapshot record (19 04)
 * @param[in]     EventId: Event ID
 * @param[in]     MemoryIndex: Memory index
 * @param[out]    SizeOfFreezeFrame: Number of bytes in the requested freeze frame record.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_GetSizeOfFFSelectionCalculateSizeObd(
    Dem_EventIdType    EventId,
    Dem_NvBlockNumType MemoryIndex,
    uint32*            SizeOfFreezeFrame);
#endif

#if ((DEM_MAX_SIZE_FREEZEFRAME > 0u) || (DEM_OBDII_SUPPORT == STD_ON))
/**
 * @brief         Calculates the size of all snapshot records
 * @param[in]     ReadoutBuffer: The buffer to store readout data
 * @param[out]    SizeOfFreezeFrame: Number of bytes in the requested freeze frame record.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType
    Dem_GetSizeOfFFSelectionCalculateSizeAll(Dem_ReadoutBufferEntryType ReadoutBuffer, uint32* SizeOfFreezeFrame);

/**
 * @brief         Test whether filter for snapshot record(s) matches
 * @param[in]     RecordNumberFilter: Filter RecordNumber.
 * @param[in]     RecordNumber: This parameter is a unique identifier for a freeze frame record as defined in
 *                              ISO14229-1. 0xFF means most recent freeze frame record is returned. 0x00 is only
 *                              supported if the Dem module supports WWH-OBD (refer to DemOBDSupport)
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_GetNextFFDataCheckRecordMatch(uint8 RecordNumberFilter, uint8 RecordNumber);
#endif

#if (DEM_MAX_SIZE_FREEZEFRAME > 0u)
/**
 * @brief         Calculates the size of a standard snapshot record
 * @param[in]     ReadoutBuffer: The buffer to store readout data
 * @param[in]     RecordNumber: This parameter is a unique identifier for a freeze frame record as defined in
 *                              ISO14229-1. 0xFF means most recent freeze frame record is returned. 0x00 is only
 *                              supported if the Dem module supports WWH-OBD (refer to DemOBDSupport)
 * @param[out]    SizeOfFreezeFrame: Number of bytes in the requested freeze frame record.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_GetSizeOfFFSelectionCalculateSizeStd(
    Dem_ReadoutBufferEntryType ReadoutBuffer,
    uint8                      RecordNumber,
    uint32*                    SizeOfFreezeFrame);

/**
 * @brief         Copies the contents of a stored snapshot record
 * @param[in]     EventId: Event ID
 * @param[in]     DestinationBuffer: Pointer to destination buffer
 * @param[in]     SourceBuffer: Pointer to source buffer
 * @param[in]     MemoryIndex: Memory index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DataReportFFCopyData(
    Dem_DestinationBufferPtrType DestinationBuffer,
    const uint8*                 SourceBuffer,
    Dem_EventIdType              EventId,
    Dem_NvBlockNumType           MemoryIndex);
#endif

#if (DEM_OBDII_SUPPORT == STD_ON)
/**
 * @brief         Copies the contents of an OBD freeze frame.
 * @param[in]     EventId: Event ID
 * @param[in]     DestinationBuffer: Pointer to destination buffer
 * @param[in]     MemoryIndex: Memory index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DataReportObdFreezeFrameCopyUdsData(
    Dem_NvBlockNumType           MemoryIndex,
    Dem_EventIdType              EventId,
    Dem_DestinationBufferPtrType DestinationBuffer);

/**
 * @brief         Validates the request to get the OBD FreezeFrame in UDS Format, and copies the data.
 * @param[in]     ReadoutBuffer: The buffer to store readout data
 * @param[in]     DestinationBuffer: Pointer to destination buffer
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_GetNextFFDataCopyNextRecordObd(
    Dem_ReadoutBufferEntryType   ReadoutBuffer,
    Dem_DestinationBufferPtrType DestinationBuffer);
#endif
#endif

/* ============================================ internal data definitions =========================================== */
#if ((DEM_DCM_CLIENT_NUMBER > 0u) || (DEM_J1939DCM_CLIENT_NUMBER > 0u))
#define DEM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dem_MemMap.h"
/**
 * @brief     Type definition for Freeze frame Iterator Table
 * @range NA
 */
DEM_LOCAL Dem_FFIteratorFilterType Dem_FFIteratorTable[DEM_DCM_CLIENT_NUMBER + DEM_J1939DCM_CLIENT_NUMBER];
#define DEM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dem_MemMap.h"
#endif

/* ========================================== internal function definitions ========================================= */
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"

#if (DEM_MAX_SIZE_FREEZEFRAME > 0u)
/**
 * @brief get freeze frame reference
 * @param[in]     DTCAttr: The reference DTC attribute
 * @return        Dem_FFRefNumType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_FFRefNumType Dem_GetFFRef(Dem_DTCAttRefNumType DTCAttr)
{
    return Dem_GetFFRefOfDTCAttr(DTCAttr);
}

/**
 * @brief         Check if freeze frame reference is valid
 * @param[in]     DTCAttr: The reference DTC attribute
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckFFRef(Dem_DTCAttRefNumType DTCAttr)
{
    return (Dem_GetFFRef(DTCAttr) != DEM_ATTRI_INVALID_FREEZEFRAME); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/**
 * @brief Sample a UDS snapshot into the prestorage buffer
 */
DEM_LOCAL void Dem_GetFFFromEnv(Dem_EventIdType EventId, uint8* DestBuffer, uint16 BufferSize)
{
    Dem_DTCAttRefNumType lDTCAttr = Dem_GetDTCAttr(EventId);
    if ((Dem_CheckDTCAttr(lDTCAttr) == TRUE) && (Dem_CheckFFRef(lDTCAttr) == TRUE))
    {
        Dem_FFRefNumType          lFFRef = Dem_GetFFRef(lDTCAttr);
        Dem_DataInfoType          dataInfo;
        Dem_DestinationBufferType destBufferInfo;
        Dem_DestBufferInit(&destBufferInfo, DestBuffer, BufferSize);
        Dem_DataInfoInit(&dataInfo, &destBufferInfo, EventId);
        for (Dem_DIDRefNumType didIndex = Dem_GetDIDRefStartOfFF(lFFRef); didIndex < Dem_GetDIDRefEndOfFF(lFFRef);
             ++didIndex)
        {
            Dem_DIDRefNumType didRef = Dem_GetValueOfDIDRef(didIndex);
            Dem_GetEnvDataByDid(&dataInfo, didRef);
        }
    }
}

#endif

/**
 * @brief Init FF Iterator.
 */
DEM_LOCAL void
    Dem_InitFFIterator(Dem_FFEntryIterType* FFEntryIter, Dem_FFRNRefNumType FFRNStart, Dem_FFRNRefNumType FFRNEnd)
{
    FFEntryIter->FFRNStart = FFRNStart;
    FFEntryIter->FFRNEnd   = FFRNEnd;
}

#if ((DEM_OBDII_SUPPORT == STD_ON) && (DEM_MAX_SIZE_OBDFREEZEFRAME > 0u))
/**
 * @brief get UDS OBD freeze frame reference
 * @param[in]     DTCAttr: The reference DTC attribute
 * @return        Dem_FFRefNumType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_FFRefNumType Dem_GetUDSOBDRef(Dem_DTCAttRefNumType DTCAttr)
{
    return Dem_GetUDSOBDFFRefOfDTCAttr(DTCAttr);
}

/**
 * @brief Check if OBD freeze frame reference is valid
 * @param[in]     DTCAttr: The reference DTC attribute
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckOBDRef(Dem_DTCAttRefNumType DTCAttr)
{
    return ((Dem_GetUDSOBDRef(DTCAttr) != DEM_ATTRI_INVALID_OBDFREEZEFRAME) || (DEM_PID_CLASS_NUMBER > 0u));
}

/**
 * @brief Sample a OBD snapshot into the prestorage buffer
 */
DEM_LOCAL void Dem_GetOBDFFFromEnv(Dem_EventIdType EventId, uint8* DestBuffer, uint16 BufferSize)
{
    Dem_DTCAttRefNumType lDTCAttr = Dem_GetDTCAttr(EventId);
    if ((Dem_CheckDTCAttr(lDTCAttr) == TRUE) && (Dem_CheckOBDRef(lDTCAttr) == TRUE))
    {
        Dem_DataInfoType          dataInfo;
        Dem_DestinationBufferType destBufferInfo;
        Dem_DestBufferInit(&destBufferInfo, DestBuffer, BufferSize);
        Dem_DataInfoInit(&dataInfo, &destBufferInfo, EventId);
#if (DEM_PID_CLASS_NUMBER > 0u)
        for (uint8 pidIndex = 0; pidIndex < DEM_PID_CLASS_NUMBER; ++pidIndex)
        {
            Dem_GetEnvDataByPid(&dataInfo, pidIndex);
        }
#endif
#if (DEM_UDS_OBD_FREEZE_FRAME_ENABLE == STD_ON)
        {
            Dem_FFRefNumType lFFRef = Dem_GetFFRef(lDTCAttr);
            if (Dem_CheckFFRef(lDTCAttr) == TRUE)
            {
                for (Dem_DIDRefNumType didIndex = Dem_GetDIDRefStartOfFF(lFFRef);
                     didIndex < Dem_GetDIDRefEndOfFF(lFFRef);
                     ++didIndex)
                {
                    Dem_DIDRefNumType didRef = Dem_GetValueOfDIDRef(didIndex);
                    Dem_GetEnvDataByDid(&dataInfo, didRef);
                }
            }
        }
#endif
    }
}
#endif

#if (DEM_MAX_SIZE_J1939FREEZEFRAME > 0u)
/**
 * @brief get J1939 freeze frame reference
 * @param[in]     DTCAttr: The reference DTC attribute
 * @return        Dem_FFRefNumType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_FFRefNumType Dem_GetJ1939FFRef(Dem_DTCAttRefNumType DTCAttr)
{
    return Dem_GetJ1939FFRefOfDTCAttr(DTCAttr);
}

/**
 * @brief Check if J1939 freeze frame reference is valid
 * @param[in]     DTCAttr: The reference DTC attribute
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckJ1939FFRef(Dem_DTCAttRefNumType DTCAttr)
{
    return (Dem_GetJ1939FFRef(DTCAttr) != DEM_ATTRI_INVALID_J1939FREEZEFRAME); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/**
 * @brief Sample a j1939 snapshot into the prestorage buffer
 */
DEM_LOCAL void Dem_GetJ1939FFFromEnv(Dem_EventIdType EventId, uint8* DestBuffer, uint16 BufferSize)
{
    Dem_DTCAttRefNumType lDTCAttr = Dem_GetDTCAttr(EventId);
    if ((Dem_CheckDTCAttr(lDTCAttr) == TRUE) && (Dem_CheckJ1939FFRef(lDTCAttr) == TRUE))
    {
        Dem_J1939FFRefNumType     lJ1939FFRef = Dem_GetJ1939FFRef(lDTCAttr);
        Dem_DataInfoType          dataInfo;
        Dem_DestinationBufferType destBufferInfo;
        Dem_DestBufferInit(&destBufferInfo, DestBuffer, BufferSize);
        Dem_DataInfoInit(&dataInfo, &destBufferInfo, EventId);
        for (Dem_SPNCRefNumType lSPNIndex = Dem_GetSPNCRefStartOfJ1939FF(lJ1939FFRef);
             lSPNIndex < Dem_GetSPNCRefEndOfJ1939FF(lJ1939FFRef);
             ++lSPNIndex)
        {
            Dem_SPNCRefNumType didRef = Dem_GetValueOfSPNCRef(lSPNIndex);
            Dem_GetEnvDataBySPN(&dataInfo, didRef);
        }
    }
}
#endif

#if (DEM_MAX_SIZE_J1939EXFREEZEFRAME > 0u)
/**
 * @brief get J1939 Expanded freeze frame reference
 * @param[in]     DTCAttr: The reference DTC attribute
 * @return        Dem_FFRefNumType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_FFRefNumType Dem_GetJ1939ExFFRef(Dem_DTCAttRefNumType DTCAttr)
{
    return Dem_GetJ1939EFFRefOfDTCAttr(DTCAttr);
}

/**
 * @brief Check if J1939 Expanded freeze frame reference is valid
 * @param[in]     DTCAttr: The reference DTC attribute
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckJ1939ExFFRef(Dem_DTCAttRefNumType DTCAttr)
{
    return (Dem_GetJ1939ExFFRef(DTCAttr) != DEM_ATTRI_INVALID_J1939EXFREEZEFRAME); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/**
 * @brief Sample a j1939 expanded snapshot into the prestorage buffer
 */
DEM_LOCAL void Dem_GetJ1939ExFFFromEnv(Dem_EventIdType EventId, uint8* DestBuffer, uint16 BufferSize)
{
    Dem_DTCAttRefNumType lDTCAttr = Dem_GetDTCAttr(EventId);
    if ((Dem_CheckDTCAttr(lDTCAttr) == TRUE) && (Dem_CheckJ1939ExFFRef(lDTCAttr) == TRUE))
    {
        Dem_J1939FFRefNumType     lJ1939ExFFRef = Dem_GetJ1939ExFFRef(lDTCAttr);
        Dem_DataInfoType          dataInfo;
        Dem_DestinationBufferType destBufferInfo;
        Dem_DestBufferInit(&destBufferInfo, DestBuffer, BufferSize);
        Dem_DataInfoInit(&dataInfo, &destBufferInfo, EventId);
        for (Dem_SPNCRefNumType lSPNIndex = Dem_GetSPNCRefStartOfJ1939FF(lJ1939ExFFRef);
             lSPNIndex < Dem_GetSPNCRefEndOfJ1939FF(lJ1939ExFFRef);
             ++lSPNIndex)
        {
            Dem_SPNCRefNumType didRef = Dem_GetValueOfSPNCRef(lSPNIndex);
            Dem_GetEnvDataBySPN(&dataInfo, didRef);
        }
    }
}
#endif

#if (DEM_FEATURE_SWC_GET_FF_ED == STD_ON)
#if (DEM_PID_CLASS_NUMBER > 0u)
/**
 * @brief Extracts the OBD freezeframe, or a single PID from it
 * @param[in]     EventId: Event ID
 * @param[in]     DataId: This parameter specifies the DID (ISO14229-1) that shall be copied to the destination buffer.
 * @param[in]     DestinationBuffer: Pointer to destination buffer
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
DEM_LOCAL_INLINE Std_ReturnType Dem_EnvDataCopyPidFromObdFreezeFrame(
    Dem_EventIdType              EventId,
    uint16                       DataId,
    Dem_DestinationBufferPtrType DestinationBuffer)
/* PRQA S 3673 -- */
{
    Std_ReturnType ret;
    ret = DEM_NO_SUCH_ELEMENT;
    DEM_PARAM_UNUSED(EventId);
    DEM_PARAM_UNUSED(DataId);
    DEM_PARAM_UNUSED(DestinationBuffer);
    return ret;
}
#endif
#endif

#if (DEM_MAX_SIZE_FREEZEFRAME > 0u)
/**
 * @brief Initializes the snapshot entry iterator
 */
DEM_LOCAL void
    Dem_FFEntryIteratorInit(Dem_EventIdType EventId, Dem_NvBlockNumType MemoryIndex, Dem_FFEntryIterType* FFEntryIter)
{
    Dem_FFRNRefNumType lFFRNStart = 0u;
    Dem_FFRNRefNumType lFFRNEnd   = 0u;
    if (EventId != DEM_EVENT_INVALID)
    {
        Dem_DTCAttRefNumType lDTCAttr = Dem_GetDTCAttr(EventId);
#if (DEM_GENERAL_FF_RECNUM_CALCULATED == STD_ON)
        Dem_MemoryNumType                         memRef                   = Dem_GetMemRef(lDTCAttr);
        Dem_TypeOfFreezeFrameRecordNumerationType typeOfFFRecordNumeration = Dem_GetFFRecordOfMemory(memRef);
        if (typeOfFFRecordNumeration == DEM_FF_RECNUM_CALCULATED)
        {
            lFFRNStart = 0u;
            lFFRNEnd   = Dem_GetMaxNumFFROfDTCAttr(lDTCAttr);
        }
        else
#endif
        {
#if (DEM_GENERAL_FF_RECNUM_CONFIGURED == STD_ON)
            Dem_FFRNRefNumType lFFRNRef = Dem_GetFFRNRefOfDTCAttr(lDTCAttr);
            if (DEM_FREEZE_FRAME_REC_NUM_CLASS_NUMBER != lFFRNRef)
            {
                lFFRNStart = Dem_GetFFRRefStartOfFF(lFFRNRef);
                lFFRNEnd   = Dem_GetFFRRefEndOfFF(lFFRNRef);
            }
#endif
        }
    }
    Dem_InitFFIterator(FFEntryIter, lFFRNStart, lFFRNEnd);
    FFEntryIter->EventId      = EventId;
    FFEntryIter->MemoryIndex  = MemoryIndex;
    FFEntryIter->FFEntryIndex = 0u;
}

#if (DEM_FEATURE_SWC_GET_FF_ED == STD_ON)
/* PRQA S 5016 ++ */ /* VL_Dem_5016 */
/**
 * @brief Get the snapshot data buffer
 * @param[in]     FFEntryIter: Pointer to Iterator for the snapshot entries
 * @return        uint8*
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE const uint8* Dem_FFEntryIteratorGetFFRecordPtr(const Dem_FFEntryIterType* FFEntryIter)
{
    return Dem_GetFreezeFramePtrOfMemEntry(FFEntryIter->MemoryIndex, FFEntryIter->FFEntryIndex);
}
/* PRQA S 5016 -- */
#endif

/**
 * @brief Get the memory entry handle
 * @param[in]     FFEntryIter: Pointer to Iterator for the snapshot entries
 * @return        Dem_NvBlockNumType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_NvBlockNumType Dem_FFEntryIteratorGetMemoryEntryHandle(const Dem_FFEntryIterType* FFEntryIter)
{
    return FFEntryIter->MemoryIndex;
}

/**
 * @brief Test if the snapshot entry iterator points to a valid element
 * @param[in]     FFEntryIter: Pointer to Iterator for the snapshot entries
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_FFEntryIteratorExists(const Dem_FFEntryIterType* const FFEntryIter)
{
    boolean Exist = FALSE;
    if (FFEntryIter->MemoryIndex != DEM_MEM_INVALID_MEMORY_INDEX)
    {
        Exist = (boolean)(FFEntryIter->FFRNStart < FFEntryIter->FFRNEnd);
    }
    return Exist;
}

/**
 * @brief Move the snapshot entry iterator to the next element
 * @param[in]     FFEntryIter: Pointer to Iterator for the snapshot entries
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_FFEntryIteratorNext(Dem_FFEntryIterType* FFEntryIter)
{
    if (FFEntryIter->MemoryIndex != DEM_MEM_INVALID_MEMORY_INDEX)
    {
        FFEntryIter->FFRNStart++;
        FFEntryIter->FFEntryIndex++;
    }
}

#if (DEM_FEATURE_SWC_GET_FF_ED == STD_ON)
#if (DEM_GENERAL_FF_RECNUM_CALCULATED == STD_ON)
/**
 * @brief Move the snapshot entry iterator to the index of the last stored snapshot entry.
 */
DEM_LOCAL void Dem_FFEntryIteratorFindLastStoredEntry(
    Dem_EventIdType      EventId,
    Dem_NvBlockNumType   MemoryIndex,
    Dem_FFEntryIterType* FFEntryIter)
{
    Dem_FFEntryIteratorInit(EventId, MemoryIndex, FFEntryIter);
    while (Dem_FFEntryIteratorExists(FFEntryIter) == TRUE) /* PRQA S 2487 */ /* VL_Dem_2487 */
    {
        uint8 FFEntryIndex = FFEntryIter->FFEntryIndex;

        if ((Dem_CheckFFStored(FFEntryIndex, MemoryIndex) == TRUE) && (FFEntryIndex <= DEM_MAX_NUMBER_FREEZEFRAME))
        {
            if (Dem_CheckFFStored((FFEntryIndex + 1u), MemoryIndex) == FALSE)
            {
                break;
            }
        }
        Dem_FFEntryIteratorNext(FFEntryIter);
    }
}
#endif
#endif

/**
 * @brief Move the snapshot entry iterator to the index of the snapshot entry with requested record number.
 */
DEM_LOCAL void Dem_FFEntryIteratorFindEntryWithRecordNumber(
    Dem_EventIdType      EventId,
    uint8                RecordNumber,
    Dem_NvBlockNumType   MemoryIndex,
    Dem_FFEntryIterType* FFEntryIter)
{
    Dem_FFEntryIteratorInit(EventId, MemoryIndex, FFEntryIter);
    while (Dem_FFEntryIteratorExists(FFEntryIter) == TRUE) /* PRQA S 2487 */ /* VL_Dem_2487 */
    {
#if ((DEM_GENERAL_FF_RECNUM_CONFIGURED == STD_ON) && (DEM_GENERAL_FF_RECNUM_CALCULATED == STD_ON))
        Dem_MemoryNumType memRef = Dem_GetMemRef(Dem_GetDTCAttr(EventId));
#endif
        Dem_TypeOfFreezeFrameRecordNumerationType typeOfFFRecordNumeration = Dem_GetFFRecordOfMemory(memRef);
        uint8                                     lFFEntryIndex            = FFEntryIter->FFEntryIndex;
#if (DEM_GENERAL_FF_RECNUM_CONFIGURED == STD_ON)
        Dem_FFRRefNumType lFFRecordRef = Dem_GetValueOfDFFRRef(FFEntryIter->FFRNStart);
#endif
        if (
#if ((DEM_GENERAL_FF_RECNUM_CALCULATED == STD_ON) && (DEM_GENERAL_FF_RECNUM_CONFIGURED == STD_ON))
            (
#endif
#if (DEM_GENERAL_FF_RECNUM_CALCULATED == STD_ON)
                ((typeOfFFRecordNumeration == DEM_FF_RECNUM_CALCULATED) && ((lFFEntryIndex + 1u) == RecordNumber))
#endif
#if ((DEM_GENERAL_FF_RECNUM_CALCULATED == STD_ON) && (DEM_GENERAL_FF_RECNUM_CONFIGURED == STD_ON))
                ||
#endif
#if (DEM_GENERAL_FF_RECNUM_CONFIGURED == STD_ON)
                ((typeOfFFRecordNumeration == DEM_FF_RECNUM_CONFIGURED)
                 && (Dem_GetFFRNumberOfFFRC(lFFRecordRef) == RecordNumber))
#endif
#if ((DEM_GENERAL_FF_RECNUM_CALCULATED == STD_ON) && (DEM_GENERAL_FF_RECNUM_CONFIGURED == STD_ON))
                    )
#endif
            && (Dem_CheckFFStored(lFFEntryIndex, MemoryIndex) == TRUE)) /* PRQA S 3415 */ /* VL_Dem_3415 */
        {
            break;
        }
        Dem_FFEntryIteratorNext(FFEntryIter);
    }
}

#if (DEM_FEATURE_SWC_GET_FF_ED == STD_ON)
/**
 * @brief Extracts a snapshot record, or a single DID from it.
 */
DEM_LOCAL Std_ReturnType Dem_DataReportCopyDidFF(
    uint16                       DataId,
    Dem_NvBlockNumType           MemoryIndex,
    const uint8*                 SourceBuffer,
    Dem_DestinationBufferPtrType DestinationBuffer)
{
    Std_ReturnType       ret      = DEM_NO_SUCH_ELEMENT;
    Dem_EventIdType      eventId  = Dem_GetEventIdOfMemEntry(MemoryIndex);
    Dem_DTCAttRefNumType lDTCAttr = Dem_GetDTCAttr(eventId);
    Dem_FFRefNumType     lFFRef   = Dem_GetFFRef(lDTCAttr);
    Dem_DataInfoType     copyDidInfo;

    /** Initialize a CopyDid Context */
    if (Dem_CheckFFRef(lDTCAttr) == TRUE)
    {
        Dem_DataReportInitCopyDidInfo(&copyDidInfo, DestinationBuffer, SourceBuffer, eventId, MemoryIndex);
        for (Dem_DIDRefNumType didIndex = Dem_GetDIDRefStartOfFF(lFFRef); didIndex < Dem_GetDIDRefEndOfFF(lFFRef);
             ++didIndex)
        {
            Dem_DIDRefNumType didRef = Dem_GetValueOfDIDRef(didIndex);
            if (Dem_GetIdentifierOfDidClass(didRef) == DataId)
            {
                if (Dem_GetSizeOfDidClass(didRef) <= copyDidInfo.DestinationBuffer->BufferSize)
                {
                    (void)Dem_DataReportCopyDid(&copyDidInfo, didRef);
                    ret = E_OK;
                }
                else
                {
                    ret = DEM_BUFFER_TOO_SMALL;
                }
                break;
            }
            else
            {
                /** not the requested DID */
                Dem_DataReportSkipDid(&copyDidInfo, didRef);
            }
        }
    }
    return ret;
}

/**
 * @brief Extracts a snapshot record, or a single DID from it
 */
/* PRQA S 6030,6070 ++ */ /* VL_MTR_Dem_STMIF,VL_MTR_Dem_STCAL */
DEM_LOCAL Std_ReturnType Dem_EnvDataCopyDidFromFF(
    Dem_EventIdType              EventId,
    uint8                        RecordNumber,
    uint16                       DataId,
    Dem_DestinationBufferPtrType DestinationBuffer)
{
    Std_ReturnType       ret;
    Dem_DTCAttRefNumType lDTCAttr = Dem_GetDTCAttr(EventId);

    if (lDTCAttr != DEM_DTC_ATTRIBUTES_NUMBER)
    {
#if ((DEM_GENERAL_FF_RECNUM_CONFIGURED == STD_ON) && (DEM_GENERAL_FF_RECNUM_CALCULATED == STD_ON))
        Dem_MemoryNumType memRef = Dem_GetMemRef(lDTCAttr);
#endif
        Dem_TypeOfFreezeFrameRecordNumerationType typeOfFFRecordNumeration = Dem_GetFFRecordOfMemory(memRef);
#if (DEM_GENERAL_FF_RECNUM_CONFIGURED == STD_ON)
        if ((RecordNumber == DEM_DCM_SNAPSHOTDATARECORD_ALL) && (typeOfFFRecordNumeration == DEM_FF_RECNUM_CONFIGURED))
        {
            ret = E_NOT_OK;
        }
        else
#endif
        {
            Dem_MemStateInfoConstPtrType memoryInfo = Dem_MemStateInfoInit(Dem_GetMemRef(Dem_GetDTCAttr(EventId)));
            Dem_NvBlockNumType           memoryIndex;
            boolean                      modificationDetected;
            do
            {
                modificationDetected = FALSE;
                memoryIndex          = Dem_MemoryFindIndex(memoryInfo, EventId);
                if (memoryIndex != DEM_MEM_INVALID_MEMORY_INDEX)
                {
                    /** The record currently exists, but without critical section it could age or displace */
                    boolean updateState = Dem_MemoryUpdateGetState(memoryIndex);
                    if (updateState == FALSE)
                    {
                        /** This call does not currently interrupt a concurrent data update */
                        Dem_FFEntryIterType lFFEntryIter;
#if (DEM_GENERAL_FF_RECNUM_CALCULATED == STD_ON)
                        if ((RecordNumber == DEM_DCM_SNAPSHOTDATARECORD_ALL)
                            && (typeOfFFRecordNumeration == DEM_FF_RECNUM_CALCULATED))
                        {
                            Dem_FFEntryIteratorFindLastStoredEntry(EventId, memoryIndex, &lFFEntryIter);
                        }
                        else
#endif
                        {
                            Dem_FFEntryIteratorFindEntryWithRecordNumber(
                                EventId,
                                RecordNumber,
                                memoryIndex,
                                &lFFEntryIter);
                        }

                        if (Dem_FFEntryIteratorExists(&lFFEntryIter) == TRUE)
                        {
                            ret = Dem_DataReportCopyDidFF(
                                DataId,
                                Dem_FFEntryIteratorGetMemoryEntryHandle(&lFFEntryIter),
                                Dem_FFEntryIteratorGetFFRecordPtr(&lFFEntryIter),
                                DestinationBuffer);
                        }
                        else
                        {
                            ret = DEM_NO_SUCH_ELEMENT;
                        }

                        if ((ret == E_OK) && (Dem_MemoryUpdateGetState(memoryIndex) != updateState))
                        {
                            /** The memory entry has been modified while trying to copy the stored data. */
                            /** We cannot be sure it is still used by the same event, so start from the beginning */
                            modificationDetected = TRUE;
                        }
                    }
                    else
                    {
                        /** The caller task has higher priority than Dem_MainFunction, but we cannot 'wait' for the
                         * update to complete. The requested data is not available at this moment. */
                        ret = E_NOT_OK;
                    }
                }
                else
                {
                    /** The requested event is currently not stored in the event memory */
                    ret = DEM_NO_SUCH_ELEMENT;
                }
            } while (modificationDetected == TRUE);
        }
    }
    return ret;
}
#endif
/* PRQA S 6030,6070 -- */
#endif

#if (DEM_FEATURE_SWC_GET_FF_ED == STD_ON)
/**
 * @brief Implementation of SWC API Dem_GetEventFreezeFrameDataEx().
 */
DEM_LOCAL Std_ReturnType Dem_EnvDataSwcApiGetEventFreezeFrameDataEx(
    Dem_EventIdType EventId,
    uint8           RecordNumber,
    uint16          DataId,
    uint8*          DestBuffer,
    uint16*         BufSize)
{
    Std_ReturnType            ret = E_NOT_OK;
    Dem_DestinationBufferType destinationBuffer;

    /** Initialize a Destination Buffer */
    Dem_DestBufferInit(&destinationBuffer, DestBuffer, *BufSize);
    *BufSize = 0u;
    if ((Dem_GetAvailableInVariantByEvent(EventId) == TRUE) && (Dem_GetEventAvailableStatusByEventId(EventId) == TRUE))
    {
        if (RecordNumber == 0x00u)
        {
#if (DEM_PID_CLASS_NUMBER > 0u)
            ret = Dem_EnvDataCopyPidFromObdFreezeFrame(EventId, DataId, &destinationBuffer);
#elif (DEM_UDS_OBD_FREEZE_FRAME_ENABLE == STD_ON)
            ret = Dem_EnvDataCopyDidFromWwhObdFreezeFrame(EventId, DataId, &destinationBuffer);
#else
            ret = DEM_NO_SUCH_ELEMENT;
#endif
        }
#if (DEM_MAX_SIZE_FREEZEFRAME > 0u)
        else
        {
            ret = Dem_EnvDataCopyDidFromFF(EventId, RecordNumber, DataId, &destinationBuffer);
        }
#endif
#if ((DEM_PID_CLASS_NUMBER > 0u) || (DEM_UDS_OBD_FREEZE_FRAME_ENABLE == STD_ON) || (DEM_MAX_SIZE_FREEZEFRAME > 0u))
        if (ret == E_OK)
        {
            *BufSize = destinationBuffer.WriteIndex;
        }
#endif
    }
    return ret;
}
#endif

#if (DEM_DCM_CLIENT_NUMBER > 0u)
/**
 * @brief Counts the number of snapshot records stored in a memory entry
 */
DEM_LOCAL uint8 Dem_FFEntryGetNumberOfFFRecords(Dem_NvBlockNumType MemoryIndex)
{
    uint8        numberOfRecords = 0u;
    const uint8* lFFRecord       = Dem_GetFreezeFrameRecordPtrOfMemEntry(MemoryIndex);
    for (uint8 wordPos = 0u; wordPos < DEM_MAX_NUMBER_FREEZEFRAMERECORD; ++wordPos)
    {
        uint8 lFFHeader = lFFRecord[wordPos];
        while (lFFHeader != 0u)
        {
            numberOfRecords = (uint8)(numberOfRecords + (lFFHeader & 0x01u));
            lFFHeader       = (uint8)(lFFHeader >> 0x01u);
        }
    }
    return numberOfRecords;
}
#endif

#if ((DEM_DCM_CLIENT_NUMBER > 0u) || (DEM_J1939DCM_CLIENT_NUMBER > 0u))
/**
 * @brief Get the filter data of a freeze frame iterator.
 * @param[in]     DcmClientId: Identification of a client.
 * @return        Dem_FFIteratorFilterType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_FFIteratorFilterType Dem_FFIteratorGetFreezeFrameFilterData(uint8 DcmClientId)
{
    return Dem_FFIteratorTable[DcmClientId];
}

/**
 * @brief Set the filter data of a freeze frame iterator.
 * @param[in]     DcmClientId: Identification of a client.
 * @param[in]     FFFilterData: Iterator for the snapshot entries
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_FFIteratorSetFreezeFrameFilterData(uint8 DcmClientId, Dem_FFIteratorFilterType FFFilterData)
{
    Dem_FFIteratorTable[DcmClientId] = FFFilterData;
}

/**
 * @brief Get the DTCFormat of the FreezeFrameIterator.
 * @param[in]     DcmClientId: Identification of a client.
 * @return        Dem_FFIteratorDTCFormatType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_FFIteratorDTCFormatType Dem_FFIteratorGetDTCFormat(uint8 DcmClientId)
{
    return Dem_FFIteratorTable[DcmClientId].FilterDTCFormat;
}

/**
 * @brief Initialize FreezeFrameIterator
 * @param[in]     DcmClientId: Identification of a client.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_FFIteratorInitIterator(uint8 DcmClientId)
{
    Dem_FFEntryIterType      lFFEntryIter;
    Dem_FFIteratorFilterType lFFFilterData;

    Dem_FFEntryIteratorInit(DEM_EVENT_INVALID, DEM_MEM_INVALID_MEMORY_INDEX, &lFFEntryIter);
    lFFFilterData                 = Dem_FFIteratorGetFreezeFrameFilterData(DcmClientId);
    lFFFilterData.MemoryIndex     = DEM_MEM_INVALID_MEMORY_INDEX;
    lFFFilterData.FFEntryIter     = lFFEntryIter;
    lFFFilterData.FilterDTCFormat = DEM_FREEZEFRAMEITERATOR_DTCFORMAT_INIT;
#if (DEM_J1939_SUPPORT == STD_ON)
    lFFFilterData.FilterState = DEM_FREEZEFRAMEITERATOR_J1939STATE_INIT;
#endif
    Dem_FFIteratorSetFreezeFrameFilterData(DcmClientId, lFFFilterData);
}

#if (DEM_MAX_SIZE_FREEZEFRAME > 0u)
/**
 * @brief Initialize (start) FreezeFrameIterator with given criteria.
 * @param[in]     DcmClientId: Identification of a client.
 * @param[in]     FilterDTCFormat: Filter DTC Format
 * @param[in]     MemoryIndex: Memory index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_FFIteratorStartIterator(
    uint8                       DcmClientId,
    Dem_FFIteratorDTCFormatType FilterDTCFormat,
    Dem_NvBlockNumType          MemoryIndex)
{
    Dem_FFEntryIterType      lFFEntryIter;
    Dem_FFIteratorFilterType lFFFilterData;

    Dem_FFEntryIteratorInit(DEM_EVENT_INVALID, DEM_MEM_INVALID_MEMORY_INDEX, &lFFEntryIter);
    lFFFilterData                 = Dem_FFIteratorGetFreezeFrameFilterData(DcmClientId);
    lFFFilterData.MemoryIndex     = MemoryIndex;
    lFFFilterData.FFEntryIter     = lFFEntryIter;
    lFFFilterData.FilterDTCFormat = FilterDTCFormat;
#if (DEM_J1939_SUPPORT == STD_ON)
    lFFFilterData.FilterState = DEM_FREEZEFRAMEITERATOR_J1939STATE_INIT;
#endif
    Dem_FFIteratorSetFreezeFrameFilterData(DcmClientId, lFFFilterData);
}

/**
 * @brief  Move the snapshot entry iterator to the index of the next stored snapshot entry.
 */
DEM_LOCAL boolean Dem_FFEntryIteratorSetNextStoredEntry(
    Dem_TypeOfFreezeFrameRecordNumerationType TypeOfFFRecordNumeration,
    Dem_FFEntryIterType*                      FFEntryIter)
{
    boolean foundEntry = FALSE;
    if (FFEntryIter->MemoryIndex != DEM_MEM_INVALID_MEMORY_INDEX)
    {
#if (DEM_GENERAL_FF_RECNUM_CONFIGURED == STD_ON)
#if (DEM_GENERAL_FF_RECNUM_CALCULATED == STD_ON)
        if (TypeOfFFRecordNumeration == DEM_FF_RECNUM_CALCULATED)
#else
        DEM_PARAM_UNUSED(TypeOfFFRecordNumeration);
#endif
        {
            while (Dem_FFEntryIteratorExists(FFEntryIter) == TRUE) /* PRQA S 2487 */ /* VL_Dem_2487 */
            {
                if (Dem_CheckFFStored(FFEntryIter->FFEntryIndex, FFEntryIter->MemoryIndex) == TRUE)
                {
                    foundEntry = TRUE;
                    break;
                }
                else
                {
                    Dem_FFEntryIteratorNext(FFEntryIter);
                }
            }
        }
#endif
#if (DEM_GENERAL_FF_RECNUM_CALCULATED == STD_ON)
#if (DEM_GENERAL_FF_RECNUM_CONFIGURED == STD_ON)
        if (TypeOfFFRecordNumeration == DEM_FF_RECNUM_CALCULATED)
#else
        DEM_PARAM_UNUSED(TypeOfFFRecordNumeration);
#endif
        {
            if (Dem_CheckFFStored(FFEntryIter->FFEntryIndex, FFEntryIter->MemoryIndex) == TRUE)
            {
                foundEntry = TRUE;
            }
        }
#endif
    }
    return foundEntry;
}

/**
 * @brief Gets the number of stored snapshot records
 */
DEM_LOCAL uint16 Dem_FFIteratorGetNumberOfFF(uint8 ClientId)
{
    /** Count stored snapshot records in primary memory to prevent concurrent modification of the chrono index to have
     * an effect on the iteration */
    Dem_MemStateInfoConstPtrType memoryInfo =
        Dem_MemStateInfoInit(Dem_GetPrimaryRefOfMemSet(Dem_GetMemorySetRefOfClient(ClientId)));
    Dem_NvBlockNumType memoryIndexEnd = Dem_GetMemEntryEndOfMemoryInfo(memoryInfo);
    Dem_NvBlockNumType memoryIndex    = Dem_GetMemEntryStartOfMemoryInfo(memoryInfo);
    uint16             recordCount    = 0u;

    while (memoryIndex < memoryIndexEnd)
    {
        Dem_EventIdType eventId = Dem_GetEventIdOfMemEntry(memoryIndex);
        if ((Dem_CheckEventIdInValid(eventId) == FALSE) && (Dem_CheckDTCSuppressed(eventId) == FALSE))
        {
            /** Occupied entry */
            recordCount = (uint16)(recordCount + Dem_FFEntryGetNumberOfFFRecords(memoryIndex));
        }
        ++memoryIndex;
    }
    return recordCount;
}

/**
 * @brief Filters the snapshot records of events stored in primary memory.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
DEM_LOCAL Std_ReturnType Dem_FFIteratorGetNext(uint8 ClientId, uint32* DTC, uint8* RecordNumber)
{
    Std_ReturnType               ret = DEM_NO_SUCH_ELEMENT;
    Dem_MemStateInfoConstPtrType memoryInfo =
        Dem_MemStateInfoInit(Dem_GetPrimaryRefOfMemSet(Dem_GetMemorySetRefOfClient(ClientId)));
    Dem_NvBlockNumType       memoryIndexEnd        = Dem_GetMemEntryEndOfMemoryInfo(memoryInfo);
    uint8                    dcmClientId           = Dem_GetDcmClientIdOfClient(ClientId);
    Dem_FFIteratorFilterType freezeFrameFilterData = Dem_FFIteratorGetFreezeFrameFilterData(dcmClientId);
    Dem_TypeOfFreezeFrameRecordNumerationType typeOfFFRecordNumeration =
        Dem_GetFFRecordOfMemory(Dem_GetPrimaryRefOfMemSet(Dem_GetMemorySetRefOfClient(ClientId)));
    Dem_FFEntryIterType* lFFEntryIter = &freezeFrameFilterData.FFEntryIter;

    while (freezeFrameFilterData.MemoryIndex < memoryIndexEnd)
    {
        Dem_NvBlockNumType memoryIndex = freezeFrameFilterData.MemoryIndex;
        Dem_EventIdType    eventId     = Dem_GetEventIdOfMemEntry(memoryIndex);
        if ((Dem_CheckEventIdInValid(eventId) == FALSE) && (Dem_CheckDTCSuppressed(eventId) == FALSE))
        {
#if (DEM_OBDII_SUPPORT == STD_ON)
            /** Skip Events which do not have an OBD DTC but filter is set for OBD format */
            if ((Dem_CheckEventObdRelated(eventId) == TRUE)
                || (freezeFrameFilterData.FilterDTCFormat != DEM_FREEZEFRAMEITERATOR_DTCFORMAT_OBD))
#endif
            {
                if (Dem_FFEntryIteratorGetMemoryEntryHandle(lFFEntryIter) == DEM_MEM_INVALID_MEMORY_INDEX)
                {
                    Dem_FFEntryIteratorInit(eventId, memoryIndex, lFFEntryIter);
                }

                if (Dem_FFEntryIteratorSetNextStoredEntry(typeOfFFRecordNumeration, lFFEntryIter) == TRUE)
                {
#if (DEM_OBDII_SUPPORT == STD_ON)
                    if (freezeFrameFilterData.FilterDTCFormat == DEM_FREEZEFRAMEITERATOR_DTCFORMAT_OBD)
                    {
                        *DTC = Dem_GetEventObdDTC(eventId);
                    }
                    else
#endif
#if (DEM_J1939_SUPPORT == STD_ON)
                        if (freezeFrameFilterData.FilterDTCFormat == DEM_FREEZEFRAMEITERATOR_DTCFORMAT_J1939)
                    {
                        *DTC = Dem_GetEventJ1939DTC(eventId);
                    }
                    else
#endif
                    {
                        /** Only EventIds with a valid DTC number are processed here */
                        *DTC = Dem_GetEventDTC(eventId);
                    }

#if (DEM_GENERAL_FF_RECNUM_CALCULATED == STD_ON)
                    if (typeOfFFRecordNumeration == DEM_FF_RECNUM_CALCULATED)
                    {
                        *RecordNumber = (uint8)(lFFEntryIter->FFEntryIndex + 1u);
                    }
                    else
#endif
                    {
#if (DEM_GENERAL_FF_RECNUM_CONFIGURED == STD_ON)
                        *RecordNumber = Dem_GetFFRNumberOfFFRC(lFFEntryIter->FFEntryIndex + lFFEntryIter->FFRNStart);
#endif
                    }

                    /** Increment in global state, the next iteration will continue with the next record */
                    Dem_FFEntryIteratorNext(lFFEntryIter);
                    Dem_FFIteratorSetFreezeFrameFilterData(dcmClientId, freezeFrameFilterData);
                    ret = E_OK;
                    break;
                }
            }
        }
        if (ret == DEM_NO_SUCH_ELEMENT)
        {
            ++freezeFrameFilterData.MemoryIndex;
            Dem_FFEntryIteratorInit(eventId, DEM_MEM_INVALID_MEMORY_INDEX, lFFEntryIter);
        }
    }
    Dem_FFIteratorSetFreezeFrameFilterData(dcmClientId, freezeFrameFilterData);
    return ret;
}
/* PRQA S 6070 -- */
#endif
#endif

#if (DEM_DCM_CLIENT_NUMBER > 0u)
#if (DEM_OBDII_SUPPORT == STD_ON)
/**
 * @brief Calculates the total size of the OBD II freeze frame, when formatted as UDS snapshot record (19 04)
 */
DEM_LOCAL Std_ReturnType Dem_GetSizeOfFFSelectionCalculateSizeObd(
    Dem_EventIdType    EventId,
    Dem_NvBlockNumType MemoryIndex,
    uint32*            SizeOfFreezeFrame)
{
    Std_ReturnType ret;
    if (Dem_CheckEventObdRelated(EventId) == FALSE)
    {
        ret = DEM_NO_SUCH_ELEMENT;
    }
    else
    {
        /** Check if FreezeFrame is available */
        if (TRUE == Dem_CheckOBDFFStored(MemoryIndex))
        {
#if (DEM_PID_CLASS_NUMBER > 0u)
            *SizeOfFreezeFrame = DEM_PID_CLASS_TOTAL_SIZE + 1u;
#endif
#if (DEM_UDS_OBD_FREEZE_FRAME_ENABLE == STD_ON)
            *SizeOfFreezeFrame = Dem_GetFFSizeOfFF(Dem_GetFFRef(Dem_GetDTCAttr(EventId))) + 1u;
#endif
        }
        else
        {
            /** FreezeFrame is not stored */
            *SizeOfFreezeFrame = 0;
        }
        ret = E_OK;
    }
    return ret;
}
#endif

#if ((DEM_MAX_SIZE_FREEZEFRAME > 0u) || (DEM_OBDII_SUPPORT == STD_ON))
/**
 * @brief Calculates the size of all snapshot records
 */
DEM_LOCAL Std_ReturnType
    Dem_GetSizeOfFFSelectionCalculateSizeAll(Dem_ReadoutBufferEntryType ReadoutBuffer, uint32* SizeOfFreezeFrame)
{
    Std_ReturnType     ret;
    Dem_NvBlockNumType memoryIndex = ReadoutBuffer.FFIterator.FFEntryIter.MemoryIndex;
    Dem_EventIdType    eventId     = ReadoutBuffer.EventId;

    /** No data is a positive result for 0xff */
#if (DEM_MAX_SIZE_FREEZEFRAME > 0u)
    Dem_FFRefNumType ref = Dem_GetFFRef(Dem_GetDTCAttr(eventId));

    ret = E_OK;
    /* PRQA S 4391 ++ */ /* VL_Dem_4391 */
    *SizeOfFreezeFrame =
        (uint32)((uint32)Dem_FFEntryGetNumberOfFFRecords(memoryIndex) * (uint32)(Dem_GetFFSizeOfFF(ref) + 1u));
    /* PRQA S 4391 -- */
#else
    ret = DEM_NO_SUCH_ELEMENT;
    /** SizeOfFreezeFrame intentionally not modified due to invalid request */
#endif

    /** Add the size of the OBD freeze frame */
#if (DEM_OBDII_SUPPORT == STD_ON)
    {
        Dem_EventIdType lEventId;
        uint32          lSizeOfFreezeFrame;
        if (Dem_GetSizeOfFFSelectionCalculateSizeObd(eventId, memoryIndex, &lSizeOfFreezeFrame) == E_OK)
        {
#if (DEM_MAX_SIZE_FREEZEFRAME > 0u)
            /** ret = E_OK already set */
            *SizeOfFreezeFrame = (uint32)(*SizeOfFreezeFrame + lSizeOfFreezeFrame);
#else
            *SizeOfFreezeFrame = lSizeOfFreezeFrame;
#endif
        }
    }
#endif
    return ret;
}

/**
 * @brief Test whether filter for snapshot record(s) matches
 */
DEM_LOCAL boolean Dem_GetNextFFDataCheckRecordMatch(uint8 RecordNumberFilter, uint8 RecordNumber)
{
    boolean match;
    if (RecordNumberFilter == DEM_DCM_SNAPSHOTDATARECORD_ALL)
    {
#if (DEM_OBDII_SUPPORT == STD_ON)
        /** exclude OBD Snapshot if necessary */
        if (RecordNumber == DEM_DCM_SNAPSHOTDATARECORD_OBD)
        {
            match = FALSE;
        }
        else
#endif
        {
            match = TRUE;
        }
    }
    else
    {
        if (RecordNumberFilter == RecordNumber)
        {
            match = TRUE;
        }
        else
        {
            match = FALSE;
        }
    }
    return match;
}

#endif

#if (DEM_MAX_SIZE_FREEZEFRAME > 0u)
/**
 * @brief Calculates the size of a standard snapshot record
 */
DEM_LOCAL Std_ReturnType Dem_GetSizeOfFFSelectionCalculateSizeStd(
    Dem_ReadoutBufferEntryType ReadoutBuffer,
    uint8                      RecordNumber,
    uint32*                    SizeOfFreezeFrame)
{
    Std_ReturnType ret = E_OK;
    /** Specific record not found is a negative result */
    Dem_FFEntryIterType lFFEntryIter;
    Dem_EventIdType     eventId = ReadoutBuffer.EventId;

    Dem_FFEntryIteratorFindEntryWithRecordNumber(
        eventId,
        RecordNumber,
        ReadoutBuffer.FFIterator.FFEntryIter.MemoryIndex,
        &lFFEntryIter);

    if (Dem_FFEntryIteratorExists(&lFFEntryIter) == FALSE)
    {
        ret = DEM_NO_SUCH_ELEMENT;
    }
    else
    {
        Dem_FFRefNumType ref = Dem_GetFFRef(Dem_GetDTCAttr(eventId));
        *SizeOfFreezeFrame   = Dem_GetFFSizeOfFF(ref) + 1u; /* PRQA S 4491 */ /* VL_Dem_4491 */
    }
    return ret;
}

/**
 * @brief Copies the contents of a stored snapshot record
 */
DEM_LOCAL void Dem_DataReportFFCopyData(
    Dem_DestinationBufferPtrType DestinationBuffer,
    const uint8*                 SourceBuffer,
    Dem_EventIdType              EventId,
    Dem_NvBlockNumType           MemoryIndex)
{
    /** Reserve one byte for Did count */
    uint16 writeIndex = DestinationBuffer->WriteIndex;
    if (Dem_CheckAndIncreaseDestBufferWriteIndex(DestinationBuffer, 1u) == TRUE)
    {
        /** Initialize number of DIDs */
        uint16           didCount = 0u;
        Dem_FFRefNumType lFFRef   = Dem_GetFFRef(Dem_GetDTCAttr(EventId));
        Dem_DataInfoType dataInfo;
        /** Initialize a CopyDid Context. */
        Dem_DataReportInitCopyDidInfo(&dataInfo, DestinationBuffer, SourceBuffer, EventId, MemoryIndex);

        for (Dem_DIDRefNumType didIndex = Dem_GetDIDRefStartOfFF(lFFRef); didIndex < Dem_GetDIDRefEndOfFF(lFFRef);
             ++didIndex)
        {
            Dem_DIDRefNumType didRef = Dem_GetValueOfDIDRef(didIndex);
            ++didCount;
            Dem_WriteUint16ToBuffer(DestinationBuffer, Dem_GetIdentifierOfDidClass(didRef));
            (void)Dem_DataReportCopyDid(&dataInfo, didRef);
        }
        /** Set the DID count to 0 in case of more than 255 DIDs. */
        if (didCount > DEM_MAX_DID_COUNT)
        {
            didCount = 0u;
        }
        /** first byte contains record number */
        Dem_WriteDestinationBufferAtIndex(DestinationBuffer, writeIndex, Dem_GetLoByte(didCount));
    }
}
#endif

#if (DEM_OBDII_SUPPORT == STD_ON)

/**
 * @brief Copies the contents of an OBD freeze frame.
 */
DEM_LOCAL void Dem_DataReportObdFreezeFrameCopyUdsData(
    Dem_NvBlockNumType           MemoryIndex,
    Dem_EventIdType              EventId,
    Dem_DestinationBufferPtrType DestinationBuffer)
{
    if ((DestinationBuffer->BufferSize - DestinationBuffer->WriteIndex) > 0)
    {
        /** initialize PID counter */
        uint16 didCount = 0;
        /** Initialize a CopyDid Context. Reserve one byte at the start for the DID count. */
        uint16           writeIndex = DestinationBuffer->WriteIndex;
        Dem_DataInfoType dataInfo;

        (void)Dem_CheckAndIncreaseDestBufferWriteIndex(DestinationBuffer, 1);
        Dem_DataReportInitCopyDidInfo(
            &dataInfo,
            DestinationBuffer,
            (const uint8*)Dem_GetOBDFreezeFramePtrOfMemEntry(MemoryIndex),
            DEM_EVENT_INVALID,
            DEM_MEM_INVALID_MEMORY_INDEX);

#if (DEM_PID_CLASS_NUMBER > 0u)
        for (uint8 pidIndex = 0; pidIndex < DEM_PID_CLASS_NUMBER; ++pidIndex)
        {
            Dem_GetEnvDataByPid(&dataInfo, pidIndex);
            /** Count the PIDs */
            ++didCount;
            /** Store the UDS DID number 0xF4 <PID> into the destination buffer */
            Dem_WriteUint8ToBuffer(DestinationBuffer, 0xF4u);
            Dem_WriteUint8ToBuffer(DestinationBuffer, Dem_GetPidIdentifier(pidIndex));
            /** Copy the PID data into the destination buffer */
            (void)Dem_DataReportCopyPid(&dataInfo, pidIndex);
        }
#endif
#if (DEM_UDS_OBD_FREEZE_FRAME_ENABLE == STD_ON)
        {
            Dem_FFRefNumType lFFRef = Dem_GetFFRef(Dem_GetDTCAttr(EventId));
            for (Dem_DIDRefNumType didIndex = Dem_GetDIDRefStartOfFF(lFFRef); didIndex < Dem_GetDIDRefEndOfFF(lFFRef);
                 ++didIndex)
            {
                Dem_DIDRefNumType didRef = Dem_GetValueOfDIDRef(didIndex);
                /** Count the PIDs */
                ++didCount;
                /** Store the UDS DID number 0xF4 <PID> into the destination buffer */
                Dem_WriteUint16ToBuffer(DestinationBuffer, Dem_GetIdentifierOfDidClass(didRef));
                /** Copy thes data into the destination buffer */
                (void)Dem_DataReportCopyDid(&dataInfo, didIndex);
            }
        }
#endif
        /** Store the number of PIDs to the first byte of the destination buffer. Set the DID count to 0 in case of more
         * than 255 DIDs. */
        if (didCount > 255)
        {
            didCount = 0;
        }
        Dem_WriteDestinationBufferAtIndex(DestinationBuffer, writeIndex, Dem_GetLoByte(didCount));
    }
}

/**
 * @brief Validates the request to get the OBD FreezeFrame in UDS Format, and copies the data.
 */
DEM_LOCAL Std_ReturnType Dem_GetNextFFDataCopyNextRecordObd(
    Dem_ReadoutBufferEntryType   ReadoutBuffer,
    Dem_DestinationBufferPtrType DestinationBuffer)
{
    Std_ReturnType  ret;
    Dem_EventIdType eventId = ReadoutBuffer.EventId;

    if (Dem_CheckEventObdRelated(eventId) == FALSE)
    {
        ret = DEM_NO_SUCH_ELEMENT;
    }
    else
    {
        Dem_NvBlockNumType memoryIndex = ReadoutBuffer.FFIterator.FFEntryIter.MemoryIndex;
        if (FALSE == Dem_CheckOBDFFStored(memoryIndex))
        {
            /** FreezeFrame is not stored */
            ret = E_OK;
        }
        else
        {
            uint16 size;
            /** Event has a stored OBD freeze frame */
#if (DEM_PID_CLASS_NUMBER > 0u)
            size = DEM_PID_CLASS_TOTAL_SIZE + 1u;
#endif
#if (DEM_UDS_OBD_FREEZE_FRAME_ENABLE == STD_ON)
            size = Dem_GetFFSizeOfFF(Dem_GetFFRef(Dem_GetDTCAttr(eventId))) + 1u;
#endif
            if (size > DestinationBuffer->BufferSize)
            {
                /** supported record, but the buffer is not large enough for the data */
                ret = DEM_BUFFER_TOO_SMALL;
            }
            else
            {
                /** Read Freeze Frame */
                Dem_WriteUint8ToBuffer(DestinationBuffer, 0x00u);
                Dem_DataReportObdFreezeFrameCopyUdsData(memoryIndex, eventId, DestinationBuffer);
                /** check if FreezeFrame EventId is still the same */
                if (Dem_GetEventIdOfMemEntry(memoryIndex) != eventId)
                {
                    /** EventId has changed, so the FreezeFrame data also */
                    DestinationBuffer->WriteIndex = 0;
                }
                ret = E_OK;
            }
        }
    }
    return ret;
}
#endif

#endif

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
