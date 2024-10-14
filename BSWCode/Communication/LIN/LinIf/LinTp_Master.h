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
 *
 ********************************************************************************
 **                                                                            **
 **  FILENAME    : LinTp_Master.h                                              **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      : HuRongbo                                                    **
 **  Vendor      :                                                             **
 **  DESCRIPTION :                                                             **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
 **                                                                            **
 *******************************************************************************/
/* PRQA S 3108-- */
#ifndef LINTP_MASTER_H
#define LINTP_MASTER_H

#include "LinIf_Cfg.h"
#if ((LINTP_MASTER_SUPPORT == STD_ON) && (STD_ON == LINIF_MASTER_SUPPORT))
#include "LinTp.h"
#include "LinTp_Internal.h"

/*******************************************************************************
**                            Macros Definitions                              **
*******************************************************************************/
#define LINTP_MASTER_CH(ch)    &LinTp_MasterRTData[ch]

#define LINTP_SET_EVENT(e)     tpChPtr->SubEvent |= (e)
#define LINTP_CLR_EVENT(e)     tpChPtr->SubEvent &= ~(e)
#define LINTP_IS_EVENT(e)      (tpChPtr->SubEvent & (e)) != 0u

#define LINTP_SET_TRS_EVENT(e) tpChPtr->TrsEvent |= (e)
#define LINTP_CLR_TRS_EVENT(e) tpChPtr->TrsEvent &= ~(e)
#define LINTP_IS_TRS_EVENT(e)  (tpChPtr->TrsEvent & (e)) != 0u

/* Convert time(ms) to tick */
#define LINTP_MS2TICK(TIME) (OS_MS2TICK_SYSTIMERCORE0(TIME))

/*******************************************************************************
**                      Runtime Type Definitions                              **
*******************************************************************************/
typedef struct
{
    P2CONST(LinTp_ChannelConfigType, AUTOMATIC, LINIF_APPL_CONST) ChCfgPtr;
    P2CONST(LinTp_TxNSduType, AUTOMATIC, LINIF_APPL_CONST) TxNSduPtr;
    P2CONST(LinTp_RxNSduType, AUTOMATIC, LINIF_APPL_CONST) RxNSduPtr;
    PduLengthType SduSize;
    PduLengthType SduIdx;
    PduLengthType SduRemaining;
    PduLengthType UpperBufRemaining;
    LinTp_StateType LinTpChannelState;
    uint8 SduSN;
    uint16 SubEvent;
    uint8 SduBuf[LINTP_FRAME_LEN_MAX];
    uint8 BufReqNum;
    LinTp_ChannelTimerType TpTimer;
    LinTp_ChannelTimerType TpP2Timer;
    uint8 MRFRequestedNad;
    uint8 MRFRequestedSID;
    uint16 PendingFrameNum;
    LinTp_FrameType LastFrameType;
    P2CONST(LinTp_TxNSduType, AUTOMATIC, LINIF_APPL_CONST) FuncReqNSduPtr;
    PduLengthType FuncReqSduSize;
    LinTp_Mode RecoverMode;
    uint8 TrsEvent;
    P2CONST(LinTp_TxNSduType, AUTOMATIC, LINIF_APPL_CONST) PhyReqNSduPtr;
    PduLengthType PhyReqSduSize;
} LinTp_MasterRuntimeType;

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

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
FUNC(void, LINIF_CODE) LinTp_MasterInit(void);

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
FUNC(void, LINIF_CODE) LinTp_MasterChannelInit(NetworkHandleType LinIfChannelId);

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
    P2CONST(PduInfoType, AUTOMATIC, LINIF_APPL_CONST) LinTpTxInfoPtr);

/******************************************************************************/
/*
 * Brief               Shutdowns the LIN TP
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE) LinTp_MasterShutdown(void);

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
FUNC(void, LINIF_CODE) LinTp_MasterTxSuccessHandle(NetworkHandleType LinIfChannelId);

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
FUNC(void, LINIF_CODE) LinTp_MasterTxErrorHandle(NetworkHandleType LinIfChannelId, Lin_StatusType st);

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
FUNC(void, LINIF_CODE) LinTp_TxProcess(NetworkHandleType LinIfChannelId);

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
FUNC(void, LINIF_CODE) LinTp_RxProcess(NetworkHandleType LinIfChannelId);

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
FUNC(void, LINTP_CODE) LinTp_HandleTimers(NetworkHandleType LinIfChannelId);

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
FUNC(void, LINIF_CODE) LinTp_RxEventParse(uint8 LinIfChannelId, P2CONST(uint8, AUTOMATIC, LINIF_APPL_CONST) sdu);

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
LinTp_MasterGetMRFResponse(NetworkHandleType LinIfChannelId, P2VAR(uint8, AUTOMATIC, LINIF_APPL_DATA) sduBufPtr);

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
FUNC(boolean, LINIF_CODE) LinTp_IsWaitEventSet(NetworkHandleType LinIfChannelId);

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
FUNC(boolean, LINIF_CODE) LinTp_IsStopSRFSendEventSet(uint8 LinIfChannelId);

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
FUNC(boolean, LINIF_CODE) LinTp_IsStopMRFOrSRFSendEventSet(NetworkHandleType LinIfChannelId);

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
FUNC(boolean, LINIF_CODE) LinTp_GetScheduleChangeDiag(NetworkHandleType LinIfChannelId);
#endif /* STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT */

#endif /* LINTP_MASTER_SUPPORT == STD_ON && STD_ON == LINIF_MASTER_SUPPORT */

#endif /* LINTP_MASTER_H */
