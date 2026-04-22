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
 **  @file               : LinTp_Master.c
 **  @author             : HuRongbo
 **  @date               :
 **  @vendor             : isoft
 **  @description        : LinTp master implementation header file
 **
 ***********************************************************************************************************************/

/* ================================================= version define ================================================= */

/* =================================================== inclusions =================================================== */
#include "LinIf_Cfg.h"
#if (LINIF_TP_SUPPORTED == STD_ON)
#include "LinTp_Cfg.h"
#if (LINTP_MASTER_SUPPORT == STD_ON)
#include "LinTp_Master.h"
#include "LinIf_Master.h"
#include "LinTp_Internal.h"
#include "LinIf_Internal.h"
#include "PduR_LinTp.h"
#include "BswM_LinTp.h"
/* PRQA S 0553 EOF */ /* VL_QAC_UnUsedFiles */
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ============================================ internal data definition ============================================ */

/* ========================================= internal function declarations ========================================= */
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

LINTP_LOCAL void LinTp_MasterRxHandler(
    uint8                          pciType,
    const uint8*                   sdu,
    LinTp_MasterRuntimeType*       tpChPtr,
    const LinTp_ChannelConfigType* chCfgPtr,
    const LinTp_RxNSduType*        rx);

LINTP_LOCAL void LinTp_MasterCancelRxHandler(
    LinTp_MasterRuntimeType*       tpChPtr,
    const LinTp_ChannelConfigType* chCfgPtr,
    const LinTp_RxNSduType*        rx);

LINTP_LOCAL void LinTp_HandleCopyTxDataFailure(LinTp_MasterRuntimeType* tpChPtr);

LINTP_LOCAL boolean LinTp_CopyTxDataFromPduR(LinTp_MasterRuntimeType* tpChPtr);

LINTP_LOCAL boolean LinTp_TxEventRequest(LinTp_MasterRuntimeType* tpChPtr);

LINTP_LOCAL void LinTp_TxEventHandler(LinTp_MasterRuntimeType* tpChPtr);

LINTP_LOCAL void LinTp_TxEventConfirmation(LinTp_MasterRuntimeType* tpChPtr);

LINTP_LOCAL boolean LinTp_IsRxSF(
    LinTp_MasterRuntimeType* tpChPtr,
    const LinTp_RxNSduType*  rx,
    const uint8*             sdu,
    boolean*                 isIgnoreFramePtr);

LINTP_LOCAL boolean LinTp_IsRxFF(
    LinTp_MasterRuntimeType* tpChPtr,
    const LinTp_RxNSduType*  rx,
    const uint8*             sdu,
    boolean*                 isIgnoreFramePtr);

LINTP_LOCAL boolean LinTp_IsRxCF(LinTp_MasterRuntimeType* tpChPtr, const uint8* sdu);

LINTP_LOCAL const LinTp_ChannelConfigType* LinTp_GetChannelRefConfig(uint8 ch);

LINTP_LOCAL void LinTp_RxBufferRequestStart(LinTp_MasterRuntimeType* tpChPtr);

LINTP_LOCAL void LinTp_RxBufferRequest(LinTp_MasterRuntimeType* tpChPtr);

LINTP_LOCAL void LinTp_RxEventRequest(LinTp_MasterRuntimeType* tpChPtr);

LINTP_LOCAL void LinTp_RxEventHandler(LinTp_MasterRuntimeType* tpChPtr);

LINTP_LOCAL void LinTp_RxEventIndication(LinTp_MasterRuntimeType* tpChPtr);

#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"
/* ============================================ internal data definition ============================================ */

/* ========================================== external function definition ========================================== */

/* PRQA S 2814,2824,2889 ++ */ /* VL_QAC_DerefNullPtr */

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
/**
 * LinTp master node runtime data init
 */
void LinTp_MasterInit(void)
{
    uint8                    idx     = LinTp_ConfigPtr->NumOfMasterChannel;
    LinTp_MasterRuntimeType* tpChPtr = LinTp_MasterRtDataPtr(0u);

    /* Reset all of the channel */
    while (idx > 0u)
    {
        /*@req <SWS_LinIf_00320>,<SWS_LinIf_00710> */
        LinTp_MasterChReset(tpChPtr);
        tpChPtr->TrsEvent          = LINTP_TRS_EVT_NONE;
        tpChPtr->RecoverMode       = LINTP_APPLICATIVE_SCHEDULE;
        tpChPtr->NeedRestoreScence = FALSE;
        tpChPtr++;

        idx--;
    }
}

/**
 * LinTp master node channel runtime data init
 */
void LinTp_MasterChannelInit(NetworkHandleType LinIfChannelId)
{
    NetworkHandleType        linTpChId = LinTp_GetLinTpChannel(LinIfChannelId);
    LinTp_MasterRuntimeType* tpChPtr   = LinTp_MasterRtDataPtr(0u);

    LinTp_MasterChReset(tpChPtr);
}

/**
 * Requests the transfer of segmented data.
 */
Std_ReturnType LinTp_MasterTransmit(const LinTp_TxNSduType* txNSdu, const PduInfoType* LinTpTxInfoPtr)
{
    NetworkHandleType        linIfChId = txNSdu->LinIfChannelRef;
    NetworkHandleType        linTpChId = LinTp_GetLinTpChannel(linIfChId);
    LinTp_MasterRuntimeType* tpChPtr   = LinTp_MasterRtDataPtr(linTpChId);
    Std_ReturnType           ret;

    /* Handle Functional Request*/
    if (LINTP_FUNCTIONAL_REQ_NAD == txNSdu->TxNSduNad)
    {
        if (LinTp_IsTrsEvent(tpChPtr, LINTP_TRS_EVT_FUN_TX_REQ | LINTP_TRS_EVT_FUN_TX))
        {
            ret = E_NOT_OK;
        }
        else
        {
            /*@req <SWS_LinIf_00413>*/
            /* Set the status of channel */
            tpChPtr->ChannelState = LINTP_CHANNEL_BUSY;
            /* Save transmit request information */
            tpChPtr->FuncReqNSduPtr = txNSdu;
            tpChPtr->FuncReqSduSize = LinTpTxInfoPtr->SduLength;

            LinTp_SetTrsEvent(tpChPtr, LINTP_TRS_EVT_FUN_TX_REQ);

            ret = E_OK;
        }
    }
    /* Handle Physic Request*/
    else
    {
        /*@req <SWS_LinIf_00413>*/
        /* Set the status of channel */
        tpChPtr->ChannelState = LINTP_CHANNEL_BUSY;
        /* Save transmit request information */
        tpChPtr->PhyReqNSduPtr = txNSdu;
        tpChPtr->PhyReqSduSize = LinTpTxInfoPtr->SduLength;

        LinTp_SetTrsEvent(tpChPtr, LINTP_TRS_EVT_PHY_TX_REQ);

        ret = E_OK;
    }

    return ret;
}

/**
 * Shutdowns the LINTP master node
 */
void LinTp_MasterShutdown(void)
{
    uint8                    idx     = LinTp_ConfigPtr->NumOfMasterChannel;
    LinTp_MasterRuntimeType* tpChPtr = LinTp_MasterRtDataPtr(0u);

    /*@req <SWS_LinIf_00433> */
    /* Reset all of the channel */
    while (idx > 0u)
    {
        /*@req <SWS_LinIf_00356>*/
        LinTp_MasterChReset(tpChPtr);
        tpChPtr->TrsEvent = LINTP_TRS_EVT_NONE;
        /*@req <SWS_LinIf_00484>*/
        tpChPtr->ChannelState = LINTP_UNINIT;
        tpChPtr++;

        idx--;
    }
}

/**
 * Tx success process in master node.
 */
