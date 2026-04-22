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
 *
 ************************************************************************************************************************
 **
 **  @file               : Os_Osm_LoadRatio.c
 **  @author             : i-soft-os
 **  @date               : 2025/12/15
 **  @vendor             : isoft
 **  @description        : Os source file for LoadRatio API implementations
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Os_Arch_Processor.h"
#include "Os_Monitor.h"
#include "Os_Osm_LoadRatio.h"
#include "Os_Kernel.h"
#include "Os_Interrupt.h"
/* ========================================== external function definitions ========================================= */
#if (TRUE == CFG_LOAD_RATIO_CALC_ENABLE) /* PRQA S 3332 */ /* VL_Os_3332 */
/**
 * Get the specified Task or ISR load ratio value.
 */
OS_LOCAL uint32 Os_GetLoadRatioValue(ObjectType objectId, Os_LoadRatioCalcType calcType);

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Initialize the load ratio function module.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_InitLoadRatio(Os_CoreIdType coreId)
/* PRQA S 1532 -- */
{
	Os_SCBType *pScb = Os_GetSystemContext(coreId);
	Os_LoadRatioMonitorRefType Os_LoadRatioMonitor = &pScb->LoadRatioMonitor;
	Os_LoadRatioRefType Os_TaskLoadRatio;
	Os_LoadRatioRefType Os_IsrLoadRatio;
	Os_LoadRatioRefType Os_CpuLoadRatio = &Os_LoadRatioMonitor->Os_CpuLoadRatio;
    uint16 i;
    uint16 taskStartRange = (uint16)Os_TaskIdRange[coreId].AllTask.Start;
    uint16 taskEndRange   = (uint16)Os_TaskIdRange[coreId].AllTask.End;
    uint16 isrStartRange  = (uint16)Os_IsrIdRange[coreId].AllIsr.IsrStart;
    uint16 isrEndRange    = (uint16)Os_IsrIdRange[coreId].AllIsr.IsrEnd;

    Os_LoadRatioMonitor->Os_LoadRatioRunState = LR_INVALID_STA;
    Os_LoadRatioMonitor->Os_CpuLoadRatioCount = 0u;
    Os_CpuLoadRatio->averageLoadRatio = 0u;
	Os_CpuLoadRatio->maxLoadRatio = 0u;
	Os_CpuLoadRatio->minLoadRatio = 0u;
	Os_CpuLoadRatio->totalLoadRatio = 0u;
	Os_CpuLoadRatio->singleLoadRatio = 0u;
	Os_CpuLoadRatio->totalTicks = 0ULL;
	Os_CpuLoadRatio->startTicks = 0ULL;

    Os_LoadRatioMonitor->Os_TaskLoadRatio = Os_TaskLoadRatio_Inf;
    Os_LoadRatioMonitor->Os_IsrLoadRatio  = Os_IsrLoadRatio_Inf;

    Os_TaskLoadRatio = Os_LoadRatioMonitor->Os_TaskLoadRatio;
    Os_IsrLoadRatio = Os_LoadRatioMonitor->Os_IsrLoadRatio;

    Os_CpuLoadRatio->startTicks = Os_Hal_GetTimeTicks();

    for (i = taskStartRange; i < taskEndRange; i++)
    {
        Os_TaskLoadRatio[i].averageLoadRatio = 0u;
        Os_TaskLoadRatio[i].maxLoadRatio     = 0u;
        Os_TaskLoadRatio[i].minLoadRatio     = 0u;
        Os_TaskLoadRatio[i].totalLoadRatio   = 0u;
        Os_TaskLoadRatio[i].singleLoadRatio  = 0u;
        Os_TaskLoadRatio[i].totalTicks       = 0ULL;
        Os_TaskLoadRatio[i].startTicks       = Os_CpuLoadRatio->startTicks;
    }
    for (i = isrStartRange; i < isrEndRange; i++)
    {
        Os_IsrLoadRatio[i].averageLoadRatio = 0u;
        Os_IsrLoadRatio[i].maxLoadRatio     = 0u;
        Os_IsrLoadRatio[i].minLoadRatio     = 0u;
        Os_IsrLoadRatio[i].totalLoadRatio   = 0u;
        Os_IsrLoadRatio[i].singleLoadRatio  = 0u;
        Os_IsrLoadRatio[i].totalTicks       = 0ULL;
        Os_IsrLoadRatio[i].startTicks       = Os_CpuLoadRatio->startTicks;
    }
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Get the gap tick value.
 */
/* PRQA S 3450 ++ */ /* VL_Os_3450 */
static Os_TimerTickType Os_GetTickGap(Os_TimerTickType startTicks, Os_TimerTickType curTicks)
/* PRQA S 3450 -- */
{
    return curTicks - startTicks;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Save the average, maximum and minimum value of the load rate.
 */
/* PRQA S 3450 ++ */ /* VL_Os_3450 */
static void Os_SaveLoadRatio(Os_LoadRatioRefType ObjloadRatio, uint32 loadRatio)
/* PRQA S 3450 -- */
{
    const Os_SCBType *pScb = Os_GetCurrentContext();
    ObjloadRatio->singleLoadRatio = loadRatio;

    if (loadRatio > ObjloadRatio->maxLoadRatio)
    {
        ObjloadRatio->maxLoadRatio = loadRatio;
    }

    if ((loadRatio < ObjloadRatio->minLoadRatio) || (0u == ObjloadRatio->minLoadRatio))
    {
        ObjloadRatio->minLoadRatio = loadRatio;
    }

    ObjloadRatio->totalLoadRatio += loadRatio;

    if (pScb->LoadRatioMonitor.Os_CpuLoadRatioCount < OS_LOAD_RATIO_CALC_NUM)
    {
        /* PRQA S 2834 ++ */ /* VL_Os_2834 */
        ObjloadRatio->averageLoadRatio = (ObjloadRatio->totalLoadRatio / pScb->LoadRatioMonitor.Os_CpuLoadRatioCount);
        /* PRQA S 2834 -- */
    }
    else
    {
        ObjloadRatio->averageLoadRatio = (ObjloadRatio->totalLoadRatio / OS_LOAD_RATIO_CALC_NUM);
        ObjloadRatio->totalLoadRatio   = 0u;
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Calculate the load ratio.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_CalcLoadRatio(Os_TimerTickType curTicks)
/* PRQA S 1532 -- */
{
	Os_SCBType *pScb = Os_GetCurrentContext();
	Os_LoadRatioMonitorRefType Os_LoadRatioMonitor = &pScb->LoadRatioMonitor;
	Os_LoadRatioRefType Os_TaskLoadRatio = Os_LoadRatioMonitor->Os_TaskLoadRatio;
	Os_LoadRatioRefType Os_IsrLoadRatio = Os_LoadRatioMonitor->Os_IsrLoadRatio;
    Os_TimerTickType totalTicks = Os_GetTickGap(Os_LoadRatioMonitor->Os_CpuLoadRatio.startTicks, curTicks);

    if (totalTicks >= (Os_TimerTickType)(OS_LOAD_RATIO_CALC_MS_INTERVAL * OS_HAL_SYSTEM_TIMER_REG_PER_MS_VALUE))
    {
        uint16 i;
        uint64 objTotalTicks;
        uint32 loadRatio = 0u;

        Os_LoadRatioMonitor->Os_CpuLoadRatioCount++;
        if (OS_LOAD_RATIO_CALC_NUM < Os_LoadRatioMonitor->Os_CpuLoadRatioCount)
        {
            Os_LoadRatioMonitor->Os_CpuLoadRatioCount = 1u;
        }

        Os_CoreIdType coreId = pScb->SysCore;
        uint16 taskStartRange = (uint16)Os_TaskIdRange[coreId].AllTask.Start;
        uint16 taskEndRange   = (uint16)Os_TaskIdRange[coreId].AllTask.End;
        uint16 isrStartRange  = (uint16)Os_IsrIdRange[coreId].AllIsr.IsrStart;
        uint16 isrEndRange    = (uint16)Os_IsrIdRange[coreId].AllIsr.IsrEnd;

        for (i = taskStartRange; i < taskEndRange; i++)
        {
            objTotalTicks                  = Os_TaskLoadRatio[i].totalTicks;
            /* PRQA S 4461,3120 ++ */ /* VL_Os_4461,VL_QAC_MagicNum*/
            loadRatio                      = (uint32)((objTotalTicks * 10000u) / totalTicks);
            /* PRQA S 4461,3120 -- */
            Os_TaskLoadRatio[i].totalTicks = 0ULL;

            Os_SaveLoadRatio(&Os_TaskLoadRatio[i], loadRatio);
        }
        for (i = isrStartRange; i < isrEndRange; i++)
        {
            objTotalTicks                 = Os_IsrLoadRatio[i].totalTicks;
            /* PRQA S 4461,3120 ++ */ /* VL_Os_4461,VL_QAC_MagicNum*/
            loadRatio                     = (uint32)((objTotalTicks * 10000u) / totalTicks);
            /* PRQA S 4461,3120 -- */
            Os_IsrLoadRatio[i].totalTicks = 0ULL;

            Os_SaveLoadRatio(&Os_IsrLoadRatio[i], loadRatio);
        }
        /* Add the system load rate deviation to the load rate of the system clock. */
        /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum*/
        loadRatio = 10000u - Os_TaskLoadRatio[pScb->TaskIdle].singleLoadRatio;
        /* PRQA S 3120 -- */
        Os_SaveLoadRatio(&Os_LoadRatioMonitor->Os_CpuLoadRatio, loadRatio);

        Os_LoadRatioMonitor->Os_CpuLoadRatio.startTicks = Os_Hal_GetTimeTicks();
        for (i = taskStartRange; i < taskEndRange; i++)
        {
            Os_TaskLoadRatio[i].startTicks = Os_LoadRatioMonitor->Os_CpuLoadRatio.startTicks;
        }
        for (i = isrStartRange; i < isrEndRange; i++)
        {
            Os_IsrLoadRatio[i].startTicks = Os_LoadRatioMonitor->Os_CpuLoadRatio.startTicks;
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Calculate exectution time ticks when entering an task.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_EnterTaskRecordTick(TaskType TaskID, Os_TimerTickType curTicks)
/* PRQA S 1532 -- */
{
    if(INVALID_TASK != TaskID)
    {
        Os_SCBType *pScb = Os_GetCurrentContext();
        Os_LoadRatioMonitorRefType Os_LoadRatioMonitor = &pScb->LoadRatioMonitor;
        /* Only work in Os_SwitchTask */
        if (Os_LoadRatioMonitor->Os_LoadRatioRunState != LR_ENTER_ISR)
        {
            Os_LoadRatioMonitor->Os_LoadRatioRunState                = LR_ENTER_TASK;
            Os_LoadRatioMonitor->Os_TaskLoadRatio[TaskID].startTicks = curTicks;
        }
    }
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Calculate exectution time ticks when exiting an task.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Os_TimerTickType Os_ExitTaskRecordTick(TaskType TaskID)
/* PRQA S 1532 -- */
{
    Os_TimerTickType curTicks = Os_Hal_GetTimeTicks();
    if(INVALID_TASK != TaskID)
    {
        Os_SCBType *pScb = Os_GetCurrentContext();
        /* PRQA S 3679 ++ */ /* VL_Os_3679 */
        Os_LoadRatioMonitorRefType Os_LoadRatioMonitor = &pScb->LoadRatioMonitor;
        /* PRQA S 3679 -- */
        Os_LoadRatioRunStateType *Os_LoadRatioRunState = &Os_LoadRatioMonitor->Os_LoadRatioRunState;
        Os_LoadRatioRefType Os_TaskLoadRatio = Os_LoadRatioMonitor->Os_TaskLoadRatio;
        /* Only work in Os_SwitchTask, except when first switch task */
        if (*Os_LoadRatioRunState == LR_ENTER_TASK)
        {
            Os_TaskLoadRatio[TaskID].totalTicks += Os_GetTickGap(Os_TaskLoadRatio[TaskID].startTicks, curTicks);
            *Os_LoadRatioRunState = LR_EXIT_TASK;
        }        
    }
    return curTicks;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Calculate exectution time ticks when entering an interrupt.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_EnterIsrRecordTick(ISRType ISRID)
/* PRQA S 1532 -- */
{
	Os_SCBType *pScb = Os_GetCurrentContext();
	Os_LoadRatioMonitorRefType Os_LoadRatioMonitor = &pScb->LoadRatioMonitor;
	Os_LoadRatioRefType Os_TaskLoadRatio = Os_LoadRatioMonitor->Os_TaskLoadRatio;
	Os_LoadRatioRefType Os_IsrLoadRatio = Os_LoadRatioMonitor->Os_IsrLoadRatio;
	Os_LoadRatioMonitor->Os_LoadRatioRunState         = LR_ENTER_ISR;
    Os_TimerTickType curTicks      = Os_Hal_GetTimeTicks();
    if (0u == pScb->IntNestISR1)
    {
        if (0u == pScb->IntNestISR2)
        {
            /*Enter ISR2 from task.*/
            Os_TaskLoadRatio[pScb->SysRunningTaskId].totalTicks +=
                Os_GetTickGap(Os_TaskLoadRatio[pScb->SysRunningTaskId].startTicks, curTicks);
        }
        else
        {
            /*Enter ISR1/ISR2 from ISR2.*/
            Os_IsrLoadRatio[pScb->SysRunningIsrCat2Id].totalTicks +=
                Os_GetTickGap(Os_IsrLoadRatio[pScb->SysRunningIsrCat2Id].startTicks, curTicks);
        }
    }
    else
    {
        /* Enter ISR1 from ISR1. */
        ISRType prevIsrId = pScb->SysIsrNestQueue[pScb->IntNestISR2 + pScb->IntNestISR1 - 1u];

        Os_IsrLoadRatio[prevIsrId].totalTicks += Os_GetTickGap(Os_IsrLoadRatio[prevIsrId].startTicks, curTicks);
    }

    Os_IsrLoadRatio[ISRID].startTicks = curTicks;
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Calculate exectution time ticks when exiting an interrupt.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Os_TimerTickType Os_ExitIsrRecordTick(ISRType ISRID)
/* PRQA S 1532 -- */
{
	Os_SCBType *pScb = Os_GetCurrentContext();
	Os_LoadRatioMonitorRefType Os_LoadRatioMonitor = &pScb->LoadRatioMonitor;
	Os_LoadRatioRefType Os_TaskLoadRatio = Os_LoadRatioMonitor->Os_TaskLoadRatio;
	Os_LoadRatioRefType Os_IsrLoadRatio  = Os_LoadRatioMonitor->Os_IsrLoadRatio;
    Os_TimerTickType curTicks = Os_Hal_GetTimeTicks();
    Os_IsrLoadRatio[ISRID].totalTicks += Os_GetTickGap(Os_IsrLoadRatio[ISRID].startTicks, curTicks);

    if ((pScb->IntNestISR2 == 0u) && ((pScb->IntNestISR1 == 0u) || (pScb->IntNestISR1 == 1u)))
    {
    	Os_LoadRatioMonitor->Os_LoadRatioRunState = LR_ENTER_TASK;
        /* Exit ISR1/ISR2 to task. */
        Os_TaskLoadRatio[pScb->SysRunningTaskId].startTicks = curTicks;
    }
    else if (pScb->IntNestISR1 > 0u)
    {
        /* Exit ISR1 to ISR. */
        /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum*/
        ISRType prevIsrId        = pScb->SysIsrNestQueue[pScb->IntNestISR2 + pScb->IntNestISR1 - 2u];
        /* PRQA S 3120 -- */
        Os_IsrLoadRatio[prevIsrId].startTicks = curTicks;
    }
    else
    {
        /* Exit ISR2 to ISR. */
        ISRType prevIsrId        = pScb->SysIsrNestQueue[pScb->IntNestISR2 - 1u];
        Os_IsrLoadRatio[prevIsrId].startTicks = curTicks;
    }

    return curTicks;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Get the specified Task or ISR load ratio value.
 */
/* PRQA S 6010 ++ */ /* VL_MTR_Os_STCYC */
OS_LOCAL uint32 Os_GetLoadRatioValue(ObjectType objectId, Os_LoadRatioCalcType calcType)
/* PRQA S 6010 -- */
{
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3679 ++ */ /* VL_Os_3679 */
    Os_LoadRatioMonitorRefType Os_LoadRatioMonitor = &pScb->LoadRatioMonitor;
    /* PRQA S 3679 -- */
    Os_LoadRatioType Os_CpuLoadRatio = Os_LoadRatioMonitor->Os_CpuLoadRatio;
    /* PRQA S 3679 ++ */ /* VL_Os_3679 */
    Os_LoadRatioRefType Os_TaskLoadRatio = Os_LoadRatioMonitor->Os_TaskLoadRatio;
    Os_LoadRatioRefType Os_IsrLoadRatio  = Os_LoadRatioMonitor->Os_IsrLoadRatio;
    /* PRQA S 3679 -- */
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum*/
    uint32 loadRatioValue = 0xFFFFFFFFU;
    /* PRQA S 3120 -- */

    /* PRQA S 2002 ++ */ /* VL_QAC_NoDefaultCase */
    switch (calcType)
    /* PRQA S 2002 -- */
    {
    case LR_CALC_TASK_AVERAGE:
        loadRatioValue = Os_TaskLoadRatio[objectId].averageLoadRatio;
        break;
    case LR_CALC_TASK_MAX:
        loadRatioValue = Os_TaskLoadRatio[objectId].maxLoadRatio;
        break;
    case LR_CALC_TASK_MIN:
        loadRatioValue = Os_TaskLoadRatio[objectId].minLoadRatio;
        break;
    case LR_CALC_TASK_LATEST:
        loadRatioValue = Os_TaskLoadRatio[objectId].singleLoadRatio;
        break;
    case LR_CALC_ISR_AVERAGE:
        loadRatioValue = Os_IsrLoadRatio[objectId].averageLoadRatio;
        break;
    case LR_CALC_ISR_MAX:
        loadRatioValue = Os_IsrLoadRatio[objectId].maxLoadRatio;
        break;
    case LR_CALC_ISR_MIN:
        loadRatioValue = Os_IsrLoadRatio[objectId].minLoadRatio;
        break;
    case LR_CALC_ISR_LATEST:
        loadRatioValue = Os_IsrLoadRatio[objectId].singleLoadRatio;
        break;
    case LR_CALC_CPU_AVERAGE:
        loadRatioValue = Os_CpuLoadRatio.averageLoadRatio;
        break;
    case LR_CALC_CPU_MAX:
        loadRatioValue = Os_CpuLoadRatio.maxLoadRatio;
        break;
    case LR_CALC_CPU_MIN:
        loadRatioValue = Os_CpuLoadRatio.minLoadRatio;
        break;
    case LR_CALC_CPU_LATEST:
        loadRatioValue = Os_CpuLoadRatio.singleLoadRatio;
        break;
    default: /* Nothing to do. */
        break;
    }
    return loadRatioValue;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Get the specified Task or ISR load ratio value.
 */
/* PRQA S 1503, 3006, 3408, 1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_Os_3408, VL_Os_1512 */
uint32 GetLoadRatioValue(ObjectType objectId, LoadRatioCalcType calcType)
/* PRQA S 1503, 3006, 3408, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum*/
    uint32 loadRatioValue = 0xFFFFFFFFU;
    /* PRQA S 3120 -- */

    switch(calcType)
    {
    case LR_CALC_TASK_AVERAGE:
    case LR_CALC_TASK_MAX:
    case LR_CALC_TASK_MIN:
    case LR_CALC_TASK_LATEST:
    	if (objectId < CFG_TASK_MAX)
    	{
    		loadRatioValue = Os_GetLoadRatioValue(objectId, calcType);
    	}
    	break;
    case LR_CALC_ISR_AVERAGE:
    case LR_CALC_ISR_MAX:
    case LR_CALC_ISR_MIN:
    case LR_CALC_ISR_LATEST:
    	if (objectId < CFG_ISR_MAX)
    	{
    		loadRatioValue = Os_GetLoadRatioValue(objectId, calcType);
    	}
    	break;
    case LR_CALC_CPU_AVERAGE:
    case LR_CALC_CPU_MAX:
    case LR_CALC_CPU_MIN:
    case LR_CALC_CPU_LATEST:
    	loadRatioValue = Os_GetLoadRatioValue(objectId, calcType);
    	break;

    default: /* Nothing to do. */
    	break;
    }

    /* PRQA S 3138,3141 ++ */ /* VL_Os_PlatformNoDef */
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3138,3141 -- */
    return loadRatioValue;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif

/*=======[E N D   O F   F I L E]==============================================*/
/* PRQA S 0553 EOF */ /* VL_QAC_UnUsedFiles */
/* PRQA S 6610 EOF */ /* VL_MTR_Os_STCDN */
