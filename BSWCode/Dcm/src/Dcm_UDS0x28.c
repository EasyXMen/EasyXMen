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
 **  @file               : Dcm_UDS0x28.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm UDS service 0x28 function implementation
 **
 ***********************************************************************************************************************/
/* PRQA S 3120, 1532 EOF */ /* VL_QAC_MagicNum, VL_QAC_OneFunRef */
/* =================================================== inclusions =================================================== */
#include "Dcm_Cfg.h"
#if (STD_ON == DCM_UDS_0X28)
#ifdef SEV_UDS_COMMUNICATION_CONTROL_SWITCHED_OFF
#include "IdsM.h"
#endif
#if (STD_ON == DCM_USE_BSWM)
#include "BswM_Dcm.h"
#endif
#include "Dcm_Internal.h"
/* ===================================================== macros ===================================================== */
/**
 * @brief The minimum request length of UDS 0x28
 */
#define DCM_UDS28_MINREQLEN 2uL
/**
 * @brief The subfunction enableRxAndDisableTxWithEnhancedAddressInformation
 */
#define DCM_UDS28_ENRX_DISTX_EAI 4u
/**
 * @brief The subfunction enableRxAndTxWithEnhancedAddressInformation
 */
#define DCM_UDS28_EN_RXTX_EAI 5u
/**
 * @brief The communicationType mask
 */
#define DCM_COMMTYPE_MASK 0x0Fu
/**
 * @brief The SubnetNumber mask
 */
#define DCM_SUBNET_MASK 0xF0u
#if ((DCM_COMCONTROL_ALL_NUM > 0) || (DCM_COMCONTROL_SPE_NUM > 0) || (DCM_COMCONTROL_SUB_NUM > 0))
/* ============================================ internal data definitions =========================================== */
#define DCM_START_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
#if (DCM_COMCONTROL_ALL_NUM > 0)
/**
 * @brief Tracks the control status of all communication channels in DCM @range NA.
 */
DCM_LOCAL boolean Dcm_ControlledAllChannel[DCM_COMCONTROL_ALL_NUM];
#endif
#if (DCM_COMCONTROL_SPE_NUM > 0)
/**
 * @brief Tracks the control status of specific communication channels in DCM @range NA.
 */
DCM_LOCAL boolean Dcm_ControlledSpecificChannel[DCM_COMCONTROL_SPE_NUM];
#endif
#if (DCM_COMCONTROL_SUB_NUM > 0)
DCM_LOCAL boolean Dcm_ControlledSubNode[DCM_COMCONTROL_SUB_NUM]; /* PRQA S 3218 */ /* VL_Dcm_3218 */
#endif
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
/* ========================================== internal function declarations ======================================== */
/**
 * @brief         This function switchs the communication modes and enables all coomunication
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DCM_LOCAL void Dcm_UDS0x28_ReEnableCommunication(void);
/**
 * @brief         This function calculate communicationMode
 * @param[in]     controlType              : the requested cotrolType
 * @param[in]     communicationType        : the requested communicationType
 * @return        uint8
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DCM_LOCAL uint8 Dcm_UDS0x28_GetCommunicationMode(uint8 controlType, uint8 communicationType);
#endif
#if ((defined DCM_UDS_0X28_0X4) || (defined DCM_UDS_0X28_0X5))
/**
 * @brief         This function handels txConfirmation of extended address subfunction
 * @param[in]     controlType              : the requested cotrolType
 * @param[in]     communicationType        : the requested communicationType
 * @param[in]     communicationMode : the requested communicationMode
 * @param[in]     nodeIdentificationNumber : the requested nodeIdentiferNumber
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DCM_LOCAL void Dcm_UDS0x28_TxConfirmationExtHandle(
    uint8  controlType,
    uint8  communicationType,
    uint8  communicationMode,
    uint16 nodeIdentificationNumber);
/**
 * @brief         This function finds matched subNode cfg based on input nodeId
 * @param[in]     nodeIdentificationNumber : the requested nodeIdentiferNumber
 * @param[out]    ErrorCode                : the nrc to send
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was unsuccessful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DCM_LOCAL Std_ReturnType
    Dcm_UDS0x28_FindSubNode(uint16 nodeIdentificationNumber, Dcm_NegativeResponseCodeType* ErrorCode);
#endif
/* ========================================== external function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/* PRQA S 5017,6010,6080 ++ */ /* VL_Dcm_5017,VL_MTR_Dcm_STCYC,VL_MTR_Dcm_STPTH */
/**
 * The service interpreter for UDS 0x28
 */
