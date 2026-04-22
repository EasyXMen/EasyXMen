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
********************************************************************************
**                                                                            **
**  FILENAME    : LinIf_Master.c                                              **
**                                                                            **
**  Created on  :                                                             **
**  Author      : HuRongbo                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/

/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "LinIf.h"
#include "istd_lib.h"
#if (STD_ON == LINIF_MASTER_SUPPORT)
#include "LinIf_Master.h"
#include "SchM_LinIf.h"
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif /*LINIF_DEV_ERROR_DETECT == STD_ON*/
#if (LINIF_TP_SUPPORTED == STD_ON)
#include "LinTp_Cfg.h"
#if (LINTP_MASTER_SUPPORT == STD_ON)
#include "LinTp_Master.h"
#endif /*LINTP_MASTER_SUPPORT == STD_ON*/
#endif /*LINIF_TP_SUPPORTED == STD_ON*/

/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/

/* PRQA S 3432 ++ */ /* VL_LinIf_MacrosFunctionUsed */
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

static FUNC(P2CONST(LinIf_SubstitutionFramesType, AUTOMATIC, LINIF_CONST), LINIF_CODE)
    LinIf_GetTransmitPending(P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_APPL_CONST) frame);

static FUNC(void, LINIF_CODE) LinIf_SendSporadic(uint8 ch, P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_APPL_CONST) frame);

static FUNC(void, LINIF_CODE)
    LinIf_SendEventTrigger(uint8 ch, P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_APPL_CONST) frame);

static FUNC(void, LINIF_CODE)
    LinIf_SendUnconditional(uint8 ch, P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_APPL_CONST) frame);

static FUNC(void, LINIF_CODE) LinIf_MemCopyFixedFrame(
    P2VAR(uint8, AUTOMATIC, LINIF_APPL_DATA) Dst,
    P2CONST(LinIf_FixedFrameSduType, AUTOMATIC, LINIF_APPL_CONST) Src);

static FUNC(void, LINIF_CODE)
    LinIf_SendNodeConfiguration(uint8 ch, P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_APPL_CONST) frame);

#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_MASTER_SUPPORT == STD_ON))
static FUNC(void, LINIF_CODE) LinIf_SendMRF(uint8 ch, P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_APPL_CONST) frame);

static FUNC(void, LINIF_CODE) LinIf_SendSRF(uint8 ch, P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_APPL_CONST) frame);
#endif

static FUNC(void, LINIF_CODE) LinIf_SetPduInfo(
    P2VAR(Lin_PduType, AUTOMATIC, LINIF_APPL_DATA) pdu,
    P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_APPL_CONST) frame);

static FUNC(void, LINIF_CODE) LinIf_HandleEntryTail(P2VAR(LinIf_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) chPtr);

static FUNC(void, LINIF_CODE) LinIf_SwitchScheduleNotify(uint8 ch);

static FUNC(void, LINIF_CODE) LinIf_SwitchNewSchedule(uint8 ch);

static FUNC(void, LINIF_CODE) LinIf_SwitchNewEntry(uint8 ch);

#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
#if !defined(LINIF_LOCAL)
#define LINIF_LOCAL static /* PRQA S 3414 */ /* VL_QAC_FctLikeMacro */
#endif

#define LINIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "LinIf_MemMap.h"

/* All channels global buffer information */
LINIF_LOCAL VAR(LinIf_MasterRuntimeType, LINIF_VAR) LinIf_MasterChRtData[LINIF_MASTER_CHANNEL_NUMBER];

#define LINIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "LinIf_MemMap.h"
/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Macros Function Definitions                           **
*******************************************************************************/
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

static inline LinIf_MasterRuntimeType* LinIf_GetMasterRuntimeData(NetworkHandleType chId)
{
    return &LinIf_MasterChRtData[chId];
}

static inline void LinIf_SetEvent(LinIf_MasterRuntimeType* chPtr, uint16 event)
{
    chPtr->RootEvent |= event;
}

static inline void LinIf_ClrEvent(LinIf_MasterRuntimeType* chPtr, uint16 event)
{
    chPtr->RootEvent &= (uint16)(~event);
}

static inline boolean LinIf_IsEvent(const LinIf_MasterRuntimeType* chPtr, uint16 event)
{
    return (chPtr->RootEvent & event) != 0u;
}

#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
/* PRQA S 2814,2824,2813,2889 ++ */ /* VL_QAC_DerefNullPtr */
/* PRQA S 1532,1503 ++ */           /* VL_QAC_OneFunRef */
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
/******************************************************************************/
/*
 * Brief               Initializes the LinIf master channel.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE) LinIf_MasterInit(void)
{
    NetworkHandleType idx;
    P2VAR(LinIf_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) chPtr;

    for (idx = 0u; idx < LINIF_MASTER_CHANNEL_NUMBER; idx++)
    {
        chPtr = LinIf_GetMasterRuntimeData(idx);

        /*@req <SWS_LinIf_00507>*/
        /* Set the state of channel */
        if (LINIF_STARTUP_NORMAL == LINIF_GET_STARTUP_STATE(idx))
        {
            chPtr->LinIfChannelState = LINIF_CHANNEL_OPERATIONAL;
        }
        else
        {
            chPtr->LinIfChannelState = LINIF_CHANNEL_SLEEP;
        }

        /*@req <SWS_LinIf_00233>*/
        /* Reset channel */
        chPtr->CurSchedule   = NULL_PTR;
        chPtr->ResSchedule   = NULL_PTR;
        chPtr->RdySchedule   = NULL_PTR;
        chPtr->CurEntry      = NULL_PTR;
        chPtr->CurEntryIndex = 0u;
        chPtr->ResEntryIndex = 0u;
        chPtr->RootEvent     = LINIF_EVENT_NONE;
        chPtr->Timer         = 0u;
        chPtr->WakeupFlag    = FALSE;
    }
}

