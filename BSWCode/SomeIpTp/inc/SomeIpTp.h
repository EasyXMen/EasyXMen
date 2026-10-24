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
 **  @file               : SomeIpTp.h
 **  @author             : HuRongbo
 **  @date               : 2020/07/07
 **  @vendor             : isoft
 **  @description        : SOMEIP Transport Protocol
 **
 ***********************************************************************************************************************/
/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author       Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V02.00.00 2020-07-20  HuRongbo          Initial Vertion.
 *  V02.00.01 2021-07-30  HuRongbo          Bug fix
 *  V02.00.02 2022-04-18  HuRongbo          Fix bug when rx first segment memcpy length error.
 *  V02.00.03 2022-05-22  HuRongbo          1> Fix bug when reveive single segment
 *                                          2> Fix bug of SeparationTimer
 *                                          3> Modify call PduR_SomeIpTpTxConfirmation() function parameter from
 *                                             FALSE/TRUE to E_OK/E_NOT_OK.
 *  V02.00.04 2022-06-08  HuRongbo          Modify SomeIpTpTxNSduNum to SomeIpTpRxNSduNum in function
 *                                          SomeIpTp_GetRxChannelCfgPtr() and SomeIpTp_GetRxNSduCfgPtr().
 *  V02.00.05 2022-08-21  HuRongbo          Modify rx first segment handle,Indicate to uppler layer when the frame is
 *                                          single frame (JIRA:CPT-6608)
 *  V02.00.06 2022-08-23  HuRongbo          Resolve the sending exception caused by incorrect availableData returned by
 *                                          the upper layer module (JIRA:CPT-6617).
 *  V02.00.07 2023-11-22  HuRongbo          1> Code execution efficiency optimization (JIRA:CPD-33590)
 *                                          2> QAC warning handle
 *  V02.00.08 2024-01-24  HuRongbo          Replace standard library functions to iSoft library functions.
 *  V02.00.09 2024-03-06  HuRongbo          QAC check with new rule set.
 *  V02.00.09 2025-06-24  Jianyu.yang       Developed according to R23-11 specification.
 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:Com<br>
  RuleSource:puhua-rule.rcf 3.0.0

    \li VL_MTR_SomeIpTp_QacSTMIF
      Reason: Special condition to ensure better performance.
      Risk:  Not easy to maintain
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_SomeIpTp_3415
      Reason: Right hand operand of '&&' or '||' is an expression with persistent side effects.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_SomeIpTp_1536
      Reason: The tag '%1s' is declared but not used within this project.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_SomeIpTp_1512
      Reason: Identifier '${name}' with external linkage has separate declarations in multiple translation units.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_SomeIpTp_1513
      Reason: Identifier '${name}' with external linkage has separate non-defining declarations in more than one
              location.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_SomeIpTp_STPTH .<br>
      Reason: Splitting the function into sub-functions will lead to an increase in the stack.
      Risk: The code is difficult to maintain.
      Prevention: Design and code review + clear structure and well-commented code.
 */
/* PRQA S 1512,1513 EOF */ /* VL_SomeIpTp_1512,VL_SomeIpTp_1513 */
#ifndef SOMEIPTP_H
#define SOMEIPTP_H

/* =================================================== inclusions =================================================== */
#include "SchM_SomeIpTp.h"
#include "SomeIpTp_Types.h"
#include "SomeIpTp_Lcfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */

#define SOMEIPTP_VENDOR_ID          62U
#define SOMEIPTP_MODULE_ID          177U
#define SOMEIPTP_INSTANCE_ID        0U
#define SOMEIPTP_AR_MAJOR_VERSION   4U
#define SOMEIPTP_AR_MINOR_VERSION   9U
#define SOMEIPTP_AR_PATCH_VERSION   0U
#define SOMEIPTP_H_SW_MAJOR_VERSION 2U
#define SOMEIPTP_H_SW_MINOR_VERSION 0U
#define SOMEIPTP_H_SW_PATCH_VERSION 1U

/* Development Errors */
#define SOMEIPTP_E_UNINIT        0x01U
#define SOMEIPTP_E_PARAM_POINTER 0x02U
#define SOMEIPTP_E_PARAM         0x03U
#define SOMEIPTP_E_INIT_FAILED   0x04U

/* Runtime Errors */
#define SOMEIPTP_E_MESSAGE_TYPE          0x04U
#define SOMEIPTP_E_INCONSISTENT_SEQUENCE 0x05U
#define SOMEIPTP_E_INCONSISTENT_HEADER   0x06U
#define SOMEIPTP_E_DISASSEMBLY_INTERRUPT 0x07U
#define SOMEIPTP_E_ASSEMBLY_INTERRUPT    0x08U

