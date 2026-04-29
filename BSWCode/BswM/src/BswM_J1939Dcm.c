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
 **  @file               : BswM_J1939Dcm.c
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : BSW Mode Manager
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "BswM_J1939Dcm.h"
#include "BswM_Internal.h"
#if (BSWM_J1939DCM_ENABLED == STD_ON)
/* PRQA S 0553 EOF */ /* VL_BswM_0553 */
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
#if (BSWM_J1939DCMBROADCASTSTATUS_ENABLED == STD_ON)
/**
 * Initializes the J1939 DCM request ports by setting the initial broadcast status.
 * This function retrieves configuration data, iterates through the channels, and sets the initial status if provided.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_InitJ1939DcmRequestPorts(void)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PartitionPCCfgType*      bswmPartPCCfgPtr;
    const BswM_ModeRqstPortPCCfgType*   modeRqstPCfgPtr;
    const BswM_ModeRqstPortRuntimeType* modeRqstPtr;
    boolean                             result;
    NetworkHandleType                   numOfj1939DcmCh;
    NetworkHandleType                   chIdx;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        modeRqstPCfgPtr = bswmPartPCCfgPtr->modeRqstPCCfg;
        modeRqstPtr     = bswmPartPCCfgPtr->modeRqstPortRunPtr;
        numOfj1939DcmCh = modeRqstPCfgPtr->numOfj1939DcmCh;
        for (chIdx = 0u; chIdx < numOfj1939DcmCh; chIdx++)
        {
            if (NULL_PTR != modeRqstPCfgPtr->j1939DcmInitStatus)
            {
                modeRqstPtr->j1939DcmBrdcstStatus[chIdx] = *(modeRqstPCfgPtr->j1939DcmInitStatus);
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
 * Retrieves the broadcast status of a specific J1939 channel.
 * This function fetches the configuration data and returns the broadcast status for the given channel index.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
boolean BswM_GetJ1939ChBroadStatus(NetworkHandleType j1939ChIdx)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
    const BswM_PartitionLCfgType*  bswmPartLCfgPtr;
    boolean                        j1939Status = FALSE;
    boolean                        result;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        j1939Status = bswmPartPCCfgPtr->modeRqstPortRunPtr->j1939DcmBrdcstStatus[j1939ChIdx];
    }
    return j1939Status;
}

#endif /*BSWM_J1939DCMBROADCASTSTATUS_ENABLED == STD_ON*/

/**
 * Updates the broadcast status for J1939 channels based on the provided network mask.
 * This function checks the configuration, updates the broadcast status for each channel,
 * and evaluates associated rules if necessary.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_J1939DcmBroadcastStatus(uint16 NetworkMask)
/* PRQA S 1503,1505,1532 --*/
{
#if (BSWM_J1939DCMBROADCASTSTATUS_ENABLED == STD_ON)
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
    NetworkHandleType                   numOfj1939DcmCh;
    NetworkHandleType                   chIdx;
    NetworkHandleType                   j1939BelongNw;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkJ1939DcmBroad())
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
            numOfj1939DcmCh = modeRqstPCfgPtr->numOfj1939DcmCh;
            for (chIdx = 0u; chIdx < numOfj1939DcmCh; chIdx++)
            {
                modeRqstPtr   = bswmPartPCCfgPtr->modeRqstPortRunPtr;
                j1939BelongNw = modeRqstLCfgPtr->j1939DcmBCChRef[chIdx];
                if (0u != (NetworkMask & (NetworkHandleType)((NetworkHandleType)1u << j1939BelongNw)))
                {
                    modeRqstPtr->j1939DcmBrdcstStatus[chIdx] = TRUE;
                }
                else
                {
                    modeRqstPtr->j1939DcmBrdcstStatus[chIdx] = FALSE;
                }
                if (BSWM_IMMEDIATE == modeRqstLCfgPtr->j1939DcmBC[chIdx].process)
                {
                    numOfRules = modeRqstLCfgPtr->j1939DcmBC[chIdx].belongToRlueNum;
                    for (idx = 0u; idx < numOfRules; idx++)
                    {
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
                        if ((modeRqstLCfgPtr->j1939DcmBC[chIdx].belongToRlue[idx].pbConfigTag == BswM_GetPBConfigTags())
                            || (BSWM_PB_CONFIG_IN_ALL_VARIANTS
                                == modeRqstLCfgPtr->j1939DcmBC[chIdx].belongToRlue[idx].pbConfigTag))
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
                        {
                            ruleIdx = modeRqstLCfgPtr->j1939DcmBC[chIdx].belongToRlue[idx].ruleIndex;
                            /* An Independent rule,to be evaluated each time applicable*/
                            BswM_ArbitrateRule(bswmPartLCfgPtr, ruleIdx, FALSE);
                        }
                    }
                }
            }
        }
    }
#else
    (void)NetworkMask;
#endif /*BSWM_J1939DCMBROADCASTSTATUS_ENABLED == STD_ON*/
}

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
/* ========================================== internal function definitions ========================================= */
#endif /*BSWM_J1939DCM_ENABLED == STD_ON*/