/******************************************************************************/
/*
 * Brief               Transmit request process in master node.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 *                     frameIdx: Frame Index
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE)
LinIf_MasterTransmit(NetworkHandleType ch, uint16 frameIdx)
{
    const LinIf_MasterRuntimeType* chPtr = LinIf_GetMasterRuntimeData(ch);
    Std_ReturnType                 ret;

    if (NULL_PTR == chPtr->CurSchedule)
    {
        /*@req <SWS_LinIf_00719>*/
        ret = E_NOT_OK;
    }
    else
    {
        /* Locked */
        SchM_Enter_LinIf_ExclusiveArea_Channel();

        /*@req <SWS_LinIf_00341>,<SWS_LinIf_00730>*/
        /* Set transmit request flag */
        LINIF_GET_TRANSMIT_REQ(frameIdx) = TRUE;

        /* Unlocked */
        SchM_Exit_LinIf_ExclusiveArea_Channel();
        ret = E_OK;
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               Transmit request process in master node.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 *                     frameIdx: Frame Index
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE)
LinIf_MasterScheduleRequest(NetworkHandleType ch, LinIf_SchHandleType ScheduleId)
{
    P2VAR(LinIf_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA)
    chPtr                        = LinIf_GetMasterRuntimeData(ch);
    Std_ReturnType      ret      = E_NOT_OK;
    LinIf_SchHandleType schedule = ScheduleId;

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    if ((LINIF_CHANNEL_SLEEP == chPtr->LinIfChannelState)
        || ((LINIF_NULL_SCHEDULE_INDEX != ScheduleId)
            && ((ScheduleId < LINIF_GET_SCHEDULE_OFS(ch))
                || ((LINIF_GET_SCHEDULE_NUM(ch) + LINIF_GET_SCHEDULE_OFS(ch)) <= ScheduleId))))
    {
        /*@req <SWS_LinIf_00567>*/
        LinIf_Det_ReportError(LINIF_SCHEDULEREQUEST_ID, LINIF_E_SCHEDULE_REQUEST_ERROR);
    }
    else
#endif
    {
        if (LINIF_NULL_SCHEDULE_INDEX == schedule)
        {
            chPtr->RdySchedule = NULL_PTR;
        }
        else
        {
            schedule -= LINIF_GET_SCHEDULE_OFS(ch);
            chPtr->RdySchedule = &LINIF_GET_SCHEDULE(ch, schedule);
        }

        /* Set schedule request flag */
        LinIf_SetEvent(chPtr, LINIF_EVENT_SCHEDULE_REQ);
        ret = E_OK;
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               Go-to-sleep process in master node.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE)
LinIf_MasterGotoSleep(NetworkHandleType ch)
{
    P2VAR(LinIf_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA)
    chPtr = LinIf_GetMasterRuntimeData(ch);

    /*@req <SWS_LinIf_00488>*/
    /* Set sleep request flag */
    LinIf_SetEvent(chPtr, LINIF_EVENT_SLEEP);
}

/* PRQA S 1505 ++ */ /* VL_LinIf_1505 */

/******************************************************************************/
/*
 * Brief: Process sleep state transmit
 * Param-Name[in]: ch
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
FUNC(void, LINIF_CODE) LinIf_SleepProcess(uint8 ch)
{
    uint8* sdu = NULL_PTR;
    uint8  linDriver;
    uint8  linChannel;
    P2VAR(LinIf_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA)
    chPtr = LinIf_GetMasterRuntimeData(ch);

    if (!(LinIf_MasterChRtData[ch].Timer == 0u))
    {
        return;
    }

    if (LinIf_IsEvent(chPtr, LINIF_EVENT_SLEEP))
    {
        linDriver  = LINIF_GET_LIN_DRIVER_ID(ch);
        linChannel = LINIF_GET_LIN_CHANNEL_ID(ch);
        if (!(LinIf_IsEvent(chPtr, LINIF_EVENT_GOTO_SLEEP)))
        {
            if (LINIF_CHANNEL_SLEEP != chPtr->LinIfChannelState)
            {
                /*@req <SWS_LinIf_00453>*/
                /* Send a goto-sleep request on bus */
                (void)Lin_DriverApi[linDriver].LinGoToSleep(linChannel);
                /* Set sleep frame slot time */
                chPtr->Timer = LINIF_GET_LIN_CHANNEL_SLEEPDELAY(ch);

                /* Set goto-sleep flag */
                LinIf_SetEvent(chPtr, LINIF_EVENT_GOTO_SLEEP);
                /* Prepare sleep */
                chPtr->LinIfChannelState = LINIF_CHANNEL_GOTO_SLEEP;
            }
            else
            {
                /*@req <SWS_LinIf_00597>*/
                /* Internal goto-sleep request */
                (void)Lin_DriverApi[linDriver].LinGoToSleepInternal(linChannel);

                /*@req <SWS_LinIf_00557>*/
                /* Notify upper */
                USER_GOTO_SLEEP_CONFIRMATION(ch, LINIF_GET_COMM_NETWORK(ch), TRUE);
            }

            /*@req <SWS_LinIf_00712>*/
            /* Clear wakeup flag */
            chPtr->WakeupFlag = FALSE;
        }
        else
        {
            Lin_StatusType st = Lin_DriverApi[linDriver].LinGetStatus(linChannel, (uint8**)&sdu);
            if (LIN_CH_SLEEP == st)
            {
                /*@req <SWS_LinIf_00455>*/
                chPtr->LinIfChannelState = LINIF_CHANNEL_SLEEP;

                /*@req <SWS_LinIf_00293>,<SWS_LinIf_00444>*/
                /* Switch the current used schedule table to the NULL_SCHEDULE */
                chPtr->RdySchedule   = NULL_PTR;
                chPtr->CurSchedule   = NULL_PTR;
                chPtr->CurEntry      = NULL_PTR;
                chPtr->CurEntryIndex = 0u;
                chPtr->ResSchedule   = NULL_PTR;
                chPtr->ResEntryIndex = 0u;
                chPtr->RootEvent     = LINIF_EVENT_NONE;
#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_MASTER_SUPPORT == STD_ON))
                if (LINIF_SUPPORT_TP_TRANSMIT(ch))
                {
                    LinTp_MasterChannelInit(ch);
                }
#endif

                /*@req <SWS_LinIf_00495> */
                /* Notify upper change to NULL_SCHEDULE */
                USER_SCHEDULE_REQUEST_CONFIRMATION(ch, LINIF_GET_COMM_NETWORK(ch), 0u);

                /*@req <SWS_LinIf_00557>*/
                /* Notify upper */
                USER_GOTO_SLEEP_CONFIRMATION(ch, LINIF_GET_COMM_NETWORK(ch), TRUE);
            }
            else
            {
                /*@req <SWS_LinIf_00558>*/
                /* Notify upper */
                USER_GOTO_SLEEP_CONFIRMATION(ch, LINIF_GET_COMM_NETWORK(ch), FALSE);

                /* Go-to-sleep not successful,state back to OPERATIONAL */
                chPtr->LinIfChannelState = LINIF_CHANNEL_OPERATIONAL;
            }

            LinIf_ClrEvent(chPtr, LINIF_EVENT_SLEEP);
            LinIf_ClrEvent(chPtr, LINIF_EVENT_GOTO_SLEEP);
        }
    }
    else
    {
        /* Only for MISRA-C */
    }
}

