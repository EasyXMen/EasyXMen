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
 **  @file               : LinTp_Slave.c
 **  @author             : HuRongbo
 **  @date               :
 **  @vendor             : isoft
 **  @description        : LinTp slave implementation source file
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "LinIf_Cfg.h"
#if (LINIF_TP_SUPPORTED == STD_ON)
#include "LinTp_Cfg.h"

#if (LINTP_SLAVE_SUPPORT == STD_ON)
#include "LinTp_Slave.h"
#include "LinIf_Internal.h"
#include "PduR_LinTp.h"
#include "LinIf_Slave.h"

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ============================================ internal data definition ============================================ */

/* ========================================= internal function declarations ========================================= */
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

LINTP_LOCAL_INLINE LinTp_SlaveRuntimeType* LinTp_GetSlaveRtDataPtr(uint8 ch)
{
    return LinTp_ConfigPtr->RtDataVariantMapPtr->SlaveRtDataPtr[ch - LinTp_ConfigPtr->NumOfMasterChannel];
}

LINTP_LOCAL void LinTp_SlaveSFRxHandle(NetworkHandleType tpCh, const uint8* Lin_SduPtr, const LinTp_RxNSduType* rxNSdu);

LINTP_LOCAL void LinTp_SlaveFFRxHandle(NetworkHandleType tpCh, const uint8* Lin_SduPtr, const LinTp_RxNSduType* rxNSdu);

LINTP_LOCAL void LinTp_SlaveCFRxHandle(NetworkHandleType tpCh, const uint8* Lin_SduPtr);

LINTP_LOCAL void LinTp_SlaveSFRxMainHandle(LinTp_SlaveRuntimeType* tpSlaveRTPtr);

LINTP_LOCAL void LinTp_SlaveFFRxMainHandle(LinTp_SlaveRuntimeType* tpSlaveRTPtr);

LINTP_LOCAL void LinTp_SlaveCFRxMainHandle(LinTp_SlaveRuntimeType* tpSlaveRTPtr);

LINTP_LOCAL void LinTp_SlaveSFTxHandle(NetworkHandleType ch, Lin_PduType* PduPtr, LinTp_SlaveRuntimeType* tpSlaveRTPtr);

LINTP_LOCAL void LinTp_SlaveFFTxHandle(NetworkHandleType ch, Lin_PduType* PduPtr, LinTp_SlaveRuntimeType* tpSlaveRTPtr);

LINTP_LOCAL void LinTp_SlaveCFTxHandle(NetworkHandleType ch, Lin_PduType* PduPtr, LinTp_SlaveRuntimeType* tpSlaveRTPtr);

LINTP_LOCAL void              LinTp_SlaveAbortTxRxAndNotifyFailToUpper(LinTp_SlaveRuntimeType* tpSlaveRTPtr);
LINTP_LOCAL NetworkHandleType LinTp_SlaveGetLinTpChannel(NetworkHandleType ComMChannel);
LINTP_LOCAL void              LinTp_SlaveTimerHandle(LinTp_SlaveRuntimeType* tpSlaveRTPtr);
LINTP_LOCAL void              LinTp_SlaveResetRtData(LinTp_SlaveRuntimeType* tpSlaveRTPtr);

#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

/* ============================================ internal data definition ============================================ */

/* ============================================ external data definition ============================================ */

/* ========================================== external function definition ========================================== */

/* PRQA S 2814,2824,2812 ++ */ /* VL_QAC_DerefNullPtr */
/* PRQA S 1532 ++ */           /* VL_QAC_OneFunRef */

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

/**
 * LinTp slave node runtime data init
 */
void LinTp_SlaveInit(void)
{
    LinTp_SlaveRuntimeType* tpSlaveRTPtr = LinTp_ConfigPtr->RtDataVariantMapPtr->SlaveRtDataPtr[0u];
    uint16                  idx          = LinTp_ConfigPtr->NumOfSlaveChannel;

    /* Reset all of the channel */
    while (idx > 0u)
    {
        LinTp_SlaveResetRtData(tpSlaveRTPtr);

        tpSlaveRTPtr++;
        idx--;
    }
}

/**
 * LinTp slave node runtime data init and cancel TP process on going
 */
void LinTp_SlavePreInit(NetworkHandleType ch)
{
    NetworkHandleType       tpCh         = LinTp_SlaveGetLinTpChannel(LinIf_GetComMNetwork(ch));
    LinTp_SlaveRuntimeType* tpSlaveRTPtr = LinTp_GetSlaveRtDataPtr(tpCh);
    if (LINTP_CHANNEL_BUSY == tpSlaveRTPtr->channelState)
    {
        LinTp_SlaveAbortTxRxAndNotifyFailToUpper(tpSlaveRTPtr);
        LinTp_SlaveResetRtData(tpSlaveRTPtr);
    }
}

/**
 * TP message receive indication process in slave channel.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_LinTp_STCAL*/
void LinTp_SlaveMRFIndication(NetworkHandleType ch, const uint8* Lin_SduPtr)
/* PRQA S 6070 -- */
{
    uint8                   pciType = Lin_SduPtr[LINTP_PDU_OFS_PCI] & LINTP_PDU_PCI_MASK;
    const LinTp_RxNSduType* rxNSdu  = LinTp_GetRxNSduByNad(ch, Lin_SduPtr[LINTP_PDU_OFS_NAD]);

    if (NULL_PTR != rxNSdu)
    {
        NetworkHandleType commCh = rxNSdu->RxNSduChannelRef;
        NetworkHandleType tpCh   = LinTp_SlaveGetLinTpChannel(commCh);

        switch (pciType)
        {
        /* SF */
        case LINTP_PDU_PCI_SF:
            LinTp_SlaveSFRxHandle(tpCh, Lin_SduPtr, rxNSdu);
            break;

        /* FF */
        case LINTP_PDU_PCI_FF:
            LinTp_SlaveFFRxHandle(tpCh, Lin_SduPtr, rxNSdu);
            break;

        /* CF */
        case LINTP_PDU_PCI_CF:
            LinTp_SlaveCFRxHandle(tpCh, Lin_SduPtr);
            break;

        /* Unknow PCI */
        default:
            /*@req <SWS_LinIf_00697>*/
            /* Unknown PCI type is received, ignore this LIN frame*/
            break;
        }
    }
    else
    {
        NetworkHandleType       commCh       = LinIf_GetComMNetwork(ch);
        NetworkHandleType       tpCh         = LinTp_SlaveGetLinTpChannel(commCh);
        LinTp_SlaveRuntimeType* tpSlaveRTPtr = LinTp_GetSlaveRtDataPtr(tpCh);
        if ((LINTP_CHANNEL_BUSY == tpSlaveRTPtr->channelState)
            && (Lin_SduPtr[LINTP_PDU_OFS_NAD] != tpSlaveRTPtr->MRFRequestedNad))
        {
            if ((pciType == LINTP_PDU_PCI_SF) || (pciType == LINTP_PDU_PCI_FF))
            {
                LinTp_SlaveAbortTxRxAndNotifyFailToUpper(tpSlaveRTPtr);
            }
        }
    }
}

