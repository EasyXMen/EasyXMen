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
 **  @file               : BswM.h
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : LC configures the data type, as well as the declaration of the common API
 **
 ***********************************************************************************************************************/
/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author       Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V02.02.00 2024-08-30    Jian.Jiang     1. Update code according AUTOSAR R23-11 Specification
 *  V02.02.01 2025-01-09    Jian.Jiang     1. Specification of QAC Notes and Explanation of Reasons
 *  V02.02.02 2025-01-18    Jian.Jiang     1. Change PB structure code adjustment
 *  V02.02.03 2025-02-19    Jian.Jiang     1. Specification of QAC Notes and Explanation of Reasons
*   V02.02.04 2025-04-28    Xudong.Guan    1. Remove include file "Rte_BswM_Type.h"
 *
 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:BswM<br>
  RuleSorce:puhua_rules-2024.12.rcf

   \li VL_BswM_1505
      Reason: Functions defined out are not used in the project and have no impact on the integration application.
      Risk: No impact on integrated functions.
      Prevention: Provided to the application to use, the application use need to use the corresponding interface.

   \li VL_BswM_2991
      Reason: Functions defined out are not used in the project and have no impact on the integration application.
      Risk: No impact on integrated functions.
      Prevention: Provided to the application to use, the application use need to use the corresponding interface.

   \li VL_BswM_2995
      Reason: Functions defined out are not used in the project and have no impact on the integration application.
      Risk: No impact on integrated functions.
      Prevention: Provided to the application to use, the application use need to use the corresponding interface.

   \li VL_BswM_2981
      Reason:Init may be necessary for return variant.
      Risk:There may be redundant assignments.
      Prevention:Code inspection and test of the different configurations in the component test.

    \li VL_BswM_3432
      Reason: Function-like macros are used to allow more efficient code.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_BswM_0380
      Reason: Macro definitions are necessary
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_BswM_Action
      Reason: Generate code some return values not used
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_BswM_5004
      Reason: The usage of this file depends on the config
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_BswM_0553
      Reason: The usage of this file depends on the config
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_BswM_1317_1297
      Reason: Definitions from other modules are used
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_BswM_STMIF
      Reason: Functions handle specific tasks in the behavior of the entire component.
    Task has different scenarios that need to be covered, which depends on local conditions - this
    leads to deep nesting of the control structure.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_BswM_STCAL
      Reason: The standard compliance vote was higher than the measurement threshold. In addition, a typical way to
    reduce STCAL is to deepen function nesting, which increases call stack usage and runtime.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_BswM_0793
      Reason: Dependent on configuration.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_BswM_2743
      Reason: Use do while to control program execution.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

 */

#ifndef BSWM_H
#define BSWM_H

