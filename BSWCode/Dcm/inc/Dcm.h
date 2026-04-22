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
 **  @file               : Dcm.h
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm Public Function Declarations and Public DataType Definitions
 **
 ***********************************************************************************************************************/
/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author       Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V03.00.00 2024-02-23  xinrun.wang  Refactor and update to R23-11
 *            2025-03-18  xue.han      Set protocol state to 'sent' pre-transmission to guarantee data integrity.
 *            2025-03-20  xinrun.wang  fix Big Endian support
 *            2025-03-29  xue.han      fix buffersize handling in Dcm_UDS_DynDidDefinedDIDHandle()
 *            2025-03-31  xue.han      explicit ProtocolRowUsed validation in Dcm_StartOfReception()
 *  V03.00.01 2025-04-10  xue.han      CPT-13908,fix Dcm_SetDeauthenticatedRole()
 *            2025-04-15  xue.han      CPT-13939,fix S3 timer
 *  V03.00.02 2025-05-06  xue.han      CPT-14229,fix NRC handling for UDS service 0x27
 *                                     CPT-14230,fix NRC handling for UDS service 0x31
 *                                     CPT-14228,fix ProofOfOwnershipServerLen when calling Csm_SignatureGenerate
 *            2025-05-08  li.feng      CPT-14261,fix DCM_SUBFUNCTION_MASK
 *            2025-05-12  li.feng      CPT-14303,fix Service 31 Routine State Transition Logic
 *            2025-06-23  li.feng      CPT-14854,fix hen the DID doesn't support the 2E service, NRC 13 is prioritized
 *                                     in response instead of NRC 31.
 *                                     CPT-14853,fix Reading DIDs like F400 without OBD functionality configured
 *            2025-07-02  li.feng      CPT-15056,reading the setting of NRC within the DID interface is overwritten
 *            2025-08-05  xue.han      CPT-15468,added byte array to 16/24/32-bit integer (LE) conversion.
 *            2025-08-08  li.feng      CPT-15478,fix the parameter passing error in the Dem_SeclectDTC of the 14
 *                                     service.
 *            2025-08-19  li.feng      CPT-15518,fix abnormal periodic processing in the 2A service
 *                                     CPT-15513,fix the issue of abnormal dynamic length usage for Did signals
 *                                     CPT-15496,fix the error in the judgment of blocklength in the 34 service.
 *            2025-08-27  li.feng      CPT-15671,fix 0x19 page buffer implementation error
 *            2025-09-01  li.feng      CPT-15727,fix The functions of 0x2C and 0x86 are not fully implemented in
 *                                     accordance with the ISO standard
 *            2025-09-23  li.feng      CPD-84625,QAC issues
 * V03.00.03  2025-10-14  xue.han      CPT-15769.skip unsupported ranges unless a subsequent range has supported
 *                                     PID/MID/TID/INFOTYPE.
 *            2025-10-16  li.feng      CPT-15742.fix Sending a DID that does not support the session with 2c 01 responds
 *                                     with NRC10.
 *                                     CPT-15677.The 2E service request for RANGDID causes a crash.
 *            2025-10-16  li.feng      CPT-16098.When the PDID requested by the 2A service is a dynamic DID that is not
 *                                     defined, the DCM is expected to return 7F 2A 31.
 *            2025-10-17  xue.han      CPD-84863,Support dynamically adjust the shared RxTx buffer size based on request
 *                                     length.
 *            2025-10-24  li.feng      CPT-16000,DID config read failure from NVM, return NRC22.
 *            2025-10-27  li.feng      CPT-15522.When 2A 04 DID stops, periodic transmission does not cease.
 *            2025-10-31  li.feng      CPT-15521.clearing DDDID with 2C 04, it was not removed from the dispatch table.
 *            2025-11-04  li.feng      CPT-15516.Add reading of pDID to 0x2A.
 *            2025-11-05  li.feng      CPT-16223,Abnormal Processing of Service 28 and Service 22
 *            2025-11-07  li.feng      CPT-16293.Issue with Dcm_GetVin Implementation
 *            2025-11-10  li.feng      CPT-15697.Positive Response Suppression Configuration
 *            2025-11-12  li.feng      CPT-16313.No ECU control recovery in default session.
 *            2025-11-18  li.feng      CPT-16313.fix Code is missing in Dcm_Externals.h.
 *            2025-11-28  li.feng      CPT-16506,After executing the boot via 10 02, the NRC 0x78 fails to be
 *                                     transmitted continuously.
 *            2025-12-17  li.feng      CPT-17088.fix NRC 31 is returned when requesting to read NVM data.
 *            2025-12-18  li.feng      CPT-16947,CPT-17064.19 04/06 Read non-existent number error.
 *            2025-12-22  li.feng      CPT-17021.Fix: Compilation error (DCM_DDDID_CHECK_SOURCE unconfigured + 2A/2C
 *                                     enabled)
 *            2025-12-23  li.feng      CPT-17010.Fix:2E Variable DID Length Check Error
 *            2025-12-25  li.feng      CPT-17304.Fix: Positive Response Suppression Issue.
 *            2025-12-30  li.feng      CPT-17424.Fix: Request for service 0x2A failed due to incorrect transmission
 *                                                    cycle.
 *            2025-02-05  li.feng      CPT-18017.Fix: When the Service 22 reads multiple DIDs (Data Identifiers), among
 *                                               which some are configured with USE_BLOCK_ID, the total length of the
 *                                               response is incorrect.
 *            2026-02-12  li.feng      CPTASK-462.Fix: Optimize the execution time of Dcm_SatelliteMainFunction.
 *  V03.00.03 2026-04-04  tao.yu       CPT-18414,fix CommunicationMode calculate error in 04 and 05 subfunction of 0x28
 *                                     CPT-18344, fix NRC 33 in the 0x22 service
 *                                     CPT-18415, fix NRC 10 in the 0x22 service
 *            2026-04-05  tao.yu       CPT-18416, Fix the abnormal session state when the repair agreement is preempted.
 *                                     CPT-18417, fix error process in the 0x04 service
 *
 *====================================================================================================================
 */
