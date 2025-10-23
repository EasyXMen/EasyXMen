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
 ***********************************************************************************************************************
 **
 **  @file               : keyM_Certificate.c
 **  @author             : jie.gu
 **  @date               : 2025/03/31
 **  @vendor             : isoft
 **  @description        : KeyM  Certificate Interface Implementation source
 **
 **********************************************************************************************************************/

/* =================================================== inclusions =================================================== */

/* PRQA S 0488 ++ */      /* VL_KeyM_0488 */
/* PRQA S 4603,4604 ++ */ /* VL_KeyM_4603,VL_KeyM_4604 */
/* PRQA S 3432 ++ */      /* VL_KeyM_3432 */
/* PRQA S 1503 ++ */      /* VL_QAC_NoUsedApi */

#include "KeyM_Internal.h"
#if (STD_ON == KEYM_CERTIFICATE_MANAGER_ENABLED)
#include "StbM.h"

/* =================================================== macros =================================================== */

#define KEYM_LONG_FORMAT_MASK 0x80u /**< Mask to identify long format in ASN.1 encoding. */

#define KEYM_CERT_ASN1_BOOLEAN          0x01u /**< ASN.1 tag for boolean type. */
#define KEYM_CERT_ASN1_INTEGER          0x02u /**< ASN.1 tag for integer type. */
#define KEYM_CERT_ASN1_BIT_STRING       0x03u /**< ASN.1 tag for bit string type. */
#define KEYM_CERT_ASN1_OCTET_STRING     0x04u /**< ASN.1 tag for octet string type. */
#define KEYM_CERT_ASN1_NULL             0x05u /**< ASN.1 tag for null type. */
#define KEYM_CERT_ASN1_OID              0x06u /**< ASN.1 tag for object identifier (OID) type. */
#define KEYM_CERT_ASN1_UINT8_STRING     0x0Cu /**< ASN.1 tag for UTF8 string type. */
#define KEYM_CERT_ASN1_ASCII_STRING     0x13u /**< ASN.1 tag for printable string type (ASCII). */
#define KEYM_CERT_ASN1_IA5_STRING       0X16U /**< ASN.1 tag for IA5 string type . */
#define KEYM_CERT_ASN1_UTC_TIME         0x17u /**< ASN.1 tag for UTC time type. */
#define KEYM_CERT_ASN1_GENERALIZED_TIME 0x18u /**< ASN.1 tag for generalized time type. */
#define KEYM_CERT_ASN1_SEQ_CONSTRU      0x30u /**< ASN.1 tag for constructed sequence type. */
#define KEYM_CERT_ASN1_SET_CONSTRU      0x31u /**< ASN.1 tag for constructed set type. */
#define KEYM_CERT_ASN1_TAG              0xA0u /**< ASN.1 context-specific tag. */
                                              /**< Check val is in the range of min to max. */
#define KEYM_CHECK_RANGE(max, val) \
    do                             \
    {                              \
        if ((val) > (max))         \
        {                          \
            return (ret);          \
        }                          \
    } while (0)

/* ========================================== internal function declarations ======================================== */
/**
 * @brief          Checks the general parameters for certificate operations.
 *                 This function validates the provided parameters for certificate operations, including:
 *                 1. Initialization status of the Key Management module.
 *                 2. Validity of pointer parameters (CertElementData and CertElementDataLength).
 *                 3. Validity of the certificate ID and certificate element ID.
 *                 4. Length of the certificate element data.
 *                 5. Validity of the certificate status.
 *                 6. Non-empty status of the certificate element buffer.
 *                 If any of these checks fail, it reports a development error and returns an error code.
 * @param[in]      CertId : ID of the certificate.
 * @param[in]      CertElementId : ID of the certificate element.
 * @param[in]      CertElementData : Pointer to the certificate element data.
 * @param[in]      CertElementDataLength : Pointer to the length of the certificate element data.
 * @param[in]      ApiId : ID of the API calling this function (for error reporting).
 * @return         Std_ReturnType
 * @retval         E_OK:All parameters are valid.
 * @retval         E_NOT_OK:One or more parameters are invalid.
 * @retval         KEYM_E_UNINIT:The Key Management module is not initialized.
 * @retval         KEYM_E_PARAM_POINTER:One or more pointer parameters are invalid.
 * @retval         KEYM_E_PARAMETER_MISMATCH:The certificate ID or certificate element ID is invalid.
 * @retval         KEYM_E_KEY_CERT_SIZE_MISMATCH:The length of the certificate element data is invalid.
 * @retval         KEYM_E_KEY_CERT_INVALID:The certificate is invalid.
 * @retval         KEYM_E_KEY_CERT_EMPTY:The certificate element buffer is empty.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71436
 */
KEYM_LOCAL Std_ReturnType KeyM_CheckGeneralParam(
    KeyM_CertificateIdType CertId,
    KeyM_CertElementIdType CertElementId,
    const uint8*           CertElementData,
    const uint32*          CertElementDataLength,
    uint8                  ApiId);
/**
 * @brief          Checks if a specific certificate element exists for a given certificate.
 *                 This function iterates through the certificate elements configured for the specified certificate
 *                 and checks if an element with the specified structure type exists.
 *                 If the element is found, it returns E_OK and optionally sets the provided configuration ID.
 * @param[in]      certId : ID of the certificate to check.
 * @param[in]      certStr : Structure type of the certificate element to look for.
 * @param[out]     cfgId : Pointer to store the configuration ID of the found certificate element (optional).
 * @return         Std_ReturnType
 * @retval         E_OK:The specified certificate element exists.
 * @retval         E_NOT_OK:The specified certificate element does not exist.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71437
 */
KEYM_LOCAL Std_ReturnType
    KeyM_IsCertExist(KeyM_CertificateIdType certId, KeyM_CertEleStructType certStr, uint16* cfgId);

/**
 * @brief          Finds a specific certificate element within a certificate configuration.
 *                 This function iterates through the certificate elements configured for the specified certificate
 *                 and checks if an element matches the provided certificate element configuration.
 *                 The match is based on the maximum length, structure type, object type, and object ID.
 *                 If a matching element is found, it returns E_OK and sets the provided element index.
 * @param[in]      certId : ID of the certificate to search in.
 * @param[in]      certEle : Pointer to the certificate element configuration to match.
 * @param[out]     eleIdx : Pointer to store the index of the found certificate element.
 * @return         Std_ReturnType
 * @retval         E_OK:The specified certificate element was found.
 * @retval         E_NOT_OK:The specified certificate element was not found.
 * @retval         KEYM_E_CERTIFICATE_INVALID_TYPE:The certificate element type is invalid.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71438
 */
KEYM_LOCAL Std_ReturnType
    KeyM_FindCertElement(KeyM_CertificateIdType certId, const KeyM_CertEleType* certEle, uint16* eleIdx);

/**
 * @brief          Handles the processing of a certificate element.
 *                 This function finds the configuration index for the specified certificate element using
 * `KeyM_FindCertElement`. If the element is found, it copies the provided data into the certificate element buffer and
 * updates the tag and length.
 * @param[in]      certId : ID of the certificate to process.
 * @param[in]      certEle : Pointer to the certificate element configuration.
 * @param[in]      eleData : Pointer to the data to be copied into the certificate element buffer.
 * @param[in]      tag : Tag to be associated with the certificate element.
 * @return         Std_ReturnType
 * @retval         E_OK:The certificate element was processed successfully.
 * @retval         E_NOT_OK:The certificate element was not found or could not be processed.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71439
 */
KEYM_LOCAL Std_ReturnType KeyM_CertEleHandle(
    KeyM_CertificateIdType  certId,
    const KeyM_CertEleType* certEle,
    const uint8*            eleData,
    uint32                  tag);

/**
 * @brief          Retrieves the ASN.1 tag from a certificate and verifies its type.
 *                 This function checks if the current position in the certificate data matches the expected ASN.1 tag.
 *                 If the tag matches, it increments the pointer to the next byte and calls `KeyM_CertGetAsn1Len` to get
 * the length of the ASN.1 structure.
 * @param[in,out]  cerS : Pointer to the current position in the certificate data.
 * @param[in]      cerE : Pointer to the end of the certificate data.
 * @param[out]     cerLenPtr : Pointer to store the length of the ASN.1 structure.
 * @param[in]      tag : Expected ASN.1 tag to match.
 * @return         Std_ReturnType
 * @retval         E_OK:The ASN.1 tag was successfully retrieved and matches the expected type.
 * @retval         E_NOT_OK:The ASN.1 tag is invalid or does not match the expected type.
 * @note           This function is used internally by the Key Management module to parse ASN.1 structures in
 * certificate data.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71440
 */
KEYM_LOCAL Std_ReturnType KeyM_CertGetAsn1Tag(uint8** cerS, const uint8* cerE, uint32* cerLenPtr, uint8 tag);

/**
 * @brief          Retrieves the ASN.1 length from a certificate byte array.
 * @param[in]      cerS : Pointer to the start of the certificate byte array.
 * @param[in]      cerE : Pointer to the end of the certificate byte array.
 * @param[out]     cerLen : Pointer to the destination variable where the ASN.1 length will be stored.
 * @return         Std_ReturnType
 * @retval         E_OK: The ASN.1 length is successfully retrieved and no development error is detected.
 * @retval         E_NOT_OK: The ASN.1 length cannot be retrieved due to invalid format or insufficient data.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71441
 */
KEYM_LOCAL Std_ReturnType KeyM_CertGetAsn1Len(uint8** cerS, const uint8* cerE, uint32* cerLen);

/**
 * @brief          Retrieves an ASN.1 integer tag from a certificate byte array and extracts its value.
 * @param[in]      cerS : Pointer to the start of the certificate byte array.
 * @param[in]      cerE : Pointer to the end of the certificate byte array.
 * @param[out]     intVal : Pointer to the destination variable where the integer value will be stored.
 * @param[out]     cerLen : Pointer to the destination variable where the length of the integer tag will be stored.
 * @return         Std_ReturnType
 * @retval         E_OK: The ASN.1 integer tag is successfully retrieved and its value is extracted without any
 * development error.
 * @retval         E_NOT_OK: The ASN.1 integer tag cannot be retrieved or its value cannot be extracted due to invalid
 * format, insufficient data, or other errors.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71442
 */
KEYM_LOCAL Std_ReturnType KeyM_CertGetAsn1TagInt(uint8** cerS, const uint8* cerE, uint8* intVal, uint32* cerLen);

/**
 * @brief          Retrieves the version number of a certificate.
 * @param[in]      certId : The ID of the certificate.
 * @param[in]      cerS : Pointer to the start of the certificate byte array.
 * @param[in]      cerE : Pointer to the end of the certificate byte array.
 * @return         Std_ReturnType
 * @retval         E_OK: The version number is successfully retrieved and stored without any development error.
 * @retval         E_NOT_OK: The version number cannot be retrieved due to invalid format, insufficient data, or other
 * errors.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71443
 */
