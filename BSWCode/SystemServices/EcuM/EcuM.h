/* PRQA S 3108++ */
/**
 * Copyright (C) 2024 Isoft Infrastructure Software Co., Ltd.
 * SPDX-License-Identifier: LGPL-2.1-only-with-exception OR  LicenseRef-Commercial-License
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the
 * GNU Lesser General Public License as published by the Free Software Foundation; version 2.1.
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License along with this library;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * or see <https://www.gnu.org/licenses/>.
 *
 * Alternatively, this file may be used under the terms of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License, in which case the provisions of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License shall apply instead of those of the GNU Lesser General Public License.
 *
 * You should have received a copy of the Isoft Infrastructure Software Co., Ltd.  Commercial License
 * along with this program. If not, please find it at <https://EasyXMen.com/xy/reference/permissions.html>
 */
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : EcuM.h                                                      **
**                                                                            **
**  Created on  :                                                             **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION : This file is partly ECU dependent.                          **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11.                     **
**                                                                            **
*******************************************************************************/

/******************************************************************************
**                      Revision Control History                             **
******************************************************************************/
/*  <VERSION>    <DATE>      <AUTHOR>        <REVISION LOG>
 *  V1.0.0       2020-06-28  qinchun.yang    R19_11 EcuM initial version.
 *  V2.0.0       2021-04-08  qinchun.yang    R1911 stable version.
 */
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#ifndef ECUM_H_
#define ECUM_H_

#include "EcuM_Cfg.h"
#include "EcuM_Cbk.h"
#include "EcuM_Types.h"
#include "Os.h"
#include "Mcu.h"

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define ECUM_VENDOR_ID                62u
#define ECUM_MODULE_ID                10u
#define ECUM_INSTANCE_ID              0u

#define ECUM_AR_RELEASE_MAJOR_VERSION 4u
#define ECUM_AR_RELEASE_MINOR_VERSION 5u
#define ECUM_AR_RELEASE_PATCH_VERSION 0u

#define ECUM_SW_MAJOR_VERSION         2u
#define ECUM_SW_MINOR_VERSION         1u
#define ECUM_SW_PATCH_VERSION         16u

#define ECUM_SID_GETVERSION           0x00u

/*Initialization and Shutdown Sequences*/
#define ECUM_SID_GODOWNHALTPOLL 0x2cu
#define ECUM_SID_INIT           0x01u
#define ECUM_SID_STARTUPTWO     0x1au
#define ECUM_SID_SHUTDOWN       0x02u

/* State Management */
#define ECUM_SID_SETSTATE        0x2bu
#define ECUM_SID_REQUESTRUN      0x03u
#define ECUM_SID_RELEASERUN      0x04u
#define ECUM_SID_REQUESTPOST_RUN 0x0au
#define ECUM_SID_RELEASEPOST_RUN 0x0bu

/* Shutdown Management */
#define ECUM_SID_SELECTSHUTDOWNTARGET  0x06u
#define ECUM_SID_GETSHUTDOWNTARGET     0x09u
#define ECUM_SID_GETLASTSHUTDOWNTARGET 0x08u
#define ECUM_SID_SELECTSHUTDOWNCAUSE   0x1bu
#define ECUM_SID_GETSHUTDOWNCAUSE      0x1cu

/* Wakeup Handling */
#define ECUM_SID_GETPENDINGWAKEUPEVENT   0x0du
#define ECUM_SID_CLEARWAKEUPEVENT        0x16u
#define ECUM_SID_GETVALIDATEDWAKEUPEVENT 0x15u
#define ECUM_SID_GETEXPIREDWAKEUPEVENT   0x19u

/* Alarm Clock */
#define ECUM_SID_SETRELWAKEUPALARM 0x22u
#define ECUM_SID_SETABSWAKEUPALARM 0x23u
#define ECUM_SID_ABORTWAKEUPALARM  0x24u
#define ECUM_SID_GETCURRENTTIME    0x25u
#define ECUM_SID_GETWAKEUPTIME     0x26u
#define ECUM_SID_SETCLOCK          0x27u

