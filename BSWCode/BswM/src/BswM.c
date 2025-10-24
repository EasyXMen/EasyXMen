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
 **  @file               : BswM.c
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Apis provided by the BswM module (which does not interact with other modules), as well as
 *internal functions
 **
 ***********************************************************************************************************************/

#define BSWM_C_AR_MAJOR_VERSION 4u
#define BSWM_C_AR_MINOR_VERSION 9u
#define BSWM_C_AR_PATCH_VERSION 0u
#define BSWM_C_SW_MAJOR_VERSION 2u
#define BSWM_C_SW_MINOR_VERSION 2u
#define BSWM_C_SW_PATCH_VERSION 3u

/* =================================================== inclusions =================================================== */
#include "BswM_Internal.h"

/* ===================================================== macros ===================================================== */

#if (BSWM_C_AR_MAJOR_VERSION != BSWM_AR_MAJOR_VERSION)
#error "BswM.c : Mismatch in Specification Major Version"
#endif
#if (BSWM_C_AR_MINOR_VERSION != BSWM_AR_MINOR_VERSION)
#error "BswM.c : Mismatch in Specification Major Version"
#endif
#if (BSWM_C_AR_PATCH_VERSION != BSWM_AR_PATCH_VERSION)
#error "BswM.c : Mismatch in Specification Major Version"
#endif
#if (BSWM_C_SW_MAJOR_VERSION != BSWM_SW_MAJOR_VERSION)
#error "BswM.c : Mismatch in Specification Major Version"
#endif
#if (BSWM_C_SW_MINOR_VERSION != BSWM_SW_MINOR_VERSION)
#error "BswM.c : Mismatch in Specification Major Version"
#endif
#if (BSWM_C_SW_PATCH_VERSION != BSWM_SW_PATCH_VERSION)
#error "BswM.c : Mismatch in Specification Major Version"
#endif

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
#if (BSWM_EVENT_RQSTPORT_ENABLE == STD_ON)
/*Initialize event request port*/
BSWM_LOCAL void BswM_InitEventRequestPort(void);
#endif /* BSWM_EVENT_RQSTPORT_ENABLE == STD_ON */

/*Initialize event request port*/
BSWM_LOCAL void BswM_InitModeRequestPort(void);

/*Initialize rules*/
BSWM_LOCAL void BswM_InitRules(void);

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
#define BSWM_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
#include "BswM_MemMap.h"
/*Pointer variable to store the determined PB configuration data*/
const BswM_ConfigType* BswM_ConfigPtr;
#define BSWM_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
#include "BswM_MemMap.h"
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
/* ========================================== external function definitions ========================================= */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
/**
 * Initializes the BswM module by setting up various internal components and configurations.
 * This function initializes rules, event request ports, and mode request ports. It also sets the initialization state
 * of the BswM module. If the configuration retrieval fails, an error is reported using the Det_ReportError function.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_Init(const BswM_ConfigType* ConfigPtr)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PCCfgType* runPCCfgPtr;

    if (BswM_GetCorePCCfgPtr(&runPCCfgPtr))
    {
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
        if (NULL_PTR == BswM_ConfigPtr)
        {
            BswM_ConfigPtr = ConfigPtr;
        }
#else
        (void)ConfigPtr;
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
        BswM_InitRules();
#if (BSWM_EVENT_RQSTPORT_ENABLE == STD_ON)
        BswM_InitEventRequestPort();
#endif /* BSWM_EVENT_RQSTPORT_ENABLE == STD_ON */
        BswM_InitModeRequestPort();

        runPCCfgPtr->runtimeStatusPtr->bswmInitState = BSWM_INITED;
    }
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    else
    {
        /*no configure for this core, configure data error*/
        (void)Det_ReportError(BSWM_MODULE_ID, BSWM_INSTANCE_ID, BSWM_API_ID_INIT, BSWM_E_INIT_FAILED);
    }
#endif /*BSWM_DEV_ERROR_DETECT == STD_ON*/
}

