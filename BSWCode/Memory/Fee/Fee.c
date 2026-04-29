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
**  FILENAME    : Fee.c                                                       **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation for FEE                                      **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19_11                      **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Fee.h"
#include "Fee_InternalTypes.h"
#include "Fee_Cbk.h"
#include "Det.h"
/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/
#define FEE_C_AR_MAJOR_VERSION 4u
#define FEE_C_AR_MINOR_VERSION 5u
#define FEE_C_AR_PATCH_VERSION 0u
#define FEE_C_SW_MAJOR_VERSION 2u
#define FEE_C_SW_MINOR_VERSION 0u
#define FEE_C_SW_PATCH_VERSION 4u
/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#if (FEE_C_AR_MAJOR_VERSION != FEE_H_AR_MAJOR_VERSION)
#error "Fee.c : Mismatch in Specification Major Version"
#endif
#if (FEE_C_AR_MINOR_VERSION != FEE_H_AR_MINOR_VERSION)
#error "Fee.c : Mismatch in Specification Major Version"
#endif
#if (FEE_C_AR_PATCH_VERSION != FEE_H_AR_PATCH_VERSION)
#error "Fee.c : Mismatch in Specification Major Version"
#endif
#if (FEE_C_SW_MAJOR_VERSION != FEE_H_SW_MAJOR_VERSION)
#error "Fee.c : Mismatch in Specification Major Version"
#endif
#if (FEE_C_SW_MINOR_VERSION != FEE_H_SW_MINOR_VERSION)
#error "Fee.c : Mismatch in Specification Major Version"
#endif
#if (FEE_C_SW_PATCH_VERSION != FEE_H_SW_PATCH_VERSION)
#error "Fee.c : Mismatch in Specification Major Version"
#endif
/*******************************************************************************
**                            Local Macros                                    **
*******************************************************************************/
#if (CPU_32_WITH_16_ADR == TRUE)
#define FEE_SERIALIZE(ParamVal, ParamType, pSerialPtr)       \
    do                                                       \
    {                                                        \
        for (uint8 i = 0; i < (sizeof(ParamType) * 2u); i++) \
        {                                                    \
            pSerialPtr[i] = (uint8)(ParamVal >> (i * 8u));   \
        }                                                    \
        pSerialPtr = &pSerialPtr[sizeof(ParamType) * 2u];    \
    } while (0);

#define FEE_DESERIALIZE(pDeserialPtr, ParamVal, ParamType)                             \
    do                                                                                 \
    {                                                                                  \
        ParamVal = 0;                                                                  \
        for (uint8 i = 0; i < (sizeof(ParamType) * 2u); i++)                           \
        {                                                                              \
            ParamVal = ParamVal | (ParamType)((ParamType)pDeserialPtr[i] << (i * 8u)); \
        }                                                                              \
        pDeserialPtr = &pDeserialPtr[sizeof(ParamType) * 2u];                          \
    } while (0);
#else
#define FEE_SERIALIZE(ParamVal, ParamType, pSerialPtr)     \
    do                                                     \
    {                                                      \
        for (uint8 i = 0; i < sizeof(ParamType); i++)      \
        {                                                  \
            pSerialPtr[i] = (uint8)(ParamVal >> (i * 8u)); \
        }                                                  \
        pSerialPtr = &pSerialPtr[sizeof(ParamType)];       \
    } while (0);

#define FEE_DESERIALIZE(pDeserialPtr, ParamVal, ParamType)                             \
    do                                                                                 \
    {                                                                                  \
        ParamVal = 0;                                                                  \
        for (uint8 i = 0; i < sizeof(ParamType); i++)                                  \
        {                                                                              \
            ParamVal = ParamVal | (ParamType)((ParamType)pDeserialPtr[i] << (i * 8u)); \
        }                                                                              \
        pDeserialPtr = &pDeserialPtr[sizeof(ParamType)];                               \
    } while (0);
#endif

#ifndef FEE_LOCAL
#define FEE_LOCAL static
#endif /* FEE_LOCAL */

#if (FEE_DEV_ERROR_DETECT == STD_ON)
#define FEE_DET_REPORT_ERR(api, error) (void)Det_ReportError(FEE_MODULE_ID, FEE_INSTANCE_ID, (api), (error));
#endif /* FEE_DEV_ERROR_DETECT == STD_ON */

#define FEE_DET_REPORT_RUNTIME_ERR(api, error) \
    (void)Det_ReportRuntimeError(FEE_MODULE_ID, FEE_INSTANCE_ID, (api), (error));
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
#define FEE_START_SEC_VAR_INIT_PTR
#include "Fee_MemMap.h"
FEE_LOCAL P2CONST(Fee_ConfigType, FEE_VAR, FEE_CONST) Fee_ConfigStd = NULL_PTR;
#define FEE_STOP_SEC_VAR_INIT_PTR
#include "Fee_MemMap.h"

#define FEE_START_SEC_VAR_INIT_8
#include "Fee_MemMap.h"
FEE_LOCAL VAR(MemIf_StatusType, FEE_VAR) Fee_ModuleStatus = MEMIF_UNINIT;
#define FEE_STOP_SEC_VAR_INIT_8
#include "Fee_MemMap.h"

#define FEE_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Fee_MemMap.h"
FEE_LOCAL VAR(Fee_BlockManageType, FEE_VAR) Fee_BlockInfo[FEE_BLOCK_MAX];
#define FEE_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Fee_MemMap.h"

#define FEE_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Fee_MemMap.h"
FEE_LOCAL VAR(Fee_ClusterManageType, FEE_VAR) Fee_ClusterGrpInfo[FEE_CLUSTER_GROUP_NUM];
#define FEE_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Fee_MemMap.h"

#define FEE_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Fee_MemMap.h"
FEE_LOCAL VAR(Fee_JobManageType, FEE_VAR) Fee_JobInfo;
#define FEE_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Fee_MemMap.h"

#define FEE_START_SEC_VAR_NO_INIT_8
#include "Fee_MemMap.h"
FEE_LOCAL VAR(uint8, FEE_VAR) Fee_ClusterGroupIdx;
FEE_LOCAL VAR(uint8, FEE_VAR) Fee_ClusterIdx;
#define FEE_STOP_SEC_VAR_NO_INIT_8
#include "Fee_MemMap.h"

#define FEE_START_SEC_VAR_INIT_32
#include "Fee_MemMap.h"
FEE_LOCAL VAR(uint8, FEE_VAR) Fee_DataBuffer[FEE_BUFFER_SIZE];
FEE_LOCAL VAR(Fls_AddressType, FEE_VAR) Fee_BlockHeaderAddr;
#define FEE_STOP_SEC_VAR_INIT_32
#include "Fee_MemMap.h"
/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define FEE_START_SEC_CODE
#include "Fee_MemMap.h"
/*Flash job success end handle*/
FEE_LOCAL FUNC(void, FEE_CODE) Fee_FlsJobEndHandle(void);
/*Flash job error handle*/
FEE_LOCAL FUNC(void, FEE_CODE) Fee_FlsJobErrorHandle(void);
/*find the local block id of the BlockNumber*/
FEE_LOCAL FUNC(uint16, FEE_CODE) Fee_GetBlockId(const uint16 BlockNumber);
/*read the cluster header infomation*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobIntScanClusterHdrRead(void);
/*initial job handle,try to read cluster header*/
FEE_LOCAL FUNC(void, FEE_CODE) Fee_JobInitScan(void);
/* All internal jobs handle*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobSchedule(void);
/*Read Fee block*/
FEE_LOCAL FUNC(void, FEE_CODE) Fee_JobRead(void);
/*Write Fee block to flash*/
FEE_LOCAL FUNC(void, FEE_CODE) Fee_JobWrite(void);
/*Invalidate Fee block by writing the invalid flag to flash*/
FEE_LOCAL FUNC(void, FEE_CODE) Fee_JobInvalBlock(void);
/*Erase (pre-allocate) immediate Fee block*/
FEE_LOCAL FUNC(void, FEE_CODE) Fee_JobEraseImmediate(void);
/*invalidation of Fee block done*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobInvalBlockDone(void);
/*Erase immediate Fee block done*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobEraseImmediateDone(void);
/*Write Fee block header to flash*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobWriteHdr(void);
/*Initialize the cluster swap internal operation,
 *This can only happen during the block header writing phase*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobIntSwap(void);
/*Erase the new cluster for swap*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobIntSwapClusterErase(void);
/*Clear current Fee cluster done,writing cluster header into flash*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobIntSwapClusterClear(void);
/* Copy block from source to target cluster */
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobIntSwapBlock(void);
/*Success write cluster valid flag,update all copied block information*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobIntSwapClusterVldDone(void);
/*Validate current cluster by writing FEE_VALID_VALUE into flash*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobIntSwapClusterVld(void);
/*Read data from source cluster to internal Fee buffer*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobIntSwapDataRead(void);
/*Write block header valid flag*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobIntSwapBlockVldWrite(void);
/*Write data from internal Fee buffer to target cluster*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobIntSwapDataWrite(void);
/*Write Fee block data to flash*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobWriteData(void);
/*Validate Fee block by writing validation flag to flash*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobWriteValidate(void);
/*Write block done*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobWriteDone(void);
/*cluster header get(success/failed) handle*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobIntScanClusterHdrGet(CONST(boolean, AUTOMATIC) BufferValid);
/*scan all cluster done*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobIntScanClusterDone(void);
/*Read the Fee block header into internal buffer*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobIntScanBlockHdrRead(void);
/*Erase first Fee cluster*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobIntScanClusterErase(void);
/*First cluster erase success,writing cluster header into flash*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobIntScanClusterClear(void);
/*Fee cluster header parameters(except valid parameter) to write buffer*/
FEE_LOCAL FUNC(void, FEE_CODE) Fee_PackClusterHdr(
    CONST(uint32, AUTOMATIC) ClusterUseID,
    CONST(Fls_AddressType, AUTOMATIC) StartAddress,
    CONST(Fls_LengthType, AUTOMATIC) ClusterSize,
    P2VAR(uint8, AUTOMATIC, FEE_APPL_DATA) BufferPtr);
/*Unpack Fee cluster header parameters from read buffer*/
FEE_LOCAL FUNC(Fee_ClusterStatusType, FEE_CODE) Fee_UnpackClusterHdr(
    CONSTP2VAR(uint32, AUTOMATIC, FEE_APPL_DATA) UseID,
    CONSTP2VAR(Fls_AddressType, AUTOMATIC, FEE_APPL_DATA) StartAddress,
    CONSTP2VAR(Fls_LengthType, AUTOMATIC, FEE_APPL_DATA) ClusterSize,
    P2CONST(uint8, AUTOMATIC, FEE_APPL_CONST) BufferPtr);
#if (STD_ON == FEE_PAGE_DIRECT_WRITE_SUPPORT)
/*Block header valid or invalid flag pack*/
FEE_LOCAL FUNC(void, FEE_CODE)
    Fee_PackBlockHdrValidOrInvalid(P2VAR(uint8, AUTOMATIC, FEE_APPL_DATA) TargetPtr, CONST(uint8, AUTOMATIC) Flag);
/*Unpack valid or invalid flag from read buffer*/
FEE_LOCAL FUNC(Std_ReturnType, FEE_CODE) Fee_UnpackBlockHdrValidOrInvalid(
    CONSTP2CONST(uint8, AUTOMATIC, FEE_APPL_CONST) TargetPtr,
    CONST(uint8, AUTOMATIC) Flag,
    P2VAR(boolean, AUTOMATIC, FEE_APPL_DATA) FlagValue);
#endif /*STD_ON == FEE_PAGE_DIRECT_WRITE_SUPPORT*/
/*Pack validation or invalidation flag to write buffer*/
FEE_LOCAL FUNC(void, FEE_CODE)
    Fee_PackHdrValidOrInvalid(P2VAR(uint8, AUTOMATIC, FEE_APPL_DATA) TargetPtr, CONST(uint8, AUTOMATIC) Flag);
/*Unpack validation or invalidation flag from read buffer*/
FEE_LOCAL FUNC(Std_ReturnType, FEE_CODE) Fee_UnpackHdrValidOrInvalid(
    CONSTP2CONST(uint8, AUTOMATIC, FEE_APPL_CONST) TargetPtr,
    CONST(uint8, AUTOMATIC) Flag,
    P2VAR(boolean, AUTOMATIC, FEE_APPL_DATA) FlagValue);
/*Initial Job finished(all block is invalid,the cluster is new)*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobIntScanClusterClearDone(void);
/*Check whether specified data buffer contains
 *only the FEE_ERASED_VALUE value*/
