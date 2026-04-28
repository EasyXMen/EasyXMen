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
********************************************************************************
**                                                                            **
**  FILENAME    : LinTp_Master.c                                              **
**                                                                            **
**  Created on  :                                                             **
**  Author      : HuRongbo                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation for LinIf                                    **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/

/*******************************************************************************
**                      Version Information Definitions                       **
*******************************************************************************/

/*******************************************************************************
**                               Includes                                     **
*******************************************************************************/
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

/* PRQA S 2814,2824,2813,2889 ++ */ /* VL_QAC_DerefNullPtr */
/* PRQA S 3432 ++ */                /* VL_LinIf_MacrosFunctionUsed */
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

static FUNC(void, LINIF_CODE)
    LinTp_HandleCopyTxDataFailure(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr);

static FUNC(boolean, LINIF_CODE)
    LinTp_CopyTxDataFromPduR(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr);

static FUNC(boolean, LINIF_CODE)
    LinTp_TxEventRequest(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr);

static FUNC(void, LINIF_CODE) LinTp_TxEventHandler(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr);

static FUNC(void, LINIF_CODE)
    LinTp_TxEventConfirmation(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr);

static FUNC(boolean, LINIF_CODE) LinTp_IsRxSF(
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr,
    P2CONST(LinTp_RxNSduType, AUTOMATIC, LINIF_APPL_CONST) rx,
    P2CONST(uint8, AUTOMATIC, LINIF_APPL_DATA) sdu,
    P2VAR(boolean, AUTOMATIC, LINIF_APPL_DATA) isIgnoreFramePtr);

static FUNC(boolean, LINIF_CODE) LinTp_IsRxFF(
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr,
    P2CONST(LinTp_RxNSduType, AUTOMATIC, LINIF_APPL_CONST) rx,
    P2CONST(uint8, AUTOMATIC, LINIF_APPL_DATA) sdu,
    P2VAR(boolean, AUTOMATIC, LINIF_APPL_DATA) isIgnoreFramePtr);

static FUNC(boolean, LINIF_CODE) LinTp_IsRxCF(
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr,
    P2CONST(uint8, AUTOMATIC, LINIF_APPL_DATA) sdu);

static FUNC(void, LINIF_CODE)
    LinTp_RxBufferRequestStart(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr);

static FUNC(void, LINIF_CODE) LinTp_RxBufferRequest(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr);

static FUNC(void, LINIF_CODE) LinTp_RxEventRequest(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr);

static FUNC(void, LINIF_CODE) LinTp_RxEventHandler(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr);

static FUNC(void, LINIF_CODE)
    LinTp_RxEventIndication(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr);

#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
#if !defined(LINTP_LOCAL)
#define LINTP_LOCAL static /* PRQA S 3414 */ /* VL_QAC_FctLikeMacro */
#endif

#define LINIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "LinIf_MemMap.h"
/* PRQA S 3218 ++ */ /* VL_LinTp_OnlyAccess */
LINTP_LOCAL VAR(LinTp_MasterRuntimeType, LINIF_VAR) LinTp_MasterRTData[LINTP_MASTER_CHANNEL_NUMBER];
LINTP_LOCAL VAR(LinTp_MasterRuntimeType, LINIF_VAR) LinTp_BackupMasterRtData[LINTP_MASTER_CHANNEL_NUMBER];

/* PRQA S 3218 -- */
#define LINIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "LinIf_MemMap.h"

/*******************************************************************************
**                      Macros Function Definitions                           **
*******************************************************************************/
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

static inline LinTp_MasterRuntimeType* LinTp_GetMasterRuntimeData(NetworkHandleType ch)
{
    return &LinTp_MasterRTData[ch];
}

static inline void LinTp_SetEvent(LinTp_MasterRuntimeType* tpChPtr, uint16 event)
{
    tpChPtr->SubEvent |= (event);
}

static inline void LinTp_ClrEvent(LinTp_MasterRuntimeType* tpChPtr, uint16 event)
{
    tpChPtr->SubEvent &= (uint16)(~event);
}

static inline boolean LinTp_IsEvent(const LinTp_MasterRuntimeType* tpChPtr, uint16 event)
{
    return (tpChPtr->SubEvent & event) != 0u;
}

static inline void LinTp_SetTrsEvent(LinTp_MasterRuntimeType* tpChPtr, uint8 event)
{
    tpChPtr->TrsEvent |= (event);
}

static inline void LinTp_ClrTrsEvent(LinTp_MasterRuntimeType* tpChPtr, uint16 event)
{
    tpChPtr->TrsEvent &= (uint16)(~event);
}

static inline boolean LinTp_IsTrsEvent(const LinTp_MasterRuntimeType* tpChPtr, uint8 event)
{
    return (tpChPtr->TrsEvent & event) != 0u;
}

/* Reset channel runtime data */
static inline void LinTp_MasterChReset(LinTp_MasterRuntimeType* tpChPtr)
{
    tpChPtr->LinTpChannelState      = LINTP_CHANNEL_IDLE;
    tpChPtr->TxNSduPtr              = NULL_PTR;
    tpChPtr->RxNSduPtr              = NULL_PTR;
    tpChPtr->SduRemaining           = 0u;
    tpChPtr->SduSize                = 0u;
    tpChPtr->SduIdx                 = 0u;
    tpChPtr->SduSN                  = 0u;
    tpChPtr->SubEvent               = LINTP_EVENT_NONE;
    tpChPtr->LastFrameType          = LINTP_FRAMETYPE_NONE;
    tpChPtr->TpTimer.EnabledTimer   = LINTP_TIMER_NONE;
    tpChPtr->TpP2Timer.EnabledTimer = LINTP_TIMER_NONE;
    tpChPtr->PendingFrameNum        = 0u;
    tpChPtr->MRFRequestedNad        = 0x00u;
    tpChPtr->MRFRequestedSID        = 0x00u;
    tpChPtr->BufReqNum              = 0x00u;
    LinTp_ClrTrsEvent(tpChPtr, LINTP_TRS_EVT_PHY_TX | LINTP_TRS_EVT_FUN_TX);
}

/* Construct SF */
static inline void LinTp_ConstructSF(LinTp_MasterRuntimeType* tpChPtr)
{
    tpChPtr->SduBuf[LINTP_PDU_OFS_NAD] = tpChPtr->TxNSduPtr->LinTpTxNSduNad;
    tpChPtr->SduBuf[LINTP_PDU_OFS_PCI] = (uint8)tpChPtr->SduSize;
    tpChPtr->SduIdx                    = LINTP_PDU_OFS_SF_DATA;
}

/* Construct FF */
static inline void LinTp_ConstructFF(LinTp_MasterRuntimeType* tpChPtr)
{
    tpChPtr->SduBuf[LINTP_PDU_OFS_NAD] = tpChPtr->TxNSduPtr->LinTpTxNSduNad;
    tpChPtr->SduBuf[LINTP_PDU_OFS_PCI] =
        LINTP_PDU_PCI_FF | (uint8)(tpChPtr->SduSize >> LINTP_PDU_DATESIZE_PCI_EXTERNINFO_OFFSET);
    tpChPtr->SduBuf[LINTP_PDU_OFS_LEN] = (uint8)(tpChPtr->SduSize & LINTP_SDU_SIZE_MSK);
    tpChPtr->SduIdx                    = LINTP_PDU_OFS_FF_DATA;
    tpChPtr->SduSN                     = 1u;
}

