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
 **  @file               : Os_ReadyQue.c
 **  @author             : i-soft-os
 **  @date               : 2025/02/10
 **  @vendor             : isoft
 **  @description        : Os source file for ReadyQue API implementations
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Os_Arch_Processor.h"
#include "Os_ReadyQue.h"
#include "Os_Task.h"
#include "Os_Kernel.h"

/* ===================================================== macros ===================================================== */
/* PRQA S 3410, 3472 ++ */ /* VL_Os_3410, VL_Os_3472 */
#define OS_MAP_BIT_MASK(MAP, PRIO) \
    do { \
        const Os_PriorityType local_prio = (Os_PriorityType)(PRIO); \
        Os_PriorityType index = local_prio / OS_SIXTEEN; \
        ((Os_PriorityType *)(MAP))[index] |= (Os_PriorityType)(((Os_PriorityType)1u) << (Os_PriorityType)(local_prio % OS_SIXTEEN)); \
    } while (0)
/* PRQA S 3410, 3472 -- */

/* PRQA S 3410, 3472 ++ */ /* VL_Os_3410, VL_Os_3472 */
#define OS_MAP_BIT_DISMASK(MAP, PRIO) \
    do { \
        const Os_PriorityType local_prio = (Os_PriorityType)(PRIO); \
        Os_PriorityType index = local_prio / OS_SIXTEEN; \
        ((Os_PriorityType *)(MAP))[index] &= (Os_PriorityType)(~(((Os_PriorityType)1u) << (Os_PriorityType)(local_prio % OS_SIXTEEN))); \
    } while (0)
/* PRQA S 3410, 3472 -- */

/* ================================================ type definitions ================================================ */

/* ============================================ external data definitions =========================================== */

/* ============================================ internal data definitions =========================================== */

