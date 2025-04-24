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
**  FILENAME    : Crypto_GeneralTypes.h                                       **
**                                                                            **
**  Created on  :                                                             **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION : Type header for CSM                                         **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/******************************************************************************
**                      Revision Control History                             **
******************************************************************************/
/*  <VERSION>      <DATE>      <AUTHOR>        <REVISION LOG>
 *    V1.0.0      2019-10-18   qinchun.yang     Initial version
 *    V1.1.0      2020-07-16   yuzhe.zhang      R19-11
 *    V2.0.0      2023-02-06   jie.gu           R19-11
 */
#ifndef CRYPTO_GENERALTYPES_H
#define CRYPTO_GENERALTYPES_H

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Std_Types.h"
#include "Rte_Csm_Type.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/*
 * Enumeration of the algorithm family.
 * */
#define CRYPTO_E_QUEUE_FULL   ((Std_ReturnType)0x05u)
#define CRYPTO_E_SMALL_BUFFER ((Std_ReturnType)0x03u)
typedef enum
{
    CRYPTO_ALGOFAM_3DES = 0x01u,
    CRYPTO_ALGOFAM_AES,
    CRYPTO_ALGOFAM_BLAKE_1_256,
    CRYPTO_ALGOFAM_BLAKE_1_512,
    CRYPTO_ALGOFAM_BLAKE_2s_256,
    CRYPTO_ALGOFAM_BLAKE_2s_512,
    CRYPTO_ALGOFAM_BRAINPOOL,
    CRYPTO_ALGOFAM_CHACHA,
    CRYPTO_ALGOFAM_CUSTOM,
    CRYPTO_ALGOFAM_DH,
    CRYPTO_ALGOFAM_DRBG,
    CRYPTO_ALGOFAM_ECCANSI,
    CRYPTO_ALGOFAM_ECCNIST,
    CRYPTO_ALGOFAM_ECCSEC,
    CRYPTO_ALGOFAM_ECDH,
    CRYPTO_ALGOFAM_ECDSA,
    CRYPTO_ALGOFAM_ED25519,
    CRYPTO_ALGOFAM_EEA3,
    CRYPTO_ALGOFAM_EIA3,
    CRYPTO_ALGOFAM_FIPS186,
    CRYPTO_ALGOFAM_HKDF,
    CRYPTO_ALGOFAM_KDFX963,
    CRYPTO_ALGOFAM_NOT_SET,
    CRYPTO_ALGOFAM_PADDING_ONEWITHZEROS,
    CRYPTO_ALGOFAM_PADDING_PKCS7,
    CRYPTO_ALGOFAM_PBKDF2,
    CRYPTO_ALGOFAM_POLY1305,
    CRYPTO_ALGOFAM_RIPEMD160,
    CRYPTO_ALGOFAM_RNG,
    CRYPTO_ALGOFAM_RSA,
    CRYPTO_ALGOFAM_SHA1,
    CRYPTO_ALGOFAM_SHA2_224,
    CRYPTO_ALGOFAM_SHA2_256,
    CRYPTO_ALGOFAM_SHA2_384,
    CRYPTO_ALGOFAM_SHA2_512,
    CRYPTO_ALGOFAM_SHA2_512_224,
    CRYPTO_ALGOFAM_SHA2_512_256,
    CRYPTO_ALGOFAM_SHA3_224,
    CRYPTO_ALGOFAM_SHA3_256,
    CRYPTO_ALGOFAM_SHA3_384,
    CRYPTO_ALGOFAM_SHA3_512,
    CRYPTO_ALGOFAM_SHAKE128,
    CRYPTO_ALGOFAM_SHAKE256,
    CRYPTO_ALGOFAM_SIPHASH,
    CRYPTO_ALGOFAM_SM1,
    CRYPTO_ALGOFAM_SM2,
    CRYPTO_ALGOFAM_SM3,
    CRYPTO_ALGOFAM_SM4,
    CRYPTO_ALGOFAM_X25519
} Crypto_AlgorithmFamilyType;

/*
 * Enumeration of the algorithm mode
 * */
