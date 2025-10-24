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
 **  @file               : Dem_ExtendedData.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : Implementation file of Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_EXTENDEDDATA_H
#define DEM_EXTENDEDDATA_H

/* =================================================== inclusions =================================================== */
#include "Dem_Lcfg.h"
#if (DEM_EXTENDED_DATA_CLASS_NUMBER > 0u)
#include "Dem_EventInterface.h"
#include "Dem_DtcInterface.h"
#include "Dem_MemInterface.h"
#include "Dem_DataElement.h"

/* ===================================================== macros ===================================================== */
/**
 * @name  DEM_DCM_EXTENDEDDATARECORD_ENCODE
 * @{
 * @brief Macros encoding status of Dcm extend data record
 */
#define DEM_DCM_EXTENDEDDATARECORD_INVALID      (0x00u) /**< Invalid extended record */
#define DEM_DCM_EXTENDEDDATARECORD_FIRST        (0x01u) /**< The first valid extended record */
#define DEM_DCM_EXTENDEDDATARECORD_LAST_PRIMARY (0xEFu) /**< The last valid extended record for primary memory */
#define DEM_DCM_EXTENDEDDATARECORD_LAST_USER    (0xFEu) /**< The last valid extended record for user defined memory */
#define DEM_DCM_EXTENDEDDATARECORD_FIRST_OBD    (0x90u) /**< The first valid OBD extended record */
#define DEM_DCM_EXTENDEDDATARECORD_LAST_OBD     (0x9Fu) /**< The last valid extended record */
#define DEM_DCM_EXTENDEDDATARECORD_OBD          (0xFEu) /**< Select all OBD records */
#define DEM_DCM_EXTENDEDDATARECORD_ALL          (0xFFu) /**< Select all extended records */
/** @} */

#define DEM_DTC_ORIGIN_USER ((Dem_DTCOriginType)(0x0100u))

/* ========================================== internal function declarations ======================================== */
#if (DEM_GENERAL_ED_TRIGGER_ON_PASSED == STD_ON)
/**
 * @brief         Process extended data triggers on 'Passed' status report.
 * @param[in]     EventContext: The event context
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_ExtendedDataPassedUpdate(Dem_EventContextType* EventContext);
#endif

/**
 * @brief         Copy environmental data via ED
 * @param[in]     DataInfoPtr: Pointer to the data information buffer
 * @param[in]     CfgEDIndex: Config extend data index.
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_DataReportCopyED(Dem_DataInfoConstPtrType DataInfoPtr, Dem_EDRecordRefNumType CfgEDIndex);

/**
 * @brief         Copy a record into the given buffer
 * @param[in]     EventId: Event ID
 * @param[in]     ExtendedEntryIndex: Extend data entry index ID
 * @param[in]     CfgEDIndex: Config extend data index.
 * @param[in]     MemoryIndex: Memory index
 * @param[in]     DestinationBuffer: Pointer to destination buffer
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_DataReportCopyExtendedDataRecord(
    Dem_EventIdType              EventId,
    uint8                        ExtendedEntryIndex,
    Dem_EDRecordRefNumType       CfgEDIndex,
    Dem_NvBlockNumType           MemoryIndex,
    Dem_DestinationBufferPtrType DestinationBuffer);

#if (DEM_FEATURE_SWC_GET_FF_ED == STD_ON)
/**
 * @brief         Extract an extended data record for an event
 * @param[in]     EventId: Event ID
 * @param[in]     RecordNumber: This parameter is a unique identifier for a freeze frame record as defined in
 *                              ISO14229-1. 0xFF means most recent freeze frame record is returned. 0x00 is only
 *                              supported if the Dem module supports WWH-OBD (refer to DemOBDSupport)
 * @param[in]     DestinationBuffer: Pointer to destination buffer
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType
    Dem_EnvDataCopyED(Dem_EventIdType EventId, uint8 RecordNumber, Dem_DestinationBufferPtrType DestinationBuffer);

/**
 * @brief         Implementaion of SWC API Dem_GetEventExtendedDataRecordEx().
 * @param[in]     EventId: Event ID
 * @param[in]     RecordNumber: This parameter is a unique identifier for a freeze frame record as defined in
 *                              ISO14229-1. 0xFF means most recent freeze frame record is returned. 0x00 is only
 *                              supported if the Dem module supports WWH-OBD (refer to DemOBDSupport)
 * @param[in]     DestBuffer: Pointer to the destination buffer
 * @param[out]    BufSize: Buffer size
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_EnvDataSwcApiGetEventExtendedDataEx(
    Dem_EventIdType EventId,
    uint8           RecordNumber,
    uint8*          DestBuffer,
    uint16*         BufSize);
#endif

/* ========================================== internal function definitions ========================================= */
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"

