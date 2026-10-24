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
/*******************************************************************************
**                                                                            **
**  FILENAME    :  Os_Hal_Compiler.h                                         **
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
/* PRQA S 0286, 3412 ++ */ /* VL_Os_0286, VL_Os_3412 */
#ifndef OS_COMPILER_TASKING_H
#define OS_COMPILER_TASKING_H
/*=======[I N C L U D E S]====================================================*/
#include "Ifx_Ssw_Infra.h"
/*=======[M A C R O S]=====================================================*/
#define OS_BEGIN_NO_OPTIMIZE _Pragma("optimize 0")
#define OS_END_NO_OPTIMIZE   _Pragma("endoptimize")
#define OS_ASM  __asm /* PRQA S 3431 */ /* VL_Os_3431 */
#define OS_USED __attribute__((used))
#ifndef OS_LOCAL
#define OS_LOCAL static /* PRQA S 3414 */ /* VL_QAC_FctLikeMacro */
#endif
#ifndef OS_LOCAL_INLINE
#define OS_LOCAL_INLINE static inline
#endif

/* All preceding instructions are executed by the CPU. Then the pipeline is flushed
before the next instruction is executed */
#define OS_ARCH_ISYNC() OS_ASM("isync")
#define OS_ARCH_DSYNC() OS_ASM("dsync") /* All data access is served before the next data access. */

/* Move from core register. Move the contents of the core SFR register to adata register. */
#define OS_ARCH_MFCR(addr) (Os_ArchMsrType) __mfcr((addr))
/* Move to core register. Move the value from a data register to the coreSFR */
#define OS_ARCH_MTCR(addr, reg) /* PRQA S 3458 */ /* VL_Os_3458 */ \
    {                                                              \
        (void)__mtcr((addr), (reg));                               \
        OS_ARCH_ISYNC();                                           \
    }