#ifndef DCM_H_
#define DCM_H_

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:Dcm<br>
  RuleSorce:puhua_rules-2024.12.rcf

    \li VL_Dcm_3672
      Reason: function pointer needs to be assigned depends on configuration
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_3218
      Reason: file scoped variables only used in one function are necessary for functional usage
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_3446
      Reason: side effects are expected and will not affect the functionality
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_3315
      Reason: switch cases depend on configuration
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_2841
      Reason: the issue shall be avoided in actual running environment and configs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_2991
      Reason: the issue shall be avoided in actual running environment and configs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_2994
      Reason: the issue shall be avoided in actual running environment and configs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_2995
      Reason: the issue shall be avoided in actual running environment and configs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_2996
      Reason: the issue shall be avoided in actual running environment and configs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_2880
      Reason: the issue shall be avoided in actual running environment and configs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_2983
      Reason: the issue shall be avoided in actual running environment and configs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_4342
      Reason: enum conversion is necessary for functional usage
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_0624
      Reason: function parameter can be considered as the same via typedef
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_2120
      Reason: unsigned bit-field is required
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_1252
      Reason: suffixed constant comparison is necessary
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_0380
      Reason: Macro definitions are necessary
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_1257
      Reason: pduLength type will variy depnding on the config
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_0553
      Reason: The usage of this file depends on the config
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_4461
      Reason: Datatype conversion is necessary for functional usage
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_1271
      Reason: non-init expression for enum definition is necessary
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_5004
      Reason: pointer type definition is essential
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_3472
      Reason: function-like macros are necessary
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_3414
      Reason: specific keyword macro is necessary for unit testing
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_4152
      Reason: number and 'l' in callout function is possible
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_2784
      Reason: pointer param match with array param is necessary
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_4604
      Reason: param signal is necessary for IoHwAb operation
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_0288
      Reason: special characters in comments are necessary
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_3678
      Reason: non-const type declaration is necessary depends on the config
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_2986
      Reason: Allow some conditional statements to always equal the result on the left or right.
      Risk: It may cause incomplete coverage of the test branch.
      Prevention: Use different test data to cover.

    \li VL_Dcm_3494
      Reason: There may be a default value when judging
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_3208
      Reason: External variables and functions declaration
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_1037
      Reason: Array length may be zero
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_1441
      Reason: Passed as argument to a function parameter of enum type.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_4442
      Reason: Conversion to an enum type.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_Dcm_STECT
      Reason: This is violated under extreme conditions due to the total number of variables such as different services
  and protocols and user configuration
      Risk: The code is difficult to maintain and the data flow is complex
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_MTR_Dcm_STCYC
      Reason: Due to the implementation requirements of some diagnostic message functions, the functions were not
  separated into sub-functions
      Risk: Understandability and testability become overly complex
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_MTR_Dcm_STPAR
      Reason: The external API of the module is defined by the AUTOSAR standard, resulting in its metric being above the
  threshold
      Risk: For the target uC, the stack usage and runtime are too high
      Prevention: When testing the result code on the target uC, the user must check the stack usage in the project
  context

    \li VL_MTR_Dcm_CONF
      Reason: The format file of dcm needs to be processed for the interface of parameter/data type conversion, allowing
  for the reduction of useless comments
      Risk: None
      Prevention: None

    \li VL_MTR_Dcm_STPTH
      Reason: Due to the implementation requirements of some diagnostic message functions, the functions were not
  separated into sub-functions
      Risk: Understandability and testability become overly complex
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_MTR_Dcm_STCAL
      Reason: The software structure of Dcm is defined according to the AUTOSAR standard,resulting in its metric
  being above the threshold
      Risk: Understandability and testability become overly complex
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_MTR_Dcm_STMIF
      Reason: The return values of obtaining the security-level counters need to be judged. There are quite a few return
  value states, and each of them needs to be handled.
      Risk: Understandability and testability become overly complex
      Prevention: Design and code review.

    \li VL_Dcm_3415
      Reason: Right hand operand of '&&' or '||' is an expression with persistent side effects.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_3213
      Reason: The tag '%1s' is declared but not used within this project.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_1504
      Reason: Multi-partition variable design requires.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_3200
      Reason: '${name}' returns a value which is not being used.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_3677
      Reason: The elements of the array parameter '${name}' are not modified and so they could be qualified as 'const'.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_5017
      Reason: High cyclomatic complexity is reserved for better readability.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_Dcm_1503
      Reason: external interface
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_1512
      Reason: Identifier '${name}' with external linkage has separate declarations in multiple translation units.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_1712
      Reason: xternal identifiers have the same first '${n}' characters '${name}'.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_1533
      Reason: Configuration variables design needs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_5209
      Reason: The module ensures the correctness that unsigned long is used.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_4443
      Reason: A non-constant expression of 'essentially unsigned' type (%1s).
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_2985
      Reason: This operation is redundant. The value of the result is always that of the left-hand operand.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_2982
      Reason: The operation is not redundant, because it must be emptied.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dcm_4391
      Reason: In the code, it is necessary to convert a small range of unsigned integers into a larger range of
    unsigned integers.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.
