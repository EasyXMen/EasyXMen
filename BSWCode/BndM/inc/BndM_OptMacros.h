
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
 **  @file               : BndM_OptMacros.h
 **  @author             : tao.yu
 **  @date               : 2025/06/20
 **  @vendor             : isoft
 **  @description        : Optional macro definition for Bulk NvData Manager
 **
 ***********************************************************************************************************************/

#ifndef BNDM_OPTMACROS_H_
#define BNDM_OPTMACROS_H_

/* =================================================== inclusions =================================================== */
#include "Std_Types.h"

/** This macro definition represents an internal function of the Dem module */
#ifndef BNDM_LOCAL
#define BNDM_LOCAL static /* PRQA S 3414 */ /* VL_QAC_FctLikeMacro */
#endif

/* ===================================================== macros ===================================================== */
#ifndef BNDM_LOCAL_INLINE
#define BNDM_LOCAL_INLINE static inline /**< This macro definition represents the Dem module inline function */
#endif

#ifndef BNDM_UNUSED
#define BNDM_UNUSED(x) (void)(x) /**< This macro definition represents the Dem module unused parameter */
#endif

#define BNDM_BANK_HEAD_INFO_SIZE               \
    ((12u + (BNDM_NUMBER_OF_USER_BLOCKS * 4u)) \
     + ((12u + (BNDM_NUMBER_OF_USER_BLOCKS * 4u)) % 8)) /**< Indicate that the head size of the bank for BndM */

#define BNDM_MEM_ERASE_VALUE_INFO (4u) /**< Indicate that the memory erase value of the bank */

#ifndef BNDM_FEATURE_MEMCOPY_USED_ILIB
#define BNDM_FEATURE_MEMCOPY_USED_ILIB                                                                              \
    STD_OFF /**< This macro definition indicates the data copy, setup, and comparison of whether or not the library \
             * functions provided by ISoft are used.STD_ON then indicates that they are used, and STD_OFF indicates \
             * that they are used with the functions of the per-byte processing of the J1939Tp module. Using the    \
             * library function provided by ISoft has better performance. */
#endif

#define BNDM_POLLING_MODE STD_ON /**< This macro is used to define if BNDM uses the polling mode. */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

#endif /* BNDM_OPTMACROS_H_ */
