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
**  FILENAME    : EcuM_StartUp.c                                              **
**                                                                            **
**  Created on  :                                                             **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation code about start-up phase.                   **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*  <VERSION>    <DATE>      <AUTHOR>        <REVISION LOG>
 *  V1.0.0       2020-06-28  qinchun.yang    R19_11 EcuM initial version.
 *  V2.0.0       2021-04-08  qinchun.yang    R1911 stable version.
 */

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "EcuM_Internal.h"
#include "Rte_Main.h"
/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/*Internal variable init.*/
static FUNC(void, ECUM_CODE) EcuM_InternalInit(void);

/**
 * Activities sequence before OS started.
 */
static FUNC(void, ECUM_CODE) EcuM_StartPreOS(void);

/*Map reset reason to wake up source*/
static FUNC(void, ECUM_CODE) EcuM_MapReset2WakeupSource(Mcu_ResetType resetReason);
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
#define ECUM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EcuM_MemMap.h"
VAR(EcuM_WakeupSourceType, ECUM_CLEARED) EcuMWksPending;
VAR(EcuM_WakeupSourceType, ECUM_CLEARED) EcuMWksValidated;
VAR(EcuM_WakeupSourceType, ECUM_CLEARED) EcuMWksExpired;
#define ECUM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EcuM_MemMap.h"
/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/
#define ECUM_START_SEC_VAR_CLEARED_SHARE_UNSPECIFIED
#include "EcuM_MemMap.h"
/*Pointer variable to store the determined PB configuration data*/
P2CONST(EcuM_ConfigType, ECUM_VAR_CLEARED_PTR, ECUM_APPL_CONST) EcuM_ConfigPtr;
#define ECUM_STOP_SEC_VAR_CLEARED_SHARE_UNSPECIFIED
#include "EcuM_MemMap.h"

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/**
 * Initializes the ECU state manager and carries out the startup procedure.
 * The function will never return (it calls StartOS).
 * Service ID[hex]: 0x01
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, ECUM_CODE)
EcuM_Init(void)
{
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    /* Init Check */
    if ((boolean)TRUE == EcuM_IsInit)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_INIT, ECUM_E_REINIT);
    }
    else
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    {
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
        EcuM_IsInit = TRUE;
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/

        /*Initialize EcuM module internal state and runtime variables*/
        EcuM_InternalInit();

        /*start activities sequence before start OS*/
        EcuM_StartPreOS();

#if (ECUM_ALARM_CLOCK_PRESENT == STD_ON)
        /*Cancelled alarms.*/
        EcuM_CancellAlarms();
#endif /*ECUM_ALARM_CLOCK_PRESENT == STD_ON*/

        /*Not return.jump to OS context.*/
        StartOS(EcuM_ConfigPtr->defaultAppMode);
    }
}

/**
 * This function implements the STARTUP II state.
 * Service ID[hex]: 0x1a
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, ECUM_CODE)
EcuM_StartupTwo(void)
{
#if (ECUM_MAX_MCU_CORE_NUM > 1)
    CoreIdType coreId;
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    /* Init Check */
    if ((boolean)FALSE == EcuM_IsInit)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_STARTUPTWO, ECUM_E_UNINIT);
    }
    else
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    {
#if (ECUM_MAX_MCU_CORE_NUM > 1)
        coreId = GetCoreID();
        ECUM_AVOID_WARNNING(coreId);
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
        /*start BSW scheduler*/
        SchM_Start();

        /*Initialize BSWM*/
        BswM_Init((EcuM_ConfigPtr->modulePBCfg->bswMPbCfg));

        /*Initialize BSW Scheduler*/
        SchM_Init(NULL_PTR);

        /*Start periodical events for BSW/SWCs*/
        SchM_StartTiming();

        /*initialize ECUM state and notify BSWM*/
        EcuMRunData.State = ECUM_STATE_STARTUP;
        BswM_EcuM_CurrentState(ECUM_STATE_STARTUP);

        if ((*EcuMRunData.Wks.Validated) != 0u)
        {
            BswM_EcuM_CurrentWakeup((*EcuMRunData.Wks.Validated), ECUM_WKSTATUS_VALIDATED);
        }
    }
}
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/*Internal variable init.*/
static FUNC(void, ECUM_CODE) EcuM_InternalInit(void)
{
    P2VAR(EcuM_RunTimeType, AUTOMATIC, ECUM_VAR) pRt = &EcuMRunData; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
#if (ECUM_ALARM_CLOCK_PRESENT == STD_ON)
    EcuM_CancellAlarms();
    pRt->GlobalClock = 0u;
#endif /*ECUM_ALARM_CLOCK_PRESENT == STD_ON*/

    pRt->Wks.Pending = &EcuMWksPending;
    pRt->Wks.Expired = &EcuMWksExpired;
    pRt->Wks.Validated = &EcuMWksValidated;
#if (ECUM_MODE_HANDING == STD_ON)
    EcuM_RteState = RTE_TRANSITION_EcuM_Mode;
#endif /*ECUM_MODE_HANDING == STD_ON*/
}

