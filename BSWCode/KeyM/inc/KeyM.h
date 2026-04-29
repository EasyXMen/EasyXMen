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
 *
 ************************************************************************************************************************
 **
 **  @file               : KeyM.h
 **  @author             : jie.gu
 **  @date               : 2025/03/31
 **  @vendor             : isoft
 **  @description        : API declaration and type definitions of KeyM
 **
 ***********************************************************************************************************************/
/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version     Date          Author          Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V2.2.1    20250331    jie.gu       inittial version for R23-11
 *
 *
 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:FVM<br>
  RuleSorce:puhua_rules-2024.12.rcf

    \li VL_KeyM_1271
      Reason: non-init expression for enum definition is necessary
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_0779
      Reason: Configure tools to ensure the correctness of character matching.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_3332
      Reason: The corresponding macro definition has been obtained through the header file inclusion.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_0488
      Reason: Calculates the offset value of the array.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_4603
      Reason: Dereference pointer.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_4604
      Reason: A reserved identifier or reserved macro name shall not be declared.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_3432
      Reason: Function-like macros are used to allow more efficient code.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_5016
      Reason: Parameter passing requires
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_3218
      Reason: Used to record global information, such as the timestamp of the first entry into the main function, the
    last access to the main function.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_1252
      Reason: suffixed constant comparison is necessary.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_2981
      Reason: Init may be necessary for return variant.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_0311
      Reason: Compatible with different types of functions.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_2109
      Reason: Boolean types need to be used as operands
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_2991
      Reason: The control statement has no effect, but is reserved for better readability.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_2995
      Reason: The control statement has no effect, but is reserved for better readability.
      Risk: No risk.
      Prevention: None.

    \li VL_MTR_KeyM_STMIF
      Reason: Functions handle specific tasks in the behavior of the entire component.
    Task has different scenarios that need to be covered, which depends on local conditions - this
    leads to deep nesting of the control structure.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_0771
      Reason: Ability to quickly jump out of loops in different situations with different configurations.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_3678
      Reason: non-const type declaration is necessary depends on the config.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_2963
      Reason: Strong rotations between different pointer types have no effect.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_2880
      Reason: the issue shall be avoided in actual running environment and configs
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_2961
      Reason: Strong rotations between different pointer types have no effect.
      Risk: No risk.
      Prevention: None.

    \li VL_MTR_KeyM_STPTH
      Reason: The functions were not separated into sub-functions because there were higher requirements for
    minimizing the stack and applying them to the runtime usage of the code.
      Risk: No risk.
      Prevention: None.

    \li VL_MTR_KeyM_STCAL
      Reason: The standard compliance vote was higher than the measurement threshold. In addition, a typical way to
    reduce STCAL is to deepen function nesting, which increases call stack usage and runtime.
      Risk: No risk.
      Prevention: None.

    \li VL_MTR_KeyM_STCYC
      Reason: Functionality is not separated into subfunctions because there are higher requirements for minimizing the
  stack and applying it to runtime use of the code.
      Risk: No risk.
      Prevention: None.

    \li VL_MTR_KeyM_STST3
      Reason: Some function logics are overly complex. Splitting and implementing them will lead to reduced performance.
      Risk: No risk.
      Prevention: None.

    \li VL_MTR_KeyM_STLIN
      Reason: Some function logics are overly complex. Splitting and implementing them will lead to reduced performance.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_5017
      Reason: High cyclomatic complexity is reserved for better readability.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_2001
      Reason: The code is designed so
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_1258
      Reason: The cast conversion from a constant is safe.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_2015
      Reason: The code is designed so
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_1881
      Reason: The code is designed so
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_3206
      Reason: Variables are used in different configurations due to complex precompilation logic.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_3397
      Reason: The code is designed so
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_0770
      Reason: Ability to quickly jump of next loops in different situations with different configurations.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_1822
      Reason: Pointers are compared with integers.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_1852
      Reason: Pointers are compared with integers.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_3440
      Reason: Floating-point numbers are used in the for loop
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_3387
      Reason: The pointer increases or decreases by itself.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_2911
      Reason: Confirmed: enclosed in unsigned arithmetic operations.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_0772
      Reason: The loop contains multiple statements to terminate the loop, including goto
      Risk: No risk.
      Prevention: None.

    \li VL_MTR_KeyM_STM19
      Reason: In principle, a function is not allowed to have multiple exit points,
    but it can help improve code performance in special circumstances.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_2743
      Reason: The macro defines format requirements.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_2996
      Reason: the issue shall be avoided in actual running environment and configs
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_0597
      Reason: Type of case label expression is not consistent with type of controlling expression
    in switch statement.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_3123
      Reason: Hard-encoded string constants were used.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_0432
      Reason: Function argument is not of compatible pointer type.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_1891
      Reason: Macro Conveniently determining authenticity.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_4461
      Reason: Datatype conversion is necessary for functional usage
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_1840
      Reason: A signed constant is implicitly converted to an unsigned type
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_2992
      Reason: Different configurations will result in different values in the if logical expression.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_1504
      Reason: Variables are used in different configurations due to complex precompilation logic.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_3673
      Reason: Parameter passing requires
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_3408
      Reason: Variables are provided externally and need to be declared externally.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_2877
      Reason: The usage of this file depends on the config.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_0331
      Reason: A const pointer type is forcibly converted to a non-const pointer type.
      Risk: No risk.
      Prevention: None.

    \li VL_MTR_KeyM_STPAR
      Reason: The parameters of the function need.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_1505
      Reason: External interface.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_0310
      Reason: Compatible with different types.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_3305
      Reason: The code is designed so.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_1312
      Reason: Only valid characters need to be saved, and no ending character is required
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_1290
      Reason: Signed numbers need to be converted to unsigned numbers for use
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_3122
      Reason: Hard-coded string constants were used.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_3232
      Reason: String constants are saved using arrays.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_1257
      Reason: Shorter integers are used to save longer integers.
      Risk: No risk.
      Prevention: None.

    \li VL_MTR_KeyM_CONF
      Reason: Configuration files do not need particularly many useless comments.
      Risk: No risk.
      Prevention: None.

    \li VL_KeyM_0489
      Reason: Use the pointer to shift back one unit.
      Risk: No risk.
      Prevention: None.

 */

