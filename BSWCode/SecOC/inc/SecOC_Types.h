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
 **  @file               : SecOC_Types.h
 **  @author             : HuRongbo
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Type definitions of SecOC
 **
 ***********************************************************************************************************************/

/* ================================================ misar justifications ============================================ */
#ifndef SECOC_TYPES_H
#define SECOC_TYPES_H

#include "Os.h"
#include "ComStack_Types.h"
#include "Rte_SecOC_Type.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */
/** Unsigned const number */
#define SECOC_CONST_0U 0x0u
/** Unsigned const number */
#define SECOC_CONST_1U 0x1u
/** Unsigned const number */
#define SECOC_CONST_2U 0x2u
/** Unsigned const number */
#define SECOC_CONST_3U 0x3u
/** Unsigned const number */
#define SECOC_CONST_4U 0x4u
/** Unsigned const number */
#define SECOC_CONST_7U 0x7u
/** Unsigned const number */
#define SECOC_CONST_8U 0x8u

/** Defined data values FF */
#define SECOC_CONST_FFU 0xFFu
/** SecOC_QueryFreshnessValueType */
#define SECOC_CFUNC 0x00u
/** SecOC_QueryFreshnessValueType */
#define SECOC_RTE 0x01u
/* ================================================ type definitions ================================================ */

/**
 * @brief    Define SecOC Buffer configuration that may be used by a collection of Pdus.
 */
typedef struct
{
    uint32   SecOCBufferLength;    /**< Length of same buffer. @range 0x00 ~ 0xFFFFFFFF */
    boolean* SecOCSameBufferInUse; /**< Indicate that the same buffer is in use. @range 0x00 ~ 0x01 */
} SecOC_SameBufferPduCollectionType;

/**
 * @brief    Define strategy for handling PDUs when the received buffer overflows.
 */
typedef enum
{
    SECOC_QUEUE,  /**< Queue the PDU. @range 0x00 */
    SECOC_REJECT, /**< Reject the PDU. @range 0x01 */
    SECOC_REPLACE /**< Place the PDU in the buffer. @range 0x02 */
} SecOC_ReceptionOverflowStrategyType;

/**
 * @brief    Define which authentication state is passed to the SWC.
 */
typedef enum
{
    SECOC_BOTH,         /**< Both True and False AuthenticationStatus is propagated to SWC. @range 0x00 */
    SECOC_FAILURE_ONLY, /**< Only False AuthenticationStatus is propagated to SWC. @range 0x01 */
    SECOC_NONE          /**< No AuthenticationStatus is propagated to SWC @range 0x02 */
} SecOC_VeriStatusPropModeType;

/**
 * @brief    Define the type of API.
 */
typedef enum
{
    SECOC_IFPDU, /**< Indicate the Pdu is an I-F PDU. @range 0x00 */
    SECOC_TPPDU  /**< Indicate the Pdu is a TP PDU. @range 0x01 */
} SecOC_PduType;

/**
 * @brief    Define the encryption and decryption process, synchronous or asynchronous.
 */
typedef enum
{
    SECOC_CRYPTO_PROCESSING_ASYNC, /**< Indicate the encryption and decryption process is asynchronous. @range 0x00 */
    SECOC_CRYPTO_PROCESSING_SYNC   /**< Indicate the encryption and decryption process is synchronous. @range 0x01 */
} SecOC_CryptoProcessingType;

/**
 * @brief    Define the Csm primitive, MAC or SIGNATURE.
 */
typedef enum
{
    SECOC_CSMMAC,      /**< Indicate the Csm primitive is MAC. @range 0x00 */
    SECOC_CSMSIGNATURE /**< Indicate the Csm primitive is SIGNATURE. @range 0x01 */
} SecOC_CsmPrimitivesType;

/**
 * @brief    Define information about the Csm job.
 */
