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
 **  @file               : Det_Types.h
 **  @author             : haibin.shao
 **  @date               : 2024/09/25
 **  @vendor             : isoft
 **  @description        : Default Error Tracer
 **
 ***********************************************************************************************************************/
#ifndef DET_TYPES_H_
#define DET_TYPES_H_

/* =================================================== inclusions =================================================== */
#include "Std_Types.h"
#include "Det_Cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

/* The type is used to describe the running state in the DET module */
typedef enum
{
    DET_UNINITIALIZED = 0,
    DET_INITIALIZED   = 1,
    DET_STARTED       = 2
} Det_StateType;

/* The type is used to describe the store errors in the DET module */
typedef struct Det_EntryTypeTag /* PRQA S 1536 */ /* VL_Det_1536 */
{
    uint16 ModuleId;
    uint8  InstanceId;
    uint8  ApiId;
    uint8  ErrorId;
} Det_EntryType;

/* structure used to configure filters and store log data, using 0xFF for a filter item means don't care */
typedef struct Det_InfoTypeTag /* PRQA S 1536 */ /* VL_Det_1536 */
{
    uint16 ModuleId;
    uint8  InstanceId;
    uint8  ApiId;
    uint8  ErrorId;
} Det_InfoType;

/* structure to control the operation of DET debug extension */
typedef struct Det_StatusTypeTag /* PRQA S 1536 */ /* VL_Det_1536 */
{
    boolean LogActive;
} Det_StatusType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

#ifdef __cplusplus
}
#endif

#endif /* DET_TYPE_H_ */
