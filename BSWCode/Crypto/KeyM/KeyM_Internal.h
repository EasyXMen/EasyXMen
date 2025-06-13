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
**  FILENAME    : KeyM_Internal.h                                             **
**                                                                            **
**  Created on  :                                                             **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION : Internal implementation for KeyM                            **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R21-11                      **
**                                                                            **
*******************************************************************************/

/******************************************************************************
**                      Revision Control History                             **
******************************************************************************/
/*  <VERSION>    <DATE>      <AUTHOR>        <REVISION LOG>
 *  V1.0.0     2022-02-15   qinchun.yang     initial version.
 */

#ifndef KEYM_INTERNAL_H
#define KEYM_INTERNAL_H
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "KeyM.h"
#include "NvM.h"
#include "Csm.h"
#include "KeyM_Cfg.h"
#include "SchM_KeyM.h"
#if (STD_ON == KEYM_DEVERROR_DETECT)
#include "Det.h"
#endif
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define KEYM_SHEKEY_INPUT_LEN     (64U)
#define KEYM_SHEKEY_SLOTID_OFFSET (15U)
#define KEYM_SHEKEY_SLOTID_MASK   (0xf0U)

#define KEYM_CONST_1              (0x1U)
#define KEYM_CONST_2              (0x2U)
#define KEYM_CONST_3              (0x3U)
#define KEYM_CONST_4              (0x4U)
#define KEYM_CONST_5              (0x5U)
#define KEYM_INVALID_U16          (0xffffU)
#define KEYM_INVALID_U32          (0xffffffffU)

#define KEYM_NVM_TIMER_STOP       (0U)
#define KEYM_NVM_TIMER_START      (1U)

#if !defined(KEYM_LOCAL)
#define KEYM_LOCAL static
#endif
/* PRQA S 3469 */ /* MISRA Dir 4.9 */
#if (STD_ON == KEYM_DEVERROR_DETECT)
static inline void KEYM_DET_REPORT(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportError(KEYM_MODULE_ID, KEYM_INSTANCE_ID, (ApiId), (ErrorId));
}
#endif            /* STD_ON == KEYM_DEVERROR_DETECT */
/* PRQA S 3469 */ /* MISRA Dir 4.9 */

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

/*******************************Key Sub Module*********************************/
/*Depends on CPU platform*/
typedef uint32 KeyM_AddrType;

typedef struct
{
    KeyM_AddrType rqstData;
    uint16 rqstLen;
    KeyM_AddrType resData;
    uint16 resLen;
    uint16 cfgKeyId;
    boolean sheKey;
} KeyM_UpdateDataType;

typedef enum
{
    KEYM_VERIFY_AEADDECRYPT,
    KEYM_VERIFY_AEADENCRYPT,
    KEYM_VERIFY_DECRYPT,
    KEYM_VERIFY_ENCRYPT,
    KEYM_VERIFY_MACGENERATE,
    KEYM_VERIFY_MACVERIFY
} KeyM_CryptoCsmVerifyJobType;

typedef enum
{
    KEYM_DERIVED_KEY,
    KEYM_STORED_KEY
} KeyM_CryptoKeyGenerationType;

typedef enum
{
    KEYM_STORAGE_IN_CSM,
    KEYM_STORAGE_IN_NVM,
    KEYM_STORAGE_IN_RAM
} KeyM_StorageType;

typedef struct
{
    uint32 keyId;
    uint32 keyMaxLen;
    P2CONST(uint8, TYPEDEF, KEYM_CONST)
    keyName;
    uint16 KeyNameLen;

#if (STD_ON == KEYM_CRYPTO_KEY_VERIFY_FUNCTION_ENABLED)
    /*Specifies what type of function for key verification operation is used.*/
    KeyM_CryptoCsmVerifyJobType verJobType;
    uint32 verifyJobRef;
#endif
    P2CONST(uint8, TYPEDEF, KEYM_CONST)
    keyProps;
    P2CONST(uint8, TYPEDEF, KEYM_CONST)
    keyGenInfo;
    uint32 keyGenInfoLen;

    KeyM_CryptoKeyGenerationType keyGenType;
    KeyM_StorageType keyStorage;
    P2CONST(uint32, TYPEDEF, KEYM_CONST)
    keySrcDervRef;
    P2CONST(uint32, TYPEDEF, KEYM_CONST)
    keyTargetRef;
    P2CONST(uint16, TYPEDEF, KEYM_CONST)
    nvmBlkRef;
} KeyM_CryptoKeyPCfgType;

