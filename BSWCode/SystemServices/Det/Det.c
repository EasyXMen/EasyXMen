/* PRQA S 3108++ */
/**
 * Copyright (C) 2008-2025 isoft Infrastructure Software Co., Ltd.
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
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : Det.c                                                       **
**                                                                            **
**  Created on  :                                                             **
**  Author      : Haibin.shao                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Type definition and API declaration for DET                 **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                       **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:Det<br>
  RuleSorce:puhua-rule.rcf 2.3.1

    \li PRQA S 3206 MISRA Rule 2.7 .<br>
    Reason:Variables are used in different configurations due to complex precompilation logic.

    \li PRQA S 0791 MISRA Rule 5.4 .<br>
    Reason:Configuration data design is required.

    \li PRQA S 3408 MISRA Rule 8.4 .<br>
    Reason:Configuration structure design needs.

    \li PRQA S 1504 MISRA Rule 8.7 .<br>
    Reason:Variables are used in different configurations due to complex precompilation logic.

    \li PRQA S 3218 MISRA Rule 8.9 .<br>
    Reason:Use the macro definition symbol instead of the static keyword

    \li PRQA S 3432 MISRA Rule 20.7 .<br>
    Reason:Function-like macros are used to allow more efficient code.
 */
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Det_Types.h"
#include "Det.h"
#include "Det_Cfg.h"
#include "Det_Externals.h"
#include "SchM_Det.h"

#if (DET_FORWARD_TO_DLT == STD_ON)
#include "Dlt.h"
#endif

/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#ifndef DET_AR_RELEASE_MAJOR_VERSION
#error " DET version miss"
#endif
#ifndef DET_AR_RELEASE_MINOR_VERSION
#error " DET version miss"
#endif
#ifndef DET_AR_RELEASE_PATCH_VERSION
#error " DET version miss"
#endif

/*******************************************************************************
**                      Private Data Definitions                              **
*******************************************************************************/
#ifndef DET_LOCAL
#define DET_LOCAL static
#endif

/* PRQA S 2053,3472++ */ /* MISRA Rule 18.8, Dir 4.9 */
#ifndef DET_UNUSED
#define DET_UNUSED(a) ((void)(a))
#endif                   /* #ifndef NVM_UNUSED */
/* PRQA S 2053,3472-- */ /* MISRA Rule 18.8, Dir 4.9 */

#define DET_START_SEC_VAR_POWER_ON_INIT_BOOLEAN
#include "Det_MemMap.h"
DET_LOCAL VAR(boolean, DET_VAR) detModuleInit = (boolean)FALSE;
#define DET_STOP_SEC_VAR_POWER_ON_INIT_BOOLEAN
#include "Det_MemMap.h"

#if (STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT)
#define DET_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED /* PRQA S 0791 */ /* MISRA Rule 5.4 */
#include "Det_MemMap.h"
DET_LOCAL VAR(Det_StateType, DET_VAR) Det_RunState = DET_UNINITIALIZED;
#define DET_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED /* PRQA S 0791 */ /* MISRA Rule 5.4 */
#include "Det_MemMap.h"
#endif /* STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT */

#define DET_START_SEC_VAR_NO_INIT_8BIT
#include "Det_MemMap.h"
/* recursion limit counter \trace DSGN-Det22792 */
DET_LOCAL VAR(uint8, DET_VAR) detRecursionCounterReportError;
/* recursion limit counter \trace DSGN-Det22792 */
DET_LOCAL VAR(uint8, DET_VAR) detRecursionCounterReportRuntimeError;
/* recursion limit counter \trace DSGN-Det22792 */
DET_LOCAL VAR(uint8, DET_VAR) detRecursionCounterReportTransientFault;
#define DET_STOP_SEC_VAR_NO_INIT_8BIT
#include "Det_MemMap.h"

/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/
#if (STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT)

#define DET_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Det_MemMap.h"
/* global variables, should be put in a watch window */
#if (DET_SIZE_OF_GLOBAL_FILTER > 0)
/* use this variable to configure global filters */
volatile VAR(DetInfoType, DET_VAR) detGlobalFilter[DET_SIZE_OF_GLOBAL_FILTER];
#endif /* DET_GLOBALFILTERSIZE > 0 */

#if (DET_SIZE_OF_BREAK_FILTER > 0)
/* use this variable to configure break filters */
volatile VAR(DetInfoType, DET_VAR) detBreakFilter[DET_SIZE_OF_BREAK_FILTER];
#endif /* DET_BREAKFILTERSIZE > 0 */

#if (DET_SIZE_OF_DLT_FILTER > 0)
/* use this variable to configure dlt filters */
volatile VAR(DetInfoType, DET_VAR) detDltFilter[DET_SIZE_OF_DLT_FILTER];
#endif /* DET_DLTFILTERSIZE > 0 */

#if (DET_SIZE_OF_LOG_BUFFER > 0)
/* use this variable to view log buffer */
volatile VAR(DetInfoType, DET_VAR) detLogBuffer[DET_SIZE_OF_LOG_BUFFER];
#endif /* DET_LOGBUFFERSIZE > 0 */
#define DET_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Det_MemMap.h"

#if (DET_SIZE_OF_ERROR_HOOK_TABLE > 0)
#define DET_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Det_MemMap.h"
Det_CallbackFncType Det_CallbackList[DET_SIZE_OF_ERROR_HOOK_TABLE];
#define DET_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Det_MemMap.h"

#if (DET_MAX_RAMLOG_SIZE > 0)
#define DET_START_SEC_VAR_NO_INIT_32BIT
#include "Det_MemMap.h"
/* Ram log variables in uninitialized memory */
SECTION_RAMLOG uint32 Det_RamlogIndex;
#define DET_STOP_SEC_VAR_NO_INIT_32BIT
#include "Det_MemMap.h"

