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
**  FILENAME    : LinTp_Slave.c                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : HuRongbo                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/

/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "LinIf_Cfg.h"
#if (LINIF_TP_SUPPORTED == STD_ON)
#include "LinTp_Cfg.h"

#if (LINTP_SLAVE_SUPPORT == STD_ON)
#include "LinTp_Slave.h"
#include "LinIf_Internal.h"
#include "PduR_LinTp.h"
#include "LinIf_Slave.h"

/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

static FUNC(void, LINIF_CODE) LinTp_SlaveSFRxHandle(
    NetworkHandleType tpCh,
    P2CONST(uint8, AUTOMATIC, LINIF_APPL_DATA) Lin_SduPtr, /* PRQA S 3432 */
    P2CONST(LinTp_RxNSduType, AUTOMATIC, LINIF_CONST) rxNSdu);

static FUNC(void, LINIF_CODE) LinTp_SlaveFFRxHandle(
    NetworkHandleType tpCh,
    P2CONST(uint8, AUTOMATIC, LINIF_APPL_DATA) Lin_SduPtr, /* PRQA S 3432 */
    P2CONST(LinTp_RxNSduType, AUTOMATIC, LINIF_CONST) rxNSdu);

static FUNC(void, LINIF_CODE) LinTp_SlaveCFRxHandle(
    NetworkHandleType tpCh,
    P2CONST(uint8, AUTOMATIC, LINIF_APPL_DATA) Lin_SduPtr /* PRQA S 3432 */
);

static FUNC(void, LINIF_CODE) LinTp_SlaveSFRxMainHandle(NetworkHandleType tpCh);

static FUNC(void, LINIF_CODE) LinTp_SlaveFFRxMainHandle(NetworkHandleType tpCh);

static FUNC(void, LINIF_CODE) LinTp_SlaveCFRxMainHandle(NetworkHandleType tpCh);

static FUNC(void, LINIF_CODE) LinTp_SlaveSFTxHandle(
    NetworkHandleType ch,
    P2VAR(Lin_PduType, AUTOMATIC, LINIF_APPL_DATA) PduPtr /* PRQA S 3432 */
);

static FUNC(void, LINIF_CODE) LinTp_SlaveFFTxHandle(
    NetworkHandleType ch,
    P2VAR(Lin_PduType, AUTOMATIC, LINIF_APPL_DATA) PduPtr /* PRQA S 3432 */
);

static FUNC(void, LINIF_CODE) LinTp_SlaveCFTxHandle(
    NetworkHandleType ch,
    P2VAR(Lin_PduType, AUTOMATIC, LINIF_APPL_DATA) PduPtr /* PRQA S 3432 */
);

/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
static FUNC(void, LINIF_CODE)
    LinTp_SlaveAbortTxRxAndNotifyFailToUpper(P2VAR(LinTp_SlaveRuntimeType, AUTOMATIC, LINIF_VAR) tpSlaveRTPtr);

static FUNC(NetworkHandleType, LINIF_CODE) LinTp_SlaveGetLinTpChannel(NetworkHandleType ComMChannel);

static FUNC(void, LINIF_CODE) LinTp_SlaveTimerHandle(P2VAR(LinTp_SlaveRuntimeType, AUTOMATIC, LINIF_VAR) tpSlaveRTPtr);

static FUNC(void, LINIF_CODE) LinTp_SlaveResetRtData(P2VAR(LinTp_SlaveRuntimeType, AUTOMATIC, LINIF_VAR) tpSlaveRTPtr);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
#if !defined(LINTP_LOCAL)
#define LINTP_LOCAL static
#endif

#define LINIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "LinIf_MemMap.h"

LINTP_LOCAL VAR(LinTp_SlaveRuntimeType, LINIF_VAR) LinTp_SlaveRTData[LINTP_SLAVE_CHANNEL_NUMBER];

#define LINIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "LinIf_MemMap.h"

/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
/******************************************************************************/
/*
 * Brief               LinTp slave node runtime data init
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE) LinTp_SlaveInit(void) /* PRQA S 1532 */
{
    P2VAR(LinTp_SlaveRuntimeType, AUTOMATIC, LINIF_VAR) /* PRQA S 3432 */
    tpSlaveRTPtr = &LinTp_SlaveRTData[0u];
    uint8 idx = LINTP_SLAVE_CHANNEL_NUMBER;

    /* Reset all of the channel */
    while (idx > 0u) /* PRQA S 2877 */ /* MISRA Rule 4.1 */
    {
        LinTp_SlaveResetRtData(tpSlaveRTPtr);

        tpSlaveRTPtr++; /* PRQA S 2983 */
        idx--;
    }
}
/******************************************************************************/
/*
 * Brief               LinTp slave node runtime data init and cancel TP process on going
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE) LinTp_SlavePreInit(NetworkHandleType ch)
{
    P2VAR(LinTp_SlaveRuntimeType, AUTOMATIC, LINIF_VAR) /* PRQA S 3432 */
    tpSlaveRTPtr = &LinTp_SlaveRTData[ch];
    if (LINTP_CHANNEL_BUSY == tpSlaveRTPtr->channelState)
    {
        LinTp_SlaveAbortTxRxAndNotifyFailToUpper(tpSlaveRTPtr);
        LinTp_SlaveResetRtData(tpSlaveRTPtr);
    }
}

