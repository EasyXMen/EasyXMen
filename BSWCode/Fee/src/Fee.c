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
 **  @file               : Fee.c
 **  @author             : peng.wu
 **  @date               : 2024/09/15
 **  @vendor             : isoft
 **  @description        : Implementation for FEE
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Fee.h"
#include "Fee_Types.h"
#include "Fee_Cbk.h"
#include "SchM_Fee.h"

/* ===================================================== macros ===================================================== */
/* PRQA S 3412 ++ */ /* VL_Fee_3412 */
#if (CPU_32_WITH_16_ADR == TRUE)
/**
 * @brief Calculate the aligned values
 * Macro parameters:
 *  @param[in] ParamVal Align based on this value
 *  @param[in] ParamType Used to calculate the size of a type
 *  @param[inout] pSerialPtr Used to record the aligned address
 */
#define FEE_SERIALIZE(ParamVal, ParamType, pSerialPtr)       \
    do                                                       \
    {                                                        \
        for (uint8 i = 0; i < (sizeof(ParamType) * 2u); ++i) \
        {                                                    \
            pSerialPtr[i] = (uint8)(ParamVal >> (i * 8u));   \
        }                                                    \
        pSerialPtr = &pSerialPtr[sizeof(ParamType) * 2u];    \
    } while (0);

/**
 * @brief Calculate the value before alignment
 * Macro parameters:
 *  @param[inout] pDeserialPtr Used to record addresses before alignment
 *  @param[in] ParamVal Calculate based on this value
 *  @param[in] ParamType Used to calculate the size of a type
 */
#define FEE_DESERIALIZE(pDeserialPtr, ParamVal, ParamType)                             \
    do                                                                                 \
    {                                                                                  \
        ParamVal = 0;                                                                  \
        for (uint8 i = 0; i < (sizeof(ParamType) * 2u); ++)                            \
        {                                                                              \
            ParamVal = ParamVal | (ParamType)((ParamType)pDeserialPtr[i] << (i * 8u)); \
        }                                                                              \
        pDeserialPtr = &pDeserialPtr[sizeof(ParamType) * 2u];                          \
    } while (0);
#else
/**
 * @brief Calculate the aligned values
 * Macro parameters:
 *  @param[in] ParamVal Align based on this value
 *  @param[in] ParamType Used to calculate the size of a type
 *  @param[inout] pSerialPtr Used to record the aligned address
 */
#define FEE_SERIALIZE(ParamVal, ParamType, pSerialPtr)           \
    do                                                           \
    {                                                            \
        for (uint8 i = 0; i < sizeof(ParamType); ++i)            \
        {                                                        \
            ((pSerialPtr)[i]) = (uint8)((ParamVal) >> (i * 8u)); \
        }                                                        \
        (pSerialPtr) = &((pSerialPtr)[sizeof(ParamType)]);       \
    } while (0);

/**
 * @brief Calculate the value before alignment
 * Macro parameters:
 *  @param[inout] pDeserialPtr Used to record addresses before alignment
 *  @param[in] ParamVal Calculate based on this value
 *  @param[in] ParamType Used to calculate the size of a type
 */
/* PRQA S 2991, 2995 ++ */ /* VL_QAC_2991,VL_Fee_2995 */
#define FEE_DESERIALIZE(pDeserialPtr, ParamVal, ParamType)                                   \
    do                                                                                       \
    {                                                                                        \
        (ParamVal) = 0;                                                                      \
        for (uint8 i = 0; i < sizeof(ParamType); ++i)                                        \
        {                                                                                    \
            (ParamVal) = (ParamVal) | (ParamType)((ParamType)(pDeserialPtr)[i] << (i * 8u)); \
        }                                                                                    \
        (pDeserialPtr) = &((pDeserialPtr)[sizeof(ParamType)]);                               \
    } while (0);
#endif
/* PRQA S 2991, 2995 -- */

/* PRQA S 3412 -- */

/* ================================================ type definitions ================================================ */

/* ========================================= internal function declarations ========================================= */
/**
 * @brief According to the job state and module state of fee, the end callback notification interface is invoked
 * @reentrant   Non Reentrant
 * @synchronous Synchronous
 * @trace       -
 */
FEE_LOCAL void Fee_FlsJobEndHandle(void);

/**
 * @brief According to the job state and job result of the Fls module, the error callback notification interface is
 * invoked
 * @reentrant   Non Reentrant
 * @synchronous Synchronous
 * @trace       -
 */
FEE_LOCAL void Fee_FlsJobErrorHandle(void);

/**
 * @brief Use Binary Search to start looking up the ids of all configured blocks
 * @param[in]   BlockNumber   : The block number passed in by the upper module
 * @reentrant   Non Reentrant
 * @synchronous Synchronous
 * @trace       -
 */
FEE_LOCAL uint16 Fee_GetBlockId(const uint16 BlockNumber);

/**
 * @brief Read the cluster header infomation arrcording to the header size
 * @reentrant   Non Reentrant
 * @synchronous Asynchronous
 * @trace       -
 */
FEE_LOCAL MemIf_JobResultType Fee_JobIntScanClusterHdrRead(void);

/**
 * @brief Initial job handle,try to read cluster header
 * @reentrant   Non Reentrant
 * @synchronous Asynchronous
 * @trace       -
 */
FEE_LOCAL void Fee_JobInitScan(void);

/**
 * @brief All internal jobs handle
 * @reentrant   Non Reentrant
 * @synchronous Asynchronous
 * @trace       -
 */
FEE_LOCAL MemIf_JobResultType Fee_JobSchedule(void);

/**
 * @brief If the block is valid, the underlying interface is called to read the data
 * @reentrant   Non Reentrant
 * @synchronous Asynchronous
 * @trace       -
 */
FEE_LOCAL void Fee_JobRead(void);

/**
 * @brief Immediate data calls the immediate write interface, otherwise the write header information and queues the
 * write request
 * @reentrant   Non Reentrant
 * @synchronous Asynchronous
 * @trace       -
 */
FEE_LOCAL void Fee_JobWrite(void);

/**
 * @brief Invalidate Fee block by writing the invalid flag to flash
 * @reentrant   Non Reentrant
 * @synchronous Asynchronous
 * @trace       -
 */
FEE_LOCAL void Fee_JobInvalBlock(void);

/**
 * @brief Erase (pre-allocate) immediate Fee block
 * @reentrant   Non Reentrant
 * @synchronous Asynchronous
 * @trace       -
 */
FEE_LOCAL void Fee_JobEraseImmediate(void);

/**
 * @brief Invalidation of Fee block done
 * @reentrant   Non Reentrant
 * @synchronous Asynchronous
 * @trace       -
 */
FEE_LOCAL MemIf_JobResultType Fee_JobInvalBlockDone(void);

/**
 * @brief Erase immediate Fee block done
 * @reentrant   Non Reentrant
 * @synchronous Asynchronous
 * @trace       -
 */
FEE_LOCAL MemIf_JobResultType Fee_JobEraseImmediateDone(void);

/**
 * @brief Write Fee block header to flash
 * @reentrant   Non Reentrant
 * @synchronous Asynchronous
 * @trace       -
 */
FEE_LOCAL MemIf_JobResultType Fee_JobWriteHdr(void);

/**
 * @brief Initialize the cluster swap internal operation, This can only happen during the block header writing phase
 * @reentrant   Non Reentrant
 * @synchronous Asynchronous
 * @trace       -
 */
FEE_LOCAL MemIf_JobResultType Fee_JobIntSwap(void);

/**
 * @brief Erase the new cluster for swap
 * @reentrant   Non Reentrant
 * @synchronous Asynchronous
 * @trace       -
 */
FEE_LOCAL MemIf_JobResultType Fee_JobIntSwapClusterErase(void);

/**
 * @brief Clear current Fee cluster done,writing cluster header into flash
 * @reentrant   Non Reentrant
 * @synchronous Asynchronous
 * @trace       -
 */
FEE_LOCAL MemIf_JobResultType Fee_JobIntSwapClusterClear(void);

/**
 * @brief Copy block from source to target cluster
 * @reentrant   Non Reentrant
 * @synchronous Asynchronous
 * @trace       -
 */
FEE_LOCAL MemIf_JobResultType Fee_JobIntSwapBlock(void);

/**
 * @brief Success write cluster valid flag,update all copied block information
 * @reentrant   Non Reentrant
 * @synchronous Asynchronous
 * @trace       -
 */
FEE_LOCAL MemIf_JobResultType Fee_JobIntSwapClusterVldDone(void);

/**
 * @brief Validate current cluster by writing FEE_VALID_VALUE into flash
 * @reentrant   Non Reentrant
 * @synchronous Asynchronous
 * @trace       -
 */
FEE_LOCAL MemIf_JobResultType Fee_JobIntSwapClusterVld(void);

/**
 * @brief Read data from source cluster to internal Fee buffer
 * @reentrant   Non Reentrant
 * @synchronous Asynchronous
 * @trace       -
 */
FEE_LOCAL MemIf_JobResultType Fee_JobIntSwapDataRead(void);

/**
 * @brief Write block header valid flag
 * @reentrant   Non Reentrant
 * @synchronous Asynchronous
 * @trace       -
 */
FEE_LOCAL MemIf_JobResultType Fee_JobIntSwapBlockVldWrite(void);

/**
 * @brief Write data from internal Fee buffer to target cluster
 * @reentrant   Non Reentrant
 * @synchronous Asynchronous
 * @trace       -
 */
FEE_LOCAL MemIf_JobResultType Fee_JobIntSwapDataWrite(void);

/**
 * @brief Write Fee block data to flash
 * @reentrant   Non Reentrant
 * @synchronous Asynchronous
 * @trace       -
 */
FEE_LOCAL MemIf_JobResultType Fee_JobWriteData(void);

/**
 * @brief Validate Fee block by writing validation flag to flash
 * @reentrant   Non Reentrant
 * @synchronous Asynchronous
 * @trace       -
 */
FEE_LOCAL MemIf_JobResultType Fee_JobWriteValidate(void);

/**
 * @brief Write block done
 * @reentrant   Non Reentrant
 * @synchronous Asynchronous
 * @trace       -
 */
FEE_LOCAL MemIf_JobResultType Fee_JobWriteDone(void);

/**
 * @brief cluster header get(success/failed) handle
 * @param[in]   BufferValid   : Validity of the obtained cluster header information
 * @reentrant   Non Reentrant
 * @synchronous Asynchronous
 * @trace       -
 */
FEE_LOCAL MemIf_JobResultType Fee_JobIntScanClusterHdrGet(const boolean BufferValid);

/**
 * @brief Scan all cluster done
 * @reentrant   Non Reentrant
 * @synchronous Asynchronous
 * @trace       -
 */
FEE_LOCAL MemIf_JobResultType Fee_JobIntScanClusterDone(void);

/**
 * @brief Read the Fee block header into internal buffer
 * @reentrant   Non Reentrant
 * @synchronous Asynchronous
 * @trace       -
 */
FEE_LOCAL MemIf_JobResultType Fee_JobIntScanBlockHdrRead(void);

/**
 * @brief Erase first Fee cluster
 * @reentrant   Non Reentrant
 * @synchronous Asynchronous
 * @trace       -
 */
FEE_LOCAL MemIf_JobResultType Fee_JobIntScanClusterErase(void);

/**
 * @brief First cluster erase success,writing cluster header into flash
 * @reentrant   Non Reentrant
 * @synchronous Asynchronous
 * @trace       -
 */
FEE_LOCAL MemIf_JobResultType Fee_JobIntScanClusterClear(void);

/**
 * @brief Fee cluster header parameters(except valid parameter) to write buffer
 * @param[in]   ClusterUseID   : Currently used cluster ID
 * @param[in]   StartAddress   : Start address of the cluster header information
 * @param[in]   ClusterSize   : Length of the cluster header information
 * @param[inout]   BufferPtr   : Pointer to the buffer
 * @reentrant   Non Reentrant
 * @synchronous Synchronous
 * @trace       -
 */
FEE_LOCAL void Fee_PackClusterHdr(
    const uint32          ClusterUseID,
    const Fls_AddressType StartAddress,
    const Fls_LengthType  ClusterSize,
    uint8*                BufferPtr);

/**
 * @brief Unpack Fee cluster header parameters from read buffer
 * @param[in]   UseID   : Currently used cluster ID
 * @param[in]   StartAddress   : Start address of the cluster header information
 * @param[in]   ClusterSize   : Length of the cluster header information
 * @param[inout]   BufferPtr   : Pointer to the buffer
 * @reentrant   Non Reentrant
 * @synchronous Synchronous
 * @trace       -
 */
FEE_LOCAL Fee_ClusterStatusType Fee_UnpackClusterHdr(
    uint32* const          UseID,
    Fls_AddressType* const StartAddress,
    Fls_LengthType* const  ClusterSize,
    const uint8*           BufferPtr);

/**
 * @brief Check cluster is valid before unpacking Fee cluster header parameters from read bufferUseID
 * @param[in]   startPtr   : Start address of the cluster header information
 * @param[in]   endPtr   : End address of the cluster header information
 * @reentrant   Non Reentrant
 * @synchronous Synchronous
 * @trace       -
 */
FEE_LOCAL Fee_ClusterStatusType Fee_UnpackClusterHdrSub(const uint8* startPtr, const uint8* endPtr);

#if (STD_ON == FEE_PAGE_DIRECT_WRITE_SUPPORT)
/**
 * @brief Block header valid or invalid flag pack
 * @param[in]   TargetPtr   : Target address of the block header information
 * @param[in]   Flag   : The validity flag of the block header
 * @reentrant   Non Reentrant
 * @synchronous Synchronous
 * @trace       -
 */
FEE_LOCAL void Fee_PackBlockHdrValidOrInvalid(uint8* TargetPtr, const uint8 Flag);

/**
 * @brief Unpack valid or invalid flag from read buffer
 * @param[in]   TargetPtr   : Target address of the block header information
 * @param[in]   Flag   : The validity flag of the block header
 * @param[out]   FlagValue   : The validity flag value of the block header
 * @reentrant   Non Reentrant
 * @synchronous Synchronous
 * @trace       -
 */
FEE_LOCAL Std_ReturnType
    Fee_UnpackBlockHdrValidOrInvalid(const uint8* const TargetPtr, const uint8 Flag, boolean* FlagValue);
#endif

/**
 * @brief Pack validation or invalidation flag to write buffer
 * @param[in]   TargetPtr   : Target address of the block header information
 * @param[in]   Flag   : The validity flag of the header
 * @reentrant   Non Reentrant
 * @synchronous Synchronous
 * @trace       -
 */
FEE_LOCAL void Fee_PackHdrValidOrInvalid(uint8* TargetPtr, const uint8 Flag);

