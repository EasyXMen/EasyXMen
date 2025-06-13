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
**  FILENAME    : NvM_Types.h                                                **
**                                                                           **
**  Created on  : 2020/5/9 15:21:52                                          **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION : Providing the types for the NvM module                     **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                     **
**                                                                           **
**************************************************************************** */
#ifndef NVM_TYPES_H
#define NVM_TYPES_H

/*================================[M A C R O S]===============================*/
#define NVM_API_CONFIG_CLASS_1 1U /* All API calls belonging to class 1 */
#define NVM_API_CONFIG_CLASS_2 2U /* All API calls belonging to class 2 */
#define NVM_API_CONFIG_CLASS_3 3U /* All API calls belonging to class 3 */
/*Mark: The above three API class define must located before NvM_Cfg.h include*/

/*==============================[I N C L U D E S]=============================*/
#include "NvM_Cfg.h"
#include "Rte_NvM_Type.h"

#define USELESS_FOR_MULTIJOB 0U

/*Development Errors*/
#define NVM_E_PARAM_BLOCK_ID            (0x0Au)
#define NVM_E_PARAM_BLOCK_TYPE          (0x0Bu)
#define NVM_E_PARAM_BLOCK_DATA_IDX      (0x0Cu)
#define NVM_E_PARAM_ADDRESS             (0x0Du)
#define NVM_E_PARAM_DATA                (0x0Eu)
#define NVM_E_PARAM_POINTER             (0x0Fu)
#define NVM_E_BLOCK_WITHOUT_DEFAULTS    (0x11u)
#define NVM_E_UNINIT                    (0x14u)
#define NVM_E_BLOCK_PENDING             (0x15u)
#define NVM_E_BLOCK_CONFIG              (0x18u)
#define NVM_E_BLOCK_LOCKED              (0x19u)
#define NVM_E_WRITE_ONCE_STATUS_UNKNOWN (0x1Au)
#define NVM_E_WRITE_PROTECTED_DET       (0x1Bu)

/*Runtime Errors*/
#define NVM_E_QUEUE_FULL (0xA0u)

/*Block CRC type enum type define*/
typedef enum
{
    NVM_CRC_NOT_USED,
    NVM_CRC8,
    NVM_CRC16,
    NVM_CRC32
} NvM_BlockCRCType;

/* Block management type enum type define*/
typedef enum
{
    NVM_BLOCK_NATIVE = 1,
    NVM_BLOCK_REDUNDANT,
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
    NVM_BLOCK_DATASET
#endif
} NvM_BlockManagementType;

/*Block Descriptor Flag type enum type define*/
typedef enum
{
    NVM_BLOCK_DESC_WRITEBLOCKONCE,
    NVM_BLOCK_DESC_WRITEPROT,
    NVM_BLOCK_DESC_CALCRAMBLOCKCRC,
    NVM_BLOCK_DESC_RESISTANTTOCHANGEDSW,
    NVM_BLOCK_DESC_SELECTBLOCKFORREADALL,
    NVM_BLOCK_DESC_SELECTBLOCKFORWRITEALL,
    NVM_BLOCK_DESC_STATICBLOCKIDCHECK,
    NVM_BLOCK_DESC_WRITEVERIFICATION,
    NVM_BLOCK_DESC_AUTOVALIDATION,
    NVM_BLOCK_DESC_CRCCOMPMECHANISM,
    NVM_BLOCK_DESC_SETRAMSTATUS,
    NVM_BLOCK_DESC_SYNCMECHANISM,
    NVM_BLOCK_DESC_BSWBLOCKSTATUSINFO
} NvM_BlockDescrptorFlagType;

/*Type of memif module service requested*/
typedef enum
{
    NVM_MEMIF_REQ_READ,
    NVM_MEMIF_REQ_WRITE,
    NVM_MEMIF_REQ_ERASE,
    NVM_MEMIF_REQ_INVALID
} NvM_MemIfReqType;

