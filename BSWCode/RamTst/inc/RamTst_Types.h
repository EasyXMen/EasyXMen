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
 **  @file               : RamTst_Types.h
 **  @author             : peng.wu
 **  @date               : 2024/09/27
 **  @vendor             : isoft
 **  @description        : Type definitions of RamTst
 **
 ***********************************************************************************************************************/
#ifndef RAMTST_TYPES_H_
#define RAMTST_TYPES_H_

/* =================================================== inclusions =================================================== */
#include "Std_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */

/* ===================================================== macros ===================================================== */
#if !defined(RAMTST_LOCAL_CONST)
/* PRQA S 3414, 3461 ++ */              /* VL_QAC_FctLikeMacro, VL_RamTst_3461 */
#define RAMTST_LOCAL_CONST static const /** Defines used as the local const keyword for variables or functions */
/* PRQA S 3414, 3461 -- */
#endif
/* PRQA S 3428 ++ */ /* VL_RamTst_3428 */
#if !defined(RAMTST_CONST)
#define RAMTST_CONST const /** Defines used as the const keyword for variables or functions */
#endif
/* PRQA S 3428 -- */

/* ================================================ type definitions ================================================ */
/**
 * @brief     Identification of the algorithm type
 */
typedef uint8 RamTst_AlgParamsIdType;

/**
 * @brief     Number of the tested cells type
 */
typedef uint32 RamTst_NumberOfTestedCellsType;

/**
 * @brief     Number of the test block type
 */
typedef uint16 RamTst_NumberOfBlocksType;

/**
 * @brief     Defines status for the execution type
 */
typedef enum
{
    RAMTST_EXECUTION_UNINIT,    /**< The RAM Test is not initialized or not usable @range 0x00*/
    RAMTST_EXECUTION_STOPPED,   /**< The RAM Test is stopped and ready to be started in foreground or to be allowed in
                                   background @range 0x01*/
    RAMTST_EXECUTION_RUNNING,   /**< The RAM Test is currently running @range 0x02*/
    RAMTST_EXECUTION_SUSPENDED, /**< The RAM Test is currently running @range 0x03*/
} RamTst_ExecutionStatusType;

/**
 * @brief     Type of test result for RamTst
 */
typedef enum
{
    RAMTST_RESULT_NOT_TESTED, /**< The RAM Test is not executed @range 0x00*/
    RAMTST_RESULT_OK,         /**< The RAM Test has been tested with OK result @range 0x01*/
    RAMTST_RESULT_NOT_OK,     /**< The RAM Test has been tested with NOTOK result @range 0x02*/
    RAMTST_RESULT_UNDEFINED,  /**< The RAM Test is currently running @range 0x03*/
} RamTst_TestResultType;

/**
 * @brief     Type of used algorithm for RamTst
 */
typedef enum
{
    RAMTST_ALGORITHM_UNDEFINED, /**< Undefined algorithm (uninitialized value) @range 0x00*/
    RAMTST_CHECKERBOARD_TEST,   /**< Checkerboard test algorithm @range 0x01*/
    RAMTST_MARCH_TEST,          /**< March test algorithm @range 0x02*/
    RAMTST_WALK_PATH_TEST,      /**< Walk path test algorithm @range 0x03*/
    RAMTST_GALPAT_TEST,         /**< Galpat test algorithm @range 0x04*/
    RAMTST_TRANSP_GALPAT_TEST,  /**< Transparent Galpat test algorithm @range 0x05*/
    RAMTST_ABRAHAM_TEST,        /**< Abraham test algorithm @range 0x06*/
} RamTst_AlgorithmType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

#ifdef __cplusplus
}
#endif
#endif
