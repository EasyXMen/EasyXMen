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
**  FILENAME    : EthSM.c                                                     **
**                                                                            **
**  Created on  :                                                             **
**  Author      : HuRongbo                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/* <VERSION>   <DATE>     <AUTHOR>       <REVISION LOG>
 * V2.0.0      20200527   HuRongbo       Initial version
 * V2.0.1      20210513   HuRongbo       Bug fix
 * V2.0.2      20220711   HuRongbo       QAC check issue fix
 * V2.0.3      20220921   HuRongbo
 *   1> Add check if module initialized in EthSM_MainFunction()
 * V2.0.4      20231123 xiaojian.liang Performance optimization
 * V2.0.5      20240229 shengnan.sun   QAC_PH_2024 check.
 *                                   EthSM is UNINIT,NetworkHandle is NULL_PTR.
 * V2.0.6      20240309   SunShengnan    Support multiple partition feature.
 */

/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:EthSM<br>
  RuleSorce:puhua-rule.rcf 2.3.1

    \li PRQA S 2877 MISRA Dir 4.1 .<br>
    Reason:The exception is that the number of loops is configurable

    \li PRQA S 1532 MISRA Rule 8.7 .<br>
    Reason:The exception is API(1532).
 */
/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/

#define ETHSM_C_AR_MAJOR_VERSION 4
#define ETHSM_C_AR_MINOR_VERSION 5
#define ETHSM_C_AR_PATCH_VERSION 0
#define ETHSM_C_SW_MAJOR_VERSION 2
#define ETHSM_C_SW_MINOR_VERSION 0
#define ETHSM_C_SW_PATCH_VERSION 6

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "EthSM.h"
#include "EthSM_TcpIp.h"
/*If it hasnot the Runtime det error*/
#if (STD_ON == ETHSM_DEV_ERROR_DETECT)
#include "Det.h"
/* Multiple partition check. */
#include "Os.h"
#if (STD_ON == ETHSM_MULTIPLE_PARTITION_USED)
#include "ComM.h"
#endif
#endif /* STD_ON == ETHSM_DEV_ERROR_DETECT */
#if (STD_ON == ETHSM_DEM_SUPPORT)
#include "Dem.h"
#endif /* STD_ON == ETHSM_DEM_SUPPORT */
#include "BswM_EthSM.h"
#include "ComM_BusSM.h"
#include "EthIf.h"
#include "TcpIp.h"

/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#if (ETHSM_C_AR_MAJOR_VERSION != ETHSM_H_AR_MAJOR_VERSION)
#error "EthSM.c : Mismatch in Specification Major Version"
#endif
#if (ETHSM_C_AR_MINOR_VERSION != ETHSM_H_AR_MINOR_VERSION)
#error "EthSM.c : Mismatch in Specification _MINOR Version"
#endif
#if (ETHSM_C_AR_PATCH_VERSION != ETHSM_H_AR_PATCH_VERSION)
#error "EthSM.c : Mismatch in Specification PATCH Version"
#endif
#if (ETHSM_C_SW_MAJOR_VERSION != ETHSM_H_SW_MAJOR_VERSION)
#error "EthSM.c : Mismatch in Software Major Version"
#endif
#if (ETHSM_C_SW_MINOR_VERSION != ETHSM_H_SW_MINOR_VERSION)
#error "EthSM.c : Mismatch in Software MINOR Version"
#endif
/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
#ifndef ETHSM_LOCAL
#define ETHSM_LOCAL static
#endif

#define EthSM_GetComMNetworkId(ch)     EthSM_ConfigPtr->EthSMNetwork[ch].EthSMComMNetworkHandleRef

#define EthSM_GetEthIfControllerId(ch) EthSM_ConfigPtr->EthSMNetwork[ch].EthSMEthIfControllerRef

#if (STD_ON == ETHSM_DEM_SUPPORT)
#define EthSM_GetDemEventParameterRef(ch) EthSM_ConfigPtr->EthSMNetwork[ch].EthSMDemEventParameterRefs
#endif

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/
/* Sub State define,Intermediate state when state change to another */
typedef enum
{
    ETHSM_SUBSTATE_NONE,
    ETHSM_SUBSTATE_WAIT_CTRL_ACTIVE,
    ETHSM_SUBSTATE_WAIT_CTRL_DOWN,
    ETHSM_SUBSTATE_WAIT_TCPIP_ONLINE,
    ETHSM_SUBSTATE_WAIT_TCPIP_OFFLINE,
    ETHSM_SUBSTATE_WAIT_TCPIP_ONHOLD,
    ETHSM_SUBSTATE_WAIT_CTRL_TCPIP_CLOSE
} EthSM_SubState;

/*Define the values of the internal states of the EthSM module*/
typedef enum
{
    ETHSM_UNINITED,
    ETHSM_INIT
} EthSM_StateType;

/* Define runtime value */
typedef struct
{
    /* ComM requested communication mode */
    ComM_ModeType requestComMode;
    /* Ethernet Interface Controller mode */
    Eth_ModeType ctrlMode;
    /* Actual transceiver link state of the specific network handle */
    EthTrcv_LinkStateType trcvLinkState;
    /* Actual TcpIp state of the specific network handle */
    TcpIp_StateType tcpipState;
    /* NetWork current BSM state */
    EthSM_NetworkModeStateType curBsmState;
    /* Sub State */
    EthSM_SubState subState;
} EthSM_RunTimeType;

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define ETHSM_START_SEC_CODE
#include "EthSM_MemMap.h"

static FUNC(void, ETHSM_CODE) EthSM_StateOffline(NetworkHandleType ch);

static FUNC(void, ETHSM_CODE) EthSM_StateWaitTrcvLink(NetworkHandleType ch);

static FUNC(void, ETHSM_CODE) EthSM_StateWaitOnline(NetworkHandleType ch);

static FUNC(void, ETHSM_CODE) EthSM_StateOnline(NetworkHandleType ch);

static FUNC(void, ETHSM_CODE) EthSM_StateOnhold(NetworkHandleType ch);

static FUNC(void, ETHSM_CODE) EthSM_StateWaitOffline(NetworkHandleType ch);

