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
**************************************************************************** **
**                                                                           **
**  FILENAME    : NvM_Inter.c                                                **
**                                                                           **
**  Created on  : 2020/5/9 15:21:52                                          **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION :Containing the entire or parts of NVRAM manager code        **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                     **
**                                                                           **
**************************************************************************** */

/* req NVM076 */

/*====================[I N C L U D E S]====================*/
#include "MemIf.h"    /* req NVM077 */
#include "SchM_NvM.h" /* req NVM077 */
#include "NvM_Inter.h"

#if ((STD_ON == NVM_BSWM_MULTIBLOCK_JOBSTATUS_INFORMATION) || (STD_ON == NVM_BSWM_SINGLEBLOCK_JOBSTATUS_INFORMATION))
#include "BswM_NvM.h"
#endif
#if (STD_ON == NVM_DEM_PRODUCTION_ERROR_DETECT)
#include "Dem.h" /* req NVM077 */
#endif
#if (STD_ON == NVM_INCLUDE_CRC)
#include "Crc.h" /* req NVM077 */
#endif

/*====================[F U N C  D E C L A R A T I O N]====================*/
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"
static FUNC(void, NVM_CODE) NvM_RepireDataSave(P2CONST(uint8, TYPEDEF, NVM_APPL_DATA) DataSourceAddress, uint16 Length);
static FUNC(void, NVM_CODE)
    /* PRQA S 0661++ */ /* MISRA Rule 1.3 */
    NvM_ReadStaticBlockID(P2CONST(uint8, AUTOMATIC, NVM_APPL_DATA) Src, NvM_BlockCRCType crctype);
static FUNC(void, NVM_CODE) NvM_MemCpyStep(void);
static FUNC(void, NVM_CODE) NvM_WriteSequenceForManagementType(void);
static FUNC(void, NVM_CODE) NVM_GetRepeatMirrorOperation(void);
/* PRQA S 0661-- */ /* MISRA Rule 1.3 */
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
static FUNC(void, NVM_CODE)
    NvM_WriteStaticBlockID(P2VAR(uint8, AUTOMATIC, NVM_APPL_DATA) Dest, NvM_BlockCRCType crctype);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
static FUNC(void, NVM_CODE)
    NvM_ReadErrorHandle(NvM_CrcStaticIdStatusType CRCResult, NvM_CrcStaticIdStatusType StaticIDResult);
/* PRQA S 0661 */ /* MISRA Rule 1.3 */
static FUNC(void, NVM_CODE) NvM_MultiJobResultFeedBack(NvM_ServiceIdType ServiceId, NvM_RequestResultType Result);
/* PRQA S 0661++ */ /* MISRA Rule 1.3 */
static FUNC(void, NVM_CODE) NvM_SingleJobResultFeedBack(NvM_BlockIdType BlockId, NvM_RequestResultType Result);
static FUNC(void, NVM_CODE) NvM_GetRamAddress(uint8 QueueIndex);
static FUNC(Std_ReturnType, NVM_CODE) NvM_MultiJobDispatch(void);
static FUNC(void, NVM_CODE) NvM_RunTimeHandle(void);
static FUNC(void, NVM_CODE) NvM_CopyCurBlockInfo(void);
static FUNC(void, NVM_CODE) NvM_MemIfReadOk(void);
static FUNC(void, NVM_CODE) NvM_MemIfReadFailed(void);
static FUNC(void, NVM_CODE) NvM_MemIfWriteOk(void);
static FUNC(void, NVM_CODE) NvM_MemIfWriteFailed(void);
static FUNC(void, NVM_CODE) NvM_MemIfPending(void);
static FUNC(void, NVM_CODE) NvM_MemIfCancelled(void);
static FUNC(void, NVM_CODE) NvM_MemIfInconsistent(void);
static FUNC(void, NVM_CODE) NvM_MemIfInvalid(void);
/* PRQA S 0661-- */ /* MISRA Rule 1.3 */

/* PRQA S 3432,0661++ */ /* MISRA Rule 20.7,Rule 1.3 */
static FUNC(Std_ReturnType, NVM_CODE)
    NvM_MemCpy(P2CONST(uint8, AUTOMATIC, NVM_VAR) SrcAddrPtr, P2VAR(uint8, AUTOMATIC, NVM_VAR) DstAddrPtr);
/* PRQA S 3432,0661-- */ /* MISRA Rule 20.7,Rule 1.3 */

#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
/* PRQA S 0661++ */ /* MISRA Rule 1.3 */
#if (STD_ON == NVM_JOB_PRIORITIZATION)
static FUNC(void, NVM_CODE) NvM_MemIfEraseOk(void);
static FUNC(void, NVM_CODE) NvM_MemIfEraseFailed(void);
#endif
static FUNC(void, NVM_CODE) NvM_MemIfInvalidOk(void);
static FUNC(void, NVM_CODE) NvM_MemIfInvalidFailed(void);
/* PRQA S 0661-- */ /* MISRA Rule 1.3 */
#endif

#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
#if (STD_ON == NVM_JOB_PRIORITIZATION)
/* PRQA S 3432,0661++ */ /* MISRA Rule 20.7,Rule 1.3 */
static FUNC(Std_ReturnType, NVM_CODE)
    NvM_Get16Bits1stSettedBit(uint16 Number, P2VAR(uint8, AUTOMATIC, NVM_VAR) SettedBitPtr);
static FUNC(Std_ReturnType, NVM_CODE) NvM_GetHighestPriJob(P2VAR(uint8, AUTOMATIC, NVM_VAR) JobQueueIndexPtr);
static FUNC(Std_ReturnType, NVM_CODE)
    NvM_GetQueueSpace(NvM_JobTypeType JobType, P2VAR(uint8, AUTOMATIC, NVM_VAR) JobQueueIndexPtr);
/* PRQA S 3432,0661-- */ /* MISRA Rule 20.7,Rule 1.3 */

/* PRQA S 0661++ */ /* MISRA Rule 1.3 */
static FUNC(void, NVM_CODE) NvM_RelQueueSpace(NvM_JobTypeType JobType, uint8 JobQueueIndex);
/* PRQA S 0661-- */ /* MISRA Rule 1.3 */
#endif
#endif
static FUNC(void, NVM_CODE) NvM_AtomJobReq(NvM_AtomJobEType AtomJobReqId);
/* PRQA S 0661++ */ /* MISRA Rule 1.3 */
static FUNC(void, NVM_CODE) NvM_JobOverSetFlag(NvM_RequestResultType SingleReqResult, NvM_CrcChangeType CrcChange);
/* PRQA S 0661-- */ /* MISRA Rule 1.3 */
static FUNC(void, NVM_CODE) NvM_UpdateValidandChangeStatus(uint8 ValidStatus, uint8 ChangeStatus);
#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/*====================[V A R I A N T S]====================*/
#define NVM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "NvM_MemMap.h"
VAR(NvM_ModuleType, NVM_VAR_POWER_ON_INIT)
NvM_Module = {
    /* PRQA S 3334 */ /* MISRA Rule 5.3 */
    STD_OFF,
#if ((NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS) && (STD_ON == NVM_JOB_PRIORITIZATION))
    0,
#endif
    NVM_JOB_TYPE_NONE,
    NVM_RUNTIME_NONE,
    NVM_MEMIF_JOB_IDLE,
    NVM_JOB_STEP_IDLE};

VAR(NvM_MultiJobType, NVM_VAR_POWER_ON_INIT)
NvM_MultiJob = {
    /* PRQA S 3334 */ /* MISRA Rule 5.3 */
    STD_OFF,
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
    NVM_NONE_SERV_ID,
#if (STD_ON == NVM_JOB_PRIORITIZATION)
    1,
#endif
#endif
    STD_OFF,
    STD_OFF,
    NVM_REQ_OK};

VAR(NvM_AtomJobType, NVM_VAR_POWER_ON_INIT)
NvM_AtomJob = {
    /* PRQA S 3334 */ /* MISRA Rule 5.3 */
    0,
    {NVM_ATOMJOB_MAX_NONE, NVM_ATOMJOB_MAX_NONE, NVM_ATOMJOB_MAX_NONE, NVM_ATOMJOB_MAX_NONE, NVM_ATOMJOB_MAX_NONE}};

VAR(NvM_CurRunningType, NVM_VAR_POWER_ON_INIT)
NvM_CurRunning = {0,         0,        0,          0,          NVM_NONE_SERV_ID,
                  0,         0,        0,          0,          0,
                  0,         0,        NVM_REQ_OK, NVM_REQ_OK, NVM_BLOCK_NATIVE,
                  NVM_CRC16, 0,        0,          0,          0,
                  NULL_PTR,  NULL_PTR, 0,          NULL_PTR,   0,
                  0,         0,        NULL_PTR,   NULL_PTR,   NULL_PTR,
                  NULL_PTR};

#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
/* PRQA S 3334++ */ /* MISRA Rule 5.3 */
#if (STD_ON == NVM_JOB_PRIORITIZATION)
VAR(NvM_RoundRobinQueueManageType, NVM_VAR_POWER_ON_INIT) NvM_ImmedQueueManage = {0x00u, 0x00u, 0x00u};
#else
VAR(NvM_RoundRobinQueueManageType, NVM_VAR_POWER_ON_INIT) NvM_StandQueueManage = {0x00u, 0x00u, 0x00u};
#endif
/* PRQA S 3334-- */ /* MISRA Rule 5.3 */
#endif
#define NVM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "NvM_MemMap.h"

#define NVM_START_SEC_CONST_UNSPECIFIED
#include "NvM_MemMap.h"
#if (NVM_API_CONFIG_CLASS_3 != NVM_API_CONFIG_CLASS)
static CONST(NvM_VoidFuncVoidPtr, NVM_CONST) NvM_MemIfAsyncFuncTable[6][3] =
#elif (STD_ON == NVM_JOB_PRIORITIZATION)
static CONST(NvM_VoidFuncVoidPtr, NVM_CONST) NvM_MemIfAsyncFuncTable[6][6] =
#else
static CONST(NvM_VoidFuncVoidPtr, NVM_CONST) NvM_MemIfAsyncFuncTable[6][6] =
#endif
    {{
         &NvM_MemIfReadOk,  /*Read*/
         &NvM_MemIfWriteOk, /*Write*/
         NULL_PTR,          /*Restore default*/
#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
#if (STD_ON == NVM_JOB_PRIORITIZATION)
         &NvM_MemIfEraseOk, /*Erase*/
#else
         NULL_PTR, /*Erase*/
#endif
         NULL_PTR,           /*Cancel*/
         &NvM_MemIfInvalidOk /*Invalidate*/
#endif
     },
     {&NvM_MemIfReadFailed,
      &NvM_MemIfWriteFailed,
      NULL_PTR,
#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
#if (STD_ON == NVM_JOB_PRIORITIZATION)
      &NvM_MemIfEraseFailed,
#else
      NULL_PTR, /*Erase*/
#endif
      NULL_PTR,
      &NvM_MemIfInvalidFailed
#endif
     },
     {&NvM_MemIfPending,
      &NvM_MemIfPending,
      NULL_PTR,
#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
#if (STD_ON == NVM_JOB_PRIORITIZATION)
      &NvM_MemIfPending,
#else
      NULL_PTR, /*Erase*/
#endif
      NULL_PTR,
      &NvM_MemIfPending
#endif
     },
     {&NvM_MemIfCancelled,
      &NvM_MemIfCancelled,
      NULL_PTR,
#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
#if (STD_ON == NVM_JOB_PRIORITIZATION)
      &NvM_MemIfCancelled,
#else
      NULL_PTR, /*Erase*/
#endif
      NULL_PTR,
      &NvM_MemIfCancelled
#endif
     },
     {&NvM_MemIfInconsistent,
      &NvM_MemIfInconsistent,
      NULL_PTR,
#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
#if (STD_ON == NVM_JOB_PRIORITIZATION)
      &NvM_MemIfInconsistent,
#else
      NULL_PTR, /*Erase*/
#endif
      NULL_PTR,
      &NvM_MemIfInconsistent
#endif
     },
     {&NvM_MemIfInvalid,
      &NvM_MemIfInvalid,
      NULL_PTR,
#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
#if (STD_ON == NVM_JOB_PRIORITIZATION)
      &NvM_MemIfInvalid,
#else
      NULL_PTR, /*Erase*/
#endif
      NULL_PTR,
      &NvM_MemIfInvalid
#endif
     }};

#define NVM_STOP_SEC_CONST_UNSPECIFIED
#include "NvM_MemMap.h"

#if (NVM_MAX_LENGTH_CONFIGED_RAM_MIRROR > 0)
#define NVM_START_SEC_VAR_CLEARED_8
#include "NvM_MemMap.h"
/*PRQA S 1514,3334++*/ /* MISRA Rule 8.9,Rule 5.3 */
VAR(uint8, NVM_VAR_NOINIT) NVM_RamMirror[NVM_MAX_LENGTH_CONFIGED_RAM_MIRROR];
/*PRQA S 1514,3334--*/ /* MISRA Rule 8.9,Rule 5.3 */
#define NVM_STOP_SEC_VAR_CLEARED_8
#include "NvM_MemMap.h"
#endif

#define NVM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "NvM_MemMap.h"
/* PRQA S 3334++ */ /* MISRA Rule 5.3 */
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
#if (STD_ON == NVM_JOB_PRIORITIZATION)
VAR(NvM_PriTable2QueueType, NVM_VAR_NOINIT) NvM_PriTable2Queue[2][NVM_TOTAL_NUM_DIFF_PRI];
VAR(NvM_ImmedQueueType, NVM_VAR_NOINIT) NvM_ImmedQueue[NVM_SIZE_IMMEDIATE_JOB_QUEUE];
#endif
VAR(NvM_StandQueueType, NVM_VAR_NOINIT) NvM_StandQueue[NVM_SIZE_STANDARD_JOB_QUEUE];
#endif
VAR(NvM_AdminBlockType, NVM_VAR) NvM_AdminBlock[NVM_BLOCK_NUM_ALL];
/* PRQA S 3334 -- */ /* MISRA Rule 5.3 */
#define NVM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "NvM_MemMap.h"

#if ((NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS) && (STD_ON == NVM_JOB_PRIORITIZATION))
#define NVM_START_SEC_VAR_CLEARED_16
#include "NvM_MemMap.h"
/* PRQA S 3334++ */ /* MISRA Rule 5.3 */
VAR(uint16, NVM_VAR) NvM_QueueSpaceTalbe[NVM_TABLE_SIZE_JOB_QUEUE];
/* PRQA S 3334-- */ /* MISRA Rule 5.3 */
#define NVM_STOP_SEC_VAR_CLEARED_16
#include "NvM_MemMap.h"
#endif

#define NVM_START_SEC_VAR_CLEARED_8
#include "NvM_MemMap.h"
static VAR(uint8, NVM_VAR_NOINIT) NvM_NvDataBuffer[NVM_MAX_LENGTH_NV_BLOCK + NVM_NV_CRC_MAX_LENGTH];
static VAR(uint8, NVM_VAR_NOINIT) NVM_TemporaryRAMForRepaire[NVM_REDUNDANT_ALL][NVM_MAX_LENGTH_REDUNDANT_BLOCK];
#define NVM_STOP_SEC_VAR_CLEARED_8
#include "NvM_MemMap.h"

/*====================[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

/*
 * Brief               MemIf Req Sub Deal
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/* PRQA S 0709,0554++ */ /* MISRA Rule 1.1,Rule 1.1 */
static void NvM_MemIfReqSubDeal(NvM_MemIfReqType NvM_MemIfRequest)
/* PRQA S 0709,0554-- */ /* MISRA Rule 1.1,Rule 1.1 */
{
    uint16 l_BlockNum;
    uint16 l_Length;
    uint8 StaicBlockIdCheck;
    NvM_BlockIdType CurBlockId = 0;

    l_BlockNum = (uint16)(NvM_CurRunning.BaseNumber << NVM_DATASET_SELECTION_BITS) + (uint16)NvM_CurRunning.Index;
    switch (NvM_MemIfRequest)
    {
    case NVM_MEMIF_REQ_READ:
        if (NVM_CRC16 == NvM_CurRunning.CRCType)
        {
            l_Length = NvM_CurRunning.Length + 2U;
        }
        else if (NVM_CRC32 == NvM_CurRunning.CRCType)
        {
            l_Length = NvM_CurRunning.Length + 4U;
        }
        else if (NVM_CRC8 == NvM_CurRunning.CRCType)
        {
            l_Length = NvM_CurRunning.Length + 1U;
        }
        else
        {
            l_Length = NvM_CurRunning.Length; /*no crc*/
        }
        if (NvM_CurRunning.BlockId >= 1u)
        {
            CurBlockId = NvM_CurRunning.BlockId - 1U;
        }
        StaicBlockIdCheck =
            NvM_GetWordBitState(NvM_BlockDescriptor[CurBlockId].FlagGroup, (uint8)NVM_BLOCK_DESC_STATICBLOCKIDCHECK);
        if ((uint8)STD_ON == StaicBlockIdCheck)
        {
            l_Length = l_Length + 2U; /* PRQA S 2962 */ /* MISRA Rule 9.1 */
        }
        if ((Std_ReturnType)E_OK
            == MemIf_Read(
                NvM_CurRunning.DeviceId,
                l_BlockNum,
                0,
                NvM_NvDataBuffer,
                l_Length)) /* PRQA S 2962 */ /* MISRA Rule 9.1 */
        {
            NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_PENDING;
        }
        else
        {
            NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_REMAIN);
        }
        break;
    case NVM_MEMIF_REQ_WRITE:
        if ((Std_ReturnType)E_OK == MemIf_Write(NvM_CurRunning.DeviceId, l_BlockNum, NvM_NvDataBuffer))
        {
            NvM_CurRunning.WriteTimesCounter--;
            NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_PENDING;
        }
        else
        {
            NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_REMAIN);
        }
        break;
    case NVM_MEMIF_REQ_ERASE:
        if ((Std_ReturnType)E_OK == MemIf_EraseImmediateBlock(NvM_CurRunning.DeviceId, l_BlockNum))
        {
            NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_PENDING;
        }
        else
        {
            NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_REMAIN);
        }
        break;
    case NVM_MEMIF_REQ_INVALID:
        if ((Std_ReturnType)E_OK == MemIf_InvalidateBlock(NvM_CurRunning.DeviceId, l_BlockNum))
        {
            NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_PENDING;
        }
        else
        {
            NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_REMAIN);
        }
        break;
    default:
        /*idle*/
        break;
    }
}

/*
 * Brief               MemIf Req
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
#if ((NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS) && (STD_ON == NVM_JOB_PRIORITIZATION))
/* PRQA S 0709,0554++ */ /* MISRA Rule 1.1,Rule 1.1 */
static void NvM_MemIfReq(NvM_MemIfReqType NvM_MemIfRequest)
/* PRQA S 0709,0554-- */ /* MISRA Rule 1.1,Rule 1.1 */
{
    if (NVM_MEMIF_JOB_ASYNC_READY == NvM_Module.MemIfJobState)
    {
        MemIf_StatusType LowerStatus = MemIf_GetStatus(NvM_CurRunning.DeviceId);
        if (MEMIF_IDLE == LowerStatus)
        {
            NvM_MemIfReqSubDeal(NvM_MemIfRequest);
        }
        else if (MEMIF_UNINIT == LowerStatus)
        {
            NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_REMAIN);
        }
        else
        {
            /*idle*/
        }
    }
    if (NVM_JOB_STEP_CANCEL == NvM_Module.JobStep)
    {
        NvM_JobOverSetFlag(NVM_REQ_CANCELED, NVM_CRC_DELETE);
    }
}
#else
static void NvM_MemIfReq(NvM_MemIfReqType NvM_MemIfRequest)
{
    if (NVM_MEMIF_JOB_ASYNC_READY == NvM_Module.MemIfJobState)
    {
        if (MEMIF_IDLE == MemIf_GetStatus(NvM_CurRunning.DeviceId))
        {
            NvM_MemIfReqSubDeal(NvM_MemIfRequest);
        }
    }
}
#endif

/*
 * Brief               Set one bit of byte to 0 or 1
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/* PRQA S 3432,0709,0777,3334++ */ /* MISRA Rule 20.7,Rule 1.1,Rule 1.3,Rule 5.1,Rule 5.3 */
FUNC(void, NVM_CODE) NvM_SetWordBitState(P2VAR(uint16, AUTOMATIC, NVM_VAR) WordPtr, uint8 SetBit, uint8 OnOff)
/* PRQA S 3432,0709,0777,3334-- */ /* MISRA Rule 20.7,Rule 1.1,Rule 1.3,Rule 5.1,Rule 5.3 */
{
    if ((uint8)STD_ON == OnOff)
    {
        (*WordPtr) |= (uint16)((uint16)0x01U << SetBit);
    }
    else
    {
        (*WordPtr) &= (uint16)(~(uint16)((uint16)0x01U << SetBit));
    }
}

