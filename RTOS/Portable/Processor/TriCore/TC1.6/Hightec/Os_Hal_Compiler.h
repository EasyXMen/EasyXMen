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
 **  @file               : Os_Hal_Compiler.h
 **  @author             : i-soft-os
 **  @date               : 2025/02/10
 **  @vendor             : isoft
 **  @description        : Arch source file for Compiler API implementations
 **
 ***********************************************************************************************************************/

#ifndef OS_ARCH_COMPILER_H
#define OS_ARCH_COMPILER_H
/*=======[I N C L U D E S]====================================================*/
/* PRQA S 0388 ++ */ /* VL_Os_0388 */
#include <machine/intrinsics.h>
#include <machine/wdtcon.h>
#include "Os_Hal_Types.h"
/* PRQA S 0388 -- */
/* PRQA S 3412 EOF */ /* VL_Os_3412 */
/*=======[M A C R O S]=====================================================*/
#define OS_BEGIN_NO_OPTIMIZE __attribute__((optimize("O0")))
#define OS_END_NO_OPTIMIZE
#define OS_ASM  __asm__ /* Embedded assembly. */
#define OS_USED __attribute__((used))

/* All preceding instructions are executed by the CPU. Then the pipeline is flushed
before the next instruction is executed */
#define OS_ARCH_ISYNC() OS_ASM("isync")
#define OS_ARCH_DSYNC() OS_ASM("dsync") /* All data access is served before the next data access. */

