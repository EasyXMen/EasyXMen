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
 **  @file               : LinIf.h
 **  @author             : HuRongbo
 **  @date               :
 **  @vendor             : isoft
 **  @description        : API declaration and type definitions of LinIf
 **
 ***********************************************************************************************************************/

/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author       Description
 *  ----------------------------------------------------------------------------------------------------------------
 *  V01.00.00 2017-12-01  HuRongbo   Initial version
 *  V02.00.00 2020-08-14  HuRongbo   Update for R19-11
 *  V02.00.01 2021-05-17  HuRongbo   Bug fix
 *            2022-07-11  HuRongbo   QAC check issue fix
 *            2022-09-08  HuRongbo   1.Change LINIF_E_RESPONSE to runtime error
 *  V02.00.02 2022-07-25  HuRongbo   1.Fix bug new NC request can not cancel on going Diag service (JIRA:PRE0012022-49)
 *  V02.00.03 2022-09-21  HuRongbo   1.Delete Det check in LinIf_MainFunction() (JIRA:PRD0032020-219)
 *            2022-12-08  ZhaoTong   2.Change event-trigger frame notify the upper layer through the associated
 *                                     unconditional frame PDU
 *  V02.00.04 2022-11-14  HuRongbo   1.Fix incorrect parameter passing in LinIf_GetPIDTable() (JIRA: PRD0032020-234)
 *            2022-12-08  ZhaoTong   2.Change member LinIfFrameId to LinIfProtectedId
 *            2022-12-26  ZhaoTong   3.Fix incorrect lin channel parameter passing in LIN_DRIVER_API()
 *                                     (JIRA:PRD0042022-509)
 *  V02.00.05 2022-12-26  ZhaoTong   1.Fix incorrect lin channel parameter passing in LIN_DRIVER_API()
 *                                     (JIRA:PRD0042022-509)
 *            2023-03-14  HuRongbo   2.Modify the LinIf_SlaveWakeup() function to handle an exception when a wakeup
 *                                     request is received in LINIF_CHANNEL_GOTO_SLEEP state
 *                                   3.BusIdleTimer only running in LINIF_CHANNEL_OPERATIONAL state, Stop BusIdleTimer
 *                                     when enters BusSleepState
 *                                   4.Clear waitWakeupConfirmFlag to FALSE in
 *                                     LinIf_SlaveHeaderIndication()(JIRA:PRD0042022-590)
 *  V02.00.06 2023-09-15  ZhaoTong   1.Bug fix CPT-6932. It's necessary to add a macro switch When referencing the
 *                                     Master/Slave header file.
 *                                   2.Adjust header file references.
 *  V02.00.07 2023-09-20  ZhaoTong   1.Bug fix CPT-6946.add two macro for Master/Slave of TP separately.
 *  V02.00.08 2023-10-07  ZhaoTong     Adaptation of different AutoSar versions of lin driver, currently supporting
 *                                     versions 4.2.2, 4.3.1, 4.4.0
 *  V02.00.09 2023-11-28  ZhaoTong   1.Compile warning clean.(CPT-7666)
 *                                   2.Delete dummy API LinIf_CancelTransmit.(JIRA:CPT-7688)
 *  V02.01.00 2024-01-05  ZhaoTong     Muliticore adapt
 *  V02.01.01 2024-06-25  ZhaoTong   fix bug CPT-9224 The uncondicational frame associated with an event triggered frame
 *                                   shall resever the first data field its PID(anyway)
 *  V02.01.02 2024-07-05  ZhaoTong   Add macro condition for the call to API BswM_LinTp_RequestMode.
 *  V02.02.00 2024-12-05  ZouZhiJia  R23-11 development first release.
 *  V02.03.00 2025-04-29   ZouZhiJia  Optimize Code.
 *  V02.03.01 2025-10-16   ZouZhijia  fix bug
 CPT-15176,CPT-15536,CPT-15645,CPT-15222,CPT-15176,CPT-15674,CPT-15681,CPT-15682
 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:LinIf<br>
  RuleSorce:puhua_rules-2024.12.rcf

    \li PRQA S 2834 VL_LinIf_Division .<br>
    Reason:When the scheduling table has no entries, the modulo operation will not be executed.
    Risk: No risk.
    Prevention: Ensure that the project is working properly through unit testing.

    \li PRQA S 3225 VL_LinIf_ParamUseAutoObj .<br>
    Reason:The buffer created and used within the function has no effect outside of the function.
    Risk: No risk.
    Prevention: Ensure that the project is working properly through unit testing.

    \li PRQA S 0488 VL_LinIf_PointerArithmetic .<br>
    Reason:The offending items are array pointer operations,no effect and reserved.
    Risk: No risk.
    Prevention: Ensure that the project is working properly through unit testing.

    \li PRQA S 0770 VL_LinIf_Continue .<br>
    Reason: In order to reducing function complexity and improving readability, therefore the continue keyword is used.
    Risk: No risk.
    Prevention: Ensure that the project is working properly through unit testing.

    \li  PRQA S 2991,2995 VL_LinIf_Logical .<br>
    Reason: The condition will be modified within the loop.
    Risk: No risk.
    Prevention: Ensure that the project is working properly through unit testing.

    \li PRQA S 3334 VL_LinIf_3334 .<br>
    Reason: The function parameters defined in the specification same as a global function name
    Risk: No risk.
    Prevention: Ensure that the project is working properly through unit testing.

    \li PRQA S 0674 VL_LinIf_McalConflict .<br>
    Reason: The Mcal implementation does not comply with the specifications.
    Risk: No risk.
    Prevention: Ensure that the project is working properly through unit testing.

    \li PRQA S 3673 VL_LinIf_ConstPointer .<br>
    Reason: Due to the macro being enabled, the pointer was not modified.
    Risk: No risk.
    Prevention: Ensure that the project is working properly through unit testing.

    \li PRQA S 6030 VL_MTR_LinIf_STMIF .<br>
    Reason: There is a lot of non-generic code, allowing for higher nesting levels to be accepted.
    Risk: The code is difficult to maintain.
    Prevention: Design and code review + clear structure and well-commented code.

    \li PRQA S 6070 VL_MTR_LinIf_STCAL .<br>
    Reason: Reducing the number of function calls will increase the nesting depth and cyclomatic complexity.
    Risk: The code is difficult to maintain.
    Prevention: Design and code review + clear structure and well-commented code.

    \li PRQA S 6060 VL_MTR_LinIf_STM19 .<br>
    Reason: Multiple exit points can improve function execution efficiency.
    Risk: The code is difficult to maintain.
    Prevention: Design and code review + clear structure and well-commented code.

    \li PRQA S 6010 VL_MTR_LinIf_STCYC  .<br>
    Reason: Solving the cyclomatic complexity will affect the complete logic of the code.
    Risk: The code is difficult to maintain.
    Prevention: Design and code review + clear structure and well-commented code.

    \li PRQA S 3415 VL_LinIf_3415 .<br>
    Reason: The LinIf module guarantees the correctness of the right-hand operand in '&&' or '||'.
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

    \li PRQA S 1505 VL_LinIf_1505
    Reason: Functions defined out are not used in the project and have no impact on the integration application.
    Risk: No impact on integrated functions.
    Prevention: Provided to the application to use, the application use need to use the corresponding interface.

    \li PRQA S 1536 VL_LinIf_1536
    Reason: The tag '%1s' is declared but not used within this project.
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

    \li VL_LinIf_1756
      Reason: The issue shall be avoided in actual running environment and configs.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.
