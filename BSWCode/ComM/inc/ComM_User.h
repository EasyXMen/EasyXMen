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
 **  @file               : ComM_User.h
 **  @author             : xiaojian.liang
 **  @date               : 2024/08/21
 **  @vendor             : isoft
 **  @description        : ComM User Implementation header
 **
 **********************************************************************************************************************/

#ifndef COMM_USER_H_
#define COMM_USER_H_

/* =================================================== inclusions =================================================== */
#include "ComM_Internal.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */
typedef struct ComM_UserConfigTypeTag /* PRQA S 1536 */ /* VL_ComM_NotUsed */
{
    const NetworkHandleType* Channels;
    uint8                    NumberOfChannels;

#if COMM_MULTIPLE_PARTITION_ENABLED == STD_ON
    ApplicationType Application;
#endif
#if COMM_CURRENT_MODE_NOTIFICATION_ENABLED == STD_ON
    void (*SwitchCurrentMode)(ComM_ModeType currentMode);
#endif
} ComM_UserConfigType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief Initializes the global variables for all users that are used across multiple partitions.
 * @param[in]     UserConfigs  Pointer to post-build configuration data for all users
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-64934
 */
void ComM_UserInit(const ComM_UserConfigType* UserConfigs);

/**
 * @brief Requesting of a Communication Mode by a user.
 * @param[in]    User     Handle of the user who requests a mode
 * @param[in]    ComMode  COMM_FULL_COMMUNICATION COMM_NO_COMMUNICATION
 * @retval       E_OK                    Successfully changed to the new mode
 * @retval       COMM_E_MODE_LIMITATION  Mode can not be granted because of mode inhibition.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-64935
 */
Std_ReturnType ComM_UserRequestComMode(ComM_UserHandleType User, ComM_ModeType ComMode);

/**
 * @brief Function to query the maximum allowed Communication Mode of the corresponding user.
 * @param[in]    User      Handle of the user who requests a mode
 * @return       See ComM_ModeType
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace       CPD-64936
 */
ComM_ModeType ComM_UserGetMaxComMode(ComM_UserHandleType User);

/**
 * @brief Function to query the currently requested Communication Mode of the corresponding user.
 * @param[in]    User      Handle of the user who requests a mode
 * @return       Name of the requested mode
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-64937
 */
ComM_ModeType ComM_UserGetRequestedComMode(ComM_UserHandleType User);

/**
 * @brief Function to query the current Communication Mode.
 * @param[in]    User      Handle of the user who requests a mode
 * @return       See ComM_ModeType
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-65025
 */
ComM_ModeType ComM_UserGetCurrentComMode(ComM_UserHandleType User);

#if COMM_PNC_SUPPORT == STD_ON
/**
 * @brief The function returns the current Communication Mode of the corresponding PNC the affected user is assigned to.
 * @param[in]    User      Handle of the user who requests a mode
 * @param[out]   ComMode   See ComM_ModeType
 * @retval       E_OK      Successfully returned the state of the PNC referenced by the given ComMUser
 * @retval       E_NOT_OK  Return of the PNC state referenced by the given ComMUser failed
 * @retval       COMM_E_MULTIPLE_PNC_ASSIGNED  Function could not provide the current mode of the PNC, since multiple
 *                                             PNCs are assigned to the affected user
 * @retval       COMM_E_NO_PNC_ASSIGNED        Function could not provide the current mode of the PNC, since no PNC is
 *                                             assigned to the affected user
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-64947
 */
Std_ReturnType ComM_UserGetCurrentPNCComMode(ComM_UserHandleType User, ComM_ModeType* ComMode);
#endif

#if COMM_CURRENT_MODE_NOTIFICATION_ENABLED == STD_ON
/**
 * @brief The function returns the lowest current Communication Mode of the corresponding PNC or channel the affected
 * user is assigned to.
 * @param[in]    User      Handle of the user who requests a mode
 * @retval       ComMode   See ComM_ModeType
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-64958
 */
ComM_ModeType ComM_UserGetCurrentMode(ComM_UserHandleType User);

/**
 * @brief A SW-C that wants to get informed about its current Communication Manager Module Mode requires the
 * ModeSwitchInterface ComM_CurrentMode.
 * @param[in]    User      Handle of the user who requests a mode
 * @param[in]    ComMode   See ComM_ModeType
 * @reentrant    FALSE
 * @synchronous  TRUE
 * @trace        CPD-64964
 */
void ComM_UserSwitchCurrentMode(ComM_UserHandleType User, ComM_ModeType ComMode);
#endif

#if COMM_MULTIPLE_PARTITION_ENABLED == STD_ON
/**
 * @brief Function to query the application of the corresponding user.
 * @param[in]    User  See ComM_UserHandleType
 * @return       The application of the corresponding user
 * @reentrant    TRUE
 * @synchronous  TRUE
 */
ApplicationType ComM_UserGetApplication(ComM_UserHandleType User);
#endif
#ifdef __cplusplus
}
#endif

#endif