void LinTp_MasterTxSuccessHandle(NetworkHandleType LinIfChannelId)
{
    NetworkHandleType        linTpChId = LinTp_GetLinTpChannel(LinIfChannelId);
    LinTp_MasterRuntimeType* tpChPtr   = LinTp_MasterRtDataPtr(linTpChId);

    /* Check whether clear confirmation event */
    if (LinTp_IsEvent(tpChPtr, LINTP_EVENT_CONF))
    {
        LinTp_ClrEvent(tpChPtr, LINTP_EVENT_CONF);
        LinTp_SetEvent(tpChPtr, LINTP_EVENT_OK);
    }
    else
    {
        /*@req <SWS_LinIf_00657>,<SWS_LinIf_00661>*/
        /* Except last CF, need to start timer N_Cs and
            stop N_As */
        tpChPtr->TpTimer.EnabledTimer = LINTP_TIMER_NCS;
        tpChPtr->TpTimer.Timer        = tpChPtr->TxNSduPtr->NcsCnt;
    }

    if (LinTp_IsEvent(tpChPtr, LINTP_EVENT_WAIT))
    {
        LinTp_ClrEvent(tpChPtr, LINTP_EVENT_WAIT);
    }
}

/**
 * Tx error process in master node.
 */
void LinTp_MasterTxErrorHandle(NetworkHandleType LinIfChannelId, Lin_StatusType st)
{
    NetworkHandleType        linTpChId = LinTp_GetLinTpChannel(LinIfChannelId);
    LinTp_MasterRuntimeType* tpChPtr   = LinTp_MasterRtDataPtr(linTpChId);

    if ((LinTp_IsEvent(tpChPtr, LINTP_EVENT_TX)) && ((st == LIN_TX_ERROR) || (st == LIN_TX_HEADER_ERROR)))
    {
        /*@req <SWS_LinIf_00069> */
        /* Notify upper(PDUR) */
        PduR_LinTpTxConfirmation(tpChPtr->TxNSduPtr->TxNSduPduRef, E_NOT_OK);
#if (STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT)
        if (tpChPtr->ChCfgPtr->ScheduleChangeDiag)
        {
            /*@req <SWS_LinIf_00673> */
            /* Change to the applicative schedule */
            BswM_LinTp_RequestMode(tpChPtr->TxNSduPtr->TxNSduChannelRef, LINTP_APPLICATIVE_SCHEDULE);
            tpChPtr->RecoverMode = LINTP_APPLICATIVE_SCHEDULE;
        }
#endif
        /* Reset channel */
        LinTp_MasterChReset(tpChPtr);

        /* Next entry */
        LinIf_MoveScheduleToNextEntry(LinIfChannelId);
    }
    else
    {
        LinTp_SetEvent(tpChPtr, LINTP_EVENT_WAIT);
    }
}

/**
 * Parses a event of the receiving
 */
void LinTp_RxEventParse(uint8 LinIfChannelId, const uint8* sdu)
{
    NetworkHandleType              linTpChId = LinTp_GetLinTpChannel(LinIfChannelId);
    LinTp_MasterRuntimeType*       tpChPtr   = LinTp_MasterRtDataPtr(linTpChId);
    const LinTp_ChannelConfigType* chCfgPtr  = LinTp_GetChannelConfig(linTpChId);
    uint8                          pciType   = sdu[LINTP_PDU_OFS_PCI] & LINTP_PDU_PCI_MASK;

    if ((NULL_PTR == tpChPtr) || (NULL_PTR == chCfgPtr))
    {
        return;
    }

    const LinTp_RxNSduType* rx = LinTp_GetRxNSduByNad(LinIfChannelId, tpChPtr->MRFRequestedNad);
    if (NULL_PTR == rx)
    {
        return;
    }

    LinTp_ClrEvent(tpChPtr, LINTP_EVENT_WAIT);
    if ((LINTP_PDU_PCI_SF == pciType) && (LINTP_NEGATIVE_RSID == sdu[LINTP_PDU_OFS_SF_RSID])
        && (tpChPtr->MRFRequestedSID == sdu[LINTP_PDU_OFS_SF_NR_SID])
        && (LINTP_NRC_RESPONSE_PENDING == sdu[LINTP_PDU_OFS_SF_NR_NRC]))
    {
        tpChPtr->PendingFrameNum++;
        /*@req <SWS_LinIf_00623>*/
        if (tpChPtr->PendingFrameNum > chCfgPtr->MaxNumberOfRespPendingFrames)
        {
            LinTp_MasterCancelRxHandler(tpChPtr, chCfgPtr, rx);
        }
        else
        {
            /*@req <SWS_LinIf_00621>*/
            /* Reload P2 timer with the time P2*max */
            tpChPtr->TpP2Timer.EnabledTimer = LINTP_TIMER_P2MAX;
            tpChPtr->TpP2Timer.Timer        = chCfgPtr->P2MaxCnt;
        }
        return;
    }

    /*@req <SWS_LinIf_00612>,<SWS_LinIf_00613>*/
    if ((chCfgPtr->DropNotRequestedNad) && (sdu[LINTP_PDU_OFS_NAD] != tpChPtr->MRFRequestedNad))
    {
        LinTp_MasterCancelRxHandler(tpChPtr, chCfgPtr, rx);
    }
    else if (!(LinTp_IsEvent(tpChPtr, LINTP_EVENT_TX)) && (tpChPtr->ChannelState != LINTP_UNINIT))
    {
        LinTp_MasterRxHandler(pciType, sdu, tpChPtr, chCfgPtr, rx);
    }
}

/**
 * LinIf get MRF response from LinTp
 */
Std_ReturnType LinTp_MasterGetMRFResponse(NetworkHandleType LinIfChannelId, uint8* sduBufPtr)
{
    NetworkHandleType        linTpChId = LinTp_GetLinTpChannel(LinIfChannelId);
    LinTp_MasterRuntimeType* tpChPtr   = LinTp_MasterRtDataPtr(linTpChId);
    Std_ReturnType           ret       = E_NOT_OK;

    if (!LinTp_TxEventRequest(tpChPtr))
    {
        return ret;
    }
    LinTp_TxEventHandler(tpChPtr);

    if ((LinTp_IsEvent(tpChPtr, LINTP_EVENT_TX_REQ)) && !(LinTp_IsEvent(tpChPtr, LINTP_EVENT_STOP_MRF)))
    {
        /* Set the flag of header and response */
        LinIf_MasterRuntimeType* chPtr = LinIf_GetMasterRtDataPtr(LinIfChannelId);
        LinIf_SetEvent(chPtr, LINIF_EVENT_HEADER | LINIF_EVENT_RESPONSE);

        /* Clear TX request events */
        LinTp_ClrEvent(tpChPtr, LINTP_EVENT_TX_REQ);

        /* Check whether set confirmation event */
        if (LinTp_IsEvent(tpChPtr, LINTP_EVENT_CONF_REQ))
        {
            LinTp_ClrEvent(tpChPtr, LINTP_EVENT_CONF_REQ);
            LinTp_SetEvent(tpChPtr, LINTP_EVENT_CONF);

            /*@req <SWS_LinIf_00617>,<SWS_LinIf_00618>*/
            /* Send last MRF,start P2 Timer */
            /* But If current MRF is functional request,Need't start
               P2(functional request no response) */
            if (LINTP_FUNCTIONAL_REQ_NAD != tpChPtr->SduBuf[LINTP_PDU_OFS_NAD])
            {
                tpChPtr->TpP2Timer.EnabledTimer = LINTP_TIMER_P2;
                tpChPtr->TpP2Timer.Timer        = LinTp_GetP2Timer(linTpChId);
            }
        }

        /*@req <SWS_LinIf_00657>,<SWS_LinIf_00661>*/
        /* start the N_As timer(SF/FF/CF),stop the N_Cs timer(CF) */
        tpChPtr->TpTimer.EnabledTimer = LINTP_TIMER_NAS;
        tpChPtr->TpTimer.Timer        = tpChPtr->TxNSduPtr->NasCnt;

        /* Save MRF information(Except functional request) */
        if (LINTP_FUNCTIONAL_REQ_NAD != tpChPtr->SduBuf[LINTP_PDU_OFS_NAD])
        {
            uint8 pciType = tpChPtr->SduBuf[LINTP_PDU_OFS_PCI] & LINTP_PDU_PCI_MASK;
            if (LINTP_PDU_PCI_SF == pciType)
            {
                tpChPtr->MRFRequestedNad = tpChPtr->SduBuf[LINTP_PDU_OFS_NAD];
                tpChPtr->MRFRequestedSID = tpChPtr->SduBuf[LINTP_PDU_OFS_SF_SID];
            }
            else if (LINTP_PDU_PCI_FF == pciType)
            {
                tpChPtr->MRFRequestedNad = tpChPtr->SduBuf[LINTP_PDU_OFS_NAD];
                tpChPtr->MRFRequestedSID = tpChPtr->SduBuf[LINTP_PDU_OFS_FF_SID];
            }
            else
            {
                /* LINTP_PDU_PCI_CF need't to save any MRF information */
            }
        }

        (void)IStdLib_MemCpy(sduBufPtr, tpChPtr->SduBuf, LINTP_FRAME_LEN_MAX);

        ret = E_OK;
    }

    return ret;
}