ETHSM_LOCAL FUNC(NetworkHandleType, ETHSM_CODE) EthSM_GetChannelByNetworkHandle(NetworkHandleType NetworkHandle);

static FUNC(NetworkHandleType, ETHSM_CODE) EthSM_GetChannelByCtrlIdx(uint8 CtrlIdx);

#define ETHSM_STOP_SEC_CODE
#include "EthSM_MemMap.h"
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
#define ETHSM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthSM_MemMap.h"
/* EthSM runtime data */
static VAR(EthSM_RunTimeType, ETHSM_VAR) EthSM_RTData[ETHSM_CHANNEL_NUM];
#define ETHSM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthSM_MemMap.h"

#define ETHSM_START_SEC_VAR_INIT_PTR
#include "EthSM_MemMap.h"
/* Pointer to configuration data */
static P2CONST(EthSM_ConfigType, ETHSM_VAR, ETHSM_CONST) EthSM_ConfigPtr = NULL_PTR;
#define ETHSM_STOP_SEC_VAR_INIT_PTR
#include "EthSM_MemMap.h"

#define ETHSM_START_SEC_VAR_INIT_UNSPECIFIED
#include "EthSM_MemMap.h"
/* Variable store EthSM initialization status */
static VAR(EthSM_StateType, ETHSM_VAR_POWER_ON_INIT) EthSM_Status = ETHSM_UNINITED;
#define ETHSM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "EthSM_MemMap.h"

/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define ETHSM_START_SEC_CODE
#include "EthSM_MemMap.h"
/******************************************************************************/
/*
 * Brief               This service initializes the EthSM module.
 * ServiceId           0x07
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, ETHSM_CODE) EthSM_Init(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint8 ch;
    SchM_Enter_EthSM_Context();
    /* Initialize run time structure variable for every network */
    for (ch = 0u; ch < ETHSM_CHANNEL_NUM; ch++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
    {
        EthSM_RunTimeType* runTimePtr = &EthSM_RTData[ch];
        runTimePtr->requestComMode = COMM_NO_COMMUNICATION;
        runTimePtr->ctrlMode = ETH_MODE_DOWN;
        runTimePtr->trcvLinkState = ETHTRCV_LINK_STATE_DOWN;
        runTimePtr->tcpipState = TCPIP_STATE_OFFLINE;
        /* @req SWS_EthSM_00025 */
        /* After the initialization of the EthSM the state machine shall have a
         * transition to ETHSM_STATE_OFFLINE.
         */
        runTimePtr->curBsmState = ETHSM_STATE_OFFLINE;
        runTimePtr->subState = ETHSM_SUBSTATE_NONE;
    }

    /* Initialize config data pointer*/
    EthSM_ConfigPtr = &EthSM_Config;
    EthSM_Status = ETHSM_INIT;
    SchM_Exit_EthSM_Context();
}

/******************************************************************************/
/*
 * Brief               Output the version information of this module.
 * ServiceId           0x02
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     VersionInfo: Pointer to where to store the version
 *                                  information of this module
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if (STD_ON == ETHSM_VERSION_INFO_API)
FUNC(void, ETHSM_CODE)
EthSM_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, AUTOMATIC) VersionInfo) /* PRQA S 3432 */
{
#if (STD_ON == ETHSM_DEV_ERROR_DETECT)
    if (NULL_PTR == VersionInfo)
    {
        (void)Det_ReportError(ETHSM_MODULE_ID, ETHSM_INSTANCE_ID, ETHSM_SID_GETVERSIONINFO, ETHSM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == ETHSM_DEV_ERROR_DETECT */
    {
        VersionInfo->vendorID = ETHSM_VENDOR_ID;
        VersionInfo->moduleID = ETHSM_MODULE_ID;
        VersionInfo->sw_major_version = ETHSM_C_SW_MAJOR_VERSION;
        VersionInfo->sw_minor_version = ETHSM_C_SW_MINOR_VERSION;
        VersionInfo->sw_patch_version = ETHSM_C_SW_PATCH_VERSION;
    }
}
#endif /* STD_ON == EthSM_VERSION_INFO_API */

/******************************************************************************/
/*
 * Brief               Handles the communication mode and sets the Ethernet
 *                     network active or passive.
 * ServiceId           0x05
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      NetworkHandle: Handle of destinated communication network
 *                                    for request.
 *                     ComM_Mode: Requested communication mode.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Service accepted
 *                     E_NOT_OK: Service denied
 */
/******************************************************************************/
FUNC(Std_ReturnType, ETHSM_CODE)
EthSM_RequestComMode(NetworkHandleType NetworkHandle, ComM_ModeType ComM_Mode) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHSM_DEV_ERROR_DETECT)
    /* Multiple partition check. */
#if (STD_ON == ETHSM_MULTIPLE_PARTITION_USED)
    ApplicationType applicationID = GetApplicationID();
#endif
    if (ETHSM_UNINITED == EthSM_Status)
    {
        /* @req SWS_EthSM_00054 */
        (void)Det_ReportError(ETHSM_MODULE_ID, ETHSM_INSTANCE_ID, ETHSM_SID_REQUESTCOMMODE, ETHSM_E_UNINIT);
    }
    else if (
        (COMM_NO_COMMUNICATION != ComM_Mode) && (COMM_SILENT_COMMUNICATION != ComM_Mode)
        && (COMM_FULL_COMMUNICATION != ComM_Mode))
    {
        /* @req SWS_EthSM_00095 */
        (void)
            Det_ReportError(ETHSM_MODULE_ID, ETHSM_INSTANCE_ID, ETHSM_SID_REQUESTCOMMODE, ETHSM_E_INVALID_NETWORK_MODE);
    }
    else
#endif /* STD_ON == ETHSM_DEV_ERROR_DETECT */
    {
        NetworkHandleType ch = EthSM_GetChannelByNetworkHandle(NetworkHandle);
#if (STD_ON == ETHSM_DEV_ERROR_DETECT)
        if (ch >= ETHSM_CHANNEL_NUM)
        {
            /* @req SWS_EthSM_00051,SWS_EthSM_00052 */
            (void)Det_ReportError(
                ETHSM_MODULE_ID,
                ETHSM_INSTANCE_ID,
                ETHSM_SID_REQUESTCOMMODE,
                ETHSM_E_INVALID_NETWORK_HANDLE);
        }
        else
#if (STD_ON == ETHSM_MULTIPLE_PARTITION_USED)
            /* Multiple partition check. */
            if (applicationID != ComM_GetChannelApplicationID(NetworkHandle))
            {
                (void)Det_ReportError(ETHSM_MODULE_ID, ETHSM_INSTANCE_ID, ETHSM_SID_REQUESTCOMMODE, ETHSM_E_PARTITION);
            }
            else
#endif
#endif /* STD_ON == ETHSM_DEV_ERROR_DETECT */
            {
/* @req ECUC_EthSM_00079 */
#if (STD_OFF == ETHSM_DUMMY_MODE)
                /* @req SWS_EthSM_00053 */
                /*Store the communication mode for the network handle except SilentCom*/
                if (COMM_SILENT_COMMUNICATION != ComM_Mode)
                {
                    SchM_Enter_EthSM_Context();
                    EthSM_RTData[ch].requestComMode = ComM_Mode;
                    SchM_Exit_EthSM_Context();
                }
#endif /* STD_OFF == ETHSM_DUMMY_MODE */

                /* @req SWS_EthSM_00051,SWS_EthSM_00199 */
                ret = E_OK;
            }
    }
    return ret;
}

