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
 **  @file               : LdCom_Types.h
 **  @author             : zhengfei.li
 **  @date               : 2020/07/03
 **  @vendor             : isoft
 **  @description        : Large Data Communication callback header
 **
 ***********************************************************************************************************************/

#ifndef LDCOM_TYPES_H_
#define LDCOM_TYPES_H_

/* PRQA S 1536 EOF */ /* VL_LdCom_1536 */

/* =================================================== inclusions =================================================== */
#include "ComStack_Types.h"
#include "LdCom_Cfg.h"

#ifdef __cplusplus
extern "C" {
#endif
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */
/**
 * @brief     Define callback function types for copying data for transmission.
 */
typedef BufReq_ReturnType (*LdComTxCopyTxDataFuncPtrType)(
    const PduInfoType*   info,
    const RetryInfoType* retry,
    PduLengthType*       availableDataPtr);

/**
 * @brief     Define callback function types of notifications for TP transmission confirmation.
 */
typedef void (*LdComTpTxConfirmationFuncPtrType)(Std_ReturnType result);

/**
 * @brief     Define callback function start of reception for reception.
 */
typedef BufReq_ReturnType (*LdComRxStartOfReceptionFuncPtrType)(
    const PduInfoType* info,
    PduLengthType      TpSduLength,
    PduLengthType*     bufferSizePtr);

/**
 * @brief     Define callback function of copy data of reception for receive.
 */
typedef BufReq_ReturnType (*LdComRxCopyRxDataFuncPtrType)(const PduInfoType* info, PduLengthType* bufferSizePtr);

/**
 * @brief     Define callback function of notifications for TP receive.
 */
typedef void (*LdComTpRxIndicationFuncPtrType)(Std_ReturnType result);

/**
 * @brief      Define callback function types of notifications for IF transmission confirmation.
 */
typedef void (*LdComTxConfirmationFuncPtrType)(Std_ReturnType result);

/**
 * @brief     Define callback function of notifications for IF receive.
 */
typedef void (*LdComRxIndicationFuncPtrType)(const PduInfoType* PduInfoPtr);

/**
 * @brief     Define callback function for trigger transmit.
 */
typedef Std_ReturnType (*LdComTriggerTransmitFuncPtrType)(PduInfoType* PduInfoPtr);

/**
 * @brief     Contains the configuration parameters of the IPdu inside LdCom.
 */
typedef struct LdCom_IPduTypeTag
{
#if (STD_ON == LDCOM_SUPPORT_MULTIPLE_PARTITION)
    Os_ApplicationType ApplicationId; /** Application of Partition id of Ipdu */
#endif
    PduIdType DestModulePduId; /** Destination module Pdu Id */
#if (STD_ON == LDCOM_SUPPORT_SEND_TP)
    /**
     * Callback Function <TxCopyTxData> to the upper module
     */
    LdComTxCopyTxDataFuncPtrType CbkTxCopyTxDataFunc;
    /**
     * Callback Function <TpTxConfirmation> to the upper module
     */
    LdComTpTxConfirmationFuncPtrType CbkTpTxConfirmationFunc;
#endif

#if (STD_ON == LDCOM_SUPPORT_RECEIVER_TP)
    /**
     * Callback Function <RxStartOfReception> to the upper module
     */
    LdComRxStartOfReceptionFuncPtrType CbkRxStartOfReceptionFunc;
    /**
     * Callback Function <RxCopyRxData> to the upper module
     */
    LdComRxCopyRxDataFuncPtrType CbkRxCopyRxDataFunc;
    /**
     * Callback Function <TpRxIndication> to the upper module
     */
    LdComTpRxIndicationFuncPtrType CbkTpRxIndicationFunc;
#endif

#if (STD_ON == LDCOM_SUPPORT_SEND_IF)
    /**
     * Callback Function <TxConfirmation> to the upper module
     */
    LdComTxConfirmationFuncPtrType CbkTxConfirmationFunc;
#endif

#if (STD_ON == LDCOM_SUPPORT_RECEIVER_IF)
    /**
     * Callback Function <RxIndication> to the upper module
     */
    LdComRxIndicationFuncPtrType CbkRxIndicationFunc;
#endif
#if (STD_ON == LDCOM_SUPPORT_TRIGGER_TRANSMIT)
    /**
     * Callback Function <TriggerTransmit> to the upper module
     */
    LdComTriggerTransmitFuncPtrType CbkTriggerTransmitFunc;
#endif
} LdCom_IPduType;

/**
 * @brief     The root struct configuration parameters of LdCom.
 */
typedef struct LdCom_ConfigTypeTag
{
    PduIdType             IpduNum;    /** The Variant Ipdu number */
    const LdCom_IPduType* IPduCfgPtr; /** Ipdu configuration */

#if (STD_ON == LDCOM_SUPPORT_POST_BUILD)
    const PduIdType* UserIdMapLdComIPduIdPtr; /** User id map to LdCom Ipdu Id */
#endif
} LdCom_ConfigType;

#ifdef __cplusplus
}
#endif

#endif
