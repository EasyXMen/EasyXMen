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
/*
 ************************************************************************************************************************
 **
 **  @file               : BswM_Sd.c
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : BSW Mode Manager
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "BswM_Sd.h"
#include "BswM_Internal.h"
#if (BSWM_SD_ENABLED == STD_ON)
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
 * Initializes the SD request ports by setting the initial states for client services, consumed event groups, and event
 * handlers. This function retrieves the partition configuration and initializes the runtime status based on the
 * configuration. It handles different configurations for client services, consumed event groups, and event handlers if
 * they are enabled. If the partition configuration retrieval fails, it reports an error if development error detection
 * is enabled.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_InitSDRequestPorts(void)
/* PRQA S 1503,1505,1532 --*/
{
#if (                                                                                                                 \
    (BSWM_SDCLIENTSERVICECURRENTSTATE_ENABLED == STD_ON) || (BSWM_SDCONSUMEDEVENTGROUPCURRENTSTATE_ENABLED == STD_ON) \
    || (BSWM_SDEVENTHANDLERCURRENTSTATE_ENABLED == STD_ON))
    const BswM_PartitionPCCfgType*      bswmPartPCCfgPtr;
    const BswM_ModeRqstPortPCCfgType*   modeRqstPCfgPtr;
    const BswM_ModeRqstPortRuntimeType* modeRqstPtr;
    boolean                             result;
    uint16                              numOfSdCliSrvCurState;
    uint16                              numOfSdConsumedEvGrpCurrState;
    uint16                              numOfSdEvHdlerCurrState;
    uint16                              idx;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        modeRqstPCfgPtr = bswmPartPCCfgPtr->modeRqstPCCfg;
        modeRqstPtr     = bswmPartPCCfgPtr->modeRqstPortRunPtr;

#if (BSWM_SDCLIENTSERVICECURRENTSTATE_ENABLED == STD_ON)
        numOfSdCliSrvCurState = modeRqstPCfgPtr->numOfSdCliSrvCurState;
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
#endif /*BSWM_SDCLIENTSERVICECURRENTSTATE_ENABLED == STD_ON*/
#if (BSWM_SDCONSUMEDEVENTGROUPCURRENTSTATE_ENABLED == STD_ON)
        numOfSdConsumedEvGrpCurrState = modeRqstPCfgPtr->numOfSdConsumedEvGrpCurrState;
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
#endif /*BSWM_SDCONSUMEDEVENTGROUPCURRENTSTATE_ENABLED == STD_ON*/
#if (BSWM_SDEVENTHANDLERCURRENTSTATE_ENABLED == STD_ON)
        numOfSdEvHdlerCurrState = modeRqstPCfgPtr->numOfSdEvHdlerCurrState;
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
#endif /*BSWM_SDEVENTHANDLERCURRENTSTATE_ENABLED == STD_ON*/
    }

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    else
    {
        (void)Det_ReportError(BSWM_MODULE_ID, BSWM_INSTANCE_ID, BSWM_API_ID_INIT, BSWM_E_INIT_FAILED);
    }
#endif /*BSWM_DEV_ERROR_DETECT == STD_ON*/
#endif
}

