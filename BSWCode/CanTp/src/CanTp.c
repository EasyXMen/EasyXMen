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
 **  @file               : CanTp.c
 **  @author             : xue.han
 **  @date               : 2024/08/15
 **  @vendor             : isoft
 **  @description        : CAN Transport Layer
 **
 ***********************************************************************************************************************/
/* PRQA S 1503,1532,1505 ++ */ /* VL_QAC_NoUsedApi,VL_QAC_OneFunRef,VL_CanTp_1505 */
/* PRQA S 6520 EOF */          /* VL_MTR_CanTp_STVAR */
/* =================================================== inclusions =================================================== */
#include "PduR_CanTp.h"
#include "SchM_CanTp.h"
#include "CanIf.h"
#include "CanTp.h"
#include "CanTp_Internal.h"
#include "CanTp_Lcfg.h"
#if (CANTP_DEM_PRODUCTION_ERROR_DETECT == STD_ON)
#include "Dem.h"
#endif
#if (CANTP_ECUC_MULTIPLE_PARTITION_EXIST == STD_ON)
#include "Os.h"
#endif
/* ================================================= version define ================================================= */

/* ================================================== version check ================================================= */

/* ============================================ internal data definitions =========================================== */
#define CANTP_START_SEC_VAR_CLEARED_PTR
#include "CanTp_MemMap.h"
/**
 * @brief Module internal variable for PB configure pointer
 * @range NA
 */
const CanTp_ConfigType* CanTp_ConfigPtr;
#define CANTP_STOP_SEC_VAR_CLEARED_PTR
#include "CanTp_MemMap.h"
#define CANTP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanTp_MemMap.h"
/**
 * @brief CANTP global variable for module state:CANTP_ON, CANTP_OFF
 * @range 0..1
 */
CANTP_LOCAL CanTp_StateType CanTp_ModuleState; /* PRQA S 1504 */ /* VL_CanTp_1504 */
#define CANTP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanTp_MemMap.h"

/* ============================================ external data definition ============================================ */

/* ========================================= internal function declarations ========================================= */
/**
 * @brief       This function reset Rx channel status.
 * @param[in]   rxState        : pointer to the received N-PDU status
 * @return      void
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-73122
 */
CANTP_LOCAL void CanTp_InitRxChannel(CanTp_RxStatusType* rxState);
/**
 * @brief       This function reset Tx channel status.
 * @param[in]   txState        : pointer to the transmitted N-PDU status
 * @return      void
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-73123
 */
CANTP_LOCAL void CanTp_InitTxChannel(CanTp_TxStatusType* txState);
/**
 * @brief         Further processing of the CanTp_RxIndication()
 * @param[in]     rxPduId       : received N-PDU Id
 * @param[in]     pduInfoPtr    : pointer to received N-PDU data
 * @param[in]     FramePCIType  : protocol frame type (SF, FF, CF or FC)
 * @reentrant     Reentrant.
 * @synchronous   TRUE
 * @trace       CPD-73124
 */
CANTP_LOCAL void CanTp_RxSubDeal(PduIdType rxPduId, const PduInfoType* pduInfoPtr, CanTp_FramePCIType FramePCIType);

/**
 * @brief         Handles reception of a single frame
 * @param[in]     pduInfoPtr      : pointer to received N-PDU data
 * @param[in]     rxNSduCfgPtr    : pointer to the received N-PDU configuration
 * @param[in]     globalInfo      : pointer to the received N-PDU runtime variable
 * @reentrant     Reentrant.
 * @synchronous   TRUE
 * @trace       CPD-73125
 */
CANTP_LOCAL void CanTp_RxSubDealWithSF(
    const PduInfoType*      pduInfoPtr,
    const CanTp_RxNSduType* rxNSduCfgPtr,
    CanTp_GPartInfoType*    globalInfo);

/**
 * @brief         Setup a new unsegmented connection and inform PduR
 * @param[in]     rxStatus        : pointer to  the received N-PDU status
 * @param[in]     rxNSduCfgPtr    : pointer to  the received N-PDU configuration
 * @param[in]     upperpduInfoPtr : pointer to upper I-PDU data
 * @param[in]     receivedLength  : received N-PDU data length
 * @reentrant     Reentrant.
 * @synchronous   TRUE
 * @trace       CPD-73126
 */
CANTP_LOCAL void CanTp_RxSubDealWithSFToUp(
    CanTp_RxStatusType*     rxStatus,
    const CanTp_RxNSduType* rxNSduCfgPtr,
    PduInfoType*            upperpduInfoPtr,
    PduLengthType           receivedLength);

/**
 * @brief         Handles reception of a first frame
 * @param[in]     pduInfoPtr      : pointer to received N-PDU data
 * @param[in]     rxNSduCfgPtr    : pointer to the received N-PDU configuration
 * @param[in]     globalInfo      : pointer to the received N-PDU runtime variable
 * @reentrant     Reentrant.
 * @synchronous   TRUE
 * @trace       CPD-73127
 */
CANTP_LOCAL void CanTp_RxSubDealWithFF(
    const PduInfoType*      pduInfoPtr,
    const CanTp_RxNSduType* rxNSduCfgPtr,
    CanTp_GPartInfoType*    globalInfo);

/**
 * @brief         Setup a new segmented connection and inform PduR
 * @param[in]     globalInfo      : pointer to the received N-PDU runtime variable
 * @param[in]     rxStatus        : pointer to  the received N-PDU status
 * @param[in]     upperpduInfoPtr : pointer to upper I-PDU data
 * @param[in]     receivedLength  : received N-PDU data length
 * @reentrant     Reentrant.
 * @synchronous   TRUE
 * @trace       CPD-73128
 */
CANTP_LOCAL void CanTp_RxSubDealWithFFTOUP(
    CanTp_GPartInfoType* globalInfo,
    CanTp_RxStatusType*  rxStatus,
    PduInfoType*         upperpduInfoPtr,
    PduLengthType        receivedLength);

/**
 * @brief         Handles reception of a consecutive frame
 * @param[in]     pduInfoPtr      : pointer to received N-PDU data
 * @param[in]     rxNSduCfgPtr    : pointer to the received N-PDU configuration
 * @param[in]     globalInfo      : pointer to the received N-PDU runtime variable
 * @reentrant     Reentrant.
 * @synchronous   TRUE
 * @trace       CPD-73129
 */
CANTP_LOCAL void CanTp_RxSubDealWithCF(
    const PduInfoType*      pduInfoPtr,
    const CanTp_RxNSduType* rxNSduCfgPtr,
    CanTp_GPartInfoType*    globalInfo);

/**
 * @brief         Check the received N-PDU data
 * @param[in]     rxStatus        : pointer to the received N-PDU status
 * @param[in]     rxNSduCfgPtr    : pointer to the received N-PDU configuration
 * @param[in]     pduInfoPtr      : pointer to received N-PDU data
 * @param[out]    isLastCF        : pointer to result whether the last CF
 * @reentrant     Reentrant.
 * @synchronous   TRUE
 * @trace       CPD-73130
 */
CANTP_LOCAL Std_ReturnType CanTp_RxSubDealWithCFCheck(
    CanTp_RxStatusType*     rxStatus,
    const CanTp_RxNSduType* rxNSduCfgPtr,
    const PduInfoType*      pduInfoPtr,
    boolean*                isLastCF);

/**
 * @brief         Handles reception of a flow control frame
 * @param[in]     pduInfoPtr      : pointer to received N-PDU data
 * @param[in]     txNSduCfgPtr    : pointer to the transmitted N-PDU configuration
 * @param[in]     globalInfo      : pointer to the received N-PDU runtime variable
 * @reentrant     Reentrant.
 * @synchronous   TRUE
 * @trace       CPD-73131
 */
CANTP_LOCAL void CanTp_RxSubDealWithFC(
    const PduInfoType*      pduInfoPtr,
    const CanTp_TxNSduType* txNSduCfgPtr,
    CanTp_GPartInfoType*    globalInfo);

/**
 * @brief         Timer process of pre Rx connections
 * @param[in]     rxStatus        : pointer to the received N-PDU status
 * @param[in]     rxNSduCfgPtr    : pointer to the received N-PDU configuration
 * @param[in]     globalInfo      : pointer to the received N-PDU runtime variable
 * @reentrant     No-Reentrant.
 * @synchronous   TRUE
 * @trace       CPD-73132
 */
CANTP_LOCAL void CanTp_HandleRxTimerProcess(
    CanTp_RxStatusType*     rxStatus,
    const CanTp_RxNSduType* rxNSduCfgPtr,
    CanTp_GPartInfoType*    globalInfo);

/**
 * @brief         The rxchannels handle for scheduling the CanTp.
 * @reentrant     No-Reentrant.
 * @synchronous   TRUE
 * @trace       CPD-73133
 */
CANTP_LOCAL void CanTp_HandleRxChannels(void);

/**
 * @brief         The txchannels handle for scheduling the CanTp.
 * @reentrant     No-Reentrant.
 * @synchronous   TRUE
 * @trace       CPD-73134
 */
CANTP_LOCAL void CanTp_HandleTxChannels(void);

/**
 * @brief         Timer process of pre Tx connections
 * @param[in]     txStatus        : pointer to the transmitted N-PDU status
 * @param[in]     txNSduCfgPtr    : pointer to the transmitted N-PDU configuration
 * @param[in]     globalInfo      : pointer to the transmitted N-PDU runtime variable
 * @reentrant     No-Reentrant.
 * @synchronous   TRUE
 * @trace       CPD-73135
 */
CANTP_LOCAL void CanTp_HandleTxTimeoutProcess(
    CanTp_TxStatusType*     txStatus,
    const CanTp_TxNSduType* txNSduCfgPtr,
    CanTp_GPartInfoType*    globalInfo);

/**
 * @brief         Handles confirmation of a transmitted CanTp frame(SF,FF,CF)
 * @param[in]     txDirection  : pointer to the transmitted N-PDU direction
 * @param[in]     result       : transmission result of the PDU
 * @param[in]     globalInfo   : pointer to the transmitted N-PDU runtime variable
 * @reentrant     No-Reentrant.
 * @synchronous   TRUE
 * @trace         CPD-73136
 */
CANTP_LOCAL void CanTp_TxConfirmationTx(
    CanTp_ConfirmDirectionType* txDirection,
    CanTp_GPartInfoType*        globalInfo,
    Std_ReturnType              result);

/**
 * @brief         Handles confirmation of a transmitted CanTp frame(FC)
 * @param[in]     rxStatus    : pointer to the received N-PDU status
 * @param[in]     result      : transmission result of the PDU
 * @reentrant     No-Reentrant.
 * @synchronous   TRUE
 * @trace       CPD-73137
 */
CANTP_LOCAL void CanTp_TxFCConfirmation(CanTp_RxStatusType* rxStatus, Std_ReturnType result);

/**
 * @brief         Check parameter(Bs,STmin) value
 * @param[in]     canTpParam  : parameter type
 * @param[in]     value       : parameter value
 * @reentrant     No-Reentrant.
 * @synchronous   TRUE
 * @trace       CPD-73138
 */
CANTP_LOCAL boolean CanTP_CheckRxChangeParameter(TPParameterType canTpParam, uint16 value);
/**
 * @brief       Get Tx buffer provided by the upper layer
 * @param[in]   txStatus           : pointer to the transmitted N-PDU status
 * @param[in]   txNSduCfgPtr       : pointer to the transmitted N-PDU configuration
 * @param[in]   globalInfo         : pointer to the transmitted N-PDU runtime variable
 * @return      BufReq_ReturnType
 * @retval      BUFREQ_OK          : Get Tx buffer successfully
 * @retval      BUFREQ_E_BUSY      : No Tx buffer available and later (implementation specific) retry to copy the data
 * @retval      BUFREQ_E_NOT_OK    : No Tx buffer available
 * @retval      BUFREQ_E_OVFL      : No Tx buffer available
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73139
 */
CANTP_LOCAL BufReq_ReturnType CanTp_GetTxBuffer(
    CanTp_TxStatusType*     txStatus,
    const CanTp_TxNSduType* txNSduCfgPtr,
    CanTp_GPartInfoType*    globalInfo);

/**
 * @brief       Get available Rx buffer size provided by the upper layer
 * @param[in]   rxStatus      : pointer to the received N-PDU status
 * @return      Std_ReturnType
 * @retval      E_OK          : get available Rx buffer successfully
 * @retval      E_NOT_OK      : get available Rx buffer failed
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73140
 */
CANTP_LOCAL Std_ReturnType CanTp_GetRxBuffer(const CanTp_RxStatusType* rxStatus);

/**
 * @brief       Send FC during the receiving process
 * @param[in]   rxStatus        : pointer to the received N-PDU status
 * @param[in]   rxNSduCfgPtr    : pointer to the received N-PDU configuration
 * @param[in]   globalInfo      : pointer to the received N-PDU runtime variable
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73141
 */
CANTP_LOCAL void
    CanTp_SendFC(const CanTp_RxNSduType* rxNSduCfgPtr, CanTp_RxStatusType* rxStatus, CanTp_GPartInfoType* globalInfo);

/**
 * @brief       Set up a new transmission session.
 * @param[in]   txNSduId      : Identifier of the PDU to be transmitted
 * @param[in]   txNSduCfgPtr  : pointer to the transmitted N-PDU configuration
 * @param[in]   pduInfoPtr    : pointer to the transmitted N-PDU data
 * @param[in]   globalInfo    : pointer to the transmitted N-PDU runtime variable
 * @return      Std_ReturnType
 * @retval      E_OK          : Transmit request has been accepted.
 * @retval      E_NOT_OK      : Transmit request has not been accepted
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73142
 */
CANTP_LOCAL Std_ReturnType CanTp_TxSetupConnection(
    PduIdType               txNSduId,
    const CanTp_TxNSduType* txNSduCfgPtr,
    const PduInfoType*      pduInfoPtr,
    CanTp_GPartInfoType*    globalInfo);

/**
 * @brief       Send SF,FF,CF Frame during the Transmitting process
 * @param[in]   txStatus           : pointer to the transmitted N-PDU status
 * @param[in]   txNSduCfgPtr       : pointer to the transmitted N-PDU configuration
 * @param[in]   txpduInfo          : pointer to Lower L-PDU data
 * @param[in]   txDirection        : pointer to the transmitted N-PDU direction
 * @return      void
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73143
 */
CANTP_LOCAL void CanTp_TxTransmit(
    CanTp_TxStatusType*         txStatus,
    const CanTp_TxNSduType*     txNSduCfgPtr,
    PduInfoType*                txpduInfo,
    CanTp_ConfirmDirectionType* txDirection
#if (CANTP_MULTIPLE_PARTITION_ENABLED == STD_ON)
    ,
    const CanTp_GPartInfoType* globalInfo
#endif
);

/**
 * @brief       This function get internal STmin timer value.
 * @param[in]   STminValue      : STmin value
 * @return      uint8           :internal STmin timer
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-79943
 */
CANTP_LOCAL uint8 CanTp_CalculateSTMin(uint8 STminValue);

/* ========================================= internal function definition ========================================= */
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"

/* PRQA S 4464,4461,2995,2991,2992 ++ */ /* VL_CanTp_4464,VL_CanTp_4461,VL_CanTp_2995,VL_CanTp_2991,VL_CanTp_2992 */
/**
 *@brief  This function get internal STmin timer value.
 */
