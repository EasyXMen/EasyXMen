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
**  FILENAME    : CanTrcv.h                                                   **
**                                                                            **
**  Created on  :                                                             **
**  Author      : Xinrun.Wang                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Public interfaces declared by CanTrcv module                **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
#ifndef CANTRCV_H_
#define CANTRCV_H_

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "CanTrcv_Types.h"

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/

/* version info */
#define CANTRCV_VENDOR_ID          62U
#define CANTRCV_MODULE_ID          70U
#define CANTRCV_INSTANCE_ID        0U
#define CANTRCV_H_AR_MAJOR_VERSION 19U
#define CANTRCV_H_AR_MINOR_VERSION 11U
#define CANTRCV_H_AR_PATCH_VERSION 0U
#define CANTRCV_H_SW_MAJOR_VERSION 2U
#define CANTRCV_H_SW_MINOR_VERSION 0U
#define CANTRCV_H_SW_PATCH_VERSION 1U

/* timer type */
#define None           0x00u
#define Timer_1us16bit 0x01u

/* wakeup support */
#define CANTRCV_WAKEUP_BY_POLLING    0x00u
#define CANTRCV_WAKEUP_NOT_SUPPORTED 0x01u

/* DET ERROR CODE */
#if (STD_ON == CANTRCV_DEV_ERROR_DETECT)
#define CANTRCV_E_INVALID_TRANSCEIVER    ((uint8)0x01)
#define CANTRCV_E_PARAM_POINTER          ((uint8)0x02)
#define CANTRCV_E_UNINIT                 ((uint8)0x11)
#define CANTRCV_E_TRCV_NOT_STANDBY       ((uint8)0x21)
#define CANTRCV_E_TRCV_NOT_NORMAL        ((uint8)0x22)
#define CANTRCV_E_PARAM_TRCV_WAKEUP_MODE ((uint8)0x23)
#define CANTRCV_E_PARAM_TRCV_OPMODE      ((uint8)0x24)
#define CANTRCV_E_BAUDRATE_NOT_SUPPORTED ((uint8)0x25)
#define CANTRCV_E_INIT_FAILED            ((uint8)0x27)
#endif

#define CANTRCV_E_NO_TRCV_CONTROL ((uint8)0x26)

/* CanTrcv Module API ID */
#define CANTRCV_INIT_ID                    ((uint8)0x00)
#define CANTRCV_SETOPMODE_ID               ((uint8)0x01)
#define CANTRCV_GETOPMODE_ID               ((uint8)0x02)
#define CANTRCV_GETBUSWUREASON_ID          ((uint8)0x03)
#define CANTRCV_GETVERSIONINFO_ID          ((uint8)0x04)
#define CANTRCV_SETWAKEUPMODE_ID           ((uint8)0x05)
#define CANTRCV_MAINFUNCTION_ID            ((uint8)0x06)
#define CANTRCV_CHECKWAKEUP_ID             ((uint8)0x07)
#define CANTRCV_MAINFUNCTIONDIAGNOSTICS_ID ((uint8)0x08)
#define CANTRCV_GETTRCVSYSTEMDATA_ID       ((uint8)0x09)
#define CANTRCV_DEINIT_ID                  ((uint8)0x10)
#define CANTRCV_CLEARTRCVWUFFLAG_ID        ((uint8)0x0a)
#define CANTRCV_READTRCVTIMEOUTFLAG_ID     ((uint8)0x0b)
#define CANTRCV_CLEARTRCVTIMEOUTFLAG_ID    ((uint8)0x0c)
#define CANTRCV_READTRCVSILENCEFLAG_ID     ((uint8)0x0d)
#define CANTRCV_CHECKWAKEFLAG_ID           ((uint8)0x0e)
#define CANTRCV_SETPNACTIVATIONSTATE_ID    ((uint8)0x0f)

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
/*Data type used for describing whether PN wakeup functionality in CanTrcv is
 * enabled or disabled.*/
typedef enum
{
    PN_ENABLED, /*PN wakeup functionality in CanTrcv is enabled*/
    PN_DISABLED /*PN wakeup functionality in CanTrcv is disabled*/
} CanTrcv_PNActivationType;

/*Provides the state of a flag in the transceiver hardware*/
typedef enum
{
    CANTRCV_FLAG_SET,    /*The flag is set in the transceiver hardware*/
    CANTRCV_FLAG_CLEARED /*The flag is cleared in the transceiver hardware*/
} CanTrcv_TrcvFlagStateType;