FEE_LOCAL FUNC(Std_ReturnType, FEE_CODE) Fee_ClusterCheck(
    P2CONST(uint8, AUTOMATIC, FEE_APPL_CONST) StartPtr,
    CONSTP2CONST(uint8, AUTOMATIC, FEE_APPL_CONST) EndPtr);
/*Initial job:scan the store block, have get the block header information*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobIntScanBlockHdrGet(CONST(boolean, AUTOMATIC) BufferValid);
/*Pack the block header information*/
FEE_LOCAL FUNC(void, FEE_CODE) Fee_PackBlockHdr(
    CONST(uint16, AUTOMATIC) BlockNumber,
    CONST(uint16, AUTOMATIC) Length,
    CONST(Fls_AddressType, AUTOMATIC) TargetAddress,
    CONST(boolean, AUTOMATIC) ImmediateBlock,
    P2VAR(uint8, AUTOMATIC, FEE_APPL_DATA) BlockHdrPtr);
/*Unpack the block header information*/
FEE_LOCAL FUNC(Fee_BlockStatusType, FEE_CODE) Fee_UnpackBlockHdr(
    CONSTP2VAR(uint16, AUTOMATIC, FEE_APPL_DATA) BlockNumber,
    CONSTP2VAR(uint16, AUTOMATIC, FEE_APPL_DATA) Length,
    CONSTP2VAR(Fls_AddressType, AUTOMATIC, FEE_APPL_DATA) TargetAddress,
    CONSTP2VAR(boolean, AUTOMATIC, FEE_APPL_DATA) ImmediateBlock,
    P2CONST(uint8, AUTOMATIC, FEE_APPL_CONST) BlockHdrPtr);
#define FEE_STOP_SEC_CODE
#include "Fee_MemMap.h"
/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define FEE_START_SEC_CODE
#include "Fee_MemMap.h"
/******************************************************************************/
/*
 * Brief               Service to initialize the FEE module
 * ServiceId           0x00
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ConfigPtr: Pointer to the selected configuration set
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, FEE_CODE)
Fee_Init(P2CONST(Fee_ConfigType, AUTOMATIC, FEE_CONST_PBCFG) ConfigPtr)
{
    boolean detNoErr = TRUE;
#if (FEE_DEV_ERROR_DETECT == STD_ON)
    if (NULL_PTR == ConfigPtr)
    {
        FEE_DET_REPORT_ERR(FEE_INIT_ID, FEE_E_PARAM_POINTER)
        detNoErr = FALSE;
    }
#endif /* FEE_DEV_ERROR_DETECT == STD_ON */
    if ((MEMIF_BUSY == Fee_ModuleStatus) || (MEMIF_BUSY_INTERNAL == Fee_ModuleStatus))
    {
        FEE_DET_REPORT_RUNTIME_ERR(FEE_INIT_ID, FEE_E_BUSY)
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
    {
        uint16 cnt;
        Fee_ConfigStd = ConfigPtr;
        /* Initialize all block manage information */
        for (cnt = 0u; cnt < Fee_ConfigStd->FeeBlockSum; cnt++)
        {
            Fee_BlockInfo[cnt].Fee_BlockStatus = FEE_BLOCK_INCONSISTENT;
            Fee_BlockInfo[cnt].Fee_DataAddr    = 0u;
#if (STD_ON == FEE_PAGE_DIRECT_WRITE_SUPPORT)
            Fee_BlockInfo[cnt].Fee_HdrAddr = 0u;
#else
            Fee_BlockInfo[cnt].Fee_ValidHdrAddr = 0u;
#endif
        }
        /* Initialize all cluster group information */
        for (uint8 ClusterGrpId = 0u; ClusterGrpId < FEE_CLUSTER_GROUP_NUM; ClusterGrpId++)
        {
            /* set the initial cluster manage information */
            Fee_ClusterGrpInfo[ClusterGrpId].Fee_ValidClusterId = 0u;
            Fee_ClusterGrpInfo[ClusterGrpId].Fee_ClusterUseID   = 0u;
            Fee_ClusterGrpInfo[ClusterGrpId].Fee_BlockHdrAddr   = 0u;
            Fee_ClusterGrpInfo[ClusterGrpId].Fee_BlockDataAddr  = 0u;
        }
        /* Schedule initial job */
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
        /* set the module state */
        Fee_ModuleStatus = MEMIF_BUSY_INTERNAL;
    }
}
/******************************************************************************/
/*
 * Brief               Function to switch the mode of the underlying Flash Driver
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Mode: Desired mode for the underlying flash driver
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, FEE_CODE)
Fee_SetMode(VAR(MemIf_ModeType, AUTOMATIC) Mode)
{
#if (FEE_SET_MODE_SUPPORTED == STD_ON)
    boolean detNoErr = TRUE;
    if (MEMIF_UNINIT == Fee_ModuleStatus)
    {
#if (FEE_DEV_ERROR_DETECT == STD_ON)
        FEE_DET_REPORT_ERR(FEE_SETMODE_ID, FEE_E_UNINIT)
#endif
        detNoErr = FALSE;
    }
    if ((MEMIF_BUSY == Fee_ModuleStatus) || (MEMIF_BUSY_INTERNAL == Fee_ModuleStatus))
    {
        FEE_DET_REPORT_RUNTIME_ERR(FEE_SETMODE_ID, FEE_E_BUSY)
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
    {
        Fls_SetMode(Mode);
    }
#else
    FEE_NOUSED(Mode);
#endif /* FEE_SET_MODE_SUPPORTED == STD_ON */
}
/******************************************************************************/
/*
 * Brief               Service to initiate a read job
 * ServiceId           0x02
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      BlockNumber:Number of logical block, also denoting start
 *                                 address of that block in flash memory
 *                     BlockOffset:Read address offset inside the block
 *                     Length:Number of bytes to read
 * Param-Name[out]     DataBufferPtr:Pointer to data buffer
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 */
/******************************************************************************/
FUNC(Std_ReturnType, FEE_CODE)
Fee_Read(
    VAR(uint16, AUTOMATIC) BlockNumber,
    VAR(uint16, AUTOMATIC) BlockOffset,
    P2VAR(uint8, AUTOMATIC, FEE_APPL_DATA) DataBufferPtr,
    VAR(uint16, AUTOMATIC) Length)
{
    Std_ReturnType ret = E_NOT_OK;
    if (MEMIF_UNINIT == Fee_ModuleStatus)
    {
#if (FEE_DEV_ERROR_DETECT == STD_ON)
        FEE_DET_REPORT_ERR(FEE_READ_ID, FEE_E_UNINIT)
#endif
    }
    else
    {
        uint16 blockId = Fee_GetBlockId(BlockNumber);
#if (FEE_DEV_ERROR_DETECT == STD_ON)
        if (NULL_PTR == DataBufferPtr)
        {
            FEE_DET_REPORT_ERR(FEE_READ_ID, FEE_E_PARAM_POINTER)
        }
        else if (FEE_UNUSED_UINT16 == blockId)
        {
            FEE_DET_REPORT_ERR(FEE_READ_ID, FEE_E_INVALID_BLOCK_NO)
        }
        else if (BlockOffset >= Fee_ConfigStd->FeeBlockRef[blockId].FeeBlockSize)
        {
            FEE_DET_REPORT_ERR(FEE_READ_ID, FEE_E_INVALID_BLOCK_OFS)
        }
        else if ((0u == Length) || ((BlockOffset + Length) > Fee_ConfigStd->FeeBlockRef[blockId].FeeBlockSize))
        {
            FEE_DET_REPORT_ERR(FEE_READ_ID, FEE_E_INVALID_BLOCK_LEN)
        }
        else
#endif /*FEE_DEV_ERROR_DETECT == STD_ON*/
        {
            if ((MEMIF_BUSY != Fee_ModuleStatus) && (MEMIF_BUSY_INTERNAL != Fee_ModuleStatus))
            {
                /* Schedule the read job */
                Fee_JobInfo.Fee_BlockId            = blockId;
                Fee_JobInfo.Fee_JobBlockOffset     = BlockOffset;
                Fee_JobInfo.Fee_JobBlockLength     = Length;
                Fee_JobInfo.Fee_JobReadDataDestPtr = DataBufferPtr;
                Fee_JobInfo.Fee_Job                = FEE_JOB_READ;
                Fee_JobInfo.Fee_JobResult          = MEMIF_JOB_PENDING;
                /* set the module state */
                Fee_ModuleStatus = MEMIF_BUSY;
                ret              = E_OK;
            }
            else
            {
                FEE_DET_REPORT_RUNTIME_ERR(FEE_READ_ID, FEE_E_BUSY)
            }
        }
    }
    return ret;
}
/******************************************************************************/
/*
 * Brief               Service to initiate a write job
 * ServiceId           0x03
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      BlockNumber:Number of logical block, also denoting start
 *                                 address of that block in flash memory
 *                     DataBufferPtr:Pointer to data buffer
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 */
/******************************************************************************/
FUNC(Std_ReturnType, FEE_CODE)
Fee_Write(VAR(uint16, AUTOMATIC) BlockNumber, P2CONST(uint8, AUTOMATIC, FEE_APPL_CONST) DataBufferPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    if (MEMIF_UNINIT == Fee_ModuleStatus)
    {
#if (FEE_DEV_ERROR_DETECT == STD_ON)
        FEE_DET_REPORT_ERR(FEE_WRITE_ID, FEE_E_UNINIT)
#endif /*FEE_DEV_ERROR_DETECT == STD_ON*/
    }
    else
    {
        uint16 blockId = Fee_GetBlockId(BlockNumber);
#if (FEE_DEV_ERROR_DETECT == STD_ON)
        if (NULL_PTR == DataBufferPtr)
        {
            FEE_DET_REPORT_ERR(FEE_WRITE_ID, FEE_E_PARAM_POINTER)
        }
        else if (FEE_UNUSED_UINT16 == blockId)
        {
            FEE_DET_REPORT_ERR(FEE_WRITE_ID, FEE_E_INVALID_BLOCK_NO)
        }
        else
#endif /*FEE_DEV_ERROR_DETECT == STD_ON*/
        {
            if ((MEMIF_BUSY != Fee_ModuleStatus) && (MEMIF_BUSY_INTERNAL != Fee_ModuleStatus))
            {
                /* Schedule the write job */
                Fee_JobInfo.Fee_BlockId             = blockId;
                Fee_JobInfo.Fee_Job                 = FEE_JOB_WRITE;
                Fee_JobInfo.Fee_JobResult           = MEMIF_JOB_PENDING;
                Fee_JobInfo.Fee_JobWriteDataDestPtr = DataBufferPtr;
                /* set the module state */
                Fee_ModuleStatus = MEMIF_BUSY;
                ret              = E_OK;
            }
            else
            {
                FEE_DET_REPORT_RUNTIME_ERR(FEE_WRITE_ID, FEE_E_BUSY)
            }
        }
    }
    return ret;
}
/******************************************************************************/
/*
 * Brief               Service to call the cancel function of the underlying
 *                     flash driver
 * ServiceId           0x04
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, FEE_CODE) Fee_Cancel(void)
{
#if (FEE_CANCEL_API == STD_ON)
    if (MEMIF_UNINIT == Fee_ModuleStatus)
    {
#if (FEE_DEV_ERROR_DETECT == STD_ON)
        FEE_DET_REPORT_ERR(FEE_CANCEL_ID, FEE_E_UNINIT)
#endif /*FEE_DEV_ERROR_DETECT == STD_ON*/
    }
    else
    {
        if (MEMIF_BUSY == Fee_ModuleStatus)
        {
            /*reset the FEE module's internal variables to make the
             *module ready for a new job request from the upper layer*/
            Fee_JobInfo.Fee_Job       = FEE_JOB_NONE;
            Fee_JobInfo.Fee_JobResult = MEMIF_JOB_CANCELED;
            /* set the module state */
            Fee_ModuleStatus = MEMIF_IDLE;
            Fls_Cancel();
        }
        else
        {
            /*not change the current module status or job result.
             *just raise the runtime error.*/
            FEE_DET_REPORT_RUNTIME_ERR(FEE_CANCEL_ID, FEE_E_INVALID_CANCEL)
        }
    }
#else /*FEE_CANCEL_API == STD_ON*/
#if (FEE_DEV_ERROR_DETECT == STD_ON)
    if (MEMIF_UNINIT == Fee_ModuleStatus)
    {
        FEE_DET_REPORT_ERR(FEE_CANCEL_ID, FEE_E_UNINIT)
    }
#endif /*FEE_DEV_ERROR_DETECT == STD_ON*/
#endif /*FEE_CANCEL_API == STD_ON*/
}
/******************************************************************************/
/*
 * Brief               Service to return the status
 * ServiceId           0x05
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              MemIf_StatusType
 */
