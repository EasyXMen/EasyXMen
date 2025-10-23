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
 **  @file        : LinIf_Master.c
 **  @author      : HuRongbo
 **  @date        :
 **  @vendor      : isoft
 **  @description : LinIf master implementation source file
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "LinIf.h"
#include "IStdLib.h"
#if (STD_ON == LINIF_MASTER_SUPPORT)
#include "LinIf_Master.h"
#include "SchM_LinIf.h"
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif
#if (LINIF_TP_SUPPORTED == STD_ON)
#include "LinTp_Cfg.h"
#if (LINTP_MASTER_SUPPORT == STD_ON)
#include "LinTp_Master.h"
#endif
#endif
#include "LinIf_PartitionVar.h"

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================= internal function declarations ========================================= */
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

#if ((LINIF_MASTER_SPORADIC_FRAME_SUPPORT == STD_ON) || (LINIF_MASTER_EVENT_TRIGGER_FRAME_SUPPORT == STD_ON))
LINIF_LOCAL const LinIf_SubstitutionFramesType* LinIf_GetTransmitPending(const LinIf_FrameType* frame);
#endif

#if (LINIF_MASTER_SPORADIC_FRAME_SUPPORT == STD_ON)
LINIF_LOCAL void LinIf_SendSporadic(LinIf_MasterRuntimeType* masterChRtDataPtr, uint8 ch, const LinIf_FrameType* frame);
#endif

#if (LINIF_MASTER_EVENT_TRIGGER_FRAME_SUPPORT == STD_ON)
LINIF_LOCAL void
    LinIf_SendEventTrigger(LinIf_MasterRuntimeType* masterChRtDataPtr, uint8 ch, const LinIf_FrameType* frame);
#endif

LINIF_LOCAL void
    LinIf_SendUnconditional(LinIf_MasterRuntimeType* masterChRtDataPtr, uint8 ch, const LinIf_FrameType* frame);

#if (LINIF_MASTER_NODE_CONFIG_FRAME_SUPPORT == STD_ON)
LINIF_LOCAL void
    LinIf_SendNodeConfiguration(LinIf_MasterRuntimeType* masterChRtDataPtr, uint8 ch, const LinIf_FrameType* frame);
#endif

#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_MASTER_SUPPORT == STD_ON))

LINIF_LOCAL void LinIf_SendMRF(LinIf_MasterRuntimeType* masterChRtDataPtr, uint8 ch, const LinIf_FrameType* frame);

LINIF_LOCAL void LinIf_SendSRF(LinIf_MasterRuntimeType* masterChRtDataPtr, uint8 ch, const LinIf_FrameType* frame);
#endif

LINIF_LOCAL void LinIf_SetPduInfo(Lin_PduType* pdu, const LinIf_FrameType* frame);

LINIF_LOCAL void LinIf_HandleEntryTail(LinIf_MasterRuntimeType* masterChRtDataPtr);

LINIF_LOCAL void LinIf_SwitchScheduleNotify(LinIf_MasterRuntimeType* masterChRtDataPtr, uint8 ch);

LINIF_LOCAL void LinIf_SwitchNewSchedule(LinIf_MasterRuntimeType* masterChRtDataPtr, uint8 ch);

LINIF_LOCAL void LinIf_SwitchNewEntry(LinIf_MasterRuntimeType* masterChRtDataPtr, uint8 ch);

LINIF_LOCAL void LinIf_RxEventParse(
    uint8                    ch,
    LinIf_MasterRuntimeType* masterChRtDataPtr,
    const uint8*             sduDataPtr,
    const LinIf_FrameType*   frame);

LINIF_LOCAL void LinIf_RxEventErrorHandle(
    Lin_StatusType           st,
    LinIf_MasterRuntimeType* masterChRtDataPtr,
    const LinIf_FrameType*   frame);

LINIF_LOCAL void
    LinIf_MasterTxSuccessHandle(uint8 ch, const LinIf_FrameType* frame, LinIf_MasterRuntimeType* masterChRtDataPtr);

LINIF_LOCAL void LinIf_MasterTxErrorHandle(
    uint8                    ch,
    Lin_StatusType           st,
    LinIf_MasterRuntimeType* masterChRtDataPtr,
    const LinIf_FrameType*   frame);

LINIF_LOCAL void LinIf_PrevTransmitRxPduHandle(
    uint8                    ch,
    Lin_StatusType           st,
    LinIf_MasterRuntimeType* masterChRtDataPtr,
    const LinIf_FrameType*   frame,
    const uint8*             sduDataPtr);

LINIF_LOCAL void LinIf_PrevTransmitTxPduHandle(
    uint8                    ch,
    Lin_StatusType           st,
    LinIf_MasterRuntimeType* masterChRtDataPtr,
    const LinIf_FrameType*   frame);

#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

/* PRQA S 2814,2824,2813,2889 ++ */ /* VL_QAC_DerefNullPtr */

/* ============================================ internal data definition ============================================ */
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

LINIF_LOCAL_INLINE uint16 LinIf_GetLinChannelSleepDelay(uint8 ch)
{
    return LinIf_GetChannel(ch)->SleepModeFrameDelayCnt;
}

LINIF_LOCAL_INLINE LinIf_SchHandleType LinIf_GetScheduleNum(uint8 ch)
{
    return LinIf_GetChannel(ch)->NumOfSchedule;
}

LINIF_LOCAL_INLINE LinIf_SchHandleType LinIf_GetScheduleOfs(uint8 ch)
{
    return LinIf_GetChannel(ch)->ScheduleIndexOffset;
}

LINIF_LOCAL_INLINE const LinIf_ScheduleTableType* LinIf_GetSchedule(uint8 ch, uint8 idx)
{
    return &(LinIf_GetChannel(ch)->ScheduleTable[idx]);
}

LINIF_LOCAL_INLINE LinIf_StartupStateType LinIf_GetStartupState(uint8 ch)
{
    return LinIf_GetChannel(ch)->StartupState;
}

LINIF_LOCAL_INLINE boolean LinIf_IsEntryTail(const LinIf_MasterRuntimeType* masterChRtDataPtr)
{
    return masterChRtDataPtr->CurEntryIndex >= masterChRtDataPtr->CurSchedule->NumOfEntry;
}

LINIF_LOCAL_INLINE void
    LinIf_UserScheduleRequestConfirmation(uint8 ch, NetworkHandleType channel, LinIf_SchHandleType schedule)
{
    if (LinIf_GetChannel(ch)->ScheduleRequestConfirmation != NULL_PTR)
    {
        LinIf_GetChannel(ch)->ScheduleRequestConfirmation(channel, schedule);
    }
}

#if (LINIF_MASTER_NODE_CONFIG_FRAME_SUPPORT == STD_ON)
LINIF_LOCAL_INLINE void LinIf_MemCopyFixedFrame(uint8* Dst, const LinIf_FixedFrameSduType* Src)
{
    for (uint8 idx = 0u; idx < LINIF_FIX_FRAME_BYTE_NUM; idx++)
    {
        Dst[idx] = Src->FixedFrameSduByte[idx].FixedFrameSduByteVal;
    }
}
#endif

#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

/* ============================================ external data definition ============================================ */

/* ========================================== external function definition ========================================== */
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

/* PRQA S 1532,1503 ++ */ /* VL_QAC_OneFunRef */
/**
 * Initializes the LinIf master channel.
 */