typedef struct
{
    uint16 blkWriDelay;
    NvM_BlockIdType blkId;
} KeyM_NvmBlockPCfgType;

/***************************Certificate Sub Module*****************************/

typedef enum
{
    KEYM_CERT_ALGO_ECC,
    KEYM_CERT_ALGO_RSA
} KeyM_CertAlgorithmType;

typedef enum
{
    KEYM_CERT_FORMAT_CRL,
    KEYM_CERT_FORMAT_CVC,
    KEYM_CERT_FORMAT_X509
} KeyM_CertFormatType;
/* PRQA S 0779 ++ */ /* MISRA Rule 1.3,Rule 5.2 */
typedef enum
{
    CERTIFICATE_EXTENSION,
    CERTIFICATE_ISSUER_NAME,
    CERTIFICATE_ISSUER_UNIQUE_IDENTIFIER,
    CERTIFICATE_SERIAL_NUMBER,
    CERTIFICATE_SIGNATURE,
    CERTIFICATE_SIGNATURE_ALGORITHM,
    CERTIFICATE_SIGNATURE_ALGORITHM_ID,
    CERTIFICATE_SUBJECT_AUTHORIZATION,
    CERTIFICATE_SUBJECT_NAME,
    CERTIFICATE_SUBJECT_PUBLIC_KEY_INFO_PUBLIC_KEY_ALGORITHM,
    CERTIFICATE_SUBJECT_PUBLIC_KEY_INFO_SUBJECT_PUBLIC_KEY,
    CERTIFICATE_SUBJECT_UNIQUE_IDENTIFIER,
    CERTIFICATE_VALIDITY_PERIOD_NOT_AFTER,
    CERTIFICATE_VALIDITY_PERIOD_NOT_BEFORE,
    CERTIFICATE_VERSION_NUMBER,
    CERTIFICATE_REVOKED
} KeyM_CertEleStructType;
/* PRQA S 0779 -- */ /* MISRA Rule 1.3,Rule 5.2 */
typedef struct
{
    boolean certEleHasIteration;
    uint16 certEleId;
    uint16 certEleMaxLen;
    P2CONST(uint8, TYPEDEF, KEYM_CONST)
    certEleObjType;
    KeyM_CertEleStructType certEleStruct;
    uint16 certEleObjIdLen;
    P2VAR(uint8, TYPEDEF, KEYM_VAR)
    certEleObjId;
    P2VAR(KeyM_Asn1DesType, TYPEDEF, KEYM_VAR)
    certEleBuf;
} KeyM_CertEleType;

typedef struct KeyM_CertType
{
    KeyM_CertAlgorithmType certAlgoT;
    KeyM_CertFormatType certFormatT;
    uint16 certId;
    P2CONST(uint8, TYPEDEF, KEYM_CONST)
    certName;
    KeyM_StorageType certStorage;
    P2FUNC(Std_ReturnType, KEYM_APPL_CODE, certVeriCbkFunc)
    (KeyM_CertificateIdType CertId, KeyM_CertificateStatusType Result);

    P2FUNC(void, KEYM_APPL_CODE, servCertCbkFunc)
    (KeyM_CertificateIdType CertId, KeyM_ResultType Result, uint16 ResultDataLength, const uint8* ResultDataPtr);

    P2FUNC(boolean, KEYM_APPL_CODE, certRuleArbiFunc)
    (void); /*Related rule*/

    P2CONST(uint32, TYPEDEF, KEYM_CONST)
    certSigGenJobRef;
    uint32 certSigVeriJob;

    uint32 certSigVeriKey;
    P2CONST(uint32, TYPEDEF, KEYM_CONST)
    certKeyTarRef;

    P2CONST(uint16, TYPEDEF, KEYM_CONST)
    certNvmBlkRef;

    P2CONST(uint32, TYPEDEF, KEYM_CONST)
    certPriKeyRef;

    uint16 certStbMTime;

    uint16 numOfCertEle;
    P2CONST(KeyM_CertEleType, TYPEDEF, KEYM_CONST)
    certEleRef;
    /*Certificate body. The part that is To Be Signed.*/
    uint16 certMaxLen;
    P2VAR(KeyM_Asn1DesType, TYPEDEF, KEYM_VAR)
    tbsCert;
    P2CONST(struct KeyM_CertType, TYPEDEF, KEYM_CONST)
    certUpperHierRef;
    boolean selfSignedCert;
} KeyM_CertPCfgType;

