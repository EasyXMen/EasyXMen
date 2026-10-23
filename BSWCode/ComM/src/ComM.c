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
 ***********************************************************************************************************************
 **
 **  @file               : ComM.c
 **  @author             : xiaojian.liang
 **  @date               : 2024/08/21
 **  @vendor             : isoft
 **  @description        : ComM Interface Implementation source
 **
 **********************************************************************************************************************/
/* PRQA S 1503, 6060 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_ComM_STM19 */
/* =================================================== inclusions =================================================== */
#include "SchM_ComM.h"

#include "ComM_Nm.h"
#include "ComM_Dcm.h"
#include "ComM_EcuM.h"
#include "ComM_BswM.h"

#include "ComM_User.h"
#include "ComM_Pnc.h"
#include "ComM_Channel.h"

#if COMM_DEV_ERROR_DETECT == STD_ON
#include "Det.h"
#endif
#include "ComM_Arti.h"

/* ===================================================== macros ===================================================== */
#if (COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
#define COMM_INHIBIT_COUNTER_MAX ((uint16)65535)
#endif

/* ================================================ type definitions ================================================ */

/* ============================================ internal data definitions =========================================== */
#define COMM_START_SEC_VAR_CLEARED_PTR
#include "ComM_MemMap.h"
COMM_LOCAL const ComM_ConfigType* ComM_ConfigPtr;
#define COMM_STOP_SEC_VAR_CLEARED_PTR
#include "ComM_MemMap.h"

#define COMM_START_SEC_VAR_CLEARED_8
#include "ComM_MemMap.h"
COMM_LOCAL ComM_InitStatusType ComM_InitStatus[COMM_PARTITION_NUMBER];
#define COMM_STOP_SEC_VAR_CLEARED_8
#include "ComM_MemMap.h"

/* ============================================ external data definition ============================================ */
#if (COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
#define COMM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "ComM_MemMap.h"
ComM_InhibitionType ComM_InhibitionData;
#define COMM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "ComM_MemMap.h"
#endif

#if COMM_PNC_SUPPORT == STD_ON
#define COMM_START_SEC_VAR_CLEARED_BOOLEAN
#include "ComM_MemMap.h"
boolean ComM_PncEnabled;
#define COMM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "ComM_MemMap.h"
#endif

/* ========================================= internal function declarations ========================================= */
#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
COMM_LOCAL uint8 ComM_GetPartitionIndex(void);
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#if COMM_DEV_ERROR_DETECT == STD_ON
#ifdef QAC_ANALYZE
#pragma PRQA_NO_SIDE_EFFECTS ComM_ValidateInitStatus
#pragma PRQA_NO_SIDE_EFFECTS ComM_ValidatePointer
#pragma PRQA_NO_SIDE_EFFECTS ComM_ValidateUser
#pragma PRQA_NO_SIDE_EFFECTS ComM_ValidateComMode
#pragma PRQA_NO_SIDE_EFFECTS ComM_ValidateChannel
#pragma PRQA_NO_SIDE_EFFECTS ComM_ValidateAlreadyInitialized
#pragma PRQA_NO_SIDE_EFFECTS ComM_ValidatePnc
#endif

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
COMM_LOCAL boolean ComM_ValidatePartitionContext(uint8 apiId);
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
COMM_LOCAL boolean ComM_ValidateInitStatus(uint8 apiId);
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
COMM_LOCAL boolean ComM_ValidatePointer(uint8 apiId, const void* pointer);
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
COMM_LOCAL boolean ComM_ValidateUser(uint8 apiId, ComM_UserHandleType user);
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
COMM_LOCAL boolean ComM_ValidateComMode(uint8 apiId, ComM_ModeType comMode);
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
COMM_LOCAL boolean ComM_ValidateChannel(uint8 apiId, NetworkHandleType channel);
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
COMM_LOCAL boolean ComM_ValidateAlreadyInitialized(void);
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#if COMM_PNC_SUPPORT == STD_ON
#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
COMM_LOCAL boolean ComM_ValidatePnc(PNCHandleType Pnc);
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"
#endif
#endif

/* ========================================== external function definitions ========================================= */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * The implementation of ComM_Init.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_ComM_STCAL */
void ComM_Init(const ComM_ConfigType* ConfigPtr)
/* PRQA S 6070 -- */ /* VL_MTR_ComM_STCAL */
{
    ComM_ArtiApiStart(2, COMM_SID_INIT, ConfigPtr);
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidatePartitionContext(COMM_SID_INIT) && ComM_ValidatePointer(COMM_SID_INIT, ConfigPtr)
        && ComM_ValidateAlreadyInitialized())
