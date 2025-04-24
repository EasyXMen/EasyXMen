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
**  FILENAME    : Det.h                                                       **
**                                                                            **
**  Created on  :                                                             **
**  Author      : Haibin.shao                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Type definition and API declaration for DET                 **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*======================[Revision   History]==================================*/
/*  <VERSION>   <DATE>      <AUTHOR>        <REVISION LOG>
 *  V1.0.0      20210123    Haibin.Shao     First Version
 *  V1.0.1      20230918    tao.yu          remove warning
 *  V1.0.2      20231101    haibin.shao     update condition of Det_EndlessLoop
 *  V1.0.3      20231101    haibin.shao     The Det_EndlessLoop function was changed to be generated with configuration
 *  V1.0.4      20240202    xue.han         QAC
 */
#ifndef DET_H
#define DET_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Det_Types.h"
#include "Det_Cfg.h"

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define DET_VENDOR_ID                (62u)
#define DET_MODULE_ID                (15u)
#define DET_INSTANCE_ID              (0u)
#define DET_AR_RELEASE_MAJOR_VERSION (4u)
#define DET_AR_RELEASE_MINOR_VERSION (5u)
#define DET_AR_RELEASE_PATCH_VERSION (0u)
#define DET_SW_MAJOR_VERSION         (1u)
#define DET_SW_MINOR_VERSION         (0u)
#define DET_SW_PATCH_VERSION         (4u)

/* Development Errors */
#define DET_E_PARAM_POINTER (1u)

/* Define API service IDs */
#define DET_SID_INIT                 (0x00u)
#define DET_SID_START                (0x02u)
#define DET_SID_REPORTERROR          (0x01u)
#define DET_SID_REPORTRUNTIMEERROR   (0x04u)
#define DET_SID_REPORTTRANSIENTFAULT (0x05u)
#define DET_SID_GETVERSIONINFO       (0x03u)

/* Error codes */
#define DET_E_CBK_REGISTRATION_FAILED     0x01
#define DET_E_INDEX_OUT_OF_RANGE          0x02

#define DET_CBK_REGISTRATION_FAILED_INDEX 0xFF
#define DET_CALLBACK_API                  0xFF

#define DET_CHECK_FILTER_MATCH_ENABLE     STD_OFF

#define SECTION_RAMLOG

/* Number of entries in ramlog */
#define DET_MAX_RAMLOG_SIZE           (16)
#define DET_RUNTIME_MAX_RAMLOG_SIZE   (16)
#define DET_TRANSIENT_MAX_RAMLOG_SIZE (16)

/* don't care patterns for filters */
#define DET_DONTCARE_8  (0xFFu)
#define DET_DONTCARE_16 (0xFFFFu)

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

/*******************************************************************************
**                      Global Data Declarations                              **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#if ((STD_ON == DET_ERROR_HOOK_ENABLED) && (DET_SIZE_OF_ERROR_HOOK_TABLE > 0))
extern CONST(Det_ErrorHookFncType, DLT_CONST) Det_GetErrorHookTable[DET_SIZE_OF_ERROR_HOOK_TABLE];
#endif /* STD_ON == DET_TRANSIENT_FAULT_CALLOUT_ENABLED */

#if ((STD_ON == DET_RUNTIME_ERROR_CALLOUT_ENABLED) && (DET_SIZE_OF_RUNTIME_ERROR_CALLOUT_TABLE > 0))
extern CONST(Det_CalloutFncType, DLT_CONST) Det_GetRuntimeErrorCalloutTable[DET_SIZE_OF_RUNTIME_ERROR_CALLOUT_TABLE];
#endif /* STD_ON == DET_RUNTIME_ERROR_CALLOUT_ENABLED */

#if ((STD_ON == DET_TRANSIENT_FAULT_CALLOUT_ENABLED) && (DET_SIZE_OF_TRANSIENT_FAULT_CALLOUT_TABLE > 0))
extern CONST(Det_CalloutFncType, DLT_CONST)
    Det_GetTransientFaultCalloutTable[DET_SIZE_OF_TRANSIENT_FAULT_CALLOUT_TABLE];
#endif /* STD_ON == DET_TRANSIENT_FAULT_CALLOUT_ENABLED */

#if (STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT)
extern CONST(Det_ConfigType, DET_CONST) Det_Config;
#endif /* STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT */

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

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
extern FUNC(void, DET_CODE) Det_Init(P2CONST(Det_ConfigType, AUTOMATIC, DET_APPL_CONST) ConfigPtr);

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
extern FUNC(void, DET_CODE) Det_Start(void);

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
extern FUNC(void, DET_CODE) Det_InitMemory(void);

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
 * Return              Std_ReturnType: E_OK  always
 */
/******************************************************************************/
extern FUNC(Std_ReturnType, DET_CODE) Det_ReportError(
    VAR(uint16, AUTOMATIC) ModuleId,
    VAR(uint8, AUTOMATIC) InstanceId,
    VAR(uint8, AUTOMATIC) ApiId,
    VAR(uint8, AUTOMATIC) ErrorId);

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
 * Return              Std_ReturnType: E_OK  always
 */
/******************************************************************************/
extern FUNC(Std_ReturnType, DET_CODE) Det_ReportRuntimeError(
    VAR(uint16, AUTOMATIC) ModuleId,
    VAR(uint8, AUTOMATIC) InstanceId,
    VAR(uint8, AUTOMATIC) ApiId,
    VAR(uint8, AUTOMATIC) ErrorId);

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
extern FUNC(Std_ReturnType, DET_CODE) Det_ReportTransientFault(
    VAR(uint16, AUTOMATIC) ModuleId,
    VAR(uint8, AUTOMATIC) InstanceId,
    VAR(uint8, AUTOMATIC) ApiId,
    VAR(uint8, AUTOMATIC) FaultId);

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
#if (STD_ON == DET_VERSION_INFO_API)
extern FUNC(void, DET_CODE) Det_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, DET_APPL_DATA) versioninfo);
#endif /* STD_ON == DET_VERSION_INFO_API */

#if (STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT)

#if (DET_SIZE_OF_ERROR_HOOK_TABLE > 0)
extern FUNC(uint8, DET_CODE) Det_AddCallback(Det_CallbackFncType detCbk);
extern FUNC(void, DET_CODE) Det_RemoveCallback(uint8 detCbkIndex);
#endif /* DET_SIZE_OF_ERROR_HOOK_TABLE > 0 */

#if (DET_SIZE_OF_RUNTIME_ERROR_CALLOUT_TABLE > 0)
extern FUNC(uint8, DET_CODE) Det_AddRunTimeCallback(Det_CallbackFncType detCbk);
extern FUNC(void, DET_CODE) Det_RemoveRunTimeCallback(uint8 detCbkIndex);
#endif /* DET_SIZE_OF_RUNTIME_ERROR_CALLOUT_TABLE > 0 */

#if (DET_SIZE_OF_TRANSIENT_FAULT_CALLOUT_TABLE > 0)
extern FUNC(uint8, DET_CODE) Det_AddTransientCallback(Det_CallbackFncType detCbk);
extern FUNC(void, DET_CODE) Det_RemoveTransientCallback(uint8 detCbkIndex);
#endif /* DET_SIZE_OF_TRANSIENT_FAULT_CALLOUT_TABLE > 0 */

#endif /* STD_ON == DET_USE_EXTENDED_DEBUG_SUPPORT */

#endif
