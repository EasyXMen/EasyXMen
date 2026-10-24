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
 **  @file               : Nm_Arti.h
 **  @author             : xiaojian.liang
 **  @date               : 2026/06/03
 **  @vendor             : isoft
 **  @description        : ARTI trace interface of Nm
 **
 ***********************************************************************************************************************/

#ifndef NM_ARTI_H_
#define NM_ARTI_H_

/* =================================================== inclusions =================================================== */
#include "Arti.h"

/* ===================================================== macros ===================================================== */

/* Stub macros for Nm service IDs */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x00u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x00u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x00u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x00u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x01u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x01u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x01u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x01u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x02u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x02u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x02u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x02u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x03u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x03u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x03u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x03u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x04u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x04u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x04u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x04u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x05u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x05u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x05u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x05u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x06u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x06u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x06u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x06u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x07u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x07u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x07u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x07u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x08u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x08u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x08u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x08u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x09u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x09u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x09u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x09u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x0au_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x0au_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x0au_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x0au_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x0bu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x0bu_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x0bu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x0bu_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x0du_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x0du_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x0du_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x0du_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x0eu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x0eu_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x0eu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x0eu_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x0fu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x0fu_Bsw_Start_USER(instanceParameter) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x0fu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x0fu_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x10u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x10u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x10u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x10u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x11u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x11u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x11u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x11u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x12u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x12u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x12u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x12u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x13u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x13u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x13u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x13u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x14u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x14u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x14u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x14u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x16u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x16u_Bsw_Start_USER(instanceParameter, param1, param2, param3) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x16u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x16u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x17u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x17u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x17u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x17u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x18u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x18u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x18u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x18u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x19u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x19u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x19u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x19u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x1du_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x1du_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x1du_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x1du_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x1eu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x1eu_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x1eu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x1eu_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x1fu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x1fu_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x1fu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x1fu_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x21u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x21u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x21u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x21u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x26u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x26u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x26u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x26u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x27u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x27u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x27u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x27u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x28u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x28u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Nm_0x28u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Nm_0x28u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* Wrapper macros */
#define Nm_ArtiApiStart(numEventParameters, ...) \
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Nm, 0, Bsw_Start, numEventParameters, __VA_ARGS__)
#define Nm_ArtiApiReturn(numEventParameters, ...) \
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Nm, 0, Bsw_Return, numEventParameters, __VA_ARGS__)

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

/* ========================================== internal function definitions ========================================= */

#endif /* NM_ARTI_H_ */