KEYM_LOCAL Std_ReturnType KeyM_CertGetVersion(KeyM_CertificateIdType certId, uint8** cerS, const uint8* cerE);

/**
 * @brief          Retrieves the serial number version of a certificate.
 * @param[in]      certId : The ID of the certificate.
 * @param[in]      cerS : Pointer to the start of the certificate byte array.
 * @param[in]      cerE : Pointer to the end of the certificate byte array.
 * @return         Std_ReturnType
 * @retval         E_OK: The serial number version is successfully retrieved and stored without any development error.
 * @retval         E_NOT_OK: The serial number version cannot be retrieved due to invalid format, insufficient data, or
 * other errors.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71444
 */
KEYM_LOCAL Std_ReturnType KeyM_CertGetSerialVersion(KeyM_CertificateIdType certId, uint8** cerS, const uint8* cerE);
/**
 * @brief          Retrieves the signature algorithm identifier from a certificate.
 * @param[in]      certId : The ID of the certificate.
 * @param[in]      cerS : Pointer to the start of the certificate byte array.
 * @param[in]      cerE : Pointer to the end of the certificate byte array.
 * @param[out]     sigPar : Pointer to the structure where the signature algorithm parameters will be stored.
 * @return         Std_ReturnType
 * @retval         E_OK: The signature algorithm identifier is successfully retrieved and stored without any development
 * error.
 * @retval         E_NOT_OK: The signature algorithm identifier cannot be retrieved due to invalid format, insufficient
 * data, or other errors.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71445
 */
KEYM_LOCAL Std_ReturnType KeyM_CertGetSigAlgorithmId(
    KeyM_CertificateIdType certId,
    uint8**                cerS,
    const uint8*           cerE,
    KeyM_Asn1DesType*      sigPar);

/**
 * @brief          Handles the name information in a certificate, extracting the issuer name and its OID.
 * @param[in]      cerS : Pointer to the start of the certificate byte array containing the name information.
 * @param[in]      cerE : Pointer to the end of the certificate byte array.
 * @param[out]     isuName : Pointer to the structure where the issuer name data will be stored.
 * @param[out]     isuNameOid : Pointer to the structure where the issuer name OID will be stored.
 * @return         Std_ReturnType
 * @retval         E_OK: The issuer name and its OID are successfully extracted and stored without any development
 * error.
 * @retval         E_NOT_OK: The issuer name or its OID cannot be extracted due to invalid format, insufficient data, or
 * other errors.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71446
 */
KEYM_LOCAL Std_ReturnType
    KeyM_CertHandleName(uint8** cerS, const uint8* cerE, KeyM_Asn1DesType* isuName, KeyM_Asn1DesType* isuNameOid);

/**
 * @brief          Retrieves and processes the name information (such as issuer or subject name) from a certificate.
 * @param[in]      certId : The ID of the certificate.
 * @param[in]      cerS : Pointer to the start of the certificate byte array containing the name information.
 * @param[in]      cerE : Pointer to the end of the certificate byte array.
 * @param[in]      eleNameType : The type of name element to retrieve (e.g., issuer name or subject name).
 * @param[in]      eleNameOidType : The type of name OID element to retrieve (e.g., issuer name OID or subject name
 * OID).
 * @return         Std_ReturnType
 * @retval         E_OK: The name information is successfully retrieved and processed without any development error.
 * @retval         E_NOT_OK: The name information cannot be retrieved or processed due to invalid format, insufficient
 * data, or other errors.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71447
 */
KEYM_LOCAL Std_ReturnType KeyM_CertGetName(
    KeyM_CertificateIdType certId,
    uint8**                cerS,
    const uint8*           cerE,
    KeyM_CertEleStructType eleNameType,
    KeyM_CertEleStructType eleNameOidType);

/**
 * @brief          Parses a time element from a given byte array and converts it to a numerical value.
 * @param[in]      ele : Pointer to the byte array containing the time element.
 * @param[in]      len : Length of the byte array.
 * @param[out]     time : Pointer to the destination variable where the parsed time value will be stored.
 * @return         Std_ReturnType
 * @retval         E_OK: The time element is successfully parsed and no development error is detected.
 * @retval         KEYM_E_KEY_CERT_INVALID: The time element contains invalid characters (not between '0' and '9').
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71448
 */
KEYM_LOCAL Std_ReturnType KeyM_CertParseTimeEle(uint8** ele, uint32 len, uint32* time);

/**
 * @brief          Validates the date components of a certificate time structure.
 * @param[in]      time : Pointer to the certificate time structure to validate.
 * @return         Std_ReturnType
 * @retval         E_OK: The date components are valid.
 * @retval         E_NOT_OK: One or more date components are invalid.
 * @note           This function checks the validity of the year, month, day, hour, minute, and second components.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71449
 */
KEYM_LOCAL Std_ReturnType KeyM_CertCheckDate(const KeyM_CertTimeType* time);

/**
 * @brief          Parses a time string from a certificate and converts it to a structured time format.
 * @param[in]      len : The length of the time string.
 * @param[in]      yearLen : The length of the year part of the time string (2 for YY, 4 for YYYY).
 * @param[in]      cerS : Pointer to the start of the time string in the certificate.
 * @param[out]     time : Pointer to the structure where the parsed time will be stored.
 * @return         Std_ReturnType
 * @retval         E_OK: The time string is successfully parsed and stored.
 * @retval         E_NOT_OK: The time string is invalid or cannot be parsed.
 * @retval         KEYM_E_KEY_CERT_INVALID: The time string contains invalid characters or format.
 * @note           This function supports both UTC time (YYMMDDHHMMSSZ) and Generalized time (YYYYMMDDHHMMSSZ).
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71450
 */
KEYM_LOCAL Std_ReturnType KeyM_CertParseTime(uint32 len, uint32 yearLen, uint8** cerS, KeyM_CertTimeType* time);

/**
 * @brief          Handles and parses the time information from a certificate.
 * @param[in]      certId : The ID of the certificate.
 * @param[in]      cerS : Pointer to the start of the certificate byte array containing the time information.
 * @param[in]      cerE : Pointer to the end of the certificate byte array.
 * @param[out]     time : Pointer to the structure where the parsed time will be stored.
 * @param[in]      eleStrut : The type of time element structure (e.g., notBefore or notAfter).
 * @return         Std_ReturnType
 * @retval         E_OK: The time information is successfully parsed and stored without any development error.
 * @retval         E_NOT_OK: The time information cannot be parsed due to invalid format, insufficient data, or other
 * errors.
 * @retval         KEYM_E_KEY_CERT_INVALID: The time tag is invalid.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71451
 */
KEYM_LOCAL Std_ReturnType KeyM_CertHandleGetTime(
    KeyM_CertificateIdType certId,
    uint8**                cerS,
    const uint8*           cerE,
    KeyM_CertTimeType*     time,
    KeyM_CertEleStructType eleStrut);

/**
 * @brief          Retrieves and processes the validity period (notBefore and notAfter times) from a certificate.
 * @param[in]      certId : The ID of the certificate.
 * @param[in]      cerS : Pointer to the start of the certificate byte array containing the validity period.
 * @param[in]      cerE : Pointer to the end of the certificate byte array.
 * @return         Std_ReturnType
 * @retval         E_OK: The validity period is successfully retrieved and processed without any development error.
 * @retval         E_NOT_OK: The validity period cannot be retrieved or processed due to invalid format, insufficient
 * data, or other errors.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71452
 */
KEYM_LOCAL Std_ReturnType KeyM_CertGetTime(KeyM_CertificateIdType certId, uint8** cerS, const uint8* cerE);

/**
 * @brief          Handles and retrieves the public key algorithm information from a certificate.
 * @param[in]      certId : The ID of the certificate.
 * @param[in]      cerS : Pointer to the start of the certificate byte array containing the public key algorithm
 * information.
 * @param[in]      cerE : Pointer to the end of the certificate byte array.
 * @return         Std_ReturnType
 * @retval         E_OK: The public key algorithm information is successfully retrieved and stored without any
 * development error.
 * @retval         E_NOT_OK: The public key algorithm information cannot be retrieved due to invalid format,
 * insufficient data, or other errors.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71453
 */
KEYM_LOCAL Std_ReturnType KeyM_CertHandleGetPubKeyAlgo(KeyM_CertificateIdType certId, uint8** cerS, const uint8* cerE);

/**
 * @brief          Retrieves and processes the public key information from a certificate.
 * @param[in]      certId : The ID of the certificate.
 * @param[in]      cerS : Pointer to the start of the certificate byte array containing the public key information.
 * @param[in]      cerE : Pointer to the end of the certificate byte array.
 * @return         Std_ReturnType
 * @retval         E_OK: The public key information is successfully retrieved and stored without any development error.
 * @retval         E_NOT_OK: The public key information cannot be retrieved due to invalid format, insufficient data, or
 * other errors.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71454
 */
KEYM_LOCAL Std_ReturnType KeyM_CertGetPubKeyInfo(KeyM_CertificateIdType certId, uint8** cerS, const uint8* cerE);

/**
 * @brief          Skips a specific field in a certificate byte array based on the given type.
 * @param[in]      cerS : Pointer to the start of the certificate byte array.
 * @param[in]      cerE : Pointer to the end of the certificate byte array.
 * @param[in]      type : The type of the field to skip (e.g., ASN.1 tag).
 * @return         None
 * @note           This function is used to skip over fields that are not needed for further processing.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71455
 */
KEYM_LOCAL void KeyM_CertSkipField(uint8** cerS, const uint8* cerE, uint8 type);

/**
 * @brief          Retrieves and processes the extension information from a certificate.
 * @param[in]      certId : The ID of the certificate.
 * @param[in]      cerS : Pointer to the start of the certificate byte array containing the extension information.
 * @param[in]      cerE : Pointer to the end of the certificate byte array.
 * @return         Std_ReturnType
 * @retval         E_OK: The extension information is successfully retrieved and stored without any development error.
 * @retval         E_NOT_OK: The extension information cannot be retrieved due to invalid format, insufficient data, or
 * other errors.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71456
 */
KEYM_LOCAL Std_ReturnType KeyM_CertGetExtention(KeyM_CertificateIdType certId, uint8** cerS, const uint8* cerE);

/**
 * @brief          Retrieves and processes the signature from a certificate.
 * @param[in]      certId : The ID of the certificate.
 * @param[in]      cerS : Pointer to the start of the certificate byte array containing the signature.
 * @param[in]      cerE : Pointer to the end of the certificate byte array.
 * @return         Std_ReturnType
 * @retval         E_OK: The signature is successfully retrieved and stored without any development error.
 * @retval         E_NOT_OK: The signature cannot be retrieved due to invalid format, insufficient data, or other
 * errors.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71457
 */