#ifndef KEYM_H
#define KEYM_H
/* =================================================== inclusions =================================================== */
#include "Std_Types.h"
#include "Rte_KeyM_Type.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */
#define KEYM_MODULE_ID        115U /**< Module ID for KeyM @range 115U */
#define KEYM_H_VENDOR_ID      62U  /**< Vendor ID for KeyM @range 62U */
#define KEYM_INSTANCE_ID      7U   /**< Instance ID for KeyM @range 7U */
#define KEYM_SW_MAJOR_VERSION 2U   /**< Major software version for KeyM @range 2U */
#define KEYM_SW_MINOR_VERSION 2U   /**< Minor software version for KeyM @range 2U */
#define KEYM_SW_PATCH_VERSION 0U   /**< Patch software version for KeyM @range 0U */
#define KEYM_AR_MAJOR_VERSION 4U   /**< Major AUTOSAR version for KeyM @range 4U */
#define KEYM_AR_MINOR_VERSION 9U   /**< Minor AUTOSAR version for KeyM @range 9U */
#define KEYM_AR_PATCH_VERSION 0U   /**< Patch AUTOSAR version for KeyM @range 0U */

/* ===================================================== macros ===================================================== */
#define KEYM_INIT_ID                ((uint8)0x01U) /**< API ID for KeyM_Init function @range 0x01U */
#define KEYM_DEINIT_ID              ((uint8)0x02U) /**< API ID for KeyM_Deinit function @range 0x02U */
#define KEYM_GETVERSIONINFO_ID      ((uint8)0x03U) /**< API ID for KeyM_GetVersionInfo function @range 0x03U */
#define KEYM_START_ID               ((uint8)0x04U) /**< API ID for KeyM_Start function @range 0x04U */
#define KEYM_PREPARE_ID             ((uint8)0x05U) /**< API ID for KeyM_Prepare function @range 0x05U */
#define KEYM_UPDATE_ID              ((uint8)0x06U) /**< API ID for KeyM_Update function @range 0x06U */
#define KEYM_FINAL_ID               ((uint8)0x07U) /**< API ID for KeyM_Final function @range 0x07U */
#define KEYM_VERIFY_ID              ((uint8)0x08U) /**< API ID for KeyM_Verify function @range 0x08U */
#define KEYM_SERV_CERTIFICATE_ID    ((uint8)0x09U) /**< API ID for KeyM_Serv_Certificate function @range 0x09U */
#define KEYM_SET_CERTIFICATE_ID     ((uint8)0x0AU) /**< API ID for KeyM_Set_Certificate function @range 0x0AU */
#define KEYM_GET_CERTIFICATE_ID     ((uint8)0x0BU) /**< API ID for KeyM_Get_Certificate function @range 0x0BU */
#define KEYM_VERIFY_CERTIFICATES_ID ((uint8)0x0CU) /**< API ID for KeyM_Verify_Certificates function @range 0x0CU */
#define KEYM_VERIFY_CERTIFICATE_ID  ((uint8)0x0DU) /**< API ID for KeyM_Verify_Certificate function @range 0x0DU */
#define KEYM_VERIFY_CERTIFICATE_CHAIN_ID \
    ((uint8)0x0EU)                              /**< API ID for KeyM_Verify_Certificate_Chain function @range 0x0EU */
