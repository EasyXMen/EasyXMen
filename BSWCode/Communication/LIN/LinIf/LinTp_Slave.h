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
 **  FILENAME    : LinTp_Slave.h                                               **
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
#ifndef LINTP_SLAVE_H
#define LINTP_SLAVE_H

#include "LinIf_Cfg.h"
#if (LINTP_SLAVE_SUPPORT == STD_ON)
#include "LinTp.h"
#include "LinTp_Internal.h"

/*******************************************************************************
**                            Macros Definitions                              **
*******************************************************************************/

#define LINTP_GET_SLAVE_RTDATA_PTR(ch) &LinTp_SlaveRTData[(ch)-LINTP_MASTER_CHANNEL_NUMBER]

/*******************************************************************************
**                      Runtime Type Definitions                              **
*******************************************************************************/
typedef enum
{
    LINTP_SLAVE_RX_SF,
    LINTP_SLAVE_RX_FF,
    LINTP_SLAVE_RX_REQ_BUF,
    LINTP_SLAVE_RX_CF,
    LINTP_SLAVE_RX_WAIT_CF,
    LINTP_SLAVE_RX_IDLE
} LinTp_SlaveRxStage;

typedef enum
{
    LINTP_SLAVE_TX_SF,
    LINTP_SLAVE_TX_FF,
    LINTP_SLAVE_TX_CF,
    LINTP_SLAVE_TX_WAIT_CONFIRM,
    LINTP_SLAVE_TX_IDLE
} LinTp_SlaveTxStage;

typedef struct
{
    P2CONST(LinTp_ChannelConfigType, AUTOMATIC, LINIF_APPL_CONST) ChCfgPtr;
    P2CONST(LinTp_TxNSduType, AUTOMATIC, LINIF_APPL_CONST) TxNSduPtr;
    P2CONST(LinTp_RxNSduType, AUTOMATIC, LINIF_APPL_CONST) RxNSduPtr;
    uint8 SduBuf[LINTP_FRAME_LEN_MAX];
    PduLengthType SduSize;
    PduLengthType SduIdx;
    PduLengthType SduRemaining;
    PduLengthType UpperBufRemaining;
    LinTp_StateType channelState;
    uint16 NcrTimer;
    uint16 NasTimer;
    uint16 NcsTimer;
    LinTp_FrameType LastFrameType;
    LinTp_SlaveRxStage RxStage;
    LinTp_SlaveTxStage TxStage;
    uint8 SduSN;
    uint8 RetryCopyCnt;
    boolean FunctionAddressFlag;
} LinTp_SlaveRuntimeType;

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

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
FUNC(void, LINIF_CODE) LinTp_SlaveInit(void);

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
LinTp_SlaveMRFIndication(NetworkHandleType ch, P2CONST(uint8, AUTOMATIC, LINIF_APPL_DATA) Lin_SduPtr);

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
LinTp_SlaveTransmit(
    P2CONST(LinTp_TxNSduType, AUTOMATIC, LINIF_APPL_CONST) txNSdu,
    P2CONST(PduInfoType, AUTOMATIC, LINIF_APPL_CONST) LinTpTxInfoPtr);

/******************************************************************************/
/*
 * Brief               Shutdowns the LINTP slave node
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE) LinTp_SlaveShutdown(void);

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
LinTp_SlaveSRFIndication(NetworkHandleType ch, P2VAR(Lin_PduType, AUTOMATIC, LINIF_APPL_DATA) PduPtr);

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
LinTp_SlaveCheckFunctionAddressFlag(NetworkHandleType ch);

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
LinTp_SlaveSetFunctionAddressFlag(NetworkHandleType ch, boolean functionAddressFlag);

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
LinTp_SlaveTxConfirmation(NetworkHandleType ch, P2VAR(boolean, AUTOMATIC, LINIF_APPL_DATA) isTpTxFinish);

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
FUNC(void, LINIF_CODE) LinTp_SlaveMainFunction(NetworkHandleType ch);

#endif /* LINTP_SLAVE_SUPPORT == STD_ON */

#endif /* LINTP_SLAVE_H */
