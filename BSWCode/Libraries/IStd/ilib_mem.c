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
/*************************************************************************************************************************
**                                                                                                                    **
**  FILENAME    : ilib_mem.c                                                                                          **
**                                                                                                                    **
**  Created on  : 12/12/23                                                                                            **
**  Author      : darren.zhang                                                                                        **
**  Vendor      :                                                                                                     **
**  DESCRIPTION :  stand lib source code                                                                              **
**                                                                                                                    **
**  SPECIFICATION(S) :   AUTOSAR classic Platform                                                                     **
**                                                                                                                    **
***********************************************************************************************************************/

/*======================================================================================================================
 *                                       REVISION HISTORY
======================================================================================================================*/
/** <VERSION>  <DATE>    <AUTHOR>      <REVISION LOG>
 *  V1.0.0    20231212   darren         Initial version
 *  V1.0.1    20240102   darren         resolve QAC
 *  V1.0.1    20240102   darren         resolve short length handle bug
 *  V1.0.2    20240131   darren         1.added ASAN suppress macro
 *                                      2.modify ILib_memcmp return type to int
 *                                      3.modify ILib_memset input parameter u8 to int
 *  V1.0.2    20240304   darren         resolve QAC
 *  V1.0.2    20240429   qinmei.chen    add T320 support by adjusting some bit
 *
 */

/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:Ilib<br>
  RuleSorce:puhua-rule.rcf 2.3.1

    \li PRQA S 3332 MISRA Rule 20.9 <br>
    Reason:The corresponding macro definition has been obtained through the header file inclusion.

    \li PRQA S 0602 MISRA Rule 1.3,MISRA Rule 21.2 <br>
    Reason:The module is positioned as a library function

    \li PRQA S 0306 MISRA Rule 11.4 <br>
    Reason:Determine that the pointer address must have a type conversion

    \li PRQA S 0488 MISRA Rule 18.4 <br>
    Reason:The pointer is offset, intentionally taking advantage of platform characteristics

    \li PRQA S 3440 MISRA Rule 13.3 <br>
    Reason:The pointer grows by itself and has been verified to function properly

    \li PRQA S 3440,3387 MISRA Rule 13.3 <br>
    Reason:The pointer grows by itself and has been verified to function properly

    \li PRQA S 2001 MISRA Rule-15.1 <br>
    Reason: Based on operational performance, it has been verified by unit tests

    \li PRQA S 3472,3469 MISRA Dir-4.9 <br>
    Reason: Based on operational performance, it has been verified by unit tests

    \li PRQA S 3472,3469 MISRA Dir-4.9 <br>
    Reason: Based on operational performance, it has been verified by unit tests

 */

/*=================================================[inclusions]=======================================================*/

#include <stdint.h>
#include "istd_lib.h"

/*======================================================================================================================
 *                                                      Version
======================================================================================================================*/

/*======================================================================================================================
 *                                                  Version check
======================================================================================================================*/

/*=================================================[macros]===========================================================*/
#if BSW_NOT_USED_STDLIB == STD_ON
/* paltform used value */
#if (CPU_TYPE == CPU_TYPE_64) /* PRQA S 3332 */ /* MISRA Rule 20.9 */
/* pc environment 64bit */
#define SUFFIX_BIT_LEN   3u
#define IS_ALIGEN_ADR(x) (((uintptr_t)(x) << 61u) == 0u) /* PRQA S 3472 */  /* MISRA Dir-4.9 */
#define SUFFIX_VLU(x)    (((uintptr_t)(x) << 61u) >> 61u) /* PRQA S 3472 */ /* MISRA Dir-4.9 */

#elif (CPU_TYPE == CPU_TYPE_32)
/* normal 32-bit mcu */

