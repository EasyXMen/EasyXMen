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
 **  @file               : Det.c
 **  @author             : haibin.shao
 **  @date               : 2024/09/25
 **  @vendor             : isoft
 **  @description        : Default Error Tracer
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Det.h"
#include "Det_Externals.h"
#include "SchM_Det.h"
#include "Det_Arti.h"

#if (DET_FORWARD_TO_DLT == STD_ON)
#include "Dlt.h"
#endif

/* ===================================================== macros ===================================================== */
/* PRQA S 3414 ++ */ /* VL_Det_3414 */
#ifndef DET_LOCAL
#define DET_LOCAL static
#endif
/* PRQA S 3414 -- */

/* ================================================ type definitions ================================================ */
/* The type is used to describe the running state in the DET module */
typedef enum
{
    DET_UNINITIALIZED = 0,
    DET_INITIALIZED   = 1,
    DET_STARTED       = 2
} Det_StateType;

/* ========================================== internal function declarations ======================================== */
#if (DET_SIZE_OF_NON_BSW_MODULE_CFG > 0)
#define DET_START_SEC_CODE
#include "Det_MemMap.h"
/* This API is used to check the matching result of parameter filtering */
DET_LOCAL boolean Det_CheckFilterMatch(uint16 moduleId, uint8 instanceId);
#define DET_STOP_SEC_CODE
#include "Det_MemMap.h"
#endif

/* ============================================ internal data definitions =========================================== */
#define DET_START_SEC_VAR_CLEARED_8
#include "Det_MemMap.h"
DET_LOCAL Det_StateType Det_RunState;
#define DET_STOP_SEC_VAR_CLEARED_8
#include "Det_MemMap.h"

#if (DET_SIZE_OF_NON_BSW_MODULE_CFG > 0)
#define DET_START_SEC_VAR_CLEARED_PTR
#include "Det_MemMap.h"
DET_LOCAL const Det_ConfigType* Det_ConfigPtr;
#define DET_STOP_SEC_VAR_CLEARED_PTR
#include "Det_MemMap.h"
#endif

/* ============================================ external data definitions =========================================== */
#if (DET_REPORT_ERROR_RECURSIONLIMIT > 0u)
#define DET_START_SEC_VAR_CLEARED_8
#include "Det_MemMap.h"
/* recursion limit counter for develop error */
uint8 Det_RecursionCounterReportError;
#define DET_STOP_SEC_VAR_CLEARED_8
#include "Det_MemMap.h"
#endif
#if (DET_REPORT_RUNTIMEERROR_RECURSIONLIMIT > 0u)
#define DET_START_SEC_VAR_CLEARED_8
#include "Det_MemMap.h"
/* recursion limit counter for runtime error */
uint8 Det_RecursionCounterReportRuntimeError;
#define DET_STOP_SEC_VAR_CLEARED_8
#include "Det_MemMap.h"
#endif
#if (DET_REPORT_TRANSIENTFAULT_RECURSIONLIMIT > 0u)
#define DET_START_SEC_VAR_CLEARED_8
#include "Det_MemMap.h"
/* recursion limit counter for transient fault */
uint8 Det_RecursionCounterReportTransientFault;
#define DET_STOP_SEC_VAR_CLEARED_8
#include "Det_MemMap.h"
#endif

