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
**************************************************************************** **
**                                                                           **
**  FILENAME    : FiM.h                                                      **
**                                                                           **
**  Created on  : 2020/5/9 15:21:52                                          **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION : FiM                                                        **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */
/*======================[R E V I S I O N   H I S T O R Y]=====================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>
 *   V1.0.0     2020/5/9    tao.yu        Create
 *   V2.0.0     2021-4-8    tao.yu      release version
 *   V2.0.1     2022-12-29  tong.zhao
 *     1> check the macro FIM_NUMBER_OF_COMPONENT performs non-zero
 *        before using array FiM_ComponentStu
 *   V2.0.2     2023-9-1     tao.yu      CPT-6486, plausibility check on the EventId
 *   V2.0.3     2023-12-19   tao.yu      CPT-7897, init FID to available
 *   V2.0.4     2024-01-22   tao.yu      QAC
 *   V2.0.5     2024-02-22   xinrun.wang modify index dataType u8->u16
 *   V2.0.6     2024-04-08   tao.yu      CPT-8691,fix fid counter error
 *              2024-04-08   xinrun.wang FiM_SetFunctionAvailable control by macro
 *   V2.0.7     2024-05-13   hao.wen     1. Add FIM_LOCAL FIM_LOCAL_INLINE. 2. Resolve FiM memory access exceeding
                                         range.
*               2024-08-13   xue.han     QAC
 *   V2.0.8     2024-12-16   tao.yu      CPT-11957, fix init status when trigger by dem is off

============================================================================*/
#ifndef FIM_H_
#define FIM_H_

/******************************************************************************
**                      Includes                                             **
******************************************************************************/
#include "FiM_Cfg.h"
#include "Dem_Types.h"
#include "FiM_Types.h"

/******************************************************************************
**                      Global Symbols                                       **
******************************************************************************/
/* Published information elements */
#define FIM_MODULE_ID                11u
#define FIM_VENDOR_ID                62u
#define FIM_INSTANCE_ID              0u
#define FIM_AR_RELEASE_MAJOR_VERSION 4u
#define FIM_AR_RELEASE_MINOR_VERSION 2u
#define FIM_AR_RELEASE_PATCH_VERSION 2u
#define FIM_SW_MAJOR_VERSION         2u
#define FIM_SW_MINOR_VERSION         0u
#define FIM_SW_PATCH_VERSION         8u

/* Error classification */

#define FIM_E_UNINIT               ((uint8)0x01u)
#define FIM_E_FID_OUT_OF_RANGE     ((uint8)0x02u)
#define FIM_E_EVENTID_OUT_OF_RANGE ((uint8)0x03u)
#define FIM_E_PARAM_POINTER        ((uint8)0x04u)
#define FIM_E_INIT_FAILED          ((uint8)0x05u)

/* Service ID */
#define FIM_SERVICE_ID_INIT                        ((uint8)0x00u)
#define FIM_SERVICE_ID_GETFUNCTIONPERMISSION       ((uint8)0x01u)
#define FIM_SERVICE_ID_SETFUNCTIONAVAILABLE        ((uint8)0x07u)
#define FIM_SERVICE_ID_DEMTRIGGERONEVENTSTATUS     ((uint8)0x02u)
#define FIM_SERVICE_ID_DEMTRIGGERONCOMPONENTSTATUS ((uint8)0x06u)
#define FIM_SERVICE_ID_DEMINIT                     ((uint8)0x03u)
#define FIM_SERVICE_ID_GETVERSIONINFO              ((uint8)0x04u)
#define FIM_SERVICE_ID_MAINFUNCTION                ((uint8)0x05u)

/* Pre-compile configuration parameter type definitions */

/****link time configuration parameter type definitions**********/
/****End of link time configuration parameter type definition****/

/* Post-build configuration parameter type definitions */

typedef struct
{
    Dem_ComponentIdType ComponentId;
} FiM_InhComponentType;

typedef struct
{
    /*  this number of summary event include input event */
    uint16 InputEventCnt;
    /* reference to FiM_ConfigType --> EventIdPtr
    num <= iMMaxInputEventsPerSummaryEvents*/
    const uint16* InputEventRefPtr;
} FiMSummaryEventType;

typedef struct
{
    /* reference fim function id ,fill index */
    uint16 InhFunctionIdRef;
    /* The configuration parameter is used to specify the inhibition mask for
    an event - FID relation. */
    FiMInhInhibitionMaskType InhInhibitionMask;
    /* number of the Reference DemComponent */
    uint16 InhComponentCnt;
    /* Reference to a DemComponent which is necessary for function permission. */
    const FiM_InhComponentType* InhComponentPtr;
    /* number of the Reference dem event
    num <= FiMMaxEventsPerFidInhibitionConfiguration */
    uint16 InhEventCnt;
    /* Selection of an single DEM Event. */
    const uint16* InhEventRefPtr;
    /* number of the reference summarized Event */
    uint16 InhSummaryCnt;
    /* Selection of a summarized Event.
    num <= FiMMaxSumEventsPerFidInhibitionConfiguration */
    const uint16* InhSummaryRefPtr;

} FiM_InhibitionCfgType;

