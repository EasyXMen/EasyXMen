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
#include "Det_Types.h"
#include "Det.h"
#include "Det_Externals.h"
#include "SchM_Det.h"

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

/* ========================================== internal function declarations ======================================== */
#if (DET_SIZE_OF_NON_BSW_MODULE_CFG > 0)
#define DET_START_SEC_CODE
#include "Det_MemMap.h"
/* This API is used to check the matching result of parameter filtering */
DET_LOCAL boolean Det_CheckFilterMatch(uint16 moduleId, uint8 instanceId, uint8 apiId, uint8 faultId);
#define DET_STOP_SEC_CODE
#include "Det_MemMap.h"
#endif

/* ============================================ internal data definitions =========================================== */
#define DET_START_SEC_VAR_CLEARED_8
#include "Det_MemMap.h"
DET_LOCAL Det_StateType Det_RunState;
#define DET_STOP_SEC_VAR_CLEARED_8
#include "Det_MemMap.h"

#define DET_START_SEC_VAR_CLEARED_8
#include "Det_MemMap.h"
/* use this variable to configure DET debug operation */
DET_LOCAL Det_StatusType Det_Status;
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
#define DET_START_SEC_VAR_CLEARED_8
#include "Det_MemMap.h"
/* recursion limit counter for develop error */
uint8 Det_RecursionCounterReportError; /* PRQA S 3408,1504 */ /* VL_Det_3408,VL_Det_1504 */
/* recursion limit counter for runtime error */
uint8 Det_RecursionCounterReportRuntimeError; /* PRQA S 3408,1504 */ /* VL_Det_3408,VL_Det_1504 */
/* recursion limit counter for transient fault */
uint8 Det_RecursionCounterReportTransientFault; /* PRQA S 3408,1504 */ /* VL_Det_3408,VL_Det_1504 */
#define DET_STOP_SEC_VAR_CLEARED_8
#include "Det_MemMap.h"

/* ========================================== external function definitions ========================================= */
#define DET_START_SEC_CODE
#include "Det_MemMap.h"

/* This API is called by the EcuM module to initialize the Det module */
void Det_Init(const Det_ConfigType* configPtr) /* PRQA S 1503 */ /* VL_Det_1503 */
{
    /* Checks if the Det module is not initialized and configPtr is not null pointer */
    if ((Det_RunState == DET_UNINITIALIZED) && (NULL_PTR != configPtr))
    {
#if (DET_SIZE_OF_NON_BSW_MODULE_CFG > 0)
        /* Copy global Configuration item to store the configuration pointer. */
        Det_ConfigPtr = configPtr;
#endif
        /* Set the initial value for the DET module's logging */
        Det_Status.LogActive = (boolean)FALSE;
        /* Set the status of the DET module to initialized */
        Det_RunState = DET_INITIALIZED;
        /* Set first value of recursion limit counter */
        Det_RecursionCounterReportError          = 0u;
        Det_RecursionCounterReportRuntimeError   = 0u;
        Det_RecursionCounterReportTransientFault = 0u;
    }
}

#if (STD_ON == DET_VERSION_INFO_API)
/* This API is used to get software version information from the DET module */
void Det_GetVersionInfo(Std_VersionInfoType* versionInfoPtr) /* PRQA S 1503 */ /* VL_Det_1503 */
{
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
}
#endif /* STD_ON == DET_VERSION_INFO_API */

/* This API is used to start the DET module for error reporting */
void Det_Start(void) /* PRQA S 1503 */ /* VL_Det_1503 */
{
    /* Checks if the Det module is initialized */
    if (Det_RunState == DET_INITIALIZED)
    {
        /* Set the status of the DET module to started */
        Det_RunState = DET_STARTED;
        /* The logging of the DET module is set to active */
        Det_Status.LogActive = (boolean)TRUE;
    }
}

/* PRQA S 6030,2991,2995 ++ */ /* VL_MTR_Det_STMIF,VL_Det_2991,VL_Det_2995 */

