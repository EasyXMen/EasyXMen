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
**  FILENAME    : LinSM.c                                                     **
**                                                                            **
**  Created on  :                                                             **
**  Author      : HuRongbo                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation for LinSM                                    **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/* <VERSION>   <DATE>     <AUTHOR>   <REVISION LOG>
 * V1.0.0      20171201   HuRongbo   Initial version
 * V2.0.0      20200814   HuRongbo   Update for R19-11
 * V2.0.1      20210812   HuRongbo   Bug fix
 * V2.0.2      20220711   HuRongbo   QAC check issue fix
 * V2.0.3      20220921   HuRongbo
 *   1> Add check if module initialized in LinSM_MainFunction()
 * V2.0.4      20221114   HuRongbo
 *   1> Fix array access out of bounds issues in LinSM_RequestComMode()
 *   (JIRA: PRD0032020-233)
 *   2> Change private variable keyword from static to macro LINIF_LOCAL
 * V2.0.5      20230314   HuRongbo
 *   1> Resolves an exception that is raised when LinSMConfirmationTimeout is
 *   set to 0(JIRA:PRD0042022-590)
 * V2.0.6      20231128   SunShengnan
 *   1>LinSM_CheckSchedule,Compile warning clean.(CPT-7666)
 * V2.0.7      20240229   SunShengnan   QAC_PH_2024 check.
 *                                      LinSM is UNINIT, NetworkHandle is NULL_PTR.
 * V2.0.8      20240309   SunShengnan    Support multiple partition feature.
 * V2.0.9      20240710   liucaihong     Remove the LINSM_GOTOSLEEP_TIMER requirement
 * V2.0.10     20241127   liucaihong
 *  1> Fixed CPT-11559:  Fix Det_ReportRuntimeError violates MISRA 2012 Rule 17.3
 *  2> Fixed CPT-11561:  Fix the function partition and det coupling in LinSM_Mainfunction.
 *  3> Fixed CPT-11562:  Fix the LINSM_IS_TRANSCEIVER_PASSIVER_MODE_SUPPORT judgment condition
 */

/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:LinSM<br>
  RuleSorce:puhua-rule.rcf 2.3.1

    \li PRQA S 2877 MISRA Dir 4.1 .<br>
    Reason:The exception is that the number of loops is configurable

    \li PRQA S 1532 MISRA Rule 8.7 .<br>
    Reason:The exception is API(1532).
 */
/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/
#define LINSM_C_AR_RELEASE_MAJOR_VERSION    4
#define LINSM_C_AR_RELEASE_MINOR_VERSION    5
#define LINSM_C_AR_RELEASE_REVISION_VERSION 0

#define LINSM_C_SW_MAJOR_VERSION            2
#define LINSM_C_SW_MINOR_VERSION            0
#define LINSM_C_SW_PATCH_VERSION            10

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "LinSM.h"
/*@req <SWS_LinSM_00201> */
#include "BswM_LinSM.h"
/*@req <SWS_LinSM_00305> */
#include "ComM_BusSM.h"
/*@req <SWS_LinSM_00306> */
#include "Lin_GeneralTypes.h"
#include "SchM_LinSM.h"
#include "LinSM_Cbk.h"
#include "Det.h"
/* Multiple partition check. */
#if (STD_ON == LINSM_MULTIPLE_PARTITION_USED)
#include "ComM.h"
#include "Os.h"
#endif

/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
/*@req <SWS_LinSM_00208> */
#if (LINSM_C_AR_RELEASE_MAJOR_VERSION != LINSM_H_AR_RELEASE_MAJOR_VERSION)
#error "LinSM.c : Mismatch in Specification Major Version"
#endif
#if (LINSM_C_AR_RELEASE_MINOR_VERSION != LINSM_H_AR_RELEASE_MINOR_VERSION)
#error "LinSM.c : Mismatch in Specification Minor Version"
#endif
#if (LINSM_C_AR_RELEASE_REVISION_VERSION != LINSM_H_AR_RELEASE_REVISION_VERSION)
#error "LinSM.c : Mismatch in Specification Revision Version"
#endif
#if (LINSM_C_SW_MAJOR_VERSION != LINSM_H_SW_MAJOR_VERSION)
#error "LinSM.c : Mismatch in Specification Major Version"
#endif
#if (LINSM_C_SW_MINOR_VERSION != LINSM_H_SW_MINOR_VERSION)
#error "LinSM.c : Mismatch in Specification Minor Version"
#endif

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/
/* Current status of LinSM Module */
/*@req <SWS_LinSM_00020> */
typedef enum
{
    /*@req <SWS_LinSM_00022> */
    LINSM_UNINIT,
    /*@req <SWS_LinSM_00024> */
    LINSM_INIT
} LinSM_ModuleStatusType;

/* Sub-status of LINSM_FULL_COM */
typedef enum
{
    LINSM_RUN_COMMUNICATION,
    LINSM_GOTO_SLEEP
} LinSM_FullComSubStatusType;

/* Handling Request */
typedef enum
{
    LINSM_EXE_NOTHING,
    LINSM_EXE_GOTO_SLEEP,
    LINSM_EXE_WAKE_UP,
    LINSM_EXE_CHANGE_SCHEDULE
} LinSM_HandlingRequestType;

typedef enum
{
    LINSM_NONE_TIMER,
    LINSM_WAKEUP_TIMER,
    LINSM_SCHEDULEREQUEST_TIMER,
    LINSM_SILENCEWAKEUP_TIMER
} LinSM_TimerType;

typedef struct
{
    uint32 timerCnt;
    LinSM_TimerType timerType;
    uint32 silenceAfterWakeupTimer;
    LinSM_ModeType channelComMode;
    /* Use only when channelComMode = LINSM_FULL_COM*/
    LinSM_FullComSubStatusType fullComSubStatus;
    LinIf_SchHandleType curSchedule;
    LinSM_HandlingRequestType runningReq;
    Std_ReturnType req2LinIfResult;
    uint8 wakeupRetryCnt; /* Wakeup retry Counter */
    ComM_ModeType reqComMode;
    boolean reqComModeFlag;
} LinSM_RunTimeType;

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
#define LINSM_NETWORK_HANDLE(ch)             LinSM_CfgPtr->channelPtr[ch].comMNetworkHandleRef
#define LINSM_CHANNEL_SCHEDULE_INDEX(ch, i)  LinSM_CfgPtr->channelPtr[ch].schedulePtr[i].scheduleIndexRef
#define LINSM_CONFIRMATION_TIMEOUT(ch)       LinSM_CfgPtr->channelPtr[ch].confirmationTimeout
#define LINSM_MODE_REQUEST_REPETITION_MAX()  LinSM_CfgPtr->modeRequestRepetitionMax
#define LINSM_NODE_TYPE(ch)                  LinSM_CfgPtr->channelPtr[ch].nodeType
#define LINSM_SILENCEAFTERWAKEUP_TIMEOUT(ch) LinSM_CfgPtr->channelPtr[ch].silenceAfterWakeupTimeout

