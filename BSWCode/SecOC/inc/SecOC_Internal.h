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
 **  @file               : SecOC_Internal.h
 **  @author             : HuRongbo
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Type definitions of SecOC
 **
 ***********************************************************************************************************************/

/* ================================================ misar justifications ============================================ */
#ifndef SECOC_INTERNAL_H
#define SECOC_INTERNAL_H

/* =================================================== inclusions =================================================== */
#include "SecOC.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */

/** Maximum length of the authenticator */
#define SECOC_MAX_AUTHENTICATOR_LEN (16u + 1u)
/** Maximum length of the received secured message */
#define SECOC_MAX_RX_SECURED_MSG_LENGTH 100u

/** Length of the Data ID */
#define SECOC_DATAID_LEN 2uL

/** Event definitions for transmission */
#define SECOC_EVENT_TX_ALL 0x00FFu
/** Transmission request event */
#define SECOC_TX_REQUEST 0x0001u
/** Data acquired for transmission event */
#define SECOC_TX_DATA_ACQUIRED 0x0002u
/** Authentic PDU confirmed event */
#define SECOC_TX_AUTHENTIC_PDU_CONFIRMED 0x0004u
/** Cryptographic PDU confirmed event */
#define SECOC_TX_CRYPTOGRAPHIC_PDU_CONFIRMED 0x0008u
/** Generate finish indication waiting event */
#define SECOC_TX_GENERATE_FINISH_IND_WAITING 0x0010u
/** Generate finish indication confirmed event */
#define SECOC_TX_GENERATE_FINISH_IND_CONFIRMED 0x0020u
/** Transmit retry event */
#define SECOC_TX_TRANSMIT_RETRY 0x0040u

/** Event definitions for reception */
#define SECOC_EVENT_RX_ALL 0x03FFu
/** Reception indication event */
#define SECOC_RX_INDICATION 0x0001u
/** Secured PDU received event */
#define SECOC_RX_SECURED_PDU 0x0002u
/** Authentic PDU received event */
#define SECOC_RX_AUTHENTIC_PDU 0x0004u
/** Cryptographic PDU received event */
#define SECOC_RX_CRYPTOGRAPHIC_PDU 0x0008u
/** Parsed message received event */
#define SECOC_RX_PARSED_MESSAGE 0x0010u
/** Verify finish indication waiting event */
#define SECOC_RX_VERIFY_FINISH_IND_WAITING 0x0020u
/** Verify finish indication confirmed event */
#define SECOC_RX_VERIFY_FINISH_IND_CONFIRMED 0x0040u
/** TP data received from lower layer event */
#define SECOC_RX_TP_DATA_FROM_LOWER 0x0080u
/** TP data transferred to upper layer event */
#define SECOC_RX_TRANS_TP_DATA_TO_UPPER 0x0100u
/** TP data received from lower layer event */
#define SECOC_RX_TP_CRYPTO_DATA_FROM_LOWER 0x0200u
/* PRQA S 3472,3469 ++ */ /* VL_SecOC_3472,VL_SecOC_3469 */

/**
 * Returns the remainder result, only for powers of 2 where the divisor is
 */
#define SECOC_MOD_SHIFT(dividend, divisor) ((dividend) & ((divisor) - 1u))

/**
 * @brief       Calculates the module of a given 16-bit dividend by 8.
 * @param[in]   dividend   : The 16-bit value to be divided by 8.
 * @return      uint8
 * @retval      uint8 : The remainder of the division of the dividend by 8.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70355
 */
static inline uint8 SECOC_MOD_FROM_EIGHT(uint16 dividend)
{
    return (uint8)(dividend)&SECOC_CONST_7U;
}

/**
 * @brief       Calculates the module of a given 16-bit dividend by 8.
 * @param[in]   dividend   : The 16-bit value to be divided by 8.
 * @param[in]   bits    : The number of bits to be shifted.
 * @return      uint8
 * @retval      uint8 : The remainder of the division of the dividend by 8.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70356
 */
static inline uint16 SECOC_DIV_SHIFT(uint16 dividend, uint16 bits)
{
    return (dividend) >> (bits);
}

