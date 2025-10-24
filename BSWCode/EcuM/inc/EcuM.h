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
 **  @file               : EcuM.h
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : ECU state management
 **
 ***********************************************************************************************************************/
/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author       Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V02.02.00 2024-08-30  Jian.Jiang     1. Update code according AUTOSAR R23-11 Specification
 *  V02.02.01 2025-01-09  Jian.Jiang     1. Specification of QAC Notes and Explanation of Reasons
 *  V02.02.02 2025-01-14  Jian.Jiang     1. Adjusting the wake-up source to be used via PB
 *  V02.02.03 2025-02-19  Jian.Jiang     1. Specification of QAC Notes and Explanation of Reasons
 *
 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:EcuM<br>
  RuleSorce:puhua_rules-2024.12.rcf

    \li VL_EcuM_4604
      Reason: A reserved identifier or reserved macro name shall not be declared.
      Risk: No impact on integrated functions.
      Prevention: Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_EcuM_3432
      Reason: Function-like macros are used to allow more efficient code.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_EcuM_2743
      Reason: The macro defines format requirements.
      Risk: Encapsulate the det error interface and invoke the error interface only once, no risk.
      Prevention: Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_EcuM_2740
      Reason:Necessary logical design for while(1) and have "break" to stop.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_EcuM_3469
      Reason: A function should be used in preference to a function-like macro where they are interchangeable.
      Risk: No impact on integrated functions.
      Prevention: Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_EcuM_1840
      Reason: Weaknesses in Software Written in C.
      Risk: No impact on integrated functions.
      Prevention: Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_EcuM_0314
      Reason: void is specified in the autosar prototype interface requirements.
      Risk: In the subsequent use, operations are performed according to the specific type, no risk.
      Prevention:Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_EcuM_3218
      Reason: file scoped variables only used in one function are necessary for functional usage
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_EcuM_2991
      Reason: The control statement has no effect, but is reserved for better readability.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_EcuM_2995
      Reason: The control statement has no effect, but is reserved for better readability.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_EcuM_waitCycle
      Reason: Operator other than & (address-of) or = (assignment) applied to a volatile object.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_EcuM_FlexibleArray
      Reason:Misidentify flexible array.
      Risk: None.
      Prevention:None.

    \li VL_EcuM_3472
      Reason: Macro definitions for easy reuse.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_EcuM_3408
      Reason:Configuration parameter, design needs.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_EcuM_ComputeInvalidPtr
      Reason: Use [] on pointer.
      Risk:Pointer is NULL_PTR.
      Prevention:The pointer corresponds to the configuration one by one and is not empty.

    \li VL_EcuM_DerefInvalidPtr
      Reason: Dereference pointer.
      Risk:Pointer is NULL_PTR.
      Prevention:The pointer corresponds to the configuration one by one and is not empty.

    \li VL_EcuM_4603
      Reason: Dereference pointer.
      Risk:Pointer is NULL_PTR.
      Prevention:The pointer corresponds to the configuration one by one and is not empty.

    \li VL_EcuM_1505
      Reason: Functions defined out are not used in the project and have no impact on the integration application.
      Risk: No impact on integrated functions.
      Prevention: Provided to the application to use, the application use need to use the corresponding interface.

    \li VL_EcuM_0771
      Reason:Reduce cyclomatic complexity.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_EcuM_0553
      Reason: The usage of this file depends on the config
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_EcuM_2877
      Reason: The usage of this file depends on the config
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_EcuM_3332
      Reason:The corresponding macro definition has been obtained through the header file inclusion.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_EcuM_5016
      Reason: Parameter passing requires
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_EcuM_4397
      Reason: Macro Conveniently determining authenticity.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_EcuM_STMIF
      Reason: Functions handle specific tasks in the behavior of the entire component.
    Task has different scenarios that need to be covered, which depends on local conditions - this
    leads to deep nesting of the control structure.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_EcuM_STCAL
      Reason: The standard compliance vote was higher than the measurement threshold. In addition, a typical way to
    reduce STCAL is to deepen function nesting, which increases call stack usage and runtime
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_EcuM_STECT
      Reason: The total number of variables is related to the user configuration, allowing dynamic
    code to violate this under extreme conditions.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_EcuM_1253
      Reason: Converted to different integer type.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

 */
#ifndef ECUM_H_
#define ECUM_H_

