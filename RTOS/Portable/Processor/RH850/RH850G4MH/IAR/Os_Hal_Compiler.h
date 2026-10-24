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
 ********************************************************************************
 **                                                                            **
 **  FILENAME    : Os_Hal_Compiler.h                                           **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      :  i-soft-os                                                  **
 **  Vendor      :                                                             **
 **  DESCRIPTION :                                                             **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
 **  Version :   AUTOSAR classic Platform R19--Function Safety                 **
 **                                                                            **
 *******************************************************************************/
#ifndef OS_HAL_COMPILER_H
#define OS_HAL_COMPILER_H

/*For ASM*/
/*=======[M A C R O S]========================================================*/
#define OS_ASM_EXTERN                 extern
#define OS_ASM_RESET                  __iar_program_start4_g4
#define OS_ASM_EQU                    equ
#define OS_ASM_MACRO(MacroName)       MacroName macro
#define OS_ASM_MACRO_END              endm
#define OS_ASM_ALIGN(AlignBytes)      align AlignBytes
#define OS_ASM_GLOBAL                 public
#define OS_ASM_TEXT_ALIGN(AlignBytes) section.text : CODE(AlignBytes)

/*For C */
#if !defined(OS_ARCH_PORTASM_S) /* to avoid asm include */
/*=======[I N C L U D E S]====================================================*/
#include "intrinsics.h"
#include "Platform_Types.h"
/*=======[M A C R O S]========================================================*/
#ifndef OS_ALWAYS_INLINE
#define OS_ALWAYS_INLINE
#endif
#define ASM                        __asm volatile
#define OS_ARCH_REG_WRITE(Id, val) __LDSR(Id, val)
#define OS_ARCH_REG_READ(Id)       __STSR(Id)

#define OS_BEGIN_NO_OPTIMIZE _Pragma("optimize=none")
#define OS_END_NO_OPTIMIZE

#if !defined(RH850U2B) /*RH850U2A*/
/******************************************************************************/
/*
 * Brief                  <Atomic compare and swap for word>
 *
 * Service ID             <None>
 * Sync/Async             <Synchronous>
 * Reentrancy             <Reentrant>
 * param-address[in]      <Pointer to the address>
 * param-compareVal[in]   <The value to compare with the value at the address>
 * param-exchangedVal[in] <The value to replace the value at the address>
 * param-Name[out]        <None>
 * param-Name[in/out]     <None>
 * return                 <None>
 * CallByAPI              <GetSpinlock and so on>
 * REQ ID                 <None>
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
static inline uint32 Os_Hal_CmpSwapW(uint32* address, uint32 compareVal, uint32 exchangedVal)
/* PRQA S 1532 -- */
{
    uint32 ret;
    /* Note: The CAXI instruction unconditionally overwrites the third operand,
       but it does not matter if the third operand is overwritten here. */
    ASM("caxi [%0], %3, %1\n" /* Atomic compare-and-exchange, sets PSW.Z flag (PSW.Z = 1 indicates success) */
        "setfz %2\n"          /* Set ret according to PSW.Z */
        : "+r"(address),      /* %0: address (input, read by CAXI, not modified) */
        "+r"(exchangedVal),   /* %1: exchange value (input, overwritten by CAXI with old value, but not used) */
        "=r"(ret)             /* %2: return value */
        : "r"(compareVal)     /* %3: compare value */
        : "memory",
        "cc" /* memory barrier, condition codes modified */
    );
    return (ret == 0UL) ? 1UL : 0UL;
}
#endif /*RH850U2A*/
#endif /*C*/

#endif
