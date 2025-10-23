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
 **  @file               : CanIf_DetError.c
 **  @author             : zhengfei.li
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : CanIf source file for DET error detections
 **
 ***********************************************************************************************************************/
/* PRQA S 1503 EOF */ /* VL_QAC_NoUsedApi */
/* PRQA S 1532 EOF */ /* VL_QAC_OneFunRef */
/* PRQA S 1505 EOF */ /* VL_CanIf_1505 */
/* PRQA S 2842 EOF */ /* VL_CanIf_2842 */
/* PRQA S 1258 EOF */ /* VL_CanIf_1258 */
/* PRQA S 0553 EOF */ /* VL_QAC_UnUsedFiles */
/* =================================================== inclusions =================================================== */
#include "CanIf_Types.h"
#include "CanIf_CfgTypes.h"
#include "CanIf_DetError.h"
#include "CanIf_Internal.h"

/* ===================================================== macros ===================================================== */
#define CANIF_CAN_MAX_PDU_LENGTH   8u
#define CANIF_CANFD_MAX_PDU_LENGTH 64u

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
#if ((STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT) || (CANIF_VARIANT_NUMBER > 1u))
/**
 * @brief              Development error validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @param[in]          apiId: API ID
 * @param[in]          partitionIndex: Index of partition
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 */
CANIF_LOCAL Std_ReturnType CanIf_ValidatePartition(uint8 apiId, ApplicationType partitionIndex);

/**
 * @brief              Development error validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @param[in]          apiId: API ID
 * @param[in]          partitionIndex: Index of partition
 * @param[in]          ctrlId: Controller ID
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 */
CANIF_LOCAL Std_ReturnType CanIf_ValidateController(uint8 apiId, ApplicationType partitionIndex, uint8 ctrlId);
#endif

#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
#if (CANIF_TRCV_NUMBER > 0u)
/**
 * @brief              Development error validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @param[in]          apiId: API ID
 * @param[in]          partitionIndex: Index of partition
 * @param[in]          trcvId: Transceiver ID
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 */
CANIF_LOCAL Std_ReturnType CanIf_ValidateTransceiver(uint8 apiId, ApplicationType partitionIndex, uint8 trcvId);
#endif

/**
 * @brief              Development error validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @param[in]          apiId: API ID
 * @param[in]          partitionIndex: index of partition
 * @param[in]          txPduId: TxPdu ID
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 */
CANIF_LOCAL Std_ReturnType CanIf_ValidateTxPdu(uint8 apiId, ApplicationType partitionIndex, PduIdType txPduId);

/**
 * @brief              Development error validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @param[in]          apiId: API ID
 * @param[in]          partitionIndex: index of partition
 * @param[in]          rxPduId: rxPdu ID
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 */
CANIF_LOCAL Std_ReturnType CanIf_ValidateRxPdu(uint8 apiId, ApplicationType partitionIndex, PduIdType rxPduId);

/**
 * @brief              Development error validation
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @param[in]          txPduConfigPtr: Pointer the configuration of TxPdu
 * @param[in]          PduInfoPtr: Pointer to pdu info
 * @return             Std_ReturnType
 * @retval             E_OK: pass validation
 * @retval             E_NOT_OK: fail validation
 */
CANIF_LOCAL Std_ReturnType
    CanIf_ValidateTransmitPduInfo(const CanIf_TxPduConfigType* txPduConfigPtr, const PduInfoType* PduInfoPtr);

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

Std_ReturnType CanIf_ValidateInit(const CanIf_ConfigType* ConfigPtr)
{
    Std_ReturnType result  = E_NOT_OK;
    uint8          apiId   = CANIF_INIT_ID;
    uint8          errorId = CANIF_E_NO_ERROR;

    if (NULL_PTR == ConfigPtr)
    {
        errorId = CANIF_E_PARAM_POINTER;
    }
#if (CANIF_MAX_RXPDU_NUMBER > 0u)
    else if (ConfigPtr->RxPduConfigNumber > CANIF_MAX_RXPDU_NUMBER)
    {
        errorId = CANIF_E_INIT_FAILED;
    }
#endif
#if (CANIF_MAX_TXPDU_NUMBER > 0u)
    else if (ConfigPtr->TxPduConfigNumber > CANIF_MAX_TXPDU_NUMBER)
    {
        errorId = CANIF_E_INIT_FAILED;
    }
#endif
    else
    {
        ApplicationType partitionIndex = CanIf_GetCurrentPartition();

        if (partitionIndex >= CANIF_PARTITION_NUMBER)
        {
            errorId = CANIF_E_INVALID_PARTITION;
        }
        else if (NULL_PTR == CanIf_Runtime[partitionIndex])
        {
            errorId = CANIF_E_INIT_FAILED;
        }
        else
        {
            result = E_OK;
        }
    }

    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
    return result;
}

Std_ReturnType CanIf_ValidateDeInit(void)
{
    Std_ReturnType  result         = E_NOT_OK;
    uint8           apiId          = CANIF_DEINIT_ID;
    uint8           errorId        = CANIF_E_NO_ERROR;
    ApplicationType partitionIndex = CanIf_GetCurrentPartition();

    if (NULL_PTR == CanIf_ConfigStd)
    {
        errorId = CANIF_E_UNINIT;
    }
    else if (partitionIndex >= CANIF_PARTITION_NUMBER)
    {
        errorId = CANIF_E_INVALID_PARTITION;
    }
    else if (NULL_PTR == CanIf_Runtime[partitionIndex])
    {
        errorId = CANIF_E_INVALID_PARTITION;
    }
    else
    {
        result = E_OK;
    }

    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
    return result;
}