/**
 * Get ComMChannel corresponding LinTp channel
 */
Std_ReturnType LinTp_SlaveTransmit(const LinTp_TxNSduType* txNSdu, const PduInfoType* LinTpTxInfoPtr)
{
    NetworkHandleType       tpCh         = LinTp_SlaveGetLinTpChannel(txNSdu->TxNSduChannelRef);
    LinTp_SlaveRuntimeType* tpSlaveRTPtr = LinTp_GetSlaveRtDataPtr(tpCh);
    Std_ReturnType          ret          = E_NOT_OK;

    /*@req <SWS_LinIf_00321>*/
    if (LINTP_CHANNEL_IDLE == tpSlaveRTPtr->channelState)
    {
        tpSlaveRTPtr->TxNSduPtr    = txNSdu;
        tpSlaveRTPtr->SduRemaining = LinTpTxInfoPtr->SduLength;
        tpSlaveRTPtr->channelState = LINTP_CHANNEL_BUSY;
        if (LinTpTxInfoPtr->SduLength <= LINTP_SF_DATA_LEN_MAX)
        {
            tpSlaveRTPtr->TxStage = LINTP_SLAVE_TX_SF;
        }
        else
        {
            tpSlaveRTPtr->TxStage = LINTP_SLAVE_TX_FF;
        }
        /*@req <SWS_LinIf_00800>*/
        /* Start N_As timer */
        tpSlaveRTPtr->NasTimer = txNSdu->NasCnt;

        ret = E_OK;
    }

    return ret;
}

/**
 * Check slave node function address flag.
 */
boolean LinTp_SlaveCheckFunctionAddressFlag(NetworkHandleType ch)
{
    NetworkHandleType             tpCh         = LinTp_SlaveGetLinTpChannel(LinIf_GetComMNetwork(ch));
    const LinTp_SlaveRuntimeType* tpSlaveRTPtr = LinTp_GetSlaveRtDataPtr(tpCh);
    return tpSlaveRTPtr->FunctionAddressFlag;
}

/**
 * Set slave node function address flag.
 */
void LinTp_SlaveSetFunctionAddressFlag(NetworkHandleType ch, boolean functionAddressFlag)
{
    NetworkHandleType       tpCh         = LinTp_SlaveGetLinTpChannel(LinIf_GetComMNetwork(ch));
    LinTp_SlaveRuntimeType* tpSlaveRTPtr = LinTp_GetSlaveRtDataPtr(tpCh);
    tpSlaveRTPtr->FunctionAddressFlag    = functionAddressFlag;
}

void LinTp_SlaveShutdown(ApplicationType applicationId)
{
    LinIf_SlaveInit(applicationId);
}

/**
 * SRF process(Response transmit) in slave channel.
 */
void LinTp_SlaveSRFIndication(NetworkHandleType ch, Lin_PduType* PduPtr)
{
    NetworkHandleType       tpCh         = LinTp_SlaveGetLinTpChannel(LinIf_GetComMNetwork(ch));
    LinTp_SlaveRuntimeType* tpSlaveRTPtr = LinTp_GetSlaveRtDataPtr(tpCh);

    /*@req <SWS_LinIf_00803>*/
    /* Stop N_Cs timer */
    tpSlaveRTPtr->NcsTimer = 0u;

    switch (tpSlaveRTPtr->TxStage)
    {
    case LINTP_SLAVE_TX_SF:
        LinTp_SlaveSFTxHandle(ch, PduPtr, tpSlaveRTPtr);
        break;

    case LINTP_SLAVE_TX_FF:
        LinTp_SlaveFFTxHandle(ch, PduPtr, tpSlaveRTPtr);
        break;

    case LINTP_SLAVE_TX_CF:
        LinTp_SlaveCFTxHandle(ch, PduPtr, tpSlaveRTPtr);
        break;

    default:
        /* No data need to transmit */
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440)
        PduPtr->Drc = LIN_FRAMERESPONSE_IGNORE;
#else /* The default version of lin driver is 4.2.2 / 4.3.1 */
        PduPtr->Drc = LIN_SLAVE_TO_SLAVE;
#endif
        break;
    }
}

/**
 * TP tx confirmation process in slave channel.
 */
void LinTp_SlaveTxConfirmation(NetworkHandleType ch, boolean* isTpTxFinish)
{
    NetworkHandleType       tpCh         = LinTp_SlaveGetLinTpChannel(LinIf_GetComMNetwork(ch));
    LinTp_SlaveRuntimeType* tpSlaveRTPtr = LinTp_GetSlaveRtDataPtr(tpCh);

    if (LINTP_SLAVE_TX_WAIT_CONFIRM == tpSlaveRTPtr->TxStage)
    {
        /* Transmit finish */
        /*@req <SWS_LinIf_00068>*/
        /* Notify upper */
        PduR_LinTpTxConfirmation(tpSlaveRTPtr->TxNSduPtr->TxNSduPduRef, E_OK);

        /* Abort transmit */
        LinTp_SlaveResetRtData(tpSlaveRTPtr);
        /* TP tx finished */
        *isTpTxFinish = TRUE;
    }
    else
    {
        /* Transmit ongoing */
        /*@req <SWS_LinIf_00800>*/
        /* Stop N_As timer */
        tpSlaveRTPtr->NasTimer = 0u;

        /*@req <SWS_LinIf_00803>*/
        /* Start N_Cs timer */
        tpSlaveRTPtr->NcsTimer = tpSlaveRTPtr->TxNSduPtr->NcsCnt;
        /* TP tx not finished */
        *isTpTxFinish = FALSE;
    }
}

