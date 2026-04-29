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
 **  @file               : BswM_DetCheck.c
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : BSW Mode Manager
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "BswM_Internal.h"
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
/* PRQA S 0553 EOF */ /* VL_BswM_0553 */
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
/**
 * Checks the initialization status of the BSW Manager core.
 * If the core is not initialized, it reports a DET error and returns an error status.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
Std_ReturnType BswM_ChkBswMInitStatus(uint8 sid)
/* PRQA S 1503,1505,1532 --*/
{
    Std_ReturnType ret = E_OK;
    if (TRUE != BswM_GetCoreInitStatus())
    {
        ret = E_NOT_OK;
        /*report DET error BSWM_E_UNINIT*/
        (void)Det_ReportError(BSWM_MODULE_ID, BSWM_INSTANCE_ID, sid, BSWM_E_UNINIT);
    }
    return ret;
}

#if (BSWM_MULTI_PARTITION_ENABLED == STD_ON)
/**
 * Checks the initialization status of the BSW Manager core before performing a partition restart.
 * It uses the BswM_ChkBswMInitStatus function to verify the core initialization status.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
Std_ReturnType BswM_DetChkParRestart(void)
/* PRQA S 1503,1505,1532 --*/
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
/**
 * Checks the initialization status of the BSW Manager core and validates the CanSM current state.
 * It reports a DET error if the core is not initialized or if the current state is out of range.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
Std_ReturnType BswM_DetChkCanSMInd(CanSM_BswMCurrentStateType CurrentState)
/* PRQA S 1503,1505,1532 --*/
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

#endif /*BSWM_CANSM_ENABLED == STD_ON*/

#if (BSWM_COMM_ENABLED == STD_ON)
/**
 * Checks the initialization status of the BSW Manager core and validates the requested ComM mode.
 * It reports a DET error if the core is not initialized or if the requested mode is out of range.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
Std_ReturnType BswM_DetChkComMInd(ComM_ModeType RequestedMode)
/* PRQA S 1503,1505,1532 --*/
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
/**
 * Checks the initialization status of the BSW Manager core and validates the current ComM Pnc mode.
 * It reports a DET error if the core is not initialized or if the current Pnc mode is out of range.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
Std_ReturnType BswM_DetChkComMPnc(ComM_PncModeType CurrentPncMode)
/* PRQA S 1503,1505,1532 --*/
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
/**
 * Checks the initialization status of the BSW Manager core for the ComM initiate reset request.
 * It returns E_OK if the core is initialized, otherwise E_NOT_OK.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
Std_ReturnType BswM_DetChkComMInitRst(void)
/* PRQA S 1503,1505,1532 --*/
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
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
/**
 * Checks the initialization status of the BSW Manager core and validates the DCM application update status.
 * It reports a DET error if the core is not initialized.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
Std_ReturnType BswM_DetChkDcmUpdate(void)
/* PRQA S 1503,1505,1532 --*/
{
    Std_ReturnType ret = E_NOT_OK;

    if ((Std_ReturnType)E_OK == BswM_ChkBswMInitStatus(BSWM_API_ID_DCM_APPLICATION_UPDATED))
    {
        ret = E_OK;
    }
    return ret;
}
#endif

