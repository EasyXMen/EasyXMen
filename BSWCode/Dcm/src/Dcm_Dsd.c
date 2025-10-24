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
 **  @file               : Dcm_Dsd.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : DcmDsd Private Functions Definitions
 **
 ***********************************************************************************************************************/
/* PRQA S 1532, 1505 EOF */ /* VL_QAC_OneFunRef, VL_QAC_OneFunRef */
/* =================================================== inclusions =================================================== */
#include "Dcm_Internal.h"
#if (STD_ON == DCM_ENABLE_SECURITY_EVENT_REPORTING)
#include "IdsM.h"
#endif
#if (STD_ON == DCM_MULTICORE_ENABLED)
#include "Os.h"
#endif
/* ===================================================== macros ===================================================== */
/**
 * @brief the start value of UDS services
 */
#define DCM_UDS_SERVID_START 0x10u
/* ============================================ internal data definitions =========================================== */
#if (DCM_METADATA_NUM > 0u)
/**
 * @brief stored metadata @range NA
 */
#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
/* PRQA S 3218 ++ */ /* VL_Dcm_3218 */
DCM_LOCAL uint8 Dcm_TempMetaData[DCM_PROTOCOLROW_NUM][DCM_METADATA_SIZE];
/* PRQA S 3218 -- */
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
#endif
/* ========================================== internal function declarations ======================================== */
#if ((DCM_SUPPLIER_NOTIFICATION_NUM > 0u) || (DCM_SUPPLIER_NOTIFICATION_NUM > 0u))
/**
 * @brief         Called by DsdInternal_RxIndication do serviceRequest notification indication
 * @param[in]     protocolId      : current protocol Id
 * @param[in]     notification    : the reuqest notification
 * @param[in]     notificationNum : the number of request notification
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType DsdInternal_HandleIndication(
    uint8                                        protocolId,
    const Dcm_DsdServiceRequestNotificationType* notification,
    uint8                                        notificationNum);
#endif

/**
 * @brief         Called by DsdInternal_RxIndication to check Service condition
 * @param[in]     ConnectionId : current connection Id
 * @param[out]    servIndex    : service configuration index
 * @param[out]    errorCode    : the nrc to send
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType
    DsdInternal_CheckServiceCondition(uint16 ConnectionId, uint16* servIndex, Dcm_NegativeResponseCodeType* errorCode);

/**
 * @brief         Called by DsdInternal_RxIndication to check subService condition
 * @param[in]     protocolId   : current protocol Id
 * @param[in]     servIndex    : the configuration index of service
 * @param[out]    errorCode    : the nrc to send
 * @param[out]    subServIndex : the configuration index
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType DsdInternal_CheckSubServiceCondition(
    uint8                         protocolId,
    uint16                        servIndex,
    Dcm_NegativeResponseCodeType* errorCode,
    uint16*                       subServIndex);

#if (STD_ON == DCM_ENABLE_SECURITY_EVENT_REPORTING)
/**
 * @brief         Called by DsdInternal_SendResponse to report IdsM security events
 * @param[in]     ErrorCode : the nrc to send
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL void DsdInternal_SecurityEventReport(Dcm_NegativeResponseCodeType ErrorCode);
#endif

/**
 * @brief         check service authentication, security and session
 * @param[in]     ConnectionId     : the current connection id
 * @param[in]     DcmDsdServicePtr : the service configuration
 * @param[out]    errorCode        : the possible errorCode
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType DsdInternal_CheckServiceAuth(
    uint16                        ConnectionId,
    const Dcm_DsdServiceType*     DcmDsdServicePtr,
    Dcm_NegativeResponseCodeType* errorCode);

/**
 * @brief         prepare for actual transmission
 * @param[in]     ProtocolId : the current protocol id
 * @param[in]     errorCode  : the input errorCode
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL void DsdInternal_HandleTransmit(uint8 ProtocolId, Dcm_NegativeResponseCodeType errorCode);
/* ========================================== external function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * DSD General condition checks (e.g. security, session, etc.) applied for all services
 */
