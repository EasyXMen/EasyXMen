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
 *
 ********************************************************************************
 **                                                                            **
 **  FILENAME    : BswM_ComM.c                                                 **
 **                                                                            **
 **  Created on  : 2020-03-24                                                  **
 **  Author      : qinchun.yang                                                **
 **  Vendor      :                                                             **
 **  DESCRIPTION :                                                             **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
 **                                                                            **
 *******************************************************************************/
/* PRQA S 3108-- */

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "BswM_ComM.h"
#if (BSWM_COMM_ENABLED == STD_ON)
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
/*Initialize BswMComMIndication and BswMComMPncRequest request port*/
FUNC(void, BSWM_COMM_CODE)
BswM_InitComMRequestPorts(void)
{
    P2CONST(BswM_ModeRqstPortPCCfgType, AUTOMATIC, BSWM_CONST) modeRqstPCfgPtr;
    P2CONST(BswM_ModeRqstPortRuntimeType, AUTOMATIC, BSWM_VAR_CLEARED) modeRqstPtr;
    P2CONST(BswM_PartitionPCCfgType, TYPEDEF, BSWM_CONST) bswmPartPCCfgs;
    NetworkHandleType numOfComMCh;
    NetworkHandleType idx;
#if (BSWM_COMM_PNC_ENABLED == STD_ON)
    PNCHandleType numOfPnc;
    PNCHandleType pncIdx;
#endif /* BSWM_COMM_PNC_ENABLED == STD_ON */
    ApplicationType partIdx;
    boolean result;

    result = BswM_GetPartitionIdx(&partIdx);
    if ((boolean)TRUE == result)
    {
        bswmPartPCCfgs = &(BswM_RuntimeStatus.bswmPartPCCfgs[partIdx]);
        modeRqstPCfgPtr = bswmPartPCCfgs->modeRqstPCCfg;
        modeRqstPtr = bswmPartPCCfgs->modeRqstPortRunPtr;
        numOfComMCh = modeRqstPCfgPtr->numOfComMInd;
        for (idx = 0u; idx < numOfComMCh; idx++)
        {
            if (NULL_PTR != modeRqstPCfgPtr->comMInitMode)
            {
                modeRqstPtr->comMIndiStatus[idx] = *(modeRqstPCfgPtr->comMInitMode);
            }
            else
            {
                break;
            }
        }
#if (BSWM_COMM_PNC_ENABLED == STD_ON)
        numOfPnc = modeRqstPCfgPtr->numOfComMPncInd;
        for (pncIdx = 0u; pncIdx < numOfPnc; pncIdx++)
        {
            if (NULL_PTR != modeRqstPCfgPtr->comMPncInitMode)
            {
                modeRqstPtr->comMPncReqStatus[pncIdx] = *(modeRqstPCfgPtr->comMPncInitMode);
            }
            else
            {
                break;
            }
        }
#endif /* BSWM_COMM_PNC_ENABLED == STD_ON */
    }
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    else
    {
        (void)Det_ReportError(BSWM_MODULE_ID, BSWM_INSTANCE_ID, BSWM_API_ID_INIT, BSWM_E_INIT_FAILED);
    }
#endif /* BSWM_DEV_ERROR_DETECT == STD_ON */
}

/**
 * Function called by ComM to signal a shutdown.
 * Service ID: 0x22
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BSWM_COMM_CODE)
BswM_ComM_InitiateReset(void)
{
#if (BSWM_EVENT_RQSTPORT_ENABLE == STD_ON)
    P2CONST(BswM_EventRqstPortLCfgType, AUTOMATIC, BSWM_CONST) evRqstLCfgPtr;
    /* PRQA S 3432 ++*/ /* MISRA Rule 20.7 */
    P2VAR(BswM_EventRquestPortRuntimeType, AUTOMATIC, BSWM_VAR_CLEARED) evRqstPtr;
    /* PRQA S 3432 --*/ /* MISRA Rule 20.7 */
    P2CONST(BswM_RuleLcCfgType, AUTOMATIC, BSWM_CONST) ruleLCfgPtr;
    P2CONST(BswM_PartitionPCCfgType, TYPEDEF, BSWM_CONST) bswmPartPCCfgs;
    P2CONST(BswM_PartitionLCfgType, TYPEDEF, BSWM_CONST) bswmPartLCfgs;
    BswM_RuleIndexType numOfRules;
    BswM_RuleIndexType idx;
    BswM_RuleIndexType ruleIdx;
    BswM_EventRqstPortIdxType evIdx;
    ApplicationType partIdx;
    boolean result;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkComMInitRst())
