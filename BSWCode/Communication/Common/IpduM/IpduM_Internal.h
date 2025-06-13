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
**  FILENAME    : IpduM_Internal.h                                           **
**                                                                           **
**  Created on  :                                                            **
**  Author      : darren.zhang                                               **
**  Vendor      :                                                            **
**  DESCRIPTION : IPDUM                                                      **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */
#ifndef IPDUM_INTERNAL_H_
#define IPDUM_INTERNAL_H_
/******************************************************************************
**                      Includes                                              *
******************************************************************************/
#include "IpduM.h"
/******************************************************************************
**                      Global Symbols                                       **
******************************************************************************/
#define IPDUM_MODULE_ID        182u
#define IPDUM_VENDOR_ID        62u
#define IPDUM_SW_MAJOR_VERSION 2u
#define IPDUM_SW_MINOR_VERSION 0u
#define IPDUM_SW_PATCH_VERSION 0u
#if (STD_ON == IPDUM_MULTIPLE_PARTITION_USED)
#define IPDUM_INSTANCE_ID (GetApplicationID())
#else
#define IPDUM_INSTANCE_ID 0u
#endif

#define IPDUM_UNUSED_UINT8                  (0xFFu)

#define IPDUM_CONTAINER_DEFAULT_TXCONFTIMER 10u

#define IPUDM_TRIG_CONTAINER_SUPPORT_TXCONF STD_ON

#define IPUDM_TRIG_MULT_SUPPORT_TXCONF      STD_ON

#define IPDUM_REINIT_CHECK                  STD_ON

#define IpduM_GetStartOfMainFunctionTx(mainFunctionId) \
    ((mainFunctionId) == 0u) ? 0u : IpduM_CfgPtr->IpduMMainFunctionTxRange[(mainFunctionId)-1u]
#define IpduM_GetEndOfMainFunctionTx(mainFunctionId) IpduM_CfgPtr->IpduMMainFunctionTxRange[(mainFunctionId)]

#define IpduM_GetStartOfMainFunctionRx(mainFunctionId) \
    ((mainFunctionId) == 0u) ? 0u : IpduM_CfgPtr->IpduMMainFunctionRxRange[(mainFunctionId)-1u]
#define IpduM_GetEndOfMainFunctionRx(mainFunctionId) IpduM_CfgPtr->IpduMMainFunctionRxRange[(mainFunctionId)]

#if (IPDUM_TX_REQUEST_NUMBER_MAX > 0u)
/* multiplexer tx request pdu data type define */
typedef struct
{
    /* tx confirmation timer */
    uint16 txConfTimer;
    /* last load in tx request index */
    uint8 lastTxDynamicPartIndex;
    /* trigger flag */
    boolean isTrigFlg;
} IpduM_InnerTxRequtstType;
#endif /* IPDUM_TX_REQUEST_NUMBER_MAX > 0u */

#if ((IPDUM_TX_REQUEST_NUMBER_MAX > 0u) || (IPDUM_RX_INDICA_NUMBER_MAX > 0u))
/* multiplexer pdu translate handle input parameters typedef */
typedef struct
{
    /* segmeent number */
    SegmentCntType segmentCnt;
    /* pdu byte order */
    IpduM_ByteOrderType byteOrd;
    /* input multiplexer pdu */
    const PduInfoType* inputPduPtr;
    /* output part pdu */
    PduInfoType* outputPduPtr;
    /* segment data point */
    const IpduM_SegmentType* segmentPtr;
} IpduM_PduTranslateType;

#endif

#if (IPDUM_CONTAINER_TX_NUMBER_MAX > 0u)
/* store contained index in gloabel container queue */
typedef struct
{
    /*per queue payload byte size in container*/
    uint16 payloadSize;
    /* contained index in one queue element start position */
    uint16 containedStartPos;
    /* number of contained in one queue element */
    uint8 containedCnt;
    /* contained index in one queue element to txconfirm */
    uint16 containedTxConfirmStartPos;
    /* number of pending txconfirm contained */
    uint8 containedTxConfirmCnt;
} IpduM_InnerContainerTxQueueHandleType;

typedef struct
{
    /* adding the first contained to a container */
    boolean firstAddFlg;
    boolean trigFlg;
    /* queue type */
    uint8 transReadPoint;
    uint8 txCofReadPoint;
    uint8 writePoint;
    uint8 queueCnt;
    /* transmission timer */
    uint16 transTimer;
    /* transmission confirmation timerout timer */
    uint16 txConfTimer;
} IpduM_InnerContainerTxType;
#endif /* IPDUM_CONTAINER_TX_NUMBER_MAX > 0u */

#if (IPDUM_CONTAINER_RX_NUMBER_MAX > 0u)
/* store contained index in gloabel container queue */
typedef struct
{
    /*per queue payload byte size in container*/
    uint16 payloadSize;
} IpduM_InnerContainerRxQueueHandleType;

typedef struct
{
    /* queue type */
    uint8 readPoint;
    uint8 writePoint;
    uint8 queueCnt;
} IpduM_InnerContainerRxType;
#endif /* IPDUM_CONTAINER_RX_NUMBER_MAX > 0u */

#endif /* IPDUM_INTERNAL_H_ */
