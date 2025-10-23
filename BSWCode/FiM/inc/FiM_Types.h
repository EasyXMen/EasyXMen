/**
 * Copyright (C) 2008-2025 isoft Infrastructure Software Co., Ltd.
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
**  @file               : FiM.h
**  @author             : tao.yu
**  @date               : 2024/12/04
**  @vendor             : isoft
**  @description        : Type definitions of FiM
**
***********************************************************************************************************************/
#ifndef FIM_TYPES_H_
#define FIM_TYPES_H_

/* =================================================== inclusions =================================================== */
#include "Rte_FiM_Type.h"
#include "FiM_OptMacros.h"

/**
 * @brief This type is used to distinguish an initial FID state from a FID state change callback call.
 * Range: 0..1
 */
typedef uint8 FiM_FidStatusChangeType;
#define FIM_FID_STATUS_INIT    0x00u /** The callback is called triggered by the Fim initialisation. */
#define FIM_FID_STATUS_CHANGED 0x01u /** The callback is called triggered by a FID state change. */

#endif