*/

/* PRQA S 1756 EOF */ /* VL_LinIf_1756 */
#ifndef LINIF_H_
#define LINIF_H_

/* =================================================== inclusions =================================================== */
#include "LinIf_Cfg.h"
#include "LinIf_PBcfg.h"
#if (LINIF_TRCV_DRIVER_SUPPORTED == STD_ON)
#include "LinTrcv.h"
#endif
#if (LINIF_TP_SUPPORTED == STD_ON)
#include "LinTp_PBcfg.h"
#endif

/* ===================================================== macros ===================================================== */
/*Published information elements*/
#define LINIF_MODULE_ID                   62U
#define LINIF_VENDOR_ID                   62U
#define LINIF_AR_RELEASE_MAJOR_VERSION    4U
#define LINIF_AR_RELEASE_MINOR_VERSION    9U
#define LINIF_AR_RELEASE_REVISION_VERSION 0U

#define LINIF_SW_MAJOR_VERSION 2U
#define LINIF_SW_MINOR_VERSION 3U
#define LINIF_SW_PATCH_VERSION 1U

/*instance id*/
#define LINIF_INSTANCE_ID (uint8)0x00u

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
/*Development errors used by the LinIf module*/
#define LINIF_E_UNINIT                 (uint8)0x00u
#define LINIF_E_INIT_FAILED            (uint8)0x10u
#define LINIF_E_NONEXISTENT_CHANNEL    (uint8)0x20u
#define LINIF_E_PARAMETER              (uint8)0x30u
#define LINIF_E_PARAM_POINTER          (uint8)0x40u
#define LINIF_E_SCHEDULE_REQUEST_ERROR (uint8)0x51u
#define LINIF_E_TRCV_INV_MODE          (uint8)0x53u
#define LINIF_E_TRCV_NOT_NORMAL        (uint8)0x54u
#define LINIF_E_PARAM_WAKEUPSOURCE     (uint8)0x55u
#define LINIF_E_ERROR_PARTITION        (uint8)0x70u
#endif