KEYM_LOCAL Std_ReturnType KeyM_CertGetSignature(KeyM_CertificateIdType certId, uint8** cerS, const uint8* cerE);

/**
 * @brief          Retrieves a specific certificate element based on the certificate ID and element structure type.
 * @param[in]      certId : The ID of the certificate.
 * @param[in]      certEleStru : The type of the certificate element to retrieve (e.g., issuer name, subject name,
 * etc.).
 * @return         Pointer to the certificate element data structure.
 * @retval         Non-NULL: Pointer to the certificate element data structure.
 * @retval         NULL_PTR: The specified certificate element does not exist.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71458
 */
/* PRQA S 5016 ++ */ /* VL_KeyM_5016 */
KEYM_LOCAL KeyM_Asn1DesType* KeyM_GetCertEle(KeyM_CertificateIdType certId, KeyM_CertEleStructType certEleStru);
/* PRQA S 5016 -- */

/**
 * @brief          Compares two certificate elements for equality.
 * @param[in]      certEleBuf : Pointer to the first certificate element data structure.
 * @param[in]      upCertEleBuf : Pointer to the second certificate element data structure.
 * @return         Std_ReturnType
 * @retval         E_OK: The two certificate elements are equal.
 * @retval         E_NOT_OK: The two certificate elements are not equal or one of the pointers is NULL.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71459
 */
KEYM_LOCAL Std_ReturnType KeyM_CmpCertEle(KeyM_Asn1DesType* certEleBuf, KeyM_Asn1DesType* upCertEleBuf);

/**
 * @brief          Checks if the current time is within the validity period of a certificate.
 * @param[in]      certId : The ID of the certificate to check.
 * @return         Std_ReturnType
 * @retval         E_OK: The current time is within the validity period of the certificate.
 * @retval         E_NOT_OK: The current time is outside the validity period of the certificate or the certificate ID is
 * invalid.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71460
 */
KEYM_LOCAL Std_ReturnType KeyM_GlobalTimeCheck(KeyM_CertificateIdType certId);

/**
 * @brief          Determines whether a given year is a leap year.
 * @param[in]      year : The year to check.
 * @return         uint8
 * @retval         1: The year is a leap year.
 * @retval         0: The year is not a leap year.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71461
 */
KEYM_LOCAL uint8 KeyM_IsLeapYear(uint16 year);

/**
 * @brief          Converts a Beijing time to a Unix timestamp.
 * @param[in]      bjTime : Pointer to the Beijing time structure to convert.
 * @return         uint32
 * @retval         The Unix timestamp corresponding to the Beijing time.
 * @note           This function assumes the input time is in Beijing time and converts it to a Unix timestamp (seconds
 * since January 1, 1970, 00:00:00 UTC).
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71462
 */
KEYM_LOCAL uint32 KeyM_ConvertTime_BeijingToUnix(const KeyM_CertTimeType* bjTime);

/* ============================================ internal data definitions =========================================== */
/* PRQA S 3218 ++*/ /* VL_KeyM_3218 */
#if (KEYM_NVM_BLOCK_NUM > 0)
/**
 * @brief          Array to store the block write delay run status for each NVM block.
 * @details        This array is used to manage the write operations to NVM blocks, ensuring that writes are performed
 * with the required delays.
 * @range         KEYM_NVM_BLOCK_NUM
 */
KEYM_LOCAL KeyM_BlkWriType KeyM_BlkWriDelayRun[KEYM_NVM_BLOCK_NUM];
#endif
/* PRQA S 3218 --*/
#define KEYM_START_SEC_CONST_UNSPECIFIED
#include "KeyM_MemMap.h"
/* PRQA S 3218,3132 ++*/ /* VL_KeyM_3218,VL_QAC_MagicNum */

/**
 * @brief          Number of days in each month for a non-leap year.
 * @details        This array contains the number of days in each month for a non-leap year, used for date calculations.
 * @range         12
 */
KEYM_LOCAL const uint8 KeyM_ComMonthDay[12] = {31u, 28u, 31u, 30u, 31u, 30u, 31u, 31u, 30u, 31u, 30u, 31u};

/**
 * @brief          Array representing the number of days in each month for a leap year.
 * @details        This array contains the number of days in each month for a leap year, used for date calculations.
 * @range         12
 */
KEYM_LOCAL const uint8 KeyM_LeapMonthDay[12] = {31u, 29u, 31u, 30u, 31u, 30u, 31u, 31u, 30u, 31u, 30u, 31u};
/* PRQA S 3218,3132 --*/
#define KEYM_STOP_SEC_CONST_UNSPECIFIED
#include "KeyM_MemMap.h"

#define KEYM_START_SEC_VAR_CLEARED_8
#include "KeyM_MemMap.h"

/**
 * @brief          Array to store the status of each certificate.
 * @details        This array contains the status of each certificate, indicating whether it is valid, expired, or has
 * other states.
 * @range         KEYM_CERT_NUM
 */
KEYM_LOCAL KeyM_CertificateStatusType KeyM_CertStatus[KEYM_CERT_NUM];
#define KEYM_STOP_SEC_VAR_CLEARED_8
#include "KeyM_MemMap.h"

/* ============================================ external data definitions =========================================== */
#define KEYM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "KeyM_MemMap.h"
/**
 * @brief          Array to store the start time of each certificate.
 * @details        This array contains the start times for each certificate, indicating when the certificate becomes
 * valid.
 * @range         KEYM_CERT_NUM
 */
/* PRQA S 1504 ++ */ /* VL_QAC_OneRefSymbol */
KeyM_CertTimeType KeyM_CertStartTime[KEYM_CERT_NUM];
/* PRQA S 1504 -- */

/**
 * @brief          Array to store the end time of each certificate.
 * @details        This array contains the end times for each certificate, indicating when the certificate expires.
 * @range         KEYM_CERT_NUM
 */
/* PRQA S 1504 ++ */ /* VL_QAC_OneRefSymbol */
KeyM_CertTimeType KeyM_CertEndTime[KEYM_CERT_NUM];
/* PRQA S 1504 -- */

/**
 * @brief          Array to store the version of each certificate.
 * @details        This array contains the version numbers for each certificate, which can be used to identify different
 * versions of the same certificate.
 * @range         KEYM_CERT_NUM
 */
/* PRQA S 1504 ++ */ /* VL_QAC_OneRefSymbol */
uint32 KeyM_CertVersion[KEYM_CERT_NUM];
/* PRQA S 1504 -- */
#define KEYM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "KeyM_MemMap.h"

/* ========================================== external function definitions ========================================= */
#define KEYM_START_SEC_CODE
#include "KeyM_MemMap.h"
#if (KEYM_SERVICE_CERTIFICATE_FUNCTION_ENABLED == STD_ON)
/*************************************************************************/
/*
 * @Brief  The key server requests an operation from the key client.
 */