typedef enum
{
    NVM_READ_ALL,        /*NvM_ReadAll was performed*/
    NVM_WRITE_ALL,       /*NvM_WriteAll was performed*/
    NVM_VALIDATE_ALL,    /*NvM_ValidateAll was performed*/
    NVM_FIRST_INIT_ALL,  /*NvM_FirstInitAll was performed*/
    NVM_CANCEL_WRITE_ALL /*NvM_CancelWriteAll was performed*/
} NvM_MultiBlockRequestType;

/*
 * Brief    Per block callback routine which shall be called by the NvM module to
 *          copy default data to a RAM block if a ROM block isn't configured.
 *
 * Return   E_OK: Callback function has been processed successfully
 *          E_NOT_OK: Callback function has not been processed successfully.
 */
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
typedef P2FUNC(Std_ReturnType, NVM_APPL_CODE, NvM_InitBlockCallbackType)(
    NvM_InitBlockRequestType InitBlockRequest); /* A pointer to initial callback function.*/
/* PRQA S 3432-- */                             /* MISRA Rule 20.7 */

/*
 * Brief    Per block callback routine to notify the upper layer that
 *          an asynchronous single block request has been finished.
 *
 * Return   E_OK: Callback function has been processed successfully
 *          E_NOT_OK: Callback function has not been processed successfully.
 */
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
typedef P2FUNC(Std_ReturnType, NVM_APPL_CODE, NvM_SingleBlockCallbackType)(
    NvM_BlockRequestType BlockRequest,
    NvM_RequestResultType JobResult);
/* A pointer to single block callback function.*/
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*
 * Brief     Entry address of a block specific callback routine which shall be called in
 *            order to let the application copy data from the NvM module's mirror to RAM block
 *
 * Return   E_OK: Callback function has been processed successfully
 *          E_NOT_OK:Callback function has not been processed successfully
 */
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
typedef P2FUNC(Std_ReturnType, NVM_APPL_CODE, NvM_ReadRamBlockFromNvmCallbackType)(const void* NvMBuffer);
/* A pointer to copy ram to ram mirror callback function.*/
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*
 * Brief     Entry address of a block specific callback routine which shall be called in
 *          order to let the application copy data from RAM block to NvM module's mirror.
 *
 * Return   E_OK: Callback function has been processed successfully
 *          E_NOT_OK:Callback function has not been processed successfully
 */
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
typedef P2FUNC(Std_ReturnType, NVM_APPL_CODE, NvM_WriteRamBlockToNvmCallbackType)(void* NvMBuffer);
/* A pointer to copy ram to ram mirror callback function.*/
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*
 * Common callback routine to notify the upper layer that
 * an asynchronous multi block request has been finished.
 */
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
typedef P2FUNC(void, NVM_APPL_CODE, NvM_MultiBlockCallbackType)(
    NvM_MultiBlockRequestType MultiBlockRequest,
    NvM_RequestResultType JobResult);
/* A pointer to multi block callback function.*/
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*
 * Function Pointer type define, with void parameter and void return value.
 */
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
typedef P2FUNC(void, NVM_CODE, NvM_VoidFuncVoidPtr)(void);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

#if (STD_ON == NVM_JOB_PRIORITIZATION)
/*The job queue table size*/
#define NVM_TABLE_SIZE_JOB_QUEUE ((NVM_SIZE_STANDARD_JOB_QUEUE + 16u) / 16u) + 1u

#endif

/*
 * The CRC space byte size
 */
#define NVM_NV_CRC_MAX_LENGTH                  4U
#define NVM_NV_BLOCK_ID_LENGTH                 2U

#define NVM_ADMIN_ENQUEUE                      0U
#define NVM_ADMIN_RAM_VALID                    1U
#define NVM_ADMIN_RAM_CHANGED                  2U
#define NVM_ADMIN_NV_WRPRO                     3U
#define NVM_ADMIN_NV_REPAIR                    4U
#define NVM_ADMIN_RAM_VALID_CHANGE_STATUS_USED 5U
#define NVM_ADMIN_BLOCK_LOCKED                 6U
#define NVM_ADMIN_REDUNDANCY_LOSS              7U
#define NVM_ADMIN_RAMMIRROR_OR_NOT             8U
#define NVM_ADMIN_CRC_VALID                    9U
#define NVM_ADMIN_WRITE_COPY_RESULT            10U