#define KEYM_CERT_ELEMENT_GET_ID ((uint8)0x0FU) /**< API ID for KeyM_Cert_Element_Get function @range 0x0FU */
#define KEYM_CERT_ELEMENT_GET_FIRST_ID \
    ((uint8)0x10U) /**< API ID for KeyM_Cert_Element_Get_First function @range 0x10U */
#define KEYM_CERT_ELEMENT_GET_NEXT_ID                                                                           \
    ((uint8)0x11U)                             /**< API ID for KeyM_Cert_Element_Get_Next function @range 0x11U \
                                                */
#define KEYM_CERT_GET_STATUS_ID ((uint8)0x12U) /**< API ID for KeyM_Cert_Get_Status function @range 0x12U */
#define KEYM_CERT_ELEMENT_GET_BY_INDEX \
    ((uint8)0x1BU) /**< API ID for KeyM_Cert_Element_Get_By_Index function @range 0x1BU */
#define KeyM_CertElementGetCount_ID ((uint8)0x1CU) /**< API ID for KeyM_CertElementGetCount function @range 0x1CU */

#define KEYM_E_PARAM_POINTER  ((uint8)0x01U) /**< Error code for parameter pointer error @range 0x01U */
#define KEYM_E_SMALL_BUFFER   ((uint8)0x02U) /**< Error code for small buffer error @range 0x02U */
#define KEYM_E_UNINIT         ((uint8)0x03U) /**< Error code for uninitialized error @range 0x03U */
#define KEYM_E_INIT_FAILED    ((uint8)0x04U) /**< Error code for initialization failed error @range 0x04U */
#define KEYM_E_CONFIG_FAILURE ((uint8)0x05U) /**< Error code for configuration failure error @range 0x05U */

#define KEYM_E_BUSY    ((uint8)0x02U) /**< Error code for busy error @range 0x02U */
#define KEYM_E_PENDING ((uint8)0x03U) /**< Error code for pending error @range 0x03U */
#define KEYM_E_KEY_CERT_SIZE_MISMATCH \
    ((uint8)0x04U) /**< Error code for key certificate size mismatch error @range 0x04U */
