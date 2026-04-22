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
 **  @file               : Dcm_UDS0x86.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm UDS service 0x86 function implementation
 **
 ***********************************************************************************************************************/
/* PRQA S 0553 EOF */ /* VL_Dcm_0553 */
/* =================================================== inclusions =================================================== */
#include "Dcm_Cfg.h"
#if (STD_ON == DCM_UDS_0X86)
#include "Dcm_Internal.h"
/* ===================================================== macros ===================================================== */
/**
 * @brief The minimum request length of UDS 0x86
 */
#define DCM_UDS86_MINREQLEN 2u
#if (                                                                                            \
    (STD_ON == DCM_UDS_0X86_0X0) || (STD_ON == DCM_UDS_0X86_0X5) || (STD_ON == DCM_UDS_0X86_0X6) \
    || (STD_ON == DCM_UDS_0X86_0X4))
/**
 * @brief subfunction stopROE
 */
#define DCM_UDS86_STOPROE 0u
/**
 * @brief subfunction startROE
 */
#define DCM_UDS86_STARTROE 5u
/**
 * @brief subfunction clearROE
 */
#define DCM_UDS86_CLEARROE 6u
/**
 * @brief subfunction reportActivatedEvents
 */
#define DCM_UDS86_REPORTACTIVATEDEVENTS 4u
#endif
#ifdef DCM_UDS_0X86_0X1
/**
 * @brief subfunction responseOnDTCStatusChange
 */
#define DCM_UDS86_ONDTC 1u
#endif
#ifdef DCM_UDS_0X86_0X3
/**
 * @brief subfunction responseOnDidChange
 */
#define DCM_UDS86_ONDID 3u
#endif
#ifdef DCM_UDS_0X86_0X8
/**
 * @brief subfunction reportMostRecentDTC
 */
#define DCM_UDS86_REPORTMOSTRECENTDTC 8u
#endif
/**
 * @brief bitmask for storage bit
 */
#define DCM_UDS86_STORAGE_BIT_MASK 0x40u
/**
 * @brief service id of UDS service Read DTC Information
 */
#define DCM_UDS_READDTCINFO_SID 0x19u
/* ============================================ internal data definitions =========================================== */
#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
/**
 * @brief total requested number of trigger on DTC roe @range 0..255
 */
DCM_LOCAL uint8 Dcm_RoeCtrlDtcNum;
/**
 * @brief total requested number of trigger on Did roe @range 0..255
 */
DCM_LOCAL uint8 Dcm_RoeCtrlDidNum;
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_32
#include "Dcm_MemMap.h"
/**
 * @brief scheduler rate timer @range 0..0xFFFFFFFF
 */
DCM_LOCAL uint32 Dcm_RoeSchedulerRateTimer; /* PRQA S 3218 */ /* VL_Dcm_3218 */
#define DCM_STOP_SEC_VAR_CLEARED_32
#include "Dcm_MemMap.h"
/* ============================================ external data definitions =========================================== */
#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
/**
 * @brief total requested number roe @range 0..255
 */
uint8 Dcm_RoeCtrlNum;
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
/**
 * @brief roe control unit @range 0..255
 */