/* =================================================== inclusions =================================================== */
#include "EcuM_PBcfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */
/** Vendor ID */
#define ECUM_VENDOR_ID 62u
/** Module ID */
#define ECUM_MODULE_ID 10u
/** Instance ID */
#define ECUM_INSTANCE_ID 0u

/** AUTOSAR Release Major Version */
#define ECUM_AR_RELEASE_MAJOR_VERSION 4u
/** AUTOSAR Release Minor Version */
#define ECUM_AR_RELEASE_MINOR_VERSION 9u
/** AUTOSAR Release Patch Version */
#define ECUM_AR_RELEASE_PATCH_VERSION 0u

/** Software Major Version */
#define ECUM_SW_MAJOR_VERSION 2u
/** Software Minor Version */
#define ECUM_SW_MINOR_VERSION 2u
/** Software Patch Version */
#define ECUM_SW_PATCH_VERSION 3u

/* ===================================================== macros ===================================================== */
/** Service ID for GetVersion */
#define ECUM_SID_GETVERSION 0x00u

/*Initialization and Shutdown Sequences*/
/** Service ID for GoDownHaltPoll */
#define ECUM_SID_GODOWNHALTPOLL 0x2cu
/** Service ID for Init */
#define ECUM_SID_INIT 0x01u
/** Service ID for StartupTwo */
#define ECUM_SID_STARTUPTWO 0x1au
/** Service ID for Shutdown */
#define ECUM_SID_SHUTDOWN 0x02u

/* State Management */
/** Service ID for SetState */
#define ECUM_SID_SETSTATE 0x2bu
/** Service ID for RequestRun */
#define ECUM_SID_REQUESTRUN 0x03u
/** Service ID for ReleaseRun */
#define ECUM_SID_RELEASERUN 0x04u
/** Service ID for RequestPostRun */
#define ECUM_SID_REQUESTPOST_RUN 0x0au
/** Service ID for ReleasePostRun */
#define ECUM_SID_RELEASEPOST_RUN 0x0bu

/* Shutdown Management */
/** Service ID for SelectShutdownTarget */
#define ECUM_SID_SELECTSHUTDOWNTARGET 0x06u
/** Service ID for GetShutdownTarget */
#define ECUM_SID_GETSHUTDOWNTARGET 0x09u
/** Service ID for GetLastShutdownTarget */
#define ECUM_SID_GETLASTSHUTDOWNTARGET 0x08u
/** Service ID for SelectShutdownCause */
#define ECUM_SID_SELECTSHUTDOWNCAUSE 0x1bu
/** Service ID for GetShutdownCause */
#define ECUM_SID_GETSHUTDOWNCAUSE 0x1cu

/* Wakeup Handling */
/** Service ID for GetPendingWakeupEvent */
#define ECUM_SID_GETPENDINGWAKEUPEVENT 0x0du
/** Service ID for ClearWakeupEvent */
#define ECUM_SID_CLEARWAKEUPEVENT 0x16u
/** Service ID for GetValidatedWakeupEvent */
#define ECUM_SID_GETVALIDATEDWAKEUPEVENT 0x15u
/** Service ID for GetExpiredWakeupEvent */
#define ECUM_SID_GETEXPIREDWAKEUPEVENT 0x19u

/* Alarm Clock */
/** Service ID for SetRelWakeupAlarm */
#define ECUM_SID_SETRELWAKEUPALARM 0x22u
/** Service ID for SetAbsWakeupAlarm */
#define ECUM_SID_SETABSWAKEUPALARM 0x23u
/** Service ID for AbortWakeupAlarm */
#define ECUM_SID_ABORTWAKEUPALARM 0x24u
/** Service ID for GetCurrentTime */
#define ECUM_SID_GETCURRENTTIME 0x25u
/** Service ID for GetWakeupTime */
#define ECUM_SID_GETWAKEUPTIME 0x26u
/** Service ID for SetClock */
#define ECUM_SID_SETCLOCK 0x27u

/* Miscellaneous */
/** Service ID for SelectBootTarget */
#define ECUM_SID_SELECTBOOTTARGET 0x12u
/** Service ID for GetBootTarget */
#define ECUM_SID_GETBOOTTARGET 0x13u