CANTP_LOCAL uint8 CanTp_CalculateSTMin(uint8 STminValue)
{
    uint8 STminStamp = STminValue;

    if (STminStamp > CANTP_MAX_SEPARATE_TIME)
    {
        /* microsecond range validity check */
        const boolean isMicrosec = (STminStamp >= CANTP_STMIN_MICROSEC_MIN) && (STminStamp <= CANTP_STMIN_MICROSEC_MAX);

        STminStamp = isMicrosec ? CANTP_MAIN_FUNCTION_PERIOD : CANTP_MAX_SEPARATE_TIME;
    }
    STminStamp = CEIL_DIV(STminStamp, CANTP_MAIN_FUNCTION_PERIOD);

    return STminStamp;
}

/**
 *@brief  This function reset Rx channel status.
 */
CANTP_LOCAL void CanTp_InitRxChannel(CanTp_RxStatusType* rxState)
{
    SchM_Enter_CanTp_ExclusiveArea();
#if (STD_ON == CANTP_RX_QUEUE)
    if (rxState->RxNSduId != CANTP_INVALID_U16)
    {
#if (CANTP_ECUC_MULTIPLE_PARTITION_EXIST == STD_ON)
        uint8                   index;
        const CanTp_RxNSduType* rxNSduCfgPtr = CanTp_GetRxNSduCfg(rxState->RxNSduId);
        index                                = CanTp_ApplicationMap[rxNSduCfgPtr->ApplicationId];
        CanTp_GPartInfoType* globalInfo      = &CanTp_GlobalPartitionInfo[index];
#else
        CanTp_GPartInfoType* globalInfo = &CanTp_GlobalPartitionInfo[0];
#endif
        CanTp_ClearRxQueue(globalInfo->RxQueue, rxState->RxNSduId, &globalInfo->CurrentIndex);
    }
#endif
    rxState->RxNSduId       = CANTP_INVALID_U16;
    rxState->RxPduId        = CANTP_INVALID_U16;
    rxState->ChannelState   = CANTP_RX_IDLE;
    rxState->PCIOffset      = 0u;
    rxState->Timer          = 0u;
    rxState->DatalenTotal   = 0u;
    rxState->DatalenRemain  = 0u;
    rxState->InitalDLC      = 0u;
    rxState->HandledCfCount = 0u;
    rxState->ExpectdCfSn    = 0u;
    rxState->SentWftCount   = 0u;
    rxState->EventFlags     = 0u;
#if (CANTP_DYN_ID_SUPPORT == STD_ON)
    rxState->MetaDataLength = 0u;
    rxState->MetaDataNAe    = 0u;
    rxState->MetaDataNSa    = 0u;
    rxState->MetaDataNTa    = 0u;
#endif
    SchM_Exit_CanTp_ExclusiveArea();
}
/**
 * @brief  This function reset Tx channel status.
 */
CANTP_LOCAL void CanTp_InitTxChannel(CanTp_TxStatusType* txState)
{
    SchM_Enter_CanTp_ExclusiveArea();
    txState->TxNSduId        = CANTP_INVALID_U16;
    txState->TxPduId         = CANTP_INVALID_U16;
    txState->PCIOffset       = 0u;
    txState->FFDataMax       = 0u;
    txState->CFDataMax       = 0u;
    txState->ChannelState    = CANTP_TX_IDLE;
    txState->Timer           = 0u;
    txState->STminTimer      = 0u;
    txState->DatalenTotal    = 0u;
    txState->DatalenRemain   = 0u;
    txState->ExpectdCfSn     = 0u;
    txState->HandledCfCount  = 0u;
    txState->TxDLC           = 0u;
    txState->FirstFCReceived = FALSE;
#if (CANTP_DYN_ID_SUPPORT == STD_ON)
    txState->MetaDataLength = 0u;
    txState->MetaDataNAe    = 0u;
    txState->MetaDataNSa    = 0u;
    txState->MetaDataNTa    = 0u;
#endif
#if (CANTP_FD == STD_ON)
    CanTp_MemSet(txState->LocalBuf, 0u, CANTP_CANFD_FRAME_LEN_MAX);
#else
    CanTp_MemSet(txState->LocalBuf, 0u, CANTP_CAN20_FRAME_LEN_MAX);
#endif
    SchM_Exit_CanTp_ExclusiveArea();
}

/**
 * @brief Send FC during the receiving process
 */
CANTP_LOCAL void CanTp_SendFC(
    const CanTp_RxNSduType* rxNSduCfgPtr,
    CanTp_RxStatusType*     rxStatus,
    CanTp_GPartInfoType*    globalInfo) /* PRQA S 3673 */ /* VL_CanTp_3673 */
{
    PduInfoType pduInfo;
    uint8       sdudata[CANTP_CAN20_FRAME_LEN_MAX];
    pduInfo.SduDataPtr = sdudata;
#if (CANTP_DYN_ID_SUPPORT == STD_ON)
    uint8 metaData[CANTP_METADATA_MAXLENGTH] = {0};
    pduInfo.MetaDataPtr                      = metaData;
    CanTp_ConstructTxFcMetaDataInfo(rxStatus, rxNSduCfgPtr, &pduInfo);
#endif
    SchM_Enter_CanTp_ExclusiveArea();
    switch (rxStatus->ChannelState)
    {
    case CANTP_RX_TRANSMIT_FC_CFS:
        pduInfo.SduDataPtr[rxStatus->PCIOffset] = CANTP_FTYPE_FC | CANTP_FC_FS_CTS;
        break;
    case CANTP_RX_TRANSMIT_FC_OVFL:
        pduInfo.SduDataPtr[rxStatus->PCIOffset] = CANTP_FTYPE_FC | CANTP_FC_FS_OVFL;
        break;
    case CANTP_RX_TRANSMIT_FC_WT:
        pduInfo.SduDataPtr[rxStatus->PCIOffset] = CANTP_FTYPE_FC | CANTP_FC_FS_WT;
        break;
    default:
        /* idle */
        break;
    }
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    pduInfo.SduDataPtr[rxStatus->PCIOffset + 1u] = rxStatus->CurrentBs;
    pduInfo.SduDataPtr[rxStatus->PCIOffset + 2u] = CanTp_GetRxCurrentSTmin(
        rxStatus->RxNSduId
#if (CANTP_CHANGE_PARAMETER == STD_ON)
        ,
        globalInfo
#endif
    );
    /* PRQA S 3120 -- */
    /* PRQA S 4391 ++*/ /* VL_CanTp_DataConverteToWider */
    pduInfo.SduLength = (PduLengthType)(rxStatus->PCIOffset + CANTP_PCI_LENGTH_FC);
    /* PRQA S 4391 --*/
#if (CANTP_ENABLED_RX_BYTE_PADDING == STD_ON)
    if ((CANTP_PADDING_ON == rxNSduCfgPtr->RxPaddingActivation) && (pduInfo.SduLength < CANTP_CAN20_FRAME_LEN_MAX))
    {
        /** @see SWS_CanTp_00347 If CanTpRxPaddingActivation is equal to CANTP_ON for an Rx N-SDU, the CanTp module
         * shall transmit FC N-PDUs with a length of eight bytes.
         *  */
        /* PRQA S 4491 ++ */ /* VL_CanTp_4491 */
        CanTp_MemSet(
            &pduInfo.SduDataPtr[rxStatus->PCIOffset + CANTP_PCI_LENGTH_FC],
            CANTP_PADDING_BYTE,
            (CANTP_CAN20_FRAME_LEN_MAX - rxStatus->PCIOffset - CANTP_PCI_LENGTH_FC));
        /* PRQA S 4491 -- */
        pduInfo.SduLength = CANTP_CAN20_FRAME_LEN_MAX;
    }
#endif
#if (CANTP_MULTIPLE_PARTITION_ENABLED == STD_ON)
    CanTp_ConfirmDirectionType* txDirection =
        &globalInfo->CanTpTxDirection[rxNSduCfgPtr->TxFcNPduId - CANTP_GETTXNPDUSTART(globalInfo->index)];
#else
    CanTp_ConfirmDirectionType* txDirection = &globalInfo->CanTpTxDirection[rxNSduCfgPtr->TxFcNPduId];
#endif
    /* check whether Tx Buffer occupied */
    if (txDirection->ConnectionId == CANTP_INVALID_U16)
    {
        txDirection->TxDirection = CANTP_DIR_RX;
#if (CANTP_MULTIPLE_PARTITION_ENABLED == STD_ON)
        txDirection->ConnectionId = rxStatus->RxNSduId - CANTP_GETRXNSDUSTART(globalInfo->index);
#else
        txDirection->ConnectionId = rxStatus->RxNSduId;
#endif
        SchM_Exit_CanTp_ExclusiveArea();
        Std_ReturnType result = CanIf_Transmit(rxNSduCfgPtr->TxFcLPduId, &pduInfo);
        SchM_Enter_CanTp_ExclusiveArea();
        if (E_OK == result)
        {
            switch (rxStatus->ChannelState)
            {
            case CANTP_RX_TRANSMIT_FC_CFS:
                rxStatus->ChannelState = CANTP_RX_WAIT_FCCONF_CFS;
                break;
            case CANTP_RX_TRANSMIT_FC_OVFL:
                rxStatus->ChannelState = CANTP_RX_WAIT_FCCONF_OVFL;
#if (CANTP_E_FC_OVERFLOW_TRANSMITTED_EABLED == STD_ON)
                Dem_SetEventStatus(CANTP_E_FC_OVERFLOW_TRANSMITTED, DEM_EVENT_STATUS_FAILED);
#endif
                break;
            case CANTP_RX_TRANSMIT_FC_WT:
                rxStatus->ChannelState = CANTP_RX_WAIT_FCCONF_WT;
                break;
            default:
                /**idle*/
                break;
            }
            SchM_Exit_CanTp_ExclusiveArea();
        }
        else
        {
            txDirection->TxDirection  = CANTP_DIR_IDLE;
            txDirection->ConnectionId = CANTP_INVALID_U16;
            CanTp_InitRxChannel(rxStatus);
            SchM_Exit_CanTp_ExclusiveArea();
            PduR_CanTpRxIndication(rxNSduCfgPtr->RxIPduId, E_NOT_OK);
        }
    }
    else
    {
        SchM_Exit_CanTp_ExclusiveArea();
    }
}

/**
 * @brief  Get Tx buffer provided by the upper layer
 */
CANTP_LOCAL BufReq_ReturnType CanTp_GetTxBuffer(
    CanTp_TxStatusType*     txStatus,
    const CanTp_TxNSduType* txNSduCfgPtr,
    CanTp_GPartInfoType*    globalInfo) /* PRQA S 3673 */ /* VL_CanTp_3673 */
{
    PduInfoType       pduInfo;
    uint8*            pLocalBuf = txStatus->LocalBuf;
    PduLengthType     bufferSize;
    uint8             dataOffset;
    PduLengthType     len;
    boolean           furtherProcess = FALSE;
    BufReq_ReturnType bufRslt        = BUFREQ_OK;
#if (CANTP_DYN_ID_SUPPORT == STD_ON)
    uint8 metadata[CANTP_METADATA_MAXLENGTH] = {0};
    pduInfo.MetaDataPtr                      = metadata;
#endif
    SchM_Enter_CanTp_ExclusiveArea();
    switch (txStatus->ChannelState)
    {
    case CANTP_TX_TRANSMIT_SF:
        len                = txStatus->DatalenTotal + txStatus->PCIOffset + CANTP_PCI_LENGTH_SF;
        pduInfo.SduLength  = txStatus->DatalenTotal;
        dataOffset         = (len > CANTP_CAN20_FRAME_LEN_MAX) ? CANTP_PCI_LENGTH_SF_FD : CANTP_PCI_LENGTH_SF;
        pduInfo.SduDataPtr = &pLocalBuf[txStatus->PCIOffset + dataOffset];
        furtherProcess     = TRUE;
        break;
    case CANTP_TX_TRANSMIT_FF:
        pduInfo.SduLength = txStatus->FFDataMax;
        dataOffset =
            (txStatus->DatalenTotal <= CANTP_FF_DL_12BIT_LENGTH) ? CANTP_PCI_LENGTH_FF : CANTP_PCI_LENGTH_FF_12BIT;
        pduInfo.SduDataPtr = &pLocalBuf[txStatus->PCIOffset + dataOffset];
        furtherProcess     = TRUE;
        break;
    case CANTP_TX_TRANSMIT_CF:
        if (txStatus->DatalenRemain >= txStatus->CFDataMax)
        {
            pduInfo.SduLength = txStatus->CFDataMax;
        }
        else
        {
            pduInfo.SduLength = txStatus->DatalenRemain;
        }
        pduInfo.SduDataPtr = &pLocalBuf[txStatus->PCIOffset + CANTP_PCI_LENGTH_CF];
        furtherProcess     = TRUE;
        break;
    default:
        /* idle */
        break;
    }
    SchM_Exit_CanTp_ExclusiveArea();
    if (furtherProcess == TRUE)
    {
        bufRslt = PduR_CanTpCopyTxData(txNSduCfgPtr->TxIPduId, &pduInfo, NULL_PTR, &bufferSize);
#if (CANTP_MULTIPLE_PARTITION_ENABLED == STD_ON)
        CanTp_ConfirmDirectionType* txDirection =
            &globalInfo->CanTpTxDirection[txStatus->TxPduId - CANTP_GETTXNPDUSTART(globalInfo->index)];
#else
        CanTp_ConfirmDirectionType* txDirection = &globalInfo->CanTpTxDirection[txStatus->TxPduId];
#endif
        if ((bufRslt == BUFREQ_OK) && (txDirection->ConnectionId == CANTP_INVALID_U16))
        {
            CanTp_TxTransmit(
                txStatus,
                txNSduCfgPtr,
                &pduInfo,
                txDirection
#if (CANTP_MULTIPLE_PARTITION_ENABLED == STD_ON)
                ,
                globalInfo
#endif
            );
        }
        else if (bufRslt == BUFREQ_E_BUSY)
        {
            /* later retry to copy the data. */
        }
        else
        {
            CanTp_InitTxChannel(txStatus);
            /*Notify the upper*/
            PduR_CanTpTxConfirmation(txNSduCfgPtr->TxIPduId, E_NOT_OK);
        }
    }
    return bufRslt;
}
/**
 *   get rxbuffer during each processing of the MainFunction.
 */
CANTP_LOCAL Std_ReturnType CanTp_GetRxBuffer(const CanTp_RxStatusType* rxStatus)
{
    Std_ReturnType ret = E_NOT_OK;
    if (rxStatus->ChannelState == CANTP_RX_WAIT_BUFFER_CF)
    {
        PduLengthType bufferSize;
        PduInfoType   upperpduInfo;
        PduInfoType*  upperpduInfoPtr = &upperpduInfo;
        upperpduInfoPtr->SduLength    = 0u;
        upperpduInfoPtr->SduDataPtr   = NULL_PTR;
        /* SWS_CanTp_00222 */
        if (BUFREQ_OK == PduR_CanTpCopyRxData(rxStatus->RxPduId, upperpduInfoPtr, &bufferSize))
        {
            if (E_OK == CanTp_CompareRxBufferWithBs(rxStatus, bufferSize))
            {
                ret = E_OK;
            }
        }
    }
    return ret;
}

