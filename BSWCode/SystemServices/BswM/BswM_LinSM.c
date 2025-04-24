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
**  FILENAME    : BswM_LinSM.c                                                **
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
#include "BswM_LinSM.h"
#if (BSWM_LINSM_ENABLED == STD_ON)
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
/*Initialize BswMLinSMIndication and BswMLinScheduleIndication request port*/
FUNC(void, BSWM_LINSM_CODE)
BswM_InitLinSMRequestPorts(void)
{
    P2CONST(BswM_ModeRqstPortPCCfgType, AUTOMATIC, BSWM_CONST) modeRqstPCfgPtr;
    P2CONST(BswM_ModeRqstPortRuntimeType, AUTOMATIC, BSWM_VAR_CLEARED) modeRqstPtr;
    ApplicationType partIdx;
    boolean result;
    NetworkHandleType numOfLinSMInd;
    NetworkHandleType linsmIdx;

    result = BswM_GetPartitionIdx(&partIdx);
    if ((boolean)TRUE == result)
    {
        modeRqstPCfgPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPCCfg;
        modeRqstPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPortRunPtr;
        numOfLinSMInd = modeRqstPCfgPtr->numOfLinSMInd;
        for (linsmIdx = 0u; linsmIdx < numOfLinSMInd; linsmIdx++)
        {
            if (NULL_PTR != modeRqstPCfgPtr->linsmInitState)
            {
                modeRqstPtr->linSMIndiStatus[linsmIdx] = *(modeRqstPCfgPtr->linsmInitState);
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
 * Function called by LinSM to indicate the currently active schedule table for
 * a specific LIN channel.
 * Service ID: 0x0a
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): Network, The LIN channel that the schedule table switch have
 *                          occurred on.
 *                 CurrentSchedule, The currently active schedule table of the
 *                                  LIN channel
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BSWM_LINSM_CODE)
BswM_LinSM_CurrentSchedule(NetworkHandleType Network, LinIf_SchHandleType CurrentSchedule)
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
    NetworkHandleType numOfLinsmCh;
    NetworkHandleType chIdx;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkLinSMCurSch())
#endif /*BSWM_DEV_ERROR_DETECT == STD_ON*/
    {
        result = BswM_GetPartitionIdx(&partIdx);
        if ((boolean)TRUE == result)
        {
            modeRqstPCfgPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPCCfg;
            modeRqstLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].modeRqstLCfg;
            if (0u != modeRqstPCfgPtr->numOfLinSchInd)
            {
                numOfLinsmCh = modeRqstPCfgPtr->numOfLinSchInd;
            }
            else
            {
                numOfLinsmCh = 0;
            }

            for (chIdx = 0u; chIdx < numOfLinsmCh; chIdx++)
            {
                if ((Network == modeRqstLCfgPtr->linSchNKChRef[chIdx])
                    && (CurrentSchedule == modeRqstLCfgPtr->linSchChRef[chIdx]))
                {
                    modeRqstPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPortRunPtr;
                    modeRqstPtr->linSchIndiStatus[chIdx] = CurrentSchedule;
                    if (BSWM_IMMEDIATE == modeRqstLCfgPtr->linSchInd[chIdx].process)
                    {
                        numOfRules = modeRqstLCfgPtr->linSchInd[chIdx].belongToRlueNum;
                        ruleLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].ruleLCfg;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
                            ruleIdx = modeRqstLCfgPtr->linSchInd[chIdx].belongToRlue[idx];
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

/**
 * Function called by LinSM to indicate its current state.
 * Service ID: 0x09
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): Network, The LIN channel that the indicated state corresponds to
 *                 CurrentState, The current state of the LIN channel
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BSWM_LINSM_CODE)
BswM_LinSM_CurrentState(NetworkHandleType Network, LinSM_ModeType CurrentState)
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
    NetworkHandleType numOfLinsmCh;
    NetworkHandleType chIdx;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkLinSMInd(CurrentState))
#endif /*BSWM_DEV_ERROR_DETECT == STD_ON*/
    {
        result = BswM_GetPartitionIdx(&partIdx);
        if ((boolean)TRUE == result)
        {
            modeRqstPCfgPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPCCfg;
            modeRqstLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].modeRqstLCfg;
            numOfLinsmCh = modeRqstPCfgPtr->numOfLinSMInd;
            for (chIdx = 0u; chIdx < numOfLinsmCh; chIdx++)
            {
                if (Network == modeRqstLCfgPtr->linsmIndChRef[chIdx])
                {
                    modeRqstPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPortRunPtr;
                    modeRqstPtr->linSMIndiStatus[chIdx] = CurrentState;
                    if (BSWM_IMMEDIATE == modeRqstLCfgPtr->linsmInd[chIdx].process)
                    {
                        numOfRules = modeRqstLCfgPtr->linsmInd[chIdx].belongToRlueNum;
                        ruleLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].ruleLCfg;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
                            ruleIdx = modeRqstLCfgPtr->linsmInd[chIdx].belongToRlue[idx];
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

/*Get linsm indicate status*/
FUNC(LinSM_ModeType, BSWM_FRSM_CODE)
BswM_GetLinSmIndStatus(NetworkHandleType linsmChIdx)
{
    LinSM_ModeType retStatus = BSWM_INVALID_U8;
    ApplicationType partIdx;
    boolean result;

    result = BswM_GetPartitionIdx(&partIdx);
    if ((boolean)TRUE == result)
    {
        retStatus = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPortRunPtr->linSMIndiStatus[linsmChIdx];
    }
    return retStatus;
}

/*Get linsm current schedule*/
FUNC(LinIf_SchHandleType, BSWM_FRSM_CODE)
BswM_GetLinSmCurSchedule(NetworkHandleType linsmChIdx)
{
    LinIf_SchHandleType curSch = BSWM_INVALID_U8;
    ApplicationType partIdx;
    boolean result;

    result = BswM_GetPartitionIdx(&partIdx);
    if ((boolean)TRUE == result)
    {
        curSch = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPortRunPtr->linSchIndiStatus[linsmChIdx];
    }
    return curSch;
}
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/

#endif /*BSWM_LINSM_ENABLED == STD_ON*/
