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
 ************************************************************************************************************************
 **
 **  @file               : BswM_Sd.h
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : BSW Mode Manager
 **
 ***********************************************************************************************************************/

#ifndef BSWM_SD_H
#define BSWM_SD_H

/* =================================================== inclusions =================================================== */
#include "BswM_Cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
#if (BSWM_SD_ENABLED == STD_ON)

/**
 * @brief       Function called by Service Discovery to indicate the current state of the Client Service
 * (available/down).
 * @param[in]   SdClientServiceHandleId : HandleId to identify the ClientService.
 * @param[in]   CurrentClientState : Current state of the ClientService.
 * @return      None
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-66336
 */
void BswM_Sd_ClientServiceCurrentState(
    uint16                           SdClientServiceHandleId,
    Sd_ClientServiceCurrentStateType CurrentClientState);

/**
 * @brief       Function called by Service Discovery to indicate the current status of the Consumed Eventgroup
 * (available/down).
 * @param[in]   SdConsumedEventGroupHandleId : HandleId to identify the Consumed Eventgroup.
 * @param[in]   ConsumedEventGroupState : Status of the Consumed Eventgroup.
 * @return      None
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-66338
 */
void BswM_Sd_ConsumedEventGroupCurrentState(
    uint16                                SdConsumedEventGroupHandleId,
    Sd_ConsumedEventGroupCurrentStateType ConsumedEventGroupState);

/**
 * @brief       Function called by Service Discovery to indicate the current status of the EventHandler
 * (requested/released).
 * @param[in]   SdEventHandlerHandleId : HandleId to identify the EventHandler.
 * @param[in]   EventHandlerStatus : Status of the EventHandler.
 * @return      None
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-66340
 */
void BswM_Sd_EventHandlerCurrentState(
    uint16                          SdEventHandlerHandleId,
    Sd_EventHandlerCurrentStateType EventHandlerStatus);

#endif /*BSWM_SD_ENABLED == STD_ON*/

#ifdef __cplusplus
}
#endif
#endif /* BSWM_SD_H */
