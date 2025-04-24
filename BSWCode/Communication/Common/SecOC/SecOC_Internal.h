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
 */
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : SomeIpTp_Internal.h                                         **
**                                                                            **
**  Created on  : 2020/08/18                                                  **
**  Author      : HuRongbo                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION : SecOC internal use                                          **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
#ifndef SECOC_INTERNAL_H
#define SECOC_INTERNAL_H

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "ComStack_Types.h"
#include "SecOC.h"

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
#define SECOC_MAX_AUTHENTICATOR_LEN     (16u + 1u)
#define SECOC_MAX_RX_SECURED_MSG_LENGTH 100u

#define SECOC_DATAID_LEN                2u

/* Event */
#define SECOC_EVENT_TX_ALL                     0x00FFu
#define SECOC_TX_REQUEST                       0x0001u
#define SECOC_TX_DATA_ACQUIRED                 0x0002u
#define SECOC_TX_AUTHENTIC_PDU_CONFIRMED       0x0004u
#define SECOC_TX_CRYPTOGRAPHIC_PDU_CONFIRMED   0x0008u
#define SECOC_TX_GENERATE_FINISH_IND_WAITING   0x0010u
#define SECOC_TX_GENERATE_FINISH_IND_CONFIRMED 0x0020u

#define SECOC_EVENT_RX_ALL                     0x01FFu
#define SECOC_RX_INDICATION                    0x0001u
#define SECOC_RX_SECURED_PDU                   0x0002u
#define SECOC_RX_AUTHENTIC_PDU                 0x0004u
#define SECOC_RX_CRYPTOGRAPHIC_PDU             0x0008u
#define SECOC_RX_PARSED_MESSAGE                0x0010u
#define SECOC_RX_VERIFY_FINISH_IND_WAITING     0x0020u
#define SECOC_RX_VERIFY_FINISH_IND_CONFIRMED   0x0040u
#define SECOC_RX_TP_DATA_FROM_LOWER            0x0080u
#define SECOC_RX_TRANS_TP_DATA_TO_UPPER        0x0100u
/* PRQA S 3472,3469 ++ */ /* MISRA Dir 4.9 */

/* Returns the remainder result, only for powers of 2 where the divisor is */
#define SECOC_MOD_SHIFT(dividend, divisor) ((dividend) & ((divisor)-1u))

/* Returns the remainder form 8 */
static inline uint8 SECOC_MOD_FROM_EIGHT(uint16 dividend)
{
    return (uint8)(dividend) & 0X07u;
}

/* Returns the quotient, only for powers of 2 where the divisor is, y is raised to the power of 2*/
static inline uint16 SECOC_DIV_SHIFT(uint16 dividend, uint16 y)
{
    return (dividend) >> (y);
}

static uint16 SECOC_BIT_TO_BYTE(uint16 nbits)
{
    return SECOC_DIV_SHIFT(((nbits) + 7u), 3u);
}

/* Macro used to get configuration data */
/* PRQA S 3472,3469 -- */ /* MISRA Dir 4.9 */

/*******************************************************************************
**                      Private Type Definitions                               *
*******************************************************************************/
typedef enum
{
    SECOC_TX_NONE,
    SECOC_TX_CALC,
    SECOC_TX_TRANS,
    SECOC_TX_RETRY,
    SECOC_TX_CANCEL
} SecOC_TxStatusType;

typedef enum
{
    SECOC_RX_NONE,
    SECOC_RX_CALC,
    SECOC_RX_RETRY
} SecOC_RxStatusType;

#if (SECOC_TX_PDU_NUM > 0u)
/* Define transmit pdu runtime value */
typedef struct
{
    SecOC_TxStatusType txPduStatus;
    PduLengthType txPduLength;
    PduLengthType txSecuredPduLen;
    PduLengthType txAuthenticPduLen;
    PduLengthType txCryptographicPduLen;
    SecOC_FreshnessArrayType txFreshnessValue;
    boolean txPduReady;
    uint16 txAuthBuildCnt;
    uint8 txResultBuffer[SECOC_MAX_AUTHENTICATOR_LEN];
    uint32 txResultLength;
    boolean txDefaultAuthInfoEnable;
#if (SECOC_TX_TP_PDU_NUM > 0u)
    PduLengthType tpTxPduOffset;
#endif
} SecOC_TxPduRTType;
#endif /* SECOC_TX_PDU_NUM > 0u */

#if (SECOC_RX_PDU_NUM > 0u)
/* Queue Info type for SecOC rx process */
/* Data in queue buffer:
 * QUEUE_INFO|DATA|QUEUE_INFO|DATA...
 */
typedef struct
{
    PduLengthType pduLength;
    PduIdType pduId;
} SecOC_QueueInfoType;

typedef struct
{
    uint16 queueInfoIdx;
    uint16 freeQueSize;
    uint16 freeQueStartIdx;
    uint16 queueFrameCnt;
} SecOC_QueueManageType;

/* Define receive pdu runtime value */
typedef struct
{
    SecOC_RxStatusType rxPduStatus;
    PduIdType curPduId;
    PduLengthType rxSecuredPduLen;
    PduLengthType rxAuthenticPduLen;
    PduLengthType rxCryptographicPduLen;
    PduLengthType rxParsedAuthPduLen;
    SecOC_QueueManageType queueManage;
    uint8 rxParsedFreshnessValue[8u]; /* Max 64 bits */
    uint8 rxParsedAuthenticator[SECOC_MAX_AUTHENTICATOR_LEN];
    uint8* rxParsedAuthPduBufPtr;
    uint16 rxAuthBuildAttempts;
    uint16 rxVerifyAttempts;
    SecOC_FreshnessArrayType rxFreshnessValue;
    Crypto_VerifyResultType rxVerifyResult;
    SecOC_OverrideStatusType rxOverrideStatus;
    uint8 rxNumOfMsgToOverride;
    uint8 rxBakNumOfMsgToOverride;
    SecOC_VerificationResultType rxVeryfyResult;
#if (SECOC_RX_TP_PDU_NUM > 0)
    P2VAR(uint8, TYPEDEF, SECOC_APPL_DATA) tpRxDestBufPtr;
    PduLengthType sduRemaining;
    PduLengthType upLayerBufSize;
    PduLengthType tpRxPduOffset;
#endif
} SecOC_RxPduRTType;
#endif /* SECOC_RX_PDU_NUM > 0u */

#endif /* SECOC_INTERNAL_H */
