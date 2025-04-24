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
**  FILENAME    : LinIf.c                                                     **
**                                                                            **
**  Created on  :                                                             **
**  Author      : HuRongbo                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation for LinIf                                    **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
#ifndef LINIF_INTERNAL_H
#define LINIF_INTERNAL_H

#include "LinIf_Types.h"
/*@req <SWS_LinIf_00497> */
#include "PduR_LinIf.h"
/*@req <SWS_LinIf_00638> */
#include "Lin_GeneralTypes.h"
/*@req <SWS_LinIf_00556> */
#include "LinSM_Cbk.h"
/*@req <SWS_LinIf_00690> */
#include "EcuM.h"
#include "SchM_LinIf.h"
/*@req <SWS_LinIf_00498> */
#include "Det.h"
/*@req <SWS_LinIf_00555> */
#if (LINIF_TRCV_DRIVER_SUPPORTED == STD_ON)
#include "LinTrcv.h"
#endif
#include "istd_lib.h"
#include "LinIf.h"

/*******************************************************************************
**                            Macros Definitions                              **
*******************************************************************************/
/* PC/LT/PB configuration */
#define LINIF_GET_CHANNEL(ch)                              LinIf_ConfigPtr->LinIfChannel[ch]
#define LINIF_GET_SUB_FRAME(idx)                           LinIf_ConfigPtr->LinIfSubstitution[idx]
#define LINIF_GET_SUB_NUM()                                LinIf_ConfigPtr->LinIfNumOfSubstitution
#define LINIF_GET_TXPDU(idx)                               LinIf_ConfigPtr->LinIfTxPdu[idx]
#define LINIF_GET_TXPDU_NUM()                              LinIf_ConfigPtr->LinIfNumOfTxPdu
#define LINIF_GET_GLOBAL_FRAME(idx)                        LinIf_ConfigPtr->LinIfFrame[idx]
#define LINIF_GET_LIN_CHANNEL_SLEEPDELAY(ch)               LINIF_GET_CHANNEL(ch).LinIfSleepModeFrameDelayCnt
#define LINIF_GET_SCHEDULE_NUM(ch)                         LINIF_GET_CHANNEL(ch).LinIfNumOfSchedule
#define LINIF_GET_FRAME_NUM(ch)                            LINIF_GET_CHANNEL(ch).LinIfNumOfFrame
#define LINIF_GET_SCHEDULE_OFS(ch)                         LINIF_GET_CHANNEL(ch).LinIfScheduleIndexOffset
#define LINIF_GET_FRAME_OFS(ch)                            LINIF_GET_CHANNEL(ch).LinIfFrameIndexOffset
#define LINIF_GET_TRANSMIT_REQ(idx)                        LINIF_GET_GLOBAL_FRAME(idx).LinIfIsTransmitPending[0]
#define LINIF_GET_SCHEDULE(ch, idx)                        LINIF_GET_CHANNEL(ch).LinIfScheduleTable[idx]
#define LINIF_GET_FRAME(ch, idx)                           LINIF_GET_CHANNEL(ch).LinIfFrame[idx]
#define LINIF_GET_LIN_CHANNEL_ID(ch)                       LINIF_GET_CHANNEL(ch).LinIfChannelRef->LinChannelIdRef
#define LINIF_GET_LIN_DRIVER_ID(ch)                        LINIF_GET_CHANNEL(ch).LinIfChannelRef->LinDriverId
#define LINIF_GET_LIN_DRIVER_WAKEUP_SOURCE(ch)             LINIF_GET_CHANNEL(ch).LinIfChannelRef->WakeUpSource
#define LINIF_GET_LIN_TRCV_WAKEUP_SOURCE(ch)               LINIF_GET_CHANNEL(ch).LinIfTransceiverDrvConfig->WakeUpSource
#define LINIF_GET_COMM_NETWORK(ch)                         LINIF_GET_CHANNEL(ch).LinIfComMNetworkHandleRef
#define LINIF_GET_STARTUP_STATE(ch)                        LINIF_GET_CHANNEL(ch).LinIfStartupState
#define LINIF_GET_NODETYPE(ch)                             LINIF_GET_CHANNEL(ch).LinIfNodeType->LinIfNodeType
#define USER_WAKEUP_CONFIRMATION(ch, channel, success)     LINIF_GET_CHANNEL(ch).WakeupConfirmation(channel, success)
#define USER_GOTO_SLEEP_CONFIRMATION(ch, channel, success) LINIF_GET_CHANNEL(ch).GotoSleepConfirmation(channel, success)
#define USER_SCHEDULE_REQUEST_CONFIRMATION(ch, channel, schedule) \
    LINIF_GET_CHANNEL(ch).ScheduleRequestConfirmation(channel, schedule)

