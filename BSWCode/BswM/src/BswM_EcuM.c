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
 **  @file               : BswM_EcuM.c
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : BSW Mode Manager
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "BswM_EcuM.h"
#include "BswM_Internal.h"
#if (BSWM_ECUM_ENABLED == STD_ON)
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
 * Initializes the ECU Manager request ports by setting up the initial states of wakeup sources and run request sources.
 * The function retrieves configuration pointers and initializes the status of wakeup sources and run request sources
 * based on the provided configurations. If the partition configuration retrieval fails, an error is reported.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_InitEcuMRequestPorts(void)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
    const BswM_PartitionPbConfigType* bswmPartPbConfigPtr;
#endif /* BSWM_PB_CONFIG_ENABLED */
    const BswM_PartitionLCfgType*       bswmPartLCfgPtr;
    const BswM_ModeRqstPortPCCfgType*   modeRqstPCfgPtr;
    const BswM_ModeRqstPortRuntimeType* modeRqstPtr;
    boolean                             result;
    EcuM_WakeupSourceType               numOfEcuMWakeupSrc;
    EcuM_WakeupSourceType               wkSrcIdx;

    uint8 numOfEcuMRunRqstSource;
    uint8 runRqstIdx;

    result = BswM_GetPartitionLcPCCfgPtr(
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
        &bswmPartPbConfigPtr,
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
        &bswmPartPCCfgPtr,
        &bswmPartLCfgPtr);
    if (result)
    {
        modeRqstPCfgPtr = bswmPartPCCfgPtr->modeRqstPCCfg;
        modeRqstPtr     = bswmPartPCCfgPtr->modeRqstPortRunPtr;
#if (BSWM_ECUMWAKEUPSOURCE_ENABLED == STD_ON)
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
#endif /*BSWM_ECUMWAKEUPSOURCE_ENABLED == STD_ON*/

#if (BSWM_ECUMRUNREQUESTINDICATION_ENABLED == STD_ON)
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
#endif /*BSWM_ECUMRUNREQUESTINDICATION_ENABLED == STD_ON*/
    }
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    else
    {
        (void)Det_ReportError(BSWM_MODULE_ID, BSWM_INSTANCE_ID, BSWM_API_ID_INIT, BSWM_E_INIT_FAILED);
    }
#endif /*BSWM_DEV_ERROR_DETECT == STD_ON*/
}

/**
 * Updates the current wakeup status for a specified wakeup source.
 * The function checks if the given wakeup source is valid and updates its status accordingly.
 * If the wakeup source is configured to process immediately, it triggers the evaluation of associated rules.
 * If the feature is disabled, the function does nothing.
 */
