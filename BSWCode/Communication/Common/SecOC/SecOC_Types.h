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
**  FILENAME    : SecOC_Types.h                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : HuRongbo                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION : Type definitions of SecOC                                   **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*  <VERSION>    <DATE>      <AUTHOR>    <REVISION LOG>
 *  V1.0.0       2020-08-18  HuRongbo    R19_11 SecOC initial version.
 *  V2.0.0       2021-06-17  HuRongbo    Bug fix.
 */

#ifndef SECOC_TYPES_H
#define SECOC_TYPES_H

#include "ComStack_Types.h"
#include "Rte_SecOC_Type.h"

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
/* States of the SecOC module */
typedef enum
{
    SECOC_UNINIT,
    SECOC_INIT
} SecOC_StateType;

/* SecOC_QueryFreshnessValueType */
#define SECOC_CFUNC 0x00u
#define SECOC_RTE   0x01u

/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
typedef struct
{
    uint32 SecOCBufferLength;
    P2VAR(boolean, TYPEDEF, SECOC_CFG_DATA) SecOCSameBufferInUse;
    uint16 SecOCAuthenticPduBuffLength;
    P2VAR(uint8, TYPEDEF, SECOC_CFG_DATA) SecOCAuthenticPduBufferRef;
    uint16 SecOCSecuredPduBuffLength;
    P2VAR(uint8, TYPEDEF, SECOC_CFG_DATA) SecOCSecuredPduBufferRef;
    uint16 SecOCColAuthenticPduBuffLength;
    P2VAR(uint8, TYPEDEF, SECOC_APPL_DATA) SecOCColAuthenticPduBufferRef;
    uint16 SecOCCryptographicPduBuffLength;
    P2VAR(uint8, TYPEDEF, SECOC_APPL_DATA) SecOCCryptographicPduBufferRef;
} SecOC_SameBufferPduCollectionType;
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

typedef enum
{
    SECOC_QUEUE,
    SECOC_REJECT,
    SECOC_REPLACE
} SecOC_ReceptionOverflowStrategyType;

typedef enum
{
    SECOC_BOTH,         /*Both True and False AuthenticationStatus is propagated to SWC*/
    SECOC_FAILURE_ONLY, /*Only False AuthenticationStatus is propagated to SWC*/
    SECOC_NONE          /*No AuthenticationStatus is propagated to SWC*/
} SecOC_VeriStatusPropModeType;

typedef enum
{
    SECOC_IFPDU,
    SECOC_TPPDU
} SecOC_PduType;

typedef enum
{
    SECOC_CRYPTO_PROCESSING_ASYNC,
    SECOC_CRYPTO_PROCESSING_SYNC
} SecOC_CryptoProcessingTypeType;

typedef enum
{
    SECOC_CSMMAC,
    SECOC_CSMSIGNATURE
} SecOC_CsmPrimitivesType;

typedef struct
{
    uint32 SecOCJobId;
    SecOC_CryptoProcessingTypeType SecOCCryptoProcessingType;
    SecOC_CsmPrimitivesType SecOCCsmPrimitives;
} SecOC_CsmJobType;

typedef struct
{
    PduIdType SecOCRxPduRAsUpLayerId; /*the up layer pdu id.*/
    SecOC_PduType SecOCPduType;
    PduLengthType SecOCPduLength;
} SecOC_RxAuthenticPduLayerType;

typedef struct
{
    uint8 SecOCAuthPduHeaderLength;
    PduIdType SecOCRxSecuredLayerPduId;
    PduIdType SecOCRxPduRAsLowerLayerPduId;
    boolean SecOCSecuredRxPduVerification;
    boolean SecOCDynamicLength;
    PduLengthType SecOCPduLength;
    uint16 SecOCRxSecuredPduBuffLength;
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, TYPEDEF, SECOC_APPL_DATA) SecOCRxSecuredPduBufferRef;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
} SecOC_RxSecuredPduType;

typedef struct
{
    uint8 SecOCAuthPduHeaderLength;
    PduIdType SecOCRxAuthenticPduId;
    PduIdType SecOCRxPduRAsLowerLayerPduId;
    boolean SecOCDynamicLength;
    PduLengthType SecOCPduLength;
    uint16 SecOCRxColAuthenticPduBuffLength;
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, TYPEDEF, SECOC_APPL_DATA) SecOCRxColAuthenticPduBufferRef;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
} SecOC_RxAuthenticPduType;