/* Miscellaneous */
#define ECUM_SID_SELECTBOOTTARGET 0x12u
#define ECUM_SID_GETBOOTTARGET    0x13u

/*Callbacks from Wakeup Sources*/
#define ECUM_SID_SETWAKEUPEVENT      0x0cu
#define ECUM_SID_CHECKWAKEUP         0x42u
#define ECUM_SID_VALIDATEWAKEUPEVENT 0x14u

/*Scheduled Functions*/
#define ECUM_SID_MAINFUNCTION 0x18u
/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
/*********************Configuration Types***************************/
/*Post build configuration root type definition*/
typedef struct
{
    uint32 configConsistencyHash;               /*Configuration consistency hash code for
                                                pre-compile and link time configure*/
    AppModeType defaultAppMode;                 /*EcuMDefaultAppMode*/
    EcuM_ShutdownTargetType defaultShutdownTgt; /*EcuMDefaultState*/
    EcuM_ShutdownModeType defaultShutdownMode;  /*EcuMDefaultResetMode or
                                                  EcuMDefaultSleepMode*/
    const EcuM_GenBSWPbCfgType* modulePBCfg;    /*PB CFG data pointer for modules*/
} EcuM_ConfigType;
/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/
extern CONST(uint32, ECUM_CONST) EcuM_ConfigConsistencyHash;
/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
#if (ECUM_VERSION_INFO_API == STD_ON)
/**
 * Returns the version information of this module.
 * Service ID: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): versioninfo, Pointer to where to store the version
 *                               information of this module.
 * Return value: NA
 */
FUNC(void, ECUM_CODE) EcuM_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, ECUM_APPL_DATA) versioninfo);
#endif /*ECUM_VERSION_INFO_API == STD_ON*/

/**
 * Instructs the ECU State Manager module to go into a sleep mode, Reset or OFF
 * depending on the previously selected shutdown target.
 * Service ID: 0x2C
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): caller:Module ID of the calling module. Only special modules are allowed to
 *                      call this function and only valid when shutdown target is RESET or OFF.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_NOT_OK: The request was not accepted.
 *               E_OK:  If the ShutdownTargetType is SLEEP the call successfully returns, the
 *                       ECU has left the sleep again.
 */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_GoDownHaltPoll(uint16 caller);

/**
 * Initializes the ECU state manager and carries out the startup procedure.
 * The function will never return (it calls StartOS).
 * Service ID[hex]: 0x01
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, ECUM_CODE)
EcuM_Init(void);

/**
 * This function implements the STARTUP II state.
 * Service ID[hex]: 0x1a
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, ECUM_CODE)
EcuM_StartupTwo(void);

/**
 * Typically called from the shutdown hook, this function takes over execution
 * control and will carry out GO OFF II activities.
 * Service ID[hex]: 0x02
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, ECUM_CODE) EcuM_Shutdown(void);

/**
 * Function called by BswM to notify about State Switch.
 * Service ID[hex]: 0x2b
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): state, State indicated by BswM.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, ECUM_CODE) EcuM_SetState(EcuM_StateType state);

#if (STD_ON == ECUM_MODE_HANDING)
/**
 * Places a request for the RUN state. Requests can be placed by every user
 * made known to the state manager at configuration time.
 * Service ID[hex]: 0x03
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): user, ID of the entity requesting the RUN state
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: The request was accepted by EcuM.
 *               E_NOT_OK: The request was not accepted by EcuM, a detailed error
 *               condition was sent to DET (see Error Codes).
 */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_RequestRUN(EcuM_UserType user);

/**
 * Releases a RUN request previously done with a call to EcuM_RequestRUN.
 * The service is intended for implementing AUTOSAR ports.
 * Service ID[hex]: 0x04
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): user, ID of the entity releasing the RUN state
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: The request was accepted by EcuM.
 *               E_NOT_OK: The request was not accepted by EcuM, a detailed error
 *               condition was sent to DET (see Error Codes).
 */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_ReleaseRUN(EcuM_UserType user);

