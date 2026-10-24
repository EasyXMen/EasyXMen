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
 **  @file               : IStdLib_Mem.c
 **  @author             : darren.zhang,QinmeiChen
 **  @date               : 2024/09/09
 **  @vendor             : isoft
 **  @description        : standard lib source code
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */

#include <stdint.h>
#include "IStdLib.h"

/* ================================================== version check ================================================= */
/* ===================================================== macros ===================================================== */
#if ISTDLIB_BSW_NOT_USED_STDLIB == STD_ON
/* platform used value */
#if (CPU_TYPE == CPU_TYPE_64)
/* pc environment 64bit */
#define ISTDLIB_SUFFIX_BIT_LEN  3u
#define ISTDLIB_IS_ALIGN_ADR(x) (((uintptr_t)(x) << 61uL) == 0uL)
#define ISTDLIB_SUFFIX_VLU(x)   (((uintptr_t)(x) << 61uL) >> 61uL)
/* Explicit uintptr_t max for this CPU_TYPE (64-bit): avoids relying on
 * <stdint.h>'s UINTPTR_MAX under ISTDLIB_BSW_NOT_USED_STDLIB mode. */
#define ISTDLIB_UINTPTR_MAX     0xFFFFFFFFFFFFFFFFuL

#elif (CPU_TYPE == CPU_TYPE_32)
/* normal 32-bit mcu */
/* PRQA S 0750,3472 ++ */ /* VL_IStdLib_MacroFunc */
#if (CPU_32_WITH_16_ADR == TRUE)
#define ISTDLIB_SUFFIX_BIT_LEN  1u
#define ISTDLIB_IS_ALIGN_ADR(x) (((uintptr_t)(x) << 31uL) == 0uL)
#define ISTDLIB_SUFFIX_VLU(x)   (((uintptr_t)(x) << 31uL) >> 31uL)
/* 32-bit CPU with 16-bit address bus: uintptr_t is still 32-bit wide
 * (the shift by 31 requires >=32 bits), so the max is 2^32 - 1. */
#define ISTDLIB_UINTPTR_MAX     0xFFFFFFFFuL
#else
#define ISTDLIB_SUFFIX_BIT_LEN  2u
#define ISTDLIB_IS_ALIGN_ADR(x) (((uintptr_t)(x) << 30uL) == 0uL)
#define ISTDLIB_SUFFIX_VLU(x)   (((uintptr_t)(x) << 30uL) >> 30uL)
/* Explicit uintptr_t max for this CPU_TYPE (32-bit): 2^32 - 1. */
#define ISTDLIB_UINTPTR_MAX     0xFFFFFFFFuL
#endif

#elif (CPU_TYPE == CPU_TYPE_16)
/*  16-bit mcu */
#define ISTDLIB_SUFFIX_BIT_LEN  1u
#define ISTDLIB_IS_ALIGN_ADR(x) (((uintptr_t)(x) << 15uL) == 0uL)
#define ISTDLIB_SUFFIX_VLU(x)   ((((uintptr_t)x) << 15uL) >> 15uL)
/* Explicit uintptr_t max for this CPU_TYPE (16-bit): 2^16 - 1. */
#define ISTDLIB_UINTPTR_MAX     0xFFFFuL
#else
/* default 32-bit mcu */
#define ISTDLIB_SUFFIX_BIT_LEN  2u
#define ISTDLIB_IS_ALIGN_ADR(x) (((uintptr_t)(x) << 30uL) == 0uL)
#define ISTDLIB_SUFFIX_VLU(x)   (((uintptr_t)(x) << 30uL) >> 30uL)
/* Explicit uintptr_t max for this CPU_TYPE (default 32-bit): 2^32 - 1. */
#define ISTDLIB_UINTPTR_MAX     0xFFFFFFFFuL
#endif /* SIZE_MAX */
#if (CPU_TYPE != CPU_TYPE_8)
/* Gets the forward-aligned address of the current address */
#define ISTDLIB_GET_ALIGN_FORWARD_ADR(x) (((uintptr_t)(x) >> ISTDLIB_SUFFIX_BIT_LEN) << ISTDLIB_SUFFIX_BIT_LEN)
/* 4-uptr block size = 4 * sizeof(uintptr_t) bytes (loop unroll factor) */
#define ISTDLIB_4UPTR_BLOCK_SIZE         (sizeof(uintptr_t) << 2uL)
/* 4-uptr loop read reach = 5 * sizeof(uintptr_t) bytes (4-uptr block + 1-uptr read-ahead).
 * Bounds the unaligned 4-uptr loop so src[0..4] never passes source_end. */