/* Construct CF */
static inline void LinTp_ConstructCF(LinTp_MasterRuntimeType* tpChPtr)
{
    tpChPtr->SduBuf[LINTP_PDU_OFS_NAD] = tpChPtr->TxNSduPtr->LinTpTxNSduNad;
    tpChPtr->SduBuf[LINTP_PDU_OFS_PCI] = LINTP_PDU_PCI_CF | tpChPtr->SduSN;
    tpChPtr->SduIdx                    = LINTP_PDU_OFS_CF_DATA;
    tpChPtr->SduSN++;
    tpChPtr->SduSN &= LINTP_PDU_PCI_SN_MASK;
}

#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */

/******************************************************************************/
/*
 * Brief               LinTp master node runtime data init
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE) LinTp_MasterInit(void)
{
    uint8 idx = LINTP_MASTER_CHANNEL_NUMBER;
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA)
    tpChPtr = LinTp_GetMasterRuntimeData(0u);

    /* Reset all of the channel */
    while (idx > 0u)
    {
        /*@req <SWS_LinIf_00320>,<SWS_LinIf_00710> */
        LinTp_MasterChReset(tpChPtr);
        tpChPtr->TrsEvent          = LINTP_TRS_EVT_NONE;
        tpChPtr->RecoverMode       = LINTP_APPLICATIVE_SCHEDULE;
        tpChPtr->NeedRestoreScence = FALSE;
        tpChPtr++; /* PRQA S 2983 */ /* VL_LinTp_PointerCalu */
        idx--;
    }
}

/******************************************************************************/
/*
 * Brief               LinTp master node channel runtime data init
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE)
LinTp_MasterChannelInit(NetworkHandleType LinIfChannelId)
{
    NetworkHandleType linTpChId = LinTp_GetLinTpChannel(LinIfChannelId);
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA)
    tpChPtr = LinTp_GetMasterRuntimeData(linTpChId);

    LinTp_MasterChReset(tpChPtr);
}

/******************************************************************************/
/*
 * Brief               Requests the transfer of segmented data.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ComMChannel: ComM Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              LinTp channel id
 */
/******************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE)
LinTp_MasterTransmit(
    P2CONST(LinTp_TxNSduType, AUTOMATIC, LINIF_APPL_CONST) txNSdu,
    P2CONST(PduInfoType, AUTOMATIC, LINIF_APPL_CONST) LinTpTxInfoPtr)
{
    NetworkHandleType linIfChId = txNSdu->LinTpLinIfChannelRef;
    NetworkHandleType linTpChId = LinTp_GetLinTpChannel(linIfChId);
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA)
    tpChPtr = LinTp_GetMasterRuntimeData(linTpChId);
    Std_ReturnType ret;

    /* Handle Functional Request*/
    if (LINTP_FUNCTIONAL_REQ_NAD == txNSdu->LinTpTxNSduNad)
    {
        if (LinTp_IsTrsEvent(tpChPtr, LINTP_TRS_EVT_FUN_TX_REQ | LINTP_TRS_EVT_FUN_TX))
        {
            ret = E_NOT_OK;
        }
        else
        {
            /*@req <SWS_LinIf_00413>*/
            /* Set the status of channel */
            tpChPtr->LinTpChannelState = LINTP_CHANNEL_BUSY;
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
        if (LinTp_IsTrsEvent(tpChPtr, LINTP_TRS_EVT_PHY_TX_REQ))
        {
            PduR_LinTpTxConfirmation(tpChPtr->PhyReqNSduPtr->LinTpTxNSduPduRef, E_NOT_OK);
        }

        /*@req <SWS_LinIf_00413>*/
        /* Set the status of channel */
        tpChPtr->LinTpChannelState = LINTP_CHANNEL_BUSY;
        /* Save transmit request information */
        tpChPtr->PhyReqNSduPtr          = txNSdu;
        tpChPtr->PhyReqSduSize          = LinTpTxInfoPtr->SduLength;
        tpChPtr->TpP2Timer.EnabledTimer = LINTP_TIMER_NONE;

        LinTp_SetTrsEvent(tpChPtr, LINTP_TRS_EVT_PHY_TX_REQ);

        ret = E_OK;
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               Shutdowns the LINTP master node
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE) LinTp_MasterShutdown(void)
{
    uint8 idx = LINTP_MASTER_CHANNEL_NUMBER;
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA)
    tpChPtr = LinTp_GetMasterRuntimeData(0u);

    /*@req <SWS_LinIf_00433> */
    /* Reset all of the channel */
    while (idx > 0u)
    {
        /*@req <SWS_LinIf_00356>*/
        LinTp_MasterChReset(tpChPtr);
        tpChPtr->TrsEvent = LINTP_TRS_EVT_NONE;
        /*@req <SWS_LinIf_00484>*/
        tpChPtr->LinTpChannelState = LINTP_UNINIT;
        tpChPtr++; /* PRQA S 2983 */ /* VL_LinTp_PointerCalu */
        idx--;
    }
}

/******************************************************************************/
/*
 * Brief               Rx success process in master node.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      LinIfChannelId: LinIf channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE)
LinTp_MasterTxSuccessHandle(NetworkHandleType LinIfChannelId)
{
    NetworkHandleType linTpChId = LinTp_GetLinTpChannel(LinIfChannelId);
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA)
    tpChPtr = LinTp_GetMasterRuntimeData(linTpChId);

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
        tpChPtr->TpTimer.Timer        = tpChPtr->TxNSduPtr->LinTpNcsCnt;
    }

    if (LinTp_IsEvent(tpChPtr, LINTP_EVENT_WAIT))
    {
        LinTp_ClrEvent(tpChPtr, LINTP_EVENT_WAIT);
    }
}

/******************************************************************************/
/*
 * Brief               Tx error process in master node.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      LinIfChannelId: LinIf channel
 *                     st: SRF frame
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
/* PRQA S 6070 ++ */ /* VL_MTR_LinTp_STCAL */
FUNC(void, LINIF_CODE)
LinTp_MasterTxErrorHandle(NetworkHandleType LinIfChannelId, Lin_StatusType st)
{
    NetworkHandleType linTpChId = LinTp_GetLinTpChannel(LinIfChannelId);
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA)
    tpChPtr = LinTp_GetMasterRuntimeData(linTpChId);

    if ((LinTp_IsEvent(tpChPtr, LINTP_EVENT_TX)) && ((st == LIN_TX_ERROR) || (st == LIN_TX_HEADER_ERROR)))
    {
        /*@req <SWS_LinIf_00069> */
        /* Notify upper(PDUR) */
        PduR_LinTpTxConfirmation(tpChPtr->TxNSduPtr->LinTpTxNSduPduRef, E_NOT_OK);
#if (STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT)
        if (TRUE == tpChPtr->ChCfgPtr->LinTpScheduleChangeDiag)
        {
            /*@req <SWS_LinIf_00673> */
            /* Change to the applicative schedule */
            BswM_LinTp_RequestMode(tpChPtr->TxNSduPtr->LinTpTxNSduChannelRef, LINTP_APPLICATIVE_SCHEDULE);
            tpChPtr->RecoverMode = LINTP_APPLICATIVE_SCHEDULE;
        }
#endif /* STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT */
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
/* PRQA S 6070 -- */

/******************************************************************************/
/*
 * Brief: Transmitting process
 * Param-Name[in]: ch
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
FUNC(void, LINIF_CODE) LinTp_TxProcess(NetworkHandleType lintpChannelId)
{
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA)
    tpChPtr = LinTp_GetMasterRuntimeData(lintpChannelId);

    /* Transmit confirmation */
    LinTp_TxEventConfirmation(tpChPtr);
}

