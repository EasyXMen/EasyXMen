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
 ***********************************************************************************************************************
 **
 **  @file               : EthSM.c
 **  @author             : xiaojian.liang
 **  @date               : 2024/12/23
 **  @vendor             : isoft
 **  @description        : Implementation source of EthSM
 **
 **********************************************************************************************************************/
/* PRQA S 6060 ++ */ /* VL_MTR_EthSM_STM19 */
/* =================================================== inclusions =================================================== */
#include "EthSM_Internal.h" /* Import EthSM_Cfg.h, EthSM.h */
#include "EthSM_TcpIp.h"    /* Import EthSM_TcpIpModeIndication */
#include "EthIf.h"          /* Import EthIf_SetControllerMode */
#include "BswM_EthSM.h"     /* Import BswM_EthSM_CurrentState */
#include "ComM.h"           /* Import ComM_BusSM_ModeIndication */

#if ETHSM_DEV_ERROR_DETECT == STD_ON
#include "Det.h" /* Import Det_ReportError */
#endif

#if ETHSM_DEM_ENABLED == STD_ON
#include "Dem.h" /* Import Dem_SetEventStatus */
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
ETHSM_LOCAL_INLINE ComM_ModeType EthSM_NetworkModeState2ComMode(EthSM_NetworkModeStateType state);
/* PRQA S 5016 ++ */ /* VL_EthSM_FalsePositive */
ETHSM_LOCAL const EthSM_NetworkType* EthSM_FindNetworkByNetworkHandle(NetworkHandleType networkHandle);
ETHSM_LOCAL const EthSM_NetworkType* EthSM_FindNetworkByCtrlIdx(uint8 ctrlIdx);
/* PRQA S 5016 -- */ /* VL_EthSM_FalsePositive */
ETHSM_LOCAL void EthSM_PrivateStateMainFunction(EthSM_NetworkVarType* varPtr);
ETHSM_LOCAL void EthSM_StateOffline(const EthSM_NetworkConfigType* configPtr, EthSM_NetworkVarType* varPtr);
ETHSM_LOCAL void EthSM_StateWaitTrcvLink(const EthSM_NetworkConfigType* configPtr, EthSM_NetworkVarType* varPtr);
ETHSM_LOCAL void EthSM_StateWaitOnline(const EthSM_NetworkConfigType* configPtr, EthSM_NetworkVarType* varPtr);
ETHSM_LOCAL void EthSM_StateOnline(const EthSM_NetworkConfigType* configPtr, EthSM_NetworkVarType* varPtr);
ETHSM_LOCAL void EthSM_StateOnhold(const EthSM_NetworkConfigType* configPtr, EthSM_NetworkVarType* varPtr);
ETHSM_LOCAL void EthSM_StateWaitOffline(const EthSM_NetworkConfigType* configPtr, EthSM_NetworkVarType* varPtr);
ETHSM_LOCAL void EthSM_StateChange(
    const EthSM_NetworkConfigType* configPtr,
    EthSM_NetworkVarType*          varPtr,
    EthSM_NetworkModeStateType     currentState);
#if ETHSM_TCPIP_ENABLED == STD_ON
ETHSM_LOCAL void EthSM_TcpIpRequestComMode(
    const EthSM_NetworkConfigType* configPtr,
    EthSM_NetworkVarType*          varPtr,
    TcpIp_StateType                tcpIpState);
#endif

