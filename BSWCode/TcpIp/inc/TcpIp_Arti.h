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
 **  @file               : TcpIp_Arti.h
 **  @author             : darren.zhang
 **  @date               :
 **  @vendor             : isoft
 **  @description        : ARTI trace interface of TcpIp
 **
 ***********************************************************************************************************************/

#ifndef TCPIP_ARTI_H_
#define TCPIP_ARTI_H_

/* =================================================== inclusions =================================================== */
#include "Arti.h"
#include "TcpIp_Cfg.h"

/* ================================================== trace macros ================================================== */

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x01_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x01_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x01_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x01_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x02_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x02_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x02_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x02_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x03_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x03_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x03_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x03_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x04_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x04_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x04_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x04_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x05_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x05_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x05_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x05_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x06_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x06_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x06_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x06_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x07_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x07_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x07_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x07_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x08_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x08_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x08_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x08_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x09_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x09_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x09_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x09_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x0A_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x0A_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4, param5) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x0A_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x0A_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x0B_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x0B_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x0B_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x0B_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x0C_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x0C_Bsw_Start_USER( \
    instanceParameter,                                      \
    param1,                                                 \
    param2,                                                 \
    param3,                                                 \
    param4,                                                 \
    param5,                                                 \
    param6,                                                 \
    param7)                                                 \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x0C_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x0C_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x0D_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x0D_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x0D_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x0D_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x0E_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x0E_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x0E_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x0E_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x0F_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x0F_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x0F_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x0F_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x10_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x10_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x10_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x10_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x11_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x11_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x11_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x11_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x12_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x12_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x12_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x12_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x13_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x13_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x13_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x13_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x14_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x14_Bsw_Start_USER( \
    instanceParameter,                                      \
    param1,                                                 \
    param2,                                                 \
    param3,                                                 \
    param4,                                                 \
    param5,                                                 \
    param6)                                                 \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x14_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x14_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#if (STD_ON == TCPIP_PARTITION_USED)
#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x15_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x15_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif
#else
#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x15_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x15_Bsw_Start_USER(instanceParameter) ((void)(instanceParameter))
#endif
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x15_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x15_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x16_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x16_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x16_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x16_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x17_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x17_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x17_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x17_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x18_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x18_Bsw_Start_USER( \
    instanceParameter,                                      \
    param1,                                                 \
    param2,                                                 \
    param3,                                                 \
    param4,                                                 \
    param5,                                                 \
    param6,                                                 \
    param7)                                                 \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x18_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x18_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x19_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x19_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x19_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x19_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x1A_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x1A_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x1A_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x1A_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x1B_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x1B_Bsw_Start_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x1B_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x1B_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x1C_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x1C_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x1C_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x1C_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x1D_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x1D_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x1D_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x1D_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x45_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x45_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x45_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x45_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x46_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x46_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x46_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_TcpIp_0x46_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#endif
