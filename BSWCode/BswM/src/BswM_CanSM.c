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
 **  @file               : BswM_CanSM.c
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : BSW Mode Manager
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "BswM_CanSM.h"
#include "BswM_Internal.h"
#if (BSWM_CANSM_ENABLED == STD_ON)
/* PRQA S 0553 EOF */ /* VL_BswM_0553 */
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
#if (BSWM_CANSMINDICATION_ENABLED == STD_ON)
/**
 * This function initializes the CAN State Manager request ports. It retrieves the partition configuration pointer,
 * checks the validity of the configuration, and sets the initial status of the CAN SM indication ports based on the
 * provided initialization state. If the configuration is invalid, it reports an error.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_InitCanSMRequestPorts(void)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PartitionPCCfgType*      bswmPartPCCfgPtr;
    const BswM_ModeRqstPortPCCfgType*   modeRqstPCfgPtr;
    const BswM_ModeRqstPortRuntimeType* modeRqstPtr;
    NetworkHandleType                   numOfCanSMInd;
    NetworkHandleType                   idx;
    boolean                             result;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        modeRqstPCfgPtr = bswmPartPCCfgPtr->modeRqstPCCfg;
        modeRqstPtr     = bswmPartPCCfgPtr->modeRqstPortRunPtr;
        numOfCanSMInd   = modeRqstPCfgPtr->numOfCanSMInd;
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
    }
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    else
    {
        (void)Det_ReportError(BSWM_MODULE_ID, BSWM_INSTANCE_ID, BSWM_API_ID_INIT, BSWM_E_INIT_FAILED);
    }
#endif /*BSWM_DEV_ERROR_DETECT == STD_ON*/
}

/**
 * This function retrieves the current status of the CAN State Manager indication port for a given channel index.
 * It retrieves the partition configuration pointer and returns the status of the specified CAN SM indication port.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
CanSM_BswMCurrentStateType BswM_GetCanSmIndStatus(NetworkHandleType cansmChIdx)
/* PRQA S 1503,1505,1532 --*/
{
    CanSM_BswMCurrentStateType     retStatu = CANSM_BSWM_NO_COMMUNICATION;
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
    boolean                        result;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        retStatu = bswmPartPCCfgPtr->modeRqstPortRunPtr->canSMIndiStatus[cansmChIdx];
    }
    return retStatu;
}
#endif /*BSWM_CANSMINDICATION_ENABLED == STD_ON*/

/**
 * This function updates the current state of the CAN State Manager for a specific network.
 * It retrieves the necessary configuration pointers and updates the CAN SM indication status.
 * If the process is immediate, it evaluates the associated rules.
 */
/* PRQA S 1503,1505,1532,6030 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef, VL_MTR_BswM_STMIF */
void BswM_CanSM_CurrentState(NetworkHandleType Network, CanSM_BswMCurrentStateType CurrentState)
/* PRQA S 1503,1505,1532,6030 --*/
{
#if (BSWM_CANSMINDICATION_ENABLED == STD_ON)
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
    const BswM_PartitionPbConfigType* bswmPartPbConfigPtr;
#endif /* BSWM_PB_CONFIG_ENABLED */
    const BswM_PartitionLCfgType*       bswmPartLCfgPtr;
    const BswM_ModeRqstPortPCCfgType*   modeRqstPCfgPtr;
    const BswM_ModeRqstPortLCfgType*    modeRqstLCfgPtr;
    const BswM_ModeRqstPortRuntimeType* modeRqstPtr;
    NetworkHandleType                   numOfCansmCh;
    NetworkHandleType                   chIdx;
    BswM_RuleIndexType                  numOfRules;
    BswM_RuleIndexType                  idx;
    BswM_RuleIndexType                  ruleIdx;
    boolean                             result;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkCanSMInd(CurrentState))
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
            numOfCansmCh    = modeRqstPCfgPtr->numOfCanSMInd;
            for (chIdx = 0u; chIdx < numOfCansmCh; chIdx++)
            {
                if (Network == modeRqstLCfgPtr->cansmIndChRef[chIdx])
                {
                    modeRqstPtr                         = bswmPartPCCfgPtr->modeRqstPortRunPtr;
                    modeRqstPtr->canSMIndiStatus[chIdx] = CurrentState;

                    if (BSWM_IMMEDIATE == modeRqstLCfgPtr->cansmInd[chIdx].process)
                    {
                        numOfRules = modeRqstLCfgPtr->cansmInd[chIdx].belongToRlueNum;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
                            if ((modeRqstLCfgPtr->cansmInd[chIdx].belongToRlue[idx].pbConfigTag
                                 == BswM_GetPBConfigTags())
                                || (BSWM_PB_CONFIG_IN_ALL_VARIANTS
                                    == modeRqstLCfgPtr->cansmInd[chIdx].belongToRlue[idx].pbConfigTag))
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
                            {
                                ruleIdx = modeRqstLCfgPtr->cansmInd[chIdx].belongToRlue[idx].ruleIndex;
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
#endif /* BSWM_CANSMINDICATION_ENABLED == STD_ON */
}

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
/* ========================================== internal function definitions ========================================= */

#endif /*BSWM_CANSM_ENABLED == STD_ON*/