/* ========================================== external function definitions ========================================= */
#define DET_START_SEC_CODE
#include "Det_MemMap.h"
/* This API is called by the EcuM module to initialize the Det module */
void Det_Init(const Det_ConfigType* configPtr)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Det, 0, Bsw_Start, 2, 0x00, configPtr);
    /* Checks if the Det module is not initialized and configPtr is not null pointer */
    if ((Det_RunState == DET_UNINITIALIZED) && (NULL_PTR != configPtr))
    {
#if (DET_SIZE_OF_NON_BSW_MODULE_CFG > 0)
        /* Copy global Configuration item to store the configuration pointer. */
        Det_ConfigPtr = configPtr;
#else
        DET_PARAM_UNUSED(configPtr);
#endif
        /* Set the status of the DET module to initialized */
        Det_RunState = DET_INITIALIZED;
        /* Set first value of recursion limit counter */
#if (DET_REPORT_ERROR_RECURSIONLIMIT > 0u)
        Det_RecursionCounterReportError = 0u;
#endif
#if (DET_REPORT_RUNTIMEERROR_RECURSIONLIMIT > 0u)
        Det_RecursionCounterReportRuntimeError = 0u;
#endif
#if (DET_REPORT_TRANSIENTFAULT_RECURSIONLIMIT > 0u)
        Det_RecursionCounterReportTransientFault = 0u;
#endif
    }
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Det, 0, Bsw_Return, 1, 0x00);
}
#define DET_STOP_SEC_CODE
#include "Det_MemMap.h"
#if (STD_ON == DET_VERSION_INFO_API)
#define DET_START_SEC_CODE
#include "Det_MemMap.h"
/* This API is used to get software version information from the DET module */
void Det_GetVersionInfo(Std_VersionInfoType* versionInfoPtr)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Det, 0, Bsw_Start, 2, 0x03, versionInfoPtr);
    /* if versioninfo is no NULL pointer */
    if (NULL_PTR != versionInfoPtr)
    {
        /* fill versions from defines into versioninfo structure */
        versionInfoPtr->vendorID         = DET_VENDOR_ID;
        versionInfoPtr->moduleID         = DET_MODULE_ID;
        versionInfoPtr->sw_major_version = DET_SW_MAJOR_VERSION;
        versionInfoPtr->sw_minor_version = DET_SW_MINOR_VERSION;
        versionInfoPtr->sw_patch_version = DET_SW_PATCH_VERSION;
    }
    else
    {
        (void)Det_ReportError(
            (uint16)DET_MODULE_ID,
            (uint8)DET_INSTANCE_ID,
            (uint8)DET_SID_GETVERSIONINFO,
            (uint8)DET_E_PARAM_POINTER);
    }
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Det, 0, Bsw_Return, 1, 0x03);
}
#define DET_STOP_SEC_CODE
#include "Det_MemMap.h"
#endif /* STD_ON == DET_VERSION_INFO_API */
#define DET_START_SEC_CODE
#include "Det_MemMap.h"
/* This API is used to start the DET module for error reporting */
void Det_Start(void)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Det, 0, Bsw_Start, 1, 0x02);
    /* Checks if the Det module is initialized */
    if (Det_RunState == DET_INITIALIZED)
    {
        /* Set the status of the DET module to started */
        Det_RunState = DET_STARTED;
    }
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Det, 0, Bsw_Return, 1, 0x02);
}
#define DET_STOP_SEC_CODE
#include "Det_MemMap.h"
#define DET_START_SEC_CODE
#include "Det_MemMap.h"
/* This API is used to report development error for DET modules */
/* PRQA S 6030 ++ */ /* VL_MTR_Det_STMIF */
Std_ReturnType Det_ReportError(uint16 moduleId, uint8 instanceId, uint8 apiId, uint8 errorId)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Det, 0, Bsw_Start, 5, 0x01, moduleId, instanceId, apiId, errorId);
    Std_ReturnType retVal = E_OK;
    /* Check if the running status of the DET module is Started */
    if (Det_RunState == DET_STARTED)
    {
#if (DET_REPORT_ERROR_RECURSIONLIMIT > 0u)
        SchM_Enter_Det_DET_EXCLUSIVE_AREA_1();
        ++Det_RecursionCounterReportError;
        SchM_Exit_Det_DET_EXCLUSIVE_AREA_1();
        /* increment nesting counter for recursion detection */
        if (Det_RecursionCounterReportError < DET_REPORT_ERROR_RECURSIONLIMIT)
#endif
        {
#if (DET_SIZE_OF_NON_BSW_MODULE_CFG > 0)
            /* Check if the parameter filter of the DET module is matched successfully */
            if (TRUE == Det_CheckFilterMatch(moduleId, instanceId))
#endif
            {
#if (DET_SIZE_OF_ERROR_HOOK_TABLE > 0U)
                /* call error hooks (if configured) */
                for (uint32 iloop = 0u; iloop < DET_SIZE_OF_ERROR_HOOK_TABLE; ++iloop)
                {
                    if (NULL_PTR != Det_GetErrorHookTable[iloop])
                    {
                        retVal |= Det_GetErrorHookTable[iloop](moduleId, instanceId, apiId, errorId);
                    }
                }
#endif /* DET_SIZE_OF_ERROR_HOOK_TABLE > 0U */
#if (STD_ON == DET_FORWARD_TO_DLT)
                /* forward DET error to DLT module (if configured) unless filtered out by a DLT filter */
                Dlt_DetForwardErrorTrace(moduleId, instanceId, apiId, errorId);
#endif /* STD_ON == DET_FORWARD_TO_DLT*/
#if (DET_SIZE_OF_ERROR_HOOK_TABLE > 0U)
                if (E_OK == retVal)
#endif /* DET_SIZE_OF_ERROR_HOOK_TABLE > 0U */
                {
                    /* Check if logging record are activated */
                    /* Enter infinite loop */
                    Det_EndlessLoop();
                }
            }
        }
#if (DET_REPORT_ERROR_RECURSIONLIMIT > 0u)
        else
        {
            /* Enter infinite loop */
            Det_EndlessLoop();
        }
        SchM_Enter_Det_DET_EXCLUSIVE_AREA_1();
        --Det_RecursionCounterReportError;
        SchM_Exit_Det_DET_EXCLUSIVE_AREA_1();
#endif
#if ((DET_SIZE_OF_NON_BSW_MODULE_CFG == 0) && (DET_SIZE_OF_ERROR_HOOK_TABLE == 0U) && (STD_OFF == DET_FORWARD_TO_DLT))
        DET_PARAM_UNUSED(moduleId);
        DET_PARAM_UNUSED(instanceId);
#endif
#if ((DET_SIZE_OF_ERROR_HOOK_TABLE == 0U) && (STD_OFF == DET_FORWARD_TO_DLT))
        DET_PARAM_UNUSED(apiId);
        DET_PARAM_UNUSED(errorId);
#endif
    }
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Det, 0, Bsw_Return, 2, 0x01, retVal);
    return retVal;
}
#define DET_STOP_SEC_CODE
#include "Det_MemMap.h"
#define DET_START_SEC_CODE
#include "Det_MemMap.h"
/* This API is used to report runtime error for DET modules */
Std_ReturnType Det_ReportRuntimeError(uint16 moduleId, uint8 instanceId, uint8 apiId, uint8 errorId)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Det, 0, Bsw_Start, 5, 0x04, moduleId, instanceId, apiId, errorId);
    Std_ReturnType retVal = E_OK;
    /* Check if the running status of the DET module is Started */
    if (Det_RunState == DET_STARTED)
    {
#if (DET_REPORT_RUNTIMEERROR_RECURSIONLIMIT > 0u)
        SchM_Enter_Det_DET_EXCLUSIVE_AREA_1();
        ++Det_RecursionCounterReportRuntimeError;
        SchM_Exit_Det_DET_EXCLUSIVE_AREA_1();
        /* increment nesting counter for recursion detection */
        if (Det_RecursionCounterReportRuntimeError < DET_REPORT_RUNTIMEERROR_RECURSIONLIMIT)
#endif
        {
#if (DET_SIZE_OF_NON_BSW_MODULE_CFG > 0)
            /* Check if the parameter filter of the DET module is matched successfully */
            if (TRUE == Det_CheckFilterMatch(moduleId, instanceId))
#endif
            {
#if (DET_SIZE_OF_RUNTIME_ERROR_CALLOUT_TABLE > 0U)
                /* call runtime error callouts (if configured) */
                for (uint32 iloop = 0u; iloop < DET_SIZE_OF_RUNTIME_ERROR_CALLOUT_TABLE; ++iloop)
                {
                    if (Det_GetRuntimeErrorCalloutTable[iloop] != NULL_PTR)
                    {
                        retVal |= Det_GetRuntimeErrorCalloutTable[iloop](moduleId, instanceId, apiId, errorId);
                    }
                }
#endif /* DET_SIZE_OF_RUNTIME_ERROR_CALLOUT_TABLE > 0U */
#if (STD_ON == DET_FORWARD_TO_DLT)
                /* forward DET error to DLT module (if configured) unless filtered out by a DLT filter */
                Dlt_DetForwardErrorTrace(moduleId, instanceId, apiId, errorId);
#endif /* STD_ON == DET_FORWARD_TO_DLT */
            }
        }
#if (DET_REPORT_RUNTIMEERROR_RECURSIONLIMIT > 0u)
        else
        {
            /* Enter infinite loop */
            Det_EndlessLoop();
        }
        SchM_Enter_Det_DET_EXCLUSIVE_AREA_1();
        --Det_RecursionCounterReportRuntimeError;
        SchM_Exit_Det_DET_EXCLUSIVE_AREA_1();
#endif
#if (                                                                                        \
    (DET_SIZE_OF_NON_BSW_MODULE_CFG == 0) && (DET_SIZE_OF_RUNTIME_ERROR_CALLOUT_TABLE == 0U) \
    && (STD_OFF == DET_FORWARD_TO_DLT))
        DET_PARAM_UNUSED(moduleId);
        DET_PARAM_UNUSED(instanceId);
