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
 **  @file               : WdgIf_Types.h
 **  @author             : xudong.guan
 **  @date               : 2024/08/20
 **  @vendor             : isoft
 **  @description        : Type definition for WdgIf
 **
 ***********************************************************************************************************************/
#ifndef WDGIF_TYPES_H
#define WDGIF_TYPES_H

/* ================================================ type definitions ================================================ */

/* Type of WDG Mode */
typedef enum
{
    /* OFF Mode */
    WDGIF_OFF_MODE,
    /* SLOW Mode */
    WDGIF_SLOW_MODE,
    /* FAST Mode */
    WDGIF_FAST_MODE
} WdgIf_ModeType;

#endif /* WDGIF_TYPES_H */
