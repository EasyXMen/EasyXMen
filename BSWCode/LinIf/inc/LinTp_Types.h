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
 **  @file               : LinTp_Types.h
 **  @author             : HuRongbo
 **  @date               :
 **  @vendor             : isoft
 **  @description        : Type definitions of LinTp
 **
 ***********************************************************************************************************************/

#ifndef LINTP_TYPES_H_
#define LINTP_TYPES_H_
/* PRQA S 1536,5209 EOF */ /* VL_LinTp_1536,VL_LinTp_5209 */
/* =================================================== inclusions =================================================== */
#include "ComStack_Types.h"
#include "LinTp_Cfg.h"
#include "LinIf_Cfg.h"
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
#include "Os_Types.h"
#endif

/* ===================================================== macros ===================================================== */
#define LINTP_FRAME_LEN_MAX 8u
#define LINTP_PDU_PCI_MASK  0xF0u
#define LINTP_PDU_PCI_SF    0x00u
#define LINTP_PDU_PCI_FF    0x10u
#define LINTP_PDU_PCI_CF    0x20u

/* ================================================ type definitions ================================================ */
#if (STD_ON == LINIF_TP_SUPPORTED)
/*@req <SWS_LinIf_00629> */
typedef enum LinTp_ModeTag
{
    LINTP_APPLICATIVE_SCHEDULE,
    LINTP_DIAG_REQUEST,
    LINTP_DIAG_RESPONSE
} LinTp_Mode;

typedef enum
{
    LINTP_UNINIT,
    LINTP_CHANNEL_IDLE,
    LINTP_CHANNEL_BUSY,
    LINTP_INIT
} LinTp_StateType;

/* current enabled timer for receiver or sender  */
typedef enum
{
    LINTP_TIMER_NONE, /* No timer started */
    LINTP_TIMER_NAS,  /* N_As started */
    LINTP_TIMER_NCS,  /* N_Cs started */
    LINTP_TIMER_NCR,  /* N_Cr started */
    LINTP_TIMER_P2,   /* P2 started */
    LINTP_TIMER_P2MAX /* P2*MAX started */
} LinTp_TimerType;

typedef enum
{
    LINTP_FRAMETYPE_NONE = (uint8)LINTP_PDU_PCI_MASK,
    LINTP_FRAMETYPE_SF   = (uint8)LINTP_PDU_PCI_SF,
    LINTP_FRAMETYPE_FF   = (uint8)LINTP_PDU_PCI_FF,
    LINTP_FRAMETYPE_CF   = (uint8)LINTP_PDU_PCI_CF
} LinTp_FrameType;

typedef enum
{
    LINTP_SLAVE_RX_IDLE,
    LINTP_SLAVE_RX_SF,
    LINTP_SLAVE_RX_FF,
    LINTP_SLAVE_RX_REQ_BUF,
    LINTP_SLAVE_RX_WAIT_CF,
    LINTP_SLAVE_RX_CF
} LinTp_SlaveRxStage;

typedef enum
{
    LINTP_SLAVE_TX_IDLE,
    LINTP_SLAVE_TX_SF,
    LINTP_SLAVE_TX_FF,
    LINTP_SLAVE_TX_CF,
    LINTP_SLAVE_TX_WAIT_CONFIRM
} LinTp_SlaveTxStage;

typedef struct LinTp_ChannelConfigTag
{
    uint16            MaxNumberOfRespPendingFrames;
    uint16            P2MaxCnt;
    uint16            P2TimingCnt;
    NetworkHandleType ChannelRef;      /* Refer to ComM */
    NetworkHandleType LinIfChannelRef; /* Refer to LinIf channel */
    boolean           DropNotRequestedNad;
#if (STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT)
    boolean ScheduleChangeDiag;
#endif
} LinTp_ChannelConfigType;

typedef struct LinTp_RxNSduTag
{
    PduIdType RxNSduId;
    PduIdType RxNSduPduRef;
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
    ApplicationType PduApplicationId;
#endif
    uint16            NcrCnt;
    NetworkHandleType RxNSduChannelRef; /* Refer to ComM */
    NetworkHandleType LinIfChannelRef;  /* Refer to LinIf channel */
    uint8             RxNSduNad;
} LinTp_RxNSduType;