/**
 * @brief Unpack validation or invalidation flag from read buffer
 * @param[in]   TargetPtr   : Target address of the header information
 * @param[in]   Flag   : The validity flag of the header
 * @param[out]   FlagValue   : The validity flag value of the block header
 * @reentrant   Non Reentrant
 * @synchronous Synchronous
 * @trace       -
 */
FEE_LOCAL Std_ReturnType
    Fee_UnpackHdrValidOrInvalid(const uint8* const TargetPtr, const uint8 Flag, boolean* FlagValue);

/**
 * @brief Initial Job finished(all block is invalid,the cluster is new)
 * @reentrant   Non Reentrant
 * @synchronous Synchronous
 * @trace       -
 */
FEE_LOCAL MemIf_JobResultType Fee_JobIntScanClusterClearDone(void);

/**
 * @brief Check whether specified data buffer contains only
 * @param[in]   StartPtr   : Start address
 * @param[in]   EndPtr   : End address
 * @reentrant   Non Reentrant
 * @synchronous Synchronous
 * @trace       -
 */
FEE_LOCAL Std_ReturnType Fee_ClusterCheck(const uint8* StartPtr, const uint8* const EndPtr);

/**
 * @brief Initial job:scan the store block, have get the block header information
 * @param[in]   BufferValid   : The validity flag of the header header information
 * @reentrant   Non Reentrant
 * @synchronous Synchronous
 * @trace       -
 */
FEE_LOCAL MemIf_JobResultType Fee_JobIntScanBlockHdrGet(const boolean BufferValid);

/**
 * @brief The address of the stored header needs to be aligned based on BlockNumber, Length, TargetAddress
 * @param[in]   BlockNumber   : Currently used block ID
 * @param[in]   Length   : Length of the block header information
 * @param[in]   TargetAddress   : Start address of the block header information
 * @param[inout]   BlockHdrPtr   : Pointer to the block header buffer
 * @reentrant   Non Reentrant
 * @synchronous Synchronous
 * @trace       -
 */
FEE_LOCAL uint8 Fee_PackBlockHdrSubOne(
    const uint16          BlockNumber,
    const uint16          Length,
    const Fls_AddressType TargetAddress,
    uint8*                BlockHdrPtr);

/**
 * @brief Check if it is an immediate number, set the checksum, and then set the erase value based on the aligned
 * address
 * @param[in]   ImmediateBlock   : The immediately block flag
 * @param[in]   offset   : Offset of the block header information
 * @param[out]   checkSum   : Checksum of the block header information
 * @param[inout]   BlockHdrPtr   : Pointer to the block header buffer
 * @reentrant   Non Reentrant
 * @synchronous Synchronous
 * @trace       -
 */
FEE_LOCAL void
    Fee_PackBlockHdrSubTwo(uint32* checkSum, const boolean ImmediateBlock, uint8* BlockHdrPtr, const uint8 offset);

/**
 * @brief Unpack the block header information
 * @param[in]   BlockNumber   : Currently used block ID
 * @param[in]   Length   : Length of the block header information
 * @param[in]   TargetAddress   : Start address of the block header information
 * @param[in]   ImmediateBlock   : The immediately block flag
 * @reentrant   Non Reentrant
 * @synchronous Synchronous
 * @trace       -
 */
FEE_LOCAL Fee_BlockStatusType Fee_UnpackBlockHdr(
    uint16* const          BlockNumber,
    uint16* const          Length,
    Fls_AddressType* const TargetAddress,
    boolean* const         ImmediateBlock);

/**
 * @brief Align the address and set the checksum based on the immediate number before unpacking the block header
 * information
 * @param[in]   BlockNumber   : Currently used block ID
 * @param[in]   Length   : Length of the block header information
 * @param[in]   TargetAddress   : Start address of the block header information
 * @param[in]   ImmediateBlock   : The immediately block flag
 * @reentrant   Non Reentrant
 * @synchronous Synchronous
 * @trace       -
 */
FEE_LOCAL Fee_BlockStatusType Fee_UnpackBlockHdrSubOne(
    uint16* const          BlockNumber,
    uint16* const          Length,
    Fls_AddressType* const TargetAddress,
    boolean* const         ImmediateBlock);

/**
 * @brief Check the block is valid before unpacking the block header information
 * @param[out]   readCheckSum   : Currently read checksum of the block header information
 * @param[out]   calcCheckSum   : Currently caculate checksum of the block header information
 * @param[in]   blockHdrPtr   : Pointer to the block header buffer
 * @reentrant   Non Reentrant
 * @synchronous Synchronous
 * @trace       -
 */
FEE_LOCAL Fee_BlockStatusType
    Fee_UnpackBlockHdrSubTwo(const uint32 readCheckSum, const uint32 calcCheckSum, const uint8* blockHdrPtr);

/**
 * @brief Call the corresponding job processing interface based on the requested job, then set the status and invoke the
 * callback interface based on the job result
 * @reentrant   Non Reentrant
 * @synchronous Synchronous
 * @trace       -
 */
FEE_LOCAL void Fee_MainFunctionSub(void);

/* ============================================ internal data definition ============================================ */
/* PRQA S 3408 ++ */ /* VL_Fee_3408 */
#define FEE_START_SEC_VAR_INIT_PTR
#include "Fee_MemMap.h"
/**
 * @brief The PB configuration parameter used to point to the Fee module
 */
FEE_LOCAL const Fee_ConfigType* Fee_ConfigStd = NULL_PTR;
#define FEE_STOP_SEC_VAR_INIT_PTR
#include "Fee_MemMap.h"

#define FEE_START_SEC_VAR_INIT_8
#include "Fee_MemMap.h"
/**
 * @brief Record the status of the Fee module
 */
FEE_LOCAL MemIf_StatusType Fee_ModuleStatus = MEMIF_UNINIT;
#define FEE_STOP_SEC_VAR_INIT_8
#include "Fee_MemMap.h"

#define FEE_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Fee_MemMap.h"

/**
 * @brief Record the management data of the current task
 */
FEE_LOCAL Fee_JobManageType Fee_JobInfo;
#define FEE_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Fee_MemMap.h"

#define FEE_START_SEC_VAR_INIT_8
#include "Fee_MemMap.h"
FEE_LOCAL uint8 Fee_ClusterGroupIdx = 0u;
FEE_LOCAL uint8 Fee_ClusterIdx      = 0u;
#define FEE_STOP_SEC_VAR_INIT_8
#include "Fee_MemMap.h"

/**
 * @brief The address of this buffer may not be 4-byte aligned during memory allocation, so this array variable needs to
 * be allocated as 4-byte
 */
#define FEE_START_SEC_VAR_CLEARED_32
#include "Fee_MemMap.h"
/**
 * @brief Used for the Fee module data Buffer
 */
FEE_LOCAL uint8           Fee_DataBuffer[FEE_BUFFER_SIZE];
FEE_LOCAL Fls_AddressType Fee_BlockHeaderAddr;
#define FEE_STOP_SEC_VAR_CLEARED_32
#include "Fee_MemMap.h"
/* PRQA S 3408 -- */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definition ========================================== */
#define FEE_START_SEC_CODE
#include "Fee_MemMap.h"
/* PRQA S 1503 ++ */ /* VL_Fee_1503 */
/**
 * @brief Check the module status, if no error occurs, initialize global variables and set state machines
 */
void Fee_Init(const Fee_ConfigType* ConfigPtr)
{
#if (STD_ON == FEE_DEV_ERROR_DETECT)
    if (NULL_PTR == ConfigPtr)
    {
        Fee_Det_ReportError(FEE_INIT_ID, FEE_E_PARAM_POINTER);
    }
    else if (MEMIF_BUSY == Fee_ModuleStatus)
    {
        Fee_Det_ReportRunTimeError(FEE_INIT_ID, FEE_E_BUSY);
    }
    else
#endif
    {
        Fee_ConfigStd = ConfigPtr;
        /** Initialize all block manage information */
        /* PRQA S 2844 ++ */ /* VL_Fee_2844 */
        for (uint16 cnt = 0u; cnt < Fee_ConfigStd->FeeBlockSum; ++cnt)
        {
            Fee_BlockInfo[cnt].Fee_BlockStatus = FEE_BLOCK_INVALID;
            Fee_BlockInfo[cnt].Fee_DataAddr    = 0u;
#if (STD_ON == FEE_PAGE_DIRECT_WRITE_SUPPORT)
            Fee_BlockInfo[cnt].Fee_HdrAddr = 0u;
#else
            Fee_BlockInfo[cnt].Fee_ValidHdrAddr = 0u;
#endif
        }
        /* PRQA S 2844 -- */

        /* Initialize all cluster group information */
        for (uint8 ClusterGrpId = 0u; ClusterGrpId < FEE_MAX_CLUSTER_GROUP_SIZE; ClusterGrpId++)
        {
            Fee_ClusterGrpInfo[ClusterGrpId].Fee_ValidClusterId = 0u;
            Fee_ClusterGrpInfo[ClusterGrpId].Fee_ClusterUseID   = 0u;
            Fee_ClusterGrpInfo[ClusterGrpId].Fee_BlockHdrAddr   = 0u;
            Fee_ClusterGrpInfo[ClusterGrpId].Fee_BlockDataAddr  = 0u;
        }
        /** Schedule initial job */
        Fee_JobInfo.Fee_BlockId                    = 0u;
        Fee_JobInfo.Fee_Job                        = FEE_JOB_INT_SCAN;
        Fee_JobInfo.Fee_JobResult                  = MEMIF_JOB_PENDING;
        Fee_JobInfo.Fee_JobBlockOffset             = 0u;
        Fee_JobInfo.Fee_JobBlockLength             = 0u;
        Fee_JobInfo.Fee_JobReadDataDestPtr         = NULL_PTR;
        Fee_JobInfo.Fee_JobWriteDataDestPtr        = NULL_PTR;
        Fee_JobInfo.Fee_SwapJobInfo.Swap_ClusterId = 0u;
        Fee_JobInfo.Fee_SwapJobInfo.Swap_BlockId   = 0u;
        Fee_JobInfo.Fee_SwapJobInfo.Swap_DataAddr  = 0u;
        Fee_JobInfo.Fee_SwapJobInfo.Swap_HdrAddr   = 0u;
        Fee_JobInfo.Fee_SwapJobInfo.Swap_PreJob    = FEE_JOB_NONE;
        Fee_BlockHeaderAddr                        = 0u;
        /** set the module state */
        Fee_ModuleStatus = MEMIF_BUSY_INTERNAL;
    }
}
/* PRQA S 1503 -- */

#if (FEE_SET_MODE_SUPPORTED == STD_ON)
/**
 * @brief If the module has been initialized, if no task is currently executing, call lower layer to set mode
 */
void Fee_SetMode(MemIf_ModeType Mode)
{
#if (FEE_DEV_ERROR_DETECT == STD_ON)
    if (MEMIF_UNINIT == Fee_ModuleStatus)
    {
        Fee_Det_ReportError(FEE_SETMODE_ID, FEE_E_UNINIT);
    }
    else if (MEMIF_BUSY == Fee_ModuleStatus)
    {
        Fee_Det_ReportRunTimeError(FEE_SETMODE_ID, FEE_E_BUSY);
    }
    else
#endif
    {
        Fls_SetMode(Mode);
    }
}
#endif

/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
/* PRQA S 6030 ++ */ /* VL_MTR_Fee_STMIF */
/**
 * @brief If the module has been initialized, get the current block id value, if no task is currently executing, put the
 * read request into the queue
 */
Std_ReturnType Fee_Read(uint16 BlockNumber, uint16 BlockOffset, uint8* DataBufferPtr, uint16 Length)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == FEE_DEV_ERROR_DETECT)
    uint8 errorId = FEE_E_NO_ERROR;
    if (MEMIF_UNINIT == Fee_ModuleStatus)
    {
        errorId = FEE_E_UNINIT;
    }
    else if (MEMIF_BUSY == Fee_ModuleStatus)
    {
        Fee_Det_ReportRunTimeError(FEE_READ_ID, FEE_E_BUSY); /* PRQA S 2880 */ /* VL_Fee_2880 */
    }
    else
#endif
    {
        uint16 blockId = Fee_GetBlockId(BlockNumber);
#if (STD_ON == FEE_DEV_ERROR_DETECT)
        if (NULL_PTR == DataBufferPtr)
        {
            errorId = FEE_E_PARAM_POINTER;
        }
        else if (FEE_UNUSED_UINT16 == blockId)
        {
            errorId = FEE_E_INVALID_BLOCK_NO;
        }
        /* PRQA S 2824, 2814 ++ */ /* VL_Fee_2844, VL_Fee_2814 */
        else if (BlockOffset >= Fee_ConfigStd->FeeBlockRef[blockId].FeeBlockSize)
        /* PRQA S 2824, 2814 -- */
        {
            errorId = FEE_E_INVALID_BLOCK_OFS;
        }
        else if (
            (0u == Length) || ((BlockOffset + Length) > Fee_ConfigStd->FeeBlockRef[blockId].FeeBlockSize)
#if (FEE_MINIMUM_READ_PAGE_SIZE > 0u)
            || (0U != (Length % FEE_MINIMUM_READ_PAGE_SIZE))
#endif
        )
        {
            errorId = FEE_E_INVALID_BLOCK_LEN;
        }
        else
#endif
        {
            /** Schedule the read job */
            Fee_JobInfo.Fee_BlockId            = blockId;
            Fee_JobInfo.Fee_JobBlockOffset     = BlockOffset;
            Fee_JobInfo.Fee_JobBlockLength     = Length;
            Fee_JobInfo.Fee_JobReadDataDestPtr = DataBufferPtr;
            Fee_JobInfo.Fee_Job                = FEE_JOB_READ;
            Fee_JobInfo.Fee_JobResult          = MEMIF_JOB_PENDING;
            /** set the module state */
            Fee_ModuleStatus = MEMIF_BUSY;
            ret              = E_OK;
        }
    }

#if (STD_ON == FEE_DEV_ERROR_DETECT)
    if (errorId != FEE_E_NO_ERROR)
    {
        Fee_Det_ReportError(FEE_READ_ID, errorId);
    }
#endif

    return ret;
}
/* PRQA S 6030 -- */

/**
 * @brief If the module has been initialized, get the current block id value, if no task is currently executing, put the
 * write request into the queue
 */
Std_ReturnType Fee_Write(uint16 BlockNumber, const uint8* DataBufferPtr)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == FEE_DEV_ERROR_DETECT)
    uint8 errorId = FEE_E_NO_ERROR;
    if (MEMIF_UNINIT == Fee_ModuleStatus)
    {
        errorId = FEE_E_UNINIT;
    }
    else if (MEMIF_BUSY == Fee_ModuleStatus)
    {
        Fee_Det_ReportRunTimeError(FEE_WRITE_ID, FEE_E_BUSY); /* PRQA S 2880 */ /* VL_Fee_2880 */
    }
    else