#endif
    {
        SchM_Enter_ComM_Global();
        if (ComM_ConfigPtr == NULL_PTR)
        {
#if (COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
#if COMM_GLOBAL_NVM_BLOCK_DESCRIPTOR == STD_OFF
            ComM_InhibitionData = ComM_InhibitionDefault;
#endif
#endif
            ComM_ChannelGlobalInit(ConfigPtr->ChannelConfigs);

#if COMM_PNC_SUPPORT == STD_ON
            ComM_PncEnabled = ConfigPtr->PncEnabled;
            if (ComM_PncEnabled)
            {
                ComM_PncInit(ConfigPtr->PncConfigs);
            }
#endif

            ComM_UserInit(ConfigPtr->UserConfigs);

            ComM_ConfigPtr = ConfigPtr;
        }
        SchM_Exit_ComM_Global();

        ComM_ChannelLocalInit();

        ComM_InitStatus[ComM_GetPartitionIndex()] = COMM_INIT;
    }
    ComM_ArtiApiReturn(1, COMM_SID_INIT);
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * The implementation of ComM_DeInit.
 */
void ComM_DeInit(void)
{
    ComM_ArtiApiStart(1, COMM_SID_DEINIT);
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidatePartitionContext(COMM_SID_DEINIT) && ComM_ValidateInitStatus(COMM_SID_DEINIT))
#endif
    {
        ComM_InitStatus[ComM_GetPartitionIndex()] = COMM_UNINIT;

        boolean allDeInitialized = TRUE;
        SchM_Enter_ComM_Global();
        for (uint8 i = 0u; i < COMM_PARTITION_NUMBER; ++i)
        {
            if (ComM_InitStatus[i] != COMM_UNINIT)
            {
                allDeInitialized = FALSE;
                break;
            }
        }

        if (allDeInitialized)
        {
            ComM_ConfigPtr = NULL_PTR;
        }
        SchM_Exit_ComM_Global();
    }
    ComM_ArtiApiReturn(1, COMM_SID_DEINIT);
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * The implementation of ComM_GetStatus.
 */
Std_ReturnType ComM_GetStatus(ComM_InitStatusType* Status)
{
    Std_ReturnType stdReturn = E_NOT_OK;
    ComM_ArtiApiStart(2, COMM_SID_GETSTATUS, Status);
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidatePartitionContext(COMM_SID_GETSTATUS) && ComM_ValidatePointer(COMM_SID_GETSTATUS, Status))
#endif
    {
        *Status   = ComM_InitStatus[ComM_GetPartitionIndex()];
        stdReturn = E_OK;
    }
    ComM_ArtiApiReturn(2, COMM_SID_GETSTATUS, stdReturn);
    return stdReturn;
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#if COMM_VERSION_INFO_API == STD_ON
#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * The implementation of ComM_GetVersionInfo.
 */
void ComM_GetVersionInfo(Std_VersionInfoType* Versioninfo)
{
    ComM_ArtiApiStart(2, COMM_SID_GETVERSIONINFO, Versioninfo);
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidatePointer(COMM_SID_GETVERSIONINFO, Versioninfo))
#endif
    {
        Versioninfo->vendorID         = COMM_VENDOR_ID;
        Versioninfo->moduleID         = COMM_MODULE_ID;
        Versioninfo->sw_major_version = COMM_SW_MAJOR_VERSION;
        Versioninfo->sw_minor_version = COMM_SW_MINOR_VERSION;
        Versioninfo->sw_patch_version = COMM_SW_PATCH_VERSION;
    }
    ComM_ArtiApiReturn(1, COMM_SID_GETVERSIONINFO);
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"
#endif

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * The implementation of ComM_RequestComMode.
 */
Std_ReturnType ComM_RequestComMode(ComM_UserHandleType User, ComM_ModeType ComMode)
{
    Std_ReturnType stdReturn = E_NOT_OK;
    ComM_ArtiApiStart(3, COMM_SID_REQUESTCOMMODE, User, ComMode);
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidatePartitionContext(COMM_SID_REQUESTCOMMODE) && ComM_ValidateInitStatus(COMM_SID_REQUESTCOMMODE)
        && ComM_ValidateUser(COMM_SID_REQUESTCOMMODE, User) && ComM_ValidateComMode(COMM_SID_REQUESTCOMMODE, ComMode))
#endif
    {
        stdReturn = ComM_UserRequestComMode(User, ComMode);
#if (COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
        if (stdReturn == COMM_E_MODE_LIMITATION)
        {
            SchM_Enter_ComM_CommunicationInhibition();
            if (ComM_InhibitionData.InhibitCounter < COMM_INHIBIT_COUNTER_MAX)
            {
                ++ComM_InhibitionData.InhibitCounter;
            }
            SchM_Exit_ComM_CommunicationInhibition();
        }
#endif
    }
    ComM_ArtiApiReturn(2, COMM_SID_REQUESTCOMMODE, stdReturn);
    return stdReturn;
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * The implementation of ComM_GetMaxComMode.
 */
Std_ReturnType ComM_GetMaxComMode(ComM_UserHandleType User, ComM_ModeType* ComMode)
{
    Std_ReturnType stdReturn = E_NOT_OK;
    ComM_ArtiApiStart(3, COMM_SID_GETMAXCOMMODE, User, ComMode);
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidatePartitionContext(COMM_SID_GETMAXCOMMODE) && ComM_ValidateInitStatus(COMM_SID_GETMAXCOMMODE)
        && ComM_ValidateUser(COMM_SID_GETMAXCOMMODE, User) && ComM_ValidatePointer(COMM_SID_GETMAXCOMMODE, ComMode))