/*change calli to ji*/
/* Assembly language, define trap. */
#define DEFINE_TRAP(i) /* PRQA S 0884, 3412 */ /* VL_Os_0884 */ /* VL_Os_3412 */ \
    OS_ASM(".global __trap_" #i); /* PRQA S 0341 */             /* VL_Os_0341 */ \
    OS_ASM("__trap_" #i ":");                                                    \
    OS_ASM("svlcx");                                                             \
    OS_ASM("movh.a %a15,hi:Tdisptab+(4*" #i ")");                                \
    OS_ASM("ld.w %d4,[%a15]lo:Tdisptab+(4*" #i ")");                             \
    OS_ASM("mov.a %a15, %d4");                                                   \
    OS_ASM("movh %d4, " #i); /* D4 = Exception class */                          \
    OS_ASM("or %d4, %d15");  /* D4 |= TIN */                                     \
    OS_ASM("ji %a15");                                                           \
    OS_ASM(".align 5")

/* Assembly language, defining interrupts. */
#define DEFINE_INT(i) /* PRQA S 0884,3412 */ /* VL_Os_0884 */ /* VL_Os_3412 */ \
    OS_ASM(".global __interrupt_" #i); /* PRQA S 0341 */      /* VL_Os_0341 */ \
    OS_ASM("__interrupt_" #i ":");                                             \
    OS_ASM("svlcx");                                                           \
    OS_ASM("movh.a %a15,hi:Cdisptab+4*" #i);                                   \
    OS_ASM("lea %a15,[%a15]lo:Cdisptab+4*" #i);                                \
    OS_ASM("ld.a %a14,[%a15+]");                                               \
    OS_ASM("ld.w %d4,[%a15]");                                                 \
    OS_ASM("ji %a14");                                                         \
    OS_ASM(".align 5")

#define OS_ARCH_DECLARE_TRAP_TAB()                                                  \
    OS_ASM(                                                                         \
        ".section .traptab, \"ax\", @progbits\n" /* PRQA S 0286 */ /* VL_Os_0286 */ \
        ".align 8\n"                                                                \
        ".global TriCore_trap_table\n"                                              \
        "TriCore_trap_table:\n");

#define OS_ARCH_DECLARE_INT_TAB()                                                  \
    OS_ASM(                                                                        \
        ".section .inttab, \"ax\", @progbits\n" /* PRQA S 0286 */ /* VL_Os_0286 */ \
        ".align 13\n"                                                              \
        ".global TriCore_int_table\n"                                              \
        "TriCore_int_table:\n");

#define OS_ARCH_DEFINE_INT_0()   \
    OS_ASM(                      \
        "debug        # int 0\n" \
        ".align 5\n");

#define OS_ARCH_DECLARE_TEXT_SECTION() OS_ASM(".text")

#define OS_ARCH_EXTRU(res, a, p, w)                          \
    OS_ASM volatile(                                         \
        "mov %%d14,%2  \n\
                   mov %%d15,%3  \n\
                   extr.u %0,%1,%%e14" : "=d"(res) : "d"(a), \
        "d"(p),                                              \
        "d"(w) : "d14",                                      \
        "d15");

/* PRQA S 3410 ++ */ /* VL_Os_3410 */
#define OS_ARCH_CMPSWAP(tmp, address, exchangedVal, compareVal)                                                        \
    uint64 tmp = (uint64)exchangedVal | ((uint64)((uint64)compareVal << 32u)); /* PRQA S 3120 */ /* VL_QAC_MagicNum */ \
    OS_ASM volatile("cmpswap.w [%[addr]]0, %A[reg]" : [reg] "+d"(tmp) : [addr] "a"(address) : "memory");               \
    /* PRQA S 1006 */ /* VL_Os_1006 */                                                                                 \
    tmp = ((uint32)tmp == compareVal) ? 0ULL : 1ULL;
/* PRQA S 3410 -- */

#define OS_ARCH_STORE_SP()               \
    OS_ASM("movh.a %a10, hi:Os_ArchSp"); \
    OS_ASM("ld.a %a10, [%a10]lo:Os_ArchSp")

#define OS_ARCH_GET_SP() OS_ASM volatile("mov.aa  %0, %%a10" : "=a"(CurrentSp)::)

#define OS_ARCH_SET_BTV()      \
    vptr = TriCore_trap_table; \
    OS_ASM volatile("mtcr $btv,%0" : : "d"(vptr)); /* PRQA S 0286 */ /* VL_Os_0286 */

#define OS_ARCH_SET_BIV()     \
    vptr = TriCore_int_table; \
    OS_ASM volatile("mtcr $biv,%0" : : "d"(vptr)); /* PRQA S 0286 */ /* VL_Os_0286 */

#define OS_ARCH_GET_GLOBAL_ADDR()                                  \
    __asm__ volatile("mov.d  %0, %%a0" : "=d"(AddressRegisters0)); \
    __asm__ volatile("mov.d  %0, %%a1" : "=d"(AddressRegisters1)); \
    __asm__ volatile("mov.d  %0, %%a8" : "=d"(AddressRegisters8)); \
    __asm__ volatile("mov.d  %0, %%a9" : "=d"(AddressRegisters9))

#define OS_ARCH_RESUME_INFO()                                                 \
    __asm__ volatile("mov.aa  %%a0, %0" ::"a"(context->AddressRegisters[0])); \
    __asm__ volatile("mov.aa  %%a1, %0" ::"a"(context->AddressRegisters[1])); \
    __asm__ volatile("mov.aa  %%a8, %0" ::"a"(context->AddressRegisters[8])); \
    __asm__ volatile("mov.aa  %%a9, %0" ::"a"(context->AddressRegisters[9]))

#define OS_ARCH_GET_TRAP_SP() OS_ASM volatile("mov.aa  %0, %%a10" : "=a"(Os_ArchTrapSp)::)

#define OS_ARCH_STORE_TRAP_SP()              \
    OS_ASM("movh.a %a10, hi:Os_ArchTrapSp"); \
    OS_ASM("ld.a %a10, [%a10]lo:Os_ArchTrapSp")

#define OS_ALWAYS_INLINE __attribute__((always_inline))

#define Os_Hal_DisableInt() (void)_disable() /* Disables global interrupt. */
#define Os_Hal_EnableInt()  (void)_enable()  /* Enables global interrupt. */

#define OS_SYSCALL(num) (void)_syscall(num)

#define OS_ARCH_MFCR(addr) \
    (Os_ArchMsrType)       \
        _mfcr((addr)) /* Move from core register. Move the contents of the core SFR register to adata register. */
/* Move to core register. Move the value from a data register to the coreSFR */
#define OS_ARCH_MTCR(addr, reg) /* PRQA S 3458 */ /* VL_Os_3458 */ \
    {                                                              \
        (void)_mtcr((addr), (reg));                                \
        OS_ARCH_ISYNC();                                           \
    }

/* PRQA S 3684, 0632, 3218, 3678, 1752 ++ */ /* VL_Os_3684, VL_Os_0632, VL_Os_3218, VL_Os_3678, VL_Os_1752 */
extern uint32 TriCore_int_table[];
/* PRQA S 3684, 0632, 3218, 3678, 1752 -- */

/* PRQA S 3684, 1752 ++ */ /* VL_Os_3684, VL_Os_1752 */
extern uint32 TriCore_trap_table[];
/* PRQA S 3684, 1752 -- */

#endif
