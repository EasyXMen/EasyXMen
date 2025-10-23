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
 **  @file               : Dem_DataElement.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : Implementation file of Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_DATAELEMENT_H
#define DEM_DATAELEMENT_H

/* =================================================== inclusions =================================================== */
#include "Dem_Int.h"
#include "Dem_SatelliteInterface.h"
#include "Dem_OperationCycle.h"
#include "Dem_Error.h"

/* ===================================================== macros ===================================================== */
/**
 * @name DEM_DATAELEMENT_INVALID_BYTE
 * @{
 * @brief Data byte value substituted when callbacks fail to retrieve data
 */
#define DEM_DATAELEMENT_INVALID_PATTERN (0xFFu)       /**< Invalid pattern */
#define DEM_DATAELEMENT_INVALID_SIGNED  (-1)          /**< Invalid signed */
#define DEM_DATAELEMENT_INVALID_1Byte   (0xFFu)       /**< Invalid 1 Byte */
#define DEM_DATAELEMENT_INVALID_2Byte   (0xFFFFu)     /**< Invalid 2 Byte */
#define DEM_DATAELEMENT_INVALID_4Byte   (0xFFFFFFFFu) /**< Invalid 4 Byte */
/** @} */

/**
 * @name DEM_DATAELEMENT_TYPE
 * @{
 * @brief Dataelement type
 */
#define DEM_DATAELEMENT_INTERNAL_DATA (27u) /**< First internal data */
#define DEM_DATAELEMENT_EXTERNAL_DATA (60u) /**< Last internal data */
/** @} */

/**
 * @name DEM_DATAELEMENT_LENGTH
 * @{
 * @brief Data byte size
 */
#define DEM_DATAELEMENT_SIZE_1Byte (0x1u) /**< 1 byte size */
#define DEM_DATAELEMENT_SIZE_2Byte (0x2u) /**< 2 byte size */
#define DEM_DATAELEMENT_SIZE_4Byte (0x4u) /**< 4 byte size */
/** @} */

/**
 * @name DEM_DATAELEMENT_DATA
 * @{
 * @brief Data Right Shift
 */
#define DEM_DATAELEMENT_Shift_1 (0x1u) /**< Shift 1 bit */
#define DEM_DATAELEMENT_Shift_2 (0x2u) /**< Shift 2 bit */
/** @} */

/**
 * @name DEM_DATAELEMENT_DATA
 * @{
 * @brief Data byte offset
 */
#define DEM_DATAELEMENT_OFFSET_1Byte (0x1u) /**< 1 byte offset */
#define DEM_DATAELEMENT_OFFSET_2Byte (0x2u) /**< 2 byte offset */
#define DEM_DATAELEMENT_OFFSET_3Byte (0x3u) /**< 4 byte offset */
/** @} */

#define DEM_DATAELEMENT_COUNTER_MAX (0xFFu) /**< max counter value */
#define DEM_DATAELEMENT_FDC_MAX     (127)   /**< max FDC value */

/* ================================================ type definitions ================================================ */
/**
 * @brief Structure for Destination Buffer
 */
typedef struct Dem_DestinationBufferTag /* PRQA S 1536 */ /* VL_Dem_1536 */
{
    uint8* Buffer;     /**< Destination buffer receiving the data @range NA */
    uint16 BufferSize; /**< Buffer size of the destination buffer @range 0..65535 */
    uint16 WriteIndex; /**< Current write index in the destination buffer @range 0..65535 */
} Dem_DestinationBufferType;

/* PRQA S 5004 ++ */ /* VL_Dem_5004 */
/**
 * @brief pointer to Dem_DestinationBufferType
 */
typedef Dem_DestinationBufferType* Dem_DestinationBufferPtrType;

/**
 * @brief const pointer to Dem_DestinationBufferType
 */
typedef const Dem_DestinationBufferType* const Dem_DestinationBufferConstPtrType;

/**
 * @brief Structure for Data Info
 */
typedef struct Dem_DataInfoTag /* PRQA S 1536 */ /* VL_Dem_1536 */
{
    Dem_DestinationBufferPtrType DestinationBuffer; /**< destination buffer context @range NA */
    const uint8*                 SourceBuffer;      /**< source buffer providing the DID data @range NA */
    Dem_NvBlockNumType           MemoryIndex;       /**< index of memory entry holding the DTC data @range 0..65535 */
    Dem_EventIdType              EventId;           /**< EventId for which data is collected @range 0..65535 */
    uint16                       ReadIndex;         /**< Current read index from the source buffer @range 0..65535 */
} Dem_DataInfoType;

/**
 * @brief pointer to Dem_DataInfoType
 */
typedef Dem_DataInfoType* Dem_DataInfoPtrType;

/**
 * @brief const pointer to Dem_DataInfoType
 */
typedef const Dem_DataInfoType* const Dem_DataInfoConstPtrType;

/**
 * @brief Function pointer type for Inter Data Element Callback
 */
typedef boolean (*Dem_InterDataElementCallbackType)(Dem_DataInfoConstPtrType CopyDidInfoPtr);

/**
 * @brief         Function pointer type for Data Element Callback
 * @param[in]     DestBuffer: Pointer to the destination buffer
 * @param[in]     DataIndex: Data index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
typedef void (
    *Dem_DataElementCallbackType)(Dem_DestinationBufferPtrType DestBuffer, Dem_DataElementClassNumType DataIndex);
/* PRQA S 5004 -- */

/* ========================================== internal function declarations ======================================== */
#if (defined(DEM_DATA_CBK_WITH_EVENTID))
/**
 * @brief         Call the Data Element with Event ID parameter function
 * @param[in]     DestBuffer: Pointer to the destination buffer
 * @param[in]     DataIndex: Data index
 * @param[in]     EventId: Event ID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DataElementCallbackWithEventId(
    Dem_DestinationBufferPtrType DestBuffer,
    Dem_DataElementClassNumType  DataIndex,
    Dem_EventIdType              EventId);
#endif

#if (defined(DEM_DATA_CBK) || defined(DEM_DATA_PROT_UINT8) || defined(DEM_DATA_PROT_UINT8_N))
/**
 * @brief         Call the Data Element with data type uint8 uint8_N or application callback
 * @param[in]     DestBuffer: Pointer to the destination buffer
 * @param[in]     DataIndex: Data index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void
    Dem_DataElementCallbackUint8(Dem_DestinationBufferPtrType DestBuffer, Dem_DataElementClassNumType DataIndex);
#endif

#if (defined(DEM_DATA_PROT_BOOLEAN))
/**
 * @brief         Call the Data Element with data type boolean
 * @param[in]     DestBuffer: Pointer to the destination buffer
 * @param[in]     DataIndex: Data index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void
    Dem_DataElementCallbackBoolean(Dem_DestinationBufferPtrType DestBuffer, Dem_DataElementClassNumType DataIndex);
#endif

#if (defined(DEM_DATA_PROT_SINT8) || defined(DEM_DATA_PROT_SINT8_N))
/**
 * @brief         Call the Data Element with data type sint8 sint8_N
 * @param[in]     DestBuffer: Pointer to the destination buffer
 * @param[in]     DataIndex: Data index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void
    Dem_DataElementCallbackSint8(Dem_DestinationBufferPtrType DestBuffer, Dem_DataElementClassNumType DataIndex);
#endif

#if (defined(DEM_DATA_PROT_UINT16) || defined(DEM_DATA_PROT_UINT16_N))
/**
 * @brief         Call the Data Element with data type uint16 Motorola uint16_N Motorola
 * @param[in]     DestBuffer: Pointer to the destination buffer
 * @param[in]     DataIndex: Data index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void
    Dem_DataElementCallbackUint16(Dem_DestinationBufferPtrType DestBuffer, Dem_DataElementClassNumType DataIndex);
#endif

#if (defined(DEM_DATA_PROT_SINT16) || defined(DEM_DATA_PROT_SINT16_N))
/**
 * @brief         Call the Data Element with data type sint16 Motorola sint16_N Motorola
 * @param[in]     DestBuffer: Pointer to the destination buffer
 * @param[in]     DataIndex: Data index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void
    Dem_DataElementCallbackSint16(Dem_DestinationBufferPtrType DestBuffer, Dem_DataElementClassNumType DataIndex);
#endif

#if (defined(DEM_DATA_PROT_UINT16_N_INTEL) || defined(DEM_DATA_PROT_UINT16_INTEL))
/**
 * @brief         Call the Data Element with data type uint16 Intel uint16_N Intel
 * @param[in]     DestBuffer: Pointer to the destination buffer
 * @param[in]     DataIndex: Data index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void
    Dem_DataElementCallbackUint16Inter(Dem_DestinationBufferPtrType DestBuffer, Dem_DataElementClassNumType DataIndex);
#endif

#if (defined(DEM_DATA_PROT_SINT16_INTEL) || defined(DEM_DATA_PROT_SINT16_N_INTEL))
/**
 * @brief         Call the Data Element with data type sint16 Intel sint16_N Intel
 * @param[in]     DestBuffer: Pointer to the destination buffer
 * @param[in]     DataIndex: Data index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void
    Dem_DataElementCallbackSint16Inter(Dem_DestinationBufferPtrType DestBuffer, Dem_DataElementClassNumType DataIndex);
#endif

#if (defined(DEM_DATA_PROT_UINT32) || defined(DEM_DATA_PROT_UINT32_N))
/**
 * @brief         Call the Data Element with data type uint32 Motorola uint32_N Motorola
 * @param[in]     DestBuffer: Pointer to the destination buffer
 * @param[in]     DataIndex: Data index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void
    Dem_DataElementCallbackUint32(Dem_DestinationBufferPtrType DestBuffer, Dem_DataElementClassNumType DataIndex);
#endif

#if (defined(DEM_DATA_PROT_SINT32) || defined(DEM_DATA_PROT_SINT32_N))
/**
 * @brief         Call the Data Element with data type sint32 Motorola sint32_N Motorola
 * @param[in]     DestBuffer: Pointer to the destination buffer
 * @param[in]     DataIndex: Data index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void
    Dem_DataElementCallbackSint32(Dem_DestinationBufferPtrType DestBuffer, Dem_DataElementClassNumType DataIndex);
#endif

#if (defined(DEM_DATA_PROT_UINT32_INTEL) || defined(DEM_DATA_PROT_UINT32_N_INTEL))
/**
 * @brief         Call the Data Element with data type uint32 Intel uint32_N Intel
 * @param[in]     DestBuffer: Pointer to the destination buffer
 * @param[in]     DataIndex: Data index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void
    Dem_DataElementCallbackUint32Inter(Dem_DestinationBufferPtrType DestBuffer, Dem_DataElementClassNumType DataIndex);
#endif

#if (defined(DEM_DATA_PROT_SINT32_INTEL) || defined(DEM_DATA_PROT_SINT32_N_INTEL))
/**
 * @brief         Call the Data Element with data type sint32 Intel sint32_N Intel
 * @param[in]     DestBuffer: Pointer to the destination buffer
 * @param[in]     DataIndex: Data index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void
    Dem_DataElementCallbackSint32Inter(Dem_DestinationBufferPtrType DestBuffer, Dem_DataElementClassNumType DataIndex);
#endif

#if (defined(DEM_DATA_PROT_FLOAT) || defined(DEM_DATA_PROT_FLOAT_N))
/**
 * @brief         Call the Data Element with data type sint32 Motorola sint32_N Motorola
 * @param[in]     DestBuffer: Pointer to the destination buffer
 * @param[in]     DataIndex: Data index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void
    Dem_DataElementCallbackFloat(Dem_DestinationBufferPtrType DestBuffer, Dem_DataElementClassNumType DataIndex);
#endif

#if (defined(DEM_DATA_PROT_FLOAT_INTEL) || defined(DEM_DATA_PROT_FLOAT_N_INTEL))
/**
 * @brief         Call the Data Element with data type uint32 Intel uint32_N Intel
 * @param[in]     DestBuffer: Pointer to the destination buffer
 * @param[in]     DataIndex: Data index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void
    Dem_DataElementCallbackFloatInter(Dem_DestinationBufferPtrType DestBuffer, Dem_DataElementClassNumType DataIndex);
#endif

/**
 * @brief         Get environmental data through Data Element
 * @param[in]     DataInfoPtr: Pointer to the data information buffer
 * @param[in]     DataIndex: Data index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_GetEnvDataByDataElement(Dem_DataInfoConstPtrType DataInfoPtr, Dem_DataElementClassNumType DataIndex);

#if (defined(DEM_AGINGCTR_UPCNT_FIRST_ACTIVE))
/**
 * @brief         Up-counting Dem-internal aging counter
 * @param[in]     CopyDidInfoPtr: Pointer to copy DID data information buffer
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_DataCopyUpwardsFirstAgingCounter(Dem_DataInfoConstPtrType CopyDidInfoPtr);
#endif

#if (defined(DEM_AGINGCTR_UPCNT))
/**
 * @brief         Up-counting Dem-internal aging counter
 * @param[in]     CopyDidInfoPtr: Pointer to copy DID data information buffer
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_DataCopyUpwardsAgingCounter(Dem_DataInfoConstPtrType CopyDidInfoPtr);
#endif

#if (defined(DEM_AGINGCTR_DOWNCNT))
/**
 * @brief         Down-counting Dem-internal aging counter
 * @param[in]     CopyDidInfoPtr: Pointer to copy DID data information buffer
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_DataCopyDownwardsAgingCounter(Dem_DataInfoConstPtrType CopyDidInfoPtr);
#endif

#if ((defined(DEM_CYCLES_SINCE_FIRST_FAILED)) || (defined(DEM_OCC3)))
/**
 * @brief         Dem-internal Operation Cycle Counter - Cycles since first failed
 * @param[in]     CopyDidInfoPtr: Pointer to copy DID data information buffer
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_DataCopyCyclesSinceFirstFailed(Dem_DataInfoConstPtrType CopyDidInfoPtr);
#endif

#if ((defined(DEM_CYCLES_SINCE_LAST_FAILED)) || (defined(DEM_OCC1)))
/**
 * @brief         Dem-internal Operation Cycle Counter - Cycles since last failed
 * @param[in]     CopyDidInfoPtr: Pointer to copy DID data information buffer
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_DataCopyCyclesSinceLastFailed(Dem_DataInfoConstPtrType CopyDidInfoPtr);
#endif

#if (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE))
/**
 * @brief         Dem-internal DTC Fault Detection Counter maximum value during current operation cycle
 * @param[in]     CopyDidInfoPtr: Pointer to copy DID data information buffer
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_DataCopyMaxFdcThisCycle(Dem_DataInfoConstPtrType CopyDidInfoPtr);
#endif

#if (defined(DEM_J1939LAMP_STATUS))
/**
 * @brief         Current state of applicable lamp(s) for this DTC in SAE J1939 DM31 format.
 * @param[in]     CopyDidInfoPtr: Pointer to copy DID data information buffer
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_DataCopyJ1939LampStatus(Dem_DataInfoConstPtrType CopyDidInfoPtr);
#endif

#if (defined(DEM_MAX_FDC_SINCE_LAST_CLEAR))
/**
 * @brief         Dem-internal DTC Fault Detection Counter maximum value since last clear
 * @param[in]     CopyDidInfoPtr: Pointer to copy DID data information buffer
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_DataCopyMaxFdcSinceLastClear(Dem_DataInfoConstPtrType CopyDidInfoPtr);
#endif

/* ========================================== internal function definitions ========================================= */
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Dest Buffer info Init
 * @param[in]     Size: The buffer size
 * @param[in]     Buffer: Pointer to source buffer
 * @param[out]    DestBuffer: This parameter contains a byte pointer that points to the buffer, to which the
 *                            freeze frame data record shall be written to. The format is raw hexadecimal values
 *                            and contains no header-information.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_DestBufferInit(Dem_DestinationBufferType* DestBuffer, uint8* Buffer, uint16 Size)
{
    DestBuffer->Buffer     = Buffer;
    DestBuffer->BufferSize = Size;
    DestBuffer->WriteIndex = 0u;
}

