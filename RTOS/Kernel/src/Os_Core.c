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
 **  FILENAME    : Os_Core.c                                                   **
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

/*=======[I N C L U D E S]====================================================*/
#define OS_GLOBAL_VAR
#include "Os_Internal.h"

/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The function starts the core specified by the CoreID. >
 * Service ID           <0x17>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-CoreID[in]     <Reference to the core>
 * Param-Status[out]    <Return value of the function>
 * return               <StatusType>
 * PreCondition         <Not support calling the API after calling StartOS.>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 6030, 1532, 1503 ++ */ /* VL_MTR_Os_STMIF, VL_QAC_OneFunRef, VL_QAC_NoUsedApi */
void StartCore(CoreIdType CoreID, StatusType* Status)
/* PRQA S 6030, 1532, 1503 -- */
{
    StatusType err       = E_OK;
    CoreIdType logCoreId = Os_GetCoreLogID(CoreID);

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (NULL_PTR == Status)
    {
        err = E_OS_PARAM_POINTER;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_START_CORE) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
#if (OS_STATUS_EXTENDED == CFG_STATUS)
        /* SWS_Os_00606,SWS_Os_00678:StartCore is called after StartOS.*/
        /* PRQA S 3442, 4340, 1881 ++ */ /* VL_Os_3442, VL_Os_4340, VL_QAC_AutosarBool */
        if (TRUE == Os_CoreCB.coreStateSynPoint2[Os_SCB.sysCore])
        /* PRQA S 3442, 4340, 1881 -- */
        {
            err = E_OS_ACCESS;
        }
        /* SWS_Os_00677:The function StartCore shall start one core
         * that shall run under the control of the AUTOSAR OS.*/
        else if (OS_AUTOSAR_CORES <= logCoreId)
        {
            err = E_OS_ID;
        }
        /* SWS_Os_00679,SWS_Os_00680:If the parameter CoreIDs refers to a core
         * that was already started by the function StartCore the related core
         * is ignored and E_OS_STATE shall be returned.*/
        /* PRQA S 3442, 4397 ++ */ /* VL_Os_3442, VL_Os_4397 */
        else if ((Os_CoreCB.coreStateActive & ((CoreIdType)1U << logCoreId)) > 0u)
        /* PRQA S 3442, 4397 -- */
        {
            err = E_OS_STATE;
        }
        else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
        {
            Os_ArchStartCore(CoreID);
            /* Set Activated core flag bit */
            Os_CoreCB.coreStateActive |= (uint8)(1U << logCoreId); /* PRQA S 3442 */ /* VL_Os_3442 */
            Os_CoreCB.coreStatus[logCoreId] = OS_RUN;
            *Status                         = E_OK;
        }
    }

    if (err != E_OK) /* PRQA S 2992, 2996 */ /* VL_Os_2992, VL_Os_2996 */
    {
        if (NULL_PTR != Status) /* PRQA S 2880 */ /* VL_Os_2880 */
        {
            *Status = err;
        }

#if (CFG_ERRORHOOK == TRUE)
        Os_TraceErrorHook(OSError_Save_StartCore(CoreID, Status), OSServiceId_StartCore, err);
#endif
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (CFG_CORE_MAX > 1)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The function starts the core specified by the parameter
 *                       CoreID. It is allowed to call this function after
 *                       StartOS().>
 * Service ID           <0x18>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-CoreID[in]     <Reference to the core>
 * Param-Status[out]    <Return value of the function>
 * return               <StatusType>
 * PreCondition         <None>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 6010, 6080, 1503, 3006 ++ */ /* VL_MTR_Os_STCYC, VL_MTR_Os_STPTH, VL_QAC_NoUsedApi, VL_Os_3006 */
void StartNonAutosarCore(CoreIdType CoreID, StatusType* Status)
/* PRQA S 6010, 6080, 1503, 3006 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    StatusType err = E_OK;
#if (OS_STATUS_EXTENDED == CFG_STATUS)
    CoreIdType logCoreId;

    if (NULL_PTR == Status)
    {
        err = E_OS_PARAM_POINTER;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_START_NON_AUTOSAR_CORE) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
#if (OS_STATUS_EXTENDED == CFG_STATUS)
        /* SWS_Os_00685:If the parameter CoreID refers to an unknown core the
         * function StartNonAutosarCore has no effect and sets Status to E_OS_ID. */
        if (CFG_CORE_MAX <= CoreID)
        {
            err = E_OS_ID;
        }
        else
        {
            /* SWS_Os_00683:The function StartNonAutosarCore shall
             * start a core that is not controlled by the AUTOSAR OS. */
            logCoreId = Os_GetCoreLogID(CoreID);
            if (OS_CORE_INVALID != logCoreId)
            {
                err = E_OS_ID;
            }
        }

        if ((StatusType)E_OK == err)
        {
            /* SWS_Os_00684:If the parameter CoreID refers to a core
             * that was already started by the function StartNonAutosarCore
             * has no effect and sets "Status" to E_OS_STATE.*/
            if (OS_CORE_MODE_HALT != Os_GetCoreMode(CoreID))
            {
                err = E_OS_STATE;
            }
        }
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */

        if ((StatusType)E_OK == err) /* PRQA S 2991, 2995 */ /* VL_Os_2991, VL_Os_2995 */
        {
            Os_ArchStartCore(CoreID);
            *Status = E_OK;
        }
    }

    if (err != E_OK) /* PRQA S 2992, 2996 */ /* VL_Os_2992, VL_Os_2996 */
    {
        if (NULL_PTR != Status) /* PRQA S 2880 */ /* VL_Os_2880 */
        {
            *Status = err;
        }

#if (CFG_ERRORHOOK == TRUE)
        Os_TraceErrorHook(OSError_Save_StartNonAutosarCore(CoreID, Status), OSServiceId_StartNonAutosarCore, err);
#endif
    }

    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* OS_AUTOSAR_CORES > 1 */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <This API allows the caller to select the idle mode
 *                       action which is performed during idle time of the OS>
 * Service ID           <0x1d>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-CoreID[in]     <selects the core which idle mode is set>
 * param-IdleMode[in]   <the mode which shall be performed during idle time>
 * return               <StatusType, Return result of the function>
 * PreCondition         <None>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 6010, 6030, 6080 ++ */ /* VL_MTR_Os_STCYC, VL_MTR_Os_STMIF, VL_MTR_Os_STPTH */