#if (LINIF_SUPPORT_RESPONSE_ERROR_SIGNAL == STD_ON)
void LinTp_SlaveTxErrorSignalHandler(NetworkHandleType LinIfChannelId)
{
    NetworkHandleType             linTpChId = LinTp_SlaveGetLinTpChannel(LinIf_GetComMNetwork(LinIfChannelId));
    const LinTp_SlaveRuntimeType* tpChPtr   = LinTp_GetSlaveRtDataPtr(linTpChId);

    if (tpChPtr != NULL_PTR && tpChPtr->TxNSduPtr != NULL_PTR)
    {
        PduR_LinTpTxConfirmation(tpChPtr->TxNSduPtr->TxNSduPduRef, E_NOT_OK);
    }
}
#endif

/**
 * LinTp slave main function
 */
void LinTp_SlaveMainFunction(NetworkHandleType ch)
{
    NetworkHandleType tpCh = LinTp_SlaveGetLinTpChannel(LinIf_GetComMNetwork(ch));

    if (tpCh < LinTp_ConfigPtr->NumOfChannel)
    {
        LinTp_SlaveRuntimeType* tpSlaveRTPtr = LinTp_GetSlaveRtDataPtr(tpCh);

        LinTp_SlaveTimerHandle(tpSlaveRTPtr);
        if (NULL_PTR != tpSlaveRTPtr->RxNSduPtr)
        {
            /* Rx processing */
            switch (tpSlaveRTPtr->RxStage)
            {
            case LINTP_SLAVE_RX_SF:
                LinTp_SlaveSFRxMainHandle(tpSlaveRTPtr);
                break;

            case LINTP_SLAVE_RX_FF:
                LinTp_SlaveFFRxMainHandle(tpSlaveRTPtr);
                break;

            case LINTP_SLAVE_RX_REQ_BUF:
            case LINTP_SLAVE_RX_CF:
                LinTp_SlaveCFRxMainHandle(tpSlaveRTPtr);
                break;

            default:
                /* Other Stage */
                break;
            }
        }
    }
}
/* PRQA S 1532 -- */

/* ========================================== internal function definition ========================================== */

/**
 * @brief               LinTp slave node receive Single frame handle
 * @param[in]           linifChannelId: Identification of the LIN channel
 * @param[in]           tpCh: Identification of the LinTp channel
 * @param[in]           Lin_SduPtr: pointer to a buffer where the current SDU is stored.
 * @param[in]           rxNSdu: pointer to lintp rxNSdu
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINTP_LOCAL void LinTp_SlaveSFRxHandle(NetworkHandleType tpCh, const uint8* Lin_SduPtr, const LinTp_RxNSduType* rxNSdu)
{
    LinTp_SlaveRuntimeType* tpSlaveRTPtr = LinTp_GetSlaveRtDataPtr(tpCh);
    uint16                  sduSize      = (uint16)Lin_SduPtr[LINTP_PDU_OFS_PCI] & LINTP_PDU_PCI_DL_MASK;
    boolean                 ignoreFlag   = FALSE;

    if (LINTP_CHANNEL_BUSY == tpSlaveRTPtr->channelState)
    {
        if (Lin_SduPtr[LINTP_PDU_OFS_NAD] != LINTP_FUNCTIONAL_REQ_NAD)
        {
            /* @req <SWS_LinIf_00651>,<SWS_LinIf_00653>,
               <SWS_LinIf_00808>,<SWS_LinIf_00797> */
            LinTp_SlaveAbortTxRxAndNotifyFailToUpper(tpSlaveRTPtr);
        }
        else
        {
            /* @req <SWS_LinIf_00807>,<SWS_LinIf_00798> */
            /* Physical request is ongoing, ignore the functional request.*/
            ignoreFlag = TRUE;
        }
    }

    /* @req <SWS_LinIf_00652> */
    /* Rx message error check*/
    if ((!ignoreFlag) && (sduSize > LINTP_LENGTH_SF_MIN) && (sduSize <= LINTP_LENGTH_SF_MAX))
    {
        /* Locked */
        SchM_Enter_LinTp_ExclusiveArea_Channel();

        tpSlaveRTPtr->RxNSduPtr    = rxNSdu;
        tpSlaveRTPtr->channelState = LINTP_CHANNEL_BUSY;
        tpSlaveRTPtr->SduSize      = sduSize;
        tpSlaveRTPtr->SduRemaining = sduSize;
        /* Data copy */
        (void)IStdLib_MemCpy(tpSlaveRTPtr->SduBuf, Lin_SduPtr, LINTP_FRAME_LEN_MAX);
        tpSlaveRTPtr->LastFrameType   = LINTP_FRAMETYPE_SF;
        tpSlaveRTPtr->ChCfgPtr        = LinTp_GetChannelConfig(tpCh);
        tpSlaveRTPtr->RxStage         = LINTP_SLAVE_RX_SF;
        tpSlaveRTPtr->SduSN           = 0u;
        tpSlaveRTPtr->MRFRequestedNad = Lin_SduPtr[LINTP_PDU_OFS_NAD];
        /* Reset functional addressing flag */
        if (tpSlaveRTPtr->FunctionAddressFlag)
        {
            tpSlaveRTPtr->FunctionAddressFlag = FALSE;
        }
        if (Lin_SduPtr[LINTP_PDU_OFS_NAD] == LINTP_FUNCTIONAL_REQ_NAD)
        {
            /* Functional addressing flag set */
            tpSlaveRTPtr->FunctionAddressFlag = TRUE;
        }

        /* Unlocked */
        SchM_Exit_LinTp_ExclusiveArea_Channel();
    }
}

