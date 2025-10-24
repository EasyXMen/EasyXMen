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
 **  @file               : LinSM.c
 **  @author             : HuRongbo
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Implementation for LinSM
 **
 ***********************************************************************************************************************/
/* PRQA S 2814,2824 ++ */  /* VL_LinSM_DerefNullPtr */
/* PRQA S 1503, 1532 ++ */ /* VL_QAC_NoUsedApi, VL_QAC_OneFunRef */
/* =================================================== inclusions =================================================== */
#include "LinSM.h"
/** SWS_LinSM_00201 */
#include "BswM_LinSM.h"
/** SWS_LinSM_00305 */
#include "Lin_GeneralTypes.h"
#include "SchM_LinSM.h"
#if LINSM_DEV_ERROR_DETECT == STD_ON
#include "Det.h"
#endif

/* ===================================================== macros ===================================================== */
#define LINSM_NETWORK_HANDLE(ch)             LinSM_CfgPtr->ChannelPtr[ch].ComMNetworkHandleRef
#define LINSM_CONFIRMATION_TIMEOUT(ch)       LinSM_CfgPtr->ChannelPtr[ch].ConfirmationTimeout
#define LINSM_MODE_REQUEST_REPETITION_MAX()  LinSM_CfgPtr->ModeRequestRepetitionMax
#define LINSM_NODE_TYPE(ch)                  LinSM_CfgPtr->ChannelPtr[ch].NodeType
#define LINSM_SILENCEAFTERWAKEUP_TIMEOUT(ch) LinSM_CfgPtr->ChannelPtr[ch].SilenceAfterWakeupTimeout
#define LINSM_RUNTIME_PTR(ch)                LinSM_CfgPtr->ChannelPtr[ch].RuntimePtr

#if LINSM_MULTIPLE_PARTITION_USED == STD_ON
#define LINSM_PARTITION_ID(ch) LinSM_CfgPtr->ChannelPtr[ch].ApplicationID
#endif

#if LINSM_TRANSCEIVER_PASSIVER_MODE_SUPPORT == STD_ON
#define LINSM_IS_TRANSCEIVER_PASSIVER_MODE_SUPPORT(ch) LinSM_CfgPtr->ChannelPtr[ch].IsTransceiverPassiveModeSupport
#define LINSM_GET_TRANSCEIVER_PASSIVER_MODE(ch)        LinSM_CfgPtr->ChannelPtr[ch].TransceiverPassiveMode
#endif

#if LINSM_DEV_ERROR_DETECT == STD_ON
#define LINSM_CHANNEL_SCHEDULE_INDEX(ch) LinSM_CfgPtr->ChannelPtr[ch].ScheduleIndex
#define LINSM_CHANNEL_SCHEDULE_NUM(ch)   LinSM_CfgPtr->ChannelPtr[ch].ScheduleNum
#endif

#define LINSM_UNUSED(x) (void)(x)

#if !defined(LINSM_LOCAL)
#ifdef GLOBAL_LINSM_EXTERN
#define LINSM_LOCAL
#else
#define LINSM_LOCAL static /* PRQA S 3414 */ /* VL_QAC_KeyWord */
#endif
#endif

#if !defined(LINSM_LOCAL_INLINE)
#define LINSM_LOCAL_INLINE static inline
#endif
/* ================================================ type definitions ================================================ */
/* Current status of LinSM Module */
/** SWS_LinSM_00020 */
typedef enum
{
    /** SWS_LinSM_00022 */
    LINSM_UNINIT,
    /** SWS_LinSM_00024 */
    LINSM_INIT
} LinSM_ModuleStatusType;

/* ========================================== internal function declarations ======================================== */
#define LINSM_START_SEC_CODE
#include "LinSM_MemMap.h"

/**
 * @brief       Function to handle the wake-up timer periodically
 * @param[in]   chlIdx : Index of the LIN channel.
 * @reentrant   TRUE (Reentrant for different channel. Non reentrant for the same channel.)
 * @synchronous TRUE
 * @trace       CPD-59551
 */
LINSM_LOCAL void LinSM_HandleWakeupPeriod(LinSM_ChannelIndexType chlIdx);
/**
 * @brief       Function to handle the silent timers periodically
 * @param[in]   chlIdx : Index of the LIN channel.
 * @reentrant   TRUE (Reentrant for different channel. Non reentrant for the same channel.)
 * @synchronous TRUE
 * @trace       CPD-66106
 */
LINSM_LOCAL void LinSM_HandleSilencePeriod(LinSM_ChannelIndexType chlIdx);

/**
 * @brief       The trigger channel starts the wake-up process.
 * @param[in]   chlIdx   : Index of the LIN channel.
 * @param[in]   oneShot  : True if trigger failure also starts the wakeup process, false otherwise
 * @return      Std_ReturnType
 * @retval      E_OK     : wake-up process started successfully
 * @retval      E_NOT_OK : wake-up process failed to start
 * @reentrant   Reentrant for different Channels
 * @synchronous TRUE
 * @trace       CPD-66104
 */
