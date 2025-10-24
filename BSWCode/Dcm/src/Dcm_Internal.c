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
 **  @file               : Dcm_Internal.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm Private Functions/Variables Definitions
 **
 ***********************************************************************************************************************/
/* PRQA S 1532, 3120, 1503 EOF */ /* VL_QAC_OneFunRef, VL_QAC_MagicNum, VL_QAC_NoUsedApi */
/* =================================================== inclusions =================================================== */
#include "PduR_Dcm.h"
#include "Dcm_Internal.h"
#if (STD_ON == DCM_USE_BSWM)
#include "BswM_Dcm.h" /* PRQA S 0380 */ /* VL_Dcm_0380 */
#endif
#ifdef DCM_MEMCOPY_USED_ILIB
#include "IStdLib.h"
#endif
/* ============================================ external data definitions =========================================== */
/**
 * @brief the programing condition (communicating with bootloader)
 */
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
Dcm_ProgConditionsType Dcm_ProgConditions;
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"

/**
 * @brief the config pointer
 */
#define DCM_START_SEC_VAR_INIT_PTR
#include "Dcm_MemMap.h"
const Dcm_ConfigType* Dcm_CfgPtr = NULL_PTR;
#define DCM_STOP_SEC_VAR_INIT_PTR
#include "Dcm_MemMap.h"
/* ========================================== internal function declarations ======================================== */
/**
 * @brief         called to check P2Timer
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL void DcmInternal_CheckP2Timer(void);
#if (STD_ON == DCM_P4_SERVER)
/**
 * @brief         called to check P4Timer
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL void DcmInternal_CheckP4Timer(void);
#endif
/**
 * @brief         This function process protocol preemption
 * @param[in]     curProtocolId    : the current running protocol Id
 * @param[in]     targetProtocolId : the preempted protocol id
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType DslInternal_ProcessPreemption(uint8 curProtocolId, uint8 targetProtocolId);

#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
/**
 * @brief         This function process queueud request
 * @param[in]     protocolId : the input protocol Id
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL void DcmInternal_ProcessQueuedRequest(uint8 protocolId);
#endif

/**
 * @brief         This function calls service interpreter
 * @param[in]     protocolId : the input protocol Id
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL void DcmInternal_DispatchService(uint8 protocolId);

/**
 * @brief         This function handles preemption
 * @param[in]     protocolId : the input protocol Id
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType DcmInternal_HandlePreemption(uint8 protocolId);

#if (DCM_CALLBACK_DCM_REQUEST_SERVICE > 0u)
/**
 * @brief         This function handles request callbacks
 * @param[in]     protocolId : the input protocol Id
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType DcmInternal_HandleRequestCallback(uint8 protocolId);
#endif

#if ((DCM_MANUFACTURER_NOTIFICATION_NUM > 0) || (DCM_SUPPLIER_NOTIFICATION_NUM > 0))
/**
 * @brief         This function handles manufacturer/supplier notification confirmation
 * @param[in]     protocolId      : the input protocol Id
 * @param[in]     notification    : the notification container
 * @param[in]     notificationNum : the number of notification container
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL void DcmInternal_HandleConfirmation(
    uint8                                        protocolId,
    const Dcm_DsdServiceRequestNotificationType* notification,
    uint8                                        notificationNum);
#endif

/**
 * @brief         This function deal with service function return value
 * @param[in]     result     : the service result
 * @param[in]     protocolId : the current protocol id
 * @param[in]     errorCode  : nrc returned by service
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL void
    DcmInternal_HandleServiceReturn(Std_ReturnType result, uint8 protocolId, Dcm_NegativeResponseCodeType errorCode);

/**
 * @brief         This function setup pre-conditions for rxIndication
 * @param[in]     protocolId : the current protocol id
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType DcmInternal_PrepareRxIndication(uint8 protocolId);
/* ========================================== external function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
#ifndef DCM_MEMCOPY_USED_ILIB
/**
 * Dcm memory copy implementation
 */
void DcmInternal_Memcpy(uint8* dest, const uint8* src, uint32 size)
{
    for (uint32 index = 0u; index < size; index++)
    {
        dest[index] = src[index];
    }
}

/**
 * Dcm memory set implementation
 */
void DcmInternal_Memset(uint8* dest, uint8 data, uint32 size)
{
    for (uint32 index = 0u; index < size; index++)
    {
        dest[index] = data;
    }
}
/**
 * Dcm  Compares two blocks of memory byte by byte
 */