/**
 * Main function of the BswM module that handles periodic tasks.
 * This function evaluates deferred rules, manages timers, and handles notifications and requests for BSW and SWC modes.
 * The function checks the initialization status and retrieves partition configuration pointers before performing these
 * tasks.
 */
/* PRQA S 1503,1505,1532,6030 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef, VL_MTR_BswM_STMIF */
void BswM_MainFunction(void)
/* PRQA S 1503,1505,1532,6030 --*/
{
#if (BSWM_TIMER_ENABLED == STD_ON)
    const BswM_ModeRqstPortRuntimeType* modeRunRqstPtr;
    BswM_TimerIdxType                   timerNum;
    BswM_TimerIdxType                   timerIdx;
#endif /*BSWM_TIMER_ENABLED == STD_ON*/
    BswM_RuleIndexType             defRuleNum;
    BswM_RuleIndexType             defIdx;
    BswM_RuleIndexType             ruleIdx;
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
    const BswM_PartitionPbConfigType* bswmPartPbConfigPtr;
#endif /* BSWM_PB_CONFIG_ENABLED */
    const BswM_PartitionLCfgType* bswmPartLCfgPtr;
    boolean                       result;

    if (BswM_GetCoreInitStatus())
    {
        result = BswM_GetPartitionLcPCCfgPtr(
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
            &bswmPartPbConfigPtr,
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
            &bswmPartPCCfgPtr,
            &bswmPartLCfgPtr);
        if (result) /* PRQA S 2991,2995 */ /* VL_BswM_2991,VL_BswM_2995 */
        {
            /*1.Perform evaluation of all deferred rules.*/
            defRuleNum = bswmPartLCfgPtr->numOfDefRules;
            for (ruleIdx = 0u; ruleIdx < defRuleNum; ruleIdx++)
            {
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
                if ((bswmPartLCfgPtr->defRuleRefLCfg[ruleIdx].pbConfigTag == BswM_GetPBConfigTags())
                    || (BSWM_PB_CONFIG_IN_ALL_VARIANTS == bswmPartLCfgPtr->defRuleRefLCfg[ruleIdx].pbConfigTag))
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
                {
                    defIdx = bswmPartLCfgPtr->defRuleRefLCfg[ruleIdx].ruleIndex;
                    BswM_ArbitrateRule(bswmPartLCfgPtr, defIdx, FALSE);
                }
            }
            /*2.Handle BSWM timer.*/
#if (BSWM_TIMER_ENABLED == STD_ON)
            timerNum       = bswmPartPCCfgPtr->modeRqstPCCfg->numOfBswMTimer;
            modeRunRqstPtr = bswmPartPCCfgPtr->modeRqstPortRunPtr;
            for (timerIdx = 0u; timerIdx < timerNum; timerIdx++)
            {
                if ((BSWM_TIMER_START == modeRunRqstPtr->timerStatus[timerIdx])
                    && (modeRunRqstPtr->timerCycle[timerIdx] > 0uL))
                {
                    modeRunRqstPtr->timerCycle[timerIdx]--;
                    if (0uL == modeRunRqstPtr->timerCycle[timerIdx])
                    {
                        modeRunRqstPtr->timerStatus[timerIdx] = BSWM_TIMER_EXPIRED;
                    }
                }
            }
#endif /*BSWM_TIMER_ENABLED == STD_ON*/
#if (BSWM_BSW_MODE_DEF_NOTIFICATION == STD_ON)
            if (NULL_PTR != bswmPartLCfgPtr->bswModeNotificationFunc)
            {
                bswmPartLCfgPtr->bswModeNotificationFunc();
            }
#endif /* BSWM_BSW_MODE_DEF_NOTIFICATION == STD_ON */
#if (BSWM_SWC_MODE_DEF_NOTIFICATION == STD_ON)
            if (NULL_PTR != bswmPartLCfgPtr->swcModeNotificationFunc)
            {
                bswmPartLCfgPtr->swcModeNotificationFunc();
            }
#endif /* BSWM_SWC_MODE_DEF_NOTIFICATION == STD_ON */
#if (BSWM_SWC_MODE_DEF_REQUEST == STD_ON)
            if (NULL_PTR != bswmPartLCfgPtr->bswModeRequestFunc)
            {
                bswmPartLCfgPtr->bswModeRequestFunc();
            }
#endif /* BSWM_BSW_MODE_DEF_REQUEST == STD_ON */
        }
    }
}

