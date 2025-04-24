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
/*
********************************************************************************
**                                                                            **
**  FILENAME    : Os_LoadRatio.c                                              **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : Os monitor                                                  **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Os.h"
#include "Os_Internal.h"
#include "Os_LoadRatio.h"

/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/
#if (TRUE == CFG_LOAD_RATIO_CALC_ENABLE)
#define OS_START_SEC_VAR_CLONE_UNSPECIFIED
#include "Os_MemMap.h"
static VAR(Os_LoadRatioRunStateType, OS_VAR) Os_LoadRatioRunState = LR_INVALID_STA;
#define OS_STOP_SEC_VAR_CLONE_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_UNSPECIFIED
#include "Os_MemMap.h"
static VAR(uint32, OS_VAR) Os_CpuLoadRatioCount = 0u;
#define OS_STOP_SEC_VAR_CLONE_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_UNSPECIFIED
#include "Os_MemMap.h"
static VAR(Os_LoadRatioType, OS_VAR) Os_CpuLoadRatio = {0u};
#define OS_STOP_SEC_VAR_CLONE_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
static P2VAR(Os_LoadRatioType, AUTOMATIC, OS_VAR) Os_TaskLoadRatio = NULL_PTR;
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
static P2VAR(Os_LoadRatioType, AUTOMATIC, OS_VAR) Os_IsrLoadRatio = NULL_PTR;
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Initialize the load ratio function module.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_InitSystem>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_InitLoadRatio(Os_CoreIdType coreId)
{
    VAR(uint16, OS_VAR) i;

    Os_ArchLoadRatioInit();

    Os_TaskLoadRatio = Os_TaskLoadRatio_Inf[coreId];
    Os_IsrLoadRatio = Os_IsrLoadRatio_Inf[coreId];

    Os_CpuLoadRatio.startTicks = Os_ArchGetTimeTicks();

    for (i = 0u; i < Os_SCB.sysTaskMax; i++)
    {
        Os_TaskLoadRatio[i].averageLoadRatio = 0u;
        Os_TaskLoadRatio[i].maxLoadRatio = 0u;
        Os_TaskLoadRatio[i].minLoadRatio = 0u;
        Os_TaskLoadRatio[i].totalLoadRatio = 0u;
        Os_TaskLoadRatio[i].singleLoadRatio = 0;
        Os_TaskLoadRatio[i].totalTicks = 0u;
        Os_TaskLoadRatio[i].startTicks = Os_CpuLoadRatio.startTicks;
    }
    for (i = 0u; i < Os_CfgIsrMax; i++)
    {
        Os_IsrLoadRatio[i].averageLoadRatio = 0u;
        Os_IsrLoadRatio[i].maxLoadRatio = 0u;
        Os_IsrLoadRatio[i].minLoadRatio = 0u;
        Os_IsrLoadRatio[i].totalLoadRatio = 0u;
        Os_IsrLoadRatio[i].singleLoadRatio = 0;
        Os_IsrLoadRatio[i].totalTicks = 0u;
        Os_IsrLoadRatio[i].startTicks = Os_CpuLoadRatio.startTicks;
    }
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Gets the gap tick value.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_CalcLoadRatio, Os_ExitTaskRecordTick,
 *                       Os_EnterIsrRecordTick, Os_ExitIsrRecordTick>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(uint32, OS_CODE) Os_GetTickGap(uint32 startTicks, uint32 curTicks)
{
    VAR(uint32, OS_VAR) interTicks;

    if (curTicks >= startTicks)
    {
        interTicks = curTicks - startTicks;
    }
    else
    {
        interTicks = (OS_SYSTEM_TIMER_MAX_TICK_VALUE - startTicks) + curTicks;
    }

    return interTicks;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Save the average, maximum and minimum value of the load rate.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_CalcLoadRatio>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(void, OS_CODE) Os_SaveLoadRatio(Os_LoadRatioRefType ObjloadRatio, uint32 loadRatio)
{
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

    if (Os_CpuLoadRatioCount < OS_LOAD_RATIO_CALC_NUM)
    {
        ObjloadRatio->averageLoadRatio = (ObjloadRatio->totalLoadRatio / Os_CpuLoadRatioCount);
    }
    else
    {
        ObjloadRatio->averageLoadRatio = (ObjloadRatio->totalLoadRatio / OS_LOAD_RATIO_CALC_NUM);
        ObjloadRatio->totalLoadRatio = 0;
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Calculate the load ratio.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ExitISR2>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_CalcLoadRatio(uint32 curTicks)
{
    VAR(uint32, OS_VAR) totalTicks = Os_GetTickGap(Os_CpuLoadRatio.startTicks, curTicks);

    if (totalTicks >= (OS_LOAD_RATIO_CALC_MS_INTERVAL * OS_SYSTEM_TIMER_REG_PER_MS_VALUE))
    {
        VAR(uint16, OS_VAR) i;
        VAR(uint64, OS_VAR) objTotalTicks;
        VAR(uint32, OS_VAR) loadRatio = 0u;

        Os_CpuLoadRatioCount++;
        if (OS_LOAD_RATIO_CALC_NUM < Os_CpuLoadRatioCount)
        {
            Os_CpuLoadRatioCount = 1u;
        }

        for (i = 0u; i < Os_SCB.sysTaskMax; i++)
        {
            objTotalTicks = Os_TaskLoadRatio[i].totalTicks;
            loadRatio = (objTotalTicks * 10000u) / totalTicks;
            Os_TaskLoadRatio[i].totalTicks = 0u;

            Os_SaveLoadRatio(&Os_TaskLoadRatio[i], loadRatio);
        }
        for (i = 0u; i < Os_CfgIsrMax; i++)
        {
            objTotalTicks = Os_IsrLoadRatio[i].totalTicks;
            loadRatio = (objTotalTicks * 10000u) / totalTicks;
            Os_IsrLoadRatio[i].totalTicks = 0u;

            Os_SaveLoadRatio(&Os_IsrLoadRatio[i], loadRatio);
        }
        /* Add the system load rate deviation to the load rate of the system clock. */
        loadRatio = 10000u - Os_TaskLoadRatio[OS_TASK_IDLE].singleLoadRatio;
        Os_SaveLoadRatio(&Os_CpuLoadRatio, loadRatio);

        Os_CpuLoadRatio.startTicks = Os_ArchGetTimeTicks();
        for (i = 0u; i < Os_SCB.sysTaskMax; i++)
        {
            Os_TaskLoadRatio[i].startTicks = Os_CpuLoadRatio.startTicks;
        }
        for (i = 0u; i < Os_CfgIsrMax; i++)
        {
            Os_IsrLoadRatio[i].startTicks = Os_CpuLoadRatio.startTicks;
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Calculate exectution time ticks when entering an task.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ExitISR2, Os_SwitchTask>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_EnterTaskRecordTick(TaskType TaskID, uint32 curTicks)
{
    /* Only work in Os_SwitchTask */
    if (Os_LoadRatioRunState != LR_ENTER_ISR)
    {
        Os_LoadRatioRunState = LR_ENTER_TASK;
        Os_TaskLoadRatio[TaskID].startTicks = curTicks;
    }
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Calculate exectution time ticks when exiting an task.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_Dispatch>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(uint32, OS_CODE) Os_ExitTaskRecordTick(TaskType TaskID)
{
    /* Only work in Os_SwitchTask, except when first switch task */
    VAR(uint32, OS_VAR) curTicks = Os_ArchGetTimeTicks();
    if (Os_LoadRatioRunState == LR_ENTER_TASK)
    {
        Os_TaskLoadRatio[TaskID].totalTicks += Os_GetTickGap(Os_TaskLoadRatio[TaskID].startTicks, curTicks);
        Os_LoadRatioRunState = LR_EXIT_TASK;
    }
    return curTicks;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Calculate exectution time ticks when entering an interrupt.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_EnterISR2, Os_EnterISR1>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_EnterIsrRecordTick(ISRType ISRID)
{
    Os_LoadRatioRunState = LR_ENTER_ISR;
    VAR(uint32, OS_VAR) curTicks = Os_ArchGetTimeTicks();
    if (0u == Os_IntNestISR1)
    {
        if (0u == Os_IntNestISR2)
        {
            /*Enter ISR2 from task.*/
            Os_TaskLoadRatio[Os_SCB.sysRunningTaskID].totalTicks +=
                Os_GetTickGap(Os_TaskLoadRatio[Os_SCB.sysRunningTaskID].startTicks, curTicks);
        }
        else
        {
            /*Enter ISR1/ISR2 from ISR2.*/
            Os_IsrLoadRatio[Os_SCB.sysRunningIsrCat2Id].totalTicks +=
                Os_GetTickGap(Os_IsrLoadRatio[Os_SCB.sysRunningIsrCat2Id].startTicks, curTicks);
        }
    }
    else
    {
        /* Enter ISR1 from ISR1. */
        VAR(ISRType, OS_VAR) prevIsrId = Os_SCB.sysIsrNestQueue[Os_IntNestISR2 + Os_IntNestISR1 - 1u];

        Os_IsrLoadRatio[prevIsrId].totalTicks += Os_GetTickGap(Os_IsrLoadRatio[prevIsrId].startTicks, curTicks);
    }

    Os_IsrLoadRatio[ISRID].startTicks = curTicks;
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Calculate exectution time ticks when exiting an interrupt.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ExitISR2, Os_ExitISR1>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(uint32, OS_CODE) Os_ExitIsrRecordTick(ISRType ISRID)
{
    VAR(uint32, OS_VAR) curTicks = Os_ArchGetTimeTicks();
    Os_IsrLoadRatio[ISRID].totalTicks += Os_GetTickGap(Os_IsrLoadRatio[ISRID].startTicks, curTicks);

    if ((Os_IntNestISR2 == 0) && ((Os_IntNestISR1 == 0) || (Os_IntNestISR1 == 1)))
    {
        Os_LoadRatioRunState = LR_ENTER_TASK;
        /* Exit ISR1/ISR2 to task. */
        Os_TaskLoadRatio[Os_SCB.sysRunningTaskID].startTicks = curTicks;
    }
    else if (Os_IntNestISR1 > 0)
    {
        /* Exit ISR1 to ISR. */
        VAR(ISRType, OS_VAR) prevIsrId = Os_SCB.sysIsrNestQueue[Os_IntNestISR2 + Os_IntNestISR1 - 2u];
        Os_IsrLoadRatio[prevIsrId].startTicks = curTicks;
    }
    else
    {
        /* Exit ISR2 to ISR. */
        VAR(ISRType, OS_VAR) prevIsrId = Os_SCB.sysIsrNestQueue[Os_IntNestISR2 - 1u];
        Os_IsrLoadRatio[prevIsrId].startTicks = curTicks;
    }

    return curTicks;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get the specified Task or ISR load ratio value>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <ObjectID: Task ID or ISR ID>
 *                      <calcType: Calculation method of load ratio>
 *                      <For CPU load ratio, Do Not care the value of ObjectID>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <10000 times larger percentage load rate>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(uint32, OS_CODE) Os_GetLoadRatioValue(AppObjectId ObjectID, Os_LoadRatioCalcType calcType)
{
    VAR(uint32, OS_VAR) loadRatioValue = 0xFFFFFFFF;

    switch (calcType)
    {
    case LR_CALC_TASK_AVERAGE:
        loadRatioValue = Os_TaskLoadRatio[ObjectID].averageLoadRatio;
        break;
    case LR_CALC_TASK_MAX:
        loadRatioValue = Os_TaskLoadRatio[ObjectID].maxLoadRatio;
        break;
    case LR_CALC_TASK_MIN:
        loadRatioValue = Os_TaskLoadRatio[ObjectID].minLoadRatio;
        break;
    case LR_CALC_TASK_LATEST:
        loadRatioValue = Os_TaskLoadRatio[ObjectID].singleLoadRatio;
        break;
    case LR_CALC_ISR_AVERAGE:
        loadRatioValue = Os_IsrLoadRatio[ObjectID].averageLoadRatio;
        break;
    case LR_CALC_ISR_MAX:
        loadRatioValue = Os_IsrLoadRatio[ObjectID].maxLoadRatio;
        break;
    case LR_CALC_ISR_MIN:
        loadRatioValue = Os_IsrLoadRatio[ObjectID].minLoadRatio;
        break;
    case LR_CALC_ISR_LATEST:
        loadRatioValue = Os_IsrLoadRatio[ObjectID].singleLoadRatio;
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
    default:
        break;
    }
    return loadRatioValue;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get the specified Task or ISR load ratio value>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <ObjectID: Task ID or ISR ID>
 *                      <calcType: Calculation method of load ratio>
 *                      <For CPU load ratio, Do Not care the value of ObjectID>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <10000 times larger percentage load rate>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(uint32, OS_CODE) GetLoadRatioValue(AppObjectId ObjectID, Os_LoadRatioCalcType calcType)
{
    OS_ENTER_KERNEL();
    VAR(uint32, OS_VAR) loadRatioValue = 0xFFFFFFFF;

    if (((calcType < LR_CALC_ISR_AVERAGE) && (ObjectID < Os_SCB.sysTaskMax))
        || ((calcType > LR_CALC_TASK_LATEST) && (calcType < LR_CALC_CPU_AVERAGE) && (ObjectID < Os_CfgIsrMax))
        || ((calcType > LR_CALC_ISR_LATEST) && (calcType < LR_CALC_INVALID)))
    {
        loadRatioValue = Os_GetLoadRatioValue(ObjectID, calcType);
    }

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA Dir 4.9 */ /* OS_TASK_MACRO_TO_FUNCTION_004 */
    return loadRatioValue;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif /* TRUE == CFG_LOAD_RATIO_CALC_ENABLE */

/*=======[E N D   O F   F I L E]==============================================*/
