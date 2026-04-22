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
 **  @file               : CanTp.h
 **  @author             : xue.han
 **  @date               : 2024/08/15
 **  @vendor             : isoft
 **  @description        : CAN Transport Layer header
 **
 ***********************************************************************************************************************/
/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author       Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V03.0.0  2024-08-15   xue.han      Initial version
 *           2024-12-10   xue.han      CPP-16636,Support CAN2.0 with CANFD channels
 *           2025-02-25   xue.han      CPT-12980,fix assemble PaddingData
 *                                     CPT-12807,delete duplicate break statements
 *           2025-03-07   xue.han      CPT-13166,CPT-13165,fix  received blocksize process
 *                                     CPT-13116,fix CanTp Exclusive Area
 *                                     CPT-13164,fix FF_DL calculate
 *  v3.0.1  2025-05-10    xue.han      CPT-14262,CPT-14284,fix data length check and assemble
 *          2025-05-10    xue.han      CPT-14320,fix FF interrupt and add STmin us process
 *          2025-05-11    xue.han      CPT-14384,fix STmin timer calculation
 *          2025-05-19    xue.han      CPT-14396,fix CanID CanFD flag storage when CanTpSupportCanWithCanFD enable
 *          2025-06-19    xue.han      CPT-14852,CPT-14629,fix SFLen Check
 *                                     CPT-14620,release RxFc when unexpected FF N-PDU arrival
                                       fix CanID CanFD flag storage when CanTpSupportCanWithCanFD enable
 *  v3.0.2  2025-09-16    xue.han      CPT-15622,fix SF_DL valid range check when received SF
 *                                     CPT-14950,fix FF_DL min check
 *                                     CPT-15628,fix BS calculate
 *                                     CPT-16089,fix CanTp_GetRxCurrentBs()
 *  v3.0.3  2025-10-27    xue.han      CPD-84862,Optimization of memory space
 *                        xue.han      CPT-16401,fix FF_DL overflow risk by checking CANTP_PDU_LENGTH_TYPE_MAX
 *                                     CPT-16371:exchanged SA and TA processes; CPT-17197: added const qualifiers.
 *  v3.0.4  2026-01-28    chao.sun     CPTASK-129,update CanTp_GetRxNSduCfg()
 ==================================================================================================================== */
/*===================================================================================================================*/
/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:CanTp<br>
  RuleSorce:puhua_rules-2024.12.rcf

    \li VL_CanTp_1505
      Reason: external interface
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_CanTp_DataConverteToWider
      Reason:Data conversion to a wider range is necessary for accumulation.
      Risk:No risk.
      Prevention:None.

    \li VL_CanTp_1504
      Reason: Multi-partition variable design requires
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_CanTp_3472
      Reason: Macro definitions for easy reuse.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_CanTp_3473
      Reason: Macro definitions for easy reuse.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_CanTp_4461
      Reason: Datatype conversion is necessary for functional usage
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_CanTp_3408
      Reason:Configuration parameter, design needs.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_CanTp_5016
      Reason: Parameter passing requires
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_CanTp_5017
      Reason: High cyclomatic complexity is reserved for better readability.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_CanTp_1503
      Reason: external interface
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_CanTp_2880
      Reason: the issue shall be avoided in actual running environment and configs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_CanTp_2991
      Reason: the issue shall be avoided in actual running environment and configs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_CanTp_2995
      Reason: the issue shall be avoided in actual running environment and configs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_CanTp_3673
      Reason: Parameter passing requires
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_CanTp_2003
      Reason: Avoid duplicate code calls.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_CanTp_5004
      Reason: Parameter passing requires
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_CanTp_3414
      Reason: specific keyword macro is necessary for unit testing
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_CanTp_1290
      Reason: Other Module Macro Definition References
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_CanTp_3678
      Reason: non-const type declaration is necessary depends on the config
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_CanTp_STVAR
      Reason: The total number of variables such as different channels and Sdu is related to the user configuration
      Risk: The code is difficult to maintain.
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_MTR_CanTp_STCAL
      Reason: The software structure of CanTp is defined according to the AUTOSAR standard,resulting in its metric
  being above the threshold
      Risk: Understandability and testability become overly complex
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_MTR_CanTp_STCYC
      Reason: Due to the implementation requirements of some diagnostic message functions, the functions were not
  separated into sub-functions
      Risk: Understandability and testability become overly complex
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_MTR_CanTp_STMIF
      Reason: Functions have different scenarios that need to be covered, which depends on the configuration of
  different channels, sdu and other configuration conditions, as well as some precompiled macros
      Risk: The code is difficult to maintain.
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_MTR_CanTp_STPTH
      Reason: Due to the implementation requirements of some diagnostic message functions, the functions were not
  separated into sub-functions
      Risk: Understandability and testability become overly complex
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_CanTp_3415
      Reason: Right hand operand of '&&' or '||' is an expression with persistent side effects.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_CanTp_2985
      Reason: Block information needs to be recorded in the current queue to sort multiple jobs.
      Risk: This value is calculated on this interface only when it is in the queue, no risk.
      Prevention:Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_CanTp_4397
      Reason: Macro Conveniently determining authenticity.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_CanTp_2996
      Reason: the issue shall be avoided in actual running environment and configs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_CanTp_2992
      Reason: The control statement has no effect, but is reserved for better readability.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_CanTp_2834
      Reason: The dividend is derived from the configuration and cannot be set to 0.
      Risk: No risk.
      Prevention: Ensure the normal operation of the project through unit testing

    \li VL_CanTp_4491
      Reason: DataType is determined by the configuration.
      Risk: Code redundancy and may difficult to maintain
      Prevention: Code inspection and test of the different configurations in the component test.

    \li VL_CanTp_3396
      Reason: Extra parentheses recommended. A binary operation is the operand of a conditional operator.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_CanTp_3132
      Reason: This cast is converting a complex floating point expression to an integral type.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_CanTp_1759
      Reason: Object '${name}' with internal linkage is not unique.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_CanTp_1531
      Reason: The object '%1s' is referenced in only one translation unit - but not the one in which it is defined.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_CanTp_1512
      Reason: Identifier '${name}' with external linkage has separate declarations in multiple translation units.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_CanTp_0499
      Reason: The arithmetic is safe.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_CanTp_4464
      Reason: A constant expression of 'essentially unsigned' type (%1s) is being c
              onverted to narrower unsigned type, '%2s' on assignment.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.
 */