/**
 * @brief               LinTp slave node receive First Frame handle
 * @param[in]           tpCh: Identification of the LinTp channel
 * @param[in]           Lin_SduPtr: pointer to a buffer where the current SDU is stored.
 * @param[in]           rxNSdu: pointer to lintp rxNSdu
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINTP_LOCAL void LinTp_SlaveFFRxHandle(NetworkHandleType tpCh, const uint8* Lin_SduPtr, const LinTp_RxNSduType* rxNSdu)
{
    LinTp_SlaveRuntimeType* tpSlaveRTPtr = LinTp_GetSlaveRtDataPtr(tpCh);
    boolean                 ignoreFlag   = FALSE;

    if (LINTP_CHANNEL_BUSY == tpSlaveRTPtr->channelState)
    {
        if (Lin_SduPtr[LINTP_PDU_OFS_NAD] != LINTP_FUNCTIONAL_REQ_NAD)
        {
            /* @req <SWS_LinIf_00651>,<SWS_LinIf_00653>,
               <SWS_LinIf_00808>,<SWS_LinIf_00797> */
            LinTp_SlaveAbortTxRxAndNotifyFailToUpper(tpSlaveRTPtr);
        }
        else
        {
            /* @req <SWS_LinIf_00807> */
            /* Physical request is ongoing, ignore the functional request.*/
            ignoreFlag = TRUE;
        }
    }

    /* @req <SWS_LinIf_00652> */
    /* Rx message error check*/
    uint16 sduSize = (uint16)Lin_SduPtr[LINTP_PDU_OFS_PCI] & LINTP_PDU_PCI_DL_MASK;
    sduSize        = sduSize << LINTP_PDU_DATESIZE_PCI_EXTERNINFO_OFFSET;
    sduSize        = sduSize | Lin_SduPtr[LINTP_PDU_OFS_LEN];
    if ((!ignoreFlag) && (sduSize >= LINTP_LENGTH_FF_MIN))
    {
        /* Locked */
        SchM_Enter_LinTp_ExclusiveArea_Channel();

        tpSlaveRTPtr->RxNSduPtr    = rxNSdu;
        tpSlaveRTPtr->channelState = LINTP_CHANNEL_BUSY;
        tpSlaveRTPtr->SduSize      = sduSize;
        tpSlaveRTPtr->SduRemaining = sduSize;
        /* Data copy */
        (void)IStdLib_MemCpy(tpSlaveRTPtr->SduBuf, Lin_SduPtr, LINTP_FRAME_LEN_MAX);
        tpSlaveRTPtr->LastFrameType   = LINTP_FRAMETYPE_FF;
        tpSlaveRTPtr->ChCfgPtr        = LinTp_GetChannelConfig(tpCh);
        tpSlaveRTPtr->RxStage         = LINTP_SLAVE_RX_FF;
        tpSlaveRTPtr->SduSN           = 0u;
        tpSlaveRTPtr->MRFRequestedNad = Lin_SduPtr[LINTP_PDU_OFS_NAD];

        /* @req <SWS_LinIf_00652> */
        /* Start the N_Cr timer */
        tpSlaveRTPtr->NcrTimer = tpSlaveRTPtr->RxNSduPtr->NcrCnt;

        /* Unlocked */
        SchM_Exit_LinTp_ExclusiveArea_Channel();
    }
}

/**
 * @brief               LinTp slave node receive Continue Frame handle
 * @param[in]           tpCh: Identification of the LinTp channel
 * @param[in]           Lin_SduPtr: pointer to a buffer where the current SDU is stored.
 * @param[in]           rxNSdu: pointer to lintp rxNSdu
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
/* PRQA S 6030 ++ */ /* VL_MTR_LinTp_STMIF */
LINTP_LOCAL void LinTp_SlaveCFRxHandle(NetworkHandleType tpCh, const uint8* Lin_SduPtr)
/* PRQA S 6030 -- */
{
    LinTp_SlaveRuntimeType* tpSlaveRTPtr = LinTp_GetSlaveRtDataPtr(tpCh);
    if (LINTP_CHANNEL_BUSY == tpSlaveRTPtr->channelState)
    {
        if (Lin_SduPtr[LINTP_PDU_OFS_NAD] != LINTP_FUNCTIONAL_REQ_NAD)
        {
            if (LINTP_SLAVE_RX_REQ_BUF == tpSlaveRTPtr->RxStage)
            {
                /* @req <SWS_LinIf_00795> */
                LinTp_SlaveAbortTxRxAndNotifyFailToUpper(tpSlaveRTPtr);
            }
            else if (LINTP_SLAVE_RX_WAIT_CF == tpSlaveRTPtr->RxStage)
            {
                /* Rx message error check*/
                tpSlaveRTPtr->SduSN++;
                tpSlaveRTPtr->SduSN = tpSlaveRTPtr->SduSN % LINTP_PDU_PCI_SN_MAX;
                if ((Lin_SduPtr[LINTP_PDU_OFS_PCI] & LINTP_PDU_PCI_SN_MASK) == tpSlaveRTPtr->SduSN)
                {
                    /* Data copy */
                    (void)IStdLib_MemCpy(tpSlaveRTPtr->SduBuf, Lin_SduPtr, LINTP_FRAME_LEN_MAX);
                    tpSlaveRTPtr->LastFrameType = LINTP_FRAMETYPE_CF;
                    tpSlaveRTPtr->RxStage       = LINTP_SLAVE_RX_CF;

                    /* @req <SWS_LinIf_00652> */
                    /* Restart the N_Cr timer */
                    tpSlaveRTPtr->NcrTimer = tpSlaveRTPtr->RxNSduPtr->NcrCnt;
                }
                else
                {
                    /* @req <SWS_LinIf_00079>,<SWS_LinIf_00081> */
                    LinTp_SlaveAbortTxRxAndNotifyFailToUpper(tpSlaveRTPtr);
                }
            }
            else
            {
                /* Wrong stage,ignore */
                /* @req <SWS_LinIf_00696> */
            }
        }
    }
    else
    {
        /* @req <SWS_LinIf_00696> */
        /* CF is received instead of a FF or a SF,Ignore this LIN frame. */
    }
}

/**
 * @brief               Single Frame receive process in mainfunction
 * @param[in]           tpSlaveRTPtr: Runtime data of lintp slave channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINTP_LOCAL void LinTp_SlaveSFRxMainHandle(LinTp_SlaveRuntimeType* tpSlaveRTPtr)
{
    uint8       data[LINTP_METADATA_BUFFER_LEN] = {0};
    PduInfoType pduInfo;
    pduInfo.MetaDataPtr           = data;
    pduInfo.SduLength             = 0u;
    const PduInfoType* pduInfoPtr = &pduInfo;

    /*@req <SWS_LinIf_00075>*/
    /* Notify PduR prepare to receive data */
    BufReq_ReturnType bufRslt = PduR_LinTpStartOfReception(
        tpSlaveRTPtr->RxNSduPtr->RxNSduPduRef,
        pduInfoPtr,
        tpSlaveRTPtr->SduSize,
        &(tpSlaveRTPtr->UpperBufRemaining));

    /* Buffer Request Success */
    if ((BUFREQ_OK == bufRslt) && (tpSlaveRTPtr->UpperBufRemaining >= tpSlaveRTPtr->SduSize))
    {
        /*@req <SWS_LinIf_00674> */
        /* Set destination buffer pointer and the number of bytes to be copied */
        pduInfo.SduDataPtr = &(tpSlaveRTPtr->SduBuf[LINTP_PDU_OFS_SF_DATA]);
        pduInfo.SduLength  = tpSlaveRTPtr->SduSize;
        /* Copy data to PduR */
        bufRslt =
            PduR_LinTpCopyRxData(tpSlaveRTPtr->RxNSduPtr->RxNSduPduRef, &pduInfo, &(tpSlaveRTPtr->UpperBufRemaining));

        PduIdType upLayerPduId = tpSlaveRTPtr->RxNSduPtr->RxNSduPduRef;
        /* @req <SWS_LinIf_00677>*/
        /* Abort the reception or receive finish,Reset runtime data */
        LinTp_SlaveResetRtData(tpSlaveRTPtr);
        if (BUFREQ_OK == bufRslt)
        {
            /*@req <SWS_LinIf_00078>*/
            /* Notify upper with E_OK */
            PduR_LinTpRxIndication(upLayerPduId, E_OK);
        }
        else
        {
            /*@req <SWS_LinIf_00677>*/
            /* Notify upper with E_NOT_OK */
            PduR_LinTpRxIndication(upLayerPduId, E_NOT_OK);
        }
    }
    else
    {
        /*@req <SWS_LinIf_00701>*/
        if (BUFREQ_OK == bufRslt)
        {
            /* Notify upper */
            PduR_LinTpRxIndication(tpSlaveRTPtr->RxNSduPtr->RxNSduPduRef, E_NOT_OK);
        }

        /* @req <SWS_LinIf_00676>,<SWS_LinIf_00701>*/
        /* Abort the reception */
        LinTp_SlaveResetRtData(tpSlaveRTPtr);
    }
}