/******************************************************************************/
/*
 * Brief               This service shall put out the current communication mode
 *                     of a Ethernet network.
 * ServiceId           0x04
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      NetworkHandle: Network handle whose current communication
 *                                    mode shall be put out
 * Param-Name[out]     ComM_ModePtr: Pointer where to put out the current
 *                                   communication mode
 * Param-Name[in/out]  None
 * Return              E_OK: Service accepted
 *                     E_NOT_OK: Service denied
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
FUNC(Std_ReturnType, ETHSM_CODE)
EthSM_GetCurrentComMode(NetworkHandleType NetworkHandle, ComM_ModeType* ComM_ModePtr)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHSM_DEV_ERROR_DETECT)
    if (ETHSM_UNINITED == EthSM_Status)
    {
        /* @req SWS_EthSM_00060 */
        (void)Det_ReportError(ETHSM_MODULE_ID, ETHSM_INSTANCE_ID, ETHSM_SID_GETCURRENTCOMMODE, ETHSM_E_UNINIT);
    }
    else
#endif /* STD_ON == ETHSM_DEV_ERROR_DETECT */
    {
        NetworkHandleType ch = EthSM_GetChannelByNetworkHandle(NetworkHandle);
#if (STD_ON == ETHSM_DEV_ERROR_DETECT)
/* Multiple partition check. */
#if (STD_ON == ETHSM_MULTIPLE_PARTITION_USED)
        ApplicationType applicationID = GetApplicationID();
#endif
        if (ch >= ETHSM_CHANNEL_NUM)
        {
            /* @req SWS_EthSM_00058 */
            (void)Det_ReportError(
                ETHSM_MODULE_ID,
                ETHSM_INSTANCE_ID,
                ETHSM_SID_GETCURRENTCOMMODE,
                ETHSM_E_INVALID_NETWORK_HANDLE);
        }
        else
#if (STD_ON == ETHSM_MULTIPLE_PARTITION_USED)
            /* Multiple partition check. */
            if (applicationID != ComM_GetChannelApplicationID(NetworkHandle))
            {
                (void)
                    Det_ReportError(ETHSM_MODULE_ID, ETHSM_INSTANCE_ID, ETHSM_SID_GETCURRENTCOMMODE, ETHSM_E_PARTITION);
            }
            else

#endif
#endif /* STD_ON == ETHSM_DEV_ERROR_DETECT */
            {
                /* @req SWS_EthSM_00057,SWS_EthSM_00059 */
                /* Output the current communication mode for the network handle */
                switch (EthSM_RTData[ch].curBsmState)
                {
                case ETHSM_STATE_ONLINE:
                case ETHSM_STATE_ONHOLD:
                case ETHSM_STATE_WAIT_OFFLINE:
                    *ComM_ModePtr = COMM_FULL_COMMUNICATION;
                    break;
                default:
                    *ComM_ModePtr = COMM_NO_COMMUNICATION;
                    break;
                }
                /* @req SWS_EthSM_00057 */
                ret = E_OK;
            }
    }
    return ret;
}
#define ETHSM_STOP_SEC_CODE
#include "EthSM_MemMap.h"

#define ETHSM_START_SEC_ETHSMCTRLMODEINDICATION_CALLBACK_CODE
#include "EthSM_MemMap.h"
/******************************************************************************/
/*
 * Brief               Called when mode has been read out.
 * ServiceId           0x09
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant (only for different Ethernet controllers)
 * Param-Name[in]      CtrlIdx: Ethernet Interface Controller whose mode has
 *                              changed.
 *                     CtrlMode: Notified Ethernet Interface Controller mode.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, ETHSM_CODE)
EthSM_CtrlModeIndication(uint8 CtrlIdx, Eth_ModeType CtrlMode) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (STD_ON == ETHSM_DEV_ERROR_DETECT)
    if (ETHSM_UNINITED == EthSM_Status)
    {
        /* @req SWS_EthSM_00192 */
        (void)Det_ReportError(ETHSM_MODULE_ID, ETHSM_INSTANCE_ID, ETHSM_SID_CTRLMODEINDICATION, ETHSM_E_UNINIT);
    }
    else
