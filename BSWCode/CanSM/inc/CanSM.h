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
 **  @file               : CanSM.h
 **  @author             : Wanglili
 **  @date               : 2020/08/17
 **  @vendor             : isoft
 **  @description        : CAN State Manage
 **
 ***********************************************************************************************************************/
/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author         Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V02.00.00 2020-07-23  lili.wang      1. Update code according AUTOSAR R19-11 Specification
 *                                       2. Modify code with the new coding rule
 *                                       3. Fix bugs find in 01.00.03

 *  V02.00.00 2020-08-17  lili.wang      R19_11 initial version.
 *  V02.00.01 2020-10-19  lili.wang      QAC check,code update
 *  V02.00.02 2021-02-02  lili.wang      1. Modify the timing mechanism for MainFunction timing.
 *                                       2. ComM_BusSM_ModeIndication pass parameter modification.
 *  V02.00.03 2021-02-23  lili.wang      1. Modification of retry mechanism after CanIf request fails
 *                                       2. not include FreeRTTimer.h any more
 *  V02.00.04 2021-03-23  lili.wang      Modify the macro control conditions of the Dem parameter,and some compilation
 warnings
 *  V02.00.05 2021-04-23  lili.wang      Delete CanSM_TxTimeoutException macro control CANSM_PNC_SUPPORT.
 *  V02.00.06 2021-06-16  lili.wang      In CanSM_FullCom_S_CC_Started_Wait,get T_CC_STARTED_INDICATED,call
 * CanIf_SetPduMode with controllerID and CANIF_ONLINE
 *  V02.00.07 2021-06-24  lili.wang      In CanSM_FullCom_S_CC_Started,get T_CC_STARTED_INDICATED,call
 * CanIf_SetPduMode with controllerID and CANIF_ONLINE
 *  V02.00.08 2022-03-17  darren         Adjust header files and interface macro switches
 *  V02.00.09 2022-04-24  darren.zhang   Adjust the header file inclusion order
 *  V02.00.10 2022-07-26  darren.zhang   modify memap
 *  V02.00.11 2023-01-05  darren.zhang   support pending comm reqeust
 *  V02.00.12 2023-07-24  xiaojian.liang 1. Fixed CPT-6229, CanSM_GetCurrentComMode cannot return
 COMM_SILENT_COMMUNICATION when bussoff occurs
 *                                       2. Fixed CPT-472, Transition with best effort instead of at most one transition
 *  V02.00.13 2023-08-11  xiaojian.liang 1. Fixed CPT-6413, The busoff event is lost in the S_TX_OFF state and can not
 be automatically recovered
 *                                       2. Fixed CPT-6482, ModeRequestRepetitionTime, BorTimeTxEnsured, BorTimeL2 and
 BorTimeL1 counts incorrectly
 *  V02.00.14 2023-11-29  xiaojian.liang 1. Performance optimization.
 *                                       2. Fixed CPT-7745, Unable to retry if CanIf_SetControllerMode return E_NOT_OK
 *  V02.00.15 2024-04-28  xiaojian.liang 1. CPT-8597  Runtime error CANSM_E_MODE_REQUEST_TIMEOUT depends on
 CanSMDevErrorDetect.
 *                                       2. CPT-8723  CanSM_DeInit still detect the CANSM_NO_COMMUNICATION error if
 CanSMDevErrorDetect is false.
 *                                       3. Removed Pretended Networking.
 *                                       4. Compilation error if CanIfPublicTxConfirmPollingSupport is set to true.
 *                                       5. Support multiple partition feature.
 *  V2.1.1  2025-04-18  xiaojian.liang   CPT-14007, CanSM code size reduction.
 *          2025-04-24  xiaojian.liang   CPT-14057, Effect E_FULL_COM change pdu mode to CANIF_ONLINE if ECU is already
 in passive mode.
 *
 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:PduR<br>
  RuleSorce:puhua_rules-2024.12.rcf
    \li PRQA S 3218 VL_CanSM_3218 .<br>
      Reason: file scoped variables only used in one function are necessary for functional usage
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li PRQA S 0770 VL_CanSM_0770 .<br>
      Reason: design needs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li PRQA S 5017 VL_CanSM_5017 .<br>
      Reason: High cyclomatic complexity is reserved for better readability.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_MTR_CanSM_STM19
      Reason: Multiple exit points can contribute to improved code performance in specific scenarios.
      Risk: Increases the difficulty of debugging the function.
      Prevention: Adequate testing ensures correct functionality.

    \li VL_MTR_CanSM_STMIF
      Reason: Functions have different scenarios that need to be covered, which depends on the configuration of
  different channels, sdu and other configuration conditions, as well as some precompiled macros
      Risk: The code is difficult to maintain.
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_MTR_CanSM_STCYC
      Reason: Because there are higher requirements for minimizing the stack and runtime usage of the applied code.
      Risk: Understandability and testability become overly complex
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_MTR_CanSM_STCAL
      Reason: The software structure of CanSM is defined according to the AUTOSAR standard,resulting in its metric
  being above the threshold
      Risk: Understandability and testability become overly complex
      Prevention: Design and code review, and have a clear structure and annotated code.
 */

