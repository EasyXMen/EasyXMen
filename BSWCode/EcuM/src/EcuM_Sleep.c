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
 **  @file               : EcuM_Sleep.c
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : ECU state management
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "EcuM_Internal.h"
#if (ECUM_COMM_PNC_ENABLED == STD_ON) || (ECUM_COMM_CHANNEL_ENABLED == STD_ON)
#include "ComM_EcuM.h"
#endif /* ECUM_COMM_PNC_ENABLED == STD_ON */
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
#if (ECUM_SLEEP_MODE_ENABLED == STD_ON)
/**
 * @brief Puts the ECU into the specified sleep mode.
 * @details This function transitions the ECU into the specified sleep mode by configuring the wake-up sources and
 * notifying the BswM module. It handles both single-core and multi-core configurations.
 * @param[in] pRt : Pointer to the runtime data structure containing the current state and target sleep mode.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-64790
 */
ECUM_LOCAL void EcuM_GoSleep(EcuM_RunTimeLcType* pRt);

/**
 * @brief Executes the halt sequence for the ECU.
 * @details This function synchronizes the cores (if multi-core), disables all interrupts, generates a RAM hash (if
 * master core), and enters the halt mode. It also handles wake-up events and re-enables interrupts upon wake-up.
 * @param[in] pRt : Pointer to the runtime data structure containing the current state and target sleep mode.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-64792
 */
ECUM_LOCAL void EcuM_HaltSequence(const EcuM_RunTimeLcType* pRt);

/**
 * @brief Executes the poll sequence for the ECU.
 * @details This function disables all interrupts, sets the MCU to a power-saving mode, and then enters a loop where it
 *          performs sleep activities and checks for polling-based wake-up sources. If an alarm clock is present, it
 * also checks for expired wake-up sources. The function continues until a valid wake-up event is detected.
 * @param[in] pRt : Pointer to the runtime data structure containing the current state and target sleep mode.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-69018
 */
ECUM_LOCAL void EcuM_PollSequence(const EcuM_RunTimeLcType* pRt);

/**
 * @brief Executes the exit sequence from the halt mode.
 * @details This function handles the exit from the halt mode by checking the RAM integrity and signaling other cores
 *          to continue execution. It first checks if the alarm clock is present and if the alarm has not expired.
 *          If these conditions are met, it disables all interrupts, generates a RAM hash, and enters the halt mode.
 *          Upon returning from the halt mode, it checks the RAM hash for integrity. If the RAM check fails, it calls
 *          the error hook. If the RAM check passes, it notifies the BSW manager and signals any slave cores to
 * continue.
 * @param[in] pRt : Pointer to the runtime data structure containing the current state and target sleep mode.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-64794
 */
ECUM_LOCAL void EcuM_HaltSequenceExit(const EcuM_RunTimeLcType* pRt);

/**
 * @brief Notifies the BSW manager about the current wakeup sources.
 * @details This function iterates through all configured wakeup sources and checks if they are pending.
 *          If a wakeup source is pending and requires validation, it calls the BswM_EcuM_CurrentWakeup function
 *          with the pending status. If no validation is required, it calls the BswM_EcuM_CurrentWakeup function
 *          with the validated status, but only for internal wakeup sources.
 * @param[in] pRt : Pointer to the runtime data structure containing the current state and pending wakeup sources.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-64873
 */
ECUM_LOCAL void EcuM_SleepNotifyBswM(const EcuM_RunTimeLcType* pRt);

/**
 * @brief Restarts the wakeup process.
 * @details This function cancels any active alarms, restores the MCU to its normal mode if necessary,
 *          processes pending and validated wakeup sources, and initializes drivers that need restarting.
 *          It also releases the operating system resources for the current core.
 * @return None
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-69020
 */
ECUM_LOCAL void EcuM_WakeupRestart(void);

#if (ECUM_MAX_MCU_CORE_NUM > 1)
/**
 * @brief Determines if all slave cores are ready to sleep.
 * @details This function iterates through all slave cores and checks if they are ready to sleep.
 *          If any slave core is not ready, the function returns FALSE.
 * @return boolean
 * @retval TRUE : All slave cores are ready to sleep.
 * @retval FALSE : At least one slave core is not ready to sleep.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-64799
 */
ECUM_LOCAL boolean EcuM_JudgeSleep(void);
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/

#if (ECUM_MAX_MCU_CORE_NUM > 1)
/**
 * @brief Clears the ready-to-sleep flags for all slave cores.
 * @details This function acquires a spin lock, sets the ready-to-sleep flags for all slave cores to FALSE,
 *          and then releases the spin lock. If acquiring or releasing the spin lock fails, an error hook is called.
 * @return None
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-64771
 */
ECUM_LOCAL void EcuM_ClrSlaveReadySleepFlag(void);

/**
 * @brief Sets the ready-to-sleep flags for all slave cores.
 * @details This function acquires a spin lock, sets the ready-to-sleep flags for all slave cores to TRUE,
 *          and then releases the spin lock. If acquiring or releasing the spin lock fails, an error hook is called.
 * @return None
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-64872
 */
ECUM_LOCAL void EcuM_SetSlaveReadySleepFlag(void);
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/

/**
 * @brief Executes the sleep phase of the ECU.
 * @details This function prepares the ECU to enter a sleep mode. It determines the appropriate sleep mode based on the
 * runtime data, and performs the necessary actions to either halt the MCU or enter a polling state. After the sleep
 * phase, it restarts the activities required for wake-up from the polling state.
 * @param[in] pRt : Pointer to the runtime data structure.
 * @return Std_ReturnType
 * @retval E_OK : The sleep phase was successfully executed.
 * @retval E_NOT_OK : An error occurred during the sleep phase.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-64773
 */
ECUM_LOCAL Std_ReturnType EcuM_DoSleepPhase(EcuM_RunTimeLcType* pRt);
#endif /*ECUM_SLEEP_MODE_ENABLED == STD_ON*/

/**
 * @brief Processes pending and validated wake-up sources.
 * @details This function retrieves the pending and validated wake-up sources, and then disables them.
 *          Other wake-up sources are retained for subsequent wake-ups.
 * @return None
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-69021
 */
ECUM_LOCAL void EcuM_ProcessPendingAndValidatedWakeupSources(void);

/**
 * @brief Validates a wake-up event and performs associated actions.
 * @details This function validates a wake-up event by updating the wake-up status and notifying relevant communication
 * channels. It also handles PNC (Power Network Coordination) wake-up indications if enabled.
 * @param[in] wkSrcIdx : Index of the wake-up source configuration.
 * @return None
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-64876
 */
ECUM_LOCAL void EcuM_ValidateWkEvAction(uint8 wkSrcIdx);

/**
 * @brief Handles expired wake-up sources.
 * @details This function processes wake-up sources that have timed out. It stops the timer for the specified wake-up
 * source, clears the pending status, sets the expired status, and notifies the BSWM (Basic Software Module) of the
 * time-out.
 * @param[in] pRt : Pointer to the runtime data structure.
 * @param[in] wksIdx : Index of the wake-up source.
 * @param[in] sources : Wake-up sources that have expired.
 * @return None
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-69023
 */
