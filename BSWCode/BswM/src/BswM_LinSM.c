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
 **  @file               : BswM_LinSM.c
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : BSW Mode Manager
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "BswM_LinSM.h"
#include "BswM_Internal.h"
#if (BSWM_LINSM_ENABLED == STD_ON)
/* PRQA S 0553 EOF */ /* VL_BswM_0553 */
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
#if (BSWM_LINSMINDICATION_ENABLED == STD_ON)
/**
 * Initializes the LIN State Machine request ports.
 * This function sets the initial states for the LIN State Machine indicators based on the configuration.
 * If the initialization state is not provided, the function will terminate early.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_InitLinSMRequestPorts(void)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PartitionPCCfgType*      bswmPartPCCfgPtr;
    const BswM_ModeRqstPortPCCfgType*   modeRqstPCfgPtr;
    const BswM_ModeRqstPortRuntimeType* modeRqstPtr;
    boolean                             result;
    NetworkHandleType                   numOfLinSMInd;
    NetworkHandleType                   linsmIdx;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        modeRqstPCfgPtr = bswmPartPCCfgPtr->modeRqstPCCfg;
        modeRqstPtr     = bswmPartPCCfgPtr->modeRqstPortRunPtr;
        numOfLinSMInd   = modeRqstPCfgPtr->numOfLinSMInd;
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
 * Retrieves the current status of the specified LIN State Machine indicator.
 * This function queries the runtime configuration to get the status of a specific LIN SM indicator.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
LinSM_ModeType BswM_GetLinSmIndStatus(NetworkHandleType linsmChIdx)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
    LinSM_ModeType                 retStatus = BSWM_INVALID_U8;
    boolean                        result;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        retStatus = bswmPartPCCfgPtr->modeRqstPortRunPtr->linSMIndiStatus[linsmChIdx];
    }
    return retStatus;
}
#endif /* BSWM_LINSMINDICATION_ENABLED == STD_ON */

/**
 * Updates the current state of the specified LIN State Machine.
 * This function checks and updates the state of a specific LIN SM based on the provided network handle and current
 * state. It also evaluates any associated rules if the process type is immediate.
 */
