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
 **  @file               : Os_Peripheral.c
 **  @author             : i-soft-os
 **  @date               : 2025/02/10
 **  @vendor             : isoft
 **  @description        : Os source file for Peripheral API implementations
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Os_Arch_Processor.h"
#include "Os_Peripheral.h"
#include "Os_Appl.h"
#include "Os_Hook.h"
#include "Os_Kernel.h"
#include "Os_Sprot.h"
#include "Os_Err.h"
#include "Os_Rti.h"
#include "Os_Arti.h"

#if (CFG_PERIPHERAL_MAX > 0U)

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ============================================ external data definitions =========================================== */

/* ============================================ internal data definitions =========================================== */

/* ========================================== internal function declarations ======================================== */
/**
 * @brief           Validates peripheral access parameters
 * @param[in]       area: ID of the peripheral area to access
 * @param[in]       address: Address within the peripheral area to access
 * @param[in]       pScb: Pointer to the System Control Block
 * @return          StatusType
 * @retval          E_OK: Parameters are valid and accessible
 * @retval          E_OS_ID: Invalid area ID
 * @retval          E_OS_VALUE: Address is outside the valid range for the area
 * @retval          E_OS_ACCESS: Application doesn't have access rights to the area
 * @retval          E_OS_CALLEVEL: Called from an incorrect context
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_CheckPeripheralParameters(
    AreaIdType area,
    uint32 address,
    const Os_SCBType *pScb);

/* ========================================== external function definitions ========================================= */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * ReadPeripheral8
 */
/* PRQA S 6030 ++ */ /* VL_MTR_Os_STMIF */
OS_LOCAL StatusType Os_CheckPeripheralParameters(
    AreaIdType area,
    uint32 address,
    const Os_SCBType *pScb)
/* PRQA S 6030 -- */
{
    StatusType status;
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    Os_ApplicationType runAppId = pScb->SysRunningAppId;
#endif

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (area >= CFG_PERIPHERAL_MAX)
    {
        status = E_OS_ID;
    }
    else if ((address < Os_PeriAreaCfg[area].PeriAreaStartAddr) || (address > Os_PeriAreaCfg[area].PeriAreaEndAddr))
    {
        status = E_OS_VALUE;
    }
    else
#endif

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        /*the current application have access rights to the object*/
        /* PRQA S 4397, 1258, 1840, 0582 ++ */ /* VL_Os_4397, VL_Os_1258, VL_Os_1840, VL_Os_0582 */
        if (0U == ((Os_PeriAreaCfg[area].AccAppRef[OS_APPGETACCESS_GP(runAppId)]) & OS_APPACCESS_MASK(runAppId)))
        /* PRQA S 4397, 1258, 1840, 0582 -- */
        {
            status = E_OS_ACCESS;
        }
        else if (TRUE != Os_WrongContext(pScb, OS_CONTEXT_OPEARTION_PERIPHERAL))
        {
            status = E_OS_CALLEVEL;
        }
        else if (OS_APPLICATION_ACCESSIBLE != Os_GetAppStateInternal(runAppId))
        {
            status = E_OS_ACCESS;
        }
        else
#endif
        {
            status = E_OK;
        }

    UNUSED_PARAMETER(address);
    UNUSED_PARAMETER(area);
    UNUSED_PARAMETER(pScb);
    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * ReadPeripheral8
 */
/* PRQA S 1503, 3408, 3006, 6070, 1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3408, VL_Os_3006, VL_MTR_Os_STCAL, VL_Os_1512 */
StatusType ReadPeripheral8(
    AreaIdType Area,
    const uint8 *Address,
    uint8 *ReadValue)
/* PRQA S 1503, 3408, 3006, 6070, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    StatusType status = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 1259, 3432, 4442, 4521 ++ */ /* VL_Os_1317, VL_Os_1259, VL_Os_3432, VL_Os_4442, VL_Os_4521 */
    OSRtiEnterApi(pScb, OSApiId_ReadPeripheral8);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ReadPeripheral8_Start, 0);
    /* PRQA S 1317, 1259, 3432, 4442, 4521 -- */
    /* PRQA S 3138, 3141 -- */
#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (NULL_PTR == ReadValue)
    {
        status = E_OS_ILLEGAL_ADDRESS;
    }
    else