/* PRQA S 0602, 0603 ++ */ /* VL_Os_0602, VL_Os_0603 */
#define __ALIGN_TRAP_TAB__ OS_ASM(" .align 32")
#define __ALIGN_INTI_TAB__ OS_ASM(" .align 32")
/* PRQA S 0602, 0603 -- */
/* PRQA S 0341, 0884 ++ */               /* VL_Os_0341, VL_Os_0884 */
#define DEFINE_TRAP(i) /* PRQA S 3412 */ /* VL_Os_3412 */ \
    __ALIGN_TRAP_TAB__;                                   \
    OS_ASM("svlcx");                                      \
    OS_ASM("movh.a  a15, #@his(Tdisptab+4*" #i ")");      \
    OS_ASM("lea a15, [a15]@los(Tdisptab+4*" #i ")");      \
    OS_ASM("ld.a   a14, [a15]");                          \
    OS_ASM("mov    d4, d15");                             \
    OS_ASM("ji     a14")

/* Assembly language, defining interrupts. */
#define DEFINE_INT(i)                                \
    __ALIGN_INTI_TAB__;                              \
    OS_ASM("svlcx");                                 \
    OS_ASM("movh.a  a15, #@his(Cdisptab+4*" #i ")"); \
    OS_ASM("lea a15, [a15]@los(Cdisptab+4*" #i ")"); \
    OS_ASM("ld.a   a14, [a15]");                     \
    OS_ASM("mov    d5, #" #i);                       \
    OS_ASM("ji     a14")
/* PRQA S 0341, 0884 -- */
/* PRQA S 3411 ++ */ /* VL_Os_3411 */
#define OS_ARCH_DECLARE_TRAP_TAB()                 \
    OS_ALIGN(8 * 32) void TriCore_trap_table(void) \
    {
#define OS_ARCH_DECLARE_INT_TAB()                   \
    OS_ALIGN(256 * 32) void TriCore_int_table(void) \
    {
/* PRQA S 3411 -- */
#define OS_ARCH_DEFINE_INT_0() DEFINE_INT(0);

#define OS_ARCH_DECLARE_TEXT_SECTION() } /* PRQA S 3431 */ /* VL_Os_3431 */

#define OS_ARCH_EXTRU(res, a, p, w)             \
    OS_ASM(                                     \
        "mov d14,%2  \n\
        mov d15,%3  \n\
        extr.u %0,%1,e14" : "=d"(res) : "d"(a), \
        "d"(p),                                 \
        "d"(w) : "d14",                         \
        "d15")

#define OS_ARCH_CMPSWAP(tmp, address, exchangedVal, compareVal) \
    uint32 tmp = __cmpswapw(                                    \
        (unsigned int*)address,                                 \
        exchangedVal,                                           \
        compareVal) /* PRQA S 3409, 3410, 5209 */ /* VL_Os_3409, VL_Os_3410, VL_Os_5209 */

#define OS_ARCH_STORE_SP()                  \
    OS_ASM("movh.a a10, #@HIS(Os_ArchSp)"); \
    OS_ASM("ld.a a10, [a10]@LOS(Os_ArchSp)")

#define OS_ARCH_GET_SP() OS_ASM("mov.aa  %0, a10" : "=a"(CurrentSp)::)

#define OS_ARCH_SET_BTV()               \
    vptr = (uint32*)TriCore_trap_table; \
    __mtcr(OS_REG_BTV, (unsigned int)vptr) /* PRQA S 5209 */ /* VL_Os_5209 */

#define OS_ARCH_SET_BIV()              \
    vptr = (uint32*)TriCore_int_table; \
    __mtcr(OS_REG_BIV, (unsigned int)vptr) /* PRQA S 5209 */ /* VL_Os_5209 */

#define OS_ARCH_GET_GLOBAL_ADDR()                      \
    OS_ASM("mov.d  %0, a0" : "=d"(AddressRegisters0)); \
    OS_ASM("mov.d  %0, a1" : "=d"(AddressRegisters1)); \
    OS_ASM("mov.d  %0, a8" : "=d"(AddressRegisters8)); \
    OS_ASM("mov.d  %0, a9" : "=d"(AddressRegisters9))

#define OS_ARCH_RESUME_INFO()                                     \
    OS_ASM("mov.aa  a0, %0" ::"a"(context->AddressRegisters[0])); \
    OS_ASM("mov.aa  a1, %0" ::"a"(context->AddressRegisters[1])); \
    OS_ASM("mov.aa  a8, %0" ::"a"(context->AddressRegisters[8])); \
    OS_ASM("mov.aa  a9, %0" ::"a"(context->AddressRegisters[9]))

#define OS_ARCH_GET_TRAP_SP() OS_ASM("mov.aa  %0, a10" : "=a"(Os_ArchTrapSp)::)

#define OS_ARCH_STORE_TRAP_SP()                 \
    OS_ASM("movh.a a10, #@HIS(Os_ArchTrapSp)"); \
    OS_ASM("ld.a a10, [a10]@LOS(Os_ArchTrapSp)")

#ifndef lock_wdtcon
#define lock_wdtcon() /* PRQA S 3472 */ /* VL_Os_3472 */ \
    Ifx_Ssw_setCpuEndinit(                               \
        &MODULE_SCU.WDTCPU[OS_ARCH_MFCR(CPU_CUS_ID)],    \
        Ifx_Ssw_getCpuWatchdogPassword(&MODULE_SCU.WDTCPU[OS_ARCH_MFCR(CPU_CUS_ID)]))
#endif

#ifndef unlock_wdtcon
#define unlock_wdtcon() /* PRQA S 3472 */ /* VL_Os_3472 */ \
    Ifx_Ssw_clearCpuEndinit(                               \
        &MODULE_SCU.WDTCPU[OS_ARCH_MFCR(CPU_CUS_ID)],      \
        Ifx_Ssw_getCpuWatchdogPassword(&MODULE_SCU.WDTCPU[OS_ARCH_MFCR(CPU_CUS_ID)]))
#endif

#ifndef lock_safety_wdtcon
#define lock_safety_wdtcon() Ifx_Ssw_setSafetyEndinit(Ifx_Ssw_getSafetyWatchdogPassword())
#endif

#ifndef unlock_safety_wdtcon
#define unlock_safety_wdtcon() Ifx_Ssw_clearSafetyEndinit(Ifx_Ssw_getSafetyWatchdogPassword())
#endif

#define OS_ALWAYS_INLINE

#define Os_Hal_DisableInt() (void)__disable() /* Disables global interrupt. */
#define Os_Hal_EnableInt()  (void)__enable()  /* Enables global interrupt. */

#define OS_SYSCALL(num) (void)__syscall(num)

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
extern void TriCore_trap_table(void);
extern void TriCore_int_table(void);
#endif
/* PRQA S 0286, 3412 -- */