#ifndef CANTP_H_
#define CANTP_H_

/* =================================================== inclusions =================================================== */
#include "ComStack_Types.h"
#include "CanTp_Cfg.h"
#include "CanTp_Lcfg.h"
#include "CanTp_PBcfg.h"
#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */
/**
 * @name CanTpPublishedInfoMacro
 * @{
 * @brief Published information elements
 */
#define CANTP_MODULE_ID   35u /**< Vendor Id */
#define CANTP_VENDOR_ID   62u /**< Module Id */
#define CANTP_INSTANCE_ID 0u  /**< InStance Id */
/** @} */
/**
 * @name CanTpAUTOSARSWSpecificationMacro
 * @{
 * @brief AUTOSAR Software Specification Version Information - decimal encoding
 */
#define CANTP_AR_RELEASE_MAJOR_VERSION    4U /**< AUTOSAR Software Specification Major version */
#define CANTP_AR_RELEASE_MINOR_VERSION    9U /**< AUTOSAR Software Specification Minor version */
#define CANTP_AR_RELEASE_REVISION_VERSION 0U /**< AUTOSAR Software Specification Patch version */
/** @} */

/**
 * @name CanTpComponentVersionMacro
 * @{
 * @brief Component Version Information - decimal encoding
 */
#define CANTP_SW_MAJOR_VERSION 3U /**< Software Major version */
#define CANTP_SW_MINOR_VERSION 0U /**< Software Minor version */
#define CANTP_SW_PATCH_VERSION 4U /**< Software Patch version */
/** @} */
/* ===================================================== macros ===================================================== */
/** This macro definition indicates the data copy, setup, and comparison of whether or not the library functions
 * provided by ISoft are used.STD_ON then indicates that they are used, and STD_OFF indicates that they are used with
 * the functions of the per-byte processing of the CanTp module. Using the library function provided by ISoft has better
 * performance. */
#ifndef CANTP_FEATURE_MEMCOPY_USED_ILIB
#define CANTP_FEATURE_MEMCOPY_USED_ILIB STD_OFF /**< Whether to use the library functions provided by ISoft.  */
#endif

/** This macro definition specifies whether the Consecutive Frame (CF) should be transmitted immediately during the
 * confirmation phase, without waiting for the completion of a main function cycle, when the STmin value in the received
 * Flow Control (FC) frame is 0. */
#ifndef CANTP_STMIN_IMMEDIATE_TX_CONFIRMATION
#define CANTP_STMIN_IMMEDIATE_TX_CONFIRMATION STD_ON /**< Whether transmitted immediately during the confirmation  */
#endif
/* ================================================= global variable ================================================*/
/**
 * @brief Pointer variable to store the determined PB configuration data
 */
extern const CanTp_ConfigType* CanTp_ConfigPtr;
/* ========================================= external function declarations ========================================= */

/**
 * @brief         Notify CanTp to receive CAN N-PDU frame.
 * @param[in]     RxPduId     : Identification of the received N-PDU.
 * @param[in]     PduInfoPtr  : Provides the received PDU length,a pointer to a buffer and the MetaData.
 * @reentrant     Reentrant for different PduIds. Non reentrant for the same PduId.
 * @synchronous   TRUE
 * @trace       CPD-73223
 */

extern void CanTp_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr);

/**
 * @brief         Notify CanTp to Confirm the transmission of CAN N-PDU frame.
 * @param[in]     TxPduId    : Identification of the transmitted N-PDU.
 * @param[in]     result     : result : E_OK: The PDU was transmitted. E_NOT_OK: Transmission of the PDU failed.
 * @reentrant     Reentrant for different Ids. Non reentrant for the same Id.
 * @synchronous   TRUE
 * @trace       CPD-73224
 */
