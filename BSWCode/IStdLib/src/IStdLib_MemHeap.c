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
 **  @file               : IStd_MemHeap.c
 **  @author             : lizhi.huang,QinmeiChen
 **  @date               : 2024/09/09
 **  @vendor             : isoft
 **  @description        : stand lib source code
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include <stddef.h>
#include "IStdLib.h"
/* ================================================== version check ================================================= */
/* ===================================================== macros ===================================================== */

/* SLI definition */
#define ISTDLIB_MAX_LOG2_SLI (uint32)(3uL)
#define ISTDLIB_MAX_SLI      (size_type)(1uL << ISTDLIB_MAX_LOG2_SLI)

/* block align address */
#define ISTDLIB_BLOCK_ALIGN          (size_type)(ISTDLIB_MAX_SLI - 1uL)
#define ISTDLIB_ROUNDUP_SIZE(size)   (size_type)(((size) + ISTDLIB_BLOCK_ALIGN) & (size_type)(~ISTDLIB_BLOCK_ALIGN))
#define ISTDLIB_ROUNDDOWN_SIZE(size) (size_type)((size) & (size_type)(~ISTDLIB_BLOCK_ALIGN))

/* minimum block size  */
#define ISTDLIB_SMALL_BLOCK (size_type)(32u)

/* FLI definition */
#define ISTDLIB_MAX_FLI            (size_type)(16uL)
#define ISTDLIB_MAX_SLI_ARRAY_SIZE (uint16)(12u)
#define ISTDLIB_FLI_OFFSET         (size_type)(4uL)
#define ISTDLIB_REAL_FLI           (ISTDLIB_MAX_FLI - ISTDLIB_FLI_OFFSET)

#define ISTDLIB_MEMHEAP_INIT_FLAG 0x2A59FA59uL

/* bit of the block state */
#define ISTDLIB_BLOCK_STATE (size_type)(0x1u)
#define ISTDLIB_PREV_STATE  (size_type)(0x2u)

/* bit 0 of the block size */
#define ISTDLIB_FREE_BLOCK (size_type)(0x1u)
#define ISTDLIB_USED_BLOCK (size_type)(0x0u)

/* bit 1 of the block size */
#define ISTDLIB_PREV_FREE (size_type)(0x2u)
#define ISTDLIB_PREV_USED (size_type)(0x0u)

/* the minimum applicable size  */
#define ISTDLIB_MIN_APPLICABLE_SIZE ISTDLIB_ROUNDUP_SIZE((sizeof(IStdLib_FreePtrType)))
#define ISTDLIB_BLOCK_HEAD_SIZE     (size_type)(sizeof(IStdLib_BlockType) - sizeof(IStdLib_FreePtrType))
#define ISTDLIB_BLOCK_SIZE_ALIGN    ((size_type)(~(size_type)0u) - (3u))

/* ================================================ type definitions ================================================ */
#if defined(CPU_TYPE_64) && (CPU_TYPE == CPU_TYPE_64)
typedef uint32 size_type;
#elif defined(CPU_TYPE_32) && (CPU_TYPE == CPU_TYPE_32)
#if (CPU_32_WITH_16_ADR == TRUE)
typedef uint32 size_type;
#else
typedef uint16 size_type;
#endif
#elif defined(CPU_TYPE_16) && (CPU_TYPE == CPU_TYPE_16)
typedef uint16 size_type;
#else
typedef uint32 size_type;
#endif

/* free block link list pointer */
typedef struct
{
    size_type prev;
    size_type next;
} IStdLib_FreePtrType;

/* the block head */
typedef struct
{
    size_type prev;
    size_type size;
} IStdLib_BlockHeadType;

/* the block struct */
typedef struct
{
    IStdLib_BlockHeadType head;
    /* PRQA S 0750 ++ */ /* VL_IStdLib_0750 */
    union
    {
        IStdLib_FreePtrType free;
        uint8               buffer[1];
    } ptr;
    /* PRQA S 0750 -- */
} IStdLib_BlockType;

/* the manager block struct */
typedef struct
{
    uint32    initFlag;
    size_type manageSize;
    uint16    fli;
    uint8     sli[ISTDLIB_REAL_FLI];
    size_type freeLinker[ISTDLIB_REAL_FLI][ISTDLIB_MAX_SLI];

#if ISTDLIB_MEMHEAP_STATISTIC == STD_ON
    size_type realSize;
    size_type usedSize;
    size_type maxMallocSize;
#endif

} IStdLib_HeapManager;

/* ========================================== internal function declarations ======================================== */
/**
 * @brief        get LSBIT
 * @param[in]    i : the data to get LSBIT
 * @return       uint32
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
ISTDLIB_LOCAL uint32 IStdLib_MemHeapLSBIT(uint32 i);
/**
 * @brief        get MSBIT
 * @param[in]    i : the data to get MSBIT
 * @return       uint32
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
ISTDLIB_LOCAL uint32 IStdLib_MemHeapMSBIT(uint32 i);
/**
 * @brief        get the next index from fil and sli
 * @param[inout] size : request mem size
 * @param[out]   fli  : fli calculated based on size
 * @param[out]   sli  : the next level sli calculated based on size
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
ISTDLIB_LOCAL void IStdLib_MemHeapMatchNextIndex(uint32* size, uint32* fli, uint32* sli);
/**
 * @brief        get the index from fil and sli
 * @param[in]    size : request mem size
 * @param[out]   fli  : fli calculated based on size
 * @param[out]   sli  : sli calculated based on size
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
ISTDLIB_LOCAL void IStdLib_MemHeapMatchIndex(size_type size, uint32* fli, uint32* sli);
/**
 * @brief        find first free block
 * @param[in]    manager : manager block
 * @param[inout] fli     : minimum available fli in manager to meet requirements
 * @param[inout] sli     : minimum available sli in manager to meet requirements
 * @return       the point of IStdLib_BlockType
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
/* PRQA S 5016 ++ */ /* VL_IStdLib_Mutable */
ISTDLIB_LOCAL IStdLib_BlockType*
    IStdLib_MemHeapFindFirstFreeBlock(const IStdLib_HeapManager* manager, uint32* fli, uint32* sli);
