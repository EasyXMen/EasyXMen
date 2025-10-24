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
 **  @file               : CanIf_DetError.h
 **  @author             : zhengfei.li
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : CanIf header file for DET error detections
 **
 ***********************************************************************************************************************/

#ifndef CANIF_DETERROR_H_
#define CANIF_DETERROR_H_

/* =================================================== inclusions =================================================== */
#include "CanIf_Cfg.h"
#include "CanIf_PBcfg.h"
#include "CanIf_Types.h"
#include "Det.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */
/*Service ID*/
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
#define CANIF_INIT_ID                        (0x01u)
#define CANIF_DEINIT_ID                      (0x02u)
#define CANIF_SETCONTROLLER_MODE_ID          (0x03u)
#define CANIF_GETCONTROLLER_MODE_ID          (0x04u)
#define CANIF_READRXPDUDATA_ID               (0x06u)
#define CANIF_READTXNOTIFSTATUS_ID           (0x07u)
#define CANIF_READRXNOTIFSTATUS_ID           (0x08u)
#define CANIF_SETPDUMODE_ID                  (0x09u)
#define CANIF_GETPDUMODE_ID                  (0x0Au)
#define CANIF_GETVERSIONINFO_ID              (0x0Bu)
#define CANIF_SETDYNAMICTXID_ID              (0x0Cu)
#define CANIF_SETTRCVMODE_ID                 (0x0Du)
#define CANIF_GETTRCVMODE_ID                 (0x0Eu)
#define CANIF_GETTRCVWAKEUPREASON_ID         (0x0Fu)
#define CANIF_SETTRCVWAKEUPMODE_ID           (0x10u)
#define CANIF_CHECKVALIDATION_ID             (0x12u)
#define CANIF_GETTXCONFIRMATIONSTATE         (0x19u)
#define CANIF_CLEARTRCVWUFFLAG_ID            (0x1Eu)
#define CANIF_CHECKTRCVWAKEFLAG_ID           (0x1Fu)
#define CANIF_SETBAUDRATE_ID                 (0x27u)
#define CANIF_ENABLEBUSMIRRORING_ID          (0x4cu)
#define CANIF_TRIGGERTRANSMIT_ID             (0x41u)
#define CANIF_TXCONFIRMATION_ID              (0x13u)
#define CANIF_CONTROLLERBUSOFF_ID            (0x16u)
#define CANIF_CONFIRMPNAVAILABILITY_ID       (0x1Au)
#define CANIF_CLEARTRCVWUFFLAGINDICATION_ID  (0x20u)
#define CANIF_CHECKTRCVWAKEFLAGINDICATION_ID (0x21u)
#define CANIF_CONTROLLERMODEINDICATION_ID    (0x17u)
#define CANIF_TRCVMODEINDICATION_ID          (0x22u)
#define CANIF_CURRENTICOMCONFIGURATION_ID    (0x26u)
#endif
#define CANIF_CHECKWAKEUP_ID  (0x11u)
#define CANIF_TRANSMIT_ID     (0x05u)
#define CANIF_RXINDICATION_ID (0x14u)

/* Error detection */
#if ((STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT) || (CANIF_VARIANT_NUMBER > 1u))
#define CANIF_E_NO_ERROR             (00u)
#define CANIF_E_PARAM_CANID          (10u)
#define CANIF_E_PARAM_HOH            (12u)
#define CANIF_E_PARAM_LPDU           (13u)
#define CANIF_E_PARAM_CONTROLLERID   (15u)
#define CANIF_E_PARAM_WAKEUPSOURCE   (16u)
#define CANIF_E_PARAM_TRCV           (17u)
#define CANIF_E_PARAM_TRCVMODE       (18u)
#define CANIF_E_PARAM_TRCVWAKEUPMODE (19u)
#define CANIF_E_PARAM_CTRLMODE       (21u)
#define CANIF_E_PARAM_PDU_MODE       (22u)
#define CANIF_E_PARAM_POINTER        (20u)
#define CANIF_E_UNINIT               (30u)
#define CANIF_E_INVALID_TXPDUID      (50u)
#define CANIF_E_INVALID_RXPDUID      (60u)
#define CANIF_E_INIT_FAILED          (80u)
#define CANIF_E_INVALID_PARTITION    (40u)
#endif
#define CANIF_E_INVALID_DLC           (61u)
#define CANIF_E_DATA_LENGTH_MISMATCH  (62u)
#define CANIF_E_STOPPED               (70u)
#define CANIF_E_NOT_SLEEP             (71u)
#define CANIF_E_TXPDU_LENGTH_EXCEEDED (90u)

