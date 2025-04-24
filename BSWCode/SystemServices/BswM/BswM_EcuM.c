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
**  FILENAME    : BswM_EcuM.c                                                 **
**                                                                            **
**  Created on  : 2020-03-24                                                  **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "BswM_EcuM.h"
#if (BSWM_ECUM_ENABLED == STD_ON)
/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
/*Initialize BswMEcuMWakeupSource request port*/
FUNC(void, BSWM_ECUM_CODE)
BswM_InitEcuMRequestPorts(void)
{
    P2CONST(BswM_ModeRqstPortPCCfgType, AUTOMATIC, BSWM_CONST) modeRqstPCfgPtr;
    P2CONST(BswM_ModeRqstPortRuntimeType, AUTOMATIC, BSWM_VAR_CLEARED) modeRqstPtr;
    ApplicationType partIdx;
    boolean result;
    EcuM_WakeupSourceType numOfEcuMWakeupSrc;
    EcuM_WakeupSourceType wkSrcIdx;

    uint8 numOfEcuMRunRqstSource;
    uint8 runRqstIdx;

    result = BswM_GetPartitionIdx(&partIdx);
    if ((boolean)TRUE == result)
    {
        modeRqstPCfgPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPCCfg;
        modeRqstPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPortRunPtr;
        numOfEcuMWakeupSrc = modeRqstPCfgPtr->numOfEcuMWakeupSource;
        for (wkSrcIdx = 0u; wkSrcIdx < numOfEcuMWakeupSrc; wkSrcIdx++)
        {
            if (NULL_PTR != modeRqstPCfgPtr->ecumWkInitstate)
            {
                modeRqstPtr->ecuMWakeupSrcStatus[wkSrcIdx] = *(modeRqstPCfgPtr->ecumWkInitstate);
            }
            else
            {
                break;
            }
        }

        numOfEcuMRunRqstSource = modeRqstPCfgPtr->numOfEcuMRunRqstSource;
        for (runRqstIdx = 0u; runRqstIdx < numOfEcuMRunRqstSource; runRqstIdx++)
        {
            if (NULL_PTR != modeRqstPCfgPtr->ecumRunRqstInitstate)
            {
                modeRqstPtr->ecuMRunRqstStatus[runRqstIdx] = *(modeRqstPCfgPtr->ecumRunRqstInitstate);
            }
            else
            {
                break;
            }
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
 * Function called by EcuM to indicate the current state of a wakeup source.
 * Service ID: 0x10
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): source, wakeup source(s) that changed state
 *                 state, the new state of the wakeup source(s)
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BSWM_ECUM_CODE)
BswM_EcuM_CurrentWakeup(EcuM_WakeupSourceType source, EcuM_WakeupStatusType state)
{
    P2CONST(BswM_ModeRqstPortPCCfgType, AUTOMATIC, BSWM_CONST) modeRqstPCfgPtr;
    P2CONST(BswM_ModeRqstPortLCfgType, AUTOMATIC, BSWM_CONST) modeRqstLCfgPtr;
    P2CONST(BswM_ModeRqstPortRuntimeType, AUTOMATIC, BSWM_VAR_CLEARED) modeRqstPtr;
    P2CONST(BswM_RuleLcCfgType, AUTOMATIC, BSWM_CONST) ruleLCfgPtr;
    BswM_RuleIndexType numOfRules;
    BswM_RuleIndexType idx;
    BswM_RuleIndexType ruleIdx;
    ApplicationType partIdx;
    boolean result;
    EcuM_WakeupSourceType numOfWakeSrc;
    EcuM_WakeupSourceType wkSrcIdx;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkEcuMCurWakeUp(state))
#endif /*BSWM_DEV_ERROR_DETECT == STD_ON*/
    {
        result = BswM_GetPartitionIdx(&partIdx);
        if ((boolean)TRUE == result)
        {
            modeRqstPCfgPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPCCfg;
            modeRqstLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].modeRqstLCfg;
            numOfWakeSrc = modeRqstPCfgPtr->numOfEcuMWakeupSource;
            for (wkSrcIdx = 0u; wkSrcIdx < numOfWakeSrc; wkSrcIdx++)
            {
                if ((source & modeRqstLCfgPtr->ecuMWkSrcRef[wkSrcIdx]) != 0u)
                {
                    modeRqstPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPortRunPtr;
                    modeRqstPtr->ecuMWakeupSrcStatus[wkSrcIdx] = state;
                    if (BSWM_IMMEDIATE == modeRqstLCfgPtr->ecuMWkSrc[wkSrcIdx].process)
                    {
                        numOfRules = modeRqstLCfgPtr->ecuMWkSrc[wkSrcIdx].belongToRlueNum;
                        ruleLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].ruleLCfg;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
                            ruleIdx = modeRqstLCfgPtr->ecuMWkSrc[wkSrcIdx].belongToRlue[idx];
                            if ((boolean)FALSE == ruleLCfgPtr[ruleIdx].isNestExe)
                            {
                                /* An Independent rule,to be evaluated each time applicable*/
                                BswM_ArbitrateRule(ruleIdx, partIdx);
                            }
                        }
                    }
                    break;
                }
            }
        }
    }
}

/*Get specified current wake up source status*/
FUNC(EcuM_WakeupStatusType, BSWM_ECUM_CODE)
BswM_GetEcuMWakeSrcStatus(EcuM_WakeupSourceType wkSrcIdx)
{
    EcuM_WakeupStatusType wkStatus = BSWM_INVALID_U8;
    ApplicationType partIdx;
    boolean result;

    result = BswM_GetPartitionIdx(&partIdx);
    if ((boolean)TRUE == result)
    {
        wkStatus = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPortRunPtr->ecuMWakeupSrcStatus[wkSrcIdx];
    }
    return wkStatus;
}