/* This API is used to report development error for DET modules */
/* PRQA S 1503 ++ */ /* VL_Det_1503 */
Std_ReturnType Det_ReportError(uint16 moduleId, uint8 instanceId, uint8 apiId, uint8 errorId)
/* PRQA S 1503 -- */
{
    Std_ReturnType retVal = E_OK;
    /* Check if the running status of the DET module is Started */
    if (Det_RunState == DET_STARTED)
    {
        SchM_Enter_Det_DET_EXCLUSIVE_AREA_1();
        /* increment nesting counter for recursion detection */
        if (++Det_RecursionCounterReportError < DET_REPORT_ERROR_RECURSIONLIMIT) /* PRQA S 3440 */ /* VL_Det_3440 */
        {
            SchM_Exit_Det_DET_EXCLUSIVE_AREA_1();
#if (DET_SIZE_OF_NON_BSW_MODULE_CFG > 0)
            /* Check if the parameter filter of the DET module is matched successfully */
            if ((boolean)TRUE == Det_CheckFilterMatch(moduleId, instanceId, apiId, errorId))
#endif
            {
#if (DET_SIZE_OF_ERROR_HOOK_TABLE > 0U)
                SchM_Enter_Det_DET_EXCLUSIVE_AREA_0();
                /* call error hooks (if configured) */
                /* PRQA S 1290,2814,2986,1252 ++ */ /* VL_Det_1290,VL_QAC_DerefNullPtr,VL_Det_2986,VL_QAC_1252 */
                for (uint32 iloop = 0u; (((Std_ReturnType)E_OK == retVal) && (iloop < DET_SIZE_OF_ERROR_HOOK_TABLE));
                     ++iloop)
                {
                    if (NULL_PTR != Det_GetErrorHookTable[iloop])
                    {
                        retVal |= Det_GetErrorHookTable[iloop](moduleId, instanceId, apiId, errorId);
                    }
                }
                /* PRQA S 1290,2814,2986,1252 -- */
                SchM_Exit_Det_DET_EXCLUSIVE_AREA_0();
#endif /* DET_SIZE_OF_ERROR_HOOK_TABLE > 0U */
#if (STD_ON == DET_FORWARD_TO_DLT)
                /* forward DET error to DLT module (if configured) unless filtered out by a DLT filter */
                Dlt_DetForwardErrorTrace(moduleId, instanceId, apiId, errorId);
#endif /* STD_ON == DET_FORWARD_TO_DLT*/
                if ((Std_ReturnType)E_OK == retVal)
                {
                    /* Check if logging record are activated */
                    if ((boolean)TRUE == Det_Status.LogActive)
                    {
                        /* Enter infinite loop */
                        Det_EndlessLoop();
                    }
                }
            }
        }
        else
        {
            SchM_Exit_Det_DET_EXCLUSIVE_AREA_1();
        }
    }
    DET_PARAM_UNUSED(moduleId);
    DET_PARAM_UNUSED(instanceId);
    DET_PARAM_UNUSED(apiId);
    DET_PARAM_UNUSED(errorId);
    return retVal;
}

