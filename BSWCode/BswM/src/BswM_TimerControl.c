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
 **  @file               : BswM_TimerControl.c
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : BSW Mode Manager
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "BswM_Internal.h"
/* PRQA S 0553 EOF */ /* VL_BswM_0553 */
#if (BSWM_TIMER_ENABLED == STD_ON)
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

/* ========================================== internal function definitions ========================================= */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
/**
 * Initializes the BswM timers by stopping all timers and setting their cycles to zero.
 * This function retrieves the configuration and runtime data, iterates through each timer, and sets their status to
 * stopped. If the configuration retrieval fails, an error is reported using the Det_ReportError function.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_InitBswMTimer(void)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PartitionPCCfgType*      bswmPartPCCfgPtr;
    const BswM_ModeRqstPortPCCfgType*   modeRqstPCfgPtr;
    const BswM_ModeRqstPortRuntimeType* modeRqstPtr;
    BswM_TimerIdxType                   numOfBswMTimer;
    BswM_TimerIdxType                   timerIdx;
    boolean                             result;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        modeRqstPCfgPtr = bswmPartPCCfgPtr->modeRqstPCCfg;
        modeRqstPtr     = bswmPartPCCfgPtr->modeRqstPortRunPtr;
        numOfBswMTimer  = modeRqstPCfgPtr->numOfBswMTimer;
        for (timerIdx = 0u; timerIdx < numOfBswMTimer; timerIdx++)
        {
            /*Each BswMTimer shall be stopped (BSWM_TIMER_STOPPED) during initialization*/
            modeRqstPtr->timerStatus[timerIdx] = BSWM_TIMER_STOP;
            modeRqstPtr->timerCycle[timerIdx]  = 0u;
        }
    }
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    else
    {
        (void)Det_ReportError(BSWM_MODULE_ID, BSWM_INSTANCE_ID, BSWM_API_ID_INIT, BSWM_E_INIT_FAILED);
    }
#endif /*BSWM_DEV_ERROR_DETECT == STD_ON*/
}

/**
 * Retrieves the status of a specified BswM timer.
 * This function checks the configuration and runtime data to find the timer status for the given timer index.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
BswM_TimerEnumType BswM_GerTimerStatus(BswM_TimerIdxType timerIdx)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
    BswM_TimerEnumType             timerStatus = BSWM_TIMER_START;
    boolean                        result;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        timerStatus = bswmPartPCCfgPtr->modeRqstPortRunPtr->timerStatus[timerIdx];
    }
    return timerStatus;
}
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
/* ========================================== internal function definitions ========================================= */

#endif /*BSWM_TIMER_ENABLED == STD_ON*/
