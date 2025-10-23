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
 **  @file               : Dcm_UDS0x2A.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm UDS service 0x2A function implementation
 **
 ***********************************************************************************************************************/
/* PRQA S 0553 EOF */ /* VL_Dcm_0553 */
/* =================================================== inclusions =================================================== */
#include "Dcm_Cfg.h"
#if (STD_ON == DCM_UDS_0X2A)
#include "PduR_Dcm.h"
#include "Dcm_Internal.h"
/* ===================================================== macros ===================================================== */
/**
 * @brief The minimum request length of UDS 0x2A
 */
#define DCM_UDS2A_MINREQLEN 1u
/**
 * @brief The stop transmissionMode
 */
#define DCM_UDS2A_STOP 4u
/* ================================================ type definitions ================================================ */
/**
 * @brief UDS 0x2A scheduler manager structure type
 */
typedef struct SchedulerType /* PRQA S 1536,3213  */ /* VL_Dcm_1536,VL_Dcm_3213 */
{
    /* the status of dynamic reading */
    Dcm_OpStatusType OpStatus;
    /* timer for counting time to send */
    uint16 Timer;
    /* the scheduling dynamic did index */
    uint16 DDDIdIndex;
    /* the rate of periodic transmission */
    uint16 Rate;
    /* store the dynamic data length (if have) */
    uint16 DynDataLength;
    /* the current connection id */
    uint16 ConnectionId;
    /* the current transMode */
    uint8 TransMode;
    /* the current did */
    uint16 Did;
} Dcm_SchedulerType;
/* ============================================ internal data definitions =========================================== */
#if (STD_ON == DCM_DYN_DID)
/**
 * @brief UDS 0x2A scheduler manager structure
 */
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
DCM_LOCAL Dcm_SchedulerType Dcm_Scheduler[DCM_MAX_SCHEDULER_NUM];
/* UDS 0x2A scheduler manager structure */
DCM_LOCAL PduInfoType Dcm_PeriodicPduInfo; /* PRQA S 3218 */ /* VL_Dcm_3218 */
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
/**
 * @brief number of scheduling for each rate, index 0 stands for total
 */
#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
DCM_LOCAL uint8 Dcm_ScheduledRate[4u];
DCM_LOCAL uint8 Dcm_PeriodicDataBuffer[64u]; /* PRQA S 3218 */ /* VL_Dcm_3218 */
DCM_LOCAL uint8 Dcm_SchedulerMetaData[4u]; /* PRQA S 3218 */   /* VL_Dcm_3218 */
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
#endif
/* ============================================ external data definitions =========================================== */
#if (DCM_PERIODIC_CONNECTION_NUM > 0u)
/**
 * @brief used to check txConfirmation, set to true when sent and false when confirmed @range NA
 */