/******************************************************************************/
/*
 * Brief: Process wake-up state transmit
 * Param-Name[in]: ch
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE)
LinIf_MasterWakeUp(NetworkHandleType ch)
{
    P2VAR(LinIf_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA)
    chPtr                     = LinIf_GetMasterRuntimeData(ch);
    uint8          linDriver  = LINIF_GET_LIN_DRIVER_ID(ch);
    uint8          linChannel = LINIF_GET_LIN_CHANNEL_ID(ch);
    Std_ReturnType ret;

    if (LINIF_CHANNEL_SLEEP == chPtr->LinIfChannelState)
    {
        if (TRUE == chPtr->WakeupFlag)
        {
            /*@req <SWS_LinIf_00713>,<SWS_LinIf_00721>*/
            ret = Lin_DriverApi[linDriver].LinWakeupInternal(linChannel);
        }
        else
        {
            /*@req <SWS_LinIf_00296>,<SWS_LinIf_00720>*/
            ret = Lin_DriverApi[linDriver].LinWakeup(linChannel);
        }

        /*@req <SWS_LinIf_00720>,<SWS_LinIf_00721>*/
        if (E_OK == ret)
        {
            /*@req <SWS_LinIf_00478>*/
            chPtr->LinIfChannelState = LINIF_CHANNEL_OPERATIONAL;
        }
    }
    else if (LINIF_CHANNEL_OPERATIONAL == chPtr->LinIfChannelState)
    {
        /*@req <SWS_LinIf_00459>*/
        /* requested go-to-sleep command is not executed,clear it */
        if (LinIf_IsEvent(chPtr, LINIF_EVENT_SLEEP))
        {
            LinIf_ClrEvent(chPtr, LINIF_EVENT_SLEEP);
        }
        else
        {
        }
        /*@req <SWS_LinIf_00432>*/
        ret = E_OK;
    }
    else
    {
        /* LINIF_CHANNEL_GOTO_SLEEP */
        /* This case the AUTOSAR specification is not describe */
        ret = E_NOT_OK;
    }

    /*@req <SWS_LinIf_00720>,<SWS_LinIf_00721>*/
    if (E_OK == ret)
    {
        /*@req <SWS_LinIf_00714>*/
        chPtr->WakeupFlag = FALSE;
    }

    /* Set notify upper layer flag according to request result */
    if (E_OK == ret)
    {
        LinIf_SetEvent(chPtr, LINIF_EVENT_WAKEUP_E_OK);
    }
    else
    {
        LinIf_SetEvent(chPtr, LINIF_EVENT_WAKEUP_E_NOT_OK);
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief: Wakeup confirmation process in master node
 * Param-Name[in]: ch
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
FUNC(void, LINIF_CODE)
LinIf_MasterWakeupConfirmation(EcuM_WakeupSourceType WakeupSource)
{
#if (LINIF_LIN_CHANNEL_WAKEUP_SUPPORT == STD_ON) \
    || ((LINIF_TRCV_DRIVER_SUPPORTED == STD_ON) && (LINIF_LIN_TRCV_WAKEUP_SUPPORT == STD_ON))
    P2VAR(LinIf_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) chPtr;
    EcuM_WakeupSourceType localWakeupSource;
    NetworkHandleType     ch;
#endif

#if (LINIF_LIN_CHANNEL_WAKEUP_SUPPORT == STD_ON)
    /*check all Lin driver channel to find the wake-up source*/
    for (ch = 0u; ch < LINIF_MASTER_CHANNEL_NUMBER; ch++)
    {
        localWakeupSource = LINIF_GET_LIN_DRIVER_WAKEUP_SOURCE(ch);
        if (0u != (WakeupSource & localWakeupSource))
        {
            /*@req <SWS_LinIf_00716>*/
            chPtr             = LinIf_GetMasterRuntimeData(ch);
            chPtr->WakeupFlag = TRUE;
            break;
        }
    }
#endif

#if (LINIF_TRCV_DRIVER_SUPPORTED == STD_ON)
#if (LINIF_LIN_TRCV_WAKEUP_SUPPORT == STD_ON)
    /*check all Lin transceiver channel to find the wake-up source*/
    for (ch = 0; ch < LINIF_MASTER_CHANNEL_NUMBER; ch++)
    {
        localWakeupSource = LINIF_GET_LIN_TRCV_WAKEUP_SOURCE(ch);
        if (0u != (WakeupSource & localWakeupSource))
        {
            /*@req <SWS_LinIf_00716>*/
            chPtr             = LinIf_GetMasterRuntimeData(ch);
            chPtr->WakeupFlag = TRUE;
            break;
        }
    }
#endif
#endif
}

/******************************************************************************/
/*
 * Brief: LinIf master main function
 * Param-Name[in]: None
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
/* PRQA S 6070 ++ */ /* VL_MTR_LinIf_STCAL */
FUNC(void, LINIF_CODE) LinIf_MasterMainHandle(NetworkHandleType ch)
{
#if (LINIF_SCHEDULE_CHANGE_NEXT_TIME_BASE_SUPPORT == STD_ON)
    const LinIf_ChannelType* chCfgPtr = &LINIF_GET_CHANNEL(ch);
    LinIf_MasterRuntimeType* chPtr    = LinIf_GetMasterRuntimeData(ch);
#endif

#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_MASTER_SUPPORT == STD_ON))
    NetworkHandleType lintpChannelId = LinTp_GetLinTpChannel(ch);
#endif

    LinIf_SlotTimer(ch);
    LinIf_WakeUpProcess(ch);
    LinIf_SleepProcess(ch);

    if (LINIF_CHANNEL_OPERATIONAL == LinIf_GetChannelState(ch))
    {
        if ((TRUE == LinIf_IsEntryDelayTimeout(ch))
#if (LINIF_SCHEDULE_CHANGE_NEXT_TIME_BASE_SUPPORT == STD_ON)
            /* @req SWS_LinIf_00727 */
            /* When LinIfScheduleChangeNextTimeBase is TRUE, the entry has not ended, should determine the message
               transmission or reception within the entry is complete or not */
            || (LinIf_IsEvent(chPtr, LINIF_EVENT_SCHEDULE_REQ | LINIF_EVENT_SKIP_SLOT_TIMER)
                && (TRUE == chCfgPtr->LinIfScheduleChangeNextTimeBase)
                && (!LinIf_IsEvent(chPtr, LINIF_EVENT_ENTRY_NOT_END_SCHEDULE_CHANGE)))
#endif
        )
        {
            LinIf_PrevTransmit(ch);

#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_MASTER_SUPPORT == STD_ON))
            if (LINIF_SUPPORT_TP_TRANSMIT(ch))
            {
                LinTp_RxProcess(lintpChannelId);
                LinTp_TxProcess(lintpChannelId);
            }
#endif
        }

#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_MASTER_SUPPORT == STD_ON))
        /* Load transmit request */
        if (LINIF_SUPPORT_TP_TRANSMIT(ch))
        {
            LinTp_LoadTxRequest(lintpChannelId);
            LinTp_HandleTimers(ch, lintpChannelId);
        }
#endif

        if ((TRUE == LinIf_IsEntryDelayTimeout(ch))
#if (LINIF_SCHEDULE_CHANGE_NEXT_TIME_BASE_SUPPORT == STD_ON)
            /* @req SWS_LinIf_00727 */
            /* Transmission or reception within the entry is complete, Schedule table can be change */
            || LinIf_IsEvent(chPtr, LINIF_EVENT_ENTRY_NOT_END_SCHEDULE_CHANGE)
#endif
        )
        {
#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_MASTER_SUPPORT == STD_ON))
            if ((!LINIF_SUPPORT_TP_TRANSMIT(ch)) || (!LinTp_IsWaitEventSet(ch))) /* PRQA S 3415 */ /* VL_LinIf_3415 */
#endif
            {
#if (LINIF_SCHEDULE_CHANGE_NEXT_TIME_BASE_SUPPORT == STD_ON)
                LinIf_ClrEvent(chPtr, LINIF_EVENT_ENTRY_NOT_END_SCHEDULE_CHANGE);
#endif
                LinIf_UpdateSchedule(ch);
                LinIf_NextTransmit(ch);
            }
        }
    }
}
/* PRQA S 6070 -- */

/******************************************************************************/
/*
 * Brief: Process wake-up state transmit
 * Param-Name[in]: ch
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
FUNC(void, LINIF_CODE) LinIf_WakeUpProcess(uint8 ch)
{
    P2VAR(LinIf_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA)
    chPtr = LinIf_GetMasterRuntimeData(ch);

    /* Notify Uppler layer LinIf_Wakeup process result */
    if (LinIf_IsEvent(chPtr, LINIF_EVENT_WAKEUP_E_OK))
    {
        /*@req <SWS_LinIf_00496>*/
        USER_WAKEUP_CONFIRMATION(ch, LINIF_GET_COMM_NETWORK(ch), TRUE);

        LinIf_ClrEvent(chPtr, LINIF_EVENT_WAKEUP_E_OK);
    }
    else if (LinIf_IsEvent(chPtr, LINIF_EVENT_WAKEUP_E_NOT_OK))
    {
        /*@req <SWS_LinIf_00670>*/
        USER_WAKEUP_CONFIRMATION(ch, LINIF_GET_COMM_NETWORK(ch), FALSE);

        LinIf_ClrEvent(chPtr, LINIF_EVENT_WAKEUP_E_NOT_OK);
    }
    else
    {
        /* Only for MISRA-C */
    }
}

/******************************************************************************/
/*
 * Brief: Frame slot timer
 * Param-Name[in]: ch
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: Std_ReturnType
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
FUNC(void, LINIF_CODE) LinIf_SlotTimer(uint8 ch)
{
    if (LinIf_MasterChRtData[ch].Timer > 0u)
    {
        /* The current frame slot counter minus 1 */
        LinIf_MasterChRtData[ch].Timer--;
    }
}

