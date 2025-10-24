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
 ************************************************************************************************************************
 **
 **  @file               : SecOC.h
 **  @author             : HuRongbo
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : API declaration and type definitions of SecOC
 **
 ***********************************************************************************************************************/
/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author       Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V02.02.00 2024-12-20  Jian.Jiang    1. Update code according AUTOSAR R23-11 Specification
 *  V02.02.01 2025-02-14  Jian.Jiang    1. Changes to the Match PduR Receiver Function
 *  V02.02.02 2025-02-18  Jian.Jiang    1. Add certified data macro switch for freshness values
 *  V02.02.03 2025-02-19  Jian.Jiang    1. Specification of QAC Notes and Explanation of Reasons
 *
 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:SecOC<br>
  RuleSorce:puhua_rules-2024.12.rcf

    \li VL_SecOC_5016
      Reason: Parameter passing requires
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_SecOC_5017
      Reason: High cyclomatic complexity is reserved for better readability.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_SecOC_0448
      Reason: Performing pointer arithmetic.
      Risk: no risk.
      Prevention: Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_SecOC_2986
      Reason: Allow some conditional statements to always equal the result on the left or right. The product code has
  high reusability, and in certain configuration scenarios, conditional statements are redundant.
      Risk: It may cause incomplete coverage of the test branch.
      Prevention: Use different test data to cover.

    \li VL_SecOC_2985
      Reason: Allow some conditional statements to always equal the result on the left or right. The product code has
  high reusability, and in certain configuration scenarios, conditional statements are redundant.
      Risk: It may cause incomplete coverage of the test branch.
      Prevention: Use different test data to cover.

    \li VL_SecOC_4397
      Reason: Macro Conveniently determining authenticity.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_SecOC_2834
      Reason:When the scheduling table has no entries, the modulo operation will not be executed.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_SecOC_3218
      Reason: To reduce the number of lines of code in a specific interface.
      Risk: The scope is within this file, but will not be accessed by other files, no risk.
      Prevention:Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_SecOC_3678
      Reason: The parameter will be modified, it could not be qulified as "const".
      Risk: This parameter is declared as an array may be misused.
      Prevention: Code inspection and test of the different configurations in the component test.

    \li VL_SecOC_3414
      Reason: specific keyword macro is necessary for unit testing
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_SecOC_1336
      Reason: Defining function pointer types
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_SecOC_0771
      Reason: Ability to quickly jump out of loops in different situations with different configurations.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_SecOC_3472
      Reason: Macro definitions for easy reuse.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_SecOC_5004
      Reason: Parameter passing requires
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_SecOC_0488
      Reason: Calculates the offset value of the array.
      Risk: The offset value is calculated with a fixed length, no risk.
      Prevention: Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_SecOC_2991
      Reason: The macro defines format requirements.
      Risk: Check whether errorId is valid before using it, no risk.
      Prevention: Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_SecOC_2995
      Reason: Functions defined out are not used in the project and have no impact on the integration application.
      Risk: No impact on integrated functions.
      Prevention: Provided to the application to use, the application use need to use the corresponding interface.

    \li VL_SecOC_3469
      Reason: A function should be used in preference to a function-like macro where they are interchangeable.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_SecOC_STTPP
      Reason: The number of lines of code is related to the user configuration, allowing
    dynamic code to violate this under extreme conditions
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_SecOC_STVAR
      Reason: The total number of variables is related to the user configuration, allowing
    dynamic code to violate this under extreme conditions
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_SecOC_STCAL
      Reason: The standard compliance vote was higher than the measurement threshold. In addition, a typical way to
    reduce STCAL is to deepen function nesting, which increases call stack usage and runtime
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_SecOC_STCYC
      Reason: Functionality is not separated into subfunctions because there are higher requirements for minimizing the
    stack and applying it to runtime use of the code
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_SecOC_STPTH
      Reason: The functions were not separated into sub-functions because there were higher requirements for
    minimizing the stack and applying them to the runtime usage of the code
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_SecOC_STST3
      Reason: Some function logics are overly complex. Splitting and implementing them will lead to reduced performance.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_SecOC_STMIF
      Reason: Functions handle specific tasks in the behavior of the entire component.
    Task has different scenarios that need to be covered, which depends on local conditions - this
    leads to deep nesting of the control structure.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_SecOC_STLIN
      Reason: Some function logics are overly complex. Splitting and implementing them will lead to reduced performance.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_SecOC_2755
      Reason: There is a lot of logic in the code and it is deeply nested, which makes the processing time longer.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_SecOC_STM19
      Reason: In principle, a function is not allowed to have multiple exit points,
    but it can help improve code performance in special circumstances.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_SecOC_STPAR
      Reason: The parameters of the function need.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_SecOC_3232
      Reason: A pointer array is needed.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_SecOC_1502
      Reason: Configuration variables design needs.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

 */

