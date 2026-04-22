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
 **  @file               : BswM_NvM.h
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : BSW Mode Manager
 **
 ***********************************************************************************************************************/

#ifndef BSWM_NVM_H
#define BSWM_NVM_H

/* =================================================== inclusions =================================================== */
#include "BswM_Cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
#if (BSWM_NVM_ENABLED == STD_ON)

/**
 * @brief       Function called by NvM to inform the BswM about the current state of a multi block job.
 * @param[in]   ServiceId : Indicates whether the callback refers to multi block services NvM_ReadAll or NvM_WriteAll.
 * @param[in]   CurrentJobMode : Current state of the multi block job indicated by parameter ServiceId.
 * @return      None
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-66331
 */
void BswM_NvM_CurrentJobMode(NvM_MultiBlockRequestType MultiBlockRequest, NvM_RequestResultType CurrentJobMode);

/**
 * @brief       Function called by NvM to indicate the current block mode of an NvM block.
 *              To use this function, integration code will be needed.
 * @param[in]   Block : The block that the new NvM mode corresponds to.
 * @param[in]   CurrentBlockMode : The current block mode of the NvM block.
 * @return      None
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-66333
 */
void BswM_NvM_CurrentBlockMode(NvM_BlockIdType Block, NvM_RequestResultType CurrentBlockMode);

#endif /*BSWM_NVM_ENABLED == STD_ON*/

#ifdef __cplusplus
}
#endif
#endif /* BSWM_NVM_H */
