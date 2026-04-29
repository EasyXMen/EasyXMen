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
 **  @file               : Os_Tprot.c
 **  @author             : i-soft-os
 **  @date               : 2025/02/10
 **  @vendor             : isoft
 **  @description        : Os source file for Tprot API implementations
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Os_Arch_Processor.h"
#include "Os_Tprot.h"
#include "Os_Task.h"
#include "Os_Resource.h"
#include "Os_Interrupt.h"
#include "Os_ProtectHook.h"
#include "Os_Kernel.h"

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ============================================ external data definitions =========================================== */

/* ============================================ internal data definitions =========================================== */
#define OS_START_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "Os_MemMap.h"
OS_LOCAL Os_TpMonitorType Os_TpMonitor[CFG_CORE_MAX];
#define OS_STOP_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "Os_MemMap.h"

/* ========================================== internal function declarations ======================================== */
/**
 * @brief           Inserts a timing protection node into a list
 * @param[inout]    HeadAddr: Pointer to the list head pointer
 * @param[inout]    pNode: Pointer to the node to be inserted
 * @return          boolean
 * @retval          TRUE: The compare register needs to be set
 * @retval          FALSE: No need to update the compare register
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL boolean Os_TpNodeInsert(Os_TpNodeType **HeadAddr, Os_TpNodeType *pNode);

/**
 * @brief           Deletes a timing protection node from a list
 * @param[inout]    HeadAddr: Pointer to the list head pointer
 * @param[inout]    pNode: Pointer to the node to be deleted
 * @return          boolean
 * @retval          TRUE: The compare register needs to be set
 * @retval          FALSE: No need to update the compare register
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL boolean Os_TpNodeDelete(Os_TpNodeType **HeadAddr, Os_TpNodeType *pNode);

/**
 * @brief           Sets the hardware compare register for timing protection
 * @param[in]       coreId: ID of the core
 * @param[in]       Increment: Time increment for the compare register
 * @param[in]       MonitoredType: Type of monitored entity (task or ISR)
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_TpSetCompare(Os_CoreIdType coreId, Os_Hal_TpType Increment, Os_TpMonitoredType MonitoredType);

/**
 * @brief           Updates timestamp for task timing protection
 * @param[in]       coreId: ID of the core
 * @param[inout]    pTcb: Pointer to the Task Control Block
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_TpTaskUpsetTimeStamp(Os_CoreIdType coreId, Os_TCBType* pTcb);

/**
 * @brief           Inserts a timing protection node for a task
 * @param[in]       coreId: ID of the core
 * @param[inout]    pTcb: Pointer to the Task Control Block
 * @param[inout]    pTpNode: Pointer to the timing protection node
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_TmProtTaskNodeInsert(Os_CoreIdType coreId, Os_TCBType* pTcb, Os_TpNodeType* pTpNode);

/**
 * @brief           Deletes a timing protection node for a task
 * @param[in]       coreId: ID of the core
 * @param[inout]    pTcb: Pointer to the Task Control Block
 * @param[inout]    pTpNode: Pointer to the timing protection node
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_TmProtTaskNodeDelete(Os_CoreIdType coreId, Os_TCBType* pTcb, Os_TpNodeType* pTpNode);

/**
 * @brief           Updates timestamp for ISR timing protection
 * @param[in]       coreId: ID of the core
 * @param[inout]    pIcb: Pointer to the ISR Control Block
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_TpIsrUpsetTimeStamp(Os_CoreIdType coreId, Os_ICBType* pIcb);

/**
 * @brief           Inserts a timing protection node for an ISR
 * @param[in]       coreId: ID of the core
 * @param[inout]    pIcb: Pointer to the ISR Control Block
 * @param[inout]    pTpNode: Pointer to the timing protection node
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_TmProtIsrNodeInsert(Os_CoreIdType coreId, Os_ICBType* pIcb, Os_TpNodeType* pTpNode);

/**
 * @brief           Deletes a timing protection node for an ISR
 * @param[in]       coreId: ID of the core
 * @param[inout]    pIcb: Pointer to the ISR Control Block
 * @param[inout]    pTpNode: Pointer to the timing protection node
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_TmProtIsrNodeDelete(Os_CoreIdType coreId, Os_ICBType* pIcb, Os_TpNodeType* pTpNode);

#if (CFG_STD_RESOURCE_MAX > 0)
/**
 * @brief           Initializes resource lock budgets for an ISR
 * @param[in]       isrId: ID of the ISR
 * @param[in]       pIsrTpCfg: Pointer to the ISR timing protection configuration
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_TmProtInitIsrResLock(Os_IsrType isrId, const Os_TmProtCfgType* pIsrTpCfg);
#endif

/**
 * @brief           Initializes timing protection for an ISR
 * @param[in]       isrId: ID of the ISR
 * @param[in]       pIsrTpCfg: Pointer to the ISR timing protection configuration
 * @param[inout]    pIcb: Pointer to the ISR Control Block
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_InitIsrTmProt(Os_IsrType isrId, const Os_TmProtCfgType* pIsrTpCfg, Os_ICBType* pIcb);

#if (CFG_STD_RESOURCE_MAX > 0)
/**
 * @brief           Initializes resource lock budgets for a task
 * @param[in]       osTaskId: ID of the task
 * @param[in]       pTaskTpCfg: Pointer to the task timing protection configuration
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_TmProtInitTaskResLock(Os_TaskType osTaskId, const Os_TmProtCfgType* pTaskTpCfg);
#endif

/**
 * @brief           Initializes timing protection for a task
 * @param[in]       osTaskId: ID of the task
 * @param[in]       pTaskTpCfg: Pointer to the task timing protection configuration
 * @param[inout]    pTcb: Pointer to the Task Control Block
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_InitTaskTmProt(Os_TaskType osTaskId, const Os_TmProtCfgType* pTaskTpCfg, Os_TCBType* pTcb);

/**
 * @brief           Calls the protection hook for timing protection violations
 * @param[in]       pScb: Pointer to the System Control Block
 * @param[in]       Head: Pointer to the timing protection node
 * @param[in]       osWhoHook: Indicator for task or ISR context
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_TpCallProtectHook(Os_SCBType *pScb, const Os_TpNodeType *Head, uint32 osWhoHook);

/**
 * @brief           Calls the protection hook for task timing protection violations
 * @param[in]       pScb: Pointer to the System Control Block
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_TpTaskCallProtectHook(Os_SCBType *pScb);

/**
 * @brief           Calls the protection hook for ISR timing protection violations
 * @param[in]       pScb: Pointer to the System Control Block
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_TpIsrCallProtectHook(Os_SCBType *pScb);

/* ========================================== external function definitions ========================================= */
/**
 * Insert TpNode to the list
 */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