int DcmInternal_MemCmp(const void* ptr1, const void* ptr2, int num)
{
    const uint8* p1 = (const uint8*)ptr1;
    const uint8* p2 = (const uint8*)ptr2;
    for (int i = 0; i < num; i++)
    {
        if (p1[i] != p2[i])
        {
            return p1[i] - p2[i];
        }
    }
    return 0u;
}

#else
/* API mapping for memcpy/memset */
#define DcmInternal_Memcpy(dest, src, size)  IStdLib_MemCpy(dest, src, size)
#define DcmInternal_Memset(dest, data, size) IStdLib_MemSet(dest, data, size)
#define DcmInternal_MemCmp(dest, data, size) IStdLib_MemCmp(dest, data, size)
#endif

/**
 * get metadata source and target address
 */
void DcmInternal_GetMetaDataSATA(uint8 protocolId, uint16* sourceAddress, uint16* targetAddress)
{
    const Dcm_ProtocolCtrlType* protocolCtrlPtr = &Dcm_ProtocolCtrl[protocolId];
#if (0u == DCM_METADATA_NUM)
    DCM_UNUSED(targetAddress);
#endif

    if (protocolCtrlPtr->RxPduId < DCM_PROTOCOLRX_NUM)
    {
#if (0u != DCM_METADATA_NUM)
        Std_ReturnType result = E_NOT_OK;
        /* retrieve metadata from pduinfo only for generic connection */
        if (Dcm_DslMainConnection[Dcm_DslProtocolRx[protocolCtrlPtr->RxPduId].ParentMainConnectionId]
                .MainConnectionUseGenericConnection)
        {
            result = E_OK;
            if (NULL_PTR != sourceAddress)
            {
#if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
                *sourceAddress =
                    (uint16)((uint16)protocolCtrlPtr->MetaDataPtr[3u] << 8u) | (uint16)protocolCtrlPtr->MetaDataPtr[2u];
#else
                *sourceAddress =
                    (uint16)((uint16)protocolCtrlPtr->MetaDataPtr[0u] << 8u) | (uint16)protocolCtrlPtr->MetaDataPtr[1u];
#endif
            }
            if (NULL_PTR != targetAddress)
            {
#if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
                *targetAddress =
                    (uint16)((uint16)protocolCtrlPtr->MetaDataPtr[1u] << 8u) | (uint16)protocolCtrlPtr->MetaDataPtr[0u];
#else
                *targetAddress =
                    (uint16)((uint16)protocolCtrlPtr->MetaDataPtr[2u] << 8u) | (uint16)protocolCtrlPtr->MetaDataPtr[3u];
#endif
            }
        }
        /* retrieve from configuration if not generic connection */
        if (E_NOT_OK == result)
#endif
        {
            if (NULL_PTR != sourceAddress)
            {
                *sourceAddress =
                    Dcm_DslMainConnection[Dcm_DslProtocolRx[protocolCtrlPtr->RxPduId].ParentMainConnectionId]
                        .TesterSourceAddr;
            }
        }
    }
}

/**
 * decrease timer, if timeout, set to 0
 */
boolean DcmInternal_DecreaseTimer(uint32* timer)
{
    boolean timeout = FALSE;

    if (0uL != *timer)
    {
        if (*timer <= DCM_MAINFUNCTION_PERIOD)
        {
            *timer  = 0u;
            timeout = TRUE;
        }
        else
        {
            *timer -= DCM_MAINFUNCTION_PERIOD;
        }
    }

    return timeout;
}

/**
 * check timer for 0x10/0x27/0x86, P2 and P4
 */
void DcmInternal_CheckTimer(void)
{
    Dcm_UDS0x10_CheckTimer();
#if (STD_ON == DCM_UDS_0X27)
    Dcm_UDS0x27_CheckTimer();
#endif
#if (STD_ON == DCM_UDS_0X86)
    Dcm_UDS0x86_CheckTimer();
#endif
#if (STD_ON == DCM_P4_SERVER)
    DcmInternal_CheckP4Timer();
#endif
    DcmInternal_CheckP2Timer();
}

/**
 * cancel running jobs of target protocol, cancel transmit/receive if in the process
 */