/******************************************************************************/
/*
 * Brief               TP message receive indication process in slave channel.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 *                     Lin_SduPtr: pointer to a buffer where the current SDU is
 *                                 stored.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE)
LinTp_SlaveMRFIndication(/* PRQA S 1532 */
                         NetworkHandleType ch,
                         P2CONST(uint8, AUTOMATIC, LINIF_APPL_DATA) Lin_SduPtr /* PRQA S 3432 */
)
{
    uint8 pciType = Lin_SduPtr[LINTP_PDU_OFS_PCI] & LINTP_PDU_PCI_MASK;
    P2CONST(LinTp_RxNSduType, AUTOMATIC, LINIF_CONST) rxNSdu = LinTp_GetRxNSduByNad(ch, Lin_SduPtr[LINTP_PDU_OFS_NAD]);
    NetworkHandleType commCh;
    NetworkHandleType tpCh;

    if (NULL_PTR != rxNSdu)
    {
        commCh = rxNSdu->LinTpRxNSduChannelRef;
        tpCh = LinTp_SlaveGetLinTpChannel(commCh);

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
}

/******************************************************************************/
/*
 * Brief               Get ComMChannel corresponding LinTp channel
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ComMChannel: ComM Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              LinTp channel id
 */
/******************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE)
LinTp_SlaveTransmit(/* PRQA S 1532 */
                    P2CONST(LinTp_TxNSduType, AUTOMATIC, LINIF_APPL_CONST) txNSdu,
                    P2CONST(PduInfoType, AUTOMATIC, LINIF_APPL_CONST) LinTpTxInfoPtr)
{
    NetworkHandleType tpCh = LinTp_SlaveGetLinTpChannel(txNSdu->LinTpTxNSduChannelRef);
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(LinTp_SlaveRuntimeType, AUTOMATIC, LINIF_VAR)
    tpSlaveRTPtr = LINTP_GET_SLAVE_RTDATA_PTR(tpCh);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    Std_ReturnType ret = E_NOT_OK;

    /*@req <SWS_LinIf_00321>*/
    if (LINTP_CHANNEL_IDLE == tpSlaveRTPtr->channelState)
    {
        tpSlaveRTPtr->TxNSduPtr = txNSdu;
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
        tpSlaveRTPtr->NasTimer = txNSdu->LinTpNasCnt;

        ret = E_OK;
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               Check slave node function address flag.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Function address flag
 */
/******************************************************************************/
FUNC(boolean, LINIF_CODE)
LinTp_SlaveCheckFunctionAddressFlag(NetworkHandleType ch)
{
    NetworkHandleType tpCh = LinTp_SlaveGetLinTpChannel(LINIF_GET_COMM_NETWORK(ch));
    const LinTp_SlaveRuntimeType* tpSlaveRTPtr = LINTP_GET_SLAVE_RTDATA_PTR(tpCh);
    return tpSlaveRTPtr->FunctionAddressFlag;
}

/******************************************************************************/
/*
 * Brief               Set slave node function address flag.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 *                     functionAddressFlag
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE)
LinTp_SlaveSetFunctionAddressFlag(NetworkHandleType ch, boolean functionAddressFlag)
{
    NetworkHandleType tpCh = LinTp_SlaveGetLinTpChannel(LINIF_GET_COMM_NETWORK(ch));
    LinTp_SlaveRuntimeType* tpSlaveRTPtr = LINTP_GET_SLAVE_RTDATA_PTR(tpCh);
    tpSlaveRTPtr->FunctionAddressFlag = functionAddressFlag;
}

/******************************************************************************/
/*
 * Brief               SRF process(Response transmit) in slave channel.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  PduPtr: Lin_PduType buffer pointer provided by Lin.
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE)
LinTp_SlaveSRFIndication(/* PRQA S 1532 */
                         NetworkHandleType ch,
                         P2VAR(Lin_PduType, AUTOMATIC, LINIF_APPL_DATA) PduPtr /* PRQA S 3432 */
)
{
    NetworkHandleType tpCh = LinTp_SlaveGetLinTpChannel(LINIF_GET_COMM_NETWORK(ch));
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(LinTp_SlaveRuntimeType, AUTOMATIC, LINIF_VAR)
    tpSlaveRTPtr = LINTP_GET_SLAVE_RTDATA_PTR(tpCh);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

    /*@req <SWS_LinIf_00803>*/
    /* Stop N_Cs timer */
    tpSlaveRTPtr->NcsTimer = 0u;

    switch (tpSlaveRTPtr->TxStage)
    {
    case LINTP_SLAVE_TX_SF:
        LinTp_SlaveSFTxHandle(ch, PduPtr);
        break;

    case LINTP_SLAVE_TX_FF:
        LinTp_SlaveFFTxHandle(ch, PduPtr);
        break;

    case LINTP_SLAVE_TX_CF:
        LinTp_SlaveCFTxHandle(ch, PduPtr);
        break;

    default:
        /* No data need to transmit */
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440)
        PduPtr->Drc = LIN_FRAMERESPONSE_IGNORE;
#else  /* The default version of lin driver is 4.2.2 / 4.3.1 */
        PduPtr->Drc = LIN_SLAVE_TO_SLAVE;
#endif /* LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440 */
        break;
    }
}