typedef enum
{
    CRYPTO_ALGOMODE_12ROUNDS = 0x01u,
    CRYPTO_ALGOMODE_20ROUNDS,
    CRYPTO_ALGOMODE_8ROUNDS,
    CRYPTO_ALGOMODE_CBC,
    CRYPTO_ALGOMODE_CFB,
    CRYPTO_ALGOMODE_CMAC,
    CRYPTO_ALGOMODE_CTR,
    CRYPTO_ALGOMODE_CTRDRBG,
    CRYPTO_ALGOMODE_CUSTOM,
    CRYPTO_ALGOMODE_ECB,
    CRYPTO_ALGOMODE_GCM,
    CRYPTO_ALGOMODE_GMAC,
    CRYPTO_ALGOMODE_HMAC,
    CRYPTO_ALGOMODE_NOT_SET,
    CRYPTO_ALGOMODE_OFB,
    CRYPTO_ALGOMODE_PXXXR,
    CRYPTO_ALGOMODE_RSAES_OAEP,
    CRYPTO_ALGOMODE_RSAES_PKCS1_v1_5,
    CRYPTO_ALGOMODE_RSASSA_PKCS1_v1_5,
    CRYPTO_ALGOMODE_RSASSA_PSS,
    CRYPTO_ALGOMODE_SIPHASH_2_4,
    CRYPTO_ALGOMODE_SIPHASH_4_8,
    CRYPTO_ALGOMODE_XTS
} Crypto_AlgorithmModeType;

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
/*
 * Defines which of the input/output parameters are re-directed to a key element.
 *  The values can be combined to define a bit field.
 * */
/* PRQA S 0779 ++ */ /* MISRA Rule 1.3,Rule 5.2 */
typedef enum
{
    CRYPTO_REDIRECT_CONFIG_PRIMARY_INPUT = 0x01u,
    CRYPTO_REDIRECT_CONFIG_SECONDARY_INPUT = 0x02u,
    CRYPTO_REDIRECT_CONFIG_TERTIARY_INPUT = 0x04u,
    CRYPTO_REDIRECT_CONFIG_PRIMARY_OUTPUT = 0x10u,
    CRYPTO_REDIRECT_CONFIG_SECONDARY_OUTPUT = 0x20u
} Crypto_InputOutputRedirectionConfigType;
/* PRQA S 0779 -- */ /* MISRA Rule 1.3,Rule 5.2 */
/*Enumeration of the current job state*/
typedef enum
{

    CRYPTO_JOBSTATE_IDLE = 0x00u,  /*Job is in the state "idle".*/
    CRYPTO_JOBSTATE_ACTIVE = 0x01u /*Job is in the state "active".*/
} Crypto_JobStateType;

/*Structure which contains input and output information depending on the job and
 * the crypto primitive.,ref@01009*/
typedef struct
{
    P2CONST(uint8, TYPEDEF, CSM_APPL_DATA) inputPtr;
    uint32 inputLength;
    P2CONST(uint8, TYPEDEF, CSM_APPL_DATA) secondaryInputPtr;
    uint32 secondaryInputLength;
    P2CONST(uint8, TYPEDEF, CSM_APPL_DATA) tertiaryInputPtr;
    uint32 tertiaryInputLength;
    P2VAR(uint8, TYPEDEF, CSM_APPL_DATA) outputPtr;
    P2VAR(uint32, TYPEDEF, CSM_APPL_DATA) outputLengthPtr;
    P2VAR(uint8, TYPEDEF, CSM_APPL_DATA) secondaryOutputPtr;
    P2VAR(uint32, TYPEDEF, CSM_APPL_DATA) secondaryOutputLengthPtr;
    uint64 input64;
    P2VAR(Crypto_VerifyResultType, TYPEDEF, CSM_APPL_DATA) verifyPtr;
    P2VAR(uint64, TYPEDEF, CSM_APPL_DATA) output64Ptr;
    Crypto_OperationModeType mode;
    uint32 cryIfKeyId;
    uint32 targetCryIfKeyId;
} Crypto_JobPrimitiveInputOutputType;