ECUM_LOCAL void EcuM_HandleExpiredWakeupSources(EcuM_RunTimeLcType* pRt, uint8 wksIdx, EcuM_WakeupSourceType sources);

/**
 * @brief Sets the wakeup source as valid.
 * @details This function checks if the wakeup source requires validation. If it does, it starts the validation timer
 *          and sets the timer state to valid. If no validation is required, it updates the pending and validated wakeup
 *          sources and calls the EcuM_ValidateWkEvAction function if the current state is greater than or equal to
 *          ECUM_STATE_STARTUP.
 * @param[in] pRt : Pointer to the runtime data structure containing the current state and wakeup source information.
 * @param[in] sources : The wakeup sources that need to be validated.
 * @param[in] wksIdx : Index of the wakeup source in the configuration array.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-69019
 */
ECUM_LOCAL void EcuM_SetWakeupIsValid(EcuM_RunTimeLcType* pRt, EcuM_WakeupSourceType sources, uint8 wksIdx);

/**
 * @brief Sets the pending wake-up sources.
 * @details This function sets the specified wake-up sources as pending in the runtime data structure.
 *          It uses a critical section to ensure thread safety.
 * @param[in] pRt : Pointer to the runtime data structure.
 * @param[in] sources : Wake-up sources to be set as pending.
 * @return None
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-69022
 */
ECUM_LOCAL void EcuM_SetPendingWakeupSources(EcuM_RunTimeLcType* pRt, EcuM_WakeupSourceType sources);

#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"
/* ============================================ internal data definitions =========================================== */
#if (ECUM_MAX_MCU_CORE_NUM > 1)
#define ECUM_START_SEC_VAR_FAST_CLEARED_BOOLEAN
#include "EcuM_MemMap.h"
/*Shutdown request flag, set by master core, check by slave main function*/
ECUM_LOCAL volatile boolean EcuM_SlaveReadySleepFlag[ECUM_SLAVE_CORE_NUM];
#define ECUM_STOP_SEC_VAR_FAST_CLEARED_BOOLEAN
#include "EcuM_MemMap.h"
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"

/* PRQA S 1503, 2814 ++ */ /* VL_QAC_NoUsedApi, VL_QAC_MultiReturn */
/**
 * Initiates the appropriate shutdown or sleep phase based on the specified user and shutdown target.
 *
 * This function checks the current initialization state and the shutdown target. Depending on the target,
 * it either initiates the shutdown phase or the sleep phase. If the target is invalid or the module is not initialized,
 * it reports the corresponding error.
 */
Std_ReturnType EcuM_GoDownHaltPoll(EcuM_UserType UserID)
{
    Std_ReturnType      ret = E_OK;
    EcuM_RunTimeLcType* pRt;
    uint8               detErrorId = ECUM_E_OK;

    /* Init Check */
    if (EcuM_isInitState(&pRt))
    {
        switch (pRt->SdtgNext.Target)
        {
        case ECUM_SHUTDOWN_TARGET_RESET:
            /*Do nothing*/
        case ECUM_SHUTDOWN_TARGET_OFF:
            /*EcuM_GoDownHaltPoll with shutdown target RESET or OFF initiates the SHUTDOWN Phase.*/
            ret = EcuM_DoShutDownPhase(UserID);
            break;
#if (ECUM_SLEEP_MODE_ENABLED == STD_ON)
        case ECUM_SHUTDOWN_TARGET_SLEEP:
            /*EcuM_GoDownHaltPoll with shutdown target SLEEP initiate the SLEEP phase.*/
            ret = EcuM_DoSleepPhase(pRt);
            break;
#endif /* ECUM_SLEEP_MODE_ENABLED == STD_ON */
        default:
            ret        = E_NOT_OK;
            detErrorId = ECUM_E_STATE_PAR_OUT_OF_RANGE;
            break;
        }
    }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    else
    {
        detErrorId = ECUM_E_UNINIT;
        ret        = E_NOT_OK;
    }

    if (ECUM_E_OK != detErrorId)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_GODOWNHALTPOLL, detErrorId);
    }

#else
    (void)detErrorId;
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON */

    return ret;
}
/* PRQA S 1503, 2814 -- */

/* PRQA S 2814 ++ */ /* VL_QAC_MultiReturn */
/**
 * Retrieves the pending wakeup events for the ECU module.
 *
 * This function checks if the ECU module is in the initialized state and then retrieves the pending wakeup events.
 * It uses critical section protection to ensure thread safety when accessing the wakeup events.
 * If the module is not initialized, it reports an error.
 */
EcuM_WakeupSourceType EcuM_GetPendingWakeupEvents(void)
{
    EcuM_WakeupSourceType wks = ECUM_WKSOURCE_NONE;
    EcuM_RunTimeLcType*   pRt;

    /* Init Check */
    if (EcuM_isInitState(&pRt))
    {
        /*Enter critical protection zone*/
        SchM_Enter_EcuM_WkEv();
        wks = *pRt->Wks.Pending;
        SchM_Exit_EcuM_WkEv();
    }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    /* Init Check */
    else
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_GETPENDINGWAKEUPEVENT, ECUM_E_UNINIT);
    }
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/

    return wks;
}
/* PRQA S 2814 -- */

/* PRQA S 1503, 1532, 2814 ++ */ /* VL_QAC_NoUsedApi, VL_QAC_OneFunRef, VL_QAC_MultiReturn */
/**
 * Clears the specified wakeup events for the ECU module.
 *
 * This function checks if the ECU module is in the initialized state and then clears the specified wakeup events.
 * It uses critical section protection to ensure thread safety when modifying the wakeup events.
 * If the module is not initialized, it reports an error.
 */
void EcuM_ClearWakeupEvent(EcuM_WakeupSourceType sources)
{
    EcuM_RunTimeLcType* pRt;

    /* Init Check */
    if (EcuM_isInitState(&pRt))
    {
        /*Enter critical protection zone*/
        SchM_Enter_EcuM_WkEv();
        EcuMWksPending   = (EcuM_WakeupSourceType)((*pRt->Wks.Pending) & (~sources));
        EcuMWksValidated = (EcuM_WakeupSourceType)((*pRt->Wks.Validated) & (~sources));
        EcuMWksExpired   = (EcuM_WakeupSourceType)((*pRt->Wks.Expired) & (~sources));
        SchM_Exit_EcuM_WkEv();
        BswM_EcuM_CurrentWakeup(sources, ECUM_WKSTATUS_NONE);
    }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    else
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_CLEARWAKEUPEVENT, ECUM_E_UNINIT);
    }
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/

    return;
}
/* PRQA S 1503, 1532, 2814 -- */

/* PRQA S 2814 ++ */ /* VL_QAC_MultiReturn */
/**
 * Retrieves the validated wakeup events for the ECU module.
 *
 * This function checks if the ECU module is in the initialized state and then retrieves the validated wakeup events.
 * It uses critical section protection to ensure thread safety when accessing the wakeup events.
 * If the module is not initialized, it reports an error.
 */
