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
 **  FILENAME    : Os_Peripheral.c                                             **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      :  i-soft-os                                                  **
 **  Vendor      :                                                             **
 **  DESCRIPTION : Peripheral manager                                          **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
 **  Version :   AUTOSAR classic Platform R19--Function Safety                 **
 **                                                                            **
 *******************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Os_Internal.h"
#if (CFG_PERIPHERAL_MAX > 0U)

/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <ReadPeripheral8>
 * Service ID           <0x28>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3006, 6030, 6010, 6070 ++ */ /* VL_Os_3006, VL_MTR_Os_STMIF, VL_MTR_Os_STCYC, VL_MTR_Os_STCAL */
StatusType ReadPeripheral8(AreaIdType Area, const uint8* Address, uint8* ReadValue)
/* PRQA S 3006, 6030, 6010, 6070 -- */
{
    /* PRQA S 2741, 2742, 2880, 3138, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2741, 2742, 2880, 3138, 3141 -- */

    StatusType status = E_OK;

#if (OS_STATUS_EXTENDED == CFG_STATUS)

    uint32 cmpAddr = (uint32)Address;/* PRQA S 0306 */ /* VL_Os_0306 */

    if (Area >= CFG_PERIPHERAL_MAX)
    {
        status = E_OS_ID;
    }
    else if (NULL_PTR == ReadValue)
    {
        status = E_OS_PARAM_POINTER;
    }
    else if ((cmpAddr < Os_PeriAreaCfg[Area].periAreaStartAddr) || (cmpAddr > Os_PeriAreaCfg[Area].periAreaEndAddr))
    {
        status = E_OS_VALUE;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        /*the current application have access rights to the object*/
        /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral */
        if (0U == ((Os_PeriAreaCfg[Area].periAreaAppAccMask) & ((uint64)1u << Os_SCB.sysRunningAppID)))
        /* PRQA S 1258 -- */
        {
            status = E_OS_ACCESS;
        }
        else if (Os_WrongContext(OS_CONTEXT_OPEARTION_PERIPHERAL) != TRUE)
        {
            status = E_OS_CALLEVEL;
        }
        /* PRQA S 3442 ++ */ /* VL_Os_3442 */
        else if (APPLICATION_ACCESSIBLE != Os_AppCB[Os_SCB.sysRunningAppID].appState)
        /* PRQA S 3442 -- */
        {
            status = E_OS_ACCESS;
        }
        else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
        {
            OS_ARCH_DECLARE_CRITICAL();
            OS_ARCH_ENTRY_CRITICAL();
            /* PRQA S 3120,2985 ++ */ /* VL_QAC_MagicNum, VL_Os_2985 */
            *ReadValue = *Address & 0xFFU;
            /* PRQA S 3120,2985 -- */
            OS_ARCH_EXIT_CRITICAL();
        }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK)
    {
        /* PRQA S 3138, 3141, 0311 ++ */ /* VL_Os_3138, VL_Os_3141, VL_Os_0311 */
        Os_TraceErrorHook(
            OSError_Save_ReadPeripheral8(Area, (uint8*)Address, 0u),
            OSServiceId_ReadPeripheral8,
            status);
        /* PRQA S 3138, 3141, 0311 -- */
    }
#endif

    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <ReadPeripheral16>
 * Service ID           <0x29>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3006, 4152, 6030, 6010, 6070 ++ */ /* VL_Os_3006, VL_Os_4152, VL_MTR_Os_STMIF, VL_MTR_Os_STCYC, VL_MTR_Os_STCAL */
StatusType ReadPeripheral16(AreaIdType Area, const uint16* Address, uint16* ReadValue)
/* PRQA S 3006, 4152, 6030, 6010, 6070 -- */
{
    /* PRQA S 2741, 2742, 2880, 3138, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2741, 2742, 2880, 3138, 3141 -- */

    StatusType status = E_OK;

#if (OS_STATUS_EXTENDED == CFG_STATUS)

    uint32 cmpAddr = (uint32)Address;/* PRQA S 0306 */ /* VL_Os_0306 */

    if (Area >= CFG_PERIPHERAL_MAX)
    {
        status = E_OS_ID;
    }
    else if (NULL_PTR == ReadValue)
    {
        status = E_OS_PARAM_POINTER;
    }
    else if ((cmpAddr < Os_PeriAreaCfg[Area].periAreaStartAddr) || (cmpAddr > Os_PeriAreaCfg[Area].periAreaEndAddr))
    {
        status = E_OS_VALUE;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        /*the current application have access rights to the object*/
        /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral */
        if (0U == ((Os_PeriAreaCfg[Area].periAreaAppAccMask) & ((uint64)1u << Os_SCB.sysRunningAppID)))
        /* PRQA S 1258 -- */
        {
            status = E_OS_ACCESS;
        }
        else if (TRUE != Os_WrongContext(OS_CONTEXT_OPEARTION_PERIPHERAL))
        {
            status = E_OS_CALLEVEL;
        }
        /* PRQA S 3442 ++ */ /* VL_Os_3442 */
        else if (APPLICATION_ACCESSIBLE != Os_AppCB[Os_SCB.sysRunningAppID].appState)
        /* PRQA S 3442 -- */
        {
            status = E_OS_ACCESS;
        }
        else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
        {
            OS_ARCH_DECLARE_CRITICAL();

            OS_ARCH_ENTRY_CRITICAL();

            *ReadValue = *Address;

            OS_ARCH_EXIT_CRITICAL();
        }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK)
    {
        /* PRQA S 3138, 3141, 0311 ++ */ /* VL_Os_3138, VL_Os_3141, VL_Os_0311 */
        Os_TraceErrorHook(
            OSError_Save_ReadPeripheral16(Area, (uint16*)Address, 0u),
            OSServiceId_ReadPeripheral16,
            status);
        /* PRQA S 3138, 3141, 0311 -- */
    }
#endif

    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <ReadPeripheral32>
 * Service ID           <0x2a>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3006, 6030, 6010, 6070 ++ */ /* VL_Os_3006, VL_MTR_Os_STMIF, VL_MTR_Os_STCYC, VL_MTR_Os_STCAL */
StatusType ReadPeripheral32(AreaIdType Area, const uint32* Address, uint32* ReadValue)
/* PRQA S 3006, 6030, 6010, 6070 -- */
{
    /* PRQA S 2741, 2742, 2880, 3138, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2741, 2742, 2880, 3138, 3141 -- */

    StatusType status = E_OK;

#if (OS_STATUS_EXTENDED == CFG_STATUS)

    uint32 cmpAddr = (uint32)Address;/* PRQA S 0306 */ /* VL_Os_0306 */

    if (Area >= CFG_PERIPHERAL_MAX)
    {
        status = E_OS_ID;
    }
    else if (NULL_PTR == ReadValue)
    {
        status = E_OS_PARAM_POINTER;
    }
    else if ((cmpAddr < Os_PeriAreaCfg[Area].periAreaStartAddr) || (cmpAddr > Os_PeriAreaCfg[Area].periAreaEndAddr))
    {
        status = E_OS_VALUE;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        /*the current application have access rights to the object*/
        /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral */
        if (0U == ((Os_PeriAreaCfg[Area].periAreaAppAccMask) & ((uint64)1u << Os_SCB.sysRunningAppID)))
        /* PRQA S 1258 -- */
        {
            status = E_OS_ACCESS;
        }
        else if (TRUE != Os_WrongContext(OS_CONTEXT_OPEARTION_PERIPHERAL))
        {
            status = E_OS_CALLEVEL;
        }
        /* PRQA S 3442 ++ */ /* VL_Os_3442 */
        else if (APPLICATION_ACCESSIBLE != Os_AppCB[Os_SCB.sysRunningAppID].appState)
        /* PRQA S 3442 -- */
        {
            status = E_OS_ACCESS;
        }
        else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
        {
            OS_ARCH_DECLARE_CRITICAL();

            OS_ARCH_ENTRY_CRITICAL();

            *ReadValue = *Address;

            OS_ARCH_EXIT_CRITICAL();
        }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK)
    {
        /* PRQA S 3138, 3141, 0311 ++ */ /* VL_Os_3138, VL_Os_3141, VL_Os_0311 */
        Os_TraceErrorHook(
            OSError_Save_ReadPeripheral32(Area, (uint32*)Address, 0u),
            OSServiceId_ReadPeripheral32,
            status);
        /* PRQA S 3138, 3141, 0311 -- */
    }
#endif

    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <WritePeripheral8>
 * Service ID           <0x2b>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3006, 6030, 6010, 6070 ++ */ /* VL_Os_3006, VL_MTR_Os_STMIF, VL_MTR_Os_STCYC, VL_MTR_Os_STCAL */
StatusType WritePeripheral8(AreaIdType Area, uint8* Address, uint8 WriteValue)
/* PRQA S 3006, 6030, 6010, 6070 -- */
{
    /* PRQA S 2741, 2742, 2880, 3138, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2741, 2742, 2880, 3138, 3141 -- */

    StatusType status = E_OK;

#if (OS_STATUS_EXTENDED == CFG_STATUS)

    uint32 cmpAddr = (uint32)Address;/* PRQA S 0306 */ /* VL_Os_0306 */

    if (Area >= CFG_PERIPHERAL_MAX)
    {
        status = E_OS_ID;
    }
    else if ((cmpAddr < Os_PeriAreaCfg[Area].periAreaStartAddr) || (cmpAddr > Os_PeriAreaCfg[Area].periAreaEndAddr))
    {
        status = E_OS_VALUE;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        /*the current application have access rights to the object*/
        /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral */
        if (0U == ((Os_PeriAreaCfg[Area].periAreaAppAccMask) & ((uint64)1u << Os_SCB.sysRunningAppID)))
        /* PRQA S 1258 -- */
        {
            status = E_OS_ACCESS;
        }
        else if (TRUE != Os_WrongContext(OS_CONTEXT_OPEARTION_PERIPHERAL))
        {
            status = E_OS_CALLEVEL;
        }
        /* PRQA S 3442 ++ */ /* VL_Os_3442 */
        else if (APPLICATION_ACCESSIBLE != Os_AppCB[Os_SCB.sysRunningAppID].appState)
        /* PRQA S 3442 -- */
        {
            status = E_OS_ACCESS;
        }
        else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
        {
            OS_ARCH_DECLARE_CRITICAL();

            OS_ARCH_ENTRY_CRITICAL();
            /* PRQA S 3120,2985 ++ */ /* VL_QAC_MagicNum, VL_Os_2985 */
            *Address = (WriteValue & 0xFFU);
            /* PRQA S 3120,2985 -- */
            OS_ARCH_EXIT_CRITICAL();
        }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK)
    {
        /* PRQA S 3138, 3141 ++ */ /* VL_Os_3138, VL_Os_3141 */
        Os_TraceErrorHook(
            OSError_Save_WritePeripheral8(Area, Address, WriteValue),
            OSServiceId_WritePeripheral8,
            status);
        /* PRQA S 3138, 3141 -- */
    }
#endif

    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <WritePeripheral16>
 * Service ID           <0x2c>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3006, 4152, 6030, 6010, 6070 ++ */ /* VL_Os_3006, VL_Os_4152, VL_MTR_Os_STMIF, VL_MTR_Os_STCYC, VL_MTR_Os_STCAL */
StatusType WritePeripheral16(AreaIdType Area, uint16* Address, uint16 WriteValue)
/* PRQA S 3006, 4152, 6030, 6010, 6070 -- */
{
    /* PRQA S 2741, 2742, 2880, 3138, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2741, 2742, 2880, 3138, 3141 -- */

    StatusType status = E_OK;

#if (OS_STATUS_EXTENDED == CFG_STATUS)

    uint32 cmpAddr = (uint32)Address;/* PRQA S 0306 */ /* VL_Os_0306 */

    if (Area >= CFG_PERIPHERAL_MAX)
    {
        status = E_OS_ID;
    }
    else if ((cmpAddr < Os_PeriAreaCfg[Area].periAreaStartAddr) || (cmpAddr > Os_PeriAreaCfg[Area].periAreaEndAddr))
    {
        status = E_OS_VALUE;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        /*the current application have access rights to the object*/
        /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral */
        if (0U == ((Os_PeriAreaCfg[Area].periAreaAppAccMask) & ((uint64)1u << Os_SCB.sysRunningAppID)))
        /* PRQA S 1258 -- */
        {
            status = E_OS_ACCESS;
        }
        else if (TRUE != Os_WrongContext(OS_CONTEXT_OPEARTION_PERIPHERAL))
        {
            status = E_OS_CALLEVEL;
        }
        /* PRQA S 3442 ++ */ /* VL_Os_3442 */
        else if (APPLICATION_ACCESSIBLE != Os_AppCB[Os_SCB.sysRunningAppID].appState)
        /* PRQA S 3442 -- */
        {
            status = E_OS_ACCESS;
        }
        else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
        {
            OS_ARCH_DECLARE_CRITICAL();

            OS_ARCH_ENTRY_CRITICAL();

            *Address = WriteValue;

            OS_ARCH_EXIT_CRITICAL();
        }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK)
    {
        /* PRQA S 3138, 3141 ++ */ /* VL_Os_3138, VL_Os_3141 */
        Os_TraceErrorHook(
            OSError_Save_WritePeripheral16(Area, Address, WriteValue),
            OSServiceId_WritePeripheral16,
            status);
        /* PRQA S 3138, 3141 -- */
    }
#endif

    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <WritePeripheral32>
 * Service ID           <0x2d>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3006, 6030, 6010, 6070 ++ */ /* VL_Os_3006, VL_MTR_Os_STMIF, VL_MTR_Os_STCYC, VL_MTR_Os_STCAL */
StatusType WritePeripheral32(AreaIdType Area, uint32* Address, uint32 WriteValue)
/* PRQA S 3006, 6030, 6010, 6070 -- */
{
    /* PRQA S 2741, 2742, 2880, 3138, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2741, 2742, 2880, 3138, 3141 -- */

    StatusType status = E_OK;

#if (OS_STATUS_EXTENDED == CFG_STATUS)

    uint32 cmpAddr = (uint32)Address; /* PRQA S 0306 */ /* VL_Os_0306 */

    if (Area >= CFG_PERIPHERAL_MAX)
    {
        status = E_OS_ID;
    }
    else if ((cmpAddr < Os_PeriAreaCfg[Area].periAreaStartAddr) || (cmpAddr > Os_PeriAreaCfg[Area].periAreaEndAddr))
    {
        status = E_OS_VALUE;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        /*the current application have access rights to the object*/
        /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral */
        if (0U == ((Os_PeriAreaCfg[Area].periAreaAppAccMask) & ((uint64)1u << Os_SCB.sysRunningAppID)))
        /* PRQA S 1258 -- */
        {
            status = E_OS_ACCESS;
        }
        else if (TRUE != Os_WrongContext(OS_CONTEXT_OPEARTION_PERIPHERAL))
        {
            status = E_OS_CALLEVEL;
        }
        /* PRQA S 3442 ++ */ /* VL_Os_3442 */
        else if (APPLICATION_ACCESSIBLE != Os_AppCB[Os_SCB.sysRunningAppID].appState)
        /* PRQA S 3442 -- */
        {
            status = E_OS_ACCESS;
        }
        else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
        {
            OS_ARCH_DECLARE_CRITICAL();

            OS_ARCH_ENTRY_CRITICAL();

            *Address = WriteValue;

            OS_ARCH_EXIT_CRITICAL();
        }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK)
    {
        /* PRQA S 3138, 3141 ++ */ /* VL_Os_3138, VL_Os_3141 */
        Os_TraceErrorHook(
            OSError_Save_WritePeripheral32(Area, Address, WriteValue),
            OSServiceId_WritePeripheral32,
            status);
        /* PRQA S 3138, 3141 -- */
    }
#endif

    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <ModifyPeripheral8>
 * Service ID           <0x2e>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3006, 6030, 6010, 6070 ++ */ /* VL_Os_3006, VL_MTR_Os_STMIF, VL_MTR_Os_STCYC, VL_MTR_Os_STCAL */
StatusType ModifyPeripheral8(AreaIdType Area, uint8* Address, uint8 Clearmask, uint8 Setmask)
/* PRQA S 3006, 6030, 6010, 6070 -- */
{
    /* PRQA S 2741, 2742, 2880, 3138, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2741, 2742, 2880, 3138, 3141 -- */

    StatusType status = E_OK;

#if (OS_STATUS_EXTENDED == CFG_STATUS)

    uint32 cmpAddr = (uint32)Address; /* PRQA S 0306 */ /* VL_Os_0306 */

    if (Area >= CFG_PERIPHERAL_MAX)
    {
        status = E_OS_ID;
    }
    else if ((cmpAddr < Os_PeriAreaCfg[Area].periAreaStartAddr) || (cmpAddr > Os_PeriAreaCfg[Area].periAreaEndAddr))
    {
        status = E_OS_VALUE;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        /*the current application have access rights to the object*/
        /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral */
        if (0U == ((Os_PeriAreaCfg[Area].periAreaAppAccMask) & ((uint64)1u << Os_SCB.sysRunningAppID)))
        /* PRQA S 1258 -- */
        {
            status = E_OS_ACCESS;
        }
        else if (TRUE != Os_WrongContext(OS_CONTEXT_OPEARTION_PERIPHERAL))
        {
            status = E_OS_CALLEVEL;
        }
        /* PRQA S 3442 ++ */ /* VL_Os_3442 */
        else if (APPLICATION_ACCESSIBLE != Os_AppCB[Os_SCB.sysRunningAppID].appState)
        /* PRQA S 3442 -- */
        {
            status = E_OS_ACCESS;
        }
        else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
        {
            OS_ARCH_DECLARE_CRITICAL();

            OS_ARCH_ENTRY_CRITICAL();

            /* modify the value to address. */
            /* PRQA S 3120,2985 ++ */ /* VL_QAC_MagicNum, VL_Os_2985 */
            *Address = (((*Address) & Clearmask) | Setmask) & 0xFFU;
            /* PRQA S 3120,2985 -- */

            OS_ARCH_EXIT_CRITICAL();
        }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK)
    {
        /* PRQA S 3138, 3141 ++ */ /* VL_Os_3138, VL_Os_3141 */
        Os_TraceErrorHook(
            OSError_Save_ModifyPeripheral8(Area, Address, Clearmask, Setmask),
            OSServiceId_ModifyPeripheral8,
            status);
        /* PRQA S 3138, 3141 -- */
    }
#endif

    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <ModifyPeripheral16>
 * Service ID           <0x35>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3006, 4152, 6030, 6010, 6070 ++ */ /* VL_Os_3006, VL_Os_4152, VL_MTR_Os_STMIF, VL_MTR_Os_STCYC, VL_MTR_Os_STCAL */
StatusType ModifyPeripheral16(AreaIdType Area, uint16* Address, uint16 Clearmask, uint16 Setmask)
/* PRQA S 3006, 4152, 6030, 6010, 6070 -- */
{
    /* PRQA S 2741, 2742, 2880, 3138, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2741, 2742, 2880, 3138, 3141 -- */

    StatusType status = E_OK;

#if (OS_STATUS_EXTENDED == CFG_STATUS)

    uint32 cmpAddr = (uint32)Address; /* PRQA S 0306 */ /* VL_Os_0306 */

    if (Area >= CFG_PERIPHERAL_MAX)
    {
        status = E_OS_ID;
    }
    else if ((cmpAddr < Os_PeriAreaCfg[Area].periAreaStartAddr) || (cmpAddr > Os_PeriAreaCfg[Area].periAreaEndAddr))
    {
        status = E_OS_VALUE;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        /*the current application have access rights to the object*/
        /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral */
        if (0U == ((Os_PeriAreaCfg[Area].periAreaAppAccMask) & ((uint64)1u << Os_SCB.sysRunningAppID)))
        /* PRQA S 1258 -- */
        {
            status = E_OS_ACCESS;
        }
        else if (TRUE != Os_WrongContext(OS_CONTEXT_OPEARTION_PERIPHERAL))
        {
            status = E_OS_CALLEVEL;
        }
        /* PRQA S 3442 ++ */ /* VL_Os_3442 */
        else if (APPLICATION_ACCESSIBLE != Os_AppCB[Os_SCB.sysRunningAppID].appState)
        /* PRQA S 3442 -- */
        {
            status = E_OS_ACCESS;
        }
        else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
        {
            OS_ARCH_DECLARE_CRITICAL();

            OS_ARCH_ENTRY_CRITICAL();

            /*modify the value to address*/
            *Address = ((*Address) & Clearmask) | Setmask;

            OS_ARCH_EXIT_CRITICAL();
        }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK)
    {
        /* PRQA S 3138, 3141 ++ */ /* VL_Os_3138, VL_Os_3141 */
        Os_TraceErrorHook(
            OSError_Save_ModifyPeripheral16(Area, Address, Clearmask, Setmask),
            OSServiceId_ModifyPeripheral16,
            status);
        /* PRQA S 3138, 3141 -- */
    }
#endif

    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <ModifyPeripheral32>
 * Service ID           <0x2f>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3006, 6030, 6010, 6070 ++ */ /* VL_Os_3006, VL_MTR_Os_STMIF, VL_MTR_Os_STCYC, VL_MTR_Os_STCAL */
StatusType ModifyPeripheral32(AreaIdType Area, uint32* Address, uint32 Clearmask, uint32 Setmask)
/* PRQA S 3006, 6030, 6010, 6070 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    StatusType status = E_OK;

#if (OS_STATUS_EXTENDED == CFG_STATUS)

    uint32 cmpAddr = (uint32)Address; /* PRQA S 0306 */ /* VL_Os_0306 */

    if (Area >= CFG_PERIPHERAL_MAX)
    {
        status = E_OS_ID;
    }
    else if ((cmpAddr < Os_PeriAreaCfg[Area].periAreaStartAddr) || (cmpAddr > Os_PeriAreaCfg[Area].periAreaEndAddr))
    {
        status = E_OS_VALUE;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        /*the current application have access rights to the object*/
        /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral */
        if (0U == ((Os_PeriAreaCfg[Area].periAreaAppAccMask) & ((uint64)1u << Os_SCB.sysRunningAppID)))
        /* PRQA S 1258 -- */
        {
            status = E_OS_ACCESS;
        }
        else if (TRUE != Os_WrongContext(OS_CONTEXT_OPEARTION_PERIPHERAL))
        {
            status = E_OS_CALLEVEL;
        }
        /* PRQA S 3442 ++ */ /* VL_Os_3442 */
        else if (APPLICATION_ACCESSIBLE != Os_AppCB[Os_SCB.sysRunningAppID].appState)
        /* PRQA S 3442 -- */
        {
            status = E_OS_ACCESS;
        }
        else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
        {
            OS_ARCH_DECLARE_CRITICAL();

            OS_ARCH_ENTRY_CRITICAL();

            /*modify the value to address*/
            *Address = ((*Address) & Clearmask) | Setmask;

            OS_ARCH_EXIT_CRITICAL();
        }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK)
    {
        /* PRQA S 3138, 3141 ++ */ /* VL_Os_3138, VL_Os_3141 */
        Os_TraceErrorHook(
            OSError_Save_ModifyPeripheral32(Area, Address, Clearmask, Setmask),
            OSServiceId_ModifyPeripheral32,
            status);
        /* PRQA S 3138, 3141 -- */
    }
#endif

    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif /* CFG_PERIPHERAL_MAX > 0U */

/*=======[E N D   O F   F I L E]==============================================*/
/* PRQA S 0553 EOF */ /* VL_QAC_UnUsedFiles */