/**
 * Updates the current state of a specified client service.
 * This function retrieves the partition configuration and updates the runtime status of the client service.
 * It also evaluates associated rules if the client service is configured to process immediately.
 * If development error detection is enabled, it checks the validity of the current client state.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_Sd_ClientServiceCurrentState(
    uint16                           SdClientServiceHandleId,
    Sd_ClientServiceCurrentStateType CurrentClientState)
/* PRQA S 1503,1505,1532 --*/
{
#if (BSWM_SDCLIENTSERVICECURRENTSTATE_ENABLED == STD_ON)
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
    uint16                              numOfSdCliSrvCurState;
    uint16                              cliIdx;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkClientCurState(CurrentClientState))
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
            modeRqstPCfgPtr       = bswmPartPCCfgPtr->modeRqstPCCfg;
            modeRqstLCfgPtr       = bswmPartLCfgPtr->modeRqstLCfg;
            numOfSdCliSrvCurState = modeRqstPCfgPtr->numOfSdCliSrvCurState;
            for (cliIdx = 0u; cliIdx < numOfSdCliSrvCurState; cliIdx++)
            {
                if (SdClientServiceHandleId == modeRqstLCfgPtr->sdClientSrvMethodsRef[cliIdx])
                {
                    modeRqstPtr                                    = bswmPartPCCfgPtr->modeRqstPortRunPtr;
                    modeRqstPtr->sdCliServiceCurrStaStatus[cliIdx] = CurrentClientState;
                    if (BSWM_IMMEDIATE == modeRqstLCfgPtr->sdClientSrv[cliIdx].process)
                    {
                        numOfRules = modeRqstLCfgPtr->sdClientSrv[cliIdx].belongToRlueNum;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
                            if ((modeRqstLCfgPtr->sdClientSrv[cliIdx].belongToRlue[idx].pbConfigTag
                                 == BswM_GetPBConfigTags())
                                || (BSWM_PB_CONFIG_IN_ALL_VARIANTS
                                    == modeRqstLCfgPtr->sdClientSrv[cliIdx].belongToRlue[idx].pbConfigTag))
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
                            {
                                ruleIdx = modeRqstLCfgPtr->sdClientSrv[cliIdx].belongToRlue[idx].ruleIndex;

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
    (void)SdClientServiceHandleId;
    (void)CurrentClientState;
#endif /*BSWM_SDCLIENTSERVICECURRENTSTATE_ENABLED == STD_ON*/
}

#if (BSWM_SDCLIENTSERVICECURRENTSTATE_ENABLED == STD_ON)
/**
 * Retrieves the current state of a specified client service.
 * This function fetches the partition configuration and returns the current state of the client service.
 * If the partition configuration is successfully retrieved, it reads the current state from the runtime data.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
Sd_ClientServiceCurrentStateType BswM_GetClientServiceCurState(uint16 cliIdx)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PartitionPCCfgType*   bswmPartPCCfgPtr;
    Sd_ClientServiceCurrentStateType cliCurState = SD_CLIENT_SERVICE_DOWN;
    boolean                          result;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        cliCurState = bswmPartPCCfgPtr->modeRqstPortRunPtr->sdCliServiceCurrStaStatus[cliIdx];
    }
    return cliCurState;
}
#endif /*BSWM_SDCLIENTSERVICECURRENTSTATE_ENABLED == STD_ON*/

/**
 * Updates the current state of a consumed event group.
 * This function checks the validity of the consumed event group state and updates the internal state.
 * It also processes any immediate rules associated with the event group if the state update is successful.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_Sd_ConsumedEventGroupCurrentState(
    uint16                                SdConsumedEventGroupHandleId,
    Sd_ConsumedEventGroupCurrentStateType ConsumedEventGroupState)
/* PRQA S 1503,1505,1532 --*/
{
#if (BSWM_SDCONSUMEDEVENTGROUPCURRENTSTATE_ENABLED == STD_ON)
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
    uint16                              numOfSdConsumedEvGrpCurrState;
    uint16                              evIdx;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkEvGruCurState(ConsumedEventGroupState))
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
            modeRqstPCfgPtr               = bswmPartPCCfgPtr->modeRqstPCCfg;
            modeRqstLCfgPtr               = bswmPartLCfgPtr->modeRqstLCfg;
            numOfSdConsumedEvGrpCurrState = modeRqstPCfgPtr->numOfSdConsumedEvGrpCurrState;
            for (evIdx = 0u; evIdx < numOfSdConsumedEvGrpCurrState; evIdx++)
            {
                if (SdConsumedEventGroupHandleId == modeRqstLCfgPtr->sdConEvGroupIdRef[evIdx])
                {
                    modeRqstPtr                                      = bswmPartPCCfgPtr->modeRqstPortRunPtr;
                    modeRqstPtr->sdConsumedEvGrpCurrStaStatus[evIdx] = ConsumedEventGroupState;
                    if (BSWM_IMMEDIATE == modeRqstLCfgPtr->sdConEvGroup[evIdx].process)
                    {
                        numOfRules = modeRqstLCfgPtr->sdConEvGroup[evIdx].belongToRlueNum;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
                            if ((modeRqstLCfgPtr->sdConEvGroup[evIdx].belongToRlue[idx].pbConfigTag
                                 == BswM_GetPBConfigTags())
                                || (BSWM_PB_CONFIG_IN_ALL_VARIANTS
                                    == modeRqstLCfgPtr->sdConEvGroup[evIdx].belongToRlue[idx].pbConfigTag))
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
                            {
                                ruleIdx = modeRqstLCfgPtr->sdConEvGroup[evIdx].belongToRlue[idx].ruleIndex;
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
    (void)SdConsumedEventGroupHandleId;
    (void)ConsumedEventGroupState;
#endif /*BSWM_SDCONSUMEDEVENTGROUPCURRENTSTATE_ENABLED == STD_ON*/
}

#if (BSWM_SDCONSUMEDEVENTGROUPCURRENTSTATE_ENABLED == STD_ON)
/**
 * Retrieves the current state of a specified consumed event group.
 * This function fetches the partition configuration and returns the current state of the event group.
 * If the partition configuration is successfully retrieved, it reads the current state from the runtime data.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
Sd_ConsumedEventGroupCurrentStateType BswM_GetEvGruCurState(uint16 evIdx)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PartitionPCCfgType*        bswmPartPCCfgPtr;
    Sd_ConsumedEventGroupCurrentStateType evGruCurState = SD_CONSUMED_EVENTGROUP_DOWN;
    boolean                               result;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        evGruCurState = bswmPartPCCfgPtr->modeRqstPortRunPtr->sdConsumedEvGrpCurrStaStatus[evIdx];
    }
    return evGruCurState;
}
#endif /*BSWM_SDCONSUMEDEVENTGROUPCURRENTSTATE_ENABLED == STD_ON*/

/**
 * Handles the current state of an event handler.
 * This function updates the current state of the specified event handler and processes any immediate rules associated
 * with it. It retrieves the necessary configuration and runtime data, checks for the event handler handle ID, and
 * updates the status accordingly. If the event handler is configured to process immediately, it evaluates the
 * associated rules.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_Sd_EventHandlerCurrentState(uint16 SdEventHandlerHandleId, Sd_EventHandlerCurrentStateType EventHandlerStatus)
/* PRQA S 1503,1505,1532 --*/
{
#if (BSWM_SDEVENTHANDLERCURRENTSTATE_ENABLED == STD_ON)
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
    uint16                              numOfSdEvHdlerCurrState;
    uint16                              handlerIdx;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if ((Std_ReturnType)E_OK == BswM_DetChkEvHandlerCurState(EventHandlerStatus))
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
            modeRqstPCfgPtr         = bswmPartPCCfgPtr->modeRqstPCCfg;
            modeRqstLCfgPtr         = bswmPartLCfgPtr->modeRqstLCfg;
            numOfSdEvHdlerCurrState = modeRqstPCfgPtr->numOfSdEvHdlerCurrState;
            for (handlerIdx = 0u; handlerIdx < numOfSdEvHdlerCurrState; handlerIdx++)
            {
                if (SdEventHandlerHandleId == modeRqstLCfgPtr->sdEvHandleIdRef[handlerIdx])
                {
                    modeRqstPtr                                     = bswmPartPCCfgPtr->modeRqstPortRunPtr;
                    modeRqstPtr->sdEvHdlerCurrStaStatus[handlerIdx] = EventHandlerStatus;
                    if (BSWM_IMMEDIATE == modeRqstLCfgPtr->sdEvHandler[handlerIdx].process)
                    {
                        numOfRules = modeRqstLCfgPtr->sdEvHandler[handlerIdx].belongToRlueNum;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
                            if ((modeRqstLCfgPtr->sdEvHandler[handlerIdx].belongToRlue[idx].pbConfigTag
                                 == BswM_GetPBConfigTags())
                                || (BSWM_PB_CONFIG_IN_ALL_VARIANTS
                                    == modeRqstLCfgPtr->sdEvHandler[handlerIdx].belongToRlue[idx].pbConfigTag))
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
                            {
                                ruleIdx = modeRqstLCfgPtr->sdEvHandler[handlerIdx].belongToRlue[idx].ruleIndex;
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
    (void)SdEventHandlerHandleId;
    (void)EventHandlerStatus;
#endif /*BSWM_SDEVENTHANDLERCURRENTSTATE_ENABLED == STD_ON*/
}

#if (BSWM_SDEVENTHANDLERCURRENTSTATE_ENABLED == STD_ON)
/**
 * Retrieves the current state of a specified event handler.
 * This function fetches the partition configuration and returns the current state of the event handler.
 * If the partition configuration is successfully retrieved, it reads the current state from the runtime data.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
Sd_EventHandlerCurrentStateType BswM_GetEvHandlerCurState(uint16 handlerIdx)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PartitionPCCfgType*  bswmPartPCCfgPtr;
    Sd_EventHandlerCurrentStateType evHandlerCurState = SD_EVENT_HANDLER_RELEASED;
    boolean                         result;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        evHandlerCurState = bswmPartPCCfgPtr->modeRqstPortRunPtr->sdEvHdlerCurrStaStatus[handlerIdx];
    }
    return evHandlerCurState;
}
#endif /*BSWM_SDEVENTHANDLERCURRENTSTATE_ENABLED == STD_ON*/

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
/* ========================================== internal function definitions ========================================= */

#endif /*BSWM_SD_ENABLED == STD_ON*/
