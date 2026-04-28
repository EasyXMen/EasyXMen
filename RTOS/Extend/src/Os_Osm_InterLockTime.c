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
 **  @file               : Os_Osm_InterLockTime.c
 **  @author             : i-soft-os
 **  @date               : 2025/12/15
 **  @vendor             : isoft
 **  @description        : Os source file for InterLockTime API implementations
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Os_Monitor.h"
#include "Os_Kernel.h"
#include "Os_Arch_Processor.h"

/* ========================================== external function definitions ========================================= */
#if (CFG_INTERRUPT_MONITOR_ENABLE == TRUE) /* PRQA S 3332 */ /* VL_Os_3332 */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Init the Interrupt Lock Monitor block control.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_InterMonitorInit(Os_CoreIdType coreId)
/* PRQA S 1532 -- */
{
	Os_SCBType *pScb = Os_GetSystemContext(coreId);
	Os_InterMonitorRatioRefType IsrMonitorRatio = &pScb->InterMonitorRatio;
    pScb->InterInitFlag = FALSE;
    if (NULL_PTR != Os_InterRecordTable_Inf[coreId])
    {
        uint16            loop = 1U;
        Os_InterLinkList* List = Os_InterTopRecordTable_Inf[coreId];

        IsrMonitorRatio->LoopCnt              = 0U;
        IsrMonitorRatio->InterAllStartTime = 0ULL;
        IsrMonitorRatio->InterOsStartTime  = 0ULL;
        IsrMonitorRatio->InterAllLockTime  = 0ULL;
        IsrMonitorRatio->InterOsLockTime   = 0ULL;

        IsrMonitorRatio->InterRecordTable = Os_InterRecordTable_Inf[coreId];

        List[0].Pre  = NULL_PTR;
        List[0].Next = &(List[1]);
        for (; loop < (TOP_RECORED_TABLE_NUM - 1U); loop++)
        {
            List[loop].Pre           = &(List[loop - 1U]);
            List[loop].Next          = &(List[loop + 1U]);
            List[loop].Node.LockTime = 0ULL;
        }
        /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum*/
        List[TOP_RECORED_TABLE_NUM - 1U].Pre  = &(List[TOP_RECORED_TABLE_NUM - 2U]);
        /* PRQA S 3120 -- */
        List[TOP_RECORED_TABLE_NUM - 1U].Next = NULL_PTR;

        IsrMonitorRatio->TopRecordTableHead = List;
        /* PRQA S 0488,0489 ++ */ /* VL_Os_0488,VL_Os_0489 */
        IsrMonitorRatio->TopRecordTableEnd  = (List + TOP_RECORED_TABLE_NUM - 1U);
        /* PRQA S 0488,0489 -- */

        IsrMonitorRatio->StartTime = Os_Hal_GetTimeTicks();

        pScb->InterInitFlag = TRUE;
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Insert the Interrupt Lock Monitor Node to TopTable.
 */
/* PRQA S 3450,3673 ++ */ /* VL_Os_3450,VL_QAC_3673 */
static void Os_TopTableInsert(Os_InterRecordType* InsertNode)
/* PRQA S 3450,3673 -- */
{
    Os_SCBType *pScb = Os_GetCurrentContext();
    Os_InterMonitorRatioRefType IsrMonitorRatio = &pScb->InterMonitorRatio;
    Os_InterLinkList* Temp     = IsrMonitorRatio->TopRecordTableEnd->Pre;
    Os_InterLinkList* list     = IsrMonitorRatio->TopRecordTableEnd;
    uint64            LockTime = InsertNode->LockTime;

    /* PRQA S 4461 ++ */ /* VL_Os_4461 */
    list->Node.LockTime  = LockTime;
    /* PRQA S 4461 -- */
    list->Node.CallerId  = InsertNode->CallerId;
    list->Node.CallLevel = InsertNode->CallLevel;
    list->Node.CallApi   = InsertNode->CallApi;

    while ((Temp->Node.LockTime < LockTime) && (Temp->Pre != NULL_PTR))
    {
        Temp = Temp->Pre;
    }

    if (IsrMonitorRatio->TopRecordTableEnd->Pre != Temp)
    {
    	IsrMonitorRatio->TopRecordTableEnd       = IsrMonitorRatio->TopRecordTableEnd->Pre;
    	IsrMonitorRatio->TopRecordTableEnd->Next = NULL_PTR;

        if (IsrMonitorRatio->TopRecordTableHead->Node.LockTime < LockTime)
        {
            Temp->Pre          = list;
            list->Next         = Temp;
            list->Pre          = NULL_PTR;
            IsrMonitorRatio->TopRecordTableHead = list;
        }
        else
        {
            Temp->Next->Pre = list;
            list->Next      = Temp->Next;
            list->Pre       = Temp;
            Temp->Next      = list;
        }
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Record the Interrupt Lock Monitor Start Time.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_InterMonitorStartRecord(Os_InterApiType ApiType)
/* PRQA S 1532 -- */
{
    Os_SCBType *pScb = Os_GetCurrentContext();
    Os_InterMonitorRatioRefType IsrMonitorRatio = &pScb->InterMonitorRatio;
    if (OS_OS_SUSPEND == ApiType)
    {
    	IsrMonitorRatio->InterOsStartTime = Os_Hal_GetTimeTicks();
    }
    else if (OS_ALL_SUSPEND == ApiType)
    {
    	IsrMonitorRatio->InterAllStartTime = Os_Hal_GetTimeTicks();
    }
    else
    {
        /*Nothing to do.*/
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Record the Interrupt Lock Monitor Lock Time to Record Table.
 */
/* PRQA S 1532, 2755 ++ */ /* VL_QAC_OneFunRef, VL_Os_2755 */
StatusType Os_InterMonitorEndRecord(Os_InterApiType ApiType)
/* PRQA S 1532, 2755 -- */
{
    Os_TimerTickType LockTime = 0ULL;
    uint16 CallerId = 0U;
    Os_SCBType *pScb = Os_GetCurrentContext();
    Os_InterMonitorRatioRefType IsrMonitorRatio = &pScb->InterMonitorRatio;
    uint16 LoopCnt = IsrMonitorRatio->LoopCnt;
    Os_InterRecordType* InterRecordTable = IsrMonitorRatio->InterRecordTable;
    StatusType err = E_NOT_OK;

    if (OS_OS_SUSPEND == ApiType)
    {
        /* PRQA S 3138,1840 ++ */ /* VL_Os_3138,VL_Os_1840 */
        LockTime = Os_GetTickInterval(IsrMonitorRatio->InterOsStartTime) / NS_PER_US;
        /* PRQA S 3138,1840 -- */
        /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral */
        if (LockTime > (Os_TimerTickType)CFG_INTERRUPT_MIN_RECORD_TIME)
        /* PRQA S 1258 -- */ 
        {
        	IsrMonitorRatio->InterOsLockTime += LockTime;
            err = E_OK;
        }
        IsrMonitorRatio->InterOsStartTime = 0ULL;
    }
    else if (OS_ALL_SUSPEND == ApiType)
    {
        /* PRQA S 3138,1840 ++ */ /* VL_Os_3138,VL_Os_1840 */
        LockTime = Os_GetTickInterval(IsrMonitorRatio->InterAllStartTime) / NS_PER_US;
        /* PRQA S 3138,1840 -- */
        /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral */
        if (LockTime > (Os_TimerTickType)CFG_INTERRUPT_MIN_RECORD_TIME)
        /* PRQA S 1258 -- */ 
        {
        	IsrMonitorRatio->InterAllLockTime += LockTime;
            err = E_OK;
        }
        IsrMonitorRatio->InterAllStartTime = 0ULL;
    }
    else
    {
        /*Nothing to do.*/
    }

    if (E_OK == err)
    {
        if (OS_LEVEL_TASK == pScb->SysOsLevel)
        {
            CallerId = (uint16)pScb->SysRunningTaskId;
        }
#if (CFG_ISR2_MAX > 0)
        else if (OS_LEVEL_ISR2 == pScb->SysOsLevel) /* PRQA S 2004 */ /* VL_Os_2004 */
        {
            CallerId = (uint16)pScb->SysRunningIsrCat2Id;
        }
#endif

        InterRecordTable[LoopCnt].CallerId  = CallerId;
        InterRecordTable[LoopCnt].CallLevel = pScb->SysOsLevel;
        InterRecordTable[LoopCnt].CallApi   = ApiType;
        InterRecordTable[LoopCnt].LockTime  = LockTime;

        if (IsrMonitorRatio->TopRecordTableEnd->Node.LockTime < LockTime)
        {
            Os_TopTableInsert(&InterRecordTable[LoopCnt]);
        }

        IsrMonitorRatio->LoopCnt = (LoopCnt + 1U) % RECORED_TABLE_NUM;
    }

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

/*=======[E N D   O F   F I L E]==============================================*/
/* PRQA S 0553 EOF */ /* VL_QAC_UnUsedFiles */