void LinIf_MasterInit(ApplicationType applicationId)
{
    for (NetworkHandleType idx = 0u; idx < LINIF_MASTER_CHANNEL_NUMBER; idx++)
    {
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
        if (LinIf_GetChannelApplicationId(idx) != applicationId)
        {
            continue; /* PRQA S 0770 */ /* VL_LinIf_Continue */
        }
#endif

        LinIf_MasterRuntimeType* chPtr = LinIf_GetMasterRtDataPtr(idx);

        /*@req <SWS_LinIf_00507>*/
        /* Set the state of channel */
        if (LINIF_STARTUP_NORMAL == LinIf_GetStartupState(idx))
        {
            chPtr->ChannelState = LINIF_CHANNEL_OPERATIONAL;
        }
        else
        {
            chPtr->ChannelState = LINIF_CHANNEL_SLEEP;
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

    LINIF_NOUSED(applicationId);
}

#if (LINIF_MASTER_SPORADIC_FRAME_SUPPORT == STD_ON)
/**
 * Transmit request process in master node.
 */
Std_ReturnType LinIf_MasterTransmit(NetworkHandleType ch, uint16 frameIdx)
{
    const LinIf_MasterRuntimeType* chPtr = LinIf_GetMasterRtDataPtr(ch);
    Std_ReturnType                 ret;

    if (NULL_PTR == chPtr->CurSchedule)
    {
        /*@req <SWS_LinIf_00719>*/
        ret = E_NOT_OK;
    }
    else
    {
        /*@req <SWS_LinIf_00341>,<SWS_LinIf_00730>*/
        /* Set transmit request flag */
        SchM_Enter_LinIf_ExclusiveArea_Channel();
        LinIf_SetTransmitReq(frameIdx, TRUE);
        SchM_Exit_LinIf_ExclusiveArea_Channel();
        ret = E_OK;
    }

    return ret;
}
#endif

/**
 * Schedule change request process in master node.
 */
Std_ReturnType LinIf_MasterScheduleRequest(NetworkHandleType ch, LinIf_SchHandleType LinIfSchedule)
{
    LinIf_MasterRuntimeType* chPtr    = LinIf_GetMasterRtDataPtr(ch);
    Std_ReturnType           ret      = E_NOT_OK;
    LinIf_SchHandleType      schedule = LinIfSchedule;

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    if ((LINIF_CHANNEL_SLEEP == chPtr->ChannelState)
        || ((LINIF_NULL_SCHEDULE_INDEX != schedule)
            && ((schedule < LinIf_GetScheduleOfs(ch))
                || ((LinIf_GetScheduleNum(ch) + LinIf_GetScheduleOfs(ch)) <= schedule))))
    {
        /*@req <SWS_LinIf_00567>*/
        LinIf_DetReportError(LINIF_SCHEDULEREQUEST_ID, LINIF_E_SCHEDULE_REQUEST_ERROR);
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
            schedule -= LinIf_GetScheduleOfs(ch);
            chPtr->RdySchedule = LinIf_GetSchedule(ch, schedule);
        }

        /* Set schedule request flag */
        LinIf_SetEvent(chPtr, LINIF_EVENT_SCHEDULE_REQ);
        ret = E_OK;
    }

    return ret;
}

/**
 * Go-to-sleep process in master node.
 */
void LinIf_MasterGotoSleep(NetworkHandleType ch)
{
    LinIf_MasterRuntimeType* chPtr = LinIf_GetMasterRtDataPtr(ch);

    /*@req <SWS_LinIf_00488>*/
    /* Set sleep request flag */
    LinIf_SetEvent(chPtr, LINIF_EVENT_SLEEP);
}

/**
 * Process sleep state transmit
 */
/* PRQA S 6070 ++ */ /* VL_MTR_LinIf_STCAL*/
void LinIf_SleepProcess(LinIf_MasterRuntimeType* masterChRtDataPtr, uint8 ch)
/* PRQA S 6070 -- */
{
    uint8*                   sdu   = NULL_PTR;
    LinIf_MasterRuntimeType* chPtr = masterChRtDataPtr;

    if (!(chPtr->Timer == 0u))
    {
        return;
    }

    if (LinIf_IsEvent(chPtr, LINIF_EVENT_SLEEP))
    {
        uint8 linDriver  = LinIf_GetLinDriverId(ch);
        uint8 linChannel = LinIf_GetLinChannelId(ch);
        if (!(LinIf_IsEvent(chPtr, LINIF_EVENT_GOTO_SLEEP)))
        {
            if (LINIF_CHANNEL_SLEEP != chPtr->ChannelState)
            {
                /*@req <SWS_LinIf_00453>*/
                /* Send a goto-sleep request on bus */
                (void)Lin_DriverApi[linDriver].LinGoToSleep(linChannel);
                /* Set sleep frame slot time */
                chPtr->Timer = LinIf_GetLinChannelSleepDelay(ch);

                /* Set goto-sleep flag */
                LinIf_SetEvent(chPtr, LINIF_EVENT_GOTO_SLEEP);
                /* Prepare sleep */
                chPtr->ChannelState = LINIF_CHANNEL_GOTO_SLEEP;
            }
            else
            {
                /*@req <SWS_LinIf_00597>*/
                /* Internal goto-sleep request */
                (void)Lin_DriverApi[linDriver].LinGoToSleepInternal(linChannel);

                /*@req <SWS_LinIf_00557>*/
                /* Notify upper */
                LinIf_UserGotoSleepConfirmation(ch, LinIf_GetComMNetwork(ch), TRUE);
            }

            /*@req <SWS_LinIf_00712>*/
            /* Clear wakeup flag */
            chPtr->WakeupFlag = FALSE;
        }
        else
        {
            Lin_StatusType st = Lin_DriverApi[linDriver].LinGetStatus(linChannel, &sdu);
            if (LIN_CH_SLEEP == st)
            {
                /*@req <SWS_LinIf_00455>*/
                chPtr->ChannelState = LINIF_CHANNEL_SLEEP;

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
                LinTp_MasterChannelInit(ch);
#endif

                /*@req <SWS_LinIf_00495> */
                /* Notify upper change to NULL_SCHEDULE */
                LinIf_UserScheduleRequestConfirmation(ch, LinIf_GetComMNetwork(ch), 0u);

                /*@req <SWS_LinIf_00557>*/
                /* Notify upper */
                LinIf_UserGotoSleepConfirmation(ch, LinIf_GetComMNetwork(ch), TRUE);
            }
            else
            {
                /*@req <SWS_LinIf_00558>*/
                /* Notify upper */
                LinIf_UserGotoSleepConfirmation(ch, LinIf_GetComMNetwork(ch), FALSE);

                /* Go-to-sleep not successful,state back to OPERATIONAL */
                chPtr->ChannelState = LINIF_CHANNEL_OPERATIONAL;
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

/**
 * Process wake-up state transmit
 */
Std_ReturnType LinIf_MasterWakeUp(NetworkHandleType ch)
{
    LinIf_MasterRuntimeType* chPtr      = LinIf_GetMasterRtDataPtr(ch);
    uint8                    linDriver  = LinIf_GetLinDriverId(ch);
    uint8                    linChannel = LinIf_GetLinChannelId(ch);
    Std_ReturnType           ret;

    if (LINIF_CHANNEL_SLEEP == chPtr->ChannelState)
    {
        if (chPtr->WakeupFlag)
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
            chPtr->ChannelState = LINIF_CHANNEL_OPERATIONAL;
        }
    }
    else if (LINIF_CHANNEL_OPERATIONAL == chPtr->ChannelState)
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

    /* Set notify upper layer flag according to request result */
    if (E_OK == ret)
    {
        LinIf_SetEvent(chPtr, LINIF_EVENT_WAKEUP_E_OK);
        /*@req <SWS_LinIf_00720>,<SWS_LinIf_00721>*/
        chPtr->WakeupFlag = FALSE;
    }
    else
    {
        LinIf_SetEvent(chPtr, LINIF_EVENT_WAKEUP_E_NOT_OK);
    }

    return ret;
}

/**
 * Wakeup confirmation process in master node
 */
void LinIf_MasterWakeupConfirmation(EcuM_WakeupSourceType WakeupSource, ApplicationType applicationId)
{
#if (LINIF_LIN_CHANNEL_WAKEUP_SUPPORT == STD_ON)
    /*check all Lin driver channel to find the wake-up source*/
    for (NetworkHandleType ch = 0u; ch < LINIF_MASTER_CHANNEL_NUMBER; ch++)
    {
        EcuM_WakeupSourceType wakeupSource = LinIf_GetLinDriverWakeupSource(ch);
        if (
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
            LinIf_GetChannelApplicationId(ch) == applicationId &&
#endif
            0u != (WakeupSource & wakeupSource))
        {
            /*@req <SWS_LinIf_00716>*/
            LinIf_MasterRuntimeType* chPtr = LinIf_GetMasterRtDataPtr(ch);
            chPtr->WakeupFlag              = TRUE;
            break;
        }
    }
#endif

#if (LINIF_TRCV_DRIVER_SUPPORTED == STD_ON)
#if (LINIF_LIN_TRCV_WAKEUP_SUPPORT == STD_ON)
    /*check all Lin transceiver channel to find the wake-up source*/
    for (NetworkHandleType ch = 0; ch < LINIF_MASTER_CHANNEL_NUMBER; ch++)
    {
        EcuM_WakeupSourceType wakeupSource = LinIf_GetLinTrcvWakeupSource(ch);
        if (
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
            LinIf_GetChannelApplicationId(ch) == applicationId &&
#endif
            0u != (WakeupSource & wakeupSource))
        {
            /*@req <SWS_LinIf_00716>*/
            LinIf_MasterRuntimeType* chPtr = LinIf_GetMasterRtDataPtr(ch);
            chPtr->WakeupFlag              = TRUE;
            break;
        }
    }
#endif
#endif

    LINIF_NOUSED(applicationId);
    LINIF_NOUSED(WakeupSource);
}

/**
 * Process wake-up state transmit
 */
void LinIf_WakeUpProcess(LinIf_MasterRuntimeType* masterChRtDataPtr, uint8 ch)
{
    /* Notify Uppler layer LinIf_Wakeup process result */
    if (LinIf_IsEvent(masterChRtDataPtr, LINIF_EVENT_WAKEUP_E_OK))
    {
        /*@req <SWS_LinIf_00496>*/
        LinIf_UserWakeupConfirmation(ch, LinIf_GetComMNetwork(ch), TRUE);

        LinIf_ClrEvent(masterChRtDataPtr, LINIF_EVENT_WAKEUP_E_OK);
    }
    else if (LinIf_IsEvent(masterChRtDataPtr, LINIF_EVENT_WAKEUP_E_NOT_OK))
    {
        /*@req <SWS_LinIf_00670>*/
        LinIf_UserWakeupConfirmation(ch, LinIf_GetComMNetwork(ch), FALSE);

        LinIf_ClrEvent(masterChRtDataPtr, LINIF_EVENT_WAKEUP_E_NOT_OK);
    }
    else
    {
        /* Only for MISRA-C */
    }
}

/**
 * Process the preview frame transmission
 */
/* PRQA S 6070 ++ */ /* VL_MTR_LinIf_STCAL*/
void LinIf_PrevTransmit(LinIf_MasterRuntimeType* masterChRtDataPtr, uint8 ch)
/* PRQA S 6070 -- */
{
    if (masterChRtDataPtr->CurSchedule == NULL_PTR)
    {
        return;
    }

    const LinIf_FrameType* frame = LinIf_GetFrame(ch, (masterChRtDataPtr->CurEntry->FrameRef - LinIf_GetFrameOfs(ch)));

    if ((LinIf_IsEvent(masterChRtDataPtr, LINIF_EVENT_HEADER | LINIF_EVENT_RESPONSE))
#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_MASTER_SUPPORT == STD_ON))
        || (TRUE == LinTp_IsWaitEventSet(ch))
#endif
    )
    {
        uint8*         sduDataPtr = NULL_PTR;
        uint8          linDriver  = LinIf_GetLinDriverId(ch);
        uint8          linChannel = LinIf_GetLinChannelId(ch);
        Lin_StatusType st         = Lin_DriverApi[linDriver].LinGetStatus(linChannel, &sduDataPtr);

        /* Clear flag of header and response */
        LinIf_ClrEvent(masterChRtDataPtr, LINIF_EVENT_HEADER | LINIF_EVENT_RESPONSE);

        switch (frame->PduDirection->PduDirectionId)
        {
        case LINIF_RX_PDU:
            LinIf_PrevTransmitRxPduHandle(ch, st, masterChRtDataPtr, frame, sduDataPtr);
            break;

        case LINIF_TX_PDU:
            LinIf_PrevTransmitTxPduHandle(ch, st, masterChRtDataPtr, frame);
            break;

        case LINIF_INTERNAL_PDU:
        case LINIF_SLAVE_TO_SLAVE_PDU:
            /* Next entry */
            masterChRtDataPtr->CurEntryIndex++;
            break;
        default:
            /* Other type PDU */
            break;
        }
    }
#if (LINIF_MASTER_SPORADIC_FRAME_SUPPORT == STD_ON)
    else if (LINIF_SPORADIC == frame->FrameType)
    {
        /* Next entry */
        masterChRtDataPtr->CurEntryIndex++;
    }
#endif
}

/**
 * Process the next frame transmission
 */
void LinIf_NextTransmit(LinIf_MasterRuntimeType* masterChRtDataPtr, uint8 ch)
{
    if (masterChRtDataPtr->CurSchedule == NULL_PTR)
    {
        return;
    }

    const LinIf_FrameType* frame = LinIf_GetFrame(ch, (masterChRtDataPtr->CurEntry->FrameRef - LinIf_GetFrameOfs(ch)));

    switch (frame->FrameType)
    {
#if (LINIF_MASTER_SPORADIC_FRAME_SUPPORT == STD_ON)
    /* Sporadic frame */
    case LINIF_SPORADIC:
        LinIf_SendSporadic(masterChRtDataPtr, ch, frame);
        break;
#endif

#if (LINIF_MASTER_EVENT_TRIGGER_FRAME_SUPPORT == STD_ON)
    /* Event-Trigger frame */
    case LINIF_EVENT_TRIGGERED:
        LinIf_SendEventTrigger(masterChRtDataPtr, ch, frame);
        break;
#endif

    /* Unconditional frame */
    case LINIF_UNCONDITIONAL:
        LinIf_SendUnconditional(masterChRtDataPtr, ch, frame);
        break;

#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_MASTER_SUPPORT == STD_ON))
    /* Master request frame */
    case LINIF_MRF:
        LinIf_SendMRF(masterChRtDataPtr, ch, frame);
        break;

    /* Slave response frame */
    case LINIF_SRF:
        LinIf_SendSRF(masterChRtDataPtr, ch, frame);
        break;
#endif

#if (LINIF_MASTER_NODE_CONFIG_FRAME_SUPPORT == STD_ON)
#if (LINIF_NC_OPTIONAL_REQUEST_SUPPORTED == STD_ON)
    case LINIF_ASSIGN_NAD:
    case LINIF_CONDITIONAL:
#endif
    case LINIF_ASSIGN:
    case LINIF_ASSIGN_FRAME_ID_RANGE:
    case LINIF_FREE:
    case LINIF_SAVE_CONFIGURATION:
    case LINIF_UNASSIGN:
        LinIf_SendNodeConfiguration(masterChRtDataPtr, ch, frame);
        break;
#endif

    default:
        /* Other type frame */
        break;
    }
}

/**
 * Updates the schedule strategy
 */
/* PRQA S 6070 ++ */ /* VL_MTR_LinIf_STCAL*/
void LinIf_UpdateSchedule(LinIf_MasterRuntimeType* masterChRtDataPtr, uint8 ch)
/* PRQA S 6070 -- */
{
    /*@req <SWS_LinIf_00393> */
#if (LINIF_MASTER_EVENT_TRIGGER_FRAME_SUPPORT == STD_ON)
    if (LinIf_IsEvent(masterChRtDataPtr, LINIF_EVENT_COLLISION))
    {
        LinIf_ClrEvent(masterChRtDataPtr, LINIF_EVENT_COLLISION);
        /* Set 'collision-back' and 'notify' event */
        LinIf_SetEvent(masterChRtDataPtr, LINIF_EVENT_INTERRUPT_BACK | LINIF_EVENT_SCHEDULE_CONF);

        /* Save schedule and entry information  */
        masterChRtDataPtr->ResSchedule = masterChRtDataPtr->CurSchedule;
        /*@req <SWS_LinIf_00485> */
        /* Save entry information */
        if (LINIF_CONTINUE_AT_IT_POINT == masterChRtDataPtr->CurSchedule->ResumePosition)
        {
            /* PRQA S 2834 ++ */ /* VL_LinIf_Division */
            masterChRtDataPtr->ResEntryIndex =
                masterChRtDataPtr->CurEntryIndex % masterChRtDataPtr->CurSchedule->NumOfEntry;
            /* PRQA S 2834 -- */
        }
        else
        {
            masterChRtDataPtr->ResEntryIndex = 0u;
        }

        /*@req <SWS_LinIf_00176> */
        /* Switch to the collision resolving schedule table  */
        masterChRtDataPtr->CurSchedule =
            LinIf_GetSchedule(ch, masterChRtDataPtr->CollisionResolvingRef - LinIf_GetScheduleOfs(ch));
        masterChRtDataPtr->CurEntryIndex = 0u;
    }
    else
#endif
        if (LinIf_IsEvent(masterChRtDataPtr, LINIF_EVENT_SCHEDULE_REQ))
    {
        /* Uppler layer request NULL_SCHEDULE */
        if (NULL_PTR == masterChRtDataPtr->RdySchedule)
        {
            /*@req <SWS_LinIf_00444> */
            masterChRtDataPtr->CurSchedule   = NULL_PTR;
            masterChRtDataPtr->CurEntry      = NULL_PTR;
            masterChRtDataPtr->CurEntryIndex = 0u;
            masterChRtDataPtr->ResSchedule   = NULL_PTR;
            masterChRtDataPtr->ResEntryIndex = 0u;
            masterChRtDataPtr->RootEvent     = LINIF_EVENT_NONE;
            masterChRtDataPtr->Timer         = 0u;
#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_MASTER_SUPPORT == STD_ON))
            LinTp_MasterChannelInit(ch);
#endif

            /* Clear Schedule Request flag */
            LinIf_ClrEvent(masterChRtDataPtr, LINIF_EVENT_SCHEDULE_REQ);
            /* Set 'notify' event */
            LinIf_SetEvent(masterChRtDataPtr, LINIF_EVENT_SCHEDULE_CONF);
        }
        else
        {
            /* Switch to the new schedule */
            LinIf_SwitchNewSchedule(masterChRtDataPtr, ch);
        }
    }
    else
    {
        /* Nothing to do here */
    }

    /* Handles the last entry */
    LinIf_HandleEntryTail(masterChRtDataPtr);

    /* Process switch schedule notification */
    LinIf_SwitchScheduleNotify(masterChRtDataPtr, ch);

    /* Switch to the new entry */
    LinIf_SwitchNewEntry(masterChRtDataPtr, ch);
}

/**
 * Get current frame type
 */
Std_ReturnType LinIf_GetCurFrameType(NetworkHandleType ch, LinIf_FrameTypeType* frameType)
{
    const LinIf_MasterRuntimeType* chPtr = LinIf_GetMasterRtDataPtr(ch);
    Std_ReturnType                 ret   = E_NOT_OK;

    if (NULL_PTR != chPtr->CurEntry)
    {
        const LinIf_FrameType* frame = LinIf_GetFrame(ch, (chPtr->CurEntry->FrameRef - LinIf_GetFrameOfs(ch)));
        *frameType                   = frame->FrameType;
        ret                          = E_OK;
    }

    return ret;
}

/* PRQA S 1532,1503 -- */
/* ========================================== internal function definitions ==========================================
 */

#if ((LINIF_MASTER_SPORADIC_FRAME_SUPPORT == STD_ON) || (LINIF_MASTER_EVENT_TRIGGER_FRAME_SUPPORT == STD_ON))
/**
 * @brief               Get transmit pending flag
 * @param[in]           frame: LinIf frame pointer
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINIF_LOCAL const LinIf_SubstitutionFramesType* LinIf_GetTransmitPending(const LinIf_FrameType* frame)
{
    const LinIf_SubstitutionFramesType* sub    = &frame->SubstitutionFrames[0];
    const LinIf_SubstitutionFramesType* retSub = NULL_PTR;

    for (uint8 idx = 0u; idx < frame->NumOfSubstitutionFrame; idx++)
    {
        uint16 frameIdx = sub->SubstitutionFrameRef;
        if (LinIf_GetTransmitReq(frameIdx))
        {
            retSub = sub;
            break;
        }
        sub++;
    }

    return retSub;
}
#endif

#if (LINIF_MASTER_SPORADIC_FRAME_SUPPORT == STD_ON)
/**
 * @brief               Sends a sporadic frame.
 * @param[inout]        masterChRtDataPtr: LinIf master channel runtime data
 * @param[in]           ch: LinIf channel id
 * @param[in]           frame: LinIf frame pointer
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINIF_LOCAL void LinIf_SendSporadic(LinIf_MasterRuntimeType* masterChRtDataPtr, uint8 ch, const LinIf_FrameType* frame)
{
    const LinIf_SubstitutionFramesType* sub = LinIf_GetTransmitPending(frame);

    if (sub != NULL_PTR)
    {
        const LinIf_FrameType* fra = LinIf_GetFrame(ch, (sub->SubstitutionFrameRef - LinIf_GetFrameOfs(ch)));
        uint8                  sdu[LINIF_SDU_LEN];

        const LinIf_TxPduType* txPduPtr = fra->PduDirection->TxPdu;
        /* Get the data part of the frame from upper layer */
        PduInfoType pduInfo;
        pduInfo.SduDataPtr = &sdu[0];
        pduInfo.SduLength  = fra->Length;
        /*@req <SWS_LinIf_00225>*/
        Std_ReturnType ret = txPduPtr->TxTriggerTransmitUL(txPduPtr->TxPduRef, &pduInfo);

        if (E_OK == ret)
        {
            /* Set the header information */
            LinIf_SetPduInfo(&masterChRtDataPtr->TxPdu, fra);
            /* Set the response information */
            masterChRtDataPtr->TxPdu.SduPtr = pduInfo.SduDataPtr;

            /* Set the flag of header and response */
            LinIf_SetEvent(masterChRtDataPtr, LINIF_EVENT_HEADER | LINIF_EVENT_RESPONSE);

            /*@req <SWS_LinIf_00226>*/
            /* Send frame */
            uint8 linDriver  = LinIf_GetLinDriverId(ch);
            uint8 linChannel = LinIf_GetLinChannelId(ch);
            (void)Lin_DriverApi[linDriver].LinSendFrame(linChannel, &masterChRtDataPtr->TxPdu);
        }
        else
        {
            /*@req <SWS_LinIf_00706>*/
            /* When copy data fail from uppler layer,LinIf shall not transmit
               the frame */
        }
    }
}
#endif

#if (LINIF_MASTER_EVENT_TRIGGER_FRAME_SUPPORT == STD_ON)
/**
 * @brief               Sends a event trigger frame
 * @param[inout]        masterChRtDataPtr: LinIf master channel runtime data
 * @param[in]           ch: LinIf channel id
 * @param[in]           frame: LinIf frame pointer
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINIF_LOCAL void
    LinIf_SendEventTrigger(LinIf_MasterRuntimeType* masterChRtDataPtr, uint8 ch, const LinIf_FrameType* frame)
{
    uint8 linDriver  = LinIf_GetLinDriverId(ch);
    uint8 linChannel = LinIf_GetLinChannelId(ch);

    /* Send header */
    LinIf_SetPduInfo(&masterChRtDataPtr->TxPdu, frame);
    masterChRtDataPtr->TxPdu.SduPtr = NULL_PTR;
    (void)Lin_DriverApi[linDriver].LinSendFrame(linChannel, &masterChRtDataPtr->TxPdu);

    /* Set the flag of header */
    LinIf_SetEvent(masterChRtDataPtr, LINIF_EVENT_HEADER);
}
#endif

/**
 * @brief               Sends a unconditional frame
 * @param[inout]        masterChRtDataPtr: LinIf master channel runtime data
 * @param[in]           ch: LinIf channel id
 * @param[in]           frame: LinIf frame pointer
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINIF_LOCAL void
    LinIf_SendUnconditional(LinIf_MasterRuntimeType* masterChRtDataPtr, uint8 ch, const LinIf_FrameType* frame)
{
    Std_ReturnType ret                = E_OK;
    uint8          sdu[LINIF_SDU_LEN] = {0};

    if (LINIF_TX_PDU == frame->PduDirection->PduDirectionId)
    {
        const LinIf_TxPduType* txPduPtr = frame->PduDirection->TxPdu;
        /* Get the data part of the frame from upper layer */

        PduInfoType pduInfo;
        pduInfo.SduDataPtr = sdu;
        pduInfo.SduLength  = frame->Length;

        /*@req <SWS_LinIf_00225>*/
        ret = txPduPtr->TxTriggerTransmitUL(txPduPtr->TxPduRef, &pduInfo);

        if (E_OK == ret)
        {
            /* Set the response information */
            masterChRtDataPtr->TxPdu.SduPtr = pduInfo.SduDataPtr;

            /* Set the flag of response */
            LinIf_SetEvent(masterChRtDataPtr, LINIF_EVENT_RESPONSE);
        }
    }
    else
    {
        masterChRtDataPtr->TxPdu.SduPtr = NULL_PTR;
    }

    if (E_OK == ret)
    {
        /* Set the header information */
        LinIf_SetPduInfo(&masterChRtDataPtr->TxPdu, frame);
        /* Set the flag of header */
        LinIf_SetEvent(masterChRtDataPtr, LINIF_EVENT_HEADER);

        /*@req <SWS_LinIf_00226>*/
        /* Send frame */
        uint8 linDriver  = LinIf_GetLinDriverId(ch);
        uint8 linChannel = LinIf_GetLinChannelId(ch);
        (void)Lin_DriverApi[linDriver].LinSendFrame(linChannel, &masterChRtDataPtr->TxPdu);
    }
    else
    {
        /*@req <SWS_LinIf_00706>*/
        /* When copy data fail from uppler layer,LinIf shall not transmit
           the frame */
    }
}