#ifndef SECOC_H
#define SECOC_H

/* =================================================== inclusions =================================================== */
#include "SecOC_Cfg.h"
#include "SecOC_Cbk.h"
#include "Rte_Csm_Type.h"
#include "Rte_SecOC_Type.h"
#include "SchM_SecOC.h"
#include "SecOC_PBcfg.h"

#ifdef __cplusplus
extern "C" {
#endif
/* =============================================== version information ============================================== */
/** Vendor ID */
#define SECOC_VENDOR_ID 62u
/** Module ID */
#define SECOC_MODULE_ID 150u
/** Instance ID */
#define SECOC_INSTANCE_ID 0u
/** AUTOSAR major version */
#define SECOC_H_AR_MAJOR_VERSION 4u
/** AUTOSAR minor version */
#define SECOC_H_AR_MINOR_VERSION 9u
/** AUTOSAR patch version */
#define SECOC_H_AR_PATCH_VERSION 0u
/** Software major version */
#define SECOC_H_SW_MAJOR_VERSION 2u
/** Software minor version */
#define SECOC_H_SW_MINOR_VERSION 2u
/** Software patch version */
#define SECOC_H_SW_PATCH_VERSION 3u

/* ===================================================== macros ===================================================== */
/** Service ID for SecOC_Init */
#define SECOC_INIT_ID 0x01u

/** Service ID for SecOC_DeInit */
#define SECOC_DEINIT_ID 0x05u
/** Service ID for SecOC_GetVersionInfo */
#define SECOC_GETVERSIONINFO_ID 0x02u
/** Service ID for SecOC_IfTransmit */
#define SECOC_IFTRANSMIT_ID 0x49u
/** Service ID for SecOC_TpTransmit */
#define SECOC_TPTRANSMIT_ID 0x49u
/** Service ID for SecOC_IfCancelTransmit */
#define SECOC_IFCANCELTRANSMIT_ID 0x4au
/** Service ID for SecOC_TpCancelTransmit */
#define SECOC_TPCANCELTRANSMIT_ID 0x4au
/** Service ID for SecOC_TpCancelReceive */
#define SECOC_TPCANCELRECEIVE_ID 0x4cu
/** Service ID for SecOC_VerifyStatusOverride */
#define SECOC_VERIFYSTATUSOVERRIDE_ID 0x0bu
/** Service ID for SecOC_SendDefaultAuthenticationInformation */
#define SECOC_SENDDEFAULTAUTHENTICATIONINFORMATION_ID 0x04u
/** Service ID for SecOC_RxIndication */
#define SECOC_RXINDICATION_ID 0x42u
/** Service ID for SecOC_TpRxIndication */
#define SECOC_TPRXINDICATION_ID 0x45u
/** Service ID for SecOC_TxConfirmation */
#define SECOC_TXCONFIRMATION_ID 0x40u
/** Service ID for SecOC_TpTxConfirmation */
#define SECOC_TPTXCONFIRMATION_ID 0x48u
/** Service ID for SecOC_TriggerTransmit */
#define SECOC_TRIGGERTRANSMIT_ID 0x41u
/** Service ID for SecOC_CopyRxData */
#define SECOC_COPYRXDATA_ID 0x44u
/** Service ID for SecOC_CopyTxData */
#define SECOC_COPYTXDATA_ID 0x43u
/** Service ID for SecOC_StartOfReception */
#define SECOC_STARTOFRECEPTION_ID 0x46u
/** Service ID for SecOC_MainFunctionRx */
#define SECOC_MAINFUNCTIONRX_ID 0x06u
/** Service ID for SecOC_MainFunctionTx */
#define SECOC_MAINFUNCTIONTX_ID 0x03u
/** Service ID for SecOC_CsmGenerateJobFinishedIndication */
#define SECOC_CSMGENERATEJOBFINISHEDINDICATION_ID 0x50u
/** Service ID for SecOC_CsmVerifyJobFinishedIndication */
#define SECOC_CSMVERIFYJOBFINISHEDINDICATION_ID 0x51u

/** Development Error: Parameter pointer is NULL */
#define SECOC_E_PARAM_POINTER 0x01u
/** Development Error: Module is uninitialized */
#define SECOC_E_UNINIT 0x02u
/** Development Error: Invalid PDU SDU ID */
#define SECOC_E_INVALID_PDU_SDU_ID 0x03u
/** Development Error: Crypto failure */
#define SECOC_E_CRYPTO_FAILURE 0x04u
/** Development Error: Initialization failed */
#define SECOC_E_INIT_FAILED 0x07u
/** Development Error: Error partition */
#define SECOC_E_ERROR_PARTITION 0x09u
/** Runtime Error: Freshness failure */
#define SECOC_E_FRESHNESS_FAILURE 0x08u

#if !defined(SECOC_LOCAL)
/** Local function definition */
#define SECOC_LOCAL static /* PRQA S 3414 */ /* VL_SecOC_3414 */
#endif

#if !defined(SECOC_LOCAL_INLINE)
/** Local inline function definition  */
#define SECOC_LOCAL_INLINE static inline
#endif

/* ================================================ type definitions ================================================ */
/**
 * @brief    States of the SecOC module.
 */
typedef enum SecOC_StateTag
{
    SECOC_UNINIT, /**< The SecOC module is uninitialized. @range 0 */
    SECOC_INIT    /**< The SecOC module is initialized. @range 1 */
} SecOC_StateType;

/**
 * @brief    Structure for the configuration of the SecOC module.
 */
typedef SecOC_PbConfigType SecOC_ConfigType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

/**
 * @brief          Initializes the the SecOC module.
 * @param[in]      config: Pointer to a selected configuration structure.
 * @reentrant      FALSE
 * @synchronous    TRUE
 * @trace       CPD-70334
 */
void SecOC_Init(const SecOC_ConfigType* config);

/**
 * @brief          This service stops the secure onboard communication.
 *                 All buffered I-PDU are removed and have to be obtained
 *                 again, if needed, after SecOC_Init has been called.
 * @reentrant      FALSE
 * @synchronous    TRUE
 * @trace       CPD-70335
 */
void SecOC_DeInit(void);

/**
 * @brief          Returns the version information of this module.
 * @param[inout]   versioninfo: Pointer to where to store the version
 *                              information of this module.
 * @reentrant      TRUE
 * @synchronous    TRUE
 * @trace       CPD-70336
 */
void SecOC_GetVersionInfo(Std_VersionInfoType* versioninfo);

/**
 * @brief          Requests transmission of a PDU.
 * @param[in]      TxPduId: Identifier of the PDU to be transmitted.
 * @param[in]      PduInfoPtr: Length of and pointer to the PDU data and
 *                             pointer to MetaData.
 * @return         Std_ReturnType
 * @retval         E_OK: Transmit request has been accepted.
 * @retval         E_NOT_OK: Transmit request has not been accepted.
 * @reentrant      Reentrant for different PduIds.
 * @synchronous    TRUE
 * @trace       CPD-70337
 */
Std_ReturnType SecOC_IfTransmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr);

