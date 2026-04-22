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
/*********************************************************************************
**                                                                            **
**  FILENAME    : LinIf.h                                                     **
**                                                                            **
**  Created on  :                                                             **
**  Author      : HuRongbo                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION : API declaration and type definitions of LinIf               **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:LinIf<br>
  RuleSorce:puhua_rules-2024.12.rcf

  \li VL_LinIf_Division .<br>
    Reason:When the scheduling table has no entries, the modulo operation will not be executed.
    Risk: No risk.
    Prevention: Ensure that the project is working properly through unit testing.

  \li VL_LinIf_PointerArithmetic .<br>
    Reason:The offending items are array pointer operations,no effect and reserved.
    Risk: No risk.
    Prevention: Ensure that the project is working properly through unit testing.

  \li VL_LinIf_MacrosFunctionUsed
    Reason: Function-like macros are used to allow more efficient code.
    Risk: No risk.
    Prevention: Ensure that the project is working properly through unit testing.

  \li VL_LinIf_ConstPointer .<br>
    Reason: Due to the macro being enabled, the pointer was not modified.
    Risk: No risk.
    Prevention: Ensure that the project is working properly through unit testing.

  \li VL_LinIf_3219 .<br>
    Reason: The static function has been called in other functions.
    Risk: No risk.
    Prevention: Ensure that the project is working properly through unit testing.

  \li VL_LinIf_1504 .<br>
    Reason: This variable is used in multiple files.
    Risk: No risk.
    Prevention: Ensure that the project is working properly through unit testing.

  \li VL_LinIf_1505
    Reason: Functions defined out are not used in the project and have no impact on the integration application.
    Risk: No risk.
    Prevention: Ensure that the project is working properly through unit testing.

  \li VL_LinIf_1502
    Reason: The configuration struct is used by other modules.
    Risk: No risk.
    Prevention: Ensure that the project is working properly through unit testing.

  \li VL_LinIf_3415
    Reason: Functional design requires, and it is necessary under different configurations.
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

  \li VL_LinIf_OnlyAccess .<br>
    Reason: file scoped variables only used in one function are necessary for functional usage
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

  \li VL_MTR_LinIf_STMIF .<br>
    Reason: There is a lot of non-generic code, allowing for higher nesting levels to be accepted.
    Risk: The code is difficult to maintain.
    Prevention: Design and code review + clear structure and well-commented code.

  \li VL_MTR_LinIf_STST3 .<br>
    Reason: Splitting their implementation will result in a decrease in performance.
    Risk: The code is difficult to maintain.
    Prevention: Design and code review + clear structure and well-commented code.

  \li VL_MTR_LinIf_STCAL .<br>
    Reason: Reducing the number of function calls will increase the nesting depth and cyclomatic complexity.
    Risk: The code is difficult to maintain.
    Prevention: Design and code review + clear structure and well-commented code.

  \li VL_MTR_LinIf_STM19 .<br>
    Reason: Multiple exit points can improve function execution efficiency.
    Risk: The code is difficult to maintain.
    Prevention: Design and code review + clear structure and well-commented code.

  \li VL_MTR_LinIf_STCYC
    Reason: The function needs to ensure the integrity of its functionality.
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

  \li VL_MTR_LinIf_STLIN .<br>
    Reason: Splitting them into smaller parts will lead to performance degradation.
    Risk: The code is difficult to maintain.
    Prevention: Design and code review + clear structure and well-commented code.

  \li VL_LinIf_Logical .<br>
    Reason: The condition will be modified within the loop.
    Risk: No risk.
    Prevention: Ensure that the project is working properly through unit testing.

  \li VL_LinIf_3415
    Reason: Functional design requires, and it is necessary under different configurations.
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

  \li VL_LinIf_3449
    Reason: Special code design of the SchM.
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

  \li VL_LinIf_3451
    Reason: Special code design of the SchM.
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

  \li VL_LinIf_0674
    Reason: The parameters of the Macl interface do not conform to the specifications.
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.
*/