/* PRQA S 1503,1505,1532,6030 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef, VL_MTR_BswM_STMIF */
void BswM_LinSM_CurrentState(NetworkHandleType Network, LinSM_ModeType CurrentState)
/* PRQA S 1503,1505,1532,6030 --*/
{
#if (BSWM_LINSMINDICATION_ENABLED == STD_ON)
    const BswM_PartitionPCCfgType*    bswmPartPCCfgPtr;
    const BswM_PartitionLCfgType*     bswmPartLCfgPtr;
    const BswM_ModeRqstPortPCCfgType* modeRqstPCfgPtr;
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
    const BswM_PartitionPbConfigType* bswmPartPbConfigPtr;
#endif /* BSWM_PB_CONFIG_ENABLED */
    const BswM_ModeRqstPortLCfgType*    modeRqstLCfgPtr;
    const BswM_ModeRqstPortRuntimeType* modeRqstPtr;
    BswM_RuleIndexType                  numOfRules;
    BswM_RuleIndexType                  idx;
    BswM_RuleIndexType                  ruleIdx;
    boolean                             result;
    NetworkHandleType                   numOfLinsmCh;
    NetworkHandleType                   chIdx;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkLinSMInd(CurrentState))
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
            numOfLinsmCh    = modeRqstPCfgPtr->numOfLinSMInd;
            for (chIdx = 0u; chIdx < numOfLinsmCh; chIdx++)
            {
                if (Network == modeRqstLCfgPtr->linsmIndChRef[chIdx])
                {
                    modeRqstPtr                         = bswmPartPCCfgPtr->modeRqstPortRunPtr;
                    modeRqstPtr->linSMIndiStatus[chIdx] = CurrentState;
                    if (BSWM_IMMEDIATE == modeRqstLCfgPtr->linsmInd[chIdx].process)
                    {
                        numOfRules = modeRqstLCfgPtr->linsmInd[chIdx].belongToRlueNum;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
                            if ((modeRqstLCfgPtr->linsmInd[chIdx].belongToRlue[idx].pbConfigTag
                                 == BswM_GetPBConfigTags())
                                || (BSWM_PB_CONFIG_IN_ALL_VARIANTS
                                    == modeRqstLCfgPtr->linsmInd[chIdx].belongToRlue[idx].pbConfigTag))
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
                            {
                                ruleIdx = modeRqstLCfgPtr->linsmInd[chIdx].belongToRlue[idx].ruleIndex;

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
    (void)CurrentState;
#endif /* BSWM_LINSMINDICATION_ENABLED == STD_ON */
}

/**
 * Updates the current schedule of the specified LIN State Machine.
 * This function checks and updates the schedule of a specific LIN SM based on the provided network handle and current
 * schedule. It also evaluates any associated rules if the process type is immediate.
 */
/* PRQA S 1503,1505,1532,6030 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef, VL_MTR_BswM_STMIF */
void BswM_LinSM_CurrentSchedule(NetworkHandleType Network, LinIf_SchHandleType CurrentSchedule)
/* PRQA S 1503,1505,1532,6030 --*/
{
#if (BSWM_LINSCHEDULEINDICATION_ENABLED == STD_ON)
    const BswM_PartitionPCCfgType*    bswmPartPCCfgPtr;
    const BswM_PartitionLCfgType*     bswmPartLCfgPtr;
    const BswM_ModeRqstPortPCCfgType* modeRqstPCfgPtr;
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
    const BswM_PartitionPbConfigType* bswmPartPbConfigPtr;
#endif /* BSWM_PB_CONFIG_ENABLED */
    const BswM_ModeRqstPortLCfgType*    modeRqstLCfgPtr;
    const BswM_ModeRqstPortRuntimeType* modeRqstPtr;
    BswM_RuleIndexType                  numOfRules;
    BswM_RuleIndexType                  idx;
    BswM_RuleIndexType                  ruleIdx;
    boolean                             result;
    NetworkHandleType                   numOfLinsmCh;
    NetworkHandleType                   chIdx;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkLinSMCurSch())
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
            if (0u != modeRqstPCfgPtr->numOfLinSchInd)
            {
                numOfLinsmCh = modeRqstPCfgPtr->numOfLinSchInd;
            }
            else
            {
                numOfLinsmCh = 0u;
            }

            for (chIdx = 0u; chIdx < numOfLinsmCh; chIdx++)
            {
#if (BSWM_PB_CONFIG_ENABLED == STD_OFF)
                NetworkHandleType   lNetwork         = modeRqstLCfgPtr->linSchNKChRef[chIdx];
                LinIf_SchHandleType lCurrentSchedule = modeRqstLCfgPtr->linSchChRef[chIdx];
#else
                NetworkHandleType   lNetwork         = bswmPartPbConfigPtr->linSchNKChRef[chIdx];
                LinIf_SchHandleType lCurrentSchedule = bswmPartPbConfigPtr->linSchChRef[chIdx];
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
                if ((Network == lNetwork) && (CurrentSchedule == lCurrentSchedule))
                {
                    modeRqstPtr                          = bswmPartPCCfgPtr->modeRqstPortRunPtr;
                    modeRqstPtr->linSchIndiStatus[chIdx] = CurrentSchedule;
                    if (BSWM_IMMEDIATE == modeRqstLCfgPtr->linSchInd[chIdx].process)
                    {
                        numOfRules = modeRqstLCfgPtr->linSchInd[chIdx].belongToRlueNum;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
                            if ((modeRqstLCfgPtr->linSchInd[chIdx].belongToRlue[idx].pbConfigTag
                                 == BswM_GetPBConfigTags())
                                || (BSWM_PB_CONFIG_IN_ALL_VARIANTS
                                    == modeRqstLCfgPtr->linSchInd[chIdx].belongToRlue[idx].pbConfigTag))
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
                            {
                                ruleIdx = modeRqstLCfgPtr->linSchInd[chIdx].belongToRlue[idx].ruleIndex;

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
    (void)CurrentSchedule;
#endif /*BSWM_LINSCHEDULEINDICATION_ENABLED == STD_ON*/
}

#if (BSWM_LINSCHEDULEINDICATION_ENABLED == STD_ON)
/**
 * Retrieves the current schedule of the specified LIN State Machine channel.
 * This function returns the current schedule index for the given LIN SM channel.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
LinIf_SchHandleType BswM_GetLinSmCurSchedule(NetworkHandleType linsmChIdx)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
    LinIf_SchHandleType            curSch = BSWM_INVALID_U8;
    boolean                        result;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        curSch = bswmPartPCCfgPtr->modeRqstPortRunPtr->linSchIndiStatus[linsmChIdx];
    }
    return curSch;
}
#endif /*BSWM_LINSCHEDULEINDICATION_ENABLED == STD_ON*/
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
/* ========================================== internal function definitions ========================================= */

#endif /*BSWM_LINSM_ENABLED == STD_ON*/