typedef struct
{
    uint32 certYear;
    uint32 certMon;
    uint32 certDay;
    uint32 certHour;
    uint32 certMin;
    uint32 certSec;
} KeyM_CertTimeType;

typedef struct
{
    P2VAR(KeyM_Asn1DesType, TYPEDEF, KEYM_VAR)
    nvmCertEle;
    uint16 delayTime;
    boolean timeStart;
} KeyM_BlkWriType;
/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/
extern VAR(boolean, KEYM_VAR) KeyM_InitStatus;
extern VAR(boolean, KEYM_VAR) KeyM_SessionOpen;

extern CONST(KeyM_CryptoKeyPCfgType, KEYM_CONST) KeyM_CryptoKeyCfg[KEYM_KEY_NUM];
#if (KEYM_NVM_BLOCK_NUM > 0)
extern CONST(KeyM_NvmBlockPCfgType, KEYM_CONST) KeyM_NvmBlockPCfg[KEYM_NVM_BLOCK_NUM];
#endif
#if (KEYM_CERT_NUM > 0)
extern CONST(KeyM_CertPCfgType, KEYM_CONST) KeyM_CertPCfg[KEYM_CERT_NUM];

extern KeyM_CertTimeType KeyM_CertStartTime[KEYM_CERT_NUM];

extern KeyM_CertTimeType KeyM_CertEndTime[KEYM_CERT_NUM];

extern uint32 KeyM_CertVersion[KEYM_CERT_NUM];
#endif
/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
FUNC(void, KEYM_CODE)
KeyM_CopyData(P2VAR(void, AUTOMATIC, KEYM_APPL_DATA) dest, P2CONST(void, AUTOMATIC, KEYM_CONST) src, uint32 size);

FUNC(Std_ReturnType, KEYM_CODE)
KeyM_strcmp(P2CONST(uint8, AUTOMATIC, KEYM_CONST) str1, P2CONST(uint8, AUTOMATIC, KEYM_CONST) str2, uint16 size);

FUNC(Std_ReturnType, KEYM_CODE)
KeyM_HandleUpdate(
    P2CONST(uint8, AUTOMATIC, KEYM_APPL_DATA) ResultDataPtr,
    uint16 ResultDataLength,
    uint16 KeyIdx,
    boolean sheKey);

FUNC(Std_ReturnType, KEYM_CODE)
KeyM_GetSHEKey_M4M5(
    uint32 keyId,
    P2VAR(uint8, AUTOMATIC, AUTOMATIC) ResponseDataPtr,
    P2VAR(uint16, AUTOMATIC, AUTOMATIC) ResponseMaxDataLength);

FUNC(Std_ReturnType, KEYM_CODE)
KeyM_HandleParseCert(
    KeyM_CertificateIdType CertId,
    P2CONST(uint8, AUTOMATIC, KEYM_APPL_CONST) certDataPtr,
    uint32 certDataLength);

FUNC(void, KEYM_CODE)
KeyM_HandleCsmKeyStorage(uint32 keyId, uint16 certId, boolean keySet);

FUNC(Std_ReturnType, KEYM_CODE)
KeyM_HandleCertcVerify(
    P2CONST(KeyM_CertPCfgType, AUTOMATIC, KEYM_CONST) certCfgPtr,
    P2CONST(KeyM_CertPCfgType, TYPEDEF, KEYM_CONST) certUpperHierRef);
#endif /*KEYM_INTERNAL_H*/