/**
 * Get whether LINTP_EVENT_WAIT flag is set
 */
boolean LinTp_IsWaitEventSet(NetworkHandleType LinIfChannelId)
{
    NetworkHandleType              linTpChId = LinTp_GetLinTpChannel(LinIfChannelId);
    const LinTp_MasterRuntimeType* tpChPtr   = LinTp_MasterRtDataPtr(linTpChId);

    return LinTp_IsEvent(tpChPtr, LINTP_EVENT_WAIT);
}

/**
 * Get whether LINTP_EVENT_TX or LINTP_EVENT_STOP_SRF flag is set
 */
boolean LinTp_IsStopSRFSendEventSet(uint8 LinIfChannelId)
{
    NetworkHandleType              linTpChId = LinTp_GetLinTpChannel(LinIfChannelId);
    const LinTp_MasterRuntimeType* tpChPtr   = LinTp_MasterRtDataPtr(linTpChId);

    return LinTp_IsEvent(tpChPtr, LINTP_EVENT_TX | LINTP_EVENT_STOP_SRF);
}

/**
 * Get if LINTP_EVENT_STOP_SRF or LINTP_EVENT_STOP_MRF_SRF flag is set
 */
boolean LinTp_IsStopMRFOrSRFSendEventSet(NetworkHandleType LinIfChannelId)
{
    NetworkHandleType              linTpChId = LinTp_GetLinTpChannel(LinIfChannelId);
    const LinTp_MasterRuntimeType* tpChPtr   = LinTp_MasterRtDataPtr(linTpChId);

    return LinTp_IsEvent(tpChPtr, LINTP_EVENT_STOP_SRF | LINTP_EVENT_STOP_MRF);
}

#if (STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT)

/**
 * Get LinTpScheduleChangeDiag parameter
 */
boolean LinTp_GetScheduleChangeDiag(NetworkHandleType LinIfChannelId)
{
    NetworkHandleType              linTpChId  = LinTp_GetLinTpChannel(LinIfChannelId);
    const LinTp_ChannelConfigType* tpChCfgPtr = LinTp_GetChannelConfig(linTpChId);

    return (tpChCfgPtr->ScheduleChangeDiag);
}
#endif

/**
 * Transmitting process
 */
void LinTp_TxProcess(LinTp_MasterRuntimeType* LinTpMasterRtDataPtr)
{
    LinTp_MasterRuntimeType* tpChPtr = LinTpMasterRtDataPtr;

    /* Transmit confirmation */
    LinTp_TxEventConfirmation(tpChPtr);
}

/**
 * Receiving process
 */
void LinTp_RxProcess(LinTp_MasterRuntimeType* masterChRtDataPtr)
{
    LinTp_MasterRuntimeType* tpChPtr = masterChRtDataPtr;

    /* Receive request */
    LinTp_RxEventRequest(tpChPtr);

    /* Receive handler */
    LinTp_RxEventHandler(tpChPtr);

    /* Receive indication */
    LinTp_RxEventIndication(tpChPtr);
}

/**
 * Load transmit request information
 */
void LinTp_LoadTxRequest(NetworkHandleType ch, LinTp_MasterRuntimeType* tpChPtr)
{
    /* If tx or rx is ongoing,shall not to load transmit request */
    if (LinTp_IsTrsEvent(tpChPtr, LINTP_TRS_EVT_FUN_TX_REQ | LINTP_TRS_EVT_PHY_TX_REQ))
    {
        tpChPtr->ChCfgPtr                       = LinTp_GetChannelRefConfig((uint8)ch);
        NetworkHandleType        linifChannelId = tpChPtr->ChCfgPtr->LinIfChannelRef;
        LinIf_MasterRuntimeType* linifChPtr     = LinIf_GetMasterRtDataPtr(linifChannelId);

        if (!LinIf_IsEvent(linifChPtr, LINIF_EVENT_SKIP_SLOT_TIMER))
        {
            LinIf_SetEvent(linifChPtr, LINIF_EVENT_SKIP_SLOT_TIMER);
        }

        if (!LinIf_IsEvent(linifChPtr, LINIF_EVENT_HEADER))
        {
            LinIf_ClrEvent(linifChPtr, LINIF_EVENT_SKIP_SLOT_TIMER);

            SchM_Enter_LinTp_ExclusiveArea_Channel();

            if ((tpChPtr->RecoverMode != LINTP_APPLICATIVE_SCHEDULE)
                && (LinTp_IsTrsEvent(tpChPtr, LINTP_TRS_EVT_FUN_TX_REQ)))
            {
                LinTp_MasterRuntimeType* backupRtDataPtr = LinTp_BackupMasterRtDataPtr(ch);
                (void)IStdLib_MemCpy(backupRtDataPtr, tpChPtr, sizeof(LinTp_MasterRuntimeType));
                backupRtDataPtr->TrsEvent &= ~(LINTP_TRS_EVT_FUN_TX_REQ);
                LinTp_MasterChReset(tpChPtr);
                tpChPtr->ChannelState      = LINTP_CHANNEL_BUSY;
                tpChPtr->NeedRestoreScence = TRUE;
            }

            if (LinTp_IsTrsEvent(tpChPtr, LINTP_TRS_EVT_FUN_TX_REQ))
            {
                tpChPtr->TxNSduPtr = tpChPtr->FuncReqNSduPtr;
                tpChPtr->SduSize   = tpChPtr->FuncReqSduSize;

                /* Set functional request ongoing flag */
                LinTp_ClrTrsEvent(tpChPtr, LINTP_TRS_EVT_FUN_TX_REQ);
                LinTp_SetTrsEvent(tpChPtr, LINTP_TRS_EVT_FUN_TX);
            }
            else
            {
                if (LinTp_IsEvent(tpChPtr, LINTP_EVENT_RX))
                {
                    PduR_LinTpRxIndication(tpChPtr->RxNSduPtr->RxNSduPduRef, E_NOT_OK);
                }

                if (LinTp_IsEvent(tpChPtr, LINTP_EVENT_TX))
                {
                    PduR_LinTpTxConfirmation(tpChPtr->TxNSduPtr->TxNSduPduRef, E_NOT_OK);
                }

                /*@req <SWS_LinIf_00616>,<SWS_LinIf_00708>*/
                LinTp_MasterChReset(tpChPtr);
                tpChPtr->TxNSduPtr    = tpChPtr->PhyReqNSduPtr;
                tpChPtr->SduSize      = tpChPtr->PhyReqSduSize;
                tpChPtr->ChannelState = LINTP_CHANNEL_BUSY;
#if (STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT)
                if (tpChPtr->ChCfgPtr->ScheduleChangeDiag)
                {
                    tpChPtr->RecoverMode = LINTP_DIAG_REQUEST;
                }
#endif

                /* Set physic request ongoing flag */
                LinTp_ClrTrsEvent(tpChPtr, LINTP_TRS_EVT_PHY_TX_REQ);
                LinTp_SetTrsEvent(tpChPtr, LINTP_TRS_EVT_PHY_TX);
            }

            SchM_Exit_LinTp_ExclusiveArea_Channel();

            tpChPtr->BufReqNum    = tpChPtr->TxNSduPtr->MaxBufReq;
            tpChPtr->SduRemaining = tpChPtr->SduSize;

            /* Set transmit events */
            LinTp_SetEvent(tpChPtr, LINTP_EVENT_TX | LINTP_EVENT_COPY_REQ);
            if (tpChPtr->SduSize > (LINTP_FRAME_LEN_MAX - LINTP_PDU_OFS_SF_DATA))
            {
                LinTp_SetEvent(tpChPtr, LINTP_EVENT_FF_REQ);
            }
            else
            {
                LinTp_SetEvent(tpChPtr, LINTP_EVENT_SF_REQ);
            }

#if (STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT)
            /*@req <SWS_LinIf_00646>*/
            if (tpChPtr->ChCfgPtr->ScheduleChangeDiag)
            {
                /*@req <SWS_LinIf_00641>*/
                BswM_LinTp_RequestMode(tpChPtr->TxNSduPtr->TxNSduChannelRef, LINTP_DIAG_REQUEST);
            }
#endif /* STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT */
        }
    }
}