#if (STD_ON == LINSM_TRANSCEIVER_PASSIVER_MODE_SUPPORT)
#define LINSM_IS_TRANSCEIVER_PASSIVER_MODE_SUPPORT(ch) LinSM_CfgPtr->channelPtr[ch].isTransceiverPassiveModeSupport
#define LINSM_GET_TRANSCEIVER_PASSIVER_MODE(ch)        LinSM_CfgPtr->channelPtr[ch].transceiverPassiveMode
#endif /* STD_ON == LINSM_TRANSCEIVER_PASSIVER_MODE_SUPPORT */

#if (STD_ON == LINSM_DEV_ERROR_DETECT)
#define LINSM_CHANNEL_SCHEDULE_NUM(ch) LinSM_CfgPtr->channelPtr[ch].scheduleNum
#endif /* STD_ON == LINSM_DEV_ERROR_DETECT */

#define LINSM_UNUSED(x) (void)(x)
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define LINSM_START_SEC_CODE
#include "LinSM_MemMap.h"

static FUNC(uint8, LINSM_CODE) LinSM_GetChannelByNetworkHandle(NetworkHandleType network);

#if ((STD_ON == LINSM_DEV_ERROR_DETECT) && (LINSM_MASTER_NODE_SUPPORT == STD_ON))
static FUNC(boolean, LINSM_CODE) LinSM_CheckSchedule(uint8 ch, LinIf_SchHandleType schedule);
#endif /* STD_ON == LINSM_DEV_ERROR_DETECT && LINSM_MASTER_NODE_SUPPORT == STD_ON */

static FUNC(void, LINSM_CODE) LinSM_HandleGotoSleepPeriod(uint8 chl);

static FUNC(void, LINSM_CODE) LinSM_HandleWakeupPeriod(uint8 chl);

static FUNC(void, LINSM_CODE) LinSM_HandleChangeSchedulePeriod(uint8 chl);

#define LINSM_STOP_SEC_CODE
#include "LinSM_MemMap.h"
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
#if !defined(LINSM_LOCAL)
#define LINSM_LOCAL static
#endif

#define LINSM_START_SEC_VAR_INIT_UNSPECIFIED
#include "LinSM_MemMap.h"
LINSM_LOCAL VAR(LinSM_ModuleStatusType, LINSM_VAR_POWER_ON_INIT) LinSM_ModuleStatus = LINSM_UNINIT;
#define LINSM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "LinSM_MemMap.h"

#define LINSM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "LinSM_MemMap.h"
LINSM_LOCAL VAR(LinSM_RunTimeType, LINSM_VAR) LinSM_RTArray[LINSM_CHANNEL_NUM];
#define LINSM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "LinSM_MemMap.h"

/* global variable to store the configuration pointer */
#define LINSM_START_SEC_VAR_INIT_PTR
#include "LinSM_MemMap.h"
LINSM_LOCAL P2CONST(LinSM_ConfigType, LINSM_VAR, LINSM_CONST) LinSM_CfgPtr = NULL_PTR;
#define LINSM_STOP_SEC_VAR_INIT_PTR
#include "LinSM_MemMap.h"
/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define LINSM_START_SEC_CODE
#include "LinSM_MemMap.h"
/******************************************************************************/
/*
 * Brief               This function initializes the LinSM.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[in/out]  N/A
 * Param-Name[out]     N/A
 * Return              N/A
 */
/******************************************************************************/
FUNC(void, LINSM_CODE)
LinSM_Init(P2CONST(LinSM_ConfigType, AUTOMATIC, LINSM_CONST_PBCFG) ConfigPtr) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (STD_ON == LINSM_DEV_ERROR_DETECT)
    /* check whether configuration pointer is null,if null, report det error */
    if (NULL_PTR == ConfigPtr)
    {
        (void)Det_ReportError(LINSM_MODULE_ID, LINSM_INSTANCE_ID, LINSM_SERVICE_ID_INIT, LINSM_E_PARAM_POINTER);
    }
    else
#endif
    {
        LinSM_CfgPtr = ConfigPtr;

        /*@req <SWS_LinSM_00043> */
        SchM_Enter_LinSM_Context();
        LinSM_RunTimeType* LinSM_RTPtr;
        uint8 loop = 0u;
        /* initialize run time structure variable for every channel */
        while (loop < LINSM_CHANNEL_NUM) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
        {
            LinSM_RTPtr = &LinSM_RTArray[loop];
            /*@req <SWS_LinSM_00152>,<SWS_LinSM_00160> */
            LinSM_RTPtr->channelComMode = LINSM_NO_COM;
            /*@req <SWS_LinSM_00216> */
            LinSM_RTPtr->curSchedule = LINSM_NULL_SCHEDULE_INDEX;
            LinSM_RTPtr->timerCnt = 0x00;
            LinSM_RTPtr->runningReq = LINSM_EXE_NOTHING;
            LinSM_RTPtr->req2LinIfResult = E_NOT_OK;
            LinSM_RTPtr->wakeupRetryCnt = 0x00;
            loop++;
        }
        /*@req <SWS_LinSM_00025> */
        /* change module state */
        LinSM_ModuleStatus = LINSM_INIT;
        SchM_Exit_LinSM_Context();
    }
}

#if (LINSM_MASTER_NODE_SUPPORT == STD_ON)
/******************************************************************************/
/*
 * Brief               The upper layer requests a schedule table to be changed
                       on one LIN network.
 * ServiceId           0x10
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      network:  Identification of the LIN channel
 *                     schedule: Pointer to the new Schedule table
 * Param-Name[in/out]  N/A
 * Param-Name[out]     N/A
 * Return              E_OK - Schedule table request has been accepted.
 *                     E_NOT_OK - Schedule table switch request has not been
 *                     accepted due to one of the following reasons:
 *                     * LinSM has not been initialized referenced channel does
 *                       not exist (identification is out of range)
 *                     * Referenced schedule table does not exist
 *                       (identification is out of range)
 *                     * Sub-state is not LINSM_FULL_COM
 */
/******************************************************************************/
FUNC(Std_ReturnType, LINSM_CODE)
LinSM_ScheduleRequest /* PRQA S 1503 */
    (NetworkHandleType network, LinIf_SchHandleType schedule)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == LINSM_DEV_ERROR_DETECT)
    /*@req <SWS_LinSM_00116> */
    /* the state LINSM_UNINIT is active ,report error */
    if (LINSM_UNINIT == LinSM_ModuleStatus)
    {
        (void)Det_ReportError(LINSM_MODULE_ID, LINSM_INSTANCE_ID, LINSM_SERVICE_ID_SCHEDULE_REQUEST, LINSM_E_UNINIT);
    }
    else