/**
 * @brief Send SF,FF,CF Frame during the Transmitting process
 */
/* PRQA S 6070 ++ */ /* VL_MTR_CanTp_STCAL */
CANTP_LOCAL void CanTp_TxTransmit(
    CanTp_TxStatusType*         txStatus,
    const CanTp_TxNSduType*     txNSduCfgPtr,
    PduInfoType*                txpduInfo,
    CanTp_ConfirmDirectionType* txDirection
#if (CANTP_MULTIPLE_PARTITION_ENABLED == STD_ON)
    ,
    const CanTp_GPartInfoType* globalInfo
#endif
)
{
    SchM_Enter_CanTp_ExclusiveArea();
    uint8 dataOffset = 0x00u;
    CanTp_TxConstructPCI(txStatus, &dataOffset);
    uint8 length = txStatus->PCIOffset + dataOffset;
    CanTp_MemCpy(&txStatus->LocalBuf[length], txpduInfo->SduDataPtr, txpduInfo->SduLength);
    uint8 offset = length + (uint8)(txpduInfo->SduLength);

    txDirection->TxDirection = CANTP_DIR_TX;
#if (CANTP_MULTIPLE_PARTITION_ENABLED == STD_ON)
    txDirection->ConnectionId = txStatus->TxNSduId - CANTP_GETTXNSDUSTART(globalInfo->index);
#else
    txDirection->ConnectionId = txStatus->TxNSduId;
#endif
    /* For DLC values from 9 to 15 only the mandatory padding should be used. */
    if (
#if (CANTP_ENABLED_TX_BYTE_PADDING == STD_ON)
        (CANTP_PADDING_ON == txNSduCfgPtr->TxPaddingActivation) ||
#endif
        (offset > CANTP_CAN20_FRAME_LEN_MAX))
    {
        CanTp_AssemblePaddingData(txStatus, txpduInfo, offset);
    }
    else
    {
        txpduInfo->SduLength = offset;
    }
    txpduInfo->SduDataPtr = txStatus->LocalBuf;
#if (CANTP_DYN_ID_SUPPORT == STD_ON)
    CanTp_ConstructTxMetaDataInfo(txStatus, txNSduCfgPtr, txpduInfo);
#endif
    SchM_Exit_CanTp_ExclusiveArea();
    Std_ReturnType rslt = CanIf_Transmit(txNSduCfgPtr->TxLPduId, txpduInfo);
    SchM_Enter_CanTp_ExclusiveArea();
    if (E_OK == rslt)
    {
        switch (txStatus->ChannelState)
        {
        case CANTP_TX_TRANSMIT_SF:
            txStatus->ChannelState = CANTP_TX_WAIT_SFCONF;
            break;
        case CANTP_TX_TRANSMIT_FF:
            txStatus->ChannelState = CANTP_TX_WAIT_FFCONF;
            break;
        case CANTP_TX_TRANSMIT_CF:
            txStatus->ChannelState = CANTP_TX_WAIT_CFCONF;
            break;
        default:
            /* idle */
            break;
        }
        /* As Timer */
        txStatus->Timer = txNSduCfgPtr->N_As;
        SchM_Exit_CanTp_ExclusiveArea();
    }
    else
    {
        txDirection->TxDirection  = CANTP_DIR_IDLE;
        txDirection->ConnectionId = CANTP_INVALID_U16;
        CanTp_InitTxChannel(txStatus);
        SchM_Exit_CanTp_ExclusiveArea();
        /*Notify the upper*/
        PduR_CanTpTxConfirmation(txNSduCfgPtr->TxIPduId, E_NOT_OK);
    }
}
/* PRQA S 6070 -- */

/**
 * @brief Set up a new transmission session.
 */
CANTP_LOCAL Std_ReturnType CanTp_TxSetupConnection(
    PduIdType               txNSduId,
    const CanTp_TxNSduType* txNSduCfgPtr,
    const PduInfoType*      pduInfoPtr,
    CanTp_GPartInfoType*    globalInfo) /* PRQA S 3673 */ /* VL_CanTp_3673 */
{
    Std_ReturnType ret = E_NOT_OK;
#if (CANTP_MULTIPLE_PARTITION_ENABLED == STD_ON)
    CanTp_TxStatusType* txStatus =
        &globalInfo->CanTpTxConnectionStatus[txNSduId - CANTP_GETTXNSDUSTART(globalInfo->index)];
#else
    CanTp_TxStatusType* txStatus = &globalInfo->CanTpTxConnectionStatus[txNSduId];
#endif

    /* SWS_CanTp_00123 */
    /* SWS_CanTp_00206 */ /* reject a request when transmission session is running */
    if ((pduInfoPtr->SduLength > 0u) && (txStatus->ChannelState == CANTP_TX_IDLE))
    {
        uint8 pciOffset  = CANTP_RXGETADROFFSET(txNSduCfgPtr->AddressingFormat);
        uint8 dataOffset = CANTP_PCI_LENGTH_SF;
#if (CANTP_TRANSMIT_CAN20_WITH_CANFD == STD_ON)
        /* PRQA S 3396 ++ */ /* VL_CanTp_3396 */
        txStatus->TxDLC = CanTp_ChannelCanWithCanFD[txNSduCfgPtr->ChannelId].IsSupport ? CANTP_CAN20_FRAME_LEN_MAX
                                                                                       : txNSduCfgPtr->TxNPduDLC;
        /* PRQA S 3396 -- */
#else
        txStatus->TxDLC = txNSduCfgPtr->TxNPduDLC;
#endif

        PduLengthType totalFrameSize = (uint16)pciOffset + (uint16)dataOffset + pduInfoPtr->SduLength;
        boolean       sendSF         = (totalFrameSize <= CANTP_CAN20_FRAME_LEN_MAX)
#if (CANTP_FD == STD_ON)
                         || ((txStatus->TxDLC > CANTP_CAN20_FRAME_LEN_MAX)
                             && ((pciOffset + CANTP_PCI_LENGTH_SF_FD + pduInfoPtr->SduLength) <= txStatus->TxDLC))
#endif
            ;
        /* Unified frame transmission handling */
        if (sendSF)
        {
            /* SWS_CanTp_00231 send a SF N-PDU */
            txStatus->TxNSduId      = txNSduId;
            txStatus->TxPduId       = txNSduCfgPtr->TxNPduId;
            txStatus->ChannelState  = CANTP_TX_TRANSMIT_SF;
            txStatus->PCIOffset     = pciOffset;
            txStatus->DatalenTotal  = pduInfoPtr->SduLength;
            txStatus->DatalenRemain = pduInfoPtr->SduLength;
            txStatus->Timer         = txNSduCfgPtr->N_Cs;
            ret                     = E_OK;
        }
        else
        { /* PRQA S 2985 ++ */ /* VL_CanTp_2985 */
            uint8 payloadMax = (uint8)(txStatus->TxDLC) - pciOffset;
            /* PRQA S 2985 -- */
            /* SWS_CanTp_00232 initiate a multiple frame transmission session */
            txStatus->ChannelState = CANTP_TX_TRANSMIT_FF;
            if (txNSduCfgPtr->TxTaType != CANTP_FUNCTIONAL)
            {
                txStatus->TxNSduId      = txNSduId;
                txStatus->TxPduId       = txNSduCfgPtr->TxNPduId;
                txStatus->ChannelState  = CANTP_TX_TRANSMIT_FF;
                txStatus->PCIOffset     = pciOffset;
                txStatus->DatalenTotal  = pduInfoPtr->SduLength;
                txStatus->DatalenRemain = pduInfoPtr->SduLength;
                uint8 ffOffset          = (pduInfoPtr->SduLength <= CANTP_FF_DL_12BIT_LENGTH) ? CANTP_PCI_LENGTH_FF
                                                                                              : CANTP_PCI_LENGTH_FF_12BIT;
                txStatus->FFDataMax     = payloadMax - ffOffset;
                txStatus->CFDataMax     = payloadMax - CANTP_PCI_LENGTH_CF;
                txStatus->Timer         = txNSduCfgPtr->N_Cs;
                ret                     = E_OK;
            }
            else
            {
                CanTp_InitTxChannel(txStatus);
#if (STD_ON == CANTP_RUNTIME_ERROR_DETECT)
                /*check TxTaType type, invalid*/
                CANTP_DET_REPORTRUNTIMEERROR(CANTP_SERVICEID_TRANSMIT, CANTP_E_INVALID_TATYPE);
#endif
            }
        }
        /* SWS_CanTp_00334 */
        if (ret == E_OK)
        {
#if (                                                                                               \
    (CANTP_DYN_ID_SUPPORT == STD_ON)                                                                \
    && ((CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON) || (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON) \
        || (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON) || (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON)))
            /* store the addressing information */
            CanTp_SaveTxMetaDataInfo(txStatus, txNSduCfgPtr, pduInfoPtr);
#endif
#if (CANTP_SYNC_TRANSMIT == STD_ON)
            /** Only when synchronous transmission is opened, immediately get a buffer to transmit SF / FF. */
            CanTp_GetTxBuffer(txStatus, txNSduCfgPtr, globalInfo);
#endif
        }
    }
    return ret;
}

/**
 * @brief Comfirm after FC Frame has been transmitted.
 */
CANTP_LOCAL void CanTp_TxFCConfirmation(CanTp_RxStatusType* rxStatus, Std_ReturnType result)
{
    const CanTp_RxNSduType* rxNSduCfgPtr = &CanTp_ConfigPtr->CanTpRxNSduCfg[rxStatus->RxNSduId];
    if (NULL_PTR == rxNSduCfgPtr)
    {
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
        /*SWS_CanTp_00359 No configured TX SDU matched with this TxSduId*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_TXCONFIRMATION, CANTP_E_INVALID_TX_ID);
#endif
    }
    else if (result == E_OK)
    {
        switch (rxStatus->ChannelState)
        {
            /* FC.CTS is confirmed */
        case CANTP_RX_WAIT_FCCONF_CFS:
            SchM_Enter_CanTp_ExclusiveArea();
            rxStatus->ChannelState = CANTP_RX_WAIT_RECEIVE_CF;
            rxStatus->Timer        = rxNSduCfgPtr->N_Cr;
            rxStatus->SentWftCount = 0u;
            SchM_Exit_CanTp_ExclusiveArea();
            break;
        case CANTP_RX_WAIT_FCCONF_OVFL:
            CanTp_InitRxChannel(rxStatus);
            break;
        case CANTP_RX_WAIT_FCCONF_WT:
            /* try to get buffer again */
            SchM_Enter_CanTp_ExclusiveArea();
            rxStatus->ChannelState = CANTP_RX_WAIT_BUFFER_CF;

            rxStatus->Timer = rxNSduCfgPtr->N_Br;
            rxStatus->SentWftCount++;
            SchM_Exit_CanTp_ExclusiveArea();
            break;
        default:
            /*idle*/
            break;
        }
    }
    else
    {
        CanTp_InitRxChannel(rxStatus);
        PduR_CanTpRxIndication(rxNSduCfgPtr->RxIPduId, E_NOT_OK);
    }
}
/* PRQA S 5017,6010 ++ */ /* VL_CanTp_5017,VL_MTR_CanTp_STCYC */
/**
 *  @brief The rxchannels timeout handle for scheduling the CAN TP.
 */
/* PRQA S 3415 ++ */ /* VL_CanTp_3415 */
CANTP_LOCAL void CanTp_HandleRxTimerProcess(
    CanTp_RxStatusType*     rxStatus,
    const CanTp_RxNSduType* rxNSduCfgPtr,
    CanTp_GPartInfoType*    globalInfo)
{
    boolean isFcSent = FALSE;
    SchM_Enter_CanTp_ExclusiveArea();
    rxStatus->Timer--;
    if (rxStatus->Timer == 0uL)
    {
        boolean checkPassed = FALSE;
        switch (rxStatus->ChannelState)
        {
        case CANTP_RX_WAIT_BUFFER_CF:
            if (E_OK != CanTp_GetRxBuffer(rxStatus))
            {
                if (rxStatus->SentWftCount < rxNSduCfgPtr->RxWftMax)
                {
                    // send wait FC
                    checkPassed            = TRUE;
                    rxStatus->ChannelState = CANTP_RX_TRANSMIT_FC_WT;
                    rxStatus->Timer        = rxNSduCfgPtr->N_Ar;
                    isFcSent               = TRUE;
                }

                else
                {
                    /* reach RxWftMax */
                    CanTp_InitRxChannel(rxStatus);
#if (CANTP_E_BR_TIMEOUT_EABLED == STD_ON)
                    Dem_SetEventStatus(CANTP_E_CANTPNBR_TIMEOUT_OCCURRED, DEM_EVENT_STATUS_FAILED);
#endif
                }
            }
            else
            {
                /* SWS_CanTp_00224 */
                checkPassed            = TRUE;
                rxStatus->ChannelState = CANTP_RX_TRANSMIT_FC_CFS;
                rxStatus->Timer        = rxNSduCfgPtr->N_Ar;
                isFcSent               = TRUE;
            }
            break;
        case CANTP_RX_TRANSMIT_FC_OVFL:
        case CANTP_RX_TRANSMIT_FC_WT:
        case CANTP_RX_TRANSMIT_FC_CFS:
            CanTp_InitRxChannel(rxStatus);
            /* Br Timeout */
#if (CANTP_E_BR_TIMEOUT_EABLED == STD_ON)
            Dem_SetEventStatus(CANTP_E_CANTPNBR_TIMEOUT_OCCURRED, DEM_EVENT_STATUS_FAILED);
#endif
            break;
        case CANTP_RX_WAIT_FCCONF_CFS:
        case CANTP_RX_WAIT_FCCONF_OVFL:
        case CANTP_RX_WAIT_FCCONF_WT:
            CanTp_InitRxChannel(rxStatus);
#if (CANTP_E_AR_TIMEOUT_EABLED == STD_ON)
            Dem_SetEventStatus(CANTP_E_CANTPNAR_TIMEOUT_OCCURRED, DEM_EVENT_STATUS_FAILED);
#endif
            break;
        case CANTP_RX_WAIT_RECEIVE_CF:
            /* Cr Timeout */
            CanTp_InitRxChannel(rxStatus);
#if (CANTP_E_CR_TIMEOUT_EABLED == STD_ON)
            Dem_SetEventStatus(CANTP_E_CANTPNCR_TIMEOUT_OCCURRED, DEM_EVENT_STATUS_FAILED);
#endif
            break;
        default:
            /* idle */
            break;
        }
        SchM_Exit_CanTp_ExclusiveArea();
        if (checkPassed == FALSE)
        {
            PduR_CanTpRxIndication(rxNSduCfgPtr->RxIPduId, E_NOT_OK);
#if (STD_ON == CANTP_RUNTIME_ERROR_DETECT)
            CANTP_DET_REPORTRUNTIMEERROR(CANTP_SERVICEID_MAINFUNCTION, CANTP_E_RX_COM);
#endif
        }
        if (isFcSent == TRUE)
        {
            CanTp_SendFC(rxNSduCfgPtr, rxStatus, globalInfo);
        }
    }
    else
    {
        if ((rxStatus->ChannelState == CANTP_RX_WAIT_BUFFER_CF) && (E_OK == CanTp_GetRxBuffer(rxStatus)))
        {
            /* SWS_CanTp_00224 */
            rxStatus->ChannelState = CANTP_RX_TRANSMIT_FC_CFS;
            rxStatus->Timer        = rxNSduCfgPtr->N_Ar;
            isFcSent               = TRUE;
            SchM_Exit_CanTp_ExclusiveArea();
        }
        else
        {
            SchM_Exit_CanTp_ExclusiveArea();
        }
        if (isFcSent == TRUE)
        {
            CanTp_SendFC(rxNSduCfgPtr, rxStatus, globalInfo);
        }
    }
}
/* PRQA S 3415 -- */
/* PRQA S 5017,6010 -- */