#define DET_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Det_MemMap.h"
/* Supress lintwarning about Det_Ramlog not being accessed */
SECTION_RAMLOG Det_EntryType Det_RamLog[DET_MAX_RAMLOG_SIZE];
#define DET_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Det_MemMap.h"
#endif /* DET_MAX_RAMLOG_SIZE > 0 */

#endif /* DET_SIZE_OF_ERROR_HOOK_TABLE > 0 */

#if (DET_SIZE_OF_RUNTIME_ERROR_CALLOUT_TABLE > 0)

#define DET_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Det_MemMap.h"
Det_CallbackFncType Det_RunTimeCallbackList[DET_SIZE_OF_RUNTIME_ERROR_CALLOUT_TABLE];
#define DET_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Det_MemMap.h"

#if (DET_RUNTIME_MAX_RAMLOG_SIZE > 0)
#define DET_START_SEC_VAR_NO_INIT_32BIT
#include "Det_MemMap.h"
/* Ram log variables in uninitialized memory */
SECTION_RAMLOG uint32 Det_RunTimeRamlogIndex;
#define DET_STOP_SEC_VAR_NO_INIT_32BIT
#include "Det_MemMap.h"

#define DET_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Det_MemMap.h"
/* Supress lintwarning about Det_Ramlog not being accessed */
SECTION_RAMLOG Det_EntryType Det_RunTimeRamLog[DET_RUNTIME_MAX_RAMLOG_SIZE];
#define DET_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Det_MemMap.h"
#endif /* DET_RUNTIME_MAX_RAMLOG_SIZE > 0 */
#endif /* DET_SIZE_OF_RUNTIME_ERROR_CALLOUT_TABLE > 0 */

#if (DET_SIZE_OF_TRANSIENT_FAULT_CALLOUT_TABLE > 0)
#define DET_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Det_MemMap.h"
Det_CallbackFncType Det_TransientCallbackList[DET_SIZE_OF_TRANSIENT_FAULT_CALLOUT_TABLE];
#define DET_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Det_MemMap.h"

#if (DET_TRANSIENT_MAX_RAMLOG_SIZE > 0)
#define DET_START_SEC_VAR_NO_INIT_32BIT
#include "Det_MemMap.h"
/* Ram log variables in uninitialized memory */
SECTION_RAMLOG uint32 Det_TransientRamlogIndex;
#define DET_STOP_SEC_VAR_NO_INIT_32BIT
#include "Det_MemMap.h"

#define DET_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Det_MemMap.h"
/* Supress lintwarning about Det_Ramlog not being accessed */
SECTION_RAMLOG Det_EntryType Det_TransientRamLog[DET_TRANSIENT_MAX_RAMLOG_SIZE];
#define DET_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Det_MemMap.h"

#endif /* DET_TRANSIENT_MAX_RAMLOG_SIZE > 0 */
#endif /* DET_SIZE_OF_TRANSIENT_FAULT_CALLOUT_TABLE > 0 */

#endif /* STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT */

#define DET_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Det_MemMap.h"
/* use this variable to configure DET debug operation */
/* PRQA S 1504,3218 ++*/ /* MISRA Rule 8.7,8.9 */
DET_LOCAL volatile VAR(DetStatusType, DET_VAR) detStatus;
/* PRQA S 1504,3218 --*/ /* MISRA Rule 8.7,8.9 */
#define DET_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Det_MemMap.h"

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define DET_START_SEC_CODE
#include "Det_MemMap.h"

#if (STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT)
DET_LOCAL FUNC(boolean, DET_CODE) Det_CheckFilterMatch(
    volatile P2CONST(DetInfoType, AUTOMATIC, DET_VAR) filter,
    VAR(uint8, AUTOMATIC) size,
    VAR(boolean, AUTOMATIC) active,
    P2CONST(DetInfoType, AUTOMATIC, DET_VAR) detInfo);
DET_LOCAL FUNC(boolean, DET_CODE)
    Det_LogError(VAR(boolean, AUTOMATIC) active, P2CONST(DetInfoType, AUTOMATIC, DET_VAR) detInfo);
#endif /* STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT */

#define DET_STOP_SEC_CODE
#include "Det_MemMap.h"

#define DET_START_SEC_CODE
#include "Det_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/

#if (STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT)

/******************************************************************************/
/*
 * Brief               Check if a DET error matches filter conditions
 * details             A filter match is detected if all structure members of at least one filter and
                       detInfo are identical or the non-identical ones are don't care (0xFF).
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      filter: pointer to an array of filters
                       size: number of filters
                       active: flag to indicate if filter is active
                       detInfo: pointer to structure holding DET error for filtering
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              boolean: FALSE: no filter match, TRUE: filter match
 */
/******************************************************************************/
DET_LOCAL FUNC(boolean, DET_CODE) Det_CheckFilterMatch(
    volatile P2CONST(DetInfoType, AUTOMATIC, DET_VAR) filter,
    VAR(uint8, AUTOMATIC) size,
    VAR(boolean, AUTOMATIC) active,
    P2CONST(DetInfoType, AUTOMATIC, DET_VAR) detInfo)
{
    VAR(boolean, AUTOMATIC) retval = (boolean)FALSE;
#if (STD_ON == DET_CHECK_FILTER_MATCH_ENABLE)
    VAR(uint32, AUTOMATIC) iloop;
    /* if filter is active */
    if ((boolean)TRUE == active)
    {
        /* linear search over all filters */
        for (iloop = 0U; iloop < size; iloop++)
        {
            /* compare each structure member if it is identical or if filter is set to don't care */
            if (((filter[iloop].mModuleId == detInfo->mModuleId) || (filter[iloop].mModuleId == DET_DONTCARE_16))
                && ((filter[iloop].mInstanceId == detInfo->mInstanceId)
                    || (filter[iloop].mInstanceId == DET_DONTCARE_8))
                && ((filter[iloop].mApiId == detInfo->mApiId) || (filter[iloop].mApiId == DET_DONTCARE_8))
                && ((filter[iloop].mErrorId == detInfo->mErrorId) || (filter[iloop].mErrorId == DET_DONTCARE_8)))
            {
                retval = (boolean)TRUE;
                break;
            }
        }
    }
    else
    {
        retval = (boolean)TRUE;
    }
#else
    (void)filter;
    (void)size;
    (void)active;
    (void)detInfo;
#endif /* STD_ON == DET_CHECK_FILTER_MATCH_ENABLE */
    return retval;
}

