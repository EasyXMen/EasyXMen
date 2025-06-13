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
**************************************************************************** **
**                                                                           **
**  FILENAME    : IpduM_Cbk.h                                                **
**                                                                           **
**  Created on  :                                                            **
**  Author      : darren.zhang                                               **
**  Vendor      :                                                            **
**  DESCRIPTION : IPDUM                                                      **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */
#ifndef IPDUM_CBK_H_
#define IPDUM_CBK_H_
/******************************************************************************
**                      Includes                                              *
******************************************************************************/
#include "IpduM_Internal.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/*Published information elements*/
#define IPDUM_CBK_H_AR_RELEASE_MAJOR_VERSION    4u
#define IPDUM_CBK_H_AR_RELEASE_MINOR_VERSION    5u
#define IPDUM_CBK_H_AR_RELEASE_REVISION_VERSION 0u

/******************************Public API Declarations************************/
/**
 * Indication of a received I-PDU from a lower layer communication
    interface module.
 * Service ID: 0x42
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant for different PduIds. Non reentrant for the same PduId.
 * Parameters(IN): RxPduId, ID of the received I-PDU.
                    PduInfoPtr, Contains the length of the received I-PDU and a
        pointer to a buffer (SduDataPtr) containing the I-PDU.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *  SWS_IpduM_00041,SWS_IpduM_00042,SWS_IpduM_00217,SWS_IpduM_00086
 */

extern FUNC(void, IPDUM_RXINDICATION_CODE)
    IpduM_RxIndication(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr);

/**
 * The lower layer communication interface module confirms the transmission of
    an I-PDU.
 * Service ID: 0x40
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant for different PduIds. Non reentrant for the same PduId.
 * Parameters(IN): TxPduId, ID of the I-PDU that has been transmitted.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *  SWS_IpduM_00087,SWS_IpduM_00088
 */

extern FUNC(void, IPDUM_TXCONFIRMATION_CODE) IpduM_TxConfirmation(PduIdType TxPduId);

/**
 * IpduM_TriggerTransmit
 * Service ID: 0x41
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant for different PduIds. Non reentrant for the same PduId.
 * Parameters(IN): TxPduId, ID of the received I-PDU.
 * Parameters(INOUT):  PduInfoPtr, Contains the length of the
        transmit I-PDU and a pointer to a buffer containing the I-PDU.
 * Parameters(OUT): NA
 * Return value: Std_ReturnType E_OK: SDU data has been copied
        E_NOT_OK: No SDU data has been copied.
 *  SWS_IpduM_00089,SWS_IpduM_00090,SWS_IpduM_00091
 */

extern FUNC(Std_ReturnType, IPDUM_TRIGGERTRANSMIT_CODE)
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    IpduM_TriggerTransmit(PduIdType TxPduId, P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

#endif /* IPDUM_CBK_H_ */