Std_ReturnType DcmInternal_CancelJobs(uint8 ProtocolId)
{
    Std_ReturnType        result          = E_OK;
    Dcm_ProtocolCtrlType* protocolCtrlPtr = &Dcm_ProtocolCtrl[ProtocolId];
    uint16                connectionId    = protocolCtrlPtr->ConnectionId;
    Dcm_StateType         currentState    = protocolCtrlPtr->State;

    /* only process for non-idle protocol */
    if (DCM_MSG_IDLE != currentState)
    {
        /* cancel ongoing reception/transmission */
        if (((DCM_MSG_SENT == currentState) || (DCM_MSG_PENDING_SENT == currentState))
            && (NULL_PTR != DCM_PROTOCOL_TX(connectionId)))
        {
            DslInternal_SetStateIdle(ProtocolId, NULL_PTR, TRUE, FALSE);
            result = PduR_DcmCancelTransmit(*DCM_PROTOCOL_TX(connectionId));
        }
        else if (DCM_MSG_RECEIVING == currentState)
        {
            result = PduR_DcmCancelReceive(Dcm_MsgContext[ProtocolId].dcmRxPduId);
        }
        else if (DCM_PENDING == protocolCtrlPtr->OpStatus)
        {
            /* during pending process, notify servcie of cancellation */
            protocolCtrlPtr->OpStatus = DCM_CANCEL;
        }
        else
        {
            /* idle */
        }
        if (DCM_CANCEL != protocolCtrlPtr->OpStatus)
        {
            DslInternal_SetStateIdle(ProtocolId, NULL_PTR, TRUE, FALSE);
        }
    }

    return result;
}

/**
 * asynchronously calls service process function
 */
void DcmInternal_ProcessRequest(void)
{
    for (uint8 index = 0u; index < DCM_PROTOCOLROW_NUM; index++)
    {
#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
        DcmInternal_ProcessQueuedRequest(index);
#endif
#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)

#endif
        const Dcm_ProtocolCtrlType* protocolCtrlPtr = &Dcm_ProtocolCtrl[index];
        /* only process when there is ongoing request */
        if ((DCM_MSG_PROCESSING == protocolCtrlPtr->State)
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
            || (DCM_MSG_PAGEDBUFFER_SENT == protocolCtrlPtr->State)
#endif
            || (DCM_CANCEL == protocolCtrlPtr->OpStatus))
        {
            DcmInternal_DispatchService(index);
        }
    }
}

/**
 * find session index based on session level
 */
Std_ReturnType DcmInternal_FindSessionIndex(Dcm_SesCtrlType session, uint8* sessionIndex)
{
    Std_ReturnType result = E_NOT_OK;

    for (uint8 index = 0u; index < DCM_SESSION_NUM; index++)
    {
        if (Dcm_DspSessionRow[index].SessionLevel == session)
        {
            *sessionIndex = index;
            result        = E_OK;
            break;
        }
    }

    return result;
}

#if (STD_ON == DCM_UDS_0X27)
/**
 * find security index based on security level
 */
void DcmInternal_FindSecurityIndex(Dcm_SecLevelType security, uint8* securityIndex)
{
    for (uint8 index = 0u; index < DCM_SECURITY_NUM; index++)
    {
        if (Dcm_DspSecurityRow[index].SecurityLevel == security)
        {
            *securityIndex = index;
            break;
        }
    }
}
#endif

/**
 * calls Dcm_SetProgConditions to prepare for reset
 */
Std_ReturnType DcmInternal_SetProgConditions(Dcm_OpStatusType OpStatus)
{
    Std_ReturnType result = Dcm_SetProgConditions(OpStatus, &Dcm_ProgConditions);
    /* only continue processing when initial or pending */
    if ((Dcm_OpStatusType)DCM_CANCEL != OpStatus)
    {
        if (E_OK == result)
        {
            /* setProgCondition succeed, notify via mode switch interface, enter no com mode to avoid further
             * communication */
            Dcm_ProcessingSetProg = FALSE;
            (void)Rte_Switch_EcuResetModeSwitchInterface_ecuReset(RTE_MODE_DcmEcuReset_DCM_EXECUTE);
            for (uint16 index = 0u; index < DCM_MAINCONNECTION_NUM; index++)
            {
                Dcm_CommState[index] = DCM_COMM_NO_COMMUNICATION;
            }
        }
        else if ((DCM_E_PENDING == result) || (DCM_E_FORCE_RCRRP == result))
        {
            Dcm_ProcessingSetProg = TRUE;
        }
#if (STD_ON == DCM_DEV_ERROR_DETECT)
        else if (E_NOT_OK != result)
        {
            DCM_DET_REPORT(DCM_MAIN_FUNCTION_ID, DCM_E_INTERFACE_RETURN_VALUE);
            result = E_NOT_OK;
        }
#endif
        else
        {
            Dcm_ProcessingSetProg = FALSE;
        }
    }

#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if (E_NOT_OK == result)
    {
        DCM_DET_REPORT(DCM_MAIN_FUNCTION_ID, DCM_E_SET_PROG_CONDITIONS_FAIL);
    }
#endif

    return result;
}