/**
 * @brief               Send a Node Configuration frame
 * @param[inout]        masterChRtDataPtr: LinIf master channel runtime data
 * @param[in]           ch: LinIf channel id
 * @param[in]           frame: LinIf frame pointer
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
#if (LINIF_MASTER_NODE_CONFIG_FRAME_SUPPORT == STD_ON)
LINIF_LOCAL void
    LinIf_SendNodeConfiguration(LinIf_MasterRuntimeType* masterChRtDataPtr, uint8 ch, const LinIf_FrameType* frame)
{
    uint8 linDriver  = LinIf_GetLinDriverId(ch);
    uint8 linChannel = LinIf_GetLinChannelId(ch);

    /* MRF */
    if (LINIF_TX_PDU == frame->PduDirection->PduDirectionId)
    {
        uint8 sdu[LINIF_SDU_LEN];
        /* Set the header information */
        LinIf_SetPduInfo(&masterChRtDataPtr->TxPdu, frame);

        LinIf_MemCopyFixedFrame(sdu, frame->FixedFrameSdu);
        /* Set the response information */
        /* PRQA S 3225 ++ */ /* VL_LinIf_ParamUseAutoObj */
        masterChRtDataPtr->TxPdu.SduPtr = sdu;
        /* PRQA S 3225 -- */

        /* Set the flag of header and response */
        LinIf_SetEvent(masterChRtDataPtr, LINIF_EVENT_HEADER | LINIF_EVENT_RESPONSE);
        /* Set the flag of NC MRF */
        LinIf_SetEvent(masterChRtDataPtr, LINIF_EVENT_NCMRF_SEND);

        /* Send frame */
        (void)Lin_DriverApi[linDriver].LinSendFrame(linChannel, &masterChRtDataPtr->TxPdu);
    }
    /* SRF */
    else if (LINIF_RX_PDU == frame->PduDirection->PduDirectionId)
    {
        if (LinIf_IsEvent(masterChRtDataPtr, LINIF_EVENT_NCSRF_SEND))
        {
            /* Set the header information */
            LinIf_SetPduInfo(&masterChRtDataPtr->TxPdu, frame);
            /* Set the response information */
            masterChRtDataPtr->TxPdu.SduPtr = NULL_PTR;

            /* Set the flag of header and response */
            LinIf_SetEvent(masterChRtDataPtr, LINIF_EVENT_HEADER);

            /* Send frame */
            (void)Lin_DriverApi[linDriver].LinSendFrame(linChannel, &masterChRtDataPtr->TxPdu);
        }
        else
        {
            /*@req <SWS_LinIf_00709>*/
            /* Next entry */
            masterChRtDataPtr->CurEntryIndex++;
        }
    }
    else
    {
        /* Only for MISRA-C */
    }
}
#endif

