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
 **  @file               : CanIf.c
 **  @author             : zhengfei.li
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : CanIf source file for external API implementations
 **
 ***********************************************************************************************************************/
/* PRQA S 1503 EOF */ /* VL_QAC_NoUsedApi */
/* PRQA S 1532 EOF */ /* VL_QAC_OneFunRef */
/* PRQA S 1505 EOF */ /* VL_CanIf_1505 */
/* PRQA S 2842 EOF */ /* VL_CanIf_2842 */
/* PRQA S 1258 EOF */ /* VL_CanIf_1258 */
/* PRQA S 6520 EOF */ /* VL_MTR_CanIf_STVAR */
/* =================================================== inclusions =================================================== */
#include "CanIf_Types.h"
#include "CanIf_CfgTypes.h"
#include "CanIf.h"
#include "CanIf_Internal.h"
#include "CanIf_Cbk.h"
#include "CanIf_DetError.h"
#if (STD_ON == CANIF_CDD_CANIDS_MODULE_ENABLE)
#include "CDD_CanIds.h"
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
/**
 * @brief              Initialize all used buffers
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @param[in]          partitionIndex: Index of partition
 */
CANIF_LOCAL void CanIf_InitAllBuffer(ApplicationType partitionIndex);

/**
 * @brief              Set controller mode to started
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @param[in]          canDrvId: id of CAN driver
 * @param[in]          canCtrlId: id of CAN controller
 * @return             Std_ReturnType
 * @retval             E_OK: Request has been accepted
 * @retval             E_NOT_OK: Request has not been accepted
 */
CANIF_LOCAL Std_ReturnType CanIf_SetControllStarted(uint8 canDrvId, uint8 canCtrlId);

/**
 * @brief              Set controller mode to sleep
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @param[in]          ctrlConfigPtr: Pointer to controller configuration
 * @param[in]          ctrlPbConfigPtr: Pointer to controller post-build configuration
 * @param[in]          ctrlRuntimePtr: Pointer to controller runtime
 * @param[in]          canDrvId: id of CAN driver
 * @param[in]          canCtrlId: id of CAN controller
 * @return             Std_ReturnType
 * @retval             E_OK: Request has been accepted
 * @retval             E_NOT_OK: Request has not been accepted
 */
CANIF_LOCAL Std_ReturnType CanIf_SetControllSleep(
    const CanIf_CtrlConfigType*   ctrlConfigPtr,
    const CanIf_CtrlPbConfigType* ctrlPbConfigPtr,
    CanIf_CtrlRuntimeType*        ctrlRuntimePtr,
    uint8                         canDrvId,
    uint8                         canCtrlId);

/**
 * @brief              Set controller mode to stopped
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @param[in]          ctrlPbConfigPtr: Pointer to controller post-build configuration
 * @param[in]          ctrlRuntimePtr: Pointer to controller runtimer
 * @param[in]          canDrvId: id of CAN driver
 * @param[in]          canCtrlId: id of CAN controller
 * @return             Std_ReturnType
 * @retval             E_OK: Request has been accepted
 * @retval             E_NOT_OK: Request has not been accepted
 */
CANIF_LOCAL Std_ReturnType CanIf_SetControllStopped(
    const CanIf_CtrlPbConfigType* ctrlPbConfigPtr,
    CanIf_CtrlRuntimeType*        ctrlRuntimePtr,
    uint8                         canDrvId,
    uint8                         canCtrlId);

/**
 * @brief              Check CAN ID is in range
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @param[in]          hrhIndex: HRH index
 * @param[in]          canId: CAN ID
 * @param[in]          rxPduIndexPtr: pointer to RxPdu index
 * @return             Std_ReturnType
 * @retval             E_OK: Request has been accepted
 * @retval             E_NOT_OK: Request has not been accepted
 */
CANIF_LOCAL Std_ReturnType
    CanIf_CanIdRangCheck(Can_HwHandleType hrhIndex, CanIf_IdType canId, PduIdType* rxPduIndexPtr);

/**
 * @brief              HRH filter by linear search
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @param[in]          hrhConfigPtr: Pointer to the configuration of HRH
 * @param[in]          canId: CAN ID
 * @param[out]         rxPduIndexPtr: Pointer to RxPdu ID found by search
 * @return             Std_ReturnType
 * @retval             E_OK: Request has been accepted
 * @retval             E_NOT_OK: Request has not been accepted
 */
CANIF_LOCAL Std_ReturnType
    CanIf_RxPduHrhSearchByLinear(const CanIf_HrhConfigType* hrhConfigPtr, CanIf_IdType canId, PduIdType* rxPduIndexPtr);

#if (CANIF_SOFTWARE_FILTER_TYPE == CANIF_SOFTWARE_FILTER_BINARY)
/**
 * @brief              HRH filter by binary search
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @param[in]          hrhConfigPtr: Pointer to the configuration of HRH
 * @param[in]          canId: CAN ID
 * @param[out]         rxPduIndexPtr: Pointer to RxPdu ID found by search
 * @return             Std_ReturnType
 * @retval             E_OK: Request has been accepted
 * @retval             E_NOT_OK: Request has not been accepted
 */
CANIF_LOCAL Std_ReturnType
    CanIf_RxPduHrhSearchByBinary(const CanIf_HrhConfigType* hrhConfigPtr, CanIf_IdType canId, PduIdType* rxPduIndexPtr);
#endif

/**
 * @brief              Get index of HRH which accepts the specified CanId
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @param[in]          rxPduConfigPtr: Pointer to the configuration of RxPdu
 * @param[in]          canId: CAN ID
 * @return             Std_ReturnType
 * @retval             E_OK: Request has been accepted
 * @retval             E_NOT_OK: Request has not been accepted
 */
CANIF_LOCAL Std_ReturnType CanIf_GetPduHrh(const CanIf_RxPduConfigType* rxPduConfigPtr, CanIf_IdType canId);

/**
 * @brief              Handle the indication of RxPdu from CAN driver
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @param[in]          ctrlRuntimePtr: Pointer to the runtime status of controller
 * @param[in]          rxPduConfigPtr: Pointer to the configuration of RxPdu
 * @param[in]          canId: CAN ID
 * @param[in]          pduInfoPtr: Pointer to the indicated pdu info
 */
CANIF_LOCAL void CanIf_RxIndicationHandle(
    const CanIf_CtrlRuntimeType* ctrlRuntimePtr,
    const CanIf_RxPduConfigType* rxPduConfigPtr,
#if (STD_ON == CANIF_META_DATA_SUPPORT)
    CanIf_IdType canId,
#endif
    const PduInfoType* pduInfoPtr);

#if (STD_ON == CANIF_WAKEUP_SUPPORT)
#if (STD_ON == CANIF_CANDRV_WAKE_UP_SUPPORT)
/**
 * @brief              Handle the wakeup check for a specified controller
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @param[in]          ctrlPbConfigPtr: Pointer to the configuration of controller
 * @param[in]          ctrlConfigPtr: Pointer the configuration of controller
 * @param[out]         controllerNotSleep: Pointer to the result of whether a controller is not in sleep mode
 * @return             Std_ReturnType
 * @retval             E_OK: Request has been accepted
 * @retval             E_NOT_OK: Request has not been accepted
 */
CANIF_LOCAL Std_ReturnType CanIf_CheckCtrlWakeupHandle(
    const CanIf_CtrlPbConfigType* ctrlPbConfigPtr,
    const CanIf_CtrlConfigType*   ctrlConfigPtr,
    boolean*                      controllerNotSleep);
#endif

#if (STD_ON == CANIF_CANTRCV_WAKE_UP_SUPPORT)
/**
 * @brief              Handle the wakeup check for transceivers
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @param[in]          partitionIndex: index of partition
 * @param[in]          wakeupSource: Wakeup source
 * @return             Std_ReturnType
 * @retval             E_OK: Request has been accepted
 * @retval             E_NOT_OK: Request has not been accepted
 */
CANIF_LOCAL Std_ReturnType CanIf_CheckTrcvWakeup(ApplicationType partitionIndex, EcuM_WakeupSourceType wakeupSource);
#endif
#endif

#if (STD_ON == CANIF_PUBLIC_WAKEUP_CHECK_VALID_SUPPORT)
/**
 * @brief              Handle the wakeup validation for a specified controller
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @param[in]          ctrlPbConfigPtr: Pointer to the configuration of controller
 * @param[in]          wakeupSource: Wakeup source
 * @return             Std_ReturnType
 * @retval             E_OK: Request has been accepted
 * @retval             E_NOT_OK: Request has not been accepted
 */
CANIF_LOCAL Std_ReturnType
    CanIf_CheckValidationHandle(const CanIf_CtrlPbConfigType* ctrlPbConfigPtr, EcuM_WakeupSourceType wakeupSource);
#endif

/* ============================================ internal data definitions =========================================== */
#define CANIF_START_SEC_VAR_CLEARED_PTR
#include "CanIf_MemMap.h"
const CanIf_ConfigType* CanIf_ConfigStd;
#define CANIF_STOP_SEC_VAR_CLEARED_PTR
#include "CanIf_MemMap.h"

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

/**
 * This service Initializes internal and external interfaces of the CAN Interface for the further
 * processing.
 */
void CanIf_Init(const CanIf_ConfigType* ConfigPtr)
{
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (E_OK == CanIf_ValidateInit(ConfigPtr))
#endif
    {
        ApplicationType    partitionIndex = CanIf_GetCurrentPartition();
        CanIf_RuntimeType* runtimePtr     = CanIf_Runtime[partitionIndex];

        /* save the configuration to global variable */
        CanIf_ConfigStd = ConfigPtr;
        /* CanIf Module has not been initialized */
        if (CANIF_INITED != runtimePtr->InitStatus)
        {
            runtimePtr->InitStatus = CANIF_INITED;
            CanIf_InitCtrlRuntime(partitionIndex);
            CanIf_InitAllBuffer(partitionIndex);
        }
    }
}

/**
 * De-initializes the CanIf module.
 */