/* Runtime errors */
#define LINIF_E_RESPONSE       (uint8)0x60u
#define LINIF_E_NC_NO_RESPONSE (uint8)0x61u

/*service id*/
#define LINIF_INIT_ID                (uint8)0x01u
#define LINIF_GETVERSIONINFO_ID      (uint8)0x03u
#define LINIF_TRANSMIT_ID            (uint8)0x04u
#define LINIF_SCHEDULEREQUEST_ID     (uint8)0x05u
#define LINIF_GOTOSLEEP_ID           (uint8)0x06u
#define LINIF_WAKEUP_ID              (uint8)0x07u
#define LINIF_SETTRCVMODE_ID         (uint8)0x08u
#define LINIF_GETTRCVMODE_ID         (uint8)0x09u
#define LINIF_GETTRCVWAKEUPREASON_ID (uint8)0x0Au
#define LINIF_SETTRCVWAKEUPREASON_ID (uint8)0x0Bu
#define LINIF_CANCELTRANSMIT_ID      (uint8)0x0Cu
#define LINIF_CHECKWAKEUP_ID         (uint8)0x60u
#define LINIF_WAKEUPCONFIRMATION_ID  (uint8)0x61u
#define LINIF_MAINFUNCTION_ID        (uint8)0x80u
#define LINIF_GETPIDTABLE_ID         (uint8)0x72u
#define LINIF_SETPIDTABLE_ID         (uint8)0x73u
#define LINIF_GETCONFIGUREDNAD_ID    (uint8)0x70u
#define LINIF_SETCONFIGUREDNAD_ID    (uint8)0x71u
#define LINIF_HEADERINDICATION_ID    (uint8)0x78u
#define LINIF_RXINDICATION_ID        (uint8)0x79u
#define LINIF_TXCONFIRMATION_ID      (uint8)0x7au
#define LINIF_LINERRORINDICATION_ID  (uint8)0x7bu
#define LINIF_NOUSED(paramter)       (void)(paramter)
/* ================================================ type definitions ================================================ */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

/**
 * @brief              Initializes the LIN Interface.
 * @param[in]          ConfigPtr: Pointer to the LIN Interface configuration
 * @reentrant          FALSE
 * @synchronous        TRUE
 * @trace              CPD-56870 */
extern void LinIf_Init(const LinIf_ConfigType* ConfigPtr);

/**
 * @brief              Returns the version information of this module.
 * @param[out]         versioninfo: Pointer to where to store the version
 *                                  information of this module.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-56872
 */
#if (STD_ON == LINIF_VERSION_INFO_API)
extern void LinIf_GetVersionInfo(Std_VersionInfoType* versionInfo);
#endif

