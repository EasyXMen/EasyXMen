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
 **  @file               : BswM_Dcm.c
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : BSW Mode Manager
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "BswM_Dcm.h"
#include "BswM_Internal.h"
#if (BSWM_DCM_ENABLED == STD_ON)
/* PRQA S 0553 EOF */ /* VL_BswM_0553 */
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"

#if (BSWM_DCMCOMMODEREQUEST_ENABLED == STD_ON)
/**
 * Initializes the DCM request ports by setting the initial communication mode request status.
 * If the initialization mode is not NULL, it sets the status for each DCM channel.
 * If the configuration pointer retrieval fails, it reports an error if development error detection is enabled.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_InitDcmRequestPorts(void)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PartitionPCCfgType*      bswmPartPCCfgPtr;
    const BswM_ModeRqstPortPCCfgType*   modeRqstPCfgPtr;
    const BswM_ModeRqstPortRuntimeType* modeRqstPtr;
    NetworkHandleType                   numOfDcmCh;
    NetworkHandleType                   idx;
    boolean                             result;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        modeRqstPCfgPtr = bswmPartPCCfgPtr->modeRqstPCCfg;
        modeRqstPtr     = bswmPartPCCfgPtr->modeRqstPortRunPtr;
        numOfDcmCh      = modeRqstPCfgPtr->numOfDcmComModeReq;
        for (idx = 0u; idx < numOfDcmCh; idx++)
        {
            if (NULL_PTR != modeRqstPCfgPtr->dcmInitMode)
            {
                modeRqstPtr->dcmComModeReqStatus[idx] = *(modeRqstPCfgPtr->dcmInitMode);
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
#endif /* BSWM_DCMCOMMODEREQUEST_ENABLED == STD_ON */

/**
 * Handles the DCM application update indication by setting the event request port status and evaluating rules.
 * If the event request is immediate, it evaluates the associated rules for the updated DCM application.
 * Development error detection is performed to ensure the function is called correctly.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_Dcm_ApplicationUpdated(void)
/* PRQA S 1503,1505,1532 --*/
{
#if (BSWM_EVENT_DCM_APPLICATION_UPDATED_INDICATION_ENABLE == STD_ON)
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
    const BswM_PartitionPbConfigType* bswmPartPbConfigPtr;
#endif /* BSWM_PB_CONFIG_ENABLED */
    const BswM_PartitionLCfgType*     bswmPartLCfgPtr;
    const BswM_EventRqstPortLCfgType* evRqstLCfgPtr;
    BswM_EventRquestPortRuntimeType*  evRqstPtr;
    BswM_RuleIndexType                numOfRules;
    BswM_RuleIndexType                idx;
    BswM_RuleIndexType                ruleIdx;
    BswM_EventRqstPortIdxType         evIdx;
    boolean                           result;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkDcmUpdate())
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
            evRqstPtr        = bswmPartPCCfgPtr->eventRqstPortRunPtr;
            evIdx            = *(bswmPartPCCfgPtr->evRqstPCCfg->dcmAppUpdateIndIdxPtr);
            evRqstPtr[evIdx] = BSWM_EVENT_IS_SET;
            evRqstLCfgPtr    = bswmPartLCfgPtr->evRqstLCfg;
            if (BSWM_IMMEDIATE == evRqstLCfgPtr->dcmAppUpdate->process)
            {
                numOfRules = evRqstLCfgPtr->dcmAppUpdate->belongToRlueNum;
                for (idx = 0u; idx < numOfRules; idx++)
                {
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
                    if ((evRqstLCfgPtr->dcmAppUpdate->belongToRlue[idx].pbConfigTag == BswM_GetPBConfigTags())
                        || (BSWM_PB_CONFIG_IN_ALL_VARIANTS
                            == evRqstLCfgPtr->dcmAppUpdate->belongToRlue[idx].pbConfigTag))
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
                    {
                        ruleIdx = evRqstLCfgPtr->dcmAppUpdate->belongToRlue[idx].ruleIndex;

                        /* An Independent rule,to be evaluated each time applicable*/
                        BswM_ArbitrateRule(bswmPartLCfgPtr, ruleIdx, FALSE);
                    }
                }
            }
        }
    }
#endif /*BSWM_EVENT_DCM_APPLICATION_UPDATED_INDICATION_ENABLE == STD_ON*/
}

/**
 * Handles the current communication mode request for a specific network by updating the mode request status and
 * evaluating rules. If the mode request is immediate, it evaluates the associated rules for the requested communication
 * mode. Development error detection is performed to ensure the function is called with valid parameters.
 */