*/
/* PRQA S 1271, 3451, 3449 EOF */ /* VL_Dcm_1271, VL_QAC_MultiDeclaration, VL_QAC_MultiDeclaration */
/* =================================================== inclusions =================================================== */
#include "ComStack_Cfg.h"
#include "ComStack_Types.h"
#include "Dcm_Cfg.h"
#if (STD_ON == DCM_UDS_0X29)
#include "Rte_KeyM_Type.h"
#include "Rte_Csm_Type.h"
#endif
#if (STD_ON == DCM_USE_BNDM)
#include "BndM.h"
#endif
#include "Rte_Dcm_Type.h"
#include "Dcm_PBCfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */
/**
 * @brief version info
 */
#define DCM_AR_RELEASE_MAJOR_VERSION    (4u) /**< Dcm AR Release Major Version */
#define DCM_AR_RELEASE_MINOR_VERSION    (9u) /**< Dcm AR Release Minor Version */
#define DCM_AR_RELEASE_REVISION_VERSION (0u) /**< Dcm AR Release Patch Version */
#define DCM_SW_MAJOR_VERSION            3U   /**<Software major version */
#define DCM_SW_MINOR_VERSION            0U   /**<Software minor version */
#define DCM_SW_PATCH_VERSION            3U   /**<Software patch version*/
#define DCM_VENDOR_ID                   62U  /**<AUTOSAR vendor ID (assigned by AUTOSAR) */
#define DCM_MODULE_ID                   53U  /**<Module ID for Diagnostic */
#define DCM_INSTANCE_ID                 0u   /**<Instance ID */
/* ===================================================== macros ===================================================== */
/**
 * @brief Development Errors
 */
#define DCM_E_INVALID_VALUE                                                                                          \
    0x02u /**< The Dcm is getting called with an invalid input parameter value or the Dcm has called an function and \
             this function returns an invalid out parameter or return value. */
#define DCM_E_INTERFACE_RETURN_VALUE   0x03u /**< Interface return-value is out of range */
#define DCM_E_UNINIT                   0x05u /**<Internal:Dcm not initialized */
#define DCM_E_PARAM                    0x06u /**<Dcm API function with invalid input parameter */
#define DCM_E_PARAM_POINTER            0x07u /**<Dcm API service invoked with NULL POINTER as parameter */
#define DCM_E_INIT_FAILED              0x08u /**< Dcm initialisation failed */
#define DCM_E_SET_PROG_CONDITIONS_FAIL 0x09u /**< Storing the ProgConditions failed */

/**
 * @brief Runtime Errors
 */
#define DCM_E_INTERFACE_TIMEOUT                                                                                    \
    0x01u /**< Interface: Timeout occurred during interaction with another module (e.g. maximum number of response \
             pending is reached) */
#define DCM_E_INTERFACE_BUFFER_OVERFLOW                                                                               \
    0x03u /**< Interface: Boundary check of buffers provided by the Dcm failed during interaction with another module \
             (application,Dem,PduR, etc.) */

/* Possible Errors */
#ifndef E_PROTOCOL_NOT_ALLOWED
#define E_PROTOCOL_NOT_ALLOWED 5u /**< conditions in application allows no further procession of protocol */
#endif

