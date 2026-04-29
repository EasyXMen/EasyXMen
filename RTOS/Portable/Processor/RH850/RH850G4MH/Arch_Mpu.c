/* PRQA S 3108++ */
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
/* PRQA S 3108-- */
/*
 ********************************************************************************
 **                                                                            **
 **  FILENAME    : Arch_Mpu.c                                                  **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      : i-soft-os                                                   **
 **  Vendor      :                                                             **
 **  DESCRIPTION : functions related to the Memory Protection Unit (MPU)       **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
 **  Version :   AUTOSAR classic Platform R19--Function Safety                 **
 **                                                                            **
 *******************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Os_Internal.h"
/*=======[E X T E R N A L   D A T A]==========================================*/
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
/*The address of the interrupt data.*/
P2CONST(OS_ISR_ADDR, AUTOMATIC, OS_VAR) Os_IsrDAddr;
/*The address of the task data.*/
P2CONST(OS_TASK_ADDR, AUTOMATIC, OS_VAR) Os_TaskDAddr;

#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
/*=======[I N T E R N A L   D A T A]==========================================*/
#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
static VAR(Os_ApplicationType, OS_VAR) Os_MpuCurrentAppId;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Initialize the memory protection mapping for the os>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <xxx>
 * Reentrancy           <Non Reentrant>
 * param[in]            <xxx>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <xxx>
 * REQ ID               <xxx>
 */
