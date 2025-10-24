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
 **  @file               : Dcm_UDS.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm UDS public function implementation
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Dcm_Internal.h"
/* ===================================================== macros ===================================================== */
/**
 * @brief max memory address and size byte number
 */
#define DCM_MAX_MEMORY_ADD_SIZE_BYTENUM 4u
/**
 * @brief the did mask for obd did
 */
#define DCM_OBD_DID_MASK 0x00FFu
/* ========================================= internal function declarations ========================================= */
#if ((STD_ON == DCM_UDS_0X23) || (STD_ON == DCM_UDS_0X34) || (STD_ON == DCM_UDS_0X2C) || (STD_ON == DCM_UDS_0X3D))
/**
 * @brief         Verify memory request condition such as addAndLengthformatId and request length
 * @param[in]     MemoryCfg     : the target memory configuration ptr
 * @param[in]     AddAndLenFid  : the requested address and length format id
 * @param[in]     byteNumber    : the requested byte number
 * @param[out]    addByteNumber : the number of byte of address
 * @param[out]    LenByteNumber : the number of byte of memory length
 * @param[out]    ErrorCode     : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm
 *                                module shall send a negative response with NRC code equal to the parameter
 *                                ErrorCode parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was unsuccessful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS_CheckMemoryCondition(
    const Dcm_DspMemoryType*      MemoryCfg,
    uint8                         AddAndLenFid,
    Dcm_MsgLenType                byteNumber,
    uint8*                        addByteNumber,
    uint8*                        LenByteNumber,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         Verify requested memory is within the configuration
 * @param[in]     MemoryCfg        : the target memory configuration ptr
 * @param[in]     memoryAccess     : read/write memory access
 * @param[in]     MemoryAddress    : the requested address
 * @param[in]     MemorySize       : the requested size
 * @param[out]    MemoryIdentifier : configured memory Id
 * @param[out]    targetMemoryInfo : the configured memory range info
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was unsuccessful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS_CheckMemoryRangeCheck(
    const Dcm_DspMemoryType*           MemoryCfg,
    uint8                              memoryAccess,
    uint32                             MemoryAddress,
    uint32                             MemorySize,
    uint8*                             MemoryIdentifier,
    const Dcm_DspMemoryRangeInfoType** targetMemoryInfo);

/**
 * @brief         find requested memory range info within the configuration
 * @param[in]     MemoryAddress    : the requested address
 * @param[in]     MemorySize       : the requested size
 * @param[in]     memoryInfo       : memoryInfo to search from
 * @param[in]     infoNum          : the number of memoryInfo
 * @param[out]    targetMemoryInfo : the configured memory range info
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was unsuccessful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS_FindMemoryRange(
    uint32                             MemoryAddress,
    uint32                             MemorySize,
    const Dcm_DspMemoryRangeInfoType*  memoryInfo,
    uint16                             infoNum,
    const Dcm_DspMemoryRangeInfoType** targetMemoryInfo);

/**
 * @brief         check target memory range session/security/modeRule
 * @param[in]     memoryRangeInfo : the requested memoryRangeInfo
 * @param[out]    ErrorCode       : the nrc to send
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was unsuccessful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType
    Dcm_UDS_CheckMemoryAuth(const Dcm_DspMemoryRangeInfoType* memoryRangeInfo, Dcm_NegativeResponseCodeType* ErrorCode);
#endif
#if ((STD_ON == DCM_UDS_0X22) || (STD_ON == DCM_UDS_0X2A) || (STD_ON == DCM_UDS_0X2F))
#if ((STD_ON == DCM_USE_NVM) && (STD_ON == DCM_USE_BLOCK_ID_ENABLED))
/**
 * @brief         Calls NvM APIs to read data from nvm block
 * @param[in]     OpStatus      : Indicates the current operation status
 * @param[in]     dspData       : Data configuration pointer
 * @param[out]    DestBuffer    : Target storage buffer
 *                                ErrorCode parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was unsuccessful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType
    Dcm_UDS_SignalHandleNvM(Dcm_ExtendedOpStatusType OpStatus, const Dcm_DspDataCfgType* dspData, uint8* DestBuffer);
#endif

/**
 * @brief         read data from signal using callouts(CS/SR/Callout)
 * @param[in]     OpStatus      : Indicates the current operation status
 * @param[in]     dspData       : Data configuration pointer
 * @param[out]    DestBuffer    : Buffer for storing read data
 * @param[out]    DataLength    : Pointer to store data length
 * @param[out]    ErrorCode     : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm
 *                                module shall send a negative response with NRC code equal to the parameter
 *                                ErrorCode parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was unsuccessful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS_SignalHandleDefault(
    Dcm_ExtendedOpStatusType      OpStatus,
    const Dcm_DspDataCfgType*     dspData,
    uint8*                        DestBuffer,
    uint16*                       DataLength,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#if (STD_ON == DCM_DYN_DID)
/**
 * @brief         prepare for reading dynamic did
 * @param[in]     OpStatus     : Indicates the current operation status
 * @param[in]     ConnectionId : the current connection
 * @param[in]     DDDIdIndex   : the index of the dynamic defined did
 * @param[in]     index        : the index of the dynamic defined source did
 * @param[out]    offset     : the input and resulting offset
 * @param[out]    DestBuffer   : the did's output data
 * @param[inout]  BufSize      : When the function is called this parameter contains the maximum number of data bytes
 *                               that can be written to the buffer. The function returns the actual number of written
 *                               data bytes in this parameter.
 * @param[out]    checkFail    : whether the source did check fails
 * @param[out]    ErrorCode    : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                               shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                               value.
 * @return        Std_ReturnType
 * @retval        E_OK              : Request was successful
 * @retval        E_NOT_OK          : Request was unsuccessful
 * @retval        DCM_E_PENDING     : Request is not yet finished
 * @retval        DCM_E_FORCE_RCRRP : Application request the transmission of a response Response Pending (NRC 0x78)
 * @reentrant     TRUE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS_DynDidDefinedDID(
    Dcm_ExtendedOpStatusType OpStatus,
    uint16                   ConnectionId,
    uint16                   DDDIdIndex,
    uint8                    index,
    Dcm_MsgLenType*          offset,
    uint8*                   DestBuffer,
    Dcm_MsgLenType*          BufSize,
#if (STD_ON == DCM_DDDID_CHECK_SOURCE)
    boolean* checkFail,
#endif
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         read dynamic did that is defined by source did
 * @param[in]     OpStatus   : Indicates the current operation status
 * @param[in]     DDDIdIndex : the index of the dynamic defined did
 * @param[in]     index      : the index of the dynamic defined source did
 * @param[out]    DestBuffer : the did's output data
 * @param[out]    offset     : the input and resulting offset
 * @param[inout]  BufSize    : When the function is called this parameter contains the maximum number of data bytes
 *                             that can be written to the buffer. The function returns the actual number of written
 *                             data bytes in this parameter.
 * @param[out]    ErrorCode  : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                             shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                             value.
 * @return        Std_ReturnType
 * @retval        E_OK              : Request was successful
 * @retval        E_NOT_OK          : Request was unsuccessful
 * @retval        DCM_E_PENDING     : Request is not yet finished
 * @retval        DCM_E_FORCE_RCRRP : Application request the transmission of a response Response Pending (NRC 0x78)
 * @reentrant     TRUE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS_DynDidDefinedDIDHandle(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint16                        DDDIdIndex,
    uint8                         index,
    Dcm_MsgLenType*               offset,
    uint8*                        DestBuffer,
    Dcm_MsgLenType*               BufSize,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         read dynamic did that is defined by memory address and memory size
 * @param[in]     OpStatus   : Indicates the current operation status
 * @param[in]     DDDIdIndex : the index of the dynamic defined did
 * @param[in]     index      : the index of the dynamic defined source did
 * @param[in]     offset     : Pointer to a variable holding the offset value, used to specify the starting position
 * within the memory area related to the DID.
 * @param[out]    DestBuffer : The output data of the DID.
 * @param[out]    DestBuffer : the did's output data
 * @param[inout]  BufSize    : When the function is called this parameter contains the maximum number of data bytes
 *                             that can be written to the buffer. The function returns the actual number of written
 *                             data bytes in this parameter.
 * @param[out]    ErrorCode  : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                             shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                             value.
 * @return        Std_ReturnType
 * @retval        E_OK              : Request was successful
 * @retval        E_NOT_OK          : Request was unsuccessful
 * @retval        DCM_E_PENDING     : Request is not yet finished
 * @retval        DCM_E_FORCE_RCRRP : Application request the transmission of a response Response Pending (NRC 0x78)
 * @reentrant     TRUE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS_DynDidDefinedAdd(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint16                        DDDIdIndex,
    uint8                         index,
    Dcm_MsgLenType*               offset,
    uint8*                        DestBuffer,
    Dcm_MsgLenType*               BufSize,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif
/* ========================================== external function definitions ========================================= */
#if ((STD_ON == DCM_UDS_0X23) || (STD_ON == DCM_UDS_0X34) || (STD_ON == DCM_UDS_0X2C) || (STD_ON == DCM_UDS_0X3D))
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * check whether addressAndLengthFormatId is supported, memory address + size fits in configured memory range, and
 * requested session/security/modeRule is satisfied
 */
