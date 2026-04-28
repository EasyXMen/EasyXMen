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
#if (CPU_TYPE == CPU_TYPE_64)
/* pc environment 64bit */
#define SUFFIX_BIT_LEN   3u
#define IS_ALIGEN_ADR(x) (((uintptr_t)(x) << 61u) == 0u)
#define SUFFIX_VLU(x)    (((uintptr_t)(x) << 61u) >> 61u)

#elif (CPU_TYPE == CPU_TYPE_32)
/* normal 32-bit mcu */
/* PRQA S 3472 ++ */ /* VL_IStdLib_MacroFunc */
#if (CPU_32_WITH_16_ADR == TRUE)
#define SUFFIX_BIT_LEN   1u
#define IS_ALIGEN_ADR(x) (((uintptr_t)(x) << 31u) == 0u)
#define SUFFIX_VLU(x)    (((uintptr_t)(x) << 31u) >> 31u)
#else
#define SUFFIX_BIT_LEN   2u
#define IS_ALIGEN_ADR(x) (((uintptr_t)(x) << 30u) == 0u)
#define SUFFIX_VLU(x)    (((uintptr_t)(x) << 30u) >> 30u)
#endif

#elif (CPU_TYPE == CPU_TYPE_16)
/*  16-bit mcu */
#define SUFFIX_BIT_LEN   1u
#define IS_ALIGEN_ADR(x) (((uintptr_t)(x) << 15u) == 0u)
#define SUFFIX_VLU(x)    ((((uintptr_t)x) << 15u) >> 15u)
#else
/* default 32-bit mcu */
#define SUFFIX_BIT_LEN   2u
#define IS_ALIGEN_ADR(x) (((uintptr_t)(x) << 30u) == 0u)
#define SUFFIX_VLU(x)    (((uintptr_t)(x) << 30u) >> 30u)
#endif /* SIZE_MAX */
#if (CPU_TYPE != CPU_TYPE_8)

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

#endif /* CPU_TYPE != CPU_TYPE_8 */
/* PRQA S 3472 -- */

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
/**
 * @brief        handle tail unaligned data of memcmp
 * @param[in]    dst     : Points to the array str1
 * @param[in]    src     : Points to the array str2
 * @param[in]    dstLast : Pointer to the end of dst.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         -
 */
static uint8 ILib_endMemcmp(const void* dst, const void* src, const void* dstLast);
/**
 * @brief        handle tail unaligned data of memcpy
 * @param[inout] dst     : Points to the array str1
 * @param[in]    src     : Points to the array str2
 * @param[in]    dstLast : Pointer to the end of dst.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         -
 */
static void ILib_endMemcpy(void* dst, const void* src, const void* dstLast);
/**
 * @brief        handle tail unaligned data of memcpy
 * @param[inout] dst     : Points to the array str1
 * @param[in]    val     : Fill the value of memory
 * @param[in]    dstLast : Pointer to the end of dst.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         -
 */
static void ILib_endMemset(void* dst, uintptr_t vlu, const void* dstLast);
/*==========================================[external function definitions]===========================================*/

#define ILIB_START_SEC_CODE
#include "iLib_MemMap.h"
/**
 * @ingroup ilib
 * @brief isoft memcpy
 * @param[in] dstptr Points to the target array used to store the copied content
 * @param[in] srcptr Points to the data source you want to copy
 * @param[in] length The number of bytes to be copied
 * @return This function returns a pointer to the target store dstptr.
 */