/******************************************************************************/
/******************************************************************************/
/*
 * Brief                <Initialize the memory protection mapping for the os>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <xxx>
 * Reentrancy           <Non Reentrant>
 * param[in]            <xxx>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <xxx>
 * REQ ID               <xxx>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ArchInitKnMemMap(void)
{
    /* All Flash area, RX */
    OS_MPU_ENABLE_REGION(OS_MPU_REGION_0, OS_MPU_USER_E, 
                        (uint32)OS_ARCH_FLASH_ADDR_START, 
                        (uint32)OS_ARCH_FLASH_ADDR_END);

    /* All Peripheral area, RW */    
    OS_MPU_ENABLE_REGION(OS_MPU_REGION_1, OS_MPU_USER_RW, 
                        (uint32)OS_ARCH_PERIPH_ADDR_START, 
                        (uint32)OS_ARCH_PERIPH_ADDR_END);

    /* Run time modification  */
    if ((uint32)Os_Core_App_DAddr[0].APP_ADDR_START < (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_START)
    {
        /* the address of CoreN is greater than the address of Core0
         * e.g. CoreN application data is mapped to cluster ram
         */

        /* RAM area out of Os-Application, RW */
        if ((uint32)OS_ARCH_RAM_ADDR_START != (uint32)Os_Core_App_DAddr[0U].APP_ADDR_START)
        {
            OS_MPU_ENABLE_REGION(OS_MPU_REGION_2,  OS_MPU_USER_RW, 
                                (uint32)OS_ARCH_RAM_ADDR_START, 
                                (uint32)Os_Core_App_DAddr[0].APP_ADDR_START - 4U);
        }

        /* RAM area in Os-Application, R */
        OS_MPU_ENABLE_REGION(OS_MPU_REGION_3,  OS_MPU_USER_R, 
                            (uint32)Os_Core_App_DAddr[0].APP_ADDR_START,
                            (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_END - 4);

        /* RAM area out of Os-Application, RW */
        if ((uint32)OS_ARCH_RAM_ADDR_END != (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_END)
        {
            OS_MPU_ENABLE_REGION(OS_MPU_REGION_4,  OS_MPU_USER_RW, 
                            (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_END, 
                            (uint32)OS_ARCH_RAM_ADDR_END - 4U);
        }
    }
    else
    {
        /* the address of Core0 is greater than the address of CoreN
         * e.g. CoreN application data is mapped to local ram 
         */

        /* RAM area out of Os-Application, RW */
        if ((uint32)OS_ARCH_RAM_ADDR_START != (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_START)
        {
            OS_MPU_ENABLE_REGION(OS_MPU_REGION_2, OS_MPU_USER_RW, 
                                (uint32)OS_ARCH_RAM_ADDR_START, 
                                (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_START - 4U);
        }

        /* RAM area in Os-Application, R */
        OS_MPU_ENABLE_REGION(OS_MPU_REGION_3, OS_MPU_USER_R, 
                            (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_START,
                            (uint32)Os_Core_App_DAddr[0].APP_ADDR_END - 4);

        /* RAM area out of Os-Application, RW */
        if ((uint32)OS_ARCH_RAM_ADDR_END != (uint32)Os_Core_App_DAddr[0].APP_ADDR_END)
        {
            OS_MPU_ENABLE_REGION(OS_MPU_REGION_4, OS_MPU_USER_RW, 
                            (uint32)Os_Core_App_DAddr[0].APP_ADDR_END, 
                            (uint32)OS_ARCH_RAM_ADDR_END - 4U);
        }
    }
}


/******************************************************************************/
/*
 * Brief                <Set memory protection map for trusted app>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <xxx>
 * Reentrancy           <Non Reentrant>
 * param[in]            <xxx>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <xxx>
 * REQ ID               <xxx>
 */
/******************************************************************************/
STATIC FUNC(void, OS_CODE) Os_ArchSetTruReg(void)
{
    /* Run time modification  */
    if ((uint32)Os_Core_App_DAddr[0].APP_ADDR_START < (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_START)
    {
        /* the address of CoreN is greater than the address of Core0
         * e.g. CoreN application data is mapped to cluster ram
         */

        /* RAM area out of Os-Application, RW */
        if ((uint32)OS_ARCH_RAM_ADDR_START != (uint32)Os_Core_App_DAddr[0U].APP_ADDR_START)
        {
            OS_MPU_ENABLE_REGION(OS_MPU_REGION_2,  OS_MPU_USER_RW, 
                                (uint32)OS_ARCH_RAM_ADDR_START, 
                                (uint32)Os_Core_App_DAddr[0].APP_ADDR_START - 4U);
        }

        /* RAM area in Os-Application, R */
        OS_MPU_ENABLE_REGION(OS_MPU_REGION_3,  OS_MPU_USER_RW, 
                            (uint32)Os_Core_App_DAddr[0].APP_ADDR_START,
                            (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_END - 4);

        /* RAM area out of Os-Application, RW */
        if ((uint32)OS_ARCH_RAM_ADDR_END != (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_END)
        {
            OS_MPU_ENABLE_REGION(OS_MPU_REGION_4,  OS_MPU_USER_RW, 
                            (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_END, 
                            (uint32)OS_ARCH_RAM_ADDR_END - 4U);
        }
    }
    else
    {
        /* the address of Core0 is greater than the address of CoreN
         * e.g. CoreN application data is mapped to local ram 
         */

        /* RAM area out of Os-Application, RW */
        if ((uint32)OS_ARCH_RAM_ADDR_START != (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_START)
        {
            OS_MPU_ENABLE_REGION(OS_MPU_REGION_2, OS_MPU_USER_RW, 
                                (uint32)OS_ARCH_RAM_ADDR_START, 
                                (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_START - 4U);
        }

        /* RAM area in Os-Application, R */
        OS_MPU_ENABLE_REGION(OS_MPU_REGION_3, OS_MPU_USER_RW, 
                            (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_START,
                            (uint32)Os_Core_App_DAddr[0].APP_ADDR_END - 4);

        /* RAM area out of Os-Application, RW */
        if ((uint32)OS_ARCH_RAM_ADDR_END != (uint32)Os_Core_App_DAddr[0].APP_ADDR_END)
        {
            OS_MPU_ENABLE_REGION(OS_MPU_REGION_4, OS_MPU_USER_RW, 
                            (uint32)Os_Core_App_DAddr[0].APP_ADDR_END, 
                            (uint32)OS_ARCH_RAM_ADDR_END - 4U);
        }
    }
}

/******************************************************************************/
/*
 * Brief                <Set memory protection map for isr of trusted app>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <xxx>
 * Reentrancy           <Non Reentrant>
 * param[in]            <xxx>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <xxx>
 * REQ ID               <xxx>
 */
/******************************************************************************/
STATIC FUNC(void, OS_CODE) Os_ArchSetTruIsrReg(ISRType IsrId, ApplicationType HostAppId)
{
    (void)IsrId;

    /* Run time modification  */
    if ((uint32)Os_Core_App_DAddr[0].APP_ADDR_START < (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_START)
    {
        /* the address of CoreN is greater than the address of Core0
         * e.g. CoreN application data is mapped to cluster ram
         */

        /* RAM area out of Os-Application, RW */
        if ((uint32)OS_ARCH_RAM_ADDR_START != (uint32)Os_Core_App_DAddr[0U].APP_ADDR_START)
        {
            OS_MPU_ENABLE_REGION(OS_MPU_REGION_2,  OS_MPU_USER_RW, 
                                (uint32)OS_ARCH_RAM_ADDR_START, 
                                (uint32)Os_Core_App_DAddr[0].APP_ADDR_START - 4U);
        }

        /* RAM area in Os-Application, R */
        OS_MPU_ENABLE_REGION(OS_MPU_REGION_3,  OS_MPU_USER_R, 
                            (uint32)Os_Core_App_DAddr[0].APP_ADDR_START,
                            (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_END - 4);

        /* RAM area out of Os-Application, RW */
        if ((uint32)OS_ARCH_RAM_ADDR_END != (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_END)
        {
            OS_MPU_ENABLE_REGION(OS_MPU_REGION_4,  OS_MPU_USER_RW, 
                            (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_END, 
                            (uint32)OS_ARCH_RAM_ADDR_END - 4U);
        }

    }
    else
    {
        /* the address of Core0 is greater than the address of CoreN
         * e.g. CoreN application data is mapped to local ram 
         */

        /* RAM area out of Os-Application, RW */
        if ((uint32)OS_ARCH_RAM_ADDR_START != (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_START)
        {
            OS_MPU_ENABLE_REGION(OS_MPU_REGION_2, OS_MPU_USER_RW, 
                                (uint32)OS_ARCH_RAM_ADDR_START, 
                                (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_START - 4U);
        }

        /* RAM area in Os-Application, R */
        OS_MPU_ENABLE_REGION(OS_MPU_REGION_3, OS_MPU_USER_R, 
                            (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_START,
                            (uint32)Os_Core_App_DAddr[0].APP_ADDR_END - 4);

        /* RAM area out of Os-Application, RW */
        if ((uint32)OS_ARCH_RAM_ADDR_END != (uint32)Os_Core_App_DAddr[0].APP_ADDR_END)
        {
            OS_MPU_ENABLE_REGION(OS_MPU_REGION_4, OS_MPU_USER_RW, 
                            (uint32)Os_Core_App_DAddr[0].APP_ADDR_END, 
                            (uint32)OS_ARCH_RAM_ADDR_END - 4U);
        }
    }

    /* RW */
    OS_MPU_ENABLE_REGION(OS_MPU_REGION_5, OS_MPU_USER_W, 
                        (uint32)Os_App_DAddr[HostAppId].APP_ADDR_START, 
                        (uint32)Os_App_DAddr[HostAppId].APP_ADDR_END - 4U);
}

/******************************************************************************/
/*
 * Brief                <>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <xxx>
 * Reentrancy           <Non Reentrant>
 * param[in]            <xxx>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <xxx>
 * REQ ID               <xxx>
 */
/******************************************************************************/
STATIC FUNC(void, OS_CODE) Os_ArchSetNonTruIsrReg(ISRType IsrId, ApplicationType HostAppId)
{
    (void)IsrId;

    /* Run time modification  */
    if ((uint32)Os_Core_App_DAddr[0].APP_ADDR_START < (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_START)
    {
        /* the address of CoreN is greater than the address of Core0
         * e.g. CoreN application data is mapped to cluster ram
         */

        /* RAM area out of Os-Application, R */
        if ((uint32)OS_ARCH_RAM_ADDR_START != (uint32)Os_Core_App_DAddr[0U].APP_ADDR_START)
        {
            OS_MPU_ENABLE_REGION(OS_MPU_REGION_2,  OS_MPU_USER_R, 
                                (uint32)OS_ARCH_RAM_ADDR_START, 
                                (uint32)Os_Core_App_DAddr[0].APP_ADDR_START - 4U);
        }

        /* RAM area in Os-Application, R */
        OS_MPU_ENABLE_REGION(OS_MPU_REGION_3,  OS_MPU_USER_NONE, 
                            (uint32)Os_Core_App_DAddr[0].APP_ADDR_START,
                            (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_END - 4);

        /* RAM area out of Os-Application, R */
        if ((uint32)OS_ARCH_RAM_ADDR_END != (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_END)
        {
            OS_MPU_ENABLE_REGION(OS_MPU_REGION_4,  OS_MPU_USER_R, 
                            (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_END, 
                            (uint32)OS_ARCH_RAM_ADDR_END - 4U);
        }
    }
    else
    {
        /* the address of Core0 is greater than the address of CoreN
         * e.g. CoreN application data is mapped to local ram 
         */

        /* RAM area out of Os-Application, R */
        if ((uint32)OS_ARCH_RAM_ADDR_START != (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_START)
        {
            OS_MPU_ENABLE_REGION(OS_MPU_REGION_2, OS_MPU_USER_R, 
                                (uint32)OS_ARCH_RAM_ADDR_START, 
                                (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_START - 4U);
        }

        /* RAM area in Os-Application, R */
        OS_MPU_ENABLE_REGION(OS_MPU_REGION_3, OS_MPU_USER_NONE, 
                            (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_START,
                            (uint32)Os_Core_App_DAddr[0].APP_ADDR_END - 4);

        /* RAM area out of Os-Application, R */
        if ((uint32)OS_ARCH_RAM_ADDR_END != (uint32)Os_Core_App_DAddr[0].APP_ADDR_END)
        {
            OS_MPU_ENABLE_REGION(OS_MPU_REGION_4, OS_MPU_USER_R, 
                            (uint32)Os_Core_App_DAddr[0].APP_ADDR_END, 
                            (uint32)OS_ARCH_RAM_ADDR_END - 4U);
        }
    }

    /* RW */
    OS_MPU_ENABLE_REGION(OS_MPU_REGION_5, OS_MPU_USER_RW, 
                        (uint32)Os_App_DAddr[HostAppId].APP_ADDR_START, 
                        (uint32)Os_App_DAddr[HostAppId].APP_ADDR_END - 4U);
}

/******************************************************************************/
/*
 * Brief                <MPU init state or trusted application>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <xxx>
 * Reentrancy           <Non Reentrant>
 * param[in]            <xxx>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <xxx>
 * REQ ID               <xxx>
 */
/******************************************************************************/
STATIC FUNC(void, OS_CODE) Os_ArchSetTruTaskReg(TaskType TaskId, ApplicationType HostAppId)
{
    (void)TaskId;

    /* Run time modification  */
    if ((uint32)Os_Core_App_DAddr[0].APP_ADDR_START < (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_START)
    {
        /* the address of CoreN is greater than the address of Core0
         * e.g. CoreN application data is mapped to cluster ram
         */

        /* RAM area out of Os-Application, R */
        if ((uint32)OS_ARCH_RAM_ADDR_START != (uint32)Os_Core_App_DAddr[0U].APP_ADDR_START)
        {
            OS_MPU_ENABLE_REGION(OS_MPU_REGION_2,  OS_MPU_USER_R, 
                                (uint32)OS_ARCH_RAM_ADDR_START, 
                                (uint32)Os_Core_App_DAddr[0].APP_ADDR_START - 4U);
        }

        /* RAM area in Os-Application, R */
        OS_MPU_ENABLE_REGION(OS_MPU_REGION_3,  OS_MPU_USER_R, 
                            (uint32)Os_Core_App_DAddr[0].APP_ADDR_START,
                            (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_END - 4);

        /* RAM area out of Os-Application, R */
        if ((uint32)OS_ARCH_RAM_ADDR_END != (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_END)
        {
            OS_MPU_ENABLE_REGION(OS_MPU_REGION_4,  OS_MPU_USER_R, 
                            (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_END, 
                            (uint32)OS_ARCH_RAM_ADDR_END - 4U);
        }
    }
    else
    {
        /* the address of Core0 is greater than the address of CoreN
         * e.g. CoreN application data is mapped to local ram 
         */

        /* RAM area out of Os-Application, R */
        if ((uint32)OS_ARCH_RAM_ADDR_START != (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_START)
        {
            OS_MPU_ENABLE_REGION(OS_MPU_REGION_2, OS_MPU_USER_R, 
                                (uint32)OS_ARCH_RAM_ADDR_START, 
                                (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_START - 4U);
        }

        /* RAM area in Os-Application, R */
        OS_MPU_ENABLE_REGION(OS_MPU_REGION_3, OS_MPU_USER_R, 
                            (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_START,
                            (uint32)Os_Core_App_DAddr[0].APP_ADDR_END - 4);

        /* RAM area out of Os-Application, R */
        if ((uint32)OS_ARCH_RAM_ADDR_END != (uint32)Os_Core_App_DAddr[0].APP_ADDR_END)
        {
            OS_MPU_ENABLE_REGION(OS_MPU_REGION_4, OS_MPU_USER_R, 
                            (uint32)Os_Core_App_DAddr[0].APP_ADDR_END, 
                            (uint32)OS_ARCH_RAM_ADDR_END - 4U);
        }
    }

    /* RW */
    OS_MPU_ENABLE_REGION(OS_MPU_REGION_5, OS_MPU_USER_RW, 
                        (uint32)Os_App_DAddr[HostAppId].APP_ADDR_START, 
                        (uint32)Os_App_DAddr[HostAppId].APP_ADDR_END - 4U);
}

/******************************************************************************/
/*
 * Brief                <>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <xxx>
 * Reentrancy           <Non Reentrant>
 * param[in]            <xxx>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <xxx>
 * REQ ID               <xxx>
 */
/******************************************************************************/
STATIC FUNC(void, OS_CODE) Os_ArchSetNonTruTaskReg(TaskType TaskId, ApplicationType HostAppId)
{
    (void)TaskId;

    /* Run time modification  */
    if ((uint32)Os_Core_App_DAddr[0].APP_ADDR_START < (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_START)
    {
        /* the address of CoreN is greater than the address of Core0
         * e.g. CoreN application data is mapped to cluster ram
         */

        /* RAM area out of Os-Application, R */
        if ((uint32)OS_ARCH_RAM_ADDR_START != (uint32)Os_Core_App_DAddr[0U].APP_ADDR_START)
        {
            OS_MPU_ENABLE_REGION(OS_MPU_REGION_2,  OS_MPU_USER_R, 
                                (uint32)OS_ARCH_RAM_ADDR_START, 
                                (uint32)Os_Core_App_DAddr[0].APP_ADDR_START - 4U);
        }

        /* RAM area in Os-Application, None */
        OS_MPU_ENABLE_REGION(OS_MPU_REGION_3,  OS_MPU_USER_NONE, 
                            (uint32)Os_Core_App_DAddr[0].APP_ADDR_START,
                            (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_END - 4);

        /* RAM area out of Os-Application, R */
        if ((uint32)OS_ARCH_RAM_ADDR_END != (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_END)
        {
            OS_MPU_ENABLE_REGION(OS_MPU_REGION_4, OS_MPU_USER_R, 
                            (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_END, 
                            (uint32)OS_ARCH_RAM_ADDR_END - 4U);
        }
    }
    else
    {
        /* the address of Core0 is greater than the address of CoreN
         * e.g. CoreN application data is mapped to local ram 
         */

        /* RAM area out of Os-Application, R */
        if ((uint32)OS_ARCH_RAM_ADDR_START != (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_START)
        {
            OS_MPU_ENABLE_REGION(OS_MPU_REGION_2, OS_MPU_USER_R, 
                                (uint32)OS_ARCH_RAM_ADDR_START, 
                                (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_START - 4U);
        }

        /* RAM area in Os-Application, None */
        OS_MPU_ENABLE_REGION(OS_MPU_REGION_3, OS_MPU_USER_NONE, 
                            (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_START,
                            (uint32)Os_Core_App_DAddr[0].APP_ADDR_END - 4);

        /* RAM area out of Os-Application, R */
        if ((uint32)OS_ARCH_RAM_ADDR_END != (uint32)Os_Core_App_DAddr[0].APP_ADDR_END)
        {
            OS_MPU_ENABLE_REGION(OS_MPU_REGION_4, OS_MPU_USER_R, 
                            (uint32)Os_Core_App_DAddr[0].APP_ADDR_END, 
                            (uint32)OS_ARCH_RAM_ADDR_END - 4U);
        }
    }

    /* RW */
    OS_MPU_ENABLE_REGION(OS_MPU_REGION_5, OS_MPU_USER_RW, 
                        (uint32)Os_App_DAddr[HostAppId].APP_ADDR_START, 
                        (uint32)Os_App_DAddr[HostAppId].APP_ADDR_END - 4U);
}

/******************************************************************************/
/*
 * Brief                <Os_ArchSetIsrMemMap>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_EnterISR2,Os_ExitISR2>
 * REQ ID               <DD_1_0758, DD_1_0759, DD_1_1308, DD_1_1309, DD_1_1310>
 */
/******************************************************************************/
FUNC(void,OS_CODE) Os_ArchSetIsrMemMap
(
    ISRType         IsrId, 
    ApplicationType HostAppId, 
    uint32          isTrusted
)
{
    if (TRUE != isTrusted) /* Non-trusted app running. */
    {
        Os_ArchSetNonTruIsrReg(IsrId, HostAppId);
    }
    else  /* Trusted app or os_kernel running. */
    {
        /*OSTrustedApplicationWithProtection != TRUE*/
        if (Os_AppCfg[HostAppId].OsTrustedAppWithProtection != TRUE)
        {
            Os_ArchSetTruReg();
        }
        else
        {
            Os_ArchSetTruIsrReg(IsrId,HostAppId);
        }
    }
}

/******************************************************************************/
/*
 * Brief                <MemMap for Memory Protection Settings Task>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ExitISR2, Os_SwitchTask>
 * REQ ID               <DD_1_0760, DD_1_0761, DD_1_1311, DD_1_1312, DD_1_1313>
 */
/******************************************************************************/
FUNC(void,OS_CODE) Os_ArchSetTaskMemMap
(
    TaskType        TaskId, 
    ApplicationType HostAppId, 
    uint32          isTrusted
)
{
    if (TRUE != isTrusted) /* Non-trusted app running. */
    {
        Os_ArchSetNonTruTaskReg(TaskId, HostAppId);
    }
    else  /* Trusted app or os_kernel running. */
    {
        /*OSTrustedApplicationWithProtection != TRUE*/
        if (Os_AppCfg[HostAppId].OsTrustedAppWithProtection != TRUE)
        {
            Os_ArchSetTruReg();
        }
        else
        {
            Os_ArchSetTruTaskReg(TaskId, HostAppId);
        }
    }
}

/******************************************************************************/
/*
 * Brief                <Cat1 MemMap for Memory Protection Settings Task>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_MemProtTaskCat1Map(void)
{
    VAR(ApplicationType, OS_VAR) sysAppId = Os_SCB.sysAppId;
    VAR(Os_ApplicationType, OS_VAR) sysRunningAppID = Os_SCB.sysRunningAppID;

    if (sysAppId != sysRunningAppID)
    {
        /* Memory protection: Set memory map according to new running task. */
        /*new task,default set register 03*/
        Os_ArchSetTaskMemMap(
            Os_GetObjLocalId(Os_SCB.sysRunningTaskID),
            sysRunningAppID,
            (uint32)Os_AppCfg[sysRunningAppID].OsTrusted);
    }
    else
    {
        /*SYS_APP, as OS kernel, have all access rights*/
        Os_ArchSetTruReg();
    }
}

/******************************************************************************/
/*
 * Brief                <Cat2 MemMap for Memory Protection Settings Task>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_MemProtTaskCat2Map(void)
{
    VAR(ApplicationType, OS_VAR) sysAppId = Os_SCB.sysAppId;
    VAR(Os_ApplicationType, OS_VAR) sysRunningAppID = Os_SCB.sysRunningAppID;

    if (sysAppId != sysRunningAppID)
    {
        /* Memory protection: Set memory map according to new running task. */
        Os_ArchSetTaskMemMap(
            Os_GetObjLocalId(Os_SCB.sysRunningTaskID),
            sysRunningAppID,
            (uint32)Os_AppCfg[sysRunningAppID].OsTrusted);
    }
    else
    {
        /*SYS_APP, as OS kernel, have all access rights*/
        Os_ArchSetTruReg();
    }
}

/******************************************************************************/
/*
 * Brief                <MemMap for Memory Protection Settings ISR>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_MemProtIsrMap(void)
{
    /*if memory protection is configured*/
    /*Preparing to enter the ISR2 outside with its memory region table*/
    VAR(Os_IsrType, OS_VAR) epilogueISR = Os_SCB.sysRunningIsrCat2Id;
    VAR(Os_ApplicationType, OS_VAR) epilogueapp = Os_SCB.sysRunningAppID;

    /*find pre ISR's PSW*/
    if (Os_SCB.sysAppId != epilogueapp)
    {
        Os_ArchSetIsrMemMap(epilogueISR, epilogueapp, (uint32)Os_AppCfg[epilogueapp].OsTrusted);
    }
    else
    {
        /*SYS_APP, as OS kernel, have all access rights*/
        Os_ArchSetTruReg();
    }
}


#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */
/*=======[E N D   O F   F I L E]==============================================*/

