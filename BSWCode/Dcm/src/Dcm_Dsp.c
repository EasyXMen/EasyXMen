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
 **  @file               : Dcm_Dsp.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : DcmDsp Private Functions Definitions
 **
 ***********************************************************************************************************************/
/* PRQA S 1532 EOF */ /* VL_QAC_OneFunRef */
/* =================================================== inclusions =================================================== */
#include "Dcm_Internal.h"
#ifdef SEV_UDS_ECU_RESET
#include "IdsM.h"
#endif
/* ===================================================== macros ===================================================== */
/**
 * @brief service id of UDS service CommunicationControl
 */
#define DCM_UDS_COMCTRL_SID 0x28u
/* ========================================== external function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * deal with 0x11/0x29/0x28 txConfirmation(session, authentication state, and communication mode change)
 */
void DspInternal_DcmConfirmation(uint8 protocolId, uint16 connectionId)
{
    const Dcm_ProtocolCtrlType* protocolCtrlPtr = &Dcm_ProtocolCtrl[protocolId];
#if (((STD_OFF == DCM_UDS_0X29) || (!defined DCM_AUTHENTICATION_DEFAULT_SESSION_TIMEOUT)) && (STD_OFF == DCM_UDS_0X28))
    DCM_UNUSED(connectionId);
#endif

    if (DCM_POS_RESP == protocolCtrlPtr->ErrorCode)
    {
#if (STD_ON == DCM_UDS_0X11)
        if (DCM_UDS_ECURESET_SID == protocolCtrlPtr->Sid)
        {
            Dcm_ResetProcessing = TRUE;
#ifdef SEV_UDS_ECU_RESET
            IdsM_SetSecurityEvent(SEV_UDS_ECU_RESET);
#endif
            (void)Rte_Switch_EcuResetModeSwitchInterface_ecuReset(RTE_MODE_DcmEcuReset_DCM_EXECUTE);
        }
#endif

#if ((STD_ON == DCM_UDS_0X29) && (defined DCM_AUTHENTICATION_DEFAULT_SESSION_TIMEOUT))
        Dcm_UDS0x29_SetAuthenticationCtrlOn(connectionId);
#endif

        if (DCM_UDS_SESSIONCONTROL_SID == protocolCtrlPtr->Sid)
        {
            Dcm_UDS0x10_SessionChange();
        }

#if (STD_ON == DCM_UDS_0X28)
        if (DCM_UDS_COMCTRL_SID == protocolCtrlPtr->Sid)
        {
            Dcm_UDS0x28_TxConfirmation(connectionId);
        }
#endif
    }

    /** S3 Subsequent start
     * Case1:T_Data.con that indicates the completion of any response message that concludes a service execution
     * (final response message) in case a response message is required/allowed to be transmitted (this includes
     * positive and negative response messages). A negative response with negative response code 0x78 does not
     * restart the S3Server timer.
     * Case2:Completion of the requested action (service conclusion) in case no response message (positive and
     * negative) isrequired/allowed.
     */
    if ((DCM_DEFAULT_SESSION != Dcm_SessionCtrl.CurrentSession)
        && (Dcm_SessionCtrl.CurrentSessionProtocolId == protocolId))
    {
        Dcm_SessionCtrl.SessionTimer = DCM_S3_TIMEOUT;
    }
}

#if (STD_ON == DCM_UDS_0X86)
/* handle roeEvents related to DTC */
void DspInternal_handleDTCRoe(Dem_UdsStatusByteType DTCStatusOld, Dem_UdsStatusByteType DTCStatusNew)
{
    /* iterate over roeEvents */
    for (uint8 index = 0u; index < Dcm_RoeCtrlNum; index++)
    {
        Dcm_RoeCtrlType* roeCtrlPtr = &Dcm_RoeCtrl[index];
        Dcm_RoeEventType roeEvent   = roeCtrlPtr->RoeEvent;
        /* check for running ones with matched triggering condition */
        if (((DCM_ROE_ONDTC == roeEvent) || (DCM_ROE_DTCRECORD == roeEvent) || (DCM_ROE_MOSTRECENTDTC == roeEvent))
            && (DCM_ROE_ON == roeCtrlPtr->RoeStatus))
        {
            uint8 DTCMask;
            if (DCM_ROE_MOSTRECENTDTC == roeEvent)
            {
                DTCMask = 0x09u;
            }
            else
            {
                DTCMask = roeCtrlPtr->DTCMask;
            }
            /* compare over dtc status change, check for DTCMask */
            if ((DTCStatusNew & DTCMask) != (DTCStatusOld & DTCMask))
            {
                Dcm_UDS0x86_TriggerServiceRequest(roeCtrlPtr);
            }
        }
    }
}
#endif
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