typedef struct
{
    PduIdType SecOCRxCryptographicPduId;
    PduIdType SecOCRxPduRAsLowerLayerPduId;
    boolean SecOCDynamicLength;
    PduLengthType SecOCPduLength;
    uint16 SecOCRxCryptographicPduBuffLength;
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, TYPEDEF, SECOC_APPL_DATA) SecOCRxCryptographicPduBufferRef;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
} SecOC_RxCryptographicPduType;

typedef struct
{
    uint16 SecOCMessageLinkLen;
    uint16 SecOCMessageLinkPos;
} SecOC_UseMessageLinkType;

typedef struct
{
    boolean SecOCSecuredRxPduVerification;
    P2CONST(SecOC_RxAuthenticPduType, TYPEDEF, SECOC_CONST) SecOCRxAuthenticPdu;
    P2CONST(SecOC_RxCryptographicPduType, TYPEDEF, SECOC_CONST) SecOCRxCryptographicPdu;
    P2CONST(SecOC_UseMessageLinkType, TYPEDEF, SECOC_CONST) SecOCUseMessageLink;
} SecOC_RxSecuredPduCollectionType;

typedef struct
{
    P2CONST(SecOC_RxSecuredPduType, TYPEDEF, SECOC_CONST) SecOCRxSecuredPdu;
    P2CONST(SecOC_RxSecuredPduCollectionType, TYPEDEF, SECOC_CONST)
    SecOCRxSecuredPduCollection;
} SecOC_RxSecuredPduLayerType;

typedef struct
{
    uint32 SecOCSecuredRxPduLength;
    uint32 SecOCSecuredRxPduOffset;
} SecOC_RxPduSecuredAreaType;

typedef struct
{
    uint16 SecOCAuthDataFreshnessLen;
    uint16 SecOCAuthDataFreshnessStartPosition;
    uint16 SecOCAuthenticationBuildAttempts;
    uint16 SecOCAuthenticationVerifyAttempts;
    uint16 SecOCAuthInfoTruncLength;
    uint16 SecOCDataId;
    uint16 SecOCFreshnessValueId;
    uint8 SecOCFreshnessValueLength;
    uint8 SecOCFreshnessValueTruncLength;
    SecOC_ReceptionOverflowStrategyType SecOCReceptionOverflowStrategy;
    uint16 SecOCReceptionQueueSize;
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, TYPEDEF, SECOC_CFG_DATA) SecOCReceptionQueueRef;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    boolean SecOCUseAuthDataFreshness;
    SecOC_VeriStatusPropModeType SecOCVerificationStatusPropagationMode;
    P2CONST(SecOC_CsmJobType, TYPEDEF, SECOC_CONST) SecOCRxAuthServiceConfigRef;
    P2CONST(SecOC_SameBufferPduCollectionType, TYPEDEF, SECOC_CONST)
    SecOCSameBufferPduRef;
    P2CONST(SecOC_RxAuthenticPduLayerType, TYPEDEF, SECOC_CONST)
    SecOCRxAuthenticPduLayer;
    P2CONST(SecOC_RxPduSecuredAreaType, TYPEDEF, SECOC_CONST)
    SecOCRxPduSecuredArea;
    P2CONST(SecOC_RxSecuredPduLayerType, TYPEDEF, SECOC_CONST)
    SecOCRxSecuredPduLayer;
    uint16 SecOCAuthenticPduBuffLength;
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, TYPEDEF, SECOC_CFG_DATA) SecOCAuthenticPduBufferRef;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    uint16 SecOCAuthFrsInfoLength;
} SecOC_RxPduProcessingType;

typedef struct
{
    SecOC_PduType SecOCPduType;
    PduIdType SecOCTxAuthenticLayerPduId; /*PDU identifier assigned by SecOC module.*/
    PduIdType SecOCTxPduRAsUpLayerId;     /* Save up layer pduid */
} SecOC_TxAuthenticPduLayerType;

typedef struct
{
    uint32 SecOCSecuredTxPduLength;
    uint32 SecOCSecuredTxPduOffset;
} SecOC_TxPduSecuredAreaType;

typedef struct
{
    uint8 SecOCAuthPduHeaderLength;
    PduIdType SecOCTxSecuredLayerPduId;  /*PDU identifier assigned by SecOC module.*/
    PduIdType SecOCTxPduRAsLowerLayerId; /* Save lower layer pduid */
    uint16 SecOCTxSecuredPduBuffLength;
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, TYPEDEF, SECOC_APPL_DATA) SecOCTxSecuredPduBufferRef;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
} SecOC_TxSecuredPduType;