/**
 * @brief               First Frame receive process in mainfunction
 * @param[in]           tpSlaveRTPtr: Runtime data of lintp slave channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINTP_LOCAL void LinTp_SlaveFFRxMainHandle(LinTp_SlaveRuntimeType* tpSlaveRTPtr)
{
    uint8       data[LINTP_METADATA_BUFFER_LEN] = {0};
    PduInfoType pduInfo;
    pduInfo.MetaDataPtr           = data;
    pduInfo.SduLength             = 0u;
    const PduInfoType* pduInfoPtr = &pduInfo;

    /*@req <SWS_LinIf_00075>*/
    /* Notify PduR prepare to receive data */
    BufReq_ReturnType bufRslt = PduR_LinTpStartOfReception(
        tpSlaveRTPtr->RxNSduPtr->RxNSduPduRef,
        pduInfoPtr,
        tpSlaveRTPtr->SduSize,
        &(tpSlaveRTPtr->UpperBufRemaining));

    /* Buffer Request Success */
    if ((BUFREQ_OK == bufRslt) && (tpSlaveRTPtr->UpperBufRemaining >= LINTP_FF_DATA_LEN))
    {
        /*@req <SWS_LinIf_00674> */
        /* Set destination buffer pointer and the number of bytes to be copied */
        pduInfo.SduDataPtr = &(tpSlaveRTPtr->SduBuf[LINTP_PDU_OFS_FF_DATA]);
        pduInfo.SduLength  = LINTP_FF_DATA_LEN;
        /* Copy data to PduR */
        bufRslt =
            PduR_LinTpCopyRxData(tpSlaveRTPtr->RxNSduPtr->RxNSduPduRef, &pduInfo, &(tpSlaveRTPtr->UpperBufRemaining));
        if (BUFREQ_OK == bufRslt)
        {
            tpSlaveRTPtr->SduRemaining -= pduInfo.SduLength;
            PduLengthType nextCFLen = (PduLengthType)LinIf_Min(tpSlaveRTPtr->SduRemaining, LINTP_CF_DATA_LEN_MAX);
            if (tpSlaveRTPtr->UpperBufRemaining >= nextCFLen)
            {
                tpSlaveRTPtr->RxStage = LINTP_SLAVE_RX_WAIT_CF;
            }
            else
            {
                /* @req <SWS_LinIf_00793>*/
                tpSlaveRTPtr->RxStage = LINTP_SLAVE_RX_REQ_BUF;
            }
        }
        else
        {
            /*@req <SWS_LinIf_00677>*/
            LinTp_SlaveAbortTxRxAndNotifyFailToUpper(tpSlaveRTPtr);
        }
    }
    else
    {
        /*@req <SWS_LinIf_00701>*/
        if ((BUFREQ_OK == bufRslt) && (tpSlaveRTPtr->UpperBufRemaining < tpSlaveRTPtr->SduSize))
        {
            /* Notify upper */
            PduR_LinTpRxIndication(tpSlaveRTPtr->RxNSduPtr->RxNSduPduRef, E_NOT_OK);
        }

        /* @req <SWS_LinIf_00676>,<SWS_LinIf_00701>*/
        /* Abort the reception */
        LinTp_SlaveResetRtData(tpSlaveRTPtr);
    }
}

/**
 * @brief               Consecutive Frame receive process in mainfunction
 * @param[in]           tpSlaveRTPtr: Runtime data of lintp slave channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINTP_LOCAL void LinTp_SlaveCFRxMainHandle(LinTp_SlaveRuntimeType* tpSlaveRTPtr)
{
    PduInfoType pduInfo;

    /*Set destination buffer pointer and the number of bytes to be copied */
    pduInfo.SduDataPtr = &(tpSlaveRTPtr->SduBuf[LINTP_PDU_OFS_CF_DATA]);
    if (LINTP_SLAVE_RX_REQ_BUF == tpSlaveRTPtr->RxStage)
    {
        /*@req <SWS_LinIf_00793> */
        pduInfo.SduLength = 0u;
    }
    else
    {
        pduInfo.SduLength = (PduLengthType)LinIf_Min(tpSlaveRTPtr->SduRemaining, LINTP_CF_DATA_LEN_MAX);
    }

    /*@req <SWS_LinIf_00674> */
    /* Copy data to PduR */
    BufReq_ReturnType bufRslt =
        PduR_LinTpCopyRxData(tpSlaveRTPtr->RxNSduPtr->RxNSduPduRef, &pduInfo, &(tpSlaveRTPtr->UpperBufRemaining));
    if (BUFREQ_OK == bufRslt)
    {
        tpSlaveRTPtr->SduRemaining -= pduInfo.SduLength;
        if (0u == tpSlaveRTPtr->SduRemaining)
        {
            /* Reception finish */
            PduIdType upLayerPduId = tpSlaveRTPtr->RxNSduPtr->RxNSduPduRef;
            /* Abort the reception */
            LinTp_SlaveResetRtData(tpSlaveRTPtr);
            /*@req <SWS_LinIf_00078>*/
            /* Notify upper with E_OK */
            PduR_LinTpRxIndication(upLayerPduId, E_OK);
        }
        else
        {
            PduLengthType nextCFLen = (PduLengthType)LinIf_Min(tpSlaveRTPtr->SduRemaining, LINTP_CF_DATA_LEN_MAX);
            if (tpSlaveRTPtr->UpperBufRemaining >= nextCFLen)
            {
                tpSlaveRTPtr->RxStage = LINTP_SLAVE_RX_WAIT_CF;
            }
            else
            {
                /* @req <SWS_LinIf_00793>*/
                tpSlaveRTPtr->RxStage = LINTP_SLAVE_RX_REQ_BUF;
            }
        }
    }
    else
    {
        /*@req <SWS_LinIf_00677>*/
        LinTp_SlaveAbortTxRxAndNotifyFailToUpper(tpSlaveRTPtr);
    }
}

