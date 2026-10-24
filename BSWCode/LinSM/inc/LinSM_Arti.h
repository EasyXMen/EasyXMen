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
 **  @file               : LinSM_Arti.h
 **  @author             : xiaojian.liang
 **  @date               : 2026/06/03
 **  @vendor             : isoft
 **  @description        : ARTI trace interface of LinSM
 **
 ***********************************************************************************************************************/

#ifndef LINSM_ARTI_H_
#define LINSM_ARTI_H_

/* =================================================== inclusions =================================================== */
#include "Arti.h"

/* ===================================================== macros ===================================================== */

/* LINSM_SERVICE_ID_INIT = 0x01 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_LinSM_0x01u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinSM_0x01u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_LinSM_0x01u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinSM_0x01u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* LINSM_SERVICE_ID_GET_VERSIONINFO = 0x02 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_LinSM_0x02u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinSM_0x02u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_LinSM_0x02u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinSM_0x02u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* LINSM_SERVICE_ID_GOTOSLEEP_INDICATION = 0x03 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_LinSM_0x03u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinSM_0x03u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_LinSM_0x03u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinSM_0x03u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* LINSM_SERVICE_ID_SCHEDULE_REQUEST = 0x10 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_LinSM_0x10u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinSM_0x10u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_LinSM_0x10u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinSM_0x10u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* LINSM_SERVICE_ID_GET_CURRENTCOMMODE = 0x11 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_LinSM_0x11u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinSM_0x11u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_LinSM_0x11u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinSM_0x11u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* LINSM_SERVICE_ID_REQUEST_COMMODE = 0x12 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_LinSM_0x12u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinSM_0x12u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_LinSM_0x12u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinSM_0x12u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* LINSM_SERVICE_ID_SCHEDULEREQUEST_CONFIRMATION = 0x20 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_LinSM_0x20u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinSM_0x20u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_LinSM_0x20u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinSM_0x20u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* LINSM_SERVICE_ID_WAKEUP_CONFIRMATION = 0x21 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_LinSM_0x21u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinSM_0x21u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_LinSM_0x21u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinSM_0x21u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* LINSM_SERVICE_ID_GOTOSLEEP_CONFIRMATION = 0x22 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_LinSM_0x22u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinSM_0x22u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_LinSM_0x22u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinSM_0x22u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* LINSM_SERVICE_ID_MAIN_FUNCTION = 0x30 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_LinSM_0x30u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinSM_0x30u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_LinSM_0x30u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinSM_0x30u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* Wrapper macros */
#define LinSM_ArtiApiStart(numEventParameters, ...) \
    ARTI_TRACE_N(USER, AR_CP_BSW_API, LinSM, 0, Bsw_Start, numEventParameters, __VA_ARGS__)
#define LinSM_ArtiApiReturn(numEventParameters, ...) \
    ARTI_TRACE_N(USER, AR_CP_BSW_API, LinSM, 0, Bsw_Return, numEventParameters, __VA_ARGS__)

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

/* ========================================== internal function definitions ========================================= */

#endif /* LINSM_ARTI_H_ */