#ifndef E_REQUEST_NOT_ACCEPTED
#define E_REQUEST_NOT_ACCEPTED 8u /**< no response will be sent */
#endif

#ifndef DCM_E_PENDING
#define DCM_E_PENDING 10u /**< Request is not yet finished. Further call(s) required to finish. */
#endif

#ifndef DCM_E_COMPARE_KEY_FAILED
#define DCM_E_COMPARE_KEY_FAILED 11u /**< Key did not match. */
#endif

#ifndef DCM_E_FORCE_RCRRP
#define DCM_E_FORCE_RCRRP 12u /**< application request the transmission of a response Response Pending (NRC 0x78) */
#endif

/**
 * @brief API Service Id
 */
#define DCM_INIT_ID                   0x01u /**<Dcm_Init ID */
#define DCM_GETVERSIONINFO_ID         0x24u /**<Dcm_GetVersionInfo ID */
#define DCM_DEMTRIGGERONDTCSTATUS_ID  0x2Bu /**<Dcm_DemTriggerOnDTCStatus ID */
#define DCM_GETVIN_ID                 0x07u /**<Dcm_GetVin ID */
#define DCM_SETDEAUTHENTICATEDROLE_ID 0x79u /**<Dcm_SetDeauthenticatedRole ID */
#define DCM_GETSECURITYLEVEL_ID       0x0Du /**<Dcm_GetSecurityLevel ID */
#define DCM_GETSESCTRLTYPE_ID         0x06u /**<Dcm_GetSesCtrlType ID */
#define DCM_RESETTODEFAULTSESSION_ID  0x2Au /**<Dcm_ResetToDefaultSession ID */
#define DCM_SETACTIVEDIAGNOSTIC_ID    0x56u /**<Dcm_SetActiveDiagnostic ID */
#define DCM_STARTOFRECEPTION_ID       0x46u /**<Dcm_StartOfReception ID */
#define DCM_COPYRXDATA_ID             0x44u /**<Dcm_CopyRxData ID */
#define DCM_TPRXINDICATION_ID         0x45u /**<Dcm_TpRxIndication ID */
#define DCM_COPYTXDATA_ID             0x43u /**<DslInternal_CopyTxDataCheckParam ID */
#define DCM_TPTXCONFIRMATION_ID       0x48u /**<Dcm_TpTxConfirmation ID */
#define DCM_TXCONFIRMATION_ID         0x40u /**<Dcm_TxConfirmation ID */
#define DCM_COMM_NOCOMMODEENTERED     0x21u /**<Dcm_ComM_NoComModeEntered ID */
#define DCM_COMM_SILENTCOMMODEENTERED 0x22u /**<Dcm_ComM_SilentComModeEntered ID */
#define DCM_COMM_FULLCOMMODEENTERED   0x23u /**<Dcm_ComM_FullComModeEntered ID */
#define DCM_MAIN_FUNCTION_ID          0x25u /**<Dcm_MainFunction ID */
/* ================================================ type definitions ================================================ */
/**
 * @brief Base item type to transport status information.
 */
typedef enum
{
    DCM_E_OK                      = 0x00U, /**< This value is representing a successful operation @range 0x00. */
    DCM_E_ROE_NOT_ACCEPTED        = 0x06U, /**< ResponseOnOneEvent request is not accepted by DCM @range 0x06*/
    DCM_E_PERIODICID_NOT_ACCEPTED = 0x07U  /**< Periodic transmission request is not accepted by DCM @range 0x07*/
} Dcm_StatusType;

/**
 * @brief communication mode enum
 */
typedef enum
{
    DCM_ENABLE_RX_TX_NORM         = 0x00u, /**< Enable the Rx and Tx for normal communication @range 0x00*/
    DCM_ENABLE_RX_DISABLE_TX_NORM = 0x01u, /**< Enable the Rx and disable the Tx for normal communication @range 0x01*/
    DCM_DISABLE_RX_ENABLE_TX_NORM = 0x02u, /**< Disable the Rx and enable the Tx for normal communication @range 0x02*/
    DCM_DISABLE_RX_TX_NORMAL      = 0x03u, /**< Disable Rx and Tx for normal communication @range 0x03*/
    DCM_ENABLE_RX_TX_NM           = 0x04u, /**< Enable the Rx and Tx for network management communication @range 0x04*/
    DCM_ENABLE_RX_DISABLE_TX_NM =
        0x05u, /**< Enable Rx and disable the Tx for network management communication @range 0x05*/
    DCM_DISABLE_RX_ENABLE_TX_NM =
        0x06u, /**< Disable the Rx and enable the Tx for network management communication @range 0x06*/
    DCM_DISABLE_RX_TX_NM = 0x07u, /**< Disable Rx and Tx for network management communication @range 0x07*/
    DCM_ENABLE_RX_TX_NORM_NM =
        0x08u, /**< Enable Rx and Tx for normal and network management communication @range 0x08*/
    DCM_ENABLE_RX_DISABLE_TX_NORM_NM =
        0x09u, /**< Enable the Rx and disable the Tx for normal and network management communication @range 0x09*/
    DCM_DISABLE_RX_ENABLE_TX_NORM_NM =
        0x0Au, /**< Disable the Rx and enable the Tx for normal and network management communication @range 0x0A*/
    DCM_DISABLE_RX_TX_NORM_NM =
        0x0Bu /**< Disable Rx and Tx for normal and network management communication @range 0x0B*/
} Dcm_CommunicationModeType;

