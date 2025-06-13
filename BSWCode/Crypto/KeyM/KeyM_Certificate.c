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
/* ********************************************************************************
 **                                                                            **
 **  FILENAME    : KeyM_Certificate.c                                          **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      : qinchun.yang                                                **
 **  Vendor      :                                                             **
 **  DESCRIPTION : Implementation for certificate sub module                   **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform R21-11                      **
 **                                                                            **
 *******************************************************************************/

/******************************************************************************
**                      Revision Control History                             **
******************************************************************************/
/*  <VERSION>    <DATE>      <AUTHOR>        <REVISION LOG>
 *  V1.0.0     2022-02-15   qinchun.yang     initial version.
 *  V1.0.1     2022-02-15   qinchun.yang     Modify Certificate version check.
 */

/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:KeyM<br>
  RuleSorce:puhua-rule.rcf 2.3.1

    \li PRQA S 2877 MISRA Dir 4.1 .<br>
    Reason:The exception is that the number of loops is configurable.

    \li PRQA S 3432 MISRA Rule 21.2 .<br>
    Reason:Object only used to pass value

    \li PRQA S 3432 MISRA Rule 20.7 .<br>
    Reason:Function-like macros are used to allow more efficient code

    \li PRQA S 3432 MISRA Rule 18.4 .<br>
    Reason:pointers are needed to analyze certificates

    \li PRQA S 3432 MISRA Rule 14.3 .<br>
    Reason:different configuration will leads to different result

    \li PRQA S 3440 MISRA Rule 13.3 .<br>
    Reason:pointer operation is necessary

    \li PRQA S 3440 MISRA Rule 11.8 .<br>
    Reason:pointer can be changed after get the original pointer address

    \li PRQA S 4461 MISRA Rule 10.4 .<br>
    Reason:Macros need to be used within operation

    \li PRQA S 4461 MISRA Rule 10.3 .<br>
    Reason:value judge or convert is necessary

    \li PRQA S 2996 MISRA Rule 2.2 .<br>
    Reason:parameters will be assigned value in actual use

    \li PRQA S 3206 MISRA Rule 2.7 .<br>
    Reason: parameter is defined in AUTOSAR specification

    \li PRQA S 4461 MISRA Rule 2.1 .<br>
    Reason:Code will take effect in actual project

    \li PRQA S 3673 MISRA Rule 8.13 .<br>
    Reason:pointer should be modified if condition is true

    \li PRQA S 0772,2001 MISRA Rule 15.4 15.1 .<br>
    Reason: Need goto to simplify cert element parse

 */

/* PRQA S 0488 ++ */      /* MISAR Rule-18.4 */
/* PRQA S 4603,4604 ++ */ /* MISAR Rule-21.2 */
/* PRQA S 3432  ++ */     /* MISRA Rule 20.7 */
/* PRQA S 1503 ++ */      /* MISRA Rule 2.1 */
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "KeyM_Internal.h"
#if (STD_ON == KEYM_CERTIFICATE_MANAGER_ENABLED)
#include "StbM.h"
/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
#define KEYM_LONG_FORMAT_MASK           0x80u

#define KEYM_CERT_ASN1_BOOLEAN          0x01u
#define KEYM_CERT_ASN1_INTEGER          0x02u
#define KEYM_CERT_ASN1_BIT_STRING       0x03u
#define KEYM_CERT_ASN1_OCTET_STRING     0x04u
#define KEYM_CERT_ASN1_NULL             0x05u
#define KEYM_CERT_ASN1_OID              0x06u
#define KEYM_CERT_ASN1_UINT8_STRING     0x0Cu
#define KEYM_CERT_ASN1_ASCII_STRING     0x13u
#define KEYM_CERT_ASN1_UTC_TIME         0x17u
#define KEYM_CERT_ASN1_GENERALIZED_TIME 0x18u
#define KEYM_CERT_ASN1_SEQ_CONSTRU      0x30u
#define KEYM_CERT_ASN1_SET_CONSTRU      0x31u
#define KEYM_CERT_ASN1_TAG              0xA0u

#define KEYM_CHECK_RANGE(min, max, val)         \
    do                                          \
    {                                           \
        if (((val) < (min)) || ((val) > (max))) \
        {                                       \
            return (ret);                       \
        }                                       \
    } while (0)
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CheckGeneralParam(
    VAR(KeyM_CertificateIdType, AUTOMATIC) CertId,
    VAR(KeyM_CertElementIdType, AUTOMATIC) CertElementId,
    P2CONST(uint8, AUTOMATIC, KEYM_APPL_DATA) CertElementData,
    P2CONST(uint32, AUTOMATIC, KEYM_APPL_DATA) CertElementDataLength,
    uint8 ApiId);

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_IsCertExist(
    KeyM_CertificateIdType certId,
    KeyM_CertEleStructType certStr,
    P2VAR(uint16, AUTOMATIC, KEYM_APPL_DATA) cfgId);

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_FindCertElement(
    KeyM_CertificateIdType certId,
    P2CONST(KeyM_CertEleType, AUTOMATIC, KEYM_APPL_DATA) certEle,
    P2VAR(uint16, AUTOMATIC, KEYM_APPL_DATA) eleIdx);

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertEleHandle(
    KeyM_CertificateIdType certId,
    P2CONST(KeyM_CertEleType, AUTOMATIC, KEYM_APPL_DATA) certEle,
    P2CONST(uint8, AUTOMATIC, KEYM_APPL_DATA) eleData,
    uint32 tag);

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertGetAsn1Tag(
    P2VAR(uint8*, AUTOMATIC, KEYM_APPL_DATA) cerS,
    P2CONST(uint8, TYPEDEF, KEYM_CONST) cerE,
    P2VAR(uint32, AUTOMATIC, KEYM_APPL_DATA) cerLenPtr,
    uint8 tag);

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertGetAsn1Len(
    P2VAR(uint8*, AUTOMATIC, KEYM_APPL_DATA) cerS,
    P2CONST(uint8, TYPEDEF, KEYM_CONST) cerE,
    P2VAR(uint32, AUTOMATIC, KEYM_APPL_DATA) cerLen);

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertGetAsn1TagInt(
    P2VAR(uint8*, AUTOMATIC, KEYM_APPL_DATA) cerS,
    P2CONST(uint8, TYPEDEF, KEYM_CONST) cerE,
    P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) intVal,
    P2VAR(uint32, AUTOMATIC, KEYM_APPL_DATA) cerLen);

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertGetVersion(
    KeyM_CertificateIdType certId,
    P2VAR(uint8*, AUTOMATIC, KEYM_APPL_DATA) cerS,
    P2CONST(uint8, TYPEDEF, KEYM_CONST) cerE);

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertGetSerialVersion(
    KeyM_CertificateIdType certId,
    P2VAR(uint8*, AUTOMATIC, KEYM_APPL_DATA) cerS,
    P2CONST(uint8, TYPEDEF, KEYM_CONST) cerE);

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertGetSigAlgorithmId(
    KeyM_CertificateIdType certId,
    P2VAR(uint8*, AUTOMATIC, KEYM_APPL_DATA) cerS,
    P2CONST(uint8, TYPEDEF, KEYM_CONST) cerE,
    P2VAR(KeyM_Asn1DesType, AUTOMATIC, KEYM_APPL_DATA) sigPar);

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertHandleName(
    P2VAR(uint8*, AUTOMATIC, KEYM_APPL_DATA) cerS,
    P2CONST(uint8, TYPEDEF, KEYM_CONST) cerE,
    P2VAR(KeyM_Asn1DesType, TYPEDEF, KEYM_APPL_DATA) isuName,
    P2VAR(KeyM_Asn1DesType, TYPEDEF, KEYM_APPL_DATA) isuNameOid);

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertGetName(
    KeyM_CertificateIdType certId,
    P2VAR(uint8*, AUTOMATIC, KEYM_APPL_DATA) cerS,
    P2CONST(uint8, TYPEDEF, KEYM_CONST) cerE,
    KeyM_CertEleStructType eleNameType,
    KeyM_CertEleStructType eleNameOidType);

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertParseTimeEle(
    P2VAR(uint8*, AUTOMATIC, KEYM_APPL_DATA) ele,
    uint32 len,
    P2VAR(uint32, AUTOMATIC, KEYM_APPL_DATA) time);

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE)
    KeyM_CertCheckDate(P2CONST(KeyM_CertTimeType, AUTOMATIC, KEYM_APPL_DATA) time);

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertParseTime(
    uint32 len,
    uint32 yearLen,
    P2VAR(uint8*, AUTOMATIC, KEYM_APPL_DATA) cerS,
    P2VAR(KeyM_CertTimeType, AUTOMATIC, KEYM_APPL_DATA) time);
/* PRQA S 4604 */ /* MISRA Rule 21.2 */

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertHandleGetTime(
    KeyM_CertificateIdType certId,
    P2VAR(uint8*, AUTOMATIC, KEYM_APPL_DATA) cerS,
    P2CONST(uint8, TYPEDEF, KEYM_CONST) cerE,
    P2VAR(KeyM_CertTimeType, AUTOMATIC, KEYM_APPL_DATA) time,
    KeyM_CertEleStructType eleStrut);

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertGetTime(
    KeyM_CertificateIdType certId,
    P2VAR(uint8*, AUTOMATIC, KEYM_APPL_DATA) cerS,
    P2CONST(uint8, TYPEDEF, KEYM_CONST) cerE);

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertHandleGetPubKeyAlgo(
    KeyM_CertificateIdType certId,
    P2VAR(uint8*, AUTOMATIC, KEYM_APPL_DATA) cerS,
    P2CONST(uint8, TYPEDEF, KEYM_CONST) cerE);

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertGetPubKeyInfo(
    KeyM_CertificateIdType certId,
    P2VAR(uint8*, AUTOMATIC, KEYM_APPL_DATA) cerS,
    P2CONST(uint8, TYPEDEF, KEYM_CONST) cerE);

KEYM_LOCAL FUNC(void, KEYM_CODE) KeyM_CertSkipField(
    P2VAR(uint8*, AUTOMATIC, KEYM_APPL_DATA) cerS,
    P2CONST(uint8, TYPEDEF, KEYM_CONST) cerE,
    uint8 type);

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertGetExtention(
    KeyM_CertificateIdType certId,
    P2VAR(uint8*, AUTOMATIC, KEYM_APPL_DATA) cerS,
    P2CONST(uint8, TYPEDEF, KEYM_CONST) cerE);

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertGetSignature(
    KeyM_CertificateIdType certId,
    P2VAR(uint8*, AUTOMATIC, KEYM_APPL_DATA) cerS,
    P2CONST(uint8, TYPEDEF, KEYM_CONST) cerE);

KEYM_LOCAL FUNC(KeyM_Asn1DesType*, KEYM_CODE)
    KeyM_GetCertEle(KeyM_CertificateIdType certId, KeyM_CertEleStructType certEleStru);

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CmpCertEle(
    P2VAR(KeyM_Asn1DesType, TYPEDEF, KEYM_VAR) certEleBuf,
    P2VAR(KeyM_Asn1DesType, TYPEDEF, KEYM_VAR) upCertEleBuf);

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_GlobalTimeCheck(KeyM_CertificateIdType certId);

KEYM_LOCAL FUNC(uint8, KEYM_CODE) KeyM_IsLeapYear(uint16 year);

