/**
 * Copyright (C) 2008-2025 isoft Infrastructure Software Co., Ltd.
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
 ********************************************************************************
 **                                                                            **
 **  FILENAME    :  Os_Extened.c                                               **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      :  i-soft-os                                                  **
 **  Vendor      :                                                             **
 **  DESCRIPTION :  Extended functions                                         **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
 **  Version :   AUTOSAR classic Platform R19--Function Safety                 **
 **                                                                            **
 *******************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Os_Extend.h"
/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Provide Version information to user.>
 * Service ID           <0xfd>
 * Sync/Async           <none>
 * Reentrancy           <Yes>
 * param-eventId[in]    <Versioninfo>
 * Param-Name[out]      <no>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * REQ ID               <None>
 */
/********************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
void OSGetVersionInfo(Std_VersionInfoType* osVerInfoPtr)
/* PRQA S 1503 -- */
{
    if (NULL_PTR == osVerInfoPtr)
    {
        /*nothing to do*/
    }
    else
    {
        /* PRQA S 1290 ++ */ /* VL_Os_1290 */
        osVerInfoPtr->vendorID = OS_VENDOR_ID;
        osVerInfoPtr->moduleID = OS_MODULE_ID;
        /* PRQA S 1290 -- */
        osVerInfoPtr->sw_major_version = OS_CFG_H_SW_MAJOR_VERSION;
        osVerInfoPtr->sw_minor_version = OS_CFG_H_SW_MINOR_VERSION;
        osVerInfoPtr->sw_patch_version = OS_CFG_H_SW_PATCH_VERSION;
    }
}
#if (CHECK_STACK_USAGE > 0)
/********************************************************************/
/*
 * Brief                <Get max usage of system,task,ISR2 stack.>
 * Service ID           <0xfe>
 * Sync/Async           <none>
 * Reentrancy           <Yes>
 * param-eventId[in]    <id>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <osStackUsageType>
 * PreCondition         <None>
 * REQ ID               <None>
 */
/********************************************************************/
osStackUsageType OSGetStackUsage(osStackObject stack, uint16 id)
{
    osStackUsageType MaxUsage    = 0U;
    const uint32*    ptr         = NULL_PTR;
    const uint32*    stackBottom = NULL_PTR;
    const uint32*    stackTop    = NULL_PTR;
    boolean          Status      = TRUE;
    OS_ARCH_DECLARE_CRITICAL();

    id = Os_GetObjLocalId(id);

    switch (stack)
    {
    case OS_STACK_SYSTEM:
    {
        stackBottom = Os_SystemStack->stackBottom;
        stackTop    = Os_SystemStack->stackTop;
    }
    break;

#if (CFG_TASK_MAX > 0U)
    case OS_STACK_TASK:
    {
        /* Input_para check. */
        if (id >= Os_SCB.sysTaskMax)
        {
            Status = FALSE;
        }

        stackBottom = Os_TaskStack[id].stackBottom;
        stackTop    = Os_TaskStack[id].stackTop;
    }
    break;
#endif /* CFG_TASK_MAX > 0U */

#if (CFG_ISR2_MAX > 0U)
    case OS_STACK_ISR2:
    {
        /* Input_para check. */
        if (id >= Os_CfgIsr2Max)
        {
            Status = FALSE;
        }

        stackBottom = Os_ISR2Stack[id].stackBottom;
        stackTop    = Os_ISR2Stack[id].stackTop;
    }
    break;
#endif /* CFG_ISR2_MAX > 0U */

    /*add comments to pass QAC.*/
    default:
        /* Nothing to do. */
        Status = FALSE;
        break;
    }

    if ((boolean)TRUE == Status)
    {
        OS_ARCH_ENTRY_CRITICAL();

        /*
         * MISRA-C:2004 Rule 17.4
         * Msg(4:0489) The integer value 1 is being added or subtracted from a
         *             pointer.
         */
        /* In stack storage area, all bytes are initialized to magic word(0xCC).
         * if this byte is used, the value will be changed. */
        for (ptr = stackBottom; ptr < stackTop; ptr++)
        {
            if (OS_STACK_FILL_PATTERN != (*ptr))
            {
                break;
            }
        }
        MaxUsage = (osStackUsageType)(stackTop - ptr);

        OS_ARCH_EXIT_CRITICAL();
    }

    return MaxUsage;
}
#endif

/********************************************************************/
/*
 * Brief                <check ISR source>
 * Service ID           <0xd8>
 * Sync/Async           <none>
 * Reentrancy           <Yes>
 * param-eventId[in]    <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * REQ ID               <None>
 */
/********************************************************************/
StatusType OSCheckISRSource(uint32 Source) /* PRQA S 1503 */ /* VL_QAC_NoUsedApi */
{
    /* PRQA S 4404 ++ */ /* VL_QAC_AutosarBool */
    StatusType osRet = FALSE;
    /* PRQA S 4404 -- */

/* OS263. */
#if ((CFG_ISR2_MAX > 0) && (CFG_SC == OS_SC4))
    if (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
    {
        if (OS_ISR_CATEGORY2 == Os_IsrCfg[Os_IntCfgIsrId].OsIsrCatType)
        {
            if (Source == Os_IsrCfg[Os_IntCfgIsrId].OsIsrSrc)
            {
                osRet = TRUE;
            }
        }
    }
#else
    (void)Source;
#endif /* CFG_ISR2_MAX > 0 && CFG_SC == OS_SC4 */

    return osRet;
}

/********************************************************************/
/*
 * Brief                <check whether CPU information is correct.>
 * Service ID           <0xff>
 * Sync/Async           <none>
 * Reentrancy           <Yes>
 * param-eventId[in]    <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * REQ ID               <None>
 */
/********************************************************************/
void OSCheckCPUInformation(void) /* PRQA S 1503 */ /* VL_QAC_NoUsedApi */
{
    Arch_CheckCPUInformation();
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/*=======[E N D   O F   F I L E]==============================================*/