/**
 * @brief               Single frame transmit process.
 * @param[in]           tpSlaveRTPtr: Runtime data of lintp slave channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINTP_LOCAL void LinTp_SlaveSFTxHandle(NetworkHandleType ch, Lin_PduType* PduPtr, LinTp_SlaveRuntimeType* tpSlaveRTPtr)
{
    if (tpSlaveRTPtr->RetryCopyCnt <= tpSlaveRTPtr->TxNSduPtr->MaxBufReq)
    {
        /* Set buffer value to padding data(0xFF) */
        (void)IStdLib_MemSet(tpSlaveRTPtr->SduBuf, LINTP_PADDING_VALUE, LINTP_FRAME_LEN_MAX);
        /* Set destination buffer pointer and he number of bytes to be copied */
        PduInfoType pduInfo;
        pduInfo.SduDataPtr = &(tpSlaveRTPtr->SduBuf[LINTP_PDU_OFS_SF_DATA]);
        pduInfo.SduLength  = tpSlaveRTPtr->SduRemaining;

        /*@req <SWS_LinIf_00671>,<SWS_LinIf_00705>*/
        /* Copy data from PduR */
        BufReq_ReturnType bufRslt = PduR_LinTpCopyTxData(
            tpSlaveRTPtr->TxNSduPtr->TxNSduPduRef,
            &pduInfo,
            NULL_PTR,
            &(tpSlaveRTPtr->UpperBufRemaining));

        switch (bufRslt)
        {
        case BUFREQ_OK:
            (void)LinIf_SlaveGetConfiguredNAD(ch, &tpSlaveRTPtr->SduBuf[LINTP_PDU_OFS_NAD]);
            tpSlaveRTPtr->SduBuf[LINTP_PDU_OFS_PCI] = (uint8)pduInfo.SduLength;
            /* Reset retry counter */
            tpSlaveRTPtr->RetryCopyCnt = 0u;
            tpSlaveRTPtr->TxStage      = LINTP_SLAVE_TX_WAIT_CONFIRM;

            if (tpSlaveRTPtr->FunctionAddressFlag)
            {
                /* Functional addressing with SF,no respond */
                /* Notify upper */
                PduR_LinTpTxConfirmation(tpSlaveRTPtr->TxNSduPtr->TxNSduPduRef, E_OK);
                /* Abort transmit */
                LinTp_SlaveResetRtData(tpSlaveRTPtr);
            }
            else
            {
                /* Copy data to Lin module provide buffer */
                (void)IStdLib_MemCpy(PduPtr->SduPtr, tpSlaveRTPtr->SduBuf, LINTP_FRAME_LEN_MAX);
            }
            break;

        case BUFREQ_E_NOT_OK:
            /*@req <SWS_LinIf_00073>*/
            LinTp_SlaveAbortTxRxAndNotifyFailToUpper(tpSlaveRTPtr);
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440)
            PduPtr->Drc = LIN_FRAMERESPONSE_IGNORE;
#else /* The default version of lin driver is 4.2.2 / 4.3.1 */
            PduPtr->Drc = LIN_SLAVE_TO_SLAVE;
#endif
            break;

        case BUFREQ_E_BUSY:
            /* retry to copy data */
            tpSlaveRTPtr->RetryCopyCnt++;
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440)
            PduPtr->Drc = LIN_FRAMERESPONSE_IGNORE;
#else /* The default version of lin driver is 4.2.2 / 4.3.1 */
            PduPtr->Drc = LIN_SLAVE_TO_SLAVE;
#endif
            break;

        default:
            /* Other Result */
            break;
        }
    }
    else
    {
        /*@req <SWS_LinIf_00330>*/
        LinTp_SlaveAbortTxRxAndNotifyFailToUpper(tpSlaveRTPtr);
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440)
        PduPtr->Drc = LIN_FRAMERESPONSE_IGNORE;
#else /* The default version of lin driver is 4.2.2 / 4.3.1 */
        PduPtr->Drc = LIN_SLAVE_TO_SLAVE;
#endif
    }
}