/******************************************************************************/
/*
 * Brief               TP tx confirmation process in slave channel.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 * Param-Name[out]     isTpTxFinish: Indiacate if TP transmit finished
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE)
LinTp_SlaveTxConfirmation(/* PRQA S 1532 */
                          NetworkHandleType ch,
                          P2VAR(boolean, AUTOMATIC, LINIF_APPL_DATA) isTpTxFinish /* PRQA S 3432 */
)
{
    NetworkHandleType tpCh = LinTp_SlaveGetLinTpChannel(LINIF_GET_COMM_NETWORK(ch));
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(LinTp_SlaveRuntimeType, AUTOMATIC, LINIF_VAR)
    tpSlaveRTPtr = LINTP_GET_SLAVE_RTDATA_PTR(tpCh);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

    if (LINTP_SLAVE_TX_WAIT_CONFIRM == tpSlaveRTPtr->TxStage)
    {
        /* Transmit finish */
        /*@req <SWS_LinIf_00068>*/
        /* Notify upper */
        PduR_LinTpTxConfirmation(tpSlaveRTPtr->TxNSduPtr->LinTpTxNSduPduRef, E_OK);

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
        tpSlaveRTPtr->NcsTimer = tpSlaveRTPtr->TxNSduPtr->LinTpNcsCnt;
        /* TP tx not finished */
        *isTpTxFinish = FALSE;
    }
}