/******************************************************************************/
FUNC(MemIf_StatusType, FEE_CODE) Fee_GetStatus(void)
{
    return Fee_ModuleStatus;
}
/******************************************************************************/
/*
 * Brief               Service to query the result of the last accepted job issued
 *                     by the upper layer software
 * ServiceId           0x06
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              MemIf_JobResultType
 */
/******************************************************************************/
FUNC(MemIf_JobResultType, FEE_CODE) Fee_GetJobResult(void)
{
    MemIf_JobResultType ret = MEMIF_JOB_FAILED;
    if (MEMIF_UNINIT == Fee_ModuleStatus)
    {
#if (FEE_DEV_ERROR_DETECT == STD_ON)
        FEE_DET_REPORT_ERR(FEE_GETJOBRESULT_ID, FEE_E_UNINIT)
#endif /* FEE_DEV_ERROR_DETECT == STD_ON */
    }
    else
    {
        ret = Fee_JobInfo.Fee_JobResult;
    }
    return ret;
}
/******************************************************************************/
/*
 * Brief               Service to invalidate a logical block
 * ServiceId           0x07
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      BlockNumber:Number of logical block, also denoting start
 *                     address of that block in flash memory
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 */
/******************************************************************************/
FUNC(Std_ReturnType, FEE_CODE) Fee_InvalidateBlock(VAR(uint16, AUTOMATIC) BlockNumber)
{
    Std_ReturnType ret = E_OK;
    if (MEMIF_UNINIT == Fee_ModuleStatus)
    {
#if (FEE_DEV_ERROR_DETECT == STD_ON)
        FEE_DET_REPORT_ERR(FEE_INVALIDATEBLOCK_ID, FEE_E_UNINIT)
#endif
        ret = E_NOT_OK;
    }
    else
    {
        uint16 blockId = Fee_GetBlockId(BlockNumber);
        if (FEE_UNUSED_UINT16 == blockId)
        {
#if (FEE_DEV_ERROR_DETECT == STD_ON)
            FEE_DET_REPORT_ERR(FEE_INVALIDATEBLOCK_ID, FEE_E_INVALID_BLOCK_NO)
#endif /*FEE_DEV_ERROR_DETECT == STD_ON*/
            ret = E_NOT_OK;
        }
        else
        {
            /* do nothing */
        }
        if ((MEMIF_BUSY == Fee_ModuleStatus) || (MEMIF_BUSY_INTERNAL == Fee_ModuleStatus))
        {
            FEE_DET_REPORT_RUNTIME_ERR(FEE_INVALIDATEBLOCK_ID, FEE_E_BUSY)
            ret = E_NOT_OK;
        }
        if (ret == E_OK)
        {
            /* Schedule the invalidate block job */
            Fee_JobInfo.Fee_BlockId   = blockId;
            Fee_JobInfo.Fee_Job       = FEE_JOB_INVAL_BLOCK;
            Fee_JobInfo.Fee_JobResult = MEMIF_JOB_PENDING;
            /* set the module state */
            Fee_ModuleStatus = MEMIF_BUSY;
        }
    }
    return ret;
}
#if (FEE_VERSION_INFO_API == STD_ON)
/******************************************************************************/
/*
 * Brief               Service to return the version information of the FEE module
 * ServiceId           0x08
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     VersionInfoPtr:Pointer to standard version information structure
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, FEE_CODE)
Fee_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, FEE_APPL_DATA) VersionInfoPtr)
{
#if (FEE_DEV_ERROR_DETECT == STD_ON)
    if (NULL_PTR == VersionInfoPtr)
    {
        FEE_DET_REPORT_ERR(FEE_GETVERSIONINFO_ID, FEE_E_PARAM_POINTER)
    }
    else
#endif /* FEE_DEV_ERROR_DETECT == STD_ON */
    {
        VersionInfoPtr->moduleID         = (uint16)FEE_MODULE_ID;
        VersionInfoPtr->vendorID         = (uint16)FEE_VENDOR_ID;
        VersionInfoPtr->sw_major_version = (uint8)FEE_C_SW_MAJOR_VERSION;
        VersionInfoPtr->sw_minor_version = (uint8)FEE_C_SW_MINOR_VERSION;
        VersionInfoPtr->sw_patch_version = (uint8)FEE_C_SW_PATCH_VERSION;
    }
}
#endif /* FEE_VERSION_INFO_API == STD_ON */
/******************************************************************************/
/*
 * Brief               Service to erase a logical block
 * ServiceId           0x09
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      BlockNumber:Number of logical block
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 */
/******************************************************************************/
FUNC(Std_ReturnType, FEE_CODE) Fee_EraseImmediateBlock(VAR(uint16, AUTOMATIC) BlockNumber)
{
    Std_ReturnType ret = E_OK;
    if (MEMIF_UNINIT == Fee_ModuleStatus)
    {
#if (FEE_DEV_ERROR_DETECT == STD_ON)
        FEE_DET_REPORT_ERR(FEE_ERASEIMMEDIATEBLOCK_ID, FEE_E_UNINIT)
#endif
        ret = E_NOT_OK;
    }
    else
    {
        uint16 blockId = Fee_GetBlockId(BlockNumber);
        if ((FEE_UNUSED_UINT16 == blockId) || (FALSE == Fee_ConfigStd->FeeBlockRef[blockId].FeeImmediateData))
        {
#if (FEE_DEV_ERROR_DETECT == STD_ON)
            FEE_DET_REPORT_ERR(FEE_ERASEIMMEDIATEBLOCK_ID, FEE_E_INVALID_BLOCK_NO)
#endif /*FEE_DEV_ERROR_DETECT == STD_ON*/
            ret = E_NOT_OK;
        }
        else
        {
            /* do nothing */
        }
        if ((MEMIF_BUSY == Fee_ModuleStatus) || (MEMIF_BUSY_INTERNAL == Fee_ModuleStatus))
        {
            FEE_DET_REPORT_RUNTIME_ERR(FEE_ERASEIMMEDIATEBLOCK_ID, FEE_E_BUSY)
            ret = E_NOT_OK;
        }
        if (ret == E_OK)
        {
            /* Schedule the erase immediate block job */
            Fee_JobInfo.Fee_BlockId   = blockId;
            Fee_JobInfo.Fee_Job       = FEE_JOB_ERASE_IMMEDIATE;
            Fee_JobInfo.Fee_JobResult = MEMIF_JOB_PENDING;
            /* set the module state */
            Fee_ModuleStatus = MEMIF_BUSY;
        }
    }
    return ret;
}

/******************************************************************************/
/*
 * Brief               Service to handle the requested read / write / erase jobs
 *                     and the internal management operations
 * ServiceId           0x12
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, FEE_CODE) Fee_MainFunction(void)
{
#if (STD_ON == FEE_POLLING_MODE)
    MemIf_JobResultType flsJobResult;
#endif
    if (MEMIF_UNINIT != Fee_ModuleStatus)
    {
        if (MEMIF_JOB_PENDING == Fee_JobInfo.Fee_JobResult)
        {
#if (STD_ON == FEE_POLLING_MODE)
            /*Internal job handle*/
            if (Fee_JobInfo.Fee_Job > FEE_JOB_ERASE_IMMEDIATE)
            {
                flsJobResult = Fls_GetJobResult();
                if (MEMIF_JOB_PENDING == flsJobResult)
                {
                    /* Nothing to do */
                }
                else if (MEMIF_JOB_OK == flsJobResult)
                {
                    /*Fls job success end handle*/
                    Fee_FlsJobEndHandle();
#if ((FEE_E_INIT_FAILED_DET == STD_ON) && (FEE_DEV_ERROR_DETECT == STD_ON))
                    if (Fee_ModuleStatus == MEMIF_UNINIT)
                    {
                        FEE_DET_REPORT_ERR(FEE_MAINFUNCTION_ID, FEE_E_INIT_FAILED)
                    }
#endif /*FEE_E_INIT_FAILED_DET == STD_ON && FEE_DEV_ERROR_DETECT == STD_ON*/
                }
                else
                {
                    /*Fls job error end handle*/
                    Fee_FlsJobErrorHandle();
#if ((FEE_E_INIT_FAILED_DET == STD_ON) && (FEE_DEV_ERROR_DETECT == STD_ON))
                    if (Fee_ModuleStatus == MEMIF_UNINIT)
                    {
                        FEE_DET_REPORT_ERR(FEE_MAINFUNCTION_ID, FEE_E_INIT_FAILED)
                    }
#endif /*FEE_E_INIT_FAILED_DET == STD_ON && FEE_DEV_ERROR_DETECT == STD_ON*/
                }
            }
            else
#endif /*STD_ON == FEE_POLLING_MODE*/
            {
                switch (Fee_JobInfo.Fee_Job)
                {
                case FEE_JOB_INT_SCAN:
                    Fee_JobInitScan();
                    break;
                case FEE_JOB_READ:
                    Fee_JobRead();
                    break;
                case FEE_JOB_WRITE:
                    Fee_JobWrite();
                    break;
                case FEE_JOB_INVAL_BLOCK:
                    Fee_JobInvalBlock();
                    break;
                case FEE_JOB_ERASE_IMMEDIATE:
                    Fee_JobEraseImmediate();
                    break;
                default:
                    /* Internal or subsequent job */
                    break;
                }
                if (MEMIF_JOB_PENDING == Fee_JobInfo.Fee_JobResult)
                {
                    /* Nothing to do */
                }
                else if (MEMIF_JOB_OK == Fee_JobInfo.Fee_JobResult)
                {
                    if (MEMIF_BUSY_INTERNAL == Fee_ModuleStatus)
                    {
                        /* no notifications from internal jobs */
                        Fee_ModuleStatus = MEMIF_IDLE;
                    }
                    else
                    {
                        Fee_ModuleStatus = MEMIF_IDLE;
                        /* Call job end notification function */
                        FEE_NVMJOBEND_NOTIFICATION
                    }
                }
                else
                {
                    if (MEMIF_BUSY_INTERNAL == Fee_ModuleStatus)
                    {
                        /* Call initial job error notification function */
                        Fee_ModuleStatus = MEMIF_UNINIT;
                        FEE_INITERROR_NOTIFICATION
#if ((FEE_E_INIT_FAILED_DET == STD_ON) && (FEE_DEV_ERROR_DETECT == STD_ON))
                        FEE_DET_REPORT_ERR(FEE_MAINFUNCTION_ID, FEE_E_INIT_FAILED)
#endif /*FEE_E_INIT_FAILED_DET == STD_ON && FEE_DEV_ERROR_DETECT == STD_ON*/
                    }
                    else
                    {
                        Fee_ModuleStatus = MEMIF_IDLE;
                        /* Call job error notification function */
                        FEE_NVMJOBERROR_NOTIFICATION
                    }
                }
            }
        }
    }
}
#define FEE_STOP_SEC_CODE
#include "Fee_MemMap.h"

/******************************************************************************/
/*
 * Brief               Service to report to this module the successful end of
 *                     an asynchronous operation
 * ServiceId           0x10
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#define FEE_START_SEC_FEEJOBENDNOTIFICATION_CALLBACK_CODE
#include "Fee_MemMap.h"
FUNC(void, FEE_CODE) Fee_JobEndNotification(void)
{
#if (STD_OFF == FEE_POLLING_MODE)
    if (MEMIF_UNINIT == Fee_ModuleStatus)
    {
#if (FEE_DEV_ERROR_DETECT == STD_ON)
        FEE_DET_REPORT_ERR(FEE_JOBENDNOTIFICATION_ID, FEE_E_UNINIT)
#endif /* FEE_DEV_ERROR_DETECT == STD_ON */
    }
    else
    {
        Fee_FlsJobEndHandle();
#if ((FEE_E_INIT_FAILED_DET == STD_ON) && (FEE_DEV_ERROR_DETECT == STD_ON))
        if (Fee_ModuleStatus == MEMIF_UNINIT)
        {
            FEE_DET_REPORT_ERR(FEE_JOBENDNOTIFICATION_ID, FEE_E_INIT_FAILED)
        }
#endif /*FEE_E_INIT_FAILED_DET == STD_ON && FEE_DEV_ERROR_DETECT == STD_ON*/
    }