/**
 * @brief         Data info Init
 * @param[in]     EventId: The event ID
 * @param[in]     DestinationBufferPtr: Pointer to destination buffer
 * @param[out]    DataInfoPtr: Pointer to the data information buffer
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_DataInfoInit(
    Dem_DataInfoPtrType                DataInfoPtr,
    const Dem_DestinationBufferPtrType DestinationBufferPtr,
    Dem_EventIdType                    EventId)
{
    DataInfoPtr->MemoryIndex       = DEM_NV_RAM_BLOCK_NUMBER;
    DataInfoPtr->SourceBuffer      = (const uint8*)NULL_PTR;
    DataInfoPtr->DestinationBuffer = DestinationBufferPtr;
    DataInfoPtr->EventId           = EventId;
    DataInfoPtr->ReadIndex         = 0u;
}

/**
 * @brief         Initializes the passed CopyDidInfo context.
 * @param[in]     EventId: The event ID
 * @param[in]     MemoryIndex: Memory entry handle
 * @param[in]     SourceBuffer: Pointer to source buffer
 * @param[in]     DestinationBuffer: Pointer to destination buffer
 * @param[out]    DataInfoPtr: Pointer to the data information buffer
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_DataReportInitCopyDidInfo(
    Dem_DataInfoPtrType        DataInfoPtr,
    Dem_DestinationBufferType* DestinationBuffer,
    const uint8*               SourceBuffer,
    Dem_EventIdType            EventId,
    Dem_NvBlockNumType         MemoryIndex)
{
    DataInfoPtr->DestinationBuffer = DestinationBuffer;
    DataInfoPtr->SourceBuffer      = SourceBuffer;
    DataInfoPtr->ReadIndex         = 0u;
    DataInfoPtr->MemoryIndex       = MemoryIndex;
    DataInfoPtr->EventId           = EventId;
}

/**
 * @brief         Get the Data Element Kind
 * @param[in]     DataIndex: Data index
 * @retval        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetDataElementKind(Dem_DataElementClassNumType DataIndex)
{
    return Dem_GetKindOfDataElement(DataIndex);
}

/**
 * @brief         Get the Data Element data size
 * @param[in]     DataIndex: Data index
 * @retval        uint8
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetDataElementSize(Dem_DataElementClassNumType DataIndex)
{
    return Dem_GetDataSizeOfDataElement(DataIndex);
}

/* PRQA S 5016 ++ */ /* VL_Dem_5016 */
/**
 * @brief         Get the Data Element data read function
 * @param[in]     DataIndex: Data index
 * @retval        Dem_ReadDataFPtrType
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_ReadDataFPtrType Dem_GetDataElementFnc(Dem_DataElementClassNumType DataIndex)
{
    return Dem_GetReadFncOfDataElement(DataIndex);
}
/* PRQA S 5016 -- */

/**
 * @brief         Check buffer size and increase write pointer to target buffer
 * @param[in]     Size: Data size
 * @param[out]    DestBuffer: Pointer to the destination buffer
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckAndIncreaseDestBufferWriteIndex(Dem_DestinationBufferPtrType DestBuffer, uint16 Size)
{
    boolean ret = FALSE;
    if (DestBuffer->WriteIndex <= (DestBuffer->BufferSize - Size))
    {
        DestBuffer->WriteIndex += Size;
        ret = TRUE;
    }
    return ret;
}

/**
 * @brief         Check if the Data Element is internal data
 * @param[in]     DataIndex: Data index
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckDataElementInternal(Dem_DataElementClassNumType DataIndex)
{
    return (Dem_GetDataElementKind(DataIndex) >= DEM_DATAELEMENT_INTERNAL_DATA); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/**
 * @brief         Copy passed uint8 data to destination buffer at passed write index
 * @param[in]     Data: Process data
 * @param[in]     WriteIndex: Write data index
 * @param[out]    DestBuffer: Pointer to the destination buffer
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void
    Dem_WriteDestinationBufferAtIndex(Dem_DestinationBufferConstPtrType DestBuffer, uint16 WriteIndex, uint8 Data)
{
    DestBuffer->Buffer[WriteIndex] = Data;
}

/**
 * @brief         Write 8-bit data to the target buffer
 * @param[in]     Data: Process data
 * @param[out]    DestBuffer: Pointer to the destination buffer
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_WriteUint8ToBuffer(Dem_DestinationBufferPtrType DestBuffer, uint8 Data)
{
    uint16 writeIndex = DestBuffer->WriteIndex;
    if (Dem_CheckAndIncreaseDestBufferWriteIndex(DestBuffer, DEM_DATAELEMENT_SIZE_1Byte) == TRUE)
    {
        DestBuffer->Buffer[writeIndex] = Data;
    }
}

/**
 * @brief         Write 16-bit data in Motorola format to the target buffer
 * @param[in]     Data: Process data
 * @param[out]    DestBuffer: Pointer to the destination buffer
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_WriteUint16ToBuffer(Dem_DestinationBufferPtrType DestBuffer, uint16 Data)
{
    uint16 writeIndex = DestBuffer->WriteIndex;
    if (Dem_CheckAndIncreaseDestBufferWriteIndex(DestBuffer, DEM_DATAELEMENT_SIZE_2Byte) == TRUE)
    {
        DestBuffer->Buffer[writeIndex]                                = Dem_GetHiByte(Data);
        DestBuffer->Buffer[writeIndex + DEM_DATAELEMENT_OFFSET_1Byte] = Dem_GetLoByte(Data);
    }
}

/**
 * @brief         Write 16-bit data in Intel format to the target buffer
 * @param[in]     Data: Process data
 * @param[out]    DestBuffer: Pointer to the destination buffer
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_WriteUint16ToBufferIntel(Dem_DestinationBufferPtrType DestBuffer, uint16 Data)
{
    uint16 writeIndex = DestBuffer->WriteIndex;
    if (Dem_CheckAndIncreaseDestBufferWriteIndex(DestBuffer, DEM_DATAELEMENT_SIZE_2Byte) == TRUE)
    {
        DestBuffer->Buffer[writeIndex]                                = Dem_GetLoByte(Data);
        DestBuffer->Buffer[writeIndex + DEM_DATAELEMENT_OFFSET_1Byte] = Dem_GetHiByte(Data);
    }
}

/**
 * @brief         Write 32-bit data in Motorola format to the target buffer
 * @param[in]     Data: Process data
 * @param[out]    DestBuffer: Pointer to the destination buffer
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_WriteUint32ToBuffer(Dem_DestinationBufferPtrType DestBuffer, uint32 Data)
{
    uint16 writeIndex = DestBuffer->WriteIndex;
    if (Dem_CheckAndIncreaseDestBufferWriteIndex(DestBuffer, DEM_DATAELEMENT_SIZE_4Byte) == TRUE)
    {
        DestBuffer->Buffer[writeIndex]                                = Dem_GetHiHiByte(Data);
        DestBuffer->Buffer[writeIndex + DEM_DATAELEMENT_OFFSET_1Byte] = Dem_GetHiLoByte(Data);
        DestBuffer->Buffer[writeIndex + DEM_DATAELEMENT_OFFSET_2Byte] = Dem_GetLoHiByte(Data);
        DestBuffer->Buffer[writeIndex + DEM_DATAELEMENT_OFFSET_3Byte] = Dem_GetLoLoByte(Data);
    }
}

/**
 * @brief         Write 32-bit data in Intel format to the target buffer
 * @param[in]     Data: Process data
 * @param[out]    DestBuffer: Pointer to the destination buffer
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_WriteUint32ToBufferIntel(Dem_DestinationBufferPtrType DestBuffer, uint32 Data)
{
    uint16 writeIndex = DestBuffer->WriteIndex;
    if (Dem_CheckAndIncreaseDestBufferWriteIndex(DestBuffer, DEM_DATAELEMENT_SIZE_4Byte) == TRUE)
    {
        DestBuffer->Buffer[writeIndex]                                = Dem_GetLoLoByte(Data);
        DestBuffer->Buffer[writeIndex + DEM_DATAELEMENT_OFFSET_1Byte] = Dem_GetLoHiByte(Data);
        DestBuffer->Buffer[writeIndex + DEM_DATAELEMENT_OFFSET_2Byte] = Dem_GetHiLoByte(Data);
        DestBuffer->Buffer[writeIndex + DEM_DATAELEMENT_OFFSET_3Byte] = Dem_GetHiHiByte(Data);
    }
}

#if (defined(DEM_DATA_CBK_WITH_EVENTID))
/**
 * @brief         Check if the Data Element with Event ID parameter
 * @param[in]     DataIndex: Data index
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckDataElementWithEventId(Dem_DataElementClassNumType DataIndex)
{
    return (Dem_GetDataElementKind(DataIndex) == DEM_DATA_CBK_WITH_EVENTID); /* PRQA S 4404 */ /* VL_Dem_4404 */
}