/******************************************************************************/
/*
 * Brief               LinTp slave main function
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
void LinTp_SlaveMainFunction(NetworkHandleType ch)
{
    NetworkHandleType tpCh = LinTp_SlaveGetLinTpChannel(LINIF_GET_COMM_NETWORK(ch));
    P2VAR(LinTp_SlaveRuntimeType, AUTOMATIC, LINIF_VAR) tpSlaveRTPtr; /* PRQA S 3432 */

    if (tpCh < LINTP_NUMBER_OF_CHANNELS)
    {
        tpSlaveRTPtr = LINTP_GET_SLAVE_RTDATA_PTR(tpCh);

        LinTp_SlaveTimerHandle(tpSlaveRTPtr);
        if (NULL_PTR != tpSlaveRTPtr->RxNSduPtr)
        {
            /* Rx processing */
            switch (tpSlaveRTPtr->RxStage)
            {
            case LINTP_SLAVE_RX_SF:
                LinTp_SlaveSFRxMainHandle(tpCh);
                break;

            case LINTP_SLAVE_RX_FF:
                LinTp_SlaveFFRxMainHandle(tpCh);
                break;

            case LINTP_SLAVE_RX_REQ_BUF:
            case LINTP_SLAVE_RX_CF:
                LinTp_SlaveCFRxMainHandle(tpCh);
                break;

            default:
                /* Other Stage */
                break;
            }
        }
    }
}
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
/******************************************************************************/
/*
 * Brief               LinTp slave node receive Single frame handle
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      tpCh: LinTp Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE) LinTp_SlaveSFRxHandle(
    NetworkHandleType tpCh,
    P2CONST(uint8, AUTOMATIC, LINIF_APPL_DATA) Lin_SduPtr, /* PRQA S 3432 */
    P2CONST(LinTp_RxNSduType, AUTOMATIC, LINIF_CONST) rxNSdu)
{
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(LinTp_SlaveRuntimeType, AUTOMATIC, LINIF_VAR)
    tpSlaveRTPtr = LINTP_GET_SLAVE_RTDATA_PTR(tpCh);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    uint16 sduSize = (uint16)Lin_SduPtr[LINTP_PDU_OFS_PCI] & LINTP_PDU_PCI_DL_MASK;
    boolean ignoreFlag = FALSE;

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
    if ((FALSE == ignoreFlag) && (sduSize > LINTP_LENGTH_SF_MIN) && (sduSize <= LINTP_LENGTH_SF_MAX))
    {
        /* Locked */
        SchM_Enter_LinTp_ExclusiveArea_Channel();

        tpSlaveRTPtr->RxNSduPtr = rxNSdu;
        tpSlaveRTPtr->channelState = LINTP_CHANNEL_BUSY;
        tpSlaveRTPtr->SduSize = sduSize;
        tpSlaveRTPtr->SduRemaining = sduSize;
        /* Data copy */
        (void)ILib_memcpy(tpSlaveRTPtr->SduBuf, Lin_SduPtr, LINTP_FRAME_LEN_MAX);
        tpSlaveRTPtr->LastFrameType = LINTP_FRAMETYPE_SF;
        tpSlaveRTPtr->ChCfgPtr = &LINTP_GET_CHANNEL_CONFIG(tpCh);
        tpSlaveRTPtr->RxStage = LINTP_SLAVE_RX_SF;
        tpSlaveRTPtr->SduSN = 0u;
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

/******************************************************************************/
/*
 * Brief               LinTp slave node receive First Frame handle
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      tpCh: LinTp Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE) LinTp_SlaveFFRxHandle(
    NetworkHandleType tpCh,
    P2CONST(uint8, AUTOMATIC, LINIF_APPL_DATA) Lin_SduPtr, /* PRQA S 3432 */
    P2CONST(LinTp_RxNSduType, AUTOMATIC, LINIF_CONST) rxNSdu)
{
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(LinTp_SlaveRuntimeType, AUTOMATIC, LINIF_VAR)
    tpSlaveRTPtr = LINTP_GET_SLAVE_RTDATA_PTR(tpCh);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    uint16 sduSize;
    boolean ignoreFlag = FALSE;

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
    sduSize = (uint16)Lin_SduPtr[LINTP_PDU_OFS_PCI] & LINTP_PDU_PCI_DL_MASK;
    sduSize = sduSize << 8u;
    sduSize = sduSize | Lin_SduPtr[LINTP_PDU_OFS_LEN];
    if ((FALSE == ignoreFlag) && (sduSize >= LINTP_LENGTH_FF_MIN))
    {
        /* Locked */
        SchM_Enter_LinTp_ExclusiveArea_Channel();

        tpSlaveRTPtr->RxNSduPtr = rxNSdu;
        tpSlaveRTPtr->channelState = LINTP_CHANNEL_BUSY;
        tpSlaveRTPtr->SduSize = sduSize;
        tpSlaveRTPtr->SduRemaining = sduSize;
        /* Data copy */
        (void)ILib_memcpy(tpSlaveRTPtr->SduBuf, Lin_SduPtr, LINTP_FRAME_LEN_MAX);
        tpSlaveRTPtr->LastFrameType = LINTP_FRAMETYPE_FF;
        tpSlaveRTPtr->ChCfgPtr = &LINTP_GET_CHANNEL_CONFIG(tpCh);
        tpSlaveRTPtr->RxStage = LINTP_SLAVE_RX_FF;
        tpSlaveRTPtr->SduSN = 0u;

        /* @req <SWS_LinIf_00652> */
        /* Start the N_Cr timer */
        tpSlaveRTPtr->NcrTimer = tpSlaveRTPtr->RxNSduPtr->LinTpNcrCnt;

        /* Unlocked */
        SchM_Exit_LinTp_ExclusiveArea_Channel();
    }
}

/******************************************************************************/
/*
 * Brief               LinTp slave node receive First Frame handle
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      tpCh: LinTp Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE) LinTp_SlaveCFRxHandle(
    NetworkHandleType tpCh,
    P2CONST(uint8, AUTOMATIC, LINIF_APPL_DATA) Lin_SduPtr /* PRQA S 3432 */
)
{
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(LinTp_SlaveRuntimeType, AUTOMATIC, LINIF_VAR)
    tpSlaveRTPtr = LINTP_GET_SLAVE_RTDATA_PTR(tpCh);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

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
                /* Locked */
                SchM_Enter_LinTp_ExclusiveArea_Channel();

                /* Rx message error check*/
                tpSlaveRTPtr->SduSN++;
                tpSlaveRTPtr->SduSN = tpSlaveRTPtr->SduSN % 0x10u;
                if ((Lin_SduPtr[LINTP_PDU_OFS_PCI] & LINTP_PDU_PCI_SN_MASK) == tpSlaveRTPtr->SduSN)
                {
                    /* Data copy */
                    (void)ILib_memcpy(tpSlaveRTPtr->SduBuf, Lin_SduPtr, LINTP_FRAME_LEN_MAX);
                    tpSlaveRTPtr->LastFrameType = LINTP_FRAMETYPE_CF;
                    tpSlaveRTPtr->RxStage = LINTP_SLAVE_RX_CF;

                    /* @req <SWS_LinIf_00652> */
                    /* Restart the N_Cr timer */
                    tpSlaveRTPtr->NcrTimer = tpSlaveRTPtr->RxNSduPtr->LinTpNcrCnt;
                }
                else
                {
                    /* @req <SWS_LinIf_00079>,<SWS_LinIf_00081> */
                    LinTp_SlaveAbortTxRxAndNotifyFailToUpper(tpSlaveRTPtr);
                }
                /* Unlocked */
                SchM_Exit_LinTp_ExclusiveArea_Channel();
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

/******************************************************************************/
/*
 * Brief               Single Frame receive process in mainfunction
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      tpCh: LinTp Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE) LinTp_SlaveSFRxMainHandle(NetworkHandleType tpCh)
{
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(LinTp_SlaveRuntimeType, AUTOMATIC, LINIF_VAR)
    tpSlaveRTPtr = LINTP_GET_SLAVE_RTDATA_PTR(tpCh);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    PduInfoType pduInfo;
    BufReq_ReturnType bufRslt;
    PduIdType upLayerPduId;
    const PduInfoType* pduInfoPtr = &pduInfo;
    uint8 data[5] = {0};

    pduInfo.MetaDataPtr = data;
    pduInfo.SduLength = 0;
    /*@req <SWS_LinIf_00075>*/
    /* Notify PduR prepare to receive data */
    bufRslt = PduR_LinTpStartOfReception(
        tpSlaveRTPtr->RxNSduPtr->LinTpRxNSduPduRef,
        pduInfoPtr,
        tpSlaveRTPtr->SduSize,
        &(tpSlaveRTPtr->UpperBufRemaining));

    /* Buffer Request Success */
    if ((BUFREQ_OK == bufRslt) && (tpSlaveRTPtr->UpperBufRemaining >= tpSlaveRTPtr->SduSize))
    {
        /*@req <SWS_LinIf_00674> */
        /* Set destination buffer pointer and the number of bytes to be copied */
        pduInfo.SduDataPtr = &(tpSlaveRTPtr->SduBuf[LINTP_PDU_OFS_SF_DATA]);
        pduInfo.SduLength = tpSlaveRTPtr->SduSize;
        /* Copy data to PduR */
        bufRslt = PduR_LinTpCopyRxData(
            tpSlaveRTPtr->RxNSduPtr->LinTpRxNSduPduRef,
            &pduInfo,
            &(tpSlaveRTPtr->UpperBufRemaining));

        upLayerPduId = tpSlaveRTPtr->RxNSduPtr->LinTpRxNSduPduRef;
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
        if ((BUFREQ_OK == bufRslt) && (tpSlaveRTPtr->UpperBufRemaining < tpSlaveRTPtr->SduSize))
        {
            /* Notify upper */
            PduR_LinTpRxIndication(tpSlaveRTPtr->RxNSduPtr->LinTpRxNSduPduRef, E_NOT_OK);
        }

        /* @req <SWS_LinIf_00676>,<SWS_LinIf_00701>*/
        /* Abort the reception */
        LinTp_SlaveResetRtData(tpSlaveRTPtr);
    }
}

