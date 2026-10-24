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
 **  @file               : Tm_Arti.h
 **  @author             : mengbo.li
 **  @date               : 2026/06/04
 **  @vendor             : isoft
 **  @description        : ARTI trace interface of Tm
 **
 ***********************************************************************************************************************/

#ifndef TM_ARTI_H_
#define TM_ARTI_H_

/* =================================================== inclusions =================================================== */
#include "Arti.h"

/* ===================================================== macros ===================================================== */

#define Tm_ArtiApiStart(num, ...) ARTI_TRACE_N(USER, AR_CP_BSW_API, Tm, 0, Bsw_Start, num, __VA_ARGS__)
#define Tm_ArtiApiStop(num, ...)  ARTI_TRACE_N(USER, AR_CP_BSW_API, Tm, 0, Bsw_Return, num, __VA_ARGS__)

/* TM_INIT_ID = 0x01u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Tm_0x01u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Tm_0x01u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Tm_0x01u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Tm_0x01u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* TM_GETVERSIONINFO_ID = 0x02u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Tm_0x02u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Tm_0x02u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Tm_0x02u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Tm_0x02u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* TM_RESETTIMER_ID = 0x03u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Tm_0x03u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Tm_0x03u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Tm_0x03u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Tm_0x03u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* TM_GETTIMESPAN_ID = 0x04u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Tm_0x04u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Tm_0x04u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Tm_0x04u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Tm_0x04u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* TM_SHIFTTIMER_ID = 0x05u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Tm_0x05u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Tm_0x05u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Tm_0x05u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Tm_0x05u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* TM_SYNCTIMER_ID = 0x06u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Tm_0x06u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Tm_0x06u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Tm_0x06u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Tm_0x06u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* TM_BUSYWAIT1US_ID = 0x07u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Tm_0x07u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Tm_0x07u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Tm_0x07u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Tm_0x07u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif
/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

/* ========================================== internal function definitions ========================================= */

#endif /* TM_ARTI_H_ */
