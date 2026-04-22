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
 **  @file               : Dcm_UDS0x19.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm UDS service 0x19 function implementation
 **
 ***********************************************************************************************************************/
/* PRQA S 3120 EOF */ /* VL_QAC_MagicNum */
/* =================================================== inclusions =================================================== */
#include "Dcm_Cfg.h"
#if (STD_ON == DCM_UDS_0X19)
#include "Dcm_Internal.h"
/* ===================================================== macros ===================================================== */
#ifdef DCM_UDS_0X19_0X1
/**
 * @brief subfunction reportNumberOfDTCByStatusMask
 */
#define DCM_UDS19_REPODTCNUM 1u
#endif
#if ((defined DCM_UDS_0X19_0X2) || (defined DCM_UDS_0X19_0XA))
/**
 * @brief subfunction reportDTCByStatusMask
 */
#define DCM_UDS19_REPODTC 2u
/**
 * @brief subfunction reportSupportedDTC
 */
#define DCM_UDS19_REPOSUPDTC 0xAu
#endif
#ifdef DCM_UDS_0X19_0X3
/**
 * @brief subfunction reportDTCSnapshotIdentification
 */
#define DCM_UDS19_REPOSNAPSHOTID 3u
#endif
#ifdef DCM_UDS_0X19_0X4
/**
 * @brief subfunction reportDTCSnapshotRecordByDTCNumber
 */
#define DCM_UDS19_REPOSNAPSHOTRECORD 4u
#endif
#ifdef DCM_UDS_0X19_0XE
/**
 * @brief subfunction reportMostRecentConfirmedDTC
 */
#define DCM_UDS19_REPOMRCDTC 0xEu
#endif
/**
 * @brief the length of DTC and its status
 */
#define DCM_DTC_STATUS_LENGTH 4uL
/**
 * @brief the length of DTC
 */