/*
 * Brief               Get the specfied bit status of byte
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/* PRQA S 0777,3334,0709++ */ /* MISRA Rule 1.3,Rule 5.1,Rule 5.3,Rule 1.1 */
FUNC(uint8, NVM_CODE)
NvM_GetWordBitState(uint16 WordNum, uint8 GetBit)
/* PRQA S 0777,3334,0709-- */ /* MISRA Rule 1.3,Rule 5.1,Rule 5.3,Rule 1.1 */
{
    uint8 l_Number = 0U;

    if (0U != (WordNum & (uint16)((uint16)0x01U << GetBit)))
    {
        l_Number = 1;
    }
    return l_Number;
}

/*
 * Brief               Copy specified length data from one memory to another
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
static FUNC(Std_ReturnType, NVM_CODE)
    NvM_MemCpy(P2CONST(uint8, AUTOMATIC, NVM_VAR) SrcAddrPtr, P2VAR(uint8, AUTOMATIC, NVM_VAR) DstAddrPtr)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint32 l_LengthLoop = 0;
    Std_ReturnType l_ReturnValue = E_NOT_OK;
    uint8 MirrorOrNot;

    if ((SrcAddrPtr != NULL_PTR) && (DstAddrPtr != NULL_PTR))
    {
        MirrorOrNot =
            NvM_GetWordBitState(NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].FlagGroup, NVM_ADMIN_RAMMIRROR_OR_NOT);
        if ((uint8)STD_ON == MirrorOrNot)
        {
            NvM_CurRunning.RepeatMirrorCounter--;
#if (STD_ON == NVM_SET_RAM_BLOCK_STATUS_API)
            if ((NVM_WRITE_BLOCK_SERV_ID == NvM_CurRunning.ServiceId)
                || (NVM_WRITE_PRAM_BLOCK_SERV_ID == NvM_CurRunning.ServiceId)
                || (NVM_WRITE_ALL_SERV_ID == NvM_CurRunning.ServiceId)
                || (NVM_SET_RAM_BLOCK_STATUS_SERV_ID == NvM_CurRunning.ServiceId)
                || (NVM_VALIDATE_ALL_SERV_ID == NvM_CurRunning.ServiceId))
#else
            if ((NVM_WRITE_BLOCK_SERV_ID == NvM_CurRunning.ServiceId)
                || (NVM_WRITE_PRAM_BLOCK_SERV_ID == NvM_CurRunning.ServiceId)
                || (NVM_WRITE_ALL_SERV_ID == NvM_CurRunning.ServiceId))
#endif
            {
                if ((Std_ReturnType)E_OK
                    == (*NvM_CurRunning.NvM_WriteRamBlockToNvm)(
                        (void*)SrcAddrPtr)) /*PRQA S 0311*/ /* MISRA Rule 11.8 */ /* NVM_MEMCPY_001 */
                {
                    /* Parameter is ok, copy data*/
                    for (; l_LengthLoop < NvM_CurRunning.Length; l_LengthLoop++)
                    {
                        DstAddrPtr[l_LengthLoop] = SrcAddrPtr[l_LengthLoop];
                    }
                    l_ReturnValue = E_OK;
                    if ((NVM_SET_RAM_BLOCK_STATUS_SERV_ID != NvM_CurRunning.ServiceId)
                        && (NVM_VALIDATE_ALL_SERV_ID != NvM_CurRunning.ServiceId))
                    {
                        /*Confirm that using the RAM mirror, so the RAM status is used*/
                        NvM_SetWordBitState(
                            &NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].FlagGroup,
                            NVM_ADMIN_RAM_VALID,
                            STD_ON);
                        NvM_SetWordBitState(
                            &NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].FlagGroup,
                            NVM_ADMIN_RAM_CHANGED,
                            STD_ON);
                        NvM_UpdateValidandChangeStatus(STD_ON, STD_ON);
                        /* req NVM303*/
                    }
                }
                else
                {
                    l_ReturnValue = E_NOT_OK;
                }
            }
            /***********************************/
            else if (
                (NVM_READ_BLOCK_SERV_ID == NvM_CurRunning.ServiceId)
                || (NVM_READ_ALL_SERV_ID == NvM_CurRunning.ServiceId)
                || (NVM_READ_PRAM_BLOCK_SERV_ID == NvM_CurRunning.ServiceId)
                || (NvM_Module.JobStep == NVM_JOB_STEP_READ_ROM))
            {
                /* Parameter is ok, copy data*/
                for (; l_LengthLoop < NvM_CurRunning.Length; l_LengthLoop++)
                {
                    DstAddrPtr[l_LengthLoop] = SrcAddrPtr[l_LengthLoop];
                }
                if (NVM_JOB_STEP_READ_ROM != NvM_Module.JobStep)
                {
                    NvM_SetWordBitState(
                        &NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].FlagGroup,
                        NVM_ADMIN_RAM_VALID,
                        STD_OFF);
                    NvM_SetWordBitState(
                        &NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].FlagGroup,
                        NVM_ADMIN_RAM_CHANGED,
                        STD_OFF);
                    NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
                }
                l_ReturnValue = (*NvM_CurRunning.NvM_ReadRamBlockFromNvm)((void*)DstAddrPtr);
            }
            else
            {
                /*idle*/
            }
        }
        else
        {
            /* Parameter is ok, copy data*/
            for (; l_LengthLoop < NvM_CurRunning.Length; l_LengthLoop++)
            {
                DstAddrPtr[l_LengthLoop] = SrcAddrPtr[l_LengthLoop];
            }
            l_ReturnValue = E_OK;
        }
    }
    return l_ReturnValue;
}

/*
 * Brief               Send an atom job request
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/* PRQA S 0709++ */ /* MISRA Rule 1.1 */
static FUNC(void, NVM_CODE) NvM_AtomJobReq(NvM_AtomJobEType AtomJobReqId)
/* PRQA S 0709-- */ /* MISRA Rule 1.1 */
{
    NvM_AtomJob.Count++;
    NvM_AtomJob.ReqId[NvM_AtomJob.Count] = AtomJobReqId;
}

/*
 * Brief               Detect module runtime type
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
static FUNC(void, NVM_CODE) NvM_RunTimeHandle(void) /* PRQA S 0709 */ /* MISRA Rule 1.1 */
{

#if (STD_OFF == NVM_DYNAMIC_CONFIGURATION)
    NvM_Module.RuntimeType = NVM_RUNTIME_NORMAL;
#else
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    if (((NVM_REQ_OK != NvM_AdminBlock[NVM_READALL_FIRST_BLOCKID - 1U].SingleReqResult)
         && (NVM_REQ_PENDING != NvM_AdminBlock[NVM_READALL_FIRST_BLOCKID - 1U].SingleReqResult))
        || (((((uint16)NVM_COMPILED_CONFIG_ID) & 0x00ffU)
             != *(P2VAR(uint8, AUTOMATIC, NVM_CONST))(
                 NvM_BlockDescriptor[NVM_READALL_FIRST_BLOCKID - 1U].NvmRamBlockDataAddress))
            || (((((uint16)NVM_COMPILED_CONFIG_ID) >> 8U) & 0x00ffU)
                != *(P2VAR(uint8, AUTOMATIC, NVM_CONST))(
                    NvM_BlockDescriptor[NVM_READALL_FIRST_BLOCKID - 1U].NvmRamBlockDataAddress + 1U))))
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    {
        /* the runtime need more information to charge*/
        NvM_Module.RuntimeType = NVM_RUNTIME_EXTENDED;
        NvM_MultiJob.ID1WriteMark = STD_ON;
    }
    else
    {
        NvM_Module.RuntimeType = NVM_RUNTIME_NORMAL;
    }
#endif
}

/*
 * Brief               Copy current block infomation to global variant
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
static FUNC(void, NVM_CODE) NvM_CopyCurBlockInfo(void) /* PRQA S 0709 */ /* MISRA Rule 1.1 */
{
    uint8 SyncMechanism;
    NvM_BlockIdType CurBlockId = NvM_CurRunning.BlockId - 1U;
    NvM_CurRunning.ManagementType = NvM_BlockDescriptor[CurBlockId].NvmBlockManagementType;
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
    if (NVM_BLOCK_DATASET != NvM_CurRunning.ManagementType)
    {
        NvM_AdminBlock[CurBlockId].CurrentIndex = 0;
    }
#else
    NvM_AdminBlock[CurBlockId].CurrentIndex = 0;
#endif
    /***********************************/
    NVM_GetRepeatMirrorOperation();
    NvM_CurRunning.Index = NvM_AdminBlock[CurBlockId].CurrentIndex;
    NvM_CurRunning.StaticId = 0xFFFF;
    NvM_CurRunning.AdminFlagGroup = NvM_AdminBlock[CurBlockId].FlagGroup;
    NvM_CurRunning.Crc = NvM_AdminBlock[CurBlockId].Crc;
    NvM_CurRunning.ESingleReqResult = NvM_AdminBlock[CurBlockId].SingleReqResult;
    NvM_CurRunning.WriteTimesCounter = NvM_BlockDescriptor[CurBlockId].NvMMaxNumOfWriteRetries + 1U; /*req 659*/
    NvM_CurRunning.ReadRetryCounter = NvM_BlockDescriptor[CurBlockId].NvMMaxNumOfReadRetries;        /*req 533*/
    NvM_CurRunning.VerificationSize = NvM_BlockDescriptor[CurBlockId].NvMWriteVerificationDataSize;
    NvM_CurRunning.VerificationCounter = 0;
    NvM_CurRunning.DeviceId = NvM_BlockDescriptor[CurBlockId].NvmNvramDeviceId;
    NvM_CurRunning.NvNum = NvM_BlockDescriptor[CurBlockId].NvmNvBlockNum;
    NvM_CurRunning.RomNum = NvM_BlockDescriptor[CurBlockId].NvmRomBlockNum;
    NvM_CurRunning.CRCType = NvM_BlockDescriptor[CurBlockId].NvmBlockCRCType;
    NvM_CurRunning.BaseNumber = NvM_BlockDescriptor[CurBlockId].NvmNvBlockBaseNumber;
    NvM_CurRunning.Length = NvM_BlockDescriptor[CurBlockId].NvmNvBlockLength;
#if (0xFFFFU != NVM_CRC_NUM_OF_BYTES)
    NvM_CurRunning.CrcFlag = 0;
#endif /* 0xFFFFU != NVM_CRC_NUM_OF_BYTES */
    /***********************************/
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
    if (NVM_BLOCK_DATASET != NvM_CurRunning.ManagementType)
    {
        NvM_CurRunning.RomAddr = NvM_BlockDescriptor[CurBlockId].NvmRomBlockDataAddress;
    }
    else if (NvM_CurRunning.NvNum <= NvM_CurRunning.Index)
    {
        NvM_CurRunning.RomAddr = &NvM_BlockDescriptor[CurBlockId].NvmRomBlockDataAddress[(
            (NvM_CurRunning.Index - NvM_CurRunning.NvNum) * (uint8)NvM_CurRunning.Length)];
    }
    else
    {
        /* None will not happen */
    }
#else
    NvM_CurRunning.RomAddr = NvM_BlockDescriptor[CurBlockId].NvmRomBlockDataAddress;
#endif
    SyncMechanism = NvM_GetWordBitState(NvM_BlockDescriptor[CurBlockId].FlagGroup, (uint8)NVM_BLOCK_DESC_SYNCMECHANISM);
    if ((uint8)STD_ON == SyncMechanism)
    {
        NvM_CurRunning.NvM_WriteRamBlockToNvm = NvM_BlockDescriptor[CurBlockId].NvM_WriteRamBlockToNvm;
        NvM_CurRunning.NvM_ReadRamBlockFromNvm = NvM_BlockDescriptor[CurBlockId].NvM_ReadRamBlockFromNvm;
    }
    /***********************************/

#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
    NvM_CurRunning.InitCallback = NvM_BlockDescriptor[CurBlockId].NvmInitBlockCallback;
#endif
    NvM_CurRunning.SingleCallback = NvM_BlockDescriptor[CurBlockId].NvmSingleBlockCallback;
}

/*
 * Brief               Multiple type job dispatch
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
#if ((NVM_API_CONFIG_CLASS_1 == NVM_API_CONFIG_CLASS) || (STD_OFF == NVM_JOB_PRIORITIZATION))
static FUNC(Std_ReturnType, NVM_CODE) NvM_MultiJobDispatch(void)
{
    Std_ReturnType l_ReturnValue = E_NOT_OK;

    if (NVM_READ_ALL_SERV_ID == NvM_CurRunning.ServiceId)
    {
        NvM_CurRunning.BlockId++;
        if ((uint16)NVM_BLOCK_NUM_ALL < NvM_CurRunning.BlockId)
        {
#if (STD_ON == NVM_DRV_MODE_SWITCH)
            /*switch the low layer process mode to slow mode after job over*/
            MemIf_SetMode(MEMIF_MODE_SLOW);
#endif
            NvM_MultiJob.Enqueue = STD_OFF;
            if (NVM_REQ_NOT_OK == NvM_CurRunning.EMultiReqResult)
            {
                NvM_MultiJobResultFeedBack(NvM_CurRunning.ServiceId, NVM_REQ_NOT_OK);
            }
            else
            {
                NvM_MultiJobResultFeedBack(NvM_CurRunning.ServiceId, NVM_REQ_OK);
            }
            NvM_CurRunning.EMultiReqResult = NVM_REQ_OK;
            NvM_Module.CurrentJobType = NVM_JOB_TYPE_NONE;
        }
        else
        {
            NvM_CurRunning.ServiceId = NVM_READ_ALL_SERV_ID;
            NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].ServiceID = NVM_READ_ALL_SERV_ID;
            NvM_GetRamAddress(USELESS_FOR_MULTIJOB);
            NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].SingleReqResult = NVM_REQ_PENDING;
            NvM_CopyCurBlockInfo();
            l_ReturnValue = E_OK;
        }
    }
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
    else if (NVM_VALIDATE_ALL_SERV_ID == NvM_CurRunning.ServiceId)
    {
        NvM_CurRunning.BlockId++;
        if ((uint16)NVM_BLOCK_NUM_ALL < NvM_CurRunning.BlockId)
        {
            NvM_MultiJob.Enqueue = STD_OFF;
            if (NVM_REQ_NOT_OK == NvM_CurRunning.EMultiReqResult)
            {
                NvM_MultiJobResultFeedBack(NvM_CurRunning.ServiceId, NVM_REQ_NOT_OK);
            }
            else
            {
                NvM_MultiJobResultFeedBack(NvM_CurRunning.ServiceId, NVM_REQ_OK);
            }
            NvM_CurRunning.EMultiReqResult = NVM_REQ_OK;
            NvM_Module.CurrentJobType = NVM_JOB_TYPE_NONE;
        }
        else
        {
            NvM_CurRunning.ServiceId = NVM_VALIDATE_ALL_SERV_ID;
            NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].ServiceID = NVM_VALIDATE_ALL_SERV_ID;
            NvM_GetRamAddress(USELESS_FOR_MULTIJOB);
            NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].SingleReqResult = NVM_REQ_PENDING;
            NvM_CopyCurBlockInfo();
            l_ReturnValue = E_OK;
        }
    }
#endif
    else
    {
        NvM_CurRunning.BlockId++;
        if ((NVM_WRITEALL_FIRST_BLOCKID == NvM_CurRunning.BlockId)
            || (NVM_SET_RAM_BLOCK_STATUS_SERV_ID == NvM_CurRunning.ServiceId)
            || (((uint16)NVM_BLOCK_NUM_ALL < NvM_CurRunning.BlockId) && ((uint8)STD_OFF == NvM_MultiJob.ID1WriteMark)))
        {
#if (STD_ON == NVM_DRV_MODE_SWITCH)
            /*switch the low layer process mode to slow mode after job over*/
            MemIf_SetMode(MEMIF_MODE_SLOW);
#endif
            NvM_MultiJob.Enqueue = STD_OFF;
            if (NVM_REQ_NOT_OK == NvM_CurRunning.EMultiReqResult)
            {
                NvM_MultiJobResultFeedBack(NvM_CurRunning.ServiceId, NVM_REQ_NOT_OK);
            }
            else
            {
                NvM_MultiJobResultFeedBack(NvM_CurRunning.ServiceId, NVM_REQ_OK);
            }
            NvM_CurRunning.EMultiReqResult = NVM_REQ_OK;
            NvM_Module.CurrentJobType = NVM_JOB_TYPE_NONE;
        }
        else if (((uint16)NVM_BLOCK_NUM_ALL < NvM_CurRunning.BlockId) && ((uint8)STD_ON == NvM_MultiJob.ID1WriteMark))
        {
            /* pending the job and clear the write flag */
            NvM_CurRunning.BlockId = 1;
            NvM_MultiJob.ID1WriteMark = STD_OFF;

            NvM_CurRunning.ServiceId = NVM_WRITE_ALL_SERV_ID;
            NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].ServiceID = NVM_WRITE_ALL_SERV_ID;

            NvM_GetRamAddress(USELESS_FOR_MULTIJOB);
            *(NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].NvmRamBlockDataAddress) =
                (uint8)(((uint16)(NVM_COMPILED_CONFIG_ID)) & 0x00ffU);
            *((NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].NvmRamBlockDataAddress) + 1U) =
                (uint8)(((uint16)(NVM_COMPILED_CONFIG_ID)) >> 8U);

            NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].SingleReqResult = NVM_REQ_PENDING;
            NvM_CopyCurBlockInfo();
            l_ReturnValue = E_OK;
        }
        else
        {
            /* pending the job */
            NvM_CurRunning.ServiceId = NVM_WRITE_ALL_SERV_ID;
            /* PRQA S 2842 ++ */ /* MISRA Dir 4.1 */
            NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].ServiceID = NVM_WRITE_ALL_SERV_ID;
            /* PRQA S 2842 -- */ /* MISRA Dir 4.1 */
            NvM_GetRamAddress(USELESS_FOR_MULTIJOB);
            NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].SingleReqResult = NVM_REQ_PENDING;
            NvM_CopyCurBlockInfo();
            l_ReturnValue = E_OK;
        }
    }
    return l_ReturnValue;
}

/*
 * Brief               Put a job to the job queue to be processing
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
FUNC(Std_ReturnType, NVM_CODE)
NvM_JobEnqueue(NvM_BlockIdType BlockId, NvM_ServiceIdType ServiceId, P2VAR(uint8, AUTOMATIC, NVM_APPL_DATA) DestSrcPtr)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType l_ReturnValue = E_NOT_OK;
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
    uint16 BlockIdIndex;
    uint16 l_BlockIdLoop;
    BlockIdIndex = BlockId - 1U;
    if (NVM_SIZE_STANDARD_JOB_QUEUE > NvM_StandQueueManage.Count)
    {
        SchM_Enter_NvM_Queue();
        NvM_StandQueue[NvM_StandQueueManage.TailIndex].BlockId = BlockId;
        NvM_StandQueue[NvM_StandQueueManage.TailIndex].ServiceId = ServiceId;
        NvM_StandQueue[NvM_StandQueueManage.TailIndex].DestSrcPtr = DestSrcPtr;
        NvM_StandQueueManage.TailIndex++;
        /* protect the tailindex is overflow, when this happened, reset it*/
        if (NVM_SIZE_STANDARD_JOB_QUEUE <= NvM_StandQueueManage.TailIndex)
        {
            NvM_StandQueueManage.TailIndex = 0;
        }
        NvM_StandQueueManage.Count++;
        SchM_Exit_NvM_Queue();
        if ((NVM_READ_ALL_SERV_ID == ServiceId) || (NVM_WRITE_ALL_SERV_ID == ServiceId)
            || (NVM_VALIDATE_ALL_SERV_ID == ServiceId))
        {
            SchM_Enter_NvM_Queue();
            /* set multiple type job flag */
            NvM_MultiJob.Enqueue = STD_ON;
            NvM_MultiJob.ServiceId = ServiceId;
            SchM_Exit_NvM_Queue();
            NvM_MultiJobResultFeedBack(ServiceId, NVM_REQ_PENDING);
            if (NVM_READ_ALL_SERV_ID == ServiceId)
            {
                SchM_Enter_NvM_Queue();
                for (l_BlockIdLoop = 0; l_BlockIdLoop < (uint16)NVM_BLOCK_NUM_ALL; l_BlockIdLoop++)
                {
                    NvM_AdminBlock[l_BlockIdLoop].ReadReq = TRUE; /* req NVM952 */
                }
                SchM_Exit_NvM_Queue();
            }
        }
        else
        {
            SchM_Enter_NvM_Queue();
            /* set single type job flag */
            NvM_SetWordBitState(&NvM_AdminBlock[BlockIdIndex].FlagGroup, NVM_ADMIN_ENQUEUE, STD_ON);
            SchM_Exit_NvM_Queue();
            NvM_SingleJobResultFeedBack(BlockId, NVM_REQ_PENDING);
        }
        l_ReturnValue = E_OK;
    }
