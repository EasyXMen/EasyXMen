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
 **  @file               : Bfx_Static.h
 **  @author             : ShenXu,Yb,QinmeiChen
 **  @date               : 2024/09/09
 **  @vendor             : isoft
 **  @description        : Bfx internal bit constants
 **
 ***********************************************************************************************************************/
/* ================================================ misar justifications ============================================ */
#ifndef BFX_STATIC_H
#define BFX_STATIC_H
/* =================================================== inclusions =================================================== */
#include "Std_Types.h"
/* ===================================================== macros ===================================================== */
/* ================================================ Bit constants =================================================== */
#define BFX_CONST_BIT8_ONE       (uint8)1
#define BFX_CONST_BIT8_TWO       (uint8)2
#define BFX_CONST_BIT8_BITLEN    (uint8)8
#define BFX_CONST_BIT8_LSB_BITPN (uint8)0
#define BFX_CONST_BIT8_MSB_BITPN (uint8)7

#define BFX_CONST_BIT16_ONE       (uint16)1
#define BFX_CONST_BIT16_BITLEN    (uint8)16
#define BFX_CONST_BIT16_LSB_BITPN (uint8)0
#define BFX_CONST_BIT16_MSB_BITPN (uint8)15

#define BFX_CONST_BIT32_ONE       1uL
#define BFX_CONST_BIT32_BITLEN    (uint8)32
#define BFX_CONST_BIT32_LSB_BITPN (uint8)0
#define BFX_CONST_BIT32_MSB_BITPN (uint8)31

#define BFX_CONST_BIT64_ONE       1uLL
#define BFX_CONST_BIT64_BITLEN    (uint8)64
#define BFX_CONST_BIT64_LSB_BITPN (uint8)0
#define BFX_CONST_BIT64_MSB_BITPN (uint8)63

#endif