#endif
    {
        uint32 cmpAddr = (uint32)Address; /* PRQA S 0306 */ /* VL_Os_0306 */
        status = Os_CheckPeripheralParameters(Area, cmpAddr, pScb);
    }

    if (E_OK == status) /* PRQA S 2991, 2995 */ /* VL_Os_2991, VL_Os_2995 */
    {
        OS_HAL_DECLARE_CRITICAL();
        OS_HAL_ENTRY_CRITICAL();
        *ReadValue = *Address;
        OS_HAL_EXIT_CRITICAL();
    }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK) /* PRQA S 2992, 2996 */ /* VL_Os_2992, VL_Os_2996 */
    {
        Os_TraceErrorHook(OSError_Save_ReadPeripheral8(Area, (uint8 *)Address, ReadValue), /* PRQA S 0311 */ /* VL_Os_0311 */
                          OSServiceId_ReadPeripheral8,
                          status, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544, 1259 ++ */ /* VL_Os_3432, VL_Os_4544, VL_Os_1259 */
    OSRtiExitApi(pScb, OSApiId_ReadPeripheral8);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ReadPeripheral8_Return, 0);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544, 1259 -- */
    /* PRQA S 3138, 3141 -- */

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * ReadPeripheral16
 */
/* PRQA S 1503, 3408, 3006, 6070, 4152, 1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3408, VL_Os_3006, VL_MTR_Os_STCAL, VL_Os_4152, VL_Os_1512 */
StatusType ReadPeripheral16(
    AreaIdType Area,
    const uint16 *Address,
    uint16 *ReadValue)
/* PRQA S 1503, 3408, 3006, 6070, 4152, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    StatusType status = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 1259, 3432, 4442, 4521 ++ */ /* VL_Os_1317, VL_Os_1259, VL_Os_3432, VL_Os_4442, VL_Os_4521 */
    OSRtiEnterApi(pScb, OSApiId_ReadPeripheral16);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ReadPeripheral16_Start, 0);
    /* PRQA S 1317, 1259, 3432, 4442, 4521 -- */
    /* PRQA S 3138, 3141 -- */
#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (NULL_PTR == ReadValue)
    {
        status = E_OS_ILLEGAL_ADDRESS;
    }
    else
#endif
    {
        uint32 cmpAddr = (uint32)Address; /* PRQA S 0306 */ /* VL_Os_0306 */
        status = Os_CheckPeripheralParameters(Area, cmpAddr, pScb); 
    }

    if (E_OK == status) /* PRQA S 2991, 2995 */ /* VL_Os_2991, VL_Os_2995 */
    {
        OS_HAL_DECLARE_CRITICAL();
        OS_HAL_ENTRY_CRITICAL();
        *ReadValue = *Address;
        OS_HAL_EXIT_CRITICAL();
    }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK) /* PRQA S 2992, 2996 */ /* VL_Os_2992, VL_Os_2996 */
    {
        Os_TraceErrorHook(OSError_Save_ReadPeripheral16(Area, (uint16 *)Address, ReadValue), /* PRQA S 0311 */ /* VL_Os_0311 */
                          OSServiceId_ReadPeripheral16,
                          status, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544, 1259 ++ */ /* VL_Os_3432, VL_Os_4544, VL_Os_1259 */
    OSRtiExitApi(pScb, OSApiId_ReadPeripheral16);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ReadPeripheral16_Return, 0);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544, 1259 -- */
    /* PRQA S 3138, 3141 -- */

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * ReadPeripheral32
 */
/* PRQA S 1503, 3408, 3006, 6070, 1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3408, VL_Os_3006, VL_MTR_Os_STCAL, VL_Os_1512 */
StatusType ReadPeripheral32(
    AreaIdType Area,
    const uint32 *Address,
    uint32 *ReadValue)
/* PRQA S 1503, 3408, 3006, 6070, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    StatusType status = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 1259, 3432, 4442, 4521 ++ */ /* VL_Os_1317, VL_Os_1259, VL_Os_3432, VL_Os_4442, VL_Os_4521 */
    OSRtiEnterApi(pScb, OSApiId_ReadPeripheral32);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ReadPeripheral32_Start, 0);
    /* PRQA S 1317, 1259, 3432, 4442, 4521 -- */
    /* PRQA S 3138, 3141 -- */