/**
 * @brief Call the Data Element with Event ID parameter function
 */
DEM_LOCAL void Dem_DataElementCallbackWithEventId(
    Dem_DestinationBufferPtrType DestBuffer,
    Dem_DataElementClassNumType  DataIndex,
    Dem_EventIdType              EventId)
{
    uint8  dataSize   = Dem_GetDataElementSize(DataIndex);
    uint16 writeIndex = DestBuffer->WriteIndex;
    if (Dem_CheckAndIncreaseDestBufferWriteIndex(DestBuffer, dataSize) == TRUE)
    {
        /** Callback interface non-null judgment by the tool to verify the completion of the code no longer deal with
         * the relevant content */
        uint8* bufferPtr = &DestBuffer->Buffer[writeIndex];
        /* PRQA S 0313 ++ */ /* VL_Dem_0313 */
        if (((*((Dem_ReadDataExtendFPtrType)Dem_GetDataElementFnc(DataIndex))))(EventId, bufferPtr) != E_OK)
        /* PRQA S 0313 -- */
        {
            /** In case the function failed, fill with padding bytes */
            Dem_MemSet(bufferPtr, DEM_DATAELEMENT_INVALID_PATTERN, dataSize);
            Dem_DetReportRuntimeError(DEM_SID_INTERNALAPIID, DEM_E_NODATAAVAILABLE);
        }
    }
}
#endif

#if (defined(DEM_DATA_CBK) || defined(DEM_DATA_PROT_UINT8) || defined(DEM_DATA_PROT_UINT8_N))
/**
 * @brief Call the Data Element with data type uint8 uint8_N or application callback
 */
DEM_LOCAL void
    Dem_DataElementCallbackUint8(Dem_DestinationBufferPtrType DestBuffer, Dem_DataElementClassNumType DataIndex)
{
    uint8  dataSize   = Dem_GetDataElementSize(DataIndex);
    uint16 writeIndex = DestBuffer->WriteIndex;
    if (Dem_CheckAndIncreaseDestBufferWriteIndex(DestBuffer, dataSize) == TRUE)
    {
        /** Callback interface non-null judgment by the tool to verify the completion of the code no longer deal with
         * the relevant content */
        uint8* bufferPtr = &DestBuffer->Buffer[writeIndex];
        /* PRQA S 0313 ++ */ /* VL_Dem_0313 */
        if (((*((Dem_ReadDataStandardFPtrType)Dem_GetDataElementFnc(DataIndex))))(bufferPtr) != E_OK)
        /* PRQA S 0313 -- */
        {
            /** In case the function failed, fill with padding bytes */
            Dem_MemSet(bufferPtr, DEM_DATAELEMENT_INVALID_PATTERN, dataSize);
            Dem_DetReportRuntimeError(DEM_SID_INTERNALAPIID, DEM_E_NODATAAVAILABLE);
        }
    }
}
#endif

#if (defined(DEM_DATA_PROT_BOOLEAN))
/**
 * @brief Call the Data Element with data type boolean
 */
DEM_LOCAL void
    Dem_DataElementCallbackBoolean(Dem_DestinationBufferPtrType DestBuffer, Dem_DataElementClassNumType DataIndex)
{
    uint8  dataSize   = Dem_GetDataElementSize(DataIndex);
    uint16 writeIndex = DestBuffer->WriteIndex;
    if (Dem_CheckAndIncreaseDestBufferWriteIndex(DestBuffer, dataSize) == TRUE)
    {
        /** Callback interface non-null judgment by the tool to verify the completion of the code no longer deal with
         * the relevant content */
        boolean applData;
        uint8*  bufferPtr = &DestBuffer->Buffer[writeIndex];
        /* PRQA S 0313 ++ */ /* VL_Dem_0313 */
        if (((*((Dem_ReadDataBooleanFPtrType)Dem_GetDataElementFnc(DataIndex))))(&applData) != E_OK)
        /* PRQA S 0313 -- */
        {
            /** In case the function failed, fill with padding bytes */
            bufferPtr[0] = DEM_DATAELEMENT_INVALID_1Byte;
            Dem_DetReportRuntimeError(DEM_SID_INTERNALAPIID, DEM_E_NODATAAVAILABLE);
        }
        else
        {
            bufferPtr[0] = (uint8)applData;
        }
    }
}
#endif

#if (defined(DEM_DATA_PROT_SINT8) || defined(DEM_DATA_PROT_SINT8_N))
/**
 * @brief Call the Data Element with data type sint8 sint8_N
 */
DEM_LOCAL void
    Dem_DataElementCallbackSint8(Dem_DestinationBufferPtrType DestBuffer, Dem_DataElementClassNumType DataIndex)
{
    uint8 arraySize = Dem_GetDataElementSize(DataIndex);
    /** Callback interface non-null judgment by the tool to verify the completion of the code no longer deal with the
     * relevant content */
    sint8 applData[DEM_DATA_SINT8_MAX_SIZE];
    /* PRQA S 0313 ++ */ /* VL_Dem_0313 */
    if (((*((Dem_ReadDataSint8FPtrType)Dem_GetDataElementFnc(DataIndex))))(&applData[0]) != E_OK)
    /* PRQA S 0313 -- */
    {
        /** In case the function failed, fill with padding bytes */
        for (uint8 iloop = 0u; iloop < arraySize; ++iloop)
        {
            applData[iloop] = DEM_DATAELEMENT_INVALID_SIGNED;
        }
        Dem_DetReportRuntimeError(DEM_SID_INTERNALAPIID, DEM_E_NODATAAVAILABLE);
    }
    for (uint8 iloop = 0u; iloop < arraySize; ++iloop)
    {
        Dem_WriteUint8ToBuffer(DestBuffer, (uint8)applData[iloop]);
    }
}
#endif

#if (defined(DEM_DATA_PROT_UINT16) || defined(DEM_DATA_PROT_UINT16_N))
/**
 * @brief Call the Data Element with data type uint16 Motorola uint16_N Motorola
 */
DEM_LOCAL void
    Dem_DataElementCallbackUint16(Dem_DestinationBufferPtrType DestBuffer, Dem_DataElementClassNumType DataIndex)
{
    uint8 arraySize = (Dem_GetDataElementSize(DataIndex) >> DEM_DATAELEMENT_Shift_1);
    /** Callback interface non-null judgment by the tool to verify the completion of the code no longer deal with the
     * relevant content */
    uint16 applData[DEM_DATA_UINT16_MAX_SIZE];
    /* PRQA S 0313 ++ */ /* VL_Dem_0313 */
    if (((*((Dem_ReadDataUint16FPtrType)Dem_GetDataElementFnc(DataIndex))))(&applData[0]) != E_OK)
    /* PRQA S 0313 -- */
    {
        /** In case the function failed, fill with padding bytes */
        for (uint8 iloop = 0; iloop < arraySize; ++iloop)
        {
            applData[iloop] = DEM_DATAELEMENT_INVALID_2Byte;
        }
        Dem_DetReportRuntimeError(DEM_SID_INTERNALAPIID, DEM_E_NODATAAVAILABLE);
    }
    for (uint8 iloop = 0; iloop < arraySize; ++iloop)
    {
        Dem_WriteUint16ToBuffer(DestBuffer, applData[iloop]);
    }
}
#endif

#if (defined(DEM_DATA_PROT_SINT16) || defined(DEM_DATA_PROT_SINT16_N))
/**
 * @brief Call the Data Element with data type sint16 Motorola sint16_N Motorola
 */