/**
* @brief               First frame transmit process.
* @param[in]           ch: Identification of the LinTp channel
* @param[inout]        PduPtr: Pointer to PDU providing the received PID and pointer to the SDU
                               data buffer as in parameter.
* @param[inout]        tpSlaveRTPtr: Runtime data of lintp slave channel
* @reentrant           TRUE
* @synchronous         TRUE
* @trace               -
*/
LINTP_LOCAL void LinTp_SlaveFFTxHandle(NetworkHandleType ch, Lin_PduType* PduPtr, LinTp_SlaveRuntimeType* tpSlaveRTPtr)
{
    if (tpSlaveRTPtr->RetryCopyCnt <= tpSlaveRTPtr->TxNSduPtr->MaxBufReq)
    {
        /* Set buffer value to padding data(0xFF) */
        (void)IStdLib_MemSet(tpSlaveRTPtr->SduBuf, LINTP_PADDING_VALUE, LINTP_FRAME_LEN_MAX);
        /* Set destination buffer pointer and he number of bytes to be copied */
        PduInfoType pduInfo;
        pduInfo.SduDataPtr = &(tpSlaveRTPtr->SduBuf[LINTP_PDU_OFS_FF_DATA]);
        pduInfo.SduLength  = LINTP_FF_DATA_LEN;

        /*@req <SWS_LinIf_00671>,<SWS_LinIf_00705>*/
        /* Copy data from PduR */
        BufReq_ReturnType bufRslt = PduR_LinTpCopyTxData(
            tpSlaveRTPtr->TxNSduPtr->TxNSduPduRef,
            &pduInfo,
            NULL_PTR,
            &(tpSlaveRTPtr->UpperBufRemaining));

        switch (bufRslt)
        {
        case BUFREQ_OK:
            (void)LinIf_SlaveGetConfiguredNAD(ch, &tpSlaveRTPtr->SduBuf[LINTP_PDU_OFS_NAD]);
            tpSlaveRTPtr->SduBuf[LINTP_PDU_OFS_PCI] =
                LINTP_PDU_PCI_FF | (uint8)(tpSlaveRTPtr->SduRemaining >> LINTP_PDU_DATESIZE_PCI_EXTERNINFO_OFFSET);
            tpSlaveRTPtr->SduBuf[LINTP_PDU_OFS_LEN] = (uint8)(tpSlaveRTPtr->SduRemaining);
            tpSlaveRTPtr->SduRemaining -= pduInfo.SduLength;
            /* Reset retry counter */
            tpSlaveRTPtr->RetryCopyCnt = 0u;
            tpSlaveRTPtr->TxStage      = LINTP_SLAVE_TX_CF;
            tpSlaveRTPtr->SduSN        = 0u;

            /* Copy data to Lin module provide buffer */
            (void)IStdLib_MemCpy(PduPtr->SduPtr, tpSlaveRTPtr->SduBuf, LINTP_FRAME_LEN_MAX);

            break;

        case BUFREQ_E_NOT_OK:
            /*@req <SWS_LinIf_00073>*/
            LinTp_SlaveAbortTxRxAndNotifyFailToUpper(tpSlaveRTPtr);
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440)
            PduPtr->Drc = LIN_FRAMERESPONSE_IGNORE;
#else /* The default version of lin driver is 4.2.2 / 4.3.1 */
            PduPtr->Drc = LIN_SLAVE_TO_SLAVE;
#endif
            break;

        case BUFREQ_E_BUSY:
            /* retry to copy data */
            tpSlaveRTPtr->RetryCopyCnt++;
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440)
            PduPtr->Drc = LIN_FRAMERESPONSE_IGNORE;
#else /* The default version of lin driver is 4.2.2 / 4.3.1 */
            PduPtr->Drc = LIN_SLAVE_TO_SLAVE;
#endif
            break;

        default:
            /* Other Result */
            break;
        }
    }
    else
    {
        /*@req <SWS_LinIf_00330>*/
        LinTp_SlaveAbortTxRxAndNotifyFailToUpper(tpSlaveRTPtr);
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440)
        PduPtr->Drc = LIN_FRAMERESPONSE_IGNORE;
#else /* The default version of lin driver is 4.2.2 / 4.3.1 */
        PduPtr->Drc = LIN_SLAVE_TO_SLAVE;
#endif
    }
}

/**
 * @brief               Consecutive Frame transmit process.
 * @param[in]           ch: Identification of the LinTp channel
 * @param[inout]        PduPtr: Pointer to PDU providing the received PID and pointer to the SDU
                                data buffer as in parameter.
 * @param[inout]        tpSlaveRTPtr: Runtime data of lintp slave channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINTP_LOCAL void LinTp_SlaveCFTxHandle(NetworkHandleType ch, Lin_PduType* PduPtr, LinTp_SlaveRuntimeType* tpSlaveRTPtr)
{
    if (NULL_PTR != tpSlaveRTPtr->TxNSduPtr)
    {
        if (0u == tpSlaveRTPtr->RetryCopyCnt)
        {
            /*@req <SWS_LinIf_00800>*/
            /* Start N_As timer */
            tpSlaveRTPtr->NasTimer = tpSlaveRTPtr->TxNSduPtr->NasCnt;
        }

        if (tpSlaveRTPtr->RetryCopyCnt <= tpSlaveRTPtr->TxNSduPtr->MaxBufReq)
        {
            /* Set buffer value to padding data(0xFF) */
            (void)IStdLib_MemSet(tpSlaveRTPtr->SduBuf, LINTP_PADDING_VALUE, LINTP_FRAME_LEN_MAX);
            PduInfoType pduInfo;
            /* Set destination buffer pointer and he number of bytes to be copied */
            pduInfo.SduDataPtr = &(tpSlaveRTPtr->SduBuf[LINTP_PDU_OFS_CF_DATA]);
            pduInfo.SduLength  = (PduLengthType)LinIf_Min(tpSlaveRTPtr->SduRemaining, LINTP_CF_DATA_LEN_MAX);
            /*@req <SWS_LinIf_00671>,<SWS_LinIf_00705>*/
            /* Copy data from PduR */
            BufReq_ReturnType bufRslt = PduR_LinTpCopyTxData(
                tpSlaveRTPtr->TxNSduPtr->TxNSduPduRef,
                &pduInfo,
                NULL_PTR,
                &(tpSlaveRTPtr->UpperBufRemaining));

            switch (bufRslt)
            {
            case BUFREQ_OK:
                (void)LinIf_SlaveGetConfiguredNAD(ch, &tpSlaveRTPtr->SduBuf[LINTP_PDU_OFS_NAD]);
                tpSlaveRTPtr->SduSN++;
                tpSlaveRTPtr->SduSN                     = tpSlaveRTPtr->SduSN % LINTP_PDU_PCI_SN_MAX;
                tpSlaveRTPtr->SduBuf[LINTP_PDU_OFS_PCI] = LINTP_PDU_PCI_CF | tpSlaveRTPtr->SduSN;
                /* Reset retry counter */
                tpSlaveRTPtr->RetryCopyCnt = 0u;
                tpSlaveRTPtr->SduRemaining -= pduInfo.SduLength;
                if (0u == tpSlaveRTPtr->SduRemaining)
                {
                    tpSlaveRTPtr->TxStage = LINTP_SLAVE_TX_WAIT_CONFIRM;
                }
                else
                {
                    tpSlaveRTPtr->TxStage = LINTP_SLAVE_TX_CF;
                }

                /* Copy data to Lin module provide buffer */
                (void)IStdLib_MemCpy(PduPtr->SduPtr, tpSlaveRTPtr->SduBuf, LINTP_FRAME_LEN_MAX);

                break;

            case BUFREQ_E_NOT_OK:
                /*@req <SWS_LinIf_00073>*/
                LinTp_SlaveAbortTxRxAndNotifyFailToUpper(tpSlaveRTPtr);
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440)
                PduPtr->Drc = LIN_FRAMERESPONSE_IGNORE;
#else /* The default version of lin driver is 4.2.2 / 4.3.1 */
                PduPtr->Drc = LIN_SLAVE_TO_SLAVE;
#endif
                break;

            case BUFREQ_E_BUSY:
                /* retry to copy data */
                tpSlaveRTPtr->RetryCopyCnt++;
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440)
                PduPtr->Drc = LIN_FRAMERESPONSE_IGNORE;