/* PRQA S 1503,1505,1532,6030 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef, VL_MTR_BswM_STMIF */
void BswM_EcuM_CurrentWakeup(EcuM_WakeupSourceType source, EcuM_WakeupStatusType state)
/* PRQA S 1503,1505,1532,6030 --*/
{
#if (BSWM_ECUMWAKEUPSOURCE_ENABLED == STD_ON)
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
    EcuM_WakeupSourceType               numOfWakeSrc;
    EcuM_WakeupSourceType               wkSrcIdx;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkEcuMCurWakeUp(state))
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
            numOfWakeSrc    = modeRqstPCfgPtr->numOfEcuMWakeupSource;
            for (wkSrcIdx = 0u; wkSrcIdx < numOfWakeSrc; wkSrcIdx++)
            {
                if ((source & modeRqstLCfgPtr->ecuMWkSrcRef[wkSrcIdx]) != 0uL)
                {
                    modeRqstPtr                                = bswmPartPCCfgPtr->modeRqstPortRunPtr;
                    modeRqstPtr->ecuMWakeupSrcStatus[wkSrcIdx] = state;
                    if (BSWM_IMMEDIATE == modeRqstLCfgPtr->ecuMWkSrc[wkSrcIdx].process)
                    {
                        numOfRules = modeRqstLCfgPtr->ecuMWkSrc[wkSrcIdx].belongToRlueNum;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
                            if ((modeRqstLCfgPtr->ecuMWkSrc[wkSrcIdx].belongToRlue[idx].pbConfigTag
                                 == BswM_GetPBConfigTags())
                                || (BSWM_PB_CONFIG_IN_ALL_VARIANTS
                                    == modeRqstLCfgPtr->ecuMWkSrc[wkSrcIdx].belongToRlue[idx].pbConfigTag))
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
                            {
                                ruleIdx = modeRqstLCfgPtr->ecuMWkSrc[wkSrcIdx].belongToRlue[idx].ruleIndex;

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
    (void)source;
    (void)state;
#endif /*BSWM_ECUMWAKEUPSOURCE_ENABLED == STD_ON*/
}

#if (BSWM_ECUMWAKEUPSOURCE_ENABLED == STD_ON)
/**
 * Retrieves the current wakeup status for a specified wakeup source.
 * The function checks if the partition configuration pointer is valid and returns the wakeup status.
 * If the configuration pointer is invalid, an invalid status is returned.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
EcuM_WakeupStatusType BswM_GetEcuMWakeSrcStatus(EcuM_WakeupSourceType wkSrcIdx)
/* PRQA S 1503,1505,1532 --*/
{
    EcuM_WakeupStatusType          wkStatus = BSWM_INVALID_U8;
    boolean                        result;
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        wkStatus = bswmPartPCCfgPtr->modeRqstPortRunPtr->ecuMWakeupSrcStatus[wkSrcIdx];
    }
    return wkStatus;
}
#endif /*BSWM_ECUMWAKEUPSOURCE_ENABLED == STD_ON*/

/**
 * Handles the requested state transition for the EcuM module.
 * This function updates the runtime request status for the specified state and evaluates rules if the process type is
 * immediate. It also includes error detection and configuration retrieval steps.
 */
/* PRQA S 1503,1505,1532,6030 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef, VL_MTR_BswM_STMIF */
void BswM_EcuM_RequestedState(EcuM_StateType State, EcuM_RunStatusType CurrentState)
/* PRQA S 1503,1505,1532,6030 --*/
{
#if (BSWM_ECUMRUNREQUESTINDICATION_ENABLED == STD_ON)
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
    uint8                               numOfEcuMRunRqstSource;
    uint8                               runRqstIdx;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkRequestedState(State, CurrentState))
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
            modeRqstPCfgPtr        = bswmPartPCCfgPtr->modeRqstPCCfg;
            modeRqstLCfgPtr        = bswmPartLCfgPtr->modeRqstLCfg;
            numOfEcuMRunRqstSource = modeRqstPCfgPtr->numOfEcuMRunRqstSource;
            for (runRqstIdx = 0u; runRqstIdx < numOfEcuMRunRqstSource; runRqstIdx++)
            {
                if (State == modeRqstLCfgPtr->ecuMRqstIndSrcRef[runRqstIdx])
                {
                    modeRqstPtr                                = bswmPartPCCfgPtr->modeRqstPortRunPtr;
                    modeRqstPtr->ecuMRunRqstStatus[runRqstIdx] = CurrentState;
                    if (BSWM_IMMEDIATE == modeRqstLCfgPtr->ecuMRqstIndSrc[runRqstIdx].process)
                    {
                        numOfRules = modeRqstLCfgPtr->ecuMRqstIndSrc[runRqstIdx].belongToRlueNum;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
                            if ((modeRqstLCfgPtr->ecuMRqstIndSrc[runRqstIdx].belongToRlue[idx].pbConfigTag
                                 == BswM_GetPBConfigTags())
                                || (BSWM_PB_CONFIG_IN_ALL_VARIANTS
                                    == modeRqstLCfgPtr->ecuMRqstIndSrc[runRqstIdx].belongToRlue[idx].pbConfigTag))
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
                            {
                                ruleIdx = modeRqstLCfgPtr->ecuMRqstIndSrc[runRqstIdx].belongToRlue[idx].ruleIndex;

                                /* An Independent rule,to be evaluated each time applicable*/
                                BswM_ArbitrateRule(bswmPartLCfgPtr, ruleIdx, FALSE);
                            }
                        }
                    }
                }
            }
        }
    }
#else
    (void)State;
    (void)CurrentState;
#endif /*BSWM_ECUMRUNREQUESTINDICATION_ENABLED == STD_ON*/
}

#if (BSWM_ECUMRUNREQUESTINDICATION_ENABLED == STD_ON)
/**
 * Retrieves the runtime request status for a specified EcuM state.
 * The function checks if the partition configuration pointer is valid and returns the request status.
 * If the configuration pointer is invalid, an invalid status is returned.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
EcuM_RunStatusType BswM_GetEcuMRunRqstStatus(EcuM_StateType rqstIdx)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
    EcuM_RunStatusType             rqstStatus = BSWM_INVALID_U8;
    boolean                        result;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        rqstStatus = bswmPartPCCfgPtr->modeRqstPortRunPtr->ecuMRunRqstStatus[rqstIdx];
    }
    return rqstStatus;
}
#endif /*BSWM_ECUMRUNREQUESTINDICATION_ENABLED == STD_ON*/