KEYM_LOCAL FUNC(uint32, KEYM_CODE) KeyM_ConvertTime_BeijingToUnix(P2CONST(KeyM_CertTimeType, TYPEDEF, KEYM_VAR) bjTime);
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
/* PRQA S 3218 ++*/ /* MISRA Rule 8.9 */
#if (KEYM_NVM_BLOCK_NUM > 0)
KEYM_LOCAL KeyM_BlkWriType KeyM_BlkWriDelayRun[KEYM_NVM_BLOCK_NUM];
#endif
/* PRQA S 3218 --*/ /* MISRA Rule 8.9 */
#define KEYM_START_SEC_CONST_UNSPECIFIED
#include "KeyM_MemMap.h"
/* PRQA S 3218 ++*/ /* MISRA Rule 8.9 */
KEYM_LOCAL const uint8 KeyM_ComMonthDay[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

KEYM_LOCAL const uint8 KeyM_LeapMonthDay[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
/* PRQA S 3218 --*/ /* MISRA Rule 8.9 */
#define KEYM_STOP_SEC_CONST_UNSPECIFIED
#include "KeyM_MemMap.h"

#define KEYM_START_SEC_VAR_CLEARED_8
#include "KeyM_MemMap.h"
KEYM_LOCAL KeyM_CertificateStatusType KeyM_CertStatus[KEYM_CERT_NUM];
#define KEYM_STOP_SEC_VAR_CLEARED_8
#include "KeyM_MemMap.h"
/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/

#define KEYM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "KeyM_MemMap.h"
KeyM_CertTimeType KeyM_CertStartTime[KEYM_CERT_NUM]; /* PRQA S 1504 */ /* MISRA Rule 8.7 */
KeyM_CertTimeType KeyM_CertEndTime[KEYM_CERT_NUM]; /* PRQA S 1504 */   /* MISRA Rule 8.7 */

uint32 KeyM_CertVersion[KEYM_CERT_NUM]; /* PRQA S 1504 */ /* MISRA Rule 8.7 */
#define KEYM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "KeyM_MemMap.h"

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define KEYM_START_SEC_CODE
#include "KeyM_MemMap.h"
#if (KEYM_SERVICE_CERTIFICATE_FUNCTION_ENABLED == STD_ON)
/*************************************************************************/
/*
 * Brief               The key server requests an operation from the key client.
 * ServiceId           0x09
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Service: Provides the type of service the key manager has to perform.
 *                     CertNamePtr: Points to an array that defines the name of the certificate
 *                                  to be updated
 *                     CertNameLength: Specifies the number of bytes in CertNamePtr. The value 0
 *                                indicates that no CertNamePtr is provided within this function.
 *                     RequestDataPtr: Information that comes along with the request
 *                     RequestDataLength: Length of data in the RequestData array
 *                     ResponseDataLength: Max number of bytes available in ResponseData
 * Param-Name[out]     ResponseData: Data returned by the function.
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 *                     E_OK: Operation was successfully performed. Result information are available.
 *                     E_NOT_OK: Operation not accepted due to an internal error.
 *                     KEYM_E_PARAMETER_MISMATCH: Parameter do not match with expected value.
 *                     KEYM_E_KEY_CERT_SIZE_MISMATCH: Parameter size doesn't match
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
FUNC(Std_ReturnType, KEYM_CODE)
KeyM_ServiceCertificate(
    VAR(KeyM_ServiceCertificateType, AUTOMATIC) Service,
    P2CONST(uint8, AUTOMATIC, KEYM_APPL_CONST) CertNamePtr,
    VAR(uint16, AUTOMATIC) CertNameLength,
    P2CONST(uint8, AUTOMATIC, KEYM_APPL_CONST) RequestData,
    VAR(uint16, AUTOMATIC) RequestDataLength,
    P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) ResponseData,
    VAR(uint16, AUTOMATIC) ResponseDataLength)
{
    P2CONST(KeyM_CertPCfgType, AUTOMATIC, KEYM_CONST)
    certCfgPtr;
    P2CONST(KeyM_CertPCfgType, AUTOMATIC, KEYM_CONST)
    certUpperHierRef;
    uint32 jobId;
    uint16 certIdx;
    Std_ReturnType ret = E_OK;

    if (KeyM_InitStatus != TRUE)
    {
#if (KEYM_DEVERROR_DETECT == STD_ON)
        KEYM_DET_REPORT(KEYM_SERV_CERTIFICATE_ID, KEYM_E_UNINIT);
#endif
        ret = E_NOT_OK;
    }
    else if ((RequestData == NULL_PTR) || (ResponseData == NULL_PTR) || (NULL_PTR == CertNamePtr))
    {
#if (KEYM_DEVERROR_DETECT == STD_ON)
        KEYM_DET_REPORT(KEYM_SERV_CERTIFICATE_ID, KEYM_E_PARAM_POINTER);
#endif
        ret = E_NOT_OK;
    }
    else
    {
#if (KEYM_CRYPTO_KEY_START_FINALIZE_FUNCTION_ENABLED == STD_ON)
        if (KeyM_SessionOpen == FALSE)
        {
            ret = E_NOT_OK;
        }
        else
#endif
        {
#if (KEYM_CRYPTO_KEYHANDLER_SERVICE_CERTIFICATE_ENABLED == STD_ON)
            ret = KeyM_KH_ServiceCertificate(
                Service,
                CertNamePtr,
                CertNameLength,
                RequestData,
                (uint32)RequestDataLength,
                ResponseData,
                ResponseDataLength);
#else
            /*Searching for a configured certificate by name.*/
            for (certIdx = 0u; certIdx < KEYM_CERT_NUM; certIdx++)
            {
                certCfgPtr = &KeyM_CertPCfg[certIdx];
                if (E_OK == KeyM_strcmp(certCfgPtr->certName, CertNamePtr, CertNameLength))
                {
                    break;
                }
            }
            if (certIdx == KEYM_CERT_NUM)
            {
                ret = KEYM_E_KEY_CERT_SIZE_MISMATCH;
            }
            /*Service check.*/
            else if ((Service < KEYM_SERVICE_CERT_REQUEST_CSR) || (Service > KEYM_SERVICE_CERT_UPDATE_CRL))
            {
                ret = E_NOT_OK;
            }
            else
            {
                switch (Service)
                {
                case KEYM_SERVICE_CERT_REQUEST_CSR:
                    if (certCfgPtr->certSigGenJobRef == NULL_PTR)
                    {
                        ret = E_NOT_OK;
                    }
                    else
                    {
                        jobId = *(certCfgPtr->certSigGenJobRef);
                        ret = Csm_KeyGenerate(KeyM_CryptoKeyCfg[*(certCfgPtr->certPriKeyRef)].keyId);
                        if (ret == E_OK)
                        {
                            ret = Csm_SignatureGenerate(
                                jobId,
                                CRYPTO_OPERATIONMODE_SINGLECALL,
                                RequestData,
                                (uint32)RequestDataLength,
                                ResponseData,
                                (uint32*)&ResponseDataLength);
                        }
                    }
                    break;
                case KEYM_SERVICE_CERT_UPDATE_SIGNED_CSR:
                    break;

                case KEYM_SERVICE_CERT_SET_ROOT:    /*Add a new root certificate*/
                case KEYM_SERVICE_CERT_UPDATE_ROOT: /*Update an existing root certificate*/
                    if (certCfgPtr->selfSignedCert == TRUE)
                    {
                        /*Root certificate.*/
                        ret = KeyM_HandleParseCert(certCfgPtr->certId, RequestData, (uint32)RequestDataLength);
                    }
                    break;
                case KEYM_SERVICE_CERT_SET_INTERMEDIATE:    /*Add a new CA certificate*/
                case KEYM_SERVICE_CERT_UPDATE_INTERMEDIATE: /*Update an existing CA certificate*/
                    /*Pre-requisite:
                     * Root certificate shall have been stored before for a successful verification.*/
                    if (certCfgPtr->selfSignedCert != TRUE)
                    {
                        certUpperHierRef = certCfgPtr;
                        /*Verification shall be done from the top of the certificate hierarchy to the bottom.*/
                        do
                        {
                            certUpperHierRef = certUpperHierRef->certUpperHierRef;
                            if ((certUpperHierRef != NULL_PTR)
                                && (KeyM_CertStatus[certUpperHierRef->certId] != KEYM_CERTIFICATE_VALID))
                            {
                                ret = KEYM_E_CERT_INVALID_CHAIN_OF_TRUST;
                                break;
                            }
                        } while ((certUpperHierRef->selfSignedCert != TRUE) && (ret == E_OK));
                        if (E_OK == ret)
                        {
                            ret = KeyM_HandleParseCert(certCfgPtr->certId, RequestData, (uint32)RequestDataLength);
                        }
                    }
                    else
                    {
                        ret = E_NOT_OK;
                    }
                    break;

                case KEYM_SERVICE_CERT_UPDATE_CRL:
                    break;
                default:
                    /*User specific extensions not supported temporarily*/
                    ret = E_NOT_OK;
                    break;
                }
            }
#endif /*KEYM_CRYPTO_KEYHANDLER_SERVICE_CERTIFICATE_ENABLED == STD_ON*/
        }
    }

SERV_CERT_EXIT:
    return ret;
}
#endif /*KEYM_SERVICE_CERTIFICATE_FUNCTION_ENABLED == STD_ON*/
/*
 * Brief               This function provides the certificate data to the key management module to
 *                     temporarily store the certificate.
 * ServiceId           0x0a
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CertId: Holds the identifier of the certificate.
 *                     CertificateDataPtr: Pointer to a structure that provides the certificate data.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 *                     E_OK: Certificate accepted.
 *                     E_NOT_OK: Certificate could not be set.
 *                     KEYM_E_PARAMETER_MISMATCH: Parameter do not match with expected value.
 *                     KEYM_E_KEY_CERT_SIZE_MISMATCH: Parameter size doesn't match
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
FUNC(Std_ReturnType, KEYM_CODE)
KeyM_SetCertificate(
    VAR(KeyM_CertificateIdType, AUTOMATIC) CertId,
    P2CONST(KeyM_CertDataType, AUTOMATIC, KEYM_APPL_CONST) CertificateDataPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == KEYM_DEVERROR_DETECT)
    if ((boolean)FALSE == KeyM_InitStatus)
    {
        KEYM_DET_REPORT(KEYM_SET_CERTIFICATE_ID, KEYM_E_UNINIT);
    }
    else if ((CertId >= KEYM_CERT_NUM) || (CertificateDataPtr == NULL_PTR))
    {
        KEYM_DET_REPORT(KEYM_SET_CERTIFICATE_ID, KEYM_E_PARAM_POINTER);
    }
    else
#endif /*STD_ON == KEYM_DEVERROR_DETECT*/
    {
        if (CertificateDataPtr->certDataLength == 0u)
        {
            /*SWS_KeyM_00141*/
            KeyM_CertStatus[CertId] = KEYM_CERTIFICATE_NOT_AVAILABLE;
        }
        else
        {
            /*Parsing is in progress*/
            KeyM_CertStatus[CertId] = KEYM_CERTIFICATE_NOT_PARSED;

            if (KeyM_CertPCfg[CertId].certStorage != KEYM_STORAGE_IN_RAM)
            {
                /*SWS_KeyM_00166*/
#if (STD_ON == KEYM_DEVERROR_DETECT)
                KEYM_DET_REPORT(KEYM_SET_CERTIFICATE_ID, KEYM_E_CONFIG_FAILURE);
#endif /*STD_ON == KEYM_DEVERROR_DETECT*/
                ret = E_NOT_OK;
            }
            else
            {
                ret = KeyM_HandleParseCert(CertId, CertificateDataPtr->certData, CertificateDataPtr->certDataLength);
                if (ret != E_OK)
                {
                    KeyM_CertStatus[CertId] = KEYM_E_CERTIFICATE_INVALID_FORMAT;
                }
                else
                {
                    /*do nothing*/
                }
            }
        }
    }

    return ret;
}

/*************************************************************************/
/*
 * Brief               This function provides the certificate data
 * ServiceId           0x0b
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CertId: Holds the identifier of the certificate.
 * Param-Name[out]     None
 * Param-Name[in/out]  CertificateDataPtr: Pointer to a structure that provides the certificate data.
 * Return              Std_ReturnType
 *                     E_OK: Certificate accepted.
 *                     E_NOT_OK: Certificate could not be set.
 *                     KEYM_E_PARAMETER_MISMATCH: Parameter do not match with expected value.
 *                     KEYM_E_KEY_CERT_SIZE_MISMATCH: Parameter size doesn't match
 *                     KEYM_E_KEY_CERT_EMPTY: No certificate data available, the certificate slot
 *                                            is empty.
 *                     KEYM_E_KEY_CERT_READ_FAIL: Certificate cannot be provided, access denied.
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
FUNC(Std_ReturnType, KEYM_CODE)
KeyM_GetCertificate(
    VAR(KeyM_CertificateIdType, AUTOMATIC) CertId,
    P2VAR(KeyM_CertDataType, AUTOMATIC, KEYM_APPL_DATA) CertificateDataPtr)
{
    P2CONST(KeyM_CertPCfgType, AUTOMATIC, KEYM_CONST)
    certCfgPtr;
    /* PRQA S 2981 ++*/ /* MISRA Rule 2.2 */
    Std_ReturnType ret = E_NOT_OK;
    /* PRQA S 2981 --*/ /* MISRA Rule 2.2 */
#if (STD_ON == KEYM_DEVERROR_DETECT)
    if ((boolean)FALSE == KeyM_InitStatus)
    {
        KEYM_DET_REPORT(KEYM_GET_CERTIFICATE_ID, KEYM_E_UNINIT);
    }
    else if ((CertId >= KEYM_CERT_NUM) || (CertificateDataPtr == NULL_PTR))
    {
        KEYM_DET_REPORT(KEYM_GET_CERTIFICATE_ID, KEYM_E_PARAM_POINTER);
    }
    else
#endif /*STD_ON == KEYM_DEVERROR_DETECT*/
    {
        certCfgPtr = &KeyM_CertPCfg[CertId];
        if (certCfgPtr->tbsCert->len > CertificateDataPtr->certDataLength)
        {
            ret = KEYM_E_KEY_CERT_SIZE_MISMATCH;
        }
        else if (certCfgPtr->tbsCert->len == 0u)
        {
            ret = KEYM_E_KEY_CERT_EMPTY;
        }
        else
        {
            /* PRQA S 0311 ++*/ /* MISRA Rule 11.8 */
            KeyM_CopyData((void*)CertificateDataPtr->certData, certCfgPtr->tbsCert->dataEle, certCfgPtr->tbsCert->len);
            /* PRQA S 0311 --*/ /* MISRA Rule 11.8 */
            CertificateDataPtr->certDataLength = certCfgPtr->tbsCert->len;
            ret = E_OK;
        }
    }

    return ret;
}

/*************************************************************************/
/*
 * Brief               This function provides the certificate data to the key management module to
 *                     temporarily store the certificate.
 * ServiceId           0x0c
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CertId: Holds the identifier of the lower certificate in the chain
 *                     CertUpperId: Holds the identifier of the upper certificate in the chain
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 *                     E_OK: Certificate verification request accepted.
 *                     E_NOT_OK: Operation not accepted due to an internal error.
 *                     KEYM_E_BUSY:Validation cannot be performed yet. KeyM is
 *                                 currently busy with other jobs.
 *                     KEYM_E_PARAMETER_MISMATCH: Certificate ID invalid.
 *                     KEYM_E_KEY_CERT_EMPTY: One of the certificate slots are empty.
 *                     KEYM_E_CERT_INVALID_CHAIN_OF_TRUST: An upper certificate is not valid.
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
FUNC(Std_ReturnType, KEYM_CODE)
KeyM_VerifyCertificates(
    VAR(KeyM_CertificateIdType, AUTOMATIC) CertId,
    VAR(KeyM_CertificateIdType, AUTOMATIC) CertUpperId)
{
    Std_ReturnType ret;

    if (KeyM_InitStatus != TRUE)
    {
#if (KEYM_DEVERROR_DETECT == STD_ON)
        KEYM_DET_REPORT(KEYM_VERIFY_CERTIFICATES_ID, KEYM_E_UNINIT);
#endif
        ret = E_NOT_OK;
    }
    else if ((CertId >= KEYM_CERT_NUM) || (CertUpperId >= KEYM_CERT_NUM))
    {
        ret = KEYM_E_PARAMETER_MISMATCH;
    }
    /*[SWS_KeyM_00118]:The certificate referenced by CertUpperId shall have been validated before.*/
    else if (
        (KeyM_CertStatus[CertUpperId] != KEYM_CERTIFICATE_VALID)
        || (KeyM_CertPCfg[CertId].certUpperHierRef->certId != CertUpperId))
    {
        ret = KEYM_E_CERT_INVALID_CHAIN_OF_TRUST;
    }
    else if (KeyM_CertStatus[CertId] != KEYM_CERTIFICATE_PARSED_NOT_VALIDATED)
    {
        /*SWS_KeyM_00028*/
        ret = E_NOT_OK;
    }
    else
    {
        ret = E_OK;
    }

    return ret;
}