DEM_LOCAL void
    Dem_DataElementCallbackSint16(Dem_DestinationBufferPtrType DestBuffer, Dem_DataElementClassNumType DataIndex)
{
    uint8 arraySize = (Dem_GetDataElementSize(DataIndex) >> DEM_DATAELEMENT_Shift_1);
    /** Callback interface non-null judgment by the tool to verify the completion of the code no longer deal with the
     * relevant content */
    uint16 applData[DEM_DATA_SINT16_MAX_SIZE];
    /* PRQA S 0313 ++ */ /* VL_Dem_0313 */
    if (((*((Dem_ReadDataSint16FPtrType)Dem_GetDataElementFnc(DataIndex))))(&applData[0]) != E_OK)
    /* PRQA S 0313 -- */
    {
        /** In case the function failed, fill with padding bytes */
        for (uint8 iloop = 0; iloop < arraySize; ++iloop)
        {
            applData[iloop] = DEM_DATAELEMENT_INVALID_SIGNED;
        }
        Dem_DetReportRuntimeError(DEM_SID_INTERNALAPIID, DEM_E_NODATAAVAILABLE);
    }
    for (uint8 iloop = 0; iloop < arraySize; ++iloop)
    {
        Dem_WriteUint16ToBuffer(DestBuffer, (uint16)applData[iloop]);
    }
}
#endif

#if (defined(DEM_DATA_PROT_UINT16_N_INTEL) || defined(DEM_DATA_PROT_UINT16_INTEL))
/**
 * @brief Call the Data Element with data type uint16 Intel uint16_N Intel
 */
DEM_LOCAL void
    Dem_DataElementCallbackUint16Inter(Dem_DestinationBufferPtrType DestBuffer, Dem_DataElementClassNumType DataIndex)
{
    uint8 arraySize = (Dem_GetDataElementSize(DataIndex) >> DEM_DATAELEMENT_Shift_1);
    /** Callback interface non-null judgment by the tool to verify the completion of the code no longer deal with the
     * relevant content */
    uint16 applData[DEM_DATA_UINT16_MAX_SIZE];
    /* PRQA S 0313 ++ */ /* VL_Dem_0313 */
    if (((*((Dem_ReadDataUint16FPtrType)Dem_GetDataElementFnc(DataIndex))))(&applData[0]) != E_OK)
    /* PRQA S 0313 -- */
    {
        /** In case the function failed, fill with padding bytes */
        for (uint8 iloop = 0; iloop < arraySize; ++iloop)
        {
            applData[iloop] = DEM_DATAELEMENT_INVALID_2Byte;
        }
        Dem_DetReportRuntimeError(DEM_SID_INTERNALAPIID, DEM_E_NODATAAVAILABLE);
    }
    for (uint8 iloop = 0; iloop < arraySize; ++iloop)
    {
        Dem_WriteUint16ToBufferIntel(DestBuffer, applData[iloop]);
    }
}
#endif

#if (defined(DEM_DATA_PROT_SINT16_INTEL) || defined(DEM_DATA_PROT_SINT16_N_INTEL))
/**
 * @brief Call the Data Element with data type sint16 Intel sint16_N Intel
 */
DEM_LOCAL void
    Dem_DataElementCallbackSint16Inter(Dem_DestinationBufferPtrType DestBuffer, Dem_DataElementClassNumType DataIndex)
{
    uint8 arraySize = (Dem_GetDataElementSize(DataIndex) >> DEM_DATAELEMENT_Shift_1);
    /** Callback interface non-null judgment by the tool to verify the completion of the code no longer deal with the
     * relevant content */
    uint16 applData[DEM_DATA_SINT16_MAX_SIZE];
    /* PRQA S 0313 ++ */ /* VL_Dem_0313 */
    if (((*((Dem_ReadDataSint16FPtrType)Dem_GetDataElementFnc(DataIndex))))(&applData[0]) != E_OK)
    /* PRQA S 0313 -- */
    {
        /** In case the function failed, fill with padding bytes */
        for (uint8 iloop = 0; iloop < arraySize; ++iloop)
        {
            applData[iloop] = DEM_DATAELEMENT_INVALID_SIGNED;
        }
        Dem_DetReportRuntimeError(DEM_SID_INTERNALAPIID, DEM_E_NODATAAVAILABLE);
    }
    for (uint8 iloop = 0; iloop < arraySize; ++iloop)
    {
        Dem_WriteUint16ToBufferIntel(DestBuffer, (uint16)applData[iloop]);
    }
}
#endif

#if (defined(DEM_DATA_PROT_UINT32) || defined(DEM_DATA_PROT_UINT32_N))
/**
 * @brief Call the Data Element with data type uint32 Motorola uint32_N Motorola
 */
DEM_LOCAL void
    Dem_DataElementCallbackUint32(Dem_DestinationBufferPtrType DestBuffer, Dem_DataElementClassNumType DataIndex)
{
    uint8 arraySize = (Dem_GetDataElementSize(DataIndex) >> DEM_DATAELEMENT_Shift_2);
    /** Callback interface non-null judgment by the tool to verify the completion of the code no longer deal with the
     * relevant content */
    uint32 applData[DEM_DATA_UINT32_MAX_SIZE];
    /* PRQA S 0313 ++ */ /* VL_Dem_0313 */
    if (((*((Dem_ReadDataUint32FPtrType)Dem_GetDataElementFnc(DataIndex))))(&applData[0]) != E_OK)
    /* PRQA S 0313 -- */
    {
        /** In case the function failed, fill with padding bytes */
        for (uint8 iloop = 0; iloop < arraySize; ++iloop)
        {
            applData[iloop] = DEM_DATAELEMENT_INVALID_4Byte;
        }
        Dem_DetReportRuntimeError(DEM_SID_INTERNALAPIID, DEM_E_NODATAAVAILABLE);
    }
    for (uint8 iloop = 0; iloop < arraySize; ++iloop)
    {
        Dem_WriteUint32ToBuffer(DestBuffer, applData[iloop]);
    }
}
#endif

#if (defined(DEM_DATA_PROT_SINT32) || defined(DEM_DATA_PROT_SINT32_N))
/**
 * @brief Call the Data Element with data type sint32 Motorola sint32_N Motorola
 */
DEM_LOCAL void
    Dem_DataElementCallbackSint32(Dem_DestinationBufferPtrType DestBuffer, Dem_DataElementClassNumType DataIndex)
{
    uint8 arraySize = (Dem_GetDataElementSize(DataIndex) >> DEM_DATAELEMENT_Shift_2);
    /** Callback interface non-null judgment by the tool to verify the completion of the code no longer deal with the
     * relevant content */
    sint32 applData[DEM_DATA_SINT32_MAX_SIZE];
    /* PRQA S 0313 ++ */ /* VL_Dem_0313 */
    if (((*((Dem_ReadDataSint32FPtrType)Dem_GetDataElementFnc(DataIndex))))(&applData[0]) != E_OK)
    /* PRQA S 0313 -- */
    {
        /** In case the function failed, fill with padding bytes */
        for (uint8 iloop = 0; iloop < arraySize; ++iloop)
        {
            applData[iloop] = DEM_DATAELEMENT_INVALID_SIGNED;
        }
        Dem_DetReportRuntimeError(DEM_SID_INTERNALAPIID, DEM_E_NODATAAVAILABLE);
    }
    for (uint8 iloop = 0; iloop < arraySize; ++iloop)
    {
        Dem_WriteUint32ToBuffer(DestBuffer, (uint32)applData[iloop]);
    }
}
#endif

#if (defined(DEM_DATA_PROT_UINT32_INTEL) || defined(DEM_DATA_PROT_UINT32_N_INTEL))
/**
 * @brief Call the Data Element with data type uint32 Intel uint32_N Intel
 */
DEM_LOCAL void
    Dem_DataElementCallbackUint32Inter(Dem_DestinationBufferPtrType DestBuffer, Dem_DataElementClassNumType DataIndex)
{
    uint8 arraySize = (Dem_GetDataElementSize(DataIndex) >> DEM_DATAELEMENT_Shift_2);
    /** Callback interface non-null judgment by the tool to verify the completion of the code no longer deal with the
     * relevant content */
    uint32 applData[DEM_DATA_UINT32_MAX_SIZE];
    /* PRQA S 0313 ++ */ /* VL_Dem_0313 */
    if (((*((Dem_ReadDataUint32FPtrType)Dem_GetDataElementFnc(DataIndex))))(&applData[0]) != E_OK)
    /* PRQA S 0313 -- */
    {
        /** In case the function failed, fill with padding bytes */
        for (uint8 iloop = 0; iloop < arraySize; ++iloop)
        {
            applData[iloop] = DEM_DATAELEMENT_INVALID_4Byte;
        }
        Dem_DetReportRuntimeError(DEM_SID_INTERNALAPIID, DEM_E_NODATAAVAILABLE);
    }
    for (uint8 iloop = 0; iloop < arraySize; ++iloop)
    {
        Dem_WriteUint32ToBufferIntel(DestBuffer, applData[iloop]);
    }
}
#endif

#if (defined(DEM_DATA_PROT_SINT32_INTEL) || defined(DEM_DATA_PROT_SINT32_N_INTEL))
/**
 * @brief Call the Data Element with data type sint32 Intel sint32_N Intel
 */
DEM_LOCAL void
    Dem_DataElementCallbackSint32Inter(Dem_DestinationBufferPtrType DestBuffer, Dem_DataElementClassNumType DataIndex)
{
    uint8 arraySize = (Dem_GetDataElementSize(DataIndex) >> DEM_DATAELEMENT_Shift_2);
    /** Callback interface non-null judgment by the tool to verify the completion of the code no longer deal with the
     * relevant content */
    sint32 applData[DEM_DATA_SINT32_MAX_SIZE];
    /* PRQA S 0313 ++ */ /* VL_Dem_0313 */
    if (((*((Dem_ReadDataSint32FPtrType)Dem_GetDataElementFnc(DataIndex))))(&applData[0]) != E_OK)
    /* PRQA S 0313 -- */
    {
        /** In case the function failed, fill with padding bytes */
        for (uint8 iloop = 0; iloop < arraySize; ++iloop)
        {
            applData[iloop] = DEM_DATAELEMENT_INVALID_SIGNED;
        }
        Dem_DetReportRuntimeError(DEM_SID_INTERNALAPIID, DEM_E_NODATAAVAILABLE);
    }
    for (uint8 iloop = 0; iloop < arraySize; ++iloop)
    {
        Dem_WriteUint32ToBufferIntel(DestBuffer, (uint32)applData[iloop]);
    }
}
#endif

#if (defined(DEM_DATA_PROT_FLOAT) || defined(DEM_DATA_PROT_FLOAT_N))
/**
 * @brief         Call the Data Element with data type sint32 Motorola sint32_N Motorola
 */
DEM_LOCAL void
    Dem_DataElementCallbackFloat(Dem_DestinationBufferPtrType DestBuffer, Dem_DataElementClassNumType DataIndex)
{
    uint8 arraySize = (Dem_GetDataElementSize(DataIndex) >> DEM_DATAELEMENT_Shift_2);
    /** Callback interface non-null judgment by the tool to verify the completion of the code no longer deal with the
     * relevant content */
    float32 applData[DEM_DATA_FLOAT_MAX_SIZE];
    /* PRQA S 0313 ++ */ /* VL_Dem_0313 */
    if (((*((Dem_ReadDataFloat32FPtrType)Dem_GetDataElementFnc(DataIndex))))(&applData[0]) != E_OK)
    /* PRQA S 0313 -- */
    {
        /** In case the function failed, fill with padding bytes */
        for (uint8 iloop = 0; iloop < arraySize; ++iloop)
        {
            applData[iloop] = DEM_DATAELEMENT_INVALID_4Byte;
        }
        Dem_DetReportRuntimeError(DEM_SID_INTERNALAPIID, DEM_E_NODATAAVAILABLE);
    }
    for (uint8 iloop = 0; iloop < arraySize; ++iloop)
    {
        Dem_WriteUint32ToBuffer(DestBuffer, (uint32)applData[iloop]);
    }
}
#endif