#if (STD_OFF == NVM_DYNAMIC_CONFIGURATION)
#define NVM_READALL_FIRST_BLOCKID 0x02U
#else
#define NVM_READALL_FIRST_BLOCKID 0x01U
#endif

#define NVM_VALIDATEALL_FIRST_BLOCKID 0x01U
#define NVM_WRITEALL_FIRST_BLOCKID    0x02U
#define NVM_QUEUE_EMPTY_COUNT         0U

/*Type of NvM module service requested*/
typedef enum
{
    NVM_INIT_SERV_ID = 0x0,
    NVM_SET_DATA_INDEX_SERV_ID,
    NVM_GET_DATA_INDEX_SERV_ID,
    NVM_SET_BLOCK_PROTECTION_SERV_ID,
    NVM_GET_ERROR_STATUS_SERV_ID,
    NVM_SET_RAM_BLOCK_STATUS_SERV_ID,
    NVM_READ_BLOCK_SERV_ID,
    NVM_WRITE_BLOCK_SERV_ID,
    NVM_RESTORE_BLOCK_DEFAULTS_SERV_ID,
    NVM_ERASE_NV_BLOCK_SERV_ID,
    NVM_CANCEL_WRITE_ALL_SERV_ID,
    NVM_INVALIDATE_NV_BLOCK_SERV_ID,
    NVM_READ_ALL_SERV_ID,
    NVM_WRITE_ALL_SERV_ID,
    NVM_CANCEL_JOBS_SERV_ID = 0x10,
    NVM_SET_BLOCK_LOCK_STATUS_SERV_ID = 0x13,
    NVM_READ_PRAM_BLOCK_SERV_ID = 0x16,
    NVM_WRITE_PRAM_BLOCK_SERV_ID,
    NVM_RESTORE_PRAM_BLOCK_DEFAULTS_SERV_ID,
    NVM_VALIDATE_ALL_SERV_ID,
    NVM_NONE_SERV_ID
} NvM_ServiceIdType;

/*Types of tasks that are automated within the NvM module*/
typedef enum
{
    NVM_ATOMJOB_READBLOCK,
    NVM_ATOMJOB_WRITEBLOCK,
    NVM_ATOMJOB_READROM,
    NVM_ATOMJOB_CALCCRC,
    NVM_ATOMJOB_WRITEVERIFICATION,
    NVM_ATOMJOB_MAX_NONE
} NvM_AtomJobEType;

/*Types of jobs processed within the NvM module*/
typedef enum
{
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
    NVM_JOB_TYPE_SINGLE_NORMAL,
#if (STD_ON == NVM_JOB_PRIORITIZATION)
    NVM_JOB_TYPE_SINGLE_IMMED,
    NVM_JOB_TYPE_MULTI_GAP_SINGLE_IMMED,
#endif
#endif
    NVM_JOB_TYPE_MULTI,
    NVM_JOB_TYPE_NONE
} NvM_JobTypeType;

/*Types of NvM module job steps*/
typedef enum
{
    NVM_JOB_STEP_READ_1ST_NV,
    NVM_JOB_STEP_READ_2ND_NV,
    NVM_JOB_STEP_WRITE_1ST_NV,
    NVM_JOB_STEP_WRITE_2ND_NV,
    NVM_JOB_STEP_ERASE_1ST_NV,
    NVM_JOB_STEP_ERASE_2ND_NV,
    NVM_JOB_STEP_INVALID_1ST_NV,
    NVM_JOB_STEP_INVALID_2ND_NV,
    NVM_JOB_STEP_READ_ROM,
    NVM_JOB_STEP_CALC_CRC_READ_1ST_NV,
    NVM_JOB_STEP_CALC_CRC_READ_2ND_NV,
    NVM_JOB_STEP_CALC_CRC,
    NVM_JOB_STEP_IDLE, /*Task has been finished*/
    NVM_JOB_STEP_PENDING,
    NVM_JOB_STEP_CANCEL,
    NVM_JOB_STEP_COPY,
    NVM_JOB_STEP_WRITE_VERIFICATION,
    NVM_JOB_STEP_CHECK
} NvM_JobStepType;