#endif
    {
        *ComMode  = ComM_UserGetMaxComMode(User);
        stdReturn = E_OK;
    }
    ComM_ArtiApiReturn(2, COMM_SID_GETMAXCOMMODE, stdReturn);
    return stdReturn;
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * The implementation of ComM_GetRequestedComMode.
 */
Std_ReturnType ComM_GetRequestedComMode(ComM_UserHandleType User, ComM_ModeType* ComMode)
{
    Std_ReturnType stdReturn = E_NOT_OK;
    ComM_ArtiApiStart(3, COMM_SID_GETREQUESTEDCOMMODE, User, ComMode);
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidatePartitionContext(COMM_SID_GETREQUESTEDCOMMODE)
        && ComM_ValidateInitStatus(COMM_SID_GETREQUESTEDCOMMODE)
        && ComM_ValidateUser(COMM_SID_GETREQUESTEDCOMMODE, User)
        && ComM_ValidatePointer(COMM_SID_GETREQUESTEDCOMMODE, ComMode))
#endif
    {
        *ComMode  = ComM_UserGetRequestedComMode(User);
        stdReturn = E_OK;
    }
    ComM_ArtiApiReturn(2, COMM_SID_GETREQUESTEDCOMMODE, stdReturn);
    return stdReturn;
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * The implementation of ComM_GetCurrentComMode.
 */
Std_ReturnType ComM_GetCurrentComMode(ComM_UserHandleType User, ComM_ModeType* ComMode)
{
    Std_ReturnType stdReturn = E_NOT_OK;
    ComM_ArtiApiStart(3, COMM_SID_GETCURRENTCOMMODE, User, ComMode);
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidatePartitionContext(COMM_SID_GETCURRENTCOMMODE) && ComM_ValidateInitStatus(COMM_SID_GETCURRENTCOMMODE)
        && ComM_ValidateUser(COMM_SID_GETCURRENTCOMMODE, User)
        && ComM_ValidatePointer(COMM_SID_GETCURRENTCOMMODE, ComMode))
#endif
    {
        *ComMode  = ComM_UserGetCurrentComMode(User);
        stdReturn = E_OK;
    }
    ComM_ArtiApiReturn(2, COMM_SID_GETCURRENTCOMMODE, stdReturn);
    return stdReturn;
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#if COMM_PNC_SUPPORT == STD_ON
#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * The implementation of ComM_GetCurrentPNCComMode.
 */
Std_ReturnType ComM_GetCurrentPNCComMode(ComM_UserHandleType User, ComM_ModeType* ComMode)
{
    Std_ReturnType stdReturn = E_NOT_OK;
    ComM_ArtiApiStart(3, COMM_SID_GETCURRENTPNCCOMMODE, User, ComMode);
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidatePartitionContext(COMM_SID_GETCURRENTPNCCOMMODE)
        && ComM_ValidateInitStatus(COMM_SID_GETCURRENTPNCCOMMODE)
        && ComM_ValidateUser(COMM_SID_GETCURRENTPNCCOMMODE, User)
        && ComM_ValidatePointer(COMM_SID_GETCURRENTPNCCOMMODE, ComMode))
#endif
    {
        stdReturn = ComM_UserGetCurrentPNCComMode(User, ComMode);
    }
    ComM_ArtiApiReturn(2, COMM_SID_GETCURRENTPNCCOMMODE, stdReturn);
    return stdReturn;
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"
#endif

#if COMM_WAKEUP_INHIBITION_ENABLED == STD_ON
#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * The implementation of ComM_PreventWakeUp.
 */
Std_ReturnType ComM_PreventWakeUp(NetworkHandleType Channel, boolean Status)
{
    Std_ReturnType stdReturn = E_NOT_OK;
    ComM_ArtiApiStart(3, COMM_SID_PREVENTWAKEUP, Channel, Status);
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidatePartitionContext(COMM_SID_PREVENTWAKEUP) && ComM_ValidateInitStatus(COMM_SID_PREVENTWAKEUP)
        && ComM_ValidateChannel(COMM_SID_PREVENTWAKEUP, Channel))
#endif
    {
        if ((ComM_InhibitionData.EcuGroupClassification & COMM_WAKEUP_INHIBITION_ACTIVE_BflMask) != 0u)
        {
            if (Status)
            {
                SchM_Enter_ComM_CommunicationInhibition();
                ComM_InhibitionData.InhibitionStatus[Channel] |= COMM_WAKEUP_INHIBITION_ACTIVE_YES;
                SchM_Exit_ComM_CommunicationInhibition();
            }
            else
            {
                SchM_Enter_ComM_CommunicationInhibition();
                /* clang-format off */
                ComM_InhibitionData.InhibitionStatus[Channel] &= (ComM_InhibitionStatusType)~COMM_WAKEUP_INHIBITION_ACTIVE_YES;
                /* clang-format on */
                SchM_Exit_ComM_CommunicationInhibition();
            }
            stdReturn = E_OK;
        }
    }

    ComM_ArtiApiReturn(2, COMM_SID_PREVENTWAKEUP, stdReturn);
    return stdReturn;
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"
#endif