#ifndef CANSM_H_
#define CANSM_H_

/* =================================================== inclusions =================================================== */
#include "CanSM_CanIf.h"
#include "CanSM_PBcfg.h"
#include "ComM.h"
#if (STD_ON == CANSM_DEM_SUPPORT)
#include "Dem.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */
/**
 * @name CANSM_VERSION_INFO_GROUP
 * @{
 * @brief Published Information
 */
#define CANSM_MODULE_ID                   (140u) /**< Module ID of CanSM module */
#define CANSM_VENDOR_ID                   (62u)  /**< Vendor ID of CanSM module */
#define CANSM_AR_RELEASE_MAJOR_VERSION    (4u)   /**< AUTOSAR release major version */
#define CANSM_AR_RELEASE_MINOR_VERSION    (9u)   /**< AUTOSAR release minor version */
#define CANSM_AR_RELEASE_REVISION_VERSION (0u)   /**< AUTOSAR release patch version */
#define CANSM_SW_MAJOR_VERSION            (2u)   /**< Software major version */
#define CANSM_SW_MINOR_VERSION            (1u)   /**< Software minor version */
#define CANSM_SW_PATCH_VERSION            (1u)   /**< Software patch version */
/** @} */
/* ===================================================== macros ===================================================== */
#define CANSM_INSTANCE_ID 0U /**< Instance ID of CanSM module */

/**
 * @name CANSM_DEVELOPMENT_ERRORS
 * @{
 * @brief Development error values
 */
#define CANSM_E_NO_ERROR                  0x00u /**< No error */
#define CANSM_E_UNINIT                    0x01u /**< API service used without module initialization */
#define CANSM_E_PARAM_POINTER             0x02u /**< API service called with wrong pointer */
#define CANSM_E_INVALID_NETWORK_HANDLE    0x03u /**< API service called with wrong parameter */
#define CANSM_E_PARAM_CONTROLLER          0x04u /**< API service called with wrong parameter */
#define CANSM_E_PARAM_TRANSCEIVER         0x05u /**< API service called with wrong parameter */
#define CANSM_E_INVALID_PARTITION_CONTEXT 0x06u /**< Invalid partition context */
#define CANSM_E_ALREADY_INITIALIZED       0x07u /**< Module already initialized */
#define CANSM_E_NOT_IN_NO_COM \
    0x0Bu /**< DeInit API service called when not all CAN networks are in state CANSM_NO_COMMUNICATION */
/** @} */

#define CANSM_E_MODE_REQUEST_TIMEOUT \
    0x0Au /**< Mode request for a network failed more often than allowed by configuration */

/**
 * @name CANSM_SERVICE_IDS
 * @{
 * @brief Service IDs
 */
#define SERVICE_ID_CANSM_INIT                       0x00u /**< Service ID: CanSM_Init */
#define SERVICE_ID_CANSM_DEINIT                     0x14u /**< Service ID: CanSM_DeInit */
#define SERVICE_ID_CANSM_REQUESTCOMMODE             0x02u /**< Service ID: CanSM_RequestComMode */
#define SERVICE_ID_CANSM_GETCURRENTCOMMODE          0x03u /**< Service ID: CanSM_GetCurrentComMode */
#define SERVICE_ID_CANSM_STARTWAKEUPSOURCE          0x11u /**< Service ID: CanSM_StartWakeupSource */
#define SERVICE_ID_CANSM_STOPWAKEUPSOURCE           0x12u /**< Service ID: CanSM_StopWakeupSource */
#define SERVICE_ID_CANSM_GETVERSIONINFO             0x01u /**< Service ID: CanSM_GetVersionInfo */
#define SERVICE_ID_CANSM_SETBAUDRATE                0x0du /**< Service ID: CanSM_SetBaudrate */
#define SERVICE_ID_CANSM_SETICOMCONFIGURATION       0x0fu /**< Service ID: CanSM_SetIcomConfiguration */
#define SERVICE_ID_CANSM_SETECUPASSIVE              0x13u /**< Service ID: CanSM_SetEcuPassive */
#define SERVICE_ID_CANSM_CONTROLLERBUSOFF           0x04u /**< Service ID: CanSM_ControllerBusOff */
#define SERVICE_ID_CANSM_CONTROLLERMODEINDICATION   0x07u /**< Service ID: CanSM_ControllerModeIndication */
#define SERVICE_ID_CANSM_TRANCEIVERMODEINDICATION   0x09u /**< Service ID: CanSM_TransceiverModeIndication */
#define SERVICE_ID_CANSM_TXTIMEOUTEXCEPTION         0x0bu /**< Service ID: CanSM_TxTimeoutException */
#define SERVICE_ID_CANSM_CLEARTRCVWUFFLAGINDICATION 0x08u /**< Service ID: CanSM_ClearTrcvWufFlagIndication */
#define SERVICE_ID_CANSM_CHECKTRANSCEIVERWAKEFLAGINDICATION \
    0x0au                                               /**< Service ID: CanSM_CheckTransceiverWakeFlagIndication */
