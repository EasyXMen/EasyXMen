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
 **  @file               : EcuM_StartUp.c
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : ECU state management
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "EcuM_Internal.h"
#include "Rte_Main.h"
#include "BswM.h"
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/**
 * @brief Internal initialization function for EcuM module.
 * @param[in] pRt : Pointer to the runtime configuration structure.
 * @details This function initializes internal variables and structures for the EcuM module.
 *          It performs the following actions:
 *          - Initializes the global clock if the alarm clock feature is enabled.
 *          - Sets pointers to the pending, expired, and validated wake-up sources.
 *          - Initializes the timer states for all wake-up sources.
 *          - Sets the initial state for mode handling if the mode handling feature is enabled.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-64797
 */
ECUM_LOCAL void EcuM_InternalInit(EcuM_RunTimeLcType* pRt);

/**
 * @brief Pre-OS initialization function for EcuM module.
 * @details This function performs various initialization steps before the OS starts.
 *          It includes the following activities:
 *          - Setting programmable interrupt priorities if enabled.
 *          - Initializing drivers and low-level components in block 0 if enabled.
 *          - Determining and validating the post-build configuration data.
 *          - Initializing drivers and low-level components in block 1 if enabled.
 *          - Initializing the core EcuM module.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-64874
 */
ECUM_LOCAL void EcuM_StartPreOS(void);

/**
 * @brief Initializes the core EcuM module.
 * @details This function initializes the core EcuM module and performs the following tasks:
 *          - Gets the reset reason and maps it to a wake-up source.
 *          - Selects the default shutdown target.
 *          - Detects reset loops if enabled.
 *          - Starts slave cores if the system has multiple cores.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-69024
 */
ECUM_LOCAL void EcuM_InitializeCore(void);

/**
 * @brief Initializes the EcuM module and notifies the BSW manager.
 * @details This function initializes the EcuM module by setting the initial state and notifying the BSW manager.
 *          It also checks if the validated wake-up source is valid and notifies the BSW manager accordingly.
 * @param[in] tempRunData : Pointer to the runtime data structure containing the current state and wake-up information.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-69025
 */
ECUM_LOCAL void EcuM_InitAndNotifyBswM(EcuM_RunTimeLcType* tempRunData);

/**
 * @brief Maps the reset reason to the corresponding wake-up source.
 * @details This function maps the given reset reason to the appropriate wake-up source by checking the configuration.
 *          If a valid wake-up source is found, it updates the `EcuMWksValidated` variable with the corresponding
 * wake-up source. If no valid wake-up source is found, it sets the `EcuMWksValidated` to `ECUM_WKSOURCE_RESET`.
 * @param[in] resetReason : The reset reason to be mapped to a wake-up source.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-64800
 */
ECUM_LOCAL void EcuM_MapReset2WakeupSource(Mcu_ResetType resetReason);
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"
/* ============================================ internal data definitions =========================================== */
#define ECUM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EcuM_MemMap.h"
/* Wake-up sources that are pending */
EcuM_WakeupSourceType EcuMWksPending;
/* Wake-up sources that have been validated */
EcuM_WakeupSourceType EcuMWksValidated;
/* Wake-up sources that have expired */
EcuM_WakeupSourceType EcuMWksExpired;
#define ECUM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EcuM_MemMap.h"
/* ============================================ external data definitions =========================================== */
#define ECUM_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
#include "EcuM_MemMap.h"
/*Pointer variable to store the determined PB configuration data*/
const EcuM_ConfigType* EcuM_ConfigPtr;
#define ECUM_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
#include "EcuM_MemMap.h"

/* ========================================== external function definitions ========================================= */
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"

/* PRQA S 1503, 2814 ++ */ /* VL_QAC_NoUsedApi, VL_QAC_MultiReturn */
/**
 * Initializes the ECU Manager module.
 *
 * This function initializes the runtime data and internal state of the ECU Manager module.
 * It performs pre-OS start activities and cancels any active alarms if the alarm clock is present.
 * The function then starts the operating system with the default application mode.
 * If development error detection is enabled, it checks for initialization errors and reports them.
 */
void EcuM_Init(void)
{
    EcuM_RunTimeLcType* pRt;
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    uint8 detErrorId = ECUM_E_OK;
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/

    pRt = EcuM_GetLcRunDataPtr();
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    /* Init Check */
    if (NULL_PTR == pRt)
    {
        detErrorId = ECUM_E_INIT_FAILED;
    }
    else if (pRt->EcuM_IsInit)
    {
        detErrorId = ECUM_E_REINIT;
    }
    else
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    {
        pRt->EcuM_IsInit = TRUE;
        /*Initialize EcuM module internal state and runtime variables*/
        EcuM_InternalInit(pRt);

        /*start activities sequence before start OS*/
        EcuM_StartPreOS();

#if (ECUM_ALARM_CLOCK_PRESENT == STD_ON)
        /*Cancelled alarms.*/
        EcuM_CancellAlarms();
#endif /*ECUM_ALARM_CLOCK_PRESENT == STD_ON*/

        /*Not return.jump to OS context.*/
        StartOS(EcuM_ConfigPtr->defaultAppMode);
    }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    if (ECUM_E_OK != detErrorId)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_INIT, detErrorId);
    }
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON */
}
/* PRQA S 1503, 2814 -- */