#if (CPU_32_WITH_16_ADR == TRUE)
#define SUFFIX_BIT_LEN   1u
#define IS_ALIGEN_ADR(x) (((uintptr_t)(x) << 31u) == 0u) /* PRQA S 3472 */  /* MISRA Dir-4.9 */
#define SUFFIX_VLU(x)    (((uintptr_t)(x) << 31u) >> 31u) /* PRQA S 3472 */ /* MISRA Dir-4.9 */
#else
#define SUFFIX_BIT_LEN   2u
#define IS_ALIGEN_ADR(x) (((uintptr_t)(x) << 30u) == 0u) /* PRQA S 3472 */  /* MISRA Dir-4.9 */
#define SUFFIX_VLU(x)    (((uintptr_t)(x) << 30u) >> 30u) /* PRQA S 3472 */ /* MISRA Dir-4.9 */
#endif

#elif (CPU_TYPE == CPU_TYPE_16)
/*  16-bit mcu */
#define SUFFIX_BIT_LEN   1u
#define IS_ALIGEN_ADR(x) (((uintptr_t)(x) << 15u) == 0u) /* PRQA S 3472 */  /* MISRA Dir-4.9 */
#define SUFFIX_VLU(x)    ((((uintptr_t)x) << 15u) >> 15u) /* PRQA S 3472 */ /* MISRA Dir-4.9 */
#else
/* default 32-bit mcu */
#define SUFFIX_BIT_LEN   2u
#define IS_ALIGEN_ADR(x) (((uintptr_t)(x) << 30u) == 0u) /* PRQA S 3472 */  /* MISRA Dir-4.9 */
#define SUFFIX_VLU(x)    (((uintptr_t)(x) << 30u) >> 30u) /* PRQA S 3472 */ /* MISRA Dir-4.9 */
#endif                                                                      /* SIZE_MAX */
#if (CPU_TYPE != CPU_TYPE_8)
/* PRQA S 3472++ */ /* MISRA Dir-4.9 */
/* Gets the forward-aligned address of the current address */
#define GET_ALIGEN_FORWARD_ADR(x) (((uintptr_t)(x) >> SUFFIX_BIT_LEN) << SUFFIX_BIT_LEN)
/* Gets the 128-bit forward-aligned address of the current address */
#define GET_ALIGEN_128_FORWARD_ADR(x) \
    (((uintptr_t)(x) > (sizeof(uintptr_t) << 2u)) ? ((uintptr_t)(x) - (sizeof(uintptr_t) << 2u)) : 0u)
/* Gets the backward aligned address of the current address */
#define GET_ALIGN_NEXT_ADR(x) GET_ALIGEN_FORWARD_ADR(((uintptr_t)(x) + sizeof(uintptr_t) - 1u))
/*Gets shift bit for unaligned address*/
#if (CPU_32_WITH_16_ADR == TRUE)
#define GET_ALIGN_RIGNT_SHIFT(x) (SUFFIX_VLU(x) << 4u)
#define GET_ALIGN_LEFT_SHIFT(x)  ((sizeof(uintptr_t) - (SUFFIX_VLU(x))) << 4u)
#else
#define GET_ALIGN_RIGNT_SHIFT(x) (SUFFIX_VLU(x) << 3u)
#define GET_ALIGN_LEFT_SHIFT(x)  ((sizeof(uintptr_t) - (SUFFIX_VLU(x))) << 3u)
#endif
/* PRQA S 3472-- */ /* MISRA Dir-4.9 */
#endif              /* CPU_TYPE != CPU_TYPE_8 */

#if !defined(ILIB_MEM_USED_ASAN_SUPPRESSION)
/* Detection is turned on only for specific unit tests to avoid false positives */
#define ILIB_MEM_USED_ASAN_SUPPRESSION 1
#endif /* !defined ILIB_MEM_USED_ASAN_SUPPRESSION */

#if (defined(__clang__) || defined(__GNUC__)) && (ILIB_MEM_USED_ASAN_SUPPRESSION)
#define ATTRIBUTE_NO_SANITIZE_ADDRESS __attribute__((no_sanitize_address))
#else
#define ATTRIBUTE_NO_SANITIZE_ADDRESS
#endif /* defined(__clang__) || defined (__GNUC__)) && (ILIB_MEM_USED_ASAN_SUPPRESSION */
/*=================================================[internal data]====================================================*/