EcuM_WakeupSourceType EcuM_GetValidatedWakeupEvents(void)
{
    EcuM_WakeupSourceType wks = ECUM_WKSOURCE_NONE;
    EcuM_RunTimeLcType*   pRt;

    /* Init Check */
    if (EcuM_isInitState(&pRt))
    {
        /*Enter critical protection zone*/
        SchM_Enter_EcuM_WkEv();
        wks = (*pRt->Wks.Validated);
        SchM_Exit_EcuM_WkEv();
    }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    else
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_GETVALIDATEDWAKEUPEVENT, ECUM_E_UNINIT);
    }
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/

    return wks;
}
/* PRQA S 2814 -- */

/* PRQA S 1503, 2814 ++ */ /* VL_QAC_NoUsedApi, VL_QAC_MultiReturn */
/**
 * Retrieves the expired wakeup events for the ECU module.
 *
 * This function checks if the ECU module is in the initialized state and then retrieves the expired wakeup events.
 * It uses critical section protection to ensure thread safety when accessing the wakeup events.
 * If the module is not initialized, it reports an error.
 */
EcuM_WakeupSourceType EcuM_GetExpiredWakeupEvents(void)
{
    EcuM_WakeupSourceType wks = ECUM_WKSOURCE_NONE;
    EcuM_RunTimeLcType*   pRt;

    /* Init Check */
    if (EcuM_isInitState(&pRt))
    {
        /*Enter critical protection zone*/
        SchM_Enter_EcuM_WkEv();
        wks = (*pRt->Wks.Expired);
        SchM_Exit_EcuM_WkEv();
    }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    else
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_GETEXPIREDWAKEUPEVENT, ECUM_E_UNINIT);
    }
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/

    return wks;
}
/* PRQA S 1503, 2814 -- */

/* PRQA S 1503, 1505, 2814 ++ */ /* VL_QAC_NoUsedApi, VL_EcuM_1505, VL_QAC_MultiReturn */
/**
 * Checks and processes the specified wakeup source for the ECU module.
 *
 * This function first checks if the ECU module is in the initialized state. It then validates the wakeup source.
 * If the wakeup source is valid, it calls the `EcuM_CheckWakeupHook` function to handle the wakeup event.
 * If the module is not initialized or the wakeup source is invalid, it reports the corresponding error.
 */
void EcuM_CheckWakeup(EcuM_WakeupSourceType wakeupSource)
{
    EcuM_RunTimeLcType* pRt;
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    uint8 detErrorId = ECUM_E_OK;
    /* Init Check */
    if (!EcuM_isInitState(&pRt))
    {
        detErrorId = ECUM_E_UNINIT;
    }
    /*SWS_EcuM_02867*/
    else if (
        (ECUM_WKSOURCE_NONE == wakeupSource)
        || (ECUM_WKSOURCE_NONE
            != (EcuM_WakeupSourceType)(wakeupSource & (~((EcuM_WakeupSourceType)EcuM_ConfigPtr->allWkSource)))))
    {
        detErrorId = ECUM_E_UNKNOWN_WAKEUP_SOURCE;
    }
    else
#else
    if (EcuM_isInitState(&pRt))
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    {
        EcuM_CheckWakeupHook(wakeupSource);
    }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    if (ECUM_E_OK != detErrorId)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_CHECKWAKEUP, detErrorId);
    }
#endif /* ECUM_DEV_ERROR_DETECT == STD_ON */
}
/* PRQA S 1503, 1505, 2814 -- */

#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"

#define ECUM_START_SEC_CODE_FAST
#include "EcuM_MemMap.h"

/* PRQA S 1503, 2814, 6030 ++ */ /* VL_QAC_NoUsedApi, VL_QAC_MultiReturn, VL_MTR_EcuM_STMIF */
/**
 * Sets the specified wakeup events for the ECU module.
 *
 * This function checks if the ECU module is in the initialized state and validates the wakeup sources.
 * It then sets the pending wakeup sources and calculates the elapsed time for each wakeup source.
 * If the wakeup timeout has been exceeded, it handles the expired wakeup sources.
 * If the module is not initialized or the wakeup source is invalid, it reports the corresponding error.
 */
void EcuM_SetWakeupEvent(EcuM_WakeupSourceType sources)
{
    uint32              spanTime;
    EcuM_RunTimeLcType* pRt;

#if (ECUM_USE_TIMER == ECUM_TIMER_USE_GPT)
    Std_ReturnType ret;
#endif /*ECUM_USE_TIMER == ECUM_TIMER_USE_GPT*/
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    uint8 detErrorId = ECUM_E_OK;
    /* Init Check */
    if (!EcuM_isInitState(&pRt))
    {
        detErrorId = ECUM_E_UNINIT;
    }
    /*SWS_EcuM_02867*/
    else if (
        (ECUM_WKSOURCE_NONE == sources)
        || (ECUM_WKSOURCE_NONE
            != (EcuM_WakeupSourceType)(sources & (~((EcuM_WakeupSourceType)EcuM_ConfigPtr->allWkSource)))))
    {
        detErrorId = ECUM_E_UNKNOWN_WAKEUP_SOURCE;
    }
    else
#else
    if (EcuM_isInitState(&pRt))
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    {
        do
        {
            /*SWS_EcuM_04138
             * Ignore all events passed in the sources parameter that are not associated
             * to the selected sleep mode*/
#if (ECUM_SLEEP_MODE_ENABLED == STD_ON)
            EcuM_ShutdownModeType sleepModeId = pRt->SdtgNext.Mode;
            if ((sources & EcuM_SleepModeCfgs[sleepModeId].wkMask) != ECUM_WKSOURCE_NONE)
#endif /*ECUM_SLEEP_MODE_ENABLED == STD_ON*/
            {
                EcuM_SetPendingWakeupSources(pRt, sources);

                /*Calculate config wake up index.*/
                uint8 wksIdx = EcuM_WkSrcMap2CfgWkIdx(sources);
                if (wksIdx != (uint8)EcuM_ConfigPtr->wkSourceNum)
                {
                    if (EcuM_GetWakeupSourceCfgPtr(wksIdx)->checkWkupTimeout != 0uL)
                    {
#if (ECUM_USE_TIMER == ECUM_TIMER_USE_GPT)
                        ret = Tm_GetTimeSpan100us32bit(&(pRt->Wks.wkTime[wksIdx]), &spanTime);
#else
                        spanTime = EcuM_CalculateElapsedMS(pRt->Wks.wkTime[wksIdx]);
#endif /*ECUM_USE_TIMER == ECUM_TIMER_USE_GPT*/
                        if ((EcuM_GetWakeupSourceCfgPtr(wksIdx)->checkWkupTimeout < spanTime)
#if (ECUM_USE_TIMER == ECUM_TIMER_USE_GPT)
                            || (ret != E_OK)
#endif /*ECUM_USE_TIMER == ECUM_TIMER_USE_GPT*/
                        )
                        {
                            EcuM_HandleExpiredWakeupSources(pRt, wksIdx, sources);
                            break;
                        }
                    }
                    EcuM_SetWakeupIsValid(pRt, sources, wksIdx);
                }
            }
        } while (0u); /* PRQA S 2743 */ /* VL_EcuM_2743 */
    }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    if (ECUM_E_OK != detErrorId)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_SETWAKEUPEVENT, detErrorId);
    }