/* PRQA S 1503, 1532, 2814 ++ */ /* VL_QAC_NoUsedApi, VL_QAC_OneFunRef, VL_QAC_MultiReturn */
/**
 * Performs the second stage of the ECU Manager startup process.
 *
 * This function checks if the ECU Manager is initialized and starts the BSW scheduler.
 * It initializes the BSW Manager and the BSW Scheduler, and starts periodic events for BSW/SWCs.
 * It also notifies the BSW Manager of certain states.
 * If the ECU Manager is not initialized, it reports an error if development error detection is enabled.
 */
void EcuM_StartupTwo(void)
{
    EcuM_RunTimeLcType* tempRunData;
    uint8               detErrorId = ECUM_E_OK;

    /* Init Check */
    if (EcuM_isInitState(&tempRunData))
    {
#if (ECUM_MAX_MCU_CORE_NUM > 1)
        CoreIdType coreId;
        coreId = tempRunData->coreId;
        (void)(coreId);
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
        /*start BSW scheduler*/
        SchM_Start();

        /*Initialize BSWM*/
        /* PRQA S 0316 ++ */ /* VL_QAC_0316 */
        BswM_Init((const BswM_ConfigType*)(EcuM_ConfigPtr->modulePBCfg->bswMPbCfg));
        /*Initialize BSW Scheduler*/
        SchM_Init((const SchM_ConfigType*)(EcuM_ConfigPtr->modulePBCfg->schMPbCfg));
        /* PRQA S 0316 -- */

        /*Start periodical events for BSW/SWCs*/
        SchM_StartTiming();

        /* Notify BswM of some states */
        EcuM_InitAndNotifyBswM(tempRunData);
    }
    else
    {
        detErrorId = ECUM_E_UNINIT;
    }

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    if (ECUM_E_OK != detErrorId)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_STARTUPTWO, detErrorId);
    }
#else
    (void)detErrorId;
#endif /* ECUM_DEV_ERROR_DETECT == STD_ON */
}
/* PRQA S 1503, 1532, 2814 -- */
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"
/* ========================================== internal function definitions ========================================= */
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"

/* PRQA S 2814 ++ */ /* VL_QAC_MultiReturn */
/**
 * Initializes the ECU Manager state and notifies the BSW Manager.
 *
 * This function sets the ECU Manager state to startup and notifies the BSW Manager of the current state.
 * If there are any validated wakeup sources, it also notifies the BSW Manager of these sources.
 */
ECUM_LOCAL void EcuM_InitAndNotifyBswM(EcuM_RunTimeLcType* tempRunData)
{
    /* Initialise ECUM state and notify BSWM */
    tempRunData->State = ECUM_STATE_STARTUP;
    BswM_EcuM_CurrentState(ECUM_STATE_STARTUP);

    /* If the verified wake-up source is valid, notify the BSWM */
    if ((*tempRunData->Wks.Validated) != 0uL)
    {
        BswM_EcuM_CurrentWakeup((*tempRunData->Wks.Validated), ECUM_WKSTATUS_VALIDATED);
    }
}
/* PRQA S 2814 -- */

/* PRQA S 1503, 2814 ++ */ /* VL_QAC_NoUsedApi, VL_QAC_MultiReturn */
/**
 * Initializes the internal state and runtime variables of the ECU Manager.
 *
 * This function sets the global clock to zero if the alarm clock is present.
 * It initializes pointers to pending, expired, and validated wakeup sources.
 * It also initializes the timer state for each wakeup source.
 * If mode handling is enabled, it sets the runtime state to transition mode.
 */
ECUM_LOCAL void EcuM_InternalInit(EcuM_RunTimeLcType* pRt)
{
#if (ECUM_ALARM_CLOCK_PRESENT == STD_ON)
    pRt->GlobalClock     = 0u;
    pRt->EcuM_TimeRecord = 0u;
#endif /*ECUM_ALARM_CLOCK_PRESENT == STD_ON*/

    pRt->Wks.Pending   = &EcuMWksPending;
    pRt->Wks.Expired   = &EcuMWksExpired;
    pRt->Wks.Validated = &EcuMWksValidated;
#if (ECUM_MAX_MCU_CORE_NUM > 1u)
    pRt->coreId = GetCoreID();
#endif /*ECUM_MAX_MCU_CORE_NUM > 1 */

    for (uint8 i = 0u; i < (uint8)ECUM_MAX_WAKE_UP_SOURCE_NUM; i++)
    {
        pRt->Wks.timerState[i] = ECUM_TIMER_STOP;
    }

#if (ECUM_MODE_HANDING == STD_ON)
    EcuM_RteState = RTE_TRANSITION_EcuM_Mode;
#endif /*ECUM_MODE_HANDING == STD_ON*/
}
/* PRQA S 1503, 2814 -- */