/* Service Id */
#define SOMEIPTP_SID_GETVERSIONINFO  0x01U
#define SOMEIPTP_SID_INIT            0x02U
#define SOMEIPTP_SID_TRANSMIT        0x49U
#define SOMEIPTP_SID_TRIGGERTRANSMIT 0x41U
#define SOMEIPTP_SID_RXINDICATION    0x42U
#define SOMEIPTP_SID_TXCONFIRMATION  0x40U
#define SOMEIPTP_SID_MAINFUNCTIONTX  0x03U
#define SOMEIPTP_SID_MAINFUNCTIONRX  0x04U

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

/**
 * @brief           Returns the version information of this module.
 * @param[out]      VersionInfo: Pointer to where to store the version information of this module
 * @return          None
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-78971
 */
#if (STD_ON == SOMEIPTP_VERSION_INFO_API)
#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"

void SomeIpTp_GetVersionInfo(Std_VersionInfoType* VersionInfo);
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"
#endif

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
/**
 * @brief           Initializes the SOME/IP TP module.
 * @param[in]       config: Base pointer to the configuration structure of the SOME/IP TP module.
 * @return          None
 * @synchronous     TRUE
 * @reentrant       FALS
 * @trace           CPD-78972
 */
void SomeIpTp_Init(const SomeIpTp_ConfigType* config);
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
/**
 * @brief          Requests transmission of a PDU.
 * @param[in]      TxPduId: Identifier of the PDU to be transmitted PduInfoPtr: Length of and pointer to
 *                          the PDU data and pointer to MetaData.
 * @return         Std_ReturnType
 * @retval         E_OK: Transmit request has been accepted.
 * @retval         E_NOT_OK: Transmit request has not been accepted.
 * @synchronous    TRUE
 * @reentrant      Reentrant for different PduIds.
 * @trace          CPD-78973
 */
Std_ReturnType SomeIpTp_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr);
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE_FAST
#include "SomeIpTp_MemMap.h"
/**
 * @brief          Within this API, the upper layer module (called module) shall check whether the available data fits
 * into the buffer size reported by PduInfoPtr->SduLength.If it fits,it shall copy its data into the buffer provided by
 *                 PduInfoPtr->SduDataPtr and update the length of the actual copied data in PduInfoPtr->SduLength. If
 * not, it returns E_NOT_OK without changing PduInfoPtr.
 * @param[in]      TxPduId:ID of the SDU that is requested to be transmitted
 * @param[inout]   PduInfoPtr: Contains a pointer to a buffer (SduDataPtr) to where the SDU data shall be copied, and
 * the available buffer size in SduLengh. On return, the service will indicate the length of the copied SDU data in
 * SduLength.
 * @return         Std_ReturnType
 * @retval         E_OK: SDU has been copied and SduLength indicates the number of copied bytes.
 * @retval         E_NOT_OK: No SDU data has been copied. PduInfoPtr must not be used since it may contain a NULL
 * pointer or point to invalid data.
 * @synchronous    TRUE
 * @reentrant      Reentrant for different PduIds.
 * @trace          CPD-78974
 */
Std_ReturnType SomeIpTp_TriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr);
#define SOMEIPTP_STOP_SEC_CODE_FAST
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE_FAST
#include "SomeIpTp_MemMap.h"
/**
 * @brief          Indication of a received PDU from a lower layer communication interface module.
 * @param[in]      RxPduId: ID of the received PDU.
 * @param[in]      PduInfoPtr: Contains the length (SduLength) of the received PDU, a pointer to a buffer (SduDataPt
 *                             containing the PDU, and the MetaData related to this PDU).
 * @return         None
 * @synchronous    TRUE
 * @reentrant      Reentrant for different PduIds.
 * @trace          CPD-78975
 */
void SomeIpTp_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr);
#define SOMEIPTP_STOP_SEC_CODE_FAST
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE_FAST
#include "SomeIpTp_MemMap.h"
/**
 * @brief          This service is called by the Ethernet Interface to report a transceiver link state change.
 * @param[in]      RxPduId: ID of the PDU that has been transmitted.
 * @param[in]      PduInfoPtr: E_OK: The PDU was transmitted. E_NOT_OK: Transmission of the PDU failed.
 * @return         None
 * @synchronous    TRUE
 * @reentrant      FALSE
 * @trace          CPD-78976
 */
void SomeIpTp_TxConfirmation(PduIdType TxPduId, Std_ReturnType result);
#define SOMEIPTP_STOP_SEC_CODE_FAST
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
/**
 * @brief          This function performs the processing of the AUTOSAR SOME/IP TP module's transmission activities.
 * @param[in]      None
 * @return         None
 * @return         None
 * @synchronous    TRUE
 * @reentrant      FALSE
 * @trace          CPD-78977
 */
void SomeIpTp_MainFunctionTx(uint8 someIpTpPartitionId);
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
/**
 * @brief         This function performs the processing of the AUTOSAR SOME/IP TP module's reception activities.
 * @param[in]      None
 * @return         None
 * @synchronous    TRUE
 * @reentrant      FALSE
 * @trace          CPD-78978
 */
void SomeIpTp_MainFunctionRx(uint8 someIpTpPartitionId);
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#ifdef __cplusplus
}
#endif

#endif