/**
 * @brief Return values of Callout Dcm_ReadMemory
 */
typedef enum
{
    DCM_READ_OK      = 0x00u, /**< Reading has been done @range 0x00*/
    DCM_READ_PENDING = 0x01u, /**< Reading is pending, another call is request to finalize the reading @range 0x01*/
    DCM_READ_FAILED  = 0x02u, /**< Reading has failed @range 0x02*/
    DCM_READ_FORCE_RCRRP =
        0x03u /**< Reading is pending, the Response pending transmission starts immediately @range 0x03*/
} Dcm_ReturnReadMemoryType;

/**
 * @brief Return type of callout Dcm_WriteMemory
 */
typedef enum
{
    DCM_WRITE_OK      = 0x00u, /**< Writing has been done @range 0x00*/
    DCM_WRITE_PENDING = 0x01u, /**< Writing is pending, another called is requested @range 0x01*/
    DCM_WRITE_FAILED  = 0x02u, /**< The writing has failed @range 0x02*/
    DCM_WRITE_FORCE_RCRRP =
        0x03u /**< Writing is pending, the Response pending transmission starts immediately @range 0x03*/
} Dcm_ReturnWriteMemoryType;

/**
 * @brief Allows the DCM to know if a diagnostic response shall be sent in the case of a jump from bootloader
 */
typedef enum
{
    DCM_COLD_START = 0x00u, /**< The ECU starts normally @range 0x00*/
    DCM_WARM_START = 0x01u  /**< The ECU starts from a bootloader jump @range 0x01*/
} Dcm_EcuStartModeType;

/**
 * @brief Used in Dcm_SetProgConditions() to allow the integrator to store relevant information prior to jumping to
 * bootloader jump due to ECUReset request.
 */
typedef struct
{
    uint16 ConnectionId;  /**< Unique id of the connection on which the request has been received @range 0..65535*/
    uint16 TesterAddress; /**< Source address of the received request if meta data is enabled, otherwise the value as
                             configured in DcmDslProtocolRxTesterSourceAddr @range 0..65535*/
    uint8   Sid;          /**< Service identifier of the received request @range 0..255*/
    uint8   SubFncId;     /**< Identifier of the received subfunction @range 0..255*/
    boolean ReprogramingRequest; /**< Set to true in order to request reprogramming of the ECU. @range 0..1*/
    boolean ApplUpdated;         /**< Indicate whether the application has been updated or not. @range 0..1*/
    boolean
        ResponseRequired; /**< Set to true in case the flashloader or application shall send a response. @range 0..1*/
} Dcm_ProgConditionsType;

/**
 * @brief Base type for diagnostic message item
 */
typedef uint8 Dcm_MsgItemType;

/**
 * @brief Base type for diagnostic message (request, positive or negative response)
 */
typedef Dcm_MsgItemType* Dcm_MsgType; /* PRQA S 5004 */ /* VL_Dcm_5004 */

/**
 * @brief Length of diagnostic message (request, positive or negative response). The maximum length is dependent of the
 * underlying transport protocol/media.
 */
typedef uint32 Dcm_MsgLenType;

/**
 * @brief Additional information on message request.
 */
typedef struct
{
    unsigned int reqType             : 1; /**< (Pos LSB+0) 0 = physical request 1 = functional request @range 0..1*/
    unsigned int suppressPosResponse : 1; /**< Position LSB+1 0 = no (do not suppress) 1 = yes (no positive response
                                      will be sent) @range 0..1*/
} Dcm_MsgAddInfoType;

/**
 * @brief This message context identifier can be used to determine the relation between request and response
 * confirmation.
 */
typedef uint8 Dcm_IdContextType;

/**
 * @brief This data structure contains all information which is necessary to process a diagnostic message from request
 * to response and response confirmation.
 */
