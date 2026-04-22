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
/*
********************************************************************************
**                                                                            **
**  FILENAME    : KeyM.h                                                      **
**                                                                            **
**  Created on  :                                                             **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION : API declaration and type definitions of KeyManager          **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R21-11                      **
**                                                                            **
*******************************************************************************/

/******************************************************************************
**                      Revision Control History                             **
******************************************************************************/

/**
 \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
   ModeName:KeyM<br>
 RuleSorce:puhua-rule.rcf 2.3.1

    \li VL_KeyM_2991
      Reason: Results are related to parameters.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_KeyM_2995
      Reason: Results are related to parameters.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_KeyM_2963
      Reason: Object will be modified by function.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_KeyM_0771
      Reason: More than one break statement.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_KeyM_0779
      Reason: User defined characters, with configuration tools ensuring the accuracy of character matching.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_KeyM_0489
      Reason: Added or subtracted to a pointer.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_KeyM_2755
      Reason: Function execution time may be long.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_KeyM_2822
      Reason: NULL pointer operation.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_KeyM_2877
      Reason: The exception is that the number of loops is configurable.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_KeyM_0772_2001
      Reason: Need goto to simplify cert element parse.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_KeyM_3440
      Reason: Pointer operation is necessary
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_KeyM_4461
      Reason: Macros need to be used within operation.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_KeyM_2996
      Reason: Parameters will be assigned value in actual use
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_KeyM_3206
      Reason: Parameter is defined in AUTOSAR specification
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_KeyM_STCDN
      Reason: Special file, with fewer annotations.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_KeyM_3332
      Reason: Undefined macro, but in fact the macro exists in the configuration file.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_KeyM_0488
      Reason: Calculates the offset value of the array.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_KeyM_4603
      Reason: Dereference pointer.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_KeyM_4604
      Reason: A reserved identifier or reserved macro name shall not be declared.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_KeyM_3218
      Reason: Used to record global information, such as the timestamp of the first entry into the main function, the
    last access to the main function.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_KeyM_3678
      Reason: non-const type declaration is necessary depends on the config.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_KeyM_6010
      Reason: Functionality is not separated into subfunctions because there are higher requirements for minimizing the
  stack and applying it to runtime use of the code.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_KeyM_6020
      Reason: Some function logics are overly complex. Splitting and implementing them will lead to reduced performance.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_KeyM_0311
      Reason: Compatible with different types of functions.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_KeyM_1881
      Reason: The code is designed so
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_KeyM_1822
      Reason: Pointers are compared with integers.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_KeyM_3387
      Reason: The pointer increases or decreases by itself.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_KeyM_1852
      Reason: Pointers are compared with integers.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_KeyM_3469
      Reason: A function should be used in preference to a function-like macro where they are interchangeable.
      Risk: No impact on integrated functions.
      Prevention: Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_MTR_KeyM_6040
      Reason: The parameters of the function need.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_KeyM_1505
      Reason: External interface.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_KeyM_3397
      Reason: The code is designed so
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

*/

#ifndef KEYM_H
#define KEYM_H
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Std_Types.h"
#include "Rte_KeyM_Type.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/

#define KEYM_MODULE_ID        115U
#define KEYM_H_VENDOR_ID      62U
#define KEYM_INSTANCE_ID      7U
#define KEYM_SW_MAJOR_VERSION 2U
#define KEYM_SW_MINOR_VERSION 1U
#define KEYM_SW_PATCH_VERSION 11U

#define KEYM_AR_MAJOR_VERSION 4U
#define KEYM_AR_MINOR_VERSION 7U
#define KEYM_AR_PATCH_VERSION 0U