typedef struct
{
    uint8 SecOCAuthPduHeaderLength;
    PduIdType SecOCTxAuthenticPduId;
    PduIdType SecOCTxPduRAsLowerLayerId; /* Save lower layer pduid */
    uint16 SecOCTxColAuthenticPduBuffLength;
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, TYPEDEF, SECOC_APPL_DATA) SecOCTxColAuthenticPduBufferRef;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
} SecOC_TxAuthenticPduType;

typedef struct
{
    PduIdType SecOCTxCryptographicPduId;
    PduIdType SecOCTxPduRAsLowerLayerId; /* Save lower layer pduid */
    uint16 SecOCTxCryptographicPduBuffLength;
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, TYPEDEF, SECOC_APPL_DATA) SecOCTxCryptographicPduBufferRef;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
} SecOC_TxCryptographicPduType;

typedef struct
{
    P2CONST(SecOC_TxAuthenticPduType, TYPEDEF, SECOC_CONST) SecOCTxAuthenticPdu;
    P2CONST(SecOC_TxCryptographicPduType, TYPEDEF, SECOC_CONST)
    SecOCTxCryptographicPdu;
    P2CONST(SecOC_UseMessageLinkType, TYPEDEF, SECOC_CONST) SecOCUseMessageLink;
} SecOC_TxSecuredPduCollectionType;

typedef struct
{
    P2CONST(SecOC_TxSecuredPduType, TYPEDEF, SECOC_CONST) SecOCTxSecuredPdu;
    P2CONST(SecOC_TxSecuredPduCollectionType, TYPEDEF, SECOC_CONST)
    SecOCTxSecuredPduCollection;
} SecOC_TxSecuredPduLayerType;

typedef struct
{
    uint16 SecOCAuthenticationBuildAttempts;
    uint16 SecOCAuthInfoTruncLength;
    uint16 SecOCDataId;
    uint16 SecOCFreshnessValueId;
    uint8 SecOCFreshnessValueLength;
    uint8 SecOCFreshnessValueTruncLength;
    boolean SecOCProvideTxTruncatedFreshnessValue;
    boolean SecOCUseTxConfirmation;
    P2CONST(SecOC_SameBufferPduCollectionType, TYPEDEF, SECOC_CONST)
    SecOCSameBufferPduRef;
    P2CONST(SecOC_CsmJobType, TYPEDEF, SECOC_CONST) SecOCTxAuthServiceConfigRef;
    P2CONST(SecOC_TxAuthenticPduLayerType, TYPEDEF, SECOC_CONST)
    SecOCTxAuthenticPduLayer;
    P2CONST(SecOC_TxPduSecuredAreaType, TYPEDEF, SECOC_CONST)
    SecOCTxPduSecuredArea;
    P2CONST(SecOC_TxSecuredPduLayerType, TYPEDEF, SECOC_CONST)
    SecOCTxSecuredPduLayer;
    uint16 SecOCAuthenticPduBuffLength;
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, TYPEDEF, SECOC_CONST) SecOCAuthenticPduBufferRef;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    /* ((SecOCAuthInfoTruncLength + SecOCFreshnessValueTruncLength) + 7)/8 */
    uint16 SecOCAuthFrsInfoLength;
} SecOC_TxPduProcessingType;

/* PRQA S 1336 ++ */ /* MISRA Rule 8.2 */
typedef void (*SecOC_VerificationStatusCalloutType)(SecOC_VerificationStatusType);
/* PRQA S 1336 -- */ /* MISRA Rule 8.2 */

/* SWS_SecOC_00104: Configuration data structure of SecOC module */
typedef struct
{
    P2CONST(SecOC_RxPduProcessingType, TYPEDEF, SECOC_CONST)
    SecOCRxPduProcessing;
    P2CONST(SecOC_SameBufferPduCollectionType, TYPEDEF, SECOC_CONST)
    SecOCSameBufferPduCollection;
    P2CONST(SecOC_TxPduProcessingType, TYPEDEF, SECOC_CONST)
    SecOCTxPduProcessing;
    P2CONST(SecOC_VerificationStatusCalloutType, TYPEDEF, SECOC_CONST)
    SecOCVerificationStatusCallout;
    P2VAR(boolean, TYPEDEF, SECOC_APPL_DATA) SecOCIgnoreVerificationResultRef;
} SecOC_ConfigType;

#endif /* SECOC_TYPES_H */
