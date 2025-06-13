/* PRQA S 3108++ */
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
/* PRQA S 3108-- */
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

/*******************************************************************************
**                             Version Check                                  **
*******************************************************************************/

/*******************************************************************************
**                            Macros Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Runtime Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

static FUNC(void, LINIF_CODE)
    LinTp_HandleCopyTxDataFailure(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr /* PRQA S 3432 */
    );

static FUNC(void, LINIF_CODE)
    LinTp_CopyTxDataFromPduR(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr /* PRQA S 3432 */
    );

static FUNC(void, LINIF_CODE)
    LinTp_TxEventRequest(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr /* PRQA S 3432 */
    );

static FUNC(void, LINIF_CODE)
    LinTp_TxEventHandler(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr /* PRQA S 3432 */
    );

static FUNC(void, LINIF_CODE)
    LinTp_TxEventConfirmation(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr /* PRQA S 3432 */
    );

static FUNC(boolean, LINIF_CODE) LinTp_IsRxSF(
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr, /* PRQA S 3432 */
    P2CONST(LinTp_RxNSduType, AUTOMATIC, LINIF_APPL_CONST) rx,
    P2CONST(uint8, AUTOMATIC, LINIF_APPL_DATA) sdu,
    P2VAR(boolean, AUTOMATIC, LINIF_APPL_DATA) isIgnoreFramePtr /* PRQA S 3432 */
);

static FUNC(boolean, LINIF_CODE) LinTp_IsRxFF(
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr, /* PRQA S 3432 */
    P2CONST(LinTp_RxNSduType, AUTOMATIC, LINIF_APPL_CONST) rx,
    P2CONST(uint8, AUTOMATIC, LINIF_APPL_DATA) sdu,
    P2VAR(boolean, AUTOMATIC, LINIF_APPL_DATA) isIgnoreFramePtr /* PRQA S 3432 */
);

static FUNC(boolean, LINIF_CODE) LinTp_IsRxCF(
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr, /* PRQA S 3432 */
    P2CONST(uint8, AUTOMATIC, LINIF_APPL_DATA) sdu);

static FUNC(P2CONST(LinTp_ChannelConfigType, AUTOMATIC, LINIF_APPL_CONST), LINIF_CODE)
    LinTp_GetChannelConfig(NetworkHandleType ch);

static FUNC(void, LINIF_CODE)
    LinTp_RxBufferRequestStart(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr /* PRQA S 3432 */
    );

static FUNC(void, LINIF_CODE)
    LinTp_RxBufferRequest(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr /* PRQA S 3432 */
    );

static FUNC(void, LINIF_CODE)
    LinTp_RxEventRequest(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr /* PRQA S 3432 */
    );

static FUNC(void, LINIF_CODE)
    LinTp_RxEventHandler(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr /* PRQA S 3432 */
    );

static FUNC(void, LINIF_CODE)
    LinTp_RxEventIndication(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr /* PRQA S 3432 */
    );

static FUNC(void, LINIF_CODE) LinTp_LoadTxRequest(
    NetworkHandleType ch,
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr /* PRQA S 3432 */
);

static FUNC(NetworkHandleType, LINIF_CODE) LinTp_GetLinTpChannel(NetworkHandleType LinIfChannelId);

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

LINTP_LOCAL VAR(LinTp_MasterRuntimeType, LINIF_VAR) LinTp_MasterRTData[LINTP_MASTER_CHANNEL_NUMBER];

