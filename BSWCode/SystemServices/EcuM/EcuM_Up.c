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
**  FILENAME    : EcuM_Up.c                                                   **
**                                                                            **
**  Created on  :                                                             **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION : Implement code for ECUM UP phase                            **
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
 *  V2.1.0       2021-11-25  qinchun.yang    Modify EcuM mode handle function.
 */

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "EcuM_Internal.h"
#if (ECUM_MODE_HANDING == STD_ON)
#include "Rte_EcuM.h"
#endif /*ECUM_MODE_HANDING == STD_ON*/
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
static FUNC(void, ECUM_CODE) EcuM_WakeupMainFunction(void);

#if (ECUM_MODE_HANDING == STD_ON)
/*Do EcuM Mode Handling*/
static FUNC(void, ECUM_CODE) EcuM_ModeHandingMainFunction(void);
#endif /*ECUM_MODE_HANDING == STD_ON*/
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/**
 * The purpose of this service is to implement all activities of the ECU State
 * Manager while the OS is up and running.
 * Service ID[hex]: 0x18
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): NA
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: NA
 */
FUNC(void, ECUM_CODE) EcuM_MainFunction(void)
{
#if (ECUM_MAX_MCU_CORE_NUM > 1)
    CoreIdType coreId;
    static boolean rqstShutFlag = FALSE;
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/

    /*1.Check if wake-up sources have woken up and to initiate wake-up validation.*/
    EcuM_WakeupMainFunction();
#if (ECUM_ALARM_CLOCK_PRESENT == STD_ON)
    /*2.Update the Alarm Clock timer.*/
    EcuM_UpdateEcuMClock();
#endif /*ECUM_ALARM_CLOCK_PRESENT == STD_ON*/

#if (ECUM_MAX_MCU_CORE_NUM > 1)
    /*get core ID for this running core*/
    coreId = GetCoreID();
    if (ECUM_MASTER_CORE_ID != coreId)
    {
        if ((TRUE == EcuM_ShutdownFlag) && (FALSE == rqstShutFlag))
        {
            rqstShutFlag = TRUE;
            EcuMRunData.State = ECUM_STATE_SHUTDOWN;
            BswM_EcuM_CurrentState(ECUM_STATE_SHUTDOWN);
            /*start OffPreOS sequence*/
            EcuM_OffPreOS();
        }
    }
    else
    {
#if (ECUM_MODE_HANDING == STD_ON)
        /*3.Arbitrate RUN and POST_RUN requests and releases.*/
        if (EcuMRunData.State != EcuMRunData.rqstState)
        {
            EcuM_ModeHandingMainFunction();
        }
#endif /*ECUM_MODE_HANDING == STD_ON*/
    }
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
}
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
static FUNC(void, ECUM_CODE) EcuM_WakeupMainFunction(void)
{
    P2CONST(EcuM_RunTimeType, AUTOMATIC, ECUM_CONST) pRt = &EcuMRunData; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    P2CONST(EcuM_WakeupSourceCfgType, AUTOMATIC, ECUM_CONST) pWks;
    uint32 spanTime;
#if (ECUM_USE_TIMER == ECUM_TIMER_USE_GPT)
    Std_ReturnType ret;
#endif /*ECUM_USE_TIMER == ECUM_TIMER_USE_GPT*/
    EcuM_WakeupSourceType pendingWkup;

    uint8 wksIdx;

    /*Get pending wakeup events for validation*/
    pendingWkup = EcuM_GetPendingWakeupEvents();

    /*Loop WHILE no wakeup event has been validated AND timeout not expired*/
    for (wksIdx = 0; wksIdx < (uint8)ECUM_MAX_WAKE_UP_SOURCE_NUM; wksIdx++)
    {
        pWks = &(EcuM_WkSourceCfgs[wksIdx]);
        if ((pWks->wkSource & pendingWkup) != 0u)
        {
            EcuM_CheckValidation(pWks->wkSource);
#if (ECUM_USE_TIMER == ECUM_TIMER_USE_GPT)
            ret = Tm_GetTimeSpan100us32bit(&(pRt->Wks.wkTime[wksIdx]), &spanTime);
#else
            spanTime = EcuM_CalculateElapsedMS(pRt->Wks.wkTime[wksIdx]);
#endif /*ECUM_USE_TIMER == ECUM_TIMER_USE_GPT*/
            /*Validation Timeout.*/
            if ((pWks->validationTimeout < spanTime)
#if (ECUM_USE_TIMER == ECUM_TIMER_USE_GPT)
                || (ret != E_OK)
#endif /*ECUM_USE_TIMER == ECUM_TIMER_USE_GPT*/
            )

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

#if (ECUM_MODE_HANDING == STD_ON)
/*Do EcuM Mode Handling*/
static FUNC(void, ECUM_CODE) EcuM_ModeHandingMainFunction(void)
{
    /*Arbitrate RUN and POST_RUN requests and releases.*/
    if (EcuM_RteState != RTE_TRANSITION_EcuM_Mode)
    {
        (void)SchM_Switch_currentMode(EcuM_RteState);
        EcuM_RteState = RTE_TRANSITION_EcuM_Mode;
    }
    if (SchM_SwitchAck_currentMode() != RTE_E_NO_DATA)
    {
        EcuMRunData.State = EcuMRunData.rqstState;
        BswM_EcuM_CurrentState(EcuMRunData.State);
    }
}
#endif /*ECUM_MODE_HANDING == STD_ON*/
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"