#define KEYM_INIT_ID                     ((uint8)0x01)
#define KEYM_DEINIT_ID                   ((uint8)0x02)
#define KEYM_GETVERSIONINFO_ID           ((uint8)0x03)
#define KEYM_START_ID                    ((uint8)0x04)
#define KEYM_PREPARE_ID                  ((uint8)0x05)
#define KEYM_UPDATE_ID                   ((uint8)0x06)
#define KEYM_FINAL_ID                    ((uint8)0x07)
#define KEYM_VERIFY_ID                   ((uint8)0x08)
#define KEYM_SERV_CERTIFICATE_ID         ((uint8)0x09)
#define KEYM_SET_CERTIFICATE_ID          ((uint8)0x0A)
#define KEYM_GET_CERTIFICATE_ID          ((uint8)0x0B)
#define KEYM_VERIFY_CERTIFICATES_ID      ((uint8)0x0C)
#define KEYM_VERIFY_CERTIFICATE_ID       ((uint8)0x0D)
#define KEYM_VERIFY_CERTIFICATE_CHAIN_ID ((uint8)0x0E)
#define KEYM_CERT_ELEMENT_GET_ID         ((uint8)0x0F)
#define KEYM_CERT_ELEMENT_GET_FIRST_ID   ((uint8)0x10)
#define KEYM_CERT_ELEMENT_GET_NEXT_ID    ((uint8)0x11)
#define KEYM_CERT_GET_STATUS_ID          ((uint8)0x12)
#define KEYM_CERT_ELEMENT_GET_BY_INDEX   ((uint8)0x1B)
#define KeyM_CertElementGetCount_ID      ((uint8)0x1c)

/* Development Error Types */
#define KEYM_E_PARAM_POINTER  ((uint8)0x01)
#define KEYM_E_SMALL_BUFFER   ((uint8)0x02)
#define KEYM_E_UNINIT         ((uint8)0x03)
#define KEYM_E_INIT_FAILED    ((uint8)0x04)
#define KEYM_E_CONFIG_FAILURE ((uint8)0x05)

/*Extension to the Std_ReturnType for KeyManager module */
#define KEYM_E_BUSY                        ((uint8)0x02)
#define KEYM_E_PENDING                     ((uint8)0x03)
#define KEYM_E_KEY_CERT_SIZE_MISMATCH      ((uint8)0x04)
#define KEYM_E_PARAMETER_MISMATCH          ((uint8)0x05)
#define KEYM_E_KEY_CERT_INVALID            ((uint8)0x06)
#define KEYM_E_CERT_INVALID                ((uint8)0x06)
#define KEYM_E_KEY_CERT_WRITE_FAIL         ((uint8)0x07)
#define KEYM_E_KEY_CERT_UPDATE_FAIL        ((uint8)0x08)
#define KEYM_E_KEY_CERT_READ_FAIL          ((uint8)0x09)
#define KEYM_E_KEY_CERT_EMPTY              ((uint8)0x0A)
#define KEYM_E_CERT_INVALID_CHAIN_OF_TRUST ((uint8)0x0B)

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
/*This structure is the base type to initialize the Key Manager module.*/
typedef struct
{
    uint8 reserved;
} KeyM_ConfigType;

/*Specifies the type of key handler update operation that was performed in the callback.*/
typedef enum
{
    KEYM_KH_UPDATE_KEY_UPDATE_REPEAT = 1U,
    KEYM_KH_UPDATE_FINISH
} KeyM_KH_UpdateOperationType;

/*TLV*/
typedef struct KeyM_Asn1Type
{
    uint32 tag;
    uint32 len;
    P2VAR(uint8, TYPEDEF, KEYM_VAR)
    dataEle;
    P2VAR(struct KeyM_Asn1Type, TYPEDEF, KEYM_VAR) next;
} KeyM_Asn1DesType;

/*This structure is used to iterate through a number of elements of a certificate.*/
typedef struct
{
    KeyM_CertificateIdType certId;
    KeyM_CertElementIdType certEleId;
    P2VAR(KeyM_Asn1DesType, TYPEDEF, KEYM_VAR)
    eleItrator;
} KeyM_CertElementIteratorType;

/*Crypto key handle.*/
typedef uint16 KeyM_CryptoKeyIdType;

typedef uint8* KeyM_CertDataPointerType;

/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
FUNC(void, KEYM_CODE)
KeyM_Init(P2CONST(KeyM_ConfigType, AUTOMATIC, KEYM_APPL_DATA) ConfigPtr);

