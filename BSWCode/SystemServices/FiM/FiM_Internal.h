/* PRQA S 3108++ */
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
/* PRQA S 3108-- */
/*
**************************************************************************** **
**                                                                           **
**  FILENAME    : FiM_Internal.h                                             **
**                                                                           **
**  Created on  : 2020/5/9 15:21:52                                          **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION : fim internal typedef                                       **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */

#ifndef FIM_INTERNAL_H_
#define FIM_INTERNAL_H_

/******************************************************************************
**                      Includes                                             **
******************************************************************************/

#include "FiM.h"

/******************************************************************************
**                      Private Macro Definitions                            **
******************************************************************************/

/******************************************************************************
**                      Private Type Definitions                             *
******************************************************************************/

#define FIM_FID_PERMISSION_STU_BIT ((uint8)0x01)
#define FIM_FID_AVAILABLE_BIT      ((uint8)0x02)

/* maximum the fid inhibition counter value */
#define FIM_INHIBITION_COUNTER_MAX 0xffffu

/* FiM module init step macro define */
#define FIM_INIT_STEP_NONE     ((uint8)0x0u)
#define FIM_INIT_STEP_LOCAL    ((uint8)0x1u)
#define FIM_INIT_STEP_DEM      ((uint8)0x2u)

#define FIM_DEM_UDS_STU_TF     ((uint8)0x01)
#define FIM_DEM_UDS_STU_TNCTOC ((uint8)0x40)

#if !defined(FIM_LOCAL)
#define FIM_LOCAL static
#endif /* !defined(FIM_LOCAL */

#if !defined(FIM_LOCAL_INLINE)
#define FIM_LOCAL_INLINE static inline
#endif /* !defined(FIM_LOCAL_INLINE */

/* funtion Identifier inner valiable */
typedef struct
{
    /* inhibition counter */
    uint16 inhCouter;
    /* permission state and available control */
    uint8 ctrlStu;
} FiM_InnerFuncIdStatusType;

/* event status valiable in fim module */
typedef struct
{
    boolean ComponentStu;
} FiM_InnerComponentSatusType;

/* event status valiable in fim module */
typedef struct
{
    Dem_UdsStatusByteType eventStu;
} FiM_InnerEventSatusType;

#endif /* FIM_INTERNAL_H_ */