LINSM_LOCAL Std_ReturnType LinSM_TriggerFullCom(LinSM_ChannelIndexType chlIdx, boolean oneShot);
/**
 * @brief       The trigger channel starts the go-to-sleep process.
 * @param[in]   chlIdx   : Index of the LIN channel.
 * @return      Std_ReturnType
 * @retval      E_OK     : go-to-sleep process started successfully
 * @retval      E_NOT_OK : go-to-sleep process failed to start
 * @reentrant   Reentrant for different Channels
 * @synchronous TRUE
 * @trace       CPD-66105
 */
LINSM_LOCAL Std_ReturnType LinSM_TriggerGotoSleep(LinSM_ChannelIndexType chlIdx);

/**
 * @brief       Find the index of the channel in the LinSM according to ComMNetwork.
 * @param[in]   network   : Index of the ComM network.
 * @return      LinSM_ChannelIndexType
 * @retval      Index of the LIN channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-59657
 */
LINSM_LOCAL LinSM_ChannelIndexType LinSM_GetChannelByNetworkHandle(NetworkHandleType network);
/**
 * @brief       Find the index of the partition in the Linsm involved partition according to ApplicationID.
 * @return      uint16
 * @retval      Index of the partition configured by LIN , 0 if no multiple partitions are involved
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-66107
 */
LINSM_LOCAL uint16 LinSM_GetPartitionIndex(void);

#if LINSM_DEV_ERROR_DETECT == STD_ON
/**
 * @brief       Check that the current running partition is valid.
 * @param[in]   apiId   : The ID of the API to use when calling this function.
 * @return      boolean
 * @retval      TRUE if the currently running partition is valid, FALSE otherwise
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-66108
 */
LINSM_LOCAL boolean LinSM_ValidatePartitionContext(uint8 apiId);
/**
 * @brief       Checks if the current initialization state is valid
 * @param[in]   apiId   : The ID of the API to use when calling this function.
 * @return      boolean
 * @retval      TRUE if the current initialization state is valid, FALSE otherwise
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-66109
 */
LINSM_LOCAL boolean LinSM_ValidateInitStatus(uint8 apiId);
/**
 * @brief       Check that the current channel of ComMNetwork is valid
 * @param[in]   apiId   : The ID of the API to use when calling this function.
 * @param[in]   network : Index of the ComM network.
 * @return      boolean
 * @retval      TRUE if the current channel of ComMNetwork is valid, FALSE otherwise
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-66111
 */
LINSM_LOCAL boolean LinSM_ValidateNetworkHandle(uint8 apiId, NetworkHandleType network);
/**
 * @brief       Check that the function parameter pointer is valid.
 * @param[in]   apiId   : The ID of the API to use when calling this function.
 * @param[in]   pointer : Input parameter pointer.
 * @return      boolean
 * @retval      TRUE if the Input parameter pointer is not NULL_PTR, FALSE otherwise
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-66110
 */
LINSM_LOCAL boolean LinSM_ValidatePointer(uint8 apiId, const void* pointer);
#if LINSM_MASTER_NODE_SUPPORT == STD_ON
/**
 * @brief       Check that the schedule table is valid.
 * @param[in]   apiId   : The ID of the API to use when calling this function.
 * @param[in]   network : Index of the ComM network.
 * @param[in]   schedule: Schedule table to be checked
 * @return      boolean
 * @retval      TRUE if the schedule table is valid, FALSE otherwise
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-59658
 */
LINSM_LOCAL boolean LinSM_ValidateSchedule(uint8 apiId, NetworkHandleType network, LinIf_SchHandleType schedule);
#endif
#endif
/**
 * @brief       Check if the request for communication mode is valid.
 * @param[in]   apiId   : The ID of the API to use when calling this function.
 * @param[in]   mode    : The communication mode of the request.
 * @return      boolean
 * @retval      TRUE if the request for communication mode is valid, FALSE otherwise
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-66113
 */
LINSM_LOCAL boolean LinSM_ValidateRequestComMode(uint8 apiId, ComM_ModeType mode);

#define LINSM_STOP_SEC_CODE
#include "LinSM_MemMap.h"

/* ============================================ internal data definitions =========================================== */
/* Module initialize status, TRUE initialized, FALSE not initialized */
#define LINSM_START_SEC_VAR_CLEARED_8
#include "LinSM_MemMap.h"
LINSM_LOCAL LinSM_ModuleStatusType LinSM_ModuleStatus[LINSM_PARTITION_NUM];
#define LINSM_STOP_SEC_VAR_CLEARED_8
#include "LinSM_MemMap.h"

/* global variable to store the configuration pointer */
#define LINSM_START_SEC_VAR_CLEARED_PTR
#include "LinSM_MemMap.h"
LINSM_LOCAL const LinSM_ConfigType* LinSM_CfgPtr;
#define LINSM_STOP_SEC_VAR_CLEARED_PTR
#include "LinSM_MemMap.h"
/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define LINSM_START_SEC_CODE
#include "LinSM_MemMap.h"
/**
 * This function initializes the LinSM.
 */
