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
 **  @file        : LinIf.c
 **  @author      : HuRongbo
 **  @date        :
 **  @vendor      : isoft
 **  @description : LinIf internal header for internal API declarations and definitions
 **
 ***********************************************************************************************************************/

#ifndef LINIF_INTERNAL_H_
#define LINIF_INTERNAL_H_

/* =================================================== inclusions =================================================== */
#include "LinIf_Types.h"
/*@req <SWS_LinIf_00497> */
#include "PduR_LinIf.h"
/*@req <SWS_LinIf_00638> */
#include "Lin_GeneralTypes.h"
/*@req <SWS_LinIf_00556> */
#include "LinSM.h"
/*@req <SWS_LinIf_00690> */
#include "EcuM.h"
#include "SchM_LinIf.h"
/*@req <SWS_LinIf_00498> */
#include "Det.h"
/*@req <SWS_LinIf_00555> */
#if (LINIF_TRCV_DRIVER_SUPPORTED == STD_ON)
#include "LinTrcv.h"
#endif
#include "IStdLib.h"
#include "LinIf_PartitionVar.h"
#include "LinIf_PBcfg.h"
#include "LinIf.h"

/* ===================================================== macros ===================================================== */
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

#define LINIF_NC_FRAME_PID_0X3C 0x3Cu
#define LINIF_NC_FRAME_PID_0XFE 0xFEu
#define LINIF_NC_FRAME_PID_0X7D 0x7Du
#define LINIF_NC_FRAME_PID_0XBF 0xBFu
#define LINIF_NC_FRAME_PID_0XFF 0xFFu
#define LINIF_NC_FRAME_PID_0X06 0x06u
#define LINIF_NC_FRAME_PID_0X04 0x04u

#define LINIF_NC_NEGTIVE_RESPONSE_PCI_VALUE     0x03u
#define LINIF_NC_NEGTIVE_RESPONSE_DATA2_VALUE   0x12u
#define LINIF_NC_RESPONSE_POSITIVE_RESPONSE_SID 0x40u
#define LINIF_NC_READBYIDENTIFIER_FUNCTION_ID   0x00u
#define LINIF_NC_READBYIDENTIFIER_NEGTIVE       0x02u

#define LINIF_NC_ASSIGN_FRAME_ID_DATA_LEN 0x04u

#if !defined(LINIF_LOCAL)
#define LINIF_LOCAL static /* PRQA S 3414 */ /* VL_QAC_FctLikeMacro */
#endif

#if !defined(LINIF_LOCAL_INLINE)
#define LINIF_LOCAL_INLINE static inline
#endif

/* =========================================== internal data declarations =========================================== */
/* Global configuration pointer of LINIF */
extern const LinIf_ConfigType* LinIf_ConfigPtr;

/* =========================================== inline function definition =========================================== */

LINIF_LOCAL_INLINE const LinIf_ChannelType* LinIf_GetChannel(NetworkHandleType ch)
{
    return &(LinIf_ConfigPtr->Channel[ch]);
}

LINIF_LOCAL_INLINE const LinIf_SubstitutionFramesType* LinIf_GetSubFrame(uint16 idx)
{
    return &(LinIf_ConfigPtr->Substitution[idx]);
}

LINIF_LOCAL_INLINE uint16 LinIf_GetSubNum(void)
{
    return LinIf_ConfigPtr->NumOfSubstitution;
}

LINIF_LOCAL_INLINE const LinIf_TxPduType* LinIf_GetTxPdu(uint16 idx)
{
    return &(LinIf_ConfigPtr->TxPdu[idx]);
}

LINIF_LOCAL_INLINE uint16 LinIf_GetTxPduNum(void)
{
    return LinIf_ConfigPtr->NumOfTxPdu;
}

LINIF_LOCAL_INLINE const LinIf_FrameType* LinIf_GetGlobalFrame(uint16 idx)
{
    return &(LinIf_ConfigPtr->Frame[idx]);
}

LINIF_LOCAL_INLINE uint8 LinIf_GetMasterFrameNum(void)
{
    return LinIf_ConfigPtr->NumOfMasterFrame;
}

LINIF_LOCAL_INLINE uint8 LinIf_GetSlaveFrameNum(void)
{
    return LinIf_ConfigPtr->NumOfSlaveFrame;
}

LINIF_LOCAL_INLINE uint8 LinIf_GetFrameNum(uint8 ch)
{
    return LinIf_GetChannel(ch)->NumOfFrame;
}

LINIF_LOCAL_INLINE uint16 LinIf_GetFrameOfs(uint8 ch)
{
    return LinIf_GetChannel(ch)->FrameIndexOffset;
}

LINIF_LOCAL_INLINE boolean LinIf_GetTransmitReq(uint16 idx)
{
    return LinIf_GetGlobalFrame(idx)->IsTransmitPending[0];
}

LINIF_LOCAL_INLINE void LinIf_SetTransmitReq(uint16 idx, boolean param)
{
    LinIf_GetGlobalFrame(idx)->IsTransmitPending[0] = param;
}