/******************************************************************************/
/*
 * Brief               First Frame receive process in mainfunction
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      tpCh: LinTp Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE) LinTp_SlaveFFRxMainHandle(NetworkHandleType tpCh)
{
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(LinTp_SlaveRuntimeType, AUTOMATIC, LINIF_VAR)
    tpSlaveRTPtr = LINTP_GET_SLAVE_RTDATA_PTR(tpCh);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    PduInfoType pduInfo;
    BufReq_ReturnType bufRslt;
    PduLengthType nextCFLen;
    const PduInfoType* pduInfoPtr = &pduInfo;
    uint8 data[5] = {0};

    pduInfo.MetaDataPtr = data;
    pduInfo.SduLength = 0;
    /*@req <SWS_LinIf_00075>*/
    /* Notify PduR prepare to receive data */
    bufRslt = PduR_LinTpStartOfReception(
        tpSlaveRTPtr->RxNSduPtr->LinTpRxNSduPduRef,
        pduInfoPtr,
        tpSlaveRTPtr->SduSize,
        &(tpSlaveRTPtr->UpperBufRemaining));

    /* Buffer Request Success */
    if ((BUFREQ_OK == bufRslt) && (tpSlaveRTPtr->UpperBufRemaining >= LINTP_FF_DATA_LEN))
    {
        /*@req <SWS_LinIf_00674> */
        /* Set destination buffer pointer and the number of bytes to be copied */
        pduInfo.SduDataPtr = &(tpSlaveRTPtr->SduBuf[LINTP_PDU_OFS_FF_DATA]);
        pduInfo.SduLength = LINTP_FF_DATA_LEN;
        /* Copy data to PduR */
        bufRslt = PduR_LinTpCopyRxData(
            tpSlaveRTPtr->RxNSduPtr->LinTpRxNSduPduRef,
            &pduInfo,
            &(tpSlaveRTPtr->UpperBufRemaining));
        if (BUFREQ_OK == bufRslt)
        {
            tpSlaveRTPtr->SduRemaining -= pduInfo.SduLength;
            nextCFLen = LINIF_MIN(tpSlaveRTPtr->SduRemaining, LINTP_CF_DATA_LEN_MAX);
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
            PduR_LinTpRxIndication(tpSlaveRTPtr->RxNSduPtr->LinTpRxNSduPduRef, E_NOT_OK);
        }

        /* @req <SWS_LinIf_00676>,<SWS_LinIf_00701>*/
        /* Abort the reception */
        LinTp_SlaveResetRtData(tpSlaveRTPtr);
    }
}