#endif /*STD_OFF == FEE_POLLING_MODE*/
}
#define FEE_STOP_SEC_FEEJOBENDNOTIFICATION_CALLBACK_CODE
#include "Fee_MemMap.h"
/******************************************************************************/
/*
 * Brief               Service to report to this module the failure of an
 *                     asynchronous operation
 * ServiceId           0x11
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#define FEE_START_SEC_FEEJOBERRORNOTIFICATION_CALLBACK_CODE
#include "Fee_MemMap.h"
FUNC(void, FEE_CODE) Fee_JobErrorNotification(void)
{
#if (STD_OFF == FEE_POLLING_MODE)
    if (MEMIF_UNINIT == Fee_ModuleStatus)
    {
#if (FEE_DEV_ERROR_DETECT == STD_ON)
        FEE_DET_REPORT_ERR(FEE_JOBERRORNOTIFICATION_ID, FEE_E_UNINIT)
#endif /* FEE_DEV_ERROR_DETECT == STD_ON */
    }
    else
    {
        Fee_FlsJobErrorHandle();
#if ((FEE_E_INIT_FAILED_DET == STD_ON) && (FEE_DEV_ERROR_DETECT == STD_ON))
        if (Fee_ModuleStatus == MEMIF_UNINIT)
        {
            FEE_DET_REPORT_ERR(FEE_JOBERRORNOTIFICATION_ID, FEE_E_INIT_FAILED)
        }
#endif /*FEE_E_INIT_FAILED_DET == STD_ON && FEE_DEV_ERROR_DETECT == STD_ON*/
    }
#endif /*STD_OFF == FEE_POLLING_MODE*/
}
#define FEE_STOP_SEC_FEEJOBERRORNOTIFICATION_CALLBACK_CODE
#include "Fee_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#define FEE_START_SEC_CODE
#include "Fee_MemMap.h"
/*Flash job success end handle*/
FEE_LOCAL FUNC(void, FEE_CODE) Fee_FlsJobEndHandle(void)
{
    if (FEE_JOB_NONE == Fee_JobInfo.Fee_Job)
    {
        Fee_JobInfo.Fee_JobResult = MEMIF_JOB_OK;
        if (MEMIF_BUSY_INTERNAL == Fee_ModuleStatus)
        {
            /* no notifications from internal jobs */
            Fee_ModuleStatus = MEMIF_IDLE;
        }
        else
        {
            Fee_ModuleStatus = MEMIF_IDLE;
            /* Call job end notification function */
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
                /* no notifications from internal jobs */
                Fee_ModuleStatus = MEMIF_IDLE;
            }
            else
            {
                Fee_ModuleStatus = MEMIF_IDLE;
                /* Call job end notification function */
                FEE_NVMJOBEND_NOTIFICATION
            }
        }
        else if (MEMIF_JOB_PENDING == Fee_JobInfo.Fee_JobResult)
        {
            /* Nothing to do (ongoing Fls job) */
        }
        else
        {
            if (MEMIF_BUSY_INTERNAL == Fee_ModuleStatus)
            {
                /* Call initial job error notification function */
                Fee_ModuleStatus = MEMIF_UNINIT;
                FEE_INITERROR_NOTIFICATION
            }
            else
            {
                Fee_ModuleStatus = MEMIF_IDLE;
                /* Call job error notification function */
                FEE_NVMJOBERROR_NOTIFICATION
            }
        }
    }
}
/*Flash job error handle*/
FEE_LOCAL FUNC(void, FEE_CODE) Fee_FlsJobErrorHandle(void)
{
    if (MEMIF_JOB_CANCELED == Fee_JobInfo.Fee_JobResult)
    {
        FEE_NVMJOBERROR_NOTIFICATION
    }
    else
    {
        uint8 clusterGrpIdx = Fee_ConfigStd->FeeBlockRef[Fee_JobInfo.Fee_BlockId].ClusterGrp;
        switch (Fee_JobInfo.Fee_Job)
        {
        /* Error while reading block header. Treat block header as
            invalid and reading the next block headers */
        case FEE_JOB_INT_SCAN_BLOCK_HDR_GET:
            Fee_JobInfo.Fee_JobResult = Fee_JobIntScanBlockHdrGet((boolean)FALSE);
            break;
        /* Error while reading cluster header. Treat cluster as
            invalid and move on to next cluster */
        case FEE_JOB_INT_SCAN_CLUSTER_HDR_GET:
            Fee_JobInfo.Fee_JobResult = Fee_JobIntScanClusterHdrGet((boolean)FALSE);
            break;
        case FEE_JOB_WRITE_DATA:
        case FEE_JOB_WRITE_VALIDATE:
        case FEE_JOB_WRITE_DONE:
        case FEE_JOB_ERASE_IMMEDIATE_DONE:
#if (STD_ON == FEE_ERROR_SWAP_CLUSTER)
            /* Set the next write/immediate job trigger the swap handle */
            Fee_ClusterGrpInfo[clusterGrpIdx].Fee_BlockDataAddr =
                Fee_ClusterGrpInfo[clusterGrpIdx].Fee_BlockHdrAddr + (2u * FEE_BLOCK_HEADER_SIZE);
            Fee_JobInfo.Fee_JobResult = Fls_GetJobResult();
            Fee_ModuleStatus          = MEMIF_IDLE;
            /* Call job error notification function */
            FEE_NVMJOBERROR_NOTIFICATION
            break;
#endif
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
                /* Call initial job error notification function */
                Fee_ModuleStatus = MEMIF_UNINIT;
                FEE_INITERROR_NOTIFICATION
            }
            else
            {
                Fee_ModuleStatus = MEMIF_IDLE;
                /* Call job error notification function */
                FEE_NVMJOBERROR_NOTIFICATION
            }
            break;
        default:
            /*Never run to here,unless administrative data has been tampered*/
            /*FEE_JOB_WRITE,FEE_JOB_READ,FEE_JOB_INVAL_BLOCK,
             *FEE_JOB_ERASE_IMMEDIATE,FEE_JOB_INT_SCAN*/
            break;
        }
    }
}
/*find the local block id of the BlockNumber*/
FEE_LOCAL FUNC(uint16, FEE_CODE) Fee_GetBlockId(const uint16 BlockNumber)
{
    sint32 sLow  = 0L;
    sint32 sHigh = (sint32)(Fee_ConfigStd->FeeBlockSum) - 1;
    sint32 sMiddle;
    uint16 ret = FEE_UNUSED_UINT16;
    uint16 sMiddleBlockNumber;
    while (sLow <= sHigh)
    {
        sMiddle            = sLow + ((sHigh - sLow) / 2);
        sMiddleBlockNumber = Fee_ConfigStd->FeeBlockRef[sMiddle].FeeBlockNumber;
        if (BlockNumber < sMiddleBlockNumber)
        {
            sHigh = sMiddle - 1;
        }
        else if (BlockNumber > sMiddleBlockNumber)
        {
            sLow = sMiddle + 1;
        }
        else
        {
            ret = (uint16)sMiddle;
            break;
        }
    }
    return ret;
}
/*read the cluster header infomation*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobIntScanClusterHdrRead(void)
{
    MemIf_JobResultType ret;
    Fls_AddressType     clusterStartAddr;
    /*active Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_ClusterScanId must be 0*/
    clusterStartAddr = Fee_ClusterGroupConfig[Fee_ClusterGroupIdx].ClusterPtr[Fee_ClusterIdx].FeeClusterStartAddr;
    if (((Std_ReturnType)E_OK) != Fls_Read(clusterStartAddr, Fee_DataBuffer, FEE_CLUSTER_HEADER_SIZE))
    {
        /* Fls read job hasn't been accepted */
        ret = MEMIF_JOB_FAILED;
    }
    else
    {
        /* Fls read job has been accepted */
        ret = MEMIF_JOB_PENDING;
    }
    Fee_JobInfo.Fee_Job = FEE_JOB_INT_SCAN_CLUSTER_HDR_GET;
    return ret;
}
/*initial job handle,try to read cluster header*/
FEE_LOCAL FUNC(void, FEE_CODE) Fee_JobInitScan(void)
{
    Fee_ClusterGroupIdx       = 0;
    Fee_ClusterIdx            = 0;
    Fee_JobInfo.Fee_JobResult = Fee_JobIntScanClusterHdrRead();
}
/* All internal jobs handle*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobSchedule(void) /* PRQA S 6010 */ /* VL_MTR_Fee_STCYC */
{
    MemIf_JobResultType ret = MEMIF_JOB_FAILED;
    switch (Fee_JobInfo.Fee_Job)
    {
    /* write related jobs */
    case FEE_JOB_WRITE_DATA:
        ret = Fee_JobWriteData();
        break;
    case FEE_JOB_WRITE_VALIDATE:
        ret = Fee_JobWriteValidate();
        break;
    case FEE_JOB_WRITE_DONE:
        ret = Fee_JobWriteDone();
        break;
    /* invalid related jobs */
    case FEE_JOB_INVAL_BLOCK_DONE:
        ret = Fee_JobInvalBlockDone();
        break;
    /* erase related jobs */
    case FEE_JOB_ERASE_IMMEDIATE_DONE:
        ret = Fee_JobEraseImmediateDone();
        break;
    /* Initial related jobs */
    case FEE_JOB_INT_SCAN_CLUSTER_HDR_GET:
        ret = Fee_JobIntScanClusterHdrGet((boolean)TRUE);
        break;
    case FEE_JOB_INT_SCAN_CLUSTER_CLEAR:
        ret = Fee_JobIntScanClusterClear();
        break;
    case FEE_JOB_INT_SCAN_CLUSTER_CLEAR_DONE:
        ret = Fee_JobIntScanClusterClearDone();
        break;
    case FEE_JOB_INT_SCAN_BLOCK_HDR_GET:
        ret = Fee_JobIntScanBlockHdrGet((boolean)TRUE);
        break;
    /* Swap related jobs */
    case FEE_JOB_INT_SWAP_CLUSTER_CLEAR:
        ret = Fee_JobIntSwapClusterClear();
        break;
    case FEE_JOB_INT_SWAP_BLOCK:
        ret = Fee_JobIntSwapBlock();
        break;
    case FEE_JOB_INT_SWAP_DATA_READ:
        ret = Fee_JobIntSwapDataRead();
        break;
    case FEE_JOB_INT_SWAP_DATA_WRITE:
        ret = Fee_JobIntSwapDataWrite();
        break;
    case FEE_JOB_INT_SWAP_BLOCK_VLD:
        ret = Fee_JobIntSwapBlockVldWrite();
        break;
    case FEE_JOB_INT_SWAP_CLUSTER_VLD_DONE:
        ret = Fee_JobIntSwapClusterVldDone();
        break;
    default:
        /* Warning: This default branch will never be reached */
        break;
    }
    return ret;
}
/*Read Fee block*/
FEE_LOCAL FUNC(void, FEE_CODE) Fee_JobRead(void)
{
    Fee_BlockStatusType blockStatus;
    Fls_AddressType     blockAddress;
    blockStatus  = Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_BlockStatus;
    blockAddress = Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_DataAddr;
    if (FEE_BLOCK_VALID == blockStatus)
    {
        if (((Std_ReturnType)E_OK)
            != Fls_Read(
                blockAddress + Fee_JobInfo.Fee_JobBlockOffset,
                Fee_JobInfo.Fee_JobReadDataDestPtr,
                Fee_JobInfo.Fee_JobBlockLength))
        {
            /* Fls read job hasn't been accepted */
            Fee_JobInfo.Fee_JobResult = MEMIF_JOB_FAILED;
        }
        else
        {
            /* Fls read job has been accepted */
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
    /*Job switch to FEE_JOB_NONE*/
    Fee_JobInfo.Fee_Job = FEE_JOB_NONE;
}
/*Write Fee block to flash*/
FEE_LOCAL FUNC(void, FEE_CODE) Fee_JobWrite(void)
{
    if (TRUE == Fee_ConfigStd->FeeBlockRef[Fee_JobInfo.Fee_BlockId].FeeImmediateData)
    {
        /*ImmediateData block's header has been written done*/
        if (FEE_BLOCK_INCONSISTENT == Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_BlockStatus)
        {
            /* The immediate block is ready to be written */
            Fee_JobInfo.Fee_JobResult = Fee_JobWriteData();
        }
        else
        {
            /* The immediate block is not pre-erased and ready to be written */
            Fee_JobInfo.Fee_JobResult = MEMIF_JOB_FAILED;
        }
    }
    else
    {
        /* Write block header */
        Fee_JobInfo.Fee_JobResult = Fee_JobWriteHdr();
        /* Schedule the next internal job */
        if (FEE_JOB_INT_SWAP_CLUSTER_CLEAR == Fee_JobInfo.Fee_Job)
        {
            /* Cluster swap has been enforced... */
        }
        else
        {
            Fee_JobInfo.Fee_Job = FEE_JOB_WRITE_DATA;
        }
    }
}
/*Invalidate Fee block by writing the invalid flag to flash*/
FEE_LOCAL FUNC(void, FEE_CODE) Fee_JobInvalBlock(void)
{
    if (FEE_BLOCK_INVALID == Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_BlockStatus)
    {
        /* Fee job completed */
        Fee_JobInfo.Fee_Job       = FEE_JOB_NONE;
        Fee_JobInfo.Fee_JobResult = MEMIF_JOB_OK;
    }
    else
    {
#if (STD_ON == FEE_PAGE_DIRECT_WRITE_SUPPORT)
        /* Pack invalid flag to buffer */
        Fee_PackBlockHdrValidOrInvalid(Fee_DataBuffer, FEE_INVALID_VALUE);
        /* Write invalidation pattern to flash */
        if (((Std_ReturnType)E_OK)
            == Fls_Write(Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_HdrAddr, Fee_DataBuffer, FEE_VIRTUALPAGE_SIZE))
#else
        /* Pack invalid flag to buffer */
        Fee_PackHdrValidOrInvalid(Fee_DataBuffer, FEE_INVALID_VALUE);
        /* Write invalidation pattern to flash */
        if (((Std_ReturnType)E_OK)
            == Fls_Write(
                (Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_ValidHdrAddr + FEE_VIRTUALPAGE_SIZE),
                Fee_DataBuffer,
                FEE_VIRTUALPAGE_SIZE))
#endif
        {
            /* Fls write job has been accepted */
            Fee_JobInfo.Fee_JobResult = MEMIF_JOB_PENDING;
        }
        else
        {
            /* Fls write job hasn't been accepted */
            Fee_JobInfo.Fee_JobResult = MEMIF_JOB_FAILED;
        }
        /* Switch job to FEE_JOB_INVAL_BLOCK_DONE */
        Fee_JobInfo.Fee_Job = FEE_JOB_INVAL_BLOCK_DONE;
    }
}
/*Erase (pre-allocate) immediate Fee block*/
FEE_LOCAL FUNC(void, FEE_CODE) Fee_JobEraseImmediate(void)
{
    /* Write block header */
    Fee_JobInfo.Fee_JobResult = Fee_JobWriteHdr();
    /* Schedule the next internal job */
    if (FEE_JOB_INT_SWAP_CLUSTER_CLEAR == Fee_JobInfo.Fee_Job)
    {
        /* Cluster swap has been enforced... */
    }
    else
    {
        Fee_JobInfo.Fee_Job = FEE_JOB_ERASE_IMMEDIATE_DONE;
    }
}
/*invalidation of Fee block done*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobInvalBlockDone(void)
{
    /* Mark the Fee block as invalid */
    Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_BlockStatus = FEE_BLOCK_INVALID;
    /* Fee job completed */
    Fee_JobInfo.Fee_Job = FEE_JOB_NONE;
    return MEMIF_JOB_OK;
}
/*Erase immediate Fee block done*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobEraseImmediateDone(void)
{
    uint16 blockSize;
    uint8  clusterGrpIdx = Fee_ConfigStd->FeeBlockRef[Fee_JobInfo.Fee_BlockId].ClusterGrp;
    blockSize            = Fee_ConfigStd->FeeBlockRef[Fee_JobInfo.Fee_BlockId].FeeBlockSize;
    /*Header has been written so update both header and data addresses of the cluster*/
    Fee_ClusterGrpInfo[clusterGrpIdx].Fee_BlockDataAddr -= blockSize;
    Fee_ClusterGrpInfo[clusterGrpIdx].Fee_BlockHdrAddr += FEE_BLOCK_HEADER_SIZE;
    /* The block header has written,but the block data not write */
    Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_BlockStatus = FEE_BLOCK_INCONSISTENT;
    Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_DataAddr    = Fee_ClusterGrpInfo[clusterGrpIdx].Fee_BlockDataAddr;
#if (STD_ON == FEE_PAGE_DIRECT_WRITE_SUPPORT)
    Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_HdrAddr =
        Fee_ClusterGrpInfo[clusterGrpIdx].Fee_BlockHdrAddr - FEE_BLOCK_HEADER_SIZE;
#else
    Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_ValidHdrAddr =
        (Fee_ClusterGrpInfo[clusterGrpIdx].Fee_BlockHdrAddr - (2u * FEE_VIRTUALPAGE_SIZE));
#endif
    /*Set the job to NONE*/
    Fee_JobInfo.Fee_Job = FEE_JOB_NONE;
    return MEMIF_JOB_OK;
}
/*Write Fee block header to flash*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobWriteHdr(void)
{
    MemIf_JobResultType ret;
    uint16              blockSize;
    Fls_AddressType     dataAddr;
    uint8               clusterGrpIdx = Fee_ConfigStd->FeeBlockRef[Fee_JobInfo.Fee_BlockId].ClusterGrp;
    blockSize                         = Fee_ConfigStd->FeeBlockRef[Fee_JobInfo.Fee_BlockId].FeeBlockSize;
    /*check the cluster have enough space or not*/
    if ((((uint32)blockSize) + (2u * FEE_BLOCK_HEADER_SIZE))
        > (Fee_ClusterGrpInfo[clusterGrpIdx].Fee_BlockDataAddr - Fee_ClusterGrpInfo[clusterGrpIdx].Fee_BlockHdrAddr))
    {
        /*No enough space,perform cluster swap handle*/
        Fee_ClusterGroupIdx = clusterGrpIdx;
        ret                 = Fee_JobIntSwap();
    }
    else
    {
        dataAddr = Fee_ClusterGrpInfo[clusterGrpIdx].Fee_BlockDataAddr - blockSize;
        Fee_PackBlockHdr(
            Fee_ConfigStd->FeeBlockRef[Fee_JobInfo.Fee_BlockId].FeeBlockNumber,
            blockSize,
            dataAddr,
            Fee_ConfigStd->FeeBlockRef[Fee_JobInfo.Fee_BlockId].FeeImmediateData,
            Fee_DataBuffer);
        /* Write header to flash */
        if (((Std_ReturnType)E_OK)
            != Fls_Write(
                Fee_ClusterGrpInfo[clusterGrpIdx].Fee_BlockHdrAddr,
                Fee_DataBuffer,
#if (STD_ON == FEE_PAGE_DIRECT_WRITE_SUPPORT)
                FEE_BLOCK_HEADER_SIZE))
#else
                FEE_BLOCK_HEADER_SIZE - (2u * FEE_VIRTUALPAGE_SIZE)))
