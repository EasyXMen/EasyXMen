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
 **  @file               : BswM_Swc.h
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : BSW Mode Manager
 **
 ***********************************************************************************************************************/

#ifndef BSWM_SWC_H
#define BSWM_SWC_H

/* PRQA S 1753 EOF */ /* VL_BswM_1753 */

/* =================================================== inclusions =================================================== */
#include "BswM_Cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

#if (                                                                                       \
    (BSWM_SWCMODEREQUEST_ENABLED == STD_ON) || (BSWM_SWCMODENOTIFICATION_ENABLED == STD_ON) \
    || (BSWM_EVENT_MODE_SWITCH_ERROR_EVENT_ENABLE == STD_ON) || (BSWM_EVENT_MODE_SWITCH_ERROR_EVENT_ENABLE == STD_ON))

/* =============================================== version information ============================================== */

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

/**
 * @brief       This is a notification that an error occurred because the partition containing
 *              the mode users of the Mode Declaration Group Prototype was restarted by the RTE.
 *              Because the Mode Machine Instance holding the current mode can reside on
 *              that terminated partition, the Mode Manager has to be informed about the loss of this partition.
 * @param[in]   portId : The ID of the switch port.
 * @return      None
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-66345
 */
void BswM_SwcModeSwitchErrorNotification(uint16 portId);

/**
 * @brief       Notifies the system of a mode switch acknowledgment for a specified port
 * @param[in]   portId : The ID of the switch port.
 * @return      None
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-66346
 */
void BswM_SwcModeSwitchAckNotification(uint16 portId);

/**
 * @brief       Function called when a SWC has switched a mode.
 * @param[in]   modeGroup : The mode group in which the mode was switched.
 * @return      None
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-66347
 */
void BswM_SwcModeNotification(BswM_ModeGroupType modeGroup);

/**
 * @brief       Function called when a SWC requests a mode.
 * @param[in]   modeGroup : The mode group in which the mode was requested.
 * @return      None
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-66348
 */
/* PRQA S 1501 ++ */ /* VL_BswM_1501 */
void BswM_SwcModeRequest(BswM_ModeGroupType modeGroup);
/* PRQA S 1501 -- */

#endif /*BSWM_RTE_ENABLED == STD_ON*/

#ifdef __cplusplus
}
#endif

#endif /* BSWM_SWC_H */
