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
 **  @file               : LinTp_Master.h
 **  @author             : HuRongbo
 **  @date               :
 **  @vendor             : isoft
 **  @description        :
 **
 ***********************************************************************************************************************/

#ifndef LINTP_MASTER_H_
#define LINTP_MASTER_H_

/* =================================================== inclusions =================================================== */
#include "LinIf_Cfg.h"
#if (LINIF_TP_SUPPORTED == STD_ON)
#include "LinTp_Cfg.h"
#include "LinTp_PartitionVar.h"
#if (LINTP_MASTER_SUPPORT == STD_ON)
#include "LinTp.h"
#include "LinTp_Internal.h"

/* ===================================================== macros ===================================================== */

/* =========================================== extern data declarations =========================================== */

/* =========================================== inline function definitions ===========================================
 */

LINTP_LOCAL_INLINE LinTp_MasterRuntimeType* LinTp_MasterRtDataPtr(uint8 ch)
{
    return LinTp_ConfigPtr->RtDataVariantMapPtr->MasterRtDataPtr[ch];
}

LINTP_LOCAL_INLINE LinTp_MasterRuntimeType* LinTp_BackupMasterRtDataPtr(uint8 ch)
{
    return LinTp_ConfigPtr->RtDataVariantMapPtr->BackupMasterRtDataPtr[ch];
}

LINTP_LOCAL_INLINE void LinTp_SetEvent(LinTp_MasterRuntimeType* tpChPtr, uint16 event)
{
    tpChPtr->SubEvent |= event;
}

LINTP_LOCAL_INLINE uint16 LinTp_GetP2Timer(uint16 idx)
{
    return LinTp_GetChannelConfig(idx)->P2TimingCnt;
}

LINTP_LOCAL_INLINE void LinTp_ClrEvent(LinTp_MasterRuntimeType* tpChPtr, uint16 event)
{
    tpChPtr->SubEvent &= (uint16)(~event);
}

LINTP_LOCAL_INLINE boolean LinTp_IsEvent(const LinTp_MasterRuntimeType* tpChPtr, uint16 event)
{
    return (tpChPtr->SubEvent & event) != 0u;
}

LINTP_LOCAL_INLINE void LinTp_SetTrsEvent(LinTp_MasterRuntimeType* tpChPtr, uint8 event)
{
    tpChPtr->TrsEvent |= event;
}

LINTP_LOCAL_INLINE void LinTp_ClrTrsEvent(LinTp_MasterRuntimeType* tpChPtr, uint8 event)
{
    tpChPtr->TrsEvent &= (uint8)(~event);
}

LINTP_LOCAL_INLINE boolean LinTp_IsTrsEvent(const LinTp_MasterRuntimeType* tpChPtr, uint16 event)
{
    return (tpChPtr->TrsEvent & event) != 0u;
}

