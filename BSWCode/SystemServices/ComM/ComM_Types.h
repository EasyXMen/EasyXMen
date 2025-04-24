
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
 ************************************************************************************************************************
 ** **
 **  @file               : ComM_Types.h **
 **  @version            : V1.0.0 **
 **  @author             : darren.zhang **
 **  @date               : 2022/01/07 **
 **  @vendor             : isoft **
 **  @description        : Communication Manager **
 **  @specification(s)   : AUTOSAR classic Platform R19-11                                                **
 ** **
 ***********************************************************************************************************************/

#ifndef COMM_TYPES_H_
#define COMM_TYPES_H_
/*=================================================[inclusions]=======================================================*/
#include "ComStack_Types.h"
#include "Rte_ComM_Type.h"

/*====================================================[macros]========================================================*/
/* comm Std_ReturnType */
/** @ref SWS_ComM_91027 [draft]*/
/* Function call has been successful but mode can not be granted because of mode inhibition */
#define COMM_E_MODE_LIMITATION 2u
/* Function could not provide the current mode of the PNC, since multiple PNCs are assigned to the affected user */
#define COMM_E_MULTIPLE_PNC_ASSIGNED 3u
/* Function could not provide the current mode of the PNC, since no PNC is assigned to the affected user */
#define COMM_E_NO_PNC_ASSIGNED 4u
/* Function call has been successfully, but functionality cannot be executed because PNC learning phase is active. */
#define COMM_E_LEARNING_ACTIVE 5u

/*=============================================[type definitions]=====================================================*/

/** \brief Initialization status of ComM. */
typedef uint8 ComM_InitStatusType;
/* !LINKPRD SWS_ComM_00668*/
/* The COM Manager is not initialized or not usable. This shall be the default value after reset.
This status shall have the value 0 */
#define COMM_UNINIT 0x0u
/* The COM Manager is initialized and usable. */
#define COMM_INIT 0x1u

/** @ref SWS_ComM_00674
 * ComM_StateType
 */
typedef uint8 ComM_StateType;
#define COMM_NO_COM_NO_PENDING_REQUEST  0u
#define COMM_NO_COM_REQUEST_PENDING     1u
#define COMM_FULL_COM_NETWORK_REQUESTED 2u
#define COMM_FULL_COM_READY_SLEEP       3u
#define COMM_SILENT_COM                 4u

/* ComM_PncModeType */
typedef uint8 ComM_PncModeType;
#define COMM_PNC_REQUESTED                     0x00u
#define COMM_PNC_READY_SLEEP                   0x01u
#define COMM_PNC_PREPARE_SLEEP                 0x02u
#define COMM_PNC_NO_COMMUNICATION              0x03u
#define COMM_PNC_REQUESTED_WITH_WAKEUP_REQUEST 0x04u

#endif