/******************************************************************************/
/*
 * Brief: Process the preview frame transmission
 * Param-Name[in]: ch
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
/* PRQA S 6010,6020,6030 ++ */ /* VL_MTR_LinIf_STCYC,VL_MTR_LinIf_STLIN,VL_MTR_LinIf_STMIF */
/* PRQA S 6050,6070 ++ */      /* VL_MTR_LinIf_STST3,VL_MTR_LinIf_STCAL */
FUNC(void, LINIF_CODE) LinIf_PrevTransmit(uint8 ch)
{
    P2VAR(LinIf_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA)
    chPtr = LinIf_GetMasterRuntimeData(ch);

    if (chPtr->CurSchedule != NULL_PTR)
    {
        P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_APPL_CONST)
        frame = &LINIF_GET_FRAME(ch, (chPtr->CurEntry->LinIfFrameRef - LINIF_GET_FRAME_OFS(ch)));

        if ((LinIf_IsEvent(chPtr, LINIF_EVENT_HEADER | LINIF_EVENT_RESPONSE))
#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_MASTER_SUPPORT == STD_ON))
            || ((LINIF_SUPPORT_TP_TRANSMIT(ch)) && (LinTp_IsWaitEventSet(ch))) /* PRQA S 3415 */ /* VL_LinIf_3415 */
#endif
        )
        {
            P2CONST(LinIf_TxPduType, AUTOMATIC, LINIF_APPL_DATA) txPduPtr;
            P2CONST(LinIf_RxPduType, AUTOMATIC, LINIF_APPL_DATA) rxPduPtr;
            P2VAR(uint8, AUTOMATIC, LINIF_APPL_DATA) sduDataPtr = NULL_PTR;
            uint8          sduPtr[LINIF_SDU_LEN];
            PduInfoType    pduInfo;
            uint8          linDriver  = LINIF_GET_LIN_DRIVER_ID(ch);
            uint8          linChannel = LINIF_GET_LIN_CHANNEL_ID(ch);
            Lin_StatusType st         = Lin_DriverApi[linDriver].LinGetStatus(linChannel, &sduDataPtr);
            P2CONST(LinIf_SubstitutionFramesType, AUTOMATIC, LINIF_CONST) sub;
            P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_APPL_CONST) fra;
            uint16 frameIdx;
            P2CONST(LinIf_ScheduleTableType, AUTOMATIC, LINIF_APPL_DATA) triSchedule;
            P2CONST(LinIf_EntryType, AUTOMATIC, LINIF_APPL_CONST) triEntry;
            uint8 idx;

            switch (frame->LinIfPduDirection->LinIfPduDirectionId)
            {
            case LINIF_RX_PDU:
                if (st == LIN_RX_OK)
                {
                    LinIf_ClrEvent(chPtr, ((uint16)(LINIF_EVENT_HEADER) | (uint16)(LINIF_EVENT_RESPONSE)));
#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_MASTER_SUPPORT == STD_ON))
                    if (frame->LinIfFrameType == LINIF_SRF)
                    {
                        /* Receiving parse */
                        LinTp_RxEventParse(ch, sduDataPtr);
                    }
                    else
#endif
                    {
                        /* Next entry */
                        chPtr->CurEntryIndex++;

                        if (!(LinIf_IsEvent(chPtr, LINIF_EVENT_NCSRF_SEND)))
                        {
                            /* Save PDU information */
                            (void)ILib_memcpy(&sduPtr[0], sduDataPtr, frame->LinIfLength);
                            pduInfo.SduDataPtr = sduPtr;
                            pduInfo.SduLength  = frame->LinIfLength;

                            /* event-triggered frame */
                            if (frame->LinIfFrameType == LINIF_EVENT_TRIGGERED)
                            {
                                /* get the corresponding collision resolving schedule table */
                                idx         = chPtr->CurEntry->LinIfCollisionResolvingRef - LINIF_GET_SCHEDULE_OFS(ch);
                                triSchedule = &LINIF_GET_SCHEDULE(ch, idx);
                                for (frameIdx = 0u; frameIdx < triSchedule->LinIfNumOfEntry; frameIdx++)
                                {
                                    /* PRQA S 0488 ++ */ /* VL_LinIf_PointerArithmetic */
                                    triEntry = (triSchedule->LinIfEntry) + frameIdx;
                                    /* PRQA S 0488 -- */
                                    frame = &LINIF_GET_GLOBAL_FRAME(triEntry->LinIfFrameRef);
                                    /* the PID of the unconditional frame in the first byte of the response data.*/
                                    if (pduInfo.SduDataPtr[0] == frame->LinIfFrameId)
                                    {
                                        /*@req <SWS_LinIf_00033> */
                                        /* Notify upper */
                                        rxPduPtr = frame->LinIfPduDirection->LinIfRxPdu;
                                        rxPduPtr->LinIfRxIndicationUL(rxPduPtr->LinIfRxPduRef, &pduInfo);
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                /*@req <SWS_LinIf_00033> */
                                /* Notify upper */
                                rxPduPtr = frame->LinIfPduDirection->LinIfRxPdu;
                                rxPduPtr->LinIfRxIndicationUL(rxPduPtr->LinIfRxPduRef, &pduInfo);
                            }
                        }
                        else
                        {
                            /*@req <SWS_LinIf_00404> */
                            /* Clear the flag of NC SRF header */
                            LinIf_ClrEvent(chPtr, LINIF_EVENT_NCSRF_SEND);
                        }
                    }
                }
                else
                {
#if (LINIF_SCHEDULE_CHANGE_NEXT_TIME_BASE_SUPPORT == STD_ON)
                    /* @req SWS_LinIf_00727 */
                    /* Reception is ongoing,keep waiting */
                    if ((st == LIN_RX_BUSY) && (!LinIf_IsEntryDelayTimeout(ch)))
                    {
                        break;
                    }
#endif
                    LinIf_ClrEvent(chPtr, ((uint16)(LINIF_EVENT_HEADER) | (uint16)(LINIF_EVENT_RESPONSE)));
                    /* Next entry */
                    chPtr->CurEntryIndex++;

                    switch (frame->LinIfFrameType)
                    {
                    case LINIF_UNCONDITIONAL:
/*@req <SWS_LinIf_00254>,<SWS_LinIf_00466> */
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
                        /* Notify upper */
                        LinIf_ReportRuntimeError(LINIF_MAINFUNCTION_ID, LINIF_E_RESPONSE);
#endif
                        break;

                    case LINIF_EVENT_TRIGGERED:
                        if ((st == LIN_RX_ERROR) || (st == LIN_RX_BUSY))
                        {
                            /*@req <SWS_LinIf_00259>,<SWS_LinIf_00588>,
                              <SWS_LinIf_00176> */
                            /* Set collision resolving flag */
                            LinIf_SetEvent(chPtr, LINIF_EVENT_COLLISION);
                            chPtr->CollisionResolvingRef = chPtr->CurEntry->LinIfCollisionResolvingRef;
                        }
                        else
                        {
                            /* Case: st == LIN_RX_NO_RESPONSE */
                            /*@req <SWS_LinIf_00258> */
                        }
                        break;

#if (LINIF_NC_OPTIONAL_REQUEST_SUPPORTED == STD_ON)
                    case LINIF_ASSIGN_NAD:
                    case LINIF_CONDITIONAL:
#endif
                    case LINIF_ASSIGN:
                    case LINIF_ASSIGN_FRAME_ID_RANGE:
                    case LINIF_FREE:
                    case LINIF_SAVE_CONFIGURATION:
                    case LINIF_UNASSIGN:
                        /* Clear the flag of NC SRF header */
                        LinIf_ClrEvent(chPtr, LINIF_EVENT_NCSRF_SEND);
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
                        /*@req <SWS_LinIf_00405> */
                        LinIf_Det_ReportError(LINIF_MAINFUNCTION_ID, LINIF_E_NC_NO_RESPONSE);
#endif
                        break;

                    default:
                        /* if LINIF_SRF, run to next entry */
                        /* Other Frame Type */
                        break;
                    }
                }
                break;

            case LINIF_TX_PDU:
                if (st == LIN_TX_OK)
                {
                    LinIf_ClrEvent(chPtr, ((uint16)(LINIF_EVENT_HEADER) | (uint16)(LINIF_EVENT_RESPONSE)));
                    switch (frame->LinIfFrameType)
                    {
                    case LINIF_UNCONDITIONAL:
                        /*@req <SWS_LinIf_00128> */
                        /* Notify upper */
                        txPduPtr = frame->LinIfPduDirection->LinIfTxPdu;
                        txPduPtr->LinIfTxConfirmationUL(txPduPtr->LinIfTxPduRef);

                        /* Next entry */
                        chPtr->CurEntryIndex++;

                        break;

                    case LINIF_SPORADIC:
                        sub      = LinIf_GetTransmitPending(frame);
                        frameIdx = sub->LinIfSubstitutionFrameRef;
                        fra      = &LINIF_GET_FRAME(ch, (frameIdx - LINIF_GET_FRAME_OFS(ch)));

                        /*@req <SWS_LinIf_00128> */
                        /* Notify upper */
                        txPduPtr = fra->LinIfPduDirection->LinIfTxPdu;
                        txPduPtr->LinIfTxConfirmationUL(txPduPtr->LinIfTxPduRef);

                        /* Next entry */
                        chPtr->CurEntryIndex++;

                        /*@req <SWS_LinIf_00463> */
                        /* A sporadic frame is transmitted successfully,
                           reset the pending flag */
                        SchM_Enter_LinIf_ExclusiveArea_Channel();
                        LINIF_GET_TRANSMIT_REQ(frameIdx) = FALSE;
                        SchM_Exit_LinIf_ExclusiveArea_Channel();
                        break;

#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_MASTER_SUPPORT == STD_ON))
                    case LINIF_MRF:
                        LinTp_MasterTxSuccessHandle(ch);
                        break;
#endif

#if (LINIF_NC_OPTIONAL_REQUEST_SUPPORTED == STD_ON)
                    case LINIF_ASSIGN_NAD:
                    case LINIF_CONDITIONAL:
#endif
                    case LINIF_ASSIGN:
                    case LINIF_ASSIGN_FRAME_ID_RANGE:
                    case LINIF_FREE:
                    case LINIF_SAVE_CONFIGURATION:
                    case LINIF_UNASSIGN:
                        /* Next entry */
                        chPtr->CurEntryIndex++;
                        LinIf_ClrEvent(chPtr, LINIF_EVENT_NCMRF_SEND);
                        LinIf_SetEvent(chPtr, LINIF_EVENT_NCSRF_SEND);
                        break;

                    default:
                        /* Other Frame Type */
                        break;
                    }
                }
                else
                {
#if (LINIF_SCHEDULE_CHANGE_NEXT_TIME_BASE_SUPPORT == STD_ON)
                    /* @req SWS_LinIf_00727 */
                    /* Transmission is ongoing,keep waiting */
                    if ((st == LIN_TX_BUSY) && (!LinIf_IsEntryDelayTimeout(ch)))
                    {
                        break;
                    }
#endif
                    LinIf_ClrEvent(chPtr, ((uint16)(LINIF_EVENT_HEADER) | (uint16)(LINIF_EVENT_RESPONSE)));

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
                    if ((st == LIN_TX_ERROR) || (st == LIN_TX_BUSY))
                    {
                        /*@req <SWS_LinIf_00036>,<SWS_LinIf_00465> */
                        /* Notify upper */
                        LinIf_ReportRuntimeError(LINIF_MAINFUNCTION_ID, LINIF_E_RESPONSE);
                    }
#endif

#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_MASTER_SUPPORT == STD_ON))
                    if (frame->LinIfFrameType == LINIF_MRF)
                    {
                        LinTp_MasterTxErrorHandle(ch, st);
                    }
                    else
#endif
                    {
                        /* Next entry */
                        chPtr->CurEntryIndex++;

                        if (LinIf_IsEvent(chPtr, LINIF_EVENT_NCMRF_SEND))
                        {
                            LinIf_ClrEvent(chPtr, LINIF_EVENT_NCMRF_SEND);
                        }
                    }
                }
                break;

            case LINIF_INTERNAL_PDU:
            case LINIF_SLAVE_TO_SLAVE_PDU:
                /* Next entry */
                chPtr->CurEntryIndex++;
                break;

            default:
                /* Other type PDU */
                break;
            }

#if (LINIF_SCHEDULE_CHANGE_NEXT_TIME_BASE_SUPPORT == STD_ON)
            /* @req SWS_LinIf_00727 */
            /* Transmission or reception within the entry has been completed,in this TimeBase can change schedule
             * table*/
            if ((!LinIf_IsEntryDelayTimeout(ch)) && (st != LIN_TX_BUSY) && (st != LIN_RX_BUSY))
            {
                LinIf_SetEvent(chPtr, LINIF_EVENT_ENTRY_NOT_END_SCHEDULE_CHANGE);
            }
#endif
        }
        else
        {
            if (LINIF_SPORADIC == frame->LinIfFrameType)
            {
                /* Next entry */
                chPtr->CurEntryIndex++;
            }
        }
    }
}
/* PRQA S 6010,6020,6030 -- */
/* PRQA S 6050,6070 -- */