FUNC(void, KEYM_CODE)
KeyM_Deinit(void);

FUNC(void, KEYM_CODE)
KeyM_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, KEYM_APPL_DATA) VersionInfo);

/*Key*/
FUNC(Std_ReturnType, CANIF_CODE)
KeyM_Start(
    VAR(KeyM_StartType, AUTOMATIC) StartType,
    P2CONST(uint8, AUTOMATIC, KEYM_APPL_CONST) RequestData,
    VAR(uint16, AUTOMATIC) RequestDataLength,
    P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) ResponseData,
    P2VAR(uint16, AUTOMATIC, KEYM_APPL_DATA) ResponseDataLength);

FUNC(Std_ReturnType, KEYM_CODE)
KeyM_Finalize(
    P2CONST(uint8, AUTOMATIC, KEYM_APPL_CONST) RequestDataPtr,
    VAR(uint16, AUTOMATIC) RequestDataLength,
    P2VAR(uint8, AUTOMATIC, AUTOMATIC) ResponseDataPtr,
    VAR(uint16, AUTOMATIC) ResponseMaxDataLength);

FUNC(Std_ReturnType, CANIF_CODE)
KeyM_Prepare(
    P2CONST(uint8, AUTOMATIC, KEYM_APPL_CONST) RequestData,
    VAR(uint16, AUTOMATIC) RequestDataLength,
    P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) ResponseData,
    P2VAR(uint16, AUTOMATIC, KEYM_APPL_DATA) ResponseDataLength);

FUNC(Std_ReturnType, KEYM_CODE)
KeyM_Update(
    P2CONST(uint8, AUTOMATIC, KEYM_APPL_CONST) KeyNamePtr,
    VAR(uint16, AUTOMATIC) KeyNameLength,
    P2CONST(uint8, AUTOMATIC, KEYM_APPL_CONST) RequestDataPtr,
    VAR(uint16, AUTOMATIC) RequestDataLength,
    P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) ResultDataPtr,
    VAR(uint16, AUTOMATIC) ResultDataMaxLength);

/* PRQA S 3332 ++ */ /* VL_KeyM_3332 */
#if (KEYM_CRYPTO_KEY_VERIFY_FUNCTION_ENABLED == STD_ON)
FUNC(Std_ReturnType, KEYM_CODE)
KeyM_Verify(
    P2CONST(uint8, AUTOMATIC, KEYM_APPL_CONST) KeyNamePtr,
    VAR(uint16, AUTOMATIC) KeyNameLength,
    P2CONST(uint8, AUTOMATIC, KEYM_APPL_CONST) RequestData,
    VAR(uint16, AUTOMATIC) RequestDataLength,
    P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) ResponseData,
    P2VAR(uint16, AUTOMATIC, KEYM_APPL_DATA) ResponseDataLength);
#endif /*KEYM_CRYPTO_KEY_VERIFY_FUNCTION_ENABLED == STD_ON*/

#if (KEYM_SERVICE_CERTIFICATE_FUNCTION_ENABLED == STD_ON)
/*Certificate*/
FUNC(Std_ReturnType, KEYM_CODE)
KeyM_ServiceCertificate(
    VAR(KeyM_ServiceCertificateType, AUTOMATIC) Service,
    P2CONST(uint8, AUTOMATIC, KEYM_APPL_CONST) CertNamePtr,
    VAR(uint16, AUTOMATIC) CertNameLength,
    P2CONST(uint8, AUTOMATIC, KEYM_APPL_CONST) RequestData,
    VAR(uint16, AUTOMATIC) RequestDataLength,
    P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) ResponseData,
    VAR(uint16, AUTOMATIC) ResponseDataLength);
#endif /*KEYM_SERVICE_CERTIFICATE_FUNCTION_ENABLED == STD_ON*/
/* PRQA S 3332 -- */

