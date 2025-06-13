/* PRQA S 3108++ */
/**
 * Copyright (C) 2008-2025 isoft Infrastructure Software Co., Ltd.
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
FUNC(StatusType, OS_CODE) ReadPeripheral8(AreaIdType Area, const uint8* Address, uint8* ReadValue)
{
    OS_ENTER_KERNEL();

    VAR(StatusType, OS_VAR) status = E_OK;

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */ /* OS_PERIPHERAL_TYPE_CAST_001 */
    VAR(uint32, OS_VAR) cmpAddr = (uint32)Address;
    /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */
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
        if (0U == ((Os_PeriAreaCfg[Area].periAreaAppAccMask) & ((uint16)1u << Os_SCB.sysRunningAppID)))
        {
            status = E_OS_ACCESS;
        }
        else if (Os_WrongContext(OS_CONTEXT_OPEARTION_PERIPHERAL) != TRUE)
        {
            status = E_OS_CALLEVEL;
        }
        else if (APPLICATION_ACCESSIBLE != Os_AppCB[Os_SCB.sysRunningAppID].appState)
        {
            status = E_OS_ACCESS;
        }
        else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
        {
            OS_ARCH_DECLARE_CRITICAL();
            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_PERIPHERAL_MACRO_TO_FUNCTION_002 */
            OS_ARCH_ENTRY_CRITICAL();
            /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
            *ReadValue = *Address & 0xFF;
            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_PERIPHERAL_MACRO_TO_FUNCTION_002 */
            OS_ARCH_EXIT_CRITICAL();
            /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
        }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK)
    {
        /* PRQA S 0311 ++ */ /* MISRA Rule 11.8 */ /* OS_PERIPHERAL_MACRO_TO_FUNCTION_002 */
        Os_TraceErrorHook(
            OSError_Save_ReadPeripheral8(Area, (uint8*)Address, ReadValue),
            /* PRQA S 0311 -- */ /* MISRA Rule 11.8 */
            OSServiceId_ReadPeripheral8,
            status);
    }
#endif

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA Dir 4.9 */ /* OS_PERIPHERAL_MACRO_TO_FUNCTION_002 */

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
FUNC(StatusType, OS_CODE) ReadPeripheral16(AreaIdType Area, const uint16* Address, uint16* ReadValue)
{
    OS_ENTER_KERNEL();

    VAR(StatusType, OS_VAR) status = E_OK;

#if (OS_STATUS_EXTENDED == CFG_STATUS)

    /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */ /* OS_PERIPHERAL_TYPE_CAST_001 */
    VAR(uint32, OS_VAR) cmpAddr = (uint32)Address;
    /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */
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
        if (0U == ((Os_PeriAreaCfg[Area].periAreaAppAccMask) & ((uint16)1u << Os_SCB.sysRunningAppID)))
        {
            status = E_OS_ACCESS;
        }
        else if (TRUE != Os_WrongContext(OS_CONTEXT_OPEARTION_PERIPHERAL))
        {
            status = E_OS_CALLEVEL;
        }
        else if (APPLICATION_ACCESSIBLE != Os_AppCB[Os_SCB.sysRunningAppID].appState)
        {
            status = E_OS_ACCESS;
        }
        else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
        {
            OS_ARCH_DECLARE_CRITICAL();
            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_PERIPHERAL_MACRO_TO_FUNCTION_002 */
            OS_ARCH_ENTRY_CRITICAL();
            /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
            *ReadValue = *Address;
            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_PERIPHERAL_MACRO_TO_FUNCTION_002 */
            OS_ARCH_EXIT_CRITICAL();
            /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
        }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK)
    {
        /* PRQA S 0311 ++ */ /* MISRA Rule 11.8 */ /* OS_PERIPHERAL_MACRO_TO_FUNCTION_002 */
        Os_TraceErrorHook(
            OSError_Save_ReadPeripheral16(Area, (uint16*)Address, ReadValue),
            /* PRQA S 0311 -- */ /* MISRA Rule 11.8 */
            OSServiceId_ReadPeripheral16,
            status);
    }