#ifndef LINIF_H
#define LINIF_H

#include "LinIf_Types.h"
#if (LINIF_TRCV_DRIVER_SUPPORTED == STD_ON)
#include "LinTrcv.h"
#endif
#if (LINIF_TP_SUPPORTED == STD_ON)
#include "LinTp.h"
#endif

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/*Published information elements*/
#define LINIF_MODULE_ID                     62U
#define LINIF_VENDOR_ID                     62U
#define LINIF_H_AR_RELEASE_MAJOR_VERSION    4U
#define LINIF_H_AR_RELEASE_MINOR_VERSION    5U
#define LINIF_H_AR_RELEASE_REVISION_VERSION 0U

#define LINIF_H_SW_MAJOR_VERSION 2U
#define LINIF_H_SW_MINOR_VERSION 1U
#define LINIF_H_SW_PATCH_VERSION 11U

/*instance id*/
#define LINIF_INSTANCE_ID 0x00u

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
/*Development errors used by the LinIf module*/
#define LINIF_E_UNINIT                 0x00u
#define LINIF_E_INIT_FAILED            0x10u
#define LINIF_E_NONEXISTENT_CHANNEL    0x20u
#define LINIF_E_PARAMETER              0x30u
#define LINIF_E_PARAM_POINTER          0x40u
#define LINIF_E_SCHEDULE_REQUEST_ERROR 0x51u
#define LINIF_E_TRCV_INV_MODE          0x53u
#define LINIF_E_TRCV_NOT_NORMAL        0x54u
#define LINIF_E_PARAM_WAKEUPSOURCE     0x55u
#endif /*LINIF_DEV_ERROR_DETECT == STD_ON*/

/* Runtime errors */
#define LINIF_E_RESPONSE       0x60u
#define LINIF_E_NC_NO_RESPONSE 0x61u

/*service id*/
#define LINIF_INIT_ID                0x01u
#define LINIF_GETVERSIONINFO_ID      0x03u
#define LINIF_TRANSMIT_ID            0x04u
#define LINIF_SCHEDULEREQUEST_ID     0x05u
#define LINIF_GOTOSLEEP_ID           0x06u
#define LINIF_WAKEUP_ID              0x07u
#define LINIF_SETTRCVMODE_ID         0x08u
#define LINIF_GETTRCVMODE_ID         0x09u
#define LINIF_GETTRCVWAKEUPREASON_ID 0x0Au
#define LINIF_SETTRCVWAKEUPREASON_ID 0x0Bu
#define LINIF_CANCELTRANSMIT_ID      0x0Cu
#define LINIF_CHECKWAKEUP_ID         0x60u
#define LINIF_WAKEUPCONFIRMATION_ID  0x61u
#define LINIF_MAINFUNCTION_ID        0x80u
#define LINIF_GETPIDTABLE_ID         0x72u
#define LINIF_SETPIDTABLE_ID         0x73u
#define LINIF_GETCONFIGUREDNAD_ID    0x70u
#define LINIF_SETCONFIGUREDNAD_ID    0x71u
#define LINIF_HEADERINDICATION_ID    0x78u
#define LINIF_RXINDICATION_ID        0x79u
#define LINIF_TXCONFIRMATION_ID      0x7au
#define LINIF_LINERRORINDICATION_ID  0x7bu
#define LINIF_NOUSED(paramter)       (void)(paramter)
/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/

extern CONST(LinIf_LinDriverApiType, LINIF_CONST) Lin_DriverApi[LINIF_NUMBER_OF_DRIVERS];

#if (LINIF_TRCV_DRIVER_SUPPORTED == STD_ON)
extern CONST(LinIf_TrcvDriverApiType, LINIF_CONST) LinTrcv_DriverApi[];
#endif

extern CONST(LinIf_ConfigType, LINIF_CONST) LinIf_PCConfig;
/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

/* PRQA S 3432 ++ */ /* VL_LinIf_MacrosFunctionUsed */