#endif
        {
            /* Fls read job hasn't been accepted */
            ret = MEMIF_JOB_FAILED;
        }
        else
        {
            /* Fls read job has been accepted */
            ret = MEMIF_JOB_PENDING;
        }
    }
    return ret;
}
/*Initialize the cluster swap internal operation,
 *This can only happen during the block header writing phase*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobIntSwap(void)
{
    MemIf_JobResultType ret;
    /* Store type of original job so Fee can re-schedule
        this job once the swap is complete */
    Fee_JobInfo.Fee_SwapJobInfo.Swap_PreJob = Fee_JobInfo.Fee_Job;
    /* Update the swap information */
    Fee_JobInfo.Fee_SwapJobInfo.Swap_BlockId   = 0u;
    Fee_JobInfo.Fee_SwapJobInfo.Swap_ClusterId = Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_ValidClusterId + 1u;
    if (Fee_JobInfo.Fee_SwapJobInfo.Swap_ClusterId == Fee_ClusterGroupConfig[Fee_ClusterGroupIdx].ClusterCount)
    {
        Fee_JobInfo.Fee_SwapJobInfo.Swap_ClusterId = 0u;
    }
    Fee_JobInfo.Fee_SwapJobInfo.Swap_HdrAddr = Fee_ClusterGroupConfig[Fee_ClusterGroupIdx]
                                                   .ClusterPtr[Fee_JobInfo.Fee_SwapJobInfo.Swap_ClusterId]
                                                   .FeeClusterStartAddr;
    Fee_JobInfo.Fee_SwapJobInfo.Swap_DataAddr =
        (Fee_JobInfo.Fee_SwapJobInfo.Swap_HdrAddr
         + Fee_ClusterGroupConfig[Fee_ClusterGroupIdx]
               .ClusterPtr[Fee_JobInfo.Fee_SwapJobInfo.Swap_ClusterId]
               .FeeClusterLength);
    Fee_JobInfo.Fee_SwapJobInfo.Swap_HdrAddr += FEE_CLUSTER_HEADER_SIZE;
    /* Erase the swap cluster */
    ret = Fee_JobIntSwapClusterErase();
    return ret;
}
/*Erase the new cluster for swap*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobIntSwapClusterErase(void)
{
    MemIf_JobResultType ret;
    Fls_AddressType     startAddr;
    Fls_LengthType      length;
    /*Get the new cluster start address and length*/
    startAddr = Fee_ClusterGroupConfig[Fee_ClusterGroupIdx]
                    .ClusterPtr[Fee_JobInfo.Fee_SwapJobInfo.Swap_ClusterId]
                    .FeeClusterStartAddr;
    length = Fee_ClusterGroupConfig[Fee_ClusterGroupIdx]
                 .ClusterPtr[Fee_JobInfo.Fee_SwapJobInfo.Swap_ClusterId]
                 .FeeClusterLength;
    /* Erase cluster */
    if (((Std_ReturnType)E_OK) == Fls_Erase(startAddr, length))
    {
        /* Fls read job has been accepted */
        ret = MEMIF_JOB_PENDING;
    }
    else
    {
        /* Fls read job hasn't been accepted */
        ret = MEMIF_JOB_FAILED;
    }
    /* Switch job to FEE_JOB_INT_SWAP_CLUSTER_CLEAR */
    Fee_JobInfo.Fee_Job = FEE_JOB_INT_SWAP_CLUSTER_CLEAR;
    return ret;
}
/*Clear current Fee cluster done,writing cluster header into flash*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobIntSwapClusterClear(void)
{
    MemIf_JobResultType ret;
    Fls_AddressType     startAddr;
    Fls_LengthType      length;
    uint32              clusterUseID;
    startAddr = Fee_ClusterGroupConfig[Fee_ClusterGroupIdx]
                    .ClusterPtr[Fee_JobInfo.Fee_SwapJobInfo.Swap_ClusterId]
                    .FeeClusterStartAddr;
    length = Fee_ClusterGroupConfig[Fee_ClusterGroupIdx]
                 .ClusterPtr[Fee_JobInfo.Fee_SwapJobInfo.Swap_ClusterId]
                 .FeeClusterLength;
    clusterUseID = Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_ClusterUseID;
    /* Build cluster header information */
    Fee_PackClusterHdr(clusterUseID + 1u, startAddr, length, Fee_DataBuffer);

    /* Write the cluster header to flash */
    if (((Std_ReturnType)E_OK) == Fls_Write(startAddr, Fee_DataBuffer, FEE_CLUSTER_HEADER_SIZE - FEE_VIRTUALPAGE_SIZE))
    {
        /* Fls read job has been accepted */
        ret = MEMIF_JOB_PENDING;
    }
    else
    {
        /* Fls write job hasn't been accepted */
        ret = MEMIF_JOB_FAILED;
    }
    /* Switch job to FEE_JOB_INT_SWAP_BLOCK */
    Fee_JobInfo.Fee_Job = FEE_JOB_INT_SWAP_BLOCK;
    return ret;
}
/* Copy block from source to target cluster */
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobIntSwapBlock(void)
{
    MemIf_JobResultType ret;
    Fee_BlockStatusType blockStatus    = FEE_BLOCK_INVALID;
    boolean             immediateBlock = FALSE;
    uint16              blockId;
    uint16              blockSize;
    uint16              blockNum;
    Fls_AddressType     dataAddr;
    blockId = Fee_JobInfo.Fee_SwapJobInfo.Swap_BlockId;
    for (; blockId < Fee_ConfigStd->FeeBlockSum; blockId++)
    {
        uint8 blockClusterGrp = Fee_ConfigStd->FeeBlockRef[blockId].ClusterGrp;
        blockStatus           = Fee_BlockInfo[blockId].Fee_BlockStatus;
        immediateBlock        = Fee_ConfigStd->FeeBlockRef[blockId].FeeImmediateData;
        if ((blockClusterGrp == Fee_ClusterGroupIdx)
            && ((FEE_BLOCK_VALID == blockStatus)
                || ((TRUE == immediateBlock) && (FEE_BLOCK_INCONSISTENT == blockStatus))))
        {
            break;
        }
        else
        {
            /* No match. Try next one... */
        }
    }
    Fee_JobInfo.Fee_SwapJobInfo.Swap_BlockId = blockId;
    if (blockId == Fee_ConfigStd->FeeBlockSum)
    {
        /* No more matching blocks. Validate the cluster */
        ret = Fee_JobIntSwapClusterVld();
    }
    else
    {
        blockSize = Fee_ConfigStd->FeeBlockRef[blockId].FeeBlockSize;
        blockNum  = Fee_ConfigStd->FeeBlockRef[blockId].FeeBlockNumber;
        dataAddr  = Fee_JobInfo.Fee_SwapJobInfo.Swap_DataAddr - blockSize;
        /*Pack the block header information*/
        Fee_PackBlockHdr(blockNum, blockSize, dataAddr, immediateBlock, Fee_DataBuffer);
        /* Write header to flash */
        if (((Std_ReturnType)E_OK)
            != Fls_Write(
                Fee_JobInfo.Fee_SwapJobInfo.Swap_HdrAddr,
                Fee_DataBuffer,
#if (STD_ON == FEE_PAGE_DIRECT_WRITE_SUPPORT)
                FEE_BLOCK_HEADER_SIZE))
#else
                FEE_BLOCK_HEADER_SIZE - (2u * FEE_VIRTUALPAGE_SIZE)))
