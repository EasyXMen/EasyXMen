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
 ***********************************************************************************************************************
 **
 **  @file               : ComM_User.c
 **  @author             : xiaojian.liang
 **  @date               : 2024/08/21
 **  @vendor             : isoft
 **  @description        : ComM User Implementation source
 **
 **********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "ComM_Internal.h"
#include "ComM_User.h"
#include "ComM_Pnc.h"
#include "ComM_Channel.h"

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ============================================ internal data definitions =========================================== */
#define COMM_START_SEC_VAR_CLEARED_PTR
#include "ComM_MemMap.h"
COMM_LOCAL const ComM_UserConfigType* ComM_UserConfigs;
#define COMM_STOP_SEC_VAR_CLEARED_PTR
#include "ComM_MemMap.h"

#define COMM_START_SEC_VAR_CLEARED_8
#include "ComM_MemMap.h"
COMM_LOCAL ComM_ModeType ComM_UserRequestedComMode[COMM_USER_NUMBER];
#define COMM_STOP_SEC_VAR_CLEARED_8
#include "ComM_MemMap.h"
/* ========================================= internal function declarations ========================================= */

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/* PRQA S 1532, 2889 ++ */ /* VL_QAC_OneFunRef, VL_QAC_MultiReturn */
/* ========================================== external function definitions ========================================= */
void ComM_UserInit(const ComM_UserConfigType* UserConfigs)
{
    for (ComM_UserHandleType user = 0u; user < COMM_USER_NUMBER; ++user)
    {
        ComM_UserRequestedComMode[user] = COMM_NO_COMMUNICATION;
    }

    ComM_UserConfigs = UserConfigs;
}

Std_ReturnType ComM_UserRequestComMode(ComM_UserHandleType User, ComM_ModeType ComMode)
{
    Std_ReturnType stdReturn = E_OK;

#if (COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
    const ComM_UserConfigType* userConfigPtr = &ComM_UserConfigs[User];

    if (ComMode == COMM_FULL_COMMUNICATION)
    {
        for (uint8 i = 0u; i < userConfigPtr->NumberOfChannels; ++i)
        {
            if (ComM_ChannelGetMaxComMode(userConfigPtr->Channels[i]) == COMM_NO_COMMUNICATION)
            {
                stdReturn = COMM_E_MODE_LIMITATION;
                break;
            }
        }
    }
#endif
    ComM_UserRequestedComMode[User] = ComMode;

    return stdReturn;
}

ComM_ModeType ComM_UserGetMaxComMode(ComM_UserHandleType User)
{
    const ComM_UserConfigType* userConfigPtr = &ComM_UserConfigs[User];
    ComM_ModeType              maxComMode    = COMM_FULL_COMMUNICATION;

    for (uint8 i = 0u; i < userConfigPtr->NumberOfChannels; ++i)
    {
        if (ComM_ChannelGetMaxComMode(userConfigPtr->Channels[i]) == COMM_NO_COMMUNICATION)
        {
            maxComMode = COMM_NO_COMMUNICATION;
            break;
        }
    }

#if COMM_PNC_SUPPORT == STD_ON
    if (ComM_PncEnabled && (maxComMode == COMM_FULL_COMMUNICATION))
    {
        for (PNCHandleType pnc = 0u; pnc < COMM_PNC_NUMBER; ++pnc)
        {
            if (ComM_PncIsUserAssigned(pnc, User))
            {
                if (ComM_PncGetMaxComMode(pnc) == COMM_NO_COMMUNICATION)
                {
                    maxComMode = COMM_NO_COMMUNICATION;
                    break;
                }
            }
        }
    }
#endif
    return maxComMode;
}

ComM_ModeType ComM_UserGetRequestedComMode(ComM_UserHandleType User)
{
    return ComM_UserRequestedComMode[User];
}

ComM_ModeType ComM_UserGetCurrentComMode(ComM_UserHandleType User)
{
    const ComM_UserConfigType* userConfigPtr = &ComM_UserConfigs[User];
    ComM_ModeType              userComMode   = COMM_FULL_COMMUNICATION;

    for (uint8 i = 0u; i < userConfigPtr->NumberOfChannels; ++i)
    {
        ComM_ModeType channelComMode = ComM_ChannelGetCurrentComMode(userConfigPtr->Channels[i]);
        if (userComMode > channelComMode)
        {
            userComMode = channelComMode;
        }
    }
    return userComMode;
}

#if COMM_PNC_SUPPORT == STD_ON
Std_ReturnType ComM_UserGetCurrentPNCComMode(ComM_UserHandleType User, ComM_ModeType* ComMode)
{
    if (!ComM_PncEnabled)
    {
        return E_NOT_OK;
    }

    ComM_ModeType comMode;
    boolean       assigned = FALSE;

    for (PNCHandleType pnc = 0u; pnc < COMM_PNC_NUMBER; ++pnc)
    {
        if (ComM_PncIsUserAssigned(pnc, User))
        {
            if (assigned)
            {
                return COMM_E_MULTIPLE_PNC_ASSIGNED;
            }
            assigned = TRUE;
            comMode =
                (ComM_PncGetMode(pnc) == COMM_PNC_NO_COMMUNICATION) ? COMM_NO_COMMUNICATION : COMM_FULL_COMMUNICATION;
        }
    }

    if (!assigned)
    {
        return COMM_E_NO_PNC_ASSIGNED;
    }

    *ComMode = comMode;
    return E_OK;
}
#endif

#if COMM_CURRENT_MODE_NOTIFICATION_ENABLED == STD_ON
ComM_ModeType ComM_UserGetCurrentMode(ComM_UserHandleType User)
{
    ComM_ModeType currentMode = COMM_FULL_COMMUNICATION;
#if COMM_PNC_SUPPORT == STD_ON
    (void)ComM_UserGetCurrentPNCComMode(User, &currentMode);
#endif
    ComM_ModeType currentComMode = ComM_UserGetCurrentComMode(User);
    return (currentMode < currentComMode) ? currentMode : currentComMode;
}

void ComM_UserSwitchCurrentMode(ComM_UserHandleType User, ComM_ModeType ComMode)
{
    const ComM_UserConfigType* userConfigPtr = &ComM_UserConfigs[User];
    if (userConfigPtr->SwitchCurrentMode != NULL_PTR)
    {
        switch (ComMode)
        {
        case COMM_FULL_COMMUNICATION:
            userConfigPtr->SwitchCurrentMode(RTE_MODE_ComMMode_COMM_FULL_COMMUNICATION);
            break;
        case COMM_SILENT_COMMUNICATION:
            userConfigPtr->SwitchCurrentMode(RTE_MODE_ComMMode_COMM_SILENT_COMMUNICATION);
            break;
        case COMM_NO_COMMUNICATION:
            userConfigPtr->SwitchCurrentMode(RTE_MODE_ComMMode_COMM_NO_COMMUNICATION);
            break;
        default:
            /* unreachable */
            break;
        }
    }
}
#endif

#if COMM_MULTIPLE_PARTITION_ENABLED == STD_ON
ApplicationType ComM_UserGetApplication(ComM_UserHandleType User)
{
    return ComM_UserConfigs[User].Application;
}
#endif

/* ========================================== internal function definitions ========================================= */
/* PRQA S 1532, 2889 -- */ /* VL_QAC_OneFunRef, VL_QAC_MultiReturn */
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"