/*The NvM module requests the type of mode the memif module is in after the lower level*/
typedef enum
{
    NVM_MEMIF_JOB_IDLE,
    NVM_MEMIF_JOB_ASYNC_READY,
    NVM_MEMIF_JOB_ASYNC_PENDING
} NvM_MemIfStateType;

/*Mode in which the NvM module is running*/
typedef enum
{
    NVM_RUNTIME_NONE,
    NVM_RUNTIME_EXTENDED,
    NVM_RUNTIME_NORMAL
} NvM_RuntimeType;

/*NvM Module Schema Information Type*/
typedef struct
{
    uint8 Init;
#if ((NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS) && (STD_ON == NVM_JOB_PRIORITIZATION))
    uint8 CurActPriTable;
#endif
    NvM_JobTypeType CurrentJobType;
    NvM_RuntimeType RuntimeType;
    NvM_MemIfStateType MemIfJobState;
    NvM_JobStepType JobStep;
} NvM_ModuleType;

/*NvM Module Multi-Block Task Mode Information Type*/
typedef struct
{
    uint8 Enqueue;
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
    NvM_ServiceIdType ServiceId;
#if (STD_ON == NVM_JOB_PRIORITIZATION)
    NvM_BlockIdType BlockId;
#endif
#endif
    uint8 CancelWriteAll;
    uint8 ID1WriteMark;
    NvM_RequestResultType ReqResult;
} NvM_MultiJobType;

/*NvM Module Automated Task Record Information Type*/
typedef struct
{
    uint8 Count;
    NvM_AtomJobEType ReqId[NVM_ATOMJOB_MAX_NONE];
} NvM_AtomJobType;

/*Type of block-related information currently processed by the NvM module*/
typedef struct
{
    NvM_BlockIdType BlockId;
    NvM_BlockIdType StaticId;
    NvM_BlockIdType BlockIdIndex;
    uint8 DeviceId;
    NvM_ServiceIdType ServiceId;
    uint8 Index;
    uint8 NvNum;
    uint8 RomNum;
    uint16 AdminFlagGroup;
    uint8 ReadRetryCounter;
    uint8 WriteTimesCounter;
    uint8 RepeatMirrorCounter;
    NvM_RequestResultType ESingleReqResult;
    NvM_RequestResultType EMultiReqResult;
    NvM_BlockManagementType ManagementType;
    NvM_BlockCRCType CRCType;
    uint16 BaseNumber;
    uint16 Length;
    uint16 VerificationSize;
    uint16 VerificationCounter;
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, TYPEDEF, NVM_APPL_DATA) RamAddr;
    P2VAR(uint8, TYPEDEF, NVM_APPL_DATA) RomAddr;
    uint8 CrcFlag;
    P2VAR(uint8, TYPEDEF, NVM_VAR) CrcAddr;
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    uint16 CrcLength;
    uint32 Crc;
    uint32 TempCrc;
    NvM_ReadRamBlockFromNvmCallbackType NvM_ReadRamBlockFromNvm;
    NvM_WriteRamBlockToNvmCallbackType NvM_WriteRamBlockToNvm;
    NvM_InitBlockCallbackType InitCallback;
    NvM_SingleBlockCallbackType SingleCallback;
} NvM_CurRunningType;

/*NvM Module Management Block Information Type*/
typedef struct
{
    uint16 FlagGroup;
    uint8 CurrentIndex;
    NvM_ServiceIdType ServiceID;
    NvM_RequestResultType SingleReqResult;
    uint32 Crc;
    boolean ReadReq;
} NvM_AdminBlockType;

