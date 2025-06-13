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
**  FILENAME    : BswM_CanSM.c                                                **
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
#include "BswM_CanSM.h"
#if (BSWM_CANSM_ENABLED == STD_ON)
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
/*Initialize BswMCanSMIndication and BswMCanSMIcomIndication request port*/
FUNC(void, BSWM_CANSM_CODE)
BswM_InitCanSMRequestPorts(void)
{
    P2CONST(BswM_ModeRqstPortPCCfgType, AUTOMATIC, BSWM_CONST) modeRqstPCfgPtr;
    P2CONST(BswM_ModeRqstPortRuntimeType, AUTOMATIC, BSWM_VAR_CLEARED) modeRqstPtr;
    NetworkHandleType numOfCanSMInd;
    NetworkHandleType idx;
#if (BSWM_CANSM_ICOM_ENABLED == STD_ON)
    uint8 numOfIcom;
    uint8 icomIdx;
#endif /*BSWM_CANSM_ICOM_ENABLED == STD_ON*/
    ApplicationType partIdx;
    boolean result;

    result = BswM_GetPartitionIdx(&partIdx);
    if ((boolean)TRUE == result)
    {
        modeRqstPCfgPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPCCfg;
        modeRqstPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPortRunPtr;
        numOfCanSMInd = modeRqstPCfgPtr->numOfCanSMInd;
        for (idx = 0u; idx < numOfCanSMInd; idx++)
        {
            if (NULL_PTR != modeRqstPCfgPtr->canSMInitState)
            {
                modeRqstPtr->canSMIndiStatus[idx] = *(modeRqstPCfgPtr->canSMInitState);
            }
            else
            {
                break;
            }
        }
#if (BSWM_CANSM_ICOM_ENABLED == STD_ON)
        numOfIcom = modeRqstPCfgPtr->numOfCanSMIcomInd;
        for (icomIdx = 0u; icomIdx < numOfIcom; icomIdx++)
        {
            if (NULL_PTR != modeRqstPCfgPtr->icomInitId)
            {
                modeRqstPtr->canSMIcomIndiStatus[icomIdx] = *(modeRqstPCfgPtr->icomInitId);
            }
            else
            {
                break;
            }
        }
#endif /*BSWM_CANSM_ICOM_ENABLED == STD_ON*/
    }
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    else
    {
        (void)Det_ReportError(BSWM_MODULE_ID, BSWM_INSTANCE_ID, BSWM_API_ID_INIT, BSWM_E_INIT_FAILED);
    }
#endif /*BSWM_DEV_ERROR_DETECT == STD_ON*/
}