/**
 * Get the LinIf channel corresponding LinTp channel
 */
NetworkHandleType LinTp_GetLinTpChannel(NetworkHandleType LinIfChannelId)
{
    const LinTp_ChannelConfigType* tpChCfgPtr        = LinTp_GetChannelConfig(0u);
    NetworkHandleType              retLinTpChannelId = 0u;

    for (NetworkHandleType ch = 0u; ch < LinTp_ConfigPtr->NumOfChannel; ch++)
    {
        if (tpChCfgPtr->LinIfChannelRef == LinIfChannelId)
        {
            retLinTpChannelId = ch;
        }

        tpChCfgPtr++;
    }
    return retLinTpChannelId;
}

/**
 * Handle the timer for specific channel.
 */
void LinTp_HandleTimers(LinTp_MasterRuntimeType* masterChRtDataPtr, NetworkHandleType LinIfChannelId)
{
    LinTp_MasterRuntimeType* tpChPtr = masterChRtDataPtr;
    NetworkHandleType        network;
    NetworkHandleType        linIfChannel;

    /* Update N_As,N_Cs,N_Cr timer */
    if (LINTP_TIMER_NONE != tpChPtr->TpTimer.EnabledTimer)
    {
        if (tpChPtr->TpTimer.Timer > 0u)
        {
            tpChPtr->TpTimer.Timer--;
            if (0u == tpChPtr->TpTimer.Timer)
            {
                /* Abort transmission process */
                if ((LINTP_TIMER_NAS == tpChPtr->TpTimer.EnabledTimer)
                    || (LINTP_TIMER_NCS == tpChPtr->TpTimer.EnabledTimer))
                {
                    /*@req <SWS_LinIf_00658>,<SWS_LinIf_00662>*/
                    PduR_LinTpTxConfirmation(tpChPtr->TxNSduPtr->TxNSduPduRef, E_NOT_OK);

                    network      = tpChPtr->TxNSduPtr->TxNSduChannelRef;
                    linIfChannel = tpChPtr->TxNSduPtr->LinIfChannelRef;
                }
                else
                {
                    /*@req <SWS_LinIf_00666>*/
                    PduR_LinTpRxIndication(tpChPtr->RxNSduPtr->RxNSduPduRef, E_NOT_OK);

                    network      = tpChPtr->RxNSduPtr->RxNSduChannelRef;
                    linIfChannel = tpChPtr->RxNSduPtr->LinIfChannelRef;
                }
#if (STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT)
                /*@req <SWS_LinIf_00646>*/
                if (tpChPtr->ChCfgPtr->ScheduleChangeDiag)
                {
                    /*@req <SWS_LinIf_00658>,<SWS_LinIf_00662>,
                      <SWS_LinIf_00666>*/
                    /* Change schedule table by BswM */
                    BswM_LinTp_RequestMode(network, LINTP_APPLICATIVE_SCHEDULE);
                    tpChPtr->RecoverMode = LINTP_APPLICATIVE_SCHEDULE;
                }
                else
                {
                }
#endif
                /* Reset channel */
                LinTp_MasterChReset(tpChPtr);
                /* Clear the flag of header and response */
                LinIf_ClearEvent(linIfChannel, LINIF_EVENT_HEADER | LINIF_EVENT_RESPONSE);
                /* Next entry */
                LinIf_MoveScheduleToNextEntry(linIfChannel);
            }
        }
    }

    /* Update P2(P2*Max) timer */
    if (LINTP_TIMER_NONE != tpChPtr->TpP2Timer.EnabledTimer)
    {
        if (tpChPtr->TpP2Timer.Timer > 0u)
        {
            tpChPtr->TpP2Timer.Timer--;
            if (0u == tpChPtr->TpP2Timer.Timer)
            {
                /*@req <SWS_LinIf_00619>*/
                const LinTp_RxNSduType* rx = LinTp_GetRxNSduByNad(LinIfChannelId, tpChPtr->MRFRequestedNad);
                PduR_LinTpRxIndication(rx->RxNSduPduRef, E_NOT_OK);
                linIfChannel = rx->LinIfChannelRef;
#if (STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT)
                /* Notify PduR,Change schedule table */
                /*@req <SWS_LinIf_00646>*/
                if (tpChPtr->ChCfgPtr->ScheduleChangeDiag)
                {
                    network = rx->RxNSduChannelRef;
                    /*@req <SWS_LinIf_00619>*/
                    /* Change schedule table by BswM */
                    BswM_LinTp_RequestMode(network, LINTP_APPLICATIVE_SCHEDULE);
                    tpChPtr->RecoverMode = LINTP_APPLICATIVE_SCHEDULE;
                }
                else
                {
                }
#endif

                /* Reset channel */
                LinTp_MasterChReset(tpChPtr);
                /* Clear the flag of header and response */
                LinIf_ClearEvent(linIfChannel, LINIF_EVENT_HEADER | LINIF_EVENT_RESPONSE);
                /* Next entry */
                LinIf_MoveScheduleToNextEntry(linIfChannel);
            }
        }
    }
}

/* PRQA S 1532 -- */

/* ========================================== internal function definition ========================================== */