/* PRQA S 0306,2889++ */ /* VL_IStdLib_0306,VL_QAC_MultiReturn */
ATTRIBUTE_NO_SANITIZE_ADDRESS
void* ILib_memcpy(void* ILIB_RESTRICT dstptr, const void* ILIB_RESTRICT srcptr, uint32 length)
/*PRQA S 2889 -- */
{
    const uintptr_t* src      = srcptr; /* PRQA S 0317 */                    /* VL_QAC_0317 */
    uintptr_t*       dst      = dstptr; /* PRQA S 0317 */                    /* VL_QAC_0317 */
    const uint8*     dst_last = &((uint8*)dstptr)[length]; /* PRQA S 0316 */ /* VL_QAC_0316 */
#if (CPU_TYPE != CPU_TYPE_8)
    const uint8* dst_tmp;
    /* Handles parts of the destination address that are not aligned */
    if (!IS_ALIGEN_ADR(dst))
    {
        dst_tmp = (const uint8*)GET_ALIGN_NEXT_ADR(dst);
        if ((uintptr_t)dst_tmp > (uintptr_t)dst_last)
        {
            ILib_endMemcpy(dst, src, dst_last);
            return dstptr;
        }
        while ((uintptr_t)dst < (uintptr_t)dst_tmp)
        {
            *(uint8*)dst = *(const uint8*)src;
            dst          = (uintptr_t*)((uintptr_t)dst + 1u);
            src          = (const uintptr_t*)((uintptr_t)src + 1u);
        }
    }
    /* Deal with the middle */
    if (IS_ALIGEN_ADR(src))
    {
        dst_tmp = (uint8*)GET_ALIGEN_128_FORWARD_ADR(dst_last);
        while ((uintptr_t)dst < (uintptr_t)dst_tmp)
        {
            dst[0] = src[0];
            dst[1] = src[1];
            dst[2] = src[2];
            dst[3] = src[3];
            src    = &src[4uL];
            dst    = &dst[4uL];
        }
        dst_tmp = (const uint8*)GET_ALIGEN_FORWARD_ADR(dst_last);
        while ((uintptr_t)dst < (uintptr_t)dst_tmp)
        {
            *dst = *src;
            dst++;
            src++;
        }
    }
    else
    {
        uintptr_t offset_vlu  = SUFFIX_VLU(src);
        uintptr_t right_shift = GET_ALIGN_RIGNT_SHIFT(src);
        uintptr_t left_shift  = GET_ALIGN_LEFT_SHIFT(src);
        dst_tmp               = (const uint8*)GET_ALIGEN_128_FORWARD_ADR(dst_last);
        src                   = (const uintptr_t*)GET_ALIGEN_FORWARD_ADR(src);

        while ((uintptr_t)dst < (uintptr_t)dst_tmp)
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

            src = &src[4uL];
            dst = &dst[4uL];
        }
        dst_tmp = (const uint8*)GET_ALIGEN_FORWARD_ADR(dst_last);
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
    /* handle tail unaligned data */
    ILib_endMemcpy(dst, src, dst_last);

    return dstptr;
}
/* PRQA S 0306-- */

/**
 * @ingroup ilib
 * @brief isoft memset
 * @param[in] dstptr Points to the memory block to be filled
 * @param[in] val The value to be set each byte
 * @param[in] n The number of characters to be set to this value
 * @return This value returns a pointer to the store dstptr.
 */

