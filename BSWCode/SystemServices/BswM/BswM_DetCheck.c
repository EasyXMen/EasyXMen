/* PRQA S 3108++ */
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
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : BswM_DetCheck.c                                             **
**                                                                            **
**  Created on  : 2020-03-24                                                  **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "BswM_Internal.h"
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
FUNC(Std_ReturnType, BSWM_CODE)
BswM_ChkBswMInitStatus(uint8 sid)
{
    Std_ReturnType ret = E_OK;

    if (BSWM_INITED != BswM_RuntimeStatus.bswmInitState)
    {
        ret = E_NOT_OK;
        /*report DET error BSWM_E_UNINIT*/
        (void)Det_ReportError(BSWM_MODULE_ID, BSWM_INSTANCE_ID, sid, BSWM_E_UNINIT);
    }
    return ret;
}

#if (BSWM_MULTI_PARTITION_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkParRestart(void)
{
    Std_ReturnType ret = E_NOT_OK;

    if (E_OK == BswM_ChkBswMInitStatus(BSWM_API_ID_BSWM_PARTITION_RESTARTED))
    {
        ret = E_OK;
    }
    return ret;
}
#endif /*BSWM_MULTI_PARTITION_ENABLED == STD_ON*/

#if (BSWM_CANSM_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkCanSMInd(CanSM_BswMCurrentStateType CurrentState)
{
    Std_ReturnType ret = E_OK;

    if ((Std_ReturnType)E_OK != BswM_ChkBswMInitStatus(BSWM_API_ID_CANSM_CURRENT_STATE))
    {
        ret = E_NOT_OK;
    }
    else if (CurrentState > CANSM_BSWM_CHANGE_BAUDRATE)
    {
        ret = E_NOT_OK;
        /*report DET error BSWM_E_UNINIT*/
        (void)Det_ReportError(
            BSWM_MODULE_ID,
            BSWM_INSTANCE_ID,
            BSWM_API_ID_CANSM_CURRENT_STATE,
            BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
    {
        /*Do Nothing*/
    }
    return ret;
}

#if (BSWM_CANSM_ICOM_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkCanSMIcom(NetworkHandleType Network)
{
    Std_ReturnType ret = E_NOT_OK;

    (void)Network;
    if ((Std_ReturnType)E_OK == BswM_ChkBswMInitStatus(BSWM_API_ID_CANSM_CURRENT_ICOM_CONFIG))
    {
        ret = E_OK;
    }
    return ret;
}
#endif /*BSWM_CANSM_ICOM_ENABLED == STD_ON*/
#endif /*BSWM_CANSM_ENABLED == STD_ON*/

#if (BSWM_COMM_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkComMInd(ComM_ModeType RequestedMode)
{
    Std_ReturnType ret = E_OK;

    if ((Std_ReturnType)E_OK != BswM_ChkBswMInitStatus(BSWM_API_ID_COMM_CURRENT_MODE))
    {
        ret = E_NOT_OK;
    }
    else if (
        (RequestedMode != COMM_NO_COMMUNICATION) && (RequestedMode != COMM_SILENT_COMMUNICATION)
        && (RequestedMode != COMM_FULL_COMMUNICATION))
    {
        ret = E_NOT_OK;
        (void)Det_ReportError(
            BSWM_MODULE_ID,
            BSWM_INSTANCE_ID,
            BSWM_API_ID_COMM_CURRENT_MODE,
            BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
    {
        /*Do Nothing*/
    }
    return ret;
}

#if (BSWM_COMM_PNC_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkComMPnc(ComM_PncModeType CurrentPncMode)
{
    Std_ReturnType ret = E_OK;

    if ((Std_ReturnType)E_OK != BswM_ChkBswMInitStatus(BSWM_API_ID_COMM_CURRENT_PNC_MODE))
    {
        ret = E_NOT_OK;
    }
    else if (
        (CurrentPncMode != COMM_PNC_NO_COMMUNICATION) && (CurrentPncMode != COMM_PNC_PREPARE_SLEEP)
        && (CurrentPncMode != COMM_PNC_READY_SLEEP) && (CurrentPncMode != COMM_PNC_REQUESTED))
    {
        ret = E_NOT_OK;
        (void)Det_ReportError(
            BSWM_MODULE_ID,
            BSWM_INSTANCE_ID,
            BSWM_API_ID_COMM_CURRENT_PNC_MODE,
            BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
    {
        /*Do Nothing*/
    }
    return ret;
}
#endif /*BSWM_COMM_PNC_ENABLED == STD_ON*/
#endif /*BSWM_COMM_ENABLED == STD_ON*/

#if (BSWM_COMM_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkComMInitRst(void)
{
    Std_ReturnType ret = E_NOT_OK;

    if ((Std_ReturnType)E_OK == BswM_ChkBswMInitStatus(BSWM_API_ID_COMM_INITIATE_RESET))
    {
        ret = E_OK;
    }
    return ret;
}
#endif /*BSWM_COMM_ENABLED == STD_ON*/

#if (BSWM_DCM_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkDcmUpdate(void)
{
    Std_ReturnType ret = E_NOT_OK;

    if ((Std_ReturnType)E_OK == BswM_ChkBswMInitStatus(BSWM_API_ID_DCM_APPLICATION_UPDATED))
    {
        ret = E_OK;
    }
    return ret;
}

FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkDcmModeCurState(Dcm_CommunicationModeType RequestedMode)
{
    Std_ReturnType ret = E_OK;

    if ((Std_ReturnType)E_OK != BswM_ChkBswMInitStatus(BSWM_API_ID_DCM_COM_MODE_CURRENT_STATE))
    {
        ret = E_NOT_OK;
    }
    /*Dcm_CommunicationModeType range:0x00(DCM_ENABLE_RX_TX_NORM)~0x0b(DCM_DISABLE_RX_TX_NORM_NM)*/
    else if (RequestedMode > 0x0bu)
    {
        ret = E_NOT_OK;
        (void)Det_ReportError(
            BSWM_MODULE_ID,
            BSWM_INSTANCE_ID,
            BSWM_API_ID_DCM_COM_MODE_CURRENT_STATE,
            BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
    {
        /*Do Nothing*/
    }
    return ret;
}
#endif /*BSWM_DCM_ENABLED == STD_ON*/

#if (BSWM_ECUM_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkEcuMCurWakeUp(EcuM_WakeupStatusType state)
{
    Std_ReturnType ret = E_OK;

    if ((Std_ReturnType)E_OK != BswM_ChkBswMInitStatus(BSWM_API_ID_ECUM_CURRENT_WAKEUP))
    {
        ret = E_NOT_OK;
    }
    else if (
        (state != ECUM_WKSTATUS_NONE) && (state != ECUM_WKSTATUS_PENDING) && (state != ECUM_WKSTATUS_VALIDATED)
        && (state != ECUM_WKSTATUS_EXPIRED)
#if (BSWM_ECUM_VERSION == BSWM_V_4_2_2)
        && (state != ECUM_WKSTATUS_ENABLED)
#endif /*BSWM_ECUM_VERSION == BSWM_V_4_2_2*/
    )
    {
        ret = E_NOT_OK;
        (void)Det_ReportError(
            BSWM_MODULE_ID,
            BSWM_INSTANCE_ID,
            BSWM_API_ID_ECUM_CURRENT_WAKEUP,
            BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
    {
        /*Do Nothing*/
    }
    return ret;
}

FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkRequestedState(EcuM_StateType State, EcuM_RunStatusType CurrentStatus)
{
    Std_ReturnType ret;

    if ((Std_ReturnType)E_OK != BswM_ChkBswMInitStatus(BSWM_API_ID_ECUM_REQUESTED_STATE))
    {
        ret = E_NOT_OK;
    }
    else if (!((State == ECUM_STATE_APP_RUN) || (State == ECUM_STATE_APP_POST_RUN)))
    {
        ret = E_NOT_OK;
        (void)Det_ReportError(
            BSWM_MODULE_ID,
            BSWM_INSTANCE_ID,
            BSWM_API_ID_ECUM_REQUESTED_STATE,
            BSWM_E_REQ_USER_OUT_OF_RANGE);
    }
    else if (CurrentStatus > ECUM_RUNSTATUS_RELEASED)
    {
        ret = E_NOT_OK;
        (void)Det_ReportError(
            BSWM_MODULE_ID,
            BSWM_INSTANCE_ID,
            BSWM_API_ID_ECUM_REQUESTED_STATE,
            BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
    {
        ret = E_OK;
    }
    return ret;
}
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkEcuMCurState(void)
{
    Std_ReturnType ret = E_OK;

    if ((Std_ReturnType)E_OK != BswM_ChkBswMInitStatus(BSWM_API_ID_ECUM_CURRENT_STATE))
    {
        ret = E_NOT_OK;
    }
    return ret;
}
#endif /*BSWM_ECUM_ENABLED == STD_ON*/

#if (BSWM_ETHIF_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkEthIfPortGru(EthTrcv_LinkStateType PortGroupState)
{
    Std_ReturnType ret = E_OK;

    if ((Std_ReturnType)E_OK != BswM_ChkBswMInitStatus(BSWM_API_ID_ETHIF_PORT_GRU_LINK_STATE))
    {
        ret = E_NOT_OK;
    }
    else if ((PortGroupState != ETHTRCV_LINK_STATE_DOWN) && (PortGroupState != ETHTRCV_LINK_STATE_ACTIVE))
    {
        ret = E_NOT_OK;
        (void)Det_ReportError(
            BSWM_MODULE_ID,
            BSWM_INSTANCE_ID,
            BSWM_API_ID_ETHIF_PORT_GRU_LINK_STATE,
            BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
    {
        /*Do Nothing*/
    }
    return ret;
}
#endif /*BSWM_ETHIF_ENABLED == STD_ON*/

#if (BSWM_ETHSM_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkEthSMInd(EthSM_NetworkModeStateType CurrentState)
{
    Std_ReturnType ret = E_OK;

    if ((Std_ReturnType)E_OK != BswM_ChkBswMInitStatus(BSWM_API_ID_ETHSM_CURRENT_STATE))
    {
        ret = E_NOT_OK;
    }
    else if (CurrentState > ETHSM_STATE_WAIT_OFFLINE)
    {
        ret = E_NOT_OK;
        (void)Det_ReportError(
            BSWM_MODULE_ID,
            BSWM_INSTANCE_ID,
            BSWM_API_ID_ETHSM_CURRENT_STATE,
            BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
    {
        /*Do Nothing*/
    }
    return ret;
}
#endif /*BSWM_ETHSM_ENABLED == STD_ON*/

#if (BSWM_FRSM_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkFrSMInd(FrSM_BswM_StateType CurrentState)
{
    Std_ReturnType ret = E_OK;

    if ((Std_ReturnType)E_OK != BswM_ChkBswMInitStatus(BSWM_API_ID_FRSM_CURRENT_STATE))
    {
        ret = E_NOT_OK;
    }
    else if ((CurrentState != FRSM_BSWM_READY) && ((CurrentState != FRSM_BSWM_READY_ECU_PASSIVE)))
    {
        ret = E_NOT_OK;
        (void)Det_ReportError(
            BSWM_MODULE_ID,
            BSWM_INSTANCE_ID,
            BSWM_API_ID_FRSM_CURRENT_STATE,
            BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
    {
        /*Do Nothing*/
    }
    return ret;
}
#endif /*BSWM_FRSM_ENABLED == STD_ON*/

#if (BSWM_J1939DCM_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkJ1939DcmBroad(void)
{
    Std_ReturnType ret = E_OK;

    if ((Std_ReturnType)E_OK != BswM_ChkBswMInitStatus(BSWM_API_ID_J1939DCM_BROADCAST_STATUS))
    {
        ret = E_NOT_OK;
    }
    return ret;
}
#endif /*BSWM_J1939DCM_ENABLED == STD_ON*/

#if (BSWM_J1939NM_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkJ1939NmInd(void)
{
    Std_ReturnType ret = E_OK;

    if ((Std_ReturnType)E_OK != BswM_ChkBswMInitStatus(BSWM_API_ID_J1939NM_STATE_CHANGE_NOTIFY))
    {
        ret = E_NOT_OK;
    }
    return ret;
}
#endif /*BSWM_J1939NM_ENABLED == STD_ON*/

#if (BSWM_LINSM_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkLinSMCurSch(void)
{
    Std_ReturnType ret = E_OK;

    if ((Std_ReturnType)E_OK != BswM_ChkBswMInitStatus(BSWM_API_ID_LINSM_CURRENT_SCHEDULE))
    {
        ret = E_NOT_OK;
    }
    return ret;
}

FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkLinSMInd(LinSM_ModeType CurrentState)
{
    Std_ReturnType ret = E_OK;

    if ((Std_ReturnType)E_OK != BswM_ChkBswMInitStatus(BSWM_API_ID_LINSM_CURRENT_STATE))
    {
        ret = E_NOT_OK;
    }
    else if ((CurrentState != (LinSM_ModeType)LINSM_FULL_COM) && ((CurrentState != (LinSM_ModeType)LINSM_NO_COM)))
    {
        ret = E_NOT_OK;
        (void)Det_ReportError(
            BSWM_MODULE_ID,
            BSWM_INSTANCE_ID,
            BSWM_API_ID_LINSM_CURRENT_STATE,
            BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
    {
        /*Do Nothing*/
    }
    return ret;
}
#endif /*BSWM_LINSM_ENABLED == STD_ON*/

#if (BSWM_LINTP_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkLinTpRqst(LinTp_Mode LinTpRequestedMode)
{
    Std_ReturnType ret = E_OK;

    if ((Std_ReturnType)E_OK != BswM_ChkBswMInitStatus(BSWM_API_ID_LINTP_REQUEST_MODE))
    {
        ret = E_NOT_OK;
    }
    else if (
        (LinTpRequestedMode != LINTP_APPLICATIVE_SCHEDULE) && (LinTpRequestedMode != LINTP_DIAG_REQUEST)
        && (LinTpRequestedMode != LINTP_DIAG_RESPONSE))
    {
        ret = E_NOT_OK;
        (void)Det_ReportError(
            BSWM_MODULE_ID,
            BSWM_INSTANCE_ID,
            BSWM_API_ID_LINTP_REQUEST_MODE,
            BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
    {
        /*Do Nothing*/
    }
    return ret;
}
#endif /*BSWM_LINTP_ENABLED == STD_ON*/

#if (BSWM_NM_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkNmCarWkUp(void)
{
    Std_ReturnType ret = E_OK;

    if ((Std_ReturnType)E_OK != BswM_ChkBswMInitStatus(BSWM_API_ID_NMIF_CAR_WAKEUP_INDICATION))
    {
        ret = E_NOT_OK;
    }
    return ret;
}
#endif /*BSWM_NM_ENABLED == STD_ON*/

#if (BSWM_NVM_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkCurNvmBlockMode(NvM_RequestResultType CurrentBlockMode)
{
    Std_ReturnType ret = E_OK;

    if ((Std_ReturnType)E_OK != BswM_ChkBswMInitStatus(BSWM_API_ID_NVM_CURRENT_BLOCK_MODE))
    {
        ret = E_NOT_OK;
    }
    else if (CurrentBlockMode > NVM_REQ_RESTORED_FROM_ROM)
    {
        ret = E_NOT_OK;
        (void)Det_ReportError(
            BSWM_MODULE_ID,
            BSWM_INSTANCE_ID,
            BSWM_API_ID_NVM_CURRENT_BLOCK_MODE,
            BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
    {
        /*Do Nothing*/
    }
    return ret;
}

FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkCurNvmJobMode(NvM_MultiBlockRequestType MultiBlockRequest)
{
    Std_ReturnType ret = E_OK;

    if ((Std_ReturnType)E_OK != BswM_ChkBswMInitStatus(BSWM_API_ID_NVM_CURRENT_JOB_MODE))
    {
        ret = E_NOT_OK;
    }
    else if (
        (MultiBlockRequest != NVM_READ_ALL) && (MultiBlockRequest != NVM_WRITE_ALL)
        && (MultiBlockRequest != NVM_VALIDATE_ALL) && (MultiBlockRequest != NVM_FIRST_INIT_ALL)
        && (MultiBlockRequest != NVM_CANCEL_WRITE_ALL))
    {
        ret = E_NOT_OK;
        (void)Det_ReportError(
            BSWM_MODULE_ID,
            BSWM_INSTANCE_ID,
            BSWM_API_ID_NVM_CURRENT_JOB_MODE,
            BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
    {
        /*Do Nothing*/
    }
    return ret;
}
#endif /*BSWM_NVM_ENABLED == STD_ON*/

#if (BSWM_GENERIC_REQUEST_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkGenericRqst(BswM_UserType requestingUser)
{
    Std_ReturnType ret = E_OK;
    BswM_UserType numOfGenericReq;
    BswM_UserType userIdx;
    ApplicationType partIdx;
    boolean result;

    if ((Std_ReturnType)E_OK != BswM_ChkBswMInitStatus(BSWM_API_ID_REQUEST_MODE))
    {
        ret = E_NOT_OK;
    }
    else
    {
        result = BswM_GetPartitionIdx(&partIdx);
        if ((boolean)TRUE == result)
        {
            numOfGenericReq = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPCCfg->numOfGenericReq;
            for (userIdx = 0u; userIdx < numOfGenericReq; userIdx++)
            {
                if (requestingUser == BswM_RuntimeStatus.bswmPartLCfgs[partIdx].modeRqstLCfg->genRqstUserRef[userIdx])
                {
                    break;
                }
            }
            if (userIdx == numOfGenericReq)
            {
                ret = E_NOT_OK;
                (void)Det_ReportError(
                    BSWM_MODULE_ID,
                    BSWM_INSTANCE_ID,
                    BSWM_API_ID_REQUEST_MODE,
                    BSWM_E_REQ_USER_OUT_OF_RANGE);
            }
        }
        else
        {
            (void)Det_ReportError(BSWM_MODULE_ID, BSWM_INSTANCE_ID, BSWM_API_ID_REQUEST_MODE, BSWM_E_PARAM_CONFIG);
        }
    }
    return ret;
}
#endif /*BSWM_GENERIC_REQUEST_ENABLED == STD_ON*/

#if (BSWM_SD_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkClientCurState(Sd_ClientServiceCurrentStateType CurrentClientState)
{
    Std_ReturnType ret = E_OK;

    if ((Std_ReturnType)E_OK != BswM_ChkBswMInitStatus(BSWM_API_ID_SD_CLIENT_SERVICE_CURRENT_STATE))
    {
        ret = E_NOT_OK;
    }
    else if ((CurrentClientState != SD_CLIENT_SERVICE_DOWN) && (CurrentClientState != SD_CLIENT_SERVICE_AVAILABLE))
    {
        ret = E_NOT_OK;
        (void)Det_ReportError(
            BSWM_MODULE_ID,
            BSWM_INSTANCE_ID,
            BSWM_API_ID_SD_CLIENT_SERVICE_CURRENT_STATE,
            BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
    {
        /*Do Nothing*/
    }
    return ret;
}

FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkEvGruCurState(Sd_ConsumedEventGroupCurrentStateType ConsumedEventGroupState)
{
    Std_ReturnType ret = E_OK;

    if ((Std_ReturnType)E_OK != BswM_ChkBswMInitStatus(BSWM_API_ID_SD_CONSUMED_EVENT_GROUP_CURRENT_STATE))
    {
        ret = E_NOT_OK;
    }
    else if (
        (ConsumedEventGroupState != SD_CONSUMED_EVENTGROUP_DOWN)
        && (ConsumedEventGroupState != SD_CONSUMED_EVENTGROUP_AVAILABLE))
    {
        ret = E_NOT_OK;
        (void)Det_ReportError(
            BSWM_MODULE_ID,
            BSWM_INSTANCE_ID,
            BSWM_API_ID_SD_CONSUMED_EVENT_GROUP_CURRENT_STATE,
            BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
    {
        /*Do Nothing*/
    }
    return ret;
}

FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkEvHandlerCurState(Sd_EventHandlerCurrentStateType EventHandlerStatus)
{
    Std_ReturnType ret = E_OK;

    if ((Std_ReturnType)E_OK != BswM_ChkBswMInitStatus(BSWM_API_ID_SD_EVENT_HANDLER_CURRENT_STATE))
    {
        ret = E_NOT_OK;
    }
    else if ((EventHandlerStatus != SD_EVENT_HANDLER_RELEASED) && (EventHandlerStatus != SD_EVENT_HANDLER_REQUESTED))
    {
        ret = E_NOT_OK;
        (void)Det_ReportError(
            BSWM_MODULE_ID,
            BSWM_INSTANCE_ID,
            BSWM_API_ID_SD_EVENT_HANDLER_CURRENT_STATE,
            BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
    {
        /*Do Nothing*/
    }
    return ret;
}
#endif /*BSWM_SD_ENABLED == STD_ON*/

#if (BSWM_WDGM_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkWgmParRst(void)
{
    Std_ReturnType ret = E_OK;

    if ((Std_ReturnType)E_OK != BswM_ChkBswMInitStatus(BSWM_API_ID_WDGM_REQUEST_PARTITION_RESET))
    {
        ret = E_NOT_OK;
    }
    return ret;
}
#endif /*BSWM_WDGM_ENABLED == STD_ON*/
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/

#endif /*BSWM_DEV_ERROR_DETECT == STD_ON*/