#endif /* ECUM_DEV_ERROR_DETECT == STD_ON */
}
/* PRQA S 1503, 2814, 6030 -- */
#define ECUM_STOP_SEC_CODE_FAST
#include "EcuM_MemMap.h"

#define ECUM_START_SEC_CODE_FAST
#include "EcuM_MemMap.h"
/* PRQA S 1503, 2814 ++ */ /* VL_QAC_NoUsedApi, VL_QAC_MultiReturn */
/**
 * Validates the specified wakeup events for the ECU module.
 *
 * This function checks if the ECU module is in the initialized state and validates the wakeup sources.
 * It then updates the pending and validated wakeup events and stops the timer for the specified wakeup source.
 * If the module is not initialized or the wakeup source is invalid, it reports the corresponding error.
 */
void EcuM_ValidateWakeupEvent(EcuM_WakeupSourceType sources)
{
    EcuM_RunTimeLcType* pRt;
    uint8               detErrorId = ECUM_E_OK;
    uint8               wkIdx;

    /* Init Check */
    if (!EcuM_isInitState(&pRt))
    {
        detErrorId = ECUM_E_UNINIT;
    }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    /* SWS_EcuM_02868 */
    else if (
        (ECUM_WKSOURCE_NONE == sources)
        || (ECUM_WKSOURCE_NONE
            != (EcuM_WakeupSourceType)(sources & (~((EcuM_WakeupSourceType)EcuM_ConfigPtr->allWkSource)))))
    {
        detErrorId = ECUM_E_UNKNOWN_WAKEUP_SOURCE;
    }
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    else
    {
        wkIdx                                = EcuM_WkSrcMap2CfgWkIdx(sources);
        const EcuM_WakeupSourceCfgType* pWks = EcuM_GetWakeupSourceCfgPtr(wkIdx);

        /*SWS_EcuM_02790*/
        if ((ECUM_STATE_RUN == (pRt->State & ECUM_STATE_RUN))
#if (ECUM_COMM_CHANNEL_ENABLED == STD_ON)
            && (NULL_PTR == pWks->comMChnl)
#endif /*ECUM_COMM_CHANNEL_ENABLED == STD_ON*/
#if (ECUM_COMM_PNC_ENABLED == STD_ON)
            && (NULL_PTR == pWks->pnc)
#endif /*ECUM_COMM_PNC_ENABLED == STD_ON*/
        )
        {
            (void)detErrorId;
        }
        else
        {
            SchM_Enter_EcuM_WkEv();
            (*pRt->Wks.Pending) &= ~(sources);
            (*pRt->Wks.Validated) |= sources;
            SchM_Exit_EcuM_WkEv();
            if (wkIdx != (uint8)EcuM_ConfigPtr->wkSourceNum)
            {
                pRt->Wks.timerState[wkIdx] = ECUM_TIMER_STOP;
                EcuM_ValidateWkEvAction(wkIdx);
            }
            (void)pWks;
        }
    }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    if (ECUM_E_OK != detErrorId)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_VALIDATEWAKEUPEVENT, detErrorId);
    }
#else
    (void)detErrorId;
#endif /* ECUM_DEV_ERROR_DETECT == STD_ON */
}
/* PRQA S 1503, 2814 -- */

/**
 * Maps the specified wakeup source to its corresponding configuration index.
 *
 * This function iterates through the wakeup source configurations to find the index of the specified wakeup source.
 * If the wakeup source is found, it returns the corresponding index; otherwise, it returns
 * `ECUM_MAX_WAKE_UP_SOURCE_NUM`.
 */
/* PRQA S 1505 ++ */ /* VL_EcuM_1505 */
uint8 EcuM_WkSrcMap2CfgWkIdx(EcuM_WakeupSourceType wkSrc)
{
    uint8 wkIdx = EcuM_ConfigPtr->wkSourceNum;
    uint8 idx;

    for (idx = 0u; idx < (uint8)EcuM_ConfigPtr->wkSourceNum; idx++)
    {
        if ((EcuM_GetWakeupSourceCfgPtr(idx)->wkSource & wkSrc) != 0uL)
        {
            wkIdx = idx;
        }
    }
    return wkIdx;
}
/* PRQA S 1505 -- */

#define ECUM_STOP_SEC_CODE_FAST
#include "EcuM_MemMap.h"

/* ========================================== internal function definitions ========================================= */
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"

#if (ECUM_SLEEP_MODE_ENABLED == STD_ON)
/* PRQA S 2814,2824 ++ */ /* VL_QAC_MultiReturn */
/**
 * Prepares the ECU module to enter the sleep mode.
 *
 * This function determines the sleep mode and configures the wakeup sources for the target sleep mode.
 * It notifies the BSW manager about the current wakeup state and enables the wakeup sources.
 * For multi-core systems, it handles the resource allocation for both master and slave cores.
 * Finally, it sets the ECU module state to sleep and notifies the BSW manager of the state change.
 * If the OS resource allocation fails, it calls the error hook.
 */
ECUM_LOCAL void EcuM_GoSleep(EcuM_RunTimeLcType* pRt)
{
#if (ECUM_MAX_MCU_CORE_NUM > 1)
    CoreIdType coreId;
    uint8      slaveIdx;
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
    StatusType            osRet = E_OK;
    uint8                 index;
    EcuM_ShutdownModeType sleepModeId;
    uint8                 wksNum;
    uint8                 refWkSrcIdx;

    sleepModeId = pRt->SdtgNext.Mode;
    if (sleepModeId <= (EcuM_ShutdownModeType)ECUM_MAX_SLEEP_MODE_NUM)
    {
        wksNum = EcuM_SleepModeCfgs[sleepModeId].numberOfWakeSource;

        for (index = 0u; index < wksNum; index++)
        {
            refWkSrcIdx = EcuM_SleepModeCfgs[sleepModeId].wkSrcIdx[index];
            /*Notify BswM about wake up state.*/
            BswM_EcuM_CurrentWakeup(EcuM_GetWakeupSourceCfgPtr(refWkSrcIdx)->wkSource, ECUM_WKSTATUS_NONE);
            /*To set the wakeup sources up for the next sleep mode, the ECU Manager
             *module shall execute the EcuM_EnableWakeupSources callout for each wakeup
             *source that is configured for the target sleep mode*/
            EcuM_EnableWakeupSources(EcuM_GetWakeupSourceCfgPtr(refWkSrcIdx)->wkSource);
        }
#if (ECUM_MAX_MCU_CORE_NUM > 1)
        /*get core ID for this running core*/
        coreId = pRt->coreId;
        if (ECUM_MASTER_CORE_ID == coreId)
        {
            osRet = GetResource(EcuM_OSResource[ECUM_MASTER_CORE_ID]);
        }
        else
        {
            /*Get slave core Index in configuration*/
            slaveIdx = EcuM_GetSlaveCoreIndex(coreId);
            osRet    = GetResource(EcuM_OSResource[slaveIdx + 1u]);
        }
#else  /*Single core.*/
        osRet = GetResource(EcuM_OSResource);
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
        /*Enter in SLEEP mode.*/
        pRt->State = ECUM_STATE_SLEEP;
        BswM_EcuM_CurrentState(ECUM_STATE_SLEEP);
    }
    if ((StatusType)E_OK != osRet)
    {
        EcuM_ErrorHook(ECUM_E_CALL_OS_FAILED);
    }
}
/* PRQA S 2814,2824 -- */