#if ETHSM_DEV_ERROR_DETECT == STD_ON
#ifdef QAC_ANALYZE
#pragma PRQA_NO_SIDE_EFFECTS EthSM_ValidateComMMode
#pragma PRQA_NO_SIDE_EFFECTS EthSM_ValidateEthMode
#pragma PRQA_NO_SIDE_EFFECTS EthSM_ValidateTransceiverLinkState
#pragma PRQA_NO_SIDE_EFFECTS EthSM_ValidatePointer
#if ETHSM_TCPIP_ENABLED == STD_ON
#pragma PRQA_NO_SIDE_EFFECTS EthSM_ValidateTcpIpState
#endif
#endif
ETHSM_LOCAL boolean EthSM_ValidateNetworkContext(uint8 apiId, const EthSM_NetworkType* networkPtr, uint8 errorId);
ETHSM_LOCAL boolean EthSM_ValidatePointer(uint8 apiId, const void* pointer);
ETHSM_LOCAL boolean EthSM_ValidateComMMode(uint8 apiId, ComM_ModeType comMMode);
#if ETHSM_TCPIP_ENABLED == STD_ON
ETHSM_LOCAL boolean EthSM_ValidateTcpIpState(uint8 apiId, TcpIp_StateType tcpIpState);
#endif
ETHSM_LOCAL boolean EthSM_ValidateEthMode(uint8 apiId, Eth_ModeType ethMode);
ETHSM_LOCAL boolean EthSM_ValidateTransceiverLinkState(uint8 apiId, EthTrcv_LinkStateType ethTrcvLinkState);
#endif

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* PRQA S 1532, 2889 ++ */ /* VL_QAC_OneFunRef, VL_QAC_MultiReturn */
/* ========================================== external function definitions ========================================= */
#define ETHSM_START_SEC_CODE
#include "EthSM_MemMap.h"
/**
 * The implementation of EthSM_Init.
 */
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
void EthSM_Init(void)
/* PRQA S 1503 -- */ /* VL_QAC_NoUsedApi */
{
#if ETHSM_MULTIPLE_PARTITION_ENABLED == STD_ON
    ApplicationType application = GetApplicationID();
#endif

#if (ETHSM_DEV_ERROR_DETECT == STD_ON) && (ETHSM_MULTIPLE_PARTITION_ENABLED == STD_ON)
    boolean networkFound = FALSE;
#endif

    for (uint8 networkIndex = 0u; networkIndex < ETHSM_NETWORK_NUM; ++networkIndex)
    {
        const EthSM_NetworkType* networkPtr = &EthSM_Networks[networkIndex];

#if ETHSM_MULTIPLE_PARTITION_ENABLED == STD_ON
        const EthSM_NetworkConfigType* configPtr = networkPtr->ConfigPtr;
        if (configPtr->Application == application)
#endif
        {
#if (ETHSM_DEV_ERROR_DETECT == STD_ON) && (ETHSM_MULTIPLE_PARTITION_ENABLED == STD_ON)
            networkFound = TRUE;
#endif
            EthSM_NetworkVarType* varPtr = networkPtr->VarPtr;
            varPtr->ComMMode             = COMM_NO_COMMUNICATION;
            varPtr->WakeupRequest        = FALSE;
            varPtr->EthMode              = ETH_MODE_DOWN;
            varPtr->EthTrcvLinkState     = ETHTRCV_LINK_STATE_DOWN;
#if ETHSM_TCPIP_ENABLED == STD_ON
            varPtr->TcpIpState = TCPIP_STATE_OFFLINE;
#endif
            varPtr->NetworkModeState    = ETHSM_STATE_OFFLINE;
            varPtr->NetworkPrivateState = ETHSM_PRIVATE_STATE_NONE;
        }
    }

#if (ETHSM_DEV_ERROR_DETECT == STD_ON) && (ETHSM_MULTIPLE_PARTITION_ENABLED == STD_ON)
    if (!networkFound)
    {
        (void)Det_ReportError(ETHSM_MODULE_ID, 0u, ETHSM_SID_INIT, ETHSM_E_INVALID_PARTITION_CONTEXT);
    }
#endif
}

/**
 * The implementation of EthSM_GetVersionInfo.
 */
#if ETHSM_VERSION_INFO_API == STD_ON
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
void EthSM_GetVersionInfo(Std_VersionInfoType* versioninfo)
/* PRQA S 1503 -- */ /* VL_QAC_NoUsedApi */
{
#if ETHSM_DEV_ERROR_DETECT == STD_ON
    if (EthSM_ValidatePointer(ETHSM_SID_GETVERSIONINFO, versioninfo))
#endif
    {
        versioninfo->vendorID         = ETHSM_VENDOR_ID;
        versioninfo->moduleID         = ETHSM_MODULE_ID;
        versioninfo->sw_major_version = ETHSM_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = ETHSM_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = ETHSM_SW_PATCH_VERSION;
    }
}
#endif

/**
 * The implementation of EthSM_RequestComMode.
 */
Std_ReturnType EthSM_RequestComMode(NetworkHandleType NetworkHandle, ComM_ModeType ComM_Mode)
{
    Std_ReturnType                 stdReturn  = E_NOT_OK;
    const EthSM_NetworkType* const networkPtr = EthSM_FindNetworkByNetworkHandle(NetworkHandle);
#if ETHSM_DEV_ERROR_DETECT == STD_ON
    if (EthSM_ValidateNetworkContext(ETHSM_SID_REQUESTCOMMODE, networkPtr, ETHSM_E_INVALID_NETWORK_HANDLE)
        && EthSM_ValidateComMMode(ETHSM_SID_REQUESTCOMMODE, ComM_Mode))
#endif
    {
#if ETHSM_DUMMY_MODE == STD_OFF
        EthSM_NetworkVarType*          varPtr    = networkPtr->VarPtr;
        const EthSM_NetworkConfigType* configPtr = networkPtr->ConfigPtr;

        switch (ComM_Mode)
        {
        case COMM_NO_COMMUNICATION:
            varPtr->WakeupRequest = FALSE;
            varPtr->ComMMode      = COMM_NO_COMMUNICATION;
            break;
        case COMM_SILENT_COMMUNICATION:
            varPtr->WakeupRequest = FALSE;
            if (configPtr->ForwardSilentCommunicationEnabled)
            {
                varPtr->ComMMode = COMM_SILENT_COMMUNICATION;
            }
            break;
        case COMM_FULL_COMMUNICATION:
            varPtr->ComMMode = COMM_FULL_COMMUNICATION;
            break;
        case COMM_FULL_COMMUNICATION_WITH_WAKEUP_REQUEST:
            if (configPtr->WakeupSleepOnDatalineEnabled)
            {
                varPtr->WakeupRequest = TRUE;
            }
            varPtr->ComMMode = COMM_FULL_COMMUNICATION;
            break;
        default:
            /* Unreachable. */
            break;
        }
#endif
        stdReturn = E_OK;
    }
    return stdReturn;
}

