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
**  FILENAME    : FiM.c                                                      **
**                                                                           **
**  Created on  : 2020/5/9 15:21:52                                          **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION : fim source code                                            **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */

/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:FiM<br>
  RuleSorce:puhua-rule.rcf 2.3.1

    \li PRQA S 3432 MISRA Rule 20.7 .<br>
    Reason:Function-like macros are used to allow more efficient code.

    \li PRQA S 2877 MISRA Dir 4.1 .<br>
    Reason:for loop condition is a macro definition, which varies in different projs.

    \li PRQA S 4340 MISRA Dir 10.5 .<br>
    Reason:Design required type conversions.

    \li PRQA S 4394 MISRA Dir 10.8 .<br>
    Reason:Design required type conversions.

    \li PRQA S 4404 MISRA Dir 10.3 .<br>
    Reason:Design required type conversions.

    \li PRQA S 4440 MISRA Dir 10.3 .<br>
    Reason:Design required type conversions.
 */

/******************************************************************************
**                      Includes                                             **
******************************************************************************/
#include "Dem.h"
#include "FiM_Internal.h"
#include "SchM_FiM.h"
#if (STD_ON == FIM_DEV_ERROR_DETECT)
#include "Det.h"
#endif /* STD_ON == FIM_DEV_ERROR_DETECT */

/******************************************************************************
**                      Imported Compiler Switch Check                       **
******************************************************************************/

/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#ifndef FIM_AR_RELEASE_MAJOR_VERSION
#error " FiM version miss"
#endif
#ifndef FIM_AR_RELEASE_MINOR_VERSION
#error " FiM version miss"
#endif
#ifndef FIM_AR_RELEASE_PATCH_VERSION
#error " FiM version miss"
#endif

#if (FIM_MAX_EVENTS_PER_INH_CFG > FIM_NUMBER_OF_EVENT)
#error "per events in Inhibition configure is error!"
#endif /* FIM_MAX_EVENTS_PER_INH_CFG>FIM_NUMBER_OF_EVENT */

#if (FIM_MAX_SUM_PER_INH_CFG > FIM_MAX_SUM_EVENTS)
#error "per summarize event in Inhibition configure is error!"
#endif /* FIM_MAX_SUM_PER_INH_CFG > FIM_MAX_SUM_EVENTS */

/******************************************************************************
**                      Private Macro Definitions                            **
******************************************************************************/
#if (STD_ON == FIM_DEV_ERROR_DETECT)
FIM_LOCAL_INLINE void FIM_DET_REPORT(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportError(FIM_MODULE_ID, FIM_INSTANCE_ID, ApiId, ErrorId);
}
#endif /* STD_ON == FIM_DEV_ERROR_DETECT */

FIM_LOCAL_INLINE uint8 FIM_GETEVENTSTATUS(uint8 stu)
{
    return ((stu) & (FIM_DEM_UDS_STU_TF | FIM_DEM_UDS_STU_TNCTOC));
}

#ifndef FIM_UNUSED
#define FIM_UNUSED(a) (void)(a)
#endif

/* Symbolic name of invalid event ID (derived from Dem SWS) */
#define FIM_EVENT_ID_INVALID 0u
/******************************************************************************
**                      Private Type Definitions                             *
******************************************************************************/

/******************************************************************************
**                      Private Constant Definitions                         **
******************************************************************************/

/******************************************************************************
**                      Private Variable Definitions                         **
******************************************************************************/
#define FIM_START_SEC_VAR_INIT_8
#include "FiM_MemMap.h"
/*Module initialize status, TRUE initialized, FALSE not initialized*/
FIM_LOCAL VAR(uint8, FIM_VAR_INIT_8) FiM_InitStep = FIM_INIT_STEP_NONE;
#define FIM_STOP_SEC_VAR_INIT_8
#include "FiM_MemMap.h"

/* store nm post build configuration data pointer */
#define FIM_START_SEC_VAR_INIT_32
#include "FiM_MemMap.h"
FIM_LOCAL P2CONST(FiM_ConfigType, FIM_VAR_INIT_32, FIM_APPL_DATA) FiM_CfgPtr = NULL_PTR;
#define FIM_STOP_SEC_VAR_INIT_32
#include "FiM_MemMap.h"

/* per fid inner status variable */
#define FIM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "FiM_MemMap.h"
FIM_LOCAL VAR(FiM_InnerFuncIdStatusType, FIM_VAR_NO_INIT_UNSPECIFIED) FiM_FidStu[FIM_NUMBER_OF_FID];
#define FIM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "FiM_MemMap.h"

/* per event inner data recode */
#define FIM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "FiM_MemMap.h"
FIM_LOCAL VAR(FiM_InnerEventSatusType, FIM_VAR_NO_INIT_UNSPECIFIED)
    FiM_EventStu[FIM_NUMBER_OF_EVENT + (FIM_MAX_INPUT_EVENTS_PER_SUM * FIM_MAX_SUM_PER_INH_CFG * FIM_MAX_INH_CFGS)];
#define FIM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "FiM_MemMap.h"

