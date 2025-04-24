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
**  FILENAME    : Os_LoadRatio.h                                              **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/
#ifndef OS_LOADRATIO_H
#define OS_LOADRATIO_H

/*=======[I N C L U D E S]====================================================*/
#include "Os_Types.h"

/*=======[M A C R O S]========================================================*/

#if (TRUE == CFG_LOAD_RATIO_CALC_ENABLE) /* PRQA S 3332 ++*/ /* MISRA Rule-20.9*/

#define OS_LOAD_RATIO_CALC_NUM         (10u)

#define OS_LOAD_RATIO_CALC_MS_INTERVAL (1000u)

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
typedef struct
{
    uint32 averageLoadRatio;
    uint32 maxLoadRatio;
    uint32 minLoadRatio;
    uint32 totalLoadRatio;
    uint32 singleLoadRatio;
    uint32 totalTicks;
    uint32 startTicks;
} Os_LoadRatioType;

typedef enum
{
    LR_INVALID_STA = 0,
    LR_ENTER_TASK,
    LR_EXIT_TASK,
    LR_ENTER_ISR,
    LR_EXIT_ISR,
} Os_LoadRatioRunStateType;

typedef enum
{
    LR_CALC_TASK_AVERAGE = 0,
    LR_CALC_TASK_MAX,
    LR_CALC_TASK_MIN,
    LR_CALC_TASK_LATEST,
    LR_CALC_ISR_AVERAGE,
    LR_CALC_ISR_MAX,
    LR_CALC_ISR_MIN,
    LR_CALC_ISR_LATEST,
    LR_CALC_CPU_AVERAGE,
    LR_CALC_CPU_MAX,
    LR_CALC_CPU_MIN,
    LR_CALC_CPU_LATEST,
    LR_CALC_INVALID,
} Os_LoadRatioCalcType;

typedef Os_LoadRatioType* Os_LoadRatioRefType;
/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

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
extern FUNC(void, OS_CODE) Os_InitLoadRatio(Os_CoreIdType coreId);

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
extern FUNC(void, OS_CODE) Os_CalcLoadRatio(uint32 curTicks);

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
extern FUNC(void, OS_CODE) Os_EnterTaskRecordTick(TaskType TaskID, uint32 curTicks);

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
extern FUNC(uint32, OS_CODE) Os_ExitTaskRecordTick(TaskType TaskID);

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
extern FUNC(void, OS_CODE) Os_EnterIsrRecordTick(ISRType ISRID);

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
extern FUNC(uint32, OS_CODE) Os_ExitIsrRecordTick(ISRType ISRID);

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
extern FUNC(uint32, OS_CODE) Os_GetLoadRatioValue(AppObjectId ObjectID, Os_LoadRatioCalcType calcType);

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
extern FUNC(uint32, OS_CODE) GetLoadRatioValue(AppObjectId ObjectID, Os_LoadRatioCalcType calcType);

#endif /* TRUE == CFG_LOAD_RATIO_CALC_ENABLE */

#endif /* OS_LOADRATIO_H */
/*=======[E N D   O F   F I L E]==============================================*/