#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_MASTER_SUPPORT == STD_ON))

/**
 * @brief               Sends a MRF frame
 * @param[in]           ch: LinIf channel id
 * @param[in]           frame: LinIf frame pointer
 * @param[inout]        masterChRtDataPtr: LinIf master channel runtime data
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINIF_LOCAL void LinIf_SendMRF(LinIf_MasterRuntimeType* masterChRtDataPtr, uint8 ch, const LinIf_FrameType* frame)
{
    uint8 sdu[LINIF_SDU_LEN];

    /* PRQA S 3225 ++ */ /* VL_LinIf_ParamUseAutoObj */
    Std_ReturnType ret = LinTp_MasterGetMRFResponse(ch, sdu);
    /* PRQA S 3225 -- */
    if (E_OK == ret)
    {
        /* Set up SDU buffer pointer */
        /* PRQA S 3225 ++ */ /* VL_LinIf_ParamUseAutoObj */
        masterChRtDataPtr->TxPdu.SduPtr = sdu;
        /* PRQA S 3225 -- */

        /* Send header and response */
        LinIf_SetPduInfo(&masterChRtDataPtr->TxPdu, frame);
        uint8 linDriver  = LinIf_GetLinDriverId(ch);
        uint8 linChannel = LinIf_GetLinChannelId(ch);
        (void)Lin_DriverApi[linDriver].LinSendFrame(linChannel, &masterChRtDataPtr->TxPdu);
    }
}

