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
 **  @file               : Os_Core.c
 **  @author             : i-soft-os
 **  @date               : 2025/02/10
 **  @vendor             : isoft
 **  @description        : Os source file for Core API implementations
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Os_Arch_Processor.h"
#include "Os_Core.h"
#include "Os_Interrupt.h"
#include "Os_Spinlock.h"
#include "Os_Appl.h"
#include "Os_Task.h"
#include "Os_Sprot.h"
#include "Os_Rpc.h"
#include "Os_Kernel.h"
#include "Os_Hook.h"
#include "Os_Err.h"
#include "Os_Arti.h"

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ============================================ external data definitions =========================================== */
#define OS_START_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "Os_MemMap.h"
volatile Os_CoreCBType Os_CoreCB;
#define OS_STOP_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "Os_MemMap.h"
/* ============================================ internal data definitions =========================================== */

/* ========================================== internal function declarations ======================================== */
#if (OS_AUTOSAR_CORES > 1)
/**
 * @brief           RPC action handler for killing a core
 * @param[in]       inPara: Parameter array containing error code
 * @return          StatusType
 * @retval          E_OK: Operation successful
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_RpcAction_KillAllCore(uint32 *inPara);

/**
 * @brief           Makes RPC call to kill a core
 * @param[in]       ownerCore: Core ID to execute the call
 * @param[in]       Error: Error code to pass to shutdown hook
 * @return          StatusType
 * @retval          E_OK: Remote call completed successfully
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_RpcCall_KillAllCore(Os_CoreIdType ownerCore, StatusType Error);

/**
 * @brief           Sends kill command to all cores except the calling core
 * @param[in]       core: ID of the calling core
 * @param[in]       error: Error code to pass to shutdown hook
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_KillAllCore(Os_CoreIdType core, StatusType error);
#endif

/* ========================================== external function definitions ========================================= */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * The function starts the core specified by the CoreID.
 */
/* PRQA S 1532, 3408, 6030, 1503, 1512 ++ */ /* VL_QAC_OneFunRef, VL_Os_3408, VL_MTR_Os_STMIF, VL_QAC_NoUsedApi, VL_Os_1512 */
void StartCore(CoreIdType CoreID, StatusType *Status)
/* PRQA S 1532, 3408, 6030, 1503, 1512 -- */
{
    StatusType err = E_OK;
    CoreIdType logCoreId = CoreID;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_StartCore);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_StartCore_Start, CoreID);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141-- */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (NULL_PTR == Status)
    {
        err = E_OS_ILLEGAL_ADDRESS;
    }
    else