/**
 *  @brief The rxchannels handle for scheduling the CAN TP.
 */
/* PRQA S 6030 ++ */ /* VL_MTR_CanTp_STMIF */
CANTP_LOCAL void CanTp_HandleRxChannels(void)
{
    CanTp_RxStatusType*     rxStatus;
    const CanTp_RxNSduType* rxNSduCfgPtr;
#if (CANTP_ECUC_MULTIPLE_PARTITION_EXIST == STD_ON)
    ApplicationType      curAppId   = GetApplicationID();
    CanTp_GPartInfoType* globalInfo = CanTp_GetPartitionInfoPtr(curAppId);
#else
    CanTp_GPartInfoType* globalInfo = CanTp_GlobalPartitionInfo;
#endif

    if ((globalInfo != NULL_PTR) && (globalInfo->CanTpRxConnectionStatus != NULL_PTR))
    {
#if (CANTP_RX_QUEUE == STD_ON)
        for (uint8 iloop = 0; iloop < globalInfo->CurrentIndex; iloop++)
#else
#if (CANTP_MULTIPLE_PARTITION_ENABLED == STD_ON)
        uint8 num = CANTP_GETRXNSDUEND(globalInfo->index) - CANTP_GETRXNSDUSTART(globalInfo->index);
        for (uint8 index = 0u; index <= num; index++)
#else
        for (uint8 index = 0u; index < CanTp_ConfigPtr->CanTpRxNSduNum; index++)
#endif
#endif
        {
#if (CANTP_RX_QUEUE == STD_ON)
            uint8 index = globalInfo->RxQueue[iloop];
#endif
            rxStatus = &globalInfo->CanTpRxConnectionStatus[index];
            if (rxStatus->ChannelState != CANTP_RX_IDLE)
            {
                rxNSduCfgPtr = &CanTp_ConfigPtr->CanTpRxNSduCfg[rxStatus->RxNSduId];
#if (STD_OFF == CANTP_SYNCHRONOUS_RXINDICATION)
                if (CANTP_FLAGISSET(rxStatus->EventFlags, CANTP_EVENT_RXNOTIFIUPPER))
                {
#if (CANTP_TRANSMIT_CAN20_WITH_CANFD == STD_ON)
                    if CANTP_FLAGISSET (rxStatus->EventFlags, CANTP_EVENT_CAN20_WITH_CANFD)
                    {
                        CanTp_ChannelCanWithCanFD[rxNSduCfgPtr->ChannelId].IsSupport = TRUE;
                    }
                    else
                    {
                        CanTp_ChannelCanWithCanFD[rxNSduCfgPtr->ChannelId].IsSupport = FALSE;
                    }
#endif
                    PduR_CanTpRxIndication(rxNSduCfgPtr->RxIPduId, E_OK);
                    CanTp_InitRxChannel(rxStatus);
                }
                else
#endif
                {
                    CanTp_HandleRxTimerProcess(rxStatus, rxNSduCfgPtr, globalInfo);
                }
            }
        }
    }
}
/* PRQA S 6030 -- */

/**
 * @brief  The txchannels timeout(As,Bs,Cs) handle for scheduling the CAN TP.
 */
CANTP_LOCAL void CanTp_HandleTxTimeoutProcess(
    CanTp_TxStatusType*     txStatus,
    const CanTp_TxNSduType* txNSduCfgPtr,
    CanTp_GPartInfoType*    globalInfo)
{
    CanTp_ConfirmDirectionType* txDirection = &globalInfo->CanTpTxDirection[txStatus->TxPduId];
    boolean                     timeoutFlag = FALSE;
    switch (txStatus->ChannelState)
    {
    case CANTP_TX_WAIT_CF_STMIN:
        /* don't trigger the end of STmin when Cs timer expired (=0)! Stop waiting for STmin */
        txStatus->ChannelState = CANTP_TX_TRANSMIT_CF;
    case CANTP_TX_TRANSMIT_SF: /* PRQA S 2003 */ /* VL_CanTp_2003 */
    case CANTP_TX_TRANSMIT_FF:
    case CANTP_TX_TRANSMIT_CF:
        /* Cs Timeout,the last attempt to get a Tx buffer */
        if (BUFREQ_E_BUSY == CanTp_GetTxBuffer(txStatus, txNSduCfgPtr, globalInfo))
        {
            timeoutFlag = TRUE;
#if (CANTP_E_CS_TIMEOUT_EABLED == STD_ON)
            Dem_SetEventStatus(CANTP_E_CANTPNCS_TIMEOUT_OCCURRED, DEM_EVENT_STATUS_FAILED);
#endif
        }
        break;
    case CANTP_TX_WAIT_SFCONF:
    case CANTP_TX_WAIT_FFCONF:
    case CANTP_TX_WAIT_CFCONF:
        /* As Timeout */
        timeoutFlag = TRUE;
#if (CANTP_E_AS_TIMEOUT_EABLED == STD_ON)
        Dem_SetEventStatus(CANTP_E_CANTPNAS_TIMEOUT_OCCURRED, DEM_EVENT_STATUS_FAILED);
#endif
        break;
    case CANTP_TX_WAIT_RECEIVE_FC: /* PRQA S 2003 */ /* VL_CanTp_2003 */
        /* Bs Timeout */
        timeoutFlag = TRUE;
#if (CANTP_E_BS_TIMEOUT_EABLED == STD_ON)
        Dem_SetEventStatus(CANTP_E_CANTPNBS_TIMEOUT_OCCURRED, DEM_EVENT_STATUS_FAILED);
#endif
        break;
    default:
        /* idle */
        break;
    }
    if (timeoutFlag == TRUE)
    {
        SchM_Enter_CanTp_ExclusiveArea();
        txDirection->TxDirection  = CANTP_DIR_IDLE;
        txDirection->ConnectionId = CANTP_INVALID_U16;
        CanTp_InitTxChannel(txStatus);
        /*Notify the upper*/
        SchM_Exit_CanTp_ExclusiveArea();
        PduR_CanTpTxConfirmation(txNSduCfgPtr->TxIPduId, E_NOT_OK);
#if (STD_ON == CANTP_RUNTIME_ERROR_DETECT)
        CANTP_DET_REPORTRUNTIMEERROR(CANTP_SERVICEID_MAINFUNCTION, CANTP_E_TX_COM);
#endif
    }
}

/**
 * @brief  The txchannels handle for scheduling the CAN TP.
 */
/* PRQA S 6030 ++ */ /* VL_MTR_CanTp_STMIF */
CANTP_LOCAL void CanTp_HandleTxChannels(void)
{
    CanTp_TxStatusType*     txStatus;
    const CanTp_TxNSduType* txNSduCfgPtr;
#if (CANTP_ECUC_MULTIPLE_PARTITION_EXIST == STD_ON)
    ApplicationType      curAppId   = GetApplicationID();
    CanTp_GPartInfoType* globalInfo = CanTp_GetPartitionInfoPtr(curAppId);
#else
    CanTp_GPartInfoType* globalInfo = CanTp_GlobalPartitionInfo;
#endif

    if ((globalInfo != NULL_PTR) && (globalInfo->CanTpTxConnectionStatus != NULL_PTR))
    {
        txStatus = globalInfo->CanTpTxConnectionStatus;
#if (CANTP_MULTIPLE_PARTITION_ENABLED == STD_ON)
        for (uint8 index = CANTP_GETTXNSDUSTART(globalInfo->index); index <= CANTP_GETTXNSDUEND(globalInfo->index);
             index++)
#else
        for (uint8 index = 0u; index < CanTp_ConfigPtr->CanTpTxNSduNum; index++)
#endif
        {
            txNSduCfgPtr = &CanTp_ConfigPtr->CanTpTxNSduCfg[txStatus->TxNSduId];
            if (txStatus->ChannelState != CANTP_TX_IDLE)
            {
                if (txStatus->STminTimer > 0u)
                {
                    txStatus->STminTimer--;
                    if ((txStatus->STminTimer == 0u) && (CANTP_TX_WAIT_CF_STMIN == txStatus->ChannelState))
                    {
                        /* STmin expired, trigger transmission of CF */
                        txStatus->ChannelState = CANTP_TX_TRANSMIT_CF;
                    }
                }
                txStatus->Timer--;
                if (txStatus->Timer == 0uL)
                {
                    /* As,Bs,Cs timeoutprocess */
                    CanTp_HandleTxTimeoutProcess(txStatus, txNSduCfgPtr, globalInfo);
                }
                else
                {
                    /* Transmit SF,FF,CF */
                    (void)CanTp_GetTxBuffer(txStatus, txNSduCfgPtr, globalInfo);
                }
            }
            txStatus++;
        }
    }
}

/**
 * @brief  process received FC and switch TxConnection status
 */
CANTP_LOCAL void CanTp_RxSubDealWithFC(
    const PduInfoType*      pduInfoPtr,
    const CanTp_TxNSduType* txNSduCfgPtr,
    CanTp_GPartInfoType*    globalInfo) /* PRQA S 3673 */ /* VL_CanTp_3673 */
{
    uint8 pciOffset = CANTP_RXGETADROFFSET(txNSduCfgPtr->AddressingFormat);
#if (CANTP_MULTIPLE_PARTITION_ENABLED == STD_ON)
    CanTp_TxStatusType* txStatus =
        &globalInfo->CanTpTxConnectionStatus[txNSduCfgPtr->TxNSduId - CANTP_GETTXNSDUSTART(globalInfo->index)];
#else
    CanTp_TxStatusType* txStatus = &globalInfo->CanTpTxConnectionStatus[txNSduCfgPtr->TxNSduId];
#endif
    if (txStatus->ChannelState == CANTP_TX_WAIT_RECEIVE_FC)
    {
        if (pduInfoPtr->SduLength < (uint8)(CANTP_PCI_LENGTH_FC + pciOffset))
        {
            PduR_CanTpTxConfirmation(txNSduCfgPtr->TxIPduId, E_NOT_OK);
        }
#if ((STD_ON == CANTP_RUNTIME_ERROR_DETECT) && (CANTP_ENABLED_TX_BYTE_PADDING == STD_ON))
        else if (
            (txNSduCfgPtr->TxPaddingActivation == CANTP_PADDING_ON)
            && (pduInfoPtr->SduLength < CANTP_CAN20_FRAME_LEN_MAX))
        {
            PduR_CanTpTxConfirmation(txNSduCfgPtr->TxIPduId, E_NOT_OK);
            CANTP_DET_REPORTRUNTIMEERROR(CANTP_SERVICEID_RXINDICATION, CANTP_E_PADDING);
        }
#endif
#if (                                                                                               \
    (CANTP_DYN_ID_SUPPORT == STD_ON)                                                                \
    && ((CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON) || (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON) \
        || (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON) || (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON)))
        else if (E_OK != CanTp_CheckRxFCMetaData(txStatus, txNSduCfgPtr, pduInfoPtr))
        {
            PduR_CanTpTxConfirmation(txNSduCfgPtr->TxIPduId, E_NOT_OK);
        }
#endif
        else
        {
            CanTp_ConfirmDirectionType* txDirection = &globalInfo->CanTpTxDirection[txStatus->TxPduId];
            switch ((uint8)(pduInfoPtr->SduDataPtr[pciOffset] & CANTP_FC_FS_MASK))
            {
            case CANTP_FC_FS_CTS:
                if (txStatus->FirstFCReceived == FALSE)
                {
                    /* first Fc */
                    txStatus->FirstFCReceived = TRUE;
                    /* PRQA S 3120 ++*/ /* VL_QAC_MagicNum */
                    /** When BS is set to zero by the receiver, the sender is not waiting for an authorization to
                     * continue the transmission. */
                    txStatus->ExpectdCfSn  = 0x01u;
                    uint8 blocksizeCounter = pduInfoPtr->SduDataPtr[pciOffset + 1u];
                    txStatus->FcBs         = (blocksizeCounter > 0u) ? blocksizeCounter : CANTP_INVALID_U8;
                    txStatus->FcSTmin      = CanTp_CalculateSTMin(pduInfoPtr->SduDataPtr[pciOffset + 2u]);
                }
                txStatus->ChannelState = CANTP_TX_TRANSMIT_CF;
                txStatus->Timer        = txNSduCfgPtr->N_Cs;
                break;
            case CANTP_FC_FS_WT:
                /* reset Bs */
                txStatus->Timer = txNSduCfgPtr->N_Bs;
                break;
            case CANTP_FC_FS_OVFL:
#if (CANTP_E_FC_OVERFLOW_RECEIVED_EABLED == STD_ON)
                Dem_SetEventStatus(CANTP_E_FC_OVERFLOW_RECEIVED, DEM_EVENT_STATUS_FAILED);
#endif
            default:
                txDirection->TxDirection  = CANTP_DIR_IDLE;
                txDirection->ConnectionId = CANTP_INVALID_U16;
                CanTp_InitTxChannel(txStatus);
                /*Notify the upper transmit failed*/
                PduR_CanTpTxConfirmation(txNSduCfgPtr->TxIPduId, E_NOT_OK);
                break;
            }
        }
    }
}
/* PRQA S 6030 -- */

/**
 * @brief  check received CF Length/SN/Metadata
 */