/******************************************************************************/
/*
 * Brief               Initializes the LIN Interface.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ConfigPtr: Pointer to the LIN Interface configuration
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
extern FUNC(void, LINIF_CODE) LinIf_Init(P2CONST(LinIf_ConfigType, AUTOMATIC, LINIF_APPL_CONST) ConfigPtr);

/******************************************************************************/
/*
 * Brief               Returns the version information of this module.
 * ServiceId           0x03
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[in/out]  None
 * Param-Name[out]     versioninfo: Pointer to where to store the version
 *                                  information of this module.
 * Return              None
 */
/******************************************************************************/
#if (STD_ON == LINIF_VERSION_INFO_API)

#if (STD_ON == LINIF_DEV_ERROR_DETECT)
#define LinIf_GetVersionInfo(versionInfo)                                                                        \
    do                                                                                                           \
    {                                                                                                            \
        if (NULL_PTR == (versionInfo))                                                                           \
        {                                                                                                        \
            Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID, LINIF_GETVERSIONINFO_ID, LINIF_E_PARAM_POINTER); \
        }                                                                                                        \
        else                                                                                                     \
        {                                                                                                        \
            (versionInfo)->vendorID         = LINIF_VENDOR_ID;                                                   \
            (versionInfo)->moduleID         = LINIF_MODULE_ID;                                                   \
            (versionInfo)->sw_major_version = LINIF_H_SW_MAJOR_VERSION;                                          \
            (versionInfo)->sw_minor_version = LINIF_H_SW_MINOR_VERSION;                                          \
            (versionInfo)->sw_patch_version = LINIF_H_SW_PATCH_VERSION;                                          \
        }                                                                                                        \
    } while (0)
#else
#define LinIf_GetVersionInfo(versionInfo)                           \
    do                                                              \
    {                                                               \
        (versionInfo)->vendorID         = LINIF_VENDOR_ID;          \
        (versionInfo)->moduleID         = LINIF_MODULE_ID;          \
        (versionInfo)->sw_major_version = LINIF_H_SW_MAJOR_VERSION; \
        (versionInfo)->sw_minor_version = LINIF_H_SW_MINOR_VERSION; \
        (versionInfo)->sw_patch_version = LINIF_H_SW_PATCH_VERSION; \
    } while (0)
#endif

#endif

/******************************************************************************/
/*
 * Brief               Indicates a request.
 * ServiceId           0x49
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      LinTxPduId: Upper layer identification of the LIN frame
 *                                 to be transmitted (not the LIN protected ID).
 *                     PduInfoPtr: This parameter is not used by this call.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              E_OK:     Transmit request has been accepted.
 *                     E_NOT_OK: Transmit request has not been accepted
 *
 */
/******************************************************************************/
/* PRQA S 3449,3451 ++ */ /* VL_LinIf_3449,VL_LinIf_3451 */
extern FUNC(Std_ReturnType, LINIF_CODE)
    LinIf_Transmit(PduIdType LinTxPduId, P2CONST(PduInfoType, AUTOMATIC, LINIF_APPL_CONST) PduInfoPtr);
/* PRQA S 3449,3451 -- */

/******************************************************************************/
/*
 * Brief               Requests a schedule table to be executed.
 * ServiceId           0x05
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Channel:  Channel index.
 *                     Schedule: Identification of the new schedule to be set.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              E_OK:     Schedule table request has been accepted.
 *                     E_NOT_OK: Schedule table switch request has not been accepted.
 *
 */
/******************************************************************************/
extern FUNC(Std_ReturnType, LINIF_CODE)
    LinIf_ScheduleRequest(NetworkHandleType Channel, LinIf_SchHandleType ScheduleId);

/******************************************************************************/
/*
 * Brief               Initiates a transition into the Sleep Mode on the
 *                     selected channel.
 * ServiceId           0x06
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Channel:  Identification of the LIN channel.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              E_OK:     Request to go to sleep has been accepted or sleep
 *                               transition is already in progress or controller
 *                               is already in sleep state.
 *                     E_NOT_OK: Request to go to sleep has not been accepted.
 *
 */