/* PRQA S 5016 -- */
/**
 * @brief        extract the free block depending on fli and sli
 * @param[inout] manager : manager block
 * @param[inout] block   : memory block
 * @param[in]    fli     : the first bit index
 * @param[in]    sli     : the second bit index
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
ISTDLIB_LOCAL void
    IStdLib_MemHeapExtractBlockDirect(IStdLib_HeapManager* manager, IStdLib_BlockType* block, uint32 fli, uint32 sli);
/**
 * @brief        extract the block depending on the addr of block
 * @param[inout] manager : manager block
 * @param[inout] block   : memory block
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
ISTDLIB_LOCAL void IStdLib_MemHeapExtractBlock(IStdLib_HeapManager* manager, IStdLib_BlockType* block);
/**
 * @brief        insert the block to manager
 * @param[inout] manager : manager block
 * @param[inout] block   : memory block
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
ISTDLIB_LOCAL void IStdLib_MemHeapInsertBlock(IStdLib_HeapManager* manager, IStdLib_BlockType* block);
/**
 * @brief        get the free block depending on size
 * @param[in]    manager : manager block
 * @param[in]    block   : memory block
 * @param[in]    size    : request size
 * @return       the point of IStdLib_BlockType
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
/* PRQA S 5016 ++ */ /* VL_IStdLib_Mutable */
ISTDLIB_LOCAL IStdLib_BlockType*
    IStdLib_MemHeapSplitBlock(const IStdLib_HeapManager* manager, const IStdLib_BlockType* block, uint32 size);
/* PRQA S 5016 -- */
/**
 * @brief        bit set operation
 * @param[inout] val : value
 * @param[in]    bit : bit to set
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
ISTDLIB_LOCAL void IStdLib_setBit8(uint8* val, uint32 bit);
/**
 * @brief        bit set operation
 * @param[inout] val : value
 * @param[in]    bit : bit to set
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
ISTDLIB_LOCAL void IStdLib_setBit16(uint16* val, uint32 bit);
/**
 * @brief        bit clear operation
 * @param[inout] val : value
 * @param[in]    bit : bit to clear
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
ISTDLIB_LOCAL void IStdLib_clearBit8(uint8* val, uint32 bit);
/**
 * @brief        bit clear operation
 * @param[inout] val : value
 * @param[in]    bit : bit to clear
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
ISTDLIB_LOCAL void IStdLib_clearBit16(uint16* val, uint32 bit);

/* get the next block addr */
/**
 * @brief        get the next block addr
 * @param[in]    addr   : the origin addr
 * @param[in]    offset : the addr offset
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
/* PRQA S 5016 ++ */ /* VL_IStdLib_Mutable */
ISTDLIB_LOCAL IStdLib_BlockType* IStdLib_getBlock(size_t addrBlock, size_type offset);
/* PRQA S 5016 -- */

/**
 * @brief        block offset address and physical address conversion
 * @param[in]    block   : memory block
 * @param[in]    manager : manager block
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
ISTDLIB_LOCAL size_type IStdLib_getBlockOffset(const IStdLib_BlockType* block, const IStdLib_HeapManager* manager);

/**
 * @brief        block offset address and physical address conversion
 * @param[in]    offset  : the addr offset
 * @param[in]    manager : manager block
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
/* PRQA S 5016 ++ */ /* VL_IStdLib_Mutable */
ISTDLIB_LOCAL IStdLib_BlockType* IStdLib_getBlockAddr(size_type offset, const IStdLib_HeapManager* manager);
/* PRQA S 5016 -- */

/**
 * @brief        block link of the prev of the head of curblock
 * @param[inout] curBlock : memory block
 * @param[in]    preBlock : manager block
 * @param[in]    manager  : manager block
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
ISTDLIB_LOCAL void IStdLib_linkPrevBlock(
    IStdLib_BlockType*         curBlock,
    const IStdLib_BlockType*   preBlock,
    const IStdLib_HeapManager* manager);

/**
 * @brief        the init of IStdLib_MemHeapMalloc.
 * @param[in]    size  : the addr offset
 * @param[in]    manager : manager block
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
ISTDLIB_LOCAL StatusType IStdLib_initofMalloc(const IStdLib_HeapManager* manager, uint32 size);

/**
 * @brief        the init of IStdLib_MemHeapFree.
 * @param[in]    ptr  : the addr offset
 * @param[in]    manager : manager block
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
ISTDLIB_LOCAL StatusType IStdLib_initofFree(const IStdLib_HeapManager* manager, const void* ptr);

/**
 * @brief        the init of IStdLib_MemHeapInit.
 * @param[in]    size  : the addr offset
 * @param[in]    manager : manager block
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
ISTDLIB_LOCAL StatusType IStdLib_initofHeapInit(const IStdLib_HeapManager* manager, uint32 size);

/**
 * @brief        the init of IStdLib_MemHeapGetCurFreeMaxBlockSize
 * @param[in]    size  : the addr offset
 * @param[in]    manager : manager block
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
ISTDLIB_LOCAL StatusType IStdLib_initofGetSize(const IStdLib_HeapManager* manager, const uint32* size);
/* ============================================ internal data definitions =========================================== */
#define ISTDLIB_START_SEC_CONST_32
#include "IStdLib_MemMap.h"
ISTDLIB_LOCAL const uint32 IStdLib_MemHeapTab[] = {
    0xFFFFFFFFu, 0u, 1u, 1u, 2u, 2u, 2u, 2u, 3u, 3u, 3u, 3u, 3u, 3u, 3u, 3u, 4u, 4u, 4u, 4u, 4u, 4u, 4u, 4u, 4u, 4u,
    4u,          4u, 4u, 4u, 4u, 4u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u,
    5u,          5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u,
    6u,          6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u,
    6u,          6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 7u, 7u,
    7u,          7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u,
    7u,          7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u,
    7u,          7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u,
    7u,          7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u,
    7u,          7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u};
#define ISTDLIB_STOP_SEC_CONST_32
#include "IStdLib_MemMap.h"

