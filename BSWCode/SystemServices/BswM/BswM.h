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
**  FILENAME    : BswM.h                                                      **
**                                                                            **
**  Created on  : 2020-03-24                                                  **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

#ifndef BSWM_H
#define BSWM_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "BswM_Types.h"
#include "Os.h"

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define BSWM_MODULE_ID 42
#if (BSWM_MULTI_PARTITION_ENABLED == STD_ON)
#define BSWM_INSTANCE_ID ((uint8)GetApplicationID())
#else
#define BSWM_INSTANCE_ID 0
#endif /*BSWM_MULTI_PARTITION_ENABLED == STD_ON*/
#define BSWM_VENDOR_ID        62
#define BSWM_SW_MAJOR_VERSION 2
#define BSWM_SW_MINOR_VERSION 1
#define BSWM_SW_PATCH_VERSION 11
#define BSWM_AR_MAJOR_VERSION 4
#define BSWM_AR_MINOR_VERSION 5
#define BSWM_AR_PATCH_VERSION 0

/*******************API ID**************************/
#define BSWM_API_ID_INIT                                  0
#define BSWM_API_ID_GET_VERSION_INFO                      0x01
#define BSWM_API_ID_MAINFUNCTION                          0x03
#define BSWM_API_ID_CANSM_CURRENT_STATE                   0x05
#define BSWM_API_ID_DCM_COM_MODE_CURRENT_STATE            0x06
#define BSWM_API_ID_BSWM_MODE_REQUEST                     0x1c
#define BSWM_API_ID_BSWM_MODE_SWITCH                      0x1d
#define BSWM_API_ID_CANSM_CURRENT_ICOM_CONFIG             0x1a
#define BSWM_API_ID_COMM_CURRENT_MODE                     0x0e
#define BSWM_API_ID_COMM_CURRENT_PNC_MODE                 0x15
#define BSWM_API_ID_COMM_INITIATE_RESET                   0x22
#define BSWM_API_ID_DCM_APPLICATION_UPDATED               0x14
#define BSWM_API_ID_ECUM_CURRENT_STATE                    0x0f
#define BSWM_API_ID_ECUM_CURRENT_WAKEUP                   0x10
#define BSWM_API_ID_ECUM_REQUESTED_STATE                  0x23
#define BSWM_API_ID_ETHIF_PORT_GRU_LINK_STATE             0x26
#define BSWM_API_ID_FRSM_CURRENT_STATE                    0x0c
#define BSWM_API_ID_ETHSM_CURRENT_STATE                   0x0d
#define BSWM_API_ID_J1939DCM_BROADCAST_STATUS             0x1b
#define BSWM_API_ID_J1939NM_STATE_CHANGE_NOTIFY           0x18
#define BSWM_API_ID_LINSM_CURRENT_STATE                   0x09
#define BSWM_API_ID_LINSM_CURRENT_SCHEDULE                0x0a
#define BSWM_API_ID_LINTP_REQUEST_MODE                    0x0b
#define BSWM_API_ID_NMIF_CAR_WAKEUP_INDICATION            0x24
#define BSWM_API_ID_NVM_CURRENT_BLOCK_MODE                0x16
#define BSWM_API_ID_NVM_CURRENT_JOB_MODE                  0x17
#define BSWM_API_ID_SD_CLIENT_SERVICE_CURRENT_STATE       0x1f
#define BSWM_API_ID_SD_EVENT_HANDLER_CURRENT_STATE        0x20
#define BSWM_API_ID_SD_CONSUMED_EVENT_GROUP_CURRENT_STATE 0x21
#define BSWM_API_ID_MODE_SWITCH_ERROR_NOTIFICATION        0xff
#define BSWM_API_ID_MODE_REQUEST_PORT                     0xfe
#define BSWM_API_ID_MODE_NOTIFICATION_PORT                0xfd
#define BSWM_API_ID_BSW_MODE_NOTIFICATION                 0xfc
#define BSWM_API_ID_WDGM_REQUEST_PARTITION_RESET          0x11
#define BSWM_API_ID_BSWM_PARTITION_RESTARTED              0x1e
#define BSWM_API_ID_REQUEST_MODE                          0x02

#define BSWM_API_ID_DO_ACTION_LIST                        0X07

/*********Error code for development****************/
/*A service was called prior to initialization */
#define BSWM_E_UNINIT 0x01
/*A null pointer was passed as an argument*/
#define BSWM_E_NULL_POINTER 0x02
/*A parameter was invalid (unspecific)*/
#define BSWM_E_PARAM_INVALID 0x03
/*A requesting user was out of range*/
#define BSWM_E_REQ_USER_OUT_OF_RANGE 0x04
/*A requested mode was out of range*/
#define BSWM_E_REQ_MODE_OUT_OF_RANGE 0x05
/*The provided configuration is inconsistent*/
#define BSWM_E_PARAM_CONFIG 0x06
/*A parameter pointer was invalid*/
#define BSWM_E_PARAM_POINTER 0x07
/*Invalid configuration set selection*/
#define BSWM_E_INIT_FAILED 0x08
/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
typedef uint16 BswM_UserType;

/****Link time configuration****/
typedef struct
{
    P2CONST(BswM_RuleIndexType, TYPEDEF, BSWM_CONST) belongToRlue;
    BswM_RuleIndexType belongToRlueNum;
    BswM_RqstPortProcessType process;
} BswM_RqstPortLcCfgType;