/**
 * @brief               Sends a SRF frame
 * @param[in]           ch: LinIf channel id
 * @param[in]           frame: LinIf frame pointer
 * @param[inout]        masterChRtDataPtr: LinIf master channel runtime data
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINIF_LOCAL void LinIf_SendSRF(LinIf_MasterRuntimeType* masterChRtDataPtr, uint8 ch, const LinIf_FrameType* frame)
{
    if (!LinTp_IsStopSRFSendEventSet(ch))
    {
        /* Send header */
        LinIf_SetPduInfo(&masterChRtDataPtr->TxPdu, frame);
        masterChRtDataPtr->TxPdu.SduPtr = NULL_PTR;
        uint8 linDriver                 = LinIf_GetLinDriverId(ch);
        uint8 linChannel                = LinIf_GetLinChannelId(ch);
        (void)Lin_DriverApi[linDriver].LinSendFrame(linChannel, &masterChRtDataPtr->TxPdu);

        /* Set the flag of header */
        LinIf_SetEvent(masterChRtDataPtr, LINIF_EVENT_HEADER);
    }
}
#endif

/**
 * @brief               Sets the PDU information
 * @param[in]           pdu: Lin pdu
 * @param[in]           frame: LinIf frame pointer
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINIF_LOCAL void LinIf_SetPduInfo(Lin_PduType* pdu, const LinIf_FrameType* frame)
{
    /* Save DL and PID */
    pdu->Dl  = frame->Length;
    pdu->Pid = frame->ProtectedId;

    /* Save checksum type */
    if (LINIF_ENHANCED == frame->ChecksumType)
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
    switch (frame->PduDirection->PduDirectionId)
    {
    case LINIF_RX_PDU:
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440)
        pdu->Drc = LIN_FRAMERESPONSE_RX;
