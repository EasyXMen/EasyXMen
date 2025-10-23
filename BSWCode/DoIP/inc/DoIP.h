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
 **  @file               : DoIP.h
 **  @author             : hao.wen
 **  @date               : 2025/02/04
 **  @vendor             : isoft
 **  @description        : API of DoIP
 **
 ***********************************************************************************************************************/
/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version    Date          Author          Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V1.0.0     2019-08-30    YangBo          first edition.
 *  V2.0.0     2020-08-05    YangBo          Update to R19-11.
 *  V2.0.7     2021-06-16    zhiqiang.huang  add tx queue buffer.
 *  V2.1.0     2025-02-03    hao.wen         Refactor.
==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:CanNm<br>
  RuleSorce:puhua_rules-2024.12.rcf

    \li VL_DoIP_ArithNullptr
      Reason: Use [] on pointer.
      Risk: Pointer is NULL_PTR.
      Prevention: Pointers are actually configurations and cannot be null.

    \li VL_DoIP_DerefInvalidPtr
      Reason: Dereference pointer.
      Risk:Pointer is NULL_PTR.
      Prevention:The pointer corresponds to the configuration one by one and is not empty.

    \li VL_DoIP_ComputeInvalidPtr
      Reason: Use [] on pointer.
      Risk:Pointer is NULL_PTR.
      Prevention:The pointer corresponds to the configuration one by one and is not empty.

    \li VL_DoIP_UseUninitValue
      Reason: Use uninited pointer.
      Risk:Pointer is NULL_PTR.
      Prevention:A value is assigned before use.

     \li VL_DoIP_PartialInit
      Reason:Pudinfo->metadataptr is NULL_PTR.
      Risk: None.
      Prevention:No need to use Metadataptr.

    \li VL_DoIP_RefeOneFile
      Reason:Not defined in the usage source file.
      Risk: None.
      Prevention:Due to the division of document responsibilities.

    \li VL_DoIP_NullStatement
      Reason:SchM's API is NULL.
      Risk: None.
      Prevention:For test.

    \li VL_DoIP_WithoutPreDeclaration
      Reason: No declaration.
      Risk: None.
      Prevention:There are actually declarations.

     \li VL_DoIP_NotCastEssentialType
      Reason: Type conversion.
      Risk: None.
      Prevention:No solution found for this qac.

     \li VL_DoIP_NotDefineInUseFile
      Reason: Not defined in the usage source file.
      Risk: None.
      Prevention:Due to the division of document responsibilities.

     \li VL_DoIP_OnlyRefeByOneFunc
      Reason:Used in only one function.
      Risk: None.
      Prevention:Other usage scenarios are not open.

    \li VL_DoIP_UsedBeforModify
      Reason:Used before modify.
      Risk: None.
      Prevention:That's how the logic works.

    \li VL_DoIP_CouldReplacedByFuncCall
      Reason:Can replace macros with function calls.
      Risk: None.
      Prevention:None.

    \li VL_DoIP_DefinedInHeaderFile
      Reason:Define the function in the header file.
      Risk: None.
      Prevention:None.

    \li VL_DoIP_TestAlwayFalse
      Reason:The test result is always FALSE.
      Risk: None.
      Prevention:Prevents user calling with integers.

    \li VL_DoIP_FlexibleArray
      Reason:Misidentify flexible array.
      Risk: None.
      Prevention:None.

    \li VL_DoIP_UseConstInitStaticVar
      Reason:Need const var init static var.
      Risk: None.
      Prevention:None.

    \li VL_DoIP_3451
      Reason: multiple declaration is necessary for RTE
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_DoIP_3449
      Reason: multiple declaration is necessary for RTE
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_DoIP_3414
      Reason: specific keyword macro is necessary for unit testing
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_DoIP_1039
      Reason: Configure the length of the generated array, it depends on the configuration, may be configured as 1
              length.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_DoIP_5016
      Reason: Parameter passing requires
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_DoIP_3408
      Reason:Configuration parameter, design needs.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_DoIP_1334
      Reason: Only parameter name is different.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_DoIP_1252
      Reason: suffixed constant comparison is necessary
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_DoIP_1258
      Reason: Necessary data processing
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_DoIP_0686
     Reason: Array has fewer initializers than its declared size.
     Risk: No risk.
     Prevention: Functional reliability guaranteed by design.

    \li VL_DoIP_1503
      Reason: external interface
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_DoIP_4558
      Reason: Expression logical operator
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_DoIP_3206
      Reason: Variables are used in different configurations due to complex precompilation logic.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_DoIP_STCAL
      Reason: The software structure of DoIP is defined according to the AUTOSAR standard,resulting in its metric
  being above the threshold
      Risk: The code is difficult to maintain.
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_MTR_DoIP_STMIF
      Reason: Functions have different scenarios that need to be covered, which depends on the configuration of
  different channels, connections, prorocols, routings and other configuration conditions, as well as some precompiled
  macros
      Risk: The code is difficult to maintain.
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_DoIP_1536
      Reason: The tag '%1s' is declared but not used within this project.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_DoIP_3415
      Reason: Right hand operand of '&&' or '||' is an expression with persistent side effects.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_DoIP_1512
      Reason: Identifier '${name}' with external linkage has separate declarations in multiple translation units.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_DoIP_4404
      Reason: An expression of 'essentially Boolean' type (%1s) is being converted to unsigned type, '%2s' on
  assignment. Risk: No risk. Prevention: Ensure that the project is working properly through unit testing.

    \li VL_DoIP_4115
      Reason: Operand of logical && or || operator is not an 'essentially Boolean' expression.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_DoIP_2016
      Reason: This 'switch' statement 'default' clause is empty.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_DoIP_1297
      Reason: Definitions from other modules are used
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_DoIP_3397
      Reason: The code is designed so
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_DoIP_2907
      Reason: Apparent: Positive integer value truncated by cast to a smaller unsigned type.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_DoIP_2844
      Reason: When using arrays, index values can be out of range, resulting in access errors.
      Risk: The passed array index value is obtained only within the configured block range, no risk.
      Prevention: Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_DoIP_1338
      Reason: Conversion of internal and external ids requires
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_DoIP_4603
      Reason: Dereference pointer.
      Risk:Pointer is NULL_PTR.
      Prevention:The pointer corresponds to the configuration one by one and is not empty.

    \li VL_DoIP_1501
      Reason: The function '${name}' is declared but is not used within this project.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_DoIP_1753
      Reason: The function '${name}' with external linkage is declared but not defined within this project.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_DoIP_1513
      Reason: Identifier '${name}' with external linkage has separate non-defining declarations in more than one
  location. Risk: No risk. Prevention: Functional reliability guaranteed by design.

    \li VL_DoIP_0624
      Reason: function parameter can be considered as the same via typedef
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_DoIP_1707
      Reason: Function '${name}' is not using the same aliases.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_DoIP_1709
      Reason: Function '${name}' is not using the same parameter names.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

 */