/**
 * The implementation of EthSM_GetCurrentComMode.
 */
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType EthSM_GetCurrentComMode(NetworkHandleType NetworkHandle, ComM_ModeType* ComM_ModePtr)
/* PRQA S 1503 -- */ /* VL_QAC_NoUsedApi */
{
    Std_ReturnType                 stdReturn  = E_NOT_OK;
    const EthSM_NetworkType* const networkPtr = EthSM_FindNetworkByNetworkHandle(NetworkHandle);

#if ETHSM_DEV_ERROR_DETECT == STD_ON
    if (EthSM_ValidateNetworkContext(ETHSM_SID_GETCURRENTCOMMODE, networkPtr, ETHSM_E_INVALID_NETWORK_HANDLE)
        && EthSM_ValidatePointer(ETHSM_SID_GETCURRENTCOMMODE, ComM_ModePtr))
#endif
    {
        EthSM_NetworkModeStateType networkModeState = networkPtr->VarPtr->NetworkModeState;
        *ComM_ModePtr                               = EthSM_NetworkModeState2ComMode(networkModeState);
        stdReturn                                   = E_OK;
    }
    return stdReturn;
}

/**
 * The implementation of EthSM_CtrlModeIndication.
 */
void EthSM_CtrlModeIndication(uint8 CtrlIdx, Eth_ModeType CtrlMode)
{
    const EthSM_NetworkType* const networkPtr = EthSM_FindNetworkByCtrlIdx(CtrlIdx);

#if ETHSM_DEV_ERROR_DETECT == STD_ON
    if (EthSM_ValidateNetworkContext(ETHSM_SID_CTRLMODEINDICATION, networkPtr, ETHSM_E_PARAM_CONTROLLER)
        && EthSM_ValidateEthMode(ETHSM_SID_CTRLMODEINDICATION, CtrlMode))
#endif
    {
        networkPtr->VarPtr->EthMode = CtrlMode;
    }
}

/**
 * The implementation of EthSM_TrcvLinkStateChg.
 */
void EthSM_TrcvLinkStateChg(uint8 CtrlIdx, EthTrcv_LinkStateType TransceiverLinkState)
{
    const EthSM_NetworkType* const networkPtr = EthSM_FindNetworkByCtrlIdx(CtrlIdx);

#if ETHSM_DEV_ERROR_DETECT == STD_ON
    if (EthSM_ValidateNetworkContext(ETHSM_SID_TRCVLINKSTATECHG, networkPtr, ETHSM_E_PARAM_CONTROLLER)
        && EthSM_ValidateTransceiverLinkState(ETHSM_SID_TRCVLINKSTATECHG, TransceiverLinkState))
#endif
    {
        networkPtr->VarPtr->EthTrcvLinkState = TransceiverLinkState;
    }
}

#if ETHSM_TCPIP_ENABLED == STD_ON
/**
 * The implementation of EthSM_TcpIpModeIndication.
 */
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
void EthSM_TcpIpModeIndication(uint8 CtrlIdx, TcpIp_StateType TcpIpState)
/* PRQA S 1503 -- */ /* VL_QAC_NoUsedApi */
{
    const EthSM_NetworkType* const networkPtr = EthSM_FindNetworkByCtrlIdx(CtrlIdx);

#if ETHSM_DEV_ERROR_DETECT == STD_ON
    if (EthSM_ValidateNetworkContext(ETHSM_SID_TCPIPMODEINDICATION, networkPtr, ETHSM_E_PARAM_CONTROLLER)
        && EthSM_ValidateTcpIpState(ETHSM_SID_TCPIPMODEINDICATION, TcpIpState))
#endif
    {
        networkPtr->VarPtr->TcpIpState = TcpIpState;
    }
}
#endif

/**
 * The implementation of EthSM_MainFunction.
 */
void EthSM_MainFunction(const EthSM_NetworkType* networkPtr)
{
#if ETHSM_DEV_ERROR_DETECT == STD_ON
    if (EthSM_ValidateNetworkContext(ETHSM_SID_MAINFUNCTION, networkPtr, ETHSM_E_INVALID_NETWORK_HANDLE))
#endif
    {
        EthSM_NetworkVarType* varPtr = networkPtr->VarPtr;

        EthSM_PrivateStateMainFunction(varPtr);

        if (varPtr->NetworkPrivateState == ETHSM_PRIVATE_STATE_NONE)
        {
            void (*const behaviour[])(const EthSM_NetworkConfigType* configPtr, EthSM_NetworkVarType* varPtr) = {
                EthSM_StateOffline,
                EthSM_StateWaitTrcvLink,
                EthSM_StateWaitOnline,
                EthSM_StateOnline,
                EthSM_StateOnhold,
                EthSM_StateWaitOffline,
            };
            behaviour[varPtr->NetworkModeState](networkPtr->ConfigPtr, varPtr);
        }
    }
}