#endif

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA Dir 4.9 */ /* OS_PERIPHERAL_MACRO_TO_FUNCTION_002 */

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
FUNC(StatusType, OS_CODE) ReadPeripheral32(AreaIdType Area, const uint32* Address, uint32* ReadValue)
{
    OS_ENTER_KERNEL();

    VAR(StatusType, OS_VAR) status = E_OK;

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */ /* OS_PERIPHERAL_TYPE_CAST_001 */
    VAR(uint32, OS_VAR) cmpAddr = (uint32)Address;
    /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */
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
        if (0U == ((Os_PeriAreaCfg[Area].periAreaAppAccMask) & ((uint16)1u << Os_SCB.sysRunningAppID)))
        {
            status = E_OS_ACCESS;
        }
        else if (TRUE != Os_WrongContext(OS_CONTEXT_OPEARTION_PERIPHERAL))
        {
            status = E_OS_CALLEVEL;
        }
        else if (APPLICATION_ACCESSIBLE != Os_AppCB[Os_SCB.sysRunningAppID].appState)
        {
            status = E_OS_ACCESS;
        }
        else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
        {
            OS_ARCH_DECLARE_CRITICAL();
            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_PERIPHERAL_MACRO_TO_FUNCTION_002 */
            OS_ARCH_ENTRY_CRITICAL();
            /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
            *ReadValue = *Address;
            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_PERIPHERAL_MACRO_TO_FUNCTION_002 */
            OS_ARCH_EXIT_CRITICAL();
            /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
        }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK)
    {
        /* PRQA S 0311 ++ */ /* MISRA Rule 11.8 */ /* OS_PERIPHERAL_MACRO_TO_FUNCTION_002 */
        Os_TraceErrorHook(
            OSError_Save_ReadPeripheral32(Area, (uint32*)Address, ReadValue),
            /* PRQA S 0311 -- */ /* MISRA Rule 11.8 */
            OSServiceId_ReadPeripheral32,
            status);
    }
#endif

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA Dir 4.9 */ /* OS_PERIPHERAL_MACRO_TO_FUNCTION_002 */

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
FUNC(StatusType, OS_CODE) WritePeripheral8(AreaIdType Area, uint8* Address, uint8 WriteValue)
{
    OS_ENTER_KERNEL();

    VAR(StatusType, OS_VAR) status = E_OK;

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */ /* OS_PERIPHERAL_TYPE_CAST_001 */
    VAR(uint32, OS_VAR) cmpAddr = (uint32)Address;
    /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */
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
        if (0U == ((Os_PeriAreaCfg[Area].periAreaAppAccMask) & ((uint16)1u << Os_SCB.sysRunningAppID)))
        {
            status = E_OS_ACCESS;
        }
        else if (TRUE != Os_WrongContext(OS_CONTEXT_OPEARTION_PERIPHERAL))
        {
            status = E_OS_CALLEVEL;
        }
        else if (APPLICATION_ACCESSIBLE != Os_AppCB[Os_SCB.sysRunningAppID].appState)
        {
            status = E_OS_ACCESS;
        }
        else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
        {
            OS_ARCH_DECLARE_CRITICAL();
            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_PERIPHERAL_MACRO_TO_FUNCTION_002 */
            OS_ARCH_ENTRY_CRITICAL();
            /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
            *Address = (WriteValue & 0xFF);
            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_PERIPHERAL_MACRO_TO_FUNCTION_002 */
            OS_ARCH_EXIT_CRITICAL();
            /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
        }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK)
    {
        Os_TraceErrorHook(
            OSError_Save_WritePeripheral8(Area, Address, WriteValue),
            OSServiceId_WritePeripheral8,
            status);
    }
