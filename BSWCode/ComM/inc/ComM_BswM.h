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
 **  @file               : ComM_BswM.h
 **  @author             : xiaojian.liang
 **  @date               : 2024/08/21
 **  @vendor             : isoft
 **  @description        : Callback of ComM Implementation header for BswM
 **
 **********************************************************************************************************************/

#ifndef COMM_BSWM_H_
#define COMM_BSWM_H_

/* =================================================== inclusions =================================================== */
#include "ComM_Cfg.h"
#include "ComStack_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief EcuM or BswM shall indicate to ComM when communication is allowed. If EcuM/Flex is used: BswM
 * @param[in]    Channel   Channel
 * @param[in]    Allowed   TRUE: Communication is allowed FALSE: Communication is not allowed
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-59644
 */
void ComM_CommunicationAllowed(NetworkHandleType Channel, boolean Allowed);

#ifdef __cplusplus
}
#endif

#endif