#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
#define CanIf_DetReportError(apiId, errorId) (void)Det_ReportError(CANIF_MODULE_ID, 0u, (apiId), (errorId))
#endif
#define CanIf_DetReportRuntimeError(apiId, errorId) \
    (void)Det_ReportRuntimeError(CANIF_MODULE_ID, 0u, (apiId), (errorId))

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)

/**
 * @brief              Development error validation
 * @param[in]          ConfigPtr: Pointer to post-build configuration
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern Std_ReturnType CanIf_ValidateInit(const CanIf_ConfigType* ConfigPtr);

/**
 * @brief              Development error validation
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern Std_ReturnType CanIf_ValidateDeInit(void);

/**
 * @brief              Development error validation
 * @param[in]          ControllerId: Controller ID
 * @param[in]          ControllerMode: Controller mode
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern Std_ReturnType CanIf_ValidateSetControllerMode(uint8 ControllerId, Can_ControllerStateType ControllerMode);

/**
 * @brief              Development error validation
 * @param[in]          ControllerId: Controller ID
 * @param[in]          ControllerModePtr: Pointer to controller mode
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern Std_ReturnType
    CanIf_ValidateGetControllerMode(uint8 ControllerId, const Can_ControllerStateType* ControllerModePtr);

/**
 * @brief              Development error validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @param[in]          CanIfTxSduId: TxPdu ID
 * @param[in]          PduInfoPtr: Pointer ot pdu info
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 * @trace              -
 */
extern Std_ReturnType CanIf_ValidateTransmit(PduIdType CanIfTxSduId, const PduInfoType* PduInfoPtr);

#if (STD_ON == CANIF_PUBLIC_READ_RX_PDU_DATA_API)
/**
 * @brief              Development error validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @param[in]          CanIfRxSduId: RxPdu ID
 * @param[in]          CanIfRxInfoPtr: Pointer to pdu info
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 * @trace              -
 */
extern Std_ReturnType CanIf_ValidateReadRxPduData(PduIdType CanIfRxSduId, const PduInfoType* CanIfRxInfoPtr);
#endif