/*Callbacks from Wakeup Sources*/
/** Service ID for SetWakeupEvent */
#define ECUM_SID_SETWAKEUPEVENT 0x0cu
/** Service ID for CheckWakeup */
#define ECUM_SID_CHECKWAKEUP 0x42u
/** Service ID for ValidateWakeupEvent */
#define ECUM_SID_VALIDATEWAKEUPEVENT 0x14u

/*Scheduled Functions*/
/** Service ID for MainFunction */
#define ECUM_SID_MAINFUNCTION 0x18u
/* ================================================ type definitions ================================================ */
/*Result of the Run Request Protocol sent to BswM*/
typedef uint8 EcuM_RunStatusType;

/**
 * @name ECUM_RUNSTATUS_SYSTEM_DEFINITION
 * @{
 * @brief Result of the Run Request Protocol sent to BswM.
 */
/** Run Status Unknown */
#define ECUM_RUNSTATUS_UNKNOWN (EcuM_RunStatusType)0u
/** Run Status Requested */
#define ECUM_RUNSTATUS_REQUESTED (EcuM_RunStatusType)1u
/** Run Status Released */
#define ECUM_RUNSTATUS_RELEASED (EcuM_RunStatusType)2u
/** @} */

/*The bit field provides one bit for each wake up source.
 *In WAKEUP, all bits cleared indicates that no wake up source is known.
 *In STARTUP, all bits cleared indicates that no reason for restart or reset is known.
 *In this case, ECUM_WKSOURCE_RESET shall be assumed.
 *EcuM defines a bit field with 5 pre-defined positions.
 */
typedef uint32 EcuM_WakeupSourceType;
/**
 * @name ECUM_WKSOURCE_SYSTEM_DEFINITION
 * @{
 * @brief The bit field provides one bit for each wake up source.
 */
/** Wakeup Source Power */
#define ECUM_WKSOURCE_POWER (EcuM_WakeupSourceType)0x00000001uL
/** Wakeup Source Reset */
#define ECUM_WKSOURCE_RESET (EcuM_WakeupSourceType)0x00000002uL
/** Wakeup Source Internal Reset */
#define ECUM_WKSOURCE_INTERNAL_RESET (EcuM_WakeupSourceType)0x00000004uL
/** Wakeup Source Internal WDG */
#define ECUM_WKSOURCE_INTERNAL_WDG (EcuM_WakeupSourceType)0x00000008uL
/** Wakeup Source External WDG */
#define ECUM_WKSOURCE_EXTERNAL_WDG (EcuM_WakeupSourceType)0x00000010uL
/** @} */

/*The type describes the possible states of a wakeup source.*/
typedef uint8 EcuM_WakeupStatusType;
/**
 * @name ECUM_WKSTATUS_SYSTEM_DEFINITION
 * @{
 * @brief The type describes the possible states of a wakeup source.
 */
/** Wakeup Status None */
#define ECUM_WKSTATUS_NONE (EcuM_WakeupStatusType)0u
/** Wakeup Status Pending */
#define ECUM_WKSTATUS_PENDING (EcuM_WakeupStatusType)1u
/** Wakeup Status Validated */
#define ECUM_WKSTATUS_VALIDATED (EcuM_WakeupStatusType)2u
/** Wakeup Status Expired */
#define ECUM_WKSTATUS_EXPIRED (EcuM_WakeupStatusType)3u
/** Wakeup Status Enabled */
#define ECUM_WKSTATUS_ENABLED (EcuM_WakeupStatusType)6u
/** @} */

/*This type describes the reset mechanisms supported by the ECU State Manager.
 *It can be extended by configuration.*/
typedef uint8 EcuM_ResetType;
/**
 * @name ECUM_RESET_RESOURCE
 * @{
 * @brief This type describes the reset mechanisms supported by the ECU State Manager.
 *It can be extended by configuration.
 */
/** Reset Type MCU */
#define ECUM_RESET_MCU (EcuM_ResetType)0u
/** Reset Type WDG */
#define ECUM_RESET_WDG (EcuM_ResetType)1u
/** Reset Type IO */
#define ECUM_RESET_IO (EcuM_ResetType)2u
/** @} */

/*Encode states and sub-states of the ECU Manager module.
 *States shall be encoded in the high-nibble, sub-states in the low-nibble.
 */
typedef uint8 EcuM_StateType;
/**
 * @name ECUM_RESET_RESOURCE
 * @{
 * @brief Encode states and sub-states of the ECU Manager module.
 */
