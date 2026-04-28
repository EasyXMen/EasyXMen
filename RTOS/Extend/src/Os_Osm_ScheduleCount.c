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
 **  @file               : Os_Osm_ScheduleCount.c
 **  @author             : i-soft-os
 **  @date               : 2025/12/15
 **  @vendor             : isoft
 **  @description        : Os source file for ScheduleCount API implementations
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Os_Kernel.h"
#include "Os_Interrupt.h"
#include "Os_Task.h"

/* ========================================== external function definitions ========================================= */
#if (TRUE == CFG_SCHEDULE_COUNT_MONITOR) /* PRQA S 3332 */ /* VL_Os_3332 */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Increase the scheduling frequency of tasks.
 */
/* PRQA S 1532, 3408, 1512 ++ */ /* VL_QAC_OneFunRef, VL_Os_3408, VL_Os_1512 */
void Os_IncrementTaskScheduleCount(void)
/* PRQA S 1532, 3408, 1512 -- */
{
    const Os_SCBType *pScb = Os_GetCurrentContext();
    
    pScb->SysRunningTCB->taskScheduleCount = pScb->SysRunningTCB->taskScheduleCount + 1U;
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Increase the scheduling frequency of ISR2.
 */
/* PRQA S 1532, 3408, 1512 ++ */ /* VL_QAC_OneFunRef, VL_Os_3408, VL_Os_1512 */
void Os_IncrementIsr2ScheduleCount(ISRType ISRID)
/* PRQA S 1532, 3408, 1512 -- */
{
    Os_ICB[ISRID]->isrScheduleCount = Os_ICB[ISRID]->isrScheduleCount + 1U;
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Get the scheduling frequency of tasks.
 */
/* PRQA S 1503, 3408, 1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3408, VL_Os_1512 */
void GetTaskScheduleCount(TaskType taskId, uint32* taskCnt)
/* PRQA S 1503, 3408, 1512 -- */
{
    *taskCnt = Os_TCB[taskId]->taskScheduleCount;
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Get the scheduling frequency of ISR2.
 */
/* PRQA S 1503, 3408, 1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3408, VL_Os_1512 */
void GetIsr2ScheduleCount(ISRType isrId, uint32* isrCnt)
/* PRQA S 1503, 3408, 1512 -- */
{
    *isrCnt = Os_ICB[isrId]->isrScheduleCount;
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif

/*=======[E N D   O F   F I L E]==============================================*/
/* PRQA S 0553 EOF */ /* VL_QAC_UnUsedFiles */
/* PRQA S 6610 EOF */ /* VL_MTR_Os_STCDN */
