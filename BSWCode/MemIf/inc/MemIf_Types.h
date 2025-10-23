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
 **  @file               : MemIf_Types.h
 **  @author             : tao.yu
 **  @date               : 2024/09/19
 **  @vendor             : isoft
 **  @description        : Type definitions of MemIf
 **
 ***********************************************************************************************************************/

#ifndef MEMIF_TYPES_H_
#define MEMIF_TYPES_H_

/* =================================================== inclusions =================================================== */
#include "Std_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ================================================ type definitions ================================================ */

/**
 * @brief Denotes the current status of the underlying abstraction module and device drive.
 */
typedef enum
{
    MEMIF_UNINIT,       /**< The underlying abstraction module or device driver has not been initialized (yet). */
    MEMIF_IDLE,         /**< The underlying abstraction module or device driver is currently idle. */
    MEMIF_BUSY,         /**< The underlying abstraction module or device driver is currently busy */
    MEMIF_BUSY_INTERNAL /**< The underlying abstraction module is busy with internal management operations. The
                               underlying device driver can be busy or idle. */
} MemIf_StatusType;

/**
 * @brief Denotes the result of the last job.
 */
typedef enum
{
    MEMIF_JOB_OK,             /**< The job has been finished successfully */
    MEMIF_JOB_FAILED,         /**< The job has not been finished successfully */
    MEMIF_JOB_PENDING,        /**< The job has not yet been finished. */
    MEMIF_JOB_CANCELED,       /**< The job has been canceled. */
    MEMIF_BLOCK_INCONSISTENT, /**< 1. The requested block is inconsistent, it may contain corrupted data. 2. Block is
                                 NOT found. */
    MEMIF_BLOCK_INVALID       /**< The requested block has been marked as invalid, the requested operation can not be
                                 performed. */
} MemIf_JobResultType;

/**
 * @brief Denotes the mode type of the lower layer.(Reserved for compatibility with older versions of Mcal)
 */
typedef enum
{
    MEMIF_MODE_SLOW, /**< The underlying memory abstraction modules and drivers are working in slow mode. */
    MEMIF_MODE_FAST  /**< The underlying memory abstraction modules and drivers are working in fast mode. */
} MemIf_ModeType;

#ifdef __cplusplus
}
#endif
#endif