/* PRQA S 3120 ++ */      /* VL_QAC_MagicNum */
/* PRQA S 3673,6040 ++ */ /* VL_QAC_3673,VL_MTR_Dcm_STPAR */
Std_ReturnType Dcm_UDS_CheckMemory(
    const Dcm_DspMemoryType*      MemoryCfg,
    uint8                         AddAndLenFid,
    Dcm_MsgLenType                byteNumber,
    uint8*                        reqData,
    uint8*                        MemoryIdentifier,
    uint32*                       MemoryAddress,
    uint32*                       MemorySize,
    uint8                         memoryAccess,
    Dcm_NegativeResponseCodeType* ErrorCode)
/* PRQA S 3673,6040 -- */
{
    uint8 addByteNumber = 0u;
    uint8 LenByteNumber = 0u;

    /* check input addAndLenFid, request length */
    Std_ReturnType result =
        Dcm_UDS_CheckMemoryCondition(MemoryCfg, AddAndLenFid, byteNumber, &addByteNumber, &LenByteNumber, ErrorCode);

    /* get requesed memory address and memory size, check the range and get the targeted memoryRangeInfo */
    const Dcm_DspMemoryRangeInfoType* memoryRangeInfo = NULL_PTR;
    if (E_OK == result)
    {
        if ((addByteNumber <= DCM_MAX_MEMORY_ADD_SIZE_BYTENUM) && (LenByteNumber <= DCM_MAX_MEMORY_ADD_SIZE_BYTENUM))
        {
            uint32 tempMemoryAddress = 0u;
            uint32 tempMemorySize    = 0u;
            for (uint8 index = 0u; index < addByteNumber; index++)
            {
                tempMemoryAddress |= (uint32)reqData[index] << ((addByteNumber - index - 1u) * 8u);
            }
            for (uint8 index = 0u; index < LenByteNumber; index++)
            {
                tempMemorySize |= (uint32)reqData[addByteNumber + index] << ((LenByteNumber - index - 1u) * 8u);
            }
            *MemoryAddress = tempMemoryAddress;
            *MemorySize    = tempMemorySize;
            result         = Dcm_UDS_CheckMemoryRangeCheck(
                MemoryCfg,
                memoryAccess,
                tempMemoryAddress,
                tempMemorySize,
                MemoryIdentifier,
                &memoryRangeInfo);
        }
    }

    /* check session/security/modeRule of the requested memoryRangeInfo */
    if ((E_OK == result) && (NULL_PTR != memoryRangeInfo))
    {
        result = Dcm_UDS_CheckMemoryAuth(memoryRangeInfo, ErrorCode);
    }

    if ((E_NOT_OK == result) && (DCM_POS_RESP == *ErrorCode))
    {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
    }

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif

#if ((STD_ON == DCM_UDS_0X34) || (STD_ON == DCM_UDS_0X35))
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * set lengthFormatId and blockLength to target address
 */
void Dcm_UDS_SetBlockLength(uint32 BlockLength, uint8* ResData, Dcm_MsgLenType* ResDataLen)
{
    uint8 lengthFormatIdentifier;

    /* get lengthFormatId based on blcokLength, the upper part stands for the number of byte of blockLength */
    if (BlockLength <= DCM_INVALID_UINT8) /* PRQA S 1252 */ /* VL_QAC_1252 */
    {
        lengthFormatIdentifier = 0x10u;
        ResData[1u]            = (uint8)BlockLength;
        *ResDataLen            = 2u;
    }
    else if (BlockLength <= DCM_INVALID_UINT16)
    {
        lengthFormatIdentifier = 0x20u;
        DcmInternal_TransformArray_u16(&ResData[1u], (uint16)BlockLength, DCM_OPAQUE);
        *ResDataLen = 3u;
    }
    else if (BlockLength <= DCM_INVALID_UINT24)
    {
        lengthFormatIdentifier = 0x30u;
        DcmInternal_TransformArray_u24(&ResData[1u], BlockLength, DCM_OPAQUE);
        *ResDataLen = 4u;
    }
    else
    {
        lengthFormatIdentifier = 0x40u;
        DcmInternal_TransformArray_u32(&ResData[1u], BlockLength, DCM_OPAQUE);
        *ResDataLen = 5u;
    }
    ResData[0u] = lengthFormatIdentifier;
}
/* PRQA S 3120 -- */
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif

#if (STD_ON == DCM_UDS_0X36)
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * initialize Dcm_TransferData
 */
void Dcm_UDS_InitTransferData(
    Dcm_TransferStatusType transferStatus,
    uint8                  memoryIdentifier,
    uint32                 memoryAddress,
    uint32                 blockLength,
    uint32                 memorySize)
{
    Dcm_TransferData.TransferStatus       = transferStatus;
    Dcm_TransferData.BlockSequenceCounter = 0u;
    Dcm_TransferData.FirstTransfer        = TRUE;
    Dcm_TransferData.MemoryIdentifier     = memoryIdentifier;
    Dcm_TransferData.MemoryAddress        = memoryAddress;
    Dcm_TransferData.BlockLength          = blockLength;
    Dcm_TransferData.MemorySizeRemain     = memorySize;
    Dcm_TransferData.LastSize             = 0u;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif

#if (                                                                                                            \
    (STD_ON == DCM_UDS_0X22) || (STD_ON == DCM_UDS_0X2E) || (STD_ON == DCM_UDS_0X24) || (STD_ON == DCM_UDS_0X2A) \
    || (STD_ON == DCM_UDS_0X2C))
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * check session/authentication/security/modeRule for requested did(DidInfo)
 */
Std_ReturnType Dcm_UDS_CheckReadWriteDid(
    uint16                         ConnectionId,
    uint16                         Did,
    const Dcm_DspDidReadWriteType* DcmDspDidReadWrite,
    Dcm_NegativeResponseCodeType*  ErrorCode)
{
    Std_ReturnType result = E_NOT_OK;

    if (NULL_PTR != DcmDspDidReadWrite)
    {
        result = Dcm_UDS0x10_CheckSession(
            DcmDspDidReadWrite->DidReadWriteSessionRef,
            DcmDspDidReadWrite->DidReadWriteSessionRefNum,
            NULL_PTR);

#if (STD_ON == DCM_UDS_0X29)
        if (E_OK == result)
        {
            result = Dcm_UDS0x29_DidAuthenticationCheck(ConnectionId, Did, DcmDspDidReadWrite, NULL_PTR, ErrorCode);
        }
#else
        DCM_UNUSED(ConnectionId);
        DCM_UNUSED(Did);
#endif

#if (STD_ON == DCM_UDS_0X27)
        if (E_OK == result)
        {
            result = Dcm_UDS0x27_CheckSecurity(
                DcmDspDidReadWrite->DidReadWriteSecurityLevelRef,
                DcmDspDidReadWrite->DidReadWriteSecurityLevelRefNum,
                ErrorCode);
        }
#endif

#if (STD_ON == DCM_DIDREADWRITEMODE)
        if ((E_OK == result) && (NULL_PTR != DcmDspDidReadWrite->DidReadWriteModeRuleRef))
        {
            result = DcmDspDidReadWrite->DidReadWriteModeRuleRef(ErrorCode);
        }
#endif
    }

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif

#if (                                                                                                            \
    (STD_ON == DCM_UDS_0X2E) || (STD_ON == DCM_UDS_0X2F) || (STD_ON == DCM_UDS_0X86) || (STD_ON == DCM_UDS_0X24) \
    || (STD_ON == DCM_UDS_0X2C) || (STD_ON == DCM_UDS_0X2A))
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * get did index of input did, classify didType
 */
Std_ReturnType
    Dcm_UDS_FindDid(uint16 Did, Dcm_DidType* didType, uint16* DidIndex, Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_NOT_OK;

#if (DCM_DID_NUM > 0)
    /* check for regular did first (based on config)*/
    for (uint16 index = 0u; index < DCM_DID_NUM; index++)
    {
        if ((Dcm_CfgPtr->DidUsed[index]) && (Did == Dcm_DspDid[index].DidIdentifier))
        {
            *DidIndex = index;
            if (NULL_PTR != didType)
            {
                *didType = DCM_REGULAR_DID;
            }
            result = E_OK;
            break;
        }
    }
#endif

    if (NULL_PTR != didType)
    {
#if (DCM_RANGE_DID_NUM > 0)
        /* only check for range did if not specifically configured to be a regular did */
        if (E_NOT_OK == result)
        {
            for (uint16 index = 0u; index < DCM_RANGE_DID_NUM; index++)
            {
                const Dcm_DspDidRangeType* DcmDspDidRangeCfg = &Dcm_DspDidRange[index];
                if ((Did >= DcmDspDidRangeCfg->DidRangeIdentifierLowerLimit)
                    && (Did <= DcmDspDidRangeCfg->DidRangeIdentifierUpperLimit))
                {
                    *DidIndex = index;
                    *didType  = DCM_RANGE_DID;
                    result    = E_OK;
                    break;
                }
            }
        }
#endif

#if (STD_ON == DCM_OBD_DID_MIRROR)
        /* check for pid and vehInfo availablity did */
        if ((((Did >= DCM_PIDDID_RANGE_LOWER) && (Did <= DCM_PIDDID_RANGE_UPPER))
             || ((Did >= DCM_VEHINFODID_RANGE_LOWER) && (Did <= DCM_VEHINFODID_RANGE_UPPER)))
            && (Dcm_OBD_isAvailability((uint8)(Did & DCM_OBD_DID_MASK), NULL_PTR)))
        {
            *didType = DCM_OBD_DID;
            result   = E_OK;
        }
        /* check for non-availablity OBD did when DcmDspEnableObdMirror is True */
        if ((E_NOT_OK == result)
            && (((Did >= DCM_PIDDID_RANGE_LOWER) && (Did <= DCM_PIDDID_RANGE_UPPER))
                || ((Did >= DCM_MIDDID_RANGE_LOWER) && (Did <= DCM_MIDDID_RANGE_UPPER))
                || ((Did >= DCM_VEHINFODID_RANGE_LOWER) && (Did <= DCM_VEHINFODID_RANGE_UPPER))))
        {
            *didType = DCM_OBD_DID;
            result   = E_OK;
        }
#endif
    }

    if ((E_NOT_OK == result) && (NULL_PTR != ErrorCode))
    {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
    }

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif

#if ((STD_ON == DCM_UDS_0X22) || (STD_ON == DCM_UDS_0X2A) || (STD_ON == DCM_UDS_0X2F))
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * read data from did signals, iterate over signal and process based on type of DcmDspDataUsePort
 */
/* PRQA S 1505++ */ /* VL_QAC_OneFunRef */
Std_ReturnType Dcm_UDS_SignalHandle(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint16                        DidIndex,
    uint8*                        DestBuffer,
    uint16*                       DataLength,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType finalResult = E_OK;

#if (DCM_DID_SIGNAL_NUM > 0)

    for (uint16 index = Dcm_DspDid[DidIndex].DidSignalIndexStart; index <= Dcm_DspDid[DidIndex].DidSignalIndexEnd;
         index++)
    {
        uint16                    offset      = Dcm_DspDidSignal[index].DidByteOffset;
        const Dcm_DspDataCfgType* dspData     = Dcm_DspDidSignal[index].DidDataRef;
        Dcm_DspDataUsePortType    dataUsePort = dspData->DspDataUsePort;
        Std_ReturnType            result      = E_OK;
        switch (dataUsePort) /* PRQA S 3315 */ /* VL_Dcm_3315 */
        {
#if ((STD_ON == DCM_USE_NVM) && (STD_ON == DCM_USE_BLOCK_ID_ENABLED))
        case DCM_USE_BLOCK_ID:
        {
            if (NULL_PTR != DestBuffer)
            {
                /* read data from configured nvm block */
                result = Dcm_UDS_SignalHandleNvM(OpStatus, dspData, &DestBuffer[offset]);
            }
            break;
        }
#endif
#if (STD_ON == DCM_USE_ECU_SIGNAL_ENABLED)
        case DCM_USE_ECU_SIGNAL:
        {
            /* read data from ecu signal (may be IoHwAb or callout) */
            if (NULL_PTR != DestBuffer)
            {
                result = dspData->ReadEcuSignal(&DestBuffer[offset]);
            }
            break;
        }
#endif
        default:
        {
            /* read data from CS/SR/callout */
            if (NULL_PTR != DestBuffer)
            {
                result = Dcm_UDS_SignalHandleDefault(OpStatus, dspData, &DestBuffer[offset], DataLength, ErrorCode);
            }
            break;
        }
        }
        /* the result is pending, save the final result */
        if (DCM_E_PENDING == result)
        {
            finalResult = DCM_E_PENDING;
        }
        else if (E_OK != result)
        {
            finalResult = E_NOT_OK;
            break;
        }
        else
        {
            /* idle */
        }
    }
#else
    finalResult = E_NOT_OK;
#endif

    return finalResult;
}
/* PRQA S 1505-- */ /* VL_QAC_OneFunRef */
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * read data of did, process by signal for DidUsePort set to DATE_ELEMENT, otherwise call DidReadFnc(SR/NV/BNDM)
 */
Std_ReturnType Dcm_UDS_DidReadHandle(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint16                        DidIndex,
    uint8*                        DestBuffer,
    Dcm_MsgLenType*               BufSize,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
#if (DCM_DID_NUM > 0)
    Std_ReturnType        result     = E_OK;
    const Dcm_DspDidType* didCfg     = &Dcm_DspDid[DidIndex];
    uint16                dataLength = 0u;

    /* did configuration size is larger than buffer size */
    if ((didCfg->DidSize > *BufSize) && (NULL_PTR != ErrorCode))
    {
        *ErrorCode = DCM_E_RESPONSETOOLONG;
        result     = E_NOT_OK;
    }

    /* first set data to 0 */
    if ((DCM_INITIAL == OpStatus) && (E_OK == result) && (NULL_PTR != DestBuffer))
    {
        DcmInternal_Memset(DestBuffer, 0u, didCfg->DidSize);
    }

    if (E_OK == result)
    {
        /* process depending on DcmDspDidUsePort */
        if (DCM_USE_DATA_ELEMENT_SPECIFIC_INTERFACES == didCfg->DidUsePort)
        {
            result = Dcm_UDS_SignalHandle(OpStatus, DidIndex, DestBuffer, &dataLength, ErrorCode);
        }
        else
        {
            result = didCfg->DidReadFnc(DestBuffer);
        }
    }

    /* extra dataLength for dynamic did (0 for non-dynamic one) */
    if (E_NOT_OK != result)
    {
        *BufSize = (Dcm_MsgLenType)didCfg->DidSize + (Dcm_MsgLenType)dataLength;
    }

    return result;
#else
    return E_NOT_OK;
#endif
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#if (STD_ON == DCM_DYN_DID)
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * read dynamically defined did data (from memory or other dids)
 */
Std_ReturnType Dcm_UDS_DynDidHandle(
    Dcm_ExtendedOpStatusType OpStatus,
    uint16                   ConnectionId,
    uint16                   DDDIdIndex,
    uint8*                   DestBuffer,
    Dcm_MsgLenType*          BufSize,
#if (STD_ON == DCM_DDDID_CHECK_SOURCE)
    boolean* checkFail,
#endif
    Dcm_NegativeResponseCodeType* ErrorCode)
{
#if (STD_OFF == DCM_UDS_0X2C)
    return E_NOT_OK;
#else
#if (DCM_DDDID_NUM > 0u)
    Std_ReturnType result      = E_OK;
    Std_ReturnType finalResult = E_OK;

    Dcm_MsgLenType offset = 0u;

    /* the requested dynamic did is not defined */
    if ((DCM_NOT_DEFINED == Dcm_DDDID[DDDIdIndex].DDDIDStatus[0]) && (NULL_PTR != ErrorCode))
    {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        result     = E_NOT_OK;
    }

    Dcm_MsgLenType tempBufSize = *BufSize;
    for (uint8 index = 0u; (index < Dcm_DDDID[DDDIdIndex].SourceElementsNum) && (E_NOT_OK != result); index++)
    {
        Dcm_DDDIDStatusType DDDIDStatus = Dcm_DDDID[DDDIdIndex].DDDIDStatus[index];
        if (DCM_DEFINED_SOURCE_DID == DDDIDStatus)
        {
            result = Dcm_UDS_DynDidDefinedDID(
                OpStatus,
                ConnectionId,
                DDDIdIndex,
                index,
                &offset,
                DestBuffer,
                &tempBufSize,
#if (STD_ON == DCM_DDDID_CHECK_SOURCE)
                checkFail,
#endif
                ErrorCode);
        }
        else if (DCM_DEFINED_MEMORY_ADD == DDDIDStatus)
        {
            result = Dcm_UDS_DynDidDefinedAdd(OpStatus, DDDIdIndex, index, &offset, DestBuffer, BufSize, ErrorCode);
        }
        else
        {
            /* idle */
        }

        if ((E_OK == finalResult) || (DCM_E_FORCE_RCRRP == result))
        {
            finalResult = result;
        }
    }

    if (E_NOT_OK == result)
    {
        finalResult = E_NOT_OK;
    }
    *BufSize = offset;

    return finalResult;
#else
    return E_NOT_OK;
#endif
#endif
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
#endif
/* ========================================== internal function definitions ========================================= */
#if ((STD_ON == DCM_UDS_0X23) || (STD_ON == DCM_UDS_0X34) || (STD_ON == DCM_UDS_0X2C) || (STD_ON == DCM_UDS_0X3D))
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/* check addAndLenFid and request length */
/* PRQA S 6040 ++ */ /* VL_MTR_Dcm_STPAR */
DCM_LOCAL Std_ReturnType Dcm_UDS_CheckMemoryCondition(
    const Dcm_DspMemoryType*      MemoryCfg,
    uint8                         AddAndLenFid,
    Dcm_MsgLenType                byteNumber,
    uint8*                        addByteNumber,
    uint8*                        LenByteNumber,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_NOT_OK;
    if (NULL_PTR != MemoryCfg)
    {
        if (0u == MemoryCfg->SupportedAddressAndLengthFormatIdentifierNum)
        {
            result = E_OK;
        }
        else
        {
            for (uint8 index = 0u; index < MemoryCfg->SupportedAddressAndLengthFormatIdentifierNum; index++)
            {
                if (AddAndLenFid == MemoryCfg->SupportedAddressAndLengthFormatIdentifier[index])
                {
                    result = E_OK;
                    break;
                }
            }
        }
    }

    if (E_OK == result)
    {
        /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
        *LenByteNumber = AddAndLenFid >> 4u;
        *addByteNumber = AddAndLenFid & 0x0Fu;
        /* PRQA S 3120 -- */
        if ((0uL != byteNumber) && (byteNumber != ((Dcm_MsgLenType)*addByteNumber + (Dcm_MsgLenType)*LenByteNumber)))
        {
            *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
            result     = E_NOT_OK;
        }
        if ((0u == *addByteNumber) || (0u == *LenByteNumber))
        {
            result = E_NOT_OK;
        }
    }

    return result;
}

/* iterate over memoryIdInfo to find matched range */
DCM_LOCAL Std_ReturnType Dcm_UDS_CheckMemoryRangeCheck(
    const Dcm_DspMemoryType*           MemoryCfg,
    uint8                              memoryAccess,
    uint32                             MemoryAddress,
    uint32                             MemorySize,
    uint8*                             MemoryIdentifier,
    const Dcm_DspMemoryRangeInfoType** targetMemoryInfo)
{
    Std_ReturnType result = E_NOT_OK;

    if ((MemoryAddress + MemorySize) > MemoryAddress)
    {
#if (DCM_MEMORYIDINFO_NUM > 0)
        for (uint8 index = 0u; index < MemoryCfg->MemoryIdInfoNum; index++)
        {
            const Dcm_DspMemoryIdInfoType* MemoryIdInfo = &MemoryCfg->MemoryIdInfo[index];

            if (DCM_READ_MEMORY == memoryAccess)
            {
                result = Dcm_UDS_FindMemoryRange(
                    MemoryAddress,
                    MemorySize,
                    MemoryIdInfo->ReadMemoryRangeInfo,
                    MemoryIdInfo->ReadMemoryRangeInfoNum,
                    targetMemoryInfo);
            }
            else
            {
                result = Dcm_UDS_FindMemoryRange(
                    MemoryAddress,
                    MemorySize,
                    MemoryIdInfo->WriteMemoryRangeInfo,
                    MemoryIdInfo->WriteMemoryRangeInfoNum,
                    targetMemoryInfo);
            }

            if (NULL_PTR != MemoryIdInfo->MemoryIdValue)
            {
                *MemoryIdentifier = *MemoryIdInfo->MemoryIdValue;
            }

            if (E_OK == result)
            {
                break;
            }
        }
#endif
    }

    return result;
}
/* PRQA S 6040 -- */

/* iterate over input memoryInfo to find matched rangeInfo */
DCM_LOCAL Std_ReturnType Dcm_UDS_FindMemoryRange(
    uint32                             MemoryAddress,
    uint32                             MemorySize,
    const Dcm_DspMemoryRangeInfoType*  memoryInfo,
    uint16                             infoNum,
    const Dcm_DspMemoryRangeInfoType** targetMemoryInfo)
{
    Std_ReturnType result = E_NOT_OK;
    for (uint8 index = 0u; index < infoNum; index++)
    {
        if ((MemoryAddress >= memoryInfo[index].MemoryRangeLow)
            && ((MemoryAddress + MemorySize) <= memoryInfo[index].MemoryRangeHigh))
        {
            *targetMemoryInfo = &memoryInfo[index];
            result            = E_OK;
            break;
        }
    }

    return result;
}

/* check security/session/modeRule requirement for the specific memoryRange */
DCM_LOCAL Std_ReturnType
    Dcm_UDS_CheckMemoryAuth(const Dcm_DspMemoryRangeInfoType* memoryRangeInfo, Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = Dcm_UDS0x10_CheckSession(
        memoryRangeInfo->MemoryRangeSessionLevelRef,
        memoryRangeInfo->MemoryRangeSessionLevelRefNum,
        NULL_PTR);

#if (STD_ON == DCM_UDS_0X27)
    if (E_OK == result)
    {
        result = Dcm_UDS0x27_CheckSecurity(
            memoryRangeInfo->MemoryRangeSecurityLevelRef,
            memoryRangeInfo->MemoryRangeSecurityLevelRefNum,
            ErrorCode);
    }
#endif

#if (STD_ON == DCM_MEMORYRANGEMODE)
    if ((E_OK == result) && (NULL_PTR != memoryRangeInfo->MemoryRangeModeRuleRef))
    {
        result = memoryRangeInfo->MemoryRangeModeRuleRef(ErrorCode);
    }
#endif

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif

#if ((STD_ON == DCM_UDS_0X22) || (STD_ON == DCM_UDS_0X2A) || (STD_ON == DCM_UDS_0X2F))
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
#if ((STD_ON == DCM_USE_NVM) && (STD_ON == DCM_USE_BLOCK_ID_ENABLED))
/* Calls NvM APIs to read data from nvm block */
DCM_LOCAL Std_ReturnType
    Dcm_UDS_SignalHandleNvM(Dcm_ExtendedOpStatusType OpStatus, const Dcm_DspDataCfgType* dspData, uint8* DestBuffer)
{
    Std_ReturnType result = E_OK;

    if (DCM_INITIAL == OpStatus)
    {
        (void)NvM_CancelJobs(dspData->DspDataBlockIdRef);
        result = NvM_ReadBlock(dspData->DspDataBlockIdRef, DestBuffer);
        if (E_OK == result)
        {
            result = DCM_E_PENDING;
        }
    }
    else if ((DCM_PENDING == OpStatus) || (DCM_E_FORCE_RCRRP == OpStatus))
    {
        NvM_RequestResultType requestResult = NVM_REQ_NOT_OK;
        (void)NvM_GetErrorStatus(dspData->DspDataBlockIdRef, &requestResult);
        if (NVM_REQ_PENDING == requestResult)
        {
            result = DCM_E_PENDING;
        }
        else if ((NVM_REQ_OK != requestResult) && (NVM_REQ_RESTORED_FROM_ROM != requestResult))
        {
            result = E_NOT_OK;
        }
        else
        {
            result = E_OK;
        }
    }
    else if (DCM_CANCEL == OpStatus)
    {
        (void)NvM_CancelJobs(dspData->DspDataBlockIdRef);
    }
    else
    {
        /* idle */
    }

    return result;
}
#endif

/* read data from signal using callouts(CS/SR/Callout) */
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
DCM_LOCAL Std_ReturnType Dcm_UDS_SignalHandleDefault(
    Dcm_ExtendedOpStatusType      OpStatus,
    const Dcm_DspDataCfgType*     dspData,
    uint8*                        DestBuffer,
    uint16*                       DataLength,
    Dcm_NegativeResponseCodeType* ErrorCode)
/* PRQA S 3673 -- */
{
    Std_ReturnType result = E_OK;

    if ((NULL_PTR != dspData->ConditionCheckReadFnc) && (NULL_PTR != ErrorCode))
    {
        result = dspData->ConditionCheckReadFnc(OpStatus, ErrorCode);
#if (STD_ON == DCM_DEV_ERROR_DETECT)
        if ((E_OK != result) && (E_NOT_OK != result))
        {
            DCM_DET_REPORT(DCM_MAIN_FUNCTION_ID, DCM_E_INTERFACE_RETURN_VALUE);
            *ErrorCode = DCM_E_GENERALREJECT;
            result     = E_NOT_OK;
        }
#endif
    }

#if (STD_ON == DCM_DYN_DATA)
    if ((E_OK == result) && (NULL_PTR != ErrorCode))
    {
        if (DCM_UINT8_DYN == dspData->DspDataType)
        {
            result = dspData->ReadDataLengthFnc(OpStatus, DataLength);
            if ((E_OK == result) && (*DataLength > dspData->DspDataByteSize))
            {
                result = E_NOT_OK;
            }
        }
    }
#else
    DCM_UNUSED(DataLength);
#endif

    if ((E_OK == result) && (NULL_PTR != DestBuffer))
    {
        result = dspData->DspDataReadFnc(OpStatus, DestBuffer, ErrorCode);
#if (STD_ON == DCM_DEV_ERROR_DETECT)
        if ((E_OK != result) && (E_NOT_OK != result) && (DCM_E_PENDING != result))
        {
            DCM_DET_REPORT(DCM_MAIN_FUNCTION_ID, DCM_E_INTERFACE_RETURN_VALUE);
            if (NULL_PTR != ErrorCode)
            {
                *ErrorCode = DCM_E_GENERALREJECT;
            }
            result = E_NOT_OK;
        }
#endif
    }

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif

#if (STD_ON == DCM_DYN_DID)
/* prepare for reading dynamic did that is defined using source did */
DCM_LOCAL Std_ReturnType Dcm_UDS_DynDidDefinedDID(
    Dcm_ExtendedOpStatusType OpStatus,
    uint16                   ConnectionId,
    uint16                   DDDIdIndex,
    uint8                    index,
    Dcm_MsgLenType*          offset,
    uint8*                   DestBuffer,
    Dcm_MsgLenType*          BufSize,
#if (STD_ON == DCM_DDDID_CHECK_SOURCE)
    boolean* checkFail,
#endif
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_OK;

#if ((STD_ON == DCM_DDDID_CHECK_SOURCE) && (DCM_DDDID_NUM > 0u))
    if (NULL_PTR != checkFail)
    {
        result = Dcm_UDS_CheckReadWriteDid(
            ConnectionId,
            Dcm_DspDid[Dcm_DDDID[DDDIdIndex].DDDIDDefine[index].SourceDidIndex].DidIdentifier,
            DCM_DID_INFO(Dcm_DDDID[DDDIdIndex].DDDIDDefine[index].SourceDidIndex).DidRead,
            ErrorCode);
        if (E_OK != result)
        {
            *checkFail = TRUE;
        }
    }
#else
    DCM_UNUSED(ConnectionId);
#endif
    if (E_OK == result)
    {
        result = Dcm_UDS_DynDidDefinedDIDHandle(OpStatus, DDDIdIndex, index, offset, DestBuffer, BufSize, ErrorCode);
        if (E_NOT_OK != result)
        {
            *BufSize = *BufSize - *offset;
        }
    }
    return result;
}

/* read dynamic did that is defined using source did */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
DCM_LOCAL Std_ReturnType Dcm_UDS_DynDidDefinedDIDHandle(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint16                        DDDIdIndex,
    uint8                         index,
    Dcm_MsgLenType*               offset,
    uint8*                        DestBuffer,
    Dcm_MsgLenType*               BufSize,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result;
#if (DCM_DDDID_NUM > 0u)
    Dcm_MsgLenType datasize = *BufSize;
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
    if (NULL_PTR == DestBuffer)
    {
        result = Dcm_UDS_DidReadHandle(
            OpStatus,
            Dcm_DDDID[DDDIdIndex].DDDIDDefine[index].SourceDidIndex,
            NULL_PTR,
            &datasize,
            ErrorCode);
    }
    else
#endif
    {
        result = Dcm_UDS_DidReadHandle(
            OpStatus,
            Dcm_DDDID[DDDIdIndex].DDDIDDefine[index].SourceDidIndex,
            &DestBuffer[*offset],
            &datasize,
            ErrorCode);
    }

    if (E_OK == result)
    {
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
        if (NULL_PTR != DestBuffer)
#endif
        {
            DcmInternal_Memcpy(
                &DestBuffer[*offset],
                &DestBuffer[Dcm_DDDID[DDDIdIndex].DDDIDDefine[index].SourceDidPosition],
                Dcm_DDDID[DDDIdIndex].DDDIDDefine[index].SourceDidSize);
        }
        (*offset) += Dcm_DDDID[DDDIdIndex].DDDIDDefine[index].SourceDidSize;
        *BufSize = *BufSize - *offset;
    }
#else
    result = E_NOT_OK;
#endif

    return result;
}

/* read dynamic did that is defined by memory address and memory size */
DCM_LOCAL Std_ReturnType Dcm_UDS_DynDidDefinedAdd(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint16                        DDDIdIndex,
    uint8                         index,
    Dcm_MsgLenType*               offset,
    uint8*                        DestBuffer,
    Dcm_MsgLenType*               BufSize,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
#if (DCM_DDDID_NUM > 0u)
    Std_ReturnType result = E_OK;

    if ((*BufSize < Dcm_DDDID[DDDIdIndex].DDDIDDefine[index].MemorySize) && (NULL_PTR != ErrorCode))
    {
        *ErrorCode = DCM_E_RESPONSETOOLONG;
        result     = E_NOT_OK;
    }

    if (E_OK == result)
    {
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
        if (NULL_PTR != DestBuffer)
#endif
        {
            Dcm_NegativeResponseCodeType tempErrorCode;
            result = (Std_ReturnType)Dcm_ReadMemory(
                OpStatus,
                Dcm_DDDID[DDDIdIndex].DDDIDDefine[index].MemoryIdentifier,
                Dcm_DDDID[DDDIdIndex].DDDIDDefine[index].MemoryAddress,
                Dcm_DDDID[DDDIdIndex].DDDIDDefine[index].MemorySize,
                &DestBuffer[*offset],
                &tempErrorCode);
        }
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
        else
        {
            result = (Std_ReturnType)DCM_READ_OK;
        }
#endif
    }

    switch (result)
    {
    case (Std_ReturnType)DCM_READ_OK:
    {
        (*offset) += Dcm_DDDID[DDDIdIndex].DDDIDDefine[index].MemorySize;
        *BufSize = *BufSize - Dcm_DDDID[DDDIdIndex].DDDIDDefine[index].MemorySize;
        break;
    }
    case (Std_ReturnType)DCM_READ_PENDING:
    {
        result = DCM_E_PENDING;
        break;
    }
    case (Std_ReturnType)DCM_READ_FORCE_RCRRP:
    {
        result = DCM_E_FORCE_RCRRP;
        break;
    }
    case (Std_ReturnType)DCM_READ_FAILED:
    {
        if (NULL_PTR != ErrorCode)
        {
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        }
        result = E_NOT_OK;
        break;
    }
    default:
    {
#if (STD_ON == DCM_DEV_ERROR_DETECT)
        DCM_DET_REPORT(DCM_MAIN_FUNCTION_ID, DCM_E_INTERFACE_RETURN_VALUE);
        if (NULL_PTR != ErrorCode)
        {
            *ErrorCode = DCM_E_GENERALREJECT;
        }
        result = E_NOT_OK;
#else
/* idle */
#endif
        break;
    }
    }
    return result;
#else
    return E_NOT_OK;
#endif
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