void CanIf_DeInit(void)
{
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (E_OK == CanIf_ValidateDeInit())
#endif
    {
        ApplicationType    partitionIndex = CanIf_GetCurrentPartition();
        CanIf_RuntimeType* runtimePtr     = CanIf_Runtime[partitionIndex];
        boolean            allDeinited    = TRUE;

        runtimePtr->InitStatus = CANIF_UNINIT;

        SchM_Enter_CanIf_ExclusiveArea_Channel();
        for (partitionIndex = 0u; partitionIndex < CANIF_PARTITION_NUMBER; ++partitionIndex)
        {
            runtimePtr = CanIf_Runtime[partitionIndex];
            if (CANIF_INITED == runtimePtr->InitStatus)
            {
                allDeinited = FALSE;
                break;
            }
        }
        if (allDeinited)
        {
            CanIf_ConfigStd = NULL_PTR;
        }
        SchM_Exit_CanIf_ExclusiveArea_Channel();
    }
}

/**
 * This service calls the corresponding CAN Driver service
 * for changing of the CAN controller mode.
 */
Std_ReturnType CanIf_SetControllerMode(uint8 ControllerId, Can_ControllerStateType ControllerMode)
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (E_OK == CanIf_ValidateSetControllerMode(ControllerId, ControllerMode))
#endif
    {
        const CanIf_CtrlConfigType*   ctrlConfigPtr   = &CanIf_CtrlCfgData[ControllerId];
        uint8                         canDrvId        = ctrlConfigPtr->CanDriverId;
        uint8                         canCtrlId       = ctrlConfigPtr->CanCtrlId;
        const CanIf_CtrlPbConfigType* ctrlPbConfigPtr = &CANIF_CTRL_PBCFG(ControllerId);
        CanIf_CtrlRuntimeType*        ctrlRuntimePtr  = ctrlPbConfigPtr->RuntimePtr;

        /* check the ControllerMode to be set*/
        switch (ControllerMode) /* PRQA S 2002 */ /* VL_QAC_NoDefaultCase */
        {
        case CAN_CS_UNINIT:
            /* do nothing */
            break;
        case CAN_CS_STARTED:
            result = CanIf_SetControllStarted(canDrvId, canCtrlId);
            break;
        case CAN_CS_SLEEP:
            result = CanIf_SetControllSleep(ctrlConfigPtr, ctrlPbConfigPtr, ctrlRuntimePtr, canDrvId, canCtrlId);
            break;
        case CAN_CS_STOPPED:
            result = CanIf_SetControllStopped(ctrlPbConfigPtr, ctrlRuntimePtr, canDrvId, canCtrlId);
            break;
        }
    }
    return result;
}

/**
 * This service reports about the current status of the requested CAN controller.
 */
Std_ReturnType CanIf_GetControllerMode(uint8 ControllerId, Can_ControllerStateType* ControllerModePtr)
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (E_OK == CanIf_ValidateGetControllerMode(ControllerId, ControllerModePtr))
#endif
    {
        const CanIf_CtrlPbConfigType* ctrlPbConfigPtr = &CANIF_CTRL_PBCFG(ControllerId);
        const CanIf_CtrlRuntimeType*  ctrlRuntimePtr  = ctrlPbConfigPtr->RuntimePtr;

        /* get current mode */
        *ControllerModePtr = ctrlRuntimePtr->ControllerMode;
        result             = E_OK;
    }
    return result;
}

/* PRQA S 6030 ++ */ /* VL_MTR_CanIf_STMIF */
/**
 * This service initiates a request for transmission of the CAN L-PDU
 * specified by the CanTxSduId and CAN related data in the L-SDU structure.
 */
Std_ReturnType CanIf_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr)
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (E_OK == CanIf_ValidateTransmit(TxPduId, PduInfoPtr))
#endif
    {
        PduIdType                     txPduIndex      = CanIf_TxPduId2VarIndex(TxPduId);
        const CanIf_TxPduConfigType*  txPduConfigPtr  = &CANIF_TXPDU(txPduIndex);
        Can_HwHandleType              hthConfigIndex  = txPduConfigPtr->CanIfTxPduHthId;
        const CanIf_HthConfigType*    hthConfigPtr    = &CANIF_HTH(hthConfigIndex);
        uint8                         ctrlIndex       = hthConfigPtr->CanIfHthCanCtrlId;
        const CanIf_CtrlPbConfigType* ctrlPbConfigPtr = &CANIF_CTRL_PBCFG(ctrlIndex);
        CanIf_CtrlRuntimeType* ctrlRuntimePtr = ctrlPbConfigPtr->RuntimePtr; /* PRQA S 3678 */ /* VL_CanIf_3678 */

        SchM_Enter_CanIf_ExclusiveArea_Channel();
        if (CAN_CS_STARTED == ctrlRuntimePtr->ControllerMode)
        {
            if (CANIF_ONLINE == ctrlRuntimePtr->PduMode)
            {
                const CanIf_CtrlConfigType* ctrlConfigPtr = &CanIf_CtrlCfgData[ctrlIndex];
                Can_PduType                 canPdu        = {0};
                CanIf_IdType                canId         = CANIF_UNUSED_UINT32;

                CanIf_GetBaseCanIdHandle(
                    txPduConfigPtr,
#if (STD_ON == CANIF_META_DATA_SUPPORT)
                    PduInfoPtr,
#endif
                    &canId);

#if (CANIF_CAN_IDTYPE_UINT16 == CANIF_CAN_IDTYPE)
                if ((canId & CANIF_CANID32_CANFD_MASK) != 0u)
                {
                    /*Standard Fd frame,Fd Flag bit change to bit14 for 16 bit Can_IdType*/
                    canPdu.id = (Can_IdType)(canId | CANIF_CANID16_CANFD_MASK);
                }
                else
                {
                    canPdu.id = (Can_IdType)canId;
                }
#else
                canPdu.id = canId;
#endif

                /* get PDU length */
                canPdu.length = (uint8)PduInfoPtr->SduLength;
                if (((PduLengthType)canPdu.length) > txPduConfigPtr->CanIfTxPduDlc)
                {
                    canPdu.length = (uint8)txPduConfigPtr->CanIfTxPduDlc;
                }
                /* get PDU handle */
                canPdu.swPduHandle = TxPduId;
                /* get SDU */
                canPdu.sdu = PduInfoPtr->SduDataPtr;

#if (CANIF_CAN_AUTOSAR_VERSION >= CANIF_CAN_AUTOSAR_R431)
                Std_ReturnType retVal = E_NOT_OK;
#else
                Can_ReturnType retVal = CAN_NOT_OK;
#endif
                uint8            canDrvId = ctrlConfigPtr->CanDriverId;
                Can_HwHandleType canHthId = hthConfigPtr->CanObjectId;

#if (STD_ON == CANIF_PUBLIC_PN_SUPPORT)
                /* ctrlRuntimePtr->PnTxFilter is TRUE only if ctrlConfigPtr->CanIfPnFilter is TRUE */
                if (ctrlRuntimePtr->PnTxFilter)
                {
                    if (txPduConfigPtr->CanIfTxPduPnFilterPdu)
                    {
                        retVal = Can_DriverApi[canDrvId].CanWriteApi(canHthId, &canPdu);
                    }
                }
                else
#endif
                {
                    retVal = Can_DriverApi[canDrvId].CanWriteApi(canHthId, &canPdu);
                }

                if (CAN_OK == retVal)
                {
                    result = E_OK;
                }
#if (STD_ON == CANIF_PUBLIC_TX_BUFFERING)
                else if (CAN_BUSY == retVal)
                {
                    result = CanIf_TxBusyHandle(txPduConfigPtr, &canPdu);
                    if ((E_OK == result) && ((PduInfoPtr->SduLength) > (PduLengthType)(canPdu.length)))
                    {
                        CanIf_DetReportRuntimeError(CANIF_TRANSMIT_ID, CANIF_E_DATA_LENGTH_MISMATCH);
                    }
                }
#endif
                else
                {
                    result = E_NOT_OK;
                }
                if (E_OK == result)
                {
                    if (NULL_PTR != txPduConfigPtr->UpConfirmTxPduPtr)
                    {
                        CanIf_SetUpConfirmTxPduStatus(txPduConfigPtr);
                    }
                }
                SchM_Exit_CanIf_ExclusiveArea_Channel();
            }
#if (STD_ON == CANIF_TX_OFFLINE_ACTIVE_SUPPORT)
            else if (CANIF_TX_OFFLINE_ACTIVE == ctrlRuntimePtr->PduMode)
            {
                /* check notify enable */
#if (STD_ON == CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API)
                CanIf_SetTxNotifStatusHandle(txPduConfigPtr);
#endif
#if (STD_ON == CANIF_PUBLIC_TX_CONFIRM_POLLING_SUPPORT)
                ctrlRuntimePtr->TxConfirmationState = CANIF_TX_RX_NOTIFICATION;
#endif
                SchM_Exit_CanIf_ExclusiveArea_Channel();

                if (NULL_PTR != txPduConfigPtr->UpConfirmTxPduPtr)
                {
                    CanIf_UpTxConfirmationArray[txPduConfigPtr->UpTxConfirmationApiIndex](
                        txPduConfigPtr->CanIfUpPduId,
                        E_OK);
                }
                result = E_OK;
            }
#endif
            else
            {
                SchM_Exit_CanIf_ExclusiveArea_Channel();

                CanIf_DetReportRuntimeError(CANIF_TRANSMIT_ID, CANIF_E_STOPPED);
            }
        }
        else /* Not CAN_CS_STARTED mode */
        {
            SchM_Exit_CanIf_ExclusiveArea_Channel();

            CanIf_DetReportRuntimeError(CANIF_TRANSMIT_ID, CANIF_E_STOPPED);
        }
    }
    return result;
}
/* PRQA S 6030 -- */

#if (STD_ON == CANIF_PUBLIC_READ_RX_PDU_DATA_API)
/**
 * This service provides the CAN DLC and the received data of the requested CanIfRxSduId to the
 * calling upper layer.
 */