#if (defined(DEM_DATA_PROT_FLOAT_INTEL) || defined(DEM_DATA_PROT_FLOAT_N_INTEL))
/**
 * @brief Call the Data Element with data type uint32 Intel uint32_N Intel
 */
DEM_LOCAL void
    Dem_DataElementCallbackFloatInter(Dem_DestinationBufferPtrType DestBuffer, Dem_DataElementClassNumType DataIndex)
{
    uint8 arraySize = (Dem_GetDataElementSize(DataIndex) >> DEM_DATAELEMENT_Shift_2);
    /** Callback interface non-null judgment by the tool to verify the completion of the code no longer deal with the
     * relevant content */
    float32 applData[DEM_DATA_FLOAT_MAX_SIZE];
    /* PRQA S 0313 ++ */ /* VL_Dem_0313 */
    if (((*((Dem_ReadDataFloat32FPtrType)Dem_GetDataElementFnc(DataIndex))))(&applData[0]) != E_OK)
    /* PRQA S 0313 -- */
    {
        /** In case the function failed, fill with padding bytes */
        for (uint8 iloop = 0; iloop < arraySize; ++iloop)
        {
            applData[iloop] = DEM_DATAELEMENT_INVALID_4Byte;
        }
        Dem_DetReportRuntimeError(DEM_SID_INTERNALAPIID, DEM_E_NODATAAVAILABLE);
    }
    for (uint8 iloop = 0; iloop < arraySize; ++iloop)
    {
        Dem_WriteUint32ToBufferIntel(DestBuffer, (uint32)applData[iloop]);
    }
}
#endif

/**
 * @brief Get environmental data through Data Element
 */
DEM_LOCAL void Dem_GetEnvDataByDataElement(Dem_DataInfoConstPtrType DataInfoPtr, Dem_DataElementClassNumType DataIndex)
{
#if (defined(DEM_DATA_CBK_WITH_EVENTID))
    if (Dem_CheckDataElementWithEventId(DataIndex) == TRUE)
    {
        Dem_DataElementCallbackWithEventId(DataInfoPtr->DestinationBuffer, DataIndex, DataInfoPtr->EventId);
    }
    else
#endif
    {
        if (Dem_CheckDataElementInternal(DataIndex) == FALSE)
        {
            const Dem_DataElementCallbackType Dem_DataElementCallback[DEM_DATAELEMENT_INTERNAL_DATA] = {
                NULL_PTR, /** DEM_DATA_CBK_WITH_EVENTID */
#if (defined(DEM_DATA_CBK))
                Dem_DataElementCallbackUint8, /** DEM_DATA_CBK */
#else
                NULL_PTR, /** DEM_DATA_CBK */
#endif
#if (defined(DEM_DATA_PROT_BOOLEAN))
                Dem_DataElementCallbackBoolean, /** DEM_DATA_PROT_BOOLEAN */
#else
                NULL_PTR, /** DEM_DATA_PROT_BOOLEAN */
#endif
#if (defined(DEM_DATA_PROT_UINT8))
                Dem_DataElementCallbackUint8, /** DEM_DATA_PROT_UINT8 */
#else
                NULL_PTR, /** DEM_DATA_PROT_UINT8 */
#endif
#if (defined(DEM_DATA_PROT_SINT8))
                Dem_DataElementCallbackSint8, /** DEM_DATA_PROT_SINT8 */
#else
                NULL_PTR, /** DEM_DATA_PROT_SINT8 */
#endif
#if (defined(DEM_DATA_PROT_UINT8_N))
                Dem_DataElementCallbackUint8, /** DEM_DATA_PROT_UINT8_N */
#else
                NULL_PTR, /** DEM_DATA_PROT_UINT8_N */
#endif
#if (defined(DEM_DATA_PROT_SINT8_N))
                Dem_DataElementCallbackSint8, /** DEM_DATA_PROT_SINT8_N */
#else
                NULL_PTR, /** DEM_DATA_PROT_SINT8_N */
#endif
#if (defined(DEM_DATA_PROT_UINT16))
                Dem_DataElementCallbackUint16, /** DEM_DATA_PROT_UINT16 */
#else
                NULL_PTR, /** DEM_DATA_PROT_UINT16 */
#endif
#if (defined(DEM_DATA_PROT_SINT16))
                Dem_DataElementCallbackSint16, /** DEM_DATA_PROT_SINT16 */
#else
                NULL_PTR, /** DEM_DATA_PROT_SINT16 */
#endif
#if (defined(DEM_DATA_PROT_UINT16_N))
                Dem_DataElementCallbackUint16, /** DEM_DATA_PROT_UINT16_N */
#else
                NULL_PTR, /** DEM_DATA_PROT_UINT16_N */
#endif
#if (defined(DEM_DATA_PROT_SINT16_N))
                Dem_DataElementCallbackSint16, /** DEM_DATA_PROT_SINT16_N */
#else
                NULL_PTR, /** DEM_DATA_PROT_SINT16_N */
#endif
#if (defined(DEM_DATA_PROT_UINT16_INTEL))
                Dem_DataElementCallbackUint16Inter, /** DEM_DATA_PROT_UINT16_INTEL */
#else
                NULL_PTR, /** DEM_DATA_PROT_UINT16_INTEL */
#endif
#if (defined(DEM_DATA_PROT_SINT16_INTEL))
                Dem_DataElementCallbackSint16Inter, /** DEM_DATA_PROT_SINT16_INTEL */
#else
                NULL_PTR, /** DEM_DATA_PROT_SINT16_INTEL */
#endif
#if (defined(DEM_DATA_PROT_UINT16_N_INTEL))
                Dem_DataElementCallbackUint16Inter, /** DEM_DATA_PROT_UINT16_N_INTEL */
#else
                NULL_PTR, /** DEM_DATA_PROT_UINT16_N_INTEL */
#endif
#if (defined(DEM_DATA_PROT_SINT16_N_INTEL))
                Dem_DataElementCallbackSint16Inter, /** DEM_DATA_PROT_SINT16_N_INTEL */
#else
                NULL_PTR, /** DEM_DATA_PROT_SINT16_N_INTEL */
#endif
#if (defined(DEM_DATA_PROT_UINT32))
                Dem_DataElementCallbackUint32, /** DEM_DATA_PROT_UINT32 */
#else
                NULL_PTR, /** DEM_DATA_PROT_UINT32 */
#endif
#if (defined(DEM_DATA_PROT_SINT32))
                Dem_DataElementCallbackSint32, /** DEM_DATA_PROT_SINT32 */
#else
                NULL_PTR, /** DEM_DATA_PROT_SINT32 */
#endif
#if (defined(DEM_DATA_PROT_UINT32_N))
                Dem_DataElementCallbackUint32, /** DEM_DATA_PROT_UINT32_N */
#else
                NULL_PTR, /** DEM_DATA_PROT_UINT32_N */
#endif
#if (defined(DEM_DATA_PROT_SINT32_N))
                Dem_DataElementCallbackSint32, /** DEM_DATA_PROT_SINT32_N */
#else
                NULL_PTR, /** DEM_DATA_PROT_SINT32_N */
#endif
#if (defined(DEM_DATA_PROT_UINT32_INTEL))
                Dem_DataElementCallbackUint32Inter, /** DEM_DATA_PROT_UINT32_INTEL */
#else
                NULL_PTR, /** DEM_DATA_PROT_UINT32_INTEL */
#endif
#if (defined(DEM_DATA_PROT_SINT32_INTEL))
                Dem_DataElementCallbackSint32Inter, /** DEM_DATA_PROT_SINT32_INTEL */
#else
                NULL_PTR, /** DEM_DATA_PROT_SINT32_INTEL */
#endif
#if (defined(DEM_DATA_PROT_UINT32_N_INTEL))
                Dem_DataElementCallbackUint32Inter, /** DEM_DATA_PROT_UINT32_N_INTEL */
#else
                NULL_PTR, /** DEM_DATA_PROT_UINT32_N_INTEL */
#endif
#if (defined(DEM_DATA_PROT_SINT32_N_INTEL))
                Dem_DataElementCallbackSint32Inter, /** DEM_DATA_PROT_SINT32_N_INTEL */
#else
                NULL_PTR, /** DEM_DATA_PROT_SINT32_N_INTEL */
#endif
#if (defined(DEM_DATA_PROT_FLOAT))
                Dem_DataElementCallbackFloat, /** DEM_DATA_PROT_FLOAT */
#else
                NULL_PTR, /** DEM_DATA_PROT_FLOAT */
#endif
#if (defined(DEM_DATA_PROT_FLOAT_N))
                Dem_DataElementCallbackFloat, /** DEM_DATA_PROT_FLOAT_N */
#else
                NULL_PTR, /** DEM_DATA_PROT_FLOAT_N */
#endif
#if (defined(DEM_DATA_PROT_FLOAT_INTEL))
                Dem_DataElementCallbackFloatInter, /** DEM_DATA_PROT_FLOAT_INTEL */
#else
                NULL_PTR, /** DEM_DATA_PROT_FLOAT_INTEL */
#endif
#if (defined(DEM_DATA_PROT_FLOAT_N_INTEL))
                Dem_DataElementCallbackFloatInter, /** DEM_DATA_PROT_FLOAT_N_INTEL */
#else
                NULL_PTR, /** DEM_DATA_PROT_FLOAT_N_INTEL */
#endif
            };
            const Dem_DataElementCallbackType fnc = Dem_DataElementCallback[Dem_GetDataElementKind(DataIndex)];
            if (fnc != NULL_PTR)
            {
                (*fnc)(DataInfoPtr->DestinationBuffer, DataIndex);
            }
        }
    }
}

#if (DEM_FEATURE_SWC_GET_FF_ED == STD_ON)
/**
 * @brief         Skip data elements
 * @param[in]     CopyDidInfo: Copy DID information
 * @param[in]     DIDRef: Reference DID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_DataReportSkipDid(Dem_DataInfoPtrType CopyDidInfo, Dem_DIDRefNumType DIDRef)
{
    /** Iterate all data elements in the DID */
    for (Dem_DataElementClassNumType start = Dem_GetRefStartOfDidClass(DIDRef); start < Dem_GetRefEndOfDidClass(DIDRef);
         ++start)
    {
        Dem_DataElementClassNumType dataIndex = Dem_GetRefValueOfDataElementClassRef(start);

        if (Dem_CheckDataElementInternal(dataIndex) == FALSE)
        {
            CopyDidInfo->ReadIndex += Dem_GetDataElementSize(dataIndex);
        }
    }
}
#endif