/**
 * @brief       Converts the number of bits to the corresponding number of bytes.
 * @param[in]   nbits   : The number of bits to be converted.
 * @return      uint16
 * @retval      uint16 : The number of bytes equivalent to the given number of bits.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70357
 */
static inline uint16 SECOC_BIT_TO_BYTE(uint16 nbits)
{
    return SECOC_DIV_SHIFT(((nbits) + SECOC_CONST_7U), SECOC_CONST_3U);
}

/* Macro used to get configuration data */
/* PRQA S 3472,3469 -- */

/* ================================================ type definitions ================================================ */
/**
 * @brief   States of the SecOC module.
 */
typedef enum
{
    SECOC_TX_NONE,  /**< No transmission @range 0x00 */
    SECOC_TX_CALC,  /**< Calculate the authenticator @range 0x01 */
    SECOC_TX_TRANS, /**< Transmit the secured message  @range 0x02 */
    SECOC_TX_RETRY, /**< Retry transmission @range 0x03 */
    SECOC_TX_CANCEL /**< Cancel transmission @range 0x04 */
} SecOC_TxStatusType;

/**
 * @brief   States of the SecOC module.
 */
typedef enum
{
    SECOC_RX_NONE, /**< No reception  @range 0x00 */
    SECOC_RX_CALC, /**< Calculate the authenticator @range 0x01 */
    SECOC_RX_RETRY /**< Retry reception @range 0x02 */
} SecOC_RxStatusType;

#if (SECOC_TX_PDU_PRO_NUM > SECOC_CONST_0U)

/**
 * @brief  Runtime data of the SecOC module for transmission.
 *
 */
typedef struct
{
    boolean txDefaultAuthInfoEnable; /**< Default authentication information enable @range true/false */
    boolean txPduReady;              /**< Pdu ready @range true/false */
    uint8   retryTransmitNum;        /**< retry number @range 0x00 - 0xFF */
    uint8   txResultBuffer[SECOC_MAX_AUTHENTICATOR_LEN]; /**< Result buffer @range 0x00 - 0xFF */
    SecOC_FreshnessArrayType txFreshnessValue;           /**< Freshness value @range 0x00 - 0xFF */
    uint16                   SecOC_TxEvent;              /**< Event @range 0x00 - 0xFFFF */
    SecOC_TxStatusType       txPduStatus;                /**< Pdu status @range SECOC_TX_NONE ~ SECOC_TX_CANCEL */
    PduLengthType            txPduLength;                /**< Pdu length @range 0x00 - 0xFFFF */
    PduLengthType            txSecuredPduLen;            /**< Secured Pdu length @range 0x00 - 0xFFFF */
    PduLengthType            txAuthenticPduLen;          /**< Secured Pdu length @range 0x00 - 0xFFFF */
    PduLengthType            txCryptographicPduLen;      /**< Cryptographic Pdu length @range 0x00 - 0xFFFF */
    uint16                   txAuthBuildCnt;             /**< Authentication build count @range 0x00 - 0xFFFF */
    uint32                   txResultLength;             /**< Result length  @range 0x00 - 0xFFFFFFFF*/
#if (SECOC_TX_TP_PDU_NUM > SECOC_CONST_0U)
    PduLengthType tpTxPduOffset;       /**< TP TX PDU offset. @range 0x00 - 0xFFFF */
    PduLengthType tpTxCryptoPduOffset; /**< TP TX PDU offset. @range 0x00 - 0xFFFF */
#endif
} SecOC_TxPduRunTimeType;
#endif /* SECOC_TX_PDU_PRO_NUM > 0u */

#if (SECOC_RX_PDU_PRO_NUM > SECOC_CONST_0U)
/**
 * @brief SecOC Rx queue management structure
 *
 */
typedef struct
{
    uint16        queueInfoIdx;    /**< Queue information index  @range 0x00 - 0xFFFF */
    PduLengthType freeQueSize;     /**< Free queue size @range 0x00 - 0xFFFF */
    uint16        freeQueStartIdx; /**< Free queue start index @range 0x00 - 0xFFFF */
    uint16        queueFrameCnt;   /**< Queue frame count @range 0x00 - 0xFFFF */
    uint16        queueHead;       /**< Queue head @range 0x00 - 0xFFFF */
    uint16        queueTail;       /**< Queue tail  @range 0x00 - 0xFFFF */
} SecOC_QueueManageType;