/******************************************************************************/
/*
 * Brief               Consecutive Frame receive process in mainfunction
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      tpCh: LinTp Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE) LinTp_SlaveCFRxMainHandle(NetworkHandleType tpCh)
{
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(LinTp_SlaveRuntimeType, AUTOMATIC, LINIF_VAR)
    tpSlaveRTPtr = LINTP_GET_SLAVE_RTDATA_PTR(tpCh);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    PduInfoType pduInfo;
    BufReq_ReturnType bufRslt;
    PduLengthType nextCFLen;
    PduIdType upLayerPduId;

    /*Set destination buffer pointer and the number of bytes to be copied */
    pduInfo.SduDataPtr = &(tpSlaveRTPtr->SduBuf[LINTP_PDU_OFS_CF_DATA]);
    if (LINTP_SLAVE_RX_REQ_BUF == tpSlaveRTPtr->RxStage)
    {
        /*@req <SWS_LinIf_00793> */
        pduInfo.SduLength = 0u;
    }
    else
    {
        pduInfo.SduLength = LINIF_MIN(tpSlaveRTPtr->SduRemaining, LINTP_CF_DATA_LEN_MAX);
    }

    /*@req <SWS_LinIf_00674> */
    /* Copy data to PduR */
    bufRslt =
        PduR_LinTpCopyRxData(tpSlaveRTPtr->RxNSduPtr->LinTpRxNSduPduRef, &pduInfo, &(tpSlaveRTPtr->UpperBufRemaining));
    if (BUFREQ_OK == bufRslt)
    {
        tpSlaveRTPtr->SduRemaining -= pduInfo.SduLength;
        if (0u == tpSlaveRTPtr->SduRemaining)
        {
            /* Reception finish */

            upLayerPduId = tpSlaveRTPtr->RxNSduPtr->LinTpRxNSduPduRef;
            /* Abort the reception */
            LinTp_SlaveResetRtData(tpSlaveRTPtr);
            /*@req <SWS_LinIf_00078>*/
            /* Notify upper with E_OK */
            PduR_LinTpRxIndication(upLayerPduId, E_OK);
        }
        else
        {
            nextCFLen = LINIF_MIN(tpSlaveRTPtr->SduRemaining, LINTP_CF_DATA_LEN_MAX);
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

/******************************************************************************/
/*
 * Brief               Single frame transmit process.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      tpSlaveRTPtr: Lintp slave runtime data pointer
 * Param-Name[out]     None
 * Param-Name[in/out]  PduPtr: Lin_PduType buffer pointer provided by Lin.
 * Return              None
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE) LinTp_SlaveSFTxHandle(
    NetworkHandleType ch,
    P2VAR(Lin_PduType, AUTOMATIC, LINIF_APPL_DATA) PduPtr /* PRQA S 3432 */
)
{
    NetworkHandleType tpCh = LinTp_SlaveGetLinTpChannel(LINIF_GET_COMM_NETWORK(ch));
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(LinTp_SlaveRuntimeType, AUTOMATIC, LINIF_VAR)
    tpSlaveRTPtr = LINTP_GET_SLAVE_RTDATA_PTR(tpCh);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    PduInfoType pduInfo;
    BufReq_ReturnType bufRslt;

    if (tpSlaveRTPtr->RetryCopyCnt <= tpSlaveRTPtr->TxNSduPtr->LinTpMaxBufReq)
    {
        /* Set buffer value to padding data(0xFF) */
        (void)ILib_memset(tpSlaveRTPtr->SduBuf, LINTP_PADDING_VALUE, LINTP_FRAME_LEN_MAX);
        /* Set destination buffer pointer and he number of bytes to be copied */
        pduInfo.SduDataPtr = &(tpSlaveRTPtr->SduBuf[LINTP_PDU_OFS_SF_DATA]);
        pduInfo.SduLength = tpSlaveRTPtr->SduRemaining;
        /*@req <SWS_LinIf_00671>,<SWS_LinIf_00705>*/
        /* Copy data from PduR */
        bufRslt = PduR_LinTpCopyTxData(
            tpSlaveRTPtr->TxNSduPtr->LinTpTxNSduPduRef,
            &pduInfo,
            NULL_PTR,
            &(tpSlaveRTPtr->UpperBufRemaining));

        switch (bufRslt)
        {
        case BUFREQ_OK:
            (void)LinIf_SlaveGetConfiguredNAD(ch, &tpSlaveRTPtr->SduBuf[LINTP_PDU_OFS_NAD]);
            tpSlaveRTPtr->SduBuf[LINTP_PDU_OFS_PCI] = LINTP_PDU_PCI_SF | (uint8)pduInfo.SduLength; /* PRQA S 2986 */
            /* Reset retry counter */
            tpSlaveRTPtr->RetryCopyCnt = 0u;
            tpSlaveRTPtr->TxStage = LINTP_SLAVE_TX_WAIT_CONFIRM;

            if (tpSlaveRTPtr->FunctionAddressFlag)
            {
                /* Functional addressing with SF,no respond */
                /* Notify upper */
                PduR_LinTpTxConfirmation(tpSlaveRTPtr->TxNSduPtr->LinTpTxNSduPduRef, E_OK);
                /* Abort transmit */
                LinTp_SlaveResetRtData(tpSlaveRTPtr);
            }
            else
            {
                /* Copy data to Lin module provide buffer */
                (void)ILib_memcpy(PduPtr->SduPtr, tpSlaveRTPtr->SduBuf, LINTP_FRAME_LEN_MAX);
            }
            break;

        case BUFREQ_E_NOT_OK:
            /*@req <SWS_LinIf_00073>*/
            LinTp_SlaveAbortTxRxAndNotifyFailToUpper(tpSlaveRTPtr);
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440)
            PduPtr->Drc = LIN_FRAMERESPONSE_IGNORE;
#else  /* The default version of lin driver is 4.2.2 / 4.3.1 */
            PduPtr->Drc = LIN_SLAVE_TO_SLAVE;
#endif /* LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440 */
            break;

        case BUFREQ_E_BUSY:
            /* retry to copy data */
            tpSlaveRTPtr->RetryCopyCnt++;
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440)
            PduPtr->Drc = LIN_FRAMERESPONSE_IGNORE;
#else  /* The default version of lin driver is 4.2.2 / 4.3.1 */
            PduPtr->Drc = LIN_SLAVE_TO_SLAVE;
#endif /* LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440 */
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
#else  /* The default version of lin driver is 4.2.2 / 4.3.1 */
        PduPtr->Drc = LIN_SLAVE_TO_SLAVE;
#endif /* LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440 */
    }
}

