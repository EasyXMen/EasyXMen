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
 **  @file               : EthIf_Arti.h
 **  @author             : darren.zhang
 **  @date               :
 **  @vendor             : isoft
 **  @description        : ARTI trace interface of EthIf
 **
 ***********************************************************************************************************************/

#ifndef ETHIF_ARTI_H_
#define ETHIF_ARTI_H_

/* =================================================== inclusions =================================================== */
#include "Arti.h"
#include "EthIf_Cfg.h"

/* ================================================== trace macros ================================================== */

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x01_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x01_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x01_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x01_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x03_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x03_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x03_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x03_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x04_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x04_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x04_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x04_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x05_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x05_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x05_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x05_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x06_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x06_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x06_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x06_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x07_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x07_Bsw_Start_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x07_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x07_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x08_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x08_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x08_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x08_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x09_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x09_Bsw_Start_USER( \
    instanceParameter,                                      \
    param1,                                                 \
    param2,                                                 \
    param3,                                                 \
    param4,                                                 \
    param5,                                                 \
    param6)                                                 \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x09_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x09_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x0A_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x0A_Bsw_Start_USER( \
    instanceParameter,                                      \
    param1,                                                 \
    param2,                                                 \
    param3,                                                 \
    param4,                                                 \
    param5,                                                 \
    param6)                                                 \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x0A_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x0A_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x0B_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x0B_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x0B_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x0B_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x0C_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x0C_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x0C_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x0C_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x0D_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x0D_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x0D_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x0D_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x0E_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x0E_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x0E_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x0E_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x0F_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x0F_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x0F_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x0F_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_R23_11)
#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x10_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x10_Bsw_Start_USER( \
    instanceParameter,                                      \
    param1,                                                 \
    param2,                                                 \
    param3,                                                 \
    param4,                                                 \
    param5,                                                 \
    param6,                                                 \
    param7,                                                 \
    param8)                                                 \
    ((void)(instanceParameter))
#endif
#else
#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x10_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x10_Bsw_Start_USER( \
    instanceParameter,                                      \
    param1,                                                 \
    param2,                                                 \
    param3,                                                 \
    param4,                                                 \
    param5,                                                 \
    param6)                                                 \
    ((void)(instanceParameter))
#endif
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x10_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x10_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431)
#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x11_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x11_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif
#else
#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x11_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x11_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x11_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x11_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x12_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x12_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x12_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x12_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x13_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x13_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x13_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x13_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x15_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x15_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x15_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x15_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x17_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x17_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x17_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x17_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x18_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x18_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x18_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x18_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x19_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x19_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x19_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x19_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x1A_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x1A_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x1A_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x1A_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x1B_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x1B_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x1B_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x1B_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x20_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x20_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x20_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x20_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x21_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x21_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x21_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x21_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x23_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x23_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x23_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x23_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x24_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x24_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x24_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x24_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x25_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x25_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x25_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x25_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x28_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x28_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x28_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x28_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x29_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x29_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x29_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x29_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x2C_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x2C_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x2C_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x2C_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x2D_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x2D_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x2D_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x2D_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x30_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x30_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x30_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x30_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x38_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x38_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x38_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x38_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x39_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x39_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x39_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x39_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x40_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x40_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x40_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x40_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x41_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x41_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x41_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x41_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x43_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x43_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x43_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x43_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x44_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x44_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x44_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x44_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x45_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x45_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x45_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x45_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x46_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x46_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x46_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x46_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x47_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x47_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x47_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x47_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x48_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x48_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x48_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x48_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x49_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x49_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x49_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x49_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x4A_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x4A_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x4A_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x4A_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x4B_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x4B_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x4B_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x4B_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x4C_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x4C_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x4C_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x4C_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x4D_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x4D_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x4D_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x4D_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x4E_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x4E_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x4E_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x4E_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x4F_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x4F_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x4F_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x4F_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x50_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x50_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x50_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x50_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x51_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x51_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x51_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x51_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x52_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x52_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x52_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x52_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x53_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x53_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x53_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x53_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x54_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x54_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x54_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x54_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x55_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x55_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x55_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x55_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x56_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x56_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4, param5) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x56_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x56_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x57_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x57_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x57_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x57_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x58_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x58_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x58_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x58_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x59_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x59_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x59_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x59_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x5A_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x5A_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x5A_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x5A_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x5B_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x5B_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x5B_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x5B_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x5C_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x5C_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x5C_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x5C_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x5D_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x5D_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x5D_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x5D_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x5E_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x5E_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x5E_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x5E_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x5F_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x5F_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x5F_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x5F_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x60_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x60_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x60_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x60_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x61_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x61_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x61_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x61_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x62_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x62_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x62_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x62_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x63_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x63_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x63_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x63_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x64_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x64_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x64_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x64_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x65_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x65_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x65_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x65_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x66_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x66_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x66_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x66_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x67_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x67_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x67_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x67_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x68_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x68_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x68_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x68_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x69_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x69_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x69_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x69_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x6A_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x6A_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x6A_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x6A_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x78_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x78_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x78_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x78_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x95_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x95_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x95_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x95_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x96_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x96_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x96_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x96_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x97_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x97_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x97_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x97_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x98_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x98_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x98_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x98_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x99_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x99_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_EthIf_0x99_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EthIf_0x99_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#endif