CANTP_LOCAL Std_ReturnType CanTp_RxSubDealWithCFCheck(
    CanTp_RxStatusType*     rxStatus,
    const CanTp_RxNSduType* rxNSduCfgPtr,
    const PduInfoType*      pduInfoPtr,
    boolean*                isLastCF)
{
    Std_ReturnType ret = E_NOT_OK;
    /* check CFlength */
    uint8 dataOffSet       = CANTP_RXGETADROFFSET(rxNSduCfgPtr->AddressingFormat) + CANTP_PCI_LENGTH_CF;
    rxStatus->ChannelState = CANTP_RX_RECEIVE_CF;
    if ((rxStatus->InitalDLC < (rxStatus->DatalenRemain + dataOffSet))
        && (pduInfoPtr->SduLength == rxStatus->InitalDLC))
    {
        ret = E_OK;
    }
    else
    {
        PduLengthType len = rxStatus->DatalenRemain + dataOffSet;
#if (CANTP_FD == STD_ON)
        if ((rxNSduCfgPtr->RxNPduDLC > CANTP_CAN20_FRAME_LEN_MAX) && (len > CANTP_CAN20_FRAME_LEN_MAX))
        {
            uint8         paddingLength   = 0u;
            PduLengthType assembledLength = (rxStatus->DatalenRemain + dataOffSet);
            if (assembledLength <= 24u)
            {
                /** The 0xFC mask is used to mask out the lowest 2 bits, which means the value of CurrentLength will
                 * be rounded down to the nearest multiple of 4. */
                paddingLength = (uint8)(((uint8)(assembledLength - 1u) & 0xFCu) + 0x04u);
            }
            else
            {
                /** The 0xF0 mask is used to mask out the lowest 4 bits, which means the value of CurrentLength will
                 * be rounded down to the nearest multiple of 16. */
                paddingLength = (uint8)(((uint8)(assembledLength - 1u) & 0xF0u) + 0x10u);
            }
            /** CAN-FD with more than 8 byte */
            len = paddingLength;
        }
#endif
        // last cf length check
        if (((pduInfoPtr->SduLength == len)
#if (CANTP_ENABLED_RX_BYTE_PADDING == STD_ON)
             && (rxNSduCfgPtr->RxPaddingActivation == CANTP_PADDING_OFF)
#endif
                 )
#if (CANTP_ENABLED_RX_BYTE_PADDING == STD_ON)
            || ((((len <= CANTP_CAN20_FRAME_LEN_MAX) && (pduInfoPtr->SduLength == CANTP_CAN20_FRAME_LEN_MAX))
                 || ((len > CANTP_CAN20_FRAME_LEN_MAX) && (pduInfoPtr->SduLength == len)))
                && (rxNSduCfgPtr->RxPaddingActivation == CANTP_PADDING_ON))
#endif
        )
        {
            *isLastCF = TRUE;
            ret       = E_OK;
        }
        else
        {
#if ((STD_ON == CANTP_RUNTIME_ERROR_DETECT) && (CANTP_ENABLED_RX_BYTE_PADDING == STD_ON))
            if (rxNSduCfgPtr->RxPaddingActivation == CANTP_PADDING_ON)
            {
                CANTP_DET_REPORTRUNTIMEERROR(CANTP_SERVICEID_RXINDICATION, CANTP_E_PADDING);
            }
#endif
        }
    }
    if (ret == E_OK)
    {
        /* check SN  and metadata */
        uint8 cfSN = pduInfoPtr->SduDataPtr[rxStatus->PCIOffset] & CANTP_CF_SN_MASK;
        if (cfSN > rxStatus->ExpectdCfSn)
        {
            ret = E_NOT_OK;
#if (CANTP_E_DROPPED_CF_DETECTED_EABLED == STD_ON)
            Dem_SetEventStatus(CANTP_E_DROPPED_CONSECUTIVE_FRAMES_DETECTED, DEM_EVENT_STATUS_FAILED);
#endif
        }
        else if (cfSN < rxStatus->ExpectdCfSn)
        {
            ret = E_NOT_OK;
#if (CANTP_E_SWAPPED_CF_RECEIVED_EABLED == STD_ON)
            Dem_SetEventStatus(CANTP_E_SWAPPED_CONSECUTIVE_FRAMES_RECEIVED, DEM_EVENT_STATUS_FAILED);
#endif
        }
        else
        {
#if (                                                                                               \
    (CANTP_DYN_ID_SUPPORT == STD_ON)                                                                \
    && ((CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON) || (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON) \
        || (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON) || (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON)))
            ret = CanTp_CheckRxCFMetaData(rxStatus, rxNSduCfgPtr, pduInfoPtr);
#endif
        }
    }
    return ret;
}
/**
 * @brief  process received CF and notify upper to copy
 */
/* PRQA S 6030,6070 ++ */ /* VL_MTR_CanTp_STMIF,VL_MTR_CanTp_STCAL */
CANTP_LOCAL void CanTp_RxSubDealWithCF(
    const PduInfoType*      pduInfoPtr,
    const CanTp_RxNSduType* rxNSduCfgPtr,
    CanTp_GPartInfoType*    globalInfo)
{
#if (CANTP_MULTIPLE_PARTITION_ENABLED == STD_ON)
    CanTp_RxStatusType* rxStatus =
        &globalInfo->CanTpRxConnectionStatus[rxNSduCfgPtr->RxNSduId - CANTP_GETRXNSDUSTART(globalInfo->index)];
#else
    CanTp_RxStatusType* rxStatus = &globalInfo->CanTpRxConnectionStatus[rxNSduCfgPtr->RxNSduId];
#endif
    if (CANTP_FUNCTIONAL == rxNSduCfgPtr->RxTaType)
    {
#if (STD_ON == CANTP_RUNTIME_ERROR_DETECT)
        /*invalid TAType with CF frame, wrong configuration*/ /* SWS_CanTp_00093 */
        CANTP_DET_REPORTRUNTIMEERROR(CANTP_SERVICEID_RXINDICATION, CANTP_E_INVALID_TATYPE);
#endif
    }
    else if ((CANTP_RX_WAIT_RECEIVE_CF == rxStatus->ChannelState) && (rxNSduCfgPtr->RxNSduId == rxStatus->RxNSduId))
    {
        boolean isLastCF = FALSE;
        if (E_OK == CanTp_RxSubDealWithCFCheck(rxStatus, rxNSduCfgPtr, pduInfoPtr, &isLastCF))
        {
            PduInfoType        upperpduInfo;
            const PduInfoType* upperpduInfoPtr = &upperpduInfo;
            uint8              dataOffSet = CANTP_RXGETADROFFSET(rxNSduCfgPtr->AddressingFormat) + CANTP_PCI_LENGTH_CF;
            PduLengthType      dataLength = rxStatus->InitalDLC - dataOffSet;
            PduLengthType      bufferSize;
            if (isLastCF == TRUE)
            {
                dataLength = rxStatus->DatalenRemain;
            }
            upperpduInfo.SduDataPtr   = &(pduInfoPtr->SduDataPtr[dataOffSet]);
            upperpduInfo.SduLength    = dataLength;
            BufReq_ReturnType bufRslt = PduR_CanTpCopyRxData(rxNSduCfgPtr->RxIPduId, upperpduInfoPtr, &bufferSize);
            if (bufRslt == BUFREQ_OK)
            {
                rxStatus->DatalenRemain -= dataLength;
                /* when the SN reaches the value of 15, it shall wraparound and be set to zero for the next CF. */
                rxStatus->ExpectdCfSn =
                    (rxStatus->ExpectdCfSn < CANTP_MAX_SEQUENC_NUMBER) ? (rxStatus->ExpectdCfSn + 1u) : 0u;
                rxStatus->HandledCfCount += 1u;
                if (isLastCF == TRUE)
                {
#if (STD_ON == CANTP_SYNCHRONOUS_RXINDICATION)
#if (CANTP_TRANSMIT_CAN20_WITH_CANFD == STD_ON)
                    if CANTP_FLAGISSET (rxStatus->EventFlags, CANTP_EVENT_CAN20_WITH_CANFD)
                    {
                        CanTp_ChannelCanWithCanFD[rxNSduCfgPtr->ChannelId].IsSupport = TRUE;
                    }
                    else
                    {
                        CanTp_ChannelCanWithCanFD[rxNSduCfgPtr->ChannelId].IsSupport = FALSE;
                    }
#endif
                    PduR_CanTpRxIndication(rxNSduCfgPtr->RxIPduId, E_OK);
                    CanTp_InitRxChannel(rxStatus);
#else
                    /* PRQA S 3473 ++*/ /* VL_CanTp_3473 */
                    CANTP_SETFLAG(rxStatus->EventFlags, CANTP_EVENT_RXNOTIFIUPPER);
                    /* PRQA S 3473 --*/
#endif
                }
                else
                {
                    if ((rxStatus->HandledCfCount >= rxStatus->CurrentBs) && (rxStatus->CurrentBs != 0u))
                    {
                        /* SN value shall not be affected by any FlowControl (FC) frame */
                        rxStatus->HandledCfCount = 0u;
                        if (E_OK == CanTp_CompareRxBufferWithBs(rxStatus, bufferSize))
                        {
                            /* SWS_CanTp_00224 */
                            rxStatus->ChannelState = CANTP_RX_TRANSMIT_FC_CFS;
                            rxStatus->Timer        = rxNSduCfgPtr->N_Ar;
                            CanTp_SendFC(rxNSduCfgPtr, rxStatus, globalInfo);
                        }
                        else
                        {
                            /*the upper buffer is not enough to store the next block,so need to get more buffer*/
                            rxStatus->ChannelState = CANTP_RX_WAIT_BUFFER_CF;
                            rxStatus->Timer        = rxNSduCfgPtr->N_Br; // rxmainfunction handle
                        }
                    }
                    else
                    {
                        /* wait next Fc */
                        rxStatus->ChannelState = CANTP_RX_WAIT_RECEIVE_CF;
                        rxStatus->Timer        = rxNSduCfgPtr->N_Cr; // reset Cr time
                    }
                }
            }
            else
            {
                PduR_CanTpRxIndication(rxNSduCfgPtr->RxIPduId, E_NOT_OK);
                CanTp_InitRxChannel(rxStatus);
            }
        }
        else
        {
            PduR_CanTpRxIndication(rxNSduCfgPtr->RxIPduId, E_NOT_OK);
            CanTp_InitRxChannel(rxStatus);
        }
    }
    else
    {
        /* idle */
    }
}

#if (CANTP_CHANGE_PARAMETER == STD_ON)
/**
 * @brief Check Rx ChangeParameter range.
 */