Std_ReturnType CanIf_ValidateSetControllerMode(uint8 ControllerId, Can_ControllerStateType ControllerMode)
{
    Std_ReturnType  result         = E_NOT_OK;
    uint8           apiId          = CANIF_SETCONTROLLER_MODE_ID;
    uint8           errorId        = CANIF_E_NO_ERROR;
    ApplicationType partitionIndex = CanIf_GetCurrentPartition();

    if (NULL_PTR == CanIf_ConfigStd)
    {
        errorId = CANIF_E_UNINIT;
    }
    else if (E_OK != CanIf_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (E_OK != CanIf_ValidateController(apiId, partitionIndex, ControllerId))
    {
        /* do nothing */
    }
    else if (
        (CAN_CS_STARTED != ControllerMode) && (CAN_CS_STOPPED != ControllerMode) && (CAN_CS_SLEEP != ControllerMode))
    {
        errorId = CANIF_E_PARAM_CTRLMODE;
    }
    else
    {
        result = E_OK;
    }

    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
    return result;
}

Std_ReturnType CanIf_ValidateGetControllerMode(uint8 ControllerId, const Can_ControllerStateType* ControllerModePtr)
{
    Std_ReturnType  result         = E_NOT_OK;
    uint8           apiId          = CANIF_GETCONTROLLER_MODE_ID;
    uint8           errorId        = CANIF_E_NO_ERROR;
    ApplicationType partitionIndex = CanIf_GetCurrentPartition();

    if (NULL_PTR == CanIf_ConfigStd)
    {
        errorId = CANIF_E_UNINIT;
    }
    else if (E_OK != CanIf_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (E_OK != CanIf_ValidateController(apiId, partitionIndex, ControllerId))
    {
        /* do nothing */
    }
    else if (NULL_PTR == ControllerModePtr)
    {
        errorId = CANIF_E_PARAM_POINTER;
    }
    else
    {
        result = E_OK;
    }

    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
    return result;
}

Std_ReturnType CanIf_ValidateTransmit(PduIdType CanIfTxSduId, const PduInfoType* PduInfoPtr)
{
    Std_ReturnType  result         = E_NOT_OK;
    uint8           apiId          = CANIF_TRANSMIT_ID;
    uint8           errorId        = CANIF_E_NO_ERROR;
    ApplicationType partitionIndex = CanIf_GetCurrentPartition();

    if (NULL_PTR == CanIf_ConfigStd)
    {
        errorId = CANIF_E_UNINIT;
    }
    else if (E_OK != CanIf_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (E_OK != CanIf_ValidateTxPdu(apiId, partitionIndex, CanIfTxSduId))
    {
        /* do nothing */
    }
    else
    {
        PduIdType                    txPduIndex     = CanIf_TxPduId2VarIndex(CanIfTxSduId);
        const CanIf_TxPduConfigType* txPduConfigPtr = &CANIF_TXPDU(txPduIndex);

        result = CanIf_ValidateTransmitPduInfo(txPduConfigPtr, PduInfoPtr);
    }

    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
    return result;
}

#if (STD_ON == CANIF_PUBLIC_READ_RX_PDU_DATA_API)
Std_ReturnType CanIf_ValidateReadRxPduData(PduIdType CanIfRxSduId, const PduInfoType* CanIfRxInfoPtr)
{
    Std_ReturnType  result         = E_NOT_OK;
    uint8           apiId          = CANIF_READRXPDUDATA_ID;
    uint8           errorId        = CANIF_E_NO_ERROR;
    ApplicationType partitionIndex = CanIf_GetCurrentPartition();

    if (NULL_PTR == CanIf_ConfigStd)
    {
        errorId = CANIF_E_UNINIT;
    }
    else if (E_OK != CanIf_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (E_OK != CanIf_ValidateRxPdu(apiId, partitionIndex, CanIfRxSduId))
    {
        /* do nothing */
    }
    else if ((NULL_PTR == CanIfRxInfoPtr) || (NULL_PTR == CanIfRxInfoPtr->SduDataPtr))
    {
        errorId = CANIF_E_PARAM_POINTER;
    }
    else
    {
        result = E_OK;
    }

    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
    return result;
}
#endif

#if (STD_ON == CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API)
Std_ReturnType CanIf_ValidateReadTxNotifStatus(PduIdType CanIfTxSduId)
{
    Std_ReturnType  result         = E_NOT_OK;
    uint8           apiId          = CANIF_READTXNOTIFSTATUS_ID;
    uint8           errorId        = CANIF_E_NO_ERROR;
    ApplicationType partitionIndex = CanIf_GetCurrentPartition();

    if (NULL_PTR == CanIf_ConfigStd)
    {
        errorId = CANIF_E_UNINIT;
    }
    else if (E_OK != CanIf_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (E_OK != CanIf_ValidateTxPdu(apiId, partitionIndex, CanIfTxSduId))
    {
        /* do nothing */
    }
    else
    {
        PduIdType                    txPduIndex     = CanIf_TxPduId2VarIndex(CanIfTxSduId);
        const CanIf_TxPduConfigType* txPduConfigPtr = &CANIF_TXPDU(txPduIndex);

        if (NULL_PTR == txPduConfigPtr->TxNotifStatusPtr)
        {
            errorId = CANIF_E_INVALID_TXPDUID;
        }
        else
        {
            result = E_OK;
        }
    }

    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
    return result;
}
#endif

#if (STD_ON == CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API)
Std_ReturnType CanIf_ValidateReadRxNotifStatus(PduIdType CanIfRxSduId)
{
    Std_ReturnType  result         = E_NOT_OK;
    uint8           apiId          = CANIF_READRXNOTIFSTATUS_ID;
    uint8           errorId        = CANIF_E_NO_ERROR;
    ApplicationType partitionIndex = CanIf_GetCurrentPartition();

    if (NULL_PTR == CanIf_ConfigStd)
    {
        errorId = CANIF_E_UNINIT;
    }
    else if (E_OK != CanIf_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (E_OK != CanIf_ValidateRxPdu(apiId, partitionIndex, CanIfRxSduId))
    {
        /* do nothing */
    }
    else
    {
        PduIdType                    rxPduIndex     = CanIf_RxPduId2VarIndex(CanIfRxSduId);
        const CanIf_RxPduConfigType* rxPduConfigPtr = &CANIF_RXPDU(rxPduIndex);

        if (NULL_PTR == rxPduConfigPtr->RxNotifStatusPtr)
        {
            errorId = CANIF_E_INVALID_RXPDUID;
        }
#if ((STD_ON == CANIF_PUBLIC_READ_RX_PDU_DATA_API) && (CANIF_RXBUFFER_MAX_NUMBER > 0u))
        else if (rxPduConfigPtr->RxBufferIndex >= CANIF_RXBUFFER_NUMBER(partitionIndex))
        {
            errorId = CANIF_E_INVALID_RXPDUID;
        }
#endif
        else
        {
            result = E_OK;
        }
    }

    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
    return result;
}
#endif

Std_ReturnType CanIf_ValidateSetPduMode(uint8 ControllerId, CanIf_PduModeType PduModeRequest)
{
    Std_ReturnType  result         = E_NOT_OK;
    uint8           apiId          = CANIF_SETPDUMODE_ID;
    uint8           errorId        = CANIF_E_NO_ERROR;
    ApplicationType partitionIndex = CanIf_GetCurrentPartition();

    if (NULL_PTR == CanIf_ConfigStd)
    {
        errorId = CANIF_E_UNINIT;
    }
    else if (E_OK != CanIf_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (E_OK != CanIf_ValidateController(apiId, partitionIndex, ControllerId))
    {
        /* do nothing */
    }
#if (STD_ON == CANIF_TX_OFFLINE_ACTIVE_SUPPORT)
    else if (
        (CANIF_OFFLINE != PduModeRequest) && (CANIF_TX_OFFLINE != PduModeRequest)
        && (CANIF_TX_OFFLINE_ACTIVE != PduModeRequest) && (CANIF_ONLINE != PduModeRequest))
    {
        errorId = CANIF_E_PARAM_PDU_MODE;
    }
#else
    else if (
        (CANIF_OFFLINE != PduModeRequest) && (CANIF_TX_OFFLINE != PduModeRequest) && (CANIF_ONLINE != PduModeRequest))
    {
        errorId = CANIF_E_PARAM_PDU_MODE;
    }
#endif
    else
    {
        result = E_OK;
    }

    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
    return result;
}

Std_ReturnType CanIf_ValidateGetPduMode(uint8 ControllerId, const CanIf_PduModeType* PduModePtr)
{
    Std_ReturnType  result         = E_NOT_OK;
    uint8           apiId          = CANIF_GETPDUMODE_ID;
    uint8           errorId        = CANIF_E_NO_ERROR;
    ApplicationType partitionIndex = CanIf_GetCurrentPartition();

    if (NULL_PTR == CanIf_ConfigStd)
    {
        errorId = CANIF_E_UNINIT;
    }
    else if (E_OK != CanIf_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (E_OK != CanIf_ValidateController(apiId, partitionIndex, ControllerId))
    {
        /* do nothing */
    }
    else if (NULL_PTR == PduModePtr)
    {
        errorId = CANIF_E_PARAM_POINTER;
    }
    else
    {
        result = E_OK;
    }

    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
    return result;
}

#if (STD_ON == CANIF_PUBLIC_SET_DYNAMIC_TX_ID_API)
Std_ReturnType CanIf_ValidateSetDynamicTxId(PduIdType CanIfTxSduId, Can_IdType CanId)
{
    Std_ReturnType  result         = E_NOT_OK;
    uint8           apiId          = CANIF_SETDYNAMICTXID_ID;
    uint8           errorId        = CANIF_E_NO_ERROR;
    ApplicationType partitionIndex = CanIf_GetCurrentPartition();

    if (NULL_PTR == CanIf_ConfigStd)
    {
        errorId = CANIF_E_UNINIT;
    }
    else if (E_OK != CanIf_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (E_OK != CanIf_ValidateTxPdu(apiId, partitionIndex, CanIfTxSduId))
    {
        /* do nothing */
    }
    else
    {
        PduIdType                    txPduIndex     = CanIf_TxPduId2VarIndex(CanIfTxSduId);
        const CanIf_TxPduConfigType* txPduConfigPtr = &CANIF_TXPDU(txPduIndex);

        CanId &= ~(CANIF_CANID32_CANFD_MASK | CANIF_CANID_EXTENDED_MASK);

        if (NULL_PTR == txPduConfigPtr->DynamicTxPduPtr)
        {
            errorId = CANIF_E_INVALID_TXPDUID;
        }
#if (STD_ON == CANIF_TX_EXTENDED_CAN_SUPPORT)
        else if ((CANIF_TX_EXTENDED_CAN == txPduConfigPtr->CanIfTxPduCanIdType) && (CanId > CANIF_EXTENDED_CANID_MAX))
        {
            errorId = CANIF_E_PARAM_CANID;
        }
#endif
#if (STD_ON == CANIF_TX_EXTENDED_FD_CAN_SUPPORT)
        else if (
            (CANIF_TX_EXTENDED_FD_CAN == txPduConfigPtr->CanIfTxPduCanIdType) && (CanId > CANIF_EXTENDED_CANID_MAX))
        {
            errorId = CANIF_E_PARAM_CANID;
        }
#endif
#if (STD_ON == CANIF_TX_STANDARD_CAN_SUPPORT)
        else if ((CANIF_TX_STANDARD_CAN == txPduConfigPtr->CanIfTxPduCanIdType) && (CanId > CANIF_STANDARD_CANID_MAX))
        {
            errorId = CANIF_E_PARAM_CANID;
        }
#endif
#if (STD_ON == CANIF_TX_STANDARD_FD_CAN_SUPPORT)
        else if (
            (CANIF_TX_STANDARD_FD_CAN == txPduConfigPtr->CanIfTxPduCanIdType) && (CanId > CANIF_STANDARD_CANID_MAX))
        {
            errorId = CANIF_E_PARAM_CANID;
        }
#endif
        else
        {
            result = E_OK;
        }
    }

    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
    return result;
}
#endif

#if (CANIF_TRCV_NUMBER > 0u)
Std_ReturnType CanIf_ValidateSetTrcvMode(uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode)
{
    Std_ReturnType  result         = E_NOT_OK;
    uint8           apiId          = CANIF_SETTRCVMODE_ID;
    uint8           errorId        = CANIF_E_NO_ERROR;
    ApplicationType partitionIndex = CanIf_GetCurrentPartition();

    if (NULL_PTR == CanIf_ConfigStd)
    {
        errorId = CANIF_E_UNINIT;
    }
    else if (E_OK != CanIf_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (E_OK != CanIf_ValidateTransceiver(apiId, partitionIndex, TransceiverId))
    {
        /* do nothing */
    }
    else if (
        (CANTRCV_TRCVMODE_NORMAL != TransceiverMode) && (CANTRCV_TRCVMODE_SLEEP != TransceiverMode)
        && (CANTRCV_TRCVMODE_STANDBY != TransceiverMode))
    {
        errorId = CANIF_E_PARAM_TRCVMODE;
    }
    else
    {
        result = E_OK;
    }

    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
    return result;
}

Std_ReturnType CanIf_ValidateGetTrcvMode(uint8 TransceiverId, const CanTrcv_TrcvModeType* TransceiverModePtr)
{
    Std_ReturnType  result         = E_NOT_OK;
    uint8           apiId          = CANIF_GETTRCVMODE_ID;
    uint8           errorId        = CANIF_E_NO_ERROR;
    ApplicationType partitionIndex = CanIf_GetCurrentPartition();

    if (NULL_PTR == CanIf_ConfigStd)
    {
        errorId = CANIF_E_UNINIT;
    }
    else if (E_OK != CanIf_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (E_OK != CanIf_ValidateTransceiver(apiId, partitionIndex, TransceiverId))
    {
        /* do nothing */
    }
    else if (NULL_PTR == TransceiverModePtr)
    {
        errorId = CANIF_E_PARAM_POINTER;
    }
    else
    {
        result = E_OK;
    }

    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
    return result;
}

Std_ReturnType
    CanIf_ValidateGetTrcvWakeupReason(uint8 TransceiverId, const CanTrcv_TrcvWakeupReasonType* TrcvWuReasonPtr)
{
    Std_ReturnType  result         = E_NOT_OK;
    uint8           apiId          = CANIF_GETTRCVWAKEUPREASON_ID;
    uint8           errorId        = CANIF_E_NO_ERROR;
    ApplicationType partitionIndex = CanIf_GetCurrentPartition();

    if (NULL_PTR == CanIf_ConfigStd)
    {
        errorId = CANIF_E_UNINIT;
    }
    else if (E_OK != CanIf_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (E_OK != CanIf_ValidateTransceiver(apiId, partitionIndex, TransceiverId))
    {
        /* do nothing */
    }
    else if (NULL_PTR == TrcvWuReasonPtr)
    {
        errorId = CANIF_E_PARAM_POINTER;
    }
    else
    {
        result = E_OK;
    }

    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
    return result;
}

Std_ReturnType CanIf_ValidateSetTrcvWakeupMode(uint8 TransceiverId, CanTrcv_TrcvWakeupModeType TrcvWakeupMode)
{
    Std_ReturnType  result         = E_NOT_OK;
    uint8           apiId          = CANIF_SETTRCVWAKEUPMODE_ID;
    uint8           errorId        = CANIF_E_NO_ERROR;
    ApplicationType partitionIndex = CanIf_GetCurrentPartition();

    if (NULL_PTR == CanIf_ConfigStd)
    {
        errorId = CANIF_E_UNINIT;
    }
    else if (E_OK != CanIf_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (E_OK != CanIf_ValidateTransceiver(apiId, partitionIndex, TransceiverId))
    {
        /* do nothing */
    }
    else if (
        (CANTRCV_WUMODE_ENABLE != TrcvWakeupMode) && (CANTRCV_WUMODE_DISABLE != TrcvWakeupMode)
        && (CANTRCV_WUMODE_CLEAR != TrcvWakeupMode))
    {
        errorId = CANIF_E_PARAM_TRCVWAKEUPMODE;
    }
    else
    {
        result = E_OK;
    }

    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
    return result;
}
#endif

#if (STD_ON == CANIF_WAKEUP_SUPPORT)
Std_ReturnType CanIf_ValidateCheckWakeup(EcuM_WakeupSourceType WakeupSource)
{
    Std_ReturnType  result         = E_NOT_OK;
    uint8           apiId          = CANIF_CHECKWAKEUP_ID;
    uint8           errorId        = CANIF_E_NO_ERROR;
    ApplicationType partitionIndex = CanIf_GetCurrentPartition();

    if (NULL_PTR == CanIf_ConfigStd)
    {
        errorId = CANIF_E_UNINIT;
    }
    else if (E_OK != CanIf_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (WakeupSource > CANIF_WAKEUPSOURCE_MAX)
    {
        errorId = CANIF_E_PARAM_WAKEUPSOURCE;
    }
    else
    {
        result = E_OK;
    }

    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
    return result;
}
#endif

#if (STD_ON == CANIF_PUBLIC_WAKEUP_CHECK_VALID_SUPPORT)
Std_ReturnType CanIf_ValidateCheckValidation(EcuM_WakeupSourceType WakeupSource)
{
    Std_ReturnType  result         = E_NOT_OK;
    uint8           apiId          = CANIF_CHECKVALIDATION_ID;
    uint8           errorId        = CANIF_E_NO_ERROR;
    ApplicationType partitionIndex = CanIf_GetCurrentPartition();

    if (NULL_PTR == CanIf_ConfigStd)
    {
        errorId = CANIF_E_UNINIT;
    }
    else if (E_OK != CanIf_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (WakeupSource > CANIF_WAKEUPSOURCE_MAX)
    {
        errorId = CANIF_E_PARAM_WAKEUPSOURCE;
    }
    else
    {
        result = E_OK;
    }

    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
    return result;
}
#endif

#if (STD_ON == CANIF_PUBLIC_TX_CONFIRM_POLLING_SUPPORT)
Std_ReturnType CanIf_ValidateGetTxConfirmationState(uint8 ControllerId)
{
    Std_ReturnType  result         = E_NOT_OK;
    uint8           apiId          = CANIF_GETTXCONFIRMATIONSTATE;
    uint8           errorId        = CANIF_E_NO_ERROR;
    ApplicationType partitionIndex = CanIf_GetCurrentPartition();

    if (NULL_PTR == CanIf_ConfigStd)
    {
        errorId = CANIF_E_UNINIT;
    }
    else if (E_OK != CanIf_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (E_OK != CanIf_ValidateController(apiId, partitionIndex, ControllerId))
    {
        /* do nothing */
    }
    else
    {
        result = E_OK;
    }

    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
    return result;
}
#endif

#if ((STD_ON == CANIF_PUBLIC_PN_SUPPORT) && (STD_ON == CANIF_PUBLIC_TRCV_PN_SUPPORT))
Std_ReturnType CanIf_ValidateClearTrcvWufFlag(uint8 TransceiverId)
{
    Std_ReturnType  result         = E_NOT_OK;
    uint8           apiId          = CANIF_CLEARTRCVWUFFLAG_ID;
    uint8           errorId        = CANIF_E_NO_ERROR;
    ApplicationType partitionIndex = CanIf_GetCurrentPartition();

    if (NULL_PTR == CanIf_ConfigStd)
    {
        errorId = CANIF_E_UNINIT;
    }
    else if (E_OK != CanIf_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (E_OK != CanIf_ValidateTransceiver(apiId, partitionIndex, TransceiverId))
    {
        /* do nothing */
    }
    else
    {
        result = E_OK;
    }

    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
    return result;
}

Std_ReturnType CanIf_ValidateCheckTrcvWakeFlag(uint8 TransceiverId)
{
    Std_ReturnType  result         = E_NOT_OK;
    uint8           apiId          = CANIF_CHECKTRCVWAKEFLAG_ID;
    uint8           errorId        = CANIF_E_NO_ERROR;
    ApplicationType partitionIndex = CanIf_GetCurrentPartition();

    if (NULL_PTR == CanIf_ConfigStd)
    {
        errorId = CANIF_E_UNINIT;
    }
    else if (E_OK != CanIf_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (E_OK != CanIf_ValidateTransceiver(apiId, partitionIndex, TransceiverId))
    {
        /* do nothing */
    }
    else
    {
        result = E_OK;
    }

    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
    return result;
}
#endif

#if (STD_ON == CANIF_SET_BAUDRATE_API)
Std_ReturnType CanIf_ValidateSetBaudrate(uint8 ControllerId, uint16 BaudRateConfigID)
{
    Std_ReturnType  result         = E_NOT_OK;
    uint8           apiId          = CANIF_SETBAUDRATE_ID;
    uint8           errorId        = CANIF_E_NO_ERROR;
    ApplicationType partitionIndex = CanIf_GetCurrentPartition();

    CANIF_NOUSED(BaudRateConfigID);

    if (NULL_PTR == CanIf_ConfigStd)
    {
        errorId = CANIF_E_UNINIT;
    }
    else if (E_OK != CanIf_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (E_OK != CanIf_ValidateController(apiId, partitionIndex, ControllerId))
    {
        /* do nothing */
    }
    else
    {
        result = E_OK;
    }

    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
    return result;
}
#endif

#if (STD_ON == CANIF_TRIGGER_TRANSMIT_SUPPORT)
Std_ReturnType CanIf_ValidateTriggerTransmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr)
{
    Std_ReturnType  result         = E_NOT_OK;
    uint8           apiId          = CANIF_TRIGGERTRANSMIT_ID;
    uint8           errorId        = CANIF_E_NO_ERROR;
    ApplicationType partitionIndex = CanIf_GetCurrentPartition();

    CANIF_NOUSED(PduInfoPtr);

    if (NULL_PTR == CanIf_ConfigStd)
    {
        errorId = CANIF_E_UNINIT;
    }
    else if (E_OK != CanIf_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (E_OK != CanIf_ValidateTxPdu(apiId, partitionIndex, TxPduId))
    {
        /* do nothing */
    }
    else
    {
        result = E_OK;
    }

    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
    return result;
}
#endif

Std_ReturnType CanIf_ValidateTxConfirmation(PduIdType CanTxPduId)
{
    Std_ReturnType  result         = E_NOT_OK;
    uint8           apiId          = CANIF_TXCONFIRMATION_ID;
    uint8           errorId        = CANIF_E_NO_ERROR;
    ApplicationType partitionIndex = CanIf_GetCurrentPartition();

    if ((NULL_PTR == CanIf_ConfigStd) || (NULL_PTR == CANIF_TXPDU_REF))
    {
        errorId = CANIF_E_UNINIT;
    }
    else if (E_OK != CanIf_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
#if (CANIF_MAX_TXPDU_NUMBER > 0u)
    else if (CanTxPduId >= CANIF_MAX_TXPDU_NUMBER)
    {
        errorId = CANIF_E_PARAM_LPDU;
    }
#endif
    else
    {
        PduIdType txPduIndex = CanIf_TxPduId2VarIndex(CanTxPduId);

        if ((txPduIndex >= CANIF_TXPDU_NUMBER) || (txPduIndex >= CanIf_ConfigStd->TxPduConfigNumber))
        {
            errorId = CANIF_E_PARAM_LPDU;
        }
        else if (partitionIndex != CANIF_PARTITION_INDEX(&CANIF_TXPDU(txPduIndex)))
        {
            errorId = CANIF_E_INVALID_PARTITION;
        }
        else
        {
            result = E_OK;
        }
    }

    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
    return result;
}
#endif

#if ((STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT) || (CANIF_VARIANT_NUMBER > 1u))
Std_ReturnType CanIf_ValidateRxIndication(const Can_HwType* Mailbox, const PduInfoType* PduInfoPtr)
{
    Std_ReturnType  result         = E_NOT_OK;
    uint8           apiId          = CANIF_RXINDICATION_ID;
    uint8           errorId        = CANIF_E_NO_ERROR;
    ApplicationType partitionIndex = CanIf_GetCurrentPartition();

    if ((NULL_PTR == CanIf_ConfigStd) || (NULL_PTR == CANIF_HRH_REF) || (NULL_PTR == CANIF_HOH2HRH_REF))
    {
        errorId = CANIF_E_UNINIT;
    }
    else if (E_OK != CanIf_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if ((NULL_PTR == Mailbox) || (NULL_PTR == PduInfoPtr))
    {
        errorId = CANIF_E_PARAM_POINTER;
    }
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
#if (CANIF_CAN_IDTYPE_UINT16 == CANIF_CAN_IDTYPE)
    else if (0x0000u != (Mailbox->CanId & 0xb800u)) /*Standard frame*/
#else
    else if (
        /**
         * The two most significant bits specify the frame type:
         * 00 CAN message with Standard CAN ID
         * 01 CAN FD frame with Standard CAN ID
         * 10 CAN message with Extended CAN ID
         * 11 CAN FD frame with Extended CAN ID
         */
        (0x80000000uL <= Mailbox->CanId) ? (0x80000000uL != (Mailbox->CanId & 0xa0000000uL))  /*Extended Frame*/
                                         : (0x00000000uL != (Mailbox->CanId & 0xbffff800uL))) /*Standard frame*/
#endif
    /* PRQA S 3120 -- */
    {
        errorId = CANIF_E_PARAM_CANID;
    }
    else if (Mailbox->ControllerId >= CANIF_CANCTRLID2INDEX_NUMBER)
    {
        errorId = CANIF_E_PARAM_CONTROLLERID;
    }
    else
    {
        uint8 controllerId = CanIf_CanCtrlId2Index[Mailbox->ControllerId];

        if (E_OK != CanIf_ValidateController(apiId, partitionIndex, controllerId))
        {
            /* do nothing */
        }
        else if (
            (CANIF_CONTROLLER_DRV_ID(controllerId) >= CANIF_CANDRIVER_NUMBER)
            || (NULL_PTR == CANIF_HOH2HRH(controllerId).CanHrhIdRef))
        {
            errorId = CANIF_E_PARAM_CONTROLLERID;
        }
#if (STD_ON == CANIF_RXINDICATION_AUTOSAR_PARAMETER)
        else if (
            (Mailbox->Hoh >= CanIf_ConfigStd->HrhConfigNumber)
            || (CANIF_HRH(Mailbox->Hoh).CanIfHrhCanCtrlId != controllerId))
#else
        else if (
            (Mailbox->Hoh >= CANIF_HOH2HRH(controllerId).CanHrhIdNumber)
            || (CANIF_HOH2HRH(controllerId).CanHrhIdRef[Mailbox->Hoh] == CANIF_HOHID_INVALID))
#endif
        {
            errorId = CANIF_E_PARAM_HOH;
        }
        else
        {
            result = E_OK;
        }
    }

#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
#endif
    return result;
}
#endif

#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
Std_ReturnType CanIf_ValidateControllerBusOff(uint8 ControllerId)
{
    Std_ReturnType  result         = E_NOT_OK;
    uint8           apiId          = CANIF_CONTROLLERBUSOFF_ID;
    uint8           errorId        = CANIF_E_NO_ERROR;
    ApplicationType partitionIndex = CanIf_GetCurrentPartition();

    if (NULL_PTR == CanIf_ConfigStd)
    {
        errorId = CANIF_E_UNINIT;
    }
    else if (E_OK != CanIf_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (ControllerId >= CANIF_CANCTRLID2INDEX_NUMBER)
    {
        errorId = CANIF_E_PARAM_CONTROLLERID;
    }
    else
    {
        uint8 controllerId = CanIf_CanCtrlId2Index[ControllerId];

        result = CanIf_ValidateController(apiId, partitionIndex, controllerId);
    }

    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
    return result;
}

#if ((STD_ON == CANIF_PUBLIC_PN_SUPPORT) && (STD_ON == CANIF_PUBLIC_TRCV_PN_SUPPORT))
Std_ReturnType CanIf_ValidateConfirmPnAvailability(uint8 TransceiverId)
{
    Std_ReturnType  result         = E_NOT_OK;
    uint8           apiId          = CANIF_CONFIRMPNAVAILABILITY_ID;
    uint8           errorId        = CANIF_E_NO_ERROR;
    ApplicationType partitionIndex = CanIf_GetCurrentPartition();

    if (NULL_PTR == CanIf_ConfigStd)
    {
        errorId = CANIF_E_UNINIT;
    }
    else if (E_OK != CanIf_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (TransceiverId >= CANIF_CANTRCVID2INDEX_NUMBER)
    {
        errorId = CANIF_E_PARAM_TRCV;
    }
    else
    {
        uint8 transceiverId = CanIf_CanTrcvId2Index[TransceiverId];

        result = CanIf_ValidateTransceiver(apiId, partitionIndex, transceiverId);
    }

    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
    return result;
}

Std_ReturnType CanIf_ValidateClearTrcvWufFlagIndication(uint8 TransceiverId)
{
    Std_ReturnType  result         = E_NOT_OK;
    uint8           apiId          = CANIF_CLEARTRCVWUFFLAGINDICATION_ID;
    uint8           errorId        = CANIF_E_NO_ERROR;
    ApplicationType partitionIndex = CanIf_GetCurrentPartition();

    if (NULL_PTR == CanIf_ConfigStd)
    {
        errorId = CANIF_E_UNINIT;
    }
    else if (E_OK != CanIf_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (TransceiverId >= CANIF_CANTRCVID2INDEX_NUMBER)
    {
        errorId = CANIF_E_PARAM_TRCV;
    }
    else
    {
        uint8 transceiverId = CanIf_CanTrcvId2Index[TransceiverId];

        result = CanIf_ValidateTransceiver(apiId, partitionIndex, transceiverId);
    }

    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
    return result;
}

Std_ReturnType CanIf_ValidateCheckTrcvWakeFlagIndication(uint8 TransceiverId)
{
    Std_ReturnType  result         = E_NOT_OK;
    uint8           apiId          = CANIF_CHECKTRCVWAKEFLAGINDICATION_ID;
    uint8           errorId        = CANIF_E_NO_ERROR;
    ApplicationType partitionIndex = CanIf_GetCurrentPartition();

    if (NULL_PTR == CanIf_ConfigStd)
    {
        errorId = CANIF_E_UNINIT;
    }
    else if (E_OK != CanIf_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (TransceiverId >= CANIF_CANTRCVID2INDEX_NUMBER)
    {
        errorId = CANIF_E_PARAM_TRCV;
    }
    else
    {
        uint8 transceiverId = CanIf_CanTrcvId2Index[TransceiverId];

        result = CanIf_ValidateTransceiver(apiId, partitionIndex, transceiverId);
    }

    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
    return result;
}
#endif

Std_ReturnType CanIf_ValidateControllerModeIndication(uint8 ControllerId, Can_ControllerStateType ControllerMode)
{
    Std_ReturnType  result         = E_NOT_OK;
    uint8           apiId          = CANIF_CONTROLLERMODEINDICATION_ID;
    uint8           errorId        = CANIF_E_NO_ERROR;
    ApplicationType partitionIndex = CanIf_GetCurrentPartition();

    CANIF_NOUSED(ControllerMode);

    if (NULL_PTR == CanIf_ConfigStd)
    {
        errorId = CANIF_E_UNINIT;
    }
    else if (E_OK != CanIf_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (ControllerId >= CANIF_CANCTRLID2INDEX_NUMBER)
    {
        errorId = CANIF_E_PARAM_CONTROLLERID;
    }
    else
    {
        uint8 controllerId = CanIf_CanCtrlId2Index[ControllerId];

        result = CanIf_ValidateController(apiId, partitionIndex, controllerId);
    }

    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
    return result;
}

#if (CANIF_TRCV_NUMBER > 0u)
Std_ReturnType CanIf_ValidateTrcvModeIndication(uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode)
{
    Std_ReturnType  result         = E_NOT_OK;
    uint8           apiId          = CANIF_TRCVMODEINDICATION_ID;
    uint8           errorId        = CANIF_E_NO_ERROR;
    ApplicationType partitionIndex = CanIf_GetCurrentPartition();

    CANIF_NOUSED(TransceiverMode);

    if (NULL_PTR == CanIf_ConfigStd)
    {
        errorId = CANIF_E_UNINIT;
    }
    else if (E_OK != CanIf_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (TransceiverId >= CANIF_CANTRCVID2INDEX_NUMBER)
    {
        errorId = CANIF_E_PARAM_TRCV;
    }
    else
    {
        uint8 transceiverId = CanIf_CanTrcvId2Index[TransceiverId];

        result = CanIf_ValidateTransceiver(apiId, partitionIndex, transceiverId);
    }

    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
    return result;
}
#endif
#endif

/* ========================================== internal function definitions ========================================= */
#if ((STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT) || (CANIF_VARIANT_NUMBER > 1u))
CANIF_LOCAL Std_ReturnType CanIf_ValidatePartition(uint8 apiId, ApplicationType partitionIndex)
{
    Std_ReturnType result  = E_NOT_OK;
    uint8          errorId = CANIF_E_NO_ERROR;

    if (partitionIndex >= CANIF_PARTITION_NUMBER)
    {
        errorId = CANIF_E_INVALID_PARTITION;
    }
    else if ((NULL_PTR == CanIf_Runtime[partitionIndex]) || (CANIF_INITED != CanIf_Runtime[partitionIndex]->InitStatus))
    {
        errorId = CANIF_E_UNINIT;
    }
    else
    {
        result = E_OK;
    }

#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
#endif
    return result;
}

CANIF_LOCAL Std_ReturnType CanIf_ValidateController(uint8 apiId, ApplicationType partitionIndex, uint8 ctrlId)
{
    Std_ReturnType result  = E_NOT_OK;
    uint8          errorId = CANIF_E_NO_ERROR;

    if (NULL_PTR == CANIF_CTRL_PBCFG_REF)
    {
        errorId = CANIF_E_UNINIT;
    }
    else if (ctrlId >= CANIF_CANCONTROLLER_NUMBER)
    {
        errorId = CANIF_E_PARAM_CONTROLLERID;
    }
    else if (partitionIndex != CANIF_PARTITION_INDEX(&CANIF_CTRL_PBCFG(ctrlId)))
    {
        errorId = CANIF_E_INVALID_PARTITION;
    }
    else
    {
        result = E_OK;
    }

#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
#endif
    return result;
}
#endif

#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
#if (CANIF_TRCV_NUMBER > 0u)
CANIF_LOCAL Std_ReturnType CanIf_ValidateTransceiver(uint8 apiId, ApplicationType partitionIndex, uint8 trcvId)
{
    Std_ReturnType result  = E_NOT_OK;
    uint8          errorId = CANIF_E_NO_ERROR;

    if (NULL_PTR == CANIF_TRCV_PBCFG_REF)
    {
        errorId = CANIF_E_UNINIT;
    }
    else if (trcvId >= CANIF_TRCV_NUMBER)
    {
        errorId = CANIF_E_PARAM_TRCV;
    }
    else if (partitionIndex != CANIF_PARTITION_INDEX(&CANIF_TRCV_PBCFG(trcvId)))
    {
        errorId = CANIF_E_INVALID_PARTITION;
    }
    else
    {
        result = E_OK;
    }

    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
    return result;
}
#endif

CANIF_LOCAL Std_ReturnType CanIf_ValidateTxPdu(uint8 apiId, ApplicationType partitionIndex, PduIdType txPduId)
{
    Std_ReturnType result  = E_NOT_OK;
    uint8          errorId = CANIF_E_NO_ERROR;

    if (NULL_PTR == CANIF_TXPDU_REF)
    {
        errorId = CANIF_E_UNINIT;
    }
#if (CANIF_MAX_TXPDU_NUMBER > 0u)
    else if (txPduId >= CANIF_MAX_TXPDU_NUMBER)
    {
        errorId = CANIF_E_INVALID_TXPDUID;
    }
#endif
    else
    {
        PduIdType txPduIndex = CanIf_TxPduId2VarIndex(txPduId);

        if ((txPduIndex >= CANIF_TXPDU_NUMBER) || (txPduIndex >= CanIf_ConfigStd->TxPduConfigNumber))
        {
            errorId = CANIF_E_INVALID_TXPDUID;
        }
        else if (partitionIndex != CANIF_PARTITION_INDEX(&CANIF_TXPDU(txPduIndex)))
        {
            errorId = CANIF_E_INVALID_PARTITION;
        }
        else
        {
            result = E_OK;
        }
    }

    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
    return result;
}

CANIF_LOCAL Std_ReturnType CanIf_ValidateRxPdu(uint8 apiId, ApplicationType partitionIndex, PduIdType rxPduId)
{
    Std_ReturnType result  = E_NOT_OK;
    uint8          errorId = CANIF_E_NO_ERROR;

    if (NULL_PTR == CANIF_RXPDU_REF)
    {
        errorId = CANIF_E_UNINIT;
    }
#if (CANIF_MAX_RXPDU_NUMBER > 0u)
    else if (rxPduId >= CANIF_MAX_RXPDU_NUMBER)
    {
        errorId = CANIF_E_INVALID_RXPDUID;
    }
#endif
    else
    {
        PduIdType rxPduIndex = CanIf_RxPduId2VarIndex(rxPduId);

        if ((rxPduIndex >= CANIF_RXPDU_NUMBER) || (rxPduIndex >= CanIf_ConfigStd->RxPduConfigNumber))
        {
            errorId = CANIF_E_INVALID_RXPDUID;
        }
        else if (partitionIndex != CANIF_PARTITION_INDEX(&CANIF_RXPDU(rxPduIndex)))
        {
            errorId = CANIF_E_INVALID_PARTITION;
        }
        else
        {
            result = E_OK;
        }
    }

    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
    return result;
}

CANIF_LOCAL Std_ReturnType
    CanIf_ValidateTransmitPduInfo(const CanIf_TxPduConfigType* txPduConfigPtr, const PduInfoType* PduInfoPtr)
{
    Std_ReturnType result  = E_NOT_OK;
    uint8          apiId   = CANIF_TRANSMIT_ID;
    uint8          errorId = CANIF_E_NO_ERROR;

    if (NULL_PTR == txPduConfigPtr)
    {
        errorId = CANIF_E_INVALID_TXPDUID;
    }
    else if (NULL_PTR == PduInfoPtr)
    {
        errorId = CANIF_E_PARAM_POINTER;
    }
    else if ((txPduConfigPtr->TxMetaDataEnable) && (NULL_PTR == PduInfoPtr->MetaDataPtr))
    {
        errorId = CANIF_E_PARAM_POINTER;
    }
#if (STD_ON == CANIF_TX_STANDARD_CAN_SUPPORT)
    else if (
        (CANIF_TX_STANDARD_CAN == txPduConfigPtr->CanIfTxPduCanIdType)
        && (PduInfoPtr->SduLength > CANIF_CAN_MAX_PDU_LENGTH))
    {
        CanIf_DetReportRuntimeError(apiId, CANIF_E_DATA_LENGTH_MISMATCH);
    }
#endif
#if (STD_ON == CANIF_TX_EXTENDED_CAN_SUPPORT)
    else if (
        (CANIF_TX_EXTENDED_CAN == txPduConfigPtr->CanIfTxPduCanIdType)
        && (PduInfoPtr->SduLength > CANIF_CAN_MAX_PDU_LENGTH))
    {
        CanIf_DetReportRuntimeError(apiId, CANIF_E_DATA_LENGTH_MISMATCH);
    }
#endif
#if (STD_ON == CANIF_TX_STANDARD_FD_CAN_SUPPORT)
    else if (
        (CANIF_TX_STANDARD_FD_CAN == txPduConfigPtr->CanIfTxPduCanIdType)
        && (PduInfoPtr->SduLength > CANIF_CANFD_MAX_PDU_LENGTH))
    {
        CanIf_DetReportRuntimeError(apiId, CANIF_E_DATA_LENGTH_MISMATCH);
    }
#endif
#if (STD_ON == CANIF_TX_EXTENDED_FD_CAN_SUPPORT)
    else if (
        (CANIF_TX_EXTENDED_FD_CAN == txPduConfigPtr->CanIfTxPduCanIdType)
        && (PduInfoPtr->SduLength > CANIF_CANFD_MAX_PDU_LENGTH))
    {
        CanIf_DetReportRuntimeError(apiId, CANIF_E_DATA_LENGTH_MISMATCH);
    }
#endif
    else if ((!txPduConfigPtr->CanIfTxPduTruncation) && (PduInfoPtr->SduLength > txPduConfigPtr->CanIfTxPduDlc))
    {
        CanIf_DetReportRuntimeError(apiId, CANIF_E_TXPDU_LENGTH_EXCEEDED);
    }
    else
    {
        result = E_OK;
    }

    if (CANIF_E_NO_ERROR != errorId)
    {
        CanIf_DetReportError(apiId, errorId);
    }
    return result;
}

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#endif