/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/

/* CanTrcv config data */

extern CONST(CanTrcv_ConfigType, CANTRCV_CONST_PBCFG) CanTrcv_Config;

/* CanTrcv config data */

extern P2CONST(CanTrcv_ConfigType, CANTRCV_CONST, CANTRCV_CONST_PBCFG) CanTrcv_ConfigPtr;

#if (STD_ON == CANTRCV_HWPN_SUPPORT)
/* CanTrcv PN switch */

extern VAR(CanTrcv_PNActivationType, CANTRCV_VAR_NOINIT) CanTrcv_PNActivation;

#endif /* STD_ON == CANTRCV_HWPN_SUPPORT */

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

/**
 * This function initializes the CanTrcv module.
 * Service ID: 0x00
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): ConfigPtr: Pointer to selected configuration structure
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */

extern FUNC(void, CANTRCV_CODE) CanTrcv_Init(P2CONST(CanTrcv_ConfigType, AUTOMATIC, CANTRCV_CONST_PBCFG) ConfigPtr);

/**
 * This function sets the mode of the Transceiver to the value OpMode.
 * Service ID: 0x01
 * Sync/Async: Asynchronous
 * Reentrancy: Reentrant for different transceivers
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 *                 OpMode: This parameter contains the desired operating mode.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: the request for transceiver mode change has been accepted.
 *               E_NOT_OK: the request for transceiver mode change has not been accepted
 *                         or any parameter is out of the allowed range.
 *
 */

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_SetOpMode(uint8 Transceiver, CanTrcv_TrcvModeType OpMode);

/**
 * This function gets the mode of the Transceiver and returns it in OpMode.
 * Service ID: 0x02
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): OpMode: Pointer to operation mode of the bus the API is applied to.
 * Return value: E_OK: the operation mode was detected.
 *               E_NOT_OK: the operation mode was not detected.
 *
 */

extern FUNC(Std_ReturnType, CANTRCV_CODE)
    CanTrcv_GetOpMode(uint8 Transceiver, P2VAR(CanTrcv_TrcvModeType, AUTOMATIC, CANTRCV_APPL_DATA) OpMode);

/**
 * This function gets the wakeup reason for the Transceiver and returns it in parameter Reason.
 * Service ID: 0x03
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): reason: Pointer to wake up reason of the bus the API is applied to.
 * Return value: E_OK: the transceiver wakeup reason was provided.
 *               E_NOT_OK: no wake up reason is available or the service request failed due to development errors.
 *
 */

extern FUNC(Std_ReturnType, CANTRCV_CODE)
    CanTrcv_GetBusWuReason(uint8 Transceiver, P2VAR(CanTrcv_TrcvWakeupReasonType, AUTOMATIC, CANTRCV_APPL_DATA) reason);

#if (STD_ON == CANTRCV_VERSIONINFO_API)
/**
 * This function gets the version of the module and returns it in VersionInfo.
 * Service ID: 0x04
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): versioninfo: Pointer to version information of this module.
 * Return value: NA
 *
 */

extern FUNC(void, CANTRCV_CODE)
    CanTrcv_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, CANTRCV_APPL_DATA) versioninfo);

#endif /* STD_ON == CANTRCV_VERSIONINFO_API */

/**
 * This function enables, disables or clears wake-up events of the Transceiver according to TrcvWakeupMode.
 * Service ID: 0x05
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant for different transceivers
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 *                 TrcvWakeupMode: Requested transceiver wakeup reason.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: the wakeup state has been changed to the requested mode.
 *               E_NOT_OK: the wakeup state change has failed or the parameter is out of the allowed range. The previous
 * state has not been changed.
 *
 */

extern FUNC(Std_ReturnType, CANTRCV_CODE)
    CanTrcv_SetWakeupMode(uint8 Transceiver, CanTrcv_TrcvWakeupModeType TrcvWakeupMode);

#if (CANTRCV_WAKEUP_BY_POLLING == CANTRCV_WAKEUP_SUPPORT)
/**
 * This is the main function of CanTrcv, which scans all busses for wake up events and perform these event.
 * Service ID: 0x06
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */

extern FUNC(void, CANTRCV_CODE) CanTrcv_MainFunction(void);