typedef struct
{
    Dcm_MsgType reqData; /**< Request data, starting directly after service identifier (which is not part of this data)
                            @range NA*/
    Dcm_MsgLenType reqDataLen; /**< Request data length (excluding service identifier)  @range NA*/
    Dcm_MsgType resData; /**< Positive response data, starting directly after service identifier (which is not part of
                            this data)  @range NA. */
    Dcm_MsgLenType resDataLen; /**< Positive response data length (excluding service identifier) @range NA*/
    Dcm_MsgAddInfoType
        msgAddInfo; /**< Additional information about service request and response (see: Dcm_MsgAddInfo) @range NA*/
    Dcm_MsgLenType
        resMaxDataLen; /**< The maximal length of a response is restricted by the size of the buffer. The buffer size
                          can depend on the diagnostic protocol identifier which is assigned to this message, e. g. an
                          OBD protocol id can obtain other properties than the enhanced diagnostic protocol id. The
                          resMaxData Len is a property of the diagnostic protocol assigned by the DSL. The value does
                          not change during communication. It cannot be implemented as a constant, because it can differ
                          between different diagnostic protocols @range NA */
    Dcm_IdContextType idContext; /**< This message context identifier can be used to determine the relation between
                                    request and response        confirmation. This identifier can be stored within the
                                    application at request time, so that the        response can be assigned to the
                                    original request. Background: Within the confirmation, the message        context is
                                    no more valid, all message data is lost. You need an additional information to
                                    determine the request to which this confirmation belongs @range NA */
    PduIdType dcmRxPduId; /**< Pdu identifier on which the request was received. The PduId of the request can have
                             consequences for message processing. E. g. an OBD request will be received on the OBD PduId
                             and will be processed slightly different than an enhanced diagnostic request received on
                             the physical @range NA */
} Dcm_MsgContextType;

#ifndef DCM_INITIAL
#define DCM_INITIAL 0x00u /**< Initial call of the operation */
#endif

#ifndef DCM_PENDING
#define DCM_PENDING 0x01u /**< Indicates that a pending return has been done on the previous call of the operation */
#endif

#ifndef DCM_CANCEL
#define DCM_CANCEL 0x02u /**< Indicates that the Dcm requests to cancel the pending operation */
#endif

#ifndef DCM_FORCE_RCRRP_OK
#define DCM_FORCE_RCRRP_OK 0x03u /**< Confirm a response pending transmission Variation */
#endif

#ifndef DCM_POS_RESPONSE_SENT
#define DCM_POS_RESPONSE_SENT 0x04u /**< Indicates that a positive response has been sent successfully */
#endif

#ifndef DCM_POS_RESPONSE_FAILED
#define DCM_POS_RESPONSE_FAILED 0x05u /**< Indicates that a positive response has not been sent successfully */
#endif

#ifndef DCM_NEG_RESPONSE_SENT
#define DCM_NEG_RESPONSE_SENT 0x06u /**< Indicates that a negative response has been sent successfully */
#endif

#ifndef DCM_NEG_RESPONSE_FAILED
#define DCM_NEG_RESPONSE_FAILED 0x07u /**< Indicates that a negative response has not been sent successfully */
#endif
/**
 * @brief extended operation status enum
 */
typedef uint8 Dcm_ExtendedOpStatusType;
/* ========================================= external function declarations ========================================= */
/**
 * @brief         Service for basic initialization of DCM module.
 * @param[in]     ConfigPtr : Pointer to configuration set in Variant Post-Build.
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-60693
 */
void Dcm_Init(const Dcm_ConfigType* ConfigPtr);

/**
 * @brief         Returns the version information of this module
 * @param[out]    versionInfo : Pointer to where to store the version information of this module.
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-60694
 */
void Dcm_GetVersionInfo(Std_VersionInfoType* versionInfo);

/**
 * @brief         Function to get the VIN (as defined in SAE J1979-DA)
 * @param[out]    Data : Pointer to where to store the VIN
 * @return        Std_ReturnType
 * @retval        E_OK     : The Data pointer has been filled with valid VIN
 * @retval        E_NOT_OK : The default VIN will be used in the DoIP
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-60696
 */
Std_ReturnType Dcm_GetVin(uint8* Data);

#if (STD_ON == DCM_USE_BNDM)
/**
 * @brief         Called by BndM to indicate that a block write operation has finished.
 * @param[in]     BlockId : the requested blockId
 * @param[in]     result  : the result of the request
 * @return        void
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-60697
 */
void Dcm_BndMWriteBlockFinish(BndM_BlockIdType BlockId, BndM_ResultType result);
#endif

#if (STD_ON == DCM_UDS_0X29)
/**
 * @brief         Sets a new role used in deauthenticated state for that connection. The set role is valid until the
 *                connection switches into authenticated state or the ECU is reset.
 * @param[in]     connectionId        : Unique connection identifier identifiying the connection for which a
 *                                      deauthenticated roles is set.
 * @param[in]     deauthenticatedRole : New deauthenticated role that is assigned to that connection
 * @return        Std_ReturnType
 * @retval        E_OK : this value is always returned.
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-60701
 */
