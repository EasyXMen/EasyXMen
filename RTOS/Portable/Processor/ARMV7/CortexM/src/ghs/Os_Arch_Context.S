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
 *
 ********************************************************************************
 **                                                                            **
 **  FILENAME    : Os_Arch_Context.S                                           **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      : i-soft-os                                                   **
 **  Vendor      :                                                             **
 **  DESCRIPTION :                                                             **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform r23                         **
 **  Version :   AUTOSAR classic Platform R23--Function Safety                 **
 **                                                                            **
 *******************************************************************************/
/* PRQA S 3108-- */

#include "Os_Hal_Compiler.h"

#include "Os_Cfg_S.h"

/*=======[E X T E R N A L   D A T A]==========================================*/

    OS_EXTERN Os_ArchExternIrqTable
    OS_EXTERN Os_ArchIsrTempIPSR
    OS_EXTERN Os_SwitchTask
    OS_EXTERN Os_Arch_SaveMainSpAndXpsr
    OS_EXTERN Os_Arch_GetTerminatedIntMainSP
    OS_EXTERN Os_Arch_GetRunningTaskStack
    OS_EXTERN Os_Arch_SetRunningTaskStack
    OS_EXTERN Os_Arch_IsFirstDispatch
    OS_EXTERN Os_Arch_HardFault_Process

    OS_SYNTAX_UNIFIED
    .text
    .align 2
    OS_THUMB

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
/*****************************************************************************/
/*
 * Brief                <Os_Arch_HardFault_Handler>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <System services>
 * REQ ID               <None>
 */
/*****************************************************************************/
THUMB_FUNC
.global Os_Arch_HardFault_Handler
Os_Arch_HardFault_Handler:
    /* Check if msp or psp */
    tst     lr, #0x04
    ite     eq
    mrseq   r0, msp
    mrsne   r0, psp
    /* Check if fpu register need save */
    tst     lr, #0x8
    ite     eq
    /* store flag at sp bit[0] */
    orreq   r0, #1
    orrne   r0, #0
    push    {lr}
    bl      Os_Arch_HardFault_Process
    pop     {lr}
    bx      lr
    .endf Os_Arch_HardFault_Handler

/*****************************************************************************/
/*
 * Brief                <Switch tasks>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <System services>
 * REQ ID               <None>
 */
/*****************************************************************************/
THUMB_FUNC
.global Os_Arch_PendSV_Handler
Os_Arch_PendSV_Handler:
    cpsid i
OS_IS_FIRST_DISPATCH:
    push {lr}
    bl Os_Arch_IsFirstDispatch
    pop {lr}
    cmp r0, #0
    bne     OS_ASM_NEW_TASK
    mrs r0, psp
    isb

    /* Is the task using the FPU context?  If so, push high vfp registers. */
    tst r14, #0x10
    it eq
    vstmdbeq r0!, {s16-s31}

    /* Save the CONTROL and other core registers. */
    mrs r3,control
    stmdb r0!, {r3-r11, r14}

    push {lr}
    bl Os_Arch_SetRunningTaskStack
    pop {lr}

OS_ASM_NEW_TASK:
    dsb
    isb

    push {lr}
    bl Os_SwitchTask
    pop {lr}

    push {lr}
    bl Os_Arch_GetRunningTaskStack
    pop {lr}

    ldmia r0!, {r3-r11, r14}
    msr control,r3    /*CONTROL register*/

    /* Is the task using the FPU context?  If so, pop the high vfp registers
    too.*/
    tst r14, #0x10
    it eq
    vldmiaeq r0!, {s16-s31}

    msr psp, r0
    dsb
    isb
    cpsie i
    bx lr
    .endf Os_Arch_PendSV_Handler

/*****************************************************************************/
/*
 * Brief                <Force exit for nested interrupt>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <OS_ARCH_TIME_PROT_EXIT (Os_Arch_Irq.h)>
 * REQ ID               <None>
 */