#endif /* STD_ON == LINSM_DEV_ERROR_DETECT */
    {
        /* get channel by network */
        uint8 chl = LinSM_GetChannelByNetworkHandle(network);
#if (STD_ON == LINSM_DEV_ERROR_DETECT)
/* Multiple partition check. */
#if (STD_ON == LINSM_MULTIPLE_PARTITION_USED)
        ApplicationType applicationID = GetApplicationID();
#endif
        /*@req <SWS_LinSM_00114> */
        /* the network parameter has an invalid value, report error */
        if (LINSM_CHANNEL_NUM == chl)
        {
            (void)Det_ReportError(
                LINSM_MODULE_ID,
                LINSM_INSTANCE_ID,
                LINSM_SERVICE_ID_SCHEDULE_REQUEST,
                LINSM_E_NONEXISTENT_NETWORK);
        }
        /*@req <SWS_LinSM_00115> */
        /* the Schedule parameter has an invalid value, report error*/
        else if (!LinSM_CheckSchedule(chl, schedule))
        {
            (void)Det_ReportError(
                LINSM_MODULE_ID,
                LINSM_INSTANCE_ID,
                LINSM_SERVICE_ID_SCHEDULE_REQUEST,
                LINSM_E_PARAMETER);
        }
        else
#if (STD_ON == LINSM_MULTIPLE_PARTITION_USED)
            /* Multiple partition check. */
            if (applicationID != ComM_GetChannelApplicationID(network))
            {
                (void)Det_ReportError(
                    LINSM_MODULE_ID,
                    LINSM_INSTANCE_ID,
                    LINSM_SERVICE_ID_SCHEDULE_REQUEST,
                    LINSM_E_PARTITION);
            }
            else
#endif
#endif
            {
                LinSM_RunTimeType* LinSM_RTPtr = &LinSM_RTArray[chl];

                /*@req <SWS_LinSM_00163> */
                /* If the function LinSM_ScheduleRequest is called and another
                 * request is in process on the same network,return with E_NOT_OK */
                /*@req <SWS_LinSM_10211> */
                /* If the function LinSM_ScheduleRequest is called and the
                 * state is not LINSM_FULL_COM, return directly with E_NOT_OK */
                if ((LINSM_EXE_NOTHING == LinSM_RTPtr->runningReq) && (LINSM_FULL_COM == LinSM_RTPtr->channelComMode))
                {
                    /* call Schedule request function of LinIf module */
                    /*@req <SWS_LinSM_00079>,<SWS_LinSM_00167>,<SWS_LinSM_00168>*/
                    ret = LinIf_ScheduleRequest(network, schedule);

                    SchM_Enter_LinSM_Context();
                    if ((E_OK == ret) && (0u != LINSM_CONFIRMATION_TIMEOUT(chl)))
                    {
                        /*@req <SWS_LinSM_00100>,<SWS_LinSM_00103> */
                        /* start timer */
                        LinSM_RTPtr->timerCnt = LINSM_CONFIRMATION_TIMEOUT(chl);
                    }
                    /* Runtime Information setup */
                    LinSM_RTPtr->runningReq = LINSM_EXE_CHANGE_SCHEDULE;
                    LinSM_RTPtr->req2LinIfResult = ret;
                    SchM_Exit_LinSM_Context();
                }
            }
    }

    return ret;
}
#endif /* LINSM_MASTER_NODE_SUPPORT == STD_ON */

/******************************************************************************/
/*
 * Brief               Function to query the current communication mode.
 * ServiceId           0x11
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      network: Identification of the LIN channel
 * Param-Name[in/out]  N/A
 * Param-Name[out]     mode: Returns the active mode,
 *                           see ComM_ModeType for descriptions of the modes
 * Return              E_OK - Ok
 *                     E_NOT_OK - Not possible to perform the request, e.g. not
 *                                initialized.
 */
