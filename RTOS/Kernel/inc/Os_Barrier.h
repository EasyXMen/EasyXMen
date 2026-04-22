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
 *
 ************************************************************************************************************************
 **
 **  @file               : Os_Barrier.h
 **  @author             : i-soft-os
 **  @date               : 2025/04/14
 **  @vendor             : isoft
 **  @description        : Os header file for barrier api declarations
 **
 ***********************************************************************************************************************/

 #ifndef OS_BARRIER_H
 #define OS_BARRIER_H
 
 /* =================================================== inclusions =================================================== */
 #include "Os_Types.h"

 /* ===================================================== macros ===================================================== */
 #define  BARRRIR_TICKET_THRESHOLD_VALUE    ((Os_BarrierTicketType)(1 << ((sizeof(Os_BarrierTicketType) * 8) - 1)))

 /* ========================================= external function declarations ========================================= */
 #if (CFG_BARRIER_MAX > 0U)
/**
 * @brief              Init the barrier control block.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
 extern void Os_BarrierInit(void);
 #endif


#endif