#define DCM_START_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
boolean Dcm_PeriodicConnectionSent[DCM_PERIODIC_CONNECTION_NUM]; /* PRQA S 1037,1533 */ /* VL_Dcm_1037,VL_Dcm_1533 */
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
#endif
/* ========================================= internal function declarations ========================================= */
#if (STD_ON == DCM_DYN_DID)
/**
 * @brief         Called by UDS 0x2A to initialize the scheduler
 * @param[in]     schedulerPtr : the scheduler ptr
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL void Dcm_UDS0x2A_SchedulerInit(Dcm_SchedulerType* schedulerPtr);

/**
 * @brief         check conditions for UDS 0x2A
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @reentrant     TRUE
 * @synchronous   Synchronous
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType
    Dcm_UDS0x2A_ConditionCheck(Dcm_MsgContextType* pMsgContext, Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         handle did read for UDS 0x2A
 * @param[in]     OpStatus     : current operation status
 * @param[in]     connectionId : current connection id
 * @param[inout]  pMsgContext  : Message-related information for one diagnostic protocol identifier. The pointers in
 *                               pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode    : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                               shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                               value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @reentrant     TRUE
 * @synchronous   Synchronous
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x2A_HandleDid(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    uint16                        connectionId,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         handles did conditionCheck callout
 * @param[in]     didIndex  : the reuqested did index
 * @param[in]     OpStatus  : current operation status
 * @param[out]    ErrorCode : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                            shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                            value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @reentrant     TRUE
 * @synchronous   Synchronous
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x2A_HandleDidConditionCheck(
    uint16                        didIndex,
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         prepare for dynamic did read
 * @param[in]     connectionId : current connection id
 * @param[in]     didIndex     : the reuqested did index
 * @param[in]     OpStatus     : current operation status
 * @param[out]    dataLength   : the dataLength of dynamic did
 * @param[out]    ErrorCode    : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                               shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                               value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     TRUE
 * @synchronous   Depends on Application
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x2A_DidReadPrepare(
    uint16                        connectionId,
    uint16                        didIndex,
    Dcm_ExtendedOpStatusType      OpStatus,
    uint16*                       dataLength,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         setup scheduler for requested periodic read
 * @param[in]     didIndex     : the reuqested did index
 * @param[in]     connectionId : current connection id
 * @param[out]    dataLength   : the dataLength of dynamic did
 * @param[inout]  pMsgContext  : Message-related information for one diagnostic protocol identifier. The pointers in
 *                               pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode    : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                               shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                               value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @reentrant     TRUE
 * @synchronous   Synchronous
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x2A_SetScheduler(
    uint16                        didIndex,
    uint16                        connectionId,
    uint16                        dataLength,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         setup scheduler for requested periodic read for nonStop subfunction
 * @param[in]     schedulerPtr : the previously found schedulerPtr
 * @param[in]     didIndex     : the reuqested did index
 * @param[in]     connectionId : current connection id
 * @param[in]     transMode    : the requested transMode
 * @param[in]     dataLength   : the dataLength of dynamic did
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @reentrant     TRUE
 * @synchronous   Synchronous
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x2A_SetNonStopScheduler(
    Dcm_SchedulerType* schedulerPtr,
    uint16             didIndex,
    uint16             connectionId,
    uint8              transMode,
    uint16             dataLength);

/**
 * @brief         handle a operating scheduler
 * @param[in]     schedulerPtr : the target scheduler
 * @return        void
 * @reentrant     TRUE
 * @synchronous   Synchronous
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL void Dcm_UDS0x2A_MainFunctionHandle(Dcm_SchedulerType* schedulerPtr);

/**
 * @brief         prepare data and send periodic data
 * @param[in]     connIndex    : the current connection index
 * @param[in]     schedulerPtr : the current scheduler pointer
 * @return        void
 * @reentrant     TRUE
 * @synchronous   Synchronous
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL void Dcm_UDS0x2A_PeriodicSend(uint16 connIndex, Dcm_SchedulerType* schedulerPtr);
#endif
/* ========================================== external function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * The service interpreter for UDS 0x2A
 */