/** Substate Mask */
#define ECUM_SUBSTATE_MASK (EcuM_StateType)0x0fu
/** State Startup */
#define ECUM_STATE_STARTUP (EcuM_StateType)0x10u
/** State Run */
#define ECUM_STATE_RUN (EcuM_StateType)0x30u
/** State App Run */
#define ECUM_STATE_APP_RUN (EcuM_StateType)0x32u
/** State App Post Run */
#define ECUM_STATE_APP_POST_RUN (EcuM_StateType)0x33u
/** State Shutdown */
#define ECUM_STATE_SHUTDOWN (EcuM_StateType)0x40u
/** State Sleep */
#define ECUM_STATE_SLEEP (EcuM_StateType)0x50u
/** @} */

/**
 * @brief The type describes the possible shutdown targets of the ECU State Manager.
 *
 */
typedef struct EcuM_TargetTag
{
    EcuM_ShutdownTargetType Target; /**< Target to shutdown to. @range 0x00 ~ 0xFF.  */
    EcuM_ShutdownModeType
        Mode; /**< If target is 'SLEEP', otherwise this parameter may ignored. @range 0x00 ~ 0xFFFF. */
} EcuM_TargetType;

/* EcuM Configuration Type (PB) */
typedef EcuM_PBConfigType EcuM_ConfigType;
/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */
/* Configuration Consistency Hash */
extern const uint32 EcuM_ConfigConsistencyHash;
/* ========================================= external function declarations ========================================= */
#if (ECUM_VERSION_INFO_API == STD_ON)
/**
 * @brief       Returns the version information of this module.
 * @param[out]  versioninfo : Pointer to where to store the version information of this module.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-64728
 */
void EcuM_GetVersionInfo(Std_VersionInfoType* versioninfo);
#endif /*ECUM_VERSION_INFO_API == STD_ON*/

/**
 * @brief       Instructs the ECU State Manager module to go into a sleep mode, reset, or off
 *              depending on the previously selected shutdown target.
 * @param[in]   UserID : Module ID of the calling module. Only special modules are allowed to
 *                       call this function and only valid when the shutdown target is RESET or OFF.
 * @return      Std_ReturnType: Status of the request.
 * @retval      E_OK: Request accepted and processed successfully.
 * @retval      E_NOT_OK: The request was not accepted.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-64730
 */
Std_ReturnType EcuM_GoDownHaltPoll(EcuM_UserType UserID);

/**
 * @brief       Initializes the ECU state manager and carries out the startup procedure.
 *              The function will never return (it calls StartOS).
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-64748
 */
void EcuM_Init(void);

/**
 * @brief       Implements the STARTUP II state.
 * @return      void
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64769
 */
void EcuM_StartupTwo(void);

/**
 * @brief       Typically called from the shutdown hook, this function takes over execution
 *              control and carries out GO OFF II activities.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-64767
 */
void EcuM_Shutdown(void);

/**
 * @brief       Function called by BswM to notify about a state switch.
 * @param[in]   state : State indicated by BswM.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-64766
 */
void EcuM_SetState(EcuM_StateType state);

#if (STD_ON == ECUM_MODE_HANDING)
/**
 * @brief       Places a request for the RUN state. Requests can be placed by every user
 *              made known to the state manager at configuration time.
 * @param[in]   user : ID of the entity requesting the RUN state.
 * @return      Std_ReturnType: Status of the request.
 * @retval      E_OK: The request was accepted by EcuM.
 * @retval      E_NOT_OK: The request was not accepted by EcuM, a detailed error
 *                        condition was sent to DET (see Error Codes).
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-64756
 */
Std_ReturnType EcuM_RequestRUN(EcuM_UserType user);

/**
 * @brief       Releases a RUN request previously done with a call to EcuM_RequestRUN.
 *              The service is intended for implementing AUTOSAR ports.
 * @param[in]   user : ID of the entity releasing the RUN state.
 * @return      Std_ReturnType: Status of the request.
 * @retval      E_OK: The request was accepted by EcuM.
 * @retval      E_NOT_OK: The request was not accepted by EcuM, a detailed error
 *                        condition was sent to DET (see Error Codes).
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-64754
 */
Std_ReturnType EcuM_ReleaseRUN(EcuM_UserType user);