#endif

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA Dir 4.9 */ /* OS_PERIPHERAL_MACRO_TO_FUNCTION_002 */

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
FUNC(StatusType, OS_CODE) WritePeripheral16(AreaIdType Area, uint16* Address, uint16 WriteValue)
{
    OS_ENTER_KERNEL();

    VAR(StatusType, OS_VAR) status = E_OK;

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */ /* OS_PERIPHERAL_TYPE_CAST_001 */
    VAR(uint32, OS_VAR) cmpAddr = (uint32)Address;
    /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */
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
        if (0U == ((Os_PeriAreaCfg[Area].periAreaAppAccMask) & ((uint16)1u << Os_SCB.sysRunningAppID)))
        {
            status = E_OS_ACCESS;
        }
        else if (TRUE != Os_WrongContext(OS_CONTEXT_OPEARTION_PERIPHERAL))
        {
            status = E_OS_CALLEVEL;
        }
        else if (APPLICATION_ACCESSIBLE != Os_AppCB[Os_SCB.sysRunningAppID].appState)
        {
            status = E_OS_ACCESS;
        }
        else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
        {
            OS_ARCH_DECLARE_CRITICAL();
            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_PERIPHERAL_MACRO_TO_FUNCTION_002 */
            OS_ARCH_ENTRY_CRITICAL();
            /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
            *Address = WriteValue;
            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_PERIPHERAL_MACRO_TO_FUNCTION_002 */
            OS_ARCH_EXIT_CRITICAL();
            /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
        }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK)
    {
        Os_TraceErrorHook(
            OSError_Save_WritePeripheral16(Area, Address, WriteValue),
            OSServiceId_WritePeripheral16,
            status);
    }
#endif

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA Dir 4.9 */ /* OS_PERIPHERAL_MACRO_TO_FUNCTION_002 */

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
FUNC(StatusType, OS_CODE) WritePeripheral32(AreaIdType Area, uint32* Address, uint32 WriteValue)
{
    OS_ENTER_KERNEL();

    VAR(StatusType, OS_VAR) status = E_OK;

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */ /* OS_PERIPHERAL_TYPE_CAST_001 */
    VAR(uint32, OS_VAR) cmpAddr = (uint32)Address;
    /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */
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
        if (0U == ((Os_PeriAreaCfg[Area].periAreaAppAccMask) & ((uint16)1u << Os_SCB.sysRunningAppID)))
        {
            status = E_OS_ACCESS;
        }
        else if (TRUE != Os_WrongContext(OS_CONTEXT_OPEARTION_PERIPHERAL))
        {
            status = E_OS_CALLEVEL;
        }
        else if (APPLICATION_ACCESSIBLE != Os_AppCB[Os_SCB.sysRunningAppID].appState)
        {
            status = E_OS_ACCESS;
        }
        else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
        {
            OS_ARCH_DECLARE_CRITICAL();
            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_PERIPHERAL_MACRO_TO_FUNCTION_002 */
            OS_ARCH_ENTRY_CRITICAL();
            /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
            *Address = WriteValue;
            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_PERIPHERAL_MACRO_TO_FUNCTION_002 */
            OS_ARCH_EXIT_CRITICAL();
            /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
        }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK)
    {
        Os_TraceErrorHook(
            OSError_Save_WritePeripheral32(Area, Address, WriteValue),
            OSServiceId_WritePeripheral32,
            status);
    }
#endif

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA Dir 4.9 */ /* OS_PERIPHERAL_MACRO_TO_FUNCTION_002 */

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
FUNC(StatusType, OS_CODE) ModifyPeripheral8(AreaIdType Area, uint8* Address, uint8 Clearmask, uint8 Setmask)
{
    OS_ENTER_KERNEL();

    VAR(StatusType, OS_VAR) status = E_OK;

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */ /* OS_PERIPHERAL_TYPE_CAST_001 */
    VAR(uint32, OS_VAR) cmpAddr = (uint32)Address;
    /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */
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
        if (0U == ((Os_PeriAreaCfg[Area].periAreaAppAccMask) & ((uint16)1u << Os_SCB.sysRunningAppID)))
        {
            status = E_OS_ACCESS;
        }
        else if (TRUE != Os_WrongContext(OS_CONTEXT_OPEARTION_PERIPHERAL))
        {
            status = E_OS_CALLEVEL;
        }
        else if (APPLICATION_ACCESSIBLE != Os_AppCB[Os_SCB.sysRunningAppID].appState)
        {
            status = E_OS_ACCESS;
        }
        else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
        {
            OS_ARCH_DECLARE_CRITICAL();

            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_PERIPHERAL_MACRO_TO_FUNCTION_002 */
            OS_ARCH_ENTRY_CRITICAL();
            /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
            /* modify the value to address. */
            *Address = (((*Address) & Clearmask) | Setmask) & 0xFF;
            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_PERIPHERAL_MACRO_TO_FUNCTION_002 */
            OS_ARCH_EXIT_CRITICAL();
            /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
        }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK)
    {
        Os_TraceErrorHook(
            OSError_Save_ModifyPeripheral8(Area, Address, Clearmask, Setmask),
            OSServiceId_ModifyPeripheral8,
            status);
    }