typedef struct
{
    uint32                     SecOCJobId;                /**< Csm job identifier of SecOC. @range 0x00 ~ 0xFFFFFFFF */
    SecOC_CryptoProcessingType SecOCCryptoProcessingType; /**< Indicate the encryption and decryption process,
                                                             synchronous or asynchronous. @range 0x00 ~ 0x01 */
    SecOC_CsmPrimitivesType SecOCCsmPrimitives; /**< Indicate the Csm primitive, MAC or SIGNATURE. @range 0x00 ~ 0x01 */
} SecOC_CsmJobType;

/**
 * @brief    Define the Pdu that is received by the SecOC module from the PduR.
 */
typedef struct
{
    uint8
        SecOCAuthPduHeaderLength; /**< Indicate the length (in bytes) of the Secured I-PDU Header in the Secured I-PDU.
                                   * @range 0x00 ~ 0xFF
                                   */
    PduIdType SecOCRxSecuredLayerPduId;     /**< Rx Secured Pdu identifier. @range 0x00 ~ 0xFFFF */
    PduIdType SecOCRxPduRAsLowerLayerPduId; /**< Save lower layer pduid. @range 0x00 ~ 0xFFFF */
    boolean SecOCSecuredRxPduVerification;  /**< Indicate whether the signature authentication or MAC verification shall
                                               be   performed on this Secured I-PDU. @range 0x00 ~ 0x01 */
    boolean SecOCDynamicLength;   /**< Indicate whether Rx Secured Pdu supports dynamic length. @range 0x00 ~ 0x01 */
    PduLengthType SecOCPduLength; /**< Length of the Rx Secured Pdu. @range 0x00 ~ 0xFFFF */
    uint16        SecOCRxSecuredPduBuffLength; /**< Length of the buffer of Rx Secured Pdu. @range 0x00 ~ 0xFFFF */
    uint8*        SecOCRxSecuredPduBufferRef;  /**< Buffer of Rx Secured Pdu. @range NA */
} SecOC_RxSecuredPduType;

/**
 * @brief    Define the Authentic Pdu that is received by the SecOC module from the PduR.
 */
typedef struct
{
    uint8
        SecOCAuthPduHeaderLength; /**< Indicate the length (in bytes) of the Secured I-PDU Header in the Secured I-PDU.
                                   * @range 0x00 ~ 0xFFFF
                                   */
    PduIdType SecOCRxAuthenticPduId;        /**< Rx Authentic Pdu identifier. @range 0x00 ~ 0xFFFF */
    PduIdType SecOCRxPduRAsLowerLayerPduId; /**< Save lower layer pduid. @range 0x00 ~ 0xFFFF */
    boolean   SecOCDynamicLength; /**< Indicate whether Rx Authentic Pdu supports dynamic length. @range 0x00 ~ 0x01 */
    PduLengthType SecOCPduLength; /**< Length of the Rx Authentic Pdu. @range 0x00 ~ 0xFFFF */
    uint16 SecOCRxColAuthenticPduBuffLength; /**< Length of the buffer of Rx Authentic Pdu. @range 0x00 ~ 0xFFFF */
    uint8* SecOCRxColAuthenticPduBufferRef;  /**< Point to the buffer of Rx Authentic Pdu. @range NA */
} SecOC_RxAuthenticPduType;

/**
 * @brief    Define the Cryptographic Pdu that is received by the SecOC module from the PduR.
 */
typedef struct
{
    PduIdType SecOCRxCryptographicPduId;    /**< Rx Cryptographic Pdu identifier. @range 0x00 ~ 0xFFFF */
    PduIdType SecOCRxPduRAsLowerLayerPduId; /**< Save lower layer pduid. @range 0x00 ~ 0xFFFF */
    boolean
        SecOCDynamicLength; /**< Indicate whether Rx Cryptographic Pdu supports dynamic length. @range 0x00 ~ 0x01 */
    PduLengthType SecOCPduLength;             /**< Length of the Rx Cryptographic Pdu. @range 0x00 ~ 0xFFFF */
    uint16 SecOCRxCryptographicPduBuffLength; /**< Length of the buffer of Rx Cryptographic Pdu. @range 0x00 ~ 0xFFFF */
    uint8* SecOCRxCryptographicPduBufferRef;  /**< Point to the buffer of Rx Cryptographic Pdu. @range NA */
} SecOC_RxCryptographicPduType;