void LinSM_Init(const LinSM_ConfigType* ConfigPtr)
{
#if LINSM_DEV_ERROR_DETECT == STD_ON
    if (LinSM_ValidatePartitionContext(LINSM_SERVICE_ID_INIT) && LinSM_ValidateInitStatus(LINSM_SERVICE_ID_INIT)
        && LinSM_ValidatePointer(LINSM_SERVICE_ID_INIT, ConfigPtr))
#endif
    {
        LinSM_CfgPtr = ConfigPtr;

        /** SWS_LinSM_00043 */
#if LINSM_MULTIPLE_PARTITION_USED == STD_ON
        ApplicationType applicationID = GetApplicationID();
#endif
        SchM_Enter_LinSM_Context();
        /** initialize run time structure variable for every channel */
        for (uint8 loop = 0u; loop < LINSM_CHANNEL_NUM; loop += 1u)
        {
            LinSM_RunTimeType* LinSM_RTPtr = LINSM_RUNTIME_PTR(loop);
#if LINSM_MULTIPLE_PARTITION_USED == STD_ON
            if (LINSM_PARTITION_ID(loop) != applicationID)
#endif
            {
                /** SWS_LinSM_00152 SWS_LinSM_00160 */
                LinSM_RTPtr->ChannelComMode = LINSM_NO_COM;
                LinSM_RTPtr->TimerCnt       = 0u;
                LinSM_RTPtr->TimerType      = LINSM_NONE_TIMER;
                LinSM_RTPtr->RunningReq     = LINSM_EXE_NOTHING;
                LinSM_RTPtr->WakeupRetryCnt = 0u;
                LinSM_RTPtr->ReqComModeFlag = FALSE;
#if LINSM_MASTER_NODE_SUPPORT == STD_ON
                /** SWS_LinSM_00216 */
                LinSM_RTPtr->CurSchedule    = 0u;
                LinSM_RTPtr->ReportSchedule = FALSE;
#endif
            }
        }
        SchM_Exit_LinSM_Context();
        /** SWS_LinSM_00025
         * change module state */
        uint16 partitionIndex = LinSM_GetPartitionIndex();
        SchM_Enter_LinSM_Context();
        LinSM_ModuleStatus[partitionIndex] = LINSM_INIT;
        SchM_Exit_LinSM_Context();
    }
}

#if LINSM_MASTER_NODE_SUPPORT == STD_ON
/**
 * The upper layer requests a schedule table to be changed on one LIN network.
 */
Std_ReturnType LinSM_ScheduleRequest(NetworkHandleType network, LinIf_SchHandleType schedule)
{
    Std_ReturnType ret = E_NOT_OK;
#if LINSM_DEV_ERROR_DETECT == STD_ON
    if (LinSM_ValidatePartitionContext(LINSM_SERVICE_ID_SCHEDULE_REQUEST)
        && LinSM_ValidateInitStatus(LINSM_SERVICE_ID_SCHEDULE_REQUEST)
        && LinSM_ValidateNetworkHandle(LINSM_SERVICE_ID_SCHEDULE_REQUEST, network)
        && LinSM_ValidateSchedule(LINSM_SERVICE_ID_SCHEDULE_REQUEST, network, schedule))
#endif
    {
        LinSM_ChannelIndexType chl         = LinSM_GetChannelByNetworkHandle(network);
        LinSM_RunTimeType*     LinSM_RTPtr = LINSM_RUNTIME_PTR(chl);

        /** SWS_LinSM_10211 */
        if (LinSM_RTPtr->ChannelComMode == LINSM_FULL_COM)
        {
            /** SWS_LinSM_00079 SWS_LinSM_00168 */
            ret = LinIf_ScheduleRequest(network, LinSM_CfgPtr->SchedulePtr[schedule]);

            if (ret == E_NOT_OK)
            {
                LinSM_RTPtr->ReportSchedule = TRUE;
            }
        }
    }
    return ret;
}
#endif

/**
 * Function to get the version information of LinSM
 */
#if LINSM_VERSION_INFO_API == STD_ON
void LinSM_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
#if LINSM_DEV_ERROR_DETECT == STD_ON
    if (LinSM_ValidatePointer(LINSM_SERVICE_ID_GET_VERSIONINFO, versioninfo))
#endif
    {
        (versioninfo)->vendorID         = LINSM_VENDOR_ID;
        (versioninfo)->moduleID         = LINSM_MODULE_ID;
        (versioninfo)->sw_major_version = LINSM_SW_MAJOR_VERSION;
        (versioninfo)->sw_minor_version = LINSM_SW_MINOR_VERSION;
        (versioninfo)->sw_patch_version = LINSM_SW_PATCH_VERSION;
    }
}
#endif

/**
 * Function to query the current communication mode.
 */