/**
 * Places a request for the POST RUN state. Requests can be placed by every user
 * made known to the state manager at configuration time. Requests for RUN and
 * POST RUN must be tracked independently (in other words: two independent
 *  variables). The service is intended for implementing AUTOSAR ports.
 * Service ID[hex]: 0x0a
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): user, ID of the entity requesting the POST RUN state
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: The request was accepted by EcuM
 *               E_NOT_OK: The request was not accepted by EcuM, a detailed
 *                         error condition was sent to DET (see Error Codes).
 */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_RequestPOST_RUN(EcuM_UserType user);

/**
 * Releases a POST RUN request previously done with a call to EcuM_RequestPOST_RUN.
 * The service is intended for implementing AUTOSAR ports.
 * Service ID[hex]: 0x0b
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): user, ID of the entity releasing the POST RUN state
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: The release request was accepted by EcuM
 *               E_NOT_OK: The release request was not accepted by EcuM,
 *                         a detailed error condition was sent to DET (see Error Codes).
 */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_ReleasePOST_RUN(EcuM_UserType user);
#endif /*STD_ON == ECUM_MODE_HANDING*/

/**
 * EcuM_SelectShutdownTarget selects the shutdown target.
 * EcuM_SelectShutdownTarget is part of the ECU Manager Module port interface.
 * Service ID[hex]: 0x06
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): shutdownTarget The selected shutdown target
 *                  shutdownMode   The identifier of a sleep mode (if target is
 *                                 ECUM_STATE_SLEEP) or a reset mechanism
 *                                 (if target is ECUM_STATE_RESET) as defined by configuration.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: The new shutdown target was set
 *               E_NOT_OK: The new shutdown target was not set
 */
FUNC(Std_ReturnType, ECUM_CODE)
EcuM_SelectShutdownTarget(EcuM_ShutdownTargetType shutdownTarget, EcuM_ShutdownModeType shutdownMode);

/**
 * Returns the currently selected shutdown target as set by EcuM_SelectShutdownTarget.
 * EcuM_GetShutdownTarget is part of the ECU Manager Module port interface.
 * Service ID[hex]: 0x09
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): shutdownTarget One of these values is returned:
 *                                 ECUM_STATE_SLEEP/ECUM_STATE_RESET/ECUM_STATE_OFF
 *                  shutdownMode  If the out parameter "shutdownTarget" is
 *                                ECUM_STATE_SLEEP, sleepMode tells which of the
 *                                configured sleep modes was actually chosen.
 *                                If "shutdownTarget" is ECUM_STATE_RESET,
 *                                sleepMode tells which of the configured reset
 *                                modes was actually chosen.
 * Return value: E_OK: The service has succeeded
 *               E_NOT_OK: The service has failed, e.g. due to NULL pointer being passed
 */
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
FUNC(Std_ReturnType, ECUM_CODE)
EcuM_GetShutdownTarget(
    P2VAR(EcuM_ShutdownTargetType, AUTOMATIC, ECUM_APPL_DATA) shutdownTarget,
    P2VAR(EcuM_ShutdownModeType, AUTOMATIC, ECUM_APPL_DATA) shutdownMode);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

/**
 * Returns the shutdown target of the previous shutdown process.
 * EcuM_GetLastShutdownTarget is part of the ECU Manager Module port interface.
 * Service ID[hex]: 0x08
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): shutdownTarget One of these values is returned:
 *                                 ECUM_STATE_SLEEP/ECUM_STATE_RESET/ECUM_STATE_OFF
 *                  shutdownMode  If the out parameter "shutdownTarget" is
 *                                ECUM_STATE_SLEEP, sleepMode tells which of the
 *                                configured sleep modes was actually chosen.
 *                                If "shutdownTarget" is ECUM_STATE_RESET,
 *                                sleepMode tells which of the configured reset
 *                                modes was actually chosen.
 * Return value: E_OK: The service has succeeded
 *               E_NOT_OK: The service has failed, e.g. due to NULL pointer being passed
 */
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
FUNC(Std_ReturnType, ECUM_CODE)
EcuM_GetLastShutdownTarget(
    P2VAR(EcuM_ShutdownTargetType, AUTOMATIC, ECUM_APPL_DATA) shutdownTarget,
    P2VAR(EcuM_ShutdownModeType, AUTOMATIC, ECUM_APPL_DATA) shutdownMode);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