/*==========================================[external data declarations]==============================================*/

/*=========================================[internal function declarations]===========================================*/

/*==========================================[external function definitions]===========================================*/

#define ILIB_START_SEC_CODE
#include "iLib_MemMap.h"
/**
 * @ingroup ilib
 * @brief isoft memcpy
 * @param[in] __dst Points to the target array used to store the copied content
 * @param[in] __src Points to the data source you want to copy
 * @param[in] __len The number of bytes to be copied
 * @return This function returns a pointer to the target store __dst.
 */
/* PRQA S 0602++ */ /* MISRA Rule 1.3,MISRA Rule 21.2 */
/* PRQA S 0306++ */ /* MISRA Rule 11.4 */
ATTRIBUTE_NO_SANITIZE_ADDRESS
void* ILib_memcpy(void* ILIB_RESTRICT __dst, const void* ILIB_RESTRICT __src, uint32 __len)
/* PRQA S 0602-- */ /* MISRA Rule 1.3,MISRA Rule 21.2 */
{
    const uintptr_t* src = __src;
    uintptr_t* dst = __dst;
    const uint8* dst_last = (uint8*)__dst + __len; /* PRQA S 0488 */ /* MISRA Rule 18.4 Rule-8.13*/
#if (CPU_TYPE != CPU_TYPE_8)
    const uint8* dst_tmp;
    /* Handles parts of the destination address that are not aligned */
    if (!IS_ALIGEN_ADR(dst)) /* PRQA S 3472,3469 */ /* MISRA Dir-4.9 */
    {
        dst_tmp = (const uint8*)GET_ALIGN_NEXT_ADR(dst); /* PRQA S 3472,3469 */ /* MISRA Dir-4.9 */
        if ((uintptr_t)dst_tmp > (uintptr_t)dst_last)
        {
            goto end_memcpy; /* PRQA S 2001 */ /* MISRA Rule-15.1 */
        }
        while ((uintptr_t)dst < (uintptr_t)dst_tmp)
        {
            *(uint8*)dst = *(const uint8*)src;
            dst = (uintptr_t*)((uintptr_t)dst + 1u);
            src = (const uintptr_t*)((uintptr_t)src + 1u);
        }
    }
    /* Deal with the middle */
    if (IS_ALIGEN_ADR(src)) /* PRQA S 3472,3469 */ /* MISRA Dir-4.9 */
    {
        dst_tmp = (uint8*)GET_ALIGEN_128_FORWARD_ADR(dst_last); /* PRQA S 3472,3469 */ /* MISRA Dir-4.9 */
        while ((uintptr_t)dst < (uintptr_t)dst_tmp)
        {
            dst[0] = src[0];
            dst[1] = src[1];
            dst[2] = src[2];
            dst[3] = src[3];
            src += 4u; /* PRQA S 0488 */ /* MISRA Rule 18.4 */
            dst += 4u; /* PRQA S 0488 */ /* MISRA Rule 18.4 */
        }
        dst_tmp = (const uint8*)GET_ALIGEN_FORWARD_ADR(dst_last); /* PRQA S 3472,3469 */ /* MISRA Dir-4.9 */
        while ((uintptr_t)dst < (uintptr_t)dst_tmp)
        {
            *dst++ = *src++; /* PRQA S 3440,3387 */ /* MISRA Rule 13.3 */
        }
    }
    else
    {
        /* PRQA S 3472,3469++ */ /* MISRA Dir-4.9 */
        uintptr_t offset_vlu = SUFFIX_VLU(src);
        uintptr_t right_shift = GET_ALIGN_RIGNT_SHIFT(src);
        uintptr_t left_shift = GET_ALIGN_LEFT_SHIFT(src);
        dst_tmp = (const uint8*)GET_ALIGEN_128_FORWARD_ADR(dst_last);
        src = (const uintptr_t*)GET_ALIGEN_FORWARD_ADR(src);
        /* PRQA S 3472,3469-- */                                      /* MISRA Dir-4.9 */
        while ((uintptr_t)dst < (uintptr_t)dst_tmp) /* PRQA S 0310 */ /* MISRA Rule-11.3 */
        {
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
            dst[0] = (src[0] >> right_shift) | (src[1] << left_shift);
            dst[1] = (src[1] >> right_shift) | (src[2] << left_shift);
            dst[2] = (src[2] >> right_shift) | (src[3] << left_shift);
            dst[3] = (src[3] >> right_shift) | (src[4] << left_shift);
#else
            dst[0] = (src[0] << right_shift) | (src[1] >> left_shift);
            dst[1] = (src[1] << right_shift) | (src[2] >> left_shift);
            dst[2] = (src[2] << right_shift) | (src[3] >> left_shift);
            dst[3] = (src[3] << right_shift) | (src[4] >> left_shift);
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */

            src += 4u; /* PRQA S 0488 */ /* MISRA Rule 18.4 */
            dst += 4u; /* PRQA S 0488 */ /* MISRA Rule 18.4 */
        }
        dst_tmp = (const uint8*)GET_ALIGEN_FORWARD_ADR(dst_last); /* PRQA S 3472,3469 */ /* MISRA Dir-4.9 */
        for (; (uintptr_t)dst < (uintptr_t)dst_tmp; dst++)
        {
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
            *dst = (src[0] >> right_shift) | (src[1] << left_shift);
#else
            *dst = (src[0] << right_shift) | (src[1] >> left_shift);
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */
            src++;
        }
        src = (const uintptr_t*)((uintptr_t)src + offset_vlu);
    }
#endif /* CPU_TYPE != CPU_TYPE_8 */
end_memcpy:
    /* handle tail unaligned data */
    while ((uintptr_t)dst < (uintptr_t)dst_last)
    {
        *(uint8*)dst = *(const uint8*)src;
        dst = (uintptr_t*)((uintptr_t)dst + 1u);
        src = (const uintptr_t*)((uintptr_t)src + 1u);
    }

    return __dst;
}
/* PRQA S 0306-- */ /* MISRA Rule 11.4 */