typedef struct
{
    /* number of the fim module include fids */
    uint16 FunctionIdCnt;
    /* This container includes symbolic names of all FIDs */
    const FiM_FunctionIdType* FunctionIdPtr;
    /* number of the inhition configure
     <=FiMMaxFiMInhibitionConfigurations */
    uint16 InhibitionCfgCnt;
    /* This container includes all configuration parameters
    concerning the relationship between event and FID. */
    const FiM_InhibitionCfgType* InhCfgPtr;
    /* number of the summarized events in fim module
    <=FiMMaxSummaryEvents */
    uint16 SummaryEventCnt;
    /* The summarized EventId definition record consists of a
    summarized event ID and specific Dem Events */
    const FiMSummaryEventType* SummaryEventPtr;
    /* number of the fim module include events */
    uint16 EventIdCnt;
    /* This container includes symbolic names of all events */
    const Dem_EventIdType* EventIdPtr;
} FiM_ConfigType;

extern CONST(FiM_ConfigType, AUTOMATIC) FimPbCfg;

/***************************Public API Declarations****************************/

/**
 * This service initializes the FIM.
 * Service ID: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): FiMConfigPtr
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
extern FUNC(void, FIM_CODE) FiM_Init(P2CONST(FiM_ConfigType, AUTOMATIC, FIM_APPL_DATA) FiMConfigPtr);

/**
 * This service reports the permission state to the functionality.
 * Service ID: 0x01
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): FID:Identification of a functionality by assigned FID
 * Parameters(INOUT): NA
 * Parameters(OUT): Permission:TRUE: FID has permission to run
                    FALSE: FID has no permission to run
 * Return value: Std_ReturnType
            E_OK: The request is accepted
            E_NOT_OK: The request is not accepted
 */
FUNC(Std_ReturnType, FIM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
FiM_GetFunctionPermission(FiM_FunctionIdType FID, P2VAR(boolean, AUTOMATIC, FIM_APPL_DATA) Permission);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

#if (STD_ON == FIM_AVAILABILITY_SUPPORT)
/**
 * This service sets the availability of a function.
 * Service ID: 0x07
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): FID:Identification of a functionality by assigned FID
                    Availability:The permission of the requested FID:
                                TRUE: Function is available.
                                FALSE: Function is not available.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: Std_ReturnType
            E_OK: The request is accepted
            E_NOT_OK: The request is not accepted
 */
FUNC(Std_ReturnType, FIM_CODE)
FiM_SetFunctionAvailable(FiM_FunctionIdType FID, boolean Availability);
#endif /* STD_ON == FIM_AVAILABILITY_SUPPORT */
#if (STD_ON == FIM_EVENT_UPDATE_TRIGGERED_BY_DEM)
/**
 * This service is provided by the Dem in order to call FiM upon status changes.
 * Service ID: 0x02
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): EventId:Identification of an Event by assigned event number
                    EventStatusByteOld:Extended event status before change
                    EventStatusByteNew:Detected / reported of event status
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: Std_ReturnType
            E_OK: The request is accepted
            E_NOT_OK: The request is not accepted
 */
FUNC(void, FIM_CODE)
FiM_DemTriggerOnEventStatus(
    Dem_EventIdType EventId,
    Dem_UdsStatusByteType EventStatusByteOld,
    Dem_UdsStatusByteType EventStatusByteNew);

/**
 * Triggers on changes of the component failed status.
 * Service ID: 0x06
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): ComponentId:Identification of a DemComponent.
                    ComponentFailedStatus:New FAILED status of the component
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, FIM_CODE)
FiM_DemTriggerOnComponentStatus(Dem_ComponentIdType ComponentId, boolean ComponentFailedStatus);
#endif /* STD_ON == FIM_EVENT_UPDATE_TRIGGERED_BY_DEM */
/**
 * This service re-initializes the FIM.
 * Service ID: 0x03
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN):NA
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, FIM_CODE)
FiM_DemInit(void);

#if (STD_ON == FIM_VERSION_INFO_API)
/**
 * This service returns the version information of this module.
 * Service ID: 0x04
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):NA
 * Parameters(INOUT): NA
 * Parameters(OUT): versioninfo:Pointer to where to store the
                version information of this module
 * Return value: NA
 */
FUNC(void, FIM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
FiM_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, FIM_APPL_DATA) versioninfo);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /* STD_ON == FIM_VERSION_INFO_API */

#endif /* FIM_H_ */