/**
 * Elects the cause for a shutdown.
 * EcuM_SelectShutdownCause is part of the ECU Manager Module port interface.
 * Service ID[hex]: 0x1b
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): target, the selected shutdown cause
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: E_OK: The new shutdown cause was set
 *               E_NOT_OK: The new shutdown cause was not set
 */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_SelectShutdownCause(EcuM_ShutdownCauseType cause);

/**
 * Returns the selected shutdown cause as set by EcuM_SelectShutdownCause.
 * EcuM_GetShutdownCause is part of the ECU Manager Module port interface.
 * Service ID[hex]: 0x1c
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): NA
 * Parameters(INOUT): NA
 * Parameters(OUT):  shutdownCause, the selected cause of the next shutdown.
 * Return value: E_OK: The service has succeeded
 *               E_NOT_OK: The service has failed
 */
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
FUNC(Std_ReturnType, ECUM_CODE)
EcuM_GetShutdownCause(P2VAR(EcuM_ShutdownCauseType, AUTOMATIC, ECUM_APPL_DATA) shutdownCause);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

/**
 * Gets pending wakeup events.
 * Service ID[hex]: 0x0d
 * Sync/Async: Synchronous
 * Reentrancy: Non-Reentrant, Non-Interruptible
 * Parameters (in): NA
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: All wakeup events
 */
FUNC(EcuM_WakeupSourceType, ECUM_CODE) EcuM_GetPendingWakeupEvents(void);

/**
 * Clears wakeup events.
 * Service ID[hex]: 0x16
 * Sync/Async: Synchronous
 * Reentrancy: Non-Reentrant, Non-Interruptible
 * Parameters (in): sources, events to be cleared
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: NA
 */
FUNC(void, ECUM_CODE) EcuM_ClearWakeupEvent(EcuM_WakeupSourceType sources);

/**
 * Gets validated wakeup events.
 * Service ID[hex]: 0x15
 * Sync/Async: Synchronous
 * Reentrancy: Non-Reentrant, Non-Interruptible
 * Parameters (in): NA
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: All wakeup events
 */
FUNC(EcuM_WakeupSourceType, ECUM_CODE) EcuM_GetValidatedWakeupEvents(void);

/**
 * Gets expired wakeup events.
 * Service ID[hex]: 0x19
 * Sync/Async: Synchronous
 * Reentrancy: Non-Reentrant, Non-Interruptible
 * Parameters (in): NA
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: All wakeup events:Returns all events that have been set and
 *               for which validation has failed. Events which do not need
 *               validation must never be reported by this function.
 */
FUNC(EcuM_WakeupSourceType, ECUM_CODE) EcuM_GetExpiredWakeupEvents(void);

/**
 * Sets a user's wakeup alarm relative to the current point in time.
 * EcuM_SetRelWakeupAlarm is part of the ECU Manager Module port interface.
 * Service ID[hex]: 0x22
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): user, the user that wants to set the wakeup alarm.
 *                  time, relative time from now in seconds.
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: E_OK: The service has succeeded
 *               E_NOT_OK: The service failed
 *               ECUM_E_EARLIER_ACTIVE: An earlier alarm is already set
 */
/* PRQA S 4604 ++ */ /* MISRA Rule 21.2 */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_SetRelWakeupAlarm(EcuM_UserType user, EcuM_TimeType time);
/* PRQA S 4604 -- */ /* MISRA Rule 21.2 */

/**
 * Sets the user's wakeup alarm to an absolute point in time.
 * EcuM_SetAbsWakeupAlarm is part of the ECU Manager Module port interface.
 * Service ID[hex]: 0x23
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): user, the user that wants to set the wakeup alarm.
 *                  time, absolute time in seconds. Note that, absolute alarms
 *                        use knowledge of the current time.
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: E_OK: The service has succeeded
 *               E_NOT_OK: The service failed
 *               ECUM_E_EARLIER_ACTIVE: An earlier alarm is already set
 *               ECUM_E_PAST: The given point in time has already passed
 */