typedef struct
{
#if (BSWM_EVENT_RQSTPORT_ENABLE == STD_ON) /*EventRequestPort configuration exists*/
#if (BSWM_COMM_ENABLED == STD_ON)
    P2CONST(BswM_RqstPortLcCfgType, TYPEDEF, BSWM_CONST) comMInitReset;
#endif /*BSWM_CANSM_ENABLED == STD_ON*/

#if (BSWM_DCM_ENABLED == STD_ON)
    P2CONST(BswM_RqstPortLcCfgType, TYPEDEF, BSWM_CONST) dcmAppUpdate;
#endif /*BSWM_DCM_ENABLED == STD_ON*/

#if (BSWM_RTE_ENABLED == STD_ON)
    P2CONST(BswM_RqstPortLcCfgType, TYPEDEF, BSWM_CONST) modeSwiErr;
    P2CONST(uint16, TYPEDEF, BSWM_CONST) modeSwiErrPort;
#endif /*BSWM_RTE_ENABLED == STD_ON*/

#if (BSWM_NM_ENABLED == STD_ON)
    P2CONST(BswM_RqstPortLcCfgType, TYPEDEF, BSWM_CONST) nmWakeUpInd;
    P2CONST(NetworkHandleType, TYPEDEF, BSWM_CONST) nmWkChRef;
#endif /*BSWM_NM_ENABLED == STD_ON*/

#if (BSWM_MULTI_PARTITION_ENABLED == STD_ON)
    P2CONST(BswM_RqstPortLcCfgType, TYPEDEF, BSWM_CONST) bswMParRestart;
#endif /*BSWM_MAX_NUM_PARTITION > 1*/

#if (BSWM_WDGM_ENABLED == STD_ON)
    P2CONST(BswM_RqstPortLcCfgType, TYPEDEF, BSWM_CONST) wdgMRqstParReset;
    P2CONST(ApplicationType, TYPEDEF, BSWM_CONST) wdgAppRef;
#endif /*BSWM_WDGM_ENABLED == STD_ON*/

#else  /*EventRequestPort configuration don't exist*/
    uint8 dummy;
#endif /*BSWM_EVENT_RQSTPORT_ENABLE == STD_ON*/
} BswM_EventRqstPortLCfgType;
typedef struct
{
#if (BSWM_SCHM_ENABLED == STD_ON)
    /*BswMBswModeNotification*/
    P2CONST(BswM_RqstPortLcCfgType, TYPEDEF, BSWM_CONST) bswModeNoti;
    P2CONST(BswM_ModeGroupType, TYPEDEF, BSWM_CONST) bswModeNotiRef;
#endif /*BSWM_SCHM_ENABLED == STD_ON*/

#if (BSWM_CANSM_ENABLED == STD_ON)
#if (BSWM_CANSM_ICOM_ENABLED == STD_ON)
    P2CONST(BswM_RqstPortLcCfgType, TYPEDEF, BSWM_CONST) cansmIcom;
    P2CONST(NetworkHandleType, TYPEDEF, BSWM_CONST) cansmIcomChRef;
    P2CONST(IcomSwitch_ErrorType, TYPEDEF, BSWM_CONST) cansmIcomSwichErrRef;
#endif /*BSWM_CANSM_ICOM_ENABLED == STD_ON*/
    P2CONST(BswM_RqstPortLcCfgType, TYPEDEF, BSWM_CONST) cansmInd;
    P2CONST(NetworkHandleType, TYPEDEF, BSWM_CONST) cansmIndChRef;
#endif /*BSWM_CANSM_ENABLED == STD_ON*/

#if (BSWM_COMM_ENABLED == STD_ON)
    P2CONST(BswM_RqstPortLcCfgType, TYPEDEF, BSWM_CONST) comMInd;
    P2CONST(NetworkHandleType, TYPEDEF, BSWM_CONST) comMIndChRef;
#if (BSWM_COMM_PNC_ENABLED == STD_ON)
    P2CONST(BswM_RqstPortLcCfgType, TYPEDEF, BSWM_CONST) comMPncRqst;
    P2CONST(PNCHandleType, TYPEDEF, BSWM_CONST) comMPncRef;
#endif /*BSWM_COMM_PNC_ENABLED == STD_ON*/
#endif /*BSWM_COMM_ENABLED == STD_ON*/

#if (BSWM_DCM_ENABLED == STD_ON)
    P2CONST(BswM_RqstPortLcCfgType, TYPEDEF, BSWM_CONST) dcmComModeRqst;
    P2CONST(NetworkHandleType, TYPEDEF, BSWM_CONST) dcmChRef;
#endif /*BSWM_DCM_ENABLED == STD_ON*/

#if (BSWM_ECUM_ENABLED == STD_ON)
    P2CONST(BswM_RqstPortLcCfgType, TYPEDEF, BSWM_CONST) ecuMState;
    P2CONST(BswM_RqstPortLcCfgType, TYPEDEF, BSWM_CONST) ecuMWkSrc;
    P2CONST(EcuM_WakeupSourceType, TYPEDEF, BSWM_CONST) ecuMWkSrcRef;
    /*Add EcuM Request Protocol*/
    P2CONST(BswM_RqstPortLcCfgType, TYPEDEF, BSWM_CONST) ecuMRqstIndSrc;
    P2CONST(EcuM_StateType, TYPEDEF, BSWM_CONST) ecuMRqstIndSrcRef;
#endif /*BSWM_ECUM_ENABLED == STD_ON*/

#if (BSWM_ETHIF_ENABLED == STD_ON)
    P2CONST(BswM_RqstPortLcCfgType, TYPEDEF, BSWM_CONST) ethIfPortGroLink;
    P2CONST(EthIf_SwitchPortGroupIdxType, TYPEDEF, BSWM_CONST) ethIfPortGroRef;
#endif /*BSWM_ETHIF_ENABLED == STD_ON*/

#if (BSWM_ETHSM_ENABLED == STD_ON)
    P2CONST(BswM_RqstPortLcCfgType, TYPEDEF, BSWM_CONST) ethsmInd;
    P2CONST(NetworkHandleType, TYPEDEF, BSWM_CONST) ethsmChRef;
#endif /*BSWM_ETHSM_ENABLED == STD_ON*/

#if (BSWM_FRSM_ENABLED == STD_ON)
    P2CONST(BswM_RqstPortLcCfgType, TYPEDEF, BSWM_CONST) frsmInd;
    P2CONST(NetworkHandleType, TYPEDEF, BSWM_CONST) frsmChRef;
#endif /*BSWM_FRSM_ENABLED == STD_ON*/

#if (BSWM_GENERIC_REQUEST_ENABLED == STD_ON)
    P2CONST(BswM_RqstPortLcCfgType, TYPEDEF, BSWM_CONST) genericRqst;
    P2CONST(BswM_UserType, TYPEDEF, BSWM_CONST) genRqstUserRef;
#endif /*BSWM_GENERIC_REQUEST_ENABLED == STD_ON*/

#if (BSWM_J1939DCM_ENABLED == STD_ON)
    P2CONST(BswM_RqstPortLcCfgType, TYPEDEF, BSWM_CONST) j1939DcmBC;
    P2CONST(NetworkHandleType, TYPEDEF, BSWM_CONST) j1939DcmBCChRef;
#endif /*BSWM_J1939DCM_ENABLED == STD_ON*/

#if (BSWM_J1939NM_ENABLED == STD_ON)
    P2CONST(BswM_RqstPortLcCfgType, TYPEDEF, BSWM_CONST) j1939NmInd;
    P2CONST(NetworkHandleType, TYPEDEF, BSWM_CONST) j1939NmChRef;
    P2CONST(uint8, TYPEDEF, BSWM_CONST) j1939NmNodeRef;
#endif /*BSWM_J1939NM_ENABLED == STD_ON*/

#if (BSWM_LINSM_ENABLED == STD_ON)
    /*BswMLinSMIndication*/
    P2CONST(BswM_RqstPortLcCfgType, TYPEDEF, BSWM_CONST) linsmInd;
    P2CONST(NetworkHandleType, TYPEDEF, BSWM_CONST) linsmIndChRef; /*BswMLinSMChannelRef*/
    /*BswMLinScheduleIndication*/
    P2CONST(BswM_RqstPortLcCfgType, TYPEDEF, BSWM_CONST) linSchInd;
    P2CONST(NetworkHandleType, TYPEDEF, BSWM_CONST) linSchNKChRef; /*BswMLinSMChannelRef*/
    P2CONST(LinIf_SchHandleType, TYPEDEF, BSWM_CONST) linSchChRef; /*BswMLinScheduleRef */
#endif                                                             /*BSWM_LINSM_ENABLED == STD_ON*/

#if (BSWM_LINTP_ENABLED == STD_ON)
    P2CONST(BswM_RqstPortLcCfgType, TYPEDEF, BSWM_CONST) lintpModeRqst;
    P2CONST(NetworkHandleType, TYPEDEF, BSWM_CONST) lintpChRef;
#endif /*BSWM_LINTP_ENABLED == STD_ON*/

#if (BSWM_NVM_ENABLED == STD_ON)
    /*BswMNvMJobModeIndication*/
    P2CONST(BswM_RqstPortLcCfgType, TYPEDEF, BSWM_CONST) nvmJobInd;
    P2CONST(NvM_MultiBlockRequestType, TYPEDEF, BSWM_CONST) nvmService;
    /*BswMNvMRequest*/
    P2CONST(BswM_RqstPortLcCfgType, TYPEDEF, BSWM_CONST) nvmRqst;
    P2CONST(NvM_BlockIdType, TYPEDEF, BSWM_CONST) nvmBlockRef;
#endif /*BSWM_NVM_ENABLED == STD_ON*/

#if (BSWM_SD_ENABLED == STD_ON)
    /*BswMSdClientServiceCurrentState*/
    P2CONST(BswM_RqstPortLcCfgType, TYPEDEF, BSWM_CONST) sdClientSrv;
    P2CONST(uint16, TYPEDEF, BSWM_CONST) sdClientSrvMethodsRef;
    /*BswMSdConsumedEventGroupCurrentState*/
    P2CONST(BswM_RqstPortLcCfgType, TYPEDEF, BSWM_CONST) sdConEvGroup;
    P2CONST(uint16, TYPEDEF, BSWM_CONST) sdConEvGroupIdRef;
    /*BswMSdEventHandlerCurrentState*/
    P2CONST(BswM_RqstPortLcCfgType, TYPEDEF, BSWM_CONST) sdEvHandler;
    P2CONST(uint16, TYPEDEF, BSWM_CONST) sdEvHandleIdRef;
#endif /*BSWM_SD_ENABLED == STD_ON*/

#if (BSWM_RTE_ENABLED == STD_ON)
    /*BswMSwcModeNotification*/
    P2CONST(BswM_RqstPortLcCfgType, TYPEDEF, BSWM_CONST) swcModeNoti;
    P2CONST(BswM_ModeGroupType, TYPEDEF, BSWM_CONST) swcModeNotisRef;
    /*BswMSwcModeRequest*/
    P2CONST(BswM_RqstPortLcCfgType, TYPEDEF, BSWM_CONST) swcModeRqst;
    P2CONST(BswM_ModeGroupType, TYPEDEF, BSWM_CONST) swcModeRqstsRef;
#endif /*BSWM_RTE_ENABLED == STD_ON*/

#if (BSWM_MODE_RQSTPORT_ENABLE == STD_OFF) /*ModeRequestPort configuration don't exist*/
    uint8 dummy;
#endif /*BSWM_MODE_RQSTPORT_ENABLE == STD_OFF*/
} BswM_ModeRqstPortLCfgType;

