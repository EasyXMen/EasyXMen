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
**  FILENAME    : LdCom_Types.h                                              **
**                                                                           **
**  Created on  :                                                            **
**  Author      : zhengfei.li                                                **
**  Vendor      :                                                            **
**  DESCRIPTION :                                                            **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */
#ifndef LDCOM_TYPES_H_
#define LDCOM_TYPES_H_
/******************************************************************************
**                      Global Symbols                                       **
******************************************************************************/
#include "LdCom_Cfg.h"
#include "PduR_LdCom.h"

#define LDCOM_TYPES_H_AR_MAJOR_VERSION 4u
#define LDCOM_TYPES_H_AR_MINOR_VERSION 5u
#define LDCOM_TYPES_H_AR_PATCH_VERSION 0u
#define LDCOM_TYPES_H_SW_MAJOR_VERSION 2u
#define LDCOM_TYPES_H_SW_MINOR_VERSION 0u
#define LDCOM_TYPES_H_SW_PATCH_VERSION 1u

#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
/* Error classification */
#define LDCOM_E_PARAM              ((uint8)0x01u)
#define LDCOM_E_UNINIT             ((uint8)0x02u)
#define LDCOM_E_PARAM_POINTER      ((uint8)0x03u)
#define LDCOM_E_INVALID_PDU_SDU_ID ((uint8)0x04u)
#define LDCOM_E_INVALID_SIGNAL_ID  ((uint8)0x05u)

/* Service ID[hex] */
#define LDCOM_SERVICE_ID_INIT             ((uint8)0x00u)
#define LDCOM_SERVICE_ID_DEINIT           ((uint8)0x02u)
#define LDCOM_SERVICE_ID_GETVERSIONINFO   ((uint8)0x03u)
#define LDCOM_SERVICE_ID_TRANSMIT         ((uint8)0x05u)
#define LDCOM_SERVICE_ID_COPYTXDATA       ((uint8)0x43u)
#define LDCOM_SERVICE_ID_TPTXCONFIRMATION ((uint8)0x48u)
#define LDCOM_SERVICE_ID_STARTOFRECEPTION ((uint8)0x46u)
#define LDCOM_SERVICE_ID_COPYRXDATA       ((uint8)0x44u)
#define LDCOM_SERVICE_ID_TPRXINDICATION   ((uint8)0x45u)
#define LDCOM_SERVICE_ID_RXINDICATION     ((uint8)0x42u)
#define LDCOM_SERVICE_ID_TXCONFIRMATION   ((uint8)0x40u)
#define LDCOM_SERVICE_ID_TRIGGERTRANSMIT  ((uint8)0x41u)
#endif /*STD_ON == LDCOM_DEV_ERROR_DETECT*/

#define LDCOM_UNUSED_UINT8 (0xFFu)

typedef P2FUNC(BufReq_ReturnType, LDCOM_APPL_CODE, LdComTxCopyTxDataFuncPtrType)(
    const PduInfoType* info,
    const RetryInfoType* retry,
    PduLengthType* availableDataPtr);
typedef P2FUNC(void, LDCOM_APPL_CODE, LdComTpTxConfirmationFuncPtrType)(Std_ReturnType result);
typedef P2FUNC(BufReq_ReturnType, LDCOM_APPL_CODE, LdComRxStartOfReceptionFuncPtrType)(
    const PduInfoType* info,
    PduLengthType TpSduLength,
    PduLengthType* bufferSizePtr);
typedef P2FUNC(BufReq_ReturnType, LDCOM_APPL_CODE, LdComRxCopyRxDataFuncPtrType)(
    const PduInfoType* info,
    PduLengthType* bufferSizePtr);
typedef P2FUNC(void, LDCOM_APPL_CODE, LdComTpRxIndicationFuncPtrType)(Std_ReturnType result);
typedef P2FUNC(void, LDCOM_APPL_CODE, LdComTxConfirmationFuncPtrType)(void);
typedef P2FUNC(void, LDCOM_APPL_CODE, LdComRxIndicationFuncPtrType)(const PduInfoType* PduInfoPtr);
typedef P2FUNC(Std_ReturnType, LDCOM_APPL_CODE, LdComTxTriggerTransmitFuncPtrType)(PduInfoType* PduInfoPtr);
/* Contains the configuration parameters of the IPdu inside LdCom. */
typedef struct
{
    PduIdType LdComPduId;
    PduIdType DestModulePduId;
#if (STD_ON == LDCOM_SUPPORT_SEND_TP)
    LdComTxCopyTxDataFuncPtrType LdComTxCopyTxDataFuncPtr;
    LdComTpTxConfirmationFuncPtrType LdComTpTxConfirmationFuncPtr;
#endif /* STD_ON == LDCOM_SUPPORT_SEND_TP */

#if (STD_ON == LDCOM_SUPPORT_RECEIVER_TP)
    LdComRxStartOfReceptionFuncPtrType LdComRxStartOfReceptionFuncPtr;
    LdComRxCopyRxDataFuncPtrType LdComRxCopyRxDataFuncPtr;
    LdComTpRxIndicationFuncPtrType LdComTpRxIndicationFuncPtr;
#endif /* STD_ON == LDCOM_SUPPORT_RECEIVER_TP */

#if (STD_ON == LDCOM_SUPPORT_SEND_IF)
    LdComTxConfirmationFuncPtrType LdComTxConfirmationFuncPtr;
#endif /* STD_ON == LDCOM_SUPPORT_SEND_IF */

#if (STD_ON == LDCOM_SUPPORT_RECEIVER_IF)
    LdComRxIndicationFuncPtrType LdComRxIndicationFuncPtr;
#endif /* STD_ON == LDCOM_SUPPORT_RECEIVER_IF */
#if (STD_ON == LDCOM_SUPPORT_TRIGGER_TRANSMIT)
    LdComTxTriggerTransmitFuncPtrType LdComTxTriggerTransmitFuncPtr;
#endif /* STD_ON == LDCOM_SUPPORT_TRIGGER_TRANSMIT */
} LdCom_IPduType;

typedef struct
{
    P2CONST(LdCom_IPduType, AUTOMATIC, LDCOM_CONST_PBCFG) LdComIPduPtr;
} LdCom_ConfigType;

#endif /* LDCOM_TYPES_H_ */