/* =================================================== inclusions =================================================== */
#include "BswM_Types.h"
#include "Os.h"
#ifdef __cplusplus
extern "C" {
#endif
/* =============================================== version information ============================================== */
#define BSWM_MODULE_ID 42u
#if (BSWM_MULTI_PARTITION_ENABLED == STD_ON)
#define BSWM_INSTANCE_ID ((uint8)GetApplicationID())
#else
#define BSWM_INSTANCE_ID 0u
#endif /*BSWM_MULTI_PARTITION_ENABLED == STD_ON*/
#define BSWM_VENDOR_ID        62u
#define BSWM_SW_MAJOR_VERSION 2u
#define BSWM_SW_MINOR_VERSION 2u
#define BSWM_SW_PATCH_VERSION 3u
#define BSWM_AR_MAJOR_VERSION 4u
#define BSWM_AR_MINOR_VERSION 9u
#define BSWM_AR_PATCH_VERSION 0u

/* ===================================================== macros ===================================================== */

#define BSWM_API_ID_INIT                                  0u
#define BSWM_API_ID_GET_VERSION_INFO                      0x01u
#define BSWM_API_ID_MAINFUNCTION                          0x03u
#define BSWM_API_ID_CANSM_CURRENT_STATE                   0x05u
#define BSWM_API_ID_DCM_COM_MODE_CURRENT_STATE            0x06u
#define BSWM_API_ID_BSWM_MODE_REQUEST                     0x1cu
#define BSWM_API_ID_BSWM_MODE_SWITCH                      0x1du
#define BSWM_API_ID_CANSM_CURRENT_ICOM_CONFIG             0x1au
#define BSWM_API_ID_COMM_CURRENT_MODE                     0x0eu
#define BSWM_API_ID_COMM_CURRENT_PNC_MODE                 0x15u
#define BSWM_API_ID_COMM_INITIATE_RESET                   0x22u
#define BSWM_API_ID_DCM_APPLICATION_UPDATED               0x14u
#define BSWM_API_ID_ECUM_CURRENT_STATE                    0x0fu
#define BSWM_API_ID_ECUM_CURRENT_WAKEUP                   0x10u
#define BSWM_API_ID_ECUM_REQUESTED_STATE                  0x23u
#define BSWM_API_ID_ETHIF_PORT_GRU_LINK_STATE             0x26u
#define BSWM_API_ID_FRSM_CURRENT_STATE                    0x0cu
#define BSWM_API_ID_ETHSM_CURRENT_STATE                   0x0du
#define BSWM_API_ID_J1939DCM_BROADCAST_STATUS             0x1bu
#define BSWM_API_ID_J1939NM_STATE_CHANGE_NOTIFY           0x18u
#define BSWM_API_ID_LINSM_CURRENT_STATE                   0x09u
#define BSWM_API_ID_LINSM_CURRENT_SCHEDULE                0x0au
#define BSWM_API_ID_LINTP_REQUEST_MODE                    0x0bu
#define BSWM_API_ID_NMIF_CAR_WAKEUP_INDICATION            0x24u
#define BSWM_API_ID_NM_STATECHANGENOTIFICATION            0x27u
#define BSWM_API_ID_NVM_CURRENT_BLOCK_MODE                0x16u
#define BSWM_API_ID_NVM_CURRENT_JOB_MODE                  0x17u
#define BSWM_API_ID_SD_CLIENT_SERVICE_CURRENT_STATE       0x1fu
#define BSWM_API_ID_SD_EVENT_HANDLER_CURRENT_STATE        0x20u
#define BSWM_API_ID_SD_CONSUMED_EVENT_GROUP_CURRENT_STATE 0x21u
#define BSWM_API_ID_SOAD_SOCONMODECHG                     0x2au
#define BSWM_API_ID_MODE_SWITCH_ERROR_NOTIFICATION        0xffu
#define BSWM_API_ID_MODE_REQUEST_PORT                     0xfeu
#define BSWM_API_ID_MODE_NOTIFICATION_PORT                0xfdu
#define BSWM_API_ID_BSW_MODE_NOTIFICATION                 0xfcu
#define BSWM_API_ID_WDGM_REQUEST_PARTITION_RESET          0x11u
#define BSWM_API_ID_BSWM_PARTITION_RESTARTED              0x1eu
#define BSWM_API_ID_REQUEST_MODE                          0x02u

/* API ID for DoActionList */
#define BSWM_API_ID_DO_ACTION_LIST 0X07u

/*********Error code for development****************/
/* No errors occur */
#define BSWM_E_OK 0x0u
/*A service was called prior to initialization */
#define BSWM_E_UNINIT 0x01u
/*A null pointer was passed as an argument*/
#define BSWM_E_NULL_POINTER 0x02u
/*A parameter was invalid (unspecific)*/
#define BSWM_E_PARAM_INVALID 0x03u
/*A requesting user was out of range*/
#define BSWM_E_REQ_USER_OUT_OF_RANGE 0x04u
/*A requested mode was out of range*/
#define BSWM_E_REQ_MODE_OUT_OF_RANGE 0x05u
/*The provided configuration is inconsistent*/
#define BSWM_E_PARAM_CONFIG 0x06u
/*A parameter pointer was invalid*/
#define BSWM_E_PARAM_POINTER 0x07u
/*Invalid configuration set selection*/
#define BSWM_E_INIT_FAILED 0x08u

/* ================================================ type definitions ================================================ */

/**
 * @brief This type identifies the modes that can be requested by BswM Users.
 *
 */
typedef uint16 BswM_ModeType;
/**
 * @brief This type identifies a BswM User that makes mode requests to the BswM.
 *
 */
typedef uint16 BswM_UserType;

/* PRQA S 5004 ++ */ /* VL_BswM_5004 */
/**
 * @brief function type definition when the actionList is executed
 */
typedef Std_ReturnType (*BswM_ActionListCtrlFuncType)(void);
/* PRQA S 5004 -- */

/**
 * @brief define the actionList function type definition
 *
 */
typedef uint16 BswM_ActionListHandlerType;

/**
 * @brief This type identifies the index of a rule.
 *
 */
typedef struct BswMRulePbIndexTag
{
    BswM_RuleIndexType ruleIndex; /**> The index of the rule. @range 0 ~ 0xFF  */
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
    uint8 pbConfigTag; /**> The index of the PB configuration. @range 0 ~ 0xFF */
#endif                 /* BSWM_PB_CONFIG_ENABLED == STD_ON */
} BswMRulePbIndexType;

#if (BSWM_ETHIFPORTGROUPLINKSTATECHG_ENABLED == STD_ON)
/**
 * @brief This type identifies the index of a ETHif port group.
 *
 */
typedef struct BswMEthIfSwitchPortGroupIdxTag
{
    EthIf_SwitchPortGroupIdxType portGroupInx; /**> The index of the port group. @range 0 ~ 0xFF */
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
    uint8 pbConfigTag; /**> The index of the PB configuration. @range 0 ~ 0xFF */
#endif                 /* BSWM_PB_CONFIG_ENABLED == STD_ON */
} BswMEthIfSwitchPortGroupIdxType;
#endif /* BSWM_ETHIFPORTGROUPLINKSTATECHG_ENABLED == STD_ON */

#if (BSWM_LINSCHEDULEINDICATION_ENABLED == STD_ON)
/**
 * @brief  This type identifies  for the Linif Schedule handler
 *
 */
typedef struct BswMLinIfSchHandleTag
{
    LinIf_SchHandleType schHandle; /**> The index of the Linif Schedule handler. @range 0 ~ 0xFF */
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
    uint8 pbConfigTag; /**> The index of the PB configuration. @range 0 ~ 0xFF */
#endif                 /* BSWM_PB_CONFIG_ENABLED == STD_ON */
} BswMLinIfSchHandleType;

/**
 * @brief  This type identifies  for the Linif network
 *
 */
typedef struct BswMNetworkHandleTag
{
    NetworkHandleType networkHandle; /**> The index of  linif network. @range 0 ~ 0xFFFF */
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
    uint8 pbConfigTag; /**> The index of the PB configuration. @range 0 ~ 0xFF */
#endif                 /* BSWM_PB_CONFIG_ENABLED == STD_ON */
} BswMNetworkHandleType;
#endif /* BSWM_LINSCHEDULEINDICATION_ENABLED == STD_ON */

/**
 * @brief This type identifies the request port configuration.
 *
 */
typedef struct BswM_RqstPortLcCfgTag
{
    const BswMRulePbIndexType* belongToRlue; /**> The rule index that the request port belongs to. @range None */
    BswM_RuleIndexType belongToRlueNum; /**> The number of rules that the request port belongs to. @range 0 ~ 0xFF */
    BswM_RqstPortProcessType process;   /**> The type of request port process. @range 0 ~ 0xFF */
} BswM_RqstPortLcCfgType;

/**
 * @brief This type identifies the request port configuration.
 *
 */
typedef struct BswM_EventRqstPortLCfgTag
{
#if (BSWM_EVENT_RQSTPORT_ENABLE == STD_ON) /**>EventRequestPort configuration exists*/
#if (BSWM_EVENT_COMM_INITIATE_RESET_LIST_ENABLE == STD_ON)
    const BswM_RqstPortLcCfgType*
        comMInitReset; /**>The configuration of the request port for the event comM initiate reset. @range None */
#endif                 /*BSWM_EVENT_COMM_INITIATE_RESET_LIST_ENABLE == STD_ON*/

#if (BSWM_EVENT_DCM_APPLICATION_UPDATED_INDICATION_ENABLE == STD_ON)
    const BswM_RqstPortLcCfgType* dcmAppUpdate; /**>The configuration of the request port for the event dcm application
                                                   updated indication. @range None */
#endif                                          /*BSWM_EVENT_DCM_APPLICATION_UPDATED_INDICATION_ENABLE == STD_ON*/

#if (BSWM_EVENT_MODE_SWITCH_ERROR_EVENT_ENABLE == STD_ON)
    const BswM_RqstPortLcCfgType*
        modeSwiErr; /**>The configuration of the request port for the event mode switch error. @range None */
    const uint16*
        modeSwiErrPort; /**>The configuration of the request port for the event mode switch error. @range None */
#endif                  /*BSWM_EVENT_MODE_SWITCH_ERROR_EVENT_ENABLE == STD_ON*/

#if (BSWM_EVENT_NM_CAR_WAKEUP_INDICATION_ENABLE == STD_ON)
    const BswM_RqstPortLcCfgType*
        nmWakeUpInd; /**>The configuration of the wakeup port for the  event nm wakeup indication. @range None */
    const NetworkHandleType*
        nmWkChRef; /**>The configuration of the wakeup port for the  event nm wakeup indication. @range None */
#endif             /*BSWM_EVENT_NM_CAR_WAKEUP_INDICATION_ENABLE == STD_ON*/

#if (BSWM_EVENT_PARTITION_RESTARTED_ENABLE == STD_ON)
    const BswM_RqstPortLcCfgType*
        bswMParRestart; /**>The configuration of the request port for the event partition restarted. @range None */
#endif                  /*BSWM_EVENT_PARTITION_RESTARTED_ENABLE == STD_ON*/

#if (BSWM_EVENT_SWITCH_ACK_NOTIFICATION_ENABLE == STD_ON)
    const BswM_RqstPortLcCfgType*
        modeSwiAck; /**>The configuration of the request port for the event Switch Ack Notification. @range None */
    const uint16*
        modeSwiAckPort; /**>The configuration of the request port for the event Switch Ack Notification. @range None */
#endif                  /*BSWM_EVENT_SWITCH_ACK_NOTIFICATION_ENABLE == STD_ON*/

#else  /*EventRequestPort configuration don't exist*/
    uint8 dummy; /**< The dummy variable. @range None */
#endif /*BSWM_EVENT_RQSTPORT_ENABLE == STD_ON*/
} BswM_EventRqstPortLCfgType;

/**
 * @brief This type identifies the mode request port configuration.
 *
 */
typedef struct BswM_ModeRqstPortLCfgTag
{
#if (BSWM_MODE_RQSTPORT_ENABLE == STD_ON) /*ModeRequestPort configuration exists*/
#if (BSWM_BSWMODENOTIFICATION_ENABLED == STD_ON)
    /**>The configuration of the request port for the event Bsw Mode Notification. @range None */
    const BswM_RqstPortLcCfgType* bswModeNoti;
    /**>The configuration of the request port for the event Bsw Mode Notification. @range None */
    const BswM_ModeGroupType* bswModeNotiRef;
#endif /*BSWM_BSWMODENOTIFICATION_ENABLED == STD_ON*/

#if (BSWM_CANSMINDICATION_ENABLED == STD_ON)
    /**>The configuration of the request port for the event CanSm Indication. @range None */
    const BswM_RqstPortLcCfgType* cansmInd;
    /**>The configuration of the request port for the event Cansm Indication. @range None */
    const NetworkHandleType* cansmIndChRef;
#endif /*BSWM_CANSMINDICATION_ENABLED == STD_ON*/

#if (BSWM_COMMINDICATION_ENABLED == STD_ON)
    /**>The configuration of the request port for the event ComM Indication. @range None */
    const BswM_RqstPortLcCfgType* comMInd;
    /**>The configuration of the request port for the event ComM Indication. @range None */
    const NetworkHandleType* comMIndChRef;
#endif /*BSWM_COMMINDICATION_ENABLED == STD_ON*/
#if (BSWM_COMMPNCREQUEST_ENABLED == STD_ON)
    /**>The configuration of the request port for the event ComM PNC Request. @range None */
    const BswM_RqstPortLcCfgType* comMPncRqst;
    /**>The configuration of the request port for the event comM PNC Request. @range None */
    const PNCHandleType* comMPncRef;
#endif /*BSWM_COMMPNCREQUEST_ENABLED == STD_ON*/

#if (BSWM_DCMCOMMODEREQUEST_ENABLED == STD_ON)
    /**>The configuration of the request port for the event DcM Com Mode Request. @range None */
    const BswM_RqstPortLcCfgType* dcmComModeRqst;
    /**>The configuration of the request port for the event DcM Com Mode Request. @range None */
    const NetworkHandleType* dcmChRef;
#endif /*BSWM_DCMCOMMODEREQUEST_ENABLED == STD_ON*/

#if (BSWM_ECUMINDICATION_ENABLED == STD_ON)
    /**>The configuration of the request port for the event ecuM State. @range None */
    const BswM_RqstPortLcCfgType* ecuMState;
#endif /*BSWM_ECUMINDICATION_ENABLED == STD_ON*/

#if (BSWM_ECUMRUNREQUESTINDICATION_ENABLED == STD_ON)
    /**>The configuration of the request port for the event EcuM Request Protocol. @range None */
    const BswM_RqstPortLcCfgType* ecuMRqstIndSrc;
    /**>The configuration of the request port for the event EcuM Request Protocol. @range None */
    const EcuM_StateType* ecuMRqstIndSrcRef;
#endif /*BSWM_ECUMRUNREQUESTINDICATION_ENABLED == STD_ON*/

#if (BSWM_ECUMWAKEUPSOURCE_ENABLED == STD_ON)
    /**>The configuration of the request port for the event EcuM Wakeup Source. @range None */
    const BswM_RqstPortLcCfgType* ecuMWkSrc;
    /**>The configuration of the request port for the event EcuM Wakeup Source. @range None */
    const EcuM_WakeupSourceType* ecuMWkSrcRef;
#endif /*BSWM_ECUMWAKEUPSOURCE_ENABLED == STD_ON*/

#if (BSWM_ETHIFPORTGROUPLINKSTATECHG_ENABLED == STD_ON)
    /**>The configuration of the request port for the event EthIf Port Group Link State Changed. @range None */
    const BswM_RqstPortLcCfgType* ethIfPortGroLink;
#if (BSWM_PB_CONFIG_ENABLED == STD_OFF)
    /**>The configuration of the request port for the event EthIf Port Group Link State Changed. @range None */
    const EthIf_SwitchPortGroupIdxType* ethIfPortGroRef;
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
#endif /*BSWM_ETHIFPORTGROUPLINKSTATECHG_ENABLED == STD_ON*/

#if (BSWM_ETHSMINDICATION_ENABLED == STD_ON)
    /**>The configuration of the request port for the event ETHSM Indication. @range None */
    const BswM_RqstPortLcCfgType* ethsmInd;
    /**>The configuration of the request port for the event ETHSM Indication. @range None */
    const NetworkHandleType* ethsmChRef;
#endif /*BSWM_ETHSMINDICATION_ENABLED == STD_ON*/

#if (BSWM_FRSMINDICATION_ENABLED == STD_ON)
    /**>The configuration of the request port for the event FrSM Indication. @range None */
    const BswM_RqstPortLcCfgType* frsmInd;
    /**>The configuration of the request port for the event FrSM Indication. @range None */
    const NetworkHandleType* frsmChRef;
#endif /*BSWM_FRSMINDICATION_ENABLED == STD_ON*/

#if (BSWM_GENERICREQUEST_ENABLED == STD_ON)
    /**>The configuration of the request port for the event generic request. @range None */
    const BswM_RqstPortLcCfgType* genericRqst;
    /**>The configuration of the request port for the event generic request. @range None */
    const BswM_UserType* genRqstUserRef;
#endif /*BSWM_GENERICREQUEST_ENABLED == STD_ON*/

#if (BSWM_J1939DCMBROADCASTSTATUS_ENABLED == STD_ON)
    /**>The configuration of the request port for the event J1939 DCM. @range None */
    const BswM_RqstPortLcCfgType* j1939DcmBC;
    /**>The configuration of the request port for the event J1939 DCM. @range None */
    const NetworkHandleType* j1939DcmBCChRef;
#endif /*BSWM_J1939DCMBROADCASTSTATUS_ENABLED == STD_ON*/

#if (BSWM_J1939NMINDICATION_ENABLED == STD_ON)
    /**>The configuration of the request port for the event J1939 NM. @range None */
    const BswM_RqstPortLcCfgType* j1939NmInd;
    /**>The configuration of the request port for the event J1939 NM. @range None */
    const NetworkHandleType* j1939NmChRef;
    /**>The configuration of the request port for the event J1939 NM. @range None */
    const uint8* j1939NmNodeRef;
#endif /*BSWM_J1939NMINDICATION_ENABLED == STD_ON*/

#if (BSWM_LINSMINDICATION_ENABLED == STD_ON)
    /**>The configuration of the request port for the event linsm indication. @range None */
    const BswM_RqstPortLcCfgType* linsmInd;
    /**>The configuration of the request port for the event linsm indication. @range None */
    const NetworkHandleType* linsmIndChRef;
#endif /*BSWM_LINSMINDICATION_ENABLED == STD_ON*/

#if (BSWM_LINSCHEDULEINDICATION_ENABLED == STD_ON)
    /**>The configuration of the request port for the event lin schedule indication. @range None */
    const BswM_RqstPortLcCfgType* linSchInd;
#if (BSWM_PB_CONFIG_ENABLED == STD_OFF)
    /**>The configuration of the request port for the event lin schedule indication. @range None */
    const NetworkHandleType* linSchNKChRef;
    /**>The configuration of the request port for the event lin schedule indication. @range None */
    const LinIf_SchHandleType* linSchChRef;
#endif /*BSWM_PB_CONFIG_ENABLED == STD_OFF*/
#endif /*BSWM_LINSCHEDULEINDICATION_ENABLED == STD_ON*/

#if (BSWM_LINTPMODEREQUEST_ENABLED == STD_ON)
    /**>The configuration of the request port for the event lin tp mode request. @range None */
    const BswM_RqstPortLcCfgType* lintpModeRqst;
    /**>The configuration of the request port for the event lin tp mode request. @range None */
    const NetworkHandleType* lintpChRef;
#endif /*BSWM_LINTPMODEREQUEST_ENABLED == STD_ON*/

#if (BSWM_NMSTATECHANGENOTIFICATION_ENABLED == STD_ON)
    /**>The configuration of the request port for the event nm state change notification. @range None */
    const BswM_RqstPortLcCfgType* nmInd;
    /**>The configuration of the request port for the event nm state change notification. @range None */
    const NetworkHandleType* nmChRef;
#endif /*BSWM_NMSTATECHANGENOTIFICATION_ENABLED == STD_ON*/

#if (BSWM_NVMJOBMODEINDICATION_ENABLED == STD_ON)
    /**>The configuration of the request port for the event nvm job mode indication. @range None */
    const BswM_RqstPortLcCfgType* nvmJobInd;
    /**>The configuration of the request port for the event nvm job mode indication. @range None */
    const NvM_MultiBlockRequestType* nvmService;
#endif /*BSWM_NVMJOBMODEINDICATION_ENABLED == STD_ON*/

#if (BSWM_NVMREQUEST_ENABLED == STD_ON)
    /**>The configuration of the request port for the event NVM_REQUEST. @range None */
    const BswM_RqstPortLcCfgType* nvmRqst;
    /**>The configuration of the request port for the event NVM_REQUEST. @range None */
    const NvM_BlockIdType* nvmBlockRef;
#endif /*BSWM_NVMREQUEST_ENABLED == STD_ON*/

#if (BSWM_SDCLIENTSERVICECURRENTSTATE_ENABLED == STD_ON)
    /**>The configuration of the request port for the event sd client state. @range None */
    const BswM_RqstPortLcCfgType* sdClientSrv;
    /**>The configuration of the request port for the event sd client state. @range None */
    const uint16* sdClientSrvMethodsRef;
#endif /*BSWM_SDCLIENTSERVICECURRENTSTATE_ENABLED == STD_ON*/

#if (BSWM_SDCONSUMEDEVENTGROUPCURRENTSTATE_ENABLED == STD_ON)
    /**>The configuration of the request port for the event sd consumed event group. @range None */
    const BswM_RqstPortLcCfgType* sdConEvGroup;
    /**>The configuration of the request port for the event sd consumed event group. @range None */
    const uint16* sdConEvGroupIdRef;
#endif /*BSWM_SDCONSUMEDEVENTGROUPCURRENTSTATE_ENABLED == STD_ON*/

#if (BSWM_SDEVENTHANDLERCURRENTSTATE_ENABLED == STD_ON)
    /**>The configuration of the request port for the event sd event handler. @range None */
    const BswM_RqstPortLcCfgType* sdEvHandler;
    /**>The configuration of the request port for the event sd event handler. @range None */
    const uint16* sdEvHandleIdRef;
#endif /*BSWM_SDEVENTHANDLERCURRENTSTATE_ENABLED == STD_ON*/

#if (BSWM_SOADSOCONMODECHG_ENABLED == STD_ON)
    /**>The configuration of the request port for the event soad socon mode change. @range None */
    const BswM_RqstPortLcCfgType* soadSoconModeChange;
    /**>The configuration of the request port for the event soad socon mode change. @range None */
    const uint16* soadSoConIdRef;
#endif /*BSWM_SOADSOCONMODECHG_ENABLED == STD_ON*/

#if (BSWM_SWCMODENOTIFICATION_ENABLED == STD_ON)
    /**>The configuration of the request port for the event swc mode notification. @range None */
    const BswM_RqstPortLcCfgType* swcModeNoti;
    /**>The configuration of the request port for the event swc mode notification. @range None */
    const BswM_ModeGroupType* swcModeNotisRef;
#endif /*BSWM_SWCMODENOTIFICATION_ENABLED == STD_ON*/

#if (BSWM_SWCMODEREQUEST_ENABLED == STD_ON)
    /**>The configuration of the request port for the event swc mode request. @range None */
    const BswM_RqstPortLcCfgType* swcModeRqst;
    /**>The configuration of the request port for the event swc mode request. @range None */
    const BswM_ModeGroupType* swcModeRqstsRef;
#endif /*BSWM_SWCMODEREQUEST_ENABLED == STD_ON*/

#else  /*ModeRequestPort configuration don't exist*/
    uint8 dummy; /**< defaul value for  the request port. @range None */
#endif /*BSWM_MODE_RQSTPORT_ENABLE == STD_ON*/
} BswM_ModeRqstPortLCfgType;

/**
 * @brief   type definition for actionlist configuration
 *
 */
typedef struct BswM_ActionListLcCfgTag
{
    /**> An actionlist to hold all configurations. @range None */
    BswM_ActionListCtrlFuncType actionListCtrlFct;
} BswM_ActionListLcCfgType;

/**
 * @brief type definition for link time configure of rules
 *
 */
typedef struct BswM_RuleLcCfgTag
{
    /*BswMRuleExpressionRef*/
    BswM_ActionListHandlerType (*modeArbitraFunc)(
        void); /**>The configuration of the actionlist to hold all configurations */
    /*BswMRuleInitState. @range None */
    BswM_RuleStateType ruleInitState; /**>The configuration of the rule init state. @range 0~2 */
    /*BswMNestedExecutionOnly*/
    boolean isNestExe; /**> true if execution of nested execution only. @range 0~1 */
} BswM_RuleLcCfgType;

/**
 * @brief type definition for run time of rules
 *
 */
typedef struct BswM_RuleRunTimeTag
{
    BswM_RuleStateType ruleStatus;        /**> old status for Rules. @range 0~2 */
    boolean            ruleNeedArbitrate; /**> TRUE, rule need arbitration. @range 0~1 */
} BswM_RuleRunTimeType;

/**
 * @brief type definition for link time configure of partition
 *
 */
typedef struct BswM_PartitionLCfgTag
{
#if (BSWM_EVENT_RQSTPORT_ENABLE == STD_ON) /*EventRequestPort configuration exists*/
    const BswM_EventRqstPortLCfgType*
        evRqstLCfg; /**>The configuration of the request port for the event. @range None */
#endif              /* BSWM_EVENT_RQSTPORT_ENABLE == STD_ON */
    const BswM_ModeRqstPortLCfgType*
                               modeRqstLCfg;   /**>The configuration of the request port for the mode. @range None */
    BswM_RuleIndexType         numOfRules;     /**>The configuration of the rule number. @range 0~0xFF */
    const BswM_RuleLcCfgType*  ruleLCfg;       /**>The configuration of the rule. @range None */
    BswM_RuleIndexType         numOfDefRules;  /**> The configuration of the number of default rules. @range 0~0xFF */
    const BswMRulePbIndexType* defRuleRefLCfg; /**>The configuration of the default rule. @range None */
    BswM_RuleRunTimeType*      ruleRunTimeStatus;    /**>The configuration of the rule run time status. @range None */
    const BswM_ActionListLcCfgType* actionListLCfg;  /**>The configuration of the actionlist. @range None */
    uint16                          numOfActionList; /**>The configuration of the number of actionlist. @range 0~0xFF */
#if (BSWM_BSW_MODE_DEF_NOTIFICATION == STD_ON)
    void (*bswModeNotificationFunc)(void); /**>The configuration of the bsw mode notification function. @range None */
#endif                                     /* BSWM_BSW_MODE_DEF_NOTIFICATION == STD_ON */
#if (BSWM_SWC_MODE_DEF_NOTIFICATION == STD_ON)
    void (*swcModeNotificationFunc)(void); /**>The configuration of the swc mode notification function. @range None */
#endif                                     /* BSWM_SWC_MODE_DEF_NOTIFICATION == STD_ON */
#if (BSWM_SWC_MODE_DEF_REQUEST == STD_ON)
    void (*bswModeRequestFunc)(void); /**>The configuration of the swc mode request function. @range None */
#endif                                /* BSWM_SWC_MODE_DEF_REQUEST == STD_ON */
} BswM_PartitionLCfgType;

#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
/**
 * @brief Manager. A pointer to this structure is passed to the BSW Mode Manager
 * initialization function for configuration.
 * This container exists once per partition.
 *
 */
typedef BswM_PbConfigType     BswM_ConfigType;
extern const BswM_ConfigType* BswM_ConfigPtr;
#else
typedef struct BswM_ConfigTag
{
    uint8 reserved; /**> dummy */
} BswM_ConfigType;
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

/**
 * @brief       Initializes the BSW Mode Manager.
 * @param[in]   ConfigPtr : Pointer to post-build configuration data.
 * @return      None
 * @reentrant   Conditionally
 * @synchronous TRUE
 * @trace       CPD-66209
 */
void BswM_Init(const BswM_ConfigType* ConfigPtr);

/**
 * @brief       Deinitializes the BSW Mode Manager.
 *              After a call of BswM_Deinit, no mode processing shall be performed by BswM
 *              even if any mode requests are made or the BswM main function is called.
 * @return      None
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-66218
 */
void BswM_Deinit(void);

#if (BSWM_VERSION_INFO_API == STD_ON)
/**
 * @brief       Returns the version information of this module.
 * @param[out]  VersionInfo : Pointer to where to store the version information of the module.
 * @return      None
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-66223
 */
void BswM_GetVersionInfo(Std_VersionInfoType* VersionInfo);
#endif /*BSWM_VERSION_INFO_API == STD_ON*/

#if (BSWM_MULTI_PARTITION_ENABLED == STD_ON)
/**
 * @brief       Function called by Restart Task if the partition containing the BswM has been restarted.
 * @return      None
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-66224
 */
void BswM_BswMPartitionRestarted(void);
#endif /*BSWM_MULTI_PARTITION_ENABLED == STD_ON*/

#if (BSWM_GENERIC_REQUEST_ENABLED == STD_ON)
/**
 * @brief       Generic function call to request modes. This function shall only be used by
 *              other BSW modules that do not have a specific mode request interface.
 * @param[in]   requesting_user : The user that requests the mode.
 * @param[in]   requested_mode : The requested mode.
 * @return      None
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-66220
 */
void BswM_RequestMode(BswM_UserType requesting_user, BswM_ModeType requested_mode);
#endif /*BSWM_GENERIC_REQUEST_ENABLED == STD_ON*/

#ifdef __cplusplus
}
#endif
#endif /* BSWM_H */