/**
 * @brief    Define information about the message linker.SecOC links an Authentic I-Pdu
 *           and Cryptographic I-Pdu together by repeating a specific part (Message Linker)
 *           of the Authentic I-Pdu in the Cryptographic I-Pdu.
 */
typedef struct
{
    uint16 SecOCMessageLinkLen; /**< Length of the Message Linker inside the Authentic I-PDU in bits. @range 0x00 ~
                                   0xFFFF */
    uint16 SecOCMessageLinkPos; /**< The position of the Message Linker inside the Authentic I-PDU in bits. @range 0x00
                                   ~ 0xFFFF */
} SecOC_UseMessageLinkType;

/**
 * @brief    Define information about the queue where PDUs reside.
 */
typedef struct
{
    PduLengthType pduLength; /**< Length of Pdu. @range 0x00 ~ 0xFFFF */
    PduIdType     pduId;     /**< Pdu identifier. @range 0x00 ~ 0xFFFF */
} SecOC_QueueInfoType;

/**
 * @brief    Define the processing information of the Rx Pdu.
 */
typedef struct
{
    uint16 SecOCAuthDataFreshnessLen; /**< Indicate the length of the external authentic PDU data in bits (uint16).
                                         @range 0x00 ~ 0xFFFF */
    uint16 SecOCAuthDataFreshnessStartPosition; /**< Indicate the start position in bits (uint16) of the Authentic PDU
                                                   that shall be passed on to the Freshness SWC. @range 0x00 ~ 0xFFFF */
    uint16 SecOCAuthenticationBuildAttempts;    /**< Indicate the number of authentication build attempts. @range 0x00 ~
                                                   0xFFFF */
    uint16 SecOCAuthenticationVerifyAttempts; /**< specifies the number of authentication verify attempts that are to be
                                                 carried out when the verification of the authentication information
                                                 failed for a given Secured I-PDU.  @range 0x00 ~ 0xFFFF */
    SecOC_ReceptionOverflowStrategyType
           SecOCReceptionOverflowStrategy; /**< Indicate the overflow strategy for receiving PDUs. @range 0x00 ~ 0x02 */
    uint16 SecOCReceptionQueueSize;        /**< Indicate the number of PDUs that can be received in the receive queue.
        @range 0x00 ~ 0xFFFF */
    uint16 SecOCReceptionPduQueueSize;     /**< Indicate the queue size(in bytes) in case the overflow strategy for
                                                     receiving PDUs is set to QUEUE. @range 0x00 ~ 0xFFFF */
    uint8*               SecOCReceptionQueueRef;     /**< Point to the receive queue. @range NA */
    SecOC_QueueInfoType* SecOCReceptionQueueInfoRef; /**< Indicate the PDU information in the queue. @range NA */
    boolean SecOCUseAuthDataFreshness; /**< Indicate if a part of the Authentic-PDU shall be passed on to the SWC that
                                          verifies and generates the Freshness. @range 0x00 ~ 0x01 */
    const SecOC_CsmJobType* SecOCRxAuthServiceConfigRef; /**< Indicate which crypto service function is called for
                                                            authentication.   @range NA */
    const SecOC_SameBufferPduCollectionType*
        SecOCSameBufferPduRef; /**< Used for collecting Pdus that are using the same SecOC buffer. @range NA */
} SecOC_RxPduProcessingType;

/**
 * @brief    Define the processing information of the Tx Pdu.
 */
