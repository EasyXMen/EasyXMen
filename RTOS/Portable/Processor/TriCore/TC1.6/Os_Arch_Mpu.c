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
 **  @file               : Os_Arch_Mpu.c
 **  @author             : i-soft-os
 **  @date               : 2025/02/10
 **  @vendor             : isoft
 **  @description        : Arch source file for Mpu API implementations
 **
 ***********************************************************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Os_Arch_Processor.h"
#include "Os_Kernel2Port.h"
#include "Os_Appl.h"

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
/*=======[M A C R O S]========================================================*/
#define ARCH_PSW_IO_MASK      (0xfffff7ffU)
#define ARCH_PSW_IO_USER1_BIT (0x00000400U)
#define ARCH_PSW_PRS_BIT      (12U)

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/
#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
uint32 Os_PSW_PRS;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Change csa PSW.IO bits to User-1 Mode>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_Hal_FirstEnterTask>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_ModeModify(void) /* PRQA S 3006, 1532 */ /* VL_Os_3006, VL_QAC_OneFunRef */
{
#if (TRUE == CFG_SRV_SHELLOS)
    uint32          temp = 0u;
    Os_ArchCsaType* Csa1; /* PRQA S 3432 */ /* VL_Os_3432 */

    /*make sure current app is trusted or not*/
    if (TRUE != Os_AppCfg[Os_Kernel_GetSysRunningAppID()].Trusted)
    {
        /*get the address of the PCX point to*/
        Csa1 = OS_PCX_TO_EA((Os_ArchMsrType)OS_ARCH_MFCR(OS_REG_PCX)); /* PRQA S 0306 */ /* VL_Os_0306 */

        /*change csa PSW.IO bits to User-1 Mode*/ /* PRQA S 1252 ++ */ /* VL_Os_1252 */
        Csa1->reg[1] &= ARCH_PSW_IO_MASK; /* PRQA S 1252 */            /* VL_Os_1252 */
        Csa1->reg[1] = Csa1->reg[1] | ARCH_PSW_IO_USER1_BIT;

        /*change csa PSW.IO bits to User-1 Mode*/
        temp = (Os_ArchMsrType)OS_ARCH_MFCR(OS_REG_PSW);
        temp = temp & ARCH_PSW_IO_MASK;
        temp = temp | ARCH_PSW_IO_USER1_BIT;
        /* PRQA S 1252 -- */ /* VL_Os_1252 */

        OS_ARCH_MTCR(OS_REG_PSW, temp) /* PRQA S 1006 */ /* VL_Os_1006 */
    }
#endif /* TRUE == CFG_SRV_SHELLOS */

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Set the memory protection properties>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <MpSetIdx>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <Os_Hal_InitKnMemMap and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_ArchSetMemProtSet(uint32 MpSetIdx) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    Os_PSW_PRS = MpSetIdx << ARCH_PSW_PRS_BIT;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Gets the memory protection property>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <Os_McuSetIsrMemMap,Os_McuSetTaskMemMap>
 * REQ ID               <None>
 */
/******************************************************************************/
uint32 Os_ArchGetMemProtSet(void) /* PRQA S 1532, 1503 */ /* VL_QAC_OneFunRef */ /* VL_Os_1503 */
{
    return Os_PSW_PRS;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */

/*=======[E N D   O F   F I L E]==============================================*/
/* PRQA S 0553, 6610 EOF */ /* VL_QAC_UnUsedFiles, VL_MTR_Os_STCDN */
