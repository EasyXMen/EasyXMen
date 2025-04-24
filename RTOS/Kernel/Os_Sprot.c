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
**  FILENAME    :  Os_Sprot.c                                                 **
**                                                                            **
**  Created on  :                                                             **
**  Author      :  i-soft-os                                                  **
**  Vendor      :                                                             **
**  DESCRIPTION :  AutoSar Protection Managment                               **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Os_Internal.h"

/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/
#if (CFG_TRUSTED_SYSTEM_SERVICE_MAX > 0U)
#define OS_START_SEC_VAR_CLONE_16
#include "Os_MemMap.h"
static VAR(TrustedFunctionIndexType, OS_VAR) Os_CfgTrustedServiceMax;
#define OS_STOP_SEC_VAR_CLONE_16
#include "Os_MemMap.h"
#endif

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
static FUNC(void, OS_CODE) Os_SProTerminateTask(void);
#endif

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#if (CFG_TRUSTED_SYSTEM_SERVICE_MAX > 0U)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Init the TrustedFunction>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * PreCondition         <None>
 * CallByAPI            <Os_InitSystem>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_InitTrustedFunction(void)
{
    VAR(uint8, OS_VAR) i;
    VAR(uint16, OS_VAR) vCoreId = Os_SCB.sysCore;
    Os_TrustedFuncNest = 0;

    Os_TrustedFuncTp.TrustedFuncTportDelayCall = 0;
    Os_TrustedFuncTp.TrustedFuncTporFlag = FALSE;
    Os_TrustedFuncTp.TrustedFuncTporErrType = 0;
    Os_TrustedFuncTp.osWhoHook = 0;

    if (Os_CfgTrustedServiceMax_Inf[vCoreId] > 0U)
    {
        Os_TrustedFuncNestQueue = Os_TrustedFuncNestQueue_Inf[vCoreId];
        Os_CfgTrustedServiceMax = Os_CfgTrustedServiceMax_Inf[vCoreId];
        for (i = 0u; i < Os_CfgTrustedServiceMax; i++)
        {
            Os_TrustedFuncNestQueue[i] = INVALID_OSAPPLICATION;
        }
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Call Trusted Function>
 * Service ID           <0x02>
 * Sync/Async           <Depends on called function. If called function is synchronous then serviec
 *                       is synchronous.May cause reschduling>
 * Reentrancy           <Yes>
 * param-eventId[in]    <FunctionIndex>
 *                      <FunctionParams>
 * Param-Name[out]      <void>
 * Param-Name[in/out]   <None>
 * return               <E_OK>
 *                      <E_OS_SERVICEID>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/********************************************************************/
FUNC(StatusType, OS_CODE)
CallTrustedFunction(TrustedFunctionIndexType FunctionIndex, TrustedFunctionParameterRefType FunctionParams)
{
    OS_ENTER_KERNEL();
    OS_ARCH_DECLARE_CRITICAL();
    VAR(StatusType, OS_VAR) err = E_OK;
    VAR(ApplicationType, OS_VAR) ApplID;

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (NULL_PTR == FunctionParams)
    {
        err = E_OS_PARAM_POINTER;
    }
    else
#endif
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_CALL_TRUSTED_FUNCTION) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    /* PRQA S 0326 ++ */ /* MISRA Rule 11.6 */ /* OS_SPROT_TYPE_CAST_001 */
    else if (Os_AddressWritable((uint32)FunctionParams) != TRUE)
    /* PRQA S 0326 -- */ /* MISRA Rule 11.6 */
    {
        err = E_OS_ILLEGAL_ADDRESS;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        err = E_OS_DISABLEDINT;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
/* On a Multi-Core system, these trusted function calls from
 * one OS-Application to another are limited to the same core.*/
#if (OS_AUTOSAR_CORES > 1)
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SPROT_MACRO_TO_FUNCTION_004 */
        if (Os_SCB.sysCore != Os_GetObjCoreId(FunctionIndex))
        /* PRQA S 3469 --*/ /* MISRA Dir 4.9 */
        {
            err = E_OS_ACCESS;
        }
        else
#endif /* OS_AUTOSAR_CORES > 1 */
        {
            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SPROT_MACRO_TO_FUNCTION_004 */
            OS_ARCH_ENTRY_CRITICAL();
            /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
            /* Save Trusted Function host App ID. */
            Os_TrustedFuncNestQueue[Os_TrustedFuncNest] = (0x0F00u & FunctionIndex) >> 8u;
            ApplID = Os_TrustedFuncNestQueue[Os_TrustedFuncNest];
            /* Save Trusted Function time protection delay call flag*/
            Os_TrustedFuncTp.TrustedFuncTportDelayCall = Os_AppCfg[ApplID].OsTrustedApplicationDelayTimingViolationCall;
            Os_TrustedFuncNest++;
            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SPROT_MACRO_TO_FUNCTION_004 */
            OS_ARCH_EXIT_CRITICAL();
            /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */

            /* PRQA S 1338 ++ */ /* MISRA Dir 17.8  */ /* OS_SPROT_PARAMETER_MODIFIED_005 */
            FunctionIndex = 0x00FFU & FunctionIndex;
            /* PRQA S 1338 -- */ /* MISRA Dir 17.8  */
            /*OS292,CFG_MAX_SYSTEM_SERVICE is generate by tool*/
            if (FunctionIndex >= Os_CfgTrustedServiceMax)
            {
                err = E_OS_SERVICEID;
            }
            else
            {
                /*Os_TrustedServiceList is generate by tool*/
                (*(Os_TrustedServiceList[FunctionIndex]))(FunctionIndex, FunctionParams);
            }
            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SPROT_MACRO_TO_FUNCTION_004 */
            OS_ARCH_ENTRY_CRITICAL();
            /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
            Os_TrustedFuncNest--;
            Os_TrustedFuncNestQueue[Os_TrustedFuncNest] = INVALID_OSAPPLICATION;

            if (0U == Os_TrustedFuncNest)
            {
                Os_TrustedFuncTp.TrustedFuncTportDelayCall = FALSE;
            }
            else
            {
                /* Save the last trusted function time protection delay call flag */
                ApplID = Os_TrustedFuncNestQueue[Os_TrustedFuncNest - 1U];
                Os_TrustedFuncTp.TrustedFuncTportDelayCall =
                    Os_AppCfg[ApplID].OsTrustedApplicationDelayTimingViolationCall;
            }
            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SPROT_MACRO_TO_FUNCTION_004 */
            OS_ARCH_EXIT_CRITICAL();
            /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
        }
    }

    /* Time protection and delay processing are triggered in the trusted function */
    if ((TRUE == Os_TrustedFuncTp.TrustedFuncTporFlag) && (FALSE == Os_TrustedFuncTp.TrustedFuncTportDelayCall))
    {
        Os_TrustedFuncTp.TrustedFuncTporFlag = FALSE;
        Os_TrustedFuncTp.TrustedFuncTporErrType = 0;
        /* Hook. */
        (void)Os_CallProtectionHook(Os_TrustedFuncTp.TrustedFuncTporErrType, Os_TrustedFuncTp.osWhoHook);
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(
            OSError_Save_CallTrustedFunction(FunctionIndex, FunctionParams),
            OSServiceId_CallTrustedFunction,
            err);
    }
#endif

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA Dir 4.9 */ /* OS_SPROT_MACRO_TO_FUNCTION_004 */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* CFG_TRUSTED_SYSTEM_SERVICE_MAX > 0 */

/* Service protection */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <If interrupts are disabled/suspended, OS service should ignore>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <None>
 * param-eventId[in]    <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <E_OS_DISABLEDINT>
 * PreCondition         <None>
 * CallByAPI            <GetAlarm and so on>
 * REQ ID               <None>
 */
/********************************************************************/
FUNC(boolean, OS_CODE) Os_IgnoreService(void)
{
    VAR(boolean, OS_VAR) status = TRUE;
    /*OS093*/
    if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
    {
        if ((Os_SCB.sysRunningTCB->taskDisableAllCount > 0u) || (Os_SCB.sysRunningTCB->taskSuspendAllCount > 0u)
            || (Os_SCB.sysRunningTCB->taskSuspendOSCount > 0u))
        {
            status = FALSE; /*FALSE:ignore*/
        }
        else
        {
            status = TRUE;
        }
    }
    else if (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
    {
        if ((Os_ICB[Os_SCB.sysRunningIsrCat2Id].isrC2DisableAllCount > 0u)
            || (Os_ICB[Os_SCB.sysRunningIsrCat2Id].isrC2SuspendAllCount > 0u)
            || (Os_ICB[Os_SCB.sysRunningIsrCat2Id].isrC2SuspendOSCount > 0u))
        {
            status = FALSE;
        }
        else
        {
            status = TRUE;
        }
    }
    else
    {
        /* Intentionally Empty */
    }

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Indicate the address if writable by this OS-App>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <None>
 * param-eventId[in]    <Address>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <E_OS_ILLEGAL_ADDRESS>
 * PreCondition         <None>
 * CallByAPI            <GetAlarm and so on>
 * REQ ID               <None>
 */
/********************************************************************/
FUNC(boolean, OS_CODE) Os_AddressWritable(VAR(uint32, OS_VAR) Address)
{
    VAR(Os_TaskType, OS_VAR) TaskID = Os_SCB.sysRunningTaskID;
    VAR(Os_IsrType, OS_VAR) Isr2ID = Os_SCB.sysRunningIsrCat2Id;
    VAR(Os_ObjectTypeType, OS_VAR) ObjType = Os_SCB.sysRunningAppObj;
    VAR(AccessType, OS_VAR) Access = (AccessType)NO_PERMISSION;
    VAR(Os_CoreIdType, OS_VAR) coreId = Os_SCB.sysCore;
    VAR(Os_CoreIdType, OS_VAR) coreIndex = coreId << 12;
    VAR(boolean, OS_VAR) status = TRUE;

    /*OS051*/
    /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */ /* OS_SPROT_TYPE_CAST_002 */
    if (OBJECT_TASK == ObjType)
    {
        if (((uint32)Os_TaskStack[TaskID].stackTop >= Address) && ((uint32)Os_TaskStack[TaskID].stackBottom <= Address))
        {
            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SPROT_MACRO_TO_FUNCTION_004 */
            Access = OSMEMORY_SET_WRITEABLE(Access);
            /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
        }
        else
        {
            Access = Os_CheckTaskMemoryAccess((coreIndex | TaskID), Address, 0);
        }
    }
    else if (OBJECT_ISR == ObjType)
    {
        if (((uint32)Os_ISR2Stack[Isr2ID].stackTop >= Address) && ((uint32)Os_ISR2Stack[Isr2ID].stackBottom <= Address))
        {
            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SPROT_MACRO_TO_FUNCTION_004 */
            Access = OSMEMORY_SET_WRITEABLE(Access);
            /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
        }
        else
        {
            Access = Os_CheckISRMemoryAccess((coreIndex | Isr2ID), Address, 0);
        }
    }
    else
    {
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SPROT_MACRO_TO_FUNCTION_004 */
        Access = OSMEMORY_SET_WRITEABLE(Access);
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
    }
    /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */

    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SPROT_MACRO_TO_FUNCTION_004 */
    if (0U != OSMEMORY_IS_WRITEABLE(Access))
    /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
    {
        status = TRUE;
    }
    else
    {
        status = FALSE;
    }

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Indicate the service if in right context>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <None>
 * param-eventId[in]    <AllowedContext>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <E_OS_DISABLEDINT>
 * PreCondition         <None>
 * CallByAPI            <GetAlarm and so on>
 * REQ ID               <None>
 */
/********************************************************************/
FUNC(boolean, OS_CODE) Os_WrongContext(uint16 AllowedContext)
{
    VAR(uint16, OS_VAR) Os_LevelBit;
    VAR(boolean, OS_VAR) status = FALSE;

    switch (Os_SCB.sysOsLevel)
    {
    case OS_LEVEL_ERRORHOOK_APP:
        Os_LevelBit = OS_LEVEL_BIT_ERRORHOOK_APP;
        break;

    case OS_LEVEL_TASK:
        Os_LevelBit = OS_LEVEL_BIT_TASK;
        break;

    case OS_LEVEL_ISR1:
        Os_LevelBit = OS_LEVEL_BIT_ISR1;
        break;

    case OS_LEVEL_ISR2:
        Os_LevelBit = OS_LEVEL_BIT_ISR2;
        break;

    case OS_LEVEL_ERRORHOOK:
        Os_LevelBit = OS_LEVEL_BIT_ERRORHOOK;
        break;

    case OS_LEVEL_PRETASKHOOK:
        Os_LevelBit = OS_LEVEL_BIT_PRETASKHOOK;
        break;

    case OS_LEVEL_POSTTASKHOOK:
        Os_LevelBit = OS_LEVEL_BIT_POSTTASKHOOK;
        break;

    case OS_LEVEL_STARTUPHOOK:
        Os_LevelBit = OS_LEVEL_BIT_STARTUPHOOK;
        break;

    case OS_LEVEL_SHUTDOWNHOOK:
        Os_LevelBit = OS_LEVEL_BIT_SHUTDOWNHOOK;
        break;

    case OS_LEVEL_ALARMCALLBACK:
        Os_LevelBit = OS_LEVEL_BIT_ALARMCALLBACK;
        break;

    case OS_LEVEL_PROTECTIONHOOK:
        Os_LevelBit = OS_LEVEL_BIT_PROTECTIONHOOK;
        break;

    default:
        Os_LevelBit = (uint16)OS_LEVEL_MAIN;
        break;
    }

    if ((uint16)OS_LEVEL_MAIN == Os_LevelBit)
    {
        if ((OS_CONTEXT_START_CORE == AllowedContext) || (OS_CONTEXT_START_NON_AUTOSAR_CORE == AllowedContext))
        {
            status = TRUE;
        }
    }

    if ((boolean)FALSE == status)
    {
        /*OS088*/
        if ((Os_LevelBit & AllowedContext) > 0U)
        {
            status = TRUE;
        }
        else
        {
            status = FALSE; /*FALSE:Wrong context*/
        }
    }

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Service protection terminates the task>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <None>
 * param-eventId[in]    <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_TaskEndNoTerminate>
 * REQ ID               <None>
 */
/********************************************************************/
static FUNC(void, OS_CODE) Os_SProTerminateTask(void)
{
/*Reference to current Task CB*/
#if (CFG_STD_RESOURCE_MAX > 0U)
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SPROT_COMPILER_003 */
    P2VAR(Os_TCBType const, AUTOMATIC, OS_VAR) pCurTaskTcb;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    VAR(Os_ResourceType, OS_VAR) taskResCountTmp;
    VAR(Os_CoreIdType, OS_VAR) coreId;
    VAR(Os_CoreIdType, OS_VAR) coreIndex;
#endif

    OS_ARCH_DECLARE_CRITICAL();

#if (CFG_STD_RESOURCE_MAX > 0U)
    pCurTaskTcb = Os_SCB.sysRunningTCB;
#endif

#if (CFG_STD_RESOURCE_MAX > 0U)
    taskResCountTmp = pCurTaskTcb->taskResCount;
    if (taskResCountTmp > 0U)
    {
        Os_ResourceType OsResLoopi;

        Os_SCB.sysDispatchLocker = Os_SCB.sysDispatchLocker + 1u;

        coreId = Os_SCB.sysCore;
        coreIndex = coreId << 12;
        for (OsResLoopi = taskResCountTmp; OsResLoopi > 0U; OsResLoopi--)
        {
            (void)Os_ReleaseResource(coreIndex | pCurTaskTcb->taskResourceStack[OsResLoopi - 1U]);
        }
    }
#endif /* CFG_STD_RESOURCE_MAX > 0U */

    OS_ARCH_ENTRY_CRITICAL(); /* PRQA S 3469 */ /* MISRA Dir 4.9 */ /* OS_SPROT_MACRO_TO_FUNCTION_004*/
    Os_PostTaskHook(); /* PRQA S 3469 */ /* MISRA Dir 4.9 */        /* OS_SPROT_MACRO_TO_FUNCTION_004 */

#if (CFG_INTERNAL_RESOURCE_MAX > 0U)
    Os_ReleaseInternalResource(Os_SCB.sysRunningTaskID);
#endif

#if ((OS_BCC1 == CFG_CC) || (OS_ECC1 == CFG_CC))
    Os_SCB.sysRunningTCB->taskState = TASK_STATE_SUSPENDED;
#else
    if (Os_SCB.sysRunningTCB->taskActCount > 0U)
    {
        Os_SCB.sysRunningTCB->taskActCount = Os_SCB.sysRunningTCB->taskActCount - 1U;
    }
    if (Os_SCB.sysRunningTCB->taskSelfActCount > 0U)
    {
        Os_SCB.sysRunningTCB->taskSelfActCount = Os_SCB.sysRunningTCB->taskSelfActCount - 1U;
    }
    if (Os_SCB.sysRunningTCB->taskActCount > 0U)
    {
        Os_SCB.sysRunningTCB->taskState = TASK_STATE_START;
    }
    else
    {
        Os_SCB.sysRunningTCB->taskState = TASK_STATE_SUSPENDED;
    }
#endif /* OS_BCC1 == CFG_CC || OS_ECC1 == CFG_CC */

    Os_ReadyQueueRemove(OS_LEVEL_TASK, Os_TCB[Os_SCB.sysRunningTaskID].taskRunPrio);

#if (CFG_STD_RESOURCE_MAX > 0U)
    Os_SCB.sysRunningTCB->taskResCount = 0U;
#endif

    Os_SCB.sysRunningTCB->taskRunPrio = Os_TaskCfg[Os_SCB.sysRunningTaskID].osTaskPriority;

#if (CFG_EXTENDED_TASK_MAX > 0U)
    if (Os_SCB.sysRunningTaskID < Os_CfgExtendTaskMax)
    {
        Os_ECB[Os_SCB.sysRunningTaskID].eventSetEvent = 0U;
        Os_ECB[Os_SCB.sysRunningTaskID].eventWaitEvent = 0U;
    }
#endif /* CFG_EXTENDED_TASK_MAX > 0U */

    if (Os_SCB.sysHighTaskID == Os_SCB.sysRunningTaskID)
    {
        Os_SCB.sysHighPrio = Os_GetHighPrio();
        Os_SCB.sysHighTaskID = Os_ReadyQueueGetFirst(Os_SCB.sysHighPrio);
    }

/* Timing protection: reset task exe time. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    Os_TmProtTaskEnd(Os_SCB.sysRunningTaskID, TP_TASK_EXE);
#endif
    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SPROT_MACRO_TO_FUNCTION_004 */
    Os_ErrorHook(E_OS_MISSINGEND);

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceTaskSwitch(
        Os_SCB.sysRunningTaskID,
        Os_SCB.sysHighTaskID,
        OS_TRACE_TASK_SWITCH_REASON_SPROT_ERROR_TERMINATE,
        OS_TRACE_TASK_SWITCH_REASON_SPROT_ERROR_ACTIVE);
#endif

    Os_SCB.sysDispatchLocker = 0U;
    Os_Dispatch();
    OS_ARCH_EXIT_CRITICAL();
    /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Task ends without calling a TerminateTask or ChainTask>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <None>
 * param-eventId[in]    <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * PreCondition         <None>
 * CallByAPI            <Os_TaskErrBack>
 * REQ ID               <None>
 */
/********************************************************************/
FUNC(void, OS_CODE) Os_TaskEndNoTerminate(void)
{
#if (TRUE == CFG_TASK_RESPONSE_TIME_ENABLE)
    Os_TaskRecordTotalTick(Os_SCB.sysRunningTaskID);
#endif /* TRUE == CFG_TASK_RESPONSE_TIME_ENABLE */

/*OS239*/
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    Os_ArchEnableAllInt_ButTimingProtInt();
#else
    Os_ArchEnableInt();
#endif

    /*OS052,OS070*/
    Os_SProTerminateTask();
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <ISR2 ends with locked interrupts or allocated
 *                       resources>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <None>
 * param-eventId[in]    <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/********************************************************************/
FUNC(void, OS_CODE) Os_Isr2OccupyIntRes(void)
{
#if ((CFG_STD_RESOURCE_MAX > 0U) || (CFG_SPINLOCK_MAX > 0U))
    VAR(Os_CoreIdType, OS_VAR) coreId = Os_SCB.sysCore;
    VAR(Os_CoreIdType, OS_VAR) coreIndex = coreId << 12;
#endif

    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SPROT_COMPILER_003 */
    /* PRQA S 3678 ++ */ /* MISRA Rule 8.13 */ /* OS_SPROT_LACK_CONST_001 */
    P2VAR(Os_ICBType, AUTOMATIC, OS_VAR) pOsICB = &Os_ICB[Os_SCB.sysRunningIsrCat2Id];
    /* PRQA S 3678 -- */ /* MISRA Rule-8.13*/
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

#if (CFG_SPINLOCK_MAX > 0U)
    VAR(uint16, OS_VAR) CountIdx;
    VAR(uint16, OS_VAR) CurrentCount;
    VAR(SpinlockIdType, OS_VAR) SpinlockId;
    VAR(boolean, OS_VAR) SpinlockFlag = FALSE;
#if (CFG_STD_RESOURCE_MAX > 0U)
    VAR(ResourceType, OS_VAR) ResourceId;
    VAR(boolean, OS_VAR) ResouceFlag = FALSE;
#endif

    for (CountIdx = pOsICB->isr2CriticalZoneCount; CountIdx > 0u; CountIdx--)
    {
        CurrentCount = pOsICB->isr2CriticalZoneCount - 1u;
        if (OBJECT_SPINLOCK == pOsICB->isr2CriticalZoneType[CurrentCount])
        {
            SpinlockId = pOsICB->isr2CriticalZoneStack[CurrentCount];
            (void)Os_ReleaseSpinlock(SpinlockId);
            SpinlockFlag = TRUE;
        }
#if (CFG_STD_RESOURCE_MAX > 0U)
        else
        {
            ResourceId = (coreIndex | pOsICB->isr2CriticalZoneStack[CurrentCount]);
            (void)Os_ReleaseResource(ResourceId);
            ResouceFlag = TRUE;
        }
#endif
    }

    if (TRUE == SpinlockFlag)
    {
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SPROT_MACRO_TO_FUNCTION_004 */
        Os_ErrorHook(E_OS_SPINLOCK);
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
    }
#if (CFG_STD_RESOURCE_MAX > 0U)
    if (TRUE == ResouceFlag)
    {
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SPROT_MACRO_TO_FUNCTION_004 */
        Os_ErrorHook(E_OS_RESOURCE);
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
    }
#endif

#elif (CFG_STD_RESOURCE_MAX > 0U)
    /*OS369*/
    /* free the resources occupied on the ISRs*/
    if (pOsICB->IsrC2ResCount > 0u)
    {
        VAR(uint16, OS_VAR) i;
        for (i = pOsICB->IsrC2ResCount; i > 0u; i--)
        {
            (void)Os_ReleaseResource(coreIndex | pOsICB->IsrC2ResourceStack[i - 1u]);
        }
        pOsICB->IsrC2ResCount = 0u;

        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SPROT_MACRO_TO_FUNCTION_004 */
        Os_ErrorHook(E_OS_RESOURCE);
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
    }
#endif

    /*OS368*/
    if ((pOsICB->isrC2DisableAllCount > 0U) || (pOsICB->isrC2SuspendAllCount > 0U)
        || (pOsICB->isrC2SuspendOSCount > 0U))
    {
        if (pOsICB->isrC2DisableAllCount > 0U)
        {
            Os_EnableAllInterrupts();
        }
        while (pOsICB->isrC2SuspendAllCount > 0U)
        {
            Os_ResumeAllInterrupts();
        }
        while (pOsICB->isrC2SuspendOSCount > 0U)
        {
            Os_ResumeOSInterrupts();
        }

        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SPROT_MACRO_TO_FUNCTION_004 */
        Os_ErrorHook(E_OS_DISABLEDINT);
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Check object access to the calling Task/Isr2>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <None>
 * param-eventId[in]    <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * PreCondition         <None>
 * CallByAPI            <GetAlarm>
 * REQ ID               <None>
 */
/********************************************************************/
FUNC(boolean, OS_CODE) Os_CheckObjAcs(ObjectTypeType ObjectType, Os_AppObjectId ObjectID)
{
    VAR(boolean, OS_VAR) status = TRUE;

    /*OS056*/
    if ((OS_LEVEL_TASK == Os_SCB.sysOsLevel) || (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel))
    {
        if (NO_ACCESS == Os_CheckObjectAccess(Os_SCB.sysRunningAppID, ObjectType, ObjectID))
        {
            status = FALSE; /*no access*/
        }
        else
        {
            status = TRUE;
        }
    }

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
#endif /* OS_SC3 == CFG_SC || OS_SC4 == CFG_SC */

/*=======[E N D   O F   F I L E]==============================================*/