#ifndef DOIP_H_
#define DOIP_H_

/* =================================================== inclusions =================================================== */

#include "ComStack_Types.h"
#include "DoIP_Cfg.h"
#include "DoIP_PBcfg.h"
#include "DoIP_ConfigType.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */

/* Published information elements */
#define DOIP_VENDOR_ID                   (62u)  /**< Value of vendor */
#define DOIP_MODULE_ID                   (173u) /**< Value of module */
#define DOIP_INSTANCE_ID                 (0u)   /**< Value of instance */
#define DOIP_AR_RELEASE_MAJOR_VERSION    (4u)   /**< Value of Autosar major version */
#define DOIP_AR_RELEASE_MINOR_VERSION    (9u)   /**< Value of Autosar minor version */
#define DOIP_AR_RELEASE_REVISION_VERSION (0u)   /**< Value of Autosar patch version */
#define DOIP_SW_MAJOR_VERSION            (2u)   /**< Value of module major version */
#define DOIP_SW_MINOR_VERSION            (1u)   /**< Value of module minor version */
#define DOIP_SW_PATCH_VERSION            (0u)   /**< Value of module patch version */

/* ===================================================== macros ===================================================== */

#if (STD_ON == DOIP_DEV_ERROR_DETECT)

#define DOIP_E_UNINIT             (0x01u) /**< The DoIP is not initialized */
#define DOIP_E_PARAM_POINTER      (0x02u) /**< Pointer is NULL_PTR */
#define DOIP_E_INVALID_PDU_SDU_ID (0x03u) /**< Invalid PDU ID */
#define DOIP_E_INVALID_PARAMETER  (0x04u) /**< Invalid parameter */