/******************************************************************************/
/*
 * Brief               Store a DET error in the log buffer
 * details             It is further detected if an overrun occurs and if a forced break handler call is required.
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      active:flag to indicate if logging is active
                       detInfo: pointer to structure holding DET error for filtering
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              boolean: FALSE: do not force break handler call, TRUE: force break handler call
 */
/******************************************************************************/
DET_LOCAL FUNC(boolean, DET_CODE)
    Det_LogError(VAR(boolean, AUTOMATIC) active, P2CONST(DetInfoType, AUTOMATIC, DET_VAR) detInfo)
{
    /* do not force break handler call */
    VAR(boolean, AUTOMATIC) forceBreakHandler = (boolean)FALSE;
    /* if logging is active */
    if ((boolean)TRUE == active)
    {
        SchM_Enter_Det_LogError();
        detStatus.logIndex++;
        /* if overrun of circular buffer occurs */
        if (detStatus.logIndex >= (uint8)DET_SIZE_OF_LOG_BUFFER)
        {
            detStatus.logIndex = 0U;
            /* if break on overrun is active set return value to force break handler call */
            if ((boolean)TRUE == detStatus.breakOnLogOverrun)
            {
                forceBreakHandler = (boolean)TRUE;
            }
        }
        /* store DET error in log buffer */
        detLogBuffer[detStatus.logIndex].mModuleId = detInfo->mModuleId;
        detLogBuffer[detStatus.logIndex].mInstanceId = detInfo->mInstanceId;
        detLogBuffer[detStatus.logIndex].mApiId = detInfo->mApiId;
        detLogBuffer[detStatus.logIndex].mErrorId = detInfo->mErrorId;
        SchM_Exit_Det_LogError();
    }
    return forceBreakHandler;
}

#endif /* STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT */

#define DET_STOP_SEC_CODE
#include "Det_MemMap.h"

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define DET_START_SEC_CODE
#include "Det_MemMap.h"
/******************************************************************************/
/*
 * Brief               Initializes the DET
 * details             Global function as an interface from EcuM to DET for initialization.
 * note                Shall be called during startup by EcuM in a safe context.
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ConfigPtr: Pointer to the selected configuration set.
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              None:
 */
/******************************************************************************/
FUNC(void, DET_CODE) Det_Init(P2CONST(Det_ConfigType, AUTOMATIC, DET_APPL_CONST) ConfigPtr)
{
    DET_UNUSED(ConfigPtr); /* PRQA S 3472,3469 */ /* MISRA Dir 4.9 */
#if (STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT)
#if (DET_SIZE_OF_ERROR_HOOK_TABLE > 0)
    for (uint32 iloop = 0; iloop < DET_SIZE_OF_ERROR_HOOK_TABLE; iloop++)
    {
        Det_CallbackList[iloop] = NULL_PTR;
    }
#if (DET_MAX_RAMLOG_SIZE > 0)
    for (uint32 iloop = 0; iloop < DET_MAX_RAMLOG_SIZE; iloop++)
    {
        Det_RamLog[iloop].moduleId = 0;
        Det_RamLog[iloop].instanceId = 0;
        Det_RamLog[iloop].apiId = 0;
        Det_RamLog[iloop].errorId = 0;
    }
    Det_RamlogIndex = 0;
#endif /* DET_MAX_RAMLOG_SIZE > 0 */
#endif /* DET_SIZE_OF_ERROR_HOOK_TABLE > 0 */
#if (DET_SIZE_OF_RUNTIME_ERROR_CALLOUT_TABLE > 0)
    for (uint32 iloop = 0; iloop < DET_SIZE_OF_RUNTIME_ERROR_CALLOUT_TABLE; iloop++)
    {
        Det_RunTimeCallbackList[iloop] = NULL_PTR;
    }
#if (DET_RUNTIME_MAX_RAMLOG_SIZE > 0)
    for (uint32 iloop = 0; iloop < DET_RUNTIME_MAX_RAMLOG_SIZE; iloop++)
    {
        Det_RunTimeRamLog[iloop].moduleId = 0;
        Det_RunTimeRamLog[iloop].instanceId = 0;
        Det_RunTimeRamLog[iloop].apiId = 0;
        Det_RunTimeRamLog[iloop].errorId = 0;
    }
    Det_RunTimeRamlogIndex = 0;
#endif /* DET_RUNTIME_MAX_RAMLOG_SIZE > 0 */
#endif /* DET_SIZE_OF_RUNTIME_ERROR_CALLOUT_TABLE > 0 */
#if (DET_SIZE_OF_TRANSIENT_FAULT_CALLOUT_TABLE > 0)
    for (uint32 iloop = 0; iloop < DET_SIZE_OF_TRANSIENT_FAULT_CALLOUT_TABLE; iloop++)
    {
        Det_TransientCallbackList[iloop] = NULL_PTR;
    }
#if (DET_TRANSIENT_MAX_RAMLOG_SIZE > 0)
    for (uint32 iloop = 0; iloop < DET_TRANSIENT_MAX_RAMLOG_SIZE; iloop++)
    {
        Det_TransientRamLog[iloop].moduleId = 0;
        Det_TransientRamLog[iloop].instanceId = 0;
        Det_TransientRamLog[iloop].apiId = 0;
        Det_TransientRamLog[iloop].errorId = 0;
    }
    Det_TransientRamlogIndex = 0;
#endif /* DET_TRANSIENT_MAX_RAMLOG_SIZE > 0 */
#endif /* DET_SIZE_OF_TRANSIENT_FAULT_CALLOUT_TABLE > 0 */
#if (DET_SIZE_OF_GLOBAL_FILTER > 0)
    for (uint32 iloop = 0U; iloop < (uint32)DET_SIZE_OF_GLOBAL_FILTER; iloop++)
    {
        detGlobalFilter[iloop].mModuleId = 0U;
        detGlobalFilter[iloop].mInstanceId = 0U;
        detGlobalFilter[iloop].mApiId = 0U;
        detGlobalFilter[iloop].mErrorId = 0U;
    }
#endif /* DET_GLOBALFILTERSIZE > 0 */
#if (DET_SIZE_OF_BREAK_FILTER > 0)
    for (uint32 iloop = 0U; iloop < (uint32)DET_SIZE_OF_BREAK_FILTER; iloop++)
    {
        detBreakFilter[iloop].mModuleId = 0U;
        detBreakFilter[iloop].mInstanceId = 0U;
        detBreakFilter[iloop].mApiId = 0U;
        detBreakFilter[iloop].mErrorId = 0U;
    }
#endif /* DET_BREAKFILTERSIZE > 0 */
#if (DET_SIZE_OF_DLT_FILTER > 0)
    for (uint32 iloop = 0U; iloop < (uint32)DET_SIZE_OF_DLT_FILTER; iloop++)
    {
        detDltFilter[iloop].mModuleId = 0U;
        detDltFilter[iloop].mInstanceId = 0U;
        detDltFilter[iloop].mApiId = 0U;
        detDltFilter[iloop].mErrorId = 0U;
    }
#endif /* DET_DLTFILTERSIZE > 0 */
#if (DET_SIZE_OF_LOG_BUFFER > 0)
    for (uint32 iloop = 0U; iloop < (uint32)DET_SIZE_OF_LOG_BUFFER; iloop++)
    {
        detLogBuffer[iloop].mModuleId = 0U;
        detLogBuffer[iloop].mInstanceId = 0U;
        detLogBuffer[iloop].mApiId = 0U;
        detLogBuffer[iloop].mErrorId = 0U;
    }
#endif /* DET_LOGBUFFERSIZE > 0 */
#endif /* STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT */
    detStatus.globalFilterActive = (boolean)FALSE;
    detStatus.logActive = (boolean)FALSE;
    detStatus.logIndex = 0U;
    detStatus.breakOnLogOverrun = (boolean)FALSE;
    detStatus.breakFilterActive = (boolean)FALSE;
    /* reset recursion limit counter */
    detRecursionCounterReportError = 0U;
    /* reset recursion limit counter */
    detRecursionCounterReportRuntimeError = 0U;
    /* reset recursion limit counter */
    detRecursionCounterReportTransientFault = 0U;
    detModuleInit = (boolean)TRUE;
#if (STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT)
    Det_RunState = DET_INITIALIZED;
#endif /* STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT */
}