/* ============================================ external data definitions =========================================== */
/* ========================================== external function definitions ========================================= */
#define ISTDLIB_START_SEC_CODE
#include "IStdLib_MemMap.h"
/*PRQA S 1503 ++ */                                                        /*VL_QAC_NoUsedApi*/
uint8 IStdLib_MemHeapInit(void* ramHeap, uint32 size) /*PRQA S 2889,6060*/ /*VL_QAC_MultiReturn,VL_MTR_IStdLib_STM19*/
{
    StatusType           ret     = ISTDLIB_MEMHEAP_OK;
    IStdLib_HeapManager* manager = (IStdLib_HeapManager*)ramHeap; /* PRQA S 0316 */ /* VL_QAC_0316 */

    ret = IStdLib_initofHeapInit(manager, size);
    if (ret != ISTDLIB_MEMHEAP_OK)
    {
        return ret;
    }

    (void)IStdLib_MemSet(manager, 0u, sizeof(IStdLib_HeapManager));

    manager->initFlag   = ISTDLIB_MEMHEAP_INIT_FLAG;
    manager->manageSize = (size_type)size;

    /* system block : always used */
    /*PRQA S 0306 ++ */ /*VL_IStdLib_0306*/
    IStdLib_BlockType* firstBlock =
        IStdLib_getBlock((size_t)manager, ISTDLIB_ROUNDUP_SIZE(sizeof(IStdLib_HeapManager)));
    /*PRQA S 0306 -- */

    firstBlock->head.prev = 0u;
    firstBlock->head.size = 0u | ISTDLIB_USED_BLOCK | ISTDLIB_PREV_USED;

    /* user block */
    /*PRQA S 0306 ++ */ /*VL_IStdLib_0306*/
    IStdLib_BlockType* block = IStdLib_getBlock((size_t)firstBlock, ISTDLIB_ROUNDUP_SIZE(ISTDLIB_BLOCK_HEAD_SIZE));
    /*PRQA S 0306 -- */
    IStdLib_linkPrevBlock(block, firstBlock, manager);
    block->ptr.free.prev = 0u;
    block->ptr.free.next = 0u;
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */ /*PRQA S 2985 ++ */ /*VL_IStdLib_Redundant*/
    block->head.size = ISTDLIB_ROUNDDOWN_SIZE((size_type)((
                           (size_type)size - ISTDLIB_ROUNDUP_SIZE(sizeof(IStdLib_HeapManager))
                           - ((size_type)3u * (ISTDLIB_BLOCK_HEAD_SIZE)))))
                       | ISTDLIB_USED_BLOCK | ISTDLIB_PREV_USED;
    /* PRQA S 3120 -- */ /*PRQA S 2985 -- */

    /* system block : always used */
    /*PRQA S 0306 ++ */ /*VL_IStdLib_0306*/ /*PRQA S 2985 ++ */ /*VL_IStdLib_Redundant*/
    IStdLib_BlockType* endBlock =
        IStdLib_getBlock((size_t)(block->ptr.buffer), block->head.size & ISTDLIB_BLOCK_SIZE_ALIGN);
    /*PRQA S 0306 -- */ /*PRQA S 2985 -- */
    IStdLib_linkPrevBlock(endBlock, block, manager);
    endBlock->head.size = 0u | ISTDLIB_USED_BLOCK | ISTDLIB_PREV_FREE;

    manager->manageSize = (size_type)size;

#if ISTDLIB_MEMHEAP_STATISTIC == STD_ON
    manager->realSize      = block->head.size;
    manager->usedSize      = block->head.size;
    manager->maxMallocSize = 0u;
#endif
    /* release user block */
    ret = IStdLib_MemHeapFree(ramHeap, block->ptr.buffer);

    return ret;
}

void* IStdLib_MemHeapMalloc(void* ramHeap, uint32 size)
{
    StatusType           ret = ISTDLIB_MEMHEAP_OK;
    IStdLib_HeapManager* manager;
    void*                ptr = NULL_PTR;

    manager = (IStdLib_HeapManager*)ramHeap; /* PRQA S 0316 */ /* VL_QAC_0316 */

    ret = IStdLib_initofMalloc(manager, size);
    if (ret == ISTDLIB_MEMHEAP_OK)
    {
        uint32 fli;
        uint32 sli;

        /* The size must be greater than or equal to ISTDLIB_MIN_APPLICABLE_SIZE and byte alignment */
        uint32 localSize =
            (size < ISTDLIB_MIN_APPLICABLE_SIZE) ? ISTDLIB_MIN_APPLICABLE_SIZE : ISTDLIB_ROUNDUP_SIZE(size);

        IStdLib_MemHeapMatchNextIndex(&localSize, &fli, &sli);
        IStdLib_BlockType* block = IStdLib_MemHeapFindFirstFreeBlock(manager, &fli, &sli);

        if (NULL_PTR != block)
        {
            IStdLib_MemHeapExtractBlockDirect(manager, block, fli, sli);
            /*PRQA S 0306 ++ */ /*VL_IStdLib_0306*/
            IStdLib_BlockType* nextBlock =
                IStdLib_getBlock((size_t)(block->ptr.buffer), block->head.size & ISTDLIB_BLOCK_SIZE_ALIGN);
            /*PRQA S 0306 -- */

            /* unused space forms a new free block */
            IStdLib_BlockType* splitBlock = IStdLib_MemHeapSplitBlock(manager, block, localSize);

            if (NULL_PTR != splitBlock)
            {
                /* put unused blocks back into the free link list */
                IStdLib_MemHeapInsertBlock(manager, splitBlock);

                /* modify the block size, but use old state of the block */
                block->head.size = ((size_type)localSize | (block->head.size & (size_type)ISTDLIB_PREV_STATE))
                                   & (size_type)(~ISTDLIB_FREE_BLOCK);
            }
            else
            {
                /* modify the block size, but use old state of the block */
                block->head.size &= (size_type)(~ISTDLIB_FREE_BLOCK);

                nextBlock->head.size &= (size_type)(~ISTDLIB_PREV_STATE);
            }
#if ISTDLIB_MEMHEAP_STATISTIC == STD_ON
            manager->usedSize = manager->usedSize + (size_type)(block->head.size);

            if (manager->maxMallocSize < block->head.size)
            {
                manager->maxMallocSize = (size_type)(block->head.size);
            }
#endif
            ptr = (void*)block->ptr.buffer; /* PRQA S 0314 */ /* VL_IStdLib_0314 */
            /*PRQA S 0306 ++ */                               /*VL_IStdLib_0306*/
            if (!(((size_t)block >= ((size_t)manager + (size_t)sizeof(IStdLib_HeapManager)))
                  && (((size_t)block) < ((size_t)manager + manager->manageSize - (size_t)sizeof(IStdLib_BlockHeadType)))
                  && (((size_t)block + block->head.size)
                      >= ((size_t)manager + (size_t)sizeof(IStdLib_HeapManager)
                          + (size_t)sizeof(IStdLib_BlockHeadType)))
                  && (((size_t)block + block->head.size)
                      < ((size_t)manager + manager->manageSize - (size_t)sizeof(IStdLib_BlockHeadType)))))
            /*PRQA S 0306 -- */
            {
                ret = ISTDLIB_MEMHEAP_INVALID_PTR;
                ptr = NULL_PTR;
            }
        }
    }

    (void)ret;
    return ptr;
}

