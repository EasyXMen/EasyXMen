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
/*
 ************************************************************************************************************************
 **
 **  @file               : EcuM_Up.c
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : ECU state management
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "EcuM_Internal.h"
#if (ECUM_MODE_HANDING == STD_ON)
#include "Rte_EcuM.h"
#endif /*ECUM_MODE_HANDING == STD_ON*/
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/**
 * @brief Main function for processing wakeup sources.
 * @param[in] pRt : Pointer to the runtime configuration structure.
 * @details This function processes all wakeup sources to check for timeouts and validate wakeup events.
 *          It iterates through all configured wakeup sources and performs the following actions:
 *          - Checks if a wakeup occurred but no wakeup event was set within the timeout.
 *          - Notifies BSWM if a wakeup source has expired.
 *          - Stops the wakeup source if it has expired.
 *          - Validates the wakeup source if it is pending.
 *          - Updates internal variables to record expired wakeup sources.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-69088
 */
ECUM_LOCAL void EcuM_WakeupMainFunction(const EcuM_RunTimeLcType* pRt);

#if (ECUM_MODE_HANDING == STD_ON)
/**
 * @brief Main function for handling mode transitions.
 * @param[in] pRt : Pointer to the runtime configuration structure.
 * @details This function handles the arbitration between RUN and POST_RUN requests and releases.
 *          It performs the following actions:
 *          - Switches the current mode to the requested mode if the state is not in transition.
 *          - Updates the runtime state and notifies BSWM of the current state if the switch acknowledgment is
 * successful.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-64801
 */
ECUM_LOCAL void EcuM_ModeHandingMainFunction(EcuM_RunTimeLcType* pRt);
#endif /*ECUM_MODE_HANDING == STD_ON*/
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"
/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"

/* PRQA S 1503, 1532, 2814 ++ */ /* VL_QAC_NoUsedApi, VL_QAC_OneFunRef, VL_QAC_MultiReturn */
/**
 * Main function for the ECU Manager.
 *
 * This function checks if any wakeup sources have triggered and initiates wakeup validation.
 * If the alarm clock is present, it updates the alarm clock timer.
 * For multi-core systems, it checks if a shutdown request has been issued on a non-master core and handles the shutdown
 * sequence. If mode handling is enabled, it arbitrates RUN and POST_RUN requests and releases.
 */
void EcuM_MainFunction(void)
{
    EcuM_RunTimeLcType* pRt;
    if (EcuM_isInitState(&pRt))
    {
        /*1.Check if wake-up sources have woken up and to initiate wake-up validation.*/
        EcuM_WakeupMainFunction(pRt);
#if (ECUM_ALARM_CLOCK_PRESENT == STD_ON)
        /*2.Update the Alarm Clock timer.*/
        EcuM_UpdateEcuMClock(pRt);
#endif /*ECUM_ALARM_CLOCK_PRESENT == STD_ON*/

#if (ECUM_MAX_MCU_CORE_NUM > 1)
        /*get core ID for this running core*/
        if (ECUM_MASTER_CORE_ID != pRt->coreId)
        {
            boolean lShutdowntag = EcuM_ShutdownFlag;
            if ((lShutdowntag) && (!pRt->rqstShutFlag))
            {
                pRt->rqstShutFlag = TRUE;
                pRt->State        = ECUM_STATE_SHUTDOWN;
                BswM_EcuM_CurrentState(ECUM_STATE_SHUTDOWN);
                /*start OffPreOS sequence*/
                EcuM_OffPreOS();
            }
        }
        else
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
        {
#if (ECUM_MODE_HANDING == STD_ON)
            /*3.Arbitrate RUN and POST_RUN requests and releases.*/
            if ((pRt->State != pRt->rqstState) && (0u != pRt->rqstState))
            {
                EcuM_ModeHandingMainFunction(pRt);
            }
#endif /*ECUM_MODE_HANDING == STD_ON*/
        }
    }
}
/* PRQA S 1503, 1532, 2814 -- */
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"
/* ========================================== internal function definitions ========================================= */
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/* PRQA S 2814 ++ */ /* VL_QAC_MultiReturn */
/**
 * Handles the main wakeup function for the ECU Manager.
 *
 * This function retrieves pending wakeup events and validates them.
 * It checks if any wakeup sources have timed out and notifies the BSW Manager accordingly.
 * It also stops the wakeup sources if they are expired or invalid.
 * The function loops through all configured wakeup sources and performs the necessary checks and actions.
 */
