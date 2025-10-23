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
/*
 ************************************************************************************************************************
 **
 **  @file               : NvM_Inter.h
 **  @author             : peng.wu
 **  @date               : 2024/08/27
 **  @vendor             : isoft
 **  @description        : NVRAM manager internal declarations
 **
 ***********************************************************************************************************************/
#ifndef NVM_INTER_H
#define NVM_INTER_H

/* =================================================== inclusions =================================================== */
#include "NvM.h"
#if (STD_ON == NVM_DEV_ERROR_DETECT)
#include "Det.h"
#endif
#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */

/* ===================================================== macros ===================================================== */
/* PRQA S 3414 ++ */ /* VL_NvM_3414 */
#ifndef NVM_LOCAL
#define NVM_LOCAL static /**< Defines used as the local keyword for variables or functions */
#endif
/* PRQA S 3414 -- */

#ifndef NVM_LOCAL_INLINE
#define NVM_LOCAL_INLINE static inline /**< Defines used as keyword for the local inline functions */
#endif

#if !defined(NVM_PARAM_UNUSED)
#define NVM_PARAM_UNUSED(x) (void)(x) /**< Defines used as keyword for the unused variables */
#endif

#define USELESS_FOR_MULTIJOB 0U /**< Defines as parameters for the not used multi job */

#if (STD_ON == NVM_JOB_PRIORITIZATION)
#define NVM_TABLE_SIZE_JOB_QUEUE \
    ((((NVM_SIZE_STANDARD_JOB_QUEUE + 16u)) / 16u) + 1u) /**< Defined as parameters for the job queue table size */
#endif

#define NVM_NV_CRC_MAX_LENGTH 4U /**< Defines used as parameters for the CRC space byte size */

/** Defines used as parameters for the Administrative block flag */
#define NVM_ADMIN_ENQUEUE                      0U  /**< Enqueue flag */
#define NVM_ADMIN_RAM_VALID                    1U  /**< Ram valid flag */
#define NVM_ADMIN_RAM_CHANGED                  2U  /**< Ram changed flag */
#define NVM_ADMIN_NV_WRPRO                     3U  /**< Write protection falg */
#define NVM_ADMIN_NV_REPAIR                    4U  /**< Repair data flag */
#define NVM_ADMIN_RAM_VALID_CHANGE_STATUS_USED 5U  /**< Update ram valid/changed status flag */
#define NVM_ADMIN_BLOCK_LOCKED                 6U  /**< Block lock flag */
#define NVM_ADMIN_REDUNDANCY_LOSS              7U  /**< Redundant block data loss flag */
#define NVM_ADMIN_RAMMIRROR_OR_NOT             8U  /**< Support mirror flag */
#define NVM_ADMIN_CRC_VALID                    9U  /**< Crc valid flag */
#define NVM_ADMIN_WRITE_COPY_RESULT            10U /**< Copy step flag when writting */
#define NVM_ADMIN_READ_REQUEST                 11U /**< The block read flag during readall */

#if (STD_OFF == NVM_DYNAMIC_CONFIGURATION)
#define NVM_READALL_FIRST_BLOCKID \
    0x02U /**< Defines used as parameters for the blockId of handling ReadAll without dynamic configuration */
#else
#define NVM_READALL_FIRST_BLOCKID \
    0x01U /** Defines used as parameters for the blockId of handling ReadAll with dynamic configuration */
#endif

#define NVM_VALIDATEALL_FIRST_BLOCKID 0x01U /**< Defines used as parameters for the blockId of handling ValidateAll */
#define NVM_WRITEALL_FIRST_BLOCKID    0x02U /**< Defines used as parameters for the blockId of handling WriteAll */

#define NVM_QUEUE_EMPTY_COUNT 0U /**< Defines used as parameters for the empty counter of queue */

/* ================================================ type definitions ================================================ */
/**
 * @brief Block Descriptor Flag type enum type define
 */