#else /* The default version of lin driver is 4.2.2 / 4.3.1 */
        pdu->Drc = LIN_SLAVE_RESPONSE;
#endif
        break;

    case LINIF_TX_PDU:
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440)
        pdu->Drc = LIN_FRAMERESPONSE_TX;
#else /* The default version of lin driver is 4.2.2 / 4.3.1 */
        pdu->Drc = LIN_MASTER_RESPONSE;
#endif
        break;

    case LINIF_SLAVE_TO_SLAVE_PDU:
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440)
        pdu->Drc = LIN_FRAMERESPONSE_IGNORE;
#else /* The default version of lin driver is 4.2.2 / 4.3.1 */
        pdu->Drc = LIN_SLAVE_TO_SLAVE;
#endif
        break;

    default:
        /*Other type PDU */
        break;
    }
}

/**
 * @brief               Handles the last entry
 * @param[inout]        masterChRtDataPtr: LinIf master channel runtime data
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINIF_LOCAL void LinIf_HandleEntryTail(LinIf_MasterRuntimeType* masterChRtDataPtr)
{
    /* If it is the last entry */
    if ((masterChRtDataPtr->CurSchedule != NULL_PTR) && (LinIf_IsEntryTail(masterChRtDataPtr)))
    {
        if (LinIf_IsEvent(masterChRtDataPtr, LINIF_EVENT_SCHEDULE_REQ))
        {
            LinIf_ClrEvent(masterChRtDataPtr, LINIF_EVENT_SCHEDULE_REQ);

            /* Switch to a new schedule */
            masterChRtDataPtr->CurSchedule   = masterChRtDataPtr->RdySchedule;
            masterChRtDataPtr->CurEntryIndex = 0u;

            if (LINIF_RUN_CONTINUOUS == masterChRtDataPtr->RdySchedule->RunMode)
            {
                masterChRtDataPtr->ResSchedule   = NULL_PTR;
                masterChRtDataPtr->ResEntryIndex = 0u;

                if (LinIf_IsEvent(masterChRtDataPtr, LINIF_EVENT_INTERRUPT_BACK))
                {
                    /* Clear INTERRUPT_BACK flag */
                    LinIf_ClrEvent(masterChRtDataPtr, LINIF_EVENT_INTERRUPT_BACK);
                }
            }
            else
            {
                /* LINIF_RUN_ONCE == chPtr->RdySchedule->RunMode */
                /* Hold the value of ResSchedule and ResEntryIndex */
            }

            /* Set 'notify' event */
            LinIf_SetEvent(masterChRtDataPtr, LINIF_EVENT_SCHEDULE_CONF);
        }
        else if (LinIf_IsEvent(masterChRtDataPtr, LINIF_EVENT_INTERRUPT_BACK))
        {
            /* Clear INTERRUPT_BACK flag */
            LinIf_ClrEvent(masterChRtDataPtr, LINIF_EVENT_INTERRUPT_BACK);

            /*@req <SWS_LinIf_00397> */
            /* Resume schedule and entry */
            masterChRtDataPtr->CurSchedule   = masterChRtDataPtr->ResSchedule;
            masterChRtDataPtr->CurEntryIndex = masterChRtDataPtr->ResEntryIndex;
            masterChRtDataPtr->ResSchedule   = NULL_PTR;
            masterChRtDataPtr->ResEntryIndex = 0u;

            /* Set 'notify' event */
            LinIf_SetEvent(masterChRtDataPtr, LINIF_EVENT_SCHEDULE_CONF);
        }
        else
        {
            /* Update entry information */
            masterChRtDataPtr->CurEntryIndex = 0u;
        }
    }
}

/**
 * @brief               Process switch schedule notify
 * @param[inout]        masterChRtDataPtr: LinIf master channel runtime data
 * @param[in]           ch: LinIf channel id
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINIF_LOCAL void LinIf_SwitchScheduleNotify(LinIf_MasterRuntimeType* masterChRtDataPtr, uint8 ch)
{
    LinIf_SchHandleType idx = 0u;

    /* Check notification event */
    if (LinIf_IsEvent(masterChRtDataPtr, LINIF_EVENT_SCHEDULE_CONF))
    {
        /* Clear notification event */
        LinIf_ClrEvent(masterChRtDataPtr, LINIF_EVENT_SCHEDULE_CONF);

        if (masterChRtDataPtr->CurSchedule == NULL_PTR)
        {
            idx = LINIF_NULL_SCHEDULE_INDEX;
        }
        else
        {
            idx = masterChRtDataPtr->CurSchedule->ScheduleTableIndex;
        }

        /*@req <SWS_LinIf_00495> */
        /* Notify upper */
        LinIf_UserScheduleRequestConfirmation(ch, LinIf_GetComMNetwork(ch), idx);
    }
}

/**
 * @brief               Switches to a new schedule
 * @param[inout]        masterChRtDataPtr: LinIf master channel runtime data
 * @param[in]           ch: LinIf channel id
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
/* PRQA S 6030 ++ */ /* VL_MTR_LinIf_STMIF */
LINIF_LOCAL void LinIf_SwitchNewSchedule(LinIf_MasterRuntimeType* masterChRtDataPtr, uint8 ch)
/* PRQA S 6030 -- */
{
    if (masterChRtDataPtr->CurSchedule == NULL_PTR)
    {
        /* Setting up a new schedule */
        masterChRtDataPtr->CurSchedule   = masterChRtDataPtr->RdySchedule;
        masterChRtDataPtr->CurEntryIndex = 0u;
        masterChRtDataPtr->ResSchedule   = NULL_PTR;
        masterChRtDataPtr->ResEntryIndex = 0u;

        /* Clear Schedule Request flag */
        LinIf_ClrEvent(masterChRtDataPtr, LINIF_EVENT_SCHEDULE_REQ);
        /* Set 'notify' event */
        LinIf_SetEvent(masterChRtDataPtr, LINIF_EVENT_SCHEDULE_CONF);

        if (LINIF_RUN_ONCE == masterChRtDataPtr->RdySchedule->RunMode)
        {
            /* If schecule type is RUN_ONCE,should set flag to stop schedule
               after it run_once */
            LinIf_SetEvent(masterChRtDataPtr, LINIF_EVENT_INTERRUPT_BACK);
        }
    }
    else
    {
        if (LINIF_RUN_CONTINUOUS == masterChRtDataPtr->CurSchedule->RunMode)
        {
            /* Clear Schedule Request flag */
            LinIf_ClrEvent(masterChRtDataPtr, LINIF_EVENT_SCHEDULE_REQ);

            uint16                 frameIdx = masterChRtDataPtr->RdySchedule->Entry[0].FrameRef - LinIf_GetFrameOfs(ch);
            const LinIf_FrameType* frame    = LinIf_GetFrame(ch, frameIdx);
            if (((LINIF_RUN_ONCE == masterChRtDataPtr->RdySchedule->RunMode)
                 && ((LINIF_MRF != frame->FrameType) && (LINIF_SRF != frame->FrameType)))
#if (                                                                  \
    (LINIF_TP_SUPPORTED == STD_ON) && (LINTP_MASTER_SUPPORT == STD_ON) \
    && (STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT))
                || (((LINIF_MRF == frame->FrameType) || (LINIF_SRF == frame->FrameType))
                    && (TRUE == LinTp_GetScheduleChangeDiag(ch)))
#endif
            )
            {
                /* Set 'interrupt-back' and 'notify' event */
                if (LINIF_RUN_ONCE
                    == masterChRtDataPtr->RdySchedule->RunMode) /* PRQA S 2991,2995 */ /* VL_LinIf_Logical */
                {
                    LinIf_SetEvent(masterChRtDataPtr, LINIF_EVENT_INTERRUPT_BACK);

                    /* Save schedule information  */
                    masterChRtDataPtr->ResSchedule = masterChRtDataPtr->CurSchedule;

                    /*@req <SWS_LinIf_00485> */
                    /* Save entry information */
                    if (LINIF_CONTINUE_AT_IT_POINT == masterChRtDataPtr->CurSchedule->ResumePosition)
                    {
                        /* PRQA S 2834 ++ */ /* VL_LinIf_Division */
                        masterChRtDataPtr->ResEntryIndex =
                            masterChRtDataPtr->CurEntryIndex % masterChRtDataPtr->CurSchedule->NumOfEntry;
                        /* PRQA S 2834 -- */
                    }
                    else
                    {
                        masterChRtDataPtr->ResEntryIndex = 0u;
                    }
                }
                LinIf_SetEvent(masterChRtDataPtr, LINIF_EVENT_SCHEDULE_CONF);
            }
            else
            {
                /* Set 'notify' event */
                LinIf_SetEvent(masterChRtDataPtr, LINIF_EVENT_SCHEDULE_CONF);

                masterChRtDataPtr->ResSchedule   = NULL_PTR;
                masterChRtDataPtr->ResEntryIndex = 0u;
            }

            /*@req <SWS_LinIf_00028> */
            /* Switch to a new schedule */
            masterChRtDataPtr->CurSchedule   = masterChRtDataPtr->RdySchedule;
            masterChRtDataPtr->CurEntryIndex = 0u;
        }
        else
        {
        }
    }
}