/*type definition for link time configure of rules*/
typedef struct
{
    /*BswMRuleExpressionRef*/
    P2FUNC(BswM_RuleStateType, BSWM_APPL_CODE, modeArbitraFunc)(void); /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    /*BswMRuleInitState*/
    BswM_RuleStateType ruleInitState;
    /*BswMNestedExecutionOnly*/
    boolean isNestExe;
    /*BswMRuleFalseActionList*/
    P2CONST(BswM_ActionListIndexType, TYPEDEF, BSWM_CONST) falseActList;
    /*BswMRuleTrueActionList*/
    P2CONST(BswM_ActionListIndexType, TYPEDEF, BSWM_CONST) trueActList;
} BswM_RuleLcCfgType;

typedef struct
{
    BswM_ActionEnumType avbActType;
    uint8 refActIdx;
} BswM_ActionType;

typedef struct
{
    P2CONST(BswM_ActionListIndexType, TYPEDEF, BSWM_CONST) actListRefIdx; /*Ref BswMActionList*/
    P2CONST(BswM_RuleIndexType, TYPEDEF, BSWM_CONST) ruleRefIdx;          /*Ref BswMRule*/
    P2CONST(BswM_ActionType, TYPEDEF, BSWM_CONST) avActType;              /*Ref BswMAction*/
} BswM_ActionListItemRefType;

/*BswM action item type*/
typedef struct
{
    uint8 actionItemIdx;                                                /*BswMActionListItemIndex*/
    boolean abortOnFail;                                                /*BswMAbortOnFail*/
    P2CONST(uint8, TYPEDEF, BSWM_CONST) runtimeErrId;                   /*BswMReportFailRuntimeErrorId*/
    BswM_ActionListItemEnumType actionItemType;                         /*action type, action or rule or action list*/
    P2CONST(BswM_ActionListItemRefType, TYPEDEF, BSWM_CONST) actionRef; /*action type if item is action*/
} BswM_ActionListItemType;