#define SERVICE_ID_CANSM_CONFIRMPNAVAILABILITY    0x06u /**< Service ID: CanSM_ConfirmPnAvailability */
#define SERVICE_ID_CANSM_CURRENTICOMCONFIGURATION 0x10u /**< Service ID: CanSM_CurrentIcomConfiguration */
#define SERVICE_ID_CANSM_MAINFUNCTION             0x05u /**< Service ID: CanSM_MainFunction */
/** @} */
/* ================================================ type definitions ================================================ */
/**
 * @brief Defines the values of the internal states of the CanSM module
 */
typedef enum CanSM_ModuleStatusTypeTag
{
    CANSM_UNINITED, /**< Module is not initialized @range 0 */
    CANSM_INITED    /**< Module is initialized @range 1 */
} CanSM_ModuleStatusType;

/**
 * @brief Defines the values of the major state machine
 */
typedef enum CanSM_BSM_StateTypeTag
{
    CANSM_BSM_DEINITPNNOTSUPPORTED, /**< CANSM_BSW - CANSM_BSM_S_PRE_NOCOM @range 0 */
#if (STD_ON == CANSM_PNC_SUPPORT)
    CANSM_BSM_DEINITPNSUPPORTED, /**< CANSM_BSW - CANSM_BSM_S_PRE_NOCOM @range 0 or 1 */
#endif
    CANSM_BSM_S_NOCOM,       /**< CANSM_BSW - CANSM_BSM_S_NOCOM @range 1 or 2 */
    CANSM_BSM_WUVALIDATION,  /**< CANSM_BSW - CANSM_BSM_WUVALIDATION @range 2 or 3 */
    CANSM_BSM_S_PRE_FULLCOM, /**< CANSM_BSW - CANSM_BSM_S_PRE_FULLCOM @range 3 or 4 */
    CANSM_BSM_S_FULLCOM,     /**< CANSM_BSW - CANSM_BSM_S_FULLCOM @range 4 or 5 */
#if (STD_ON == CANSM_SET_BAUDRATE_API)
    CANSM_BSM_S_CHANGE_BAUDRATE, /**< CANSM_BSW - CANSM_BSM_S_CHANGE_BAUDRATE @range 5 or 6 */
#endif
    CANSM_BSM_S_SILENTCOM,    /**< CANSM_BSW - CANSM_BSM_S_SILENTCOM @range 5 or 6 or 7 */
    CANSM_BSM_S_SILENTCOM_BOR /**< CANSM_BSW - CANSM_BSM_S_SILENTCOM_BOR @range 6 or 7 or 8 */
} CanSM_BSM_StateType;

#if (STD_ON == CANSM_PNC_SUPPORT)
/**
 * @brief Defines the values of the Sub state machine CANSM_BSM_DeinitPnSupported
 */