ECUM_LOCAL void EcuM_WakeupMainFunction(const EcuM_RunTimeLcType* pRt)
{
    const EcuM_WakeupSourceCfgType* pWks;
    uint32                          spanTime;
#if (ECUM_USE_TIMER == ECUM_TIMER_USE_GPT)
    Std_ReturnType ret;
#endif /*ECUM_USE_TIMER == ECUM_TIMER_USE_GPT*/
    EcuM_WakeupSourceType pendingWkup;

    uint8 wksIdx;
    /*Get pending wakeup events for validation*/
    pendingWkup = EcuM_GetPendingWakeupEvents();

    /*Loop WHILE no wakeup event has been validated AND timeout not expired*/
    for (wksIdx = 0u; wksIdx < (uint8)EcuM_ConfigPtr->wkSourceNum; wksIdx++)
    {
        pWks = EcuM_GetWakeupSourceCfgPtr(wksIdx);
        /* Check that a wakeup occurred but no wakeup event set timeout */
        if ((pWks->checkWkupTimeout != 0uL) && (pRt->Wks.timerState[wksIdx] == ECUM_TIMER_VALID)
#if (ECUM_MAX_MCU_CORE_NUM > 1)
            && (pRt->coreId == pRt->Wks.coreId)
#endif /* ECUM_MAX_MCU_CORE_NUM > 1 */
        )
        {
#if (ECUM_USE_TIMER == ECUM_TIMER_USE_GPT)
            ret = Tm_GetTimeSpan100us32bit(&(pRt->Wks.wkTime[wksIdx]), &spanTime);
#else
            spanTime = EcuM_CalculateElapsedMS(pRt->Wks.wkTime[wksIdx]);
#endif /*ECUM_USE_TIMER == ECUM_TIMER_USE_GPT*/
            if ((pWks->checkWkupTimeout < spanTime)
#if (ECUM_USE_TIMER == ECUM_TIMER_USE_GPT)
                || (ret != E_OK)
#endif /*ECUM_USE_TIMER == ECUM_TIMER_USE_GPT*/
            )
            {
                /*notify BSWM for expired wake up source*/
                BswM_EcuM_CurrentWakeup(pWks->wkSource, ECUM_WKSTATUS_EXPIRED);
                /*stop wake up source*/
                EcuM_StopWakeupSources(pWks->wkSource);
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
                (void)Det_ReportRuntimeError(
                    ECUM_MODULE_ID,
                    ECUM_INSTANCE_ID,
                    ECUM_SID_MAINFUNCTION,
                    ECUM_E_WAKEUP_TIMEOUT);
#endif /* ECUM_DEV_ERROR_DETECT == STD_ON */
            }
        }

        /* Check if the wake-up source is valid */
        if (((pWks->wkSource & pendingWkup) != 0uL)
#if (ECUM_MAX_MCU_CORE_NUM > 1)
            && (pRt->coreId == pRt->Wks.coreId)
#endif /* ECUM_MAX_MCU_CORE_NUM > 1 */
        )
        {
            EcuM_CheckValidation(pWks->wkSource);
#if (ECUM_USE_TIMER == ECUM_TIMER_USE_GPT)
            ret = Tm_GetTimeSpan100us32bit(&(pRt->Wks.wkTime[wksIdx]), &spanTime);
#else
            spanTime = EcuM_CalculateElapsedMS(pRt->Wks.wkTime[wksIdx]);
#endif /*ECUM_USE_TIMER == ECUM_TIMER_USE_GPT*/
            /*Validation Timeout.*/
            if (((pWks->validationTimeout < spanTime)
#if (ECUM_USE_TIMER == ECUM_TIMER_USE_GPT)
                 || (ret != E_OK)
#endif /*ECUM_USE_TIMER == ECUM_TIMER_USE_GPT*/
                     )
                && (pRt->Wks.timerState[wksIdx] == ECUM_TIMER_VALID))
            {
                *pRt->Wks.Pending &= ~(pWks->wkSource);
                /*Update internal variable(record expired wake up source.)*/
                (*pRt->Wks.Expired) |= pWks->wkSource;
                /*notify BSWM for expired wake up source*/
                BswM_EcuM_CurrentWakeup(pWks->wkSource, ECUM_WKSTATUS_EXPIRED);
                /*stop wake up source*/
                EcuM_StopWakeupSources(pWks->wkSource);
            }
        }
    }
}
/* PRQA S 2814 -- */

#if (ECUM_MODE_HANDING == STD_ON)
/* PRQA S 2814 ++ */ /* VL_QAC_MultiReturn */
/**
 * Handles mode transitions for the ECU Manager.
 *
 * This function arbitrates between RUN and POST_RUN requests and releases.
 * It switches the current mode if the runtime state is not in transition.
 * It updates the ECU Manager state and notifies the BSW Manager if the mode switch acknowledgment is successful.
 */
ECUM_LOCAL void EcuM_ModeHandingMainFunction(EcuM_RunTimeLcType* pRt)
{
    /*Arbitrate RUN and POST_RUN requests and releases.*/
    if (EcuM_RteState != RTE_TRANSITION_EcuM_Mode)
    {
        if (RTE_E_OK == Rte_Switch_currentMode_currentMode(EcuM_RteState))
        {
            EcuM_RteState = RTE_TRANSITION_EcuM_Mode;
        }
    }
    if (Rte_SwitchAck_currentMode_currentMode() != RTE_E_NO_DATA)
    {
        pRt->State = pRt->rqstState;
        BswM_EcuM_CurrentState(pRt->State);
    }
}
/* PRQA S 2814 -- */
#endif /*ECUM_MODE_HANDING == STD_ON*/
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"
