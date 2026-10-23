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
 **  @file               : Dcm_UDS0x22.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm UDS service 0x22 function implementation
 **
 ***********************************************************************************************************************/
/* PRQA S 3120, 1532 EOF */ /* VL_QAC_MagicNum, VL_QAC_OneFunRef */
/* =================================================== inclusions =================================================== */
#include "Dcm_Cfg.h"
#if (STD_ON == DCM_UDS_0X22)
#include "Dcm_Internal.h"
/* ===================================================== macros ===================================================== */
/**
 * @brief The minimum request length of UDS 0x22
 */
#define DCM_UDS22_MINREQLEN         2uL
/**
 * @brief The assigned did for reading current session
 */
#define DCM_ACTIVE_DIAG_SESSION_DID 0xF186u

/* ============================================ internal data definitions =========================================== */
/**
 * @brief did read status
 */
#ifdef DCM_MAX_DID_TO_READ
#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
/**
 * @brief An array to store the status of Diagnostic Identification (DID) read operations @range 0..255.
 */
DCM_LOCAL uint8 Dcm_ReadCheckStatus[DCM_MAX_DID_TO_READ]; /* PRQA S 3218 */ /* VL_Dcm_3218 */
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
#else
#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
/**
 * @brief An array to store the status of Diagnostic Identification (DID) read operations @range 0..255.
 */
DCM_LOCAL uint8 Dcm_ReadCheckStatus[DCM_DID_NUM]; /* PRQA S 3218 */ /* VL_Dcm_3218 */
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
#endif

#define DCM_START_SEC_VAR_CLEARED_32
#include "Dcm_MemMap.h"
DCM_LOCAL uint32 Dcm_0x22ReadOffset; /* PRQA S 3218 */ /* VL_Dcm_3218 */
#define DCM_STOP_SEC_VAR_CLEARED_32
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_16
#include "Dcm_MemMap.h"
DCM_LOCAL uint16 Dcm_0x22ReadDidIndex; /* PRQA S 3218 */ /* VL_Dcm_3218 */
#define DCM_STOP_SEC_VAR_CLEARED_16
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
DCM_LOCAL Dcm_ExtendedOpStatusType Dcm_0x22ReadOpStatus; /* PRQA S 3218 */ /* VL_Dcm_3218 */
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
/* ========================================== internal function declarations ======================================== */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * @brief         This function sets did status
 * @param[in]     result      : the previous result
 * @param[in]     did         : the requested did
 * @param[in]     index       : the current reading index
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[inout]  offset      : the current and resulting offset
 * @param[inout]  bufSize     : the current available buffer size and resulting data size
 * @return        void
 * @reentrant     TRUE
 * @synchronous   Synchronous
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL void
    Dcm_UDS0x22_HandleDidStatus(uint16 did, Dcm_MsgContextType* pMsgContext, uint32* offset, Dcm_MsgLenType* bufSize);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#if (STD_ON == DCM_DID_RANGE_ENABLED)
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * @brief         This function gets the configured did index, also applying to rangeDid and OBDDid.
 * @param[in]     OpStatus      : Indicates the current operation status
 * @param[in]     ConnectionId  : the current connectionid
 * @param[in]     Did           : the input did
 * @param[in]     RangeDidIndex : the input range did index
 * @param[out]    DestBuffer    : address to output the did data
 * @param[inout]  BufSize       : When the function is called this parameter contains the maximum number of data bytes
 *                                that can be written to the buffer. The function returns the actual number of written
 *                                data bytes in this parameter.
 * @param[out]    ErrorCode     : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                                shall send a negative response with NRC code equal to the parameter ErrorCode
 *                                parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was unsuccessful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     TRUE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x22_RangeDidHandle(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint16                        ConnectionId,
    uint16                        Did,
    uint16                        RangeDidIndex,
    uint8*                        DestBuffer,
    Dcm_MsgLenType*               BufSize,
    Dcm_NegativeResponseCodeType* ErrorCode);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"