/******************************************************************************/
/*
 * Brief: Receiving process
 * Param-Name[in]: ch
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
FUNC(void, LINIF_CODE) LinTp_RxProcess(NetworkHandleType lintpChannelId)
{
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA)
    tpChPtr = LinTp_GetMasterRuntimeData(lintpChannelId);

    /* Receive request */
    LinTp_RxEventRequest(tpChPtr);

    /* Receive handler */
    LinTp_RxEventHandler(tpChPtr);

    /* Receive indication */
    LinTp_RxEventIndication(tpChPtr);
}

/******************************************************************************/
/*
 * Brief               Handle the timer for specific channel.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx index of channel
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A.
 * PreCondition        Module initialized.
 * CallByAPI           This is a internal function
 */
/******************************************************************************/
/* PRQA S 6070 ++ */ /* VL_MTR_LinTp_STCAL */
FUNC(void, LINTP_CODE) LinTp_HandleTimers(NetworkHandleType LinIfChannelId, NetworkHandleType LinTpChannelId)
{
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA)
    tpChPtr = LinTp_GetMasterRuntimeData(LinTpChannelId);
    P2CONST(LinTp_RxNSduType, AUTOMATIC, LINIF_APPL_CONST) rx;
    NetworkHandleType network;
    NetworkHandleType linIfChannel;

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
                    PduR_LinTpTxConfirmation(tpChPtr->TxNSduPtr->LinTpTxNSduPduRef, E_NOT_OK);

                    network      = tpChPtr->TxNSduPtr->LinTpTxNSduChannelRef;
                    linIfChannel = tpChPtr->TxNSduPtr->LinTpLinIfChannelRef;
                }
                else
                {
                    /*@req <SWS_LinIf_00666>*/
                    PduR_LinTpRxIndication(tpChPtr->RxNSduPtr->LinTpRxNSduPduRef, E_NOT_OK);

                    network      = tpChPtr->RxNSduPtr->LinTpRxNSduChannelRef;
                    linIfChannel = tpChPtr->RxNSduPtr->LinTpLinIfChannelRef;
                }