typedef struct
{
    uint16 SecOCAuthenticationBuildAttempts; /**< Indicate the number of authentication build attempts. @range 0x00 ~
                                                0xFFFF */
    boolean SecOCProvideTxTruncatedFreshnessValue; /**< specifies if the Tx query freshness function provides the
                                                      truncated freshness info instead of generating this by SecOC.
                                                      @range 0x00 ~ 0x01 */
    uint8 SecOCTxPduUnusedAreasDefault; /**< The AUTOSAR SecOC module fills not used areas of a transmitted Secured Pdu
                                           or a transmitted Cryptographic Pdu with this byte pattern. @range 0x00 ~ 0xFF
                                         */
    boolean SecOCUseTxConfirmation; /**< Indicate if the function SecOC_SPduTxConfirmation shall be called for this PDU.
                                       @range 0x00 ~ 0x01 */
    const SecOC_SameBufferPduCollectionType* SecOCSameBufferPduRef; /**< Point to the buffer that used to collect Pdus
                                                                       using the same SecOC buffer. @range NA */
    const SecOC_CsmJobType* SecOCTxAuthServiceConfigRef; /**< Indicate which crypto service function is called for
                                                            authentication. @range NA */
} SecOC_TxPduProcessingType;

/**
 * @brief    When Secured I-Pdu is received in Authentic I-Pdu and CryptoGraphic I-Pdu,
 *           specify all information of this Secured I-Pdu.
 */
typedef struct
{
    boolean SecOCSecuredRxPduVerification; /**< Indicate whether the signature authentication or MAC verification shall
                                              be performed on this Secured I-PDU. @range 0x00 ~ 0x01 */
    const SecOC_RxAuthenticPduType*     SecOCRxAuthenticPdu;     /**< Point to the Tx Authentic Pdu. @range NA */
    const SecOC_RxCryptographicPduType* SecOCRxCryptographicPdu; /**< Point to the Rx Cryptographic Pdu. @range NA */
    const SecOC_UseMessageLinkType*     SecOCUseMessageLink; /**< Point to the message linker.Used to link an Authentic
                                                                I-PDU     and Cryptographic I-PDU. @range NA */
} SecOC_RxSecuredPduCollectionType;

/**
 * @brief    Define information about the two types of Rx Secured I-PDUs.
 */
typedef struct
{
    const SecOC_RxSecuredPduType*
        SecOCRxSecuredPdu; /**< Point to the Pdu that is received by the SecOC module from the
                              PduR. For this Pdu the Mac verification is provided. @range NA */
    const SecOC_RxSecuredPduCollectionType*
        SecOCRxSecuredPduCollection; /**< Point to two Pdus that are received by the SecOC module from the PduR and a
                                        message linking between them. @range NA */
} SecOC_RxSecuredPduLayerType;

/**
 * @brief    Define an area in the Authentic I-Pdu that will be the input to the
 *           Authenticator verification algorithm.
 */
typedef struct
{
    uint32 SecOCSecuredRxPduLength; /**< Indicate the length(in bytes) of the area within the Pdu which is secured.
                                       @range 0x00 ~ 0xFFFFFFFF */
    uint32 SecOCSecuredRxPduOffset; /**< Indicate the start position (offset in bytes) of the area within the Pdu which
                                       is secured. @range 0x00 ~ 0xFFFFFFFF */
} SecOC_RxPduSecuredAreaType;

/**
 * @brief    Define the Pdu that is received by the SecOC module from the PduR.
 */
typedef struct
{
    PduIdType     SecOCRxPduRAsUpLayerId; /**< the up layer pdu id. @range 0x00 ~ 0xFFFF */
    SecOC_PduType SecOCPduType;   /**< Indicate API Type to use for communication with PduR. @range 0x00 ~ 0x01 */
    PduLengthType SecOCPduLength; /**< Length of the Pdu. @range 0x00 ~ 0xFFFF */
} SecOC_RxAuthenticPduLayerType;

/**
 * @brief    Define the information at the post-build phase to configure the RxPdus to be verified by the
 *           SecOC module.
 */