/**
 * @brief         This function reads data of range did
 * @param[in]     OpStatus          : Indicates the current operation status
 * @param[in]     Did               : the input did
 * @param[in]     DcmDspDidRangeCfg : the DcmDspDidRange Configuration
 * @param[inout]  BufSize           : When the function is called this parameter contains the maximum number of data
 *                                    bytes that can be written to the buffer. The function returns the actual number of
 *                                    written data bytes in this parameter.
 * @param[out]    DestBuffer        : the pointer for result
 * @param[out]    ErrorCode         : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm
 *                                    module shall send a negative response with NRC code equal to the parameter
 *                                    ErrorCode parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was unsuccessful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     TRUE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x22_RangeDidReadData(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint16                        Did,
    const Dcm_DspDidRangeType*    DcmDspDidRangeCfg,
    uint8*                        DestBuffer,
    Dcm_MsgLenType*               BufSize,
    Dcm_NegativeResponseCodeType* ErrorCode);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
#if (STD_ON == DCM_OBD_DID_MIRROR)
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * @brief         This function deals with obd did to get data
 * @param[in]     OpStatus   : Indicates the current operation status
 * @param[in]     Did        : the input did
 * @param[out]    DestBuffer : address to output the did data
 * @param[inout]  BufSize    : When the function is called this parameter contains the maximum number of data bytes that
 *                             can be written to the buffer. The function returns the actual number of written data
 *                             bytes in this parameter.
 * @param[out]    ErrorCode  : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                             shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                             value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was unsuccessful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     TRUE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */

DCM_LOCAL Std_ReturnType Dcm_UDS0x22_OBDDidHandle(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint16                        Did,
    uint8*                        DestBuffer,
    Dcm_MsgLenType*               BufSize,
    Dcm_NegativeResponseCodeType* ErrorCode);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * @brief         This function gets the did length to read for pagedBuffer functionality
 * @param[in]     Did          : the input did
 * @param[in]     ConnectionId : the current connection id
 * @param[out]    DataLength   : The data length of the did
 * @param[out]    ErrorCode    : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                               shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                               value.
 * @return        Std_ReturnType
 * @retval        E_OK: Request was successful
 * @retval        E_NOT_OK: Request was unsuccessful
 * @retval        DCM_E_PENDING: Request is not yet finished
 * @retval        DCM_E_FORCE_RCRRP: Application request the transmission of a response Response Pending (NRC 0x78)
 * @reentrant     TRUE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x22_GetDidLength(
    uint16                        Did,
    uint16                        ConnectionId,
    Dcm_MsgLenType*               DataLength,
    Dcm_NegativeResponseCodeType* ErrorCode);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"

