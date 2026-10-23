
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
 **  @file               : Dem_Arti.h
 **  @author             : tao.yu
 **  @date               : 2026/06/04
 **  @vendor             : isoft
 **  @description        : Declaration for Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_ARTI_H
#define DEM_ARTI_H

/* =================================================== inclusions =================================================== */
#include "Arti.h"

/* =================================================== trace macros ===================================================
 */

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x00_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x00_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x00_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x00_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x01_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x01_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x01_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x01_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x02_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x02_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x02_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x02_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x03_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x03_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x03_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x03_Bsw_Start_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x04_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x04_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x04_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x04_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x05_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x05_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x05_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x05_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x06_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x06_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x06_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x06_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x07_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x07_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x07_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x07_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x08_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x08_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x08_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x08_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x09_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x09_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x09_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x09_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x0D_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x0D_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x0D_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x0D_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x0E_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x0E_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x0E_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x0E_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x13_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x13_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x13_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x13_Bsw_Start_USER( \
    instanceParameter,                                    \
    param1,                                               \
    param2,                                               \
    param3,                                               \
    param4,                                               \
    param5,                                               \
    param6,                                               \
    param7)                                               \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x15_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x15_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x15_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x15_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x16_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x16_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x16_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x16_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x17_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x17_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x17_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x17_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x18_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x18_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x18_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x18_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x19_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x19_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x19_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x19_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x1A_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x1A_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x1A_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x1A_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x1B_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x1B_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x1B_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x1B_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x1D_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x1D_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x1D_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x1D_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x1F_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x1F_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x1F_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x1F_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x20_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x20_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x20_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x20_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x21_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x21_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x21_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x21_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x23_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x23_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x23_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x23_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x24_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x24_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x24_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x24_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x25_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x25_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x25_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x25_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x29_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x29_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x29_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x29_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x2A_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x2A_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x2A_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x2A_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x2B_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x2B_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x2B_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x2B_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x32_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x32_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x32_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x32_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x33_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x33_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x33_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x33_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x34_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x34_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x34_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x34_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x35_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x35_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x35_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x35_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x37_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x37_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x37_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x37_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x38_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x38_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x38_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x38_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x39_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x39_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x39_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x39_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x3A_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x3A_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x3A_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x3A_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x3B_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x3B_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x3B_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x3B_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x3C_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x3C_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x3C_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x3C_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x3D_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x3D_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x3D_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x3D_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4, param5) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x3E_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x3E_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x3E_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x3E_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x3F_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x3F_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x3F_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x3F_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x51_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x51_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x51_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x51_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x52_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x52_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x52_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x52_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x53_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x53_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x53_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x53_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x55_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x55_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x55_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x55_Bsw_Start_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x56_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x56_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x56_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x56_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x57_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x57_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x57_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x57_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x5A_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x5A_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x5A_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x5A_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x61_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x61_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x61_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x61_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x63_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x63_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x63_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x63_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x64_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x64_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x64_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x64_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x65_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x65_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x65_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x65_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x66_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x66_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x66_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x66_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x67_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x67_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x67_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x67_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x68_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x68_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x68_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x68_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x69_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x69_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x69_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x69_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x6A_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x6A_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x6A_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x6A_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x6B_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x6B_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x6B_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x6B_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x6C_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x6C_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x6C_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x6C_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x6D_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x6D_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x6D_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x6D_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x6E_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x6E_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x6E_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x6E_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4, param5) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x72_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x72_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x72_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x72_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x73_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x73_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x73_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x73_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x79_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x79_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x79_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x79_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x7A_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x7A_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x7A_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x7A_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x90_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x90_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x90_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x90_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4, param5) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x91_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x91_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x91_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x91_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x92_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x92_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x92_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x92_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x93_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x93_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x93_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x93_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x94_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x94_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x94_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x94_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x95_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x95_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x95_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x95_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x96_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x96_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x96_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x96_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x97_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x97_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x97_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x97_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4, param5) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x98_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x98_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x98_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x98_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x99_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x99_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x99_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x99_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x9A_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x9A_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x9A_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x9A_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x9B_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x9B_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x9B_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x9B_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x9C_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x9C_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x9C_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x9C_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x9D_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x9D_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x9D_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x9D_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x9F_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x9F_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0x9F_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0x9F_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xA2_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xA2_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xA2_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xA2_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4, param5) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xA3_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xA3_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xA3_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xA3_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xA4_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xA4_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xA4_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xA4_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xA5_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xA5_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xA5_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xA5_Bsw_Start_USER( \
    instanceParameter,                                    \
    param1,                                               \
    param2,                                               \
    param3,                                               \
    param4,                                               \
    param5,                                               \
    param6,                                               \
    param7)                                               \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xA6_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xA6_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xA6_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xA6_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xA7_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xA7_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xA7_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xA7_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xA8_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xA8_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xA8_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xA8_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xA9_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xA9_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xA9_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xA9_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xAB_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xAB_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xAB_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xAB_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xAE_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xAE_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xAE_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xAE_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xAF_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xAF_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xAF_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xAF_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xB1_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xB1_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xB1_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xB1_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xB2_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xB2_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xB2_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xB2_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xB3_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xB3_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xB3_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xB3_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xB4_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xB4_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xB4_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xB4_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xB5_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xB5_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xB5_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xB5_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xB6_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xB6_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xB6_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xB6_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xB7_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xB7_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xB7_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xB7_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xB8_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xB8_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xB8_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xB8_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xB9_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xB9_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xB9_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xB9_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xBA_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xBA_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xBA_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xBA_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xBB_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xBB_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xBB_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xBB_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xBC_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xBC_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xBC_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xBC_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xBD_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xBD_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xBD_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xBD_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xBE_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xBE_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xBE_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xBE_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xC1_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xC1_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xC1_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xC1_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xC2_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xC2_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xC2_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xC2_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xC3_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xC3_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xC3_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xC3_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xC4_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xC4_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xC4_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xC4_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xC5_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xC5_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xC5_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xC5_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xF0_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xF0_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xF0_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xF0_Bsw_Start_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xF1_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xF1_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xF1_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xF1_Bsw_Start_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xF2_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xF2_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xF2_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xF2_Bsw_Start_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xF3_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xF3_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xF3_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xF3_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xF4_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xF4_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xF4_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xF4_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xF5_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xF5_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xF5_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xF5_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xF6_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xF6_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_Dem_0xF6_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Dem_0xF6_Bsw_Start_USER(instanceParameter, param1, param2) ((void)(instanceParameter))
#endif

#endif