#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (NULL_PTR == ReadValue)
    {
        status = E_OS_ILLEGAL_ADDRESS;
    }
    else
#endif
    {
        uint32 cmpAddr = (uint32)Address; /* PRQA S 0306 */ /* VL_Os_0306 */
        status = Os_CheckPeripheralParameters(Area, cmpAddr, pScb);     
    }

    if (E_OK == status) /* PRQA S 2991, 2995 */ /* VL_Os_2991, VL_Os_2995 */
    {
        OS_HAL_DECLARE_CRITICAL();
        OS_HAL_ENTRY_CRITICAL();
        *ReadValue = *Address; 
        OS_HAL_EXIT_CRITICAL();
    }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK) /* PRQA S 2992, 2996 */ /* VL_Os_2992, VL_Os_2996 */
    {
        Os_TraceErrorHook(OSError_Save_ReadPeripheral32(Area, (uint32 *)Address, ReadValue), /* PRQA S 0311 */ /* VL_Os_0311 */
                          OSServiceId_ReadPeripheral32,
                          status, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544, 1259 ++ */ /* VL_Os_3432, VL_Os_4544, VL_Os_1259 */
    OSRtiExitApi(pScb, OSApiId_ReadPeripheral32);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ReadPeripheral32_Return, 0);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544, 1259 -- */
    /* PRQA S 3138, 3141 -- */

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * WritePeripheral8
 */
/* PRQA S 1503, 3408, 3006, 6070, 1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3408, VL_Os_3006, VL_MTR_Os_STCAL, VL_Os_1512 */
StatusType WritePeripheral8(
    AreaIdType Area,
    uint8 *Address,
    uint8 WriteValue)
/* PRQA S 1503, 3408, 3006, 6070, 1512 -- */
{   
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    StatusType status = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 1259, 3432, 4442, 4521 ++ */ /* VL_Os_1317, VL_Os_1259, VL_Os_3432, VL_Os_4442, VL_Os_4521 */
    OSRtiEnterApi(pScb, OSApiId_WritePeripheral8);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_WritePeripheral8_Start, 0);
    /* PRQA S 1317, 1259, 3432, 4442, 4521 -- */
    /* PRQA S 3138, 3141 -- */

    uint32 cmpAddr = (uint32)Address; /* PRQA S 0306 */ /* VL_Os_0306 */
    status = Os_CheckPeripheralParameters(Area, cmpAddr, pScb);

    if (E_OK == status) /* PRQA S 2991, 2995 */ /* VL_Os_2991, VL_Os_2995 */
    {
        OS_HAL_DECLARE_CRITICAL();
        OS_HAL_ENTRY_CRITICAL();
        *Address = WriteValue;
        OS_HAL_EXIT_CRITICAL();
    }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK) /* PRQA S 2992, 2996 */ /* VL_Os_2992, VL_Os_2996 */
    {
        Os_TraceErrorHook(OSError_Save_WritePeripheral8(Area, Address, WriteValue),
                          OSServiceId_WritePeripheral8,
                          status, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_WritePeripheral8);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_WritePeripheral8_Return, status);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * WritePeripheral16
 */
/* PRQA S 1503, 3408, 3006, 6070, 4152, 1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3408, VL_Os_3006, VL_MTR_Os_STCAL, VL_Os_4152, VL_Os_1512 */
StatusType WritePeripheral16(
    AreaIdType Area,
    uint16 *Address,
    uint16 WriteValue)
