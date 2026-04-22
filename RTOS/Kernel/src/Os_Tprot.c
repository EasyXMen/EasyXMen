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
 ********************************************************************************
 **                                                                            **
 **  FILENAME    :  Os_Tprot.c                                                 **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      :  i-soft-os                                                  **
 **  Vendor      :                                                             **
 **  DESCRIPTION :                                                             **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
 **  Version :   AUTOSAR classic Platform R19--Function Safety                 **
 **                                                                            **
 *******************************************************************************/
/*=======[I N C L U D E S]====================================================*/
#include "Os_Internal.h"
/* PRQA S 0553 EOF */ /* VL_QAC_UnUsedFiles */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
/*=======[M A C R O S]========================================================*/
/* Initialize the time protection control block. */
/* PRQA S 3412 ++ */ /* VL_Os_3412 */
#define Os_TmProtInitCbData(pCbData, budget) \
    (pCbData)->osIsTpStart = FALSE;          \
    (pCbData)->osTpTime    = 0u;             \
    (pCbData)->osTpBudget  = (budget)
/* PRQA S 3412 -- */
/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#if (CFG_STD_RESOURCE_MAX > 0)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static void Os_TmProtResCounter(ResourceType osResId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static void Os_TmProtInitTaskResLock(Os_TaskType osTaskId, const Os_TaskCfgType* pTaskCfg);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* CFG_STD_RESOURCE_MAX > 0 */

#if (CFG_ISR2_MAX > 0U)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static void Os_TmProtIsrProc(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static void Os_TmProtIsrFrameCounter(Os_IsrType osIsrId, Os_TmProtOptIsrType osOptType);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static void Os_TmProtIsrCounter(Os_IsrType osIsrId, Os_TmProtOptIsrType osOptType);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static void Os_InitIsrTmProt(Os_IsrType osIsrId, const Os_IsrCfgType* pIsrCfg, Os_ICBType* pIcb);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (CFG_STD_RESOURCE_MAX > 0)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static void Os_TmProtInitIsrResLock(Os_IsrType osIsrId, const Os_IsrCfgType* pIsrCfg);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif /* CFG_STD_RESOURCE_MAX > 0 */
#endif /* CFG_ISR2_MAX > 0U */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static void Os_TmProtTaskFrameCounter(Os_TaskType osTaskId, Os_TmProtOptTaskType osOptType);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static void Os_TmProtTaskCounter(Os_TaskType osTaskId, Os_TmProtOptTaskType osOptType);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static void Os_InitTaskTmProt(Os_TaskType osTaskId, const Os_TaskCfgType* pTaskCfg, Os_TCBType* pTcb);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static void Os_TmProtTaskProc(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#if (CFG_STD_RESOURCE_MAX > 0)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Start process for budget of resource locking.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <None>
 * param-eventId[in]    <osResId>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <GetResource>
 * REQ ID               <None>
 */