#if (DEM_MAX_SIZE_FREEZEFRAME > 0u)
/**
 * @brief         Obtaining environmental data via DID
 * @param[in]     DataInfoPtr: Pointer to data information
 * @param[in]     DIDRef: Reference DID
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_GetEnvDataByDid(Dem_DataInfoConstPtrType DataInfoPtr, Dem_DIDRefNumType DIDRef)
{
    /** Each DID consists of 1-* data elements. */
    for (Dem_DataElementClassNumType start = Dem_GetRefStartOfDidClass(DIDRef); start < Dem_GetRefEndOfDidClass(DIDRef);
         ++start)
    {
        Dem_DataElementClassNumType dataIndex = Dem_GetRefValueOfDataElementClassRef(start);
        Dem_GetEnvDataByDataElement(DataInfoPtr, dataIndex);
    }
}
#endif

#if (DEM_PID_CLASS_NUMBER > 0u)
/**
 * @brief         Obtaining environmental data via PID
 * @param[in]     DataInfoPtr: Pointer to data information
 * @param[in]     PIDIndex: PID index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_GetEnvDataByPid(Dem_DataInfoPtrType DataInfoPtr, uint8 PIDIndex)
{
    /** Each PID consists of 1-255 data elements. */
    for (Dem_PidDataNumType start = Dem_GetDataRefStartOfPID(PIDIndex); start < Dem_GetDataEndRefEndOfPID(PIDIndex);
         ++start)
    {
        Dem_DataElementClassNumType dataIndex = Dem_GetPidDataRefOfPID(start);
        Dem_GetEnvDataByDataElement(DataInfoPtr, dataIndex);
    }
}
#endif

#if ((DEM_MAX_SIZE_J1939FREEZEFRAME > 0u) || (DEM_MAX_SIZE_J1939EXFREEZEFRAME > 0u))

/**
 * @brief         Obtaining environmental data via SPN
 * @param[in]     DataInfoPtr: Pointer to data information
 * @param[in]     SPNRef: Reference SPN
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_GetEnvDataBySPN(Dem_DataInfoConstPtrType DataInfoPtr, Dem_SPNCRefNumType SPNRef)
{
    /** Each SPN consists of 1 data elements. */
    Dem_DataElementClassNumType dataIndex = Dem_GetDataElementRefOfSPNC(SPNRef);
    Dem_GetEnvDataByDataElement(DataInfoPtr, dataIndex);
}
#endif

#if (DEM_EXTENDED_DATA_CLASS_NUMBER > 0u)
/**
 * @brief         Obtaining environmental data via ED
 * @param[in]     DataInfoPtr: Pointer to data information
 * @param[in]     CfgEDIndex: Config extend data index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_GetEnvDataByEDR(Dem_DataInfoConstPtrType DataInfoPtr, Dem_EDRecordRefNumType CfgEDIndex)
{
    /** Each ED consists of 1-* data elements. */
    for (Dem_DataElementClassNumType start = Dem_GetEDRRStartOfEDRC(CfgEDIndex);
         start < Dem_GetEDRREndOfEDRC(CfgEDIndex);
         ++start)
    {
        Dem_DataElementClassNumType dataIndex = Dem_GetRefValueOfDataElementClassRef(start);
        Dem_GetEnvDataByDataElement(DataInfoPtr, dataIndex);
    }
}
#endif

/**
 * @brief         Data Write to Destination Buffer
 * @param[in]     SourceBuffer: Pointer to source buffer
 * @param[in]     DestinationBuffer: Pointer to destination buffer
 * @param[in]     Size: Process size
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_DataWriteDestinationBuffer(
    Dem_DestinationBufferPtrType DestinationBuffer,
    const uint8*                 SourceBuffer,
    uint16                       Size)
{
    uint16 writeIndex = DestinationBuffer->WriteIndex;
    if (Dem_CheckAndIncreaseDestBufferWriteIndex(DestinationBuffer, Size) == TRUE)
    {
        Dem_MemCpy(&DestinationBuffer->Buffer[writeIndex], SourceBuffer, Size);
    }
}

/**
 * @brief         Retrieves the user data stored in the event memory entry
 * @param[in]     CopyDidInfoPtr: Pointer to copy DID data information buffer
 * @param[in]     DataIndex: Data index
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean
    Dem_DataReportCopyStoredUserData(Dem_DataInfoPtrType CopyDidInfoPtr, Dem_DataElementClassNumType DataIndex)
{
    uint8 dataSize = Dem_GetDataSizeOfDataElement(DataIndex);
    Dem_DataWriteDestinationBuffer(
        CopyDidInfoPtr->DestinationBuffer,
        &CopyDidInfoPtr->SourceBuffer[CopyDidInfoPtr->ReadIndex],
        dataSize);
    CopyDidInfoPtr->ReadIndex += dataSize;
    return TRUE;
}

#if (defined(DEM_AGINGCTR_UPCNT_FIRST_ACTIVE))
/**
 * @brief Up-counting Dem-internal aging counter
 */
DEM_LOCAL boolean Dem_DataCopyUpwardsFirstAgingCounter(Dem_DataInfoConstPtrType CopyDidInfoPtr)
{
    uint8           agingCounter;
    uint16          targetCounter = Dem_GetAgingCounterOfMemEntry(CopyDidInfoPtr->MemoryIndex);
    Dem_EventIdType eventId       = CopyDidInfoPtr->EventId;

    if ((targetCounter == DEM_OPERATIONCYCLE_CYCLECOUNT_INVALID) || (Dem_GetEventAgingThreshold(eventId) == 0u))
    {
        agingCounter = 0x00u;
    }
    else
    {
        uint16 current = Dem_GetCycleCounter(Dem_GetEventAgingCycle(eventId));
        agingCounter = (uint8)(Dem_GetEventMaxAgingThreshold(eventId) - Dem_CycleCountDistance(current, targetCounter));
    }
    /** If the configuration parameter DemInternalDataElement is set to DEM_AGINGCTR_UPCNT_FIRST_ACTIVE, the value
     * shall be calculated analogous to DEM_AGINGCTR_UPCNT. The difference is, that in case the current aging
     * counter has a value of 0 and the event status bits TestFailedThisOperationCycle and
     * TestNotCompletedThisOperationCycle are set to False, a value of 1 is reported upon reading the Dem internal
     * aging counter.*/
    if ((agingCounter == 0x00u))
    {
        Dem_UdsStatusByteType eventStatus = Dem_GetEventUDSStatus(eventId);
        if (((Dem_UDSStatusCheckTFTOC(eventStatus)) == FALSE) && (Dem_UDSStatusCheckTNCTOC(eventStatus) == FALSE))
        {
            agingCounter = 0x01u;
        }
    }

    Dem_WriteUint8ToBuffer(CopyDidInfoPtr->DestinationBuffer, agingCounter);
    return TRUE;
}
#endif

#if (defined(DEM_AGINGCTR_UPCNT))
/**
 * @brief Up-counting Dem-internal aging counter
 */
DEM_LOCAL boolean Dem_DataCopyUpwardsAgingCounter(Dem_DataInfoConstPtrType CopyDidInfoPtr)
{
    uint8           agingCounter;
    uint16          targetCounter = Dem_GetAgingCounterOfMemEntry(CopyDidInfoPtr->MemoryIndex);
    Dem_EventIdType eventId       = CopyDidInfoPtr->EventId;

    if ((targetCounter == DEM_OPERATIONCYCLE_CYCLECOUNT_INVALID) || (Dem_GetEventAgingThreshold(eventId) == 0u))
    {
        agingCounter = 0x00u;
    }
    else
    {
        uint16 current = Dem_GetCycleCounter(Dem_GetEventAgingCycle(eventId));
        agingCounter = (uint8)(Dem_GetEventMaxAgingThreshold(eventId) - Dem_CycleCountDistance(current, targetCounter));
    }
    Dem_WriteUint8ToBuffer(CopyDidInfoPtr->DestinationBuffer, agingCounter);
    return TRUE;
}
#endif

#if (defined(DEM_AGINGCTR_DOWNCNT))
/**
 * @brief Down-counting Dem-internal aging counter
 */
DEM_LOCAL boolean Dem_DataCopyDownwardsAgingCounter(Dem_DataInfoConstPtrType CopyDidInfoPtr)
{
    uint8           agingCounter;
    uint16          targetCounter = Dem_GetAgingCounterOfMemEntry(CopyDidInfoPtr->MemoryIndex);
    Dem_EventIdType eventId       = CopyDidInfoPtr->EventId;

    if (Dem_GetEventAgingThreshold(eventId) == 0u)
    {
        agingCounter = DEM_DATAELEMENT_COUNTER_MAX;
    }
    else if (targetCounter == DEM_OPERATIONCYCLE_CYCLECOUNT_INVALID)
    {
        agingCounter = Dem_GetEventMaxAgingThreshold(CopyDidInfoPtr->EventId);
    }
    else
    {
        uint16 current = Dem_GetCycleCounter(Dem_GetEventAgingCycle(eventId));
        agingCounter   = (uint8)Dem_CycleCountDistance(current, targetCounter);
    }
    Dem_WriteUint8ToBuffer(CopyDidInfoPtr->DestinationBuffer, agingCounter);
    return TRUE;
}
#endif

#if (defined(DEM_CURRENT_FDC))
/**
 * @brief         Dem-internal fault detection counter
 * @param[in]     CopyDidInfoPtr: Pointer to copy DID data information buffer
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_DataCopyCurrentFdc(Dem_DataInfoConstPtrType CopyDidInfoPtr)
{
    boolean ret = FALSE;
    sint8   fdc;

    if (Dem_DTCGetFaultDetectionCounter(CopyDidInfoPtr->EventId, &fdc) == E_OK)
    {
        Dem_WriteUint8ToBuffer(CopyDidInfoPtr->DestinationBuffer, (uint8)fdc);
        ret = TRUE;
    }
    return ret;
}
#endif

#if ((defined(DEM_CYCLES_SINCE_FIRST_FAILED)) || (defined(DEM_OCC3)))
/**
 * @brief Dem-internal Operation Cycle Counter - Cycles since first failed
 */
DEM_LOCAL boolean Dem_DataCopyCyclesSinceFirstFailed(Dem_DataInfoConstPtrType CopyDidInfoPtr)
{
    uint8  cycleCounter;
    uint16 targetCounter = Dem_GetCyclesSinceFirstFailedOfMemEntry(CopyDidInfoPtr->MemoryIndex);

    if (targetCounter == DEM_OPERATIONCYCLE_CYCLECOUNT_INVALID)
    {
        cycleCounter = 0x00u;
    }
    else if (targetCounter == DEM_OPERATIONCYCLE_CYCLECOUNT_LATCHED)
    {
        cycleCounter = DEM_DATAELEMENT_COUNTER_MAX;
    }
    else
    {
        cycleCounter = (uint8)Dem_CycleCountDistance(
            targetCounter,
            Dem_GetCycleCounter(Dem_GetOCRefOfEvent(CopyDidInfoPtr->EventId)));
    }
    Dem_WriteUint8ToBuffer(CopyDidInfoPtr->DestinationBuffer, cycleCounter);
    return TRUE;
}
#endif