/******************************************************************************/
FUNC(Std_ReturnType, LINSM_CODE)
LinSM_GetCurrentComMode /* PRQA S 1503 */
    (NetworkHandleType network, ComM_ModeType* mode)
{
    Std_ReturnType ret;
    /*@req <SWS_LinSM_00125> */
    /* the state LINSM_UNINIT is active ,report error */
#if (STD_ON == LINSM_DEV_ERROR_DETECT)
    ret = E_NOT_OK;
    if (LINSM_UNINIT == LinSM_ModuleStatus)
    {
        (void)Det_ReportError(LINSM_MODULE_ID, LINSM_INSTANCE_ID, LINSM_SERVICE_ID_GET_CURRENTCOMMODE, LINSM_E_UNINIT);
        /*@req <SWS_LinSM_00182> */
        /* If active state is LINSM_UNINIT the state
         * COMM_NO_COMMUNICATION shall be returned. */
        *mode = COMM_NO_COMMUNICATION;
    }
    else
#endif /* STD_ON == LINSM_DEV_ERROR_DETECT */
    {
        /* get channel by network */
        uint8 chl = LinSM_GetChannelByNetworkHandle(network);
#if (STD_ON == LINSM_DEV_ERROR_DETECT)
/* Multiple partition check. */
#if (STD_ON == LINSM_MULTIPLE_PARTITION_USED)
        ApplicationType applicationID = GetApplicationID();
#endif
        /*@req <SWS_LinSM_00123> */
        /* the network parameter has an invalid value, report error */
        if (LINSM_CHANNEL_NUM == chl)
        {
            (void)Det_ReportError(
                LINSM_MODULE_ID,
                LINSM_INSTANCE_ID,
                LINSM_SERVICE_ID_GET_CURRENTCOMMODE,
                LINSM_E_NONEXISTENT_NETWORK);
        }
        /*@req <SWS_LinSM_00124> */
        /* the mode pointer parameter is invalid, report error */
        else if (NULL_PTR == mode)
        {
            (void)Det_ReportError(
                LINSM_MODULE_ID,
                LINSM_INSTANCE_ID,
                LINSM_SERVICE_ID_GET_CURRENTCOMMODE,
                LINSM_E_PARAM_POINTER);
        }
        else
#if (STD_ON == LINSM_MULTIPLE_PARTITION_USED)
            /* Multiple partition check. */
            if (applicationID != ComM_GetChannelApplicationID(network))
            {
                (void)Det_ReportError(
                    LINSM_MODULE_ID,
                    LINSM_INSTANCE_ID,
                    LINSM_SERVICE_ID_GET_CURRENTCOMMODE,
                    LINSM_E_PARTITION);
            }
            else
#endif
#endif
            {
                const LinSM_RunTimeType* LinSM_RTPtr = &LinSM_RTArray[chl];
                /*@req <SWS_LinSM_00181> COMM_FULL_COMMUNICATION*/
                /*@req <SWS_LinSM_00180> COMM_NO_COMMUNICATION */
                *mode =
                    (LINSM_FULL_COM == LinSM_RTPtr->channelComMode) ? COMM_FULL_COMMUNICATION : COMM_NO_COMMUNICATION;
                ret = E_OK;
            }
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               Requesting of a communication mode.
 *                     The mode switch will not be made instant. The LinSM will
 *                     notify the caller when mode transition is made.
 * ServiceId           0x12
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      network: Identification of the LIN channel
 *                     mode:    Request mode
 * Param-Name[in/out]  N/A
 * Param-Name[out]     N/A
 * Return              E_OK - Request accepted
 *                     E_NOT_OK - Not possible to perform the request, e.g. not
 *                                initialized.
 */
/******************************************************************************/
FUNC(Std_ReturnType, LINSM_CODE)
LinSM_RequestComMode /* PRQA S 1532 */ /* MISRA Rule 8.7 */
    (NetworkHandleType network, ComM_ModeType mode)
{
    Std_ReturnType ret = E_OK;
    /*@req <SWS_LinSM_00183> */
    /* If COMM_SILENT_COMMUNICATION is requested the
     * function shall return E_NOT_OK directly without action */
    if (COMM_SILENT_COMMUNICATION == mode)
    {
        ret = E_NOT_OK;
    }
    else
    {
        /*@req <SWS_LinSM_00128> */
        /* the state LINSM_UNINIT is active ,report error */
#if (STD_ON == LINSM_DEV_ERROR_DETECT)
        if (LINSM_UNINIT == LinSM_ModuleStatus)
        {
            (void)Det_ReportError(LINSM_MODULE_ID, LINSM_INSTANCE_ID, LINSM_SERVICE_ID_REQUEST_COMMODE, LINSM_E_UNINIT);
            ret = E_NOT_OK;
        }
        else
#endif /* STD_ON == LINSM_DEV_ERROR_DETECT */
        {
            /* get channel by network */
            uint8 chl = LinSM_GetChannelByNetworkHandle(network);
#if (STD_ON == LINSM_DEV_ERROR_DETECT)
/* Multiple partition check. */
#if (STD_ON == LINSM_MULTIPLE_PARTITION_USED)
            ApplicationType applicationID = GetApplicationID();
#endif
            /*@req <SWS_LinSM_00127> */
            /* the network parameter has an invalid value, report error */
            if (LINSM_CHANNEL_NUM == chl)
            {
                (void)Det_ReportError(
                    LINSM_MODULE_ID,
                    LINSM_INSTANCE_ID,
                    LINSM_SERVICE_ID_REQUEST_COMMODE,
                    LINSM_E_NONEXISTENT_NETWORK);
                ret = E_NOT_OK;
            }
            /*@req <SWS_LinSM_00191> */
            /* the ComM_Mode parameter has an invalid value, report error */
            else if ((COMM_FULL_COMMUNICATION != mode) && (COMM_NO_COMMUNICATION != mode))
            {
                (void)Det_ReportError(
                    LINSM_MODULE_ID,
                    LINSM_INSTANCE_ID,
                    LINSM_SERVICE_ID_REQUEST_COMMODE,
                    LINSM_E_PARAMETER);
                ret = E_NOT_OK;
            }
            else
#if (STD_ON == LINSM_MULTIPLE_PARTITION_USED)
                /* Multiple partition check. */
                if (applicationID != ComM_GetChannelApplicationID(network))
                {
                    (void)Det_ReportError(
                        LINSM_MODULE_ID,
                        LINSM_INSTANCE_ID,
                        LINSM_SERVICE_ID_REQUEST_COMMODE,
                        LINSM_E_PARTITION);
                }
                else
#endif
#endif
                {
                    LinSM_RunTimeType* LinSM_RTPtr = &LinSM_RTArray[chl];

                    /*@req <SWS_LinSM_00047>,<SWS_LinSM_00178>,<SWS_LinSM_00237>*/
                    if ((COMM_FULL_COMMUNICATION == mode)
                        && ((LINSM_MASTER == LINSM_NODE_TYPE(chl))
                            || !(
                                (LINSM_SILENCEWAKEUP_TIMER == LinSM_RTPtr->timerType) && (LinSM_RTPtr->timerCnt > 0u))))
                    {

                        ret = LinIf_Wakeup(network);

                        /*@req <SWS_LinSM_00176> */
                        if (E_OK == ret)
                        {
                            SchM_Enter_LinSM_Context();
                            /*@req <SWS_LinSM_00103> */
                            if (0u != LINSM_CONFIRMATION_TIMEOUT(chl))
                            {
                                /*@req <SWS_LinSM_00100> */
                                /* start timer */
                                LinSM_RTPtr->timerCnt = LINSM_CONFIRMATION_TIMEOUT(chl);
                                LinSM_RTPtr->timerType = LINSM_WAKEUP_TIMER;
                                /* Set max retry times */
                                LinSM_RTPtr->wakeupRetryCnt = LINSM_MODE_REQUEST_REPETITION_MAX();
                            }
                            /* Information bakeup */
                            LinSM_RTPtr->runningReq = LINSM_EXE_WAKE_UP;
                            LinSM_RTPtr->req2LinIfResult = ret;
                            SchM_Exit_LinSM_Context();
                        }
                    }

                    /*@req <SWS_LinSM_00035>,<SWS_LinSM_10208>,<SWS_LinSM_10209>*/
                    if ((COMM_NO_COMMUNICATION == mode) && (LINSM_FULL_COM == LinSM_RTPtr->channelComMode)
                        && (LINSM_RUN_COMMUNICATION == LinSM_RTPtr->fullComSubStatus))
                    {
                        if (LINSM_MASTER == LINSM_NODE_TYPE(chl))
                        {
                            /*@req <SWS_LinSM_00036>,<SWS_LinSM_00177>*/
                            ret = LinIf_GotoSleep(network);

                            SchM_Enter_LinSM_Context();
                            if (E_OK == ret)
                            {
                                /*@req <SWS_LinSM_00302> */
                                LinSM_RTPtr->fullComSubStatus = LINSM_GOTO_SLEEP;
                            }
                            /* Information bakeup */
                            LinSM_RTPtr->runningReq = LINSM_EXE_GOTO_SLEEP;
                            LinSM_RTPtr->req2LinIfResult = ret;
                            SchM_Exit_LinSM_Context();
                        }
                        else
                        {
                            /*@req <SWS_LinSM_00230> */
                            ret = E_OK;
                        }
                    }

                    /*@req <SWS_LinSM_00230>,<SWS_LinSM_00236>,<SWS_LinSM_00237> */
                    LinSM_RTPtr->reqComMode = mode;     /* PRQA S 2842 */
                    LinSM_RTPtr->reqComModeFlag = TRUE; /* PRQA S 2842 */
                }
        }
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               Periodic function that runs the timers of different
 *                     request timeouts
 * ServiceId           0x30
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[in/out]  N/A
 * Param-Name[out]     N/A
 * Return              N/A
 */
/******************************************************************************/
FUNC(void, LINSM_CODE)
LinSM_MainFunction(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    LinSM_RunTimeType* LinSM_RTPtr;
    uint8 chl;

    if (LINSM_INIT == LinSM_ModuleStatus)
    {
#if (STD_ON == LINSM_MULTIPLE_PARTITION_USED)
        /* Multiple partition check. */
        ApplicationType applicationID = GetApplicationID();
#endif
        for (chl = 0u; chl < LINSM_CHANNEL_NUM; ++chl) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
        {
            LinSM_RTPtr = &LinSM_RTArray[chl];
#if (STD_ON == LINSM_MULTIPLE_PARTITION_USED)
            /* Multiple partition check. */
            if (applicationID != ComM_GetChannelApplicationID(LINSM_NETWORK_HANDLE(chl)))
            {
                continue;
            }
#endif
            switch (LinSM_RTPtr->runningReq)
            {
            case LINSM_EXE_GOTO_SLEEP:
                LinSM_HandleGotoSleepPeriod(chl);
                break;

            case LINSM_EXE_WAKE_UP:
                LinSM_HandleWakeupPeriod(chl);
                break;

            case LINSM_EXE_CHANGE_SCHEDULE:
                LinSM_HandleChangeSchedulePeriod(chl);
                break;

            case LINSM_EXE_NOTHING:
                break;

            default:
                SchM_Enter_LinSM_Context();
                /* RAM abnormal countermeasure */
                LinSM_RTPtr->runningReq = LINSM_EXE_NOTHING;
                SchM_Exit_LinSM_Context();
                break;
            }
        }
    }
}

#define LINSM_STOP_SEC_CODE
#include "LinSM_MemMap.h"

#if (LINSM_MASTER_NODE_SUPPORT == STD_ON)
#define LINSM_START_SEC_LINSMSCHEDULEREQUESTCONFIRMATION_CALLBACK_CODE
#include "LinSM_MemMap.h"
/******************************************************************************/
/*
 * Brief               The LinIf module will call this callback when the new
 *                     requested schedule table is active.
 * ServiceId           0x20
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      network:  Identification of the LIN channel
 *                     schedule: Pointer to the new active Schedule table
 * Param-Name[in/out]  N/A
 * Param-Name[out]     N/A
 * Return              N/A
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
FUNC(void, LINSM_CODE)
LinSM_ScheduleRequestConfirmation(NetworkHandleType network, LinIf_SchHandleType schedule)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    /*@req <SWS_LinSM_00131> */
    /* the state LINSM_UNINIT is active ,report error */
#if (STD_ON == LINSM_DEV_ERROR_DETECT)
    if (LINSM_UNINIT == LinSM_ModuleStatus)
    {
        (void)Det_ReportError(
            LINSM_MODULE_ID,
            LINSM_INSTANCE_ID,
            LINSM_SERVICE_ID_SCHEDULEREQUEST_CONFIRMATION,
            LINSM_E_UNINIT);
    }
    else
#endif /* STD_ON == LINSM_DEV_ERROR_DETECT */
    {
        /* get channel by network */
        uint8 chl = LinSM_GetChannelByNetworkHandle(network);
#if (STD_ON == LINSM_DEV_ERROR_DETECT)
        /*@req <SWS_LinSM_00130> */
        /* the network parameter has an invalid value, report error */
        if (LINSM_CHANNEL_NUM == chl)
        {
            (void)Det_ReportError(
                LINSM_MODULE_ID,
                LINSM_INSTANCE_ID,
                LINSM_SERVICE_ID_SCHEDULEREQUEST_CONFIRMATION,
                LINSM_E_NONEXISTENT_NETWORK);
        }
        else
#endif
        {
            LinSM_RunTimeType* LinSM_RTPtr = &LinSM_RTArray[chl];
            if (LINSM_EXE_CHANGE_SCHEDULE == LinSM_RTPtr->runningReq)
            {
                SchM_Enter_LinSM_Context();
                /*@req <SWS_LinSM_00154> */
                /* stop timer */
                LinSM_RTPtr->timerCnt = 0u;
                LinSM_RTPtr->runningReq = LINSM_EXE_NOTHING;
                LinSM_RTPtr->reqComModeFlag = FALSE;
                LinSM_RTPtr->timerType = LINSM_NONE_TIMER;
                SchM_Exit_LinSM_Context();
            }
            /*@req <SWS_LinSM_00206>,<SWS_LinSM_00207>*/
            BswM_LinSM_CurrentSchedule(network, schedule);

            SchM_Enter_LinSM_Context();
            LinSM_RTPtr->curSchedule = schedule;
            SchM_Exit_LinSM_Context();
        }
    }
}
#define LINSM_STOP_SEC_LINSMSCHEDULEREQUESTCONFIRMATION_CALLBACK_CODE
#include "LinSM_MemMap.h"
#endif /* LINSM_MASTER_NODE_SUPPORT == STD_ON */

#if (LINSM_SLAVE_NODE_SUPPORT == STD_ON)
#define LINSM_START_SEC_LINSMGOTOSLEEPINDICATION_CALLBACK_CODE
#include "LinSM_MemMap.h"
/******************************************************************************/
/*
 * Brief               The LinIf will call this callback when the go to sleep
 *                     command is received on the network or a bus idle timeout
 *                     occurs.
 * ServiceId           0x03
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      network: Identification of the LIN channel
 * Param-Name[in/out]  N/A
 * Param-Name[out]     N/A
 * Return              N/A
 */
/******************************************************************************/
FUNC(void, LINSM_CODE)
LinSM_GotoSleepIndication(NetworkHandleType network) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    /*@req <SWS_LinSM_00240> */
    /* the state LINSM_UNINIT is active ,report error */
#if (STD_ON == LINSM_DEV_ERROR_DETECT)
    if (LINSM_UNINIT == LinSM_ModuleStatus)
    {
        (void)
            Det_ReportError(LINSM_MODULE_ID, LINSM_INSTANCE_ID, LINSM_SERVICE_ID_GOTOSLEEP_INDICATION, LINSM_E_UNINIT);
    }
    else
#endif /* STD_ON == LINSM_DEV_ERROR_DETECT */
    {
        /* get channel by network */
        uint8 chl = LinSM_GetChannelByNetworkHandle(network);
#if (STD_ON == LINSM_DEV_ERROR_DETECT)
        /*@req <SWS_LinSM_00239> */
        /* the network parameter has an invalid value, report error */
        if (LINSM_CHANNEL_NUM == chl)
        {
            (void)Det_ReportError(
                LINSM_MODULE_ID,
                LINSM_INSTANCE_ID,
                LINSM_SERVICE_ID_GOTOSLEEP_INDICATION,
                LINSM_E_NONEXISTENT_NETWORK);
        }
        else
#endif
        {

            LinSM_RunTimeType* LinSM_RTPtr = &LinSM_RTArray[chl];

            if (LINSM_FULL_COM == LinSM_RTPtr->channelComMode)
            {
                /*@req <SWS_LinSM_00231>,<SWS_LinSM_00232>*/
                Std_ReturnType ret = LinIf_GotoSleep(network);

                if (E_OK == ret)
                {
                    /*@req <SWS_LinSM_00233>*/
                    if (COMM_NO_COMMUNICATION == LinSM_RTPtr->reqComMode)
                    {
                        ComM_BusSM_BusSleepMode(network);
                    }
                    LinSM_RTPtr->reqComModeFlag = FALSE;
                    /*@req <SWS_LinSM_00302> */
                    LinSM_RTPtr->fullComSubStatus = LINSM_GOTO_SLEEP;
                    /* Information bakeup */
                    LinSM_RTPtr->runningReq = LINSM_EXE_GOTO_SLEEP;
                    LinSM_RTPtr->req2LinIfResult = ret;
                }
            }
        }
    }
}
#define LINSM_STOP_SEC_LINSMGOTOSLEEPINDICATION_CALLBACK_CODE
#include "LinSM_MemMap.h"
#endif /* LINSM_SLAVE_NODE_SUPPORT == STD_ON */

#define LINSM_START_SEC_LINSMGOTOSLEEPCONFIRMATION_CALLBACK_CODE
#include "LinSM_MemMap.h"
/******************************************************************************/
/*
 * Brief               The LinIf will call this callback when the go to sleep
 *                     command is sent successfully or not sent successfully
 *                     on the network.
 * ServiceId           0x22
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      network: Identification of the LIN channel
 *                     success: True if goto sleep was successfully sent,
 *                              false otherwise
 * Param-Name[in/out]  N/A
 * Param-Name[out]     N/A
 * Return              N/A
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
FUNC(void, LINSM_CODE)
LinSM_GotoSleepConfirmation(NetworkHandleType network, boolean success)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    /*@req <SWS_LinSM_00137> */
    /* the state LINSM_UNINIT is active ,report error */
#if (STD_ON == LINSM_DEV_ERROR_DETECT)
    if (LINSM_UNINIT == LinSM_ModuleStatus)
    {
        (void)Det_ReportError(
            LINSM_MODULE_ID,
            LINSM_INSTANCE_ID,
            LINSM_SERVICE_ID_GOTOSLEEP_CONFIRMATION,
            LINSM_E_UNINIT);
    }
    else
#endif /* STD_ON == LINSM_DEV_ERROR_DETECT */
    {
        uint8 chl = LinSM_GetChannelByNetworkHandle(network);
#if (STD_ON == LINSM_DEV_ERROR_DETECT)
        /*@req <SWS_LinSM_00136> */
        /* the network parameter has an invalid value, report error */
        if (LINSM_CHANNEL_NUM == chl)
        {
            (void)Det_ReportError(
                LINSM_MODULE_ID,
                LINSM_INSTANCE_ID,
                LINSM_SERVICE_ID_GOTOSLEEP_CONFIRMATION,
                LINSM_E_NONEXISTENT_NETWORK);
        }
        else
#endif
        {
            /* get channel by network */
            LinSM_RunTimeType* LinSM_RTPtr = &LinSM_RTArray[chl];

            if (LINSM_EXE_GOTO_SLEEP == LinSM_RTPtr->runningReq)
            {
                if (COMM_NO_COMMUNICATION == LinSM_RTPtr->reqComMode)
                {
                    SchM_Enter_LinSM_Context();
                    /*@req <SWS_LinSM_00154> */
                    /* stop Go-To-Sleep timer */
                    LinSM_RTPtr->timerCnt = 0u;
                    LinSM_RTPtr->timerType = LINSM_NONE_TIMER;
                    LinSM_RTPtr->runningReq = LINSM_EXE_NOTHING;
                    LinSM_RTPtr->reqComModeFlag = FALSE;
                    /* state back to LINSM_RUN_COMMUNICATION(for fail safe purpose)*/
                    /* if success = FALSE,new gotosleep request can be handled */
                    LinSM_RTPtr->fullComSubStatus = LINSM_RUN_COMMUNICATION;
                    /*@req <SWS_LinSM_00046> */
                    /* Note: if not timeout,current state is LINSM_GOTO_SLEEP,
                     * needn't duplicate check */
                    LinSM_RTPtr->channelComMode = LINSM_NO_COM;
                    SchM_Exit_LinSM_Context();

                    /*@req <SWS_LinSM_00027> */
                    ComM_BusSM_ModeIndication(network, COMM_NO_COMMUNICATION);
                    /*@req <SWS_LinSM_00193> */
                    BswM_LinSM_CurrentState(network, LINSM_NO_COM);
/*@req <SWS_LinSM_00203> */
#if (STD_ON == LINSM_TRANSCEIVER_PASSIVER_MODE_SUPPORT)
                    if (LINSM_IS_TRANSCEIVER_PASSIVER_MODE_SUPPORT(chl))
                    {
                        LinTrcv_TrcvModeType LinTrcvModeType = LINSM_GET_TRANSCEIVER_PASSIVER_MODE(chl)
                                                                   ? LINTRCV_TRCV_MODE_STANDBY
                                                                   : LINTRCV_TRCV_MODE_SLEEP;
                        (void)LinIf_SetTrcvMode(network, LinTrcvModeType);
                    }
#endif
                }
                else
                {
                    /*@req <SWS_LinSM_00234> */
                    /* Restart Wakeup */
                    Std_ReturnType ret = LinIf_Wakeup(network);

                    /*@req <SWS_LinSM_00176> */
                    if (E_OK == ret)
                    {
                        SchM_Enter_LinSM_Context();
                        /*@req <SWS_LinSM_00103> */
                        if (0u != LINSM_CONFIRMATION_TIMEOUT(chl))
                        {
                            /*@req <SWS_LinSM_00100> */
                            /* start timer */
                            LinSM_RTPtr->timerCnt = LINSM_CONFIRMATION_TIMEOUT(chl);
                            LinSM_RTPtr->timerType = LINSM_WAKEUP_TIMER;
                            /* Set max retry times */
                            LinSM_RTPtr->wakeupRetryCnt = LINSM_MODE_REQUEST_REPETITION_MAX();
                        }
                        /* Information bakeup */
                        LinSM_RTPtr->runningReq = LINSM_EXE_WAKE_UP;
                        LinSM_RTPtr->req2LinIfResult = ret;
                        SchM_Exit_LinSM_Context();
                    }
                }
            }
        }
    }

    LINSM_UNUSED(success);
}
#define LINSM_STOP_SEC_LINSMGOTOSLEEPCONFIRMATION_CALLBACK_CODE
#include "LinSM_MemMap.h"

#define LINSM_START_SEC_LINSMWAKEUPCONFIRMATION_CALLBACK_CODE
#include "LinSM_MemMap.h"
/******************************************************************************/
/*
 * Brief               The LinIf will call this callback when the wake up signal
 *                     command is sent not successfully/successfully on the
 *                     network.
 * ServiceId           0x21
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      network: Identification of the LIN channel
 *                     success: True if wakeup was successfully sent,
 *                              false otherwise
 * Param-Name[in/out]  N/A
 * Param-Name[out]     N/A
 * Return              N/A
 */
/******************************************************************************/
FUNC(void, LINSM_CODE)
LinSM_WakeupConfirmation(NetworkHandleType network, boolean success) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    /*@req <SWS_LinSM_00134> */
    /* the state LINSM_UNINIT is active ,report error */
#if (STD_ON == LINSM_DEV_ERROR_DETECT)
    if (LINSM_UNINIT == LinSM_ModuleStatus)
    {
        (void)Det_ReportError(LINSM_MODULE_ID, LINSM_INSTANCE_ID, LINSM_SERVICE_ID_WAKEUP_CONFIRMATION, LINSM_E_UNINIT);
    }
    else
#endif /* STD_ON == LINSM_DEV_ERROR_DETECT */
    {
        uint8 chl = LinSM_GetChannelByNetworkHandle(network);
#if (STD_ON == LINSM_DEV_ERROR_DETECT)
        /*@req <SWS_LinSM_00133> */
        /* the network parameter has an invalid value, report error */
        if (LINSM_CHANNEL_NUM == chl)
        {
            (void)Det_ReportError(
                LINSM_MODULE_ID,
                LINSM_INSTANCE_ID,
                LINSM_SERVICE_ID_WAKEUP_CONFIRMATION,
                LINSM_E_NONEXISTENT_NETWORK);
        }
        else
#endif
        {
            /* get channel by network */
            LinSM_RunTimeType* LinSM_RTPtr = &LinSM_RTArray[chl];

            if (LINSM_EXE_WAKE_UP == LinSM_RTPtr->runningReq)
            {
                SchM_Enter_LinSM_Context();
                /*@req <SWS_LinSM_00154> */
                /* stop timer */
                LinSM_RTPtr->timerCnt = 0u;
                LinSM_RTPtr->timerType = LINSM_NONE_TIMER;
                LinSM_RTPtr->runningReq = LINSM_EXE_NOTHING;
                SchM_Exit_LinSM_Context();

                if (success)
                {
                    SchM_Enter_LinSM_Context();
                    /*@req <SWS_LinSM_00049> */
                    LinSM_RTPtr->channelComMode = LINSM_FULL_COM;
                    /*@req <SWS_LinSM_00301> */
                    LinSM_RTPtr->fullComSubStatus = LINSM_RUN_COMMUNICATION;
                    SchM_Exit_LinSM_Context();

                    /*@req <SWS_LinSM_00033> */
                    ComM_BusSM_ModeIndication(network, COMM_FULL_COMMUNICATION);
                    /*@req <SWS_LinSM_00192> */
                    BswM_LinSM_CurrentState(network, LINSM_FULL_COM);
/*@req <SWS_LinSM_00205> */
#if (STD_ON == LINSM_TRANSCEIVER_PASSIVER_MODE_SUPPORT)
                    if (LINSM_IS_TRANSCEIVER_PASSIVER_MODE_SUPPORT(chl))
                    {
                        (void)LinIf_SetTrcvMode(network, LINTRCV_TRCV_MODE_NORMAL);
                    }
#endif
                }
                else
                {
                    /*@req <SWS_LinSM_00202> */
                    ComM_BusSM_ModeIndication(network, COMM_NO_COMMUNICATION);
                    BswM_LinSM_CurrentState(network, LINSM_NO_COM);
                }
            }
        }
    }
}
#define LINSM_STOP_SEC_LINSMWAKEUPCONFIRMATION_CALLBACK_CODE
#include "LinSM_MemMap.h"

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#define LINSM_START_SEC_CODE
#include "LinSM_MemMap.h"