void DsdInternal_RxIndication(uint8 protocolId)
{
    Dcm_ProtocolCtrlType* protocolCtrlPtr = &Dcm_ProtocolCtrl[protocolId];
    uint16                ConnectionId    = Dcm_ProtocolCtrl[protocolId].ConnectionId;
    Std_ReturnType        furtherProcess  = E_OK;

#if (STD_OFF == DCM_RESPOND_ALL_REQUEST)
    if (0u == (protocolCtrlPtr->Sid & DCM_POSITIVE_RSPMASK))
#endif
    {
        /* Verification of the service */
        uint16                       servIndex = 0u;
        Dcm_NegativeResponseCodeType errorCode = DCM_POS_RESP;
        furtherProcess = DsdInternal_CheckServiceCondition(ConnectionId, &servIndex, &errorCode);

        /* Verification of the SubService */
        uint16              subServIndex  = 0u;
        Dcm_MsgContextType* msgContextPtr = &Dcm_MsgContext[protocolId];
        if (E_OK == furtherProcess)
        {
            protocolCtrlPtr->ServIndex = servIndex;
            if (Dcm_ProtocolCtrl[protocolId].Sid != 0x31u) /* PRQA S 3120 */ /* VL_QAC_MagicNum */
            {
                furtherProcess = DsdInternal_CheckSubServiceCondition(protocolId, servIndex, &errorCode, &subServIndex);
            }
            else
            {
                /* PRQA S 3120++ */ /* VL_QAC_MagicNum */
                msgContextPtr->msgAddInfo.suppressPosResponse = (msgContextPtr->reqData[0u] & DCM_SUPPRESS_BIT) >> 7u;
                /* PRQA S 3120-- */ /* VL_QAC_MagicNum */
                msgContextPtr->reqData[0u] &= (uint8)(~DCM_SUPPRESS_BIT);
            }
        }

        if (E_OK == furtherProcess)
        {
            const Dcm_DsdServiceType* DcmDsdServicePtr = &Dcm_DsdService[servIndex];
#if (STD_ON == DCM_P4_SERVER)
            if (0u != DcmDsdServicePtr->SidTabP4ServerMax)
            {
                protocolCtrlPtr->P4Timer = DcmDsdServicePtr->SidTabP4ServerMax;
            }
#endif
            if (DcmDsdServicePtr->SubfuncAvail)
            {
#if (STD_ON == DCM_P4_SERVER)
                const Dcm_DsdSubServiceType* DcmDsdSubServicePtr = &Dcm_DsdSubService[subServIndex];
                if (0u != DcmDsdSubServicePtr->SubServiceP4ServerMax)
                {
                    protocolCtrlPtr->P4Timer = DcmDsdServicePtr->SidTabP4ServerMax;
                }
#endif
                protocolCtrlPtr->SubServIndex = subServIndex;
            }
            protocolCtrlPtr->OpStatus = DCM_INITIAL;
            protocolCtrlPtr->State    = DCM_MSG_PROCESSING;
        }
        else
        {
            if (DCM_POS_RESP != errorCode)
            {
                DsdInternal_SendResponse(protocolId, errorCode);
            }
        }
    }
#if (STD_OFF == DCM_RESPOND_ALL_REQUEST)
    else
    {
        DslInternal_SetStateIdle(protocolId, NULL_PTR, TRUE, FALSE);
    }
#endif
}

/**
 * asynchronous confirmation handled in mainfunction (involving callout to SWC, shall not be executed in ISR context)
 */