/**
 * @brief               Switches to a new entry
 * @param[inout]        masterChRtDataPtr: LinIf master channel runtime data
 * @param[in]           ch: LinIf channel id
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINIF_LOCAL void LinIf_SwitchNewEntry(LinIf_MasterRuntimeType* masterChRtDataPtr, uint8 ch)
{
    if (masterChRtDataPtr->CurSchedule != NULL_PTR)
    {
        /* If it's not last entry */
        if (!LinIf_IsEntryTail(masterChRtDataPtr)
#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_MASTER_SUPPORT == STD_ON))
            /* TP buffer not enough */
            && (FALSE == LinTp_IsStopMRFOrSRFSendEventSet(ch))
#endif
        )
        {
            /* Setting up a new entry */
            masterChRtDataPtr->CurEntry = &masterChRtDataPtr->CurSchedule->Entry[masterChRtDataPtr->CurEntryIndex];
            masterChRtDataPtr->Timer    = masterChRtDataPtr->CurEntry->Delay;
        }
    }

    LINIF_NOUSED(ch);
}

/**
 * @brief               Master node parse linif message
 * @param[inout]        masterChRtDataPtr: LinIf master channel runtime data
 * @param[in]           ch: LinIf channel id
 * @param[in]           sduDataPtr: receive sdu data buffer
 * @param[in]           frame: current frame
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
/* PRQA S 3673 ++ */ /* VL_LinIf_ConstPointer */
LINIF_LOCAL void LinIf_RxEventParse(
    uint8                    ch,
    LinIf_MasterRuntimeType* masterChRtDataPtr,
    const uint8*             sduDataPtr,
    const LinIf_FrameType*   frame)
/* PRQA S 3673 -- */
{
#if (LINIF_MASTER_NODE_CONFIG_FRAME_SUPPORT == STD_ON)
    if (!(LinIf_IsEvent(masterChRtDataPtr, LINIF_EVENT_NCSRF_SEND)))
#endif
    {
        uint8       sduPtr[LINIF_SDU_LEN] = {0};
        PduInfoType pduInfo;
        pduInfo.SduDataPtr = sduPtr;
        pduInfo.SduLength  = frame->Length;

        /* Save PDU information */
        (void)IStdLib_MemCpy(&sduPtr[0], sduDataPtr, frame->Length);

#if (LINIF_MASTER_EVENT_TRIGGER_FRAME_SUPPORT == STD_ON)
        /* event-triggered frame */
        if (frame->FrameType == LINIF_EVENT_TRIGGERED)
        {
            /* get the corresponding collision resolving schedule table */
            uint8 idx = masterChRtDataPtr->CurEntry->CollisionResolvingRef - LinIf_GetScheduleOfs(ch);
            const LinIf_ScheduleTableType* triSchedule = LinIf_GetSchedule(ch, idx);
            for (uint16 frameIdx = 0u; frameIdx < triSchedule->NumOfEntry; frameIdx++)
            {
                /* PRQA S 0488 ++ */ /* VL_LinIf_PointerArithmetic */
                const LinIf_EntryType* triEntry = (triSchedule->Entry) + frameIdx;
                /* PRQA S 0488 -- */
                const LinIf_FrameType* eventTriggerFrame = LinIf_GetGlobalFrame(triEntry->FrameRef);
                /* the PID of the unconditional frame in the first byte of the response data.*/
                if (pduInfo.SduDataPtr[0] == eventTriggerFrame->FrameId)
                {
                    /*@req <SWS_LinIf_00033> */
                    /* Notify upper */
                    const LinIf_RxPduType* rxPduPtr = eventTriggerFrame->PduDirection->RxPdu;
                    rxPduPtr->RxIndicationUL(rxPduPtr->RxPduRef, &pduInfo);
                    break;
                }
            }
        }
        else
#endif
        {
            /*@req <SWS_LinIf_00033> */
            /* Notify upper */
            const LinIf_RxPduType* rxPduPtr = frame->PduDirection->RxPdu;
            rxPduPtr->RxIndicationUL(rxPduPtr->RxPduRef, &pduInfo);
        }
    }
#if (LINIF_MASTER_NODE_CONFIG_FRAME_SUPPORT == STD_ON)
    else
    {
        /*@req <SWS_LinIf_00404> */
        /* Clear the flag of NC SRF header */
        LinIf_ClrEvent(masterChRtDataPtr, LINIF_EVENT_NCSRF_SEND);
    }
#endif

    LINIF_NOUSED(ch);
    LINIF_NOUSED(masterChRtDataPtr);
}

/**
 * @brief               Master node receive error process
 * @param[inout]        masterChRtDataPtr: LinIf master channel runtime data
 * @param[in]           st: error status
 * @param[in]           frame: current frame
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
/* PRQA S 3673 ++ */ /* VL_LinIf_ConstPointer */
LINIF_LOCAL void LinIf_RxEventErrorHandle(
    Lin_StatusType           st,
    LinIf_MasterRuntimeType* masterChRtDataPtr,
    const LinIf_FrameType*   frame)
/* PRQA S 3673 -- */
{
    switch (frame->FrameType)
    {
    case LINIF_UNCONDITIONAL:
/*@req <SWS_LinIf_00254>,<SWS_LinIf_00466> */
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
        /* Notify upper */
        LinIf_ReportRuntimeError(LINIF_MAINFUNCTION_ID, LINIF_E_RESPONSE);
#endif
        break;

#if (LINIF_MASTER_EVENT_TRIGGER_FRAME_SUPPORT == STD_ON)
    case LINIF_EVENT_TRIGGERED:
        if ((st == LIN_RX_ERROR) || (st == LIN_RX_BUSY))
        {
            /*@req <SWS_LinIf_00259>,<SWS_LinIf_00588>,
                <SWS_LinIf_00176> */
            /* Set collision resolving flag */
            LinIf_SetEvent(masterChRtDataPtr, LINIF_EVENT_COLLISION);
            masterChRtDataPtr->CollisionResolvingRef = masterChRtDataPtr->CurEntry->CollisionResolvingRef;
        }
        break;
#endif

#if (LINIF_MASTER_NODE_CONFIG_FRAME_SUPPORT == STD_ON)
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
        LinIf_ClrEvent(masterChRtDataPtr, LINIF_EVENT_NCSRF_SEND);
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
        /*@req <SWS_LinIf_00405> */
        LinIf_DetReportError(LINIF_MAINFUNCTION_ID, LINIF_E_NC_NO_RESPONSE);
#endif
        break;
#endif

    default:
        /* Other Frame Type */
        break;
    }
    LINIF_NOUSED(st);
    LINIF_NOUSED(masterChRtDataPtr);
}