#if (STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT)
                /*@req <SWS_LinIf_00646>*/
                if (tpChPtr->ChCfgPtr->LinTpScheduleChangeDiag)
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
    SchM_Enter_LinTp_ExclusiveArea_Channel();
    if (LINTP_TIMER_NONE != tpChPtr->TpP2Timer.EnabledTimer)
    {
        if (tpChPtr->TpP2Timer.Timer > 0u)
        {
            tpChPtr->TpP2Timer.Timer--;
            if (0u == tpChPtr->TpP2Timer.Timer)
            {
                /*@req <SWS_LinIf_00619>*/
                rx = LinTp_GetRxNSduByNad(LinIfChannelId, tpChPtr->MRFRequestedNad);
                PduR_LinTpRxIndication(rx->LinTpRxNSduPduRef, E_NOT_OK);
                linIfChannel = rx->LinTpLinIfChannelRef;
#if (STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT)
                /* Notify PduR,Change schedule table */
                /*@req <SWS_LinIf_00646>*/
                if (tpChPtr->ChCfgPtr->LinTpScheduleChangeDiag)
                {
                    network = rx->LinTpRxNSduChannelRef;
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
    SchM_Exit_LinTp_ExclusiveArea_Channel();
}
/* PRQA S 6070 -- */

/******************************************************************************/
/*
 * Brief: Get the LinIf channel corresponding LinTp channel
 * Param-Name[in]: id
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: LinTp Channel Id
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
FUNC(NetworkHandleType, LINIF_CODE) LinTp_GetLinTpChannel(NetworkHandleType LinIfChannelId)
{
    P2CONST(LinTp_ChannelConfigType, AUTOMATIC, LINIF_APPL_CONST)
    tpChCfgPtr = &LINTP_GET_CHANNEL_CONFIG(0u);
    NetworkHandleType ch;
    NetworkHandleType retLinTpChannelId = LINTP_NUMBER_OF_CHANNELS;

    for (ch = 0u; ch < LINTP_NUMBER_OF_CHANNELS; ch++)
    {
        if (tpChCfgPtr->LinTpLinIfChannelRef == LinIfChannelId)
        {
            retLinTpChannelId = ch;
            break;
        }

        tpChCfgPtr++;
    }
    return retLinTpChannelId;
}

/******************************************************************************/
/*
 * Brief: Parses a event of the receiving
 * Param-Name[in]: ch, sdu
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
/* PRQA S 6010,6020,6030 ++ */      /* VL_MTR_LinTp_STCYC,VL_MTR_LinTp_STLIN,VL_MTR_LinTp_STMIF */
/* PRQA S 6060,6050,6070,6080 ++ */ /* VL_MTR_LinTp_STM19,VL_MTR_LinTp_STST3,VL_MTR_LinTp_STCAL,VL_MTR_LinTp_STPTH */
FUNC(void, LINIF_CODE)
LinTp_RxEventParse(uint8 LinIfChannelId, P2CONST(uint8, AUTOMATIC, LINIF_APPL_CONST) sdu)
{
    NetworkHandleType linTpChId = LinTp_GetLinTpChannel(LinIfChannelId);
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA)
    tpChPtr = LinTp_GetMasterRuntimeData(linTpChId);
    P2CONST(LinTp_ChannelConfigType, AUTOMATIC, LINIF_APPL_CONST)
    chCfgPtr             = &LINTP_GET_CHANNEL_CONFIG(linTpChId);
    boolean rxCancelFlag = FALSE;
    P2CONST(LinTp_RxNSduType, AUTOMATIC, LINIF_APPL_CONST) rx;
    uint8 pciType = sdu[LINTP_PDU_OFS_PCI] & LINTP_PDU_PCI_MASK;

    if ((NULL_PTR == tpChPtr) || (NULL_PTR == chCfgPtr))
    {
        return;
    }

    rx = LinTp_GetRxNSduByNad(LinIfChannelId, tpChPtr->MRFRequestedNad);
    if (NULL_PTR == rx)
    {
        return;
    }

    if (LinTp_IsEvent(tpChPtr, LINTP_EVENT_WAIT))
    {
        LinTp_ClrEvent(tpChPtr, LINTP_EVENT_WAIT);
    }

    if ((LINTP_PDU_PCI_SF == pciType) && (LINTP_NEGATIVE_RSID == sdu[LINTP_PDU_OFS_SF_RSID])
        && (tpChPtr->MRFRequestedSID == sdu[LINTP_PDU_OFS_SF_NR_SID])
        && (LINTP_NRC_RESPONSE_PENDING == sdu[LINTP_PDU_OFS_SF_NR_NRC]))
    {
        tpChPtr->PendingFrameNum++;
        /*@req <SWS_LinIf_00623>*/
        if (tpChPtr->PendingFrameNum > chCfgPtr->LinTpMaxNumberOfRespPendingFrames)
        {
            rxCancelFlag = TRUE;
        }
        else
        {
            /*@req <SWS_LinIf_00621>*/
            /* Reload P2 timer with the time P2*max */
            tpChPtr->TpP2Timer.EnabledTimer = LINTP_TIMER_P2MAX;
            tpChPtr->TpP2Timer.Timer        = chCfgPtr->LinTpP2MaxCnt;

            LinTp_SetEvent(tpChPtr, LINTP_EVENT_UDS_PENDING);
        }
    }

    if (!rxCancelFlag)
    {
        /*@req <SWS_LinIf_00612>,<SWS_LinIf_00613>*/
        if ((chCfgPtr->LinTpDropNotRequestedNad) && (sdu[LINTP_PDU_OFS_NAD] != tpChPtr->MRFRequestedNad))
        {
            rxCancelFlag = TRUE;
        }
        else
        {
            if (!(LinTp_IsEvent(tpChPtr, LINTP_EVENT_TX)) && (tpChPtr->LinTpChannelState != LINTP_UNINIT))
            {
                boolean isCopy;
                boolean isIgnoreFrame = FALSE;
                uint8   bakupMRFRequestedNad;
                uint8   bakupMRFRequestedSID;

                switch (pciType)
                {
                /* SF */
                case LINTP_PDU_PCI_SF:
                    if ((LINTP_FRAMETYPE_CF == tpChPtr->LastFrameType)
                        || (LINTP_FRAMETYPE_FF == tpChPtr->LastFrameType))
                    {
                        /*@req <SWS_LinIf_00653>*/
                        PduR_LinTpRxIndication(rx->LinTpRxNSduPduRef, E_NOT_OK);

                        /* MRF Requested Nad and SID should not be cleared*/
                        bakupMRFRequestedNad = tpChPtr->MRFRequestedNad;
                        bakupMRFRequestedSID = tpChPtr->MRFRequestedSID;
                        /*@req <SWS_LinIf_00651>*/
                        /* Reset channel */
                        LinTp_MasterChReset(tpChPtr);
                        /* Recover Nad and SID */
                        tpChPtr->MRFRequestedNad = bakupMRFRequestedNad;
                        tpChPtr->MRFRequestedSID = bakupMRFRequestedSID;
                    }
                    /*@req <SWS_LinIf_00080>*/
                    isCopy = LinTp_IsRxSF(tpChPtr, rx, sdu, &isIgnoreFrame);
                    break;

                /* FF */
                case LINTP_PDU_PCI_FF:
                    if ((LINTP_FRAMETYPE_CF == tpChPtr->LastFrameType)
                        || (LINTP_FRAMETYPE_FF == tpChPtr->LastFrameType))
                    {
                        /*@req <SWS_LinIf_00653>*/
                        PduR_LinTpRxIndication(rx->LinTpRxNSduPduRef, E_NOT_OK);

                        /* MRF Requested Nad and SID should not be cleared*/
                        bakupMRFRequestedNad = tpChPtr->MRFRequestedNad;
                        bakupMRFRequestedSID = tpChPtr->MRFRequestedSID;
                        /*@req <SWS_LinIf_00651>*/
                        /* Reset channel */
                        LinTp_MasterChReset(tpChPtr);
                        /* Recover Nad and SID */
                        tpChPtr->MRFRequestedNad = bakupMRFRequestedNad;
                        tpChPtr->MRFRequestedSID = bakupMRFRequestedSID;
                    }
                    /*@req <SWS_LinIf_00080>*/
                    isCopy = LinTp_IsRxFF(tpChPtr, rx, sdu, &isIgnoreFrame);
                    break;

                /* CF */
                case LINTP_PDU_PCI_CF:
                    if (LINTP_FRAMETYPE_NONE == tpChPtr->LastFrameType)
                    {
                        /*@req <SWS_LinIf_00696>*/
                        isIgnoreFrame = TRUE;
                    }
                    else
                    {
                        isCopy = LinTp_IsRxCF(tpChPtr, sdu);
                    }
                    break;

                default:
                    /*@req <SWS_LinIf_00697>*/
                    isIgnoreFrame = TRUE;
                    break;
                }

                if (!isIgnoreFrame)
                {
                    /*@req <SWS_LinIf_00664>,<SWS_LinIf_00665>*/
                    /* Start the N_Cr timer(FF or CF),stop the last N_Cr */
                    if ((LINTP_PDU_PCI_FF == pciType) || (LINTP_PDU_PCI_CF == pciType))
                    {
                        tpChPtr->TpTimer.EnabledTimer = LINTP_TIMER_NCR;
                        tpChPtr->TpTimer.Timer        = rx->LinTpNcrCnt;
                    }

                    /*@req <SWS_LinIf_00617>,<SWS_LinIf_00618>*/
                    /* Stop P2/P2*max timer */
                    if (((LINTP_PDU_PCI_FF == pciType) || (LINTP_PDU_PCI_SF == pciType))
                        && !(LinTp_IsEvent(tpChPtr, LINTP_EVENT_UDS_PENDING)))
                    {
                        tpChPtr->TpP2Timer.EnabledTimer = LINTP_TIMER_NONE;
                    }

                    if (isCopy)
                    {
                        /* Data copy */
                        (void)ILib_memcpy(tpChPtr->SduBuf, sdu, LINTP_FRAME_LEN_MAX);

                        /* Set events */
                        LinTp_SetEvent(tpChPtr, LINTP_EVENT_RX_REQ);
                        /* PRQA S 4342 ++ */ /* VL_LinTp_4342 */
                        tpChPtr->LastFrameType = (LinTp_FrameType)pciType;
                        /* PRQA S 4342 -- */
                        tpChPtr->ChCfgPtr = chCfgPtr;
                    }
                    else
                    {
                        /* stop the current LIN TP message reception*/
                        rxCancelFlag = TRUE;
                    }
                }
            }
        }
    }

    if (rxCancelFlag)
    {
        /*@req <SWS_LinIf_00655>,<SWS_LinIf_00081>,<SWS_LinIf_00623>*/
        PduR_LinTpRxIndication(rx->LinTpRxNSduPduRef, E_NOT_OK);

        /* Reset channel */
        LinTp_MasterChReset(tpChPtr);

        /* Next entry */
        LinIf_MoveScheduleToNextEntry(rx->LinTpLinIfChannelRef);
#if (STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT)
        /*@req <SWS_LinIf_00646>*/
        if (chCfgPtr->LinTpScheduleChangeDiag)
        {
            /*@req <SWS_LinIf_00655>,<SWS_LinIf_00623>*/
            BswM_LinTp_RequestMode(rx->LinTpRxNSduChannelRef, LINTP_APPLICATIVE_SCHEDULE);
            tpChPtr->RecoverMode = LINTP_APPLICATIVE_SCHEDULE;
        }
#endif /* STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT */
    }
}
/* PRQA S 6010,6020,6030 -- */
/* PRQA S 6060,6050,6070,6080 -- */

/******************************************************************************/
/*
 * Brief: LinIf get MRF response from LinTp
 * Param-Name[in]: LinIfChannelId
 * Param-Name[out]: sduBufPtr
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
/* PRQA S 6070 ++ */ /* VL_MTR_LinTp_STCAL */
FUNC(Std_ReturnType, LINIF_CODE)
LinTp_MasterGetMRFResponse(NetworkHandleType LinIfChannelId, P2VAR(uint8, AUTOMATIC, LINIF_APPL_DATA) sduBufPtr)
{
    NetworkHandleType linTpChId = LinTp_GetLinTpChannel(LinIfChannelId);
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA)
    tpChPtr            = LinTp_GetMasterRuntimeData(linTpChId);
    Std_ReturnType ret = E_NOT_OK;
    uint8          pciType;

    if (!LinTp_TxEventRequest(tpChPtr))
    {
        return ret;
    }
    LinTp_TxEventHandler(tpChPtr);

    if ((LinTp_IsEvent(tpChPtr, LINTP_EVENT_TX_REQ)) && !(LinTp_IsEvent(tpChPtr, LINTP_EVENT_STOP_MRF)))
    {
        /* Set the flag of header and response */
        LinIf_SetEventByChId(LinIfChannelId, LINIF_EVENT_HEADER | LINIF_EVENT_RESPONSE);

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
                tpChPtr->TpP2Timer.Timer        = LINTP_GET_P2_TIME(linTpChId);
            }
        }

        /*@req <SWS_LinIf_00657>,<SWS_LinIf_00661>*/
        /* start the N_As timer(SF/FF/CF),stop the N_Cs timer(CF) */
        tpChPtr->TpTimer.EnabledTimer = LINTP_TIMER_NAS;
        tpChPtr->TpTimer.Timer        = tpChPtr->TxNSduPtr->LinTpNasCnt;

        /* Save MRF information(Except functional request) */
        if (LINTP_FUNCTIONAL_REQ_NAD != tpChPtr->SduBuf[LINTP_PDU_OFS_NAD])
        {
            pciType = tpChPtr->SduBuf[LINTP_PDU_OFS_PCI] & LINTP_PDU_PCI_MASK;
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

        (void)ILib_memcpy(sduBufPtr, tpChPtr->SduBuf, LINTP_FRAME_LEN_MAX);

        ret = E_OK;
    }

    return ret;
}
/* PRQA S 6070 -- */

