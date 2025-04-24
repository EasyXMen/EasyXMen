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
**                                                                            **
**  FILENAME    : Arch_Context.h                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/

#ifdef COMPILER_GCC
#include "Os_Compiler_gcc.h"
#elif defined(COMPLIER_GHS)
#include "Os_Compiler_ghs.h"
#endif

#include "Os_Cfg_S.h"

    OS_SYNTAX_UNIFIED
    .text
    OS_ALIGN 2
    OS_THUMB

/*=======[E X T E R N A L   D A T A]==========================================*/
    OS_EXTERN Os_SCB
    OS_EXTERN Os_LocalVector
    OS_EXTERN Os_Isr2_Ipl_Limit

/*=======[M A C R O S]========================================================*/



/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
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
.thumb_func
.global PendSV_Handler
PendSV_Handler:
	mrs r3, basepri 
    push {r3}
    ldr r3, =Os_Isr2_Ipl_Limit
    ldr r3, [r3]
    msr basepri, r3                            
    ldr r2, =Os_SCB     
    ldr r2, [r2]              
    cbz r2, lable   

    mrs r0, psp
    isb

    /* Is the task using the FPU context?  If so, push high vfp registers. */
    tst r14, #0x10
    it eq
    vstmdbeq r0!, {s16-s31}
    
    /* Save the CONTROL and other core registers. */
    mrs r3,control
    stmdb r0!, {r3-r11, r14}
    
    str r0, [r2]
    
lable:                               
    dsb                               
    isb                                

    push {lr}
    bl Os_SwitchTask
    pop {lr}

    ldr r2, =Os_SCB     
    ldr r2, [r2]
    ldr r0, [r2]
    
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
    pop {r3}
    msr basepri, r3
    bx lr

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

.global TERMINATEISR_ISR
TERMINATEISR_ISR:
    push {r0}
    ldr r0, =Os_IsrTempIPSR
    ldr r0, [r0]
    ORR  R0, R0, #0x01000000U
    push {r0}  /*xpsr*/   
	ldr r0, =TERMINATEISR_ISR_1
	push {r0}
	push {R14}
	push {R12}
	push {R3}
	push {R2}
	push {R1}
	push {R0}
	ldr lr, =0xfffffff1
	bx lr

TERMINATEISR_ISR_1:
    pop {r0}
    bx lr

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

.global armv7_default_isr
armv7_default_isr:
Os_DisableInt:
    cpsid i
    mrs     r12, ipsr
    lsl     r12, r12, #2
#ifndef OS_ASM_CFG_SC_1
/* When the time protection terminates interrupt,the previous nested interrupt may not be able to exit */
OS_WHETHER_INTERRUPT_NEST:
    mrs     r2, msp
    ldr     r1, [r2]
    mov     r2, 0x1FFu
    and     r2, r1, r2
    cmp     r2, #0
    bne     OS_WHETHER_INTERRUPT_PREEMPT
    b       OS_SAVE_CONTEXT      
OS_WHETHER_INTERRUPT_PREEMPT:
    cmp     r0, r1
    bne     OS_SAVE_CONTEXT
    ldr     r3, =0xFFFFFE00
    and     r2, r0, r3
    ldr     r3, =0xABCDEE00
    cmp     r2, r3
    bne     OS_SAVE_CONTEXT
OS_SIMULATE_EXIT:
    ldr     r1, =Os_IsrTempIPSR
    mrs     r2, msp
    ldr     r3, [r2,#28]
    str     r3, [r1]
    ldr     r3, [r2,#20]
    push    {r3,r14}
    bl      TERMINATEISR_ISR
    pop     {r3,r14}
    msr     msp, r2
    tst     r14, #0x10
    bne     NO_FLOAT
    add     r1, r2,#96
    b       END_SIMULATE
NO_FLOAT:
    add     r1, r2,#32
END_SIMULATE:
    mov     r14, r3
    msr     msp, r1
#endif
/* save context */
OS_SAVE_CONTEXT:
    mrs     r0, msp
    isb
    tst     r14, #0x10                  
    it eq                  
    vstmdbeq r0!, {s16-s31} 
    mrs     r3,control
    stmdb   r0!, {r3-r11, r14}
    msr     msp, r0
    dsb
    isb
/* Save msp in "Os_ArchMasterSp_ARRAY" */
OS_SAVE_MSP:
    ldr     r0, =Os_IntNestISR2
    ldrb    r0, [r0]
    ldr     r3, =Os_IntNestISR1
    ldrb    r3, [r3]
    add     r0, r0, r3
    ldr     r1, =Os_ArchMasterSp_ARRAY
    lsl     r0, r0, #2
    add     r1, r1, r0
    mrs     r2, msp
    str     r2, [r1]
/* Jump to the Handler for interruption */
OS_JUMP_TO_HANDLER:
    ldr     r1, =Os_LocalVector
    add     r1, r1, r12
    ldr     r12, [r1]
    bx      r12
OS_ALIGN 4
.ltorg


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
.thumb_func
.global Os_CmpSwapW
Os_CmpSwapW:
    push    {r4,lr}
    ldrex   r4, [r0]
    cmp     r4,r1
    bne     ClearExclusive
    STREX   r4, r2, [r0]
    mov     r0, r4
    b       CmpExit
ClearExclusive:
    clrex
CmpExit:
    pop {r4,pc}

OS_ALIGN 4
.ltorg
/*=======[E N D   O F   F I L E]==============================================*/