/*BSWM action list type definition of rules*/
typedef struct
{
    BswM_ActionListExecutionType executeType;
    /*BswMActionListPriority*/
    P2CONST(uint8, TYPEDEF, BSWM_CONST) actListPrior;
    /*BswMActionListItem*/
    uint8 numOfActionItem;
    P2CONST(BswM_ActionListItemType, TYPEDEF, BSWM_CONST) actionItems;
} BswM_ActionListLCfgType;

/**********************action Items*****************************************/
/*BswMComMAllowCom action: ComM_CommunicationAllowed*/
#if (BSWM_ACTION_COMMALLOWCOM_ENABLED == STD_ON)
typedef struct
{
    boolean comAllowed;
    NetworkHandleType channel; /*channel to allow or block*/
} BswM_ActionComMAllowComLCfgType;
#endif /*BSWM_ACTION_COMMALLOWCOM_ENABLED == STD_ON*/

#if (BSWM_ACTION_COMMMODELIMITATION_ENABLED == STD_ON)
/*BswMComMModeLimitation action:ComM_LimitChannelToNoComMode*/
typedef struct
{
    boolean limitToNo;
    NetworkHandleType channel; /*channel to limit*/
} BswM_ActionComMModeLimitLCfgType;
#endif /*BSWM_ACTION_COMMMODELIMITATION_ENABLED == STD_ON*/

#if (BSWM_ACTION_COMMMODESWITCH_ENABLED == STD_ON)
/*BswMComMModeSwitch action: ComM_RequestComMode*/
typedef struct
{
    ComM_ModeType comMode;    /*COMM_FULL_COMMUNICATION COMM_NO_COMMUNICATION*/
    ComM_UserHandleType user; /*user who requests a mode*/
} BswM_ActionComMModeSwitchLCfgType;
#endif /*BSWM_ACTION_COMMMODESWITCH_ENABLED == STD_ON*/

#if (BSWM_MAX_NUM_CORE > 1)
#if (BSWM_ACTION_COREHALTMODE_ENABLED == STD_ON)
/*BswMCoreHaltMode action:Different possibilities are offered depending on the
 * OS implementation and the CPU HW.*/
typedef struct
{
    IdleModeType IdleMode; /*depending on the OS implementation and the CPU HW*/
    CoreIdType coreId;     /*the core to halt*/
} BswM_ActionCoreHaltModeLCfgType;
#endif /*BSWM_ACTION_COREHALTMODE_ENABLED == STD_ON*/
#endif /*BSWM_MAX_NUM_CORE > 1*/

#if (BSWM_ACTION_DEADLINEMONITORINGCONTROL_ENABLED == STD_ON)
/*BswMDeadlineMonitoringControl action:
 * v4.2.2:COM_ReceptionDMControl
 * R1911:Com_EnableReceptionDM/Com_DisableReceptionDM*/
typedef struct
{
    P2CONST(BswM_ComIpduGruType, TYPEDEF, BSWM_CONST) enableRecDmGruId;
    BswM_ComIpduGruType numOfEnableRecDmGru;
    P2CONST(BswM_ComIpduGruType, TYPEDEF, BSWM_CONST) disableRecDmGruId;
    BswM_ComIpduGruType numOfDisableRecDmGru;
} BswM_ActionDMControlLCfgType;
#endif /*BSWM_ACTION_DEADLINEMONITORINGCONTROL_ENABLED == STD_ON*/

#if (BSWM_ACTION_PDUGROUPSWITCH_ENABLED == STD_ON)
/*BswMPduGroupSwitch action:
 * v4.2.2:Com_IpduGroupControl
 * R1911:Com_IpduGroupStart/Com_IpduGroupStop*/
typedef struct
{
    boolean initialize; /*flag to request initialization of the I-PDUs*/
    P2CONST(BswM_ComIpduGruType, TYPEDEF, BSWM_CONST) enableIpduGruId;
    BswM_ComIpduGruType numOfEnableIpduGru;
    P2CONST(BswM_ComIpduGruType, TYPEDEF, BSWM_CONST) disableIpduGruId;
    BswM_ComIpduGruType numOfDisableIpduGru;
} BswM_ActionPduGrpSwitchLCfgType;
#endif /*BSWM_ACTION_PDUGROUPSWITCH_ENABLED == STD_ON*/

#if (BSWM_ACTION_SWITCHIPDUMODE_ENABLED == STD_ON)
/*BswMSwitchIPduMode action: Com_SwitchIpduTxMode*/
typedef struct
{
    boolean mode;    /*TX mode*/
    PduIdType pduId; /*IPDU id to change mode*/
} BswM_ActionSwitchIpduModeLCfgType;
#endif /*BSWM_ACTION_SWITCHIPDUMODE_ENABLED == STD_ON*/

#if (BSWM_ACTION_TRIGGERIPDUSEND_ENABLED == STD_ON)
/*BswMTriggerIPduSend action: Com_TriggerIPDUSend*/
typedef struct
{
    uint8 numOfIpdu; /*number of IPDU trigger to send*/
    P2CONST(PduIdType, TYPEDEF, BSWM_CONFIG_DATA) pduIds;
} BswM_ActionTriggerIPduSendLCfgType;
#endif /*BSWM_ACTION_TRIGGERIPDUSEND_ENABLED == STD_ON*/

#if (BSWM_ACTION_ECUMDRIVERINITLIST_ENABLED == STD_ON)
/*BswMEcuMDriverInitListBswM*/
typedef struct
{
    uint8 ecumDrvInitList; /*This is a reference to a EcuM UserId.*/
} BswM_ActionEcuMDrvInitLCfgType;
#endif /*BSWM_ACTION_ECUMDRIVERINITLIST_ENABLED == STD_ON*/

#if (BSWM_ACTION_ECUMGODOWNHALTPOLL_ENABLED == STD_ON)
/*BswMEcuMGoDownHaltPoll action:EcuM_GoDownHaltPoll*/
typedef struct
{
    uint16 goDownUser; /*This is a reference to a EcuM UserId.*/
} BswM_ActionEcuMGoDownLCfgType;
#endif /*BSWM_ACTION_ECUMGODOWNHALTPOLL_ENABLED == STD_ON*/

