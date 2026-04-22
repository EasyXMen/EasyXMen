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
 **  @file               : IpduM.h
 **  @author             : darren.zhang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : I-PDU Multiplexer
 **
 ***********************************************************************************************************************/
/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author       Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V02.00.01 2023-10-08  fupeng.yu     In IpduM_TransmitContainer, In case the PduR_IpduMTransmit() return E_NOT_OK,
 *                                      reset containedCnt of the container, Resolve the containedCnt of container
 *                                      increase when IpduM_Transmit() is called again.
 *  V02.00.02 2023-12-07  shengnan.sun  Handle frame loss.
 *  V02.00.03 2024-02-29  shengnan.sun  QAC_PH_2024 check.
 *  V02.00.04 2024-03-09  shengnan.sun  Support multiple partition feature.
 *  V02.01.00 2024-12-06  jianyu.yang   Developed according to R23-11 specification.
 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:Com<br>
  RuleSource:puhua-rule.rcf 3.0.0

    \li VL_IpduM_Division_by_zero
      Reason: To calculate relative postion.
      Risk: Division by zero.
      Prevention: Configuration tool will make sure this value is not zero.

    \li VL_IpduM_Continue_hasbeen_used
      Reason: To skip current invalid iterator in a loop.
      Risk: Code has not been used.
      Prevention: Somtime this iterator will represent an invalid item.

    \li VL_IpduM_Positive_integer_truncate
      Reason: Copy bits in a byte.
      Risk: Loss bits that outside a byte during shifting bit.
      Prevention: These bits are expected to discard.

    \li VL_IpduM_MacroMatchesOtherMacro
      Reason: Symbolic container name configured by user.
      Risk: Two macro identifiers are not unique within the first 63 characters.
      Prevention: Module Configuration Generator will keep unique name for each container.

    \li VL_IpduM_MacroMatchesOtherMacro0791
      Reason: Symbolic container name configured by user.
      Risk:  Identifiers - Implementation Limits This macro identifier is not distinguishable from other macro
  identifiers. Prevention: Module Configuration Generator will keep unique name for each container.

    \li VL_IpduM_MacroDefineSpecifierKeyword
      Reason: Make it easy for code test.
      Risk:  Sometimes it not easy to notice what it really means.
      Prevention: None.

    \li VL_IpduM_RedundantOperation
      Reason: This operation is redundant. The value of the result is always that of the left-hand operand.
      Risk:  Sometimes it not easy to notice what it really means.
      Prevention: None.

    \li VL_IpduM_2905
      Reason: Constant: Positive integer value truncated by cast to a smaller unsigned type.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_IpduM_4461
      Reason: Datatype conversion is necessary for functional usage
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_IpduM_1536
      Reason: The tag '%1s' is declared but not used within this project.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_IpduM_1512
      Reason: Identifier '${name}' with external linkage has separate declarations in multiple translation units.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_IpduM_1513
      Reason: Identifier '${name}' with external linkage has separate non-defining declarations in more than one
              location.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_IpduM_NotUsedWithinThisTranslationUnit
      Reason: Under some condition function will be used.
      Risk:  Not used within this translation unit.
      Prevention: None.

    \li VL_MTR_IpduM_QacSTPAR
      Reason: Keep prototype align with AUTOSAR SWS.
      Risk:  Make heap higher
      Prevention: These functions will not nested.

    \li VL_MTR_IpduM_QacSTMIF
      Reason: Special condition to ensure better performance.
      Risk:  Not easy to maintain
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_MTR_IpduM_QacSTVAR
      Reason: Implement all functions in one source file.
      Risk:  Not easy to maintain
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_MTR_IpduM_QacSTCAL
      Reason: To avoid nested funtions.
      Risk:  Not easy to maintain or understand
      Prevention: Design and code review, and have a clear structure and annotated code.
 */
/* PRQA S 1512,1513 EOF */ /* VL_IpduM_1512,VL_IpduM_1513 */
#ifndef IPDUM_H_
#define IPDUM_H_

/* =================================================== inclusions =================================================== */