typedef enum
{
    NVM_BLOCK_DESC_WRITEBLOCKONCE,             /**< Defines write protection after first write @range 0x00*/
    NVM_BLOCK_DESC_WRITEPROT,                  /**< Defines an initial write protection of the NV block @range 0x01*/
    NVM_BLOCK_DESC_CALCRAMBLOCKCRC,            /**< Defines CRC calculation for the RAM block @range 0x02*/
    NVM_BLOCK_DESC_RESISTANTTOCHANGEDSW,       /**< NVRAM block is resistant to changed software @range 0x03*/
    NVM_BLOCK_DESC_SELECTBLOCKFORREADALL,      /**< NVRAM block shall be processed by NvM_ReadAll @range 0x04*/
    NVM_BLOCK_DESC_SELECTBLOCKFORWRITEALL,     /**< NVRAM block shall be processed by NvM_WriteAll @range 0x05*/
    NVM_BLOCK_DESC_STATICBLOCKIDCHECK,         /**< Defines the Static Block ID check @range 0x06*/
    NVM_BLOCK_DESC_WRITEVERIFICATION,          /**< Defines Write Verification @range 0x07*/
    NVM_BLOCK_DESC_AUTOVALIDATION,             /**< Defines auto validated during shutdown phase @range 0x08*/
    NVM_BLOCK_DESC_CRCCOMPMECHANISM,           /**< Defines the CRC of the RAM Block shall be compared @range 0x09*/
    NVM_BLOCK_DESC_SETRAMSTATUS,               /**< Defines set ram block status for the block @range 0x0A*/
    NVM_BLOCK_DESC_SYNCMECHANISM,              /**< Defines an explicit synchronization mechanism @range 0x0B*/
    NVM_BLOCK_DESC_BSWBLOCKSTATUSINFO,         /**< Defines BswM is informed about the current status @range 0x0C*/
    NVM_BLOCK_DESC_SELECTBLOCKFORFIRSTINITALL, /**< NVRAM block shall be processed by NvM_FirstInitAll @range 0x0D*/
    NVM_BLOCK_DESC_BLOCKUSECOMPRESSION         /**< NVRAM block supports compression @range 0x0E*/
} NvM_BlockDescrptorFlagType;

/**
 * @brief Type of memif module service requested
 */
typedef enum
{
    NVM_MEMIF_REQ_READ,   /**< The NvM sends a read request to MemIf @range 0x00*/
    NVM_MEMIF_REQ_WRITE,  /**< The NvM sends a write request to MemIf @range 0x01*/
    NVM_MEMIF_REQ_ERASE,  /**< The NvM sends a erase request to MemIf @range 0x02*/
    NVM_MEMIF_REQ_INVALID /**< The NvM sends a invalid request to MemIf @range 0x03*/
} NvM_MemIfReqType;

/**
 * @brief Types of tasks that are automated within the NvM module
 */
typedef enum
{
    NVM_ATOMJOB_READBLOCK,         /**< Automated job for read block @range 0x00*/
    NVM_ATOMJOB_WRITEBLOCK,        /**< Automated job for write block @range 0x01*/
    NVM_ATOMJOB_READROM,           /**< Automated job for read rom block @range 0x02*/
    NVM_ATOMJOB_CALCCRC,           /**< Automated job for caculate crc @range 0x03*/
    NVM_ATOMJOB_WRITEVERIFICATION, /**< Automated job for write verification @range 0x04*/
    NVM_ATOMJOB_INVALIDATE,        /**< Automated job for invalid block @range 0x05*/
    NVM_ATOMJOB_MAX_NONE           /**< No automated job @range 0x06*/
} NvM_AtomJobEType;

/**
 * @brief Types of jobs processed within the NvM module
 */
typedef enum
{
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
    NVM_JOB_TYPE_SINGLE_NORMAL, /**< Single job without immediate data @range 0x00*/
#if (STD_ON == NVM_JOB_PRIORITIZATION)
    NVM_JOB_TYPE_SINGLE_IMMED,           /**< Single job with immediate data @range 0x01*/
    NVM_JOB_TYPE_MULTI_GAP_SINGLE_IMMED, /**< Multi job is interrupted by single job with immediate data @range 0x02*/
#endif
#endif
    NVM_JOB_TYPE_MULTI, /**< Multi job @range 0x00*/
    NVM_JOB_TYPE_NONE   /**< No job @range 0x01*/
} NvM_JobTypeType;

/**
 * @brief Types of NvM module job steps
 */
typedef enum
{
    NVM_JOB_STEP_READ_1ST_NV,          /**< Read 1st Nv block @range 0x00*/
    NVM_JOB_STEP_READ_2ND_NV,          /**< Read 2nd Nv block @range 0x01*/
    NVM_JOB_STEP_WRITE_1ST_NV,         /**< Wrtie 1st Nv block @range 0x02*/
    NVM_JOB_STEP_WRITE_2ND_NV,         /**< Write 2nd Nv block @range 0x03*/
    NVM_JOB_STEP_ERASE_1ST_NV,         /**< Erase 1st Nv block @range 0x04*/
    NVM_JOB_STEP_ERASE_2ND_NV,         /**< Erase 2nd Nv block @range 0x05*/
    NVM_JOB_STEP_INVALID_1ST_NV,       /**< Invalid 1st Nv block @range 0x06*/
    NVM_JOB_STEP_INVALID_2ND_NV,       /**< Invalid 2nd Nv block @range 0x07*/
    NVM_JOB_STEP_READ_ROM,             /**< Read Rom block @range 0x08*/
    NVM_JOB_STEP_CALC_CRC_READ_1ST_NV, /**< Caculate Crc for 1st Nv block @range 0x09*/
    NVM_JOB_STEP_CALC_CRC_READ_2ND_NV, /**< Caculate Crc for 2nd Nv block @range 0x0A*/
    NVM_JOB_STEP_CALC_CRC,             /**< Caculate Crc @range 0x0B*/
    NVM_JOB_STEP_IDLE,                 /**< Task has been finished @range 0x0C*/
    NVM_JOB_STEP_PENDING,              /**< Task is pending @range 0x0D*/
    NVM_JOB_STEP_CANCEL,               /**< cancel block @range 0x0E*/
    NVM_JOB_STEP_COPY,                 /**< Task is about to copy @range 0x0F*/
    NVM_JOB_STEP_WRITE_VERIFICATION,   /**< Write verification @range 0x10*/
    NVM_JOB_STEP_CHECK                 /**< Task is about to check static id @range 0x11*/
} NvM_JobStepType;