#endif /* CANTRCV_WAKEUP_BY_POLLING == CANTRCV_WAKEUP_SUPPORT */

/**
 * This function is called by underlying CANIF in case a wake up interrupt is detected.
 * Service ID: 0x07
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: API call has been accepted
 *               E_NOT_OK: API call has not been accepted.
 *
 */

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_CheckWakeup(uint8 Transceiver);

#if (STD_ON == CANTRCV_BUS_ERR_FLAG)
/**
 * This function reads the transceiver diagnostic status periodically and sets product/development accordingly.
 * Service ID: 0x08
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */

extern FUNC(void, CANTRCV_CODE) CanTrcv_MainFunctionDiagnostics(void);

#endif /* STD_ON == CANTRCV_BUS_ERR_FLAG */

/**
 * This function de-initializes the CanTrcv module.
 * Service ID: 0x10
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */

extern FUNC(void, CANTRCV_CODE) CanTrcv_DeInit(void);

#if (STD_ON == CANTRCV_HWPN_SUPPORT)

/**
 * This function reads the transceiver configuration/status data and returns
 * it through parameter TrcvSysData. This API shall exist only if CanTrcvHwPnSupport = TRUE.
 * Service ID: 0x09
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): TrcvSysData: Configuration/Status data of the transceiver.
 * Return value: E_OK: the transceiver status is successfully read.
 *               E_NOT_OK: the transceiver status data is not available or a development error occurs.
 *
 */

extern FUNC(Std_ReturnType, CANTRCV_CODE)
    CanTrcv_GetTrcvSystemData(uint8 Transceiver, P2VAR(uint32, AUTOMATIC, CANTRCV_CONST) TrcvSysData);

/**
 * This function clears the WUF flag in the transceiver hardware.
 * Service ID: 0x0a
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant for different transceivers
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: the WUF flag has been cleared.
 *               E_NOT_OK: the WUF flag has not been cleared or a development error occurs.
 *
 */

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_ClearTrcvWufFlag(uint8 Transceiver);

/**
 * This function reads the status of the timeout flag from the transceiver hardware.
 * Service ID: 0x0b
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): FlagState: State of the timeout flag.
 * Return value: E_OK: status of the timeout flag is success-fully read.
 *               E_NOT_OK: status of the timeout flag could not be read.
 *
 */

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_ReadTrcvTimeoutFlag(
    uint8 Transceiver,
    P2VAR(CanTrcv_TrcvFlagStateType, AUTOMATIC, CANTRCV_APPL_DATA) FlagState);

/**
 * This function clears the status of the timeout flag in the transceiver hardware.
 * Service ID: 0x0c
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: the timeout flag is successfully cleared.
 *               E_NOT_OK: the timeout flag could not be cleared.
 *
 */

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_ClearTrcvTimeoutFlag(uint8 Transceiver);

/**
 * This function reads the status of the silence flag from the transceiver hardware.
 * Service ID: 0x0d
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): FlagState: State of the silence flag.
 * Return value: E_OK: status of the silence flag is success-fully read.
 *               E_NOT_OK: status of the silence flag could not be read.
 *
 */

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_ReadTrcvSilenceFlag(
    uint8 Transceiver,
    P2VAR(CanTrcv_TrcvFlagStateType, AUTOMATIC, CANTRCV_APPL_DATA) FlagState);

/**
 * This function requests to check the status of the wakeup flag from the transceiver hardware.
 * Service ID: 0x0e
 * Sync/Async: Asynchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: the request for checking the wakeup flag has been accepted.
 *               E_NOT_OK: the request for checking the wakeup flag has not been accepted.
 *
 */

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_CheckWakeFlag(uint8 Transceiver);

/**
 * This function configures the wake-up of the transceiver for Standby and Sleep Mode.
 * Service ID: 0x0f
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): ActivationState: PN_ENABLED: PN wakeup functionality in CanTrcv shall be enabled.
 *                                  PN_DISABLED: PN wakeup functionality in CanTrcv shall be disabled.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: the PN has been changed to the requested configuration.
 *               E_NOT_OK: the PN configuration change has failed. The previous configuration has not been changed.
 *
 */

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_SetPNActivationState(CanTrcv_PNActivationType ActivationState);

#endif /* STD_ON == CANTRCV_HWPNSUPPORT */

#endif /* CANTRCV_H_ */