#define KEYM_E_PARAMETER_MISMATCH  ((uint8)0x05U) /**< Error code for parameter mismatch error @range 0x05U */
#define KEYM_E_KEY_CERT_INVALID    ((uint8)0x06U) /**< Error code for invalid key certificate error @range 0x06U */
#define KEYM_E_CERT_INVALID        ((uint8)0x06U) /**< Error code for invalid certificate error @range 0x06U */
#define KEYM_E_KEY_CERT_WRITE_FAIL ((uint8)0x07U) /**< Error code for key certificate write fail error @range 0x07U */
#define KEYM_E_KEY_CERT_UPDATE_FAIL                                                                                 \
    ((uint8)0x08U)                               /**< Error code for key certificate update fail error @range 0x08U \
                                                  */
#define KEYM_E_KEY_CERT_READ_FAIL ((uint8)0x09U) /**< Error code for key certificate read fail error @range 0x09U */
#define KEYM_E_KEY_CERT_EMPTY     ((uint8)0x0AU) /**< Error code for empty key certificate error @range 0x0AU */
#define KEYM_E_CERT_INVALID_CHAIN_OF_TRUST \
    ((uint8)0x0BU) /**< Error code for invalid certificate chain of trust error @range 0x0BU */

/* ================================================ type definitions ================================================ */
/**
 * @brief Configuration structure for the KeyM module.
 *
 */
typedef struct
{
    uint8 reserved; /**< Reserved field @range any valid uint8 value */
} KeyM_ConfigType;

/**
 * @brief Enumeration of key handler update operations.
 *
 */
typedef enum
{
    KEYM_KH_UPDATE_KEY_UPDATE_REPEAT = 1, /**< Key update repeat operation @range 1U */
    KEYM_KH_UPDATE_FINISH            = 2  /**< Finish update operation @range 2U */
} KeyM_KH_UpdateOperationType;

/**
 * @brief Structure to hold ASN.1 data.
 *
 */
typedef struct KeyM_Asn1Type
{
    uint32                tag;     /**< ASN.1 tag @range any valid uint32 value */
    uint32                len;     /**< Length of the ASN.1 data element @range any valid uint32 value */
    uint8*                dataEle; /**< Pointer to the ASN.1 data element @range any valid pointer */
    struct KeyM_Asn1Type* next;    /**< Pointer to the next ASN.1 element @range any valid pointer */
} KeyM_Asn1DesType;

/**
 * @brief Structure to hold certificate element iterator information.
 *
 */
typedef struct
{
    KeyM_CertificateIdType certId; /**< Identifier of the certificate @range any valid KeyM_CertificateIdType value */
    KeyM_CertElementIdType
        certEleId; /**< Identifier of the certificate element @range any valid KeyM_CertElementIdType value */
    KeyM_Asn1DesType* eleItrator; /**< Pointer to the current ASN.1 element iterator @range any valid pointer */
} KeyM_CertElementIteratorType;

/**
 * @brief Type definition for a crypto key identifier.
 */
typedef uint16 KeyM_CryptoKeyIdType; /**< Crypto key identifier type @range any valid uint16 value */

/**
 * @brief Type definition for a certificate data pointer.
 */
typedef uint8* KeyM_CertDataPointerType; /**< Certificate data pointer type @range any valid pointer */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief          Initializes the Key Management module.
 * @param[in]      ConfigPtr : Pointer to the Key Management configuration structure.
 * @return         None
 * @reentrant      Not reentrant.
 * @synchronous    TRUE
 * @trace       CPD-71402
 */
void KeyM_Init(const KeyM_ConfigType* ConfigPtr);

/**
 * @brief          Deinitializes the Key Management module.
 * @reentrant      Not reentrant.
 * @synchronous    TRUE
 * @trace       CPD-71403
 */
void KeyM_Deinit(void);

/**
 * @brief          Retrieves version information for the Key Management module.
 * @param[out]     VersionInfo : Pointer to the structure to store version information.
 * @return         None
 * @reentrant      Not reentrant.
 * @synchronous    TRUE
 * @trace       CPD-71404
 */
void KeyM_GetVersionInfo(Std_VersionInfoType* VersionInfo);

/*Key*/