/**
 * @brief          Requests transmission of a PDU.
 * @param[in]      TxPduId: Identifier of the PDU to be transmitted.
 * @param[in]      PduInfoPtr: Length of and pointer to the PDU data and
 *                             pointer to MetaData.
 * @return         Std_ReturnType
 * @retval         E_OK: Transmit request has been accepted.
 * @retval         E_NOT_OK: Transmit request has not been accepted.
 * @reentrant      Reentrant for different PduIds.
 * @synchronous    TRUE
 * @trace       CPD-70338
 */
Std_ReturnType SecOC_TpTransmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr);

/**
 * @brief          Requests cancellation of an ongoing transmission of a PDU
 *                 in a lower layer communication module.
 * @param[in]      TxPduId: Identification of the PDU to be cancelled.
 * @return         Std_ReturnType
 * @retval         E_OK: Cancellation was executed successfully by the
 *                       destination module.
 * @retval         E_NOT_OK: Cancellation was rejected by the destination
 *                           module.
 * @reentrant      Reentrant for different PduIds.
 * @synchronous    TRUE
 * @trace       CPD-70339
 */
Std_ReturnType SecOC_IfCancelTransmit(PduIdType TxPduId);

/**
 * @brief          Requests cancellation of an ongoing transmission of a PDU
 *                 in a lower layer communication module.
 * @param[in]      TxPduId: Identification of the PDU to be cancelled.
 * @return         Std_ReturnType
 * @retval         E_OK: Cancellation was executed successfully by the
 *                       destination module.
 * @retval         E_NOT_OK: Cancellation was rejected by the destination
 *                           module.
 * @reentrant      Reentrant for different PduIds.
 * @synchronous    TRUE
 * @trace       CPD-70340
 */