LINTP_LOCAL void LinTp_MasterRxHandler(
    uint8                          pciType,
    const uint8*                   sdu,
    LinTp_MasterRuntimeType*       tpChPtr,
    const LinTp_ChannelConfigType* chCfgPtr,
    const LinTp_RxNSduType*        rx)
{
    boolean isCopy        = FALSE;
    boolean isIgnoreFrame = FALSE;

    if ((LINTP_PDU_PCI_SF == pciType) || (LINTP_PDU_PCI_FF == pciType))
    {
        if ((LINTP_FRAMETYPE_CF == tpChPtr->LastFrameType) || (LINTP_FRAMETYPE_FF == tpChPtr->LastFrameType))
        {
            /*@req <SWS_LinIf_00653>*/
            PduR_LinTpRxIndication(rx->RxNSduPduRef, E_NOT_OK);

            /* MRF Requested Nad and SID should not be cleared*/
            uint8 bakupMRFRequestedNad = tpChPtr->MRFRequestedNad;
            uint8 bakupMRFRequestedSID = tpChPtr->MRFRequestedSID;
            /*@req <SWS_LinIf_00651>*/
            /* Reset channel */
            LinTp_MasterChReset(tpChPtr);
            /* Recover Nad and SID */
            tpChPtr->MRFRequestedNad = bakupMRFRequestedNad;
            tpChPtr->MRFRequestedSID = bakupMRFRequestedSID;
        }

        if (LINTP_PDU_PCI_SF == pciType)
        {
            isCopy = LinTp_IsRxSF(tpChPtr, rx, sdu, &isIgnoreFrame);
        }
        else
        {
            isCopy = LinTp_IsRxFF(tpChPtr, rx, sdu, &isIgnoreFrame);
        }
    }
    else if (LINTP_PDU_PCI_CF == pciType)
    {
        if (LINTP_FRAMETYPE_NONE == tpChPtr->LastFrameType)
        {
            /*@req <SWS_LinIf_00696>*/
            isIgnoreFrame = TRUE;
        }
        else
        {
            isCopy = LinTp_IsRxCF(tpChPtr, sdu);
        }
    }
    else
    {
        isIgnoreFrame = TRUE;
    }

    if (isIgnoreFrame)
    {
        return;
    }

    /*@req <SWS_LinIf_00664>,<SWS_LinIf_00665>*/
    /* Start the N_Cr timer(FF or CF),stop the last N_Cr */
    if ((LINTP_PDU_PCI_FF == pciType) || (LINTP_PDU_PCI_CF == pciType))
    {
        tpChPtr->TpTimer.EnabledTimer = LINTP_TIMER_NCR;
        tpChPtr->TpTimer.Timer        = rx->NcrCnt;
    }

    /*@req <SWS_LinIf_00617>,<SWS_LinIf_00618>*/
    /* Stop P2/P2*max timer */
    if ((LINTP_PDU_PCI_FF == pciType) || (LINTP_PDU_PCI_SF == pciType))
    {
        tpChPtr->TpP2Timer.EnabledTimer = LINTP_TIMER_NONE;
    }

    if (isCopy)
    {
        /* Data copy */
        (void)IStdLib_MemCpy(tpChPtr->SduBuf, sdu, LINTP_FRAME_LEN_MAX);

        /* Set events */
        LinTp_SetEvent(tpChPtr, LINTP_EVENT_RX_REQ);

        tpChPtr->LastFrameType = (LinTp_FrameType)pciType;
        tpChPtr->ChCfgPtr      = chCfgPtr;
    }
    else
    {
        /* stop the current LIN TP message reception*/
        LinTp_MasterCancelRxHandler(tpChPtr, chCfgPtr, rx);
    }
}

LINTP_LOCAL void LinTp_MasterCancelRxHandler(
    LinTp_MasterRuntimeType*       tpChPtr,
    const LinTp_ChannelConfigType* chCfgPtr,
    const LinTp_RxNSduType*        rx)
{
    /*@req <SWS_LinIf_00655>,<SWS_LinIf_00081>,<SWS_LinIf_00623>*/
    PduR_LinTpRxIndication(rx->RxNSduPduRef, E_NOT_OK);

    /* Reset channel */
    LinTp_MasterChReset(tpChPtr);

    /* Next entry */
    LinIf_MoveScheduleToNextEntry(rx->LinIfChannelRef);
#if (STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT)
    /*@req <SWS_LinIf_00646>*/
    if (chCfgPtr->ScheduleChangeDiag)
    {
        /*@req <SWS_LinIf_00655>,<SWS_LinIf_00623>*/
        BswM_LinTp_RequestMode(rx->RxNSduChannelRef, LINTP_APPLICATIVE_SCHEDULE);
        tpChPtr->RecoverMode = LINTP_APPLICATIVE_SCHEDULE;
    }
#endif
}

/**
 * @brief               Copy transfer data failure process
 * @param[inout]        tpChPtr: The runtime data of lintp master channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINTP_LOCAL void LinTp_HandleCopyTxDataFailure(LinTp_MasterRuntimeType* tpChPtr)
{
    /*@req <SWS_LinIf_00073>*/
    PduR_LinTpTxConfirmation(tpChPtr->TxNSduPtr->TxNSduPduRef, E_NOT_OK);
    if (LinTp_IsTrsEvent(tpChPtr, LINTP_TRS_EVT_FUN_TX))
    {
#if (STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT)
        if (tpChPtr->ChCfgPtr->ScheduleChangeDiag)
        {
            /*@req <SWS_LinIf_00673>*/
            BswM_LinTp_RequestMode(tpChPtr->TxNSduPtr->TxNSduChannelRef, LINTP_APPLICATIVE_SCHEDULE);
            tpChPtr->RecoverMode = LINTP_APPLICATIVE_SCHEDULE;
        }

        NetworkHandleType lintpCh = LinTp_GetLinTpChannel(tpChPtr->ChCfgPtr->LinIfChannelRef);
        LinTp_MasterChReset(tpChPtr);
        if (tpChPtr->NeedRestoreScence)
        {
            if (!LinTp_IsTrsEvent(tpChPtr, LINTP_TRS_EVT_PHY_TX_REQ))
            {
                LinTp_MasterRuntimeType* backupRtData = LinTp_BackupMasterRtDataPtr(lintpCh);
                (void)IStdLib_MemCpy(tpChPtr, backupRtData, sizeof(LinTp_MasterRuntimeType));
            }

            tpChPtr->ChannelState      = LINTP_CHANNEL_BUSY;
            tpChPtr->NeedRestoreScence = FALSE;
        }
#endif
    }
    else
    {
#if (STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT)
        if (tpChPtr->ChCfgPtr->ScheduleChangeDiag)
        {
            /*@req <SWS_LinIf_00673>*/
            BswM_LinTp_RequestMode(tpChPtr->TxNSduPtr->TxNSduChannelRef, LINTP_APPLICATIVE_SCHEDULE);
            tpChPtr->RecoverMode = LINTP_APPLICATIVE_SCHEDULE;
        }
#endif /* STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT */

        /* Next entry */
        LinIf_MoveScheduleToNextEntry(tpChPtr->TxNSduPtr->LinIfChannelRef);
        /* Reset channel */
        LinTp_MasterChReset(tpChPtr);
    }
}

/**
 * @brief               Copy transfer data from the 'PduR'
 * @param[inout]        tpChPtr: The runtime data of lintp master channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINTP_LOCAL boolean LinTp_CopyTxDataFromPduR(LinTp_MasterRuntimeType* tpChPtr)
{
    boolean result = FALSE;

    if (tpChPtr->BufReqNum > 0u)
    {
        PduInfoType   infoData;
        PduInfoType*  info = &infoData;
        PduLengthType len  = (LINTP_FRAME_LEN_MAX - tpChPtr->SduIdx);

        /* Get a copy of the effective length */
        len = LinIf_Min(len, tpChPtr->SduRemaining);

        /* Set destination buffer pointer and he number of bytes to be copied */
        info->SduDataPtr = &(tpChPtr->SduBuf[tpChPtr->SduIdx]);
        info->SduLength  = len;
        /*@req <SWS_LinIf_00671>,<SWS_LinIf_00705>*/
        /* Copy data from PduR */
        BufReq_ReturnType bufRslt =
            PduR_LinTpCopyTxData(tpChPtr->TxNSduPtr->TxNSduPduRef, info, NULL_PTR, &(tpChPtr->UpperBufRemaining));

        switch (bufRslt)
        {
        case BUFREQ_OK:
            /* Update buffer information */
            LinTp_ClrEvent(tpChPtr, LINTP_EVENT_COPY_REQ | LINTP_EVENT_STOP_MRF);

            tpChPtr->SduIdx += len;
            tpChPtr->SduRemaining -= len;
            /* reload retry counter */
            tpChPtr->BufReqNum = tpChPtr->TxNSduPtr->MaxBufReq;
            result             = TRUE;
            break;

        case BUFREQ_E_NOT_OK:
            LinTp_HandleCopyTxDataFailure(tpChPtr);
            break;

        case BUFREQ_E_BUSY:
            /* retry to copy data */
            /* stop to send MRF until copy data successful */
            LinTp_SetEvent(tpChPtr, LINTP_EVENT_STOP_MRF);
            break;

        default:
            /* Other return value */
            break;
        }

        tpChPtr->BufReqNum--;
    }
    else
    {
        LinTp_HandleCopyTxDataFailure(tpChPtr);
    }

    return result;
}