void DsdInternal_HandleConfirmation(uint8 protocolId, Dcm_ConfirmationStatusType confirmationStatus)
{
#if ((DCM_SUPPLIER_NOTIFICATION_NUM != 0u) || (DCM_SUPPLIER_NOTIFICATION_NUM != 0u))
    Std_ReturnType                result;
    const Dcm_DslProtocolRowType* DcmDslProtocolRowPtr = &Dcm_DslProtocolRow[protocolId];
    const Dcm_MsgContextType*     msgContextPtr        = &Dcm_MsgContext[protocolId];
    uint8                         sid                  = Dcm_ProtocolCtrl[protocolId].Sid;
    uint16                        sourceAddress;
    uint16                        connectionId = Dcm_ProtocolCtrl[protocolId].ConnectionId;

    DcmInternal_GetMetaDataSATA(protocolId, &sourceAddress, NULL_PTR);
#endif

#if ((DCM_MANUFACTURER_NOTIFICATION_NUM == 0u) && (DCM_SUPPLIER_NOTIFICATION_NUM == 0u))
    DCM_UNUSED(confirmationStatus);
#endif
#if (DCM_MANUFACTURER_NOTIFICATION_NUM > 0u)
    for (uint8 index = 0u; index < DCM_MANUFACTURER_NOTIFICATION_NUM; index++)
    {
        result = Dcm_DsdServiceRequestManufacturerNotification[index].Confirmation(
            sid,
            msgContextPtr->msgAddInfo.reqType,
            connectionId,
            confirmationStatus,
            DcmDslProtocolRowPtr->ProtocolType,
            sourceAddress);
#if (STD_ON == DCM_DEV_ERROR_DETECT)
        if ((E_OK != result) && (E_NOT_OK == result))
        {
            DCM_DET_REPORT(DCM_TPRXINDICATION_ID, DCM_E_INTERFACE_RETURN_VALUE);
        }
#endif
    }
#endif

#if (DCM_SUPPLIER_NOTIFICATION_NUM > 0u)
    for (uint8 index = 0u; index < DCM_SUPPLIER_NOTIFICATION_NUM; index++)
    {
        result = Dcm_DsdServiceRequestSupplierNotification[index].Confirmation(
            sid,
            msgContextPtr->msgAddInfo.reqType,
            connectionId,
            confirmationStatus,
            DcmDslProtocolRowPtr->ProtocolType,
            DcmDslProtocolRowPtr->EcuAddr);
#if (STD_ON == DCM_DEV_ERROR_DETECT)
        if ((E_OK != result) && (E_NOT_OK == result))
        {
            DCM_DET_REPORT(DCM_TPRXINDICATION_ID, DCM_E_INTERFACE_RETURN_VALUE);
        }
#endif
    }
#endif
}

/**
 * assemble pduInfo, deal with positive/negative response suppression, call DslInternal_ProcessTransmit to synchronously
 * send on targeted application, set flag to asynchronously send on different application
 */