LINIF_LOCAL_INLINE const LinIf_FrameType* LinIf_GetFrame(uint8 ch, uint16 idx)
{
    return &(LinIf_GetChannel(ch)->Frame[idx]);
}

LINIF_LOCAL_INLINE uint8 LinIf_GetLinChannelId(uint8 ch)
{
    return LinIf_GetChannel(ch)->ChannelRef->LinChannelIdRef;
}

LINIF_LOCAL_INLINE uint8 LinIf_GetLinDriverId(uint8 ch)
{
    return LinIf_GetChannel(ch)->ChannelRef->LinDriverId;
}

LINIF_LOCAL_INLINE EcuM_WakeupSourceType LinIf_GetLinDriverWakeupSource(uint8 ch)
{
    return LinIf_GetChannel(ch)->ChannelRef->WakeUpSource;
}

LINIF_LOCAL_INLINE EcuM_WakeupSourceType LinIf_GetLinTrcvWakeupSource(uint8 ch)
{
    return LinIf_GetChannel(ch)->TransceiverDrvConfig->WakeUpSource;
}

LINIF_LOCAL_INLINE NetworkHandleType LinIf_GetComMNetwork(uint8 ch)
{
    return LinIf_GetChannel(ch)->ComMNetworkHandleRef;
}

LINIF_LOCAL_INLINE LinIf_NodeTypeType LinIf_GetNodeType(uint8 ch)
{
    return LinIf_GetChannel(ch)->NodeType->NodeType;
}

LINIF_LOCAL_INLINE void LinIf_UserWakeupConfirmation(uint8 ch, NetworkHandleType channel, boolean success)
{
    LinIf_GetChannel(ch)->WakeupConfirmation(channel, success);
}

LINIF_LOCAL_INLINE void LinIf_UserGotoSleepConfirmation(uint8 ch, NetworkHandleType channel, boolean success)
{
    LinIf_GetChannel(ch)->GotoSleepConfirmation(channel, success);
}

#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
LINIF_LOCAL_INLINE ApplicationType LinIf_GetChannelApplicationId(uint8 ch)
{
    return LinIf_GetChannel(ch)->ChannelApplicationId;
}
#endif

LINIF_LOCAL_INLINE void LinIf_SetEvent(LinIf_MasterRuntimeType* chPtr, uint16 event)
{
    chPtr->RootEvent |= event;
}

LINIF_LOCAL_INLINE void LinIf_ClrEvent(LinIf_MasterRuntimeType* chPtr, uint16 event)
{
    chPtr->RootEvent &= (uint16)(~event);
}

LINIF_LOCAL_INLINE boolean LinIf_IsEvent(const LinIf_MasterRuntimeType* chPtr, uint16 event)
{
    return (chPtr->RootEvent & event) != 0u;
}

LINIF_LOCAL_INLINE uint32 LinIf_Min(uint32 num1, uint32 num2)
{
    return ((num1) < (num2)) ? (num1) : (num2);
}

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
LINIF_LOCAL_INLINE void LinIf_DetReportError(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID, ApiId, ErrorId);
}
#endif

LINIF_LOCAL_INLINE void LinIf_ReportRuntimeError(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportRuntimeError(LINIF_MODULE_ID, LINIF_INSTANCE_ID, ApiId, ErrorId);
}

/**
 * Encapsulation function GetApplicationID(), to support multiple partition and no partition mode
 */
LINIF_LOCAL_INLINE ApplicationType LinIf_GetApplicationID(void)
{
    ApplicationType applicationId = (ApplicationType)0u;
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
    applicationId = GetApplicationID();
#endif
    return applicationId;
}

/* PRQA S 1532,1503 ++ */ /* VL_QAC_OneFunRef */
/**
 *  @brief         Check entry delat timer is expired
 */
LINIF_LOCAL_INLINE boolean LinIf_IsEntryDelayTimeout(const LinIf_MasterRuntimeType* masterChRtDataPtr)
{
    return ((uint32)masterChRtDataPtr->Timer == 0u) ? TRUE : FALSE;
}

LINIF_LOCAL_INLINE LinIf_MasterRuntimeType* LinIf_GetMasterRtDataPtr(NetworkHandleType ch)
{
    return LinIf_MasterRtPartitionData[ch];
}

/**
 *  @brief         Request the schedule table move to next entry
 */
LINIF_LOCAL_INLINE void LinIf_MoveScheduleToNextEntry(NetworkHandleType ch)
{
    LinIf_MasterRuntimeType* chPtr = LinIf_GetMasterRtDataPtr(ch);

    /* Move to next entry */
    chPtr->CurEntryIndex++;
}

LINIF_LOCAL_INLINE void LinIf_ClearEvent(NetworkHandleType ch, uint16 event)
{
    LinIf_MasterRuntimeType* chPtr = LinIf_GetMasterRtDataPtr(ch);
    LinIf_ClrEvent(chPtr, event);
}
/* PRQA S 1532,1503 -- */
#endif