/**
 * Deinitializes the BswM module by resetting the initialization state.
 * This function sets the BswM module's runtime status to uninitialized.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_Deinit(void)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PCCfgType* runPCCfgPtr;
    if (BswM_GetCorePCCfgPtr(&runPCCfgPtr))
    {
        runPCCfgPtr->runtimeStatusPtr->bswmInitState = BSWM_UNINITED;
    }
}

#if (BSWM_GENERIC_REQUEST_ENABLED == STD_ON)
#if (BSWM_GENERICREQUEST_ENABLED == STD_ON)
/**
 * Handles a mode request from a specified user.
 * This function processes the requested mode and updates the internal state accordingly.
 * If the request is immediate, it also evaluates the associated rules.
 */
/* PRQA S 1503,1505,1532,6030 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef, VL_MTR_BswM_STMIF */
void BswM_RequestMode(BswM_UserType requesting_user, BswM_ModeType requested_mode)
/* PRQA S 1503,1505,1532,6030 --*/
{
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
    BswM_UserType                       numOfGenericReq;
    BswM_UserType                       userIdx;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    /*Don't check requested_mode*/
    if ((Std_ReturnType)E_OK == BswM_DetChkGenericRqst(requesting_user))
#endif /*BSWM_DEV_ERROR_DETECT == STD_ON*/
    {
        result = BswM_GetPartitionLcPCCfgPtr(
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
            &bswmPartPbConfigPtr,
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
            &bswmPartPCCfgPtr,
            &bswmPartLCfgPtr);
        if (result) /* PRQA S 2991,2995 */ /* VL_BswM_2991,VL_BswM_2995 */
        {
            modeRqstPCfgPtr = bswmPartPCCfgPtr->modeRqstPCCfg;
            modeRqstLCfgPtr = bswmPartLCfgPtr->modeRqstLCfg;
            numOfGenericReq = modeRqstPCfgPtr->numOfGenericReq;
            for (userIdx = 0u; userIdx < numOfGenericReq; userIdx++)
            {
                if (requesting_user == modeRqstLCfgPtr->genRqstUserRef[userIdx])
                {
                    modeRqstPtr                            = bswmPartPCCfgPtr->modeRqstPortRunPtr;
                    modeRqstPtr->genericReqStatus[userIdx] = requested_mode;
                    if (BSWM_IMMEDIATE == modeRqstLCfgPtr->genericRqst[userIdx].process)
                    {
                        numOfRules = modeRqstLCfgPtr->genericRqst[userIdx].belongToRlueNum;
                        for (idx = 0u; idx < numOfRules; idx++)
                        {
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
                            if ((modeRqstLCfgPtr->genericRqst[userIdx].belongToRlue[idx].pbConfigTag
                                 == BswM_GetPBConfigTags())
                                || (BSWM_PB_CONFIG_IN_ALL_VARIANTS
                                    == modeRqstLCfgPtr->genericRqst[userIdx].belongToRlue[idx].pbConfigTag))
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
                            {
                                ruleIdx = modeRqstLCfgPtr->genericRqst[userIdx].belongToRlue[idx].ruleIndex;
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

/**
 * Initializes the generic request ports with their initial modes.
 * This function sets the initial status of generic request ports based on the configuration.
 * If initialization fails, an error is reported.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_InitGenericRequestPorts(void)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
    const BswM_PartitionPbConfigType* bswmPartPbConfigPtr;
#endif /* BSWM_PB_CONFIG_ENABLED */
    const BswM_PartitionLCfgType*       bswmPartLCfgPtr;
    const BswM_ModeRqstPortPCCfgType*   modeRqstPCfgPtr;
    const BswM_ModeRqstPortRuntimeType* modeRqstPtr;
    boolean                             result;
    BswM_UserType                       numOfGenericReq;
    BswM_UserType                       userIdx;

    result = BswM_GetPartitionLcPCCfgPtr(
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
        &bswmPartPbConfigPtr,
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
        &bswmPartPCCfgPtr,
        &bswmPartLCfgPtr);
    if (result) /* PRQA S 2991,2995 */ /* VL_BswM_2991,VL_BswM_2995 */
    {
        modeRqstPCfgPtr = bswmPartPCCfgPtr->modeRqstPCCfg;
        modeRqstPtr     = bswmPartPCCfgPtr->modeRqstPortRunPtr;
        numOfGenericReq = modeRqstPCfgPtr->numOfGenericReq;
        for (userIdx = 0u; userIdx < numOfGenericReq; userIdx++)
        {
            if (NULL_PTR != modeRqstPCfgPtr->genRqstInitMode)
            {
                modeRqstPtr->genericReqStatus[userIdx] = *(modeRqstPCfgPtr->genRqstInitMode);
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
 * Retrieves the status of a generic request port.
 * This function returns the current status of the specified generic request port.
 * If the partition configuration cannot be retrieved, an invalid status is returned.
 */
uint16 BswM_GetGeneRqstStatus(BswM_UserType userIdx)
{
    uint16                         retStatus = BSWM_INVALID_U16; /* PRQA S 2981 */ /* VL_BswM_2981 */
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
    const BswM_PartitionPbConfigType* bswmPartPbConfigPtr;
#endif /* BSWM_PB_CONFIG_ENABLED */
    const BswM_PartitionLCfgType* bswmPartLCfgPtr;
    boolean                       result;

    result = BswM_GetPartitionLcPCCfgPtr(
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
        &bswmPartPbConfigPtr,
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
        &bswmPartPCCfgPtr,
        &bswmPartLCfgPtr);
    if (result) /* PRQA S 2991,2995 */ /* VL_BswM_2991,VL_BswM_2995 */
    {
        retStatus = bswmPartPCCfgPtr->modeRqstPortRunPtr->genericReqStatus[userIdx];
    }
    return retStatus;
}
#endif /*BSWM_GENERICREQUEST_ENABLED == STD_ON*/
#endif /*BSWM_GENERIC_REQUEST_ENABLED == STD_ON*/

#if (BSWM_VERSION_INFO_API == STD_ON)
/**
 * This function retrieves the version information of the BswM module and stores it in the provided VersionInfo
 * structure. If the development error detection is enabled and the provided pointer is NULL, an error is reported using
 * Det_ReportError. Otherwise, the module ID, vendor ID, and software version numbers (major, minor, patch) are assigned
 * to the corresponding fields in the VersionInfo structure.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_GetVersionInfo(Std_VersionInfoType* VersionInfo)
/* PRQA S 1503,1505,1532 --*/
{
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if (NULL_PTR == VersionInfo)
    {
        (void)Det_ReportError(BSWM_MODULE_ID, BSWM_INSTANCE_ID, BSWM_API_ID_GET_VERSION_INFO, BSWM_E_NULL_POINTER);
    }
    else
#endif /*BSWM_DEV_ERROR_DETECT == STD_ON*/
    {
        VersionInfo->moduleID         = BSWM_MODULE_ID;
        VersionInfo->vendorID         = BSWM_VENDOR_ID;
        VersionInfo->sw_major_version = BSWM_SW_MAJOR_VERSION;
        VersionInfo->sw_minor_version = BSWM_SW_MINOR_VERSION;
        VersionInfo->sw_patch_version = BSWM_SW_PATCH_VERSION;
    }
}
#endif /*BSWM_VERSION_INFO_API == STD_ON*/

#if ((BSWM_MULTI_PARTITION_ENABLED == STD_ON) && (BSWM_EVENT_PARTITION_RESTARTED_ENABLE == STD_ON))
/**
 * This function handles the restart of a BswM partition.
 * It first checks for any development errors if error detection is enabled.
 * If no errors are detected, it retrieves the configuration pointers for the partition and sets the event request port
 * to indicate that the partition has restarted. If the process type is immediate, it evaluates all rules associated
 * with the partition's restart event.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
void BswM_BswMPartitionRestarted(void)
/* PRQA S 1503,1505,1532 --*/
{
    const BswM_EventRqstPortLCfgType* evRqstLCfgPtr;
    BswM_EventRquestPortRuntimeType*  evRqstPtr;
    BswM_RuleIndexType                numOfRules;
    BswM_RuleIndexType                idx;
    BswM_RuleIndexType                ruleIdx;
    BswM_EventRqstPortIdxType         evIdx;
    const BswM_PartitionPCCfgType*    bswmPartPCCfgPtr;
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
    const BswM_PartitionPbConfigType* bswmPartPbConfigPtr;
#endif /* BSWM_PB_CONFIG_ENABLED */
    const BswM_PartitionLCfgType* bswmPartLCfgPtr;
    boolean                       result;

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if (E_OK == BswM_DetChkParRestart())
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
            evRqstPtr        = bswmPartPCCfgPtr->eventRqstPortRunPtr;
            evIdx            = *(bswmPartPCCfgPtr->evRqstPCCfg->parRestartIdxPtr);
            evRqstPtr[evIdx] = BSWM_EVENT_IS_SET;
            evRqstLCfgPtr    = bswmPartLCfgPtr->evRqstLCfg;
            if (BSWM_IMMEDIATE == evRqstLCfgPtr->bswMParRestart->process)
            {
                numOfRules = evRqstLCfgPtr->bswMParRestart->belongToRlueNum;
                for (idx = 0u; idx < numOfRules; idx++)
                {
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
                    if ((evRqstLCfgPtr->bswMParRestart->belongToRlue[idx].pbConfigTag == BswM_GetPBConfigTags())
                        || (BSWM_PB_CONFIG_IN_ALL_VARIANTS
                            == evRqstLCfgPtr->bswMParRestart->belongToRlue[idx].pbConfigTag))
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
                    {
                        ruleIdx = evRqstLCfgPtr->bswMParRestart->belongToRlue[idx].ruleIndex;
                        /* An Independent rule,to be evaluated each time applicable*/
                        BswM_ArbitrateRule(bswmPartLCfgPtr, ruleIdx, FALSE);
                    }
                }
            }
        }
    }
}

/**
 * This function retrieves the status of the partition restart event.
 * It first checks if the partition configuration pointers can be successfully retrieved.
 * If successful, it gets the index of the event request port for the partition restart and returns the current status
 * of this event.
 */
/* PRQA S 1503,1505,1532 ++*/ /* VL_QAC_NoUsedApi, VL_BswM_1505, VL_QAC_OneFunRef */
BswM_EventRquestPortRuntimeType BswM_GetParRestartStatus(void)
/* PRQA S 1503,1505,1532 --*/
{
    BswM_EventRquestPortRuntimeType parRstStatus = BSWM_INVALID_U8;
    BswM_EventRqstPortIdxType       evIdx;
    const BswM_PartitionPCCfgType*  bswmPartPCCfgPtr;
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
    const BswM_PartitionPbConfigType* bswmPartPbConfigPtr;
#endif /* BSWM_PB_CONFIG_ENABLED */
    const BswM_PartitionLCfgType* bswmPartLCfgPtr;
    boolean                       result;

    result = BswM_GetPartitionLcPCCfgPtr(
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
        &bswmPartPbConfigPtr,
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
        &bswmPartPCCfgPtr,
        &bswmPartLCfgPtr);
    if (result)
    {
        evIdx        = *(bswmPartPCCfgPtr->evRqstPCCfg->parRestartIdxPtr);
        parRstStatus = bswmPartPCCfgPtr->eventRqstPortRunPtr[evIdx];
    }
    return parRstStatus;
}
#endif /*BSWM_MULTI_PARTITION_ENABLED == STD_ON) && (BSWM_EVENT_PARTITION_RESTARTED_ENABLE == STD_ON*/

/**
 * This function arbitrates the execution of a specified rule for a given partition.
 * It retrieves the rule configuration and checks if the rule is not nested.
 * If the rule is not nested, it calls the mode arbitration function to determine the action list index.
 * If a valid action list index is returned, it retrieves the corresponding action list and executes the control
 * function for that list.
 */
void BswM_ArbitrateRule(const BswM_PartitionLCfgType* bswmPartLCfgPtr, BswM_RuleIndexType ruleIdx, boolean nestedTag)
{
    const BswM_RuleLcCfgType*       ruleLCfgPtr;
    const BswM_ActionListLcCfgType* actionListLCfgptr;
    BswM_ActionListHandlerType      actListIdx;

    ruleLCfgPtr = &(bswmPartLCfgPtr->ruleLCfg[ruleIdx]);
    if ((!ruleLCfgPtr->isNestExe) || (nestedTag))
    {
        /*Execution mode arbitration function*/
        actListIdx = ruleLCfgPtr->modeArbitraFunc();

        if (actListIdx != BSWM_MAX_ACTIONLIST_NUM)
        {
            /* Get the list of actions to perform, and execute them */
            actionListLCfgptr = &bswmPartLCfgPtr->actionListLCfg[actListIdx];
            actionListLCfgptr->actionListCtrlFct();
        }
    }
}

/**
 * This function checks the initialization status of the core.
 * It retrieves the core configuration pointer and checks if the runtime status indicates that the core has been
 * initialized. If the core is initialized, it sets the status to TRUE; otherwise, it remains FALSE.
 */
boolean BswM_GetCoreInitStatus(void)
{
    boolean               status = FALSE;
    const BswM_PCCfgType* runPCCfgPtr;

    if (BswM_GetCorePCCfgPtr(&runPCCfgPtr))
    {
        if (BSWM_INITED == runPCCfgPtr->runtimeStatusPtr->bswmInitState)
        {
            status = TRUE;
        }
    }
    return status;
}

#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
/**
 * This function retrieves the PB configuration tags from the BswM configuration.
 * It returns the value of the PB configuration tag stored in the configuration pointer.
 */
uint8 BswM_GetPBConfigTags(void)
{
    return BswM_ConfigPtr->BswM_PbCfgTag;
}
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
/* ========================================== internal function definitions ========================================= */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
/**
 * Initializes the BswM event request ports by setting them to a cleared state.
 * This function retrieves the configuration and runtime data, iterates through each event request port, and sets their
 * status to cleared. If the configuration retrieval fails, an error is reported using the Det_ReportError function.
 */
#if (BSWM_EVENT_RQSTPORT_ENABLE == STD_ON)
BSWM_LOCAL void BswM_InitEventRequestPort(void)
{
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
    const BswM_PartitionPbConfigType* bswmPartPbConfigPtr;
#endif /* BSWM_PB_CONFIG_ENABLED */
    const BswM_PartitionLCfgType*    bswmPartLCfgPtr;
    BswM_EventRquestPortRuntimeType* evRqstPtr;
    BswM_EventRqstPortIdxType        numOfEvPort;
    BswM_EventRqstPortIdxType        evIdx;
    boolean                          result;

    result = BswM_GetPartitionLcPCCfgPtr(
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
        &bswmPartPbConfigPtr,
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
        &bswmPartPCCfgPtr,
        &bswmPartLCfgPtr);
    if (result) /* PRQA S 2991,2995 */ /* VL_BswM_2991,VL_BswM_2995 */
    {
        evRqstPtr   = bswmPartPCCfgPtr->eventRqstPortRunPtr;
        numOfEvPort = bswmPartPCCfgPtr->evRqstSrcNum;
        for (evIdx = 0u; evIdx < numOfEvPort; evIdx++)
        {
            /*All BswMEventRequestPorts shall be initialized to a CLEAR state.*/
            evRqstPtr[evIdx] = BSWM_EVENT_IS_CLEARED;
        }
    }
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    else
    {
        (void)Det_ReportError(BSWM_MODULE_ID, BSWM_INSTANCE_ID, BSWM_API_ID_INIT, BSWM_E_INIT_FAILED);
    }
#endif /*BSWM_DEV_ERROR_DETECT == STD_ON*/
}
#endif /*BSWM_EVENT_RQSTPORT_ENABLE == STD_ON*/

/**
 * Initializes the mode request ports for various BswM modules based on the enabled configurations.
 * This function checks multiple configuration macros and calls the corresponding initialization functions
 * for each module if the respective configuration is enabled.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_BswM_STCAL */
BSWM_LOCAL void BswM_InitModeRequestPort(void)
/* PRQA S 6070 -- */
{
#if (BSWM_CANSM_ENABLED == STD_ON)
#if (BSWM_CANSMINDICATION_ENABLED == STD_ON)
    BswM_InitCanSMRequestPorts();
#endif /*BSWM_CANSM_ENABLED == STD_ON*/
#endif /*BSWM_CANSMINDICATION_ENABLED == STD_ON*/

#if (BSWM_COMM_ENABLED == STD_ON)
    BswM_InitComMRequestPorts();
#endif /*BSWM_COMM_ENABLED == STD_ON*/

#if (BSWM_DCM_ENABLED == STD_ON)
#if (BSWM_DCMCOMMODEREQUEST_ENABLED == STD_ON)
    BswM_InitDcmRequestPorts();
#endif /*BSWM_DCMCOMMODEREQUEST_ENABLED == STD_ON*/
#endif /*BSWM_DCM_ENABLED == STD_ON*/

#if (BSWM_ECUM_ENABLED == STD_ON)
    BswM_InitEcuMRequestPorts();
#endif /*BSWM_ECUM_ENABLED == STD_ON*/

#if (BSWM_ETHIF_ENABLED == STD_ON)
#if (BSWM_ETHIFPORTGROUPLINKSTATECHG_ENABLED == STD_ON)
    BswM_InitEthIfRequestPorts();
#endif /*BSWM_ETHIFPORTGROUPLINKSTATECHG_ENABLED == STD_ON*/
#endif /*BSWM_ETHIF_ENABLED == STD_ON*/

#if (BSWM_ETHSM_ENABLED == STD_ON)
#if (BSWM_ETHSMINDICATION_ENABLED == STD_ON)
    BswM_InitEthSMRequestPorts();
#endif /*BSWM_ETHSMINDICATION_ENABLED == STD_ON*/
#endif /*BSWM_ETHSM_ENABLED == STD_ON*/

#if (BSWM_FRSM_ENABLED == STD_ON)
#if (BSWM_FRSMINDICATION_ENABLED == STD_ON)
    BswM_InitFrSMRequestPorts();
#endif /*BSWM_FRSMINDICATION_ENABLED == STD_ON*/
#endif /*BSWM_FRSM_ENABLED == STD_ON*/

#if (BSWM_GENERIC_REQUEST_ENABLED == STD_ON)
#if (BSWM_GENERICREQUEST_ENABLED == STD_ON)
    BswM_InitGenericRequestPorts();
#endif /*BSWM_GENERICREQUEST_ENABLED == STD_ON*/
#endif /*BSWM_GENERIC_REQUEST_ENABLED == STD_ON*/

#if (BSWM_J1939DCM_ENABLED == STD_ON)
#if (BSWM_J1939DCMBROADCASTSTATUS_ENABLED == STD_ON)
    BswM_InitJ1939DcmRequestPorts();
#endif /*BSWM_J1939DCMBROADCASTSTATUS_ENABLED == STD_ON*/
#endif /*BSWM_J1939DCM_ENABLED == STD_ON*/

#if (BSWM_J1939NM_ENABLED == STD_ON)
#if (BSWM_J1939NMINDICATION_ENABLED == STD_ON)
    BswM_InitJ1939NmRequestPorts();
#endif /*BSWM_J1939NMINDICATION_ENABLED == STD_ON*/
#endif /*BSWM_J1939NM_ENABLED == STD_ON*/

#if (BSWM_LINSM_ENABLED == STD_ON)
#if (BSWM_LINSMINDICATION_ENABLED == STD_ON)
    BswM_InitLinSMRequestPorts();
#endif /*BSWM_LINSMINDICATION_ENABLED == STD_ON*/
#endif /*BSWM_LINSM_ENABLED == STD_ON*/

#if (BSWM_LINTP_ENABLED == STD_ON)
#if (BSWM_LINTPMODEREQUEST_ENABLED == STD_ON)
    BswM_InitLinTpRequestPorts();
#endif /*BSWM_LINTPMODEREQUEST_ENABLED == STD_ON*/
#endif /*BSWM_LINTP_ENABLED == STD_ON*/

#if (BSWM_NMSTATECHANGENOTIFICATION_ENABLED == STD_ON)
    BswM_InitNmRequestPorts();
#endif /*BSWM_NMSTATECHANGENOTIFICATION_ENABLED == STD_ON*/

#if (BSWM_NVM_ENABLED == STD_ON)
    BswM_InitNvMRequestPorts();
#endif /*BSWM_NVM_ENABLED == STD_ON*/

#if (BSWM_SD_ENABLED == STD_ON)
    BswM_InitSDRequestPorts();
#endif /*BSWM_SD_ENABLED == STD_ON*/

#if (BSWM_SOAD_ENABLED == STD_ON)
    BswM_InitSoAdRequestPorts();
#endif /*BSWM_SOAD_ENABLED == STD_ON*/

#if (BSWM_TIMER_ENABLED == STD_ON)
    BswM_InitBswMTimer();
#endif /*BSWM_TIMER_ENABLED == STD_ON*/
}

/**
 * Initializes the rules for the BswM module by setting up the rule statuses and deferred rule flags.
 * This function retrieves partition configuration pointers, initializes the rule statuses to their initial states,
 * and sets the `ruleNeedArbitrate` flag for deferred rules.
 */
BSWM_LOCAL void BswM_InitRules(void)
{
    const BswM_RuleLcCfgType*      ruleLCfgPtr;
    BswM_RuleRunTimeType*          ruleStatusPtr; /* PRQA S 3432 */ /* VL_BswM_3432 */
    const BswM_PartitionPCCfgType* bswmPartPCCfgPtr;
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
    const BswM_PartitionPbConfigType* bswmPartPbConfigPtr;
#endif /* BSWM_PB_CONFIG_ENABLED */
    const BswM_PartitionLCfgType* bswmPartLCfgPtr;
    BswM_RuleIndexType            numOfRules;
    BswM_RuleIndexType            ruleIdx;
    BswM_RuleIndexType            defRuleIdx;
    boolean                       result;

    result = BswM_GetPartitionLcPCCfgPtr(
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
        &bswmPartPbConfigPtr,
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
        &bswmPartPCCfgPtr,
        &bswmPartLCfgPtr);
    if (result) /* PRQA S 2991,2995 */ /* VL_BswM_2991,VL_BswM_2995 */
    {
        ruleLCfgPtr   = bswmPartLCfgPtr->ruleLCfg;
        ruleStatusPtr = bswmPartLCfgPtr->ruleRunTimeStatus;
        numOfRules    = bswmPartLCfgPtr->numOfRules;
        for (ruleIdx = 0u; ruleIdx < numOfRules; ruleIdx++)
        {
            ruleStatusPtr[ruleIdx].ruleNeedArbitrate = FALSE;
            ruleStatusPtr[ruleIdx].ruleStatus        = ruleLCfgPtr[ruleIdx].ruleInitState;
        }
        /*Set deferred rule status*/
        numOfRules = bswmPartLCfgPtr->numOfDefRules;
        for (ruleIdx = 0u; ruleIdx < numOfRules; ruleIdx++)
        {
            defRuleIdx = bswmPartLCfgPtr->defRuleRefLCfg[ruleIdx].ruleIndex;
            /*Only for deferred rule need to set the flag.*/
            ruleStatusPtr[defRuleIdx].ruleNeedArbitrate = TRUE;
        }
    }
}

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