/* PRQA S 1503, 3408, 3006, 6070, 4152, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    StatusType status = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 1259, 3432, 4442, 4521 ++ */ /* VL_Os_1317, VL_Os_1259, VL_Os_3432, VL_Os_4442, VL_Os_4521 */
    OSRtiEnterApi(pScb, OSApiId_WritePeripheral16);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_WritePeripheral16_Start, 0);
    /* PRQA S 1317, 1259, 3432, 4442, 4521 -- */
    /* PRQA S 3138, 3141 -- */

    uint32 cmpAddr = (uint32)Address; /* PRQA S 0306 */ /* VL_Os_0306 */
    status = Os_CheckPeripheralParameters(Area, cmpAddr, pScb);

    if (E_OK == status) /* PRQA S 2991, 2995 */ /* VL_Os_2991, VL_Os_2995 */
    {
        OS_HAL_DECLARE_CRITICAL();
        OS_HAL_ENTRY_CRITICAL();
        *Address = WriteValue;
        OS_HAL_EXIT_CRITICAL();
    }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK) /* PRQA S 2992, 2996 */ /* VL_Os_2992, VL_Os_2996 */
    {
        Os_TraceErrorHook(OSError_Save_WritePeripheral16(Area, Address, WriteValue),
                          OSServiceId_WritePeripheral16,
                          status, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_WritePeripheral16);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_WritePeripheral16_Return, status);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * WritePeripheral32
 */
/* PRQA S 1503, 3408, 3006, 6070, 1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3408, VL_Os_3006, VL_MTR_Os_STCAL, VL_Os_1512 */
StatusType WritePeripheral32(
    AreaIdType Area,
    uint32 *Address,
    uint32 WriteValue)
/* PRQA S 1503, 3408, 3006, 6070, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    StatusType status = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 1259, 3432, 4442, 4521 ++ */ /* VL_Os_1317, VL_Os_1259, VL_Os_3432, VL_Os_4442, VL_Os_4521 */
    OSRtiEnterApi(pScb, OSApiId_WritePeripheral32);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_WritePeripheral32_Start, 0);
    /* PRQA S 1317, 1259, 3432, 4442, 4521 -- */
    /* PRQA S 3138, 3141 -- */

    uint32 cmpAddr = (uint32)Address; /* PRQA S 0306 */ /* VL_Os_0306 */
    status = Os_CheckPeripheralParameters(Area, cmpAddr, pScb);

    if (E_OK == status) /* PRQA S 2991, 2995 */ /* VL_Os_2991, VL_Os_2995 */
    {
        OS_HAL_DECLARE_CRITICAL();
        OS_HAL_ENTRY_CRITICAL();
        *Address = WriteValue;
        OS_HAL_EXIT_CRITICAL();
    }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK) /* PRQA S 2992, 2996 */ /* VL_Os_2992, VL_Os_2996 */
    {
        Os_TraceErrorHook(OSError_Save_WritePeripheral32(Area, Address, WriteValue),
                          OSServiceId_WritePeripheral32,
                          status, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_WritePeripheral32);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_WritePeripheral32_Return, status);
    OS_HAL_EXIT_KERNEL();/* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * ModifyPeripheral8
 */
/* PRQA S 1503, 3408, 3006, 6070, 1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3408, VL_Os_3006, VL_MTR_Os_STCAL, VL_Os_1512 */
StatusType ModifyPeripheral8(
    AreaIdType Area,
    uint8 *Address,
    uint8 Clearmask,
    uint8 Setmask)
/* PRQA S 1503, 3408, 3006, 6070, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    StatusType status = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 1259, 3432, 4442, 4521 ++ */ /* VL_Os_1317, VL_Os_1259, VL_Os_3432, VL_Os_4442, VL_Os_4521 */
    OSRtiEnterApi(pScb, OSApiId_ModifyPeripheral8);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ModifyPeripheral8_Start, 0);
    /* PRQA S 1317, 1259, 3432, 4442, 4521 -- */
    /* PRQA S 3138, 3141 -- */

    uint32 cmpAddr = (uint32)Address; /* PRQA S 0306 */ /* VL_Os_0306 */
    status = Os_CheckPeripheralParameters(Area, cmpAddr, pScb);

    if (E_OK == status) /* PRQA S 2991, 2995 */ /* VL_Os_2991, VL_Os_2995 */
    {
        OS_HAL_DECLARE_CRITICAL();

        OS_HAL_ENTRY_CRITICAL();
        /* modify the value to address. */
        *Address = ((*Address) & Clearmask) | Setmask;
        OS_HAL_EXIT_CRITICAL();
    }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK) /* PRQA S 2992, 2996 */ /* VL_Os_2992, VL_Os_2996 */
    {
        Os_TraceErrorHook(OSError_Save_ModifyPeripheral8(Area, Address, Clearmask, Setmask),
                          OSServiceId_ModifyPeripheral8,
                          status, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_ModifyPeripheral8);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ModifyPeripheral8_Return, status);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * ModifyPeripheral16
 */