#if COMM_MODE_LIMITATION_ENABLED == STD_ON
#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * The implementation of ComM_LimitChannelToNoComMode.
 */
Std_ReturnType ComM_LimitChannelToNoComMode(NetworkHandleType Channel, boolean Status)
{
    Std_ReturnType stdReturn = E_NOT_OK;
    ComM_ArtiApiStart(3, COMM_SID_LIMITCHANNELTONOCOMMODE, Channel, Status);
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidatePartitionContext(COMM_SID_LIMITCHANNELTONOCOMMODE)
        && ComM_ValidateInitStatus(COMM_SID_LIMITCHANNELTONOCOMMODE)
        && ComM_ValidateChannel(COMM_SID_LIMITCHANNELTONOCOMMODE, Channel))
#endif
    {
        if ((ComM_InhibitionData.EcuGroupClassification & COMM_LIMITED_TO_NO_COM_BflMask) != 0u)
        {
            if (Status)
            {
                SchM_Enter_ComM_CommunicationInhibition();
                ComM_InhibitionData.InhibitionStatus[Channel] |= COMM_LIMITED_TO_NO_COM_YES;
                SchM_Exit_ComM_CommunicationInhibition();
            }
            else
            {
                SchM_Enter_ComM_CommunicationInhibition();
                ComM_InhibitionData.InhibitionStatus[Channel] &= (ComM_InhibitionStatusType)~COMM_LIMITED_TO_NO_COM_YES;
                SchM_Exit_ComM_CommunicationInhibition();
            }
            stdReturn = E_OK;
        }
    }
    ComM_ArtiApiReturn(2, COMM_SID_LIMITCHANNELTONOCOMMODE, stdReturn);
    return stdReturn;
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * The implementation of ComM_LimitECUToNoComMode.
 */
Std_ReturnType ComM_LimitECUToNoComMode(boolean Status)
{
    Std_ReturnType stdReturn = E_NOT_OK;
    ComM_ArtiApiStart(2, COMM_SID_LIMITECUTONOCOMMODE, Status);
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidatePartitionContext(COMM_SID_LIMITECUTONOCOMMODE)
        && ComM_ValidateInitStatus(COMM_SID_LIMITECUTONOCOMMODE))
#endif
    {
        if ((ComM_InhibitionData.EcuGroupClassification & COMM_LIMITED_TO_NO_COM_BflMask) != 0u)
        {
            for (NetworkHandleType channel = 0u; channel < COMM_CHANNEL_NUMBER; ++channel)
            {
                if (Status)
                {
                    SchM_Enter_ComM_CommunicationInhibition();
                    ComM_InhibitionData.InhibitionStatus[channel] |= COMM_LIMITED_TO_NO_COM_YES;
                    SchM_Exit_ComM_CommunicationInhibition();
                }
                else
                {
                    SchM_Enter_ComM_CommunicationInhibition();
                    ComM_InhibitionData.InhibitionStatus[channel] &=
                        (ComM_InhibitionStatusType)~COMM_LIMITED_TO_NO_COM_YES;
                    SchM_Exit_ComM_CommunicationInhibition();
                }
            }
            stdReturn = E_OK;
        }
    }
    ComM_ArtiApiReturn(2, COMM_SID_LIMITECUTONOCOMMODE, stdReturn);
    return stdReturn;
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"
#endif

#if (COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * The implementation of ComM_GetInhibitionStatus.
 */
Std_ReturnType ComM_GetInhibitionStatus(NetworkHandleType Channel, ComM_InhibitionStatusType* Status)
{
    Std_ReturnType stdReturn = E_NOT_OK;
    ComM_ArtiApiStart(3, COMM_SID_GETINHIBITIONSTATUS, Channel, Status);
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidatePartitionContext(COMM_SID_GETINHIBITIONSTATUS)
        && ComM_ValidateInitStatus(COMM_SID_GETINHIBITIONSTATUS)
        && ComM_ValidateChannel(COMM_SID_GETINHIBITIONSTATUS, Channel)
        && ComM_ValidatePointer(COMM_SID_GETINHIBITIONSTATUS, Status))
#endif
    {
        *Status   = ComM_InhibitionData.InhibitionStatus[Channel];
        stdReturn = E_OK;
    }

    ComM_ArtiApiReturn(2, COMM_SID_GETINHIBITIONSTATUS, stdReturn);
    return stdReturn;
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * The implementation of ComM_ReadInhibitCounter.
 */
Std_ReturnType ComM_ReadInhibitCounter(uint16* CounterValue)
{
    Std_ReturnType stdReturn = E_NOT_OK;
    ComM_ArtiApiStart(2, COMM_SID_READINHIBITCOUNTER, CounterValue);
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidatePartitionContext(COMM_SID_READINHIBITCOUNTER)
        && ComM_ValidateInitStatus(COMM_SID_READINHIBITCOUNTER)
        && ComM_ValidatePointer(COMM_SID_READINHIBITCOUNTER, CounterValue))