/**
 * @brief               Requests a event of the transmitting
 * @param[inout]        tpChPtr: The runtime data of lintp master channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINTP_LOCAL boolean LinTp_TxEventRequest(LinTp_MasterRuntimeType* tpChPtr)
{
    if (!(LinTp_IsEvent(tpChPtr, LINTP_EVENT_TX)))
    {
        return FALSE;
    }

    boolean result = FALSE;

    if (!(LinTp_IsEvent(tpChPtr, LINTP_EVENT_CONF | LINTP_EVENT_OK)))
    {
        /* SF */
        if (LinTp_IsEvent(tpChPtr, LINTP_EVENT_SF_REQ))
        {
            LinTp_ClrEvent(tpChPtr, LINTP_EVENT_SF_REQ);
            LinTp_ConstructSF(tpChPtr);
        }
        /* FF */
        else if (LinTp_IsEvent(tpChPtr, LINTP_EVENT_FF_REQ))
        {
            LinTp_ClrEvent(tpChPtr, LINTP_EVENT_FF_REQ);
            LinTp_ConstructFF(tpChPtr);
        }
        /* CF */
        else if (!(LinTp_IsEvent(tpChPtr, LINTP_EVENT_TX_REQ)) && (LinTp_IsEvent(tpChPtr, LINTP_EVENT_CF_REQ)))
        {
            LinTp_ClrEvent(tpChPtr, LINTP_EVENT_CF_REQ);
            LinTp_SetEvent(tpChPtr, LINTP_EVENT_COPY_REQ);
            LinTp_ConstructCF(tpChPtr);
        }
        else
        {
            /* only for the MISRA-C */
        }

        if (LinTp_IsEvent(tpChPtr, LINTP_EVENT_COPY_REQ))
        {
            /* Copy data from PduR */
            result = LinTp_CopyTxDataFromPduR(tpChPtr);
        }
    }
    return result;
}

/**
 * @brief               Handles a event of the transmitting
 * @param[inout]        tpChPtr: The runtime data of lintp master channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINTP_LOCAL void LinTp_TxEventHandler(LinTp_MasterRuntimeType* tpChPtr)
{
    if (!(LinTp_IsEvent(tpChPtr, LINTP_EVENT_TX)))
    {
        return;
    }

    if (!(LinTp_IsEvent(tpChPtr, LINTP_EVENT_TX_REQ | LINTP_EVENT_CONF | LINTP_EVENT_OK)))
    {
        /* FF/CF */
        if ((tpChPtr->SduRemaining != 0u) && (tpChPtr->SduIdx == LINTP_FRAME_LEN_MAX))
        {
            LinTp_SetEvent(tpChPtr, LINTP_EVENT_TX_REQ | LINTP_EVENT_CF_REQ);
        }
        /* SF/CF */
        else if (tpChPtr->SduRemaining == 0u)
        {
            /* Fill padding value */
            if (tpChPtr->SduIdx != LINTP_FRAME_LEN_MAX)
            {
                PduLengthType len = LINTP_FRAME_LEN_MAX - tpChPtr->SduIdx;
                (void)IStdLib_MemSet(&tpChPtr->SduBuf[tpChPtr->SduIdx], LINTP_PADDING_VALUE, len);
            }
            LinTp_SetEvent(tpChPtr, LINTP_EVENT_TX_REQ | LINTP_EVENT_CONF_REQ);
        }
        /* SF/FF/CF */
        else
        {
            LinTp_SetEvent(tpChPtr, LINTP_EVENT_COPY_REQ);
        }
    }
}

/**
 * @brief               Confirmations a event of the transmitting
 * @param[inout]        tpChPtr: The runtime data of lintp master channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINTP_LOCAL void LinTp_TxEventConfirmation(LinTp_MasterRuntimeType* tpChPtr)
{
    if ((LinTp_IsEvent(tpChPtr, LINTP_EVENT_OK)) && (LinTp_IsEvent(tpChPtr, LINTP_EVENT_TX)))
    {
        /*@req <SWS_LinIf_00068>*/
        /* Notify upper */
        PduR_LinTpTxConfirmation(tpChPtr->TxNSduPtr->TxNSduPduRef, E_OK);
        /*@req <SWS_LinIf_00646>*/
        if (LinTp_IsTrsEvent(tpChPtr, LINTP_TRS_EVT_FUN_TX))
        {
#if (STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT)
            if ((tpChPtr->ChCfgPtr->ScheduleChangeDiag) && (tpChPtr->RecoverMode != LINTP_DIAG_REQUEST))
            {
                /*@req <SWS_LinIf_00707>*/
                BswM_LinTp_RequestMode(tpChPtr->TxNSduPtr->TxNSduChannelRef, tpChPtr->RecoverMode);
            }
#endif

            NetworkHandleType lintpCh = LinTp_GetLinTpChannel(tpChPtr->ChCfgPtr->LinIfChannelRef);
            LinTp_MasterChReset(tpChPtr);
            if (tpChPtr->NeedRestoreScence)
            {
                if (!LinTp_IsTrsEvent(tpChPtr, LINTP_TRS_EVT_PHY_TX_REQ))
                {
                    LinTp_MasterRuntimeType* backupRtData = LinTp_BackupMasterRtDataPtr(lintpCh);
                    (void)IStdLib_MemCpy(tpChPtr, backupRtData, sizeof(LinTp_MasterRuntimeType));
                }

                tpChPtr->ChannelState      = LINTP_CHANNEL_BUSY;
                tpChPtr->NeedRestoreScence = FALSE;
            }
        }
        else
        {
#if (STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT)
            if (tpChPtr->ChCfgPtr->ScheduleChangeDiag)
            {
                /*@req <SWS_LinIf_00642>*/
                BswM_LinTp_RequestMode(tpChPtr->TxNSduPtr->TxNSduChannelRef, LINTP_DIAG_RESPONSE);
                tpChPtr->RecoverMode = LINTP_DIAG_RESPONSE;
            }
#endif

            /* MRF send OK,P2 timer should not be cleared */
            LinTp_TimerType bakupTimerType = tpChPtr->TpP2Timer.EnabledTimer;
            /* MRF Requested Nad and SID should not be cleared*/
            uint8 bakupMRFRequestedNad = tpChPtr->MRFRequestedNad;
            uint8 bakupMRFRequestedSID = tpChPtr->MRFRequestedSID;
            /* Reset channel */
            LinTp_MasterChReset(tpChPtr); /* PRQA S 2982 */ /* VL_LinTp_2982 */
            /* recover */
            tpChPtr->TpP2Timer.EnabledTimer = bakupTimerType;
            tpChPtr->MRFRequestedNad        = bakupMRFRequestedNad;
            tpChPtr->MRFRequestedSID        = bakupMRFRequestedSID;
        }
    }
}

