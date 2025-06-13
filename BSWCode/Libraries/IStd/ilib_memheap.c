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
**  FILENAME    : ilib_memheap.c                                                                                      **
**                                                                                                                    **
**  Created on  : 2024/01/09                                                                                          **
**  Author      : lizhi.huang                                                                                         **
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
 *  V1.0.0    20240109   lizhi.huang   Initial version
 *  V1.0.1    20240111   lizhi.huang   Resolve QAC
 *  V1.0.2    20240131   lizhi.huang   Resolve malloc memory crossover
 *  V1.0.3    20240304   lizhi.huang   Resolve QAC
 *  V1.0.4    20240429   qinmei.chen   add T320 support by increasing HeadSize
 *  V1.0.5    20240529   lizhi.huang   Resolve memory crossover
 *  V1.0.6    20240620   lizhi.huang   Supports 64 bit high-end environments
 *
 */

/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:Ilib<br>
  RuleSorce:puhua-rule.rcf 2.3.1

    \li PRQA S 0750 MISRA Rule 19.2 <br>
    Reason:Determine that union must be used.

    \li PRQA S 0306 MISRA Rule 11.4 <br>
    Reason:Determine that the pointer address must have a type conversion

    \li PRQA S 3472,3469 MISRA Dir-4.9 <br>
    Reason: Based on operational performance, it has been verified by unit tests

    \li PRQA S 3673 MISRA Rule-8.13 <br>
    Reason: The tool has a false positive that the parameter cannot be converted to const

 */

/*=================================================[inclusions]=======================================================*/

#include "istd_lib.h"
#include <stddef.h>

/*======================================================================================================================
 *                                                      Version
======================================================================================================================*/

/*======================================================================================================================
 *                                                  Version check
======================================================================================================================*/

/*======================================================================================================================
 *                                                  Private Macro Definitions
======================================================================================================================*/

/* SLI definition */
#define MAX_LOG2_SLI (uint32)(3u)
#define MAX_SLI      (1u << MAX_LOG2_SLI)

/* block align address */
#define BLOCK_ALIGN          (size_type)(MAX_SLI - 1u)
#define ROUNDUP_SIZE(size)   (size_type)(((size) + BLOCK_ALIGN) & ~BLOCK_ALIGN)
#define ROUNDDOWN_SIZE(size) (size_type)((size) & ~BLOCK_ALIGN)

/* minimum block size  */
#define SMALL_BLOCK (size_type)(32u)

/* FLI definition */
#define MAX_FLI           (size_type)(16u)
#define FLI_OFFSET        (size_type)(4u)
#define REAL_FLI          (MAX_FLI - FLI_OFFSET)

#define MEMHEAP_INIT_FLAG 0x2A59FA59u

/* bit of the block state */
#define BLOCK_STATE (size_type)(0x1u)
#define PREV_STATE  (size_type)(0x2u)

/* bit 0 of the block size */
#define FREE_BLOCK (size_type)(0x1u)
#define USED_BLOCK (size_type)(0x0u)

/* bit 1 of the block size */
#define PREV_FREE (size_type)(0x2u)
#define PREV_USED (size_type)(0x0u)

/* the minimum applicable size  */
#define MIN_APPLICABLE_SIZE ROUNDUP_SIZE((sizeof(FreePtrType)))
#define BLOCK_HEAD_SIZE     (size_type)(sizeof(BlockType) - sizeof(FreePtrType))
#define BLOCK_SIZE_ALIGN    ((~(size_type)0u) - (3u))
/* PRQA S 3472++ */ /* MISRA Dir-4.9 */
/* get the next block addr */
#define GET_BLOCK(addr, offset) ((BlockType*)((size_t)(addr) + (size_t)(offset)))

/* block offset address and physical address conversion */
#define GET_BLOCK_OFFSET(block, manager) \
    (((block) == NULL_PTR) ? (size_type)0u : (size_type)((size_t)(block) - (size_t)(manager)))
#define GET_BLOCK_ADDR(offset, manager) \
    ((BlockType*)(((size_t)(offset) == 0u) ? 0u : ((size_t)(manager) + (size_t)(offset))))

/* block link  */
#define LINK_PREV_PHY_BLOCK(curBlock, preBlock, manager)  ((curBlock)->head.prev = GET_BLOCK_OFFSET(preBlock, manager))
#define LINK_PREV_FREE_BLOCK(curBlock, preBlock, manager) ((curBlock)->ptr.free = GET_BLOCK_OFFSET(preBlock, manager))

/* bit operation  */
#define SET_BIT_8(val, bit)    ((val) = ((val) | ((uint8)1u << (bit))))
#define SET_BIT_16(val, bit)   ((val) = ((val) | ((uint16)1u << (bit))))
#define CLEAR_BIT_8(val, bit)  ((val) = ((val) & (~((uint8)1u << (bit)))))
#define CLEAR_BIT_16(val, bit) ((val) = ((val) & (~((uint16)1u << (bit)))))
/* PRQA S 3472-- */ /* MISRA Dir-4.9 */