/******************************************************************************/
/*
 * Brief               Starts the DET
 * details             Global function as an interface from EcuM to DET for start
 * note                This API has no functionality in this DET implementation. Therefore its call is optional.
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None:
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              None:
 */
/******************************************************************************/
FUNC(void, DET_CODE) Det_Start(void)
{
#if (STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT)
    Det_RunState = DET_STARTED;
#endif /* STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT */
}

/******************************************************************************/
/*
 * Brief               Initializes the DET
 * details             Global function as an interface from EcuM to DET for initialization
 * note                Shall be called during startup by EcuM in a safe context.
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None:
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              None:
 */
/******************************************************************************/
FUNC(void, DET_CODE) Det_InitMemory(void)
{
    detModuleInit = (boolean)FALSE;
}

/******************************************************************************/
/*
 * Brief               Development error reporting function
 * details             Global function for BSW-Modules to report errors to the DET
 * note                If filtering is active the default settings of the filter discard error reports of ModuleId 0
 (zero).
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ModuleId: Module ID of calling module
                       InstanceId: The identifier of the index based instance of a module
                       ApiId: ID of API service in which error is detected
                       ErrorId: ID of detected development error
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              Std_ReturnType: E_OK always
 */
/******************************************************************************/
FUNC(Std_ReturnType, DET_CODE)
Det_ReportError(
    /* PRQA S 3206++ */ /* MISRA Rule 2.7*/
    VAR(uint16, AUTOMATIC) ModuleId,
    VAR(uint8, AUTOMATIC) InstanceId,
    VAR(uint8, AUTOMATIC) ApiId,
    /* PRQA S 3206-- */ /* MISRA Rule 2.7 */
    VAR(uint8, AUTOMATIC) ErrorId)
{
#if ((STD_ON == DET_ERROR_HOOK_ENABLED) && (DET_SIZE_OF_ERROR_HOOK_TABLE > 0))
    VAR(Std_ReturnType, AUTOMATIC) calloutReturn = (Std_ReturnType)E_OK;
#endif
#if (STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT)
    VAR(boolean, AUTOMATIC) forceBreakHandler;
    VAR(DetInfoType, AUTOMATIC) detInfo;
    detInfo.mModuleId = ModuleId;
    detInfo.mInstanceId = InstanceId;
    detInfo.mApiId = ApiId;
    detInfo.mErrorId = ErrorId;
#else
    /* PRQA S 3472,3469++ */ /* MISRA Dir 4.9 */
    DET_UNUSED(ModuleId);
    DET_UNUSED(InstanceId);
    DET_UNUSED(ApiId);
    DET_UNUSED(ErrorId);
    /* PRQA S 3472,3469-- */ /* MISRA Dir 4.9 */
#endif /* STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT */
    /* if DET is initialized: in case of call to uninitialized DET return */
    if ((boolean)TRUE == detModuleInit)
    {
        /* increment nesting counter for recursion detection */
        detRecursionCounterReportError++;
        if (detRecursionCounterReportError > DET_REPORT_ERROR_RECURSIONLIMIT)
        {
            Det_EndlessLoop();
        }
        else
        {
            /* call error hooks (if configured) */
#if ((STD_ON == DET_ERROR_HOOK_ENABLED) && (DET_SIZE_OF_ERROR_HOOK_TABLE > 0))
            for (uint32 iloop = 0; iloop < DET_SIZE_OF_ERROR_HOOK_TABLE; iloop++)
            {
                if (NULL_PTR != Det_GetErrorHookTable[iloop])
                {
                    calloutReturn = Det_GetErrorHookTable[iloop](ModuleId, InstanceId, ApiId, ErrorId);
                }
            }
#if (STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT)
            /* No action is taken if the module is not started */
            if (DET_STARTED == Det_RunState)
            {
                for (uint32 iloop = 0; iloop < DET_SIZE_OF_ERROR_HOOK_TABLE; iloop++)
                {
                    if (NULL_PTR != Det_CallbackList[iloop])
                    {
                        (*Det_CallbackList[iloop])(ModuleId, InstanceId, ApiId, ErrorId);
                    }
                }
#if (DET_MAX_RAMLOG_SIZE > 0)
                SchM_Enter_Det_LogError();
                if (Det_RamlogIndex < DET_MAX_RAMLOG_SIZE)
                {
                    Det_RamLog[Det_RamlogIndex].moduleId = ModuleId;
                    Det_RamLog[Det_RamlogIndex].instanceId = InstanceId;
                    Det_RamLog[Det_RamlogIndex].apiId = ApiId;
                    Det_RamLog[Det_RamlogIndex].errorId = ErrorId;
                    Det_RamlogIndex++;
                    if (Det_RamlogIndex == DET_MAX_RAMLOG_SIZE)
                    {
                        Det_RamlogIndex = 0;
                    }
                }
                SchM_Exit_Det_LogError();
#endif /* DET_MAX_RAMLOG_SIZE > 0 */
            }
#endif /* STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT */
#endif /* STD_ON == DET_ERROR_HOOK_ENABLED */

#if (STD_ON == DET_FORWARD_TO_DLT)
            /* forward DET error to DLT module (if configured) unless filtered out by a DLT filter */
#if ((DET_DLTFILTERSIZE > 0) && (STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT))
            if ((boolean)FALSE
                == Det_CheckFilterMatch(detDltFilter, (uint8)DET_DLTFILTERSIZE, detStatus.dltFilterActive, &detInfo))
#endif /* DET_DLTFILTERSIZE > 0) && (STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT */
            {
                Dlt_DetForwardErrorTrace(ModuleId, InstanceId, ApiId, ErrorId);
            }
#endif      /* STD_ON == DET_FORWARD_TO_DLT */
            /* if last error hook (if configured) returned E_OK: */
#if ((STD_ON == DET_ERROR_HOOK_ENABLED) && (DET_SIZE_OF_ERROR_HOOK_TABLE > 0))
            if ((Std_ReturnType)E_OK == calloutReturn)
#endif /* STD_ON == DET_ERROR_HOOK_ENABLED) && (DET_SIZE_OF_ERROR_HOOK_TABLE > 0 */
            {
                /* check for global filter match (if configured), in case of no match continue */
#if (STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT)
                if ((boolean)FALSE
                    == Det_CheckFilterMatch(
                        detGlobalFilter,
                        (uint8)DET_SIZE_OF_GLOBAL_FILTER,
                        detStatus.globalFilterActive,
                        &detInfo))
#endif /* DET_GLOBALFILTERSIZE > 0 && STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT */
                {
#if (STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT)
                    /* store DET error in log buffer (if configured) */
                    forceBreakHandler = Det_LogError(detStatus.logActive, &detInfo);
#endif              /* DET_LOGBUFFERSIZE > 0 && STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT */
                    /* check for break filter match or forced break handler call (if configured), in case of no
                     * match continue */
#if (STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT)
                    if ((Det_CheckFilterMatch(
                             detBreakFilter,
                             DET_SIZE_OF_BREAK_FILTER,
                             detStatus.breakFilterActive,
                             &detInfo)
                         == (boolean)FALSE)
                        || (forceBreakHandler == (boolean)TRUE))
#endif /* DET_LOGBUFFERSIZE > 0 && STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT */
                    {
                        /* enter endless loop - in variant with debug support loop can be left by setting a flag */
                        Det_EndlessLoop();
                    }
                }
            }
        }
        /* decrement nesting counter for recursion detection */
        detRecursionCounterReportError--;
    }
    return (Std_ReturnType)E_OK;
}