Std_ReturnType LinSM_GetCurrentComMode(NetworkHandleType network, ComM_ModeType* mode)
{
    Std_ReturnType ret = E_NOT_OK;
#if LINSM_DEV_ERROR_DETECT == STD_ON
    if (LinSM_ValidatePartitionContext(LINSM_SERVICE_ID_GET_CURRENTCOMMODE)
        && LinSM_ValidateInitStatus(LINSM_SERVICE_ID_GET_CURRENTCOMMODE)
        && LinSM_ValidateNetworkHandle(LINSM_SERVICE_ID_GET_CURRENTCOMMODE, network)
        && LinSM_ValidatePointer(LINSM_SERVICE_ID_GET_CURRENTCOMMODE, mode))
#endif
    {
        LinSM_ChannelIndexType   chl         = LinSM_GetChannelByNetworkHandle(network);
        const LinSM_RunTimeType* LinSM_RTPtr = LINSM_RUNTIME_PTR(chl);
        LinSM_ModuleStatusType   status      = LinSM_ModuleStatus[LinSM_GetPartitionIndex()];
        if (status == LINSM_INIT)
        {
            /** SWS_LinSM_00181 SWS_LinSM_00180 */
            *mode = (LinSM_RTPtr->ChannelComMode == LINSM_FULL_COM) ? COMM_FULL_COMMUNICATION : COMM_NO_COMMUNICATION;
        }
        else
        {
            /** SWS_LinSM_00182 */
            *mode = COMM_NO_COMMUNICATION;
        }

        ret = E_OK;
    }
    return ret;
}

/**
 * Requesting of a communication mode.
 * The mode switch will not be made instant. The LinSM will notify the caller when mode transition is made.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_LinSM_STCAL */
Std_ReturnType LinSM_RequestComMode(NetworkHandleType network, ComM_ModeType mode)
/* PRQA S 6070 -- */ /* VL_MTR_LinSM_STCAL */
{
    Std_ReturnType ret = E_NOT_OK;

    if (LinSM_ValidateRequestComMode(LINSM_SERVICE_ID_REQUEST_COMMODE, mode)
#if LINSM_DEV_ERROR_DETECT == STD_ON
        && LinSM_ValidatePartitionContext(LINSM_SERVICE_ID_REQUEST_COMMODE)
        && LinSM_ValidateInitStatus(LINSM_SERVICE_ID_REQUEST_COMMODE)
        && LinSM_ValidateNetworkHandle(LINSM_SERVICE_ID_REQUEST_COMMODE, network)
#endif
    )
    {
        LinSM_ChannelIndexType chl         = LinSM_GetChannelByNetworkHandle(network);
        LinSM_RunTimeType*     LinSM_RTPtr = LINSM_RUNTIME_PTR(chl);

        /** SWS_LinSM_00047 SWS_LinSM_00178 SWS_LinSM_00237 */
        if ((mode == COMM_FULL_COMMUNICATION)
            && ((LINSM_NODE_TYPE(chl) == LINSM_MASTER)
                || !((LINSM_SILENCEWAKEUP_TIMER == LinSM_RTPtr->TimerType) && (LinSM_RTPtr->TimerCnt > 0u))))
        {
            ret = LinSM_TriggerFullCom(chl, TRUE);
        }

        /** SWS_LinSM_00035 */
        if ((mode == COMM_NO_COMMUNICATION) && (LinSM_RTPtr->ChannelComMode == LINSM_FULL_COM)
            && (LinSM_RTPtr->FullComSubStatus == LINSM_RUN_COMMUNICATION))
        {
            /** SWS_LinSM_10208 SWS_LinSM_10209 */
            if (LINSM_NODE_TYPE(chl) == LINSM_MASTER)
            {
                ret = LinSM_TriggerGotoSleep(chl);
            }
            else
            {
                /** SWS_LinSM_00230 */
                ret = E_OK;
            }

            /** SWS_LinSM_00177 */
            if (ret == E_NOT_OK)
            {
                ComM_BusSM_ModeIndication(LINSM_NETWORK_HANDLE(chl), COMM_FULL_COMMUNICATION);
                BswM_LinSM_CurrentState(LINSM_NETWORK_HANDLE(chl), LINSM_FULL_COM);
            }
        }

        /** SWS_LinSM_00230 SWS_LinSM_00236 */
        LinSM_RTPtr->ReqComMode     = mode;
        LinSM_RTPtr->ReqComModeFlag = TRUE;
    }
    return ret;
}

/**
 * Periodic function that runs the timers of different request timeouts
 */