#endif

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA Dir 4.9 */ /* OS_PERIPHERAL_MACRO_TO_FUNCTION_002 */

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
FUNC(StatusType, OS_CODE) ModifyPeripheral16(AreaIdType Area, uint16* Address, uint16 Clearmask, uint16 Setmask)
{
    OS_ENTER_KERNEL();

    VAR(StatusType, OS_VAR) status = E_OK;

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */ /* OS_PERIPHERAL_TYPE_CAST_001 */
    VAR(uint32, OS_VAR) cmpAddr = (uint32)Address;
    /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */
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
        if (0U == ((Os_PeriAreaCfg[Area].periAreaAppAccMask) & ((uint16)1u << Os_SCB.sysRunningAppID)))
        {
            status = E_OS_ACCESS;
        }
        else if (TRUE != Os_WrongContext(OS_CONTEXT_OPEARTION_PERIPHERAL))
        {
            status = E_OS_CALLEVEL;
        }
        else if (APPLICATION_ACCESSIBLE != Os_AppCB[Os_SCB.sysRunningAppID].appState)
        {
            status = E_OS_ACCESS;
        }
        else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
        {
            OS_ARCH_DECLARE_CRITICAL();
            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_PERIPHERAL_MACRO_TO_FUNCTION_002 */
            OS_ARCH_ENTRY_CRITICAL();
            /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
            /*modify the value to address*/
            *Address = ((*Address) & Clearmask) | Setmask;
            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_PERIPHERAL_MACRO_TO_FUNCTION_002 */
            OS_ARCH_EXIT_CRITICAL();
            /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
        }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK)
    {
        Os_TraceErrorHook(
            OSError_Save_ModifyPeripheral16(Area, Address, Clearmask, Setmask),
            OSServiceId_ModifyPeripheral16,
            status);
    }
#endif

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA Dir 4.9 */ /* OS_PERIPHERAL_MACRO_TO_FUNCTION_002 */

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
FUNC(StatusType, OS_CODE) ModifyPeripheral32(AreaIdType Area, uint32* Address, uint32 Clearmask, uint32 Setmask)
{
    OS_ENTER_KERNEL();

    VAR(StatusType, OS_VAR) status = E_OK;

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */ /* OS_PERIPHERAL_TYPE_CAST_001 */
    VAR(uint32, OS_VAR) cmpAddr = (uint32)Address;
    /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */
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
        if (0U == ((Os_PeriAreaCfg[Area].periAreaAppAccMask) & ((uint16)1u << Os_SCB.sysRunningAppID)))
        {
            status = E_OS_ACCESS;
        }
        else if (TRUE != Os_WrongContext(OS_CONTEXT_OPEARTION_PERIPHERAL))
        {
            status = E_OS_CALLEVEL;
        }
        else if (APPLICATION_ACCESSIBLE != Os_AppCB[Os_SCB.sysRunningAppID].appState)
        {
            status = E_OS_ACCESS;
        }
        else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
        {
            OS_ARCH_DECLARE_CRITICAL();
            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_PERIPHERAL_MACRO_TO_FUNCTION_002 */
            OS_ARCH_ENTRY_CRITICAL();
            /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
            /*modify the value to address*/
            *Address = ((*Address) & Clearmask) | Setmask;
            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_PERIPHERAL_MACRO_TO_FUNCTION_002 */
            OS_ARCH_EXIT_CRITICAL();
            /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
        }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK)
    {
        Os_TraceErrorHook(
            OSError_Save_ModifyPeripheral32(Area, Address, Clearmask, Setmask),
            OSServiceId_ModifyPeripheral32,
            status);
    }
#endif

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA Dir 4.9 */ /* OS_PERIPHERAL_MACRO_TO_FUNCTION_002 */

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif /* CFG_PERIPHERAL_MAX > 0U */

/*=======[E N D   O F   F I L E]==============================================*/