Std_ReturnType SecOC_TpCancelTransmit(PduIdType TxPduId);

/**
 * @brief          Requests cancellation of an ongoing reception of a PDU in
 *                 a lower layer transport protocol module.
 * @param[in]      RxPduId: Identification of the PDU to be cancelled.
 * @return         Std_ReturnType
 * @retval         E_OK: Cancellation was executed successfully by the
 *                       destination module.
 * @retval         E_NOT_OK: Cancellation was rejected by the destination
 *                           module.
 * @reentrant      FALSE
 * @synchronous    TRUE
 * @trace       CPD-70341
 */
Std_ReturnType SecOC_TpCancelReceive(PduIdType RxPduId);

/**
 * @brief          This service provides the ability to force specific
 *                 behaviour of SecOc: accept or drop an I-PDU with or
 *                 without performing the verification of authenticator or
 *                 independent of the authenticator verification result, and
 *                 force a specific result for SecOC_VerificationResultType
 *                 allowing additional fault handling in the application.
 * @param[in]      ValueID: ID of the Freshness Value used to control the
 *                 verification behaviour of all assigned Secured I-PDUs
 *                 according to the override Status.Or ValueID is the DataID
 *                 of a Secured I-PDU that shall be controlled by the
 *                 overrideStatus.
 * @param[in]      overrideStatus: Defines whether verification is executed
 *                 and whether the I-PDU is passed on, and for how long the
 *                 override is active.
 * @param[in]      numberOfMessagesToOverride: Number of sequential
 *                 verification to override when using a specific counter
 *                 for authentication verification.
 * @return         Std_ReturnType
 * @retval         E_OK: request successful
 * @retval         E_NOT_OK: request failed
 * @reentrant      Reentrant for different Freshness ValueIDs
 * @synchronous    TRUE
 * @trace       CPD-70342
 */
Std_ReturnType SecOC_VerifyStatusOverride(
    uint16                   ValueID,
    SecOC_OverrideStatusType overrideStatus,
    uint8                    numberOfMessagesToOverride);

#if defined(SECOC_DEFAULT_AUTHENTICATION_INFORMATION_PATTERN)
/**
 * @brief          The service provides the ability to enable the sending of
 *                 un-authenticated PDU to lower layer.
 * @param[in]      FreshnessValueID: ID of the Freshness Value for which
 *                 sending SecOCDefaultAuthenticationInformationPattern
 *                 should be enabled.
 * @param[in]      sendDefaultAuthenticationInformation: FALSE - sending
 *                 SecOCDefaultAuthenticationInformation Pattern shall be
 *                 disabled for given FreshnessValueID TRUE - sending
 *                 SecOCDefaultAuthenticationInformationPattern shall be
 *                 enabled for given FreshnessValueID.
 * @return         Std_ReturnType
 * @retval         E_OK: Cancellation was executed successfully by the
 *                       destination module.
 * @retval         E_NOT_OK: Cancellation was rejected by the destination
 *                           module.
 * @reentrant      Reentrant for different FreshnessValueIDs.
 * @synchronous    TRUE
 * @trace       CPD-70343
 */
Std_ReturnType
    SecOC_SendDefaultAuthenticationInformation(uint16 FreshnessValueID, boolean sendDefaultAuthenticationInformation);
#endif

#ifdef __cplusplus
}
#endif
#endif /* SECOC_H */