/* per event inner data recode */
#if (FIM_NUMBER_OF_COMPONENT > 0)
#define FIM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "FiM_MemMap.h"
FIM_LOCAL VAR(FiM_InnerComponentSatusType, FIM_VAR_NO_INIT_UNSPECIFIED) FiM_ComponentStu[FIM_NUMBER_OF_COMPONENT];
#define FIM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "FiM_MemMap.h"
#endif
/******************************************************************************
**                      Private Function Definitions                         **
******************************************************************************/
#define FIM_START_SEC_CODE
#include "FiM_MemMap.h"
/* find fid value in post build configuration FunctionIdPtr index */
FIM_LOCAL FUNC(boolean, FIM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    FiM_FindFidIndex(FiM_FunctionIdType FID, P2VAR(uint16, AUTOMATIC, FIM_APPL_DATA) fidIndexPtr);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#if (STD_ON == FIM_EVENT_UPDATE_TRIGGERED_BY_DEM)
/* find componentId in inhibition configuration index */
FIM_LOCAL FUNC(boolean, FIM_CODE)
    FiM_FindComponetInhIndex(Dem_ComponentIdType ComponentId, P2VAR(uint16, AUTOMATIC, FIM_APPL_DATA) inhIndex);
#endif /* STD_ON == FIM_EVENT_UPDATE_TRIGGERED_BY_DEM */
/* poll mode get event status in FiMInhEventRef */
FIM_LOCAL FUNC(void, FIM_CODE) FiM_NormalEventHandle(uint16 eventIndex, uint16 inhIndex);
#if (0u < FIM_MAX_SUM_EVENTS)
/* poll mode handle summary event in FiMInhSumRef */
FIM_LOCAL FUNC(void, FIM_CODE) FiM_SummaryEventHandle(uint16 summaryIndex, uint16 inhIndex);
#endif /* 0u < FIM_MAX_SUM_EVENTS */
/* poll mode get component faile statue in FiMInhComponentRef */
FIM_LOCAL FUNC(void, FIM_CODE)
    FiM_ComponentHandle(uint16 componentIndex, Dem_ComponentIdType componentId, uint16 inhIndex);

/* fid perssion status handle */
FIM_LOCAL FUNC(void, FIM_CODE) FiM_FidPerssionHandle(uint16 fidIndex, boolean inhibstu);
#if (STD_ON == FIM_EVENT_UPDATE_TRIGGERED_BY_DEM)
/* trigger mode handle event link fid status */
FIM_LOCAL FUNC(void, FIM_CODE) FiM_TrigEventStatusHandle(Dem_EventIdType EventId, Dem_UdsStatusByteType EventStatus);
#endif /* STD_ON == FIM_EVENT_UPDATE_TRIGGERED_BY_DEM */

/* event status and inhibition mask process */
FIM_LOCAL FUNC(boolean, FIM_CODE) FiM_EventsStatusDiscern(
    P2CONST(FiM_InhibitionCfgType, AUTOMATIC, FIM_APPL_DATA) inhCfgPtr,
    Dem_UdsStatusByteType OldEventStatus,
    Dem_UdsStatusByteType NewEventStatus,
    boolean* Changed);

#define FIM_STOP_SEC_CODE
#include "FiM_MemMap.h"
/******************************************************************************
**                      Global Constant Definitions                          **
******************************************************************************/

/******************************************************************************
**                      Global Variable Definitions                          **
******************************************************************************/

/******************************************************************************
**                      Global Function Definitions                          **
******************************************************************************/

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
#define FIM_START_SEC_CODE
#include "FiM_MemMap.h"
FUNC(void, FIM_CODE)
FiM_Init(P2CONST(FiM_ConfigType, AUTOMATIC, FIM_APPL_DATA) FiMConfigPtr)
{
    uint16 index;
    const FiM_InhibitionCfgType* inhCfgPtr;
    FiM_InitStep = FIM_INIT_STEP_NONE;
#if (STD_ON == FIM_DEV_ERROR_DETECT)
    if (NULL_PTR == FiMConfigPtr)
    {
        FIM_DET_REPORT(FIM_SERVICE_ID_INIT, FIM_E_PARAM_POINTER);
    }
    else if (
        (NULL_PTR == FiMConfigPtr->FunctionIdPtr) || (NULL_PTR == FiMConfigPtr->InhCfgPtr)
#if (0u < FIM_MAX_SUM_EVENTS)
        || ((0u < FiMConfigPtr->SummaryEventCnt) && (NULL_PTR == FiMConfigPtr->SummaryEventPtr))
#endif /* 0u < FIM_MAX_SUM_EVENTS */
    )
    {
        FIM_DET_REPORT(FIM_SERVICE_ID_INIT, FIM_E_PARAM_POINTER);
    }
    else if (
        (0x0u == FiMConfigPtr->EventIdCnt) || (0x0u == FiMConfigPtr->FunctionIdCnt)
        || (0x0u == FiMConfigPtr->InhibitionCfgCnt) || (FIM_NUMBER_OF_FID < FiMConfigPtr->FunctionIdCnt)
        || (FIM_MAX_INH_CFGS < FiMConfigPtr->InhibitionCfgCnt) || (FIM_MAX_SUM_EVENTS < FiMConfigPtr->SummaryEventCnt)
        || (FIM_NUMBER_OF_EVENT < FiMConfigPtr->EventIdCnt))
    {
        FIM_DET_REPORT(FIM_SERVICE_ID_INIT, FIM_E_INIT_FAILED);
    }
    else
#endif
    {
        FiM_CfgPtr = FiMConfigPtr;
#if (STD_OFF == FIM_EVENT_UPDATE_TRIGGERED_BY_DEM)
        FiM_InitStep = FIM_INIT_STEP_DEM;
#else
        FiM_InitStep = FIM_INIT_STEP_LOCAL;
#endif
        /* PRQA S 2877++ */ /* MISRA Dir 4.1 */
        for (index = 0u; (FIM_INIT_STEP_LOCAL == FiM_InitStep) && (index < FiM_CfgPtr->InhibitionCfgCnt); index++)
        /* PRQA S 2877-- */ /* MISRA Dir 4.1 */
        {
            inhCfgPtr = &FiM_CfgPtr->InhCfgPtr[index];
            if ((NULL_PTR == inhCfgPtr->InhComponentPtr) && (NULL_PTR == inhCfgPtr->InhSummaryRefPtr)
                && (NULL_PTR == inhCfgPtr->InhEventRefPtr))
            {
                /* At least one FiMInhSumRef or FiMInhEventRef or
                FiMInhComponentRef needs to be configured */
                FiM_InitStep = FIM_INIT_STEP_NONE;
            }
            else if (
                (inhCfgPtr->InhEventCnt > FIM_MAX_EVENTS_PER_INH_CFG)
                || (inhCfgPtr->InhSummaryCnt > FIM_MAX_SUM_PER_INH_CFG)
                || (inhCfgPtr->InhFunctionIdRef > FiM_CfgPtr->FunctionIdCnt))
            {
                /* per unit is configured error */
                FiM_InitStep = FIM_INIT_STEP_NONE;
            }
            else
            {
#if (0u < FIM_MAX_SUM_EVENTS)
                /* check summerref in inhibition config */
                uint16 loop;
                for (loop = 0x0u; (FIM_INIT_STEP_LOCAL == FiM_InitStep) && (loop < inhCfgPtr->InhSummaryCnt); loop++)
                {
                    if (FiM_CfgPtr->SummaryEventCnt <= (inhCfgPtr->InhSummaryRefPtr[loop]))
                    {
                        FiM_InitStep = FIM_INIT_STEP_NONE;
                    }
                }
#endif /* 0u < FIM_MAX_SUM_EVENTS */
            }
        }

#if (STD_ON == FIM_DEV_ERROR_DETECT)
#if (STD_OFF == FIM_EVENT_UPDATE_TRIGGERED_BY_DEM)
        if (FIM_INIT_STEP_DEM != FiM_InitStep)
#else
        if (FIM_INIT_STEP_LOCAL != FiM_InitStep)
#endif
        {
            FIM_DET_REPORT(FIM_SERVICE_ID_INIT, FIM_E_INIT_FAILED);
        }
#endif
        /* clear inner variable */
        /* fid and event must exist */
        for (index = 0x0u; index < FIM_NUMBER_OF_FID; index++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
        {
            FiM_FidStu[index].inhCouter = 0x0u;
            FiM_FidStu[index].ctrlStu = (uint8)((1u << FIM_FID_PERMISSION_STU_BIT) | (1u << FIM_FID_AVAILABLE_BIT));
        }
#if (FIM_NUMBER_OF_EVENT > 0)
        for (index = 0x0u; index < FIM_NUMBER_OF_EVENT; index++)
        {
            FiM_EventStu[index].eventStu = 0x0u;
        }
#endif
#if (FIM_NUMBER_OF_COMPONENT > 0)
        for (index = 0x0u; index < FIM_NUMBER_OF_COMPONENT; index++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
        {
            FiM_ComponentStu[index].ComponentStu = FALSE;
        }
#endif
    }
}
#define FIM_STOP_SEC_CODE
#include "FiM_MemMap.h"

/* Interface SW-Components <---> FiM */

/**
 * This service reports the permission state to the functionality.
 * Service ID: 0x01
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): FID:Identification of a functionality by assigned FID
 * Parameters(INOUT): NA
 * Parameters(OUT): Permission
                    TRUE: FID has permission to run
                    FALSE: FID has no permission to run
 * Return value: Std_ReturnType
            E_OK: The request is accepted
            E_NOT_OK: The request is not accepted
 */
#define FIM_START_SEC_CODE
#include "FiM_MemMap.h"
FUNC(Std_ReturnType, FIM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
FiM_GetFunctionPermission(FiM_FunctionIdType FID, P2VAR(boolean, AUTOMATIC, FIM_APPL_DATA) Permission)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    uint16 fidIndex = 0xffffu;
#if (STD_ON == FIM_DEV_ERROR_DETECT)
    if (FIM_INIT_STEP_DEM != FiM_InitStep)
    {
        FIM_DET_REPORT(FIM_SERVICE_ID_GETFUNCTIONPERMISSION, FIM_E_UNINIT);
    }
    else if (Permission == NULL_PTR)
    {
        FIM_DET_REPORT(FIM_SERVICE_ID_GETFUNCTIONPERMISSION, FIM_E_PARAM_POINTER);
    }
    else
#endif
    {
        if (TRUE == FiM_FindFidIndex(FID, &fidIndex))
        {
            ret = E_OK;

            if ((1u << FIM_FID_AVAILABLE_BIT) == ((FiM_FidStu[fidIndex].ctrlStu) & (1u << FIM_FID_AVAILABLE_BIT)))
            {
                SchM_Enter_FiM_ExclusiveArea();
                /* PRQA S 4440++ */ /* MISRA Rule 10.3 */
                *Permission = (((FiM_FidStu[fidIndex].ctrlStu) & (1u << FIM_FID_PERMISSION_STU_BIT))) >> 1u;
                /* PRQA S 4440-- */ /* MISRA Rule 10.3 */
                SchM_Exit_FiM_ExclusiveArea();
            }
            else
            {
                /*not available ,Permission state always return false */
                *Permission = FALSE;
            }
        }
#if (STD_ON == FIM_DEV_ERROR_DETECT)
        else
        {
            FIM_DET_REPORT(FIM_SERVICE_ID_GETFUNCTIONPERMISSION, FIM_E_FID_OUT_OF_RANGE);
        }
#endif
    }
    return ret;
}
#define FIM_STOP_SEC_CODE
#include "FiM_MemMap.h"

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
#define FIM_START_SEC_CODE
#include "FiM_MemMap.h"
FUNC(Std_ReturnType, FIM_CODE)
FiM_SetFunctionAvailable(FiM_FunctionIdType FID, boolean Availability)
{
    Std_ReturnType ret = E_NOT_OK;
    uint16 fidIndex = 0xffffu;
#if (STD_ON == FIM_DEV_ERROR_DETECT)
    if (FIM_INIT_STEP_DEM != FiM_InitStep)
    {
        FIM_DET_REPORT(FIM_SERVICE_ID_SETFUNCTIONAVAILABLE, FIM_E_UNINIT);
    }
    else
#endif
    {
        if (TRUE == FiM_FindFidIndex(FID, &fidIndex))
        {
            ret = E_OK;
            SchM_Enter_FiM_ExclusiveArea();
            /* PRQA S 4304++ */ /* MISRA Rule 10.5 */
            FiM_FidStu[fidIndex].ctrlStu = (FiM_FidStu[fidIndex].ctrlStu & ((uint8)(~(1u << FIM_FID_AVAILABLE_BIT))))
                                           | (uint8)(((uint8)Availability) << FIM_FID_AVAILABLE_BIT);
            /* PRQA S 4304-- */ /* MISRA Rule 10.5 */
            SchM_Exit_FiM_ExclusiveArea();
        }
#if (STD_ON == FIM_DEV_ERROR_DETECT)
        else
        {
            FIM_DET_REPORT(FIM_SERVICE_ID_SETFUNCTIONAVAILABLE, FIM_E_FID_OUT_OF_RANGE);
        }
#endif
    }
    return ret;
}
#define FIM_STOP_SEC_CODE
#include "FiM_MemMap.h"
#endif /* STD_ON == FIM_AVAILABILITY_SUPPORT */
/* Interface Dem <----> FiM */
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
#define FIM_START_SEC_CODE
#include "FiM_MemMap.h"
FUNC(void, FIM_CODE)
FiM_DemTriggerOnEventStatus(
    Dem_EventIdType EventId,
    Dem_UdsStatusByteType EventStatusByteOld,
    Dem_UdsStatusByteType EventStatusByteNew)
{
#if (STD_ON == FIM_DEV_ERROR_DETECT)
    /*fim notified by dem about event status change */
    if (FIM_INIT_STEP_DEM != FiM_InitStep)
    {
        FIM_DET_REPORT(FIM_SERVICE_ID_DEMTRIGGERONEVENTSTATUS, FIM_E_UNINIT);
    }
    else if ((FIM_EVENT_ID_INVALID == EventId) || (FIM_MAX_DEM_EVENT_ID < EventId))
    {
        FIM_DET_REPORT(FIM_SERVICE_ID_DEMTRIGGERONEVENTSTATUS, FIM_E_EVENTID_OUT_OF_RANGE);
    }
    else
#endif
    {
        if (FIM_GETEVENTSTATUS(EventStatusByteNew) != FIM_GETEVENTSTATUS(EventStatusByteOld))
        {
            FiM_TrigEventStatusHandle(EventId, EventStatusByteNew);
        }
    }
}
#define FIM_STOP_SEC_CODE
#include "FiM_MemMap.h"
#endif /* STD_ON == FIM_EVENT_UPDATE_TRIGGERED_BY_DEM */

#if (STD_ON == FIM_EVENT_UPDATE_TRIGGERED_BY_DEM)
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
#define FIM_START_SEC_CODE
#include "FiM_MemMap.h"
FUNC(void, FIM_CODE)
FiM_DemTriggerOnComponentStatus(Dem_ComponentIdType ComponentId, boolean ComponentFailedStatus)
{
    uint16 inhIndex = 0xffffu;
    uint16 index;
#if (STD_ON == FIM_DEV_ERROR_DETECT)
    if (FIM_INIT_STEP_DEM != FiM_InitStep)
    {
        FIM_DET_REPORT(FIM_SERVICE_ID_DEMTRIGGERONCOMPONENTSTATUS, FIM_E_UNINIT);
    }
    else
#endif
    {
        if (TRUE == FiM_FindComponetInhIndex(ComponentId, &inhIndex))
        {
            for (index = 0; index < FiM_CfgPtr->InhCfgPtr[inhIndex].InhComponentCnt; index++)
            {
                if ((FiM_CfgPtr->InhCfgPtr[inhIndex].InhComponentPtr[index].ComponentId == ComponentId)
#if (FIM_NUMBER_OF_COMPONENT > 0)
                    && (FiM_ComponentStu[index].ComponentStu != ComponentFailedStatus)
#endif
                )
                {
#if (FIM_NUMBER_OF_COMPONENT > 0)
                    FiM_ComponentStu[index].ComponentStu = ComponentFailedStatus;
#endif
                    FiM_FidPerssionHandle(FiM_CfgPtr->InhCfgPtr[inhIndex].InhFunctionIdRef, ComponentFailedStatus);
                }
            }
        }
    }
}
#define FIM_STOP_SEC_CODE
#include "FiM_MemMap.h"
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
#define FIM_START_SEC_CODE
#include "FiM_MemMap.h"
FUNC(void, FIM_CODE)
FiM_DemInit(void)
{
    uint16 loop;
    uint16 inhIndex;
    const FiM_InhibitionCfgType* inhCfgPtr;
#if (STD_ON == FIM_DEV_ERROR_DETECT)
    if (FIM_INIT_STEP_LOCAL > FiM_InitStep)
    {
        FIM_DET_REPORT(FIM_SERVICE_ID_DEMINIT, FIM_E_UNINIT);
    }
    else
#endif
    {
        /* update fid permission state */
        /* re-compute the permission state for all  fids */
        /*  Fim_DemInit shall only be used during first Dem_Init after
        system start-up */
        for (inhIndex = 0u; (inhIndex < FiM_CfgPtr->InhibitionCfgCnt); inhIndex++)
        {
            inhCfgPtr = &FiM_CfgPtr->InhCfgPtr[inhIndex];
            /* Synchronous access the eventid status via the api
            Dem_GetEventStatus */
            /* first search FiMInhEventRef */
            for (loop = 0x0u; loop < inhCfgPtr->InhEventCnt; loop++)
            {
                FiM_NormalEventHandle(inhCfgPtr->InhEventRefPtr[loop], inhIndex);
            }
#if (0u < FIM_MAX_SUM_EVENTS)
            /* next search summary */
            for (loop = 0x0u; loop < inhCfgPtr->InhSummaryCnt; loop++)
            {
                FiM_SummaryEventHandle(inhCfgPtr->InhSummaryRefPtr[loop], inhIndex);
            }
#endif
            /* end need search componet */
            for (loop = 0x0u; loop < inhCfgPtr->InhComponentCnt; loop++)
            {
                FiM_ComponentHandle(loop, inhCfgPtr->InhComponentPtr[loop].ComponentId, inhIndex);
            }
        }
        FiM_InitStep = FIM_INIT_STEP_DEM;
    }
}
#define FIM_STOP_SEC_CODE
#include "FiM_MemMap.h"
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
#define FIM_START_SEC_CODE
#include "FiM_MemMap.h"
FUNC(void, FIM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
FiM_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, FIM_APPL_DATA) versioninfo)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
#if (STD_ON == FIM_DEV_ERROR_DETECT)
    if (NULL_PTR == versioninfo)
    {
        FIM_DET_REPORT(FIM_SERVICE_ID_GETVERSIONINFO, FIM_E_PARAM_POINTER);
    }
    else
#endif
    {
        versioninfo->moduleID = FIM_MODULE_ID;
        versioninfo->vendorID = FIM_VENDOR_ID;
        versioninfo->sw_major_version = FIM_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = FIM_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = FIM_SW_PATCH_VERSION;
    }
}
#define FIM_STOP_SEC_CODE
#include "FiM_MemMap.h"
#endif /* STD_ON == FIM_VERSION_INFO_API */

/* Call-back notifications */

/* Scheduled functions */
/**
 * mainfunction
 * Service ID: 0x05
 * Sync/Async: Synchronous
 */
#define FIM_START_SEC_CODE
#include "FiM_MemMap.h"
FUNC(void, FIM_CODE)
FiM_MainFunction(void)
{
#if (STD_OFF == FIM_EVENT_UPDATE_TRIGGERED_BY_DEM)
    /* cyclic manner poll event status */
    uint16 inhIndex;
    const FiM_InhibitionCfgType* inhCfgPtr;
    uint16 loop;
    if (FIM_INIT_STEP_DEM == FiM_InitStep)
    {
        /* update fid permission state */
        /*  Fim_DemInit shall only be used during first Dem_Init after
        system start-up */
        for (inhIndex = 0u; (inhIndex < FiM_CfgPtr->InhibitionCfgCnt); inhIndex++)
        {
            inhCfgPtr = &FiM_CfgPtr->InhCfgPtr[inhIndex];
            /* first search FiMInhEventRef */
            for (loop = 0x0u; loop < inhCfgPtr->InhEventCnt; loop++)
            {
                FiM_NormalEventHandle(inhCfgPtr->InhEventRefPtr[loop], inhIndex);
            }
#if (0u < FIM_MAX_SUM_EVENTS)
            /* next search summary */
            for (loop = 0x0u; loop < inhCfgPtr->InhSummaryCnt; loop++)
            {
                FiM_SummaryEventHandle(inhCfgPtr->InhSummaryRefPtr[loop], inhIndex);
            }
#endif /* 0u < FIM_MAX_SUM_EVENTS */
            /* next search component */
            for (loop = 0x0u; loop < inhCfgPtr->InhComponentCnt; loop++)
            {
                FiM_ComponentHandle(loop, inhCfgPtr->InhComponentPtr[loop].ComponentId, inhIndex);
            }
        }
    }
#endif /* STD_OFF == FIM_EVENT_UPDATE_TRIGGERED_BY_DEM */
}
#define FIM_STOP_SEC_CODE
#include "FiM_MemMap.h"
/******************************************************************************
**                      Private Function                                     **
******************************************************************************/
#define FIM_START_SEC_CODE
#include "FiM_MemMap.h"

/* find fid value in post build configuration FunctionIdPtr index */
FIM_LOCAL FUNC(boolean, FIM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    FiM_FindFidIndex(FiM_FunctionIdType FID, P2VAR(uint16, AUTOMATIC, FIM_APPL_DATA) fidIndexPtr)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint16 index;
    boolean findFlg = FALSE;
    /* PRQA S 2877++ */ /* MISRA Dir 4.1 */
    for (index = 0u; (FALSE == findFlg) && (index < FIM_NUMBER_OF_FID); index++)
    /* PRQA S 2877-- */ /* MISRA Dir 4.1 */
    {
        if (FID == FiM_CfgPtr->FunctionIdPtr[index])
        {
            findFlg = TRUE;
            *fidIndexPtr = index;
        }
    }
    return findFlg;
}

#if (STD_ON == FIM_EVENT_UPDATE_TRIGGERED_BY_DEM)
/* find componentId in inhibition configuration index */
FIM_LOCAL FUNC(boolean, FIM_CODE)
    FiM_FindComponetInhIndex(Dem_ComponentIdType ComponentId, P2VAR(uint16, AUTOMATIC, FIM_APPL_DATA) inhIndex)
{
    uint16 index;
    boolean findFlg = FALSE;
    uint16 comIndex;
    const FiM_InhibitionCfgType* inhCfgPtr;
    for (index = 0u; (FALSE == findFlg) && (index < FIM_MAX_INH_CFGS); index++)
    {
        inhCfgPtr = &FiM_CfgPtr->InhCfgPtr[index];
        if ((NULL_PTR != inhCfgPtr) && (NULL_PTR != inhCfgPtr->InhComponentPtr))
        {
            for (comIndex = 0x0u; (FALSE == findFlg) && (comIndex < inhCfgPtr->InhComponentCnt); comIndex++)
            {
                if (ComponentId == inhCfgPtr->InhComponentPtr[comIndex].ComponentId)
                {
                    *inhIndex = index;
                    findFlg = TRUE;
                }
            }
        }
    }
    return findFlg;
}
#endif /* STD_ON == FIM_EVENT_UPDATE_TRIGGERED_BY_DEM */

/* poll mode get event status in FiMInhEventRef */
FIM_LOCAL FUNC(void, FIM_CODE) FiM_NormalEventHandle(uint16 eventIndex, uint16 inhIndex)
{
    Std_ReturnType ret;
    const FiM_InhibitionCfgType* inhCfgPtr = &FiM_CfgPtr->InhCfgPtr[inhIndex];
    boolean inhibitionFlag, changed;
    Dem_UdsStatusByteType stu = 0x0u;
    if (eventIndex < FiM_CfgPtr->EventIdCnt)
    {
        ret = Dem_GetEventStatus(FiM_CfgPtr->EventIdPtr[eventIndex], &stu);
        stu = FIM_GETEVENTSTATUS(stu);
        if ((E_OK == ret) && (FiM_EventStu[eventIndex].eventStu != stu))
        {
            inhibitionFlag = FiM_EventsStatusDiscern(inhCfgPtr, FiM_EventStu[eventIndex].eventStu, stu, &changed);
            if (changed == TRUE)
            {
                FiM_EventStu[eventIndex].eventStu = stu;
                FiM_FidPerssionHandle(FiM_CfgPtr->InhCfgPtr[inhIndex].InhFunctionIdRef, inhibitionFlag);
            }
        }
        /* dem return E_NOT_OK,fim shall not consider this event */
    }
}

#if (0u < FIM_MAX_SUM_EVENTS)
/* poll mode handle summary event in FiMInhSumRef */
FIM_LOCAL FUNC(void, FIM_CODE) FiM_SummaryEventHandle(uint16 summaryIndex, uint16 inhIndex)
{
    uint16 index;
    const FiM_InhibitionCfgType* inhCfgPtr = &FiM_CfgPtr->InhCfgPtr[inhIndex];
    const FiMSummaryEventType* sumCfgPtr = &FiM_CfgPtr->SummaryEventPtr[summaryIndex];
    uint16 eventIndex;
    Std_ReturnType ret;
    Dem_UdsStatusByteType stu = 0x0u;
    boolean inhibitionFlag, changed;
    for (index = 0x0u; (index < sumCfgPtr->InputEventCnt); index++)
    {
        eventIndex = sumCfgPtr->InputEventRefPtr[index];
        ret = Dem_GetEventStatus(FiM_CfgPtr->EventIdPtr[eventIndex], &stu);
        stu = FIM_GETEVENTSTATUS(stu);
        if ((E_OK == ret) && (FiM_EventStu[eventIndex].eventStu != stu))
        {
            inhibitionFlag = FiM_EventsStatusDiscern(inhCfgPtr, FiM_EventStu[eventIndex].eventStu, stu, &changed);
            if (changed == TRUE)
            {
                FiM_EventStu[eventIndex].eventStu = stu;
                /* any of the specific events decide this summary event */
                FiM_FidPerssionHandle(FiM_CfgPtr->InhCfgPtr[inhIndex].InhFunctionIdRef, inhibitionFlag);
            }
        }
    }
}
#endif /* 0u < FIM_MAX_SUM_EVENTS */

/* poll mode get component faile statue in FiMInhComponentRef */
FIM_LOCAL FUNC(void, FIM_CODE)
    FiM_ComponentHandle(uint16 componentIndex, Dem_ComponentIdType componentId, uint16 inhIndex)
{
    boolean componentFailedStatus;
    Std_ReturnType ret = Dem_GetComponentFailed(componentId, &componentFailedStatus);
    if ((E_OK == ret)
#if (FIM_NUMBER_OF_COMPONENT > 0)
        && (FiM_ComponentStu[componentIndex].ComponentStu != componentFailedStatus)
#endif
    )
    {
#if (FIM_NUMBER_OF_COMPONENT > 0)
        FiM_ComponentStu[componentIndex].ComponentStu = componentFailedStatus;
#endif
        FiM_FidPerssionHandle(FiM_CfgPtr->InhCfgPtr[inhIndex].InhFunctionIdRef, componentFailedStatus);
    }
#if (FIM_NUMBER_OF_COMPONENT == 0)
    FIM_UNUSED(componentIndex);
#endif
}

/* fid perssion status handle */
FIM_LOCAL FUNC(void, FIM_CODE) FiM_FidPerssionHandle(uint16 fidIndex, boolean inhibstu)
{
    SchM_Enter_FiM_ExclusiveArea();
    if (TRUE == inhibstu)
    {
        if (0x0u == FiM_FidStu[fidIndex].inhCouter)
        {
            /* PRQA S 4340,4394,4404++ */ /* MISRA Rule 10.5,Rule 10.8,Rule 10.3 */
            FiM_FidStu[fidIndex].ctrlStu =
                (boolean)(FiM_FidStu[fidIndex].ctrlStu & ((uint8)(~(1u << FIM_FID_PERMISSION_STU_BIT))));
            /* PRQA S 4340,4394,4404-- */ /* MISRA Rule 10.5,Rule 10.8,Rule 10.3 */
        }
        if (FIM_INHIBITION_COUNTER_MAX > FiM_FidStu[fidIndex].inhCouter)
        {
            FiM_FidStu[fidIndex].inhCouter++;
        }
    }
    else
    {
        if (0u < FiM_FidStu[fidIndex].inhCouter)
        {
            FiM_FidStu[fidIndex].inhCouter--;
        }
        if (0u == FiM_FidStu[fidIndex].inhCouter)
        {
            /* PRQA S 4340,4394,4404++ */ /* MISRA Rule 10.5,Rule 10.8,Rule 10.3 */
            FiM_FidStu[fidIndex].ctrlStu =
                (boolean)((FiM_FidStu[fidIndex].ctrlStu & ((uint8)(~(1u << FIM_FID_PERMISSION_STU_BIT))))
                          | (uint8)(1u << FIM_FID_PERMISSION_STU_BIT));
            /* PRQA S 4340,4394,4404-- */ /* MISRA Rule 10.5,Rule 10.8,Rule 10.3 */
        }
    }
    SchM_Exit_FiM_ExclusiveArea();
}

#if (STD_ON == FIM_EVENT_UPDATE_TRIGGERED_BY_DEM)
/* trigger mode handle event link fid status */
FIM_LOCAL FUNC(void, FIM_CODE) FiM_TrigEventStatusHandle(Dem_EventIdType EventId, Dem_UdsStatusByteType EventStatus)
{
    uint16 inhIndex, loop, eventIndex;
    const FiM_InhibitionCfgType* inhCfgPtr;
    boolean inhibitionFlag, changed;
    Dem_UdsStatusByteType stu = FIM_GETEVENTSTATUS(EventStatus);
#if (0u < FIM_MAX_SUM_EVENTS)
    const FiMSummaryEventType* sumCfgPtr;
    uint16 sumIndex;
#endif
    for (inhIndex = 0x0u; inhIndex < FiM_CfgPtr->InhibitionCfgCnt; inhIndex++)
    {
        inhCfgPtr = &FiM_CfgPtr->InhCfgPtr[inhIndex];
        /* normal event handle */
        for (loop = 0u; loop < inhCfgPtr->InhEventCnt; loop++)
        {
            eventIndex = inhCfgPtr->InhEventRefPtr[loop];
            if (EventId == FiM_CfgPtr->EventIdPtr[eventIndex])
            {
                inhibitionFlag = FiM_EventsStatusDiscern(inhCfgPtr, FiM_EventStu[eventIndex].eventStu, stu, &changed);
                if (changed == TRUE)
                {
                    FiM_FidPerssionHandle(FiM_CfgPtr->InhCfgPtr[inhIndex].InhFunctionIdRef, inhibitionFlag);
                    FiM_EventStu[eventIndex].eventStu = stu;
                }
            }
        }
#if (0u < FIM_MAX_SUM_EVENTS)
        for (loop = 0u; loop < inhCfgPtr->InhSummaryCnt; loop++)
        {
            sumCfgPtr = &FiM_CfgPtr->SummaryEventPtr[inhCfgPtr->InhSummaryRefPtr[loop]];
            for (sumIndex = 0u; sumIndex < sumCfgPtr->InputEventCnt; sumIndex++)
            {
                eventIndex = sumCfgPtr->InputEventRefPtr[sumIndex];
                if (EventId == FiM_CfgPtr->EventIdPtr[eventIndex])
                {
                    inhibitionFlag =
                        FiM_EventsStatusDiscern(inhCfgPtr, FiM_EventStu[eventIndex].eventStu, stu, &changed);
                    if (changed == TRUE)
                    {
                        FiM_FidPerssionHandle(FiM_CfgPtr->InhCfgPtr[inhIndex].InhFunctionIdRef, inhibitionFlag);
                        FiM_EventStu[eventIndex].eventStu = stu;
                    }
                }
            }
        }
#endif /* 0u < FIM_MAX_SUM_EVENTS */
        /* summary event handle */
    }
}
#endif /* STD_ON == FIM_EVENT_UPDATE_TRIGGERED_BY_DEM */

/* event status and inhibition mask process */
FIM_LOCAL FUNC(boolean, FIM_CODE) FiM_EventsStatusDiscern(
    P2CONST(FiM_InhibitionCfgType, AUTOMATIC, FIM_APPL_DATA) inhCfgPtr,
    Dem_UdsStatusByteType OldEventStatus,
    Dem_UdsStatusByteType NewEventStatus,
    boolean* Changed)
{
    boolean inhibitionFlg = FALSE;
    *Changed = FALSE;
    switch (inhCfgPtr->InhInhibitionMask)
    {
    case FIM_LAST_FAILED:
        if (0x0u != (NewEventStatus & FIM_DEM_UDS_STU_TF))
        {
            inhibitionFlg = TRUE;
        }
        if (((OldEventStatus ^ NewEventStatus) & FIM_DEM_UDS_STU_TF) != 0x0u)
        {
            *Changed = TRUE;
        }
        break;
    case FIM_NOT_TESTED:
        if (0x0u != (NewEventStatus & FIM_DEM_UDS_STU_TNCTOC))
        {
            inhibitionFlg = TRUE;
        }
        if (((OldEventStatus ^ NewEventStatus) & FIM_DEM_UDS_STU_TNCTOC) != 0x0u)
        {
            *Changed = TRUE;
        }
        break;
    case FIM_TESTED:
        if (0x0u == (NewEventStatus & FIM_DEM_UDS_STU_TNCTOC))
        {
            inhibitionFlg = TRUE;
        }
        if (((OldEventStatus ^ NewEventStatus) & FIM_DEM_UDS_STU_TNCTOC) != 0x0u)
        {
            *Changed = TRUE;
        }
        break;
    case FIM_TESTED_AND_FAILED:
        if ((0x0u != (NewEventStatus & FIM_DEM_UDS_STU_TF)) && (0x0u == (NewEventStatus & FIM_DEM_UDS_STU_TNCTOC)))
        {
            inhibitionFlg = TRUE;
        }
        if ((OldEventStatus ^ NewEventStatus) != 0x0u)
        {
            *Changed = TRUE;
        }
        break;
    default:
        /*idle*/
        break;
    }
    return inhibitionFlg;
}

#define FIM_STOP_SEC_CODE
#include "FiM_MemMap.h"
