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

/*=======[M A C R O S]========================================================*/
/*For ASM*/
#define OS_ASM_EXTERN                 .extern
#define OS_ASM_RESET                  _RESET
#define OS_ASM_EQU                    .equ
#define OS_ASM_MACRO(MacroName)       .macro MacroName
#define OS_ASM_MACRO_END              .endm
#define OS_ASM_ALIGN(AlignBytes)      .align(1U << (AlignBytes))
#define OS_ASM_GLOBAL                 .global
#define OS_ASM_TEXT_ALIGN(AlignBytes) .align(1U << (AlignBytes))
/*Greenhills is bit-aligned, so bytes need to be converted to bits.*/

/*For C */
#if !defined(OS_ARCH_PORTASM_S) /* to avoid asm include */
/*=======[I N C L U D E S]====================================================*/
#include <v800_ghs.h>
#include "Platform_Types.h"
/*=======[M A C R O S]========================================================*/
#ifndef OS_ALWAYS_INLINE
#define OS_ALWAYS_INLINE
#endif

#define ASM __asm volatile

#define OS_BEGIN_NO_OPTIMIZE __attribute__((ghs_optimize(Onone)))
#define OS_END_NO_OPTIMIZE
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
/* Base Register Read and Write */
/* PRQA S 3206,3008,5209++ */ /* VL_Os_3206, VL_Os_3008, VL_Os_5209*/
static inline void OS_ARCH_REG_WRITE(unsigned long regId, unsigned long selId, unsigned long val)
{
    ASM("LDSR %2, %0 , %1" ::"%"(regId), "%"(selId), "r"(val) : "memory"); /* PRQA S 1006*/ /* VL_Os_1006*/
}

static inline unsigned long OS_ARCH_REG_READ(unsigned long regId, unsigned long selId)
{
    unsigned long val;
    ASM("STSR %1, %0, %2" : "=r"(val) : "%"(regId), "%"(selId) : "memory"); /* PRQA S 1006*/ /* VL_Os_1006*/
    return val;
}
/* PRQA S 3206,3008,5209-- */

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
#if !defined(RH850G4MH_RH850G4KH)
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
static inline uint32 Os_Hal_CmpSwapW(uint32* address, uint32 compareVal, uint32 exchangedVal)
/* PRQA S 1532 -- */
{
    uint32 ret;
    /* Note: The CAXI instruction unconditionally overwrites the third operand,
       but it does not matter if the third operand is overwritten here. */
    /* PRQA S 0317, 0314 ++ */ /* VL_QAC_0317 */ /* VL_Os_0314 */
    ret = (uint32)__CAXI2((void*)address, (sint32)compareVal, (sint32)exchangedVal);
    /* PRQA S 0317, 0314 -- */ 
    return (ret == 0UL) ? 1UL : 0UL;
}
#endif /*RH850U2A*/

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
#if defined(RH850G4MH_RH850G4KH)
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
static inline uint32 Os_Hal_CmpSwapW(uint32* address, uint32 compareVal, uint32 exchangedVal)
/* PRQA S 1532 -- */
{
    uint32 oldVal = 0U;
    uint32 result = 0U;

    /* Load Link - Read current value and establish monitoring */
    ASM("ldl.w [%1], %0" : "=r"(oldVal) : "r"(address));

    if (oldVal != compareVal)
    {
        result = 0;
    }
    else
    {
        /* If the current value equals the expected value, attempt conditional storage. */
        ASM("stc.w %2, [%1] \n\t"
            "mov %2, %0 \n\t"
            "snooze \n\t" : "=r"(result) : "r"(address),
            "r"(exchangedVal) : "memory");
    }
    return result == 1U ? 0U : 1U;
}
#endif /* defined(RH850G4MH_RH850G4KH) */

#endif /*C*/

#endif