#endif
#if ((DET_SIZE_OF_RUNTIME_ERROR_CALLOUT_TABLE == 0U) && (STD_OFF == DET_FORWARD_TO_DLT))
        DET_PARAM_UNUSED(apiId);
        DET_PARAM_UNUSED(errorId);
#endif
    }
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Det, 0, Bsw_Return, 2, 0x04, retVal);
    return retVal;
}
#define DET_STOP_SEC_CODE
#include "Det_MemMap.h"
#define DET_START_SEC_CODE
#include "Det_MemMap.h"
/* This API is used to report transient fault for DET modules */
Std_ReturnType Det_ReportTransientFault(uint16 moduleId, uint8 instanceId, uint8 apiId, uint8 faultId)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Det, 0, Bsw_Start, 5, 0x05, moduleId, instanceId, apiId, faultId);
    Std_ReturnType retVal = E_OK;
    /* Check if the running status of the DET module is Started */
    if (Det_RunState == DET_STARTED)
    {
#if (DET_REPORT_TRANSIENTFAULT_RECURSIONLIMIT > 0u)
        SchM_Enter_Det_DET_EXCLUSIVE_AREA_1();
        ++Det_RecursionCounterReportTransientFault;
        SchM_Exit_Det_DET_EXCLUSIVE_AREA_1();
        /* increment nesting counter for recursion detection */
        if (Det_RecursionCounterReportTransientFault < DET_REPORT_TRANSIENTFAULT_RECURSIONLIMIT)
#endif
        {
#if (DET_SIZE_OF_NON_BSW_MODULE_CFG > 0)
            /* Check if the parameter filter of the DET module is matched successfully */
            if (TRUE == Det_CheckFilterMatch(moduleId, instanceId))
#endif
            {
#if (DET_SIZE_OF_TRANSIENT_FAULT_CALLOUT_TABLE > 0U)
                /* call transient fault callouts (if configured) */
                for (uint32 iloop = 0u; iloop < DET_SIZE_OF_TRANSIENT_FAULT_CALLOUT_TABLE; ++iloop)
                {
                    if (Det_GetTransientFaultCalloutTable[iloop] != NULL_PTR)
                    {
                        retVal |= Det_GetTransientFaultCalloutTable[iloop](moduleId, instanceId, apiId, faultId);
                    }
                }
#endif /* DET_SIZE_OF_TRANSIENT_FAULT_CALLOUT_TABLE > 0U */
#if (STD_ON == DET_FORWARD_TO_DLT)
                /* forward DET error to DLT module (if configured) unless filtered out by a DLT filter */
                Dlt_DetForwardErrorTrace(moduleId, instanceId, apiId, faultId);
#endif /* STD_ON == DET_FORWARD_TO_DLT */
            }
        }
#if (DET_REPORT_TRANSIENTFAULT_RECURSIONLIMIT > 0u)
        else
        {
            /* Enter infinite loop */
            Det_EndlessLoop();
        }
        SchM_Enter_Det_DET_EXCLUSIVE_AREA_1();
        --Det_RecursionCounterReportTransientFault;
        SchM_Exit_Det_DET_EXCLUSIVE_AREA_1();
#endif
#if (                                                                                          \
    (DET_SIZE_OF_NON_BSW_MODULE_CFG == 0) && (DET_SIZE_OF_TRANSIENT_FAULT_CALLOUT_TABLE == 0U) \
    && (STD_OFF == DET_FORWARD_TO_DLT))
        DET_PARAM_UNUSED(moduleId);
        DET_PARAM_UNUSED(instanceId);
