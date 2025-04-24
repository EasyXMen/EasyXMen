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
********************************************************************************
**                                                                            **
**  FILENAME    : MemIf_Types.h                                               **
**                                                                            **
**  Created on  : 2020/08/19                                                  **
**  Author      : Haibin.Shao                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : MemIf Types header description                              **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

/******************************************************************************
**                      Revision Control History                             **
******************************************************************************/
/*  <VERSION>    <DATE>      <AUTHOR>        <REVISION LOG>
 *  V1.0.0     2020-08-19   Haibin.Shao   R19_11 Ea initial version.
 */
#ifndef MEMIF_TYPES_H_
#define MEMIF_TYPES_H_

/*==============================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/
#include "Std_Types.h"

/*==============================================================================
*                              SOURCE FILE VERSION INFORMATION
==============================================================================*/

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

/* A memory hardware abstraction module's state is denoted in
 * this type. Mainly used in MemIf_GetStatus and related
 * APIs of memory hardware abstraction modules.
 */
typedef enum
{
    /* The underlying abstraction module or device driver has not been initialized (yet). */
    MEMIF_UNINIT = 0,
    /* The underlying abstraction module or device driver is currently idle. */
    MEMIF_IDLE = 1,
    /* The underlying abstraction module or device driver is currently busy. */
    MEMIF_BUSY = 2,
    /* The underlying abstraction module is busy with internal management operations. The underlying device driver can
     * be busy or idle. */
    MEMIF_BUSY_INTERNAL = 3
} MemIf_StatusType;

/* A requested job can have one these values after it has been
 * processed. Use in MemIf_GetJobResult and related memory
 * hardware abstraction module APIs.
 */
typedef enum
{
    /*The job has been finished successfully.*/
    MEMIF_JOB_OK = 0,
    /*The job has not been finished successfully.*/
    MEMIF_JOB_FAILED = 1,
    /*The job has not yet been finished.*/
    MEMIF_JOB_PENDING = 2,
    /*The job has been canceled.*/
    MEMIF_JOB_CANCELED = 3,
    /*1. The requested block is inconsistent, it may contain corrupted data. 2. Block is NOT found.*/
    MEMIF_BLOCK_INCONSISTENT = 4,
    /*The requested block has been marked as invalid, the requested operation can not be performed.*/
    MEMIF_BLOCK_INVALID = 5
} MemIf_JobResultType;

/* Specifies the processing speed of requested jobs. The exact
 * processing duration in the different modes depends on
 * configuration of the underlying memory hardware abstraction
 * modules.
 */
typedef enum
{
    /*The underlying memory abstraction modules and drivers are working in slow mode.*/
    MEMIF_MODE_SLOW = 0,
    /*The underlying memory abstraction modules and drivers are working in fast mode.*/
    MEMIF_MODE_FAST = 1
} MemIf_ModeType;

/*---- API CFG Type definitions --------------------------------------------*/
/* type definitions for referencing APIs of underlying memory hardware abstraction modules */
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
typedef P2FUNC(void, MEMIF_CODE, MemIf_SetModeType)(MemIf_ModeType Mode);
typedef P2FUNC(
    Std_ReturnType,
    MEMIF_CODE,
    MemIf_ApiReadType)(uint16 BlockNumber, uint16 BlockOffset, uint8* DataBufferPtr, uint16 Length);
typedef P2FUNC(Std_ReturnType, MEMIF_CODE, MemIf_ApiWriteType)(uint16 BlockNumber, uint8 const* DataBufferPtr);
typedef P2FUNC(Std_ReturnType, MEMIF_CODE, MemIf_ApiEraseImmediateBlockType)(uint16 BlockNumber);
typedef P2FUNC(Std_ReturnType, MEMIF_CODE, MemIf_ApiInvalidateBlockType)(uint16 BlockNumber);
typedef P2FUNC(void, MEMIF_CODE, MemIf_ApiCancelType)(void);
typedef P2FUNC(MemIf_StatusType, MEMIF_CODE, MemIf_ApiGetStatusType)(void);
typedef P2FUNC(MemIf_JobResultType, MEMIF_CODE, MemIf_ApiGetJobResultType)(void);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/* configuration structure, handles the API mapping */
typedef struct
{
    MemIf_SetModeType SetMode;
    MemIf_ApiReadType Read;
    MemIf_ApiWriteType Write;
    MemIf_ApiEraseImmediateBlockType EraseImmediateBlock;
    MemIf_ApiInvalidateBlockType InvalidateBlock;
    MemIf_ApiCancelType Cancel;
    MemIf_ApiGetStatusType GetStatus;
    MemIf_ApiGetJobResultType GetJobResult;
} MemIf_MemHwAApi_Type;

#endif
