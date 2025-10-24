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
 **  @file               : LinTp_Slave.h
 **  @author             : HuRongbo
 **  @date               :
 **  @vendor             : isoft
 **  @description        : LinTp slave implementation header file
 **
 ***********************************************************************************************************************/

#ifndef LINTP_SLAVE_H_
#define LINTP_SLAVE_H_

/* =================================================== inclusions =================================================== */
#include "LinIf_Cfg.h"
#if (LINIF_TP_SUPPORTED == STD_ON)
#include "LinTp_Cfg.h"
#include "LinTp_PartitionVar.h"
#if (LINTP_SLAVE_SUPPORT == STD_ON)
#include "LinTp.h"
#include "LinTp_Internal.h"

/* ================================================ type definitions ================================================ */

/* ========================================= external function declarations ========================================= */

/**
 * @brief               LinTp slave node runtime data init
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
void LinTp_SlaveInit(void);

/**
 * Brief                LinTp slave node runtime data init and cancel TP process on going
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
void LinTp_SlavePreInit(NetworkHandleType ch);

/**
 *  @brief         TP message receive indication process in slave channel.
 *  @param[in]     ch: LinIf Channel
 *  @param[in]     Lin_SduPtr: pointer to a buffer where the current SDU is stored.
 *  @reentrant     FALSE
 *  @synchronous   FALSE
 *  @trace        -
 */
void LinTp_SlaveMRFIndication(NetworkHandleType ch, const uint8* Lin_SduPtr);

/**
 * @brief               Get ComMChannel corresponding LinTp channel
 * @param[in]           ComMChannel: Identification of the Com channel
 * @param[in]           LinTpTxInfoPtr: Pointer of lintp txPduInfo
 * @return              E_OK: Transmit request has been accepted
 * @return              E_NOT_OK: Transmit request has not been accepted.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
Std_ReturnType LinTp_SlaveTransmit(const LinTp_TxNSduType* txNSdu, const PduInfoType* LinTpTxInfoPtr);

/**
 * @brief               Shutdowns the LINTP slave node
 * @param[in]           applicationId: The partition id of lintp channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
void LinTp_SlaveShutdown(ApplicationType applicationId);

/**
 * @brief               Check slave node function address flag.
 * @param[in]           ch: Identification of the LIN channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
boolean LinTp_SlaveCheckFunctionAddressFlag(NetworkHandleType ch);

/**
 * @brief               Set slave node function address flag.
 * @param[in]           ch: Identification of the LIN channel
 * @param[in]           functionAddressFlag: Receive tp message is function address
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
void LinTp_SlaveSetFunctionAddressFlag(NetworkHandleType ch, boolean functionAddressFlag);

/**
 * @brief               SRF process(Response transmit) in slave channel.
 * @param[in]           ch: Identification of the LIN channel
 * @param[inout]        PduPtr: Lin_PduType buffer pointer provided by Lin.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               - */
void LinTp_SlaveSRFIndication(NetworkHandleType ch, Lin_PduType* PduPtr);

/**
 * @brief               TP tx confirmation process in slave channel.
 * @param[in]           ch: Identification of the LIN channel
 * @param[out]          isTpTxFinish: Indiacate if TP transmit finished
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
void LinTp_SlaveTxConfirmation(NetworkHandleType ch, boolean* isTpTxFinish);

#if (LINIF_SUPPORT_RESPONSE_ERROR_SIGNAL == STD_ON)
/**
 * @brief               Tx error signal process in slave channel.
 * @param[in]           LinIfChannelId: Identification of the LIN channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
void LinTp_SlaveTxErrorSignalHandler(NetworkHandleType LinIfChannelId);
#endif

/**
 * @brief               LinTp slave main function
 * @param[in]           ch: Identification of the LIN channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
void LinTp_SlaveMainFunction(NetworkHandleType ch);

#endif
#endif
#endif