/*======================================================================================================================
 *                                                  Private Data Types
======================================================================================================================*/
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
} FreePtrType;

/* the block head */
typedef struct
{
    size_type prev;
    size_type size;
} BlockHeadType;

/* the block struct */
typedef struct
{
    BlockHeadType head;
    union { /* PRQA S 0750 */ /* MISRA Rule 19.2 */
        FreePtrType free;
        uint8 buffer[1];
    } ptr;
} BlockType;

/* the manager block struct */
typedef struct
{
    uint32 initFlag;
    size_type manageSize;
    uint16 fli;
    uint8 sli[REAL_FLI];
    size_type freeLinker[REAL_FLI][MAX_SLI];

#if MEMHEAP_STATISTIC == STD_ON
    size_type realSize;
    size_type usedSize;
    size_type maxMallocSize;
#endif

} HeapManager;

/*======================================================================================================================
 *                                                  Private Constant Definitions
======================================================================================================================*/
#define ILIB_START_SEC_CONST
#include "iLib_MemMap.h"
static const uint32 MemHeap_Table[] = {
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
#define ILIB_STOP_SEC_CONST
#include "iLib_MemMap.h"

/*======================================================================================================================
 *                                                  Private Function Declarations
======================================================================================================================*/

/*======================================================================================================================
 *                                                  Extern Function Definitions
======================================================================================================================*/

#define ILIB_START_SEC_CODE
#include "iLib_MemMap.h"

/**
 * @ingroup ilib
 * @brief isoft MemHeap_LSBIT
 * @param[in] i
 * @return the lowest bit of i.
 */
static uint32 MemHeap_LSBIT(uint32 i)
{
    uint32 a;
    uint32 x = i & ((size_type)0u - i);

    a = (x <= 0xFFFFu) ? ((x <= 0xFFu) ? 0u : 8u) : ((x <= 0xFFFFFFu) ? 16u : 24u);
    a = a + MemHeap_Table[x >> a];
    return a;
}

/**
 * @ingroup ilib
 * @brief isoft MemHeap_MSBIT
 * @param[in] i
 * @return the highest bit of i.
 */
static uint32 MemHeap_MSBIT(uint32 i)
{
    uint32 a;
    uint32 x = i;

    a = (x <= 0xFFFFu) ? ((x <= 0xFFu) ? 0u : 8u) : ((x <= 0xFFFFFFu) ? 16u : 24u);
    a = a + MemHeap_Table[x >> a];
    return a;
}

/**
 * @ingroup ilib
 * @brief isoft MemHeap_MatchNextIndex
 * @param[inout] size request mem size
 * @param[out] fli fli calculated based on size
 * @param[out] sli the next level sli calculated based on size
 */
static void MemHeap_MatchNextIndex(uint32* size, uint32* fli, uint32* sli)
{
    uint32 slBlockSize;

    if (*size < SMALL_BLOCK)
    {
        *fli = 0u;
        *sli = (*size / (SMALL_BLOCK / MAX_SLI));
    }
    else
    {
        /* calculate the fine-grained of blocks that can accommodate size */
        slBlockSize = ((uint32)1u << (MemHeap_MSBIT(*size) - MAX_LOG2_SLI)) - 1u;

        /* the size needs to be obtained from the next sl greater than the current sl
           to avoid lookup */
        *size = *size + slBlockSize;

        /* Size greater than SMALL_BLOCK requires calculation of fli and sli */
        *fli = MemHeap_MSBIT(*size);
        *sli = (*size >> (*fli - MAX_LOG2_SLI)) - MAX_SLI;
        *fli -= FLI_OFFSET;

        /* take slBlockSize from size */
        *size &= ~(size_type)slBlockSize;
    }
}

/**
 * @ingroup ilib
 * @brief isoft MemHeap_MatchIndex
 * @param[in] size request mem size
 * @param[out] fli fli calculated based on size
 * @param[out] sli sli calculated based on size
 */
static void MemHeap_MatchIndex(size_type size, uint32* fli, uint32* sli)
{
    if (size < SMALL_BLOCK)
    {
        /* size smaller than SMALL_BLOCK Block,
           then directly search for sli from the position where fli=0,
           with a stride of 8 bytes for each sli
        */
        *fli = 0;
        *sli = ((uint32)size / (SMALL_BLOCK / MAX_SLI));
    }
    else
    {
        *fli = MemHeap_MSBIT(size);
        /* find sli from the fli level. the scope of SLI:[0, MAX_SLI - 1] */
        *sli = ((uint32)size >> (*fli - MAX_LOG2_SLI)) - MAX_SLI;
        /* convert from logical fli to physical fli */
        /* fli 0:[0, SMALL_BLOCK-1] 1:[SMALL_BLOCK, SMALL_BLOCK*2-1] */
        *fli -= FLI_OFFSET;
    }
}

/**
 * @ingroup ilib
 * @brief isoft MemHeap_FindFirstFreeBlock
 * @param[in] manager manager block
 * @param[inout] fli minimum available fli in manager to meet requirements
 * @param[inout] sli minimum available sli in manager to meet requirements
 */
/* PRQA S 0306++ */ /* MISRA Rule 11.4 */
static BlockType* MemHeap_FindFirstFreeBlock(const HeapManager* manager, uint32* fli, uint32* sli)
{
    uint32 tmp_sli = ((uint32)manager->sli[*fli] & ((~(uint32)0u) << *sli));
    BlockType* block = NULL_PTR;

    if ((tmp_sli > 0u) && (tmp_sli < ((uint32)1u << MAX_SLI)))
    {
        *sli = MemHeap_LSBIT(tmp_sli);
        block = GET_BLOCK_ADDR(manager->freeLinker[*fli][*sli], manager); /* PRQA S 3469 */ /* MISRA Dir-4.9 */
    }
    else
    {
        /* no sli that meets the condition, get the next non-0 fl of the current fl  */
        *fli = MemHeap_LSBIT((uint32)manager->fli & (~(uint32)0u << (*fli + 1u)));
        if ((*fli > 0u) && (*fli < ((uint32)REAL_FLI)))
        {
            *sli = MemHeap_LSBIT((uint32)manager->sli[*fli]);
            block = GET_BLOCK_ADDR(manager->freeLinker[*fli][*sli], manager); /* PRQA S 3469 */ /* MISRA Dir-4.9 */
        }
        else
        {
            *fli = 0u;
        }
    }
    return block;
}
/* PRQA S 0306-- */ /* MISRA Rule 11.4 */

/**
 * @ingroup ilib
 * @brief isoft MemHeap_ExtractBlockDirect
 * @param[inout] manager manager block
 * @param[inout] block memory block
 * @param[in] fli
 * @param[in] sli
 */
/* PRQA S 0306++ */ /* MISRA Rule 11.4 */
static void MemHeap_ExtractBlockDirect(HeapManager* manager, BlockType* block, uint32 fli, uint32 sli)
{
    /* this block must be the first one for the link list */
    manager->freeLinker[fli][sli] = block->ptr.free.next;
    if (0u != manager->freeLinker[fli][sli])
    {
        /* PRQA S 3469++ */ /* MISRA Dir-4.9 */
        GET_BLOCK_ADDR(manager->freeLinker[fli][sli], manager)->ptr.free.prev = 0u;
        /* PRQA S 3469-- */ /* MISRA Dir-4.9 */
    }
    else
    {
        /* there are no free blocks after this block */
        CLEAR_BIT_8(manager->sli[fli], sli); /* PRQA S 3472,3473 */ /* MISRA Dir-4.9 */
        if (0u == manager->sli[fli])
        {
            CLEAR_BIT_16(manager->fli, fli); /* PRQA S 3472,3473 */ /* MISRA Dir-4.9 */
        }
    }
    block->ptr.free.prev = 0u;
    block->ptr.free.next = 0u;
}
/* PRQA S 0306-- */ /* MISRA Rule 11.4 */

/**
 * @ingroup ilib
 * @brief isoft MemHeap_ExtractBlock
 * @param[inout] manager manager block
 * @param[inout] block memory block
 */
/* PRQA S 0306++ */ /* MISRA Rule 11.4 */
static void MemHeap_ExtractBlock(HeapManager* manager, BlockType* block)
{
    BlockType* tempBlock;
    uint32 fli;
    uint32 sli;

    MemHeap_MatchIndex(block->head.size & (size_type)BLOCK_SIZE_ALIGN, &fli, &sli);

    if (0u != block->ptr.free.next)
    {
        tempBlock = GET_BLOCK_ADDR(block->ptr.free.next, manager); /* PRQA S 3469 */ /* MISRA Dir-4.9 */
        tempBlock->ptr.free.prev = block->ptr.free.prev;
    }

    if (0u != block->ptr.free.prev)
    {
        tempBlock = GET_BLOCK_ADDR(block->ptr.free.prev, manager); /* PRQA S 3469 */ /* MISRA Dir-4.9 */
        tempBlock->ptr.free.next = block->ptr.free.next;
    }

    /* the block is head of free block link list */
    if (manager->freeLinker[fli][sli] == GET_BLOCK_OFFSET(block, manager)) /* PRQA S 3469 */ /* MISRA Dir-4.9 */
    {
        manager->freeLinker[fli][sli] = block->ptr.free.next;

        if (0u == manager->freeLinker[fli][sli])
        {
            /* there are no free block in sl */
            CLEAR_BIT_8(manager->sli[fli], sli); /* PRQA S 3472,3473 */ /* MISRA Dir-4.9 */
            if (0u == manager->sli[fli])
            {
                /* there are no non-zero sl in the fl */
                CLEAR_BIT_16(manager->fli, fli); /* PRQA S 3472,3473 */ /* MISRA Dir-4.9 */
            }
        }
    }

    block->ptr.free.prev = 0u;
    block->ptr.free.next = 0u;
}
/* PRQA S 0306-- */ /* MISRA Rule 11.4 */

/**
 * @ingroup ilib
 * @brief isoft MemHeap_InsertBlock
 * @param[inout] manager manager block
 * @param[inout] block memory block
 */
/* PRQA S 0306++ */ /* MISRA Rule 11.4 */
static void MemHeap_InsertBlock(HeapManager* manager, BlockType* block)
{
    uint32 fli = 0u;
    uint32 sli = 0u;

    /* Calculate the corresponding fli and sli based on the block size */
    MemHeap_MatchIndex(block->head.size & (size_type)BLOCK_SIZE_ALIGN, &fli, &sli);

    block->ptr.free.prev = 0u;
    block->ptr.free.next = manager->freeLinker[fli][sli];

    if (0u != manager->freeLinker[fli][sli])
    {
        /* freeLinker is not empty, point the prev of the first node
           of freeLinker to the block node */
        /* PRQA S 3469++ */ /* MISRA Dir-4.9 */
        GET_BLOCK_ADDR(manager->freeLinker[fli][sli], manager)->ptr.free.prev = GET_BLOCK_OFFSET(block, manager);
        /* PRQA S 3469-- */ /* MISRA Dir-4.9 */
    }

    /* point freeLinker to block as freeLinker's first node */
    manager->freeLinker[fli][sli] = GET_BLOCK_OFFSET(block, manager); /* PRQA S 3469 */ /* MISRA Dir-4.9 */

    SET_BIT_8(manager->sli[fli], sli); /* PRQA S 3472,3473 */ /* MISRA Dir-4.9 */
    SET_BIT_16(manager->fli, fli); /* PRQA S 3472,3473 */     /* MISRA Dir-4.9 */
}
/* PRQA S 0306-- */ /* MISRA Rule 11.4 */

/**
 * @ingroup ilib
 * @brief isoft MemHeap_SplitBlock
 * @param[in] manager manager block
 * @param[in] block memory block
 * @param[in] size request size
 */
/* PRQA S 0306++ */ /* MISRA Rule 11.4 */
static BlockType* MemHeap_SplitBlock(const HeapManager* manager, const BlockType* block, uint32 size)
{
    BlockType* splitBlock = (BlockType*)NULL_PTR;
    uint16 remainSize = (block->head.size & (size_type)BLOCK_SIZE_ALIGN) - (size_type)size;

    if (remainSize >= sizeof(BlockType))
    {
        remainSize = remainSize - BLOCK_HEAD_SIZE;

        /* unused space forms a new free block */
        splitBlock = GET_BLOCK(block->ptr.buffer, size); /* PRQA S 3469 */ /* MISRA Dir-4.9 */
        splitBlock->head.size = remainSize | FREE_BLOCK | PREV_USED;

        /* the prev pointer of the next node in the block
           points to the splitBlock node after splitting */
        /* PRQA S 3469++ */ /* MISRA Dir-4.9 */
        BlockType* nextBlock = GET_BLOCK(block->ptr.buffer, (size_t)block->head.size & BLOCK_SIZE_ALIGN);
        /* PRQA S 3469-- */ /* MISRA Dir-4.9 */

        LINK_PREV_PHY_BLOCK(splitBlock, block, manager); /* PRQA S 3469 */     /* MISRA Dir-4.9 */
        LINK_PREV_PHY_BLOCK(nextBlock, splitBlock, manager); /* PRQA S 3469 */ /* MISRA Dir-4.9 */
    }

    return splitBlock;
}
/* PRQA S 0306-- */ /* MISRA Rule 11.4 */

/**
 * @ingroup ilib
 * @brief isoft ILib_MemHeapInit
 * @param[inout] ram the whole of memory space
 * @param[in] size the size of ram
 * @return error code
 */
/* PRQA S 0306++ */ /* MISRA Rule 11.4 */
uint8 ILib_MemHeapInit(void* ram, uint32 size)
{
    StatusType ret = MEMHEAP_OK;
    HeapManager* manager = (HeapManager*)ram;

    /* illegal pointer */
    if (NULL_PTR == manager)
    {
        ret = MEMHEAP_INVALID_PTR;
    }

    /* address not byte aligned */
    else if (0u != ((size_t)manager & (sizeof(void*) - 1u)))
    {
        ret = MEMHEAP_INVALID_ALIGN;
    }

    /* unable to manage space larger than 64K */
    else if ((uint64)size > (uint64)((uint16)(-1)))
    {
        ret = MEMHEAP_INVALID_SIZE;
    }

    /* size too small for initialization */
    else if (size < (sizeof(HeapManager) + (sizeof(BlockType) * 2u)))
    {
        ret = MEMHEAP_INVALID_SIZE;
    }

    /* repeated initialization */
    else if (MEMHEAP_INIT_FLAG == manager->initFlag)
    {
        ret = MEMHEAP_INVALID_INIT;
    }

    else
    {
        (void)ILib_memset(manager, 0, sizeof(HeapManager));

        manager->initFlag = MEMHEAP_INIT_FLAG;
        manager->manageSize = (size_type)size;

        /* system block : always used */
        /* PRQA S 3469++ */ /* MISRA Dir-4.9 */
        BlockType* firstBlock = GET_BLOCK(manager, ROUNDUP_SIZE(sizeof(HeapManager)));
        /* PRQA S 3469-- */ /* MISRA Dir-4.9 */
        firstBlock->head.prev = 0u;
        firstBlock->head.size = 0u | USED_BLOCK | PREV_USED;

        /* user block */
        BlockType* block = GET_BLOCK(firstBlock, ROUNDUP_SIZE(BLOCK_HEAD_SIZE)); /* PRQA S 3469 */ /* MISRA Dir-4.9 */
        LINK_PREV_PHY_BLOCK(block, firstBlock, manager); /* PRQA S 3469 */                         /* MISRA Dir-4.9 */
        block->ptr.free.prev = 0u;
        block->ptr.free.next = 0u;
        block->head.size =
            ROUNDDOWN_SIZE((
                size_type)(((size_type)size - ROUNDUP_SIZE(sizeof(HeapManager)) - ((size_type)3u * (BLOCK_HEAD_SIZE)))))
            | USED_BLOCK | PREV_USED;

        /* system block : always used */
        /* PRQA S 3469++ */ /* MISRA Dir-4.9 */
        BlockType* endBlock = GET_BLOCK(block->ptr.buffer, block->head.size & BLOCK_SIZE_ALIGN);
        /* PRQA S 3469-- */                                              /* MISRA Dir-4.9 */
        LINK_PREV_PHY_BLOCK(endBlock, block, manager); /* PRQA S 3469 */ /* MISRA Dir-4.9 */
        endBlock->head.size = 0u | USED_BLOCK | PREV_FREE;

        manager->manageSize = (size_type)size;

#if MEMHEAP_STATISTIC == STD_ON
        manager->realSize = block->head.size;
        manager->usedSize = block->head.size;
        manager->maxMallocSize = 0;
#endif
        /* release user block */
        ret = ILib_MemHeapFree(ram, block->ptr.buffer);
    }

    return ret;
}
/* PRQA S 0306-- */ /* MISRA Rule 11.4 */

/**
 * @ingroup ilib
 * @brief isoft ILib_MemHeapMalloc
 * @param[inout] ram the whole of memory space
 * @param[in] size request size
 * @return memory address
 */
/* PRQA S 0306++ */ /* MISRA Rule 11.4 */
void* ILib_MemHeapMalloc(void* ram, uint32 size)
{
    StatusType ret = MEMHEAP_OK;
    HeapManager* manager;
    void* ptr = NULL_PTR;

    manager = (HeapManager*)ram;

    /* illegal pointer */
    if (NULL_PTR == manager)
    {
        ret = MEMHEAP_INVALID_PTR;
    }

    /* address not byte aligned */
    else if (0u != ((size_t)manager & (size_t)(sizeof(void*) - (size_t)1u)))
    {
        ret = MEMHEAP_INVALID_ALIGN;
    }

    /* uninitialized */
    else if (MEMHEAP_INIT_FLAG != manager->initFlag)
    {
        ret = MEMHEAP_INVALID_INIT;
    }

    /* unable to manage space larger than 64K */
    else if ((uint64)size > (uint64)((uint16)(-1)))
    {
        ret = MEMHEAP_INVALID_SIZE;
    }

    /* size is zero */
    else if (0u == size)
    {
        ret = MEMHEAP_INVALID_SIZE;
    }

    else
    {
        uint32 fli;
        uint32 sli;

        /* The size must be greater than or equal to MIN_APPLICABLE_SIZE and byte alignment */
        uint32 local_size = (size < MIN_APPLICABLE_SIZE) ? MIN_APPLICABLE_SIZE : ROUNDUP_SIZE(size);

        MemHeap_MatchNextIndex(&local_size, &fli, &sli);
        BlockType* block = MemHeap_FindFirstFreeBlock(manager, &fli, &sli);

        if (NULL_PTR != block)
        {

#if MEMHEAP_STATISTIC == STD_ON
            manager->usedSize = manager->usedSize + (size_type)(local_size & BLOCK_SIZE_ALIGN);

            if (manager->maxMallocSize < local_size)
            {
                manager->maxMallocSize = (size_type)local_size;
            }
#endif

            MemHeap_ExtractBlockDirect(manager, block, fli, sli);

            /* PRQA S 3469++ */ /* MISRA Dir-4.9 */
            BlockType* nextBlock = GET_BLOCK(block->ptr.buffer, (size_t)block->head.size & BLOCK_SIZE_ALIGN);
            /* PRQA S 3469-- */ /* MISRA Dir-4.9 */

            /* unused space forms a new free block */
            BlockType* splitBlock = MemHeap_SplitBlock(manager, block, local_size);

            if (NULL_PTR != splitBlock)
            {
                /* put unused blocks back into the free link list */
                MemHeap_InsertBlock(manager, splitBlock);

                /* modify the block size, but use old state of the block */
                block->head.size = ((size_type)local_size | (block->head.size & (size_type)PREV_STATE)) & (~FREE_BLOCK);
            }
            else
            {
                /* modify the block size, but use old state of the block */
                block->head.size &= (~FREE_BLOCK);

                nextBlock->head.size &= (~PREV_STATE);
            }

            ptr = (void*)block->ptr.buffer;

            if (!(((size_t)block >= ((size_t)manager + (size_t)sizeof(HeapManager)))
                  && (((size_t)block) < ((size_t)manager + manager->manageSize - (size_t)sizeof(BlockHeadType)))
                  && (((size_t)block + block->head.size)
                      >= ((size_t)manager + (size_t)sizeof(HeapManager) + (size_t)sizeof(BlockHeadType)))
                  && (((size_t)block + block->head.size)
                      < ((size_t)manager + manager->manageSize - (size_t)sizeof(BlockHeadType)))))
            {
                ret = MEMHEAP_INVALID_PTR;
                ptr = NULL_PTR;
            }
        }
    }

    (void)ret;
    return ptr;
}
/* PRQA S 0306-- */ /* MISRA Rule 11.4 */

/**
 * @ingroup ilib
 * @brief isoft ILib_MemHeapCalloc
 * @param[inout] ram the whole of memory space
 * @param[in] count request count
 * @param[in] size request size
 * @return memory address
 */
void* ILib_MemHeapCalloc(void* ram, uint32 count, uint32 size)
{
    void* ptr = ILib_MemHeapMalloc(ram, count * size);
    if (NULL_PTR != ptr)
    {
        (void)ILib_memset(ptr, 0, count * size);
    }
    return ptr;
}

/**
 * @ingroup ilib
 * @brief isoft ILib_MemHeapFree
 * @param[inout] ram the whole of memory space
 * @param[in] ptr memory pointer to be released
 * @return error code
 */
/* PRQA S 0306++ */                                            /* MISRA Rule 11.4 */
uint8 ILib_MemHeapFree(void* ram, void* ptr) /* PRQA S 3673 */ /* MISRA Rule-8.13 */
{
    StatusType ret = MEMHEAP_OK;
    HeapManager* manager = (HeapManager*)ram;
    BlockType* block = (BlockType*)((size_t)ptr - BLOCK_HEAD_SIZE); /* PRQA S 0326 */ /* MISRA Rule 11.6 */

    /* illegal pointer */
    if (NULL_PTR == manager)
    {
        ret = MEMHEAP_INVALID_PTR;
    }

    /* address not byte aligned */
    else if (0u != ((size_t)manager & (sizeof(void*) - 1u)))
    {
        ret = MEMHEAP_INVALID_ALIGN;
    }

    /* uninitialized */
    else if (MEMHEAP_INIT_FLAG != manager->initFlag)
    {
        ret = MEMHEAP_INVALID_INIT;
    }

    /* illegal pointer */
    else if (ptr == NULL_PTR)
    {
        ret = MEMHEAP_INVALID_PTR;
    }

    /* pointer not in valid range */
    else if (!(((size_t)block >= ((size_t)manager + (size_t)sizeof(HeapManager)))
               && (((size_t)block) < ((size_t)manager + manager->manageSize - (size_t)sizeof(BlockHeadType)))
               && (((size_t)block + block->head.size)
                   >= ((size_t)manager + (size_t)sizeof(HeapManager) + (size_t)sizeof(BlockHeadType)))
               && (((size_t)block + block->head.size)
                   < ((size_t)manager + manager->manageSize - (size_t)sizeof(BlockHeadType)))))
    {
        ret = MEMHEAP_INVALID_PTR;
    }

    else
    {

#if MEMHEAP_STATISTIC == STD_ON
        manager->usedSize = manager->usedSize - (block->head.size & (size_type)BLOCK_SIZE_ALIGN);
#endif

        block->head.size |= FREE_BLOCK;

        block->ptr.free.prev = 0u;
        block->ptr.free.next = 0u;

        /* PRQA S 3469++ */ /* MISRA Dir-4.9 */
        BlockType* tmp_block = GET_BLOCK(block->ptr.buffer, block->head.size & BLOCK_SIZE_ALIGN);
        /* PRQA S 3469-- */ /* MISRA Dir-4.9 */

        /* the next block of the current block is free */
        if (0u != (tmp_block->head.size & BLOCK_STATE))
        {
            MemHeap_ExtractBlock(manager, tmp_block);

            /* merge the current size with the size of the next block */
            block->head.size += (tmp_block->head.size & (size_type)BLOCK_SIZE_ALIGN) + BLOCK_HEAD_SIZE;
        }

        /* the previous block of the current block is free */
        if (0u != (block->head.size & PREV_STATE))
        {
            tmp_block = GET_BLOCK_ADDR(block->head.prev, manager); /* PRQA S 3469 */ /* MISRA Dir-4.9 */
            MemHeap_ExtractBlock(manager, tmp_block);
            tmp_block->head.size += (block->head.size & (size_type)BLOCK_SIZE_ALIGN) + BLOCK_HEAD_SIZE;
            block = tmp_block;
        }

        /* put the merged block back into the free link list */
        MemHeap_InsertBlock(manager, block);
        /* PRQA S 3469++ */ /* MISRA Dir-4.9 */
        tmp_block = GET_BLOCK(block->ptr.buffer, block->head.size & BLOCK_SIZE_ALIGN);
        /* PRQA S 3469-- */ /* MISRA Dir-4.9 */

        /* change the 'prev' state of the next block to free */
        tmp_block->head.size |= PREV_FREE;
        tmp_block->head.prev = GET_BLOCK_OFFSET(block, manager); /* PRQA S 3469 */ /* MISRA Dir-4.9 */
    }
    return ret;
}
/* PRQA S 0306-- */ /* MISRA Rule 11.4 */

#if MEMHEAP_STATISTIC == STD_ON
/**
 * @ingroup ilib
 * @brief isoft ILib_MemHeapGetManageSize
 * @param[in] ram the whole of memory space
 * @param[out] size maximum size of ram
 * @return error code
 */
/* PRQA S 0306++ */ /* MISRA Rule 11.4 */
uint8 ILib_MemHeapGetManageSize(const void* ram, uint32* size)
{
    StatusType ret = MEMHEAP_OK;
    const HeapManager* manager = (const HeapManager*)ram;

    /* illegal pointer */
    if (NULL_PTR == manager)
    {
        ret = MEMHEAP_INVALID_PTR;
    }

    /* address not byte aligned */
    else if (0u != ((size_t)manager & (sizeof(void*) - 1u)))
    {
        ret = MEMHEAP_INVALID_ALIGN;
    }

    /* uninitialized */
    else if (MEMHEAP_INIT_FLAG != manager->initFlag)
    {
        ret = MEMHEAP_INVALID_INIT;
    }

    /* illegal pointer */
    else if (NULL_PTR == size)
    {
        ret = MEMHEAP_INVALID_PTR;
    }

    else
    {
        *size = (uint32)manager->manageSize;
    }

    return ret;
}
/* PRQA S 0306-- */ /* MISRA Rule 11.4 */

/**
 * @ingroup ilib
 * @brief isoft ILib_MemHeapGetRealMaxSize
 * @param[in] ram the whole of memory space
 * @param[out] size actual maximum available size(remove management block information size)
 * @return error code
 */
/* PRQA S 0306++ */ /* MISRA Rule 11.4 */
uint8 ILib_MemHeapGetRealMaxSize(const void* ram, uint32* size)
{
    StatusType ret = MEMHEAP_OK;
    const HeapManager* manager = (const HeapManager*)ram;

    /* illegal pointer */
    if (NULL_PTR == manager)
    {
        ret = MEMHEAP_INVALID_PTR;
    }

    /* address not byte aligned */
    else if (0u != ((size_t)manager & (sizeof(void*) - 1u)))
    {
        ret = MEMHEAP_INVALID_ALIGN;
    }

    /* uninitialized */
    else if (MEMHEAP_INIT_FLAG != manager->initFlag)
    {
        ret = MEMHEAP_INVALID_INIT;
    }

    /* illegal pointer */
    else if (NULL_PTR == size)
    {
        ret = MEMHEAP_INVALID_PTR;
    }

    else
    {
        *size = (uint32)manager->realSize;
    }

    return ret;
}
/* PRQA S 0306-- */ /* MISRA Rule 11.4 */

/**
 * @ingroup ilib
 * @brief isoft ILib_MemHeapGetUsedSize
 * @param[in] ram the whole of memory space
 * @param[out] size the currently used size
 * @return error code
 */
/* PRQA S 0306++ */ /* MISRA Rule 11.4 */
uint8 ILib_MemHeapGetUsedSize(const void* ram, uint32* size)
{
    StatusType ret = MEMHEAP_OK;
    const HeapManager* manager = (const HeapManager*)ram;

    /* illegal pointer */
    if (NULL_PTR == manager)
    {
        ret = MEMHEAP_INVALID_PTR;
    }

    /* address not byte aligned */
    else if (0u != ((size_t)manager & (sizeof(void*) - 1u)))
    {
        ret = MEMHEAP_INVALID_ALIGN;
    }

    /* uninitialized */
    else if (MEMHEAP_INIT_FLAG != manager->initFlag)
    {
        ret = MEMHEAP_INVALID_INIT;
    }

    /* illegal pointer */
    else if (NULL_PTR == size)
    {
        ret = MEMHEAP_INVALID_PTR;
    }

    else
    {
        *size = (uint32)manager->usedSize;
    }

    return ret;
}
/* PRQA S 0306-- */ /* MISRA Rule 11.4 */

/**
 * @ingroup ilib
 * @brief isoft ILib_MemHeapGetCurMaxBlockSize
 * @param[in] ram the whole of memory space
 * @param[out] size the current free maximum size
 * @return error code
 */
/* PRQA S 0306++ */ /* MISRA Rule 11.4 */
uint8 ILib_MemHeapGetCurFreeMaxBlockSize(const void* ram, uint32* size)
{
    StatusType ret = MEMHEAP_OK;
    const HeapManager* manager = (const HeapManager*)ram;

    /* illegal pointer */
    if (NULL_PTR == manager)
    {
        ret = MEMHEAP_INVALID_PTR;
    }

    /* address not byte aligned */
    else if (0u != ((size_t)manager & (sizeof(void*) - 1u)))
    {
        ret = MEMHEAP_INVALID_ALIGN;
    }

    /* uninitialized */
    else if (MEMHEAP_INIT_FLAG != manager->initFlag)
    {
        ret = MEMHEAP_INVALID_INIT;
    }

    /* illegal pointer */
    else if (NULL_PTR == size)
    {
        ret = MEMHEAP_INVALID_PTR;
    }

    else
    {
        uint32 tempSize = 0u;
        uint32 maxSize = 0u;
        if (manager->fli > 0u)
        {
            uint32 fli = MemHeap_MSBIT((uint32)manager->fli);
            uint32 sli = MemHeap_MSBIT((uint32)manager->sli[fli]);

            if ((sli < ((uint32)1u << MAX_SLI)) && (fli < ((uint32)1u << REAL_FLI)))
            {
                /* PRQA S 3469++ */ /* MISRA Dir-4.9 */
                const BlockType* block = GET_BLOCK_ADDR(manager->freeLinker[fli][sli], manager);
                /* PRQA S 3469-- */ /* MISRA Dir-4.9 */

                while (block != NULL_PTR)
                {
                    tempSize = (fli * SMALL_BLOCK) + (((fli * SMALL_BLOCK) >> MAX_LOG2_SLI) * sli);
                    if (maxSize < tempSize)
                    {
                        maxSize = tempSize;
                    }
                    /* PRQA S 3469++ */ /* MISRA Dir-4.9 */
                    block = GET_BLOCK_ADDR(block->ptr.free.next, manager);
                    /* PRQA S 3469-- */ /* MISRA Dir-4.9 */
                }

                maxSize = ROUNDDOWN_SIZE(maxSize);
            }
        }
        *size = (size_type)maxSize;
    }

    return ret;
}
/* PRQA S 0306-- */ /* MISRA Rule 11.4 */

/**
 * @ingroup ilib
 * @brief isoft ILib_MemHeapGetMaxMallocSize
 * @param[in] ram the whole of memory space
 * @param[out] size maximum size applied for by malloc
 * @return error code
 */
/* PRQA S 0306++ */ /* MISRA Rule 11.4 */
uint8 ILib_MemHeapGetMaxMallocSize(const void* ram, uint32* size)
{
    StatusType ret = MEMHEAP_OK;
    const HeapManager* manager = (const HeapManager*)ram;

    /* illegal pointer */
    if (NULL_PTR == manager)
    {
        ret = MEMHEAP_INVALID_PTR;
    }

    /* address not byte aligned */
    else if (0u != ((size_t)manager & (sizeof(void*) - 1u)))
    {
        ret = MEMHEAP_INVALID_ALIGN;
    }

    /* uninitialized */
    else if (MEMHEAP_INIT_FLAG != manager->initFlag)
    {
        ret = MEMHEAP_INVALID_INIT;
    }

    /* illegal pointer */
    else if (NULL_PTR == size)
    {
        ret = MEMHEAP_INVALID_PTR;
    }

    else
    {
        *size = (uint32)manager->maxMallocSize;
    }

    return ret;
}
/* PRQA S 0306-- */ /* MISRA Rule 11.4 */

#endif

#define ILIB_STOP_SEC_CODE
#include "iLib_MemMap.h"