/**
 * @brief         This function handles the result of read Did and deal with pagedBuffer initialization
 * @param[in]     protocolId  : the current protocol id
 * @param[in]     readResult  : the result of read did
 * @param[in]     index       : the index of the requested did
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK: Request was successful
 * @retval        E_NOT_OK: Request was unsuccessful
 * @retval        DCM_E_PENDING: Request is not yet finished
 * @retval        DCM_E_FORCE_RCRRP: Application request the transmission of a response Response Pending (NRC 0x78)
 * @reentrant     TRUE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x22_ReadDidPostProcess(
    uint8                         protocolId,
    Std_ReturnType                readResult,
    uint16                        index,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"

/**
 * @brief         This function setup the initial read status and iterate over requested did
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[in]     protocolId  : the current protocol id
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK: Request was successful
 * @retval        E_NOT_OK: Request was unsuccessful
 * @retval        DCM_E_PENDING: Request is not yet finished
 * @retval        DCM_E_FORCE_RCRRP: Application request the transmission of a response Response Pending (NRC 0x78)
 * @reentrant     TRUE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x22_ReadDidPrepare(
    Dcm_ExtendedOpStatusType OpStatus,
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
    uint8 protocolId,
#endif
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"

/**
 * @brief         This function reads the data of a did
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[in]     index       : the index of the requested did
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[inout]  offset      : the current offset of the response buffer
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK: Request was successful
 * @retval        E_NOT_OK: Request was unsuccessful
 * @retval        DCM_E_PENDING: Request is not yet finished
 * @retval        DCM_E_FORCE_RCRRP: Application request the transmission of a response Response Pending (NRC 0x78)
 * @reentrant     TRUE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x22_ReadDid(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint16                        index,
    Dcm_MsgContextType*           pMsgContext,
    uint32*                       offset,
    Dcm_NegativeResponseCodeType* ErrorCode);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"

/**
 * @brief         This function reads a did based on is type (regular/range/OBD)
 * @param[in]     OpStatus     : Indicates the current operation status
 * @param[in]     connectionId : the current connection id
 * @param[in]     did          : the requested did
 * @param[in]     didType      : the type of the requested did
 * @param[in]     didIndex     : the index of the requested did
 * @param[inout]  DestBuffer   : output buffer
 * @param[inout]  bufSize      : When the function is called this parameter contains the maximum number of data bytes
 * that can be written to the buffer. The function returns the actual number of written data bytes in this parameter.
 * @param[out]    ErrorCode    : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                               shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                               value.
 * @return        Std_ReturnType
 * @retval        E_OK: Request was successful
 * @retval        E_NOT_OK: Request was unsuccessful
 * @retval        DCM_E_PENDING: Request is not yet finished
 * @retval        DCM_E_FORCE_RCRRP: Application request the transmission of a response Response Pending (NRC 0x78)
 * @reentrant     TRUE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x22_ReadDidProcess(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint16                        connectionId,
    uint16                        did,
    Dcm_DidType                   didType,
    uint16                        didIndex,
    uint8*                        DestBuffer,
    Dcm_MsgLenType*               bufSize,
    Dcm_NegativeResponseCodeType* ErrorCode);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/* ========================================== external function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * The service interpreter for UDS 0x22
 */
