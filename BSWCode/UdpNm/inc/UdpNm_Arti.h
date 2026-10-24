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
 **  @file               : UdpNm_Arti.h
 **  @author             : xiaojian.liang
 **  @date               : 2026/06/03
 **  @vendor             : isoft
 **  @description        : ARTI trace interface of UdpNm
 **
 ***********************************************************************************************************************/

#ifndef UDPNM_ARTI_H_
#define UDPNM_ARTI_H_

/* =================================================== inclusions =================================================== */
#include "Arti.h"

/* ===================================================== macros ===================================================== */

/* Stub macros for UdpNm service IDs */
#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x01u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x01u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x01u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x01u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x02u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x02u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x02u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x02u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x03u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x03u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x03u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x03u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x04u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x04u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x04u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x04u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x05u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x05u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x05u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x05u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x06u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x06u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x06u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x06u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x07u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x07u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x07u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x07u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x08u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x08u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x08u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x08u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x09u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x09u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x09u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x09u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x0au_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x0au_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x0au_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x0au_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x0bu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x0bu_Bsw_Start_USER(instanceParameter, param1, param2, param3) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x0bu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x0bu_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x0cu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x0cu_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x0cu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x0cu_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x0du_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x0du_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x0du_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x0du_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x0eu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x0eu_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x0eu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x0eu_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x11u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x11u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x11u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x11u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x13u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x13u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x13u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x13u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x14u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x14u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x14u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x14u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x16u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x16u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x16u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x16u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x40u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x40u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x40u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x40u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x41u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x41u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x41u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x41u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x42u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x42u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x42u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x42u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x49u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x49u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x49u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_UdpNm_0x49u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* Wrapper macros */
#define UdpNm_ArtiApiStart(numEventParameters, ...) \
    ARTI_TRACE_N(USER, AR_CP_BSW_API, UdpNm, 0, Bsw_Start, numEventParameters, __VA_ARGS__)
#define UdpNm_ArtiApiReturn(numEventParameters, ...) \
    ARTI_TRACE_N(USER, AR_CP_BSW_API, UdpNm, 0, Bsw_Return, numEventParameters, __VA_ARGS__)

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

/* ========================================== internal function definitions ========================================= */

#endif /* UDPNM_ARTI_H_ */