/* ========================================== internal function definitions ========================================= */
/**
 * @brief     Convert the network mode state to the corresponding ComM mode.
 * @param[in] state  The current state of the network
 * @retval    COMM_FULL_COMMUNICATION if the network is online, onhold, or wait offline
 * @retval    COMM_NO_COMMUNICATION if the network is offline, wait trcvlink, or wait online
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73931
 */
ETHSM_LOCAL_INLINE ComM_ModeType EthSM_NetworkModeState2ComMode(EthSM_NetworkModeStateType state)
{
    if ((state == ETHSM_STATE_OFFLINE) || (state == ETHSM_STATE_WAIT_TRCVLINK) || (state == ETHSM_STATE_WAIT_ONLINE))
    {
        return COMM_NO_COMMUNICATION;
    }
    return COMM_FULL_COMMUNICATION;
}

/**
 * @brief     Find the network by network handle.
 * @param[in] networkHandle     Network handle to find
 * @return    Pointer to the network instance that satisfies the networkHandle, or NULL_PTR if there is no such network.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73932
 */
ETHSM_LOCAL const EthSM_NetworkType* EthSM_FindNetworkByNetworkHandle(NetworkHandleType networkHandle)
{
    for (uint8 networkIndex = 0u; networkIndex < ETHSM_NETWORK_NUM; ++networkIndex)
    {
        const EthSM_NetworkType* const networkPtr = &EthSM_Networks[networkIndex];
        const EthSM_NetworkConfigType* configPtr  = networkPtr->ConfigPtr;
        if (configPtr->ComMChannelId == networkHandle)
        {
            return networkPtr;
        }
    }
    return NULL_PTR;
}

/**
 * @brief     Find the network by controller index.
 * @param[in] ctrlIdx           Controller index to find
 * @return    Pointer to the network instance that satisfies the ctrlIdx, or NULL_PTR if there is no such network.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73934
 */
ETHSM_LOCAL const EthSM_NetworkType* EthSM_FindNetworkByCtrlIdx(uint8 ctrlIdx)
{
    for (uint8 networkIndex = 0u; networkIndex < ETHSM_NETWORK_NUM; ++networkIndex)
    {
        const EthSM_NetworkType* const networkPtr = &EthSM_Networks[networkIndex];
        const EthSM_NetworkConfigType* configPtr  = networkPtr->ConfigPtr;
        if (configPtr->EthIfCtrlIdx == ctrlIdx)
        {
            return networkPtr;
        }
    }
    return NULL_PTR;
}

/**
 * @brief     Handle the private state main function.
 * @param[in] varPtr     Pointer to the network variable
 * @reentrant    TRUE (Reentrant for different networks. Non reentrant for the same network.)
 * @synchronous  TRUE
 * @trace        CPD-73936
 */
ETHSM_LOCAL void EthSM_PrivateStateMainFunction(EthSM_NetworkVarType* varPtr)
{
    switch (varPtr->NetworkPrivateState)
    {
    case ETHSM_PRIVATE_STATE_WAIT_CTRLACTIVE:
        if (varPtr->EthMode == ETH_MODE_ACTIVE)
        {
            varPtr->NetworkPrivateState = ETHSM_PRIVATE_STATE_NONE;
        }
        break;
    case ETHSM_PRIVATE_STATE_WAIT_CTRLDOWN:
        if (varPtr->EthMode == ETH_MODE_DOWN)
        {
            varPtr->NetworkPrivateState = ETHSM_PRIVATE_STATE_NONE;
        }
        break;
    default:
        /* do nothing. */
        break;
    }
}

/**
 * @brief     Handle state transition in ETHSM_STATE_OFFLINE.
 * @param[in] configPtr  Pointer to the network configuration
 * @param[in] varPtr     Pointer to the network variable
 * @reentrant    TRUE (Reentrant for different networks. Non reentrant for the same network.)
 * @synchronous  TRUE
 * @trace        CPD-73937
 */
ETHSM_LOCAL void EthSM_StateOffline(const EthSM_NetworkConfigType* configPtr, EthSM_NetworkVarType* varPtr)
{
    if (varPtr->WakeupRequest)
    {
        if (EthIf_SetControllerMode(configPtr->EthIfCtrlIdx, ETH_MODE_ACTIVE_WITH_WAKEUP_REQUEST) == E_OK)
        {
            varPtr->WakeupRequest       = FALSE;
            varPtr->NetworkPrivateState = ETHSM_PRIVATE_STATE_WAIT_CTRLACTIVE;
            EthSM_StateChange(configPtr, varPtr, ETHSM_STATE_WAIT_TRCVLINK);
        }
        return;
    }

    if (varPtr->ComMMode == COMM_FULL_COMMUNICATION)
    {
        if (EthIf_SetControllerMode(configPtr->EthIfCtrlIdx, ETH_MODE_ACTIVE) == E_OK)
        {
            varPtr->NetworkPrivateState = ETHSM_PRIVATE_STATE_WAIT_CTRLACTIVE;
            EthSM_StateChange(configPtr, varPtr, ETHSM_STATE_WAIT_TRCVLINK);
        }
    }
}

