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
 **  @file               : ComM.c
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : BSW Mode Manager
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "BswM_Nm.h"
#include "BswM_Internal.h"
#if (BSWM_NM_ENABLED == STD_ON)
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
 * Processes the car wake-up indication for the specified network.
 * This function updates the event request status and evaluates the associated rules if immediate processing is
 * required.
 */
/* PRQA S 1503,1505,1532,6030 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef, VL_MTR_BswM_STMIF */
void BswM_Nm_CarWakeUpIndication(NetworkHandleType Network)
/* PRQA S 1503,1505,1532,6030 --*/
{
#if (BSWM_EVENT_NM_CAR_WAKEUP_INDICATION_ENABLE == STD_ON)
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
    NetworkHandleType                 numOfNmWkCh;
    NetworkHandleType                 chIdx;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkNmCarWkUp())
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
            numOfNmWkCh   = bswmPartPCCfgPtr->evRqstPCCfg->numOfNmWkCh;
            evRqstLCfgPtr = bswmPartLCfgPtr->evRqstLCfg;
            for (chIdx = 0u; chIdx < numOfNmWkCh; chIdx++)
            {
                if (Network == evRqstLCfgPtr->nmWkChRef[chIdx])
                {
                    evIdx            = bswmPartPCCfgPtr->evRqstPCCfg->nmWkEvPortIdxPtr[chIdx];
                    evRqstPtr        = bswmPartPCCfgPtr->eventRqstPortRunPtr;
                    evRqstPtr[evIdx] = BSWM_EVENT_IS_SET;
                    if (BSWM_IMMEDIATE == evRqstLCfgPtr->nmWakeUpInd[chIdx].process)
                    {
                        numOfRules = evRqstLCfgPtr->nmWakeUpInd[chIdx].belongToRlueNum;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
                            if ((evRqstLCfgPtr->nmWakeUpInd[chIdx].belongToRlue[idx].pbConfigTag
                                 == BswM_GetPBConfigTags())
                                || (BSWM_PB_CONFIG_IN_ALL_VARIANTS
                                    == evRqstLCfgPtr->nmWakeUpInd[chIdx].belongToRlue[idx].pbConfigTag))
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
                            {
                                ruleIdx = evRqstLCfgPtr->nmWakeUpInd[chIdx].belongToRlue[idx].ruleIndex;

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
#endif /*BSWM_EVENT_NM_CAR_WAKEUP_INDICATION_ENABLE == STD_ON*/
}

#if (BSWM_NMSTATECHANGENOTIFICATION_ENABLED == STD_ON)
/**
 * Initializes the network management request ports by setting their initial status.
 * This function retrieves configuration data and sets the initial status of each network channel.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_InitNmRequestPorts(void)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PartitionPCCfgType*      bswmPartPCCfgPtr;
    const BswM_ModeRqstPortPCCfgType*   modeRqstPCfgPtr;
    const BswM_ModeRqstPortRuntimeType* modeRqstPtr;
    boolean                             result;
    NetworkHandleType                   numOfNmCh;
    NetworkHandleType                   NmIdx;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        modeRqstPCfgPtr = bswmPartPCCfgPtr->modeRqstPCCfg;
        modeRqstPtr     = bswmPartPCCfgPtr->modeRqstPortRunPtr;
        numOfNmCh       = modeRqstPCfgPtr->numOfNmChannel;
        for (NmIdx = 0u; NmIdx < numOfNmCh; NmIdx++)
        {
            if (NULL_PTR != modeRqstPCfgPtr->nmInitStatus)
            {
                modeRqstPtr->NmModeReqStatus[NmIdx] = *(modeRqstPCfgPtr->nmInitStatus);
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
 * Retrieves the current network management state for a specified network channel.
 * This function gets the configuration data and returns the current state of the specified network channel.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
Nm_StateType BswM_GetCurNmState(uint8 nmChIdx)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
    Nm_StateType                   NmState = NM_STATE_UNINIT;
    boolean                        result;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        NmState = bswmPartPCCfgPtr->modeRqstPortRunPtr->NmModeReqStatus[nmChIdx];
    }
    return NmState;
}
#endif /*BSWM_NMSTATECHANGENOTIFICATION_ENABLED == STD_ON*/

/**
 * Notifies the system about a change in the network management state for a specified network.
 * This function updates the internal state and triggers any necessary actions based on the new state.
 * It checks the configuration data, updates the state, and evaluates rules if required.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_Nm_StateChangeNotification(NetworkHandleType Network, Nm_StateType currentState)
/* PRQA S 1503,1505,1532 --*/
{
#if (BSWM_NMSTATECHANGENOTIFICATION_ENABLED == STD_ON)
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
    NetworkHandleType                   numOfNmCh;
    NetworkHandleType                   chIdx;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkNmStateChange())
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
            numOfNmCh       = modeRqstPCfgPtr->numOfNmChannel;
            for (chIdx = 0u; chIdx < numOfNmCh; chIdx++)
            {
                if (Network == modeRqstLCfgPtr->nmChRef[chIdx])
                {
                    modeRqstPtr                         = bswmPartPCCfgPtr->modeRqstPortRunPtr;
                    modeRqstPtr->NmModeReqStatus[chIdx] = currentState;
                    if (BSWM_IMMEDIATE == modeRqstLCfgPtr->nmInd[chIdx].process)
                    {
                        numOfRules = modeRqstLCfgPtr->nmInd[chIdx].belongToRlueNum;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
                            if ((modeRqstLCfgPtr->nmInd[chIdx].belongToRlue[idx].pbConfigTag == BswM_GetPBConfigTags())
                                || (BSWM_PB_CONFIG_IN_ALL_VARIANTS
                                    == modeRqstLCfgPtr->nmInd[chIdx].belongToRlue[idx].pbConfigTag))
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
                            {
                                ruleIdx = modeRqstLCfgPtr->nmInd[chIdx].belongToRlue[idx].ruleIndex;

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
    (void)currentState;
#endif /*BSWM_NMSTATECHANGENOTIFICATION_ENABLED == STD_ON*/
}

/**
 * Retrieves the event request port runtime type for a specified network index.
 * This function checks the configuration data and returns the corresponding event request port runtime type.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
BswM_EventRquestPortRuntimeType BswM_GetNmWkInd(NetworkHandleType nmIdx)
/* PRQA S 1503,1505,1532 --*/
{
    BswM_EventRquestPortRuntimeType ret = BSWM_INVALID_U8;
#if (BSWM_EVENT_NM_CAR_WAKEUP_INDICATION_ENABLE == STD_ON)
    const BswM_PartitionPCCfgType*  bswmPartPCCfgPtr;
    boolean                         result;
    BswM_EventRquestPortRuntimeType evIdx;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        evIdx = bswmPartPCCfgPtr->evRqstPCCfg->nmWkEvPortIdxPtr[nmIdx];
        ret   = bswmPartPCCfgPtr->eventRqstPortRunPtr[evIdx];
    }
#else
    (void)(nmIdx);
#endif /*BSWM_EVENT_NM_CAR_WAKEUP_INDICATION_ENABLE == STD_ON*/
    return ret;
}

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
/* ========================================== internal function definitions ========================================= */

#endif /*BSWM_NM_ENABLED == STD_ON*/