#if (BSWM_ACTION_ECUMSELECTSHUTDOWNTARGET_ENABLED == STD_ON)
/*BswMEcuMSelectShutdownTarget action: EcuM_SelectShutdownTarget*/
typedef struct
{
    EcuM_ShutdownTargetType target; /*shutdown target that the BswM selects at the EcuM*/
    EcuM_ShutdownModeType mode;     /*mode that the BswM selects at the EcuM*/
} BswM_ActionEcuMSelectShutTgtLCfgType;
#endif /*BSWM_ACTION_ECUMSELECTSHUTDOWNTARGET_ENABLED == STD_ON*/

#if (BSWM_ACTION_ECUMSTATESWITCH_ENABLED == STD_ON)
/*BswMEcuMStateSwitch action: EcuM_SetState*/
typedef struct
{
#if (BSWM_ECUM_VERSION == BSWM_V_4_2_2)
    EcuM_StateType ecuMState; /*state to switch*/
#else
    EcuM_ShutdownTargetType ecuMState; /*state to switch*/
#endif /*BSWM_ECUM_VERSION == BSWM_V_4_2_2*/
} BswM_ActionEcuMStateSwitchLCfgType;
#endif /*#(BSWM_ACTION_ECUMSTATESWITCH_ENABLED == STD_ON)*/

#if (BSWM_ACTION_ETHIFSWITCHPORTGROUPREQUESTMODE_ENABLED == STD_ON)
/*BswMEthIfSwitchPortGroupRequestMode action: EthIf_SwitchPortGroupRequestMode */
typedef struct
{
    EthIf_SwitchPortGroupIdxType ethSwiGruIdx;
    Eth_ModeType ethTrcvMode;
} BswM_ActionEthIfSwiPortGruLCfgType;
#endif /*BSWM_ACTION_ETHIFSWITCHPORTGROUPREQUESTMODE_ENABLED == STD_ON*/

#if (BSWM_ACTION_FRSMALLSLOTS_ENABLED == STD_ON)
/*BswMFrSMAllSlots action: FrSM_AllSlots*/
typedef struct
{
    NetworkHandleType channel; /*flex ray cluster to all slots*/
} BswM_ActionFrSMAllSlotsLCfgType;
#endif /*BSWM_ACTION_FRSMALLSLOTS_ENABLED == STD_ON*/

#if (BSWM_ACTION_J1939DCMSTATESWITCH_ENABLED == STD_ON)
/*BswMJ1939DcmStateSwitch action: J1939Dcm_SetState*/
typedef struct
{
    J1939Dcm_StateType state;  /*state to switch*/
    NetworkHandleType channel; /*channel to switch*/
    uint8 node;                /*node to switch*/
} BswM_ActionJ1939DcmStateSwitchLCfgType;
#endif /*BSWM_ACTION_J1939DCMSTATESWITCH_ENABLED == STD_ON*/

#if (BSWM_ACTION_J1939RMSTATESWITCH_ENABLED == STD_ON)
/*BswMJ1939RmStateSwitch action: J1939Rm_SetState*/
typedef struct
{
    J1939Rm_StateType state;   /*state to switch*/
    NetworkHandleType channel; /*channel to switch*/
    uint8 node;                /*node to switch*/
} BswM_ActionJ1939RmStateSwitchLCfgType;
#endif /*BSWM_ACTION_J1939RMSTATESWITCH_ENABLED == STD_ON*/

#if (BSWM_ACTION_LINSCHEDULESWITCH_ENABLED == STD_ON)
/*BswMLinScheduleSwitch action: LinSM_ScheduleRequest*/
typedef struct
{
    NetworkHandleType network;
    LinIf_SchHandleType schedule;
} BswM_ActionLinScheduleSwitchLCfgType;
#endif /*#(BSWM_ACTION_LINSCHEDULESWITCH_ENABLED == STD_ON)*/

#if (BSWM_NM_ENABLED == STD_ON)
/*BswMNMControl action: Nm_EnableCommunication, Nm_DisableCommunication*/
typedef enum
{
    BSWM_NM_DISABLE,
    BSWM_NM_ENABLE
} BswM_NmControlType;

#if (BSWM_ACTION_NMCONTROL_ENABLED == STD_ON)
typedef struct
{
    BswM_NmControlType action;   /*Disable or enable*/
    NetworkHandleType nmChannel; /*channel defined by the ComMChannel*/
} BswM_ActionNMControlLCfgType;
#endif /*#(BSWM_ACTION_NMCONTROL_ENABLED == STD_ON)*/
#endif /*BSWM_NM_ENABLED == STD_ON*/

#if (BSWM_PDUR_ENABLED == STD_ON)
/*BswMPduRouterControl action: PduR_EnableRouting, PduR_DisableRouting*/
/*PDUR router action type*/
typedef enum
{
    BSWM_PDUR_DISABLE,
    BSWM_PDUR_ENABLE
} BswM_PduRControlType;

#if (BSWM_ACTION_PDUROUTERCONTROL_ENABLED == STD_ON)
typedef struct
{
    BswM_PduRControlType action;                      /*Disable or enable*/
    P2CONST(boolean, TYPEDEF, BSWM_CONST) initialize; /*BswMPduRouterDisableInitBuffer,only for disable*/
    uint8 numOfPathGrp;
    P2CONST(PduR_RoutingPathGroupIdType, TYPEDEF, BSWM_CONST)
    ids; /*the routing path groups*/
} BswM_ActionPduRCtrlLCfgType;
#endif /*BSWM_ACTION_PDUROUTERCONTROL_ENABLED == STD_ON*/
#endif /*BSWM_PDUR_ENABLED == STD_ON*/

#if (BSWM_ACTION_RTEMODEREQUEST_ENABLED == STD_ON)
/*BswMRteModeRequest action: RTE_Write*/
typedef struct
{
    BswM_ModeType mode; /*mode to request*/
    P2FUNC(Std_ReturnType, BSWM_APPL_CODE, rteRequestFctPtr)(BswM_ModeType);
} BswM_ActionRteModeReqLCfgType;
#endif /*BSWM_ACTION_RTEMODEREQUEST_ENABLED == STD_ON*/