#endif
    {
        uint16 blockId = Fee_GetBlockId(BlockNumber);
#if (STD_ON == FEE_DEV_ERROR_DETECT)
        if ((NULL_PTR == DataBufferPtr)
#if (FEE_BUFFER_ALIGN_VALUE > 0uL)
            || (0U != (DataBufferPtr % FEE_BUFFER_ALIGN_VALUE))
#endif
        )
        {
            errorId = FEE_E_PARAM_POINTER;
        }
        else if (FEE_UNUSED_UINT16 == blockId)
        {
            errorId = FEE_E_INVALID_BLOCK_NO;
        }
        else
#endif
        {
            /** Schedule the write job */
            Fee_JobInfo.Fee_BlockId             = blockId;
            Fee_JobInfo.Fee_Job                 = FEE_JOB_WRITE;
            Fee_JobInfo.Fee_JobResult           = MEMIF_JOB_PENDING;
            Fee_JobInfo.Fee_JobWriteDataDestPtr = DataBufferPtr;
            /** set the module state */
            Fee_ModuleStatus = MEMIF_BUSY;
            ret              = E_OK;
        }
    }

#if (STD_ON == FEE_DEV_ERROR_DETECT)
    if (errorId != FEE_E_NO_ERROR)
    {
        Fee_Det_ReportError(FEE_WRITE_ID, errorId);
    }
#endif

    return ret;
}

/**
 * @brief If the module has been initialized, get the current block id value, if the module is busy, cancel the current
 * job and set corresponding status
 */
void Fee_Cancel(void)
{
#if (FEE_CANCEL_API == STD_ON)
#if (STD_ON == FEE_DEV_ERROR_DETECT)
    if (MEMIF_UNINIT == Fee_ModuleStatus)
    {
        Fee_Det_ReportError(FEE_CANCEL_ID, FEE_E_UNINIT);
    }
    else if (MEMIF_BUSY != Fee_ModuleStatus)
    {
        /** not change the current module status or job result.
         *just raise the runtime error. */
        Fee_Det_ReportRunTimeError(FEE_CANCEL_ID, FEE_E_INVALID_CANCEL);
    }
    else
#endif
    {
        /** reset the FEE module's internal variables to make the
         *module ready for a new job request from the upper layer */
        Fee_JobInfo.Fee_Job       = FEE_JOB_NONE;
        Fee_JobInfo.Fee_JobResult = MEMIF_JOB_CANCELED;
        /** set the module state */
        Fee_ModuleStatus = MEMIF_IDLE;
        Fls_Cancel();
    }
#endif
}

/**
 * @brief return the recorded module status
 */
MemIf_StatusType Fee_GetStatus(void)
{
    return Fee_ModuleStatus;
}

/**
 * @brief If the module has been initialized, return the job result
 */
MemIf_JobResultType Fee_GetJobResult(void)
{
    MemIf_JobResultType ret = MEMIF_JOB_FAILED;
#if (STD_ON == FEE_DEV_ERROR_DETECT)
    if (MEMIF_UNINIT == Fee_ModuleStatus)
    {
        Fee_Det_ReportError(FEE_GETJOBRESULT_ID, FEE_E_UNINIT);
    }
    else
#endif
    {
        ret = Fee_JobInfo.Fee_JobResult;
    }

    return ret;
}

/**
 * @brief If the module has been initialized, get the current block id value and check it, if no error occurs and no
 * task is currently executing, put the invalidate request into the queue
 */
Std_ReturnType Fee_InvalidateBlock(uint16 BlockNumber)
{
    Std_ReturnType ret = E_OK;
#if (STD_ON == FEE_DEV_ERROR_DETECT)
    uint8 errorId = FEE_E_NO_ERROR;
    if (MEMIF_UNINIT == Fee_ModuleStatus)
    {
        errorId = FEE_E_UNINIT;
        ret     = E_NOT_OK;
    }
    else if (MEMIF_BUSY == Fee_ModuleStatus)
    {
        Fee_Det_ReportRunTimeError(FEE_INVALIDATEBLOCK_ID, FEE_E_BUSY);
        ret = E_NOT_OK;
    }
    else
#endif
    {
        uint16 blockId = Fee_GetBlockId(BlockNumber);
        if (FEE_UNUSED_UINT16 == blockId)
        {
#if (STD_ON == FEE_DEV_ERROR_DETECT)
            errorId = FEE_E_INVALID_BLOCK_NO;
#endif
            ret = E_NOT_OK;
        }
        else
        {
            /** Schedule the invalidate block job */
            Fee_JobInfo.Fee_BlockId   = blockId;
            Fee_JobInfo.Fee_Job       = FEE_JOB_INVAL_BLOCK;
            Fee_JobInfo.Fee_JobResult = MEMIF_JOB_PENDING;
            /** set the module state */
            Fee_ModuleStatus = MEMIF_BUSY;
        }
    }

#if (STD_ON == FEE_DEV_ERROR_DETECT)
    if (errorId != FEE_E_NO_ERROR)
    {
        Fee_Det_ReportError(FEE_INVALIDATEBLOCK_ID, errorId);
    }
#endif

    return ret;
}
/* PRQA S 1532 -- */

#if (FEE_VERSION_INFO_API == STD_ON)
/* PRQA S 1503 ++ */ /* VL_Fee_1503 */
/**
 * @brief If the module has been initialized, return the version information
 */
void Fee_GetVersionInfo(Std_VersionInfoType* VersionInfoPtr)
{
#if (STD_ON == FEE_DEV_ERROR_DETECT)
    if (NULL_PTR == VersionInfoPtr)
    {
        Fee_Det_ReportError(FEE_GETVERSIONINFO_ID, FEE_E_PARAM_POINTER);
    }
    else
#endif
    {
        VersionInfoPtr->moduleID         = (uint16)FEE_MODULE_ID;
        VersionInfoPtr->vendorID         = (uint16)FEE_VENDOR_ID;
        VersionInfoPtr->sw_major_version = (uint8)FEE_SW_MAJOR_VERSION;
        VersionInfoPtr->sw_minor_version = (uint8)FEE_SW_MINOR_VERSION;
        VersionInfoPtr->sw_patch_version = (uint8)FEE_SW_PATCH_VERSION;
    }
}
/* PRQA S 1503 -- */
#endif

/**
 * @brief If the module has been initialized, get the current block id value and check it, and check the block is
 * selected for erase immediate, if no error occurs and no task is currently executing, put the erase immediate request
 * into the queue
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType Fee_EraseImmediateBlock(uint16 BlockNumber)
{
    Std_ReturnType ret = E_OK;
#if (STD_ON == FEE_DEV_ERROR_DETECT)
    uint8 errorId = FEE_E_NO_ERROR;
    if (MEMIF_UNINIT == Fee_ModuleStatus)
    {
        errorId = FEE_E_UNINIT;
        ret     = E_NOT_OK;
    }
    else if (MEMIF_BUSY == Fee_ModuleStatus)
    {
        Fee_Det_ReportRunTimeError(FEE_ERASEIMMEDIATEBLOCK_ID, FEE_E_BUSY);
        ret = E_NOT_OK;
    }
    else
#endif
    {
        uint16 blockId = Fee_GetBlockId(BlockNumber);
        /* PRQA S 2824, 2814 ++ */ /* VL_Fee_2824, VL_Fee_2814 */
        if ((FEE_UNUSED_UINT16 == blockId) || (FALSE == Fee_ConfigStd->FeeBlockRef[blockId].FeeImmediateData))
        /* PRQA S 2824, 2814 -- */
        {
#if (STD_ON == FEE_DEV_ERROR_DETECT)
            errorId = FEE_E_INVALID_BLOCK_NO;
#endif
            ret = E_NOT_OK;
        }
        else
        {
            /** Schedule the erase immediate block job */
            Fee_JobInfo.Fee_BlockId   = blockId;
            Fee_JobInfo.Fee_Job       = FEE_JOB_ERASE_IMMEDIATE;
            Fee_JobInfo.Fee_JobResult = MEMIF_JOB_PENDING;
            /** set the module state */
            Fee_ModuleStatus = MEMIF_BUSY;
        }
    }

#if (STD_ON == FEE_DEV_ERROR_DETECT)
    if (errorId != FEE_E_NO_ERROR)
    {
        Fee_Det_ReportError(FEE_ERASEIMMEDIATEBLOCK_ID, errorId);
    }
#endif

    return ret;
}

/**
 * @brief Check the initialization status, in the polling mode, if a request is queued, the job is processed according
 * to the job result of the underlying fls nodule. In interrupt mode, the corresponding job request is processed
 * according to the state machine, and then the callback notification interface is called according to the requested job
 * result
 */
/* PRQA S 6030 ++ */ /* VL_MTR_Fee_STMIF */
void Fee_MainFunction(void)
{
    if (MEMIF_UNINIT != Fee_ModuleStatus)
    {
        if (MEMIF_JOB_PENDING == Fee_JobInfo.Fee_JobResult)
        {
#if (STD_ON == FEE_POLLING_MODE)
            /** Internal job handle */
            if (Fee_JobInfo.Fee_Job > FEE_JOB_ERASE_IMMEDIATE)
            {
                MemIf_JobResultType flsJobResult = Fls_GetJobResult();
                if (MEMIF_JOB_PENDING == flsJobResult)
                {
                    /** Nothing to do */
                }
                else if (MEMIF_JOB_OK == flsJobResult)
                {
                    /** Fls job success end handle */
                    Fee_FlsJobEndHandle();
                }
                else
                {
                    /** Fls job error end handle */
                    Fee_FlsJobErrorHandle();
                }
            }
            else
#endif
            {
                Fee_MainFunctionSub();
            }
        }
    }
}
/* PRQA S 6030 -- */

/**
 * @brief If the module has been initialized, invokes the interface to notifies the upper-layer application that job is
 * completed
 */
/* PRQA S 1503 ++ */ /* VL_Fee_1503 */
void Fee_JobEndNotification(void)
{
#if (STD_OFF == FEE_POLLING_MODE)
#if (STD_ON == FEE_DEV_ERROR_DETECT)
    if (MEMIF_UNINIT == Fee_ModuleStatus)
    {
        Fee_Det_ReportError(FEE_JOBENDNOTIFICATION_ID, FEE_E_UNINIT);
    }
    else
#endif
    {
        Fee_JobInfo.Fee_JobResult = Fls_GetJobResult();
        if ((Fee_JobInfo.Fee_JobResult == MEMIF_JOB_OK) || (Fee_JobInfo.Fee_JobResult == MEMIF_JOB_PENDING))
        {
            Fee_FlsJobEndHandle();
        }
        else
        {
            Fee_FlsJobErrorHandle();
        }
    }
#endif
}
/* PRQA S 1532 -- */
/* PRQA S 1503 -- */
/* ========================================== internal function definition ========================================== */
/**
 * @brief According to the job state and module state of fee, the end callback notification interface is invoked
 */
FEE_LOCAL void Fee_FlsJobEndHandle(void)
{
    if (FEE_JOB_NONE == Fee_JobInfo.Fee_Job)
    {
        Fee_JobInfo.Fee_JobResult = MEMIF_JOB_OK;
        if (MEMIF_BUSY_INTERNAL == Fee_ModuleStatus)
        {
            /** no notifications from internal jobs */
            Fee_ModuleStatus = MEMIF_IDLE;
        }
        else
        {
            Fee_ModuleStatus = MEMIF_IDLE;
            /** Call job end notification function */
            FEE_NVMJOBEND_NOTIFICATION
        }
    }
    else
    {
        Fee_JobInfo.Fee_JobResult = Fee_JobSchedule();
        if (MEMIF_JOB_OK == Fee_JobInfo.Fee_JobResult)
        {
            if (MEMIF_BUSY_INTERNAL == Fee_ModuleStatus)
            {
                /** no notifications from internal jobs */
                Fee_ModuleStatus = MEMIF_IDLE;
            }
            else
            {
                Fee_ModuleStatus = MEMIF_IDLE;
                /** Call job end notification function */
                FEE_NVMJOBEND_NOTIFICATION
            }
        }
        else if (MEMIF_JOB_PENDING == Fee_JobInfo.Fee_JobResult)
        {
            /** Nothing to do (ongoing Fls job) */
        }
        else
        {
            if (MEMIF_BUSY_INTERNAL == Fee_ModuleStatus)
            {
                /** Call initial job error notification function */
                Fee_ModuleStatus = MEMIF_UNINIT;
                FEE_INITERROR_NOTIFICATION
            }
            else
            {
                Fee_ModuleStatus = MEMIF_IDLE;
                /** Call job error notification function */
                FEE_NVMJOBERROR_NOTIFICATION
            }
        }
    }
}

/**
 * @brief According to the job state and job result of the Fls module, the error callback notification interface is
 * invoked
 */
FEE_LOCAL void Fee_FlsJobErrorHandle(void)
{
    if (MEMIF_JOB_CANCELED == Fee_JobInfo.Fee_JobResult)
    {
        FEE_NVMJOBERROR_NOTIFICATION
    }
    else
    {
        switch (Fee_JobInfo.Fee_Job)
        {
        /** Error while reading block header. Treat block header as
            invalid and reading the next block headers */
        case FEE_JOB_INT_SCAN_BLOCK_HDR_GET:
            Fee_JobInfo.Fee_JobResult = Fee_JobIntScanBlockHdrGet((boolean)FALSE);
            break;
        /** Error while reading cluster header. Treat cluster as
            invalid and move on to next cluster */
        case FEE_JOB_INT_SCAN_CLUSTER_HDR_GET:
            Fee_JobInfo.Fee_JobResult = Fee_JobIntScanClusterHdrGet((boolean)FALSE);
            break;
        case FEE_JOB_WRITE_DATA:
        case FEE_JOB_WRITE_VALIDATE:
        case FEE_JOB_WRITE_DONE:
        case FEE_JOB_ERASE_IMMEDIATE_DONE:
            Fee_JobInfo.Fee_JobResult = Fls_GetJobResult();
            Fee_ModuleStatus          = MEMIF_IDLE;
            /** Call job error notification function */
            FEE_NVMJOBERROR_NOTIFICATION
            break;
        case FEE_JOB_NONE:
        case FEE_JOB_INVAL_BLOCK_DONE:
        case FEE_JOB_INT_SCAN_CLUSTER_CLEAR:
        case FEE_JOB_INT_SCAN_CLUSTER_CLEAR_DONE:
        case FEE_JOB_INT_SWAP_CLUSTER_CLEAR:
        case FEE_JOB_INT_SWAP_BLOCK:
        case FEE_JOB_INT_SWAP_DATA_READ:
        case FEE_JOB_INT_SWAP_DATA_WRITE:
        case FEE_JOB_INT_SWAP_BLOCK_VLD:
        case FEE_JOB_INT_SWAP_CLUSTER_VLD_DONE:
            Fee_JobInfo.Fee_JobResult = Fls_GetJobResult();
            if (MEMIF_BUSY_INTERNAL == Fee_ModuleStatus)
            {
                /** Call initial job error notification function */
                Fee_ModuleStatus = MEMIF_UNINIT;
                FEE_INITERROR_NOTIFICATION
            }
            else
            {
                Fee_ModuleStatus = MEMIF_IDLE;
                /** Call job error notification function */
                FEE_NVMJOBERROR_NOTIFICATION
            }
            break;
        default:
            /** Never run to here,unless administrative data has been tampered */
            /** FEE_JOB_WRITE,FEE_JOB_READ,FEE_JOB_INVAL_BLOCK,
             *FEE_JOB_ERASE_IMMEDIATE,FEE_JOB_INT_SCAN */
            break;
        }
    }
}