Std_ReturnType CanIf_ReadRxPduData(PduIdType CanIfRxSduId, PduInfoType* CanIfRxInfoPtr)
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (E_OK == CanIf_ValidateReadRxPduData(CanIfRxSduId, CanIfRxInfoPtr))
#endif
#if (CANIF_RXBUFFER_MAX_NUMBER > 0u)
    {
        PduIdType                    rxPduIndex     = CanIf_RxPduId2VarIndex(CanIfRxSduId);
        const CanIf_RxPduConfigType* rxPduConfigPtr = &CANIF_RXPDU(rxPduIndex);
        ApplicationType              partitionIndex = CANIF_PARTITION_INDEX(rxPduConfigPtr);
        CanIf_RxBufferNumberType     rxBufferIndex  = rxPduConfigPtr->RxBufferIndex;

        if (rxBufferIndex < CANIF_RXBUFFER_NUMBER(partitionIndex))
        {
            Can_HwHandleType              hrhIndex        = rxPduConfigPtr->CanIfRxPduHrhId;
            const CanIf_HrhConfigType*    hrhConfigPtr    = &CANIF_HRH(hrhIndex);
            uint8                         ctrlIndex       = hrhConfigPtr->CanIfHrhCanCtrlId;
            const CanIf_CtrlPbConfigType* ctrlPbConfigPtr = &CANIF_CTRL_PBCFG(ctrlIndex);
            const CanIf_CtrlRuntimeType*  ctrlRuntimePtr  = ctrlPbConfigPtr->RuntimePtr;
            const CanIf_RxBufferType*     rxBufferPtr     = &CANIF_RXBUFFER(partitionIndex, rxBufferIndex);
            const CanIf_RxPduBufferType*  rxPduBufferPtr  = &rxBufferPtr->RxPduBuffer;

            SchM_Enter_CanIf_ExclusiveArea_Channel();
            if ((CAN_CS_STARTED == ctrlRuntimePtr->ControllerMode) && (CANIF_OFFLINE != ctrlRuntimePtr->PduMode)
                && (rxPduBufferPtr->Length > 0u))
            {
                CanIfRxInfoPtr->SduLength = (PduLengthType)rxPduBufferPtr->Length;
                (void)IStdLib_MemCpy(CanIfRxInfoPtr->SduDataPtr, rxPduBufferPtr->Data, CanIfRxInfoPtr->SduLength);
#if (STD_ON == CANIF_META_DATA_SUPPORT)
                if (rxPduConfigPtr->RxMetaDataEnable)
                {
                    CanIf_CanIdToMetaData(rxPduBufferPtr->MetaData, CanIfRxInfoPtr->MetaDataPtr);
                }
#endif
                result = E_OK;
            }
            SchM_Exit_CanIf_ExclusiveArea_Channel();
        }
    }
#endif
    return result;
}
#endif

#if (STD_ON == CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API)
/**
 * This service returns the confirmation status (confirmation occurred or not)
 * of a specific static or dynamic CAN Tx L-PDU, requested by the CanIfTxSduId.
 */
CanIf_NotifStatusType CanIf_ReadTxNotifStatus(PduIdType CanIfTxSduId)
{
    CanIf_NotifStatusType result = CANIF_NO_NOTIFICATION;

#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (E_OK == CanIf_ValidateReadTxNotifStatus(CanIfTxSduId))
#endif
    {
#if (CANIF_TXNOTIFSTATUS_MAX_NUMBER > 0u)
        PduIdType                    txPduIndex       = CanIf_TxPduId2VarIndex(CanIfTxSduId);
        const CanIf_TxPduConfigType* txPduConfigPtr   = &CANIF_TXPDU(txPduIndex);
        CanIf_NotifStatusType*       txNotifStatusPtr = txPduConfigPtr->TxNotifStatusPtr;

        if (NULL_PTR != txNotifStatusPtr)
        {
            SchM_Enter_CanIf_ExclusiveArea_Channel();
            result            = *txNotifStatusPtr;
            *txNotifStatusPtr = CANIF_NO_NOTIFICATION;
            SchM_Exit_CanIf_ExclusiveArea_Channel();
        }
#endif
    }
    return result;
}
#endif

#if (STD_ON == CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API)
/**
 * This service returns the indication status (indication occurred or not) of a
 * specific CAN Rx L-PDU, requested by the CanIfRxSduId.
 */
CanIf_NotifStatusType CanIf_ReadRxNotifStatus(PduIdType CanIfRxSduId)
{
    CanIf_NotifStatusType result = CANIF_NO_NOTIFICATION;

#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (E_OK == CanIf_ValidateReadRxNotifStatus(CanIfRxSduId))
#endif
    {
#if (CANIF_RXNOTIFSTATUS_MAX_NUMBER > 0u)
        PduIdType                    rxPduIndex       = CanIf_RxPduId2VarIndex(CanIfRxSduId);
        const CanIf_RxPduConfigType* rxPduConfigPtr   = &CANIF_RXPDU(rxPduIndex);
        CanIf_NotifStatusType*       rxNotifStatusPtr = rxPduConfigPtr->RxNotifStatusPtr;

        if (NULL_PTR != rxNotifStatusPtr)
        {
            SchM_Enter_CanIf_ExclusiveArea_Channel();
            result            = *rxNotifStatusPtr;
            *rxNotifStatusPtr = CANIF_NO_NOTIFICATION;
            SchM_Exit_CanIf_ExclusiveArea_Channel();
        }
#endif
    }
    return result;
}
#endif

/**
 * This service sets the requested mode at the L-PDUs of a predefined logical PDU channel.
 */
Std_ReturnType CanIf_SetPduMode(uint8 ControllerId, CanIf_PduModeType PduModeRequest)
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (E_OK == CanIf_ValidateSetPduMode(ControllerId, PduModeRequest))
#endif
    {
        const CanIf_CtrlPbConfigType* ctrlPbConfigPtr   = &CANIF_CTRL_PBCFG(ControllerId);
        CanIf_CtrlRuntimeType*        ctrlRuntimePtr    = ctrlPbConfigPtr->RuntimePtr;
        boolean                       negativeTxConfirm = FALSE;

        SchM_Enter_CanIf_ExclusiveArea_Channel();
        if (CAN_CS_STARTED == ctrlRuntimePtr->ControllerMode)
        {
            switch (PduModeRequest) /* PRQA S 2002 */ /* VL_QAC_NoDefaultCase */
            {
            case CANIF_OFFLINE:
                ctrlRuntimePtr->PduMode = CANIF_OFFLINE;
#if (STD_ON == CANIF_PUBLIC_TX_BUFFERING)
                CanIf_FreeControllerTxBuffer(ctrlPbConfigPtr);
#endif
#if (STD_ON == CANIF_PUBLIC_READ_RX_PDU_DATA_API)
                CanIf_FreeControllerRxBuffer(ctrlPbConfigPtr);
#endif
                negativeTxConfirm = TRUE;
                result            = E_OK;
                break;
            case CANIF_TX_OFFLINE:
                ctrlRuntimePtr->PduMode = CANIF_TX_OFFLINE;
#if (STD_ON == CANIF_PUBLIC_TX_BUFFERING)
                CanIf_FreeControllerTxBuffer(ctrlPbConfigPtr);
#endif
                negativeTxConfirm = TRUE;
#if (STD_ON == CANIF_PUBLIC_PN_SUPPORT)
                const CanIf_CtrlConfigType* ctrlConfigPtr = &CanIf_CtrlCfgData[ControllerId];
                if (ctrlConfigPtr->CanIfPnFilter)
                {
                    ctrlRuntimePtr->PnTxFilter = TRUE;
                }
#endif
                result = E_OK;
                break;
#if (STD_ON == CANIF_TX_OFFLINE_ACTIVE_SUPPORT)
            case CANIF_TX_OFFLINE_ACTIVE:
                ctrlRuntimePtr->PduMode = CANIF_TX_OFFLINE_ACTIVE;
                result                  = E_OK;
                break;
#endif
            case CANIF_ONLINE:
                ctrlRuntimePtr->PduMode = CANIF_ONLINE;
                result                  = E_OK;
                break;
            }
        }
        SchM_Exit_CanIf_ExclusiveArea_Channel();

        if (negativeTxConfirm)
        {
            CanIf_NotifyNegativeTxConfirmation(ctrlPbConfigPtr);
        }
    }
    return result;
}

/**
 * This service reports the current mode of a requested PDU channel.
 */
Std_ReturnType CanIf_GetPduMode(uint8 ControllerId, CanIf_PduModeType* PduModePtr)
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (E_OK == CanIf_ValidateGetPduMode(ControllerId, PduModePtr))
#endif
    {
        const CanIf_CtrlPbConfigType* ctrlPbConfigPtr = &CANIF_CTRL_PBCFG(ControllerId);
        const CanIf_CtrlRuntimeType*  ctrlRuntimePtr  = ctrlPbConfigPtr->RuntimePtr;

        /* get current pdu mode */
        *PduModePtr = ctrlRuntimePtr->PduMode;
        result      = E_OK;
    }
    return result;
}

#if (STD_ON == CANIF_PUBLIC_VERSION_INFO_API)
/**
 * This service returns the version information of the called CAN Interface module.
 */
void CanIf_GetVersionInfo(Std_VersionInfoType* VersionInfo)
{
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (NULL_PTR == VersionInfo)
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_GETVERSIONINFO_ID, CANIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        VersionInfo->vendorID         = CANIF_VENDOR_ID;
        VersionInfo->moduleID         = CANIF_MODULE_ID;
        VersionInfo->sw_major_version = CANIF_SW_MAJOR_VERSION;
        VersionInfo->sw_minor_version = CANIF_SW_MINOR_VERSION;
        VersionInfo->sw_patch_version = CANIF_SW_PATCH_VERSION;
    }
}
#endif

#if (STD_ON == CANIF_PUBLIC_SET_DYNAMIC_TX_ID_API)
/**
 * This service reconfigures the corresponding CAN identifier of the requested CAN L-PDU.
 */
void CanIf_SetDynamicTxId(PduIdType CanIfTxSduId, Can_IdType CanId)
{
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (E_OK == CanIf_ValidateSetDynamicTxId(CanIfTxSduId, CanId))
#endif
    {
#if (CANIF_DYNAMICTXPDU_MAX_NUMBER > 0u)
        const CanIf_TxPduConfigType* txPduConfigPtr  = &CANIF_TXPDU(CanIfTxSduId);
        CanIf_DynamicTxPduType*      dynamicTxPduPtr = txPduConfigPtr->DynamicTxPduPtr;

        if (NULL_PTR != dynamicTxPduPtr)
        {
            dynamicTxPduPtr->CanId = (CanIf_IdType)CanId;
        }
#endif
    }
}
#endif

#if (CANIF_TRCV_NUMBER > 0u)
/**
 * This service changes the operation mode of the tansceiver TransceiverId,
 * via calling the corresponding CAN Transceiver Driver service.
 */
