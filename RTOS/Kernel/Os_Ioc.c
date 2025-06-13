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
**  FILENAME    : Os_Ioc.c                                                    **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : IOC manager                                                 **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Os_Internal.h"
#if (CFG_IOC_MAX > 0u)
/*=======[M A C R O S]========================================================*/
#if (OS_AUTOSAR_CORES > 1U)
#define IOC_LOCK(com)                                                \
    if (CROSS_CORE_COM == Os_IocCommunicationCfg[com].IocCfgComMark) \
    {                                                                \
        Os_GetInternalSpinlock(&Os_IocSpinlock[com]);                \
    }
#define IOC_UNLOCK(com)                                              \
    if (CROSS_CORE_COM == Os_IocCommunicationCfg[com].IocCfgComMark) \
    {                                                                \
        Os_ReleaseInternalSpinlock(&Os_IocSpinlock[com]);            \
    }
#else
#define IOC_LOCK(com)
#define IOC_UNLOCK(com)
#endif
/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/
#define OS_START_SEC_VAR_CLEARED_GLOBAL_32
#include "Os_MemMap.h"
static Os_SpinlockType Os_IocSpinlock[CFG_IOC_MAX];
#define OS_STOP_SEC_VAR_CLEARED_GLOBAL_32
#include "Os_MemMap.h"