Dcm_RoeCtrlType Dcm_RoeCtrl[DCM_MAX_ROE_DID + DCM_MAX_ROE_DTC];
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
/* ========================================== internal function declarations ======================================== */
#if ((defined(DCM_UDS_0X86_0X1)) || ((defined(DCM_UDS_0X86_0X3)) && (DCM_DID_NUM > 0)) || (defined(DCM_UDS_0X86_0X8)))
/**
 * @brief         called to set the roeEvent to Dcm_RoeCtrl
 * @param[in]     protocolType    : current protocolType
 * @param[in]     roeEvent        : the input roeEvent
 * @param[in]     eventWindowTime : the input event window time
 * @param[out]    ErrorCode       : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                                  shall send a negative response with NRC code equal to the parameter ErrorCode
 *                                  parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x86_SetEvent(
    Dcm_ProtocolType              protocolType,
    Dcm_RoeEventType              roeEvent,
    Dcm_RoeEventWindowTimeType    eventWindowTime,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

/**
 * @brief         called by Dcm_UDS0x86_CheckTimer to act as the roe scheduler
 * @retval        void
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL void Dcm_UDS0x86_MainFunction(void);

/**
 * @brief         general condition check for UDS 0x86
 * @param[inout]  pMsgContext     : Message-related information for one diagnostic protocol identifier. The pointers in
 *                                  pMsgContext shall point behind the SID.
 * @param[out]    eventWindowTime : the requested event window time
 * @param[out]    ErrorCode       : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                                  shall send a negative response with NRC code equal to the parameter ErrorCode
 *                                  parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x86_ConditionCheck(
    Dcm_MsgContextType*           pMsgContext,
    Dcm_RoeEventWindowTimeType*   eventWindowTime,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         general checks for subfunction
 * @param[in]     reqDataLen  : the requested data length
 * @param[in]     subFunction : the requested subFunction
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode
 *                              parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType
    Dcm_UDS0x86_SubFunctionCheck(Dcm_MsgLenType reqDataLen, uint8 subFunction, Dcm_NegativeResponseCodeType* ErrorCode);

#ifdef DCM_UDS_0X86_0X1
/**
 * @brief         service interpreter for subFunction 0x1
 * @param[in]     protocolId      : the current protocolId
 * @param[in]     roeCtrlPtr      : the roe control Unit pointer
 * @param[in]     eventWindowTime : the requested eventWindowTime
 * @param[inout]  pMsgContext     : Message-related information for one diagnostic protocol identifier. The pointers in
 *                                  pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode       : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                                  shall send a negative response with NRC code equal to the parameter ErrorCode
 *                                  parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x86_01(
    uint8                         protocolId,
    Dcm_RoeCtrlType*              roeCtrlPtr,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_RoeEventWindowTimeType    eventWindowTime,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#ifdef DCM_UDS_0X86_0X3
/**
 * @brief         service interpreter for subFunction 0x3
 * @param[in]     protocolId      : the current protocolId
 * @param[in]     roeCtrlPtr      : the roe control Unit pointer
 * @param[in]     eventWindowTime : the requested eventWindowTime
 * @param[inout]  pMsgContext     : Message-related information for one diagnostic protocol identifier. The pointers in
 *                                  pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode       : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                                  shall send a negative response with NRC code equal to the parameter ErrorCode
 *                                  parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x86_03(
    uint8                         protocolId,
    Dcm_RoeCtrlType*              roeCtrlPtr,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_RoeEventWindowTimeType    eventWindowTime,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#ifdef DCM_UDS_0X86_0X4
/**
 * @brief         service interpreter for subFunction 0x4
 * @param[inout]  pMsgContext     : Message-related information for one diagnostic protocol identifier. The pointers in
 *                                  pMsgContext shall point behind the SID.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL void Dcm_UDS0x86_04(Dcm_MsgContextType* pMsgContext);
#endif

#ifdef DCM_UDS_0X86_0X6
/**
 * @brief         service interpreter for subFunction 0x6
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL void Dcm_UDS0x86_06(void);
#endif

#ifdef DCM_UDS_0X86_0X8
/**
 * @brief         service interpreter for subFunction 0x8
 * @param[in]     protocolId      : the current protocolId
 * @param[in]     roeCtrlPtr      : the roe control Unit pointer
 * @param[in]     eventWindowTime : the requested eventWindowTime
 * @param[inout]  pMsgContext     : Message-related information for one diagnostic protocol identifier. The pointers in
 *                                  pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode       : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                                  shall send a negative response with NRC code equal to the parameter ErrorCode
 *                                  parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x86_08(
    uint8                         protocolId,
    Dcm_RoeCtrlType*              roeCtrlPtr,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_RoeEventWindowTimeType    eventWindowTime,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

/**
 * @brief         store roe info to NvM
 * @param[in]     subFunction     : the requested subFunction
 * @param[in]     eventWindowTime : the requested eventWindowTime
 * @param[inout]  pMsgContext     : Message-related information for one diagnostic protocol identifier. The pointers in
 *                                  pMsgContext shall point behind the SID.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL void
    Dcm_UDS0x86_Store(uint8 subFunction, Dcm_RoeEventWindowTimeType eventWindowTime, Dcm_MsgContextType* pMsgContext);

/**
 * @brief         handle different subFunctions
 * @param[in]     protocolId      : the current protocolId
 * @param[in]     roeCtrlPtr      : the roe control Unit pointer
 * @param[in]     eventWindowTime : the requested eventWindowTime
 * @param[inout]  pMsgContext     : Message-related information for one diagnostic protocol identifier. The pointers in
 *                                  pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode       : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                                  shall send a negative response with NRC code equal to the parameter ErrorCode
 *                                  parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x86_HandleSubFunction(
    uint8                         protocolId,
    Dcm_RoeCtrlType*              roeCtrlPtr,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_RoeEventWindowTimeType    eventWindowTime,
    Dcm_NegativeResponseCodeType* ErrorCode);

#if ((defined DCM_UDS_0X86_0X5) || (defined DCM_UDS_0X86_0X0))
/**
 * @brief         set all roeStatus
 * @param[in]     RoeStatus : the current roeStatus
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL void Dcm_UDS0x86_SetRoeStatus(Dcm_RoeStatusType RoeStatus);
#endif

#if ((defined(DCM_UDS_0X86_0X1)) || ((defined(DCM_UDS_0X86_0X3)) && (DCM_DID_NUM > 0)) || (defined(DCM_UDS_0X86_0X8)))
/**
 * @brief         get roe PduId and protocolId based on requested protocolType
 * @param[in]     protocolType : the requested protocolType
 * @param[out]    roePduId     : the matched roePduId
 * @param[out]    protocolId   : the matched roe protocolId
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL void
    Dcm_UDS0x86_getRoePduIdAndProtocolId(Dcm_ProtocolType protocolType, PduIdType* roePduId, uint8* protocolId);

/**
 * @brief         check Service and subService for requested response service
 * @param[in]     protocolType : the requested protocolType
 * @param[in]     roeCtrl      : the matched roeCtrl unit
 * @param[in]     reqData      : the requested service data
 * @param[in]     reqDataLen   : the requested service data len
 * @param[out]    ErrorCode    : the nrc to be sent
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x86_CheckService(
    Dcm_ProtocolType              protocolType,
    Dcm_RoeCtrlType*              roeCtrl,
    uint8*                        reqData,
    Dcm_MsgLenType                reqDataLen,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#if ((STD_ON == DCM_UDS_0X22) && (STD_ON == DCM_UDS_0X86_0X3))
/**
 * @brief            Check if the DID has changed.
 * @param[in]        did : data identifier
 * @param[inout]     change      : Has the DID changed?
 * @param[inout]     oldDidBuffer      : The DID data queried last time
 * @return           Std_ReturnType
 * @retval           E_OK     : Request was successful
 * @retval           E_NOT_OK : Request was not successful
 * @reentrant        TRUE
 * @synchronous      TRUE
 * @trace            CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_UDS0x86_DIDChangeCheck(uint16 did, boolean* change, uint8* oldDidBuffer, uint16 conId);

#if (STD_ON == DCM_DID_RANGE_ENABLED)
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
DCM_LOCAL Std_ReturnType Dcm_UDS0x86_RangeDidReadData(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint16                        Did,
    const Dcm_DspDidRangeType*    DcmDspDidRangeCfg,
    uint8*                        DestBuffer,
    Dcm_MsgLenType*               BufSize,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#if (STD_ON == DCM_DID_RANGE_ENABLED)
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
DCM_LOCAL Std_ReturnType Dcm_UDS0x86_RangeDidHandle(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint16                        ConnectionId,
    uint16                        Did,
    uint16                        RangeDidIndex,
    uint8*                        DestBuffer,
    Dcm_MsgLenType*               BufSize,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#if (STD_ON == DCM_OBD_DID_MIRROR)
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

DCM_LOCAL Std_ReturnType Dcm_UDS0x86_OBDDidHandle(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint16                        Did,
    uint8*                        DestBuffer,
    Dcm_MsgLenType*               BufSize,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif
/* ========================================== external function definitions ========================================= */