/**
 * @brief               Processing of messages sent by the master node.
 * @param[inout]        masterChRtDataPtr: LinIf master channel runtime data
 * @param[in]           frame: current frame
 * @param[in]           ch: LinIf channel id
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
/* PRQA S 6070 ++ */ /* VL_MTR_LinIf_STCAL*/
LINIF_LOCAL void
    LinIf_MasterTxSuccessHandle(uint8 ch, const LinIf_FrameType* frame, LinIf_MasterRuntimeType* masterChRtDataPtr)
/* PRQA S 6070 -- */
{
    const LinIf_TxPduType* txPduPtr;
#if (LINIF_MASTER_SPORADIC_FRAME_SUPPORT == STD_ON)
    const LinIf_SubstitutionFramesType* sub;
    const LinIf_FrameType*              fra;
    uint16                              frameIdx;
#endif

    switch (frame->FrameType)
    {
    case LINIF_UNCONDITIONAL:
        /*@req <SWS_LinIf_00128> */
        /* Notify upper */
        txPduPtr = frame->PduDirection->TxPdu;
        txPduPtr->TxConfirmationUL(txPduPtr->TxPduRef, E_OK);

        /* Next entry */
        masterChRtDataPtr->CurEntryIndex++;

        break;

#if (LINIF_MASTER_SPORADIC_FRAME_SUPPORT == STD_ON)
    case LINIF_SPORADIC:
        sub      = LinIf_GetTransmitPending(frame);
        frameIdx = sub->SubstitutionFrameRef;
        fra      = LinIf_GetFrame(ch, (frameIdx - LinIf_GetFrameOfs(ch)));

        /*@req <SWS_LinIf_00128> */
        /* Notify upper */
        txPduPtr = fra->PduDirection->TxPdu;
        txPduPtr->TxConfirmationUL(txPduPtr->TxPduRef, E_OK);

        /* Next entry */
        masterChRtDataPtr->CurEntryIndex++;

        /*@req <SWS_LinIf_00463> */
        /* A sporadic frame is transmitted successfully,
            reset the pending flag */
        SchM_Enter_LinIf_ExclusiveArea_Channel();
        LinIf_SetTransmitReq(frameIdx, FALSE);
        SchM_Exit_LinIf_ExclusiveArea_Channel();
        break;
#endif

#if (LINIF_MASTER_NODE_CONFIG_FRAME_SUPPORT == STD_ON)
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
        masterChRtDataPtr->CurEntryIndex++;
        LinIf_ClrEvent(masterChRtDataPtr, LINIF_EVENT_NCMRF_SEND);
        LinIf_SetEvent(masterChRtDataPtr, LINIF_EVENT_NCSRF_SEND);
        break;
#endif

    default:
        /* Other Frame Type */
        break;
    }
    LINIF_NOUSED(ch);
}

/**
 * @brief               Handling of message sending failures for the master node.
 * @param[inout]        masterChRtDataPtr: LinIf master channel runtime data
 * @param[in]           frame: current frame
 * @param[in]           st: error status
 * @param[in]           ch: LinIf channel id
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINIF_LOCAL void LinIf_MasterTxErrorHandle(
    uint8                    ch,
    Lin_StatusType           st,
    LinIf_MasterRuntimeType* masterChRtDataPtr,
    const LinIf_FrameType*   frame)
{
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    if ((st == LIN_TX_ERROR) || (st == LIN_TX_BUSY))
    {
        /*@req <SWS_LinIf_00036>,<SWS_LinIf_00465> */
        /* Notify upper */
        LinIf_ReportRuntimeError(LINIF_MAINFUNCTION_ID, LINIF_E_RESPONSE);
    }
#endif

#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_MASTER_SUPPORT == STD_ON))
    if (frame->FrameType == LINIF_MRF)
    {
        LinTp_MasterTxErrorHandle(ch, st);
    }
    else
#endif
    {
        /* @req <SWS_LinIf_00728> */
#if (LINIF_MASTER_SPORADIC_FRAME_SUPPORT == STD_ON)
        if (frame->FrameType == LINIF_SPORADIC)
        {
            const LinIf_SubstitutionFramesType* sub      = LinIf_GetTransmitPending(frame);
            uint16                              frameIdx = sub->SubstitutionFrameRef;
            const LinIf_FrameType*              fra      = LinIf_GetFrame(ch, (frameIdx - LinIf_GetFrameOfs(ch)));
            const LinIf_TxPduType*              txPduPtr = fra->PduDirection->TxPdu;
            txPduPtr->TxConfirmationUL(txPduPtr->TxPduRef, E_NOT_OK);
        }
        else
#endif
        {
            const LinIf_TxPduType* txPduPtr = frame->PduDirection->TxPdu;
            txPduPtr->TxConfirmationUL(txPduPtr->TxPduRef, E_NOT_OK);
        }

        /* Next entry */
        masterChRtDataPtr->CurEntryIndex++;
        LinIf_ClrEvent(masterChRtDataPtr, LINIF_EVENT_NCMRF_SEND);
    }
    LINIF_NOUSED(ch);
}

/**
 * @brief               Master node parse linif message
 * @param[inout]        masterChRtDataPtr: LinIf master channel runtime data
 * @param[in]           ch: LinIf channel id
 * @param[in]           sduDataPtr: receive sdu data buffer
 * @param[in]           frame: current frame
 * @param[in]           st: lin driver status
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINIF_LOCAL void LinIf_PrevTransmitRxPduHandle(
    uint8                    ch,
    Lin_StatusType           st,
    LinIf_MasterRuntimeType* masterChRtDataPtr,
    const LinIf_FrameType*   frame,
    const uint8*             sduDataPtr)
{
    if (st == LIN_RX_OK)
    {
#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_MASTER_SUPPORT == STD_ON))
        if (frame->FrameType == LINIF_SRF)
        {
            /* Receiving parse */
            LinTp_RxEventParse(ch, sduDataPtr);
        }
        else
#endif
        {
            masterChRtDataPtr->CurEntryIndex++;
            LinIf_RxEventParse(ch, masterChRtDataPtr, sduDataPtr, frame);
        }
    }
    else
    {
        masterChRtDataPtr->CurEntryIndex++;

        LinIf_RxEventErrorHandle(st, masterChRtDataPtr, frame);
    }
    LINIF_NOUSED(ch);
}

/**
 * @brief               Master node parse linif message
 * @param[inout]        masterChRtDataPtr: LinIf master channel runtime data
 * @param[in]           ch: LinIf channel id
 * @param[in]           frame: current frame
 * @param[in]           st: lin driver status
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINIF_LOCAL void LinIf_PrevTransmitTxPduHandle(
    uint8                    ch,
    Lin_StatusType           st,
    LinIf_MasterRuntimeType* masterChRtDataPtr,
    const LinIf_FrameType*   frame)
{
    if (st == LIN_TX_OK)
    {
#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_MASTER_SUPPORT == STD_ON))
        if (frame->FrameType == LINIF_MRF)
        {
            LinTp_MasterTxSuccessHandle(ch);
        }
        else
#endif
        {
            LinIf_MasterTxSuccessHandle(ch, frame, masterChRtDataPtr);
        }
    }
    else
    {
        LinIf_MasterTxErrorHandle(ch, st, masterChRtDataPtr, frame);
    }
    LINIF_NOUSED(ch);
}

#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

/* PRQA S 2814,2824,2813,2889 -- */

#endif