/**
 * Updates the current state of EcuM and processes any immediate rules associated with the new state.
 * This function performs error checking to ensure the system is initialized and the state is valid.
 * If the configuration pointers are valid, it updates the runtime status and evaluates the rules.
 * Error reporting is handled if any errors are detected during the process.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_EcuM_CurrentState(EcuM_StateType CurrentState)
/* PRQA S 1503,1505,1532 --*/
{
#if (BSWM_ECUMINDICATION_ENABLED == STD_ON)
    const BswM_ModeRqstPortLCfgType* modeRqstLCfgPtr;
    BswM_RuleIndexType               numOfRules;
    BswM_RuleIndexType               idx;
    BswM_RuleIndexType               ruleIdx;
    const BswM_PartitionPCCfgType*   bswmPartPCCfgPtr;
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
    const BswM_PartitionPbConfigType* bswmPartPbConfigPtr;
#endif /* BSWM_PB_CONFIG_ENABLED */
    const BswM_PartitionLCfgType* bswmPartLCfgPtr;
    const BswM_PCCfgType*         runPCCfgPtr;
    boolean                       result;

    result = BswM_GetCorePCCfgPtr(&runPCCfgPtr);
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = BSWM_E_OK;
    if ((BSWM_UNINITED == runPCCfgPtr->runtimeStatusPtr->bswmInitState) || (FALSE == result))
    {
        errorId = BSWM_E_UNINIT;
    }
    else if (
        (ECUM_STATE_STARTUP != CurrentState) && (ECUM_STATE_APP_RUN != CurrentState)
        && (ECUM_STATE_APP_POST_RUN != CurrentState) && (ECUM_STATE_SHUTDOWN != CurrentState)
        && (ECUM_STATE_SLEEP != CurrentState) && (ECUM_STATE_RUN != CurrentState))
    {
        errorId = BSWM_E_REQ_MODE_OUT_OF_RANGE;
    }
    else
#endif /*BSWM_DEV_ERROR_DETECT == STD_ON*/
    {
        (void)result;
        result = BswM_GetPartitionLcPCCfgPtr(
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
            &bswmPartPbConfigPtr,
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
            &bswmPartPCCfgPtr,
            &bswmPartLCfgPtr);
        if (result)
        {
            runPCCfgPtr->runtimeStatusPtr->ecumState = CurrentState;
            modeRqstLCfgPtr                          = bswmPartLCfgPtr->modeRqstLCfg;
            if (BSWM_IMMEDIATE == modeRqstLCfgPtr->ecuMState->process)
            {
                numOfRules = modeRqstLCfgPtr->ecuMState->belongToRlueNum;
                for (idx = 0u; idx < numOfRules; idx++)
                {
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
                    if ((modeRqstLCfgPtr->ecuMState->belongToRlue[idx].pbConfigTag == BswM_GetPBConfigTags())
                        || (BSWM_PB_CONFIG_IN_ALL_VARIANTS
                            == modeRqstLCfgPtr->ecuMState->belongToRlue[idx].pbConfigTag))
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
                    {
                        ruleIdx = modeRqstLCfgPtr->ecuMState->belongToRlue[idx].ruleIndex;

                        /* An Independent rule,to be evaluated each time applicable*/
                        BswM_ArbitrateRule(bswmPartLCfgPtr, ruleIdx, FALSE);
                    }
                }
            }
        }
    }

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if (errorId != BSWM_E_OK)
    {
        (void)Det_ReportError(BSWM_MODULE_ID, BSWM_INSTANCE_ID, BSWM_API_ID_ECUM_CURRENT_STATE, errorId);
    }
#endif
#else
    (void)CurrentState;
#endif /*BSWM_ECUMINDICATION_ENABLED == STD_ON*/
}

#if (BSWM_ECUMINDICATION_ENABLED == STD_ON)
/**
 * Retrieves the current state of EcuM from the runtime configuration.
 * The function fetches the configuration pointer and returns the current EcuM state.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
EcuM_StateType BswM_GetEcuMState(void)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PCCfgType* runPCCfgPtr;
    (void)BswM_GetCorePCCfgPtr(&runPCCfgPtr);
    return runPCCfgPtr->runtimeStatusPtr->ecumState;
}
#endif /*BSWM_ECUMINDICATION_ENABLED == STD_ON*/

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
/* ========================================== internal function definitions ========================================= */

#endif /*BSWM_ECUM_ENABLED == STD_ON*/