void LinSM_MainFunction(LinSM_ChannelIndexType chlIndex)
{
    if (LinSM_ModuleStatus[LinSM_GetPartitionIndex()] == LINSM_INIT)
    {
        LinSM_RunTimeType* LinSM_RTPtr = LINSM_RUNTIME_PTR(chlIndex);

        if (LinSM_RTPtr->TimerCnt != 0u)
        {
            /** decrease the timer counter */
            LinSM_RTPtr->TimerCnt--;
        }

        if ((LinSM_RTPtr->RunningReq == LINSM_EXE_WAKE_UP) && (LinSM_RTPtr->TimerCnt == 0u))
        {
            if (LinSM_RTPtr->TimerType == LINSM_WAKEUP_TIMER)
            {
                LinSM_HandleWakeupPeriod(chlIndex);
            }
            else if (LinSM_RTPtr->TimerType == LINSM_SILENCEWAKEUP_TIMER)
            {
                LinSM_HandleSilencePeriod(chlIndex);
            }
        }

#if LINSM_MASTER_NODE_SUPPORT == STD_ON
        /** SWS_LinSM_00206 SWS_LinSM_00207 */
        if (LinSM_RTPtr->ReportSchedule)
        {
            LinSM_RTPtr->ReportSchedule = FALSE;
            BswM_LinSM_CurrentSchedule(LINSM_NETWORK_HANDLE(chlIndex), LinSM_RTPtr->CurSchedule);
        }
#endif
    }
}

#if LINSM_MASTER_NODE_SUPPORT == STD_ON
/**
 * The LinIf module will call this callback when the new requested schedule table is active.
 */
void LinSM_ScheduleRequestConfirmation(NetworkHandleType network, LinIf_SchHandleType schedule)
{
#if LINSM_DEV_ERROR_DETECT == STD_ON
    if (LinSM_ValidatePartitionContext(LINSM_SERVICE_ID_SCHEDULEREQUEST_CONFIRMATION)
        && LinSM_ValidateInitStatus(LINSM_SERVICE_ID_SCHEDULEREQUEST_CONFIRMATION)
        && LinSM_ValidateNetworkHandle(LINSM_SERVICE_ID_SCHEDULEREQUEST_CONFIRMATION, network)
        && LinSM_ValidateSchedule(LINSM_SERVICE_ID_SCHEDULEREQUEST_CONFIRMATION, network, schedule))
#endif
    {
        LinSM_ChannelIndexType chl         = LinSM_GetChannelByNetworkHandle(network);
        LinSM_RunTimeType*     LinSM_RTPtr = LINSM_RUNTIME_PTR(chl);

        LinSM_RTPtr->CurSchedule    = schedule;
        LinSM_RTPtr->ReportSchedule = TRUE;
    }
}
#endif

#if LINSM_SLAVE_NODE_SUPPORT == STD_ON
/**
 * The LinIf will call this callback when the go to sleep command is received on the network or a bus idle timeout
 */
void LinSM_GotoSleepIndication(NetworkHandleType network)
{
#if LINSM_DEV_ERROR_DETECT == STD_ON
    if (LinSM_ValidatePartitionContext(LINSM_SERVICE_ID_GOTOSLEEP_INDICATION)
        && LinSM_ValidateInitStatus(LINSM_SERVICE_ID_GOTOSLEEP_INDICATION)
        && LinSM_ValidateNetworkHandle(LINSM_SERVICE_ID_GOTOSLEEP_INDICATION, network))
    {
#endif
        LinSM_ChannelIndexType   chl         = LinSM_GetChannelByNetworkHandle(network);
        const LinSM_RunTimeType* LinSM_RTPtr = LINSM_RUNTIME_PTR(chl);

        if (LinSM_RTPtr->ChannelComMode == LINSM_FULL_COM)
        {
            (void)LinSM_TriggerGotoSleep(chl);
        }
    }
}
#endif

/**
 * The LinIf will call this callback when the go to sleep command is sent successfully
 * or not sent successfully on the network.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_LinSM_STCAL */