/******************************************************************************/
/*
 * Brief: Get whether LINTP_EVENT_WAIT flag is set
 * Param-Name[in]: LinIfChannelId
 * Param-Name[out]: sduBufPtr
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
FUNC(boolean, LINIF_CODE)
LinTp_IsWaitEventSet(NetworkHandleType LinIfChannelId)
{
    NetworkHandleType              linTpChId = LinTp_GetLinTpChannel(LinIfChannelId);
    const LinTp_MasterRuntimeType* tpChPtr   = LinTp_GetMasterRuntimeData(linTpChId);

    return LinTp_IsEvent(tpChPtr, LINTP_EVENT_WAIT);
}

/******************************************************************************/
/*
 * Brief: Get whether LINTP_EVENT_TX or LINTP_EVENT_STOP_SRF flag is set
 * Param-Name[in]: LinIfChannelId
 * Param-Name[out]: sduBufPtr
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
FUNC(boolean, LINIF_CODE)
LinTp_IsStopSRFSendEventSet(uint8 LinIfChannelId)
{
    NetworkHandleType              linTpChId = LinTp_GetLinTpChannel(LinIfChannelId);
    const LinTp_MasterRuntimeType* tpChPtr   = LinTp_GetMasterRuntimeData(linTpChId);

    return LinTp_IsEvent(tpChPtr, LINTP_EVENT_TX | LINTP_EVENT_STOP_SRF);
}

/******************************************************************************/
/*
 * Brief: Get if LINTP_EVENT_STOP_SRF or LINTP_EVENT_STOP_MRF_SRF flag is set
 * Param-Name[in]: LinIfChannelId
 * Param-Name[out]: sduBufPtr
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
FUNC(boolean, LINIF_CODE)
LinTp_IsStopMRFOrSRFSendEventSet(NetworkHandleType LinIfChannelId)
{
    NetworkHandleType              linTpChId = LinTp_GetLinTpChannel(LinIfChannelId);
    const LinTp_MasterRuntimeType* tpChPtr   = LinTp_GetMasterRuntimeData(linTpChId);

    return LinTp_IsEvent(tpChPtr, LINTP_EVENT_STOP_SRF | LINTP_EVENT_STOP_MRF);
}

#if (STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT)
/******************************************************************************/
/*
 * Brief: Get LinTpScheduleChangeDiag parameter
 * Param-Name[in]: LinIfChannelId
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
FUNC(boolean, LINIF_CODE)
LinTp_GetScheduleChangeDiag(NetworkHandleType LinIfChannelId)
{
    NetworkHandleType linTpChId = LinTp_GetLinTpChannel(LinIfChannelId);
    P2CONST(LinTp_ChannelConfigType, AUTOMATIC, LINIF_APPL_CONST)
    tpChCfgPtr = &LINTP_GET_CHANNEL_CONFIG(linTpChId);

    return (tpChCfgPtr->LinTpScheduleChangeDiag);
}
#endif /* STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT */

/* PRQA S 1532 -- */

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
/******************************************************************************/
/*
 * Brief: Copy transfer data failure process
 * Param-Name[in]: ch
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE)
    LinTp_HandleCopyTxDataFailure(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr)
{
    /*@req <SWS_LinIf_00073>*/
    PduR_LinTpTxConfirmation(tpChPtr->TxNSduPtr->LinTpTxNSduPduRef, E_NOT_OK);

    if (LinTp_IsTrsEvent(tpChPtr, LINTP_TRS_EVT_FUN_TX))
    {
#if (STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT)
        if ((tpChPtr->ChCfgPtr->LinTpScheduleChangeDiag) && (tpChPtr->RecoverMode != LINTP_DIAG_REQUEST))
        {
            /*@req <SWS_LinIf_00673>*/
            BswM_LinTp_RequestMode(tpChPtr->TxNSduPtr->LinTpTxNSduChannelRef, tpChPtr->RecoverMode);
        }
#endif /* STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT */

        NetworkHandleType lintpCh = LinTp_GetLinTpChannel(tpChPtr->ChCfgPtr->LinTpLinIfChannelRef);
        LinTp_MasterChReset(tpChPtr);
        if (tpChPtr->NeedRestoreScence)
        {
            if (!LinTp_IsTrsEvent(tpChPtr, LINTP_TRS_EVT_PHY_TX_REQ))
            {
                LinTp_MasterRTData[lintpCh] = LinTp_BackupMasterRtData[lintpCh];
            }

            tpChPtr->LinTpChannelState = LINTP_CHANNEL_BUSY;
            tpChPtr->NeedRestoreScence = FALSE;
        }
    }
    else
    {
#if (STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT)
        if (tpChPtr->ChCfgPtr->LinTpScheduleChangeDiag)
        {
            /*@req <SWS_LinIf_00673>*/
            BswM_LinTp_RequestMode(tpChPtr->TxNSduPtr->LinTpTxNSduChannelRef, LINTP_APPLICATIVE_SCHEDULE);
            tpChPtr->RecoverMode = LINTP_APPLICATIVE_SCHEDULE;
        }
#endif /* STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT */
        /* Next entry */
        LinIf_MoveScheduleToNextEntry(tpChPtr->TxNSduPtr->LinTpLinIfChannelRef);
        /* Reset channel */
        LinTp_MasterChReset(tpChPtr);
    }
}