/* ----- API service IDs ----- */
#define DOIP_SID_TPTRANSMIT             (0x03u) /**< DoIP_TpTransmit */
#define DOIP_SID_TPCANCELTRANSMIT       (0x04u) /**< DoIP_TpCancelTransmit */
#define DOIP_SID_TPCANCELRECEIVE        (0x05u) /**< DoIP_TpCancelReceive */
#define DOIP_SID_IFTRANSMIT             (0x49u) /**< DoIP_IfTransmit */
#define DOIP_SID_IFCANCELTRANSMIT       (0x4au) /**< DoIP_IfCancelTransmit */
#define DOIP_SID_INIT                   (0x01u) /**< DoIP_Init */
#define DOIP_SID_GETVERSIONINFO         (0x00u) /**< DoIP_GetVersionInfo */
#define DOIP_SID_SOADTPCOPYTXDATA       (0x43u) /**< DoIP_SoAdTpCopyTxData */
#define DOIP_SID_SOADTPTXCONFIRMATION   (0x48u) /**< DoIP_SoAdTpTxConfirmation  */
#define DOIP_SID_SOADTPCOPYRXDATA       (0x44u) /**< DoIP_SoAdTpCopyRxData  */
#define DOIP_SID_SOADTPSTARTOFRECEPTION (0x46u) /**< DoIP_SoAdTpStartOfReception */
#define DOIP_SID_SOADTPRXINDICATION     (0x45u) /**< DoIP_SoAdTpRxIndication  */
#define DOIP_SID_SOADIFRXINDICATION     (0x42u) /**< DoIP_SoAdIfRxIndication */
#define DOIP_SID_SOADIFTXCONFIRMATION   (0x40u) /**< DoIP_SoAdIfTxConfirmation */
#define DOIP_SID_SOCONMODECHG           (0x0bu) /**< DoIP_SoConModeChg */
#define DOIP_SID_LOCALIPADDRASSIGNMENT  (0x0cu) /**< DoIP_LocalIpAddrAssignmentChg  */
#define DOIP_SID_ACTIVATIONLINESWITCH   (0x0fu) /**< DoIP_ActivationLineSwitch */
#define DOIP_SID_MAINFUNCTION           (0x02u) /**< DoIP_MainFunction */
#define DoIP_SID_MAINFUNCTIONHIGHFEQ    (0x02u) /**< DoIP_MainFunction_HighFrequency */

#endif

/* ========================================= external function declarations ========================================= */

/**
 * @brief       This service initializes all global variables of the DoIP module. After return of
 *              this service the DoIP module is operational.
 * @param[in]   doipConfigPtr: PB configuration.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
void DoIP_Init(const DoIP_ConfigType* doipConfigPtr);

/**
 * @brief       Returns the version information of this module.
 * @param[out]  versionInfo: Pointer to where to store the version information of this module.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
/* PRQA S 1501,1753 ++ */ /* VL_DoIP_1501,VL_DoIP_1753 */
void DoIP_GetVersionInfo(Std_VersionInfoType* versionInfo);
/* PRQA S 1501,1753 -- */

/**
 * @brief       This function is used to notify the DoIP on a switch of the DoIPActivationLine
 * @param[inout]  active: Specifies activate or deactivate,and return status of DoIP.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
/* PRQA S 1512,1513 ++ */           /* VL_DoIP_1512,VL_DoIP_1513 */
/* PRQA S 3449,3451,0624,1707 ++ */ /* VL_DoIP_3449,VL_DoIP_3451,VL_DoIP_0624,VL_DoIP_1707 */
void DoIP_ActivationLineSwitch(boolean* active);
/* PRQA S 3449,3451,0624,1707 -- */