#else
    /* the api class is class 1, put job to global variant directly, no need queue */
    NvM_CurRunning.BlockId = BlockId;
    NvM_MultiJob.Enqueue = STD_ON;
    NvM_CurRunning.ServiceId = ServiceId;
    NvM_MultiJobResultFeedBack(NvM_CurRunning.ServiceId, NVM_REQ_PENDING);
    NvM_CurRunning.RamAddr = DestSrcPtr;
    l_ReturnValue = E_OK;
#endif
    return l_ReturnValue;
}

/*
 * Brief               Get a job from the job queue to process
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
FUNC(Std_ReturnType, NVM_CODE) NvM_JobDequeue(void)
{
    Std_ReturnType l_ReturnValue;
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
    uint8 l_Index;

    if (NVM_JOB_TYPE_MULTI == NvM_Module.CurrentJobType)
    {
        /* multiple type job dispatch */
        l_ReturnValue = NvM_MultiJobDispatch();
    }
    else if (0U < NvM_StandQueueManage.Count)
    {
        SchM_Enter_NvM_Queue();
        /* get the queue current head index */
        l_Index = NvM_StandQueueManage.HeadIndex;
        NvM_CurRunning.ServiceId = NvM_StandQueue[l_Index].ServiceId;
        if ((NVM_READ_ALL_SERV_ID == NvM_CurRunning.ServiceId) || (NVM_WRITE_ALL_SERV_ID == NvM_CurRunning.ServiceId)
            || (NVM_VALIDATE_ALL_SERV_ID == NvM_CurRunning.ServiceId))
        {
            NvM_Module.CurrentJobType = NVM_JOB_TYPE_MULTI;
            if (NVM_READ_ALL_SERV_ID == NvM_CurRunning.ServiceId)
            {
                NvM_CurRunning.BlockId = NVM_READALL_FIRST_BLOCKID;
            }
            else
            {
                NvM_CurRunning.BlockId = NVM_WRITEALL_FIRST_BLOCKID;
            }
            NvM_GetRamAddress(0);
            NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].SingleReqResult = NVM_REQ_PENDING;
            /*Single block from the multi job*/
#if (STD_ON == NVM_DRV_MODE_SWITCH)
            /* switch the low layer process mode to fast mode before multiple type job start */
            MemIf_SetMode(MEMIF_MODE_FAST);
#endif
        }
        else
        {
            NvM_Module.CurrentJobType = NVM_JOB_TYPE_SINGLE_NORMAL;
            NvM_CurRunning.BlockId = NvM_StandQueue[l_Index].BlockId;
            NvM_GetRamAddress(l_Index);
            NvM_SetWordBitState(&NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].FlagGroup, NVM_ADMIN_ENQUEUE, STD_OFF);
        }
        NvM_CopyCurBlockInfo();
        /* set the queue management flag */
        NvM_StandQueueManage.HeadIndex++;
        if (NVM_SIZE_STANDARD_JOB_QUEUE <= NvM_StandQueueManage.HeadIndex)
        {
            /* prevent the index overflow, when it happen, need reset it */
            NvM_StandQueueManage.HeadIndex = 0;
        }
        NvM_StandQueueManage.Count--;
        SchM_Exit_NvM_Queue();
        l_ReturnValue = E_OK;
    }
    else
    {
        /* the job queue is empty */
        l_ReturnValue = E_NOT_OK;
    }
#else
    if (NVM_JOB_TYPE_MULTI == NvM_Module.CurrentJobType)
    {
        /* multiply type job dispatch */
        l_ReturnValue = NvM_MultiJobDispatch();
    }
    else if ((uint8)STD_ON == NvM_MultiJob.Enqueue)
    {
        SchM_Enter_NvM_Queue();
        NvM_Module.CurrentJobType = NVM_JOB_TYPE_MULTI;
        if (NVM_READ_ALL_SERV_ID == NvM_CurRunning.ServiceId)
        {
            NvM_CurRunning.BlockId = NVM_READALL_FIRST_BLOCKID;
        }
        else
        {
            NvM_CurRunning.BlockId = NVM_WRITEALL_FIRST_BLOCKID;
        }
        /* multiple type job did not have temprary parameter, so using default ram address*/
        NvM_GetRamAddress(0);
        NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].SingleReqResult = NVM_REQ_PENDING;
        /*Single block from the multi job*/
        SchM_Exit_NvM_Queue();
#if (STD_ON == NVM_DRV_MODE_SWITCH)
        MemIf_SetMode(MEMIF_MODE_FAST);
#endif
        /* copy current block information to global variant to process */
        NvM_CopyCurBlockInfo();
        l_ReturnValue = E_OK;
    }
    else
    {
        /* there is no job need process or the flag set error happend */
        l_ReturnValue = E_NOT_OK;
    }
#endif
    return l_ReturnValue;
}

#endif

#if ((NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS) && (STD_ON == NVM_JOB_PRIORITIZATION))
/*
 * Brief               Find the first setted bit of a given 16 bits source number
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/* PRQA S 3432,0709++ */ /* MISRA Rule 20.7,Rule 1.1 */
static FUNC(Std_ReturnType, NVM_CODE)
    NvM_Get16Bits1stSettedBit(uint16 Number, P2VAR(uint8, AUTOMATIC, NVM_VAR) SettedBitPtr)
/* PRQA S 3432,0709-- */ /* MISRA Rule 20.7,Rule 1.1 */
{
    uint8 l_BitsLoop = 0x00u;
    uint16 l_BaseNum = 0x0001u;
    Std_ReturnType l_ReturnValue = E_OK;

    if (0x0000u == Number)
    {
        /* the number equal 0, can not find the setted bit */
        l_ReturnValue = E_NOT_OK;
    }

    /***********************************/
    if ((Std_ReturnType)E_OK == l_ReturnValue)
    {
        do
        {
            if (0u != (l_BaseNum & Number))
            {
                *SettedBitPtr = l_BitsLoop;
                break;
            }
            l_BaseNum = (uint16)(l_BaseNum << 1u);
            l_BitsLoop++;
        } while (16u > l_BitsLoop);
    }
    return l_ReturnValue;
}

/*
 * Brief               Get the available job queue space
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/* PRQA S 3432,0709++ */ /* MISRA Rule 20.7,Rule 1.1 */
static FUNC(Std_ReturnType, NVM_CODE)
    NvM_GetQueueSpace(NvM_JobTypeType JobType, P2VAR(uint8, AUTOMATIC, NVM_VAR) JobQueueIndexPtr)
/* PRQA S 3432,0709-- */ /* MISRA Rule 20.7,Rule 1.1 */
{
    uint8 l_JobQueueGroup = 0x00u;
    uint8 l_JobQueueRdyBit = 0x00u;
    uint8 l_JobQueueIndex;
    Std_ReturnType l_ReturnValue = E_OK;

    if (NVM_JOB_TYPE_SINGLE_IMMED == JobType)
    {
        if (NVM_SIZE_IMMEDIATE_JOB_QUEUE <= NvM_ImmedQueueManage.Count)
        {
            /* Job queue overflow */
            l_ReturnValue = E_NOT_OK;
        }
        else
        {
            SchM_Enter_NvM_Queue();
            *JobQueueIndexPtr = NvM_ImmedQueueManage.TailIndex;
            NvM_ImmedQueueManage.TailIndex++;
            if (NVM_SIZE_IMMEDIATE_JOB_QUEUE <= NvM_ImmedQueueManage.TailIndex)
            {
                /* prevent index overflow, reset it */
                NvM_ImmedQueueManage.TailIndex = 0;
            }
            NvM_ImmedQueueManage.Count++;
            SchM_Exit_NvM_Queue();
        }
    }
    /***********************************/
    else if (NVM_JOB_TYPE_SINGLE_NORMAL == JobType)
    {
        l_ReturnValue = NvM_Get16Bits1stSettedBit(NvM_QueueSpaceTalbe[NVM_TABLE_SIZE_JOB_QUEUE - 1U], &l_JobQueueGroup);

        if (((Std_ReturnType)E_NOT_OK == l_ReturnValue)
            || (((NVM_SIZE_STANDARD_JOB_QUEUE - 1U) >> 4U) < l_JobQueueGroup))
        {
            if ((Std_ReturnType)E_NOT_OK != l_ReturnValue)
            {
                /* Initial error happened, Correct the mistakes */
                NvM_QueueSpaceTalbe[NVM_TABLE_SIZE_JOB_QUEUE - 1U] &=
                    (uint16)(~(uint16)((uint16)0x01u << l_JobQueueGroup));
                l_ReturnValue = E_NOT_OK;
            }
            /* Job queue overflow */
        }
        /***********************************/
        else
        {
            l_ReturnValue = NvM_Get16Bits1stSettedBit(NvM_QueueSpaceTalbe[l_JobQueueGroup], &l_JobQueueRdyBit);
            l_JobQueueIndex = (uint8)(l_JobQueueGroup << 4u) + l_JobQueueRdyBit; /*PRQA S 2986*/ /* MISRA Rule 2.2 */
            if (((Std_ReturnType)E_NOT_OK == l_ReturnValue) || (NVM_SIZE_STANDARD_JOB_QUEUE <= l_JobQueueIndex))
            {
                /* Flag error happened, Correct the mistakes */
                if ((Std_ReturnType)E_NOT_OK == l_ReturnValue)
                {
                    NvM_QueueSpaceTalbe[NVM_TABLE_SIZE_JOB_QUEUE - 1U] &=
                        (uint16)(~(uint16)((uint16)0x01u << l_JobQueueGroup));
                }
                else
                {
                    /* Initial error happened, Correct the mistakes */
                    l_ReturnValue = E_NOT_OK;
                }
            }
            /***********************************/
            else if ((Std_ReturnType)E_OK == l_ReturnValue) /*PRQA S 2995,2991*/
            {
                SchM_Enter_NvM_Queue();
                *JobQueueIndexPtr = l_JobQueueIndex;
                /* clear the job queue space table, mark it as not available */

                NvM_QueueSpaceTalbe[l_JobQueueGroup] &= (uint16)(~(uint16)((uint16)0x01u << l_JobQueueRdyBit));
                if (0u == NvM_QueueSpaceTalbe[l_JobQueueGroup])
                {
                    /* the group's space are all in using, clear the group flag */
                    NvM_QueueSpaceTalbe[NVM_TABLE_SIZE_JOB_QUEUE - 1U] &=
                        (uint16)(~(uint16)((uint16)0x01u << l_JobQueueGroup));
                }
                SchM_Exit_NvM_Queue();
            }
            else
            {
                /*idle*/
            }
        }
    }
    else
    {
        /* None will not happen */
    }
    return l_ReturnValue;
}

/*
 * Brief               Release the space which indicated by the index
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
static FUNC(void, NVM_CODE) NvM_RelQueueSpace(NvM_JobTypeType JobType, uint8 JobQueueIndex)
{
    uint8 l_QueueLowBits;
    uint8 l_QueueHighBits;

    SchM_Enter_NvM_Queue();
    if ((NVM_JOB_TYPE_SINGLE_IMMED == JobType) || (NVM_JOB_TYPE_MULTI_GAP_SINGLE_IMMED == JobType))
    {
        NvM_ImmedQueueManage.HeadIndex++;
        if (NVM_SIZE_IMMEDIATE_JOB_QUEUE <= NvM_ImmedQueueManage.HeadIndex)
        {
            /* round robin queue, prvent index overflow, reset it */
            NvM_ImmedQueueManage.HeadIndex = 0;
        }
        NvM_ImmedQueueManage.Count--;
    }
    else if (NVM_JOB_TYPE_SINGLE_NORMAL == JobType)
    {
        /* set job queue table flag */

        l_QueueLowBits = JobQueueIndex & 0x0FU;
        l_QueueHighBits = (uint8)((JobQueueIndex & 0xF0U) >> 4U);
        NvM_QueueSpaceTalbe[NVM_TABLE_SIZE_JOB_QUEUE - 1U] |= (uint16)((uint16)0x01U << l_QueueHighBits);
        NvM_QueueSpaceTalbe[l_QueueHighBits] |= (uint16)((uint16)0x01U << l_QueueLowBits);
    }
    else
    {
        /* None Will not happen */
    }
    SchM_Exit_NvM_Queue();
}

/*
 * Brief               Get the highest priority job
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
static FUNC(Std_ReturnType, NVM_CODE) NvM_GetHighestPriJob(P2VAR(uint8, AUTOMATIC, NVM_VAR) JobQueueIndexPtr)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 l_PriGroup = 0x0u;
    uint8 l_PriRdyBit = 0xFFu;
    uint8 l_PriHighest;
    uint8 l_CurActPriTable;
    uint8 l_SamePriHeadIndex;
    Std_ReturnType l_ReturnValue;

    /***********************************/
    l_CurActPriTable = NvM_Module.CurActPriTable;
    l_ReturnValue =
        NvM_Get16Bits1stSettedBit(NvM_PriorityTable[l_CurActPriTable][NVM_TABLE_SIZE_PRIORITY - 1U], &l_PriGroup);
    if ((Std_ReturnType)E_OK == l_ReturnValue)
    {
        if (NVM_TABLE_SIZE_PRIORITY < (uint8)(l_PriGroup + 2u))
        {
            /* Initial error happened, Correct the mistakes */
            NvM_PriorityTable[l_CurActPriTable][NVM_TABLE_SIZE_PRIORITY - 1U] &=
                (uint16)(~(uint16)((uint16)0x01u << l_PriGroup));
            l_ReturnValue = E_NOT_OK;
        }
        else
        {
            l_ReturnValue = NvM_Get16Bits1stSettedBit(NvM_PriorityTable[l_CurActPriTable][l_PriGroup], &l_PriRdyBit);
            /***********************************/
            if ((Std_ReturnType)E_NOT_OK == l_ReturnValue)
            {
                /* Flag error happened, Correct the mistakes */
                NvM_PriorityTable[l_CurActPriTable][NVM_TABLE_SIZE_PRIORITY - 1U] &=
                    (uint16)(~(uint16)((uint16)0x01u << l_PriGroup));
            }
            else
            {
                /***********************************/
                l_PriHighest = (uint8)(l_PriGroup << 4u) + l_PriRdyBit;
                if (NVM_TOTAL_NUM_DIFF_PRI <= l_PriHighest)
                {
                    /* Flag error happened, Correct the mistakes */
                    NvM_PriorityTable[l_CurActPriTable][l_PriGroup] &=
                        (uint16)(~(uint16)((uint16)0x01u << l_PriRdyBit)); /*PRQA S 2792*/
                    l_ReturnValue = E_NOT_OK;
                }
                else
                {
                    SchM_Enter_NvM_Queue();
                    l_SamePriHeadIndex = NvM_PriTable2Queue[l_CurActPriTable][l_PriHighest].HeadIndex;
                    *JobQueueIndexPtr = l_SamePriHeadIndex;

                    /***********************************/
                    if (NvM_PriTable2Queue[l_CurActPriTable][l_PriHighest].HeadIndex
                        == NvM_PriTable2Queue[l_CurActPriTable][l_PriHighest].TailIndex)
                    {

                        /* This is the last element, clear priority table flag */
                        NvM_PriorityTable[l_CurActPriTable][l_PriGroup] &=
                            (uint16)(~(uint16)((uint16)0x01u << l_PriRdyBit));
                        if (0U == NvM_PriorityTable[l_CurActPriTable][l_PriGroup])
                        {
                            NvM_PriorityTable[l_CurActPriTable][NVM_TABLE_SIZE_PRIORITY - 1U] &=
                                (uint16)(~(uint16)((uint16)0x01u << l_PriGroup));
                        }
                    }
                    else
                    {
                        NvM_PriTable2Queue[l_CurActPriTable][l_PriHighest].HeadIndex =
                            NvM_StandQueue[l_SamePriHeadIndex].NextIndex;

                        NvM_StandQueue[l_SamePriHeadIndex].NextIndex = l_SamePriHeadIndex;
                    }
                    SchM_Exit_NvM_Queue();
                }
            }
        }
    }
    return l_ReturnValue;
}

/*
 * Brief               Multiple type job dispatch WriteAll Operation
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/* PRQA S 0554,0709++ */ /* MISRA Rule 1.1,Rule 1.1 */
static FUNC(Std_ReturnType, NVM_CODE) NvM_MultiJobDispatchWrite(void)
/* PRQA S 0554,0709-- */ /* MISRA Rule 1.1,Rule 1.1 */
{
    Std_ReturnType l_ReturnValue = E_NOT_OK;

    NvM_MultiJob.BlockId++;
    if ((2U == NvM_MultiJob.BlockId)
        || (((uint16)NVM_BLOCK_NUM_ALL < NvM_MultiJob.BlockId) && ((uint8)STD_OFF == NvM_MultiJob.ID1WriteMark)))
    {
#if (STD_ON == NVM_DRV_MODE_SWITCH)
        /* switch low layer mode to slow mode after multiple type job is processed over */
        MemIf_SetMode(MEMIF_MODE_SLOW);
#endif
        if (0U != NvM_PriorityTable[1U - NvM_Module.CurActPriTable][NVM_TABLE_SIZE_PRIORITY - 1U])
        {
            /* switch the current active priority table */
            NvM_Module.CurActPriTable = 1U - NvM_Module.CurActPriTable;
        }
        NvM_MultiJob.Enqueue = STD_OFF;
        /***********************************/
        if (NVM_REQ_NOT_OK == NvM_CurRunning.EMultiReqResult)
        {
            NvM_MultiJobResultFeedBack(NvM_MultiJob.ServiceId, NVM_REQ_NOT_OK);
        }
        else
        {
            NvM_MultiJobResultFeedBack(NvM_MultiJob.ServiceId, NVM_REQ_OK);
        }
        NvM_CurRunning.EMultiReqResult = NVM_REQ_OK;
        NvM_Module.CurrentJobType = NVM_JOB_TYPE_NONE;
    }
    /***********************************/
    else if ((uint16)NVM_BLOCK_NUM_ALL >= NvM_MultiJob.BlockId)
    {
        /* copy the current job information to global variant to process */
        NvM_Module.CurrentJobType = NVM_JOB_TYPE_MULTI;
        NvM_CurRunning.BlockId = NvM_MultiJob.BlockId;
        NvM_CurRunning.ServiceId = NvM_MultiJob.ServiceId;

        NvM_GetRamAddress(0);

        NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].SingleReqResult = NVM_REQ_PENDING;
        NvM_CopyCurBlockInfo();
        l_ReturnValue = E_OK;
    }
    /***********************************/
    /*. If Block1 should write, then Block1 should the last block to handle*/
    else if ((uint8)STD_ON == NvM_MultiJob.ID1WriteMark)
    {
        /* copy the current job information to global variant to process */
        NvM_MultiJob.BlockId = 1;
        NvM_MultiJob.ID1WriteMark = STD_OFF;
        NvM_CurRunning.BlockId = NvM_MultiJob.BlockId;
        NvM_CurRunning.ServiceId = NvM_MultiJob.ServiceId;
        NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].ServiceID = NvM_MultiJob.ServiceId;

        *(NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].NvmRamBlockDataAddress) =
            (uint8)(((uint16)(NVM_COMPILED_CONFIG_ID)) & 0x00ffU);
        *((NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].NvmRamBlockDataAddress) + 1U) =
            (uint8)(((uint16)(NVM_COMPILED_CONFIG_ID)) >> 8U);
        NvM_CurRunning.RamAddr = NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].NvmRamBlockDataAddress;
        NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].SingleReqResult = NVM_REQ_PENDING;
        NvM_CopyCurBlockInfo();
        l_ReturnValue = E_OK;
    }
    else
    {
        /*idle*/
    }
    return l_ReturnValue;
}