#include "PduR_IpduM.h"
#include "ComStack_Types.h"
#include "IpduM_Internal.h"
#include "SchM_IpduM.h"
#include "IpduM_PBcfg.h"

#ifdef __cplusplus
extern "C" {
#endif
/* =============================================== version information ============================================== */

#define IPDUM_AR_RELEASE_MAJOR_VERSION    (4u)
#define IPDUM_AR_RELEASE_MINOR_VERSION    (9u)
#define IPDUM_AR_RELEASE_REVISION_VERSION (0u)

#define IPDUM_C_SW_MAJOR_VERSION 2u
#define IPDUM_C_SW_MINOR_VERSION 1u
#define IPDUM_C_SW_PATCH_VERSION 0u

/* ===================================================== macros ===================================================== */
/* Development Errors */
#define IPDUM_E_PARAM             (0x10u)
#define IPDUM_E_PARAM_POINTER     (0x11u)
#define IPDUM_E_UNINIT            (0x20u)
#define IPDUM_E_INIT_FAILED       (0x21u)
#define IPDUM_E_INVALID_PARTITION (0x60u)
/* Runtime Errors */
#define IPDUM_E_HEADER    (0x30u)
#define IPDUM_E_QUEUEOVFL (0x31u)
#define IPDUM_E_CONTAINER (0x32u)

/* Service ID */
#define IPDUM_SERVICE_ID_INIT            (0x00u)
#define IPDUM_SERVICE_ID_GETVERSIONINFO  (0x01u)
#define IPDUM_SERVICE_ID_TRANSMIT        (0x03u)
#define IPDUM_SERVICE_ID_RXINDICATION    (0x42u)
#define IPDUM_SERVICE_ID_TXCONFIRMATION  (0x40u)
#define IPDUM_SERVICE_ID_TRIGGERTRANSMIT (0x41u)
#define IPDUM_SERVICE_ID_MAINFUNCTIONRX  (0x11u)
#define IPDUM_SERVICE_ID_MAINFUNCTIONTX  (0x12u)

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

/**
 * @brief         Initializes the I-PDU Multiplexer.
 * @param[in]     config : Pointer to the AUTOSAR IpduM module's configuration data.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-60204
 */
void IpduM_Init(const IpduM_ConfigType* config);

#if (STD_ON == IPDUM_VERSION_INFO_API)
/**
 * @brief         Service returns the version information of this module.
 * @param[in]     versioninfo : Pointer to where to store the version.
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-60210
 */
void IpduM_GetVersionInfo(Std_VersionInfoType* versioninfo);
#endif

/**
 * @brief         Service is called by the PDU-Router to request a transmission.
 * @param[in]     TxPduId : ID of I-PDU to be transmitted.
 * @param[in]     PduInfoPtr : A pointer to a structure with I-PDU related.
 *                data that shall be transmitted: data length and pointer to I-SDU buffer.
 * @reentrant     Non Reentrant for the same PDU-ID. Reentrant for different PDU-ID.
 * @synchronous   TRUE
 * @trace         CPD-60203
 */
Std_ReturnType IpduM_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr);

/**
 * @brief         Performs the rx-processes of the activities that are not directly.
 *                initiated by the calls from PDU-R.
 * @param[in]     ipduMPartitionId : ID of partition where this function will running.
 * @reentrant     Reentrant for different partitions. Non reentrant for the same instance.
 * @synchronous   TRUE
 * @trace         CPD-60208
 */
void IpduM_MainFunctionTx(IpduM_PartitionIDType ipduMPartitionId);

/**
 * @brief         Performs the tx-processes of the activities that are not directly.
                  initiated by the calls from PDU-R.
 *                initiated by the calls from PDU-R.
 * @param[in]     ipduMPartitionId : ID of partition where this function will running.
 * @reentrant     Reentrant for different partitions. Non reentrant for the same instance.
 * @synchronous   TRUE
 * @trace         CPD-60209
 */
void IpduM_MainFunctionRx(IpduM_PartitionIDType ipduMPartitionId);

#ifdef __cplusplus
}
#endif

#endif