/*************************************************************************/
Std_ReturnType KeyM_ServiceCertificate(
    KeyM_ServiceCertificateType Service,
    const uint8*                CertNamePtr,
    uint32                      CertNameLength,
    const uint8*                RequestData,
    uint32                      RequestDataLength,
    uint8*                      ResponseData,
    uint32*                     ResponseDataLength)
{
    const KeyM_CertPCfgType* certCfgPtr;
    const KeyM_CertPCfgType* certUpperHierRef;
    uint32                   jobId;
    uint16                   certIdx;
    Std_ReturnType           ret = E_OK;

    if (!KeyM_InitStatus)
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
                RequestDataLength,
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
                        ret   = Csm_KeyGenerate(KeyM_CryptoKeyCfg[*(certCfgPtr->certPriKeyRef)].keyId);
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
                            if (certUpperHierRef == NULL_PTR)
                            {
                                ret = KEYM_E_KEY_CERT_READ_FAIL;
                                break;
                            }
                        } while (certUpperHierRef->selfSignedCert != TRUE);
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
Std_ReturnType KeyM_ServiceCertificateByCertId(
    KeyM_CertificateIdType      CertId,
    KeyM_ServiceCertificateType Service,
    const uint8*                RequestData,
    uint32                      RequestDataLength,
    uint8*                      ResponseData,
    uint32*                     ResponseDataLength)
{
    const KeyM_CertPCfgType* certCfgPtr;
    const KeyM_CertPCfgType* certUpperHierRef;
    uint32                   jobId;
    Std_ReturnType           ret = E_OK;

    if (!KeyM_InitStatus)
    {
#if (KEYM_DEVERROR_DETECT == STD_ON)
        KEYM_DET_REPORT(KEYM_SERV_CERTIFICATE_ID, KEYM_E_UNINIT);
#endif
        ret = E_NOT_OK;
    }
    else if ((RequestData == NULL_PTR) || (ResponseData == NULL_PTR))
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
            ret = KeyM_KH_ServiceCertificateByCertId(
                CertId,
                Service,
                RequestData,
                RequestDataLength,
                ResponseData,
                ResponseDataLength);
#else

            if (CertId >= KEYM_CERT_NUM)
            {
                ret = KEYM_E_KEY_CERT_SIZE_MISMATCH;
            }
            else
            {
                certCfgPtr = &KeyM_CertPCfg[CertId];
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
                        ret   = Csm_KeyGenerate(KeyM_CryptoKeyCfg[*(certCfgPtr->certPriKeyRef)].keyId);
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
                        ret = KeyM_HandleParseCert(CertId, RequestData, (uint32)RequestDataLength);
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
                            if (certUpperHierRef == NULL_PTR)
                            {
                                ret = KEYM_E_KEY_CERT_READ_FAIL;
                                break;
                            }
                        } while (certUpperHierRef->selfSignedCert != TRUE);
                        if (E_OK == ret)
                        {
                            ret = KeyM_HandleParseCert(CertId, RequestData, (uint32)RequestDataLength);
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
/*************************************************************************/
/**
 * @Brief  This function provides the certificate data to the key management module to
 */
/*************************************************************************/
Std_ReturnType KeyM_SetCertificate(KeyM_CertificateIdType CertId, const KeyM_CertDataType* CertificateDataPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == KEYM_DEVERROR_DETECT)
    if (!KeyM_InitStatus)
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
        /* PRQA S 1252 ++ */ /* VL_KeyM_1252 */
        if (CertificateDataPtr->certDataLength == 0u)
        /* PRQA S 1252 -- */
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
                /* PRQA S 0317 ++ */ /* VL_QAC_0317 */
                ret = KeyM_HandleParseCert(CertId, CertificateDataPtr->certData, CertificateDataPtr->certDataLength);
                /* PRQA S 0317 -- */
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
 * @Brief  This function provides the certificate data
 */
/*************************************************************************/
Std_ReturnType KeyM_GetCertificate(KeyM_CertificateIdType CertId, KeyM_CertDataType* CertificateDataPtr)
{
    const KeyM_CertPCfgType* certCfgPtr;
    /* PRQA S 2981 ++*/ /* VL_KeyM_2981 */
    Std_ReturnType ret = E_NOT_OK;
    /* PRQA S 2981 --*/
#if (STD_ON == KEYM_DEVERROR_DETECT)
    if (!KeyM_InitStatus)
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
        /* PRQA S 1252 ++ */ /* VL_KeyM_1252 */
        else if (certCfgPtr->tbsCert->len == 0u)
        /* PRQA S 1252 -- */
        {
            ret = KEYM_E_KEY_CERT_EMPTY;
        }
        else
        {
            /* PRQA S 0311 ++*/ /* VL_KeyM_0311 */
            KeyM_CopyData((void*)CertificateDataPtr->certData, certCfgPtr->tbsCert->dataEle, certCfgPtr->tbsCert->len);
            /* PRQA S 0311 --*/
            CertificateDataPtr->certDataLength = certCfgPtr->tbsCert->len;
            ret                                = E_OK;
        }
    }

    return ret;
}

/*************************************************************************/
/*
 * @Brief  This function provides the certificate data to the key management module to
 */
/*************************************************************************/
Std_ReturnType KeyM_VerifyCertificates(KeyM_CertificateIdType CertId, KeyM_CertificateIdType CertUpperId)
{
    Std_ReturnType ret;

    if (!KeyM_InitStatus) /* PRQA S 2109 */ /* VL_KeyM_2109 */
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
    /* PRQA S 2991,2995 ++*/ /* VL_KeyM_2991,VL_KeyM_2995 */
    else if (KeyM_CertStatus[CertId] != KEYM_CERTIFICATE_PARSED_NOT_VALIDATED)
    /* PRQA S 2991,2995 --*/
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
 * @Brief  This function verifies a certificate that was previously provided with
 */
/*************************************************************************/
/* PRQA S 6030 ++ */ /* VL_MTR_KeyM_STMIF */
Std_ReturnType KeyM_VerifyCertificate(KeyM_CertificateIdType CertId)
/* PRQA S 6030 -- */
{
    const KeyM_CertPCfgType* certCfgPtr;
    const KeyM_CertPCfgType* certUpperHierRef;
    Std_ReturnType           ret;

    if (!KeyM_InitStatus) /* PRQA S 2109 */ /* VL_KeyM_2109 */
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
            ret              = E_OK;
            /*Verification shall be done from the top of the certificate hierarchy to the bottom.*/
            if (certUpperHierRef->selfSignedCert) /* PRQA S 2109 */ /* VL_KeyM_2109 */
            {
                /*do nothing*/
            }
            else
            {
                do /* PRQA S 0771 */ /* VL_KeyM_0771 */
                {
                    certUpperHierRef = certUpperHierRef->certUpperHierRef;
                    if ((certUpperHierRef != NULL_PTR)
                        && (KeyM_CertStatus[certUpperHierRef->certId] != KEYM_CERTIFICATE_VALID))
                    {
                        ret = KEYM_E_CERT_INVALID_CHAIN_OF_TRUST;
                        break;
                    }
                    if (certUpperHierRef == NULL_PTR)
                    {
                        ret = KEYM_E_KEY_CERT_READ_FAIL;
                        break;
                    }
                } while (!certUpperHierRef->selfSignedCert); /* PRQA S 2109 */ /* VL_KeyM_2109 */
            }
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * @Brief  This function performs a certificate verification against a list of
 */
/*************************************************************************/
Std_ReturnType KeyM_VerifyCertificateChain(
    KeyM_CertificateIdType  CertId,
    const KeyM_CertDataType certChainData[],
    uint8                   NumberOfCertificates)
{
    Std_ReturnType ret = E_NOT_OK;
    uint8          certCnt;

    if (!KeyM_InitStatus) /* PRQA S 2109 */ /* VL_KeyM_2109 */
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
            const KeyM_CertDataType* nextCertData    = &(certChainData[certCnt + 1u]);
            (void)currentCertData;
            (void)nextCertData;
        }
    }

    return ret;
}

/*************************************************************************/
/*
 * @Brief Provides the content of a specific certificate element.
 */
/*************************************************************************/
Std_ReturnType KeyM_CertElementGet(
    KeyM_CertificateIdType CertId,
    KeyM_CertElementIdType CertElementId,
    uint8*                 CertElementData,
    uint32*                CertElementDataLength)
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
 * @Brief This function provides the element data of a certificate.
 */
/*************************************************************************/
Std_ReturnType KeyM_CertificateElementGetByIndex(
    KeyM_CertificateIdType CertId,
    KeyM_CertElementIdType CertElementId,
    uint32                 Index,
    uint8*                 CertElementDataPtr,
    uint32*                CertElementDataLengthPtr)
{
    const KeyM_CertEleType* certElePCfgPtr;
    /* PRQA S 3678 ++ */ /* VL_KeyM_3678 */
    KeyM_Asn1DesType* certEleBufPtr;
    /* PRQA S 3678 -- */
    uint32         idx;
    uint32         certElenum;
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
        certElenum     = Index + 1u; /* PRQA S 1252 */ /* VL_KeyM_1252 */
        certElePCfgPtr = &KeyM_CertPCfg[CertId].certEleRef[CertElementId];
        for (idx = 0u; idx < certElenum; idx++)
        {
            if (idx == 0u) /* PRQA S 1252 */ /* VL_KeyM_1252 */
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
            /* PRQA S 2963 ++*/ /* VL_KeyM_2963 */
            KeyM_CopyData(CertElementDataPtr, certEleBufPtr->dataEle, certEleBufPtr->len);
            /* PRQA S 2963 --*/
            *CertElementDataLengthPtr = certEleBufPtr->len;
        }
    }

    return ret;
}

/*************************************************************************/
/*
 * @Brief  This function provides the total number of data
 */
/*************************************************************************/
Std_ReturnType KeyM_CertificateElementGetCount(
    KeyM_CertificateIdType CertId,
    KeyM_CertElementIdType CertElementId,
    uint16*                CountPtr)
{
    /* PRQA S 3678 ++ */ /* VL_KeyM_3678 */
    KeyM_Asn1DesType* certEleBufPtr;
    /* PRQA S 3678 -- */
    uint16 cnt = 0u;
    /* PRQA S 2981 ++ */ /* VL_KeyM_2981 */
    Std_ReturnType ret = E_NOT_OK;
    /* PRQA S 2981 -- */
#if (STD_ON == KEYM_DEVERROR_DETECT)
    if (!KeyM_InitStatus)
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
            ret       = E_OK;
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * @Brief  This function is used to initialize the interative extraction of a
 */
/*************************************************************************/
Std_ReturnType KeyM_CertElementGetFirst(
    KeyM_CertificateIdType        CertId,
    KeyM_CertElementIdType        CertElementId,
    KeyM_CertElementIteratorType* CertElementIterator,
    uint8*                        CertElementData,
    uint32*                       CertElementDataLength)
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
            CertElementIterator->certId     = CertId;
            CertElementIterator->certEleId  = CertElementId;
            CertElementIterator->eleItrator = KeyM_CertPCfg[CertId].certEleRef[CertElementId].certEleBuf;

            KeyM_CopyData(
                CertElementData,
                KeyM_CertPCfg[CertId].certEleRef[CertElementId].certEleBuf->dataEle,
                KeyM_CertPCfg[CertId].certEleRef[CertElementId].certEleBuf->len);
            *CertElementDataLength          = KeyM_CertPCfg[CertId].certEleRef[CertElementId].certEleBuf->len;
            CertElementIterator->eleItrator = CertElementIterator->eleItrator->next;
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * @Brief  This function provides further data from a certificate element.
 */
/*************************************************************************/
Std_ReturnType KeyM_CertElementGetNext(
    KeyM_CertElementIteratorType* CertElementIterator,
    uint8*                        CertElementData,
    uint32*                       CertElementDataLength)
{
    const KeyM_CertEleType* certElePCfgPtr;
    Std_ReturnType          ret = E_NOT_OK;
#if (STD_ON == KEYM_DEVERROR_DETECT)
    if (!KeyM_InitStatus)
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
                /* PRQA S 1252,2109 ++ */ /* VL_KeyM_1252,VL_KeyM_2109 */
                if (!certElePCfgPtr->certEleHasIteration)
                /* PRQA S 1252,2109 -- */
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
 * @Brief  This function provides the status of a certificate.
 */
/*************************************************************************/
/* PRQA S 1532 ++ */ /* VL_KeyM_1532 */
Std_ReturnType KeyM_CertGetStatus(
    /* PRQA S 1532 -- */
    KeyM_CertificateIdType      CertId,
    KeyM_CertificateStatusType* Status)
{
    /* PRQA S 2981 ++ */ /* VL_KeyM_2981 */
    Std_ReturnType ret = E_NOT_OK;
    /* PRQA S 2981 -- */
#if (STD_ON == KEYM_DEVERROR_DETECT)
    if (!KeyM_InitStatus)
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
            ret     = E_OK;
        }
    }
    return ret;
}

Std_ReturnType KeyM_CertSetStatus(KeyM_CertificateIdType CertId, KeyM_CertificateStatusType Status)
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

/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void KeyM_CertStoreNvmHandle(void)
/* PRQA S 1532 -- */
{
#if (KEYM_NVM_BLOCK_NUM > 0)
    uint16 nvmBlkIdx;
#if (KEYM_NVM_BLOCK_NUM > 1)
    for (nvmBlkIdx = 0u; nvmBlkIdx < KEYM_NVM_BLOCK_NUM; nvmBlkIdx++)
#endif
    {
        /* PRQA S 2880,2961 ++ */ /* VL_KeyM_2880,VL_KeyM_2961 */
        if (KeyM_BlkWriDelayRun[nvmBlkIdx].timeStart == TRUE)
        /* PRQA S 2880,2961 -- */
        {
            KeyM_BlkWriDelayRun[nvmBlkIdx].delayTime--;
            if (KeyM_BlkWriDelayRun[nvmBlkIdx].delayTime == 0u)
            {
                (void)NvM_WriteBlock(
                    KeyM_NvmBlockPCfg[nvmBlkIdx].blkId,
                    KeyM_BlkWriDelayRun[nvmBlkIdx].nvmCertEle->dataEle);
                KeyM_BlkWriDelayRun[nvmBlkIdx].timeStart  = FALSE;
                KeyM_BlkWriDelayRun[nvmBlkIdx].nvmCertEle = NULL_PTR;
            }
        }
    }
#endif
}