typedef enum CanSM_DeinitPnSupportedStateTypeTag
{
    DEINITPN_S_PN_CLEAR_WUF,                /**< CANSM_BSM_DeinitPnSupported - S_PN_CLEAR_WUF @range 0 */
    DEINITPN_S_PN_CLEAR_WUF_WAIT,           /**< CANSM_BSM_DeinitPnSupported - S_PN_CLEAR_WUF_WAIT @range 1 */
    DEINITPN_S_PN_CC_STOPPED,               /**< CANSM_BSM_DeinitPnSupported - S_CC_STOPPED @range 2 */
    DEINITPN_S_CC_STOPPED_WAIT,             /**< CANSM_BSM_DeinitPnSupported - S_CC_STOPPED_WAIT @range 3 */
    DEINITPN_S_TRCV_NORMAL,                 /**< CANSM_BSM_DeinitPnSupported - S_TRCV_NORMAL @range 4 */
    DEINITPN_S_TRCV_NORMAL_WAIT,            /**< CANSM_BSM_DeinitPnSupported - S_TRCV_NORMAL_WAIT @range 5 */
    DEINITPN_S_TRCV_STANDBY,                /**< CANSM_BSM_DeinitPnSupported - S_TRCV_STANDBY @range 6 */
    DEINITPN_S_TRCV_STANDBY_WAIT,           /**< CANSM_BSM_DeinitPnSupported - S_TRCV_STANDBY_WAIT @range 7 */
    DEINITPN_S_CC_SLEEP,                    /**< CANSM_BSM_DeinitPnSupported - S_CC_SLEEP @range 8 */
    DEINITPN_S_CC_SLEEP_WAIT,               /**< CANSM_BSM_DeinitPnSupported - S_CC_SLEEP_WAIT @range 9 */
    DEINITPN_S_CHECK_WFLAG_IN_CC_SLEEP,     /**< CANSM_BSM_DeinitPnSupported - S_CHECK_WFLAG_IN_CC_SLEEP @range 10 */
    DEINITPN_S_CHECK_WUF_IN_CC_SLEEP_WAIT,  /**< CANSM_BSM_DeinitPnSupported - S_CHECK_WUF_IN_CC_SLEEP_WAIT @range 11 */
    DEINITPN_S_CHECK_WFLAG_IN_NOT_CC_SLEEP, /**< CANSM_BSM_DeinitPnSupported - S_CHECK_WFLAG_IN_NOT_CC_SLEEP @range 12
                                             */
    DEINITPN_S_CHECK_WUF_IN_NOT_CC_SLEEP_WAIT /**< CANSM_BSM_DeinitPnSupported - S_CHECK_WUF_IN_NOT_CC_SLEEP_WAIT @range
                                                 13 */
} CanSM_DeinitPnSupportedStateType;
#endif

/**
 * @brief Defines the values of the Sub state machine CANSM_BSM_DeinitPnNotSupported
 */
typedef enum CanSM_DeinitPnNotSupportedStateTypetag
{
    DEINITPNNOT_S_CC_STOPPED,       /**< CANSM_BSM_DeinitPnNotSupported - S_CC_STOPPED @range 0 */
    DEINITPNNOT_S_CC_STOPPED_WAIT,  /**< CANSM_BSM_DeinitPnNotSupported - S_CC_STOPPED_WAIT @range 1 */
    DEINITPNNOT_S_CC_SLEEP,         /**< CANSM_BSM_DeinitPnNotSupported - S_CC_SLEEP @range 2 */
    DEINITPNNOT_S_CC_SLEEP_WAIT,    /**< CANSM_BSM_DeinitPnNotSupported - S_CC_SLEEP_WAIT @range 3 */
    DEINITPNNOT_S_TRCV_NORMAL,      /**< CANSM_BSM_DeinitPnNotSupported - S_TRCV_NORMAL @range 4 */
    DEINITPNNOT_S_TRCV_NORMAL_WAIT, /**< CANSM_BSM_DeinitPnNotSupported - S_TRCV_NORMAL_WAIT @range 5 */
    DEINITPNNOT_S_TRCV_STANDBY,     /**< CANSM_BSM_DeinitPnNotSupported - S_TRCV_STANDBY @range 6 */
    DEINITPNNOT_S_TRCV_STANDBY_WAIT /**< CANSM_BSM_DeinitPnNotSupported - S_TRCV_STANDBY_WAIT @range 7 */
} CanSM_DeinitPnNotSupportedStateType;

/**
 * @brief Defines the values of the Sub state machine CANSM_BSM_WUVALIDATION
 */
typedef enum CanSM_WUValidationStateTypeTag
{
    WUVALIDATION_S_TRCV_NORMAL,          /**< CANSM_BSM_WUVALIDATION - S_TRCV_NORMAL @range 0 */
    WUVALIDATION_S_TRCV_NORMAL_WAIT,     /**< CANSM_BSM_WUVALIDATION - S_TRCV_NORMAL_WAIT @range 1 */
    WUVALIDATION_S_CC_STOPPED,           /**< CANSM_BSM_WUVALIDATION - S_CC_STOPPED @range 2 */
    WUVALIDATION_S_CC_STOPPED_WAIT,      /**< CANSM_BSM_WUVALIDATION - S_CC_STOPPED_WAIT @range 3 */
    WUVALIDATION_S_CC_STARTED,           /**< CANSM_BSM_WUVALIDATION - S_CC_STARTED @range 4 */
    WUVALIDATION_S_CC_STARTED_WAIT,      /**< CANSM_BSM_WUVALIDATION - S_CC_STARTED_WAIT @range 5 */
    WUVALIDATION_WAIT_WUVALIDATION_LEAVE /**< CANSM_BSM_WUVALIDATION - WAIT_WUVALIDATION_LEAVE @range 6 */
} CanSM_WUValidationStateType;