/* This API is used to report runtime error for DET modules */
/* PRQA S 1503,3206 ++ */ /* VL_Det_1503,VL_Det_3206 */
Std_ReturnType Det_ReportRuntimeError(uint16 moduleId, uint8 instanceId, uint8 apiId, uint8 errorId)
/* PRQA S 1503,3206 -- */
{
    Std_ReturnType retVal = E_OK;
    /* Check if the running status of the DET module is Started */
    if (Det_RunState == DET_STARTED)
    {
        SchM_Enter_Det_DET_EXCLUSIVE_AREA_1();
        /* increment nesting counter for recursion detection */ /* PRQA S 3440 ++ */ /* VL_Det_3440 */
        if (++Det_RecursionCounterReportRuntimeError < DET_REPORT_RUNTIMEERROR_RECURSIONLIMIT)
        /* PRQA S 3440 -- */
        {
            SchM_Exit_Det_DET_EXCLUSIVE_AREA_1();
#if (DET_SIZE_OF_NON_BSW_MODULE_CFG > 0)
            /* Check if the parameter filter of the DET module is matched successfully */
            if ((boolean)TRUE == Det_CheckFilterMatch(moduleId, instanceId, apiId, errorId))
#endif
            {
#if (DET_SIZE_OF_ERROR_HOOK_TABLE > 0U)
                SchM_Enter_Det_DET_EXCLUSIVE_AREA_0();
                /* call error hooks (if configured) */
                /* PRQA S 1290,2814,2986,1252 ++ */ /* VL_Det_1290,VL_QAC_DerefNullPtr,VL_Det_2986,VL_QAC_1252 */
                for (uint32 iloop = 0u; (((Std_ReturnType)E_OK == retVal) && (iloop < DET_SIZE_OF_ERROR_HOOK_TABLE));
                     ++iloop)
                {
                    if (NULL_PTR != Det_GetErrorHookTable[iloop])
                    {
                        retVal |= Det_GetErrorHookTable[iloop](moduleId, instanceId, apiId, errorId);
                    }
                }
                /* PRQA S 1290,2814,2986,1252 -- */
                SchM_Exit_Det_DET_EXCLUSIVE_AREA_0();
#endif /* DET_SIZE_OF_ERROR_HOOK_TABLE > 0U */
#if (DET_SIZE_OF_RUNTIME_ERROR_CALLOUT_TABLE > 0U)
                SchM_Enter_Det_DET_EXCLUSIVE_AREA_0();
                /* call runtime error callouts (if configured) */
                /* PRQA S 1290,2814,2986,1252 ++ */ /* VL_Det_1290,VL_QAC_DerefNullPtr,VL_Det_2986,VL_QAC_1252 */
                for (uint32 iloop = 0u;
                     (((Std_ReturnType)E_OK == retVal) && (iloop < DET_SIZE_OF_RUNTIME_ERROR_CALLOUT_TABLE));
                     ++iloop)
                {
                    if (Det_GetRuntimeErrorCalloutTable[iloop] != NULL_PTR)
                    {
                        retVal |= Det_GetRuntimeErrorCalloutTable[iloop](moduleId, instanceId, apiId, errorId);
                    }
                }
                /* PRQA S 1290,2814,2986,1252 -- */
                SchM_Exit_Det_DET_EXCLUSIVE_AREA_0();
#endif /* DET_SIZE_OF_RUNTIME_ERROR_CALLOUT_TABLE > 0U */
#if (STD_ON == DET_FORWARD_TO_DLT)
                /* forward DET error to DLT module (if configured) unless filtered out by a DLT filter */
                Dlt_DetForwardErrorTrace(moduleId, instanceId, apiId, errorId);
#endif /* STD_ON == DET_FORWARD_TO_DLT */
                if ((Std_ReturnType)E_OK == retVal)
                {
                    /* Check if logging record are activated */
                    if ((boolean)TRUE == Det_Status.LogActive)
                    {
                        /* do nothing */
                    }
                }
            }
        }
        else
        {
            SchM_Exit_Det_DET_EXCLUSIVE_AREA_1();
        }
    }
    DET_PARAM_UNUSED(moduleId);
    DET_PARAM_UNUSED(instanceId);
    DET_PARAM_UNUSED(apiId);
    DET_PARAM_UNUSED(errorId);
    return retVal;
}

/* This API is used to report transient fault for DET modules */
/* PRQA S 1503 ++ */ /* VL_Det_1503 */
Std_ReturnType Det_ReportTransientFault(uint16 moduleId, uint8 instanceId, uint8 apiId, uint8 faultId)
/* PRQA S 1503 -- */
{
    Std_ReturnType retVal = E_OK;
    /* Check if the running status of the DET module is Started */
    if (Det_RunState == DET_STARTED)
    {
        SchM_Enter_Det_DET_EXCLUSIVE_AREA_1();
        /* increment nesting counter for recursion detection */ /* PRQA S 3440 ++ */ /* VL_Det_3440 */
        if (++Det_RecursionCounterReportTransientFault < DET_REPORT_TRANSIENTFAULT_RECURSIONLIMIT)
        /* PRQA S 3440 -- */
        {
            SchM_Exit_Det_DET_EXCLUSIVE_AREA_1();
#if (DET_SIZE_OF_NON_BSW_MODULE_CFG > 0)
            /* Check if the parameter filter of the DET module is matched successfully */
            if ((boolean)TRUE == Det_CheckFilterMatch(moduleId, instanceId, apiId, faultId))
#endif
            {
#if (DET_SIZE_OF_ERROR_HOOK_TABLE > 0U)
                SchM_Enter_Det_DET_EXCLUSIVE_AREA_0();
                /* call error hooks (if configured) */
                /* PRQA S 1290,2814,2986,1252 ++ */ /* VL_Det_1290,VL_QAC_DerefNullPtr,VL_Det_2986,VL_QAC_1252 */
                for (uint32 iloop = 0u; (((Std_ReturnType)E_OK == retVal) && (iloop < DET_SIZE_OF_ERROR_HOOK_TABLE));
                     ++iloop)
                {
                    if (NULL_PTR != Det_GetErrorHookTable[iloop])
                    {
                        retVal |= Det_GetErrorHookTable[iloop](moduleId, instanceId, apiId, faultId);
                    }
                }
                /* PRQA S 1290,2814,2986,1252-- */
                SchM_Exit_Det_DET_EXCLUSIVE_AREA_0();
#endif /* DET_SIZE_OF_ERROR_HOOK_TABLE > 0U */
#if (DET_SIZE_OF_TRANSIENT_FAULT_CALLOUT_TABLE > 0U)
                SchM_Enter_Det_DET_EXCLUSIVE_AREA_0();
                /* call transient fault callouts (if configured) */
                /* PRQA S 1290,2814,2986,1252 ++ */ /* VL_Det_1290,VL_QAC_DerefNullPtr,VL_Det_2986,VL_QAC_1252 */
                for (uint32 iloop = 0u;
                     (((Std_ReturnType)E_OK == retVal) && (iloop < DET_SIZE_OF_TRANSIENT_FAULT_CALLOUT_TABLE));
                     ++iloop)
                {
                    if (Det_GetTransientFaultCalloutTable[iloop] != NULL_PTR)
                    {
                        retVal |= Det_GetTransientFaultCalloutTable[iloop](moduleId, instanceId, apiId, faultId);
                    }
                }
                /* PRQA S 1290,2814,2986,1252 -- */
                SchM_Exit_Det_DET_EXCLUSIVE_AREA_0();
#endif /* DET_SIZE_OF_TRANSIENT_FAULT_CALLOUT_TABLE > 0U */
#if (STD_ON == DET_FORWARD_TO_DLT)
                /* forward DET error to DLT module (if configured) unless filtered out by a DLT filter */
                Dlt_DetForwardErrorTrace(moduleId, instanceId, apiId, faultId);
#endif /* STD_ON == DET_FORWARD_TO_DLT */
                if ((Std_ReturnType)E_OK == retVal)
                {
                    /* Check if logging record are activated */
                    if ((boolean)TRUE == Det_Status.LogActive)
                    {
                        /* do nothing */
                    }
                }
            }
        }
        else
        {
            SchM_Exit_Det_DET_EXCLUSIVE_AREA_1();
        }
    }
    DET_PARAM_UNUSED(moduleId);
    DET_PARAM_UNUSED(instanceId);
    DET_PARAM_UNUSED(apiId);
    DET_PARAM_UNUSED(faultId);
    return retVal;
}