/**
 * @brief     Handle state transition in ETHSM_STATE_WAIT_TRCVLINK.
 * @param[in] configPtr  Pointer to the network configuration
 * @param[in] varPtr     Pointer to the network variable
 * @reentrant    TRUE (Reentrant for different networks. Non reentrant for the same network.)
 * @synchronous  TRUE
 * @trace        CPD-73938
 */
ETHSM_LOCAL void EthSM_StateWaitTrcvLink(const EthSM_NetworkConfigType* configPtr, EthSM_NetworkVarType* varPtr)
{
    if (varPtr->WakeupRequest)
    {
        if (EthIf_SetControllerMode(configPtr->EthIfCtrlIdx, ETH_MODE_ACTIVE_WITH_WAKEUP_REQUEST) == E_OK)
        {
            varPtr->WakeupRequest = FALSE;
        }
    }

    if (varPtr->ComMMode == COMM_NO_COMMUNICATION)
    {
        if (EthIf_SetControllerMode(configPtr->EthIfCtrlIdx, ETH_MODE_DOWN) == E_OK)
        {
            varPtr->NetworkPrivateState = ETHSM_PRIVATE_STATE_WAIT_CTRLDOWN;
            EthSM_StateChange(configPtr, varPtr, ETHSM_STATE_OFFLINE);
        }
        return;
    }

    if (varPtr->EthTrcvLinkState == ETHTRCV_LINK_STATE_ACTIVE)
    {
#if ETHSM_TCPIP_ENABLED == STD_ON
        EthSM_TcpIpRequestComMode(configPtr, varPtr, TCPIP_STATE_ONLINE);
#endif
        EthSM_StateChange(configPtr, varPtr, ETHSM_STATE_WAIT_ONLINE);
    }
}

/**
 * @brief     Handle state transition in ETHSM_STATE_WAIT_ONLINE.
 * @param[in] configPtr  Pointer to the network configuration
 * @param[in] varPtr     Pointer to the network variable
 * @reentrant    TRUE (Reentrant for different networks. Non reentrant for the same network.)
 * @synchronous  TRUE
 * @trace        CPD-73939
 */
ETHSM_LOCAL void EthSM_StateWaitOnline(const EthSM_NetworkConfigType* configPtr, EthSM_NetworkVarType* varPtr)
{
#if ETHSM_TCPIP_ENABLED == STD_ON
    if (varPtr->WakeupRequest)
    {
        if (EthIf_SetControllerMode(configPtr->EthIfCtrlIdx, ETH_MODE_ACTIVE_WITH_WAKEUP_REQUEST) == E_OK)
        {
            varPtr->WakeupRequest = FALSE;
        }
    }

    if (varPtr->ComMMode == COMM_NO_COMMUNICATION)
    {
        if (EthIf_SetControllerMode(configPtr->EthIfCtrlIdx, ETH_MODE_DOWN) == E_OK)
        {
            EthSM_TcpIpRequestComMode(configPtr, varPtr, TCPIP_STATE_OFFLINE);
            varPtr->NetworkPrivateState = ETHSM_PRIVATE_STATE_WAIT_CTRLDOWN;
            EthSM_StateChange(configPtr, varPtr, ETHSM_STATE_OFFLINE);
        }
        return;
    }

    if (varPtr->EthTrcvLinkState == ETHTRCV_LINK_STATE_DOWN)
    {
        EthSM_TcpIpRequestComMode(configPtr, varPtr, TCPIP_STATE_OFFLINE);
        EthSM_StateChange(configPtr, varPtr, ETHSM_STATE_WAIT_TRCVLINK);
        return;
    }

    if (varPtr->TcpIpState == TCPIP_STATE_ONLINE)
#endif
    {
        EthSM_StateChange(configPtr, varPtr, ETHSM_STATE_ONLINE);
    }
}

/**
 * @brief     Handle state transition in ETHSM_STATE_ONLINE.
 * @param[in] configPtr  Pointer to the network configuration
 * @param[in] varPtr     Pointer to the network variable
 * @reentrant    TRUE (Reentrant for different networks. Non reentrant for the same network.)
 * @synchronous  TRUE
 * @trace        CPD-73940
 */