/**
 * @brief Defines the values of the Sub state machine CANSM_BSM_S_PRE_FULLCOM
 */
typedef enum CanSM_PreFullComStateTypeTag
{
    PREFULLCOM_S_TRCV_NORMAL,      /**< CANSM_BSM_S_PRE_FULLCOM - S_TRCV_NORMAL @range 0 */
    PREFULLCOM_S_TRCV_NORMAL_WAIT, /**< CANSM_BSM_S_PRE_FULLCOM - S_TRCV_NORMAL_WAIT @range 1 */
    PREFULLCOM_S_CC_STOPPED,       /**< CANSM_BSM_S_PRE_FULLCOM - S_CC_STOPPED @range 2 */
    PREFULLCOM_S_CC_STOPPED_WAIT,  /**< CANSM_BSM_S_PRE_FULLCOM - S_CC_STOPPED_WAIT @range 3 */
    PREFULLCOM_S_CC_STARTED,       /**< CANSM_BSM_S_PRE_FULLCOM - S_CC_STARTED @range 4 */
    PREFULLCOM_S_CC_STARTED_WAIT   /**< CANSM_BSM_S_PRE_FULLCOM - S_CC_STARTED_WAIT @range 5 */
} CanSM_PreFullComStateType;

/**
 * @brief Defines the values of the Sub state machine CANSM_BSM_S_FULLCOM
 */
typedef enum CanSM_FullComStateTypeTag
{
    FULLCOM_S_BUS_OFF_CHECK,   /**< CANSM_BSM_S_FULLCOM - S_BUS_OFF_CHECK @range 0 */
    FULLCOM_S_NO_BUS_OFF,      /**< CANSM_BSM_S_FULLCOM - S_NO_BUS_OFF @range 1 */
    FULLCOM_S_CC_STOPPED,      /**< CANSM_BSM_S_FULLCOM - S_CC_STOPPED @range 2 */
    FULLCOM_S_CC_STOPPED_WAIT, /**< CANSM_BSM_S_FULLCOM - S_CC_STOPPED_WAIT @range 3 */
    FULLCOM_S_CC_STARTED,      /**< CANSM_BSM_S_FULLCOM - S_CC_STARTED @range 4 */
    FULLCOM_S_CC_STARTED_WAIT, /**< CANSM_BSM_S_FULLCOM - S_CC_STARTED_WAIT @range 5 */
    FULLCOM_S_RESTART_CC,      /**< CANSM_BSM_S_FULLCOM - S_RESTART_CC @range 6 */
    FULLCOM_S_RESTART_CC_WAIT, /**< CANSM_BSM_S_FULLCOM - S_RESTART_CC_WAIT @range 7 */
    FULLCOM_S_TX_OFF           /**< CANSM_BSM_S_FULLCOM - S_TX_OFF @range 8 */
} CanSM_FullComStateType;

/**
 * @brief Defines the values of the Sub state machine CANSM_BSM_S_CHANGE_BAUDRATE
 */
typedef enum CanSM_ChangeBaudrateStateTypeTag
{
    CHANGEBR_CHANGE_BR_SYNC,    /**< CANSM_BSM_S_CHANGE_BAUDRATE - CANSM_BSM_CHANGE_BR_SYNC @range 0 */
    CHANGEBR_S_CC_STOPPED,      /**< CANSM_BSM_S_CHANGE_BAUDRATE - S_CC_STOPPED @range 1 */
    CHANGEBR_S_CC_STOPPED_WAIT, /**< CANSM_BSM_S_CHANGE_BAUDRATE - S_CC_STOPPED_WAIT @range 2 */
    CHANGEBR_S_CC_STARTED,      /**< CANSM_BSM_S_CHANGE_BAUDRATE - S_CC_STARTED @range 3 */
    CHANGEBR_S_CC_STARTED_WAIT  /**< CANSM_BSM_S_CHANGE_BAUDRATE - S_CC_STARTED_WAIT @range 4 */
} CanSM_ChangeBaudrateStateType;

/**
 * @brief Defines the values of the Sub state machine CANSM_BSM_S_SILENTCOM_BOR
 */
typedef enum CanSM_SilentComBORStateTypeTag
{
    SILENTBOR_S_RESTART_CC,     /**< CANSM_BSM_S_SILENTCOM_BOR - S_RESTART_CC @range 0 */
    SILENTBOR_S_RESTART_CC_WAIT /**< CANSM_BSM_S_SILENTCOM_BOR - S_RESTART_CC_WAIT @range 1 */
} CanSM_SilentComBORStateType;