void DsdInternal_SendResponse(uint8 ProtocolId, Dcm_NegativeResponseCodeType errorCode)
{
    Dcm_ProtocolCtrlType* protocolCtrlPtr = &Dcm_ProtocolCtrl[ProtocolId];
    protocolCtrlPtr->ErrorCode            = errorCode;

    if ((DCM_MSG_SENT != protocolCtrlPtr->State) && (DCM_MSG_PENDING_SENT != protocolCtrlPtr->State)
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
        && (DCM_MSG_PAGEDBUFFER_SENT != protocolCtrlPtr->State)
#endif
    )
    {
        uint16                    connectionId  = protocolCtrlPtr->ConnectionId;
        const Dcm_MsgContextType* msgContextPtr = &Dcm_MsgContext[ProtocolId];
        const PduIdType*          protocolTx    = DCM_PROTOCOL_TX(connectionId);

        protocolCtrlPtr->P2Timer = 0u;

        Dcm_ConfirmationStatusType confirmationStatus;
        const Dcm_MsgAddInfoType*  msgAddInfo = &msgContextPtr->msgAddInfo;
        if ((1u == msgAddInfo->reqType) /* PRQA S 2120 */ /* VL_Dcm_2120 */
            && ((DCM_E_SERVICENOTSUPPORTED == errorCode) || (DCM_E_SUBFUNCTIONNOTSUPPORTED == errorCode)
                || (DCM_E_REQUESTOUTOFRANGE == errorCode) || (DCM_E_SUBFUNCTIONNOTSUPPORTEDINACTIVESESSION == errorCode)
                || (DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION == errorCode)))
        {
            confirmationStatus = DCM_RES_NEG_NOT_OK;
            DslInternal_SetStateIdle(ProtocolId, &confirmationStatus, TRUE, TRUE);
        }
        /* PRQA S 2120++ */ /* VL_Dcm_2120 */
        else if (
            (DCM_INVALID_UINT8 != protocolCtrlPtr->ServIndex)
            && (Dcm_DsdService[protocolCtrlPtr->ServIndex].SubfuncAvail) && (1u == msgAddInfo->suppressPosResponse)
            && (DCM_POS_RESP == errorCode))
        /* PRQA S 2120-- */ /* VL_Dcm_2120 */
        {
            confirmationStatus = DCM_RES_POS_NOT_OK;
            DslInternal_SetStateIdle(ProtocolId, &confirmationStatus, TRUE, TRUE);
        }
        else if (NULL_PTR != protocolTx)
        {
            DsdInternal_HandleTransmit(ProtocolId, errorCode);
        }
        else
        {
            confirmationStatus = (DCM_POS_RESP == errorCode) ? DCM_RES_POS_NOT_OK : DCM_RES_NEG_NOT_OK;

            DslInternal_SetStateIdle(ProtocolId, &confirmationStatus, TRUE, TRUE);
        }
    }
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/* ========================================== internal function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/* prepare for transmission */
DCM_LOCAL void DsdInternal_HandleTransmit(uint8 ProtocolId, Dcm_NegativeResponseCodeType errorCode)
{
    Dcm_ProtocolCtrlType*     protocolCtrlPtr = &Dcm_ProtocolCtrl[ProtocolId];
    const Dcm_MsgContextType* msgContextPtr   = &Dcm_MsgContext[ProtocolId];
    uint16                    connectionId    = protocolCtrlPtr->ConnectionId;

    protocolCtrlPtr->ErrorCode = errorCode;
#ifdef DCM_DIAG_RESP_MAX_NUM_RESP_PEND
    if ((DCM_E_RESPONSE_PENDING == errorCode) && (protocolCtrlPtr->PendingCounter == DCM_DIAG_RESP_MAX_NUM_RESP_PEND))
    {
        protocolCtrlPtr->OpStatus  = DCM_CANCEL;
        protocolCtrlPtr->ErrorCode = DCM_E_GENERALREJECT;
        DCM_DET_REPORTRUNTIME(DCM_MAIN_FUNCTION_ID, DCM_E_INTERFACE_TIMEOUT);
    }
#endif
    PduInfoType* pduInfo = &Dcm_TempPduInfo[ProtocolId];
    pduInfo->SduDataPtr  = NULL_PTR;
#if (DCM_METADATA_NUM > 0u)
    const uint8* metaDataPtr = protocolCtrlPtr->MetaDataPtr;
    if (NULL_PTR != metaDataPtr)
    {
/* PRQA S 3120++ */ /* VL_QAC_MagicNum */
#if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
        Dcm_TempMetaData[ProtocolId][0u] = metaDataPtr[1u];
        Dcm_TempMetaData[ProtocolId][1u] = metaDataPtr[0u];
        Dcm_TempMetaData[ProtocolId][2u] = metaDataPtr[3u];
        Dcm_TempMetaData[ProtocolId][3u] = metaDataPtr[2u];
#else
        Dcm_TempMetaData[ProtocolId][0u] = metaDataPtr[2u];
        Dcm_TempMetaData[ProtocolId][1u] = metaDataPtr[3u];
        Dcm_TempMetaData[ProtocolId][2u] = metaDataPtr[0u];
        Dcm_TempMetaData[ProtocolId][3u] = metaDataPtr[1u];
#endif
        /* PRQA S 3120-- */ /* VL_QAC_MagicNum */
        pduInfo->MetaDataPtr = Dcm_TempMetaData[ProtocolId];
    }
    else
#endif
    {
        pduInfo->MetaDataPtr = NULL_PTR;
    }
    if (DCM_POS_RESP != errorCode)
    {
        pduInfo->SduLength = DCM_NEGATIVE_RSPMSGLEN; /* PRQA S 1257 */ /* VL_Dcm_1257 */
#ifdef DCM_NRC22_SPECIFIC_CAUSE_CODE
        if (DCM_E_CONDITIONSNOTCORRECT == errorCode)
        {
            pduInfo->SduLength = DCM_NEGATIVE_RSPMSGLEN + 1u;
        }
#endif
#if (STD_ON == DCM_ENABLE_SECURITY_EVENT_REPORTING)
        DsdInternal_SecurityEventReport(errorCode);
#endif
    }
    else
    {
        pduInfo->SduLength = (PduLengthType)msgContextPtr->resDataLen + 1u;
    }

    if (DCM_COMM_FULL_COMMUNICATION == Dcm_CommState[connectionId])
    {
        protocolCtrlPtr->TxCopyLen = 0u;
#if (STD_ON == DCM_MULTICORE_ENABLED)
        ApplicationType CurrentApplication = GetApplicationID();
        if (CurrentApplication != Dcm_DslMainConnection[connectionId].ApplicationId)
        {
            protocolCtrlPtr->State = DCM_MSG_SENDING;
        }
        else
#endif
        {
            DslInternal_ProcessTransmit(ProtocolId);
        }
    }
    else
    {
        Dcm_ConfirmationStatusType confirmationStatus =
            (DCM_POS_RESP == errorCode) ? DCM_RES_POS_NOT_OK : DCM_RES_NEG_NOT_OK;
        DslInternal_SetStateIdle(ProtocolId, &confirmationStatus, TRUE, FALSE);
    }
}

/**
 * iterate over servicetable to check whether the requested service is configured and set the configuration index
 */
Std_ReturnType DsdInternal_CheckService(uint8 protocolId, uint16* servIndex, const uint8* Sid)
{
    Std_ReturnType furtherProcess = E_NOT_OK;
    uint8          sid            = (Sid == NULL_PTR) ? Dcm_ProtocolCtrl[protocolId].Sid : *Sid;

    for (uint16 index = DCM_SERVICETABLE(protocolId)->ServiceIndex;
         index < (DCM_SERVICETABLE(protocolId)->ServiceIndex + DCM_SERVICETABLE(protocolId)->ServiceNum);
         index++)
    {
        /* PRQA S 4342 ++ */ /* VL_Dcm_4342 */
        if ((Dcm_CfgPtr->ServiceUsed[index]) && (sid == Dcm_DsdService[index].SidTabServiceId)
#if (STD_ON == DCM_ADDRESS_FORMAT)
            && ((DCM_PHYANDFUNC == Dcm_DsdService[index].SidTabAddressingFormat)
                || ((Dcm_DcmDsdAddressingFormatType)Dcm_MsgContext[protocolId].msgAddInfo.reqType
                    == Dcm_DsdService[index].SidTabAddressingFormat))
#endif
        )
        {
            *servIndex     = index;
            furtherProcess = E_OK;
            break;
        }
        /* PRQA S 4342 -- */ /* VL_Dcm_4342 */
    }
    if ((E_NOT_OK == furtherProcess) && (NULL_PTR == Sid))
    {
        DsdInternal_SendResponse(protocolId, DCM_E_SERVICENOTSUPPORTED);
    }

    return furtherProcess;
}

/**
 * iterate over servicetable to check whether the requested subService is configured and set the configuration index
 */
Std_ReturnType DsdInternal_CheckSubService(
    uint8                         protocolId,
    uint16                        servIndex,
    uint16*                       subServIndex,
    const uint8*                  SubServiceId,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType      furtherProcess = E_NOT_OK;
    Dcm_MsgContextType* msgContextPtr;
    uint8               subServiceId;

    if (NULL_PTR == SubServiceId)
    {
        msgContextPtr = &Dcm_MsgContext[protocolId];

        if (msgContextPtr->reqDataLen >= 1u) /* PRQA S 1252 */ /* VL_Dcm_1252 */
        {
            subServiceId   = msgContextPtr->reqData[0u] & DCM_SUBFUNCTION_MASK;
            furtherProcess = E_OK;
        }
        else
        {
            *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        }
    }
    else
    {
        subServiceId   = *SubServiceId;
        furtherProcess = E_OK;
    }

    if (E_OK == furtherProcess)
    {
        furtherProcess = E_NOT_OK;
        if (NULL_PTR == SubServiceId)
        {
            /* PRQA S 3120++ */ /* VL_QAC_MagicNum */
            msgContextPtr->msgAddInfo.suppressPosResponse = (msgContextPtr->reqData[0u] & DCM_SUPPRESS_BIT) >> 7u;
            /* PRQA S 3120-- */ /* VL_QAC_MagicNum */
            msgContextPtr->reqData[0u] &= (uint8)(~DCM_SUPPRESS_BIT);
        }

        for (uint16 index = Dcm_DsdService[servIndex].SubServiceIndex;
             index < (Dcm_DsdService[servIndex].SubServiceIndex + Dcm_DsdService[servIndex].SubServiceNum);
             index++)
        {
            /* PRQA S 4342 ++ */ /* VL_Dcm_4342 */
            if ((Dcm_CfgPtr->SubServiceUsed[index]) && (subServiceId == Dcm_DsdSubService[index].SubServiceId)
#if (STD_ON == DCM_ADDRESS_FORMAT)
                && ((DCM_PHYANDFUNC == Dcm_DsdSubService[index].SubServiceAddressingFormat)
                    || ((Dcm_DcmDsdAddressingFormatType)Dcm_MsgContext[protocolId].msgAddInfo.reqType
                        == Dcm_DsdSubService[index].SubServiceAddressingFormat))
#endif
            )
            {
                *subServIndex  = index;
                furtherProcess = E_OK;
                break;
            }
            /* PRQA S 4342 -- */ /* VL_Dcm_4342 */
        }
        if ((E_NOT_OK == furtherProcess) && (NULL_PTR == SubServiceId))
        {
            *ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTED;
        }
    }

    return furtherProcess;
}

#if ((DCM_SUPPLIER_NOTIFICATION_NUM > 0u) || (DCM_SUPPLIER_NOTIFICATION_NUM > 0u))
/**
 * iterate over requestNotification(supplier/Manufacturer) to call indication
 */
DCM_LOCAL Std_ReturnType DsdInternal_HandleIndication(
    uint8                                        protocolId,
    const Dcm_DsdServiceRequestNotificationType* notification,
    uint8                                        notificationNum)
{
    boolean                       indicationIgnore     = FALSE;
    Std_ReturnType                furtherProcess       = E_OK;
    Dcm_NegativeResponseCodeType  firstErrorCode       = DCM_POS_RESP;
    const Dcm_DslProtocolRowType* DcmDslProtocolRowPtr = &Dcm_DslProtocolRow[protocolId];
    const Dcm_MsgContextType*     msgContextPtr        = &Dcm_MsgContext[protocolId];
    uint16                        sourceAddress;

    DcmInternal_GetMetaDataSATA(protocolId, &sourceAddress, NULL_PTR);

    for (uint8 index = 0u; index < notificationNum; index++)
    {
        Dcm_NegativeResponseCodeType errorCode;
        Std_ReturnType               result = notification[index].Indication(
            Dcm_ProtocolCtrl[protocolId].Sid,
            msgContextPtr->reqData,
            msgContextPtr->reqDataLen,
            msgContextPtr->msgAddInfo.reqType,
            Dcm_ProtocolCtrl[protocolId].ConnectionId,
            &errorCode,
            DcmDslProtocolRowPtr->ProtocolType,
            sourceAddress);
        if (E_REQUEST_NOT_ACCEPTED == result)
        {
            furtherProcess   = E_NOT_OK;
            indicationIgnore = TRUE;
            break;
        }
        else if (E_NOT_OK == result)
        {
            furtherProcess = E_NOT_OK;
            firstErrorCode = (DCM_POS_RESP == firstErrorCode) ? errorCode : firstErrorCode;
        }
#if (STD_ON == DCM_DEV_ERROR_DETECT)
        else if (E_OK != result)
        {
            firstErrorCode = (DCM_POS_RESP == firstErrorCode) ? DCM_E_GENERALREJECT : firstErrorCode;
            furtherProcess = E_NOT_OK;
            DCM_DET_REPORT(DCM_TPRXINDICATION_ID, DCM_E_INTERFACE_RETURN_VALUE);
        }
#endif
        else
        {
            /* idle */
        }
    }

    if (E_NOT_OK == furtherProcess)
    {
        if (!indicationIgnore)
        {
            DsdInternal_SendResponse(protocolId, firstErrorCode);
        }
        else
        {
            Dcm_ConfirmationStatusType confirmationStatus = DCM_RES_NEG_NOT_OK;
            DslInternal_SetStateIdle(protocolId, &confirmationStatus, TRUE, FALSE);
        }
    }

    return furtherProcess;
}
#endif
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#if (STD_ON == DCM_ENABLE_SECURITY_EVENT_REPORTING)
DCM_LOCAL void DsdInternal_SecurityEventReport(Dcm_NegativeResponseCodeType ErrorCode)
{
    switch (ErrorCode) /* PRQA S 3315 */ /* VL_Dcm_3315 */
    {
#ifdef SEV_UDS_SECURITY_ACCESS_DENIED
    case DCM_E_SECURITYACCESSDENIED:
    {
        IdsM_SetSecurityEvent(SEV_UDS_SECURITY_ACCESS_DENIED);
        break;
    }
#endif
#ifdef SEV_UDS_SERVICE_NOT_SUPPORTED
    case DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION:
    {
        IdsM_SetSecurityEvent(SEV_UDS_SERVICE_NOT_SUPPORTED);
        break;
    }
#endif
#ifdef SEV_UDS_SUBFUNCTION_NOT_SUPPORTED
    case DCM_E_SUBFUNCTIONNOTSUPPORTED:
    {
        IdsM_SetSecurityEvent(SEV_UDS_SUBFUNCTION_NOT_SUPPORTED);
        break;
    }
#endif
#ifdef SEV_UDS_INCORRECT_MESSAGE_LENGTH_OR_FORMAT
    case DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT:
    {
        IdsM_SetSecurityEvent(SEV_UDS_INCORRECT_MESSAGE_LENGTH_OR_FORMAT);
        break;
    }
#endif
#ifdef SEV_UDS_REQUEST_SEQUENCE_ERROR
    case DCM_E_REQUESTSEQUENCEERROR:
    {
        IdsM_SetSecurityEvent(SEV_UDS_REQUEST_SEQUENCE_ERROR);
        break;
    }
#endif
#ifdef SEV_UDS_REQUEST_OUT_OF_RANGE
    case DCM_E_REQUESTOUTOFRANGE:
    {
        IdsM_SetSecurityEvent(SEV_UDS_REQUEST_OUT_OF_RANGE);
        break;
    }
#endif
#ifdef SEV_UDS_REQUESTED_ACTIONS_REQUIRES_AUTHENTICATION
    case DCM_E_AUTHENTICATIONREQUIRED:
    {
        IdsM_SetSecurityEvent(SEV_UDS_REQUESTED_ACTIONS_REQUIRES_AUTHENTICATION);
        break;
    }
#endif
#ifdef SEV_UDS_SECURITY_ACCESS_NUMBER_OF_ATTEMPTS_EXCEEDED
    case DCM_E_EXCEEDNUMBEROFATTEMPTS:
    {
        IdsM_SetSecurityEvent(SEV_UDS_SECURITY_ACCESS_NUMBER_OF_ATTEMPTS_EXCEEDED);
        break;
    }
#endif
#ifdef SEV_UDS_SECURITY_ACCESS_INVALID_KEY
    case DCM_E_INVALIDKEY:
    {
        IdsM_SetSecurityEvent(SEV_UDS_SECURITY_ACCESS_INVALID_KEY);
        break;
    }
#endif
#ifdef SEV_UDS_SECURITY_ACCESS_REQUIRED_TIME_DELAY_NOT_EXPIRED
    case DCM_E_REQUIREDTIMEDELAYNOTEXPIRED:
    {
        IdsM_SetSecurityEvent(SEV_UDS_SECURITY_ACCESS_REQUIRED_TIME_DELAY_NOT_EXPIRED);
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
#endif

/* do general condition check for services */
DCM_LOCAL Std_ReturnType
    DsdInternal_CheckServiceCondition(uint16 ConnectionId, uint16* servIndex, Dcm_NegativeResponseCodeType* errorCode)
{
    uint8          protocolId     = Dcm_DslMainConnection[ConnectionId].ParentProtocolId;
    Std_ReturnType furtherProcess = E_OK;
#if (DCM_MANUFACTURER_NOTIFICATION_NUM > 0u)
    /* 1. Verification of Manufacturer permission */
    furtherProcess = DsdInternal_HandleIndication(
        protocolId,
        Dcm_DsdServiceRequestManufacturerNotification,
        DCM_MANUFACTURER_NOTIFICATION_NUM);
#endif

    /* 2. Verification of the SID */
    const Dcm_DsdServiceType* DcmDsdServicePtr;
#if (DCM_MANUFACTURER_NOTIFICATION_NUM > 0u)
    if (E_OK == furtherProcess)
#endif
    {
        furtherProcess = DsdInternal_CheckService(protocolId, servIndex, NULL_PTR);
        if (E_OK == furtherProcess)
        {
            DcmDsdServicePtr = &Dcm_DsdService[*servIndex];
        }
    }

    if (E_OK == furtherProcess)
    {
        furtherProcess = DsdInternal_CheckServiceAuth(ConnectionId, DcmDsdServicePtr, errorCode);
    }

#if (DCM_SUPPLIER_NOTIFICATION_NUM > 0u)
    /* 6. Verification of the Supplier permission */
    if (E_OK == furtherProcess)
    {
        furtherProcess = DsdInternal_HandleIndication(
            protocolId,
            Dcm_DsdServiceRequestSupplierNotification,
            DCM_SUPPLIER_NOTIFICATION_NUM);
    }
#endif

    /* 7. Verification of the Mode Rules */
    if ((E_OK == furtherProcess) && (NULL_PTR != DcmDsdServicePtr->SidTabModeRuleRef))
    {
        furtherProcess = DcmDsdServicePtr->SidTabModeRuleRef(errorCode);
    }

    return furtherProcess;
}

DCM_LOCAL Std_ReturnType DsdInternal_CheckServiceAuth(
    uint16                        ConnectionId,
    const Dcm_DsdServiceType*     DcmDsdServicePtr,
    Dcm_NegativeResponseCodeType* errorCode)
{
    Std_ReturnType furtherProcess = E_OK;
#if (STD_ON != DCM_UDS_0X29)
    DCM_UNUSED(ConnectionId);
#else
    /* 3. Verification of the service access control on the current authentication State */
    if (DcmDsdServicePtr->SidTabServiceId >= DCM_UDS_SERVID_START)
    {
        furtherProcess = Dcm_UDS0x29_AuthenticationCheck(
            ConnectionId,
            DcmDsdServicePtr->ServiceRoleRef,
            DcmDsdServicePtr->ServiceRoleRefNum,
            errorCode);
    }

    /* 4. Verification of the Diagnostic Session */
    if (E_OK == furtherProcess)
#endif
    {
        furtherProcess = Dcm_UDS0x10_CheckSession(
            DcmDsdServicePtr->SidTabSessionLevelRef,
            DcmDsdServicePtr->SidTabSessionLevelRefNum,
            NULL_PTR);
        if (E_NOT_OK == furtherProcess)
        {
            *errorCode = DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION;
        }
    }

#if (STD_ON == DCM_UDS_0X27)
    /* 5. Verification of the Service Security Access levels */
    if (E_OK == furtherProcess)
    {
        furtherProcess = Dcm_UDS0x27_CheckSecurity(
            DcmDsdServicePtr->SidTabSecurityLevelRef,
            DcmDsdServicePtr->SidTabSecurityLevelRefNum,
            errorCode);
    }
#endif

    return furtherProcess;
}

/* do general condition check for subServices */
DCM_LOCAL Std_ReturnType DsdInternal_CheckSubServiceCondition(
    uint8                         protocolId,
    uint16                        servIndex,
    Dcm_NegativeResponseCodeType* errorCode,
    uint16*                       subServIndex)
{
    const Dcm_DsdSubServiceType* DcmDsdSubServicePtr;
    Std_ReturnType               furtherProcess = E_OK;

    if (Dcm_DsdService[servIndex].SubfuncAvail)
    {
        /* 8. Verification of the SubSID */
        furtherProcess = DsdInternal_CheckSubService(protocolId, servIndex, subServIndex, NULL_PTR, errorCode);
        if (E_OK == furtherProcess)
        {
            DcmDsdSubServicePtr = &Dcm_DsdSubService[*subServIndex];
        }

#if (STD_ON == DCM_UDS_0X29)
        /* 9. Verification of the service access control on the current authentication State for SubServiceId */
        if ((E_OK == furtherProcess) && (Dcm_DsdService[servIndex].SidTabServiceId >= DCM_UDS_SERVID_START))
        {
            furtherProcess = Dcm_UDS0x29_AuthenticationCheck(
                Dcm_ProtocolCtrl[protocolId].ConnectionId,
                DcmDsdSubServicePtr->SubServiceRoleRef,
                DcmDsdSubServicePtr->SubServiceRoleRefNum,
                errorCode);
        }
#endif

        /* 10. Verification of the Diagnostic Session for SubServiceId */
        if (E_OK == furtherProcess)
        {
            furtherProcess = Dcm_UDS0x10_CheckSession(
                DcmDsdSubServicePtr->SubServiceSessionLevelRef,
                DcmDsdSubServicePtr->SubServiceSessionLevelRefNum,
                NULL_PTR);
            if (E_NOT_OK == furtherProcess)
            {
                *errorCode = DCM_E_SUBFUNCTIONNOTSUPPORTEDINACTIVESESSION;
            }
        }

#if (STD_ON == DCM_UDS_0X27)
        /* 11. Verification of the Service Security Access levels of SubSID */
        if (E_OK == furtherProcess)
        {
            furtherProcess = Dcm_UDS0x27_CheckSecurity(
                DcmDsdSubServicePtr->SubServiceSecurityLevelRef,
                DcmDsdSubServicePtr->SubServiceSecurityLevelRefNum,
                errorCode);
        }
#endif

        /* 12. Verification of the Mode rules */
        if ((E_OK == furtherProcess) && (NULL_PTR != DcmDsdSubServicePtr->SubServiceModeRuleRef))
        {
            furtherProcess = DcmDsdSubServicePtr->SubServiceModeRuleRef(errorCode);
        }
    }

    return furtherProcess;
}