CANTP_LOCAL boolean CanTP_CheckRxChangeParameter(TPParameterType canTpParam, uint16 value)
{
    boolean paramValid = FALSE;

    if ((TP_BS == canTpParam) || (TP_STMIN == canTpParam))
    {
        if ((TP_BS == canTpParam) && (value <= CANTP_INVALID_U8))
        {
            paramValid = TRUE;
        }
        else if (
            (TP_STMIN == canTpParam)
            && ((value <= CANTP_MAX_SEPARATE_TIME)
                || ((value >= CANTP_STMIN_MICROSEC_MIN) && (value <= CANTP_STMIN_MICROSEC_MAX))))
        {
            paramValid = TRUE;
        }
        else
        {
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
            /*CanTp module not started*/
            CANTP_DET_REPORTERROR(CANTP_SERVICEID_CHANGEPARAMETER, CANTP_E_PARAM_CONFIG);
#endif
        }
    }
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
    else
    {
        /*CanTp module not started*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_CHANGEPARAMETER, CANTP_E_PARAM_ID);
    }
#endif
    return paramValid;
}
/* PRQA S 6030,6070 -- */

#endif
/**
 *  @brief process received FF and notify upper
 */
/* PRQA S 6070 ++ */ /* VL_MTR_CanTp_STCAL */
CANTP_LOCAL void CanTp_RxSubDealWithFFTOUP(
    CanTp_GPartInfoType* globalInfo,
    CanTp_RxStatusType*  rxStatus,
    PduInfoType*         upperpduInfoPtr,
    PduLengthType        receivedLength)
{
    const CanTp_RxNSduType* rxNSduCfgPtr = CanTp_GetRxNSduCfg(rxStatus->RxNSduId);
    PduLengthType           bufferSize;
    /* SWS_CanTp_00079 */
    BufReq_ReturnType bufRslt =
        PduR_CanTpStartOfReception(rxNSduCfgPtr->RxIPduId, upperpduInfoPtr, rxStatus->DatalenTotal, &bufferSize);
    switch (bufRslt)
    {
    case BUFREQ_OK:
        /* SWS_CanTp_00080 */
#if (STD_ON == CANTP_RX_QUEUE)
        CanTp_AddRxQueue(globalInfo->RxQueue, rxNSduCfgPtr->RxNSduId, &globalInfo->CurrentIndex);
#endif
        if (bufferSize < receivedLength)
        {
            /* SWS_CanTp_00339 */
            PduR_CanTpRxIndication(rxNSduCfgPtr->RxIPduId, E_NOT_OK);
            CanTp_InitRxChannel(rxStatus);
        }
        else
        {
            upperpduInfoPtr->SduLength    = receivedLength;
            BufReq_ReturnType copybufRslt = PduR_CanTpCopyRxData(rxNSduCfgPtr->RxIPduId, upperpduInfoPtr, &bufferSize);
            switch (copybufRslt)
            {
            case BUFREQ_OK:
                rxStatus->DatalenRemain = rxStatus->DatalenTotal - receivedLength;
                (void)CanTp_GetRxCurrentBs(rxStatus->RxNSduId, globalInfo);
                if (E_OK == CanTp_CompareRxBufferWithBs(rxStatus, bufferSize))
                {
                    rxStatus->ChannelState = CANTP_RX_TRANSMIT_FC_CFS;
                    rxStatus->Timer        = rxNSduCfgPtr->N_Ar;
                    CanTp_SendFC(rxNSduCfgPtr, rxStatus, globalInfo);
                }
                else
                {
                    // buffer insufficient for CF receiving
                    rxStatus->ChannelState = CANTP_RX_WAIT_BUFFER_CF;
                    rxStatus->Timer        = rxNSduCfgPtr->N_Br;
                }
                break;
            case BUFREQ_E_OVFL:
                rxStatus->ChannelState = CANTP_RX_TRANSMIT_FC_OVFL;
                rxStatus->Timer        = rxNSduCfgPtr->N_Ar;
                CanTp_SendFC(rxNSduCfgPtr, rxStatus, globalInfo);
                break;
            case BUFREQ_E_NOT_OK:
            case BUFREQ_E_BUSY:
            default:
                CanTp_InitRxChannel(rxStatus);
                PduR_CanTpRxIndication(rxNSduCfgPtr->RxIPduId, E_NOT_OK);
                break;
            }
        }
        break;
    case BUFREQ_E_OVFL:
        /* SWS_CanTp_00318*/
        /* start Ar time*/
        rxStatus->Timer        = rxNSduCfgPtr->N_Ar;
        rxStatus->ChannelState = CANTP_RX_TRANSMIT_FC_OVFL;
        CanTp_SendFC(rxNSduCfgPtr, rxStatus, globalInfo);
        break;
    case BUFREQ_E_BUSY:
    case BUFREQ_E_NOT_OK: /* SWS_CanTp_00081*/
    default:
        /*abort the N-SDU reception without notify upper*/
        CanTp_InitRxChannel(rxStatus);
        break;
    }
}
/* PRQA S 6070 -- */

/**
 * @brief  process received FF
 */
/* PRQA S 6030 ++ */ /* VL_MTR_CanTp_STMIF */
CANTP_LOCAL void CanTp_RxSubDealWithFF(
    const PduInfoType*      pduInfoPtr,
    const CanTp_RxNSduType* rxNSduCfgPtr,
    CanTp_GPartInfoType*    globalInfo)
{
#if (CANTP_MULTIPLE_PARTITION_ENABLED == STD_ON)
    CanTp_RxStatusType* rxStatus =
        &globalInfo->CanTpRxConnectionStatus[rxNSduCfgPtr->RxNSduId - CANTP_GETRXNSDUSTART(globalInfo->index)];
#else
    CanTp_RxStatusType* rxStatus = &globalInfo->CanTpRxConnectionStatus[rxNSduCfgPtr->RxNSduId];
#endif
#if (STD_ON == CANTP_RUNTIME_ERROR_DETECT)
    uint8 lErrorId = CANTP_E_NO_ERROR;
#endif

    if (pduInfoPtr->SduLength < CANTP_CAN20_FRAME_LEN_MAX)
    {
#if (STD_ON == CANTP_RUNTIME_ERROR_DETECT)
        /* SWS_CanTp_00352 */
        lErrorId = CANTP_E_PADDING;
#endif
    }
    else if (CANTP_FUNCTIONAL == rxNSduCfgPtr->RxTaType)
    {
#if (STD_ON == CANTP_RUNTIME_ERROR_DETECT)
        /*invalid TAType with CF frame, wrong configuration*/ /* SWS_CanTp_00093 */
        lErrorId = CANTP_E_INVALID_TATYPE;
#endif
    }
#if (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON)
    else if ((rxStatus->ChannelState != CANTP_RX_IDLE) && (pduInfoPtr->MetaDataPtr != NULL_PTR))
    {
        /** @see SWS_CanTp_00337
         *   When an SF or FF N-PDU with MetaData (indicating a generic connection) is received, and the corresponding
         * connection channel is currently receiving, the SF or FF shall be ignored.
         *  */
    }
#endif
    else
    {
        PduLengthType         totalLength;
        uint8                 dataOffset;
        CanTp_CheckResultType ret = CanTp_CheckReceivedFFDL(pduInfoPtr, rxNSduCfgPtr, &totalLength, &dataOffset);
        if (CANTP_CHECK_NOT_OK != ret)
        {
#if (CANTP_TRANSMIT_CAN20_WITH_CANFD == STD_ON)
            /* CANFD channel is revicving CAN2.0 Frame ,whether Enable, CanFD channel, Can2.0Frame*/
            if ((CANTP_FLAGISSET(rxNSduCfgPtr->CanTpPduFlag, CANTP_CAN20_CANFD_MASK))
                && (pduInfoPtr->MetaDataPtr != NULL_PTR)
                && (CANTP_FLAG_ISNSET(pduInfoPtr->MetaDataPtr[CANTP_ID_OFFSET_META_IF], CANTP_CANID_CANFD_MASK)))
            {
                CANTP_SETFLAG(rxStatus->EventFlags, CANTP_EVENT_CAN20_WITH_CANFD); /* PRQA S 3473 */ /* VL_CanTp_3473 */
            }
            else
            {
                /* PRQA S 3473 ++ */ /* VL_CanTp_3473 */
                CANTP_CLEARFLAG(rxStatus->EventFlags, CANTP_EVENT_CAN20_WITH_CANFD);
                /* PRQA S 3473 -- */
            }
#endif
            if (rxStatus->ChannelState != CANTP_RX_IDLE)
            {
                /* Handling of unexpected N-PDU arrival */
                if (rxStatus->ChannelState > CANTP_RX_WAIT_BUFFER_CF)
                {
#if (CANTP_MULTIPLE_PARTITION_ENABLED == STD_ON)
                    CanTp_ConfirmDirectionType* txDirection =
                        &globalInfo
                             ->CanTpTxDirection[rxNSduCfgPtr->TxFcNPduId - CANTP_GETTXNPDUSTART(globalInfo->index)];
#else
                    CanTp_ConfirmDirectionType* txDirection = &globalInfo->CanTpTxDirection[rxNSduCfgPtr->TxFcNPduId];
#endif
                    txDirection->TxDirection  = CANTP_DIR_IDLE;
                    txDirection->ConnectionId = CANTP_INVALID_U16;
                }
                CanTp_InitRxChannel(rxStatus);
                PduR_CanTpRxIndication(rxNSduCfgPtr->RxIPduId, E_NOT_OK);
            }
            rxStatus->PCIOffset = CANTP_RXGETADROFFSET(rxNSduCfgPtr->AddressingFormat);
            rxStatus->RxNSduId  = rxNSduCfgPtr->RxNSduId;
            rxStatus->RxPduId   = rxNSduCfgPtr->RxIPduId;
            if (ret == CANTP_CHECK_OVERFLOW)
            {
                rxStatus->Timer        = rxNSduCfgPtr->N_Ar;
                rxStatus->ChannelState = CANTP_RX_TRANSMIT_FC_OVFL;
                CanTp_SendFC(rxNSduCfgPtr, rxStatus, globalInfo);
            }
            else
            {
                rxStatus->ChannelState   = CANTP_RX_RECEIVE_FF;
                rxStatus->InitalDLC      = pduInfoPtr->SduLength;
                rxStatus->DatalenTotal   = totalLength;
                rxStatus->DatalenRemain  = totalLength;
                rxStatus->HandledCfCount = 0u;
                rxStatus->SentWftCount   = 0u;
                rxStatus->ExpectdCfSn    = 1u;
                PduInfoType upperpduInfo;
                upperpduInfo.SduLength   = 0u;
                upperpduInfo.SduDataPtr  = &pduInfoPtr->SduDataPtr[dataOffset]; /*SWS_CanTp_00329*/
                upperpduInfo.MetaDataPtr = NULL_PTR;
#if (CANTP_DYN_ID_SUPPORT == STD_ON)
#if ((CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON) || (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON))
                uint8 data[CANTP_METADATA_MAXLENGTH] = {0};
#endif
                if (E_OK == CanTp_SaveRxMetaDataInfo(rxStatus, rxNSduCfgPtr, pduInfoPtr))
                {
#if ((CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON) || (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON))
                    if ((rxNSduCfgPtr->AddressingFormat == CANTP_NORMALFIXED)
                        || (rxNSduCfgPtr->AddressingFormat == CANTP_MIXED29BIT))
                    {
                        /** @see SWS_CanTp_00331
                         *   for N-SDU with MetaData, the CanTp module shall forward the extracted addressing
                         * information via the MetaData of the N-SDU
                         *  */
                        data[CANTP_SA_OFFSET_META_UPPER] = rxStatus->MetaDataNSa;
                        data[CANTP_TA_OFFSET_META_UPPER] = rxStatus->MetaDataNTa;
                        upperpduInfo.MetaDataPtr         = data;
                    }
#endif
                }
#endif
                PduLengthType receivedLen = pduInfoPtr->SduLength - dataOffset;
                CanTp_RxSubDealWithFFTOUP(globalInfo, rxStatus, &upperpduInfo, receivedLen);
            }
        }
    }
#if (STD_ON == CANTP_RUNTIME_ERROR_DETECT)
    if (lErrorId != CANTP_E_NO_ERROR)
    {
        CANTP_DET_REPORTRUNTIMEERROR(CANTP_SERVICEID_RXINDICATION, lErrorId);
    }
#endif
}
/* PRQA S 6030 -- */

/**
 * @brief Setup a new unsegmented connection and inform PduR
 */
CANTP_LOCAL void CanTp_RxSubDealWithSFToUp(
    CanTp_RxStatusType*     rxStatus,
    const CanTp_RxNSduType* rxNSduCfgPtr,
    PduInfoType*            upperpduInfoPtr,
    PduLengthType           receivedLength)
{
    PduLengthType bufferSize;
    /* SWS_CanTp_00079 */
    BufReq_ReturnType bufRslt =
        PduR_CanTpStartOfReception(rxNSduCfgPtr->RxIPduId, upperpduInfoPtr, receivedLength, &bufferSize);
    switch (bufRslt)
    {
    case BUFREQ_OK:
    {
#if (STD_ON == CANTP_RX_QUEUE)
#if (CANTP_ECUC_MULTIPLE_PARTITION_EXIST == STD_ON)
        uint8                index      = CanTp_ApplicationMap[rxNSduCfgPtr->ApplicationId];
        CanTp_GPartInfoType* globalInfo = &CanTp_GlobalPartitionInfo[index];
#else
        CanTp_GPartInfoType* globalInfo = &CanTp_GlobalPartitionInfo[0];
#endif
        CanTp_AddRxQueue(globalInfo->RxQueue, rxNSduCfgPtr->RxNSduId, &globalInfo->CurrentIndex);
#endif
        /* SWS_CanTp_00080 */
        if (bufferSize < receivedLength)
        {
            /* SWS_CanTp_00339 */
            PduR_CanTpRxIndication(rxNSduCfgPtr->RxIPduId, E_NOT_OK);
            CanTp_InitRxChannel(rxStatus);
        }
        else
        {
            upperpduInfoPtr->SduLength = receivedLength;
            bufRslt                    = PduR_CanTpCopyRxData(rxNSduCfgPtr->RxIPduId, upperpduInfoPtr, &bufferSize);
            if (bufRslt == BUFREQ_OK)
            {
#if (STD_ON == CANTP_SYNCHRONOUS_RXINDICATION)
                PduR_CanTpRxIndication(rxNSduCfgPtr->RxIPduId, E_OK);
#if (CANTP_TRANSMIT_CAN20_WITH_CANFD == STD_ON)
                if CANTP_FLAGISSET (rxStatus->EventFlags, CANTP_EVENT_CAN20_WITH_CANFD)
                {
                    CanTp_ChannelCanWithCanFD[rxNSduCfgPtr->ChannelId].IsSupport = TRUE;
                }
                else
                {
                    CanTp_ChannelCanWithCanFD[rxNSduCfgPtr->ChannelId].IsSupport = FALSE;
                }
#endif
                CanTp_InitRxChannel(rxStatus);
#else
                if ((upperpduInfoPtr->SduLength == CANTP_PARALLEL_LENGTH)
                    && (upperpduInfoPtr->SduDataPtr[0u] == CANTP_PARALLEL_SID)
                    && (upperpduInfoPtr->SduDataPtr[1u] == CANTP_PARALLEL_SUBFUNCTION)
                    && (rxNSduCfgPtr->RxTaType == CANTP_FUNCTIONAL))
                {
                    PduR_CanTpRxIndication(rxNSduCfgPtr->RxIPduId, E_OK);
                    CanTp_InitRxChannel(rxStatus);
                }
                else
                {
                    /* PRQA S 3473 ++*/ /* VL_CanTp_3473 */
                    CANTP_SETFLAG(rxStatus->EventFlags, CANTP_EVENT_RXNOTIFIUPPER);
                }
                /* PRQA S 3473 --*/
#endif
            }
            else
            {
                PduR_CanTpRxIndication(rxNSduCfgPtr->RxIPduId, E_NOT_OK);
                CanTp_InitRxChannel(rxStatus);
            }
        }
        break;
    }
    case BUFREQ_E_NOT_OK: /* SWS_CanTp_00081*/
    case BUFREQ_E_OVFL:   /* SWS_CanTp_00353*/
    default:
        /*abort the N-SDU reception without notify upper*/ /* reset channel */
        CanTp_InitRxChannel(rxStatus);
        break;
    }
}
/**
 * @brief  process received SF and notify upper to copy
 */
CANTP_LOCAL void CanTp_RxSubDealWithSF(
    const PduInfoType*      pduInfoPtr,
    const CanTp_RxNSduType* rxNSduCfgPtr,
    CanTp_GPartInfoType*    globalInfo) /* PRQA S 3673 */ /* VL_CanTp_3673 */
{
    boolean furtherProcess = TRUE;
#if (CANTP_MULTIPLE_PARTITION_ENABLED == STD_ON)
    CanTp_RxStatusType* rxStatus =
        &globalInfo->CanTpRxConnectionStatus[rxNSduCfgPtr->RxNSduId - CANTP_GETRXNSDUSTART(globalInfo->index)];
#else
    CanTp_RxStatusType* rxStatus = &globalInfo->CanTpRxConnectionStatus[rxNSduCfgPtr->RxNSduId];
#endif
#if ((STD_ON == CANTP_RUNTIME_ERROR_DETECT) && (CANTP_ENABLED_RX_BYTE_PADDING == STD_ON))
    if ((rxNSduCfgPtr->RxPaddingActivation == CANTP_PADDING_ON) && (pduInfoPtr->SduLength < CANTP_CAN20_FRAME_LEN_MAX))
    {
        furtherProcess = FALSE;
        CANTP_DET_REPORTRUNTIMEERROR(CANTP_SERVICEID_RXINDICATION, CANTP_E_PADDING);
    }
#endif
#if (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON)
    if ((furtherProcess) && (rxStatus->ChannelState != CANTP_RX_IDLE) && (pduInfoPtr->MetaDataPtr != NULL_PTR))
    {
        /* SWS_CanTp_00337
         When an SF or FF N-PDU with MetaData (indicating a generic connection) is received, and the corresponding
         connection channel is currently receiving, the SF or FF shall be ignored. */
        furtherProcess = FALSE;
    }
#endif
    if (furtherProcess)
    { /* check length */
        uint8         dataOffset;
        PduLengthType receivedLength;
        if (TRUE
            == CanTp_CheckReceivedSFDL(
                pduInfoPtr,
                rxNSduCfgPtr,
                &receivedLength,
                &dataOffset
#if (CANTP_TRANSMIT_CAN20_WITH_CANFD == STD_ON)
                ,
                rxStatus
#endif
                ))
        {
            if (rxStatus->ChannelState != CANTP_RX_IDLE)
            {
                /* Handling of unexpected N-PDU arrival */
                CanTp_InitRxChannel(rxStatus);
                PduR_CanTpRxIndication(rxNSduCfgPtr->RxIPduId, E_NOT_OK);
            }
            /* Setup new connection */
            rxStatus->ChannelState = CANTP_RX_RECEIVE_SF;
            rxStatus->RxNSduId     = rxNSduCfgPtr->RxNSduId;
            PduInfoType upperpduInfo;
            upperpduInfo.SduLength   = 0u;
            upperpduInfo.SduDataPtr  = &pduInfoPtr->SduDataPtr[dataOffset]; /*SWS_CanTp_00329*/
            upperpduInfo.MetaDataPtr = NULL_PTR;
#if (CANTP_DYN_ID_SUPPORT == STD_ON)
#if ((CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON) || (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON))
            uint8 data[CANTP_METADATA_MAXLENGTH] = {0};
#endif
            if (E_OK == CanTp_SaveRxMetaDataInfo(rxStatus, rxNSduCfgPtr, pduInfoPtr))
            {
#if ((CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON) || (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON))
                if ((rxNSduCfgPtr->AddressingFormat == CANTP_NORMALFIXED)
                    || (rxNSduCfgPtr->AddressingFormat == CANTP_MIXED29BIT))
                {
                    /** @see SWS_CanTp_00331
                     *   for N-SDU with MetaData, the CanTp module shall forward the extracted addressing information
                     *   via the MetaData of the N-SDU
                     *  */
                    data[CANTP_SA_OFFSET_META_UPPER] = rxStatus->MetaDataNSa;
                    data[CANTP_TA_OFFSET_META_UPPER] = rxStatus->MetaDataNTa;
                    upperpduInfo.MetaDataPtr         = data;
                }
#endif
            }
#endif
            CanTp_RxSubDealWithSFToUp(rxStatus, rxNSduCfgPtr, &upperpduInfo, receivedLength);
        }
    }
}

/**
 * @brief  process received SF/FF/CF/FC frame
 */
/* PRQA S 6070 ++ */ /* VL_MTR_CanTp_STCAL */
CANTP_LOCAL void CanTp_RxSubDeal(PduIdType rxPduId, const PduInfoType* pduInfoPtr, CanTp_FramePCIType FramePCIType)
{
#if (CANTP_ECUC_MULTIPLE_PARTITION_EXIST == STD_ON)
    ApplicationType curAppId = GetApplicationID();
#endif
    if (FramePCIType == CANTP_FTYPE_FC)
    {
        const CanTp_TxNSduType* txNSduCfgPtr = CanTp_GetTxNSduCfg(CanTp_GetTxNSduIndex_FC(rxPduId));

        if ((NULL_PTR != txNSduCfgPtr)
#if (CANTP_ECUC_MULTIPLE_PARTITION_EXIST == STD_ON)
            && (curAppId == txNSduCfgPtr->ApplicationId)
#endif
        )
        {
            /*A FC received*/
#if (CANTP_ECUC_MULTIPLE_PARTITION_EXIST == STD_ON)
            CanTp_GPartInfoType* globalInfo = CanTp_GetPartitionInfoPtr(curAppId);
#else
            CanTp_GPartInfoType* globalInfo = CanTp_GlobalPartitionInfo;
#endif
            if (globalInfo != NULL_PTR)
            {
                CanTp_RxSubDealWithFC(pduInfoPtr, txNSduCfgPtr, globalInfo);
            }
        }
    }
    else
    {
        const CanTp_RxNSduType* rxNSduCfgPtr = CanTp_GetRxNSduCfg(CanTp_GetRxNSduIndex(rxPduId));
        if ((NULL_PTR != rxNSduCfgPtr)
#if (CANTP_ECUC_MULTIPLE_PARTITION_EXIST == STD_ON)
            && (curAppId == rxNSduCfgPtr->ApplicationId)
#endif
        )
        {
#if (CANTP_ECUC_MULTIPLE_PARTITION_EXIST == STD_ON)
            CanTp_GPartInfoType* globalInfo = CanTp_GetPartitionInfoPtr(curAppId);
#else
            CanTp_GPartInfoType* globalInfo = CanTp_GlobalPartitionInfo;
#endif
            if (globalInfo != NULL_PTR)
            {
                switch (FramePCIType)
                {
                case CANTP_FTYPE_CF:
                    CanTp_RxSubDealWithCF(pduInfoPtr, rxNSduCfgPtr, globalInfo);
                    break;
                case CANTP_FTYPE_FF:
                    CanTp_RxSubDealWithFF(pduInfoPtr, rxNSduCfgPtr, globalInfo);
                    break;
                case CANTP_FTYPE_SF:
                    CanTp_RxSubDealWithSF(pduInfoPtr, rxNSduCfgPtr, globalInfo);
                    break;
                default:
                    /*this will never happen*/
                    break;
                }
            }
        }
    }
}
/* PRQA S 6070 -- */

/**
 * @brief  transmit confirmation SF/FF/CF frame
 */
/* PRQA S 6030 ++ */ /* VL_MTR_CanTp_STMIF */
CANTP_LOCAL void CanTp_TxConfirmationTx(
    CanTp_ConfirmDirectionType* txDirection,
    CanTp_GPartInfoType*        globalInfo,
    Std_ReturnType              result)
{
    CanTp_TxStatusType*     txStatus     = &globalInfo->CanTpTxConnectionStatus[txDirection->ConnectionId];
    const CanTp_TxNSduType* txNSduCfgPtr = &CanTp_ConfigPtr->CanTpTxNSduCfg[txStatus->TxNSduId];
    txDirection->TxDirection             = CANTP_DIR_IDLE;
    txDirection->ConnectionId            = CANTP_INVALID_U16;
    if (NULL_PTR == txNSduCfgPtr)
    {
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
        /*SWS_CanTp_00359 No configured TX SDU matched with this TxSduId*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_TXCONFIRMATION, CANTP_E_INVALID_TX_ID);
#endif
    }
    else if (E_OK == result)
    {
        switch (txStatus->ChannelState)
        {
        case CANTP_TX_WAIT_SFCONF:
            CanTp_InitTxChannel(txStatus);
            /*Notify the upper transmit successfully*/
            PduR_CanTpTxConfirmation(txNSduCfgPtr->TxIPduId, E_OK);
            break;
        case CANTP_TX_WAIT_FFCONF:
            /* wait FC */
            txStatus->ChannelState = CANTP_TX_WAIT_RECEIVE_FC;
            txStatus->Timer        = txNSduCfgPtr->N_Bs;
            break;
        case CANTP_TX_WAIT_CFCONF:
            if (txStatus->DatalenRemain != 0u)
            {
                txStatus->HandledCfCount++;
                txStatus->ExpectdCfSn++;
                if (txStatus->HandledCfCount == txStatus->FcBs)
                {
                    /* wait FC */
                    txStatus->HandledCfCount = 0u;
                    txStatus->ChannelState   = CANTP_TX_WAIT_RECEIVE_FC;
                    txStatus->Timer          = txNSduCfgPtr->N_Bs;
                }
                else if (txStatus->FcSTmin == 0u)
                {
                    /* next CF */
                    txStatus->ChannelState = CANTP_TX_TRANSMIT_CF;
                    txStatus->Timer        = txNSduCfgPtr->N_Cs;
#if (CANTP_STMIN_IMMEDIATE_TX_CONFIRMATION == STD_ON)
                    (void)CanTp_GetTxBuffer(txStatus, txNSduCfgPtr, globalInfo);
#endif
                }
                else
                {
                    txStatus->ChannelState = CANTP_TX_WAIT_CF_STMIN;
                    txStatus->Timer        = txNSduCfgPtr->N_Cs;
                    /* PRQA S 4491 ++ */ /* VL_CanTp_4491 */
                    txStatus->STminTimer = txStatus->FcSTmin;
                    /* PRQA S 4491 -- */
                }
            }
            else
            {
                CanTp_InitTxChannel(txStatus);
                /*Notify the upper transmit successfully*/
                PduR_CanTpTxConfirmation(txNSduCfgPtr->TxIPduId, E_OK);
            }
            break;
        default:
            /*idle*/
            break;
        }
    }
    else
    {
        CanTp_InitTxChannel(txStatus);
        /*Notify the upper*/
        PduR_CanTpTxConfirmation(txNSduCfgPtr->TxIPduId, E_NOT_OK);
    }
}
#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"
/* ========================================== external function definitions ========================================= */
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"
#if (CANTP_TC == STD_ON)
/**
 * @brief Requests cancellation of an ongoing transmission of a PDU in a lower layer communication module.
 */
Std_ReturnType CanTp_CancelTransmit(PduIdType TxPduId)
{
    Std_ReturnType result = E_NOT_OK;
    /*check module state, handle the request only when module started*/
    if (CANTP_ON != CanTp_ModuleState)
    {
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
        /*CanTp module not started*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_CANCELTRANSMITREQUEST, CANTP_E_UNINIT);
#endif
    }
    else
    {
#if (CANTP_ECUC_MULTIPLE_PARTITION_EXIST == STD_ON)
        ApplicationType curAppId = GetApplicationID();
#endif
        uint16 internalId = TxPduId;
#if (CANTP_SUPPORT_VARIANT == STD_ON)
        internalId = CanTp_InternalTxNSduIndex(TxPduId);
#endif
        const CanTp_TxNSduType* txNSduCfgPtr = CanTp_GetTxNSduCfg(internalId);
        if ((internalId >= CanTp_ConfigPtr->CanTpTxNSduNum) || (txNSduCfgPtr == NULL_PTR)
#if (CANTP_ECUC_MULTIPLE_PARTITION_EXIST == STD_ON)
            || (curAppId != txNSduCfgPtr->ApplicationId)
#endif
        )
        {
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
            /**Invalid TxNSduId*/
            CANTP_DET_REPORTERROR(CANTP_SERVICEID_CANCELTRANSMITREQUEST, CANTP_E_INVALID_TX_ID);
#endif
        }
        else
        {
            /* PRQA S 3678++ */ /* VL_CanTp_3678 */
#if (CANTP_ECUC_MULTIPLE_PARTITION_EXIST == STD_ON)
            CanTp_GPartInfoType* globalInfo = CanTp_GetPartitionInfoPtr(curAppId);
#else
            CanTp_GPartInfoType* globalInfo = CanTp_GlobalPartitionInfo;
#endif
            /* PRQA S 3678 -- */
#if (CANTP_MULTIPLE_PARTITION_ENABLED == STD_ON)
            CanTp_TxStatusType* txStatus =
                &(globalInfo->CanTpTxConnectionStatus[internalId - CANTP_GETTXNSDUSTART(globalInfo->index)]);
#else
            CanTp_TxStatusType* txStatus = &(globalInfo->CanTpTxConnectionStatus[internalId]);
#endif
            if (CANTP_TX_IDLE == txStatus->ChannelState)
            {
#if (STD_ON == CANTP_RUNTIME_ERROR_DETECT)
                /*not on transmission process*/
                CANTP_DET_REPORTRUNTIMEERROR(CANTP_SERVICEID_CANCELTRANSMITREQUEST, CANTP_E_OPER_NOT_SUPPORTED);
#endif
            }
            else if (CANTP_TC_MASK == (txNSduCfgPtr->CanTpPduFlag & CANTP_TC_MASK))
            {
#if (CANTP_MULTIPLE_PARTITION_ENABLED == STD_ON)
                CanTp_ConfirmDirectionType* txDirection =
                    &globalInfo->CanTpTxDirection[txStatus->TxPduId - CANTP_GETTXNPDUSTART(globalInfo->index)];
#else
                CanTp_ConfirmDirectionType* txDirection = &globalInfo->CanTpTxDirection[txStatus->TxPduId];
#endif
                if (txDirection->TxDirection == CANTP_DIR_TX)
                {
                    txDirection->TxDirection  = CANTP_DIR_IDLE;
                    txDirection->ConnectionId = CANTP_INVALID_U16;
                }
                PduR_CanTpTxConfirmation(txNSduCfgPtr->TxIPduId, E_NOT_OK);
                CanTp_InitTxChannel(txStatus);
                result = E_OK;
            }
            else
            {
                /** ilde */
            }
        }
    }
    return result;
}
#endif
/* PRQA S 6030 -- */