/******************************************************************************/
/*
 * Brief: Process the next frame transmission
 * Param-Name[in]: ch
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
FUNC(void, LINIF_CODE) LinIf_NextTransmit(uint8 ch)
{
    const LinIf_MasterRuntimeType* chPtr = LinIf_GetMasterRuntimeData(ch);
    P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_APPL_CONST) frame;

    if (chPtr->CurSchedule == NULL_PTR)
    {
        return;
    }

    frame = &LINIF_GET_FRAME(ch, (chPtr->CurEntry->LinIfFrameRef - LINIF_GET_FRAME_OFS(ch)));

    switch (frame->LinIfFrameType)
    {
    /* Sporadic frame */
    case LINIF_SPORADIC:
        LinIf_SendSporadic(ch, frame);
        break;

    /* Event-Trigger frame */
    case LINIF_EVENT_TRIGGERED:
        LinIf_SendEventTrigger(ch, frame);
        break;

    /* Unconditional frame */
    case LINIF_UNCONDITIONAL:
        LinIf_SendUnconditional(ch, frame);
        break;

#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_MASTER_SUPPORT == STD_ON))
    /* Master request frame */
    case LINIF_MRF:
        LinIf_SendMRF(ch, frame);
        break;

    /* Slave response frame */
    case LINIF_SRF:
        LinIf_SendSRF(ch, frame);
        break;
#endif

#if (LINIF_NC_OPTIONAL_REQUEST_SUPPORTED == STD_ON)
    case LINIF_ASSIGN_NAD:
    case LINIF_CONDITIONAL:
#endif
    case LINIF_ASSIGN:
    case LINIF_ASSIGN_FRAME_ID_RANGE:
    case LINIF_FREE:
    case LINIF_SAVE_CONFIGURATION:
    case LINIF_UNASSIGN:
        LinIf_SendNodeConfiguration(ch, frame);
        break;

    default:
        /* Other type frame */
        break;
    }
}

/******************************************************************************/
/*
 * Brief: Updates the schedule strategy
 * Param-Name[in]: ch
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
/* PRQA S 6070 ++ */ /* VL_MTR_LinIf_STCAL */
FUNC(void, LINIF_CODE) LinIf_UpdateSchedule(uint8 ch)
{
    P2VAR(LinIf_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA)
    chPtr = LinIf_GetMasterRuntimeData(ch);

    /*@req <SWS_LinIf_00393> */
    if (LinIf_IsEvent(chPtr, LINIF_EVENT_COLLISION))
    {
        LinIf_ClrEvent(chPtr, LINIF_EVENT_COLLISION);
        /* Set 'collision-back' and 'notify' event */
        LinIf_SetEvent(chPtr, LINIF_EVENT_INTERRUPT_BACK | LINIF_EVENT_SCHEDULE_CONF);

        /* Save schedule and entry information  */
        chPtr->ResSchedule = chPtr->CurSchedule;
        /*@req <SWS_LinIf_00485> */
        /* Save entry information */
        if (LINIF_CONTINUE_AT_IT_POINT == chPtr->CurSchedule->LinIfResumePosition)
        {
            /* PRQA S 2834 ++ */ /* VL_LinIf_Division */
            chPtr->ResEntryIndex = chPtr->CurEntryIndex % chPtr->CurSchedule->LinIfNumOfEntry;
            /* PRQA S 2834 -- */
        }
        else
        {
            chPtr->ResEntryIndex = 0u;
        }

        /*@req <SWS_LinIf_00176> */
        /* Switch to the collision resolving schedule table  */
        chPtr->CurSchedule   = &LINIF_GET_SCHEDULE(ch, chPtr->CollisionResolvingRef - LINIF_GET_SCHEDULE_OFS(ch));
        chPtr->CurEntryIndex = 0u;
    }
    else if (LinIf_IsEvent(chPtr, LINIF_EVENT_SCHEDULE_REQ))
    {
        /* Uppler layer request NULL_SCHEDULE */
        if (NULL_PTR == chPtr->RdySchedule)
        {
            /*@req <SWS_LinIf_00444> */
            chPtr->CurSchedule   = NULL_PTR;
            chPtr->CurEntry      = NULL_PTR;
            chPtr->CurEntryIndex = 0u;
            chPtr->ResSchedule   = NULL_PTR;
            chPtr->ResEntryIndex = 0u;
            chPtr->RootEvent     = LINIF_EVENT_NONE;
            chPtr->Timer         = 0u;
#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_MASTER_SUPPORT == STD_ON))
            if (LINIF_SUPPORT_TP_TRANSMIT(ch))
            {
                LinTp_MasterChannelInit(ch);
            }
#endif

            /* Clear Schedule Request flag */
            LinIf_ClrEvent(chPtr, LINIF_EVENT_SCHEDULE_REQ);
            /* Set 'notify' event */
            LinIf_SetEvent(chPtr, LINIF_EVENT_SCHEDULE_CONF);
        }
        else
        {
            /* Switch to the new schedule */
            LinIf_SwitchNewSchedule(ch);
        }
    }
    else
    {
        /* Nothing to do here */
    }

    /* Handles the last entry */
    LinIf_HandleEntryTail(chPtr);

    /* Process switch schedule notification */
    LinIf_SwitchScheduleNotify(ch);

    /* Switch to the new entry */
    LinIf_SwitchNewEntry(ch);
}
/* PRQA S 6070 -- */

/******************************************************************************/
/*
 * Brief: Request the schedule table move to next entry
 * Param-Name[in]: ch
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
FUNC(void, LINIF_CODE) LinIf_MoveScheduleToNextEntry(NetworkHandleType ch)
{
    P2VAR(LinIf_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA)
    chPtr = LinIf_GetMasterRuntimeData(ch);

    /* Move to next entry */
    chPtr->CurEntryIndex++;
}

