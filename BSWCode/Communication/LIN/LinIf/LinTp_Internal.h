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
**  FILENAME    : LinTp_Internal.h                                            **
**                                                                            **
**  Created on  :                                                             **
**  Author      : HuRongbo                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation for LinIf                                    **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
#ifndef LINTP_INTERNAL_H
#define LINTP_INTERNAL_H

#include "LinIf_Cfg.h"
#if (LINIF_TP_SUPPORTED == STD_ON)
#include "LinTp_Types.h"
#include "SchM_LinTp.h"

/*******************************************************************************
**                            Macros Definitions                              **
*******************************************************************************/
/* PC/LT/PB configuration */
#define LINTP_GET_RXNSDU(idx)         LinTp_ConfigPtr->LinTpRxNSdu[idx]
#define LINTP_GET_TXNSDU(idx)         LinTp_ConfigPtr->LinTpTxNSdu[idx]
#define LINTP_GET_RXNSDU_NUM          LinTp_ConfigPtr->LinTpNumOfRxNSdu
#define LINTP_GET_TXNSDU_NUM          LinTp_ConfigPtr->LinTpNumOfTxNSdu
#define LINTP_GET_CHANNEL_CONFIG(idx) LinTp_ConfigPtr->LinTpChannelConfig[idx]
#define LINTP_GET_P2_TIME(idx)        LINTP_GET_CHANNEL_CONFIG(idx).LinTpP2TimingCnt

/* The maximum frame length */
#define LINTP_FRAME_LEN_MAX   8u
#define LINTP_SF_DATA_LEN_MAX 6u
#define LINTP_FF_DATA_LEN     5u
#define LINTP_CF_DATA_LEN_MAX 6u

/* Handle events of LINTP */
#define LINTP_EVENT_MASK     0xFFFFu
#define LINTP_EVENT_NONE     0x0000u
#define LINTP_EVENT_SF_REQ   0x0001u
#define LINTP_EVENT_FF_REQ   0x0002u
#define LINTP_EVENT_CF_REQ   0x0004u
#define LINTP_EVENT_COPY_REQ 0x0008u
#define LINTP_EVENT_RX_REQ   0x0010u
#define LINTP_EVENT_TX_REQ   0x0020u
#define LINTP_EVENT_RX       0x0040u
#define LINTP_EVENT_TX       0x0080u
#define LINTP_EVENT_CONF_REQ 0x0100u
#define LINTP_EVENT_CONF     0x0200u
#define LINTP_EVENT_OK       0x0400u
#define LINTP_EVENT_ERR      0x0800u
#define LINTP_EVENT_WAIT     0x1000u
#define LINTP_EVENT_STOP_SRF 0x2000u
#define LINTP_EVENT_STOP_MRF 0x4000u

/* Handle events of LINTP Transmit */
#define LINTP_TRS_EVT_MASK       0xFFu
#define LINTP_TRS_EVT_NONE       0x00u
#define LINTP_TRS_EVT_PHY_TX_REQ 0x01u
#define LINTP_TRS_EVT_PHY_TX     0x02u
#define LINTP_TRS_EVT_FUN_TX_REQ 0x04u
#define LINTP_TRS_EVT_FUN_TX     0x08u

/* PCI information mask */
#define LINTP_PDU_PCI_DL_MASK 0x0Fu
#define LINTP_PDU_PCI_SN_MASK 0x0Fu
#define LINTP_PDU_PCI_MASK    0xF0u
#define LINTP_PDU_PCI_SF      0x00u
#define LINTP_PDU_PCI_FF      0x10u
#define LINTP_PDU_PCI_CF      0x20u

/* PDU information offset index */
#define LINTP_PDU_OFS_NAD          0u
#define LINTP_PDU_OFS_PCI          1u
#define LINTP_PDU_OFS_LEN          2u
#define LINTP_PDU_OFS_SF_SID       2u
#define LINTP_PDU_OFS_SF_RSID      2u
#define LINTP_PDU_OFS_SF_DATA      2u
#define LINTP_PDU_OFS_SF_NR_SID    3u
#define LINTP_PDU_OFS_SF_NR_NRC    4u
#define LINTP_PDU_OFS_FF_SID       3u
#define LINTP_PDU_OFS_FF_RSID      3u
#define LINTP_PDU_OFS_FF_DATA      3u
#define LINTP_PDU_OFS_CF_DATA      2u

#define LINTP_NEGATIVE_RSID        0x7Fu
#define LINTP_NRC_RESPONSE_PENDING 0x78u
#define LINTP_FUNCTIONAL_REQ_NAD   0x7Eu
#define LINTP_BROADCAST_REQ_NAD    0x7Fu

#define LINTP_LENGTH_SF_MIN        0u
#define LINTP_LENGTH_SF_MAX        6u
#define LINTP_LENGTH_FF_MIN        7u
/*******************************************************************************
**                      Runtime Type Definitions                              **
*******************************************************************************/
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
    LINTP_FRAMETYPE_NONE = LINTP_PDU_PCI_MASK,
    LINTP_FRAMETYPE_SF = LINTP_PDU_PCI_SF,
    LINTP_FRAMETYPE_FF = LINTP_PDU_PCI_FF,
    LINTP_FRAMETYPE_CF = LINTP_PDU_PCI_CF
} LinTp_FrameType;

/* running control of the timer for receiver and sender */
typedef struct
{
    LinTp_TimerType EnabledTimer;
    uint16 Timer;
} LinTp_ChannelTimerType;

/*******************************************************************************
**                      Private Variable Declarations                         **
*******************************************************************************/

/* The status of LINTP */
extern VAR(LinTp_StateType, LINIF_VAR) LinTp_Status;

/* Global configuration pointer of LINTP */
extern P2CONST(LinTp_ConfigType, AUTOMATIC, LINIF_CONST) LinTp_ConfigPtr;

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

/******************************************************************************/
/*
 * Brief: Gets the configuration pointer by receive 'NAD'
 * Param-Name[in]: ch: LinIf channel
 *                 Nad: received 'NAD'
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: const LinTp_RxNSduType*
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
FUNC(P2CONST(LinTp_RxNSduType, AUTOMATIC, LINIF_APPL_CONST), LINIF_CODE)
LinTp_GetRxNSduByNad(NetworkHandleType ch, uint8 Nad);

#endif

#endif /* LINTP_INTERNAL_H */