/* PRQA S 0306,2889++ */ /* VL_IStdLib_0306,VL_QAC_MultiReturn */
ATTRIBUTE_NO_SANITIZE_ADDRESS
void* ILib_memset(void* ILIB_RESTRICT dstptr, uint8 val, uint32 n)
/* PRQA S 2889 -- */
{
    uintptr_t    vlu      = val;
    uintptr_t*   dst      = dstptr; /* PRQA S 0317 */               /* VL_QAC_0317 */
    const uint8* dst_last = &((uint8*)dstptr)[n]; /* PRQA S 0316 */ /* VL_QAC_0316 */
    const uint8* dst_tmp;
#if (CPU_TYPE != CPU_TYPE_8)
    if (n >= sizeof(uintptr_t))
    {
        /* Data assembly */
        for (uintptr_t i = 0u; i < sizeof(uintptr_t); i++)
        {
#if (CPU_32_WITH_16_ADR == TRUE)
            vlu <<= 16u;
#else
            vlu <<= 8u;
#endif
            vlu |= (uintptr_t)val;
        }
    }
    if (!IS_ALIGEN_ADR(dst))
    {
        /* start address unaligned processing */
        dst_tmp = (uint8*)GET_ALIGN_NEXT_ADR(dst);
        if ((uintptr_t)dst_tmp > (uintptr_t)dst_last)
        {
            ILib_endMemset(dst, vlu, dst_last);
            return dstptr;
        }
        while ((uintptr_t)dst < (uintptr_t)dst_tmp)
        {
            *(uint8*)dst = (uint8)vlu;
            dst          = (uintptr_t*)((uintptr_t)dst + 1u);
        }
    }
    /* Deal with the middle */
    dst_tmp = (uint8*)GET_ALIGEN_128_FORWARD_ADR(dst_last);
    while ((uintptr_t)dst < (uintptr_t)dst_tmp)
    {
        dst[0] = vlu;
        dst[1] = vlu;
        dst[2] = vlu;
        dst[3] = vlu;
        dst    = &dst[4uL];
    }
    dst_tmp = (uint8*)GET_ALIGEN_FORWARD_ADR(dst_last);
    while ((uintptr_t)dst < (uintptr_t)dst_tmp)
    {
        *dst = vlu;
        dst++;
    }
#endif /* CPU_TYPE != CPU_TYPE_8 */
    /* handle tail unaligned data */
    ILib_endMemset(dst, vlu, dst_last);
    return dstptr;
}
/* PRQA S 0306-- */

/**
 * @ingroup ilib
 * @brief isoft memcmp
 * @param[in] str1ptr Points to the array str1
 * @param[in] str2ptr Points to the array str2
 * @param[in] n The number of bytes to be compared
 * @return 0:str1ptr is equal to str2ptr,1:other condition.
 */
/* PRQA S 0306,2889++ */ /* VL_IStdLib_0306,VL_QAC_MultiReturn */
ATTRIBUTE_NO_SANITIZE_ADDRESS
uint8 ILib_memcmp(const void* ILIB_RESTRICT str1ptr, const void* ILIB_RESTRICT str2ptr, uint32 n)
/*PRQA S 2889 -- */
{
    const uintptr_t* src      = str2ptr; /* PRQA S 0317 */                     /* VL_QAC_0317 */
    const uintptr_t* dst      = str1ptr; /* PRQA S 0317 */                     /* VL_QAC_0317 */
    const uint8*     dst_last = &((const uint8*)str1ptr)[n]; /* PRQA S 0316 */ /* VL_QAC_0316 */
#if (CPU_TYPE != CPU_TYPE_8)
    const uint8* dst_tmp;
    if (!IS_ALIGEN_ADR(dst))
    {
        dst_tmp = (const uint8*)GET_ALIGN_NEXT_ADR(dst);
        if ((uintptr_t)dst_tmp > (uintptr_t)dst_last)
        {
            return ILib_endMemcmp(dst, src, dst_last);
        }
        while ((uintptr_t)dst < (uintptr_t)dst_tmp)
        {
            if (*(const uint8*)dst != *(const uint8*)src)
            {
                return 1u;
            }
            dst = (const uintptr_t*)((uintptr_t)dst + 1u);
            src = (const uintptr_t*)((uintptr_t)src + 1u);
        }
    }
    /* Deal with the middle */
    if (IS_ALIGEN_ADR(src))
    {
        dst_tmp = (const uint8*)GET_ALIGEN_128_FORWARD_ADR(dst_last);
        while ((uintptr_t)dst < (uintptr_t)dst_tmp)
        {
            if ((dst[0] != src[0]) || (dst[1] != src[1]) || (dst[2] != src[2]) || (dst[3] != src[3]))
            {
                return 1u;
            }
            src = &src[4uL];
            dst = &dst[4uL];
        }
        dst_tmp = (const uint8*)GET_ALIGEN_FORWARD_ADR(dst_last);
        while ((uintptr_t)dst < (uintptr_t)dst_tmp)
        {
            if (*(dst) != *(src))
            {
                return 1u;
            }
            dst++;
            src++;
        }
    }
    else
    {
        uintptr_t offset_vlu  = SUFFIX_VLU(src);
        uintptr_t right_shift = GET_ALIGN_RIGNT_SHIFT(src);
        uintptr_t left_shift  = GET_ALIGN_LEFT_SHIFT(src);
        dst_tmp               = (const uint8*)GET_ALIGEN_128_FORWARD_ADR(dst_last);
        src                   = (const uintptr_t*)GET_ALIGEN_FORWARD_ADR(src);

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
                return 1u;
            }
            src = &src[4uL];
            dst = &dst[4uL];
        }
        dst_tmp = (const uint8*)GET_ALIGEN_FORWARD_ADR(dst_last);
        for (; (uintptr_t)dst < (uintptr_t)dst_tmp; dst++)
        {
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
            if (*dst != ((src[0] >> right_shift) | (src[1] << left_shift)))
#else
            if (*dst != ((src[0] << right_shift) | (src[1] >> left_shift)))
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */
            {
                return 1u;
            }
            src++;
        }
        src = (const uintptr_t*)((uintptr_t)src + offset_vlu);
    }