OS_LOCAL boolean Os_TpNodeInsert(Os_TpNodeType **HeadAddr, Os_TpNodeType *pNode)
{
    boolean NeedSetCMP = FALSE;
    Os_TpNodeType *Head = *HeadAddr;

    if(NULL_PTR == Head)
    {
        pNode->Next = NULL_PTR;
        *HeadAddr = pNode;
        NeedSetCMP = TRUE;
    }
    else if(Head->TpEndTime > pNode->TpEndTime)
    {
        pNode->Next = Head;
        Head->Pre = pNode;
        *HeadAddr = pNode;
        NeedSetCMP = TRUE;
    }
    else
    {
        Os_TpNodeType *Current = Head;
        while ((NULL_PTR != Current->Next) && (Current->Next->TpEndTime <= pNode->TpEndTime))
        {
            Current = Current->Next;
        }
        if(NULL_PTR != Current->Next)
        {
            Current->Next->Pre = pNode;
            pNode->Next = Current->Next;
        }
        Current->Next = pNode;
        pNode->Pre = Current;
    }
    pNode->TpNodeInList = TRUE;

    return NeedSetCMP;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Delete TpNode from the list
 */
OS_LOCAL boolean Os_TpNodeDelete(Os_TpNodeType **HeadAddr, Os_TpNodeType *pNode)
{
    boolean NeedSetCMP = FALSE;
    Os_TpNodeType *Head = *HeadAddr;

    if (Head == pNode)
    {
        Head = Head->Next;
        *HeadAddr = Head;
        NeedSetCMP = TRUE;
    }
    else if (NULL_PTR == pNode->Next)
    {
        pNode->Pre->Next = NULL_PTR;
    }
    else
    {
        pNode->Pre->Next = pNode->Next;
        pNode->Next->Pre = pNode->Pre;
    }
    
    pNode->Next = NULL_PTR;
    pNode->Pre = NULL_PTR;
    pNode->TpEndTime = 0U;
    pNode->TpNodeInList = FALSE;

    return NeedSetCMP;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * TimeProtect set CMP reg
 */
OS_LOCAL void Os_TpSetCompare(Os_CoreIdType coreId, Os_Hal_TpType Increment, Os_TpMonitoredType MonitoredType)
{
    Os_TpMonitor[coreId].Os_TpMonitored = MonitoredType;
    Os_Hal_TpSetCompare(coreId, Increment);
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Task update timestamp
 */
OS_LOCAL void Os_TpTaskUpsetTimeStamp(Os_CoreIdType coreId, Os_TCBType* pTcb)
{
    Os_TpMonitor[coreId].Os_TpTimeStamp += Os_Hal_TpGetTimePass(coreId);
    uint64 TimePass = Os_TpMonitor[coreId].Os_TpTimeStamp - pTcb->taskTpLastReadTime;
    pTcb->taskTpLastReadTime = Os_TpMonitor[coreId].Os_TpTimeStamp;
    pTcb->taskTpTimeStamp += TimePass;
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Timing protection upset task timestap, then insert it to Head list
 */
OS_LOCAL void Os_TmProtTaskNodeInsert(Os_CoreIdType coreId, Os_TCBType* pTcb, Os_TpNodeType* pTpNode)
{
    OS_HAL_DECLARE_CRITICAL();
    OS_HAL_ENTRY_CRITICAL();

    if(TP_READY == pTcb->taskTpStatus)
    {
        pTcb->taskTpStatus = TP_WORKING;
        Os_TpMonitor[coreId].Os_TpTimeStamp += Os_Hal_TpGetTimePass(coreId);
        pTcb->taskTpTimeStamp = 0U;
        pTcb->taskTpLastReadTime = Os_TpMonitor[coreId].Os_TpTimeStamp;
    }
    else
    {
        Os_TpTaskUpsetTimeStamp(coreId, pTcb);
    }

    pTpNode->TpEndTime = pTcb->taskTpTimeStamp + pTpNode->osTpBudget;
    if(TRUE == Os_TpNodeInsert(&(pTcb->taskTpHead), pTpNode))
    {
        Os_TpSetCompare(coreId, pTpNode->osTpBudget, TP_FOR_TASK);
    }
    OS_HAL_EXIT_CRITICAL();
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Timing protection delete task node from head list.
 */
OS_LOCAL void Os_TmProtTaskNodeDelete(Os_CoreIdType coreId, Os_TCBType* pTcb, Os_TpNodeType* pTpNode)
{
    OS_HAL_DECLARE_CRITICAL();
    OS_HAL_ENTRY_CRITICAL();

    if(TRUE == Os_TpNodeDelete(&(pTcb->taskTpHead), pTpNode))
    {
        if(NULL_PTR == pTcb->taskTpHead)
        {
            pTcb->taskTpStatus = TP_READY;
            Os_TpSetCompare(coreId, OS_HAL_TP_MAX_INCREMENT_VALUE, TP_FOR_NONE);
        }
        else
        {
            Os_TpTaskUpsetTimeStamp(coreId, pTcb);
            uint32 CompareVale = (uint32)(pTcb->taskTpHead->TpEndTime - pTcb->taskTpTimeStamp);
            Os_TpSetCompare(coreId, CompareVale, TP_FOR_TASK);
        }
    }
    OS_HAL_EXIT_CRITICAL();
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * ISR2 update timestamp
 */
OS_LOCAL void Os_TpIsrUpsetTimeStamp(Os_CoreIdType coreId, Os_ICBType* pIcb)
{
    Os_TpMonitor[coreId].Os_TpTimeStamp += Os_Hal_TpGetTimePass(coreId);
    uint64 TimePass = Os_TpMonitor[coreId].Os_TpTimeStamp - pIcb->isrTpLastReadTime;
    pIcb->isrTpLastReadTime = Os_TpMonitor[coreId].Os_TpTimeStamp;
    pIcb->isrTpTimeStamp += TimePass;
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Timing protection upset isr timestap, then insert it to head list.
 */
OS_LOCAL void Os_TmProtIsrNodeInsert(Os_CoreIdType coreId, Os_ICBType* pIcb, Os_TpNodeType* pTpNode)
{
    OS_HAL_DECLARE_CRITICAL();
    OS_HAL_ENTRY_CRITICAL();

    if(TP_READY == pIcb->isrTpStatus)
    {
        pIcb->isrTpStatus = TP_WORKING;
        Os_TpMonitor[coreId].Os_TpTimeStamp += Os_Hal_TpGetTimePass(coreId);
        pIcb->isrTpTimeStamp = 0U;
        pIcb->isrTpLastReadTime = Os_TpMonitor[coreId].Os_TpTimeStamp;
    }
    else
    {
        Os_TpIsrUpsetTimeStamp(coreId, pIcb);
    }

    pTpNode->TpEndTime = pIcb->isrTpTimeStamp + pTpNode->osTpBudget;
    if(TRUE == Os_TpNodeInsert(&(pIcb->isrTpHead), pTpNode))
    {
        Os_TpSetCompare(coreId, pTpNode->osTpBudget, TP_FOR_ISR);
    }
    OS_HAL_EXIT_CRITICAL();
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Timing protection delete Isr node from head list.
 */
OS_LOCAL void Os_TmProtIsrNodeDelete(Os_CoreIdType coreId, Os_ICBType* pIcb, Os_TpNodeType* pTpNode)
{
    OS_HAL_DECLARE_CRITICAL();
    OS_HAL_ENTRY_CRITICAL();

    if(TRUE == Os_TpNodeDelete(&(pIcb->isrTpHead), pTpNode))
    {
        if(NULL_PTR == pIcb->isrTpHead)
        {
            pIcb->isrTpStatus = TP_READY;
            Os_TpSetCompare(coreId, OS_HAL_TP_MAX_INCREMENT_VALUE, TP_FOR_NONE);
        }
        else
        {
            Os_TpIsrUpsetTimeStamp(coreId, pIcb);
            uint32 CompareVale = (uint32)(pIcb->isrTpHead->TpEndTime - pIcb->isrTpTimeStamp);
            Os_TpSetCompare(coreId, CompareVale, TP_FOR_ISR);
        }
    }
    OS_HAL_EXIT_CRITICAL();
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Time protect suspend current monitored thread
 */
void Os_TpSupend(const Os_SCBType *pScb, Os_TpMonitoredType MonitoredType)
{
    if (MonitoredType < TP_FOR_NONE)
    {
        Os_CoreIdType coreId = pScb->SysCore;
        if(TP_FOR_TASK == MonitoredType)
        {
            Os_TCBType* pTcb = pScb->SysRunningTCB;
            if(TP_WORKING == pTcb->taskTpStatus)
            {
                pTcb->taskTpStatus = TP_SUSPEND;
                Os_TpTaskUpsetTimeStamp(coreId, pTcb);
                Os_TpSetCompare(coreId, OS_HAL_TP_MAX_INCREMENT_VALUE, TP_FOR_NONE);
            }
        }
        else
        {
            Os_ICBType* pIcb = Os_ICB[pScb->SysRunningIsrCat2Id];
            if(TP_WORKING == pIcb->isrTpStatus)
            {
                pIcb->isrTpStatus = TP_SUSPEND;
                Os_TpIsrUpsetTimeStamp(coreId, pIcb);
                Os_TpSetCompare(coreId, OS_HAL_TP_MAX_INCREMENT_VALUE, TP_FOR_NONE);
            }
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Time protect resume monitored node
 */
void Os_TpResume(const Os_SCBType *pScb, Os_TpMonitoredType MonitoredType)
{
    if (MonitoredType < TP_FOR_NONE)
    {
        uint32 RemainTime;
        Os_CoreIdType coreId = pScb->SysCore;
        if(TP_FOR_TASK == MonitoredType)
        {
            Os_TCBType* pTcb = pScb->SysRunningTCB;
            if(TP_SUSPEND == pTcb->taskTpStatus)
            {
                pTcb->taskTpStatus = TP_WORKING;
                Os_TpMonitor[coreId].Os_TpTimeStamp += Os_Hal_TpGetTimePass(coreId);
                pTcb->taskTpLastReadTime = Os_TpMonitor[coreId].Os_TpTimeStamp;
                RemainTime = (uint32)(pTcb->taskTpHead->TpEndTime - pTcb->taskTpTimeStamp);
                Os_TpSetCompare(coreId, RemainTime, TP_FOR_TASK);
            }
        }
        else
        {
            Os_ICBType* pIcb = Os_ICB[pScb->SysRunningIsrCat2Id];
            if(TP_SUSPEND == pIcb->isrTpStatus)
            {
                pIcb->isrTpStatus = TP_WORKING;
                Os_TpMonitor[coreId].Os_TpTimeStamp += Os_Hal_TpGetTimePass(coreId);
                pIcb->isrTpLastReadTime = Os_TpMonitor[coreId].Os_TpTimeStamp;
                RemainTime = (uint32)(pIcb->isrTpHead->TpEndTime - pIcb->isrTpTimeStamp);
                Os_TpSetCompare(coreId, RemainTime, TP_FOR_ISR);
            }
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (CFG_STD_RESOURCE_MAX > 0)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Start process for budget of resource locking
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_TmProtResStart(const Os_SCBType *pScb, ResourceType resId, Os_TpMonitoredType MonitoredType)
/* PRQA S 1532 -- */
{
    if (MonitoredType < TP_FOR_NONE)
    {
        uint32 osBudget;
        Os_TpNodeType* pTpNode;
        Os_RCBType* pRcb = Os_RCB[resId];

        /* Get budget according to MonitoredType. */
        if (TP_FOR_TASK == MonitoredType)      /* By task. */
        {
            Os_TCBType* pTcb = pScb->SysRunningTCB;
            if(TP_NO_INIT != pTcb->taskTpStatus)
            {
                osBudget = pRcb->TmProtResBgtTask[pScb->SysRunningTaskId];
                if (OS_TICK_INVALID != osBudget) /* PRQA S 1258 */ /* VL_Os_1258 */
                {
                    pTpNode = &(pRcb->osResTpNode);
                    pTpNode->osTpBudget = osBudget;
                    Os_TmProtTaskNodeInsert(pScb->SysCore, pTcb, pTpNode);
                    /* Which task occupy this resource. */
                    pRcb->WhichTaskOccupy = pScb->SysRunningTaskId;
                }
            }
        }
        else /*TP_FOR_ISR == MonitoredType*/
        {

            Os_ICBType* pIcb = Os_ICB[pScb->SysRunningIsrCat2Id];
            if(TP_NO_INIT != pIcb->isrTpStatus)
            {
                osBudget = pRcb->TmProtResBgtIsr[pScb->SysRunningIsrCat2Id];
                if (OS_TICK_INVALID != osBudget) /* PRQA S 1258 */ /* VL_Os_1258 */
                {
                    pTpNode = &(pRcb->osResTpNode);
                    pTpNode->osTpBudget = osBudget;
                    Os_TmProtIsrNodeInsert(pScb->SysCore, pIcb, pTpNode);
                    /* Which isr occupy this resource. */
                    pRcb->WhichIsrOccupy = pScb->SysRunningIsrCat2Id;
                }
            }
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * End process for budget of resource locking
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_TmProtResEnd(Os_CoreIdType coreId, ResourceType resId)
/* PRQA S 1532 -- */
{
    Os_RCBType* pRcb = Os_RCB[resId];
    Os_TpNodeType* pTpNode = &(pRcb->osResTpNode);

    /* Stop timing protection for this resource. */
    if (TRUE == pTpNode->TpNodeInList)
    {
        if(OS_TASK_INVALID != pRcb->WhichTaskOccupy) /* PRQA S 4342 */ /* VL_Os_4342 */
        {
            Os_TCBType* pTcb = Os_TCB[pRcb->WhichTaskOccupy];
            Os_TmProtTaskNodeDelete(coreId, pTcb, pTpNode);
            pRcb->WhichTaskOccupy = OS_TASK_INVALID; /* PRQA S 4342 */ /* VL_Os_4342 */
        }
        else
        {
            Os_ICBType* pIcb = Os_ICB[pRcb->WhichIsrOccupy];
            Os_TmProtIsrNodeDelete(coreId, pIcb, pTpNode);
            pRcb->WhichIsrOccupy  = INVALID_ISR; /* PRQA S 1297*/ /* VL_Os_1297*/
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Time frame of task: frame check process
 */
/* PRQA S 6030 ++ */ /* VL_MTR_Os_STMIF */
StatusType Os_TmProtTaskFrameChk(Os_TaskType taskId)
/* PRQA S 6030 -- */
{
    StatusType osRet = E_OK;
    Os_TCBType *pTcb = Os_TCB[taskId];

    if(TP_NO_INIT != pTcb->taskTpStatus)
    {
        uint32 TpFrameBudget = pTcb->taskTpNode[TP_ARRIVAL].osTpBudget;

        /* Timing frame for this task is not configed. */
        if(OS_TICK_INVALID != TpFrameBudget) /* PRQA S 1258 */ /* VL_Os_1258 */
        {
            Os_CoreIdType coreId = OS_TASK_GET_COREID(taskId);
            Os_TpMonitor[coreId].Os_TpTimeStamp += Os_Hal_TpGetTimePass(coreId);

            if(pTcb->taskTpEerlistArrival > Os_TpMonitor[coreId].Os_TpTimeStamp)
            {
#if ((OS_SC4 == CFG_SC) && (CFG_TRUSTED_SYSTEM_SERVICE_MAX > 0U))
                Os_SCBType *pScb = Os_GetSystemContext(coreId);
                Os_TrustedFuncTpDataDef *trustedFuncTp = &pScb->Os_TrustedFuncTp;
                if (TRUE == trustedFuncTp->TrustedFuncTportDelayCall)
                {
                    /* Time protection and delay processing are triggered in the trusted function */
                    trustedFuncTp->TrustedFuncTporFlag = TRUE;
                    trustedFuncTp->TrustedFuncTporErrType = E_OS_PROTECTION_ARRIVAL;
                    trustedFuncTp->TrustedFuncWhoHook = OS_TMPROT_HOOK_TASK;
                }
                else
#endif
                {
                    /* Hook. */
                    if (PRO_IGNORE != Os_CallProtectionHook(E_OS_PROTECTION_ARRIVAL, OS_TMPROT_HOOK_TASK)) /* PRQA S 1520 */ /* VL_Os_1520 */
                    {
                        osRet = E_OS_PROTECTION_ARRIVAL;
                    }
                }
            }
            else
            {
                pTcb->taskTpEerlistArrival = Os_TpMonitor[coreId].Os_TpTimeStamp + TpFrameBudget;
            }
        }
    }

    return osRet;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Timing protection start process of task.
 */
void Os_TmProtTaskStart(Os_CoreIdType coreId, Os_TaskType taskId, Os_TmProtOptType osOptType)
{
    Os_TCBType* pTcb = Os_TCB[taskId];

    if(TP_NO_INIT != pTcb->taskTpStatus)
    {
        Os_TpNodeType* pTpNode = &(pTcb->taskTpNode[osOptType]);
        if(OS_TICK_INVALID != pTpNode->osTpBudget) /* PRQA S 1258 */ /* VL_Os_1258 */
        {
            Os_TmProtTaskNodeInsert(coreId, pTcb, pTpNode);
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Timing protection end process of task.
 */
void Os_TmProtTaskEnd(Os_CoreIdType coreId, Os_TaskType taskId, Os_TmProtOptType osOptType)
{
    Os_TCBType* pTcb = Os_TCB[taskId];

    if(TP_NO_INIT != pTcb->taskTpStatus)
    {
        Os_TpNodeType* pTpNode = &(pTcb->taskTpNode[osOptType]);
       if(TRUE == pTpNode->TpNodeInList)
        {
            Os_TmProtTaskNodeDelete(coreId, pTcb, pTpNode);
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Time frame of isrId: frame check process
 */
/* PRQA S 6030, 1532 ++ */ /* VL_MTR_Os_STMIF, VL_QAC_OneFunRef */
StatusType Os_TmProtIsrFrameChk(Os_IsrType isrId)
/* PRQA S 6030, 1532 -- */
{
    StatusType osRet = E_OK;

    /* Note: When Os_ArchSystemTimer isr arrive, Os_IntCfgIsrId is
     * not set to vaild value.
     * On tc1782, priority of Os_ArchSystemTimer isr is 255. */
    if (Os_CheckIsr2Id(isrId))
    {
        Os_ICBType *pIcb = Os_ICB[isrId];
        if(TP_NO_INIT != pIcb->isrTpStatus)
        {
            uint32 TpFrameBudget = pIcb->isrTpNode[TP_ARRIVAL].osTpBudget;

            if(OS_TICK_INVALID != TpFrameBudget) /* PRQA S 1258 */ /* VL_Os_1258 */
            {
                Os_CoreIdType coreId = OS_ISR_GET_COREID(isrId);
                Os_TpMonitor[coreId].Os_TpTimeStamp += Os_Hal_TpGetTimePass(coreId);
                if(pIcb->isrTpEerlistArrival > Os_TpMonitor[coreId].Os_TpTimeStamp)
                {
#if ((OS_SC4 == CFG_SC) && (CFG_TRUSTED_SYSTEM_SERVICE_MAX > 0U))
                    Os_SCBType *pScb = Os_GetSystemContext(coreId);
                    Os_TrustedFuncTpDataDef *trustedFuncTp = &pScb->Os_TrustedFuncTp;
                    if (TRUE == trustedFuncTp->TrustedFuncTportDelayCall)
                    {
                        /* Time protection and delay processing are triggered in the trusted function */
                        trustedFuncTp->TrustedFuncTporFlag = TRUE;
                        trustedFuncTp->TrustedFuncTporErrType = E_OS_PROTECTION_ARRIVAL;
                        trustedFuncTp->TrustedFuncWhoHook = OS_TMPROT_HOOK_ISR;
                    }
                    else
#endif
                    {
                        /* Hook. */
                        if (PRO_IGNORE != Os_CallProtectionHook(E_OS_PROTECTION_ARRIVAL, OS_TMPROT_HOOK_ISR))
                        {
                            osRet = E_OS_PROTECTION_ARRIVAL;
                        }
                    }
                }
                else
                {
                    pIcb->isrTpEerlistArrival = Os_TpMonitor[coreId].Os_TpTimeStamp + TpFrameBudget;
                }
            }
        }
    }

    return osRet;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Timing protection start process of isr.
 */
void Os_TmProtIsrStart(Os_CoreIdType coreId, Os_IsrType isrId, Os_TmProtOptType osOptType)
{
    Os_ICBType* pIcb = Os_ICB[isrId];

    if(TP_NO_INIT != pIcb->isrTpStatus)
    {
        Os_TpNodeType* pTpNode = &(pIcb->isrTpNode[osOptType]);
        if(OS_TICK_INVALID != pTpNode->osTpBudget) /* PRQA S 1258 */ /* VL_Os_1258 */
        {
            Os_TmProtIsrNodeInsert(coreId, pIcb, pTpNode);
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Timing protection end process of isr
 */
void Os_TmProtIsrEnd(Os_CoreIdType coreId, Os_IsrType isrId, Os_TmProtOptType osOptType)
{
    Os_ICBType* pIcb = Os_ICB[isrId];

    if(TP_NO_INIT != pIcb->isrTpStatus)
    {
        Os_TpNodeType* pTpNode = &(pIcb->isrTpNode[osOptType]);
        if(TRUE == pTpNode->TpNodeInList)
        {
            Os_TmProtIsrNodeDelete(coreId, pIcb, pTpNode);
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (CFG_STD_RESOURCE_MAX > 0)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Init resource lock budget for isr
 */
OS_LOCAL void Os_TmProtInitIsrResLock(Os_IsrType isrId, const Os_TmProtCfgType* pIsrTpCfg)
{
    /* If Isr cfg OsIsrResourceLock, read budget to RCB. */
    uint16 osResCnt = pIsrTpCfg->osResLockCnt;

    for (uint16 i = 0u; i < osResCnt; i++)
    {
        const Os_TpResLockType* pResLock = &(pIsrTpCfg->osResLockRef[i]);

        /* Read budget to RCB. */
        Os_RCB[pResLock->OsResLockResRef]->TmProtResBgtIsr[isrId] = pResLock->OsResLockBudget;
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Init timing protection for isr
 */
OS_LOCAL void Os_InitIsrTmProt
(
    Os_IsrType isrId,
    const Os_TmProtCfgType* pIsrTpCfg,
    Os_ICBType* pIcb
)
{
    pIcb->isrTpStatus = TP_READY;
    pIcb->isrTpHead = NULL_PTR;
    pIcb->isrTpTimeStamp = 0U;
    pIcb->isrTpLastReadTime = 0U;
    pIcb->isrTpEerlistArrival = 0U;

    /* Set budget of each type. */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_3138, VL_Os_3141 */
    Os_TmProtInitCbData(&pIcb->isrTpNode[TP_EXE], TP_EXE, pIsrTpCfg->osExecutionBudget);
    Os_TmProtInitCbData(&pIcb->isrTpNode[TP_ARRIVAL], TP_ARRIVAL, pIsrTpCfg->osTimeFrame);
    Os_TmProtInitCbData(&pIcb->isrTpNode[TP_SUS_OS_INT], TP_SUS_OS_INT,
                             pIsrTpCfg->osOsInterruptLockBudget);
    Os_TmProtInitCbData(&pIcb->isrTpNode[TP_SUS_ALL_INT], TP_SUS_ALL_INT,
                             pIsrTpCfg->osAllInterruptLockBudget);
    Os_TmProtInitCbData(&pIcb->isrTpNode[TP_DIS_ALL_INT], TP_DIS_ALL_INT,
                             pIsrTpCfg->osAllInterruptLockBudget);
    /* PRQA S 3138, 3141 -- */
/* Init resource lock budget for isr. */
#if (CFG_STD_RESOURCE_MAX > 0)
    if(NULL_PTR != pIsrTpCfg->osResLockRef)
    {
        Os_TmProtInitIsrResLock(isrId, pIsrTpCfg);
    }
#endif

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (CFG_STD_RESOURCE_MAX > 0)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Init resource lock budget for task
 */
OS_LOCAL void Os_TmProtInitTaskResLock(
    Os_TaskType osTaskId, 
    const Os_TmProtCfgType* pTaskTpCfg
)
{
    /* If task cfg OsTaskResourceLock, read budget to RCB. */
    uint16 osResCnt = pTaskTpCfg->osResLockCnt;

    for (uint16 i = 0u; i < osResCnt; i++)
    {
        const Os_TpResLockType* pResLock = &(pTaskTpCfg->osResLockRef[i]);

        /* Read budget to RCB. */
        Os_RCB[pResLock->OsResLockResRef]->TmProtResBgtTask[osTaskId] = pResLock->OsResLockBudget;
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Init timing protection for task
 */
OS_LOCAL void Os_InitTaskTmProt(
    Os_TaskType osTaskId,
    const Os_TmProtCfgType* pTaskTpCfg,
    Os_TCBType* pTcb
)
{
    pTcb->taskTpStatus = TP_READY;
    pTcb->taskTpHead = NULL_PTR;
    pTcb->taskTpTimeStamp = 0U;
    pTcb->taskTpLastReadTime = 0U;
    pTcb->taskTpEerlistArrival = 0U;

    /* Set budget of each type*/
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_3138, VL_Os_3141 */
    Os_TmProtInitCbData(&pTcb->taskTpNode[TP_EXE], TP_EXE, pTaskTpCfg->osExecutionBudget);
    Os_TmProtInitCbData(&pTcb->taskTpNode[TP_ARRIVAL], TP_ARRIVAL, pTaskTpCfg->osTimeFrame);
    Os_TmProtInitCbData(&pTcb->taskTpNode[TP_SUS_OS_INT], TP_SUS_OS_INT,
                             pTaskTpCfg->osOsInterruptLockBudget);
    Os_TmProtInitCbData(&pTcb->taskTpNode[TP_SUS_ALL_INT], TP_SUS_ALL_INT,
                             pTaskTpCfg->osAllInterruptLockBudget);
    Os_TmProtInitCbData(&pTcb->taskTpNode[TP_DIS_ALL_INT], TP_DIS_ALL_INT,
                             pTaskTpCfg->osAllInterruptLockBudget);
    /* PRQA S 3138, 3141 -- */

/* Init resource lock budget for task. */
#if (CFG_STD_RESOURCE_MAX > 0)
    if(NULL_PTR != pTaskTpCfg->osResLockRef)
    {
        Os_TmProtInitTaskResLock(osTaskId, pTaskTpCfg);
    }
#endif

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Init timing protection module
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_InitTmProt(Os_CoreIdType coreId)
/* PRQA S 1532 -- */
{
    boolean TpCoreNeedInit = Os_CfgTprot_Inf[coreId]; /* PRQA S 4461 */ /* VL_Os_4461 */
    Os_TpMonitor[coreId].Os_TpMonitored = TP_FOR_NONE;
    Os_TpMonitor[coreId].Os_TpTimeStamp = 0u;

/* Init protection para for task. */
    Os_TaskType taskIdStartRange = Os_TaskIdRange[coreId].AllTask.Start;
    Os_TaskType taskIdEndRange = Os_TaskIdRange[coreId].AllTask.End;
    for (uint16 i = (uint16)taskIdStartRange; i < (uint16)taskIdEndRange; i++)
    {
        if((FALSE == TpCoreNeedInit) || (NULL_PTR == Os_TaskCfg[i].TaskTmProtCfgRef))
        {
            Os_TCB[i]->taskTpStatus = TP_NO_INIT;
        }
        else
        {
            /* PRQA S 4342 ++ */ /* VL_Os_4342 */
            Os_InitTaskTmProt((Os_TaskType)i, Os_TaskCfg[i].TaskTmProtCfgRef, Os_TCB[i]);
            /* PRQA S 4342 -- */
        }
    }

    Os_IsrType isrIdStartRange = Os_IsrIdRange[coreId].Isr2.IsrStart;
    Os_IsrType isrIdEndRange = Os_IsrIdRange[coreId].Isr2.IsrEnd;
    /* Init protection para for Isr-Cat2. */
    for (uint16 i = (uint16)isrIdStartRange; i < (uint16)isrIdEndRange; i++)
    {
        if((FALSE == TpCoreNeedInit) || (NULL_PTR == Os_IsrCfg[i].isrTmProtCfgRef))
        {
            Os_ICB[i]->isrTpStatus = TP_NO_INIT;
        }
        else
        {
            /* PRQA S 4342 ++ */ /* VL_Os_4342 */
            Os_InitIsrTmProt((Os_IsrType)i, Os_IsrCfg[i].isrTmProtCfgRef, Os_ICB[i]);
            /* PRQA S 4342 -- */
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Timing Protection call Protecthook
 */
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
OS_LOCAL void Os_TpCallProtectHook(Os_SCBType *pScb, const Os_TpNodeType *Head, uint32 osWhoHook)
/* PRQA S 3673 -- */
{
    StatusType osErrType = E_OS_PROTECTION_LOCKED;
    if(TP_EXE == Head->TpNodeOpt)
    {
        osErrType = E_OS_PROTECTION_TIME;
    }

#if ((OS_SC4 == CFG_SC) && (CFG_TRUSTED_SYSTEM_SERVICE_MAX > 0U))
    Os_TrustedFuncTpDataDef *trustedFuncTp = &pScb->Os_TrustedFuncTp;
    if (TRUE == trustedFuncTp->TrustedFuncTportDelayCall)
    {
        /* Time protection and delay processing are triggered in the trusted function */
        trustedFuncTp->TrustedFuncTporFlag = TRUE;
        trustedFuncTp->TrustedFuncTporErrType = osErrType;
        trustedFuncTp->TrustedFuncWhoHook = osWhoHook;
        Os_TpSetCompare(pScb->SysCore, OS_HAL_TP_MAX_INCREMENT_VALUE, TP_FOR_NONE);
    }
    else
#endif
    {
        /* Hook. */
        (void)Os_CallProtectionHook(osErrType, osWhoHook);
    }

    UNUSED_PARAMETER(pScb);
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Timing Protection call Protecthook for task
 */
OS_LOCAL void Os_TpTaskCallProtectHook(Os_SCBType *pScb)
{
    const Os_TpNodeType* pHead = pScb->SysRunningTCB->taskTpHead;
    if(NULL_PTR != pHead)
    {
        Os_TpCallProtectHook(pScb, pHead, OS_TMPROT_HOOK_TASK);
    }
    else
    {
        Os_TpSetCompare(pScb->SysCore, OS_HAL_TP_MAX_INCREMENT_VALUE, TP_FOR_NONE);
    }
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Timing Protection call Protecthook for Isr2
 */
OS_LOCAL void Os_TpIsrCallProtectHook(Os_SCBType *pScb)
{
    Os_IsrType IsrId = pScb->SysIsrNestQueue[pScb->IntNestISR2 - OS_TMPROT_TARGET_ISR_OFFSET];
    const Os_TpNodeType* pHead = Os_ICB[IsrId]->isrTpHead;
    if(NULL_PTR != pHead)
    {
        Os_TpCallProtectHook(pScb, pHead, OS_TMPROT_HOOK_ISR);
    }
    else
    {
        Os_TpSetCompare(pScb->SysCore, OS_HAL_TP_MAX_INCREMENT_VALUE, TP_FOR_NONE);
    }
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Timing Protection isr callback
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_TmProtMainProc(void)
/* PRQA S 1532 -- */
{
    Os_CoreIdType coreId = Os_GetCoreIdLocal();
    Os_SCBType *pScb = Os_GetSystemContext(coreId);

    switch (Os_TpMonitor[coreId].Os_TpMonitored)
    {
    case TP_FOR_NONE:
        Os_TpMonitor[coreId].Os_TpTimeStamp += Os_Hal_TpGetTimePass(coreId);
        Os_Hal_TpSetCompare(coreId, OS_HAL_TP_MAX_INCREMENT_VALUE);
        break;

    case TP_FOR_TASK:
    	Os_TpTaskCallProtectHook(pScb);
        break;

    case TP_FOR_ISR:
    	Os_TpIsrCallProtectHook(pScb);
        break;

    default:
        /* Nothing to do */
        break;
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

/*=======[E N D   O F   F I L E]==============================================*/
/* PRQA S 0553 EOF */ /* VL_QAC_UnUsedFiles */