/**
 * @brief               Indicates a request.
 * @param[in]           LinTxPduId: Upper layer identification of the LIN frame
 *                                 to be transmitted (not the LIN protected ID).
 * @param[in]           PduInfoPtr: This parameter is not used by this call.
 * @return              E_OK:     Transmit request has been accepted.
 * @return              E_NOT_OK: Transmit request has not been accepted
 * @reentrant           FALSE
 * @synchronous         TRUE
 * @trace               CPD-56873
 */
extern Std_ReturnType LinIf_Transmit(PduIdType LinTxPduId, const PduInfoType* PduInfoPtr);

/**
 * @brief               Requests a schedule table to be executed.
 * @param[in]           Channel:  Channel index.
 * @param[in]           Schedule: Identification of the new schedule to be set.
 * @return              E_OK:     Schedule table request has been accepted.
 * @return              E_NOT_OK: Schedule table switch request has not been accepted.
 * @reentrant           FALSE
 * @synchronous         TRUE
 * @trace               CPD-56874
 */
/* PRQA S 3334 ++ */ /* VL_LinIf_3334 */
extern Std_ReturnType LinIf_ScheduleRequest(NetworkHandleType Channel, LinIf_SchHandleType Schedule);
/* PRQA S 3334 -- */

/**
 * @brief               Initiates a transition into the Sleep Mode on the
 *                      selected channel.
 * @param[in]           Channel: Identification of the LIN channel.
 * @return              E_OK:     equest to go to sleep has been accepted or sleep
 *                                transition is already in progress or controller
 *                                is already in sleep state.
 * @return              E_NOT_OK: Request to go to sleep has not been accepted.
 * @reentrant           FALSE
 * @synchronous         TRUE
 * @trace               CPD-56875
 */
extern Std_ReturnType LinIf_GotoSleep(NetworkHandleType Channel);

/**
 * @brief               Initiates the wake up process.
 * @param[in]           Channel:  Identification of the LIN channel.
 * @return              E_OK:     Request to wake up has been accepted or the
 *                                controller is not in sleep state.
 * @return              E_NOT_OK: Request to wake up has not been accepted.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-56876
 */
extern Std_ReturnType LinIf_Wakeup(NetworkHandleType Channel);

#if (LINIF_TRCV_DRIVER_SUPPORTED == STD_ON)
/**
 * @brief               Set the given LIN transceiver to the given mode.
 * @param[in]           Channel:  Identification of the LIN channel.
 * @param[in]           TransceiverMode: Requested mode transition.
 * @return              E_OK:     The transceiver state has been changed to the
 *                                requested mode.
 * @return              E_NOT_OK: The transceiver state change has failed or
 *                                the parameter is out of the allowed range.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-56877
 */
extern Std_ReturnType LinIf_SetTrcvMode(NetworkHandleType Channel, LinTrcv_TrcvModeType TransceiverMode);

/**
 * @brief               Returns the actual state of a LIN Transceiver Driver.
 * @param[in]           Channel:  Identification of the LIN channel.
 * @param[out]          TransceiverModePtr: Pointer to a memory location where
 *                                          output value will be stored.
 * @return              E_OK:    The call of the LIN Transceiver Driver's API
 *                               service has returned E_OK.
 * @return             E_NOT_OK: The call of the LIN Transceiver Driver's API
 *                               service has returned E_NOT_OK or channel
 *                               parameter is invalid or pointer is NULL.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-56878
 */
extern Std_ReturnType LinIf_GetTrcvMode(NetworkHandleType Channel, LinTrcv_TrcvModeType* TransceiverModePtr);

/**
 * @brief              Returns the reason for the wake up that has been detected
 *                     by the LIN Transceiver Driver.
 * @param[in]          Channel: Identification of the LIN channel.
 * @param[out]         TrcvWuReasonPtr: Pointer to a memory location where output
 *                                      value will be stored.
 * @return             E_OK:     The call of the LIN Transceiver Driver's API
 *                               service has returned E_OK.
 * @return             E_NOT_OK: The call of the LIN Transceiver Driver's API
 *                               service has returned E_NOT_OK or channel
 *                               parameter is invalid or pointer is NULL.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-56879
 */
extern Std_ReturnType
    LinIf_GetTrcvWakeupReason(NetworkHandleType Channel, LinTrcv_TrcvWakeupReasonType* TrcvWuReasonPtr);