/******************************************************************************/
extern FUNC(Std_ReturnType, LINIF_CODE) LinIf_GotoSleep(NetworkHandleType Channel);

/******************************************************************************/
/*
 * Brief               Initiates the wake up process.
 * ServiceId           0x07
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Channel:  Identification of the LIN channel.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              E_OK:     Request to wake up has been accepted or the
 *                               controller is not in sleep state.
 *                     E_NOT_OK: Request to wake up has not been accepted.
 *
 */
/******************************************************************************/
extern FUNC(Std_ReturnType, LINIF_CODE) LinIf_Wakeup(NetworkHandleType Channel);

#if (LINIF_TRCV_DRIVER_SUPPORTED == STD_ON)
/******************************************************************************/
/*
 * Brief               Set the given LIN transceiver to the given mode.
 * ServiceId           0x08
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Channel:  Identification of the LIN channel.
 *                     TransceiverMode: Requested mode transition.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              E_OK:     The transceiver state has been changed to the
 *                               requested mode.
 *                     E_NOT_OK: The transceiver state change has failed or
 *                               the parameter is out of the allowed range.
 *
 */
/******************************************************************************/
extern FUNC(Std_ReturnType, LINIF_CODE)
    LinIf_SetTrcvMode(NetworkHandleType Channel, LinTrcv_TrcvModeType TransceiverMode);

/******************************************************************************/
/*
 * Brief               Returns the actual state of a LIN Transceiver Driver.
 * ServiceId           0x09
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Channel:  Identification of the LIN channel.
 * Param-Name[in/out]  None
 * Param-Name[out]     TransceiverModePtr:  Pointer to a memory location where
 *                                          output value will be stored.
 * Return              E_OK:     The call of the LIN Transceiver Driver's API
 *                               service has returned E_OK.
 *                     E_NOT_OK: The call of the LIN Transceiver Driver's API
 *                               service has returned E_NOT_OK or channel
 *                               parameter is invalid or pointer is NULL.
 *
 */
/******************************************************************************/
extern FUNC(Std_ReturnType, LINIF_CODE) LinIf_GetTrcvMode(
    NetworkHandleType Channel,
    P2VAR(LinTrcv_TrcvModeType, AUTOMATIC, LINIF_APPL_DATA) TransceiverModePtr);

/******************************************************************************/
/*
 * Brief               Returns the reason for the wake up that has been detected
 *                     by the LIN Transceiver Driver.
 * ServiceId           0x0a
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Channel:  Identification of the LIN channel.
 * Param-Name[in/out]  None
 * Param-Name[out]     TrcvWuReasonPtr: Pointer to a memory location where output
 *                                      value will be stored.
 * Return              E_OK:     The call of the LIN Transceiver Driver's API
 *                               service has returned E_OK.
 *                     E_NOT_OK: The call of the LIN Transceiver Driver's API
 *                               service has returned E_NOT_OK or channel
 *                               parameter is invalid or pointer is NULL.
 *
 */
/******************************************************************************/
extern FUNC(Std_ReturnType, LINIF_CODE) LinIf_GetTrcvWakeupReason(
    NetworkHandleType Channel,
    P2VAR(LinTrcv_TrcvWakeupReasonType, AUTOMATIC, LINIF_APPL_DATA) TrcvWuReasonPtr);

/******************************************************************************/
/*
 * Brief               This API enables, disables and clears the notification
 *                     for wakeup events on the addressed network.
 * ServiceId           0x0b
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Channel:           Identification of the LIN channel.
 *                     LinTrcvWakeupMode: Requested transceiver wake up reason.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              E_OK:     The call of the LIN Transceiver Driver's API
 *                               service has returned E_OK.
 *                     E_NOT_OK: The call of the LIN Transceiver Driver's API
 *                               service has returned E_NOT_OK or channel or
 *                               mode parameter is invalid.
 *
 */