/******************************************************************************/
/*
 * Brief               Runtime error reporting function
 * details             Global function for BSW-Modules to report errors to the DET
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ModuleId: Module ID of calling module
                       InstanceId: The identifier of the index based instance of a module
                       ApiId: ID of API service in which error is detected
                       ErrorId: ID of detected development error
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              Std_ReturnType: E_OK always
 */
/******************************************************************************/
FUNC(Std_ReturnType, DET_CODE)
Det_ReportRuntimeError(
    /* PRQA S 3206++ */ /* MISRA Rule 2.7 */
    VAR(uint16, AUTOMATIC) ModuleId,
    VAR(uint8, AUTOMATIC) InstanceId,
    VAR(uint8, AUTOMATIC) ApiId,
    /* PRQA S 3206-- */ /* MISRA Rule 2.7 */
    VAR(uint8, AUTOMATIC) ErrorId)
{
#if ((STD_ON == DET_RUNTIME_ERROR_CALLOUT_ENABLED) && (DET_SIZE_OF_RUNTIME_ERROR_CALLOUT_TABLE > 0))
    VAR(Std_ReturnType, AUTOMATIC) calloutReturn = E_OK;
#endif
#if (STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT)
    VAR(DetInfoType, AUTOMATIC) detInfo;
    detInfo.mModuleId = ModuleId;
    detInfo.mInstanceId = InstanceId;
    detInfo.mApiId = ApiId;
    detInfo.mErrorId = ErrorId;
#else
    /* PRQA S 3472,3469++ */ /* MISRA Dir 4.9 */
    DET_UNUSED(ModuleId);
    DET_UNUSED(InstanceId);
    DET_UNUSED(ApiId);
    DET_UNUSED(ErrorId);
    /* PRQA S 3472,3469-- */ /* MISRA Dir 4.9 */
#endif /* STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT */
    /* if DET is initialized: in case of call to uninitialized DET return */
    if ((boolean)TRUE == detModuleInit)
    {
        /* increment nesting counter for recursion detection */
        detRecursionCounterReportRuntimeError++;
        if (detRecursionCounterReportRuntimeError > DET_REPORT_RUNTIMEERROR_RECURSIONLIMIT)
        {
            /* Det_EndlessLoop; */
        }
        else
        {
            /* call runtime error callouts (if configured) */
#if ((STD_ON == DET_RUNTIME_ERROR_CALLOUT_ENABLED) && (DET_SIZE_OF_RUNTIME_ERROR_CALLOUT_TABLE > 0))
            for (uint32 iloop = 0; iloop < DET_SIZE_OF_RUNTIME_ERROR_CALLOUT_TABLE; iloop++)
            {
                if (Det_GetRuntimeErrorCalloutTable[iloop] != NULL_PTR)
                {
                    calloutReturn = Det_GetRuntimeErrorCalloutTable[iloop](ModuleId, InstanceId, ApiId, ErrorId);
                }
            }
#if (STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT)
            /* No action is taken if the module is not started */
            if (DET_STARTED == Det_RunState)
            {
                for (uint32 iloop = 0; iloop < DET_SIZE_OF_RUNTIME_ERROR_CALLOUT_TABLE; iloop++)
                {
                    if (NULL_PTR != Det_RunTimeCallbackList[iloop])
                    {
                        (*Det_RunTimeCallbackList[iloop])(ModuleId, InstanceId, ApiId, ErrorId);
                    }
                }
#if (DET_RUNTIME_MAX_RAMLOG_SIZE > 0)
                SchM_Enter_Det_LogError();
                if (Det_RunTimeRamlogIndex < DET_RUNTIME_MAX_RAMLOG_SIZE)
                {
                    Det_RunTimeRamLog[Det_RunTimeRamlogIndex].moduleId = ModuleId;
                    Det_RunTimeRamLog[Det_RunTimeRamlogIndex].instanceId = InstanceId;
                    Det_RunTimeRamLog[Det_RunTimeRamlogIndex].apiId = ApiId;
                    Det_RunTimeRamLog[Det_RunTimeRamlogIndex].errorId = ErrorId;
                    Det_RunTimeRamlogIndex++;
                    if (Det_RunTimeRamlogIndex == DET_RUNTIME_MAX_RAMLOG_SIZE)
                    {
                        Det_RunTimeRamlogIndex = 0;
                    }
                }
                SchM_Exit_Det_LogError();
#endif /* DET_RUNTIME_MAX_RAMLOG_SIZE > 0 */
            }
#endif /* STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT */
#endif /* STD_ON == DET_RUNTIME_ERROR_CALLOUT_ENABLED */

#if (STD_ON == DET_FORWARD_TO_DLT)
            /* forward DET error to DLT module (if configured) unless filtered out by a DLT filter */
#if ((DET_DLTFILTERSIZE > 0) && (STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT))
            if (Det_CheckFilterMatch(detDltFilter, DET_DLTFILTERSIZE, detStatus.dltFilterActive, &detInfo)
                == (boolean)FALSE)
#endif /* DET_DLTFILTERSIZE > 0 && STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT */
            {
                Dlt_DetForwardErrorTrace(ModuleId, InstanceId, ApiId, ErrorId);
            }
#endif      /* STD_ON == DET_FORWARD_TO_DLT */
            /* if last runtime error callout (if configured) returned E_OK: */
#if ((STD_ON == DET_RUNTIME_ERROR_CALLOUT_ENABLED) && (DET_SIZE_OF_RUNTIME_ERROR_CALLOUT_TABLE > 0))
            if ((Std_ReturnType)E_OK == calloutReturn)
#endif /* STD_ON == DET_RUNTIME_ERROR_CALLOUT_ENABLED && DET_SIZE_OF_RUNTIME_ERROR_CALLOUT_TABLE > 0*/
            {
#if (STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT)
                /* store DET error in log buffer (if configured) */
                (void)Det_LogError(detStatus.logActive, &detInfo);
#endif /* DET_LOGBUFFERSIZE > 0 && STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT */
            }
        }
        /* decrement nesting counter for recursion detection */
        detRecursionCounterReportRuntimeError--;
    }
    return (Std_ReturnType)E_OK;
}