void* IStdLib_MemHeapCalloc(void* ramHeap, uint32 count, uint32 size) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    void* ptr = IStdLib_MemHeapMalloc(ramHeap, count * size);
    if (NULL_PTR != ptr)
    {
        (void)IStdLib_MemSet(ptr, 0u, count * size);
    }
    return ptr;
}
/* PRQA S 2889,6060 ++ */ /*VL_QAC_MultiReturn,VL_MTR_IStdLib_STM19*/
uint8 IStdLib_MemHeapFree(void* ramHeap, const void* ptr)
/* PRQA S 2889,6060 -- */
{
    StatusType           ret     = (StatusType)ISTDLIB_MEMHEAP_OK;
    IStdLib_HeapManager* manager = (IStdLib_HeapManager*)ramHeap; /* PRQA S 0316 */ /* VL_QAC_0316 */
    /*PRQA S 0306,0326 ++ */                                                        /*VL_IStdLib_0306*/
    IStdLib_BlockType* block = (IStdLib_BlockType*)((size_t)ptr - ISTDLIB_BLOCK_HEAD_SIZE);
    /*PRQA S 0306,0326 -- */

    ret = IStdLib_initofFree(manager, ptr);

    if (ret != ISTDLIB_MEMHEAP_OK)
    {
        return ret;
    }

    /* pointer not in valid range */
    /*PRQA S 0306 ++ */ /*VL_IStdLib_0306*/
    if (!(((size_t)block >= ((size_t)manager + (size_t)sizeof(IStdLib_HeapManager)))
          && (((size_t)block) < ((size_t)manager + manager->manageSize - (size_t)sizeof(IStdLib_BlockHeadType)))
          && (((size_t)block + block->head.size)
              >= ((size_t)manager + (size_t)sizeof(IStdLib_HeapManager) + (size_t)sizeof(IStdLib_BlockHeadType)))
          && (((size_t)block + block->head.size)
              < ((size_t)manager + manager->manageSize - (size_t)sizeof(IStdLib_BlockHeadType)))))
    /*PRQA S 0306 -- */
    {
        ret = ISTDLIB_MEMHEAP_INVALID_PTR;
    }

    else
    {
#if ISTDLIB_MEMHEAP_STATISTIC == STD_ON
        manager->usedSize = manager->usedSize - (block->head.size & (size_type)ISTDLIB_BLOCK_SIZE_ALIGN);
#endif

        block->head.size |= ISTDLIB_FREE_BLOCK;

        block->ptr.free.prev = 0u;
        block->ptr.free.next = 0u;
        /*PRQA S 0306 ++ */ /*VL_IStdLib_0306*/
        IStdLib_BlockType* tmp_block =
            IStdLib_getBlock((size_t)(block->ptr.buffer), block->head.size & ISTDLIB_BLOCK_SIZE_ALIGN);
        /*PRQA S 0306 --*/

        /* the next block of the current block is free */
        if (0u != (tmp_block->head.size & ISTDLIB_BLOCK_STATE))
        {
            IStdLib_MemHeapExtractBlock(manager, tmp_block);

            /* merge the current size with the size of the next block */
            block->head.size += (tmp_block->head.size & (size_type)ISTDLIB_BLOCK_SIZE_ALIGN) + ISTDLIB_BLOCK_HEAD_SIZE;
        }

        /* the previous block of the current block is free */
        if (0u != (block->head.size & ISTDLIB_PREV_STATE))
        {
            tmp_block = IStdLib_getBlockAddr(block->head.prev, manager);
            IStdLib_MemHeapExtractBlock(manager, tmp_block);
            /*PRQA S 2812 ++ */ /*VL_IStdLib_NullPointer*/
            tmp_block->head.size += (block->head.size & (size_type)ISTDLIB_BLOCK_SIZE_ALIGN) + ISTDLIB_BLOCK_HEAD_SIZE;
            /*PRQA S 2812 -- */
            block = tmp_block;
        }

        /* put the merged block back into the free link list */
        IStdLib_MemHeapInsertBlock(manager, block);
        /*PRQA S 0306 ++ */ /*VL_IStdLib_0306*/
        tmp_block = IStdLib_getBlock((size_t)(block->ptr.buffer), block->head.size & ISTDLIB_BLOCK_SIZE_ALIGN);
        /*PRQA S 0306 -- */

        /* change the 'prev' state of the next block to free */
        tmp_block->head.size |= ISTDLIB_PREV_FREE;
        tmp_block->head.prev = IStdLib_getBlockOffset(block, manager);
    }
    return ret;
}

#if ISTDLIB_MEMHEAP_STATISTIC == STD_ON

