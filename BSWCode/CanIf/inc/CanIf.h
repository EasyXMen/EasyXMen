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
 **  @file               : CanIf.h
 **  @author             : zhengfei.li
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : CanIf header file for external API declarations
 **
 ***********************************************************************************************************************/
/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author          Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V2.0.0    20200217    zhengfei.li     Initial version
 *                                        (Upgrade according to the R19_11 standards)
 *  V2.0.1    20210408    zhengfei.li     Optimize wake-up source detection,
 *                                        Modify the bug of API CanIf_CheckTrcvWakeFlag when the DET is STD_OFF.
 *  V2.1.0    20211217    zhengfei.li     PB/PC configuration parameter split
 *  V2.1.1    20230526    tong.zhao       fix Bug CPT-459
 *                                        Modify the bug of invalid address access in API CanIf_RxIndicationHandle,
 *                                        when meta is not support and ReadRxPdu is support.
 *  V2.1.2    20231009    tong.zhao       fix Bug CPT-7169
 *                                        1> Add condication for CanIf_MetaDataToCanId and CanIf_CanIdToMetaData.
 *                                        2> Change Version  Check.
 *  V2.1.3    20231007    tong.zhao       CPP-419
 *                                        1> Adaptation of different AutoSar versions of can driver, currently
 *                                        supporting versions 4.2.2, 4.3.1, 4.4.0
 *  V2.1.4    20231031    xiaojian.liang  Add feauture CanIfRxPduDataLengthCheck.
 *  V2.1.5    20231116    tong.zhao       CPT-7511
 *                                        1> Modify the bug of Can_IdType may be defined as uint16 in only
 *                                        Standard frame, at AUTOSAR Driver version 422.
 *  V2.1.6    20240401    tong.zhao       fix Bug CPT-8618
 *                                        In order to ensure that ControllerId and TransciverId in CanIf does
 *                                        not rely on CanDrv and CanTransciver index
 *  V2.2.0    20240524    tong.zhao       CPD-33700         Muliticore adapt
 *  V2.2.1    20240606    tong.zhao       CPT-9165
 *                                        Modify exclusivearea in Api CanIf_TransmitHandle.
 *  V2.2.2    20240628    tong.zhao       Optimize flash
 *  V2.2.3    20240705    tong.zhao       Add binary search method for software filtering.
 *            20240705    tong.zhao       Compatible with 16-bit addressable CPU
 *  V2.3.0    20241205    miao.wang       R23-11 development first release.
 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2023 Compliance Exceptions
    ModeName:CanIf<br>
  RuleSource:puhua-rule.rcf 3.0.0

    \li VL_CanIf_2991
      Reason: The control statement has no effect, but is reserved for better readability.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_CanIf_1505
      Reason: In order to make the module code structure clear, the functions are classified.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_CanIf_2842
      Reason: Array index has been checked outside the access function.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_CanIf_5017
      Reason: High cyclomatic complexity is reserved for better readability.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_CanIf_1258
      Reason: The cast conversion from a constant is safe.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_CanIf_3678
      Reason: The variable is modified in some cases.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_CanIf_1502
      Reason: The variable is indirectly accessed by pointer in CanIf_Config.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_CanIf_2905
      Reason: The truncation is used for better readability.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_CanIf_1536
      Reason: The tag '%1s' is declared but not used within this project.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_CanIf_2983
      Reason: The issue shall be avoided in actual running environment and configs.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_CanIf_3472
      Reason: Macro definitions for easy reuse.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_CanIf_3315
      Reason: The issue shall be avoided in actual running environment and configs.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_CanIf_1512
      Reason: The issue shall be avoided in actual running environment and configs.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_CanIf_5004
      Reason: Function Pointers are not used for data copying.
      Risk: No risk.
      Prevention: The reliability of the function is guaranteed by the design.

    \li VL_MTR_CanIf_STVAR
      Reason: The function prototype is defined in AUTOSAR standard.
      Risk: The code is difficult to maintain.
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_MTR_CanIf_STMIF
      Reason: Functions have different scenarios that need to be covered, which depends on the configuration of
              different channels, sdu and other configuration conditions, as well as some precompiled macros.
      Risk: The code is difficult to maintain.
      Prevention: Design and code review, and have a clear structure and annotated code.
 */

/* PRQA S 1513 EOF */ /* VL_QAC_MultiDeclaration */
/* PRQA S 1512 EOF */ /* VL_CanIf_1512 */
#ifndef CANIF_H_
#define CANIF_H_