/**
 * @brief       Places a request for the POST RUN state. Requests can be placed by every user
 *              made known to the state manager at configuration time. Requests for RUN and
 *              POST RUN must be tracked independently (i.e., using two independent variables).
 *              The service is intended for implementing AUTOSAR ports.
 * @param[in]   user : ID of the entity requesting the POST RUN state.
 * @return      Std_ReturnType: Status of the request.
 * @retval      E_OK: The request was accepted by EcuM.
 * @retval      E_NOT_OK: The request was not accepted by EcuM, a detailed error
 *                        condition was sent to DET (see Error Codes).
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-64755
 */
Std_ReturnType EcuM_RequestPOST_RUN(EcuM_UserType user);

/**
 * @brief       Releases a POST RUN request previously done with a call to EcuM_RequestPOST_RUN.
 *              The service is intended for implementing AUTOSAR ports.
 * @param[in]   user : ID of the entity releasing the POST RUN state.
 * @return      Std_ReturnType: Status of the request.
 * @retval      E_OK: The release request was accepted by EcuM.
 * @retval      E_NOT_OK: The release request was not accepted by EcuM,
 *                        a detailed error condition was sent to DET (see Error Codes).
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-64753
 */
Std_ReturnType EcuM_ReleasePOST_RUN(EcuM_UserType user);
#endif /*STD_ON == ECUM_MODE_HANDING*/

/**
 * @brief       Selects the shutdown target. This function is part of the ECU Manager Module port interface.
 * @param[in]   shutdownTarget : The selected shutdown target.
 * @param[in]   shutdownMode : The identifier of a sleep mode (if target is ECUM_STATE_SLEEP)
 *                             or a reset mechanism (if target is ECUM_STATE_RESET) as defined by configuration.
 * @return      Std_ReturnType: Status of the request.
 * @retval      E_OK: The new shutdown target was set.
 * @retval      E_NOT_OK: The new shutdown target was not set.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-64759
 */
Std_ReturnType EcuM_SelectShutdownTarget(EcuM_ShutdownTargetType shutdownTarget, EcuM_ShutdownModeType shutdownMode);

/**
 * @brief       Returns the currently selected shutdown target as set by EcuM_SelectShutdownTarget.
 *              This function is part of the ECU Manager Module port interface.
 * @param[out]  shutdownTarget : One of these values is returned:
 *                              ECUM_STATE_SLEEP / ECUM_STATE_RESET / ECUM_STATE_OFF.
 * @param[out]  shutdownMode : If the out parameter "shutdownTarget" is ECUM_STATE_SLEEP,
 *                             shutdownMode indicates which of the configured sleep modes was chosen.
 *                             If "shutdownTarget" is ECUM_STATE_RESET, shutdownMode indicates
 *                             which of the configured reset modes was chosen.
 * @return      Std_ReturnType: Status of the request.
 * @retval      E_OK: The service has succeeded.
 * @retval      E_NOT_OK: The service has failed, e.g., due to a NULL pointer being passed.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-64726
 */
Std_ReturnType EcuM_GetShutdownTarget(EcuM_ShutdownTargetType* shutdownTarget, EcuM_ShutdownModeType* shutdownMode);

/**
 * @brief       Returns the shutdown target of the previous shutdown process.
 *              This function is part of the ECU Manager Module port interface.
 * @param[out]  shutdownTarget : One of these values is returned:
 *                              ECUM_STATE_SLEEP / ECUM_STATE_RESET / ECUM_STATE_OFF.
 * @param[out]  shutdownMode : If the out parameter "shutdownTarget" is ECUM_STATE_SLEEP,
 *                             shutdownMode indicates which of the configured sleep modes was chosen.
 *                             If "shutdownTarget" is ECUM_STATE_RESET, shutdownMode indicates
 *                             which of the configured reset modes was chosen.
 * @return      Std_ReturnType: Status of the request.
 * @retval      E_OK: The service has succeeded.
 * @retval      E_NOT_OK: The service has failed, e.g., due to a NULL pointer being passed.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-64723
 */
Std_ReturnType EcuM_GetLastShutdownTarget(EcuM_ShutdownTargetType* shutdownTarget, EcuM_ShutdownModeType* shutdownMode);

/**
 * @brief       Elects the cause for a shutdown. This function is part of the ECU Manager Module port interface.
 * @param[in]   target : The selected shutdown cause.
 * @return      Std_ReturnType: Status of the request.
 * @retval      E_OK: The new shutdown cause was set.
 * @retval      E_NOT_OK: The new shutdown cause was not set.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-64758
 */