uint8 IStdLib_MemHeapGetManageSize(const void* ramHeap, uint32* size)
{
    StatusType                 ret     = ISTDLIB_MEMHEAP_OK;
    const IStdLib_HeapManager* manager = (const IStdLib_HeapManager*)ramHeap; /* PRQA S 0316 */ /* VL_QAC_0316 */

    /* illegal pointer */
    if (NULL_PTR == manager)
    {
        ret = ISTDLIB_MEMHEAP_INVALID_PTR;
    }

    /* address not byte aligned */
    else if (0uL != ((size_t)manager & (sizeof(void*) - 1uL))) /*PRQA S 0306 */ /*VL_IStdLib_0306*/
    {
        ret = ISTDLIB_MEMHEAP_INVALID_ALIGN;
    }

    /* uninitialized */
    else if (ISTDLIB_MEMHEAP_INIT_FLAG != manager->initFlag)
    {
        ret = ISTDLIB_MEMHEAP_INVALID_INIT;
    }

    /* illegal pointer */
    else if (NULL_PTR == size)
    {
        ret = ISTDLIB_MEMHEAP_INVALID_PTR;
    }

    else
    {
        *size = (uint32)manager->manageSize;
    }

    return ret;
}

uint8 IStdLib_MemHeapGetRealMaxSize(const void* ramHeap, uint32* size)
{
    StatusType                 ret     = ISTDLIB_MEMHEAP_OK;
    const IStdLib_HeapManager* manager = (const IStdLib_HeapManager*)ramHeap; /* PRQA S 0316 */ /* VL_QAC_0316 */

    /* illegal pointer */
    if (NULL_PTR == manager)
    {
        ret = ISTDLIB_MEMHEAP_INVALID_PTR;
    }

    /* address not byte aligned */
    else if (0uL != ((size_t)manager & (sizeof(void*) - 1uL))) /*PRQA S 0306 */ /*VL_IStdLib_0306*/
    {
        ret = ISTDLIB_MEMHEAP_INVALID_ALIGN;
    }

    /* uninitialized */
    else if (ISTDLIB_MEMHEAP_INIT_FLAG != manager->initFlag)
    {
        ret = ISTDLIB_MEMHEAP_INVALID_INIT;
    }

    /* illegal pointer */
    else if (NULL_PTR == size)
    {
        ret = ISTDLIB_MEMHEAP_INVALID_PTR;
    }

    else
    {
        *size = (uint32)manager->realSize;
    }

    return ret;
}

uint8 IStdLib_MemHeapGetUsedSize(const void* ramHeap, uint32* size)
{
    StatusType                 ret     = ISTDLIB_MEMHEAP_OK;
    const IStdLib_HeapManager* manager = (const IStdLib_HeapManager*)ramHeap; /* PRQA S 0316 */ /* VL_QAC_0316 */

    /* illegal pointer */
    if (NULL_PTR == manager)
    {
        ret = ISTDLIB_MEMHEAP_INVALID_PTR;
    }

    /* address not byte aligned */
    else if (0uL != ((size_t)manager & (sizeof(void*) - 1uL))) /*PRQA S 0306 */ /*VL_IStdLib_0306*/
    {
        ret = ISTDLIB_MEMHEAP_INVALID_ALIGN;
    }

    /* uninitialized */
    else if (ISTDLIB_MEMHEAP_INIT_FLAG != manager->initFlag)
    {
        ret = ISTDLIB_MEMHEAP_INVALID_INIT;
    }

    /* illegal pointer */
    else if (NULL_PTR == size)
    {
        ret = ISTDLIB_MEMHEAP_INVALID_PTR;
    }

    else
    {
        *size = (uint32)manager->usedSize;
    }

    return ret;
}

/*PRQA S 2889,6060 ++ */ /*VL_QAC_MultiReturn,VL_MTR_IStdLib_STM19*/
uint8 IStdLib_MemHeapGetCurFreeMaxBlockSize(const void* ramHeap, uint32* size)
/*PRQA S 2889,6060 -- */
{
    StatusType                 ret        = ISTDLIB_MEMHEAP_OK;
    const IStdLib_HeapManager* manager    = (const IStdLib_HeapManager*)ramHeap; /* PRQA S 0316 */ /* VL_QAC_0316 */
    uint32                     tempSize   = 0u;
    uint32                     maxSizeRam = 0u;

    ret = IStdLib_initofGetSize(manager, size);
    if (ret != ISTDLIB_MEMHEAP_OK)
    {
        return ret;
    }
    if (manager->fli > (uint16)0u)
    {
        uint32 fli = IStdLib_MemHeapMSBIT((uint32)manager->fli);
        if (fli > ISTDLIB_MAX_SLI_ARRAY_SIZE)
        {
            ret = ISTDLIB_MEMHEAP_INVALID_PTR;
            return ret;
        }
        uint32 sli = IStdLib_MemHeapMSBIT((uint32)manager->sli[fli]);
        if (sli < ((uint32)1uL << ISTDLIB_MAX_SLI))
        {
            const IStdLib_BlockType* block = IStdLib_getBlockAddr(manager->freeLinker[fli][sli], manager);
            while (block != NULL_PTR)
            {
                tempSize = (fli * ISTDLIB_SMALL_BLOCK) + (((fli * ISTDLIB_SMALL_BLOCK) >> ISTDLIB_MAX_LOG2_SLI) * sli);
                maxSizeRam = (maxSizeRam < tempSize) ? tempSize : maxSizeRam;

                block = IStdLib_getBlockAddr(block->ptr.free.next, manager);
            }
            maxSizeRam = ISTDLIB_ROUNDDOWN_SIZE(maxSizeRam);
        }
    }
    *size = (size_type)maxSizeRam;

    return ret;
}

uint8 IStdLib_MemHeapGetMaxMallocSize(const void* ramHeap, uint32* size)
{
    StatusType                 ret     = ISTDLIB_MEMHEAP_OK;
    const IStdLib_HeapManager* manager = (const IStdLib_HeapManager*)ramHeap; /* PRQA S 0316 */ /* VL_QAC_0316 */

    /* illegal pointer */
    if (NULL_PTR == manager)
    {
        ret = ISTDLIB_MEMHEAP_INVALID_PTR;
    }

    /* address not byte aligned */
    else if (0uL != ((size_t)manager & (sizeof(void*) - 1uL))) /*PRQA S 0306 */ /*VL_IStdLib_0306*/
    {
        ret = ISTDLIB_MEMHEAP_INVALID_ALIGN;
    }

    /* uninitialized */
    else if (ISTDLIB_MEMHEAP_INIT_FLAG != manager->initFlag)
    {
        ret = ISTDLIB_MEMHEAP_INVALID_INIT;
    }

    /* illegal pointer */
    else if (NULL_PTR == size)
    {
        ret = ISTDLIB_MEMHEAP_INVALID_PTR;
    }

    else
    {
        *size = (uint32)manager->maxMallocSize;
    }

    return ret;
}

