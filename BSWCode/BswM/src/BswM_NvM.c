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
 **  @file               : BswM_NvM.c
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : BSW Mode Manager
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "BswM_NvM.h"
#include "BswM_Internal.h"
#if (BSWM_NVM_ENABLED == STD_ON)
/* PRQA S 0553 EOF */ /* VL_BswM_0553 */
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
/**
 * Initializes the NvM request ports based on the configuration settings.
 * This function retrieves the partition configuration and initializes the NvM job mode indication status and NvM
 * request status arrays. It handles both NvM job mode indication and NvM request initialization if the respective
 * features are enabled. If the configuration retrieval fails, it reports an error if development error detection is
 * enabled.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_InitNvMRequestPorts(void)
/* PRQA S 1503,1505,1532 --*/
{
#if ((BSWM_NVMJOBMODEINDICATION_ENABLED == STD_ON) || (BSWM_NVMREQUEST_ENABLED == STD_ON))
    const BswM_PartitionPCCfgType*      bswmPartPCCfgPtr;
    const BswM_ModeRqstPortPCCfgType*   modeRqstPCfgPtr;
    const BswM_ModeRqstPortRuntimeType* modeRqstPtr;
    uint8                               idx;
    boolean                             result;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        modeRqstPCfgPtr = bswmPartPCCfgPtr->modeRqstPCCfg;
        modeRqstPtr     = bswmPartPCCfgPtr->modeRqstPortRunPtr;
#if (BSWM_NVMJOBMODEINDICATION_ENABLED == STD_ON)
        uint16 numOfNvmJob;
        numOfNvmJob = modeRqstPCfgPtr->numOfNvMJob;
        if (NULL_PTR != modeRqstPCfgPtr->nvmJobInitMode)
        {
            for (idx = 0u; idx < numOfNvmJob; idx++)
            {
                modeRqstPtr->nvMJobModeIndiStatus[idx] = *(modeRqstPCfgPtr->nvmJobInitMode);
            }
        }
#endif /* BSWM_NVMJOBMODEINDICATION_ENABLED == STD_ON */
#if (BSWM_NVMREQUEST_ENABLED == STD_ON)
        NvM_BlockIdType numOfNvMBlock;
        numOfNvMBlock = modeRqstPCfgPtr->numOfNvMBlock;
        if (NULL_PTR != modeRqstPCfgPtr->nvmInitMode)
        {
            for (idx = 0u; idx < numOfNvMBlock; idx++)
            {
                modeRqstPtr->nvMReqStatus[idx] = *(modeRqstPCfgPtr->nvmInitMode);
            }
        }
#endif /* BSWM_NVMREQUEST_ENABLED == STD_ON */
    }
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    else
    {
        (void)Det_ReportError(BSWM_MODULE_ID, BSWM_INSTANCE_ID, BSWM_API_ID_INIT, BSWM_E_INIT_FAILED);
    }
#endif /*BSWM_DEV_ERROR_DETECT == STD_ON*/
#endif
}

/**
 * Updates the current job mode for a specific NvM multi-block request.
 * This function retrieves the partition configuration and updates the NvM job mode indication status.
 * It also evaluates associated rules if the job is configured to process immediately.
 * Error checking is performed if development error detection is enabled.
 */
/* PRQA S 1503,1505,1532,6030 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef, VL_MTR_BswM_STMIF */
void BswM_NvM_CurrentJobMode(NvM_MultiBlockRequestType MultiBlockRequest, NvM_RequestResultType CurrentJobMode)
/* PRQA S 1503,1505,1532,6030 --*/
{
#if (BSWM_NVMJOBMODEINDICATION_ENABLED == STD_ON)
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
    const BswM_PartitionPbConfigType* bswmPartPbConfigPtr;
#endif /* BSWM_PB_CONFIG_ENABLED */
    const BswM_PartitionLCfgType*       bswmPartLCfgPtr;
    const BswM_ModeRqstPortPCCfgType*   modeRqstPCfgPtr;
    const BswM_ModeRqstPortLCfgType*    modeRqstLCfgPtr;
    const BswM_ModeRqstPortRuntimeType* modeRqstPtr;
    BswM_RuleIndexType                  numOfRules;
    BswM_RuleIndexType                  idx;
    BswM_RuleIndexType                  ruleIdx;
    boolean                             result;
    uint16                              numOfNvmJob;
    uint16                              jobIdx;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkCurNvmJobMode(MultiBlockRequest))
#endif /*BSWM_DEV_ERROR_DETECT == STD_ON*/
    {
        result = BswM_GetPartitionLcPCCfgPtr(
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
            &bswmPartPbConfigPtr,
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
            &bswmPartPCCfgPtr,
            &bswmPartLCfgPtr);
        if (result)
        {
            modeRqstPCfgPtr = bswmPartPCCfgPtr->modeRqstPCCfg;
            modeRqstLCfgPtr = bswmPartLCfgPtr->modeRqstLCfg;
            numOfNvmJob     = modeRqstPCfgPtr->numOfNvMJob;
            for (jobIdx = 0u; jobIdx < numOfNvmJob; jobIdx++)
            {
                if (MultiBlockRequest == modeRqstLCfgPtr->nvmService[jobIdx])
                {
                    modeRqstPtr                               = bswmPartPCCfgPtr->modeRqstPortRunPtr;
                    modeRqstPtr->nvMJobModeIndiStatus[jobIdx] = CurrentJobMode;
                    if (BSWM_IMMEDIATE == modeRqstLCfgPtr->nvmJobInd[jobIdx].process)
                    {
                        numOfRules = modeRqstLCfgPtr->nvmJobInd[jobIdx].belongToRlueNum;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
                            if ((modeRqstLCfgPtr->nvmJobInd[jobIdx].belongToRlue[idx].pbConfigTag
                                 == BswM_GetPBConfigTags())
                                || (BSWM_PB_CONFIG_IN_ALL_VARIANTS
                                    == modeRqstLCfgPtr->nvmJobInd[jobIdx].belongToRlue[idx].pbConfigTag))
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
                            {
                                ruleIdx = modeRqstLCfgPtr->nvmJobInd[jobIdx].belongToRlue[idx].ruleIndex;

                                /* An Independent rule,to be evaluated each time applicable*/
                                BswM_ArbitrateRule(bswmPartLCfgPtr, ruleIdx, FALSE);
                            }
                        }
                    }
                    break;
                }
            }
        }
    }