Std_ReturnType CanIf_SetTrcvMode(uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode)
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (E_OK == CanIf_ValidateSetTrcvMode(TransceiverId, TransceiverMode))
#endif
    {
        const CanIf_TrcvConfigType* trcvConfigPtr = &CanIf_TrcvCfgData[TransceiverId];
        uint8                       canTrcvDrvId  = trcvConfigPtr->CanTrcvDriverId;
        uint8                       canTrcvId     = trcvConfigPtr->CanTrcvId;

        result = Can_TrcvApi[canTrcvDrvId].CanTrcvSetOpModeApi(canTrcvId, TransceiverMode);
    }
    return result;
}

/**
 * This function invokes CanTrcv_GetOpMode and updates the parameter
 * TransceiverModePtr with the value OpMode provided by CanTrcv.
 */
Std_ReturnType CanIf_GetTrcvMode(uint8 TransceiverId, CanTrcv_TrcvModeType* TransceiverModePtr)
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (E_OK == CanIf_ValidateGetTrcvMode(TransceiverId, TransceiverModePtr))
#endif
    {
        const CanIf_TrcvConfigType* trcvConfigPtr = &CanIf_TrcvCfgData[TransceiverId];
        uint8                       canTrcvDrvId  = trcvConfigPtr->CanTrcvDriverId;
        uint8                       canTrcvId     = trcvConfigPtr->CanTrcvId;

        result = Can_TrcvApi[canTrcvDrvId].CanTrcvGetOpModeApi(canTrcvId, TransceiverModePtr);
    }
    return result;
}

/**
 * This service returns the reason for the wake up of the transceiver
 * TransceiverId, via calling the corresponding CAN Transceiver Driver service.
 */
Std_ReturnType CanIf_GetTrcvWakeupReason(uint8 TransceiverId, CanTrcv_TrcvWakeupReasonType* TrcvWuReasonPtr)
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (E_OK == CanIf_ValidateGetTrcvWakeupReason(TransceiverId, TrcvWuReasonPtr))
#endif
    {
        const CanIf_TrcvConfigType* trcvConfigPtr = &CanIf_TrcvCfgData[TransceiverId];
        uint8                       canTrcvDrvId  = trcvConfigPtr->CanTrcvDriverId;
        uint8                       canTrcvId     = trcvConfigPtr->CanTrcvId;

        result = Can_TrcvApi[canTrcvDrvId].CanTrcvGetBusWuReasonApi(canTrcvId, TrcvWuReasonPtr);
    }
    return result;
}

/**
 * This function shall call CanTrcv_SetTrcvWakeupMode.
 */
Std_ReturnType CanIf_SetTrcvWakeupMode(uint8 TransceiverId, CanTrcv_TrcvWakeupModeType TrcvWakeupMode)
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (E_OK == CanIf_ValidateSetTrcvWakeupMode(TransceiverId, TrcvWakeupMode))
#endif
    {
        const CanIf_TrcvConfigType* trcvConfigPtr = &CanIf_TrcvCfgData[TransceiverId];
        uint8                       canTrcvDrvId  = trcvConfigPtr->CanTrcvDriverId;
        uint8                       canTrcvId     = trcvConfigPtr->CanTrcvId;

        result = Can_TrcvApi[canTrcvDrvId].CanTrcvSetWakeupModeApi(canTrcvId, TrcvWakeupMode);
    }
    return result;
}
#endif

#if (STD_ON == CANIF_WAKEUP_SUPPORT)
#if (STD_ON == CANIF_CANDRV_WAKE_UP_SUPPORT)
CANIF_LOCAL Std_ReturnType CanIf_CheckCtrlWakeupHandle(
    const CanIf_CtrlPbConfigType* ctrlPbConfigPtr,
    const CanIf_CtrlConfigType*   ctrlConfigPtr,
    boolean*                      controllerNotSleep)
{
    Std_ReturnType               result         = E_NOT_OK;
    const CanIf_CtrlRuntimeType* ctrlRuntimePtr = ctrlPbConfigPtr->RuntimePtr;

    if (CAN_CS_SLEEP != ctrlRuntimePtr->ControllerMode)
    {
        if (NULL_PTR != controllerNotSleep)
        {
            *controllerNotSleep = TRUE;
        }
    }
    else
    {
        uint8 canDrvId  = ctrlConfigPtr->CanDriverId;
        uint8 canCtrlId = ctrlConfigPtr->CanCtrlId;

        if (NULL_PTR != Can_DriverApi[canDrvId].CanCheckWakeupApi)
        {
            if (CAN_OK == Can_DriverApi[canDrvId].CanCheckWakeupApi(canCtrlId))
            {
                result = E_OK;
            }
        }
    }
    return result;
}
#endif

#if (STD_ON == CANIF_CANTRCV_WAKE_UP_SUPPORT)
CANIF_LOCAL Std_ReturnType CanIf_CheckTrcvWakeup(ApplicationType partitionIndex, EcuM_WakeupSourceType wakeupSource)
{
    Std_ReturnType                result          = E_NOT_OK;
    const CanIf_TrcvPbConfigType* trcvPbConfigPtr = &CANIF_TRCV_PBCFG(0u);
    const CanIf_TrcvConfigType*   trcvConfigPtr   = &CanIf_TrcvCfgData[0u];

    for (uint8 trcvIndex = 0u; trcvIndex < CANIF_TRCV_NUMBER; ++trcvIndex)
    {
        EcuM_WakeupSourceType canTrcvSource = trcvConfigPtr->CanIfWakeUpSource;

        if ((CANIF_PARTITION_INDEX(trcvPbConfigPtr) == partitionIndex) && (trcvConfigPtr->CanIfTrcvWakeupSupport)
            && (0u != (wakeupSource & canTrcvSource)))
        {
            uint8 canTrcvDrvId = trcvConfigPtr->CanTrcvDriverId;
            uint8 canTrcvId    = trcvConfigPtr->CanTrcvId;

            if (NULL_PTR != Can_TrcvApi[canTrcvDrvId].CanTrcvCheckWakeupApi)
            {
                result = Can_TrcvApi[canTrcvDrvId].CanTrcvCheckWakeupApi(canTrcvId);
                if (E_OK == result)
                {
                    break;
                }
            }
        }

        ++trcvPbConfigPtr;
        ++trcvConfigPtr;
    }
    return result;
}
#endif

/**
 * This service checks, whether an underlying CAN driver or a CAN
 * transceiver driver already signals a wakeup event.
 */
Std_ReturnType CanIf_CheckWakeup(EcuM_WakeupSourceType WakeupSource)
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (E_OK == CanIf_ValidateCheckWakeup(WakeupSource))
#endif
    {
#if ((STD_ON == CANIF_CANDRV_WAKE_UP_SUPPORT) || (STD_ON == CANIF_CANTRCV_WAKE_UP_SUPPORT))
        ApplicationType partitionIndex     = CanIf_GetCurrentPartition();
        boolean         controllerNotSleep = FALSE;

#if (STD_ON == CANIF_CANDRV_WAKE_UP_SUPPORT)
        const CanIf_CtrlPbConfigType* ctrlPbConfigPtr = &CANIF_CTRL_PBCFG(0u);
        const CanIf_CtrlConfigType*   ctrlConfigPtr   = &CanIf_CtrlCfgData[0u];

        for (uint8 ctrlIndex = 0u; ctrlIndex < CANIF_CANCONTROLLER_NUMBER; ++ctrlIndex)
        {
            EcuM_WakeupSourceType canCtrlSource = ctrlConfigPtr->CanIfWakeUpSource;

            if ((CANIF_PARTITION_INDEX(ctrlPbConfigPtr) == partitionIndex) && (ctrlConfigPtr->CanIfCtrlWakeupSupport)
                && (0u != (WakeupSource & canCtrlSource)))
            {
                result = CanIf_CheckCtrlWakeupHandle(ctrlPbConfigPtr, ctrlConfigPtr, &controllerNotSleep);
                if (controllerNotSleep || (E_OK == result))
                {
                    break;
                }
            }

            ++ctrlPbConfigPtr;
            ++ctrlConfigPtr;
        }
#endif
#if (STD_ON == CANIF_CANTRCV_WAKE_UP_SUPPORT)
        if ((!controllerNotSleep) && (E_NOT_OK == result))
        {
            result = CanIf_CheckTrcvWakeup(partitionIndex, WakeupSource);
        }
#endif
#endif
    }
    return result;
}
#endif

#if (STD_ON == CANIF_PUBLIC_WAKEUP_CHECK_VALID_SUPPORT)
CANIF_LOCAL Std_ReturnType
    CanIf_CheckValidationHandle(const CanIf_CtrlPbConfigType* ctrlPbConfigPtr, EcuM_WakeupSourceType wakeupSource)
{
    Std_ReturnType               result         = E_NOT_OK;
    const CanIf_CtrlRuntimeType* ctrlRuntimePtr = ctrlPbConfigPtr->RuntimePtr;

    if ((CAN_CS_STARTED == ctrlRuntimePtr->ControllerMode) && (ctrlRuntimePtr->FirstCallRxInd))
    {
        if (NULL_PTR != CanIf_DispatchConfigData.CanIfDispatchUserValidateWakeupEventName)
        {
            CanIf_DispatchConfigData.CanIfDispatchUserValidateWakeupEventName(wakeupSource);
        }
        result = E_OK;
    }
    return result;
}

/**
 * This service is performed to validate a previous wakeup event.
 */
Std_ReturnType CanIf_CheckValidation(EcuM_WakeupSourceType WakeupSource)
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (E_OK == CanIf_ValidateCheckValidation(WakeupSource))
#endif
    {
        ApplicationType               partitionIndex  = CanIf_GetCurrentPartition();
        const CanIf_CtrlPbConfigType* ctrlPbConfigPtr = &CANIF_CTRL_PBCFG(0u);
        const CanIf_CtrlConfigType*   ctrlConfigPtr   = &CanIf_CtrlCfgData[0u];

        for (uint8 ctrlIndex = 0u; ctrlIndex < CANIF_CANCONTROLLER_NUMBER; ++ctrlIndex)
        {
            EcuM_WakeupSourceType canCtrlSource = ctrlConfigPtr->CanIfWakeUpSource;

            if ((CANIF_PARTITION_INDEX(ctrlPbConfigPtr) == partitionIndex) && (0u != (WakeupSource & canCtrlSource)))
            {
                result = CanIf_CheckValidationHandle(ctrlPbConfigPtr, WakeupSource & canCtrlSource);
                if (E_OK == result)
                {
                    break;
                }
            }

            ++ctrlPbConfigPtr;
            ++ctrlConfigPtr;
        }
    }
    return result;
}
#endif