/**
 * @brief Use Binary Search to start looking up the ids of all configured blocks
 */
FEE_LOCAL uint16 Fee_GetBlockId(const uint16 BlockNumber)
{
    sint32 sLow  = 0;
    sint32 sHigh = (sint32)(Fee_ConfigStd->FeeBlockSum) - 1; /* PRQA S 2814,1253 */ /* VL_Fee_2814,VL_Fee_1253 */
    uint16 ret   = FEE_UNUSED_UINT16;

    while (sLow <= sHigh)
    {
        /* PRQA S 3103, 3120,1253 ++ */ /* VL_Fee_3103, VL_QAC_MagicNum,VL_Fee_1253 */
        sint32 sMiddle = sLow + ((sHigh - sLow) / 2);
        /* PRQA S 3103, 3120,1253 -- */
        /* PRQA S 2824 ++ */ /* VL_Fee_2824 */
        uint16 sMiddleBlockNumber = Fee_ConfigStd->FeeBlockRef[sMiddle].FeeBlockNumber;
        /* PRQA S 2824 -- */
        if (BlockNumber < sMiddleBlockNumber)
        {
            sHigh = sMiddle - 1; /* PRQA S 1253 */ /* VL_Fee_1253 */
        }
        else if (BlockNumber > sMiddleBlockNumber)
        {
            sLow = sMiddle + 1; /* PRQA S 1253 */ /* VL_Fee_1253 */
        }
        else
        {
            ret = (uint16)sMiddle;
            break;
        }
    }
    return ret;
}

/**
 * @brief Read the cluster header infomation arrcording to the header size
 */
FEE_LOCAL MemIf_JobResultType Fee_JobIntScanClusterHdrRead(void)
{
    MemIf_JobResultType ret;

    /** active Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_ClusterScanId must be 0 */
    Fls_AddressType clusterStartAddr =
        Fee_ClusterGroupConfig[Fee_ClusterGroupIdx].ClusterPtr[Fee_ClusterIdx].FeeClusterStartAddr;
    if (((Std_ReturnType)E_OK) != Fls_Read(clusterStartAddr, Fee_DataBuffer, FEE_CLUSTER_HEADER_SIZE))
    {
        /** Fls read job hasn't been accepted */
        ret = MEMIF_JOB_FAILED;
    }
    else
    {
        /** Fls read job has been accepted */
        ret = MEMIF_JOB_PENDING;
    }
    Fee_JobInfo.Fee_Job = FEE_JOB_INT_SCAN_CLUSTER_HDR_GET;
    return ret;
}

/**
 * @brief Initial job handle,try to read cluster header
 */
FEE_LOCAL void Fee_JobInitScan(void)
{
    Fee_ClusterGroupIdx       = 0u;
    Fee_ClusterIdx            = 0u;
    Fee_JobInfo.Fee_JobResult = Fee_JobIntScanClusterHdrRead();
}

/**
 * @brief All internal jobs handle
 */
/* PRQA S 3223 ++ */ /* VL_Fee_3223 */
FEE_LOCAL MemIf_JobResultType Fee_JobSchedule(void)
{
    MemIf_JobResultType            ret                           = MEMIF_JOB_FAILED;
    FEE_LOCAL const Fee_JobFuncPtr Fee_JobResultTypeFuncMatrix[] = {
        NULL_PTR,                        /** FEE_JOB_INT_SCAN */
        NULL_PTR,                        /** FEE_JOB_READ */
        NULL_PTR,                        /** FEE_JOB_WRITE */
        NULL_PTR,                        /** FEE_JOB_INVAL_BLOCK */
        NULL_PTR,                        /** FEE_JOB_ERASE_IMMEDIATE */
        Fee_JobWriteData,                /** FEE_JOB_WRITE_DATA */
        Fee_JobWriteValidate,            /** FEE_JOB_WRITE_VALIDATE */
        &Fee_JobWriteDone,               /** FEE_JOB_WRITE_DONE */
        &Fee_JobInvalBlockDone,          /** FEE_JOB_INVAL_BLOCK_DONE */
        &Fee_JobEraseImmediateDone,      /** FEE_JOB_ERASE_IMMEDIATE_DONE */
        NULL_PTR,                        /** FEE_JOB_INT_SCAN_CLUSTER_HDR_GET */
        &Fee_JobIntScanClusterClear,     /** FEE_JOB_INT_SCAN_CLUSTER_CLEAR */
        &Fee_JobIntScanClusterClearDone, /** FEE_JOB_INT_SCAN_CLUSTER_CLEAR_DONE */
        NULL_PTR,                        /** FEE_JOB_INT_SCAN_BLOCK_HDR_GET */
        &Fee_JobIntSwapClusterClear,     /** FEE_JOB_INT_SWAP_CLUSTER_CLEAR */
        &Fee_JobIntSwapBlock,            /** FEE_JOB_INT_SWAP_BLOCK */
        &Fee_JobIntSwapDataRead,         /** FEE_JOB_INT_SWAP_DATA_READ */
        &Fee_JobIntSwapDataWrite,        /** FEE_JOB_INT_SWAP_DATA_WRITE */
        &Fee_JobIntSwapBlockVldWrite,    /** FEE_JOB_INT_SWAP_BLOCK_VLD */
        &Fee_JobIntSwapClusterVldDone,   /** FEE_JOB_INT_SWAP_CLUSTER_VLD_DONE */
        NULL_PTR,                        /** FEE_JOB_NONE */
    };

    if (Fee_JobInfo.Fee_Job == FEE_JOB_INT_SCAN_CLUSTER_HDR_GET)
    {
        /** Initial related jobs */
        ret = Fee_JobIntScanClusterHdrGet((boolean)TRUE);
    }
    else if (Fee_JobInfo.Fee_Job == FEE_JOB_INT_SCAN_BLOCK_HDR_GET)
    {
        ret = Fee_JobIntScanBlockHdrGet((boolean)TRUE);
    }
    else
    {
        /* PRQA S 2814, 2844 ++ */ /* VL_Fee_2814,VL_Fee_2844 */
        if (Fee_JobResultTypeFuncMatrix[Fee_JobInfo.Fee_Job] != NULL_PTR)
        {
            ret = (*Fee_JobResultTypeFuncMatrix[Fee_JobInfo.Fee_Job])();
        }
        /* PRQA S 2814, 2844 -- */
    }

    return ret;
}
/* PRQA S 3223 -- */
/**
 * @brief If the block is valid, the underlying interface is called to read the data
 */
FEE_LOCAL void Fee_JobRead(void)
{
    /* PRQA S 2844 ++ */ /* VL_Fee_2844 */
    Fee_BlockStatusType blockStatus = Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_BlockStatus;
    /* PRQA S 2844 -- */

    if (FEE_BLOCK_VALID == blockStatus)
    {
        Fls_AddressType blockAddress = Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_DataAddr;

        if (((Std_ReturnType)E_OK)
            != Fls_Read(
                blockAddress + Fee_JobInfo.Fee_JobBlockOffset,
                Fee_JobInfo.Fee_JobReadDataDestPtr,
                Fee_JobInfo.Fee_JobBlockLength))
        {
            /** Fls read job hasn't been accepted */
            Fee_JobInfo.Fee_JobResult = MEMIF_JOB_FAILED;
        }
        else
        {
            /** Fls read job has been accepted */
            Fee_JobInfo.Fee_JobResult = MEMIF_JOB_PENDING;
        }
    }
    else if (FEE_BLOCK_INVALID == blockStatus)
    {
        Fee_JobInfo.Fee_JobResult = MEMIF_BLOCK_INVALID;
    }
    else if (FEE_BLOCK_INCONSISTENT == blockStatus)
    {
        Fee_JobInfo.Fee_JobResult = MEMIF_BLOCK_INCONSISTENT;
    }
    else
    {
        Fee_JobInfo.Fee_JobResult = MEMIF_JOB_FAILED;
    }
    /** Job switch to FEE_JOB_NONE */
    Fee_JobInfo.Fee_Job = FEE_JOB_NONE;
}

/**
 * @brief Immediate data calls the immediate write interface, otherwise the write header information and queues the
 * write request
 */
FEE_LOCAL void Fee_JobWrite(void)
{
    /* PRQA S 2824, 2814 ++ */ /* VL_Fee_2814 */
    if (TRUE == Fee_ConfigStd->FeeBlockRef[Fee_JobInfo.Fee_BlockId].FeeImmediateData)
    /* PRQA S 2824, 2814 -- */
    {
        /** ImmediateData block's header has been written done */
        if (FEE_BLOCK_INCONSISTENT
            == Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_BlockStatus) /* PRQA S 2844 */ /* VL_Fee_2844 */
        {
            /** The immediate block is ready to be written */
            Fee_JobInfo.Fee_JobResult = Fee_JobWriteData();
        }
        else
        {
            /** The immediate block is not pre-erased and ready to be written */
            Fee_JobInfo.Fee_JobResult = MEMIF_JOB_FAILED;
        }
    }
    else
    {
        /** Write block header */
        Fee_JobInfo.Fee_JobResult = Fee_JobWriteHdr();
        /** Schedule the next internal job */
        if (FEE_JOB_INT_SWAP_CLUSTER_CLEAR == Fee_JobInfo.Fee_Job)
        {
            /** cluster swap has been enforced... */
        }
        else
        {
            Fee_JobInfo.Fee_Job = FEE_JOB_WRITE_DATA;
        }
    }
}

/**
 * @brief Invalidate Fee block by writing the invalid flag to flash
 */
FEE_LOCAL void Fee_JobInvalBlock(void)
{
    if (FEE_BLOCK_INVALID == Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_BlockStatus) /* PRQA S 2844 */ /* VL_Fee_2844 */
    {
        /** Fee job completed */
        Fee_JobInfo.Fee_Job       = FEE_JOB_NONE;
        Fee_JobInfo.Fee_JobResult = MEMIF_JOB_OK;
    }
    else
    {
#if (STD_ON == FEE_PAGE_DIRECT_WRITE_SUPPORT)
        /** Pack invalid flag to buffer */
        Fee_PackBlockHdrValidOrInvalid(Fee_DataBuffer, FEE_INVALID_VALUE);
        /** Write invalidation pattern to flash */
        if (((Std_ReturnType)E_OK)
            == Fls_Write(Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_HdrAddr, Fee_DataBuffer, FEE_VIRTUALPAGE_SIZE))
#else
        /** Pack invalid flag to buffer */
        Fee_PackHdrValidOrInvalid(Fee_DataBuffer, FEE_INVALID_VALUE);
        /** Write invalidation pattern to flash */ /* PRQA S 1252 ++ */ /* VL_QAC_1252 */
        if (((Std_ReturnType)E_OK)
            == Fls_Write(
                (Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_ValidHdrAddr + FEE_VIRTUALPAGE_SIZE),
                Fee_DataBuffer,
                FEE_VIRTUALPAGE_SIZE))
        /* PRQA S 1252 -- */
#endif
        {
            /** Fls write job has been accepted */
            Fee_JobInfo.Fee_JobResult = MEMIF_JOB_PENDING;
        }
        else
        {
            /** Fls write job hasn't been accepted */
            Fee_JobInfo.Fee_JobResult = MEMIF_JOB_FAILED;
        }
        /** Switch job to FEE_JOB_INVAL_BLOCK_DONE */
        Fee_JobInfo.Fee_Job = FEE_JOB_INVAL_BLOCK_DONE;
    }
}

/**
 * @brief Erase (pre-allocate) immediate Fee block
 */
FEE_LOCAL void Fee_JobEraseImmediate(void)
{
    /** Write block header */
    Fee_JobInfo.Fee_JobResult = Fee_JobWriteHdr();
    /** Schedule the next internal job */
    if (FEE_JOB_INT_SWAP_CLUSTER_CLEAR == Fee_JobInfo.Fee_Job)
    {
        /** cluster swap has been enforced... */
    }
    else
    {
        Fee_JobInfo.Fee_Job = FEE_JOB_ERASE_IMMEDIATE_DONE;
    }
}

/**
 * @brief Invalidation of Fee block done
 */
FEE_LOCAL MemIf_JobResultType Fee_JobInvalBlockDone(void)
{
    /** Mark the Fee block as invalid */
    Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_BlockStatus = FEE_BLOCK_INVALID; /* PRQA S 2844 */ /* VL_Fee_2844 */
    /** Fee job completed */
    Fee_JobInfo.Fee_Job = FEE_JOB_NONE;
    return MEMIF_JOB_OK;
}

/**
 * @brief Erase immediate Fee block done
 */
FEE_LOCAL MemIf_JobResultType Fee_JobEraseImmediateDone(void)
{
    uint8 clusterGrpIdx = Fee_ConfigStd->FeeBlockRef[Fee_JobInfo.Fee_BlockId].ClusterGrp;
    /* PRQA S 2824, 2814 ++ */ /* VL_Fee_2814 */
    uint16 blockSize = Fee_ConfigStd->FeeBlockRef[Fee_JobInfo.Fee_BlockId].FeeBlockSize;
    /* PRQA S 2824, 2814 -- */
    /** Header has been written so update both header and data addresses of the cluster*/
    Fee_ClusterGrpInfo[clusterGrpIdx].Fee_BlockDataAddr -= blockSize;
    Fee_ClusterGrpInfo[clusterGrpIdx].Fee_BlockHdrAddr += FEE_BLOCK_HEADER_SIZE; /* PRQA S 1252 */ /* VL_QAC_1252 */
    /** The block header has written,but the block data not write */
    /* PRQA S 2844 ++ */ /* VL_Fee_2844 */
    Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_BlockStatus = FEE_BLOCK_INCONSISTENT;
    Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_DataAddr    = Fee_ClusterGrpInfo[clusterGrpIdx].Fee_BlockDataAddr;
#if (STD_ON == FEE_PAGE_DIRECT_WRITE_SUPPORT)
    Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_HdrAddr =
        Fee_ClusterGrpInfo[clusterGrpIdx].Fee_BlockHdrAddr - FEE_BLOCK_HEADER_SIZE;
#else
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_ValidHdrAddr =
        (Fee_ClusterGrpInfo[clusterGrpIdx].Fee_BlockHdrAddr - (2u * FEE_VIRTUALPAGE_SIZE));
    /* PRQA S 2844 -- */
    /* PRQA S 3120 -- */
#endif
    /** Set the job to NONE */
    Fee_JobInfo.Fee_Job = FEE_JOB_NONE;
    return MEMIF_JOB_OK;
}