#if ((defined(DEM_CYCLES_SINCE_LAST_FAILED)) || (defined(DEM_OCC1)))
/**
 * @brief Dem-internal Operation Cycle Counter - Cycles since last failed
 */
DEM_LOCAL boolean Dem_DataCopyCyclesSinceLastFailed(Dem_DataInfoConstPtrType CopyDidInfoPtr)
{
    uint8  cycleCounter;
    uint16 targetCounter = Dem_GetCyclesSinceLastFailedOfMemEntry(CopyDidInfoPtr->MemoryIndex);

    if (targetCounter == DEM_OPERATIONCYCLE_CYCLECOUNT_INVALID)
    {
        cycleCounter = 0x00u;
    }
    else if (targetCounter == DEM_OPERATIONCYCLE_CYCLECOUNT_LATCHED)
    {
        cycleCounter = DEM_DATAELEMENT_COUNTER_MAX;
    }
    else
    {
        cycleCounter = (uint8)Dem_CycleCountDistance(
            targetCounter,
            Dem_GetCycleCounter(Dem_GetOCRefOfEvent(CopyDidInfoPtr->EventId)));
    }
    Dem_WriteUint8ToBuffer(CopyDidInfoPtr->DestinationBuffer, cycleCounter);
    return TRUE;
}
#endif

#if (defined(DEM_DTC_PRIORITY))
/**
 * @brief         DTC priority statically assigned to this DTC via DemDTCPriority
 * @param[in]     CopyDidInfoPtr: Pointer to copy DID data information buffer
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_DataCopyEventPriority(Dem_DataInfoConstPtrType CopyDidInfoPtr)
{
    Dem_WriteUint8ToBuffer(CopyDidInfoPtr->DestinationBuffer, Dem_GetEventPriority(CopyDidInfoPtr->EventId));
    return TRUE;
}
#endif

#if (defined(DEM_EVENT_ASSOCIATED_IDENTIFICATION))
/**
 * @brief Represents the static value associated to an event by DemEventAssociatedIdentification
 * @param[in]     CopyDidInfoPtr: Pointer to copy DID data information buffer
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_DataCopyEventAssociatedIdentification(Dem_DataInfoConstPtrType CopyDidInfoPtr)
{
    Dem_WriteUint16ToBuffer(CopyDidInfoPtr->DestinationBuffer, Dem_GetAssociatedIdByEvent(CopyDidInfoPtr->EventId));
    return TRUE;
}
#endif

#if (defined(DEM_FAILED_CYCLES))
/**
 * @brief         Dem-internal Operation Cycle Counter - Failed cycles
 * @param[in]     CopyDidInfoPtr: Pointer to copy DID data information buffer
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_DataCopyFailedCycles(Dem_DataInfoConstPtrType CopyDidInfoPtr)
{
    Dem_WriteUint8ToBuffer(
        CopyDidInfoPtr->DestinationBuffer,
        Dem_GetFailedCycleCounterOfMemEntry(CopyDidInfoPtr->MemoryIndex));
    return TRUE;
}
#endif

#if (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE))
/**
 * @brief Dem-internal DTC Fault Detection Counter maximum value during current operation cycle
 */
DEM_LOCAL boolean Dem_DataCopyMaxFdcThisCycle(Dem_DataInfoConstPtrType CopyDidInfoPtr)
{
    sint8           fdc;
    Dem_EventIdType eventId = CopyDidInfoPtr->EventId;
#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
    Dem_CombinationNumType groupId = Dem_GetGroupIdOfEvent(eventId);
    if (groupId != DEM_COMBINATION_NUMBER)
    {
        fdc = Dem_GetFDCMaxOfDTCCombined(groupId);
    }
    else
#endif
    {
        fdc = Dem_SatelliteGetMaxFaultDetectionCounter(
            Dem_GetSatelliteIdByEventId(eventId),
            Dem_GetMonitorIdByEventId(eventId));
    }
    Dem_WriteUint8ToBuffer(CopyDidInfoPtr->DestinationBuffer, (uint8)fdc);
    return TRUE;
}
#endif

#if (defined(DEM_J1939LAMP_STATUS))
/**
 * @brief Current state of applicable lamp(s) for this DTC in SAE J1939 DM31 format.
 */
DEM_LOCAL boolean Dem_DataCopyJ1939LampStatus(Dem_DataInfoConstPtrType CopyDidInfoPtr)
{
#if (DEM_INDICATOR_NUMBER > 0u)
    /*Get lamp status*/
    Dem_EventIdType        eventId     = CopyDidInfoPtr->EventId;
    Dem_EventClientRefType clientStart = Dem_GetClientStartRefOfEvent(eventId);
    /** always used first client , more client not defined */
    Dem_J1939DcmLampStatusType lampStatus =
        Dem_CalculateEventJ1939LampStatus(Dem_GetClientOfEventRef(clientStart), eventId);
    Dem_WriteUint8ToBuffer(CopyDidInfoPtr->DestinationBuffer, lampStatus.LampStatus);
    Dem_WriteUint8ToBuffer(CopyDidInfoPtr->DestinationBuffer, lampStatus.FlashLampStatus);
#else
    Dem_WriteUint16ToBuffer(CopyDidInfoPtr->DestinationBuffer, 0);
#endif
    return TRUE;
}
#endif

#if (defined(DEM_MAX_FDC_SINCE_LAST_CLEAR))
/**
 * @brief  Dem-internal DTC Fault Detection Counter maximum value since last clear
 */
DEM_LOCAL boolean Dem_DataCopyMaxFdcSinceLastClear(Dem_DataInfoConstPtrType CopyDidInfoPtr)
{
    sint8 fdc;
    if (Dem_UDSStatusCheckTFSLC(Dem_GetDTCUDSStatus(CopyDidInfoPtr->EventId)) == TRUE)
    {
        fdc = DEM_DATAELEMENT_FDC_MAX;
    }
    else
    {
        fdc = (sint8)Dem_GetMaxDebounceValueOfMemEntry(CopyDidInfoPtr->MemoryIndex);
    }
    Dem_WriteUint8ToBuffer(CopyDidInfoPtr->DestinationBuffer, (uint8)fdc);
    return TRUE;
}
#endif

#if (defined(DEM_MONITOR_DATA_0))
/**
 * @brief         The monitorData0 value provided along with Dem_SetEventStatusWithMonitorData
 * @param[in]     CopyDidInfoPtr: Pointer to copy DID data information buffer
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_DataCopyMonitorData0(Dem_DataInfoConstPtrType CopyDidInfoPtr)
{
    Dem_MonitorDataType monitorData0;
    Dem_EventIdType     eventId = CopyDidInfoPtr->EventId;
    monitorData0 =
        Dem_GetSatelliteMonitorData0(Dem_GetSatelliteIdByEventId(eventId), Dem_GetMonitorIdByEventId(eventId));
    Dem_WriteUint32ToBuffer(CopyDidInfoPtr->DestinationBuffer, monitorData0);
    return TRUE;
}
#endif

#if (defined(DEM_MONITOR_DATA_1))
/**
 * @brief         The monitorData1 value provided along with Dem_SetEventStatusWithMonitorData
 * @param[in]     CopyDidInfoPtr: Pointer to copy DID data information buffer
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_DataCopyMonitorData1(Dem_DataInfoConstPtrType CopyDidInfoPtr)
{
    Dem_MonitorDataType monitorData1;
    Dem_EventIdType     eventId = CopyDidInfoPtr->EventId;
    monitorData1 =
        Dem_GetSatelliteMonitorData1(Dem_GetSatelliteIdByEventId(eventId), Dem_GetMonitorIdByEventId(eventId));
    Dem_WriteUint32ToBuffer(CopyDidInfoPtr->DestinationBuffer, monitorData1);
    return TRUE;
}
#endif

#if (defined(DEM_OCCCTR))
/**
 * @brief         Dem-internal occurrence counter
 * @param[in]     CopyDidInfoPtr: Pointer to copy DID data information buffer
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_DataCopyOccurrenceCounter(Dem_DataInfoConstPtrType CopyDidInfoPtr)
{
    Dem_WriteUint8ToBuffer(CopyDidInfoPtr->DestinationBuffer, Dem_GetOCCOfMemEntry(CopyDidInfoPtr->MemoryIndex));
    return TRUE;
}
#endif

#if (defined(DEM_OVFLIND))
/**
 * @brief         Dem-internal overflow indication
 * @param[in]     CopyDidInfoPtr: Pointer to copy DID data information buffer
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_DataCopyOverflowIndication(Dem_DataInfoConstPtrType CopyDidInfoPtr)
{
    /* PRQA S 4304 ++ */ /* VL_Dem_4304 */
    Dem_WriteUint8ToBuffer(
        CopyDidInfoPtr->DestinationBuffer,
        (uint8)Dem_GetMemoryOverflow(Dem_GetMemRef(Dem_GetDTCAttr(CopyDidInfoPtr->EventId))));
    /* PRQA S 4304 -- */
    return TRUE;
}
#endif

