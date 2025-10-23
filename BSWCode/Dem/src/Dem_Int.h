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
 **  @file               : Dem_Int.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : Implementation file of Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_INT_H
#define DEM_INT_H

/* =================================================== inclusions =================================================== */
#include "Dem_Types.h"
#include "SchM_Dem.h"

/* PRQA S 3472 ++ */ /* VL_Dem_3472 */
/* ===================================================== macros ===================================================== */
/**
 * @name  DEM_BIT_FIELD_ENCODE
 * @{
 * @brief Macros encoding for bitfield length
 */
#define DEM_1BYTE 1 /**< bitfield length of one byte width (8bit) */
#define DEM_2BYTE 2 /**< bitfield length of two byte width (16bit) */
#define DEM_4BYTE 4 /**< bitfield length of four byte width (32bit) */
/** @} */

/**
 * @name  DEM_BIT_MASK_ENCODE
 * @{
 * @brief Macros encoding for bitmask length
 */
#define DEM_MASK_1 0xffu        /**< bitmask of one byte width (8bit) */
#define DEM_MASK_2 0xffffu      /**< bitmask of two byte width (16bit) */
#define DEM_MASK_4 0xffffffffuL /**< bitmask of four byte width (32bit) */
/** @} */

/* PRQA S 0342 ++ */                   /* VL_Dem_0342 */
#define DEM_MASK(size) DEM_MASK_##size /**< bitmask builder for mask of 'size' byte width */
/* PRQA S 0342 -- */

/**
 * @name  DEM_BIT_SET_ENCODE
 * @{
 * @brief Macros encoding for bit set
 */
#define DEM_BIT(pos) (0x01u << (pos)) /**< bitmask with bit 'pos' set */
#define DEM_BIT_MASK(pos, size) \
    (DEM_MASK(size) ^ DEM_BIT(pos)) /**< bitmask of 'size' byte width, with all bits but 'pos' set */
#define DEM_NBIT(n, pos) ((DEM_MASK(1) >> (8u - (n))) << (pos)) /**< bitmask with n bits set, starting at 'pos' */
#define DEM_NBIT_MASK(n, pos, size) \
    (DEM_MASK(size)                 \
     ^ DEM_NBIT(n, pos)) /**< bitmask of 'size' byte width, with all bits set, but n bits starting at 'pos' */
#define DEM_TEST_BIT(mask, pos) ((((mask) & DEM_BIT(pos))) != 0u) /**< Test if bit 'pos' in a bitmask is set */
#define DEM_SET_BIT(mask, pos)                                                             \
    ((mask) | DEM_BIT(pos)) /**< bitmask with bit 'pos' set without changing existing bits \
                             */
#define DEM_RESET_BIT(mask, pos, size) \
    ((mask) & DEM_BIT_MASK(pos, size)) /**< bitmask with bit 'pos' reset without changing existing bits */
/** @} */

/**
 * @name  DEM_GET_BIT_ENCODE
 * @{
 * @brief Macros encoding for bit get
 */
#define Dem_GetHiByte(value) \
    ((uint8)((uint16)(value) >> 8u))            /**< Get the high byte of a 16 bit value. Used to serialize data */
#define Dem_GetLoByte(value) ((uint8)((value))) /**< Get the low byte of a 16 bit value. Used to serialize data */
#define Dem_GetHiHiByte(value) \
    ((uint8)((uint32)(value) >> 24u)) /**< Get the high word, high byte of a 32 bit value. Used to serialize data */
#define Dem_GetHiLoByte(value) \
    ((uint8)((uint32)(value) >> 16u)) /**< Get the high word, low byte of a 32 bit value. Used to serialize data */
#define Dem_GetLoHiByte(value) \
    ((uint8)((uint32)(value) >> 8u)) /**< Get the low word, high byte of a 32 bit value. Used to serialize data */
#define Dem_GetLoLoByte(value) \
    ((uint8)(value)) /**< Get the low word, low byte of a 32 bit value. Used to serialize data */
/** @} */

/**
 * @name  DEM_BIT_STATUS_ENCODE
 * @{
 * @brief Status Bits Functions
 *        _p = &uint8_array[0]
 *        _n = pos
 */
#define DEM_BITS_SET(_p, _n)    ((_p)[((_n) >> 3u)] |= ((uint8)(1u << ((_n) & 7u))))             /**< Set bit */
#define DEM_BITS_RESET(_p, _n)  ((_p)[((_n) >> 3u)] &= ((uint8)(~((uint8)(1u << ((_n) & 7u)))))) /**< Reset bit */
#define DEM_BITS_ISSET(_p, _n)  (((_p)[((_n) >> 3u)] & ((uint8)(1u << ((_n) & 7u)))) != 0u)      /**< Test bit */
#define DEM_BITS_ISNSET(_p, _n) (((_p)[((_n) >> 3u)] & ((uint8)(1u << ((_n) & 7u)))) == 0u)      /**< Test bit */
/** @} */

/**
 * @name  DEM_FLAG_STATUS_ENCODE
 * @{
 * @brief Status flag Functions
 *        _status = Status data
 *        _val = mask
 */
#define DEM_FLAG_SET(_status, _val)    ((_status) |= (_val))            /**< Set flag */
#define DEM_FLAG_RESET(_status, _val)  ((_status) &= (uint32)(~(_val))) /**< Reset flag */
#define DEM_FLAG_ISSET(_status, _val)  (((_status) & (_val)) != 0u)     /**< Test flag */
#define DEM_FLAG_ISNSET(_status, _val) (((_status) & (_val)) == 0u)     /**< Test flag */
/** @} */

/** --------------------- Critical Sections------------------------ */
/**
 * @name  DEM_CRITICAL_ENCODE
 * @{
 * @brief Critical Functions
 */
#define Dem_EnterCritical_DiagMonitor() \
    SchM_Enter_Dem_DEM_EXCLUSIVE_AREA_0() /**< Enter critical section 'DiagMonitor' */
#define Dem_LeaveCritical_DiagMonitor()                                            \
    SchM_Exit_Dem_DEM_EXCLUSIVE_AREA_0() /**< Leave critical section 'DiagMonitor' \
                                          */
#define Dem_EnterCritical_StateManager() \
    SchM_Enter_Dem_DEM_EXCLUSIVE_AREA_1() /**< Enter critical section 'StateManager' */
#define Dem_LeaveCritical_StateManager() \
    SchM_Exit_Dem_DEM_EXCLUSIVE_AREA_1()                                 /**< Leave critical section 'StateManager' */
#define Dem_EnterCritical_DcmApi() SchM_Enter_Dem_DEM_EXCLUSIVE_AREA_2() /**< Enter critical section 'DcmAPI' */
#define Dem_LeaveCritical_DcmApi() SchM_Exit_Dem_DEM_EXCLUSIVE_AREA_2()  /**< Leave critical section 'DcmAPI' */
#define Dem_EnterCritical_CrossCoreComm() \
    SchM_Enter_Dem_DEM_EXCLUSIVE_AREA_3() /**< Enter critical section 'CrossCoreCommunication' */
#define Dem_LeaveCritical_CrossCoreComm() \
    SchM_Exit_Dem_DEM_EXCLUSIVE_AREA_3() /**< Leave critical section 'CrossCoreCommunication' */
/** @} */

/* PRQA S 3472 -- */

#endif

/* ----------------------------------------------------- end of file ------------------------------------------------ */