#define ISTDLIB_5UPTR_REACH_SIZE         (ISTDLIB_4UPTR_BLOCK_SIZE + sizeof(uintptr_t))
/* Gets the 4-uptr-boundary forward-aligned address of the current address */
#define ISTDLIB_GET_ALIGN_4UPTR_FORWARD_ADR(x) \
    (((uintptr_t)(x) > ISTDLIB_4UPTR_BLOCK_SIZE) ? ((uintptr_t)(x) - ISTDLIB_4UPTR_BLOCK_SIZE) : 0uL)
/* 4-uptr block-loop safe boundary for unaligned src: the loop reads src[0..4]
 * (5*W reach) per iteration; src tracks dst 1:1, so source_end maps to
 * (dstLast + offsetVlu). Pull the dst boundary back by ISTDLIB_5UPTR_REACH_SIZE
 * so src + 5*W <= source_end on every iteration. When the buffer is too small
 * (dstAtSrcEnd <= 5*W), return fallback so the block loop is skipped. Ternary
 * lowers to a cmov; the compiler CSEs (dstLast + offsetVlu) to one add. */
#define ISTDLIB_GET_4UPTR_LOOP_SAFE_ADR(dstLast, offsetVlu, fallback)                      \
    ((((uintptr_t)(dstLast) + (offsetVlu)) > ISTDLIB_5UPTR_REACH_SIZE)                     \
         ? (const uint8*)(((uintptr_t)(dstLast) + (offsetVlu)) - ISTDLIB_5UPTR_REACH_SIZE) \
         : (const uint8*)(fallback))
/* Tail word-loop safe boundary: pulls the aligned boundary back by one word
 * when (offsetVlu + misalign) < W, so the tail loop's 2-word read-ahead
 * stays within source_end. Wraparound-safe: only pulls back when
 * aligned >= W to avoid unsigned underflow. (See definition below.) */
/* Gets the backward aligned address of the current address */
#define GET_ALIGN_NEXT_ADR(x) ISTDLIB_GET_ALIGN_FORWARD_ADR(((uintptr_t)(x) + sizeof(uintptr_t) - 1uL))
/*Gets shift bit for unaligned address*/
#if (CPU_32_WITH_16_ADR == TRUE)
#define ISTDLIB_GET_ALIGN_RIGHT_SHIFT(x) (ISTDLIB_SUFFIX_VLU(x) << 4uL)
#define ISTDLIB_GET_ALIGN_LEFT_SHIFT(x)  ((sizeof(uintptr_t) - (ISTDLIB_SUFFIX_VLU(x))) << 4uL)
#else
#define ISTDLIB_GET_ALIGN_RIGHT_SHIFT(x) (ISTDLIB_SUFFIX_VLU(x) << 3uL)
#define ISTDLIB_GET_ALIGN_LEFT_SHIFT(x)  ((sizeof(uintptr_t) - (ISTDLIB_SUFFIX_VLU(x))) << 3uL)
#endif
#endif
/* PRQA S 0750,3472 -- */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
#define ISTDLIB_START_SEC_CODE
#include "IStdLib_MemMap.h"
/**
 * @brief        handle tail unaligned data of memcmp
 * @param[in]    dst     : Points to the array str1
 * @param[in]    src     : Points to the array str2
 * @param[in]    dstLast : Pointer to the end of dst.
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         -
 */