Std_ReturnType EcuM_SelectShutdownCause(EcuM_ShutdownCauseType cause);

/**
 * @brief       Returns the selected shutdown cause as set by EcuM_SelectShutdownCause.
 *              This function is part of the ECU Manager Module port interface.
 * @param[out]  shutdownCause : The selected cause of the next shutdown.
 * @return      Std_ReturnType: Status of the request.
 * @retval      E_OK: The service has succeeded.
 * @retval      E_NOT_OK: The service has failed.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-64725
 */
Std_ReturnType EcuM_GetShutdownCause(EcuM_ShutdownCauseType* shutdownCause);

/**
 * @brief       Gets pending wakeup events.
 * @return      EcuM_WakeupSourceType: All wakeup events.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64724
 */
EcuM_WakeupSourceType EcuM_GetPendingWakeupEvents(void);

/**
 * @brief       Clears wakeup events.
 * @param[in]   sources : Events to be cleared.
 * @return      None.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64419
 */
void EcuM_ClearWakeupEvent(EcuM_WakeupSourceType sources);

/**
 * @brief       Gets validated wakeup events.
 * @return      EcuM_WakeupSourceType: All wakeup events.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64727
 */
EcuM_WakeupSourceType EcuM_GetValidatedWakeupEvents(void);

/**
 * @brief       Gets expired wakeup events.
 * @return      EcuM_WakeupSourceType: All events that have been set and for which validation has failed.
 *              Events which do not need validation must never be reported by this function.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64722
 */
EcuM_WakeupSourceType EcuM_GetExpiredWakeupEvents(void);

/* PRQA S 4604 ++ */ /* VL_EcuM_4604 */
/**
 * @brief       Sets a user's wakeup alarm relative to the current point in time.
 *              This function is part of the ECU Manager Module port interface.
 * @param[in]   user : The user that wants to set the wakeup alarm.
 * @param[in]   time : Relative time from now in seconds.
 * @return      Std_ReturnType: Status of the request.
 * @retval      E_OK: The service has succeeded.
 * @retval      E_NOT_OK: The service failed.
 * @retval      ECUM_E_EARLIER_ACTIVE: An earlier alarm is already set.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-64763
 */
Std_ReturnType EcuM_SetRelWakeupAlarm(EcuM_UserType user, EcuM_TimeType time);
/* PRQA S 4604 -- */

/* PRQA S 4604 ++ */ /* VL_EcuM_4604 */
/**
 * @brief       Sets the user's wakeup alarm to an absolute point in time.
 *              This function is part of the ECU Manager Module port interface.
 * @param[in]   user : The user that wants to set the wakeup alarm.
 * @param[in]   time : Absolute time in seconds. Note that absolute alarms use knowledge of the current time.
 * @return      Std_ReturnType: Status of the request.
 * @retval      E_OK: The service has succeeded.
 * @retval      E_NOT_OK: The service failed.
 * @retval      ECUM_E_EARLIER_ACTIVE: An earlier alarm is already set.
 * @retval      ECUM_E_PAST: The given point in time has already passed.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-64761
 */
Std_ReturnType EcuM_SetAbsWakeupAlarm(EcuM_UserType user, EcuM_TimeType time);
/* PRQA S 4604 -- */

/**
 * @brief       Aborts the wakeup alarm previously set by this user.
 *              This function is part of the ECU Manager Module port interface.
 * @param[in]   user : The user that wants to cancel the wakeup alarm.
 * @return      Std_ReturnType: Status of the request.
 * @retval      E_OK: The service has succeeded.
 * @retval      E_NOT_OK: The service failed.
 * @retval      ECUM_E_NOT_ACTIVE: No owned alarm found.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-64416
 */
Std_ReturnType EcuM_AbortWakeupAlarm(EcuM_UserType user);

/* PRQA S 3432,4604 ++ */ /* VL_EcuM_3432, VL_EcuM_4604 */
/**
 * @brief       Returns the current value of the EcuM clock (i.e., the time since battery connect).
 *              This function is part of the ECU Manager Module port interface.
 * @param[out]  time : Absolute time in seconds since battery connect.
 * @return      Std_ReturnType: Status of the request.
 * @retval      E_OK: The service has succeeded.
 * @retval      E_NOT_OK: `time` points to NULL or the module is not initialized.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-64721
 */
