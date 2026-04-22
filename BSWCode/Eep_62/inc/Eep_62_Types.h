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
 **  @file               : Eep_62_Types.h
 **  @author             : peng.wu
 **  @date               : 2024/12/23
 **  @vendor             : isoft
 **  @description        : Type definitions of Eep
 **
 ***********************************************************************************************************************/

#ifndef EEP_62_TYPES_H_
#define EEP_62_TYPES_H_

/* =================================================== inclusions =================================================== */
#include "Std_Types.h"
#include "MemIf_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */
/**
 * @brief Used as address offset from the configured EEPROM base address to access a certain EEPROM memory area @range
 * NA
 */
typedef uint32 Eep_62_AddressType;

/**
 * @brief Specifies the number of bytes to read/write/erase/compare @range NA
 */
typedef uint16 Eep_62_LengthType;

/**
 * @brief Define EEPROM module request job type
 */
typedef enum
{
    EEP_62_JOB_NONE,   /**< IDLE @range 0x00*/
    EEP_62_JOB_ERASE,  /**< Eep_62_Erase_Handle @range 0x01*/
    EEP_62_JOB_WRITE,  /**< Eep_62_Write_Handle @range 0x02*/
    EEP_62_JOB_READ,   /**< Eep_62_Read_Handle @range 0x03*/
    EEP_62_JOB_COMPARE /**< Eep_62_Compare_Handle @range 0x04*/
} Eep_62_RequestJobType;

/**
 * @brief Prototype of EEprom driver routine
 */
typedef struct
{
    boolean               StateForRunMainFunction; /**< Control flag for main function execution @range 0..1*/
    MemIf_ModeType        currentMode;             /**< The currently mode @range NA*/
    Eep_62_RequestJobType currentJob;              /**< The currently request job @range NA*/
    Eep_62_LengthType     length;                  /**< The length of request job @range 0..65535*/
    Eep_62_LengthType     CompareLength;           /**< The length of compare job @range 0..65535*/
    Eep_62_LengthType     maxReadSize;             /**< Max read size @range 0..65535*/
    Eep_62_LengthType     maxWriteSize;            /**< Max write size @range 0..65535*/
    Eep_62_AddressType    operateAddr;             /**< Address of currently operation @range 0..0xFFFFFFFF*/
    uint8*                dataBufferPtr;           /**< Pointer to the data buffer @range NA*/
    const uint8*          WriteBuffer;             /**< Pointer to the write buffer @range NA*/
} Eep_62_RuntimeType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

#ifdef __cplusplus
}
#endif
#endif
