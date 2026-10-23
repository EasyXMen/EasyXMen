
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
 **  @file               : NvM_Arti.h
 **  @author             : tao.yu
 **  @date               : 2026/06/05
 **  @vendor             : isoft
 **  @description        : Declaration for NvM
 **
 ***********************************************************************************************************************/

#ifndef NVM_ARTI_H
#define NVM_ARTI_H

/* =================================================== inclusions =================================================== */
#include "Arti.h"

/* =================================================== trace macros ===================================================
 */

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x00_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x00_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x00_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x00_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x01_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x01_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x01_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x01_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x02_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x02_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x02_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x02_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x03_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x03_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x03_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x03_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x04_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x04_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x04_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x04_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x05_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x05_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x05_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x05_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x06_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x06_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x06_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x06_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x07_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x07_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x07_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x07_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x08_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x08_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x08_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x08_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x09_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x09_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x09_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x09_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x0A_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x0A_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x0A_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x0A_Bsw_Start_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x0B_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x0B_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x0B_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x0B_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x0C_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x0C_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x0C_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x0C_Bsw_Start_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x0D_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x0D_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x0D_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x0D_Bsw_Start_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x0E_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x0E_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x0E_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x0E_Bsw_Start_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x0F_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x0F_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x0F_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x0F_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x10_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x10_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x10_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x10_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x11_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x11_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x11_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x11_Bsw_Start_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x12_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x12_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x12_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x12_Bsw_Start_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x14_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x14_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x14_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x14_Bsw_Start_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x16_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x16_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x16_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x16_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x17_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x17_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x17_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x17_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x18_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x18_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x18_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x18_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x19_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x19_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_NvM_0x19_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_NvM_0x19_Bsw_Start_USER(instanceParameter) ((void)(instanceParameter))
#endif

#endif
