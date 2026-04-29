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
 **  @file               : LinTp_Internal.h
 **  @author             : HuRongbo
 **  @date               :
 **  @vendor             : isoft
 **  @description        : LinTp internal header for internal API declarations and definitions
 **
 ***********************************************************************************************************************/

/* PRQA S 1510 EOF */ /* VL_LinTp_1510 */
#ifndef LINTP_INTERNAL_H_
#define LINTP_INTERNAL_H_

/* =================================================== inclusions =================================================== */
#include "LinIf_Cfg.h"
#if (LINIF_TP_SUPPORTED == STD_ON)
#include "LinTp_Types.h"
#include "SchM_LinTp.h"

/* ===================================================== macros ===================================================== */
#if !defined(LINTP_LOCAL_INLINE)
#define LINTP_LOCAL_INLINE static inline
#endif

#if !defined(LINTP_LOCAL)
#define LINTP_LOCAL static /* PRQA S 3414 */ /* VL_QAC_FctLikeMacro */
#endif

/* =========================================== internal data declarations =========================================== */
/* Global configuration pointer of LINTP */
extern const LinTp_ConfigType* LinTp_ConfigPtr;

/* =========================================== inline function definition =========================================== */
LINTP_LOCAL_INLINE const LinTp_RxNSduType* LinTp_GetRxNSduCfg(uint32 idx)
{
    return &(LinTp_ConfigPtr->RxNSdu[idx]);
}

LINTP_LOCAL_INLINE const LinTp_TxNSduType* LinTp_GetTxNSduCfg(uint32 idx)
{
    return &(LinTp_ConfigPtr->TxNSdu[idx]);
}

LINTP_LOCAL_INLINE uint16 LinTp_GetRxNSduNum(void)
{
    return LinTp_ConfigPtr->NumOfRxNSdu;
}

LINTP_LOCAL_INLINE uint16 LinTp_GetTxNSduNum(void)
{
    return LinTp_ConfigPtr->NumOfTxNSdu;
}

LINTP_LOCAL_INLINE const LinTp_ChannelConfigType* LinTp_GetChannelConfig(uint16 idx)
{
    return &(LinTp_ConfigPtr->ChannelConfig[idx]);
}

/* The maximum frame length */
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
#define LINTP_PDU_PCI_SN_MAX  0x10u
#define LINTP_PDU_PCI_DL_MASK 0x0Fu
#define LINTP_PDU_PCI_SN_MASK 0x0Fu

/* PDU information offset index */
#define LINTP_PDU_OFS_NAD       0u
#define LINTP_PDU_OFS_PCI       1u
#define LINTP_PDU_OFS_LEN       2u
#define LINTP_PDU_OFS_SF_SID    2u
#define LINTP_PDU_OFS_SF_RSID   2u
#define LINTP_PDU_OFS_SF_DATA   2u
#define LINTP_PDU_OFS_SF_NR_SID 3u
#define LINTP_PDU_OFS_SF_NR_NRC 4u
#define LINTP_PDU_OFS_FF_SID    3u
#define LINTP_PDU_OFS_FF_RSID   3u
#define LINTP_PDU_OFS_FF_DATA   3u
#define LINTP_PDU_OFS_CF_DATA   2u

#define LINTP_NEGATIVE_RSID        0x7Fu
#define LINTP_NRC_RESPONSE_PENDING 0x78u
#define LINTP_FUNCTIONAL_REQ_NAD   0x7Eu
#define LINTP_BROADCAST_REQ_NAD    0x7Fu

#define LINTP_LENGTH_SF_MIN 0u
#define LINTP_LENGTH_SF_MAX 6u
#define LINTP_LENGTH_FF_MIN 7u

#define LINTP_PDU_DATESIZE_PCI_EXTERNINFO_OFFSET 8u
#define LINTP_SDU_SIZE_MSK                       0xffu
#define LINTP_METADATA_BUFFER_LEN                5u

/* ================================================ type definitions ================================================ */

/* ========================================= external function declarations ========================================= */
/**
 * @brief               Gets the configuration pointer by receive 'NAD'
 * @param[in]           ch: Identification of the LIN channel
 * @param[in]           Nad: Nad of LinTp channel
 * @return              LinTp_RxNSduType: The pointer of RxNSdu
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
const LinTp_RxNSduType* LinTp_GetRxNSduByNad(NetworkHandleType ch, uint8 Nad);

#endif

#endif