/**
 * @brief The NvM module requests the type of mode the memif module is in after the lower level
 */
typedef enum
{
    NVM_MEMIF_JOB_IDLE,         /**< Layer memif job is finished @range 0x00*/
    NVM_MEMIF_JOB_ASYNC_READY,  /**< Layer memif job is ready @range 0x01*/
    NVM_MEMIF_JOB_ASYNC_PENDING /**< Layer memif job is pending @range 0x02*/
} NvM_MemIfStateType;

/**
 * @brief Mode in which the NvM module is running
 */
typedef enum
{
    NVM_RUNTIME_NONE,     /**< Initial mode for the running @range 0x00*/
    NVM_RUNTIME_EXTENDED, /**< Dynamic configuration management handling enabled @range 0x01*/
    NVM_RUNTIME_NORMAL    /**< Dynamic configuration management handling disabled @range 0x02*/
} NvM_RuntimeType;

/**
 * @brief NvM module CRC result processing method type
 */
typedef enum
{
    NVM_CRC_REMAIN, /**< Remain the caculated crc @range 0x00*/
    NVM_CRC_DELETE, /**< Delete the caculated crc @range 0x01*/
    NVM_CRC_UPDATE  /**< Update the caculated crc @range 0x02*/
} NvM_CrcChangeType;

/**
 * @brief NvM module CRC and static ID check result types
 */
typedef enum
{
    NVM_CRC_STATICID_NO_USE, /**< Not use static Id @range 0x00*/
    NVM_CRC_STATICID_NOT_OK, /**< Static Id is inconsistent @range 0x01*/
    NVM_CRC_STATICID_OK      /**< Static Id is consistent @range 0x02*/
} NvM_CrcStaticIdStatusType;

/* PRQA S 1536 ++ */ /* VL_NvM_1536 */
/**
 * @brief NvM Module Schema Information Type
 */
typedef struct NvM_ModuleTypeTag
{
    boolean Init; /**< Initial status @range 0..255*/
#if ((NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS) && (STD_ON == NVM_JOB_PRIORITIZATION))
    uint8 CurActPriTable; /**< Current action priority table @range 0..255*/
#endif
    NvM_JobTypeType    CurrentJobType; /**< Type for the current job @range NA*/
    NvM_RuntimeType    RuntimeType;    /**< Type for the NvM module runtime @range NA*/
    NvM_MemIfStateType MemIfJobState;  /**< Job status for memif @range NA*/
    NvM_JobStepType    JobStep;        /**< Job step for NvM module @range NA*/
} NvM_ModuleType;

/**
 * @brief NvM Module Multi-Block Task Mode Information Type
 */
typedef struct NvM_MultiJobTypeTag
{
    boolean               Enqueue;        /**< Queue status flag @range 0..255*/
    boolean               CancelWriteAll; /**< Cancel write all flag @range 0..255*/
    boolean               ID1WriteMark;   /**< Write flag for Block 1 @range 0..255*/
    NvM_RequestResultType ReqResult;      /**< Request result for multi job @range NA*/
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
    NvM_ServiceIdType ServiceId; /**< Requeset services Id @range NA*/
#if (STD_ON == NVM_JOB_PRIORITIZATION)
    NvM_BlockIdType BlockId; /**< BlockId for multi job @range NA*/
#endif
#endif
} NvM_MultiJobType;

/**
 * @brief NvM Module Automated Task Record Information Type
 */
typedef struct NvM_AtomJobTypeTag
{
    uint8            Count;                       /**< Automated job counter @range 0..255*/
    NvM_AtomJobEType ReqId[NVM_ATOMJOB_MAX_NONE]; /**< Request Id for Automated job @range NA*/
} NvM_AtomJobType;

/**
 * @brief Type of block-related information currently processed by the NvM module
 */