/* =================================================== inclusions =================================================== */
#include "CanIf_Types.h"
#include "CanIf_CfgTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */
#define CANIF_MODULE_ID                   60u
#define CANIF_VENDOR_ID                   62u
#define CANIF_AR_RELEASE_MAJOR_VERSION    4u
#define CANIF_AR_RELEASE_MINOR_VERSION    9u
#define CANIF_AR_RELEASE_REVISION_VERSION 0u
#define CANIF_SW_MAJOR_VERSION            2u
#define CANIF_SW_MINOR_VERSION            3u
#define CANIF_SW_PATCH_VERSION            0u

/* ===================================================== macros ===================================================== */
#define CANIF_INSTANCE_ID (0u)

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief           This service Initializes internal and external interfaces of the CAN Interface
 *                  for the further processing.
 * @param[in]       ConfigPtr: Pointer to configuration parameter set, used e.g. for post build parameters.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-61392
 */
extern void CanIf_Init(const CanIf_ConfigType* ConfigPtr);

/**
 * @brief           De-initializes the CanIf module.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-61393
 */
extern void CanIf_DeInit(void);

/**
 * @brief           This service calls the corresponding CAN Driver service for changing of
 *                  the CAN controller mode.
 * @param[in]       ControllerId: Abstracted CanIf ControllerId which is assigned to a CAN controller,
 *                  which is requested for mode transition.
 * @param[in]       ControllerMode: Requested mode transition.
 * @return          Std_ReturnType
 * @retval          E_OK: Controller mode request has been accepted.
 * @retval          E_NOT_OK: Controller mode request has not been accepted.
 * @synchronous     FALSE
 * @reentrant       Reentrant (Not for the same controller)
 * @trace           CPD-61394
 */
extern Std_ReturnType CanIf_SetControllerMode(uint8 ControllerId, Can_ControllerStateType ControllerMode);

/**
 * @brief           This service calls the corresponding CAN Driver service for obtaining the current
 *                  status of the CAN controller.
 * @param[in]       ControllerId: Abstracted CanIf ControllerId which is assigned to a CAN controller,
 *                  which is requested for current operation mode.
 * @param[out]      ControllerModePtr: Pointer to a memory location, where the current mode of the
 *                  CAN controller will be stored.
 * @return          Std_ReturnType
 * @retval          E_OK: Controller mode request has been accepted.
 * @retval          E_NOT_OK: Controller mode request has not been accepted.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-61395
 */
extern Std_ReturnType CanIf_GetControllerMode(uint8 ControllerId, Can_ControllerStateType* ControllerModePtr);

/**
 * @brief           Requests transmission of a PDU.
 * @param[in]       TxPduId: Identifier of the PDU to be transmitted.
 * @param[in]       PduInfoPtr: Length of and pointer to the PDU data and pointer to MetaData.
 * @return          Std_ReturnType
 * @retval          E_OK: Transmit request has been accepted.
 * @retval          E_NOT_OK: Transmit request has not been accepted.
 * @synchronous     TRUE
 * @reentrant       Reentrant for different PduIds. Non reentrant for the same PduId.
 * @trace           CPD-61396
 */
extern Std_ReturnType CanIf_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr);

#if (STD_ON == CANIF_PUBLIC_READ_RX_PDU_DATA_API)
/**
 * @brief           This service provides the Data Length and the received data of the requested
 *                  CanIfRxSduId to the calling upper layer.
 * @param[in]       CanIfRxSduId: Receive L-SDU handle specifying the corresponding CAN L-SDU ID and
 *                  implicitly the CAN Driver instance as well as the corresponding CAN controller device.
 * @param[out]      CanIfRxInfoPtr: Contains the length (SduLength) of the received PDU, a pointer to
 *                  a buffer (SduDataPtr) containing the PDU, and the MetaData related to this PDU.
 * @return          Std_ReturnType
 * @retval          E_OK: Request for L-SDU data has been accepted.
 * @retval          E_NOT_OK: No valid data has been received.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-61397
 */
extern Std_ReturnType CanIf_ReadRxPduData(PduIdType CanIfRxSduId, PduInfoType* CanIfRxInfoPtr);
#endif

#if (STD_ON == CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API)
/**
 * @brief           This service returns the confirmation status (confirmation occurred or not) of a
 *                  specific static or dynamic CAN Tx L-PDU, requested by the CanIfTxSduId.
 * @param[in]       CanIfTxSduId: L-SDU handle to be transmitted. This handle specifies the corresponding
 *                  CAN L-SDU ID and implicitly the CAN Driver instance as well as the corresponding
 *                  CAN controller device.
 * @return          CanIf_NotifStatusType: Current confirmation status of the corresponding CAN Tx L-PDU.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-61398
 */