/******************************************************************************/
/*
 * Brief               Get LinSM channel id coresponding to input network
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      network: ComM channel id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              LinSM channel id
 */
/******************************************************************************/
static FUNC(uint8, LINSM_CODE) LinSM_GetChannelByNetworkHandle(NetworkHandleType network)
{
    uint8 loop;

    /* loop for every channel, search corresponding network */
    for (loop = 0u; loop < LINSM_CHANNEL_NUM; ++loop) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
    {
        if (LINSM_NETWORK_HANDLE(loop) == network)
        {
            break;
        }
    }

    return loop;
}

#if ((STD_ON == LINSM_DEV_ERROR_DETECT) && (LINSM_MASTER_NODE_SUPPORT == STD_ON))
/******************************************************************************/
/*
 * Brief               Check input 'schedule' id is valid
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinSM channel id
 *                     schedule: Schedule table id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              TRUE: 'schedule' id is valid
 *                     FALSE:'schedule' id is invalid
 */
/******************************************************************************/
static FUNC(boolean, LINSM_CODE) LinSM_CheckSchedule(uint8 ch, LinIf_SchHandleType schedule)
{
    uint8 loop;

    /*  search corresponding schedule */
    for (loop = 0u; loop < LINSM_CHANNEL_SCHEDULE_NUM(ch); ++loop)
    {
        if (LINSM_CHANNEL_SCHEDULE_INDEX(ch, loop) == schedule)
        {
            break;
        }
    }

    return loop < LINSM_CHANNEL_SCHEDULE_NUM(ch);
}
#endif /* STD_ON == LINSM_DEV_ERROR_DETECT */