#endif
        {
            /* Fls write job hasn't been accepted */
            ret = MEMIF_JOB_FAILED;
        }
        else
        {
            /* Fls write job has been accepted */
            ret = MEMIF_JOB_PENDING;
        }
        if (FEE_BLOCK_VALID == blockStatus)
        {
            /*Read the block data*/
            Fee_JobInfo.Fee_Job = FEE_JOB_INT_SWAP_DATA_READ;
        }
        else
        {
            /* Move on to next block */
            Fee_JobInfo.Fee_SwapJobInfo.Swap_BlockId += 1u;
            Fee_JobInfo.Fee_Job = FEE_JOB_INT_SWAP_BLOCK;
        }
        /*Update the swap cluster address(data&header)*/
        Fee_JobInfo.Fee_SwapJobInfo.Swap_DataAddr = dataAddr;
        Fee_JobInfo.Fee_SwapJobInfo.Swap_HdrAddr += FEE_BLOCK_HEADER_SIZE;
    }
    return ret;
}
/*Success write cluster valid flag,update all copied block information*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobIntSwapClusterVldDone(void)
{
    MemIf_JobResultType ret;
    uint16              cnt;
    uint16              blockSize;
    Fls_AddressType     hdrAddr;
    Fls_AddressType     dataAddr;
    boolean             immediateBlock;
    hdrAddr = Fee_ClusterGroupConfig[Fee_ClusterGroupIdx]
                  .ClusterPtr[Fee_JobInfo.Fee_SwapJobInfo.Swap_ClusterId]
                  .FeeClusterStartAddr;
    dataAddr = hdrAddr
               + Fee_ClusterGroupConfig[Fee_ClusterGroupIdx]
                     .ClusterPtr[Fee_JobInfo.Fee_SwapJobInfo.Swap_ClusterId]
                     .FeeClusterLength;
    hdrAddr += FEE_CLUSTER_HEADER_SIZE;
    /*update all copied block information*/
    for (cnt = 0u; cnt < Fee_ConfigStd->FeeBlockSum; cnt++)
    {
        uint8 clusterId = Fee_ConfigStd->FeeBlockRef[cnt].ClusterGrp;
        if (clusterId == Fee_ClusterGroupIdx)
        {
            immediateBlock = Fee_ConfigStd->FeeBlockRef[cnt].FeeImmediateData;
            if ((FEE_BLOCK_VALID == Fee_BlockInfo[cnt].Fee_BlockStatus)
                || ((FEE_BLOCK_INCONSISTENT == Fee_BlockInfo[cnt].Fee_BlockStatus)
                    && ((boolean)TRUE == immediateBlock)))
            {
                blockSize = Fee_ConfigStd->FeeBlockRef[cnt].FeeBlockSize;
                dataAddr -= blockSize;
                hdrAddr += FEE_BLOCK_HEADER_SIZE;
                Fee_BlockInfo[cnt].Fee_DataAddr = dataAddr;
#if (STD_ON == FEE_PAGE_DIRECT_WRITE_SUPPORT)
                Fee_BlockInfo[cnt].Fee_HdrAddr = hdrAddr - FEE_BLOCK_HEADER_SIZE;
#else
                Fee_BlockInfo[cnt].Fee_ValidHdrAddr = (hdrAddr - (2u * FEE_VIRTUALPAGE_SIZE));
#endif
            }
        }
    }
    /* Switch to the new cluster */
    Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_ClusterUseID += 1u;
    Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_ValidClusterId = Fee_JobInfo.Fee_SwapJobInfo.Swap_ClusterId;
    Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_BlockDataAddr  = dataAddr;
    Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_BlockHdrAddr   = hdrAddr;
    /*Restores the pre-swap job*/
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
        /*never run to here,unless administrative data has been tampered*/
        ret = MEMIF_JOB_FAILED;
    }
    return ret;
}
/*Validate current cluster by writing FEE_VALID_VALUE into flash*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobIntSwapClusterVld(void)
{
    MemIf_JobResultType ret;
    Fls_AddressType     startAddr;
    startAddr = Fee_ClusterGroupConfig[Fee_ClusterGroupIdx]
                    .ClusterPtr[Fee_JobInfo.Fee_SwapJobInfo.Swap_ClusterId]
                    .FeeClusterStartAddr;
    /* Pack valid flag to buffer */
    Fee_PackHdrValidOrInvalid(Fee_DataBuffer, FEE_VALID_VALUE);
    /* Write validation pattern to flash */
    if (((Std_ReturnType)E_OK)
        == Fls_Write(
            (startAddr + FEE_CLUSTER_HEADER_SIZE - FEE_VIRTUALPAGE_SIZE),
            Fee_DataBuffer,
            FEE_VIRTUALPAGE_SIZE))
    {
        /* Fls write job has been accepted */
        ret = MEMIF_JOB_PENDING;
    }
    else
    {
        /* Fls write job hasn't been accepted */
        ret = MEMIF_JOB_FAILED;
    }
    /* Switch job to FEE_JOB_INT_SWAP_CLUSTER_VLD_DONE */
    Fee_JobInfo.Fee_Job = FEE_JOB_INT_SWAP_CLUSTER_VLD_DONE;
    return ret;
}
/*Read data from source cluster to internal Fee buffer*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobIntSwapDataRead(void)
{
    MemIf_JobResultType ret;
    uint16              blockId;
    uint16              blockSize;
    blockId   = Fee_JobInfo.Fee_SwapJobInfo.Swap_BlockId;
    blockSize = Fee_ConfigStd->FeeBlockRef[blockId].FeeBlockSize;
    /* Read the block data */
    if (((Std_ReturnType)E_OK) != Fls_Read(Fee_BlockInfo[blockId].Fee_DataAddr, Fee_DataBuffer, blockSize))
    {
        /* Fls read job hasn't been accepted */
        ret = MEMIF_JOB_FAILED;
    }
    else
    {
        /* Fls read job has been accepted */
        ret = MEMIF_JOB_PENDING;
    }
    /*Write the block data*/
    Fee_JobInfo.Fee_Job = FEE_JOB_INT_SWAP_DATA_WRITE;
    return ret;
}
/*Write block header valid flag*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobIntSwapBlockVldWrite(void)
{
    MemIf_JobResultType ret;
#if (STD_ON == FEE_PAGE_DIRECT_WRITE_SUPPORT)
    /* Pack valid flag to buffer */
    Fee_PackBlockHdrValidOrInvalid(Fee_DataBuffer, FEE_VALID_VALUE);
    /* Write validation pattern to flash */
    if (((Std_ReturnType)E_OK)
        != Fls_Write(
            Fee_JobInfo.Fee_SwapJobInfo.Swap_HdrAddr - FEE_BLOCK_HEADER_SIZE,
            Fee_DataBuffer,
            FEE_VIRTUALPAGE_SIZE))
#else
    /* Pack valid flag to buffer */
    Fee_PackHdrValidOrInvalid(Fee_DataBuffer, FEE_VALID_VALUE);
    /* Write validation pattern to flash */
    if (((Std_ReturnType)E_OK)
        != Fls_Write(
            Fee_JobInfo.Fee_SwapJobInfo.Swap_HdrAddr - (2u * FEE_VIRTUALPAGE_SIZE),
            Fee_DataBuffer,
            FEE_VIRTUALPAGE_SIZE))
#endif
    {
        /* Fls write job hasn't been accepted */
        ret = MEMIF_JOB_FAILED;
    }
    else
    {
        /* Fls write job has been accepted */
        ret = MEMIF_JOB_PENDING;
    }
    /* Move on to next block */
    Fee_JobInfo.Fee_SwapJobInfo.Swap_BlockId += 1u;
    Fee_JobInfo.Fee_Job = FEE_JOB_INT_SWAP_BLOCK;
    return ret;
}
/*Write data from internal Fee buffer to target cluster*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobIntSwapDataWrite(void)
{
    MemIf_JobResultType ret;
    uint16              blockId;
    uint16              blockSize;
    blockId   = Fee_JobInfo.Fee_SwapJobInfo.Swap_BlockId;
    blockSize = Fee_ConfigStd->FeeBlockRef[blockId].FeeBlockSize;
    if (((Std_ReturnType)E_OK) == Fls_Write(Fee_JobInfo.Fee_SwapJobInfo.Swap_DataAddr, Fee_DataBuffer, blockSize))
    {
        /* Fls write job has been accepted */
        ret = MEMIF_JOB_PENDING;
    }
    else
    {
        /* Fls write job hasn't been accepted */
        ret = MEMIF_JOB_FAILED;
    }
    Fee_JobInfo.Fee_Job = FEE_JOB_INT_SWAP_BLOCK_VLD;
    return ret;
}
/*Write Fee block data to flash*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobWriteData(void)
{
    MemIf_JobResultType ret;
    uint16              blockSize;
    blockSize           = Fee_ConfigStd->FeeBlockRef[Fee_JobInfo.Fee_BlockId].FeeBlockSize;
    uint8 clusterGrpInx = Fee_ConfigStd->FeeBlockRef[Fee_JobInfo.Fee_BlockId].ClusterGrp;
    if (FALSE == Fee_ConfigStd->FeeBlockRef[Fee_JobInfo.Fee_BlockId].FeeImmediateData)
    {
        /*Header has been written so update both header and data addresses of the cluster*/
        Fee_ClusterGrpInfo[clusterGrpInx].Fee_BlockDataAddr -= blockSize;
        Fee_ClusterGrpInfo[clusterGrpInx].Fee_BlockHdrAddr += FEE_BLOCK_HEADER_SIZE;
        /* The block header has written,but the block data not write */
        Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_BlockStatus = FEE_BLOCK_INCONSISTENT;
        Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_DataAddr    = Fee_ClusterGrpInfo[clusterGrpInx].Fee_BlockDataAddr;
#if (STD_ON == FEE_PAGE_DIRECT_WRITE_SUPPORT)
        Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_HdrAddr =
            Fee_ClusterGrpInfo[clusterGrpInx].Fee_BlockHdrAddr - FEE_BLOCK_HEADER_SIZE;
#else
        Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_ValidHdrAddr =
            (Fee_ClusterGrpInfo[clusterGrpInx].Fee_BlockHdrAddr - (2u * FEE_VIRTUALPAGE_SIZE));
#endif
    }
    /*Write block data*/
    if (((Std_ReturnType)E_OK)
        == Fls_Write(
            Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_DataAddr,
            Fee_JobInfo.Fee_JobWriteDataDestPtr,
            blockSize))
    {
        /* Fls write job has been accepted */
        ret = MEMIF_JOB_PENDING;
    }
    else
    {
        /* Fls write job hasn't been accepted */
        ret = MEMIF_JOB_FAILED;
    }
    Fee_JobInfo.Fee_Job = FEE_JOB_WRITE_VALIDATE;
    return ret;
}
/*Validate Fee block by writing validation flag to flash*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobWriteValidate(void)
{
    MemIf_JobResultType ret;
#if (STD_ON == FEE_PAGE_DIRECT_WRITE_SUPPORT)
    /* Set valid flag to buffer */
    Fee_PackBlockHdrValidOrInvalid(Fee_DataBuffer, FEE_VALID_VALUE);
    /* Write validation pattern to flash */
    if (((Std_ReturnType)E_OK)
        == Fls_Write(Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_HdrAddr, Fee_DataBuffer, FEE_VIRTUALPAGE_SIZE))