/**
 * calls Dcm_GetProgConditions during intiailization to check app update status
 */
void DcmInternal_ProgCtrlInit(void)
{
    /* warm start means the ECU starts from a boorloader jump, thus need to evaluate ApplUpDataed */
    if (DCM_WARM_START == Dcm_GetProgConditions(&Dcm_ProgConditions))
    {
        DslInternal_SetDiagnosticState(Dcm_ProgConditions.ConnectionId, DCM_ACTIVE);
#if (STD_ON == DCM_USE_BSWM)
        if (Dcm_ProgConditions.ApplUpdated)
        {
            BswM_Dcm_ApplicationUpdated();
        }
#endif
    }
}

/**
 * transform uint16 data to uint8 array
 */
void DcmInternal_TransformArray_u16(uint8* dest, uint16 src, Dcm_EndiannessType endianness)
{
    if ((endianness == DCM_BIG_ENDIAN) || (endianness == DCM_OPAQUE))
    {
        for (uint8 index = 0u; index < 2u; index++)
        {
            dest[index] = (uint8)(src >> ((2u - index - 1u) * 8u));
        }
    }
    else
    {
        for (uint8 index = 0u; index < 2u; index++)
        {
            dest[index] = (uint8)(src >> (index * 8u));
        }
    }
}

/**
 * transform uint24 data to uint8 array
 */
void DcmInternal_TransformArray_u24(uint8* dest, uint32 src, Dcm_EndiannessType endianness)
{
    if ((endianness == DCM_BIG_ENDIAN) || (endianness == DCM_OPAQUE))
    {
        for (uint8 index = 0u; index < 3u; index++)
        {
            dest[index] = (uint8)(src >> ((3u - index - 1u) * 8u));
        }
    }
    else
    {
        for (uint8 index = 0u; index < 3u; index++)
        {
            dest[index] = (uint8)(src >> (index * 8u));
        }
    }
}

/**
 * transform uint32 data to uint8 array
 */
void DcmInternal_TransformArray_u32(uint8* dest, uint32 src, Dcm_EndiannessType endianness)
{
    if ((endianness == DCM_BIG_ENDIAN) || (endianness == DCM_OPAQUE))
    {
        for (uint8 index = 0u; index < 4u; index++)
        {
            dest[index] = (uint8)(src >> ((4u - index - 1u) * 8u));
        }
    }
    else
    {
        for (uint8 index = 0u; index < 4u; index++)
        {
            dest[index] = (uint8)(src >> (index * 8u));
        }
    }
}

/**
 * transform uint64 data to uint8 array
 */
void DcmInternal_TransformArray_u64(uint8* dest, uint64 src, Dcm_EndiannessType endianness)
{
    if ((endianness == DCM_BIG_ENDIAN) || (endianness == DCM_OPAQUE))
    {
        for (uint8 index = 0u; index < 8u; index++)
        {
            dest[index] = (uint8)(src >> ((8u - index - 1u) * 8u));
        }
    }
    else
    {
        for (uint8 index = 0u; index < 8u; index++)
        {
            dest[index] = (uint8)(src >> (index * 8u));
        }
    }
}

/**
 * asynchronously process rxindication to avoid callout in ISR context
 */