extern CanIf_NotifStatusType CanIf_ReadTxNotifStatus(PduIdType CanIfTxSduId);
#endif

#if (STD_ON == CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API)
/**
 * @brief           This service returns the indication status (indication occurred or not) of
 *                  a specific CAN Rx L-PDU, requested by the CanIfRxSduId.
 * @param[in]       CanIfRxSduId: Receive L-SDU handle specifying the corresponding CAN L-SDU ID and
 *                  implicitly the CAN Driver instance as well as the corresponding CAN controller device.
 * @return          CanIf_NotifStatusType: Current indication status of the corresponding CAN Rx L-PDU.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-61399
 */
extern CanIf_NotifStatusType CanIf_ReadRxNotifStatus(PduIdType CanIfRxSduId);
#endif

/**
 * @brief           This service sets the requested mode at the L-PDUs of a predefined logical PDU channel.
 * @param[in]       ControllerId: All PDUs of the own ECU connected to the corresponding CanIf ControllerId,
 *                  which is assigned to a physical CAN controller are addressed.
 * @param[in]       PduModeRequest: Requested PDU mode change.
 * @return          Std_ReturnType
 * @retval          E_OK: Request for mode transition has been accepted.
 * @retval          E_NOT_OK: Request for mode transition has not been accepted.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-61400
 */
extern Std_ReturnType CanIf_SetPduMode(uint8 ControllerId, CanIf_PduModeType PduModeRequest);

/**
 * @brief           This service reports the current mode of a requested PDU channel.
 * @param[in]       ControllerId: All PDUs of the own ECU connected to the corresponding CanIf
                    ControllerId, which is assigned to a physical CAN controller are addressed.
 * @param[out]      PduModePtr: Pointer to a memory location, where the current mode of the logical
                    PDU channel will be stored.
 * @return          Std_ReturnType
 * @retval          E_OK: PDU mode request has been accepted.
 * @retval          E_NOT_OK: PDU mode request has not been accepted.
 * @synchronous     TRUE
 * @reentrant       Reentrant (Not for the same channel)
 * @trace           CPD-61401
 */
extern Std_ReturnType CanIf_GetPduMode(uint8 ControllerId, CanIf_PduModeType* PduModePtr);

#if (STD_ON == CANIF_PUBLIC_VERSION_INFO_API)
/**
 * @brief           This service returns the version information of the called CAN Interface module.
 * @param[out]      VersionInfo: Pointer to where to store the version information of this module.
 * @synchronous     TRUE
 * @reentrant       Reentrant
 * @trace           CPD-61402
 */
extern void CanIf_GetVersionInfo(Std_VersionInfoType* VersionInfo);
#endif

#if (STD_ON == CANIF_PUBLIC_SET_DYNAMIC_TX_ID_API)
/**
 * @brief           This service reconfigures the corresponding CAN identifier of the requested CAN L-PDU.
 * @param[in]       CanIfTxSduId: L-SDU handle to be transmitted. This handle specifies the corresponding
 *                  CAN L-SDU ID and implicitly the CAN Driver instance as well as the corresponding
 *                  CAN controller device.
 * @param[in]       CanId: Standard/Extended CAN ID of CAN L-SDU that shall be transmitted as FD or
 *                  conventional CAN frame.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-61403
 */
extern void CanIf_SetDynamicTxId(PduIdType CanIfTxSduId, Can_IdType CanId);
#endif

#if (CANIF_TRCV_NUMBER > 0u)
/**
 * @brief           This service changes the operation mode of the tansceiver TransceiverId,
 *                  via calling the corresponding CAN Transceiver Driver service.
 * @param[in]       TransceiverId: Abstracted CanIf TransceiverId, which is assigned to a CAN transceiver,
                    which is requested for mode transition
 * @param[in]       TransceiverMode: Requested mode transition.
 * @return          Std_ReturnType
 * @retval          E_OK: Transceiver mode request has been accepted.
 * @retval          E_NOT_OK: Transceiver mode request has not been accepted.
 * @synchronous     FALSE
 * @reentrant       Non Reentrant
 * @trace           CPD-61404
 */
extern Std_ReturnType CanIf_SetTrcvMode(uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode);

/**
 * @brief           This function invokes CanTrcv_GetOpMode and updates the parameter TransceiverModePtr
 *                  with the value OpMode provided by CanTrcv.
 * @param[in]       TransceiverId: Abstracted CanIf TransceiverId, which is assigned to a CAN transceiver,
 *                  which is requested for current operation mode.
 * @param[out]      TransceiverModePtr: Requested mode of requested network the Transceiver is connected to.
 * @return          Std_ReturnType
 * @retval          E_OK: Transceiver mode request has been accepted.
 * @retval          E_NOT_OK: Transceiver mode request has not been accepted.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-61405
 */