#define DCM_DTC_LENGTH 3uL
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
#define DCM_START_SEC_VAR_CLEARED_16
#include "Dcm_MemMap.h"
static boolean oldPagedBufferStarted = FALSE;
#define DCM_STOP_SEC_VAR_CLEARED_16
#include "Dcm_MemMap.h"
#endif
/* ========================================== internal function declarations ======================================== */
#ifdef DCM_UDS_0X19_0X1
/**
 * @brief         The service interpreter for UDS 0x19 subfunction 0x01
 * @param[in]     clientId    : the Dem clientId
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     TRUE
 * @synchronous   Depending on Dem
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType
    Dcm_UDS0x19_01(uint8 clientId, Dcm_MsgContextType* pMsgContext, Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#if ((defined DCM_UDS_0X19_0X2) || (defined DCM_UDS_0X19_0XA))
/**
 * @brief         The service interpreter for UDS 0x19 subfunction 0x02 and 0x0A
 * @param[in]     clientId           : the Dem clientId
 * @param[inout]  pMsgContext        : Message-related information for one diagnostic protocol identifier. The pointers
 *                                     in pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode          : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm
 *                                     module shall send a negative response with NRC code equal to the parameter
 *                                     ErrorCode parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     TRUE
 * @synchronous   Depending on Dem
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType
    Dcm_UDS0x19_02_0A(uint8 clientId, Dcm_MsgContextType* pMsgContext, Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         prepare for processing sunfunction 0x2/0xA
 * @param[in]     clientId        : the Dem clientId
 * @param[inout]  pMsgContext     : Message-related information for one diagnostic protocol identifier. The pointers
 *                                  in pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode       : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm
 *                                  module shall send a negative response with NRC code equal to the parameter ErrorCode
 *                                  parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @reentrant     TRUE
 * @synchronous   Synchronous
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType
    Dcm_UDS0x19_02_0A_Prepare(uint8 clientId, Dcm_MsgContextType* pMsgContext, Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         process sunfunction 0x2/0xA
 * @param[in]     clientId              : the Dem clientId
 * @param[in]     oldPagedBufferStarted : whether the page buffer is started or not
 * @param[inout]  pMsgContext           : Message-related information for one diagnostic protocol identifier. The
 *                                        pointers in pMsgContext shall point behind the SID.
 * @param[out]    offset                : The resulting offset
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @reentrant     TRUE
 * @synchronous   Synchronous
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x19_02_0A_Process(
    uint8               clientId,
    Dcm_MsgContextType* pMsgContext,
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
    boolean oldPagedBufferStarted,
#endif
    uint32* offset);
#endif

#ifdef DCM_UDS_0X19_0X3
/**
 * @brief         The service interpreter for UDS 0x19 subfunction 0x03
 * @param[in]     clientId           : the Dem clientId
 * @param[inout]  pMsgContext        : Message-related information for one diagnostic protocol identifier. The pointers
 *                                     in pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode          : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm
 *                                     module shall send a negative response with NRC code equal to the parameter
 *                                     ErrorCode parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     TRUE
 * @synchronous   Depending on Dem
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType
    Dcm_UDS0x19_03(uint8 clientId, Dcm_MsgContextType* pMsgContext, Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         prepare for processing sunfunction 0x3
 * @param[in]     clientId        : the Dem clientId
 * @param[inout]  pMsgContext     : Message-related information for one diagnostic protocol identifier. The pointers
 *                                  in pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode       : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm
 *                                  module shall send a negative response with NRC code equal to the parameter ErrorCode
 *                                  parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @reentrant     TRUE
 * @synchronous   Synchronous
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType
    Dcm_UDS0x19_03_Prepare(uint8 clientId, Dcm_MsgContextType* pMsgContext, Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#ifdef DCM_UDS_0X19_0X4
/**
 * @brief         The service interpreter for UDS 0x19 subfunction 0x04
 * @param[in]     OpStatus        : current operation status
 * @param[in]     clientId           : the Dem clientId
 * @param[inout]  pMsgContext        : Message-related information for one diagnostic protocol identifier. The
 *                                     pointers in pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode          : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm
 *                                     module shall send a negative response with NRC code equal to the parameter
 *                                     ErrorCode parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     TRUE
 * @synchronous   Depending on Dem
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x19_04(
    Dcm_OpStatusType              OpStatus,
    uint8                         clientId,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         prepare for processing sunfunction 0x4
 * @param[in]     OpStatus        : current operation status
 * @param[in]     clientId        : the Dem clientId
 * @param[inout]  pMsgContext     : Message-related information for one diagnostic protocol identifier. The pointers
 *                                  in pMsgContext shall point behind the SID.
 * @param[out]    disabled        : set to TRUE when Dem_DisableDTCRecordUpdate return E_OK
 * @param[out]    ErrorCode       : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm
 *                                  module shall send a negative response with NRC code equal to the parameter ErrorCode
 *                                  parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @reentrant     TRUE
 * @synchronous   Synchronous
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x19_04_Prepare(
    Dcm_OpStatusType              OpStatus,
    uint8                         clientId,
    Dcm_MsgContextType*           pMsgContext,
    boolean*                      disabled,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#ifdef DCM_UDS_0X19_0X6
/**
 * @brief         The service interpreter for UDS 0x19 subfunction 0x06
 * @param[in]     OpStatus           : current operation status
 * @param[in]     clientId           : the Dem clientId
 * @param[inout]  pMsgContext        : Message-related information for one diagnostic protocol identifier. The pointers
 *                                     in pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode          : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm
 *                                     module shall send a negative response with NRC code equal to the parameter
 *                                     ErrorCode parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     TRUE
 * @synchronous   Depending on Dem
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x19_06(
    Dcm_OpStatusType              OpStatus,
    uint8                         clientId,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         prepare for processing sunfunction 0x4
 * @param[in]     OpStatus        : current operation status
 * @param[in]     clientId        : the Dem clientId
 * @param[inout]  pMsgContext     : Message-related information for one diagnostic protocol identifier. The pointers
 *                                  in pMsgContext shall point behind the SID.
 * @param[out]    disabled        : set to TRUE when Dem_DisableDTCRecordUpdate return E_OK
 * @param[out]    ErrorCode       : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm
 *                                  module shall send a negative response with NRC code equal to the parameter ErrorCode
 *                                  parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @reentrant     TRUE
 * @synchronous   Synchronous
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x19_06_Prepare(
    Dcm_OpStatusType              OpStatus,
    uint8                         clientId,
    Dcm_MsgContextType*           pMsgContext,
    boolean*                      disabled,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#ifdef DCM_UDS_0X19_0XD
/**
 * @brief         The service interpreter for UDS 0x19 subfunction 0x0D
 * @param[in]     clientId    : the Dem clientId
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     TRUE
 * @synchronous   Depending on Dem
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType
    Dcm_UDS0x19_0D(uint8 clientId, Dcm_MsgContextType* pMsgContext, Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#ifdef DCM_UDS_0X19_0XE
/**
 * @brief         The service interpreter for UDS 0x19 subfunction 0x0E
 * @param[in]     clientId    : the Dem clientId
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     TRUE
 * @synchronous   Depending on Dem
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType
    Dcm_UDS0x19_0E(uint8 clientId, Dcm_MsgContextType* pMsgContext, Dcm_NegativeResponseCodeType* ErrorCode);
#endif

/**
 * @brief         process subFunction of 0x19
 * @param[in]     OpStatus           : current operation status
 * @param[in]     protocolId         : current protocol id
 * @param[out]    ErrorCode          : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm
 * module shall send a negative response with NRC code equal to the parameter ErrorCode parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     TRUE
 * @synchronous   Depending on Dem
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x19_ProcessSubFunction(
    Dcm_OpStatusType              OpStatus,
    uint8                         protocolId,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         deal with subFunction process result
 * @param[in]     protocolId        : current protocol id
 * @param[in]     subFunctionResult : the result of Dcm_UDS0x19_ProcessSubFunction
 * @param[out]    ErrorCode         : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm
 * module shall send a negative response with NRC code equal to the parameter ErrorCode parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     TRUE
 * @synchronous   Depending on Dem
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x19_PostProcess(
    uint8                         protocolId,
    Std_ReturnType                subFunctionResult,
    Dcm_NegativeResponseCodeType* ErrorCode);

#if (                                                                                            \
    (STD_ON == DCM_UDS_0X19_0X2) || (STD_ON == DCM_UDS_0X19_0XA) || (STD_ON == DCM_UDS_0X19_0X6) \
    || (STD_ON == DCM_UDS_0X19_0X4) || (STD_ON == DCM_UDS_0X19_0X3))
/**
 * @brief         deal with total response length
 * @param[in]     pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : nrc to send
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @reentrant     TRUE
 * @synchronous   Synchronous
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType
    Dcm_UDS0x19_HandleTotalLength(const Dcm_MsgContextType* pMsgContext, Dcm_NegativeResponseCodeType* ErrorCode);
#endif
/* ========================================== external function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * The service interpreter for UDS 0x19
 */