/**
 * @brief  SecOC Rx queue management structure
 *
 */
typedef struct
{
    boolean rxSkipVerification; /**< Whether to skip verification of the Rx packet. @range true/false */
    uint8   rxParsedFreshnessValue[SECOC_CONST_8U];             /**< Max 64 bits @range SECOC_CONST_8U */
    uint8   rxParsedAuthenticator[SECOC_MAX_AUTHENTICATOR_LEN]; /**< Max 128 bits @range SECOC_MAX_AUTHENTICATOR_LEN */
    uint8   rxNumOfMsgToOverride;                       /**< Number of messages to override @range 0x00 - 0xFF */
    uint8   rxBakNumOfMsgToOverride;                    /**< Backup number of messages to override @range 0x00 - 0xFF */
    uint8*  rxParsedAuthPduBufPtr;                      /**< Parsed authentic Pdu buffer pointer @range NA*/
    uint8*  tpRxDestBufPtr;                             /**< TP RX destination buffer pointer @range NA */
    uint8*  tpRxCryptoDestBufPtr;                       /**< TP RX destination buffer pointer @range NA */
    uint16  SecOC_RxEvent;                              /**< Event @range 0x00 - 0xFFFF */
    uint16  rxAuthBuildAttempts;                        /**< Authentication build attempts @range 0x00 - 0xFFFF */
    uint16  rxVerifyAttempts;                           /**< Verify attempts @range 0x00 - 0xFFFF */
    SecOC_RxStatusType           rxPduStatus;           /**< Pdu status @range SECOC_RX_NONE ~ SECOC_RX_RETRY */
    PduIdType                    curPduId;              /**< Current Pdu id @range 0x00 - 0xFFFF */
    PduIdType                    curCryptoPduId;        /**< Current Pdu id @range 0x00 - 0xFFFF */
    PduLengthType                rxSecuredPduLen;       /**< Length of the secured Pdu @range 0x00 - 0xFFFF */
    PduLengthType                rxAuthenticPduLen;     /**< Length of the authentic Pdu @range 0x00 - 0xFFFF */
    PduLengthType                rxCryptographicPduLen; /**< Length of the cryptographic Pdu  @range 0x00 - 0xFFFF */
    PduLengthType                rxParsedAuthPduLen;    /**< Length of the parsed authentic Pdu @range 0x00 - 0xFFFF */
    SecOC_QueueManageType        queueManage;           /**< Queue management @range NA */
    SecOC_FreshnessArrayType     rxFreshnessValue;      /**< Freshness value @range 0x00 - 0xFF */
    Crypto_VerifyResultType      rxVerifyResult;        /**< Verify result @range 0x00 - 0xFF */
    SecOC_OverrideStatusType     rxOverrideStatus;      /**< Override status @range 0x00 - 0xFF */
    SecOC_VerificationResultType rxVeryfyResult;        /**< Verify result  @range 0x00 - 0xFF */
#if (SECOC_RX_TP_PDU_NUM > SECOC_CONST_0U)
    PduLengthType sduRemaining;        /**< SDU remaining @range 0x00 - 0xFFFF */
    PduLengthType sduCryptoRemaining;  /**< SDU remaining @range 0x00 - 0xFFFF */
    PduLengthType upLayerBufSize;      /**< Up layer buffer size @range 0x00 - 0xFFFF */
    PduLengthType tpRxPduOffset;       /**< TP RX PDU offset @range 0x00 - 0xFFFF */
    PduLengthType tpRxCryptoPduOffset; /**< TP RX PDU offset @range 0x00 - 0xFFFF */
#endif
} SecOC_RxPduRunTimeType;
#endif /* SECOC_RX_PDU_PRO_NUM > 0u */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

#ifdef __cplusplus
}
#endif
#endif /* SECOC_INTERNAL_H */
