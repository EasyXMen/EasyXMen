
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
 **  @file               : DoIP_Arti.h
 **  @author             : tao.yu
 **  @date               : 2026/06/04
 **  @vendor             : isoft
 **  @description        : Declaration for DoIP
 **
 ***********************************************************************************************************************/

#ifndef DOIP_ARTI_H
#define DOIP_ARTI_H

/* =================================================== inclusions =================================================== */
#include "Arti.h"

/* =================================================== trace macros ===================================================
 */

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x00_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x00_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x00_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x00_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x01_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x01_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x01_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x01_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x02_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x02_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x02_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x02_Bsw_Start_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x0E_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x0E_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x0E_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x0E_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x0b_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x0b_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x0b_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x0b_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x0c_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x0c_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x0c_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x0c_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x40_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x40_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x40_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x40_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x42_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x42_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x42_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x42_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x43_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x43_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x43_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x43_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x44_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x44_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x44_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x44_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x45_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x45_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x45_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x45_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x46_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x46_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x46_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x46_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x48_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x48_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x48_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x48_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x49_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x49_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x49_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x49_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x4A_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x4A_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x4A_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x4A_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x4C_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x4C_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x4C_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x4C_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x53_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x53_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x53_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x53_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x54_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x54_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0x54_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0x54_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0xFF_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0xFF_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_DoIP_0xFF_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_DoIP_0xFF_Bsw_Start_USER(instanceParameter) ((void)(instanceParameter))
#endif

#endif