/* Reset channel runtime data */
LINTP_LOCAL_INLINE void LinTp_MasterChReset(LinTp_MasterRuntimeType* tpChPtr)
{
    tpChPtr->ChannelState           = LINTP_CHANNEL_IDLE;
    tpChPtr->TxNSduPtr              = (const LinTp_TxNSduType*)NULL_PTR;
    tpChPtr->RxNSduPtr              = (const LinTp_RxNSduType*)NULL_PTR;
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
LINTP_LOCAL_INLINE void LinTp_ConstructSF(LinTp_MasterRuntimeType* tpChPtr)
{
    tpChPtr->SduBuf[LINTP_PDU_OFS_NAD] = tpChPtr->TxNSduPtr->TxNSduNad;
    tpChPtr->SduBuf[LINTP_PDU_OFS_PCI] = (uint8)tpChPtr->SduSize;
    tpChPtr->SduIdx                    = LINTP_PDU_OFS_SF_DATA;
}

/* Construct FF */
LINTP_LOCAL_INLINE void LinTp_ConstructFF(LinTp_MasterRuntimeType* tpChPtr)
{
    tpChPtr->SduBuf[LINTP_PDU_OFS_NAD] = tpChPtr->TxNSduPtr->TxNSduNad;
    tpChPtr->SduBuf[LINTP_PDU_OFS_PCI] =
        LINTP_PDU_PCI_FF | (uint8)(tpChPtr->SduSize >> LINTP_PDU_DATESIZE_PCI_EXTERNINFO_OFFSET);
    tpChPtr->SduBuf[LINTP_PDU_OFS_LEN] = (uint8)(tpChPtr->SduSize & LINTP_SDU_SIZE_MSK);
    tpChPtr->SduIdx                    = LINTP_PDU_OFS_FF_DATA;
    tpChPtr->SduSN                     = 1u;
}

/* Construct CF */
LINTP_LOCAL_INLINE void LinTp_ConstructCF(LinTp_MasterRuntimeType* tpChPtr)
{
    tpChPtr->SduBuf[LINTP_PDU_OFS_NAD] = tpChPtr->TxNSduPtr->TxNSduNad;
    tpChPtr->SduBuf[LINTP_PDU_OFS_PCI] = LINTP_PDU_PCI_CF | tpChPtr->SduSN;
    tpChPtr->SduIdx                    = LINTP_PDU_OFS_CF_DATA;
    tpChPtr->SduSN++;
    tpChPtr->SduSN &= LINTP_PDU_PCI_SN_MASK;
}

/* ========================================= external function declarations ========================================= */

/**
 * @brief               LinTp master node runtime data init
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
void LinTp_MasterInit(void);

/**
 * @brief               LinTp master node channel runtime data init
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
void LinTp_MasterChannelInit(NetworkHandleType LinIfChannelId);

/**
 * @brief               Requests the transfer of segmented data.
 * @param[in]           txNSdu: Pointer of TxSdu to be transmit
 * @param[in]           LinTpTxInfoPtr: Length of and pointer to the PDU data
 * @return              E_OK: Transmit request has been accepted.
 * @return              E_NOT_OK: Transmit request has not been accepted
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
Std_ReturnType LinTp_MasterTransmit(const LinTp_TxNSduType* txNSdu, const PduInfoType* LinTpTxInfoPtr);

/**
 * @brief               Shutdowns the LIN TP
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
void LinTp_MasterShutdown(void);

/**
 * @brief               Tx success process in master node.
 * @param[in]           LinIfChannelId: Identification of the LIN channel.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
void LinTp_MasterTxSuccessHandle(NetworkHandleType LinIfChannelId);

/**
 * @brief               Tx error process in master node.
 * @param[in]           LinIfChannelId: Identification of the LIN channel.
 * @param[in]           st: Error status of transmitted
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
void LinTp_MasterTxErrorHandle(NetworkHandleType LinIfChannelId, Lin_StatusType st);

/**
 * @brief               Parses a event of the receiving
 * @param[in]           LinIfChannelId: Identification of the LIN channel
 * @param[in]           sdu: Pointer of sdu that be received
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
void LinTp_RxEventParse(uint8 LinIfChannelId, const uint8* sdu);

/**
 * @brief               LinIf get MRF response from LinTp
 * @param[in]           LinIfChannelId: Identification of the LIN channel
 * @param[out]          sduBufPtr: Pointer of sduBufPtr that be received
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
Std_ReturnType LinTp_MasterGetMRFResponse(NetworkHandleType LinIfChannelId, uint8* sduBufPtr);

/**
 * @brief               Get whether LINTP_EVENT_WAIT flag is set
 * @param[in]           LinIfChannelId: Identification of the LIN channel
 * @return              FALSE: LINTP_EVENT_WAIT isn't happend
 * @return              TRUE: LINTP_EVENT_WAIT happend
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
boolean LinTp_IsWaitEventSet(NetworkHandleType LinIfChannelId);

/**
 * @brief               Get whether LINTP_EVENT_TX or LINTP_EVENT_STOP_SRF flag is set
 * @param[in]           LinIfChannelId: Identification of the LIN channel
 * @return              FALSE: LINTP_EVENT_TX or LINTP_EVENT_STOP_SRF isn't happend
 * @return              TRUE: LINTP_EVENT_TX or LINTP_EVENT_STOP_SRF happend
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
boolean LinTp_IsStopSRFSendEventSet(uint8 LinIfChannelId);

/**
 * @brief               Get if LINTP_EVENT_STOP_SRF or LINTP_EVENT_STOP_MRF_SRF flag is set
 * @param[in]           LinIfChannelId: Identification of the LIN channel
 * @return              FALSE: LINTP_EVENT_STOP_SRF or LINTP_EVENT_STOP_MRF_SRF isn't happend
 * @return              TRUE: LINTP_EVENT_STOP_SRF or LINTP_EVENT_STOP_MRF_SRF happend
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
boolean LinTp_IsStopMRFOrSRFSendEventSet(NetworkHandleType LinIfChannelId);

#if (STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT)
/**
 * @brief               Get LinTpScheduleChangeDiag parameter
 * @param[in]           LinIfChannelId: Identification of the LIN channel
 * @return              FALSE: BswM is not called
 * @return              TRUE: BswM is called
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
boolean LinTp_GetScheduleChangeDiag(NetworkHandleType LinIfChannelId);

/**
 * @brief               LinTp master main function
 * @param[in]           LinIfChannelId: Identification of the LIN channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
void LinTp_MasterMainFunction(uint8 LinIfChannelId);
#endif

/**
 * @brief               Transmitting process
 * @param[inout]        LinTpMasterRtDataPtr: The runtime data of lintp master channel
 * @param[in]           LinIfChannelId: Identification of the LIN channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
void LinTp_TxProcess(LinTp_MasterRuntimeType* LinTpMasterRtDataPtr);

/**
 * @brief               Receiving process
 * @param[inout]        masterChRtDataPtr: The runtime data of lintp master channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
void LinTp_RxProcess(LinTp_MasterRuntimeType* masterChRtDataPtr);

/**
 * @brief               Load transmit request information
 * @param[in]           ch: Identification of the LIN channel
 * @param[inout]        tpChPtr: Runtime data of lintp master channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
void LinTp_LoadTxRequest(NetworkHandleType ch, LinTp_MasterRuntimeType* tpChPtr);

/**
 * @brief               Handle the timer for specific channel.
 * @param[inout]        masterChRtDataPtr: The runtime data of lintp master channel
 * @param[in]           LinIfChannelId: Identification of the LIN channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
void LinTp_HandleTimers(LinTp_MasterRuntimeType* masterChRtDataPtr, NetworkHandleType LinIfChannelId);

/**
 * @brief               Get the LinIf channel corresponding LinTp channel
 * @param[in]           ch: Identification of the LIN channel
 * @return              retLinTpChannelId: Identification of the Lintp channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
NetworkHandleType LinTp_GetLinTpChannel(NetworkHandleType LinIfChannelId);

#endif
#endif
#endif