/**
 * @brief Defines the values of the request communication mode
 */
typedef enum CanSM_RequestModeTypeTag
{
    CANSM_NO_REQUEST,           /**< No request for request mode @range 0 */
    CANSM_NO_COMMUNICATION,     /**< No communication for request mode @range 1 */
    CANSM_SILENT_COMMUNICATION, /**< Silent communication for request mode @range 2 */
    CANSM_FULL_COMMUNICATION    /**< Full communication for request mode @range 3 */
} CanSM_RequestModeType;

/**
 * @brief Defines the values of CanIf API indication type
 */
typedef enum CanSM_CanIfIndicatedTypeTag
{
    T_NO_INDICATED, /**< No CanIf API indicated @range 0 */
#if (STD_ON == CANSM_PNC_SUPPORT)
    T_CLEAR_WUF_INDICATED,   /**< CanIf API indicated T_CLEAR_WUF_INDICATED @range 1 */
    T_CHECK_WFLAG_INDICATED, /**< CanIf API indicated T_CHECK_WFLAG_INDICATED @range 2 */
#endif
    T_TRCV_NORMAL_INDICATED,  /**< CanIf API indicated T_TRCV_NORMAL_INDICATED @range 1 or 3 */
    T_TRCV_STANDBY_INDICATED, /**< CanIf API indicated T_TRCV_STANDBY_INDICATED @range 2 or 4 */
} CanSM_CanIfIndicatedType;

/**
 * @brief Can specific communication modes / states notified to the BswM module
 */
typedef enum CanSM_BswMCurrentStateTypeTag
{
    CANSM_BSWM_NO_COMMUNICATION,     /**< No communication for notified to thr BswM module @range 0 */
    CANSM_BSWM_SILENT_COMMUNICATION, /**< Silent communication for notified to thr BswM module @range 1 */
    CANSM_BSWM_FULL_COMMUNICATION,   /**< Full communication for notified to thr BswM module @range 2 */
    CANSM_BSWM_BUS_OFF,              /**< Bus off for notified to thr BswM module @range 3 */
    CANSM_BSWM_CHANGE_BAUDRATE       /**< Change baudrate for notified to thr BswM module @range 4 */
} CanSM_BswMCurrentStateType;

/**
 * @brief The range is at least equal to the range of type NetworkHandleType
 */
typedef uint8 CanSM_NetworkIndexType;

/**
 * @brief Transceiver parameters of each CAN network
 */
typedef struct CanSM_TrcvRefTypeTag
{
    uint8   TransceiverId;    /**< ID of the CAN transceiver assigned to the configured network handle @range 0..255 */
    boolean CanTrcvPnEnabled; /**<the transceiver support PN or not @range true/false */
} CanSM_TrcvRefType;

/**
 * @brief CAN controller ID parameters of each CAN network
 */
typedef struct CanSM_ControllerRefTypeTag
{
    uint8        NetWorkRefControllerNumber; /**< Number of controllers which reference the network @range 0..255 */
    const uint8* ControllerId; /**< ID of the CAN controller assigned to the configured network handle @range 0..255 */
    Can_ControllerStateType*
        RuntimeControlModePtr; /**< Pointer to the runtime control mode of the controller @range pointer */
} CanSM_ControllerRefType;

#if (STD_ON == CANSM_DEM_SUPPORT)
/**
 * @brief Reference to configured DEM event to report dem errors for this CAN network
 */
typedef struct CanSM_DemEventParameterRefTypeTag
{
    const Dem_EventIdType*
        BusOffPara; /**< Reference to configured DEM event,CANSM_E_BUS_OFF parameter @range pointer */
    const Dem_EventIdType* ModeReqTimeoutPara; /**<Reference to configured DEM event,CANSM_E_MODE_REQUEST_TIMEOUT
                                                  parameter @range pointer */
} CanSM_DemEventParameterRefType;
#endif

/**
 * @brief type of CanSM module's runtime state
 */