/******Compatible with lower versions******/

/**
 * Function called by EcuM to notify about current Status of the Run Request
 * Protocol.
 * Service ID: 0x23
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): State, the requested state by EcuMFlex
 *                 CurrentStatus, result of the Run Request Protocol
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BSWM_ECUM_CODE)
BswM_EcuM_RequestedState(EcuM_StateType State, EcuM_RunStatusType CurrentStatus)
{
    P2CONST(BswM_ModeRqstPortPCCfgType, AUTOMATIC, BSWM_CONST) modeRqstPCfgPtr;
    P2CONST(BswM_ModeRqstPortLCfgType, AUTOMATIC, BSWM_CONST) modeRqstLCfgPtr;
    P2CONST(BswM_ModeRqstPortRuntimeType, AUTOMATIC, BSWM_VAR_CLEARED) modeRqstPtr;
    P2CONST(BswM_RuleLcCfgType, AUTOMATIC, BSWM_CONST) ruleLCfgPtr;
    BswM_RuleIndexType numOfRules;
    BswM_RuleIndexType idx;
    BswM_RuleIndexType ruleIdx;
    ApplicationType partIdx;
    boolean result;
    uint8 numOfEcuMRunRqstSource;
    uint8 runRqstIdx;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkRequestedState(State, CurrentStatus))
#endif /*BSWM_DEV_ERROR_DETECT == STD_ON*/
    {
        result = BswM_GetPartitionIdx(&partIdx);
        if ((boolean)TRUE == result)
        {
            modeRqstPCfgPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPCCfg;
            modeRqstLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].modeRqstLCfg;
            numOfEcuMRunRqstSource = modeRqstPCfgPtr->numOfEcuMRunRqstSource;
            for (runRqstIdx = 0u; runRqstIdx < numOfEcuMRunRqstSource; runRqstIdx++)
            {
                if (State == modeRqstLCfgPtr->ecuMRqstIndSrcRef[runRqstIdx])
                {
                    modeRqstPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPortRunPtr;
                    modeRqstPtr->ecuMRunRqstStatus[runRqstIdx] = CurrentStatus;
                    if (BSWM_IMMEDIATE == modeRqstLCfgPtr->ecuMRqstIndSrc[runRqstIdx].process)
                    {
                        numOfRules = modeRqstLCfgPtr->ecuMRqstIndSrc[runRqstIdx].belongToRlueNum;
                        ruleLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].ruleLCfg;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
                            ruleIdx = modeRqstLCfgPtr->ecuMRqstIndSrc[runRqstIdx].belongToRlue[idx];
                            if ((boolean)FALSE == ruleLCfgPtr[ruleIdx].isNestExe)
                            {
                                /* An Independent rule,to be evaluated each time applicable*/
                                BswM_ArbitrateRule(ruleIdx, partIdx);
                            }
                        }
                    }
                }
            }
        }
    }
}

FUNC(EcuM_RunStatusType, BSWM_ECUM_CODE)
BswM_GetEcuMRunRqstStatus(EcuM_StateType rqstIdx)
{
    EcuM_RunStatusType rqstStatus = BSWM_INVALID_U8;
    ApplicationType partIdx;
    boolean result;

    result = BswM_GetPartitionIdx(&partIdx);
    if ((boolean)TRUE == result)
    {
        rqstStatus = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPortRunPtr->ecuMRunRqstStatus[rqstIdx];
    }
    return rqstStatus;
}
/**
 * Function called by EcuM to indicate the current ECU Operation Mode.
 * Service ID: 0x0f
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): CurrentState, the requested ECU Operation Mode
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BSWM_ECUM_CODE)
BswM_EcuM_CurrentState(EcuM_StateType CurrentState)
{
    P2CONST(BswM_ModeRqstPortLCfgType, AUTOMATIC, BSWM_CONST) modeRqstLCfgPtr;
    P2CONST(BswM_RuleLcCfgType, AUTOMATIC, BSWM_CONST) ruleLCfgPtr;
    BswM_RuleIndexType numOfRules;
    BswM_RuleIndexType idx;
    BswM_RuleIndexType ruleIdx;
    ApplicationType partIdx;
    boolean result;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkEcuMCurState())
#endif /*BSWM_DEV_ERROR_DETECT == STD_ON*/
    {
        result = BswM_GetPartitionIdx(&partIdx);
        if ((boolean)TRUE == result)
        {
            BswM_RuntimeStatus.ecumState = CurrentState;
            modeRqstLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].modeRqstLCfg;
            if (BSWM_IMMEDIATE == modeRqstLCfgPtr->ecuMState->process)
            {
                numOfRules = modeRqstLCfgPtr->ecuMState->belongToRlueNum;
                ruleLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].ruleLCfg;
                for (idx = 0u; idx < numOfRules; idx++)
                {
                    ruleIdx = modeRqstLCfgPtr->ecuMState->belongToRlue[idx];
                    if ((boolean)FALSE == ruleLCfgPtr[ruleIdx].isNestExe)
                    {
                        /* An Independent rule,to be evaluated each time applicable*/
                        BswM_ArbitrateRule(ruleIdx, partIdx);
                    }
                }
            }
        }
    }
}

FUNC(EcuM_StateType, BSWM_ECUM_CODE)
BswM_GetEcuMState(void)
{
    return BswM_RuntimeStatus.ecumState;
}

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/

#endif /*BSWM_ECUM_ENABLED == STD_ON*/