#if (STD_ON == CANIF_PUBLIC_TX_CONFIRM_POLLING_SUPPORT)
/**
 * This service reports, if any TX confirmation has been done for the whole
 * CAN controller since the last CAN controller start.
 */
CanIf_NotifStatusType CanIf_GetTxConfirmationState(uint8 ControllerId)
{
    CanIf_NotifStatusType result = CANIF_NO_NOTIFICATION;

#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (E_OK == CanIf_ValidateGetTxConfirmationState(ControllerId))
#endif
    {
        const CanIf_CtrlPbConfigType* ctrlPbConfigPtr = &CANIF_CTRL_PBCFG(ControllerId);
        CanIf_CtrlRuntimeType*        ctrlRuntimePtr  = ctrlPbConfigPtr->RuntimePtr;

        SchM_Enter_CanIf_ExclusiveArea_Channel();
        /* get the  TX confirmation state of the corresponding controller */
        result = ctrlRuntimePtr->TxConfirmationState;
        /* clear the state */
        ctrlRuntimePtr->TxConfirmationState = CANIF_NO_NOTIFICATION;
        SchM_Exit_CanIf_ExclusiveArea_Channel();
    }
    return result;
}
#endif

#if ((STD_ON == CANIF_PUBLIC_PN_SUPPORT) && (STD_ON == CANIF_PUBLIC_TRCV_PN_SUPPORT))
/**
 * Requests the CanIf module to clear the WUF flag of the designated CAN transceiver.
 */
Std_ReturnType CanIf_ClearTrcvWufFlag(uint8 TransceiverId)
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (E_OK == CanIf_ValidateClearTrcvWufFlag(TransceiverId))
#endif
    {
        const CanIf_TrcvConfigType* trcvConfigPtr = &CanIf_TrcvCfgData[TransceiverId];
        uint8                       canTrcvDrvId  = trcvConfigPtr->CanTrcvDriverId;
        uint8                       canTrcvId     = trcvConfigPtr->CanTrcvId;

        result = Can_TrcvApi[canTrcvDrvId].CanTrcvClearTrcvWufFlagApi(canTrcvId);
    }
    return result;
}

/**
 * Requests the CanIf module to check the Wake flag of the designated CAN transceiver.
 */
Std_ReturnType CanIf_CheckTrcvWakeFlag(uint8 TransceiverId)
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (E_OK == CanIf_ValidateCheckTrcvWakeFlag(TransceiverId))
#endif
    {
        const CanIf_TrcvConfigType* trcvConfigPtr = &CanIf_TrcvCfgData[TransceiverId];
        uint8                       canTrcvDrvId  = trcvConfigPtr->CanTrcvDriverId;
        uint8                       canTrcvId     = trcvConfigPtr->CanTrcvId;

        result = Can_TrcvApi[canTrcvDrvId].CanTrcvCheckWakeFlagApi(canTrcvId);
    }
    return result;
}
#endif

#if (STD_ON == CANIF_SET_BAUDRATE_API)
/**
 * This service shall set the baud rate configuration of the CAN controller.
 * Depending on necessary baud rate modifications the controller might have to reset.
 */
Std_ReturnType CanIf_SetBaudrate(uint8 ControllerId, uint16 BaudRateConfigID)
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (E_OK == CanIf_ValidateSetBaudrate(ControllerId, BaudRateConfigID))
#endif
    {
        const CanIf_CtrlConfigType* ctrlConfigPtr = &CanIf_CtrlCfgData[ControllerId];
        uint8                       canDrvId      = ctrlConfigPtr->CanDriverId;
        uint8                       canCtrlId     = ctrlConfigPtr->CanCtrlId;

        result = Can_DriverApi[canDrvId].CanSetBaudrateApi(canCtrlId, BaudRateConfigID);
    }
    return result;
}
#endif

#if (CANIF_CAN_AUTOSAR_VERSION >= CANIF_CAN_AUTOSAR_R431)
Std_ReturnType CanIf_GetControllerErrorState(uint8 ControllerId, Can_ErrorStateType* ErrorStatePtr)
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (E_OK == CanIf_ValidateGetControllerErrorState(ControllerId, ErrorStatePtr))
#endif
    {
        const CanIf_CtrlConfigType* ctrlConfigPtr = &CanIf_CtrlCfgData[ControllerId];
        uint8                       canDrvId      = ctrlConfigPtr->CanDriverId;
        uint8                       canCtrlId     = ctrlConfigPtr->CanCtrlId;

        result = Can_DriverApi[canDrvId].CanGetControllerErrorStateApi(canCtrlId, ErrorStatePtr);
    }
    return result;
}
#endif

#if (CANIF_CAN_AUTOSAR_VERSION >= CANIF_CAN_AUTOSAR_R440)
Std_ReturnType CanIf_GetControllerRxErrorCounter(uint8 ControllerId, uint8* RxErrorCounterPtr)
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (E_OK == CanIf_ValidateGetControllerRxErrorCounter(ControllerId, RxErrorCounterPtr))
#endif
    {
        const CanIf_CtrlConfigType* ctrlConfigPtr = &CanIf_CtrlCfgData[ControllerId];
        uint8                       canDrvId      = ctrlConfigPtr->CanDriverId;
        uint8                       canCtrlId     = ctrlConfigPtr->CanCtrlId;

        result = Can_DriverApi[canDrvId].CanGetControllerRxErrorCounterApi(canCtrlId, RxErrorCounterPtr);
    }
    return result;
}

Std_ReturnType CanIf_GetControllerTxErrorCounter(uint8 ControllerId, uint8* TxErrorCounterPtr)
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (E_OK == CanIf_ValidateGetControllerTxErrorCounter(ControllerId, TxErrorCounterPtr))
#endif
    {
        const CanIf_CtrlConfigType* ctrlConfigPtr = &CanIf_CtrlCfgData[ControllerId];
        uint8                       canDrvId      = ctrlConfigPtr->CanDriverId;
        uint8                       canCtrlId     = ctrlConfigPtr->CanCtrlId;

        result = Can_DriverApi[canDrvId].CanGetControllerTxErrorCounterApi(canCtrlId, TxErrorCounterPtr);
    }
    return result;
}
#endif

#if (STD_ON == CANIF_TRIGGER_TRANSMIT_SUPPORT)
/**
 * Within this API, the upper layer module (called module) shall check
 * whether the available data fits into the buffer size reported by PduInfoPtr-
 * >SduLength. If it fits, it shall copy its data into the buffer provided by
 * PduInfoPtr->SduDataPtr and update the length of the actual copied data in
 * PduInfoPtr->SduLength. If not, it returns E_NOT_OK without changing PduInfoPtr.
 */
Std_ReturnType CanIf_TriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr)
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (E_OK == CanIf_ValidateTriggerTransmit(TxPduId, PduInfoPtr))
#endif
    {
#if (STD_ON == CANIF_UP_TRIGGER_TRANSMIT_ENABLE)
        PduIdType                    txPduIndex     = CanIf_TxPduId2VarIndex(TxPduId);
        const CanIf_TxPduConfigType* txPduConfigPtr = &CANIF_TXPDU(txPduIndex);

        if (txPduConfigPtr->UpTriggerTransmitApiIndex < CANIF_UPTRIGGERTRANSMIT_API_NUMBER)
        {
            result = CanIf_UpTriggerTransmitArray[txPduConfigPtr->UpTriggerTransmitApiIndex](
                txPduConfigPtr->CanIfUpPduId,
                PduInfoPtr);
        }
#endif
    }
    return result;
}
#endif

/**
 * This service confirms a previously successfully processed transmission of a CAN TxPDU.
 */
void CanIf_TxConfirmation(PduIdType CanTxPduId)
{
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (E_OK == CanIf_ValidateTxConfirmation(CanTxPduId))
#endif
    {
        PduIdType                     txPduIndex       = CanIf_TxPduId2VarIndex(CanTxPduId);
        const CanIf_TxPduConfigType*  txPduConfigPtr   = &CANIF_TXPDU(txPduIndex);
        Can_HwHandleType              hthIndex         = txPduConfigPtr->CanIfTxPduHthId;
        const CanIf_HthConfigType*    hthConfigPtr     = &CANIF_HTH(hthIndex);
        Can_HwHandleType              ctrlIndex        = hthConfigPtr->CanIfHthCanCtrlId;
        const CanIf_CtrlPbConfigType* ctrlPbConfigPtr  = &CANIF_CTRL_PBCFG(ctrlIndex);
        CanIf_CtrlRuntimeType*        ctrlRuntimePtr   = ctrlPbConfigPtr->RuntimePtr;
        boolean                       unconfirmedTxPdu = FALSE;

        SchM_Enter_CanIf_ExclusiveArea_Channel();
#if (STD_ON == CANIF_PUBLIC_TX_BUFFERING)
        CanIf_TransmitBufferedPdu(txPduConfigPtr);
#endif

        if (CAN_CS_STARTED == ctrlRuntimePtr->ControllerMode)
        {
#if (STD_ON == CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API)
            CanIf_SetTxNotifStatusHandle(txPduConfigPtr);
#endif
#if (STD_ON == CANIF_PUBLIC_TX_CONFIRM_POLLING_SUPPORT)
            ctrlRuntimePtr->TxConfirmationState = CANIF_TX_RX_NOTIFICATION;
#endif
        }

        if (NULL_PTR != txPduConfigPtr->UpConfirmTxPduPtr)
        {
            unconfirmedTxPdu = CanIf_ClearUpConfirmTxPduStatus(txPduConfigPtr);
        }
        SchM_Exit_CanIf_ExclusiveArea_Channel();

        if (unconfirmedTxPdu)
        {
            /* call upper layer function,feedback to upper layer*/
            CanIf_UpTxConfirmationArray[txPduConfigPtr->UpTxConfirmationApiIndex](txPduConfigPtr->CanIfUpPduId, E_OK);
        }

#if (STD_ON == CANIF_PUBLIC_PN_SUPPORT)
        const CanIf_CtrlConfigType* ctrlConfigPtr = &CanIf_CtrlCfgData[ctrlIndex];
        if (ctrlConfigPtr->CanIfPnFilter)
        {
            ctrlRuntimePtr->PnTxFilter = FALSE;
        }
#endif
    }
}

/**
 * This service indicates a successful reception of a received CAN Rx LPDU
 * to the CanIf after passing all filters and validation checks.
 */