ETHSM_LOCAL void EthSM_StateOnline(const EthSM_NetworkConfigType* configPtr, EthSM_NetworkVarType* varPtr)
{
    if (varPtr->WakeupRequest)
    {
        if (EthIf_SetControllerMode(configPtr->EthIfCtrlIdx, ETH_MODE_ACTIVE_WITH_WAKEUP_REQUEST) == E_OK)
        {
            varPtr->WakeupRequest = FALSE;
        }
    }

#if ETHSM_TCPIP_ENABLED == STD_ON
    if (varPtr->TcpIpState == TCPIP_STATE_OFFLINE)
    {
        EthSM_StateChange(configPtr, varPtr, ETHSM_STATE_WAIT_ONLINE);
        return;
    }
#endif

    if (varPtr->ComMMode == COMM_NO_COMMUNICATION)
    {
#if ETHSM_TCPIP_ENABLED == STD_ON
        EthSM_TcpIpRequestComMode(configPtr, varPtr, TCPIP_STATE_OFFLINE);
#endif
        EthSM_StateChange(configPtr, varPtr, ETHSM_STATE_WAIT_OFFLINE);
        return;
    }

    if (varPtr->ComMMode == COMM_SILENT_COMMUNICATION)
    {
        if (EthIf_SetControllerMode(configPtr->EthIfCtrlIdx, ETH_MODE_ACTIVE_TX_OFFLINE) == E_OK)
        {
            EthSM_StateChange(configPtr, varPtr, ETHSM_STATE_WAIT_OFFLINE);
        }
        return;
    }

    if (varPtr->EthTrcvLinkState == ETHTRCV_LINK_STATE_DOWN)
    {
#if ETHSM_TCPIP_ENABLED == STD_ON
        EthSM_TcpIpRequestComMode(configPtr, varPtr, TCPIP_STATE_ONHOLD);
#endif
        EthSM_StateChange(configPtr, varPtr, ETHSM_STATE_ONHOLD);
    }
}

/**
 * @brief     Handle state transition in ETHSM_STATE_ONHOLD.
 * @param[in] configPtr  Pointer to the network configuration
 * @param[in] varPtr     Pointer to the network variable
 * @reentrant    TRUE (Reentrant for different networks. Non reentrant for the same network.)
 * @synchronous  TRUE
 * @trace        CPD-73941
 */
ETHSM_LOCAL void EthSM_StateOnhold(const EthSM_NetworkConfigType* configPtr, EthSM_NetworkVarType* varPtr)
{
#if ETHSM_TCPIP_ENABLED == STD_ON
    if (varPtr->WakeupRequest)
    {
        if (EthIf_SetControllerMode(configPtr->EthIfCtrlIdx, ETH_MODE_ACTIVE_WITH_WAKEUP_REQUEST) == E_OK)
        {
            varPtr->WakeupRequest = FALSE;
        }
    }

    if (varPtr->ComMMode == COMM_NO_COMMUNICATION)
    {
        if (EthIf_SetControllerMode(configPtr->EthIfCtrlIdx, ETH_MODE_DOWN) == E_OK)
        {
            EthSM_TcpIpRequestComMode(configPtr, varPtr, TCPIP_STATE_OFFLINE);
            varPtr->NetworkPrivateState = ETHSM_PRIVATE_STATE_WAIT_CTRLDOWN;
            EthSM_StateChange(configPtr, varPtr, ETHSM_STATE_OFFLINE);
        }
        return;
    }

    if (varPtr->EthTrcvLinkState == ETHTRCV_LINK_STATE_ACTIVE)
    {
        EthSM_TcpIpRequestComMode(configPtr, varPtr, TCPIP_STATE_ONLINE);
        EthSM_StateChange(configPtr, varPtr, ETHSM_STATE_ONLINE);
        return;
    }

    if (varPtr->TcpIpState == TCPIP_STATE_OFFLINE)
#endif
    {
        EthSM_StateChange(configPtr, varPtr, ETHSM_STATE_WAIT_TRCVLINK);
    }
}

/**
 * @brief     Handle state transition in ETHSM_STATE_WAIT_OFFLINE.
 * @param[in] configPtr  Pointer to the network configuration
 * @param[in] varPtr     Pointer to the network variable
 * @reentrant    TRUE (Reentrant for different networks. Non reentrant for the same network.)
 * @synchronous  TRUE
 * @trace        CPD-73942
 */
ETHSM_LOCAL void EthSM_StateWaitOffline(const EthSM_NetworkConfigType* configPtr, EthSM_NetworkVarType* varPtr)
{
    if (varPtr->ComMMode == COMM_NO_COMMUNICATION)
    {
#if ETHSM_TCPIP_ENABLED == STD_ON
        EthSM_TcpIpRequestComMode(configPtr, varPtr, TCPIP_STATE_OFFLINE);
        if (varPtr->TcpIpState == TCPIP_STATE_OFFLINE)
#endif
        {
            if (EthIf_SetControllerMode(configPtr->EthIfCtrlIdx, ETH_MODE_DOWN) == E_OK)
            {
                varPtr->NetworkPrivateState = ETHSM_PRIVATE_STATE_WAIT_CTRLDOWN;
                EthSM_StateChange(configPtr, varPtr, ETHSM_STATE_OFFLINE);
            }
        }
        return;
    }

    if (varPtr->WakeupRequest)
    {
        if (EthIf_SetControllerMode(configPtr->EthIfCtrlIdx, ETH_MODE_ACTIVE_WITH_WAKEUP_REQUEST) == E_OK)
        {
            varPtr->WakeupRequest = FALSE;
#if ETHSM_TCPIP_ENABLED == STD_ON
            EthSM_TcpIpRequestComMode(configPtr, varPtr, TCPIP_STATE_ONLINE);
            if (varPtr->TcpIpState == TCPIP_STATE_ONLINE)
#endif
            {
                EthSM_StateChange(configPtr, varPtr, ETHSM_STATE_ONLINE);
            }
        }
        return;
    }

    if (varPtr->ComMMode == COMM_FULL_COMMUNICATION)
    {
        if (EthIf_SetControllerMode(configPtr->EthIfCtrlIdx, ETH_MODE_ACTIVE) == E_OK)
        {
#if ETHSM_TCPIP_ENABLED == STD_ON
            EthSM_TcpIpRequestComMode(configPtr, varPtr, TCPIP_STATE_ONLINE);
            if (varPtr->TcpIpState == TCPIP_STATE_ONLINE)
#endif
            {
                EthSM_StateChange(configPtr, varPtr, ETHSM_STATE_ONLINE);
            }
        }
    }
}