typedef struct NvM_CurRunningTypeTag
{
    uint8             DeviceId;            /**< Device ID for layer module @range 0..255*/
    NvM_ServiceIdType ServiceId;           /**< Service ID value @range NA*/
    uint8             Index;               /**< Offset value @range 0..255*/
    uint8             FailedNum;           /**< Failed number @range 0..255*/
    uint8             NvNum;               /**< NVRAM block number @range 0..255*/
    uint8             RomNum;              /**< Rom block number @range 0..255*/
    uint8             ReadRetryCounter;    /**< Max num of read retries @range 0..255*/
    uint8             WriteTimesCounter;   /**< Max num of write retries @range 0..255*/
    uint8             RepeatMirrorCounter; /**< the number of retries to copy data from mirror  @range 0..255*/
#if (STD_ON == NVM_CIPHERING_ENABLE)
    uint8 NvMCsmRetryCounter; /**< Max num of Csm retries @range 0..255*/
#endif
    uint8                   CrcFlag;             /**< Crc flag of the block @range 0..255*/
    NvM_RequestResultType   ESingleReqResult;    /**< Single job request result @range NA*/
    NvM_RequestResultType   EMultiReqResult;     /**< Multi job request result @range NA*/
    NvM_BlockManagementType ManagementType;      /**< Block management type block number @range NA*/
    NvM_BlockCRCType        CRCType;             /**< Type of Used Crc @range NA*/
    NvM_BlockIdType         BlockId;             /**< Block ID value @range NA*/
    NvM_BlockIdType         StaticId;            /**< Static ID value @range NA*/
    NvM_BlockIdType         BlockIdIndex;        /**< Index of Block ID @range NA*/
    uint16                  AdminFlagGroup;      /**< Administrative block flag @range 0..65535*/
    uint16                  BaseNumber;          /**< Base number of the block @range 0..65535*/
    uint16                  Length;              /**< Length of the block @range 0..65535*/
    uint16                  VerificationSize;    /**< Verification size of the block @range 0..65535*/
    uint16                  VerificationCounter; /**< The number of verified @range 0..65535*/
    uint16                  CrcLength;           /**< Length of Crc @range 0..65535*/
    uint8*                  RamAddr;             /**< Ram block address @range NA*/
    uint8*                  RomAddr;             /**< Rom block address @range NA*/
    uint8*                  CrcAddr;             /**< Address for storing the Crc value @range NA*/
    uint32                  Crc;                 /**< Caculated Crc value @range 0..0xFFFFFFFF*/
    uint32                  TempCrc;             /**< Temporary to store Crc value @range 0..0xFFFFFFFF*/

    NvM_ReadRamBlockFromNvmCallbackType
        NvM_ReadRamBlockFromNvm; /**< Read callback function pointer address to copy ram to ram mirror @range NA*/
    NvM_WriteRamBlockToNvmCallbackType
        NvM_WriteRamBlockToNvm; /**< Write callback function pointer address to copy ram to ram mirror @range NA*/
    NvM_InitBlockCallbackType   InitCallback;   /**< Initial callback function pointer @range NA*/
    NvM_SingleBlockCallbackType SingleCallback; /**< Single block callback function pointer @range NA*/
} NvM_CurRunningType;

/**
 * @brief NvM Module Management Block Information Type
 */
typedef struct NvM_AdminBlockTypeTag
{
    uint8                 CurrentIndex;    /**< Current index for dataset type @range 0..255*/
    NvM_ServiceIdType     ServiceID;       /**< Request service ID @range NA*/
    NvM_RequestResultType SingleReqResult; /**< Request single job result @range NA*/
    uint16                FlagGroup;       /**< Administrative block flag @range 0..65535*/
#if (NVM_ECUC_PARTITION_NUM > 1U)
    uint16 SatelliteFeedBack; /**< Flag for satellite feedback @range 0..65535*/
#endif
    uint32 Crc; /**< Crc value of administrative block @range 0..0xFFFFFFFF*/
} NvM_AdminBlockType;

#if (STD_ON == NVM_JOB_PRIORITIZATION)
/**
 * @brief NvM Module Priority List Record Information Type
 */
typedef struct NvM_PriTable2QueueTypeTag
{
    uint8 HeadIndex; /**< Head index of the queue @range 0..255*/
    uint8 TailIndex; /**< Tail index of the queue @range 0..255*/
} NvM_PriTable2QueueType;

/**
 * @brief NvM Module Immediate Queue Type
 */
typedef struct NvM_ImmedQueueTypeTag
{
    NvM_BlockIdType BlockId;    /**< Block ID of the queue with immediate data @range 0..65535*/
    uint8*          DestSrcPtr; /**< Pointer of the Destination or source address @range NA*/
} NvM_ImmedQueueType;
#endif

#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
/**
 * @brief NvM Module Queue Management Message Type
 */
typedef struct NvM_RoundRobinQueueManageTypeTag
{
    uint8 Count;     /**< Number of the job in the queue @range 0..255*/
    uint8 HeadIndex; /**< Head index of the queue @range 0..255*/
    uint8 TailIndex; /**< Tail index of the queue @range 0..255*/
} NvM_RoundRobinQueueManageType;