#if (DEM_GENERAL_ED_TRIGGER_ON_PASSED == STD_ON)
/**
 * @brief Process extended data triggers on 'Passed' status report.
 */
DEM_LOCAL void Dem_ExtendedDataPassedUpdate(Dem_EventContextType* EventContext)
{
    Dem_EventIdType      eventId  = EventContext->EventId;
    Dem_DTCAttRefNumType lDTCAttr = Dem_GetDTCAttr(eventId);
    if (Dem_CheckDTCAttr(lDTCAttr) == TRUE)
    {
        Dem_MemStateInfoConstPtrType memoryInfo = Dem_MemStateInfoInit(Dem_GetMemRef(lDTCAttr));
        if (Dem_GetEventstored(eventId) == TRUE)
        {
            Dem_NvBlockNumType memoryIndex = Dem_MemoryFindIndex(memoryInfo, eventId);
            if ((DEM_FLAG_ISSET(EventContext->Eventtriggers, DEM_TRIGGER_TESTPASSED))
                && (memoryIndex != DEM_MEM_INVALID_MEMORY_INDEX))
            {
                uint8 blockModification = Dem_MemUpdateEventExtendData(eventId, memoryIndex, DEM_TRIGGER_TESTPASSED);
                if (blockModification == DEM_DATA_USERDATA_CHANGED)
                {
                    Dem_NotiEventDataChanged(eventId);
                }
#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
                if (blockModification != DEM_DATA_NO_CHANGES)
                {
                    Dem_NvmSetSingleBlockState(memoryIndex, DEM_NVM_BLOCKSTATE_DIRTY);
                }
#endif
            }
        }
    }
}
#endif

/**
 * @brief Copy environmental data via ED
 */
DEM_LOCAL boolean Dem_DataReportCopyED(Dem_DataInfoConstPtrType DataInfoPtr, Dem_EDRecordRefNumType CfgEDIndex)
{
    boolean ret = TRUE;
    /** Each ED consists of 1-* data elements. */
    for (Dem_DataElementClassNumType start = Dem_GetEDRRStartOfEDRC(CfgEDIndex);
         start < Dem_GetEDRREndOfEDRC(CfgEDIndex);
         ++start)
    {
        Dem_DataElementClassNumType dataIndex = Dem_GetRefValueOfDataElementClassRef(start);
        if (Dem_DataReportCopyStoredDataElement(DataInfoPtr, dataIndex) == FALSE)
        {
            ret = FALSE;
        }
    }
    return ret;
}

/**
 * @brief Copy a record into the given buffer
 */
DEM_LOCAL Std_ReturnType Dem_DataReportCopyExtendedDataRecord(
    Dem_EventIdType              EventId,
    uint8                        ExtendedEntryIndex,
    Dem_EDRecordRefNumType       CfgEDIndex,
    Dem_NvBlockNumType           MemoryIndex,
    Dem_DestinationBufferPtrType DestinationBuffer)
{
    Std_ReturnType   ret = E_OK;
    Dem_DataInfoType copyDidInfo;
    /** Statistic records always need to collect current data - if possible depending on event state */
    if (Dem_GetEDSizeOfEDRC(CfgEDIndex) > (DestinationBuffer->BufferSize - DestinationBuffer->WriteIndex))
    {
        DestinationBuffer->WriteIndex = 0u;
        ret                           = DEM_BUFFER_TOO_SMALL;
    }
    else
    {
        const uint8* sourceBuffer;
        if (Dem_GetHaveUserDataOfEDRC(CfgEDIndex) == TRUE)
        {
#if (DEM_MAX_SIZE_EXTENDED_DATA > 0u)
            sourceBuffer = Dem_GetExtendedDataPtrOfMemEntryByEntryIndex(MemoryIndex, CfgEDIndex, ExtendedEntryIndex);
#else
            DEM_PARAM_UNUSED(ExtendedEntryIndex);
#endif
        }
        else
        {
            sourceBuffer = (uint8*)NULL_PTR;
        }
        Dem_DataReportInitCopyDidInfo(&copyDidInfo, DestinationBuffer, sourceBuffer, EventId, MemoryIndex);
        if (Dem_DataReportCopyED(&copyDidInfo, CfgEDIndex) == FALSE)
        {
            ret                           = DEM_NO_SUCH_ELEMENT;
            DestinationBuffer->WriteIndex = 0u;
        }
    }
    return ret;
}