void CanIf_RxIndication(const Can_HwType* Mailbox, const PduInfoType* PduInfoPtr)
{
#if ((STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT) || (CANIF_VARIANT_NUMBER > 1u))
    if (E_OK == CanIf_ValidateRxIndication(Mailbox, PduInfoPtr))
#endif
    {
        uint8                         ctrlIndex       = CanIf_CanCtrlId2Index[Mailbox->ControllerId];
        const CanIf_CtrlPbConfigType* ctrlPbConfigPtr = &CANIF_CTRL_PBCFG(ctrlIndex);
        CanIf_CtrlRuntimeType* ctrlRuntimePtr = ctrlPbConfigPtr->RuntimePtr; /* PRQA S 3678 */ /* VL_CanIf_3678 */
        CanIf_IdType           canId          = (CanIf_IdType)Mailbox->CanId;
#if (CANIF_CAN_IDTYPE_UINT16 == CANIF_CAN_IDTYPE)
        if ((canId & CANIF_CANID16_CANFD_MASK) != 0u)
        {
            /*Standard Fd frame*/
            canId = (canId & ~CANIF_CANID16_CANFD_MASK) | CANIF_CANID32_CANFD_MASK;
        }
#endif
#if (STD_ON == CANIF_RXINDICATION_AUTOSAR_PARAMETER)
        Can_HwHandleType hrhIndex = Mailbox->Hoh;
#else
        Can_HwHandleType hrhIndex = CANIF_HOH2HRH(ctrlIndex).CanHrhIdRef[Mailbox->Hoh];
#endif
        PduIdType rxPduIndex = CANIF_PDUID_INVALID;

        if (E_OK == CanIf_CanIdRangCheck(hrhIndex, canId, &rxPduIndex))
        {
            const CanIf_RxPduConfigType* rxPduConfigPtr = &CANIF_RXPDU(rxPduIndex);

            /* DLC check enabled */
#if (STD_ON == CANIF_PRIVATE_DLC_CHECK)
            if (PduInfoPtr->SduLength < rxPduConfigPtr->CanIfRxPduDlc)
            {
                CanIf_DetReportRuntimeError(CANIF_RXINDICATION_ID, CANIF_E_INVALID_DATA_LENGTH);
            }
            else
#endif
            {
                CanIf_RxIndicationHandle(
                    ctrlRuntimePtr,
                    rxPduConfigPtr,
#if (STD_ON == CANIF_META_DATA_SUPPORT)
                    canId,
#endif
                    PduInfoPtr);
            }

#if (STD_ON == CANIF_PUBLIC_WAKEUP_CHECK_VALID_SUPPORT)
#if (STD_ON == CANIF_PUBLIC_WAKEUP_CHECK_VALID_BY_NM)
            if (rxPduConfigPtr->CanIfRxPduForNM)
            {
                /* set first call rxindication event flag */
                ctrlRuntimePtr->FirstCallRxInd = TRUE;
            }
#else
            /* set first call rxindication event flag */
            ctrlRuntimePtr->FirstCallRxInd = TRUE;
#endif
#endif
#if (STD_ON == CANIF_PUBLIC_PN_SUPPORT)
            const CanIf_CtrlConfigType* ctrlConfigPtr = &CanIf_CtrlCfgData[ctrlIndex];
            if (ctrlConfigPtr->CanIfPnFilter)
            {
                ctrlRuntimePtr->PnTxFilter = FALSE;
            }
#endif
        }
    }

#if (STD_ON == CANIF_CDD_CANIDS_MODULE_ENABLE)
    CDD_CanIds_RxIndication(
        (uint32)Mailbox->CanId,
        PduInfoPtr->SduDataPtr,
        (uint8)PduInfoPtr->SduLength,
        Mailbox->ControllerId);
#endif
}

/**
 * This service indicates a Controller BusOff event referring to
 * the corresponding CAN Controller with the abstract CanIf ControllerId.
 */
void CanIf_ControllerBusOff(uint8 ControllerId)
{
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (E_OK == CanIf_ValidateControllerBusOff(ControllerId))
#endif
    {
        uint8                         ctrlIndex       = CanIf_CanCtrlId2Index[ControllerId];
        const CanIf_CtrlPbConfigType* ctrlPbConfigPtr = &CANIF_CTRL_PBCFG(ctrlIndex);
        CanIf_CtrlRuntimeType*        ctrlRuntimePtr  = ctrlPbConfigPtr->RuntimePtr;

        SchM_Enter_CanIf_ExclusiveArea_Channel();
#if (STD_ON == CANIF_PUBLIC_TX_CONFIRM_POLLING_SUPPORT)
        ctrlRuntimePtr->TxConfirmationState = CANIF_NO_NOTIFICATION;
#endif
        /* set controller to STOP status */
        ctrlRuntimePtr->ControllerMode = CAN_CS_STOPPED;
        /*set the PDU channel mode of the corresponding channel to CANIF_TX_OFFLINE*/
        ctrlRuntimePtr->PduMode = CANIF_TX_OFFLINE;
/* free tx buffer if enabled */
#if (STD_ON == CANIF_PUBLIC_TX_BUFFERING)
        CanIf_FreeControllerTxBuffer(ctrlPbConfigPtr);
#endif
        SchM_Exit_CanIf_ExclusiveArea_Channel();

        CanIf_NotifyNegativeTxConfirmation(ctrlPbConfigPtr);
        if (NULL_PTR != CanIf_DispatchConfigData.CanIfDispatchUserCtrlBusOffName)
        {
            CanIf_DispatchConfigData.CanIfDispatchUserCtrlBusOffName(ctrlIndex);
        }
    }
}

#if ((STD_ON == CANIF_PUBLIC_PN_SUPPORT) && (STD_ON == CANIF_PUBLIC_TRCV_PN_SUPPORT))
/**
 * This service indicates that the transceiver is running in PN
 * communication mode referring to the corresponding CAN transceiver
 * with the abstract CanIf TransceiverId.
 */
void CanIf_ConfirmPnAvailability(uint8 TransceiverId)
{
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (E_OK == CanIf_ValidateConfirmPnAvailability(TransceiverId))
#endif
    {
        uint8 trcvIndex = CanIf_CanTrcvId2Index[TransceiverId];

        if (NULL_PTR != CanIf_DispatchConfigData.CanIfDispatchUserConfirmPnAvailabilityName)
        {
            CanIf_DispatchConfigData.CanIfDispatchUserConfirmPnAvailabilityName(trcvIndex);
        }
    }
}

/**
 * This service indicates that the transceiver has cleared the WufFlag
 * referring to the corresponding CAN transceiver with the abstract CanIf TransceiverId.
 */
void CanIf_ClearTrcvWufFlagIndication(uint8 TransceiverId)
{
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (E_OK == CanIf_ValidateClearTrcvWufFlagIndication(TransceiverId))
#endif
    {
        uint8 trcvIndex = CanIf_CanTrcvId2Index[TransceiverId];

        if (NULL_PTR != CanIf_DispatchConfigData.CanIfDispatchUserClearTrcvWufFlagIndicationName)
        {
            CanIf_DispatchConfigData.CanIfDispatchUserClearTrcvWufFlagIndicationName(trcvIndex);
        }
    }
}

/**
 * This service indicates that the check of the transceiver!/s wake-up flag
 * has been finished by the corresponding CAN transceiver with the abstract
 * CanIf TransceiverId. This indication is used to cope with the asynchronous
 * transceiver communication.
 */
void CanIf_CheckTrcvWakeFlagIndication(uint8 TransceiverId)
{
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (E_OK == CanIf_ValidateCheckTrcvWakeFlagIndication(TransceiverId))
#endif
    {
        uint8 trcvIndex = CanIf_CanTrcvId2Index[TransceiverId];

        if (NULL_PTR != CanIf_DispatchConfigData.CanIfDispatchUserCheckTrcvWakeFlagIndicationName)
        {
            CanIf_DispatchConfigData.CanIfDispatchUserCheckTrcvWakeFlagIndicationName(trcvIndex);
        }
    }
}
#endif

/**
 * This service indicates a controller state transition referring
 * to the corresponding CAN controller with the abstract CanIf ControllerId.
 */
void CanIf_ControllerModeIndication(uint8 ControllerId, Can_ControllerStateType ControllerMode)
{
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (E_OK == CanIf_ValidateControllerModeIndication(ControllerId, ControllerMode))
#endif
    {
        uint8                         ctrlIndex       = CanIf_CanCtrlId2Index[ControllerId];
        const CanIf_CtrlPbConfigType* ctrlPbConfigPtr = &CANIF_CTRL_PBCFG(ctrlIndex);
        CanIf_CtrlRuntimeType*        ctrlRuntimePtr  = ctrlPbConfigPtr->RuntimePtr;

        ctrlRuntimePtr->ControllerMode = ControllerMode;
        if (NULL_PTR != CanIf_DispatchConfigData.CanIfDispatchUserCtrlModeIndicationName)
        {
            CanIf_DispatchConfigData.CanIfDispatchUserCtrlModeIndicationName(ctrlIndex, ControllerMode);
        }
    }
}

#if (CANIF_TRCV_NUMBER > 0u)
/**
 * This service indicates a transceiver state transition referring
 * to the corresponding CAN transceiver with the abstract CanIf TransceiverId.
 */
void CanIf_TrcvModeIndication(uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode)
{
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (E_OK == CanIf_ValidateTrcvModeIndication(TransceiverId, TransceiverMode))
#endif
    {
        uint8 trcvIndex = CanIf_CanTrcvId2Index[TransceiverId];

        if (NULL_PTR != CanIf_DispatchConfigData.CanIfDispatchUserTrcvModeIndicationName)
        {
            CanIf_DispatchConfigData.CanIfDispatchUserTrcvModeIndicationName(trcvIndex, TransceiverMode);
        }
    }
}
#endif

/* ========================================== internal function definitions ========================================= */
/**
 * Set controller state to started
 */
CANIF_LOCAL Std_ReturnType CanIf_SetControllStarted(uint8 canDrvId, uint8 canCtrlId)
{
    Std_ReturnType result = E_NOT_OK;

#if (CANIF_CAN_AUTOSAR_VERSION >= CANIF_CAN_AUTOSAR_R431)
    if (CAN_OK == Can_DriverApi[canDrvId].CanSetControllerModeApi(canCtrlId, CAN_CS_STARTED))
#else /*The default version of can driver is 4.2.2*/
    if (CAN_OK == Can_DriverApi[canDrvId].CanSetControllerModeApi(canCtrlId, CAN_T_START))
#endif
    {
        result = E_OK;
    }
    return result;
}

