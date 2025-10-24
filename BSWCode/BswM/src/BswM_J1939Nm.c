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
 **  @file               : BswM_J1939Nm.c
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : BSW Mode Manager
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "BswM_J1939Nm.h"
#include "BswM_Internal.h"
#if (BSWM_J1939NM_ENABLED == STD_ON)
/* PRQA S 0553 EOF */ /* VL_BswM_0553 */
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
#if (BSWM_J1939NMINDICATION_ENABLED == STD_ON)
/**
 * Initializes the J1939 network management request ports by setting the initial status.
 * This function retrieves the configuration, sets the initial status for each channel,
 * and reports an error if the configuration is not valid.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_InitJ1939NmRequestPorts(void)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PartitionPCCfgType*      bswmPartPCCfgPtr;
    const BswM_ModeRqstPortPCCfgType*   modeRqstPCfgPtr;
    const BswM_ModeRqstPortRuntimeType* modeRqstPtr;
    boolean                             result;
    NetworkHandleType                   numOfJ1939NmInd;
    NetworkHandleType                   chIdx;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        modeRqstPCfgPtr = bswmPartPCCfgPtr->modeRqstPCCfg;
        modeRqstPtr     = bswmPartPCCfgPtr->modeRqstPortRunPtr;
        numOfJ1939NmInd = modeRqstPCfgPtr->numOfJ1939NmInd;
        for (chIdx = 0u; chIdx < numOfJ1939NmInd; chIdx++)
        {
            if (NULL_PTR != modeRqstPCfgPtr->j1939NmInitState)
            {
                modeRqstPtr->j1939NmIndiStatus[chIdx] = *(modeRqstPCfgPtr->j1939NmInitState);
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
 * Retrieves the current status of the J1939 network management for a specific channel.
 * This function fetches the configuration and returns the status of the specified channel.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
Nm_StateType BswM_GetJ1939NmStatus(NetworkHandleType j1939ChIdx)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
    Nm_StateType                   retStatus = NM_STATE_UNINIT;
    boolean                        result;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        retStatus = bswmPartPCCfgPtr->modeRqstPortRunPtr->j1939NmIndiStatus[j1939ChIdx];
    }
    return retStatus;
}
#endif /*BSWM_J1939NMINDICATION_ENABLED == STD_ON*/

/**
 * Handles the state change notification for the J1939 network management.
 * This function updates the internal state of the specified network and node,
 * and triggers the evaluation of associated rules if necessary.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_J1939Nm_StateChangeNotification(NetworkHandleType Network, uint8 Node, Nm_StateType NmState)
/* PRQA S 1503,1505,1532 --*/
{
#if (BSWM_J1939NMINDICATION_ENABLED == STD_ON)
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
    NetworkHandleType                   numOfJ1939NmInd;
    NetworkHandleType                   chIdx;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkJ1939NmInd())
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
            numOfJ1939NmInd = modeRqstPCfgPtr->numOfJ1939NmInd;
            for (chIdx = 0u; chIdx < numOfJ1939NmInd; chIdx++)
            {
                if ((Network == modeRqstLCfgPtr->j1939NmChRef[chIdx])
                    && (Node == modeRqstLCfgPtr->j1939NmNodeRef[chIdx]))
                {
                    modeRqstPtr                           = bswmPartPCCfgPtr->modeRqstPortRunPtr;
                    modeRqstPtr->j1939NmIndiStatus[chIdx] = NmState;
                    if (BSWM_IMMEDIATE == modeRqstLCfgPtr->j1939NmInd[chIdx].process)
                    {
                        numOfRules = modeRqstLCfgPtr->j1939NmInd[chIdx].belongToRlueNum;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
                            if ((modeRqstLCfgPtr->j1939NmInd[chIdx].belongToRlue[idx].pbConfigTag
                                 == BswM_GetPBConfigTags())
                                || (BSWM_PB_CONFIG_IN_ALL_VARIANTS
                                    == modeRqstLCfgPtr->j1939NmInd[chIdx].belongToRlue[idx].pbConfigTag))
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
                            {
                                ruleIdx = modeRqstLCfgPtr->j1939NmInd[chIdx].belongToRlue[idx].ruleIndex;
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
    (void)Node;
    (void)NmState;
#endif /*BSWM_J1939NMINDICATION_ENABLED == STD_ON*/
}

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
/* ========================================== internal function definitions ========================================= */
#endif /*BSWM_J1939NM_ENABLED == STD_ON*/