/******************************************************************************/
/*
 * Brief: Copy transfer data from the 'PduR'
 * Param-Name[in]: ch
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static FUNC(boolean, LINIF_CODE)
    LinTp_CopyTxDataFromPduR(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr)
{
    boolean result = FALSE;

    if (tpChPtr->BufReqNum > 0u)
    {
        PduInfoType infoData;
        P2VAR(PduInfoType, AUTOMATIC, LINIF_APPL_DATA) info = &infoData;
        BufReq_ReturnType bufRslt;
        PduLengthType     len = (LINTP_FRAME_LEN_MAX - tpChPtr->SduIdx);

        /* Get a copy of the effective length */
        len = (PduLengthType)LinIf_Min(len, tpChPtr->SduRemaining);

        /* Set destination buffer pointer and the number of bytes to be copied */
        info->SduDataPtr = &(tpChPtr->SduBuf[tpChPtr->SduIdx]);
        info->SduLength  = len;
        /*@req <SWS_LinIf_00671>,<SWS_LinIf_00705>*/
        /* Copy data from PduR */
        bufRslt =
            PduR_LinTpCopyTxData(tpChPtr->TxNSduPtr->LinTpTxNSduPduRef, info, NULL_PTR, &(tpChPtr->UpperBufRemaining));

        switch (bufRslt)
        {
        case BUFREQ_OK:
            /* Update buffer information */
            LinTp_ClrEvent(tpChPtr, ((uint16)(LINTP_EVENT_COPY_REQ) | (LINTP_EVENT_STOP_MRF)));

            tpChPtr->SduIdx += len;
            tpChPtr->SduRemaining -= len;
            /* reload retry counter */
            tpChPtr->BufReqNum = tpChPtr->TxNSduPtr->LinTpMaxBufReq;
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

/******************************************************************************/
/*
 * Brief: Requests a event of the transmitting
 * Param-Name[in]: ch
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static FUNC(boolean, LINIF_CODE)
    LinTp_TxEventRequest(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr)
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

/******************************************************************************/
/*
 * Brief: Handles a event of the transmitting
 * Param-Name[in]: ch
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE) LinTp_TxEventHandler(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr)
{
    PduLengthType len;

    if (!(LinTp_IsEvent(tpChPtr, LINTP_EVENT_TX)))
    {
        return;
    }

    if (!(LinTp_IsEvent(tpChPtr, LINTP_EVENT_TX_REQ | LINTP_EVENT_CONF | LINTP_EVENT_OK)))
    {
        /* PRQA S 2995 ++ */ /* VL_LinTp_Logical */
        /* FF/CF */
        if ((tpChPtr->SduRemaining != 0u) && (tpChPtr->SduIdx == LINTP_FRAME_LEN_MAX))
        {
            LinTp_SetEvent(tpChPtr, LINTP_EVENT_TX_REQ | LINTP_EVENT_CF_REQ);
        }
        /* SF/CF */
        else if ((tpChPtr->SduRemaining == 0u) && (tpChPtr->SduIdx != LINTP_FRAME_LEN_MAX))
        {
            /* Fill padding value */
            len = LINTP_FRAME_LEN_MAX - tpChPtr->SduIdx;
            (void)ILib_memset(&tpChPtr->SduBuf[tpChPtr->SduIdx], LINTP_PADDING_VALUE, len);
            LinTp_SetEvent(tpChPtr, LINTP_EVENT_TX_REQ | LINTP_EVENT_CONF_REQ);
        }
        /* SF/CF */
        else if ((tpChPtr->SduRemaining == 0u) && (tpChPtr->SduIdx == LINTP_FRAME_LEN_MAX))
        {
            LinTp_SetEvent(tpChPtr, LINTP_EVENT_TX_REQ | LINTP_EVENT_CONF_REQ);
        }
        /* SF/FF/CF */
        else
        {
            LinTp_SetEvent(tpChPtr, LINTP_EVENT_COPY_REQ);
        }
        /* PRQA S 2995 -- */
    }
}

/******************************************************************************/
/*
 * Brief: Confirmations a event of the transmitting
 * Param-Name[in]: ch
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE)
    LinTp_TxEventConfirmation(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr)
{
    LinTp_TimerType bakupTimerType;
    uint8           bakupMRFRequestedNad;
    uint8           bakupMRFRequestedSID;

    if ((LinTp_IsEvent(tpChPtr, LINTP_EVENT_OK)) && (LinTp_IsEvent(tpChPtr, LINTP_EVENT_TX)))
    {
        /*@req <SWS_LinIf_00068>*/
        /* Notify upper */
        PduR_LinTpTxConfirmation(tpChPtr->TxNSduPtr->LinTpTxNSduPduRef, E_OK);

        /*@req <SWS_LinIf_00646>*/
        if (LinTp_IsTrsEvent(tpChPtr, LINTP_TRS_EVT_FUN_TX))
        {
#if (STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT)
            if ((tpChPtr->ChCfgPtr->LinTpScheduleChangeDiag) && (tpChPtr->RecoverMode != LINTP_DIAG_REQUEST))
            {
                /*@req <SWS_LinIf_00707>*/
                BswM_LinTp_RequestMode(tpChPtr->TxNSduPtr->LinTpTxNSduChannelRef, tpChPtr->RecoverMode);
            }
#endif

            NetworkHandleType lintpCh = LinTp_GetLinTpChannel(tpChPtr->ChCfgPtr->LinTpLinIfChannelRef);
            LinTp_MasterChReset(tpChPtr);
            if (tpChPtr->NeedRestoreScence)
            {
                if (!LinTp_IsTrsEvent(tpChPtr, LINTP_TRS_EVT_PHY_TX_REQ))
                {
                    LinTp_MasterRTData[lintpCh] = LinTp_BackupMasterRtData[lintpCh];
                }

                tpChPtr->LinTpChannelState = LINTP_CHANNEL_BUSY;
                tpChPtr->NeedRestoreScence = FALSE;
            }
        }
        else
        {
#if (STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT)
            if (tpChPtr->ChCfgPtr->LinTpScheduleChangeDiag)
            {
                /*@req <SWS_LinIf_00642>*/
                BswM_LinTp_RequestMode(tpChPtr->TxNSduPtr->LinTpTxNSduChannelRef, LINTP_DIAG_RESPONSE);
                tpChPtr->RecoverMode = LINTP_DIAG_RESPONSE;
            }
#endif

            /* MRF send OK,P2 timer should not be cleared */
            bakupTimerType = tpChPtr->TpP2Timer.EnabledTimer;
            /* MRF Requested Nad and SID should not be cleared*/
            bakupMRFRequestedNad = tpChPtr->MRFRequestedNad;
            bakupMRFRequestedSID = tpChPtr->MRFRequestedSID;
            /* Reset channel */
            LinTp_MasterChReset(tpChPtr); /* PRQA S 2982 */ /* VL_LinTp_2982 */
            /* recover */
            tpChPtr->TpP2Timer.EnabledTimer = bakupTimerType;
            tpChPtr->MRFRequestedNad        = bakupMRFRequestedNad;
            tpChPtr->MRFRequestedSID        = bakupMRFRequestedSID;
        }
    }
}