typedef struct
{
    uint16 SecOCAuthInfoTruncLength; /**< Indicate the length in bits of the authentication code to be included in the
                                        payload of the Secured I-Pdu. @range 0x00 ~ 0xFFFF */
    uint16 SecOCDataId; /**< Indicate a unique numerical identifier for the Secured IPDU. @range 0x00 ~ 0xFFFF */
    uint16 SecOCFreshnessValueId; /**< Indicate the Id of the Freshness Value. @range 0x00 ~ 0xFFFF */
    uint8
        SecOCFreshnessValueLength; /**< Indicate the complete length in bits of the Freshness Value. @range 0x00 ~ 0xFF
                                    */
    uint8 SecOCFreshnessValueTruncLength; /**< Indicate the length in bits of the Freshness Value to be included in the
                                             payload of the Secured I-Pdu. @range 0x00 ~ 0xFF */
    ApplicationType ChannelApplicationId; /**< partition identifier. @range 0x00 ~ 0xFFFF */
    SecOC_VeriStatusPropModeType
        SecOCClientServerVerificationStatusPropagationMode; /**< Determine the propagation of the verification status
                                                               through the client/server interface to an SW-C.
                                                               @range 0x00 ~ 0x01 */
    SecOC_VeriStatusPropModeType
        SecOCVerificationStatusPropagationMode; /**< describe the propagation of the status of each verification attempt
                                                   from the SecOC module to SWCs . @range 0x00 ~ 0x01 */
    const SecOC_RxAuthenticPduLayerType*
        SecOCRxAuthenticPduLayer; /**< Point the Pdu that is transmitted by the SecOC
                                     module to the PduR after the Mac was verified. @range NA */
    const SecOC_RxPduSecuredAreaType*
        SecOCRxPduSecuredArea; /**< Point to the area in the Authentic I-Pdu that will be the input to the Authenticator
                                  verification algorithm.  @range NA */
    const SecOC_RxSecuredPduLayerType* SecOCRxSecuredPduLayer; /**< Point to the Rx Secured Pdu. @range NA */
    PduLengthType SecOCAuthenticPduBuffLength; /**< Length of buffer of the Authentic I-Pdu. @range 0x00 ~ 0xFFFF  */
    uint8*        SecOCAuthenticPduBufferRef;  /**< Point to the buffer of the Authentic I-Pdu. @range NA */
    uint16 SecOCAuthFrsInfoLength; /**< ((SecOCAuthInfoTruncLength + SecOCFreshnessValueTruncLength) + 7)/8. @range 0x00
                                      ~ 0xFFFF */
    uint8* SecOCRxAuthDataFreshnessBufferRef; /**< Point to the buffer of Rx Authentic Pdu. @range NA */
} SecOC_PbRxPduProcessingType;

/**
 * @brief    Define one Pdu that is transmitted by the SecOC module to the PduR
 *           after the Mac was generated.
 */
typedef struct
{
    uint8 SecOCAuthPduHeaderLength; /**< This parameter indicates the length (in bytes) of the Secured I-Pdu Header in
                                       the Secured I-Pdu. The length of zero means there's no header in the Pdu.
                                       @range 0x00 ~ 0xFF */
    PduIdType     SecOCTxSecuredLayerPduId;    /**< Pdu identifier assigned by SecOC module. @range 0x00 ~ 0xFFFF */
    PduIdType     SecOCTxPduRAsLowerLayerId;   /**< Save lower layer pduid. @range 0x00 ~ 0xFFFF */
    PduLengthType SecOCTxSecuredPduBuffLength; /**< Length of buffer of Secured Pdu that will be sent to PduR.
                                                @range 0x00 ~ 0xFFFF */
    uint8* SecOCTxSecuredPduBufferRef; /**< Point to the buffer of Secured Pdu that will be sent to PduR. @range NA */
} SecOC_TxSecuredPduType;

/**
 * @brief    Define the Pdu (that is transmitted by the SecOC module to the PduR)
 *           which contains the Secured I-Pdu Header and the Authentic I-Pdu.
 */
