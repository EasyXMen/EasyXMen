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
 **  @file               : BswM_LinTp.c
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : BSW Mode Manager
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "BswM_LinTp.h"
#include "BswM_Internal.h"
#if (BSWM_LINTP_ENABLED == STD_ON)
/* PRQA S 0553 EOF */ /* VL_BswM_0553 */
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
#if (BSWM_LINTPMODEREQUEST_ENABLED == STD_ON)
/**
 * Initializes the LIN TP request ports with their initial modes.
 * This function sets the initial mode for each LIN TP request port based on the configuration.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_InitLinTpRequestPorts(void)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PartitionPCCfgType*      bswmPartPCCfgPtr;
    const BswM_ModeRqstPortPCCfgType*   modeRqstPCfgPtr;
    const BswM_ModeRqstPortRuntimeType* modeRqstPtr;
    boolean                             result;
    NetworkHandleType                   numOfLinTpModeReq;
    NetworkHandleType                   lintpIdx;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        modeRqstPCfgPtr   = bswmPartPCCfgPtr->modeRqstPCCfg;
        modeRqstPtr       = bswmPartPCCfgPtr->modeRqstPortRunPtr;
        numOfLinTpModeReq = modeRqstPCfgPtr->numOfLinTpModeReq;
        for (lintpIdx = 0u; lintpIdx < numOfLinTpModeReq; lintpIdx++)
        {
            if (NULL_PTR != modeRqstPCfgPtr->lintpRqstInitMode)
            {
                modeRqstPtr->linTpModeReqStatus[lintpIdx] = *(modeRqstPCfgPtr->lintpRqstInitMode);
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
 * Retrieves the requested mode of the specified LIN TP channel.
 * This function returns the current requested mode for the given LIN TP channel.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
LinTp_Mode BswM_GetLinTpRqstMode(NetworkHandleType linTpChIdx)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
    LinTp_Mode                     retRqstMode = LINTP_APPLICATIVE_SCHEDULE;
    boolean                        result;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        retRqstMode = bswmPartPCCfgPtr->modeRqstPortRunPtr->linTpModeReqStatus[linTpChIdx];
    }
    return retRqstMode;
}
#endif /*BSWM_LINTPMODEREQUEST_ENABLED == STD_ON*/

/**
 * Requests a mode change for the specified LIN TP channel.
 * This function processes the mode request and updates the internal state accordingly.
 * If the immediate processing flag is set, it also evaluates the associated rules.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_LinTp_RequestMode(NetworkHandleType Network, LinTp_Mode LinTpRequestedMode)
/* PRQA S 1503,1505,1532 --*/
{
#if (BSWM_LINTPMODEREQUEST_ENABLED == STD_ON)
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
    NetworkHandleType                   numOfLinTpModeReq;
    NetworkHandleType                   lintpIdx;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkLinTpRqst(LinTpRequestedMode))
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
            modeRqstPCfgPtr   = bswmPartPCCfgPtr->modeRqstPCCfg;
            modeRqstLCfgPtr   = bswmPartLCfgPtr->modeRqstLCfg;
            numOfLinTpModeReq = modeRqstPCfgPtr->numOfLinTpModeReq;
            for (lintpIdx = 0u; lintpIdx < numOfLinTpModeReq; lintpIdx++)
            {
                if (Network == modeRqstLCfgPtr->lintpChRef[lintpIdx])
                {
                    modeRqstPtr                               = bswmPartPCCfgPtr->modeRqstPortRunPtr;
                    modeRqstPtr->linTpModeReqStatus[lintpIdx] = LinTpRequestedMode;
                    if (BSWM_IMMEDIATE == modeRqstLCfgPtr->lintpModeRqst[lintpIdx].process)
                    {
                        numOfRules = modeRqstLCfgPtr->lintpModeRqst[lintpIdx].belongToRlueNum;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
                            if ((modeRqstLCfgPtr->lintpModeRqst[lintpIdx].belongToRlue[idx].pbConfigTag
                                 == BswM_GetPBConfigTags())
                                || (BSWM_PB_CONFIG_IN_ALL_VARIANTS
                                    == modeRqstLCfgPtr->lintpModeRqst[lintpIdx].belongToRlue[idx].pbConfigTag))
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
                            {
                                ruleIdx = modeRqstLCfgPtr->lintpModeRqst[lintpIdx].belongToRlue[idx].ruleIndex;

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
    (void)LinTpRequestedMode;
#endif /*BSWM_LINTPMODEREQUEST_ENABLED == STD_ON*/
}

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
/* ========================================== internal function definitions ========================================= */
#endif /*BSWM_LINTP_ENABLED == STD_ON*/