/******************************************************************************/
/*
 * Brief: Clear LinIf master event,LinTp master used the API interact with
 *        LinIf master
 * Param-Name[in]: ch, event
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
FUNC(void, LINIF_CODE) LinIf_ClearEvent(NetworkHandleType ch, uint16 event)
{
    P2VAR(LinIf_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA)
    chPtr = LinIf_GetMasterRuntimeData(ch);

    LinIf_ClrEvent(chPtr, event);
}

FUNC(boolean, LINIF_CODE) LinIf_CheckEvent(NetworkHandleType ch, uint16 event)
{
    P2CONST(LinIf_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA)
    chPtr = LinIf_GetMasterRuntimeData(ch);

    return LinIf_IsEvent(chPtr, event);
}
/******************************************************************************/
/*
 * Brief: Set LinIf master event,LinTp master used the API interact with
 *        LinIf master
 * Param-Name[in]: ch, event
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
FUNC(void, LINIF_CODE) LinIf_SetEventByChId(NetworkHandleType ch, uint16 event)
{
    P2VAR(LinIf_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA)
    chPtr = LinIf_GetMasterRuntimeData(ch);

    LinIf_SetEvent(chPtr, event);
}

/******************************************************************************/
/*
 * Brief: Request the schedule table move to next entry
 * Param-Name[in]: ch, event
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE)
LinIf_GetCurFrameType(NetworkHandleType ch, P2VAR(LinIf_FrameTypeType, AUTOMATIC, LINIF_APPL_DATA) frameType)
{
    const LinIf_MasterRuntimeType* chPtr = LinIf_GetMasterRuntimeData(ch);
    P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_APPL_CONST) frame;
    Std_ReturnType ret = E_NOT_OK;

    if (NULL_PTR != chPtr->CurEntry)
    {
        frame      = &LINIF_GET_FRAME(ch, (chPtr->CurEntry->LinIfFrameRef - LINIF_GET_FRAME_OFS(ch)));
        *frameType = frame->LinIfFrameType;
        ret        = E_OK;
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief: Request the schedule table move to next entry
 * Param-Name[in]: ch, event
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
FUNC(boolean, LINIF_CODE) LinIf_IsEntryDelayTimeout(NetworkHandleType ch)
{
    boolean ret;

    if (LinIf_MasterChRtData[ch].Timer == 0u)
    {
        ret = TRUE;
    }
    else
    {
        ret = FALSE;
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief: Gets the channel state
 * Param-Name[in]: ch
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: Std_ReturnType
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
FUNC(LinIf_ChannelStateType, LINIF_CODE)
LinIf_GetChannelState(NetworkHandleType ch)
{
    return LinIf_MasterChRtData[ch].LinIfChannelState;
}

/* PRQA S 1505 -- */
/* PRQA S 1532,1503 -- */

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
/******************************************************************************/
/*
 * Brief: Sends a sporadic frame.
 * Param-Name[in]: ch, frame
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static FUNC(P2CONST(LinIf_SubstitutionFramesType, AUTOMATIC, LINIF_CONST), LINIF_CODE)
    LinIf_GetTransmitPending(P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_APPL_CONST) frame)
{
    P2CONST(LinIf_SubstitutionFramesType, AUTOMATIC, LINIF_CONST)
    sub = &frame->LinIfSubstitutionFrames[0];
    P2CONST(LinIf_SubstitutionFramesType, AUTOMATIC, LINIF_CONST)
    retSub = NULL_PTR;
    uint8  idx;
    uint16 frameIdx;

    for (idx = 0u; idx < frame->LinIfNumOfSubstitutionFrame; idx++)
    {
        frameIdx = sub->LinIfSubstitutionFrameRef;
        if (TRUE == LINIF_GET_TRANSMIT_REQ(frameIdx))
        {
            retSub = sub;
            break;
        }
        sub++;
    }

    return retSub;
}

/******************************************************************************/
/*
 * Brief: Sends a sporadic frame.
 * Param-Name[in]: ch, frame
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE) LinIf_SendSporadic(uint8 ch, P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_APPL_CONST) frame)
{
    P2CONST(LinIf_SubstitutionFramesType, AUTOMATIC, LINIF_APPL_CONST)
    sub = LinIf_GetTransmitPending(frame);
    P2VAR(LinIf_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA)
    chPtr = LinIf_GetMasterRuntimeData(ch);

    if (sub != NULL_PTR)
    {
        P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_APPL_CONST)
        fra = &LINIF_GET_FRAME(ch, (sub->LinIfSubstitutionFrameRef - LINIF_GET_FRAME_OFS(ch)));
        PduInfoType pduInfo;
        uint8       sdu[LINIF_SDU_LEN];
        P2CONST(LinIf_TxPduType, AUTOMATIC, LINIF_APPL_DATA) txPduPtr;
        Std_ReturnType ret;
        uint8          linDriver;
        uint8          linChannel;

        txPduPtr = fra->LinIfPduDirection->LinIfTxPdu;
        /* Get the data part of the frame from upper layer */
        pduInfo.SduDataPtr = &sdu[0];
        pduInfo.SduLength  = fra->LinIfLength;
        /*@req <SWS_LinIf_00225>*/
        ret = txPduPtr->LinIfTxTriggerTransmitUL(txPduPtr->LinIfTxPduRef, &pduInfo);

        if (E_OK == ret)
        {
            /* Set the header information */
            LinIf_SetPduInfo(&chPtr->TxPdu, fra);
            /* Set the response information */
            chPtr->TxPdu.SduPtr = pduInfo.SduDataPtr;

            /* Set the flag of header and response */
            LinIf_SetEvent(chPtr, LINIF_EVENT_HEADER | LINIF_EVENT_RESPONSE);

            /*@req <SWS_LinIf_00226>*/
            /* Send frame */
            linDriver  = LINIF_GET_LIN_DRIVER_ID(ch);
            linChannel = LINIF_GET_LIN_CHANNEL_ID(ch);
            (void)Lin_DriverApi[linDriver].LinSendFrame(linChannel, &chPtr->TxPdu);
        }
        else
        {
            /*@req <SWS_LinIf_00706>*/
            /* When copy data fail from uppler layer,LinIf shall not transmit
               the frame */
        }
    }
}

/******************************************************************************/
/*
 * Brief: Sends a event trigger frame
 * Param-Name[in]: ch, frame
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE)
    LinIf_SendEventTrigger(uint8 ch, P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_APPL_CONST) frame)
{
    P2VAR(LinIf_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA)
    chPtr            = LinIf_GetMasterRuntimeData(ch);
    uint8 linDriver  = LINIF_GET_LIN_DRIVER_ID(ch);
    uint8 linChannel = LINIF_GET_LIN_CHANNEL_ID(ch);

    /* Send header */
    LinIf_SetPduInfo(&chPtr->TxPdu, frame);
    chPtr->TxPdu.SduPtr = NULL_PTR;
    (void)Lin_DriverApi[linDriver].LinSendFrame(linChannel, &chPtr->TxPdu);

    /* Set the flag of header */
    LinIf_SetEvent(chPtr, LINIF_EVENT_HEADER);
}

/******************************************************************************/
/*
 * Brief: Sends a unconditional frame
 * Param-Name[in]: ch, frame
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE)
    LinIf_SendUnconditional(uint8 ch, P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_APPL_CONST) frame)
{
    uint8       sdu[LINIF_SDU_LEN];
    PduInfoType pduInfo;
    P2VAR(LinIf_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA)
    chPtr = LinIf_GetMasterRuntimeData(ch);
    P2CONST(LinIf_TxPduType, AUTOMATIC, LINIF_APPL_DATA) txPduPtr;
    Std_ReturnType ret = E_OK;
    uint8          linDriver;
    uint8          linChannel;

    if (LINIF_TX_PDU == frame->LinIfPduDirection->LinIfPduDirectionId)
    {
        txPduPtr = frame->LinIfPduDirection->LinIfTxPdu;
        /* Get the data part of the frame from upper layer */
        pduInfo.SduDataPtr = &sdu[0];
        pduInfo.SduLength  = frame->LinIfLength;
        /*@req <SWS_LinIf_00225>*/
        ret = txPduPtr->LinIfTxTriggerTransmitUL(txPduPtr->LinIfTxPduRef, &pduInfo);

        if (E_OK == ret)
        {
            /* Set the response information */
            chPtr->TxPdu.SduPtr = pduInfo.SduDataPtr;

            /* Set the flag of response */
            LinIf_SetEvent(chPtr, LINIF_EVENT_RESPONSE);
        }
    }
    else
    {
        chPtr->TxPdu.SduPtr = NULL_PTR;
    }

    if (E_OK == ret)
    {
        /* Set the header information */
        LinIf_SetPduInfo(&chPtr->TxPdu, frame);
        /* Set the flag of header */
        LinIf_SetEvent(chPtr, LINIF_EVENT_HEADER);

        /*@req <SWS_LinIf_00226>*/
        /* Send frame */
        linDriver  = LINIF_GET_LIN_DRIVER_ID(ch);
        linChannel = LINIF_GET_LIN_CHANNEL_ID(ch);
        (void)Lin_DriverApi[linDriver].LinSendFrame(linChannel, &chPtr->TxPdu);
    }
    else
    {
        /*@req <SWS_LinIf_00706>*/
        /* When copy data fail from uppler layer,LinIf shall not transmit
           the frame */
    }
}