/* PRQA S 6070, 1503, 3006 ++ */ /* VL_MTR_Os_STCAL, VL_QAC_NoUsedApi, VL_Os_3006 */
StatusType ControlIdle(CoreIdType CoreID, IdleModeType IdleMode)
/* PRQA S 6070, 1503, 3006 -- */
/* PRQA S 6010, 6030, 6080 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    CoreIdType logCoreId = Os_GetCoreLogID(CoreID);
    StatusType rst       = E_OK;
    OS_ARCH_DECLARE_CRITICAL();

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    if (Os_WrongContext(OS_CONTEXT_CONTROL_IDLE) != TRUE)
    {
        rst = E_OS_CALLEVEL;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        rst = E_OS_DISABLEDINT;
    }
    else
    {
        /* nothing to do*/
    }
    if ((StatusType)E_OK == rst)
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
#if (OS_STATUS_EXTENDED == CFG_STATUS)
        /* SWS_Os_00771:The function ControlIdle shall return E_OS_ID,
         * if the parameter CoreID or IdleMode is invalid. */
        if (OS_AUTOSAR_CORES <= logCoreId)
        {
            rst = E_OS_ID;
        }
        else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
        {
            OS_ARCH_ENTRY_CRITICAL();
#if (OS_AUTOSAR_CORES > 1U)
            Os_GetInternalSpinlock(&Os_SpinlockSync);
#endif
            /* SWS_Os_00802:the new IdleMode shall become effective
             * the next time that core enters the idle mode. */
            /* PRQA S 3442, 4397 ++ */ /* VL_Os_3442, VL_Os_4397 */
            if ((Os_CoreCB.coreStateActive & ((CoreIdType)1U << logCoreId)) > 0u)
            /* PRQA S 3442, 4397 -- */
            {
                switch (IdleMode)
                {
                case OS_IDLE_NO_HALT:
                    /* PRQA S 2487 ++ */ /* VL_Os_2487 */
                    while (FALSE == Os_SetCoreMode(CoreID, OS_CORE_MODE_IDLE))
                    /* PRQA S 2487 -- */
                    {
                        /* Nothing to do. */
                    }
                    Os_CoreCB.coreStatus[logCoreId] = OS_IDLE_NO_HALT;
                    break;

                case OS_RUN:
                    /* PRQA S 2487 ++ */ /* VL_Os_2487 */
                    while (FALSE == Os_SetCoreMode(CoreID, OS_CORE_MODE_RUN))
                    /* PRQA S 2487 -- */
                    {
                        /* Nothing to do. */
                    }
                    Os_CoreCB.coreStatus[logCoreId] = OS_RUN;
                    break;

                default:
                    rst = (StatusType)E_OS_ID;
                    break;
                }
            }
            else
            {
                rst = (StatusType)E_OS_ID;
            }
#if (OS_AUTOSAR_CORES > 1U)
            Os_ReleaseInternalSpinlock(&Os_SpinlockSync);
#endif
            OS_ARCH_EXIT_CRITICAL();
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (rst != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_ControlIdle(CoreID, IdleMode), OSServiceId_ControlIdle, rst);
    }
