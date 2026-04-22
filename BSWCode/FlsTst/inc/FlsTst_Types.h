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
 **  @file               : FlsTst_Types.h
 **  @author             : peng.wu
 **  @date               : 2024/10/30
 **  @vendor             : isoft
 **  @description        : Type definitions of FlsTst
 **
 ***********************************************************************************************************************/
#ifndef FLSTST_TYPES_H
#define FLSTST_TYPES_H

/* =================================================== inclusions =================================================== */
#include "Std_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */

/* ===================================================== macros ===================================================== */
/* PRQA S 3461 ++ */ /* VL_FlsTst_3461*/
#if !defined(FLSTST_LOCAL_CONST)
#define FLSTST_LOCAL_CONST static const /** Defines used as the local const keyword for variables or functions */
#endif
/* PRQA S 3461 -- */

/* ================================================ type definitions ================================================ */
/**
 * @brief     Identification of the Foreground test block @range 0..0xFFFFFFFF
 */
typedef uint32 FlsTst_BlockIdFgndType;

/**
 * @brief     Definition of status datatype
 */
typedef enum
{
    FLSTST_UNINIT,   /**< The Flash Test is not initialized or not usable @range 0x00*/
    FLSTST_INIT,     /**< The Flash Test is initialized and ready to be started @range 0x01*/
    FLSTST_RUNNING,  /**< The Flash Test is currently running @range 0x02*/
    FLSTST_ABORTED,  /**< The Flash Test is aborted @range 0x03*/
    FLSTST_SUSPENDED /**< The Flash Test is waiting to be resumed or is waiting to start foreground mode test @range
                        0x04*/
} FlsTst_StateType;

/**
 * @brief     Definition of test result datatype
 */
typedef enum
{
    FLSTST_RESULT_NOT_TESTED, /**< There is no result availabl @range 0x00*/
    FLSTST_RESULT_OK,         /**< The last Flash Test has been tested with OK result @range 0x01*/
    FLSTST_RESULT_NOT_OK,     /**< The last Flash Test has been tested with NOT_OK result @range 0x02*/
} FlsTst_TestResultType;

/**
 * @brief     Definition of Foreground test result datatype
 */
typedef enum
{
    FLSTST_NOT_TESTED, /**< There is no result available @range 0x00*/
    FLSTST_OK,         /**< The last Flash Test has been tested with OK result @range 0x01*/
    FLSTST_NOT_OK,     /**< The last Flash Test has been tested with NOT_OK result @range 0x02*/
} FlsTst_TestResultFgndType;

/**
 * @brief     Definition of test algorithm datatype
 */
typedef enum
{
    FLSTST_ALGORITHM_UNDEFINED, /**< Undefined algorithm (uninitialized value) @range 0x00*/
    FLSTST_16BIT_CRC,           /**< CRC16 Algorithm @range 0x01*/
    FLSTST_32BIT_CRC,           /**< CRC32 Algorithm @range 0x02*/
    FLSTST_8BIT_CRC,            /**< CRC8 Algorithm @range 0x03*/
    FLSTST_CHECKSUM,            /**< CHECKSUM Algorithm @range 0x04*/
    FLSTST_DUPLICATED_MEMORY,   /**< DUPLICATED_MEMORY Algorithm @range 0x05*/
    FLSTST_ECC                  /**< ECC Algorithm @range 0x06*/
} FlsTst_AlgorithmType;

/**
 * @brief     Definition of Background test result datatype
 */
typedef struct
{
    uint32 ResultIntervalID; /**< FlsTstTestIntervalId,which is incremented by each new start of an test interval @range
                                0..0xFFFFFFFF*/
    FlsTst_TestResultType result; /**< Test Result in background flash test @range NA*/
} FlsTst_TestResultBgndType;

/**
 * @brief     Definition of detail error information datatype
 */
typedef struct
{
    uint32               ErrorBlockID;    /**< Record the block ID value when the error occurred @range 0..0xFFFFFFFF*/
    FlsTst_AlgorithmType Algorithm;       /**< The test algorithm used by the current block @range NA*/
    uint32               SignatureResult; /**< The signature result of the current block @range 0.. 0xFFFFFFFF*/
} FlsTst_ErrorDetailsType;

/**
 * @brief     Definition of Foreground test signature datatype
 */
typedef struct
{
    uint32 FgndSignature; /**< Signature result of foreground test @range 0.. 0xFFFFFFFF*/
} FlsTst_TestSignatureFgndType;

/**
 * @brief     Definition of Background test signature information datatype
 */
typedef struct
{
    uint32 SignatureIntervalID; /**< current value of FlsTstTestIntervalId @range 0..@range 0x00*/
    uint32 BgndSignature;       /**< Signature result of background test @range 0.. 0xFFFFFFFF*/
} FlsTst_TestSignatureBgndType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

#ifdef __cplusplus
}
#endif
#endif