#endif

/* ========================================== internal function definitions ========================================= */
ISTDLIB_LOCAL uint32 IStdLib_MemHeapLSBIT(uint32 i)
{
    uint32 a;
    uint32 x = i & ((size_type)0u - i); /*PRQA S 2986*/ /*VL_IStdLib_Redundant*/
    /* PRQA S 3120 ++ */                                /* VL_QAC_MagicNum */
    a = (x <= 0xFFFFuL) ? ((x <= 0xFFuL) ? 0uL : 8uL) : ((x <= 0xFFFFFFuL) ? 16uL : 24uL);
    /* PRQA S 3120 -- */
    a = a + IStdLib_MemHeapTab[x >> a];
    return a;
}

ISTDLIB_LOCAL uint32 IStdLib_MemHeapMSBIT(uint32 i)
{
    uint32 a;
    uint32 x = i;
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    a = (x <= 0xFFFFuL) ? ((x <= 0xFFuL) ? 0uL : 8uL) : ((x <= 0xFFFFFFuL) ? 16uL : 24uL);
    /* PRQA S 3120 -- */
    a = a + IStdLib_MemHeapTab[x >> a];
    return a;
}

ISTDLIB_LOCAL void IStdLib_MemHeapMatchNextIndex(uint32* size, uint32* fli, uint32* sli)
{
    uint32 slBlockSize;

    if (*size < ISTDLIB_SMALL_BLOCK)
    {
        *fli = 0u;
        *sli = (*size / (ISTDLIB_SMALL_BLOCK / ISTDLIB_MAX_SLI));
    }
    else
    {
        /* calculate the fine-grained of blocks that can accommodate size */
        slBlockSize = ((uint32)1uL << (IStdLib_MemHeapMSBIT(*size) - ISTDLIB_MAX_LOG2_SLI)) - 1uL;

        /* the size needs to be obtained from the next sl greater than the current sl
           to avoid lookup */
        *size = *size + slBlockSize;

        /* Size greater than ISTDLIB_SMALL_BLOCK requires calculation of fli and sli */
        *fli = IStdLib_MemHeapMSBIT(*size);
        *sli = (*size >> (*fli - ISTDLIB_MAX_LOG2_SLI)) - ISTDLIB_MAX_SLI;
        *fli -= ISTDLIB_FLI_OFFSET;

        /* take slBlockSize from size */
        *size &= (size_type)(~(size_type)slBlockSize);
    }
}

ISTDLIB_LOCAL void IStdLib_MemHeapMatchIndex(size_type size, uint32* fli, uint32* sli)
{
    if (size < ISTDLIB_SMALL_BLOCK)
    {
        /* size smaller than ISTDLIB_SMALL_BLOCK Block,
           then directly search for sli from the position where fli=0,
           with a stride of 8 bytes for each sli
        */
        *fli = 0uL;
        *sli = ((uint32)size / (ISTDLIB_SMALL_BLOCK / ISTDLIB_MAX_SLI));
    }
    else
    {
        *fli = IStdLib_MemHeapMSBIT(size);
        /* find sli from the fli level. the scope of SLI:[0, ISTDLIB_MAX_SLI - 1] */
        *sli = ((uint32)size >> (*fli - ISTDLIB_MAX_LOG2_SLI)) - ISTDLIB_MAX_SLI;
        /* convert from logical fli to physical fli */
        /* fli 0:[0, ISTDLIB_SMALL_BLOCK-1] 1:[ISTDLIB_SMALL_BLOCK, ISTDLIB_SMALL_BLOCK*2-1] */
        *fli -= ISTDLIB_FLI_OFFSET;
    }
}

ISTDLIB_LOCAL IStdLib_BlockType*
    IStdLib_MemHeapFindFirstFreeBlock(const IStdLib_HeapManager* manager, uint32* fli, uint32* sli)
{
    uint32             tmpSli = ((uint32)manager->sli[*fli] & ((~(uint32)0uL) << *sli));
    IStdLib_BlockType* block  = NULL_PTR;

    if (tmpSli > 0uL)
    {
        *sli  = IStdLib_MemHeapLSBIT(tmpSli);
        block = IStdLib_getBlockAddr(manager->freeLinker[*fli][*sli], manager);
    }
    else
    {
        /* no sli that meets the condition, get the next non-0 fl of the current fl  */
        *fli = IStdLib_MemHeapLSBIT((uint32)manager->fli & (~(uint32)0uL << (*fli + 1uL)));
        if ((*fli > 0uL) && (*fli < ((uint32)ISTDLIB_REAL_FLI)))
        {
            *sli  = IStdLib_MemHeapLSBIT((uint32)manager->sli[*fli]);
            block = IStdLib_getBlockAddr(manager->freeLinker[*fli][*sli], manager);
        }
        else
        {
            *fli = 0u;
        }
    }
    return block;
}
/*PRQA S 2889,6060 ++ */ /*VL_QAC_MultiReturn,VL_MTR_IStdLib_STM19*/
ISTDLIB_LOCAL void IStdLib_MemHeapExtractBlockDirect(
    /*PRQA S 2889,6060 -- */
    IStdLib_HeapManager* manager,
    IStdLib_BlockType*   block,
    uint32               fli,
    uint32               sli)
{
    /* this block must be the first one for the link list */
    manager->freeLinker[fli][sli] = block->ptr.free.next;
    if (0u != manager->freeLinker[fli][sli])
    {
        /*PRQA S 2812 ++ */ /*VL_IStdLib_NullPointer*/
        IStdLib_getBlockAddr(manager->freeLinker[fli][sli], manager)->ptr.free.prev = 0u;
        /*PRQA S 2812 -- */
    }
    else
    {
        /* there are no free blocks after this block */
        IStdLib_clearBit8(&manager->sli[fli], sli);
        if (0u == manager->sli[fli])
        {
            IStdLib_clearBit16(&manager->fli, fli);
        }
    }
    block->ptr.free.prev = 0u;
    block->ptr.free.next = 0u;
}