#endif

    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */

    return rst;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The function returns a unique core identifier.>
 * Service ID           <0x16>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * return               <CoreIdType, The return value is the unique logical
 *                       CoreID of the core.>
 * PreCondition         <None>
 * REQ ID               <None>
 */
/******************************************************************************/
CoreIdType GetCoreID(void) /* PRQA S 3006, 1503 */ /* VL_Os_3006, VL_QAC_NoUsedApi */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    CoreIdType phyCoreId;
    CoreIdType logCoreId;

    phyCoreId = Os_ArchGetCoreID();
    logCoreId = Os_GetCoreLogID(phyCoreId);

    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */

    return logCoreId;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The function returns the number of cores activated by
 *                          the StartCore function.>
 * Service ID           <0x15>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * return               <Number of cores activated by the StartCore function.>
 * PreCondition         <None>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3006, 1503 ++ */ /* VL_Os_3006, VL_QAC_NoUsedApi */
uint32 GetNumberOfActivatedCores(void)
/* PRQA S 3006, 1503 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    uint32        numOfActivatedCores = 0U;
    Os_CoreIdType loop_core;
    StatusType    status = E_OK;

/*service protection*/
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    if (Os_WrongContext(OS_CONTEXT_GET_NUMBER_OF_ACTIVATED_CORES) != TRUE)
    {
        status = E_OS_CALLEVEL;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        status = E_OS_DISABLEDINT;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
        for (loop_core = 0u; loop_core < OS_AUTOSAR_CORES; loop_core++)
        {
            /* PRQA S 3442, 4397 ++ */ /* VL_Os_3442, VL_Os_4397 */
            if ((Os_CoreCB.coreStateActive & ((Os_CoreIdType)1u << loop_core)) > 0u)
            /* PRQA S 3442, 4397 -- */
            {
                numOfActivatedCores++;
            }
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_GetNumberOfActivatedCores(), OSServiceId_GetNumberOfActivatedCores, status);
    }
#endif

    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    UNUSED_PARAMETER(status);
    return numOfActivatedCores;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (OS_AUTOSAR_CORES > 1)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The function kill all core.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-core[in]       <the ID of the core>
 * param-Error[in]      <the error status associated with the shutdown>
 * Param-Name[in/out]   <None>
 * Param-Name[out]      <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <ShutdownAllCores>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3450 ++ */ /* VL_Os_3450 */