/**
 * @brief               Receiving a signal frame
 * @param[inout]        tpChPtr: The runtime data of lintp master channel
 * @param[in]           tx: pointer of rxSdu that be received
 * @param[in]           sdu: pointer of sdu data buffer
 * @param[out]          isIgnoreFramePtr: Whether need to ignore receive frame
 * @return              TRUE: Received sdu buffer has copy
 * @return              FALSE: Received sdu buffer not copy
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINTP_LOCAL boolean LinTp_IsRxSF(
    LinTp_MasterRuntimeType* tpChPtr,
    const LinTp_RxNSduType*  rx,
    const uint8*             sdu,
    boolean*                 isIgnoreFramePtr)
{
    boolean isCopy = FALSE;

    /* Update buffer information */
    tpChPtr->SduSize      = (PduLengthType)sdu[LINTP_PDU_OFS_PCI] & LINTP_PDU_PCI_DL_MASK;
    tpChPtr->SduRemaining = tpChPtr->SduSize;
    tpChPtr->RxNSduPtr    = rx;

    if ((tpChPtr->SduSize > LINTP_LENGTH_SF_MIN) && (tpChPtr->SduSize <= LINTP_LENGTH_SF_MAX))
    {
        /* Set events */
        LinTp_SetEvent(tpChPtr, LINTP_EVENT_RX | LINTP_EVENT_SF_REQ | LINTP_EVENT_COPY_REQ);
        /* Do not need to send a header again */
        LinTp_SetEvent(tpChPtr, LINTP_EVENT_STOP_SRF);

        /*@req <SWS_LinIf_00323>*/
        /* Setting up the status */
        tpChPtr->ChannelState = LINTP_CHANNEL_BUSY;

        isCopy = TRUE;
    }
    else
    {
        /*@req <SWS_LinIf_00652>*/
        /* Invalid data length,Ignore this message */
        *isIgnoreFramePtr = TRUE;
    }

    return isCopy;
}

/**
 * @brief               Receiving a first frame
 * @param[inout]        tpChPtr: The runtime data of lintp master channel
 * @param[in]           tx: pointer of rxSdu that be received
 * @param[in]           sdu: pointer of sdu data buffer
 * @param[out]          isIgnoreFramePtr: Whether need to ignore receive frame
 * @return              TRUE: Received sdu buffer has copy
 * @return              FALSE: Received sdu buffer not copy
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINTP_LOCAL boolean LinTp_IsRxFF(
    LinTp_MasterRuntimeType* tpChPtr,
    const LinTp_RxNSduType*  rx,
    const uint8*             sdu,
    boolean*                 isIgnoreFramePtr)
{
    boolean isCopy = FALSE;

    /* Update buffer information */
    tpChPtr->SduSize = (PduLengthType)sdu[LINTP_PDU_OFS_PCI] & LINTP_PDU_PCI_DL_MASK;
    tpChPtr->SduSize <<= LINTP_PDU_DATESIZE_PCI_EXTERNINFO_OFFSET;
    tpChPtr->SduSize |= sdu[LINTP_PDU_OFS_LEN];
    tpChPtr->SduRemaining = tpChPtr->SduSize;
    tpChPtr->RxNSduPtr    = rx;

    /*@req <SWS_LinIf_00652>*/
    if (tpChPtr->SduSize >= LINTP_LENGTH_FF_MIN)
    {
        /* Set events */
        LinTp_SetEvent(tpChPtr, LINTP_EVENT_RX | LINTP_EVENT_FF_REQ | LINTP_EVENT_COPY_REQ);

        /*@req <SWS_LinIf_00323>*/
        /* Setting up the status */
        tpChPtr->ChannelState = LINTP_CHANNEL_BUSY;

        isCopy = TRUE;
    }
    else
    {
        /*@req <SWS_LinIf_00652>*/
        /* Invalid data length,Ignore this message */
        *isIgnoreFramePtr = TRUE;
    }

    return isCopy;
}

/**
 * @brief               Receiving a consecutive frame
 * @param[inout]        tpChPtr: The runtime data of lintp master channel
 * @param[in]           sdu: pointer of sdu data buffer
 * @return              TRUE: Received sdu buffer has copy
 * @return              FALSE: Received sdu buffer not copy
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINTP_LOCAL boolean LinTp_IsRxCF(LinTp_MasterRuntimeType* tpChPtr, const uint8* sdu)
{
    boolean isCopy = FALSE;

    /* Update serial number */
    tpChPtr->SduSN++;
    tpChPtr->SduSN &= LINTP_PDU_PCI_SN_MASK;

    if ((LinTp_IsEvent(tpChPtr, LINTP_EVENT_RX))
        && ((sdu[LINTP_PDU_OFS_PCI] & LINTP_PDU_PCI_SN_MASK) == tpChPtr->SduSN))
    {
        /* Set events */
        LinTp_SetEvent(tpChPtr, LINTP_EVENT_CF_REQ | LINTP_EVENT_COPY_REQ);

        /* Check Whether need to send a header again */
        if (tpChPtr->SduRemaining <= (LINTP_FRAME_LEN_MAX - LINTP_PDU_OFS_SF_DATA))
        {
            /* Do not need to send a header again */
            LinTp_SetEvent(tpChPtr, LINTP_EVENT_STOP_SRF);
        }
        isCopy = TRUE;
    }
    else
    {
        /*@req <SWS_LinIf_00079>*/
        /* Incorrect sequence number is received,stop current reception*/
    }

    return isCopy;
}

/**
 * @brief               Gets the LinTp channel corresponding to LinIf channel
 * @param[in]           ch: Identification of the LIN channel
 * @return              chCfgPtr: LinIf channel config
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINTP_LOCAL const LinTp_ChannelConfigType* LinTp_GetChannelRefConfig(uint8 ch)
{
    const LinTp_ChannelConfigType* chCfgPtr = LinTp_GetChannelConfig(0u);
    uint8                          idx      = LINIF_NUMBER_OF_CHANNELS;

    while (idx > 0u)
    {
        if (chCfgPtr->LinIfChannelRef == ch)
        {
            return chCfgPtr;
        }
        chCfgPtr++;

        idx--;
    }

    return NULL_PTR;
}

/**
 * @brief               Requests a receive buffer from the 'PduR'
 * @param[inout]        tpChPtr: Runtime data of lintp master channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINTP_LOCAL void LinTp_RxBufferRequestStart(LinTp_MasterRuntimeType* tpChPtr)
{
    PduLengthType len                             = (LINTP_FRAME_LEN_MAX - tpChPtr->SduIdx);
    uint8         data[LINTP_METADATA_BUFFER_LEN] = {0};

    /* Get a copy of the effective length */
    len = LinIf_Min(len, tpChPtr->SduSize);

    PduInfoType infoData;
    infoData.MetaDataPtr = data;
    infoData.SduLength   = 0u;

    const PduInfoType* infoDataPtr = &infoData;
    /*@req <SWS_LinIf_00075>*/
    /* Start copy data to PduR */
    BufReq_ReturnType bufRslt = PduR_LinTpStartOfReception(
        tpChPtr->RxNSduPtr->RxNSduPduRef,
        infoDataPtr,
        tpChPtr->SduSize,
        &(tpChPtr->UpperBufRemaining));

    if ((BUFREQ_OK == bufRslt) && (tpChPtr->UpperBufRemaining >= len))
    {
        /* Buffer Request Success */
    }
    else
    {
        /*@req <SWS_LinIf_00701>*/
        if (BUFREQ_OK == bufRslt) /* PRQA S 2991,2995 */ /* VL_LinTp_Logical */
        {
            /* Notify upper */
            PduR_LinTpRxIndication(tpChPtr->RxNSduPtr->RxNSduPduRef, E_NOT_OK);
        }
#if (STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT)
        if (tpChPtr->ChCfgPtr->ScheduleChangeDiag)
        {
            BswM_LinTp_RequestMode(tpChPtr->RxNSduPtr->RxNSduChannelRef, LINTP_APPLICATIVE_SCHEDULE);
            tpChPtr->RecoverMode = LINTP_APPLICATIVE_SCHEDULE;
        }
#endif

        /* Clear flag of header */
        LinIf_ClearEvent(tpChPtr->RxNSduPtr->LinIfChannelRef, LINIF_EVENT_HEADER);
        /* Next entry */
        LinIf_MoveScheduleToNextEntry(tpChPtr->RxNSduPtr->LinIfChannelRef);

        /*@req <SWS_LinIf_00676>,<SWS_LinIf_00701> */
        /* Reset channel */
        LinTp_MasterChReset(tpChPtr);
    }
}

