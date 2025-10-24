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
/*
 ************************************************************************************************************************
 **
 **  @file               : CDD_FVM_Internal.h
 **  @author             : Jian.Jiang
 **  @date               : 2023/10/20
 **  @vendor             : isoft
 **  @description        : FVM internal use
 **
 ***********************************************************************************************************************/

#ifndef CDD_FVM_INTERNAL_H
#define CDD_FVM_INTERNAL_H

/* =================================================== inclusions =================================================== */
#include "CDD_FVM_Cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */

/** Unsigned const number */
#define CDD_FVM_CONST_0U 0x0u
/** Unsigned const number */
#define CDD_FVM_CONST_1U 0x1u
/** Unsigned const number */
#define CDD_FVM_CONST_2U 0x2u
/** Unsigned const number */
#define CDD_FVM_CONST_3U 0x3u
/** Unsigned const number */
#define CDD_FVM_CONST_4U 0x4u
/** Unsigned const number */
#define CDD_FVM_CONST_5U 0x5u
/** Unsigned const number */
#define CDD_FVM_CONST_7U 0x7u
/** Unsigned const number */
#define CDD_FVM_CONST_8U 0x8u
/** Unsigned const number */
#define CDD_FVM_CONST_64U 0x40u

/** Maximum number of bytes for freshness value */
#define CDD_FVM_FRESHNESS_VALUE_MAX_BYTE (8u)

/** Maximum number of bytes for trip counter value */
#define CDD_FVM_TRIP_COUNTER_VALUE_MAX_BYTE (3u)
/** Maximum number of bytes for reset counter value */
#define CDD_FVM_RESET_COUNTER_VALUE_MAX_BYTE (3u)
/** Maximum number of bytes for message counter value */
#define CDD_FVM_MESSAGE_COUNTER_VALUE_MAX_BYTE (6u)

/** Initial value for single freshness count */
#define CDD_FVM_SINGLE_FRESHNESS_COUNT_INIT_VALUE (1u)

/* Error codes */
/** Error codes for freshness value */
#define CDD_FVM_E_EQUAL (0u)
/** Error codes for freshness value */
#define CDD_FVM_E_NOT_OK (1u)
/** Error codes for freshness value */
#define CDD_FVM_E_MORE_THAN (111u)
/** Error codes for freshness value */
#define CDD_FVM_E_LESS_THAN (112u)

/** Initial value for trip counter */
#define CDD_FVM_TRIP_COUNTER_INIT_VALUE (1u)

/** Define the maximum data statistics that can be received from the slave. */
#define CDD_FVM_MASTER_RX_SLAVE_DATA_MAX (32u)

/** Define the maximum length of the control frame */
#define CDD_FVM_CTRL_FRAME_BYTES_MAX (8u)

/** Define the broadcast message for control */
#define CDD_FVM_CTRL_BROADCAST_SYNMSG_ID (0xFFFF)

/** Define the protocol version of the control frame */
#define CDD_FVM_CTRL_FRAME_PROTOCOL_VERSION (1u)

/** Define the message type of the control frame */
#define CDD_FVM_CTRL_MSG_TYPE_VERIFY (1u)
/** Define the message type of the control frame */
#define CDD_FVM_CTRL_MSG_TYPE_ASK (2u)
/** Define the message type of the control frame */
#define CDD_FVM_CTRL_MSG_TYPE_REQUEST (4u)

/* ================================================ type definitions ================================================ */

/* For master-slave control frame state switching */
/**
 * @brief  Define the state of the control frame.
 *
 */
typedef enum CDD_FVM_MultipleCtrlStatusTag
{
    CDD_FVM_CTRL_INIT = 0, /**<  initial state @range 0 */
    CDD_FVM_CTRL_WAIT = 1, /**<  State after sending a request frame @range 1 */
    CDD_FVM_CTRL_OK   = 2, /**<  Status of correct response received @range 2 */
    CDD_FVM_CTRL_ERR  = 3, /**<  Received an incorrect response @range 3 */
} CDD_FVM_MultipleCtrlStatusType;

/**
 * @brief  Define the structure of the freshness counter in Tx mode.
 *
 */
typedef struct CDD_FVM_TxFreshnessCntTag
{
    uint8 FVMmsgCnt[CDD_FVM_FRESHNESS_VALUE_MAX_BYTE]; /**<  Freshness counter value. @range NA*/
} CDD_FVM_TxFreshnessCntType;

/**
 * @brief  Define the structure of the freshness counter in Rx mode.
 *
 */