#define LINIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "LinIf_MemMap.h"

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
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
FUNC(void, LINIF_CODE) LinTp_MasterInit(void) /* PRQA S 1532 */
{
    uint8 idx = LINTP_MASTER_CHANNEL_NUMBER;
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) /* PRQA S 3432 */
    tpChPtr = LINTP_MASTER_CH(0u);

    /* Reset all of the channel */
    while (idx > 0u) /* PRQA S 2877 */ /* MISRA Rule 4.1 */
    {
        /*@req <SWS_LinIf_00320>,<SWS_LinIf_00710> */
        LinTp_MasterChReset(tpChPtr);
        tpChPtr->TrsEvent = LINTP_TRS_EVT_NONE;
        tpChPtr++; /* PRQA S 2983 */

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
LinTp_MasterChannelInit(/* PRQA S 1532 */
                        NetworkHandleType LinIfChannelId)
{
    NetworkHandleType linTpChId = LinTp_GetLinTpChannel(LinIfChannelId);
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) /* PRQA S 3432 */
    tpChPtr = LINTP_MASTER_CH(linTpChId);

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
LinTp_MasterTransmit(/* PRQA S 1532 */
                     P2CONST(LinTp_TxNSduType, AUTOMATIC, LINIF_APPL_CONST) txNSdu,
                     P2CONST(PduInfoType, AUTOMATIC, LINIF_APPL_CONST) LinTpTxInfoPtr)
{
    NetworkHandleType linIfChId = txNSdu->LinTpLinIfChannelRef;
    NetworkHandleType linTpChId = LinTp_GetLinTpChannel(linIfChId);
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) /* PRQA S 3432 */
    tpChPtr = LINTP_MASTER_CH(linTpChId);
    Std_ReturnType ret;

    /* Handle Functional Request*/
    if (LINTP_FUNCTIONAL_REQ_NAD == txNSdu->LinTpTxNSduNad)
    {
        if (LINTP_IS_TRS_EVENT(LINTP_TRS_EVT_FUN_TX_REQ))
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

            LINTP_SET_TRS_EVENT(LINTP_TRS_EVT_FUN_TX_REQ);

            ret = E_OK;
        }
    }
    /* Handle Physic Request*/
    else
    {
        /*@req <SWS_LinIf_00616>,<SWS_LinIf_00708>*/
        if (LINTP_IS_TRS_EVENT(LINTP_TRS_EVT_PHY_TX))
        {
            /* Drop the old physical request */
            LinTp_MasterChReset(tpChPtr);
        }

        /*@req <SWS_LinIf_00413>*/
        /* Set the status of channel */
        tpChPtr->LinTpChannelState = LINTP_CHANNEL_BUSY;
        /* Save transmit request information */
        tpChPtr->PhyReqNSduPtr = txNSdu;
        tpChPtr->PhyReqSduSize = LinTpTxInfoPtr->SduLength;

        LINTP_SET_TRS_EVENT(LINTP_TRS_EVT_PHY_TX_REQ);

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
FUNC(void, LINIF_CODE) LinTp_MasterShutdown(void) /* PRQA S 1532 */
{
    uint8 idx = LINTP_MASTER_CHANNEL_NUMBER;
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) /* PRQA S 3432 */
    tpChPtr = LINTP_MASTER_CH(0u);

    /*@req <SWS_LinIf_00433> */
    /* Reset all of the channel */
    while (idx > 0u) /* PRQA S 2877 */ /* MISRA Rule 4.1 */
    {
        /*@req <SWS_LinIf_00356>*/
        LinTp_MasterChReset(tpChPtr);
        tpChPtr->TrsEvent = LINTP_TRS_EVT_NONE;
        /*@req <SWS_LinIf_00484>*/
        tpChPtr->LinTpChannelState = LINTP_UNINIT;
        tpChPtr++; /* PRQA S 2983 */

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
LinTp_MasterTxSuccessHandle(/* PRQA S 1532 */
                            NetworkHandleType LinIfChannelId)
{
    NetworkHandleType linTpChId = LinTp_GetLinTpChannel(LinIfChannelId);
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) /* PRQA S 3432 */
    tpChPtr = LINTP_MASTER_CH(linTpChId);

    /* Check whether clear confirmation event */
    if (LINTP_IS_EVENT(LINTP_EVENT_CONF))
    {
        LINTP_CLR_EVENT(LINTP_EVENT_CONF);
        LINTP_SET_EVENT(LINTP_EVENT_OK);
    }
    else
    {
        /*@req <SWS_LinIf_00657>,<SWS_LinIf_00661>*/
        /* Except last CF, need to start timer N_Cs and
            stop N_As */
        tpChPtr->TpTimer.EnabledTimer = LINTP_TIMER_NCS;
        tpChPtr->TpTimer.Timer = tpChPtr->TxNSduPtr->LinTpNcsCnt;
    }

    if (LINTP_IS_EVENT(LINTP_EVENT_WAIT))
    {
        LINTP_CLR_EVENT(LINTP_EVENT_WAIT);
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
FUNC(void, LINIF_CODE)
LinTp_MasterTxErrorHandle(/* PRQA S 1532 */
                          NetworkHandleType LinIfChannelId,
                          Lin_StatusType st)
{
    NetworkHandleType linTpChId = LinTp_GetLinTpChannel(LinIfChannelId);
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA)
    tpChPtr = LINTP_MASTER_CH(linTpChId);

    if ((LINTP_IS_EVENT(LINTP_EVENT_TX)) && ((st == LIN_TX_ERROR) || (st == LIN_TX_HEADER_ERROR)))
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
        LINTP_SET_EVENT(LINTP_EVENT_WAIT);
    }
}

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
FUNC(void, LINIF_CODE) LinTp_TxProcess(NetworkHandleType LinIfChannelId) /* PRQA S 1532 */
{
    NetworkHandleType linTpChId = LinTp_GetLinTpChannel(LinIfChannelId);
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) /* PRQA S 3432 */
    tpChPtr = LINTP_MASTER_CH(linTpChId);
    LinIf_FrameTypeType frameType;
    boolean entryDelayTimeout = LinIf_IsEntryDelayTimeout(LinIfChannelId);
    Std_ReturnType ret;

    if (TRUE == entryDelayTimeout)
    {
        ret = LinIf_GetCurFrameType(LinIfChannelId, &frameType);
        if ((E_OK == ret) && (LINIF_MRF == frameType))
        {
            /* Transmit request */
            LinTp_TxEventRequest(tpChPtr);

            /* Transmit handler */
            LinTp_TxEventHandler(tpChPtr);

            /* Transmit confirmation */
            LinTp_TxEventConfirmation(tpChPtr);
        }
    }

    /* Load transmit request */
    LinTp_LoadTxRequest(linTpChId, tpChPtr);
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
FUNC(void, LINIF_CODE) LinTp_RxProcess(NetworkHandleType LinIfChannelId) /* PRQA S 1532 */
{
    NetworkHandleType linTpChId = LinTp_GetLinTpChannel(LinIfChannelId);
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) /* PRQA S 3432 */
    tpChPtr = LINTP_MASTER_CH(linTpChId);

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
FUNC(void, LINTP_CODE) LinTp_HandleTimers(NetworkHandleType LinIfChannelId) /* PRQA S 1532 */
{
    NetworkHandleType linTpChId = LinTp_GetLinTpChannel(LinIfChannelId);
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA)
    tpChPtr = LINTP_MASTER_CH(linTpChId);
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

                    network = tpChPtr->TxNSduPtr->LinTpTxNSduChannelRef;
                    linIfChannel = tpChPtr->TxNSduPtr->LinTpLinIfChannelRef;
                }
                else
                {
                    /*@req <SWS_LinIf_00666>*/
                    PduR_LinTpRxIndication(tpChPtr->RxNSduPtr->LinTpRxNSduPduRef, E_NOT_OK);

                    network = tpChPtr->RxNSduPtr->LinTpRxNSduChannelRef;
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
FUNC(void, LINIF_CODE)
LinTp_RxEventParse(/* PRQA S 1532 */
                   uint8 LinIfChannelId,
                   P2CONST(uint8, AUTOMATIC, LINIF_APPL_CONST) sdu)
{
    NetworkHandleType linTpChId = LinTp_GetLinTpChannel(LinIfChannelId);
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA)
    tpChPtr = LINTP_MASTER_CH(linTpChId);
    P2CONST(LinTp_ChannelConfigType, AUTOMATIC, LINIF_APPL_CONST)
    chCfgPtr = &LINTP_GET_CHANNEL_CONFIG(linTpChId);
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

    if (LINTP_IS_EVENT(LINTP_EVENT_WAIT))
    {
        LINTP_CLR_EVENT(LINTP_EVENT_WAIT);
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
            tpChPtr->TpP2Timer.Timer = chCfgPtr->LinTpP2MaxCnt;

            LINTP_SET_EVENT(LINTP_EVENT_WAIT);
            /* Send a head again */
            LinIf_NextTransmit(LinIfChannelId);
            return;
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
            if (!(LINTP_IS_EVENT(LINTP_EVENT_TX)) && (tpChPtr->LinTpChannelState != LINTP_UNINIT))
            {
                boolean isCopy;
                boolean isIgnoreFrame = FALSE;
                uint8 bakupMRFRequestedNad;
                uint8 bakupMRFRequestedSID;

                switch (pciType)
                {
                /* SF */
                case LINTP_PDU_PCI_SF:
                    if (LINTP_FRAMETYPE_CF == tpChPtr->LastFrameType)
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
                    if (LINTP_FRAMETYPE_CF == tpChPtr->LastFrameType)
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
                        tpChPtr->TpTimer.Timer = rx->LinTpNcrCnt;
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
                        (void)ILib_memcpy(tpChPtr->SduBuf, sdu, LINTP_FRAME_LEN_MAX);

                        /* Set events */
                        LINTP_SET_EVENT(LINTP_EVENT_RX_REQ);

                        tpChPtr->LastFrameType = (LinTp_FrameType)pciType; /* PRQA S 4342 */
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
FUNC(Std_ReturnType, LINIF_CODE)
LinTp_MasterGetMRFResponse(/* PRQA S 1532 */
                           NetworkHandleType LinIfChannelId,
                           P2VAR(uint8, AUTOMATIC, LINIF_APPL_DATA) sduBufPtr /* PRQA S 3432 */
)
{
    NetworkHandleType linTpChId = LinTp_GetLinTpChannel(LinIfChannelId);
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) /* PRQA S 3432 */
    tpChPtr = LINTP_MASTER_CH(linTpChId);
    Std_ReturnType ret = E_NOT_OK;
    uint8 pciType;

    if ((LINTP_IS_EVENT(LINTP_EVENT_TX_REQ)) && !(LINTP_IS_EVENT(LINTP_EVENT_STOP_MRF)))
    {
        /* Set the flag of header and response */
        LinIf_SetEvent(LinIfChannelId, LINIF_EVENT_HEADER | LINIF_EVENT_RESPONSE);

        /* Clear TX request events */
        LINTP_CLR_EVENT(LINTP_EVENT_TX_REQ);

        /* Check whether set confirmation event */
        if (LINTP_IS_EVENT(LINTP_EVENT_CONF_REQ))
        {
            LINTP_CLR_EVENT(LINTP_EVENT_CONF_REQ);
            LINTP_SET_EVENT(LINTP_EVENT_CONF);

            /*@req <SWS_LinIf_00617>,<SWS_LinIf_00618>*/
            /* Send last MRF,start P2 Timer */
            /* But If current MRF is functional request,Need't start
               P2(functional request no response) */
            if (LINTP_FUNCTIONAL_REQ_NAD != tpChPtr->SduBuf[LINTP_PDU_OFS_NAD])
            {
                tpChPtr->TpP2Timer.EnabledTimer = LINTP_TIMER_P2;
                tpChPtr->TpP2Timer.Timer = LINTP_GET_P2_TIME(linTpChId);
            }
        }

        /*@req <SWS_LinIf_00657>,<SWS_LinIf_00661>*/
        /* start the N_As timer(SF/FF/CF),stop the N_Cs timer(CF) */
        tpChPtr->TpTimer.EnabledTimer = LINTP_TIMER_NAS;
        tpChPtr->TpTimer.Timer = tpChPtr->TxNSduPtr->LinTpNasCnt;

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
LinTp_IsWaitEventSet(/* PRQA S 1532 */
                     NetworkHandleType LinIfChannelId)
{
    NetworkHandleType linTpChId = LinTp_GetLinTpChannel(LinIfChannelId);
    const LinTp_MasterRuntimeType* tpChPtr = LINTP_MASTER_CH(linTpChId);

    return LINTP_IS_EVENT(LINTP_EVENT_WAIT);
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
LinTp_IsStopSRFSendEventSet(/* PRQA S 1532 */
                            uint8 LinIfChannelId)
{
    NetworkHandleType linTpChId = LinTp_GetLinTpChannel(LinIfChannelId);
    const LinTp_MasterRuntimeType* tpChPtr = LINTP_MASTER_CH(linTpChId);

    return LINTP_IS_EVENT(LINTP_EVENT_TX | LINTP_EVENT_STOP_SRF);
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
LinTp_IsStopMRFOrSRFSendEventSet(/* PRQA S 1532 */
                                 NetworkHandleType LinIfChannelId)
{
    NetworkHandleType linTpChId = LinTp_GetLinTpChannel(LinIfChannelId);
    const LinTp_MasterRuntimeType* tpChPtr = LINTP_MASTER_CH(linTpChId);

    return LINTP_IS_EVENT(LINTP_EVENT_STOP_SRF | LINTP_EVENT_STOP_MRF);
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
LinTp_GetScheduleChangeDiag(/* PRQA S 1532 */
                            NetworkHandleType LinIfChannelId)
{
    NetworkHandleType linTpChId = LinTp_GetLinTpChannel(LinIfChannelId);
    P2CONST(LinTp_ChannelConfigType, AUTOMATIC, LINIF_APPL_CONST)
    tpChCfgPtr = &LINTP_GET_CHANNEL_CONFIG(linTpChId);

    return (tpChCfgPtr->LinTpScheduleChangeDiag);
}
#endif /* STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT */

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
static FUNC(void, LINIF_CODE)
    LinTp_CopyTxDataFromPduR(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr /* PRQA S 3432 */
    )
{
    if (tpChPtr->BufReqNum > 0u)
    {
        PduInfoType infoData;
        P2VAR(PduInfoType, AUTOMATIC, LINIF_APPL_DATA) info = &infoData; /* PRQA S 3432 */
        BufReq_ReturnType bufRslt;
        PduLengthType len = (LINTP_FRAME_LEN_MAX - tpChPtr->SduIdx);

        /* Get a copy of the effective length */
        len = LINIF_MIN(len, tpChPtr->SduRemaining);

        /* Set destination buffer pointer and he number of bytes to be copied */
        info->SduDataPtr = &(tpChPtr->SduBuf[tpChPtr->SduIdx]);
        info->SduLength = len;
        /*@req <SWS_LinIf_00671>,<SWS_LinIf_00705>*/
        /* Copy data from PduR */
        bufRslt =
            PduR_LinTpCopyTxData(tpChPtr->TxNSduPtr->LinTpTxNSduPduRef, info, NULL_PTR, &(tpChPtr->UpperBufRemaining));

        switch (bufRslt)
        {
        case BUFREQ_OK:
            /* Update buffer information */
            LINTP_CLR_EVENT(LINTP_EVENT_COPY_REQ | LINTP_EVENT_STOP_MRF);

            tpChPtr->SduIdx += len;
            tpChPtr->SduRemaining -= len;
            /* reload retry counter */
            tpChPtr->BufReqNum = tpChPtr->TxNSduPtr->LinTpMaxBufReq;

            break;

        case BUFREQ_E_NOT_OK:
            LinTp_HandleCopyTxDataFailure(tpChPtr);
            break;

        case BUFREQ_E_BUSY:
            /* retry to copy data */
            /* stop to send MRF until copy data successful */
            LINTP_SET_EVENT(LINTP_EVENT_STOP_MRF);
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
static FUNC(void, LINIF_CODE)
    LinTp_TxEventRequest(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr /* PRQA S 3432 */
    )
{
    if (!(LINTP_IS_EVENT(LINTP_EVENT_TX)))
    {
        return;
    }

    if (!(LINTP_IS_EVENT(LINTP_EVENT_CONF | LINTP_EVENT_OK)))
    {
        /* SF */
        if (LINTP_IS_EVENT(LINTP_EVENT_SF_REQ))
        {
            LINTP_CLR_EVENT(LINTP_EVENT_SF_REQ);
            LinTp_ConstructSF(tpChPtr);
        }
        /* FF */
        else if (LINTP_IS_EVENT(LINTP_EVENT_FF_REQ))
        {
            LINTP_CLR_EVENT(LINTP_EVENT_FF_REQ);
            LinTp_ConstructFF(tpChPtr);
        }
        /* CF */
        else if (!(LINTP_IS_EVENT(LINTP_EVENT_TX_REQ)) && (LINTP_IS_EVENT(LINTP_EVENT_CF_REQ)))
        {
            LINTP_CLR_EVENT(LINTP_EVENT_CF_REQ);
            LINTP_SET_EVENT(LINTP_EVENT_COPY_REQ);
            LinTp_ConstructCF(tpChPtr);
        }
        else
        {
            /* only for the MISRA-C */
        }

        if (LINTP_IS_EVENT(LINTP_EVENT_COPY_REQ))
        {
            /* Copy data from PduR */
            LinTp_CopyTxDataFromPduR(tpChPtr);
        }
    }
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
static FUNC(void, LINIF_CODE)
    LinTp_TxEventHandler(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr /* PRQA S 3432 */
    )
{
    PduLengthType len;

    if (!(LINTP_IS_EVENT(LINTP_EVENT_TX)))
    {
        return;
    }

    if (!(LINTP_IS_EVENT(LINTP_EVENT_TX_REQ | LINTP_EVENT_CONF | LINTP_EVENT_OK)))
    {
        /* FF/CF */
        if ((tpChPtr->SduRemaining != 0u) && (tpChPtr->SduIdx == LINTP_FRAME_LEN_MAX))
        {
            LINTP_SET_EVENT(LINTP_EVENT_TX_REQ | LINTP_EVENT_CF_REQ);
        }
        /* SF/CF */
        else if ((tpChPtr->SduRemaining == 0u) && (tpChPtr->SduIdx != LINTP_FRAME_LEN_MAX))
        {
            /* Fill padding value */
            len = LINTP_FRAME_LEN_MAX - tpChPtr->SduIdx;
            (void)ILib_memset(&tpChPtr->SduBuf[tpChPtr->SduIdx], LINTP_PADDING_VALUE, len);
            LINTP_SET_EVENT(LINTP_EVENT_TX_REQ | LINTP_EVENT_CONF_REQ);
        }
        /* SF/CF */
        else if ((tpChPtr->SduRemaining == 0u) && (tpChPtr->SduIdx == LINTP_FRAME_LEN_MAX)) /* PRQA S 2995 */
        {
            LINTP_SET_EVENT(LINTP_EVENT_TX_REQ | LINTP_EVENT_CONF_REQ);
        }
        /* SF/FF/CF */
        else
        {
            LINTP_SET_EVENT(LINTP_EVENT_COPY_REQ);
        }
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
    uint8 bakupMRFRequestedNad;
    uint8 bakupMRFRequestedSID;

    if ((LINTP_IS_EVENT(LINTP_EVENT_OK)) && (LINTP_IS_EVENT(LINTP_EVENT_TX)))
    {
        /*@req <SWS_LinIf_00068>*/
        /* Notify upper */
        PduR_LinTpTxConfirmation(tpChPtr->TxNSduPtr->LinTpTxNSduPduRef, E_OK);
#if (STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT)
        /*@req <SWS_LinIf_00646>*/
        if (tpChPtr->ChCfgPtr->LinTpScheduleChangeDiag)
        {
            if (LINTP_IS_TRS_EVENT(LINTP_TRS_EVT_FUN_TX))
            {
                /*@req <SWS_LinIf_00707>*/
                BswM_LinTp_RequestMode(tpChPtr->TxNSduPtr->LinTpTxNSduChannelRef, tpChPtr->RecoverMode);
            }
            else
            {
                /*@req <SWS_LinIf_00642>*/
                BswM_LinTp_RequestMode(tpChPtr->TxNSduPtr->LinTpTxNSduChannelRef, LINTP_DIAG_RESPONSE);
                tpChPtr->RecoverMode = LINTP_DIAG_RESPONSE;
            }
        }
#endif

        /* Next entry */
        LinIf_MoveScheduleToNextEntry(tpChPtr->TxNSduPtr->LinTpLinIfChannelRef);

        /* MRF send OK,P2 timer should not be cleared */
        bakupTimerType = tpChPtr->TpP2Timer.EnabledTimer;
        /* MRF Requested Nad and SID should not be cleared*/
        bakupMRFRequestedNad = tpChPtr->MRFRequestedNad;
        bakupMRFRequestedSID = tpChPtr->MRFRequestedSID;
        /* Reset channel */
        LinTp_MasterChReset(tpChPtr); /* PRQA S 2982 */ /* MISRA Rule 2.2 */
        /* recover */
        tpChPtr->TpP2Timer.EnabledTimer = bakupTimerType;
        tpChPtr->MRFRequestedNad = bakupMRFRequestedNad;
        tpChPtr->MRFRequestedSID = bakupMRFRequestedSID;
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
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr, /* PRQA S 3432 */
    P2CONST(LinTp_RxNSduType, AUTOMATIC, LINIF_APPL_CONST) rx,
    P2CONST(uint8, AUTOMATIC, LINIF_APPL_DATA) sdu,
    P2VAR(boolean, AUTOMATIC, LINIF_APPL_DATA) isIgnoreFramePtr /* PRQA S 3432 */
)
{
    boolean isCopy = FALSE;

    /* Update buffer information */
    tpChPtr->SduSize = (PduLengthType)sdu[LINTP_PDU_OFS_PCI] & LINTP_PDU_PCI_DL_MASK;
    tpChPtr->SduRemaining = tpChPtr->SduSize;
    tpChPtr->RxNSduPtr = rx;

    if ((tpChPtr->SduSize > LINTP_LENGTH_SF_MIN) && (tpChPtr->SduSize <= LINTP_LENGTH_SF_MAX))
    {
        /* Set events */
        LINTP_SET_EVENT(LINTP_EVENT_RX | LINTP_EVENT_SF_REQ | LINTP_EVENT_COPY_REQ);
        /* Do not need to send a header again */
        LINTP_SET_EVENT(LINTP_EVENT_STOP_SRF);

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
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr, /* PRQA S 3432 */
    P2CONST(LinTp_RxNSduType, AUTOMATIC, LINIF_APPL_CONST) rx,
    P2CONST(uint8, AUTOMATIC, LINIF_APPL_DATA) sdu,
    P2VAR(boolean, AUTOMATIC, LINIF_APPL_DATA) isIgnoreFramePtr /* PRQA S 3432 */
)
{
    boolean isCopy = FALSE;

    /* Update buffer information */
    tpChPtr->SduSize = (PduLengthType)sdu[LINTP_PDU_OFS_PCI] & LINTP_PDU_PCI_DL_MASK;
    tpChPtr->SduSize <<= 8u;
    tpChPtr->SduSize |= sdu[LINTP_PDU_OFS_LEN];
    tpChPtr->SduRemaining = tpChPtr->SduSize;
    tpChPtr->RxNSduPtr = rx;

    /*@req <SWS_LinIf_00652>*/
    if (tpChPtr->SduSize >= LINTP_LENGTH_FF_MIN)
    {
        /* Set events */
        LINTP_SET_EVENT(LINTP_EVENT_RX | LINTP_EVENT_FF_REQ | LINTP_EVENT_COPY_REQ);

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
    P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr, /* PRQA S 3432 */
    P2CONST(uint8, AUTOMATIC, LINIF_APPL_DATA) sdu)
{
    boolean isCopy = FALSE;

    /* Update serial number */
    tpChPtr->SduSN++;
    tpChPtr->SduSN &= LINTP_PDU_PCI_SN_MASK;

    if ((LINTP_IS_EVENT(LINTP_EVENT_RX)) && ((sdu[LINTP_PDU_OFS_PCI] & LINTP_PDU_PCI_SN_MASK) == tpChPtr->SduSN))
    {
        /* Set events */
        LINTP_SET_EVENT(LINTP_EVENT_CF_REQ | LINTP_EVENT_COPY_REQ);

        /* Check Whether need to send a header again */
        if (tpChPtr->SduRemaining <= (LINTP_FRAME_LEN_MAX - LINTP_PDU_OFS_SF_DATA))
        {
            /* Do not need to send a header again */
            LINTP_SET_EVENT(LINTP_EVENT_STOP_SRF);
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
 * Brief: Gets the LinTp channel corresponding to LinIf channel
 * Param-Name[in]: ch: LinIf channel
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: const LinTp_ChannelConfigType*
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static FUNC(P2CONST(LinTp_ChannelConfigType, AUTOMATIC, LINIF_APPL_CONST), LINIF_CODE)
    LinTp_GetChannelConfig(NetworkHandleType ch)
{
    P2CONST(LinTp_ChannelConfigType, AUTOMATIC, LINIF_APPL_CONST)
    chCfgPtr = &LINTP_GET_CHANNEL_CONFIG(0);
    NetworkHandleType idx = LINIF_NUMBER_OF_CHANNELS;

    while (idx > 0u)
    {
        if (chCfgPtr->LinTpLinIfChannelRef == ch)
        {
            return chCfgPtr;
        }
        chCfgPtr++;

        idx--;
    }

    return NULL_PTR;
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
    PduInfoType infoData;
    BufReq_ReturnType bufRslt;
    PduLengthType len = (LINTP_FRAME_LEN_MAX - tpChPtr->SduIdx);
    const PduInfoType* infoDataPtr = &infoData;
    uint8 data[5] = {0};

    /* Get a copy of the effective length */
    len = LINIF_MIN(len, tpChPtr->SduSize);

    infoData.MetaDataPtr = data;
    infoData.SduLength = 0u;
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
        if ((BUFREQ_OK == bufRslt) && (tpChPtr->UpperBufRemaining < len)) /* PRQA S 2995 */
        {
            /* Notify upper */
            PduR_LinTpRxIndication(tpChPtr->RxNSduPtr->LinTpRxNSduPduRef, E_NOT_OK);
        }
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
static FUNC(void, LINIF_CODE) LinTp_RxBufferRequest(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr)
{
    PduInfoType infoData;
    P2VAR(PduInfoType, AUTOMATIC, LINIF_APPL_DATA) info = &infoData;
    BufReq_ReturnType bufRslt;
    PduLengthType len = (LINTP_FRAME_LEN_MAX - tpChPtr->SduIdx);
    uint8 data[5] = {0};

    /* Get a copy of the effective length */
    len = LINIF_MIN(len, tpChPtr->SduRemaining);

    /* Set destination buffer pointer and the number of bytes to be copied */
    info->SduDataPtr = &(tpChPtr->SduBuf[tpChPtr->SduIdx]);
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
            LINTP_CLR_EVENT(LINTP_EVENT_RX_REQ | LINTP_EVENT_COPY_REQ);
            LINTP_SET_EVENT(LINTP_EVENT_OK);
        }
        else
        {
            /* The needed size of next CF */
            len = (LINTP_FRAME_LEN_MAX - LINTP_PDU_OFS_CF_DATA);
            len = LINIF_MIN(len, tpChPtr->SduRemaining);

            if (tpChPtr->UpperBufRemaining >= len)
            {
                LINTP_CLR_EVENT(LINTP_EVENT_RX_REQ | LINTP_EVENT_COPY_REQ);
                if (info->SduLength == 0u)
                {
                    /*@req <SWS_LinIf_00680> */
                    /* Resume send header */
                    LINTP_CLR_EVENT(LINTP_EVENT_STOP_SRF);
                }
            }
            else
            {
                /* Retry to request buffer */
                LINTP_SET_EVENT(LINTP_EVENT_CF_REQ);
                if (info->SduLength != 0u)
                {
                    /*@req <SWS_LinIf_00679> */
                    /* Stop send header */
                    LINTP_SET_EVENT(LINTP_EVENT_STOP_SRF);
                }
            }
        }
    }
}

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
static FUNC(void, LINIF_CODE)
    LinTp_RxEventRequest(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr /* PRQA S 3432 */
    )
{
    boolean firstRequestBufFlag = FALSE;

    if (LINTP_IS_EVENT(LINTP_EVENT_RX_REQ))
    {
        /* SF */
        if (LINTP_IS_EVENT(LINTP_EVENT_SF_REQ))
        {
            LINTP_CLR_EVENT(LINTP_EVENT_SF_REQ);
            tpChPtr->SduIdx = LINTP_PDU_OFS_SF_DATA;
            firstRequestBufFlag = TRUE;
        }
        /* FF */
        else if (LINTP_IS_EVENT(LINTP_EVENT_FF_REQ))
        {
            LINTP_CLR_EVENT(LINTP_EVENT_FF_REQ);
            tpChPtr->SduIdx = LINTP_PDU_OFS_FF_DATA;
            tpChPtr->SduSN = 0u;
            firstRequestBufFlag = TRUE;
        }
        /* CF */
        else if (LINTP_IS_EVENT(LINTP_EVENT_CF_REQ))
        {
            LINTP_CLR_EVENT(LINTP_EVENT_CF_REQ);
            tpChPtr->SduIdx = LINTP_PDU_OFS_CF_DATA;
        }
        else
        {
            /* only for the MISRA-C */
        }

        if ((TRUE == firstRequestBufFlag) && (LINTP_IS_EVENT(LINTP_EVENT_COPY_REQ)))
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
static FUNC(void, LINIF_CODE)
    LinTp_RxEventHandler(P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr /* PRQA S 3432 */
    )
{
    if ((LINTP_IS_EVENT(LINTP_EVENT_RX_REQ)) && (LINTP_IS_EVENT(LINTP_EVENT_COPY_REQ)))
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
    if ((LINTP_IS_EVENT(LINTP_EVENT_OK)) && (LINTP_IS_EVENT(LINTP_EVENT_RX)))
    {
        /*@req <SWS_LinIf_00078>*/
        /* Notify upper */
        PduR_LinTpRxIndication(tpChPtr->RxNSduPtr->LinTpRxNSduPduRef, E_OK);
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
static FUNC(void, LINIF_CODE)
    LinTp_LoadTxRequest(NetworkHandleType ch, P2VAR(LinTp_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) tpChPtr)
{
    /* If tx or rx is ongoing,shall not to load transmit request */
    if (!(LINTP_IS_EVENT(LINTP_EVENT_TX | LINTP_EVENT_RX))
        && (LINTP_IS_TRS_EVENT(LINTP_TRS_EVT_FUN_TX_REQ | LINTP_TRS_EVT_PHY_TX_REQ)))
    {
        P2CONST(LinTp_ChannelConfigType, AUTOMATIC, LINIF_APPL_CONST)
        chCfgPtr = LinTp_GetChannelConfig(ch);

        if (LINTP_IS_TRS_EVENT(LINTP_TRS_EVT_FUN_TX_REQ))
        {
            tpChPtr->TxNSduPtr = tpChPtr->FuncReqNSduPtr;
            tpChPtr->SduSize = tpChPtr->FuncReqSduSize;

            /* Set functional request ongoing flag */
            LINTP_CLR_TRS_EVENT(LINTP_TRS_EVT_FUN_TX_REQ);
            LINTP_SET_TRS_EVENT(LINTP_TRS_EVT_FUN_TX);
        }
        else
        {
            tpChPtr->TxNSduPtr = tpChPtr->PhyReqNSduPtr;
            tpChPtr->SduSize = tpChPtr->PhyReqSduSize;

            /* Set physic request ongoing flag */
            LINTP_CLR_TRS_EVENT(LINTP_TRS_EVT_PHY_TX_REQ);
            LINTP_SET_TRS_EVENT(LINTP_TRS_EVT_PHY_TX);
        }
        tpChPtr->ChCfgPtr = chCfgPtr;
        tpChPtr->BufReqNum = tpChPtr->TxNSduPtr->LinTpMaxBufReq;
        tpChPtr->SduRemaining = tpChPtr->SduSize;

        /* Last Tx confirmation process may change channel state to IDLE->LINTP_MASTER_CH_RESET(tpChPtr)*/
        tpChPtr->LinTpChannelState = LINTP_CHANNEL_BUSY;

        /* Set transmit events */
        LINTP_SET_EVENT(LINTP_EVENT_TX | LINTP_EVENT_COPY_REQ);
        if (tpChPtr->SduSize > (LINTP_FRAME_LEN_MAX - LINTP_PDU_OFS_SF_DATA))
        {
            LINTP_SET_EVENT(LINTP_EVENT_FF_REQ);
        }
        else
        {
            LINTP_SET_EVENT(LINTP_EVENT_SF_REQ);
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
static FUNC(NetworkHandleType, LINIF_CODE) LinTp_GetLinTpChannel(NetworkHandleType LinIfChannelId)
{
    P2CONST(LinTp_ChannelConfigType, AUTOMATIC, LINIF_APPL_CONST)
    tpChCfgPtr = &LINTP_GET_CHANNEL_CONFIG(0u);
    NetworkHandleType ch;
    NetworkHandleType retLinTpChannelId = 0u;

    for (ch = 0u; ch < LINTP_NUMBER_OF_CHANNELS; ch++)
    {
        if (tpChCfgPtr->LinTpLinIfChannelRef == LinIfChannelId)
        {
            retLinTpChannelId = ch;
        }

        tpChCfgPtr++;
    }
    return retLinTpChannelId;
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#endif /* LINTP_MASTER_SUPPORT == STD_ON */
#endif /* LINIF_TP_SUPPORTED == STD_ON */
