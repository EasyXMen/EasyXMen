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
 **  @file               : MemAcc_Types.h
 **  @author             : peng.wu
 **  @date               : 2025/04/04
 **  @vendor             : isoft
 **  @description        : Type definitions of MemAcc
 **
 ***********************************************************************************************************************/

#ifndef MEMACC_TYPES_H
#define MEMACC_TYPES_H

/* =================================================== inclusions =================================================== */
#include "Std_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */
/**
 * @brief     Unique address area ID type @range 0..0xFFFF
 */
typedef uint16 MemAcc_AddressAreaIdType;

#if (MEMACC_64BIT_SUPPORT == STD_OFF)
/**
 * @brief     Logical memory address type @range 0..0xFFFFFFFF
 */
typedef uint32 MemAcc_AddressType;

/**
 * @brief     Job length type @range 0..0xFFFFFFFF
 */
typedef uint32 MemAcc_LengthType;
#else
/**
 * @brief     Logical memory address type @range 0..0xFFFFFFFFFFFFFFFF
 */
typedef uint64 MemAcc_AddressType;

/**
 * @brief     Job length type @range 0..0xFFFFFFFFFFFFFFFF
 */
typedef uint64 MemAcc_LengthType;
#endif

/**
 * @brief     General data type @range 0..0xFF
 */
typedef uint8 MemAcc_DataType;

/**
 * @brief     Type for the unique numeric identifiers of all Mem hardware instances used for hardware specific requests
 * @range 0..0xFFFFFFFF
 */
typedef uint32 MemAcc_HwIdType;

/**
 * @brief     Asynchronous job result type
 */
typedef enum
{
    MEM_JOB_OK,          /**< The last job has been finished successfully @range 0x00 */
    MEM_JOB_PENDING,     /**< A job is currently being processed @range 0x01 */
    MEM_JOB_FAILED,      /**< Job failed for some unspecific reason @range 0x02 */
    MEM_INCONSISTENT,    /**< The checked page is not blank @range 0x03 */
    MEM_ECC_UNCORRECTED, /**< Uncorrectable ECC errors occurred during memory access @range 0x04 */
    MEM_ECC_CORRECTED    /**< Correctable ECC errors occurred during memory access @range 0x05 */
} MemAcc_MemJobResultType;

/**
 * @brief     Asynchronous job result type
 */
typedef enum
{
    MEMACC_OK,              /**< The last MemAcc job was finished successfully @range 0x00 */
    MEMACC_FAILED,          /**< The last MemAcc job resulted in an unspecific failure and the job was not completed
                                   @range 0x01 */
    MEMACC_INCONSISTENT,    /**< The results of the last MemAcc job didn't meet the expected result @range 0x02 */
    MEMACC_CANCELED,        /**< The last MemAcc job was canceled @range 0x03 */
    MEMACC_ECC_UNCORRECTED, /**< The last memory operation returned an uncorrectable ECC error @range 0x04 */
    MEMACC_ECC_CORRECTED    /**< The last memory operation returned a correctable ECC error @range 0x05 */
} MemAcc_JobResultType;

/**
 * @brief     Asynchronous job status type
 */
typedef enum
{
    MEMACC_JOB_IDLE,   /**< Job processing was completed or no job currently pending @range 0x00 */
    MEMACC_JOB_PENDING /**< A job is currently being processed @range 0x01 */
} MemAcc_JobStatusType;

/**
 * @brief     Type for asynchronous jobs
 */
typedef enum
{
    MEMACC_NO_JOB,            /**< No request job @range 0x00 */
    MEMACC_WRITE_JOB,         /**< Request write job @range 0x01 */
    MEMACC_READ_JOB,          /**< Request read job @range 0x02 */
    MEMACC_COMPARE_JOB,       /**< Request compare job @range 0x03 */
    MEMACC_ERASE_JOB,         /**< Request erase job @range 0x04 */
    MEMACC_MEMHWSPECIFIC_JOB, /**< Request hardware specific job @range 0x05 */
    MEMACC_BLANKCHECK_JOB,    /**< Request blank check job @range 0x06 */
    MEMACC_REQUESTLOCK_JOB    /**< Request lock job @range 0x07 */
} MemAcc_JobType;

/**
 * @brief     This structure contains information of Mem device characteristics
 */
typedef struct MemAcc_MemoryInfoTypeTag
{
    uint32 EraseSectorSize;           /**< Size of a sector in bytes @range 0..0xFFFFFFFF */
    uint32 EraseSectorBurstSize;      /**< Size of a sector burst in bytes. Equals SectorSize in case burst is disabled
                                         @range 0..0xFFFFFFFF */
    uint32 MinReadSize;               /**< Smallest readable unit in bytes @range 0..0xFFFFFFFF */
    uint32 WritePageSize;             /**< Write size of a page in bytes @range 0..0xFFFFFFFF */
    uint32 MaxReadSize;               /**< Largest readable unit in bytes @range 0..0xFFFFFFFF */
    uint32 WritePageBurstSize;        /**< Size of a page burst in bytes. Equals WritePageSize in case burst is disabled
                                           @range 0..0xFFFFFFFF */
    uint32             HwId;          /**< Referenced memory driver hardware identifier @range 0..0xFFFFFFFF */
    MemAcc_AddressType LogiStartAddr; /**< Logical start address of sub address area @range NA */
    MemAcc_AddressType PhysicalStartAddress; /**< Physical start address of sub address area @range NA */
    MemAcc_LengthType  MaxOffset;            /**< Size of sub address area in bytes -1 @range NA */
} MemAcc_MemoryInfoType;

/**
 * @brief     This structure contains information the current processing state of the MemAcc module
 */
typedef struct MemAcc_JobInfoTypeTag
{
    uint32               MemInstanceId;  /**< Instance ID of the current memory request @range 0..0xFFFFFFFF */
    MemAcc_LengthType    Length;         /**< Length of the currently active address area request @range NA */
    MemAcc_HwIdType      HwId;           /**< Referenced memory driver hardware identifier @range 0..0xFFFFFFFF */
    MemAcc_LengthType    MemLength;      /**< Length of memory driver request @range NA */
    MemAcc_JobType       CurrentJob;     /**< Currently active MemAcc job @range NA */
    MemAcc_JobResultType MemResult;      /**< Current or last Mem driver result @range NA */
    MemAcc_AddressType   LogicalAddress; /**< Address of currently active address area request @range NA */
    MemAcc_AddressType   MemAddress;     /**< Physical address of the current memory driver request @range NA */
} MemAcc_JobInfoType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

#ifdef __cplusplus
}
#endif

#endif