#else
    /* Set valid flag to buffer */
    Fee_PackHdrValidOrInvalid(Fee_DataBuffer, FEE_VALID_VALUE);
    /* Write validation pattern to flash */
    if (((Std_ReturnType)E_OK)
        == Fls_Write(Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_ValidHdrAddr, Fee_DataBuffer, FEE_VIRTUALPAGE_SIZE))
#endif
    {
        /* Fls write job has been accepted */
        ret = MEMIF_JOB_PENDING;
    }
    else
    {
        /* Fls write job hasn't been accepted */
        ret = MEMIF_JOB_FAILED;
    }
    /* Switch the job to FEE_JOB_WRITE_DONE */
    Fee_JobInfo.Fee_Job = FEE_JOB_WRITE_DONE;
    return ret;
}
/*Write block done*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobWriteDone(void)
{
    /* Mark the Fee block as valid */
    Fee_BlockInfo[Fee_JobInfo.Fee_BlockId].Fee_BlockStatus = FEE_BLOCK_VALID;
    /* No more Fls jobs to schedule */
    Fee_JobInfo.Fee_Job = FEE_JOB_NONE;
    return MEMIF_JOB_OK;
}
/*cluster header get(success/failed) handle*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobIntScanClusterHdrGet(CONST(boolean, AUTOMATIC) BufferValid)
{
    MemIf_JobResultType ret;
    if (TRUE == BufferValid)
    {
        Fee_ClusterStatusType clusterStatus;
        uint32                clusterUseID;
        Fls_AddressType       clusterStartAddr;
        Fls_LengthType        clusterLength;
        /*analysis cluster header information*/
        clusterStatus = Fee_UnpackClusterHdr(&clusterUseID, &clusterStartAddr, &clusterLength, Fee_DataBuffer);
        if ((FEE_CLUSTER_VALID == clusterStatus)
            && (clusterStartAddr
                == Fee_ClusterGroupConfig[Fee_ClusterGroupIdx].ClusterPtr[Fee_ClusterIdx].FeeClusterStartAddr)
            && (clusterLength
                == Fee_ClusterGroupConfig[Fee_ClusterGroupIdx].ClusterPtr[Fee_ClusterIdx].FeeClusterLength)
            && (clusterUseID > Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_ClusterUseID))
        {
            /*Found valid and active cluster*/
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
    if (FEE_CLUSTER_GROUP_NUM == Fee_ClusterGroupIdx)
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
        /*scan all cluster done,Determine the existence of a valid cluster*/
        ret = Fee_JobIntScanClusterDone();
    }
    else
    {
        /*read the next cluster header*/
        ret = Fee_JobIntScanClusterHdrRead();
    }
    return ret;
}
/*scan all cluster done*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobIntScanClusterDone(void)
{
    MemIf_JobResultType ret;
    Fls_AddressType     clusterStartAddr;
    Fls_LengthType      clusterLen;
    if (FEE_CLUSTER_GROUP_NUM == Fee_ClusterGroupIdx)
    {
        /* No more cluster groups to scan */
        Fee_JobInfo.Fee_Job = FEE_JOB_NONE;
        ret                 = MEMIF_JOB_OK;
    }
    else
    {
        if (0u == Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_ClusterUseID)
        {
            /*No active cluster found,erase the first cluster*/
            Fee_ClusterIdx = 0;
            ret            = Fee_JobIntScanClusterErase();
        }
        else
        {
            /*Active cluster found*/
            uint8 ClusterIndex = Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_ValidClusterId;
            clusterStartAddr = Fee_ClusterGroupConfig[Fee_ClusterGroupIdx].ClusterPtr[ClusterIndex].FeeClusterStartAddr;
            clusterLen       = Fee_ClusterGroupConfig[Fee_ClusterGroupIdx].ClusterPtr[ClusterIndex].FeeClusterLength;
            Fee_BlockHeaderAddr                                       = clusterStartAddr + FEE_CLUSTER_HEADER_SIZE;
            Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_BlockHdrAddr  = Fee_BlockHeaderAddr;
            Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_BlockDataAddr = clusterStartAddr + clusterLen;
            /*Active cluster found so read the first block header*/
            ret = Fee_JobIntScanBlockHdrRead();
        }
    }
    return ret;
}
/*Read the Fee block header into internal buffer*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobIntScanBlockHdrRead(void)
{
    MemIf_JobResultType ret;
    /* Read the block header */
    if (((Std_ReturnType)E_OK) != Fls_Read(Fee_BlockHeaderAddr, Fee_DataBuffer, FEE_BLOCK_HEADER_SIZE))
    {
        /* Fls read job hasn't been accepted */
        ret = MEMIF_JOB_FAILED;
    }
    else
    {
        /* Fls read job has been accepted */
        ret = MEMIF_JOB_PENDING;
    }
    Fee_JobInfo.Fee_Job = FEE_JOB_INT_SCAN_BLOCK_HDR_GET;
    return ret;
}
/*Erase first Fee cluster*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobIntScanClusterErase(void)
{
    MemIf_JobResultType ret;
    Fls_AddressType     startAddr;
    Fls_LengthType      length;
    startAddr = Fee_ClusterGroupConfig[Fee_ClusterGroupIdx].ClusterPtr[Fee_ClusterIdx].FeeClusterStartAddr;
    length    = Fee_ClusterGroupConfig[Fee_ClusterGroupIdx].ClusterPtr[Fee_ClusterIdx].FeeClusterLength;
    if (((Std_ReturnType)E_OK) == Fls_Erase(startAddr, length))
    {
        /* Fls erase job has been accepted */
        ret = MEMIF_JOB_PENDING;
    }
    else
    {
        /* Fls erase job hasn't been accepted */
        ret = MEMIF_JOB_FAILED;
    }
    /* Schedule cluster clear job */
    Fee_JobInfo.Fee_Job = FEE_JOB_INT_SCAN_CLUSTER_CLEAR;
    return ret;
}
/*First cluster erase success,writing cluster header into flash*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobIntScanClusterClear(void)
{
    MemIf_JobResultType ret;
    Fls_AddressType     startAddr;
    Fls_LengthType      length;
    startAddr = Fee_ClusterGroupConfig[Fee_ClusterGroupIdx].ClusterPtr[Fee_ClusterIdx].FeeClusterStartAddr;
    length    = Fee_ClusterGroupConfig[Fee_ClusterGroupIdx].ClusterPtr[Fee_ClusterIdx].FeeClusterLength;
    /*The cluster useID valid from 1...(0:invalid)*/
    Fee_PackClusterHdr(1u, startAddr, length, Fee_DataBuffer);
    /*Set the cluster valid*/
    Fee_PackHdrValidOrInvalid(&Fee_DataBuffer[FEE_CLUSTER_HEADER_SIZE - FEE_VIRTUALPAGE_SIZE], FEE_VALID_VALUE);
    /* Write the cluster header to flash */
    if (((Std_ReturnType)E_OK) == Fls_Write(startAddr, Fee_DataBuffer, FEE_CLUSTER_HEADER_SIZE))
    {
        /* Fls read job has been accepted */
        ret = MEMIF_JOB_PENDING;
    }
    else
    {
        /* Fls write job hasn't been accepted */
        ret = MEMIF_JOB_FAILED;
    }
    /* Schedule cluster clear done job */
    Fee_JobInfo.Fee_Job = FEE_JOB_INT_SCAN_CLUSTER_CLEAR_DONE;
    return ret;
}
/*Fee cluster header parameters(except valid parameter) to write buffer*/
FEE_LOCAL FUNC(void, FEE_CODE) Fee_PackClusterHdr(
    CONST(uint32, AUTOMATIC) ClusterUseID,
    CONST(Fls_AddressType, AUTOMATIC) StartAddress,
    CONST(Fls_LengthType, AUTOMATIC) ClusterSize,
    P2VAR(uint8, AUTOMATIC, FEE_APPL_DATA) BufferPtr)
{
    uint8*       bufferPtr = BufferPtr;
    const uint8* endPtr    = &BufferPtr[FEE_CLUSTER_HEADER_SIZE];
    uint32       checkSum  = ClusterUseID + StartAddress + ClusterSize;
    FEE_SERIALIZE((ClusterUseID), uint32, (bufferPtr))
    FEE_SERIALIZE((StartAddress), Fls_AddressType, (bufferPtr))
    FEE_SERIALIZE((ClusterSize), Fls_LengthType, (bufferPtr))
    FEE_SERIALIZE((checkSum), uint32, (bufferPtr))
    /*Fill rest of the header with erase default value*/
    for (; bufferPtr < endPtr; bufferPtr++)
    {
        *bufferPtr = FEE_ERASED_VALUE;
    }
}
/*Unpack Fee cluster header parameters from read buffer*/
FEE_LOCAL FUNC(Fee_ClusterStatusType, FEE_CODE) Fee_UnpackClusterHdr(
    CONSTP2VAR(uint32, AUTOMATIC, FEE_APPL_DATA) UseID,
    CONSTP2VAR(Fls_AddressType, AUTOMATIC, FEE_APPL_DATA) StartAddress,
    CONSTP2VAR(Fls_LengthType, AUTOMATIC, FEE_APPL_DATA) ClusterSize,
    P2CONST(uint8, AUTOMATIC, FEE_APPL_CONST) BufferPtr)
{
    Fee_ClusterStatusType ret = FEE_CLUSTER_INVALID;
    uint32                checkSum;
    boolean               flagvalid                    = (boolean)FALSE;
    P2CONST(uint8, AUTOMATIC, FEE_APPL_CONST) startPtr = BufferPtr;
    P2CONST(uint8, AUTOMATIC, FEE_APPL_CONST) endPtr   = &BufferPtr[FEE_CLUSTER_HEADER_SIZE - FEE_VIRTUALPAGE_SIZE];
    FEE_DESERIALIZE((startPtr), (*UseID), uint32)
    FEE_DESERIALIZE((startPtr), (*StartAddress), Fls_AddressType)
    FEE_DESERIALIZE((startPtr), (*ClusterSize), Fls_LengthType)
    FEE_DESERIALIZE((startPtr), (checkSum), uint32)
    if ((*UseID + *StartAddress + *ClusterSize) != checkSum)
    {
        /* FEE_CLUSTER_INVALID */
    }
    else if (((Std_ReturnType)E_OK) != Fee_ClusterCheck(startPtr, endPtr))
    {
        /* FEE_CLUSTER_INVALID */
    }
    else if (((Std_ReturnType)E_OK) != Fee_UnpackHdrValidOrInvalid(endPtr, FEE_VALID_VALUE, &flagvalid))
    {
        /* FEE_CLUSTER_INVALID */
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
/*Block header valid or invalid flag pack*/
FEE_LOCAL FUNC(void, FEE_CODE)
    Fee_PackBlockHdrValidOrInvalid(P2VAR(uint8, AUTOMATIC, FEE_APPL_DATA) TargetPtr, CONST(uint8, AUTOMATIC) Flag)
{
    uint8* startPtr;
    uint8* endPtr = TargetPtr + FEE_VIRTUALPAGE_SIZE;
    for (startPtr = TargetPtr; startPtr < endPtr; startPtr++)
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
/*Unpack valid or invalid flag from read buffer*/
FEE_LOCAL FUNC(Std_ReturnType, FEE_CODE) Fee_UnpackBlockHdrValidOrInvalid(
    CONSTP2CONST(uint8, AUTOMATIC, FEE_APPL_CONST) TargetPtr,
    CONST(uint8, AUTOMATIC) Flag,
    P2VAR(boolean, AUTOMATIC, FEE_APPL_DATA) FlagValue)
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
        /*Never run to here,except for the interface call error*/
        ret = (Std_ReturnType)E_NOT_OK;
    }
    return ret;
}
#endif /*STD_ON == FEE_PAGE_DIRECT_WRITE_SUPPORT*/
/*Pack validation or invalidation flag to write buffer*/
FEE_LOCAL FUNC(void, FEE_CODE)
    Fee_PackHdrValidOrInvalid(P2VAR(uint8, AUTOMATIC, FEE_APPL_DATA) TargetPtr, CONST(uint8, AUTOMATIC) Flag)
{
    uint8*       startPtr = TargetPtr;
    const uint8* endPtr   = &TargetPtr[FEE_VIRTUALPAGE_SIZE];
    *startPtr             = Flag;
    for (startPtr++; startPtr < endPtr; startPtr++)
    {
        *startPtr = FEE_ERASED_VALUE;
    }
}
/*Unpack validation or invalidation flag from read buffer*/
FEE_LOCAL FUNC(Std_ReturnType, FEE_CODE) Fee_UnpackHdrValidOrInvalid(
    CONSTP2CONST(uint8, AUTOMATIC, FEE_APPL_CONST) TargetPtr,
    CONST(uint8, AUTOMATIC) Flag,
    P2VAR(boolean, AUTOMATIC, FEE_APPL_DATA) FlagValue)
{
    Std_ReturnType ret;
    if ((Flag == *TargetPtr) || (FEE_ERASED_VALUE == *TargetPtr))
    {
        if (Flag == *TargetPtr)
        {
            *FlagValue = (boolean)TRUE;
        }
        else
        {
            *FlagValue = (boolean)FALSE;
        }
        ret = Fee_ClusterCheck(&TargetPtr[1u], &TargetPtr[FEE_VIRTUALPAGE_SIZE]);
    }
    else
    {
        ret = (Std_ReturnType)E_NOT_OK;
    }
    return ret;
}
/*Initial Job finished(all block is invalid,the cluster is new)*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobIntScanClusterClearDone(void)
{
    /*Initial job finished,the active cluster is new,all block is invalid*/
    Fls_AddressType startAddr;
    Fls_LengthType  length;
    Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_ValidClusterId = Fee_ClusterIdx;
    Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_ClusterUseID   = 1u;
    startAddr = Fee_ClusterGroupConfig[Fee_ClusterGroupIdx].ClusterPtr[Fee_ClusterIdx].FeeClusterStartAddr;
    length    = Fee_ClusterGroupConfig[Fee_ClusterGroupIdx].ClusterPtr[Fee_ClusterIdx].FeeClusterLength;
    Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_BlockHdrAddr  = startAddr + FEE_CLUSTER_HEADER_SIZE;
    Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_BlockDataAddr = startAddr + length;
    Fee_ClusterGroupIdx++;
    MemIf_JobResultType RetVal = Fee_JobIntScanClusterDone();
    return RetVal;
}
/*Check whether specified data buffer contains only
 *the FEE_ERASED_VALUE value*/