#if (DEM_FEATURE_SWC_GET_FF_ED == STD_ON)
/**
 * @brief Extract an extended data record for an event
 */
/* PRQA S 6030 ++ */ /* VL_MTR_Dem_STMIF */
DEM_LOCAL Std_ReturnType
    Dem_EnvDataCopyED(Dem_EventIdType EventId, uint8 RecordNumber, Dem_DestinationBufferPtrType DestinationBuffer)
{
    Std_ReturnType         ret;
    Dem_DTCAttRefNumType   lDTCAttr = Dem_GetDTCAttr(EventId);
    Dem_EDRefNumType       lEDRef   = Dem_GetEDRefOfDTCAttr(lDTCAttr);
    Dem_EDRecordRefNumType lEDRNStart;
    Dem_EDRecordRefNumType lEDRNEnd;
    Dem_EDRecordRefNumType cfgEDIndex = 0u;

    if (lEDRef != DEM_ATTRI_INVALID_EXTENDDATA)
    {
        lEDRNStart = Dem_GetEDRStartOfED(lEDRef);
        lEDRNEnd   = Dem_GetEDREndOfED(lEDRef);
    }
    else
    {
        lEDRNStart = 0u;
        lEDRNEnd   = 0u;
    }
    /** Find the requested record, and it's storage location */
    for (; lEDRNStart < lEDRNEnd; ++lEDRNStart)
    {
        cfgEDIndex = Dem_GetValueOfEDRecordRef(lEDRNStart);
        if (RecordNumber == Dem_GetEDRNumberOfEDRC(cfgEDIndex))
        {
            break;
        }
    }

    if (lEDRNStart < lEDRNEnd)
    {
        /** Found the record, retrieve it from its storage */
        /** A stored record needs to be retrieved from event memory */
        Dem_NvBlockNumType           memoryIndex;
        boolean                      modificationDetected;
        Dem_MemStateInfoConstPtrType memoryInfo = Dem_MemStateInfoInit(Dem_GetMemRef(lDTCAttr));
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
                    /** Supported record, test if it is currently stored */
                    if (Dem_CheckEDStored(memoryIndex, 0u) == TRUE)
                    {
                        /** The DTC has an event entry, but the requested record was not yet stored */
                        DestinationBuffer->WriteIndex = 0u;
                        ret                           = DEM_NO_SUCH_ELEMENT;
                    }
                    else
                    {
                        /** This call does not currently interrupt a concurrent data update */
                        ret = Dem_DataReportCopyExtendedDataRecord(
                            EventId,
                            0u,
                            cfgEDIndex,
                            memoryIndex,
                            DestinationBuffer);
                    }
                    if (updateState != Dem_MemoryUpdateGetState(memoryIndex))
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
                    DestinationBuffer->WriteIndex = 0u;
                    ret                           = E_NOT_OK;
                }
            }
            else
            {
                /** The requested data is currently not stored in the event memory */
                DestinationBuffer->WriteIndex = 0u;
                ret                           = DEM_NO_SUCH_ELEMENT;
            }
        } while (modificationDetected == TRUE);
    }
    else
    {
        DestinationBuffer->WriteIndex = 0u;
        ret                           = DEM_NO_SUCH_ELEMENT;
    }
    return ret;
}
/* PRQA S 6030 -- */
#endif

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

#if (DEM_FEATURE_SWC_GET_FF_ED == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief Implementaion of SWC API Dem_GetEventExtendedDataRecordEx().
 */
DEM_LOCAL Std_ReturnType Dem_EnvDataSwcApiGetEventExtendedDataEx(
    Dem_EventIdType EventId,
    uint8           RecordNumber,
    uint8*          DestBuffer,
    uint16*         BufSize)
{
    Std_ReturnType            ret = E_NOT_OK;
    Dem_DestinationBufferType destinationBuffer;

    /** Initialize a Destination Buffer */
    Dem_DestBufferInit(&destinationBuffer, DestBuffer, *BufSize);
    *BufSize = 0u;

#if (DEM_EXTENDED_DATA_CLASS_NUMBER > 0u)
    if ((Dem_GetAvailableInVariantByEvent(EventId) == TRUE) && (Dem_GetEventAvailableStatusByEventId(EventId) == TRUE))
    {
        if (Dem_GetDTCAttr(EventId) != DEM_DTC_ATTRIBUTES_NUMBER)
        {
            /** Find the Record number */
            ret = Dem_EnvDataCopyED(EventId, RecordNumber, &destinationBuffer);
        }
    }
    *BufSize = destinationBuffer.WriteIndex;
#endif
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

#endif