/**
 * @ingroup ilib
 * @brief isoft memset
 * @param[in] __dst Points to the memory block to be filled
 * @param[in] __val The value to be set each byte
 * @param[in] __n The number of characters to be set to this value
 * @return This value returns a pointer to the store __dst.
 */
/* PRQA S 0602++ */ /* MISRA Rule 1.3,MISRA Rule 21.2 */
/* PRQA S 0306++ */ /* MISRA Rule 11.4 */
ATTRIBUTE_NO_SANITIZE_ADDRESS
void* ILib_memset(void* ILIB_RESTRICT __dst, int __val, uint32 __n)
/* PRQA S 0602-- */ /* MISRA Rule 1.3,MISRA Rule 21.2 */
{
    uintptr_t vlu = (uint8)__val;
    uintptr_t* dst = __dst;
    const uint8* dst_last = (uint8*)__dst + __n; /* PRQA S 0488 */ /* MISRA Rule 18.4 */
    const uint8* dst_tmp;
#if (CPU_TYPE != CPU_TYPE_8)
    if (__n >= sizeof(uintptr_t))
    {
        /* Data assembly */
        for (uintptr_t i = 0u; i < sizeof(uintptr_t); i++)
        {
#if (CPU_32_WITH_16_ADR == TRUE)
            vlu <<= 16u;
#else
            vlu <<= 8u;
#endif
            vlu |= (uintptr_t)__val;
        }
    }
    if (!IS_ALIGEN_ADR(dst)) /* PRQA S 3472,3469 */ /* MISRA Dir-4.9 */
    {
        /* start address unaligned processing */
        dst_tmp = (uint8*)GET_ALIGN_NEXT_ADR(dst); /* PRQA S 3472,3469 */ /* MISRA Dir-4.9 */
        if ((uintptr_t)dst_tmp > (uintptr_t)dst_last)
        {
            goto end_memset; /* PRQA S 2001 */ /* MISRA Rule-15.1 */
        }
        while ((uintptr_t)dst < (uintptr_t)dst_tmp)
        {
            *(uint8*)dst = (uint8)vlu;
            dst = (uintptr_t*)((uintptr_t)dst + 1u);
        }
    }
    /* Deal with the middle */
    dst_tmp = (uint8*)GET_ALIGEN_128_FORWARD_ADR(dst_last); /* PRQA S 3472,3469 */ /* MISRA Dir-4.9 */
    while ((uintptr_t)dst < (uintptr_t)dst_tmp)
    {
        dst[0] = vlu;
        dst[1] = vlu;
        dst[2] = vlu;
        dst[3] = vlu;
        dst += 4u; /* PRQA S 0488 */ /* MISRA Rule 18.4 */
    }
    dst_tmp = (uint8*)GET_ALIGEN_FORWARD_ADR(dst_last); /* PRQA S 3472,3469 */ /* MISRA Dir-4.9 */
    while ((uintptr_t)dst < (uintptr_t)dst_tmp)
    {
        *dst = vlu;
        dst++;
    }
#endif /* CPU_TYPE != CPU_TYPE_8 */
end_memset:
    /* handle tail unaligned data */
    while ((uintptr_t)dst < (uintptr_t)dst_last)
    {
        *(uint8*)dst = (uint8)vlu;
        dst = (uintptr_t*)((uintptr_t)dst + 1u);
    }
    return __dst;
}
/* PRQA S 0306-- */ /* MISRA Rule 11.4 */