ISTDLIB_LOCAL StatusType IStdLib_endMemcmp(const void* dst, const void* src, const void* dstLast);
#define ISTDLIB_STOP_SEC_CODE
#include "IStdLib_MemMap.h"

#define ISTDLIB_START_SEC_CODE
#include "IStdLib_MemMap.h"
/**
 * @brief        handle tail unaligned data of memcpy
 * @param[inout] dst     : Points to the array str1
 * @param[in]    src     : Points to the array str2
 * @param[in]    dstLast : Pointer to the end of dst.
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         -
 */
ISTDLIB_LOCAL void IStdLib_endMemcpy(void* dst, const void* src, const void* dstLast);
#define ISTDLIB_STOP_SEC_CODE
#include "IStdLib_MemMap.h"

#define ISTDLIB_START_SEC_CODE
#include "IStdLib_MemMap.h"
/**
 * @brief        handle tail unaligned data of memcpy
 * @param[inout] dst     : Points to the array str1
 * @param[in]    val     : Fill the value of memory
 * @param[in]    dstLast : Pointer to the end of dst.
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         -
 */
ISTDLIB_LOCAL void IStdLib_endMemset(void* dst, uintptr_t vlu, const void* dstLast);
#define ISTDLIB_STOP_SEC_CODE
#include "IStdLib_MemMap.h"

#define ISTDLIB_START_SEC_CODE
#include "IStdLib_MemMap.h"
/**
 * @brief     Helper function to handle unaligned start of memCmp
 * @param[in] dst     : Points to the array str1
 * @param[in] src     : Points to the array str2
 * @param[in] dstLast : Pointer to the end of dst.
 * @return    int
 * @retval    0 : str1ptr is equal to str2ptr
 * @retval    1 : other condition.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       -
 */
ISTDLIB_LOCAL StatusType
    IStdLib_handleUnalignedStart(const uintptr_t** dst, const uintptr_t** src, const uint8* dstLast);
#define ISTDLIB_STOP_SEC_CODE
#include "IStdLib_MemMap.h"

#define ISTDLIB_START_SEC_CODE
#include "IStdLib_MemMap.h"
/**
 * @brief     Helper function to handle aligned middle of memCmp
 * @param[in] dst     : Points to the array str1
 * @param[in] src     : Points to the array str2
 * @param[in] dstLast : Pointer to the end of dst.
 * @return    int
 * @retval    0 : str1ptr is equal to str2ptr
 * @retval    1 : other condition.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       -
 */
ISTDLIB_LOCAL StatusType
    IStdLib_handleAlignedMiddle(const uintptr_t** dst, const uintptr_t** src, const uint8* dstLast);
#define ISTDLIB_STOP_SEC_CODE
#include "IStdLib_MemMap.h"

#define ISTDLIB_START_SEC_CODE
#include "IStdLib_MemMap.h"
/**
 * @brief     Helper function to handle unaligned middle of memCmp
 * @param[in] dst     : Points to the array str1
 * @param[in] src     : Points to the array str2
 * @param[in] dstLast : Pointer to the end of dst.
 * @return    int
 * @retval    0 : str1ptr is equal to str2ptr
 * @retval    1 : other condition.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       -
 */
ISTDLIB_LOCAL StatusType
    IStdLib_handleUnalignedMiddle(const uintptr_t** dst, const uintptr_t** src, const uint8* dstLast);
#define ISTDLIB_STOP_SEC_CODE
#include "IStdLib_MemMap.h"

/* ============================================ inline function definitions ========================================= */
/* Tail word-loop safe boundary: pulls the aligned boundary back by one word
 * when (offsetVlu + misalign) < W, so the tail loop's 2-word read-ahead
 * stays within source_end. Wraparound-safe: only pulls back when
 * aligned >= W to avoid unsigned underflow. */