/**
 * @brief Write Fee block header to flash
 */
FEE_LOCAL MemIf_JobResultType Fee_JobWriteHdr(void)
{
    MemIf_JobResultType ret;
    uint8               clusterGrpIdx = Fee_ConfigStd->FeeBlockRef[Fee_JobInfo.Fee_BlockId].ClusterGrp;
    /* PRQA S 2824, 2814 ++ */ /* VL_Fee_2814 */
    uint16 blockSize = Fee_ConfigStd->FeeBlockRef[Fee_JobInfo.Fee_BlockId].FeeBlockSize;
    /* PRQA S 2824, 2814 -- */
    /*check the cluster have enough space or not */ /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    if ((((uint32)blockSize) + (2u * FEE_BLOCK_HEADER_SIZE))
        > (Fee_ClusterGrpInfo[clusterGrpIdx].Fee_BlockDataAddr - Fee_ClusterGrpInfo[clusterGrpIdx].Fee_BlockHdrAddr))
    /* PRQA S 3120 -- */
    {
        /** No enough space,perform cluster swap handle */
        ret = Fee_JobIntSwap();
    }
    else
    {
        Fls_AddressType dataAddr = Fee_ClusterGrpInfo[clusterGrpIdx].Fee_BlockDataAddr - blockSize;
        uint16          blockNum = Fee_ConfigStd->FeeBlockRef[Fee_JobInfo.Fee_BlockId].FeeBlockNumber;
        uint8           offSet   = Fee_PackBlockHdrSubOne(blockNum, blockSize, dataAddr, Fee_DataBuffer);
        /** Calculate the block header checksum */
        uint32  checkSum       = (uint32)blockNum + (uint32)blockSize + dataAddr;
        boolean immediateBlock = Fee_ConfigStd->FeeBlockRef[Fee_JobInfo.Fee_BlockId].FeeImmediateData;
        Fee_PackBlockHdrSubTwo(&checkSum, immediateBlock, Fee_DataBuffer, offSet);
        /** Write header to flash */
        if (((Std_ReturnType)E_OK)
            != Fls_Write(
                Fee_ClusterGrpInfo[clusterGrpIdx].Fee_BlockHdrAddr,
                Fee_DataBuffer,
#if (STD_ON == FEE_PAGE_DIRECT_WRITE_SUPPORT)
                FEE_BLOCK_HEADER_SIZE))
#else
                FEE_BLOCK_HEADER_SIZE - (2u * FEE_VIRTUALPAGE_SIZE))) /* PRQA S 3120 */ /* VL_QAC_MagicNum */
#endif
        {
            /** Fls read job hasn't been accepted */
            ret = MEMIF_JOB_FAILED;
        }
        else
        {
            /** Fls read job has been accepted */
            ret = MEMIF_JOB_PENDING;
        }
    }
    return ret;
}

/**
 * @brief Initialize the cluster swap internal operation, This can only happen during the block header writing phase
 */
FEE_LOCAL MemIf_JobResultType Fee_JobIntSwap(void)
{
    /** Store type of original job so Fee can re-schedule
        this job once the swap is complete */
    Fee_JobInfo.Fee_SwapJobInfo.Swap_PreJob = Fee_JobInfo.Fee_Job;
    /** Update the swap information */
    Fee_JobInfo.Fee_SwapJobInfo.Swap_BlockId   = 0u;
    Fee_JobInfo.Fee_SwapJobInfo.Swap_ClusterId = Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_ValidClusterId + 1u;
    if (Fee_JobInfo.Fee_SwapJobInfo.Swap_ClusterId == Fee_ClusterGroupConfig[Fee_ClusterGroupIdx].ClusterCount)
    {
        Fee_JobInfo.Fee_SwapJobInfo.Swap_ClusterId = 0u;
    }
    /* PRQA S 2844 ++ */ /* VL_Fee_2844 */
    Fee_JobInfo.Fee_SwapJobInfo.Swap_HdrAddr = Fee_ClusterGroupConfig[Fee_ClusterGroupIdx]
                                                   .ClusterPtr[Fee_JobInfo.Fee_SwapJobInfo.Swap_ClusterId]
                                                   .FeeClusterStartAddr;
    /* PRQA S 2844 -- */
    Fee_JobInfo.Fee_SwapJobInfo.Swap_DataAddr =
        (Fee_JobInfo.Fee_SwapJobInfo.Swap_HdrAddr
         + Fee_ClusterGroupConfig[Fee_ClusterGroupIdx]
               .ClusterPtr[Fee_JobInfo.Fee_SwapJobInfo.Swap_ClusterId]
               .FeeClusterLength);
    Fee_JobInfo.Fee_SwapJobInfo.Swap_HdrAddr += FEE_CLUSTER_HEADER_SIZE; /* PRQA S 1252 */ /* VL_QAC_1252 */
    /** Erase the swap cluster */
    MemIf_JobResultType ret = Fee_JobIntSwapClusterErase();
    return ret;
}

/**
 * @brief Erase the new cluster for swap
 */
FEE_LOCAL MemIf_JobResultType Fee_JobIntSwapClusterErase(void)
{
    MemIf_JobResultType ret;

    /** Get the new cluster start address and length */ /* PRQA S 2844 ++ */ /* VL_Fee_2844 */
    Fls_AddressType startAddr = Fee_ClusterGroupConfig[Fee_ClusterGroupIdx]
                                    .ClusterPtr[Fee_JobInfo.Fee_SwapJobInfo.Swap_ClusterId]
                                    .FeeClusterStartAddr;
    /* PRQA S 2844 -- */
    Fls_LengthType length = Fee_ClusterGroupConfig[Fee_ClusterGroupIdx]
                                .ClusterPtr[Fee_JobInfo.Fee_SwapJobInfo.Swap_ClusterId]
                                .FeeClusterLength;
    /** Erase cluster */
    if (((Std_ReturnType)E_OK) == Fls_Erase(startAddr, length))
    {
        /** Fls read job has been accepted */
        ret = MEMIF_JOB_PENDING;
    }
    else
    {
        /** Fls read job hasn't been accepted */
        ret = MEMIF_JOB_FAILED;
    }
    /** Switch job to FEE_JOB_INT_SWAP_CLUSTER_CLEAR */
    Fee_JobInfo.Fee_Job = FEE_JOB_INT_SWAP_CLUSTER_CLEAR;
    return ret;
}

/**
 * @brief Clear current Fee cluster done,writing cluster header into flash
 */
FEE_LOCAL MemIf_JobResultType Fee_JobIntSwapClusterClear(void)
{
    MemIf_JobResultType ret;

    /* PRQA S 2844 ++ */ /* VL_Fee_2844 */
    Fls_AddressType startAddr = Fee_ClusterGroupConfig[Fee_ClusterGroupIdx]
                                    .ClusterPtr[Fee_JobInfo.Fee_SwapJobInfo.Swap_ClusterId]
                                    .FeeClusterStartAddr;
    /* PRQA S 2844 -- */
    Fls_LengthType length = Fee_ClusterGroupConfig[Fee_ClusterGroupIdx]
                                .ClusterPtr[Fee_JobInfo.Fee_SwapJobInfo.Swap_ClusterId]
                                .FeeClusterLength;
    uint32 clusterUseID = Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_ClusterUseID;
    /** Build cluster header information */
    Fee_PackClusterHdr(clusterUseID, startAddr, length, Fee_DataBuffer);

    /** Write the cluster header to flash */
    if (((Std_ReturnType)E_OK) == Fls_Write(startAddr, Fee_DataBuffer, FEE_CLUSTER_HEADER_SIZE - FEE_VIRTUALPAGE_SIZE))
    {
        /** Fls read job has been accepted */
        ret = MEMIF_JOB_PENDING;
    }
    else
    {
        /** Fls write job hasn't been accepted */
        ret = MEMIF_JOB_FAILED;
    }
    /** Switch job to FEE_JOB_INT_SWAP_BLOCK */
    Fee_JobInfo.Fee_Job = FEE_JOB_INT_SWAP_BLOCK;
    return ret;
}

/**
 * @brief Copy block from source to target cluster
 */
FEE_LOCAL MemIf_JobResultType Fee_JobIntSwapBlock(void)
{
    MemIf_JobResultType ret;
    Fee_BlockStatusType blockStatus    = FEE_BLOCK_VALID;
    boolean             immediateBlock = FALSE;
    uint16              blockId        = Fee_JobInfo.Fee_SwapJobInfo.Swap_BlockId;

    for (; blockId < Fee_ConfigStd->FeeBlockSum; ++blockId) /* PRQA S 2814 */ /* VL_Fee_2814 */
    {
        blockStatus    = Fee_BlockInfo[blockId].Fee_BlockStatus; /* PRQA S 2844 */               /* VL_Fee_2844 */
        immediateBlock = Fee_ConfigStd->FeeBlockRef[blockId].FeeImmediateData; /* PRQA S 2824 */ /* VL_Fee_2824 */
        if ((FEE_BLOCK_VALID == blockStatus) || ((TRUE == immediateBlock) && (FEE_BLOCK_INCONSISTENT == blockStatus)))
        {
            break;
        }
        else
        {
            /** No match. Try next one... */
        }
    }
    Fee_JobInfo.Fee_SwapJobInfo.Swap_BlockId = blockId;
    if (blockId == Fee_ConfigStd->FeeBlockSum)
    {
        /** No more matching blocks. Validate the cluster */
        ret = Fee_JobIntSwapClusterVld();
    }
    else
    {
        uint16          blockSize = Fee_ConfigStd->FeeBlockRef[blockId].FeeBlockSize;
        uint16          blockNum  = Fee_ConfigStd->FeeBlockRef[blockId].FeeBlockNumber;
        Fls_AddressType dataAddr  = Fee_JobInfo.Fee_SwapJobInfo.Swap_DataAddr - blockSize;
        /** Pack the block header information */
        uint8 offSet = Fee_PackBlockHdrSubOne(blockNum, blockSize, dataAddr, Fee_DataBuffer);
        /** Calculate the block header checksum */
        uint32 checkSum = (uint32)blockNum + (uint32)blockSize + dataAddr;
        Fee_PackBlockHdrSubTwo(&checkSum, immediateBlock, Fee_DataBuffer, offSet);
        /** Write header to flash */
        if (((Std_ReturnType)E_OK)
            != Fls_Write(
                Fee_JobInfo.Fee_SwapJobInfo.Swap_HdrAddr,
                Fee_DataBuffer,
#if (STD_ON == FEE_PAGE_DIRECT_WRITE_SUPPORT)
                FEE_BLOCK_HEADER_SIZE))
#else
                FEE_BLOCK_HEADER_SIZE - (2u * FEE_VIRTUALPAGE_SIZE))) /* PRQA S 3120 */ /* VL_QAC_MagicNum */
#endif
        {
            /** Fls write job hasn't been accepted */
            ret = MEMIF_JOB_FAILED;
        }
        else
        {
            /** Fls write job has been accepted */
            ret = MEMIF_JOB_PENDING;
        }
        if (FEE_BLOCK_VALID == blockStatus)
        {
            /*Read the block data*/
            Fee_JobInfo.Fee_Job = FEE_JOB_INT_SWAP_DATA_READ;
        }
        else
        {
            /** Move on to next block */
            Fee_JobInfo.Fee_SwapJobInfo.Swap_BlockId += 1u;
            Fee_JobInfo.Fee_Job = FEE_JOB_INT_SWAP_BLOCK;
        }
        /** Update the swap cluster address(data&header) */
        Fee_JobInfo.Fee_SwapJobInfo.Swap_DataAddr = dataAddr;
        Fee_JobInfo.Fee_SwapJobInfo.Swap_HdrAddr += FEE_BLOCK_HEADER_SIZE; /* PRQA S 1252 */ /* VL_QAC_1252 */
    }
    return ret;
}

/**
 * @brief Success write cluster valid flag,update all copied block information
 */
FEE_LOCAL MemIf_JobResultType Fee_JobIntSwapClusterVldDone(void)
{
    MemIf_JobResultType ret;
    /* PRQA S 2844 ++ */ /* VL_Fee_2844 */
    Fls_AddressType hdrAddr = Fee_ClusterGroupConfig[Fee_ClusterGroupIdx]
                                  .ClusterPtr[Fee_JobInfo.Fee_SwapJobInfo.Swap_ClusterId]
                                  .FeeClusterStartAddr;
    /* PRQA S 2844 -- */
    Fls_AddressType dataAddr = hdrAddr
                               + Fee_ClusterGroupConfig[Fee_ClusterGroupIdx]
                                     .ClusterPtr[Fee_JobInfo.Fee_SwapJobInfo.Swap_ClusterId]
                                     .FeeClusterLength;
    hdrAddr += FEE_CLUSTER_HEADER_SIZE; /* PRQA S 1252 */ /* VL_QAC_1252 */

    /** update all copied block information */
    for (uint16 cnt = 0u; cnt < Fee_ConfigStd->FeeBlockSum; ++cnt) /* PRQA S 2814 */ /* VL_Fee_2814 */
    {
        /* PRQA S 2824 ++ */ /* VL_Fee_2824 */
        boolean immediateBlock = Fee_ConfigStd->FeeBlockRef[cnt].FeeImmediateData;
        if ((FEE_BLOCK_VALID == Fee_BlockInfo[cnt].Fee_BlockStatus)
            || ((FEE_BLOCK_INCONSISTENT == Fee_BlockInfo[cnt].Fee_BlockStatus) && ((boolean)TRUE == immediateBlock)))
        {
            uint16 blockSize = Fee_ConfigStd->FeeBlockRef[cnt].FeeBlockSize;
            dataAddr -= blockSize;
            hdrAddr += FEE_BLOCK_HEADER_SIZE; /* PRQA S 1252 */ /* VL_QAC_1252 */
            Fee_BlockInfo[cnt].Fee_DataAddr = dataAddr;
#if (STD_ON == FEE_PAGE_DIRECT_WRITE_SUPPORT)
            Fee_BlockInfo[cnt].Fee_HdrAddr = hdrAddr - FEE_BLOCK_HEADER_SIZE;
#else
            /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
            Fee_BlockInfo[cnt].Fee_ValidHdrAddr = (hdrAddr - (2u * FEE_VIRTUALPAGE_SIZE));
            /* PRQA S 3120 -- */
#endif
        }
        /* PRQA S 2824 -- */
    }
    /** Switch to the new cluster */
    Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_ClusterUseID += 1uL;
    Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_ValidClusterId = Fee_JobInfo.Fee_SwapJobInfo.Swap_ClusterId;
    Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_BlockDataAddr  = dataAddr;
    Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_BlockHdrAddr   = hdrAddr;
    /** Restores the pre-swap job */
    Fee_JobInfo.Fee_Job = Fee_JobInfo.Fee_SwapJobInfo.Swap_PreJob;
    if (FEE_JOB_WRITE == Fee_JobInfo.Fee_Job)
    {
        Fee_JobWrite();
        ret = Fee_JobInfo.Fee_JobResult;
    }
    else if (FEE_JOB_ERASE_IMMEDIATE == Fee_JobInfo.Fee_Job)
    {
        Fee_JobEraseImmediate();
        ret = Fee_JobInfo.Fee_JobResult;
    }
    else
    {
        /** never run to here,unless administrative data has been tampered */
        ret = MEMIF_JOB_FAILED;
    }
    return ret;
}