/**
 * @ingroup ilib
 * @brief isoft memcmp
 * @param[in] __str1 Points to the array str1
 * @param[in] __str2 Points to the array str2
 * @param[in] __n The number of bytes to be compared
 * @return 0:__str1 is equal to __str2,1:other condition.
 */
/* PRQA S 0602++ */ /* MISRA Rule 1.3,MISRA Rule 21.2 */
/* PRQA S 0306++ */ /* MISRA Rule 11.4 */
ATTRIBUTE_NO_SANITIZE_ADDRESS
int ILib_memcmp(const void* ILIB_RESTRICT __str1, const void* ILIB_RESTRICT __str2, uint32 __n)
/* PRQA S 0602-- */ /* MISRA Rule 1.3,MISRA Rule 21.2 */
{
    const uintptr_t* src = __str2;
    const uintptr_t* dst = __str1;
    const uint8* dst_last = (const uint8*)__str1 + __n; /* PRQA S 0488 */ /* MISRA Rule 18.4 */
#if (CPU_TYPE != CPU_TYPE_8)
    const uint8* dst_tmp;
    if (!IS_ALIGEN_ADR(dst)) /* PRQA S 3469 */ /* MISRA Dir-4.9 */
    {
        dst_tmp = (const uint8*)GET_ALIGN_NEXT_ADR(dst); /* PRQA S 3472,3469 */ /* MISRA Dir-4.9 */
        if ((uintptr_t)dst_tmp > (uintptr_t)dst_last)
        {
            goto end_memcmp; /* PRQA S 2001 */ /* MISRA Rule-15.1 */
        }
        while ((uintptr_t)dst < (uintptr_t)dst_tmp)
        {
            if (*(const uint8*)dst != *(const uint8*)src)
            {
                return 1;
            }
            dst = (const uintptr_t*)((uintptr_t)dst + 1u);
            src = (const uintptr_t*)((uintptr_t)src + 1u);
        }
    }
    /* Deal with the middle */
    if (IS_ALIGEN_ADR(src)) /* PRQA S 3472,3469 */ /* MISRA Dir-4.9 */
    {
        dst_tmp = (const uint8*)GET_ALIGEN_128_FORWARD_ADR(dst_last); /* PRQA S 3472,3469 */ /* MISRA Dir-4.9 */
        while ((uintptr_t)dst < (uintptr_t)dst_tmp)
        {
            if ((dst[0] != src[0]) || (dst[1] != src[1]) || (dst[2] != src[2]) || (dst[3] != src[3]))
            {
                return 1;
            }
            src += 4u; /* PRQA S 0488 */ /* MISRA Rule 18.4 */
            dst += 4u; /* PRQA S 0488 */ /* MISRA Rule 18.4 */
        }
        dst_tmp = (const uint8*)GET_ALIGEN_FORWARD_ADR(dst_last); /* PRQA S 3472,3469 */ /* MISRA Dir-4.9 */
        while ((uintptr_t)dst < (uintptr_t)dst_tmp)
        {
            if (*(dst++) != *(src++)) /* PRQA S 3440,3387 */ /* MISRA Rule 13.3 */
            {
                return 1;
            }
        }
    }
    else
    {
        /* PRQA S 3472,3469++ */ /* MISRA Dir-4.9 */
        uintptr_t offset_vlu = SUFFIX_VLU(src);
        uintptr_t right_shift = GET_ALIGN_RIGNT_SHIFT(src);
        uintptr_t left_shift = GET_ALIGN_LEFT_SHIFT(src);
        dst_tmp = (const uint8*)GET_ALIGEN_128_FORWARD_ADR(dst_last);
        src = (const uintptr_t*)GET_ALIGEN_FORWARD_ADR(src);
        /* PRQA S 3472,3469-- */ /* MISRA Dir-4.9 */
        while ((uintptr_t)dst < (uintptr_t)dst_tmp)
        {
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
            if ((dst[0] != ((src[0] >> right_shift) | (src[1] << left_shift)))
                || (dst[1] != ((src[1] >> right_shift) | (src[2] << left_shift)))
                || (dst[2] != ((src[2] >> right_shift) | (src[3] << left_shift)))
                || (dst[3] != ((src[3] >> right_shift) | (src[4] << left_shift))))
#else
            if ((dst[0] != ((src[0] << right_shift) | (src[1] >> left_shift)))
                || (dst[1] != ((src[1] << right_shift) | (src[2] >> left_shift)))
                || (dst[2] != ((src[2] << right_shift) | (src[3] >> left_shift)))
                || (dst[3] != ((src[3] << right_shift) | (src[4] >> left_shift))))
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */
            {
                return 1;
            }
            src += 4u; /* PRQA S 0488 */ /* MISRA Rule 18.4 */
            dst += 4u; /* PRQA S 0488 */ /* MISRA Rule 18.4 */
        }
        dst_tmp = (const uint8*)GET_ALIGEN_FORWARD_ADR(dst_last); /* PRQA S 3472,3469 */ /* MISRA Dir-4.9 */
        for (; (uintptr_t)dst < (uintptr_t)dst_tmp; dst++)
        {
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
            if (*dst != ((src[0] >> right_shift) | (src[1] << left_shift)))
#else
            if (*dst != ((src[0] << right_shift) | (src[1] >> left_shift)))
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */
            {
                return 1;
            }
            src++;
        }
        src = (const uintptr_t*)((uintptr_t)src + offset_vlu);
    }
#endif /* CPU_TYPE != CPU_TYPE_8 */
end_memcmp:
    /* handle tail unaligned data */
    while ((uintptr_t)dst < (uintptr_t)dst_last)
    {
        if (*(const uint8*)dst != *(const uint8*)src)
        {
            return 1;
        }
        dst = (uintptr_t*)((uintptr_t)dst + 1u);
        src = (const uintptr_t*)((uintptr_t)src + 1u);
    }

    return 0;
}
/* PRQA S 0306-- */ /* MISRA Rule 11.4 */
#define ILIB_STOP_SEC_CODE
#include "iLib_MemMap.h"
#endif