Std_ReturnType Dcm_SetDeauthenticatedRole(uint16 connectionId, const Dcm_AuthenticationRoleType deauthenticatedRole);
#endif

/* PRQA S 1512,1513 ++ */ /* VL_Dcm_1512,VL_QAC_MultiDeclaration */
/**
 * @brief         This function provides the active security level value.
 * @param[out]    SecLevel : Active Security Level value Conversion formula to calculate SecurityLevel out of tester
 *                           requested SecurityAccessType parameter: SecurityLevel = (SecurityAccessType + 1) / 2
 *                           Content of SecurityAccessType is according to "securityAccessType" parameter of
 *                           SecurityAccess request
 * @return        Std_ReturnType
 * @retval        E_OK: this value is always returned.
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-60702
 */
Std_ReturnType Dcm_GetSecurityLevel(Dcm_SecLevelType* SecLevel);

/**
 * @brief         This function provides the active session control type value.
 * @param[out]    SesCtrlType : Active Session Control Type value Content is according to "diagnosticSessionType"
 *                              parameter of DiagnosticSessionControl request
 * @return        Std_ReturnType
 * @retval        E_OK: this value is always returned.
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-60703
 */
Std_ReturnType Dcm_GetSesCtrlType(Dcm_SesCtrlType* SesCtrlType);

/**
 * @brief         The call to this function allows the application to reset the current session to Default session.
 *                Example: Automatic termination of an extended diagnostic session upon exceeding of a speed limit.
 * @return        Std_ReturnType
 * @retval        E_OK: this value is always returned.
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-60705
 */
Std_ReturnType Dcm_ResetToDefaultSession(void);

/**
 * @brief         Allows to activate and deactivate the call of ComM_DCM_ActiveDiagnostic() function.
 * @param[in]     active : If false Dcm shall not call ComM_DCM_ActiveDiagnostic(). If true Dcm will call
 *                         ComM_DCM_ActiveDiagnostic().
 * @return        Std_ReturnType
 * @retval        E_OK: this value is always returned.
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-60706
 */
Std_ReturnType Dcm_SetActiveDiagnostic(boolean active); /* PRQA S 0624 */ /* VL_Dcm_0624 */

/**
 * @brief         This function is called at the start of receiving an N-SDU. The N-SDU might be fragmented into
 *                multiple N-PDUs (FF with one or more following CFs) or might consist of a single N-PDU (SF). If the
 *                service is successful and BUFREQ_OK is returned, the service provides the currently available maximum
 *                buffer size. This function might be called in interrupt context.
 * @param[in]     id            : Identification of the I-PDU.
 * @param[in]     info          : Pointer to a PduInfoType structure containing the payload data (without protocol
 *                                information) and payload length of the first frame or single frame of a transport
 *                                protocol I-PDU reception, and the MetaData related to this PDU. If neither
 *                                first/single frame data nor MetaData are available, this parameter is set to NULL_PTR.
 * @param[in]     TpSduLength   : Total length of the N-SDU to be received.
 * @param[out]    bufferSizePtr : Available receive buffer in the receiving module. This parameter will be used to
 *                                compute the Block Size (BS) in the transport protocol module.
 * @return        Std_ReturnType
 * @retval        BUFREQ_OK       : Connection has been accepted. bufferSizePtr indicates the available receive buffer;
 *                                  reception is continued. If no buffer of the requested size is available, a receive
 *                                  buffer size of 0 shall be indicated by bufferSizePtr.
 * @retval        BUFREQ_E_NOT_OK : Connection has been rejected; reception is aborted. bufferSizePtr remains unchanged.
 * @retval        BUFREQ_E_OVFL   : No buffer of the required length can be provided; reception is aborted.
 *                                  bufferSizePtr remains unchanged.
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-60708
 */
BufReq_ReturnType Dcm_StartOfReception(
    PduIdType          id,
    const PduInfoType* info,
    PduLengthType      TpSduLength,
    PduLengthType*     bufferSizePtr);

/**
 * @brief         This function is called to provide the received data of an I-PDU segment (N-PDU) to the upper layer.
 *                Each call to this function provides the next part of the I-PDU data. The size of the remaining buffer
 *                is written to the position indicated by bufferSizePtr. This function might be called in interrupt
 *                context.
 * @param[in]     id            : Identification of the received I-PDU.
 * @param[in]     info          : Provides the source buffer (SduDataPtr) and the number of bytes to be copied
 *                                (SduLength). An SduLength of 0 can be used to query the current amount of available
 *                                buffer in the upper layer module. In this case, the SduDataPtr may be a NULL_PTR.
 * @param[out]    bufferSizePtr : Available receive buffer after data has been copied.
 * @return        Std_ReturnType
 * @retval        BUFREQ_OK       : Data copied successfully
 * @retval        BUFREQ_E_NOT_OK : Data was not copied because an error occurred.
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-60710
 */