/**
 * @brief NvM Module Standard Queue Types
 */
typedef struct NvM_StandQueueTypeTag
{
#if (STD_ON == NVM_JOB_PRIORITIZATION)
    uint8 NextIndex; /**< Point to the head index of the linked list @range 0..255*/
#endif
    NvM_ServiceIdType ServiceId;  /**< Service ID value @range NA*/
    NvM_BlockIdType   BlockId;    /**< Block ID value @range NA*/
    uint8*            DestSrcPtr; /**< Pointer of the Destination or source address @range NA*/
} NvM_StandQueueType;
#endif
/* PRQA S 1536 -- */
/**
 * @brief       Function Pointer type define, with void parameter and void return value
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76749
 */
typedef void (*NvM_VoidFuncVoidPtr)(void); /* PRQA S 5004 */ /* VL_NvM_5004 */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */
#if ((NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS) && (STD_ON == NVM_JOB_PRIORITIZATION))
/**
 * @brief BUffer for queue with priority @range NA
 */
extern uint16 NvM_QueueSpaceTalbe[NVM_TABLE_SIZE_JOB_QUEUE];
#endif

#if (NVM_API_CONFIG_CLASS_3 != NVM_API_CONFIG_CLASS)
/**
 * @brief API function for class1 and calss3
 */
extern const NvM_VoidFuncVoidPtr NvM_MemIfAsyncFuncTable[NVM_MEMIF_JOB_MAX_NUM][NVM_MEMIF_CLASS1_AND_CLASS2_NUM];
#elif (STD_ON == NVM_JOB_PRIORITIZATION)
/**
 * @brief API function for class1 and calss3 @range NA
 */
extern const NvM_VoidFuncVoidPtr NvM_MemIfAsyncFuncTable[NVM_MEMIF_JOB_MAX_NUM][NVM_MEMIF_JOB_MAX_NUM];
#else
/**
 * @brief API function for class1 and calss3 @range NA
 */
extern const NvM_VoidFuncVoidPtr NvM_MemIfAsyncFuncTable[NVM_MEMIF_JOB_MAX_NUM][NVM_MEMIF_JOB_MAX_NUM];
#endif

#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
#if (STD_ON == NVM_JOB_PRIORITIZATION)
/**
 * @brief Record the priority list information @range NA
 */
extern NvM_PriTable2QueueType NvM_PriTable2Queue[NVM_PRI_TAB_MAX_NUM][NVM_TOTAL_NUM_DIFF_PRI];

/**
 * @brief Immediate Queue @range NA
 */
extern NvM_ImmedQueueType NvM_ImmedQueue[NVM_SIZE_IMMEDIATE_JOB_QUEUE];
#endif

/**
 * @brief Standard Queue @range NA
 */
extern NvM_StandQueueType NvM_StandQueue[NVM_SIZE_STANDARD_JOB_QUEUE];
#endif

/**
 * @brief Record the management block information @range NA
 */
extern NvM_AdminBlockType NvM_AdminBlock[NVM_BLOCK_NUM_ALL];

/**
 * @brief Record the schema information in NvM module @range NA
 */
extern NvM_ModuleType NvM_Module;

/**
 * @brief Record the multi-block job information in NvM module @range NA
 */
extern NvM_MultiJobType NvM_MultiJob;

/**
 * @brief Record the automated job information in NvM Module @range NA
 */
extern NvM_AtomJobType NvM_AtomJob;

/**
 * @brief Record the block-related information currently processed @range NA
 */
extern NvM_CurRunningType NvM_CurRunning;

#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
#if (STD_ON == NVM_JOB_PRIORITIZATION)
/**
 * @brief Management message information of the queue with immediately data @range NA
 */
extern NvM_RoundRobinQueueManageType NvM_ImmedQueueManage;
#else
/**
 * @brief Management message information of the queue without immediately data @range NA
 */
extern NvM_RoundRobinQueueManageType NvM_StandQueueManage;
#endif
#endif

#if ((NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS) && (STD_ON == NVM_JOB_PRIORITIZATION))
/**
 * @brief Buffer of Priority Table @range NA
 */
/* PRQA S 1512 ++ */ /* VL_NvM_1512 */
extern uint16 NvM_PriorityTable[NVM_PRI_TAB_MAX_NUM][NVM_TABLE_SIZE_PRIORITY];
/* PRQA S 1512 -- */
#endif

/**
 * @brief BUffer of NVRAM block @range NA
 */
extern uint8 NvM_NvDataBuffer[NVM_MAX_LENGTH_NV_BLOCK + NVM_NV_CRC_MAX_LENGTH];

/**
 * @brief Buffer for temporary to store the repair data @range NA
 */