#if (CANTP_RC == STD_ON)
/**
 * @brief Requests cancellation of an ongoing reception of a PDU in a lower layer transport protocol module.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_CanTp_STCAL */
Std_ReturnType CanTp_CancelReceive(PduIdType RxPduId)
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == CANTP_DEV_ERROR_DETECT)
    /*check module state, handle the request only when module started*/
    if (CANTP_ON != CanTp_ModuleState)
    {
        /*CanTp module not started*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_CANCELRECEIVEREQUEST, CANTP_E_UNINIT);
    }
    else
#endif
    {
        uint16 internalId = RxPduId;
#if (CANTP_SUPPORT_VARIANT == STD_ON)
        internalId = CanTp_InternalRxNSduIndex(RxPduId);
#endif
#if (CANTP_ECUC_MULTIPLE_PARTITION_EXIST == STD_ON)
        ApplicationType curAppId = GetApplicationID();
#endif
        const CanTp_RxNSduType* rxNSduCfgPtr =
            (internalId < CanTp_ConfigPtr->CanTpRxNSduNum) ? CanTp_GetRxNSduCfg(internalId) : NULL_PTR;

        if ((rxNSduCfgPtr == NULL_PTR)
#if (CANTP_ECUC_MULTIPLE_PARTITION_EXIST == STD_ON)
            || (curAppId != rxNSduCfgPtr->ApplicationId)
#endif
        )
        {
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
            /**Invalid RxNSduId*/
            CANTP_DET_REPORTERROR(CANTP_SERVICEID_CANCELRECEIVEREQUEST, CANTP_E_INVALID_RX_ID);
#endif
        }
        else
        {
            /* PRQA S 3678++ */ /* VL_CanTp_3678 */
#if (CANTP_ECUC_MULTIPLE_PARTITION_EXIST == STD_ON)
            CanTp_GPartInfoType* globalInfo = CanTp_GetPartitionInfoPtr(curAppId);
#else
            CanTp_GPartInfoType* globalInfo = CanTp_GlobalPartitionInfo;
#endif
            /* PRQA S 3678-- */
#if (CANTP_MULTIPLE_PARTITION_ENABLED == STD_ON)
            uint8 index = CanTp_GetRxNSduIndex(internalId) - CANTP_GETRXNSDUSTART(globalInfo->index);
#else
            uint8 index = CanTp_GetRxNSduIndex(internalId);
#endif
            CanTp_RxStatusType* rxStatus = &globalInfo->CanTpRxConnectionStatus[index];
            if (CANTP_RX_IDLE == rxStatus->ChannelState)
            {
                /*not on reception process*/
#if (STD_ON == CANTP_RUNTIME_ERROR_DETECT)
                CANTP_DET_REPORTRUNTIMEERROR(CANTP_SERVICEID_CANCELRECEIVEREQUEST, CANTP_E_OPER_NOT_SUPPORTED);
#endif
            }
            else if ((CANTP_RX_RECEIVE_SF != rxStatus->ChannelState) && (rxStatus->DatalenRemain > rxStatus->InitalDLC))
            {
                PduR_CanTpRxIndication(rxNSduCfgPtr->RxIPduId, E_NOT_OK);
                /*Release the resource related with the channel and Notify the upper*/
                CanTp_InitRxChannel(rxStatus);
                result = E_OK;
            }
            else
            {
                /* ilde */
            }
        }
    }
    return result;
}
#endif

#if (CANTP_CHANGE_PARAMETER == STD_ON)
/**
 * @brief Request to change a specific transport protocol parameter (e.g. block size).
 */
/* PRQA S 6030 ++ */ /* VL_MTR_CanTp_STMIF */
Std_ReturnType CanTp_ChangeParameter(PduIdType id, TPParameterType parameter, uint16 value)
{
    Std_ReturnType result = E_NOT_OK;
    /**check module state, handle only when module started*/
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
    if (CANTP_ON != CanTp_ModuleState)
    {
        /**CanTp module not started*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_CHANGEPARAMETER, CANTP_E_UNINIT);
    }
    else
#endif
    {
        uint16 internalId = id;
#if (CANTP_SUPPORT_VARIANT == STD_ON)
        internalId = CanTp_InternalRxNSduIndex(id);
#endif
#if (CANTP_ECUC_MULTIPLE_PARTITION_EXIST == STD_ON)
        ApplicationType            curAppId     = GetApplicationID();
        const CanTp_RxNSduType*    rxNSduCfgPtr = &CanTp_ConfigPtr->CanTpRxNSduCfg[internalId];
        const CanTp_GPartInfoType* globalInfo   = CanTp_GetPartitionInfoPtr(curAppId);
#else
        const CanTp_GPartInfoType* globalInfo = CanTp_GlobalPartitionInfo;
#endif
        if ((internalId > CanTp_ConfigPtr->CanTpRxNSduNum)
#if (CANTP_ECUC_MULTIPLE_PARTITION_EXIST == STD_ON)
            || (curAppId != rxNSduCfgPtr->ApplicationId)
#endif
        )
        {
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
            /**CanTp module not started*/
            CANTP_DET_REPORTERROR(CANTP_SERVICEID_CHANGEPARAMETER, CANTP_E_INVALID_RX_ID);
#endif
        }
        else
        {
            if (TRUE == CanTP_CheckRxChangeParameter(parameter, value))
            {
                if (globalInfo != NULL_PTR)
                {
#if (CANTP_MULTIPLE_PARTITION_ENABLED == STD_ON)
                    uint16 index = internalId - CANTP_GETRXNSDUSTART(globalInfo->index);
#else
                    uint16 index = internalId;
#endif
                    const CanTp_RxStatusType*  rxStatus          = &(globalInfo->CanTpRxConnectionStatus[index]);
                    CanTp_ChangeParameterType* changeRxParameter = &globalInfo->CanTpChangeRxParameter[index];
                    if (rxStatus->ChannelState == CANTP_RX_IDLE)
                    {
                        if (TP_BS == parameter)
                        {
                            changeRxParameter->FcBs = (uint8)value;
                        }
                        else
                        {
                            changeRxParameter->FcSTmin = (uint8)value;
                        }
                        result = E_OK;
                    }
                }
            }
        }
    }
    return result;
}
#endif
/* PRQA S 6030 -- */
/* PRQA S 6070 -- */
#if (CANTP_READ_PARAMETER == STD_ON)
/**
 * @brief  This service is used to read the current value of reception parameters BS and STmin for a specified
 */
