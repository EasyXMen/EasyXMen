/**
 * Copyright (C) 2008-2025 isoft Infrastructure Software Co., Ltd.
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
 ********************************************************************************
 **                                                                            **
 **  FILENAME    : Os_Hook.c                                                   **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      : i-soft-os                                                   **
 **  Vendor      :                                                             **
 **  DESCRIPTION : hook manager                                                **
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

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/

#if (TRUE == CFG_PROTECTIONHOOK)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <check the running task ID is legal,
 *                          running task ID < idle task ID
 *                          running Isr2 ID < CFG_ISR2_MAX>
 * Service ID   :       <Check whether the running task and interrupt ID are legitimate>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_CallProtectionHook>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3450 ++ */ /* VL_Os_3450 */
static StatusType Os_CheckRunningTaskIDandIsr2ID(uint32 osWhoHook)
/* PRQA S 3450 -- */
{
    StatusType err = E_NOT_OK;

    if (OS_TMPROT_HOOK_TASK == osWhoHook)
    {
        if (Os_SCB.sysRunningTaskID < OS_TASK_IDLE)
        {
            err = E_OK;
        }
        else
        {
            err = E_NOT_OK;
        }
    }

#if (CFG_ISR2_MAX > 0)
    if (OS_TMPROT_HOOK_ISR == osWhoHook)
    {
        if (Os_SCB.sysRunningIsrCat2Id < Os_CfgIsr2Max)
        {
            err = E_OK;
        }
        else
        {
            err = E_NOT_OK;
        }
    }
#endif

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_PROTECTIONHOOK */

/* Attention: this function is only called in os_kernel. Usr_app or test_case_app
 * can not call this function directly. */
#if ((TRUE == CFG_ERRORHOOK) || (TRUE == CFG_APPL_ERRORHOOK))
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Call the error hook function>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <Error>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <TerminateApplication and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_CallErrorHook(StatusType Error)
{
    Os_CallLevelType osSameLevel;
#if (CFG_OSAPPLICATION_MAX > 0U)
    Os_ApplicationType bakAppId;
#endif
    OS_ARCH_DECLARE_CRITICAL();
#if (TRUE == CFG_APPL_ERRORHOOK)
    const Os_ApplicationCfgType* pAppCfg;
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceLastError(Error);
#endif /* TRUE == CFG_TRACE_ENABLE */

    if (OS_LEVEL_ERRORHOOK == Os_SCB.sysOsLevel)
    {
        /*nothing to do*/
    }
    else
    {
        OS_ARCH_ENTRY_CRITICAL();
/*Triggering the error hook is Task or Isr,then update the running ApplID and Object*/
#if (CFG_OSAPPLICATION_MAX > 0U)
        bakAppId = Os_SCB.sysRunningAppID;
        if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
        {
            Os_SCB.sysRunningAppID  = Os_ObjectAppCfg[OBJECT_TASK][Os_SCB.sysRunningTaskID].hostApp;
            Os_SCB.sysRunningAppObj = OBJECT_TASK;
        }
        if (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
        {
            Os_SCB.sysRunningAppID  = Os_ObjectAppCfg[OBJECT_ISR][Os_SCB.sysRunningIsrCat2Id].hostApp;
            Os_SCB.sysRunningAppObj = OBJECT_ISR;
        }
#endif

        osSameLevel       = Os_SCB.sysOsLevel;
        Os_SCB.sysOsLevel = OS_LEVEL_ERRORHOOK;

#if (TRUE == CFG_ERRORHOOK)
        ErrorHook(Error);
#endif

/* If cfg, call application_specific hook. */
#if (TRUE == CFG_APPL_ERRORHOOK)
        pAppCfg = &Os_AppCfg[Os_SCB.sysRunningAppID];
        if (NULL_PTR != pAppCfg->OsApplicationHooks.OsAppErrorHook)
        {
            Os_SCB.sysOsLevel = OS_LEVEL_ERRORHOOK_APP;
            pAppCfg->OsApplicationHooks.OsAppErrorHook(Error);
        }
#endif

        Os_SCB.sysOsLevel = osSameLevel;
#if (CFG_OSAPPLICATION_MAX > 0U)
        Os_SCB.sysRunningAppID = bakAppId;
#endif
        OS_ARCH_EXIT_CRITICAL();
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_ERRORHOOK || TRUE == CFG_APPL_ERRORHOOK */

#if (TRUE == CFG_PRETASKHOOK)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Call the PreTask hook function>
 * Service ID           <none>
 * Sync/Async           <none>
 * Reentrancy           <none>
 * param-eventId[in]    <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_SwitchTask>
 * REQ ID               <None>
 *
 */
/********************************************************************/
void Os_CallPreTaskHook(void)
{
    Os_CallLevelType osSameLevel;
#if (CFG_OSAPPLICATION_MAX > 0U)
    Os_ApplicationType bakAppId;
#endif
    OS_ARCH_DECLARE_CRITICAL();

    OS_ARCH_ENTRY_CRITICAL();
/*if task trigger the pretask hook ,update the running ApplID and Object*/
#if (CFG_OSAPPLICATION_MAX > 0U)
    if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
    {
        bakAppId = Os_SCB.sysRunningAppID;
        Os_SCB.sysRunningAppID  = Os_ObjectAppCfg[OBJECT_TASK][Os_SCB.sysRunningTaskID].hostApp;
        Os_SCB.sysRunningAppObj = OBJECT_TASK;
    }
#endif

    osSameLevel       = Os_SCB.sysOsLevel;
    Os_SCB.sysOsLevel = OS_LEVEL_PRETASKHOOK;
    PreTaskHook();
    Os_SCB.sysOsLevel = osSameLevel;
#if (CFG_OSAPPLICATION_MAX > 0U)
    if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
    {
        Os_SCB.sysRunningAppID = bakAppId;
    }
#endif
    OS_ARCH_EXIT_CRITICAL();
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_PRETASKHOOK */

#if (TRUE == CFG_POSTTASKHOOK)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Call the Post Task hook function>
 *
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <WaitEvent and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_CallPostTaskHook(void)
{
    Os_CallLevelType osSameLevel;
#if (CFG_OSAPPLICATION_MAX > 0U)
    Os_ApplicationType bakAppId;
#endif
    OS_ARCH_DECLARE_CRITICAL();

    OS_ARCH_ENTRY_CRITICAL();
/*if task trigger the posttask hook ,update the running ApplID and Object*/
#if (CFG_OSAPPLICATION_MAX > 0U)
    if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
    {
        bakAppId = Os_SCB.sysRunningAppID;
        Os_SCB.sysRunningAppID  = Os_ObjectAppCfg[OBJECT_TASK][Os_SCB.sysRunningTaskID].hostApp;
        Os_SCB.sysRunningAppObj = OBJECT_TASK;
    }
#endif

    osSameLevel       = Os_SCB.sysOsLevel;
    Os_SCB.sysOsLevel = OS_LEVEL_POSTTASKHOOK;
    PostTaskHook();
    Os_SCB.sysOsLevel = osSameLevel;
#if (CFG_OSAPPLICATION_MAX > 0U)
    if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
    {
        Os_SCB.sysRunningAppID = bakAppId;
    }
#endif
    OS_ARCH_EXIT_CRITICAL();
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_POSTTASKHOOK */

#if (TRUE == CFG_STARTUPHOOK)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Call the Startup hook function>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <StartOS>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_CallStartupHook(void)
{
    Os_CallLevelType osSameLevel;

    osSameLevel       = Os_SCB.sysOsLevel;
    Os_SCB.sysOsLevel = OS_LEVEL_STARTUPHOOK;
    StartupHook();
    Os_SCB.sysOsLevel = osSameLevel;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_STARTUPHOOK */

#if (TRUE == CFG_SHUTDOWNHOOK)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Call the Shutdown hook function>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <Error>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <ShutdownOS and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_CallShutdownHook(StatusType Error)
{
    Os_CallLevelType osSameLevel;
#if (CFG_OSAPPLICATION_MAX > 0U)
    Os_ApplicationType bakAppId;
#endif
    OS_ARCH_DECLARE_CRITICAL();

    OS_ARCH_ENTRY_CRITICAL();
/*Triggering the shutdown hook is Task or Isr,then update the running ApplID and Object*/
#if (CFG_OSAPPLICATION_MAX > 0U)
    bakAppId = Os_SCB.sysRunningAppID;
    if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
    {
        Os_SCB.sysRunningAppID  = Os_ObjectAppCfg[OBJECT_TASK][Os_SCB.sysRunningTaskID].hostApp;
        Os_SCB.sysRunningAppObj = OBJECT_TASK;
    }
    else if (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
    {
        Os_SCB.sysRunningAppID  = Os_ObjectAppCfg[OBJECT_ISR][Os_SCB.sysRunningIsrCat2Id].hostApp;
        Os_SCB.sysRunningAppObj = OBJECT_ISR;
    }
    else if (OS_LEVEL_ISR1 == Os_SCB.sysOsLevel)
    {
        Os_SCB.sysRunningAppID =
            Os_ObjectAppCfg[OBJECT_ISR][Os_SCB.sysIsrNestQueue[Os_IntNestISR2 + Os_IntNestISR1]].hostApp;
        Os_SCB.sysRunningAppObj = OBJECT_ISR;
    }
    else
    {
        /* Nothing to do. */
    }
#endif /* CFG_OSAPPLICATION_MAX > 0U */

    osSameLevel       = Os_SCB.sysOsLevel;
    Os_SCB.sysOsLevel = OS_LEVEL_SHUTDOWNHOOK;
    ShutdownHook(Error);
    Os_SCB.sysOsLevel = osSameLevel;
#if (CFG_OSAPPLICATION_MAX > 0U)
    Os_SCB.sysRunningAppID = bakAppId;
#endif
    OS_ARCH_EXIT_CRITICAL();
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_SHUTDOWNHOOK */

/* Application specific hook. Ref: 8.5. */
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_APPL_STARTUPHOOK)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Call Appl's startup hook function>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <StartOS>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_ApplStartupHook(void)
{
    Os_ApplicationType           i;
    Os_CallLevelType             osSameLevel;
    const Os_ApplicationCfgType* pAppCfg;

    osSameLevel = Os_SCB.sysOsLevel;

    Os_SCB.sysOsLevel = OS_LEVEL_STARTUPHOOK;
    /* Loop each application, if cfg startuphook, then call it. */
    for (i = 0u; i < (CFG_OSAPPLICATION_MAX - 1u); i++)
    {
        pAppCfg = &Os_AppCfg[i];

        if (NULL_PTR != pAppCfg->OsApplicationHooks.OsAppStartupHook)
        {
            if (pAppCfg->OsHostCore == Os_SCB.sysCore)
            {
                pAppCfg->OsApplicationHooks.OsAppStartupHook();
            }
        }
    }
    Os_SCB.sysRunningAppID  = INVALID_OSAPPLICATION;
    Os_SCB.sysRunningAppObj = OBJECT_MAX;

    Os_SCB.sysOsLevel = osSameLevel;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_APPL_STARTUPHOOK */

#if (TRUE == CFG_APPL_SHUTDOWNHOOK)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Call Appl's Shutdown hook function>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <Fatalerror>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <ShutdownOS and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_ApplShutdownHook(StatusType Fatalerror)
{
    Os_ApplicationType           i;
    Os_ApplicationType           bakAppId;
    Os_CallLevelType             osSameLevel;
    const Os_ApplicationCfgType* pAppCfg;
    OS_ARCH_DECLARE_CRITICAL();

    bakAppId = Os_SCB.sysRunningAppID;

    /*Triggering the shutdown hook is Task or Isr,then update the running ApplID and Object*/
    if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
    {
        Os_SCB.sysRunningAppID  = Os_ObjectAppCfg[OBJECT_TASK][Os_SCB.sysRunningTaskID].hostApp;
        Os_SCB.sysRunningAppObj = OBJECT_TASK;
    }
    else if (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
    {
        Os_SCB.sysRunningAppID  = Os_ObjectAppCfg[OBJECT_ISR][Os_SCB.sysRunningIsrCat2Id].hostApp;
        Os_SCB.sysRunningAppObj = OBJECT_ISR;
    }
    else if (OS_LEVEL_ISR1 == Os_SCB.sysOsLevel)
    {
        Os_SCB.sysRunningAppID =
            Os_ObjectAppCfg[OBJECT_ISR][Os_SCB.sysIsrNestQueue[Os_IntNestISR2 + Os_IntNestISR1]].hostApp;
        Os_SCB.sysRunningAppObj = OBJECT_ISR;
    }
    else
    {
        /* Nothing to do. */
    }

    osSameLevel       = Os_SCB.sysOsLevel;
    Os_SCB.sysOsLevel = OS_LEVEL_SHUTDOWNHOOK;

    /* Loop each application, if cfg shutdownhook, then call it. */
    for (i = 0u; i < (CFG_OSAPPLICATION_MAX - 1u); i++)
    {
        pAppCfg = &Os_AppCfg[i];

        if (NULL_PTR != pAppCfg->OsApplicationHooks.OsAppShutdownHook)
        {
            OS_ARCH_ENTRY_CRITICAL();
            pAppCfg->OsApplicationHooks.OsAppShutdownHook(Fatalerror);
            OS_ARCH_EXIT_CRITICAL();
        }
    }

    Os_SCB.sysOsLevel = osSameLevel;
    Os_SCB.sysRunningAppID = bakAppId;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_APPL_SHUTDOWNHOOK */
#endif /* OS_SC3 == CFG_SC || OS_SC4 == CFG_SC */

/* Protection Hook. */
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_PROTECTIONHOOK)
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Resume isr operation in protection hook.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <osOpt>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ProtHookTerminateTask>
 * REQ ID               <None>
 */
/********************************************************************/
static void Os_ProtHookTaskIsrOpt(Os_TaskIsrOptType osOpt)
{
    Os_TaskType osRunningTaskID;

    Os_TCBType* pTcb;

    osRunningTaskID = Os_SCB.sysRunningTaskID;
    pTcb            = &Os_TCB[osRunningTaskID];
    switch (osOpt)
    {
    case TP_OPT_BUTT:
        /* Nothing to do. */
        break;

    /* SuspendOSInterrupts */
    case TP_SUS_OS_INT:
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
        if (pTcb->taskSuspendOSCount > 0u)
        {
            pTcb->taskSuspendOSCount = 0;
        }
#endif

        Os_TCB[Os_SCB.sysRunningTaskID].taskIsrOpt = TP_OPT_BUTT;
        Os_TmProtTaskEnd(osRunningTaskID, TP_TASK_SUS_OS_INT);
        Os_SuspendOsCount = 0;
        Os_ArchSetIpl(Os_SaveOsIntNested, OS_ISR_ENABLE);
        break;

    /* SuspendAllInterrupts */
    case TP_SUS_ALL_INT:
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
        if (pTcb->taskSuspendAllCount > 0u)
        {
            pTcb->taskSuspendAllCount = 0;
        }
#endif

        Os_TCB[Os_SCB.sysRunningTaskID].taskIsrOpt = TP_OPT_BUTT;
        Os_TmProtTaskEnd(osRunningTaskID, TP_TASK_SUS_ALL_INT);
        Os_SuspendAllCount = 0;
        Os_ArchEnableAllInt_ButTimingProtInt();
        break;

    /* DisableAllInterrupts */
    case TP_DIS_ALL_INT:
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
        if (pTcb->taskDisableAllCount > 0u)
        {
            pTcb->taskDisableAllCount = 0;
        }
#endif

        Os_TCB[Os_SCB.sysRunningTaskID].taskIsrOpt = TP_OPT_BUTT;
        Os_TmProtTaskEnd(osRunningTaskID, TP_TASK_DIS_ALL_INT);
        Os_ArchEnableAllInt_ButTimingProtInt();
        break;

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
/********************************************************************/
/*
 * Brief                <Resume isr operation in protection hook.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <osOpt>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ProtHookTerminateIsrC2>
 * REQ ID               <None>
 */
/********************************************************************/
static void Os_ProtHookIsr2IsrOpt(Os_TaskIsrOptType osOpt)
{
    Os_ICBType* pOsICB;
    Os_IsrType  OsIsrID;

    OsIsrID = Os_SCB.sysIsrNestQueue[Os_IntNestISR2 - 2U];
    pOsICB  = &Os_ICB[OsIsrID];
    switch (osOpt)
    {
    case TP_OPT_BUTT:
        /* Nothing to do. */
        break;

    /* SuspendOSInterrupts */
    case TP_SUS_OS_INT:
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
        if (pOsICB->isrC2SuspendOSCount > 0u)
        {
            pOsICB->isrC2SuspendOSCount = 0;
        }
#endif

        Os_ICB[Os_SCB.sysRunningIsrCat2Id].IsrC2IsrOpt = TP_OPT_BUTT;
        Os_TmProtIsrEnd(OsIsrID, TP_ISR_CAT2_SUS_OS_INT);
        Os_SuspendOsCount = 0;
        Os_ArchSetIpl(Os_SaveOsIntNested, OS_ISR_ENABLE);
        break;

    /* SuspendAllInterrupts */
    case TP_SUS_ALL_INT:
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
        if (pOsICB->isrC2SuspendAllCount > 0u)
        {
            pOsICB->isrC2SuspendAllCount = 0;
        }
#endif

        Os_ICB[Os_SCB.sysRunningIsrCat2Id].IsrC2IsrOpt = TP_OPT_BUTT;
        Os_TmProtIsrEnd(OsIsrID, TP_ISR_CAT2_SUS_ALL_INT);
        Os_SuspendAllCount = 0;
        Os_ArchEnableAllInt_ButTimingProtInt();
        break;

    /* DisableAllInterrupts */
    case TP_DIS_ALL_INT:
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
        if (pOsICB->isrC2DisableAllCount > 0u)
        {
            pOsICB->isrC2DisableAllCount = 0u;
        }
#endif

        Os_ICB[Os_SCB.sysRunningIsrCat2Id].IsrC2IsrOpt = TP_OPT_BUTT;
        Os_TmProtIsrEnd(OsIsrID, TP_ISR_CAT2_DIS_ALL_INT);
        Os_ArchEnableAllInt_ButTimingProtInt();
        break;

    default:
        Os_Panic();
        break;
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Terminate task in protection hook. Terminate
 *                        running task. >
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ProtHookTermTaskIsr>
 * REQ ID               <None>
 */
/********************************************************************/
/* PRQA S 6020, 6080, 3450 ++ */ /* VL_MTR_Os_STLIN, VL_MTR_Os_STPTH, VL_Os_3450 */
static void Os_ProtHookTerminateTask(StatusType osErrType)
/* PRQA S 6020, 6080, 3450 -- */
{
    (void)osErrType;
    Os_TaskType       osRunningTaskID;
    const Os_TCBType* pRunningTaskTcb;

#if ((CFG_STD_RESOURCE_MAX > 0U) || (CFG_SPINLOCK_MAX > 0U))
    Os_CoreIdType coreId = Os_SCB.sysCore;
    /* PRQA S 4544, 3120 ++ */ /*  VL_Os_4544, VL_QAC_MagicNum */
    Os_CoreIdType coreIndex = coreId << 12;
    /* PRQA S 4544, 3120 -- */
#endif

    osRunningTaskID = Os_SCB.sysRunningTaskID;
    pRunningTaskTcb = &Os_TCB[osRunningTaskID];
/* Release resources occupied by running task. */
/* OESK 8.2:
   In case of multiple resource occupation within one task, the user has
   to request and release resources following the LIFO principle
   (stack like). */
#if (CFG_SPINLOCK_MAX > 0U)
    uint16 CountIdx;
    uint16 CurrentCount;
#if (CFG_STD_RESOURCE_MAX > 0U)
    ResourceType ResourceId;
#endif
    SpinlockIdType SpinlockId;

    for (CountIdx = pRunningTaskTcb->taskCriticalZoneCount; CountIdx > 0u; CountIdx--)
    {
        CurrentCount = pRunningTaskTcb->taskCriticalZoneCount - 1u;
        if (OBJECT_SPINLOCK == pRunningTaskTcb->taskCriticalZoneType[CurrentCount])
        {
            SpinlockId = pRunningTaskTcb->taskCriticalZoneStack[CurrentCount];
            (void)Os_ReleaseSpinlock(SpinlockId);
        }
#if (CFG_STD_RESOURCE_MAX > 0U)
        else
        {
            ResourceId = (coreIndex | pRunningTaskTcb->taskCriticalZoneStack[CurrentCount]);
            (void)Os_ReleaseResource(ResourceId);
        }
#endif
    }

#elif (CFG_STD_RESOURCE_MAX > 0U)
    uint16 i;
    if (pRunningTaskTcb->taskResCount > 0u)
    {
        for (i = pRunningTaskTcb->taskResCount; i > 0u; i--)
        {
            (void)Os_ReleaseResource(coreIndex | pRunningTaskTcb->taskResourceStack[i - 1u]);
        }
    }
#endif

#if (CFG_INTERNAL_RESOURCE_MAX > 0U)
    Os_ReleaseInternalResource(osRunningTaskID);
#endif

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    Os_ProtHookTaskIsrOpt(pRunningTaskTcb->taskIsrOpt);
#endif

/* Update task state. */
#if ((OS_BCC1 == CFG_CC) || (OS_ECC1 == CFG_CC))
    Os_SCB.sysRunningTCB->taskState = TASK_STATE_SUSPENDED;
#else
    if (Os_SCB.sysRunningTCB->taskActCount > 0u)
    {
        Os_SCB.sysRunningTCB->taskActCount = Os_SCB.sysRunningTCB->taskActCount - 1u;
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

    /*If the task is a non-preemptive task, then release it.*/
    if (OS_PREEMPTIVE_NON == Os_TaskCfg[osRunningTaskID].osTaskSchedule)
    {
        Os_SCB.sysDispatchLocker = 1U;
    }
    /* Remove task from ready queue. */
    Os_ReadyQueueRemove(OS_LEVEL_TASK, pRunningTaskTcb->taskRunPrio);

    /* Reset TCB. */
    Os_SCB.sysRunningTCB->taskTop = Os_TaskStack[Os_GetObjLocalId(Os_TaskCfg[osRunningTaskID].osTaskStackId)].stackTop;

#if (CFG_STD_RESOURCE_MAX > 0U)
    Os_SCB.sysRunningTCB->taskResCount = 0U;
#endif

    Os_SCB.sysRunningTCB->taskRunPrio = Os_TaskCfg[osRunningTaskID].osTaskPriority;

#if (CFG_EXTENDED_TASK_MAX > 0U)
    if (osRunningTaskID < Os_CfgExtendTaskMax)
    {
        Os_ECB[osRunningTaskID].eventSetEvent  = 0U;
        Os_ECB[osRunningTaskID].eventWaitEvent = 0U;
    }
#endif

    if (Os_SCB.sysHighTaskID == osRunningTaskID)
    {
        Os_SCB.sysHighPrio   = Os_GetHighPrio();
        Os_SCB.sysHighTaskID = Os_ReadyQueueGetFirst(Os_SCB.sysHighPrio);
    }
#if (TRUE == CFG_STACK_CHECK)
    if (E_OS_STACKFAULT == (StatusType)osErrType)
    {
        /*nothing to do*/
    }
    else
#endif
    {
/* Timing protection: reset task exe time. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        Os_TmProtTaskEnd(Os_SCB.sysRunningTaskID, TP_TASK_EXE);
        /* Timing protection do task_dispatch at the end of tm_prot_isr. */
        if ((E_OS_PROTECTION_LOCKED != (StatusType)osErrType) && (E_OS_PROTECTION_TIME != (StatusType)osErrType))
        {
#if (TRUE == CFG_TRACE_ENABLE)
            Os_TraceTaskSwitch(
                Os_SCB.sysRunningTaskID,
                Os_SCB.sysHighTaskID,
                OS_TRACE_TASK_SWITCH_REASON_PROT_HOOK_TERMINATE,
                OS_TRACE_TASK_SWITCH_REASON_PROT_HOOK_ACTIVE);
#endif
            Os_SCB.sysDispatchLocker = 0U;
            Os_Dispatch(); /* PRQA S 1290, 3138 */ /* VL_Os_1290, VL_Os_PlatformDef */
        }

#if ((OS_SC4 == CFG_SC) && (CFG_TRUSTED_SYSTEM_SERVICE_MAX > 0U))
        /* When a trusted function requires deferred execution for hook time protection, 
        reschedule the task and terminate it. */
        if (TRUE == Os_TrustedFuncTp.TrustedFuncTporFlag)
        {
            Os_TrustedFuncTp.TrustedFuncTporFlag = FALSE;
            Os_SCB.sysDispatchLocker = 0U;
            Os_Dispatch(); /* PRQA S 1290, 3138 */ /* VL_Os_1290, VL_Os_PlatformDef */
        }
#endif

        Os_TprotTerminateTask = TRUE;
#else

#if (TRUE == CFG_TRACE_ENABLE)
        Os_TraceTaskSwitch(
            Os_SCB.sysRunningTaskID,
            Os_SCB.sysHighTaskID,
            OS_TRACE_TASK_SWITCH_REASON_PROT_HOOK_TERMINATE,
            OS_TRACE_TASK_SWITCH_REASON_PROT_HOOK_ACTIVE);
#endif

        Os_SCB.sysDispatchLocker = 0U;
        Os_Dispatch(); /* PRQA S 1290, 3138 */ /* VL_Os_1290, VL_Os_PlatformDef */
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (CFG_ISR2_MAX > 0)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Terminate isr cat2 in protection hook.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ProtHookTermTaskIsr>
 * REQ ID               <None>
 */
/********************************************************************/
/* PRQA S 6030, 6070, 3006, 3450 ++ */ /* VL_MTR_Os_STMIF, VL_MTR_Os_STCAL, VL_Os_3006, VL_Os_3450 */
static void Os_ProtHookTerminateIsrC2(StatusType osErrType)
/* PRQA S 6030, 6070, 3006, 3450 -- */
{
    (void)osErrType;

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    Os_IsrType RunIsrId = Os_SCB.sysRunningIsrCat2Id;
#endif

#if ((CFG_STD_RESOURCE_MAX > 0U) || (CFG_SPINLOCK_MAX > 0U))
    Os_CoreIdType coreId = Os_SCB.sysCore;
    /* PRQA S 4544, 3120 ++ */ /*  VL_Os_4544, VL_QAC_MagicNum */
    Os_CoreIdType coreIndex = coreId << 12;
    /* PRQA S 4544, 3120 -- */
#endif

/* In timing protection, . */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    if ((E_OS_PROTECTION_TIME == osErrType) || (E_OS_PROTECTION_LOCKED == osErrType))
    {
        if (Os_IntNestISR2 >= OS_TMPROT_TARGET_ISR_OFFSET)
        {
            Os_SCB.sysRunningIsrCat2Id = Os_SCB.sysIsrNestQueue[Os_IntNestISR2 - OS_TMPROT_TARGET_ISR_OFFSET];
        }
    }
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */

#if ((CFG_STD_RESOURCE_MAX > 0U) || (CFG_SPINLOCK_MAX > 0U) || (TRUE == CFG_TIMING_PROTECTION_ENABLE))
    const Os_ICBType* pIcb = &Os_ICB[Os_SCB.sysRunningIsrCat2Id];
#endif

/* Terminate resource. */
/* OESK 8.2:
   In case of multiple resource occupation within one task, the user has to request and release
   resources following the LIFO principle (stack like). */
#if (CFG_SPINLOCK_MAX > 0U)
    uint16 CountIdx;
    uint16 CurrentCount;
#if (CFG_STD_RESOURCE_MAX > 0U)
    ResourceType ResourceId;
#endif
    SpinlockIdType SpinlockId;

    for (CountIdx = pIcb->isr2CriticalZoneCount; CountIdx > 0u; CountIdx--)
    {
        CurrentCount = pIcb->isr2CriticalZoneCount - 1u;
        if (OBJECT_SPINLOCK == pIcb->isr2CriticalZoneType[CurrentCount])
        {
            SpinlockId = pIcb->isr2CriticalZoneStack[CurrentCount];
            (void)Os_ReleaseSpinlock(SpinlockId);
        }
#if (CFG_STD_RESOURCE_MAX > 0U)
        else
        {
            ResourceId = (coreIndex | pIcb->isr2CriticalZoneStack[CurrentCount]);
            (void)Os_ReleaseResource(ResourceId);
        }
#endif
    }

#elif (CFG_STD_RESOURCE_MAX > 0U)
    uint16 i;
    if (pIcb->IsrC2ResCount > 0u)
    {
        for (i = pIcb->IsrC2ResCount; i > 0u; i--)
        {
            (void)Os_ReleaseResource(coreIndex | pIcb->IsrC2ResourceStack[i - 1u]);
        }
    }
#endif

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    Os_ProtHookIsr2IsrOpt(pIcb->IsrC2IsrOpt);
    /* Stop isr exe budget. */
    Os_TmProtIsrEnd(Os_SCB.sysRunningIsrCat2Id, TP_ISR_CAT2_EXE);
    Os_SCB.sysRunningIsrCat2Id = RunIsrId;
    /* Set terminate_isr flag. */
    Os_TprotTerminateIsr = TRUE;

    /* Timing protection do task_dispatch at the end of tm_prot_isr. */
    if ((E_OS_PROTECTION_LOCKED != osErrType) && (E_OS_PROTECTION_TIME != osErrType) && (E_OS_STACKFAULT != osErrType))
    {
        OS_ARCH_ISR2_EPILOGUE();
    }
#else
    if (E_OS_STACKFAULT != osErrType)
    {
        /* PRQA S 3442,2481,0306,0310,2743 ++ */ /* VL_Os_3442,VL_Os_2481,VL_Os_0306,VL_Os_0310,VL_Os_2743 */
        /* PRQA S 0404,3345 ++ */                /*VL_Os_VolatileAccess*/
        /* PRQA S 1006, 3415 ++ */                     /* VL_Os_1006, VL_Os_3415 */
        OS_ARCH_ISR2_EPILOGUE();
        /* PRQA S 1006, 3415 -- */
        /* PRQA S 0404,3345 -- */
        /* PRQA S 3442,2481,0306,0310,2743 -- */
    }
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* CFG_ISR2_MAX > 0 */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <PRO_TERMINATETASKISR: terminate task or isr in hook.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_CallProtectionHook>
 * REQ ID               <None>
 */
/********************************************************************/
/* PRQA S 3450 ++ */ /* VL_Os_3450 */
static void Os_ProtHookTermTaskIsr(uint32 osWhoHook, StatusType osErrType)
/* PRQA S 3450 -- */
{
    if (OS_TMPROT_HOOK_TASK == osWhoHook)
    {
        Os_ProtHookTerminateTask(osErrType);
    }

#if (CFG_ISR2_MAX > 0)
    if (OS_TMPROT_HOOK_ISR == osWhoHook)
    {
        Os_ProtHookTerminateIsrC2(osErrType);
    }
#endif

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
/* Terminate os_app process. */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Terminate os_app process>
 *
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <osErrType,RestartOption,osWhoHook>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <Os_CallProtectionHook>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3450 ++ */ /* VL_Os_3450 */
static void Os_ProtHookTermAppl(StatusType osErrType, RestartType RestartOption, uint32 osWhoHook)
/* PRQA S 3450 -- */
{
    (void)osWhoHook;
    (void)osErrType;
    Os_ApplicationType RunApplIdTemp = Os_SCB.sysRunningAppID;

/* In timing protection, the running_application maybe SYS_APP. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    Os_IsrType  IsrId;
    Os_TaskType TaskId;
    if ((E_OS_PROTECTION_LOCKED == osErrType) || (E_OS_PROTECTION_TIME == osErrType))
    {
        if (OS_TMPROT_HOOK_ISR == osWhoHook)
        {
            /* Update running_application_id. */
            if (Os_IntNestISR2 >= OS_TMPROT_TARGET_ISR_OFFSET)
            {
                IsrId = Os_SCB.sysIsrNestQueue[Os_IntNestISR2 - OS_TMPROT_TARGET_ISR_OFFSET];

                if (IsrId < CFG_ISR_MAX)
                {
                    Os_SCB.sysRunningAppID = Os_ObjectAppCfg[OBJECT_ISR][IsrId].hostApp;
                }
            }
        }
        else if (OS_TMPROT_HOOK_TASK == osWhoHook)
        {
            TaskId                 = Os_SCB.sysRunningTaskID;
            Os_SCB.sysRunningAppID = Os_ObjectAppCfg[OBJECT_TASK][TaskId].hostApp;
        }
        else
        {
            /*Nothing to do*/
        }
    }
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */

    /*If the ProtectionHook returns PRO_TERMINATEAPPL_RESTART and no OsRestartTask was configured
     *for the faulty OS-Application, ShutdownOS is called.*/
    if ((Os_AppCfg[Os_SCB.sysRunningAppID].OsRestartTask >= Os_SCB.sysTaskMax) && (RESTART == RestartOption))
    {
        Os_ShutdownOS(osErrType, SHUTDOWN_OS);
    }
    else
    {
        /* Call TerminateApplication. */
        Os_TerminateApplication(Os_SCB.sysRunningAppID, RestartOption);

        Os_SCB.sysRunningAppID = RunApplIdTemp;
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* OS_SC3 == CFG_SC || OS_SC4 == CFG_SC */
#endif /* TRUE == CFG_PROTECTIONHOOK */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/* ProtHook shutdownOs process. */
static void Os_ProtHookShutdownOS(StatusType Error) /* PRQA S 3450 */ /* VL_Os_3450 */
{
    (void)Error;

/* SWS_Os_00586: Application specific hook. */
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_APPL_SHUTDOWNHOOK)
    Os_ApplShutdownHook(Error);
#endif
#endif

/* SWS_Os_00588: Global ShutdownHook. */
#if (TRUE == CFG_SHUTDOWNHOOK)
    Os_CallShutdownHook(Error);
#endif

    OS_DisableInterrupts();
/*Os_00620*/
#if (CFG_SPINLOCK_MAX > 0U)
    Os_TaskType     i;
    SpinlockIdType  SpinlockId;
    Os_SpinlockType spinLockIdx;
    /* OS_HOOK_CONST_003 */
    Os_TCBType* pTCB;
    Os_ICBType* pICB;

    for (i = 0; i < (Os_SCB.sysTaskMax - (uint16)1); i++) /* PRQA S 1290 */ /* VL_Os_1290 */
    {
        pTCB = &Os_TCB[i];
        for (spinLockIdx = pTCB->taskCriticalZoneCount; spinLockIdx > 0U; spinLockIdx--)
        {
            if (pTCB->taskCriticalZoneType[pTCB->taskCriticalZoneCount - 1U] == OBJECT_SPINLOCK)
            {
                SpinlockId = pTCB->taskCriticalZoneStack[pTCB->taskCriticalZoneCount - 1U];
                (void)Os_ReleaseSpinlock(SpinlockId);
            }
            else
            {
                pTCB->taskCriticalZoneCount--;
            }
        }
    }

    for (i = 0; i < Os_CfgIsr2Max; i++) /* PRQA S 1290 */ /* VL_Os_1290 */
    {
        pICB = &Os_ICB[i];
        for (spinLockIdx = pICB->isr2CriticalZoneCount; spinLockIdx > 0U; spinLockIdx--)
        {
            if (pICB->isr2CriticalZoneType[pICB->isr2CriticalZoneCount - 1U] == OBJECT_SPINLOCK)
            {
                SpinlockId = pICB->isr2CriticalZoneStack[pICB->isr2CriticalZoneCount - 1U];
                (void)Os_ReleaseSpinlock(SpinlockId);
            }
            else
            {
                pICB->isr2CriticalZoneCount--;
            }
        }
    }
#endif /* CFG_SPINLOCK_MAX > 0U */

    Os_DeInitCoreInfo();

    /* SWS_Os_00715*/
    for (;;) /* PRQA S 2870 */ /* VL_Os_2870 */
    {
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <ProtectionHook called by OS.>
 * Service ID           <None>
 * Sync/Async           <None>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <osErrType>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <WaitEvent>
 * REQ ID               <None>
 */
/********************************************************************/
/* PRQA S 6010, 6070 ++ */ /* VL_MTR_Os_STCYC, VL_MTR_Os_STCAL */
ProtectionReturnType Os_CallProtectionHook(StatusType osErrType, uint32 osWhoHook)
/* PRQA S 6010, 6070 -- */
{
    ProtectionReturnType osRet;

#if (TRUE == CFG_PROTECTIONHOOK)
    Os_CallLevelType osSameLevel;
    OS_ARCH_DECLARE_CRITICAL();

    OS_ARCH_ENTRY_CRITICAL();
    osSameLevel       = Os_SCB.sysOsLevel;
    Os_SCB.sysOsLevel = OS_LEVEL_PROTECTIONHOOK;
    OS_ARCH_EXIT_CRITICAL();

    OS_ARCH_ENTRY_CRITICAL();
    osRet = ProtectionHook(osErrType);
    OS_ARCH_EXIT_CRITICAL();

    OS_ARCH_ENTRY_CRITICAL();
    Os_SCB.sysOsLevel = osSameLevel;
    OS_ARCH_EXIT_CRITICAL();

    if ((E_OS_PROTECTION_ARRIVAL == osErrType) && (PRO_IGNORE != osRet))
    {
        /* SWS_Os_00506 */
        osRet = PRO_SHUTDOWN;
    }

    /* Hook post process. */
    switch (osRet)
    {
    case PRO_IGNORE:
        /* SWS_Os_00475 */
        if (E_OS_PROTECTION_ARRIVAL != osErrType)
        {
            Os_ProtHookShutdownOS(osErrType);
        }
        break;

    case PRO_TERMINATETASKISR:
        /* PRQA S 1881 ++ */ /* VL_QAC_AutosarBool */
        if ((boolean)E_OK == Os_CheckRunningTaskIDandIsr2ID(osWhoHook))
        /* PRQA S 1881 -- */
        {
            Os_ProtHookTermTaskIsr(osWhoHook, osErrType);
            break; /* PRQA S 3333 */ /* VL_Os_3333 */
        }
/*OS243
if the reaction is to forcibly terminate the Task/Category 2 OsIsr and no Task
or OsIsr can be associated with the error,the running OS-Application is forcibly
terminated by the Operating System
*/
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        if (Os_SCB.sysRunningAppID <= Os_SCB.sysAppId)
#else
        if (Os_SCB.sysRunningAppID < Os_SCB.sysAppId)
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */
        {
            Os_ProtHookTermAppl(osErrType, NO_RESTART, osWhoHook);
            break; /* PRQA S 3333 */ /* VL_Os_3333 */
        }
#endif /* OS_SC3 == CFG_SC || OS_SC4 == CFG_SC */
        Os_ShutdownOS(osErrType, SHUTDOWN_OS);
        break;

    case PRO_TERMINATEAPPL:
/*OS244
 * If the reaction is to forcibly terminate the faulty OS-Application
 * and no OS-Application can be assigned,ShutdownOS() is called.
 */
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        if (Os_SCB.sysRunningAppID <= Os_SCB.sysAppId)
#else
        if (Os_SCB.sysRunningAppID < Os_SCB.sysAppId)
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */
        {
            Os_ProtHookTermAppl(osErrType, NO_RESTART, osWhoHook);
            break; /* PRQA S 3333 */ /* VL_Os_3333 */
        }
#endif /* OS_SC3 == CFG_SC || OS_SC4 == CFG_SC */
        Os_ShutdownOS(osErrType, SHUTDOWN_OS);
        break;

    case PRO_TERMINATEAPPL_RESTART:
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        if (Os_SCB.sysRunningAppID <= Os_SCB.sysAppId)
#else
        if (Os_SCB.sysRunningAppID < Os_SCB.sysAppId)
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */
        {
            Os_ProtHookTermAppl(osErrType, RESTART, osWhoHook);
            break; /* PRQA S 3333 */ /* VL_Os_3333 */
        }
#endif /* OS_SC3 == CFG_SC || OS_SC4 == CFG_SC */
        Os_ShutdownOS(osErrType, SHUTDOWN_OS);
        break;

    case PRO_SHUTDOWN:
        Os_ProtHookShutdownOS(osErrType);
        break; /* PRQA S 2880 */ /* VL_Os_2880 */

    default:
        /* OS107: ShutdownOS. */
        Os_ProtHookShutdownOS(osErrType);
        break; /* PRQA S 2880 */ /* VL_Os_2880 */
    }
#else
    (void)osWhoHook;
    /* OS107: ShutdownOS. */
    Os_ProtHookShutdownOS(osErrType);
    osRet = PRO_SHUTDOWN;
#endif /* TRUE == CFG_PROTECTIONHOOK */

    return osRet;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC */

/*=======[E N D   O F   F I L E]==============================================*/
/* PRQA S 0553 EOF */ /* VL_QAC_UnUsedFiles */