/*************************************************************************/
/*
 * Brief               This function verifies a certificate that was previously provided with
 *                     KeyM_SetCertificate() against already stored and provided certificates
 *                     stored with other certificate IDs.
 * ServiceId           0x0d
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CertId: Holds the identifier of certificate
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 *                     E_OK: Certificate verification request accepted.
 *                     E_NOT_OK: Operation not accepted due to an internal error
 *                     KEYM_E_BUSY:Validation cannot be performed yet. KeyM is
 *                                 currently busy with other jobs.
 *                     KEYM_E_PARAMETER_MISMATCH: Certificate ID invalid.
 *                     KEYM_E_KEY_CERT_EMPTY: One of the certificate slots are empty.
 *                     KEYM_E_CERT_INVALID_CHAIN_OF_TRUST: An upper certificate is not valid.
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
FUNC(Std_ReturnType, KEYM_CODE)
KeyM_VerifyCertificate(VAR(KeyM_CertificateIdType, AUTOMATIC) CertId)
{
    P2CONST(KeyM_CertPCfgType, AUTOMATIC, KEYM_CONST)
    certCfgPtr;
    P2CONST(KeyM_CertPCfgType, TYPEDEF, KEYM_CONST)
    certUpperHierRef;
    Std_ReturnType ret;

    if (KeyM_InitStatus != TRUE)
    {
#if (KEYM_DEVERROR_DETECT == STD_ON)
        KEYM_DET_REPORT(KEYM_VERIFY_CERTIFICATE_ID, KEYM_E_UNINIT);
#endif
        ret = E_NOT_OK;
    }
    else if (CertId >= KEYM_CERT_NUM)
    {
        ret = KEYM_E_PARAMETER_MISMATCH;
    }
    else
    {
        certCfgPtr = &KeyM_CertPCfg[CertId];

        /*SWS_KeyM_00028: A verification of a certificate shall only be started if the certificate is
         * in the status KEYM_CERTIFICATE_PARSED_NOT_VALIDATED*/
        if (KeyM_CertStatus[CertId] != KEYM_CERTIFICATE_PARSED_NOT_VALIDATED)
        {
            ret = E_NOT_OK;
        }
        else
        {
            certUpperHierRef = certCfgPtr;
            ret = E_OK;
            /*Verification shall be done from the top of the certificate hierarchy to the bottom.*/
            if ((certUpperHierRef->selfSignedCert == TRUE))
            {
                /*do nothing*/
            }
            else
            {
                do
                {
                    certUpperHierRef = certUpperHierRef->certUpperHierRef;
                    if ((certUpperHierRef != NULL_PTR)
                        && (KeyM_CertStatus[certUpperHierRef->certId] != KEYM_CERTIFICATE_VALID))
                    {
                        ret = KEYM_E_CERT_INVALID_CHAIN_OF_TRUST;
                        break;
                    }
                } while (certUpperHierRef->selfSignedCert != TRUE);
            }
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               This function performs a certificate verification against a list of
 *                     certificates.
 * ServiceId           0x0e
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CertId: Holds the identifier of the last certificate in the chain.
 *                     certChainData: This is a pointer to an array of certificates sorted
 *                                    according to the order in the PKI.
 *                     NumberOfCertificates: Defines the number of certificates stored in
 *                                           the CertChainData array.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 *                     E_OK: Certificate verification request accepted.
 *                     E_NOT_OK: Operation not accepted due to an internal error
 *                     KEYM_E_BUSY:Validation cannot be performed yet. KeyM is
 *                                 currently busy with other jobs.
 *                     KEYM_E_PARAMETER_MISMATCH: Certificate ID invalid.
 *                     KEYM_E_KEY_CERT_EMPTY: One of the certificate slots are empty.
 *                     KEYM_E_CERT_INVALID_CHAIN_OF_TRUST: An upper certificate is not valid.
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
FUNC(Std_ReturnType, KEYM_CODE)
KeyM_VerifyCertificateChain(
    VAR(KeyM_CertificateIdType, AUTOMATIC) CertId,
    CONST(KeyM_CertDataType, AUTOMATIC) certChainData[],
    VAR(uint8, AUTOMATIC) NumberOfCertificates)
{
    Std_ReturnType ret = E_NOT_OK;
    uint8 certCnt;

    if (KeyM_InitStatus != TRUE)
    {
#if (KEYM_DEVERROR_DETECT == STD_ON)
        KEYM_DET_REPORT(KEYM_VERIFY_CERTIFICATE_CHAIN_ID, KEYM_E_UNINIT);
#endif
        ret = E_NOT_OK;
    }
    else if (NULL_PTR == certChainData)
    {
#if (KEYM_DEVERROR_DETECT == STD_ON)
        KEYM_DET_REPORT(KEYM_VERIFY_CERTIFICATE_CHAIN_ID, KEYM_E_PARAM_POINTER);
#endif
    }
    else if (CertId >= KEYM_CERT_NUM)
    {
        ret = KEYM_E_PARAMETER_MISMATCH;
    }
    else
    {
        /*TODO*/
        for (certCnt = 0u; certCnt < NumberOfCertificates; certCnt++)
        {
            const KeyM_CertDataType* currentCertData = &(certChainData[certCnt]);
            const KeyM_CertDataType* nextCertData = &(certChainData[certCnt + 1u]);
            (void)currentCertData;
            (void)nextCertData;
        }
    }

    return ret;
}

/*************************************************************************/
/*
 * Brief               Provides the content of a specific certificate element.
 * ServiceId           0x0f
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CertId: Holds the identifier of the last certificate in the chain.
 *                     CertElementId: Specifies the ElementId where the data shall be read from.
 *                     NumberOfCertificates: Defines the number of certificates stored in
 *                                           the CertChainData array.
 * Param-Name[out]     CertElementData: Pointer to a data buffer allocated by the caller of this function.
 * Param-Name[in/out]  CertElementDataLength:
 *                       In: Pointer to a value that contains the maximum data length of the
 *                           CertElementData buffer.
 *                       Out: The data length will be overwritten with the actual length of data
 *                            placed to the buffer if the function returns E_OK. Otherwise, the
 *                            it will be overwritten with the value zero.
 * Return              Std_ReturnType
 *                     E_OK: Certificate verification request accepted.
 *                     E_NOT_OK: Operation not accepted due to an internal error
 *                     KEYM_E_BUSY:Validation cannot be performed yet. KeyM is
 *                                 currently busy with other jobs.
 *                     KEYM_E_PARAMETER_MISMATCH: Certificate ID invalid.
 *                     KEYM_E_KEY_CERT_EMPTY: One of the certificate slots are empty.
 *                     KEYM_E_CERT_INVALID_CHAIN_OF_TRUST: An upper certificate is not valid.
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
FUNC(Std_ReturnType, KEYM_CODE)
KeyM_CertElementGet(
    VAR(KeyM_CertificateIdType, AUTOMATIC) CertId,
    VAR(KeyM_CertElementIdType, AUTOMATIC) CertElementId,
    P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) CertElementData,
    P2VAR(uint32, AUTOMATIC, KEYM_APPL_DATA) CertElementDataLength)
{
    Std_ReturnType ret;

    ret =
        KeyM_CheckGeneralParam(CertId, CertElementId, CertElementData, CertElementDataLength, KEYM_CERT_ELEMENT_GET_ID);
    if (ret == (Std_ReturnType)E_OK)
    {
        KeyM_CopyData(
            CertElementData,
            KeyM_CertPCfg[CertId].certEleRef[CertElementId].certEleBuf->dataEle,
            KeyM_CertPCfg[CertId].certEleRef[CertElementId].certEleBuf->len);
        *CertElementDataLength = KeyM_CertPCfg[CertId].certEleRef[CertElementId].certEleBuf->len;
    }

    return ret;
}

/*************************************************************************/
/*
 * Brief               This function provides the element data of a certificate.
 * ServiceId           0x1b
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CertId: Identifier of the certificate.
 *                     CertElementId:Specifies the ElementId where the data shall be read from.
 *                     Index:Specifies the index to the element that shall be read (0..N).
 * Param-Name[out]     CertElementDataPtr: Pointer to a data buffer allocated by the caller of this function
 * Param-Name[in/out]  CertElementDataLengthPtr
 *                     In: Pointer to a value that contains the maximum data length of
 *                          the CertElementData buffer.
 *                     Out:Pointer to a data buffer allocated by the caller of this function
 * Return              Std_ReturnType
 *                     E_OK
 *                     E_NOT_OK
 *                     KEYM_E_PARAMETER_MISMATCH
 *                     KEYM_E_KEY_CERT_SIZE_MISMATCH
 *                     KEYM_E_KEY_CERT_EMPTY
 *                     KEYM_E_CERT_INVALID
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
FUNC(Std_ReturnType, KEYM_CODE)
KeyM_CertificateElementGetByIndex(
    VAR(KeyM_CertificateIdType, AUTOMATIC) CertId,
    VAR(KeyM_CertElementIdType, AUTOMATIC) CertElementId,
    VAR(uint32, AUTOMATIC) Index,
    P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) CertElementDataPtr,
    P2VAR(uint32, AUTOMATIC, KEYM_APPL_DATA) CertElementDataLengthPtr)
{
    P2CONST(KeyM_CertEleType, AUTOMATIC, KEYM_CONST)
    certElePCfgPtr;
    /* PRQA S 3678 ++ */ /* MISRA Rule 8.13 */
    P2VAR(KeyM_Asn1DesType, AUTOMATIC, KEYM_VAR)
    certEleBufPtr;
    /* PRQA S 3678 -- */ /* MISRA Rule 8.13 */
    uint32 idx;
    uint32 certElenum;
    Std_ReturnType ret;

    ret = KeyM_CheckGeneralParam(
        CertId,
        CertElementId,
        CertElementDataPtr,
        CertElementDataLengthPtr,
        KEYM_CERT_ELEMENT_GET_BY_INDEX);
    if (ret == (Std_ReturnType)E_OK)
    {
        /*Index from 0..N*/
        certElenum = Index + 1u;
        certElePCfgPtr = &KeyM_CertPCfg[CertId].certEleRef[CertElementId];
        for (idx = 0; idx < certElenum; idx++)
        {
            if (idx == 0u)
            {
                certEleBufPtr = certElePCfgPtr->certEleBuf;
            }
            else if (certEleBufPtr->next != NULL_PTR)
            {
                certEleBufPtr = certEleBufPtr->next;
            }
            else
            {
                ret = E_NOT_OK;
                break;
            }
        }
        if (idx == certElenum)
        {
            KeyM_CopyData(CertElementDataPtr, certEleBufPtr->dataEle, certEleBufPtr->len);
            *CertElementDataLengthPtr = certEleBufPtr->len;
        }
    }

    return ret;
}

/*************************************************************************/
/*
 * Brief               This function provides the total number of data
 *                     elements that are available for the specified certificate element.
 * ServiceId           0x1c
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CertId: Identifier of the certificate.
 *                     CertElementId:Specifies the certificate element.
 * Param-Name[out]     CountPtr: Pointer to the buffer where the number of available data elements
 *                     for this certificate element shall be copied to.
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 *                     E_OK
 *                     E_NOT_OK
 *                     KEYM_E_PARAMETER_MISMATCH
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
FUNC(Std_ReturnType, KEYM_CODE)
KeyM_CertificateElementGetCount(
    VAR(KeyM_CertificateIdType, AUTOMATIC) CertId,
    VAR(KeyM_CertElementIdType, AUTOMATIC) CertElementId,
    P2VAR(uint16, AUTOMATIC, KEYM_APPL_DATA) CountPtr)
{
    /* PRQA S 3678 ++ */ /* MISRA Rule 8.13 */
    P2VAR(KeyM_Asn1DesType, AUTOMATIC, KEYM_VAR)
    certEleBufPtr;
    /* PRQA S 3678 -- */ /* MISRA Rule 8.13 */
    uint16 cnt = 0;
    /* PRQA S 2981 ++*/ /* MISRA Rule 2.2 */
    Std_ReturnType ret = E_NOT_OK;
    /* PRQA S 2981 --*/ /* MISRA Rule 2.2 */
#if (STD_ON == KEYM_DEVERROR_DETECT)
    if ((boolean)FALSE == KeyM_InitStatus)
    {
        KEYM_DET_REPORT(KeyM_CertElementGetCount_ID, KEYM_E_UNINIT);
    }
    else if (NULL_PTR == CountPtr)
    {
        KEYM_DET_REPORT(KeyM_CertElementGetCount_ID, KEYM_E_PARAM_POINTER);
    }
    else