/* PRQA S 4604 ++ */ /* MISRA Rule 21.2 */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_SetAbsWakeupAlarm(EcuM_UserType user, EcuM_TimeType time);
/* PRQA S 4604 -- */ /* MISRA Rule 21.2 */

/**
 * Aborts the wakeup alarm previously set by this user.
 * EcuM_AbortWakeupAlarm is part of the ECU Manager Module port interface.
 * Service ID[hex]: 0x24
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): user, the user that wants to cancel the wakeup alarm
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: E_OK: The service has succeeded
 *               E_NOT_OK: The service failed
 *               ECUM_E_NOT_ACTIVE: No owned alarm found
 */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_AbortWakeupAlarm(EcuM_UserType user);

/**
 * Returns the current value of the EcuM clock (i.e. the time since battery connect).
 * EcuM_GetCurrentTime is part of the ECU Manager Module port interface.
 * Service ID[hex]: 0x25
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): NA
 * Parameters(INOUT): NA
 * Parameters(OUT):  time, absolute time in seconds since battery connect.
 * Return value: E_OK: The service has succeeded
 *               E_NOT_OK: time points to NULL or the module is not initialized
 */
/* PRQA S 3432,4604 ++ */ /* MISRA Rule 20.7,21.2 */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_GetCurrentTime(P2VAR(EcuM_TimeType, AUTOMATIC, ECUM_APPL_DATA) time);
/* PRQA S 3432,4604 -- */ /* MISRA Rule 20.7,21.2 */

/**
 * Returns the current value of the master alarm clock (the minimum absolute
 * time of all user alarm clocks).
 * EcuM_GetWakeupTime is part of the ECU Manager Module port interface.
 * Service ID[hex]: 0x26
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): NA
 * Parameters(INOUT): NA
 * Parameters(OUT):  time, absolute time in seconds for next wakeup.
 *                        0xFFFFFFFF means no active alarm.
 * Return value: E_OK: The service has succeeded
 *               E_NOT_OK: time points to NULL or the module is not initialized
 */
/* PRQA S 3432,4604 ++ */ /* MISRA Rule 20.7,21.2 */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_GetWakeupTime(P2VAR(EcuM_TimeType, AUTOMATIC, ECUM_APPL_DATA) time);
/* PRQA S 3432,4604 -- */ /* MISRA Rule 20.7,21.2 */

/**
 * Sets the EcuM clock time to the provided value. This API is useful for
 * testing the alarm services; Alarms that take days to expire can be tested.
 * EcuM_SetClock is part of the ECU Manager Module port interface.
 * Service ID[hex]: 0x27
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): user, user that wants to set the clock
 *                  time, absolute time in seconds since battery connect.
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: E_OK: The service has succeeded
 *               E_NOT_OK: The service failed
 */
/* PRQA S 4604 ++ */ /* MISRA Rule 21.2 */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_SetClock(EcuM_UserType user, EcuM_TimeType time);
/* PRQA S 4604 -- */ /* MISRA Rule 21.2 */

/**
 * Selects a boot target.
 * EcuM_SelectBootTarget is part of the ECU Manager Module port interface.
 * Service ID[hex]: 0x12
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): target, the selected boot target.
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: E_OK: The new boot target was accepted by EcuM
 *               E_NOT_OK: The new boot target was not accepted by EcuM
 */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_SelectBootTarget(EcuM_BootTargetType target);

/**
 * Returns the current boot target.
 * EcuM_GetBootTarget is part of the ECU Manager Module port interface.
 * Service ID[hex]: 0x13
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): NA
 * Parameters(INOUT): NA
 * Parameters(OUT):  target, the currently selected boot target.
 * Return value: E_OK: The service always succeeds.
 */
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_GetBootTarget(P2VAR(EcuM_BootTargetType, AUTOMATIC, ECUM_APPL_DATA) target);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

FUNC(void, ECUM_AL_DRIVERINITBSWM_CODE)
EcuM_AL_DriverInitBswM(uint8 drvInitIdx);
#endif /*ECUM_H_*/
