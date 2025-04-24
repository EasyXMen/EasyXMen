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
 ********************************************************************************
 ************************************************************************************************************************
 ** **
 **  @file               : ComM_Internal.h **
 **  @version            : V1.0.0 **
 **  @author             : darren.zhang **
 **  @date               : 2022/01/07 **
 **  @vendor             : isoft **
 **  @description        : Communication Manager **
 **  @specification(s)   : AUTOSAR classic Platform R19-11 **
 ** **
 ***********************************************************************************************************************/
#ifndef COMM_INTERNAL_H_
#define COMM_INTERNAL_H_

/*=================================================[inclusions]=======================================================*/
#include "ComM_Cfg.h"
#include "ComM_PBCfg.h"
#include "Std_Types.h"
#if defined(COMM_FUNC_TRACE_SUPPORT) || defined(COMM_FUNC_CHECK)
#include "func_trace.h"
#endif
#include "ComStack_Types.h"
#if (COMM_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif
#include "ComM_Types.h"

#if COMM_MULTIPLE_PARTITION_ENABLED == STD_ON
#include "Os.h"
#endif

/*====================================================[macros]========================================================*/

#if defined(COMM_FUNC_TRACE_SUPPORT)
#define COMM_FUNC_TRACE_ENTRY(...) FuncTrace_Entry(__VA_ARGS__)
#define COMM_FUNC_TRACE_EXIT(...)  FuncTrace_Exit(__VA_ARGS__)
#define COMM_FUNC_TRACE_MARK(...)  FuncTrace_Mark(__VA_ARGS__)
#else
#define COMM_FUNC_TRACE_ENTRY(...)
#define COMM_FUNC_TRACE_EXIT(...)
#define COMM_FUNC_TRACE_MARK(...)
#endif /* defined(COMM_FUNC_TRACE_SUPPORT) */

#if !defined(COMM_FUNC_CHECK)
#define COMM_FUNC_CHECK 0
#endif

#if COMM_FUNC_CHECK
#define COMM_CHECK_OUTPUT(...) FuncCheckOutput(__VA_ARGS__)
#else
#define COMM_CHECK_OUTPUT(...)
#endif /* COMM_FUNC_CHECK */

#if !defined(COMM_LOCAL)
#define COMM_LOCAL static
#endif

#if !defined(COMM_LOCAL_INLINE)
#define COMM_LOCAL_INLINE LOCAL_INLINE
#endif

/* PRQA S 3472 ++ */ /* MISRA Dir 4.9 */
#define COMM_VLU_MAPU8_NUM(idx)   (((uint8)(idx) + 7u) / 8u)
#define COMM_VLU_MAPU8_BYTE(idx)  ((uint8)((uint8)(idx) >> 3u))
#define COMM_VLU_MAPU8_INDEX(idx) ((uint8)(idx) & (uint8)0x07u)
#define COMM_VLU_MAPU8_MASK(idx)  ((uint8)((uint8)1u << (COMM_VLU_MAPU8_INDEX(idx))))
/* PRQA S 3472 -- */ /* MISRA Dir 4.9 */

#if !defined(COMM_PARA_UNUSED)
#define COMM_PARA_UNUSED(x) (void)(x)
#endif

/*=============================================[type definitions]=====================================================*/

typedef uint32 uintx;

typedef struct
{
    uint8 ecuGroupClass;
    uint16 inhibitCnt;
} ComM_StoryDataType;

extern ComM_InitStatusType ComM_InitStatus;
extern const ComM_ConfigType* ComM_ConfigPtr;

#if COMM_MULTIPLE_PARTITION_ENABLED == STD_ON
#if COMM_USER_NUMBER > 0u
extern const ApplicationType ComM_UserApplicationID[COMM_USER_NUMBER];
#endif

extern const ApplicationType ComM_ChannelApplicationID[COMM_CHANNEL_NUMBER];
#endif
#if (COMM_USER_NUMBER > 0u)
extern ComM_ModeType ComM_UserReqMode[COMM_USER_NUMBER];
#endif

extern Std_ReturnType ComM_HandleUserReqest(ComM_UserHandleType User, ComM_ModeType ComMode);

static inline void ComM_CommonSetBit(uint8* data, uint32 bitPn)
{
    data[bitPn / 8u] |= 1u << (bitPn % 8u);
}

static inline void ComM_CommonClrBit(uint8* data, uint32 bitPn)
{
    data[bitPn / 8u] &= ~(1u << (bitPn % 8u));
}

#ifdef QAC_ANALYZE
#pragma PRQA_NO_SIDE_EFFECTS ComM_CommonGetBit
#endif

static inline boolean ComM_CommonGetBit(const uint8* data, uint32 bitPn)
{
    return (data[bitPn / 8u] & (1u << (bitPn % 8u))) != 0u;
}
#endif /* COMM_INTERNAL_H_ */