/* Check whether the tail of entry */
#define LINIF_IS_ENTRY_TAIL(ch) (chPtr->CurEntryIndex >= chPtr->CurSchedule->LinIfNumOfEntry)

/* Handle events of LINIF */
#define LINIF_EVENT_MASK            0xFFFFu
#define LINIF_EVENT_NONE            0x0000u
#define LINIF_EVENT_SLEEP           0x0001u
#define LINIF_EVENT_GOTO_SLEEP      0x0002u
#define LINIF_EVENT_WAKEUP_E_OK     0x0004u
#define LINIF_EVENT_WAKEUP_E_NOT_OK 0x0008u
#define LINIF_EVENT_COLLISION       0x0010u
#define LINIF_EVENT_INTERRUPT_BACK  0x0020u
#define LINIF_EVENT_HEADER          0x0040u
#define LINIF_EVENT_RESPONSE        0x0080u
#define LINIF_EVENT_SCHEDULE_CONF   0x0100u
#define LINIF_EVENT_SCHEDULE_REQ    0x0200u
#define LINIF_EVENT_NCMRF_SEND      0x0400u
#define LINIF_EVENT_NCSRF_SEND      0x0800u

#define LINIF_SET_EVENT(e)          chPtr->RootEvent |= (e)
#define LINIF_CLR_EVENT(e)          chPtr->RootEvent &= ~(e)
#define LINIF_IS_EVENT(e)           (chPtr->RootEvent & (e)) != 0u

#define LINIF_CH(ch)                &LinIf_MasterChRtData[ch]

/* Return the minimum value */
#define LINIF_MIN(a, b) ((a) < (b)) ? (a) : (b)

/* Return the highest priority of schedule */
#define LINIF_HIGH_PRIORITY(a, b) ((a)->LinIfSchedulePriority <= (b)->LinIfSchedulePriority) ? (a) : (b)

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
#define LinIf_Det_ReportError(ApiId, ErrorId) (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID, ApiId, ErrorId);
#endif

#define LinIf_ReportRuntimeError(ApiId, ErrorId) \
    (void)Det_ReportRuntimeError(LINIF_MODULE_ID, LINIF_INSTANCE_ID, ApiId, ErrorId);
/*******************************************************************************
**                      Runtime Type Definitions                              **
*******************************************************************************/
typedef enum
{
    LINIF_UNINIT,
    LINIF_INIT
} LinIf_StatusType;

typedef enum
{
    LINIF_CHANNEL_UNINIT,
    LINIF_CHANNEL_OPERATIONAL,
    LINIF_CHANNEL_GOTO_SLEEP,
    LINIF_CHANNEL_SLEEP
} LinIf_ChannelStateType;

/*******************************************************************************
**                      Private Variable Declarations                         **
*******************************************************************************/

/* The status of LINIF */
extern VAR(LinIf_StatusType, LINIF_VAR) LinIf_Status;

/* Global configuration pointer of LINIF */
extern P2CONST(LinIf_ConfigType, AUTOMATIC, LINIF_APPL_CONST) LinIf_ConfigPtr;

#endif /* LINIF_INTERNAL_H */