/* PRQA S 6080,6070,6010 ++ */ /* VL_MTR_KeyM_STPTH,VL_MTR_KeyM_STCAL,VL_MTR_KeyM_STCYC */
/* PRQA S 6050,6020,5017 ++ */ /* VL_MTR_KeyM_STST3,VL_MTR_KeyM_STLIN,VL_KeyM_5017 */
Std_ReturnType KeyM_HandleParseCert(KeyM_CertificateIdType CertId, const uint8* certDataPtr, uint32 certDataLength)
/* PRQA S 6050,6020,5017 -- */
/* PRQA S 6080,6070,6010 -- */
{
    const KeyM_CertPCfgType* certCfgPtr;
    /* PRQA S 3678 ++ */ /* VL_KeyM_3678 */
    KeyM_Asn1DesType* pbKeyBuf;
    /* PRQA S 3678 -- */
    uint8* certStartPtr;
    /* PRQA S 3678 ++ */ /* VL_KeyM_3678 */
    uint8* tbsPtr;
    uint8* certendPtr;
    uint8* certTotalendPtr;
    /* PRQA S 3678 -- */
    uint32           certEleLen;
    KeyM_Asn1DesType sifParam;
    Std_ReturnType   ret;

    certCfgPtr   = &KeyM_CertPCfg[CertId];
    certStartPtr = (uint8*)certDataPtr; /* PRQA S 0311 */ /* VL_KeyM_0311 */
    certEleLen   = certDataLength;
    certendPtr   = certStartPtr + certEleLen;
    /*
     * Certificate  ::=  SEQUENCE  {
     *      tbsCertificate       TBSCertificate,
     *      signatureAlgorithm   AlgorithmIdentifier,
     *      signatureValue       BIT STRING  }
     */
    ret = KeyM_CertGetAsn1Tag(&certStartPtr, certendPtr, &certEleLen, KEYM_CERT_ASN1_SEQ_CONSTRU);
    /* PRQA S 2001 ++ */ /* VL_KeyM_2001 */
    if (ret != E_OK)
    {
        goto PARSE_CERT_EXIT;
    }
    certendPtr      = certStartPtr + certEleLen;
    certTotalendPtr = certendPtr;
    /*
     * TBSCertificate  ::=  SEQUENCE  {
     */
    tbsPtr = certStartPtr;
    ret    = KeyM_CertGetAsn1Tag(&certStartPtr, certendPtr, &certEleLen, KEYM_CERT_ASN1_SEQ_CONSTRU);
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
        if (ret != (Std_ReturnType)E_OK)
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
    if (ret != (Std_ReturnType)E_OK)
    {
        goto PARSE_CERT_EXIT;
    }
    /*
     * Issuer Name
     */
    ret = KeyM_CertGetAsn1Tag(&certStartPtr, certendPtr, &certEleLen, KEYM_CERT_ASN1_SEQ_CONSTRU);
    if (ret != (Std_ReturnType)E_OK)
    {
        goto PARSE_CERT_EXIT;
    }
    ret = KeyM_CertGetName(
        CertId,
        &certStartPtr,
        certStartPtr + certEleLen,
        CERTIFICATE_ISSUER_NAME,
        CERTIFICATE_ISSUER_UNIQUE_IDENTIFIER);
    if (ret != (Std_ReturnType)E_OK)
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
    pbKeyBuf = KeyM_GetCertEle(CertId, CERTIFICATE_SUBJECT_PUBLIC_KEY_INFO_SUBJECT_PUBLIC_KEY);
    /* PRQA S 1258 ++ */ /* VL_KeyM_1258 */
    ret = Csm_KeyElementSet(
        certCfgPtr->certSigVeriKey,
        CRYPTO_KE_SIGNATURE_KEY,
        &pbKeyBuf->dataEle[1],
        pbKeyBuf->len - 1u);
    /* PRQA S 1258 -- */
    if (ret != E_OK)
    {
        goto PARSE_CERT_EXIT;
    }
    (void)Csm_KeySetValid(certCfgPtr->certSigVeriKey);
    /*
     *  issuerUniqueID  [1]  IMPLICIT UniqueIdentifier OPTIONAL,
     *                       -- If present, version shall be v2 or v3
     *  subjectUniqueID [2]  IMPLICIT UniqueIdentifier OPTIONAL,
     *                       -- If present, version shall be v2 or v3
     */
    if ((KeyM_CertVersion[CertId] == KEYM_CONST_1) || (KeyM_CertVersion[CertId] == KEYM_CONST_2))
    {
        /*Skip issuerUniqueID & subjectUniqueID*/
        KeyM_CertSkipField(&certStartPtr, certendPtr, 0xa1u); /* PRQA S 3120 */ /* VL_QAC_MagicNum */
        KeyM_CertSkipField(&certStartPtr, certendPtr, 0xa1u); /* PRQA S 3120 */ /* VL_QAC_MagicNum */
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
            /*only tbs*/
            KeyM_CopyData(certCfgPtr->tbsCert->dataEle, &certDataPtr[KEYM_CONST_4], certCfgPtr->tbsCert->len);
        }
    }
PARSE_CERT_EXIT: /* PRQA S 2015 */ /* VL_KeyM_2015 */
    /* PRQA S 2001 -- */
    if (ret == E_OK) /* PRQA S 1881 */ /* VL_KeyM_1881 */
    {
        (void)KeyM_CertSetStatus(CertId, KEYM_CERTIFICATE_PARSED_NOT_VALIDATED);
    }
    else if (ret == KEYM_E_CERTIFICATE_INVALID_TYPE) /* PRQA S 1881 */ /* VL_KeyM_1881 */
    {
        (void)KeyM_CertSetStatus(CertId, KEYM_E_CERTIFICATE_INVALID_TYPE);
    }
    else
    {
        (void)KeyM_CertSetStatus(CertId, KEYM_E_CERTIFICATE_INVALID_FORMAT);
    }
    return ret;
}

/*keySet = TRUE:Storrage KeyElement.
 * keySet = FALSE:Get KeyElement.
 * */