/******************************************************************************/
/*
 * Brief               First frame transmit process.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      tpSlaveRTPtr: Lintp slave runtime data pointer
 * Param-Name[out]     None
 * Param-Name[in/out]  PduPtr: Lin_PduType buffer pointer provided by Lin.
 * Return              None
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE) LinTp_SlaveFFTxHandle(
    NetworkHandleType ch,
    P2VAR(Lin_PduType, AUTOMATIC, LINIF_APPL_DATA) PduPtr /* PRQA S 3432 */
)
{
    NetworkHandleType tpCh = LinTp_SlaveGetLinTpChannel(LINIF_GET_COMM_NETWORK(ch));
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(LinTp_SlaveRuntimeType, AUTOMATIC, LINIF_VAR)
    tpSlaveRTPtr = LINTP_GET_SLAVE_RTDATA_PTR(tpCh);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    PduInfoType pduInfo;
    BufReq_ReturnType bufRslt;

    if (tpSlaveRTPtr->RetryCopyCnt <= tpSlaveRTPtr->TxNSduPtr->LinTpMaxBufReq)
    {
        /* Set buffer value to padding data(0xFF) */
        (void)ILib_memset(tpSlaveRTPtr->SduBuf, LINTP_PADDING_VALUE, LINTP_FRAME_LEN_MAX);
        /* Set destination buffer pointer and he number of bytes to be copied */
        pduInfo.SduDataPtr = &(tpSlaveRTPtr->SduBuf[LINTP_PDU_OFS_FF_DATA]);
        pduInfo.SduLength = LINTP_FF_DATA_LEN;
        /*@req <SWS_LinIf_00671>,<SWS_LinIf_00705>*/
        /* Copy data from PduR */
        bufRslt = PduR_LinTpCopyTxData(
            tpSlaveRTPtr->TxNSduPtr->LinTpTxNSduPduRef,
            &pduInfo,
            NULL_PTR,
            &(tpSlaveRTPtr->UpperBufRemaining));

        switch (bufRslt)
        {
        case BUFREQ_OK:
            (void)LinIf_SlaveGetConfiguredNAD(ch, &tpSlaveRTPtr->SduBuf[LINTP_PDU_OFS_NAD]);
            tpSlaveRTPtr->SduBuf[LINTP_PDU_OFS_PCI] = LINTP_PDU_PCI_FF | (uint8)(tpSlaveRTPtr->SduRemaining >> 8u);
            tpSlaveRTPtr->SduBuf[LINTP_PDU_OFS_LEN] = (uint8)(tpSlaveRTPtr->SduRemaining);
            tpSlaveRTPtr->SduRemaining -= pduInfo.SduLength;
            /* Reset retry counter */
            tpSlaveRTPtr->RetryCopyCnt = 0u;
            tpSlaveRTPtr->TxStage = LINTP_SLAVE_TX_CF;
            tpSlaveRTPtr->SduSN = 0u;

            /* Copy data to Lin module provide buffer */
            (void)ILib_memcpy(PduPtr->SduPtr, tpSlaveRTPtr->SduBuf, LINTP_FRAME_LEN_MAX);

            break;

        case BUFREQ_E_NOT_OK:
            /*@req <SWS_LinIf_00073>*/
            LinTp_SlaveAbortTxRxAndNotifyFailToUpper(tpSlaveRTPtr);
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440)
            PduPtr->Drc = LIN_FRAMERESPONSE_IGNORE;
#else  /* The default version of lin driver is 4.2.2 / 4.3.1 */
            PduPtr->Drc = LIN_SLAVE_TO_SLAVE;
#endif /* LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440 */
            break;

        case BUFREQ_E_BUSY:
            /* retry to copy data */
            tpSlaveRTPtr->RetryCopyCnt++;
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440)
            PduPtr->Drc = LIN_FRAMERESPONSE_IGNORE;
#else  /* The default version of lin driver is 4.2.2 / 4.3.1 */
            PduPtr->Drc = LIN_SLAVE_TO_SLAVE;
#endif /* LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440 */
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
#else  /* The default version of lin driver is 4.2.2 / 4.3.1 */
        PduPtr->Drc = LIN_SLAVE_TO_SLAVE;
#endif /* LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440 */
    }
}

/******************************************************************************/
/*
 * Brief               Consecutive Frame transmit process.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      tpSlaveRTPtr: Lintp slave runtime data pointer
 * Param-Name[out]     None
 * Param-Name[in/out]  PduPtr: Lin_PduType buffer pointer provided by Lin.
 * Return              None
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE) LinTp_SlaveCFTxHandle(
    NetworkHandleType ch,
    P2VAR(Lin_PduType, AUTOMATIC, LINIF_APPL_DATA) PduPtr /* PRQA S 3432 */
)
{
    NetworkHandleType tpCh = LinTp_SlaveGetLinTpChannel(LINIF_GET_COMM_NETWORK(ch));
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(LinTp_SlaveRuntimeType, AUTOMATIC, LINIF_VAR)
    tpSlaveRTPtr = LINTP_GET_SLAVE_RTDATA_PTR(tpCh);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    PduInfoType pduInfo;
    BufReq_ReturnType bufRslt;

    if (NULL_PTR != tpSlaveRTPtr->TxNSduPtr)
    {
        if (0u == tpSlaveRTPtr->RetryCopyCnt)
        {
            /*@req <SWS_LinIf_00800>*/
            /* Start N_As timer */
            tpSlaveRTPtr->NasTimer = tpSlaveRTPtr->TxNSduPtr->LinTpNasCnt;
        }

        if (tpSlaveRTPtr->RetryCopyCnt <= tpSlaveRTPtr->TxNSduPtr->LinTpMaxBufReq)
        {
            /* Set buffer value to padding data(0xFF) */
            (void)ILib_memset(tpSlaveRTPtr->SduBuf, LINTP_PADDING_VALUE, LINTP_FRAME_LEN_MAX);
            /* Set destination buffer pointer and he number of bytes to be copied */
            pduInfo.SduDataPtr = &(tpSlaveRTPtr->SduBuf[LINTP_PDU_OFS_CF_DATA]);
            pduInfo.SduLength = LINIF_MIN(tpSlaveRTPtr->SduRemaining, LINTP_CF_DATA_LEN_MAX);
            /*@req <SWS_LinIf_00671>,<SWS_LinIf_00705>*/
            /* Copy data from PduR */
            bufRslt = PduR_LinTpCopyTxData(
                tpSlaveRTPtr->TxNSduPtr->LinTpTxNSduPduRef,
                &pduInfo,
                NULL_PTR,
                &(tpSlaveRTPtr->UpperBufRemaining));

            switch (bufRslt)
            {
            case BUFREQ_OK:
                (void)LinIf_SlaveGetConfiguredNAD(ch, &tpSlaveRTPtr->SduBuf[LINTP_PDU_OFS_NAD]);
                tpSlaveRTPtr->SduSN++;
                tpSlaveRTPtr->SduSN = tpSlaveRTPtr->SduSN % 16u;
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
                (void)ILib_memcpy(PduPtr->SduPtr, tpSlaveRTPtr->SduBuf, LINTP_FRAME_LEN_MAX);

                break;

            case BUFREQ_E_NOT_OK:
                /*@req <SWS_LinIf_00073>*/
                LinTp_SlaveAbortTxRxAndNotifyFailToUpper(tpSlaveRTPtr);
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440)
                PduPtr->Drc = LIN_FRAMERESPONSE_IGNORE;
#else  /* The default version of lin driver is 4.2.2 / 4.3.1 */
                PduPtr->Drc = LIN_SLAVE_TO_SLAVE;
#endif /* LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440 */
                break;

            case BUFREQ_E_BUSY:
                /* retry to copy data */
                tpSlaveRTPtr->RetryCopyCnt++;
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440)
                PduPtr->Drc = LIN_FRAMERESPONSE_IGNORE;
#else  /* The default version of lin driver is 4.2.2 / 4.3.1 */
                PduPtr->Drc = LIN_SLAVE_TO_SLAVE;
#endif /* LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440 */
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
#else  /* The default version of lin driver is 4.2.2 / 4.3.1 */
            PduPtr->Drc = LIN_SLAVE_TO_SLAVE;
#endif /* LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440 */
        }
    }
}