void LinSM_GotoSleepConfirmation(NetworkHandleType network, boolean success)
/* PRQA S 6070 -- */ /* VL_MTR_LinSM_STCAL */
{
#if LINSM_DEV_ERROR_DETECT == STD_ON
    if (LinSM_ValidatePartitionContext(LINSM_SERVICE_ID_GOTOSLEEP_CONFIRMATION)
        && LinSM_ValidateInitStatus(LINSM_SERVICE_ID_GOTOSLEEP_CONFIRMATION)
        && LinSM_ValidateNetworkHandle(LINSM_SERVICE_ID_GOTOSLEEP_CONFIRMATION, network))
#endif
    {
        LinSM_ChannelIndexType chl         = LinSM_GetChannelByNetworkHandle(network);
        LinSM_RunTimeType*     LinSM_RTPtr = LINSM_RUNTIME_PTR(chl);

        if (LinSM_RTPtr->RunningReq == LINSM_EXE_GOTO_SLEEP)
        {
            if (LinSM_RTPtr->ReqComMode == COMM_NO_COMMUNICATION)
            {
                SchM_Enter_LinSM_Context();
                LinSM_RTPtr->RunningReq     = LINSM_EXE_NOTHING;
                LinSM_RTPtr->ReqComModeFlag = FALSE;
                /* state back to LINSM_RUN_COMMUNICATION(for fail safe purpose) */
                /* if success = FALSE,new gotosleep request can be handled */
                LinSM_RTPtr->FullComSubStatus = LINSM_RUN_COMMUNICATION;
                LinSM_RTPtr->ChannelComMode   = LINSM_NO_COM;
                SchM_Exit_LinSM_Context();

                /** SWS_LinSM_00027 SWS_LinSM_00193 */
                ComM_BusSM_ModeIndication(network, COMM_NO_COMMUNICATION);
                BswM_LinSM_CurrentState(network, LINSM_NO_COM);

#if LINSM_TRANSCEIVER_PASSIVER_MODE_SUPPORT == STD_ON
                if (LINSM_IS_TRANSCEIVER_PASSIVER_MODE_SUPPORT(chl) == TRUE)
                {
                    LinTrcv_TrcvModeType LinTrcvModeType = (TRUE == LINSM_GET_TRANSCEIVER_PASSIVER_MODE(chl))
                                                               ? LINTRCV_TRCV_MODE_STANDBY
                                                               : LINTRCV_TRCV_MODE_SLEEP;
                    (void)LinIf_SetTrcvMode(network, LinTrcvModeType);
                }
#endif
            }
            else
            {
                /** Goto sleep specific for slave node */
                /** SWS_LinSM_00234 */
                (void)LinSM_TriggerFullCom(chl, FALSE);
            }
        }
    }

    LINSM_UNUSED(success);
}

/**
 * The LinIf will call this callback when the wake up signal command is sent not successfully/successfully on the
 * network.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_LinSM_STCAL */
void LinSM_WakeupConfirmation(NetworkHandleType network, boolean success)
/* PRQA S 6070 -- */ /* VL_MTR_LinSM_STCAL */
{
#if LINSM_DEV_ERROR_DETECT == STD_ON
    if (LinSM_ValidatePartitionContext(LINSM_SERVICE_ID_WAKEUP_CONFIRMATION)
        && LinSM_ValidateInitStatus(LINSM_SERVICE_ID_WAKEUP_CONFIRMATION)
        && LinSM_ValidateNetworkHandle(LINSM_SERVICE_ID_WAKEUP_CONFIRMATION, network))
#endif
    {
        LinSM_ChannelIndexType chl         = LinSM_GetChannelByNetworkHandle(network);
        LinSM_RunTimeType*     LinSM_RTPtr = LINSM_RUNTIME_PTR(chl);

        if (LinSM_RTPtr->RunningReq == LINSM_EXE_WAKE_UP)
        {
            SchM_Enter_LinSM_Context();
            /** SWS_LinSM_00154
             *   stop timer
             *  */
            LinSM_RTPtr->TimerCnt   = 0u;
            LinSM_RTPtr->TimerType  = LINSM_NONE_TIMER;
            LinSM_RTPtr->RunningReq = LINSM_EXE_NOTHING;
            SchM_Exit_LinSM_Context();

            if (success)
            {
                SchM_Enter_LinSM_Context();
                /** SWS_LinSM_00049 */
                LinSM_RTPtr->ChannelComMode = LINSM_FULL_COM;
                /** SWS_LinSM_00301 */
                LinSM_RTPtr->FullComSubStatus = LINSM_RUN_COMMUNICATION;
                LinSM_RTPtr->ReqComModeFlag   = FALSE;
                SchM_Exit_LinSM_Context();

                /** SWS_LinSM_00033 SWS_LinSM_00192 */
                ComM_BusSM_ModeIndication(network, COMM_FULL_COMMUNICATION);
                BswM_LinSM_CurrentState(network, LINSM_FULL_COM);
/** SWS_LinSM_00205 */
#if LINSM_TRANSCEIVER_PASSIVER_MODE_SUPPORT == STD_ON
                if (LINSM_IS_TRANSCEIVER_PASSIVER_MODE_SUPPORT(chl) == TRUE)
                {
                    (void)LinIf_SetTrcvMode(network, LINTRCV_TRCV_MODE_NORMAL);
                }
#endif
            }
            else
            {
                /** SWS_LinSM_00202 */
                ComM_BusSM_ModeIndication(network, COMM_NO_COMMUNICATION);
                BswM_LinSM_CurrentState(network, LINSM_NO_COM);
            }
        }
    }
}