/* PRQA S 1503, 3408, 3006, 6070, 4152, 1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3408, VL_Os_3006, VL_MTR_Os_STCAL, VL_Os_4152, VL_Os_1512 */
StatusType ModifyPeripheral16(
    AreaIdType Area,
    uint16 *Address,
    uint16 Clearmask,
    uint16 Setmask)
/* PRQA S 1503, 3408, 3006, 6070, 4152, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    StatusType status = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 1259, 3432, 4442, 4521 ++ */ /* VL_Os_1317, VL_Os_1259, VL_Os_3432, VL_Os_4442, VL_Os_4521 */
    OSRtiEnterApi(pScb, OSApiId_ModifyPeripheral16);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ModifyPeripheral16_Start, 0);
    /* PRQA S 1317, 1259, 3432, 4442, 4521 -- */
    /* PRQA S 3138, 3141 -- */

    uint32 cmpAddr = (uint32)Address; /* PRQA S 0306 */ /* VL_Os_0306 */
    status = Os_CheckPeripheralParameters(Area, cmpAddr, pScb);

    if (E_OK == status) /* PRQA S 2991, 2995 */ /* VL_Os_2991, VL_Os_2995 */
    {
        OS_HAL_DECLARE_CRITICAL();
        OS_HAL_ENTRY_CRITICAL();
        /*modify the value to address*/
        *Address = ((*Address) & Clearmask) | Setmask;
        OS_HAL_EXIT_CRITICAL();
    }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK) /* PRQA S 2992, 2996 */ /* VL_Os_2992, VL_Os_2996 */
    {
        Os_TraceErrorHook(OSError_Save_ModifyPeripheral16(Area, Address, Clearmask, Setmask),
                          OSServiceId_ModifyPeripheral16,
                          status, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_ModifyPeripheral16);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ModifyPeripheral16_Return, status);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * ModifyPeripheral32
 */
/* PRQA S 1503, 3408, 3006, 6070, 1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3408, VL_Os_3006, VL_MTR_Os_STCAL, VL_Os_1512 */
StatusType ModifyPeripheral32(
    AreaIdType Area,
    uint32 *Address,
    uint32 Clearmask,
    uint32 Setmask)
/* PRQA S 1503, 3408, 3006, 6070, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    StatusType status = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 1259, 3432, 4442, 4521 ++ */ /* VL_Os_1317, VL_Os_1259, VL_Os_3432, VL_Os_4442, VL_Os_4521 */
    OSRtiEnterApi(pScb, OSApiId_ModifyPeripheral32);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ModifyPeripheral32_Start, 0);
    /* PRQA S 1317, 1259, 3432, 4442, 4521 -- */
    /* PRQA S 3138, 3141 -- */

    uint32 cmpAddr = (uint32)Address; /* PRQA S 0306 */ /* VL_Os_0306 */
    status = Os_CheckPeripheralParameters(Area, cmpAddr, pScb);

    if (E_OK == status) /* PRQA S 2991, 2995 */ /* VL_Os_2991, VL_Os_2995 */
    {
        OS_HAL_DECLARE_CRITICAL();
        OS_HAL_ENTRY_CRITICAL();
        /*modify the value to address*/
        *Address = ((*Address) & Clearmask) | Setmask;
        OS_HAL_EXIT_CRITICAL();
    }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK) /* PRQA S 2992, 2996 */ /* VL_Os_2992, VL_Os_2996 */
    {
        Os_TraceErrorHook(OSError_Save_ModifyPeripheral32(Area, Address, Clearmask, Setmask),
                          OSServiceId_ModifyPeripheral32,
                          status, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_ModifyPeripheral32);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ModifyPeripheral32_Return, status);
    OS_HAL_EXIT_KERNEL();/* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /*(CFG_PERIPHERAL_MAX > 0U)*/ /* PRQA S 2053 */ /* VL_Os_2053 */

/*=======[E N D   O F   F I L E]==============================================*/
/* PRQA S 0553 EOF */ /* VL_QAC_UnUsedFiles */