/**
 * @brief Validate current cluster by writing FEE_VALID_VALUE into flash
 */
FEE_LOCAL MemIf_JobResultType Fee_JobIntSwapClusterVld(void)
{
    MemIf_JobResultType ret;

    /** Pack valid flag to buffer */
    Fee_PackHdrValidOrInvalid(Fee_DataBuffer, FEE_VALID_VALUE);

    /* PRQA S 2844 ++ */ /* VL_Fee_2844 */
    Fls_AddressType startAddr = Fee_ClusterGroupConfig[Fee_ClusterGroupIdx]
                                    .ClusterPtr[Fee_JobInfo.Fee_SwapJobInfo.Swap_ClusterId]
                                    .FeeClusterStartAddr;
    /* PRQA S 2844 -- */
    /** Write validation pattern to flash */ /* PRQA S 1252 ++ */ /* VL_QAC_1252 */
    if (((Std_ReturnType)E_OK)
        == Fls_Write(
            (startAddr + FEE_CLUSTER_HEADER_SIZE - FEE_VIRTUALPAGE_SIZE),
            Fee_DataBuffer,
            FEE_VIRTUALPAGE_SIZE))
    /* PRQA S 1252 -- */
    {
        /** Fls write job has been accepted */
        ret = MEMIF_JOB_PENDING;
    }
    else
    {
        /** Fls write job hasn't been accepted */
        ret = MEMIF_JOB_FAILED;
    }
    /** Switch job to FEE_JOB_INT_SWAP_CLUSTER_VLD_DONE */
    Fee_JobInfo.Fee_Job = FEE_JOB_INT_SWAP_CLUSTER_VLD_DONE;
    return ret;
}

/**
 * @brief Read data from source cluster to internal Fee buffer
 */
FEE_LOCAL MemIf_JobResultType Fee_JobIntSwapDataRead(void)
{
    MemIf_JobResultType ret;
    uint16              blockId = Fee_JobInfo.Fee_SwapJobInfo.Swap_BlockId;
    uint16 blockSize = Fee_ConfigStd->FeeBlockRef[blockId].FeeBlockSize; /* PRQA S 2824, 2814 */ /* VL_Fee_2814 */

    /** Read the block data */ /* PRQA S 2844 ++ */ /* VL_Fee_2844 */
    if (((Std_ReturnType)E_OK) != Fls_Read(Fee_BlockInfo[blockId].Fee_DataAddr, Fee_DataBuffer, blockSize))
    /* PRQA S 2844 -- */
    {
        /** Fls read job hasn't been accepted */
        ret = MEMIF_JOB_FAILED;
    }
    else
    {
        /** Fls read job has been accepted */
        ret = MEMIF_JOB_PENDING;
    }
    /** Write the block data */
    Fee_JobInfo.Fee_Job = FEE_JOB_INT_SWAP_DATA_WRITE;
    return ret;
}

/**
 * @brief Write block header valid flag
 */
FEE_LOCAL MemIf_JobResultType Fee_JobIntSwapBlockVldWrite(void)
{
    MemIf_JobResultType ret;
#if (STD_ON == FEE_PAGE_DIRECT_WRITE_SUPPORT)
    /** Pack valid flag to buffer */
    Fee_PackBlockHdrValidOrInvalid(Fee_DataBuffer, FEE_VALID_VALUE);
    /** Write validation pattern to flash */
    if (((Std_ReturnType)E_OK)
        != Fls_Write(
            Fee_JobInfo.Fee_SwapJobInfo.Swap_HdrAddr - FEE_BLOCK_HEADER_SIZE,
            Fee_DataBuffer,
            FEE_VIRTUALPAGE_SIZE))
#else
    /** Pack valid flag to buffer */
    Fee_PackHdrValidOrInvalid(Fee_DataBuffer, FEE_VALID_VALUE);
    /** Write validation pattern to flash */ /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    if (((Std_ReturnType)E_OK)
        != Fls_Write(
            Fee_JobInfo.Fee_SwapJobInfo.Swap_HdrAddr - (2u * FEE_VIRTUALPAGE_SIZE),
            Fee_DataBuffer,
            FEE_VIRTUALPAGE_SIZE))
    /* PRQA S 3120 -- */
#endif
    {
        /** Fls write job hasn't been accepted */
        ret = MEMIF_JOB_FAILED;
    }
    else
    {
        /** Fls write job has been accepted */
        ret = MEMIF_JOB_PENDING;
    }
    /** Move on to next block */
    Fee_JobInfo.Fee_SwapJobInfo.Swap_BlockId += 1u;
    Fee_JobInfo.Fee_Job = FEE_JOB_INT_SWAP_BLOCK;
    return ret;
}

/**
 * @brief Write data from internal Fee buffer to target cluster
 */
FEE_LOCAL MemIf_JobResultType Fee_JobIntSwapDataWrite(void)
{
    MemIf_JobResultType ret;
    uint16              blockId = Fee_JobInfo.Fee_SwapJobInfo.Swap_BlockId;
    /* PRQA S 2824, 2814 ++ */ /* VL_Fee_2824, VL_Fee_2814 */
    uint16 blockSize = Fee_ConfigStd->FeeBlockRef[blockId].FeeBlockSize;
    /* PRQA S 2824, 2814 -- */

    if (((Std_ReturnType)E_OK) == Fls_Write(Fee_JobInfo.Fee_SwapJobInfo.Swap_DataAddr, Fee_DataBuffer, blockSize))
    {
        /** Fls write job has been accepted */
        ret = MEMIF_JOB_PENDING;
    }
    else
    {
        /** Fls write job hasn't been accepted */
        ret = MEMIF_JOB_FAILED;
    }
    Fee_JobInfo.Fee_Job = FEE_JOB_INT_SWAP_BLOCK_VLD;
    return ret;
}

/**
 * @brief Write Fee block data to flash
 */
FEE_LOCAL MemIf_JobResultType Fee_JobWriteData(void)
{
    MemIf_JobResultType ret;

    /* PRQA S 2824, 2814 ++ */ /* VL_Fee_2814 */
    uint16 blockSize     = Fee_ConfigStd->FeeBlockRef[Fee_JobInfo.Fee_BlockId].FeeBlockSize;
    uint8  clusterGrpInx = Fee_ConfigStd->FeeBlockRef[Fee_JobInfo.Fee_BlockId].ClusterGrp;
    /* PRQA S 2824, 2814 -- */
    if (FALSE == Fee_ConfigStd->FeeBlockRef[Fee_JobInfo.Fee_BlockId].FeeImmediateData)
    {
        /** Header has been written so update both header and data addresses of the cluster */
        Fee_ClusterGrpInfo[clusterGrpInx].Fee_BlockDataAddr -= blockSize;
        Fee_ClusterGrpInfo[clusterGrpInx].Fee_BlockHdrAddr += FEE_BLOCK_HEADER_SIZE; /* PRQA S 1252 */ /* VL_QAC_1252 */
        /** The block header has written,but the block data not write */ /* PRQA S 2844 ++ */          /* VL_Fee_2844 */
        Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_BlockStatus = FEE_BLOCK_INCONSISTENT;
        Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_DataAddr    = Fee_ClusterGrpInfo[clusterGrpInx].Fee_BlockDataAddr;
#if (STD_ON == FEE_PAGE_DIRECT_WRITE_SUPPORT)
        Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_HdrAddr =
            Fee_ClusterGrpInfo[clusterGrpInx].Fee_BlockHdrAddr - FEE_BLOCK_HEADER_SIZE;
#else
        /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
        Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_ValidHdrAddr =
            (Fee_ClusterGrpInfo[clusterGrpInx].Fee_BlockHdrAddr - (2u * FEE_VIRTUALPAGE_SIZE));
        /* PRQA S 3120 -- */
#endif
    }
    /** Write block data */
    if (((Std_ReturnType)E_OK)
        == Fls_Write(
            Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_DataAddr,
            Fee_JobInfo.Fee_JobWriteDataDestPtr,
            blockSize))
    /* PRQA S 2844 -- */
    {
        /** Fls write job has been accepted */
        ret = MEMIF_JOB_PENDING;
    }
    else
    {
        /** Fls write job hasn't been accepted */
        ret = MEMIF_JOB_FAILED;
    }
    Fee_JobInfo.Fee_Job = FEE_JOB_WRITE_VALIDATE;
    return ret;
}

/**
 * @brief Validate Fee block by writing validation flag to flash
 */
FEE_LOCAL MemIf_JobResultType Fee_JobWriteValidate(void)
{
    MemIf_JobResultType ret;
#if (STD_ON == FEE_PAGE_DIRECT_WRITE_SUPPORT)
    /** Set valid flag to buffer */
    Fee_PackBlockHdrValidOrInvalid(Fee_DataBuffer, FEE_VALID_VALUE);
    /** Write validation pattern to flash */
    if (((Std_ReturnType)E_OK)
        == Fls_Write(Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_HdrAddr, Fee_DataBuffer, FEE_VIRTUALPAGE_SIZE))
#else
    /** Set valid flag to buffer */
    Fee_PackHdrValidOrInvalid(Fee_DataBuffer, FEE_VALID_VALUE);
    /** Write validation pattern to flash */ /* PRQA S 2844 ++ */ /* VL_Fee_2844 */
    if (((Std_ReturnType)E_OK)
        == Fls_Write(Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_ValidHdrAddr, Fee_DataBuffer, FEE_VIRTUALPAGE_SIZE))
    /* PRQA S 2844 -- */
#endif
    {
        /** Fls write job has been accepted */
        ret = MEMIF_JOB_PENDING;
    }
    else
    {
        /** Fls write job hasn't been accepted */
        ret = MEMIF_JOB_FAILED;
    }
    /** Switch the job to FEE_JOB_WRITE_DONE */
    Fee_JobInfo.Fee_Job = FEE_JOB_WRITE_DONE;
    return ret;
}

/**
 * @brief Write block done
 */
FEE_LOCAL MemIf_JobResultType Fee_JobWriteDone(void)
{
    /** Mark the Fee block as valid */
    Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_BlockStatus = FEE_BLOCK_VALID; /* PRQA S 2844 */ /* VL_Fee_2844 */
    /** No more Fls jobs to schedule */
    Fee_JobInfo.Fee_Job = FEE_JOB_NONE;
    return MEMIF_JOB_OK;
}

/**
 * @brief cluster header get(success/failed) handle
 */
FEE_LOCAL MemIf_JobResultType Fee_JobIntScanClusterHdrGet(const boolean BufferValid)
{
    MemIf_JobResultType ret;
    if (TRUE == BufferValid)
    {
        uint32          clusterUseID;
        Fls_AddressType clusterStartAddr;
        Fls_LengthType  clusterLength;

        /** analysis cluster header information */
        Fee_ClusterStatusType clusterStatus =
            Fee_UnpackClusterHdr(&clusterUseID, &clusterStartAddr, &clusterLength, Fee_DataBuffer);
        if ((FEE_CLUSTER_VALID == clusterStatus)
            && (clusterStartAddr
                == Fee_ClusterGroupConfig[Fee_ClusterGroupIdx].ClusterPtr[Fee_ClusterIdx].FeeClusterStartAddr)
            && (clusterLength
                == Fee_ClusterGroupConfig[Fee_ClusterGroupIdx].ClusterPtr[Fee_ClusterIdx].FeeClusterLength)
            && (clusterUseID > Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_ClusterUseID))
        {
            /** Found valid and active cluster*/
            Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_ClusterUseID   = clusterUseID;
            Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_ValidClusterId = Fee_ClusterIdx;
        }
    }
    Fee_ClusterIdx++;
    if (Fee_ClusterIdx == Fee_ClusterGroupConfig[Fee_ClusterGroupIdx].ClusterCount)
    {
        /* Move on to next cluster group */
        Fee_ClusterGroupIdx++;
        Fee_ClusterIdx = 0u;
    }

    boolean allGrpScanDone;
    if (FEE_MAX_CLUSTER_GROUP_SIZE == Fee_ClusterGroupIdx)
    {
        /* Done scanning all cluster headers */
        Fee_ClusterGroupIdx = 0u;
        allGrpScanDone      = TRUE;
    }
    else
    {
        /* There are still clusters to scan */
        allGrpScanDone = FALSE;
    }

    if (allGrpScanDone == TRUE)
    {
        /** scan all cluster done,Determine the existence of a valid cluster */
        ret = Fee_JobIntScanClusterDone();
    }
    else
    {
        /** read the next cluster header */
        ret = Fee_JobIntScanClusterHdrRead();
    }
    return ret;
}

/**
 * @brief Scan all cluster done
 */
FEE_LOCAL MemIf_JobResultType Fee_JobIntScanClusterDone(void)
{
    MemIf_JobResultType ret;
    if (FEE_MAX_CLUSTER_GROUP_SIZE == Fee_ClusterGroupIdx)
    {
        /* No more cluster groups to scan */
        Fee_JobInfo.Fee_Job = FEE_JOB_NONE;
        ret                 = MEMIF_JOB_OK;
    }
    else
    {
        if (0uL == Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_ClusterUseID)
        {
            Fee_ClusterIdx = 0u;
            /** No active cluster found,erase the first cluster */
            ret = Fee_JobIntScanClusterErase();
        }
        else
        {
            /** Active cluster found */
            uint8           ClusterIndex = Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_ValidClusterId;
            Fls_AddressType clusterStartAddr =
                Fee_ClusterGroupConfig[Fee_ClusterGroupIdx].ClusterPtr[ClusterIndex].FeeClusterStartAddr;
            Fls_LengthType clusterLength =
                Fee_ClusterGroupConfig[Fee_ClusterGroupIdx].ClusterPtr[ClusterIndex].FeeClusterLength;
            Fee_BlockHeaderAddr                                      = clusterStartAddr + FEE_CLUSTER_HEADER_SIZE;
            Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_BlockHdrAddr = Fee_BlockHeaderAddr;
            /* PRQA S 1252 */ /* VL_QAC_1252 */
            Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_BlockDataAddr = clusterStartAddr + clusterLength;
            /** Active cluster found so read the first block header */
            ret = Fee_JobIntScanBlockHdrRead();
        }
    }
    return ret;
}