/* PRQA S 6010,5017 ++ */ /* VL_MTR_KeyM_STCYC,VL_KeyM_5017 */
void KeyM_HandleCsmKeyStorage(uint32 keyId, uint16 certId, boolean keySet)
/* PRQA S 6010,5017 -- */
{
    const KeyM_CertPCfgType* certCfgPtr;
    const KeyM_CertEleType*  certEleRef;
    uint32                   keyElementId;
    uint16                   certEle;

    certCfgPtr = &KeyM_CertPCfg[certId];
    for (certEle = 0u; certEle < certCfgPtr->numOfCertEle; certEle++)
    {
        certEleRef = &certCfgPtr->certEleRef[certEle];
        switch (certEleRef->certEleStruct)
        {
        /* PRQA S 1258 ++ */ /* VL_KeyM_1258 */
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
        /* PRQA S 1258 -- */
        if (keyElementId != KEYM_INVALID_U32) /* PRQA S 1252 */ /* VL_KeyM_1252 */
        {
            if (keySet) /* PRQA S 2109 */ /* VL_KeyM_2109 */
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

/* ========================================== internal function definitions ========================================= */
/* PRQA S 6030,3206 ++ */ /* VL_MTR_KeyM_STMIF,VL_KeyM_3206 */
KEYM_LOCAL Std_ReturnType KeyM_CheckGeneralParam(
    KeyM_CertificateIdType CertId,
    KeyM_CertElementIdType CertElementId,
    const uint8*           CertElementData,
    const uint32*          CertElementDataLength,
    uint8                  ApiId)
/* PRQA S 6030,3206 -- */
{
    Std_ReturnType ret;

    if (!KeyM_InitStatus) /* PRQA S 2109 */
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
#if (STD_ON == KEYM_DEVERROR_DETECT)
            KEYM_DET_REPORT(ApiId, KEYM_E_SMALL_BUFFER);
#endif /*STD_ON == KEYM_DEVERROR_DETECT*/
        }
        else if (KeyM_CertStatus[CertId] != KEYM_CERTIFICATE_VALID)
        {
            ret = KEYM_E_KEY_CERT_INVALID;
        }
        /* PRQA S 1252 ++ */ /* VL_KeyM_1252 */
        else if (KeyM_CertPCfg[CertId].certEleRef[CertElementId].certEleBuf->len == 0U)
        /* PRQA S 1252 -- */
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

KEYM_LOCAL Std_ReturnType KeyM_IsCertExist(KeyM_CertificateIdType certId, KeyM_CertEleStructType certStr, uint16* cfgId)
{
    const KeyM_CertEleType* certEleCfgPtr;
    uint16                  idx;
    Std_ReturnType          ret = E_NOT_OK;

    for (idx = 0u; idx < KeyM_CertPCfg[certId].numOfCertEle; idx++)
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

/* PRQA S 6030,6010,5017 ++ */ /* VL_MTR_KeyM_STMIF,VL_MTR_KeyM_STCYC,VL_KeyM_5017  */
KEYM_LOCAL Std_ReturnType
    KeyM_FindCertElement(KeyM_CertificateIdType certId, const KeyM_CertEleType* certEle, uint16* eleIdx)
/* PRQA S 6030,6010,5017 -- */
{
    const KeyM_CertEleType* certEleCfgPtr;
    uint16                  idx;
    uint16                  len;
    Std_ReturnType          ret      = E_NOT_OK;
    boolean                 oidCheck = FALSE;

    for (idx = 0u; idx < KeyM_CertPCfg[certId].numOfCertEle; idx++)
    {
        certEleCfgPtr = &KeyM_CertPCfg[certId].certEleRef[idx];
        if ((certEleCfgPtr->certEleMaxLen < certEle->certEleMaxLen)
            || (certEleCfgPtr->certEleStruct != certEle->certEleStruct))
        {
            /* PRQA S 3397 ++ */ /* VL_KeyM_3397 */
            if (idx == KeyM_CertPCfg[certId].numOfCertEle - 1u)
            /* PRQA S 3397 -- */
            {
                /*SWS_KeyM_00168*/
                ret = (Std_ReturnType)KEYM_E_CERTIFICATE_INVALID_TYPE;
            }
            continue; /* PRQA S 0770 */ /* VL_KeyM_0770 */
        }
        else
        {
            /*Check KeyMCertificateElementObjectType*/
            if ((certEleCfgPtr->certEleObjType != NULL_PTR) && (certEle->certEleObjType != NULL_PTR))
            {
                if (((*certEleCfgPtr->certEleObjType) & (*certEle->certEleObjType)) == 0u)
                {
                    continue; /* PRQA S 0770 */ /* VL_KeyM_0770 */
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

            if (oidCheck) /* PRQA S 2109 */ /* VL_KeyM_2109 */
            {
                *eleIdx = idx;
                ret     = E_OK;
                break;
            }
        }
    }
    return ret;
}

KEYM_LOCAL Std_ReturnType
    KeyM_CertEleHandle(KeyM_CertificateIdType certId, const KeyM_CertEleType* certEle, const uint8* eleData, uint32 tag)
{
    uint16         eleCfgIndex;
    uint16         eleIdx;
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

KEYM_LOCAL Std_ReturnType KeyM_CertGetAsn1Tag(uint8** cerS, const uint8* cerE, uint32* cerLenPtr, uint8 tag)
{
    Std_ReturnType ret;

    /* PRQA S 1822,1852 ++ */ /* VL_KeyM_1822,VL_KeyM_1852 */
    if (((cerE - *cerS) < KEYM_CONST_1) || (**cerS != tag))
    /* PRQA S 1822,1852 -- */
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

/* PRQA S 6010,5017 ++ */ /* VL_MTR_KeyM_STCYC,VL_KeyM_5017 */
KEYM_LOCAL Std_ReturnType KeyM_CertGetAsn1Len(uint8** cerS, const uint8* cerE, uint32* cerLen)
/* PRQA S 6010,5017 -- */
{
    Std_ReturnType ret = E_OK;

    if ((**cerS & KEYM_LONG_FORMAT_MASK) == 0u)
    {
        *cerLen = *(*cerS)++; /* PRQA S 3440,3387 */ /* VL_KeyM_3440,VL_KeyM_3387 */
    }
    else /*length > 127*/
    {
        switch (**cerS & (KEYM_LONG_FORMAT_MASK - KEYM_CONST_1))
        {
        case KEYM_CONST_1:
            /* PRQA S 1822,1852 ++ */ /* VL_KeyM_1822,VL_KeyM_1852 */
            if ((cerE - *cerS) < KEYM_CONST_2)
            /* PRQA S 1822,1852 */
            {
                ret = E_NOT_OK;
            }
            else
            {
                *cerLen = (*cerS)[1];
                (*cerS) += 2; /* PRQA S 3120 */
            }
            break;
        case KEYM_CONST_2:
            /* PRQA S 1822,1852 ++ */ /* VL_KeyM_1822,VL_KeyM_1852 */
            if ((cerE - *cerS) < KEYM_CONST_3)
            /* PRQA S 1822,1852 */
            {
                ret = E_NOT_OK;
            }
            else
            {
                /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
                *cerLen = ((uint32)(*cerS)[1u] << 8u) | (*cerS)[2u];
                (*cerS) += 3u;
                /* PRQA S 3120 -- */
            }
            break;
        case KEYM_CONST_3:
            /* PRQA S 1822,1852 ++ */ /* VL_KeyM_1822,VL_KeyM_1852 */
            if ((cerE - *cerS) < KEYM_CONST_4)
            /* PRQA S 1822,1852 */
            {
                ret = E_NOT_OK;
            }
            else
            {
                /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
                *cerLen = ((uint32)(*cerS)[1u] << 16u) | ((uint32)(*cerS)[2u] << 8u) | (*cerS)[3u];
                (*cerS) += 4u;
                /* PRQA S 3120 -- */
            }
            break;
        case KEYM_CONST_4:
            /* PRQA S 1822,1852 ++ */ /* VL_KeyM_1822,VL_KeyM_1852 */
            if ((cerE - *cerS) < KEYM_CONST_5)
            /* PRQA S 1822,1852 */
            {
                ret = E_NOT_OK;
            }
            else
            {
                /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
                *cerLen = ((uint32)(*cerS)[1u] << 24u) | ((uint32)(*cerS)[2u] << 16u) | ((uint32)(*cerS)[3u] << 8u)
                          | (*cerS)[4u];
                (*cerS) += 5u;
                /* PRQA S 3120 -- */
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

KEYM_LOCAL Std_ReturnType KeyM_CertGetAsn1TagInt(uint8** cerS, const uint8* cerE, uint8* intVal, uint32* cerLen)
{
    uint32         len;
    Std_ReturnType ret;

    *intVal = 0u;
    ret     = KeyM_CertGetAsn1Tag(cerS, cerE, &len, KEYM_CERT_ASN1_INTEGER);
    /* PRQA S 3120,1252 ++ */ /* VL_QAC_MagicNum,VL_KeyM_1252 */
    if ((ret == (Std_ReturnType)E_OK) && (len > 0u) && (len <= 4u) && ((**cerS & KEYM_LONG_FORMAT_MASK) == 0u))
    /* PRQA S 3120,1252 -- */
    {
        *cerLen = len;
        /*Skip leading zeros.*/
        while ((len > 0u) && (**cerS == 0u)) /* PRQA S 1252 */ /* VL_KeyM_1252 */
        {
            ++(*cerS);
            --len;
        }
        /* PRQA S 3440,1252,2911 ++ */ /* VL_KeyM_3440,VL_KeyM_1252,VL_KeyM_2911 */
        while (len-- > 0u)
        {
            *intVal = **cerS;
            (*cerS)++;
        }
        /* PRQA S 3440,1252,2911 -- */
    }
    else
    {
        ret = E_NOT_OK;
    }
    return ret;
}

KEYM_LOCAL Std_ReturnType KeyM_CertGetVersion(KeyM_CertificateIdType certId, uint8** cerS, const uint8* cerE)
{
    uint32         cerLen;
    uint16         eleIdx;
    Std_ReturnType ret;
    uint8          ver;
    const uint8*   temp_EndAddr = cerE;

    ret = KeyM_CertGetAsn1Tag(cerS, temp_EndAddr, &cerLen, KEYM_CERT_ASN1_TAG);
    if (ret == (Std_ReturnType)E_OK)
    {
        temp_EndAddr = *cerS + cerLen;
        ret          = KeyM_CertGetAsn1TagInt(cerS, temp_EndAddr, &ver, &cerLen);
        if ((ret != E_OK) || (*cerS != temp_EndAddr))
        {
            ret = E_NOT_OK;
        }
        else
        {
            ret = KeyM_IsCertExist(certId, CERTIFICATE_VERSION_NUMBER, &eleIdx);
            if (E_OK == ret)
            {
                KeyM_CertVersion[certId]                                        = ver;
                KeyM_CertPCfg[certId].certEleRef[eleIdx].certEleBuf->dataEle[0] = ver;
            }
        }
    }
    return ret;
}

KEYM_LOCAL Std_ReturnType KeyM_CertGetSerialVersion(KeyM_CertificateIdType certId, uint8** cerS, const uint8* cerE)
{
    uint32           tag;
    uint32           cerLen;
    KeyM_CertEleType certEle;
    uint8            objType = KEYM_CERT_ASN1_INTEGER;
    Std_ReturnType   ret;
    /* PRQA S 1822,1852 ++ */ /* VL_KeyM_1822,VL_KeyM_1852 */
    if (((cerE - *cerS) < (uint8)KEYM_CONST_1) || (**cerS != KEYM_CERT_ASN1_INTEGER))
    /* PRQA S 1822,1852 -- */
    {
        ret = E_NOT_OK;
    }
    else
    {
        tag = *(*cerS)++; /* PRQA S 3440,3387 */ /* VL_KeyMN_3440,VL_KeyM_3387 */
        ret = KeyM_CertGetAsn1Len(cerS, cerE, &cerLen);
        if (ret == (Std_ReturnType)E_OK)
        {
            certEle.certEleMaxLen  = (uint16)cerLen;
            certEle.certEleObjType = &objType;
            certEle.certEleStruct  = CERTIFICATE_SERIAL_NUMBER;
            ret                    = KeyM_CertEleHandle(certId, &certEle, *cerS, tag);
            if (ret == (Std_ReturnType)E_OK)
            {
                *cerS += cerLen;
            }
        }
    }
    return ret;
}

/* PRQA S 6030 ++ */ /* VL_MTR_KeyM_STMIF */
KEYM_LOCAL Std_ReturnType
    KeyM_CertGetSigAlgorithmId(KeyM_CertificateIdType certId, uint8** cerS, const uint8* cerE, KeyM_Asn1DesType* sigPar)
/* PRQA S 6030 -- */
{
    uint32           tag;
    uint32           cerLen;
    KeyM_CertEleType certEle;
    uint8            objType = KEYM_CERT_ASN1_OID;
    Std_ReturnType   ret;
    const uint8*     temp_EndAddr = cerE;

    ret = KeyM_CertGetAsn1Tag(cerS, temp_EndAddr, &cerLen, KEYM_CERT_ASN1_SEQ_CONSTRU);
    /* PRQA S 1822,1852 ++ */ /* VL_KeyM_1822,VL_KeyM_1852 */
    if ((ret == (Std_ReturnType)E_OK) || ((temp_EndAddr - *cerS) < (uint8)KEYM_CONST_1))
    /* PRQA S 1822,1852 -- */
    {
        tag          = **cerS;
        temp_EndAddr = *cerS + cerLen;
        ret          = KeyM_CertGetAsn1Tag(cerS, temp_EndAddr, &cerLen, KEYM_CERT_ASN1_OID);
        if (ret == E_OK)
        {
            certEle.certEleMaxLen  = (uint16)cerLen;
            certEle.certEleObjType = &objType;
            certEle.certEleStruct  = CERTIFICATE_SIGNATURE_ALGORITHM_ID;
            ret                    = KeyM_CertEleHandle(certId, &certEle, *cerS, tag);
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

/* PRQA S 6030 ++ */ /* VL_MTR_KeyM_STMIF */
KEYM_LOCAL Std_ReturnType
    KeyM_CertHandleName(uint8** cerS, const uint8* cerE, KeyM_Asn1DesType* isuName, KeyM_Asn1DesType* isuNameOid)
/* PRQA S 6030 -- */
{
    uint32         len;
    Std_ReturnType ret;
    const uint8*   temp_EndAddr = cerE;

    ret = KeyM_CertGetAsn1Tag(cerS, temp_EndAddr, &len, KEYM_CERT_ASN1_SEQ_CONSTRU);
    if (ret == E_OK)
    {
        temp_EndAddr = *cerS + len;
        /* PRQA S 1822,1852 ++ */ /* VL_KeyM_1822,VL_KeyM_1852 */
        if ((temp_EndAddr - *cerS) < (uint8)KEYM_CONST_1)
        /* PRQA S 1822,1852 -- */
        {
            ret = E_NOT_OK;
        }
        else
        {
            isuNameOid->tag = **cerS;
            ret             = KeyM_CertGetAsn1Tag(cerS, temp_EndAddr, &isuNameOid->len, KEYM_CERT_ASN1_OID);
            if (ret == E_OK)
            {
                KeyM_CopyData(isuNameOid->dataEle, *cerS, isuNameOid->len);
                *cerS += isuNameOid->len;

                /* PRQA S 1822,1852 ++ */ /* VL_KeyM_1822,VL_KeyM_1852 */
                if (((temp_EndAddr - *cerS) < (uint8)KEYM_CONST_1)
                    || ((**cerS != KEYM_CERT_ASN1_BIT_STRING) && (**cerS != KEYM_CERT_ASN1_OCTET_STRING)
                        && (**cerS != KEYM_CERT_ASN1_UINT8_STRING) && (**cerS != KEYM_CERT_ASN1_ASCII_STRING)
                        && (**cerS != KEYM_CERT_ASN1_IA5_STRING)))
                /* PRQA S 1822,1852 -- */
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

/* PRQA S 6080 ++ */ /* VL_MTR_KeyM_STPTH */
KEYM_LOCAL Std_ReturnType KeyM_CertGetName(
    KeyM_CertificateIdType certId,
    uint8**                cerS,
    const uint8*           cerE,
    KeyM_CertEleStructType eleNameType,
    KeyM_CertEleStructType eleNameOidType)
/* PRQA S 6080 -- */
{
    const uint8*      cerEndSet;
    uint16            nameCfgId;
    KeyM_Asn1DesType* nameEleBuf;
    uint16            nameOidCfgId;
    KeyM_Asn1DesType* nameOidEleBuf;
    uint32            setLen;
    Std_ReturnType    ret;
    /* PRQA S 2001 ++ */ /* VL_KeyM_2001 */
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
    /* PRQA S 0772 ++ */ /* VL_KeyM_0772 */
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
        /* PRQA S 0772 -- */
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
        nameEleBuf    = nameEleBuf->next;
        nameOidEleBuf = nameOidEleBuf->next;
    }

ISSUER_NAME: /* PRQA S 2015 */ /* VL_KeyM_2015 */
    /* PRQA S 2001 -- */
    return ret;
}

KEYM_LOCAL Std_ReturnType KeyM_CertParseTimeEle(uint8** ele, uint32 len, uint32* time)
{
    uint32         eleLen = len;
    Std_ReturnType ret    = E_OK;

    *time = 0u;
    for (; eleLen > 0u; --eleLen) /* PRQA S 1252 */ /* VL_KeyM_2015 */
    {
        /*'0' <--> 0x30*/
        /*'9' <--> 0x39*/
        /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
        if ((**ele < 0x30u) || (**ele > 0x39u))
        /* PRQA S 3120 -- */
        {
            ret = KEYM_E_KEY_CERT_INVALID;
            break;
        }
        else
        {
            /* PRQA S 1252,3120 ++ */ /* VL_KeyM_1252,VL_QAC_MagicNum */
            /*ASCII convert*/
            *time *= 10u;
            *time += *(*ele)++; /* PRQA S 3440,3387 */ /* VL_KeyMN_3440,VL_KeyM_3387 */
            *time -= 0x30u;
            /* PRQA S 1252,3120 -- */
        }
    }
    return ret;
}

/* PRQA S 6060,2889 ++ */ /* VL_MTR_KeyM_STM19,VL_KeyM_2889 */
KEYM_LOCAL Std_ReturnType KeyM_CertCheckDate(const KeyM_CertTimeType* time)
/* PRQA S 6060,2889 -- */
{
    uint32         mon;
    Std_ReturnType ret = E_NOT_OK;
    /* PRQA S 2743,3120 ++ */ /* VL_KeyM_2743,VL_QAC_MagicNum */
    /* PRQA S 2996,1252 ++ */ /* VL_KeyM_2996,VL_KeyM_1252 */
    KEYM_CHECK_RANGE(9999u, time->certYear);
    KEYM_CHECK_RANGE(23u, time->certHour);
    KEYM_CHECK_RANGE(59u, time->certMin);
    KEYM_CHECK_RANGE(59u, time->certSec);
    /* PRQA S 2996,1252 -- */
    /*Calculate how many days in this month*/
    switch (time->certMon)
    {
        /* PRQA S 0597 ++ */ /* VL_KeyM_0597 */
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
        /* PRQA S 1252 ++ */ /* VL_KeyM_1252 */
        if ((((time->certYear % 4u) == 0u) && ((time->certYear % 100u) != 0u)) || ((time->certYear % 400u) == 0u))
        /* PRQA S 1252 -- */
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
        /* PRQA S 0597 -- */
    }
    KEYM_CHECK_RANGE(mon, time->certMon);
    /* PRQA S 2743,3120 -- */
    return E_OK;
}

/* PRQA S 6030, 6010 ++ */ /* VL_MTR_KeyM_STMIF, VL_MTR_KeyM_STCYC  */
KEYM_LOCAL Std_ReturnType KeyM_CertParseTime(uint32 len, uint32 yearLen, uint8** cerS, KeyM_CertTimeType* time)
/* PRQA S 6030, 6010 -- */
{
    Std_ReturnType ret        = E_NOT_OK;
    uint32         timebitnum = len;
    /*Minimum length is 10 or 12 depending on yearLen*/
    /* PRQA S 3120,1252 ++ */ /* VL_QAC_MagicNum, VL_KeyM_1252 */
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
                                /* PRQA S 1881,3123 ++ */ /* VL_KeyM_1881,VL_KeyM_3123 */
                                if ((KEYM_CONST_1 == timebitnum) && ('Z' == **cerS))
                                /* PRQA S 1881,3123 -- */
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
    /* PRQA S 3120,1252 -- */
    return ret;
}

KEYM_LOCAL Std_ReturnType KeyM_CertHandleGetTime(
    KeyM_CertificateIdType certId,
    uint8**                cerS,
    const uint8*           cerE,
    KeyM_CertTimeType*     time,
    KeyM_CertEleStructType eleStrut)
{
    uint32           yearLen;
    uint32           len;
    KeyM_CertEleType certEle;
    Std_ReturnType   ret = E_NOT_OK;
    uint8            timeTag;

    /* PRQA S 1822,1852 ++ */ /* VL_KeyM_1822,VL_KeyM_1852 */
    if ((cerE - *cerS) >= (uint8)KEYM_CONST_1)
    /* PRQA S 1822,1852 -- */
    {
        timeTag = **cerS;
        if (timeTag == KEYM_CERT_ASN1_UTC_TIME)
        {
            yearLen = 2u; /* PRQA S 3120 */ /* VL_QAC_MagicNum */
            ret     = E_OK;
        }
        else if (timeTag == KEYM_CERT_ASN1_GENERALIZED_TIME)
        {
            yearLen = 4u; /* PRQA S 3120 */ /* VL_QAC_MagicNum */
            ret     = E_OK;
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
                certEle.certEleMaxLen  = (uint16)len;
                certEle.certEleObjType = &timeTag;
                certEle.certEleStruct  = eleStrut;
                certEle.certEleObjId   = NULL_PTR;
                ret                    = KeyM_CertEleHandle(certId, &certEle, *cerS, timeTag);
                if (ret == E_OK)
                {
                    ret = KeyM_CertParseTime(len, yearLen, cerS, time);
                }
            }
        }
    }
    return ret;
}

KEYM_LOCAL Std_ReturnType KeyM_CertGetTime(KeyM_CertificateIdType certId, uint8** cerS, const uint8* cerE)
{
    uint32         len;
    Std_ReturnType ret;
    const uint8*   temp_EndAddr = cerE;

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

/* PRQA S 6030 ++ */ /* VL_MTR_KeyM_STMIF */
KEYM_LOCAL Std_ReturnType KeyM_CertHandleGetPubKeyAlgo(KeyM_CertificateIdType certId, uint8** cerS, const uint8* cerE)
/* PRQA S 6030 -- */
{
    const uint8*      cerEndPtr;
    KeyM_Asn1DesType* pubAlgoPtr;
    uint32            len;
    uint16            pubAlgoCfgIdx;
    Std_ReturnType    ret;

    ret = KeyM_IsCertExist(certId, CERTIFICATE_SUBJECT_PUBLIC_KEY_INFO_PUBLIC_KEY_ALGORITHM, &pubAlgoCfgIdx);
    if (ret == E_OK)
    {
        pubAlgoPtr = KeyM_CertPCfg[certId].certEleRef[pubAlgoCfgIdx].certEleBuf;
        ret        = KeyM_CertGetAsn1Tag(cerS, cerE, &len, KEYM_CERT_ASN1_SEQ_CONSTRU);
        if (ret == E_OK)
        {
            cerEndPtr = *cerS + len;

            pubAlgoPtr->tag = **cerS;
            ret             = KeyM_CertGetAsn1Tag(cerS, cerEndPtr, &pubAlgoPtr->len, KEYM_CERT_ASN1_OID);
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
                    if (pubAlgoPtr->tag == KEYM_CERT_ASN1_OID) /* PRQA S 1252 */ /* VL_KeyM_1252 */
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
                        pubAlgoPtr->len = 0u;
                        (*cerS) += 2u; /* PRQA S 3120 */ /* VL_QAC_MagicNum */
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

/* PRQA S 6030 ++ */ /* VL_MTR_KeyM_STMIF */
KEYM_LOCAL Std_ReturnType KeyM_CertGetPubKeyInfo(KeyM_CertificateIdType certId, uint8** cerS, const uint8* cerE)
/* PRQA S 6030 -- */
{
    uint32           len;
    uint32           tag;
    KeyM_CertEleType certEle;
    uint8            objType = KEYM_CERT_ASN1_BIT_STRING;
    Std_ReturnType   ret;
    const uint8*     temp_EndAddr = cerE;

    ret = KeyM_CertGetAsn1Tag(cerS, temp_EndAddr, &len, KEYM_CERT_ASN1_SEQ_CONSTRU);
    if (ret == E_OK)
    {
        temp_EndAddr = *cerS + len;
        ret          = KeyM_CertHandleGetPubKeyAlgo(certId, cerS, temp_EndAddr);
        if (ret == E_OK)
        {
            /*Public key info.*/
            tag = **cerS;
            ret = KeyM_CertGetAsn1Tag(cerS, temp_EndAddr, &len, KEYM_CERT_ASN1_BIT_STRING);
            if (ret == E_OK)
            {
                certEle.certEleMaxLen  = (uint16)len;
                certEle.certEleObjType = &objType;
                certEle.certEleStruct  = CERTIFICATE_SUBJECT_PUBLIC_KEY_INFO_SUBJECT_PUBLIC_KEY;
                ret                    = KeyM_CertEleHandle(certId, &certEle, *cerS, tag);
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

KEYM_LOCAL void KeyM_CertSkipField(uint8** cerS, const uint8* cerE, uint8 type)
{
    uint32         len;
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

KEYM_LOCAL Std_ReturnType KeyM_CertGetExtention(KeyM_CertificateIdType certId, uint8** cerS, const uint8* cerE)
{
    uint32           len;
    uint32           tag;
    KeyM_CertEleType certEle;
    uint8            objType = KEYM_CERT_ASN1_TAG | 3u; /* PRQA S 3120 */ /* VL_QAC_MagicNum */
    Std_ReturnType   ret;

    if (**cerS == 0xa3u) /* PRQA S 3120 */ /* VL_QAC_MagicNum */
    {
        tag = 0xa3u; /* PRQA S 3120 */                                        /* VL_QAC_MagicNum */
        ret = KeyM_CertGetAsn1Tag(cerS, cerE, &len, 0xa3u); /* PRQA S 3120 */ /* VL_QAC_MagicNum */
        if (ret == (Std_ReturnType)E_OK)
        {
            certEle.certEleMaxLen  = (uint16)len;
            certEle.certEleObjType = &objType;
            certEle.certEleStruct  = CERTIFICATE_EXTENSION;
            ret                    = KeyM_CertEleHandle(certId, &certEle, *cerS, tag);
            *cerS += len;
        }
    }
    else
    {
        ret = E_NOT_OK;
    }
    return ret;
}

KEYM_LOCAL Std_ReturnType KeyM_CertGetSignature(KeyM_CertificateIdType certId, uint8** cerS, const uint8* cerE)
{
    uint32           len;
    uint32           tag;
    KeyM_CertEleType certEle;
    uint8            objType = KEYM_CERT_ASN1_BIT_STRING;
    Std_ReturnType   ret;

    tag = **cerS;
    ret = KeyM_CertGetAsn1Tag(cerS, cerE, &len, KEYM_CERT_ASN1_BIT_STRING);
    if (ret == E_OK)
    {
        certEle.certEleMaxLen  = (uint16)len;
        certEle.certEleObjType = &objType;
        certEle.certEleStruct  = CERTIFICATE_SIGNATURE;
        /*unused bit*/
        if ((len % 0x08u) != 0u) /* PRQA S 3120,1252 */ /* VL_QAC_MagicNum,VL_KeyM_1252 */
        {
            *cerS += 1u; /* PRQA S 0489 */ /* VL_KeyM_0489 */
            certEle.certEleMaxLen -= 1u;
        }
        ret = KeyM_CertEleHandle(certId, &certEle, *cerS, tag);
        if (ret == E_OK)
        {
            *cerS += len - 1u; /* PRQA S 1252 */ /* VL_KeyM_1252 */
        }
    }
    return ret;
}

KEYM_LOCAL KeyM_Asn1DesType* KeyM_GetCertEle(KeyM_CertificateIdType certId, KeyM_CertEleStructType certEleStru)
{
    const KeyM_CertPCfgType* certCfgPtr;
    KeyM_Asn1DesType*        certEleBuf;
    uint16                   eleIdx;

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
            continue; /* PRQA S 0770 */ /* VL_KeyM_0770 */
        }
    }
    /* PRQA S 2963 ++ */ /* VL_KeyM_2963 */
    if (eleIdx == certCfgPtr->numOfCertEle)
    {
        certEleBuf = NULL_PTR;
    }
    return certEleBuf;
    /* PRQA S 2963 -- */
}

KEYM_LOCAL Std_ReturnType KeyM_CmpCertEle(
    /* PRQA S 3673 ++ */ /* VL_QAC_3673 */
    KeyM_Asn1DesType* certEleBuf,
    KeyM_Asn1DesType* upCertEleBuf)
/* PRQA S 3673 -- */
{
    Std_ReturnType          ret               = E_NOT_OK;
    const KeyM_Asn1DesType* temp_certEleBuf   = certEleBuf;
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
            temp_certEleBuf   = temp_certEleBuf->next;
            temp_upcertEleBuf = temp_upcertEleBuf->next;
        }
    }
    if ((temp_certEleBuf == NULL_PTR) && (temp_upcertEleBuf == NULL_PTR))
    {
        ret = E_OK;
    }

    return ret;
}

KEYM_LOCAL Std_ReturnType KeyM_GlobalTimeCheck(KeyM_CertificateIdType certId)
{
    uint64             ts_s;
    uint32             notBefore_s;
    uint32             notAfter_s;
    StbM_TimeTupleType stbM_TimeStamp;
    StbM_UserDataType  stbM_UserData;
    Std_ReturnType     ret = E_NOT_OK;

    if (certId < (KeyM_CertificateIdType)KEYM_CERT_NUM)
    {
        /* PRQA S 0432 ++ */ /* VL_KeyM_0432 */
        ret = StbM_GetCurrentTime(KeyM_CertPCfg[certId].certStbMTime, &stbM_TimeStamp, &stbM_UserData);
        /* PRQA S 0432 -- */
        if (ret == E_OK)
        {
            /*secondsHi:16 bit MSB of the 48 bits Seconds part of the time*/
            /*seconds:32 bit LSB of the 48 bits Seconds part of the time*/
            /*Convert the current time to seconds*/
            /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
            ts_s = (((uint64)stbM_TimeStamp.globalTime.secondsHi << 32u) | stbM_TimeStamp.globalTime.seconds);
            /* PRQA S 3120 -- */
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

KEYM_LOCAL uint8 KeyM_IsLeapYear(uint16 year)
{
    uint8 ret = 0u;

    if ((year % 4u) == 0u) /* PRQA S 3120 */ /* VL_QAC_MagicNum */
    {
        if (((year % 100u) == 0u) && ((year % 400u) != 0u)) /* PRQA S 3120 */ /* VL_QAC_MagicNum */
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

KEYM_LOCAL uint32 KeyM_ConvertTime_BeijingToUnix(const KeyM_CertTimeType* bjTime)
{
    uint32 dayNum = 0u;
    uint32 secNum;
    uint16 unixStartYear = 1970u; /* PRQA S 3120 */ /* VL_QAC_MagicNum */
    uint16 mon           = 0u;

    /*1.Calculate the number of days in all years*/
    while (unixStartYear < bjTime->certYear) /* PRQA S 1891 */ /* VL_KeyM_1891 */
    {
        if (KeyM_IsLeapYear(unixStartYear) != 0u)
        {
            dayNum += 366u; /* PRQA S 3120,1252 */ /* VL_QAC_MagicNum,VL_KeyM_1252 */
        }
        else
        {
            dayNum += 365u; /* PRQA S 3120,1252 */ /* VL_QAC_MagicNum,VL_KeyM_1252 */
        }
        unixStartYear++;
    }
    /*2.Calculate the number of days in all months.*/
    while ((mon + 1U) < bjTime->certMon) /* PRQA S 1252,1891 */ /* VL_KeyM_1252,VL_KeyM_1891 */
    {
        if (KeyM_IsLeapYear(bjTime->certYear) != 0u) /* PRQA S 4461 */ /* VL_KeyM_4461 */
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
    dayNum += bjTime->certDay - 1u; /* PRQA S 1252 */ /* VL_KeyM_1252 */

    /* PRQA S 3120,1252 ++  */ /* VL_QAC_MagicNum,VL_KeyM_1252 */
    /*4.Hour Minute Second*/
    secNum = dayNum * 24u * 60u * 60u;

    secNum += bjTime->certHour * 60u * 60u; /*Hour*/

    secNum += bjTime->certMin * 60u; /*Minute*/

    secNum += bjTime->certSec; /*Second*/

    /*5.Time zone adjustment*/
    secNum -= 8u * 60u * 60u; /*8:Beijing Time zone.*/
    /* PRQA S 3120,1252 -- */
    return secNum;
}

/* PRQA S 6030,6020,6080 ++ */ /* VL_MTR_KeyM_STMIF,VL_MTR_KeyM_STLIN,VL_MTR_KeyM_STPTH, */
/* PRQA S 6010,1532 ++ */      /* VL_MTR_KeyM_STCYC,VL_QAC_OneFunRef */
Std_ReturnType KeyM_HandleCertcVerify(const KeyM_CertPCfgType* certCfgPtr, const KeyM_CertPCfgType* certUpperHierRef)
/* PRQA S 6010,1532 -- */
/* PRQA S 6030,6020,6080 -- */
{
    KeyM_Asn1DesType* certEleBuf;
    KeyM_Asn1DesType* upCertEleBuf;
    /* PRQA S 3678 ++ */ /* VL_QAC_3678 */
    KeyM_Asn1DesType* sigBuf;
    KeyM_Asn1DesType* pbKeyBuf;
    /* PRQA S 3678 -- */
    uint32 sigVerJob;
    uint32 tarKey;
    uint16 CertId = certCfgPtr->certId;
#if (KEYM_NVM_BLOCK_NUM > 0)
    uint16 nvmRefBloId;
#endif
    Crypto_VerifyResultType verifyRes = CRYPTO_E_VER_NOT_OK;
    Std_ReturnType          ret;

    /*1.Subject check*/
    if (certCfgPtr->selfSignedCert == TRUE) /* PRQA S 2109 */ /* VL_KeyM_2109 */
    {
        /*self-signed : SUBJECT = ISSUER*/
        certEleBuf   = KeyM_GetCertEle(CertId, CERTIFICATE_SUBJECT_NAME);
        upCertEleBuf = KeyM_GetCertEle(CertId, CERTIFICATE_ISSUER_NAME);
    }
    else
    {
        certEleBuf   = KeyM_GetCertEle(CertId, CERTIFICATE_ISSUER_NAME);
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
                if (!certCfgPtr->certRuleArbiFunc()) /* PRQA S 2109 */ /* VL_KeyM_2109 */
                {
                    ret                     = E_NOT_OK;
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
                        if (certCfgPtr->selfSignedCert == TRUE) /* PRQA S 2109 */ /* VL_KeyM_2109 */
                        {
                            /*For self-signed certificates, first the public key of this certificate needs
                             * to be set*/
                            /* PRQA S 1840,1258 ++ */ /* VL_KeyM_1840,VL_KeyM_1258 */
                            ret = Csm_KeyElementSet(
                                certCfgPtr->certSigVeriKey,
                                CRYPTO_KE_SIGNATURE_KEY,
                                &pbKeyBuf->dataEle[1],
                                pbKeyBuf->len - 1);
                            /* PRQA S 1840,1258 -- */
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

                            /* PRQA S 2981,2996,2992 ++ */ /* VL_KeyM_2981,VL_KeyM_2996,VL_KeyM_2992 */
                            if (nvmRefBloId < KEYM_NVM_BLOCK_NUM)
                            /* PRQA S 2981,2996,2992 -- */
                            {
                                /* PRQA S 2880 ++ */ /* VL_KeyM_2880 */
                                if (KeyM_NvmBlockPCfg[nvmRefBloId].blkWriDelay == 0u)
                                /* PRQA S 2880 -- */
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
#endif /*STD_ON == KEYM_CERTIFICATE_MANAGER_ENABLED*/
/* PRQA S 0488 -- */
/* PRQA S 4603,4604 -- */
/* PRQA S 3432 --*/
/* PRQA S 1503 --*/