/**
 * @brief          Starts the Key Management module.
 * @param[in]      StartType : Type of start operation.
 * @param[in]      RequestData : Pointer to the request data.
 * @param[in]      RequestDataLength : Length of the request data.
 * @param[out]     ResponseData : Pointer to the response data.
 * @param[out]     ResponseDataLength : Pointer to the length of the response data.
 * @return         Std_ReturnType
 * @retval         E_OK: The start operation was successful.
 * @retval         E_NOT_OK: The start operation failed.
 * @reentrant      Not reentrant.
 * @synchronous    TRUE
 * @trace       CPD-71405
 */
#if (KEYM_CRYPTO_KEY_START_FINALIZE_FUNCTION_ENABLED == STD_ON) /* PRQA S 3332 */ /* VL_KeyM_3332 */
Std_ReturnType KeyM_Start(
    KeyM_StartType StartType,
    const uint8*   RequestData,
    uint16         RequestDataLength,
    uint8*         ResponseData,
    uint16*        ResponseDataLength);

/**
 * @brief          Finalizes the Key Management module.
 * @param[in]      RequestDataPtr : Pointer to the request data.
 * @param[in]      RequestDataLength : Length of the request data.
 * @param[out]     ResponseDataPtr : Pointer to the response data.
 * @param[in]      ResponseMaxDataLength : Maximum length of the response data.
 * @return         Std_ReturnType
 * @retval         E_OK: The finalize operation was successful.
 * @retval         E_NOT_OK: The finalize operation failed.
 * @reentrant      Not reentrant.
 * @synchronous    TRUE
 * @trace       CPD-71406
 */
Std_ReturnType KeyM_Finalize(
    const uint8* RequestDataPtr,
    uint16       RequestDataLength,
    uint8*       ResponseDataPtr,
    uint16       ResponseMaxDataLength);
#endif

#if (STD_ON == KEYM_CRYPTO_KEY_PREPARE_FUNCTION_ENABLED) /* PRQA S 3332 */ /* VL_KeyM_3332 */
/**
 * @brief          Prepares the Key Management module for a cryptographic operation.
 * @param[in]      RequestData : Pointer to the request data.
 * @param[in]      RequestDataLength : Length of the request data.
 * @param[out]     ResponseData : Pointer to the response data.
 * @param[inout]    ResponseDataLength : Pointer to the length of the response data.
 * @return         Std_ReturnType
 * @retval         E_OK: The preparation was successful.
 * @retval         E_NOT_OK: The preparation failed.
 * @reentrant      Not reentrant.
 * @synchronous    TRUE
 * @trace       CPD-71407
 */
Std_ReturnType
    KeyM_Prepare(const uint8* RequestData, uint16 RequestDataLength, uint8* ResponseData, uint16* ResponseDataLength);
#endif
/**
 * @brief          Updates a key in the Key Management module.
 * @param[in]      KeyNamePtr : Pointer to the key name.
 * @param[in]      KeyNameLength : Length of the key name.
 * @param[in]      RequestDataPtr : Pointer to the request data.
 * @param[in]      RequestDataLength : Length of the request data.
 * @param[out]     ResultDataPtr : Pointer to the result data.
 * @param[in]      ResultDataMaxLength : Maximum length of the result data.
 * @return         Std_ReturnType
 * @retval         E_OK: The update operation was successful.
 * @retval         E_NOT_OK: The update operation failed.
 * @reentrant      Not reentrant.
 * @synchronous    TRUE
 * @trace       CPD-71408
 */
Std_ReturnType KeyM_Update(
    const uint8* KeyNamePtr,
    uint16       KeyNameLength,
    const uint8* RequestDataPtr,
    uint16       RequestDataLength,
    uint8*       ResultDataPtr,
    uint16       ResultDataMaxLength);