/******************************************************************************/
/*
 * Brief               Transient fault reporting function
 * details             Global function for BSW-Modules to report errors to the DET
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ModuleId: Module ID of calling module
                       InstanceId: The identifier of the index based instance of a module
                       ApiId: ID of API service in which error is detected
                       ErrorId: ID of detected development error
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              Std_ReturnType: E_OK (always) if no callouts are configured - return value of last callout if
 callouts are configured
 */
/******************************************************************************/
FUNC(Std_ReturnType, DET_CODE)
Det_ReportTransientFault(
    /* PRQA S 3206++ */ /* MISRA Rule 2.7 */
    VAR(uint16, AUTOMATIC) ModuleId,
    VAR(uint8, AUTOMATIC) InstanceId,
    VAR(uint8, AUTOMATIC) ApiId,
    /* PRQA S 3206-- */ /* MISRA Rule 2.7 */
    VAR(uint8, AUTOMATIC) FaultId)
{
#if ((STD_ON == DET_TRANSIENT_FAULT_CALLOUT_ENABLED) && (DET_SIZE_OF_TRANSIENT_FAULT_CALLOUT_TABLE > 0))
    VAR(Std_ReturnType, AUTOMATIC) calloutReturn = (Std_ReturnType)E_OK;
#endif
#if (STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT)
    VAR(DetInfoType, AUTOMATIC) detInfo;
    detInfo.mModuleId = ModuleId;
    detInfo.mInstanceId = InstanceId;
    detInfo.mApiId = ApiId;
    detInfo.mErrorId = FaultId;
#else
    /* PRQA S 3472,3469++ */ /* MISRA Dir 4.9 */
    DET_UNUSED(ModuleId);
    DET_UNUSED(InstanceId);
    DET_UNUSED(ApiId);
    DET_UNUSED(FaultId);
    /* PRQA S 3472,3469-- */ /* MISRA Dir 4.9 */
#endif /* STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT */
    /* if DET is initialized: in case of call to uninitialized DET return */
    if ((boolean)TRUE == detModuleInit)
    {
        /* increment nesting counter for recursion detection */
        detRecursionCounterReportTransientFault++;
        if (detRecursionCounterReportTransientFault > DET_REPORT_TRANSIENTFAULT_RECURSIONLIMIT)
        {
            /* Det_EndlessLoop */
        }
        else
        {
            /* call transient fault callouts (if configured) */
#if ((STD_ON == DET_TRANSIENT_FAULT_CALLOUT_ENABLED) && (DET_SIZE_OF_TRANSIENT_FAULT_CALLOUT_TABLE > 0))
            for (uint32 iloop = 0; iloop < DET_SIZE_OF_TRANSIENT_FAULT_CALLOUT_TABLE; iloop++)
            {
                if (Det_GetTransientFaultCalloutTable[iloop] != NULL_PTR)
                {
                    calloutReturn = Det_GetTransientFaultCalloutTable[iloop](ModuleId, InstanceId, ApiId, FaultId);
                }
            }
#if (STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT)
            /* No action is taken if the module is not started */
            if (Det_RunState == DET_STARTED)
            {
                for (uint32 iloop = 0; iloop < DET_SIZE_OF_TRANSIENT_FAULT_CALLOUT_TABLE; iloop++)
                {
                    if (NULL_PTR != Det_TransientCallbackList[iloop])
                    {
                        (*Det_TransientCallbackList[iloop])(ModuleId, InstanceId, ApiId, FaultId);
                    }
                }
#if (DET_TRANSIENT_MAX_RAMLOG_SIZE > 0)
                SchM_Enter_Det_LogError();
                if (Det_TransientRamlogIndex < DET_TRANSIENT_MAX_RAMLOG_SIZE)
                {
                    Det_TransientRamLog[Det_TransientRamlogIndex].moduleId = ModuleId;
                    Det_TransientRamLog[Det_TransientRamlogIndex].instanceId = InstanceId;
                    Det_TransientRamLog[Det_TransientRamlogIndex].apiId = ApiId;
                    Det_TransientRamLog[Det_TransientRamlogIndex].errorId = FaultId;
                    Det_TransientRamlogIndex++;
                    if (Det_TransientRamlogIndex == DET_TRANSIENT_MAX_RAMLOG_SIZE)
                    {
                        Det_TransientRamlogIndex = 0;
                    }
                }
                SchM_Exit_Det_LogError();
#endif /* DET_TRANSIENT_MAX_RAMLOG_SIZE > 0 */
            }
#endif /* STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT*/
#endif /* STD_ON == DET_TRANSIENT_FAULT_CALLOUT_ENABLED */

#if (DET_FORWARD_TO_DLT == STD_ON)
            /* forward DET error to DLT module (if configured) unless filtered out by a DLT filter */
#if ((DET_DLTFILTERSIZE > 0) && (STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT))
            if (Det_CheckFilterMatch(detDltFilter, DET_DLTFILTERSIZE, detStatus.dltFilterActive, &detInfo)
                == (boolean)FALSE)
#endif /* DET_DLTFILTERSIZE > 0) && (STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT */
            {
                Dlt_DetForwardErrorTrace(ModuleId, InstanceId, ApiId, FaultId);
            }
#endif /* STD_ON == DET_FORWARD_TO_DLT */
#if ((STD_ON == DET_TRANSIENT_FAULT_CALLOUT_ENABLED) && (DET_SIZE_OF_TRANSIENT_FAULT_CALLOUT_TABLE > 0))
            if ((Std_ReturnType)E_OK == calloutReturn)
#endif
            {
#if (STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT)
                /* store DET error in log buffer (if configured) */
                (void)Det_LogError(detStatus.logActive, &detInfo);
#endif /* DET_LOGBUFFERSIZE > 0) && (STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT */
            }
        }
        /* decrement nesting counter for recursion detection */
        detRecursionCounterReportTransientFault--;
    }
    return (Std_ReturnType)E_OK;
}