typedef struct CanSM_NetWorkRunTimeTypeTag
{
    uint32 ModeRequestRepetitionTime;   /**< time for repeat CanIf API @range 0..4294967295 */
    uint32 WaitCanIfIndicatedStartTime; /**< timer for CanIf API indicated state machine @range 0..4294967295 */
    uint32 BusOffEventStartTime;        /**< timer for bus-off event @range 0..4294967295 */
#if CANSM_BOR_TIME_TX_ENSURED_ENABLED == STD_ON
    uint32 BorTimeTxEnsuredTime; /**< timer for G_BUS_OFF_PASSIVE state machine @range 0..4294967295 */
#endif
    uint8 BusOffCounter; /**< counter for bus-off recovery state machine @range 0..255 */
    uint8 RepeatCounter; /**< counter for repeat CanIf API @range 0.255 */
    /** Set in CanSM_ControllerBusOff and clear before calling function CanIf_SetControllerMode @range true/false */
    boolean                  BusOffEvent;
    CanSM_CanIfIndicatedType CanIfIndicated; /**< CanIf API indication type @range CanSM_CanIfIndicatedType */
    CanSM_RequestModeType    RequestComMode; /**< request communication mode of net work @range CanSM_RequestModeType */
    ComM_ModeType            CurComMode;     /**< current communication mode of net work @range ComM_ModeType */
    CanSM_BSM_StateType      CurBsmState;    /**< NetWork current BSM state @range CanSM_BSM_StateType */
#if (STD_ON == CANSM_SET_BAUDRATE_API)
    uint16
        BaudRateConfigID; /**< store the BaudRateConfigID when CanSM_SetBaudrate is called successful @range 0..65535 */
    CanSM_ChangeBaudrateStateType
        ChangeBaudrateState; /**< sub BSM state(CANSM_BSM_S_CHANGE_BAUDRATE) @range CanSM_ChangeBaudrateStateType */
#endif
#if (STD_ON == CANSM_GET_BUSOFF_DELAY_FUNCTION_USED)
    uint8 BusOffDelayTime; /**< wait additionally to L1/L2 after a BusOff occurred @range 0..255 */
#endif
#if (STD_ON == CANSM_PNC_SUPPORT)
    /** sub BSM state(CANSM_BSM_S_PRE_NOCOM) if support PN @range CanSM_DeinitPnSupportedStateType */
    CanSM_DeinitPnSupportedStateType DeinitPnSupportedState;
#endif
    /** sub BSM state(CANSM_BSM_S_PRE_NOCOM) if not support PN @range CanSM_DeinitPnNotSupportedStateType*/
    CanSM_DeinitPnNotSupportedStateType DeinitPnNotSupportedState;
    CanSM_WUValidationStateType
        WuValidationState; /**< sub BSM state(CANSM_BSM_WUVALIDATION) @range CanSM_WUValidationStateType */
    CanSM_PreFullComStateType
        PreFullComState;                 /**< sub BSM state(CANSM_BSM_S_PRE_FULLCOM) @range CanSM_PreFullComStateType */
    CanSM_FullComStateType FullComState; /**< sub BSM state(CANSM_BSM_S_FULLCOM) @range CanSM_FullComStateType */
    CanSM_SilentComBORStateType
        SilentComBORState; /**< sub BSM state(CANSM_BSM_S_SILENTCOM_BOR) @range CanSM_SilentComBORStateType */
} CanSM_NetWorkRunTimeType;

/**
 * @brief This type contains the CAN network specific parameters of each CAN network
 */
typedef struct CanSM_ManagerNetworkTypeTag
{
#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
    ApplicationType ApplicationID; /**< Application Id @range ApplicationType */
#endif
    /** The threshold count of bus-offs until the bus-off recovery switches from level 1 to level 2 @range 0..255*/
    uint8 BorCounterL1ToL2;
    /** This time parameter defines the duration of the bus-off recovery time in level 1 @range 0..65535 */
    uint16 BorTimeL1;
    /** Time parameter defines in ms the duration of the bus-off recovery time in level 2 @range 0..65535 */
    uint16 BorTimeL2;
#if CANSM_BOR_TIME_TX_ENSURED_ENABLED == STD_ON
    /** This parameter defines in ms the duration of the bus-off event check @range 0..65535 */
    uint16 BorTimeTxEnsured;
#if CANSM_BOR_TX_CONFIRMATION_POLLING_ENABLED == STD_ON
    /** Polls the CanIf_GetTxConfirmationState API to decide the bus-off state to be recovered @range true/false */
    boolean BorTxConfirmationPolling;
#endif
#endif
    /** Parameter defines if the <User_GetBusOffDelay> shall be called for this network @range true/false */
    boolean EnableBusOffDelay;
    NetworkHandleType
        ComMNetworkHandleRef; /**< Unique handle to identify one certain CAN network @range NetworkHandleType */
    const CanSM_TrcvRefType*
        TrcvRef; /**< Reference to one of the transceivers managed by the CanIf module @range pointer */
    const CanSM_ControllerRefType*
        ControllerRef; /**< Reference to one of the CAN controllers managed by the CanIf module @range pointer */
#if (STD_ON == CANSM_DEM_SUPPORT)
    const CanSM_DemEventParameterRefType*
        DemEventParameterRefs; /**< References to DemEventParameter elements @range pointer */
#endif
#if CANSM_TX_OFFLINE_ACTIVE_SUPPORT == STD_ON
    void (*SetNetworkPassive)(const uint8* controllerIds, uint8 networkControllerNumber);
#endif
    CanSM_NetWorkRunTimeType* RuntimeNetPtr; /**< Pointer to the runtime state of the network @range pointer */
} CanSM_ManagerNetworkType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief       This service initializes the CanSM module
 * @param[in]   ConfigPtr   : Pointer to init structure for the post build parameters of the CanSM
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70675
 */