#if (KEYM_CRYPTO_KEY_VERIFY_FUNCTION_ENABLED == STD_ON) /* PRQA S 3332 */ /* VL_KeyM_3332 */
/**
 * @brief          Verifies a key in the Key Management module.
 * @param[in]      KeyNamePtr : Pointer to the key name.
 * @param[in]      KeyNameLength : Length of the key name.
 * @param[in]      RequestData : Pointer to the request data.
 * @param[in]      RequestDataLength : Length of the request data.
 * @param[out]     ResponseData : Pointer to the response data.
 * @param[inout]    ResponseDataLength : Pointer to the length of the response data.
 * @return         Std_ReturnType
 * @retval         E_OK: The verify operation was successful.
 * @retval         E_NOT_OK: The verify operation failed.
 * @retval         KEYM_E_PENDING: The verify operation is pending.
 * @reentrant      Not reentrant.
 * @synchronous    TRUE
 * @trace       CPD-71409
 */
Std_ReturnType KeyM_Verify(
    const uint8* KeyNamePtr,
    uint16       KeyNameLength,
    const uint8* RequestData,
    uint16       RequestDataLength,
    uint8*       ResponseData,
    uint16*      ResponseDataLength);
#endif
/*Certificate*/
/**
 * @brief          Performs a certificate service operation in the Key Management module.
 * @param[in]      Service : Type of certificate service operation.
 * @param[in]      CertNamePtr : Pointer to the certificate name.
 * @param[in]      CertNameLength : Length of the certificate name.
 * @param[in]      RequestData : Pointer to the request data.
 * @param[in]      RequestDataLength : Length of the request data.
 * @param[out]     ResponseData : Pointer to the response data.
 * @param[in]      ResponseDataLength : Length of the response data.
 * @return         Std_ReturnType
 * @retval         E_OK: The certificate service operation was successful.
 * @retval         E_NOT_OK: The certificate service operation failed.
 * @reentrant      Not reentrant.
 * @synchronous    TRUE
 * @trace       CPD-71410
 */
Std_ReturnType KeyM_ServiceCertificate(
    KeyM_ServiceCertificateType Service,
    const uint8*                CertNamePtr,
    uint32                      CertNameLength,
    const uint8*                RequestData,
    uint32                      RequestDataLength,
    uint8*                      ResponseData,
    uint32*                     ResponseDataLength);
/**
 * @brief          Performs a certificate service operation in the Key Management module.
 * @param[in]      Service : Type of certificate service operation.
 * @param[in]      CertId :  the certificate ID.
 * @param[in]      RequestData : Pointer to the request data.
 * @param[in]      RequestDataLength : Length of the request data.
 * @param[out]     ResponseData : Pointer to the response data.
 * @param[in]      ResponseDataLength : Length of the response data.
 * @return         Std_ReturnType
 * @retval         E_OK: The certificate service operation was successful.
 * @retval         E_NOT_OK: The certificate service operation failed.
 * @reentrant      Not reentrant.
 * @synchronous    TRUE
 * @trace       CPD-59104
 */
Std_ReturnType KeyM_ServiceCertificateByCertId(
    KeyM_CertificateIdType      CertId,
    KeyM_ServiceCertificateType Service,
    const uint8*                RequestData,
    uint32                      RequestDataLength,
    uint8*                      ResponseData,
    uint32*                     ResponseDataLength);

/**
 * @brief          Sets a certificate in the Key Management module.
 * @param[in]      CertId : ID of the certificate to set.
 * @param[in]      CertificateDataPtr : Pointer to the certificate data.
 * @return         Std_ReturnType
 * @retval         E_OK: The certificate was set successfully.
 * @retval         E_NOT_OK: The certificate could not be set.
 * @reentrant      Not reentrant.
 * @synchronous    TRUE
 * @trace       CPD-71411
 */
Std_ReturnType KeyM_SetCertificate(KeyM_CertificateIdType CertId, const KeyM_CertDataType* CertificateDataPtr);

/**
 * @brief          Retrieves a certificate from the Key Management module.
 * @param[in]      CertId : ID of the certificate to retrieve.
 * @param[out]     CertificateDataPtr : Pointer to the buffer to store the certificate data.
 * @return         Std_ReturnType
 * @retval         E_OK: The certificate was retrieved successfully.
 * @retval         E_NOT_OK: The certificate could not be retrieved.
 * @retval         KEYM_E_KEY_CERT_SIZE_MISMATCH: The provided buffer is too small to hold the certificate.
 * @retval         KEYM_E_KEY_CERT_EMPTY: The certificate is empty.
 * @reentrant      Not reentrant.
 * @synchronous    TRUE
 * @trace       CPD-71412
 */