#endif
#if ((DET_SIZE_OF_TRANSIENT_FAULT_CALLOUT_TABLE == 0U) && (STD_OFF == DET_FORWARD_TO_DLT))
        DET_PARAM_UNUSED(apiId);
        DET_PARAM_UNUSED(faultId);
#endif
    }
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Det, 0, Bsw_Return, 2, 0x05, retVal);
    return retVal;
}
#define DET_STOP_SEC_CODE
#include "Det_MemMap.h"

/* ========================================== internal function definitions ========================================= */
#if (DET_SIZE_OF_NON_BSW_MODULE_CFG > 0)
#define DET_START_SEC_CODE
#include "Det_MemMap.h"
/* This API is used to check the matching result of parameter filtering */
DET_LOCAL boolean Det_CheckFilterMatch(uint16 moduleId, uint8 instanceId)
{
    boolean retVal = TRUE;
    /* Check if the range of the module ID is not BSW */
    if (moduleId >= DET_MAX_BSW_MODULE_ID)
    {
        const Det_ModuleType* moduleCfg = Det_ConfigPtr->ModuleConfigPtr;
        retVal                          = FALSE;
        uint16 moduleConfigNbr          = Det_ConfigPtr->ModuleConfigNbr;
        for (uint16 iloop = 0u; (iloop < moduleConfigNbr) && (FALSE == retVal); ++iloop)
        {
            /* Check if the request module ID is match as the user configuration ID */
            if (moduleCfg->ModuleId == moduleId)
            {
                uint16 instanceConfigNbr = moduleCfg->InstanceConfigNbr;
                for (uint16 jloop = 0u; (jloop < instanceConfigNbr) && (FALSE == retVal); jloop++)
                {
                    /* Check if the request instance ID is match as the user configuration ID */
                    if (moduleCfg->InstanceConfigPtr[jloop] == instanceId)
                    {
                        retVal = TRUE;
                    }
                }
            }
            moduleCfg++;
        }
    }
    return retVal;
}
#define DET_STOP_SEC_CODE
#include "Det_MemMap.h"
/* PRQA S 6030 -- */

#endif