extern void CanTp_TxConfirmation(PduIdType TxPduId, Std_ReturnType result);

/**
 * @brief         Service for basic initialization of CanTp module.
 * @param[in]     CfgPtr      : Pointer to configuration set in Variant Post-Build.
 * @reentrant     Non Reentrant.
 * @synchronous   TRUE
 * @trace       CPD-73225
 */
extern void CanTp_Init(const CanTp_ConfigType* CfgPtr);

#if (CANTP_VERSION_INFO_API == STD_ON)
/**
 * @brief         Returns the version information of this module.
 * @param[inout]    versioninfo : Pointer to where to store the version information of this module.
 * @reentrant     Reentrant.
 * @synchronous   TRUE
 * @trace       CPD-73226
 */
extern void CanTp_GetVersionInfo(Std_VersionInfoType* versioninfo);
#endif

/**
 * @brief         This function to shutdown the CanTp module.
 * @reentrant     Non Reentrant.
 * @synchronous   TRUE
 * @trace       CPD-73227
 */
extern void CanTp_Shutdown(void);

/* PRQA S 1512,1513 ++ */ /* VL_CanTp_1512,VL_QAC_MultiDeclaration */
/**
 * @brief         The transmission of segmented or unsegmented message is requested.
 * @param[in]     TxPduId      : Identification of the transmitted N-PDU.
 * @param[in]     PduInfoPtr   : Length of and pointer to the PDU data and pointer to MetaData.
 * @reentrant     Reentrant for different Ids. Non reentrant for the same Id.
 * @return        Std_ReturnType
 * @retval        E_OK         : Transmit request has been accepted.
 * @retval        E_NOT_OK     : Transmit request has not been accepted.
 * @synchronous   TRUE
 * @trace       CPD-73228
 */
extern Std_ReturnType CanTp_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr);

#if (CANTP_TC == STD_ON)
/**
 * @brief         Requests cancellation of an ongoing transmission of a PDU in a lower layer communication module.
 * @param[in]     TxPduId       : Identification of the PDU to be cancelled.
 * @reentrant     Reentrant for different Ids. Non reentrant for the same Id.
 * @return        Std_ReturnType
 * @retval        E_OK          : CancelTransmit request has been accepted.
 * @retval        E_NOT_OK      : CancelTransmit request has not been accepted.
 * @synchronous   TRUE
 * @trace       CPD-73229
 */
extern Std_ReturnType CanTp_CancelTransmit(PduIdType TxPduId);
#endif

#if (CANTP_RC == STD_ON)
/**
 * @brief         Requests cancellation of an ongoing reception of a PDU in a lower layer transport protocol module.
 * @param[in]     RxPduId        : Identification of the PDU to be cancelled.
 * @return        Std_ReturnType
 * @retval        E_OK           : CancelReceive request has been accepted.
 * @retval        E_NOT_OK       : CancelReceive request has not been accepted.
 * @reentrant     Reentrant for different Ids. Non reentrant for the same Id.
 * @synchronous   TRUE
 * @trace       CPD-73230
 */
extern Std_ReturnType CanTp_CancelReceive(PduIdType RxPduId);
#endif
/* PRQA S 1512,1513 -- */

#if (CANTP_CHANGE_PARAMETER == STD_ON)
/**
 * @brief         Request to change a specific transport protocol parameter (e.g. block size).
 * @param[in]     id             :Identification of the PDU which the parameter change shall affect.
 * @param[in]     parameter      : parameter change shall affect.
 * @param[in]     value          : The new value of the parameter.
 * @return        Std_ReturnType
 * @retval        E_OK           : The parameter was changed successfully.
 * @retval        E_NOT_OK       : The parameter change was rejected.
 * @reentrant     Non Reentrant.
 * @synchronous   TRUE
 * @trace       CPD-73231
 */
extern Std_ReturnType CanTp_ChangeParameter(PduIdType id, TPParameterType parameter, uint16 value);
#endif

#if (CANTP_READ_PARAMETER == STD_ON)
/**
 * @brief         This service is used to read the current value of reception parameters BS and STmin for a specified
 * N-SDU.
 * @param[in]     id             :Identifier of the received N-SDU on which the reception parameter are read.
 * @param[in]     parameter      : Specify the parameter to which the value has to be read (BS or STmin).
 * @param[out]    value          : Pointer where the parameter value will be provided.
 * @return        Std_ReturnType
 * @retval        E_OK           : request is accepted.
 * @retval        E_NOT_OK       : request is not accepted.
 * @reentrant     Non Reentrant.
 * @synchronous   TRUE
 * @trace       CPD-73232
 */
extern Std_ReturnType CanTp_ReadParameter(PduIdType id, TPParameterType parameter, uint16* value);
#endif

#ifdef __cplusplus
}
#endif
#endif