Std_ReturnType KeyM_GetCertificate(KeyM_CertificateIdType CertId, KeyM_CertDataType* CertificateDataPtr);

/**
 * @brief          Verifies a certificate chain in the Key Management module.
 * @param[in]      CertId : ID of the certificate to verify.
 * @param[in]      CertUpperId : ID of the upper-level certificate in the chain.
 * @return         Std_ReturnType
 * @retval         E_OK: The certificate chain was verified successfully.
 * @retval         E_NOT_OK: The certificate chain could not be verified.
 * @retval         KEYM_E_PARAMETER_MISMATCH: The certificate IDs are invalid.
 * @retval         KEYM_E_CERT_INVALID_CHAIN_OF_TRUST: The certificate chain is invalid.
 * @reentrant      Not reentrant.
 * @synchronous    TRUE
 * @trace       CPD-71413
 */
Std_ReturnType KeyM_VerifyCertificates(KeyM_CertificateIdType CertId, KeyM_CertificateIdType CertUpperId);

/**
 * @brief          Verifies a certificate in the Key Management module.
 * @param[in]      CertId : ID of the certificate to verify.
 * @return         Std_ReturnType
 * @retval         E_OK: The certificate was verified successfully.
 * @retval         E_NOT_OK: The certificate could not be verified.
 * @retval         KEYM_E_PARAMETER_MISMATCH: The certificate ID is invalid.
 * @retval         KEYM_E_CERT_INVALID_CHAIN_OF_TRUST: The certificate chain is invalid.
 * @reentrant      Not reentrant.
 * @synchronous    TRUE
 * @trace       CPD-71414
 */
Std_ReturnType KeyM_VerifyCertificate(KeyM_CertificateIdType CertId);

/**
 * @brief          Verifies a certificate chain in the Key Management module.
 * @param[in]      CertId : ID of the starting certificate in the chain.
 * @param[in]      certChainData : Array of certificate data for the chain.
 * @param[in]      NumberOfCertificates : Number of certificates in the chain.
 * @return         Std_ReturnType
 * @retval         E_OK: The certificate chain was verified successfully.
 * @retval         E_NOT_OK: The certificate chain could not be verified.
 * @reentrant      Not reentrant.
 * @synchronous    TRUE
 * @trace       CPD-71415
 */
Std_ReturnType KeyM_VerifyCertificateChain(
    KeyM_CertificateIdType  CertId,
    const KeyM_CertDataType certChainData[],
    uint8                   NumberOfCertificates);

/**
 * @brief          Retrieves a certificate element from the Key Management module.
 * @param[in]      CertId : ID of the certificate.
 * @param[in]      CertElementId : ID of the certificate element to retrieve.
 * @param[out]     CertElementData : Pointer to the buffer to store the certificate element data.
 * @param[inout]    CertElementDataLength : Pointer to the length of the certificate element data.
 * @return         Std_ReturnType
 * @retval         E_OK: The certificate element was retrieved successfully.
 * @retval         E_NOT_OK: The certificate element could not be retrieved.
 * @reentrant      Not reentrant.
 * @synchronous    TRUE
 * @trace       CPD-71416
 */
Std_ReturnType KeyM_CertElementGet(
    KeyM_CertificateIdType CertId,
    KeyM_CertElementIdType CertElementId,
    uint8*                 CertElementData,
    uint32*                CertElementDataLength);

/**
 * @brief          Retrieves a certificate element by index from the Key Management module.
 * @param[in]      CertId : ID of the certificate.
 * @param[in]      CertElementId : ID of the certificate element to retrieve.
 * @param[in]      Index : Index of the certificate element to retrieve.
 * @param[out]     CertElementDataPtr : Pointer to the buffer to store the certificate element data.
 * @param[inout]    CertElementDataLengthPtr : Pointer to the length of the certificate element data.
 * @return         Std_ReturnType
 * @retval         E_OK: The certificate element was retrieved successfully.
 * @retval         E_NOT_OK: The certificate element could not be retrieved.
 * @reentrant      Not reentrant.
 * @synchronous    TRUE
 * @trace       CPD-71417
 */
