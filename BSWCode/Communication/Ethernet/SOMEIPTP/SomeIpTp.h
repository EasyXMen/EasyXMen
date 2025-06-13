/* PRQA S 3108++ */
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
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : SomeIpTp.h                                                  **
**                                                                            **
**  Created on  :                                                             **
**  Author      : HuRongbo                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19_11                      **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*  <VERSION>    <DATE>       <AUTHOR>   <REVISION LOG>
 *  V2.0.0       2020-07-06   HuRongbo   R19_11 SomeIpTp initial version.
 *  V2.0.1       2021-07-30   HuRongbo   Bug fix
 */

#ifndef SOMEIPTP_H
#define SOMEIPTP_H

#include "SomeIpTp_Types.h"
#include "SomeIpTp_Cfg.h"

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define SOMEIPTP_VENDOR_ID          62U
#define SOMEIPTP_MODULE_ID          177U
#define SOMEIPTP_INSTANCE_ID        0U
#define SOMEIPTP_H_AR_MAJOR_VERSION 4U
#define SOMEIPTP_H_AR_MINOR_VERSION 5U
#define SOMEIPTP_H_AR_PATCH_VERSION 0U
#define SOMEIPTP_H_SW_MAJOR_VERSION 2U
#define SOMEIPTP_H_SW_MINOR_VERSION 0U
#define SOMEIPTP_H_SW_PATCH_VERSION 1U

/* Development Errors */
#define SOMEIPTP_E_UNINIT        0x01U
#define SOMEIPTP_E_PARAM_POINTER 0x02U
#define SOMEIPTP_E_PARAM         0x03U

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

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/

extern CONST(SomeIpTp_ConfigType, SOMEIPTP_CONST) SomeIpTp_Config;

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

/******************************************************************************/
/*
 * Brief               Returns the version information of this module.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     VersionInfo: Pointer to where to store the version
 *                                  information of this module
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if (STD_ON == SOMEIPTP_VERSION_INFO_API)
FUNC(void, SOMEIPTP_CODE)
SomeIpTp_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) VersionInfo);
#endif /*STD_ON == SOMEIPTP_VERSION_INFO_API*/

/******************************************************************************/
/*
 * Brief               Initializes the SOME/IP TP module.
 * ServiceId           0x02
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      config: Base pointer to the configuration structure of
 *                             the SOME/IP TP module.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, SOMEIPTP_CODE) SomeIpTp_Init(P2CONST(SomeIpTp_ConfigType, AUTOMATIC, SOMEIPTP_APPL_CONST) config);

/******************************************************************************/
/*
 * Brief               Requests transmission of a PDU.
 * ServiceId           0x49
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds.
 * Param-Name[in]      TxPduId: Identifier of the PDU to be transmitted
 *                     PduInfoPtr: Length of and pointer to the PDU data and
 *                                 pointer to MetaData.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Transmit request has been accepted.
 *                     E_NOT_OK: Transmit request has not been accepted.
 */
/******************************************************************************/
FUNC(Std_ReturnType, SOMEIPTP_CODE)
SomeIpTp_Transmit(PduIdType TxPduId, P2CONST(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr);

/******************************************************************************/
/*
 * Brief               Within this API, the upper layer module (called module)
 *                     shall check whether the available data fits into the
 *                     buffer size reported by PduInfoPtr->SduLength.If it fits,
 *                     it shall copy its data into the buffer provided by
 *                     PduInfoPtr->SduDataPtr and update the length of the
 *                     actual copied data in PduInfoPtr->SduLength. If not, it
 *                     returns E_NOT_OK without changing PduInfoPtr.
 * ServiceId           0x41
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds.
 * Param-Name[in]      TxPduId:ID of the SDU that is requested to be transmitted
 * Param-Name[out]     None
 * Param-Name[in/out]  PduInfoPtr: Contains a pointer to a buffer (SduDataPtr)
 *                     to where the SDU data shall be copied, and the available
 *                     buffer size in SduLengh. On return, the service will
 *                     indicate the length of the copied SDU data in SduLength.
 * Return              E_OK: SDU has been copied and SduLength indicates the
 *                           number of copied bytes.
 *                     E_NOT_OK: No SDU data has been copied. PduInfoPtr must
 *                               not be used since it may contain a NULL pointer
 *                               or point to invalid data.
 */
/******************************************************************************/
FUNC(Std_ReturnType, SOMEIPTP_CODE)
SomeIpTp_TriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr);

/******************************************************************************/
/*
 * Brief               Indication of a received PDU from a lower layer
 *                     communication interface module.
 * ServiceId           0x42
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds.
 * Param-Name[in]      RxPduId: ID of the received PDU.
 *                     PduInfoPtr: Contains the length (SduLength) of the
 *                     received PDU, a pointer to a buffer (SduDataPtr)
 *                     containing the PDU, and the MetaData related to this PDU.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, SOMEIPTP_CODE)
SomeIpTp_RxIndication(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr);

/******************************************************************************/
/*
 * Brief               This service is called by the Ethernet Interface to report
 *                     a transceiver link state change.
 * ServiceId           0x06
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CtrlIdx: Index of the Ethernet controller within the context
 *                              of the Ethernet Interface
 *                     TransceiverLinkState: Actual transceiver link state of the
 *                                           specific network handle
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, SOMEIPTP_CODE)
SomeIpTp_TxConfirmation(
    PduIdType TxPduId
#ifdef SOMEIPTP_VER_R19_11
    ,
    Std_ReturnType result
#endif
);

/******************************************************************************/
/*
 * Brief               This function performs the processing of the AUTOSAR
 *                     SOME/IP TP module's transmission activities.
 * ServiceId           0x03
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, SOMEIPTP_CODE) SomeIpTp_MainFunctionTx(void);

/******************************************************************************/
/*
 * Brief               This function performs the processing of the AUTOSAR
 *                     SOME/IP TP module's reception activities.
 * ServiceId           0x04
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, SOMEIPTP_CODE) SomeIpTp_MainFunctionRx(void);

#endif /* SOMEIPTP_H */