/*Enumeration of the kind of the service.*/
typedef enum
{
    CRYPTO_HASH = 0x00u,                  /* Hash Service */
    CRYPTO_MACGENERATE = 0x01u,           /* MacGenerate Service */
    CRYPTO_MACVERIFY = 0x02u,             /* MacVerify Service*/
    CRYPTO_ENCRYPT = 0x03u,               /* Encrypt Service */
    CRYPTO_DECRYPT = 0x04u,               /* Decrypt Service */
    CRYPTO_AEADENCRYPT = 0x05u,           /* AEADEncrypt Service */
    CRYPTO_AEADDECRYPT = 0x06u,           /* AEADDecrypt Service */
    CRYPTO_SIGNATUREGENERATE = 0x07u,     /* SignatureGenerate Service*/
    CRYPTO_SIGNATUREVERIFY = 0x08u,       /* SignatureVerify Service */
    CRYPTO_SECCOUNTERINCREMENT = 0x09u,   /* SECCOUNTERINCREMENT Service */
    CRYPTO_SECCOUNTERREAD = 0x0Au,        /* SECCOUNTERREAD Service */
    CRYPTO_RANDOMGENERATE = 0x0Bu,        /* RandomGenerate Service */
    CRYPTO_RANDOMSEED = 0x0Cu,            /* RandomSeed Service */
    CRYPTO_KEYGENERATE = 0x0Du,           /* KeyGenerate Service */
    CRYPTO_KEYDERIVE = 0x0Eu,             /* KeyDerive Service */
    CRYPTO_KEYEXCHANGECALCPUBVAL = 0x0Fu, /* KeyExchangeCalcPubVal Service */
    CRYPTO_KEYEXCHANGECALCSECRET = 0x10u, /* KeyExchangeCalcSecret Service */
    CRYPTO_KEYSETVALID = 0x13u,           /* KeySetValid Service */
    CUSTOM_SERVICE = 0x15u,               /* Custom Service */
} Crypto_ServiceInfoType;

/*Structure which determines the exact algorithm.*/
typedef struct
{
    Crypto_AlgorithmFamilyType family;
    Crypto_AlgorithmFamilyType secondaryFamily;
    uint32 keyLength;
    Crypto_AlgorithmModeType mode;
} Crypto_AlgorithmInfoType;

/*Structure which contains basic information about the crypto primitive.*/
typedef struct
{
    CONST(uint32, CSM_CONST) resultLength;
    CONST(Crypto_ServiceInfoType, CSM_CONST) service;
    CONST(Crypto_AlgorithmInfoType, CSM_CONST) algorithm;
} Crypto_PrimitiveInfoType;

/*Enumeration of the processing type.*/
typedef enum
{
    CRYPTO_PROCESSING_ASYNC = 0x00u, /* Asynchronous job processing*/
    CRYPTO_PROCESSING_SYNC = 0x01u   /* Synchronous job processing*/
} Crypto_ProcessingType;

/*Structure which contains further information, which depends on the job and
 * the crypto primitive.*/
typedef struct
{
    uint32 callbackId;
    P2CONST(Crypto_PrimitiveInfoType, TYPEDEF, CSM_CONST) primitiveInfo;
    uint32 cryIfKeyId;
    Crypto_ProcessingType processingType;
    boolean callbackUpdateNotification;
} Crypto_JobPrimitiveInfoType;

/*Structure which contains job information (job ID and job priority).*/
typedef struct
{
    CONST(uint32, CSM_CONST) jobId;
    CONST(uint32, CSM_CONST) jobPriority;
} Crypto_JobInfoType;

/*Structure which holds the identifiers of the keys and key elements which shall be used
 * as input and output for a job and a bit structure which indicates which buffers shall
 * be redirected to those key elements.*/
typedef struct
{
    uint8 redirectionConfig;
    uint32 inputKeyId;
    uint32 inputKeyElementId;
    uint32 secondaryInputKeyId;
    uint32 secondaryInputKeyElementId;
    uint32 tertiaryInputKeyId;
    uint32 tertiaryInputKeyElementId;
    uint32 outputKeyId;
    uint32 outputKeyElementId;
    uint32 secondaryOutputKeyId;
    uint32 secondaryOutputKeyElementId;
} Crypto_JobRedirectionInfoType;

/* Structure which contains further information, which depends on the job and the
 * crypto primitive.*/
typedef struct
{
    uint32 jobId;
    Crypto_JobStateType jobState;
    Crypto_JobPrimitiveInputOutputType jobPrimitiveInputOutput;
    P2CONST(Crypto_JobPrimitiveInfoType, TYPEDEF, CSM_CONST) jobPrimitiveInfo;
    P2CONST(Crypto_JobInfoType, TYPEDEF, CSM_CONST) jobInfo;
    uint32 cryptoKeyId;
    P2VAR(Crypto_JobRedirectionInfoType, TYPEDEF, CSM_APPL_DATA) jobRedirectionInfoRef;
    uint32 targetCryptoKeyId;
} Crypto_JobType;

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

#endif /* CSM_TYPES_H */