Std_ReturnType Dcm_UDS0x28(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result     = E_OK;
    Dcm_MsgLenType reqDataLen = pMsgContext->reqDataLen;
    const uint8*   reqData    = pMsgContext->reqData;
    DCM_UNUSED(OpStatus);

    if (reqDataLen < DCM_UDS28_MINREQLEN)
    {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        result     = E_NOT_OK;
    }

    uint8 controlType;
    if (E_OK == result)
    {
        controlType = reqData[0u];

#if ((defined DCM_UDS_0X28_0X4) || (defined DCM_UDS_0X28_0X5))
        if ((DCM_UDS28_ENRX_DISTX_EAI == controlType) || (DCM_UDS28_EN_RXTX_EAI == controlType))
        {
            if (reqDataLen == (DCM_UDS28_MINREQLEN + 2u))
            {
                result = Dcm_UDS0x28_FindSubNode(DCM_U8N_TO_U16(&reqData[2]), ErrorCode);
            }
            else
            {
                *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                result     = E_NOT_OK;
            }
        }
        else
        {
            if (reqDataLen != DCM_UDS28_MINREQLEN)
            {
                *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                result     = E_NOT_OK;
            }
        }
#else
        if (reqDataLen != DCM_UDS28_MINREQLEN)
        {
            *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
            result     = E_NOT_OK;
        }
#endif
    }

    if (E_OK == result)
    {
        uint8 communicationType = reqData[1u] & DCM_COMMTYPE_MASK;
        if ((communicationType == 0u) || (communicationType > 3u))
        {
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            result     = E_NOT_OK;
        }
    }

    if (E_OK == result)
    {
        uint8 subnetNumber = (reqData[1u] & DCM_SUBNET_MASK) >> 4u;
        result             = E_NOT_OK;
        switch (subnetNumber)
        {
#if (DCM_COMCONTROL_ALL_NUM > 0)
        case 0x00u:
        {
            for (uint16 index = 0u; index < DCM_COMCONTROL_ALL_NUM; index++)
            {
                if (Dcm_CfgPtr->ComControlAllChannelUsed[index])
                {
                    result = E_OK;
                    break;
                }
            }
            break;
        }
#endif
        case 0x0Fu:
        {
            result = E_OK;
            break;
        }
        default:
        {
#if (DCM_COMCONTROL_SPE_NUM > 0)
            for (uint16 index = 0u; index < DCM_COMCONTROL_SPE_NUM; index++)
            {
                const Dcm_DspComControlSpecificChannelType* speCom = &Dcm_DspComControlSpecificChannel[index];
                if ((TRUE == Dcm_CfgPtr->ComControlSpecificChannelUsed[index])
                    && (speCom->SubnetNumber == subnetNumber))
                {
                    result = E_OK;
                    break;
                }
            }
#else
            /* idle */
#endif
            break;
        }
        }
        if (E_NOT_OK == result)
        {
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        }
    }

    if (E_OK == result)
    {
        pMsgContext->resData[0u] = controlType;
        pMsgContext->resDataLen  = 1u;
    }

    return result;
}
/* PRQA S 5017,6010,6080 -- */

/**
 * The confirmation handle for 0x28, calls Com mode switch functions
 */