#if (BSWM_ACTION_RTESWITCH_ENABLED == STD_ON)
/*BswMRteSwitch action: RTE_Switch*/
typedef struct
{
    BswM_ModeType mode; /*mode to switch*/
    P2FUNC(Std_ReturnType, BSWM_APPL_CODE, rteSwitchFctPtr)(BswM_ModeType);
} BswM_ActionRteSwitchLCfgType;
#endif /*BSWM_ACTION_RTESWITCH_ENABLED == STD_ON*/

#if (BSWM_ACTION_SCHMSWITCH_ENABLED == STD_ON)
/*BswMSchMSwitch action: SchM_Switch*/
typedef struct
{
    BswM_ModeType mode; /*mode to switch*/
    P2FUNC(Std_ReturnType, BSWM_APPL_CODE, schMSwitchFctPtr)(BswM_ModeType);
} BswM_ActionSchMSwitchLCfgType;
#endif /*BSWM_ACTION_SCHMSWITCH_ENABLED == STD_ON*/

#if (BSWM_ACTION_SDCLIENTSERVICEMODEREQUEST_ENABLED == STD_ON)
/*BswMSdClientServiceModeRequest action: Sd_ClientServiceSetState*/
typedef struct
{
    Sd_ClientServiceSetStateType state; /*state to set*/
    uint16 clientServiceHandleId;       /*client service to set*/
} BswM_ActionSdCliServiceModeReqLCfgType;
#endif /*BSWM_ACTION_SDCLIENTSERVICEMODEREQUEST_ENABLED == STD_ON*/

#if (BSWM_ACTION_SDCONSUMEDEVENTGROUPMODEREQUEST_ENABLED == STD_ON)
/*BswMSdConsumedEventGroupModeRequest action: Sd_ConsumedEventGroupSetState*/
typedef struct
{
    Sd_ConsumedEventGroupSetStateType state; /*state to set*/
    uint16 sdConsumedEventGroupHandleId;     /*event group to set*/
} BswM_ActionSdConsumedEvGrpModeReqLCfgType;
#endif /*BSWM_ACTION_SDCONSUMEDEVENTGROUPMODEREQUEST_ENABLED == STD_ON*/

#if (BSWM_ACTION_SDSERVERSERVICEMODEREQUEST_ENABLED == STD_ON)
/*BswMSdServerServiceModeRequest action: Sd_ServerServiceSetState*/
typedef struct
{
    Sd_ServerServiceSetStateType state; /*state to set*/
    uint16 sdServerServiceHandleId;     /*server service to set*/
} BswM_ActionSdServerServiceModeReqLCfgType;
#endif /*BSWM_ACTION_SDSERVERSERVICEMODEREQUEST_ENABLED == STD_ON*/

#if (BSWM_ACTION_TIMERCONTROL_ENABLED == STD_ON)
/*BswMTimerControl action:*/
typedef struct
{
    BswM_TimerEnumType timerAct;
    uint32 timerVal;
    BswM_TimerIdxType timerRef;
} BswM_ActionTimerCtrlLCfgType;
#endif /*BSWM_ACTION_TIMERCONTROL_ENABLED == STD_ON*/

#if (BSWM_ACTION_USERCALLOUT_ENABLED == STD_ON)
/*BswMUserCallout action: user custom call out function*/
typedef struct
{
    P2FUNC(void, BSWM_APPL_CODE, userCalloutFctPtr)(void); /* PRQA S 3432 */ /* MISRA Rule 20.7 */
} BswM_ActionUserCalloutLCfgType;
#endif /*BSWM_ACTION_USERCALLOUT_ENABLED == STD_ON*/