ISTDLIB_LOCAL_INLINE const uint8* IStdLib_GetTailSafeAdr(uintptr_t dstLast, uintptr_t offsetVlu)
{
    uintptr_t aligned = ISTDLIB_GET_ALIGN_FORWARD_ADR(dstLast);
    /* Pull back one word when tail read-ahead would exceed source_end.
     * Guard aligned >= W to prevent unsigned underflow on the subtraction. */
    if (((offsetVlu + ISTDLIB_SUFFIX_VLU(dstLast)) < sizeof(uintptr_t)) && (aligned >= sizeof(uintptr_t)))
    {
        aligned -= sizeof(uintptr_t);
    }
    /*PRQA S 0306 ++ */ /*VL_IStdLib_0306*/
    return (const uint8*)aligned;
    /*PRQA S 0306 -- */
}
/* ============================================ internal data definitions =========================================== */
/* ============================================ external data definitions =========================================== */
/* ========================================== external function definitions ========================================= */
#define ISTDLIB_START_SEC_CODE
#include "IStdLib_MemMap.h"
/*PRQA S 0306 ++ */ /*VL_IStdLib_0306*/ /*PRQA S 2889,6060 ++ */ /*VL_QAC_MultiReturn,VL_MTR_IStdLib_STM19*/
void* IStdLib_MemCpy(void* ISTDLIB_RESTRICT dstptr, const void* ISTDLIB_RESTRICT srcptr, uint32 length)
/*PRQA S 2889,6060 -- */
{
    const uintptr_t* src     = srcptr; /* PRQA S 0317 */                    /* VL_QAC_0317 */
    uintptr_t*       dst     = dstptr; /* PRQA S 0317 */                    /* VL_QAC_0317 */
    const uint8*     dstLast = &((uint8*)dstptr)[length]; /* PRQA S 0316 */ /* VL_QAC_0316 */
#if (CPU_TYPE != CPU_TYPE_8)
    const uint8* dstTmp;
    /* Handles parts of the destination address that are not aligned */
    if (!ISTDLIB_IS_ALIGN_ADR(dst))
    {
        dstTmp = (const uint8*)GET_ALIGN_NEXT_ADR(dst);
        if ((uintptr_t)dstTmp > (uintptr_t)dstLast)
        {
            IStdLib_endMemcpy(dst, src, dstLast);
            return dstptr;
        }
        while ((uintptr_t)dst < (uintptr_t)dstTmp)
        {
            *(uint8*)dst = *(const uint8*)src;
            dst          = (uintptr_t*)((uintptr_t)dst + 1uL);
            src          = (const uintptr_t*)((uintptr_t)src + 1uL);
        }
    }
    /* Deal with the middle */
    if (ISTDLIB_IS_ALIGN_ADR(src))
    {
        dstTmp = (uint8*)ISTDLIB_GET_ALIGN_4UPTR_FORWARD_ADR(dstLast);
        while ((uintptr_t)dst < (uintptr_t)dstTmp)
        {
            dst[0] = src[0];
            dst[1] = src[1];
            dst[2] = src[2]; /* PRQA S 3120 */    /* VL_QAC_MagicNum */
            dst[3] = src[3]; /* PRQA S 3120 */    /* VL_QAC_MagicNum */
            src    = &src[4uL]; /* PRQA S 3120 */ /* VL_QAC_MagicNum */
            dst    = &dst[4uL]; /* PRQA S 3120 */ /* VL_QAC_MagicNum */
        }
        dstTmp = (const uint8*)ISTDLIB_GET_ALIGN_FORWARD_ADR(dstLast);
        while ((uintptr_t)dst < (uintptr_t)dstTmp)
        {
            *dst = *src;
            dst++;
            src++;
        }
    }
    else
    {
        uintptr_t offsetVlu  = ISTDLIB_SUFFIX_VLU(src);
        uintptr_t rightShift = ISTDLIB_GET_ALIGN_RIGHT_SHIFT(src);
        uintptr_t leftShift  = ISTDLIB_GET_ALIGN_LEFT_SHIFT(src);
        /* Bound the 4-uptr block loop so src + 5*W <= source_end on every iteration. */
        dstTmp = ISTDLIB_GET_4UPTR_LOOP_SAFE_ADR(dstLast, offsetVlu, dst);
        src    = (const uintptr_t*)ISTDLIB_GET_ALIGN_FORWARD_ADR(src);
        while ((uintptr_t)dst < (uintptr_t)dstTmp)
        {
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
            dst[0] = (src[0] >> rightShift) | (src[1] << leftShift);
            dst[1] = (src[1] >> rightShift) | (src[2] << leftShift); /* PRQA S 3120 */ /* VL_QAC_MagicNum */
            dst[2] = (src[2] >> rightShift) | (src[3] << leftShift); /* PRQA S 3120 */ /* VL_QAC_MagicNum */
            dst[3] = (src[3] >> rightShift) | (src[4] << leftShift); /* PRQA S 3120 */ /* VL_QAC_MagicNum */
#else
            dst[0] = (src[0] << rightShift) | (src[1] >> leftShift);
            dst[1] = (src[1] << rightShift) | (src[2] >> leftShift); /* PRQA S 3120 */ /* VL_QAC_MagicNum */
            dst[2] = (src[2] << rightShift) | (src[3] >> leftShift); /* PRQA S 3120 */ /* VL_QAC_MagicNum */
            dst[3] = (src[3] << rightShift) | (src[4] >> leftShift); /* PRQA S 3120 */ /* VL_QAC_MagicNum */
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */

            src = &src[4uL]; /* PRQA S 3120 */ /* VL_QAC_MagicNum */
            dst = &dst[4uL]; /* PRQA S 3120 */ /* VL_QAC_MagicNum */
        }
        /* Tail word loop reads src[1] (2*W read-ahead); shrink the boundary by one
         * word when unsafe so endMemcpy's byte-by-byte handler covers the tail. */
        dstTmp = IStdLib_GetTailSafeAdr((uintptr_t)dstLast, offsetVlu);
        for (; (uintptr_t)dst < (uintptr_t)dstTmp; dst++)
        {
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
            *dst = (src[0] >> rightShift) | (src[1] << leftShift);
#else
            *dst = (src[0] << rightShift) | (src[1] >> leftShift);
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */
            src++;
        }
        src = (const uintptr_t*)((uintptr_t)src + offsetVlu);
    }
#endif /* CPU_TYPE != CPU_TYPE_8 */

    IStdLib_endMemcpy(dst, src, dstLast);
    return dstptr;
}
#define ISTDLIB_STOP_SEC_CODE
#include "IStdLib_MemMap.h"
/* PRQA S 2889,6060 ++ */ /*VL_QAC_MultiReturn,VL_MTR_IStdLib_STM19*/
#define ISTDLIB_START_SEC_CODE
#include "IStdLib_MemMap.h"
void* IStdLib_MemSet(void* ISTDLIB_RESTRICT dstptr, uint8 val, uint32 n)
/* PRQA S 2889,6060 -- */
{
    uintptr_t    vlu     = val; /* PRQA S 0317 */                  /* VL_QAC_0317 */
    uintptr_t*   dst     = dstptr; /* PRQA S 0317 */               /* VL_QAC_0317 */
    const uint8* dstLast = &((uint8*)dstptr)[n]; /* PRQA S 0316 */ /* VL_QAC_0316 */
    const uint8* dstTmp;
#if (CPU_TYPE != CPU_TYPE_8)
    if (n >= sizeof(uintptr_t))
    {
        /* Data assembly */
        for (uintptr_t i = 0u; i < sizeof(uintptr_t); i++)
        {
#if (CPU_32_WITH_16_ADR == TRUE)
            vlu <<= 16u; /* PRQA S 3120 */ /* VL_QAC_MagicNum */
#else
            vlu <<= 8u; /* PRQA S 3120 */ /* VL_QAC_MagicNum */
#endif
            vlu |= (uintptr_t)val;
        }
    }
    if (!ISTDLIB_IS_ALIGN_ADR(dst))
    {
        /* start address unaligned processing */
        dstTmp = (uint8*)GET_ALIGN_NEXT_ADR(dst);
        if ((uintptr_t)dstTmp > (uintptr_t)dstLast)
        {
            IStdLib_endMemset(dst, vlu, dstLast);
            return dstptr;
        }
        while ((uintptr_t)dst < (uintptr_t)dstTmp)
        {
            *(uint8*)dst = (uint8)vlu;
            dst          = (uintptr_t*)((uintptr_t)dst + 1uL);
        }
    }
    /* Deal with the middle */
    dstTmp = (uint8*)ISTDLIB_GET_ALIGN_4UPTR_FORWARD_ADR(dstLast);
    while ((uintptr_t)dst < (uintptr_t)dstTmp)
    {
        dst[0] = vlu;
        dst[1] = vlu;
        dst[2] = vlu; /* PRQA S 3120 */       /* VL_QAC_MagicNum */
        dst[3] = vlu; /* PRQA S 3120 */       /* VL_QAC_MagicNum */
        dst    = &dst[4uL]; /* PRQA S 3120 */ /* VL_QAC_MagicNum */
    }
    dstTmp = (uint8*)ISTDLIB_GET_ALIGN_FORWARD_ADR(dstLast);
    while ((uintptr_t)dst < (uintptr_t)dstTmp)
    {
        *dst = vlu;
        dst++;
    }
#endif /* CPU_TYPE != CPU_TYPE_8 */
    IStdLib_endMemset(dst, vlu, dstLast);
    return dstptr;
}
#define ISTDLIB_STOP_SEC_CODE
#include "IStdLib_MemMap.h"
#define ISTDLIB_START_SEC_CODE
#include "IStdLib_MemMap.h"
/*PRQA S 2889,6060 ++ */ /*VL_QAC_MultiReturn,VL_MTR_IStdLib_STM19*/
StatusType IStdLib_MemCmp(const void* ISTDLIB_RESTRICT str1ptr, const void* ISTDLIB_RESTRICT str2ptr, uint32 n)
/*PRQA S 2889,6060 -- */
{
    const uintptr_t* src     = str2ptr; /* PRQA S 0317 */                     /* VL_QAC_0317 */
    const uintptr_t* dst     = str1ptr; /* PRQA S 0317 */                     /* VL_QAC_0317 */
    const uint8*     dstLast = &((const uint8*)str1ptr)[n]; /* PRQA S 0316 */ /* VL_QAC_0316 */

#if (CPU_TYPE != CPU_TYPE_8)
    StatusType Ret = 0u;

    // Handle unaligned start
    if (!ISTDLIB_IS_ALIGN_ADR(dst))
    {
        Ret = IStdLib_handleUnalignedStart(&dst, &src, dstLast);
        if (Ret != 0u)
        {
            return Ret;
        }
    }

    // Handle aligned middle
    if (ISTDLIB_IS_ALIGN_ADR(src))
    {
        Ret = IStdLib_handleAlignedMiddle(&dst, &src, dstLast);
    }
    else
    {
        Ret = IStdLib_handleUnalignedMiddle(&dst, &src, dstLast);
    }

    if (Ret != 0u)
    {
        return Ret;
    }
#endif /* CPU_TYPE != CPU_TYPE_8 */

    // Handle remaining bytes
    return IStdLib_endMemcmp(dst, src, dstLast);
}
#define ISTDLIB_STOP_SEC_CODE
#include "IStdLib_MemMap.h"
/* ========================================== internal function definitions ========================================= */
#define ISTDLIB_START_SEC_CODE
#include "IStdLib_MemMap.h"
/*PRQA S 2889,6060 ++ */ /*VL_QAC_MultiReturn,VL_MTR_IStdLib_STM19*/
ISTDLIB_LOCAL StatusType
    IStdLib_handleUnalignedStart(const uintptr_t** dst, const uintptr_t** src, const uint8* dstLast)