/* PRQA S 1503,1505,1532,6030 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef, VL_MTR_BswM_STMIF */
void BswM_Dcm_CommunicationMode_CurrentState(NetworkHandleType Network, Dcm_CommunicationModeType RequestedMode)
/* PRQA S 1503,1505,1532,6030 --*/
{
#if (BSWM_DCMCOMMODEREQUEST_ENABLED == STD_ON)
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
    const BswM_PartitionLCfgType*  bswmPartLCfgPtr;
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
    const BswM_PartitionPbConfigType* bswmPartPbConfigPtr;
#endif /* BSWM_PB_CONFIG_ENABLED */
    const BswM_ModeRqstPortPCCfgType*   modeRqstPCfgPtr;
    const BswM_ModeRqstPortLCfgType*    modeRqstLCfgPtr;
    const BswM_ModeRqstPortRuntimeType* modeRqstPtr;
    BswM_RuleIndexType                  numOfRules;
    BswM_RuleIndexType                  idx;
    BswM_RuleIndexType                  ruleIdx;
    boolean                             result;
    NetworkHandleType                   numOfDcmCh;
    NetworkHandleType                   chIdx;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkDcmModeCurState(RequestedMode))
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
            numOfDcmCh      = modeRqstPCfgPtr->numOfDcmComModeReq;
            for (chIdx = 0u; chIdx < numOfDcmCh; chIdx++)
            {
                if (Network == modeRqstLCfgPtr->dcmChRef[chIdx])
                {
                    modeRqstPtr                             = bswmPartPCCfgPtr->modeRqstPortRunPtr;
                    modeRqstPtr->dcmComModeReqStatus[chIdx] = RequestedMode;
                    if (BSWM_IMMEDIATE == modeRqstLCfgPtr->dcmComModeRqst[chIdx].process)
                    {
                        numOfRules = modeRqstLCfgPtr->dcmComModeRqst[chIdx].belongToRlueNum;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
                            if ((modeRqstLCfgPtr->dcmComModeRqst[chIdx].belongToRlue[idx].pbConfigTag
                                 == BswM_GetPBConfigTags())
                                || (BSWM_PB_CONFIG_IN_ALL_VARIANTS
                                    == modeRqstLCfgPtr->dcmComModeRqst[chIdx].belongToRlue[idx].pbConfigTag))
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
                            {
                                ruleIdx = modeRqstLCfgPtr->dcmComModeRqst[chIdx].belongToRlue[idx].ruleIndex;

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
    (void)Network;
    (void)RequestedMode;
#endif /*BSWM_DCMCOMMODEREQUEST_ENABLED == STD_ON*/
}

#if (BSWM_DCMCOMMODEREQUEST_ENABLED == STD_ON)
/**
 * Retrieves the current communication mode status for a given DCM channel index.
 * It checks the partition configuration to get the current mode status of the specified DCM channel.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
Dcm_CommunicationModeType BswM_GetDcmCurModeStatus(NetworkHandleType dcmChIdx)
/* PRQA S 1503,1505,1532 --*/
{
    /* PRQA S 1317, 1297 ++*/ /* VL_BswM_1317_1297 */
    Dcm_CommunicationModeType dcmComMode = BSWM_INVALID_LONG_U8;
    /* PRQA S 1317, 1297 --*/
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
    boolean                        result;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        dcmComMode = bswmPartPCCfgPtr->modeRqstPortRunPtr->dcmComModeReqStatus[dcmChIdx];
    }
    return dcmComMode;
}
#endif /*BSWM_DCMCOMMODEREQUEST_ENABLED == STD_ON*/

/**
 * Retrieves the updated status of the DCM application.
 * It checks the partition configuration to get the event request port runtime status for the DCM application update
 * indication.
 */
/* PRQA S 1503 ++*/ /* VL_QAC_NoUsedApi */
BswM_EventRquestPortRuntimeType BswM_GetDcmAppUpdatedStatus(void)
/* PRQA S 1503 --*/ /* VL_QAC_NoUsedApi */
{
    BswM_EventRquestPortRuntimeType dcmAppStatus = BSWM_INVALID_U8;
#if (BSWM_EVENT_DCM_APPLICATION_UPDATED_INDICATION_ENABLE == STD_ON)
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
    BswM_EventRqstPortIdxType      evIdx;
    boolean                        result;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        evIdx        = *(bswmPartPCCfgPtr->evRqstPCCfg->dcmAppUpdateIndIdxPtr);
        dcmAppStatus = bswmPartPCCfgPtr->eventRqstPortRunPtr[evIdx];
    }
#endif /*BSWM_EVENT_DCM_APPLICATION_UPDATED_INDICATION_ENABLE == STD_ON*/
    return dcmAppStatus;
}

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
/* ========================================== internal function definitions ========================================= */

#endif /*BSWM_DCM_ENABLED == STD_ON*/