/* PRQA S 1532++ */ /* VL_QAC_OneFunRef */
Std_ReturnType Dcm_UDS0x19(
    Dcm_ExtendedOpStatusType OpStatus,
    /* PRQA S 3673++ */ /* VL_QAC_3673 */
    Dcm_MsgContextType* pMsgContext,
    /* PRQA S 3673-- */ /* VL_QAC_3673 */
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    DCM_UNUSED(OpStatus);
    Std_ReturnType result = E_OK;
    uint8          protocolId;
    (void)DslInternal_FindProtocolRowByRxPduId(pMsgContext->dcmRxPduId, &protocolId, NULL_PTR);
    result = Dcm_UDS0x19_ProcessSubFunction(OpStatus, protocolId, ErrorCode);
    result = Dcm_UDS0x19_PostProcess(protocolId, result, ErrorCode);

    return result;
}
/* PRQA S 1532-- */ /* VL_QAC_OneFunRef */
/* ========================================== internal function definitions ========================================= */
#ifdef DCM_UDS_0X19_0X1
/**
 * The service interpreter for UDS 0x19, subfunctiun 0x01
 */
DCM_LOCAL Std_ReturnType
    Dcm_UDS0x19_01(uint8 clientId, Dcm_MsgContextType* pMsgContext, Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_OK;

    if (2uL != pMsgContext->reqDataLen)
    {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        result     = E_NOT_OK;
    }

    if (E_OK == result)
    {
        result = Dem_SetDTCFilter(
            clientId,
            pMsgContext->reqData[1u],
            DEM_DTC_FORMAT_UDS,
            DEM_DTC_ORIGIN_PRIMARY_MEMORY,
            FALSE,
            DEM_SEVERITY_NO_SEVERITY,
            FALSE);
    }

    uint16 NumberOfFilteredDTC = 0u;
    if (E_OK == result)
    {
        result = Dem_GetNumberOfFilteredDTC(clientId, &NumberOfFilteredDTC);
    }
    Dem_UdsStatusByteType DTCStatusAvailabilityMask = 0u;
    if (E_OK == result)
    {
        result = Dem_GetDTCStatusAvailabilityMask(clientId, &DTCStatusAvailabilityMask, DEM_DTC_ORIGIN_PRIMARY_MEMORY);
    }
    if (E_OK == result)
    {
        uint8* resData = pMsgContext->resData;
        resData[1u]    = (uint8)DTCStatusAvailabilityMask;
        resData[2u]    = Dem_GetTranslationType(clientId);
        DcmInternal_TransformArray_u16(&resData[3u], NumberOfFilteredDTC, DCM_OPAQUE);
        pMsgContext->resDataLen = 5u;
    }

    return result;
}
#endif

#if ((STD_ON == DCM_UDS_0X19_0X2) || (STD_ON == DCM_UDS_0X19_0XA))
/**
 * The service interpreter for UDS 0x19, subfunctiun 0x02/0x0A
 */
DCM_LOCAL Std_ReturnType
    Dcm_UDS0x19_02_0A(uint8 clientId, Dcm_MsgContextType* pMsgContext, Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_OK;

#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
    uint8 protocolId;
    (void)DslInternal_FindProtocolRowByRxPduId(pMsgContext->dcmRxPduId, &protocolId, NULL_PTR);
    oldPagedBufferStarted = Dcm_ProtocolCtrl[protocolId].PagedBufferStarted;
    if (FALSE == oldPagedBufferStarted)
#endif
    {
        result = Dcm_UDS0x19_02_0A_Prepare(clientId, pMsgContext, ErrorCode);
    }

    uint32 offset = 2u;
    if (E_OK == result)
    {
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
        if (TRUE == oldPagedBufferStarted)
        {
            offset = 0u;
        }
        result = Dcm_UDS0x19_02_0A_Process(clientId, pMsgContext, oldPagedBufferStarted, &offset);
#else
        result = Dcm_UDS0x19_02_0A_Process(clientId, pMsgContext, &offset);
#endif
    }

#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
    if (((E_OK == result) || (DEM_NO_SUCH_ELEMENT == result))
        && (TRUE == Dcm_ProtocolCtrl[protocolId].PagedBufferStarted))
    {
        if (FALSE == oldPagedBufferStarted)
        {
            Dcm_ProtocolCtrl[protocolId].RemainPageLength += (offset + 1u);
        }
        else
        {
            Dcm_ProtocolCtrl[protocolId].RemainPageLength += offset;
        }
    }
#endif

    return result;
}

