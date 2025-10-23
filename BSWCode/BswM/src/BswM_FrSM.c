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
#include "BswM_FrSM.h"
#include "BswM_Internal.h"
#if (BSWM_FRSM_ENABLED == STD_ON)
/* PRQA S 0553 EOF */ /* VL_BswM_0553 */
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
#if (BSWM_FRSMINDICATION_ENABLED == STD_ON)
/**
 * Initializes the FlexRay State Manager (FrSM) request ports.
 * This function retrieves the configuration pointers and initializes the FrSM indication status based on the provided
 * initial states. If the initialization fails, an error is reported.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_InitFrSMRequestPorts(void)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PartitionPCCfgType*      bswmPartPCCfgPtr;
    const BswM_ModeRqstPortPCCfgType*   modeRqstPCfgPtr;
    const BswM_ModeRqstPortRuntimeType* modeRqstPtr;
    boolean                             result;
    NetworkHandleType                   numOfFrSMInd;
    NetworkHandleType                   frsmIdx;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        modeRqstPCfgPtr = bswmPartPCCfgPtr->modeRqstPCCfg;
        modeRqstPtr     = bswmPartPCCfgPtr->modeRqstPortRunPtr;
        numOfFrSMInd    = modeRqstPCfgPtr->numOfFrSMInd;
        for (frsmIdx = 0u; frsmIdx < numOfFrSMInd; frsmIdx++)
        {
            if (NULL_PTR != modeRqstPCfgPtr->frsmInitState)
            {
                modeRqstPtr->frSMIndiStatus[frsmIdx] = *(modeRqstPCfgPtr->frsmInitState);
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
 * Retrieves the FlexRay State Manager (FrSM) indication status for a given channel index.
 * This function fetches the configuration pointer and returns the current FrSM indication status.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
FrSM_BswM_StateType BswM_GetFrSmIndStatus(NetworkHandleType frsmChIdx)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
    FrSM_BswM_StateType            retStatus = FRSM_BSWM_READY;
    boolean                        result;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        retStatus = bswmPartPCCfgPtr->modeRqstPortRunPtr->frSMIndiStatus[frsmChIdx];
    }
    return retStatus;
}
#endif /*BSWM_FRSMINDICATION_ENABLED == STD_ON*/

/**
 * Updates the current state of the FlexRay State Manager (FrSM) for a specified network.
 * This function checks the configuration, updates the FrSM indication status, and evaluates rules if necessary.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_FrSM_CurrentState(NetworkHandleType Network, FrSM_BswM_StateType CurrentState)
/* PRQA S 1503,1505,1532 --*/
{
#if (BSWM_FRSMINDICATION_ENABLED == STD_ON)
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
    NetworkHandleType                   numOfFrsmCh;
    NetworkHandleType                   chIdx;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkFrSMInd(CurrentState))
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
            numOfFrsmCh     = modeRqstPCfgPtr->numOfFrSMInd;
            for (chIdx = 0u; chIdx < numOfFrsmCh; chIdx++)
            {
                if (Network == modeRqstLCfgPtr->frsmChRef[chIdx])
                {
                    modeRqstPtr                        = bswmPartPCCfgPtr->modeRqstPortRunPtr;
                    modeRqstPtr->frSMIndiStatus[chIdx] = CurrentState;
                    if (BSWM_IMMEDIATE == modeRqstLCfgPtr->frsmInd[chIdx].process)
                    {
                        numOfRules = modeRqstLCfgPtr->frsmInd[chIdx].belongToRlueNum;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
                            if ((modeRqstLCfgPtr->frsmInd[chIdx].belongToRlue[idx].pbConfigTag
                                 == BswM_GetPBConfigTags())
                                || (BSWM_PB_CONFIG_IN_ALL_VARIANTS
                                    == modeRqstLCfgPtr->frsmInd[chIdx].belongToRlue[idx].pbConfigTag))
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
                            {
                                ruleIdx = modeRqstLCfgPtr->frsmInd[chIdx].belongToRlue[idx].ruleIndex;
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
#endif /*BSWM_FRSMINDICATION_ENABLED == STD_ON*/
}

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_FRSM_ENABLED == STD_ON*/
/* ========================================== internal function definitions ========================================= */