/* ========================================== internal function definitions ========================================= */
LINSM_LOCAL Std_ReturnType LinSM_TriggerFullCom(LinSM_ChannelIndexType chlIdx, boolean oneShot)
{
    LinSM_RunTimeType* LinSM_RTPtr = LINSM_RUNTIME_PTR(chlIdx);

    Std_ReturnType ret = LinIf_Wakeup(LINSM_NETWORK_HANDLE(chlIdx));

    SchM_Enter_LinSM_Context();
    if ((ret == E_OK) || (!oneShot))
    {
        LinSM_RTPtr->TimerCnt   = LINSM_CONFIRMATION_TIMEOUT(chlIdx);
        LinSM_RTPtr->TimerType  = LINSM_WAKEUP_TIMER;
        LinSM_RTPtr->RunningReq = LINSM_EXE_WAKE_UP;

        if (oneShot)
        {
            /** The number of WakeupRetryCnt is loaded when the wake-up process is triggered only once. */
            LinSM_RTPtr->WakeupRetryCnt = LINSM_MODE_REQUEST_REPETITION_MAX();
        }
    }

    if (LinSM_RTPtr->TimerCnt == 0u)
    {
        LinSM_RTPtr->TimerType = LINSM_NONE_TIMER;
    }
    SchM_Exit_LinSM_Context();
    return ret;
}

LINSM_LOCAL Std_ReturnType LinSM_TriggerGotoSleep(LinSM_ChannelIndexType chlIdx)
{
    LinSM_RunTimeType* LinSM_RTPtr = LINSM_RUNTIME_PTR(chlIdx);
    Std_ReturnType     ret         = LinIf_GotoSleep(LINSM_NETWORK_HANDLE(chlIdx));

    if (ret == E_OK)
    {
        /** SWS_LinSM_00302 */
        LinSM_RTPtr->FullComSubStatus = LINSM_GOTO_SLEEP;
        LinSM_RTPtr->RunningReq       = LINSM_EXE_GOTO_SLEEP;
    }
    else
    {
        LinSM_RTPtr->RunningReq = LINSM_EXE_NOTHING;
    }
    return ret;
}

/**
 * SlienceAfterWakeupTimer timeout
 */
LINSM_LOCAL void LinSM_HandleSilencePeriod(LinSM_ChannelIndexType chlIdx)
{
    SchM_Enter_LinSM_Context();
    LinSM_RunTimeType* LinSM_RTPtr = LINSM_RUNTIME_PTR(chlIdx);
    /** SWS_LinSM_00236 */
    if (LinSM_RTPtr->ReqComModeFlag && (LinSM_RTPtr->ReqComMode == COMM_FULL_COMMUNICATION))
    {
        LinSM_RTPtr->TimerType      = LINSM_WAKEUP_TIMER;
        LinSM_RTPtr->TimerCnt       = LINSM_CONFIRMATION_TIMEOUT(chlIdx);
        LinSM_RTPtr->WakeupRetryCnt = LINSM_MODE_REQUEST_REPETITION_MAX();
    }
    else
    {
        LinSM_RTPtr->TimerType = LINSM_NONE_TIMER;
        LinSM_RTPtr->TimerCnt  = 0u;
    }
    SchM_Exit_LinSM_Context();
}

/**
 * WakeupTimer timeout
 */
LINSM_LOCAL void LinSM_HandleWakeupPeriod(LinSM_ChannelIndexType chlIdx)
{
    SchM_Enter_LinSM_Context();
    LinSM_RunTimeType* LinSM_RTPtr = LINSM_RUNTIME_PTR(chlIdx);

    /** SWS_LinSM_00102 */
    if (LINSM_MODE_REQUEST_REPETITION_MAX() > 0u)
    {
        if (LinSM_RTPtr->WakeupRetryCnt == 0u)
        {
            (void)Det_ReportRuntimeError(
                LINSM_MODULE_ID,
                LINSM_INSTANCE_ID,
                LINSM_SERVICE_ID_MAIN_FUNCTION,
                LINSM_E_CONFIRMATION_TIMEOUT);
            if (LINSM_NODE_TYPE(chlIdx) == LINSM_SLAVE)
            {
                LinSM_RTPtr->TimerCnt  = LINSM_SILENCEAFTERWAKEUP_TIMEOUT(chlIdx);
                LinSM_RTPtr->TimerType = LINSM_SILENCEWAKEUP_TIMER;
            }
            else
            {
                LinSM_RTPtr->WakeupRetryCnt = LINSM_MODE_REQUEST_REPETITION_MAX();
            }
        }
        else
        {
            LinSM_RTPtr->WakeupRetryCnt--;
        }
    }
    SchM_Exit_LinSM_Context();

    if (LinSM_RTPtr->ReqComModeFlag && (LinSM_RTPtr->ReqComMode == COMM_FULL_COMMUNICATION))
    {
        if (LinSM_RTPtr->TimerType != LINSM_SILENCEWAKEUP_TIMER)
        {
            /** SWS_LinSM_00304 SWS_LinSM_00235 */
            (void)LinSM_TriggerFullCom(chlIdx, FALSE);
        }
    }
}

/**
 * Get LinSM channel id coresponding to input network
 */
LINSM_LOCAL LinSM_ChannelIndexType LinSM_GetChannelByNetworkHandle(NetworkHandleType network)
{
    uint8 loop;
    /* loop for every channel, search corresponding network */
    for (loop = 0u; loop < LINSM_CHANNEL_NUM; ++loop)
    {
        if (LINSM_NETWORK_HANDLE(loop) == network)
        {
            break;
        }
    }

    return loop;
}

