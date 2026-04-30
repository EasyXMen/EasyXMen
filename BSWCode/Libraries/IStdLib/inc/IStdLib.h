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
 **  @file               : IStdLib.h
 **  @author             : darren.zhang,lizhi.huang,QinmeiChen
 **  @date               : 2024/09/11
 **  @vendor             : isoft
 **  @description        : stand lib header file
 **
 ***********************************************************************************************************************/
/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author       Description
 *  -------------------------------------------------------------------------------------------------------------------
 * V01.00.00 2023-12-12   darren        Initial version
 * V01.00.01 2024-01-02   darren        resolve QAC
 * V01.00.01 2024-01-02   darren        resolve short length handle bug
 * V01.00.00 2024-01-09   lizhi.huang   Initial version
 * V01.00.01 2024-01-11   lizhi.huang   Resolve QAC
 * V01.00.02 2024-01-31   lizhi.huang   Resolve malloc memory crossover
 * V01.00.02 2024-01-31   darren        1.added ASAN suppress macro
 *                                      2.modify IStdLib_MemCmp return type to int
 *                                      3.modify IStdLib_MemSet input parameter u8 to int
 * V01.00.02 2024-03-04   darren        resolve QAC
 * V01.00.03 2024-03-04   lizhi.huang   Resolve QAC
 * V01.00.02 2024-04-29   qinmei.chen   add T320 support by adjusting some bit and increasing HeadSize
 * V01.00.05 2024-05-29   lizhi.huang   Resolve memory crossover
 * V01.00.06 2024-06-20   lizhi.huang   Supports 64 bit high-end environments
 * V02.00.00 2024-09-11   qinmei.chen   fix compile error when macro ISTDLIB_BSW_NOT_USED_STDLIB is 0,
 *                                      Refactor and update to R23-11
 * V02.00.01 2025-04-28   darren        Resolve QAC
 *
