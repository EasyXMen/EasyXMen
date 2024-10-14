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
 *
 ********************************************************************************
 **                                                                            **
 **  FILENAME    : CanTSyn_Cbk.h                                               **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      : Xinrun Wang                                                 **
 **  Vendor      :                                                             **
 **  DESCRIPTION : Call-back function declarations                             **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
 **                                                                            **
 *******************************************************************************/
/* PRQA S 3108-- */
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/

#ifndef CANTSYN_CBK_H_
#define CANTSYN_CBK_H_

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/

#include "CanTSyn_Cfg.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/

#define CANTSYN_CBK_H_AR_MAJOR_VERSION 4U
#define CANTSYN_CBK_H_AR_MINOR_VERSION 5U
#define CANTSYN_CBK_H_AR_PATCH_VERSION 0U
#define CANTSYN_CBK_H_SW_MAJOR_VERSION 2U
#define CANTSYN_CBK_H_SW_MINOR_VERSION 0U
#define CANTSYN_CBK_H_SW_PATCH_VERSION 0U

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

/**
 * Indication of a received PDU from a lower layer communication interface module.
 * Service ID: 0x42
 * Sync/Async: synchronous
 * Reentrancy: Reentrant but not for the same timer instance
 * Parameters(IN): RxPduId: ID of the received PDU.
                   PduInfoPtr: Contains the length (SduLength) of the received PDU, a pointer to a buffer (SduDataPtr)
 containing the PDU, and the MetaData related to this PDU.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */

extern FUNC(void, CANTSYN_CODE)
    CanTSyn_RxIndication(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, CANTSYN_APPL_DATA) PduInfoPtr);

/**
 * The lower layer communication interface module confirms the transmission of a PDU, or the failure to transmit a PDU.
 * Service ID: 0x40
 * Sync/Async: synchronous
 * Reentrancy: Reentrant for different PduIds. Non reentrant for the same PduId.
 * Parameters(IN): TxPduId: ID of the PDU that has been transmitted.
                   result: E_OK: The PDU was transmitted.
                           E_NOT_OK: Transmission of the PDU failed.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */

extern FUNC(void, CANTSYN_CODE) CanTSyn_TxConfirmation(
    PduIdType TxPduId
#if (STD_ON == CANTSYN_VER_R19_11)
    ,
    Std_ReturnType result
#endif
);

#endif /* CANTSYN_CBK_H_ */