/*BSWM action items type definition*/
typedef struct
{
#if (BSWM_ACTION_CLEAREVENTREQUEST_ENABLED == STD_ON)
    P2CONST(BswM_EventRqstPortIdxType, TYPEDEF, BSWM_CONST) clrEvRqstRef;
#endif /*BSWM_ACTION_CLEAREVENTREQUEST_ENABLED == STD_ON*/

#if (BSWM_ACTION_COMMALLOWCOM_ENABLED == STD_ON)
    P2CONST(BswM_ActionComMAllowComLCfgType, TYPEDEF, BSWM_CONST)
    comMAllowComActions; /*BswMComMAllowCom*/
#endif                   /*BSWM_ACTION_COMMALLOWCOM_ENABLED == STD_ON*/

#if (BSWM_ACTION_COMMMODELIMITATION_ENABLED == STD_ON)
    P2CONST(BswM_ActionComMModeLimitLCfgType, TYPEDEF, BSWM_CONST)
    comMModeLimitActions; /*BswMComMModeLimitation*/
#endif                    /*BSWM_ACTION_COMMMODELIMITATION_ENABLED == STD_ON*/

#if (BSWM_ACTION_COMMMODESWITCH_ENABLED == STD_ON)
    P2CONST(BswM_ActionComMModeSwitchLCfgType, TYPEDEF, BSWM_CONST)
    comMModeSwitchActions; /*BswMComMModeSwitch*/
#endif                     /*BSWM_ACTION_COMMMODESWITCH_ENABLED == STD_ON*/

#if (BSWM_MAX_NUM_CORE > 1)
#if (BSWM_ACTION_COREHALTMODE_ENABLED == STD_ON)
    P2CONST(BswM_ActionCoreHaltModeLCfgType, TYPEDEF, BSWM_CONST)
    coreHaltModeActions; /*BswMCoreHaltMode*/
#endif                   /*BSWM_ACTION_COREHALTMODE_ENABLED == STD_ON*/
#endif                   /*BSWM_MAX_NUM_CORE > 1*/

#if (BSWM_ACTION_DEADLINEMONITORINGCONTROL_ENABLED == STD_ON)
    P2CONST(BswM_ActionDMControlLCfgType, TYPEDEF, BSWM_CONST)
    dmCtrlActionLCfgs; /*BswMDeadlineMonitoringControl*/
#endif                 /*BSWM_ACTION_DEADLINEMONITORINGCONTROL_ENABLED == STD_ON*/

#if (BSWM_ACTION_PDUGROUPSWITCH_ENABLED == STD_ON)
    P2CONST(BswM_ActionPduGrpSwitchLCfgType, TYPEDEF, BSWM_CONST)
    pduGrpSwitchActionLCfgs; /*BswMPduGroupSwitch*/
#endif                       /*BSWM_ACTION_PDUGROUPSWITCH_ENABLED == STD_ON*/

#if (BSWM_ACTION_SWITCHIPDUMODE_ENABLED == STD_ON)
    P2CONST(BswM_ActionSwitchIpduModeLCfgType, TYPEDEF, BSWM_CONST)
    switchIpduModeActionLCfgs; /*BswMSwitchIPduMode*/
#endif                         /*BSWM_ACTION_SWITCHIPDUMODE_ENABLED == STD_ON*/

#if (BSWM_ACTION_TRIGGERIPDUSEND_ENABLED == STD_ON)
    P2CONST(BswM_ActionTriggerIPduSendLCfgType, TYPEDEF, BSWM_CONST)
    triggerIpduSendActionLCfgs; /*BswMTriggerIPduSend*/
#endif                          /*BSWM_ACTION_TRIGGERIPDUSEND_ENABLED == STD_ON*/

#if (BSWM_ACTION_ECUMDRIVERINITLIST_ENABLED == STD_ON)
    P2CONST(BswM_ActionEcuMDrvInitLCfgType, TYPEDEF, BSWM_CONST)
    ecuMDrvInitListActions; /*BswMEcuMDriverInitListBswM*/
#endif                      /*BSWM_ACTION_ECUMDRIVERINITLIST_ENABLED == STD_ON*/

#if (BSWM_ACTION_ECUMGODOWNHALTPOLL_ENABLED == STD_ON)
    P2CONST(BswM_ActionEcuMGoDownLCfgType, TYPEDEF, BSWM_CONST)
    ecuMGoDownHaltPollActions; /*BswMEcuMGoDownHaltPoll*/
#endif                         /*BSWM_ACTION_ECUMGODOWNHALTPOLL_ENABLED == STD_ON*/

#if (BSWM_ACTION_ECUMSELECTSHUTDOWNTARGET_ENABLED == STD_ON)
    P2CONST(BswM_ActionEcuMSelectShutTgtLCfgType, TYPEDEF, BSWM_CONST)
    ecuMSelectShutTgtActions; /*BswMEcuMSelectShutdownTarget*/
#endif                        /*BSWM_ACTION_ECUMSELECTSHUTDOWNTARGET_ENABLED == STD_ON*/

#if (BSWM_ACTION_ECUMSTATESWITCH_ENABLED == STD_ON)
    P2CONST(BswM_ActionEcuMStateSwitchLCfgType, TYPEDEF, BSWM_CONST)
    ecuMStateSwitchActions; /*BswMEcuMStateSwitch*/
#endif                      /*BSWM_ACTION_ECUMSTATESWITCH_ENABLED == STD_ON*/

#if (BSWM_ACTION_ETHIFSWITCHPORTGROUPREQUESTMODE_ENABLED == STD_ON)
    P2CONST(BswM_ActionEthIfSwiPortGruLCfgType, TYPEDEF, BSWM_CONST)
    ethifSwiPortGruRqstActions; /*BswMEthIfSwitchPortGroupRequestMode*/
#endif                          /*BSWM_ACTION_ETHIFSWITCHPORTGROUPREQUESTMODE_ENABLED == STD_ON*/

#if (BSWM_ACTION_FRSMALLSLOTS_ENABLED == STD_ON)
    P2CONST(BswM_ActionFrSMAllSlotsLCfgType, TYPEDEF, BSWM_CONST)
    frSMAllSlotsActions; /*BswMFrSMAllSlots*/
#endif                   /*BSWM_ACTION_FRSMALLSLOTS_ENABLED == STD_ON*/

#if (BSWM_ACTION_J1939DCMSTATESWITCH_ENABLED == STD_ON)
    P2CONST(BswM_ActionJ1939DcmStateSwitchLCfgType, TYPEDEF, BSWM_CONST)
    j1939DcmStateSwitchActions; /*BswMJ1939DcmStateSwitch*/
#endif                          /*BSWM_ACTION_J1939DCMSTATESWITCH_ENABLED == STD_ON*/

#if (BSWM_ACTION_J1939RMSTATESWITCH_ENABLED == STD_ON)
    P2CONST(BswM_ActionJ1939RmStateSwitchLCfgType, TYPEDEF, BSWM_CONST)
    j1939RmStateSwitchActions; /*BswMJ1939RmStateSwitch*/
#endif                         /*BSWM_ACTION_J1939RMSTATESWITCH_ENABLED == STD_ON*/

#if (BSWM_ACTION_LINSCHEDULESWITCH_ENABLED == STD_ON)
    P2CONST(BswM_ActionLinScheduleSwitchLCfgType, TYPEDEF, BSWM_CONST)
    linSchdlSwitchActionLCfgs; /*BswMLinScheduleSwitch*/
#endif                         /*BSWM_ACTION_LINSCHEDULESWITCH_ENABLED == STD_ON*/

#if (BSWM_ACTION_NMCONTROL_ENABLED == STD_ON)
    P2CONST(BswM_ActionNMControlLCfgType, TYPEDEF, BSWM_CONST)
    nmControlActions; /*BswMNMControl*/
#endif                /*BSWM_ACTION_NMCONTROL_ENABLED == STD_ON*/

#if (BSWM_ACTION_PDUROUTERCONTROL_ENABLED == STD_ON)
    P2CONST(BswM_ActionPduRCtrlLCfgType, TYPEDEF, BSWM_CONST)
    pdurCtrlActionLCfgs; /*BswMPduRouterControl*/
#endif                   /*BSWM_ACTION_PDUROUTERCONTROL_ENABLED == STD_ON*/

#if (BSWM_ACTION_RTEMODEREQUEST_ENABLED == STD_ON)
    P2CONST(BswM_ActionRteModeReqLCfgType, TYPEDEF, BSWM_CONST)
    rteModeRequestActions; /*BswMRteModeRequest*/
#endif                     /*BSWM_ACTION_RTEMODEREQUEST_ENABLED == STD_ON*/

#if (BSWM_ACTION_RTESWITCH_ENABLED == STD_ON)
    P2CONST(BswM_ActionRteSwitchLCfgType, TYPEDEF, BSWM_CONST)
    rteSwitchActions; /*BswMRteSwitch*/
#endif                /*BSWM_ACTION_RTESWITCH_ENABLED == STD_ON*/

#if (BSWM_ACTION_SCHMSWITCH_ENABLED == STD_ON)
    P2CONST(BswM_ActionSchMSwitchLCfgType, TYPEDEF, BSWM_CONST)
    schMSwitchActions; /*BswMSchMSwitch*/
#endif                 /*BSWM_ACTION_SCHMSWITCH_ENABLED == STD_ON*/

#if (BSWM_ACTION_SDCLIENTSERVICEMODEREQUEST_ENABLED == STD_ON)
    P2CONST(BswM_ActionSdCliServiceModeReqLCfgType, TYPEDEF, BSWM_CONST)
    sdCliServiceModeReqActions; /*BswMSdClientServiceModeRequest*/
#endif                          /*BSWM_ACTION_SDCLIENTSERVICEMODEREQUEST_ENABLED == STD_ON*/

#if (BSWM_ACTION_SDCONSUMEDEVENTGROUPMODEREQUEST_ENABLED == STD_ON)
    P2CONST(BswM_ActionSdConsumedEvGrpModeReqLCfgType, TYPEDEF, BSWM_CONST)
    sdConsumedEvGrpModeReqActions; /*BswMSdConsumedEventGroupModeRequest*/
#endif                             /*BSWM_ACTION_SDCONSUMEDEVENTGROUPMODEREQUEST_ENABLED == STD_ON*/

#if (BSWM_ACTION_SDSERVERSERVICEMODEREQUEST_ENABLED == STD_ON)
    P2CONST(BswM_ActionSdServerServiceModeReqLCfgType, TYPEDEF, BSWM_CONST)
    sdServerServiceModeReqActions; /*BswMSdServerServiceModeRequest*/
#endif                             /*BSWM_ACTION_SDSERVERSERVICEMODEREQUEST_ENABLED == STD_ON*/

#if (BSWM_ACTION_TIMERCONTROL_ENABLED == STD_ON)
    P2CONST(BswM_ActionTimerCtrlLCfgType, TYPEDEF, BSWM_CONST)
    timerAction; /*BswMUserCallout*/
#endif           /*BSWM_ACTION_TIMERCONTROL_ENABLED == STD_ON*/

#if (BSWM_ACTION_USERCALLOUT_ENABLED == STD_ON)
    P2CONST(BswM_ActionUserCalloutLCfgType, TYPEDEF, BSWM_CONST)
    userCalloutActions; /*BswMUserCallout*/
#endif                  /*BSWM_ACTION_USERCALLOUT_ENABLED == STD_ON*/
} BswM_ActionItemsLCfgType;