#endif /* STD_ON == ETHSM_DEV_ERROR_DETECT */
    {
        NetworkHandleType ch = EthSM_GetChannelByCtrlIdx(CtrlIdx);
#if (STD_ON == ETHSM_DEV_ERROR_DETECT)
        if (ch >= ETHSM_CHANNEL_NUM)
        {
            /* @req SWS_EthSM_00191 */
            (void)Det_ReportError(
                ETHSM_MODULE_ID,
                ETHSM_INSTANCE_ID,
                ETHSM_SID_CTRLMODEINDICATION,
                ETHSM_E_PARAM_CONTROLLER);
        }
        else
#endif /* STD_ON == ETHSM_DEV_ERROR_DETECT */
        {
            SchM_Enter_EthSM_Context();
            EthSM_RTData[ch].ctrlMode = CtrlMode;
            SchM_Exit_EthSM_Context();
        }
    }
}
#define ETHSM_STOP_SEC_ETHSMCTRLMODEINDICATION_CALLBACK_CODE
#include "EthSM_MemMap.h"

#define ETHSM_START_SEC_ETHSMTRCVLINKSTATECHG_CALLBACK_CODE
#include "EthSM_MemMap.h"
/******************************************************************************/
/*
 * Brief               This service is called by the Ethernet Interface to
 *                     report a transceiver link state change.
 * ServiceId           0x06
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CtrlIdx: Index of the Ethernet controller within the
 *                              context of the Ethernet Interface
 *                     TransceiverLinkState: Actual transceiver link state of
 *                                           the specific network handle
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, ETHSM_CODE)
EthSM_TrcvLinkStateChg(uint8 CtrlIdx, EthTrcv_LinkStateType TransceiverLinkState) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (STD_ON == ETHSM_DEV_ERROR_DETECT)
    if (ETHSM_UNINITED == EthSM_Status)
    {
        /* @req SWS_EthSM_00115 */
        (void)Det_ReportError(ETHSM_MODULE_ID, ETHSM_INSTANCE_ID, ETHSM_SID_TRCVLINKSTATECHG, ETHSM_E_UNINIT);
    }
    else
#endif /* STD_ON == ETHSM_DEV_ERROR_DETECT */
    {
        NetworkHandleType ch = EthSM_GetChannelByCtrlIdx(CtrlIdx);
#if (STD_ON == ETHSM_DEV_ERROR_DETECT)
        if (ch >= ETHSM_CHANNEL_NUM)
        {
            /* @req SWS_EthSM_00112 */
            (void)Det_ReportError(
                ETHSM_MODULE_ID,
                ETHSM_INSTANCE_ID,
                ETHSM_SID_TRCVLINKSTATECHG,
                ETHSM_E_PARAM_CONTROLLER);
        }
        else
#endif /* STD_ON == ETHSM_DEV_ERROR_DETECT */
        {
            /* @req SWS_EthSM_00114 */
            /* Store the transceiver link state for the affected network handle */
            SchM_Enter_EthSM_Context();
            EthSM_RTData[ch].trcvLinkState = TransceiverLinkState;
            SchM_Exit_EthSM_Context();
        }
    }
}
#define ETHSM_STOP_SEC_ETHSMTRCVLINKSTATECHG_CALLBACK_CODE
#include "EthSM_MemMap.h"

#define ETHSM_START_SEC_ETHSMTCPIPMODEINDICATION_CALLBACK_CODE
#include "EthSM_MemMap.h"
/******************************************************************************/
/*
 * Brief               This service is called by the TcpIp to report the actual
 *                     TcpIp state (e.g. online,offline).
 * ServiceId           0x08
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CtrlIdx: EthIf controller index to identify the
 *                     communication network where the TcpIp state is changed.
 *                     TcpIpState: Actual TcpIp state of the specific network
 *                                 handle.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, ETHSM_CODE)
EthSM_TcpIpModeIndication(uint8 CtrlIdx, TcpIp_StateType TcpIpState) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (STD_ON == ETHSM_DEV_ERROR_DETECT)
    if (ETHSM_UNINITED == EthSM_Status)
    {
        /* @req SWS_EthSM_00120 */
        (void)Det_ReportError(ETHSM_MODULE_ID, ETHSM_INSTANCE_ID, ETHSM_SID_TCPIPMODEINDICATION, ETHSM_E_UNINIT);
    }
    else
#endif /* STD_ON == ETHSM_DEV_ERROR_DETECT */
    {
        NetworkHandleType ch = EthSM_GetChannelByCtrlIdx(CtrlIdx);
#if (STD_ON == ETHSM_DEV_ERROR_DETECT)
        if ((TCPIP_STATE_ONLINE != TcpIpState) && (TCPIP_STATE_ONHOLD != TcpIpState)
            && (TCPIP_STATE_OFFLINE != TcpIpState) && (TCPIP_STATE_STARTUP != TcpIpState)
            && (TCPIP_STATE_SHUTDOWN != TcpIpState))
        {
            /* @req SWS_EthSM_00118 */
            (void)Det_ReportError(
                ETHSM_MODULE_ID,
                ETHSM_INSTANCE_ID,
                ETHSM_SID_TCPIPMODEINDICATION,
                ETHSM_E_INVALID_TCP_IP_MODE);
        }
        else if (ch >= ETHSM_CHANNEL_NUM)
        {
            /* @req SWS_EthSM_00112 */
            (void)Det_ReportError(
                ETHSM_MODULE_ID,
                ETHSM_INSTANCE_ID,
                ETHSM_SID_TCPIPMODEINDICATION,
                ETHSM_E_PARAM_CONTROLLER);
        }
        else
#endif /* STD_ON == ETHSM_DEV_ERROR_DETECT */
        {
            /* @req SWS_EthSM_00119 */
            /* Store the TcpIp state for the affected network handle */
            SchM_Enter_EthSM_Context();
            EthSM_RTData[ch].tcpipState = TcpIpState;
            SchM_Exit_EthSM_Context();
        }
    }
}
#define ETHSM_STOP_SEC_ETHSMTCPIPMODEINDICATION_CALLBACK_CODE
#include "EthSM_MemMap.h"

