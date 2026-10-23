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
 **  @file               : WdgM_Arti.h
 **  @author             : mengbo.li
 **  @date               : 2026/06/04
 **  @vendor             : isoft
 **  @description        : ARTI trace interface of WdgM
 **
 ***********************************************************************************************************************/

#ifndef WDGM_ARTI_H_
#define WDGM_ARTI_H_

/* =================================================== inclusions =================================================== */
#include "Arti.h"

/* ===================================================== macros ===================================================== */

#define WdgM_ArtiApiStart(num, ...) ARTI_TRACE_N(USER, AR_CP_BSW_API, WdgM, 0, Bsw_Start, num, __VA_ARGS__)
#define WdgM_ArtiApiStop(num, ...)  ARTI_TRACE_N(USER, AR_CP_BSW_API, WdgM, 0, Bsw_Return, num, __VA_ARGS__)

/* WDGM_INIT_ID = 0x00u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_WdgM_0x00u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_WdgM_0x00u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_WdgM_0x00u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_WdgM_0x00u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* WDGM_DEINIT_ID = 0x01u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_WdgM_0x01u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_WdgM_0x01u_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_WdgM_0x01u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_WdgM_0x01u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* WDGM_GET_VERSION_INFO_ID = 0x02u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_WdgM_0x02u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_WdgM_0x02u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_WdgM_0x02u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_WdgM_0x02u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* WDGM_SET_MODE_ID = 0x03u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_WdgM_0x03u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_WdgM_0x03u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_WdgM_0x03u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_WdgM_0x03u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* WDGM_MAINFUNCTION_ID = 0x08u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_WdgM_0x08u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_WdgM_0x08u_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_WdgM_0x08u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_WdgM_0x08u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* WDGM_GET_MODE_ID = 0x0Bu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_WdgM_0x0Bu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_WdgM_0x0Bu_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_WdgM_0x0Bu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_WdgM_0x0Bu_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* WDGM_GET_LOCAL_STATUS_ID = 0x0Cu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_WdgM_0x0Cu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_WdgM_0x0Cu_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_WdgM_0x0Cu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_WdgM_0x0Cu_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* WDGM_GET_GLOBAL_STATUS_ID = 0x0Du */
#ifndef ARTI_TRACE_AR_CP_BSW_API_WdgM_0x0Du_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_WdgM_0x0Du_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_WdgM_0x0Du_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_WdgM_0x0Du_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* WDGM_CHECKPOINT_REACHED_ID = 0x0Eu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_WdgM_0x0Eu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_WdgM_0x0Eu_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_WdgM_0x0Eu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_WdgM_0x0Eu_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* WDGM_PERFORM_RESET_ID = 0x0Fu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_WdgM_0x0Fu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_WdgM_0x0Fu_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_WdgM_0x0Fu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_WdgM_0x0Fu_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* WDGM_GET_FIRST_EXPIRED_SEID_ID = 0x10u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_WdgM_0x10u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_WdgM_0x10u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_WdgM_0x10u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_WdgM_0x10u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif
/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

/* ========================================== internal function definitions ========================================= */

#endif /* WDGM_ARTI_H_ */