/**
 * Function called by CanSM to indicate its current state.
 * Service ID: 0x05
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): Network, the CAN channel the indicated state corresponds to
 *                 CurrentState, the current state of the CAN channel
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BSWM_CANSM_CODE)
BswM_CanSM_CurrentState(NetworkHandleType Network, CanSM_BswMCurrentStateType CurrentState)
{
    P2CONST(BswM_ModeRqstPortPCCfgType, AUTOMATIC, BSWM_CONST) modeRqstPCfgPtr;
    P2CONST(BswM_ModeRqstPortLCfgType, AUTOMATIC, BSWM_CONST) modeRqstLCfgPtr;
    P2CONST(BswM_ModeRqstPortRuntimeType, AUTOMATIC, BSWM_VAR_CLEARED) modeRqstPtr;
    P2CONST(BswM_RuleLcCfgType, AUTOMATIC, BSWM_CONST) ruleLCfgPtr;
    NetworkHandleType numOfCansmCh;
    NetworkHandleType chIdx;
    BswM_RuleIndexType numOfRules;
    BswM_RuleIndexType idx;
    BswM_RuleIndexType ruleIdx;
    ApplicationType partIdx;
    boolean result;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkCanSMInd(CurrentState))
#endif /*BSWM_DEV_ERROR_DETECT == STD_ON*/
    {
        result = BswM_GetPartitionIdx(&partIdx);
        if ((boolean)TRUE == result)
        {
            modeRqstPCfgPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPCCfg;
            modeRqstLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].modeRqstLCfg;
            numOfCansmCh = modeRqstPCfgPtr->numOfCanSMInd;
            for (chIdx = 0u; chIdx < numOfCansmCh; chIdx++)
            {
                if (Network == modeRqstLCfgPtr->cansmIndChRef[chIdx])
                {
                    modeRqstPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPortRunPtr;
                    modeRqstPtr->canSMIndiStatus[chIdx] = CurrentState;
                    if (BSWM_IMMEDIATE == modeRqstLCfgPtr->cansmInd[chIdx].process)
                    {
                        numOfRules = modeRqstLCfgPtr->cansmInd[chIdx].belongToRlueNum;
                        ruleLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].ruleLCfg;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
                            ruleIdx = modeRqstLCfgPtr->cansmInd[chIdx].belongToRlue[idx];
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

#if (BSWM_CANSM_ICOM_ENABLED == STD_ON)
/**
 * Function to inform BswM about the switch of Icom Configuration.
 * The interface shall be used by the CanSM to inform the BswM about the
 * activation of a new ICOM configuration for a given channel.The BswM shall match
 * the Network and Error parameters to a configured BswMCanSMIcomIndication and
 * utilize the ActiveConfiguration parameter as the value that gets evaluated in
 * the arbitration rules.
 * Service ID: 0x1a
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): Network, the CAN channel the requested state corresponds to
 *                 ActiveConfiguration, the configuration Id of the Icom configuration
 *                 Error, ICOM_SWITCH_E_OK: No Error
 *                        ICOM_SWITCH_E_FAILED: Switch to requested Configuration failed.
 *                                              Severe Error.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BSWM_CANSM_CODE)
BswM_CanSM_CurrentIcomConfiguration(
    NetworkHandleType Network,
    IcomConfigIdType ActiveConfiguration,
    IcomSwitch_ErrorType Error)
{
    P2CONST(BswM_ModeRqstPortPCCfgType, AUTOMATIC, BSWM_CONST) modeRqstPCfgPtr;
    P2CONST(BswM_ModeRqstPortLCfgType, AUTOMATIC, BSWM_CONST) modeRqstLCfgPtr;
    P2CONST(BswM_ModeRqstPortRuntimeType, AUTOMATIC, BSWM_VAR_CLEARED) modeRqstPtr;
    P2CONST(BswM_RuleLcCfgType, AUTOMATIC, BSWM_CONST) ruleLCfgPtr;
    NetworkHandleType numOfIcomCh;
    NetworkHandleType chIdx;
    BswM_RuleIndexType numOfRules;
    BswM_RuleIndexType idx;
    BswM_RuleIndexType ruleIdx;
    ApplicationType partIdx;
    boolean result;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkCanSMIcom(Network))
#endif /*BSWM_DEV_ERROR_DETECT == STD_ON*/
    {
        result = BswM_GetPartitionIdx(&partIdx);
        if ((boolean)TRUE == result)
        {
            modeRqstPCfgPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPCCfg;
            modeRqstLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].modeRqstLCfg;
            numOfIcomCh = modeRqstPCfgPtr->numOfCanSMIcomInd;
            for (chIdx = 0u; chIdx < numOfIcomCh; chIdx++)
            {
                if ((Network == modeRqstLCfgPtr->cansmIcomChRef[chIdx])
                    && (Error == modeRqstLCfgPtr->cansmIcomSwichErrRef[chIdx]))
                {
                    modeRqstPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPortRunPtr;
                    modeRqstPtr->canSMIcomIndiStatus[chIdx] = ActiveConfiguration;
                    if (BSWM_IMMEDIATE == modeRqstLCfgPtr->cansmIcom[chIdx].process)
                    {
                        numOfRules = modeRqstLCfgPtr->cansmIcom[chIdx].belongToRlueNum;
                        ruleLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].ruleLCfg;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
                            ruleIdx = modeRqstLCfgPtr->cansmIcom[chIdx].belongToRlue[idx];
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

/*Get cansm icom configuration index*/
FUNC(IcomConfigIdType, BSWM_CANSM_CODE)
BswM_GetCanSmIcomCfgId(NetworkHandleType icomIdx)
{
    IcomConfigIdType icomCfgId = BSWM_INVALID_U8;
    ApplicationType partIdx;
    boolean result;

    result = BswM_GetPartitionIdx(&partIdx);
    if ((boolean)TRUE == result)
    {
        icomCfgId = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPortRunPtr->canSMIcomIndiStatus[icomIdx];
    }
    return icomCfgId;
}
#endif /*BSWM_CANSM_ICOM_ENABLED == STD_ON*/

/*Get cansm indicate status*/
FUNC(CanSM_BswMCurrentStateType, BSWM_CANSM_CODE)
BswM_GetCanSmIndStatus(NetworkHandleType cansmChIdx)
{
    CanSM_BswMCurrentStateType retStatu = CANSM_BSWM_NO_COMMUNICATION;
    ApplicationType partIdx;
    boolean result;

    result = BswM_GetPartitionIdx(&partIdx);
    if ((boolean)TRUE == result)
    {
        retStatu = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPortRunPtr->canSMIndiStatus[cansmChIdx];
    }
    return retStatu;
}
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/

#endif /*BSWM_CANSM_ENABLED == STD_ON*/