/* Global */
/* PRQA S 0791++ */ /* MISRA Rule 5.4 */ /*OS_IOC_SEGMENTNAME_SIMILAR_003*/
#define OS_START_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "Os_MemMap.h"
static Os_IocCBType Os_IocCB[CFG_IOC_MAX];
#define OS_STOP_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "Os_MemMap.h"
/* PRQA S 0791-- */ /* MISRA Rule 5.4 */
/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static FUNC(boolean, OS_CODE) Os_IocUseQueue(Os_IocComIdType comId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Init the IOC control block list>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_InitIoc(void)
{
    if (OS_CORE_ID_MASTER == Os_SCB.sysCore)
    {
        P2CONST(Os_IocCommunicationCfgType, OS_VAR, OS_CONST) pIocComCfg = Os_IocCommunicationCfg;
        VAR(Os_IocU16Type, OS_VAR) loop;

        for (loop = 0U; loop < CFG_IOC_MAX; loop++)
        {
            Os_IocCB[loop].IocBlockPtr = pIocComCfg[loop].IocBlockPtr;
            Os_IocCB[loop].IocBlockLength = pIocComCfg[loop].IocBlockCnt;
            Os_IocCB[loop].IocBufferPtr = pIocComCfg[loop].IocBufferPtr;
            Os_IocCB[loop].IocDataElementSizeWithHeader =
                (uint16)(pIocComCfg[loop].IocDataTotalLength
                         + (pIocComCfg[loop].IocDataNumber * OS_IOC_DATA_HEAD_SIZE));
            (void)Os_IocEmpty(loop);
        }
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The API will copy data from source to destination.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(void, OS_CODE) Os_IocMemCopy(const uint8* src, uint8* dest, Os_IocU16Type size)
{
    VAR(Os_IocU16Type, OS_VAR) loop;

    for (loop = 0; loop < size; loop++)
    {
        *dest = *src;
        dest = dest + 1; /* PRQA S 1338 */ /* MISRA Rule-17.8*/ /*OS_IOC_CONST_003*/
        src = src + 1; /* PRQA S 1338 */ /* MISRA Rule-17.8*/   /*OS_IOC_CONST_003*/
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get Free Block ID.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(Os_IocBlockIdType, OS_CODE) Os_IocGetBlock(Os_IocComIdType comId)
{
    OS_ARCH_DECLARE_CRITICAL();

    VAR(Os_IocBlockIdType, OS_VAR) vBlockId = OS_OBJECT_INVALID;
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_IOC_COMPILER_001 */
    P2VAR(Os_IocCBType, OS_VAR, OS_VAR) pIocCom = &Os_IocCB[comId];
    P2VAR(Os_IocBlockType, OS_VAR, OS_VAR) pBlockBase = pIocCom->IocBlockPtr;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

    OS_ARCH_ENTRY_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_IOC_MACRO_004 */
    IOC_LOCK(comId);

    if (OS_OBJECT_INVALID != Os_IocCB[comId].IocBlockFreeHead)
    {
        /* remove from free block list. */
        vBlockId = pIocCom->IocBlockFreeHead;
        pIocCom->IocBlockFreeHead = pBlockBase[vBlockId].IocBlockLink.IocBlockNext;
        pBlockBase[vBlockId].IocBlockLink.IocBlockNext = OS_OBJECT_INVALID;

        /* modify the block state */
        pBlockBase[vBlockId].IocBlockState = BLOCK_WRITING;
    }

    IOC_UNLOCK(comId);
    OS_ARCH_EXIT_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_IOC_MACRO_004 */

    return (vBlockId);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Release the block to the free queue.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(void, OS_CODE) Os_IocReleaseBlock(Os_IocComIdType comId, Os_IocBlockIdType blockId)
{
    OS_ARCH_DECLARE_CRITICAL();

    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_IOC_COMPILER_001 */
    P2VAR(Os_IocCBType, OS_VAR, OS_VAR) pIocCom = &Os_IocCB[comId];
    P2VAR(Os_IocBlockType, OS_VAR, OS_VAR) pBlockBase = pIocCom->IocBlockPtr;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

    if (blockId != OS_OBJECT_INVALID)
    {
        OS_ARCH_ENTRY_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_IOC_MACRO_004 */
        IOC_LOCK(comId);

        /* add to free block list. */
        pBlockBase[blockId].IocBlockLink.IocBlockNext = pIocCom->IocBlockFreeHead;

        /* set new head */
        pIocCom->IocBlockFreeHead = blockId;

        /* modify the block state */
        pBlockBase[blockId].IocBlockState = BLOCK_IDLE;

        IOC_UNLOCK(comId);
        OS_ARCH_EXIT_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_IOC_MACRO_004 */
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Write data into block.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3673 ++*/ /* MISRA Rule-8.13*/ /*OS_IOC_CONST_005*/
static FUNC(void, OS_CODE) Os_IocWriteDataToBlock(
    Os_IocComIdType comId,
    Os_IocBlockIdType blockId,
    Os_IocSendDataSourceType* pData,
    Os_IocU16Type paraNum)
/* PRQA S 3673 --*/ /* MISRA Rule-8.13*/
{
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_IOC_COMPILER_001 */
    /* PRQA S 3678 ++ */ /* MISRA Rule-8.13*/  /*OS_IOC_CONST_005*/
    P2VAR(Os_IocCBType, OS_VAR, OS_VAR) pIocCom = &Os_IocCB[comId];
    /* PRQA S 3678 -- */ /* MISRA Rule-8.13*/
    P2VAR(Os_IocBufferType, OS_VAR, OS_VAR) pBufferBase = pIocCom->IocBufferPtr;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    VAR(Os_IocU16Type, OS_VAR) vBlockLength = pIocCom->IocBlockLength;
    VAR(Os_IocU16Type, OS_VAR) vDataElementLength = pIocCom->IocDataElementSizeWithHeader;

    VAR(Os_IocU16Type, OS_VAR) offset;
    VAR(Os_IocU16Type, OS_VAR) length;
    VAR(Os_IocU16Type, OS_VAR) loop;

    if (blockId < vBlockLength)
    {
        offset = blockId * vDataElementLength;
        for (loop = 0U; loop < paraNum; loop++)
        {
            /* write data element header */
            length = pData[loop].IocDataLenth;
            if (1U == OS_IOC_DATA_HEAD_SIZE)
            {
                pBufferBase[offset + 0U] = (Os_IocBufferType)(length);
            }
            else
            {
                pBufferBase[offset + 0U] = (Os_IocBufferType)(length >> 8U);
                pBufferBase[offset + 1U] = (Os_IocBufferType)(length & 0xFFU);
            }
            offset = (uint16)(offset + OS_IOC_DATA_HEAD_SIZE);
            /* write data element */
            /* PRQA S 0488 ++ */ /* MISRA Rule 18.4 */ /* OS_IOC_PTR_OPERATIONS_002 */
            Os_IocMemCopy((const uint8*)pData[loop].IocDataPtr, (uint8*)pBufferBase + offset, length);
            /* PRQA S 0488 -- */ /* MISRA Rule 18.4 */

            offset = offset + length;
        }
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Read data from block.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3673 ++*/ /* MISRA Rule-8.13*/ /*OS_IOC_CONST_005*/
static FUNC(void, OS_CODE) Os_IocReadDataFromBlock(
    Os_IocComIdType comId,
    Os_IocBlockIdType blockId,
    Os_IocReceiveDataSourceType* pData,
    Os_IocU16Type paraNum)
/* PRQA S 3673 --*/ /* MISRA Rule-8.13*/
{
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_IOC_COMPILER_001 */
    /* PRQA S 3678 ++ */ /* MISRA Rule-8.13*/  /*OS_IOC_CONST_005*/
    P2VAR(Os_IocCBType, OS_VAR, OS_VAR) pIocCom = &Os_IocCB[comId];
    P2VAR(Os_IocBufferType, OS_VAR, OS_VAR) pBufferBase = pIocCom->IocBufferPtr;
    /* PRQA S 3678 -- */ /* MISRA Rule-8.13*/
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    VAR(Os_IocU16Type, OS_VAR) vBlockLength = pIocCom->IocBlockLength;
    VAR(Os_IocU16Type, OS_VAR) vDataElementLength = pIocCom->IocDataElementSizeWithHeader;
    P2CONST(Os_IocCommunicationCfgType, OS_VAR, OS_CONST) pIocComCfg = Os_IocCommunicationCfg;
    P2CONST(Os_IocDataPropertiesCfgType, OS_VAR, OS_CONST) pIocData = pIocComCfg[comId].IocDataProperties;
    VAR(Os_IocU16Type, OS_VAR) offset = 0;
    VAR(Os_IocU16Type, OS_VAR) length;
    VAR(Os_IocU16Type, OS_VAR) loop;

    if (blockId < vBlockLength)
    {
        offset = blockId * vDataElementLength;
        for (loop = 0U; loop < paraNum; loop++)
        {
            /* read data element header */
            if (1U == OS_IOC_DATA_HEAD_SIZE)
            {
                length = pBufferBase[offset + 0U];
            }
            else
            {
                length = pBufferBase[offset + 0U];
                length = pBufferBase[offset + 1U] + (length << 8U);
            }
            if (length <= pIocData[loop].IocDataTypeLength)
            {
                /* check the ptr is valid */
                if (NULL_PTR != pData[loop].IocDataLenth)
                {
                    *(pData[loop].IocDataLenth) = length;
                }

                offset = (uint16)(offset + OS_IOC_DATA_HEAD_SIZE);
                /* read data element */
                /* PRQA S 0488 ++ */ /* MISRA Rule 18.4 */ /* OS_IOC_PTR_OPERATIONS_002 */
                Os_IocMemCopy((const uint8*)pBufferBase + offset, (uint8*)pData[loop].IocDataPtr, length);
                /* PRQA S 0488 -- */ /* MISRA Rule 18.4 */
                offset = offset + length;
            }
            else
            {
                break;
            }
        }
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The API will check that whether the byte number of
 *                       the API's input parameter is the same as the ORIENTAIS
 *                       Studio configuration.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3673 ++ */ /* MISRA Rule-8.13*/ /*OS_IOC_CONST_005*/
static FUNC(boolean, OS_CODE)
    Os_IocSendParaCheck(Os_IocComIdType comId, Os_IocSendDataSourceType* pData, Os_IocU16Type paraNum)
/* PRQA S 3673 -- */ /* MISRA Rule-8.13*/
{
    VAR(boolean, OS_VAR) vRet = TRUE;
    P2CONST(Os_IocCommunicationCfgType, OS_VAR, OS_CONST) pIocComCfg = Os_IocCommunicationCfg;
    P2CONST(Os_IocDataPropertiesCfgType, OS_VAR, OS_CONST) pIocData = pIocComCfg[comId].IocDataProperties;
    VAR(Os_IocU16Type, OS_VAR) loop;

    if (paraNum == pIocComCfg[comId].IocDataNumber)
    {
        for (loop = 0; loop < paraNum; loop++)
        {
            /* the parameter length must greater than 0 and smaller than max length */
            if ((pData[loop].IocDataLenth > pIocData[loop].IocDataTypeLength) || (0U == pData[loop].IocDataLenth))
            {
                vRet = FALSE;
            }
        }
    }
    else
    {
        vRet = FALSE;
    }

    return (vRet);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Insert the sent data into the block to be read.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(void, OS_CODE) Os_IocAppendReadyBlock(Os_IocComIdType comId, Os_IocBlockIdType vBlockId)
{
    OS_ARCH_DECLARE_CRITICAL();

    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_IOC_COMPILER_001 */
    P2VAR(Os_IocCBType, OS_VAR, OS_VAR) pIocCom = &Os_IocCB[comId];
    P2VAR(Os_IocBlockType, OS_VAR, OS_VAR) pBlockBase = pIocCom->IocBlockPtr;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    VAR(Os_IocBlockIdType, OS_VAR) vBlockTail;

    pBlockBase[vBlockId].IocBlockState = BLOCK_READY;

    OS_ARCH_ENTRY_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_IOC_MACRO_004 */
    IOC_LOCK(comId);

    if (OS_OBJECT_INVALID == pIocCom->IocBlockHead)
    {
        pIocCom->IocBlockHead = vBlockId;
    }

    if (OS_OBJECT_INVALID == pIocCom->IocBlockTail)
    {
        pIocCom->IocBlockTail = vBlockId;
    }
    else
    {
        vBlockTail = pIocCom->IocBlockTail;

        pBlockBase[vBlockTail].IocBlockLink.IocBlockNext = vBlockId;
        pIocCom->IocBlockTail = vBlockId;
    }

    IOC_UNLOCK(comId);
    OS_ARCH_EXIT_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_IOC_MACRO_004 */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Remove the block to be read from the queue.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(Os_IocBlockIdType, OS_CODE) Os_IocRemoveReadyBlock(Os_IocComIdType comId)
{
    OS_ARCH_DECLARE_CRITICAL();

    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_IOC_COMPILER_001 */
    P2VAR(Os_IocCBType, OS_VAR, OS_VAR) pIocCom = &Os_IocCB[comId];
    P2VAR(Os_IocBlockType, OS_VAR, OS_VAR) pBlockBase = pIocCom->IocBlockPtr;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    VAR(Os_IocBlockIdType, OS_VAR) vBlockHead;
    VAR(Os_IocBlockIdType, OS_VAR) vBlockReady;

    OS_ARCH_ENTRY_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_IOC_MACRO_004 */
    IOC_LOCK(comId);

    vBlockHead = pIocCom->IocBlockHead;
    vBlockReady = vBlockHead;

    if (OS_OBJECT_INVALID != vBlockHead)
    {
        if (pIocCom->IocBlockHead == pIocCom->IocBlockTail)
        {
            pIocCom->IocBlockHead = OS_OBJECT_INVALID;
            pIocCom->IocBlockTail = OS_OBJECT_INVALID;
        }
        else
        {
            pIocCom->IocBlockHead = pBlockBase[vBlockHead].IocBlockLink.IocBlockNext;
            pBlockBase[vBlockReady].IocBlockLink.IocBlockNext = OS_OBJECT_INVALID;
        }

        pBlockBase[vBlockReady].IocBlockState = BLOCK_READING;
    }

    IOC_UNLOCK(comId);
    OS_ARCH_EXIT_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_IOC_MACRO_004 */

    return (vBlockReady);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Check caller write permission.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(boolean, OS_CODE) Os_IocAppWriteAccess(Os_IocComIdType comId, Os_ApplicationType appId)
{
    VAR(boolean, OS_VAR) vRet = FALSE;
    P2CONST(Os_IocCommunicationCfgType, OS_VAR, OS_CONST) pIocComCfg = Os_IocCommunicationCfg;
    VAR(Os_IocU16Type, OS_VAR) loop;

    for (loop = 0; loop < pIocComCfg[comId].IocSenderNumber; loop++)
    {
        if (appId == pIocComCfg[comId].IocSenderProperties[loop].IocSenderApplicationRef)
        {
            vRet = TRUE;
            break;
        }
    }

    return (vRet);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Check caller read permission.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(boolean, OS_CODE) Os_IocAppReadAccess(Os_IocComIdType comId, Os_ApplicationType appId)
{
    VAR(boolean, OS_VAR) vRet = FALSE;
    P2CONST(Os_IocCommunicationCfgType, OS_VAR, OS_CONST) pIocComCfg = Os_IocCommunicationCfg;
    VAR(Os_IocU16Type, OS_VAR) loop;

    if (Os_SCB.sysRunningAppID == Os_SCB.sysAppId)
    {
        vRet = TRUE;
    }
    else
    {
        for (loop = 0; loop < pIocComCfg[comId].IocReceiverNumber; loop++)
        {
            if (appId == pIocComCfg[comId].IocReceiverProperties[loop].IocReceiverApplicationRef)
            {
                vRet = TRUE;
                break;
            }
        }
    }

    return (vRet);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* OS_SC3 == CFG_SC || OS_SC4 == CFG_SC */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Obtain whether the communication mode is queue or non-queue.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(boolean, OS_CODE) Os_IocUseQueue(Os_IocComIdType comId)
{
    P2CONST(Os_IocCommunicationCfgType, OS_VAR, OS_CONST) pIocComCfg = Os_IocCommunicationCfg;
    VAR(boolean, OS_VAR) vRet = pIocComCfg[comId].IocQueueEnable;

    return (vRet);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Non-queue communication occupies a private block.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(Os_IocU16Type, OS_CODE) Os_IocGetSenderBlock(Os_IocComIdType comId, Os_IocU16Type senderId)
{
    VAR(Os_IocBlockIdType, OS_VAR) vBlockId = OS_OBJECT_INVALID;
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_IOC_COMPILER_001 */
    /* PRQA S 3678 ++ */ /* MISRA Rule-8.13*/  /*OS_IOC_CONST_005*/
    P2VAR(Os_IocCBType, OS_VAR, OS_VAR) pIocCom = &Os_IocCB[comId];
    /* PRQA S 3678 -- */ /* MISRA Rule-8.13*/
    P2VAR(Os_IocBlockType, OS_VAR, OS_VAR) pBlockBase = pIocCom->IocBlockPtr;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

    vBlockId = pBlockBase[senderId].IocBlockLink.IocBlockCurrent;
    if (BLOCK_WRITING != pBlockBase[vBlockId].IocBlockState)
    {
        pBlockBase[vBlockId].IocBlockState = BLOCK_WRITING;
    }
    else
    {
        vBlockId = OS_OBJECT_INVALID;
    }
    return (vBlockId);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Non-queue communication exchanges the sending data block to the public area.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(void, OS_CODE)
    Os_IocSwitchCommonBlock(Os_IocComIdType comId, Os_IocU16Type senderId, Os_IocBlockIdType blockId)
{
    OS_ARCH_DECLARE_CRITICAL();
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_IOC_COMPILER_001 */
    P2VAR(Os_IocCBType, OS_VAR, OS_VAR) pIocCom = &Os_IocCB[comId];
    P2VAR(Os_IocBlockType, OS_VAR, OS_VAR) pBlockBase = pIocCom->IocBlockPtr;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

    OS_ARCH_ENTRY_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_IOC_MACRO_004 */
    IOC_LOCK(comId);
    if (pBlockBase[blockId].IocBlockState == BLOCK_WRITING)
    {
        pBlockBase[senderId].IocBlockLink.IocBlockCurrent = pIocCom->IocBlockRead;
        pIocCom->IocBlockRead = blockId;
        pBlockBase[blockId].IocBlockState = BLOCK_READY;
    }
    IOC_UNLOCK(comId);
    OS_ARCH_EXIT_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_IOC_MACRO_004 */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get data buffer control block.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(Os_IocU16Type, OS_CODE) Os_IocGetReadyBlock(Os_IocComIdType comId)
{
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_IOC_COMPILER_001 */
    /* PRQA S 3678 ++ */ /* MISRA Rule-8.13*/  /*OS_IOC_CONST_005*/
    P2VAR(Os_IocCBType, OS_VAR, OS_VAR) pIocCom = &Os_IocCB[comId];
    /* PRQA S 3678 -- */ /* MISRA Rule-8.13*/
    P2VAR(Os_IocBlockType, OS_VAR, OS_VAR) pBlockBase = pIocCom->IocBlockPtr;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    VAR(Os_IocBlockIdType, OS_VAR) vBlockId = pIocCom->IocBlockRead;
    if ((BLOCK_READY == pBlockBase[vBlockId].IocBlockState) || (BLOCK_READING == pBlockBase[vBlockId].IocBlockState))
    {
        pBlockBase[vBlockId].IocBlockState = BLOCK_READING;
    }
    else
    {
        vBlockId = OS_OBJECT_INVALID;
    }
    return (vBlockId);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <IOC internal communication buffer full set flag.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(void, OS_CODE) Os_IocSetLostDataFlag(Os_IocComIdType comId)
{
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_IOC_COMPILER_001 */
    P2VAR(Os_IocCBType, OS_VAR, OS_VAR) pIocCom = &Os_IocCB[comId];
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    pIocCom->IocLostData = TRUE;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get whether the internal buffer overflows.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(boolean, OS_CODE) Os_IocGetLostDataFlag(Os_IocComIdType comId)
{
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_IOC_COMPILER_001 */
    P2VAR(Os_IocCBType, OS_VAR, OS_VAR) pIocCom = &Os_IocCB[comId];
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    VAR(boolean, OS_VAR) vLostData;

    vLostData = pIocCom->IocLostData;
    pIocCom->IocLostData = FALSE;

    return (vLostData);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Call the callback function to notify the receiver.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(void, OS_CODE) Os_IocTriggerReceiveCallBack(
    Os_IocComIdType comId,
    Os_IocU16Type vReceiverId,
    Os_CoreIdType vReceiverCoreId,
    Os_ApplicationType vRecAppId)
{
    VAR(Os_IocCallbackType, OS_VAR) pCallBackFunc;
    P2CONST(Os_IocCommunicationCfgType, OS_VAR, OS_CONST) pIocComCfg = Os_IocCommunicationCfg;
    VAR(Os_ApplicationType, OS_VAR) bakappID = Os_SCB.sysRunningAppID;

#if (OS_AUTOSAR_CORES > 1)
    VAR(uint16, OS_VAR) vCoreId = Os_SCB.sysCore;
    if (vCoreId != vReceiverCoreId)
    {
        RpcInputType rpcData = {
            .sync = RPC_ASYNC,
            .remoteCoreId = vReceiverCoreId,
            .serviceId = OSServiceId_IocCallBackNotify,
            .srvPara0 = (uint32)comId,
            .srvPara1 = (uint32)vReceiverId,
            .srvPara2 = (uint32)vRecAppId,
        };

        (void)Os_RpcCallService(&rpcData);
    }
    else
#endif /* OS_AUTOSAR_CORES > 1 */
    {
        pCallBackFunc = pIocComCfg[comId].IocReceiverProperties[vReceiverId].IocCallBackFunc;
        OS_ARCH_DECLARE_CRITICAL();

        OS_ARCH_ENTRY_CRITICAL();
#if (CFG_OSAPPLICATION_MAX > 0U)
        Os_SCB.sysRunningAppID = vRecAppId;
#endif
        OS_ARCH_EXIT_CRITICAL();

        Os_SCB.sysDispatchLocker = Os_SCB.sysDispatchLocker + 1u;
        pCallBackFunc();
        Os_SCB.sysDispatchLocker = Os_SCB.sysDispatchLocker - 1u;

        OS_ARCH_ENTRY_CRITICAL();
#if (CFG_OSAPPLICATION_MAX > 0U)
        Os_SCB.sysRunningAppID = bakappID;
#endif
        OS_ARCH_EXIT_CRITICAL();
    }
    UNUSED_PARAMETER(vReceiverCoreId);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <IOC cross-core remote callback function>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(StatusType, OS_CODE)
Os_IocRemoteCallBack(Os_IocComIdType comId, Os_IocU16Type vReceiverId, Os_ApplicationType vRecAppId)
{
    P2CONST(Os_IocCommunicationCfgType, OS_VAR, OS_CONST) iocComCfgPtr;
    VAR(Os_ApplicationType, OS_VAR) bakappID = Os_SCB.sysRunningAppID;
    iocComCfgPtr = &Os_IocCommunicationCfg[comId];
    OS_ARCH_DECLARE_CRITICAL();

    OS_ARCH_ENTRY_CRITICAL();
    Os_SCB.sysRunningAppID = vRecAppId;
    OS_ARCH_EXIT_CRITICAL();

    iocComCfgPtr->IocReceiverProperties[vReceiverId].IocCallBackFunc();

    OS_ARCH_ENTRY_CRITICAL();
    Os_SCB.sysRunningAppID = bakappID;
    OS_ARCH_EXIT_CRITICAL();

    return IOC_E_OK;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <This API implements writing source data to a buffer>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(StatusType, OS_CODE)
Os_IocTransmit(Os_IocComIdType comId, Os_IocSendDataSourceType* pData, Os_IocU16Type paraNum, Os_IocU16Type senderId)
{
    OS_ENTER_KERNEL();
    OS_ARCH_DECLARE_CRITICAL();

    VAR(StatusType, OS_VAR) vRet = IOC_E_NOK;
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    VAR(Os_ApplicationType, OS_VAR) vSendAppId = Os_SCB.sysRunningAppID;
#endif /* OS_SC3 == CFG_SC || OS_SC4 == CFG_SC */
    VAR(Os_IocBlockIdType, OS_VAR) vBlockId = OS_OBJECT_INVALID;
    P2CONST(Os_IocCommunicationCfgType, OS_VAR, OS_CONST) pIocComCfg = Os_IocCommunicationCfg;

    VAR(Os_IocCallbackType, OS_VAR) pCallBackFunc;
    VAR(Os_IocU16Type, OS_VAR) loop;
    VAR(Os_CoreIdType, OS_VAR) vReceiverCoreId;
    VAR(Os_ApplicationType, OS_VAR) vRecAppId;

    if (TRUE == Os_IocSendParaCheck(comId, pData, paraNum))
    {
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
        if (TRUE == Os_IocAppWriteAccess(comId, vSendAppId))
#endif /* OS_SC3 == CFG_SC || OS_SC4 == CFG_SC */
        {
            if (TRUE == Os_IocUseQueue(comId))
            {
                /* Get Free Block ID */
                vBlockId = Os_IocGetBlock(comId);

                if (OS_OBJECT_INVALID != vBlockId)
                {
                    Os_IocWriteDataToBlock(comId, vBlockId, pData, paraNum);
                    Os_IocAppendReadyBlock(comId, vBlockId);

                    vRet = IOC_E_OK;
                }
                else
                {
                    Os_IocSetLostDataFlag(comId);
                    vRet = IOC_E_LIMIT;
                }
            }
            else
            {
                OS_ARCH_ENTRY_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_IOC_MACRO_004 */
                /* Get Sender Current Block */
                vBlockId = Os_IocGetSenderBlock(comId, senderId);
                OS_ARCH_EXIT_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_IOC_MACRO_004 */

                if (OS_OBJECT_INVALID != vBlockId)
                {
                    /* Write the Current Block */
                    Os_IocWriteDataToBlock(comId, vBlockId, pData, paraNum);

                    /* Switch the Sender Block and the Common Block */
                    Os_IocSwitchCommonBlock(comId, senderId, vBlockId);

                    vRet = IOC_E_OK;
                }
            }
        }
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
        else
        {
            /* Access Error */
            vRet = E_OS_ACCESS;
        }
#endif /* OS_SC3 == CFG_SC || OS_SC4 == CFG_SC */
    }
    else
    {
        vRet = IOC_E_LENGTH;
    }

    if (IOC_E_OK == vRet)
    {
        for (loop = 0; loop < pIocComCfg[comId].IocReceiverNumber; loop++)
        {
            /* trigger receive callback function */
            pCallBackFunc = pIocComCfg[comId].IocReceiverProperties[loop].IocCallBackFunc;

            if (NULL_PTR != pCallBackFunc)
            {
                vRecAppId = pIocComCfg[comId].IocReceiverProperties[loop].IocReceiverApplicationRef;
                vReceiverCoreId = Os_AppCoreMap[vRecAppId];
                Os_IocTriggerReceiveCallBack(comId, loop, vReceiverCoreId, vRecAppId);
            }
        }
    }

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_IOC_MACRO_004 */
    return (vRet);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <This API implements reading data from the buffer and passing it to the caller.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(StatusType, OS_CODE)
Os_IocReceive(Os_IocComIdType comId, Os_IocReceiveDataSourceType* pData, Os_IocU16Type paraNum)
{
    OS_ENTER_KERNEL();
    OS_ARCH_DECLARE_CRITICAL();

    VAR(StatusType, OS_VAR) vRet = IOC_E_NOK;
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    VAR(Os_ApplicationType, OS_VAR) vReceiveAppId = Os_SCB.sysRunningAppID;
#endif /* OS_SC3 == CFG_SC || OS_SC4 == CFG_SC */
    VAR(Os_IocBlockIdType, OS_VAR) vBlockId = OS_OBJECT_INVALID;

#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    if (TRUE == Os_IocAppReadAccess(comId, vReceiveAppId))
#endif /* OS_SC3 == CFG_SC || OS_SC4 == CFG_SC */
    {
        if (TRUE == Os_IocUseQueue(comId))
        {
            vBlockId = Os_IocRemoveReadyBlock(comId);
            if (OS_OBJECT_INVALID != vBlockId)
            {
                Os_IocReadDataFromBlock(comId, vBlockId, pData, paraNum);
                Os_IocReleaseBlock(comId, vBlockId);

                if (FALSE == Os_IocGetLostDataFlag(comId))
                {
                    vRet = IOC_E_OK;
                }
                else
                {
                    vRet = IOC_E_LOST_DATA;
                }
            }
            else
            {
                vRet = IOC_E_NO_DATA;
            }
        }
        else
        {
            OS_ARCH_ENTRY_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_IOC_MACRO_004 */
            IOC_LOCK(comId);

            vBlockId = Os_IocGetReadyBlock(comId);

            IOC_UNLOCK(comId);
            OS_ARCH_EXIT_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_IOC_MACRO_004 */

            if (OS_OBJECT_INVALID != vBlockId)
            {
                Os_IocReadDataFromBlock(comId, vBlockId, pData, paraNum);
                vRet = IOC_E_OK;
            }
        }
    }
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    else
    {
        /* Access Error */
        vRet = E_OS_ACCESS;
    }
#endif /* OS_SC3 == CFG_SC || OS_SC4 == CFG_SC */

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_IOC_MACRO_004 */
    return (vRet);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <In case of queued communication identified by the <IocId>
 *                       in the function name, the content of the IOC internal
 *                       communication queue shall be deleted.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_InitSystem>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(StatusType, OS_CODE) Os_IocEmpty(Os_IocComIdType comId)
{
    OS_ENTER_KERNEL();

    VAR(StatusType, OS_VAR) vRet = IOC_E_NOK;
    P2CONST(Os_IocCommunicationCfgType, OS_VAR, OS_CONST) pIocComCfg = Os_IocCommunicationCfg;
    P2CONST(Os_IocDataPropertiesCfgType, OS_VAR, OS_CONST) pIocData = pIocComCfg[comId].IocDataProperties;

    VAR(Os_IocU16Type, OS_VAR) vIocBlockCnt = pIocComCfg[comId].IocBlockCnt;
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_IOC_COMPILER_001 */
    P2VAR(Os_IocCBType, OS_VAR, OS_VAR) pIocCom = &Os_IocCB[comId];

    P2VAR(Os_IocBlockType, OS_VAR, OS_VAR) pBlockBase = pIocCom->IocBlockPtr;
    P2VAR(Os_IocBufferType, OS_VAR, OS_VAR) pBufferBase = pIocCom->IocBufferPtr;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    VAR(Os_IocU16Type, OS_VAR) vDataElementLength = pIocCom->IocDataElementSizeWithHeader;
    VAR(Os_IocU16Type, OS_VAR) paraNum = pIocComCfg[comId].IocDataNumber;
    VAR(Os_IocU16Type, OS_VAR) offset = 0;

    VAR(Os_IocU16Type, OS_VAR) length;
    VAR(Os_IocU16Type, OS_VAR) loop;

    if (comId < CFG_IOC_MAX)
    {
        if (TRUE == Os_IocUseQueue(comId))
        {
            for (loop = 0U; loop < vIocBlockCnt; loop++)
            {
                pBlockBase[loop].IocBlockLink.IocBlockNext = loop + 1U;
                pBlockBase[loop].IocBlockState = BLOCK_IDLE;
            }
            pBlockBase[vIocBlockCnt - 1U].IocBlockLink.IocBlockNext = OS_OBJECT_INVALID;
            pIocCom->IocBlockHead = OS_OBJECT_INVALID;
            pIocCom->IocBlockTail = OS_OBJECT_INVALID;
            pIocCom->IocBlockFreeHead = 0U;
        }
        else
        {
            for (loop = 0; loop < vIocBlockCnt; loop++)
            {
                pBlockBase[loop].IocBlockLink.IocBlockCurrent = loop;
                pBlockBase[loop].IocBlockState = BLOCK_IDLE;
            }
            pIocCom->IocBlockRead = pIocComCfg[comId].IocSenderNumber;

            offset = pIocCom->IocBlockRead * vDataElementLength;
            /* Init Value */
            for (loop = 0U; loop < paraNum; loop++)
            {
                length = pIocData[loop].IocDataTypeLength;
                if (1U == OS_IOC_DATA_HEAD_SIZE)
                {
                    pBufferBase[offset + 0U] = (Os_IocBufferType)(length);
                }
                else
                {
                    pBufferBase[offset + 0U] = (Os_IocBufferType)(length >> 8U);
                    pBufferBase[offset + 1U] = (Os_IocBufferType)(length & 0xFFU);
                }
                offset = (uint16)(offset + OS_IOC_DATA_HEAD_SIZE);
                if (TRUE == pIocData[loop].IocInitValueEnable)
                {
                    /* PRQA S 0488 ++ */ /* MISRA Rule 18.4 */ /* OS_IOC_PTR_OPERATIONS_002 */
                    Os_IocMemCopy((const uint8*)pIocData[loop].IocInitValuePtr, pBufferBase + offset, length);
                    /* PRQA S 0488 -- */ /* MISRA Rule 18.4 */
                    pBlockBase[pIocCom->IocBlockRead].IocBlockState = BLOCK_READY;
                }
                offset = offset + length;
            }
        }
        vRet = IOC_E_OK;
    }

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_IOC_MACRO_004 */

    return (vRet);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

/*=======[E N D   O F   F I L E]==============================================*/