===================================================================================================================== */
/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:IStd<br>
  RuleSorce:puhua_rules-2024.12.rcf
     \li VL_IStdLib_0306
      Reason: First, the pointer srcis cast to uintptr_t(an unsigned integer type capable of holding a pointer value).
      An integer offset (offset_vlu) is added to it, and the result is then cast back to a pointer type (const
      uintptr_t*). This pattern is used to perform byte-level pointer arithmetic that is not directly expressible with
      standard pointer arithmetic (which operates in units of the pointed-to type's size). In this context,
      it is a deliberate technique to calculate a new memory address after adjusting for
      a specific byte offset derived from prior alignment logic.
      Risk: No risk
      Prevention: None.

     \li VL_IStdLib_0750
      Reason: Fast variable type conversion, Guaranteed data consistency, Low space consumption.
      Risk: No risk.
      Prevention: None.

      \li VL_IStdLib_MacroFunc
      Reason: This usage of a function-like macro.The static analysis tool
      flags it because a function (or a static inline function) could potentially serve the same purpose.
      The arguments are properly parenthesized within the expansion, which guards against most operator precedence
      issues.
      Risk: No risk.
      Prevention: None.

      \li VL_IStdLib_Redundant
      Reason: Although redundant, it helps code understanding
      Risk: No risk.
      Prevention: None.

      \li VL_IStdLib_Mutable
      Reason: We allow the caller to modify the return value and this is necessary
      Risk: No risk.
      Prevention: None.

      \li VL_IStdLib_0314
      Reason: This function requires a void * return, and our own function ensures its security
      Risk: No risk.
      Prevention: None.

      \li VL_IStdLib_NullPointer
      Reason: Using the size of the structure as the offset to obtain memory ensures that it is not NULL.
      Risk: No risk.
      Prevention: None.

      \li VL_IStdLib_1258
      Reason: Data type conversion.
      Risk: No risk.
      Prevention: None.

      \li VL_IStdLib_1840
      Reason: Mathematical operations on pointers were performed based on the elimination of compilation warnings.
      Risk: No risk.
      Prevention: None.

      \li VL_IStdLib_3209
      Reason: Designed in accordance with the C standard library.
      Risk: No risk.
      Prevention: None.

      \li VL_MTR_IStdLib_STM19
      Reason: The multiple exit points of the function are designed to ensure the efficiency during runtime.
      Risk: No risk.
      Prevention: None.
 */
#ifndef ISTDLIB_H_
#define ISTDLIB_H_
/* =================================================== inclusions =================================================== */
#include "Std_Types.h"
#ifdef __cplusplus
extern "C" {
#endif
/* =============================================== version information ============================================== */
#define ISTDLIB_MODULE_ID                   (260u)
#define ISTDLIB_VENDOR_ID                   (62u)
#define ISTDLIB_AR_RELEASE_MAJOR_VERSION    (4u)
#define ISTDLIB_AR_RELEASE_MINOR_VERSION    (9u)
#define ISTDLIB_AR_RELEASE_REVISION_VERSION (0u)
#define ISTDLIB_SW_MAJOR_VERSION            (2u)
#define ISTDLIB_SW_MINOR_VERSION            (0u)
#define ISTDLIB_SW_PATCH_VERSION            (1u)
/* ===================================================== macros ===================================================== */
/* MemHeap Error Code */
#define ISTDLIB_MEMHEAP_OK            0U
#define ISTDLIB_MEMHEAP_INVALID_PTR   1U
#define ISTDLIB_MEMHEAP_INVALID_ALIGN 2U
#define ISTDLIB_MEMHEAP_INVALID_SIZE  3U
#define ISTDLIB_MEMHEAP_INVALID_INIT  4U

#if !defined(ISTDLIB_BSW_NOT_USED_STDLIB)
#define ISTDLIB_BSW_NOT_USED_STDLIB STD_ON
#endif

#if ISTDLIB_BSW_NOT_USED_STDLIB == STD_ON

#if !defined(ISTDLIB_BSW_ISTD_PARA_USED_RESTRICT)
#define ISTDLIB_BSW_ISTD_PARA_USED_RESTRICT STD_OFF
#endif

#if ISTDLIB_BSW_ISTD_PARA_USED_RESTRICT == STD_ON
#define ISTDLIB_RESTRICT __restrict__
#else
#define ISTDLIB_RESTRICT
#endif

#if !defined(ISTDLIB_MEMHEAP_STATISTIC)
#define ISTDLIB_MEMHEAP_STATISTIC STD_ON
#endif

#ifndef ISTDLIB_LOCAL
#define ISTDLIB_LOCAL static /* PRQA S 3414 */ /* VL_QAC_FctLikeMacro */
#endif

#ifndef ISTDLIB_LOCAL_INLINE
#define ISTDLIB_LOCAL_INLINE static inline
#endif
/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief       copy from srcptr to dstptr
 * @param[out]  dstptr : Points to the target array used to store the copied content
 * @param[in]   srcptr : Points to the data source you want to copy
 * @param[in]   length : The number of bytes to be copied
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       -
 */
/*PRQA S 3209 ++ */ /* VL_IStdLib_3209 */
void* IStdLib_MemCpy(void* ISTDLIB_RESTRICT dstptr, const void* ISTDLIB_RESTRICT srcptr, uint32 length);
/*PRQA S 3209 -- */
/**
 * @brief       Fill the destination address with val, which is n bytes in length
 * @param[out]  dstptr : Points to the target array used to store the copied content
 * @param[in]   val    : Fill the value of memory
 * @param[in]   n : The number of bytes to be copied
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       -
 */
void* IStdLib_MemSet(void* ISTDLIB_RESTRICT dstptr, uint8 val, uint32 n);

/**
 * @brief     compare str1ptr and strptr2, which is n bytes in length
 * @param[in] str1ptr : Points to the array str1
 * @param[in] str2ptr : Points to the array str2
 * @param[in] n    : The number of bytes to be compared
 * @return    int
 * @retval    0 : str1ptr is equal to str2ptr
 * @retval    1 : other condition.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       -
 */
StatusType IStdLib_MemCmp(const void* ISTDLIB_RESTRICT str1ptr, const void* ISTDLIB_RESTRICT str2ptr, uint32 n);

#else
#include <string.h>

#if !defined(IStdLib_MemCpy)
#define IStdLib_MemCpy memcpy
#endif
#if !defined(IStdLib_MemSet)
#define IStdLib_MemSet memset
#endif
#if !defined(IStdLib_MemCmp)
#define IStdLib_MemCmp memcmp
#endif

#endif

/**
 * @brief        init heap
 * @param[in]    size : the size of ram
 * @param[inout] ram : the whole of memory space
 * @return       uint8
 * @retval        ISTDLIB_MEMHEAP_INVALID_PTR   : the ram is illegal pointer
 * @retval        ISTDLIB_MEMHEAP_INVALID_SIZE  : unable to manage space larger than 64K or smaller than the size of
 * memheap head
 * @retval        ISTDLIB_MEMHEAP_INVALID_INIT  : repeated initialization
 * @retval        ISTDLIB_MEMHEAP_INVALID_ALIGN : the address of ram is not byte aligned
 * @retval        ISTDLIB_MEMHEAP_OK            : Function completed successfully
 * @reentrant    Reentrant for different heap. Non reentrant for the same heap.
 * @synchronous  TRUE
 * @trace        -
 */
uint8 IStdLib_MemHeapInit(void* ramHeap, uint32 size);

/**
 * @brief        malloc block depending on size
 * @param[inout] ram : the whole of memory space
 * @param[in]    size : request size
 * @reentrant    Reentrant for different heap. Non reentrant for the same heap.
 * @synchronous  TRUE
 * @trace        -
 */
void* IStdLib_MemHeapMalloc(void* ramHeap, uint32 size);

/**
 * @brief        malloc block depending on size, and fill 0
 * @param[inout] ramHeap   : the whole of memory space
 * @param[in]    count : request count
 * @param[in]    size  : request size
 * @reentrant    Reentrant for different heap. Non reentrant for the same heap.
 * @synchronous  TRUE
 * @trace        -
 */
void* IStdLib_MemHeapCalloc(void* ramHeap, uint32 count, uint32 size);

/**
 * @brief        free block to heap
 * @param[inout] ramHeap : the whole of memory space
 * @param[in]    ptr : memory pointer to be released
 * @return       uint8
 * @retval        ISTDLIB_MEMHEAP_INVALID_PTR   : the ram or the pointer is illegal pointer or the pointer is not in
 * valid range
 * @retval        ISTDLIB_MEMHEAP_INVALID_INIT  : uninitialized
 * @retval        ISTDLIB_MEMHEAP_INVALID_ALIGN : the address of ram is not byte aligned
 * @retval        ISTDLIB_MEMHEAP_OK            : Function completed successfully
 * @reentrant    Reentrant for different heap. Non reentrant for the same heap.
 * @synchronous  TRUE
 * @trace        -
 */
uint8 IStdLib_MemHeapFree(void* ramHeap, const void* ptr);

#if ISTDLIB_MEMHEAP_STATISTIC == STD_ON
/**
 * @brief       get the heap size
 * @param[in]   ramHeap  : the whole of memory space
 * @param[out]  size : the manage size
 * @return      uint8
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       -
 */
uint8 IStdLib_MemHeapGetManageSize(const void* ramHeap, uint32* size);

/**
 * @brief       get the total assignable size
 * @param[in]   ramHeap  : the whole of memory space
 * @param[out]  size : real max size
 * @return      uint8
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       -
 */
uint8 IStdLib_MemHeapGetRealMaxSize(const void* ramHeap, uint32* size);

/**
 * @brief       get the assigned size
 * @param[in]   ram  : the whole of memory space
 * @param[out]  size : used size
 * @return      uint8
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       -
 */
uint8 IStdLib_MemHeapGetUsedSize(const void* ramHeap, uint32* size);

/**
 * @brief       get the max free block size
 * @param[in]   ramHeap : the whole of memory space
 * @param[out]  size : the max size of block in manage space
 * @return      uint8
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       -
 */
uint8 IStdLib_MemHeapGetCurFreeMaxBlockSize(const void* ramHeap, uint32* size);

/**
 * @brief       get the max assigned size
 * @param[in]   ramHeap  : the whole of memory space
 * @param[out]  size : the max malloc size
 * @return      uint8
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       -
 */
uint8 IStdLib_MemHeapGetMaxMallocSize(const void* ramHeap, uint32* size);

#endif

#ifdef __cplusplus
}
#endif

#endif
