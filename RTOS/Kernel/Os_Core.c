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
/* PRQA S 3406 ++ */ /* MISRA Rule 8.6 */ /* OS_CORE_VAR_DEFINE_001 */
#include "Os_Internal.h"
/* PRQA S 3406 -- */ /* MISRA Rule 8.6 */

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
FUNC(void, OS_CODE) StartCore(CoreIdType CoreID, StatusType* Status)
{
    VAR(StatusType, OS_VAR) err = E_OK;
    VAR(CoreIdType, OS_VAR) logCoreId = Os_GetCoreLogID(CoreID);

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
        if (TRUE == Os_CoreCB.coreStateSynPoint2[Os_SCB.sysCore])
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
        else if ((Os_CoreCB.coreStateActive & ((CoreIdType)1U << logCoreId)) > 0u)
        {
            err = E_OS_STATE;
        }
        else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
        {
            Os_ArchStartCore(CoreID);
            /* Set Activated core flag bit */
            Os_CoreCB.coreStateActive |= (uint8)(1U << logCoreId);
            Os_CoreCB.coreStatus[logCoreId] = OS_RUN;
            *Status = E_OK;
        }
    }

    if (err != E_OK)
    {
        if (NULL_PTR != Status)
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
FUNC(void, OS_CODE) StartNonAutosarCore(CoreIdType CoreID, StatusType* Status)
{
    OS_ENTER_KERNEL();
    VAR(StatusType, OS_VAR) err = E_OK;
#if (OS_STATUS_EXTENDED == CFG_STATUS)
    VAR(CoreIdType, OS_VAR) logCoreId;

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

        if ((StatusType)E_OK == err)
        {
            Os_ArchStartCore(CoreID);
            *Status = E_OK;
        }
    }

    if (err != E_OK)
    {
        if (NULL_PTR != Status)
        {
            *Status = err;
        }

#if (CFG_ERRORHOOK == TRUE)
        Os_TraceErrorHook(OSError_Save_StartNonAutosarCore(CoreID, Status), OSServiceId_StartNonAutosarCore, err);
#endif
    }

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_CORE_MACRO_003 */
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
FUNC(StatusType, OS_CODE) ControlIdle(CoreIdType CoreID, IdleModeType IdleMode)
{
    OS_ENTER_KERNEL();
    VAR(CoreIdType, OS_VAR) logCoreId = Os_GetCoreLogID(CoreID);
    VAR(StatusType, OS_VAR) rst = E_OK;
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
            OS_ARCH_ENTRY_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_CORE_MACRO_003 */
#if (OS_AUTOSAR_CORES > 1U)
            Os_GetInternalSpinlock(&Os_SpinlockSync);
#endif
            /* SWS_Os_00802:the new IdleMode shall become effective
             * the next time that core enters the idle mode. */
            if ((Os_CoreCB.coreStateActive & ((CoreIdType)1U << logCoreId)) > 0u)
            {
                switch (IdleMode)
                {
                case OS_IDLE_NO_HALT:
                    while (FALSE == Os_SetCoreMode(CoreID, OS_CORE_MODE_IDLE))
                    {
                        /* Nothing to do. */
                    }
                    Os_CoreCB.coreStatus[logCoreId] = OS_IDLE_NO_HALT;
                    break;

                case OS_RUN:
                    while (FALSE == Os_SetCoreMode(CoreID, OS_CORE_MODE_RUN))
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
            OS_ARCH_EXIT_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_CORE_MACRO_003 */
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (rst != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_ControlIdle(CoreID, IdleMode), OSServiceId_ControlIdle, rst);
    }
#endif

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_CORE_MACRO_003 */

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
FUNC(CoreIdType, OS_CODE) GetCoreID(void)
{
    OS_ENTER_KERNEL();
    VAR(CoreIdType, OS_VAR) phyCoreId;
    VAR(CoreIdType, OS_VAR) logCoreId;

    phyCoreId = Os_ArchGetCoreID();
    logCoreId = Os_GetCoreLogID(phyCoreId);

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_CORE_MACRO_003 */

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
FUNC(uint32, OS_CODE) GetNumberOfActivatedCores(void)
{
    OS_ENTER_KERNEL();
    VAR(uint32, OS_VAR) numOfActivatedCores = 0U;
    VAR(Os_CoreIdType, OS_VAR) loop_core;
    VAR(StatusType, OS_VAR) status = E_OK;

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
            if ((Os_CoreCB.coreStateActive & ((Os_CoreIdType)1u << loop_core)) > 0u)
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

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_CORE_MACRO_003 */
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
static FUNC(void, OS_CODE) Os_KillAllCore(Os_CoreIdType core, StatusType Error)
{
    VAR(uint8, OS_VAR) coreId;

    if (core >= OS_AUTOSAR_CORES)
    {
        Os_Panic();
    }

    for (coreId = 0U; (coreId < OS_AUTOSAR_CORES); coreId++)
    {
        if (coreId != core)
        {
            RpcInputType rpcData = {
                .sync = RPC_ASYNC,
                .remoteCoreId = coreId,
                .serviceId = OSServiceId_ShutdownOS,
                .srvPara0 = Error,
                .srvPara1 = (uint32)NULL_PARA,
                .srvPara2 = (uint32)NULL_PARA,
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
FUNC(void, OS_CODE) ShutdownAllCores(StatusType Error)
{
    OS_ENTER_KERNEL();

    VAR(Os_TaskType, OS_VAR) i;
#if (CFG_SPINLOCK_MAX > 0U)
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_CORE_COMPILER_002 */
    /* PRQA S 3678 ++ */ /* MISRA  Rule-8.13*/ /* OS_APPL_CONST_004 */
    P2VAR(Os_TCBType, AUTOMATIC, OS_VAR) pTCB;
    P2VAR(Os_ICBType, AUTOMATIC, OS_VAR) pICB;
    /* PRQA S 3678 -- */ /* MISRA  Rule-8.13*/
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    VAR(SpinlockIdType, OS_VAR) SpinlockId;
    VAR(Os_SpinlockType, OS_VAR) spinLockIdx;
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
            Os_SynPoint(3);

            OS_DisableInterrupts(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_CORE_MACRO_003 */
/* SWS_Os_00588: Global ShutdownHook. */
#if (TRUE == CFG_SHUTDOWNHOOK)
            Os_CallShutdownHook(Error);
#endif

/*Os_00620*/
#if (CFG_SPINLOCK_MAX > 0U)
            for (i = 0; i < (Os_SCB.sysTaskMax - 1u); i++)
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

            for (i = 0; i < Os_CfgIsr2Max; i++)
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
            Os_CoreCB.coreStateActive &= (Os_CoreIdType)(~((Os_CoreIdType)1u << Os_SCB.sysCore));
            for (i = 0; i < OS_AUTOSAR_CORES; i++)
            {
                /* PRQA S 4404 ++ */ /* MISRA Rule 10.3 */ /* OS_CORE_TYPE_004 */
                Os_CoreCB.coreStateSynPoint0[i] = FALSE;
                Os_CoreCB.coreStateSynPoint1[i] = FALSE;
                Os_CoreCB.coreStateSynPoint2[i] = FALSE;
                Os_CoreCB.coreStateSynPoint3[i] = FALSE;
                /* PRQA S 4404 -- */ /* MISRA Rule 10.3 */
            }
            Os_CoreCB.osAppMode[Os_SCB.sysCore] = OS_NULL_APPMODE;
            Os_CoreCB.coreStatus[Os_SCB.sysCore] = OS_CORE_STATUS_INVALID;
#if (OS_AUTOSAR_CORES > 1U)
            Os_ReleaseInternalSpinlock(&Os_SpinlockSync);
#endif

            /* SWS_Os_00715*/
            for (;;)
            {
            }
        }
    }
    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_CORE_MACRO_003 */

    UNUSED_PARAMETER(Error);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/*=======[E N D   O F   F I L E]==============================================*/