void DcmInternal_ProcessRxIndication(void)
{
    for (uint8 index = 0u; index < DCM_PROTOCOLROW_NUM; index++)
    {
#if (STD_ON == DCM_UDS_0X86)
        const Dcm_DslProtocolRowType* DcmDslProtocolRowPtr = &Dcm_DslProtocolRow[index];
#endif
        const Dcm_ProtocolCtrlType* protocolCtrlPtr = &Dcm_ProtocolCtrl[index];
        if (DCM_MSG_RECEIVED == protocolCtrlPtr->State)
        {
            Std_ReturnType result = E_OK;
#if (STD_ON == DCM_UDS_0X86)
            if (((Dcm_ProtocolType)DCM_ROE_ON_CAN != DcmDslProtocolRowPtr->ProtocolType)
                && ((Dcm_ProtocolType)DCM_ROE_ON_FLEXRAY != DcmDslProtocolRowPtr->ProtocolType)
                && ((Dcm_ProtocolType)DCM_ROE_ON_IP != DcmDslProtocolRowPtr->ProtocolType))
#endif
            {
                result = DcmInternal_PrepareRxIndication(index);
            }

            if (E_OK == result)
            {
                DsdInternal_RxIndication(index);
            }
            else if (DCM_MSG_SENT != protocolCtrlPtr->State)
            {
                DslInternal_SetStateIdle(index, NULL_PTR, TRUE, FALSE);
            }
            else
            {
                /* idle */
            }
        }
    }
}

/**
 * asynchronously process txconfirmation to avoid callout in ISR context
 */
void DcmInternal_ProcessTxConfirmation(void)
{
    for (uint8 index = 0u; index < DCM_PROTOCOLROW_NUM; index++)
    {
        Dcm_ConfirmationHandleType* confirmationInfo = &Dcm_ConfirmationHandle[index];
        if (confirmationInfo->Pending)
        {
            /* handle using the same method with different input */
#if (DCM_MANUFACTURER_NOTIFICATION_NUM > 0)
            DcmInternal_HandleConfirmation(
                index,
                Dcm_DsdServiceRequestManufacturerNotification,
                DCM_MANUFACTURER_NOTIFICATION_NUM);
#endif
#if (DCM_SUPPLIER_NOTIFICATION_NUM > 0u)
            DcmInternal_HandleConfirmation(
                index,
                Dcm_DsdServiceRequestSupplierNotification,
                DCM_SUPPLIER_NOTIFICATION_NUM);
#endif
            confirmationInfo->Pending = FALSE;
        }
    }
}

/**
 * set diagnostic state for all mainConnection
 */