/* setup filter and get number of filterd DTC, prepare for further process */
DCM_LOCAL Std_ReturnType
    Dcm_UDS0x19_02_0A_Prepare(uint8 clientId, Dcm_MsgContextType* pMsgContext, Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result        = E_OK;
    Dcm_MsgLenType reqDataLen    = pMsgContext->reqDataLen;
    uint8          DTCStatusMask = 0u;
    if (0x02u == pMsgContext->reqData[0u])
    {
        if (2uL != reqDataLen)
        {
            *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
            result     = E_NOT_OK;
        }
        else
        {
            DTCStatusMask = pMsgContext->reqData[1u];
        }
    }
    else
    {
        if (1uL != reqDataLen)
        {
            *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
            result     = E_NOT_OK;
        }
    }

    if (E_OK == result)
    {
        result = Dem_SetDTCFilter(
            clientId,
            DTCStatusMask,
            DEM_DTC_FORMAT_UDS,
            DEM_DTC_ORIGIN_PRIMARY_MEMORY,
            FALSE,
            DEM_SEVERITY_NO_SEVERITY,
            FALSE);
    }

    if (E_OK == result)
    {
        uint16 NumberOfFilteredDTC = 0u;
        result                     = Dem_GetNumberOfFilteredDTC(clientId, &NumberOfFilteredDTC);
        if (E_OK == result)
        {
            pMsgContext->resDataLen = ((Dcm_MsgLenType)NumberOfFilteredDTC * DCM_DTC_STATUS_LENGTH) + 2uL;
            result                  = Dcm_UDS0x19_HandleTotalLength(pMsgContext, ErrorCode);
        }
    }

    return result;
}

DCM_LOCAL Std_ReturnType Dcm_UDS0x19_02_0A_Process(
    uint8 clientId,
    /* PRQA S 3673++ */ /* VL_QAC_3673 */
    Dcm_MsgContextType* pMsgContext,
/* PRQA S 3673-- */ /* VL_QAC_3673 */
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
    boolean oldPagedBufferStarted,
#endif
    uint32* offset)
{
    Std_ReturnType result            = E_OK;
    uint8*         resData           = pMsgContext->resData;
    uint16         transmittedLength = 0u;
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
    uint8 protocolId;
    (void)DslInternal_FindProtocolRowByRxPduId(pMsgContext->dcmRxPduId, &protocolId, NULL_PTR);
    if (TRUE == oldPagedBufferStarted)
    {
        transmittedLength = Dcm_ProtocolCtrl[protocolId].TxCopyLen + Dcm_ProtocolCtrl[protocolId].RemainPageLength - 1u;
    }
#endif
    while (E_OK == result)
    {
        uint32 DTC       = 0u;
        uint8  DTCStatus = 0u;

#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
        if ((*offset + DCM_DTC_STATUS_LENGTH) > pMsgContext->resMaxDataLen)
        {
            break;
        }
#endif
        /* PRQA S 2985 ++ */ /* VL_Dcm_2985 */
        if ((*offset + DCM_DTC_STATUS_LENGTH + transmittedLength) <= pMsgContext->resDataLen)
        {
            /* PRQA S 2985 -- */
            result = Dem_GetNextFilteredDTC(clientId, &DTC, &DTCStatus);
            if (E_OK == result)
            {
                DcmInternal_TransformArray_u24(&resData[*offset], DTC, DCM_OPAQUE);
                resData[*offset + DCM_DTC_LENGTH] = DTCStatus;
                (*offset) += DCM_DTC_STATUS_LENGTH;
            }
        }
        else
        {
            result = DEM_NO_SUCH_ELEMENT;
        }
    }

#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
    if ((FALSE == oldPagedBufferStarted) && (TRUE == Dcm_ProtocolCtrl[protocolId].PagedBufferStarted)
        && ((E_OK == result) || (DEM_NO_SUCH_ELEMENT == result)))
    {
        Dem_UdsStatusByteType DTCStatusAvailabilityMask;
        result = Dem_GetDTCStatusAvailabilityMask(clientId, &DTCStatusAvailabilityMask, DEM_DTC_ORIGIN_PRIMARY_MEMORY);
        resData[1u] = (uint8)DTCStatusAvailabilityMask;
    }
#endif

    if (DEM_NO_SUCH_ELEMENT == result)
    {
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
        while (((*offset + DCM_DTC_STATUS_LENGTH) <= pMsgContext->resMaxDataLen)
               && ((*offset + transmittedLength) < pMsgContext->resDataLen))
#else
        while (((*offset + DCM_DTC_STATUS_LENGTH) <= pMsgContext->resMaxDataLen) && (*offset < pMsgContext->resDataLen))
#endif
        {
            DcmInternal_Memset(&resData[*offset], 0x00u, DCM_DTC_STATUS_LENGTH);
            (*offset) += DCM_DTC_STATUS_LENGTH;
        }
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
        if (FALSE == Dcm_ProtocolCtrl[protocolId].PagedBufferStarted)
#endif
        {
            Dem_UdsStatusByteType DTCStatusAvailabilityMask;
            result =
                Dem_GetDTCStatusAvailabilityMask(clientId, &DTCStatusAvailabilityMask, DEM_DTC_ORIGIN_PRIMARY_MEMORY);
            resData[1u] = (uint8)DTCStatusAvailabilityMask;
        }
    }

    return result;
}
#endif

#ifdef DCM_UDS_0X19_0X3
/**
 * The service interpreter for UDS 0x19, subfunctiun 0x03
 */