/******************************************************************************/
/*
 * Brief: Copies the memory block
 * Param-Name[in]: Src, len
 * Param-Name[out]: Dst
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE) LinIf_MemCopyFixedFrame(
    P2VAR(uint8, AUTOMATIC, LINIF_APPL_DATA) Dst,
    P2CONST(LinIf_FixedFrameSduType, AUTOMATIC, LINIF_APPL_CONST) Src)
{
    uint8 idx;

    for (idx = 0u; idx < LINIF_FIXED_FRAME_MAX_LEN; idx++)
    {
        Dst[idx] = Src->LinIfFixedFrameSduByte[idx].LinIfFixedFrameSduByteVal;
    }
}

/******************************************************************************/
/*
 * Brief: Send a Node Configuration frame
 * Param-Name[in]: ch, frame
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE)
    LinIf_SendNodeConfiguration(uint8 ch, P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_APPL_CONST) frame)
{
    uint8 sdu[LINIF_SDU_LEN];
    P2VAR(LinIf_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA)
    chPtr            = LinIf_GetMasterRuntimeData(ch);
    uint8 linDriver  = LINIF_GET_LIN_DRIVER_ID(ch);
    uint8 linChannel = LINIF_GET_LIN_CHANNEL_ID(ch);

    /* MRF */
    if (LINIF_TX_PDU == frame->LinIfPduDirection->LinIfPduDirectionId)
    {
        /* Set the header information */
        LinIf_SetPduInfo(&chPtr->TxPdu, frame);

        LinIf_MemCopyFixedFrame(sdu, frame->LinIfFixedFrameSdu);
        /* Set the response information */
        chPtr->TxPdu.SduPtr = sdu;

        /* Set the flag of header and response */
        LinIf_SetEvent(chPtr, LINIF_EVENT_HEADER | LINIF_EVENT_RESPONSE);
        /* Set the flag of NC MRF */
        LinIf_SetEvent(chPtr, LINIF_EVENT_NCMRF_SEND);

        /* Send frame */
        (void)Lin_DriverApi[linDriver].LinSendFrame(linChannel, &chPtr->TxPdu);
    }
    /* SRF */
    else if (LINIF_RX_PDU == frame->LinIfPduDirection->LinIfPduDirectionId)
    {
        if (LinIf_IsEvent(chPtr, LINIF_EVENT_NCSRF_SEND))
        {
            /* Set the header information */
            LinIf_SetPduInfo(&chPtr->TxPdu, frame);
            /* Set the response information */
            chPtr->TxPdu.SduPtr = NULL_PTR;

            /* Set the flag of header and response */
            LinIf_SetEvent(chPtr, LINIF_EVENT_HEADER);

            /* Send frame */
            (void)Lin_DriverApi[linDriver].LinSendFrame(linChannel, &chPtr->TxPdu);
        }
        else
        {
            /*@req <SWS_LinIf_00709>*/
            /* Next entry */
            chPtr->CurEntryIndex++;
        }
    }
    else
    {
        /* Only for MISRA-C */
    }
}

#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_MASTER_SUPPORT == STD_ON))
/******************************************************************************/
/*
 * Brief: Sends a MRF frame
 * Param-Name[in]: ch, frame
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE) LinIf_SendMRF(uint8 ch, P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_APPL_CONST) frame)
{
    P2VAR(LinIf_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA)
    chPtr = LinIf_GetMasterRuntimeData(ch);
    uint8          linDriver;
    uint8          linChannel;
    uint8          sdu[LINIF_SDU_LEN];
    Std_ReturnType ret;

    ret = LinTp_MasterGetMRFResponse(ch, sdu);
    if (E_OK == ret)
    {
        /* Set up SDU buffer pointer */
        chPtr->TxPdu.SduPtr = sdu;

        /* Send header and response */
        LinIf_SetPduInfo(&chPtr->TxPdu, frame);
        linDriver  = LINIF_GET_LIN_DRIVER_ID(ch);
        linChannel = LINIF_GET_LIN_CHANNEL_ID(ch);
        (void)Lin_DriverApi[linDriver].LinSendFrame(linChannel, &chPtr->TxPdu);
    }
}

/******************************************************************************/
/*
 * Brief: Sends a SRF frame
 * Param-Name[in]: ch, frame
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE) LinIf_SendSRF(uint8 ch, P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_APPL_CONST) frame)
{
    P2VAR(LinIf_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA)
    chPtr = LinIf_GetMasterRuntimeData(ch);
    uint8 linDriver;
    uint8 linChannel;

    if (FALSE == LinTp_IsStopSRFSendEventSet(ch))
    {
        /* Send header */
        LinIf_SetPduInfo(&chPtr->TxPdu, frame);
        chPtr->TxPdu.SduPtr = NULL_PTR;
        linDriver           = LINIF_GET_LIN_DRIVER_ID(ch);
        linChannel          = LINIF_GET_LIN_CHANNEL_ID(ch);
        (void)Lin_DriverApi[linDriver].LinSendFrame(linChannel, &chPtr->TxPdu);

        /* Set the flag of header */
        LinIf_SetEvent(chPtr, LINIF_EVENT_HEADER);
    }
}
#endif

/******************************************************************************/
/*
 * Brief: Sets the PDU information
 * Param-Name[in]: pdu, frame
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE) LinIf_SetPduInfo(
    P2VAR(Lin_PduType, AUTOMATIC, LINIF_APPL_DATA) pdu,
    P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_APPL_CONST) frame)
{
    /* Save DL and PID */
    pdu->Dl  = frame->LinIfLength;
    pdu->Pid = frame->LinIfProtectedId;

    /* Save checksum type */
    if (LINIF_ENHANCED == frame->LinIfChecksumType)
    {
        /* Enhanced frame */
        pdu->Cs = LIN_ENHANCED_CS;
    }
    else
    {
        /* Classic frame */
        pdu->Cs = LIN_CLASSIC_CS;
    }

    /* Save direction */
    switch (frame->LinIfPduDirection->LinIfPduDirectionId)
    {
    case LINIF_RX_PDU:
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440)
        pdu->Drc = LIN_FRAMERESPONSE_RX;
#else  /* The default version of lin driver is 4.2.2 / 4.3.1 */
        pdu->Drc = LIN_SLAVE_RESPONSE;
#endif /* LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440 */
        break;

    case LINIF_TX_PDU:
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440)
        pdu->Drc = LIN_FRAMERESPONSE_TX;
#else  /* The default version of lin driver is 4.2.2 / 4.3.1 */
        pdu->Drc = LIN_MASTER_RESPONSE;
#endif /* LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440 */
        break;

    case LINIF_SLAVE_TO_SLAVE_PDU:
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440)
        pdu->Drc = LIN_FRAMERESPONSE_IGNORE;
#else  /* The default version of lin driver is 4.2.2 / 4.3.1 */
        pdu->Drc = LIN_SLAVE_TO_SLAVE;
#endif /* LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440 */
        break;

    default:
        /*Other type PDU */
        break;
    }
}