/*
 * Brief               Multiple type job dispatch
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
static FUNC(Std_ReturnType, NVM_CODE) NvM_MultiJobDispatch(void) /* PRQA S 0709 */ /* MISRA Rule 1.1 */
{
    Std_ReturnType l_ReturnValue = E_NOT_OK;

    if (NVM_READ_ALL_SERV_ID == NvM_MultiJob.ServiceId)
    {
        if ((NVM_JOB_TYPE_MULTI_GAP_SINGLE_IMMED != NvM_Module.CurrentJobType) || (1u != NvM_MultiJob.BlockId))
        {
            NvM_MultiJob.BlockId++;
        }

        if ((uint16)NVM_BLOCK_NUM_ALL < NvM_MultiJob.BlockId)
        {
#if (STD_ON == NVM_DRV_MODE_SWITCH)
            /* switch low layer mode to slow mode after multiple type job is processed over */
            MemIf_SetMode(MEMIF_MODE_SLOW);
#endif

            if (0u != NvM_PriorityTable[1u - NvM_Module.CurActPriTable][NVM_TABLE_SIZE_PRIORITY - 1U])
            {
                /***********************************/
                /* switch the current active priority table */
                NvM_Module.CurActPriTable = 1u - NvM_Module.CurActPriTable;
            }
            NvM_MultiJob.Enqueue = STD_OFF;
            if (NVM_REQ_NOT_OK == NvM_CurRunning.EMultiReqResult)
            {
                NvM_MultiJobResultFeedBack(NvM_MultiJob.ServiceId, NVM_REQ_NOT_OK);
            }
            else
            {
                NvM_MultiJobResultFeedBack(NvM_MultiJob.ServiceId, NVM_REQ_OK);
            }
            NvM_CurRunning.EMultiReqResult = NVM_REQ_OK;
            NvM_Module.CurrentJobType = NVM_JOB_TYPE_NONE; /*Current job been finished*/
        }
        /***********************************/
        /*. handle the next blockID for MultiJob*/
        else
        {
            NvM_Module.CurrentJobType = NVM_JOB_TYPE_MULTI;
            NvM_CurRunning.BlockId = NvM_MultiJob.BlockId;
            NvM_CurRunning.ServiceId = NvM_MultiJob.ServiceId;
            NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].ServiceID = NvM_MultiJob.ServiceId;
            NvM_GetRamAddress(0);
            NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].SingleReqResult = NVM_REQ_PENDING;
            NvM_CopyCurBlockInfo();
            l_ReturnValue = E_OK;
        }
    }
    else if (NVM_VALIDATE_ALL_SERV_ID == NvM_MultiJob.ServiceId)
    {
        if ((NVM_JOB_TYPE_MULTI_GAP_SINGLE_IMMED != NvM_Module.CurrentJobType) || (1u != NvM_MultiJob.BlockId))
        {
            NvM_MultiJob.BlockId++;
        }

        if ((uint16)NVM_BLOCK_NUM_ALL < NvM_MultiJob.BlockId)
        {
            if (0u != NvM_PriorityTable[1u - NvM_Module.CurActPriTable][NVM_TABLE_SIZE_PRIORITY - 1U])
            {
                /***********************************/
                /* switch the current active priority table */
                NvM_Module.CurActPriTable = 1u - NvM_Module.CurActPriTable;
            }
            NvM_MultiJob.Enqueue = STD_OFF;
            if (NVM_REQ_NOT_OK == NvM_CurRunning.EMultiReqResult)
            {
                NvM_MultiJobResultFeedBack(NvM_MultiJob.ServiceId, NVM_REQ_NOT_OK);
            }
            else
            {
                NvM_MultiJobResultFeedBack(NvM_MultiJob.ServiceId, NVM_REQ_OK);
            }
            NvM_CurRunning.EMultiReqResult = NVM_REQ_OK;
            NvM_Module.CurrentJobType = NVM_JOB_TYPE_NONE; /*. Current job been finished*/
        }
        /***********************************/
        /*. handle the next blockID for MultiJob*/
        else
        {
            NvM_Module.CurrentJobType = NVM_JOB_TYPE_MULTI;
            NvM_CurRunning.BlockId = NvM_MultiJob.BlockId;
            NvM_CurRunning.ServiceId = NvM_MultiJob.ServiceId;
            NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].ServiceID = NvM_MultiJob.ServiceId;
            NvM_GetRamAddress(0);
            NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].SingleReqResult = NVM_REQ_PENDING;
            NvM_CopyCurBlockInfo();
            l_ReturnValue = E_OK;
        }
    }
    /***********************************/
    /*WriteAll Operation*/
    else
    {
        l_ReturnValue = NvM_MultiJobDispatchWrite();
    }
    return l_ReturnValue;
}

/*
 * Brief               Put a job to the job queue to be processing
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/* PRQA S 1532,3432,0709++ */ /* MISRA Rule 8.7,Rule 20.7,Rule 1.1 */
FUNC(Std_ReturnType, NVM_CODE)
NvM_JobEnqueue(NvM_BlockIdType BlockId, NvM_ServiceIdType ServiceId, P2VAR(uint8, AUTOMATIC, NVM_APPL_DATA) DestSrcPtr)
/* PRQA S 1532,3432,0709-- */ /* MISRA Rule 8.7,Rule 20.7,Rule 1.1 */
{
    uint8 l_PriLowBits;
    uint8 l_PriHighBits;
    uint8 l_CurActPriTable;
    uint8 l_JobPri;
    uint8 l_AddJobQueueIndex = 0U;
    NvM_JobTypeType l_JobType;
    Std_ReturnType l_ReturnValue = E_OK;
    uint16 l_BlockIdLoop;
    uint8 EnQueue;

    /*Multi Job*/
    if ((NVM_READ_ALL_SERV_ID == ServiceId) || (NVM_WRITE_ALL_SERV_ID == ServiceId)
        || (NVM_VALIDATE_ALL_SERV_ID == ServiceId))
    {
        SchM_Enter_NvM_Queue();
        NvM_MultiJob.Enqueue = STD_ON;
        NvM_MultiJob.ServiceId = ServiceId;
        SchM_Exit_NvM_Queue();
        NvM_MultiJobResultFeedBack(NvM_MultiJob.ServiceId, NVM_REQ_PENDING);
        SchM_Enter_NvM_Queue();
        if (NVM_READ_ALL_SERV_ID == ServiceId)
        {
            for (l_BlockIdLoop = 0; l_BlockIdLoop < (uint16)NVM_BLOCK_NUM_ALL; l_BlockIdLoop++)
            {
                NvM_AdminBlock[l_BlockIdLoop].ReadReq = TRUE; /* req NVM952 */
            }
        }
        SchM_Exit_NvM_Queue();
    }
    /*Single Job*/
    else if (((uint16)NVM_BLOCK_NUM_ALL >= BlockId) && (0U < BlockId))
    {
        EnQueue = NvM_GetWordBitState(NvM_AdminBlock[BlockId - 1U].FlagGroup, NVM_ADMIN_ENQUEUE);
        if ((uint8)STD_OFF == EnQueue)
        {
            SchM_Enter_NvM_Queue();
            NvM_AdminBlock[BlockId - 1U].ServiceID = ServiceId;
            l_JobPri = NvM_BlockDescriptor[BlockId - 1U].NvmBlockJobPriority;
            /*.Immedi write*/
            if ((0u == l_JobPri)
                && ((NVM_WRITE_BLOCK_SERV_ID == ServiceId) || (NVM_WRITE_PRAM_BLOCK_SERV_ID == ServiceId)))
            {
                l_JobType = NVM_JOB_TYPE_SINGLE_IMMED;
            }
            else
            {
                l_JobType = NVM_JOB_TYPE_SINGLE_NORMAL;
            }
            SchM_Exit_NvM_Queue();
            if ((Std_ReturnType)E_NOT_OK == NvM_GetQueueSpace(l_JobType, &l_AddJobQueueIndex))
            {
                /* Job queue overflow */
                l_ReturnValue = E_NOT_OK;
                NVM_DET_REPORTRUNTIMEERROR((uint8)ServiceId, NVM_E_QUEUE_FULL);
            }
            else
            {
                if (NVM_JOB_TYPE_SINGLE_IMMED == l_JobType)
                {
                    SchM_Enter_NvM_Queue();
                    /* store the job information to queue and set the job state as pending */
                    NvM_ImmedQueue[l_AddJobQueueIndex].BlockId = BlockId;
                    NvM_ImmedQueue[l_AddJobQueueIndex].DestSrcPtr = DestSrcPtr;
                    NvM_SetWordBitState(&NvM_AdminBlock[BlockId - 1U].FlagGroup, NVM_ADMIN_ENQUEUE, STD_ON);
                    SchM_Exit_NvM_Queue();
                    NvM_SingleJobResultFeedBack(BlockId, NVM_REQ_PENDING);
                }
                else
                {
                    SchM_Enter_NvM_Queue();
                    l_PriLowBits = l_JobPri & 0x0FU;
                    l_PriHighBits = (uint8)((l_JobPri & 0xF0U) >> 4);
                    /*MultiJob & SingleJob each take one queue*/
                    if ((uint8)STD_ON == NvM_MultiJob.Enqueue)
                    {
                        /* if there is a multiple type job in queue,
                         * need change the active priotity table */
                        l_CurActPriTable = 1u - NvM_Module.CurActPriTable;
                    }
                    else
                    {
                        l_CurActPriTable = NvM_Module.CurActPriTable;
                    }
                    /* set the priority table group flag */
                    NvM_PriorityTable[l_CurActPriTable][NVM_TABLE_SIZE_PRIORITY - 1U] |=
                        (uint16)((uint16)0x01u << l_PriHighBits);
                    if (0U
                        != (NvM_PriorityTable[l_CurActPriTable][l_PriHighBits]
                            & (uint16)((uint16)0x01u << l_PriLowBits)))
                    {
                        /* add the new job to the queue's tail */
                        NvM_StandQueue[NvM_PriTable2Queue[l_CurActPriTable][l_JobPri].TailIndex].NextIndex =
                            l_AddJobQueueIndex;
                    }
                    else
                    {
                        /* set the priority table's corresponding bit */
                        NvM_PriorityTable[l_CurActPriTable][l_PriHighBits] |= (uint16)((uint16)0x01u << l_PriLowBits);

                        NvM_PriTable2Queue[l_CurActPriTable][l_JobPri].HeadIndex = l_AddJobQueueIndex;
                    }
                    /* store the new job information to queue
                     * and set the block status as pending*/
                    NvM_PriTable2Queue[l_CurActPriTable][l_JobPri].TailIndex = l_AddJobQueueIndex;
                    /*The new job takes the last*/
                    NvM_StandQueue[l_AddJobQueueIndex].BlockId = BlockId;
                    NvM_StandQueue[l_AddJobQueueIndex].ServiceId = ServiceId;
                    NvM_StandQueue[l_AddJobQueueIndex].NextIndex = l_AddJobQueueIndex;
                    NvM_StandQueue[l_AddJobQueueIndex].DestSrcPtr = DestSrcPtr;
                    NvM_SetWordBitState(&NvM_AdminBlock[BlockId - 1U].FlagGroup, NVM_ADMIN_ENQUEUE, STD_ON);
                    SchM_Exit_NvM_Queue();
                    NvM_SingleJobResultFeedBack(BlockId, NVM_REQ_PENDING);
                }
            }
        }
        else
        {
            /* Invalid BlockId or Job has already enqueued */
            l_ReturnValue = E_NOT_OK;
        }
    }
    else
    {
        /* Invalid BlockId or Job has already enqueued */
        l_ReturnValue = E_NOT_OK;
    }

    return l_ReturnValue;
}

/***********************************/
/*
 * Brief               Get a job from the job queue to process
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/***********************************/
/* PRQA S 1532,0709,3334++ */ /* MISRA Rule 8.7,Rule 1.1,Rule 5.3 */
FUNC(Std_ReturnType, NVM_CODE) NvM_JobDequeue(void)
/* PRQA S 1532,0709,3334-- */ /* MISRA Rule 8.7,Rule 1.1,Rule 5.3 */
{
    uint8 l_JobQueueIndex;
    Std_ReturnType l_ReturnValue = E_OK;

    /*. ImmedQueue serves first during Dequeue.*/
    if (NVM_QUEUE_EMPTY_COUNT != NvM_ImmedQueueManage.Count)
    {
        SchM_Enter_NvM_Queue();
        if ((NVM_JOB_TYPE_MULTI == NvM_Module.CurrentJobType)
            || (NVM_JOB_TYPE_MULTI_GAP_SINGLE_IMMED == NvM_Module.CurrentJobType))
        {
            NvM_Module.CurrentJobType = NVM_JOB_TYPE_MULTI_GAP_SINGLE_IMMED;
        }
        else
        {
            NvM_Module.CurrentJobType = NVM_JOB_TYPE_SINGLE_IMMED;
        }
        NvM_CurRunning.BlockId = NvM_ImmedQueue[NvM_ImmedQueueManage.HeadIndex].BlockId;
        NvM_CurRunning.ServiceId = NVM_WRITE_BLOCK_SERV_ID;
        NvM_GetRamAddress(NvM_ImmedQueueManage.HeadIndex);
        SchM_Exit_NvM_Queue();
        NvM_RelQueueSpace(NvM_Module.CurrentJobType, NvM_ImmedQueueManage.HeadIndex);
        NvM_CopyCurBlockInfo();
        NvM_SetWordBitState(&NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].FlagGroup, NVM_ADMIN_ENQUEUE, STD_OFF);
    }
    /***********************************/
    else
    {
        if ((NVM_JOB_TYPE_MULTI_GAP_SINGLE_IMMED == NvM_Module.CurrentJobType)
            || (NVM_JOB_TYPE_MULTI == NvM_Module.CurrentJobType))
        {
            if (NVM_JOB_TYPE_MULTI_GAP_SINGLE_IMMED == NvM_Module.CurrentJobType)
            {
                NvM_Module.CurrentJobType = NVM_JOB_TYPE_MULTI;
                NvM_CurRunning.BlockId = NvM_MultiJob.BlockId;
                NvM_CurRunning.ServiceId = NvM_MultiJob.ServiceId;
            }
            /* switch low layer mode to slow mode after multiple type job is processed over */
            l_ReturnValue = NvM_MultiJobDispatch();
        }
        /***********************************/
        else
        {
            /* get the highest priority from the priority table */
            l_ReturnValue = NvM_GetHighestPriJob(&l_JobQueueIndex);
            if ((Std_ReturnType)E_OK == l_ReturnValue)
            {
                SchM_Enter_NvM_Queue();
                /* transfer the job need processing to global variant */
                NvM_Module.CurrentJobType = NVM_JOB_TYPE_SINGLE_NORMAL;
                NvM_CurRunning.BlockId = NvM_StandQueue[l_JobQueueIndex].BlockId;
                NvM_CurRunning.ServiceId = NvM_StandQueue[l_JobQueueIndex].ServiceId;
                NvM_GetRamAddress(l_JobQueueIndex);
                SchM_Exit_NvM_Queue();
                /* after transfer the job to global variant, need release the space */
                NvM_RelQueueSpace(NvM_Module.CurrentJobType, l_JobQueueIndex);
                SchM_Enter_NvM_Queue();
                NvM_CopyCurBlockInfo();
                NvM_SetWordBitState(&NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].FlagGroup, NVM_ADMIN_ENQUEUE, STD_OFF);
                SchM_Exit_NvM_Queue();
            }
            /***********************************/
            else if ((uint8)STD_ON == NvM_MultiJob.Enqueue)
            {
                SchM_Enter_NvM_Queue();
                NvM_Module.CurrentJobType = NVM_JOB_TYPE_MULTI;
                NvM_CurRunning.ServiceId = NvM_MultiJob.ServiceId;
                if (NVM_READ_ALL_SERV_ID == NvM_MultiJob.ServiceId)
                {
                    NvM_MultiJob.BlockId = NVM_READALL_FIRST_BLOCKID;
                }
                else if (NVM_WRITE_ALL_SERV_ID == NvM_MultiJob.ServiceId)
                {
                    NvM_MultiJob.BlockId = NVM_WRITEALL_FIRST_BLOCKID;
                }
                else
                {
                    NvM_MultiJob.BlockId = NVM_VALIDATEALL_FIRST_BLOCKID;
                }
                NvM_CurRunning.BlockId = NvM_MultiJob.BlockId;
                NvM_GetRamAddress(0);
                NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].SingleReqResult = NVM_REQ_PENDING;
                NvM_CopyCurBlockInfo();

#if (STD_ON == NVM_DRV_MODE_SWITCH)
                /* switch the low layer process mode to fast mode before multiple type job start */
                MemIf_SetMode(MEMIF_MODE_FAST);
#endif
                SchM_Exit_NvM_Queue();
                l_ReturnValue = E_OK;
            }
            /***********************************/
            else
            {
                l_ReturnValue = E_NOT_OK;
            }
        }
    }
    return l_ReturnValue;
}
#endif

/***********************************/
/*
 * Brief               Get Ram Address
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/***********************************/
/* PRQA S 0709++ */ /* MISRA Rule 1.1 */
static FUNC(void, NVM_CODE) NvM_GetRamAddress(uint8 QueueIndex)
/* PRQA S 0709-- */ /* MISRA Rule 1.1 */
{
    uint8 SyncMechanism;
    uint8 Repair;
    NvM_BlockIdType CurBlockId = NvM_CurRunning.BlockId - 1U;

#if (STD_ON == NVM_JOB_PRIORITIZATION)
    if ((NvM_Module.CurrentJobType == NVM_JOB_TYPE_MULTI_GAP_SINGLE_IMMED)
        || (NvM_Module.CurrentJobType == NVM_JOB_TYPE_SINGLE_IMMED))
    {
        SyncMechanism =
            NvM_GetWordBitState(NvM_BlockDescriptor[CurBlockId].FlagGroup, (uint8)NVM_BLOCK_DESC_SYNCMECHANISM);
        if (NULL_PTR != NvM_ImmedQueue[QueueIndex].DestSrcPtr)
        {
            NvM_CurRunning.RamAddr = NvM_ImmedQueue[QueueIndex].DestSrcPtr;
            NvM_SetWordBitState(&NvM_AdminBlock[CurBlockId].FlagGroup, NVM_ADMIN_RAM_VALID_CHANGE_STATUS_USED, STD_OFF);
            NvM_SetWordBitState(&NvM_AdminBlock[CurBlockId].FlagGroup, NVM_ADMIN_RAMMIRROR_OR_NOT, STD_OFF);
        }
        else if ((uint8)STD_ON == SyncMechanism)
        {
#if (NVM_MAX_LENGTH_CONFIGED_RAM_MIRROR > 0)
            NvM_CurRunning.RamAddr = NVM_RamMirror;
#endif
            NvM_SetWordBitState(&NvM_AdminBlock[CurBlockId].FlagGroup, NVM_ADMIN_RAM_VALID_CHANGE_STATUS_USED, STD_ON);
            NvM_SetWordBitState(&NvM_AdminBlock[CurBlockId].FlagGroup, NVM_ADMIN_RAMMIRROR_OR_NOT, STD_ON);
        }
        else
        {
            NvM_CurRunning.RamAddr = NvM_BlockDescriptor[CurBlockId].NvmRamBlockDataAddress;
            NvM_SetWordBitState(&NvM_AdminBlock[CurBlockId].FlagGroup, NVM_ADMIN_RAM_VALID_CHANGE_STATUS_USED, STD_ON);
            NvM_SetWordBitState(&NvM_AdminBlock[CurBlockId].FlagGroup, NVM_ADMIN_RAMMIRROR_OR_NOT, STD_OFF);
        }
    }
    else if (NvM_Module.CurrentJobType == NVM_JOB_TYPE_MULTI)
#else
    if (NvM_Module.CurrentJobType == NVM_JOB_TYPE_MULTI)
#endif
    {
        Repair = NvM_GetWordBitState(NvM_AdminBlock[CurBlockId].FlagGroup, NVM_ADMIN_NV_REPAIR);
        if (((uint8)STD_ON == Repair) && (NVM_WRITE_ALL_SERV_ID == NvM_CurRunning.ServiceId))
        {
            NvM_SetWordBitState(&NvM_AdminBlock[CurBlockId].FlagGroup, NVM_ADMIN_RAM_VALID_CHANGE_STATUS_USED, STD_OFF);
            NvM_CurRunning.RamAddr = &NVM_TemporaryRAMForRepaire[NvM_BlockDescriptor[CurBlockId].RepaireIndex][0];
            /*The other routing for the config ID*/
        }
        else
        {
            SyncMechanism =
                NvM_GetWordBitState(NvM_BlockDescriptor[CurBlockId].FlagGroup, (uint8)NVM_BLOCK_DESC_SYNCMECHANISM);
            NvM_SetWordBitState(&NvM_AdminBlock[CurBlockId].FlagGroup, NVM_ADMIN_RAM_VALID_CHANGE_STATUS_USED, STD_ON);
            if ((uint8)STD_ON == SyncMechanism)
            {
#if (NVM_MAX_LENGTH_CONFIGED_RAM_MIRROR > 0)
                NvM_CurRunning.RamAddr = NVM_RamMirror;
#endif
                NvM_SetWordBitState(&NvM_AdminBlock[CurBlockId].FlagGroup, NVM_ADMIN_RAMMIRROR_OR_NOT, STD_ON);
            }
            else
            {
                NvM_CurRunning.RamAddr = NvM_BlockDescriptor[CurBlockId].NvmRamBlockDataAddress;
                NvM_SetWordBitState(&NvM_AdminBlock[CurBlockId].FlagGroup, NVM_ADMIN_RAMMIRROR_OR_NOT, STD_OFF);
            }
        }
    }
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
    else if (NvM_Module.CurrentJobType == NVM_JOB_TYPE_SINGLE_NORMAL)
    {
        SyncMechanism =
            NvM_GetWordBitState(NvM_BlockDescriptor[CurBlockId].FlagGroup, (uint8)NVM_BLOCK_DESC_SYNCMECHANISM);
        if (NULL_PTR != NvM_StandQueue[QueueIndex].DestSrcPtr)
        {
            NvM_CurRunning.RamAddr = NvM_StandQueue[QueueIndex].DestSrcPtr;
            NvM_SetWordBitState(&NvM_AdminBlock[CurBlockId].FlagGroup, NVM_ADMIN_RAM_VALID_CHANGE_STATUS_USED, STD_OFF);
            NvM_SetWordBitState(&NvM_AdminBlock[CurBlockId].FlagGroup, NVM_ADMIN_RAMMIRROR_OR_NOT, STD_OFF);
        }
        else if ((uint8)STD_ON == SyncMechanism)
        {
#if (NVM_MAX_LENGTH_CONFIGED_RAM_MIRROR > 0)
            NvM_CurRunning.RamAddr = NVM_RamMirror;
#endif
            NvM_SetWordBitState(&NvM_AdminBlock[CurBlockId].FlagGroup, NVM_ADMIN_RAM_VALID_CHANGE_STATUS_USED, STD_ON);
            NvM_SetWordBitState(&NvM_AdminBlock[CurBlockId].FlagGroup, NVM_ADMIN_RAMMIRROR_OR_NOT, STD_ON);
        }
        else
        {
            NvM_CurRunning.RamAddr = NvM_BlockDescriptor[CurBlockId].NvmRamBlockDataAddress;
            NvM_SetWordBitState(&NvM_AdminBlock[CurBlockId].FlagGroup, NVM_ADMIN_RAM_VALID_CHANGE_STATUS_USED, STD_ON);
            NvM_SetWordBitState(&NvM_AdminBlock[CurBlockId].FlagGroup, NVM_ADMIN_RAMMIRROR_OR_NOT, STD_OFF);
        }
    }
#endif
    else
    {
        /*idle*/
    }
}