#else
    (void)MultiBlockRequest;
    (void)CurrentJobMode;
#endif /*BSWM_NVMJOBMODEINDICATION_ENABLED == STD_ON*/
}

#if (BSWM_NVMJOBMODEINDICATION_ENABLED == STD_ON)
/**
 * Retrieves the current job mode for a specified NvM job index.
 * This function gets the partition configuration and returns the NvM job mode indication status.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
NvM_RequestResultType BswM_GetCurNvmJobMode(uint16 jobIdx)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
    NvM_RequestResultType          blockMode = NVM_REQ_OK;
    boolean                        result;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        blockMode = bswmPartPCCfgPtr->modeRqstPortRunPtr->nvMJobModeIndiStatus[jobIdx];
    }
    return blockMode;
}
#endif /*BSWM_NVMJOBMODEINDICATION_ENABLED == STD_ON*/

/**
 * Updates the current block mode for a specified NvM block.
 * This function checks the validity of the current block mode, retrieves the partition configuration,
 * and updates the request status for the specified block. If the process is immediate, it also evaluates
 * the associated rules.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_NvM_CurrentBlockMode(NvM_BlockIdType Block, NvM_RequestResultType CurrentBlockMode)
/* PRQA S 1503,1505,1532 --*/
{
#if (BSWM_NVMREQUEST_ENABLED == STD_ON)
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
    const BswM_PartitionPbConfigType* bswmPartPbConfigPtr;
#endif /* BSWM_PB_CONFIG_ENABLED */
    const BswM_PartitionLCfgType*       bswmPartLCfgPtr;
    const BswM_ModeRqstPortPCCfgType*   modeRqstPCfgPtr;
    const BswM_ModeRqstPortLCfgType*    modeRqstLCfgPtr;
    const BswM_ModeRqstPortRuntimeType* modeRqstPtr;
    BswM_RuleIndexType                  numOfRules;
    BswM_RuleIndexType                  idx;
    BswM_RuleIndexType                  ruleIdx;
    boolean                             result;
    NvM_BlockIdType                     numOfNvMBlock;
    NvM_BlockIdType                     bloIdx;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkCurNvmBlockMode(CurrentBlockMode))
#endif /*BSWM_DEV_ERROR_DETECT == STD_ON*/
    {
        result = BswM_GetPartitionLcPCCfgPtr(
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
            &bswmPartPbConfigPtr,
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
            &bswmPartPCCfgPtr,
            &bswmPartLCfgPtr);
        if (result)
        {
            modeRqstPCfgPtr = bswmPartPCCfgPtr->modeRqstPCCfg;
            modeRqstLCfgPtr = bswmPartLCfgPtr->modeRqstLCfg;
            numOfNvMBlock   = modeRqstPCfgPtr->numOfNvMBlock;
            for (bloIdx = 0u; bloIdx < numOfNvMBlock; bloIdx++)
            {
                if (Block == modeRqstLCfgPtr->nvmBlockRef[bloIdx])
                {
                    modeRqstPtr                       = bswmPartPCCfgPtr->modeRqstPortRunPtr;
                    modeRqstPtr->nvMReqStatus[bloIdx] = CurrentBlockMode;
                    if (BSWM_IMMEDIATE == modeRqstLCfgPtr->nvmRqst[bloIdx].process)
                    {
                        numOfRules = modeRqstLCfgPtr->nvmRqst[bloIdx].belongToRlueNum;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
                            if ((modeRqstLCfgPtr->nvmRqst[bloIdx].belongToRlue[idx].pbConfigTag
                                 == BswM_GetPBConfigTags())
                                || (BSWM_PB_CONFIG_IN_ALL_VARIANTS
                                    == modeRqstLCfgPtr->nvmRqst[bloIdx].belongToRlue[idx].pbConfigTag))
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
                            {
                                ruleIdx = modeRqstLCfgPtr->nvmRqst[bloIdx].belongToRlue[idx].ruleIndex;

                                /* An Independent rule,to be evaluated each time applicable*/
                                BswM_ArbitrateRule(bswmPartLCfgPtr, ruleIdx, FALSE);
                            }
                        }
                    }
                    break;
                }
            }
        }
    }
#else
    (void)Block;
    (void)CurrentBlockMode;
#endif /*BSWM_NVMREQUEST_ENABLED == STD_ON*/
}

#if (BSWM_NVMREQUEST_ENABLED == STD_ON)
/**
 * Retrieves the current block mode for a specified NvM block index.
 * This function gets the partition configuration and returns the NvM request status for the specified block.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
NvM_RequestResultType BswM_GetCurNvmBlockMode(NvM_BlockIdType blockIdx)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
    NvM_RequestResultType          blockMode = NVM_REQ_OK;
    boolean                        result;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        blockMode = bswmPartPCCfgPtr->modeRqstPortRunPtr->nvMReqStatus[blockIdx];
    }
    return blockMode;
}

#endif /*BSWM_NVMREQUEST_ENABLED == STD_ON*/

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
/* ========================================== internal function definitions ========================================= */

#endif /*BSWM_NVM_ENABLED == STD_ON*/