#endif
    {
        SchM_Enter_ComM_CommunicationInhibition();
        *CounterValue = ComM_InhibitionData.InhibitCounter;
        SchM_Exit_ComM_CommunicationInhibition();
        stdReturn = E_OK;
    }
    ComM_ArtiApiReturn(2, COMM_SID_READINHIBITCOUNTER, stdReturn);
    return stdReturn;
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * The implementation of ComM_ResetInhibitCounter.
 */
Std_ReturnType ComM_ResetInhibitCounter(void)
{
    Std_ReturnType stdReturn = E_NOT_OK;
    ComM_ArtiApiStart(1, COMM_SID_RESETINHIBITCOUNTER);
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidatePartitionContext(COMM_SID_RESETINHIBITCOUNTER)
        && ComM_ValidateInitStatus(COMM_SID_RESETINHIBITCOUNTER))
#endif
    {
        SchM_Enter_ComM_CommunicationInhibition();
        ComM_InhibitionData.InhibitCounter = 0u;
        SchM_Exit_ComM_CommunicationInhibition();
        stdReturn = E_OK;
    }
    ComM_ArtiApiReturn(2, COMM_SID_RESETINHIBITCOUNTER, stdReturn);
    return stdReturn;
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * The implementation of ComM_SetECUGroupClassification.
 */
Std_ReturnType ComM_SetECUGroupClassification(ComM_InhibitionStatusType Status)
{
    Std_ReturnType stdReturn = E_NOT_OK;
    ComM_ArtiApiStart(2, COMM_SID_SETECUGROUPCLASSIFICATION, Status);
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidatePartitionContext(COMM_SID_SETECUGROUPCLASSIFICATION)
        && ComM_ValidateInitStatus(COMM_SID_SETECUGROUPCLASSIFICATION))
#endif
    {
        ComM_InhibitionData.EcuGroupClassification = Status;
        stdReturn                                  = E_OK;
    }
    ComM_ArtiApiReturn(2, COMM_SID_SETECUGROUPCLASSIFICATION, stdReturn);
    return stdReturn;
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"
#endif

#if COMM_NM_ENABLED == STD_ON
#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * The implementation of ComM_Nm_NetworkStartIndication.
 */
void ComM_Nm_NetworkStartIndication(NetworkHandleType Channel)
{
    ComM_ArtiApiStart(2, COMM_SID_NM_NETWORKSTARTINDICATION, Channel);
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidatePartitionContext(COMM_SID_NM_NETWORKSTARTINDICATION)
        && ComM_ValidateInitStatus(COMM_SID_NM_NETWORKSTARTINDICATION)
        && ComM_ValidateChannel(COMM_SID_NM_NETWORKSTARTINDICATION, Channel))
#endif
    {
        ComM_ChannelWakeUpIndication(Channel, FALSE);
    }
    ComM_ArtiApiReturn(1, COMM_SID_NM_NETWORKSTARTINDICATION);
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * The implementation of ComM_Nm_NetworkMode.
 */
void ComM_Nm_NetworkMode(NetworkHandleType Channel)
{
    ComM_ArtiApiStart(2, COMM_SID_NM_NETWORKMODE, Channel);
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidatePartitionContext(COMM_SID_NM_NETWORKMODE) && ComM_ValidateInitStatus(COMM_SID_NM_NETWORKMODE)
        && ComM_ValidateChannel(COMM_SID_NM_NETWORKMODE, Channel))
#endif
    {
        ComM_ChannelNmModeIndication(Channel, COMM_NM_MODE_EVENT_NETWORK);
    }
    ComM_ArtiApiReturn(1, COMM_SID_NM_NETWORKMODE);
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * The implementation of ComM_Nm_PrepareBusSleepMode.
 */
void ComM_Nm_PrepareBusSleepMode(NetworkHandleType Channel)
{
    ComM_ArtiApiStart(2, COMM_SID_NM_PREPAREBUSSLEEPMODE, Channel);
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidatePartitionContext(COMM_SID_NM_PREPAREBUSSLEEPMODE)
        && ComM_ValidateInitStatus(COMM_SID_NM_PREPAREBUSSLEEPMODE)
        && ComM_ValidateChannel(COMM_SID_NM_PREPAREBUSSLEEPMODE, Channel))
#endif
    {
        ComM_ChannelNmModeIndication(Channel, COMM_NM_MODE_EVENT_PREPARE_BUS_SLEEP);
    }
    ComM_ArtiApiReturn(1, COMM_SID_NM_PREPAREBUSSLEEPMODE);
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * The implementation of ComM_Nm_BusSleepMode.
 */
void ComM_Nm_BusSleepMode(NetworkHandleType Channel)
{
    ComM_ArtiApiStart(2, COMM_SID_NM_BUSSLEEPMODE, Channel);
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidatePartitionContext(COMM_SID_NM_BUSSLEEPMODE) && ComM_ValidateInitStatus(COMM_SID_NM_BUSSLEEPMODE)
        && ComM_ValidateChannel(COMM_SID_NM_BUSSLEEPMODE, Channel))