void CanSM_Init(const CanSM_ConfigType* ConfigPtr);

/**
 * @brief       This service de-initializes the CanSM module.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70677
 */
void CanSM_DeInit(void);

/**
 * @brief       This service shall change the communication mode of a CAN network to the requested one.
 * @param[in]   network   : Handle of destined communication network for request
 * @param[in]   ComM_Mode : Requested communication mode
 * @return      Std_ReturnType
 * @retval      E_OK    : Service accepted
 * @retval      E_NOT_OK: Service denied
 * @reentrant   TRUE (only for different network handles)
 * @synchronous FALSE
 * @trace       CPD-70679
 */
Std_ReturnType CanSM_RequestComMode(NetworkHandleType network, ComM_ModeType ComM_Mode);

/**
 * @brief       This service shall put out the current communication mode of a CAN network
 * @param[in]   network   : Network handle, whose current communication mode shall be put out
 * @param[out]   ComM_ModePtr : Pointer, where to put out the current communication mode
 * @return      Std_ReturnType
 * @retval      E_OK    : Service accepted
 * @retval      E_NOT_OK: Service denied
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70680
 */
Std_ReturnType CanSM_GetCurrentComMode(NetworkHandleType network, ComM_ModeType* ComM_ModePtr);

#if (STD_ON == CANSM_VERSION_INFO_API)
/**
 * @brief       This service puts out the version information of this module (module ID, vendor ID, vendor specific
 * version numbers related to BSW00407)
 * @param[out]   VersionInfo   : Pointer to where to store the version information of this module.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70683
 */
void CanSM_GetVersionInfo(Std_VersionInfoType* VersionInfo);
#endif

#if (STD_ON == CANSM_SET_BAUDRATE_API)
/**
 * @brief       This service shall start an asynchronous process to change the baud rate for the configured
 * CAN controllers of a certain CAN network. Depending on necessary baud rate modifications the controllers might have
 * to reset.
 * @param[in]   Network   : Handle of the addressed CAN network for the baud rate change
 * @param[in]   BaudRateConfigID : references a baud rate configuration by ID (see CanController BaudRateConfigID)
 * @return      Std_ReturnType
 * @retval      E_OK    : Controller mode request has been accepted
 * @retval      E_NOT_OK: Controller mode request has not been accepted
 * @reentrant   FALSE
 * @synchronous TRUE (only for different Networks)
 * @trace       CPD-70684
 */
Std_ReturnType CanSM_SetBaudrate(NetworkHandleType Network, uint16 BaudRateConfigID);
#endif /*STD_ON == CANSM_SET_BAUDRATE_API*/

#if CANSM_TX_OFFLINE_ACTIVE_SUPPORT == STD_ON
/**
 * @brief       This function can be used to set all CanSM channels of the ECU to a receive only mode.
 * @param[in]   CanSM_Passive     TRUE  set all CanSM channels to passive, i.e. receive only
 *                                FALSE set all CanSM channels back to non-passive
 * @return      Std_ReturnType
 * @retval      E_OK    : Request accepted
 * @retval      E_NOT_OK: Request denied
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70685
 */
Std_ReturnType CanSM_SetEcuPassive(boolean CanSM_Passive);
#endif

/**
 * @brief       Scheduled function of the CanSM
 * @param[in]   netIndex   : Affected CAN network
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70694
 */
void CanSM_MainFunction(CanSM_NetworkIndexType netIndex);

/**
 * @brief       This function shall be called by EcuM when a wakeup source shall be started.
 * @param[in]   network   : Affected CAN network
 * @return      Std_ReturnType
 * @retval      E_OK    : Request accepted
 * @retval      E_NOT_OK: Request denied
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70681
 */
Std_ReturnType CanSM_StartWakeupSource(NetworkHandleType network);

/**
 * @brief       This function shall be called by EcuM when a wakeup source shall be stopped
 * @param[in]   network   : Affected CAN network
 * @return      Std_ReturnType
 * @retval      E_OK    : Request accepted
 * @retval      E_NOT_OK: Request denied
 * @reentrant   FALSE
 * @synchronous FALSE
 * @trace       CPD-70682
 */
Std_ReturnType CanSM_StopWakeupSource(NetworkHandleType network);

#ifdef __cplusplus
}
#endif
#endif