DCM_LOCAL Std_ReturnType
    Dcm_UDS0x19_03(uint8 clientId, Dcm_MsgContextType* pMsgContext, Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_OK;

#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
    uint8 protocolId;
    (void)DslInternal_FindProtocolRowByRxPduId(pMsgContext->dcmRxPduId, &protocolId, NULL_PTR);
    boolean oldPagedBufferStarted = Dcm_ProtocolCtrl[protocolId].PagedBufferStarted;
    if (FALSE == Dcm_ProtocolCtrl[protocolId].PagedBufferStarted)
#endif
    {
        result = Dcm_UDS0x19_03_Prepare(clientId, pMsgContext, ErrorCode);
    }

    uint32 offset  = 1u;
    uint8* resData = pMsgContext->resData;
    while (E_OK == result)
    {
        uint32 DTC = 0u;
        uint8  RecordNumber;

#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
        if ((offset + DCM_DTC_STATUS_LENGTH) > pMsgContext->resMaxDataLen)
        {
            break;
        }
#endif

        if ((offset + DCM_DTC_STATUS_LENGTH) <= pMsgContext->resDataLen)
        {
            result = Dem_GetNextFilteredRecord(clientId, &DTC, &RecordNumber);
            if (E_OK == result)
            {
                DcmInternal_TransformArray_u24(&resData[offset], DTC, DCM_OPAQUE);
                resData[offset + 3u] = RecordNumber;
                offset += DCM_DTC_STATUS_LENGTH;
            }
        }
        else
        {
            result = DEM_NO_SUCH_ELEMENT;
        }
    }

    if (DEM_NO_SUCH_ELEMENT == result)
    {
        while (offset < pMsgContext->resDataLen)
        {
            DcmInternal_Memset(&resData[offset], 0x00u, DCM_DTC_STATUS_LENGTH);
            offset += DCM_DTC_STATUS_LENGTH;
        }
    }

#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
    if (((E_OK == result) || (DEM_NO_SUCH_ELEMENT == result))
        && (TRUE == Dcm_ProtocolCtrl[protocolId].PagedBufferStarted))
    {
        if (FALSE == oldPagedBufferStarted)
        {
            Dcm_ProtocolCtrl[protocolId].RemainPageLength += offset;
        }
        else
        {
            Dcm_ProtocolCtrl[protocolId].RemainPageLength += (offset - 1u);
        }
    }
#endif

    return result;
}

/* setup filter and get number of result */
DCM_LOCAL Std_ReturnType
    Dcm_UDS0x19_03_Prepare(uint8 clientId, Dcm_MsgContextType* pMsgContext, Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_OK;
    if (1u != pMsgContext->reqDataLen)
    {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        result     = E_NOT_OK;
    }

    if (E_OK == result)
    {
        result = Dem_SetFreezeFrameRecordFilter(clientId, DEM_DTC_ORIGIN_PRIMARY_MEMORY);
    }

    if (E_OK == result)
    {
        uint16 NumberOfFilteredRecords = 0u;
        result                         = Dem_GetNumberOfFreezeFrameRecords(clientId, &NumberOfFilteredRecords);
        if (E_OK == result)
        {
            pMsgContext->resDataLen =
                ((Dcm_MsgLenType)NumberOfFilteredRecords * (Dcm_MsgLenType)DCM_DTC_STATUS_LENGTH) + 1u;
            result = Dcm_UDS0x19_HandleTotalLength(pMsgContext, ErrorCode);
        }
    }

    return result;
}
#endif

#ifdef DCM_UDS_0X19_0X4
/**
 * The service interpreter for UDS 0x19, subfunctiun 0x04
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x19_04(
    Dcm_OpStatusType              OpStatus,
    uint8                         clientId,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result   = E_OK;
    uint8*         resData  = pMsgContext->resData;
    boolean        disabled = FALSE;

#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
    uint8 protocolId;
    (void)DslInternal_FindProtocolRowByRxPduId(pMsgContext->dcmRxPduId, &protocolId, NULL_PTR);
    boolean oldPagedBufferStarted = Dcm_ProtocolCtrl[protocolId].PagedBufferStarted;
    if (FALSE == Dcm_ProtocolCtrl[protocolId].PagedBufferStarted)
#endif
    {
        result = Dcm_UDS0x19_04_Prepare(OpStatus, clientId, pMsgContext, &disabled, ErrorCode);
    }

    uint32 offset = 5u;
    /* PRQA S 4461++ */ /* VL_Dcm_4461 */
    uint16 BufSize = ((pMsgContext->resMaxDataLen - 5uL) > DCM_INVALID_UINT16)
                         ? DCM_INVALID_UINT16
                         : ((uint16)pMsgContext->resMaxDataLen - 5uL);
    /* PRQA S 4461-- */ /* VL_Dcm_4461 */
    while (E_OK == result)
    {
        result = Dem_GetNextFreezeFrameData(clientId, &resData[offset], &BufSize);
        if (DEM_BUFFER_TOO_SMALL == result)
        {
            break;
        }
        else if (E_OK == result)
        {
            offset += BufSize;
            BufSize = (uint16)(pMsgContext->resMaxDataLen - offset);
        }
        else
        {
            /* idle */
        }
    }

    if (disabled)
    {
        (void)Dem_EnableDTCRecordUpdate(clientId);
    }

#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
    if ((DEM_NO_SUCH_ELEMENT == result) && (TRUE == Dcm_ProtocolCtrl[protocolId].PagedBufferStarted))
    {
        if (FALSE == oldPagedBufferStarted)
        {
            Dcm_ProtocolCtrl[protocolId].RemainPageLength += offset;
        }
        else
        {
            Dcm_ProtocolCtrl[protocolId].RemainPageLength += (offset - 2u);
        }
    }