ISTDLIB_LOCAL void IStdLib_MemHeapExtractBlock(IStdLib_HeapManager* manager, IStdLib_BlockType* block)
{
    IStdLib_BlockType* tempBlock;
    uint32             fli;
    uint32             sli;

    IStdLib_MemHeapMatchIndex(block->head.size & (size_type)ISTDLIB_BLOCK_SIZE_ALIGN, &fli, &sli);

    if (0u != block->ptr.free.next)
    {
        tempBlock                = IStdLib_getBlockAddr(block->ptr.free.next, manager);
        tempBlock->ptr.free.prev = block->ptr.free.prev;
    }

    if (0u != block->ptr.free.prev)
    {
        tempBlock                = IStdLib_getBlockAddr(block->ptr.free.prev, manager);
        tempBlock->ptr.free.next = block->ptr.free.next;
    }

    /* the block is head of free block link list */
    if (manager->freeLinker[fli][sli] == IStdLib_getBlockOffset(block, manager))
    {
        manager->freeLinker[fli][sli] = block->ptr.free.next;

        if (0u == manager->freeLinker[fli][sli])
        {
            /* there are no free block in sl */
            IStdLib_clearBit8(&manager->sli[fli], sli);
            if (0u == manager->sli[fli])
            {
                /* there are no non-zero sl in the fl */
                IStdLib_clearBit16(&manager->fli, fli);
            }
        }
    }

    block->ptr.free.prev = 0u;
    block->ptr.free.next = 0u;
}

ISTDLIB_LOCAL void IStdLib_MemHeapInsertBlock(
    IStdLib_HeapManager* manager,
    IStdLib_BlockType*   block) /*PRQA S 2889,6060*/ /*VL_QAC_MultiReturn,VL_MTR_IStdLib_STM19*/
{
    uint32 fli = 0u;
    uint32 sli = 0u;

    /* Calculate the corresponding fli and sli based on the block size */
    IStdLib_MemHeapMatchIndex(block->head.size & (size_type)ISTDLIB_BLOCK_SIZE_ALIGN, &fli, &sli);

    block->ptr.free.prev = 0u;
    block->ptr.free.next = manager->freeLinker[fli][sli];

    if (0u != manager->freeLinker[fli][sli])
    {
        /* freeLinker is not empty, point the prev of the first node
           of freeLinker to the block node */
        /*PRQA S 2812 ++ */ /*VL_IStdLib_NullPointer*/
        IStdLib_getBlockAddr(manager->freeLinker[fli][sli], manager)->ptr.free.prev =
            IStdLib_getBlockOffset(block, manager);
        /*PRQA S 2812 -- */
    }

    /* point freeLinker to block as freeLinker's first node */
    manager->freeLinker[fli][sli] = IStdLib_getBlockOffset(block, manager);

    IStdLib_setBit8(&manager->sli[fli], sli);
    IStdLib_setBit16(&manager->fli, fli);
}

ISTDLIB_LOCAL IStdLib_BlockType*
    IStdLib_MemHeapSplitBlock(const IStdLib_HeapManager* manager, const IStdLib_BlockType* block, uint32 size)
{
    IStdLib_BlockType* splitBlock = (IStdLib_BlockType*)NULL_PTR;
    uint16             remainSize = (block->head.size & (size_type)ISTDLIB_BLOCK_SIZE_ALIGN) - (size_type)size;

    if (remainSize >= sizeof(IStdLib_BlockType))
    {
        remainSize = remainSize - ISTDLIB_BLOCK_HEAD_SIZE;

        /* unused space forms a new free block */
        /*PRQA S 0306 ++ */ /*VL_IStdLib_0306*/
        splitBlock = IStdLib_getBlock((size_t)(block->ptr.buffer), (size_type)size);
        /*PRQA S 0306 -- */
        /*PRQA S 2985 ++ */ /*VL_IStdLib_Redundant*/
        splitBlock->head.size = remainSize | ISTDLIB_FREE_BLOCK | ISTDLIB_PREV_USED;
        /*PRQA S 2985 -- */

        /* the prev pointer of the next node in the block
           points to the splitBlock node after splitting */
        /*PRQA S 0306 ++ */ /*VL_IStdLib_0306*/
        IStdLib_BlockType* nextBlock =
            IStdLib_getBlock((size_t)(block->ptr.buffer), block->head.size & ISTDLIB_BLOCK_SIZE_ALIGN);
        /*PRQA S 0306 -- */

        IStdLib_linkPrevBlock(splitBlock, block, manager);
        IStdLib_linkPrevBlock(nextBlock, splitBlock, manager);
    }

    return splitBlock;
}

ISTDLIB_LOCAL void IStdLib_setBit8(uint8* val, uint32 bit)
{
    *val = (*val | (uint8)((uint8)1u << (bit)));
}

ISTDLIB_LOCAL void IStdLib_setBit16(uint16* val, uint32 bit)
{
    *val = (*val | (uint16)((uint16)1u << (bit)));
}

ISTDLIB_LOCAL void IStdLib_clearBit8(uint8* val, uint32 bit)
{
    *val = (*val & (uint8)(~(uint8)((uint8)1u << (bit))));
}

ISTDLIB_LOCAL void IStdLib_clearBit16(uint16* val, uint32 bit)
{
    *val = (*val & (uint16)(~(uint16)((uint16)1u << (bit))));
}

ISTDLIB_LOCAL IStdLib_BlockType* IStdLib_getBlock(size_t addrBlock, size_type offset)
{
    return ((IStdLib_BlockType*)((addrBlock) + (size_t)(offset))); /*PRQA S 0306 */ /*VL_IStdLib_0306*/
}