/**
 * @brief Read the Fee block header into internal buffer
 */
FEE_LOCAL MemIf_JobResultType Fee_JobIntScanBlockHdrRead(void)
{
    MemIf_JobResultType ret;
    /** Read the block header */
    if (((Std_ReturnType)E_OK)
        != Fls_Read(Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_BlockHdrAddr, Fee_DataBuffer, FEE_BLOCK_HEADER_SIZE))
    {
        /** Fls read job hasn't been accepted */
        ret = MEMIF_JOB_FAILED;
    }
    else
    {
        /** Fls read job has been accepted */
        ret = MEMIF_JOB_PENDING;
    }
    Fee_JobInfo.Fee_Job = FEE_JOB_INT_SCAN_BLOCK_HDR_GET;
    return ret;
}

/**
 * @brief Erase first Fee cluster
 */
FEE_LOCAL MemIf_JobResultType Fee_JobIntScanClusterErase(void)
{
    MemIf_JobResultType ret;
    Fls_AddressType     startAddr =
        Fee_ClusterGroupConfig[Fee_ClusterGroupIdx].ClusterPtr[Fee_ClusterIdx].FeeClusterStartAddr;
    Fls_LengthType length = Fee_ClusterGroupConfig[Fee_ClusterGroupIdx].ClusterPtr[Fee_ClusterIdx].FeeClusterLength;

    if (((Std_ReturnType)E_OK) == Fls_Erase(startAddr, length))
    {
        /** Fls erase job has been accepted */
        ret = MEMIF_JOB_PENDING;
    }
    else
    {
        /** Fls erase job hasn't been accepted */
        ret = MEMIF_JOB_FAILED;
    }
    /** Schedule cluster clear job */
    Fee_JobInfo.Fee_Job = FEE_JOB_INT_SCAN_CLUSTER_CLEAR;
    return ret;
}

/**
 * @brief First cluster erase success,writing cluster header into flash
 */
FEE_LOCAL MemIf_JobResultType Fee_JobIntScanClusterClear(void)
{
    MemIf_JobResultType ret;
    Fls_AddressType     startAddr =
        Fee_ClusterGroupConfig[Fee_ClusterGroupIdx].ClusterPtr[Fee_ClusterIdx].FeeClusterStartAddr;
    Fls_LengthType length = Fee_ClusterGroupConfig[Fee_ClusterGroupIdx].ClusterPtr[Fee_ClusterIdx].FeeClusterLength;

    /** The cluster useID valid from 1...(0:invalid) */
    Fee_PackClusterHdr(1u, startAddr, length, Fee_DataBuffer);
    /** Set the cluster valid */
    Fee_PackHdrValidOrInvalid(&Fee_DataBuffer[FEE_CLUSTER_HEADER_SIZE - FEE_VIRTUALPAGE_SIZE], FEE_VALID_VALUE);
    /** Write the cluster header to flash */
    if (((Std_ReturnType)E_OK) == Fls_Write(startAddr, Fee_DataBuffer, FEE_CLUSTER_HEADER_SIZE))
    {
        /** Fls read job has been accepted */
        ret = MEMIF_JOB_PENDING;
    }
    else
    {
        /** Fls write job hasn't been accepted */
        ret = MEMIF_JOB_FAILED;
    }
    /** Schedule cluster clear done job */
    Fee_JobInfo.Fee_Job = FEE_JOB_INT_SCAN_CLUSTER_CLEAR_DONE;
    return ret;
}

/**
 * @brief Fee cluster header parameters(except valid parameter) to write buffer
 */
FEE_LOCAL void Fee_PackClusterHdr(
    const uint32          ClusterUseID,
    const Fls_AddressType StartAddress,
    const Fls_LengthType  ClusterSize,
    uint8*                BufferPtr)
{
    uint8* bufferPtr = BufferPtr;
    uint32 checkSum  = ClusterUseID + StartAddress + ClusterSize;
    /* PRQA S 2824, 1290, 2743 ++ */ /* VL_Fee_2824, VL_Fee_1290, VL_Fee_2743 */
    FEE_SERIALIZE((ClusterUseID), uint32, (bufferPtr))
    FEE_SERIALIZE((StartAddress), Fls_AddressType, (bufferPtr))
    FEE_SERIALIZE((ClusterSize), Fls_LengthType, (bufferPtr))
    FEE_SERIALIZE((checkSum), uint32, (bufferPtr))
    /* PRQA S 2824, 1290, 2743 -- */

    /** Fill rest of the header with erase default value */
    const uint8* endPtr = &BufferPtr[FEE_CLUSTER_HEADER_SIZE]; /* PRQA S 2824 */ /* VL_Fee_2824 */
    for (; bufferPtr < endPtr; ++bufferPtr) /* PRQA S 2824, 0490 */              /* VL_Fee_2824, VL_Fee_0490 */
    {
        *bufferPtr = FEE_ERASED_VALUE; /* PRQA S 2814 */ /* VL_Fee_2814 */
    }
}

/**
 * @brief Unpack Fee cluster header parameters from read buffer
 */
FEE_LOCAL Fee_ClusterStatusType Fee_UnpackClusterHdr(
    uint32* const          UseID,
    Fls_AddressType* const StartAddress,
    Fls_LengthType* const  ClusterSize,
    const uint8*           BufferPtr)
{
    Fee_ClusterStatusType ret = FEE_CLUSTER_INVALID;
    uint32                checkSum;
    const uint8*          startPtr = BufferPtr;
    /* PRQA S 2824, 2814, 1290, 2743 ++ */ /* VL_Fee_2824, VL_Fee_2814, VL_Fee_1290, VL_Fee_2743 */
    const uint8* endPtr = &BufferPtr[FEE_CLUSTER_HEADER_SIZE - FEE_VIRTUALPAGE_SIZE];
    FEE_DESERIALIZE((startPtr), (*UseID), uint32)
    FEE_DESERIALIZE((startPtr), (*StartAddress), Fls_AddressType)
    FEE_DESERIALIZE((startPtr), (*ClusterSize), Fls_LengthType)
    FEE_DESERIALIZE((startPtr), (checkSum), uint32)
    /* PRQA S 2824, 2814, 1290, 2743 -- */

    if ((*UseID + *StartAddress + *ClusterSize) != checkSum)
    {
        /** FEE_CLUSTER_INVALID */
    }
    else
    {
        ret = Fee_UnpackClusterHdrSub(startPtr, endPtr);
    }
    return ret;
}

/**
 * @brief Check cluster is valid before unpacking Fee cluster header parameters from read buffer
 */
FEE_LOCAL Fee_ClusterStatusType Fee_UnpackClusterHdrSub(const uint8* startPtr, const uint8* endPtr)
{
    Fee_ClusterStatusType ret       = FEE_CLUSTER_INVALID;
    boolean               flagvalid = (boolean)FALSE;

    if (((Std_ReturnType)E_OK) != Fee_ClusterCheck(startPtr, endPtr))
    {
        /** FEE_CLUSTER_INVALID */
    }
    else if (((Std_ReturnType)E_OK) != Fee_UnpackHdrValidOrInvalid(endPtr, FEE_VALID_VALUE, &flagvalid))
    {
        /** FEE_CLUSTER_INVALID */
    }
    else
    {
        if ((boolean)TRUE == flagvalid)
        {
            ret = FEE_CLUSTER_VALID;
        }
    }
    return ret;
}

#if (STD_ON == FEE_PAGE_DIRECT_WRITE_SUPPORT)
/**
 * @brief Block header valid or invalid flag pack
 */
FEE_LOCAL void Fee_PackBlockHdrValidOrInvalid(uint8* TargetPtr, const uint8 Flag)
{
    uint8* startPtr;
    uint8* endPtr = TargetPtr + FEE_VIRTUALPAGE_SIZE;
    for (startPtr = TargetPtr; startPtr < endPtr; ++startPtr)
    {
        *startPtr = FEE_ERASED_VALUE;
    }
    if (FEE_VALID_VALUE == Flag)
    {
        *(startPtr - 2u) = Flag;
    }
    else
    {
        *(startPtr - 1u) = Flag;
    }
}

/**
 * @brief Unpack valid or invalid flag from read buffer
 */
FEE_LOCAL Std_ReturnType
    Fee_UnpackBlockHdrValidOrInvalid(const uint8* const TargetPtr, const uint8 Flag, boolean* FlagValue)
{
    Std_ReturnType ret = (Std_ReturnType)E_OK;
    uint8          unpackFlag;
    if (FEE_VALID_VALUE == Flag)
    {
        unpackFlag = (*(TargetPtr + FEE_VIRTUALPAGE_SIZE - 2u));
        if ((FEE_VALID_VALUE == unpackFlag) || (FEE_ERASED_VALUE == unpackFlag))
        {
            if (FEE_VALID_VALUE == unpackFlag)
            {
                *FlagValue = (boolean)TRUE;
            }
            else
            {
                *FlagValue = (boolean)FALSE;
            }
        }
        else
        {
            ret = (Std_ReturnType)E_NOT_OK;
        }
    }
    else if (FEE_INVALID_VALUE == Flag)
    {
        unpackFlag = (*(TargetPtr + FEE_VIRTUALPAGE_SIZE - 1u));
        if ((FEE_INVALID_VALUE == unpackFlag) || (FEE_ERASED_VALUE == unpackFlag))
        {
            if (FEE_INVALID_VALUE == unpackFlag)
            {
                *FlagValue = (boolean)TRUE;
            }
            else
            {
                *FlagValue = (boolean)FALSE;
            }
        }
        else
        {
            ret = (Std_ReturnType)E_NOT_OK;
        }
    }
    else
    {
        /** Never run to here,except for the interface call error */
        ret = (Std_ReturnType)E_NOT_OK;
    }
    return ret;
}
#endif

/**
 * @brief Pack validation or invalidation flag to write buffer
 */
FEE_LOCAL void Fee_PackHdrValidOrInvalid(uint8* TargetPtr, const uint8 Flag)
{
    uint8*       startPtr = TargetPtr;
    const uint8* endPtr   = &TargetPtr[FEE_VIRTUALPAGE_SIZE]; /* PRQA S 2824 */ /* VL_Fee_2824 */
    *startPtr             = Flag; /* PRQA S 2814 */                             /* VL_Fee_2814 */
    for (startPtr++; startPtr < endPtr; startPtr++) /* PRQA S 2824, 0490 */     /* VL_Fee_2824, VL_Fee_0490 */
    {
        *startPtr = FEE_ERASED_VALUE; /* PRQA S 2814 */ /* VL_Fee_2814 */
    }
}

/**
 * @brief Unpack validation or invalidation flag from read buffer
 */
FEE_LOCAL Std_ReturnType Fee_UnpackHdrValidOrInvalid(const uint8* const TargetPtr, const uint8 Flag, boolean* FlagValue)
{
    Std_ReturnType ret;
    if ((Flag == *TargetPtr) || (FEE_ERASED_VALUE == *TargetPtr)) /* PRQA S 2814 */ /* VL_Fee_2814 */
    {
        if (Flag == *TargetPtr)
        {
            *FlagValue = (boolean)TRUE; /* PRQA S 2814 */ /* VL_Fee_2814 */
        }
        else
        {
            *FlagValue = (boolean)FALSE;
        }
        ret = Fee_ClusterCheck(&TargetPtr[1u], &TargetPtr[FEE_VIRTUALPAGE_SIZE]); /* PRQA S 2824 */ /* VL_Fee_2824 */
    }
    else
    {
        ret = (Std_ReturnType)E_NOT_OK;
    }
    return ret;
}

/**
 * @brief Initial Job finished(all block is invalid,the cluster is new)
 */
FEE_LOCAL MemIf_JobResultType Fee_JobIntScanClusterClearDone(void)
{
    Fls_AddressType startAddr;
    Fls_LengthType  length;
    Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_ValidClusterId = Fee_ClusterIdx;
    Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_ClusterUseID   = 1u;
    /** Initial job finished,the active cluster is new,all block is invalid */
    startAddr = Fee_ClusterGroupConfig[Fee_ClusterGroupIdx].ClusterPtr[Fee_ClusterIdx].FeeClusterStartAddr;
    length    = Fee_ClusterGroupConfig[Fee_ClusterGroupIdx].ClusterPtr[Fee_ClusterIdx].FeeClusterLength;
    Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_BlockHdrAddr  = startAddr + FEE_CLUSTER_HEADER_SIZE;
    Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_BlockDataAddr = startAddr + length;
    Fee_ClusterGroupIdx++;
    MemIf_JobResultType retVal = Fee_JobIntScanClusterDone();
    return retVal;
}

/**
 * @brief Check whether specified data buffer contains only
 */
FEE_LOCAL Std_ReturnType Fee_ClusterCheck(const uint8* StartPtr, const uint8* const EndPtr)
{
    Std_ReturnType ret      = (Std_ReturnType)E_OK;
    const uint8*   startPtr = StartPtr;
    for (; startPtr < EndPtr; ++startPtr) /* PRQA S 2824, 0490 */ /* VL_Fee_2824, VL_Fee_0490 */
    {
        if (FEE_ERASED_VALUE == *startPtr) /* PRQA S 2814 */ /* VL_Fee_2814 */
        {
            /** value match */
        }
        else
        {
            ret = (Std_ReturnType)E_NOT_OK;
            break;
        }
    }
    return ret;
}

/**
 * @brief Initial job:scan the store block, have get the block header information
 */