/*
 * Brief               Set the flag after job process over
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/* PRQA S 0709++ */ /* MISRA Rule 1.1 */
static FUNC(void, NVM_CODE) NvM_JobOverSetFlag(NvM_RequestResultType SingleReqResult, NvM_CrcChangeType CrcChange)
/* PRQA S 0709-- */ /* MISRA Rule 1.1 */
{
    uint8 WriteBlockOnce;
#if ((STD_OFF == NVM_JOB_PRIORITIZATION) && (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS))
    NvM_BlockIdType l_Curindex;
    NvM_BlockIdType l_BlockIdIndex;
    boolean BlockFound = FALSE;
    NvM_BlockIdType totalnum;

    totalnum = NvM_StandQueueManage.Count;
    l_BlockIdIndex = NvM_StandQueueManage.HeadIndex;

    for (l_Curindex = 0; l_Curindex < totalnum; l_Curindex++)
    {
        if (NvM_StandQueue[l_BlockIdIndex].BlockId == NvM_CurRunning.BlockId)
        {
            /*still in queue*/
            BlockFound = TRUE;
            break;
        }
        l_BlockIdIndex++;
        if (l_BlockIdIndex > NVM_SIZE_STANDARD_JOB_QUEUE - 1u) /* PRQA S 3397 */ /* MISRA Rule 12.1 */
        {
            l_BlockIdIndex = 0;
        }
    }

    /*if block still in queue should not change status*/
    if (TRUE == BlockFound)
    {
        NvM_SetWordBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_ENQUEUE, STD_ON);
    }
    else
    {
        NvM_SingleJobResultFeedBack(NvM_CurRunning.BlockId, SingleReqResult);
        NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].ServiceID = NVM_NONE_SERV_ID;
    }
#else  /* STD_OFF==NVM_JOB_PRIORITIZATION && NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS */
    NvM_SingleJobResultFeedBack(NvM_CurRunning.BlockId, SingleReqResult);
    NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].ServiceID = NVM_NONE_SERV_ID;
    NvM_SetWordBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_ENQUEUE, STD_OFF);
#endif /* STD_OFF==NVM_JOB_PRIORITIZATION && NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS */

    WriteBlockOnce = NvM_GetWordBitState(
        NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].FlagGroup,
        (uint8)NVM_BLOCK_DESC_WRITEBLOCKONCE);
    /*calculate the write once flag for each block*/
    if (((uint8)STD_ON == WriteBlockOnce)
        && (NVM_REQ_OK == NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].SingleReqResult))
    {
        NvM_SetWordBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_NV_WRPRO, STD_ON);
    }

    if (NVM_CRC_UPDATE == CrcChange)
    {
        NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].Crc = NvM_CurRunning.Crc;
        NvM_SetWordBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_CRC_VALID, STD_ON);
    }
    else if (NVM_CRC_DELETE == CrcChange)
    {
        NvM_SetWordBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_CRC_VALID, STD_OFF);
    }
    else
    {
        /*idle*/
    }
    NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].FlagGroup = NvM_CurRunning.AdminFlagGroup;

    /*. CallBack function for BlockID*/
    if (NULL_PTR != NvM_CurRunning.SingleCallback)
    {
        NvM_BlockRequestType BlockRequest;
        boolean NotiFlag = TRUE;
        switch (NvM_CurRunning.ServiceId)
        {
        case NVM_READ_BLOCK_SERV_ID:
        case NVM_READ_PRAM_BLOCK_SERV_ID:
            BlockRequest = NVM_READ_BLOCK;
            break;
        case NVM_WRITE_BLOCK_SERV_ID:
        case NVM_WRITE_PRAM_BLOCK_SERV_ID:
            BlockRequest = NVM_WRITE_BLOCK;
            break;
        case NVM_RESTORE_BLOCK_DEFAULTS_SERV_ID:
        case NVM_RESTORE_PRAM_BLOCK_DEFAULTS_SERV_ID:
            BlockRequest = NVM_RESTORE_BLOCK_DEFAULTS;
            break;
        case NVM_ERASE_NV_BLOCK_SERV_ID:
            BlockRequest = NVM_ERASE_NV_BLOCK;
            break;
        case NVM_INVALIDATE_NV_BLOCK_SERV_ID:
            BlockRequest = NVM_INVALIDATE_NV_BLOCK;
            break;
        case NVM_READ_ALL_SERV_ID:
            BlockRequest = NVM_READ_ALL_BLOCK;
            break;
        default:
            NotiFlag = FALSE;
            break;
        }
        if (NotiFlag == TRUE)
        {
            (*NvM_CurRunning.SingleCallback)(BlockRequest, NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].SingleReqResult);
        }
    }

    NvM_SetWordBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_REDUNDANCY_LOSS, STD_OFF);
    NvM_Module.MemIfJobState = NVM_MEMIF_JOB_IDLE;
    NvM_Module.JobStep = NVM_JOB_STEP_IDLE;
    NvM_AtomJob.Count = 0;
}

/*
 * Brief               The low layer processing the read job success
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/* PRQA S 0709++ */ /* MISRA Rule 1.1 */
static FUNC(void, NVM_CODE) NvM_MemIfReadOk(void)
/* PRQA S 0709-- */ /* MISRA Rule 1.1 */
{
    uint8 StaicBlockIdCheck;
    NvM_Module.MemIfJobState = NVM_MEMIF_JOB_IDLE;

    if (NVM_JOB_STEP_READ_2ND_NV == NvM_Module.JobStep)
    {
        NvM_SetWordBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_REDUNDANCY_LOSS, STD_ON);
    }
    else
    {
        NvM_SetWordBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_NV_REPAIR, STD_OFF);
        /*success read the 1st NV,Clear the repair flag*/
    }
    StaicBlockIdCheck = NvM_GetWordBitState(
        NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].FlagGroup,
        (uint8)NVM_BLOCK_DESC_STATICBLOCKIDCHECK);
    if ((uint8)STD_ON == StaicBlockIdCheck)
    {
        NvM_ReadStaticBlockID(NvM_NvDataBuffer, NvM_CurRunning.CRCType);
    }
    if (NVM_CRC_NOT_USED != NvM_CurRunning.CRCType)
    {
        NvM_CurRunning.CrcAddr = NvM_NvDataBuffer;
        if (NVM_JOB_STEP_READ_1ST_NV == NvM_Module.JobStep)
        {
            NvM_Module.JobStep = NVM_JOB_STEP_CALC_CRC_READ_1ST_NV;
        }
        else
        {
            NvM_Module.JobStep = NVM_JOB_STEP_CALC_CRC_READ_2ND_NV;
        }
        NvM_AtomJobReq(NVM_ATOMJOB_CALCCRC);
    }
    else if ((uint8)STD_ON == StaicBlockIdCheck)
    {
        NvM_Module.JobStep = NVM_JOB_STEP_CHECK;
    }
    else
    {
        NvM_Module.JobStep = NVM_JOB_STEP_COPY;
    }
}

/*
 * Brief               The low layer processing the read job failed
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/* PRQA S 0709++ */ /* MISRA Rule 1.1 */
static FUNC(void, NVM_CODE) NvM_MemIfReadFailed(void)
/* PRQA S 0709-- */ /* MISRA Rule 1.1 */
{
    NvM_Module.MemIfJobState = NVM_MEMIF_JOB_IDLE;

#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
    if (NVM_BLOCK_DATASET == NvM_CurRunning.ManagementType)
    {
        NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
#if (STD_ON == NVM_DEM_E_REQ_FAILED)
        Dem_ReportErrorStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_FAILED); /* req 678  654  781*/
#endif
#if (STD_ON == NVM_DEM_E_HARDWARE)
        Dem_ReportErrorStatus(NVM_E_HARDWARE, DEM_EVENT_STATUS_FAILED); /* req 835*/
#endif
        NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_REMAIN); /* req NVM359  361*/
    }
    else
#endif
        if (((NVM_BLOCK_NATIVE == NvM_CurRunning.ManagementType) || (NVM_JOB_STEP_READ_2ND_NV == NvM_Module.JobStep))
            && ((NULL_PTR != NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].NvmRomBlockDataAddress)
                || (NULL_PTR != NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].NvmInitBlockCallback)))
    {
        NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF); /* req NVM317*/
        NvM_CurRunning.ESingleReqResult = NVM_REQ_INTEGRITY_FAILED;
#if (STD_ON == NVM_DEM_E_REQ_FAILED)
        Dem_ReportErrorStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
#if (STD_ON == NVM_DEM_E_HARDWARE)
        Dem_ReportErrorStatus(NVM_E_HARDWARE, DEM_EVENT_STATUS_FAILED); /* req 835*/
#endif
        NVM_GetRepeatMirrorOperation();
        NvM_Module.JobStep = NVM_JOB_STEP_READ_ROM;
        NvM_AtomJobReq(NVM_ATOMJOB_READROM);
    }
    else if ((NVM_BLOCK_REDUNDANT == NvM_CurRunning.ManagementType) && (NVM_JOB_STEP_READ_1ST_NV == NvM_Module.JobStep))
    {
        /*. Only for the Redundant type*/
        NvM_Module.JobStep = NVM_JOB_STEP_READ_2ND_NV;
        NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
        NvM_CurRunning.Index = 1;
    }
    else
    {
        NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
#if (STD_ON == NVM_DEM_E_REQ_FAILED)
        Dem_ReportErrorStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
#if (STD_ON == NVM_DEM_E_HARDWARE)
        Dem_ReportErrorStatus(NVM_E_HARDWARE, DEM_EVENT_STATUS_FAILED); /* req 835*/
#endif
        NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_REMAIN);
    }
}

/*
 * Brief               The low layer is processing
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
static FUNC(void, NVM_CODE) NvM_MemIfPending(void) /* PRQA S 0709 */ /* MISRA Rule 1.1 */
{
    /*do nothing, just waiting*/
    /*do not remove the function*/
}

/*
 * Brief               The low layer job is cancelled
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/* PRQA S 0709++ */ /* MISRA Rule 1.1 */
static FUNC(void, NVM_CODE) NvM_MemIfCancelled(void)
/* PRQA S 0709-- */ /* MISRA Rule 1.1 */
{
    NvM_Module.MemIfJobState = NVM_MEMIF_JOB_IDLE;
    NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
    NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_REMAIN);
}

/*
 * Brief               The low layer block is inconsistent
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
static FUNC(void, NVM_CODE) NvM_MemIfInconsistent(void) /* PRQA S 0709 */ /* MISRA Rule 1.1 */
{
    NvM_Module.MemIfJobState = NVM_MEMIF_JOB_IDLE;
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
    if (NVM_BLOCK_DATASET == NvM_CurRunning.ManagementType)
    {
        NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
#if (STD_ON == NVM_DEM_E_INTEGRITY_FAILED)
        Dem_ReportErrorStatus(NVM_E_INTEGRITY_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
#if (STD_ON == NVM_DEM_E_HARDWARE)
        Dem_ReportErrorStatus(NVM_E_HARDWARE, DEM_EVENT_STATUS_FAILED); /* req 835*/
#endif
        NvM_JobOverSetFlag(NVM_REQ_INTEGRITY_FAILED, NVM_CRC_DELETE); /* req NVM358*/
    }
    else if (
        ((NVM_BLOCK_NATIVE == NvM_CurRunning.ManagementType) || (NVM_JOB_STEP_READ_2ND_NV == NvM_Module.JobStep))
        && ((NULL_PTR != NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].NvmRomBlockDataAddress)
            || (NULL_PTR != NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].NvmInitBlockCallback)))
#else
    if (((NVM_BLOCK_NATIVE == NvM_CurRunning.ManagementType)
         || (NVM_JOB_STEP_CALC_CRC_READ_2ND_NV == NvM_Module.JobStep))
        && ((NULL_PTR != NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].NvmRomBlockDataAddress)
            || (NULL_PTR != NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].NvmInitBlockCallback)))
#endif
    {
        NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF); /* req NVM317*/
        /* req NVM358*/
        NvM_CurRunning.ESingleReqResult = NVM_REQ_INTEGRITY_FAILED;
#if (STD_ON == NVM_DEM_E_INTEGRITY_FAILED)
        Dem_ReportErrorStatus(NVM_E_INTEGRITY_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
#if (STD_ON == NVM_DEM_E_HARDWARE)
        Dem_ReportErrorStatus(NVM_E_HARDWARE, DEM_EVENT_STATUS_FAILED);
#endif
        NVM_GetRepeatMirrorOperation();
        NvM_Module.JobStep = NVM_JOB_STEP_READ_ROM;
        NvM_AtomJobReq(NVM_ATOMJOB_READROM);
    }
    else if ((NVM_BLOCK_REDUNDANT == NvM_CurRunning.ManagementType) && (NVM_JOB_STEP_READ_1ST_NV == NvM_Module.JobStep))
    {

        NvM_Module.JobStep = NVM_JOB_STEP_READ_2ND_NV;
        NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY; /*. Only for the Redundant type*/
        NvM_CurRunning.Index = 1;
    }
    else
    {
        NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
#if (STD_ON == NVM_DEM_E_INTEGRITY_FAILED)
        Dem_ReportErrorStatus(NVM_E_INTEGRITY_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
#if (STD_ON == NVM_DEM_E_HARDWARE)
        Dem_ReportErrorStatus(NVM_E_HARDWARE, DEM_EVENT_STATUS_FAILED);
#endif
        NvM_JobOverSetFlag(NVM_REQ_INTEGRITY_FAILED, NVM_CRC_DELETE);
    }
}

/*
 * Brief               The low layer block is invalid
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
static FUNC(void, NVM_CODE) NvM_MemIfInvalid(void) /* PRQA S 0709 */ /* MISRA Rule 1.1 */
{
    NvM_Module.MemIfJobState = NVM_MEMIF_JOB_IDLE;
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
    if (NVM_BLOCK_DATASET == NvM_CurRunning.ManagementType)
    {
        NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
        NvM_JobOverSetFlag(NVM_REQ_NV_INVALIDATED, NVM_CRC_DELETE);
    }
    else
#endif
        if ((NVM_BLOCK_NATIVE == NvM_CurRunning.ManagementType) || (NVM_JOB_STEP_READ_2ND_NV == NvM_Module.JobStep))
    {
        NvM_CurRunning.ESingleReqResult = NVM_REQ_INTEGRITY_FAILED;
        NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF); /* req NVM317*/
        if ((NULL_PTR != NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].NvmRomBlockDataAddress)
            || (NULL_PTR != NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].NvmInitBlockCallback))
        {
            NVM_GetRepeatMirrorOperation();
            NvM_Module.JobStep = NVM_JOB_STEP_READ_ROM;
            NvM_AtomJobReq(NVM_ATOMJOB_READROM);
        }
        else
        {
            NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
            NvM_JobOverSetFlag(NVM_REQ_NV_INVALIDATED, NVM_CRC_DELETE);
        }
    }
    else
    {
        NvM_Module.JobStep = NVM_JOB_STEP_READ_2ND_NV;
        NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
        NvM_CurRunning.Index = 1;
    }
}

/*
 * Brief               The low layer processing the write job success
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
static FUNC(void, NVM_CODE) NvM_MemIfWriteOk(void)
{
    uint8 WriteVerification;
    NvM_Module.MemIfJobState = NVM_MEMIF_JOB_IDLE;

    if ((NVM_BLOCK_REDUNDANT == NvM_CurRunning.ManagementType) && (NVM_JOB_STEP_WRITE_2ND_NV == NvM_Module.JobStep))
    {
        NvM_CurRunning.ESingleReqResult = NVM_REQ_OK;
        NvM_Module.JobStep = NVM_JOB_STEP_WRITE_1ST_NV;
        NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
        NvM_CurRunning.Index = 0;
    }
    else
    {
        WriteVerification = NvM_GetWordBitState(
            NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].FlagGroup,
            (uint8)NVM_BLOCK_DESC_WRITEVERIFICATION);
        if ((uint8)STD_ON == WriteVerification)
        {
            NvM_Module.JobStep = NVM_JOB_STEP_READ_1ST_NV;
            NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
            NvM_AtomJobReq(NVM_ATOMJOB_WRITEVERIFICATION);
        }
        else
        {
            NvM_UpdateValidandChangeStatus(STD_ON, STD_OFF);
            if (NVM_BLOCK_REDUNDANT == NvM_CurRunning.ManagementType)
            {
                NvM_SetWordBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_NV_REPAIR, STD_OFF);
            }
            NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_UPDATE); /*req 284*/
        }
    }
}

/*
 * Brief               The low layer processing the write job failed
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
static FUNC(void, NVM_CODE) NvM_MemIfWriteFailed(void)
{
    uint8 WriteVerification;
    NvM_Module.MemIfJobState = NVM_MEMIF_JOB_IDLE;
    if (0U < NvM_CurRunning.WriteTimesCounter)
    {
        NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
    }
    else if (
        (NVM_BLOCK_REDUNDANT == NvM_CurRunning.ManagementType) && (NVM_JOB_STEP_WRITE_2ND_NV == NvM_Module.JobStep))
    {
        NvM_CurRunning.WriteTimesCounter = NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].NvMMaxNumOfWriteRetries;
        NvM_Module.JobStep = NVM_JOB_STEP_WRITE_1ST_NV;
        NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
        NvM_CurRunning.Index = 0;
        NVM_GetRepeatMirrorOperation();
#if (STD_ON == NVM_DEM_E_REQ_FAILED)
        Dem_ReportErrorStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
    }
    else
    {
        if ((NVM_BLOCK_REDUNDANT == NvM_CurRunning.ManagementType) && (NVM_REQ_OK == NvM_CurRunning.ESingleReqResult))
        {
            WriteVerification = NvM_GetWordBitState(
                NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].FlagGroup,
                (uint8)NVM_BLOCK_DESC_WRITEVERIFICATION);
            if ((uint8)STD_ON == WriteVerification)
            {
                NvM_Module.JobStep = NVM_JOB_STEP_READ_2ND_NV; /*Read 2ND firstly, only 2ND is OK*/
                NvM_CurRunning.Index = 1;
                NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
                NvM_AtomJobReq(NVM_ATOMJOB_WRITEVERIFICATION);
            }
            else
            {
                NvM_UpdateValidandChangeStatus(STD_ON, STD_OFF);
                NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_UPDATE);
            }
        }
        else
        {
            NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
            NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_DELETE);
#if (STD_ON == NVM_DEM_E_REQ_FAILED)
            Dem_ReportErrorStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
        }
    }
}

#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
#if (STD_ON == NVM_JOB_PRIORITIZATION)

/*
 * Brief               the low layer processing the erase job success
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
static FUNC(void, NVM_CODE) NvM_MemIfEraseOk(void) /* PRQA S 0709 */ /* MISRA Rule 1.1 */
{
    NvM_Module.MemIfJobState = NVM_MEMIF_JOB_IDLE;
    if (NVM_BLOCK_REDUNDANT == NvM_CurRunning.ManagementType)
    {
        if (NVM_JOB_STEP_ERASE_1ST_NV == NvM_Module.JobStep)
        {
            NvM_Module.JobStep = NVM_JOB_STEP_ERASE_2ND_NV;
            NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
            NvM_CurRunning.Index = 1;
        }
        else
        {
            NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
            NvM_SetWordBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_NV_REPAIR, STD_OFF);
            /*Both NV block is erased,repair is useless*/
            NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_DELETE);
        }
    }
    else
    {
        NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
        NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_DELETE);
    }
}