#if (STD_ON == DET_VERSION_INFO_API)
/******************************************************************************/
/*
 * Brief               Read the DET version information
 * details             Global function for BSW-Modules to get the Version of DET
 * config              DET_VERSION_INFO_API
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None:
 * Param-Name[out]     versioninfo: Version information of the DET
 * Param-Name[in/out]  None:
 * Return              None:
 */
/******************************************************************************/
FUNC(void, DET_CODE) Det_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, DET_APPL_DATA) versioninfo)
{
    /* if versioninfo is no NULL pointer */
    if (NULL_PTR != versioninfo)
    {
        /* fill versions from defines into versioninfo structure */
        versioninfo->vendorID = DET_VENDOR_ID;
        versioninfo->moduleID = DET_MODULE_ID;
        versioninfo->sw_major_version = DET_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = DET_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = DET_SW_PATCH_VERSION;
    }
    else
    {
        (void)Det_ReportError(
            (uint16)DET_MODULE_ID,
            (uint8)DET_INSTANCE_ID,
            (uint8)DET_SID_GETVERSIONINFO,
            (uint8)DET_E_PARAM_POINTER);
    }
}
#endif

#if (STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT)

#if (DET_SIZE_OF_ERROR_HOOK_TABLE > 0)
/* Add a callback function to the array of callback. After a call to Det_ReportError the callback is called.
 * This can be used in for instance unit tests to verify that correct errors are reported when sending invalid
 * parameters to a function. This function returns the index of the callback in the array when registration is
 * successful. If not DET_CBK_REGISTRATION_FAILED_INDEX is returned. The index can be used to remove a callback with the
 * Det_RemoveCbk.*/