/* PRQA S 2814 ++ */ /* VL_QAC_MultiReturn */
/**
 * Sets the specified wakeup sources as valid for the ECU module.
 *
 * This function checks if a validation timeout is configured for the wakeup source.
 * If a validation timeout is set and the timer state is not already valid, it starts the wakeup sources and initializes
 * the validation timer. If no validation is required, it updates the pending and validated wakeup events and performs
 * the necessary validation action.
 */
ECUM_LOCAL void EcuM_SetWakeupIsValid(EcuM_RunTimeLcType* pRt, EcuM_WakeupSourceType sources, uint8 wksIdx)
{
    if (EcuM_GetWakeupSourceCfgPtr(wksIdx)->validationTimeout != 0uL)
    {
        /* Modify the process, put EcuM_StartWakeupSources into
         * EcuM_SetWakeupEvent and execute(For pending wake up source).*/
        EcuM_StartWakeupSources(sources);
        /* req to SWS_EcuM_02712*/
        if (pRt->Wks.timerState[wksIdx] != ECUM_TIMER_VALID)
        {
#if (ECUM_USE_TIMER == ECUM_TIMER_USE_GPT)
            /*SWS_EcuM_02707
             * Start validation timer*/
            (void)Tm_ResetTimer100us32bit(&(pRt->Wks.wkTime[wksIdx]));
#else  /*Use Os counter.*/
            pRt->Wks.wkTime[wksIdx] = EcuM_CurrentTimestampMS();
#endif /*ECUM_USE_TIMER == ECUM_TIMER_USE_GPT*/
            pRt->Wks.timerState[wksIdx] = ECUM_TIMER_VALID;
#if (ECUM_MAX_MCU_CORE_NUM > 1)
            pRt->Wks.coreId = pRt->coreId;
#endif /* ECUM_MAX_MCU_CORE_NUM > 1 */
        }
    }
    else /*No validation.*/
    {
        (*pRt->Wks.Pending) &= ~sources;
        (*pRt->Wks.Validated) |= sources;
        /* EcuM_ValidateWakeupEvent cannot be called directly when no validation is required   */
        if (pRt->State >= ECUM_STATE_STARTUP)
        {
            EcuM_ValidateWkEvAction(wksIdx);
        }
    }
}
/* PRQA S 2814 -- */

/**
 * @brief Setting the pending wake-up source
 */
/**
 * Sets the specified wakeup sources as pending for the ECU module.
 *
 * This function enters a critical section to ensure thread safety, sets the specified wakeup sources as pending, and
 * then exits the critical section.
 */
/* PRQA S 2814, 3673 ++ */ /* VL_QAC_MultiReturn, VL_QAC_3673 */
ECUM_LOCAL void EcuM_SetPendingWakeupSources(EcuM_RunTimeLcType* pRt, EcuM_WakeupSourceType sources)
{
    SchM_Enter_EcuM_WkEv();
    /* Setting the pending wake-up source */
    (*pRt->Wks.Pending) |= sources;
#if (ECUM_MAX_MCU_CORE_NUM > 1)
    pRt->Wks.coreId = pRt->coreId;
#endif /* ECUM_MAX_MCU_CORE_NUM > 1 */
    SchM_Exit_EcuM_WkEv();
}
/* PRQA S 2814, 3673 -- */

/* PRQA S 2814 ++ */ /* VL_QAC_MultiReturn */
/**
 * Handles the expiration of the specified wakeup sources for the ECU module.
 *
 * This function enters a critical section to ensure thread safety, stops the timer for the specified wakeup source,
 * updates the pending and expired wakeup events, and then exits the critical section. It also notifies the BSW manager
 * of the expired wakeup source and stops the wakeup sources.
 */
ECUM_LOCAL void EcuM_HandleExpiredWakeupSources(EcuM_RunTimeLcType* pRt, uint8 wksIdx, EcuM_WakeupSourceType sources)
{
    SchM_Enter_EcuM_WkEv();
    pRt->Wks.timerState[wksIdx] = ECUM_TIMER_STOP;
    (*pRt->Wks.Pending) &= ~(sources);
    (*pRt->Wks.Expired) |= sources;
    SchM_Exit_EcuM_WkEv();

    /* Wake-up source to notify BSWM of time-outs */
    BswM_EcuM_CurrentWakeup(sources, ECUM_WKSTATUS_EXPIRED);
    EcuM_StopWakeupSources(sources);
}
/* PRQA S 2814 -- */

/* PRQA S 2814,6070 ++ */ /* VL_QAC_MultiReturn, VL_MTR_EcuM_STCAL */
/**
 * Handles the halt sequence for the ECU module.
 *
 * This function synchronizes the master and slave cores before entering the halt mode.
 * It waits for all slave cores to be ready to sleep and ensures that no pending or validated wakeup events are present.
 * The function disables all interrupts, generates a RAM hash on the master core, and enters the specified halt mode.
 * If a wakeup event occurs, it re-enables interrupts and exits the halt sequence.
 */