#endif /* BSWM_DEV_ERROR_DETECT == STD_ON */
    {
        result = BswM_GetPartitionIdx(&partIdx);
        if ((boolean)TRUE == result)
        {
            bswmPartPCCfgs = &(BswM_RuntimeStatus.bswmPartPCCfgs[partIdx]);
            bswmPartLCfgs = &(BswM_RuntimeStatus.bswmPartLCfgs[partIdx]);
            evRqstPtr = bswmPartPCCfgs->eventRqstPortRunPtr;
            evIdx = *(bswmPartPCCfgs->evRqstPCCfg->comMInitRstIdxPtr);
            evRqstPtr[evIdx] = BSWM_EVENT_IS_SET;
            evRqstLCfgPtr = bswmPartLCfgs->evRqstLCfg;
            if (BSWM_IMMEDIATE == evRqstLCfgPtr->comMInitReset->process)
            {
                numOfRules = evRqstLCfgPtr->comMInitReset->belongToRlueNum;
                ruleLCfgPtr = bswmPartLCfgs->ruleLCfg;
                for (idx = 0u; idx < numOfRules; idx++)
                {
                    ruleIdx = evRqstLCfgPtr->comMInitReset->belongToRlue[idx];
                    if ((boolean)FALSE == ruleLCfgPtr[ruleIdx].isNestExe)
                    {
                        /* An Independent rule,to be evaluated each time applicable*/
                        BswM_ArbitrateRule(ruleIdx, partIdx);
                    }
                }
            }
        }
    }
#endif /* BSWM_EVENT_RQSTPORT_ENABLE == STD_ON */
}