/* PRQA S 6030 ++ */ /* VL_MTR_Fee_STMIF */
FEE_LOCAL MemIf_JobResultType Fee_JobIntScanBlockHdrGet(const boolean BufferValid)
{
    MemIf_JobResultType ret;
    uint16              blockNumber;
    uint16              blockSize;
    Fls_AddressType     dataAddr;
    boolean             immediateData;

    Fee_BlockStatusType blockStatus = Fee_UnpackBlockHdr(&blockNumber, &blockSize, &dataAddr, &immediateData);
    if ((FEE_BLOCK_HEADER_INVALID == blockStatus) || ((boolean)FALSE == BufferValid))
    {
        /* PRQA S 2844 ++ */ /* VL_Fee_2844 */
        Fls_AddressType clusterEndAddr = Fee_ClusterGroupConfig[Fee_ClusterGroupIdx]
                                             .ClusterPtr[Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_ValidClusterId]
                                             .FeeClusterStartAddr
                                         + Fee_ClusterGroupConfig[Fee_ClusterGroupIdx]
                                               .ClusterPtr[Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_ValidClusterId]
                                               .FeeClusterLength;
        /* PRQA S 2844 -- */
        /** Avoid out-of-bounds access due to Header errors or read failures */
        if ((Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_BlockHdrAddr + FEE_BLOCK_HEADER_SIZE) >= clusterEndAddr)
        {
            Fee_JobInfo.Fee_Job = FEE_JOB_NONE;
            ret                 = MEMIF_JOB_OK;
        }
        else
        {
            /** Move on to next block header */
            Fee_BlockHeaderAddr += FEE_BLOCK_HEADER_SIZE;
            Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_BlockHdrAddr = Fee_BlockHeaderAddr;
            /** Read next header */
            ret = Fee_JobIntScanBlockHdrRead();
        }
    }
    else if (FEE_BLOCK_HEADER_BLANK == blockStatus)
    {
        Fee_JobInfo.Fee_Job = FEE_JOB_NONE;
        ret                 = MEMIF_JOB_OK;
    }
    else
    {
        /** found block in the block configuration */
        boolean blockUnpackValid = (boolean)FALSE;
        uint16  blockId          = Fee_GetBlockId(blockNumber);
        if (FEE_UNUSED_UINT16 != blockId)
        {
            /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
            if ((dataAddr <= (Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_BlockDataAddr - blockSize))
                && (dataAddr
                    >= (Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_BlockHdrAddr + (2u * FEE_BLOCK_HEADER_SIZE))))
            /* PRQA S 3120 -- */
            {
                /* PRQA S 2824, 2814 ++ */ /* VL_Fee_2814 */
                if ((Fee_ConfigStd->FeeBlockRef[blockId].ClusterGrp == Fee_ClusterGroupIdx)
                    && (Fee_ConfigStd->FeeBlockRef[blockId].FeeBlockSize == blockSize)
                    && (Fee_ConfigStd->FeeBlockRef[blockId].FeeImmediateData == immediateData))
                /* PRQA S 2824, 2814 -- */
                {
                    blockUnpackValid = (boolean)TRUE;
                }
            }
        }
        /** Block information unpack success */ /* PRQA S 2844 ++ */ /* VL_Fee_2844 */
        if ((boolean)TRUE == blockUnpackValid)
        {
            Fee_BlockInfo[blockId].Fee_BlockStatus = blockStatus;
            Fee_BlockInfo[blockId].Fee_DataAddr    = dataAddr;
#if (STD_ON == FEE_PAGE_DIRECT_WRITE_SUPPORT)
            Fee_BlockInfo[blockId].Fee_HdrAddr = Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_BlockHdrAddr;
#else
            /* PRQA S 3120,1252 ++ */ /* VL_QAC_MagicNum,VL_QAC_1252 */
            Fee_BlockInfo[blockId].Fee_ValidHdrAddr =
                ((Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_BlockHdrAddr + FEE_BLOCK_HEADER_SIZE)
                 - (2u * FEE_VIRTUALPAGE_SIZE));
            /* PRQA S 3120,1252 -- */
#endif
            Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_BlockDataAddr = dataAddr;
        }
        /* PRQA S 2844 -- */
        /** Move on to next block header */
        Fee_BlockHeaderAddr += FEE_BLOCK_HEADER_SIZE;
        Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_BlockHdrAddr = Fee_BlockHeaderAddr;
        /** Read next header */
        ret = Fee_JobIntScanBlockHdrRead();
    }
    return ret;
}
/* PRQA S 6030 -- */

/**
 * @brief The address of the stored header needs to be aligned based on BlockNumber, Length, TargetAddress
 */
FEE_LOCAL uint8 Fee_PackBlockHdrSubOne(
    const uint16          BlockNumber,
    const uint16          Length,
    const Fls_AddressType TargetAddress,
    uint8*                BlockHdrPtr)
{
    uint8* startPtr = BlockHdrPtr;
    /* PRQA S 2824, 0488, 1290, 2743 ++ */ /* VL_Fee_2824, VL_Fee_0488, VL_Fee_1290, VL_Fee_2743 */
    FEE_SERIALIZE((BlockNumber), uint16, (startPtr))
    FEE_SERIALIZE((Length), uint16, (startPtr))
    FEE_SERIALIZE((TargetAddress), Fls_AddressType, (startPtr))
    return (uint8)(startPtr - BlockHdrPtr);
    /* PRQA S 2824, 0488, 1290, 2743  -- */
}

/**
 * @brief Check if it is an immediate number, set the checksum, and then set the erase value based on the aligned
 * address
 */
FEE_LOCAL void
    Fee_PackBlockHdrSubTwo(uint32* checkSum, const boolean ImmediateBlock, uint8* BlockHdrPtr, const uint8 offset)
{
    uint8* startPtr = &BlockHdrPtr[offset]; /* PRQA S 2824, 0488 */ /* VL_Fee_2824, VL_Fee_0488 */

    /** Use MSB of the checksum for the immediate block flag so the checksum is 31-bit */
    if (TRUE == ImmediateBlock)
    {
        *checkSum += 1uL; /* PRQA S 2814 */          /* VL_Fee_2814 */
        *checkSum |= 0x80000000uL; /* PRQA S 3120 */ /* VL_QAC_MagicNum */
    }
    else
    {
        *checkSum &= 0x7fffffffuL; /* PRQA S 3120 */ /* VL_QAC_MagicNum */
    }
    /* PRQA S 2824, 2908, 1290, 2743 ++ */ /* VL_Fee_2824, VL_Fee_2908, VL_Fee_1290,VL_Fee_2743 */
    FEE_SERIALIZE((*checkSum), uint32, (startPtr))
    /* PRQA S 2824, 2908, 1290, 2743 -- */

    const uint8* endPtr = &BlockHdrPtr[FEE_BLOCK_HEADER_SIZE]; /* PRQA S 2824 */ /* VL_Fee_2824 */
    for (; startPtr < endPtr; ++startPtr) /* PRQA S 2824, 0490 */                /* VL_Fee_2824, VL_Fee_0490 */
    {
        *startPtr = FEE_ERASED_VALUE; /* PRQA S 2814 */ /* VL_Fee_2814 */
    }
}

/**
 * @brief Unpack the block header information
 */
FEE_LOCAL Fee_BlockStatusType Fee_UnpackBlockHdr(
    uint16* const          BlockNumber,
    uint16* const          Length,
    Fls_AddressType* const TargetAddress,
    boolean* const         ImmediateBlock)
{
    Fee_BlockStatusType ret         = FEE_BLOCK_HEADER_INVALID;
    const uint8*        blockHdrPtr = Fee_DataBuffer;

    /** check the block header is blank or not */
    if (Fee_ClusterCheck(blockHdrPtr, &blockHdrPtr[FEE_BLOCK_HEADER_SIZE]) == ((Std_ReturnType)E_OK))
    {
        ret = FEE_BLOCK_HEADER_BLANK;
    }
    else
    {
        ret = Fee_UnpackBlockHdrSubOne(BlockNumber, Length, TargetAddress, ImmediateBlock);
    }
    return ret;
}

/**
 * @brief Align the address and set the checksum based on the immediate number before unpacking the block header
 * information
 */
FEE_LOCAL Fee_BlockStatusType Fee_UnpackBlockHdrSubOne(
    uint16* const          BlockNumber,
    uint16* const          Length,
    Fls_AddressType* const TargetAddress,
    boolean* const         ImmediateBlock)
{
    Fee_BlockStatusType ret         = FEE_BLOCK_HEADER_INVALID;
    const uint8*        blockHdrPtr = Fee_DataBuffer;
    uint32              readCheckSum;
    /* PRQA S 2814, 1290, 2743 ++ */ /* VL_Fee_2814, VL_Fee_1290, VL_Fee_2743 */
    FEE_DESERIALIZE((blockHdrPtr), (*BlockNumber), uint16)
    FEE_DESERIALIZE((blockHdrPtr), (*Length), uint16)
    FEE_DESERIALIZE((blockHdrPtr), (*TargetAddress), Fls_AddressType)
    FEE_DESERIALIZE((blockHdrPtr), (readCheckSum), uint32)
    /* PRQA S 2814, 1290, 2743 -- */
    /** Use MSB of checksum for immediate block flag so the checksum is 31-bit long */
    uint32 calcCheckSum = (uint32)(*Length) + (uint32)(*TargetAddress) + (uint32)(*BlockNumber);
    if (0uL == (readCheckSum & 0x80000000uL)) /* PRQA S 3120 */ /* VL_QAC_MagicNum */
    {
        *ImmediateBlock = (boolean)FALSE; /* PRQA S 2814 */ /* VL_Fee_2814 */
    }
    else
    {
        *ImmediateBlock = (boolean)TRUE;
        calcCheckSum += 1uL;
    }

    ret = Fee_UnpackBlockHdrSubTwo(readCheckSum, calcCheckSum, blockHdrPtr);

    return ret;
}

/**
 * @brief Check the block is valid before unpacking the block header information
 */
/* PRQA S 6030 ++ */ /* VL_MTR_Fee_STMIF */
FEE_LOCAL Fee_BlockStatusType
    Fee_UnpackBlockHdrSubTwo(const uint32 readCheckSum, const uint32 calcCheckSum, const uint8* blockHdrPtr)
{
    Fee_BlockStatusType ret         = FEE_BLOCK_HEADER_INVALID;
    boolean             flagValid   = (boolean)FALSE;
    boolean             flagInvalid = (boolean)FALSE;

#if (STD_ON == FEE_PAGE_DIRECT_WRITE_SUPPORT)
    const uint8* BlockHdrPtr = Fee_DataBuffer;
    const uint8* endPtr      = &Fee_DataBuffer[FEE_BLOCK_HEADER_SIZE - 2u];
#else
    const uint8* endPtr = &Fee_DataBuffer[FEE_BLOCK_HEADER_SIZE - (2u * FEE_VIRTUALPAGE_SIZE)];
#endif
    /** Ignore MSB since it's used for ImmediateBlock flag */
    if ((readCheckSum & 0x7fffffffuL) != (calcCheckSum & 0x7fffffffuL)) /* PRQA S 3120 */ /* VL_QAC_MagicNum */
    {
        /** FEE_BLOCK_HEADER_INVALID */
    }
#if (STD_ON == FEE_PAGE_DIRECT_WRITE_SUPPORT)
    else if (((Std_ReturnType)E_OK) != Fee_ClusterCheck(blockHdrPtr, endPtr))
    {
        /** FEE_BLOCK_HEADER_INVALID */
    }
    else if (((Std_ReturnType)E_OK) != Fee_UnpackBlockHdrValidOrInvalid(BlockHdrPtr, FEE_VALID_VALUE, &flagValid))
    {
        /** FEE_BLOCK_HEADER_INVALID */
    }
    else if (((Std_ReturnType)E_OK) != Fee_UnpackBlockHdrValidOrInvalid(BlockHdrPtr, FEE_INVALID_VALUE, &flagInvalid))
    {
        /** FEE_BLOCK_HEADER_INVALID */
    }
#else
    else if (((Std_ReturnType)E_OK) != Fee_ClusterCheck(blockHdrPtr, endPtr))
    {
        /** FEE_BLOCK_HEADER_INVALID */
    }
    else if (((Std_ReturnType)E_OK) != Fee_UnpackHdrValidOrInvalid(endPtr, FEE_VALID_VALUE, &flagValid))
    {
        /** FEE_BLOCK_HEADER_INVALID */
    }
    else if (
        ((Std_ReturnType)E_OK)
        != Fee_UnpackHdrValidOrInvalid(&endPtr[FEE_VIRTUALPAGE_SIZE], FEE_INVALID_VALUE, &flagInvalid))
    {
        /** FEE_BLOCK_HEADER_INVALID */
    }
#endif
    else
    {
        if (((boolean)TRUE == flagValid) && ((boolean)FALSE == flagInvalid))
        {
            ret = FEE_BLOCK_VALID;
        }
        else
        {
            ret = FEE_BLOCK_INVALID;
        }
    }

    return ret;
}
/* PRQA S 6030 -- */

/**
 * @brief Call the corresponding job processing interface based on the requested job, then set the status and invoke the
 * callback interface based on the job result
 */
/* PRQA S 3223 ++ */ /* VL_Fee_3223 */
FEE_LOCAL void Fee_MainFunctionSub(void)
{
    FEE_LOCAL const Fee_VoidFuncVoidPtr Fee_JobVoidFuncMatrix[] = {
        &Fee_JobInitScan,       /** FEE_JOB_INT_SCAN */
        &Fee_JobRead,           /** FEE_JOB_READ */
        &Fee_JobWrite,          /** FEE_JOB_WRITE */
        &Fee_JobInvalBlock,     /** FEE_JOB_INVAL_BLOCK */
        &Fee_JobEraseImmediate, /** FEE_JOB_ERASE_IMMEDIATE */
    };

    if (Fee_JobInfo.Fee_Job < FEE_JOB_WRITE_DATA)
    {
        (*Fee_JobVoidFuncMatrix[Fee_JobInfo.Fee_Job])(); /* PRQA S 2814, 2844 */ /* VL_Fee_2814, VL_Fee_2844 */
    }

    if (MEMIF_JOB_PENDING == Fee_JobInfo.Fee_JobResult)
    {
        /** Nothing to do */
    }
    else if (MEMIF_JOB_OK == Fee_JobInfo.Fee_JobResult)
    {
        if (MEMIF_BUSY_INTERNAL == Fee_ModuleStatus)
        {
            /** no notifications from internal jobs */
            Fee_ModuleStatus = MEMIF_IDLE;
        }
        else
        {
            Fee_ModuleStatus = MEMIF_IDLE;
            /** Call job end notification function */
            FEE_NVMJOBEND_NOTIFICATION
        }
    }
    else
    {
        if (MEMIF_BUSY_INTERNAL == Fee_ModuleStatus)
        {
            /** Call initial job error notification function */
            Fee_ModuleStatus = MEMIF_UNINIT;
            FEE_INITERROR_NOTIFICATION
        }
        else
        {
            Fee_ModuleStatus = MEMIF_IDLE;
            /** Call job error notification function */
            FEE_NVMJOBERROR_NOTIFICATION
        }
    }
}
/* PRQA S 3223 -- */
#define FEE_STOP_SEC_CODE
#include "Fee_MemMap.h"