static void Os_KillAllCore(Os_CoreIdType core, StatusType Error)
/* PRQA S 3450 -- */
{
    uint8 coreId;

    if (core >= OS_AUTOSAR_CORES)
    {
        Os_Panic();
    }

    for (coreId = 0U; (coreId < OS_AUTOSAR_CORES); coreId++)
    {
        if (coreId != core)
        {
            RpcInputType rpcData = {
                .sync         = RPC_ASYNC,
                .remoteCoreId = coreId,
                .serviceId    = OSServiceId_ShutdownOS,
                .srvPara0     = Error,
                /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral  */
                .srvPara1 = (uint32)NULL_PARA,
                .srvPara2 = (uint32)NULL_PARA,
                /* PRQA S 1258 -- */
            };
            (void)Os_RpcCallService(&rpcData);
        }
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* OS_AUTOSAR_CORES > 1 */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <After this service the OS on all AUTOSAR cores is shut down.>
 * Service ID           <0x1c>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-Error[in]      <needs to be a valid error code supported by the AUTOSAR OS.>
 * return               <None>
 * PreCondition         <Allowed at TASK and ISR level and internally by the OS.>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 6010, 6030, 6080 ++ */ /* VL_MTR_Os_STCYC, VL_MTR_Os_STMIF, VL_MTR_Os_STPTH */
/* PRQA S 6070, 3006, 1532, 1503 ++ */ /* VL_MTR_Os_STCAL, VL_Os_3006, VL_QAC_OneFunRef, VL_QAC_NoUsedApi */
void ShutdownAllCores(StatusType Error)
/* PRQA S 6070, 3006, 1532, 1503 -- */
/* PRQA S 6010, 6030, 6080 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    Os_TaskType i;
#if (CFG_SPINLOCK_MAX > 0U)
    Os_TCBType*     pTCB;
    Os_ICBType*     pICB;
    SpinlockIdType  SpinlockId;
    Os_SpinlockType spinLockIdx;
#endif

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    if (Os_WrongContext(OS_CONTEXT_SHUTDOWN_ALLCORES) != TRUE)
    {
        /*nothing to do*/
    }
    else if (Os_IgnoreService() != TRUE)
    {
        /*nothing to do*/
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
/* SWS_Os_00716:If ShutdownAllCores is called from
 * non trusted code the call shall be ignored.*/
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
        if (FALSE == Os_AppCfg[Os_SCB.sysRunningAppID].OsTrusted)
        {
            /*nothing to do*/
        }
        else
#endif /* OS_SC3 == CFG_SC || OS_SC4 == CFG_SC */
        {
#if (OS_AUTOSAR_CORES > 1)
            /* SWS_Os_00762,SWS_Os_00714. */
            Os_KillAllCore(Os_SCB.sysCore, Error);
#endif

/* SWS_Os_00586: Application specific hook. */
#if ((CFG_OSAPPLICATION_MAX > 0U) && (TRUE == CFG_APPL_SHUTDOWNHOOK))
            Os_ApplShutdownHook(Error);
#endif

            /* Third synchronization */
            Os_SynPoint(3u); /* PRQA S 3120 */ /* VL_QAC_MagicNum */

            OS_DisableInterrupts();
/* SWS_Os_00588: Global ShutdownHook. */
#if (TRUE == CFG_SHUTDOWNHOOK)
            Os_CallShutdownHook(Error);
#endif

/*Os_00620*/
#if (CFG_SPINLOCK_MAX > 0U)
            for (i = 0; i < (Os_SCB.sysTaskMax - 1u); i++) /* PRQA S 1290 */ /* VL_Os_1290 */
            {
                pTCB = &Os_TCB[i];
                for (spinLockIdx = pTCB->taskCriticalZoneCount; spinLockIdx > 0u; spinLockIdx--)
                {
                    if (OBJECT_SPINLOCK == pTCB->taskCriticalZoneType[pTCB->taskCriticalZoneCount - 1u])
                    {
                        SpinlockId = pTCB->taskCriticalZoneStack[pTCB->taskCriticalZoneCount - 1u];
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
                for (spinLockIdx = pICB->isr2CriticalZoneCount; spinLockIdx > 0u; spinLockIdx--)
                {
                    if (OBJECT_SPINLOCK == pICB->isr2CriticalZoneType[pICB->isr2CriticalZoneCount - 1u])
                    {
                        SpinlockId = pICB->isr2CriticalZoneStack[pICB->isr2CriticalZoneCount - 1u];
                        (void)Os_ReleaseSpinlock(SpinlockId);
                    }
                    else
                    {
                        pICB->isr2CriticalZoneCount--;
                    }
                }
            }
#endif /* CFG_SPINLOCK_MAX > 0U */

            /*clear the CoreCB info*/
#if (OS_AUTOSAR_CORES > 1U)
            Os_GetInternalSpinlock(&Os_SpinlockSync);
#endif
            /* PRQA S 3442, 4397 ++ */ /* VL_Os_3442, VL_Os_4397 */
            Os_CoreCB.coreStateActive &= (Os_CoreIdType)(~((Os_CoreIdType)1u << Os_SCB.sysCore));
            /* PRQA S 3442, 4397 -- */
            for (i = 0; i < OS_AUTOSAR_CORES; i++) /* PRQA S 1290 */ /* VL_Os_1290 */
            {
                /* PRQA S 4404 ++ */ /* VL_QAC_AutosarBool */
                Os_CoreCB.coreStateSynPoint0[i] = FALSE;
                Os_CoreCB.coreStateSynPoint1[i] = FALSE;
                Os_CoreCB.coreStateSynPoint2[i] = FALSE;
                Os_CoreCB.coreStateSynPoint3[i] = FALSE;
                /* PRQA S 4404 -- */
            }
            Os_CoreCB.osAppMode[Os_SCB.sysCore]  = OS_NULL_APPMODE;
            Os_CoreCB.coreStatus[Os_SCB.sysCore] = OS_CORE_STATUS_INVALID;
#if (OS_AUTOSAR_CORES > 1U)
            Os_ReleaseInternalSpinlock(&Os_SpinlockSync);
#endif
            UNUSED_PARAMETER(Error);
            /* SWS_Os_00715*/
            for (;;) /* PRQA S 2870 */ /* VL_Os_2870 */
            {
            }
        }
    }
    /* PRQA S 2880 ++ */ /* VL_Os_2880 */
    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    /* PRQA S 2880 -- */ /* VL_Os_2880 */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/*=======[E N D   O F   F I L E]==============================================*/