/******************************************************************************/
/*
 * Brief: Handles the last entry
 * Param-Name[in]: ch, chPtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function called by 'LinIf_SwitchNewSchedule'
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE) LinIf_HandleEntryTail(P2VAR(LinIf_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA) chPtr)
{
    /* If it is the last entry */
    if ((chPtr->CurSchedule != NULL_PTR) && (LINIF_IS_ENTRY_TAIL(ch)))
    {
        if (LinIf_IsEvent(chPtr, LINIF_EVENT_SCHEDULE_REQ))
        {
            LinIf_ClrEvent(chPtr, LINIF_EVENT_SCHEDULE_REQ);

            /* Switch to a new schedule */
            chPtr->CurSchedule   = chPtr->RdySchedule;
            chPtr->CurEntryIndex = 0u;

            if (LINIF_RUN_CONTINUOUS == chPtr->RdySchedule->LinIfRunMode)
            {
                chPtr->ResSchedule   = NULL_PTR;
                chPtr->ResEntryIndex = 0u;

                if (LinIf_IsEvent(chPtr, LINIF_EVENT_INTERRUPT_BACK))
                {
                    /* Clear INTERRUPT_BACK flag */
                    LinIf_ClrEvent(chPtr, LINIF_EVENT_INTERRUPT_BACK);
                }
            }
            else
            {
                /* LINIF_RUN_ONCE == chPtr->RdySchedule->LinIfRunMode */
                /* Hold the value of ResSchedule and ResEntryIndex */
            }

            /* Set 'notify' event */
            LinIf_SetEvent(chPtr, LINIF_EVENT_SCHEDULE_CONF);
        }
        else if (LinIf_IsEvent(chPtr, LINIF_EVENT_INTERRUPT_BACK))
        {
            /* Clear INTERRUPT_BACK flag */
            LinIf_ClrEvent(chPtr, LINIF_EVENT_INTERRUPT_BACK);

            /*@req <SWS_LinIf_00397> */
            /* Resume schedule and entry */
            chPtr->CurSchedule   = chPtr->ResSchedule;
            chPtr->CurEntryIndex = chPtr->ResEntryIndex;
            chPtr->ResSchedule   = NULL_PTR;
            chPtr->ResEntryIndex = 0u;

            /* Set 'notify' event */
            LinIf_SetEvent(chPtr, LINIF_EVENT_SCHEDULE_CONF);
        }
        else
        {
            /* Update entry information */
            chPtr->CurEntryIndex = 0u;
        }
    }
}

/******************************************************************************/
/*
 * Brief: Process switch schedule notify
 * Param-Name[in]: ch
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE) LinIf_SwitchScheduleNotify(uint8 ch)
{
    P2VAR(LinIf_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA)
    chPtr = LinIf_GetMasterRuntimeData(ch);
    LinIf_SchHandleType idx;

    /* Check notification event */
    if (LinIf_IsEvent(chPtr, LINIF_EVENT_SCHEDULE_CONF))
    {
        /* Clear notification event */
        LinIf_ClrEvent(chPtr, LINIF_EVENT_SCHEDULE_CONF);

        if (chPtr->CurSchedule == NULL_PTR)
        {
            idx = LINIF_NULL_SCHEDULE_INDEX;
        }
        else
        {
            idx = chPtr->CurSchedule->LinIfScheduleTableIndex;
        }

        /*@req <SWS_LinIf_00495> */
        /* Notify upper */
        USER_SCHEDULE_REQUEST_CONFIRMATION(ch, LINIF_GET_COMM_NETWORK(ch), idx);
    }
}

/******************************************************************************/
/*
 * Brief: Switches to a new schedule
 * Param-Name[in]: ch
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
/* PRQA S 6030 ++ */ /* VL_MTR_LinIf_STMIF */
static FUNC(void, LINIF_CODE) LinIf_SwitchNewSchedule(uint8 ch)
{
    P2VAR(LinIf_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA)
    chPtr = LinIf_GetMasterRuntimeData(ch);
    P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_APPL_CONST) frame;
    uint16 frameIdx;

    if (chPtr->CurSchedule == NULL_PTR)
    {
        /* Setting up a new schedule */
        chPtr->CurSchedule   = chPtr->RdySchedule;
        chPtr->CurEntryIndex = 0u;
        chPtr->ResSchedule   = NULL_PTR;
        chPtr->ResEntryIndex = 0u;

        /* Clear Schedule Request flag */
        LinIf_ClrEvent(chPtr, LINIF_EVENT_SCHEDULE_REQ);
        /* Set 'notify' event */
        LinIf_SetEvent(chPtr, LINIF_EVENT_SCHEDULE_CONF);

        if (LINIF_RUN_ONCE == chPtr->RdySchedule->LinIfRunMode)
        {
            /* If schecule type is RUN_ONCE,should set flag to stop schedule
               after it run_once */
            LinIf_SetEvent(chPtr, LINIF_EVENT_INTERRUPT_BACK);
        }
    }
    else
    {
        if (LINIF_RUN_CONTINUOUS == chPtr->CurSchedule->LinIfRunMode)
        {
            /* Clear Schedule Request flag */
            LinIf_ClrEvent(chPtr, LINIF_EVENT_SCHEDULE_REQ);

            frameIdx = chPtr->RdySchedule->LinIfEntry[0].LinIfFrameRef - LINIF_GET_FRAME_OFS(ch);
            frame    = &LINIF_GET_FRAME(ch, frameIdx);

            /* PRQA S 3415 ++ */ /* VL_LinIf_3415 */
            if (((LINIF_RUN_ONCE == chPtr->RdySchedule->LinIfRunMode)
                 && ((LINIF_MRF != frame->LinIfFrameType) && (LINIF_SRF != frame->LinIfFrameType)))
#if (                                                                  \
    (LINIF_TP_SUPPORTED == STD_ON) && (LINTP_MASTER_SUPPORT == STD_ON) \
    && (STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT))
                || (((LINIF_MRF == frame->LinIfFrameType) || (LINIF_SRF == frame->LinIfFrameType))
                    && ((!LINIF_SUPPORT_TP_TRANSMIT(ch))
                        || (LinTp_GetScheduleChangeDiag(ch)))) /* PRQA S 3415 */ /* VL_LinIf_3415 */
#endif /* LINIF_TP_SUPPORTED == STD_ON && LINTP_MASTER_SUPPORT == STD_ON && STD_ON == \
          LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT */
            )
            {
                /* Set 'interrupt-back' and 'notify' event */
                if (LINIF_RUN_ONCE == chPtr->RdySchedule->LinIfRunMode)
                {
                    LinIf_SetEvent(chPtr, LINIF_EVENT_INTERRUPT_BACK);

                    /* Save schedule information  */
                    chPtr->ResSchedule = chPtr->CurSchedule;

                    /*@req <SWS_LinIf_00485> */
                    /* Save entry information */
                    if (LINIF_CONTINUE_AT_IT_POINT == chPtr->CurSchedule->LinIfResumePosition)
                    {
                        /* PRQA S 2834 ++ */ /* VL_LinIf_Division */
                        chPtr->ResEntryIndex = chPtr->CurEntryIndex % chPtr->CurSchedule->LinIfNumOfEntry;
                        /* PRQA S 2834 -- */
                    }
                    else
                    {
                        chPtr->ResEntryIndex = 0u;
                    }
                }
                LinIf_SetEvent(chPtr, LINIF_EVENT_SCHEDULE_CONF);
            }
            else
            {
                /* Set 'notify' event */
                LinIf_SetEvent(chPtr, LINIF_EVENT_SCHEDULE_CONF);

                chPtr->ResSchedule   = NULL_PTR;
                chPtr->ResEntryIndex = 0u;
            }
            /* PRQA S 3415 -- */

            /*@req <SWS_LinIf_00028> */
            /* Switch to a new schedule */
            chPtr->CurSchedule   = chPtr->RdySchedule;
            chPtr->CurEntryIndex = 0u;
        }
        else
        {
        }
    }
}
/* PRQA S 6030 -- */

/******************************************************************************/
/*
 * Brief: Switches to a new entry
 * Param-Name[in]: ch
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE) LinIf_SwitchNewEntry(uint8 ch)
{
    P2VAR(LinIf_MasterRuntimeType, AUTOMATIC, LINIF_APPL_DATA)
    chPtr = LinIf_GetMasterRuntimeData(ch);

    if (chPtr->CurSchedule != NULL_PTR)
    {
        /* PRQA S 3415 ++ */ /* VL_LinIf_3415 */
        /* If it's not last entry */
        if (!LINIF_IS_ENTRY_TAIL(ch)
#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_MASTER_SUPPORT == STD_ON))
            /* TP buffer not enough */
            && (!LINIF_SUPPORT_TP_TRANSMIT(ch)
                || (!LinTp_IsStopMRFOrSRFSendEventSet(ch))) /* PRQA S 3415 */ /* VL_LinIf_3415 */
#endif
        )
        {
            /* Setting up a new entry */
            chPtr->CurEntry = &chPtr->CurSchedule->LinIfEntry[chPtr->CurEntryIndex];
            chPtr->Timer    = chPtr->CurEntry->LinIfDelay;
        }
        /* PRQA S 3415 -- */
    }
}

/* PRQA S 2814,2824,2813,2889 -- */
/* PRQA S 3432 -- */

#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#endif