BufReq_ReturnType Dcm_CopyRxData(PduIdType id, const PduInfoType* info, PduLengthType* bufferSizePtr);

/**
 * @brief         Called after an I-PDU has been received via the TP API, the result indicates whether the transmission
 *                was successful or not. This function might be called in interrupt context.
 * @param[in]     id     : Identification of the received I-PDU.
 * @param[in]     result : Result of the reception.
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-60711
 */
void Dcm_TpRxIndication(PduIdType id, Std_ReturnType result);

/**
 * @brief         This function is called to acquire the transmit data of an I-PDU segment (N-PDU). Each call to this
 *                function provides the next part of the I-PDU data. The size of the remaining data is written to the
 *                position indicated by availableDataPtr. This function might be called in interrupt context.
 * @param[in]     id               : Identification of the transmitted I-PDU.
 * @param[in]     info             : Provides the destination buffer (SduDataPtr) and the number of bytes to be copied
 *                                   (SduLength). If not enough transmit data is available, no data is copied by the
 *                                   upper layer module and BUFREQ_E_BUSY is returned. The lower layer module may retry
 *                                   the call. An SduLength of 0 can be used to indicate state changes in the retry
 *                                   parameter or to query the current amount of available data in the upper layer
 *                                   module. In this case, the Sdu DataPtr may be a NULL_PTR.
 * @param[in]     retry            : currently not being used
 * @param[out]    availableDataPtr : Indicates the remaining number of bytes that are available in the upper layer
 *                                   module's Tx buffer. availableDataPtr can be used by TP modules that support dynamic
 *                                   payload lengths (e.g. FrIsoTp) to determine the size of the following CFs.
 * @return        Std_ReturnType
 * @retval        BUFREQ_OK       : Data has been copied to the transmit buffer completely as requested.
 * @retval        BUFREQ_E_BUSY   : Request could not be fulfilled, because the required amount of Tx data is not
 *                                  available. The lower layern module may retry this call later on. No data has been
 *                                  copied.
 * @retval        BUFREQ_E_NOT_OK : Data has not been copied. Request failed.
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-60712
 */
BufReq_ReturnType
    Dcm_CopyTxData(PduIdType id, const PduInfoType* info, const RetryInfoType* retry, PduLengthType* availableDataPtr);

/**
 * @brief         This function is called after the I-PDU has been transmitted on its network, the result indicates
 *                whether the transmission was successful or not. This function might be called in interrupt context.
 * @param[in]     id     : Identification of the transmitted I-PDU.
 * @param[in]     result : Result of the transmission of the I-PDU.
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-60713
 */
void Dcm_TpTxConfirmation(PduIdType id, Std_ReturnType result);

/**
 * @brief         The lower layer communication interface module confirms the transmission of a PDU, or the failure to
 *                transmit a PDU. This function might be called in interrupt context.
 * @param[in]     TxPduId : ID of the PDU that has been transmitted.
 * @param[in]     result  : E_OK : The PDU was transmitted. E_NOT_OK: Transmission of the PDU failed.
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-60714
 */
void Dcm_TxConfirmation(PduIdType TxPduId, Std_ReturnType result);

#if (STD_ON == DCM_UDS_0X29)
/**
 * @brief         Can be called from Csm upon finishing an asynchronous job processing. The integrator will configure
 *                this name as callback function within the Csm ECUC configuration for asynchronous jobs. Only one such
 *                callback is available, the Dcm detects the job that has finished by evaluating the job parameter.
 * @param[in]     jobId  : JobId provided by the Csm, indicating the job that has finished.
 * @param[in]     result : Return value of the asynchronous job
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-60721
 */
Std_ReturnType Dcm_CsmAsyncJobFinished(uint32 jobId, Crypto_ResultType result);

/**
 * @brief         Can be called from Key upon finishing an asynchronous certificate verification. The integrator will
 *                configure this name as callback function within the KeyM ECUC configuration for asynchronous jobs.
 *                Only one such callback is available, the Dcm detects the certificate that has finished by evaluating
 *                the certId parameter.
 * @param[in]     CertID : Certificate identifier that has finished the verification
 * @param[in]     Result : Return value of the asynchronous job
 * @return        Std_ReturnType
 * @retval        E_OK: this value is always returned
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-60722
 */
Std_ReturnType Dcm_KeyMAsyncCertificateVerifyFinished(KeyM_CertificateIdType CertID, KeyM_CertificateStatusType Result);
#endif

#if (STD_ON == DCM_MULTICORE_ENABLED)
/**
 * @brief         Scheduled by SchM, running on different cores to deal with transmission for different controller
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-68736
 */
void Dcm_SatelliteMainFunction(void);
#endif
/* PRQA S 1512,1513 -- */

#ifdef __cplusplus
}
#endif
#endif