typedef struct CDD_FVM_RxFreshnessCntTag
{
    uint8 FVMmsgCnt[CDD_FVM_FRESHNESS_VALUE_MAX_BYTE];              /**<  Freshness counter value. @range NA */
    uint8 FVMmsgRxFreshnessValue[CDD_FVM_FRESHNESS_VALUE_MAX_BYTE]; /**<  Freshness counter value. @range NA */
} CDD_FVM_RxFreshnessCntType;

/**
 * @brief  Define the structure of the freshness counter in Tx mode.
 *
 */
typedef struct CDD_FVM_MultipleFreshnessCntTag
{
    uint8 FVMFreshnessValue[CDD_FVM_FRESHNESS_VALUE_MAX_BYTE]; /**<  Freshness counter value. @range NA */
    uint8 FvmMsgCnt[CDD_FVM_MESSAGE_COUNTER_VALUE_MAX_BYTE];   /**<  Message counter value. @range NA */
    uint8 FVMPreviousValue[CDD_FVM_FRESHNESS_VALUE_MAX_BYTE];  /**<  Previous freshness counter value. @range NA */
} CDD_FVM_MultipleFreshnessCntType;

/**
 * @brief  Define the structure of the freshness counter in slave Ecu
 */
typedef struct CDD_FVM_SlaveECUSyncMsgTag
{
    CDD_FVM_MultipleCtrlStatusType status;                   /**<  Status of the control frame. @range 0 ~ 3 */
    uint8 FvMTripCnt[CDD_FVM_TRIP_COUNTER_VALUE_MAX_BYTE];   /**<  Trip counter value. @range NA */
    uint8 FvMResetCnt[CDD_FVM_RESET_COUNTER_VALUE_MAX_BYTE]; /**<  Reset counter value. @range NA */
#if (CDD_FVM_SLAVE_CTRL_PDU_ENABLED == STD_ON)
    uint8 FvmCtrlBuffer[CDD_FVM_CTRL_FRAME_BYTES_MAX]; /**<  Control frame buffer. @range NA */
    uint8 FvmCounter;                                  /**<  Counter for the control frame. @range 0x00 ~ 0xFF */
#endif                                                 /* CDD_FVM_SLAVE_CTRL_PDU_ENABLED == STD_ON */
} CDD_FVM_SlaveECUSyncMsgType;

/**
 * @brief defeine control frame structure for master ECU
 *
 */
typedef struct CDD_FVM_MasterRxSlveCtrlInfoTag
{
    uint8                          FvmEcuInstanceId; /**<  ECU instance ID. @range 0 ~ 0xFF */
    uint16                         FvmSyncMsgId;     /**<  Message ID. @range 0 ~ 0xFFFF */
    uint8                          FvmRxCounter;     /**<  Counter for the control frame. @range 0 ~ 0xFF */
    CDD_FVM_MultipleCtrlStatusType status;           /**<  Status of the control frame. @range 0 ~ 3 */
    boolean                        isUse;            /**<  Flag for the control frame. @range 0 ~ 1 */
} CDD_FVM_MasterRxSlveCtrlInfoType;

/**
 * @brief  Define the structure of the synchronization message from the master ECU.
 *
 */
typedef struct CDD_FVM_MasterECUSyncMsgTag
{
    CDD_FVM_MultipleCtrlStatusType status;                   /**<  Status of the control frame. @range 0 ~ 3 */
    uint8 FvMTripCnt[CDD_FVM_TRIP_COUNTER_VALUE_MAX_BYTE];   /**<  Trip counter value. @range NA */
    uint8 FvMResetCnt[CDD_FVM_RESET_COUNTER_VALUE_MAX_BYTE]; /**<  Reset counter value. @range NA */
    uint8
        FvMSynMsgBuff[CDD_FVM_TRIP_COUNTER_VALUE_MAX_BYTE + CDD_FVM_RESET_COUNTER_VALUE_MAX_BYTE]; /**<  Synchronization
                                                                                                      message buffer.
                                                                                                      @range NA */
    uint16  resetCycle; /**<  Reset cycle. @range NA */
    boolean isRunSend;  /**< Flag for the control frame. @range 0 ~ 1   */
#if (CDD_FVM_MASTER_CTRL_PDU_ENABLED == STD_ON)
    CDD_FVM_MasterRxSlveCtrlInfoType
          slaveRxCtrlData[CDD_FVM_MASTER_RX_SLAVE_DATA_MAX]; /**<  Control frame information. @range NA */
    uint8 FvmCtrlBuffer[CDD_FVM_CTRL_FRAME_BYTES_MAX];       /**<  Control frame buffer. @range NA */
#endif                                                       /* CDD_FVM_MASTER_CTRL_PDU_ENABLED == STD_ON */
} CDD_FVM_MasterECUSyncMsgType;

/* ========================================== internal function definitions ========================================= */