extern uint8 NVM_TemporaryRAMForRepaire[NVM_REDUNDANT_ALL][NVM_MAX_LENGTH_REDUNDANT_BLOCK];

#if (STD_ON == NVM_CIPHERING_ENABLE)
/**
 * @brief Data buffer for ciphering
 */
extern uint8 NvM_CipheringBuffer[NVM_BLOCKMAXLENGTH];
#endif

/* ========================================== internal function definitions ========================================= */
/**
 * @brief       Get the specfied bit value of the byte depending on the GetBit value passed in
 * @param[in]   WordNum: The word number
 * @param[in]   GetBit: The get bit
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76620
 */
NVM_LOCAL_INLINE uint8 NvM_GetWordBitState(uint16 WordNum, uint16 GetBit)
{
    uint8 localNumber = 0U;

    if (0U != (WordNum & (uint16)((uint16)0x01U << GetBit)))
    {
        localNumber = 1u;
    }
    return localNumber;
}

/**
 * @brief       Set one bit of the byte to 0 or 1 depending on the OnOff value passed in
 * @param[in]   WordPtr: Pointer to the word
 * @param[in]   SetBit: The set bit
 * @param[in]   OnOff: The set value
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76621
 */
NVM_LOCAL_INLINE void NvM_SetWordBitState(uint16* WordPtr, uint16 SetBit, uint8 OnOff)
{
    if (STD_ON == OnOff)
    {
        (*WordPtr) |= (uint16)((uint16)0x01U << SetBit);
    }
    else
    {
        (*WordPtr) &= (uint16)(~(uint16)((uint16)0x01U << SetBit));
    }
}

/**
 * @brief       Set the flag to on
 * @param[in]   flag: The set flag
 * @param[in]   type: The reqeust type
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76622
 */
NVM_LOCAL_INLINE void NVM_SETFLAGON(uint16* flag, uint16 type)
{
    NvM_SetWordBitState(flag, type, STD_ON);
}

/**
 * @brief       Set the flag to off
 * @param[in]   flag: The set flag
 * @param[in]   type: The reqeust type
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76623
 */
NVM_LOCAL_INLINE void NVM_SETFLAGOFF(uint16* flag, uint16 type)
{
    NvM_SetWordBitState(flag, type, STD_OFF);
}

/**
 * @brief       Check whether the flag is on
 * @param[in]   flag: The check flag
 * @param[in]   type: The reqeust type
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76624
 */
NVM_LOCAL_INLINE boolean NVM_ISFLAGON(uint16 flag, uint16 type)
{
    return ((boolean)STD_ON == NvM_GetWordBitState(flag, type)) ? TRUE : FALSE; /* PRQA S 1881 */ /* VL_NvM_1881 */
}

/**
 * @brief       Check whether the flag is off
 * @param[in]   flag: The check flag
 * @param[in]   type: The reqeust type
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76625
 */
NVM_LOCAL_INLINE boolean NVM_ISFLAGOFF(uint16 flag, uint16 type)
{
    return ((boolean)STD_OFF == NvM_GetWordBitState(flag, type)) ? TRUE : FALSE; /* PRQA S 1881 */ /* VL_NvM_1881 */
}

#if (STD_ON == NVM_DEV_ERROR_DETECT)
/**
 * @brief       Report Running Time Error
 * @param[in]   ApiId: Specifies which API reported the error.
 * @param[in]   ErrorId: Specify which error was reported.
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76626
 */
NVM_LOCAL_INLINE void NvM_Det_ReportRunTimeError(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportRuntimeError(NVM_MODULE_ID, NVM_INSTANCE_ID, ApiId, ErrorId);
}

/**
 * @brief       Report Develop Error
 * @param[in]   ApiId: Specifies which API reported the error.
 * @param[in]   ErrorId: Specify which error was reported.
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76627
 */
NVM_LOCAL_INLINE void NvM_Det_ReportError(uint8 ApiId, uint8 ErrorId)
{
    ((void)Det_ReportError(NVM_MODULE_ID, NVM_INSTANCE_ID, ApiId, ErrorId));
}
#endif

#if (NVM_ECUC_PARTITION_NUM > 1U)
/**
 * @brief       Check master partition id
 * @return      boolean
 * @retval      TRUE: Master partition id is match
 * @retval      FALSE: Master partition id is not match
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-76628
 */
NVM_LOCAL_INLINE boolean NvM_CheckMasterEcucPartition()
{
    return (boolean)(NVM_MASTER_ECUC_PARTITION == GetApplicationID());
}

/**
 * @brief       Get partition id of the block
 * @param[in]   BlockId: The block identifier uniquely identifies one NVRAM block descriptor
 * @return      ApplicationType
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-76629
 */