/******************************************************************************/
/*
 * Brief               GotoSleep period process in mainfunction
 * Sync/Async          Synchronous
 * Reentrancy          None Reentrant
 * Param-Name[in]      chl: LinSM channel id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, LINSM_CODE) LinSM_HandleGotoSleepPeriod(uint8 chl)
{
    SchM_Enter_LinSM_Context();
    LinSM_RunTimeType* LinSM_RTPtr = &LinSM_RTArray[chl];
    if (E_NOT_OK == LinSM_RTPtr->req2LinIfResult)
    {
        /*@req <SWS_LinSM_00177> */
        ComM_BusSM_ModeIndication(LINSM_NETWORK_HANDLE(chl), COMM_FULL_COMMUNICATION);
        BswM_LinSM_CurrentState(LINSM_NETWORK_HANDLE(chl), LINSM_FULL_COM);
        /* clear running flag */
        LinSM_RTPtr->runningReq = LINSM_EXE_NOTHING;
        LinSM_RTPtr->timerType = LINSM_NONE_TIMER;
    }
    SchM_Exit_LinSM_Context();
}

/******************************************************************************/
/*
 * Brief               Wakeup period process in mainfunction
 * Sync/Async          Synchronous
 * Reentrancy          None Reentrant
 * Param-Name[in]      chl: LinSM channel id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, LINSM_CODE) LinSM_HandleWakeupPeriod(uint8 chl)
{

    SchM_Enter_LinSM_Context();
    LinSM_RunTimeType* LinSM_RTPtr = &LinSM_RTArray[chl];

    if (0u != LinSM_RTPtr->timerCnt)
    {
        /*@req <SWS_LinSM_000157> */
        /*decrease the timer counter*/
        LinSM_RTPtr->timerCnt--;
        /* timeout */
        if (0u == LinSM_RTPtr->timerCnt)
        {
            Std_ReturnType ret;

            if (LINSM_WAKEUP_TIMER == LinSM_RTPtr->timerType)
            {
                /*@req <SWS_LinSM_00102> */
                /* when a timeout occurs, report runtime error to DET */
                (void)Det_ReportRuntimeError(
                    LINSM_MODULE_ID,
                    LINSM_INSTANCE_ID,
                    LINSM_SERVICE_ID_MAIN_FUNCTION,
                    LINSM_E_CONFIRMATION_TIMEOUT);

                /* the maximum number of retries have not been reached */
                if (0u != LinSM_RTPtr->wakeupRetryCnt)
                {
                    /*@req <SWS_LinSM_00304> */
                    ret = LinIf_Wakeup(LINSM_NETWORK_HANDLE(chl));

                    if (E_OK == ret)
                    {
                        /* restart timer */
                        LinSM_RTPtr->timerCnt = LINSM_CONFIRMATION_TIMEOUT(chl);
                        LinSM_RTPtr->timerType = LINSM_WAKEUP_TIMER;
                        /* decrease the retry counter */
                        LinSM_RTPtr->wakeupRetryCnt--;
                    }
                    else
                    {
                        LinSM_RTPtr->runningReq = LINSM_EXE_NOTHING;
                        LinSM_RTPtr->timerType = LINSM_NONE_TIMER;
                    }
                }
                else
                {
                    /*@req <SWS_LinSM_00170>,<SWS_LinSM_00305> */
                    ComM_BusSM_ModeIndication(LINSM_NETWORK_HANDLE(chl), COMM_NO_COMMUNICATION);
                    /*@req <SWS_LinSM_00215>,<SWS_LinSM_00305> */
                    BswM_LinSM_CurrentState(LINSM_NETWORK_HANDLE(chl), LINSM_NO_COM);

                    /*@req <SWS_LinSM_00235> */
                    if (LINSM_SLAVE == LINSM_NODE_TYPE(chl))
                    {
                        /* Start SlienceAfterWakeupTimer */
                        LinSM_RTPtr->timerCnt = LINSM_SILENCEAFTERWAKEUP_TIMEOUT(chl);
                        LinSM_RTPtr->timerType = LINSM_SILENCEWAKEUP_TIMER;
                    }
                    else
                    {
                        LinSM_RTPtr->runningReq = LINSM_EXE_NOTHING;
                    }
                }
            }
            else
            {
                /* SlienceAfterWakeupTimer timeout */
                if (LinSM_RTPtr->reqComModeFlag && (COMM_FULL_COMMUNICATION == LinSM_RTPtr->reqComMode))
                {
                    /*@req <SWS_LinSM_00236>,<SWS_LinSM_00237> */
                    ret = LinIf_Wakeup(LINSM_NETWORK_HANDLE(chl));

                    if (E_OK == ret)
                    {
                        /* restart timer */
                        LinSM_RTPtr->timerCnt = LINSM_CONFIRMATION_TIMEOUT(chl);
                        LinSM_RTPtr->timerType = LINSM_WAKEUP_TIMER;
                        /* Set max retry times */
                        LinSM_RTPtr->wakeupRetryCnt = LINSM_MODE_REQUEST_REPETITION_MAX();
                    }
                    else
                    {
                        LinSM_RTPtr->runningReq = LINSM_EXE_NOTHING;
                        LinSM_RTPtr->timerType = LINSM_NONE_TIMER;
                    }
                }
            }
        }
    }
    SchM_Exit_LinSM_Context();
}

