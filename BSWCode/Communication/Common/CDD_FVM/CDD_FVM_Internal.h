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
**  FILENAME    : CDD_FVM_Internal.h                                          **
**                                                                            **
**  Created on  : 2023/10/20                                                  **
**  Author      : Jian.Jiang                                                  **
**  Vendor      :                                                             **
**  DESCRIPTION : FVM internal use                                            **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
#ifndef CDD_FVM_INTERNAL_H
#define CDD_FVM_INTERNAL_H

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "CDD_FVM_Cfg.h"

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
#define CDD_FVM_FRESHNESS_VALUE_MAX_BYTE          (8u)

#define CDD_FVM_TRIP_COUNTER_VALUE_MAX_BYTE       (3u)
#define CDD_FVM_RESET_COUNTER_VALUE_MAX_BYTE      (3u)
#define CDD_FVM_MESSAGE_COUNTER_VALUE_MAX_BYTE    (6u)

#define CDD_FVM_SINGLE_FRESHNESS_COUNT_INIT_VALUE (1u)

#define CDD_FVM_E_EQUAL                           (0u)
#define CDD_FVM_E_NOT_OK                          (1u)
#define CDD_FVM_E_MORE_THAN                       (111u)
#define CDD_FVM_E_LESS_THAN                       (112u)

#define CDD_FVM_TRIP_COUNTER_INIT_VALUE           (1u)

/* Define the maximum data statistics that can be received from the slave. */
#define CDD_FVM_MASTER_RX_SLAVE_DATA_MAX (32u)

/* Define the maximum length of the control frame */
#define CDD_FVM_CTRL_FRAME_BYTES_MAX (8u)

/* Define the broadcast message for control */
#define CDD_FVM_CTRL_BROADCAST_SYNMSG_ID (0xFFFF)

/* Define the protocol version of the control frame */
#define CDD_FVM_CTRL_FRAME_PROTOCOL_VERSION (1u)

/* Define the control message type */
static inline uint8 CDD_FVM_CLEAR_BITS(uint8 value)
{
    return ((value) & ~((1u << 5u) - 1u));
}

static inline uint8 CDD_FVM_CTRL_MSG_TYPE_SLAVE_VERIFY(uint8 value)
{
    return (((value) & ~((1u << 5u) - 1u)) | 1u);
}

static inline uint8 CDD_FVM_CTRL_MSG_TYPE_MASTER_ASK(uint8 value)
{
    return (((value) & ~((1u << 5u) - 1u)) | 2u);
}
static inline uint8 CDD_FVM_CTRL_MSG_TYPE_SLAVE_REQUSEST(uint8 value)
{
    return (((value) & ~((1u << 5u) - 1u)) | 4u);
}

#define CDD_FVM_CTRL_MSG_TYPE_VERIFY  (1u)
#define CDD_FVM_CTRL_MSG_TYPE_ASK     (2u)
#define CDD_FVM_CTRL_MSG_TYPE_REQUEST (4u)

/* Returns the remainder result, only for powers of 2 where the divisor is */
static inline uint8 CDD_FVM_MOD_SHIFT(uint8 dividend, uint8 divisor)
{
    return ((dividend) & ((divisor)-1u));
}

/* Returns the remainder form 8 */
static inline uint8 CDD_FVM_MOD_FROM_EIGHT(uint8 dividend)
{
    return ((dividend) & 0X07u);
}

/* Returns the quotient, only for powers of 2 where the divisor is, y is raised to the power of 2*/
static inline uint8 CDD_FVM_DIV_SHIFT(uint8 dividend, uint8 y)
{
    return ((dividend) >> (y));
}

static inline uint8 CDD_FVM_BIT_TO_BYTE(uint8 nbits)
{
    return ((nbits + 7u) >> 3u);
}

static inline uint64 CDD_FVM_GET_N_BITS_MAX_VALUE(uint8 nbits)
{
    return ((1uLL << (nbits)) - 1u);
}

/* Macro used to get configuration data */

/* TX*/

/*******************************************************************************
**                      Private Type Definitions                               *
*******************************************************************************/

/* For master-slave control frame state switching */
typedef enum
{
    CDD_FVM_CTRL_INIT = 0u, /* initial state */
    CDD_FVM_CTRL_WAIT,      /* State after sending a request frame */
    CDD_FVM_CTRL_OK,        /* Status of correct response received */
    CDD_FVM_CTRL_ERR,       /* Received an incorrect response */
} CDD_FVM_MultipleCtrlStatusType;

typedef struct
{
    uint8 FVMmsgCnt[CDD_FVM_FRESHNESS_VALUE_MAX_BYTE];
} CDD_FVM_TxFreshnessCntType;

typedef struct
{
    uint8 FVMmsgCnt[CDD_FVM_FRESHNESS_VALUE_MAX_BYTE];
    uint8 FVMmsgRxFreshnessValue[CDD_FVM_FRESHNESS_VALUE_MAX_BYTE];
} CDD_FVM_RxFreshnessCntType;

typedef struct
{
    uint8 FVMFreshnessValue[CDD_FVM_FRESHNESS_VALUE_MAX_BYTE];
    uint8 FvmMsgCnt[CDD_FVM_MESSAGE_COUNTER_VALUE_MAX_BYTE];
    uint8 FVMPreviousValue[CDD_FVM_FRESHNESS_VALUE_MAX_BYTE];
} CDD_FVM_MultipleFreshnessCntType;

typedef struct
{
    CDD_FVM_MultipleCtrlStatusType status;
    uint8 FvMTripCnt[CDD_FVM_TRIP_COUNTER_VALUE_MAX_BYTE];
    uint8 FvMResetCnt[CDD_FVM_RESET_COUNTER_VALUE_MAX_BYTE];
#if (CDD_FVM_SLAVE_CTRL_PDU_ENABLED == STD_ON)
    uint8 FvmCtrlBuffer[CDD_FVM_CTRL_FRAME_BYTES_MAX];
    uint8 FvmCounter;
#endif /* CDD_FVM_SLAVE_CTRL_PDU_ENABLED == STD_ON */
} CDD_FVM_SlaveECUSyncMsgType;

typedef struct
{
    uint8 FvmEcuInstanceId;
    uint16 FvmSyncMsgId;
    uint8 FvmRxCounter;
    CDD_FVM_MultipleCtrlStatusType status;
    boolean isUse;
} CDD_FVM_MasterRxSlveCtrlInfoType;
typedef struct
{
    CDD_FVM_MultipleCtrlStatusType status;
    uint8 FvMTripCnt[CDD_FVM_TRIP_COUNTER_VALUE_MAX_BYTE];
    uint8 FvMResetCnt[CDD_FVM_RESET_COUNTER_VALUE_MAX_BYTE];
    uint8 FvMSynMsgBuff[CDD_FVM_TRIP_COUNTER_VALUE_MAX_BYTE + CDD_FVM_RESET_COUNTER_VALUE_MAX_BYTE];
    uint16 resetCycle;
    boolean isRunSend;
#if (CDD_FVM_MASTER_CTRL_PDU_ENABLED == STD_ON)
    CDD_FVM_MasterRxSlveCtrlInfoType slaveRxCtrlData[CDD_FVM_MASTER_RX_SLAVE_DATA_MAX];
    uint8 FvmCtrlBuffer[CDD_FVM_CTRL_FRAME_BYTES_MAX];
#endif /* CDD_FVM_MASTER_CTRL_PDU_ENABLED == STD_ON */
} CDD_FVM_MasterECUSyncMsgType;

#endif /* CDD_FVM_INTERNAL_H */
