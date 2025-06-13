/* PRQA S 3108++ */
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
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : BswM_Sd.c                                                   **
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
#include "BswM_Sd.h"
#if (BSWM_SD_ENABLED == STD_ON)
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
/*Initialize sd related request port*/
FUNC(void, BSWM_SD_CODE)
BswM_InitSDRequestPorts(void)
{
    P2CONST(BswM_ModeRqstPortPCCfgType, AUTOMATIC, BSWM_CONST) modeRqstPCfgPtr;
    P2CONST(BswM_ModeRqstPortRuntimeType, AUTOMATIC, BSWM_VAR_CLEARED) modeRqstPtr;
    ApplicationType partIdx;
    boolean result;
    uint16 numOfSdCliSrvCurState;
    uint16 numOfSdConsumedEvGrpCurrState;
    uint16 numOfSdEvHdlerCurrState;
    uint16 idx;

    result = BswM_GetPartitionIdx(&partIdx);
    if ((boolean)TRUE == result)
    {
        modeRqstPCfgPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPCCfg;
        modeRqstPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPortRunPtr;
        numOfSdCliSrvCurState = modeRqstPCfgPtr->numOfSdCliSrvCurState;
        numOfSdConsumedEvGrpCurrState = modeRqstPCfgPtr->numOfSdConsumedEvGrpCurrState;
        numOfSdEvHdlerCurrState = modeRqstPCfgPtr->numOfSdEvHdlerCurrState;

        for (idx = 0u; idx < numOfSdCliSrvCurState; idx++)
        {
            if (NULL_PTR != modeRqstPCfgPtr->curClientInitState)
            {
                modeRqstPtr->sdCliServiceCurrStaStatus[idx] = *(modeRqstPCfgPtr->curClientInitState);
            }
            else
            {
                break;
            }
        }
        for (idx = 0u; idx < numOfSdConsumedEvGrpCurrState; idx++)
        {
            if (NULL_PTR != modeRqstPCfgPtr->consEvGroupInitState)
            {
                modeRqstPtr->sdConsumedEvGrpCurrStaStatus[idx] = *(modeRqstPCfgPtr->consEvGroupInitState);
            }
            else
            {
                break;
            }
        }
        for (idx = 0u; idx < numOfSdEvHdlerCurrState; idx++)
        {
            if (NULL_PTR != modeRqstPCfgPtr->evHandlerInitStatus)
            {
                modeRqstPtr->sdEvHdlerCurrStaStatus[idx] = *(modeRqstPCfgPtr->evHandlerInitStatus);
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
 * Function called by Service Discovery to indicate current state of the Client
 * Service (available/down).
 * Service ID: 0x1f
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): SdClientServiceHandleId, HandleId to identify the ClientService
 *                 CurrentClientState, Current state of the ClientService
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BSWM_SD_CODE)
BswM_Sd_ClientServiceCurrentState(uint16 SdClientServiceHandleId, Sd_ClientServiceCurrentStateType CurrentClientState)
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
    uint16 numOfSdCliSrvCurState;
    uint16 cliIdx;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkClientCurState(CurrentClientState))
#endif /*BSWM_DEV_ERROR_DETECT == STD_ON*/
    {
        result = BswM_GetPartitionIdx(&partIdx);
        if ((boolean)TRUE == result)
        {
            modeRqstPCfgPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPCCfg;
            modeRqstLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].modeRqstLCfg;
            numOfSdCliSrvCurState = modeRqstPCfgPtr->numOfSdCliSrvCurState;
            for (cliIdx = 0u; cliIdx < numOfSdCliSrvCurState; cliIdx++)
            {
                if (SdClientServiceHandleId == modeRqstLCfgPtr->sdClientSrvMethodsRef[cliIdx])
                {
                    modeRqstPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPortRunPtr;
                    modeRqstPtr->sdCliServiceCurrStaStatus[cliIdx] = CurrentClientState;
                    if (BSWM_IMMEDIATE == modeRqstLCfgPtr->sdClientSrv[cliIdx].process)
                    {
                        numOfRules = modeRqstLCfgPtr->sdClientSrv[cliIdx].belongToRlueNum;
                        ruleLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].ruleLCfg;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
                            ruleIdx = modeRqstLCfgPtr->sdClientSrv[cliIdx].belongToRlue[idx];
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
}

/**
 * Function called by Service Discovery to indicate current status of the
 * Consumed Eventgroup (available/down).
 * Service ID: 0x21
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): SdConsumedEventGroupHandleId, HandleId to identify the Consumed Eventgroup
 *                 ConsumedEventGroupState, Status of the Consumed Eventgroup
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BSWM_SD_CODE)
BswM_Sd_ConsumedEventGroupCurrentState(
    uint16 SdConsumedEventGroupHandleId,
    Sd_ConsumedEventGroupCurrentStateType ConsumedEventGroupState)
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
    uint16 numOfSdConsumedEvGrpCurrState;
    uint16 evIdx;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkEvGruCurState(ConsumedEventGroupState))
#endif /*BSWM_DEV_ERROR_DETECT == STD_ON*/
    {
        result = BswM_GetPartitionIdx(&partIdx);
        if ((boolean)TRUE == result)
        {
            modeRqstPCfgPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPCCfg;
            modeRqstLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].modeRqstLCfg;
            numOfSdConsumedEvGrpCurrState = modeRqstPCfgPtr->numOfSdConsumedEvGrpCurrState;
            for (evIdx = 0u; evIdx < numOfSdConsumedEvGrpCurrState; evIdx++)
            {
                if (SdConsumedEventGroupHandleId == modeRqstLCfgPtr->sdConEvGroupIdRef[evIdx])
                {
                    modeRqstPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPortRunPtr;
                    modeRqstPtr->sdConsumedEvGrpCurrStaStatus[evIdx] = ConsumedEventGroupState;
                    if (BSWM_IMMEDIATE == modeRqstLCfgPtr->sdConEvGroup[evIdx].process)
                    {
                        numOfRules = modeRqstLCfgPtr->sdConEvGroup[evIdx].belongToRlueNum;
                        ruleLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].ruleLCfg;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
                            ruleIdx = modeRqstLCfgPtr->sdConEvGroup[evIdx].belongToRlue[idx];
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
}

/**
 * Function called by Service Discovery to indicate current status of the
 * EventHandler (requested/released).
 * Service ID: 0x20
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): SdEventHandlerHandleId, HandleId to identify the EventHandler
 *                 EventHandlerStatus, Status of the EventHandler
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BSWM_SD_CODE)
BswM_Sd_EventHandlerCurrentState(uint16 SdEventHandlerHandleId, Sd_EventHandlerCurrentStateType EventHandlerStatus)
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
    uint16 numOfSdEvHdlerCurrState;
    uint16 handlerIdx;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkEvHandlerCurState(EventHandlerStatus))
#endif /*BSWM_DEV_ERROR_DETECT == STD_ON*/
    {
        result = BswM_GetPartitionIdx(&partIdx);
        if ((boolean)TRUE == result)
        {
            modeRqstPCfgPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPCCfg;
            modeRqstLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].modeRqstLCfg;
            numOfSdEvHdlerCurrState = modeRqstPCfgPtr->numOfSdEvHdlerCurrState;
            for (handlerIdx = 0u; handlerIdx < numOfSdEvHdlerCurrState; handlerIdx++)
            {
                if (SdEventHandlerHandleId == modeRqstLCfgPtr->sdEvHandleIdRef[handlerIdx])
                {
                    modeRqstPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPortRunPtr;
                    modeRqstPtr->sdEvHdlerCurrStaStatus[handlerIdx] = EventHandlerStatus;
                    if (BSWM_IMMEDIATE == modeRqstLCfgPtr->sdEvHandler[handlerIdx].process)
                    {
                        numOfRules = modeRqstLCfgPtr->sdEvHandler[handlerIdx].belongToRlueNum;
                        ruleLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].ruleLCfg;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
                            ruleIdx = modeRqstLCfgPtr->sdEvHandler[handlerIdx].belongToRlue[idx];
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
}

/*Get Client Service Current State*/
FUNC(Sd_ClientServiceCurrentStateType, BSWM_NVM_CODE)
BswM_GetClientServiceCurState(uint16 cliIdx)
{
    Sd_ClientServiceCurrentStateType cliCurState = SD_CLIENT_SERVICE_DOWN;
    ApplicationType partIdx;
    boolean result;

    result = BswM_GetPartitionIdx(&partIdx);
    if ((boolean)TRUE == result)
    {
        cliCurState = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPortRunPtr->sdCliServiceCurrStaStatus[cliIdx];
    }
    return cliCurState;
}

/*Get Consumed Event Group Current State*/
FUNC(Sd_ConsumedEventGroupCurrentStateType, BSWM_NVM_CODE)
BswM_GetEvGruCurState(uint16 evIdx)
{
    Sd_ConsumedEventGroupCurrentStateType evGruCurState = SD_CONSUMED_EVENTGROUP_DOWN;
    ApplicationType partIdx;
    boolean result;

    result = BswM_GetPartitionIdx(&partIdx);
    if ((boolean)TRUE == result)
    {
        evGruCurState =
            BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPortRunPtr->sdConsumedEvGrpCurrStaStatus[evIdx];
    }
    return evGruCurState;
}

/*Get Event Handler Current State*/
FUNC(Sd_EventHandlerCurrentStateType, BSWM_NVM_CODE)
BswM_GetEvHandlerCurState(uint16 handlerIdx)
{
    Sd_EventHandlerCurrentStateType evHandlerCurState = SD_EVENT_HANDLER_RELEASED;
    ApplicationType partIdx;
    boolean result;

    result = BswM_GetPartitionIdx(&partIdx);
    if ((boolean)TRUE == result)
    {
        evHandlerCurState =
            BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPortRunPtr->sdEvHdlerCurrStaStatus[handlerIdx];
    }
    return evHandlerCurState;
}
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/

#endif /*BSWM_SD_ENABLED == STD_ON*/