ECUM_LOCAL void EcuM_HaltSequence(const EcuM_RunTimeLcType* pRt)
{
    EcuM_ShutdownModeType sleepModeId;
    EcuM_WakeupSourceType pendWks;
    EcuM_WakeupSourceType ValidatedWks;

#if (ECUM_MAX_MCU_CORE_NUM > 1)
    CoreIdType coreId;
    uint8      slaveIdx;

    /*The master core could issue the MCU halt before all slave cores are finished processing,
     * the cores must be synchronized before entering GoHalt.*/
    coreId = GetCoreID();
    if (ECUM_MASTER_CORE_ID == coreId)
    {
        /*Wait for all Slave cores to be ready to sleep()*/
        while (!EcuM_JudgeSleep())
        {
            /* Waiting for a wake-up to occur from when the core is dormant */
            pendWks      = EcuM_GetPendingWakeupEvents();
            ValidatedWks = EcuM_GetValidatedWakeupEvents();
            if (((EcuM_ConfigPtr->allWkSource & pendWks) != 0uL)
                || ((EcuM_ConfigPtr->allWkSource & ValidatedWks) != 0uL))
            {
                break;
            }
        }
    }
    else /*Slave core*/
    {
        StatusType osRet = E_OK;
        slaveIdx         = EcuM_GetSlaveCoreIndex(coreId);
        do
        {
            osRet = GetSpinlock(EcuM_RefSpinLockCfg);
            if (E_OK != osRet)
            {
                break;
            }
            EcuM_SlaveReadySleepFlag[slaveIdx] = TRUE;
            osRet                              = ReleaseSpinlock(EcuM_RefSpinLockCfg);
        } while (0u); /* PRQA S 2743 */ /* VL_EcuM_2743 */
        if (E_OK != osRet)
        {
            EcuM_ErrorHook(ECUM_E_CALL_OS_FAILED);
        }
    }

#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
    /*Disable all interrupts*/
    DisableAllInterrupts();
#if (ECUM_MAX_MCU_CORE_NUM > 1)
    if (ECUM_MASTER_CORE_ID == coreId)
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
    {
        EcuM_GenerateRamHash();
    }
    /* When a wakeup occurs and is not pending or validated, EcuM continues to sleep */
    while (TRUE) /* PRQA S 2740 */ /* VL_EcuM_2740 */
    {
        pendWks      = EcuM_GetPendingWakeupEvents();
        ValidatedWks = EcuM_GetValidatedWakeupEvents();
        if (((EcuM_ConfigPtr->allWkSource & pendWks) == 0uL) && ((EcuM_ConfigPtr->allWkSource & ValidatedWks) == 0uL))
        {
            sleepModeId = pRt->SdtgNext.Mode;
            /*Enter in halt mode,ECU Manager module does not execute any code.*/
            EcuM_McuSetMode(EcuM_SleepModeCfgs[sleepModeId].mcuMode);
            /*Enable all interrupts*/
            EnableAllInterrupts();
        }
        else
        {
            /*Enable all interrupts*/
            EnableAllInterrupts();
            break;
        }
    }
    /*Wait resume form MCU Halt mode.*/
    EcuM_HaltSequenceExit(pRt);
}
/* PRQA S 2814,6070 -- */

/* PRQA S 2814,6070 ++ */ /* VL_QAC_MultiReturn, VL_MTR_EcuM_STCAL */
/**
 * Executes the poll sequence for the ECU module.
 *
 * This function disables all interrupts, sets the microcontroller to a power-saving mode, and then re-enables
 * interrupts. It continuously polls the wakeup sources that require polling and checks for pending wakeup events. If an
 * alarm clock is present, it also checks for expired wakeup sources. Once a pending wakeup event is detected, it
 * notifies the BSW manager and synchronizes the slave cores if applicable. The function ensures that the slave cores
 * wait for the master core's signal to continue.
 */
ECUM_LOCAL void EcuM_PollSequence(const EcuM_RunTimeLcType* pRt)
{
    EcuM_ShutdownModeType sleepModeId;
    uint8                 index;
    EcuM_WakeupSourceType pendingWkup;
    EcuM_WakeupSourceType validatedWkup;

    DisableAllInterrupts();
    sleepModeId = pRt->SdtgNext.Mode;
    /*EcuM_McuSetMode() puts the microcontroller in some power saving mode.
     * In this mode software execution continues, but with reduced clock speed. */
    EcuM_McuSetMode(EcuM_SleepModeCfgs[sleepModeId].mcuMode);
    EnableAllInterrupts();
    do
    {
        /*Sleep activity*/
        EcuM_SleepActivity();

        /*loop FOR all wakeup sources that need polling*/
        for (index = 0u; index < (uint8)EcuM_ConfigPtr->wkSourceNum; index++)
        {
            if (EcuM_GetWakeupSourceCfgPtr(index)->isPolling)
            {
                /*check the wakeup source*/
                EcuM_CheckWakeup(EcuM_GetWakeupSourceCfgPtr(index)->wkSource);
            }
        }
        pendingWkup   = EcuM_GetPendingWakeupEvents();
        validatedWkup = EcuM_GetValidatedWakeupEvents();
#if (ECUM_ALARM_CLOCK_PRESENT == STD_ON)
        /*Get the expired wakeup sources*/
        if ((ECUM_WKSOURCE_ALARMCLOCK == ((*pRt->Wks.Validated) & ECUM_WKSOURCE_ALARMCLOCK))
            || ((pRt->GlobalClock >= pRt->MasterAlarm) && (pRt->MasterAlarm != ECUM_MAX_ALARM_VALUES)))
        {
            /*Additional Confidition to Loop: While (AlarmClockService Present AND EcuM_AlarmClock
             *  only pending event AND Alarm not expired)*/
            break;
        }
#endif /*ECUM_ALARM_CLOCK_PRESENT == STD_ON*/
    } while ((0uL == (EcuM_ConfigPtr->allWkSource & pendingWkup))
             && (0uL == (EcuM_ConfigPtr->allWkSource & validatedWkup)));
    EcuM_SleepNotifyBswM(pRt);
#if (ECUM_MAX_MCU_CORE_NUM > 1)
    CoreIdType coreId;
    coreId = pRt->coreId;
    /*Signal all slave cores to continue.*/
    if (ECUM_MASTER_CORE_ID == coreId)
    {
        EcuM_SetSlaveReadySleepFlag();
    }
    else /*Slave core*/
    {
        uint8 slaveIdx;
        slaveIdx = EcuM_GetSlaveCoreIndex(coreId);
        /*Wait for master core signal to continue.*/
        /* PRQA S 3442, 3140, 2212 ++ */ /* VL_EcuM_waitCycle */
        while (!(EcuM_SlaveReadySleepFlag[slaveIdx]))
            ;
        /* PRQA S 3442, 3140, 2212 -- */
        StatusType osRet = E_OK;
        do
        {
            osRet = GetSpinlock(EcuM_RefSpinLockCfg);
            if (E_OK != osRet)
            {
                break;
            }
            /*Clear slave core ready sleep flag.*/
            EcuM_SlaveReadySleepFlag[slaveIdx] = FALSE;
            osRet                              = ReleaseSpinlock(EcuM_RefSpinLockCfg);
        } while (0u); /* PRQA S 2743 */ /* VL_EcuM_2743 */
        if (E_OK != osRet)
        {
            EcuM_ErrorHook(ECUM_E_CALL_OS_FAILED);
        }
    }
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
}
/* PRQA S 2814,6070 -- */

/* PRQA S 2814 ++ */ /* VL_QAC_MultiReturn */
/**
 * Handles the exit sequence from the halt mode for the ECU module.
 *
 * This function checks if the alarm clock is present and, if so, ensures that the alarm has not expired.
 * It disables all interrupts, generates a RAM hash, and enters the halt mode.
 * Upon returning from the halt mode, it checks the RAM hash for integrity.
 * If the RAM check fails, it calls the error hook.
 * If the RAM check succeeds, it notifies the BSW manager and synchronizes the slave cores if applicable.
 * The function ensures that the slave cores wait for the master core's signal to continue.
 */