FUNC(uint8, DET_CODE) Det_AddCallback(Det_CallbackFncType detCbk)
{
    uint8 iloop;
    uint8 retVal = DET_CBK_REGISTRATION_FAILED_INDEX;
    if (DET_UNINITIALIZED != Det_RunState)
    {
        for (iloop = 0; iloop < DET_SIZE_OF_ERROR_HOOK_TABLE; iloop++)
        {
            if (NULL_PTR == Det_CallbackList[iloop])
            {
                Det_CallbackList[iloop] = detCbk;
                retVal = iloop;
                break;
            }
        }
    }
    if (DET_CBK_REGISTRATION_FAILED_INDEX == retVal)
    {
        Det_ReportError(DET_MODULE_ID, 0, DET_CALLBACK_API, DET_E_CBK_REGISTRATION_FAILED);
    }
    return retVal;
}

FUNC(void, DET_CODE) Det_RemoveCallback(uint8 detCbkIndex)
{
    if (detCbkIndex >= DET_SIZE_OF_ERROR_HOOK_TABLE)
    {
        Det_ReportError(DET_MODULE_ID, 0, DET_CALLBACK_API, DET_E_INDEX_OUT_OF_RANGE);
    }
    else
    {
        Det_CallbackList[detCbkIndex] = NULL_PTR;
    }
}
#endif /* DET_SIZE_OF_ERROR_HOOK_TABLE > 0 */

#if (DET_SIZE_OF_RUNTIME_ERROR_CALLOUT_TABLE > 0)
/* Add a callback function to the array of callback. After a call to Det_ReportRunTimeError the callback is called.
 * This can be used in for instance unit tests to verify that correct errors are reported when sending invalid
 * parameters to a function. This function returns the index of the callback in the array when registration is
 * successful. If not DET_CBK_REGISTRATION_FAILED_INDEX is returned. The index can be used to remove a callback with the
 * Det_RemoveRunTimeCbk.*/
FUNC(uint8, DET_CODE) Det_AddRunTimeCallback(Det_CallbackFncType detCbk)
{
    uint8 iloop;
    uint8 retVal = DET_CBK_REGISTRATION_FAILED_INDEX;
    if (Det_RunState != DET_UNINITIALIZED)
    {
        for (iloop = 0; iloop < DET_SIZE_OF_RUNTIME_ERROR_CALLOUT_TABLE; iloop++)
        {
            if (NULL_PTR == Det_RunTimeCallbackList[iloop])
            {
                Det_RunTimeCallbackList[iloop] = detCbk;
                retVal = iloop;
                break;
            }
        }
    }
    if (retVal == DET_CBK_REGISTRATION_FAILED_INDEX)
    {
        Det_ReportError(DET_MODULE_ID, 0, DET_CALLBACK_API, DET_E_CBK_REGISTRATION_FAILED);
    }
    return retVal;
}

FUNC(void, DET_CODE) Det_RemoveRunTimeCallback(uint8 detCbkIndex)
{
    if (detCbkIndex >= DET_SIZE_OF_RUNTIME_ERROR_CALLOUT_TABLE)
    {
        Det_ReportError(DET_MODULE_ID, 0, DET_CALLBACK_API, DET_E_INDEX_OUT_OF_RANGE);
    }
    else
    {
        Det_RunTimeCallbackList[detCbkIndex] = NULL_PTR;
    }
}
#endif /* DET_SIZE_OF_RUNTIME_ERROR_CALLOUT_TABLE > 0 */

#if (DET_SIZE_OF_TRANSIENT_FAULT_CALLOUT_TABLE > 0)
/* Add a callback function to the array of callback. After a call to Det_ReportTransientFault the callback is called.
 * This can be used in for instance unit tests to verify that correct errors are reported when sending invalid
 * parameters to a function. This function returns the index of the callback in the array when registration is
 * successful. If not DET_CBK_REGISTRATION_FAILED_INDEX is returned. The index can be used to remove a callback with the
 * Det_RemoveTransientCbk.*/
FUNC(uint8, DET_CODE) Det_AddTransientCallback(Det_CallbackFncType detCbk)
{
    uint8 iloop;
    uint8 retVal = DET_CBK_REGISTRATION_FAILED_INDEX;
    if (Det_RunState != DET_UNINITIALIZED)
    {
        for (iloop = 0; iloop < DET_SIZE_OF_TRANSIENT_FAULT_CALLOUT_TABLE; iloop++)
        {
            if (NULL_PTR == Det_TransientCallbackList[iloop])
            {
                Det_TransientCallbackList[iloop] = detCbk;
                retVal = iloop;
                break;
            }
        }
    }
    if (retVal == DET_CBK_REGISTRATION_FAILED_INDEX)
    {
        Det_ReportError(DET_MODULE_ID, 0, DET_CALLBACK_API, DET_E_CBK_REGISTRATION_FAILED);
    }
    return retVal;
}

FUNC(void, DET_CODE) Det_RemoveTransientCallback(uint8 detCbkIndex)
{
    if (detCbkIndex >= DET_SIZE_OF_TRANSIENT_FAULT_CALLOUT_TABLE)
    {
        Det_ReportError(DET_MODULE_ID, 0, DET_CALLBACK_API, DET_E_INDEX_OUT_OF_RANGE);
    }
    else
    {
        Det_TransientCallbackList[detCbkIndex] = NULL_PTR;
    }
}
#endif /* DET_SIZE_OF_TRANSIENT_FAULT_CALLOUT_TABLE > 0 */

#endif /* STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT */

#define DET_STOP_SEC_CODE
#include "Det_MemMap.h"