/**
 * @brief       Clears the lower 5 bits of the input value.
 * @param[in]   value   : The 8-bit value from which the lower 5 bits will be cleared.
 * @return      uint8
 * @retval      uint8 : The input value with its lower 5 bits set to 0.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70866
 */
static inline uint8 CDD_FVM_CLEAR_BITS(uint8 value)
{
    /* PRQA S 4397 ++ */ /* VL_CDD_FVM_4397 */
    return ((value) & ~((1u << CDD_FVM_CONST_5U) - 1u));
    /* PRQA S 4397 -- */
}

/**
 * @brief       Sets the lower 5 bits of the input value to 0 and sets the least significant bit to 1.
 * @param[in]   value   : The 8-bit value to be modified.
 * @return      uint8
 * @retval      uint8 : The modified value with its lower 5 bits set to 0 and the least significant bit set to 1.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70867
 */
static inline uint8 CDD_FVM_CTRL_MSG_TYPE_SLAVE_VERIFY(uint8 value)
{
    /* PRQA S 4397 ++ */ /* VL_CDD_FVM_4397 */
    return (((value) & ~((1u << CDD_FVM_CONST_5U) - 1u)) | 1u);
    /* PRQA S 4397 -- */
}

/**
 * @brief  Function used to set the bits of the value.
 *
 * @param[in] value  The value to set the bits.
 * @return uint8
 * @retval  uint8 : The value after setting the bits.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70868
 */
static inline uint8 CDD_FVM_CTRL_MSG_TYPE_MASTER_ASK(uint8 value)
{
    /* PRQA S 4397 ++ */ /* VL_CDD_FVM_4397 */
    return (((value) & ~((1u << CDD_FVM_CONST_5U) - 1u)) | CDD_FVM_CONST_2U);
    /* PRQA S 4397 -- */
}

/**
 * @brief  Function used to set the bits of the value.
 *
 * @param[in] value  The value to set the bits.
 * @return uint8 : The value after setting the bits.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70869
 */
static inline uint8 CDD_FVM_CTRL_MSG_TYPE_SLAVE_REQUSEST(uint8 value)
{
    /* PRQA S 4397 ++ */ /* VL_CDD_FVM_4397 */
    return (((value) & ~((1u << CDD_FVM_CONST_5U) - 1u)) | CDD_FVM_CONST_4U);
    /* PRQA S 4397 -- */
}

/* Returns the remainder result, only for powers of 2 where the divisor is */

/**
 * @brief Function used to get the remainder result.
 *
 * @param[in] dividend  The dividend to get the result from.
 * @param[in] divisor  The divisor to get the result from.
 * @return uint8
 * @retval    uint8 : The result after getting the remainder.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70870
 */
static inline uint8 CDD_FVM_MOD_SHIFT(uint8 dividend, uint8 divisor)
{
    return ((dividend) & ((divisor)-1u));
}

/**
 * @brief       Returns the lower 3 bits of the input value.
 * @param[in]   dividend   : The 8-bit value from which the lower 3 bits will be extracted.
 * @return      uint8
 * @retval      uint8 : The lower 3 bits of the input value.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70871
 */
static inline uint8 CDD_FVM_MOD_FROM_EIGHT(uint8 dividend)
{
    /* PRQA S 4397 ++ */ /* VL_CDD_FVM_4397 */
    return ((dividend) & (uint8)CDD_FVM_CONST_7U);
    /* PRQA S 4397 -- */
}

/**
 * @brief       Right shifts the dividend by y bits.
 * @param[in]   dividend   : The 8-bit value to be shifted.
 * @param[in]   y          : The number of bits to shift.
 * @return      uint8
 * @retval      uint8 : The result of right shifting the dividend by y bits.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70872
 */
static inline uint8 CDD_FVM_DIV_SHIFT(uint8 dividend, uint8 y)
{
    return ((dividend) >> (y));
}

/**
 * @brief       Converts the number of bits to the equivalent number of bytes.
 * @param[in]   nbits   : The number of bits to be converted.
 * @return      uint8
 * @retval      uint8 : The equivalent number of bytes.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70873
 */
static inline uint8 CDD_FVM_BIT_TO_BYTE(uint8 nbits)
{
    return ((nbits + CDD_FVM_CONST_7U) >> CDD_FVM_CONST_3U);
}

/**
 * @brief       Calculates the maximum value for a given number of bits.
 * @param[in]   nbits   : The number of bits.
 * @return      uint64
 * @retval      uint64 : The maximum value that can be represented with nbits.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70874
 */
static inline uint64 CDD_FVM_GET_N_BITS_MAX_VALUE(uint8 nbits)
{
    return ((1uLL << (nbits)) - 1uLL);
}

#ifdef __cplusplus
}
#endif
#endif /* CDD_FVM_INTERNAL_H */