#endif

    return result;
}

/* setup filter and get number of result */
DCM_LOCAL Std_ReturnType Dcm_UDS0x19_04_Prepare(
    Dcm_OpStatusType              OpStatus,
    uint8                         clientId,
    Dcm_MsgContextType*           pMsgContext,
    boolean*                      disabled,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result  = E_NOT_OK;
    const uint8*   reqData = pMsgContext->reqData;
    uint8*         resData = pMsgContext->resData;

    if (DCM_INITIAL == OpStatus)
    {
        if (5uL != pMsgContext->reqDataLen)
        {
            *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
            result     = E_NOT_OK;
        }
        else
        {
            uint32 DTC = DCM_U8N_TO_U24(&reqData[1u]);
            DcmInternal_TransformArray_u24(&resData[1u], DTC, DCM_OPAQUE);
            result = Dem_SelectDTC(clientId, DTC, DEM_DTC_FORMAT, DEM_DTC_ORIGIN_PRIMARY_MEMORY);
        }

        if (E_OK == result)
        {
            result = Dem_GetStatusOfDTC(clientId, &resData[4u]);
        }

        if (E_OK == result)
        {
            result = Dem_DisableDTCRecordUpdate(clientId);
        }
    }
    else if (DCM_PENDING == OpStatus)
    {
        result = Dem_DisableDTCRecordUpdate(clientId);
    }
    else
    {
        /* idle */
    }

    if (E_OK == result)
    {
        *disabled = TRUE;
        result    = Dem_SelectFreezeFrameData(clientId, reqData[4u]);
    }

    if (E_OK == result)
    {
        uint32 SizeOfFreezeFrame = 0u;
        result                   = Dem_GetSizeOfFreezeFrameSelection(clientId, &SizeOfFreezeFrame);
        if (E_OK == result)
        {
            pMsgContext->resDataLen = (Dcm_MsgLenType)SizeOfFreezeFrame + 5uL;
            result                  = Dcm_UDS0x19_HandleTotalLength(pMsgContext, ErrorCode);
        }
        else if (DEM_NO_SUCH_ELEMENT == result)
        {
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            result     = E_NOT_OK;
        }
        else
        {
            /* idle */
        }
    }

    return result;
}
#endif

#ifdef DCM_UDS_0X19_0X6
/**
 * The service interpreter for UDS 0x19, subfunctiun 0x06
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x19_06(
    Dcm_OpStatusType              OpStatus,
    uint8                         clientId,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result   = E_OK;
    uint8*         resData  = pMsgContext->resData;
    boolean        disabled = FALSE;

#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
    uint8 protocolId;
    (void)DslInternal_FindProtocolRowByRxPduId(pMsgContext->dcmRxPduId, &protocolId, NULL_PTR);
    boolean oldPagedBufferStarted = Dcm_ProtocolCtrl[protocolId].PagedBufferStarted;
    if (FALSE == Dcm_ProtocolCtrl[protocolId].PagedBufferStarted)
#endif
    {
        result = Dcm_UDS0x19_06_Prepare(OpStatus, clientId, pMsgContext, &disabled, ErrorCode);
    }

    uint32 offset = 5u;
    /* PRQA S 4461++ */ /* VL_Dcm_4461 */
    uint16 BufSize = ((pMsgContext->resMaxDataLen - 5uL) > DCM_INVALID_UINT16)
                         ? DCM_INVALID_UINT16
                         : ((uint16)pMsgContext->resMaxDataLen - 5uL);
    /* PRQA S 4461-- */ /* VL_Dcm_4461 */
    while (E_OK == result)
    {
        result = Dem_GetNextExtendedDataRecord(clientId, &resData[offset], &BufSize);
        if (DEM_BUFFER_TOO_SMALL == result)
        {
            break;
        }
        else if (E_OK == result)
        {
            offset += BufSize;
            BufSize = (uint16)(pMsgContext->resMaxDataLen - offset);
        }
        else
        {
            /* idle */
        }
    }

    if (disabled)
    {
        (void)Dem_EnableDTCRecordUpdate(clientId);
    }

#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
    if ((DEM_NO_SUCH_ELEMENT == result) && (TRUE == Dcm_ProtocolCtrl[protocolId].PagedBufferStarted))
    {
        if (FALSE == oldPagedBufferStarted)
        {
            Dcm_ProtocolCtrl[protocolId].RemainPageLength += offset;
        }
        else
        {
            Dcm_ProtocolCtrl[protocolId].RemainPageLength += (offset - 2u);
        }
    }
#endif

    return result;
}

