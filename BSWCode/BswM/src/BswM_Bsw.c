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
 **  @file               : BswM_Bsw.c
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : BSW Mode Manager
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "BswM_Bsw.h"
#include "BswM_Internal.h"
/* PRQA S 0553 EOF */ /* VL_BswM_0553 */
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#if (BSWM_BSWMODENOTIFICATION_ENABLED == STD_ON)
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"

/**
 * This function handles the notification of a change in the BSW mode group. It retrieves configuration pointers,
 * checks the initialization status of the core, and iterates through the mode request configurations to find the
 * relevant rules associated with the specified mode group. Once found, it arbitrates each rule.
 */
void BswM_BswModeNotification(BswM_ModeGroupType modeGroup)
{
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
    const BswM_PartitionLCfgType*  bswmPartLCfgPtr;
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
    const BswM_PartitionPbConfigType* bswmPartPbConfigPtr;
#endif /* BSWM_PB_CONFIG_ENABLED */
    const BswM_ModeRqstPortPCCfgType*   modeRqstPCfgPtr;
    const BswM_ModeRqstPortLCfgType*    modeRqstLCfgPtr;
    const BswM_ModeRqstPortRuntimeType* modeRqstPtr;
    BswM_RuleIndexType                  numOfRules;
    BswM_RuleIndexType                  idx;
    BswM_RuleIndexType                  ruleIdx;
    boolean                             result;
    uint8                               numOfBswModeNoti;
    uint8                               bswIdx;
    uint8                               coreIdx = 0u;

    if (BswM_GetCoreInitStatus())
    {
        result = BswM_GetPartitionLcPCCfgPtr(
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
            &bswmPartPbConfigPtr,
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
            &bswmPartPCCfgPtr,
            &bswmPartLCfgPtr);
        if (result)
        {
            modeRqstPCfgPtr  = bswmPartPCCfgPtr->modeRqstPCCfg;
            modeRqstLCfgPtr  = bswmPartLCfgPtr->modeRqstLCfg;
            numOfBswModeNoti = modeRqstPCfgPtr->numOfBswModeNoti;
            for (bswIdx = 0u; bswIdx < numOfBswModeNoti; bswIdx++)
            {
                if (modeGroup == modeRqstLCfgPtr->bswModeNotiRef[bswIdx])
                {
                    numOfRules = modeRqstLCfgPtr->bswModeNoti[bswIdx].belongToRlueNum;
                    for (idx = 0u; idx < numOfRules; idx++)
                    {
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
                        if ((modeRqstLCfgPtr->bswModeNoti[bswIdx].belongToRlue[idx].pbConfigTag
                             == BswM_GetPBConfigTags())
                            || (BSWM_PB_CONFIG_IN_ALL_VARIANTS
                                == modeRqstLCfgPtr->bswModeNoti[bswIdx].belongToRlue[idx].pbConfigTag))
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
                        {
                            ruleIdx = modeRqstLCfgPtr->bswModeNoti[bswIdx].belongToRlue[idx].ruleIndex;
                            /* An Independent rule,to be evaluated each time applicable*/
                            BswM_ArbitrateRule(bswmPartLCfgPtr, ruleIdx, FALSE);
                        }
                    }
                    break;
                }
            }
        }
    }
}

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_BSWMODENOTIFICATION_ENABLED == STD_ON*/
/* ========================================== internal function definitions ========================================= */