/**
 * Set controller state to sleep
 */
CANIF_LOCAL Std_ReturnType CanIf_SetControllSleep(
    const CanIf_CtrlConfigType*   ctrlConfigPtr,
    const CanIf_CtrlPbConfigType* ctrlPbConfigPtr,
    CanIf_CtrlRuntimeType*        ctrlRuntimePtr,
    uint8                         canDrvId,
    uint8                         canCtrlId)
{
    Std_ReturnType result = E_NOT_OK;

    CANIF_NOUSED(ctrlConfigPtr);

#if (CANIF_CAN_AUTOSAR_VERSION >= CANIF_CAN_AUTOSAR_R431)
    if (CAN_OK == Can_DriverApi[canDrvId].CanSetControllerModeApi(canCtrlId, CAN_CS_SLEEP))
#else /*The default version of can driver is 4.2.2*/
    if (CAN_OK == Can_DriverApi[canDrvId].CanSetControllerModeApi(canCtrlId, CAN_T_SLEEP))
#endif
    {
        SchM_Enter_CanIf_ExclusiveArea_Channel();
#if (STD_ON == CANIF_PUBLIC_WAKEUP_CHECK_VALID_SUPPORT)
        /* Clear first call rxindication event flag While Sleep */
        ctrlRuntimePtr->FirstCallRxInd = FALSE;
#endif
        /*set the PDU channel mode of the corresponding channel to CANIF_OFFLINE*/
        ctrlRuntimePtr->PduMode = CANIF_OFFLINE;
/* free tx buffer if enabled */
#if (STD_ON == CANIF_PUBLIC_TX_BUFFERING)
        CanIf_FreeControllerTxBuffer(ctrlPbConfigPtr);
#endif
#if (STD_ON == CANIF_PUBLIC_PN_SUPPORT)
        if (ctrlConfigPtr->CanIfPnFilter)
        {
            ctrlRuntimePtr->PnTxFilter = TRUE;
        }
#endif
        SchM_Exit_CanIf_ExclusiveArea_Channel();

        CanIf_NotifyNegativeTxConfirmation(ctrlPbConfigPtr);
        result = E_OK;
    }
    return result;
}

/**
 * Set controller state to stopped
 */
CANIF_LOCAL Std_ReturnType CanIf_SetControllStopped(
    const CanIf_CtrlPbConfigType* ctrlPbConfigPtr,
    CanIf_CtrlRuntimeType*        ctrlRuntimePtr,
    uint8                         canDrvId,
    uint8                         canCtrlId)
{
    Std_ReturnType result = E_NOT_OK;

    /* current mode is SLEEP */
    if (CAN_CS_SLEEP == ctrlRuntimePtr->ControllerMode)
    {
        /* wake up the controller */
#if (CANIF_CAN_AUTOSAR_VERSION >= CANIF_CAN_AUTOSAR_R431)
        if (CAN_OK == Can_DriverApi[canDrvId].CanSetControllerModeApi(canCtrlId, CAN_CS_STOPPED))
#else /*The default version of can driver is 4.2.2*/
        if (CAN_OK == Can_DriverApi[canDrvId].CanSetControllerModeApi(canCtrlId, CAN_T_WAKEUP))
#endif
        {
            /*set the PDU channel mode of the corresponding channel to CANIF_TX_OFFLINE*/
            ctrlRuntimePtr->PduMode = CANIF_TX_OFFLINE;
            result                  = E_OK;
        }
    }
    else
    {
        /* set controller mode STOP */
#if (CANIF_CAN_AUTOSAR_VERSION >= CANIF_CAN_AUTOSAR_R431)
        if (CAN_OK == Can_DriverApi[canDrvId].CanSetControllerModeApi(canCtrlId, CAN_CS_STOPPED))
#else /*The default version of can driver is 4.2.2*/
        if (CAN_OK == Can_DriverApi[canDrvId].CanSetControllerModeApi(canCtrlId, CAN_T_STOP))
#endif
        {
            SchM_Enter_CanIf_ExclusiveArea_Channel();
            /*set the PDU channel mode of the corresponding channel to CANIF_TX_OFFLINE*/
            ctrlRuntimePtr->PduMode = CANIF_TX_OFFLINE;
#if (STD_ON == CANIF_PUBLIC_TX_BUFFERING)
            CanIf_FreeControllerTxBuffer(ctrlPbConfigPtr);
#endif
#if (STD_ON == CANIF_PUBLIC_READ_RX_PDU_DATA_API)
            CanIf_FreeControllerRxBuffer(ctrlPbConfigPtr);
#endif
#if ((STD_ON == CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API) || (STD_ON == CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API))
            CanIf_FreeRxTxNotifyStatus(ctrlPbConfigPtr);
#endif
#if (STD_ON == CANIF_PUBLIC_TX_CONFIRM_POLLING_SUPPORT)
            ctrlRuntimePtr->TxConfirmationState = CANIF_NO_NOTIFICATION;
#endif
            CanIf_FreeDynamicTxPduCanIds(ctrlPbConfigPtr);
            SchM_Exit_CanIf_ExclusiveArea_Channel();

            CanIf_NotifyNegativeTxConfirmation(ctrlPbConfigPtr);
            result = E_OK;
        }
    }

    return result;
}

/**
 * Check (Basic Can) CanId and find the rxpduId
 */
CANIF_LOCAL Std_ReturnType CanIf_CanIdRangCheck(Can_HwHandleType hrhIndex, CanIf_IdType canId, PduIdType* rxPduIndexPtr)
{
    Std_ReturnType             result       = E_NOT_OK;
    boolean                    isSearch     = FALSE;
    const CanIf_HrhConfigType* hrhConfigPtr = &CANIF_HRH(hrhIndex);
#if (STD_ON == CANIF_HRH_CANID_RANGE_SUPPORT)
    PduIdType hrhCanIdRangeIndex  = hrhConfigPtr->HrhCanIdRangeIndex;
    PduIdType hrhCanIdRangeNumber = hrhConfigPtr->HrhCanIdRangeNumber;

    /* if the hrh defined as Basic Can and sofware filter enabled */
    if ((CANIF_BASIC_CAN == hrhConfigPtr->CanIfHrhType) && (hrhCanIdRangeNumber > 0u))
    {
        const CanIf_HrhCanIdRangeType* hrhCanIdRangePtr = &CanIf_HrhCanIdRangeData[hrhCanIdRangeIndex];

        /* do HRH software filter*/
        for (PduIdType rngIndex = hrhCanIdRangeIndex; rngIndex < (hrhCanIdRangeIndex + hrhCanIdRangeNumber); ++rngIndex)
        {
#if (CANIF_CAN_IDTYPE_UINT16 == CANIF_CAN_IDTYPE)
            if (CANIF_CANID_STANDARD == hrhCanIdRangePtr->HrhRangeRxPduRangeCanIdType)
#else
            /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
            boolean matchStandardType =
                ((0x80000000uL > canId) && (CANIF_CANID_STANDARD == hrhCanIdRangePtr->HrhRangeRxPduRangeCanIdType));
            boolean matchExtendedType =
                ((0x80000000uL <= canId) && (CANIF_CANID_EXTENDED == hrhCanIdRangePtr->HrhRangeRxPduRangeCanIdType));
            /* PRQA S 3120 -- */
            if (matchStandardType || matchExtendedType)
#endif
            {
                Can_IdType canIdMasked;

                if (CANIF_CANID_STANDARD == hrhCanIdRangePtr->HrhRangeRxPduRangeCanIdType)
                {
                    canIdMasked = canId & CANIF_STANDARD_CANID_MAX;
                }
                else
                {
                    canIdMasked = canId & CANIF_EXTENDED_CANID_MAX;
                }

                boolean passMaskFilter =
                    ((hrhCanIdRangePtr->HrhRangeMask > 0u)
                     && ((canIdMasked & hrhCanIdRangePtr->HrhRangeMask) == hrhCanIdRangePtr->HrhRangeBaseId));
                boolean passRangeFilter =
                    ((canIdMasked >= hrhCanIdRangePtr->HrhRangeRxPduLowerCanId)
                     && (canIdMasked <= hrhCanIdRangePtr->HrhRangeRxPduUpperCanId));
                if (passMaskFilter || passRangeFilter)
                {
                    isSearch = TRUE;
                    break;
                }
            }

            ++hrhCanIdRangePtr;
        }
    }
    else
#endif
    {
        isSearch = TRUE;
    }
    if (isSearch) /* PRQA S 2991 */ /* VL_CanIf_2991 */
    {
#if (CANIF_SOFTWARE_FILTER_TYPE == CANIF_SOFTWARE_FILTER_BINARY)
        if (hrhConfigPtr->CanIfSoftFilterType == CANIF_BINARY)
        {
            result = CanIf_RxPduHrhSearchByBinary(hrhConfigPtr, canId, rxPduIndexPtr);
        }
        else
#endif
        {
            result = CanIf_RxPduHrhSearchByLinear(hrhConfigPtr, canId, rxPduIndexPtr);
        }
    }
    return result;
}

/**
 * init all buffer
 */
CANIF_LOCAL void CanIf_InitAllBuffer(ApplicationType partitionIndex)
{
/* free tx buffer if enabled */
#if (STD_ON == CANIF_PUBLIC_TX_BUFFERING)
    CanIf_InitTxBuffer(partitionIndex);
#endif
/* (re-)init the rx Buffer */
#if (STD_ON == CANIF_PUBLIC_READ_RX_PDU_DATA_API)
    CanIf_InitRxBuffer(partitionIndex);
#endif
/* re-init the txNotifStatus of this controller */
/* re-init the rxNotifStatus of this controller */
#if ((STD_ON == CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API) || (STD_ON == CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API))
    CanIf_InitRxTxNotifyStatus(partitionIndex);
#endif
    /* init the DynamicTxPduCanId with the configured canId */
    CanIf_InitDynamicTxPduCanId(partitionIndex);
    CanIf_InitUpConfirmTxPduStatus(partitionIndex);
}

/**
 * Linear search the index of RxPdu Config based on the Hrh and CanId
 */