/*PRQA S 2889,6060 -- */
{
    const uint8* dstTmp = (const uint8*)GET_ALIGN_NEXT_ADR(*dst);
    if ((uintptr_t)dstTmp > (uintptr_t)dstLast)
    {
        return IStdLib_endMemcmp(*dst, *src, dstLast);
    }

    while ((uintptr_t)*dst < (uintptr_t)dstTmp)
    {
        if (*(const uint8*)*dst != *(const uint8*)*src)
        {
            return 1u;
        }
        *dst = (const uintptr_t*)((uintptr_t)*dst + 1uL);
        *src = (const uintptr_t*)((uintptr_t)*src + 1uL);
    }

    return 0u;
}
#define ISTDLIB_STOP_SEC_CODE
#include "IStdLib_MemMap.h"

#define ISTDLIB_START_SEC_CODE
#include "IStdLib_MemMap.h"
/*PRQA S 2889,6060 ++ */ /*VL_QAC_MultiReturn,VL_MTR_IStdLib_STM19*/
ISTDLIB_LOCAL StatusType IStdLib_handleAlignedMiddle(const uintptr_t** dst, const uintptr_t** src, const uint8* dstLast)
/*PRQA S 2889,6060 -- */
{
    const uint8* dstTmp = (const uint8*)ISTDLIB_GET_ALIGN_4UPTR_FORWARD_ADR(dstLast);
    while ((uintptr_t)*dst < (uintptr_t)dstTmp)
    {
        /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
        if (((*dst)[0] != (*src)[0]) || ((*dst)[1] != (*src)[1]) || ((*dst)[2] != (*src)[2])
            || ((*dst)[3] != (*src)[3]))
        {
            return 1u;
        }
        *src = &(*src)[4uL];
        *dst = &(*dst)[4uL];
        /* PRQA S 3120 -- */
    }

    dstTmp = (const uint8*)ISTDLIB_GET_ALIGN_FORWARD_ADR(dstLast);
    while ((uintptr_t)*dst < (uintptr_t)dstTmp)
    {
        if (*(*dst) != *(*src))
        {
            return 1u;
        }
        (*dst)++;
        (*src)++;
    }

    return 0u;
}
#define ISTDLIB_STOP_SEC_CODE
#include "IStdLib_MemMap.h"