/******************************************************************************/
extern FUNC(Std_ReturnType, LINIF_CODE)
    LinIf_SetTrcvWakeupMode(NetworkHandleType Channel, LinTrcv_TrcvWakeupModeType LinTrcvWakeupMode);
#endif

extern void LinIf_ChannelMainFunction(NetworkHandleType ch);

#if (LINIF_WAKEUP_SUPPORT == STD_ON)
/******************************************************************************/
/*
 * Brief               Will be called when the EcuM has been notified about a
 *                     wakeup on a specific LIN channel.
 * ServiceId           0x60
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      WakeupSource: Source device, which initiated the wakeup
 *                                   event: LIN controller or LIN transceiver.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              E_OK: No error has occurred during execution of the API.
 *                     E_NOT_OK: An error has occurred during execution of the
 *                               API or invalid WakeupSource.
 *
 */
/******************************************************************************/
extern FUNC(Std_ReturnType, LINIF_CODE) LinIf_CheckWakeup(EcuM_WakeupSourceType WakeupSource);
#endif

#if (STD_ON == LINIF_SLAVE_SUPPORT)
/******************************************************************************/
/*
 * Brief               Retrieves all assigned PID values. The order is congruent
 *                     to the LIN frame index.
 *                     Only applicable for LIN slave nodes.
 * ServiceId           0x72
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Channel: Identification of the LIN channel.
 * Param-Name[in/out]  PidBuffer: Pointer to existing buffer to which the
 *                                current assigned PID values are copied to.
 *                     PidBufferLength: Pointer to actual length of provided
 *                                      buffer. After successful return,it
 *                                      contains the number of copied PID values
 * Param-Name[out]     None
 * Return              E_OK: Request has been accepted.
 *                     E_NOT_OK: Request has not been accepted, development or
 *                               production error occurred.
 */
/******************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE)
LinIf_GetPIDTable(
    NetworkHandleType Channel,
    P2VAR(Lin_FramePidType, AUTOMATIC, LINIF_APPL_DATA) PidBuffer,
    P2VAR(uint8, AUTOMATIC, LINIF_APPL_DATA) PidBufferLength);

/******************************************************************************/
/*
 * Brief               Sets all assigned PID values. The order is congruent to
 *                     the LIN frame index.
 *                     Only applicable for LIN slave nodes.
 * ServiceId           0x73
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Channel: Identification of the LIN channel.
 *                     PidBuffer: Pointer to buffer which contains the PID
 *                                values to configure.
 *                     PidBufferLength: Number of PID values in the provided
 *                                      buffer.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              E_OK: Request has been accepted.
 *                     E_NOT_OK: Request has not been accepted, development or
 *                               production error occurred.
 */
/******************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE)
LinIf_SetPIDTable(
    NetworkHandleType Channel,
    P2VAR(Lin_FramePidType, AUTOMATIC, LINIF_APPL_DATA) PidBuffer,
    uint8 PidBufferLength);
/******************************************************************************/
/*
 * Brief               Reports the current configured NAD.
 *                     Only applicable for LIN slave nodes.
 * ServiceId           0x70
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Channel: Identification of the LIN channel.
 * Param-Name[in/out]  None
 * Param-Name[out]     Nad: Configured NAD of slave.
 * Return              E_OK: Request has been accepted.
 *                     E_NOT_OK: Request has not been accepted, development or
 *                               production error occurred.
 */
/******************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE)
LinIf_GetConfiguredNAD(NetworkHandleType Channel, P2VAR(uint8, AUTOMATIC, LINIF_APPL_DATA) Nad);

/******************************************************************************/
/*
 * Brief               Sets the current configured NAD.
 *                     Only applicable for LIN slave nodes.
 * ServiceId           0x71
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Channel: Identification of the LIN channel.
 *                     Nad: Configured NAD to set as new slave NAD.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              E_OK: Request has been accepted.
 *                     E_NOT_OK: Request has not been accepted, development or
 *                               production error occurred.
 */
/******************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_SetConfiguredNAD(NetworkHandleType Channel, uint8 Nad);

#endif

/* PRQA S 3432 -- */

#endif