/* ========================================== internal function declarations ======================================== */
/**
 * @brief           Determines the highest priority bit position in a priority value
 * @param[in]       highPriReadyTaskInQueue: Priority bitmap to analyze
 * @return          Os_PriorityType
 * @retval          Index of the highest set bit in the priority value
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL Os_PriorityType Os_GetHighPrioBit(Os_PriorityType highPriReadyTaskInQueue);

/* ========================================== external function definitions ========================================= */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * get the highest priority
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Os_PriorityType Os_GetHighPrio(const Os_ReadyQueueManageType *queueMg)
/* PRQA S 1532 -- */
{
    Os_PriorityType highPrio;

    if (READY_MAP_SIZE(queueMg->PriorityMax) > 1u) /* PRQA S 3432, 1840, 1891 */ /* VL_Os_3432, VL_Os_1840, VL_Os_1891 */
    {
        Os_PriorityType const *ptr = NULL_PTR;
        Os_PriorityType priorityTemp;

        /* PRQA S 3432, 1840, 1891 ++ */ /* VL_Os_3432, VL_Os_1840, VL_Os_1891 */
        ptr = &queueMg->Map[READY_MAP_SIZE(queueMg->PriorityMax) - 1u];
        priorityTemp = *ptr;
        highPrio = ((Os_PriorityType)READY_MAP_SIZE(queueMg->PriorityMax) - 1u) * OS_NUM_PRIORITYBITS_PERWORD;
        /* PRQA S 3432, 1840, 1891 -- */
        while (0u == priorityTemp)
        {
            ptr--;

            if (ptr < &queueMg->Map[0]) /* PRQA S 0490 */ /* VL_Os_0490 */
            {
                while (1) /* PRQA S 2740 */ /* VL_Os_2740 */
                {
                    /* Nothing to do. */
                }
            }

            priorityTemp = *ptr;
            /*here have a potential bug in the first version,here must be
             * decrease but not increase*/
            highPrio -= OS_NUM_PRIORITYBITS_PERWORD;
        }

        highPrio += Os_GetHighPrioBit(priorityTemp);
    }
    else
    {
        if (NULL_PTR == queueMg->Map)
        {
            while (1) /* PRQA S 2740 */ /* VL_Os_2740 */
            {
                /* Nothing to do. */
            }
        }

        highPrio = Os_GetHighPrioBit(queueMg->Map[0]);
    }

    return highPrio;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * insert a priority to ready Queue
 */
void Os_ReadyQueueInsert(const Os_ReadyQueueManageType *queueMg,
                         Os_TaskType object,
                         Os_CallLevelType level,
                         Os_PriorityType prio)
{
    switch (level)
    {
#if (CFG_STD_RESOURCE_MAX > 0U)
    case OS_LEVEL_STANDARD_RESOURCE:
/*  make the prio related with the calling task*/
#if ((OS_BCC1 == CFG_CC) || (OS_ECC1 == CFG_CC))
        queueMg->Queue[prio][0] = object;
#else
        if (queueMg->Mark[prio].QueueHead == 0u)
        {
            queueMg->Mark[prio].QueueHead = queueMg->QueueSize[prio] - 1u;
        }
        else
        {
            queueMg->Mark[prio].QueueHead--;
        }
#endif

        queueMg->Queue[prio][queueMg->Mark[prio].QueueHead] = object;
        OS_MAP_BIT_MASK(queueMg->Map, prio); /* PRQA S 2743 */ /* VL_Os_2743 */
        break;
#endif

    case OS_LEVEL_TASK:
#if ((OS_BCC1 == CFG_CC) || (OS_ECC1 == CFG_CC))
        queueMg->Queue[prio][0] = object;
#else
        queueMg->Queue[prio][queueMg->Mark[prio].QueueTail] = object;
        queueMg->Mark[prio].QueueTail++;
        if (queueMg->Mark[prio].QueueTail == queueMg->QueueSize[prio])
        {
            queueMg->Mark[prio].QueueTail = 0u;
        }
#endif

        OS_MAP_BIT_MASK(queueMg->Map, prio); /* PRQA S 2743 */ /* VL_Os_2743 */
        break;

#if (CFG_INTERNAL_RESOURCE_MAX > 0)
    case OS_LEVEL_INTERNAL_RESOURCE:
/*  make the prio related with the calling task*/
#if ((OS_BCC1 == CFG_CC) || (OS_ECC1 == CFG_CC))
        queueMg->Queue[prio][0] = object;
#else
        if (queueMg->Mark[prio].QueueHead == 0u)
        {
            queueMg->Mark[prio].QueueHead = queueMg->QueueSize[prio] - 1u;
        }
        else
        {
            queueMg->Mark[prio].QueueHead--;
        }
        queueMg->Queue[prio][queueMg->Mark[prio].QueueHead] = object;
#endif

        OS_MAP_BIT_MASK(queueMg->Map, prio); /* PRQA S 2743 */ /* VL_Os_2743 */
        break;
#endif

    default:
        Os_Panic();
        break;
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * remove a priority from ready Queue
 */
void Os_ReadyQueueRemove(const Os_ReadyQueueManageType *queueMg, Os_PriorityType prio)
{
/*make the prio related with the calling task */
#if ((OS_BCC1 == CFG_CC) || (OS_ECC1 == CFG_CC))
    OS_MAP_BIT_DISMASK(queueMg->Map, prio); /* PRQA S 2743, 4397 */ /* VL_Os_2743, VL_Os_4397 */
    queueMg->Queue[prio][0] = OS_TASK_INVALID; /* PRQA S 4342 */ /* VL_Os_4342 */
#else
    queueMg->Queue[prio][queueMg->Mark[prio].QueueHead] = OS_TASK_INVALID; /* PRQA S 4342 */ /* VL_Os_4342 */
    queueMg->Mark[prio].QueueHead++;

    if (queueMg->Mark[prio].QueueHead == queueMg->QueueSize[prio])
    {
        queueMg->Mark[prio].QueueHead = 0u;
    }

    if (queueMg->Mark[prio].QueueHead == queueMg->Mark[prio].QueueTail)
    {
        OS_MAP_BIT_DISMASK(queueMg->Map, prio); /* PRQA S 2743, 4397 */ /* VL_Os_2743, VL_Os_4397 */
    }
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * get the first task of a ready Queue that the value of
 *                      priority is equal to prio
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Os_TaskType Os_ReadyQueueGetFirst(const Os_ReadyQueueManageType *queueMg, Os_PriorityType prio)
/* PRQA S 1532 -- */
{
    Os_TaskType firstTask;

    if (prio >= queueMg->PriorityMax)
    {
        firstTask = OS_TASK_INVALID; /* PRQA S 4342 */ /* VL_Os_4342 */
    }
    else
    {
#if ((OS_BCC1 == CFG_CC) || (OS_ECC1 == CFG_CC))
        firstTask = queueMg->Queue[prio][0];
#else
        firstTask = queueMg->Queue[prio][queueMg->Mark[prio].QueueHead];
#endif
    }

    return firstTask;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * get the highest priority bit
 */
OS_LOCAL Os_PriorityType Os_GetHighPrioBit(
    Os_PriorityType highPriReadyTaskInQueue)
{
    /* Index into table is bit pattern to resolve highest priority. */
    /* DD_1_0149 */
    /* PRQA S 1290, 3132, 3223 ++ */ /* VL_Os_1290, VL_QAC_MagicNum, VL_Os_3223 */
    OS_LOCAL const uint8 mapTable[OS_SIXTEEN] = {0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3};
    /* PRQA S 1290, 3132, 3223 -- */
    Os_PriorityType highPrio = 0u;

/*means that the value of Priority is proportional to Priority */
#if (OS_NUM_PRIORITYBITS_PERWORD >= 32)
    if ((highPriReadyTaskInQueue & OS_HIGH_16_BITS_MASK) != 0u)
    {
        highPrio += OS_SIXTEEN;
        highPriReadyTaskInQueue >>= OS_SIXTEEN; /* PRQA S 1338 */ /* VL_Os_1338 */
    }
#endif

#if (OS_NUM_PRIORITYBITS_PERWORD >= OS_SIXTEEN)
    if ((highPriReadyTaskInQueue & OS_HIGH_8_BITS_MASK) != 0u)
    {
        highPrio += OS_EIGHT;
        highPriReadyTaskInQueue >>= OS_EIGHT; /* PRQA S 1338 */ /* VL_Os_1338 */
    }
#endif

    if ((highPriReadyTaskInQueue & OS_HIGH_4_BITS_MASK) != 0u)
    {
        highPrio += OS_FOUR;
        highPriReadyTaskInQueue >>= OS_FOUR; /* PRQA S 1338 */ /* VL_Os_1338 */
    }

    return (uint16)(highPrio + mapTable[highPriReadyTaskInQueue]);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * init the ready Queue or ready table
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_InitReadyTable(void)
/* PRQA S 1532 -- */
{
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    const Os_ReadyQueueManageType *queueMg = pScb->QueueMg;
    /* PRQA S 3678 -- */
    /* PRQA S 3432, 1840, 1891 ++ */ /* VL_Os_3432, VL_Os_1840, VL_Os_1891 */
    for (uint16 i = 0U; i < READY_MAP_SIZE(queueMg->PriorityMax); i++)
    /* PRQA S 3432, 1840, 1891 -- */
    {
        queueMg->Map[i] = 0U;
    }

    for (uint16 i = 0U; i < queueMg->PriorityMax; i++)
    {
#if ((OS_BCC1 == CFG_CC) || (OS_ECC1 == CFG_CC))
        queueMg->Queue[i][0] = OS_TASK_INVALID;
#else

        queueMg->Mark[i].QueueHead = 0U;
        queueMg->Mark[i].QueueTail = 0U;

        for (uint32 j = 0U; j < queueMg->QueueSize[i]; j++)
        {
            queueMg->Queue[i][j] = OS_TASK_INVALID; /* PRQA S 4342 */ /* VL_Os_4342 */
        }
#endif
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Clear priority ready Map.
 */
/* PRQA S 1505 ++ */ /* VL_Os_1505 */
void Os_ClearPrioReadyMap(const Os_ReadyQueueManageType *queueMg, Os_PriorityType prio)
/* PRQA S 1505 -- */
{
    OS_MAP_BIT_DISMASK(queueMg->Map, prio); /* PRQA S 2743, 4397 */ /* VL_Os_2743, VL_Os_4397 */
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Removing task_node from ready_queue in TerminateApplication.
 */
/* PRQA S 1532, 2755, 1503 ++ */ /* VL_QAC_OneFunRef, VL_Os_2755, VL_QAC_NoUsedApi */
void Os_ApplReadyQueueRemove(
    const Os_ReadyQueueManageType *queueMg,
    Os_TaskType taskId,
    Os_PriorityType queuePriority)
/* PRQA S 1532, 2755, 1503 -- */
{
    Os_TaskType *pOsReadyQueue = queueMg->Queue[queuePriority];
    Os_ReadyQueueType *pOsReadyQueueMark = &queueMg->Mark[queuePriority];

    /* Remove one node from Os_ReadyQueue and shift to the left. */
    uint32 queueHead = pOsReadyQueueMark->QueueHead;
    uint32 queueTail = pOsReadyQueueMark->QueueTail;
    uint32 queueSize = queueMg->QueueSize[queuePriority];

    /*Retrieve the previous position of the Queue's tail.*/
    uint32 OsQueueTailLast = (queueTail + (queueSize - 1U)) % queueSize; /* PRQA S 2834 */ /* VL_Os_2834 */

    /* Queue is empty. */
    if ((queueHead == queueTail) && (OS_TASK_INVALID == pOsReadyQueue[OsQueueTailLast])) /* PRQA S 4342 */ /* VL_Os_4342 */
    {
        /*nothing to do*/
    }
    else
    {
        uint32 queueFront = queueHead;
        /* PRQA S 2834 ++ */ /* VL_Os_2834 */
        uint32 loopWidth = (queueHead == queueTail) ? queueSize : ((queueTail + queueSize - queueHead) % queueSize);
        /* PRQA S 2834 -- */
        uint32 hitCount = 0U;

        do
        {
            /*If the Queue element is equal to taskId, increment hitCount.*/
            /* PRQA S 2834 ++ */ /* VL_Os_2834 */
            while ((taskId == pOsReadyQueue[(queueFront + hitCount) % queueSize]) && (hitCount < loopWidth))
            /* PRQA S 2834 -- */
            {
                hitCount++;
                queueTail = (queueTail + (queueSize - 1U)) % queueSize; /* PRQA S 2834 */ /* VL_Os_2834 */
            }

            /*If, after moving queueTail, queueFront becomes equal to queueTail, exit the loop.*/
            if ((queueFront == queueTail) && (hitCount > 0U))
            {
                break;
            }

            if (hitCount > 0U)
            {
                /* PRQA S 2834 ++ */ /* VL_Os_2834 */
                pOsReadyQueue[queueFront] = pOsReadyQueue[(queueFront + hitCount) % queueSize];
                /* PRQA S 2834 -- */
            }

            queueFront = (queueFront + 1u) % queueSize; /* PRQA S 2834 */ /* VL_Os_2834 */

        } while (queueFront != queueTail);

        /*Set the deleted QueueTail as invalid.*/
        while (0U != hitCount) /* PRQA S 4342 */ /* VL_Os_4342 */
        {
            /* PRQA S 4342, 2834 ++ */ /* VL_Os_4342, VL_Os_2834 */
            pOsReadyQueue[(queueTail + hitCount) % queueSize] = OS_TASK_INVALID;
            /* PRQA S 4342, 2834 -- */
            hitCount = hitCount - 1u;
        }
        /*Update the QueueTail.*/
        pOsReadyQueueMark->QueueTail = queueTail;

        /* Queue for this prio is empty. */
        if (pOsReadyQueueMark->QueueHead == pOsReadyQueueMark->QueueTail)
        {
            /* Clear prio_has_task bit flag of Os_ReadyMap. */
            Os_ClearPrioReadyMap(queueMg, queuePriority);
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/*=======[E N D   O F   F I L E]==============================================*/