/********************************************************************/
void Os_TmProtResStart(ResourceType osResId, Os_TmProtResOccupyType osOccupyType) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    uint8               osWhoHook;
    Os_TickType         osBudget;
    Os_RCBType*         pRcb;
    const Os_SCBType*   pScb;
    Os_TmProtCbDataDef* pCbData;

    pRcb = &Os_RCB[osResId];
    pScb = &Os_SCB;

    /* If start timing protection. */
    if (osOccupyType < TP_RES_OCCUPY_BUTT)
    {
        /* Get budget according to osOccupyType. */
        if (TP_RES_OCCUPY_TASK == osOccupyType) /* By task. */
        {
            osBudget  = pRcb->osTmProtResBgtTask[pScb->sysRunningTaskID];
            osWhoHook = OS_TMPROT_HOOK_TASK;

            /* Which task occupy this resource. */
            pRcb->osWhichTaskOccupy = pScb->sysRunningTaskID;
        }

#if (CFG_ISR2_MAX > 0)
        else /*TP_RES_OCCUPY_ISR == osOccupyType*/
        {
            osBudget  = pRcb->osTmProtResBgtIsr[pScb->sysRunningIsrCat2Id];
            osWhoHook = OS_TMPROT_HOOK_ISR;

            /* Which isr occupy this resource. */
            pRcb->osWhichIsrOccupy = pScb->sysRunningIsrCat2Id;
        }
#endif

        /* Timing protection start for task or isr. */
        if (OS_TICK_INVALID != osBudget) /* PRQA S 1258 */ /* VL_Os_1258 */
        {
            pCbData = &(pRcb->osResTpData);

            if (TRUE != pCbData->osIsTpStart)
            {
                pCbData->osIsTpStart = TRUE;
                pCbData->osTpTime    = 0u;
                pCbData->osTpBudget  = osBudget;
                pCbData->osWhoHook   = osWhoHook;
            }
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Counter process for budget of resource locking.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <None>
 * param-eventId[in]    <osResId>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_TmProtMainProc>
 * REQ ID               <None>
 */
/********************************************************************/
static void Os_TmProtResCounter(ResourceType osResId)
{
    Os_RCBType*         pRcb;
    const Os_TCBType*   pTcb;
    Os_TmProtCbDataDef* pCbData;
    const Os_SCBType*   pScb;
    boolean             Status = TRUE;
    pRcb                       = &Os_RCB[osResId];
    pCbData                    = &(pRcb->osResTpData);
    pScb                       = &Os_SCB;

    /* If occupied by task, if this task is not running, resource lock
     * counter stop. */
    if (pRcb->osWhichTaskOccupy < Os_SCB.sysTaskMax)
    {
        pTcb = &Os_TCB[pRcb->osWhichTaskOccupy];

        if (TASK_STATE_RUNNING != pTcb->taskState)
        {
            Status = FALSE;
        }
    }
    /* If occupied by isr and this isr is pending, resource lock counter stop.*/
    else if (Os_IntNestISR2 >= OS_TMPROT_TARGET_ISR_OFFSET)
    {
        /* Current_isr: Os_IntNestISR2-1;
         * OccupyResource_isr: Os_IntNestISR2-2. */
        if (pScb->sysIsrNestQueue[Os_IntNestISR2 - OS_TMPROT_TARGET_ISR_OFFSET] != pRcb->osWhichIsrOccupy)
        {
            Status = FALSE;
        }
    }
    else
    {
        Status = FALSE;
    }

    /* Lock_bugdet for this resource is configured. */
    if ((boolean)TRUE == Status)
    {
        if (TRUE == pCbData->osIsTpStart)
        {
            pCbData->osTpTime += 1U;
            if (pCbData->osTpTime >= pCbData->osTpBudget)
            {
                pCbData->osTpTime    = 0u;
                pCbData->osTpBudget  = 0u;
                pCbData->osIsTpStart = FALSE;

#if ((OS_SC4 == CFG_SC) && (CFG_TRUSTED_SYSTEM_SERVICE_MAX > 0U))
                if (TRUE == Os_TrustedFuncTp.TrustedFuncTportDelayCall)
                {
                    /* Time protection and delay processing are triggered in the trusted function */

                    Os_TrustedFuncTp.TrustedFuncTporFlag    = TRUE;
                    Os_TrustedFuncTp.TrustedFuncTporErrType = E_OS_PROTECTION_LOCKED;
                    Os_TrustedFuncTp.osWhoHook              = (uint32)pCbData->osWhoHook;
                }
                else
#endif
                {
                    /* Hook. */
                    (void)Os_CallProtectionHook(E_OS_PROTECTION_LOCKED, (uint32)pCbData->osWhoHook);
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
/********************************************************************/
/*
 * Brief                <End process for budget of resource locking.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <None>
 * param-eventId[in]    <osResId>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ApplReleaseResource>
 * REQ ID               <None>
 */
/********************************************************************/
void Os_TmProtResEnd(ResourceType osResId)
{
    Os_RCBType*         pRcb;
    Os_TmProtCbDataDef* pCbData;
    pRcb    = &Os_RCB[osResId];
    pCbData = &(pRcb->osResTpData);

    /* Stop timing protection for this resource. */
    if (TRUE == pCbData->osIsTpStart)
    {
        pCbData->osIsTpStart = FALSE;
        pCbData->osTpTime    = 0u;
        pCbData->osTpBudget  = 0u;
        pCbData->osWhoHook   = 0u;
    }

    /* Clear the task/isr occupy the resource. */
    pRcb->osWhichTaskOccupy = OS_TASK_INVALID;
    pRcb->osWhichIsrOccupy  = INVALID_ISR;

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* CFG_STD_RESOURCE_MAX > 0 */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Time frame of task: frame check process.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <osTaskId>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <ActivateTask>
 * REQ ID               <None>
 */
/********************************************************************/
/* PRQA S 6030, 1532 ++ */ /* VL_MTR_Os_STMIF, VL_QAC_OneFunRef */
StatusType Os_TmProtTaskFrameChk(Os_TaskType osTaskId)
/* PRQA S 6030, 1532 -- */
{
    StatusType          osRet = E_OK;
    Os_TCBType*         pTcb;
    Os_TmProtCbDataDef* pCbData;

    /* Idle task should not be protected. */
    if (OS_TASK_IDLE != osTaskId)
    {
        pTcb    = &Os_TCB[osTaskId];
        pCbData = &pTcb->osTpTask[TP_TASK_ARRIVAL];

        /* Timing frame for this task is not configed. */
        if (OS_TICK_INVALID != pCbData->osTpBudget) /* PRQA S 1258 */ /* VL_Os_1258 */
        {
            /* Timing frame: arrive so frequently. */
            if (TRUE != pTcb->taskTpFrameflag)
            {
#if ((OS_SC4 == CFG_SC) && (CFG_TRUSTED_SYSTEM_SERVICE_MAX > 0U))
                if (TRUE == Os_TrustedFuncTp.TrustedFuncTportDelayCall)
                {
                    /* Time protection and delay processing are triggered in the trusted function */
                    Os_TrustedFuncTp.TrustedFuncTporFlag    = TRUE;
                    Os_TrustedFuncTp.TrustedFuncTporErrType = E_OS_PROTECTION_ARRIVAL;
                    Os_TrustedFuncTp.osWhoHook              = OS_TMPROT_HOOK_TASK;
                }
                else
#endif
                {
                    /* Hook. */
                    /* PRQA S 1520 ++ */ /* VL_Os_1520 */
                    if (PRO_IGNORE != Os_CallProtectionHook(E_OS_PROTECTION_ARRIVAL, OS_TMPROT_HOOK_TASK))
                    /* PRQA S 1520 -- */
                    {
                        osRet = E_OS_ID;
                    }
                }
            }
            else
            {
                pCbData->osTpTime     = 0U;
                pTcb->taskTpFrameflag = FALSE;
            }
        }
    }

    return osRet;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Time frame of task: frame counter process.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <osTaskId, osOptType>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_TmProtTaskCounter>
 * REQ ID               <None>
 */
/********************************************************************/
static void Os_TmProtTaskFrameCounter(Os_TaskType osTaskId, Os_TmProtOptTaskType osOptType)
{
    Os_TCBType*         pTcb;
    Os_TmProtCbDataDef* pCbData;

    pTcb = &Os_TCB[osTaskId];

    /* Task has been actived. */
    if (TRUE != pTcb->taskTpFrameflag)
    {
        pCbData = &(pTcb->osTpTask[osOptType]);

        pCbData->osTpTime += 1U;

        if (pCbData->osTpTime >= pCbData->osTpBudget)
        {
            /* Set taskTpFrameflag, so the task can be actived again. */
            pTcb->taskTpFrameflag = TRUE;
            pCbData->osTpTime     = 0u;
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Timing protection start process of task.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <osTaskId, osOptType>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <DisableAllInterrupts>
 * REQ ID               <None>
 */
/********************************************************************/
void Os_TmProtTaskStart(Os_TaskType osTaskId, Os_TmProtOptTaskType osOptType)
{
    Os_TCBType*         pTcb;
    Os_TmProtCbDataDef* pCbData;

    /* Idle task should not be protected. */
    if (OS_TASK_IDLE != osTaskId)
    {
        pTcb    = &Os_TCB[osTaskId];
        pCbData = &(pTcb->osTpTask[osOptType]);

        /* This task cfg timing protection. */
        if (OS_TICK_INVALID != pCbData->osTpBudget) /* PRQA S 1258 */ /* VL_Os_1258 */
        {
            if (TRUE != pCbData->osIsTpStart)
            {
                pCbData->osIsTpStart = TRUE;
                pCbData->osTpTime    = 0u;
            }
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Timing protection counter process of task.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <osTaskId, osOptType>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_TmProtTaskProc>
 * REQ ID               <None>
 */
/********************************************************************/
/* PRQA S 6030 ++ */ /* VL_MTR_Os_STMIF */
static void Os_TmProtTaskCounter(Os_TaskType osTaskId, Os_TmProtOptTaskType osOptType)
/* PRQA S 6030-- */
{
    StatusType          osErrType;
    Os_TCBType*         pTcb;
    Os_TmProtCbDataDef* pCbData;

    /* Inter-arrival time. */
    if (TP_TASK_ARRIVAL == osOptType)
    {
        Os_TmProtTaskFrameCounter(osTaskId, osOptType);
    }
    else /* Other budget. */
    {
        pTcb    = &Os_TCB[osTaskId];
        pCbData = &(pTcb->osTpTask[osOptType]);

        if (TRUE == pCbData->osIsTpStart)
        {
            if (OS_TMPROT_TARGET_ISR_OFFSET <= Os_IntNestISR2)
            {
                /*nothing to do*/
            }
            else
            {
                pCbData->osTpTime += 1U;
                if (pCbData->osTpTime >= pCbData->osTpBudget)
                {
                    pCbData->osTpTime    = 0u;
                    pCbData->osIsTpStart = FALSE;

                    /* Get error type. */
                    if (TP_TASK_EXE == osOptType)
                    {
                        osErrType = E_OS_PROTECTION_TIME;
                    }
                    else
                    {
                        osErrType = E_OS_PROTECTION_LOCKED;
                    }
                    /* Resource lock is not maintained in this function. */

#if ((OS_SC4 == CFG_SC) && (CFG_TRUSTED_SYSTEM_SERVICE_MAX > 0U))
                    if (TRUE == Os_TrustedFuncTp.TrustedFuncTportDelayCall)
                    {
                        /* Time protection and delay processing are triggered in the trusted function */

                        Os_TrustedFuncTp.TrustedFuncTporFlag    = TRUE;
                        Os_TrustedFuncTp.TrustedFuncTporErrType = osErrType;
                        Os_TrustedFuncTp.osWhoHook              = OS_TMPROT_HOOK_TASK;
                    }
                    else
#endif
                    {
                        /* Hook. */
                        (void)Os_CallProtectionHook(osErrType, OS_TMPROT_HOOK_TASK);
                    }
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
/********************************************************************/
/*
 * Brief                <Timing protection end process of task.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <osTaskId, osOptType>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ApplTerminateOneTask and so on>
 * REQ ID               <None>
 */
/********************************************************************/
void Os_TmProtTaskEnd(Os_TaskType osTaskId, Os_TmProtOptTaskType osOptType)
{
    Os_TCBType*         pTcb;
    Os_TmProtCbDataDef* pCbData;

    /* Idle task should not be protected. */
    if (OS_TASK_IDLE != osTaskId)
    {
        pTcb    = &Os_TCB[osTaskId];
        pCbData = &(pTcb->osTpTask[osOptType]);

        if (TRUE == pCbData->osIsTpStart)
        {
            pCbData->osIsTpStart = FALSE;
            pCbData->osTpTime    = 0u;
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (CFG_ISR2_MAX > 0U)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Time frame of task: frame check process.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <osTaskId>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/********************************************************************/
/* PRQA S 6030, 1532 ++ */ /* VL_MTR_Os_STMIF, VL_QAC_OneFunRef */
StatusType Os_TmProtIsrFrameChk(Os_IsrType osIsrId)
/* PRQA S 6030, 1532 -- */
{
    StatusType          osRet = E_OK;
    Os_ICBType*         pIcb;
    Os_TmProtCbDataDef* pCbData;

    /* Note: When Os_ArchSystemTimer isr arrive, Os_IntCfgIsrId is
     * not set to vaild value.
     * On tc1782, priority of Os_ArchSystemTimer isr is 255. */
    if (osIsrId < CFG_ISR_MAX)
    {
        pIcb    = &Os_ICB[osIsrId];
        pCbData = &pIcb->osTpIsr[TP_ISR_CAT2_ARRIVAL];

        /* Timing frame for this isr is configed. */
        if (OS_TICK_INVALID != pCbData->osTpBudget) /* PRQA S 1258 */ /* VL_Os_1258 */
        {
            /* Timing frame: arrive so frequently. */
            if (TRUE != pIcb->osIsrTpFrameflag)
            {
#if ((OS_SC4 == CFG_SC) && (CFG_TRUSTED_SYSTEM_SERVICE_MAX > 0U))
                if (TRUE == Os_TrustedFuncTp.TrustedFuncTportDelayCall)
                {
                    /* Time protection and delay processing are triggered in the trusted function */
                    Os_TrustedFuncTp.TrustedFuncTporFlag    = TRUE;
                    Os_TrustedFuncTp.TrustedFuncTporErrType = E_OS_PROTECTION_ARRIVAL;
                    Os_TrustedFuncTp.osWhoHook              = OS_TMPROT_HOOK_ISR;
                }
                else
#endif
                {
                    /* Hook. */
                    if (PRO_IGNORE != Os_CallProtectionHook(E_OS_PROTECTION_ARRIVAL, OS_TMPROT_HOOK_ISR))
                    {
                        pCbData->osTpTime = 0U;
                        osRet             = E_OS_ID;
                    }
                }
            }
            else
            {
                pCbData->osTpTime      = 0U;
                pIcb->osIsrTpFrameflag = FALSE;
            }
        }
    }

    return osRet;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Timing protection timing frame process of isr.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <osTaskId, osOptType>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_TmProtIsrCounter>
 * REQ ID               <None>
 */
/********************************************************************/
static void Os_TmProtIsrFrameCounter(Os_IsrType osIsrId, Os_TmProtOptIsrType osOptType)
{
    Os_ICBType*         pIcb;
    Os_TmProtCbDataDef* pCbData;

    pIcb = &Os_ICB[osIsrId];

    /* Isr has been called. */
    if (TRUE != pIcb->osIsrTpFrameflag)
    {
        pCbData = &(pIcb->osTpIsr[osOptType]);
        pCbData->osTpTime += 1U;

        /* Set osIsrTpFrameflag to true, so the isr can be called again. */
        if (pCbData->osTpTime >= pCbData->osTpBudget)
        {
            pIcb->osIsrTpFrameflag = TRUE;
            pCbData->osTpTime      = 0U;
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Timing protection start process of isr.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <osTaskId, osOptType>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <DisableAllInterrupts>
 * REQ ID               <None>
 */
/********************************************************************/
void Os_TmProtIsrStart(Os_IsrType osIsrId, Os_TmProtOptIsrType osOptType) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    Os_ICBType*         pIcb;
    Os_TmProtCbDataDef* pCbData;

    pIcb    = &Os_ICB[osIsrId];
    pCbData = &(pIcb->osTpIsr[osOptType]);

    /* This isr cfg timing protection. */
    if (OS_TICK_INVALID != pCbData->osTpBudget) /* PRQA S 1258 */ /* VL_Os_1258 */
    {
        if (TRUE != pCbData->osIsTpStart)
        {
            pCbData->osIsTpStart = TRUE;
            pCbData->osTpTime    = 0u;
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Timing protection counter process of isr.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <osTaskId, osOptType>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_TmProtIsrProc>
 * REQ ID               <None>
 */
/********************************************************************/
static void Os_TmProtIsrCounter(Os_IsrType osIsrId, Os_TmProtOptIsrType osOptType)
{
    StatusType          osErrType;
    Os_ICBType*         pIcb;
    Os_TmProtCbDataDef* pCbData;

    /* Inter-arrival time. */
    if (TP_ISR_CAT2_ARRIVAL == osOptType)
    {
        Os_TmProtIsrFrameCounter(osIsrId, osOptType);
    }
    else
    {
        pIcb = &Os_ICB[osIsrId];

        pCbData = &(pIcb->osTpIsr[osOptType]);

        if (TRUE == pCbData->osIsTpStart)
        {
            pCbData->osTpTime += 1U;
            if (pCbData->osTpTime >= pCbData->osTpBudget)
            {
                pCbData->osTpTime    = 0u;
                pCbData->osIsTpStart = FALSE;

                /* Get error type. */
                if (TP_ISR_CAT2_EXE == osOptType)
                {
                    osErrType = E_OS_PROTECTION_TIME;
                }
                else
                {
                    osErrType = E_OS_PROTECTION_LOCKED;
                }
                /* TP_ISR_ARRIVAL and resource lock are not maintained
                 * in this function. */

#if ((OS_SC4 == CFG_SC) && (CFG_TRUSTED_SYSTEM_SERVICE_MAX > 0U))
                if (TRUE == Os_TrustedFuncTp.TrustedFuncTportDelayCall)
                {
                    /* Time protection and delay processing are triggered in the trusted function */

                    Os_TrustedFuncTp.TrustedFuncTporFlag    = TRUE;
                    Os_TrustedFuncTp.TrustedFuncTporErrType = osErrType;
                    Os_TrustedFuncTp.osWhoHook              = OS_TMPROT_HOOK_ISR;
                }
                else
#endif
                {
                    /* Hook. */
                    (void)Os_CallProtectionHook(osErrType, OS_TMPROT_HOOK_ISR);
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
/********************************************************************/
/*
 * Brief                <Timing protection end process of isr.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <osTaskId, osOptType>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_AppTerminateIsrKernelProc and so on>
 * REQ ID               <None>
 */
/********************************************************************/
void Os_TmProtIsrEnd(Os_IsrType osIsrId, Os_TmProtOptIsrType osOptType)
{
    Os_ICBType*         pIcb;
    Os_TmProtCbDataDef* pCbData;

    pIcb    = &Os_ICB[osIsrId];
    pCbData = &(pIcb->osTpIsr[osOptType]);

    if (TRUE == pCbData->osIsTpStart)
    {
        pCbData->osIsTpStart = FALSE;
        pCbData->osTpTime    = 0u;
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (CFG_STD_RESOURCE_MAX > 0)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Init resource lock budget for isr.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <None>
 * param-eventId[in]    <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_InitIsrTmProt>
 * REQ ID               <None>
 */
/********************************************************************/
static void Os_TmProtInitIsrResLock(Os_IsrType osIsrId, const Os_IsrCfgType* pIsrCfg)
{
    uint16                   i;
    uint32                   osResCnt;
    const Os_RCBType*        pRcb;
    const Os_IsrResLockType* pResLock;

    /* If Isr cfg OsIsrResourceLock, read budget to RCB. */
    if (NULL_PTR != pIsrCfg->OsIsrTimePt)
    {
        osResCnt = pIsrCfg->OsIsrTimePt->osIsrResLockCnt;

        for (i = 0u; i < osResCnt; i++)
        {
            /* Misrac: Msg(2:0491) Array subscripting applied to an
             * object of pointer type.
             * Note: osIsrResLockRef is array. osIsrResLockCnt is variable,
             * so osIsrResLockRef must be used like below.
             */
            pResLock = &pIsrCfg->OsIsrTimePt->osIsrResLockRef[i];

            /* PRQA S 3469 ++ */ /* VL_Os_3469 */
            pRcb = &Os_RCB[Os_GetObjLocalId(pResLock->osIsrResLockResRef)];
            /* PRQA S 3469 -- */

            /* Read budget to RCB. */
            pRcb->osTmProtResBgtIsr[osIsrId] = pResLock->osIsrResLockBudget;
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* CFG_STD_RESOURCE_MAX > 0 */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Init timing protection for isr.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <None>
 * param-eventId[in]    <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_InitTmProt>
 * REQ ID               <None>
 */
/********************************************************************/
static void Os_InitIsrTmProt(
    Os_IsrType           osIsrId,
    const Os_IsrCfgType* pIsrCfg,
    Os_ICBType*          pIcb

)
{
    const Os_IsrTimingProtectionType* pIsrTimePt;

    pIcb->IsrC2IsrOpt      = TP_OPT_BUTT;
    pIcb->osIsrTpFrameflag = TRUE;

#if (CFG_STD_RESOURCE_MAX > 0U)
    pIcb->IsrC2ResCount = 0u;
#endif

    pIsrTimePt = pIsrCfg->OsIsrTimePt;

    /* Set budget of each type. */
    if (NULL_PTR == pIsrTimePt)
    {
        /* PRQA S 1258 ++ */ /* VL_Os_1258 */
        Os_TmProtInitCbData(&pIcb->osTpIsr[TP_ISR_CAT2_EXE], OS_TICK_INVALID);
        Os_TmProtInitCbData(&pIcb->osTpIsr[TP_ISR_CAT2_ARRIVAL], OS_TICK_INVALID);
        Os_TmProtInitCbData(&pIcb->osTpIsr[TP_ISR_CAT2_SUS_OS_INT], OS_TICK_INVALID);
        Os_TmProtInitCbData(&pIcb->osTpIsr[TP_ISR_CAT2_SUS_ALL_INT], OS_TICK_INVALID);
        Os_TmProtInitCbData(&pIcb->osTpIsr[TP_ISR_CAT2_DIS_ALL_INT], OS_TICK_INVALID);
        /* PRQA S 1258 -- */
    }
    else
    {
        Os_TmProtInitCbData(&pIcb->osTpIsr[TP_ISR_CAT2_EXE], pIsrTimePt->osIsrExeBudget);
        Os_TmProtInitCbData(&pIcb->osTpIsr[TP_ISR_CAT2_ARRIVAL], pIsrTimePt->osIsrTimeFrame);
        Os_TmProtInitCbData(&pIcb->osTpIsr[TP_ISR_CAT2_SUS_OS_INT], pIsrTimePt->osIsrOsIptLockBudget);
        Os_TmProtInitCbData(&pIcb->osTpIsr[TP_ISR_CAT2_SUS_ALL_INT], pIsrTimePt->osIsrAllIptLockBudget);
        Os_TmProtInitCbData(&pIcb->osTpIsr[TP_ISR_CAT2_DIS_ALL_INT], pIsrTimePt->osIsrAllIptLockBudget);
    }

/* Init resource lock budget for isr. */
#if (CFG_STD_RESOURCE_MAX > 0)
    Os_TmProtInitIsrResLock(osIsrId, pIsrCfg);
#endif

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* CFG_ISR2_MAX > 0U */

#if (CFG_STD_RESOURCE_MAX > 0)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Init resource lock budget for task.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <None>
 * param-eventId[in]    <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_InitTaskTmProt>
 * REQ ID               <None>
 */
/********************************************************************/
static void Os_TmProtInitTaskResLock(Os_TaskType osTaskId, const Os_TaskCfgType* pTaskCfg)
{
    uint16                    i;
    uint16                    osCnt;
    const Os_RCBType*         pRcb;
    const Os_TaskResLockType* pResLock;

    /* If task cfg OsTaskResourceLock, read budget to RCB. */
    if (NULL_PTR != pTaskCfg->osTaskTmProtCfgRef->osTaskResLockRef)
    {
        osCnt = pTaskCfg->osTaskTmProtCfgRef->osTaskResLockCnt;

        for (i = 0u; i < osCnt; i++)
        {
            /* Misrac: Msg(2:0491) Array subscripting applied to an object of
             * pointer type.
             * Note: osTaskResLockRef is array. osTaskResLockCnt is variable,
             * so osTaskResLockRef must be used like below. */
            pResLock = &pTaskCfg->osTaskTmProtCfgRef->osTaskResLockRef[i];

            /* PRQA S 3469 ++ */ /* VL_Os_3469 */
            pRcb = &Os_RCB[Os_GetObjLocalId(pResLock->OsTaskResLockResRef)];
            /* PRQA S 3469 -- */

            /* Read budget to RCB. */
            pRcb->osTmProtResBgtTask[osTaskId] = pResLock->OsTaskResLockBudget;
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* CFG_STD_RESOURCE_MAX > 0 */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Init timing protection for task.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <None>
 * param-eventId[in]    <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_InitTmProt>
 * REQ ID               <None>
 */
/********************************************************************/
static void Os_InitTaskTmProt(Os_TaskType osTaskId, const Os_TaskCfgType* pTaskCfg, Os_TCBType* pTcb)
{
    const Os_TaskTmProtCfgType* pTaskTmProtCfgRef;

    pTcb->taskIsrOpt      = TP_OPT_BUTT;
    pTcb->taskTpFrameflag = TRUE;

#if (CFG_STD_RESOURCE_MAX > 0U)
    pTcb->taskResCount = 0u;
#endif

    pTaskTmProtCfgRef = pTaskCfg->osTaskTmProtCfgRef;

    /* Set budget of each type. NULL_PTR means not configed. */
    if (NULL_PTR == pTaskTmProtCfgRef)
    {
        /* PRQA S 1258 ++ */ /* VL_Os_1258 */
        Os_TmProtInitCbData(&pTcb->osTpTask[TP_TASK_EXE], OS_TICK_INVALID);
        Os_TmProtInitCbData(&pTcb->osTpTask[TP_TASK_ARRIVAL], OS_TICK_INVALID);
        Os_TmProtInitCbData(&pTcb->osTpTask[TP_TASK_SUS_OS_INT], OS_TICK_INVALID);
        Os_TmProtInitCbData(&pTcb->osTpTask[TP_TASK_SUS_ALL_INT], OS_TICK_INVALID);
        Os_TmProtInitCbData(&pTcb->osTpTask[TP_TASK_DIS_ALL_INT], OS_TICK_INVALID);
        /* PRQA S 1258 -- */
    }
    else
    {
        Os_TmProtInitCbData(&pTcb->osTpTask[TP_TASK_EXE], pTaskTmProtCfgRef->osTaskExecutionBudget);
        Os_TmProtInitCbData(&pTcb->osTpTask[TP_TASK_ARRIVAL], pTaskTmProtCfgRef->osTaskTimeFrame);
        Os_TmProtInitCbData(&pTcb->osTpTask[TP_TASK_SUS_OS_INT], pTaskTmProtCfgRef->osTaskOsInterruptLockBudget);
        Os_TmProtInitCbData(&pTcb->osTpTask[TP_TASK_SUS_ALL_INT], pTaskTmProtCfgRef->osTaskAllInterruptLockBudget);
        Os_TmProtInitCbData(&pTcb->osTpTask[TP_TASK_DIS_ALL_INT], pTaskTmProtCfgRef->osTaskAllInterruptLockBudget);

/* Init resource lock budget for task. */
#if (CFG_STD_RESOURCE_MAX > 0)
        Os_TmProtInitTaskResLock(osTaskId, pTaskCfg);
#endif
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Init protection module.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <None>
 * param-eventId[in]    <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_InitSystem>
 * REQ ID               <None>
 */
/********************************************************************/
void Os_InitTmProt(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    uint16 i;

/* Init protection para for task. */
#if (CFG_TASK_MAX > 0)
    for (i = 0U; i < Os_SCB.sysTaskMax; i++)
    {
        Os_InitTaskTmProt((Os_TaskType)i, &Os_TaskCfg[i], &Os_TCB[i]);
    }
#endif

#if (CFG_ISR2_MAX > 0U)
    /* Init protection para for Isr-Cat2. */
    for (i = 0U; i < Os_CfgIsr2Max; i++)
    {
        Os_InitIsrTmProt((Os_IsrType)i, &Os_IsrCfg[i], &Os_ICB[i]);
    }
#endif

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (CFG_ISR2_MAX > 0U)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Timing Protection isr process.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <None>
 * param-eventId[in]    <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_TmProtMainProc>
 * REQ ID               <None>
 */
/********************************************************************/
static void Os_TmProtIsrProc(void)
{
    uint16               i;
    Os_IsrType           osIsrC2Id;
    const Os_IsrCfgType* pIsrCfg;
    const Os_SCBType*    pScb;

    pScb = &Os_SCB;

    /* IntNest: current_isr is isr for timing protection,
     * previous_isr is the target to protect. */
    if (Os_IntNestISR2 >= OS_TMPROT_TARGET_ISR_OFFSET)
    {
        osIsrC2Id = pScb->sysIsrNestQueue[Os_IntNestISR2 - OS_TMPROT_TARGET_ISR_OFFSET];
        if (osIsrC2Id < CFG_ISR_MAX)
        {
            pIsrCfg = &Os_IsrCfg[osIsrC2Id];
            if (OS_ISR_CATEGORY2 == pIsrCfg->OsIsrCatType)
            {
                Os_TmProtIsrCounter(osIsrC2Id, TP_ISR_CAT2_EXE);
                Os_TmProtIsrCounter(osIsrC2Id, TP_ISR_CAT2_SUS_OS_INT);
                Os_TmProtIsrCounter(osIsrC2Id, TP_ISR_CAT2_SUS_ALL_INT);
                Os_TmProtIsrCounter(osIsrC2Id, TP_ISR_CAT2_DIS_ALL_INT);
            }
        }
    }

/* IntNest: arrival time should be checked for each isr. */
#if (CFG_ISR2_MAX > 0U)
    for (i = (Os_CfgIsrMax - Os_CfgIsr2Max); i < Os_CfgIsr2Max; i++)
    {
        /* Current_isr is running isr, this isr is for timing counter.
         * It should not be protected. */
        if (i != pScb->sysRunningIsrCat2Id)
        {
            Os_TmProtIsrCounter(i, TP_ISR_CAT2_ARRIVAL);
        }
    }
#endif

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* CFG_ISR2_MAX > 0 */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Timing Protection task process.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <None>
 * param-eventId[in]    <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_TmProtMainProc>
 * REQ ID               <None>
 */
/********************************************************************/
static void Os_TmProtTaskProc(void)
{
    uint16           i;
    Os_TaskStateType osTaskState;

#if (CFG_TASK_MAX > 0)
    for (i = 0U; i < Os_SCB.sysTaskMax; i++)
    {
        /* Implementation-dependent: IDLE task should not be protected. */
        if (OS_TASK_IDLE != (Os_TaskType)i)
        {
            osTaskState = Os_TCB[i].taskState;

            if (TASK_STATE_RUNNING == osTaskState)
            {
                /* Exe_budget only checked in running state. */
                Os_TmProtTaskCounter((Os_TaskType)i, TP_TASK_EXE);
                Os_TmProtTaskCounter((Os_TaskType)i, TP_TASK_ARRIVAL);
                Os_TmProtTaskCounter((Os_TaskType)i, TP_TASK_SUS_OS_INT);
                Os_TmProtTaskCounter((Os_TaskType)i, TP_TASK_SUS_ALL_INT);
                Os_TmProtTaskCounter((Os_TaskType)i, TP_TASK_DIS_ALL_INT);
            }
            else
            {
                /* Time-frame is checked in running and ready states. */
                Os_TmProtTaskCounter((Os_TaskType)i, TP_TASK_ARRIVAL);
            }
        }
    }
#endif

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Timing Protection isr callback.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <None>
 * param-eventId[in]    <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <ISR>
 * REQ ID               <None>
 */
/********************************************************************/
void Os_TmProtMainProc(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    uint16 i;

    /* 1: Timing Protection for task. */
    Os_TmProtTaskProc();

/* 2: Timing Protection for Resource lock. */
#if (CFG_STD_RESOURCE_MAX > 0)
    for (i = 0u; i < Os_CfgResourceMax; i++)
    {
        Os_TmProtResCounter((ResourceType)i);
    }
#endif

/* 3: Timing Protection for Isr2. */
#if (CFG_ISR2_MAX > 0)
    Os_TmProtIsrProc();
#endif

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */

/*=======[E N D   O F   F I L E]==============================================*/