#endif
    {
        if ((CertId >= KEYM_CERT_NUM) || (CertElementId > KeyM_CertPCfg[CertId].numOfCertEle))
        {
            ret = KEYM_E_PARAMETER_MISMATCH;
        }
        else
        {
            certEleBufPtr = KeyM_CertPCfg[CertId].certEleRef[CertElementId].certEleBuf;

            while (certEleBufPtr != NULL_PTR)
            {
                cnt++;
                certEleBufPtr = certEleBufPtr->next;
            }

            *CountPtr = cnt;
            ret = E_OK;
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               This function is used to initialize the interative extraction of a
 *                     certificate data element.
 * ServiceId           0x10
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CertId: Holds the identifier of the last certificate in the chain.
 *                     CertElementId: Specifies the CertElementId where the data shall be read from.
 * Param-Name[out]     CertElementData: Pointer to a data buffer allocated by the caller of this function.
 * Param-Name[in/out]  CertElementIterator: Pointer to a structure that is allocated and
 *                                          maintained by the caller.
 *                     CertElementDataLength:
 *                       In: Pointer to a value that contains the maximum data length of the
 *                           CertElementData buffer.
 *                       Out: The data length will be overwritten with the actual length of data
 *                            placed to the buffer if the function returns E_OK. Otherwise, the
 *                            it will be overwritten with the value zero.
 * Return              Std_ReturnType
 *                     E_OK:  Element found and data provided in the buffer.
 *                     E_NOT_OK: Element data not found.
 *                     KEYM_E_BUSY:Validation cannot be performed yet. KeyM is
 *                                 currently busy with other jobs.
 *                     KEYM_E_PARAMETER_MISMATCH: Certificate ID invalid.
 *                     KEYM_E_KEY_CERT_SIZE_MISMATCH: Provided buffer for the certificate
 *                                                    element too small.
 *                     KEYM_E_KEY_CERT_EMPTY: No certificate data available, the certificate
 *                                            is empty
 *                     KEYM_E_CERT_INVALID: Certificate is not valid or not verified successfull
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
FUNC(Std_ReturnType, KEYM_CODE)
KeyM_CertElementGetFirst(
    VAR(KeyM_CertificateIdType, AUTOMATIC) CertId,
    VAR(KeyM_CertElementIdType, AUTOMATIC) CertElementId,
    P2VAR(KeyM_CertElementIteratorType, AUTOMATIC, KEYM_APPL_DATA) CertElementIterator,
    P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) CertElementData,
    P2VAR(uint32, AUTOMATIC, KEYM_APPL_DATA) CertElementDataLength)
{
    Std_ReturnType ret;

    ret = KeyM_CheckGeneralParam(
        CertId,
        CertElementId,
        CertElementData,
        CertElementDataLength,
        KEYM_CERT_ELEMENT_GET_FIRST_ID);
    if (ret == (Std_ReturnType)E_OK)
    {
#if (STD_ON == KEYM_DEVERROR_DETECT)
        if (NULL_PTR == CertElementIterator)
        {
            KEYM_DET_REPORT(KEYM_CERT_ELEMENT_GET_FIRST_ID, KEYM_E_PARAM_POINTER);
            ret = E_NOT_OK;
        }
        else
#endif
        {
            /*Initialize CertElementIterator*/
            CertElementIterator->certId = CertId;
            CertElementIterator->certEleId = CertElementId;
            CertElementIterator->eleItrator = KeyM_CertPCfg[CertId].certEleRef[CertElementId].certEleBuf;

            KeyM_CopyData(
                CertElementData,
                KeyM_CertPCfg[CertId].certEleRef[CertElementId].certEleBuf->dataEle,
                KeyM_CertPCfg[CertId].certEleRef[CertElementId].certEleBuf->len);
            *CertElementDataLength = KeyM_CertPCfg[CertId].certEleRef[CertElementId].certEleBuf->len;
            CertElementIterator->eleItrator = CertElementIterator->eleItrator->next;
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               This function provides further data from a certificate element.
 * ServiceId           0x11
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      No
 * Param-Name[out]     CertElementData: Pointer to a data buffer allocated by the caller of this function.
 * Param-Name[in/out]  CertElementIterator: Pointer to a structure that is allocated and
 *                                          maintained by the caller.
 *                     CertElementDataLength:
 *                       In: Pointer to a value that contains the maximum data length of the
 *                           CertElementData buffer.
 *                       Out: The data length will be overwritten with the actual length of data
 *                            placed to the buffer if the function returns E_OK. Otherwise, the
 *                            it will be overwritten with the value zero.
 * Return              Std_ReturnType
 *                     E_OK:  Element found and data provided in the buffer.
 *                     E_NOT_OK: Element data not found.
 *                     KEYM_E_PARAMETER_MISMATCH: Certificate ID invalid.
 *                     KEYM_E_KEY_CERT_SIZE_MISMATCH: Provided buffer for the certificate
 *                                                    element too small.
 *                     KEYM_E_KEY_CERT_EMPTY: No certificate data available, the certificate
 *                                            is empty
 *                     KEYM_E_CERT_INVALID: Certificate is not valid or not verified successfull
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
FUNC(Std_ReturnType, KEYM_CODE)
KeyM_CertElementGetNext(
    P2VAR(KeyM_CertElementIteratorType, AUTOMATIC, KEYM_APPL_DATA) CertElementIterator,
    P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) CertElementData,
    P2VAR(uint32, AUTOMATIC, KEYM_APPL_DATA) CertElementDataLength)
{
    P2CONST(KeyM_CertEleType, AUTOMATIC, KEYM_CONST)
    certElePCfgPtr;
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == KEYM_DEVERROR_DETECT)
    if ((boolean)FALSE == KeyM_InitStatus)
    {
        KEYM_DET_REPORT(KEYM_CERT_ELEMENT_GET_NEXT_ID, KEYM_E_UNINIT);
    }
    else if ((NULL_PTR == CertElementIterator) || (NULL_PTR == CertElementData) || (NULL_PTR == CertElementDataLength))
    {
        KEYM_DET_REPORT(KEYM_CERT_ELEMENT_GET_NEXT_ID, KEYM_E_PARAM_POINTER);
        ret = E_NOT_OK;
    }
    else
#endif
    {

        if (CertElementIterator->eleItrator != NULL_PTR)
        {
            ret = KeyM_CheckGeneralParam(
                CertElementIterator->certId,
                CertElementIterator->certEleId,
                CertElementData,
                CertElementDataLength,
                KEYM_CERT_ELEMENT_GET_NEXT_ID);
            if (ret == (Std_ReturnType)E_OK)
            {
                certElePCfgPtr = &KeyM_CertPCfg[CertElementIterator->certId].certEleRef[CertElementIterator->certEleId];
                /*SWS_KeyM_00148*/
                if (certElePCfgPtr->certEleHasIteration == (boolean)FALSE)
                {
                    ret = KEYM_E_CERT_INVALID;
                }
                else
                {
                    KeyM_CopyData(
                        CertElementData,
                        CertElementIterator->eleItrator->dataEle,
                        CertElementIterator->eleItrator->len);
                    *CertElementDataLength = CertElementIterator->eleItrator->len;

                    CertElementIterator->eleItrator = CertElementIterator->eleItrator->next;
                }
            }
        }
    }

    return ret;
}

/*************************************************************************/
/*
 * Brief               This function provides the status of a certificate.
 * ServiceId           0x12
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CertId: Holds the identifier of the last certificate in the chain.
 * Param-Name[out]     Status: Provides the status of the certificate.
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 *                     E_OK
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
FUNC(Std_ReturnType, KEYM_CODE)
KeyM_CertGetStatus(
    /* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
    VAR(KeyM_CertificateIdType, AUTOMATIC) CertId,
    P2VAR(KeyM_CertificateStatusType, AUTOMATIC, KEYM_APPL_DATA) Status)
{
    /* PRQA S 2981 ++*/ /* MISRA Rule 2.2 */
    Std_ReturnType ret = E_NOT_OK;
    /* PRQA S 2981 --*/ /* MISRA Rule 2.2 */
#if (STD_ON == KEYM_DEVERROR_DETECT)
    if ((boolean)FALSE == KeyM_InitStatus)
    {
        KEYM_DET_REPORT(KEYM_CERT_GET_STATUS_ID, KEYM_E_UNINIT);
    }
    else if (NULL_PTR == Status)
    {
        KEYM_DET_REPORT(KEYM_CERT_GET_STATUS_ID, KEYM_E_PARAM_POINTER);
    }
    else
#endif
    {
        if (CertId >= KEYM_CERT_NUM)
        {
            ret = KEYM_E_PARAMETER_MISMATCH;
        }
        else
        {
            *Status = KeyM_CertStatus[CertId];
            ret = E_OK;
        }
    }
    return ret;
}

FUNC(Std_ReturnType, KEYM_CODE)
KeyM_CertSetStatus(VAR(KeyM_CertificateIdType, AUTOMATIC) CertId, KeyM_CertificateStatusType Status)
{
    Std_ReturnType ret = E_OK;

    if (CertId >= KEYM_CERT_NUM)
    {
        ret = KEYM_E_PARAMETER_MISMATCH;
    }
    else
    {
        KeyM_CertStatus[CertId] = Status;
    }

    return ret;
}

FUNC(void, KEYM_CODE)
KeyM_CertStoreNvmHandle(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (KEYM_NVM_BLOCK_NUM > 0)
    uint16 nvmBlkIdx;
#if (KEYM_NVM_BLOCK_NUM > 1)
    for (nvmBlkIdx = 0u; nvmBlkIdx < KEYM_NVM_BLOCK_NUM; nvmBlkIdx++)
#endif
    {
        if (KeyM_BlkWriDelayRun[nvmBlkIdx].timeStart == TRUE) /* PRQA S 2880,2961 */ /* MISRA Rule 2.1,Rule 9.1 */
        {
            KeyM_BlkWriDelayRun[nvmBlkIdx].delayTime--;
            if (KeyM_BlkWriDelayRun[nvmBlkIdx].delayTime == 0u)
            {
                (void)NvM_WriteBlock(
                    KeyM_NvmBlockPCfg[nvmBlkIdx].blkId,
                    KeyM_BlkWriDelayRun[nvmBlkIdx].nvmCertEle->dataEle);
                KeyM_BlkWriDelayRun[nvmBlkIdx].timeStart = FALSE;
                KeyM_BlkWriDelayRun[nvmBlkIdx].nvmCertEle = NULL_PTR;
            }
        }
    }
#endif
}

FUNC(Std_ReturnType, KEYM_CODE)
KeyM_HandleParseCert(
    KeyM_CertificateIdType CertId,
    P2CONST(uint8, AUTOMATIC, KEYM_APPL_CONST) certDataPtr,
    uint32 certDataLength)
{
    P2CONST(KeyM_CertPCfgType, AUTOMATIC, KEYM_CONST)
    certCfgPtr;
    P2VAR(uint8, TYPEDEF, KEYM_VAR)
    certStartPtr;
    /* PRQA S 3678 ++ */ /* MISRA Rule 8.13 */
    P2VAR(uint8, TYPEDEF, KEYM_VAR)
    tbsPtr;
    P2VAR(uint8, TYPEDEF, KEYM_VAR)
    certendPtr;
    P2VAR(uint8, TYPEDEF, KEYM_VAR)
    certTotalendPtr;
    /* PRQA S 3678 -- */ /* MISRA Rule 8.13 */
    uint32 certEleLen;
    KeyM_Asn1DesType sifParam;
    Std_ReturnType ret;

    certCfgPtr = &KeyM_CertPCfg[CertId];
    certStartPtr = (uint8*)certDataPtr; /* PRQA S 0311 */ /* MISRA Rule 11.8 */
    certEleLen = certDataLength;
    certendPtr = certStartPtr + certEleLen;
    /*
     * Certificate  ::=  SEQUENCE  {
     *      tbsCertificate       TBSCertificate,
     *      signatureAlgorithm   AlgorithmIdentifier,
     *      signatureValue       BIT STRING  }
     */
    ret = KeyM_CertGetAsn1Tag(&certStartPtr, certendPtr, &certEleLen, KEYM_CERT_ASN1_SEQ_CONSTRU);
    /* PRQA S 2001 ++ */ /* MISRA Rule 15.1 */
    if (ret != E_OK)
    {
        goto PARSE_CERT_EXIT;
    }
    certendPtr = certStartPtr + certEleLen;
    certTotalendPtr = certendPtr;
    /*
     * TBSCertificate  ::=  SEQUENCE  {
     */
    tbsPtr = certStartPtr;
    ret = KeyM_CertGetAsn1Tag(&certStartPtr, certendPtr, &certEleLen, KEYM_CERT_ASN1_SEQ_CONSTRU);
    if (ret != E_OK)
    {
        goto PARSE_CERT_EXIT;
    }
    certendPtr = certStartPtr + certEleLen;
    /*Save tbs-Certificate*/
    if ((uint16)(certendPtr - tbsPtr) > certCfgPtr->certMaxLen)
    {
        ret = E_NOT_OK;
        goto PARSE_CERT_EXIT;
    }
    certCfgPtr->tbsCert->len = (uint32)(certendPtr - tbsPtr);
    /*Use tag record TBS position.*/
    certCfgPtr->tbsCert->tag = (uint32)(tbsPtr - certDataPtr);

    if (*certStartPtr == KEYM_CERT_ASN1_TAG)
    {
        /*
         * Version  ::=  INTEGER  {v1(0), v2(1), v3(2)}
         */
        ret = KeyM_CertGetVersion(CertId, &certStartPtr, certendPtr);
        if (ret != E_OK)
        {
            goto PARSE_CERT_EXIT;
        }
    }
    else if (*certStartPtr != KEYM_CERT_ASN1_INTEGER)
    {
        goto PARSE_CERT_EXIT;
    }
    else
    {
        /*do dothing*/
    }
    /*CertificateSerialNumber  ::=  INTEGER*/
    ret = KeyM_CertGetSerialVersion(CertId, &certStartPtr, certendPtr);
    if (ret != E_OK)
    {
        goto PARSE_CERT_EXIT;
    }
    /*Signature AlgorithmIdentifier*/
    ret = KeyM_CertGetSigAlgorithmId(CertId, &certStartPtr, certendPtr, &sifParam);
    if (ret != E_OK)
    {
        goto PARSE_CERT_EXIT;
    }
    /*
     * Issuer Name
     */
    ret = KeyM_CertGetAsn1Tag(&certStartPtr, certendPtr, &certEleLen, KEYM_CERT_ASN1_SEQ_CONSTRU);
    if (ret != E_OK)
    {
        goto PARSE_CERT_EXIT;
    }
    ret = KeyM_CertGetName(
        CertId,
        &certStartPtr,
        certStartPtr + certEleLen,
        CERTIFICATE_ISSUER_NAME,
        CERTIFICATE_ISSUER_UNIQUE_IDENTIFIER);
    if (ret != E_OK)
    {
        goto PARSE_CERT_EXIT;
    }
    /*
     * Validity ::= SEQUENCE {
     *      notBefore      Time,
     *      notAfter       Time }
     *
     */
    ret = KeyM_CertGetTime(CertId, &certStartPtr, certendPtr);
    if (ret != E_OK)
    {
        goto PARSE_CERT_EXIT;
    }
    /*
     * subject Name
     * */
    ret = KeyM_CertGetAsn1Tag(&certStartPtr, certendPtr, &certEleLen, KEYM_CERT_ASN1_SEQ_CONSTRU);
    if (ret != E_OK)
    {
        goto PARSE_CERT_EXIT;
    }
    ret = KeyM_CertGetName(
        CertId,
        &certStartPtr,
        certStartPtr + certEleLen,
        CERTIFICATE_SUBJECT_NAME,
        CERTIFICATE_SUBJECT_UNIQUE_IDENTIFIER);
    if (ret != E_OK)
    {
        goto PARSE_CERT_EXIT;
    }
    /*
     *  SubjectPublicKeyInfo  ::=  SEQUENCE  {
     *       algorithm            AlgorithmIdentifier,
     *       subjectPublicKey     BIT STRING }
     */
    ret = KeyM_CertGetPubKeyInfo(CertId, &certStartPtr, certendPtr);
    if (ret != E_OK)
    {
        goto PARSE_CERT_EXIT;
    }
    /*
     *  issuerUniqueID  [1]  IMPLICIT UniqueIdentifier OPTIONAL,
     *                       -- If present, version shall be v2 or v3
     *  subjectUniqueID [2]  IMPLICIT UniqueIdentifier OPTIONAL,
     *                       -- If present, version shall be v2 or v3
     */
    if ((KeyM_CertVersion[CertId] == KEYM_CONST_1) || (KeyM_CertVersion[CertId] == KEYM_CONST_2))
    {
        /*Skip issuerUniqueID & subjectUniqueID*/
        KeyM_CertSkipField(&certStartPtr, certendPtr, 0xa1);
        KeyM_CertSkipField(&certStartPtr, certendPtr, 0xa1);
    }
    /*  extensions      [3]  EXPLICIT Extensions OPTIONAL
     *                       -- If present, version shall be v3
     * */
    if (KeyM_CertVersion[CertId] == KEYM_CONST_2)
    {
        ret = KeyM_CertGetExtention(CertId, &certStartPtr, certendPtr);
        if (ret != E_OK)
        {
            goto PARSE_CERT_EXIT;
        }
    }
    /*Check if validation completes TBS field*/
    if (certStartPtr != certendPtr)
    {
        ret = KEYM_E_KEY_CERT_INVALID;
    }
    else
    {
        /*1.Skip signatureAlgorithm AlgorithmIdentifie.
         * This field MUST contain the same algorithm identifier as the signature field in the sequence
         * tbsCertificater*/
        certendPtr = certTotalendPtr;
        KeyM_CertSkipField(&certStartPtr, certendPtr, KEYM_CERT_ASN1_SEQ_CONSTRU);
        /*Store signature*/
        ret = KeyM_CertGetSignature(CertId, &certStartPtr, certendPtr);
        if ((ret != E_OK) || (certStartPtr != certendPtr))
        {
            ret = KEYM_E_KEY_CERT_INVALID;
        }
        else
        {
            /*Parsing operation has been completed without failure*/
            KeyM_CertStatus[CertId] = KEYM_CERTIFICATE_PARSED_NOT_VALIDATED;
            KeyM_CopyData(certCfgPtr->tbsCert->dataEle, certDataPtr, certDataLength);
        }
    }
PARSE_CERT_EXIT:
    /* PRQA S 2001 -- */ /* MISRA Rule 15.1 */
    if (ret != E_OK)
    {
        (void)KeyM_CertSetStatus(CertId, KEYM_E_CERTIFICATE_INVALID_FORMAT);
    }
    return ret;
}

/*keySet = TRUE:Storrage KeyElement.
 * keySet = FALSE:Get KeyElement.
 * */
FUNC(void, KEYM_CODE)
KeyM_HandleCsmKeyStorage(uint32 keyId, uint16 certId, boolean keySet)
{
    P2CONST(KeyM_CertPCfgType, AUTOMATIC, KEYM_CONST)
    certCfgPtr;
    P2CONST(KeyM_CertEleType, AUTOMATIC, KEYM_CONST)
    certEleRef;
    uint32 keyElementId;
    uint16 certEle;

    certCfgPtr = &KeyM_CertPCfg[certId];
    for (certEle = 0u; certEle < certCfgPtr->numOfCertEle; certEle++)
    {
        certEleRef = &certCfgPtr->certEleRef[certEle];
        switch (certEleRef->certEleStruct)
        {
        case CERTIFICATE_VERSION_NUMBER:
            keyElementId = CRYPTO_KE_CERTIFICATE_VERSION;
            break;
        case CERTIFICATE_SERIAL_NUMBER:
            keyElementId = CRYPTO_KE_CERTIFICATE_SERIALNUMBER;
            break;
        case CERTIFICATE_SIGNATURE_ALGORITHM:
            keyElementId = CRYPTO_KE_CERTIFICATE_SIGNATURE_ALGORITHM;
            break;
        case CERTIFICATE_ISSUER_NAME:
            keyElementId = CRYPTO_KE_CERTIFICATE_ISSUER;
            break;
        case CERTIFICATE_VALIDITY_PERIOD_NOT_BEFORE:
            keyElementId = CRYPTO_KE_CERTIFICATE_VALIDITY_NOT_BEFORE;
            break;
        case CERTIFICATE_VALIDITY_PERIOD_NOT_AFTER:
            keyElementId = CRYPTO_KE_CERTIFICATE_VALIDITY_NOT_AFTER;
            break;
        case CERTIFICATE_SUBJECT_NAME:
            keyElementId = CRYPTO_KE_CERTIFICATE_SUBJECT;
            break;
        case CERTIFICATE_SUBJECT_PUBLIC_KEY_INFO_SUBJECT_PUBLIC_KEY:
            keyElementId = CRYPTO_KE_CERTIFICATE_SUBJECT_PUBLIC_KEY;
            break;
        case CERTIFICATE_SIGNATURE:
            keyElementId = CRYPTO_KE_CERTIFICATE_SIGNATURE;
            break;
        case CERTIFICATE_EXTENSION:
            keyElementId = CRYPTO_KE_CERTIFICATE_EXTENSIONS;
            break;
        default:
            keyElementId = KEYM_INVALID_U32;
            break;
        }
        if (keyElementId != KEYM_INVALID_U32)
        {
            if (keySet == TRUE)
            {
                (void)Csm_KeyElementSet(
                    keyId,
                    keyElementId,
                    certEleRef->certEleBuf->dataEle,
                    certEleRef->certEleBuf->len);
            }
            else
            {
                (void)Csm_KeyElementGet(
                    keyId,
                    keyElementId,
                    certEleRef->certEleBuf->dataEle,
                    &certEleRef->certEleBuf->len);
            }
        }
    }
}
#define KEYM_STOP_SEC_CODE
#include "KeyM_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CheckGeneralParam(
    VAR(KeyM_CertificateIdType, AUTOMATIC) CertId,
    VAR(KeyM_CertElementIdType, AUTOMATIC) CertElementId,
    P2CONST(uint8, AUTOMATIC, KEYM_APPL_DATA) CertElementData,
    P2CONST(uint32, AUTOMATIC, KEYM_APPL_DATA) CertElementDataLength,
    uint8 ApiId) /* PRQA S 3206 */ /* MISRA Rule 2.7 */
{
    Std_ReturnType ret;

    if ((boolean)FALSE == KeyM_InitStatus)
    {
#if (STD_ON == KEYM_DEVERROR_DETECT)
        KEYM_DET_REPORT(ApiId, KEYM_E_UNINIT);
#endif /*STD_ON == KEYM_DEVERROR_DETECT*/
        ret = E_NOT_OK;
    }
    else if ((CertElementData == NULL_PTR) || (CertElementDataLength == NULL_PTR))
    {
#if (STD_ON == KEYM_DEVERROR_DETECT)
        KEYM_DET_REPORT(ApiId, KEYM_E_PARAM_POINTER);
#endif /*STD_ON == KEYM_DEVERROR_DETECT*/
        ret = E_NOT_OK;
    }
    else
    {
        (void)ApiId;
        if ((CertId >= KEYM_CERT_NUM) || (CertElementId > KeyM_CertPCfg[CertId].numOfCertEle))
        {
            ret = KEYM_E_PARAMETER_MISMATCH;
        }
        else if (*CertElementDataLength < KeyM_CertPCfg[CertId].certEleRef[CertElementId].certEleMaxLen)
        {
            ret = KEYM_E_KEY_CERT_SIZE_MISMATCH;
        }
        else if (KeyM_CertStatus[CertId] != KEYM_CERTIFICATE_VALID)
        {
            ret = KEYM_E_KEY_CERT_INVALID;
        }
        else if (KeyM_CertPCfg[CertId].certEleRef[CertElementId].certEleBuf->len == 0U)
        {
            ret = KEYM_E_KEY_CERT_EMPTY;
        }
        else
        {
            ret = E_OK;
        }
    }
    return ret;
}

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_IsCertExist(
    KeyM_CertificateIdType certId,
    KeyM_CertEleStructType certStr,
    P2VAR(uint16, AUTOMATIC, KEYM_APPL_DATA) cfgId)
{
    P2CONST(KeyM_CertEleType, AUTOMATIC, KEYM_CONST)
    certEleCfgPtr;
    uint16 idx;
    Std_ReturnType ret = E_NOT_OK;

    for (idx = 0; idx < KeyM_CertPCfg[certId].numOfCertEle; idx++)
    {
        certEleCfgPtr = &KeyM_CertPCfg[certId].certEleRef[idx];
        if (certEleCfgPtr->certEleStruct == certStr)
        {
            ret = E_OK;
            if (cfgId != NULL_PTR)
            {
                *cfgId = idx;
            }
            break;
        }
    }
    return ret;
}

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_FindCertElement(
    KeyM_CertificateIdType certId,
    P2CONST(KeyM_CertEleType, AUTOMATIC, KEYM_APPL_DATA) certEle,
    P2VAR(uint16, AUTOMATIC, KEYM_APPL_DATA) eleIdx)
{
    P2CONST(KeyM_CertEleType, AUTOMATIC, KEYM_CONST)
    certEleCfgPtr;
    uint16 idx;
    uint16 len;
    Std_ReturnType ret = E_NOT_OK;
    boolean oidCheck = FALSE;

    for (idx = 0; idx < KeyM_CertPCfg[certId].numOfCertEle; idx++)
    {
        certEleCfgPtr = &KeyM_CertPCfg[certId].certEleRef[idx];
        if ((certEleCfgPtr->certEleMaxLen < certEle->certEleMaxLen)
            || (certEleCfgPtr->certEleStruct != certEle->certEleStruct))
        {
            continue;
        }
        else
        {
            /*Check KeyMCertificateElementObjectType*/
            if ((certEleCfgPtr->certEleObjType != NULL_PTR) && (certEle->certEleObjType != NULL_PTR))
            {
                if (((*certEleCfgPtr->certEleObjType) & (*certEle->certEleObjType)) == 0u)
                {
                    continue;
                }
            }
            /*Check KeyMCertificateElementObjectId*/
            if ((certEleCfgPtr->certEleObjId != NULL_PTR) && (certEleCfgPtr->certEleObjIdLen != 0u)
                && (certEle->certEleObjId != NULL_PTR))
            {
                for (len = 0u; len < certEleCfgPtr->certEleObjIdLen; len++)
                {
                    if (certEleCfgPtr->certEleObjId[len] != certEle->certEleObjId[len])
                    {
                        break;
                    }
                }
                if (len == certEleCfgPtr->certEleObjIdLen)
                {
                    oidCheck = TRUE;
                }
            }
            else
            {
                /*No need to check*/
                oidCheck = TRUE;
            }

            if (oidCheck == TRUE)
            {
                *eleIdx = idx;
                ret = E_OK;
                break;
            }
        }
    }
    return ret;
}

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertEleHandle(
    KeyM_CertificateIdType certId,
    P2CONST(KeyM_CertEleType, AUTOMATIC, KEYM_APPL_DATA) certEle,
    P2CONST(uint8, AUTOMATIC, KEYM_APPL_DATA) eleData,
    uint32 tag)
{
    uint16 eleCfgIndex;
    uint16 eleIdx;
    Std_ReturnType ret;

    ret = KeyM_FindCertElement(certId, certEle, &eleCfgIndex);
    if (ret == (Std_ReturnType)E_OK)
    {
        for (eleIdx = 0u; eleIdx < certEle->certEleMaxLen; eleIdx++)
        {
            KeyM_CertPCfg[certId].certEleRef[eleCfgIndex].certEleBuf->dataEle[eleIdx] = eleData[eleIdx];
        }
        KeyM_CertPCfg[certId].certEleRef[eleCfgIndex].certEleBuf->tag = tag;
        KeyM_CertPCfg[certId].certEleRef[eleCfgIndex].certEleBuf->len = certEle->certEleMaxLen;
    }
    return ret;
}

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertGetAsn1Tag(
    P2VAR(uint8*, AUTOMATIC, KEYM_APPL_DATA) cerS,
    P2CONST(uint8, TYPEDEF, KEYM_CONST) cerE,
    P2VAR(uint32, AUTOMATIC, KEYM_APPL_DATA) cerLenPtr,
    uint8 tag)
{
    Std_ReturnType ret;

    if (((cerE - *cerS) < KEYM_CONST_1) || (**cerS != tag)) /* PRQA S 1822 */ /* MISRA Rule 10.4 */
    {
        ret = E_NOT_OK;
    }
    else
    {
        (*cerS)++;
        ret = KeyM_CertGetAsn1Len(cerS, cerE, cerLenPtr);
    }

    return ret;
}

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertGetAsn1Len(
    P2VAR(uint8*, AUTOMATIC, KEYM_APPL_DATA) cerS,
    P2CONST(uint8, TYPEDEF, KEYM_CONST) cerE,
    P2VAR(uint32, AUTOMATIC, KEYM_APPL_DATA) cerLen)
{
    Std_ReturnType ret = E_OK;

    if ((**cerS & KEYM_LONG_FORMAT_MASK) == 0u)
    {
        *cerLen = *(*cerS)++; /* PRQA S 3440,3387 */ /* MISRA Rule 13.3 */
    }
    else /*length > 127*/
    {
        switch (**cerS & (KEYM_LONG_FORMAT_MASK - KEYM_CONST_1))
        {
        case KEYM_CONST_1:
            if ((cerE - *cerS) < KEYM_CONST_2) /* PRQA S 1822 */ /* MISRA Rule 10.4 */
            {
                ret = E_NOT_OK;
            }
            else
            {
                *cerLen = (*cerS)[1];
                (*cerS) += 2;
            }
            break;
        case KEYM_CONST_2:
            if ((cerE - *cerS) < KEYM_CONST_3) /* PRQA S 1822 */ /* MISRA Rule 10.4 */
            {
                ret = E_NOT_OK;
            }
            else
            {
                *cerLen = ((uint32)(*cerS)[1u] << 8u) | (*cerS)[2u];
                (*cerS) += 3u;
            }
            break;
        case KEYM_CONST_3:
            if ((cerE - *cerS) < KEYM_CONST_4) /* PRQA S 1822 */ /* MISRA Rule 10.4 */
            {
                ret = E_NOT_OK;
            }
            else
            {
                *cerLen = ((uint32)(*cerS)[1u] << 16u) | ((uint32)(*cerS)[2u] << 8u) | (*cerS)[3u];
                (*cerS) += 4u;
            }
            break;
        case KEYM_CONST_4:
            if ((cerE - *cerS) < KEYM_CONST_5) /* PRQA S 1822 */ /* MISRA Rule 10.4 */
            {
                ret = E_NOT_OK;
            }
            else
            {
                *cerLen = ((uint32)(*cerS)[1u] << 24u) | ((uint32)(*cerS)[2u] << 16u) | ((uint32)(*cerS)[3u] << 8u)
                          | (*cerS)[4u];
                (*cerS) += 5u;
            }
            break;
        default:
            ret = E_NOT_OK;
            break;
        }
    }
    if (*cerLen > (uint32)(cerE - *cerS))
    {
        ret = E_NOT_OK;
    }
    return ret;
}

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertGetAsn1TagInt(
    P2VAR(uint8*, AUTOMATIC, KEYM_APPL_DATA) cerS,
    P2CONST(uint8, TYPEDEF, KEYM_CONST) cerE,
    P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) intVal,
    P2VAR(uint32, AUTOMATIC, KEYM_APPL_DATA) cerLen)
{
    uint32 len;
    Std_ReturnType ret;

    *intVal = 0u;
    ret = KeyM_CertGetAsn1Tag(cerS, cerE, &len, KEYM_CERT_ASN1_INTEGER);
    if ((ret == (Std_ReturnType)E_OK) && (len > 0u) && (len <= 4u) && ((**cerS & KEYM_LONG_FORMAT_MASK) == 0u))
    {
        *cerLen = len;
        /*Skip leading zeros.*/
        while ((len > 0u) && (**cerS == 0u))
        {
            ++(*cerS);
            --len;
        }
        while (len-- > 0u) /* PRQA S 3440 */ /* MISRA Rule 13.3 */
        {
            *intVal = **cerS;
            (*cerS)++;
        }
    }
    else
    {
        ret = E_NOT_OK;
    }
    return ret;
}

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertGetVersion(
    KeyM_CertificateIdType certId,
    P2VAR(uint8*, AUTOMATIC, KEYM_APPL_DATA) cerS,
    P2CONST(uint8, TYPEDEF, KEYM_CONST) cerE)
{
    uint32 cerLen;
    uint16 eleIdx;
    Std_ReturnType ret;
    uint8 ver;
    const uint8* temp_EndAddr = cerE;

    ret = KeyM_CertGetAsn1Tag(cerS, temp_EndAddr, &cerLen, KEYM_CERT_ASN1_TAG);
    if (ret == (Std_ReturnType)E_OK)
    {
        temp_EndAddr = *cerS + cerLen;
        ret = KeyM_CertGetAsn1TagInt(cerS, temp_EndAddr, &ver, &cerLen);
        if ((ret != E_OK) || (*cerS != temp_EndAddr))
        {
            ret = E_NOT_OK;
        }
        else
        {
            ret = KeyM_IsCertExist(certId, CERTIFICATE_VERSION_NUMBER, &eleIdx);
            if (E_OK == ret)
            {
                KeyM_CertVersion[certId] = ver;
                KeyM_CertPCfg[certId].certEleRef[eleIdx].certEleBuf->dataEle[0] = ver;
            }
        }
    }
    return ret;
}

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertGetSerialVersion(
    KeyM_CertificateIdType certId,
    P2VAR(uint8*, AUTOMATIC, KEYM_APPL_DATA) cerS,
    P2CONST(uint8, TYPEDEF, KEYM_CONST) cerE)
{
    uint32 tag;
    uint32 cerLen;
    KeyM_CertEleType certEle;
    uint8 objType = KEYM_CERT_ASN1_INTEGER;
    Std_ReturnType ret;

    /* PRQA S 1822  ++ */ /* MISRA Rule 10.4 */
    if (((cerE - *cerS) < KEYM_CONST_1) || (**cerS != KEYM_CERT_ASN1_INTEGER))
    /* PRQA S 1822  -- */ /* MISRA Rule 10.4 */
    {
        ret = E_NOT_OK;
    }
    else
    {
        tag = *(*cerS)++; /* PRQA S 3440,3387 */ /* MISRA Rule 13.3 */
        ret = KeyM_CertGetAsn1Len(cerS, cerE, &cerLen);
        if (ret == (Std_ReturnType)E_OK)
        {
            certEle.certEleMaxLen = (uint16)cerLen;
            certEle.certEleObjType = &objType;
            certEle.certEleStruct = CERTIFICATE_SERIAL_NUMBER;
            ret = KeyM_CertEleHandle(certId, &certEle, *cerS, tag);
            if (ret == (Std_ReturnType)E_OK)
            {
                *cerS += cerLen;
            }
        }
    }
    return ret;
}

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertGetSigAlgorithmId(
    KeyM_CertificateIdType certId,
    P2VAR(uint8*, AUTOMATIC, KEYM_APPL_DATA) cerS,
    P2CONST(uint8, TYPEDEF, KEYM_CONST) cerE,
    P2VAR(KeyM_Asn1DesType, AUTOMATIC, KEYM_APPL_DATA) sigPar)
{
    uint32 tag;
    uint32 cerLen;
    KeyM_CertEleType certEle;
    uint8 objType = KEYM_CERT_ASN1_OID;
    Std_ReturnType ret;
    const uint8* temp_EndAddr = cerE;

    ret = KeyM_CertGetAsn1Tag(cerS, temp_EndAddr, &cerLen, KEYM_CERT_ASN1_SEQ_CONSTRU);
    if ((ret == (Std_ReturnType)E_OK)
        || ((temp_EndAddr - *cerS) < KEYM_CONST_1)) /* PRQA S 1822 */ /* MISRA Rule 10.4 */
    {
        tag = **cerS;
        temp_EndAddr = *cerS + cerLen;
        ret = KeyM_CertGetAsn1Tag(cerS, temp_EndAddr, &cerLen, KEYM_CERT_ASN1_OID);
        if (ret == E_OK)
        {
            certEle.certEleMaxLen = (uint16)cerLen;
            certEle.certEleObjType = &objType;
            certEle.certEleStruct = CERTIFICATE_SIGNATURE_ALGORITHM_ID;
            ret = KeyM_CertEleHandle(certId, &certEle, *cerS, tag);
            if (ret == E_OK)
            {
                *cerS += cerLen;
                if (*cerS != temp_EndAddr)
                {
                    sigPar->tag = **cerS;
                    (*cerS)++;
                    ret = KeyM_CertGetAsn1Len(cerS, temp_EndAddr, &sigPar->len);
                    if (ret != E_OK)
                    {
                        sigPar->dataEle = *cerS;
                        *cerS += sigPar->len;
                        if (*cerS != temp_EndAddr)
                        {
                            ret = E_NOT_OK;
                        }
                    }
                }
            }
        }
    }
    return ret;
}

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertHandleName(
    P2VAR(uint8*, AUTOMATIC, KEYM_APPL_DATA) cerS,
    P2CONST(uint8, TYPEDEF, KEYM_CONST) cerE,
    P2VAR(KeyM_Asn1DesType, TYPEDEF, KEYM_APPL_DATA) isuName,
    P2VAR(KeyM_Asn1DesType, TYPEDEF, KEYM_APPL_DATA) isuNameOid)
{
    uint32 len;
    Std_ReturnType ret;
    const uint8* temp_EndAddr = cerE;

    ret = KeyM_CertGetAsn1Tag(cerS, temp_EndAddr, &len, KEYM_CERT_ASN1_SEQ_CONSTRU);
    if (ret == E_OK)
    {

        temp_EndAddr = *cerS + len;
        if ((temp_EndAddr - *cerS) < KEYM_CONST_1) /* PRQA S 1822 */ /* MISRA Rule 10.4 */
        {
            ret = E_NOT_OK;
        }
        else
        {
            isuNameOid->tag = **cerS;
            ret = KeyM_CertGetAsn1Tag(cerS, temp_EndAddr, &isuNameOid->len, KEYM_CERT_ASN1_OID);
            if (ret == E_OK)
            {
                KeyM_CopyData(isuNameOid->dataEle, *cerS, isuNameOid->len);
                *cerS += isuNameOid->len;

                if (((temp_EndAddr - *cerS) < KEYM_CONST_1) /* PRQA S 1822 */ /* MISRA Rule 10.4 */
                    || ((**cerS != KEYM_CERT_ASN1_BIT_STRING) && (**cerS != KEYM_CERT_ASN1_OCTET_STRING)
                        && (**cerS != KEYM_CERT_ASN1_UINT8_STRING) && (**cerS != KEYM_CERT_ASN1_ASCII_STRING)))
                {
                    ret = E_NOT_OK;
                }
                else
                {
                    isuName->tag = **cerS;
                    (*cerS)++;
                    ret = KeyM_CertGetAsn1Len(cerS, temp_EndAddr, &isuName->len);
                    if (ret == E_OK)
                    {
                        KeyM_CopyData(isuName->dataEle, *cerS, isuName->len);
                        *cerS += isuName->len;
                        if (*cerS != temp_EndAddr)
                        {
                            ret = E_NOT_OK;
                        }
                    }
                }
            }
        }
    }
    return ret;
}

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertGetName(
    KeyM_CertificateIdType certId,
    P2VAR(uint8*, AUTOMATIC, KEYM_APPL_DATA) cerS,
    P2CONST(uint8, TYPEDEF, KEYM_CONST) cerE,
    KeyM_CertEleStructType eleNameType,
    KeyM_CertEleStructType eleNameOidType)
{
    P2CONST(uint8, TYPEDEF, KEYM_CONST)
    cerEndSet;
    uint16 nameCfgId;
    P2VAR(KeyM_Asn1DesType, TYPEDEF, KEYM_VAR)
    nameEleBuf;
    uint16 nameOidCfgId;
    P2VAR(KeyM_Asn1DesType, TYPEDEF, KEYM_VAR)
    nameOidEleBuf;
    uint32 setLen;
    Std_ReturnType ret;
    /* PRQA S 2001 ++ */ /* MISRA Rule 15.1 */
    ret = KeyM_IsCertExist(certId, eleNameType, &nameCfgId);
    if (ret != E_OK)
    {
        goto ISSUER_NAME;
    }
    nameEleBuf = KeyM_CertPCfg[certId].certEleRef[nameCfgId].certEleBuf;

    ret = KeyM_IsCertExist(certId, eleNameOidType, &nameOidCfgId);
    if (ret != E_OK)
    {
        goto ISSUER_NAME;
    }
    nameOidEleBuf = KeyM_CertPCfg[certId].certEleRef[nameOidCfgId].certEleBuf;
    /* PRQA S 0772 ++ */ /* MISRA Rule 15.4 */
    for (;;)
    {
        /*Traverse all SET.*/
        ret = KeyM_CertGetAsn1Tag(cerS, cerE, &setLen, KEYM_CERT_ASN1_SET_CONSTRU);
        if (ret != E_OK)
        {
            goto ISSUER_NAME;
        }
        cerEndSet = *cerS + setLen;
        for (;;)
        /* PRQA S 0772 -- */ /* MISRA Rule 15.4 */
        {
            if ((nameEleBuf == NULL_PTR) || (nameOidEleBuf == NULL_PTR))
            {
                ret = E_NOT_OK;
                goto ISSUER_NAME;
            }
            /*Handle a SET instance.*/
            ret = KeyM_CertHandleName(cerS, cerEndSet, nameEleBuf, nameOidEleBuf);
            if (ret != E_OK)
            {
                goto ISSUER_NAME;
            }
            if (*cerS == cerEndSet)
            {
                break;
            }
        }
        if (*cerS == cerE)
        {
            break;
        }
        nameEleBuf = nameEleBuf->next;
        nameOidEleBuf = nameOidEleBuf->next;
    }

ISSUER_NAME:
    /* PRQA S 2001 -- */ /* MISRA Rule 15.1 */
    return ret;
}

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertParseTimeEle(
    P2VAR(uint8*, AUTOMATIC, KEYM_APPL_DATA) ele,
    uint32 len,
    P2VAR(uint32, AUTOMATIC, KEYM_APPL_DATA) time)
{
    uint32 eleLen = len;
    Std_ReturnType ret = E_OK;

    *time = 0;
    for (; eleLen > 0u; --eleLen)
    {
        /*'0' <--> 0x30*/
        /*'9' <--> 0x39*/
        if ((**ele < 0x30u) || (**ele > 0x39u))
        {
            ret = KEYM_E_KEY_CERT_INVALID;
            break;
        }
        else
        {
            /*ASCII convert*/
            *time *= 10u;
            *time += *(*ele)++; /* PRQA S 3440,3387 */ /* MISRA Rule 13.3 */
            *time -= 0x30u;
        }
    }
    return ret;
}

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE)
    KeyM_CertCheckDate(P2CONST(KeyM_CertTimeType, AUTOMATIC, KEYM_APPL_DATA) time)
{
    uint32 mon;
    Std_ReturnType ret = E_NOT_OK;
    /* PRQA S 2996 ++ */ /* MISRA Rule 2.2 */
    KEYM_CHECK_RANGE(0u, 9999u, time->certYear);
    KEYM_CHECK_RANGE(0u, 23u, time->certHour);
    KEYM_CHECK_RANGE(0u, 59u, time->certMin);
    KEYM_CHECK_RANGE(0u, 59u, time->certSec);
    /* PRQA S 2996 -- */ /* MISRA Rule 2.2 */
    /*Calculate how many days in this month*/
    switch (time->certMon)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        mon = 31u;
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        mon = 30u;
        break;
    case 2:
        if ((((time->certYear % 4u) == 0u) && ((time->certYear % 100u) != 0u)) || ((time->certYear % 400u) == 0u))
        {
            mon = 29u;
        }
        else
        {
            mon = 28u;
        }
        break;
    default:
        mon = 0u;
        break;
    }
    KEYM_CHECK_RANGE(0u, mon, time->certMon); /* PRQA S 2996 */ /* MISRA Rule 2.2 */
    return E_OK;
}

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertParseTime(
    uint32 len,
    uint32 yearLen,
    P2VAR(uint8*, AUTOMATIC, KEYM_APPL_DATA) cerS,
    P2VAR(KeyM_CertTimeType, AUTOMATIC, KEYM_APPL_DATA) time)
{
    Std_ReturnType ret = E_NOT_OK;
    uint32 timebitnum = len;
    /*Minimum length is 10 or 12 depending on yearLen*/
    if (timebitnum >= (yearLen + 8u))
    {
        timebitnum -= yearLen + 8u;
        /*Parse year, month, day, hour, minute*/
        ret = KeyM_CertParseTimeEle(cerS, yearLen, &time->certYear);
        if (ret == E_OK)
        {
            if (KEYM_CONST_2 == yearLen)
            {
                if (time->certYear < 50u)
                {
                    time->certYear += 100u;
                }
                time->certYear += 1900u;
            }
            ret = KeyM_CertParseTimeEle(cerS, 2u, &time->certMon);
            if (ret == E_OK)
            {
                ret = KeyM_CertParseTimeEle(cerS, 2u, &time->certDay);
                if (ret == E_OK)
                {
                    ret = KeyM_CertParseTimeEle(cerS, 2u, &time->certHour);
                    if (ret == E_OK)
                    {
                        ret = KeyM_CertParseTimeEle(cerS, 2u, &time->certMin);
                        if (ret == E_OK)
                        {

                            /*Parse seconds if present*/
                            if (timebitnum >= 2u)
                            {
                                ret = KeyM_CertParseTimeEle(cerS, 2u, &time->certSec);
                                if (ret == E_OK)
                                {
                                    timebitnum -= 2u;
                                }
                            }
                            else
                            {
                                ret = KEYM_E_KEY_CERT_INVALID;
                            }
                            /*Parse trailing 'Z' if present
                             * Zulu Time
                             * */
                            if (ret == E_OK)
                            {
                                if ((KEYM_CONST_1 == timebitnum)
                                    && ('Z' == **cerS)) /* PRQA S 1881 */ /* MISRA Rule 10.4 */
                                {
                                    (*cerS)++;
                                    timebitnum--;
                                }
                                if (timebitnum != 0u)
                                {
                                    ret = E_NOT_OK;
                                }
                                else
                                {
                                    /*Finnal:Check if date is valid*/
                                    ret = KeyM_CertCheckDate(time);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return ret;
}

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertHandleGetTime(
    KeyM_CertificateIdType certId,
    P2VAR(uint8*, AUTOMATIC, KEYM_APPL_DATA) cerS,
    P2CONST(uint8, TYPEDEF, KEYM_CONST) cerE,
    P2VAR(KeyM_CertTimeType, AUTOMATIC, KEYM_APPL_DATA) time,
    KeyM_CertEleStructType eleStrut)
{
    uint32 yearLen;
    uint32 len;
    KeyM_CertEleType certEle;
    Std_ReturnType ret = E_NOT_OK;
    uint8 timeTag;

    if ((cerE - *cerS) >= KEYM_CONST_1) /* PRQA S 1822 */ /* MISRA Rule 10.4 */
    {
        timeTag = **cerS;
        if (timeTag == KEYM_CERT_ASN1_UTC_TIME)
        {
            yearLen = 2u;
            ret = E_OK;
        }
        else if (timeTag == KEYM_CERT_ASN1_GENERALIZED_TIME)
        {
            yearLen = 4u;
            ret = E_OK;
        }
        else
        {
            ret = KEYM_E_KEY_CERT_INVALID;
        }
        if (ret == E_OK)
        {
            (*cerS)++;
            ret = KeyM_CertGetAsn1Len(cerS, cerE, &len);
            if (ret == E_OK)
            {
                certEle.certEleMaxLen = (uint16)len;
                certEle.certEleObjType = &timeTag;
                certEle.certEleStruct = eleStrut;
                certEle.certEleObjId = NULL_PTR;
                ret = KeyM_CertEleHandle(certId, &certEle, *cerS, timeTag);
                if (ret == E_OK)
                {
                    ret = KeyM_CertParseTime(len, yearLen, cerS, time);
                }
            }
        }
    }
    return ret;
}

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertGetTime(
    KeyM_CertificateIdType certId,
    P2VAR(uint8*, AUTOMATIC, KEYM_APPL_DATA) cerS,
    P2CONST(uint8, TYPEDEF, KEYM_CONST) cerE)
{
    uint32 len;
    Std_ReturnType ret;
    const uint8* temp_EndAddr = cerE;

    ret = KeyM_CertGetAsn1Tag(cerS, temp_EndAddr, &len, KEYM_CERT_ASN1_SEQ_CONSTRU);
    if (ret == E_OK)
    {
        temp_EndAddr = *cerS + len;
        /*Validity Not Before Time*/
        ret = KeyM_CertHandleGetTime(
            certId,
            cerS,
            temp_EndAddr,
            &KeyM_CertStartTime[certId],
            CERTIFICATE_VALIDITY_PERIOD_NOT_BEFORE);
        if (ret == E_OK)
        {
            /*Validity Not After Time*/
            ret = KeyM_CertHandleGetTime(
                certId,
                cerS,
                temp_EndAddr,
                &KeyM_CertEndTime[certId],
                CERTIFICATE_VALIDITY_PERIOD_NOT_AFTER);
            if (ret == E_OK)
            {
                if (*cerS != temp_EndAddr)
                {
                    ret = E_NOT_OK;
                }
            }
        }
    }
    return ret;
}

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertHandleGetPubKeyAlgo(
    KeyM_CertificateIdType certId,
    P2VAR(uint8*, AUTOMATIC, KEYM_APPL_DATA) cerS,
    P2CONST(uint8, TYPEDEF, KEYM_CONST) cerE)
{
    P2CONST(uint8, TYPEDEF, KEYM_CONST)
    cerEndPtr;
    P2VAR(KeyM_Asn1DesType, TYPEDEF, KEYM_VAR)
    pubAlgoPtr;
    uint32 len;
    uint16 pubAlgoCfgIdx;
    Std_ReturnType ret;

    ret = KeyM_IsCertExist(certId, CERTIFICATE_SUBJECT_PUBLIC_KEY_INFO_PUBLIC_KEY_ALGORITHM, &pubAlgoCfgIdx);
    if (ret == E_OK)
    {
        pubAlgoPtr = KeyM_CertPCfg[certId].certEleRef[pubAlgoCfgIdx].certEleBuf;
        ret = KeyM_CertGetAsn1Tag(cerS, cerE, &len, KEYM_CERT_ASN1_SEQ_CONSTRU);
        if (ret == E_OK)
        {
            cerEndPtr = *cerS + len;

            pubAlgoPtr->tag = **cerS;
            ret = KeyM_CertGetAsn1Tag(cerS, cerEndPtr, &pubAlgoPtr->len, KEYM_CERT_ASN1_OID);
            if (ret == E_OK)
            {
                KeyM_CopyData(pubAlgoPtr->dataEle, *cerS, pubAlgoPtr->len);
                if (pubAlgoPtr->next == NULL_PTR)
                {
                    *cerS += (cerEndPtr - *cerS);
                }
                else
                {
                    /*Optional parameters*/
                    *cerS += pubAlgoPtr->len;
                    pubAlgoPtr = pubAlgoPtr->next;

                    pubAlgoPtr->tag = **cerS;
                    if (pubAlgoPtr->tag == KEYM_CERT_ASN1_OID)
                    {
                        ret = KeyM_CertGetAsn1Tag(cerS, cerEndPtr, &pubAlgoPtr->len, KEYM_CERT_ASN1_OID);
                        if (ret == E_OK)
                        {
                            KeyM_CopyData(pubAlgoPtr->dataEle, *cerS, pubAlgoPtr->len);
                            *cerS += pubAlgoPtr->len;
                        }
                    }
                    else /*KEYM_CERT_ASN1_NULL*/
                    {
                        pubAlgoPtr->len = 0;
                        (*cerS) += 2u;
                    }

                    if ((ret == E_OK) && (*cerS != cerEndPtr))
                    {
                        ret = E_NOT_OK;
                    }
                }
            }
        }
    }
    return ret;
}

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertGetPubKeyInfo(
    KeyM_CertificateIdType certId,
    P2VAR(uint8*, AUTOMATIC, KEYM_APPL_DATA) cerS,
    P2CONST(uint8, TYPEDEF, KEYM_CONST) cerE)
{
    uint32 len;
    uint32 tag;
    KeyM_CertEleType certEle;
    uint8 objType = KEYM_CERT_ASN1_BIT_STRING;
    Std_ReturnType ret;
    const uint8* temp_EndAddr = cerE;

    ret = KeyM_CertGetAsn1Tag(cerS, temp_EndAddr, &len, KEYM_CERT_ASN1_SEQ_CONSTRU);
    if (ret == E_OK)
    {

        temp_EndAddr = *cerS + len;
        ret = KeyM_CertHandleGetPubKeyAlgo(certId, cerS, temp_EndAddr);
        if (ret == E_OK)
        {
            /*Public key info.*/
            tag = **cerS;
            ret = KeyM_CertGetAsn1Tag(cerS, temp_EndAddr, &len, KEYM_CERT_ASN1_BIT_STRING);
            if (ret == E_OK)
            {
                certEle.certEleMaxLen = (uint16)len;
                certEle.certEleObjType = &objType;
                certEle.certEleStruct = CERTIFICATE_SUBJECT_PUBLIC_KEY_INFO_SUBJECT_PUBLIC_KEY;
                ret = KeyM_CertEleHandle(certId, &certEle, *cerS, tag);
                if (ret == E_OK)
                {
                    *cerS += len;
                    if (*cerS != temp_EndAddr)
                    {
                        ret = E_NOT_OK;
                    }
                }
            }
        }
    }

    return ret;
}

KEYM_LOCAL FUNC(void, KEYM_CODE) KeyM_CertSkipField(
    P2VAR(uint8*, AUTOMATIC, KEYM_APPL_DATA) cerS,
    P2CONST(uint8, TYPEDEF, KEYM_CONST) cerE,
    uint8 type)
{
    uint32 len;
    Std_ReturnType ret;

    if (**cerS == type)
    {
        ret = KeyM_CertGetAsn1Tag(cerS, cerE, &len, type);
        if (ret == (Std_ReturnType)E_OK)
        {
            *cerS += len;
        }
    }
}

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertGetExtention(
    KeyM_CertificateIdType certId,
    P2VAR(uint8*, AUTOMATIC, KEYM_APPL_DATA) cerS,
    P2CONST(uint8, TYPEDEF, KEYM_CONST) cerE)
{
    uint32 len;
    uint32 tag;
    KeyM_CertEleType certEle;
    uint8 objType = KEYM_CERT_ASN1_TAG | 3u;
    Std_ReturnType ret;

    if (**cerS == 0xa3u)
    {
        tag = 0xa3u;
        ret = KeyM_CertGetAsn1Tag(cerS, cerE, &len, 0xa3u);
        if (ret == (Std_ReturnType)E_OK)
        {
            certEle.certEleMaxLen = (uint16)len;
            certEle.certEleObjType = &objType;
            certEle.certEleStruct = CERTIFICATE_EXTENSION;
            ret = KeyM_CertEleHandle(certId, &certEle, *cerS, tag);
            *cerS += len;
        }
    }
    else
    {
        ret = E_NOT_OK;
    }
    return ret;
}

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertGetSignature(
    KeyM_CertificateIdType certId,
    P2VAR(uint8*, AUTOMATIC, KEYM_APPL_DATA) cerS,
    P2CONST(uint8, TYPEDEF, KEYM_CONST) cerE)
{
    uint32 len;
    uint32 tag;
    KeyM_CertEleType certEle;
    uint8 objType = KEYM_CERT_ASN1_BIT_STRING;
    Std_ReturnType ret;

    tag = **cerS;
    ret = KeyM_CertGetAsn1Tag(cerS, cerE, &len, KEYM_CERT_ASN1_BIT_STRING);
    if (ret == E_OK)
    {
        certEle.certEleMaxLen = (uint16)len;
        certEle.certEleObjType = &objType;
        certEle.certEleStruct = CERTIFICATE_SIGNATURE;
        /*unused bit*/
        if ((len % 0x08u) != 0u)
        {
            *cerS += 1;
            certEle.certEleMaxLen -= 1u;
        }
        ret = KeyM_CertEleHandle(certId, &certEle, *cerS, tag);
        if (ret == E_OK)
        {
            *cerS += len - 1u;
        }
    }
    return ret;
}

KEYM_LOCAL FUNC(KeyM_Asn1DesType*, KEYM_CODE)
    KeyM_GetCertEle(KeyM_CertificateIdType certId, KeyM_CertEleStructType certEleStru)
{
    P2CONST(KeyM_CertPCfgType, AUTOMATIC, KEYM_CONST)
    certCfgPtr;
    P2VAR(KeyM_Asn1DesType, TYPEDEF, KEYM_VAR)
    certEleBuf;
    uint16 eleIdx;

    certCfgPtr = &KeyM_CertPCfg[certId];
    for (eleIdx = 0u; eleIdx < certCfgPtr->numOfCertEle; eleIdx++)
    {
        if (certEleStru == certCfgPtr->certEleRef[eleIdx].certEleStruct)
        {
            certEleBuf = certCfgPtr->certEleRef[eleIdx].certEleBuf;
            break;
        }
        else
        {
            continue;
        }
    }
    if (eleIdx == certCfgPtr->numOfCertEle)
    {
        certEleBuf = NULL_PTR;
    }
    return certEleBuf;
}

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_CmpCertEle(
    /* PRQA S 3673 ++ */ /* MISRA Rule 8.13 */
    P2VAR(KeyM_Asn1DesType, TYPEDEF, KEYM_VAR) certEleBuf,
    P2VAR(KeyM_Asn1DesType, TYPEDEF, KEYM_VAR) upCertEleBuf)
/* PRQA S 3673 -- */ /* MISRA Rule 8.13 */
{
    Std_ReturnType ret = E_NOT_OK;
    const KeyM_Asn1DesType* temp_certEleBuf = certEleBuf;
    const KeyM_Asn1DesType* temp_upcertEleBuf = upCertEleBuf;

    while ((temp_certEleBuf != NULL_PTR) && (temp_upcertEleBuf != NULL_PTR))
    {
        ret = KeyM_strcmp(temp_certEleBuf->dataEle, temp_upcertEleBuf->dataEle, (uint16)temp_certEleBuf->len);
        if ((temp_certEleBuf->len != temp_upcertEleBuf->len) || (E_NOT_OK == ret))
        {
            break;
        }
        else
        {
            temp_certEleBuf = temp_certEleBuf->next;
            temp_upcertEleBuf = temp_upcertEleBuf->next;
        }
    }
    if ((temp_certEleBuf == NULL_PTR) && (temp_upcertEleBuf == NULL_PTR))
    {
        ret = E_OK;
    }

    return ret;
}

KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_GlobalTimeCheck(KeyM_CertificateIdType certId)
{
    uint64 ts_s;
    uint32 notBefore_s;
    uint32 notAfter_s;
    StbM_TimeStampType stbM_TimeStamp;
    StbM_UserDataType stbM_UserData;
    Std_ReturnType ret = E_NOT_OK;

    if (certId < KEYM_CERT_NUM)
    {
        ret = StbM_GetCurrentTime(KeyM_CertPCfg[certId].certStbMTime, &stbM_TimeStamp, &stbM_UserData);
        if (ret == E_OK)
        {
            /*secondsHi:16 bit MSB of the 48 bits Seconds part of the time*/
            /*seconds:32 bit LSB of the 48 bits Seconds part of the time*/
            /*Convert the current time to seconds*/
            ts_s = (((uint64)stbM_TimeStamp.secondsHi << 32u) | stbM_TimeStamp.seconds);

            /*Validity Not Before Time*/
            notBefore_s = KeyM_ConvertTime_BeijingToUnix(&KeyM_CertStartTime[certId]);

            /*Validity Not After Time*/
            notAfter_s = KeyM_ConvertTime_BeijingToUnix(&KeyM_CertEndTime[certId]);
            if ((ts_s < notBefore_s) || (ts_s > notAfter_s))
            {
                ret = E_NOT_OK;
            }
        }
    }
    return ret;
}

KEYM_LOCAL FUNC(uint8, KEYM_CODE) KeyM_IsLeapYear(uint16 year)
{
    uint8 ret = 0u;

    if ((year % 4u) == 0u)
    {
        if (((year % 100u) == 0u) && ((year % 400u) != 0u))
        {
            /*Common year*/
            ret = 0u;
        }
        else
        {
            /*Leap year*/
            ret = 1u;
        }
    }
    return ret;
}

KEYM_LOCAL FUNC(uint32, KEYM_CODE) KeyM_ConvertTime_BeijingToUnix(P2CONST(KeyM_CertTimeType, TYPEDEF, KEYM_VAR) bjTime)
{
    uint32 dayNum = 0u;
    uint32 secNum;
    uint16 unixStartYear = 1970u;
    uint16 mon = 0u;

    /*1.Calculate the number of days in all years*/
    while (unixStartYear < bjTime->certYear)
    {

        if (KeyM_IsLeapYear(unixStartYear) != 0u)
        {
            dayNum += 366u;
        }
        else
        {
            dayNum += 365u;
        }
        unixStartYear++;
    }
    /*2.Calculate the number of days in all months.*/
    while (mon < (bjTime->certMon - 1u))
    {
        if (KeyM_IsLeapYear(bjTime->certYear) != 0u) /* PRQA S 4461 */ /* MISRA Rule 10.3 */
        {
            dayNum += KeyM_LeapMonthDay[mon];
        }
        else
        {
            dayNum += KeyM_ComMonthDay[mon];
        }
        mon++;
    }

    /*3.Add day*/
    /*No need to plus one-day on the 1st of each month*/
    dayNum += bjTime->certDay - 1u;

    /*4.Hour Minute Second*/
    secNum = dayNum * 24u * 60u * 60u;

    secNum += bjTime->certHour * 60u * 60u; /*Hour*/

    secNum += bjTime->certMin * 60u; /*Minute*/

    secNum += bjTime->certSec; /*Second*/

    /*5.Time zone adjustment*/
    secNum -= 8u * 60u * 60u; /*8:Beijing Time zone.*/
    return secNum;
}

FUNC(Std_ReturnType, KEYM_CODE)
KeyM_HandleCertcVerify(
    /* PRQA S 1532 */ /* MISRA Rule 8.7 */
    P2CONST(KeyM_CertPCfgType, AUTOMATIC, KEYM_CONST) certCfgPtr,
    P2CONST(KeyM_CertPCfgType, TYPEDEF, KEYM_CONST) certUpperHierRef)
{
    P2VAR(KeyM_Asn1DesType, TYPEDEF, KEYM_VAR)
    certEleBuf;
    P2VAR(KeyM_Asn1DesType, TYPEDEF, KEYM_VAR)
    upCertEleBuf;
    /* PRQA S 3678 ++ */ /* MISRA Rule 8.13 */
    P2VAR(KeyM_Asn1DesType, TYPEDEF, KEYM_VAR)
    sigBuf;
    P2VAR(KeyM_Asn1DesType, TYPEDEF, KEYM_VAR)
    pbKeyBuf;
    /* PRQA S 3678 -- */ /* MISRA Rule 8.13 */
    uint32 sigVerJob;
    uint32 tarKey;
    uint16 CertId = certCfgPtr->certId;
#if (KEYM_NVM_BLOCK_NUM > 0)
    uint16 nvmRefBloId;
#endif
    Crypto_VerifyResultType verifyRes = CRYPTO_E_VER_NOT_OK;
    Std_ReturnType ret;

    /*1.Subject check*/
    if (certCfgPtr->selfSignedCert == TRUE)
    {
        /*self-signed : SUBJECT = ISSUER*/
        certEleBuf = KeyM_GetCertEle(CertId, CERTIFICATE_SUBJECT_NAME);
        upCertEleBuf = KeyM_GetCertEle(CertId, CERTIFICATE_ISSUER_NAME);
    }
    else
    {
        certEleBuf = KeyM_GetCertEle(CertId, CERTIFICATE_ISSUER_NAME);
        upCertEleBuf = KeyM_GetCertEle(certUpperHierRef->certId, CERTIFICATE_SUBJECT_NAME);
    }
    if ((certEleBuf == NULL_PTR) || (upCertEleBuf == NULL_PTR))
    {
        ret = KEYM_E_CERT_INVALID_CHAIN_OF_TRUST;
    }
    else
    {
        ret = KeyM_CmpCertEle(certEleBuf, upCertEleBuf);
    }

    if (ret == E_OK)
    {
        /*2.Time stamp check*/
        ret = KeyM_GlobalTimeCheck(CertId);
        if (ret != E_OK)
        {
            KeyM_CertStatus[CertId] = KEYM_E_CERTIFICATE_VALIDITY_PERIOD_FAIL;
        }
        else
        {
            /*3.Certificate elements check*/
            if (certCfgPtr->certRuleArbiFunc != NULL_PTR)
            {
                /*SWS_KeyM_00035&SWS_KeyM_00135*/
                /*The contents of certificate elements shall be checked through by a check of all
                 * KeyMCertCertificateElementRuleRef.*/
                if ((boolean)FALSE == certCfgPtr->certRuleArbiFunc())
                {
                    ret = E_NOT_OK;
                    KeyM_CertStatus[CertId] = KEYM_E_CERTIFICATE_INVALID_CONTENT;
                }
            }
            if (E_OK == ret)
            {

                /*4.Signature check*/
                /*Get public Key*/
                pbKeyBuf = KeyM_GetCertEle(CertId, CERTIFICATE_SUBJECT_PUBLIC_KEY_INFO_SUBJECT_PUBLIC_KEY);
                if (pbKeyBuf == NULL_PTR)
                {
                    ret = E_NOT_OK;
                }
                else
                {
                    /*Get signature.*/
                    sigBuf = KeyM_GetCertEle(CertId, CERTIFICATE_SIGNATURE);
                    if (sigBuf == NULL_PTR)
                    {
                        ret = E_NOT_OK;
                    }
                    else
                    {
                        sigVerJob = certUpperHierRef->certSigVeriJob;
                        if (certCfgPtr->selfSignedCert == TRUE)
                        {
                            /*For self-signed certificates, first the public key of this certificate needs
                             * to be set*/
                            ret = Csm_KeyElementSet(
                                certCfgPtr->certSigVeriKey,
                                CRYPTO_KE_SIGNATURE_KEY,
                                pbKeyBuf->dataEle,
                                pbKeyBuf->len);
                            (void)Csm_KeySetValid(certCfgPtr->certSigVeriKey);
                            sigVerJob = certCfgPtr->certSigVeriJob;
                        }

                        /*Use CA's public Key.*/
                        /*SWS_KeyM_00032*/
                        if (ret == E_OK)
                        {
                            ret = Csm_SignatureVerify(
                                sigVerJob,
                                CRYPTO_OPERATIONMODE_SINGLECALL,
                                certCfgPtr->tbsCert->dataEle,
                                certCfgPtr->tbsCert->len,
                                sigBuf->dataEle,
                                sigBuf->len,
                                &verifyRes);
                        }

                        if (ret != E_OK)
                        {
                            KeyM_CertStatus[CertId] = KEYM_E_CERTIFICATE_SIGNATURE_FAIL;
                        }
                        else
                        {
                            /*sync job finished*/
                            if (verifyRes == CRYPTO_E_VER_OK)
                            {
                                KeyM_CertStatus[CertId] = KEYM_CERTIFICATE_VALID;
                                if (certCfgPtr->certVeriCbkFunc != NULL_PTR)
                                {
                                    (void)certCfgPtr->certVeriCbkFunc(CertId, KeyM_CertStatus[CertId]);
                                }
                                else
                                {
                                    /*do nothing*/
                                }
                            }
                        }

                        /*Validation succeeded,the public key of the certificate shall be set and
                         * validated.*/
                        if ((certCfgPtr->certStorage == KEYM_STORAGE_IN_NVM) && (E_OK == ret))
                        {
#if (KEYM_NVM_BLOCK_NUM > 0)
                            nvmRefBloId = *certCfgPtr->certNvmBlkRef;

                            if (nvmRefBloId < KEYM_NVM_BLOCK_NUM) /* PRQA S 2981,2996,2992 */ /* MISRA Rule 14.3
                                                                                                 Rule 2.2 */
                            {
                                if (KeyM_NvmBlockPCfg[nvmRefBloId].blkWriDelay
                                    == 0u) /* PRQA S 2880 */ /* MISRA Rule 2.1 */
                                {
                                    /*Public-Key is Written immediately*/
                                    (void)NvM_WriteBlock(
                                        KeyM_NvmBlockPCfg[nvmRefBloId].blkId,
                                        certCfgPtr->tbsCert->dataEle);
                                }
                                else
                                {
                                    KeyM_BlkWriDelayRun[nvmRefBloId].timeStart = TRUE;
                                    KeyM_BlkWriDelayRun[nvmRefBloId].delayTime =
                                        KeyM_NvmBlockPCfg[nvmRefBloId].blkWriDelay;
                                    /*Record certificate start address.*/
                                    KeyM_BlkWriDelayRun[nvmRefBloId].nvmCertEle = certCfgPtr->tbsCert;
                                }
                            }
#endif
                        }
                        else if (certCfgPtr->certStorage == KEYM_STORAGE_IN_CSM)
                        {
                            if (certCfgPtr->certKeyTarRef != NULL_PTR)
                            {
                                tarKey = *(certCfgPtr->certKeyTarRef);
                                KeyM_HandleCsmKeyStorage(tarKey, CertId, TRUE);
                                (void)Csm_KeySetValid(tarKey);
                            }
                        }
                        else /*KEYM_STORAGE_IN_RAM*/
                        {
                            /*Do nothing.*/
                        }
                    }
                }
            }
        }
    }

    if (ret != E_OK)
    {
        if (certCfgPtr->certVeriCbkFunc != NULL_PTR)
        {
            (void)certCfgPtr->certVeriCbkFunc(CertId, KeyM_CertStatus[CertId]);
        }
        else
        {
            /*do nothing*/
        }
    }
    return ret;
}
#endif                    /*STD_ON == KEYM_CERTIFICATE_MANAGER_ENABLED*/
/* PRQA S 0488 -- */      /* MISAR Rule-18.4 */
/* PRQA S 4603,4604 -- */ /* MISAR Rule-21.2 */
/* PRQA S 3432 --*/       /* MISRA Rule 20.7 */
/* PRQA S 1503 --*/       /* MISRA Rule 2.1 */