#define DET_STOP_SEC_CODE
#include "Det_MemMap.h"

/* ========================================== internal function definitions ========================================= */
#if (DET_SIZE_OF_NON_BSW_MODULE_CFG > 0)
#define DET_START_SEC_CODE
#include "Det_MemMap.h"

/* This API is used to check the matching result of parameter filtering */
DET_LOCAL boolean Det_CheckFilterMatch(uint16 moduleId, uint8 instanceId, uint8 apiId, uint8 faultId)
{
    boolean retVal = (boolean)TRUE;
    /* Check if the range of the module ID is not BSW */
    /* PRQA S 2814,2824,1290 ++ */ /* VL_QAC_DerefNullPtr,VL_QAC_DerefNullPtr,VL_Det_1290 */
    if ((NULL_PTR != Det_ConfigPtr) && (moduleId >= DET_MAX_BSW_MODULE_ID))
    {
        boolean               ModuleIdCheckFlag   = (boolean)FALSE;
        boolean               InstanceIdCheckFlag = (boolean)FALSE;
        const Det_ModuleType* moduleCfg           = Det_ConfigPtr->ModuleConfigPtr;
        retVal                                    = (boolean)FALSE;
        for (uint16 iloop = 0; (iloop < Det_ConfigPtr->ModuleConfigNbr) && ((boolean)FALSE == ModuleIdCheckFlag);
             ++iloop)
        {
            const Det_ModuleType module = moduleCfg[iloop];
            /* Check if the request module ID is match as the user configuration ID */
            if (module.ModuleId == moduleId)
            {
                for (uint16 jloop = 0; (jloop < module.InstanceConfigNbr) && ((boolean)FALSE == InstanceIdCheckFlag);
                     jloop++)
                {
                    /* Check if the request instance ID is match as the user configuration ID */
                    if (module.InstanceConfigPtr[jloop] == instanceId)
                    {
                        InstanceIdCheckFlag = (boolean)TRUE;
                    }
                }
                ModuleIdCheckFlag = (boolean)TRUE;
            }
        }
        /* Check if the module ID and instance ID are match */
        if (((boolean)TRUE == ModuleIdCheckFlag) && ((boolean)TRUE == InstanceIdCheckFlag))
        {
            retVal = (boolean)TRUE;
        }
    }
    /* PRQA S 2814,2824,1290 -- */
    DET_PARAM_UNUSED((apiId));
    DET_PARAM_UNUSED((faultId));
    return retVal;
}
/* PRQA S 6030,2991,2995 -- */

#define DET_STOP_SEC_CODE
#include "Det_MemMap.h"
#endif
