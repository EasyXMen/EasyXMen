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
 **  @file               : Os_Hook.c
 **  @author             : i-soft-os
 **  @date               : 2025/02/10
 **  @vendor             : isoft
 **  @description        : Os source file for Hook API implementations
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Os_Arch_Processor.h"
#include "Os_Hook.h"
#include "Os_Interrupt.h"
#include "Os_Appl.h"
#include "Os_Task.h"
#include "Os_Arti.h"
#include "Os_Kernel.h"

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ============================================ external data definitions =========================================== */

/* ============================================ internal data definitions =========================================== */

/* ========================================== internal function declarations ======================================== */

/* ========================================== external function definitions ========================================= */

/* Attention: this function is only called in os_kernel. Usr_app or test_case_app
 * can not call this function directly. */
#if ((TRUE == CFG_ERRORHOOK) || (TRUE == CFG_APPL_ERRORHOOK))
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Call the error hook function
 */
void Os_CallErrorHook(StatusType error)
{
#if (CFG_OSAPPLICATION_MAX > 0U)
    Os_ApplicationType bakAppId = INVALID_OSAPPLICATION;
#endif
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1841, 4544, 4542, 4543, 1277 ++ */ /* VL_Os_1841, VL_Os_4544, VL_Os_4542, VL_Os_4543, VL_Os_1277 */
    ARTI_TRACE(NOSUSP, AR_CP_OS_HOOK, Os, pScb->SysCore, OsHook_ErrorHook_Start, error);
    /* PRQA S 1841, 4544, 4542, 4543, 1277 -- */
    /* PRQA S 3138, 3141 -- */
    OS_HAL_DECLARE_CRITICAL();

    if (OS_LEVEL_ERRORHOOK == pScb->SysOsLevel)
    {
        /*nothing to do*/
    }
    else
    {
        OS_HAL_ENTRY_CRITICAL();
/*Triggering the error hook is Task or Isr,then update the running ApplID and Object*/
#if (CFG_OSAPPLICATION_MAX > 0U)
        if (OS_LEVEL_TASK == pScb->SysOsLevel)
        {
            bakAppId = pScb->SysRunningAppId;
            pScb->SysRunningAppId = Os_GetObjectAppID(OS_OBJECT_TASK, (AppObjectId)pScb->SysRunningTaskId);
            pScb->SysRunningAppObj = OS_OBJECT_TASK;
        }
        else if (OS_LEVEL_ISR2 == pScb->SysOsLevel)
        {
            bakAppId = pScb->SysRunningAppId;
            pScb->SysRunningAppId = Os_GetObjectAppID(OS_OBJECT_ISR, (AppObjectId)pScb->SysRunningIsrCat2Id);
            pScb->SysRunningAppObj = OS_OBJECT_ISR;
        }
        else
        {
            /*nothing to do*/
        }
#endif

        Os_CallLevelType sameLevel = pScb->SysOsLevel;
        /* PRQA S 2982 ++ */ /* VL_Os_2982 */
        pScb->SysOsLevel = OS_LEVEL_ERRORHOOK;
        /* PRQA S 2982 -- */
#if (TRUE == CFG_ERRORHOOK)
        ErrorHook(error);
#endif

/* If cfg, call application_specific hook. */
#if (TRUE == CFG_APPL_ERRORHOOK)
        const Os_ApplicationCfgType *pAppCfg = &Os_AppCfg[pScb->SysRunningAppId];
        if (NULL_PTR != pAppCfg->ApplicationHooks.AppErrorHook)
        {
            pScb->SysOsLevel = OS_LEVEL_ERRORHOOK_APP; /* PRQA S 2982 */ /* VL_Os_2982 */
            pAppCfg->ApplicationHooks.AppErrorHook(error);
        }
#endif

        pScb->SysOsLevel = sameLevel;
#if (CFG_OSAPPLICATION_MAX > 0U)
        if ((OS_LEVEL_TASK == pScb->SysOsLevel) || (OS_LEVEL_ISR2 == pScb->SysOsLevel))
        {
            pScb->SysRunningAppId = bakAppId;
        }
#endif
        OS_HAL_EXIT_CRITICAL();
    }
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1841, 4544, 4542, 4543, 1277, 1259 ++ */ /* VL_Os_1841, VL_Os_4544, VL_Os_4542, VL_Os_4543, VL_Os_1277, VL_Os_1259 */
    ARTI_TRACE(NOSUSP, AR_CP_OS_HOOK, Os, pScb->SysCore, OsHook_ErrorHook_Return, 0);
    /* PRQA S 1841, 4544, 4542, 4543, 1277, 1259 -- */
    /* PRQA S 3138, 3141 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#if (TRUE == CFG_PRETASKHOOK)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Call the PreTask hook function
 */
void Os_CallPreTaskHook(Os_SCBType *pScb) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
#if (CFG_OSAPPLICATION_MAX > 0U)
    Os_ApplicationType bakAppId;
#endif
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1841, 4544, 4542, 4543, 1277, 1259 ++ */ /* VL_Os_1841, VL_Os_4544, VL_Os_4542, VL_Os_4543, VL_Os_1277, VL_Os_1259 */
    ARTI_TRACE(NOSUSP, AR_CP_OS_HOOK, Os, pScb->SysCore, OsHook_PreTaskHook_Start, 0);
    /* PRQA S 1841, 4544, 4542, 4543, 1277, 1259 -- */
    /* PRQA S 3138, 3141 -- */
    OS_HAL_DECLARE_CRITICAL();

    OS_HAL_ENTRY_CRITICAL();
/*if task trigger the pretask hook ,update the running ApplID and Object*/
#if (CFG_OSAPPLICATION_MAX > 0U)
    if (OS_LEVEL_TASK == pScb->SysOsLevel)
    {
        bakAppId = pScb->SysRunningAppId;
        pScb->SysRunningAppId  = Os_GetObjectAppID(OS_OBJECT_TASK, (AppObjectId)pScb->SysRunningTaskId); /* PRQA S 4424 */ /* VL_Os_4424 */
        pScb->SysRunningAppObj = OS_OBJECT_TASK;
    }
#endif

    Os_CallLevelType sameLevel = pScb->SysOsLevel;
    pScb->SysOsLevel = OS_LEVEL_PRETASKHOOK; /* PRQA S 2982 */ /* VL_Os_2982 */
    PreTaskHook();
    pScb->SysOsLevel = sameLevel;
#if (CFG_OSAPPLICATION_MAX > 0U)
    if (OS_LEVEL_TASK == pScb->SysOsLevel)
    {
        pScb->SysRunningAppId = bakAppId;
    }
#endif
    OS_HAL_EXIT_CRITICAL();
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1841, 4544, 4542, 4543, 1277, 1259 ++ */ /* VL_Os_1841, VL_Os_4544, VL_Os_4542, VL_Os_4543, VL_Os_1277, VL_Os_1259 */
    ARTI_TRACE(NOSUSP, AR_CP_OS_HOOK, Os, pScb->SysCore, OsHook_PreTaskHook_Return, 0);
    /* PRQA S 1841, 4544, 4542, 4543, 1277, 1259 -- */
    /* PRQA S 3138, 3141 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#if (TRUE == CFG_PRETASKHOOK)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Call the PreTask hook function
 */
void Os_CallPostTaskHook(Os_SCBType *pScb)
{
#if (CFG_OSAPPLICATION_MAX > 0U)
    Os_ApplicationType bakAppId;
#endif
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1841, 4544, 4542, 4543, 1277, 1259 ++ */ /* VL_Os_1841, VL_Os_4544, VL_Os_4542, VL_Os_4543, VL_Os_1277, VL_Os_1259 */
    ARTI_TRACE(NOSUSP, AR_CP_OS_HOOK, Os, pScb->SysCore, OsHook_PostTaskHook_Start, 0);
    /* PRQA S 1841, 4544, 4542, 4543, 1277, 1259 -- */
    /* PRQA S 3138, 3141 -- */
    OS_HAL_DECLARE_CRITICAL();

    OS_HAL_ENTRY_CRITICAL();
/*if task trigger the posttask hook ,update the running ApplID and Object*/
#if (CFG_OSAPPLICATION_MAX > 0U)
    if (OS_LEVEL_TASK == pScb->SysOsLevel)
    {
        bakAppId = pScb->SysRunningAppId;
        pScb->SysRunningAppId = Os_GetObjectAppID(OS_OBJECT_TASK, (AppObjectId)pScb->SysRunningTaskId); /* PRQA S 4424 */ /* VL_Os_4424 */
        pScb->SysRunningAppObj = OS_OBJECT_TASK;
    }
#endif

    Os_CallLevelType sameLevel = pScb->SysOsLevel;
    pScb->SysOsLevel = OS_LEVEL_POSTTASKHOOK; /* PRQA S 2982 */ /* VL_Os_2982 */
    PostTaskHook();
    pScb->SysOsLevel = sameLevel;
#if (CFG_OSAPPLICATION_MAX > 0U)
    if (OS_LEVEL_TASK == pScb->SysOsLevel)
    {
        pScb->SysRunningAppId = bakAppId;
    }
#endif
    OS_HAL_EXIT_CRITICAL();
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1841, 4544, 4542, 4543, 1277, 1259 ++ */ /* VL_Os_1841, VL_Os_4544, VL_Os_4542, VL_Os_4543, VL_Os_1277, VL_Os_1259 */
    ARTI_TRACE(NOSUSP, AR_CP_OS_HOOK, Os, pScb->SysCore, OsHook_PostTaskHook_Return, 0);
    /* PRQA S 1841, 4544, 4542, 4543, 1277, 1259 -- */
    /* PRQA S 3138, 3141 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#if (TRUE == CFG_STARTUPHOOK)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Call the Startup hook function
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_CallStartupHook(Os_SCBType *pScb)
/* PRQA S 1532 -- */
{
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1841, 4544, 4542, 4543, 1277, 1259 ++ */ /* VL_Os_1841, VL_Os_4544, VL_Os_4542, VL_Os_4543, VL_Os_1277, VL_Os_1259 */
    ARTI_TRACE(NOSUSP, AR_CP_OS_HOOK, Os, pScb->SysCore, OsHook_StartupHook_Start, 0);
    /* PRQA S 1841, 4544, 4542, 4543, 1277, 1259 -- */
    /* PRQA S 3138, 3141 -- */
    Os_CallLevelType sameLevel = pScb->SysOsLevel;
    /* PRQA S 2982 ++ */ /* VL_Os_2982 */
    pScb->SysOsLevel = OS_LEVEL_STARTUPHOOK;
    /* PRQA S 2982 -- */
    StartupHook();
    pScb->SysOsLevel = sameLevel;
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1841, 4544, 4542, 4543, 1277, 1259 ++ */ /* VL_Os_1841, VL_Os_4544, VL_Os_4542, VL_Os_4543, VL_Os_1277, VL_Os_1259 */
    ARTI_TRACE(NOSUSP, AR_CP_OS_HOOK, Os, pScb->SysCore, OsHook_StartupHook_Return, 0);
    /* PRQA S 1841, 4544, 4542, 4543, 1277, 1259 -- */
    /* PRQA S 3138, 3141 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#if (TRUE == CFG_SHUTDOWNHOOK)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Call the Shutdown hook function
 */
void Os_CallShutdownHook(Os_SCBType *pScb, StatusType error)
{
#if (CFG_OSAPPLICATION_MAX > 0U)
    Os_ApplicationType bakAppId;
#endif
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1841, 4544, 4542, 4543, 1277 ++ */ /* VL_Os_1841, VL_Os_4544, VL_Os_4542, VL_Os_4543, VL_Os_1277 */
    ARTI_TRACE(NOSUSP, AR_CP_OS_HOOK, Os, pScb->SysCore, OsHook_ShutdownHook_Start, error);
    /* PRQA S 1841, 4544, 4542, 4543, 1277 -- */
    /* PRQA S 3138, 3141 -- */
    OS_HAL_DECLARE_CRITICAL();

    OS_HAL_ENTRY_CRITICAL();
/*Triggering the shutdown hook is Task or Isr,then update the running ApplID and Object*/
#if (CFG_OSAPPLICATION_MAX > 0U)
    if (OS_LEVEL_TASK == pScb->SysOsLevel)
    {
        bakAppId = pScb->SysRunningAppId;
        pScb->SysRunningAppId = Os_GetObjectAppID(OS_OBJECT_TASK, (AppObjectId)pScb->SysRunningTaskId);
        pScb->SysRunningAppObj = OS_OBJECT_TASK;
    }
    else if (OS_LEVEL_ISR2 == pScb->SysOsLevel)
    {
        bakAppId = pScb->SysRunningAppId;
        pScb->SysRunningAppId = Os_GetObjectAppID(OS_OBJECT_ISR, (AppObjectId)pScb->SysRunningIsrCat2Id);
        pScb->SysRunningAppObj = OS_OBJECT_ISR;
    }
    else if (OS_LEVEL_ISR1 == pScb->SysOsLevel)
    {
        uint8 intNestId = (uint8)pScb->SysIsrNestQueue[pScb->IntNestISR2 + pScb->IntNestISR1];
        bakAppId = pScb->SysRunningAppId;
        pScb->SysRunningAppId = Os_GetObjectAppID(OS_OBJECT_ISR, intNestId);
        pScb->SysRunningAppObj = OS_OBJECT_ISR;
    }
    else
    {
        /* Nothing to do. */
    }
#endif

    Os_CallLevelType sameLevel = pScb->SysOsLevel;
    /* PRQA S 2982 ++ */ /* VL_Os_2982 */
    pScb->SysOsLevel = OS_LEVEL_SHUTDOWNHOOK;
    /* PRQA S 2982 -- */
    ShutdownHook(error);
    pScb->SysOsLevel = sameLevel;
#if (CFG_OSAPPLICATION_MAX > 0U)
    if ((OS_LEVEL_TASK == pScb->SysOsLevel) || (OS_LEVEL_ISR2 == pScb->SysOsLevel) || (OS_LEVEL_ISR1 == pScb->SysOsLevel))
    {
        pScb->SysRunningAppId = bakAppId;
    }
#endif
    OS_HAL_EXIT_CRITICAL();
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1841, 4544, 4542, 4543, 1277, 1259 ++ */ /* VL_Os_1841, VL_Os_4544, VL_Os_4542, VL_Os_4543, VL_Os_1277, VL_Os_1259 */
    ARTI_TRACE(NOSUSP, AR_CP_OS_HOOK, Os, pScb->SysCore, OsHook_ShutdownHook_Return, 0);
    /* PRQA S 1841, 4544, 4542, 4543, 1277, 1259 -- */
    /* PRQA S 3138, 3141 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

/* Application specific hook. Ref: 8.5. */
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_APPL_STARTUPHOOK)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Call Appl's startup hook function
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_ApplStartupHook(Os_SCBType *pScb)
/* PRQA S 1532 -- */
{
    const Os_ApplicationCfgType *pAppCfg;
    Os_CallLevelType sameLevel = pScb->SysOsLevel;

    pScb->SysOsLevel = OS_LEVEL_STARTUPHOOK; /* PRQA S 2982 */ /* VL_Os_2982 */
    /* Loop each application, if cfg startuphook, then call it. */
    for (Os_ApplicationType i = 0u; i < (CFG_OSAPPLICATION_MAX - 1u); i++)
    {
        pAppCfg = &Os_AppCfg[i];

        if (NULL_PTR != pAppCfg->ApplicationHooks.AppStartupHook)
        {
            if (pAppCfg->HostCore == pScb->SysCore)
            {
                pAppCfg->ApplicationHooks.AppStartupHook();
            }
        }
    }
    pScb->SysRunningAppId = INVALID_OSAPPLICATION;
    pScb->SysRunningAppObj = OS_OBJECT_MAX;

    pScb->SysOsLevel = sameLevel;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#if (TRUE == CFG_APPL_SHUTDOWNHOOK)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Call Appl's Shutdown hook function
 */
void Os_ApplShutdownHook(Os_SCBType *pScb, StatusType fatalError)
{
    Os_ApplicationType bakAppId;
    OS_HAL_DECLARE_CRITICAL();

    /*Triggering the shutdown hook is Task or Isr,then update the running ApplID and Object*/
    if (OS_LEVEL_TASK == pScb->SysOsLevel)
    {
        bakAppId = pScb->SysRunningAppId;
        pScb->SysRunningAppId = Os_GetObjectAppID(OS_OBJECT_TASK, pScb->SysRunningTaskId); /* PRQA S 4424, 1431 */ /* VL_Os_4424, VL_Os_1431 */
        pScb->SysRunningAppObj = OS_OBJECT_TASK;
    }
    else if (OS_LEVEL_ISR2 == pScb->SysOsLevel)
    {
        bakAppId = pScb->SysRunningAppId;
        pScb->SysRunningAppId = Os_GetObjectAppID(OS_OBJECT_ISR, pScb->SysRunningIsrCat2Id); /* PRQA S 4424, 1431 */ /* VL_Os_4424, VL_Os_1431 */
        pScb->SysRunningAppObj = OS_OBJECT_ISR;
    }
    else if (OS_LEVEL_ISR1 == pScb->SysOsLevel)
    {
        uint8 intNestId = pScb->SysIsrNestQueue[pScb->IntNestISR2 + pScb->IntNestISR1]; /* PRQA S 4424 */ /* VL_Os_4424 */
        bakAppId = pScb->SysRunningAppId;
        pScb->SysRunningAppId = Os_GetObjectAppID(OS_OBJECT_ISR, intNestId);
        pScb->SysRunningAppObj = OS_OBJECT_ISR;
    }
    else
    {
        /* Nothing to do. */
    }

    Os_CallLevelType sameLevel = pScb->SysOsLevel;
    pScb->SysOsLevel = OS_LEVEL_SHUTDOWNHOOK; /* PRQA S 2982 */ /* VL_Os_2982 */

    /* Loop each application, if cfg shutdownhook, then call it. */
    const Os_ApplicationCfgType *pAppCfg;
    for (Os_ApplicationType i = 0u; i < (CFG_OSAPPLICATION_MAX - 1u); i++)
    {
        pAppCfg = &Os_AppCfg[i];

        if (NULL_PTR != pAppCfg->ApplicationHooks.AppShutdownHook)
        {
            OS_HAL_ENTRY_CRITICAL();
            pAppCfg->ApplicationHooks.AppShutdownHook(fatalError);
            OS_HAL_EXIT_CRITICAL();
        }
    }

    pScb->SysOsLevel = sameLevel;
    if ((OS_LEVEL_TASK == pScb->SysOsLevel) || (OS_LEVEL_ISR2 == pScb->SysOsLevel) || (OS_LEVEL_ISR1 == pScb->SysOsLevel))
    {
        pScb->SysRunningAppId = bakAppId;
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif
#endif

/*=======[E N D   O F   F I L E]==============================================*/
/* PRQA S 0553 EOF */ /* VL_QAC_UnUsedFiles */