/******************************************************************************/
/*
 * Brief               Notify upper with E_NOT_OK and abort transmition and receive
 *                     (reset runtime data)
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      tpCh: LinTp Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
static FUNC(void, LINIF_CODE)
    LinTp_SlaveAbortTxRxAndNotifyFailToUpper(P2VAR(LinTp_SlaveRuntimeType, AUTOMATIC, LINIF_VAR) tpSlaveRTPtr)
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
{
    if (NULL_PTR != tpSlaveRTPtr->TxNSduPtr)
    {
        if (tpSlaveRTPtr->FunctionAddressFlag)
        {
            /* Function address notify upper with E_OK */
            PduR_LinTpTxConfirmation(tpSlaveRTPtr->TxNSduPtr->LinTpTxNSduPduRef, E_OK);
            tpSlaveRTPtr->FunctionAddressFlag = FALSE;
        }
        else
        {
            /* Notify upper with E_NOT_OK */
            PduR_LinTpTxConfirmation(tpSlaveRTPtr->TxNSduPtr->LinTpTxNSduPduRef, E_NOT_OK);
        }
    }
    if (NULL_PTR != tpSlaveRTPtr->RxNSduPtr)
    {
        /* Notify upper with E_NOT_OK */
        PduR_LinTpRxIndication(tpSlaveRTPtr->RxNSduPtr->LinTpRxNSduPduRef, E_NOT_OK);
    }

    /* Abort transmit */
    LinTp_SlaveResetRtData(tpSlaveRTPtr);
}

/******************************************************************************/
/*
 * Brief               Get ComMChannel corresponding LinTp channel
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ComMChannel: ComM Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              LinTp channel id
 */
/******************************************************************************/
static FUNC(NetworkHandleType, LINIF_CODE) LinTp_SlaveGetLinTpChannel(NetworkHandleType ComMChannel)
{
    P2CONST(LinTp_ChannelConfigType, AUTOMATIC, LINIF_CONST) tpChCfgPtr = &LINTP_GET_CHANNEL_CONFIG(0u);
    NetworkHandleType tpCh;

    for (tpCh = 0u; tpCh < LINTP_NUMBER_OF_CHANNELS; tpCh++)
    {
        if (ComMChannel == tpChCfgPtr->LinTpChannelRef)
        {
            break;
        }
        tpChCfgPtr++;
    }

    return tpCh;
}

/******************************************************************************/
/*
 * Brief               Get ComMChannel corresponding LinTp channel
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      tpSlaveRTPtr: Pointer of LinTp slave runtime data
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              LinTp channel id
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE)
    LinTp_SlaveTimerHandle(P2VAR(LinTp_SlaveRuntimeType, AUTOMATIC, LINIF_VAR) tpSlaveRTPtr /* PRQA S 3432 */
    )
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

/******************************************************************************/
/*
 * Brief               Reset the channel runtime data to default value
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      tpSlaveRTPtr: Pointer of LinTp slave runtime data
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE)
    LinTp_SlaveResetRtData(P2VAR(LinTp_SlaveRuntimeType, AUTOMATIC, LINIF_VAR) tpSlaveRTPtr /* PRQA S 3432 */
    )
{
    tpSlaveRTPtr->ChCfgPtr = NULL_PTR;
    tpSlaveRTPtr->TxNSduPtr = NULL_PTR;
    tpSlaveRTPtr->RxNSduPtr = NULL_PTR;
    tpSlaveRTPtr->SduSize = 0u;
    tpSlaveRTPtr->SduIdx = 0u;
    tpSlaveRTPtr->SduRemaining = 0u;
    tpSlaveRTPtr->UpperBufRemaining = 0u;
    tpSlaveRTPtr->channelState = LINTP_CHANNEL_IDLE;
    tpSlaveRTPtr->NcrTimer = 0u;
    tpSlaveRTPtr->NasTimer = 0u;
    tpSlaveRTPtr->NcsTimer = 0u;
    tpSlaveRTPtr->LastFrameType = LINTP_FRAMETYPE_NONE;
    tpSlaveRTPtr->RxStage = LINTP_SLAVE_RX_IDLE;
    tpSlaveRTPtr->TxStage = LINTP_SLAVE_TX_IDLE;
    tpSlaveRTPtr->SduSN = 0u;
    tpSlaveRTPtr->RetryCopyCnt = 0u;
    (void)ILib_memset(tpSlaveRTPtr->SduBuf, 0, LINTP_FRAME_LEN_MAX);
}

#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#endif /* LINTP_SLAVE_SUPPORT == STD_ON */
#endif /* LINIF_TP_SUPPORTED == STD_ON */