#if (STD_ON == NVM_JOB_PRIORITIZATION)
/*NvM Module Priority List Record Information Type*/
typedef struct
{
    uint8 HeadIndex;
    uint8 TailIndex;
} NvM_PriTable2QueueType;

/*NvM Module Immediate Queue Type*/
typedef struct
{
    NvM_BlockIdType BlockId;
    P2VAR(uint8, TYPEDEF, NVM_APPL_DATA) DestSrcPtr; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
} NvM_ImmedQueueType;
#endif

#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
/*NvM Module Queue Management Message Type*/
typedef struct
{
    uint8 Count;
    uint8 HeadIndex;
    uint8 TailIndex;
} NvM_RoundRobinQueueManageType;

/*NvM Module Standard Queue Types*/
typedef struct
{
#if (STD_ON == NVM_JOB_PRIORITIZATION)
    uint8 NextIndex;
#endif
    NvM_BlockIdType BlockId;
    NvM_ServiceIdType ServiceId;
    P2VAR(uint8, TYPEDEF, NVM_APPL_DATA) DestSrcPtr; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
} NvM_StandQueueType;
#endif

typedef struct
{
    uint8 NvmNvramDeviceId;
    NvM_BlockManagementType NvmBlockManagementType;
    NvM_BlockIdType RepaireIndex;
#if ((NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS) && (STD_ON == NVM_JOB_PRIORITIZATION))
    uint8 NvmBlockJobPriority;
#endif
    /*
     * Bit 0:NvmWriteBlockOnce/
     * Bit 1:NvmBlockWriteProt /
     * Bit 2:NvmCalcRamBlockCrc /
     * Bit 3:NvmResistantToChangedSw /
     * Bit 4:NvmSelectBlockForReadall /
     * Bit 5:NvmSelectBlockForWriteall /
     * Bit 6:NvMStaticBlockIDCheck
     * Bit 7:NvMWriteVerification
     * Bit 8:NvMBlockUseAutoValidation
     * Bit 9:NvMBlockUseCRCCompMechanism
     * Bit 10:NvMBlockUseSetRamBlockStatus
     * Bit 11:NvMBlockUseSyncMechanism
     * Bit 12:NvMBswMBlockStatusInformation
     */
    uint16 FlagGroup;
    NvM_BlockCRCType NvmBlockCRCType;
    uint16 NvmNvBlockBaseNumber;
    uint16 NvmNvBlockLength;
    uint8 NvmNvBlockNum;
    uint8 NvmRomBlockNum;
    uint8 NvMMaxNumOfReadRetries;
    uint8 NvMMaxNumOfWriteRetries;
    uint16 NvMWriteVerificationDataSize;
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, TYPEDEF, NVM_APPL_DATA) NvmRamBlockDataAddress;
    P2VAR(uint8, TYPEDEF, NVM_APPL_DATA) NvmRomBlockDataAddress;
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    NvM_InitBlockCallbackType NvmInitBlockCallback;
    NvM_SingleBlockCallbackType NvmSingleBlockCallback;
    NvM_ReadRamBlockFromNvmCallbackType NvM_ReadRamBlockFromNvm;
    NvM_WriteRamBlockToNvmCallbackType NvM_WriteRamBlockToNvm;
} NvM_BlockDescriptorType;

/*NvM module CRC result processing method type*/
typedef enum
{
    NVM_CRC_REMAIN,
    NVM_CRC_DELETE,
    NVM_CRC_UPDATE
} NvM_CrcChangeType;

/*NvM module CRC and static ID check result types*/
typedef enum
{
    NVM_CRC_STATICID_NO_USE,
    NVM_CRC_STATICID_NOT_OK,
    NVM_CRC_STATICID_OK
} NvM_CrcStaticIdStatusType;

typedef struct
{
    uint8 idle; /*not used*/
} NvM_ConfigType;

#endif /* End of NVM_TYPES_H*/

/*====================[E N D   O F   F I L E]====================*/