FUNC(Std_ReturnType, KEYM_CODE)
KeyM_SetCertificate(
    VAR(KeyM_CertificateIdType, AUTOMATIC) CertId,
    P2CONST(KeyM_CertDataType, AUTOMATIC, KEYM_APPL_CONST) CertificateDataPtr);

FUNC(Std_ReturnType, KEYM_CODE)
KeyM_GetCertificate(
    VAR(KeyM_CertificateIdType, AUTOMATIC) CertId,
    P2VAR(KeyM_CertDataType, AUTOMATIC, KEYM_APPL_DATA) CertificateDataPtr);

FUNC(Std_ReturnType, KEYM_CODE)
KeyM_VerifyCertificates(
    VAR(KeyM_CertificateIdType, AUTOMATIC) CertId,
    VAR(KeyM_CertificateIdType, AUTOMATIC) CertUpperId);

FUNC(Std_ReturnType, KEYM_CODE)
KeyM_VerifyCertificate(VAR(KeyM_CertificateIdType, AUTOMATIC) CertId);

FUNC(Std_ReturnType, KEYM_CODE)
KeyM_VerifyCertificateChain(
    VAR(KeyM_CertificateIdType, AUTOMATIC) CertId,
    CONST(KeyM_CertDataType, AUTOMATIC) certChainData[],
    VAR(uint8, AUTOMATIC) NumberOfCertificates);

FUNC(Std_ReturnType, KEYM_CODE)
KeyM_CertElementGet(
    VAR(KeyM_CertificateIdType, AUTOMATIC) CertId,
    VAR(KeyM_CertElementIdType, AUTOMATIC) CertElementId,
    P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) CertElementData,
    P2VAR(uint32, AUTOMATIC, KEYM_APPL_DATA) CertElementDataLength);

FUNC(Std_ReturnType, KEYM_CODE)
KeyM_CertificateElementGetByIndex(
    VAR(KeyM_CertificateIdType, AUTOMATIC) CertId,
    VAR(KeyM_CertElementIdType, AUTOMATIC) CertElementId,
    VAR(uint32, AUTOMATIC) Index,
    P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) CertElementDataPtr,
    P2VAR(uint32, AUTOMATIC, KEYM_APPL_DATA) CertElementDataLengthPtr);

FUNC(Std_ReturnType, KEYM_CODE)
KeyM_CertificateElementGetCount(
    VAR(KeyM_CertificateIdType, AUTOMATIC) CertId,
    VAR(KeyM_CertElementIdType, AUTOMATIC) CertElementId,
    P2VAR(uint16, AUTOMATIC, KEYM_APPL_DATA) CountPtr);

FUNC(Std_ReturnType, KEYM_CODE)
KeyM_CertElementGetFirst(
    VAR(KeyM_CertificateIdType, AUTOMATIC) CertId,
    VAR(KeyM_CertElementIdType, AUTOMATIC) CertElementId,
    P2VAR(KeyM_CertElementIteratorType, AUTOMATIC, KEYM_APPL_DATA) CertElementIterator,
    P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) CertElementData,
    P2VAR(uint32, AUTOMATIC, KEYM_APPL_DATA) CertElementDataLength);

FUNC(Std_ReturnType, KEYM_CODE)
KeyM_CertElementGetNext(
    P2VAR(KeyM_CertElementIteratorType, AUTOMATIC, KEYM_APPL_DATA) CertElementIterator,
    P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) CertElementData,
    P2VAR(uint32, AUTOMATIC, KEYM_APPL_DATA) CertElementDataLength);

FUNC(Std_ReturnType, KEYM_CODE)
KeyM_CertGetStatus(
    VAR(KeyM_CertificateIdType, AUTOMATIC) CertId,
    P2VAR(KeyM_CertificateStatusType, AUTOMATIC, KEYM_APPL_DATA) Status);

FUNC(Std_ReturnType, KEYM_CODE)
KeyM_CertSetStatus(VAR(KeyM_CertificateIdType, AUTOMATIC) CertId, KeyM_CertificateStatusType Status);

FUNC(void, KEYM_CODE)
KeyM_CertStoreNvmHandle(void);

#endif /*KEYM_H*/
