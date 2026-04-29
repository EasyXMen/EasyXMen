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
 **  @file               : BswM_Swc.c
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : BSW Mode Manager
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "BswM_Swc.h"
#include "BswM_Internal.h"
/* PRQA S 0553 EOF */ /* VL_BswM_0553 */
#if (                                                                                       \
    (BSWM_SWCMODEREQUEST_ENABLED == STD_ON) || (BSWM_SWCMODENOTIFICATION_ENABLED == STD_ON) \
    || (BSWM_EVENT_MODE_SWITCH_ERROR_EVENT_ENABLE == STD_ON) || (BSWM_EVENT_MODE_SWITCH_ERROR_EVENT_ENABLE == STD_ON))
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"

/**
 * Notifies the system of a mode switch error for a specified port.
 * This function updates the event request port runtime data to indicate a mode switch error and processes any
 * associated rules if required. It retrieves the necessary configuration and runtime data, checks for the specified
 * port ID, and sets the event status. If the process is immediate, it also evaluates the associated rules.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_SwcModeSwitchErrorNotification(uint16 portId)
/* PRQA S 1503,1505,1532 --*/
{
#if (BSWM_EVENT_MODE_SWITCH_ERROR_EVENT_ENABLE == STD_ON)
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
    const BswM_PartitionPbConfigType* bswmPartPbConfigPtr;
#endif /* BSWM_PB_CONFIG_ENABLED */
    const BswM_PartitionLCfgType*     bswmPartLCfgPtr;
    const BswM_EventRqstPortLCfgType* evRqstLCfgPtr;
    BswM_EventRquestPortRuntimeType*  evRqstPtr;
    BswM_RuleIndexType                numOfRules;
    BswM_RuleIndexType                idx;
    BswM_RuleIndexType                ruleIdx;
    BswM_EventRqstPortIdxType         evIdx;
    boolean                           result;
    uint16                            numOfModeSwiErrEv;
    uint16                            swcIdx;

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
            numOfModeSwiErrEv = bswmPartPCCfgPtr->evRqstPCCfg->numOfModeSwiErrEv;
            evRqstLCfgPtr     = bswmPartLCfgPtr->evRqstLCfg;
            for (swcIdx = 0u; swcIdx < numOfModeSwiErrEv; swcIdx++)
            {
                if (portId == evRqstLCfgPtr->modeSwiErrPort[swcIdx])
                {
                    evIdx            = bswmPartPCCfgPtr->evRqstPCCfg->modeSwiErrEvPtr[swcIdx];
                    evRqstPtr        = bswmPartPCCfgPtr->eventRqstPortRunPtr;
                    evRqstPtr[evIdx] = BSWM_EVENT_IS_SET;
                    if (BSWM_IMMEDIATE == evRqstLCfgPtr->modeSwiErr[swcIdx].process)
                    {
                        numOfRules = evRqstLCfgPtr->modeSwiErr[swcIdx].belongToRlueNum;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
                            if ((evRqstLCfgPtr->modeSwiErr[swcIdx].belongToRlue[idx].pbConfigTag
                                 == BswM_GetPBConfigTags())
                                || (BSWM_PB_CONFIG_IN_ALL_VARIANTS
                                    == evRqstLCfgPtr->modeSwiErr[swcIdx].belongToRlue[idx].pbConfigTag))
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
                            {
                                ruleIdx = evRqstLCfgPtr->modeSwiErr[swcIdx].belongToRlue[idx].ruleIndex;

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
    (void)(portId);
#endif /*BSWM_EVENT_MODE_SWITCH_ERROR_EVENT_ENABLE == STD_ON*/
}

/**
 * Notifies the system of a mode switch acknowledgment for a specified port.
 * This function updates the event request port runtime data to indicate a mode switch acknowledgment and processes any
 * associated rules if required. It retrieves the necessary configuration and runtime data, checks for the specified
 * port ID, and sets the event status. If the process is immediate, it also evaluates the associated rules.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_SwcModeSwitchAckNotification(uint16 portId)
/* PRQA S 1503,1505,1532 --*/
{
#if (BSWM_EVENT_MODE_SWITCH_ERROR_EVENT_ENABLE == STD_ON)
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
    const BswM_PartitionPbConfigType* bswmPartPbConfigPtr;
#endif /* BSWM_PB_CONFIG_ENABLED */
    const BswM_PartitionLCfgType*     bswmPartLCfgPtr;
    const BswM_EventRqstPortLCfgType* evRqstLCfgPtr;
    BswM_EventRquestPortRuntimeType*  evRqstPtr;
    BswM_RuleIndexType                numOfRules;
    BswM_RuleIndexType                idx;
    BswM_RuleIndexType                ruleIdx;
    BswM_EventRqstPortIdxType         evIdx;
    boolean                           result;
    uint16                            numOfModeSwiAckEv;
    uint16                            swcIdx;

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
            numOfModeSwiAckEv = bswmPartPCCfgPtr->evRqstPCCfg->numOfModeSwiAckEv;
            evRqstLCfgPtr     = bswmPartLCfgPtr->evRqstLCfg;
            for (swcIdx = 0u; swcIdx < numOfModeSwiAckEv; swcIdx++)
            {
                if (portId == evRqstLCfgPtr->modeSwiAckPort[swcIdx])
                {
                    evIdx            = bswmPartPCCfgPtr->evRqstPCCfg->modeSwiAckNotiPtr[swcIdx];
                    evRqstPtr        = bswmPartPCCfgPtr->eventRqstPortRunPtr;
                    evRqstPtr[evIdx] = BSWM_EVENT_IS_SET;
                    if (BSWM_IMMEDIATE == evRqstLCfgPtr->modeSwiAck[swcIdx].process)
                    {
                        numOfRules = evRqstLCfgPtr->modeSwiAck[swcIdx].belongToRlueNum;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
                            if ((evRqstLCfgPtr->modeSwiAck[swcIdx].belongToRlue[idx].pbConfigTag
                                 == BswM_GetPBConfigTags())
                                || (BSWM_PB_CONFIG_IN_ALL_VARIANTS
                                    == evRqstLCfgPtr->modeSwiAck[swcIdx].belongToRlue[idx].pbConfigTag))
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
                            {
                                ruleIdx = evRqstLCfgPtr->modeSwiAck[swcIdx].belongToRlue[idx].ruleIndex;
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
    (void)(portId);
#endif /*BSWM_EVENT_MODE_SWITCH_ERROR_EVENT_ENABLE == STD_ON*/
}

#if (BSWM_SWCMODENOTIFICATION_ENABLED == STD_ON)
/**
 * Notifies the system of a mode change for a specified mode group.
 * This function updates the mode request port runtime data to indicate a mode change and processes any associated rules
 * if required. It retrieves the necessary configuration and runtime data, checks for the specified mode group, and sets
 * the mode status. If the process is immediate, it also evaluates the associated rules.
 */
/* PRQA S 1503,1505,1532,6030 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef, VL_MTR_BswM_STMIF */
void BswM_SwcModeNotification(BswM_ModeGroupType modeGroup)
/* PRQA S 1503,1505,1532,6030 --*/
{
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
    const BswM_PartitionPbConfigType* bswmPartPbConfigPtr;
#endif /* BSWM_PB_CONFIG_ENABLED */
    const BswM_PartitionLCfgType*     bswmPartLCfgPtr;
    const BswM_ModeRqstPortPCCfgType* modeRqstPCfgPtr;
    const BswM_ModeRqstPortLCfgType*  modeRqstLCfgPtr;
    BswM_RuleIndexType                numOfRules;
    BswM_RuleIndexType                idx;
    BswM_RuleIndexType                ruleIdx;
    boolean                           result;
    uint8                             numOfSwcModeNoti;
    uint8                             swcIdx;

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
            numOfSwcModeNoti = modeRqstPCfgPtr->numOfSwcModeNoti;
            for (swcIdx = 0u; swcIdx < numOfSwcModeNoti; swcIdx++)
            {
                if (modeGroup == modeRqstLCfgPtr->swcModeNotisRef[swcIdx])
                {
                    if (BSWM_IMMEDIATE == modeRqstLCfgPtr->swcModeNoti[swcIdx].process)
                    {
                        numOfRules = modeRqstLCfgPtr->swcModeNoti[swcIdx].belongToRlueNum;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
                            if ((modeRqstLCfgPtr->swcModeNoti[swcIdx].belongToRlue[idx].pbConfigTag
                                 == BswM_GetPBConfigTags())
                                || (BSWM_PB_CONFIG_IN_ALL_VARIANTS
                                    == modeRqstLCfgPtr->swcModeNoti[swcIdx].belongToRlue[idx].pbConfigTag))
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
                            {
                                ruleIdx = modeRqstLCfgPtr->swcModeNoti[swcIdx].belongToRlue[idx].ruleIndex;
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
}
#endif /*BSWM_SWCMODENOTIFICATION_ENABLED == STD_ON*/

#if (BSWM_SWCMODEREQUEST_ENABLED == STD_ON)
/**
 * Processes a mode request for a specified mode group.
 * This function retrieves the necessary configuration and runtime data, checks for the specified mode group, and
 * processes any associated rules. It iterates through the mode requests, evaluates the rules if applicable, and updates
 * the mode request port runtime data accordingly.
 */
/* PRQA S 1503,1505,1532,1501 ++ */ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef, VL_BswM_1501 */
void BswM_SwcModeRequest(BswM_ModeGroupType modeGroup)
/* PRQA S 1503,1505,1532,1501 --*/
{
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
    const BswM_PartitionPbConfigType* bswmPartPbConfigPtr;
#endif /* BSWM_PB_CONFIG_ENABLED */
    const BswM_PartitionLCfgType*     bswmPartLCfgPtr;
    const BswM_ModeRqstPortPCCfgType* modeRqstPCfgPtr;
    const BswM_ModeRqstPortLCfgType*  modeRqstLCfgPtr;
    BswM_RuleIndexType                numOfRules;
    BswM_RuleIndexType                idx;
    BswM_RuleIndexType                ruleIdx;
    boolean                           result;
    uint8                             numOfSwcModeReq;
    uint8                             swcIdx;

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
            modeRqstPCfgPtr = bswmPartPCCfgPtr->modeRqstPCCfg;
            modeRqstLCfgPtr = bswmPartLCfgPtr->modeRqstLCfg;
            numOfSwcModeReq = modeRqstPCfgPtr->numOfSwcModeReq;
            for (swcIdx = 0u; swcIdx < numOfSwcModeReq; swcIdx++)
            {
                if (modeGroup == modeRqstLCfgPtr->swcModeRqstsRef[swcIdx])
                {
                    numOfRules = modeRqstLCfgPtr->swcModeRqst[swcIdx].belongToRlueNum;
                    for (idx = 0u; idx < numOfRules; idx++)
                    {
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
                        if ((modeRqstLCfgPtr->swcModeRqst[swcIdx].belongToRlue[idx].pbConfigTag
                             == (BswM_GetPBConfigTags()))
                            || (BSWM_PB_CONFIG_IN_ALL_VARIANTS
                                == modeRqstLCfgPtr->swcModeRqst[swcIdx].belongToRlue[idx].pbConfigTag))
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
                        {
                            ruleIdx = modeRqstLCfgPtr->swcModeRqst[swcIdx].belongToRlue[idx].ruleIndex;
                            /* An Independent rule,to be evaluated each time applicable*/
                            BswM_ArbitrateRule(bswmPartLCfgPtr, ruleIdx, FALSE);
                        }
                    }
                }
            }
        }
    }
}

#endif /*BSWM_EVENT_MODE_SWITCH_ERROR_EVENT_ENABLE == STD_ON*/

#if (BSWM_EVENT_MODE_SWITCH_ERROR_EVENT_ENABLE == STD_ON)
/**
 * Retrieves the event request port runtime type for a given port ID related to mode switch error notification.
 * This function checks the configuration and runtime data to find the corresponding event index and returns the event
 * request port runtime type.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
BswM_EventRquestPortRuntimeType BswM_GetModeSwiErrNoti(uint16 portId)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PartitionPCCfgType*  bswmPartPCCfgPtr;
    BswM_EventRquestPortRuntimeType ret = BSWM_INVALID_U8;
    boolean                         result;
    BswM_EventRquestPortRuntimeType evIdx;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        evIdx = bswmPartPCCfgPtr->evRqstPCCfg->modeSwiErrEvPtr[portId];
        ret   = bswmPartPCCfgPtr->eventRqstPortRunPtr[evIdx];
    }
    return ret;
}
#endif /*BSWM_EVENT_MODE_SWITCH_ERROR_EVENT_ENABLE == STD_ON*/

#if (BSWM_EVENT_SWITCH_ACK_NOTIFICATION_ENABLE == STD_ON)
/**
 * Retrieves the event request port runtime type for a given port ID related to mode switch acknowledgment notification.
 * This function checks the configuration and runtime data to find the corresponding event index and returns the event
 * request port runtime type.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
BswM_EventRquestPortRuntimeType BswM_GetModeSwiAckNoti(uint16 portId)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PartitionPCCfgType*  bswmPartPCCfgPtr;
    BswM_EventRquestPortRuntimeType ret = BSWM_INVALID_U8;
    boolean                         result;
    BswM_EventRquestPortRuntimeType evIdx;

    result = BswM_GetPartitionPCCfgPtr(&bswmPartPCCfgPtr);
    if (result)
    {
        evIdx = bswmPartPCCfgPtr->evRqstPCCfg->modeSwiAckNotiPtr[portId];
        ret   = bswmPartPCCfgPtr->eventRqstPortRunPtr[evIdx];
    }
    return ret;
}
#endif /*BSWM_EVENT_SWITCH_ACK_NOTIFICATION_ENABLE == STD_ON*/

/* ========================================== internal function definitions ========================================= */
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_RTE_ENABLED == STD_ON*/
