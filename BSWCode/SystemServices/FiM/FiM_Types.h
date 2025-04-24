/* PRQA S 3108++ */
/**
 * Copyright (C) 2024 Isoft Infrastructure Software Co., Ltd.
 * SPDX-License-Identifier: LGPL-2.1-only-with-exception OR  LicenseRef-Commercial-License
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the
 * GNU Lesser General Public License as published by the Free Software Foundation; version 2.1.
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License along with this library;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * or see <https://www.gnu.org/licenses/>.
 *
 * Alternatively, this file may be used under the terms of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License, in which case the provisions of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License shall apply instead of those of the GNU Lesser General Public License.
 *
 * You should have received a copy of the Isoft Infrastructure Software Co., Ltd.  Commercial License
 * along with this program. If not, please find it at <https://EasyXMen.com/xy/reference/permissions.html>
 */
/* PRQA S 3108-- */
/*
**************************************************************************** **
**                                                                           **
**  FILENAME    : FiM_Internal.h                                             **
**                                                                           **
**  Created on  : 2020/5/9 15:21:52                                          **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION : fim types                                                  **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */
#ifndef FIM_TYPES_H_
#define FIM_TYPES_H_

/******************************************************************************
**                      Includes                                             **
******************************************************************************/
#include "Std_Types.h"
#include "Rte_FiM_Type.h"

/******************************************************************************
**                      Global Symbols                                       **
******************************************************************************/

/******************************************************************************
**                      Private Type Definitions                             *
******************************************************************************/

/* The configuration parameter is used to specify the inhibition mask
for an event - FID relation */
typedef enum
{
    FIM_LAST_FAILED = 0x0u,
    FIM_NOT_TESTED = 0x1u,
    FIM_TESTED = 0x2u,
    FIM_TESTED_AND_FAILED = 0x3u
} FiMInhInhibitionMaskType;

#endif /* FIM_TYPES_H_ */