#define ISTDLIB_START_SEC_CODE
#include "IStdLib_MemMap.h"
/*PRQA S 2889,6060 ++ */ /*VL_QAC_MultiReturn,VL_MTR_IStdLib_STM19*/
ISTDLIB_LOCAL StatusType
    IStdLib_handleUnalignedMiddle(const uintptr_t** dst, const uintptr_t** src, const uint8* dstLast)
/*PRQA S 2889,6060 -- */
{
    uintptr_t    offsetVlu  = ISTDLIB_SUFFIX_VLU(*src);
    uintptr_t    rightShift = ISTDLIB_GET_ALIGN_RIGHT_SHIFT(*src);
    uintptr_t    leftShift  = ISTDLIB_GET_ALIGN_LEFT_SHIFT(*src);
    const uint8* dstTmp;
    /* Bound the 4-uptr block loop so (*src) + 5*W <= source_end on every iteration. */
    dstTmp = ISTDLIB_GET_4UPTR_LOOP_SAFE_ADR(dstLast, offsetVlu, *dst);
    *src   = (const uintptr_t*)ISTDLIB_GET_ALIGN_FORWARD_ADR(*src);

    while ((uintptr_t)*dst < (uintptr_t)dstTmp)
    {
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
        /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
        if (((*dst)[0] != (((*src)[0] >> rightShift) | ((*src)[1] << leftShift)))
            || ((*dst)[1] != (((*src)[1] >> rightShift) | ((*src)[2] << leftShift)))
            || ((*dst)[2] != (((*src)[2] >> rightShift) | ((*src)[3] << leftShift)))
            || ((*dst)[3] != (((*src)[3] >> rightShift) | ((*src)[4] << leftShift))))
        /* PRQA S 3120 -- */
#else
        /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
        if (((*dst)[0] != (((*src)[0] << rightShift) | ((*src)[1] >> leftShift)))
            || ((*dst)[1] != (((*src)[1] << rightShift) | ((*src)[2] >> leftShift)))
            || ((*dst)[2] != (((*src)[2] << rightShift) | ((*src)[3] >> leftShift)))
            || ((*dst)[3] != (((*src)[3] << rightShift) | ((*src)[4] >> leftShift))))
        /* PRQA S 3120 -- */
#endif
        {
            return 1u;
        }
        *src = &(*src)[4uL]; /* PRQA S 3120 */ /* VL_QAC_MagicNum */
        *dst = &(*dst)[4uL]; /* PRQA S 3120 */ /* VL_QAC_MagicNum */
    }

    /* Tail word loop reads (*src)[1] (2*W read-ahead); shrink the boundary by one
     * word when unsafe so endMemcmp's byte-by-byte handler covers the tail. */
    dstTmp = IStdLib_GetTailSafeAdr((uintptr_t)dstLast, offsetVlu);
    for (; (uintptr_t)*dst < (uintptr_t)dstTmp; (*dst)++)
    {
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
        if (*(*dst) != (((*src)[0] >> rightShift) | ((*src)[1] << leftShift)))
#else
        if (*(*dst) != (((*src)[0] << rightShift) | ((*src)[1] >> leftShift)))
#endif
        {
            return 1u;
        }
        (*src)++;
    }

    *src = (const uintptr_t*)((uintptr_t)*src + offsetVlu);
    return 0u;
}
#define ISTDLIB_STOP_SEC_CODE
#include "IStdLib_MemMap.h"