#endif
    {
        ComM_ChannelNmModeIndication(Channel, COMM_NM_MODE_EVENT_BUS_SLEEP);
    }
    ComM_ArtiApiReturn(1, COMM_SID_NM_BUSSLEEPMODE);
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * The implementation of ComM_Nm_RestartIndication.
 */
void ComM_Nm_RestartIndication(NetworkHandleType Channel)
{
    ComM_ArtiApiStart(2, COMM_SID_NM_RESTARTINDICATION, Channel);
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidatePartitionContext(COMM_SID_NM_RESTARTINDICATION)
        && ComM_ValidateInitStatus(COMM_SID_NM_RESTARTINDICATION)
        && ComM_ValidateChannel(COMM_SID_NM_RESTARTINDICATION, Channel))
#endif
    {
        ComM_ChannelWakeUpIndication(Channel, FALSE);
    }
    ComM_ArtiApiReturn(1, COMM_SID_NM_RESTARTINDICATION);
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#if COMM_PNC_SUPPORT == STD_ON
#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * The implementation of ComM_Nm_UpdateEIRA.
 */
void ComM_Nm_UpdateEIRA(const uint8* PncBitVectorPtr)
{
    ComM_ArtiApiStart(2, COMM_SID_NMUPDATEEIRA, PncBitVectorPtr);
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidatePartitionContext(COMM_SID_NMUPDATEEIRA) && ComM_ValidateInitStatus(COMM_SID_NMUPDATEEIRA)
        && ComM_ValidatePointer(COMM_SID_NMUPDATEEIRA, PncBitVectorPtr))
#endif
    {
        if (ComM_PncEnabled)
        {
            ComM_PncUpdateEIRA(PncBitVectorPtr);
        }
    }
    ComM_ArtiApiReturn(1, COMM_SID_NMUPDATEEIRA);
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#if COMM_PNC_GATEWAY_ENABLED == STD_ON
#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * The implementation of ComM_Nm_UpdateERA.
 */
void ComM_Nm_UpdateERA(NetworkHandleType Channel, const uint8* PncBitVectorPtr)
{
    ComM_ArtiApiStart(3, COMM_SID_NMUPDATEERA, Channel, PncBitVectorPtr);
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidatePartitionContext(COMM_SID_NMUPDATEERA) && ComM_ValidateInitStatus(COMM_SID_NMUPDATEERA)
        && ComM_ValidateChannel(COMM_SID_NMUPDATEERA, Channel)
        && ComM_ValidatePointer(COMM_SID_NMUPDATEERA, PncBitVectorPtr))
#endif
    {
        ComM_ChannelUpdateERA(Channel, PncBitVectorPtr);
    }
    ComM_ArtiApiReturn(1, COMM_SID_NMUPDATEERA);
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"
#endif
#endif
#endif

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * The implementation of ComM_DCM_ActiveDiagnostic.
 */
void ComM_DCM_ActiveDiagnostic(NetworkHandleType Channel)
{
    ComM_ArtiApiStart(2, COMM_SID_DCM_ACTIVEDIAGNOSTIC, Channel);
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidatePartitionContext(COMM_SID_DCM_ACTIVEDIAGNOSTIC)
        && ComM_ValidateInitStatus(COMM_SID_DCM_ACTIVEDIAGNOSTIC))
#endif
    {
        ComM_ChannelActiveDiagnostic(Channel, TRUE);
    }
    ComM_ArtiApiReturn(1, COMM_SID_DCM_ACTIVEDIAGNOSTIC);
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * The implementation of ComM_DCM_ActiveDiagnostic.
 */
void ComM_DCM_InactiveDiagnostic(NetworkHandleType Channel)
{
    ComM_ArtiApiStart(2, COMM_SID_DCM_INACTIVEDIAGNOSTIC, Channel);
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidatePartitionContext(COMM_SID_DCM_INACTIVEDIAGNOSTIC)
        && ComM_ValidateInitStatus(COMM_SID_DCM_INACTIVEDIAGNOSTIC))
#endif
    {
        ComM_ChannelActiveDiagnostic(Channel, FALSE);
    }
    ComM_ArtiApiReturn(1, COMM_SID_DCM_INACTIVEDIAGNOSTIC);
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * The implementation of ComM_EcuM_WakeUpIndication.
 */
void ComM_EcuM_WakeUpIndication(NetworkHandleType Channel)
{
    ComM_ArtiApiStart(2, COMM_SID_ECUM_WAKEUPINDICATION, Channel);
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidatePartitionContext(COMM_SID_ECUM_WAKEUPINDICATION)
        && ComM_ValidateInitStatus(COMM_SID_ECUM_WAKEUPINDICATION)
        && ComM_ValidateChannel(COMM_SID_ECUM_WAKEUPINDICATION, Channel))
#endif
    {
#if COMM_SYNCHRONOUS_WAKE_UP == STD_ON
        ComM_ChannelWakeUpIndication(Channel, TRUE);
#else
        ComM_ChannelWakeUpIndication(Channel, FALSE);
#endif
    }
    ComM_ArtiApiReturn(1, COMM_SID_ECUM_WAKEUPINDICATION);
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#if COMM_PNC_SUPPORT == STD_ON
#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * The implementation of ComM_EcuM_PNCWakeUpIndication.
 */
