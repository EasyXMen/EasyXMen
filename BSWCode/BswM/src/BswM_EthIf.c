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
 **  @file               : BswM_EthIf.c
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : BSW Mode Manager
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "BswM_EthIf.h"
#include "BswM_Internal.h"
#if (BSWM_ETHIF_ENABLED == STD_ON)
/* PRQA S 0553 EOF */ /* VL_BswM_0553 */
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
#if (BSWM_ETHIFPORTGROUPLINKSTATECHG_ENABLED == STD_ON)
/**
 * Initializes the Ethernet Interface request ports by setting their initial states.
 * The function retrieves partition configuration pointers, iterates through the Ethernet Interface port groups,
 * and sets their status based on the initial state configuration.
 * If the initialization fails, an error is reported if development error detection is enabled.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_InitEthIfRequestPorts(void)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PartitionPCCfgType*    bswmPartPCCfgPtr;
    const BswM_PartitionLCfgType*     bswmPartLCfgPtr;
    const BswM_ModeRqstPortPCCfgType* modeRqstPCfgPtr;
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
    const BswM_PartitionPbConfigType* bswmPartPbConfigPtr;
#endif /* BSWM_PB_CONFIG_ENABLED */
    const BswM_ModeRqstPortRuntimeType* modeRqstPtr;
    boolean                             result;
    EthIf_SwitchPortGroupIdxType        numOfEthIfPG;
    EthIf_SwitchPortGroupIdxType        pgIdx;

    result = BswM_GetPartitionLcPCCfgPtr(
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
        &bswmPartPbConfigPtr,
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
        &bswmPartPCCfgPtr,
        &bswmPartLCfgPtr);
    if (result)
    {
        modeRqstPCfgPtr = bswmPartPCCfgPtr->modeRqstPCCfg;
        modeRqstPtr     = bswmPartPCCfgPtr->modeRqstPortRunPtr;
        numOfEthIfPG    = modeRqstPCfgPtr->numOfEthIfPG;
        for (pgIdx = 0u; pgIdx < numOfEthIfPG; pgIdx++)
        {
            if (NULL_PTR != modeRqstPCfgPtr->ethIfPGInitState)
            {
                modeRqstPtr->ethIfPGStatus[pgIdx] = *(modeRqstPCfgPtr->ethIfPGInitState);
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
 * Retrieves the link state of a specific Ethernet Interface port group.
 * The function fetches the partition configuration pointer and returns the link state of the specified port group.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
EthTrcv_LinkStateType BswM_GetEthIfGruStateStatus(EthIf_SwitchPortGroupIdxType ethIdx)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
    EthTrcv_LinkStateType          ethState = ETHTRCV_LINK_STATE_DOWN;
    boolean                        result;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        ethState = bswmPartPCCfgPtr->modeRqstPortRunPtr->ethIfPGStatus[ethIdx];
    }
    return ethState;
}
#endif /*BSWM_ETHIFPORTGROUPLINKSTATECHG_ENABLED == STD_ON*/

/**
 * Handles the change in the link state of an Ethernet Interface port group.
 * This function updates the internal state of the specified port group and evaluates relevant rules if necessary.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_EthIf_PortGroupLinkStateChg(EthIf_SwitchPortGroupIdxType PortGroupIdx, EthTrcv_LinkStateType PortGroupState)
/* PRQA S 1503,1505,1532 --*/
{
#if (BSWM_ETHIFPORTGROUPLINKSTATECHG_ENABLED == STD_ON)
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
    EthIf_SwitchPortGroupIdxType        numOfEthIfPG;
    EthIf_SwitchPortGroupIdxType        pgIdx;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkEthIfPortGru(PortGroupState))
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
            numOfEthIfPG    = modeRqstPCfgPtr->numOfEthIfPG;
            for (pgIdx = 0u; pgIdx < numOfEthIfPG; pgIdx++)
            {
#if (BSWM_PB_CONFIG_ENABLED == STD_OFF)
                EthIf_SwitchPortGroupIdxType ethIfPortGroupValue = modeRqstLCfgPtr->ethIfPortGroRef[pgIdx];
#else
                EthIf_SwitchPortGroupIdxType ethIfPortGroupValue = bswmPartPbConfigPtr->ethIfPortGroRef[pgIdx];
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
                if (PortGroupIdx == ethIfPortGroupValue)
                {
                    modeRqstPtr                       = bswmPartPCCfgPtr->modeRqstPortRunPtr;
                    modeRqstPtr->ethIfPGStatus[pgIdx] = PortGroupState;
                    if (BSWM_IMMEDIATE == modeRqstLCfgPtr->ethIfPortGroLink[pgIdx].process)
                    {
                        numOfRules = modeRqstLCfgPtr->ethIfPortGroLink[pgIdx].belongToRlueNum;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
                            if ((modeRqstLCfgPtr->ethIfPortGroLink[pgIdx].belongToRlue[idx].pbConfigTag
                                 == BswM_GetPBConfigTags())
                                || (BSWM_PB_CONFIG_IN_ALL_VARIANTS
                                    == modeRqstLCfgPtr->ethIfPortGroLink[pgIdx].belongToRlue[idx].pbConfigTag))
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
                            {
                                ruleIdx = modeRqstLCfgPtr->ethIfPortGroLink[pgIdx].belongToRlue[idx].ruleIndex;

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
    (void)PortGroupIdx;
    (void)PortGroupState;
#endif /*BSWM_ETHIFPORTGROUPLINKSTATECHG_ENABLED == STD_ON*/
}

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

/* ========================================== internal function definitions ========================================= */
#endif /*BSWM_ETHIF_ENABLED == STD_ON*/