Std_ReturnType CanTp_ReadParameter(PduIdType id, TPParameterType parameter, uint16* value)
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == CANTP_DEV_ERROR_DETECT)
    /**check module state, handle only when module started*/
    if (CANTP_ON != CanTp_ModuleState)
    {
        /**CanTp module not started*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_READPARAMETER, CANTP_E_UNINIT);
    }
    else if ((parameter != TP_STMIN) && (parameter != TP_BS))
    {
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_READPARAMETER, CANTP_E_PARAM_ID);
    }
    else
#endif
    {
#if (CANTP_ECUC_MULTIPLE_PARTITION_EXIST == STD_ON)
        ApplicationType curAppId = GetApplicationID();
#endif
        uint16 internalId = id;
#if (CANTP_SUPPORT_VARIANT == STD_ON)
        internalId = CanTp_InternalRxNSduIndex(id);
#endif
        if ((internalId > CanTp_ConfigPtr->CanTpRxNSduNum)
#if (CANTP_ECUC_MULTIPLE_PARTITION_EXIST == STD_ON)
            || (curAppId != CanTp_GetRxNSduCfg(internalId)->ApplicationId)
#endif
        )
        {
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
            /**CanTp module not started*/
            CANTP_DET_REPORTERROR(CANTP_SERVICEID_READPARAMETER, CANTP_E_INVALID_RX_ID);
#endif
        }
        else if (NULL_PTR == value)
        {
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
            CANTP_DET_REPORTERROR(CANTP_SERVICEID_READPARAMETER, CANTP_E_PARAM_POINTER);
#endif
        }
        else /**All request parameters are valid*/
        {
#if (CANTP_ECUC_MULTIPLE_PARTITION_EXIST == STD_ON)
            CanTp_GPartInfoType* globalInfo = CanTp_GetPartitionInfoPtr(curAppId);
#else
            CanTp_GPartInfoType* globalInfo = CanTp_GlobalPartitionInfo;
#endif
            if (globalInfo != NULL_PTR)
            {
                if (TP_BS == parameter)
                {
                    *value = CanTp_GetRxCurrentBs(internalId, globalInfo);
                    result = E_OK;
                }
                else
                {
                    /** TP_STMIN == parameter */
                    *value = CanTp_GetRxCurrentSTmin(
                        internalId
#if (CANTP_CHANGE_PARAMETER == STD_ON)
                        ,
                        globalInfo
#endif
                    );
                    result = E_OK;
                }
            }
        }
    }
    return result;
}
#endif

#if (CANTP_VERSION_INFO_API == STD_ON)
/**
 * @brief  This function returns the version information of the CanTp module.
 */
void CanTp_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
    if (NULL_PTR == versioninfo)
    {
        /*report development error*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_GETVERSIONINFO, CANTP_E_PARAM_POINTER);
    }
    else
#endif
    {
        /*NULL_PTR != versioninfo*/
        versioninfo->moduleID         = CANTP_MODULE_ID;
        versioninfo->sw_major_version = CANTP_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = CANTP_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = CANTP_SW_PATCH_VERSION;
        versioninfo->vendorID         = CANTP_VENDOR_ID;
    }
}
#endif
/**
 * @brief  This function is called to shutdown the CanTp module.
 */
void CanTp_Shutdown(void)
{
    /*CanTp stops working*/
    CanTp_ModuleState = CANTP_OFF;
}

/* PRQA S 5017,6010,6080 ++ */ /* VL_CanTp_5017,VL_MTR_CanTp_STCYC,VL_MTR_CanTp_STPTH */
/**
 * @brief This function initializes the CanTp module.
 */
void CanTp_Init(const CanTp_ConfigType* CfgPtr)
{
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
    if (NULL_PTR == CfgPtr)
    {
        /**invalid transmit request information pointer*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_INIT, CANTP_E_PARAM_POINTER);
    }
    else if (
        (CfgPtr->CanTpRxNSduNum > CANTP_MAX_RXNSDU_NUMBER) || (CfgPtr->CanTpTxNSduNum > CANTP_MAX_TXNSDU_NUMBER)
        || (CfgPtr->CanTpRxNPduNum > CANTP_MAX_RXPDU_NUMBER) || (CfgPtr->CanTpTxNPduNum > CANTP_MAX_TXPDU_NUMBER))
    {
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_INIT, CANTP_E_INIT_FAILED);
    }
    else
#endif
    {
#if (CANTP_MULTIPLE_PARTITION_ENABLED == STD_ON)
        uint8 appId = GetApplicationID();
        if ((INVALID_OSAPPLICATION != appId)
            && (CANTP_INVALID_U8 != CanTp_ApplicationMap[appId])) /*check if the application is valid*/
#endif
        {
            if (CANTP_ON == CanTp_ModuleState)
            {
                CanTp_ModuleState = CANTP_OFF;
            }
            CanTp_ConfigPtr = CfgPtr;
            uint8 index     = 0u;
#if (CANTP_MULTIPLE_PARTITION_ENABLED == STD_ON)
            CanTp_GPartInfoType* globalInfo = &CanTp_GlobalPartitionInfo[CanTp_ApplicationMap[appId]];
            uint8 rxNum = CANTP_GETRXNSDUEND(globalInfo->index) - CANTP_GETRXNSDUSTART(globalInfo->index);
            for (index = 0u; index <= rxNum; index++)
#else
            const CanTp_GPartInfoType* globalInfo = &CanTp_GlobalPartitionInfo[0];
            for (index = 0u; index < CanTp_ConfigPtr->CanTpRxNSduNum; index++)
#endif
            {
                if (globalInfo->CanTpRxConnectionStatus != NULL_PTR)
                {
#if (CANTP_MULTIPLE_PARTITION_ENABLED == STD_ON)
                    uint8 iloop = index + CANTP_GETRXNSDUEND(globalInfo->index);
#else
                    uint8 iloop = index;
#endif
#if (STD_ON == CANTP_RX_QUEUE)
                    globalInfo->RxQueue[index] = CANTP_INVALID_U8;
#endif
#if (CANTP_CHANGE_PARAMETER == STD_ON)
                    globalInfo->CanTpChangeRxParameter[index].FcBs    = CanTp_ConfigPtr->CanTpRxNSduCfg[iloop].Bs;
                    globalInfo->CanTpChangeRxParameter[index].FcSTmin = CanTp_ConfigPtr->CanTpRxNSduCfg[iloop].STmin;
#endif
                    CanTp_InitRxChannel(&globalInfo->CanTpRxConnectionStatus[index]);
                }
            }
#if (CANTP_MULTIPLE_PARTITION_ENABLED == STD_ON)
            uint8 txNum = CANTP_GETTXNSDUEND(globalInfo->index) - CANTP_GETTXNSDUSTART(globalInfo->index);
            for (index = 0u; index <= txNum; index++)
#else
            for (index = 0u; index < CanTp_ConfigPtr->CanTpTxNSduNum; index++)
#endif
            {
                if (globalInfo->CanTpTxConnectionStatus != NULL_PTR)
                {
                    CanTp_InitTxChannel(&globalInfo->CanTpTxConnectionStatus[index]);
                }
            }
#if (CANTP_MULTIPLE_PARTITION_ENABLED == STD_ON)
            uint8 num = CANTP_GETTXNPDUEND(globalInfo->index) - CANTP_GETTXNPDUSTART(globalInfo->index);
            for (index = 0u; index <= num; index++)
#else
            for (index = 0u; index < CanTp_ConfigPtr->CanTpTxNPduNum; index++)
#endif
            {
                if (globalInfo->CanTpTxDirection != NULL_PTR)
                {
                    globalInfo->CanTpTxDirection[index].ConnectionId = CANTP_INVALID_U16;
                    globalInfo->CanTpTxDirection[index].TxDirection  = CANTP_DIR_IDLE;
                }
            }
#if (CANTP_TRANSMIT_CAN20_WITH_CANFD == STD_ON)
            for (index = 0u; index < CANTP_CHANNEL_NUMBER; index++)
            {
                CanTp_ChannelCanWithCanFD[index].IsSupport = FALSE;
            }
#endif
            /**turn the switch of module state on,CanTp start to work*/
            CanTp_ModuleState = CANTP_ON;
        }
    }
}
/* PRQA S 5017,6010,6080 -- */

/**
 * @brief The main function for scheduling the CAN TP.
 */
void CanTp_MainFunction(void)
{
    /**check module state, handle only when module started*/
    if (CANTP_ON == CanTp_ModuleState)
    {
        CanTp_HandleRxChannels();
        CanTp_HandleTxChannels();
    }
}
#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"

#define CANTP_START_SEC_CODE_FAST
#include "CanTp_MemMap.h"
/**
 *@brief receive SF/FF/CF/FC frame
 */
void CanTp_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr)
{
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
    /**check module state, handle the reception indication only when module started*/
    if (CANTP_ON != CanTp_ModuleState)
    {
        /*CanTp module not started*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_RXINDICATION, CANTP_E_UNINIT);
    }
    else if ((NULL_PTR == PduInfoPtr) || (NULL_PTR == PduInfoPtr->SduDataPtr))
    {
        /**INVALID pointer parameter*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_RXINDICATION, CANTP_E_PARAM_POINTER);
    }
    else if (RxPduId >= CANTP_MAX_RXPDU_NUMBER)
    {
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_RXINDICATION, CANTP_E_INVALID_RX_ID);
    }
    else
#endif
    {
        /**fill the received NPDU into channel*/
        CanTp_FramePCIType FramePCIType = CanTp_GetFramePCIType(RxPduId, PduInfoPtr);
        if (FramePCIType != CANTP_FTYPE_RESEVED)
        {
            CanTp_RxSubDeal(RxPduId, PduInfoPtr, FramePCIType);
        }
    }
}
/**
 * @brief Confirmation SF/FF/CF/FC frame
 */
void CanTp_TxConfirmation(PduIdType TxPduId, Std_ReturnType result)
{
    /**check module state, handle the reception indication only when module started*/
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
    if (CANTP_ON != CanTp_ModuleState)
    {
        /**CanTp module not started*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_TXCONFIRMATION, CANTP_E_UNINIT);
    }
    else
#endif
    {
#if (CANTP_ECUC_MULTIPLE_PARTITION_EXIST == STD_ON)
        ApplicationType      curAppId   = GetApplicationID();
        CanTp_GPartInfoType* globalInfo = CanTp_GetPartitionInfoPtr(curAppId);
#else
        CanTp_GPartInfoType* globalInfo = CanTp_GlobalPartitionInfo;
#endif
        PduIdType internalId = TxPduId;
#if (CANTP_SUPPORT_VARIANT == STD_ON)
        internalId = CanTp_InternalTxPduIndex(TxPduId);
#endif
        /* PRQA S 2996 ++ */ /* VL_CanTp_2996 */
        if ((globalInfo == NULL_PTR) || (internalId >= CanTp_ConfigPtr->CanTpTxNPduNum))
        /* PRQA S 2996 -- */
        {
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
            /**SWS_CanTp_00359 No configured TX SDU matched with this TxSduId*/
            CANTP_DET_REPORTERROR(CANTP_SERVICEID_TXCONFIRMATION, CANTP_E_INVALID_TX_ID);
#endif
        }
        else
        {
#if (CANTP_MULTIPLE_PARTITION_ENABLED == STD_ON)
            uint16 index = internalId - CANTP_GETTXNPDUSTART(globalInfo->index);
#else
            uint16 index = internalId;
#endif
            CanTp_ConfirmDirectionType* txDirection = &globalInfo->CanTpTxDirection[index];
            if (txDirection->TxDirection == CANTP_DIR_TX)
            {
                CanTp_TxConfirmationTx(txDirection, globalInfo, result);
            }
            else if (txDirection->TxDirection == CANTP_DIR_RX)
            {
                CanTp_RxStatusType* rxStatus = &globalInfo->CanTpRxConnectionStatus[txDirection->ConnectionId];
                txDirection->TxDirection     = CANTP_DIR_IDLE;
                txDirection->ConnectionId    = CANTP_INVALID_U16;
                CanTp_TxFCConfirmation(rxStatus, result);
            }
            else
            {
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
                CANTP_DET_REPORTERROR(CANTP_SERVICEID_TXCONFIRMATION, CANTP_E_INVALID_TX_ID);
#endif
            }
        }
    }
}

/**
 * @brief  Start transmission of SF/FF/CF/FC frame
 */
Std_ReturnType CanTp_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
    /**check module state, handle the request only when module started*/
    if (CANTP_ON != CanTp_ModuleState)
    {
        /**CanTp module not started*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_TRANSMIT, CANTP_E_UNINIT);
    }
    else if (NULL_PTR == PduInfoPtr)
    {
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_TRANSMIT, CANTP_E_PARAM_POINTER);
    }
    else
#endif
    {
        /**information pointer OK*/
#if (CANTP_ECUC_MULTIPLE_PARTITION_EXIST == STD_ON)
        ApplicationType curAppId = GetApplicationID();
#endif
        PduIdType index = TxPduId;
#if (CANTP_SUPPORT_VARIANT == STD_ON)
        index = CanTp_InternalTxNSduIndex(TxPduId);
#endif
        const CanTp_TxNSduType* txNSduCfgPtr = CanTp_GetTxNSduCfg(index);
        if ((NULL_PTR == txNSduCfgPtr)
#if (CANTP_ECUC_MULTIPLE_PARTITION_EXIST == STD_ON)
            || (curAppId != txNSduCfgPtr->ApplicationId)
#endif
        )
        {
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
            CANTP_DET_REPORTERROR(CANTP_SERVICEID_TRANSMIT, CANTP_E_INVALID_TX_ID);
#endif
        }
        else
        {
            /** setup a new Tx connection */
#if (CANTP_ECUC_MULTIPLE_PARTITION_EXIST == STD_ON)
            CanTp_GPartInfoType* globalInfo = CanTp_GetPartitionInfoPtr(curAppId);
#else
            CanTp_GPartInfoType* globalInfo = CanTp_GlobalPartitionInfo;
#endif
            if (globalInfo != NULL_PTR)
            {
                ret = CanTp_TxSetupConnection(index, txNSduCfgPtr, PduInfoPtr, globalInfo);
            }
        }
    }
    return ret;
}
#define CANTP_STOP_SEC_CODE_FAST
#include "CanTp_MemMap.h"
/* PRQA S 4464,4461,2995,2991,2992 -- */
/* PRQA S 1503,1532,1505 -- */
/* PRQA S 3120 -- */
