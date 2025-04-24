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
**  FILENAME    : BswM_NvM.c                                                  **
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
#include "BswM_NvM.h"
#if (BSWM_NVM_ENABLED == STD_ON)
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
/*Initialize BswMNvMJobModeIndication and BswMNvMRequest request port*/
FUNC(void, BSWM_NVM_CODE)
BswM_InitNvMRequestPorts(void)
{
    P2CONST(BswM_ModeRqstPortPCCfgType, AUTOMATIC, BSWM_CONST) modeRqstPCfgPtr;
    P2CONST(BswM_ModeRqstPortRuntimeType, AUTOMATIC, BSWM_VAR_CLEARED) modeRqstPtr;
    ApplicationType partIdx;
    NvM_BlockIdType numOfNvMBlock;
    uint8 numOfNvmJob;
    uint8 idx;
    boolean result;

    result = BswM_GetPartitionIdx(&partIdx);
    if ((boolean)TRUE == result)
    {
        modeRqstPCfgPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPCCfg;
        modeRqstPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPortRunPtr;
        numOfNvmJob = modeRqstPCfgPtr->numOfNvMJob;
        if (NULL_PTR != modeRqstPCfgPtr->nvmJobInitMode)
        {
            for (idx = 0u; idx < numOfNvmJob; idx++)
            {
                modeRqstPtr->nvMJobModeIndiStatus[idx] = *(modeRqstPCfgPtr->nvmJobInitMode);
            }
        }
        numOfNvMBlock = modeRqstPCfgPtr->numOfNvMBlock;
        if (NULL_PTR != modeRqstPCfgPtr->nvmInitMode)
        {
            for (idx = 0u; idx < numOfNvMBlock; idx++)
            {
                modeRqstPtr->nvMReqStatus[idx] = *(modeRqstPCfgPtr->nvmInitMode);
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
 * Function called by NvM to inform the BswM about the current state of a multi
 * block job.
 * Service ID: 0x17
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): ServiceId, Indicates whether the callback refers to multi
 *                            block services NvM_ReadAll or NvM_WriteAll
 *                 CurrentJobMode, Current state of the multi block job
 *                            indicated by parameter ServiceId
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BSWM_NVM_CODE)
BswM_NvM_CurrentJobMode(NvM_MultiBlockRequestType MultiBlockRequest, NvM_RequestResultType CurrentJobMode)
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
    uint8 numOfNvmJob;
    uint8 jobIdx;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkCurNvmJobMode(MultiBlockRequest))
#endif /*BSWM_DEV_ERROR_DETECT == STD_ON*/
    {
        result = BswM_GetPartitionIdx(&partIdx);
        if ((boolean)TRUE == result)
        {
            modeRqstPCfgPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPCCfg;
            modeRqstLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].modeRqstLCfg;
            numOfNvmJob = modeRqstPCfgPtr->numOfNvMJob;
            for (jobIdx = 0u; jobIdx < numOfNvmJob; jobIdx++)
            {
                if (MultiBlockRequest == modeRqstLCfgPtr->nvmService[jobIdx])
                {
                    modeRqstPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPortRunPtr;
                    modeRqstPtr->nvMJobModeIndiStatus[jobIdx] = CurrentJobMode;
                    if (BSWM_IMMEDIATE == modeRqstLCfgPtr->nvmJobInd[jobIdx].process)
                    {
                        numOfRules = modeRqstLCfgPtr->nvmJobInd[jobIdx].belongToRlueNum;
                        ruleLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].ruleLCfg;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
                            ruleIdx = modeRqstLCfgPtr->nvmJobInd[jobIdx].belongToRlue[idx];
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
 * Function called by NvM to indicate the current block mode of an NvM block.
 * To use this function integration code will be needed.
 * Service ID: 0x16
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): Block, The Block that the new NvM Mode corresponds to
 *                 CurrentBlockMode, The current block mode of the NvM block
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BSWM_NVM_CODE)
BswM_NvM_CurrentBlockMode(NvM_BlockIdType Block, NvM_RequestResultType CurrentBlockMode)
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
    NvM_BlockIdType numOfNvMBlock;
    NvM_BlockIdType bloIdx;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkCurNvmBlockMode(CurrentBlockMode))
#endif /*BSWM_DEV_ERROR_DETECT == STD_ON*/
    {
        result = BswM_GetPartitionIdx(&partIdx);
        if ((boolean)TRUE == result)
        {
            modeRqstPCfgPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPCCfg;
            modeRqstLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].modeRqstLCfg;
            numOfNvMBlock = modeRqstPCfgPtr->numOfNvMBlock;
            for (bloIdx = 0u; bloIdx < numOfNvMBlock; bloIdx++)
            {
                if (Block == modeRqstLCfgPtr->nvmBlockRef[bloIdx])
                {
                    modeRqstPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPortRunPtr;
                    modeRqstPtr->nvMReqStatus[bloIdx] = CurrentBlockMode;
                    if (BSWM_IMMEDIATE == modeRqstLCfgPtr->nvmRqst[bloIdx].process)
                    {
                        numOfRules = modeRqstLCfgPtr->nvmRqst[bloIdx].belongToRlueNum;
                        ruleLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].ruleLCfg;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
                            ruleIdx = modeRqstLCfgPtr->nvmRqst[bloIdx].belongToRlue[idx];
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

/*Get nvm current block mode*/
FUNC(NvM_RequestResultType, BSWM_NVM_CODE)
BswM_GetCurNvmBlockMode(uint8 blockIdx)
{
    NvM_RequestResultType blockMode = NVM_REQ_OK;
    ApplicationType partIdx;
    boolean result;

    result = BswM_GetPartitionIdx(&partIdx);
    if ((boolean)TRUE == result)
    {
        blockMode = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPortRunPtr->nvMReqStatus[blockIdx];
    }
    return blockMode;
}

/*Get nvm current Job mode*/
FUNC(NvM_RequestResultType, BSWM_NVM_CODE)
BswM_GetCurNvmJobMode(uint8 jobIdx)
{
    NvM_RequestResultType blockMode = NVM_REQ_OK;
    ApplicationType partIdx;
    boolean result;

    result = BswM_GetPartitionIdx(&partIdx);
    if ((boolean)TRUE == result)
    {
        blockMode = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPortRunPtr->nvMJobModeIndiStatus[jobIdx];
    }
    return blockMode;
}
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/

#endif /*BSWM_NVM_ENABLED == STD_ON*/