/**
 * Activities sequence before OS started.
 */
static FUNC(void, ECUM_CODE) EcuM_StartPreOS(void)
{
    Mcu_ResetType resetReason;
#if (ECUM_MAX_MCU_CORE_NUM > 1)
    CoreIdType coreId;
    CoreIdType coreNum;
    StatusType status;

    /*This ECUM instance is running on master or slaves*/
    /*get the core running this ECUM instance*/
    coreId = GetCoreID();
    if (ECUM_MASTER_CORE_ID == coreId)
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
    {
#if (ECUM_SET_PROGRAMMABLE_INTERRUPTS == STD_ON)
        /*On ECUs with programmable interrupt priorities, these priorities must be
         *set before the OS is started. This activity is optional depend on the ECU.*/
        EcuM_AL_SetProgrammableInterrupts();
#endif /*ECUM_SET_PROGRAMMABLE_INTERRUPTS == STD_ON*/
#if (ECUM_DRIVER_INIT_LIST_ZERO == STD_ON)
        /*Init block 0. This callout may only initialize BSW modules that do not use
         *post-build configuration parameters. The callout may not only contain
         *driver initialization but also any kind of pre-OS, low level initialization code.
         *This activity is optional depend on the ECU.*/
        EcuM_AL_DriverInitZero();
#endif /*ECUM_DRIVER_INIT_LIST_ZERO == STD_ON*/
        /*This callout is expected to return a pointer to a fully initialized
         *EcuM_ConfigType structure containing the post-build configuration data
         *for the ECU Manager module and all other BSW modules.*/
        EcuM_ConfigPtr = EcuM_DeterminePbConfiguration();

        /*Check consistency of configuration data.*/
        if ((EcuM_ConfigPtr == NULL_PTR) || (EcuM_ConfigConsistencyHash != EcuM_ConfigPtr->configConsistencyHash))
        {
            /*If check fails the EcuM_ErrorHook is called.*/
            EcuM_ErrorHook(ECUM_E_CONFIGURATION_DATA_INCONSISTENT);
        }
        else
        {
#if (ECUM_DRIVER_INIT_LIST_ONE == STD_ON)
            /*Init block I.The callout may not only contain driver initialization but
             *any kind of pre-OS, low level initialization code.This activity is optional
             *depend on the ECU.*/
            EcuM_AL_DriverInitOne();
#endif /*ECUM_DRIVER_INIT_LIST_ONE == STD_ON*/

            /*Get reset reason. The reset reason is derived from a call to
             *Mcu_GetResetReason and the mapping defined via the EcuMWakeupSource
             *configuration containers.*/
            resetReason = Mcu_GetResetReason();
            EcuM_MapReset2WakeupSource(resetReason);

            /*Select default shutdown target*/
            (void)EcuM_SelectShutdownTarget(EcuM_ConfigPtr->defaultShutdownTgt, EcuM_ConfigPtr->defaultShutdownMode);

#if (ECUM_RESET_LOOP_DETECTION == STD_ON)
            EcuM_LoopDetection();
#endif
#if (ECUM_MAX_MCU_CORE_NUM > 1)
            coreNum = ECUM_MAX_MCU_CORE_NUM - 1;
            for (coreId = 0; coreId < coreNum; coreId++)
            {
                StartCore(EcuM_SlaveCoreIds[coreId], &status);
            }
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
        }
    }
}

/*Map reset reason to wake up source*/
static FUNC(void, ECUM_CODE) EcuM_MapReset2WakeupSource(Mcu_ResetType resetReason)
{
    uint32 wkSrcIdx;
    uint32 rstRasaonIdx;

    for (wkSrcIdx = 0u; wkSrcIdx < (uint8)ECUM_MAX_WAKE_UP_SOURCE_NUM; wkSrcIdx++)
    {
        for (rstRasaonIdx = 0; rstRasaonIdx < EcuM_WkSourceCfgs[wkSrcIdx].resetResasonNum; rstRasaonIdx++)
        {
            if (EcuM_WkSourceCfgs[wkSrcIdx].resetResasonPtr[rstRasaonIdx] == resetReason)
            {
                EcuMWksValidated = (EcuM_WakeupSourceType)(EcuMWksValidated) | (EcuM_WkSourceCfgs[wkSrcIdx].wkSource);
                /* PRQA S 2001 ++ */ /* MISRA Rule 15.1 */
                goto EcuM_Exit_MapReset;
                /* PRQA S 2001 -- */ /* MISRA Rule 15.1 */
            }
        }
    }
EcuM_Exit_MapReset:
    return;
}
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"