/**
 * @brief               Requests a receive buffer from the 'PduR'
 * @param[inout]        tpChPtr: Runtime data of lintp master channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINTP_LOCAL void LinTp_RxBufferRequest(LinTp_MasterRuntimeType* tpChPtr)
{
    PduInfoType   infoData;
    uint8         data[LINTP_METADATA_BUFFER_LEN] = {0};
    PduLengthType len                             = (LINTP_FRAME_LEN_MAX - tpChPtr->SduIdx);

    /* Get a copy of the effective length */
    len = LinIf_Min(len, tpChPtr->SduRemaining);

    /* Set destination buffer pointer and the number of bytes to be copied */
    PduInfoType* info = &infoData;
    info->SduDataPtr  = &(tpChPtr->SduBuf[tpChPtr->SduIdx]);
    info->MetaDataPtr = data;
    if (len <= tpChPtr->UpperBufRemaining)
    {
        info->SduLength = len;
    }
    else
    {
        /*@req <SWS_LinIf_00086> */
        /* retry to get buffer */
        info->SduLength = 0u;
    }
    /*@req <SWS_LinIf_00674> */
    /* Copy data to PduR */
    BufReq_ReturnType bufRslt =
        PduR_LinTpCopyRxData(tpChPtr->RxNSduPtr->RxNSduPduRef, info, &(tpChPtr->UpperBufRemaining));

    /* Update buffer information */
    tpChPtr->SduRemaining -= info->SduLength;
    tpChPtr->SduIdx += info->SduLength;

    /*@req <SWS_LinIf_00677> */
    if (BUFREQ_E_NOT_OK == bufRslt)
    {
        /* Notify upper */
        PduR_LinTpRxIndication(tpChPtr->RxNSduPtr->RxNSduPduRef, E_NOT_OK);
#if (STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT)
        /*@req <SWS_LinIf_00646>*/
        if (tpChPtr->ChCfgPtr->ScheduleChangeDiag)
        {
            BswM_LinTp_RequestMode(tpChPtr->RxNSduPtr->RxNSduChannelRef, LINTP_APPLICATIVE_SCHEDULE);
            tpChPtr->RecoverMode = LINTP_APPLICATIVE_SCHEDULE;
        }
#endif

        /* Clear flag of header */
        LinIf_ClearEvent(tpChPtr->RxNSduPtr->LinIfChannelRef, LINIF_EVENT_HEADER);

        /* Next entry */
        LinIf_MoveScheduleToNextEntry(tpChPtr->RxNSduPtr->LinIfChannelRef);

        /* Reset channel */
        LinTp_MasterChReset(tpChPtr);
    }
    else
    {
        if (tpChPtr->SduRemaining == 0u)
        {
            LinTp_ClrEvent(tpChPtr, LINTP_EVENT_RX_REQ | LINTP_EVENT_COPY_REQ);
            LinTp_SetEvent(tpChPtr, LINTP_EVENT_OK);
        }
        else
        {
            /* The needed size of next CF */
            len = (LINTP_FRAME_LEN_MAX - LINTP_PDU_OFS_CF_DATA);
            len = LinIf_Min(len, tpChPtr->SduRemaining);

            if (tpChPtr->UpperBufRemaining >= len)
            {
                LinTp_ClrEvent(tpChPtr, LINTP_EVENT_RX_REQ | LINTP_EVENT_COPY_REQ);
                if (info->SduLength == 0u)
                {
                    /*@req <SWS_LinIf_00680> */
                    /* Resume send header */
                    LinTp_ClrEvent(tpChPtr, LINTP_EVENT_STOP_SRF);
                }
            }
            else
            {
                /* Retry to request buffer */
                LinTp_SetEvent(tpChPtr, LINTP_EVENT_CF_REQ);
                if (info->SduLength != 0u)
                {
                    /*@req <SWS_LinIf_00679> */
                    /* Stop send header */
                    LinTp_SetEvent(tpChPtr, LINTP_EVENT_STOP_SRF);
                }
            }
        }
    }
}

/**
 * @brief               Requests a event of the receiving
 * @param[inout]        tpChPtr: Runtime data of lintp master channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINTP_LOCAL void LinTp_RxEventRequest(LinTp_MasterRuntimeType* tpChPtr)
{
    boolean firstRequestBufFlag = FALSE;

    if (LinTp_IsEvent(tpChPtr, LINTP_EVENT_RX_REQ))
    {
        /* SF */
        if (LinTp_IsEvent(tpChPtr, LINTP_EVENT_SF_REQ))
        {
            LinTp_ClrEvent(tpChPtr, LINTP_EVENT_SF_REQ);
            tpChPtr->SduIdx     = LINTP_PDU_OFS_SF_DATA;
            firstRequestBufFlag = TRUE;
        }
        /* FF */
        else if (LinTp_IsEvent(tpChPtr, LINTP_EVENT_FF_REQ))
        {
            LinTp_ClrEvent(tpChPtr, LINTP_EVENT_FF_REQ);
            tpChPtr->SduIdx     = LINTP_PDU_OFS_FF_DATA;
            tpChPtr->SduSN      = 0u;
            firstRequestBufFlag = TRUE;
        }
        /* CF */
        else if (LinTp_IsEvent(tpChPtr, LINTP_EVENT_CF_REQ))
        {
            LinTp_ClrEvent(tpChPtr, LINTP_EVENT_CF_REQ);
            tpChPtr->SduIdx = LINTP_PDU_OFS_CF_DATA;
        }
        else
        {
            /* only for the MISRA-C */
        }

        if ((firstRequestBufFlag) && (LinTp_IsEvent(tpChPtr, LINTP_EVENT_COPY_REQ)))
        {
            /* Request a receiving buffer */
            LinTp_RxBufferRequestStart(tpChPtr);
        }
    }
}

/**
 * @brief               Handles a event of the receiving
 * @param[inout]        tpChPtr: Runtime data of lintp master channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINTP_LOCAL void LinTp_RxEventHandler(LinTp_MasterRuntimeType* tpChPtr)
{
    if ((LinTp_IsEvent(tpChPtr, LINTP_EVENT_RX_REQ)) && (LinTp_IsEvent(tpChPtr, LINTP_EVENT_COPY_REQ)))
    {
        /* Copy Rx data to PduR buffer */
        LinTp_RxBufferRequest(tpChPtr);
    }
}

/**
 * @brief               Indicates a event of the receiving
 * @param[inout]        tpChPtr: Runtime data of lintp master channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINTP_LOCAL void LinTp_RxEventIndication(LinTp_MasterRuntimeType* tpChPtr)
{
    if ((LinTp_IsEvent(tpChPtr, LINTP_EVENT_OK)) && (LinTp_IsEvent(tpChPtr, LINTP_EVENT_RX)))
    {
        /*@req <SWS_LinIf_00078>*/
        /* Notify upper */
        PduR_LinTpRxIndication(tpChPtr->RxNSduPtr->RxNSduPduRef, E_OK);
#if (STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT)
        /*@req <SWS_LinIf_00646>*/
        if (tpChPtr->ChCfgPtr->ScheduleChangeDiag)
        {
            /*@req <SWS_LinIf_00643>*/
            BswM_LinTp_RequestMode(tpChPtr->RxNSduPtr->RxNSduChannelRef, LINTP_APPLICATIVE_SCHEDULE);
            tpChPtr->RecoverMode = LINTP_APPLICATIVE_SCHEDULE;
        }
#endif

        /* Next entry */
        LinIf_MoveScheduleToNextEntry(tpChPtr->RxNSduPtr->LinIfChannelRef);

        /* Reset channel */
        LinTp_MasterChReset(tpChPtr);
    }
}

#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

/* PRQA S 2814,2824,2889 -- */

#endif
#endif