/******************************************************************************/
/*
 * Brief               Change schedule table period process in mainfunction
 * Sync/Async          Synchronous
 * Reentrancy          None Reentrant
 * Param-Name[in]      chl: LinSM channel id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, LINSM_CODE) LinSM_HandleChangeSchedulePeriod(uint8 chl)
{
    SchM_Enter_LinSM_Context();
    LinSM_RunTimeType* LinSM_RTPtr = &LinSM_RTArray[chl];
    if (E_NOT_OK == LinSM_RTPtr->req2LinIfResult)
    {
        /*@req <SWS_LinSM_00213> */
        BswM_LinSM_CurrentSchedule(LINSM_NETWORK_HANDLE(chl), LinSM_RTPtr->curSchedule);
        LinSM_RTPtr->runningReq = LINSM_EXE_NOTHING;
        LinSM_RTPtr->timerType = LINSM_NONE_TIMER;
    }
    else
    {
        if (0u != LinSM_RTPtr->timerCnt)
        {
            /*@req <SWS_LinSM_00157> */
            /*decrease the timer counter*/
            LinSM_RTPtr->timerCnt--;
            /* timeout */
            if (0u == LinSM_RTPtr->timerCnt)
            {
                /*@req <SWS_LinSM_00102> */
                /* when a timeout occurs, report runtime error to DET */
                (void)Det_ReportRuntimeError(
                    LINSM_MODULE_ID,
                    LINSM_INSTANCE_ID,
                    LINSM_SERVICE_ID_MAIN_FUNCTION,
                    LINSM_E_CONFIRMATION_TIMEOUT);

                /*@req <SWS_LinSM_00214> */
                BswM_LinSM_CurrentSchedule(LINSM_NETWORK_HANDLE(chl), LinSM_RTPtr->curSchedule);
                /* Runtime info setup */
                LinSM_RTPtr->runningReq = LINSM_EXE_NOTHING;
            }
        }
    }
    SchM_Exit_LinSM_Context();
}

#define LINSM_STOP_SEC_CODE
#include "LinSM_MemMap.h"