ECUM_LOCAL void EcuM_HaltSequenceExit(const EcuM_RunTimeLcType* pRt)
{
#if (ECUM_ALARM_CLOCK_PRESENT == STD_ON)
    EcuM_ShutdownModeType sleepModeId;
#endif /*ECUM_ALARM_CLOCK_PRESENT == STD_ON*/
    uint8 ramHash;
#if (ECUM_MAX_MCU_CORE_NUM > 1)
    CoreIdType coreId;
    uint8      slaveIdx;
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/

#if (ECUM_MAX_MCU_CORE_NUM > 1)
    coreId = pRt->coreId;
    if (ECUM_MASTER_CORE_ID == coreId)
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
    {
#if (ECUM_ALARM_CLOCK_PRESENT == STD_ON)
        /*Condition:
         * EcuM_AlarmClock only pending event AND Alarm not expired*/
        if ((EcuM_GetPendingWakeupEvents() & ECUM_WKSOURCE_ALARMCLOCK) && (pRt->MasterAlarm > pRt->GlobalClock)
            && (pRt->MasterAlarm != ECUM_MAX_ALARM_VALUES))
        {
            /*Disable all interrupts*/
            DisableAllInterrupts();
            EcuM_GenerateRamHash();
            sleepModeId = pRt->SdtgNext.Mode;
            /*Enter in halt mode,ECU Manager module does not execute any code.*/
            EcuM_McuSetMode(EcuM_SleepModeCfgs[sleepModeId].mcuMode);
            EnableAllInterrupts();
        }
#endif /*ECUM_ALARM_CLOCK_PRESENT == STD_ON*/

        /*returned from halt, check RAM hash*/
        ramHash = EcuM_CheckRamHash();
        if (ramHash == 0u)
        {
            /*RAM check failed,EcuM_ErrorHook will not return.*/
            EcuM_ErrorHook(ECUM_E_RAM_CHECK_FAILED);
        }
    }
    /*RAM check success*/
    EcuM_SleepNotifyBswM(pRt);
#if (ECUM_MAX_MCU_CORE_NUM > 1)
    /*Signal all slave cores to continue.*/
    if (ECUM_MASTER_CORE_ID == coreId)
    {
        EcuM_ClrSlaveReadySleepFlag();
    }
    else /*Slave core*/
    {
        slaveIdx = EcuM_GetSlaveCoreIndex(coreId);
        /*Wait for master core signal to continue.*/
        /* PRQA S 3442, 3140, 2212 ++ */ /* VL_EcuM_waitCycle */
        while (EcuM_SlaveReadySleepFlag[slaveIdx])
            ;
        /* PRQA S 3442, 3140, 2212 -- */
    }
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
}
/* PRQA S 2814 -- */

/* PRQA S 2814 ++ */ /* VL_QAC_MultiReturn */
/**
 * Notifies the BSW manager about the current wakeup sources for the ECU module.
 *
 * This function iterates through all wakeup sources and checks if they are pending.
 * If a pending wakeup source requires validation, it notifies the BSW manager with the pending status.
 * If validation is not needed, it notifies the BSW manager with the validated status, excluding external watchdog
 * sources.
 */
ECUM_LOCAL void EcuM_SleepNotifyBswM(const EcuM_RunTimeLcType* pRt)
{
    uint8 index;

    for (index = 0u; index < (uint8)EcuM_ConfigPtr->wkSourceNum; index++)
    {
        if ((EcuM_GetWakeupSourceCfgPtr(index)->wkSource & (*pRt->Wks.Pending)) != 0uL)
        {
            /*wakeup validation needed or not*/
            if (((EcuM_GetWakeupSourceCfgPtr(index)->wkSource & (*pRt->Wks.Validated)) == 0uL)
                && (EcuM_GetWakeupSourceCfgPtr(index)->validationTimeout > 0uL))
            {
                /*validation needed*/
                BswM_EcuM_CurrentWakeup(EcuM_GetWakeupSourceCfgPtr(index)->wkSource, ECUM_WKSTATUS_PENDING);
            }
        }
    }
}
/* PRQA S 2814 -- */

/*WakeUpSequence from sleep phase.*/
/**
 * Restarts the wakeup process for the ECU module.
 *
 * This function cancels any active alarms if the alarm clock is present.
 * It restores the MCU to its normal mode if necessary and processes pending and validated wakeup sources.
 * For multi-core systems, it initializes drivers that need restarting and releases the appropriate OS resources for
 * both master and slave cores. If the resource release fails, it calls the error hook.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_EcuM_STCAL */
ECUM_LOCAL void EcuM_WakeupRestart(void)
{
    StatusType osRet;
#if (ECUM_MAX_MCU_CORE_NUM > 1)
    CoreIdType coreId;
    uint8      slaveIdx;
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/

#if (ECUM_ALARM_CLOCK_PRESENT == STD_ON)
    /*Cancelled alarms.*/
    EcuM_CancellAlarms();
#endif /*ECUM_ALARM_CLOCK_PRESENT == STD_ON*/
    /*Edit by yqc,
     * 2020/08/28
     * Some MCU don't need to call EcuM_McuSetMode to reach normal mode.*/
    if (EcuM_NormalMcuModeCfg != ECUM_DAFULT_MCU_NORMAL_MODE)
    {
        DisableAllInterrupts();
        /*Restore MCU normal mode
         * Selected MCU mode is configured in the configuration parameter EcuMNormalMcuModeRef*/
        EcuM_McuSetMode(EcuM_NormalMcuModeCfg);
        EnableAllInterrupts();
    }

    EcuM_ProcessPendingAndValidatedWakeupSources();

#if (ECUM_MAX_MCU_CORE_NUM > 1)
    coreId = GetCoreID();
    if (ECUM_MASTER_CORE_ID == coreId)
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
    {
#if (ECUM_DRIVER_RESTART_LIST == STD_ON)
        /*Initialize drivers that need restarting*/
        EcuM_AL_DriverRestart();
#endif
#if (ECUM_MAX_MCU_CORE_NUM > 1)
        osRet = ReleaseResource(EcuM_OSResource[ECUM_MASTER_CORE_ID]);
#else  /*Single core.*/
        osRet = ReleaseResource(EcuM_OSResource);
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
    }
#if (ECUM_MAX_MCU_CORE_NUM > 1)
    else /*Slave Core*/
    {
        /*Get slave core Index in configuration*/
        slaveIdx = EcuM_GetSlaveCoreIndex(coreId);
        osRet    = ReleaseResource(EcuM_OSResource[slaveIdx + 1u]);
    }
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
    if ((StatusType)E_OK != osRet)
    {
        EcuM_ErrorHook(ECUM_E_CALL_OS_FAILED);
    }
}
/* PRQA S 6070 -- */

#if (ECUM_MAX_MCU_CORE_NUM > 1)
/**
 * Checks if all slave cores are ready to sleep.
 *
 * This function iterates through all slave cores and verifies if their ready-to-sleep flags are set.
 * If any slave core is not ready, it returns false; otherwise, it returns true.
 */