/**
 * Get the PartitionStatus structure address according to the applicationID
 */
LINSM_LOCAL uint16 LinSM_GetPartitionIndex(void)
{
    uint16 index = 0u;
#if LINSM_MULTIPLE_PARTITION_USED == STD_ON
    ApplicationType applicationID = GetApplicationID();
    for (; index < LINSM_PARTITION_NUM; ++index)
    {
        if (LinSM_OsApplicationList[index] == applicationID)
        {
            break;
        }
    }
#endif
    return index;
}

#if LINSM_DEV_ERROR_DETECT == STD_ON
LINSM_LOCAL boolean LinSM_ValidatePartitionContext(uint8 apiId)
{
    boolean ret = TRUE;
    if (LinSM_GetPartitionIndex() >= LINSM_PARTITION_NUM)
    {
        (void)Det_ReportError(LINSM_MODULE_ID, LINSM_INSTANCE_ID, apiId, LINSM_E_PARTITION);
        ret = FALSE;
    }
    return ret;
}

LINSM_LOCAL boolean LinSM_ValidateInitStatus(uint8 apiId)
{
    boolean                ret     = TRUE;
    uint8                  errorId = LINSM_E_INVALID_ID;
    LinSM_ModuleStatusType status  = LinSM_ModuleStatus[LinSM_GetPartitionIndex()];

    if ((apiId == LINSM_SERVICE_ID_INIT) && (status == LINSM_INIT))
    {
        errorId = LINSM_E_INIT_FAILED;
    }

    if ((apiId != LINSM_SERVICE_ID_INIT) && (status == LINSM_UNINIT))
    {
        errorId = LINSM_E_UNINIT;
    }

    if (errorId != LINSM_E_INVALID_ID)
    {
        (void)Det_ReportError(LINSM_MODULE_ID, LINSM_INSTANCE_ID, apiId, errorId);
        ret = FALSE;
    }

    return ret;
}

LINSM_LOCAL boolean LinSM_ValidatePointer(uint8 apiId, const void* pointer)
{
    boolean ret = TRUE;
    if (pointer == NULL_PTR)
    {
        (void)Det_ReportError(LINSM_MODULE_ID, LINSM_INSTANCE_ID, apiId, LINSM_E_PARAM_POINTER);
        ret = FALSE;
    }
    return ret;
}

LINSM_LOCAL boolean LinSM_ValidateNetworkHandle(uint8 apiId, NetworkHandleType network)
{
    boolean                ret     = TRUE;
    uint8                  errorId = LINSM_E_INVALID_ID;
    LinSM_ChannelIndexType chlIdx  = LinSM_GetChannelByNetworkHandle(network);
    if (chlIdx >= LINSM_CHANNEL_NUM)
    {
        errorId = LINSM_E_NONEXISTENT_NETWORK;
    }
#if LINSM_MULTIPLE_PARTITION_USED == STD_ON
    else if (GetApplicationID() != LINSM_PARTITION_ID(chlIdx))
    {
        errorId = LINSM_E_PARTITION;
    }
#endif

    if (errorId != LINSM_E_INVALID_ID)
    {
        (void)Det_ReportError(LINSM_MODULE_ID, LINSM_INSTANCE_ID, apiId, errorId);
        ret = FALSE;
    }

    return ret;
}

#if LINSM_MASTER_NODE_SUPPORT == STD_ON
/**
 * Check input 'schedule' id is valid
 */
LINSM_LOCAL boolean LinSM_ValidateSchedule(uint8 apiId, NetworkHandleType network, LinIf_SchHandleType schedule)
{
    boolean ret = TRUE;
    /* search corresponding schedule */
    LinSM_ChannelIndexType chlIdx = LinSM_GetChannelByNetworkHandle(network);

    if (schedule >= LINSM_CHANNEL_SCHEDULE_NUM(chlIdx))
    {
        (void)Det_ReportError(LINSM_MODULE_ID, LINSM_INSTANCE_ID, apiId, LINSM_E_PARAMETER);
        ret = FALSE;
    }
    return ret;
}
#endif
#endif

LINSM_LOCAL boolean LinSM_ValidateRequestComMode(uint8 apiId, ComM_ModeType mode)
{
    boolean ret = TRUE;
    if ((mode != COMM_FULL_COMMUNICATION) && (mode != COMM_NO_COMMUNICATION))
    {
#if LINSM_DEV_ERROR_DETECT == STD_ON
        (void)Det_ReportError(LINSM_MODULE_ID, LINSM_INSTANCE_ID, apiId, LINSM_E_PARAMETER);
#endif
        ret = FALSE;
    }

    return ret;
}

#define LINSM_STOP_SEC_CODE
#include "LinSM_MemMap.h"

/* PRQA S 2814,2824 -- */  /* VL_LinSM_DerefNullPtr */
/* PRQA S 1503, 1532 -- */ /* VL_QAC_NoUsedApi, VL_QAC_OneFunRef */