typedef struct
{
    uint8 SecOCAuthPduHeaderLength;  /**< This parameter indicates the length (in bytes) of the Secured I-Pdu Header in
                                        the Secured I-Pdu. The length of zero means there's no header in the Pdu.
                                        @range 0x00 ~ 0xFF */
    PduIdType SecOCTxAuthenticPduId; /**< Pdu identifier of the Authentic I-Pdu assigned by SecOC module. @range 0x00 ~
                                        0xFFFF */
    PduIdType     SecOCTxPduRAsLowerLayerId;        /**< Save lower layer pduid. @range 0x00 ~ 0xFFFF */
    PduLengthType SecOCTxColAuthenticPduBuffLength; /**< Length of buffer of Authentic Pdu that has been built with
                                                       authentication information. @range 0x00 ~ 0xFFFF */
    uint8* SecOCTxColAuthenticPduBufferRef;         /**< Point to the buffer of Authentic Pdu that has been built with
                                                       authentication information. @range NA */
} SecOC_TxAuthenticPduType;

/**
 * @brief    Define the Cryptographic Pdu that is transmitted.
 */
typedef struct
{
    PduIdType SecOCTxCryptographicPduId; /**< Pdu identifier of the Cryptographic I-Pdu assigned by SecOC module.
                                        @range 0x00 ~ 0xFFFF */
    PduIdType SecOCTxPduRAsLowerLayerId; /**< Save lower layer pduid. @range 0x00 ~ 0xFFFF */
    PduLengthType
           SecOCTxCryptographicPduBuffLength; /**< Length of the buffer of Cryptographic Pdu. @range 0x00 ~ 0xFFFF */
    uint8* SecOCTxCryptographicPduBufferRef;  /**< Point to the buffer of Cryptographic Pdu. @range NA */
} SecOC_TxCryptographicPduType;

/**
 * @brief    Define the Pdu that is transmitted by the SecOC module to the PduR
 *           after the Mac was generated.Two separate Pdus are transmitted to the
 *           PduR:Authentic I-Pdu and Cryptographic I-Pdu.
 */
typedef struct
{
    const SecOC_TxAuthenticPduType* SecOCTxAuthenticPdu; /**< Point to the Indicate the Pdu which contains the Secured
                                                            I-Pdu Header and the Authentic I-Pdu. @range NA */
    const SecOC_TxCryptographicPduType*
        SecOCTxCryptographicPdu; /**< Point to the Cryptographic Pdu that is transmitted. @range NA */
    const SecOC_UseMessageLinkType* SecOCUseMessageLink; /**< Point to the message linker.Used to link an Authentic
                                                            I-PDU and Cryptographic I-PDU. @range NA */
} SecOC_TxSecuredPduCollectionType;

/**
 * @brief    Define the Pdu that is transmitted by the SecOC module to the PduR
 *           after the Mac was generated.
 */
typedef struct
{
    const SecOC_TxSecuredPduType* SecOCTxSecuredPdu; /**< Point to the Pdu that is transmitted by the SecOC module to
                                                        the PduR after the Mac was generated. @range NA */
    const SecOC_TxSecuredPduCollectionType*
        SecOCTxSecuredPduCollection; /**< Point to the Pdu that is transmitted by the SecOC module to the PduR after the
                                        Mac was generated. @range NA */
} SecOC_TxSecuredPduLayerType;

/**
 * @brief    Define an area in the Authentic I-Pdu that will be the input to
 *           the Authenticator generation algorithm.
 */
typedef struct
{
    uint32 SecOCSecuredTxPduLength; /**< Indicate the length (in bytes) of the area within the Pdu which shall be
                                       secured. @range 0x00 ~ 0xFFFFFFFF */
    uint32 SecOCSecuredTxPduOffset; /**< Indicate the start position (offset in bytes) of the area within the Pdu which
                                       shall be secured. @range 0x00 ~ 0xFFFFFFFF */
} SecOC_TxPduSecuredAreaType;