/* setup filter and get number of result */
DCM_LOCAL Std_ReturnType Dcm_UDS0x19_06_Prepare(
    Dcm_OpStatusType              OpStatus,
    uint8                         clientId,
    Dcm_MsgContextType*           pMsgContext,
    boolean*                      disabled,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result  = E_NOT_OK;
    const uint8*   reqData = pMsgContext->reqData;
    uint8*         resData = pMsgContext->resData;

    if (OpStatus == DCM_INITIAL)
    {
        if (5uL != pMsgContext->reqDataLen)
        {
            *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
            result     = E_NOT_OK;
        }
        else
        {
            uint32 DTC = DCM_U8N_TO_U24(&reqData[1u]);
            DcmInternal_TransformArray_u24(&resData[1u], DTC, DCM_OPAQUE);
            result = Dem_SelectDTC(clientId, DTC, DEM_DTC_FORMAT, DEM_DTC_ORIGIN_PRIMARY_MEMORY);
        }

        if (E_OK == result)
        {
            result = Dem_GetStatusOfDTC(clientId, &resData[4u]);
        }

        if (E_OK == result)
        {
            result = Dem_DisableDTCRecordUpdate(clientId);
        }
    }
    else if (OpStatus == DCM_PENDING)
    {
        result = Dem_DisableDTCRecordUpdate(clientId);
    }
    else
    {
        /* idle */
    }

    if (E_OK == result)
    {
        *disabled = TRUE;
        result    = Dem_SelectExtendedDataRecord(clientId, reqData[4u]);
    }

    if (E_OK == result)
    {
        uint32 SizeOfExtendedDataRecord = 0u;
        result                          = Dem_GetSizeOfExtendedDataRecordSelection(clientId, &SizeOfExtendedDataRecord);
        if (E_OK == result)
        {
            pMsgContext->resDataLen = (Dcm_MsgLenType)SizeOfExtendedDataRecord + 5uL;
            result                  = Dcm_UDS0x19_HandleTotalLength(pMsgContext, ErrorCode);
        }
        else if (DEM_NO_SUCH_ELEMENT == result)
        {
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            result     = E_NOT_OK;
        }
        else
        {
            /* idle */
        }
    }

    return result;
}
#endif

#ifdef DCM_UDS_0X19_0XD
/**
 * The service interpreter for UDS 0x19, subfunctiun 0x0D
 */
DCM_LOCAL Std_ReturnType
    Dcm_UDS0x19_0D(uint8 clientId, Dcm_MsgContextType* pMsgContext, Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_OK;

    if (1u != pMsgContext->reqDataLen)
    {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        result     = E_NOT_OK;
    }

    Dem_UdsStatusByteType DTCStatusAvailabilityMask;
    if (E_OK == result)
    {
        result = Dem_GetDTCStatusAvailabilityMask(clientId, &DTCStatusAvailabilityMask, DEM_DTC_ORIGIN_PRIMARY_MEMORY);
    }

    uint32          DTC;
    Dcm_MsgLenType* resDataLen = &pMsgContext->resDataLen;
    uint8*          resData    = pMsgContext->resData;
    if (E_OK == result)
    {
        resData[1u] = (uint8)DTCStatusAvailabilityMask;
        result      = Dem_GetDTCByOccurrenceTime(clientId, DEM_MOST_RECENT_FAILED_DTC, &DTC);
    }

    if (E_OK == result)
    {
        DcmInternal_TransformArray_u24(&resData[2u], DTC, DCM_OPAQUE);
        result = Dem_SelectDTC(clientId, DTC, DEM_DTC_FORMAT, DEM_DTC_ORIGIN_PRIMARY_MEMORY);
    }

    if (E_OK == result)
    {
        result = Dem_GetStatusOfDTC(clientId, &resData[5u]);
    }

    if (E_OK == result)
    {
        *resDataLen = DCM_DTC_LENGTH + 3u;
    }

    return result;
}
#endif

#ifdef DCM_UDS_0X19_0XE
/**
 * The service interpreter for UDS 0x19, subfunctiun 0x0E
 */
DCM_LOCAL Std_ReturnType
    Dcm_UDS0x19_0E(uint8 clientId, Dcm_MsgContextType* pMsgContext, Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_OK;

    if (1u != pMsgContext->reqDataLen)
    {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        result     = E_NOT_OK;
    }

    Dem_UdsStatusByteType DTCStatusAvailabilityMask;
    if (E_OK == result)
    {
        result = Dem_GetDTCStatusAvailabilityMask(clientId, &DTCStatusAvailabilityMask, DEM_DTC_ORIGIN_PRIMARY_MEMORY);
    }

    uint32          DTC        = 0u;
    Dcm_MsgLenType* resDataLen = &pMsgContext->resDataLen;
    uint8*          resData    = pMsgContext->resData;
    if (E_OK == result)
    {
        resData[1u] = (uint8)DTCStatusAvailabilityMask;
        result      = Dem_GetDTCByOccurrenceTime(clientId, DEM_MOST_REC_DET_CONFIRMED_DTC, &DTC);
    }

    if (E_OK == result)
    {
        DcmInternal_TransformArray_u24(&resData[2u], DTC, DCM_OPAQUE);
        result = Dem_SelectDTC(clientId, DTC, DEM_DTC_FORMAT, DEM_DTC_ORIGIN_PRIMARY_MEMORY);
    }

    if (E_OK == result)
    {
        result = Dem_GetStatusOfDTC(clientId, &resData[5u]);
    }

    if (E_OK == result)
    {
        *resDataLen = DCM_DTC_LENGTH + 3u;
    }
    else if (DEM_NO_SUCH_ELEMENT == result)
    {
        *resDataLen = 2u;
    }
    else
    {
        /* idle */
    }

    return result;
}
#endif