#endif

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(pScb, OS_CONTEXT_START_CORE) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    else
#endif
    {
#if (OS_STATUS_EXTENDED == CFG_STATUS)
        /* SWS_Os_00677:The function StartCore shall start one core
         * that shall run under the control of the AUTOSAR OS.*/
        if (OS_AUTOSAR_CORES <= logCoreId)
        {
            err = E_OS_ID;
        }
        /* SWS_Os_00606,SWS_Os_00678:StartCore is called after StartOS.*/
        /* PRQA S 3442, 3120, 1881 ++ */ /* VL_Os_3442, VL_QAC_MagicNum, VL_Os_1881 */
        else if (TRUE == Os_CoreCB.CoreStateSynPoint[2][pScb->SysCore])
        /* PRQA S 3442, 3120, 1881 -- */
        {
            err = E_OS_ACCESS;
        }
        /* SWS_Os_00679,SWS_Os_00680:If the parameter CoreIDs refers to a core
         * that was already started by the function StartCore the related core
         * is ignored and E_OS_STATE shall be returned.*/
        /* PRQA S 3442, 4397, 1881 ++ */ /* VL_Os_3442, VL_Os_4397, VL_Os_1881 */
        else if ((Os_CoreCB.CoreStateActive & ((CoreIdType)1U << logCoreId)) > 0u)
        /* PRQA S 3442, 4397, 1881 -- */
        {
            err = E_OS_STATE;
        }
        else
#endif
        {
            Os_Hal_StartCore(logCoreId);
            /* Set Activated core flag bit */
            /* PRQA S 3442 ++ */ /* VL_Os_3442 */
            Os_CoreCB.CoreStateActive |= (uint8)(1U << logCoreId);
            /* PRQA S 3442 -- */
            Os_CoreCB.CoreStatus[logCoreId] = OS_RUN;
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
        Os_TraceErrorHook(OSError_Save_StartCore(CoreID, Status),
                          OSServiceId_StartCore,
                          err, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
#endif
    }

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544, 1258, 2995, 2997 ++ */ /* VL_Os_3432, VL_Os_4544, VL_Os_1258, VL_Os_2995, VL_Os_2997 */
    OSRtiExitApi(pScb, OSApiId_StartCore);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_StartCore_Return, (err == E_OK ) ? *Status : (StatusType)0U);
    /* PRQA S 3432, 4544, 1258, 2995, 2997 -- */
    /* PRQA S 3138, 3141 -- */
    UNUSED_PARAMETER(pScb);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (CFG_CORE_MAX > 1)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * The function starts the core specified by the parameter
 *                       CoreID. It is allowed to call this function after
 *                       StartOS().
 */
/* PRQA S 6010,6070,6080 ++ */ /* VL_MTR_Os_STCYC, VL_MTR_Os_STCAL, VL_MTR_Os_STPTH */
/* PRQA S 1503,3006,3408,1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_Os_3408, VL_Os_1512 */
void StartNonAutosarCore(CoreIdType CoreID, StatusType *Status)
/* PRQA S 1503,3006,3408,1512 -- */
/* PRQA S 6010,6070,6080 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    CoreIdType logCoreId = CoreID;
    StatusType err = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_StartNonAutosarCore);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_StartNonAutosarCore_Start, CoreID);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141-- */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (NULL_PTR == Status)
    {
        err = E_OS_ILLEGAL_ADDRESS;
    }
    else
#endif

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(pScb, OS_CONTEXT_START_NON_AUTOSAR_CORE) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    else
#endif
    {
#if (OS_STATUS_EXTENDED == CFG_STATUS)
        /* SWS_Os_00685:If the parameter CoreID refers to an unknown core the
         * function StartNonAutosarCore has no effect and sets Status to E_OS_ID. */
        if (OS_AUTOSAR_CORES <= logCoreId)
        {
            err = E_OS_ID;
        }

        if ((StatusType)E_OK == err)
        {
            /* SWS_Os_00684:If the parameter CoreID refers to a core
             * that was already started by the function StartNonAutosarCore
             * has no effect and sets "Status" to E_OS_STATE.*/
            if (OS_CORE_MODE_HALT != Os_Hal_GetCoreMode(logCoreId))
            {
                err = E_OS_STATE;
            }
        }
#endif

        if ((StatusType)E_OK == err) /* PRQA S 2991, 2995 */ /* VL_Os_2991, VL_Os_2995 */
        {
            Os_Hal_StartCore(logCoreId);
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
        Os_TraceErrorHook(OSError_Save_StartNonAutosarCore(CoreID, Status),
                          OSServiceId_StartNonAutosarCore,
                          err, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
#endif
    }

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544, 2812 ++ */ /* VL_Os_3432, VL_Os_4544, VL_Os_2812 */
    OSRtiExitApi(pScb, OSApiId_StartNonAutosarCore);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_StartNonAutosarCore_Return, *Status);
    OS_HAL_EXIT_KERNEL();
    /* PRQA S 3432, 4544, 2812 -- */
    /* PRQA S 3138, 3141 -- */
    UNUSED_PARAMETER(pScb);

}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * This API allows the caller to select the idle mode
 *                       action which is performed during idle time of the OS
 */
/* PRQA S 6030,6010,6070 ++ */ /* VL_MTR_Os_STMIF, VL_MTR_Os_STCYC, VL_MTR_Os_STCAL */
/* PRQA S 1503,3006,3408,1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_Os_3408, VL_Os_1512 */
StatusType ControlIdle(CoreIdType CoreID, IdleModeType IdleMode)
/* PRQA S 1503,3006,3408,1512 -- */
/* PRQA S 6030,6010,6070 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    CoreIdType logCoreId = CoreID;
    StatusType err = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_ControlIdle);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ControlIdle_Start, IdleMode);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141-- */
    OS_HAL_DECLARE_CRITICAL();

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    Os_ServicePortParamType SprotParam = {
        .AllowedContext = OS_SERVICEPORT_CHECK_CONTROL_IDLE,
        .ObjectType = (Os_ObjectTypeType)OS_OBJECT_INVALID,
        .ObjectID = (Os_AppObjectId)OS_OBJECT_INVALID,
        /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral */
        .Address = NULL_PARA,
        /* PRQA S 1258 -- */
    };
    err = Os_ServiceProtCheck(pScb, &SprotParam);
    if ((StatusType)E_OK == err)
#endif
    {
#if (OS_STATUS_EXTENDED == CFG_STATUS)
        /* SWS_Os_00771:The function ControlIdle shall return E_OS_ID,
         * if the parameter CoreID or IdleMode is invalid. */
        if (OS_AUTOSAR_CORES <= logCoreId)
        {
            err = E_OS_ID;
        }
        else
#endif
        {
            OS_HAL_ENTRY_CRITICAL();
#if (OS_AUTOSAR_CORES > 1U)
            Os_GetInternalSpinlock(&Os_SpinlockSync);
#endif
            /* SWS_Os_00802:the new IdleMode shall become effective
             * the next time that core enters the idle mode. */
            /* PRQA S 3442, 4397 ++ */ /* VL_Os_3442, VL_Os_4397 */
            if ((Os_CoreCB.CoreStateActive & ((CoreIdType)1U << logCoreId)) > 0u)
            /* PRQA S 3442, 4397 -- */
            {
                switch (IdleMode)
                {
                case OS_IDLE_NO_HALT:
                    /* PRQA S 2487 ++ */ /* VL_Os_2487 */
                    while (FALSE == Os_Hal_SetCoreMode(logCoreId, OS_CORE_MODE_IDLE))
                    /* PRQA S 2487 -- */
                    {
                        /* Nothing to do. */
                    }
                    Os_CoreCB.CoreStatus[logCoreId] = OS_IDLE_NO_HALT;
                    break;

                case OS_RUN:
                    /* PRQA S 2487 ++ */ /* VL_Os_2487 */
                    while (FALSE == Os_Hal_SetCoreMode(logCoreId, OS_CORE_MODE_RUN))
                    /* PRQA S 2487 -- */
                    {
                        /* Nothing to do. */
                    }
                    Os_CoreCB.CoreStatus[logCoreId] = OS_RUN;
                    break;

                default:
                    err = (StatusType)E_OS_ID;
                    break;
                }
            }
            else
            {
                err = (StatusType)E_OS_ID;
            }
#if (OS_AUTOSAR_CORES > 1U)
            Os_ReleaseInternalSpinlock(&Os_SpinlockSync);
#endif
            OS_HAL_EXIT_CRITICAL();
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_ControlIdle(CoreID, IdleMode),
                          OSServiceId_ControlIdle,
                          err, pScb);/* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_ControlIdle);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ControlIdle_Return, err);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    UNUSED_PARAMETER(pScb);

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * The function returns a unique core identifier.
 */
/* PRQA S 1503,3006,3408,1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_Os_3408, VL_Os_1512 */
CoreIdType GetCoreID(void)
/* PRQA S 1503,3006,3408,1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 1259, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_1259, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_GetCoreID);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_GetCoreID_Start, 0);
    /* PRQA S 1317, 1259, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

    CoreIdType logCoreId = Os_Hal_GetCoreID();

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_GetCoreID);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_GetCoreID_Return, logCoreId);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */

    UNUSED_PARAMETER(pScb);
    return logCoreId;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * The function returns the number of cores activated by
 *                          the StartCore function.
 */
/* PRQA S 1503, 3006, 3408, 1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_Os_3408, VL_Os_1512 */
uint32 GetNumberOfActivatedCores(void)
/* PRQA S 1503, 3006, 3408, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    uint32 numOfActivatedCores = 0U;
    StatusType status = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 1259, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_1259, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_GetNumberOfActivatedCores);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_GetNumberOfActivatedCores_Start, 0);
    /* PRQA S 1317, 1259, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

/*service protection*/
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    Os_ServicePortParamType SprotParam = {
        .AllowedContext = OS_SERVICEPORT_CHECK_GET_ACTIVATEED_CORE_NUMBER,
        .ObjectType = (Os_ObjectTypeType)OS_OBJECT_INVALID,
        .ObjectID = (Os_AppObjectId)OS_OBJECT_INVALID,
        /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral */
        .Address = NULL_PARA,
        /* PRQA S 1258 -- */
    };
    status = Os_ServiceProtCheck(pScb, &SprotParam);
    if (E_OK == status)
#endif
    {
        for (Os_CoreIdType loopCore = 0u; loopCore < OS_AUTOSAR_CORES; loopCore++)
        {
            /* PRQA S 3442, 4397 ++ */ /* VL_Os_3442, VL_Os_4397 */
            if ((Os_CoreCB.CoreStateActive & ((Os_CoreIdType)1u << loopCore)) > 0u)
            /* PRQA S 3442, 4397 -- */
            {
                numOfActivatedCores++;
            }
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK) /* PRQA S 2992, 2996 */ /* VL_Os_2992, VL_Os_2996 */
    {
        Os_TraceErrorHook(OSError_Save_GetNumberOfActivatedCores(), /* PRQA S 2880 */ /* VL_Os_2880 */
                          OSServiceId_GetNumberOfActivatedCores,
                          status, pScb);/* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_GetNumberOfActivatedCores);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_GetNumberOfActivatedCores_Return, numOfActivatedCores);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    UNUSED_PARAMETER(pScb);
    UNUSED_PARAMETER(status);

    return numOfActivatedCores;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (OS_AUTOSAR_CORES > 1)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
OS_LOCAL StatusType Os_RpcAction_KillAllCore(uint32 *inPara)
/* PRQA S 3673 -- */
{
    Os_ShutdownOS((StatusType)inPara[0], OS_SHUTDOWN_ALL_OS);
    return E_OK;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
OS_LOCAL StatusType Os_RpcCall_KillAllCore(
    Os_CoreIdType ownerCore,
    StatusType Error)
{
    StatusType err = E_OK;
    Os_RpcInputType rpcData = {
        .RpcSync = OS_RPC_ASYNC,
        .RemoteCoreId = ownerCore,
        .ActionFn = Os_RpcAction_KillAllCore,
        /* PRQA S 0691 ++ */ /* VL_Os_0691 */
        .InPara[0] = (StatusType)Error,
        /* PRQA S 0691 -- */
    };/* PRQA S 0704 */ /* VL_Os_0704 */

    err = Os_RpcCallService(&rpcData);
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#if (OS_AUTOSAR_CORES > 1)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * The function kill all core.
 */
OS_LOCAL void Os_KillAllCore(Os_CoreIdType core, StatusType error)
{
    if (core >= OS_AUTOSAR_CORES)
    {
        Os_Panic();
    }

    for (uint8 coreId = 0U; (coreId < OS_AUTOSAR_CORES); coreId++)
    {
        if (coreId != core)
        {
            (void)Os_RpcCall_KillAllCore(coreId, error);
        }
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * After this service the OS on all AUTOSAR cores is shut down.
 */
/* PRQA S 1503, 3006, 3408, 6070 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_Os_3408, VL_MTR_Os_STCAL */
/* PRQA S 6080, 6030, 6010, 1512 ++ */ /* VL_MTR_Os_STPTH, VL_MTR_Os_STMIF, VL_MTR_Os_STCYC, VL_Os_1512 */
void ShutdownAllCores(StatusType Error)  /* PRQA S 2755 */ /* VL_Os_2755 */
/* PRQA S 6080, 6030, 6010, 1512 -- */
/* PRQA S 1503, 3006, 3408, 6070 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_ShutdownAllCores);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ShutdownAllCores_Start, Error);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141-- */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    StatusType err = E_OK;
    Os_ServicePortParamType SprotParam = {
        .AllowedContext = OS_SERVICEPORT_CHECK_SHUTDOWN_ALLCORES,
        .ObjectType = (Os_ObjectTypeType)OS_OBJECT_INVALID,
        .ObjectID = (Os_AppObjectId)OS_OBJECT_INVALID,
        /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral */
        .Address = NULL_PARA,
        /* PRQA S 1258 -- */
    };
    err = Os_ServiceProtCheck(pScb, &SprotParam);
    if (E_OK == err)
#endif
    {
/* SWS_Os_00716:If ShutdownAllCores is called from
 * non trusted code the call shall be ignored.*/
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
        if (FALSE == Os_AppCfg[pScb->SysRunningAppId].Trusted)
        {
            /*nothing to do*/
        }
        else
#endif
        {
            Os_CoreCB.CoreStatus[pScb->SysCore] = OS_CORE_PRE_SHUTDOWN;
#if (OS_AUTOSAR_CORES > 1)
            /* SWS_Os_00762,SWS_Os_00714. */
            Os_KillAllCore(pScb->SysCore, Error);
#endif

/* SWS_Os_00586: Application specific hook. */
#if ((CFG_OSAPPLICATION_MAX > 0U) && (TRUE == CFG_APPL_SHUTDOWNHOOK))
            Os_ApplShutdownHook(pScb, Error);
#endif

            /* Third synchronization */
            /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
            Os_SynPoint(3U);
            /* PRQA S 3120 -- */

            Os_Hal_DisableInt(); /* PRQA S 1753, 1512, 1513 */ /* VL_Os_1753, VL_Os_1512, VL_Os_1513 */
/* SWS_Os_00588: Global ShutdownHook. */
#if (TRUE == CFG_SHUTDOWNHOOK)
            Os_CallShutdownHook(pScb, Error);
#endif

/*Os_00620*/
#if (CFG_SPINLOCK_MAX > 0U)
            SpinlockIdType spinlockId;
            Os_SpinlockType spinLockIdx;
            Os_TaskType taskIdStartRange = Os_TaskIdRange[pScb->SysCore].AllTask.Start;
            Os_TaskType taskIdEndRange = Os_TaskIdRange[pScb->SysCore].AllTask.End;
            for (uint16 i = (uint16)taskIdStartRange; i < taskIdEndRange; i++) /* PRQA S 4424, 1880 */ /* VL_Os_4424, VL_Os_1880 */
            {
                Os_TCBType *pTCB = Os_TCB[i];
                for (spinLockIdx = pTCB->TaskCriticalZoneCount; spinLockIdx > 0u; spinLockIdx--)
                {
                    if (OS_OBJECT_SPINLOCK == pTCB->TaskCriticalZoneType[pTCB->TaskCriticalZoneCount - 1u])
                    {
                        spinlockId = pTCB->TaskCriticalZoneStack[pTCB->TaskCriticalZoneCount - 1u]; /* PRQA S 4442 */ /* VL_Os_4442 */
                        (void)Os_ReleaseSpinlock(pScb, spinlockId);
                    }
                    else
                    {
                        pTCB->TaskCriticalZoneCount--;
                    }
                }
            }

            Os_IsrType isrIdStartRange = Os_IsrIdRange[pScb->SysCore].Isr2.IsrStart;
            Os_IsrType isrIdEndRange = Os_IsrIdRange[pScb->SysCore].Isr2.IsrEnd;
            for (uint16 i = (uint16)isrIdStartRange; i < isrIdEndRange; i++) /* PRQA S 4424, 1880 */ /* VL_Os_4424, VL_Os_1880 */
            {
                Os_ICBType *pICB = Os_ICB[i];
                for (spinLockIdx = pICB->Isr2CriticalZoneCount; spinLockIdx > 0u; spinLockIdx--)
                {
                    if (OS_OBJECT_SPINLOCK == pICB->Isr2CriticalZoneType[pICB->Isr2CriticalZoneCount - 1u])
                    {
                        spinlockId = pICB->Isr2CriticalZoneStack[pICB->Isr2CriticalZoneCount - 1u]; /* PRQA S 4442 */ /* VL_Os_4442 */
                        (void)Os_ReleaseSpinlock(pScb, spinlockId);
                    }
                    else
                    {
                        pICB->Isr2CriticalZoneCount--;
                    }
                }
            }
#endif

/*clear the CoreCB info*/
#if (OS_AUTOSAR_CORES > 1U)
            Os_GetInternalSpinlock(&Os_SpinlockSync);
#endif
            /* PRQA S 3442, 4397 ++ */ /* VL_Os_3442, VL_Os_4397 */
            Os_CoreCB.CoreStateActive &= (Os_CoreIdType)(~((Os_CoreIdType)1u << pScb->SysCore));
            /* PRQA S 3442, 4397 -- */
            for (uint16 i = 0U; i < OS_AUTOSAR_CORES; i++)
            {
                for (uint16 j = 0U; j < SYNC_POINTS_MAX; j++)
                {
                    /* PRQA S 4404 ++ */ /* VL_Os_4404 */
                    Os_CoreCB.CoreStateSynPoint[j][i] = FALSE;
                    /* PRQA S 4404 -- */
                }
            }
            Os_CoreCB.AppMode[pScb->SysCore] = OS_NULL_APPMODE;
            Os_CoreCB.CoreStatus[pScb->SysCore] = OS_CORE_STATUS_INVALID;
#if (OS_AUTOSAR_CORES > 1U)
            Os_ReleaseInternalSpinlock(&Os_SpinlockSync);
#endif

            /* SWS_Os_00715*/
            for (;;)
            {
            }
        }
    }
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544, 1259, 2880 ++ */ /* VL_Os_3432, VL_Os_4544, VL_Os_1259, VL_Os_2880 */
    OSRtiExitApi(pScb, OSApiId_ShutdownAllCores);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ShutdownAllCores_Return, 0);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544, 1259, 2880 -- */
    /* PRQA S 3138, 3141 -- */

    UNUSED_PARAMETER(Error);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/*=======[E N D   O F   F I L E]==============================================*/