#if (STD_ON == CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API)
/**
 * @brief              Development error validation
 * @param[in]          CanIfTxSduId: TxPdu ID
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern Std_ReturnType CanIf_ValidateReadTxNotifStatus(PduIdType CanIfTxSduId);
#endif

#if (STD_ON == CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API)
/**
 * @brief              Development error validation
 * @param[in]          CanIfRxSduId: RxPdu ID
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern Std_ReturnType CanIf_ValidateReadRxNotifStatus(PduIdType CanIfRxSduId);
#endif

/**
 * @brief              Development error validation
 * @param[in]          ControllerId: Controller ID
 * @param[in]          PduModeRequest: Requested pdu mode
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern Std_ReturnType CanIf_ValidateSetPduMode(uint8 ControllerId, CanIf_PduModeType PduModeRequest);

/**
 * @brief              Development error validation
 * @param[in]          ControllerId: Controller ID
 * @param[in]          PduModePtr: Pointer to pdu mode
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern Std_ReturnType CanIf_ValidateGetPduMode(uint8 ControllerId, const CanIf_PduModeType* PduModePtr);

#if (STD_ON == CANIF_PUBLIC_SET_DYNAMIC_TX_ID_API)
/**
 * @brief              Development error validation
 * @param[in]          CanIfTxSduId: TxPdu ID
 * @param[in]          CanId: CAN ID
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern Std_ReturnType CanIf_ValidateSetDynamicTxId(PduIdType CanIfTxSduId, Can_IdType CanId);
#endif

#if (CANIF_TRCV_NUMBER > 0u)
/**
 * @brief              Development error validation
 * @param[in]          TransceiverId: Transceiver ID
 * @param[in]          TransceiverMode: Requested ransceiver mode
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern Std_ReturnType CanIf_ValidateSetTrcvMode(uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode);

/**
 * @brief              Development error validation
 * @param[in]          TransceiverId: Transceiver ID
 * @param[in]          TransceiverModePtr: Pointer to transceiver mode
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern Std_ReturnType CanIf_ValidateGetTrcvMode(uint8 TransceiverId, const CanTrcv_TrcvModeType* TransceiverModePtr);

/**
 * @brief              Development error validation
 * @param[in]          TransceiverId: Transceiver ID
 * @param[in]          TrcvWuReasonPtr: Pointer to transceiver wakeup reason
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern Std_ReturnType
    CanIf_ValidateGetTrcvWakeupReason(uint8 TransceiverId, const CanTrcv_TrcvWakeupReasonType* TrcvWuReasonPtr);

/**
 * @brief              Development error validation
 * @param[in]          TransceiverId: Transceiver ID
 * @param[in]          TrcvWakeupMode: Transceiver wakeup mode
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern Std_ReturnType CanIf_ValidateSetTrcvWakeupMode(uint8 TransceiverId, CanTrcv_TrcvWakeupModeType TrcvWakeupMode);
#endif

#if (STD_ON == CANIF_WAKEUP_SUPPORT)
/**
 * @brief              Development error validation
 * @param[in]          WakeupSource: Wakeup source
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern Std_ReturnType CanIf_ValidateCheckWakeup(EcuM_WakeupSourceType WakeupSource);
#endif

#if (STD_ON == CANIF_PUBLIC_WAKEUP_CHECK_VALID_SUPPORT)
/**
 * @brief              Development error validation
 * @param[in]          WakeupSource: Wakeup source
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern Std_ReturnType CanIf_ValidateCheckValidation(EcuM_WakeupSourceType WakeupSource);
#endif

#if (STD_ON == CANIF_PUBLIC_TX_CONFIRM_POLLING_SUPPORT)
/**
 * @brief              Development error validation
 * @param[in]          ControllerId: Controller ID
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern Std_ReturnType CanIf_ValidateGetTxConfirmationState(uint8 ControllerId);
#endif

#if ((STD_ON == CANIF_PUBLIC_PN_SUPPORT) && (STD_ON == CANIF_PUBLIC_TRCV_PN_SUPPORT))
/**
 * @brief              Development error validation
 * @param[in]          TransceiverId: Transceiver ID
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern Std_ReturnType CanIf_ValidateClearTrcvWufFlag(uint8 TransceiverId);

/**
 * @brief              Development error validation
 * @param[in]          TransceiverId: Transceiver ID
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern Std_ReturnType CanIf_ValidateCheckTrcvWakeFlag(uint8 TransceiverId);
#endif

#if (STD_ON == CANIF_SET_BAUDRATE_API)
/**
 * @brief              Development error validation
 * @param[in]          ControllerId: Controller ID
 * @param[in]          BaudRateConfigID: Baudrate configuration ID
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern Std_ReturnType CanIf_ValidateSetBaudrate(uint8 ControllerId, uint16 BaudRateConfigID);
#endif

#if (STD_ON == CANIF_TRIGGER_TRANSMIT_SUPPORT)
/**
 * @brief              Development error validation
 * @param[in]          TxPduId: TxPdu ID
 * @param[in]          PduInfoPtr: Pointer to pdu info
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern Std_ReturnType CanIf_ValidateTriggerTransmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr);
#endif

/**
 * @brief              Development error validation
 * @param[in]          CanTxPduId: TxPdu ID
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern Std_ReturnType CanIf_ValidateTxConfirmation(PduIdType CanTxPduId);
#endif

#if ((STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT) || (CANIF_VARIANT_NUMBER > 1u))
/**
 * @brief              Development error validation
 * @param[in]          Mailbox: Pointer to mailbox
 * @param[in]          PduInfoPtr: Pointer to pdu info
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern Std_ReturnType CanIf_ValidateRxIndication(const Can_HwType* Mailbox, const PduInfoType* PduInfoPtr);
#endif

#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
/**
 * @brief              Development error validation
 * @param[in]          ControllerId: Controller ID
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern Std_ReturnType CanIf_ValidateControllerBusOff(uint8 ControllerId);

#if ((STD_ON == CANIF_PUBLIC_PN_SUPPORT) && (STD_ON == CANIF_PUBLIC_TRCV_PN_SUPPORT))
/**
 * @brief              Development error validation
 * @param[in]          TransceiverId: Transceiver ID
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern Std_ReturnType CanIf_ValidateConfirmPnAvailability(uint8 TransceiverId);

/**
 * @brief              Development error validation
 * @param[in]          TransceiverId: Transceiver ID
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern Std_ReturnType CanIf_ValidateClearTrcvWufFlagIndication(uint8 TransceiverId);

/**
 * @brief              Development error validation
 * @param[in]          TransceiverId: Transceiver ID
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern Std_ReturnType CanIf_ValidateCheckTrcvWakeFlagIndication(uint8 TransceiverId);
#endif

/**
 * @brief              Development error validation
 * @param[in]          ControllerId: Controller ID
 * @param[in]          ControllerMode: Indicated controller mode
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern Std_ReturnType
    CanIf_ValidateControllerModeIndication(uint8 ControllerId, Can_ControllerStateType ControllerMode);

#if (CANIF_TRCV_NUMBER > 0u)
/**
 * @brief              Development error validation
 * @param[in]          TransceiverId: Transceiver ID
 * @param[in]          TransceiverMode: Indicated transceiver mode
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern Std_ReturnType CanIf_ValidateTrcvModeIndication(uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode);
#endif

#endif

#ifdef __cplusplus
}
#endif

#endif