#endif /* CPU_TYPE != CPU_TYPE_8 */
    /* handle tail unaligned data */
    return ILib_endMemcmp(dst, src, dst_last);
}
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
static uint8 ILib_endMemcmp(const void* dst, const void* src, const void* dstLast)
{
    uint8            Ret      = 0u;
    const uintptr_t* pDst     = (const uintptr_t*)dst; /* PRQA S 0316 */ /* VL_QAC_0316 */
    const uintptr_t* pSrc     = (const uintptr_t*)src; /* PRQA S 0316 */ /* VL_QAC_0316 */
    const uint8*     pDstLast = (const uint8*)dstLast; /* PRQA S 0316 */ /* VL_QAC_0316 */

    while ((uintptr_t)pDst < (uintptr_t)pDstLast)
    {
        if (*(const uint8*)pDst != *(const uint8*)pSrc)
        {
            Ret = 1u;
            break;
        }
        pDst = (uintptr_t*)((uintptr_t)pDst + (uintptr_t)1uLL);
        pSrc = (const uintptr_t*)((uintptr_t)pSrc + 1); /* PRQA S 1840 */ /* VL_IStdLib_1840 */
    }

    return Ret;
}

static void ILib_endMemcpy(void* dst, const void* src, const void* dstLast)
{
    uintptr_t*       pDst     = (uintptr_t*)dst; /* PRQA S 0316 */       /* VL_QAC_0316 */
    const uintptr_t* pSrc     = (const uintptr_t*)src; /* PRQA S 0316 */ /* VL_QAC_0316 */
    const uint8*     pDstLast = (const uint8*)dstLast; /* PRQA S 0316 */ /* VL_QAC_0316 */

    while ((uintptr_t)pDst < (uintptr_t)pDstLast)
    {
        *(uint8*)pDst = *(const uint8*)pSrc; /* PRQA S 0316 */ /* VL_QAC_0316 */
        pDst          = (uintptr_t*)((uintptr_t)pDst + 1uL);
        pSrc          = (const uintptr_t*)((uintptr_t)pSrc + 1uL);
    }
}

static void ILib_endMemset(void* dst, uintptr_t vlu, const void* dstLast)
{
    uintptr_t*   pDst     = (uintptr_t*)dst; /* PRQA S 0316 */       /* VL_QAC_0316 */
    const uint8* pDstLast = (const uint8*)dstLast; /* PRQA S 0316 */ /* VL_QAC_0316 */

    while ((uintptr_t)pDst < (uintptr_t)pDstLast)
    {
        *(uint8*)pDst = (uint8)vlu;
        pDst          = (uintptr_t*)((uintptr_t)pDst + 1uL);
    }
}
/* PRQA S 0306-- */
#define ILIB_STOP_SEC_CODE
#include "iLib_MemMap.h"
#endif