#if (defined(DEM_SIGNIFICANCE))
/**
 * @brief         Dem-internal event significance (refer to DemDTCSignificance)
 * @param[in]     CopyDidInfoPtr: Pointer to copy DID data information buffer
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_DataCopyEventSignificance(Dem_DataInfoConstPtrType CopyDidInfoPtr)
{
    Dem_DTCAttRefNumType lDTCAttr = Dem_GetDTCAttr(CopyDidInfoPtr->EventId);
    uint8                data = ((Dem_GetSignificanceOfDTCAttr(lDTCAttr)) == DEM_EVENT_SIGNIFICANCE_FAULT) ? 1u : 0u;
    Dem_WriteUint8ToBuffer(CopyDidInfoPtr->DestinationBuffer, data);
    return TRUE;
}
#endif

#if (defined(DEM_OCC1))
/**
 * @brief         Dem-internal occurrence counter No.1
 * @param[in]     CopyDidInfoPtr: Pointer to copy DID data information buffer
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_DataCopyOCC1(Dem_DataInfoConstPtrType CopyDidInfoPtr)
{
    Dem_WriteUint8ToBuffer(
        CopyDidInfoPtr->DestinationBuffer,
        (uint8)Dem_GetCyclesSinceLastFailedOfMemEntry(CopyDidInfoPtr->MemoryIndex));
    return TRUE;
}
#endif

#if (defined(DEM_OCC2))
/**
 * @brief         Dem-internal occurrence counter No.2
 * @param[in]     CopyDidInfoPtr: Pointer to copy DID data information buffer
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_DataCopyOCC2(Dem_DataInfoConstPtrType CopyDidInfoPtr)
{
    Dem_WriteUint8ToBuffer(CopyDidInfoPtr->DestinationBuffer, Dem_GetOCC2OfMemEntry(CopyDidInfoPtr->MemoryIndex));
    return TRUE;
}
#endif

#if (defined(DEM_OCC4))
/**
 * @brief         Dem-internal occurrence counter No.4
 * @param[in]     CopyDidInfoPtr: Pointer to copy DID data information buffer
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_DataCopyOCC4(Dem_DataInfoConstPtrType CopyDidInfoPtr)
{
    Dem_WriteUint8ToBuffer(CopyDidInfoPtr->DestinationBuffer, Dem_GetOCC4OfMemEntry(CopyDidInfoPtr->MemoryIndex));
    return TRUE;
}
#endif

#if (defined(DEM_OCC5))
/**
 * @brief         Dem-internal occurrence counter No.5
 * @param[in]     CopyDidInfoPtr: Pointer to copy DID data information buffer
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_DataCopyOCC5(Dem_DataInfoConstPtrType CopyDidInfoPtr)
{
    Dem_WriteUint8ToBuffer(CopyDidInfoPtr->DestinationBuffer, Dem_GetOCC5OfMemEntry(CopyDidInfoPtr->MemoryIndex));
    return TRUE;
}
#endif

#if (defined(DEM_OCC6))
/**
 * @brief         Dem-internal occurrence counter No.6
 * @param[in]     CopyDidInfoPtr: Pointer to copy DID data information buffer
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_DataCopyOCC6(Dem_DataInfoConstPtrType CopyDidInfoPtr)
{
    Dem_WriteUint8ToBuffer(CopyDidInfoPtr->DestinationBuffer, Dem_GetOCC6OfMemEntry(CopyDidInfoPtr->MemoryIndex));
    return TRUE;
}
#endif

#if (defined(DEM_SI30))
/**
 * @brief         Dem-internal SI30
 * @param[in]     CopyDidInfoPtr: Pointer to copy DID data information buffer
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_DataCopySI30(Dem_DataInfoConstPtrType CopyDidInfoPtr)
{
    Dem_WriteUint8ToBuffer(CopyDidInfoPtr->DestinationBuffer, Dem_GetSI30FromMemEntry(CopyDidInfoPtr->MemoryIndex));
    return TRUE;
}
#endif

#if (defined(DEM_FAULT_PENDING_COUNTER))
/**
 * @brief         Dem-internal FaultPendingCounter
 * @param[in]     CopyDidInfoPtr: Pointer to copy DID data information buffer
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_DataCopyFaultPendingCounter(Dem_DataInfoConstPtrType CopyDidInfoPtr)
{
    Dem_UdsStatusByteType status = Dem_GetEventUDSStatus(CopyDidInfoPtr->EventId);
    Dem_WriteUint8ToBuffer(CopyDidInfoPtr->DestinationBuffer, Dem_UDSStatusCheckPDTC(status));
    return TRUE;
}
#endif

#if (defined(DEM_AGED_COUNTER))
/**
 * @brief         Dem-internal AgedCounter
 * @param[in]     CopyDidInfoPtr: Pointer to copy DID data information buffer
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_DataCopyAgedCounter(Dem_DataInfoConstPtrType CopyDidInfoPtr)
{
    Dem_WriteUint8ToBuffer(CopyDidInfoPtr->DestinationBuffer, Dem_GetAdminData().AgedCounter[CopyDidInfoPtr->EventId]);
    return TRUE;
}
#endif

/**
 * @brief         Copy stored data element into a buffer.
 * @param[in]     CopyDidInfoPtr: Pointer to copy DID data information buffer
 * @param[in]     DataIndex: Data index
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean
    Dem_DataReportCopyStoredDataElement(Dem_DataInfoConstPtrType CopyDidInfoPtr, Dem_DataElementClassNumType DataIndex)
{
    boolean ret;
    if (Dem_CheckDataElementInternal(DataIndex) == FALSE)
    {
        /* PRQA S 0311 ++ */ /* VL_Dem_0311 */
        ret = Dem_DataReportCopyStoredUserData((Dem_DataInfoPtrType)CopyDidInfoPtr, DataIndex);
        /* PRQA S 0311 -- */
    }
    else
    {
        const Dem_InterDataElementCallbackType
            Dem_InterDataElementCallback[DEM_DATAELEMENT_EXTERNAL_DATA - DEM_DATAELEMENT_INTERNAL_DATA + 1u] = {
#if (defined(DEM_AGINGCTR_DOWNCNT))
                Dem_DataCopyDownwardsAgingCounter,
#else
                NULL_PTR,
#endif
#if (defined(DEM_AGINGCTR_UPCNT))
                Dem_DataCopyUpwardsAgingCounter,
#else
                NULL_PTR,
#endif
#if (defined(DEM_AGINGCTR_UPCNT_FIRST_ACTIVE))
                Dem_DataCopyUpwardsFirstAgingCounter,
#else
                NULL_PTR,
#endif
#if (defined(DEM_CLR_DIST))
                NULL_PTR,
#else
                NULL_PTR,
#endif
#if (defined(DEM_CLR_TIME))
                NULL_PTR,
#else
                NULL_PTR,
#endif
#if (defined(DEM_CURRENT_FDC))
                Dem_DataCopyCurrentFdc,
#else
                NULL_PTR,
#endif
#if (defined(DEM_CYCLES_SINCE_FIRST_FAILED))
                Dem_DataCopyCyclesSinceFirstFailed,
#else
                NULL_PTR,
#endif
#if (defined(DEM_CYCLES_SINCE_LAST_FAILED))
                Dem_DataCopyCyclesSinceLastFailed,
#else
                NULL_PTR,
#endif
#if (defined(DEM_DTC_PRIORITY))
                Dem_DataCopyEventPriority,
#else
                NULL_PTR,
#endif
#if (defined(DEM_DTR))
                NULL_PTR,
#else
                NULL_PTR,
#endif
#if (defined(DEM_EVENT_ASSOCIATED_IDENTIFICATION))
                Dem_DataCopyEventAssociatedIdentification,
#else
                NULL_PTR,
#endif
#if (defined(DEM_FAILED_CYCLES))
                Dem_DataCopyFailedCycles,
#else
                NULL_PTR,
#endif
#if (defined(DEM_IUMPR))
                NULL_PTR,
#else
                NULL_PTR,
#endif
#if (defined(DEM_J1939LAMP_STATUS))
                Dem_DataCopyJ1939LampStatus,
#else
                NULL_PTR,
#endif
#if (defined(DEM_MAX_FDC_DURING_CURRENT_CYCLE))
                Dem_DataCopyMaxFdcThisCycle,
#else
                NULL_PTR,
#endif
#if (defined(DEM_MAX_FDC_SINCE_LAST_CLEAR))
                Dem_DataCopyMaxFdcSinceLastClear,
#else
                NULL_PTR,
#endif
#if (defined(DEM_MIL_DIST))
                NULL_PTR,
#else
                NULL_PTR,
#endif
#if (defined(DEM_MIL_TIME))
                NULL_PTR,
#else
                NULL_PTR,
#endif
#if (defined(DEM_MONITOR_ACTIVITY_DATA))
                NULL_PTR,
#else
                NULL_PTR,
#endif
#if (defined(DEM_MONITOR_DATA_0))
                Dem_DataCopyMonitorData0,
#else
                NULL_PTR,
#endif
#if (defined(DEM_MONITOR_DATA_1))
                Dem_DataCopyMonitorData1,
#else
                NULL_PTR,
#endif
#if (defined(DEM_OCCCTR))
                Dem_DataCopyOccurrenceCounter,
#else
                NULL_PTR,
#endif
#if (defined(DEM_OVFLIND))
                Dem_DataCopyOverflowIndication,
#else
                NULL_PTR,
#endif
#if (defined(DEM_SIGNIFICANCE))
                Dem_DataCopyEventSignificance,
#else
                NULL_PTR,
#endif
#if (defined(DEM_WARM_UPS))
                NULL_PTR,
#else
                NULL_PTR,
#endif
#if (defined(DEM_OCC1))
                Dem_DataCopyCyclesSinceLastFailed,
#else
                NULL_PTR,
#endif
#if (defined(DEM_OCC2))
                Dem_DataCopyOCC2,
#else
                NULL_PTR,
#endif
#if (defined(DEM_OCC3))
                Dem_DataCopyCyclesSinceFirstFailed,
#else
                NULL_PTR,
#endif
#if (defined(DEM_OCC4))
                Dem_DataCopyOCC4,
#else
                NULL_PTR,
#endif
#if (defined(DEM_OCC5))
                Dem_DataCopyOCC5,
#else
                NULL_PTR,
#endif
#if (defined(DEM_OCC6))
                Dem_DataCopyOCC6,
#else
                NULL_PTR,
#endif
#if (defined(DEM_SI30))
                Dem_DataCopySI30,
#else
                NULL_PTR,
#endif
#if (defined(DEM_FAULT_PENDING_COUNTER))
                Dem_DataCopyFaultPendingCounter,
#else
                NULL_PTR,
#endif
#if (defined(DEM_AGED_COUNTER))
                Dem_DataCopyAgedCounter,
#else
                NULL_PTR,
#endif
            };
        const Dem_InterDataElementCallbackType fnc =
            Dem_InterDataElementCallback[Dem_GetDataElementKind(DataIndex) - DEM_DATAELEMENT_INTERNAL_DATA];
        if (fnc != NULL_PTR)
        {
            ret = (*fnc)(CopyDidInfoPtr);
        }
    }
    return ret;
}

#if (DEM_MAX_SIZE_FREEZEFRAME > 0u)
/**
 * @brief         Copies DID data into a buffer.
 * @param[in]     DataInfoPtr: Pointer to the data information buffer
 * @param[in]     DIDRef: Reference DID
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_DataReportCopyDid(Dem_DataInfoConstPtrType DataInfoPtr, Dem_DIDRefNumType DIDRef)
{
    boolean ret = TRUE;
    /** Each DID consists of 1-* data elements. */
    for (Dem_DataElementClassNumType start = Dem_GetRefStartOfDidClass(DIDRef); start < Dem_GetRefEndOfDidClass(DIDRef);
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
#endif

#if (DEM_PID_CLASS_NUMBER > 0u)
/**
 * @brief         Copies PID data into a buffer.
 * @param[in]     DataInfoPtr: Pointer to the data information buffer
 * @param[in]     PidIndex: PID index
 * @return        boolean
 * @retval        TRUE: Valid
 * @retval        FALSE: Invalid
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_DataReportCopyPid(Dem_DataInfoPtrType DataInfoPtr, uint8 PidIndex)
{
    boolean ret = TRUE;
    /** Each PID consists of 1-255 data elements. */
    for (Dem_PidDataNumType start = Dem_GetDataRefStartOfPID(PidIndex); start < Dem_GetDataEndRefEndOfPID(PidIndex);
         ++start)
    {
        Dem_DataElementClassNumType dataIndex = Dem_GetPidDataRefOfPID(start);
        if (Dem_DataReportCopyStoredDataElement(DataInfoPtr, dataIndex) == FALSE)
        {
            ret = FALSE;
        }
    }
    return ret;
}
#endif

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
