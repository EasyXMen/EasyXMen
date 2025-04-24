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
**************************************************************************** **
**                                                                           **
**  FILENAME    : LdCom_Cbk.h                                                **
**                                                                           **
**  Created on  :                                                            **
**  Author      : zhengfei.li                                                **
**  Vendor      :                                                            **
**  DESCRIPTION :                                                            **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */
#ifndef LDCOM_CBK_H_
#define LDCOM_CBK_H_
/******************************************************************************
**                      Includes                                              *
******************************************************************************/
#include "LdCom_Types.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/*Published information elements*/
#define LDCOM_CBK_H_AR_RELEASE_MAJOR_VERSION    4u
#define LDCOM_CBK_H_AR_RELEASE_MINOR_VERSION    5u
#define LDCOM_CBK_H_AR_RELEASE_REVISION_VERSION 0u

#define LDCOM_CBK_H_SW_MAJOR_VERSION            2u
#define LDCOM_CBK_H_SW_MINOR_VERSION            0u
#define LDCOM_CBK_H_SW_PATCH_VERSION            1u
/*******************************************************************************
**                      Public API Declarations                               **
*******************************************************************************/
extern FUNC(BufReq_ReturnType, LDCOM_COPYTXDATA_CODE) LdCom_CopyTxData(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) info,
    P2CONST(RetryInfoType, AUTOMATIC, LDCOM_APPL_DATA) retry,
    P2VAR(PduLengthType, AUTOMATIC, LDCOM_APPL_DATA) availableDataPtr);

extern FUNC(void, LDCOM_TPTXCONFIRMATION_CODE) LdCom_TpTxConfirmation(PduIdType id, Std_ReturnType result);

extern FUNC(BufReq_ReturnType, LDCOM_STARTOFRECEPTION_CODE) LdCom_StartOfReception(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) info,
    PduLengthType TpSduLength,
    P2VAR(PduLengthType, AUTOMATIC, LDCOM_APPL_DATA) bufferSizePtr);

extern FUNC(BufReq_ReturnType, LDCOM_COPYRXDATA_CODE) LdCom_CopyRxData(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) info,
    P2VAR(PduLengthType, AUTOMATIC, LDCOM_APPL_DATA) bufferSizePtr);

extern FUNC(void, LDCOM_TPRXINDICATION_CODE) LdCom_TpRxIndication(PduIdType id, Std_ReturnType result);

extern FUNC(void, LDCOM_RXINDICATION_CODE)
    LdCom_RxIndication(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) PduInfoPtr);

extern FUNC(void, LDCOM_TXCONFIRMATION_CODE) LdCom_TxConfirmation(PduIdType TxPduId);

extern FUNC(Std_ReturnType, LDCOM_TRIGGERTRANSMIT_CODE)
    LdCom_TriggerTransmit(PduIdType TxPduId, P2VAR(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) PduInfoPtr);

#endif /* LDCOM_CBK_H_ */