void Dcm_UDS0x28_TxConfirmation(uint16 ConnectionId)
{
    const Dcm_DslMainConnectionType* mainConnection    = &Dcm_DslMainConnection[ConnectionId];
    uint8                            ProtocolId        = mainConnection->ParentProtocolId;
    const uint8*                     reqData           = Dcm_MsgContext[ProtocolId].reqData;
    uint8                            controlType       = reqData[0u];
    uint8                            communicationType = reqData[1u] & DCM_COMMTYPE_MASK;
    uint8                            subnetNumber      = (reqData[1u] & DCM_SUBNET_MASK) >> 4u;
    uint8 communicationMode = Dcm_UDS0x28_GetCommunicationMode(controlType, communicationType);

#if ((defined DCM_UDS_0X28_0X4) || (defined DCM_UDS_0X28_0X5))
    Dcm_UDS0x28_TxConfirmationExtHandle(controlType, communicationType, communicationMode, DCM_U8N_TO_U16(&reqData[2]));
#endif

    switch (subnetNumber)
    {
#if (DCM_COMCONTROL_ALL_NUM > 0)
    case 0x00u:
    {
        for (uint16 index = 0u; index < DCM_COMCONTROL_ALL_NUM; index++)
        {
            const Dcm_DspComControlAllChannelType* allCom = &Dcm_DspComControlAllChannel[index];
            if (Dcm_CfgPtr->ComControlAllChannelUsed[index])
            {
                (void)allCom->AllChannelComCtrlFnc(communicationMode);
                if ((uint8)DCM_ENABLE_RX_TX_NORM_NM == communicationMode)
                {
                    Dcm_ControlledAllChannel[index] = FALSE;
                }
                else
                {
                    Dcm_ControlledAllChannel[index] = TRUE;
                }
#if (STD_ON == DCM_USE_BSWM)
                /* PRQA S 4342 ++ */ /* VL_Dcm_4342 */
                BswM_Dcm_CommunicationMode_CurrentState(
                    allCom->AllComMChannelRef,
                    (Dcm_CommunicationModeType)communicationMode);
                /* PRQA S 4342 -- */ /* VL_Dcm_4342 */
#endif
            }
        }
        break;
    }
#endif
    case 0x0Fu:
    {
        mainConnection->ProtocolComCtrlFnc(communicationMode);
        break;
    }
    default:
    {
#if (DCM_COMCONTROL_SPE_NUM > 0)
        for (uint16 index = 0u; index < DCM_COMCONTROL_SPE_NUM; index++)
        {
            const Dcm_DspComControlSpecificChannelType* speCom = &Dcm_DspComControlSpecificChannel[index];
            if ((TRUE == Dcm_CfgPtr->ComControlSpecificChannelUsed[index]) && (speCom->SubnetNumber == subnetNumber))
            {
                (void)speCom->SpecificChannelComCtrlFnc(communicationMode);
                if ((uint8)DCM_ENABLE_RX_TX_NORM_NM == communicationMode)
                {
                    Dcm_ControlledSpecificChannel[index] = FALSE;
                }
                else
                {
                    Dcm_ControlledSpecificChannel[index] = TRUE;
                }
#if (STD_ON == DCM_USE_BSWM)
                /* PRQA S 1441,4442 ++ */ /* VL_Dcm_1441,VL_Dcm_4442 */
                /* PRQA S 4342 ++ */      /* VL_Dcm_4342 */
                BswM_Dcm_CommunicationMode_CurrentState(
                    speCom->SpecificComMChannelRef,
                    (Dcm_CommunicationModeType)communicationMode);
                /* PRQA S 4342 -- */
                /* PRQA S 1441,4442 -- */
#endif
            }
        }
#else
        /* idle */
#endif
        break;
    }
    }
#ifdef SEV_UDS_COMMUNICATION_CONTROL_SWITCHED_OFF
    if ((DCM_DISABLE_RX_TX_NORM_NM == communicationMode) || (DCM_DISABLE_RX_TX_NORMAL == communicationMode))
    {
        IdsM_SetSecurityEvent(SEV_UDS_COMMUNICATION_CONTROL_SWITCHED_OFF);
    }
#endif
}

