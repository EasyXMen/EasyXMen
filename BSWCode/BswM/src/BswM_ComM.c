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
 **  @file               : BswM_ComM.c
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : BSW Mode Manager
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "BswM_ComM.h"
#include "BswM_Internal.h"
#if (BSWM_COMM_ENABLED == STD_ON)
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
 * This function initializes the request ports for ComM communication.
 * It retrieves the necessary configuration pointers and sets the initial status for ComM indications.
 * If PnC (Power and Communication) requests are enabled, it also initializes the PnC request status.
 * If any initialization fails, it reports an error.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_InitComMRequestPorts(void)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_ModeRqstPortPCCfgType*   modeRqstPCfgPtr;
    const BswM_ModeRqstPortRuntimeType* modeRqstPtr;
    const BswM_PartitionPCCfgType*      bswmPartPCCfgPtr;
    NetworkHandleType                   numOfComMCh;
    NetworkHandleType                   idx;
#if (BSWM_COMM_PNC_ENABLED == STD_ON)
    PNCHandleType numOfPnc;
    PNCHandleType pncIdx;
#endif /*BSWM_COMM_PNC_ENABLED == STD_ON*/
    boolean result;
    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
#if (BSWM_COMMINDICATION_ENABLED == STD_ON)
        modeRqstPCfgPtr = bswmPartPCCfgPtr->modeRqstPCCfg;
        modeRqstPtr     = bswmPartPCCfgPtr->modeRqstPortRunPtr;
        numOfComMCh     = modeRqstPCfgPtr->numOfComMInd;
        for (idx = 0u; idx < numOfComMCh; idx++)
        {
            if (NULL_PTR != modeRqstPCfgPtr->comMInitMode)
            {
                modeRqstPtr->comMIndiStatus[idx] = *(modeRqstPCfgPtr->comMInitMode);
            }
            else
            {
                break;
            }
        }
#endif /*BSWM_COMMINDICATION_ENABLED == STD_ON*/
#if (BSWM_COMMPNCREQUEST_ENABLED == STD_ON)
        numOfPnc = modeRqstPCfgPtr->numOfComMPncInd;
        for (pncIdx = 0u; pncIdx < numOfPnc; pncIdx++)
        {
            if (NULL_PTR != modeRqstPCfgPtr->comMPncInitMode)
            {
                modeRqstPtr->comMPncReqStatus[pncIdx] = *(modeRqstPCfgPtr->comMPncInitMode);
            }
            else
            {
                break;
            }
        }
#endif /*BSWM_COMMPNCREQUEST_ENABLED == STD_ON*/
    }
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    else
    {
        (void)Det_ReportError(BSWM_MODULE_ID, BSWM_INSTANCE_ID, BSWM_API_ID_INIT, BSWM_E_INIT_FAILED);
    }
#endif /*BSWM_DEV_ERROR_DETECT == STD_ON*/
}

/**
 * Function called by ComM to signal a shutdown.
 * Service ID: 0x22
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_ComM_InitiateReset(void)
/* PRQA S 1503,1505,1532 --*/
{
#if (BSWM_EVENT_COMM_INITIATE_RESET_LIST_ENABLE == STD_ON)
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
    const BswM_PartitionLCfgType*  bswmPartLCfgPtr;
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
    const BswM_PartitionPbConfigType* bswmPartPbConfigPtr;
#endif /* BSWM_PB_CONFIG_ENABLED */
    const BswM_EventRqstPortLCfgType* evRqstLCfgPtr;
    BswM_EventRquestPortRuntimeType*  evRqstPtr;
    BswM_RuleIndexType                numOfRules;
    BswM_RuleIndexType                idx;
    BswM_RuleIndexType                ruleIdx;
    BswM_EventRqstPortIdxType         evIdx;
    boolean                           result;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkComMInitRst())
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
            evIdx            = *(bswmPartPCCfgPtr->evRqstPCCfg->comMInitRstIdxPtr);
            evRqstPtr[evIdx] = BSWM_EVENT_IS_SET;
            evRqstLCfgPtr    = bswmPartLCfgPtr->evRqstLCfg;
            if (BSWM_IMMEDIATE == evRqstLCfgPtr->comMInitReset->process)
            {
                numOfRules = evRqstLCfgPtr->comMInitReset->belongToRlueNum;
                for (idx = 0u; idx < numOfRules; idx++)
                {
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
                    if ((evRqstLCfgPtr->comMInitReset->belongToRlue[idx].pbConfigTag == BswM_GetPBConfigTags())
                        || (BSWM_PB_CONFIG_IN_ALL_VARIANTS
                            == evRqstLCfgPtr->comMInitReset->belongToRlue[idx].pbConfigTag))
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
                    {
                        ruleIdx = evRqstLCfgPtr->comMInitReset->belongToRlue[idx].ruleIndex;

                        /* An Independent rule,to be evaluated each time applicable*/
                        BswM_ArbitrateRule(bswmPartLCfgPtr, ruleIdx, FALSE);
                    }
                }
            }
        }
    }
#endif /*BSWM_EVENT_COMM_INITIATE_RESET_LIST_ENABLE == STD_ON*/
}

