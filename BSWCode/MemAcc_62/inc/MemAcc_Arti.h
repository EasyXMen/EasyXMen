
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
 **  @file               : MemAcc_Arti.h
 **  @author             : tao.yu
 **  @date               : 2026/06/05
 **  @vendor             : isoft
 **  @description        : Declaration for MemAcc
 **
 ***********************************************************************************************************************/

#ifndef MEMACC_ARTI_H
#define MEMACC_ARTI_H

/* =================================================== inclusions =================================================== */
#include "Arti.h"

/* =================================================== trace macros ===================================================
 */

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x01_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x01_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x01_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x01_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x02_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x02_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x02_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x02_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x03_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x03_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x03_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x03_Bsw_Start_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x04_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x04_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x04_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x04_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x05_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x05_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x05_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x05_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x06_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x06_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x06_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x06_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x07_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x07_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x07_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x07_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x08_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x08_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x08_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x08_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x09_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x09_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x09_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x09_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x0A_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x0A_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x0A_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x0A_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x0B_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x0B_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x0B_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x0B_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x0C_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x0C_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x0C_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x0C_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x0D_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x0D_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x0D_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x0D_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x0E_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x0E_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x0E_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x0E_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4, param5) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x10_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x10_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x10_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x10_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x11_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x11_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x11_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x11_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x12_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x12_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x12_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x12_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x13_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x13_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x13_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x13_Bsw_Start_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x14_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x14_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x14_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x14_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x15_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x15_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x15_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_MemAcc_0x15_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#endif