NVM_LOCAL_INLINE ApplicationType NvM_GetBlockPartitionIdByBlock(NvM_BlockIdType BlockId)
{
    return NvM_BlockDescriptor[BlockId].NvMBlockEcucPartitionRef;
}

/**
 * @brief       Record the single result of request multi-job
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76630
 */
NVM_LOCAL_INLINE boolean NvM_CheckBlockEcucPartition(NvM_BlockIdType BlockId)
{
    boolean checkFlag = TRUE;
    if (BlockId != 0u)
    {
        checkFlag = (GetApplicationID() == NvM_GetBlockPartitionIdByBlock(BlockId - 1u)) ? TRUE : FALSE;
    }
    return checkFlag;
}
#endif

/**
 * @brief       Record the single result of request multi-job
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76631
 */
NVM_LOCAL_INLINE void NvM_EMultiReqResultCheck(void)
{
    if ((NVM_JOB_TYPE_MULTI == NvM_Module.CurrentJobType) && (NVM_JOB_STEP_IDLE == NvM_Module.JobStep)
        && (NVM_REQ_OK != NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].SingleReqResult)
        && (NVM_REQ_BLOCK_SKIPPED != NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].SingleReqResult))
    {
#if ((NVM_API_CONFIG_CLASS_1 == NVM_API_CONFIG_CLASS) || (STD_OFF == NVM_JOB_PRIORITIZATION))
        if ((NvM_CurRunning.BlockId == 1U) && (NVM_READ_ALL_SERV_ID == NvM_CurRunning.ServiceId))
#else
        if ((NvM_CurRunning.BlockId == 1U) && (NVM_READ_ALL_SERV_ID == NvM_MultiJob.ServiceId))
#endif
        {
            NvM_CurRunning.EMultiReqResult = NVM_REQ_OK;
        }
        else
        {
            NvM_CurRunning.EMultiReqResult = NVM_REQ_NOT_OK;
        }
    }
}

/**
 * @brief       Set request internal job
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76632
 */
NVM_LOCAL_INLINE void NvM_AtomJobReq(NvM_AtomJobEType AtomJobReqId)
{
    ++NvM_AtomJob.Count;
    NvM_AtomJob.ReqId[NvM_AtomJob.Count] = AtomJobReqId;
}

/**
 * @brief       Update valid/changed status
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76633
 */
NVM_LOCAL_INLINE void NvM_UpdateValidandChangeStatus(uint8 ValidStatus, uint8 ChangeStatus)
{
    if (TRUE == NVM_ISFLAGON(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_VALID_CHANGE_STATUS_USED))
    {
        NvM_SetWordBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_VALID, ValidStatus);
        NvM_SetWordBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_CHANGED, ChangeStatus);
    }
}

/**
 * @brief       Manage the write sequence
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76634
 */
NVM_LOCAL_INLINE void NvM_WriteSequenceForManagementType(void)
{
    if (NVM_BLOCK_REDUNDANT == NvM_CurRunning.ManagementType)
    {
        NvM_Module.JobStep       = NVM_JOB_STEP_WRITE_2ND_NV;
        NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
        NvM_CurRunning.Index     = 1u;
    }
    else
    {
        NvM_Module.JobStep       = NVM_JOB_STEP_WRITE_1ST_NV;
        NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
    }
}

/**
 * @brief       Write static block ID
 * @param[in]   crctype: Request CRC type
 * @param[out]  Dest: Pointer to the src address
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76635
 */
NVM_LOCAL_INLINE void NvM_WriteStaticBlockID(uint8* Dest, NvM_BlockCRCType crctype)
{
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    uint16 offSet =
        (NVM_CRC_NOT_USED == crctype) ? 0u : ((NVM_CRC8 == crctype) ? 1u : ((NVM_CRC16 == crctype) ? 2u : 4u));
    Dest[NvM_CurRunning.Length + offSet] = (uint8)(NvM_CurRunning.BlockId >> 8U);
    /* PRQA S 3120 -- */
    Dest[NvM_CurRunning.Length + offSet + 1u] = (uint8)NvM_CurRunning.BlockId;
}

/**
 * @brief       Read static block ID
 * @param[in]   Src: Pointer to the src address
 * @param[in]   crctype: Request CRC type
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76636
 */
NVM_LOCAL_INLINE void NvM_ReadStaticBlockID(const uint8* Src, NvM_BlockCRCType crctype)
{
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    uint16 offSet =
        (NVM_CRC_NOT_USED == crctype) ? 0u : ((NVM_CRC8 == crctype) ? 1u : ((NVM_CRC16 == crctype) ? 2u : 4u));
    NvM_CurRunning.StaticId = (uint16)((uint16)Src[NvM_CurRunning.Length + offSet] << 8U)
                              + (uint16)(Src[NvM_CurRunning.Length + offSet + 1u]);
    /* PRQA S 3120 -- */
}

/**
 * @brief       Get repeat mirror operation
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76637
 */