/*
 * Brief               The low layer processing the erase job failed
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
static FUNC(void, NVM_CODE) NvM_MemIfEraseFailed(void)
{
    NvM_Module.MemIfJobState = NVM_MEMIF_JOB_IDLE;
    NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
    NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_DELETE); /* req NVM271*/
#if (STD_ON == NVM_DEM_E_REQ_FAILED)
    Dem_ReportErrorStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_FAILED); /* req NVM663*/
#endif
}
#endif

/*
 * Brief               The low layer processing the invalid job success
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
static FUNC(void, NVM_CODE) NvM_MemIfInvalidOk(void)
{
    NvM_Module.MemIfJobState = NVM_MEMIF_JOB_IDLE;
    if (NVM_BLOCK_REDUNDANT == NvM_CurRunning.ManagementType)
    {
        if (NVM_JOB_STEP_INVALID_1ST_NV == NvM_Module.JobStep)
        {
            NvM_Module.JobStep = NVM_JOB_STEP_INVALID_2ND_NV;
            NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
            NvM_CurRunning.Index = 1;
        }
        else if (NVM_JOB_STEP_INVALID_2ND_NV == NvM_Module.JobStep)
        {
            NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
            NvM_SetWordBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_NV_REPAIR, STD_OFF);
            /*Both NV block is invalid,repair is useless*/
            NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_DELETE); /* req NVM274 275*/
        }
        else
        {
            /*idle*/
        }
    }
    else
    {
        NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
        NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_DELETE);
    }
}

/*
 * Brief               The low layer processing the invalid job failed
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
static FUNC(void, NVM_CODE) NvM_MemIfInvalidFailed(void)
{
    NvM_Module.MemIfJobState = NVM_MEMIF_JOB_IDLE;
    NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
    NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_DELETE); /* req NVM275*/
#if (STD_ON == NVM_DEM_E_REQ_FAILED)
    Dem_ReportErrorStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_FAILED); /* req NVM666*/
#endif
}
#endif

/*
 * Brief               The module internal function to process read rom request
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/* PRQA S 1532,3334,0709++ */ /* MISRA Rule 8.7,Rule 5.3,Rule 1.1 */
FUNC(void, NVM_CODE) NvM_InterReadRom(void)
/* PRQA S 1532,3334,0709-- */ /* MISRA Rule 8.7,Rule 5.3,Rule 1.1 */
{
    uint8 CalcRamBlockCrc;
    NvM_BlockIdType CurBlockId = NvM_CurRunning.BlockId - 1U;
    if (NVM_JOB_STEP_CALC_CRC == NvM_Module.JobStep)
    {
        NvM_UpdateValidandChangeStatus(STD_ON, STD_ON); /*NVM4.2.2 Chapter7.2.2.14.2*/
        if ((NvM_CurRunning.ESingleReqResult == NVM_REQ_INTEGRITY_FAILED)
            || (NvM_AdminBlock[CurBlockId].SingleReqResult == NVM_REQ_NV_INVALIDATED)
            || (NvM_AdminBlock[CurBlockId].SingleReqResult == NVM_REQ_NOT_OK))
        {
            NvM_JobOverSetFlag(NVM_REQ_RESTORED_FROM_ROM, NVM_CRC_DELETE);
        }
        else
        {
            NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_DELETE);
        }
    }
    else
    {
        if ((NULL_PTR != NvM_BlockDescriptor[CurBlockId].NvmRomBlockDataAddress)
            && (NvM_CurRunning.RepeatMirrorCounter > 0U))
        {
            if ((Std_ReturnType)E_OK == NvM_MemCpy(NvM_CurRunning.RomAddr, NvM_CurRunning.RamAddr))
            {
                CalcRamBlockCrc = NvM_GetWordBitState(
                    NvM_BlockDescriptor[CurBlockId].FlagGroup,
                    (uint8)NVM_BLOCK_DESC_CALCRAMBLOCKCRC);
                if ((uint8)STD_ON == CalcRamBlockCrc)
                {
                    NvM_CurRunning.CrcAddr = NvM_CurRunning.RamAddr;
                    NvM_Module.JobStep = NVM_JOB_STEP_CALC_CRC;
                    NvM_AtomJobReq(NVM_ATOMJOB_CALCCRC); /* req NVM229*/
                }
                else
                {
                    NvM_UpdateValidandChangeStatus(STD_ON, STD_ON);
                    if ((NvM_CurRunning.ESingleReqResult == NVM_REQ_INTEGRITY_FAILED)
                        || (NvM_AdminBlock[CurBlockId].SingleReqResult == NVM_REQ_NV_INVALIDATED)
                        || (NvM_AdminBlock[CurBlockId].SingleReqResult == NVM_REQ_NOT_OK))
                    {
                        NvM_JobOverSetFlag(NVM_REQ_RESTORED_FROM_ROM, NVM_CRC_DELETE);
                    }
                    else
                    {
                        NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_DELETE);
                    }
                }
            }
        }
        else if (NULL_PTR != NvM_CurRunning.InitCallback)
        {
            NvM_InitBlockRequestType InitBlockRequest = NVM_INIT_READ_BLOCK;
            switch (NvM_CurRunning.ServiceId)
            {
            case NVM_RESTORE_BLOCK_DEFAULTS_SERV_ID:
            case NVM_RESTORE_PRAM_BLOCK_DEFAULTS_SERV_ID:
                InitBlockRequest = NVM_INIT_RESTORE_BLOCK_DEFAULTS;
                break;
            case NVM_READ_ALL_SERV_ID:
                InitBlockRequest = NVM_INIT_READ_ALL_BLOCK;
                break;
            default:
                /*idle*/
                break;
            }
            if ((Std_ReturnType)E_OK == (*NvM_CurRunning.InitCallback)(InitBlockRequest))
            {
                NvM_UpdateValidandChangeStatus(STD_ON, STD_ON);
                if ((NvM_CurRunning.ESingleReqResult == NVM_REQ_INTEGRITY_FAILED)
                    || (NvM_AdminBlock[CurBlockId].SingleReqResult == NVM_REQ_NV_INVALIDATED)
                    || (NvM_AdminBlock[CurBlockId].SingleReqResult == NVM_REQ_NOT_OK))
                {
                    NvM_JobOverSetFlag(NVM_REQ_RESTORED_FROM_ROM, NVM_CRC_DELETE);
                }
                else
                {
                    NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_DELETE);
                }
            }
            else
            {
                NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_REMAIN);
            }
        }
        else
        {
            NvM_AdminBlock[CurBlockId].SingleReqResult = (NvM_CurRunning.ESingleReqResult == NVM_REQ_INTEGRITY_FAILED)
                                                             ? NVM_REQ_INTEGRITY_FAILED
                                                             : NvM_AdminBlock[CurBlockId].SingleReqResult;
            NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
            NvM_JobOverSetFlag(NvM_AdminBlock[CurBlockId].SingleReqResult, NVM_CRC_DELETE);
        }
    }
}

/*
 * Brief               The module internal function to calculate crc
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
FUNC(void, NVM_CODE) NvM_InterCalcCrc(void) /* PRQA S 1532,3334 */ /* MISRA Rule 8.7,Rule 5.3 */
{
    uint16 l_CrcLength;
#if (STD_ON == NVM_INCLUDE_CRC)
    boolean NvM_CrcIsFirstCall;
#endif /* STD_ON == NVM_INCLUDE_CRC */

#if (0xFFFFU != NVM_CRC_NUM_OF_BYTES)
    if (0U == NvM_CurRunning.CrcFlag)
    {
        NvM_CurRunning.CrcFlag = 1U;
#endif /* 0xFFFFU != NVM_CRC_NUM_OF_BYTES */
        NvM_CurRunning.CrcLength = NvM_CurRunning.Length;
#if (STD_ON == NVM_INCLUDE_CRC)
        NvM_CrcIsFirstCall = TRUE;
#endif /* STD_ON == NVM_INCLUDE_CRC */
#if (0xFFFFU != NVM_CRC_NUM_OF_BYTES)
    }
#if (STD_ON == NVM_INCLUDE_CRC)
    else
    {
        NvM_CrcIsFirstCall = FALSE;
    }
#endif /* STD_ON == NVM_INCLUDE_CRC */
    if (NVM_CRC_NUM_OF_BYTES < NvM_CurRunning.CrcLength)
    {
        l_CrcLength = NVM_CRC_NUM_OF_BYTES;
    }
    else
#endif /* 0xFFFFU != NVM_CRC_NUM_OF_BYTES */
    {
        l_CrcLength = NvM_CurRunning.CrcLength;
#if (0xFFFFU != NVM_CRC_NUM_OF_BYTES)
        NvM_CurRunning.CrcFlag = 0U;
#endif /* 0xFFFFU != NVM_CRC_NUM_OF_BYTES */
        if (0U != NvM_AtomJob.Count)
        {
            NvM_AtomJob.Count--;
        }
    }
#if (STD_ON == NVM_INCLUDE_CRC)
    switch (NvM_CurRunning.CRCType)
    {
#if (STD_ON == NVM_CRC_TYPE_8)
    case NVM_CRC8:
        NvM_CurRunning.Crc =
            Crc_CalculateCRC8(NvM_CurRunning.CrcAddr, l_CrcLength, (uint8)NvM_CurRunning.Crc, NvM_CrcIsFirstCall);
        break;
#endif /* STD_ON == NVM_CRC_TYPE_8 */
#if (STD_ON == NVM_CRC_TYPE_16)
    case NVM_CRC16:
        NvM_CurRunning.Crc =
            Crc_CalculateCRC16(NvM_CurRunning.CrcAddr, l_CrcLength, (uint16)NvM_CurRunning.Crc, NvM_CrcIsFirstCall);
        break;
#endif /* STD_ON == NVM_CRC_TYPE_16 */
#if (STD_ON == NVM_CRC_TYPE_32)
    case NVM_CRC32:
        NvM_CurRunning.Crc =
            Crc_CalculateCRC32(NvM_CurRunning.CrcAddr, l_CrcLength, NvM_CurRunning.Crc, NvM_CrcIsFirstCall);
        break;
#endif /* STD_ON == NVM_CRC_TYPE_32 */
    default:
        /*idle*/
        break;
    }
#endif /* STD_ON == NVM_INCLUDE_CRC */
    NvM_CurRunning.CrcLength -= l_CrcLength;
    NvM_CurRunning.CrcAddr = &NvM_CurRunning.CrcAddr[l_CrcLength];
}

/*
 * Brief               The module internal function to process read block request sub deal
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/* PRQA S 0554++ */ /* MISRA Rule 1.1 */
static FUNC(void, NVM_CODE) NvM_InterReadBlockSubDeal(void)
/* PRQA S 0554-- */ /* MISRA Rule 1.1 */
{
    uint32 l_TempCrc;
    NvM_CrcStaticIdStatusType l_TempCRCResult = NVM_CRC_STATICID_NO_USE;
    NvM_CrcStaticIdStatusType l_TempStaticIDResult = NVM_CRC_STATICID_NO_USE;
    uint8 StaicBlockIdCheck;

    if ((NVM_JOB_STEP_CALC_CRC_READ_1ST_NV == NvM_Module.JobStep)
        || (NVM_JOB_STEP_CALC_CRC_READ_2ND_NV == NvM_Module.JobStep))
    {
        if (NVM_CRC16 == NvM_CurRunning.CRCType)
        {
            l_TempCrc = NvM_NvDataBuffer[NvM_CurRunning.Length];
            l_TempCrc |= ((uint32)NvM_NvDataBuffer[NvM_CurRunning.Length + 1U]) << 8U;
        }
        else if (NVM_CRC32 == NvM_CurRunning.CRCType)
        {
            l_TempCrc = NvM_NvDataBuffer[NvM_CurRunning.Length];
            l_TempCrc |= ((uint32)NvM_NvDataBuffer[NvM_CurRunning.Length + 1U]) << 8U;
            l_TempCrc |= ((uint32)NvM_NvDataBuffer[NvM_CurRunning.Length + 2U]) << 16U;
            l_TempCrc |= ((uint32)NvM_NvDataBuffer[NvM_CurRunning.Length + 3U]) << 24U;
        }
        else
        {
            l_TempCrc = NvM_NvDataBuffer[NvM_CurRunning.Length];
        }
    }

    if (NVM_JOB_STEP_CHECK == NvM_Module.JobStep)
    {
        if (NvM_CurRunning.StaticId == NvM_CurRunning.BlockId)
        {
            l_TempStaticIDResult = NVM_CRC_STATICID_OK;
            NvM_Module.JobStep = NVM_JOB_STEP_COPY;
        }
        else
        {
            l_TempStaticIDResult = NVM_CRC_STATICID_NOT_OK;
        }
    }
    else if (
        (NVM_JOB_STEP_CALC_CRC_READ_1ST_NV == NvM_Module.JobStep)
        || (NVM_JOB_STEP_CALC_CRC_READ_2ND_NV == NvM_Module.JobStep))
    {
        if (NvM_CurRunning.Crc == l_TempCrc)
        {
            l_TempCRCResult = NVM_CRC_STATICID_OK;
        }
        else
        {
            l_TempCRCResult = NVM_CRC_STATICID_NOT_OK;
        }
        StaicBlockIdCheck = NvM_GetWordBitState(
            NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].FlagGroup,
            (uint8)NVM_BLOCK_DESC_STATICBLOCKIDCHECK);
        if ((uint8)STD_ON == StaicBlockIdCheck)
        {
            if (NvM_CurRunning.StaticId == NvM_CurRunning.BlockId)
            {
                l_TempStaticIDResult = NVM_CRC_STATICID_OK;
                if (NVM_CRC_STATICID_OK == l_TempCRCResult)
                {
                    NvM_Module.JobStep = NVM_JOB_STEP_COPY;
                }
            }
            else
            {
                l_TempStaticIDResult = NVM_CRC_STATICID_NOT_OK;
            }
        }
        else if (NVM_CRC_STATICID_OK == l_TempCRCResult)
        {
            NvM_Module.JobStep = NVM_JOB_STEP_COPY;
        }
        else
        {
            /*idle*/
        }
    }
    else
    {
        /*idle*/
    }

    /*. CRC Inconsist || StaticId error */
    NvM_ReadErrorHandle(l_TempCRCResult, l_TempStaticIDResult);
}

/*
 * Brief               The module internal function to process read block request
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/* PRQA S 1532,3334,0709++ */ /* MISRA Rule 8.7,Rule 5.3,Rule 1.1 */
FUNC(void, NVM_CODE) NvM_InterReadBlock(void)
/* PRQA S 1532,3334,0709-- */ /* MISRA Rule 8.7,Rule 5.3,Rule 1.1 */
{
    MemIf_JobResultType l_ReturnValue;
    if (NVM_JOB_STEP_COPY == NvM_Module.JobStep)
    {
        NvM_MemCpyStep();
    }
    else
    {
        NvM_InterReadBlockSubDeal();
        NvM_MemIfReq(NVM_MEMIF_REQ_READ);
        if (NVM_MEMIF_JOB_ASYNC_PENDING == NvM_Module.MemIfJobState) /**/
        {
            l_ReturnValue = MemIf_GetJobResult(NvM_CurRunning.DeviceId);
            (*NvM_MemIfAsyncFuncTable[l_ReturnValue][(uint8)NVM_READ_BLOCK_SERV_ID - 6U])();
        }
        if (NVM_JOB_STEP_PENDING == NvM_Module.JobStep)
        {
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
            if ((NVM_BLOCK_DATASET == NvM_CurRunning.ManagementType) && (NvM_CurRunning.NvNum <= NvM_CurRunning.Index))
            {
                NVM_GetRepeatMirrorOperation();
                NvM_Module.JobStep = NVM_JOB_STEP_READ_ROM;
                NvM_AtomJobReq(NVM_ATOMJOB_READROM);
            }
            else
#endif
            {
                NvM_Module.JobStep = NVM_JOB_STEP_READ_1ST_NV;
                NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
            }
        }
    }
}

/*
 * Brief               The module internal function to process writing block request
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/* PRQA S 1532,3334,0709++ */ /* MISRA Rule 8.7,Rule 5.3,Rule 1.1 */
FUNC(void, NVM_CODE) NvM_InterWriteBlock(void)
/* PRQA S 1532,3334,0709-- */ /* MISRA Rule 8.7,Rule 5.3,Rule 1.1 */
{
    MemIf_JobResultType l_ReturnValue;
    uint8 CalcRamBlockCrc;
    uint8 CrcCompMechanism;
    uint8 Repair;
    uint8 Valid;
    uint8 Result;
    uint8 StaicBlockIdCheck;
    boolean ProcessOn = TRUE;

    NvM_MemIfReq(NVM_MEMIF_REQ_WRITE);
    if (NVM_MEMIF_JOB_ASYNC_PENDING == NvM_Module.MemIfJobState)
    {
        l_ReturnValue = MemIf_GetJobResult(NvM_CurRunning.DeviceId);
        (*NvM_MemIfAsyncFuncTable[l_ReturnValue][(uint8)NVM_WRITE_BLOCK_SERV_ID - 6U])();
    }

    /*. First Step*/
    if (NVM_JOB_STEP_PENDING == NvM_Module.JobStep)
    {
        if (NvM_CurRunning.RepeatMirrorCounter > 0U)
        {
            if ((Std_ReturnType)E_OK == NvM_MemCpy(NvM_CurRunning.RamAddr, NvM_NvDataBuffer))
            {
                NvM_SetWordBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_WRITE_COPY_RESULT, STD_ON);
            }
            else
            { /*Repeat again*/
            }
        }
        else
        {
            NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
            NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_DELETE); /*req NVM579 837*/
#if (STD_ON == NVM_DEM_E_REQ_FAILED)
            Dem_ReportErrorStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_FAILED); /*req NVM579 837*/
#endif
        }
    }

    Result = NvM_GetWordBitState(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_WRITE_COPY_RESULT);
    if ((uint8)STD_ON == Result)
    {
        NvM_SetWordBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_WRITE_COPY_RESULT, STD_OFF);
        /*. Whether the Block use the CRC*/
        if (NVM_CRC_NOT_USED != NvM_CurRunning.CRCType)
        {
            NvM_CurRunning.TempCrc = NvM_CurRunning.Crc;
            NvM_CurRunning.CrcAddr = NvM_NvDataBuffer;
            NvM_Module.JobStep = NVM_JOB_STEP_CALC_CRC;
            NvM_AtomJobReq(NVM_ATOMJOB_CALCCRC); /*. Shift the job to the CALCCRC*/
            ProcessOn = FALSE;
        }
        else
        {
            StaicBlockIdCheck = NvM_GetWordBitState(
                NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].FlagGroup,
                (uint8)NVM_BLOCK_DESC_STATICBLOCKIDCHECK);
            /*. If Block no CRC,write nv directly*/
            if ((uint8)STD_ON == StaicBlockIdCheck)
            {
                NvM_WriteStaticBlockID(NvM_NvDataBuffer, NvM_CurRunning.CRCType);
            }
            NvM_WriteSequenceForManagementType();
        }
    }

    if ((ProcessOn == TRUE) && (NVM_JOB_STEP_CALC_CRC == NvM_Module.JobStep))
    {
        CalcRamBlockCrc = NvM_GetWordBitState(
            NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].FlagGroup,
            (uint8)NVM_BLOCK_DESC_CALCRAMBLOCKCRC);
        CrcCompMechanism = NvM_GetWordBitState(
            NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].FlagGroup,
            (uint8)NVM_BLOCK_DESC_CRCCOMPMECHANISM);
        Repair = NvM_GetWordBitState(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_NV_REPAIR);
        Valid = NvM_GetWordBitState(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_CRC_VALID);
        if (((uint8)STD_ON == CalcRamBlockCrc) && ((uint8)STD_ON == CrcCompMechanism) && ((uint8)STD_OFF == Repair)
            && ((uint8)STD_ON == Valid)) /*req 851 853 854*/
        {
            if (NvM_CurRunning.Crc == NvM_CurRunning.TempCrc)
            {
                NvM_UpdateValidandChangeStatus(STD_ON, STD_OFF);
                NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_REMAIN); /*req NVM854*/
                ProcessOn = FALSE;
            }
        }
        if (ProcessOn == TRUE)
        {
            if (NVM_CRC16 == NvM_CurRunning.CRCType)
            {
                NvM_NvDataBuffer[NvM_CurRunning.Length] = (uint8)NvM_CurRunning.Crc;
                NvM_NvDataBuffer[NvM_CurRunning.Length + 1U] = (uint8)(NvM_CurRunning.Crc >> 8U);
            }
            else if (NVM_CRC32 == NvM_CurRunning.CRCType)
            {
                NvM_NvDataBuffer[NvM_CurRunning.Length] = (uint8)NvM_CurRunning.Crc;
                NvM_NvDataBuffer[NvM_CurRunning.Length + 1U] = (uint8)(NvM_CurRunning.Crc >> 8U);
                NvM_NvDataBuffer[NvM_CurRunning.Length + 2U] = (uint8)(NvM_CurRunning.Crc >> 16U);
                NvM_NvDataBuffer[NvM_CurRunning.Length + 3U] = (uint8)(NvM_CurRunning.Crc >> 24U);
            }
            else
            {
                NvM_NvDataBuffer[NvM_CurRunning.Length] = (uint8)(NvM_CurRunning.Crc);
            }
            StaicBlockIdCheck = NvM_GetWordBitState(
                NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].FlagGroup,
                (uint8)NVM_BLOCK_DESC_STATICBLOCKIDCHECK);
            if ((uint8)STD_ON == StaicBlockIdCheck)
            {
                NvM_WriteStaticBlockID(NvM_NvDataBuffer, NvM_CurRunning.CRCType);
            }
            NvM_WriteSequenceForManagementType();
        }
    }
}