ISTDLIB_LOCAL size_type IStdLib_getBlockOffset(const IStdLib_BlockType* block, const IStdLib_HeapManager* manager)
{
    /*PRQA S 0306 ++ */ /*VL_IStdLib_0306*/
    return (((block) == NULL_PTR) ? (size_type)0u : (size_type)((size_t)(block) - (size_t)(manager)));
    /*PRQA S 0306 -- */
}

ISTDLIB_LOCAL IStdLib_BlockType* IStdLib_getBlockAddr(size_type offset, const IStdLib_HeapManager* manager)
{
    /*PRQA S 0306 ++ */ /*VL_IStdLib_0306*/
    return ((IStdLib_BlockType*)((offset == 0u) ? (size_t)0uL : ((size_t)(manager) + (size_t)(offset))));
    /*PRQA S 0306 -- */
}

ISTDLIB_LOCAL void IStdLib_linkPrevBlock(
    IStdLib_BlockType*         curBlock,
    const IStdLib_BlockType*   preBlock,
    const IStdLib_HeapManager* manager)
{
    ((curBlock)->head.prev = IStdLib_getBlockOffset(preBlock, manager));
}

ISTDLIB_LOCAL StatusType IStdLib_initofMalloc(const IStdLib_HeapManager* manager, uint32 size)
{
    StatusType ret = ISTDLIB_MEMHEAP_OK;
    /* illegal pointer */
    if (NULL_PTR == manager)
    {
        ret = ISTDLIB_MEMHEAP_INVALID_PTR;
    }

    /* address not byte aligned */
    else if (0uL != ((size_t)manager & (size_t)(sizeof(void*) - (size_t)1uL))) /*PRQA S 0306*/ /*VL_IStdLib_0306*/
    {
        ret = ISTDLIB_MEMHEAP_INVALID_ALIGN;
    }

    /* uninitialized */
    else if (ISTDLIB_MEMHEAP_INIT_FLAG != manager->initFlag)
    {
        ret = ISTDLIB_MEMHEAP_INVALID_INIT;
    }

    /* unable to manage space larger than 64K */
    else
    {
        ret = (size > 0xFFFFuL) ? ISTDLIB_MEMHEAP_INVALID_SIZE /*PRQA S 3120*/ /*VL_QAC_MagicNum*/
                                : ((0uL == size) ? ISTDLIB_MEMHEAP_INVALID_SIZE : ISTDLIB_MEMHEAP_OK);
    }

    return ret;
}

ISTDLIB_LOCAL StatusType IStdLib_initofFree(const IStdLib_HeapManager* manager, const void* ptr)
{
    StatusType ret = ISTDLIB_MEMHEAP_OK;
    /* illegal pointer */
    if (NULL_PTR == manager)
    {
        ret = ISTDLIB_MEMHEAP_INVALID_PTR;
    }

    /* address not byte aligned */
    else if (0uL != ((size_t)manager & (sizeof(void*) - 1uL))) /*PRQA S 0306*/ /*VL_IStdLib_0306*/
    {
        ret = ISTDLIB_MEMHEAP_INVALID_ALIGN;
    }

    /* uninitialized */
    else if (ISTDLIB_MEMHEAP_INIT_FLAG != manager->initFlag)
    {
        ret = ISTDLIB_MEMHEAP_INVALID_INIT;
    }

    /* illegal pointer */
    else if (ptr == NULL_PTR)
    {
        ret = ISTDLIB_MEMHEAP_INVALID_PTR;
    }
    else
    {
        ret = ISTDLIB_MEMHEAP_OK;
    }
    return ret;
}

ISTDLIB_LOCAL StatusType IStdLib_initofHeapInit(const IStdLib_HeapManager* manager, uint32 size)
{
    StatusType ret = ISTDLIB_MEMHEAP_OK;

    /* illegal pointer */
    if (NULL_PTR == manager)
    {
        ret = ISTDLIB_MEMHEAP_INVALID_PTR;
    }

    /* address not byte aligned */
    else if (0uL != ((size_t)manager & (sizeof(void*) - 1uL))) /*PRQA S 0306*/ /*VL_IStdLib_0306*/
    {
        ret = ISTDLIB_MEMHEAP_INVALID_ALIGN;
    }
    /* repeated initialization */
    else if (ISTDLIB_MEMHEAP_INIT_FLAG == manager->initFlag)
    {
        ret = ISTDLIB_MEMHEAP_INVALID_INIT;
    }
    else
    {
        /* unable to manage space larger than 64K */
        ret = (size > 0xFFFFuL) ? ISTDLIB_MEMHEAP_INVALID_SIZE /*PRQA S 3120*/ /*VL_QAC_MagicNum*/
                                /* size too small for initialization */
                                : ((size
                                    < (sizeof(IStdLib_HeapManager)
                                       + (sizeof(IStdLib_BlockType) * 2u))) /* PRQA S 3120 */ /* VL_QAC_MagicNum */
                                       ? ISTDLIB_MEMHEAP_INVALID_SIZE
                                       : ISTDLIB_MEMHEAP_OK);
    }
    return ret;
}

ISTDLIB_LOCAL StatusType IStdLib_initofGetSize(const IStdLib_HeapManager* manager, const uint32* size)
{
    StatusType ret = ISTDLIB_MEMHEAP_OK;
    /* illegal pointer */
    if (NULL_PTR == manager)
    {
        ret = ISTDLIB_MEMHEAP_INVALID_PTR;
    }

    /* address not byte aligned */
    else if (0uL != ((size_t)manager & (sizeof(void*) - 1uL))) /*PRQA S 0306 */ /*VL_IStdLib_0306*/
    {
        ret = ISTDLIB_MEMHEAP_INVALID_ALIGN;
    }

    /* uninitialized */
    else if (ISTDLIB_MEMHEAP_INIT_FLAG != manager->initFlag)
    {
        ret = ISTDLIB_MEMHEAP_INVALID_INIT;
    }

    /* illegal pointer */
    else if (NULL_PTR == size)
    {
        ret = ISTDLIB_MEMHEAP_INVALID_PTR;
    }
    else
    {
        ret = ISTDLIB_MEMHEAP_OK;
    }
    return ret;
}
/*PRQA S 1503 -- */
#define ISTDLIB_STOP_SEC_CODE
#include "IStdLib_MemMap.h"