#if ((DCM_COMCONTROL_ALL_NUM > 0) || (DCM_COMCONTROL_SPE_NUM > 0) || (DCM_COMCONTROL_SUB_NUM > 0))
/**
 * called when session changes, check whether the new session supports com control, if not, restore communication
 */
/* PRQA S 3415 ++ */ /* VL_Dcm_3415 */
void Dcm_UDS0x28_SessionChange(void)
{
    const Dcm_DslProtocolRowType* protocolRow = &Dcm_DslProtocolRow[Dcm_SessionCtrl.CurrentSessionProtocolId];

    if ((DCM_DEFAULT_SESSION == Dcm_SessionCtrl.CurrentSession)
        || (E_NOT_OK
            == Dcm_UDS0x10_CheckSession(
                protocolRow->UDSComControlSessionRef,
                protocolRow->UDSComControlSessionRefNum,
                NULL_PTR)))
    {
        Dcm_UDS0x28_ReEnableCommunication();
    }
}
/* PRQA S 3415 -- */
#endif
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#if ((DCM_COMCONTROL_ALL_NUM > 0) || (DCM_COMCONTROL_SPE_NUM > 0) || (DCM_COMCONTROL_SUB_NUM > 0))
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * periodically check modeRule for communication control, restore if failed
 */