extern Std_ReturnType CanIf_GetTrcvMode(uint8 TransceiverId, CanTrcv_TrcvModeType* TransceiverModePtr);

/**
 * @brief           This service returns the reason for the wake up of the transceiver TransceiverId,
 *                  via calling the corresponding CAN Transceiver Driver service.
 * @param[in]       TransceiverId: Abstracted CanIf TransceiverId, which is assigned to a CAN transceiver,
 *                  which is requested for wake up reason.
 * @param[out]      TrcvWuReasonPtr: Provided pointer to where the requested transceiver wake up reason
 *                  shall be returned.
 * @return          Std_ReturnType
 * @retval          E_OK: Transceiver wake up reason request has been accepted.
 * @retval          E_NOT_OK: Transceiver wake up reason request has not been accepted.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-61406
 */
extern Std_ReturnType CanIf_GetTrcvWakeupReason(uint8 TransceiverId, CanTrcv_TrcvWakeupReasonType* TrcvWuReasonPtr);

/**
 * @brief           This function shall call CanTrcv_SetTrcvWakeupMode.
 * @param[in]       TransceiverId: Abstracted CanIf TransceiverId, which is assigned to a CAN transceiver,
 *                  which is requested for wake up notification mode transition.
 * @param[in]       TrcvWakeupMode: Requested transceiver wake up notification mode.
 * @return          Std_ReturnType
 * @retval          E_OK: Will be returned, if the wake up notifications state has been changed to
 *                  the requested mode.
 * @retval          E_NOT_OK: Will be returned, if the wake up notifications state change has failed or
 *                  the parameter is out of the allowed range. The previous state has not been changed.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-61407
 */
extern Std_ReturnType CanIf_SetTrcvWakeupMode(uint8 TransceiverId, CanTrcv_TrcvWakeupModeType TrcvWakeupMode);
#endif

#if (STD_ON == CANIF_WAKEUP_SUPPORT)
/**
 * @brief           This service checks, whether an underlying CAN driver or a CAN transceiver driver
 *                  already signals a wakeup event.
 * @param[in]       WakeupSource: Source device, which initiated the wake up event: CAN controller
 *                  or CAN transceiver.
 * @return          Std_ReturnType
 * @retval          E_OK: Will be returned, if the check wake up request has been accepted.
 * @retval          E_NOT_OK: Will be returned, if the check wake up request has not been accepted.
 * @synchronous     FALSE
 * @reentrant       Reentrant
 * @trace           CPD-61408
 */
extern Std_ReturnType CanIf_CheckWakeup(EcuM_WakeupSourceType WakeupSource);
#endif

#if (STD_ON == CANIF_PUBLIC_WAKEUP_CHECK_VALID_SUPPORT)
/**
 * @brief           This service is performed to validate a previous wakeup event.
 * @param[in]       WakeupSource: Source device which initiated the wake-up event and which has to
 *                  be validated: CAN controller or CAN transceiver
 * @return          Std_ReturnType
 * @retval          E_OK: Will be returned, if the check validation request has been accepted.
 * @retval          E_NOT_OK: Will be returned, if the check validation request has not been accepted.
 * @synchronous     TRUE
 * @reentrant       Reentrant
 * @trace           CPD-61409
 */
extern Std_ReturnType CanIf_CheckValidation(EcuM_WakeupSourceType WakeupSource);
#endif

#if (STD_ON == CANIF_PUBLIC_TX_CONFIRM_POLLING_SUPPORT)
/**
 * @brief           This service reports, if any TX confirmation has been done for the whole CAN
 *                  controller since the last CAN controller start.
 * @param[in]       ControllerId: Abstracted CanIf ControllerId which is assigned to a CAN controller.
 * @return          CanIf_NotifStatusType: Combined TX confirmation status for all TX PDUs of the
 *                  CAN controller
 * @synchronous     TRUE
 * @reentrant       Reentrant (Not for the same controller)
 * @trace           CPD-61410
 */
extern CanIf_NotifStatusType CanIf_GetTxConfirmationState(uint8 ControllerId);
#endif

#if ((STD_ON == CANIF_PUBLIC_PN_SUPPORT) && (STD_ON == CANIF_PUBLIC_TRCV_PN_SUPPORT))
/**
 * @brief           Requests the CanIf module to clear the WUF flag of the designated CAN transceiver.
 * @param[in]       TransceiverId: Abstract CanIf TransceiverId, which is assigned to the designated
 *                  CAN transceiver.
 * @return          Std_ReturnType
 * @retval          E_OK: Request has been accepted.
 * @retval          E_NOT_OK: Request has not been accepted.
 * @synchronous     FALSE
 * @reentrant       Reentrant for different CAN transceivers
 * @trace           CPD-61411
 */