/**
 * Function called by ComM to indicate the current communication mode of a ComM
 * channel.
 * Service ID: 0x0e
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): Network, the ComM communication channel that the indicated
 *                          state corresponds to
 *                 RequestedMode, the current state of the ComM communication channel
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BSWM_COMM_CODE)
BswM_ComM_CurrentMode(NetworkHandleType Network, ComM_ModeType RequestedMode)
{
    P2CONST(BswM_ModeRqstPortPCCfgType, AUTOMATIC, BSWM_CONST) modeRqstPCfgPtr;
    P2CONST(BswM_ModeRqstPortLCfgType, AUTOMATIC, BSWM_CONST) modeRqstLCfgPtr;
    P2CONST(BswM_ModeRqstPortRuntimeType, AUTOMATIC, BSWM_VAR_CLEARED) modeRqstPtr;
    P2CONST(BswM_RuleLcCfgType, AUTOMATIC, BSWM_CONST) ruleLCfgPtr;
    P2CONST(BswM_PartitionPCCfgType, TYPEDEF, BSWM_CONST) bswmPartPCCfgs;
    P2CONST(BswM_PartitionLCfgType, TYPEDEF, BSWM_CONST) bswmPartLCfgs;
    BswM_RuleIndexType numOfRules;
    BswM_RuleIndexType idx;
    BswM_RuleIndexType ruleIdx;
    ApplicationType partIdx;
    boolean result;
    NetworkHandleType numOfComMCh;
    NetworkHandleType chIdx;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkComMInd(RequestedMode))
#endif /* BSWM_DEV_ERROR_DETECT == STD_ON */
    {
        result = BswM_GetPartitionIdx(&partIdx);
        if ((boolean)TRUE == result)
        {
            bswmPartPCCfgs = &(BswM_RuntimeStatus.bswmPartPCCfgs[partIdx]);
            bswmPartLCfgs = &(BswM_RuntimeStatus.bswmPartLCfgs[partIdx]);
            modeRqstPCfgPtr = bswmPartPCCfgs->modeRqstPCCfg;
            modeRqstLCfgPtr = bswmPartLCfgs->modeRqstLCfg;
            numOfComMCh = modeRqstPCfgPtr->numOfComMInd;
            for (chIdx = 0u; chIdx < numOfComMCh; chIdx++)
            {
                if (Network == modeRqstLCfgPtr->comMIndChRef[chIdx])
                {
                    modeRqstPtr = bswmPartPCCfgs->modeRqstPortRunPtr;
                    modeRqstPtr->comMIndiStatus[chIdx] = RequestedMode;
                    if (BSWM_IMMEDIATE == modeRqstLCfgPtr->comMInd[chIdx].process)
                    {
                        numOfRules = modeRqstLCfgPtr->comMInd[chIdx].belongToRlueNum;
                        ruleLCfgPtr = bswmPartLCfgs->ruleLCfg;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
                            ruleIdx = modeRqstLCfgPtr->comMInd[chIdx].belongToRlue[idx];
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

#if (BSWM_COMM_PNC_ENABLED == STD_ON)
/**
 * Function called by ComM to indicate the current mode of the PNC.
 * Service ID: 0x15
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): PNC, the handle of the PNC for which the current state is reported
 *                 CurrentPncMode, the current mode of the PNC
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BSWM_COMM_CODE)
BswM_ComM_CurrentPNCMode(PNCHandleType PNC, ComM_PncModeType CurrentPncMode)
{
    P2CONST(BswM_ModeRqstPortPCCfgType, AUTOMATIC, BSWM_CONST) modeRqstPCfgPtr;
    P2CONST(BswM_ModeRqstPortLCfgType, AUTOMATIC, BSWM_CONST) modeRqstLCfgPtr;
    P2CONST(BswM_ModeRqstPortRuntimeType, AUTOMATIC, BSWM_VAR_CLEARED) modeRqstPtr;
    P2CONST(BswM_RuleLcCfgType, AUTOMATIC, BSWM_CONST) ruleLCfgPtr;
    P2CONST(BswM_PartitionPCCfgType, TYPEDEF, BSWM_CONST) bswmPartPCCfgs;
    P2CONST(BswM_PartitionLCfgType, TYPEDEF, BSWM_CONST) bswmPartLCfgs;
    BswM_RuleIndexType numOfRules;
    BswM_RuleIndexType idx;
    BswM_RuleIndexType ruleIdx;
    ApplicationType partIdx;
    boolean result;
    PNCHandleType numOfComMPncInd;
    PNCHandleType pncIdx;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkComMPnc(CurrentPncMode))
#endif /* BSWM_DEV_ERROR_DETECT == STD_ON */
    {
        result = BswM_GetPartitionIdx(&partIdx);
        if ((boolean)TRUE == result)
        {
            bswmPartPCCfgs = &(BswM_RuntimeStatus.bswmPartPCCfgs[partIdx]);
            bswmPartLCfgs = &(BswM_RuntimeStatus.bswmPartLCfgs[partIdx]);
            modeRqstPCfgPtr = bswmPartPCCfgs->modeRqstPCCfg;
            modeRqstLCfgPtr = bswmPartLCfgs->modeRqstLCfg;
            numOfComMPncInd = modeRqstPCfgPtr->numOfComMPncInd;
            for (pncIdx = 0u; pncIdx < numOfComMPncInd; pncIdx++)
            {
                if (PNC == modeRqstLCfgPtr->comMPncRef[pncIdx])
                {
                    modeRqstPtr = bswmPartPCCfgs->modeRqstPortRunPtr;
                    modeRqstPtr->comMPncReqStatus[pncIdx] = CurrentPncMode;
                    if (BSWM_IMMEDIATE == modeRqstLCfgPtr->comMPncRqst[pncIdx].process)
                    {
                        numOfRules = modeRqstLCfgPtr->comMPncRqst[pncIdx].belongToRlueNum;
                        ruleLCfgPtr = bswmPartLCfgs->ruleLCfg;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
                            ruleIdx = modeRqstLCfgPtr->comMPncRqst[pncIdx].belongToRlue[idx];
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

/*Get comM initiateReset status*/
FUNC(BswM_EventRquestPortRuntimeType, BSWM_COMM_CODE)
BswM_GetComMResetInitStatus(void)
{
    P2CONST(BswM_PartitionPCCfgType, TYPEDEF, BSWM_CONST) bswmPartPCCfgs;
    BswM_EventRquestPortRuntimeType comMInitStatus = BSWM_INVALID_U8;
    BswM_EventRqstPortIdxType evIdx;
    ApplicationType partIdx;
    boolean result;

    result = BswM_GetPartitionIdx(&partIdx);
    if ((boolean)TRUE == result)
    {
        bswmPartPCCfgs = &(BswM_RuntimeStatus.bswmPartPCCfgs[partIdx]);
        evIdx = *(bswmPartPCCfgs->evRqstPCCfg->comMInitRstIdxPtr);
        comMInitStatus = bswmPartPCCfgs->eventRqstPortRunPtr[evIdx];
    }
    return comMInitStatus;
}

/*Get ComM Pnc status*/
FUNC(ComM_PncModeType, BSWM_COMM_CODE)
BswM_GetComMPncStatus(PNCHandleType pncIdx)
{
    ComM_PncModeType pncStatus = COMM_PNC_NO_COMMUNICATION;
    ApplicationType partIdx;
    boolean result;

    result = BswM_GetPartitionIdx(&partIdx);
    if ((boolean)TRUE == result)
    {
        pncStatus = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPortRunPtr->comMPncReqStatus[pncIdx];
    }

    return pncStatus;
}
#endif /* BSWM_COMM_PNC_ENABLED == STD_ON */

/*Get ComM mode status*/
FUNC(ComM_ModeType, BSWM_COMM_CODE)
BswM_GetComMIndStatus(NetworkHandleType comChIdx)
{
    ComM_ModeType comMode = BSWM_INVALID_U8;
    ApplicationType partIdx;
    boolean result;

    result = BswM_GetPartitionIdx(&partIdx);
    if ((boolean)TRUE == result)
    {
        comMode = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPortRunPtr->comMIndiStatus[comChIdx];
    }
    return comMode;
}

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/

#endif /* BSWM_COMM_ENABLED == STD_ON */