void Dcm_UDS0x28_MainFunction(void)
{
    if (NULL_PTR != Dcm_DspComControl.ComControlCommunicationReEnableModeRuleRef)
    {
        Dcm_NegativeResponseCodeType ErrorCode;
        Std_ReturnType               result = Dcm_DspComControl.ComControlCommunicationReEnableModeRuleRef(&ErrorCode);
        if (E_NOT_OK == result)
        {
            Dcm_UDS0x28_ReEnableCommunication();
        }
    }
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/* ========================================== internal function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * restore communication control
 */
DCM_LOCAL void Dcm_UDS0x28_ReEnableCommunication(void)
{
#if (DCM_COMCONTROL_ALL_NUM > 0)
    for (uint16 index = 0u; index < DCM_COMCONTROL_ALL_NUM; index++)
    {
        const Dcm_DspComControlAllChannelType* allCom = &Dcm_DspComControlAllChannel[index];
        if ((Dcm_CfgPtr->ComControlAllChannelUsed[index]) && (Dcm_ControlledAllChannel[index]))
        {
            (void)allCom->AllChannelComCtrlFnc((uint8)DCM_ENABLE_RX_TX_NORM_NM);
#if (STD_ON == DCM_USE_BSWM)
            BswM_Dcm_CommunicationMode_CurrentState(allCom->AllComMChannelRef, DCM_ENABLE_RX_TX_NORM_NM);
#endif
            Dcm_ControlledAllChannel[index] = FALSE;
        }
    }
#endif

#if (DCM_COMCONTROL_SPE_NUM > 0)
    for (uint16 index = 0u; index < DCM_COMCONTROL_SPE_NUM; index++)
    {
        const Dcm_DspComControlSpecificChannelType* speCom = &Dcm_DspComControlSpecificChannel[index];
        if ((TRUE == Dcm_CfgPtr->ComControlSpecificChannelUsed[index])
            && (TRUE == Dcm_ControlledSpecificChannel[index]))
        {
            (void)speCom->SpecificChannelComCtrlFnc((uint8)DCM_ENABLE_RX_TX_NORM_NM);
#if (STD_ON == DCM_USE_BSWM)
            BswM_Dcm_CommunicationMode_CurrentState(speCom->SpecificComMChannelRef, DCM_ENABLE_RX_TX_NORM_NM);
#endif
            Dcm_ControlledSpecificChannel[index] = FALSE;
        }
    }
#endif

#if (DCM_COMCONTROL_SUB_NUM > 0)
    for (uint16 index = 0u; index < DCM_COMCONTROL_SUB_NUM; index++)
    {
        const Dcm_DspComControlSubNodeType* subNode = &Dcm_DspComControlSubNode[index];
        if ((TRUE == Dcm_CfgPtr->ComControlSubNodeUsed[index]) && (TRUE == Dcm_ControlledSubNode[index]))
        {
            (void)subNode->SubNodeComCtrlFnc((uint8)DCM_ENABLE_RX_TX_NORM_NM);
#if (STD_ON == DCM_USE_BSWM)
            BswM_Dcm_CommunicationMode_CurrentState(subNode->SubNodeComMChannelRef, DCM_ENABLE_RX_TX_NORM_NM);
#endif
            Dcm_ControlledSubNode[index] = FALSE;
        }
    }
#endif
}

/* This function calculate communicationMode */
DCM_LOCAL uint8 Dcm_UDS0x28_GetCommunicationMode(uint8 controlType, uint8 communicationType)
{
    uint8 communicationMode;
#if ((defined DCM_UDS_0X28_0X4) || (defined DCM_UDS_0X28_0X5))
    if (DCM_UDS28_ENRX_DISTX_EAI > controlType)
#endif
    {
        communicationMode = controlType + ((communicationType - 1u) * 4u);
    }
#if ((defined DCM_UDS_0X28_0X4) || (defined DCM_UDS_0X28_0X5))
    else if (DCM_UDS28_ENRX_DISTX_EAI == controlType)
    {
        communicationMode = 1u + ((communicationType - 1u) * 4u);
    }
    else
    {
        communicationMode = (communicationType - 1u) * 4u;
    }
#endif
    return communicationMode;
}

#if ((defined DCM_UDS_0X28_0X4) || (defined DCM_UDS_0X28_0X5))
/* This function finds matched subNode cfg based on input nodeId */
DCM_LOCAL Std_ReturnType
    Dcm_UDS0x28_FindSubNode(uint16 nodeIdentificationNumber, Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_NOT_OK;

#if (DCM_COMCONTROL_SUB_NUM > 0)
    for (uint16 index = 0u; index < DCM_COMCONTROL_SUB_NUM; index++)
    {
        const Dcm_DspComControlSubNodeType* subNode = &Dcm_DspComControlSubNode[index];
        if ((TRUE == Dcm_CfgPtr->ComControlSubNodeUsed[index]) && (nodeIdentificationNumber == subNode->SubNodeId))
        {
            result = E_OK;
            break;
        }
    }
#endif
    if (E_NOT_OK == result)
    {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
    }

    return result;
}

/* This function handels txConfirmation of extended address subfunction */
DCM_LOCAL void Dcm_UDS0x28_TxConfirmationExtHandle(
    uint8  controlType,
    uint8  communicationType,
    uint8  communicationMode,
    uint16 nodeIdentificationNumber)
{
    if ((DCM_UDS28_ENRX_DISTX_EAI == controlType) || (DCM_UDS28_EN_RXTX_EAI == controlType))
    {
        for (uint16 index = 0u; index < DCM_COMCONTROL_SUB_NUM; index++)
        {
            const Dcm_DspComControlSubNodeType* subNode = &Dcm_DspComControlSubNode[index];
            if ((TRUE == Dcm_CfgPtr->ComControlSubNodeUsed[index]) && (nodeIdentificationNumber == subNode->SubNodeId))
            {
                subNode->SubNodeComCtrlFnc(communicationMode);
                if ((uint8)DCM_ENABLE_RX_TX_NORM_NM == communicationMode)
                {
                    Dcm_ControlledSubNode[index] = FALSE;
                }
                else
                {
                    Dcm_ControlledSubNode[index] = TRUE;
                }
#if (STD_ON == DCM_USE_BSWM)
                /* PRQA S 4342 ++ */ /* VL_Dcm_4342 */
                BswM_Dcm_CommunicationMode_CurrentState(
                    subNode->SubNodeComMChannelRef,
                    (Dcm_CommunicationModeType)communicationMode);
                /* PRQA S 4342 -- */
#endif
                break;
            }
        }
    }
}
#endif
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
#endif
