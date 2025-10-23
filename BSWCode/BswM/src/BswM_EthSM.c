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
 **  @file               : ComM.c
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : BSW Mode Manager
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "BswM_EthSM.h"
#include "BswM_Internal.h"
#if (BSWM_ETHSM_ENABLED == STD_ON)
/* PRQA S 0553 EOF */ /* VL_BswM_0553 */
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
#if (BSWM_ETHSMINDICATION_ENABLED == STD_ON)
/**
 * Initializes the request ports for the Ethernet State Manager (EthSM).
 * This function sets the initial status of the EthSM indication ports based on the configuration.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_InitEthSMRequestPorts(void)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PartitionPCCfgType*      bswmPartPCCfgPtr;
    const BswM_ModeRqstPortPCCfgType*   modeRqstPCfgPtr;
    const BswM_ModeRqstPortRuntimeType* modeRqstPtr;
    boolean                             result;
    NetworkHandleType                   numOfEthSMInd;
    NetworkHandleType                   ethsmIdx;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        modeRqstPCfgPtr = bswmPartPCCfgPtr->modeRqstPCCfg;
        modeRqstPtr     = bswmPartPCCfgPtr->modeRqstPortRunPtr;
        numOfEthSMInd   = modeRqstPCfgPtr->numOfEthSMInd;
        for (ethsmIdx = 0u; ethsmIdx < numOfEthSMInd; ethsmIdx++)
        {
            if (NULL_PTR != modeRqstPCfgPtr->ethsmInitState)
            {
                modeRqstPtr->ethSMIndiStatus[ethsmIdx] = *(modeRqstPCfgPtr->ethsmInitState);
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
 * Retrieves the current status of the specified Ethernet State Manager (EthSM) channel.
 * This function queries the runtime configuration to get the current state of the EthSM indication port.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
EthSM_NetworkModeStateType BswM_GetEthSmIndStatus(NetworkHandleType ethsmChIdx)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
    EthSM_NetworkModeStateType     retStatus = ETHSM_STATE_OFFLINE;
    boolean                        result;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        retStatus = bswmPartPCCfgPtr->modeRqstPortRunPtr->ethSMIndiStatus[ethsmChIdx];
    }
    return retStatus;
}
#endif /*BSWM_ETHSMINDICATION_ENABLED == STD_ON*/

/**
 * Updates the current state of the specified Ethernet State Manager (EthSM) network.
 * This function checks the current state and updates the internal state of the EthSM indication port.
 * If the process is immediate, it also evaluates the associated rules.
 */
/* PRQA S 1503,1505,1532,6030 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef, VL_MTR_BswM_STMIF */
void BswM_EthSM_CurrentState(NetworkHandleType Network, EthSM_NetworkModeStateType CurrentState)
/* PRQA S 1503,1505,1532,6030 --*/
{
#if (BSWM_ETHSMINDICATION_ENABLED == STD_ON)
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
    NetworkHandleType                   numOfEthsmCh;
    NetworkHandleType                   chIdx;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkEthSMInd(CurrentState))
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
            numOfEthsmCh    = modeRqstPCfgPtr->numOfEthSMInd;
            for (chIdx = 0u; chIdx < numOfEthsmCh; chIdx++)
            {
                if (Network == modeRqstLCfgPtr->ethsmChRef[chIdx])
                {
                    modeRqstPtr                         = bswmPartPCCfgPtr->modeRqstPortRunPtr;
                    modeRqstPtr->ethSMIndiStatus[chIdx] = CurrentState;
                    if (BSWM_IMMEDIATE == modeRqstLCfgPtr->ethsmInd[chIdx].process)
                    {
                        numOfRules = modeRqstLCfgPtr->ethsmInd[chIdx].belongToRlueNum;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
                            if ((modeRqstLCfgPtr->ethsmInd[chIdx].belongToRlue[idx].pbConfigTag
                                 == BswM_GetPBConfigTags())
                                || (BSWM_PB_CONFIG_IN_ALL_VARIANTS
                                    == modeRqstLCfgPtr->ethsmInd[chIdx].belongToRlue[idx].pbConfigTag))
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
                            {
                                ruleIdx = modeRqstLCfgPtr->ethsmInd[chIdx].belongToRlue[idx].ruleIndex;
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
#endif /*BSWM_ETHSMINDICATION_ENABLED == STD_ON*/
}

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
/* ========================================== internal function definitions ========================================= */
#endif /*BSWM_ETHSM_ENABLED == STD_ON*/