/******************************************************************************/
/*
 * Brief: Receiving a signal frame
 * Param-Name[in]: tpChPtr, rx, sdu
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static FUNC(boolean, LINIF_CODE) LinTp_IsRxSF(
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr,
    P2CONST(LinTp_RxNSduType, AUTOMATIC, LINIF_APPL_CONST) rx,
    P2CONST(uint8, AUTOMATIC, LINIF_APPL_DATA) sdu,
    P2VAR(boolean, AUTOMATIC, LINIF_APPL_DATA) isIgnoreFramePtr)
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
        tpChPtr->LinTpChannelState = LINTP_CHANNEL_BUSY;

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

/******************************************************************************/
/*
 * Brief: Receiving a first frame
 * Param-Name[in]: tpChPtr, rx, sdu
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static FUNC(boolean, LINIF_CODE) LinTp_IsRxFF(
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr,
    P2CONST(LinTp_RxNSduType, AUTOMATIC, LINIF_APPL_CONST) rx,
    P2CONST(uint8, AUTOMATIC, LINIF_APPL_DATA) sdu,
    P2VAR(boolean, AUTOMATIC, LINIF_APPL_DATA) isIgnoreFramePtr)
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
        tpChPtr->LinTpChannelState = LINTP_CHANNEL_BUSY;

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

/******************************************************************************/
/*
 * Brief: Receiving a consecutive frame
 * Param-Name[in]: tpChPtr, rx, sdu
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static FUNC(boolean, LINIF_CODE) LinTp_IsRxCF(
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr,
    P2CONST(uint8, AUTOMATIC, LINIF_APPL_DATA) sdu)
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

/******************************************************************************/
/*
 * Brief: Requests a receive buffer from the 'PduR'
 * Param-Name[in]: ch
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE)
    LinTp_RxBufferRequestStart(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr)
{
    PduInfoType        infoData;
    BufReq_ReturnType  bufRslt;
    PduLengthType      len                             = (LINTP_FRAME_LEN_MAX - tpChPtr->SduIdx);
    const PduInfoType* infoDataPtr                     = &infoData;
    uint8              data[LINTP_METADATA_BUFFER_LEN] = {0};

    /* Get a copy of the effective length */
    len = (PduLengthType)LinIf_Min(len, tpChPtr->SduSize);

    infoData.MetaDataPtr = data;
    infoData.SduLength   = 0u;
    /*@req <SWS_LinIf_00075>*/
    /* Start copy data to PduR */
    bufRslt = PduR_LinTpStartOfReception(
        tpChPtr->RxNSduPtr->LinTpRxNSduPduRef,
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
        /* PRQA S 2995 ++ */ /* VL_LinTp_Logical */
        if ((BUFREQ_OK == bufRslt) && (tpChPtr->UpperBufRemaining < len))
        {
            /* Notify upper */
            PduR_LinTpRxIndication(tpChPtr->RxNSduPtr->LinTpRxNSduPduRef, E_NOT_OK);
        }
        /* PRQA S 2995 -- */
#if (STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT)
        if (tpChPtr->ChCfgPtr->LinTpScheduleChangeDiag)
        {
            BswM_LinTp_RequestMode(tpChPtr->RxNSduPtr->LinTpRxNSduChannelRef, LINTP_APPLICATIVE_SCHEDULE);
            tpChPtr->RecoverMode = LINTP_APPLICATIVE_SCHEDULE;
        }
#endif

        /* Clear flag of header */
        LinIf_ClearEvent(tpChPtr->RxNSduPtr->LinTpLinIfChannelRef, LINIF_EVENT_HEADER);
        /* Next entry */
        LinIf_MoveScheduleToNextEntry(tpChPtr->RxNSduPtr->LinTpLinIfChannelRef);

        /*@req <SWS_LinIf_00676>,<SWS_LinIf_00701> */
        /* Reset channel */
        LinTp_MasterChReset(tpChPtr);
    }
}

/******************************************************************************/
/*
 * Brief: Requests a receive buffer from the 'PduR'
 * Param-Name[in]: ch
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
/* PRQA S 6070 ++ */ /* VL_MTR_LinTp_STCAL */
static FUNC(void, LINIF_CODE) LinTp_RxBufferRequest(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr)
{
    PduInfoType infoData;
    P2VAR(PduInfoType, AUTOMATIC, LINIF_APPL_DATA) info = &infoData;
    BufReq_ReturnType bufRslt;
    PduLengthType     len                             = (LINTP_FRAME_LEN_MAX - tpChPtr->SduIdx);
    uint8             data[LINTP_METADATA_BUFFER_LEN] = {0};

    /* Get a copy of the effective length */
    len = (PduLengthType)LinIf_Min(len, tpChPtr->SduRemaining);

    /* Set destination buffer pointer and the number of bytes to be copied */
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
    bufRslt = PduR_LinTpCopyRxData(tpChPtr->RxNSduPtr->LinTpRxNSduPduRef, info, &(tpChPtr->UpperBufRemaining));

    /* Update buffer information */
    tpChPtr->SduRemaining -= info->SduLength;
    tpChPtr->SduIdx += info->SduLength;

    /*@req <SWS_LinIf_00677> */
    if (BUFREQ_E_NOT_OK == bufRslt)
    {
        /* Notify upper */
        PduR_LinTpRxIndication(tpChPtr->RxNSduPtr->LinTpRxNSduPduRef, E_NOT_OK);
#if (STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT)
        /*@req <SWS_LinIf_00646>*/
        if (TRUE == tpChPtr->ChCfgPtr->LinTpScheduleChangeDiag)
        {
            BswM_LinTp_RequestMode(tpChPtr->RxNSduPtr->LinTpRxNSduChannelRef, LINTP_APPLICATIVE_SCHEDULE);
            tpChPtr->RecoverMode = LINTP_APPLICATIVE_SCHEDULE;
        }
#endif /* STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT */

        /* Clear flag of header */
        LinIf_ClearEvent(tpChPtr->RxNSduPtr->LinTpLinIfChannelRef, LINIF_EVENT_HEADER);

        /* Next entry */
        LinIf_MoveScheduleToNextEntry(tpChPtr->RxNSduPtr->LinTpLinIfChannelRef);

        /* Reset channel */
        LinTp_MasterChReset(tpChPtr);
    }
    else
    {
        if (tpChPtr->SduRemaining == 0u)
        {
            LinTp_ClrEvent(tpChPtr, ((uint16)(LINTP_EVENT_RX_REQ) | (LINTP_EVENT_COPY_REQ)));
            LinTp_SetEvent(tpChPtr, LINTP_EVENT_OK);
        }
        else
        {
            /* The needed size of next CF */
            len = LINTP_FRAME_LEN_MAX - LINTP_PDU_OFS_CF_DATA;
            len = (PduLengthType)LinIf_Min(len, tpChPtr->SduRemaining);

            if (tpChPtr->UpperBufRemaining >= len)
            {
                LinTp_ClrEvent(tpChPtr, ((uint16)(LINTP_EVENT_RX_REQ) | (LINTP_EVENT_COPY_REQ)));
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
/* PRQA S 6070 -- */

/******************************************************************************/
/*
 * Brief: Requests a event of the receiving
 * Param-Name[in]: ch
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE) LinTp_RxEventRequest(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr)
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

        if ((TRUE == firstRequestBufFlag) && (LinTp_IsEvent(tpChPtr, LINTP_EVENT_COPY_REQ)))
        {
            /* Request a receiving buffer */
            LinTp_RxBufferRequestStart(tpChPtr);
        }
    }
}

/******************************************************************************/
/*
 * Brief: Handles a event of the receiving
 * Param-Name[in]: ch
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE) LinTp_RxEventHandler(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr)
{
    if ((LinTp_IsEvent(tpChPtr, LINTP_EVENT_RX_REQ)) && (LinTp_IsEvent(tpChPtr, LINTP_EVENT_COPY_REQ)))
    {
        /* Copy Rx data to PduR buffer */
        LinTp_RxBufferRequest(tpChPtr);
    }
}

