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
 ***********************************************************************************************************************
 **
 **  @file               : ComM_Internal.h
 **  @author             : xiaojian.liang
 **  @date               : 2024/08/21
 **  @vendor             : isoft
 **  @description        : ComM Interface Private Implementation header
 **
 **********************************************************************************************************************/

#ifndef COMM_INTERNAL_H_
#define COMM_INTERNAL_H_

/* =================================================== inclusions =================================================== */
#include "ComM.h"

#if COMM_MULTIPLE_PARTITION_ENABLED == STD_ON
#include "Os.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */
#define COMM_UNUSED_PARAM(param) (void)(param)
#define COMM_CHAR_BIT            (8UL)
/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */
/**
 * This function shall set the logical status of input data as '1' at the requested bit position.
 * @param[in]    data   Pointer to input data
 * @param[in]    bitPn  Bit position
 * @reentrant    TRUE (Reentrant for different input data. Non reentrant for the same input data.)
 * @synchronous  TRUE
 * @trace        CPD-65014
 */
COMM_LOCAL_INLINE void ComM_SetBit(uint8* data, uint32 bitPn)
{
    data[bitPn / COMM_CHAR_BIT] |= (uint8)(1u << (bitPn % COMM_CHAR_BIT));
}

/**
 * This function shall clear the logical status of the input data to '0' at the requested bit position.
 * @param[in]    data   Pointer to input data
 * @param[in]    bitPn  Bit position
 * @reentrant    TRUE (Reentrant for different input data. Non reentrant for the same input data.)
 * @synchronous  TRUE
 * @trace        CPD-65015
 */
COMM_LOCAL_INLINE void ComM_ClrBit(uint8* data, uint32 bitPn)
{
    data[bitPn / COMM_CHAR_BIT] &= (uint8) ~(uint8)(1u << (bitPn % COMM_CHAR_BIT));
}

#ifdef QAC_ANALYZE
#pragma PRQA_NO_SIDE_EFFECTS ComM_GetBit
#endif

/**
 * This function shall return the logical status of the input data for the requested bit position.
 * @param[in]    data   Pointer to input data
 * @param[in]    bitPn  Bit position
 * @retval       Bit Status
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-65016
 */
COMM_LOCAL_INLINE boolean ComM_GetBit(const uint8* data, uint32 bitPn)
{
    return (data[bitPn / COMM_CHAR_BIT] & (uint8)(1u << (bitPn % COMM_CHAR_BIT))) != 0u;
}

/* =========================================== external data declarations =========================================== */
#if COMM_MULTIPLE_PARTITION_ENABLED == STD_ON
extern const ApplicationType ComM_OsApplications[COMM_PARTITION_NUMBER];
#endif

#if COMM_PNC_SUPPORT == STD_ON
extern boolean ComM_PncEnabled;
#endif

/* ========================================= external function declarations ========================================= */
/**
 * @brief This function shall perform the processing of the AUTOSAR ComM activities that are not directly initiated by
 * the calls. e.g. from the RTE. There shall be one dedicated Main Function for each channel of ComM.
 * @param[in]    Channel  See NetworkHandleType
 * @reentrant    TRUE (Reentrant for different channels. Non reentrant for the same channel.)
 * @synchronous  TRUE
 * @trace        CPD-61360
 */
void ComM_MainFunction(NetworkHandleType Channel);

#ifdef __cplusplus
}
#endif

#endif /* COMM_INTERNAL_H_ */