void DcmInternal_SetActiveDiagnostic(boolean active)
{
    Dcm_DiagnosticStateType state = (active) ? DCM_ACTIVE : DCM_INACTIVE;

    for (uint16 index = 0u; index < DCM_MAINCONNECTION_NUM; index++)
    {
        DslInternal_SetDiagnosticState(index, state);
    }
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/* ========================================== internal function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * check P2 timer, send nrc 0x78 when needed
 */
DCM_LOCAL void DcmInternal_CheckP2Timer(void)
{
    for (uint8 index = 0u; index < DCM_PROTOCOLROW_NUM; index++)
    {
        if ((DcmInternal_DecreaseTimer(&Dcm_ProtocolCtrl[index].P2Timer))
            && (DCM_MSG_PROCESSING == Dcm_ProtocolCtrl[index].State))
        {
            DsdInternal_SendResponse(index, DCM_E_RESPONSE_PENDING);
        }
    }
}

#if (STD_ON == DCM_P4_SERVER)
/**
 * check P4 timer, reset protocol state if timeout
 */
DCM_LOCAL void DcmInternal_CheckP4Timer(void)
{
    for (uint8 index = 0u; index < DCM_PROTOCOLROW_NUM; index++)
    {
        if ((TRUE == DcmInternal_DecreaseTimer(&Dcm_ProtocolCtrl[index].P4Timer))
            && (DCM_MSG_IDLE != Dcm_ProtocolCtrl[index].State))
        {
            DslInternal_SetStateIdle(index, NULL_PTR, TRUE, FALSE);
        }
    }
}
#endif

/**
 * deal with protocol preemption, call stopProtocol if needed
 */
DCM_LOCAL Std_ReturnType DslInternal_ProcessPreemption(uint8 curProtocolId, uint8 targetProtocolId)
{
    Std_ReturnType result;
    /* only do preemption for protocols of lower priority */
    if (Dcm_DslProtocolRow[targetProtocolId].Priority < Dcm_DslProtocolRow[curProtocolId].Priority)
    {
        result = DslInternal_StopProtocol(curProtocolId);
        if (E_OK != result)
        {
            DsdInternal_SendResponse(targetProtocolId, DCM_E_CONDITIONSNOTCORRECT);
        }
    }
    else
    {
        result = E_NOT_OK;
#if (STD_ON == DCM_DIAG_RESP_ON_SECOND_DECLINED_REQUEST)
        DsdInternal_SendResponse(targetProtocolId, DCM_E_BUSYREPEATREQUEST);
#endif
    }
    return result;
}

#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
DCM_LOCAL void DcmInternal_ProcessQueuedRequest(uint8 protocolId)
{
    Dcm_ProtocolCtrlType*      protocolCtrlPtr = &Dcm_ProtocolCtrl[protocolId];
    Dcm_MsgContextType*        msgContextPtr   = &Dcm_MsgContext[protocolId];
    Dcm_QueuedRequestCtrlType* queuedCtrl      = NULL_PTR;
    uint8                      tempQueueIndex  = DCM_INVALID_UINT8;

    for (uint8 index = 0u; index < 2u; index++)
    {
        if ((Dcm_QueuedRequestCtrl[index].QueuedRequestProtocolId == protocolId)
            && (DCM_PENDING_REQUEST_PROCESS == Dcm_QueuedRequestCtrl[index].PendingRequestState))
        {
            tempQueueIndex = index;
            queuedCtrl     = &Dcm_QueuedRequestCtrl[index];
            break;
        }
    }

    if (queuedCtrl != NULL_PTR)
    {
        /* setup ctrl unit for further process */
        uint8 sessionIndex;
        protocolCtrlPtr->Sid              = Dcm_RequestQueuedBuffer[tempQueueIndex][0];
        protocolCtrlPtr->RxPduId          = queuedCtrl->QueuedRequestRxPduId;
        protocolCtrlPtr->ConnectionId     = queuedCtrl->QueuedRequestConnectionId;
        msgContextPtr->msgAddInfo.reqType = Dcm_DslProtocolRx[queuedCtrl->QueuedRequestRxPduId].AddrType;
        msgContextPtr->reqData            = &Dcm_RequestQueuedBuffer[tempQueueIndex][1];
        msgContextPtr->reqDataLen         = queuedCtrl->QueuedRequestLength;
        msgContextPtr->dcmRxPduId         = queuedCtrl->QueuedRequestRxPduId;
        (void)DcmInternal_FindSessionIndex(Dcm_SessionCtrl.CurrentSession, &sessionIndex);
        protocolCtrlPtr->P2Timer = Dcm_DspSessionRow[sessionIndex].P2ServerMax
                                   - Dcm_DslProtocolRow[protocolId].P2ServerAdjust - DCM_MAINFUNCTION_PERIOD;
        DsdInternal_RxIndication(protocolId);
    }
}
#endif

/* get service function and process */
DCM_LOCAL void DcmInternal_DispatchService(uint8 protocolId)
{
    const Dcm_ProtocolCtrlType*  protocolCtrlPtr     = &Dcm_ProtocolCtrl[protocolId];
    Dcm_MsgContextType*          msgContextPtr       = &Dcm_MsgContext[protocolId];
    const Dcm_DsdServiceType*    DcmDsdServicePtr    = &Dcm_DsdService[protocolCtrlPtr->ServIndex];
    const Dcm_DsdSubServiceType* DcmDsdSubServicePtr = &Dcm_DsdSubService[protocolCtrlPtr->SubServIndex];

    /* use subServiceFnc if configured, otherwise use sidTabFnc(internal or external) */
    Dcm_ServcieFncType func; /* PRQA S 3672 */ /* VL_Dcm_3672 */
    if ((DcmDsdServicePtr->SubfuncAvail) && (NULL_PTR != DcmDsdSubServicePtr->SubServiceFnc))
    {
        func = DcmDsdSubServicePtr->SubServiceFnc;
    }
    else
    {
        func = DcmDsdServicePtr->SidTabFnc;
    }
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
    if ((TRUE == protocolCtrlPtr->PagedBufferStarted) && (0u != protocolCtrlPtr->RemainPageLength))
    {
        msgContextPtr->resMaxDataLen = protocolCtrlPtr->FirstPageLength - protocolCtrlPtr->RemainPageLength;
    }
    else
    {
        msgContextPtr->resMaxDataLen = Dcm_DslProtocolRow[protocolId].TxBufferSize;
    }
#endif
    Dcm_NegativeResponseCodeType errorCode = DCM_POS_RESP;
    Std_ReturnType               result    = func(Dcm_ProtocolCtrl[protocolId].OpStatus, msgContextPtr, &errorCode);
    if (DCM_CANCEL == Dcm_ProtocolCtrl[protocolId].OpStatus)
    {
        if (DCM_MSG_SENT != Dcm_ProtocolCtrl[protocolId].State)
        {
            DslInternal_SetStateIdle(protocolId, NULL_PTR, TRUE, FALSE);
        }
    }
    else
    {
        /* deal with errorCode */
        DcmInternal_HandleServiceReturn(result, protocolId, errorCode);
    }
}

/* handle preemption for non-default session process */
DCM_LOCAL Std_ReturnType DcmInternal_HandlePreemption(uint8 protocolId)
{
    Std_ReturnType result = E_OK;
    if ((DCM_DEFAULT_SESSION != Dcm_SessionCtrl.CurrentSession)
        && (protocolId != Dcm_SessionCtrl.CurrentSessionProtocolId))
    {
        result = DslInternal_ProcessPreemption(Dcm_SessionCtrl.CurrentSessionProtocolId, protocolId);
    }
    else if (
        (DCM_INVALID_UINT8 != Dcm_SessionCtrl.NewSession) && (DCM_INVALID_UINT8 != Dcm_SessionCtrl.NewSessionProtocolId)
        && (protocolId != Dcm_SessionCtrl.NewSessionProtocolId))
    {
        result = DslInternal_ProcessPreemption(Dcm_SessionCtrl.NewSessionProtocolId, protocolId);
        if (E_OK == result)
        {
            Dcm_SessionCtrl.NewSession           = DCM_INVALID_UINT8;
            Dcm_SessionCtrl.NewSessionProtocolId = DCM_INVALID_UINT8;
        }
    }
    else
    {
        /* idle */
    }

    return result;
}

#if (DCM_CALLBACK_DCM_REQUEST_SERVICE > 0u)
/* iterate over request callbacks for protocol start */
DCM_LOCAL Std_ReturnType DcmInternal_HandleRequestCallback(uint8 protocolId)
{
    Dcm_ProtocolCtrlType*         protocolCtrlPtr      = &Dcm_ProtocolCtrl[protocolId];
    const Dcm_DslProtocolRowType* DcmDslProtocolRowPtr = &Dcm_DslProtocolRow[protocolId];
    uint16                        sourceAddress        = 0u;
    Std_ReturnType                result;

    if (DcmDslProtocolRowPtr->ProtocolUseGenericConnection)
    {
        DcmInternal_GetMetaDataSATA(protocolId, &sourceAddress, NULL_PTR);
    }

    for (uint8 callbackIndex = 0u; callbackIndex < DCM_CALLBACK_DCM_REQUEST_SERVICE; callbackIndex++)
    {
        result = Dcm_DslCallbackDCMRequestService[callbackIndex].StartProtocol(
            DcmDslProtocolRowPtr->ProtocolType,
            sourceAddress,
            protocolCtrlPtr->ConnectionId);
        if (E_OK == result)
        {
            protocolCtrlPtr->ProtocolActive = TRUE;
            DslInternal_ResetToDefaultSession(TRUE);
        }
        else if ((E_NOT_OK == result) || (E_PROTOCOL_NOT_ALLOWED == result))
        {
            result = E_NOT_OK;
            DsdInternal_SendResponse(protocolId, DCM_E_CONDITIONSNOTCORRECT);
            break;
        }
        else
        {
#if (STD_ON == DCM_DEV_ERROR_DETECT)
            DCM_DET_REPORT(DCM_MAIN_FUNCTION_ID, DCM_E_INTERFACE_RETURN_VALUE);
#endif
        }
    }

    return result;
}
#endif

#if ((DCM_MANUFACTURER_NOTIFICATION_NUM > 0) || (DCM_SUPPLIER_NOTIFICATION_NUM > 0))
/* iterate over input nofication container and call callouts */
DCM_LOCAL void DcmInternal_HandleConfirmation(
    uint8                                        protocolId,
    const Dcm_DsdServiceRequestNotificationType* notification,
    uint8                                        notificationNum)
{
    const Dcm_ConfirmationHandleType* confirmationInfo     = &Dcm_ConfirmationHandle[protocolId];
    const Dcm_DslProtocolRowType*     DcmDslProtocolRowPtr = &Dcm_DslProtocolRow[protocolId];

    for (uint8 subIndex = 0u; subIndex < notificationNum; subIndex++)
    {
        Std_ReturnType result = notification[subIndex].Confirmation(
            confirmationInfo->Sid,
            confirmationInfo->ReqType,
            confirmationInfo->ConnectionId,
            confirmationInfo->ConfirmationStatus,
            DcmDslProtocolRowPtr->ProtocolType,
            DcmDslProtocolRowPtr->EcuAddr);
#if (STD_ON == DCM_DEV_ERROR_DETECT)
        if ((E_OK != result) && (E_NOT_OK == result))
        {
            DCM_DET_REPORT(DCM_MAIN_FUNCTION_ID, DCM_E_INTERFACE_RETURN_VALUE);
        }
#else
        DCM_UNUSED(result);
#endif
    }
}
#endif

DCM_LOCAL void
    DcmInternal_HandleServiceReturn(Std_ReturnType result, uint8 protocolId, Dcm_NegativeResponseCodeType errorCode)
{
    Dcm_NegativeResponseCodeType tempErrorCode = errorCode;
#if (STD_ON == DCM_OBD)
    Dcm_ProtocolType protocolType = Dcm_DslProtocolRow[protocolId].ProtocolType;
    if ((E_NOT_OK == result) && (DCM_POS_RESP == errorCode)
        && (((Dcm_ProtocolType)DCM_OBD_ON_CAN == protocolType) || ((Dcm_ProtocolType)DCM_OBD_ON_FLEXRAY == protocolType)
            || ((Dcm_ProtocolType)DCM_OBD_ON_IP == protocolType)))
    {
        tempErrorCode = DCM_E_REQUESTOUTOFRANGE;
    }
#endif
    if (E_OK == result)
    {
        Dcm_ProtocolCtrl[protocolId].OpStatus = DCM_INITIAL;
        tempErrorCode                         = DCM_POS_RESP;
    }
    else if ((E_NOT_OK == result) && (DCM_POS_RESP == tempErrorCode))
    {
        Dcm_ProtocolCtrl[protocolId].OpStatus = DCM_INITIAL;
        tempErrorCode                         = DCM_E_GENERALREJECT;
        DCM_DET_REPORTRUNTIME(DCM_TPRXINDICATION_ID, DCM_E_INVALID_VALUE);
    }
    else if ((DCM_E_PENDING == result) || (DCM_E_FORCE_RCRRP == result))
    {
        Dcm_MsgContext[protocolId].msgAddInfo.suppressPosResponse = 0u;
    }
    else
    {
        Dcm_ProtocolCtrl[protocolId].OpStatus = DCM_INITIAL;
    }
    if (DCM_E_PENDING != result)
    {
        if (DCM_E_FORCE_RCRRP == result)
        {
            tempErrorCode                         = DCM_E_RESPONSE_PENDING;
            Dcm_ProtocolCtrl[protocolId].OpStatus = DCM_FORCE_RCRRP_OK;
        }
        DsdInternal_SendResponse(protocolId, tempErrorCode);
    }
    else
    {
        Dcm_ProtocolCtrl[protocolId].OpStatus = DCM_PENDING;
    }
}

DCM_LOCAL Std_ReturnType DcmInternal_PrepareRxIndication(uint8 protocolId)
{
    Std_ReturnType result = E_OK;

    if (Dcm_DslProtocolRow[protocolId].ProtocolUseGenericConnection)
    {
        uint16 targetAddress = 0u;
        DcmInternal_GetMetaDataSATA(protocolId, NULL_PTR, &targetAddress);
        /* PRQA S 2120++ */ /* VL_Dcm_2120 */
        if ((targetAddress != Dcm_DslProtocolRow[protocolId].EcuAddr)
            && (0u == Dcm_MsgContext[protocolId].msgAddInfo.reqType))
        {
            result = E_NOT_OK;
        }
        /* PRQA S 2120-- */ /* VL_Dcm_2120 */
    }

    if (E_OK == result)
    {
        result = DcmInternal_HandlePreemption(protocolId);
    }

    if (E_OK == result)
    {
        if (!Dcm_ProtocolCtrl[protocolId].ProtocolActive)
        {
#if (DCM_CALLBACK_DCM_REQUEST_SERVICE > 0u)
            result = DcmInternal_HandleRequestCallback(protocolId);
#endif
        }
    }

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