/******************************************************************************/
/*
 * Brief: Indicates a event of the receiving
 * Param-Name[in]: ch
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE)
    LinTp_RxEventIndication(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr)
{
    if ((LinTp_IsEvent(tpChPtr, LINTP_EVENT_OK)) && (LinTp_IsEvent(tpChPtr, LINTP_EVENT_RX)))
    {
        /*@req <SWS_LinIf_00078>*/
        /* Notify upper */
        PduR_LinTpRxIndication(tpChPtr->RxNSduPtr->LinTpRxNSduPduRef, E_OK);
        if (LinTp_IsEvent(tpChPtr, LINTP_EVENT_UDS_PENDING))
        {
            tpChPtr->SubEvent = LINTP_EVENT_NONE;
            return;
        }

#if (STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT)
        /*@req <SWS_LinIf_00646>*/
        if (tpChPtr->ChCfgPtr->LinTpScheduleChangeDiag)
        {
            /*@req <SWS_LinIf_00643>*/
            BswM_LinTp_RequestMode(tpChPtr->RxNSduPtr->LinTpRxNSduChannelRef, LINTP_APPLICATIVE_SCHEDULE);
            tpChPtr->RecoverMode = LINTP_APPLICATIVE_SCHEDULE;
        }
#endif /* STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT */

        /* Next entry */
        LinIf_MoveScheduleToNextEntry(tpChPtr->RxNSduPtr->LinTpLinIfChannelRef);

        /* Reset channel */
        LinTp_MasterChReset(tpChPtr);
    }
}

/******************************************************************************/
/*
 * Brief: Load transmit request information
 * Param-Name[in]: ch
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
FUNC(void, LINIF_CODE)
LinTp_LoadTxRequest(NetworkHandleType ch)
{
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA)
    tpChPtr = LinTp_GetMasterRuntimeData(ch);

    /* If tx or rx is ongoing,shall not to load transmit request */
    if (LinTp_IsTrsEvent(tpChPtr, LINTP_TRS_EVT_FUN_TX_REQ | LINTP_TRS_EVT_PHY_TX_REQ))
    {
        P2CONST(LinTp_ChannelConfigType, AUTOMATIC, LINIF_APPL_CONST)
        chCfgPtr = &LINTP_GET_CHANNEL_CONFIG(ch);

        if (!LinIf_CheckEvent(chCfgPtr->LinTpLinIfChannelRef, LINIF_EVENT_SKIP_SLOT_TIMER))
        {
            LinIf_SetEventByChId(chCfgPtr->LinTpLinIfChannelRef, LINIF_EVENT_SKIP_SLOT_TIMER);
        }

        if (!LinIf_CheckEvent(chCfgPtr->LinTpLinIfChannelRef, LINIF_EVENT_HEADER))
        {
            LinIf_ClearEvent(chCfgPtr->LinTpLinIfChannelRef, LINIF_EVENT_SKIP_SLOT_TIMER);

            SchM_Enter_LinTp_ExclusiveArea_Channel();

            if ((tpChPtr->RecoverMode != LINTP_APPLICATIVE_SCHEDULE)
                && (LinTp_IsTrsEvent(tpChPtr, LINTP_TRS_EVT_FUN_TX_REQ)))
            {
                LinTp_BackupMasterRtData[ch] = LinTp_MasterRTData[ch];
                LinTp_BackupMasterRtData[ch].TrsEvent &= ~(LINTP_TRS_EVT_FUN_TX_REQ);
                LinTp_MasterChReset(tpChPtr);
                tpChPtr->LinTpChannelState = LINTP_CHANNEL_BUSY;
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
                    PduR_LinTpRxIndication(tpChPtr->RxNSduPtr->LinTpRxNSduPduRef, E_NOT_OK);
                }

                if (LinTp_IsEvent(tpChPtr, LINTP_EVENT_TX))
                {
                    PduR_LinTpTxConfirmation(tpChPtr->TxNSduPtr->LinTpTxNSduPduRef, E_NOT_OK);
                }

                /*@req <SWS_LinIf_00616>,<SWS_LinIf_00708>*/
                LinTp_MasterChReset(tpChPtr);
                tpChPtr->TxNSduPtr = tpChPtr->PhyReqNSduPtr;
                tpChPtr->SduSize   = tpChPtr->PhyReqSduSize;
#if (STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT)
                if (chCfgPtr->LinTpScheduleChangeDiag)
                {
                    tpChPtr->RecoverMode = LINTP_DIAG_REQUEST;
                }
#endif

                /* Set physic request ongoing flag */
                LinTp_ClrTrsEvent(tpChPtr, LINTP_TRS_EVT_PHY_TX_REQ);
                LinTp_SetTrsEvent(tpChPtr, LINTP_TRS_EVT_PHY_TX);
            }

            SchM_Exit_LinTp_ExclusiveArea_Channel();

            tpChPtr->ChCfgPtr     = chCfgPtr;
            tpChPtr->BufReqNum    = tpChPtr->TxNSduPtr->LinTpMaxBufReq;
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
            if (chCfgPtr->LinTpScheduleChangeDiag)
            {
                /*@req <SWS_LinIf_00641>*/
                BswM_LinTp_RequestMode(tpChPtr->TxNSduPtr->LinTpTxNSduChannelRef, LINTP_DIAG_REQUEST);
            }
#endif /* STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT */
        }
    }
}

#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

/* PRQA S 3432 -- */
/* PRQA S 2814,2824,2813,2889 -- */

#endif /* LINTP_MASTER_SUPPORT == STD_ON */
#endif /* LINIF_TP_SUPPORTED == STD_ON */