/**
 * @brief    Define the Pdu that is transmitted by the SecOC module to the PduR
 *           after the Mac was verified.
 */
typedef struct
{
    SecOC_PduType SecOCPduType; /**< Indicate API Type to use for communication with PduR. @range 0x00 ~ 0x01 */
    PduIdType     SecOCTxAuthenticLayerPduId; /**< Pdu identifier assigned by SecOC module. @range 0x00 ~ 0xFFFF */
    PduIdType     SecOCTxPduRAsUpLayerId;     /**< Save up layer pduid. @range 0x00 ~ 0xFFFF */
} SecOC_TxAuthenticPduLayerType;

/**
 * @brief    Define the information at the post-build phase to configure the TxPdus
 *           to be secured by the SecOC module.
 */
typedef struct
{
    uint16 SecOCAuthInfoTruncLength; /**< Indicate the length in bits of the authentication code to be included in the
                                        payload of the Secured I-Pdu. @range 0x00 ~ 0xFFFF */
    uint16 SecOCDataId; /**< Indicate a unique numerical identifier for the Secured IPDU. @range 0x00 ~ 0xFFFF */
    uint16 SecOCFreshnessValueId; /**< Indicate the Id of the Freshness Value. @range 0x00 ~ 0xFFFF */
    uint8
        SecOCFreshnessValueLength; /**< Indicate the complete length in bits of the Freshness Value. @range 0x00 ~ 0xFF
                                    */
    uint8 SecOCFreshnessValueTruncLength; /**< Indicate the length in bits of the Freshness Value to be included in the
                                             payload of the Secured I-Pdu. @range 0x00 ~ 0xFF */
    boolean SecOCReAuthenticateAfterTriggerTransmit; /**< specifies if the authentication information of the Secured PDU
                                                        is updated after the successful transmission of a triggered
                                                        transmission was confirmed. @range 0x00 ~ 0x01 */
    ApplicationType                      ChannelApplicationId; /**< The partition identifier. @range 0x00 ~ 0xFFFF */
    const SecOC_TxAuthenticPduLayerType* SecOCTxAuthenticPduLayer; /**< Point to the the Pdu that is received by the
                                                                      SecOC module from the PduR. @range NA */
    const SecOC_TxPduSecuredAreaType*
        SecOCTxPduSecuredArea; /**< Point to the area in the Authentic I-Pdu that will be
                                  the input to the Authenticator generation algorithm. @range NA */
    const SecOC_TxSecuredPduLayerType* SecOCTxSecuredPduLayer; /**< Point to the Tx Secured Pdu. @range NA */
    PduLengthType SecOCAuthenticPduBuffLength; /**< Length of buffer of the Authentic I-Pdu. @range 0x00 ~ 0xFFFF */
    uint8*        SecOCAuthenticPduBufferRef;  /**< Point to buffer of the Authentic I-Pdu. @range NA */
    uint16 SecOCAuthFrsInfoLength; /**< ((SecOCAuthInfoTruncLength + SecOCFreshnessValueTruncLength) + 7)/8. @range 0x00
                                      ~ 0xFFFF */
} SecOC_PbTxPduProcessingType;

/**
 * @brief    Define configuration information at the post-build phase for the SecOC module.
 */
/* PRQA S 1759 ++ */ /* VL_SecOC_1759 */
typedef struct
{
    const SecOC_PbRxPduProcessingType*
        SecOC_PbRxPduProcessing; /**< Point to the Rx Pdu to be verified by SecOC. @range NA */
    const SecOC_PbTxPduProcessingType*
        SecOC_PbTxPduProcessing; /**< Point to the Rx Pdu to be secured by SecOC. @range NA */
} SecOC_PbConfigType;
/* PRQA S 1759 -- */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

#ifdef __cplusplus
}
#endif
#endif /* SECOC_TYPES_H */