Std_ReturnType Dcm_UDS0x22(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result     = E_NOT_OK;
    Dcm_MsgLenType reqDataLen = pMsgContext->reqDataLen;
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
    uint8 protocolId;
    (void)DslInternal_FindProtocolRowByRxPduId(pMsgContext->dcmRxPduId, &protocolId, NULL_PTR);
#endif

    if ((reqDataLen < DCM_UDS22_MINREQLEN) || ((reqDataLen % DCM_DID_LENGTH) != 0uL))
    {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
    }
#ifdef DCM_MAX_DID_TO_READ
    else if ((reqDataLen / DCM_DID_LENGTH) > DCM_MAX_DID_TO_READ)
    {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
    }
#endif
    else
    {
        result = Dcm_UDS0x22_ReadDidPrepare(
            OpStatus,
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
            protocolId,
#endif
            pMsgContext,
            ErrorCode);
    }

#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
    Dcm_ProtocolCtrlType* protocolCtrlPtr = &Dcm_ProtocolCtrl[protocolId];

    if ((TRUE == protocolCtrlPtr->PagedBufferStarted) && (E_OK != result))
    {
        DslInternal_PagedBufferInit(protocolCtrlPtr);
    }
#endif

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/* ========================================== internal function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/* setup read did status and iterate over did to read */
/* PRQA S 6030,6010,6080 ++ */ /* VL_MTR_Dcm_STMIF,VL_MTR_Dcm_STCYC,VL_MTR_Dcm_STPTH */
DCM_LOCAL Std_ReturnType Dcm_UDS0x22_ReadDidPrepare(
    Dcm_ExtendedOpStatusType OpStatus,
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
    uint8 protocolId,
#endif
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
/* PRQA S 6030,6010,6080 -- */
{
    Std_ReturnType finalResult = E_NOT_OK;
    uint16         didNum      = (uint16)(pMsgContext->reqDataLen / DCM_DID_LENGTH);

#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
    if (TRUE == Dcm_ProtocolCtrl[protocolId].PagedBufferStarted && OpStatus != DCM_PENDING)
    {
        Dcm_0x22ReadOffset = 0u;
        finalResult        = E_OK;
    }
#endif
    if ((DCM_INITIAL == OpStatus)
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
        && (FALSE == Dcm_ProtocolCtrl[protocolId].PagedBufferStarted)
#endif
    )
    {
        DcmInternal_Memset((uint8*)Dcm_ReadCheckStatus, 0u, (uint32)didNum);
        pMsgContext->resDataLen = 0u;
        Dcm_0x22ReadOffset      = 0u;
        Dcm_0x22ReadDidIndex    = 0u;
        Dcm_0x22ReadOpStatus    = DCM_INITIAL;
        for (uint16 index = 0u; index < didNum; index++)
        {
            uint16      connectionId;
            uint16      didIndex;
            Dcm_DidType didType;
            uint16      did = DCM_U8N_TO_U16(&pMsgContext->reqData[index * DCM_DID_LENGTH]);
            (void)DslInternal_FindProtocolRowByRxPduId(pMsgContext->dcmRxPduId, NULL_PTR, &connectionId);
            Std_ReturnType result = Dcm_UDS_FindDid(did, &didType, &didIndex, NULL_PTR);
            if (result == E_OK)
            {
                if (DCM_REGULAR_DID == didType)
                {
                    result = Dcm_UDS_CheckReadWriteDid(connectionId, did, DCM_DID_INFO(didIndex).DidRead, ErrorCode);
                }
            }

            if ((result == E_NOT_OK) && (DCM_POS_RESP != *ErrorCode)) /* PRQA S 2995 */ /* VL_Dcm_2995 */
            {
                finalResult = E_NOT_OK;
                break;
            }
            else if ((result == E_NOT_OK) && (DCM_POS_RESP == *ErrorCode)) /* PRQA S 2995 */ /* VL_Dcm_2995 */
            {
                Dcm_ReadCheckStatus[index] = 1u;
            }
            else
            {
                finalResult = E_OK;
            }
        }
    }
    else
    {
        finalResult = E_OK;
    }

    for (uint16 index = 0u; (index < didNum) && (finalResult == E_OK); index++)
    {
        if ((Dcm_ReadCheckStatus[index] != 1u) && (Dcm_0x22ReadDidIndex <= index))
        {
            uint32         Dcm_0x22ReadOffsetTemp = Dcm_0x22ReadOffset;
            Std_ReturnType result =
                Dcm_UDS0x22_ReadDid(Dcm_0x22ReadOpStatus, index, pMsgContext, &Dcm_0x22ReadOffset, ErrorCode);

            if ((result == DCM_E_PENDING) || (result == DCM_E_FORCE_RCRRP) || (result == E_NOT_OK))
            {
                finalResult          = result;
                Dcm_0x22ReadOpStatus = DCM_PENDING;
                break;
            }
            if ((result == E_OK) && (Dcm_0x22ReadOffset != Dcm_0x22ReadOffsetTemp))
            {
                Dcm_0x22ReadDidIndex++;
                Dcm_0x22ReadOpStatus = DCM_INITIAL;
            }
        }
    }

    if (((0uL == Dcm_0x22ReadOffset) && (E_OK == finalResult))
        || ((E_NOT_OK == finalResult) && (DCM_POS_RESP == *ErrorCode)))
    {
        *ErrorCode  = DCM_E_REQUESTOUTOFRANGE;
        finalResult = E_NOT_OK;
    }

    return finalResult;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/* find and check did, then call readDidProcess to read */
DCM_LOCAL Std_ReturnType Dcm_UDS0x22_ReadDid(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint16                        index,
    Dcm_MsgContextType*           pMsgContext,
    uint32*                       offset,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    uint16         did = DCM_U8N_TO_U16(&pMsgContext->reqData[index * DCM_DID_LENGTH]);
    uint16         didIndex;
    Dcm_DidType    didType;
    uint8*         resData       = pMsgContext->resData;
    Dcm_MsgLenType resMaxDataLen = pMsgContext->resMaxDataLen;
    Dcm_MsgLenType bufSize       = (resMaxDataLen < (*offset + 2uL)) ? 0uL : (resMaxDataLen - *offset - 2uL);
    uint16         connectionId;

#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
    uint8 protocolId;
    (void)DslInternal_FindProtocolRowByRxPduId(pMsgContext->dcmRxPduId, &protocolId, &connectionId);
#else
    (void)DslInternal_FindProtocolRowByRxPduId(pMsgContext->dcmRxPduId, NULL_PTR, &connectionId);
#endif
    Std_ReturnType result = Dcm_UDS_FindDid(did, &didType, &didIndex, NULL_PTR);

    if (E_OK == result)
    {
        result = Dcm_UDS0x22_ReadDidProcess(
            OpStatus,
            connectionId,
            did,
            didType,
            didIndex,
            &resData[*offset + DCM_DID_LENGTH],
            &bufSize,
            ErrorCode);
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
        if (DCM_E_RESPONSETOOLONG == *ErrorCode)
        {
            result = Dcm_UDS0x22_ReadDidPostProcess(protocolId, result, index, pMsgContext, ErrorCode);
        }
        else
#endif
            if (E_OK == result)
        {
            Dcm_UDS0x22_HandleDidStatus(did, pMsgContext, offset, &bufSize);
        }
    }

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/* PRQA S 6040++ */ /* VL_MTR_Dcm_STPAR */
DCM_LOCAL void
    Dcm_UDS0x22_HandleDidStatus(uint16 did, Dcm_MsgContextType* pMsgContext, uint32* offset, Dcm_MsgLenType* bufSize)
/* PRQA S 6040-- */ /* VL_MTR_Dcm_STPAR */
{
    uint8* resData = pMsgContext->resData;
    DcmInternal_TransformArray_u16(&resData[*offset], did, DCM_OPAQUE);
    (*offset) += *bufSize + DCM_DID_LENGTH;
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
    uint8                 protocolId;
    Dcm_ProtocolCtrlType* protocolCtrlPtr;
    (void)DslInternal_FindProtocolRowByRxPduId(pMsgContext->dcmRxPduId, &protocolId, NULL_PTR);
    protocolCtrlPtr = &Dcm_ProtocolCtrl[protocolId];
    protocolCtrlPtr->RemainPageLength += *bufSize + DCM_DID_LENGTH;
#endif
    pMsgContext->resDataLen += *bufSize + DCM_DID_LENGTH;
    *bufSize = pMsgContext->resMaxDataLen - *offset;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/* actually read the did based on its type */
/* PRQA S 6040++ */ /* VL_MTR_Dcm_STPAR */
DCM_LOCAL Std_ReturnType Dcm_UDS0x22_ReadDidProcess(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint16                        connectionId,
    uint16                        did,
    Dcm_DidType                   didType,
    uint16                        didIndex,
    uint8*                        DestBuffer,
    Dcm_MsgLenType*               bufSize,
    Dcm_NegativeResponseCodeType* ErrorCode)
/* PRQA S 6040-- */ /* VL_MTR_Dcm_STPAR */
{
    Std_ReturnType result = E_OK;
#if (STD_ON != DCM_DYN_DID)
    DCM_UNUSED(connectionId);
    DCM_UNUSED(did);
#endif

    if (DCM_ACTIVE_DIAG_SESSION_DID == did)
    {
        if ((1u > *bufSize) && (NULL_PTR != ErrorCode))
        {
            *ErrorCode = DCM_E_RESPONSETOOLONG;
            result     = E_NOT_OK;
        }
        else
        {
            DestBuffer[0] = Dcm_SessionCtrl.CurrentSession;
            *bufSize      = 1u;
        }
    }
    else
    {
        switch (didType)
        {
        case DCM_REGULAR_DID:
        {
#if (STD_ON == DCM_DYN_DID)
            if ((did >= DCM_UDS_DYNDID_RANGE_LOWER) && (did <= DCM_UDS_DYNDID_RANGE_UPPER)
                && (TRUE == DCM_DID_INFO(didIndex).DynamicallyDefined))
            {
                result = Dcm_UDS_DynDidHandle(
                    OpStatus,
                    connectionId,
                    DCM_DID_INFO(didIndex).DDDidIndex,
                    DestBuffer,
                    bufSize,
#if (STD_ON == DCM_DDDID_CHECK_SOURCE)
                    NULL_PTR,
#endif
                    ErrorCode);
            }
            else
#endif
            {
                result = Dcm_UDS_DidReadHandle(OpStatus, didIndex, DestBuffer, bufSize, ErrorCode);
            }
            break;
        }
#if (STD_ON == DCM_DID_RANGE_ENABLED)
        case DCM_RANGE_DID:
        {
            result = Dcm_UDS0x22_RangeDidHandle(OpStatus, connectionId, did, didIndex, DestBuffer, bufSize, ErrorCode);
            break;
        }
#endif
#if (STD_ON == DCM_OBD_DID_MIRROR)
        case DCM_OBD_DID:
        {
            result = Dcm_UDS0x22_OBDDidHandle(OpStatus, did, DestBuffer, bufSize, ErrorCode);
            break;
        }
#endif
        default:
        {
            /* idle */
            break;
        }
        }
    }

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
DCM_LOCAL Std_ReturnType Dcm_UDS0x22_ReadDidPostProcess(
    uint8                         protocolId,
    Std_ReturnType                readResult,
    uint16                        index,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    uint16                did;
    Std_ReturnType        result          = readResult;
    Dcm_MsgLenType*       resDataLen      = &pMsgContext->resDataLen;
    Dcm_ProtocolCtrlType* protocolCtrlPtr = &Dcm_ProtocolCtrl[protocolId];
    Dcm_MsgLenType        totalSize       = *resDataLen;
    for (uint16 remainIndex = index; remainIndex < (pMsgContext->reqDataLen / DCM_DID_LENGTH); remainIndex++)
    {
        did                       = DCM_U8N_TO_U16(&pMsgContext->reqData[remainIndex * DCM_DID_LENGTH]);
        Dcm_MsgLenType dataLength = 0u;
        result = Dcm_UDS0x22_GetDidLength(did, protocolCtrlPtr->ConnectionId, &dataLength, ErrorCode);
        if (Dcm_ReadCheckStatus[remainIndex] != 1u)
        {
            totalSize += dataLength + DCM_DID_LENGTH;
            if ((dataLength + DCM_DID_LENGTH) > pMsgContext->resMaxDataLen)
            {
                result = E_NOT_OK;
                break;
            }
        }
    }
    if ((E_OK == result) && (totalSize < Dcm_DslProtocolRow[protocolId].MaximumResponseSize))
    {
        if (FALSE == protocolCtrlPtr->PagedBufferStarted)
        {
            protocolCtrlPtr->PagedBufferStarted = TRUE;
            protocolCtrlPtr->FirstPageLength    = *resDataLen + 1u;
            protocolCtrlPtr->RemainPageLength   = *resDataLen + 1u;
            *resDataLen                         = totalSize;
        }

        *ErrorCode = DCM_POS_RESP;
    }

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * only for pagedBuffer, retrive did total dataLength without reading
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x22_GetDidLength(
    uint16                        Did,
    uint16                        ConnectionId,
    Dcm_MsgLenType*               DataLength,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    uint16      didIndex;
    Dcm_DidType didType;

    Std_ReturnType result = Dcm_UDS_FindDid(Did, &didType, &didIndex, NULL_PTR);
    if (E_OK == result)
    {
        if (DCM_REGULAR_DID == didType)
        {
            result = Dcm_UDS_CheckReadWriteDid(ConnectionId, Did, DCM_DID_INFO(didIndex).DidRead, ErrorCode);
        }

        Dcm_MsgLenType bufSize = DCM_INVALID_UINT32;
        if (E_OK == result)
        {
            switch (didType)
            {
            case DCM_REGULAR_DID:
            {
#if (STD_ON == DCM_DYN_DID)
                if ((Did >= DCM_UDS_DYNDID_RANGE_LOWER) && (Did <= DCM_UDS_DYNDID_RANGE_UPPER)
                    && (TRUE == DCM_DID_INFO(didIndex).DynamicallyDefined))
                {
                    result = Dcm_UDS_DynDidHandle(
                        DCM_INITIAL,
                        ConnectionId,
                        DCM_DID_INFO(didIndex).DDDidIndex,
                        NULL_PTR,
                        &bufSize,
#if (STD_ON == DCM_DDDID_CHECK_SOURCE)
                        NULL_PTR,
#endif
                        ErrorCode);
                }
                else
#endif
                {
                    result = Dcm_UDS_DidReadHandle(DCM_INITIAL, didIndex, NULL_PTR, &bufSize, ErrorCode);
                }
                break;
            }
#if (STD_ON == DCM_DID_RANGE_ENABLED)
            case DCM_RANGE_DID:
            {
                result =
                    Dcm_UDS0x22_RangeDidHandle(DCM_INITIAL, ConnectionId, Did, didIndex, NULL_PTR, &bufSize, ErrorCode);
                break;
            }
#endif
#if (STD_ON == DCM_OBD_DID_MIRROR)
            case DCM_OBD_DID:
            {
                result = Dcm_UDS0x22_OBDDidHandle(DCM_INITIAL, Did, NULL_PTR, &bufSize, ErrorCode);
                break;
            }
#endif
            default:
            {
                /* idle */
                break;
            }
            }
        }
        if (E_OK == result)
        {
            *DataLength = bufSize;
        }
    }
    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
#if (STD_ON == DCM_DID_RANGE_ENABLED)
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * deal with rangeDid reading
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x22_RangeDidHandle(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint16                        ConnectionId,
    uint16                        Did,
    uint16                        RangeDidIndex,
    uint8*                        DestBuffer,
    Dcm_MsgLenType*               BufSize,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result =
        Dcm_UDS_CheckReadWriteDid(ConnectionId, Did, DCM_RANGEDID_INFO(RangeDidIndex).DidRead, ErrorCode);

    if (*ErrorCode == DCM_E_REQUESTOUTOFRANGE)
    {
        *ErrorCode = DCM_POS_RESP;
    }

    if (E_OK == result)
    {
        Dcm_DidSupportedType       supported         = DCM_DID_SUPPORTED;
        const Dcm_DspDidRangeType* DcmDspDidRangeCfg = &Dcm_DspDidRange[RangeDidIndex];
        if (NULL_PTR != DcmDspDidRangeCfg->IsDidAvailableFnc)
        {
            result = DcmDspDidRangeCfg->IsDidAvailableFnc(Did, OpStatus, &supported);
        }
        if ((E_OK == result) && ((Dcm_DidSupportedType)DCM_DID_SUPPORTED == supported))
        {
            result = Dcm_UDS0x22_RangeDidReadData(OpStatus, Did, DcmDspDidRangeCfg, DestBuffer, BufSize, ErrorCode);
        }
#if (STD_ON == DCM_DEV_ERROR_DETECT)
        else if (DCM_E_PENDING != result)
        {
            DCM_DET_REPORT(DCM_MAIN_FUNCTION_ID, DCM_E_INTERFACE_RETURN_VALUE);
            *ErrorCode = DCM_E_GENERALREJECT;
            result     = E_NOT_OK;
        }
#endif
        else
        {
            result = E_NOT_OK;
        }
    }

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
DCM_LOCAL Std_ReturnType Dcm_UDS0x22_RangeDidReadData(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint16                        Did,
    const Dcm_DspDidRangeType*    DcmDspDidRangeCfg,
    uint8*                        DestBuffer,
    Dcm_MsgLenType*               BufSize,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    uint16         DataLength;
    Std_ReturnType result = E_NOT_OK;

    if (NULL_PTR != DcmDspDidRangeCfg->DidRangeReadDataLengthFnc)
    {
        result = DcmDspDidRangeCfg->DidRangeReadDataLengthFnc(Did, OpStatus, &DataLength);
        if (E_OK == result)
        {
            if (DataLength > DcmDspDidRangeCfg->DidRangeMaxDataLength)
            {
                result = E_NOT_OK;
            }
            else if (DataLength > *BufSize)
            {
                *ErrorCode = DCM_E_RESPONSETOOLONG;
                result     = E_NOT_OK;
            }
            else
            {
                /* idle */
            }
        }
#if (STD_ON == DCM_DEV_ERROR_DETECT)
        else if ((E_NOT_OK != result) && (DCM_E_PENDING != result))
        {
            DCM_DET_REPORT(DCM_MAIN_FUNCTION_ID, DCM_E_INTERFACE_RETURN_VALUE);
            *ErrorCode = DCM_E_GENERALREJECT;
            result     = E_NOT_OK;
        }
        else
        {
            /* idle */
        }
#endif
    }

    if (E_OK == result)
    {
        if ((NULL_PTR != DcmDspDidRangeCfg->DidRangeReadDidFnc) && (NULL_PTR != DestBuffer))
        {
            result = DcmDspDidRangeCfg->DidRangeReadDidFnc(Did, DestBuffer, OpStatus, DataLength, ErrorCode);
            if (E_OK == result)
            {
                *BufSize = DataLength;
            }
#if (STD_ON == DCM_DEV_ERROR_DETECT)
            else if ((E_NOT_OK != result) && (DCM_E_PENDING != result))
            {
                DCM_DET_REPORT(DCM_MAIN_FUNCTION_ID, DCM_E_INTERFACE_RETURN_VALUE);
                *ErrorCode = DCM_E_GENERALREJECT;
                result     = E_NOT_OK;
            }
            else
            {
                /* idle */
            }
#endif
        }
        else
        {
            result = E_NOT_OK;
        }
    }

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
#if (STD_ON == DCM_OBD_DID_MIRROR)
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * deals with obd did to get data, treat as different type for different range
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x22_OBDDidHandle(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint16                        Did,
    uint8*                        DestBuffer,
    Dcm_MsgLenType*               BufSize,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_NOT_OK;

    if ((Did >= DCM_PIDDID_RANGE_LOWER) && (Did <= DCM_PIDDID_RANGE_UPPER))
    {
        result = Dcm_OBD0x01_ReadPid(OpStatus, (uint8)Did, DestBuffer, BufSize, NULL_PTR);
    }
    else if ((Did >= DCM_MIDDID_RANGE_LOWER) && (Did <= DCM_MIDDID_RANGE_UPPER))
    {
        result = Dcm_OBD0x06_ReadMid((uint8)Did, DestBuffer, BufSize, NULL_PTR);
    }
    else if ((Did >= DCM_VEHINFODID_RANGE_LOWER) && (Did <= DCM_VEHINFODID_RANGE_UPPER))
    {
        result = Dcm_OBD0x09_ReadVehInfo(OpStatus, (uint8)Did, DestBuffer, BufSize, NULL_PTR, NULL_PTR);
    }
    else
    {
        result = E_NOT_OK;
    }

    if ((0u == *BufSize) && (result == E_NOT_OK))
    {
        *ErrorCode = DCM_E_RESPONSETOOLONG;
    }

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
#endif