ECUM_LOCAL boolean EcuM_JudgeSleep(void)
{
    CoreIdType cIdx;
    CoreIdType slaveCoreNum;
    boolean    ret = TRUE;

    slaveCoreNum = ECUM_MAX_MCU_CORE_NUM - 1u;
    for (cIdx = 0u; cIdx < slaveCoreNum; cIdx++)
    {
        if (!EcuM_SlaveReadySleepFlag[cIdx]) /* PRQA S 3442 */ /* VL_EcuM_waitCycle */
        {
            ret = FALSE;
            break;
        }
    }
    return ret;
}
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/

#if (ECUM_MAX_MCU_CORE_NUM > 1)
/**
 * Clears the ready-to-sleep flags for all slave cores.
 *
 * This function acquires a spinlock to ensure thread safety, clears the ready-to-sleep flags for all slave cores,
 * and then releases the spinlock. If the spinlock operations fail, it calls the error hook.
 */
ECUM_LOCAL void EcuM_ClrSlaveReadySleepFlag(void)
{
    StatusType osRet = E_OK;
    uint8      slaveIdx;

    do
    {
        osRet = GetSpinlock(EcuM_RefSpinLockCfg);
        if (E_OK != osRet)
        {
            break;
        }
        for (slaveIdx = 0u; slaveIdx < (ECUM_MAX_MCU_CORE_NUM - 1u); slaveIdx++)
        {
            EcuM_SlaveReadySleepFlag[slaveIdx] = FALSE;
        }
        osRet = ReleaseSpinlock(EcuM_RefSpinLockCfg);
    } while (0u); /* PRQA S 2743 */ /* VL_EcuM_2743 */

    if (E_OK != osRet)
    {
        EcuM_ErrorHook(ECUM_E_CALL_OS_FAILED);
    }
}

/**
 * Sets the ready-to-sleep flags for all slave cores.
 *
 * This function acquires a spinlock to ensure thread safety, sets the ready-to-sleep flags for all slave cores,
 * and then releases the spinlock. If the spinlock operations fail, it calls the error hook.
 */
ECUM_LOCAL void EcuM_SetSlaveReadySleepFlag(void)
{
    StatusType osRet = E_OK;
    uint8      slaveIdx;
    do
    {
        osRet = GetSpinlock(EcuM_RefSpinLockCfg);
        if (E_OK != osRet)
        {
            break;
        }
        for (slaveIdx = 0u; slaveIdx < (ECUM_MAX_MCU_CORE_NUM - 1u); slaveIdx++)
        {
            EcuM_SlaveReadySleepFlag[slaveIdx] = TRUE;
        }
        osRet = ReleaseSpinlock(EcuM_RefSpinLockCfg);
    } while (0u); /* PRQA S 2743 */ /* VL_EcuM_2743 */

    if (E_OK != osRet)
    {
        EcuM_ErrorHook(ECUM_E_CALL_OS_FAILED);
    }
}
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/

/**
 * Processes pending and validated wakeup sources.
 *
 * This function retrieves the pending and validated wakeup sources, disables them, and retains other wakeup sources for
 * subsequent wake-ups.
 */
ECUM_LOCAL void EcuM_ProcessPendingAndValidatedWakeupSources(void)
{
    EcuM_WakeupSourceType pendingWkSrc;
    EcuM_WakeupSourceType validatedWkSrc;
    /* Get pending wake-up sources */
    pendingWkSrc = EcuM_GetPendingWakeupEvents();
    /* Get verified wakeup source */
    validatedWkSrc = EcuM_GetValidatedWakeupEvents();
    /* Disables currently pending and verified wake sources, but retains other wake sources for subsequent wake-ups */
    EcuM_DisableWakeupSources(pendingWkSrc | validatedWkSrc);
}

/* PRQA S 2814, 2824 ,2934 ++ */ /* VL_EcuM_ComputeInvalidPtr */
/**
 * Validates a wakeup source and performs associated actions.
 *
 * This function marks the specified wakeup source as validated and notifies the BSW manager.
 * If communication channels are enabled, it also sends wakeup indications to the specified communication channels.
 * If power network control (PNC) is enabled, it sends PNC wakeup indications to the specified PNC handles.
 */
ECUM_LOCAL void EcuM_ValidateWkEvAction(uint8 wkSrcIdx)
{
    const EcuM_WakeupSourceCfgType* pWks;

    pWks = EcuM_GetWakeupSourceCfgPtr(wkSrcIdx);
    BswM_EcuM_CurrentWakeup(pWks->wkSource, ECUM_WKSTATUS_VALIDATED);
#if (ECUM_COMM_CHANNEL_ENABLED == STD_ON)
    if (NULL_PTR != pWks->comMChnl)
    {
        for (uint8 commChnIdx = 0u; commChnIdx < pWks->commChnNum; commChnIdx++)
        {
            ComM_EcuM_WakeUpIndication(pWks->comMChnl[commChnIdx]);
        }
    }
#endif /* ECUM_COMM_CHANNEL_ENABLED == STD_ON */
#if (ECUM_COMM_PNC_ENABLED == STD_ON)
    if (NULL_PTR != pWks->pnc)
    {
        /*notify COMM for PNC wake up*/
        for (PNCHandleType pncIdx = 0u; pncIdx < pWks->pncNum; pncIdx++)
        {
            ComM_EcuM_PNCWakeUpIndication(pWks->pnc[pncIdx]);
        }
    }
#endif /*ECUM_COMM_PNC_ENABLED == STD_ON*/
}
/* PRQA S 2814, 2824 ,2934 -- */

/* PRQA S 2814 ++ */ /* VL_QAC_MultiReturn */
/**
 * Executes the sleep phase for the ECU module.
 *
 * This function prepares the system to go to sleep, determines the appropriate sleep mode, and executes the
 * corresponding sequence (halt or poll). After the sleep sequence, it restarts the wakeup activities. If the sleep mode
 * is valid and the sequence is successfully executed, it returns success; otherwise, it returns an error.
 */
ECUM_LOCAL Std_ReturnType EcuM_DoSleepPhase(EcuM_RunTimeLcType* pRt)
{
    Std_ReturnType        ret = E_NOT_OK;
    EcuM_ShutdownModeType sleepModeId;
    boolean               doHalt;

    /*prepare to go sleep*/
    EcuM_GoSleep(pRt);

    /*Determine which control streams to use*/
    sleepModeId = pRt->SdtgNext.Mode;
    if (sleepModeId <= (EcuM_ShutdownModeType)ECUM_MAX_SLEEP_MODE_NUM)
    {
        doHalt = EcuM_SleepModeCfgs[sleepModeId].sleepSuspend;
        if (doHalt)
        {
            /*activities for halt MCU*/
            EcuM_HaltSequence(pRt);
        }
        else
        {
            /*Activities for POLL*/
            EcuM_PollSequence(pRt);
        }
        /*Restart activities for wakeup from POLL*/
        EcuM_WakeupRestart();
        ret = E_OK;
    }

    return ret;
}
/* PRQA S 2814 -- */
#endif /* ECUM_SLEEP_MODE_ENABLED == STD_ON */
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"