typedef struct
{
    BswM_RuleStateType ruleStatus; /*old status for Rules*/
    boolean ruleNeedArbitrate;     /*TRUE, rule need arbitration*/
} BswM_RuleRunTimeType;

/*type definition for link time configure of partition*/
typedef struct
{
    P2CONST(BswM_EventRqstPortLCfgType, TYPEDEF, BSWM_CONST) evRqstLCfg;
    P2CONST(BswM_ModeRqstPortLCfgType, TYPEDEF, BSWM_CONST) modeRqstLCfg;
    BswM_RuleIndexType numOfRules;
    P2CONST(BswM_RuleLcCfgType, TYPEDEF, BSWM_CONST) ruleLCfg;
    BswM_RuleIndexType numOfDefRules;
    P2CONST(BswM_RuleIndexType, TYPEDEF, BSWM_CONST) defRuleRefLCfg;
    uint8 numOfActionList;
    P2CONST(BswM_ActionListLCfgType, TYPEDEF, BSWM_CONST) acListLCfg;
    P2CONST(BswM_ActionItemsLCfgType, TYPEDEF, BSWM_CONST) acItemsLCfg;
    /*Runtime status variable*/
    P2VAR(BswM_RuleRunTimeType, TYPEDEF, BSWM_VAR_CLEARED) ruleRutTimeStatus; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
} BswM_PartitionLCfgType;

/*link time configure type definition for partitions and cores*/
typedef struct
{
    P2CONST(BswM_PartitionLCfgType, TYPEDEF, BSWM_CONST) bswmPartLCfgs;
} BswM_LCfgType;

/*This structure contains all post-build configurable parameters of the BSW Mode
 * Manager. A pointer to this structure is passed to the BSW Mode Manager
 * initialization function for configuration.
 * This container exists once per partition.*/
typedef struct
{
    uint8 reserved;
} BswM_ConfigType;
/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/

extern CONSTP2CONST(BswM_LCfgType, BSWM_CONST, BSWM_CONST) BswM_CoreLinkCfg;

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

/**
 * Initializes the BSW Mode Manager.
 * Service ID: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Conditionally Reentrant
 * Parameters(IN): ConfigPtr, Pointer to post-build configuration data
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BSWM_CODE)
BswM_Init(P2CONST(BswM_ConfigType, AUTOMATIC, BSWM_APPL_DATA) ConfigPtr);

/**
 * Deinitializes the BSW Mode Manager.
 * After a call of BswM_Deinit no mode processing shall be performed by BswM
 * even if any mode requests are made or the BswM main function is called.
 * Service ID: 0x04
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BSWM_CODE) BswM_Deinit(void);

#if (BSWM_VERSION_INFO_API == STD_ON)
/**
 * Returns the version information of this module.
 * Service ID: 0x01
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): VersionInfo, pointer to where to store the version
 *                               information of the module
 * Return value: NA
 */
FUNC(void, BSWM_CODE)
BswM_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, BSWM_APPL_DATA) VersionInfo);
#endif /*BSWM_VERSION_INFO_API == STD_ON*/

/**
 * Generic function call to request modes. This function shall only be used by
 * other BSW modules that does not have a specific mode request interface.
 * Service ID: 0x02
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): requesting_user, The user that requests the mode
 *                 requested_mode, The requested mode
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */

#if (BSWM_MULTI_PARTITION_ENABLED == STD_ON)
/**
 * Function called by Restart Task if the partition containing the BswM has
 * been restarted.
 * Service ID: 0x1e
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BSWM_CODE)
BswM_BswMPartitionRestarted(void);
#endif /*BSWM_MULTI_PARTITION_ENABLED == STD_ON*/

#if (BSWM_GENERIC_REQUEST_ENABLED == STD_ON)
FUNC(void, BSWM_CODE)
BswM_RequestMode(BswM_UserType requestingUser, uint16 requestedMode);
#endif /*BSWM_GENERIC_REQUEST_ENABLED == STD_ON*/

#endif /* BSWM_H */
