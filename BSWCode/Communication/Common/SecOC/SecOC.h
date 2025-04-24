/* PRQA S 3108++ */
/**
 * Copyright (C) 2024 Isoft Infrastructure Software Co., Ltd.
 * SPDX-License-Identifier: LGPL-2.1-only-with-exception OR  LicenseRef-Commercial-License
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the
 * GNU Lesser General Public License as published by the Free Software Foundation; version 2.1.
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License along with this library;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * or see <https://www.gnu.org/licenses/>.
 *
 * Alternatively, this file may be used under the terms of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License, in which case the provisions of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License shall apply instead of those of the GNU Lesser General Public License.
 *
 * You should have received a copy of the Isoft Infrastructure Software Co., Ltd.  Commercial License
 * along with this program. If not, please find it at <https://EasyXMen.com/xy/reference/permissions.html>
 */
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : SecOC.h                                                     **
**                                                                            **
**  Created on  :                                                             **
**  Author      : HuRongbo                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*  <VERSION>    <DATE>       <AUTHOR>   <REVISION LOG>
 *  V1.0.0       2020-08-19   HuRongbo   R19_11 SecOC initial version.
 *  V2.0.0       2020-06-17   HuRongbo   Bug fix.
 *  V2.0.1       2020-01-18   HuRongbo   Move callback function to SecOC_Cbk.h
 */

#ifndef SECOC_H
#define SECOC_H

#include "SecOC_Types.h"
#include "SecOC_Cfg.h"
#include "SecOC_Cbk.h"
#include "Rte_Csm_Type.h"
#include "Rte_SecOC_Type.h"
#include "SchM_SecOC.h"
#if (SECOC_CFUNC == SECOC_QUERY_FRESHNESS_VALUE)
#include "SecOC_Callout.h"
#else
#include "Rte_SecOC.h"
#endif

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define SECOC_VENDOR_ID          62u
#define SECOC_MODULE_ID          150u
#define SECOC_INSTANCE_ID        0u
#define SECOC_H_AR_MAJOR_VERSION 4u
#define SECOC_H_AR_MINOR_VERSION 5u
#define SECOC_H_AR_PATCH_VERSION 0u
#define SECOC_H_SW_MAJOR_VERSION 2u
#define SECOC_H_SW_MINOR_VERSION 1u
#define SECOC_H_SW_PATCH_VERSION 13u

/* Service Id */
#define SECOC_INIT_ID                                 0x01u
#define SECOC_DEINIT_ID                               0x05u
#define SECOC_GETVERSIONINFO_ID                       0x02u
#define SECOC_IFTRANSMIT_ID                           0x49u
#define SECOC_TPTRANSMIT_ID                           0x49u
#define SECOC_IFCANCELTRANSMIT_ID                     0x4au
#define SECOC_TPCANCELTRANSMIT_ID                     0x4au
#define SECOC_TPCANCELRECEIVE_ID                      0x4cu
#define SECOC_VERIFYSTATUSOVERRIDE_ID                 0x0bu
#define SECOC_SENDDEFAULTAUTHENTICATIONINFORMATION_ID 0x04u
#define SECOC_RXINDICATION_ID                         0x42u
#define SECOC_TPRXINDICATION_ID                       0x45u
#define SECOC_TXCONFIRMATION_ID                       0x40u
#define SECOC_TPTXCONFIRMATION_ID                     0x48u
#define SECOC_TRIGGERTRANSMIT_ID                      0x41u
#define SECOC_COPYRXDATA_ID                           0x44u
#define SECOC_COPYTXDATA_ID                           0x43u
#define SECOC_STARTOFRECEPTION_ID                     0x46u
#define SECOC_MAINFUNCTIONRX_ID                       0x06u
#define SECOC_MAINFUNCTIONTX_ID                       0x03u
#define SECOC_CSMGENERATEJOBFINISHEDINDICATION_ID     0x50u
#define SECOC_CSMVERIFYJOBFINISHEDINDICATION_ID       0x51u

/* Development Errors */
#define SECOC_E_PARAM_POINTER      0x01u
#define SECOC_E_UNINIT             0x02u
#define SECOC_E_INVALID_PDU_SDU_ID 0x03u
#define SECOC_E_CRYPTO_FAILURE     0x04u
#define SECOC_E_INIT_FAILED        0x07u

/* Runtime Errors */
#define SECOC_E_FRESHNESS_FAILURE 0x08u

#if !defined(SECOC_PARA_UNUSED)
#define SECOC_PARA_UNUSED(x) ((void)(x))
#endif /* SECOC_PARA_UNUSED */

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/

extern CONST(SecOC_ConfigType, SECOC_CONST) SecOC_ConfigData;

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

/******************************************************************************/
/*
 * Brief               Initializes the the SecOC module.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      config: Pointer to a selected configuration structure
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, SECOC_CODE) SecOC_Init(P2CONST(SecOC_ConfigType, AUTOMATIC, SECOC_CFG_CONST) config);

/******************************************************************************/
/*
 * Brief               This service stops the secure onboard communication.
 *                     All buffered I-PDU are removed and have to be obtained
 *                     again, if needed, after SecOC_Init has been called.
 * ServiceId           0x05
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, SECOC_CODE) SecOC_DeInit(void);

/******************************************************************************/
/*
 * Brief               Returns the version information of this module.
 * ServiceId           0x02
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     versioninfo: Pointer to where to store the version
 *                                  information of this module.
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
FUNC(void, SECOC_CODE) SecOC_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, SECOC_APPL_DATA) versioninfo);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

/******************************************************************************/
/*
 * Brief               Requests transmission of a PDU.
 * ServiceId           0x49
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds.
 * Param-Name[in]      TxPduId: Identifier of the PDU to be transmitted.
 *                     PduInfoPtr: Length of and pointer to the PDU data and
 *                                 pointer to MetaData.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Transmit request has been accepted.
 *                     E_NOT_OK: Transmit request has not been accepted.
 */
