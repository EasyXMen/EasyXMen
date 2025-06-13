/* PRQA S 3108++ */
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
/* PRQA S 3108-- */
/*************************************************************************************************************************
**                                                                                                                    **
**  FILENAME    : istd_lib.h                                                                                          **
**                                                                                                                    **
**  Created on  : 12/12/23                                                                                            **
**  Author      : darren.zhang                                                                                        **
**  Vendor      :                                                                                                     **
**  DESCRIPTION :  stand lib header file                                                                              **
**                                                                                                                    **
**  SPECIFICATION(S) :   AUTOSAR classic Platform                                                                     **
**                                                                                                                    **
***********************************************************************************************************************/
/***********************************************************************************************************************
**                                               Revision Control History                                             **
***********************************************************************************************************************/
/* <VERSION> <DATE> <AUTHOR> <REVISION LOG>
 * V2.0.0 [2024/9/10] [QinmeiChen] fix compile error when macro BSW_NOT_USED_STDLIB is 0.
 */
/*===================================================================================================================*/
#ifndef ISOFT_STD_LIB_H_
#define ISOFT_STD_LIB_H_

#include "Std_Types.h"

/*======================================================================================================================
 *                                                  Macro Definitions
======================================================================================================================*/

#if !defined(BSW_NOT_USED_STDLIB)
#define BSW_NOT_USED_STDLIB STD_ON
#endif

#if BSW_NOT_USED_STDLIB == STD_ON

#if !defined(BSW_ILIB_PARA_USED_RESTRICT)
#define BSW_ILIB_PARA_USED_RESTRICT STD_OFF
#endif

#if BSW_ILIB_PARA_USED_RESTRICT == STD_ON
#define ILIB_RESTRICT __restrict__
#else
#define ILIB_RESTRICT
#endif

#if !defined(MEMHEAP_STATISTIC)
#define MEMHEAP_STATISTIC STD_ON
#endif

BEGIN_C_DECLS
/**
 * @ingroup ilib
 * @brief isoft memcpy
 * @param[in] __dst Points to the target array used to store the copied content
 * @param[in] __src Points to the data source you want to copy
 * @param[in] __len The number of bytes to be copied
 * @return This function returns a pointer to the target store __dst.
 */
void* ILib_memcpy(void* ILIB_RESTRICT __dst, const void* ILIB_RESTRICT __src, uint32 __len);

/**
 * @ingroup ilib
 * @brief isoft memset
 * @param[in] __dst Points to the memory block to be filled
 * @param[in] __val The value to be set each byte
 * @param[in] __n The number of characters to be set to this value
 * @return This value returns a pointer to the store __dst.
 */
void* ILib_memset(void* ILIB_RESTRICT __dst, int __val, uint32 __n);

/**
 * @ingroup ilib
 * @brief isoft memcmp
 * @param[in] __str1 Points to the array str1
 * @param[in] __str2 Points to the array str2
 * @param[in] __n The number of bytes to be compared
 * @return 0:__str1 is equal to __str2,1:other condition.
 */
int ILib_memcmp(const void* ILIB_RESTRICT __str1, const void* ILIB_RESTRICT __str2, uint32 __n);

END_C_DECLS

#else
#include <string.h>

#if !defined(ILib_memcpy)
#define ILib_memcpy memcpy
#endif
#if !defined(ILib_memset)
#define ILib_memset memset
#endif
#if !defined(ILib_memcmp)
#define ILib_memcmp memcmp
#endif

#endif /* BSW_NOT_USED_STDLIB */

/* MemHeap Error Code */
#define MEMHEAP_OK            0U
#define MEMHEAP_INVALID_PTR   1U
#define MEMHEAP_INVALID_ALIGN 2U
#define MEMHEAP_INVALID_SIZE  3U
#define MEMHEAP_INVALID_INIT  4U

BEGIN_C_DECLS

/**
 * @ingroup ilib
 * @brief isoft ILib_MemHeapInit
 * @param[inout] ram the whole of memory space
 * @param[in] size the size of ram
 * @return error code
 */
uint8 ILib_MemHeapInit(void* ram, uint32 size);

/**
 * @ingroup ilib
 * @brief isoft ILib_MemHeapMalloc
 * @param[inout] ram the whole of memory space
 * @param[in] size request size
 * @return memory address
 */
void* ILib_MemHeapMalloc(void* ram, uint32 size);

/**
 * @ingroup ilib
 * @brief isoft ILib_MemHeapCalloc
 * @param[inout] ram the whole of memory space
 * @param[in] count request count
 * @param[in] size request size
 * @return memory address
 */
void* ILib_MemHeapCalloc(void* ram, uint32 count, uint32 size);

/**
 * @ingroup ilib
 * @brief isoft ILib_MemHeapFree
 * @param[inout] ram the whole of memory space
 * @param[in] ptr memory pointer to be released
 * @return error code
 */
uint8 ILib_MemHeapFree(void* ram, void* ptr);

#if MEMHEAP_STATISTIC == STD_ON
/**
 * @ingroup ilib
 * @brief isoft ILib_GetManageSize
 * @param[in] ram the whole of memory space
 * @param[out] size the manage size
 * @return error code
 */
uint8 ILib_MemHeapGetManageSize(const void* ram, uint32* size);

/**
 * @ingroup ilib
 * @brief isoft ILib_GetRealMaxSize
 * @param[in] ram the whole of memory space
 * @param[out] size real max size
 * @return error code
 */
uint8 ILib_MemHeapGetRealMaxSize(const void* ram, uint32* size);

/**
 * @ingroup ilib
 * @brief isoft ILib_GetUsedSize
 * @param[in] ram the whole of memory space
 * @param[out] size used size
 * @return error code
 */
uint8 ILib_MemHeapGetUsedSize(const void* ram, uint32* size);

/**
 * @ingroup ilib
 * @brief isoft ILib_GetCurMaxBlockSize
 * @param[in] ram the whole of memory space
 * @param[out] size the max size of block in manage space
 * @return error code
 */
uint8 ILib_MemHeapGetCurFreeMaxBlockSize(const void* ram, uint32* size);

/**
 * @ingroup ilib
 * @brief isoft ILib_GetMaxMallocSize
 * @param[in] ram the whole of memory space
 * @param[out] size the max malloc size
 * @return error code
 */
uint8 ILib_MemHeapGetMaxMallocSize(const void* ram, uint32* size);

#endif

END_C_DECLS

#endif /* ISOFT_STD_LIB_H_ */