#define ETHSM_START_SEC_CODE
#include "EthSM_MemMap.h"
/******************************************************************************/
/*
 * Brief               Cyclic Main Function which is called from the Scheduler.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, ETHSM_CODE) EthSM_MainFunction(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    NetworkHandleType ch;
    EthSM_NetworkModeStateType curBsmSt;

    if (ETHSM_INIT == EthSM_Status)
    {
        for (ch = 0u; ch < ETHSM_CHANNEL_NUM; ch++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
        {
            SchM_Enter_EthSM_Context();
            curBsmSt = EthSM_RTData[ch].curBsmState;
            SchM_Exit_EthSM_Context();
            switch (curBsmSt)
            {
            case ETHSM_STATE_OFFLINE:
                EthSM_StateOffline(ch);
                break;
            case ETHSM_STATE_WAIT_TRCVLINK:
                EthSM_StateWaitTrcvLink(ch);
                break;
            case ETHSM_STATE_WAIT_ONLINE:
                EthSM_StateWaitOnline(ch);
                break;
            case ETHSM_STATE_ONLINE:
                EthSM_StateOnline(ch);
                break;
            case ETHSM_STATE_ONHOLD:
                EthSM_StateOnhold(ch);
                break;
            case ETHSM_STATE_WAIT_OFFLINE:
                EthSM_StateWaitOffline(ch);
                break;
            default:
                /* Other state */
                break;
            }
        }
    }
}
#define ETHSM_STOP_SEC_CODE
#include "EthSM_MemMap.h"

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#define ETHSM_START_SEC_CODE
#include "EthSM_MemMap.h"
/******************************************************************************/
/*
 * Brief               Handle state transition in ETHSM_STATE_OFFLINE
 * Sync/Async          Synchronous
 * Reentrancy          None Reentrant
 * Param-Name[in]      ch: EthSM Channel(EthSM Network)
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, ETHSM_CODE) EthSM_StateOffline(NetworkHandleType ch)
{
    SchM_Enter_EthSM_Context();
    EthSM_RunTimeType* chPtr = &EthSM_RTData[ch];
    EthSM_SubState subState = chPtr->subState;
    ComM_ModeType requestComMode = chPtr->requestComMode;
    Eth_ModeType ctrlMode = chPtr->ctrlMode;
    SchM_Exit_EthSM_Context();
    Std_ReturnType ret;
    NetworkHandleType network;
    uint8 ctrlIdx;

    if (ETHSM_SUBSTATE_NONE == subState)
    {
        /* @req SWS_EthSM_00026 */
        /* Transition from substate OFFLINE to WAIT_TRCVLINK */
        if (COMM_FULL_COMMUNICATION == requestComMode)
        {
            /* @req SWS_EthSM_00088 */
            ctrlIdx = EthSM_GetEthIfControllerId(ch);
            ret = EthIf_SetControllerMode(ctrlIdx, ETH_MODE_ACTIVE);
            if (E_OK == ret)
            {
                SchM_Enter_EthSM_Context();
                chPtr->subState = ETHSM_SUBSTATE_WAIT_CTRL_ACTIVE;
                SchM_Exit_EthSM_Context();
            }
        }
    }
    else if (ETHSM_SUBSTATE_WAIT_CTRL_ACTIVE == subState)
    {
        if (ETH_MODE_ACTIVE == ctrlMode)
        {
            SchM_Enter_EthSM_Context();
            chPtr->curBsmState = ETHSM_STATE_WAIT_TRCVLINK;
            chPtr->subState = ETHSM_SUBSTATE_NONE;
            SchM_Exit_EthSM_Context();

            /* @req SWS_EthSM_00097 */
            network = EthSM_GetComMNetworkId(ch);
            BswM_EthSM_CurrentState(network, ETHSM_STATE_WAIT_TRCVLINK);
        }
    }
    else
    {
        /* Do nothing here */
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in ETHSM_STATE_WAIT_TRCVLINK
 * Sync/Async          Synchronous
 * Reentrancy          None Reentrant
 * Param-Name[in]      ch: EthSM Channel(EthSM Network)
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, ETHSM_CODE) EthSM_StateWaitTrcvLink(NetworkHandleType ch)
{
    SchM_Enter_EthSM_Context();
    EthSM_RunTimeType* chPtr = &EthSM_RTData[ch];
    EthSM_SubState subState = chPtr->subState;
    ComM_ModeType requestComMode = chPtr->requestComMode;
    EthTrcv_LinkStateType trcvLinkState = chPtr->trcvLinkState;
    Eth_ModeType ctrlMode = chPtr->ctrlMode;
    SchM_Exit_EthSM_Context();
    Std_ReturnType ret;
    NetworkHandleType network = EthSM_GetComMNetworkId(ch);
    uint8 ctrlIdx = EthSM_GetEthIfControllerId(ch);

    if (ETHSM_SUBSTATE_NONE == subState)
    {
        /* @req SWS_EthSM_00127 */
        /* Transition from substate WAIT_TRCVLINK to OFFLINE */
        if (COMM_NO_COMMUNICATION == requestComMode)
        {
            /* @req SWS_EthSM_00128 */
            ret = EthIf_SetControllerMode(ctrlIdx, ETH_MODE_DOWN);
            if (E_OK == ret)
            {
                SchM_Enter_EthSM_Context();
                chPtr->subState = ETHSM_SUBSTATE_WAIT_CTRL_DOWN;
                SchM_Exit_EthSM_Context();
            }
        }
        else
        {
            /* @req SWS_EthSM_00132 */
            /* Transition from substate WAIT_TRCVLINK to WAIT_ONLINE */
            if (ETHTRCV_LINK_STATE_ACTIVE == trcvLinkState)
            {
                /* @req SWS_EthSM_00133 */
                ret = TcpIp_RequestComMode(ctrlIdx, TCPIP_STATE_ONLINE);

                if (E_OK == ret)
                {
                    SchM_Enter_EthSM_Context();
                    chPtr->curBsmState = ETHSM_STATE_WAIT_ONLINE;
                    SchM_Exit_EthSM_Context();

                    /* @req SWS_EthSM_00134 */
                    BswM_EthSM_CurrentState(network, ETHSM_STATE_WAIT_ONLINE);
                }
            }
        }
    }
    else if (ETHSM_SUBSTATE_WAIT_CTRL_DOWN == subState)
    {
        if (ETH_MODE_DOWN == ctrlMode)
        {
            SchM_Enter_EthSM_Context();
            chPtr->curBsmState = ETHSM_STATE_OFFLINE;
            chPtr->subState = ETHSM_SUBSTATE_NONE;
            SchM_Exit_EthSM_Context();

            /* @req SWS_EthSM_00130 */
            BswM_EthSM_CurrentState(network, ETHSM_STATE_OFFLINE);
        }
    }
    else
    {
        /* Do nothing here */
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in ETHSM_STATE_WAIT_ONLINE
 * Sync/Async          Synchronous
 * Reentrancy          None Reentrant
 * Param-Name[in]      ch: EthSM Channel(EthSM Network)
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, ETHSM_CODE) EthSM_StateWaitOnline(NetworkHandleType ch)
{
    SchM_Enter_EthSM_Context();
    EthSM_RunTimeType* chPtr = &EthSM_RTData[ch];
    EthSM_SubState subState = chPtr->subState;
    ComM_ModeType requestComMode = chPtr->requestComMode;
    EthTrcv_LinkStateType trcvLinkState = chPtr->trcvLinkState;
    Eth_ModeType ctrlMode = chPtr->ctrlMode;
    TcpIp_StateType tcpipState = chPtr->tcpipState;
    SchM_Exit_EthSM_Context();
    Std_ReturnType ret;
    Std_ReturnType ret2;
    NetworkHandleType network = EthSM_GetComMNetworkId(ch);
    uint8 ctrlIdx = EthSM_GetEthIfControllerId(ch);
    switch (subState)
    {

    case ETHSM_SUBSTATE_NONE:
        /* @req SWS_EthSM_00140*/
        /* Transition from substate WAIT_ONLINE to OFFLINE */
        if (COMM_NO_COMMUNICATION == requestComMode)
        {
            /* @req SWS_EthSM_00141 */
            ret = EthIf_SetControllerMode(ctrlIdx, ETH_MODE_DOWN);

            /* @req SWS_EthSM_00143 */
            ret2 = TcpIp_RequestComMode(ctrlIdx, TCPIP_STATE_OFFLINE);
            if ((E_OK == ret) && (E_OK == ret2))
            {
                SchM_Enter_EthSM_Context();
                chPtr->subState = ETHSM_SUBSTATE_WAIT_CTRL_TCPIP_CLOSE;
                SchM_Exit_EthSM_Context();
            }
        }
        else if (ETHTRCV_LINK_STATE_DOWN == trcvLinkState)
        {
            /* @req SWS_EthSM_00136 */
            /* Transition from substate WAIT_ONLINE to WAIT_TRCVLINK */

            /* @req SWS_EthSM_00137 */
            ret = TcpIp_RequestComMode(ctrlIdx, TCPIP_STATE_OFFLINE);
            if (E_OK == ret)
            {
                SchM_Enter_EthSM_Context();
                chPtr->subState = ETHSM_SUBSTATE_WAIT_TCPIP_OFFLINE;
                SchM_Exit_EthSM_Context();
            }
        }
        else if (TCPIP_STATE_ONLINE == tcpipState)
        {
            /* @req SWS_EthSM_00146 */
            /* Transition from substate WAIT_ONLINE to ONLINE */
            SchM_Enter_EthSM_Context();
            chPtr->curBsmState = ETHSM_STATE_ONLINE;
            SchM_Exit_EthSM_Context();

            /* @req SWS_EthSM_00148 */
            BswM_EthSM_CurrentState(network, ETHSM_STATE_ONLINE);
            /* @req SWS_EthSM_00150 */
            ComM_BusSM_ModeIndication(network, COMM_FULL_COMMUNICATION);
        }
        else
        {
            /* Do nothing,Just for MISRA-C check */
        }
        break;
    case ETHSM_SUBSTATE_WAIT_CTRL_TCPIP_CLOSE:
        if ((ETH_MODE_DOWN == ctrlMode) && (TCPIP_STATE_OFFLINE == tcpipState))
        {
            SchM_Enter_EthSM_Context();
            chPtr->subState = ETHSM_SUBSTATE_NONE;
            chPtr->curBsmState = ETHSM_STATE_OFFLINE;
            SchM_Exit_EthSM_Context();
            /* @req SWS_EthSM_00144 */
            BswM_EthSM_CurrentState(network, ETHSM_STATE_OFFLINE);
        }
        break;
    case ETHSM_SUBSTATE_WAIT_TCPIP_OFFLINE:
        if (TCPIP_STATE_OFFLINE == tcpipState)
        {
            SchM_Enter_EthSM_Context();
            chPtr->subState = ETHSM_SUBSTATE_NONE;
            chPtr->curBsmState = ETHSM_STATE_WAIT_TRCVLINK;
            SchM_Exit_EthSM_Context();
            /* @req SWS_EthSM_00138 */
            BswM_EthSM_CurrentState(network, ETHSM_STATE_WAIT_TRCVLINK);
        }
        break;
    default:
        /* do nothing */
        break;
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in ETHSM_STATE_ONLINE
 * Sync/Async          Synchronous
 * Reentrancy          None Reentrant
 * Param-Name[in]      ch: EthSM Channel(EthSM Network)
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, ETHSM_CODE) EthSM_StateOnline(NetworkHandleType ch)
{
    SchM_Enter_EthSM_Context();
    EthSM_RunTimeType* chPtr = &EthSM_RTData[ch];
    EthSM_SubState subState = chPtr->subState;
    ComM_ModeType requestComMode = chPtr->requestComMode;
    EthTrcv_LinkStateType trcvLinkState = chPtr->trcvLinkState;
    TcpIp_StateType tcpipState = chPtr->tcpipState;
    SchM_Exit_EthSM_Context();
    Std_ReturnType ret;
    NetworkHandleType network = EthSM_GetComMNetworkId(ch);
    uint8 ctrlIdx = EthSM_GetEthIfControllerId(ch);
#if (STD_ON == ETHSM_DEM_SUPPORT)
    P2CONST(EthSM_DemEventParameterRefType, AUTOMATIC, ETHSM_CONST) demEventPtr;
#endif /* STD_ON == ETHSM_DEM_SUPPORT */

    if (ETHSM_SUBSTATE_NONE == subState)
    {
        /* @req SWS_EthSM_00155 */
        /* Transition from substate ONLINE to WAIT_OFFLINE */
        if (COMM_NO_COMMUNICATION == requestComMode)
        {
            /* @req SWS_EthSM_00157 */
            ret = TcpIp_RequestComMode(ctrlIdx, TCPIP_STATE_OFFLINE);
            if (E_OK == ret)
            {
                SchM_Enter_EthSM_Context();
                chPtr->curBsmState = ETHSM_STATE_WAIT_OFFLINE;
                SchM_Exit_EthSM_Context();

                /* @req SWS_EthSM_00158 */
                BswM_EthSM_CurrentState(network, ETHSM_STATE_WAIT_OFFLINE);
            }
        }
        else if (ETHTRCV_LINK_STATE_DOWN == trcvLinkState)
        {
            /* @req SWS_EthSM_00166 */
            /* Transition from substate ONLINE to ONHOLD */

            /* @req SWS_EthSM_00167 */
            ret = TcpIp_RequestComMode(ctrlIdx, TCPIP_STATE_ONHOLD);
            if (E_OK == ret)
            {
                SchM_Enter_EthSM_Context();
                chPtr->subState = ETHSM_SUBSTATE_WAIT_TCPIP_ONHOLD;
                SchM_Exit_EthSM_Context();
            }
        }
        else if (TCPIP_STATE_OFFLINE == tcpipState)
        {
            /* @req SWS_EthSM_00151 */
            /* Transition from substate ONLINE to WAIT_ONLINE */
            SchM_Enter_EthSM_Context();
            chPtr->curBsmState = ETHSM_STATE_WAIT_ONLINE;
            SchM_Exit_EthSM_Context();

            /* @req SWS_EthSM_00152 */
            BswM_EthSM_CurrentState(network, ETHSM_STATE_WAIT_ONLINE);
            /* @req SWS_EthSM_00154 */
            ComM_BusSM_ModeIndication(network, COMM_NO_COMMUNICATION);
        }
        else
        {
            /* Do nothing,Just for MISRA-C check */
        }
    }
    else if (ETHSM_SUBSTATE_WAIT_TCPIP_ONHOLD == subState)
    {
        if (TCPIP_STATE_ONHOLD == tcpipState)
        {
            SchM_Enter_EthSM_Context();
            chPtr->subState = ETHSM_SUBSTATE_NONE;
            chPtr->curBsmState = ETHSM_STATE_ONHOLD;
            SchM_Exit_EthSM_Context();
            /* @req SWS_EthSM_00168 */
            BswM_EthSM_CurrentState(network, ETHSM_STATE_ONHOLD);

/* @req SWS_EthSM_00188 */
#if (STD_ON == ETHSM_DEM_SUPPORT)
            demEventPtr = EthSM_GetDemEventParameterRef(ch);
            if (NULL_PTR != demEventPtr)
            {
                (void)Dem_SetEventStatus(demEventPtr->EthSMDemEventId, DEM_EVENT_STATUS_FAILED);
            }
#endif /* STD_ON == ETHSM_DEM_SUPPORT */
        }
    }
    else
    {
        /* Do nothing here */
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in ETHSM_STATE_ONHOLD
 * Sync/Async          Synchronous
 * Reentrancy          None Reentrant
 * Param-Name[in]      ch: EthSM Channel(EthSM Network)
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, ETHSM_CODE) EthSM_StateOnhold(NetworkHandleType ch)
{
    SchM_Enter_EthSM_Context();
    EthSM_RunTimeType* chPtr = &EthSM_RTData[ch];
    EthSM_SubState subState = chPtr->subState;
    ComM_ModeType requestComMode = chPtr->requestComMode;
    EthTrcv_LinkStateType trcvLinkState = chPtr->trcvLinkState;
    Eth_ModeType ctrlMode = chPtr->ctrlMode;
    TcpIp_StateType tcpipState = chPtr->tcpipState;
    SchM_Exit_EthSM_Context();
    Std_ReturnType ret;
    Std_ReturnType ret2;
    NetworkHandleType network = EthSM_GetComMNetworkId(ch);
    uint8 ctrlIdx = EthSM_GetEthIfControllerId(ch);
#if (STD_ON == ETHSM_DEM_SUPPORT)
    P2CONST(EthSM_DemEventParameterRefType, AUTOMATIC, ETHSM_CONST) demEventPtr;
#endif
    switch (subState)
    {

    case ETHSM_SUBSTATE_NONE:
        /* @req SWS_EthSM_00178 */
        /* Transition from substate ONHOLD to OFFLINE */
        if (COMM_NO_COMMUNICATION == requestComMode)
        {
            /* @req SWS_EthSM_00179 */
            ret = EthIf_SetControllerMode(ctrlIdx, ETH_MODE_DOWN);

            /* @req SWS_EthSM_00181 */
            ret2 = TcpIp_RequestComMode(ctrlIdx, TCPIP_STATE_OFFLINE);
            if ((E_OK == ret) && (E_OK == ret2))
            {
                SchM_Enter_EthSM_Context();
                chPtr->subState = ETHSM_SUBSTATE_WAIT_CTRL_TCPIP_CLOSE;
                SchM_Exit_EthSM_Context();
            }
        }
        else if (ETHTRCV_LINK_STATE_ACTIVE == trcvLinkState)
        {
            /* @req SWS_EthSM_00170 */
            /* Transition from substate ONHOLD to ONLINE */

            /* @req SWS_EthSM_00171 */
            ret = TcpIp_RequestComMode(ctrlIdx, TCPIP_STATE_ONLINE);
            if (E_OK == ret)
            {
                SchM_Enter_EthSM_Context();
                chPtr->subState = ETHSM_SUBSTATE_WAIT_TCPIP_ONLINE;
                SchM_Exit_EthSM_Context();
            }
        }
        else if (TCPIP_STATE_OFFLINE == tcpipState)
        {
            /* @req SWS_EthSM_00174 */
            /* Transition from substate ONHOLD to WAIT_TRCVLINK */
            SchM_Enter_EthSM_Context();
            chPtr->curBsmState = ETHSM_STATE_WAIT_TRCVLINK;
            SchM_Exit_EthSM_Context();

            /* @req SWS_EthSM_00175 */
            BswM_EthSM_CurrentState(network, ETHSM_STATE_WAIT_TRCVLINK);
            /* @req SWS_EthSM_00177 */
            ComM_BusSM_ModeIndication(network, COMM_NO_COMMUNICATION);
        }
        else
        {
            /* Do nothing,Just for MISRA-C check */
        }
        break;
    case ETHSM_SUBSTATE_WAIT_CTRL_TCPIP_CLOSE:
        if ((ETH_MODE_DOWN == ctrlMode) && (TCPIP_STATE_OFFLINE == tcpipState))
        {
            SchM_Enter_EthSM_Context();
            chPtr->subState = ETHSM_SUBSTATE_NONE;
            chPtr->curBsmState = ETHSM_STATE_OFFLINE;
            SchM_Exit_EthSM_Context();
            /* @req SWS_EthSM_00182 */
            BswM_EthSM_CurrentState(network, ETHSM_STATE_OFFLINE);
            /* @req SWS_EthSM_00184 */
            ComM_BusSM_ModeIndication(network, COMM_NO_COMMUNICATION);
        }
        break;
    case ETHSM_SUBSTATE_WAIT_TCPIP_ONLINE:
        if (TCPIP_STATE_ONLINE == tcpipState)
        {
            SchM_Enter_EthSM_Context();
            chPtr->subState = ETHSM_SUBSTATE_NONE;
            chPtr->curBsmState = ETHSM_STATE_ONLINE;
            SchM_Exit_EthSM_Context();
            /* @req SWS_EthSM_00172 */
            BswM_EthSM_CurrentState(network, ETHSM_STATE_ONLINE);

/* @req SWS_EthSM_00196 */
#if (STD_ON == ETHSM_DEM_SUPPORT)
            demEventPtr = EthSM_GetDemEventParameterRef(ch);
            if (NULL_PTR != demEventPtr)
            {
                (void)Dem_SetEventStatus(demEventPtr->EthSMDemEventId, DEM_EVENT_STATUS_PASSED);
            }
#endif /* STD_ON == ETHSM_DEM_SUPPORT */
        }
        break;
    default:
        /* Do nothing here */
        break;
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in ETHSM_STATE_WAIT_OFFLINE
 * Sync/Async          Synchronous
 * Reentrancy          None Reentrant
 * Param-Name[in]      ch: EthSM Channel(EthSM Network)
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, ETHSM_CODE) EthSM_StateWaitOffline(NetworkHandleType ch)
{
    SchM_Enter_EthSM_Context();
    EthSM_RunTimeType* chPtr = &EthSM_RTData[ch];
    EthSM_SubState subState = chPtr->subState;
    Eth_ModeType ctrlMode = chPtr->ctrlMode;
    TcpIp_StateType tcpipState = chPtr->tcpipState;
    SchM_Exit_EthSM_Context();
    Std_ReturnType ret;
    NetworkHandleType network = EthSM_GetComMNetworkId(ch);
    uint8 ctrlIdx = EthSM_GetEthIfControllerId(ch);

    if (ETHSM_SUBSTATE_NONE == subState)
    {
        /* @req SWS_EthSM_00160 */
        /* Transition from substate WAIT_OFFLINE to OFFLINE */
        if (TCPIP_STATE_OFFLINE == tcpipState)
        {
            /* @req SWS_EthSM_00161 */
            ret = EthIf_SetControllerMode(ctrlIdx, ETH_MODE_DOWN);
            if (E_OK == ret)
            {
                SchM_Enter_EthSM_Context();
                chPtr->subState = ETHSM_SUBSTATE_WAIT_CTRL_DOWN;
                SchM_Exit_EthSM_Context();
            }
        }
    }
    else if (ETHSM_SUBSTATE_WAIT_CTRL_DOWN == subState)
    {
        if (ETH_MODE_DOWN == ctrlMode)
        {
            SchM_Enter_EthSM_Context();
            chPtr->subState = ETHSM_SUBSTATE_NONE;
            chPtr->curBsmState = ETHSM_STATE_OFFLINE;
            SchM_Exit_EthSM_Context();
            /* @req SWS_EthSM_00163 */
            BswM_EthSM_CurrentState(network, ETHSM_STATE_OFFLINE);
            /* @req SWS_EthSM_00165 */
            ComM_BusSM_ModeIndication(network, COMM_NO_COMMUNICATION);
        }
    }
    else
    {
        /* Do nothing here */
    }
}

/******************************************************************************/
/*
 * Brief               Get EthSM channel index by NetworkHandle
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      NetworkHandle: NetworkHandle from ComM
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              If find return EthSM channel index
 *                     If not find return ETHSM_CHANNEL_NUM
 */
/******************************************************************************/
ETHSM_LOCAL FUNC(NetworkHandleType, ETHSM_CODE) EthSM_GetChannelByNetworkHandle(NetworkHandleType NetworkHandle)
{
    NetworkHandleType ch;

    /* loop for every channel, search corresponding network */
    for (ch = 0u; ch < ETHSM_CHANNEL_NUM; ch++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
    {
        if (EthSM_GetComMNetworkId(ch) == NetworkHandle)
        {
            break;
        }
    }

    return ch;
}

/******************************************************************************/
/*
 * Brief               Get EthSM channel index by EthIf controller index
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      network: NetworkHandle from ComM
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              If find return EthSM channel index
 *                     If not find return ETHSM_CHANNEL_NUM
 */
/******************************************************************************/
static FUNC(NetworkHandleType, ETHSM_CODE) EthSM_GetChannelByCtrlIdx(uint8 CtrlIdx)
{
    NetworkHandleType ch;

    /* loop for every channel, search corresponding network */
    for (ch = 0u; ch < ETHSM_CHANNEL_NUM; ch++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
    {
        if (EthSM_GetEthIfControllerId(ch) == CtrlIdx)
        {
            break;
        }
    }

    return ch;
}

#define ETHSM_STOP_SEC_CODE
#include "EthSM_MemMap.h"
