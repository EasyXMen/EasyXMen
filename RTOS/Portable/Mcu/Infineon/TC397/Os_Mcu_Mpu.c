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
 ************************************************************************************************************************
 **
 **  @file               : Os_Mcu_Mpu.c
 **  @author             : i-soft-os
 **  @date               : 2025/02/10
 **  @vendor             : isoft
 **  @description        : MCU source file for Mpu API implementations
 **
 ***********************************************************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Os_Arch_Processor.h"
#include "Os_Interrupt.h"
#include "Os_Task.h"
#include "Os_Kernel2Port.h"
#include "Os_ProtectHook.h"

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/
#if (OS_MPROT_REGION_CFG_ENABLE == TRUE)
#define OS_START_SEC_VAR_CLONE_8
#include "Os_MemMap.h"
static uint8 Os_MpuRWUsedRegionNum; /* Data MPU memory region used by kernel and core */
#define OS_STOP_SEC_VAR_CLONE_8
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_8
#include "Os_MemMap.h"
static uint8 Os_MpuExeUsedRegionNum; /* Code MPU memory region used by kernel and core */
#define OS_STOP_SEC_VAR_CLONE_8
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
static uint32 Os_MpuCoreReadAccess;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
static uint32 Os_MpuCoreWriteAccess;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
static uint32 Os_MpuCoreExecuteAccess;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
void Os_Set_Mpu_CPR(uint8 i, uint32 value)
{
    switch (i)
    {
    case 0:
        OS_ARCH_MTCR((OS_CPR_0L) + 0 * 4, value);
        break; // reserve for kernel
    case 1:
        OS_ARCH_MTCR((OS_CPR_0L) + 1 * 4, value);
        break; // reserve for kernel
    case 2:
        OS_ARCH_MTCR((OS_CPR_0L) + 2 * 4, value);
        break;
    case 3:
        OS_ARCH_MTCR((OS_CPR_0L) + 3 * 4, value);
        break;
    case 4:
        OS_ARCH_MTCR((OS_CPR_0L) + 4 * 4, value);
        break;
    case 5:
        OS_ARCH_MTCR((OS_CPR_0L) + 5 * 4, value);
        break;
    case 6:
        OS_ARCH_MTCR((OS_CPR_0L) + 6 * 4, value);
        break;
    case 7:
        OS_ARCH_MTCR((OS_CPR_0L) + 7 * 4, value);
        break;
    case 8:
        OS_ARCH_MTCR((OS_CPR_0L) + 8 * 4, value);
        break;
    case 9:
        OS_ARCH_MTCR((OS_CPR_0L) + 9 * 4, value);
        break;
    case 10:
        OS_ARCH_MTCR((OS_CPR_0L) + 10 * 4, value);
        break;
    case 11:
        OS_ARCH_MTCR((OS_CPR_0L) + 11 * 4, value);
        break;
    case 12:
        OS_ARCH_MTCR((OS_CPR_0L) + 12 * 4, value);
        break;
    case 13:
        OS_ARCH_MTCR((OS_CPR_0L) + 13 * 4, value);
        break;
    case 14:
        OS_ARCH_MTCR((OS_CPR_0L) + 14 * 4, value);
        break;
    case 15:
        OS_ARCH_MTCR((OS_CPR_0L) + 15 * 4, value);
        break;
    case 16:
        OS_ARCH_MTCR((OS_CPR_0L) + 16 * 4, value);
        break;
    case 17:
        OS_ARCH_MTCR((OS_CPR_0L) + 17 * 4, value);
        break;
    case 18:
        OS_ARCH_MTCR((OS_CPR_0L) + 18 * 4, value);
        break;
    default:
        while (1)
            ;
        break;
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
void Os_Set_Mpu_DPR(uint8 i, uint32 value)
{
    switch (i)
    {
    case 0:
        OS_ARCH_MTCR((OS_DPR_0L) + 0 * 4, value);
        break; // reserve for kernel
    case 1:
        OS_ARCH_MTCR((OS_DPR_0L) + 1 * 4, value);
        break; // reserve for kernel
    case 2:
        OS_ARCH_MTCR((OS_DPR_0L) + 2 * 4, value);
        break;
    case 3:
        OS_ARCH_MTCR((OS_DPR_0L) + 3 * 4, value);
        break;
    case 4:
        OS_ARCH_MTCR((OS_DPR_0L) + 4 * 4, value);
        break;
    case 5:
        OS_ARCH_MTCR((OS_DPR_0L) + 5 * 4, value);
        break;
    case 6:
        OS_ARCH_MTCR((OS_DPR_0L) + 6 * 4, value);
        break;
    case 7:
        OS_ARCH_MTCR((OS_DPR_0L) + 7 * 4, value);
        break;
    case 8:
        OS_ARCH_MTCR((OS_DPR_0L) + 8 * 4, value);
        break;
    case 9:
        OS_ARCH_MTCR((OS_DPR_0L) + 9 * 4, value);
        break;
    case 10:
        OS_ARCH_MTCR((OS_DPR_0L) + 10 * 4, value);
        break;
    case 11:
        OS_ARCH_MTCR((OS_DPR_0L) + 11 * 4, value);
        break;
    case 12:
        OS_ARCH_MTCR((OS_DPR_0L) + 12 * 4, value);
        break;
    case 13:
        OS_ARCH_MTCR((OS_DPR_0L) + 13 * 4, value);
        break;
    case 14:
        OS_ARCH_MTCR((OS_DPR_0L) + 14 * 4, value);
        break;
    case 15:
        OS_ARCH_MTCR((OS_DPR_0L) + 15 * 4, value);
        break;
    case 16:
        OS_ARCH_MTCR((OS_DPR_0L) + 16 * 4, value);
        break;
    case 17:
        OS_ARCH_MTCR((OS_DPR_0L) + 17 * 4, value);
        break;
    case 18:
        OS_ARCH_MTCR((OS_DPR_0L) + 18 * 4, value);
        break;
    case 19:
        OS_ARCH_MTCR((OS_DPR_0L) + 19 * 4, value);
        break;
    case 20:
        OS_ARCH_MTCR((OS_DPR_0L) + 20 * 4, value);
        break;
    case 21:
        OS_ARCH_MTCR((OS_DPR_0L) + 21 * 4, value);
        break;
    case 22:
        OS_ARCH_MTCR((OS_DPR_0L) + 22 * 4, value);
        break;
    case 23:
        OS_ARCH_MTCR((OS_DPR_0L) + 23 * 4, value);
        break;
    case 24:
        OS_ARCH_MTCR((OS_DPR_0L) + 24 * 4, value);
        break;
    case 25:
        OS_ARCH_MTCR((OS_DPR_0L) + 25 * 4, value);
        break;
    case 26:
        OS_ARCH_MTCR((OS_DPR_0L) + 26 * 4, value);
        break;
    case 27:
        OS_ARCH_MTCR((OS_DPR_0L) + 27 * 4, value);
        break;
    case 28:
        OS_ARCH_MTCR((OS_DPR_0L) + 28 * 4, value);
        break;
    case 29:
        OS_ARCH_MTCR((OS_DPR_0L) + 29 * 4, value);
        break;
    case 30:
        OS_ARCH_MTCR((OS_DPR_0L) + 30 * 4, value);
        break;
    case 31:
        OS_ARCH_MTCR((OS_DPR_0L) + 31 * 4, value);
        break;
    case 32:
        OS_ARCH_MTCR((OS_DPR_0L) + 32 * 4, value);
        break;
    case 33:
        OS_ARCH_MTCR((OS_DPR_0L) + 33 * 4, value);
        break;
    case 34:
        OS_ARCH_MTCR((OS_DPR_0L) + 34 * 4, value);
        break;
    default:
        while (1)
            ;
        break;
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/* PRQA S 3450, 3006 ++ */ /* VL_Os_3450, VL_Os_3006 */
static void Os_Hal_SetConfigKernelMemoryRegionAccess(void)
/* PRQA S 3450, 3006 -- */
{
#if (OS_MPROT_REGION_CFG_ENABLE == TRUE)
    /* Region 1 is reserved for kernel */
    Os_MpuRWUsedRegionNum  = 1;
    Os_MpuExeUsedRegionNum = 1;
#endif
    /* Set tc275 reg_set, use set_0 */
    /* Set-up the Data regions for the Os_Kernel. */
    /* PRQA S 3138, 1006 ++ */ /* VL_Os_3138, VL_Os_1006 */
    /* PRQA S 0306 ++ */ /* VL_Os_0306 */
    OS_ARCH_MTCR(OS_DPR_0L, (uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pCPU5DataRamStart);
    OS_ARCH_MTCR(OS_DPR_0U, (uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pPeripheralEnd);

    /* Set-up the Code regions for the Kernel. */
    OS_ARCH_MTCR(OS_CPR_0L, (uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRomStart);
    OS_ARCH_MTCR(OS_CPR_0U, (uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRomEnd);
    /* PRQA S 0306 -- */ /* VL_Os_0306 */

    /* Set privileges of data and code sections. */
    OS_ARCH_MTCR(OS_DPRE0, 0x0001u); /* All data regions are R */
    OS_ARCH_MTCR(OS_DPWE0, 0x0001u); /* All data regions are W */
    OS_ARCH_MTCR(OS_CPXE0, 0x0001u); /* All code regions are E */
    /* PRQA S 3138, 1006 -- */

    Os_ArchSetMemProtSet(0x00UL);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (OS_MPROT_REGION_CFG_ENABLE == TRUE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static void Os_Hal_SetConfigCoreMemoryRegionAccess(void)
{
    Os_CoreIdType                 coreId        = Os_McuGetCoreID();
    const Os_MemoryRegionObjType* coreMemRegion = &Os_Memory_Region_Core_Inf[coreId];

    if (0u != coreMemRegion->regionsNum)
    {
        Os_MpuCoreReadAccess    = 0;
        Os_MpuCoreWriteAccess   = 0;
        Os_MpuCoreExecuteAccess = 0;

        for (uint8 i = 0; i < coreMemRegion->regionsNum; i++)
        {
            const Os_MemoryRegionAttrType* pRegion = coreMemRegion->regionsRef[i];
            if ((pRegion->access & (OS_READBIT | OS_WRITEBIT | OS_STACKSPACEBIT)) != 0)
            {
                Os_Set_Mpu_DPR(Os_MpuRWUsedRegionNum * 2, pRegion->startAddr);
                Os_Set_Mpu_DPR(Os_MpuRWUsedRegionNum * 2 + 1, pRegion->endAddr);
                if ((pRegion->access & (OS_READBIT | OS_STACKSPACEBIT)) != 0)
                {
                    Os_MpuCoreReadAccess |= OS_ARCH_SET_MP_ACCESS_RIGHT(0x1U, Os_MpuRWUsedRegionNum);
                }
                if ((pRegion->access & (OS_WRITEBIT | OS_STACKSPACEBIT)) != 0)
                {
                    Os_MpuCoreWriteAccess |= OS_ARCH_SET_MP_ACCESS_RIGHT(0x1U, Os_MpuRWUsedRegionNum);
                }
                Os_MpuRWUsedRegionNum++;
            }
            if ((pRegion->access & (OS_EXECUTBIT)) != 0)
            {
                Os_Set_Mpu_CPR(Os_MpuExeUsedRegionNum * 2, pRegion->startAddr);
                Os_Set_Mpu_CPR(Os_MpuExeUsedRegionNum * 2 + 1, pRegion->endAddr);
                Os_MpuCoreExecuteAccess |= OS_ARCH_SET_MP_ACCESS_RIGHT(0x1U, Os_MpuExeUsedRegionNum);
                Os_MpuExeUsedRegionNum++;
            }
        }
        if (Os_MpuCoreReadAccess != 0)
        {
            Os_MpuCoreReadAccess |= OS_ARCH_MFCR(OS_DPRE1);
            OS_ARCH_MTCR((OS_DPRE1), Os_MpuCoreReadAccess);
        }
        if (Os_MpuCoreWriteAccess != 0)
        {
            Os_MpuCoreWriteAccess |= OS_ARCH_MFCR(OS_DPWE1);
            OS_ARCH_MTCR((OS_DPWE1), Os_MpuCoreWriteAccess);
        }
        if (Os_MpuCoreExecuteAccess != 0)
        {
            Os_MpuCoreExecuteAccess |= OS_ARCH_MFCR(OS_CPXE1);
            OS_ARCH_MTCR((OS_CPXE1), Os_MpuCoreExecuteAccess);
        }
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
void Os_Hal_SetConfigObjMemoryRegionAccess(ApplicationType appId, ObjectTypeType objType, ObjectType objId)
{
    uint8                          i    = 0;
    uint32                         read = 0, write = 0, execute = 0;
    uint8                          OsDPRStart   = Os_MpuRWUsedRegionNum;
    uint8                          OsCPRStart   = Os_MpuExeUsedRegionNum;
    const Os_MemoryRegionObjType*  appMemRegion = &Os_Memory_Region_Application_Inf[appId];
    const Os_MemoryRegionObjType*  objMemRegion;
    const Os_MemoryRegionAttrType* pRegion;

    if (0 != appMemRegion->regionsNum)
    {
        for (i = 0; i < appMemRegion->regionsNum; i++)
        {
            pRegion = appMemRegion->regionsRef[i];
            if ((pRegion->access & (OS_READBIT | OS_WRITEBIT | OS_STACKSPACEBIT)) != 0)
            {
                Os_Set_Mpu_DPR(OsDPRStart * 2, pRegion->startAddr);
                Os_Set_Mpu_DPR(OsDPRStart * 2 + 1, pRegion->endAddr);
                if ((pRegion->access & (OS_READBIT | OS_STACKSPACEBIT)) != 0)
                {
                    read |= OS_ARCH_SET_MP_ACCESS_RIGHT(0x1U, OsDPRStart);
                }
                if ((pRegion->access & (OS_WRITEBIT | OS_STACKSPACEBIT)) != 0)
                {
                    write |= OS_ARCH_SET_MP_ACCESS_RIGHT(0x1U, OsDPRStart);
                }
                OsDPRStart++;
            }
            if ((pRegion->access & (OS_EXECUTBIT)) != 0)
            {
                Os_Set_Mpu_CPR(OsCPRStart * 2, pRegion->startAddr);
                Os_Set_Mpu_CPR(OsCPRStart * 2 + 1, pRegion->endAddr);
                execute |= OS_ARCH_SET_MP_ACCESS_RIGHT(0x1U, OsCPRStart);
                OsCPRStart++;
            }
        }
    }

    if (objType == OS_OBJECT_ISR)
    {
        objMemRegion = &Os_Memory_Region_Isr_Inf[objId];
    }
    else
    {
        objMemRegion = &Os_Memory_Region_Task_Inf[objId];
    }

    if (0 != objMemRegion->regionsNum)
    {
        for (i = 0; i < objMemRegion->regionsNum; i++)
        {
            pRegion = objMemRegion->regionsRef[i];
            if ((pRegion->access & (OS_READBIT | OS_WRITEBIT | OS_STACKSPACEBIT)) != 0)
            {
                Os_Set_Mpu_DPR(OsDPRStart * 2, pRegion->startAddr);
                Os_Set_Mpu_DPR(OsDPRStart * 2 + 1, pRegion->endAddr);
                if ((pRegion->access & (OS_READBIT | OS_STACKSPACEBIT)) != 0)
                {
                    read |= OS_ARCH_SET_MP_ACCESS_RIGHT(0x1U, OsDPRStart);
                }
                if ((pRegion->access & (OS_WRITEBIT | OS_STACKSPACEBIT)) != 0)
                {
                    write |= OS_ARCH_SET_MP_ACCESS_RIGHT(0x1U, OsDPRStart);
                }
                OsDPRStart++;
            }
            if ((pRegion->access & (OS_EXECUTBIT)) != 0)
            {
                Os_Set_Mpu_CPR(OsCPRStart * 2, pRegion->startAddr);
                Os_Set_Mpu_CPR(OsCPRStart * 2 + 1, pRegion->endAddr);
                execute |= OS_ARCH_SET_MP_ACCESS_RIGHT(0x1U, OsCPRStart);
                OsCPRStart++;
            }
        }
    }
    if (read != 0)
    {
        read |= Os_MpuCoreReadAccess;
        OS_ARCH_MTCR((OS_DPRE1), read);
    }
    if (write != 0)
    {
        write |= Os_MpuCoreWriteAccess;
        OS_ARCH_MTCR((OS_DPWE1), write);
    }
    if (execute != 0)
    {
        execute |= Os_MpuCoreExecuteAccess;
        OS_ARCH_MTCR((OS_CPXE1), execute);
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Os_McuSetIsrMemMap>
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
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3450, 3006 ++ */ /* VL_Os_3450, VL_Os_3006 */
static void Os_McuSetIsrMemMap(ISRType IsrId, ApplicationType HostAppId, boolean isTrusted)
/* PRQA S 3450, 3006 -- */
{
    /* PRQA S 3120, 3006 ++ */ /* VL_QAC_MagicNum, VL_Os_3006 */
    uint32 temp = (Os_ArchGetMemProtSet() & 0x00003000U) >> 12u;
    /* PRQA S 3120, 3006 -- */

    /* Find out which register set in use  register 0 should not be used here */
    if (0x02U == temp)/* PRQA S 3120 */ /* VL_QAC_MagicNum */
    {
#if (OS_MPROT_REGION_CFG_ENABLE == FALSE)
        CoreIdType coreId = OS_ISR_GET_COREID(IsrId);
        /*set 2: OS_DPR_7~OS_DPR_12, OS_CPR_4~OS_CPR_6*/
        /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum*/
        uint8 OsDPRStart = 7u;
        uint8 OsCPRStart = 4u;
        /* PRQA S 3120 -- */

        /* PRQA S 1006, 1259, 0306, 1258, 1277 ++ */ /* VL_Os_1006, VL_Os_1259, VL_Os_0306, VL_Os_ConstToIntegral, VL_Os_1277 */
        /* PRQA S 3432, 3138 ++ */   /* VL_Os_3432, VL_Os_3138 */
        if (TRUE != isTrusted) /* Non-trusted app running. */
        {
            Os_ArchSetNonTruIsrReg(
                OS_DPR_7L,
                OS_CPR_4L,
                OsDPRStart,
                OsCPRStart,
                OS_DPRE2,
                OS_DPWE2,
                OS_CPXE2,
                IsrId,
                HostAppId,
                coreId);/* PRQA S 3141 */ /* VL_Os_3141 */
        }
        else /* Trusted app or os_kernel running. */
        {
            /*OSTrustedApplicationWithProtection != TRUE*/
            if (Os_AppCfg[HostAppId].TrustedAppWithProtection != TRUE)
            {
                Os_ArchSetTruReg(OS_DPR_7L, OS_CPR_4L, OsDPRStart, OsCPRStart, OS_DPRE2, OS_DPWE2, OS_CPXE2);
            }
            else
            {
                Os_ArchSetTruIsrProtReg(
                    OS_DPR_7L,
                    OS_CPR_4L,
                    OsDPRStart,
                    OsCPRStart,
                    OS_DPRE2,
                    OS_DPWE2,
                    OS_CPXE2,
                    IsrId,
                    HostAppId);/* PRQA S 3141 */ /*  VL_Os_3141 */
            }
        }
        /* PRQA S 3432, 3138 -- */
        /* PRQA S 1006, 1259, 0306, 1258, 1277 -- */
#else
        if (TRUE == isTrusted && TRUE != Os_AppCfg[HostAppId].TrustedAppWithProtection)
        {
            Os_ArchSetMemProtSet(0x00U);
        }
        else
        {
            Os_Hal_SetConfigObjMemoryRegionAccess(HostAppId, OS_OBJECT_ISR, IsrId);
        }
#endif
    }
    else
    {
        Os_ExceptionHandler((void *)0, E_OS_PROTECTION_MEMORY); /* PRQA S 1520 */ /* VL_Os_1520 */
    }

    UNUSED_PARAMETER(IsrId);
    UNUSED_PARAMETER(isTrusted);

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3450, 3006 ++ */ /* VL_Os_3450, VL_Os_3006 */
static void Os_McuSetTaskMemMap(TaskType TaskId, ApplicationType HostAppId, boolean isTrusted)
/* PRQA S 3450, 3006 -- */
{
    uint32 temp = (Os_ArchGetMemProtSet() & 0x00003000U) >> 12u;/* PRQA S 3120 */ /* VL_QAC_MagicNum*/
    /* Find out which register set in use  register 0 should not be used here */
    if (0x01U == temp)
    {
#if (OS_MPROT_REGION_CFG_ENABLE == FALSE)
        CoreIdType coreId = OS_TASK_GET_COREID(TaskId);
        /*set 1: OS_DPR_1~OS_DPR_6, OS_CPR_1~OS_CPR_3*/
        uint8 OsDPRStart = 1u;
        uint8 OsCPRStart = 1u;

        /* PRQA S 1006, 1259, 0306, 1258, 1277 ++ */ /* VL_Os_1006, VL_Os_1259, VL_Os_0306, VL_Os_ConstToIntegral, VL_Os_1277 */
        /* PRQA S 3432, 3138 ++ */   /* VL_Os_3432, VL_Os_3138 */
        if (TRUE != isTrusted) /* Non-trusted app running. */
        {
            Os_ArchSetNonTruTskReg(
                OS_DPR_1L,
                OS_CPR_1L,
                OsDPRStart,
                OsCPRStart,
                OS_DPRE1,
                OS_DPWE1,
                OS_CPXE1,
                TaskId,
                HostAppId,
                coreId);/* PRQA S 3141 */ /* VL_Os_3141 */
        }
        else /* Trusted app or os_kernel running. */
        {
            /*OSTrustedApplicationWithProtection != TRUE*/
            if (Os_AppCfg[HostAppId].TrustedAppWithProtection != TRUE)
            {
                Os_ArchSetTruReg(OS_DPR_1L, OS_CPR_1L, OsDPRStart, OsCPRStart, OS_DPRE1, OS_DPWE1, OS_CPXE1);
            }
            else
            {
                Os_ArchSetTruTskProtReg(
                    OS_DPR_1L,
                    OS_CPR_1L,
                    OsDPRStart,
                    OsCPRStart,
                    OS_DPRE1,
                    OS_DPWE1,
                    OS_CPXE1,
                    TaskId,
                    HostAppId);/* PRQA S 3141 */ /* VL_Os_3141 */
            }
        }
        /* PRQA S 3432, 3138 -- */
        /* PRQA S 1006, 1259, 0306, 1258, 1277 -- */
#else
        if (TRUE == isTrusted && TRUE != Os_AppCfg[HostAppId].TrustedAppWithProtection)
        {
            Os_ArchSetMemProtSet(0x00U);
        }
        else
        {
            Os_Hal_SetConfigObjMemoryRegionAccess(HostAppId, OS_OBJECT_TASK, TaskId);
        }
#endif
    }
    else
    {
        Os_ExceptionHandler((void *)0, E_OS_PROTECTION_MEMORY); /* PRQA S 1520 */ /* VL_Os_1520 */
    }

    UNUSED_PARAMETER(TaskId);
    UNUSED_PARAMETER(isTrusted);

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
 * CallByAPI            <Os_SwitchTask>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_Hal_MemProtTaskMap(Os_TaskType taskId)
{
    Os_ApplicationType AppID  = Os_TaskCfg[taskId].ObjAppCfg->HostApp;
    /* Memory protection: Set memory map according to new running task. */
    Os_ArchSetMemProtSet(0x01U);
    Os_McuSetTaskMemMap(taskId, AppID, Os_AppCfg[AppID].Trusted); /* PRQA S 1520 */ /* VL_Os_1520 */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
 * CallByAPI            <Os_EnterISR2,Os_ExitISR2>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_Hal_MemProtIsrMap(Os_IsrType isrId) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    Os_ApplicationType AppID = Os_IsrCfg[isrId].ObjAppCfg->HostApp;
    /*Preparing to enter the ISR2 routine defined by user with its memory region table*/
    /*New ISR, defualt set register 2*/
    Os_ArchSetMemProtSet(0x02U); /* PRQA S 3120 */ /* VL_QAC_MagicNum*/
    Os_McuSetIsrMemMap(isrId, AppID, Os_AppCfg[AppID].Trusted); /* PRQA S 1520 */ /* VL_Os_1520 */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
 * CallByAPI            <Os_EnterISR2,Os_ExitISR2>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_Hal_MemProtKernelMap(void)
{
    /*SYS_APP, as OS kernel, have all access rights*/
    Os_ArchSetMemProtSet(0x00U);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Initialize memory protection related registers>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <Os_SwitchTask,Os_InitMemProt>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_Hal_InitKnMemMap(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    Os_Hal_SetConfigKernelMemoryRegionAccess();

#if (OS_MPROT_REGION_CFG_ENABLE == TRUE)
    Os_Hal_SetConfigCoreMemoryRegionAccess();
#endif
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */

/*=======[E N D   O F   F I L E]==============================================*/
/* PRQA S 0553 EOF */ /* VL_QAC_UnUsedFiles */