#if (STD_ON == NVM_SET_RAM_BLOCK_STATUS_API)
/*
 * Brief               The module internal function to process set ram status request
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/* PRQA S 1532,3334++ */ /* MISRA Rule 8.7,Rule 5.3 */
FUNC(void, NVM_CODE) NvM_InterSetRamStatus(void)
/* PRQA S 1532,3334-- */ /* MISRA Rule 8.7,Rule 5.3 */
{
    if (NVM_JOB_STEP_CALC_CRC == NvM_Module.JobStep)
    {
        NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_UPDATE);
    }
    else
    {
        if (NvM_CurRunning.RepeatMirrorCounter > 0U)
        {
            if ((Std_ReturnType)E_OK == NvM_MemCpy(NvM_CurRunning.RamAddr, NvM_NvDataBuffer))
            {
                NvM_CurRunning.CrcAddr = NvM_NvDataBuffer;
                NvM_Module.JobStep = NVM_JOB_STEP_CALC_CRC;
                NvM_AtomJobReq(NVM_ATOMJOB_CALCCRC);
            }
        }
        else
        {
            NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_REMAIN); /* req NVM517 579*/
#if (STD_ON == NVM_DEM_E_REQ_FAILED)
            Dem_ReportErrorStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
        }
    }
}
#endif

#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
/*
 * Brief               The module internal function to process restore block default request
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
FUNC(void, NVM_CODE) NvM_InterRestoreBlockDefaults(void) /* PRQA S 1532,3334 */ /* MISRA Rule 8.7,Rule 5.3 */
{
    if (NVM_JOB_STEP_PENDING == NvM_Module.JobStep)
    {
        if ((NULL_PTR != NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].NvmRomBlockDataAddress)
            || (NULL_PTR != NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].NvmInitBlockCallback))
        {
            NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].SingleReqResult = NVM_REQ_PENDING;
            NvM_Module.JobStep = NVM_JOB_STEP_READ_ROM;
            NvM_AtomJobReq(NVM_ATOMJOB_READROM);
        }
    }
}
#endif

#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
#if (STD_ON == NVM_JOB_PRIORITIZATION)
/*
 * Brief               The module internal function to process erase NV block request Since the GC mechanism,
 *                     Block pre-Erase before write.
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/* PRQA S 1532,3334++ */ /* MISRA Rule 8.7,Rule 5.3 */
FUNC(void, NVM_CODE) NvM_InterEraseNvBlock(void)
/* PRQA S 1532,3334-- */ /* MISRA Rule 8.7,Rule 5.3 */
{
    MemIf_JobResultType l_ReturnValue;
    /*whether this func ia active when priority strategy not used*/

    if (NVM_JOB_STEP_PENDING == NvM_Module.JobStep)
    {
        NvM_Module.JobStep = NVM_JOB_STEP_ERASE_1ST_NV;
        NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
    }
    NvM_MemIfReq(NVM_MEMIF_REQ_ERASE);
    if (NVM_MEMIF_JOB_ASYNC_PENDING == NvM_Module.MemIfJobState)
    {
        l_ReturnValue = MemIf_GetJobResult(NvM_CurRunning.DeviceId);
        (*NvM_MemIfAsyncFuncTable[l_ReturnValue][(uint8)NVM_ERASE_NV_BLOCK_SERV_ID - 6U])();
    }
}
#endif

/*
 * Brief               The module internal function to process invalidate NV block request
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
FUNC(void, NVM_CODE) NvM_InterInvalidateNvBlock(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    MemIf_JobResultType l_ReturnValue;

    if (NVM_JOB_STEP_PENDING == NvM_Module.JobStep)
    {
        NvM_Module.JobStep = NVM_JOB_STEP_INVALID_1ST_NV;
        NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
    }
    NvM_MemIfReq(NVM_MEMIF_REQ_INVALID);
    if (NVM_MEMIF_JOB_ASYNC_PENDING == NvM_Module.MemIfJobState)
    {
        l_ReturnValue = MemIf_GetJobResult(NvM_CurRunning.DeviceId);
        (*NvM_MemIfAsyncFuncTable[l_ReturnValue][(uint8)NVM_INVALIDATE_NV_BLOCK_SERV_ID - 6U])();
    }
}
#endif

/*
 * Brief               The module internal function to process read all block request
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/* PRQA S 1532,3334++ */ /* MISRA Rule 8.7,Rule 5.3 */
FUNC(void, NVM_CODE) NvM_InterReadAll(void)
/* PRQA S 1532,3334-- */ /* MISRA Rule 8.7,Rule 5.3 */
{
    uint8 l_ResistentChangedSw;
    uint8 SelectBlockForReadAll = NvM_GetWordBitState(
        NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].FlagGroup,
        (uint8)NVM_BLOCK_DESC_SELECTBLOCKFORREADALL);
    uint8 Changed = NvM_GetWordBitState(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_CHANGED);
    if (NVM_READALL_FIRST_BLOCKID == NvM_CurRunning.BlockId) /*For MultiJob,start from first block*/
    {
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
        if (((uint8)STD_ON == SelectBlockForReadAll) && ((uint8)STD_OFF == Changed)
            && (NVM_BLOCK_DATASET != NvM_CurRunning.ManagementType))
#else
        if (((uint8)STD_ON == SelectBlockForReadAll) && ((uint8)STD_OFF == Changed))
#endif
        {
            NvM_AtomJobReq(NVM_ATOMJOB_READBLOCK);
        }
        else
        {
            /* req NVM287*/
            NvM_JobOverSetFlag(NVM_REQ_BLOCK_SKIPPED, NVM_CRC_REMAIN);
        }
    }
    else if (NVM_READALL_FIRST_BLOCKID < NvM_CurRunning.BlockId)
    {
        if ((NVM_READALL_FIRST_BLOCKID + 1U) == NvM_CurRunning.BlockId)
        {
            NvM_RunTimeHandle();
        }

#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
        if (((uint8)STD_ON == SelectBlockForReadAll) && ((uint8)STD_OFF == Changed)
            && (NVM_BLOCK_DATASET != NvM_CurRunning.ManagementType))
#else
        if (((uint8)STD_ON == SelectBlockForReadAll) && ((uint8)STD_OFF == Changed))
#endif
        {
            l_ResistentChangedSw = NvM_GetWordBitState(
                NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].FlagGroup,
                (uint8)NVM_BLOCK_DESC_RESISTANTTOCHANGEDSW);

            if ((NVM_RUNTIME_EXTENDED == NvM_Module.RuntimeType) && ((uint8)STD_OFF == l_ResistentChangedSw))
            {
                NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);

                if ((NULL_PTR != NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].NvmRomBlockDataAddress)
                    || (NULL_PTR != NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].NvmInitBlockCallback))
                {
                    NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].SingleReqResult = NVM_REQ_NOT_OK;
                    NVM_GetRepeatMirrorOperation();
                    NvM_Module.JobStep = NVM_JOB_STEP_READ_ROM;
                    NvM_AtomJobReq(NVM_ATOMJOB_READROM);
                }
                else
                {
                    NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_REMAIN);
                }
            }
            else
            {
                NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
                NvM_Module.JobStep = NVM_JOB_STEP_PENDING;
                NvM_AtomJobReq(NVM_ATOMJOB_READBLOCK);
            }
        }
        else
        {
            /* req NVM287*/
            NvM_JobOverSetFlag(NVM_REQ_BLOCK_SKIPPED, NVM_CRC_REMAIN);
        }
    }
    else
    {
        /*error block id, do nothing*/
    }
}

/*
 * Brief               The module internal function to process writing block request
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/* PRQA S 1532,3334++ */ /* MISRA Rule 8.7,Rule 5.3 */
FUNC(void, NVM_CODE) NvM_InterWriteAll(void)
/* PRQA S 1532,3334-- */ /* MISRA Rule 8.7,Rule 5.3 */
{
    uint8 l_RamValid;
    uint8 l_RamChanged;
    uint8 l_NvProt;
    uint8 l_NvRepair;
    uint8 l_NvLock;
    uint8 l_NvSelect;
    uint8 Used;

    if ((uint8)STD_ON == NvM_MultiJob.CancelWriteAll)
    {
        NvM_BlockIdType blockId = NvM_CurRunning.BlockId;
        while ((uint16)NVM_BLOCK_NUM_ALL >= blockId)
        {
            /* req NVM287*/
            NvM_UpdateValidandChangeStatus(STD_ON, STD_OFF);
            NvM_SetWordBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_ENQUEUE, STD_OFF);
            NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].SingleReqResult = NVM_REQ_CANCELED;
            NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].ServiceID = NVM_NONE_SERV_ID;
            blockId++;
            NvM_CurRunning.BlockId++;
#if ((NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS) && (STD_ON == NVM_JOB_PRIORITIZATION))
            NvM_MultiJob.BlockId = NvM_CurRunning.BlockId;
#endif
        }
        if ((uint8)STD_ON == NvM_MultiJob.ID1WriteMark)
        {
            NvM_AdminBlock[0].SingleReqResult = NVM_REQ_CANCELED;
        }
#if (STD_ON == NVM_DRV_MODE_SWITCH)
        MemIf_SetMode(MEMIF_MODE_SLOW);
#endif
        /* req NVM237*/
#if (STD_ON == NVM_DEM_E_REQ_FAILED)
        Dem_ReportErrorStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
        NvM_MultiJob.CancelWriteAll = STD_OFF;
        NvM_MultiJob.Enqueue = STD_OFF;
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
        NvM_MultiJobResultFeedBack(NVM_CANCEL_WRITE_ALL_SERV_ID, NVM_REQ_CANCELED);
        NvM_MultiJob.ServiceId = NVM_NONE_SERV_ID;
#else
        NvM_MultiJobResultFeedBack(NVM_CANCEL_WRITE_ALL_SERV_ID, NVM_REQ_CANCELED);
        NvM_CurRunning.ServiceId = NVM_NONE_SERV_ID;
#endif
        NvM_Module.CurrentJobType = NVM_JOB_TYPE_NONE;
        NvM_Module.MemIfJobState = NVM_MEMIF_JOB_IDLE;
        NvM_Module.JobStep = NVM_JOB_STEP_IDLE;
    }
    else
    {
#if (STD_OFF == NVM_SET_RAM_BLOCK_STATUS_API)
        /* req NVM344 */
        /*treat the current block as valid and changed.*/
        NvM_UpdateValidandChangeStatus(STD_ON, STD_ON);
#else
        uint8 SetRamStatus = NvM_GetWordBitState(
            NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].FlagGroup,
            (uint8)NVM_BLOCK_DESC_SETRAMSTATUS);
        if ((uint8)STD_OFF == SetRamStatus)
        {
            /* req NVM344 */
            /*treat the current block as valid and changed.*/
            NvM_UpdateValidandChangeStatus(STD_ON, STD_ON);
        }
#endif
        /*if block1 need to be write,treat the block1 as valid and changed.*/
        if ((1U == NvM_CurRunning.BlockId)
            && (NvM_CurRunning.RamAddr == NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].NvmRamBlockDataAddress))
        {
            NvM_SetWordBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_VALID, STD_ON);
            NvM_SetWordBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_CHANGED, STD_ON);
        }
        l_RamValid = NvM_GetWordBitState(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_VALID);
        l_RamChanged = NvM_GetWordBitState(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_CHANGED);
        l_NvProt = NvM_GetWordBitState(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_NV_WRPRO);
        l_NvLock = NvM_GetWordBitState(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_BLOCK_LOCKED);
        l_NvRepair = NvM_GetWordBitState(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_NV_REPAIR);
        l_NvSelect = NvM_GetWordBitState(
            NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].FlagGroup,
            (uint8)NVM_BLOCK_DESC_SELECTBLOCKFORWRITEALL);

        /*req 474*/
        if (((uint8)STD_ON == l_NvRepair)
            || (((uint8)STD_ON == l_RamValid) && ((uint8)STD_ON == l_RamChanged) && ((uint8)STD_OFF == l_NvProt)
                && ((uint8)STD_OFF == l_NvLock) && ((uint8)STD_ON == l_NvSelect)
                && (NvM_CurRunning.Index < NvM_CurRunning.NvNum)))
        {
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
            if ((NVM_BLOCK_DATASET == NvM_CurRunning.ManagementType) && (NvM_CurRunning.NvNum <= NvM_CurRunning.Index))
            {
                NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
                NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_REMAIN);
            }
            else
#endif
            {
                NvM_Module.JobStep = NVM_JOB_STEP_PENDING;
                NvM_AtomJobReq(NVM_ATOMJOB_WRITEBLOCK);
            }
        }
        else
        {
            Used = NvM_GetWordBitState(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_VALID_CHANGE_STATUS_USED);
            if ((uint8)STD_ON == Used)
            {
                NvM_SetWordBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_CHANGED, STD_OFF);
            }
            NvM_JobOverSetFlag(NVM_REQ_BLOCK_SKIPPED, NVM_CRC_REMAIN);
        }
    }
}
#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"
/*
 * Brief               Update Validand Change Status
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
static FUNC(void, NVM_CODE) NvM_UpdateValidandChangeStatus(uint8 ValidStatus, uint8 ChangeStatus)
{
    uint8 Used = NvM_GetWordBitState(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_VALID_CHANGE_STATUS_USED);
    if ((uint8)STD_ON == Used)
    {
        NvM_SetWordBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_VALID, ValidStatus);
        NvM_SetWordBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_CHANGED, ChangeStatus);
    }
}

/*
 * Brief               Write Sequence For ManagementType
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
static FUNC(void, NVM_CODE) NvM_WriteSequenceForManagementType(void)
{
    if (NVM_BLOCK_REDUNDANT == NvM_CurRunning.ManagementType)
    {
        NvM_Module.JobStep = NVM_JOB_STEP_WRITE_2ND_NV;
        NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY; /*req 761 763*/
        NvM_CurRunning.Index = 1;
    }
    else
    {
        NvM_Module.JobStep = NVM_JOB_STEP_WRITE_1ST_NV;
        NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
    }
}

/*
 * Brief               Memory Copy Step
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
static FUNC(void, NVM_CODE) NvM_MemCpyStep(void) /* PRQA S 0709 */ /* MISRA Rule 1.1 */
{
    uint8 MirrorOrNot =
        NvM_GetWordBitState(NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].FlagGroup, NVM_ADMIN_RAMMIRROR_OR_NOT);
    uint8 Loss;
    if ((uint8)STD_ON == MirrorOrNot)
    {
        if (NvM_CurRunning.RepeatMirrorCounter > 0U)
        {
            if ((Std_ReturnType)E_OK == NvM_MemCpy(NvM_NvDataBuffer, NvM_CurRunning.RamAddr))
            {
                Loss = NvM_GetWordBitState(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_REDUNDANCY_LOSS);
                if ((uint8)STD_ON == Loss)
                {
                    NvM_SetWordBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_NV_REPAIR, STD_ON);
                    NvM_RepireDataSave(NvM_CurRunning.RamAddr, NvM_CurRunning.Length);
#if (STD_ON == NVM_DEM_E_LOSS_OF_REDUNDANCY)
                    Dem_ReportErrorStatus(NVM_E_LOSS_OF_REDUNDANCY, DEM_EVENT_STATUS_FAILED);
#endif
                }
                NvM_UpdateValidandChangeStatus(STD_ON, STD_OFF);
                NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_UPDATE);
            }
        }
        else if (NvM_CurRunning.RepeatMirrorCounter == 0U)
        {
            NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
            NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_REMAIN); /* req NVM517 838*/
#if (STD_ON == NVM_DEM_E_REQ_FAILED)
            Dem_ReportErrorStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
        }
        else
        {
            /*idle*/
        }
    }
    else /* STD_OFF == MirrorOrNot */
    {
        if ((Std_ReturnType)E_OK == NvM_MemCpy(NvM_NvDataBuffer, NvM_CurRunning.RamAddr))
        {
            Loss = NvM_GetWordBitState(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_REDUNDANCY_LOSS);
            if ((uint8)STD_ON == Loss)
            {
                NvM_SetWordBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_NV_REPAIR, STD_ON);
                NvM_RepireDataSave(NvM_CurRunning.RamAddr, NvM_CurRunning.Length);
#if (STD_ON == NVM_DEM_E_LOSS_OF_REDUNDANCY)
                Dem_ReportErrorStatus(NVM_E_LOSS_OF_REDUNDANCY, DEM_EVENT_STATUS_FAILED);
#endif
            }
            NvM_UpdateValidandChangeStatus(STD_ON, STD_OFF);
            NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_UPDATE);
        }
        else
        {
            NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
            NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_REMAIN);
        }
    }
}

/*
 * Brief               Sub one Inter Write Verification
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/* PRQA S 0554++ */ /* MISRA Rule 1.1 */
static FUNC(void, NVM_CODE) NvM_SuboneInterWriteVerification(void)
/* PRQA S 0554-- */ /* MISRA Rule 1.1 */
{
    Std_ReturnType l_TempStatus = E_OK;
    uint16 l_Counter;

    for (l_Counter = 0; l_Counter < NvM_CurRunning.VerificationSize; l_Counter++)
    {
        if (NvM_CurRunning.RamAddr[NvM_CurRunning.VerificationCounter + l_Counter]
            != NvM_NvDataBuffer[l_Counter + NvM_CurRunning.VerificationCounter])
        {
            NvM_SetWordBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_CRC_VALID, STD_OFF);
            l_TempStatus = E_NOT_OK;
            NvM_CurRunning.VerificationCounter = 0;
            if (NvM_CurRunning.WriteTimesCounter > 0U)
            {
                NvM_Module.JobStep = NVM_JOB_STEP_PENDING;
                NvM_SetWordBitState(
                    &NvM_CurRunning.AdminFlagGroup,
                    NVM_ADMIN_WRITE_COPY_RESULT,
                    STD_ON); /* req NVM529*/
                NvM_Module.MemIfJobState = NVM_MEMIF_JOB_IDLE;
                NvM_AtomJob.Count--;
            }
            else
            {
                NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
                NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_DELETE);
#if (STD_ON == NVM_DEM_E_REQ_FAILED)
                Dem_ReportErrorStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_FAILED); /* req NVM659 805 684*/
#endif
#if (STD_ON == NVM_DEM_E_VERIFY_FAILED)
                Dem_ReportErrorStatus(NVM_E_VERIFY_FAILED, DEM_EVENT_STATUS_FAILED); /* req NVM528 867*/
#endif
            }
            break;
        }
    }
    if ((Std_ReturnType)E_OK == l_TempStatus)
    {
        NvM_CurRunning.VerificationCounter = NvM_CurRunning.VerificationCounter + NvM_CurRunning.VerificationSize;
    }
}

/*
 * Brief               Sub two Inter Write Verification
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
static FUNC(void, NVM_CODE) NvM_SubtwoInterWriteVerification(void) /* PRQA S 0554,0709 */ /* MISRA Rule 1.1,Rule 1.1 */
{
    uint16 l_Counter;
    for (l_Counter = 0; l_Counter < (NvM_CurRunning.Length - NvM_CurRunning.VerificationCounter); l_Counter++)
    {
        if (NvM_CurRunning.RamAddr[NvM_CurRunning.VerificationCounter + l_Counter]
            != NvM_NvDataBuffer[l_Counter + NvM_CurRunning.VerificationCounter])
        {
            NvM_SetWordBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_CRC_VALID, STD_OFF);
            if (NvM_CurRunning.WriteTimesCounter > 0U)
            {
                NvM_Module.JobStep = NVM_JOB_STEP_PENDING;
                NvM_AtomJob.Count--;
                NvM_SetWordBitState(
                    &NvM_CurRunning.AdminFlagGroup,
                    NVM_ADMIN_WRITE_COPY_RESULT,
                    STD_ON); /* req NVM529*/
                NvM_Module.MemIfJobState = NVM_MEMIF_JOB_IDLE;
            }
            else
            {
                NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
                NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_DELETE);
#if (STD_ON == NVM_DEM_E_REQ_FAILED)
                Dem_ReportErrorStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_FAILED); /* req NVM659 805 684*/
#endif
#if (STD_ON == NVM_DEM_E_VERIFY_FAILED)
                Dem_ReportErrorStatus(NVM_E_VERIFY_FAILED, DEM_EVENT_STATUS_FAILED); /* req NVM528 867*/
#endif
            }
            break;
        }
    }
    NvM_CurRunning.VerificationCounter = 0;
}