CANIF_LOCAL Std_ReturnType
    CanIf_RxPduHrhSearchByLinear(const CanIf_HrhConfigType* hrhConfigPtr, CanIf_IdType canId, PduIdType* rxPduIndexPtr)
{
    Std_ReturnType result = E_NOT_OK;

    if (hrhConfigPtr->RxPduIndexMin < hrhConfigPtr->RxPduIndexMaxExcluded)
    {
        PduIdType                    rxPduIndex     = hrhConfigPtr->RxPduIndexMin;
        const CanIf_RxPduConfigType* rxPduConfigPtr = &CANIF_RXPDU(rxPduIndex);

        for (; rxPduIndex < hrhConfigPtr->RxPduIndexMaxExcluded; ++rxPduIndex)
        {
            if (rxPduConfigPtr->CanIfRxPduCanIdMasked == (rxPduConfigPtr->CanIfRxPduCanIdMask & canId))
            {
                result = CanIf_GetPduHrh(rxPduConfigPtr, canId);
                if (E_OK == result)
                {
                    *rxPduIndexPtr = rxPduIndex;
                    break;
                }
            }

            ++rxPduConfigPtr;
        }
    }

    return result;
}

#if (CANIF_SOFTWARE_FILTER_TYPE == CANIF_SOFTWARE_FILTER_BINARY)
/**
 * Binary search the index of RxPdu Config based on the Hrh and CanId
 */
CANIF_LOCAL Std_ReturnType
    CanIf_RxPduHrhSearchByBinary(const CanIf_HrhConfigType* hrhConfigPtr, CanIf_IdType canId, PduIdType* rxPduIndexPtr)
{
    Std_ReturnType result                = E_NOT_OK;
    PduIdType      rxPduIndexMin         = hrhConfigPtr->RxPduIndexMin;
    PduIdType      rxPduIndexMaxExcluded = hrhConfigPtr->RxPduIndexMaxExcluded;

    if (rxPduIndexMin < rxPduIndexMaxExcluded)
    {
        const CanIf_RxPduConfigType* rxPduConfigPtr = &CANIF_RXPDU(rxPduIndexMin);
        /* Using binary search to find indicate that all Pdu associated with HRH using the same Mask value. */
        CanIf_IdType hrhCanIdMasked = rxPduConfigPtr->CanIfRxPduCanIdMask & canId;

        while (rxPduIndexMin < rxPduIndexMaxExcluded)
        {
            PduIdType    rxPduIndex = (rxPduIndexMaxExcluded - 1u + rxPduIndexMin) >> 1u;
            CanIf_IdType rxPduCanIdMasked;

            rxPduConfigPtr   = &CANIF_RXPDU(rxPduIndex);
            rxPduCanIdMasked = rxPduConfigPtr->CanIfRxPduCanIdMasked;

            if (hrhCanIdMasked == rxPduCanIdMasked)
            {
                result = CanIf_GetPduHrh(rxPduConfigPtr, canId);
                if (E_OK == result)
                {
                    *rxPduIndexPtr = rxPduIndex;
                }
                break;
            }
            else if (hrhCanIdMasked > rxPduCanIdMasked)
            {
                rxPduIndexMin = rxPduIndex + 1u;
            }
            else
            {
                rxPduIndexMaxExcluded = rxPduIndex;
            }
        }
    }

    return result;
}
#endif

/* PRQA S 5017, 3120, 6030 ++ */ /* VL_CanIf_5017, VL_QAC_MagicNum, VL_MTR_CanIf_STMIF */
/**
 * Get the index of RxPdu Config based on the RxPdu Config and CAN ID
 */
CANIF_LOCAL Std_ReturnType CanIf_GetPduHrh(const CanIf_RxPduConfigType* rxPduConfigPtr, CanIf_IdType canId)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == CANIF_RXPDU_CANID_RANGE_SUPPORT)
    PduIdType rxPduCanIdRangeIndex = rxPduConfigPtr->RxPduCanIdRangeIndex;
    if ((rxPduCanIdRangeIndex >= CANIF_RXPDUCANIDRANGE_NUMBER)
        || (((canId & CANIF_EXTENDED_CANID_MAX) >= CanIf_RxPduCanIdRangeData[rxPduCanIdRangeIndex].LowerCanId)
            && ((canId & CANIF_EXTENDED_CANID_MAX) <= CanIf_RxPduCanIdRangeData[rxPduCanIdRangeIndex].UpperCanId)))
#endif
    {
        switch (rxPduConfigPtr->CanIfRxPduCanIdType) /* PRQA S 2002 */ /* VL_QAC_NoDefaultCase */
        {
#if (STD_ON == CANIF_RX_EXTENDED_CAN_SUPPORT)
        case CANIF_RX_EXTENDED_CAN:
            if (canId >= 0x80000000uL)
            {
                result = E_OK;
            }
            break;
#endif
#if (STD_ON == CANIF_RX_EXTENDED_FD_CAN_SUPPORT)
        case CANIF_RX_EXTENDED_FD_CAN:
            if (canId >= 0xc0000000uL)
            {
                result = E_OK;
            }
            break;
#endif
#if (STD_ON == CANIF_RX_EXTENDED_NO_FD_CAN_SUPPORT)
        case CANIF_RX_EXTENDED_NO_FD_CAN:
            if ((canId >= 0x80000000uL) && (canId < 0xc0000000uL))
            {
                result = E_OK;
            }
            break;
#endif
#if (STD_ON == CANIF_RX_STANDARD_CAN_SUPPORT)
        case CANIF_RX_STANDARD_CAN:
            if (canId < 0x80000000uL)
            {
                result = E_OK;
            }
            break;
#endif
#if (STD_ON == CANIF_RX_STANDARD_FD_CAN_SUPPORT)
        case CANIF_RX_STANDARD_FD_CAN:
            if ((canId < 0x80000000uL) && (canId >= 0x40000000uL))
            {
                result = E_OK;
            }
            break;
#endif
#if (STD_ON == CANIF_RX_STANDARD_NO_FD_CAN_SUPPORT)
        case CANIF_RX_STANDARD_NO_FD_CAN:
            if (canId < 0x40000000uL)
            {
                result = E_OK;
            }
            break;
#endif
        default:
            /* do nothing */
            break;
        }
    }
    return result;
}
/* PRQA S 5017, 3120, 6030 -- */

/**
 * do RxIndication Handle with the correct rxPuIndex
 * which is got by hrh and canId through CanIf_CanIdRangCheck
 */
CANIF_LOCAL void CanIf_RxIndicationHandle(
    const CanIf_CtrlRuntimeType* ctrlRuntimePtr,
    const CanIf_RxPduConfigType* rxPduConfigPtr,
#if (STD_ON == CANIF_META_DATA_SUPPORT)
    CanIf_IdType canId,
#endif
    const PduInfoType* pduInfoPtr)
{
    PduInfoType pduInfo       = *pduInfoPtr;
    boolean     indicateRxPdu = FALSE;
#if (                                             \
    (STD_ON == CANIF_PUBLIC_READ_RX_PDU_DATA_API) \
    || ((STD_OFF == CANIF_RXINDICATION_AUTOSAR_PARAMETER) && (STD_ON == CANIF_META_DATA_SUPPORT)))
    boolean      metaDataEnable;
    CanIf_IdType metaData;
#endif

#if ((STD_ON == CANIF_RXINDICATION_AUTOSAR_PARAMETER) || (STD_OFF == CANIF_META_DATA_SUPPORT))
#if (STD_ON == CANIF_PUBLIC_READ_RX_PDU_DATA_API)
#if (STD_ON == CANIF_META_DATA_SUPPORT)
    metaDataEnable = (NULL_PTR != PduInfo.MetaDataPtr);
    if (metaDataEnable)
    {
        metaData = CanIf_MetaDataToCanId(PduInfo.MetaDataPtr);
    }
#endif
#endif
#else
    metaDataEnable = rxPduConfigPtr->RxMetaDataEnable;
    if (metaDataEnable)
    {
        metaData            = canId;
        pduInfo.MetaDataPtr = (uint8*)(&metaData);
    }
    else
    {
        pduInfo.MetaDataPtr = NULL_PTR;
    }
#endif

#if (                                                      \
    (STD_ON == CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API) \
    || ((STD_ON == CANIF_PUBLIC_READ_RX_PDU_DATA_API) && (CANIF_RXBUFFER_MAX_NUMBER > 0u)))
    SchM_Enter_CanIf_ExclusiveArea_Channel();
#endif
    if (CANIF_OFFLINE != ctrlRuntimePtr->PduMode)
    {
        indicateRxPdu = TRUE;
#if (STD_ON == CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API)
        CanIf_SetRxNotifStatusHandle(rxPduConfigPtr);
#endif
#if ((STD_ON == CANIF_PUBLIC_READ_RX_PDU_DATA_API) && (CANIF_RXBUFFER_MAX_NUMBER > 0u))
        ApplicationType          partitionIndex = CANIF_PARTITION_INDEX(rxPduConfigPtr);
        CanIf_RxBufferNumberType rxBufferIndex  = rxPduConfigPtr->RxBufferIndex;

        if (rxBufferIndex < CANIF_RXBUFFER_NUMBER(partitionIndex))
        {
            CanIf_RxBufferType*    rxBufferPtr    = &CANIF_RXBUFFER(partitionIndex, rxBufferIndex);
            CanIf_RxPduBufferType* rxPduBufferPtr = &rxBufferPtr->RxPduBuffer;

            rxPduBufferPtr->Length = (uint8)pduInfo.SduLength;
            if (rxPduBufferPtr->Length > rxBufferPtr->PduLength)
            {
                rxPduBufferPtr->Length = rxBufferPtr->PduLength;
            }
            (void)IStdLib_MemCpy(rxPduBufferPtr->Data, pduInfo.SduDataPtr, rxPduBufferPtr->Length);
            if (metaDataEnable)
            {
                rxPduBufferPtr->MetaData = metaData;
            }
        }
#endif
    }
#if (                                                      \
    (STD_ON == CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API) \
    || ((STD_ON == CANIF_PUBLIC_READ_RX_PDU_DATA_API) && (CANIF_RXBUFFER_MAX_NUMBER > 0u)))
    SchM_Exit_CanIf_ExclusiveArea_Channel();
#endif

    if ((indicateRxPdu) && (rxPduConfigPtr->UpRxIndicationApiIndex < CANIF_UPRXINDICATION_API_NUMBER))
    {
        CanIf_UpRxIndicationArray[rxPduConfigPtr->UpRxIndicationApiIndex](rxPduConfigPtr->CanIfUpPduId, &pduInfo);
    }
}

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