/**
 * Function called by ComM to indicate the current communication mode of a ComM
 * channel.
 * Service ID: 0x0e
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): Network, the ComM communication channel that the indicated
 *                          state corresponds to
 *                 RequestedMode, the current state of the ComM communication channel
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
/* PRQA S 1503,1505,1532,6030 ++ */ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef, VL_MTR_BswM_STMIF */
void BswM_ComM_CurrentMode(NetworkHandleType Network, ComM_ModeType RequestedMode)
/* PRQA S 1503,1505,1532,6030 -- */
{
#if (BSWM_COMMINDICATION_ENABLED == STD_ON)
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
    NetworkHandleType                   numOfComMCh;
    NetworkHandleType                   chIdx;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkComMInd(RequestedMode))
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
            numOfComMCh     = modeRqstPCfgPtr->numOfComMInd;
            for (chIdx = 0u; chIdx < numOfComMCh; chIdx++)
            {
                if (Network == modeRqstLCfgPtr->comMIndChRef[chIdx])
                {
                    modeRqstPtr                        = bswmPartPCCfgPtr->modeRqstPortRunPtr;
                    modeRqstPtr->comMIndiStatus[chIdx] = RequestedMode;
                    if (BSWM_IMMEDIATE == modeRqstLCfgPtr->comMInd[chIdx].process)
                    {
                        numOfRules = modeRqstLCfgPtr->comMInd[chIdx].belongToRlueNum;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
                            if ((modeRqstLCfgPtr->comMInd[chIdx].belongToRlue[idx].pbConfigTag
                                 == BswM_GetPBConfigTags())
                                || (BSWM_PB_CONFIG_IN_ALL_VARIANTS
                                    == modeRqstLCfgPtr->comMInd[chIdx].belongToRlue[idx].pbConfigTag))
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
                            {
                                ruleIdx = modeRqstLCfgPtr->comMInd[chIdx].belongToRlue[idx].ruleIndex;

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
#endif /* BSWM_COMMINDICATION_ENABLED == STD_ON */
}

/**
 * Function called by ComM to indicate the current mode of the PNC.
 * Service ID: 0x15
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): PNC, the handle of the PNC for which the current state is reported
 *                 CurrentPncMode, the current mode of the PNC
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_ComM_CurrentPNCMode(PNCHandleType PNC, ComM_PncModeType CurrentPncMode)
/* PRQA S 1503,1505,1532 --*/
{
#if (BSWM_COMMPNCREQUEST_ENABLED == STD_ON)
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
    PNCHandleType                       numOfComMPncInd;
    PNCHandleType                       pncIdx;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkComMPnc(CurrentPncMode))
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
            numOfComMPncInd = modeRqstPCfgPtr->numOfComMPncInd;
            for (pncIdx = 0u; pncIdx < numOfComMPncInd; pncIdx++)
            {
                if (PNC == modeRqstLCfgPtr->comMPncRef[pncIdx])
                {
                    modeRqstPtr                           = bswmPartPCCfgPtr->modeRqstPortRunPtr;
                    modeRqstPtr->comMPncReqStatus[pncIdx] = CurrentPncMode;
                    if (BSWM_IMMEDIATE == modeRqstLCfgPtr->comMPncRqst[pncIdx].process)
                    {
                        numOfRules = modeRqstLCfgPtr->comMPncRqst[pncIdx].belongToRlueNum;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
                            if ((modeRqstLCfgPtr->comMPncRqst[pncIdx].belongToRlue[idx].pbConfigTag
                                 == BswM_GetPBConfigTags())
                                || (BSWM_PB_CONFIG_IN_ALL_VARIANTS
                                    == modeRqstLCfgPtr->comMPncRqst[pncIdx].belongToRlue[idx].pbConfigTag))
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
                            {
                                ruleIdx = modeRqstLCfgPtr->comMPncRqst[pncIdx].belongToRlue[idx].ruleIndex;

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
    (void)PNC;
    (void)CurrentPncMode;
#endif /*BSWM_COMMPNCREQUEST_ENABLED == STD_ON*/
}

#if (BSWM_COMMPNCREQUEST_ENABLED == STD_ON)
/*Get ComM Pnc status*/
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
ComM_PncModeType BswM_GetComMPncStatus(PNCHandleType pncIdx)
/* PRQA S 1503,1505,1532 --*/
{
    ComM_PncModeType               pncStatus = COMM_PNC_NO_COMMUNICATION;
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
    boolean                        result;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        pncStatus = bswmPartPCCfgPtr->modeRqstPortRunPtr->comMPncReqStatus[pncIdx];
    }

    return pncStatus;
}
#endif /*BSWM_COMMPNCREQUEST_ENABLED == STD_ON*/

#if (BSWM_EVENT_COMM_INITIATE_RESET_LIST_ENABLE == STD_ON)
/*Get comM initiateReset status*/
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
BswM_EventRquestPortRuntimeType BswM_GetComMResetInitStatus(void)
/* PRQA S 1503,1505,1532 --*/
{
    BswM_EventRquestPortRuntimeType comMInitStatus = BSWM_INVALID_U8;
    const BswM_PartitionPCCfgType*  bswmPartPCCfgPtr;
    BswM_EventRqstPortIdxType       evIdx;
    boolean                         result;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        evIdx          = *(bswmPartPCCfgPtr->evRqstPCCfg->comMInitRstIdxPtr);
        comMInitStatus = bswmPartPCCfgPtr->eventRqstPortRunPtr[evIdx];
    }
    return comMInitStatus;
}
#endif /* BSWM_EVENT_COMM_INITIATE_RESET_LIST_ENABLE == STD_ON */

#if (BSWM_COMMINDICATION_ENABLED == STD_ON)
/*Get ComM mode status*/
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
ComM_ModeType BswM_GetComMIndStatus(NetworkHandleType comChIdx)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
    ComM_ModeType                  comMode = BSWM_INVALID_U8;
    boolean                        result;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        comMode = bswmPartPCCfgPtr->modeRqstPortRunPtr->comMIndiStatus[comChIdx];
    }
    return comMode;
}
#endif /*BSWM_COMMINDICATION_ENABLED == STD_ON*/

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

/* ========================================== internal function definitions ========================================= */

#endif /*BSWM_COMM_ENABLED == STD_ON*/
