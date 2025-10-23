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
 **  @file               : CanNm_Internal.h
 **  @author             : caihong.liu
 **  @date               : 2024/12/12
 **  @vendor             : isoft
 **  @description        : Can network manager internal
 **
 ***********************************************************************************************************************/

/* ================================================ misar justifications ============================================ */

#ifndef CANNM_INTERNAL_H_
#define CANNM_INTERNAL_H_

/* =================================================== inclusions =================================================== */
#include "CanNm_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */

/* ===================================================== macros ===================================================== */
/**
 *  |       | Byte 0/1                    | Value |
 *  | ----- | --------------------------- | ----- |
 *  | Bit 7 | Reserved                    | -     |
 *  | Bit 6 | Partial Network Information | 0x40  |
 *  | Bit 5 | Partial Network Learning    | 0x20  |
 *  | Bit 4 | Active Wakeup               | 0x10  |
 *  | Bit 3 | NM Coordinator Sleep Ready  | 0x08  |
 *  | Bit 2 | Reserved                    | -     |
 *  | Bit 1 | PN Shutdown Request Bit     | 0x02  |
 *  | Bit 0 | Repeat Message Request      | 0x01  |
 *  */
/**
 * @name CBV Mask of CanNm
 * @{
 * @brief CBV Mask
 */
#define CANNM_CBV_BIT_RMP_MASK ((uint8)(0x01u)) /**< Repeat Message Request bit of the cbv field in NM-Pdu */
#define CANNM_CBV_BIT_CSR_MASK ((uint8)(0x08u)) /**< RNM Coordinator Sleep Ready bit of the cbv field in NM-Pdu */
#define CANNM_CBV_BIT_AW_MASK  ((uint8)(0x10u)) /**< Active Wakeup bit of the cbv field in NM-Pdu */
#define CANNM_CBV_BIT_PNI_MASK ((uint8)(0x40u)) /**< Partial Network Information bit of the cbv field in NM-Pdu */
/** @} */

#define CANNM_SINGLEBIT01_BASENUM    (0x01u) /**< One byte of 1 */
#define CANNM_USER_DATA_INITIAL_BYTE (0xFFu) /**< One byte of all 1's */

/* ================================================ type definitions ================================================ */
/**
 * @brief Pointer to the clock timeout handler function.
 */
typedef void (*CanNm_TimerTimoutHandleType)(CanNm_ChannelIndexType chIndex);

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */
#if CANNM_MULTIPLE_PARTITION_USED == STD_ON
/**
 * @brief An array of partitions involved by the CanNm module.
 */
extern const ApplicationType CanNm_OsApplicationList[CANNM_NUMBER_OF_PARTITION]; /**< @range NA */
#endif

/**
 * @brief The channel configuration structure of the CanNm module.
 */
extern const CanNm_ChannelLConfigType CanNm_ChannelLCfgData[CANNM_NUMBER_OF_CHANNEL]; /**< @range NA */
/* ========================================= external function declarations ========================================= */

#ifdef __cplusplus
}
#endif
#endif