void ComM_EcuM_PNCWakeUpIndication(PNCHandleType Pnc)
{
    ComM_ArtiApiStart(2, COMM_SID_ECUM_PNCWAKEUPINDICATION, Pnc);
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidatePartitionContext(COMM_SID_ECUM_PNCWAKEUPINDICATION)
        && ComM_ValidateInitStatus(COMM_SID_ECUM_PNCWAKEUPINDICATION) && ComM_ValidatePnc(Pnc))
#endif
    {
        if (ComM_PncEnabled)
        {
#if COMM_SYNCHRONOUS_WAKE_UP == STD_ON
            ComM_ChannelWakeUpIndication(0u, TRUE);
#else
            ComM_PncWakeUpIndication(Pnc);
#endif
        }
    }
    ComM_ArtiApiReturn(1, COMM_SID_ECUM_PNCWAKEUPINDICATION);
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"
#endif

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * The implementation of ComM_CommunicationAllowed.
 */
void ComM_CommunicationAllowed(NetworkHandleType Channel, boolean Allowed)
{
    ComM_ArtiApiStart(3, COMM_SID_COMMUNICATIONALLOWED, Channel, Allowed);
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidatePartitionContext(COMM_SID_COMMUNICATIONALLOWED)
        && ComM_ValidateInitStatus(COMM_SID_COMMUNICATIONALLOWED)
        && ComM_ValidateChannel(COMM_SID_COMMUNICATIONALLOWED, Channel))
#endif
    {
        ComM_ChannelCommunicationAllowed(Channel, Allowed);
    }
    ComM_ArtiApiReturn(1, COMM_SID_COMMUNICATIONALLOWED);
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * The implementation of ComM_BusSM_ModeIndication.
 */
void ComM_BusSM_ModeIndication(NetworkHandleType Channel, ComM_ModeType ComMode)
{
    ComM_ArtiApiStart(3, COMM_SID_BUSSM_MODEINDICATION, Channel, ComMode);
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidatePartitionContext(COMM_SID_BUSSM_MODEINDICATION)
        && ComM_ValidateInitStatus(COMM_SID_BUSSM_MODEINDICATION)
        && ComM_ValidateChannel(COMM_SID_BUSSM_MODEINDICATION, Channel))
#endif
    {
        ComM_ChannelBusSMModeIndication(Channel, ComMode);
    }
    ComM_ArtiApiReturn(1, COMM_SID_BUSSM_MODEINDICATION);
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * The implementation of ComM_MainFunction.
 */