#if (STD_ON == DCM_OBD_DID_MIRROR)
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"

DCM_LOCAL Std_ReturnType Dcm_UDS0x86_OBDDidHandle(
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

    if (0u == *BufSize)
    {
        *ErrorCode = DCM_E_RESPONSETOOLONG;
        result     = E_NOT_OK;
    }
    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
#if (STD_ON == DCM_DID_RANGE_ENABLED)
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
DCM_LOCAL Std_ReturnType Dcm_UDS0x86_RangeDidHandle(
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
            result = Dcm_UDS0x86_RangeDidReadData(OpStatus, Did, DcmDspDidRangeCfg, DestBuffer, BufSize, ErrorCode);
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
#endif

#if (STD_ON == DCM_DID_RANGE_ENABLED)
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
DCM_LOCAL Std_ReturnType Dcm_UDS0x86_RangeDidReadData(
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

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType Dcm_UDS0x86_DIDChangeCheck(uint16 did, boolean* change, uint8* oldDidBuffer, uint16 conId)
{
    uint16                       didIndex;
    Dcm_DidType                  didType;
    Dcm_ExtendedOpStatusType     OpStatus  = DCM_INITIAL;
    Std_ReturnType               result    = Dcm_UDS_FindDid(did, &didType, &didIndex, NULL_PTR);
    Dcm_NegativeResponseCodeType ErrorCode = DCM_POS_RESP;
    uint8                        DestBuffer[DCM_ROE_MAX_DID_LENGTH];
    Dcm_MsgLenType               bufSize = DCM_ROE_MAX_DID_LENGTH;

    if (DCM_REGULAR_DID == didType)
    {
        result = Dcm_UDS_CheckReadWriteDid(conId, did, DCM_DID_INFO(didIndex).DidRead, &ErrorCode);
    }
    if (E_OK == result)
    {
        switch (didType)
        {
        case DCM_REGULAR_DID:
        {
#if (STD_ON == DCM_DYN_DID)
            if ((did >= DCM_UDS_DYNDID_RANGE_LOWER) && (did <= DCM_UDS_DYNDID_RANGE_UPPER)
                && (TRUE == DCM_DID_INFO(didIndex).DynamicallyDefined))
            {
                result = E_NOT_OK; // Dynamic DID detection is not supported.
            }
            else
#endif
            {
                result = Dcm_UDS_DidReadHandle(OpStatus, didIndex, DestBuffer, &bufSize, &ErrorCode);
            }
            break;
        }
#if (STD_ON == DCM_DID_RANGE_ENABLED)
        case DCM_RANGE_DID:
        {
            result = Dcm_UDS0x86_RangeDidHandle(OpStatus, conId, did, didIndex, DestBuffer, &bufSize, &ErrorCode);
            break;
        }
#endif
#if (STD_ON == DCM_OBD_DID_MIRROR)
        case DCM_OBD_DID:
        {
            result = Dcm_UDS0x86_OBDDidHandle(OpStatus, did, DestBuffer, &bufSize, &ErrorCode);
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

    if (E_OK == result && oldDidBuffer != NULL_PTR)
    {
        if (DcmInternal_MemCmp(DestBuffer, oldDidBuffer, bufSize) == 0u)
        {
            result = E_NOT_OK;
        }
        else
        {
            DcmInternal_Memcpy(oldDidBuffer, DestBuffer, bufSize);
            result = E_OK;
            if (change != NULL_PTR)
            {
                *change = TRUE;
            }
        }
    }

    return result;
}

#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#endif

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * The service interpreter for UDS 0x86
 */
Std_ReturnType Dcm_UDS0x86(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    const uint8*               reqData = pMsgContext->reqData;
    uint8                      protocolId;
    Dcm_RoeEventWindowTimeType eventWindowTime;
    DCM_UNUSED(OpStatus);

    (void)DslInternal_FindProtocolRowByRxPduId(pMsgContext->dcmRxPduId, &protocolId, NULL_PTR);

    Std_ReturnType result = Dcm_UDS0x86_ConditionCheck(pMsgContext, &eventWindowTime, ErrorCode);

    uint8*          resData     = pMsgContext->resData;
    Dcm_MsgLenType* resDataLen  = &pMsgContext->resDataLen;
    uint8           subFunction = reqData[0u];
    if (E_OK == result)
    {
        Dcm_RoeCtrlType* roeCtrlPtr = &Dcm_RoeCtrl[Dcm_RoeCtrlNum];
        result = Dcm_UDS0x86_HandleSubFunction(protocolId, roeCtrlPtr, pMsgContext, eventWindowTime, ErrorCode);
        Dcm_UDS0x86_Store(subFunction, eventWindowTime, pMsgContext);
    }

    if (E_OK == result)
    {
        resData[0u] = subFunction;
        (*resDataLen)++;
    }

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

/**
 * called when DTC Status change or did written to trigger established service request
 */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
void Dcm_UDS0x86_TriggerServiceRequest(Dcm_RoeCtrlType* roeCtrlPtr)
{
    uint16 tempConnectionId = Dcm_DslProtocolRx[roeCtrlPtr->RoePduId].ParentMainConnectionId;
    if (DCM_MSG_IDLE == Dcm_ProtocolCtrl[Dcm_DslMainConnection[tempConnectionId].ParentProtocolId].State)
    {
        PduInfoType   pduInfo;
        PduLengthType bufferSize;
        pduInfo.SduDataPtr            = roeCtrlPtr->RoeServiceResponseRecord;
        PduLengthType TpSduLength     = roeCtrlPtr->RoeServiceResponseRecordSize;
        pduInfo.SduLength             = TpSduLength;
        roeCtrlPtr->PendingServiceReq = FALSE;
        BufReq_ReturnType result      = Dcm_StartOfReception(roeCtrlPtr->RoePduId, &pduInfo, TpSduLength, &bufferSize);
        if (BUFREQ_OK == result)
        {
            Dcm_TpRxIndication(roeCtrlPtr->RoePduId, E_OK);
        }
        else if (BUFREQ_E_NOT_OK == result)
        {
            roeCtrlPtr->PendingServiceReq = TRUE;
        }
        else
        {
            /* idle */
        }
    }
    else
    {
        roeCtrlPtr->PendingServiceReq = TRUE;
    }
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * called by Dcm_Init to initialize roe status, only apply for nvm stored roe data
 */
void Dcm_UDS0x86_Init(void)
{
    Dcm_RoeCtrlNum    = 0u;
    Dcm_RoeCtrlDidNum = 0u;
    Dcm_RoeCtrlDtcNum = 0u;
    for (uint16 index = 0u; index < (DCM_MAX_ROE_DID + DCM_MAX_ROE_DTC); index++)
    {
        Dcm_RoeCtrl[index].PendingServiceReq = FALSE;
#ifdef DCM_ROE_STORAGE_NVMBLOCK
        boolean overMax = FALSE;
        if ((DCM_ROE_WINDOWTIME_INFINITE == Dcm_RoeCtrl[index].RoeEventWindowTime))
        {
            Dcm_RoeCtrlNum++;
            if (DCM_ROE_ONDID == Dcm_RoeCtrl[index].RoeEvent)
            {
                if (Dcm_RoeCtrlDidNum < DCM_MAX_ROE_DID)
                {
                    Dcm_RoeCtrlDidNum++;
                }
                else
                {
                    overMax = TRUE;
                }
            }
            else
            {
                if (Dcm_RoeCtrlDtcNum < DCM_MAX_ROE_DTC)
                {
                    Dcm_RoeCtrlDtcNum++;
                }
                else
                {
                    overMax = TRUE;
                }
            }
            if (TRUE == overMax)
            {
                Dcm_RoeCtrl[index].RoeEvent  = DCM_ROE_UNDEFINED;
                Dcm_RoeCtrl[index].RoeStatus = DCM_ROE_OFF;
            }
            else if ((index + 1u) != Dcm_RoeCtrlNum)
            {
                DcmInternal_Memcpy(
                    (uint8*)&Dcm_RoeCtrl[Dcm_RoeCtrlNum],
                    (uint8*)&Dcm_RoeCtrl[index],
                    sizeof(Dcm_RoeCtrlType));
            }
            else
            {
                /* idle */
            }
        }
#endif
    }
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * called by Dcm_MainFunction to check scheduler rate
 */
void Dcm_UDS0x86_CheckTimer(void)
{
    Dcm_RoeSchedulerRateTimer = DCM_ROE_SCHEDULER_RATE;
    if (TRUE == DcmInternal_DecreaseTimer(&Dcm_RoeSchedulerRateTimer))
    {
        Dcm_RoeSchedulerRateTimer = DCM_ROE_SCHEDULER_RATE;
        Dcm_UDS0x86_MainFunction();
    }
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/* ========================================== internal function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
#if ((defined(DCM_UDS_0X86_0X1)) || ((defined(DCM_UDS_0X86_0X3)) && (DCM_DID_NUM > 0)) || (defined(DCM_UDS_0X86_0X8)))
/**
 * called to set the roeEvent to Dcm_RoeCtrl
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x86_SetEvent(
    Dcm_ProtocolType              protocolType,
    Dcm_RoeEventType              roeEvent,
    Dcm_RoeEventWindowTimeType    eventWindowTime,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_OK;

    switch (roeEvent)
    {
    case DCM_ROE_ONDID:
    {
        if (Dcm_RoeCtrlDidNum >= DCM_MAX_ROE_DID)
        {
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            result     = E_NOT_OK;
        }
        else
        {
            Dcm_RoeCtrlDidNum++;
        }
        break;
    }
    case DCM_ROE_ONDTC:
    case DCM_ROE_MOSTRECENTDTC:
    case DCM_ROE_DTCRECORD:
    {
        if (Dcm_RoeCtrlDtcNum >= DCM_MAX_ROE_DTC)
        {
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            result     = E_NOT_OK;
        }
        else
        {
            Dcm_RoeCtrlDtcNum++;
        }
        break;
    }
    default:
    {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        result     = E_NOT_OK;
        break;
    }
    }

    if (E_OK == result)
    {
        Dcm_RoeCtrlType* roeCtrlPtr    = &Dcm_RoeCtrl[Dcm_RoeCtrlNum];
        roeCtrlPtr->RoeEvent           = roeEvent;
        roeCtrlPtr->RoeEventWindowTime = eventWindowTime;
        Dcm_UDS0x86_getRoePduIdAndProtocolId(protocolType, &roeCtrlPtr->RoePduId, NULL_PTR);
        Dcm_RoeCtrlNum++;
    }

    return result;
}

DCM_LOCAL void
    Dcm_UDS0x86_getRoePduIdAndProtocolId(Dcm_ProtocolType protocolType, PduIdType* roePduId, uint8* protocolId)
{
    switch (protocolType)
    {
#ifdef DCM_ROE_CAN_RXPDUID
    case DCM_UDS_ON_CAN:
    {
        *roePduId = DCM_ROE_CAN_RXPDUID;
        break;
    }
#endif
#ifdef DCM_ROE_IP_RXPDUID
    case DCM_UDS_ON_IP:
    {
        *roePduId = DCM_ROE_IP_RXPDUID;
        break;
    }
#endif
#ifdef DCM_ROE_FLEXRAY_RXPDUID
    case DCM_UDS_ON_FLEXRAY:
    {
        *roePduId = DCM_ROE_FLEXRAY_RXPDUID;
        break;
    }
#endif
    default:
    {
        /* idle */
        break;
    }
    }

    if (NULL_PTR != protocolId)
    {
        *protocolId = Dcm_DslMainConnection[Dcm_DslProtocolRx[*roePduId].ParentMainConnectionId].ParentProtocolId;
    }
}
#endif

/**
 * periodically check for pending roe service request
 */
DCM_LOCAL void Dcm_UDS0x86_MainFunction(void)
{
    uint16           index;
    Dcm_RoeCtrlType* roeCtrlPtr;

    for (index = 0u; index < (DCM_MAX_ROE_DID + DCM_MAX_ROE_DTC); index++)
    {
        roeCtrlPtr = &Dcm_RoeCtrl[index];
#ifdef DCM_UDS_0X86_0X3
        if ((roeCtrlPtr->RoeEvent == DCM_ROE_ONDID) && (DCM_ROE_ON == roeCtrlPtr->RoeStatus))
        {
            boolean        didChange = FALSE;
            uint16         conId     = Dcm_DslProtocolRx[roeCtrlPtr->RoePduId].ParentMainConnectionId;
            Std_ReturnType result =
                Dcm_UDS0x86_DIDChangeCheck(roeCtrlPtr->Did, &didChange, roeCtrlPtr->DidSignalData, conId);
            if (didChange == TRUE && result == E_OK)
            {
                roeCtrlPtr->PendingServiceReq = TRUE;
            }
        }
#endif
        if ((TRUE == roeCtrlPtr->PendingServiceReq) && (DCM_ROE_UNDEFINED != roeCtrlPtr->RoeEvent)
            && (DCM_ROE_ON == roeCtrlPtr->RoeStatus))
        {
            Dcm_UDS0x86_TriggerServiceRequest(roeCtrlPtr);
        }
    }
}

DCM_LOCAL Std_ReturnType Dcm_UDS0x86_ConditionCheck(
    Dcm_MsgContextType*           pMsgContext,
    Dcm_RoeEventWindowTimeType*   eventWindowTime,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result     = E_OK;
    const uint8*   reqData    = pMsgContext->reqData;
    Dcm_MsgLenType reqDataLen = pMsgContext->reqDataLen;

    if (reqDataLen < DCM_UDS86_MINREQLEN)
    {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        result     = E_NOT_OK;
    }

    if (E_OK == result)
    {
        if ((uint8)DCM_ROE_WINDOWTIME_INFINITE == reqData[1u])
        {
            *eventWindowTime = DCM_ROE_WINDOWTIME_INFINITE;
        }
        else if ((uint8)DCM_ROE_WINDOWTIME_POWER == reqData[1u])
        {
            *eventWindowTime = DCM_ROE_WINDOWTIME_POWER;
        }
        else
        {
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            result     = E_NOT_OK;
        }
    }

    uint8 storageState = (reqData[0u] & DCM_UDS86_STORAGE_BIT_MASK) >> 6u;
    uint8 subFunction  = reqData[0u] & DCM_SUBFUNCTION_MASK;
    if (((E_OK == result) && (1u == storageState))
#if (STD_ON == DCM_ROE_STORE_EVENT_SUPPORT)
        && ((DCM_ROE_WINDOWTIME_POWER == *eventWindowTime)
#ifdef DCM_UDS_0X86_0X5
            || (DCM_UDS86_STARTROE != subFunction)
#endif
            || (DCM_DEFAULT_SESSION != Dcm_SessionCtrl.CurrentSession))
#endif
    )
    {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        result     = E_NOT_OK;
    }

    if (E_OK == result)
    {
        result = Dcm_UDS0x86_SubFunctionCheck(reqDataLen, subFunction, ErrorCode);
    }

#ifdef DCM_UDS_0X86_0X8
    if ((E_OK == result) && (DCM_UDS86_REPORTMOSTRECENTDTC == subFunction) && (DCM_UDS19_REPOMTFDTC != reqData[2u]))
    {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        result     = E_NOT_OK;
    }
#endif

    return result;
}

DCM_LOCAL Std_ReturnType
    Dcm_UDS0x86_SubFunctionCheck(Dcm_MsgLenType reqDataLen, uint8 subFunction, Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result;

    switch (subFunction)
    {
#if (                                                                                            \
    (STD_ON == DCM_UDS_0X86_0X0) || (STD_ON == DCM_UDS_0X86_0X5) || (STD_ON == DCM_UDS_0X86_0X6) \
    || (STD_ON == DCM_UDS_0X86_0X4))
    case DCM_UDS86_STOPROE:
    case DCM_UDS86_STARTROE:
    case DCM_UDS86_CLEARROE:
    case DCM_UDS86_REPORTACTIVATEDEVENTS:
    {
        result = (reqDataLen != DCM_UDS86_MINREQLEN) ? E_NOT_OK : E_OK;
        break;
    }
#endif
#ifdef DCM_UDS_0X86_0X8
    case DCM_UDS86_REPORTMOSTRECENTDTC:
    {
        result = ((DCM_UDS86_MINREQLEN + 1u) != reqDataLen) ? E_NOT_OK : E_OK;
        break;
    }
#endif
#ifdef DCM_UDS_0X86_0X3
    case DCM_UDS86_ONDID:
    {
        result = ((reqDataLen < (DCM_UDS86_MINREQLEN + 3u))
                  || (reqDataLen > (DCM_UDS86_MINREQLEN + 2u + DCM_UDS86_SERVICERESP_MAXSIZE)))
                     ? E_NOT_OK
                     : E_OK;
        break;
    }
#endif
#ifdef DCM_UDS_0X86_0X1
    case DCM_UDS86_ONDTC:
    {
        result = ((reqDataLen < (DCM_UDS86_MINREQLEN + 2u))
                  || (reqDataLen > (DCM_UDS86_MINREQLEN + 1u + DCM_UDS86_SERVICERESP_MAXSIZE)))
                     ? E_NOT_OK
                     : E_OK;
        break;
    }
#endif
    default:
    {
        result = E_OK;
        break;
    }
    }
    if (E_NOT_OK == result)
    {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
    }

    return result;
}

#ifdef DCM_UDS_0X86_0X1
DCM_LOCAL Std_ReturnType Dcm_UDS0x86_01(
    uint8                         protocolId,
    Dcm_RoeCtrlType*              roeCtrlPtr,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_RoeEventWindowTimeType    eventWindowTime,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    uint8*         reqData    = pMsgContext->reqData;
    Dcm_MsgLenType reqDataLen = pMsgContext->reqDataLen;
    uint8*         resData    = pMsgContext->resData;

    roeCtrlPtr->DTCMask   = reqData[2u];
    Std_ReturnType result = Dcm_UDS0x86_CheckService(
        Dcm_DslProtocolRow[protocolId].ProtocolType,
        roeCtrlPtr,
        &reqData[3u],
        reqDataLen - 3u,
        ErrorCode);

    if (E_OK == result)
    {
        result = Dcm_UDS0x86_SetEvent(
            Dcm_DslProtocolRow[protocolId].ProtocolType,
            DCM_ROE_ONDTC,
            eventWindowTime,
            ErrorCode);
    }

    if (E_OK == result)
    {
        resData[3u] = roeCtrlPtr->DTCMask;
        DcmInternal_Memcpy(
            &resData[4u],
            roeCtrlPtr->RoeServiceResponseRecord,
            roeCtrlPtr->RoeServiceResponseRecordSize);
        pMsgContext->resDataLen += (Dcm_MsgLenType)roeCtrlPtr->RoeServiceResponseRecordSize + 1u;
    }

    return result;
}
#endif

#ifdef DCM_UDS_0X86_0X3
DCM_LOCAL Std_ReturnType Dcm_UDS0x86_03(
    uint8                         protocolId,
    Dcm_RoeCtrlType*              roeCtrlPtr,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_RoeEventWindowTimeType    eventWindowTime,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
#if (DCM_DID_NUM > 0)
    uint8*         reqData    = pMsgContext->reqData;
    Dcm_MsgLenType reqDataLen = pMsgContext->reqDataLen;
    uint16         did        = DCM_U8N_TO_U16(&reqData[2u]);
    uint16         didIndex;
    Std_ReturnType result = Dcm_UDS_FindDid(did, NULL_PTR, &didIndex, NULL_PTR);
    if ((E_OK == result)
        && ((did < DCM_UDS_DYNDID_RANGE_LOWER) || (did > DCM_UDS_DYNDID_RANGE_UPPER)
            || (FALSE == DCM_DID_INFO(didIndex).DynamicallyDefined))
        && (Dcm_DspDid[didIndex].DidSize <= DCM_ROE_MAX_DID_LENGTH))
    {
        roeCtrlPtr->Did = did;
        result          = Dcm_UDS0x86_CheckService(
            Dcm_DslProtocolRow[protocolId].ProtocolType,
            roeCtrlPtr,
            &reqData[4u],
            reqDataLen - 4u,
            ErrorCode);
        if (E_OK == result)
        {
            result = Dcm_UDS0x86_SetEvent(
                Dcm_DslProtocolRow[protocolId].ProtocolType,
                DCM_ROE_ONDID,
                eventWindowTime,
                ErrorCode);
        }
    }
    else
    {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        result     = E_NOT_OK;
    }
#else
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
    result     = E_NOT_OK;
#endif
    if (E_OK == result)
    {
        uint8* resData = pMsgContext->resData;
        DcmInternal_TransformArray_u16(&resData[3u], did, DCM_OPAQUE);
        DcmInternal_Memcpy(
            &resData[5u],
            roeCtrlPtr->RoeServiceResponseRecord,
            roeCtrlPtr->RoeServiceResponseRecordSize);
        pMsgContext->resDataLen += (Dcm_MsgLenType)roeCtrlPtr->RoeServiceResponseRecordSize + 2u;
        Dcm_UDS0x86_DIDChangeCheck(did, NULL_PTR, roeCtrlPtr->DidSignalData, protocolId);
    }

    return result;
}
#endif

#ifdef DCM_UDS_0X86_0X8
DCM_LOCAL Std_ReturnType Dcm_UDS0x86_08(
    uint8                         protocolId,
    Dcm_RoeCtrlType*              roeCtrlPtr,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_RoeEventWindowTimeType    eventWindowTime,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    uint8          reqData[2u] = {DCM_UDS_READDTCINFO_SID, DCM_UDS19_REPOMTFDTC};
    Std_ReturnType result =
        Dcm_UDS0x86_CheckService(Dcm_DslProtocolRow[protocolId].ProtocolType, roeCtrlPtr, reqData, 2u, ErrorCode);

    if (E_OK == result)
    {
        result = Dcm_UDS0x86_SetEvent(
            Dcm_DslProtocolRow[protocolId].ProtocolType,
            DCM_ROE_MOSTRECENTDTC,
            eventWindowTime,
            ErrorCode);
    }

    if (E_OK == result)
    {
        DcmInternal_Memcpy(
            &pMsgContext->resData[3u],
            roeCtrlPtr->RoeServiceResponseRecord,
            roeCtrlPtr->RoeServiceResponseRecordSize);
        pMsgContext->resDataLen += roeCtrlPtr->RoeServiceResponseRecordSize;
    }

    return result;
}
#endif

#ifdef DCM_UDS_0X86_0X4
DCM_LOCAL void Dcm_UDS0x86_04(Dcm_MsgContextType* pMsgContext)
{
    uint8* resData = pMsgContext->resData;

    resData[1u] = 0u;
    for (uint8 index = 0u; index < Dcm_RoeCtrlNum; index++)
    {
        const Dcm_RoeCtrlType* roeCtrlPtr = &Dcm_RoeCtrl[index];
        if (DCM_ROE_ON == roeCtrlPtr->RoeStatus)
        {
            resData[1u]++;
            Dcm_RoeEventType roeEvent = roeCtrlPtr->RoeEvent;
            uint16           offset   = 2u;
            resData[offset]           = (uint8)roeEvent;
            resData[offset + 1u]      = (uint8)roeCtrlPtr->RoeEventWindowTime;
            offset += 2u;
            switch (roeEvent)
            {
            case DCM_ROE_ONDTC:
            {
                resData[offset] = roeCtrlPtr->DTCMask;
                offset++;
                break;
            }
            case DCM_ROE_ONDID:
            {
                DcmInternal_TransformArray_u16(&resData[offset], roeCtrlPtr->Did, DCM_OPAQUE);
                offset += 2u;
                break;
            }
            case DCM_ROE_MOSTRECENTDTC:
            {
                resData[offset] = DCM_UDS19_REPOMTFDTC;
                offset++;
                break;
            }
            case DCM_ROE_DTCRECORD:
            {
                resData[offset]      = roeCtrlPtr->DTCMask;
                resData[offset + 1u] = roeCtrlPtr->RoeServiceResponseRecord[1u];
                resData[offset + 2u] = roeCtrlPtr->RoeServiceResponseRecord[2u];
                offset += 3u;
                break;
            }
            default:
            {
                /* idle */
                break;
            }
            }
            DcmInternal_Memcpy(
                &resData[offset],
                roeCtrlPtr->RoeServiceResponseRecord,
                roeCtrlPtr->RoeServiceResponseRecordSize);
            offset += (uint16)roeCtrlPtr->RoeServiceResponseRecordSize;
            pMsgContext->resDataLen = (Dcm_MsgLenType)offset - 1u;
        }
    }
}
#endif

DCM_LOCAL void
    Dcm_UDS0x86_Store(uint8 subFunction, Dcm_RoeEventWindowTimeType eventWindowTime, Dcm_MsgContextType* pMsgContext)
{
#ifdef DCM_ROE_STORAGE_NVMBLOCK
    uint8 storageState = (subFunction & DCM_UDS86_STORAGE_BIT_MASK) >> 6u;
#endif
#ifdef DCM_UDS_0X86_0X4
    if (DCM_UDS86_REPORTACTIVATEDEVENTS != subFunction)
#endif
    {
        pMsgContext->resData[1u] = 0u;
        pMsgContext->resData[2u] = (uint8)eventWindowTime;
        pMsgContext->resDataLen += 2u;
    }
#ifdef DCM_ROE_STORAGE_NVMBLOCK
    if (
#ifdef DCM_UDS_0X86_0X4
        (DCM_UDS86_REPORTACTIVATEDEVENTS != subFunction) &&
#endif
        ((1u == storageState) || (DCM_UDS86_STOPROE == subFunction) || (DCM_UDS86_STARTROE == subFunction)
         || (DCM_UDS86_CLEARROE == subFunction)))
    {
        (void)NvM_CancelJobs(DCM_ROE_STORAGE_NVMBLOCK);
        (void)NvM_WritePRAMBlock(DCM_ROE_STORAGE_NVMBLOCK);
    }
#endif
}

DCM_LOCAL Std_ReturnType Dcm_UDS0x86_HandleSubFunction(
    uint8                         protocolId,
    Dcm_RoeCtrlType*              roeCtrlPtr,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_RoeEventWindowTimeType    eventWindowTime,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result      = E_OK;
    uint8          subFunction = pMsgContext->reqData[0u];
    switch (subFunction)
    {
#ifdef DCM_UDS_0X86_0X0
    case DCM_UDS86_STOPROE:
    {
        Dcm_UDS0x86_SetRoeStatus(DCM_ROE_OFF);
        break;
    }
#endif
#ifdef DCM_UDS_0X86_0X1
    case DCM_UDS86_ONDTC:
    {
        result = Dcm_UDS0x86_01(protocolId, roeCtrlPtr, pMsgContext, eventWindowTime, ErrorCode);
        break;
    }
#endif
#ifdef DCM_UDS_0X86_0X3
    case DCM_UDS86_ONDID:
    {
        result = Dcm_UDS0x86_03(protocolId, roeCtrlPtr, pMsgContext, eventWindowTime, ErrorCode);
        break;
    }
#endif
#ifdef DCM_UDS_0X86_0X8
    case DCM_UDS86_REPORTMOSTRECENTDTC:
    {
        result = Dcm_UDS0x86_08(protocolId, roeCtrlPtr, pMsgContext, eventWindowTime, ErrorCode);
        break;
    }
#endif
#ifdef DCM_UDS_0X86_0X4
    case DCM_UDS86_REPORTACTIVATEDEVENTS:
    {
        Dcm_UDS0x86_04(pMsgContext);
        break;
    }
#endif
#ifdef DCM_UDS_0X86_0X5
    case DCM_UDS86_STARTROE:
    {
        Dcm_UDS0x86_SetRoeStatus(DCM_ROE_ON);
        break;
    }
#endif
#ifdef DCM_UDS_0X86_0X6
    case DCM_UDS86_CLEARROE:
    {
        Dcm_UDS0x86_06();
        break;
    }
#endif
    default:
    {
        /* idle */
        break;
    }
    }

    return result;
}

#if ((defined DCM_UDS_0X86_0X5) || (defined DCM_UDS_0X86_0X0))
DCM_LOCAL void Dcm_UDS0x86_SetRoeStatus(Dcm_RoeStatusType RoeStatus)
{
    for (uint8 index = 0u; index < Dcm_RoeCtrlNum; index++)
    {
        Dcm_RoeCtrl[index].RoeStatus = RoeStatus;
    }
}
#endif

#ifdef DCM_UDS_0X86_0X6
DCM_LOCAL void Dcm_UDS0x86_06(void)
{
    for (uint8 index = 0u; index < Dcm_RoeCtrlNum; index++)
    {
        Dcm_RoeCtrl[index].RoeEvent          = DCM_ROE_UNDEFINED;
        Dcm_RoeCtrl[index].RoeStatus         = DCM_ROE_OFF;
        Dcm_RoeCtrl[index].PendingServiceReq = FALSE;
    }
    Dcm_RoeCtrlNum    = 0u;
    Dcm_RoeCtrlDtcNum = 0u;
    Dcm_RoeCtrlDidNum = 0u;
}
#endif

#if ((defined(DCM_UDS_0X86_0X1)) || ((defined(DCM_UDS_0X86_0X3)) && (DCM_DID_NUM > 0)) || (defined(DCM_UDS_0X86_0X8)))
DCM_LOCAL Std_ReturnType Dcm_UDS0x86_CheckService(
    Dcm_ProtocolType              protocolType,
    Dcm_RoeCtrlType*              roeCtrl,
    uint8*                        reqData,
    Dcm_MsgLenType                reqDataLen,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    uint16    servIndex;
    PduIdType pduId = 0u;
    uint8     protocolId;
    Dcm_UDS0x86_getRoePduIdAndProtocolId(protocolType, &pduId, &protocolId);
    Std_ReturnType result = DsdInternal_CheckService(protocolId, &servIndex, &reqData[0u]);

    if ((E_OK == result) && (TRUE == Dcm_DsdService[servIndex].SubfuncAvail))
    {
        if (reqDataLen >= 2u)
        {
            uint16 subServIndex;
            result = DsdInternal_CheckSubService(protocolId, servIndex, &subServIndex, &reqData[1u], ErrorCode);
        }
        else
        {
            result = E_NOT_OK;
        }
    }

    if (E_OK == result)
    {
        DcmInternal_Memcpy(roeCtrl->RoeServiceResponseRecord, reqData, (uint32)reqDataLen);
        roeCtrl->RoeServiceResponseRecordSize = reqDataLen;
    }

    if (E_NOT_OK == result)
    {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
    }

    return result;
}
#endif

#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