NVM_LOCAL_INLINE void NVM_GetRepeatMirrorOperation(void)
{
    if (TRUE
        == NVM_ISFLAGON(
            NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].FlagGroup,
            (uint8)NVM_BLOCK_DESC_SYNCMECHANISM))
    {
        NvM_CurRunning.RepeatMirrorCounter = NVM_REPEAT_MIRROR_OPERATIONS + 1u;
    }
    else
    {
        NvM_CurRunning.RepeatMirrorCounter = 1u;
    }
}

/* ========================================= external function declarations ========================================= */
/**
 * @brief       Place the requested task in the queue
 * @param[in]   BlockId: The block identifier uniquely identifies one NVRAM block descriptor
 * @param[in]   ServiceId: The service identifier uniquely identifies one NvM service
 * @param[in]   DestSrcPtr: A pointer to the data being stored
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing Failure
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-76638
 */
extern Std_ReturnType NvM_JobEnqueue(NvM_BlockIdType BlockId, NvM_ServiceIdType ServiceId, uint8* DestSrcPtr);

#if (STD_ON == NVM_SET_RAM_BLOCK_STATUS_API)
/**
 * @brief       The module internal function to process set ram status request
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76639
 */
extern void NvM_InterSetRamStatus(void);
#endif

#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
/**
 * @brief       The module internal function to process restore block default request
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76640
 */
extern void NvM_InterRestoreBlockDefaults(void);
#endif

/**
 * @brief       Remove a task from the queue for processing
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing Failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76641
 */
extern Std_ReturnType NvM_JobDequeue(void);

/**
 * @brief       Set the corresponding flag bit when the job ends
 * @param[in]   SingleReqResult: request job result
 * @param[in]   CrcChange: Type of crc operation to be performed
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76642
 */
extern void NvM_JobOverSetFlag(NvM_RequestResultType SingleReqResult, NvM_CrcChangeType CrcChange);

/**
 * @brief       Send a request to the underlying memif
 * @param[in]   NvM_MemIfRequest: Type of request to the underlying memif
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76643
 */
extern void NvM_MemIfReq(NvM_MemIfReqType NvM_MemIfRequest);

#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
#if (STD_ON == NVM_JOB_PRIORITIZATION)
/**
 * @brief       The module internal function to process erase NV block
 *              request Since the GC mechanism, Block pre-Erase before write
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76644
 */
extern void NvM_InterEraseNvBlock(void);
#endif

/**
 * @brief       The module internal function to process invalidate NV block request
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76645
 */
extern void NvM_InterInvalidateNvBlock(void);
#endif

#if (STD_ON == NVM_WRITE_VERIFY)
/**
 * @brief       Inter Write Verification ok process
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76646
 */
extern void NvM_InterWriteVerificationOK(void);

/**
 * @brief       Inter Write Verification
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76648
 */
extern void NvM_InterWriteVerification(void);
#endif

/**
 * @brief       The module internal function to process writing block request
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76649
 */
extern void NvM_InterWriteAll(void);

#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
/**
 * @brief       The module internal function to process first init all block request
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76650
 */
extern void NvM_InterFirstInitAll(void);
#endif

/**
 * @brief       The module internal function to process read all block request
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76651
 */
extern void NvM_InterReadAll(void);

/**
 * @brief       The module internal function to process writing block request
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76652
 */
extern void NvM_InterWriteBlock(void);

/**
 * @brief       The module internal function to process read block request sub deal
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76653
 */
extern void NvM_InterReadBlock(void);

/**
 * @brief       The module internal function to calculate crc
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76654
 */
extern void NvM_InterCalcCrc(void);

#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
/**
 * @brief       The module internal function to process validating all block request
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76655
 */
extern void NvM_InterValidateAll(void);
#endif

/**
 * @brief       The module internal function to process read rom request
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76656
 */
extern void NvM_InterReadRom(void);

/**
 * @brief       The feedabck interface of multi job result
 * @param[in]   ServiceId: The service identifier uniquely identifies one NvM service
 * @param[in]   Result: Type of request result type
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76657
 */
extern void NvM_MultiJobResultFeedBack(NvM_ServiceIdType ServiceId, NvM_RequestResultType Result);

/**
 * @brief       The feedabck interface of single job result
 * @param[in]   BlockId: The block identifier uniquely identifies one NVRAM block descriptor
 * @param[in]   Result: Type of request result type
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76658
 */
extern void NvM_SingleJobResultFeedBack(NvM_BlockIdType BlockId, NvM_RequestResultType Result);

#if (NVM_ECUC_PARTITION_NUM > 1U)
/**
 * @brief       Check feedback flag of all blocks, and call BswM interface to signal
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76659
 */
extern void NvM_SatelliteMainFunction(void);
#endif

#ifdef __cplusplus
}
#endif
#endif