Std_ReturnType EcuM_GetCurrentTime(EcuM_TimeType* time);
/* PRQA S 3432,4604 -- */

/* PRQA S 3432,4604 ++ */ /* VL_EcuM_3432, VL_EcuM_4604 */
/**
 * @brief       Returns the current value of the master alarm clock (the minimum absolute time of all user alarm
 * clocks). This function is part of the ECU Manager Module port interface.
 * @param[out]  time : Absolute time in seconds for the next wakeup. 0xFFFFFFFF means no active alarm.
 * @return      Std_ReturnType: Status of the request.
 * @retval      E_OK: The service has succeeded.
 * @retval      E_NOT_OK: `time` points to NULL or the module is not initialized.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-64729
 */
Std_ReturnType EcuM_GetWakeupTime(EcuM_TimeType* time);
/* PRQA S 3432,4604 -- */

/* PRQA S 4604 ++ */ /* VL_EcuM_4604 */
/**
 * @brief       Sets the EcuM clock time to the provided value. This API is useful for testing the alarm services;
 *              Alarms that take days to expire can be tested.
 *              This function is part of the ECU Manager Module port interface.
 * @param[in]   user : The user that wants to set the clock.
 * @param[in]   time : Absolute time in seconds since battery connect.
 * @return      Std_ReturnType: Status of the request.
 * @retval      E_OK: The service has succeeded.
 * @retval      E_NOT_OK: The service failed.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-64762
 */
Std_ReturnType EcuM_SetClock(EcuM_UserType user, EcuM_TimeType time);
/* PRQA S 4604 -- */

/**
 * @brief       Selects a boot target.
 *              This function is part of the ECU Manager Module port interface.
 * @param[in]   target : The selected boot target.
 * @return      Std_ReturnType: Status of the request.
 * @retval      E_OK: The new boot target was accepted by EcuM.
 * @retval      E_NOT_OK: The new boot target was not accepted by EcuM.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-64757
 */
Std_ReturnType EcuM_SelectBootTarget(EcuM_BootTargetType target);

/**
 * @brief       Returns the current boot target.
 *              This function is part of the ECU Manager Module port interface.
 * @param[out]  target : The currently selected boot target.
 * @return      Std_ReturnType: Status of the request.
 * @retval      E_OK: The service always succeeds.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-64420
 */
Std_ReturnType EcuM_GetBootTarget(EcuM_BootTargetType* target);

/**
 * @brief       Sets the wakeup event.
 *              Sets (OR-operation) all events passed as a bit set in the `sources` parameter
 *              (EcuM_WakeupSourceType bitmask) in the internal pending wakeup events variable.
 *              Starts the wakeup validation timeout timer.
 *              Ignores all events passed in the `sources` parameter not associated with the selected sleep mode.
 *              Must be callable from interrupt context, OS context, and an OS-free context.
 * @param[in]   sources : Value to be set.
 * @return      None
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-68940
 */
void EcuM_SetWakeupEvent(EcuM_WakeupSourceType sources);

/**
 * @brief       After wakeup, the ECU State Manager will stop the process during the WAKEUP VALIDATION state/sequence to
 * wait for validation of the wakeup event. This API service is used to indicate to the ECU Manager module that the
 * wakeup events indicated in the `sources` parameter have been validated.
 * @param[in]   sources : Events that have been validated.
 * @return      None
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-68941
 */
void EcuM_ValidateWakeupEvent(EcuM_WakeupSourceType sources);

/**
 * @brief       This function can be called to check the given wakeup sources. It will pass the argument to the
 * integrator function EcuM_CheckWakeupHook. It can also be called by the ISR of a wakeup source to set up the PLL and
 * check other wakeup sources that may be connected to the same interrupt.
 * @param[in]   sources : Events that have been validated.
 * @return      None
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-68942
 */
void EcuM_CheckWakeup(EcuM_WakeupSourceType wakeupSource);

/**
 * @brief       This callback shall provide BSW module initializations to be called by the BSW Mode Manager.
 * @param[in]   drvInitIdx : Index of the driver to initialize.
 * @return      None
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-68943
 */
void EcuM_AL_DriverInitBswM(uint8 drvInitIdx);

#ifdef __cplusplus
}
#endif
#endif /*ECUM_H_*/