/**
 * @brief     Handle the state change of network state machine.
 * @param[in] configPtr  Pointer to the network configuration
 * @param[in] varPtr     Pointer to the network variable
 * @param[in] currentState      Current state of the network
 * @reentrant    TRUE (Reentrant for different networks. Non reentrant for the same network.)
 * @synchronous  TRUE
 * @trace        CPD-73943
 */
ETHSM_LOCAL void EthSM_StateChange(
    const EthSM_NetworkConfigType* configPtr,
    EthSM_NetworkVarType*          varPtr,
    EthSM_NetworkModeStateType     currentState)
{
    EthSM_NetworkModeStateType previousState = varPtr->NetworkModeState;

    varPtr->NetworkModeState = currentState;

#if ETHSM_DEM_ENABLED == STD_ON
    if ((previousState == ETHSM_STATE_ONLINE) && (currentState == ETHSM_STATE_ONHOLD))
    {
        if (configPtr->DemEventId != 0u)
        {
            (void)Dem_SetEventStatus(configPtr->DemEventId, DEM_EVENT_STATUS_FAILED);
        }
    }

    if (((previousState == ETHSM_STATE_ONHOLD) && (currentState == ETHSM_STATE_ONLINE))
        || ((previousState == ETHSM_STATE_WAIT_TRCVLINK) && (currentState == ETHSM_STATE_WAIT_ONLINE)))
    {
        if (configPtr->DemEventId != 0u)
        {
            (void)Dem_SetEventStatus(configPtr->DemEventId, DEM_EVENT_STATUS_PASSED);
        }
    }
#endif

    ComM_ModeType previousComMode = EthSM_NetworkModeState2ComMode(previousState);
    ComM_ModeType currentComMode  = EthSM_NetworkModeState2ComMode(currentState);

    if (currentComMode != previousComMode)
    {
        ComM_BusSM_ModeIndication(configPtr->ComMChannelId, currentComMode);
    }
    BswM_EthSM_CurrentState(configPtr->ComMChannelId, currentState);
}

#if ETHSM_TCPIP_ENABLED == STD_ON
/**
 * @brief     Request the communication mode for TCP/IP if the TCP/IP is used.
 * @param[in] configPtr  Pointer to the network configuration
 * @param[in] varPtr     Pointer to the network variable
 * @param[in] tcpIpState        TCP/IP state to request
 * @reentrant    TRUE (Reentrant for different networks. Non reentrant for the same network.)
 * @synchronous  TRUE
 * @trace        CPD-73944
 */
ETHSM_LOCAL void EthSM_TcpIpRequestComMode(
    const EthSM_NetworkConfigType* configPtr,
    EthSM_NetworkVarType*          varPtr,
    TcpIp_StateType                tcpIpState)
{
    if (configPtr->TcpIpUsed)
    {
        (void)TcpIp_RequestComMode(configPtr->EthIfCtrlIdx, tcpIpState);
        return;
    }
    varPtr->TcpIpState = (tcpIpState == TCPIP_STATE_ONLINE) ? TCPIP_STATE_ONLINE : TCPIP_STATE_OFFLINE;
}
#endif

#if ETHSM_DEV_ERROR_DETECT == STD_ON
/**
 * @brief     Validate the network context.
 * @param[in] apiId             API identifier
 * @param[in] networkPtr        Pointer to the network instance
 * @param[in] errorId           Error identifier
 * @retval    TRUE   The validation passed
 * @retval    FALSE  The validation failed
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73945
 */
ETHSM_LOCAL boolean EthSM_ValidateNetworkContext(uint8 apiId, const EthSM_NetworkType* networkPtr, uint8 errorId)
{
    if (networkPtr == NULL_PTR)
    {
        (void)Det_ReportError(ETHSM_MODULE_ID, 0u, apiId, errorId);
        return FALSE;
    }

#if ETHSM_MULTIPLE_PARTITION_ENABLED == STD_ON
    if (networkPtr->ConfigPtr->Application != GetApplicationID())
    {
        (void)Det_ReportError(ETHSM_MODULE_ID, 0u, apiId, ETHSM_E_INVALID_PARTITION_CONTEXT);
        return FALSE;
    }
#endif

    if (apiId != ETHSM_SID_MAINFUNCTION)
    {
        if (networkPtr->VarPtr->NetworkPrivateState == ETHSM_PRIVATE_STATE_UNINIT)
        {
            (void)Det_ReportError(ETHSM_MODULE_ID, 0u, apiId, ETHSM_E_UNINIT);
            return FALSE;
        }
    }

    return TRUE;
}

