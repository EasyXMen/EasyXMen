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
 ************************************************************************************************************************
 **
 **  @file               : Os_Extened.c
 **  @author             : i-soft-os
 **  @date               : 2024/02/10
 **  @vendor             : isoft
 **  @description        : Os source file for extended function
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Os_Arch_Processor.h"
#include "Os_Extend.h"
#include "Os_Interrupt.h"
#include "Os_Task.h"
#include "Os_Kernel.h"

/* ========================================== external function definitions ========================================= */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * This function provide version information to user.
 */
/* PRQA S 1503 ++ */  /* VL_QAC_NoUsedApi */
void OSGetVersionInfo(Std_VersionInfoType *osVerInfoPtr)
/* PRQA S 1503 -- */
{
    if (NULL_PTR == osVerInfoPtr)
    {
        /*nothing to do*/
    }
    else
    {
        osVerInfoPtr->vendorID = OS_VENDOR_ID;
        osVerInfoPtr->moduleID = OS_MODULE_ID;

        osVerInfoPtr->sw_major_version = OS_CFG_H_SW_MAJOR_VERSION;
        osVerInfoPtr->sw_minor_version = OS_CFG_H_SW_MINOR_VERSION;
        osVerInfoPtr->sw_patch_version = OS_CFG_H_SW_PATCH_VERSION;
    }
}
#if (CFG_STACK_USAGE_CHECK == TRUE) && (CFG_STACK_CHECK == TRUE)
/**
 * This service retrieves the maximum stack usage of the system, task, or interrupt.
 */
Os_StackUsageType OSGetStackUsage(Os_StackObject stack, uint16 id) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    Os_StackUsageType maxUsage = 0U;
    uint32 const *ptr = NULL_PTR;
    uint32 const *stackBottom = NULL_PTR;
    uint32 const *stackTop = NULL_PTR;
    boolean status = TRUE;
    const Os_SCBType *pScb = Os_GetCurrentContext();
    const Os_TaskCfgType *pTaskCfg;
    OS_HAL_DECLARE_CRITICAL();

    switch (stack)
    {
    case OS_STACK_SYSTEM:
        stackBottom = pScb->SystemStack->StackBottom;
        stackTop = pScb->SystemStack->StackTop;
        break;

#if (CFG_TASK_MAX > 0U)
    case OS_STACK_TASK:
        /* Input_para check. */
        /* PRQA S 4342 ++ */ /* VL_Os_4342 */
        if (!CheckCoreTaskId((Os_TaskType)id, pScb->SysCore))
        /* PRQA S 4342 -- */
        {
            status = FALSE;
        }
#if (TRUE == CFG_GLOBAL_TASK_STACK_SHARING)
        if (Os_TaskCfg[id].StackSharing == TRUE)
        {
            status = FALSE;
            break;
        }
#endif
        pTaskCfg = &Os_TaskCfg[id];
        stackBottom = pTaskCfg->TaskStack.StackBottom;
        stackTop = pTaskCfg->TaskStack.StackTop;
        break;
#endif

#if (CFG_ISR2_MAX > 0U)
    case OS_STACK_ISR2:
        /* Input_para check. */

        /* PRQA S 4304,4342 ++ */ /* VL_Os_AutosarBool, VL_Os_4342 */
        if (!Os_CheckIsr2Id((Os_IsrType)id))
        /* PRQA S 4304,4342 -- */
        {
            status = FALSE;
        }
        stackBottom = Os_ISR2Stack[id].StackBottom;
        stackTop = Os_ISR2Stack[id].StackTop;
        break;
#endif

    /*add comments to pass QAC.*/
    default:
        /* Nothing to do. */
        status = FALSE;
        break;
    }

    if ((boolean)TRUE == status)
    {
        OS_HAL_ENTRY_CRITICAL();

        /*
         * MISRA-C:2004 Rule 17.4
         * Msg(4:0489) The integer value 1 is being added or subtracted from a
         *             pointer.
         */
        /* In stack storage area, all bytes are initialized to magic word(0xCC).
         * if this byte is used, the value will be changed. */
        /* PRQA S 0490 ++ */  /* VL_Os_0490 */
        for (ptr = stackBottom; ptr < stackTop; ptr++)
        /* PRQA S 0490 -- */
        {
            if (OS_STACK_FILL_PATTERN != (*ptr))
            {
                break;
            }
        }

        /* PRQA S 0488 ++ */  /* VL_Os_0488 */
        maxUsage = (Os_StackUsageType)(stackTop - ptr);
        /* PRQA S 0488 -- */

        OS_HAL_EXIT_CRITICAL();
    }

    return maxUsage;
}
#endif

/**
 * This service check if the input interrupt source is the current interrupt.
 */
/* PRQA S 1503 ++ */  /* VL_QAC_NoUsedApi */
StatusType OSCheckISRSource(uint32 source)
/* PRQA S 1503 -- */
{
    /* PRQA S 4404 ++ */ /* VL_Os_4404 */
    StatusType osRet = FALSE;
    /* PRQA S 4404 -- */

/* OS263. */
#if ((CFG_ISR2_MAX > 0) && (CFG_SC == OS_SC4))
    Os_CoreIdType curCoreId = Os_GetCoreIdLocal();
    const Os_SCBType *pScb = Os_GetSystemContext(curCoreId);
    if (OS_LEVEL_ISR2 == pScb->SysOsLevel)
    {
        if (OS_ISR_CATEGORY2 == Os_IsrCfg[pScb->SysRunningIsrCat2Id].IsrCatType)
        {
            if (source == Os_IsrCfg[pScb->SysRunningIsrCat2Id].IsrSrc)
            {
                osRet = TRUE;
            }
        }
    }
#else
    (void)source;
#endif

    return osRet;
}

/**
 * This service check whether CPU information is correct.
 */
/* PRQA S 1503 ++ */  /* VL_QAC_NoUsedApi */
void OSCheckCPUInformation(void)
/* PRQA S 1503 -- */
{
    Os_Hal_CheckCPUInformation();
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/*=======[E N D   O F   F I L E]==============================================*/
