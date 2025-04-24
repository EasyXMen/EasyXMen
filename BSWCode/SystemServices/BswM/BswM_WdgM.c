/* PRQA S 3108++ */
/**
 * Copyright (C) 2024 Isoft Infrastructure Software Co., Ltd.
 * SPDX-License-Identifier: LGPL-2.1-only-with-exception OR  LicenseRef-Commercial-License
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the
 * GNU Lesser General Public License as published by the Free Software Foundation; version 2.1.
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License along with this library;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * or see <https://www.gnu.org/licenses/>.
 *
 * Alternatively, this file may be used under the terms of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License, in which case the provisions of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License shall apply instead of those of the GNU Lesser General Public License.
 *
 * You should have received a copy of the Isoft Infrastructure Software Co., Ltd.  Commercial License
 * along with this program. If not, please find it at <https://EasyXMen.com/xy/reference/permissions.html>
 */
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : BswM_WdgM.c                                                 **
**                                                                            **
**  Created on  : 2020-03-24                                                  **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "BswM_WdgM.h"

#if (BSWM_WDGM_ENABLED == STD_ON)
/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"

/**
 * Function called by WdgM to request a partition reset.
 * Service ID: 0x11
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): Application, The identifier of an OS-Application
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BSWM_WDGM_CODE)
BswM_WdgM_RequestPartitionReset(ApplicationType Application)
{
#if (BSWM_EVENT_RQSTPORT_ENABLE == STD_ON)
    P2CONST(BswM_EventRqstPortLCfgType, AUTOMATIC, BSWM_CONST) evRqstLCfgPtr;
    P2VAR(BswM_EventRquestPortRuntimeType, AUTOMATIC, BSWM_VAR_CLEARED) evRqstPtr;
    P2CONST(BswM_RuleLcCfgType, AUTOMATIC, BSWM_CONST) ruleLCfgPtr;
    BswM_RuleIndexType numOfRules;
    BswM_RuleIndexType idx;
    BswM_RuleIndexType ruleIdx;
    BswM_EventRqstPortIdxType evIdx;
    ApplicationType partIdx;
    boolean result;
    uint8 numOfWdgmRstEv;
    uint8 wdgIdx;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkWgmParRst())
#endif /*BSWM_DEV_ERROR_DETECT == STD_ON*/
    {
        result = BswM_GetPartitionIdx(&partIdx);
        if ((boolean)TRUE == result)
        {
            numOfWdgmRstEv = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].evRqstPCCfg->numOfWdgmRstEv;
            evRqstLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].evRqstLCfg;
            for (wdgIdx = 0u; wdgIdx < numOfWdgmRstEv; wdgIdx++)
            {
                if (Application == evRqstLCfgPtr->wdgAppRef[wdgIdx])
                {
                    evIdx = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].evRqstPCCfg->wdgmRqstParRstEvIdxPtr[wdgIdx];
                    evRqstPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].eventRqstPortRunPtr;
                    evRqstPtr[evIdx] = BSWM_EVENT_IS_SET;
                    if (BSWM_IMMEDIATE == evRqstLCfgPtr->wdgMRqstParReset[wdgIdx].process)
                    {
                        numOfRules = evRqstLCfgPtr->wdgMRqstParReset[wdgIdx].belongToRlueNum;
                        ruleLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].ruleLCfg;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
                            ruleIdx = evRqstLCfgPtr->wdgMRqstParReset[wdgIdx].belongToRlue[idx];
                            if ((boolean)FALSE == ruleLCfgPtr[ruleIdx].isNestExe)
                            {
                                /* An Independent rule,to be evaluated each time applicable*/
                                BswM_ArbitrateRule(ruleIdx, partIdx);
                            }
                        }
                    }
                    break;
                }
            }
        }
    }
#else
    BSWM_PARA_UNUSED(Application);
#endif /*BSWM_EVENT_RQSTPORT_ENABLE == STD_ON*/
}

/*Get BswM_WdgM_RequestPartitionReset*/
FUNC(BswM_EventRquestPortRuntimeType, BSWM_NMIF_CODE)
BswM_GetWgmParReset(uint8 wdgIdx)
{
    BswM_EventRquestPortRuntimeType ret = BSWM_INVALID_U8;
#if (BSWM_EVENT_RQSTPORT_ENABLE == STD_ON)
    ApplicationType partIdx;
    boolean result;
    BswM_EventRquestPortRuntimeType evIdx;

    result = BswM_GetPartitionIdx(&partIdx);
    if ((boolean)TRUE == result)
    {
        evIdx = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].evRqstPCCfg->wdgmRqstParRstEvIdxPtr[wdgIdx];
        ret = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].eventRqstPortRunPtr[evIdx];
    }
#else
    BSWM_PARA_UNUSED(wdgIdx);
#endif /*BSWM_EVENT_RQSTPORT_ENABLE == STD_ON*/
    return ret;
}

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/

#endif /*BSWM_WDGM_ENABLED == STD_ON*/