/* PRQA S 1532,3673 ++ */ /* VL_QAC_OneFunRef,VL_QAC_3673 */
Std_ReturnType Dcm_UDS0x2A(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
/* PRQA S 1532,3673 -- */
{
#if (STD_ON == DCM_DYN_DID)
    Std_ReturnType result     = E_OK;
    Dcm_MsgLenType reqDataLen = pMsgContext->reqDataLen;
    uint8          transMode;
    uint16         connectionId;

    (void)DslInternal_FindProtocolRowByRxPduId(pMsgContext->dcmRxPduId, NULL_PTR, &connectionId);
    result = Dcm_UDS0x2A_ConditionCheck(pMsgContext, ErrorCode);

    if (E_OK == result)
    {
        transMode = pMsgContext->reqData[0u];
        if (((DCM_UDS2A_STOP == transMode) && (reqDataLen == 1u)) || (DCM_CANCEL == OpStatus))
        {
            for (uint16 index = 0u; index < DCM_MAX_SCHEDULER_NUM; index++)
            {
                Dcm_UDS0x2A_SchedulerInit(&Dcm_Scheduler[index]);
            }
            DcmInternal_Memset(Dcm_ScheduledRate, 0u, 4u);
        }
        else
        {
            result = Dcm_UDS0x2A_HandleDid(OpStatus, pMsgContext, connectionId, ErrorCode);
        }
    }

    return result;
#else
    DCM_UNUSED(OpStatus);
    DCM_UNUSED(pMsgContext);
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
    return E_NOT_OK;
#endif
}

#if (STD_ON == DCM_DYN_DID)
/**
 * MainFunction handle for 0x2A to periodically read and send data of dynamic did
 */
void Dcm_UDS0x2A_MainFunction(void)
{
    for (uint8 index = 0u; index < DCM_MAX_SCHEDULER_NUM; index++)
    {
#if (DCM_DDDID_NUM > 0u)
        Dcm_SchedulerType* schedulerPtr = &Dcm_Scheduler[index];
        uint16             DDDIdIndex   = schedulerPtr->DDDIdIndex;
        if ((0u != schedulerPtr->Rate) && (DDDIdIndex < DCM_DDDID_NUM)
            && (DCM_NOT_DEFINED != Dcm_DDDID[DDDIdIndex].DDDIDStatus[0]))
        {
            if (0u != schedulerPtr->Timer)
            {
                schedulerPtr->Timer--;
            }
            else
            {
                Dcm_UDS0x2A_MainFunctionHandle(schedulerPtr);
            }
        }
#endif
    }
}

/**
 * Deal with session/security/authentication/modeRule change to decide whether to continue onging periodic tranmission
 */
void Dcm_UDS0x2A_StatusChangeHandle(void)
{
    for (uint8 index = 0u; index < DCM_MAX_SCHEDULER_NUM; index++)
    {
        Dcm_SchedulerType* schedulerPtr = &Dcm_Scheduler[index];
        if (0u != schedulerPtr->Rate)
        {
#if (DCM_DDDID_NUM > 0u)
            const Dcm_DDDIDType* DDDIdPtr = &Dcm_DDDID[schedulerPtr->DDDIdIndex];
            for (uint8 subIndex = 0u; subIndex < DDDIdPtr->SourceElementsNum; subIndex++)
            {
                Std_ReturnType result = Dcm_UDS_CheckReadWriteDid(
                    schedulerPtr->ConnectionId,
                    Dcm_DspDid[DDDIdPtr->DDDIDDefine[subIndex].SourceDidIndex].DidIdentifier,
                    DCM_DID_INFO(DDDIdPtr->DDDIDDefine[subIndex].SourceDidIndex).DidRead,
                    NULL_PTR);
                if (E_NOT_OK == result)
                {
                    uint8 transMode = schedulerPtr->TransMode;
                    Dcm_UDS0x2A_SchedulerInit(schedulerPtr);
                    Dcm_ScheduledRate[0u]--;
                    Dcm_ScheduledRate[transMode]--;
                }
            }
#endif
        }
    }
}
/* ========================================== internal function definitions ========================================= */
/**
 * initialize targeted scheduler
 */
DCM_LOCAL void Dcm_UDS0x2A_SchedulerInit(Dcm_SchedulerType* schedulerPtr)
{
    SchM_Enter_Dcm_ExclusiveArea();
    schedulerPtr->OpStatus      = DCM_INITIAL;
    schedulerPtr->Timer         = 0u;
    schedulerPtr->Did           = DCM_INVALID_UINT16;
    schedulerPtr->DDDIdIndex    = DCM_INVALID_UINT16;
    schedulerPtr->Rate          = 0u;
    schedulerPtr->TransMode     = 0u;
    schedulerPtr->DynDataLength = 0u;
    SchM_Exit_Dcm_ExclusiveArea();
}
#endif

#if (STD_ON == DCM_DYN_DID)
/* check conditions such as minimum length, invalid transmode for UDS 0x2A */
DCM_LOCAL Std_ReturnType
    Dcm_UDS0x2A_ConditionCheck(Dcm_MsgContextType* pMsgContext, Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result     = E_NOT_OK;
    Dcm_MsgLenType reqDataLen = pMsgContext->reqDataLen;

    /* Minimum length check */
    if (reqDataLen < DCM_UDS2A_MINREQLEN)
    {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
    }
    else
    {
        uint8 transMode = pMsgContext->reqData[0u];
        uint8 maxScheduledRate;

        if ((0u == transMode) || (transMode > DCM_UDS2A_STOP))
        {
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        }
        else if (
            (transMode != DCM_UDS2A_STOP)
            && ((reqDataLen < 2u)
#ifdef DCM_MAX_PERIODIC_DID_TO_READ
                || ((reqDataLen - 1u) > DCM_MAX_PERIODIC_DID_TO_READ)
#endif
                    ))
        {
            *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        }
        else if (transMode != DCM_UDS2A_STOP)
        {
            maxScheduledRate = Dcm_DspPeriodicTransmissionMaxPeriodicTransmissions[transMode - 1u];
            if (((reqDataLen - 1u + Dcm_ScheduledRate[transMode]) > maxScheduledRate) && (0u != maxScheduledRate))
            {
                *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            }
            else
            {
                result = E_OK;
            }
        }
        else
        {
            result = E_OK;
        }
    }

    return result;
}
#endif

#if (STD_ON == DCM_DYN_DID)
/* iterate over requested did to initiate read */
DCM_LOCAL Std_ReturnType Dcm_UDS0x2A_HandleDid(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    uint16                        connectionId,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    boolean        pass   = FALSE;
    Std_ReturnType result = E_OK;

    for (uint16 index = 1u; index <= (pMsgContext->reqDataLen - 1u); index++)
    {
        uint16 did = DCM_UDS_DYNDID_RANGE_LOWER | (uint16)pMsgContext->reqData[index];
        uint16 didIndex;
        result = Dcm_UDS_FindDid(did, NULL_PTR, &didIndex, ErrorCode);

        if (E_OK == result)
        {
            result = Dcm_UDS_CheckReadWriteDid(
                connectionId,
                Dcm_DspDid[didIndex].DidIdentifier,
                DCM_DID_INFO(didIndex).DidRead,
                ErrorCode);
        }

        if (E_OK == result)
        {
            result = Dcm_UDS0x2A_HandleDidConditionCheck(didIndex, OpStatus, ErrorCode);
        }

        uint16 dataLength = 0u;
        if (E_OK == result)
        {
            result = Dcm_UDS0x2A_DidReadPrepare(connectionId, didIndex, OpStatus, &dataLength, ErrorCode);
        }

        if (E_OK == result)
        {
            result = Dcm_UDS0x2A_SetScheduler(didIndex, connectionId, dataLength, pMsgContext, ErrorCode);
        }

        if (E_OK == result)
        {
            pass = TRUE;
        }
    }
    if ((FALSE == pass) && (*ErrorCode == DCM_POS_RESP))
    {
        result     = E_NOT_OK;
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
    }

    return result;
}

/* iterate over signals and check for conditionCheckReadFnc callout */
DCM_LOCAL Std_ReturnType Dcm_UDS0x2A_HandleDidConditionCheck(
    uint16                        didIndex,
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_OK;

    for (uint16 subIndex = Dcm_DspDid[didIndex].DidSignalIndexStart;
         (subIndex <= Dcm_DspDid[didIndex].DidSignalIndexEnd) && (E_OK == result);
         subIndex++)
    {
        if (NULL_PTR != Dcm_DspDidSignal[subIndex].DidDataRef->ConditionCheckReadFnc)
        {
            result = Dcm_DspDidSignal[subIndex].DidDataRef->ConditionCheckReadFnc(OpStatus, ErrorCode);
#if (STD_ON == DCM_DEV_ERROR_DETECT)
            if ((E_OK != result) && (E_NOT_OK != result))
            {
                DCM_DET_REPORT(DCM_MAIN_FUNCTION_ID, DCM_E_INTERFACE_RETURN_VALUE);
                *ErrorCode = DCM_E_GENERALREJECT;
                result     = E_NOT_OK;
            }
#endif
        }
    }

    return result;
}

/* iterate over sourc elements and do check if configured checkSource and get dynamic dataLength */
DCM_LOCAL Std_ReturnType Dcm_UDS0x2A_DidReadPrepare(
    uint16                        connectionId,
    uint16                        didIndex,
    Dcm_ExtendedOpStatusType      OpStatus,
    uint16*                       dataLength,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType            finalResult      = E_OK;
    uint16                    DDDIdIndex       = DCM_DID_INFO(didIndex).DDDidIndex;
    const Dcm_DspDidInfoType* DcmDspDidInfoPtr = &DCM_DID_INFO(didIndex);
#if ((STD_ON == DCM_DDDID_CHECK_SOURCE) && (DCM_DDDID_NUM > 0u))
    if (TRUE == DcmDspDidInfoPtr->DynamicallyDefined)
    {
        Std_ReturnType result = E_OK;
        for (uint8 subdIndex = 0u; (subdIndex < Dcm_DDDID[DDDIdIndex].SourceElementsNum) && (E_NOT_OK != result);
             subdIndex++)
        {
            if (DCM_DEFINED_SOURCE_DID == Dcm_DDDID[DDDIdIndex].DDDIDStatus[subdIndex])
            {
                uint16 subDidIndex = Dcm_DDDID[DDDIdIndex].DDDIDDefine[subdIndex].SourceDidIndex;
                result             = Dcm_UDS_CheckReadWriteDid(
                    connectionId,
                    Dcm_DspDid[subDidIndex].DidIdentifier,
                    DCM_DID_INFO(subDidIndex).DidRead,
                    ErrorCode);
#if (STD_ON == DCM_DYN_DATA)
                const Dcm_DspDataCfgType* dspDataCfg =
                    Dcm_DspDidSignal[Dcm_DspDid[subDidIndex].DidSignalIndexEnd].DidDataRef;
                if ((E_OK == result) && (DCM_UINT8_DYN == dspDataCfg->DspDataType))
                {
                    result = dspDataCfg->ReadDataLengthFnc(OpStatus, dataLength);
                    if ((E_OK == result) && (*dataLength > dspDataCfg->DspDataByteSize))
                    {
                        result = E_NOT_OK;
                    }
                    if (E_OK != result)
                    {
                        finalResult = result;
                    }
                }
#else
                DCM_UNUSED(OpStatus);
                DCM_UNUSED(dataLength);
#endif
            }
        }
    }
#else
    DCM_UNUSED(connectionId);
    DCM_UNUSED(ErrorCode);
    DCM_UNUSED(OpStatus);
    DCM_UNUSED(dataLength);
#endif

    return finalResult;
}

/* find matched scheduler and reset if existed, otherwise find a idle scheduler and do initial setup */
DCM_LOCAL Std_ReturnType Dcm_UDS0x2A_SetScheduler(
    uint16                        didIndex,
    uint16                        connectionId,
    uint16                        dataLength,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType     result = E_NOT_OK;
    uint8              schedulerIndex;
    Dcm_SchedulerType* schedulerPtr = NULL_PTR;
    uint16             DDDIdIndex   = DCM_DID_INFO(didIndex).DDDidIndex;

    for (schedulerIndex = 0u; schedulerIndex < Dcm_ScheduledRate[0u]; schedulerIndex++)
    {
        if ((Dcm_Scheduler[schedulerIndex].DDDIdIndex == DDDIdIndex) && (Dcm_Scheduler[schedulerIndex].Rate == 0u))
        {
            schedulerPtr = &Dcm_Scheduler[schedulerIndex];
            result       = E_OK;
            break;
        }
    }

    uint8 transMode = pMsgContext->reqData[0u];
    if ((0u != transMode) && (DCM_UDS2A_STOP != transMode))
    {
        result = Dcm_UDS0x2A_SetNonStopScheduler(schedulerPtr, didIndex, connectionId, transMode, dataLength);
        if (E_NOT_OK == result)
        {
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        }
    }
    else if (E_OK == result)
    {
        transMode = schedulerPtr->TransMode;
        Dcm_UDS0x2A_SchedulerInit(schedulerPtr);
        Dcm_ScheduledRate[0u]--;
        Dcm_ScheduledRate[transMode]--;
    }
    else
    {
        /* idle */
    }

    return result;
}

/* setup the scheduler for nonStop subfunctions */
DCM_LOCAL Std_ReturnType Dcm_UDS0x2A_SetNonStopScheduler(
    Dcm_SchedulerType* schedulerPtr,
    uint16             didIndex,
    uint16             connectionId,
    uint8              transMode,
    uint16             dataLength)
{
    Std_ReturnType     result           = E_NOT_OK;
    Dcm_SchedulerType* tempSchedulerPtr = schedulerPtr;

    if (NULL_PTR == schedulerPtr)
    {
        for (uint8 schedulerIndex = 0u; schedulerIndex < DCM_MAX_SCHEDULER_NUM; schedulerIndex++)
        {
            if (Dcm_Scheduler[schedulerIndex].Rate == 0u)
            {
                tempSchedulerPtr = &Dcm_Scheduler[schedulerIndex];
                result           = E_OK;
            }
        }
    }
    else
    {
        result = E_OK;
    }

    uint16 DDDIdIndex = DCM_DID_INFO(didIndex).DDDidIndex;
    if ((E_OK == result)
        && ((FALSE == DCM_DID_INFO(didIndex).DynamicallyDefined)
#if (DCM_DDDID_NUM > 0u)
            || (DCM_NOT_DEFINED == Dcm_DDDID[DDDIdIndex].DDDIDStatus)
#endif
                ))
    {
        result = E_NOT_OK;
    }

    if (E_OK == result)
    {
        uint16 rate = Dcm_DspPeriodicTransmissionRate[transMode - 1u];
        Dcm_ScheduledRate[0u]++;
        Dcm_ScheduledRate[transMode]++;
        tempSchedulerPtr->DynDataLength = dataLength;
        tempSchedulerPtr->Did           = Dcm_DspDid[didIndex].DidIdentifier;
        tempSchedulerPtr->DDDIdIndex    = DDDIdIndex;
        tempSchedulerPtr->Rate          = rate;
        tempSchedulerPtr->Timer         = rate;
        tempSchedulerPtr->ConnectionId  = connectionId;
        tempSchedulerPtr->TransMode     = transMode;
    }

    return result;
}

/* try to send periodic message */
DCM_LOCAL void Dcm_UDS0x2A_MainFunctionHandle(Dcm_SchedulerType* schedulerPtr)
{
    const Dcm_DslMainConnectionType* connection      = &Dcm_DslMainConnection[schedulerPtr->ConnectionId];
    boolean                          connectionAvail = FALSE;
    uint16                           connIndex       = 0u;
    for (uint16 subIndex = 0u; subIndex < connection->PeriodicConnectionIdNum; subIndex++)
    {
        connIndex = connection->PeriodicConnectionId + subIndex;
#if (DCM_PERIODIC_CONNECTION_NUM > 0u)
        if (FALSE == Dcm_PeriodicConnectionSent[connIndex])
        {
            connectionAvail = TRUE;
        }
#endif
    }

    if (TRUE == connectionAvail)
    {
        Dcm_UDS0x2A_PeriodicSend(connIndex, schedulerPtr);
    }
#if (STD_ON == DCM_PERIODIC_SCHEDULER_TYPE2)
    else
    {
        /* was ready but not sent */
        schedulerPtr->Timer = schedulerPtr->Rate;
    }
#endif
}

DCM_LOCAL void Dcm_UDS0x2A_PeriodicSend(uint16 connIndex, Dcm_SchedulerType* schedulerPtr)
{
    Dcm_MsgLenType offset  = 0u;
    Dcm_MsgLenType bufSize = 64u;
#if (STD_ON == DCM_DDDID_CHECK_SOURCE)
    boolean checkFail = FALSE;
#endif
    Dcm_NegativeResponseCodeType ErrorCode;
    Std_ReturnType               result = Dcm_UDS_DynDidHandle(
        schedulerPtr->OpStatus,
        schedulerPtr->ConnectionId,
        schedulerPtr->DDDIdIndex,
        &Dcm_PeriodicDataBuffer[offset + 1u],
        &bufSize,
#if (STD_ON == DCM_DDDID_CHECK_SOURCE)
        &checkFail,
#endif
        &ErrorCode);

    uint8 transMode;
    if (E_OK == result)
    {
        Dcm_PeriodicDataBuffer[offset] = (uint8)(schedulerPtr->Did);
        /* PRQA S 2986 ++ */ /* VL_Dcm_2986 */
        offset +=
            (0u == schedulerPtr->DynDataLength) ? (bufSize + 1u) : ((Dcm_MsgLenType)schedulerPtr->DynDataLength + 1u);
        /* PRQA S 2986 -- */
        Dcm_PeriodicPduInfo.SduDataPtr = Dcm_PeriodicDataBuffer;
        Dcm_PeriodicPduInfo.SduLength  = offset;
        const uint8* metaDataPtr =
            Dcm_ProtocolCtrl[Dcm_DslMainConnection[schedulerPtr->ConnectionId].ParentProtocolId].MetaDataPtr;
        if (NULL_PTR != metaDataPtr)
        {
            Dcm_SchedulerMetaData[0u]       = metaDataPtr[2u];
            Dcm_SchedulerMetaData[1u]       = metaDataPtr[3u];
            Dcm_SchedulerMetaData[2u]       = metaDataPtr[0u];
            Dcm_SchedulerMetaData[3u]       = metaDataPtr[1u];
            Dcm_PeriodicPduInfo.MetaDataPtr = Dcm_SchedulerMetaData;
        }
        else
        {
            Dcm_PeriodicPduInfo.MetaDataPtr = NULL_PTR;
        }

        if (DCM_COMM_FULL_COMMUNICATION == Dcm_CommState[schedulerPtr->ConnectionId])
        {
            result = PduR_DcmTransmit(Dcm_DslPeriodicTxPduId[connIndex], &Dcm_PeriodicPduInfo);
            if (E_OK == result)
            {
#if (DCM_PERIODIC_CONNECTION_NUM > 0u)
                Dcm_PeriodicConnectionSent[connIndex] = TRUE;
#endif
            }
            schedulerPtr->Timer    = schedulerPtr->Rate;
            schedulerPtr->OpStatus = DCM_INITIAL;
        }
        else
        {
            transMode = schedulerPtr->TransMode;
            Dcm_UDS0x2A_SchedulerInit(schedulerPtr);
            Dcm_ScheduledRate[0u]--;
            Dcm_ScheduledRate[transMode]--;
        }
    }
    else if (DCM_E_PENDING == result)
    {
        schedulerPtr->OpStatus = DCM_PENDING;
    }
#if (STD_ON == DCM_DDDID_CHECK_SOURCE)
    else if (TRUE == checkFail)
    {
        if (DCM_PENDING == schedulerPtr->OpStatus)
        {
            (void)Dcm_UDS_DynDidHandle(
                DCM_CANCEL,
                schedulerPtr->ConnectionId,
                schedulerPtr->DDDIdIndex,
                &Dcm_PeriodicDataBuffer[offset],
                &bufSize,
                NULL_PTR,
                &ErrorCode);
        }
        transMode = schedulerPtr->TransMode;
        Dcm_UDS0x2A_SchedulerInit(schedulerPtr);
        Dcm_ScheduledRate[0u]--;
        Dcm_ScheduledRate[transMode]--;
    }
#endif
    else
    {
        /* idle */
    }
}
#endif
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