/**
 * @brief               This API enables, disables and clears the notification
 *                      for wakeup events on the addressed network.
 * @param[in]           Channel:           Identification of the LIN channel.
 * @param[in]           LinTrcvWakeupMode: Requested transceiver wake up reason.
 * @return              E_OK:     The call of the LIN Transceiver Driver's API
 *                               service has returned E_OK.
 * @return              E_NOT_OK: The call of the LIN Transceiver Driver's API
 *                               service has returned E_NOT_OK or channel or
 *                               mode parameter is invalid.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-56880
 */
extern Std_ReturnType LinIf_SetTrcvWakeupMode(NetworkHandleType Channel, LinTrcv_TrcvWakeupModeType LinTrcvWakeupMode);
#endif

/**
 * @brief              LinIf main function
 * @param[in]          ch: Identification of the LIN channel.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
extern void LinIf_ChannelMainFunction(NetworkHandleType ch);

#if (LINIF_WAKEUP_SUPPORT == STD_ON)
/**
 * @brief               Will be called when the EcuM has been notified about a
 *                      wakeup on a specific LIN channel.
 * @param[in]           WakeupSource: Source device, which initiated the wakeup
 *                                   event: LIN controller or LIN transceiver.
 * @return              E_OK: No error has occurred during execution of the API.
 *                      E_NOT_OK: An error has occurred during execution of the
 *                               API or invalid WakeupSource.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-56885
 */
extern Std_ReturnType LinIf_CheckWakeup(EcuM_WakeupSourceType WakeupSource);
#endif

#if (STD_ON == LINIF_SLAVE_SUPPORT)
/**
 * @brief               Retrieves all assigned PID values. The order is congruent
 *                      to the LIN frame index.
 *                      Only applicable for LIN slave nodes.
 * @param[in]           Channel: Identification of the LIN channel.
 * @param[inout]        PidBuffer: Pointer to existing buffer to which the
 *                                 current assigned PID values are copied to.
 * @param[inout]        PidBufferLength: Pointer to actual length of provided
 *                                       buffer. After successful return,it
 *                                       contains the number of copied PID values
 * @return              E_OK: Request has been accepted.
 * @return              E_NOT_OK: Request has not been accepted, development or
 *                                production error occurred.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-56881
 */
Std_ReturnType LinIf_GetPIDTable(NetworkHandleType Channel, Lin_FramePidType* PidBuffer, uint8* PidBufferLength);

/**
 * @brief               Sets all assigned PID values. The order is congruent to
 *                      the LIN frame index.Only applicable for LIN slave nodes.
 * @param[in]           Channel: Identification of the LIN channel.
 * @param[in]           PidBuffer: Pointer to buffer which contains the PID
 *                                 values to configure.
 * @param[in]           PidBufferLength: Number of PID values in the provided
 *                                       buffer.
 * @return              E_OK: Request has been accepted.
 * @return              E_NOT_OK: Request has not been accepted, development or
 *                                production error occurred.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-56882
 */
Std_ReturnType LinIf_SetPIDTable(NetworkHandleType Channel, Lin_FramePidType* PidBuffer, uint8 PidBufferLength);
/**
 * @brief                Reports the current configured NAD.
 *                      Only applicable for LIN slave nodes.
 * @param[in]           Channel: Identification of the LIN channel.
 * @param[out]          Nad: Configured NAD of slave.
 * @return              E_OK: Request has been accepted.
 * @return              E_NOT_OK: Request has not been accepted, development or
 *                               production error occurred.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-56883
 */
Std_ReturnType LinIf_GetConfiguredNAD(NetworkHandleType Channel, uint8* Nad);

/**
 * Brief                Sets the current configured NAD.
 *                      Only applicable for LIN slave nodes.
 * @param[in]           Channel: Identification of the LIN channel.
 * @param[in]           Nad: Configured NAD to set as new slave NAD.
 * @return              E_OK: Request has been accepted.
 * @return              E_NOT_OK: Request has not been accepted, development or
 *                                production error occurred.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-56884
 */
Std_ReturnType LinIf_SetConfiguredNAD(NetworkHandleType Channel, uint8 Nad);

#endif

#endif