/**
 * @brief     Validate the pointer.
 * @param[in] apiId             API identifier
 * @param[in] pointer           Pointer to validate
 * @retval    TRUE   The validation passed
 * @retval    FALSE  The validation failed
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73946
 */
ETHSM_LOCAL boolean EthSM_ValidatePointer(uint8 apiId, const void* pointer)
{
    if (pointer == NULL_PTR)
    {
        (void)Det_ReportError(ETHSM_MODULE_ID, 0u, apiId, ETHSM_E_PARAM_POINTER);
        return FALSE;
    }
    return TRUE;
}

/**
 * @brief Validate the communication mode.
 * @param[in] apiId             API identifier
 * @param[in] comMMode          Communication mode to validate
 * @retval    TRUE   The validation passed
 * @retval    FALSE  The validation failed
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73947
 */
ETHSM_LOCAL boolean EthSM_ValidateComMMode(uint8 apiId, ComM_ModeType comMMode)
{
    if ((comMMode == COMM_NO_COMMUNICATION) || (comMMode == COMM_SILENT_COMMUNICATION)
        || (comMMode == COMM_FULL_COMMUNICATION) || (comMMode == COMM_FULL_COMMUNICATION_WITH_WAKEUP_REQUEST))
    {
        return TRUE;
    }
    (void)Det_ReportError(ETHSM_MODULE_ID, 0u, apiId, ETHSM_E_INVALID_NETWORK_MODE);
    return FALSE;
}

#if ETHSM_TCPIP_ENABLED == STD_ON
/**
 * @brief Validate the TCP/IP state.
 * @param[in] apiId             API identifier
 * @param[in] tcpIpState        TCP/IP state to validate
 * @retval    TRUE   The validation passed
 * @retval    FALSE  The validation failed
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73948
 */
ETHSM_LOCAL boolean EthSM_ValidateTcpIpState(uint8 apiId, TcpIp_StateType tcpIpState)
{
    if ((tcpIpState == TCPIP_STATE_ONLINE) || (tcpIpState == TCPIP_STATE_ONHOLD) || (tcpIpState == TCPIP_STATE_OFFLINE)
        || (tcpIpState == TCPIP_STATE_STARTUP) || (tcpIpState == TCPIP_STATE_SHUTDOWN))
    {
        return TRUE;
    }
    (void)Det_ReportError(ETHSM_MODULE_ID, 0u, apiId, ETHSM_E_INVALID_TCP_IP_MODE);
    return FALSE;
}
#endif

/**
 * @brief     Validate the Ethernet mode.
 * @param[in] apiId             API identifier
 * @param[in] ethMode           Ethernet mode to validate
 * @retval    TRUE   The validation passed
 * @retval    FALSE  The validation failed
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73949
 */
ETHSM_LOCAL boolean EthSM_ValidateEthMode(uint8 apiId, Eth_ModeType ethMode)
{
    if ((ethMode == ETH_MODE_ACTIVE) || (ethMode == ETH_MODE_DOWN) || (ethMode == ETH_MODE_ACTIVE_TX_OFFLINE)
        || (ethMode == ETH_MODE_ACTIVE_WITH_WAKEUP_REQUEST))
    {
        return TRUE;
    }
    (void)Det_ReportError(ETHSM_MODULE_ID, 0u, apiId, ETHSM_E_INVALID_ETH_MODE);
    return FALSE;
}

/**
 * @brief     Validate the transceiver link state.
 * @param[in] apiId             API identifier
 * @param[in] ethTrcvLinkState  Transceiver link state to validate
 * @retval    TRUE   The validation passed
 * @retval    FALSE  The validation failed
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73950
 */
ETHSM_LOCAL boolean EthSM_ValidateTransceiverLinkState(uint8 apiId, EthTrcv_LinkStateType ethTrcvLinkState)
{
    if ((ethTrcvLinkState == ETHTRCV_LINK_STATE_ACTIVE) || (ethTrcvLinkState == ETHTRCV_LINK_STATE_DOWN))
    {
        return TRUE;
    }
    (void)Det_ReportError(ETHSM_MODULE_ID, 0u, apiId, ETHSM_E_INVALID_ETH_TRCV_LINK_STATE);
    return FALSE;
}
#endif /* ETHSM_DEV_ERROR_DETECT == STD_ON */
#define ETHSM_STOP_SEC_CODE
#include "EthSM_MemMap.h"
/* PRQA S 1532, 2889 -- */ /* VL_QAC_OneFunRef, VL_QAC_MultiReturn */
/* PRQA S 6060 -- */       /* VL_MTR_EthSM_STM19 */