/*****************************************************************************/
THUMB_FUNC
.global Os_Arch_ForceInterruptExit
Os_Arch_ForceInterruptExit:
    push    {r0-r9, r14}
    bl      Os_Arch_GetTerminatedIntMainSP
    /* Load the EXC-RETURN value saved by the current interrupt */
    ldr     r14, [r0, #36]
    /* Load constants */
    ldr     r8, =0xFFFFFE00
    ldr     r9, =0xABCDEE00
    tst     r14, #0x10
    beq     float_context_1
    ldr     r14, [r0, #0x3C]
    add     r6,  r0,#0x28
    add     r0,  r0,#0x48
    b       check_loop
float_context_1:
    ldr     r14, [r0,#0x7C]
    add     r6,  r0,#0x68
    add     r0,  r0,#0xD0
check_loop:
    /* whether any other interrupt stack frames on the current stack */
    ldr     r7, [r6]
    and     r4, r7, r8
    cmp     r4, r9
    bne     TEMP_EXIT
    /* Simulate interrupt exit */
    ldr     r4, =Os_ArchIsrTempIPSR
    mov     r6, #0x1FFu
    and     r6, r6, r7
    str     r6, [r4]
    push    {r14}
    bl      Os_Arch_TerminateISR
    pop     {r14}
    tst     r14, #0x10
    beq     float_context_2
    ldr     r14, [r0,#0x14]
    mov     r6,  r0
    add     r0,  r0,#0x20
    b check_loop
float_context_2:
    ldr     r14, [r0,#0x14]
    mov     r6,  r0
    add     r0,  r0,#0x68
    b check_loop
TEMP_EXIT:
    pop    {r0-r9, r14}
    bx lr
    .endf Os_Arch_ForceInterruptExit

/*****************************************************************************/
/*
 * Brief                <terminate isr>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <System services>
 * REQ ID               <None>
 */
/*****************************************************************************/
THUMB_FUNC
.global Os_Arch_TerminateISR
Os_Arch_TerminateISR:
    push {r0}
    ldr r0, =Os_ArchIsrTempIPSR
    ldr r0, [r0]
    orr  R0, R0, #0x01000000U
    push {r0}  /*xpsr*/
    ldr r0, =OS_ASM_TERMINATEISR_ISR
    push {r0}
    push {R14}
    push {R12}
    push {R3}
    push {R2}
    push {R1}
    push {R0}
    ldr lr, =0xfffffff1
    bx lr

OS_ASM_TERMINATEISR_ISR:
    pop {r0}
    bx lr
    .endf Os_Arch_TerminateISR

/*****************************************************************************/
/*
 * Brief                <Unified entrance for interruption>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <System services>
 * REQ ID               <None>
 */
/*****************************************************************************/
THUMB_FUNC
.global Os_Arch_DefaultIntEntry
Os_Arch_DefaultIntEntry:
    cpsid i
/* save context */
OS_SAVE_CONTEXT:
    mrs     r0, msp
    isb
    tst     r14, #0x10
    it      eq
    vstmdbeq r0!, {s16-s31}
    mrs     r3, control
    stmdb   r0!, {r3-r11, r14}
    msr     msp, r0
    dsb
    isb
OS_ASM_SAVE_MSP_AND_XPSR:
    mrs     r0, msp
    mrs     r1, xpsr
    push    {lr}
    bl      Os_Arch_SaveMainSpAndXpsr
    pop     {lr}
/* Jump to the Handler for interruption */
OS_ASM_JUMP_TO_HANDLER:
    mrs     r12, ipsr
    lsl     r12, r12, #2
    ldr     r1, =Os_ArchExternIrqTable
    add     r1, r1, r12
    ldr     r12, [r1]
    bx      r12
.align 4
.ltorg
    .endf Os_Arch_DefaultIntEntry

/*****************************************************************************/
/*
 * Brief                <Unified Exit for interrupt>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <r0:Main sp>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <ISR>
 * REQ ID               <None>
 */
/*****************************************************************************/
THUMB_FUNC
.global Os_Arch_DefaultIntExit
Os_Arch_DefaultIntExit:
    ldmia   r0!, {r3-r11, r14}
    msr     control, r3
    tst     r14, #0x10
    it      eq
    vldmiaeq r0!, {s16-s31}
    msr     msp, r0
    mrs     r0, ipsr
    ldr     r1, =0xABCDEE00
    orr     r0, r0, r1
    dsb
    isb
    cpsie i
    bx lr
.align 4
.ltorg
    .endf Os_Arch_DefaultIntExit

/*****************************************************************************/
/*
 * Brief                <Call by Spin-lock API>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <System services>
 * REQ ID               <None>
 */
/*****************************************************************************/
THUMB_FUNC
OS_WEAK_GLOBAL_ASM Os_Hal_CmpSwapW
.global Os_Hal_CmpSwapW
Os_Hal_CmpSwapW:
    push    {r4, lr}
    ldrex   r4, [r0]
    cmp     r4, r1
    bne     OS_ASM_CLEAR_EXCLUSIVE
    strex   r4, r2, [r0]
    mov     r0, r4
    b       OS_ASM_CMP_EXIT
OS_ASM_CLEAR_EXCLUSIVE:
    clrex
OS_ASM_CMP_EXIT:
    pop {r4, pc}

.align 4
.ltorg
    .endf Os_Hal_CmpSwapW
/*=======[E N D   O F   F I L E]==============================================*/