/* PRQA S 2814 ++ */ /* VL_QAC_MultiReturn */
/**
 * Performs pre-OS initialization activities for the ECU Manager.
 *
 * This function sets programmable interrupt priorities if required.
 * It initializes block 0 drivers and other pre-OS, low-level initialization code if enabled.
 * It determines and validates the post-build configuration data for the ECU Manager and other BSW modules.
 * If the configuration data is inconsistent, it calls the error hook.
 * If the configuration is valid, it initializes block 1 drivers and other pre-OS, low-level initialization code if
 * enabled, and initializes the core.
 */
ECUM_LOCAL void EcuM_StartPreOS(void)
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
        EcuM_InitializeCore();
    }
}
/* PRQA S 2814 -- */

/* PRQA S 2814 ++ */ /* VL_QAC_MultiReturn */
/**
 * Initializes the core for the ECU Manager.
 *
 * This function determines the reset reason and maps it to a wakeup source.
 * It selects the default shutdown target and mode.
 * If reset loop detection is enabled, it performs loop detection.
 * For multi-core systems, it starts the slave cores.
 */
ECUM_LOCAL void EcuM_InitializeCore(void)
{
#if (ECUM_MAX_MCU_CORE_NUM > 1)
    CoreIdType coreId;
    CoreIdType coreNum;
    StatusType status;
#endif

#if (ECUM_MAX_MCU_CORE_NUM > 1)
    coreId = GetCoreID();
    if (ECUM_MASTER_CORE_ID == coreId)
#endif
    {
        Mcu_ResetType resetReason;

        /*Get reset reason. The reset reason is derived from a call to
         *Mcu_GetResetReason and the mapping defined via the EcuMWakeupSource
         *configuration containers.*/
        resetReason = Mcu_GetResetReason();
        EcuM_MapReset2WakeupSource(resetReason);

        /* Select Default Shutdown Target */
        (void)EcuM_SelectShutdownTarget(EcuM_ConfigPtr->defaultShutdownTgt, EcuM_ConfigPtr->defaultShutdownMode);

#if (ECUM_RESET_LOOP_DETECTION == STD_ON)
        (void)EcuM_LoopDetection();
#endif

#if (ECUM_MAX_MCU_CORE_NUM > 1)
        coreNum = ECUM_MAX_MCU_CORE_NUM - 1u;
        for (coreId = 0u; coreId < coreNum; coreId++)
        {
            StartCore(EcuM_SlaveCoreIds[coreId], &status);
        }
#endif
    }
}
/* PRQA S 2814 -- */

/* PRQA S 2814, 2824 ++ */ /* VL_QAC_MultiReturn, VL_EcuM_ComputeInvalidPtr */
/**
 * Maps the reset reason to a wakeup source.
 *
 * This function iterates through the configured wakeup sources and their associated reset reasons.
 * If a match is found between the reset reason and a configured reset reason, it sets the corresponding wakeup source
 * as validated. If no match is found, it sets a default reset wakeup source.
 */
ECUM_LOCAL void EcuM_MapReset2WakeupSource(Mcu_ResetType resetReason)
{
    uint8   wkSrcIdx;
    uint32  rstRasaonIdx;
    boolean wakeupTag = FALSE;

    for (wkSrcIdx = 0u; wkSrcIdx < (uint8)EcuM_ConfigPtr->wkSourceNum; wkSrcIdx++)
    {
        for (rstRasaonIdx = 0u; rstRasaonIdx < EcuM_GetWakeupSourceCfgPtr(wkSrcIdx)->resetResasonNum; rstRasaonIdx++)
        {
            if (EcuM_GetWakeupSourceCfgPtr(wkSrcIdx)->resetResasonPtr[rstRasaonIdx] == resetReason)
            {
                EcuMWksValidated =
                    (EcuM_WakeupSourceType)(EcuMWksValidated) | (EcuM_GetWakeupSourceCfgPtr(wkSrcIdx)->wkSource);
                wakeupTag = TRUE;
                break;
            }
        }
        if (wakeupTag)
        {
            break;
        }
    }
    if (!wakeupTag)
    {
        /* SWS_EcuM_02601 */
        EcuMWksValidated = (EcuM_WakeupSourceType)(EcuMWksValidated) | ECUM_WKSOURCE_RESET;
    }
}
/* PRQA S 2814, 2824 -- */

#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"