/**
 * Checks the initialization status of the BSW Manager core and validates the requested Dcm communication mode.
 * It reports a DET error if the core is not initialized or if the requested mode is out of range.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
Std_ReturnType BswM_DetChkDcmModeCurState(Dcm_CommunicationModeType RequestedMode)
/* PRQA S 1503,1505,1532 --*/
{
    Std_ReturnType ret = E_OK;

    if ((Std_ReturnType)E_OK != BswM_ChkBswMInitStatus(BSWM_API_ID_DCM_COM_MODE_CURRENT_STATE))
    {
        ret = E_NOT_OK;
    }
    /*Dcm_CommunicationModeType range:0x00(DCM_ENABLE_RX_TX_NORM)~0x0b(DCM_DISABLE_RX_TX_NORM_NM)*/
    else if (RequestedMode > DCM_DISABLE_RX_TX_NORM_NM)
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
/**
 * Checks the initialization status of the BSW Manager core and validates the current EcuM wakeup status.
 * It reports a DET error if the core is not initialized or if the wakeup status is out of range.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
Std_ReturnType BswM_DetChkEcuMCurWakeUp(EcuM_WakeupStatusType state)
/* PRQA S 1503,1505,1532 --*/
{
    Std_ReturnType ret = E_OK;

    if ((Std_ReturnType)E_OK != BswM_ChkBswMInitStatus(BSWM_API_ID_ECUM_CURRENT_WAKEUP))
    {
        ret = E_NOT_OK;
    }
    else if (
        (state != ECUM_WKSTATUS_NONE) && (state != ECUM_WKSTATUS_PENDING) && (state != ECUM_WKSTATUS_VALIDATED)
        && (state != ECUM_WKSTATUS_EXPIRED))
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

/**
 * Checks the initialization status of the BSW Manager core, validates the requested EcuM state, and the current run
 * status. It reports a DET error if the core is not initialized, the requested state is out of range, or the current
 * status is invalid.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
Std_ReturnType BswM_DetChkRequestedState(EcuM_StateType State, EcuM_RunStatusType CurrentStatus)
/* PRQA S 1503,1505,1532 --*/
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
            BSWM_E_REQ_MODE_OUT_OF_RANGE);
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

/**
 * Checks the initialization status of the BSW Manager core for the current EcuM state.
 * It reports a DET error if the core is not initialized.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
Std_ReturnType BswM_DetChkEcuMCurState(void)
/* PRQA S 1503,1505,1532 --*/
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
/**
 * Checks the initialization status of the BSW Manager core and validates the Ethernet port group link state.
 * It reports a DET error if the core is not initialized or if the port group state is out of range.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
Std_ReturnType BswM_DetChkEthIfPortGru(EthTrcv_LinkStateType PortGroupState)
/* PRQA S 1503,1505,1532 --*/
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
/**
 * Checks the initialization status of the BSW Manager core and validates the current EthSM network mode state.
 * It reports a DET error if the core is not initialized or if the current state is out of range.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
Std_ReturnType BswM_DetChkEthSMInd(EthSM_NetworkModeStateType CurrentState)
/* PRQA S 1503,1505,1532 --*/
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
/**
 * Checks the initialization status of the BSW Manager core and validates the current FrSM state.
 * It reports a DET error if the core is not initialized or if the current state is out of range.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
Std_ReturnType BswM_DetChkFrSMInd(FrSM_BswM_StateType CurrentState)
/* PRQA S 1503,1505,1532 --*/
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
/**
 * Checks the initialization status of the BSW Manager core for J1939 DCM broadcast status.
 * It returns an error if the core is not initialized.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
Std_ReturnType BswM_DetChkJ1939DcmBroad(void)
/* PRQA S 1503,1505,1532 --*/
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
/**
 * Checks the initialization status of the BSW Manager core for J1939 Network Management state change notifications.
 * It returns an error if the core is not initialized.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
Std_ReturnType BswM_DetChkJ1939NmInd(void)
/* PRQA S 1503,1505,1532 --*/
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
/**
 * Checks the initialization status of the BSW Manager core for the current LIN schedule.
 * It returns an error if the core is not initialized.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
Std_ReturnType BswM_DetChkLinSMCurSch(void)
/* PRQA S 1503,1505,1532 --*/
{
    Std_ReturnType ret = E_OK;

    if ((Std_ReturnType)E_OK != BswM_ChkBswMInitStatus(BSWM_API_ID_LINSM_CURRENT_SCHEDULE))
    {
        ret = E_NOT_OK;
    }
    return ret;
}

/**
 * Checks the initialization status of the BSW Manager core and validates the current LinSM state.
 * It reports a DET error if the core is not initialized or if the current state is out of range.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
Std_ReturnType BswM_DetChkLinSMInd(LinSM_ModeType CurrentState)
/* PRQA S 1503,1505,1532 --*/
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
/**
 * Checks the initialization status of the BSW Manager core and validates the requested LinTP mode.
 * It reports a DET error if the core is not initialized or if the requested mode is out of range.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
Std_ReturnType BswM_DetChkLinTpRqst(LinTp_Mode LinTpRequestedMode)
/* PRQA S 1503,1505,1532 --*/
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
/**
 * Checks the initialization status of the BSW Manager core for the Network Management car wake-up indication.
 * It returns an error if the core is not initialized.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
Std_ReturnType BswM_DetChkNmCarWkUp(void)
/* PRQA S 1503,1505,1532 --*/
{
    Std_ReturnType ret = E_OK;

    if ((Std_ReturnType)E_OK != BswM_ChkBswMInitStatus(BSWM_API_ID_NMIF_CAR_WAKEUP_INDICATION))
    {
        ret = E_NOT_OK;
    }
    return ret;
}

/**
 * Checks the initialization status of the BSW Manager core for the Network Management state change notification.
 * It returns an error if the core is not initialized.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
Std_ReturnType BswM_DetChkNmStateChange(void)
/* PRQA S 1503,1505,1532 --*/
{
    Std_ReturnType ret = E_OK;

    if ((Std_ReturnType)E_OK != BswM_ChkBswMInitStatus(BSWM_API_ID_NM_STATECHANGENOTIFICATION))
    {
        ret = E_NOT_OK;
    }
    return ret;
}

#endif /*BSWM_NM_ENABLED == STD_ON*/

#if (BSWM_NVM_ENABLED == STD_ON)
/**
 * Checks the initialization status of the BSW Manager core for the current NVM block mode.
 * It returns an error if the core is not initialized.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
Std_ReturnType BswM_DetChkCurNvmBlockMode(NvM_RequestResultType CurrentBlockMode)
/* PRQA S 1503,1505,1532 --*/
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

/**
 * Checks the initialization status of the BSW Manager core for the current NVM job mode.
 * It returns an error if the core is not initialized.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
Std_ReturnType BswM_DetChkCurNvmJobMode(NvM_MultiBlockRequestType MultiBlockRequest)
/* PRQA S 1503,1505,1532 --*/
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

#if (BSWM_GENERICREQUEST_ENABLED == STD_ON)
/**
 * Checks the initialization status of the BSW Manager core and validates the requesting user for a generic request.
 * It retrieves partition configuration pointers, checks if the requesting user is within the allowed range, and reports
 * a DET error if the user is invalid or the configuration is incorrect.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
Std_ReturnType BswM_DetChkGenericRqst(BswM_UserType requestingUser)
/* PRQA S 1503,1505,1532 --*/
{
    Std_ReturnType                 ret = E_OK;
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
    const BswM_PartitionPbConfigType* bswmPartPbConfigPtr;
#endif /* BSWM_PB_CONFIG_ENABLED */
    const BswM_PartitionLCfgType* bswmPartLCfgPtr;
    BswM_UserType                 numOfGenericReq;
    BswM_UserType                 userIdx;
    boolean                       result;

    if ((Std_ReturnType)E_OK != BswM_ChkBswMInitStatus(BSWM_API_ID_REQUEST_MODE))
    {
        ret = E_NOT_OK;
    }
    else
    {
        result = BswM_GetPartitionLcPCCfgPtr(
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
            &bswmPartPbConfigPtr,
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
            &bswmPartPCCfgPtr,
            &bswmPartLCfgPtr);
        if (result)
        {
            numOfGenericReq = bswmPartPCCfgPtr->modeRqstPCCfg->numOfGenericReq;
            for (userIdx = 0u; userIdx < numOfGenericReq; userIdx++)
            {
                if (requestingUser == bswmPartLCfgPtr->modeRqstLCfg->genRqstUserRef[userIdx])
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
#endif /*BSWM_GENERICREQUEST_ENABLED == STD_ON*/

#if (BSWM_SD_ENABLED == STD_ON)
/**
 * Checks the initialization status of the BSW Manager core and validates the requested service ID.
 * It retrieves partition configuration pointers and checks if the requested service ID is within the allowed range.
 * If the service ID is invalid or the configuration is incorrect, it reports a DET error.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
Std_ReturnType BswM_DetChkClientCurState(Sd_ClientServiceCurrentStateType CurrentClientState)
/* PRQA S 1503,1505,1532 --*/
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

/**
 * Checks the initialization status of the BSW Manager core and validates the consumed event group current state.
 * It returns an error if the core is not initialized or if the state is out of the allowed range.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
Std_ReturnType BswM_DetChkEvGruCurState(Sd_ConsumedEventGroupCurrentStateType ConsumedEventGroupState)
/* PRQA S 1503,1505,1532 --*/
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

/**
 * Checks the initialization status of the BSW Manager core and validates the event handler current state.
 * It returns an error if the core is not initialized or if the state is out of the allowed range.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
Std_ReturnType BswM_DetChkEvHandlerCurState(Sd_EventHandlerCurrentStateType EventHandlerStatus)
/* PRQA S 1503,1505,1532 --*/
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

#if (BSWM_SOAD_ENABLED == STD_ON)
/**
 * @brief
 *
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
Std_ReturnType BswM_DetChkSoAdSoConModeChg(void)
/* PRQA S 1503,1505,1532 --*/
{
    Std_ReturnType ret = E_OK;

    if ((Std_ReturnType)E_OK != BswM_ChkBswMInitStatus(BSWM_API_ID_SOAD_SOCONMODECHG))
    {
        ret = E_NOT_OK;
    }
    return ret;
}

#endif /* BSWM_SOAD_ENABLED == STD_ON */

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
/* ========================================== internal function definitions ========================================= */

#endif /*BSWM_DEV_ERROR_DETECT == STD_ON*/