FEE_LOCAL FUNC(Std_ReturnType, FEE_CODE) Fee_ClusterCheck(
    P2CONST(uint8, AUTOMATIC, FEE_APPL_CONST) StartPtr,
    CONSTP2CONST(uint8, AUTOMATIC, FEE_APPL_CONST) EndPtr)
{
    Std_ReturnType ret                                 = (Std_ReturnType)E_OK;
    P2CONST(uint8, AUTOMATIC, FEE_APPL_CONST) startPtr = StartPtr;
    for (; startPtr < EndPtr; startPtr++)
    {
        if (FEE_ERASED_VALUE == *startPtr)
        {
            /* value match */
        }
        else
        {
            ret = (Std_ReturnType)E_NOT_OK;
            break;
        }
    }
    return ret;
}
/*Initial job:scan the store block, have get the block header information*/
FEE_LOCAL FUNC(MemIf_JobResultType, FEE_CODE) Fee_JobIntScanBlockHdrGet(CONST(boolean, AUTOMATIC) BufferValid)
{
    MemIf_JobResultType ret;
    Fee_BlockStatusType blockStatus;
    Fls_AddressType     clusterEndAddr;
    uint16              blockNumber;
    uint16              blockSize;
    Fls_AddressType     dataAddr;
    boolean             immediateData;
    uint16              blockId;
    boolean             blockUnpackValid = (boolean)FALSE;
    blockStatus = Fee_UnpackBlockHdr(&blockNumber, &blockSize, &dataAddr, &immediateData, Fee_DataBuffer);
    if ((FEE_BLOCK_HEADER_INVALID == blockStatus) || ((boolean)FALSE == BufferValid))
    {
        clusterEndAddr = Fee_ClusterGroupConfig[Fee_ClusterGroupIdx]
                             .ClusterPtr[Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_ValidClusterId]
                             .FeeClusterStartAddr
                         + Fee_ClusterGroupConfig[Fee_ClusterGroupIdx]
                               .ClusterPtr[Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_ValidClusterId]
                               .FeeClusterLength;
        /*Avoid out-of-bounds access due to Header errors or read failures*/
        if ((Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_BlockHdrAddr + FEE_BLOCK_HEADER_SIZE) >= clusterEndAddr)
        {
            Fee_JobInfo.Fee_Job = FEE_JOB_NONE;
            ret                 = MEMIF_JOB_OK;
        }
        else
        {
            /* Move on to next block header */
            Fee_BlockHeaderAddr += FEE_BLOCK_HEADER_SIZE;
            Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_BlockHdrAddr = Fee_BlockHeaderAddr;
            /* Read next header */
            ret = Fee_JobIntScanBlockHdrRead();
        }
    }
    else if (FEE_BLOCK_HEADER_BLANK == blockStatus)
    {
        Fee_ClusterGroupIdx++;
        ret = Fee_JobIntScanClusterDone();
    }
    else
    {
        /* found block in the block configuration */
        blockId = Fee_GetBlockId(blockNumber);
        if (FEE_UNUSED_UINT16 != blockId)
        {
            if ((dataAddr <= (Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_BlockDataAddr - blockSize))
                && (dataAddr
                    >= (Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_BlockHdrAddr + (2u * FEE_BLOCK_HEADER_SIZE))))
            {
                if ((Fee_ConfigStd->FeeBlockRef[blockId].ClusterGrp == Fee_ClusterGroupIdx)
                    && (Fee_ConfigStd->FeeBlockRef[blockId].FeeBlockSize == blockSize)
                    && (Fee_ConfigStd->FeeBlockRef[blockId].FeeImmediateData == immediateData))
                {
                    blockUnpackValid = (boolean)TRUE;
                }
            }
        }
        /*Block information unpack success*/
        if ((boolean)TRUE == blockUnpackValid)
        {
            Fee_BlockInfo[blockId].Fee_BlockStatus = blockStatus;
            Fee_BlockInfo[blockId].Fee_DataAddr    = dataAddr;
#if (STD_ON == FEE_PAGE_DIRECT_WRITE_SUPPORT)
            Fee_BlockInfo[blockId].Fee_HdrAddr = Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_BlockHdrAddr;
#else
            Fee_BlockInfo[blockId].Fee_ValidHdrAddr =
                ((Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_BlockHdrAddr + FEE_BLOCK_HEADER_SIZE)
                 - (2u * FEE_VIRTUALPAGE_SIZE));
#endif
            Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_BlockDataAddr = dataAddr;
        }
        /* Move on to next block header */
        Fee_BlockHeaderAddr += FEE_BLOCK_HEADER_SIZE;
        Fee_ClusterGrpInfo[Fee_ClusterGroupIdx].Fee_BlockHdrAddr = Fee_BlockHeaderAddr;
        /* Read next header */
        ret = Fee_JobIntScanBlockHdrRead();
    }
    return ret;
}
/*Pack the block header information*/
FEE_LOCAL FUNC(void, FEE_CODE) Fee_PackBlockHdr(
    CONST(uint16, AUTOMATIC) BlockNumber,
    CONST(uint16, AUTOMATIC) Length,
    CONST(Fls_AddressType, AUTOMATIC) TargetAddress,
    CONST(boolean, AUTOMATIC) ImmediateBlock,
    P2VAR(uint8, AUTOMATIC, FEE_APPL_DATA) BlockHdrPtr)
{
    uint32       checkSum;
    uint8*       startPtr = BlockHdrPtr;
    const uint8* endPtr   = &startPtr[FEE_BLOCK_HEADER_SIZE];
    /* Calculate the block header checksum */
    checkSum = (uint32)BlockNumber + (uint32)Length + (uint32)TargetAddress;
    /* Use MSB of the checksum for the immediate block flag so the checksum is 31-bit */
    if (TRUE == ImmediateBlock)
    {
        checkSum += 1u;
        checkSum |= 0x80000000u;
    }
    else
    {
        checkSum &= 0x7fffffffu;
    }
    FEE_SERIALIZE((BlockNumber), uint16, (startPtr))
    FEE_SERIALIZE((Length), uint16, (startPtr))
    FEE_SERIALIZE((TargetAddress), Fls_AddressType, (startPtr))
    FEE_SERIALIZE((checkSum), uint32, (startPtr))
    for (; startPtr < endPtr; startPtr++)
    {
        *startPtr = FEE_ERASED_VALUE;
    }
}
/*Unpack the block header information*/
FEE_LOCAL FUNC(Fee_BlockStatusType, FEE_CODE) Fee_UnpackBlockHdr(
    CONSTP2VAR(uint16, AUTOMATIC, FEE_APPL_DATA) BlockNumber,
    CONSTP2VAR(uint16, AUTOMATIC, FEE_APPL_DATA) Length,
    CONSTP2VAR(Fls_AddressType, AUTOMATIC, FEE_APPL_DATA) TargetAddress,
    CONSTP2VAR(boolean, AUTOMATIC, FEE_APPL_DATA) ImmediateBlock,
    P2CONST(uint8, AUTOMATIC, FEE_APPL_CONST) BlockHdrPtr)
{
    Fee_BlockStatusType ret = FEE_BLOCK_HEADER_INVALID;
    uint32              readCheckSum;
    uint32              calcCheckSum;
    P2CONST(uint8, AUTOMATIC, FEE_APPL_CONST) endPtr;
    P2CONST(uint8, AUTOMATIC, FEE_APPL_CONST) blockHdrPtr = BlockHdrPtr;
    boolean flagValid                                     = (boolean)FALSE;
    boolean flagInvalid                                   = (boolean)FALSE;
    /*check the block header is blank or not*/
    if (Fee_ClusterCheck(blockHdrPtr, &blockHdrPtr[FEE_BLOCK_HEADER_SIZE]) == ((Std_ReturnType)E_OK))
    {
        ret = FEE_BLOCK_HEADER_BLANK;
    }
    else
    {
#if (STD_ON == FEE_PAGE_DIRECT_WRITE_SUPPORT)
        endPtr = &blockHdrPtr[FEE_BLOCK_HEADER_SIZE - 2u];
#else
        endPtr = &blockHdrPtr[FEE_BLOCK_HEADER_SIZE - (2u * FEE_VIRTUALPAGE_SIZE)];
#endif
        FEE_DESERIALIZE((blockHdrPtr), (*BlockNumber), uint16)
        FEE_DESERIALIZE((blockHdrPtr), (*Length), uint16)
        FEE_DESERIALIZE((blockHdrPtr), (*TargetAddress), Fls_AddressType)
        FEE_DESERIALIZE((blockHdrPtr), (readCheckSum), uint32)
        /* Use MSB of checksum for immediate block flag so
            the checksum is 31-bit long */
        calcCheckSum = (uint32)(*Length) + (uint32)(*TargetAddress) + (uint32)(*BlockNumber);
        if (0u == (readCheckSum & 0x80000000u))
        {
            *ImmediateBlock = (boolean)FALSE;
        }
        else
        {
            *ImmediateBlock = (boolean)TRUE;
            calcCheckSum += 1u;
        }
        /* Ignore MSB since it's used for ImmediateBlock flag */
        if ((readCheckSum & 0x7fffffffu) != (calcCheckSum & 0x7fffffffu))
        {
            /* FEE_BLOCK_HEADER_INVALID */
        }
#if (STD_ON == FEE_PAGE_DIRECT_WRITE_SUPPORT)
        else if (((Std_ReturnType)E_OK) != Fee_ClusterCheck(blockHdrPtr, endPtr))
        {
            /* FEE_BLOCK_HEADER_INVALID */
        }
        else if (((Std_ReturnType)E_OK) != Fee_UnpackBlockHdrValidOrInvalid(BlockHdrPtr, FEE_VALID_VALUE, &flagValid))
        {
            /* FEE_BLOCK_HEADER_INVALID */
        }
        else if (
            ((Std_ReturnType)E_OK) != Fee_UnpackBlockHdrValidOrInvalid(BlockHdrPtr, FEE_INVALID_VALUE, &flagInvalid))
        {
            /* FEE_BLOCK_HEADER_INVALID */
        }
#else
        else if (((Std_ReturnType)E_OK) != Fee_ClusterCheck(blockHdrPtr, endPtr))
        {
            /* FEE_BLOCK_HEADER_INVALID */
        }
        else if (((Std_ReturnType)E_OK) != Fee_UnpackHdrValidOrInvalid(endPtr, FEE_VALID_VALUE, &flagValid))
        {
            /* FEE_BLOCK_HEADER_INVALID */
        }
        else if (
            ((Std_ReturnType)E_OK)
            != Fee_UnpackHdrValidOrInvalid(&endPtr[FEE_VIRTUALPAGE_SIZE], FEE_INVALID_VALUE, &flagInvalid))
        {
            /* FEE_BLOCK_HEADER_INVALID */
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
    }
    return ret;
}
#define FEE_STOP_SEC_CODE
#include "Fee_MemMap.h"
/*******************************************************************************
**                            General Notes                                   **
*******************************************************************************/