extern Std_ReturnType CanIf_ClearTrcvWufFlag(uint8 TransceiverId);

/**
 * @brief           Requests the CanIf module to check the Wake flag of the designated CAN transceiver.
 * @param[in]       TransceiverId: Abstract CanIf TransceiverId, which is assigned to the designated
 *                  CAN transceiver.
 * @return          Std_ReturnType
 * @retval          E_OK: Request has been acceptedl
 * @retval          E_NOT_OK: Request has not been accepted.
 * @synchronous     FALSE
 * @reentrant       Reentrant for different CAN transceivers
 * @trace           CPD-61412
 */
extern Std_ReturnType CanIf_CheckTrcvWakeFlag(uint8 TransceiverId);
#endif

#if (STD_ON == CANIF_SET_BAUDRATE_API)
/**
 * @brief           This service shall set the baud rate configuration of the CAN controller.
 *                  Depending on necessary baud rate modifications the controller might have to reset.
 * @param[in]       ControllerId: Abstract CanIf ControllerId which is assigned to a CAN controller,
 *                  whose baud rate shall be set.
 * @param[in]       BaudRateConfigID: References a baud rate configuration by ID.
 * @return          Std_ReturnType
 * @retval          E_OK: Service request accepted, setting of (new) baud rate started.
 * @retval          E_NOT_OK: Service request not accepted.
 * @synchronous     TRUE
 * @reentrant       Reentrant for different ControllerIds. Non reentrant for the same ControllerId.
 * @trace           CPD-61413
 */
extern Std_ReturnType CanIf_SetBaudrate(uint8 ControllerId, uint16 BaudRateConfigID);
#endif

#if (CANIF_CAN_AUTOSAR_VERSION >= CANIF_CAN_AUTOSAR_R431)
/**
 * @brief           This service calls the corresponding CAN Driver service for obtaining the error
 *                  state of the CAN controller.
 * @param[in]       ControllerId: Abstracted CanIf ControllerId which is assigned to a CAN controller,
 *                  which is requested for ErrorState.
 * @param[out]      ErrorStatePtr: Pointer to a memory location, where the error state of the CAN
 *                  controller will be stored.
 * @return          Std_ReturnType
 * @retval          E_OK: Error state request has been accepted.
 * @retval          E_NOT_OK: Error state request has not been accepted.
 * @synchronous     TRUE
 * @reentrant       Non reentrant for the same ControllerId.
 * @trace           CPD-85798
 */
extern Std_ReturnType CanIf_GetControllerErrorState(uint8 ControllerId, Can_ErrorStateType* ErrorStatePtr);
#endif

#if (CANIF_CAN_AUTOSAR_VERSION >= CANIF_CAN_AUTOSAR_R440)
/**
 * @brief           This service calls the corresponding CAN Driver service for obtaining the Rx error
                    counter of the CAN controller.
 * @param[in]       ControllerId: Abstracted CanIf ControllerId which is assigned to a CAN controller.
 * @param[out]      RxErrorCounterPtr: Pointer to a memory location, where the current Rx error counter
                    of the CAN controller will be stored.
 * @return          Std_ReturnType
 * @retval          E_OK: Rx error counter available.
 * @retval          E_NOT_OK: Wrong ControllerId, or Rx error counter not available.
 * @synchronous     TRUE
 * @reentrant       Non reentrant for the same ControllerId.
 * @trace           CPD-85460
 */
extern Std_ReturnType CanIf_GetControllerRxErrorCounter(uint8 ControllerId, uint8* RxErrorCounterPtr);

/**
 * @brief           This service calls the corresponding CAN Driver service for obtaining the Tx error
                    counter of the CAN controller.
 * @param[in]       ControllerId: Abstracted CanIf ControllerId which is assigned to a CAN controller.
 * @param[out]      TxErrorCounterPtr: Pointer to a memory location, where the current Tx error counter
                    of the CAN controller will be stored.
 * @return          Std_ReturnType
 * @retval          E_OK: Tx error counter available.
 * @retval          E_NOT_OK: Wrong ControllerId, or Tx error counter not available.
 * @synchronous     TRUE
 * @reentrant       Non reentrant for the same ControllerId.
 * @trace           CPD-85461
 */
extern Std_ReturnType CanIf_GetControllerTxErrorCounter(uint8 ControllerId, uint8* TxErrorCounterPtr);
#endif

#ifdef __cplusplus
}
#endif

#endif