/******************************************************************************/
FUNC(Std_ReturnType, SECOC_CODE)
SecOC_IfTransmit(PduIdType TxPduId, P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_CONST) PduInfoPtr);

/******************************************************************************/
/*
 * Brief               Requests transmission of a PDU.
 * ServiceId           0x49
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds.
 * Param-Name[in]      TxPduId: Identifier of the PDU to be transmitted.
 *                     PduInfoPtr: Length of and pointer to the PDU data and
 *                                 pointer to MetaData.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Transmit request has been accepted.
 *                     E_NOT_OK: Transmit request has not been accepted.
 */
/******************************************************************************/
FUNC(Std_ReturnType, SECOC_CODE)
SecOC_TpTransmit(PduIdType TxPduId, P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_CONST) PduInfoPtr);

/******************************************************************************/
/*
 * Brief               Requests cancellation of an ongoing transmission of a PDU
 *                     in a lower layer communication module.
 * ServiceId           0x4a
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds.
 * Param-Name[in]      TxPduId: Identification of the PDU to be cancelled.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Cancellation was executed successfully by the
 *                           destination module.
 *                     E_NOT_OK: Cancellation was rejected by the destination
 *                               module.
 */
/******************************************************************************/
FUNC(Std_ReturnType, SECOC_CODE) SecOC_IfCancelTransmit(PduIdType TxPduId);

/******************************************************************************/
/*
 * Brief               Requests cancellation of an ongoing transmission of a PDU
 *                     in a lower layer communication module.
 * ServiceId           0x4a
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds.
 * Param-Name[in]      TxPduId: Identification of the PDU to be cancelled.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Cancellation was executed successfully by the
 *                           destination module.
 *                     E_NOT_OK: Cancellation was rejected by the destination
 *                               module.
 */
/******************************************************************************/
FUNC(Std_ReturnType, SECOC_CODE) SecOC_TpCancelTransmit(PduIdType TxPduId);

/******************************************************************************/
/*
 * Brief               Requests cancellation of an ongoing reception of a PDU in
 *                     a lower layer transport protocol module.
 * ServiceId           0x4c
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxPduId: Identification of the PDU to be cancelled.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Cancellation was executed successfully by the
 *                           destination module.
 *                     E_NOT_OK: Cancellation was rejected by the destination
 *                               module.
 */
/******************************************************************************/
FUNC(Std_ReturnType, SECOC_CODE) SecOC_TpCancelReceive(PduIdType RxPduId);

/******************************************************************************/
/*
 * Brief               This service provides the ability to force specific
 *                     behaviour of SecOc: accept or drop an I-PDU with or
 *                     without performing the verification of authenticator or
 *                     independent of the authenticator verification result, and
 *                     force a specific result for SecOC_VerificationResultType
 *                     allowing additional fault handling in the application.
 * ServiceId           0x0b
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different Freshness ValueIDs
 * Param-Name[in]      ValueID: ID of the Freshness Value used to control the
 *                     verification behaviour of all assigned Secured I-PDUs
 *                     according to the override Status.Or ValueID is the DataID
 *                     of a Secured I-PDU that shall be controlled by the
 *                     overrideStatus.
 *                     overrideStatus: Defines whether verification is executed
 *                     and whether the I-PDU is passed on, and for how long the
 *                     override is active.
 *                     numberOfMessagesToOverride: Number of sequential
 *                     verification to override when using a specific counter
 *                     for authentication verification.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: request successful
 *                     E_NOT_OK: request failed
 */
/******************************************************************************/
FUNC(Std_ReturnType, SECOC_CODE)
SecOC_VerifyStatusOverride(uint16 ValueID, SecOC_OverrideStatusType overrideStatus, uint8 numberOfMessagesToOverride);

#if defined(SECOC_DEFAULT_AUTHENTICATION_INFORMATION_PATTERN)
/******************************************************************************/
/*
 * Brief               The service provides the ability to enable the sending of
 *                     un-authenticated PDU to lower layer.
 * ServiceId           0x04
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different FreshnessValueIDs.
 * Param-Name[in]      FreshnessValueID: ID of the Freshness Value for which
 *                     sending SecOCDefaultAuthenticationInformationPattern
 *                     should be enabled.
 *                     sendDefaultAuthenticationInformation: FALSE - sending
 *                     SecOCDefaultAuthenticationInformation Pattern shall be
 *                     disabled for given FreshnessValueID TRUE - sending
 *                     SecOCDefaultAuthenticationInformationPattern shall be
 *                     enabled for given FreshnessValueID
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: request successful
 *                     E_NOT_OK: request failed
 */
/******************************************************************************/
FUNC(Std_ReturnType, SECOC_CODE)
SecOC_SendDefaultAuthenticationInformation(uint16 FreshnessValueID, boolean sendDefaultAuthenticationInformation);
#endif

#endif /* SECOC_H */