/*
 * Brief               Inter Write Verification
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/* PRQA S 1532,3334++ */ /* MISRA Rule 8.7,Rule 5.3 */
FUNC(void, NVM_CODE) NvM_InterWriteVerification(void)
/* PRQA S 1532,3334-- */ /* MISRA Rule 8.7,Rule 5.3 */
{
    MemIf_JobResultType l_ReturnValue;

    NvM_MemIfReq(NVM_MEMIF_REQ_READ);
    if ((NvM_Module.JobStep == NVM_JOB_STEP_READ_1ST_NV) || (NvM_Module.JobStep == NVM_JOB_STEP_READ_2ND_NV))
    {
        if (NVM_MEMIF_JOB_ASYNC_PENDING == NvM_Module.MemIfJobState)
        {
            l_ReturnValue = MemIf_GetJobResult(NvM_CurRunning.DeviceId);
            if (MEMIF_JOB_OK == l_ReturnValue)
            {
                if ((NvM_CurRunning.VerificationCounter < NvM_CurRunning.Length)
                    && (NvM_CurRunning.VerificationSize != 0u))
                {
                    if ((NvM_CurRunning.Length - NvM_CurRunning.VerificationCounter) >= NvM_CurRunning.VerificationSize)
                    {
                        NvM_SuboneInterWriteVerification();
                    }
                    else
                    {
                        NvM_SubtwoInterWriteVerification();
                    }
                }
                else
                {
                    if (NVM_BLOCK_REDUNDANT == NvM_CurRunning.ManagementType)
                    {
                        if (NVM_JOB_STEP_READ_2ND_NV != NvM_Module.JobStep)
                        {
                            NvM_SetWordBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_NV_REPAIR, STD_OFF);
                        }
                        NvM_UpdateValidandChangeStatus(STD_ON, STD_OFF);
                        NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_UPDATE);
                    }
                    else
                    {
                        NvM_UpdateValidandChangeStatus(STD_ON, STD_OFF);
                        NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_UPDATE);
                    }
                }
            }
            else if (MEMIF_JOB_PENDING != l_ReturnValue)
            {
                NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
                NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_DELETE); /* req NVM530*/
#if (STD_ON == NVM_DEM_E_VERIFY_FAILED)
                Dem_ReportErrorStatus(NVM_E_VERIFY_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
            }
            else
            {
                /*idle*/
            }
        }
    }
}

/*
 * Brief               Get Repeat Mirror Operation
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
static FUNC(void, NVM_CODE) NVM_GetRepeatMirrorOperation(void)
{
    uint8 SyncMechanism = NvM_GetWordBitState(
        NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].FlagGroup,
        (uint8)NVM_BLOCK_DESC_SYNCMECHANISM);
    if ((uint8)STD_ON == SyncMechanism)
    {
        NvM_CurRunning.RepeatMirrorCounter = NVM_REPEAT_MIRROR_OPERATIONS;
    }
    else
    {
        NvM_CurRunning.RepeatMirrorCounter = 1;
    }
}

/*
 * Brief               Write Static BlockID
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/* PRQA S 3432,0709++ */ /* MISRA Rule 20.7,Rule 1.1 */
static FUNC(void, NVM_CODE)
    NvM_WriteStaticBlockID(P2VAR(uint8, AUTOMATIC, NVM_APPL_DATA) Dest, NvM_BlockCRCType crctype)
/* PRQA S 3432,0709-- */ /* MISRA Rule 20.7,Rule 1.1 */
{
    if (NVM_CRC_NOT_USED == crctype)
    {
        Dest[NvM_CurRunning.Length] = (uint8)(NvM_CurRunning.BlockId >> 8U);
        Dest[NvM_CurRunning.Length + 1U] = (uint8)NvM_CurRunning.BlockId;
    }
    else if (NVM_CRC8 == crctype)
    {
        Dest[NvM_CurRunning.Length + 1U] = (uint8)(NvM_CurRunning.BlockId >> 8U);
        Dest[NvM_CurRunning.Length + 2U] = (uint8)NvM_CurRunning.BlockId;
    }
    else if (NVM_CRC16 == crctype)
    {
        Dest[NvM_CurRunning.Length + 2U] = (uint8)(NvM_CurRunning.BlockId >> 8U);
        Dest[NvM_CurRunning.Length + 3U] = (uint8)NvM_CurRunning.BlockId;
    }
    else
    {
        Dest[NvM_CurRunning.Length + 4U] = (uint8)(NvM_CurRunning.BlockId >> 8U);
        Dest[NvM_CurRunning.Length + 5U] = (uint8)NvM_CurRunning.BlockId;
    }
}

/*
 * Brief               Read Static BlockID
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/* PRQA S 0709++ */ /* MISRA Rule 1.1 */
static FUNC(void, NVM_CODE)
    NvM_ReadStaticBlockID(P2CONST(uint8, AUTOMATIC, NVM_APPL_DATA) Src, NvM_BlockCRCType crctype)
/* PRQA S 0709-- */ /* MISRA Rule 1.1 */
{
    if (NVM_CRC_NOT_USED == crctype)
    {
        NvM_CurRunning.StaticId =
            (uint16)((uint16)Src[NvM_CurRunning.Length] << 8U) | (uint16)(Src[NvM_CurRunning.Length + 1U]);
    }
    else if (NVM_CRC8 == crctype)
    {
        NvM_CurRunning.StaticId =
            (uint16)((uint16)Src[NvM_CurRunning.Length + 1U] << 8U) | (uint16)(Src[NvM_CurRunning.Length + 2U]);
    }
    else if (NVM_CRC16 == crctype)
    {
        NvM_CurRunning.StaticId =
            (uint16)((uint16)Src[NvM_CurRunning.Length + 2U] << 8U) | (uint16)(Src[NvM_CurRunning.Length + 3U]);
    }
    else
    {
        NvM_CurRunning.StaticId =
            (uint16)((uint16)Src[NvM_CurRunning.Length + 4U] << 8U) | (uint16)(Src[NvM_CurRunning.Length + 5U]);
    }
}

/*
 * Brief               Read Error Handle Redundant
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/* PRQA S 0709,0554++ */ /* MISRA Rule 1.1,Rule 1.1 */
static FUNC(void, NVM_CODE)
    NvM_ReadErrorHandleRedundant(NvM_CrcStaticIdStatusType CRCResult, NvM_CrcStaticIdStatusType StaticIDResult)
/* PRQA S 0709,0554-- */ /* MISRA Rule 1.1,Rule 1.1 */
{
    if ((NVM_JOB_STEP_CALC_CRC_READ_1ST_NV == NvM_Module.JobStep) || (NVM_JOB_STEP_CHECK == NvM_Module.JobStep))
    {
        if (NvM_CurRunning.ReadRetryCounter > 0U)
        {
            NvM_CurRunning.ReadRetryCounter--;
            NvM_Module.JobStep = NVM_JOB_STEP_READ_1ST_NV;
            NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
        }
        else
        {
            NvM_CurRunning.ReadRetryCounter = NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].NvMMaxNumOfReadRetries;
            /*Repeat time is N+1 via code structure*/
            NvM_Module.JobStep = NVM_JOB_STEP_READ_2ND_NV;
            NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY; /* req NVM526 582*/
            NvM_CurRunning.Index = 1;
        }
    }
    else
    {
        if (NvM_CurRunning.ReadRetryCounter > 0U)
        {
            NvM_CurRunning.ReadRetryCounter--;
            NvM_Module.JobStep = NVM_JOB_STEP_READ_2ND_NV;
            NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
        }
        else if (
            (NULL_PTR != NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].NvmRomBlockDataAddress)
            || (NULL_PTR != NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].NvmInitBlockCallback))
        {
            NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF); /* req NVM317*/
            NvM_CurRunning.ESingleReqResult = NVM_REQ_INTEGRITY_FAILED;
            if (NVM_CRC_STATICID_NOT_OK == CRCResult)
            {
#if (STD_ON == NVM_DEM_E_INTEGRITY_FAILED)
                Dem_ReportErrorStatus(NVM_E_INTEGRITY_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
#if (STD_ON == NVM_DEM_E_HARDWARE)
                Dem_ReportErrorStatus(NVM_E_HARDWARE, DEM_EVENT_STATUS_FAILED);
#endif
            }
            if (NVM_CRC_STATICID_NOT_OK == StaticIDResult)
            {
#if (STD_ON == NVM_DEM_E_WRONG_BLOCK_ID)
                Dem_ReportErrorStatus(NVM_E_WRONG_BLOCK_ID, DEM_EVENT_STATUS_FAILED);
#endif
            }
            NVM_GetRepeatMirrorOperation();
            NvM_Module.JobStep = NVM_JOB_STEP_READ_ROM;
            NvM_AtomJobReq(NVM_ATOMJOB_READROM); /* req NVM581 583*/
        }
        else
        {
            if (NVM_CRC_STATICID_NOT_OK == CRCResult)
            {
#if (STD_ON == NVM_DEM_E_INTEGRITY_FAILED)
                Dem_ReportErrorStatus(NVM_E_INTEGRITY_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
#if (STD_ON == NVM_DEM_E_HARDWARE)
                Dem_ReportErrorStatus(NVM_E_HARDWARE, DEM_EVENT_STATUS_FAILED);
#endif
            }
            if (NVM_CRC_STATICID_NOT_OK == StaticIDResult)
            {
#if (STD_ON == NVM_DEM_E_WRONG_BLOCK_ID)
                Dem_ReportErrorStatus(NVM_E_WRONG_BLOCK_ID, DEM_EVENT_STATUS_FAILED);
#endif
            }
            NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
            NvM_JobOverSetFlag(NVM_REQ_INTEGRITY_FAILED, NVM_CRC_DELETE);
        }
    }
}

/*
 * Brief               Read Error Handle
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/* PRQA S 0709++ */ /* MISRA Rule 1.1 */
static FUNC(void, NVM_CODE)
    NvM_ReadErrorHandle(NvM_CrcStaticIdStatusType CRCResult, NvM_CrcStaticIdStatusType StaticIDResult)
/* PRQA S 0709-- */ /* MISRA Rule 1.1 */
{
    if ((NVM_JOB_STEP_CHECK == NvM_Module.JobStep) || (NVM_JOB_STEP_CALC_CRC_READ_1ST_NV == NvM_Module.JobStep)
        || (NVM_JOB_STEP_CALC_CRC_READ_2ND_NV == NvM_Module.JobStep))
    {
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
        /***************DATASET Block Type********************/
        if (NVM_BLOCK_DATASET == NvM_CurRunning.ManagementType)
        {
            if (NvM_CurRunning.ReadRetryCounter > 0U)
            {
                NvM_CurRunning.ReadRetryCounter--;
                NvM_Module.JobStep = NVM_JOB_STEP_READ_1ST_NV;
                NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY; /* req NVM581 583*/
            }
            else
            {
                NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
                if (NVM_CRC_STATICID_NOT_OK == CRCResult)
                {
#if (STD_ON == NVM_DEM_E_INTEGRITY_FAILED)
                    Dem_ReportErrorStatus(NVM_E_INTEGRITY_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
#if (STD_ON == NVM_DEM_E_HARDWARE)
                    Dem_ReportErrorStatus(NVM_E_HARDWARE, DEM_EVENT_STATUS_FAILED);
#endif
                }
                if (NVM_CRC_STATICID_NOT_OK == StaticIDResult)
                {
#if (STD_ON == NVM_DEM_E_WRONG_BLOCK_ID)
                    Dem_ReportErrorStatus(NVM_E_WRONG_BLOCK_ID, DEM_EVENT_STATUS_FAILED);
#endif
                }
                NvM_JobOverSetFlag(NVM_REQ_INTEGRITY_FAILED, NVM_CRC_DELETE);
            }
        }
        /***************NATIVE Block Type********************/
        else if (NVM_BLOCK_NATIVE == NvM_CurRunning.ManagementType)
#else
        if (NVM_BLOCK_NATIVE == NvM_CurRunning.ManagementType)
#endif
        {
            if (NvM_CurRunning.ReadRetryCounter > 0U)
            {
                NvM_CurRunning.ReadRetryCounter--;
                NvM_Module.JobStep = NVM_JOB_STEP_READ_1ST_NV;
                NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY; /* req NVM581 583*/
            }
            else
            {
                NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF); /* req NVM317*/
                NvM_CurRunning.ESingleReqResult = NVM_REQ_INTEGRITY_FAILED;
                if (NVM_CRC_STATICID_NOT_OK == CRCResult)
                {
#if (STD_ON == NVM_DEM_E_INTEGRITY_FAILED)
                    Dem_ReportErrorStatus(NVM_E_INTEGRITY_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
#if (STD_ON == NVM_DEM_E_HARDWARE)
                    Dem_ReportErrorStatus(NVM_E_HARDWARE, DEM_EVENT_STATUS_FAILED);
#endif
                }
                if (NVM_CRC_STATICID_NOT_OK == StaticIDResult)
                {
#if (STD_ON == NVM_DEM_E_WRONG_BLOCK_ID)
                    Dem_ReportErrorStatus(NVM_E_WRONG_BLOCK_ID, DEM_EVENT_STATUS_FAILED);
#endif
                }

                if ((NULL_PTR != NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].NvmRomBlockDataAddress)
                    || (NULL_PTR != NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].NvmInitBlockCallback))
                {
                    NVM_GetRepeatMirrorOperation();
                    NvM_Module.JobStep = NVM_JOB_STEP_READ_ROM;
                    NvM_AtomJobReq(NVM_ATOMJOB_READROM); /* req NVM581 583*/
                }
                else
                {
                    NvM_JobOverSetFlag(NVM_REQ_INTEGRITY_FAILED, NVM_CRC_DELETE);
                }
            }
        }
        /***************Redundant Block Type********************/
        else
        {
            NvM_ReadErrorHandleRedundant(CRCResult, StaticIDResult);
        }
    }
}

/*
 * Brief               Single Job Result FeedBack
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
static FUNC(void, NVM_CODE) NvM_SingleJobResultFeedBack(NvM_BlockIdType BlockId, NvM_RequestResultType Result)
{
    /*Only for single job*/
#if (STD_ON == NVM_BSWM_SINGLEBLOCK_JOBSTATUS_INFORMATION)
    uint8 BswBlockStatusInfo =
        NvM_GetWordBitState(NvM_BlockDescriptor[BlockId - 1U].FlagGroup, (uint8)NVM_BLOCK_DESC_BSWBLOCKSTATUSINFO);
    if (((uint8)STD_ON == BswBlockStatusInfo) && (NVM_READ_ALL_SERV_ID != NvM_AdminBlock[BlockId - 1U].ServiceID)
        && (NVM_WRITE_ALL_SERV_ID != NvM_AdminBlock[BlockId - 1U].ServiceID)
        && (NVM_VALIDATE_ALL_SERV_ID != NvM_AdminBlock[BlockId - 1U].ServiceID))
    {
        BswM_NvM_CurrentBlockMode(BlockId, Result);
    }
#endif
    NvM_AdminBlock[BlockId - 1U].SingleReqResult = Result;
}

/*
 * Brief               Multi Job Result FeedBack
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/* PRQA S 0709++ */ /* MISRA Rule 1.1 */
static FUNC(void, NVM_CODE) NvM_MultiJobResultFeedBack(NvM_ServiceIdType ServiceId, NvM_RequestResultType Result)
/* PRQA S 0709-- */ /* MISRA Rule 1.1 */
{
    NvM_MultiBlockRequestType NotiServiceId = NVM_READ_ALL;
    NvM_MultiJob.ReqResult = Result;
    switch (ServiceId)
    {
    case NVM_WRITE_ALL_SERV_ID:
        NotiServiceId = NVM_WRITE_ALL;
        break;
    case NVM_VALIDATE_ALL_SERV_ID:
        NotiServiceId = NVM_VALIDATE_ALL;
        break;
    case NVM_CANCEL_WRITE_ALL_SERV_ID:
        NotiServiceId = NVM_CANCEL_WRITE_ALL;
        break;
    default:
        /* NVM_READ_ALL_SERV_ID or other */
        break;
    }
#if (STD_ON == NVM_BSWM_MULTIBLOCK_JOBSTATUS_INFORMATION)
    BswM_NvM_CurrentJobMode(NotiServiceId, Result); /*PRQA S 4442*/
#else
    if (NULL_PTR != NvmMultiBlockCallback)
    {
        /* call the callback function to notify the up layer after job processing is over */
        (*NvmMultiBlockCallback)(NotiServiceId, Result);
    }
#endif
}

/*
 * Brief               Repire Data Save
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/* PRQA S 0709++ */ /* MISRA Rule 1.1 */
static FUNC(void, NVM_CODE) NvM_RepireDataSave(P2CONST(uint8, TYPEDEF, NVM_APPL_DATA) DataSourceAddress, uint16 Length)
/* PRQA S 0709-- */ /* MISRA Rule 1.1 */
{
    uint16 l_LengthLoop;
    uint8* l_Dest = &NVM_TemporaryRAMForRepaire[NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].RepaireIndex][0];

    for (l_LengthLoop = 0; l_LengthLoop < Length; l_LengthLoop++)
    {
        l_Dest[l_LengthLoop] = DataSourceAddress[l_LengthLoop];
    }
}

#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
/*
 * Brief               The module internal function to process validating all block request
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/* PRQA S 1532,3334,0709++ */ /* MISRA Rule 8.7,Rule 5.3,Rule 1.1 */
FUNC(void, NVM_CODE) NvM_InterValidateAll(void)
/* PRQA S 1532,3334,0709-- */ /* MISRA Rule 8.7,Rule 5.3,Rule 1.1 */
{
    uint8 Repair = NvM_GetWordBitState(NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].FlagGroup, NVM_ADMIN_NV_REPAIR);
    uint8 AutoValidation;
    uint8 Valid;
    uint8 SyncMechanism;
    uint8 CalcRamBlockCrc;
    if (NVM_JOB_STEP_CALC_CRC == NvM_Module.JobStep)
    {
        if ((uint8)STD_ON == Repair)
        {
            NvM_SetWordBitState(&NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].FlagGroup, NVM_ADMIN_NV_REPAIR, STD_OFF);
        }
        NvM_UpdateValidandChangeStatus(STD_ON, STD_ON);
        NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_UPDATE);
    }
    else
    {
        AutoValidation = NvM_GetWordBitState(
            NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].FlagGroup,
            (uint8)NVM_BLOCK_DESC_AUTOVALIDATION);
        Valid = NvM_GetWordBitState(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_VALID);
        SyncMechanism = NvM_GetWordBitState(
            NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].FlagGroup,
            (uint8)NVM_BLOCK_DESC_SYNCMECHANISM);
        if (((uint8)STD_ON == AutoValidation) && ((uint8)STD_ON == Valid)
            && ((NULL_PTR != NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].NvmRamBlockDataAddress)
                || ((uint8)STD_ON == SyncMechanism)))
        {
            CalcRamBlockCrc = NvM_GetWordBitState(
                NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].FlagGroup,
                (uint8)NVM_BLOCK_DESC_CALCRAMBLOCKCRC);
            if ((uint8)STD_OFF == CalcRamBlockCrc)
            {
                Repair =
                    NvM_GetWordBitState(NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].FlagGroup, NVM_ADMIN_NV_REPAIR);
                if ((uint8)STD_ON == Repair)
                {
                    NvM_SetWordBitState(
                        &NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].FlagGroup,
                        NVM_ADMIN_NV_REPAIR,
                        STD_OFF);
                }
                NvM_UpdateValidandChangeStatus(STD_ON, STD_ON);
                NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_REMAIN);
            }
            else if (NvM_CurRunning.RepeatMirrorCounter > 0U)
            {
                if ((Std_ReturnType)E_OK == NvM_MemCpy(NvM_CurRunning.RamAddr, NvM_NvDataBuffer))
                {
                    NvM_CurRunning.CrcAddr = NvM_NvDataBuffer;
                    /*. After copy nvmbuffer to ram or mirror*/
                    NvM_Module.JobStep = NVM_JOB_STEP_CALC_CRC;
                    NvM_AtomJobReq(NVM_ATOMJOB_CALCCRC);
                }
            }
            else
            {
                NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_REMAIN);
            }
        }
        else
        {
            NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_REMAIN);
        }
    }
}
#endif
#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"
/*====================[E N D  O F   F I L E]====================*/
