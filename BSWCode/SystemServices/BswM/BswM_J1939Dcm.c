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
**  FILENAME    : BswM_J1939Dcm.c                                             **
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
#include "BswM_J1939Dcm.h"
#if (BSWM_J1939DCM_ENABLED == STD_ON)
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
/*Initialize BswMJ1939DcmBroadcastStatus request port*/
FUNC(void, BSWM_J1939DCM_CODE)
BswM_InitJ1939DcmRequestPorts(void)
{
    P2CONST(BswM_ModeRqstPortPCCfgType, AUTOMATIC, BSWM_CONST) modeRqstPCfgPtr;
    P2CONST(BswM_ModeRqstPortRuntimeType, AUTOMATIC, BSWM_VAR_CLEARED) modeRqstPtr;
    ApplicationType partIdx;
    boolean result;
    NetworkHandleType numOfj1939DcmCh;
    NetworkHandleType chIdx;

    result = BswM_GetPartitionIdx(&partIdx);
    if ((boolean)TRUE == result)
    {
        modeRqstPCfgPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPCCfg;
        modeRqstPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPortRunPtr;
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
 * This API tells the BswM the desired communication status of the available
 * networks. The status will typically be activated via COM I-PDU group switches.
 * Service ID: 0x1b
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): NetworkMask, Mask containing one bit for each available
 *                              network. 1: Network enabled 0: Network disabled
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BSWM_J1939DCM_CODE)
BswM_J1939DcmBroadcastStatus(uint16 NetworkMask)
{
    P2CONST(BswM_ModeRqstPortPCCfgType, AUTOMATIC, BSWM_CONST) modeRqstPCfgPtr;
    P2CONST(BswM_ModeRqstPortLCfgType, AUTOMATIC, BSWM_CONST) modeRqstLCfgPtr;
    P2CONST(BswM_ModeRqstPortRuntimeType, AUTOMATIC, BSWM_VAR_CLEARED) modeRqstPtr;
    P2CONST(BswM_RuleLcCfgType, AUTOMATIC, BSWM_CONST) ruleLCfgPtr;
    BswM_RuleIndexType numOfRules;
    BswM_RuleIndexType idx;
    BswM_RuleIndexType ruleIdx;
    ApplicationType partIdx;
    boolean result;
    NetworkHandleType numOfj1939DcmCh;
    NetworkHandleType chIdx;
    NetworkHandleType j1939BelongNw;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkJ1939DcmBroad())
#endif /*BSWM_DEV_ERROR_DETECT == STD_ON*/
    {
        result = BswM_GetPartitionIdx(&partIdx);
        if ((boolean)TRUE == result)
        {
            modeRqstPCfgPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPCCfg;
            modeRqstLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].modeRqstLCfg;
            numOfj1939DcmCh = modeRqstPCfgPtr->numOfj1939DcmCh;
            for (chIdx = 0u; chIdx < numOfj1939DcmCh; chIdx++)
            {
                modeRqstPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPortRunPtr;
                j1939BelongNw = modeRqstLCfgPtr->j1939DcmBCChRef[chIdx];
                if (0u != (NetworkMask & ((uint16)1u << j1939BelongNw)))
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
                    ruleLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].ruleLCfg;
                    for (idx = 0u; idx < numOfRules; idx++)
                    {
                        ruleIdx = modeRqstLCfgPtr->j1939DcmBC[chIdx].belongToRlue[idx];
                        if ((boolean)FALSE == ruleLCfgPtr[ruleIdx].isNestExe)
                        {
                            /* An Independent rule,to be evaluated each time applicable*/
                            BswM_ArbitrateRule(ruleIdx, partIdx);
                        }
                    }
                }
            }
        }
    }
}

/*Get J1939 channel broad status*/
FUNC(boolean, BSWM_J1939DCM_CODE)
BswM_GetJ1939ChBroadStatus(NetworkHandleType j1939ChIdx)
{
    boolean j1939Status = FALSE;
    ApplicationType partIdx;
    boolean result;

    result = BswM_GetPartitionIdx(&partIdx);
    if ((boolean)TRUE == result)
    {
        j1939Status = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPortRunPtr->j1939DcmBrdcstStatus[j1939ChIdx];
    }
    return j1939Status;
}
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/

#endif /*BSWM_J1939DCM_ENABLED == STD_ON*/