void ComM_MainFunction(NetworkHandleType Channel)
{
    ComM_ArtiApiStart(2, COMM_SID_MAINFUNCATION, Channel);
#if COMM_DEV_ERROR_DETECT == STD_ON && COMM_MULTIPLE_PARTITION_ENABLED == STD_ON
    if (ComM_ValidatePartitionContext(COMM_SID_MAINFUNCATION))
#endif
    {
        if (ComM_InitStatus[ComM_GetPartitionIndex()] == COMM_INIT)
        {
            ComM_ChannelMainFunction(Channel);
        }

#if COMM_CURRENT_MODE_NOTIFICATION_ENABLED == STD_ON
        if (Channel == 0u)
        {
            ComM_UserCurrentModeNotification();
        }
#endif
    }
    ComM_ArtiApiReturn(1, COMM_SID_MAINFUNCATION);
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

/* PRQA S 1532 -- */ /* VL_QAC_OneFunRef */

/* ========================================== internal function definitions ========================================= */
/* PRQA S 2889 ++ */ /* VL_QAC_MultiReturn */
#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * @brief Returns the index of current partition in the ComM configuration.
 * @return       The index of current partition in the ComM configuration
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-59646
 */
COMM_LOCAL uint8 ComM_GetPartitionIndex(void)
{
    uint8 partitionIndex = 0u;
#if COMM_MULTIPLE_PARTITION_ENABLED == STD_ON
    ApplicationType application = GetApplicationID();
    for (; partitionIndex < COMM_PARTITION_NUMBER; ++partitionIndex)
    {
        if (ComM_OsApplications[partitionIndex] == application)
        {
            break;
        }
    }
#endif
    return partitionIndex;
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#if COMM_DEV_ERROR_DETECT == STD_ON
#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * @brief Development error validation of invalid partition context
 * @param[in]    apiId  ID of API service in which error is detected
 * @retval       TRUE   Development error not occurred
 * @retval       FALSE  Development error occurred
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
COMM_LOCAL boolean ComM_ValidatePartitionContext(uint8 apiId)
{
#if COMM_MULTIPLE_PARTITION_ENABLED == STD_ON
    if (ComM_GetPartitionIndex() >= COMM_PARTITION_NUMBER)
    {
        (void)Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, apiId, COMM_E_INVALID_PARTITION_CONTEXT);
        return FALSE;
    }
#else
    COMM_UNUSED_PARAM(apiId);
#endif
    return TRUE;
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * @brief Development error validation of uninit
 * @param[in]    apiId  ID of API service in which error is detected
 * @retval       TRUE   Development error not occurred
 * @retval       FALSE  Development error occurred
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
COMM_LOCAL boolean ComM_ValidateInitStatus(uint8 apiId)
{
    if (ComM_InitStatus[ComM_GetPartitionIndex()] != COMM_INIT)
    {
        (void)Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, apiId, COMM_E_UNINIT);
        return FALSE;
    }
    return TRUE;
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * @brief Development error validation of null pointer
 * @param[in]    apiId   ID of API service in which error is detected
 * @param[in]    pointer pointer
 * @retval       TRUE    Development error not occurred
 * @retval       FALSE   Development error occurred
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
COMM_LOCAL boolean ComM_ValidatePointer(uint8 apiId, const void* pointer)
{
    if (pointer == NULL_PTR)
    {
        (void)Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, apiId, COMM_E_PARAM_POINTER);
        return FALSE;
    }
    return TRUE;
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * @brief Development error validation of invalid user
 * @param[in]    apiId  ID of API service in which error is detected
 * @param[in]    user   user
 * @retval       TRUE   Development error not occurred
 * @retval       FALSE  Development error occurred
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
COMM_LOCAL boolean ComM_ValidateUser(uint8 apiId, ComM_UserHandleType user)
{
    if (user >= COMM_USER_NUMBER)
    {
        (void)Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, apiId, COMM_E_WRONG_PARAMETERS);
        return FALSE;
    }

#if COMM_MULTIPLE_PARTITION_ENABLED == STD_ON
    if (ComM_UserGetApplication(user) != GetApplicationID())
    {
        (void)Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, apiId, COMM_E_WRONG_PARAMETERS);
        return FALSE;
    }
#endif
    return TRUE;
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * @brief Development error validation of invalid ComMode input
 * @param[in]    apiId    ID of API service in which error is detected
 * @param[in]    comMode  comMode
 * @retval       TRUE   Development error not occurred
 * @retval       FALSE  Development error occurred
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
COMM_LOCAL boolean ComM_ValidateComMode(uint8 apiId, ComM_ModeType comMode)
{
    boolean invalid;
    if (apiId == COMM_SID_REQUESTCOMMODE)
    {
        invalid = (comMode != COMM_NO_COMMUNICATION) && (comMode != COMM_FULL_COMMUNICATION);
    }
    else /* COMM_SID_BUSSM_MODEINDICATION */
    {
        invalid = (comMode != COMM_NO_COMMUNICATION) && (comMode != COMM_FULL_COMMUNICATION)
                  && (comMode != COMM_SILENT_COMMUNICATION);
    }
    if (invalid)
    {
        (void)Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, apiId, COMM_E_WRONG_PARAMETERS);
        return FALSE;
    }
    return TRUE;
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * @brief Development error validation of invalid channel
 * @param[in]    apiId   ID of API service in which error is detected
 * @param[in]    channel channel
 * @retval       TRUE   Development error not occurred
 * @retval       FALSE  Development error occurred
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
COMM_LOCAL boolean ComM_ValidateChannel(uint8 apiId, NetworkHandleType channel)
{
    if (channel >= COMM_CHANNEL_NUMBER)
    {
        (void)Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, apiId, COMM_E_WRONG_PARAMETERS);
        return FALSE;
    }

#if COMM_MULTIPLE_PARTITION_ENABLED == STD_ON
    if (ComM_ChannelGetApplication(channel) != GetApplicationID())
    {
        (void)Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, apiId, COMM_E_WRONG_PARAMETERS);
        return FALSE;
    }
#endif

    return TRUE;
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * @brief Development error validation of already initialized
 * @retval       TRUE   Development error not occurred
 * @retval       FALSE  Development error occurred
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
COMM_LOCAL boolean ComM_ValidateAlreadyInitialized(void)
{
    if (ComM_InitStatus[ComM_GetPartitionIndex()] == COMM_INIT)
    {
        (void)Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SID_INIT, COMM_E_ALREADY_INITIALIZED);
        return FALSE;
    }
    return TRUE;
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#if COMM_PNC_SUPPORT == STD_ON
#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * @brief Development error validation of invalid pnc
 * @param[in]    Pnc    Pnc
 * @retval       TRUE   Development error not occurred
 * @retval       FALSE  Development error occurred
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
COMM_LOCAL boolean ComM_ValidatePnc(PNCHandleType Pnc)
{
    if (Pnc >= COMM_PNC_NUMBER)
    {
        (void)Det_ReportError(
            COMM_MODULE_ID,
            COMM_INSTANCE_ID,
            COMM_SID_ECUM_PNCWAKEUPINDICATION,
            COMM_E_WRONG_PARAMETERS);
        return FALSE;
    }
    return TRUE;
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"
#endif
#endif
/* PRQA S 2889 -- */       /* VL_QAC_MultiReturn */
/* PRQA S 1503, 6060 -- */ /* VL_QAC_NoUsedApi, VL_MTR_ComM_STM19 */
