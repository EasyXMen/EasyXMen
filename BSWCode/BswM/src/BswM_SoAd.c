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
 **  @file               : BswM_SoAd.c
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : BSW Mode Manager
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "BswM_SoAd.h"
#include "BswM_Internal.h"
#if (BSWM_SOAD_ENABLED == STD_ON)
/* PRQA S 0553 EOF */ /* VL_BswM_0553 */
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"

#if (BSWM_SOADSOCONMODECHG_ENABLED == STD_ON)
/**
 * Initializes the SoAd request ports by setting the initial states of the SoAd SoCon modes.
 * This function retrieves the partition configuration and initializes the SoAd SoCon mode states based on the provided
 * configuration. If the configuration retrieval fails, an error is reported if development error detection is enabled.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_InitSoAdRequestPorts(void)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PartitionPCCfgType*      bswmPartPCCfgPtr;
    const BswM_ModeRqstPortPCCfgType*   modeRqstPCfgPtr;
    const BswM_ModeRqstPortRuntimeType* modeRqstPtr;
    boolean                             result;
    uint16                              lnumOfSoadSoConMode;
    uint16                              idx;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        modeRqstPCfgPtr     = bswmPartPCCfgPtr->modeRqstPCCfg;
        modeRqstPtr         = bswmPartPCCfgPtr->modeRqstPortRunPtr;
        lnumOfSoadSoConMode = modeRqstPCfgPtr->numOfSoadSoConMode;

        for (idx = 0u; idx < lnumOfSoadSoConMode; idx++)
        {
            if (NULL_PTR != modeRqstPCfgPtr->curSoComModeInitState)
            {
                modeRqstPtr->soAdSoConModeState[idx] = *(modeRqstPCfgPtr->curSoComModeInitState);
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
 * Retrieves the current state of a specified SoAd SoCon mode.
 * This function fetches the partition configuration and returns the current state of the SoAd SoCon mode.
 * If the partition configuration is successfully retrieved, it reads the current state from the runtime data.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
SoAd_SoConModeType BswM_GetSoComModeCurState(uint16 SoConIdx)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
    SoAd_SoConModeType             SoConModeCurState = SOAD_SOCON_OFFLINE;
    boolean                        result;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        SoConModeCurState = bswmPartPCCfgPtr->modeRqstPortRunPtr->soAdSoConModeState[SoConIdx];
    }
    return SoConModeCurState;
}
#endif /*BSWM_SOADSOCONMODECHG_ENABLED == STD_ON*/

/**
 * Changes the state of a specified SoAd SoCon mode.
 * This function updates the state of the specified SoAd SoCon mode and processes any associated rules if required.
 * It retrieves the necessary configuration and runtime data, checks for the specified SoAd SoCon ID, and updates its
 * state. If the change process is immediate, it also evaluates the associated rules.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_SoAd_SoConModeChg(SoAd_SoConIdType SoConId, SoAd_SoConModeType State)
/* PRQA S 1503,1505,1532 --*/
{
#if (BSWM_SOADSOCONMODECHG_ENABLED == STD_ON)
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
    uint16                              lnumOfSoadSoConMode;
    uint16                              cliIdx;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkSoAdSoConModeChg())
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
            modeRqstPCfgPtr     = bswmPartPCCfgPtr->modeRqstPCCfg;
            modeRqstLCfgPtr     = bswmPartLCfgPtr->modeRqstLCfg;
            lnumOfSoadSoConMode = modeRqstPCfgPtr->numOfSoadSoConMode;
            for (cliIdx = 0u; cliIdx < lnumOfSoadSoConMode; cliIdx++)
            {
                if (SoConId == modeRqstLCfgPtr->soadSoConIdRef[cliIdx])
                {
                    modeRqstPtr                             = bswmPartPCCfgPtr->modeRqstPortRunPtr;
                    modeRqstPtr->soAdSoConModeState[cliIdx] = State;
                    if (BSWM_IMMEDIATE == modeRqstLCfgPtr->soadSoconModeChange[cliIdx].process)
                    {
                        numOfRules = modeRqstLCfgPtr->soadSoconModeChange[cliIdx].belongToRlueNum;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
                            if ((modeRqstLCfgPtr->soadSoconModeChange[cliIdx].belongToRlue[idx].pbConfigTag
                                 == BswM_GetPBConfigTags())
                                || (BSWM_PB_CONFIG_IN_ALL_VARIANTS
                                    == modeRqstLCfgPtr->soadSoconModeChange[cliIdx].belongToRlue[idx].pbConfigTag))
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
                            {
                                ruleIdx = modeRqstLCfgPtr->soadSoconModeChange[cliIdx].belongToRlue[idx].ruleIndex;
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
#endif /*BSWM_SOADSOCONMODECHG_ENABLED == STD_ON*/
}

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
/* ========================================== internal function definitions ========================================= */

#endif /*BSWM_SOAD_ENABLED == STD_ON*/