/**
 * @brief       This service is called to request the transfer data from the PduRouter to the SoAd.
 *              It is used to indicate the transmission which will be performed in the DoIP_Mainfunction.
 * @param[in]   pdurTxPduId: DoIP unique identifier of the PDU to be transmitted by the PduR
 * @param[in]   pduInfoPtr: Tx Pdu information structure which contains the length of the DoIPTxMessage.
 * @return      Std_ReturnType
 * @retval      E_OK: Get The request has been accepted.
 * @retval      E_NOT_OK: The request has not been accepted.
 * @reentrant   Reentrant for different PduIds. Non reentrant for the same PduId.
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
/* PRQA S 1709 ++ */ /* VL_DoIP_1709 */
Std_ReturnType DoIP_TpTransmit(PduIdType pdurTxPduId, const PduInfoType* pduInfoPtr);

/**
 * @brief       This service primitive is used to cancel the transfer of pending DoIPPduRTxIds. The connection is
 *              identified by DoIPPduRTxId. When the function returns, no transmission is in progress anymore
 *              with the given DoIPPduRTxId identifier.
 * @param[in]   pdurTxPduId: DoIP unique identifier ofthe PDU to be transmitted by the PduR
 * @return      Std_ReturnType
 * @retval      E_OK: Transmit cancellation request of the specified DoIPPduRTxId is accepted.
 * @retval      E_NOT_OK: The transmit cancellation request of the DoIPPduRTxId has been rejected.
 * @reentrant   Reentrant for different PduIds. Non reentrant for the same PduId.
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
Std_ReturnType DoIP_TpCancelTransmit(PduIdType pdurTxPduId);

/**
 * @brief       By calling this API with the corresponding DoIPPduRRxId the currently ongoing data reception
 *              is terminated immediately. When the function returns, no reception is in progress anymore with
 *              the given DoIPPduRRxId identifier.
 * @param[in]   pdurRxPduId: DoIP unique identifier of the PDU for which reception shall be canceled by the PduR
 * @return      Std_ReturnType
 * @retval      E_OK: Reception was canceled successfully.
 * @retval      E_NOT_OK:  Reception was not canceled.
 * @reentrant   Reentrant for different PduIds. Non reentrant for the same PduId.
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
Std_ReturnType DoIP_TpCancelReceive(PduIdType pdurRxPduId);

/**
 * @brief       Requests transmission of an I-PDU.
 * @param[in]   pdurTxPduId: Identification of the I-PDU.
 * @param[in]   pduInfoPtr: Provides the destination buffer (SduDataPtr) and the number of bytes to be copied
 * (SduLength).
 * @return      Std_ReturnType
 * @retval      E_OK: Request is accepted by the destination module.
 * @retval      E_NOT_OK:  Request is not accepted by the destination module.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
Std_ReturnType DoIP_IfTransmit(PduIdType pdurTxPduId, const PduInfoType* pduInfoPtr);

/**
 * @brief       Requests cancellation of an ongoing transmission of an I-PDU in a lower layer communication interface
 * module.
 * @param[in]   pdurTxPduId: Identification of the I-PDU to be cancelled.
 * @return      Std_ReturnType
 * @retval      E_OK: Cancellation was executed successfully by the destination module.
 * @retval      E_NOT_OK:  Cancellation was rejected by the destination module.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
Std_ReturnType DoIP_IfCancelTransmit(PduIdType pdurTxPduId);
/* PRQA S 1709 -- */

/**
 * @brief       Schedules the Diagnostic over IP module. (Entry point for scheduling)
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
/* PRQA S 3449,3451 ++ */ /* VL_DoIP_3449,VL_DoIP_3451 */
void DoIP_MainFunction(void);
/* PRQA S 3449,3451 -- */
/* PRQA S 1512,1513 -- */
/**
 * @brief       Schedules the Diagnostic over IP module. (Entry point for scheduling) for hight frequency tasks.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
#if (DOIP_HIGH_FREQ_TASK_SUPPORT == TRUE)
extern void DoIP_MainFunction_HighFrequency(void);
#endif

#ifdef __cplusplus
}
#endif
#endif
