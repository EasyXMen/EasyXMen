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
 **  @file               : ComM_Arti.h
 **  @author             : xiaojian.liang
 **  @date               : 2026/06/03
 **  @vendor             : isoft
 **  @description        : ARTI trace interface of ComM
 **
 ***********************************************************************************************************************/

#ifndef COMM_ARTI_H_
#define COMM_ARTI_H_

/* =================================================== inclusions =================================================== */
#include "Arti.h"

/* ===================================================== macros ===================================================== */

/* COMM_SID_INIT = 0x01 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x01u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x01u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x01u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x01u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* COMM_SID_DEINIT = 0x02 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x02u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x02u_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x02u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x02u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* COMM_SID_GETSTATUS = 0x03 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x03u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x03u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x03u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x03u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* COMM_SID_GETINHIBITIONSTATUS = 0x04 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x04u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x04u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x04u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x04u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* COMM_SID_REQUESTCOMMODE = 0x05 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x05u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x05u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x05u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x05u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* COMM_SID_GETMAXCOMMODE = 0x06 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x06u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x06u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x06u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x06u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* COMM_SID_GETREQUESTEDCOMMODE = 0x07 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x07u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x07u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x07u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x07u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* COMM_SID_GETCURRENTCOMMODE = 0x08 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x08u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x08u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x08u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x08u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* COMM_SID_PREVENTWAKEUP = 0x09 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x09u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x09u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x09u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x09u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* COMM_SID_LIMITCHANNELTONOCOMMODE = 0x0b */
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x0bu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x0bu_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x0bu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x0bu_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* COMM_SID_LIMITECUTONOCOMMODE = 0x0c */
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x0cu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x0cu_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x0cu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x0cu_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* COMM_SID_READINHIBITCOUNTER = 0x0d */
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x0du_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x0du_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x0du_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x0du_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* COMM_SID_RESETINHIBITCOUNTER = 0x0e */
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x0eu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x0eu_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x0eu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x0eu_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* COMM_SID_SETECUGROUPCLASSIFICATION = 0x0f */
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x0fu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x0fu_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x0fu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x0fu_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* COMM_SID_GETVERSIONINFO = 0x10 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x10u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x10u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x10u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x10u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* COMM_SID_NM_NETWORKSTARTINDICATION = 0x15 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x15u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x15u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x15u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x15u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* COMM_SID_NM_NETWORKMODE = 0x18 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x18u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x18u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x18u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x18u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* COMM_SID_NM_PREPAREBUSSLEEPMODE = 0x19 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x19u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x19u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x19u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x19u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* COMM_SID_NM_BUSSLEEPMODE = 0x1a */
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x1au_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x1au_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x1au_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x1au_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* COMM_SID_NM_RESTARTINDICATION = 0x1b */
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x1bu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x1bu_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x1bu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x1bu_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* COMM_SID_DCM_ACTIVEDIAGNOSTIC = 0x1f */
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x1fu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x1fu_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x1fu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x1fu_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* COMM_SID_DCM_INACTIVEDIAGNOSTIC = 0x20 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x20u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x20u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x20u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x20u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* COMM_SID_ECUM_WAKEUPINDICATION = 0x2a */
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x2au_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x2au_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x2au_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x2au_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* COMM_SID_BUSSM_MODEINDICATION = 0x33 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x33u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x33u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x33u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x33u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* COMM_SID_COMMUNICATIONALLOWED = 0x35 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x35u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x35u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x35u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x35u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* COMM_SID_ECUM_PNCWAKEUPINDICATION = 0x37 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x37u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x37u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x37u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x37u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* COMM_SID_MAINFUNCATION = 0x60 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x60u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x60u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x60u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x60u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* COMM_SID_GETCURRENTPNCCOMMODE = 0x6a */
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x6au_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x6au_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x6au_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x6au_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* COMM_SID_NMUPDATEEIRA = 0x6c */
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x6cu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x6cu_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x6cu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x6cu_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* COMM_SID_NMUPDATEERA = 0x6d */
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x6du_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x6du_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x6du_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x6du_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* COMM_SID_GETCHANNELAPPLICATIONID = 0x70 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x70u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x70u_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_ComM_0x70u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_ComM_0x70u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* Wrapper macros */
#define ComM_ArtiApiStart(numEventParameters, ...) \
    ARTI_TRACE_N(USER, AR_CP_BSW_API, ComM, 0, Bsw_Start, numEventParameters, __VA_ARGS__)
#define ComM_ArtiApiReturn(numEventParameters, ...) \
    ARTI_TRACE_N(USER, AR_CP_BSW_API, ComM, 0, Bsw_Return, numEventParameters, __VA_ARGS__)

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

/* ========================================== internal function definitions ========================================= */

#endif /* COMM_ARTI_H_ */