#define ISTDLIB_START_SEC_CODE
#include "IStdLib_MemMap.h"
ISTDLIB_LOCAL StatusType IStdLib_endMemcmp(const void* dst, const void* src, const void* dstLast)
{
    StatusType       Ret      = 0u;
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
        pDst = (uintptr_t*)((uintptr_t)pDst + (uintptr_t)1uLL); /* PRQA S 1258 */ /* VL_IStdLib_1258 */
        pSrc = (const uintptr_t*)((uintptr_t)pSrc + 1); /* PRQA S 1840 */         /* VL_IStdLib_1840 */
    }

    return Ret;
}
#define ISTDLIB_STOP_SEC_CODE
#include "IStdLib_MemMap.h"

#define ISTDLIB_START_SEC_CODE
#include "IStdLib_MemMap.h"
ISTDLIB_LOCAL void IStdLib_endMemcpy(void* dst, const void* src, const void* dstLast)
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
#define ISTDLIB_STOP_SEC_CODE
#include "IStdLib_MemMap.h"

#define ISTDLIB_START_SEC_CODE
#include "IStdLib_MemMap.h"
ISTDLIB_LOCAL void IStdLib_endMemset(void* dst, uintptr_t vlu, const void* dstLast)
{
    uintptr_t*   pDst     = (uintptr_t*)dst; /* PRQA S 0316 */       /* VL_QAC_0316 */
    const uint8* pDstLast = (const uint8*)dstLast; /* PRQA S 0316 */ /* VL_QAC_0316 */

    while ((uintptr_t)pDst < (uintptr_t)pDstLast)
    {
        *(uint8*)pDst = (uint8)vlu;
        pDst          = (uintptr_t*)((uintptr_t)pDst + 1uL);
    }
}
#define ISTDLIB_STOP_SEC_CODE
#include "IStdLib_MemMap.h"
/*PRQA S 0306 -- */
#endif