typedef struct LinTp_TxNSduTag
{
    PduIdType TxNSduId;
    PduIdType TxNSduPduRef;
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
    ApplicationType PduApplicationId;
#endif
    uint16            NasCnt;
    uint16            NcsCnt;
    NetworkHandleType TxNSduChannelRef; /* Refer to ComM */
    NetworkHandleType LinIfChannelRef;  /* Refer to LinIf channel */
    uint8             MaxBufReq;
    uint8             TxNSduNad;
} LinTp_TxNSduType;

/* running control of the timer for receiver and sender */
typedef struct LinTp_ChannelTimerTag
{
    LinTp_TimerType EnabledTimer;
    uint16          Timer;
} LinTp_ChannelTimerType;

typedef struct LinTp_MasterRuntimeTag
{
    uint8                          SduBuf[LINTP_FRAME_LEN_MAX];
    LinTp_ChannelTimerType         TpTimer;
    LinTp_ChannelTimerType         TpP2Timer;
    const LinTp_ChannelConfigType* ChCfgPtr;
    const LinTp_TxNSduType*        TxNSduPtr;
    const LinTp_RxNSduType*        RxNSduPtr;
    const LinTp_TxNSduType*        FuncReqNSduPtr;
    const LinTp_TxNSduType*        PhyReqNSduPtr;
    LinTp_FrameType                LastFrameType;
    LinTp_StateType                ChannelState;
    LinTp_Mode                     RecoverMode;
    PduLengthType                  SduSize;
    PduLengthType                  SduIdx;
    PduLengthType                  SduRemaining;
    PduLengthType                  UpperBufRemaining;
    PduLengthType                  FuncReqSduSize;
    PduLengthType                  PhyReqSduSize;
    uint16                         SubEvent;
    uint16                         PendingFrameNum;
    uint8                          SduSN;
    uint8                          BufReqNum;
    uint8                          MRFRequestedNad;
    uint8                          MRFRequestedSID;
    uint8                          TrsEvent;
    boolean                        NeedRestoreScence;
} LinTp_MasterRuntimeType;

typedef struct LinTp_SlaveRuntimeTag
{
    uint8                          SduBuf[LINTP_FRAME_LEN_MAX];
    const LinTp_ChannelConfigType* ChCfgPtr;
    const LinTp_TxNSduType*        TxNSduPtr;
    const LinTp_RxNSduType*        RxNSduPtr;
    LinTp_SlaveRxStage             RxStage;
    LinTp_SlaveTxStage             TxStage;
    LinTp_FrameType                LastFrameType;
    LinTp_StateType                channelState;
    PduLengthType                  SduSize;
    PduLengthType                  SduIdx;
    PduLengthType                  SduRemaining;
    PduLengthType                  UpperBufRemaining;
    uint16                         NcrTimer;
    uint16                         NasTimer;
    uint16                         NcsTimer;
    uint8                          SduSN;
    uint8                          RetryCopyCnt;
    uint8                          MRFRequestedNad;
    boolean                        FunctionAddressFlag;
} LinTp_SlaveRuntimeType;

typedef struct LinTp_RtDataVariantMapTypeTag
{
    LinTp_MasterRuntimeType* const* MasterRtDataPtr;
    LinTp_MasterRuntimeType* const* BackupMasterRtDataPtr;
    LinTp_SlaveRuntimeType* const*  SlaveRtDataPtr;
} LinTp_RtDataVariantMapType;

typedef struct LinTp_PbsMapTypeTag
{
    const uint16* RxNSduHandlerIdMap;
    const uint16* TxNSduHandlerIdMap;
    const uint16  RxNSduMapLen;
    const uint16  TxNSduMapLen;
} LinTp_PbsMapType;

typedef struct LinTp_ConfigTag
{
    const LinTp_ChannelConfigType*    ChannelConfig;
    const LinTp_RxNSduType*           RxNSdu;
    const LinTp_TxNSduType*           TxNSdu;
    const LinTp_PbsMapType*           PbsMap;
    const LinTp_RtDataVariantMapType* RtDataVariantMapPtr;
    uint16                            NumOfRxNSdu;
    uint16                            NumOfTxNSdu;
    uint16                            NumOfChannel;
    uint16                            NumOfMasterChannel;
    uint16                            NumOfSlaveChannel;
} LinTp_ConfigType;

#endif

#endif