Std_ReturnType KeyM_CertificateElementGetByIndex(
    KeyM_CertificateIdType CertId,
    KeyM_CertElementIdType CertElementId,
    uint32                 Index,
    uint8*                 CertElementDataPtr,
    uint32*                CertElementDataLengthPtr);

/**
 * @brief          Retrieves the count of a specific certificate element in the Key Management module.
 * @param[in]      CertId : ID of the certificate.
 * @param[in]      CertElementId : ID of the certificate element.
 * @param[out]     CountPtr : Pointer to store the count of the certificate element.
 * @return         Std_ReturnType
 * @retval         E_OK: The count was retrieved successfully.
 * @retval         E_NOT_OK: The count could not be retrieved.
 * @reentrant      Not reentrant.
 * @synchronous    TRUE
 * @trace       CPD-71418
 */
Std_ReturnType KeyM_CertificateElementGetCount(
    KeyM_CertificateIdType CertId,
    KeyM_CertElementIdType CertElementId,
    uint16*                CountPtr);

/**
 * @brief          Retrieves the first occurrence of a certificate element and initializes an iterator.
 * @param[in]      CertId : ID of the certificate.
 * @param[in]      CertElementId : ID of the certificate element to retrieve.
 * @param[out]     CertElementIterator : Pointer to the iterator to be initialized.
 * @param[out]     CertElementData : Pointer to the buffer to store the certificate element data.
 * @param[inout]    CertElementDataLength : Pointer to the length of the certificate element data.
 * @return         Std_ReturnType
 * @retval         E_OK: The first certificate element was retrieved successfully and the iterator was initialized.
 * @retval         E_NOT_OK: The first certificate element could not be retrieved or the iterator could not be
 * initialized.
 * @reentrant      Not reentrant.
 * @synchronous    TRUE
 * @trace       CPD-71419
 */
Std_ReturnType KeyM_CertElementGetFirst(
    KeyM_CertificateIdType        CertId,
    KeyM_CertElementIdType        CertElementId,
    KeyM_CertElementIteratorType* CertElementIterator,
    uint8*                        CertElementData,
    uint32*                       CertElementDataLength);

/**
 * @brief          Retrieves the next occurrence of a certificate element using an iterator.
 * @param[inout]    CertElementIterator : Pointer to the iterator initialized by KeyM_CertElementGetFirst.
 * @param[out]     CertElementData : Pointer to the buffer to store the certificate element data.
 * @param[inout]    CertElementDataLength : Pointer to the length of the certificate element data.
 * @return         Std_ReturnType
 * @retval         E_OK: The next certificate element was retrieved successfully.
 * @retval         E_NOT_OK: The next certificate element could not be retrieved.
 * @reentrant      Not reentrant.
 * @synchronous    TRUE
 * @trace       CPD-71420
 */
Std_ReturnType KeyM_CertElementGetNext(
    KeyM_CertElementIteratorType* CertElementIterator,
    uint8*                        CertElementData,
    uint32*                       CertElementDataLength);

/**
 * @brief          Retrieves the status of a certificate in the Key Management module.
 * @param[in]      CertId : ID of the certificate.
 * @param[out]     Status : Pointer to store the status of the certificate.
 * @return         Std_ReturnType
 * @retval         E_OK: The status was retrieved successfully.
 * @retval         E_NOT_OK: The status could not be retrieved.
 * @reentrant      Not reentrant.
 * @synchronous    TRUE
 * @trace       CPD-71421
 */
Std_ReturnType KeyM_CertGetStatus(KeyM_CertificateIdType CertId, KeyM_CertificateStatusType* Status);

#ifdef __cplusplus
}
#endif

#endif /*KEYM_H*/
