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
 *
 ********************************************************************************
 **                                                                            **
 **  FILENAME    : BswM_Nm.c                                                   **
 **                                                                            **
 **  Created on  : 2020-03-24                                                  **
 **  Author      : qinchun.yang                                                **
 **  Vendor      :                                                             **
 **  DESCRIPTION :                                                             **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
 **                                                                            **
 *******************************************************************************/
/* PRQA S 3108-- */

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "BswM_Nm.h"
#if (BSWM_NM_ENABLED == STD_ON)
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
 * Function called by NmIf to indicate a CarWakeup.
 * Service ID: 0x24
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Network, Identification of the Nm-Channel
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BSWM_NMIF_CODE)
BswM_Nm_CarWakeUpIndication(NetworkHandleType Network)
{
#if (BSWM_EVENT_RQSTPORT_ENABLE == STD_ON)
    P2CONST(BswM_EventRqstPortLCfgType, AUTOMATIC, BSWM_CONST) evRqstLCfgPtr;
    /* PRQA S 3432 ++*/ /* MISRA Rule 20.7 */
    P2VAR(BswM_EventRquestPortRuntimeType, AUTOMATIC, BSWM_VAR_CLEARED) evRqstPtr;
    /* PRQA S 3432 --*/ /* MISRA Rule 20.7 */
    P2CONST(BswM_RuleLcCfgType, AUTOMATIC, BSWM_CONST) ruleLCfgPtr;
    P2CONST(BswM_PartitionPCCfgType, TYPEDEF, BSWM_CONST) bswmPartPCCfgs;
    P2CONST(BswM_PartitionLCfgType, TYPEDEF, BSWM_CONST) bswmPartLCfgs;
    BswM_RuleIndexType numOfRules;
    BswM_RuleIndexType idx;
    BswM_RuleIndexType ruleIdx;
    BswM_EventRqstPortIdxType evIdx;
    ApplicationType partIdx;
    boolean result;
    NetworkHandleType numOfNmWkCh;
    NetworkHandleType chIdx;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkNmCarWkUp())
#endif /* BSWM_DEV_ERROR_DETECT == STD_ON */
    {
        result = BswM_GetPartitionIdx(&partIdx);
        if ((boolean)TRUE == result)
        {
            bswmPartPCCfgs = &(BswM_RuntimeStatus.bswmPartPCCfgs[partIdx]);
            bswmPartLCfgs = &(BswM_RuntimeStatus.bswmPartLCfgs[partIdx]);
            numOfNmWkCh = bswmPartPCCfgs->evRqstPCCfg->numOfNmWkCh;
            evRqstLCfgPtr = bswmPartLCfgs->evRqstLCfg;
            for (chIdx = 0u; chIdx < numOfNmWkCh; chIdx++)
            {
                if (Network == evRqstLCfgPtr->nmWkChRef[chIdx])
                {
                    evIdx = bswmPartPCCfgs->evRqstPCCfg->nmWkEvPortIdxPtr[chIdx];
                    evRqstPtr = bswmPartPCCfgs->eventRqstPortRunPtr;
                    evRqstPtr[evIdx] = BSWM_EVENT_IS_SET;
                    if (BSWM_IMMEDIATE == evRqstLCfgPtr->nmWakeUpInd[chIdx].process)
                    {
                        numOfRules = evRqstLCfgPtr->nmWakeUpInd[chIdx].belongToRlueNum;
                        ruleLCfgPtr = bswmPartLCfgs->ruleLCfg;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
                            ruleIdx = evRqstLCfgPtr->nmWakeUpInd[chIdx].belongToRlue[idx];
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
#endif /* BSWM_EVENT_RQSTPORT_ENABLE == STD_ON */
}

#if (BSWM_EVENT_RQSTPORT_ENABLE == STD_ON)
/*Get Nm wake-up indication*/
FUNC(BswM_EventRquestPortRuntimeType, BSWM_NMIF_CODE)
BswM_GetNmWkInd(NetworkHandleType nmIdx)
{
    P2CONST(BswM_PartitionPCCfgType, TYPEDEF, BSWM_CONST) bswmPartPCCfgs;
    BswM_EventRquestPortRuntimeType ret = BSWM_INVALID_U8;
    ApplicationType partIdx;
    boolean result;
    BswM_EventRquestPortRuntimeType evIdx;

    result = BswM_GetPartitionIdx(&partIdx);
    if ((boolean)TRUE == result)
    {
        bswmPartPCCfgs = &(BswM_RuntimeStatus.bswmPartPCCfgs[partIdx]);
        evIdx = bswmPartPCCfgs->evRqstPCCfg->nmWkEvPortIdxPtr[nmIdx];
        ret = bswmPartPCCfgs->eventRqstPortRunPtr[evIdx];
    }
    return ret;
}
#endif /* BSWM_EVENT_RQSTPORT_ENABLE == STD_ON */
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/

#endif /* BSWM_NM_ENABLED == STD_ON */