/* switch case for sunfunction handling */
DCM_LOCAL Std_ReturnType
    Dcm_UDS0x19_ProcessSubFunction(Dcm_OpStatusType OpStatus, uint8 protocolId, Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType      result      = E_OK;
    uint8               subFunction = Dcm_MsgContext[protocolId].reqData[0u];
    uint8               clientId    = Dcm_DslProtocolRow[protocolId].DemClientRef;
    Dcm_MsgContextType* pMsgContext = &Dcm_MsgContext[protocolId];

    switch (subFunction)
    {
#ifdef DCM_UDS_0X19_0X1
    case DCM_UDS19_REPODTCNUM:
    {
        result = Dcm_UDS0x19_01(clientId, pMsgContext, ErrorCode);
        break;
    }
#endif
#if ((STD_ON == DCM_UDS_0X19_0X2) || (STD_ON == DCM_UDS_0X19_0XA))
    case DCM_UDS19_REPODTC:
    case DCM_UDS19_REPOSUPDTC:
    {
        result = Dcm_UDS0x19_02_0A(clientId, pMsgContext, ErrorCode);
        break;
    }
#endif
#ifdef DCM_UDS_0X19_0X3
    case DCM_UDS19_REPOSNAPSHOTID:
    {
        result = Dcm_UDS0x19_03(clientId, pMsgContext, ErrorCode);
        break;
    }
#endif
#ifdef DCM_UDS_0X19_0X4
    case DCM_UDS19_REPOSNAPSHOTRECORD:
    {
        result = Dcm_UDS0x19_04(OpStatus, clientId, pMsgContext, ErrorCode);
        break;
    }
#endif
#ifdef DCM_UDS_0X19_0X6
    case DCM_UDS19_REPOEXTDATA:
    {
        result = Dcm_UDS0x19_06(OpStatus, clientId, pMsgContext, ErrorCode);
        break;
    }
#endif
#ifdef DCM_UDS_0X19_0XD
    case DCM_UDS19_REPOMTFDTC:
    {
        result = Dcm_UDS0x19_0D(clientId, pMsgContext, ErrorCode);
        break;
    }
#endif
#ifdef DCM_UDS_0X19_0XE
    case DCM_UDS19_REPOMRCDTC:
    {
        result = Dcm_UDS0x19_0E(clientId, pMsgContext, ErrorCode);
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

/* deal with the result of subFunction process */
DCM_LOCAL Std_ReturnType
    Dcm_UDS0x19_PostProcess(uint8 protocolId, Std_ReturnType subFunctionResult, Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result      = E_NOT_OK;
    uint8          subFunction = Dcm_MsgContext[protocolId].reqData[0u];
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
    Dcm_ProtocolCtrlType* protocolCtrlPtr = &Dcm_ProtocolCtrl[protocolId];
#endif
    const Dcm_MsgContextType* pMsgContext = &Dcm_MsgContext[protocolId];

    switch (subFunctionResult)
    {
    case E_OK:
    case DEM_NO_SUCH_ELEMENT:
    {
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
        if (oldPagedBufferStarted == FALSE)
        {
            pMsgContext->resData[0u] = subFunction;
        }
#else
        pMsgContext->resData[0u] = subFunction;
#endif
        result = E_OK;

#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
        if ((TRUE == protocolCtrlPtr->PagedBufferStarted) && (0u == protocolCtrlPtr->FirstPageLength))
        {
            protocolCtrlPtr->FirstPageLength = protocolCtrlPtr->RemainPageLength;
        }
        *ErrorCode = DCM_POS_RESP;
        result     = E_OK;

#endif
        break;
    }
    case DEM_PENDING:
    {
        result = DCM_E_PENDING;
        break;
    }
    default:
    {
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
        if ((TRUE == protocolCtrlPtr->PagedBufferStarted) && (0u == protocolCtrlPtr->FirstPageLength))
        {
            protocolCtrlPtr->FirstPageLength = protocolCtrlPtr->RemainPageLength;
        }
        *ErrorCode = DCM_POS_RESP;
        result     = E_OK;
#else
        if (DCM_POS_RESP == *ErrorCode)
        {
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            result     = E_NOT_OK;
        }
#endif
        break;
    }
    }

#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
    if ((TRUE == protocolCtrlPtr->PagedBufferStarted) && (E_OK != result))
    {
        DslInternal_PagedBufferInit(protocolCtrlPtr);
    }
#endif

    return result;
}

#if (                                                                                            \
    (STD_ON == DCM_UDS_0X19_0X2) || (STD_ON == DCM_UDS_0X19_0XA) || (STD_ON == DCM_UDS_0X19_0X6) \
    || (STD_ON == DCM_UDS_0X19_0X4) || (STD_ON == DCM_UDS_0X19_0X3))
DCM_LOCAL Std_ReturnType
    Dcm_UDS0x19_HandleTotalLength(const Dcm_MsgContextType* pMsgContext, Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_OK;
    uint8          protocolId;
    (void)DslInternal_FindProtocolRowByRxPduId(pMsgContext->dcmRxPduId, &protocolId, NULL_PTR);

    if (pMsgContext->resDataLen > pMsgContext->resMaxDataLen)
    {
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
        if (pMsgContext->resDataLen > Dcm_DslProtocolRow[protocolId].MaximumResponseSize)
#endif
        {
            *ErrorCode = DCM_E_RESPONSETOOLONG;
            result     = E_NOT_OK;
        }
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
        else
        {
            Dcm_ProtocolCtrl[protocolId].PagedBufferStarted = TRUE;
            Dcm_ProtocolCtrl[protocolId].RemainPageLength   = 0u;
        }
#endif
    }

    return result;
}
#endif
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