#else /* The default version of lin driver is 4.2.2 / 4.3.1 */
                PduPtr->Drc = LIN_SLAVE_TO_SLAVE;
#endif
                break;

            default:
                /* Other Result */
                break;
            }
        }
        else
        {
            /*@req <SWS_LinIf_00330>*/
            LinTp_SlaveAbortTxRxAndNotifyFailToUpper(tpSlaveRTPtr);
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440)
            PduPtr->Drc = LIN_FRAMERESPONSE_IGNORE;
#else /* The default version of lin driver is 4.2.2 / 4.3.1 */
            PduPtr->Drc = LIN_SLAVE_TO_SLAVE;
#endif
        }
    }
}

/**
 * @brief               Notify upper with E_NOT_OK and abort transmition and receive (reset runtime data)
 * @param[inout]        tpSlaveRTPtr: Runtime data of lintp slave channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINTP_LOCAL void LinTp_SlaveAbortTxRxAndNotifyFailToUpper(LinTp_SlaveRuntimeType* tpSlaveRTPtr)
{
    if (NULL_PTR != tpSlaveRTPtr->TxNSduPtr)
    {
        if (tpSlaveRTPtr->FunctionAddressFlag)
        {
            /* Function address notify upper with E_OK */
            PduR_LinTpTxConfirmation(tpSlaveRTPtr->TxNSduPtr->TxNSduPduRef, E_OK);
            tpSlaveRTPtr->FunctionAddressFlag = FALSE;
        }
        else
        {
            /* Notify upper with E_NOT_OK */
            PduR_LinTpTxConfirmation(tpSlaveRTPtr->TxNSduPtr->TxNSduPduRef, E_NOT_OK);
        }
    }
    if (NULL_PTR != tpSlaveRTPtr->RxNSduPtr)
    {
        /* Notify upper with E_NOT_OK */
        PduR_LinTpRxIndication(tpSlaveRTPtr->RxNSduPtr->RxNSduPduRef, E_NOT_OK);
    }

    /* Abort transmit */
    LinTp_SlaveResetRtData(tpSlaveRTPtr);
}

/**
 * @brief               Get ComMChannel corresponding LinTp channel
 * @param[in]           ComMChannel: Identification of the ComM channel
 * @return              tpCh: Identification of the LinTp channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINTP_LOCAL NetworkHandleType LinTp_SlaveGetLinTpChannel(NetworkHandleType ComMChannel)
{
    const LinTp_ChannelConfigType* tpChCfgPtr = LinTp_GetChannelConfig(0u);
    NetworkHandleType              tpCh;

    for (tpCh = 0u; tpCh < LinTp_ConfigPtr->NumOfChannel; tpCh++)
    {
        if (ComMChannel == tpChCfgPtr->ChannelRef)
        {
            break;
        }
        tpChCfgPtr++;
    }

    return tpCh;
}

/**
 * @brief               LinTp slave channel timer handler
 * @param[in]           tpSlaveRTPtr: Runtime data of lintp slave channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINTP_LOCAL void LinTp_SlaveTimerHandle(LinTp_SlaveRuntimeType* tpSlaveRTPtr)
{
    if (tpSlaveRTPtr->NcrTimer > 0u)
    {
        tpSlaveRTPtr->NcrTimer--;
        if (0u == tpSlaveRTPtr->NcrTimer)
        {
            /*@req <SWS_LinIf_00806>*/
            /* N_Cr timeout occurrence */
            LinTp_SlaveAbortTxRxAndNotifyFailToUpper(tpSlaveRTPtr);
        }
    }

    if (tpSlaveRTPtr->NasTimer > 0u)
    {
        tpSlaveRTPtr->NasTimer--;
        if (0u == tpSlaveRTPtr->NasTimer)
        {
            /*@req <SWS_LinIf_00801>*/
            /* N_As timeout occurrence */
            LinTp_SlaveAbortTxRxAndNotifyFailToUpper(tpSlaveRTPtr);
        }
    }

    if (tpSlaveRTPtr->NcsTimer > 0u)
    {
        tpSlaveRTPtr->NcsTimer--;
        if (0u == tpSlaveRTPtr->NcsTimer)
        {
            /*@req <SWS_LinIf_00801>*/
            /* N_Cs timeout occurrence */
            LinTp_SlaveAbortTxRxAndNotifyFailToUpper(tpSlaveRTPtr);
        }
    }
}

/**
 * @brief               Reset the channel runtime data to default value
 * @param[in]           tpSlaveRTPtr: Runtime data of lintp slave channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINTP_LOCAL void LinTp_SlaveResetRtData(LinTp_SlaveRuntimeType* tpSlaveRTPtr)
{
    tpSlaveRTPtr->ChCfgPtr            = NULL_PTR;
    tpSlaveRTPtr->TxNSduPtr           = NULL_PTR;
    tpSlaveRTPtr->RxNSduPtr           = NULL_PTR;
    tpSlaveRTPtr->SduSize             = 0u;
    tpSlaveRTPtr->SduIdx              = 0u;
    tpSlaveRTPtr->SduRemaining        = 0u;
    tpSlaveRTPtr->UpperBufRemaining   = 0u;
    tpSlaveRTPtr->channelState        = LINTP_CHANNEL_IDLE;
    tpSlaveRTPtr->NcrTimer            = 0u;
    tpSlaveRTPtr->NasTimer            = 0u;
    tpSlaveRTPtr->NcsTimer            = 0u;
    tpSlaveRTPtr->LastFrameType       = LINTP_FRAMETYPE_NONE;
    tpSlaveRTPtr->RxStage             = LINTP_SLAVE_RX_IDLE;
    tpSlaveRTPtr->TxStage             = LINTP_SLAVE_TX_IDLE;
    tpSlaveRTPtr->SduSN               = 0u;
    tpSlaveRTPtr->RetryCopyCnt        = 0u;
    tpSlaveRTPtr->FunctionAddressFlag = FALSE;
    tpSlaveRTPtr->MRFRequestedNad     = 0x0u;
    (void)IStdLib_MemSet(tpSlaveRTPtr->SduBuf, 0u, LINTP_FRAME_LEN_MAX);
}

#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

/* PRQA S 2814,2824,2812 -- */

#endif
#endif
