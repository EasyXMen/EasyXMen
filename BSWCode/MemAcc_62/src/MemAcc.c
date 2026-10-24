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
 **  @file               : MemAcc.c
 **  @author             : peng.wu
 **  @date               : 2025/04/04
 **  @vendor             : isoft
 **  @description        : Memory access code
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "MemAcc.h"
#include "SchM_MemAcc.h"
#if (MEMACC_FEATURE_MEMCOPY_USED_ILIB == STD_ON)
#include "IStdLib.h"
#endif
#include "MemAcc_Arti.h"

/* ===================================================== macros ===================================================== */
#define MEMACC_REQJOB_NUMBER 0x7u /** Defines the number of internal request job */

/* ================================================ type definitions ================================================ */
/**
 * @brief     This structure contains elements for accessing the Mem driver service functions and consistency
 * information
 */
typedef struct MemAcc_PendReqTypeTag
{
    uint8                     ReqJobRetries;   /**< Retry number of request job @range 0..255 */
    uint16                    SubAreaIndex;    /**< Sub address area index @range 0..0xFFFF */
    MemAcc_MemHwServiceIdType hwServiceId;     /**< Service Id of hardware @range 0..0xFFFFFFFF */
    MemAcc_AddressType        ReqAddress;      /**< Requested physic address of the driver @range NA */
    MemAcc_AddressType        ReqLogicAddress; /**< Logic address of requested address area @range NA */
    MemAcc_LengthType  DealSize;   /**< The size that needs to be processed when calling a driver once time @range NA */
    MemAcc_LengthType  DealOffset; /**< Offset that has been processed @range NA */
    MemAcc_LengthType  ReqlenRemain;        /**< Remain length that need to be processed @range NA */
    MemAcc_LengthType  Reqlength;           /**< Requested length by upper layer @range NA */
    MemAcc_LengthType* lengthPtr;           /**< Point to the length value @range NA */
    MemAcc_DataType*   DataBufPtr;          /**< Point to the data buffer @range NA */
    const MemAcc_DataType* DataWriteBufPtr; /**< Point to the data write buffer @range NA */
    const MemAcc_DataType* SrcBufPtr;       /**< Point to the source buffer @range NA */
} MemAcc_PendReqType;

/**
 * @brief     This structure contains elements for accessing the Mem driver service functions and consistency
 * information
 */
typedef struct MemAcc_RunTimeAreaTypeTag
{
    uint16                 ReqAreaAddrId;   /**< Request Address area Id @range 0..0xFFFF */
    MemAcc_JobResultType   CurJobResult;    /**< RunTime job result @range NA */
    MemAcc_JobType         CurJob;          /**< RunTime job type @range NA */
    MemAcc_JobStatusType   CurJobStatus;    /**< RunTime status of the module @range NA */
    MemAcc_ReqJobStateType CurReqJobStatus; /**< RunTime status of the request job @range NA */
    MemAcc_PendReqType     CurJobInfo;      /**< Information of the request pending job @range NA */
    MemAcc_LockInfoType    CurLockInfo;     /**< Lock information of the runtime job @range NA */
} MemAcc_RunTimeAreaType;

/**
 * @brief          Function pointer for the internal request job function
 * @param[in]      RuntimeJobInfo: Point to runtime job information
 * @reentrant      Non Reentrant
 * @synchronous    TRUE
 * @trace          CPD-PlaceHolder
 */
typedef void (*MemAcc_VoidReqJobFuncPtr)(MemAcc_RunTimeAreaType* RuntimeJobInfo);

/* ========================================== internal function declarations ======================================== */
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief       Set the data used in the module
 * @param[in]   data: the setted data value
 * @param[in]   length: The request length
 * @param[out]  destPtr: point to the destination
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-80687
 */
MEMACC_LOCAL void MemAcc_Memset(uint8* destPtr, const uint8 data, const MemAcc_LengthType length);
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"

/**
 * @brief       Check the delimiter field of the memory driver binary
 * @param[in]   MemDriverHeaderPtr: the setted data value
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-80688
 */
MEMACC_LOCAL boolean MemAcc_CheckMemDriverBinaryDF(const MemAcc_MemDriverHeaderType* MemDriverHeaderPtr);
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"

/**
 * @brief       Check the memory driver binary
 * @param[in]   HeaderAddress: the binary header address
 * @param[in]   HwId: the hardware Id
 * @return      boolean
 * @retval      TRUE: The deriver is valid
 * @retval      FALSE: The deriver is invalid
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-80689
 */
MEMACC_LOCAL boolean MemAcc_CheckMemDriverBinary(MemAcc_AddressType HeaderAddress, MemAcc_HwIdType HwId);
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#endif
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"

/**
 * @brief       Schedule the memory drivers services
 * @param[in]   RuntimeJobInfo: Point to the runtime job information
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing Failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-80690
 */
MEMACC_LOCAL Std_ReturnType MemAcc_ScheduleMemServices(MemAcc_RunTimeAreaType* RuntimeJobInfo);
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"

/**
 * @brief       Update the request driver address
 * @param[in]   RuntimeJobInfo: Point to the runtime job information
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-80691
 */
MEMACC_LOCAL void MemAcc_UpdateDriverAddress(MemAcc_RunTimeAreaType* RuntimeJobInfo);
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"

/**
 * @brief       Update SubAdress area
 * @param[in]   RuntimeJobInfo: Point to the runtime job information
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-80692
 */
MEMACC_LOCAL void MemAcc_UpdateSubArea(MemAcc_RunTimeAreaType* RuntimeJobInfo);
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"

/**
 * @brief       Request memory driver service
 * @param[in]   RuntimeJobInfo: Point to the runtime job information
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-80693
 */
MEMACC_LOCAL void MemAcc_InvocationMemService(MemAcc_RunTimeAreaType* RuntimeJobInfo);
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"

/**
 * @brief       Get the memory driver requested job result
 * @param[in]   RuntimeJobInfo: Point to the runtime job information
 * @return      MemAcc_MemJobResultType
 * @retval      MEM_JOB_OK: The last job has been finished successfully
 * @retval      MEM_JOB_PENDING: A job is currently being processed
 * @retval      MEM_JOB_FAILED: Job failed for some unspecific reason
 * @retval      MEM_INCONSISTENT: The checked page is not blank
 * @retval      MEM_ECC_UNCORRECTED: Uncorrectable ECC errors occurred during memory access
 * @retval      MEM_ECC_CORRECTED: Correctable ECC errors occurred during memory access
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-80694
 */
MEMACC_LOCAL MemAcc_MemJobResultType MemAcc_GetMemJobResult(const MemAcc_RunTimeAreaType* RuntimeJobInfo);
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"

/**
 * @brief       Request job is ready
 * @param[in]   RuntimeJobInfo: Point to the runtime job information
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-80695
 */
MEMACC_LOCAL void MemAcc_ReqJobReady(MemAcc_RunTimeAreaType* RuntimeJobInfo);
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"

/**
 * @brief       Request job is processing
 * @param[in]   RuntimeJobInfo: Point to the runtime job information
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-80696
 */
MEMACC_LOCAL void MemAcc_ReqJobProcess(MemAcc_RunTimeAreaType* RuntimeJobInfo);
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"

/**
 * @brief       Request job is fail, retry it
 * @param[in]   RuntimeJobInfo: Point to the runtime job information
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-80697
 */
MEMACC_LOCAL void MemAcc_ReqJobRetry(MemAcc_RunTimeAreaType* RuntimeJobInfo);
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"

/**
 * @brief       Suspend the request job
 * @param[in]   RuntimeJobInfo: Point to the runtime job information
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-80698
 */
MEMACC_LOCAL void MemAcc_ReqJobSuspend(MemAcc_RunTimeAreaType* RuntimeJobInfo);
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"

/**
 * @brief       Resume the request job
 * @param[in]   RuntimeJobInfo: Point to the runtime job information
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-80699
 */
MEMACC_LOCAL void MemAcc_ReqJobResume(MemAcc_RunTimeAreaType* RuntimeJobInfo);
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"

/**
 * @brief       Cancel the request job
 * @param[in]   RuntimeJobInfo: Point to the runtime job information
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-80700
 */
MEMACC_LOCAL void MemAcc_ReqJobCancel(MemAcc_RunTimeAreaType* RuntimeJobInfo);
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"

/**
 * @brief       Schedule the request job
 * @param[in]   RuntimeJobInfo: Point to the runtime job information
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-80701
 */
MEMACC_LOCAL void MemAcc_ReqJobSchedule(MemAcc_RunTimeAreaType* RuntimeJobInfo);
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"

/**
 * @brief       Check the mem driver is available
 * @param[in]   RuntimeJobInfo: Point to the runtime job information
 * @return      boolean
 * @retval      TRUE: The resource is valid
 * @retval      FALSE: The resource is invalid
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-80702
 */
MEMACC_LOCAL boolean MemAcc_CheckMemResource(const MemAcc_RunTimeAreaType* RuntimeJobInfo);
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"

/**
 * @brief       Resolve the conflicting mem driver
 * @param[in]   runningAreaId: The currently running area Id
 * @param[in]   reqCurAreaId: The currently requested area Id
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-80703
 */
MEMACC_LOCAL void MemAcc_ProcessMemPriority(const uint16 runningAreaId, const uint16 reqCurAreaId);
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"

/**
 * @brief       Notify the upper level when the job is completed
 * @param[in]   RuntimeJobInfo: Point to the runtime job information
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-80704
 */
MEMACC_LOCAL void MemAcc_JobEndNotification(const MemAcc_RunTimeAreaType* RuntimeJobInfo);
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"

/**
 * @brief       Calculate the operation length of the requested job
 * @param[in]   RuntimeJobInfo: Point to the runtime job information
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-80705
 */
MEMACC_LOCAL void MemAcc_CalculateLength(MemAcc_RunTimeAreaType* RuntimeJobInfo);
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#if (MEMACC_COMPARE_API == STD_ON)
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"

/**
 * @brief       Compare the data
 * @param[in]   SourcePtr: Point to the source data
 * @param[in]   DestPtr: Point to the destination data
 * @param[in]   length: Requested length
 * @return      boolean
 * @retval      TRUE: The data is valid
 * @retval      FALSE: The data is invalid
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-80706
 */
MEMACC_LOCAL boolean
    MemAcc_CompareData(const MemAcc_DataType* SourcePtr, const MemAcc_DataType* DestPtr, MemAcc_LengthType length);
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#endif
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"

/**
 * @brief       Update the length of the request job and the location of the buffer
 * @param[in]   RuntimeJobInfo: Point to the runtime job information
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-80707
 */
MEMACC_LOCAL void MemAcc_UpdateLength(MemAcc_RunTimeAreaType* RuntimeJobInfo);
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"

/**
 * @brief       The requested job was processed successfully
 * @param[in]   RuntimeJobInfo: Point to the runtime job information
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-80708
 */
MEMACC_LOCAL void MemAcc_JobProcessingOk(MemAcc_RunTimeAreaType* RuntimeJobInfo);
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"

/**
 * @brief       Suspend successfully
 * @param[in]   RuntimeJobInfo: Point to the runtime job information
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-80710
 */
MEMACC_LOCAL void MemAcc_JobSuspendingOk(MemAcc_RunTimeAreaType* RuntimeJobInfo);
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"

/**
 * @brief       Suspend failed
 * @param[in]   RuntimeJobInfo: Point to the runtime job information
 * @param[out]  MemJobResult: Memory driver processed job result
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-80711
 */
MEMACC_LOCAL void
    MemAcc_JobSuspendingFail(MemAcc_RunTimeAreaType* RuntimeJobInfo, MemAcc_MemJobResultType MemJobResult);
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#if (MEMACC_MEMDRIVER_DIRECT_STATIC_ENABLE != STD_ON)
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"

/**
 * @brief       Periodic scheduling mem drive
 * @param[in]   RuntimeJobInfo: Point to the runtime job information
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-80712
 */
MEMACC_LOCAL void MemAcc_ScheMemDriMainFuc(const MemAcc_RunTimeAreaType* RuntimeJobInfo);
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#endif
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"

/**
 * @brief       Update lock information
 * @param[in]   AddrAreaId: Numeric identifier of address area
 * @param[in]   preLockInfo: Point to the previous lock information
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-80713
 */
MEMACC_LOCAL void
    MemAcc_UpdateLockInfo(const MemAcc_AddressAreaIdType AddrAreaId, const MemAcc_LockInfoType* preLockInfo);
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"

/* ============================================ internal data definitions =========================================== */
#define MEMACC_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "MemAcc_MemMap.h"
/**
 * @brief Initialization status of the module
 * @range 0..2
 */
MEMACC_LOCAL MemAcc_StatusType MemAcc_State;
#define MEMACC_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "MemAcc_MemMap.h"

#define MEMACC_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "MemAcc_MemMap.h"
/**
 * @brief Runtime area information
 * @range NA
 */
MEMACC_LOCAL MemAcc_RunTimeAreaType MemAcc_RunTimeArea[MEMACC_MAX_ADDRAREA_NUM];
#define MEMACC_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "MemAcc_MemMap.h"

#define MEMACC_START_SEC_VAR_CLEARED_16
#include "MemAcc_MemMap.h"
/**
 * @brief Runtime status of the Mem driver
 * @range NA
 */
MEMACC_LOCAL uint16 MemAcc_ReqMemHw[MEMACC_DRIVER_NUM];
#define MEMACC_STOP_SEC_VAR_CLEARED_16
#include "MemAcc_MemMap.h"

#define MEMACC_START_SEC_VAR_CLEARED_PTR
#include "MemAcc_MemMap.h"
/**
 * @brief Point to the configuration infotmation
 * @range NA
 */
MEMACC_LOCAL const MemAcc_ConfigType* MemAcc_CfgPtr;
#define MEMACC_STOP_SEC_VAR_CLEARED_PTR
#include "MemAcc_MemMap.h"

#define MEMACC_START_SEC_CONST_UNSPECIFIED
#include "MemAcc_MemMap.h"
/**
 * @brief Internal runtime request job
 * @range NA
 */
/* PRQA S 3218 ++ */ /* VL_MemAcc_3218 */
MEMACC_LOCAL const MemAcc_VoidReqJobFuncPtr MemAcc_ReqJobFunc[MEMACC_REQJOB_NUMBER] = {
    NULL_PTR,             /** No request job or job is stopped */
    MemAcc_ReqJobReady,   /** Job is ready */
    MemAcc_ReqJobProcess, /** Job is processing */
    MemAcc_ReqJobRetry,   /** Retry the job */
    MemAcc_ReqJobSuspend, /** Suspend the job */
    MemAcc_ReqJobResume,  /** Resume the job */
    MemAcc_ReqJobCancel   /** Cancel the job */
};
/* PRQA S 3218 -- */
#define MEMACC_STOP_SEC_CONST_UNSPECIFIED
#include "MemAcc_MemMap.h"

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
/**
 * @brief Check the configuration pointer, initialize the variables and initialize the mem driver
 */
void MemAcc_Init(const MemAcc_ConfigType* configPtr)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Start, 2, 0x01, configPtr);
#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
    if (NULL_PTR == configPtr)
    {
        MemAcc_Det_ReportError(MEMACC_SID_INIT, MEMACC_E_PARAM_POINTER);
    }
    else
#endif
    {
        MemAcc_CfgPtr = configPtr;
        /** Initialize all variables of area */
        for (uint16 iloop = 0u; iloop < MemAcc_CfgPtr->AddrAreaCfgNum; iloop++)
        {
            MemAcc_RunTimeAreaType* curAreaPtr = &(MemAcc_RunTimeArea[iloop]);
            MemAcc_PendReqType*     jobInfoPtr = &(curAreaPtr->CurJobInfo);
            curAreaPtr->CurJob                 = MEMACC_NO_JOB;
            curAreaPtr->CurJobResult           = MEMACC_OK;
            curAreaPtr->CurJobStatus           = MEMACC_JOB_IDLE;
            jobInfoPtr->DataBufPtr             = NULL_PTR;
            jobInfoPtr->DataWriteBufPtr        = NULL_PTR;
            jobInfoPtr->lengthPtr              = NULL_PTR;
        }

        /** Update hardware status */
        for (uint16 index = 0u; index < (uint16)MEMACC_DRIVER_NUM; index++)
        {
            MemAcc_ReqMemHw[index] = MEMACC_MEM_DRIVER_IDLE;
        }

        /** Initialize all Mem drivers with INDIRECT STATIC type */
        for (uint8 index = 0u; index < (uint16)MEMACC_DRIVER_NUM; ++index)
        {
            if (MEMACC_INDIRECT_STATIC == MemAcc_InvocationCfg[index])
            {
                /** Initialize the Mem drivers */
                MemAcc_MemDriverApi[index].InitFunc(NULL_PTR);
            }
        }

        MemAcc_State = MEMACC_INIT;
    }
    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Return, 1, 0x01);
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief De-Initialize the variables and the mem driver
 */
void MemAcc_DeInit(void)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Start, 1, 0x13);
#if (STD_ON == MEMACC_DEV_ERROR_DETECT)
    if (MEMACC_INIT != MemAcc_State)
    {
        MemAcc_Det_ReportError((uint8)MEMACC_SID_DEINIT, MEMACC_E_UNINIT);
    }
    else
#endif
    {
        /** Reset all variables of area */
        MemAcc_Memset((uint8*)MemAcc_RunTimeArea, 0u, sizeof(MemAcc_RunTimeArea));

        for (uint8 index = 0u; index < (uint16)MEMACC_DRIVER_NUM; ++index)
        {
            if (MEMACC_INDIRECT_STATIC == MemAcc_InvocationCfg[index])
            {
                /** Perform the initialization of Mem drivers with MEMACC_INDIRECT_STATIC type */
                MemAcc_MemDriverApi[index].DeInitFunc();
            }
        }

        MemAcc_CfgPtr = NULL_PTR;
        MemAcc_State  = MEMACC_DEINIT;
    }
    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Return, 1, 0x13);
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Check the version pointer, get the software version information
 */
void MemAcc_GetVersionInfo(Std_VersionInfoType* versionInfoPtr)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Start, 2, 0x02, versionInfoPtr);
#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
    if (NULL_PTR == versionInfoPtr)
    {
        MemAcc_Det_ReportError(MEMACC_SID_GETVERSIONINFO, MEMACC_E_PARAM_POINTER);
    }
    else
#endif
    {
        versionInfoPtr->vendorID         = MEMACC_VENDOR_ID;
        versionInfoPtr->moduleID         = MEMACC_MODULE_ID;
        versionInfoPtr->sw_major_version = MEMACC_SW_MAJOR_VERSION;
        versionInfoPtr->sw_minor_version = MEMACC_SW_MINOR_VERSION;
        versionInfoPtr->sw_patch_version = MEMACC_SW_PATCH_VERSION;
    }
    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Return, 1, 0x02);
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Check the initialization status and the validity of the area ID, then get the job result
 */
MemAcc_JobResultType MemAcc_GetJobResult(MemAcc_AddressAreaIdType addressAreaId)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Start, 2, 0x05, addressAreaId);
    MemAcc_JobResultType ret = MEMACC_FAILED;
#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = MEMACC_E_NO_ERROR;
    if (MEMACC_INIT != MemAcc_State)
    {
        errorId = MEMACC_E_UNINIT;
    }
    else if (addressAreaId >= MemAcc_CfgPtr->AddrAreaCfgNum)
    {
        errorId = MEMACC_E_PARAM_ADDRESS_AREA_ID;
    }
    else
#endif
    {
        /*If a MemAcc job is still pending, the API returns the result of the last MemAcc job */
        ret = MemAcc_RunTimeArea[addressAreaId].CurJobResult;
    }

#if (STD_ON == MEMACC_DEV_ERROR_DETECT)
    if (errorId != (uint8)MEMACC_E_NO_ERROR)
    {
        MemAcc_Det_ReportError((uint8)MEMACC_SID_GETJOBRESULT, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Return, 2, 0x05, ret);
    return ret;
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Check the initialization status and the validity of the area ID, then get the job status
 */
MemAcc_JobStatusType MemAcc_GetJobStatus(MemAcc_AddressAreaIdType addressAreaId)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Start, 2, 0x10, addressAreaId);
    MemAcc_JobStatusType ret = MEMACC_JOB_IDLE;
#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = MEMACC_E_NO_ERROR;
    if (MEMACC_INIT != MemAcc_State)
    {
        errorId = MEMACC_E_UNINIT;
    }
    else if (addressAreaId >= MemAcc_CfgPtr->AddrAreaCfgNum)
    {
        errorId = MEMACC_E_PARAM_ADDRESS_AREA_ID;
    }
    else
#endif
    {
        ret = MemAcc_RunTimeArea[addressAreaId].CurJobStatus;
    }

#if (STD_ON == MEMACC_DEV_ERROR_DETECT)
    if (errorId != (uint8)MEMACC_E_NO_ERROR)
    {
        MemAcc_Det_ReportError((uint8)MEMACC_SID_GETJOBSTATUS, errorId);
    }
#endif

    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Return, 2, 0x10, ret);
    return ret;
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Check the initialization status and the validity of the input parameters, then get the memory information
 */
/* PRQA S 6030 ++ */ /* VL_MTR_MemAcc_STMIF */
Std_ReturnType MemAcc_GetMemoryInfo(
    MemAcc_AddressAreaIdType addressAreaId,
    MemAcc_AddressType       address,
    MemAcc_MemoryInfoType*   memoryInfoPtr)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Start, 4, 0x06, addressAreaId, address, memoryInfoPtr);
    Std_ReturnType ret = E_NOT_OK;

#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = MEMACC_E_NO_ERROR;
    if (MEMACC_INIT != MemAcc_State)
    {
        errorId = MEMACC_E_UNINIT;
    }
    else if (addressAreaId >= MemAcc_CfgPtr->AddrAreaCfgNum)
    {
        errorId = MEMACC_E_PARAM_ADDRESS_AREA_ID;
    }
    else if (NULL_PTR == memoryInfoPtr)
    {
        errorId = MEMACC_E_PARAM_POINTER;
    }
    else
#endif
    {
        const MemAcc_AddrAreaType*    addrAreaPtr  = &(MemAcc_CfgPtr->AddrAreaCfg[addressAreaId]);
        uint16                        firstSubArea = addrAreaPtr->FirstSubAddrArea;
        uint16                        lastSubArea  = addrAreaPtr->LastSubAddrArea;
        const MemAcc_SubAddrAreaType* pSubAreaCfg  = &(MemAcc_CfgPtr->SubAddrAreaCfg[firstSubArea]);

        for (uint16 index = firstSubArea; (index <= lastSubArea) && (ret == (Std_ReturnType)E_NOT_OK); index++)
        {
            if ((address >= pSubAreaCfg->LogiStartAddr)
                && (address < (pSubAreaCfg->LogiStartAddr + pSubAreaCfg->LogiLength)))
            {
                uint16                   secBatIndex = pSubAreaCfg->SecBatchIndex;
                const MemAcc_SecBatType* SecBatPtr   = &(MemAcc_CfgPtr->SectorBatchRef[secBatIndex]);
                memoryInfoPtr->LogiStartAddr         = pSubAreaCfg->LogiStartAddr;
                memoryInfoPtr->PhysicalStartAddress  = SecBatPtr->MemStartAddr;
                memoryInfoPtr->EraseSectorSize       = SecBatPtr->MemEraseSectorSize;
                memoryInfoPtr->WritePageSize         = SecBatPtr->MemWritePageSize;
                memoryInfoPtr->MinReadSize           = SecBatPtr->MemMinReadSize;
                memoryInfoPtr->EraseSectorBurstSize  = SecBatPtr->MemEraseBurstSize;
                memoryInfoPtr->WritePageBurstSize    = SecBatPtr->MemWriteBurstSize;
                memoryInfoPtr->MaxReadSize           = SecBatPtr->MemMaxReadSize;
                memoryInfoPtr->MaxOffset             = pSubAreaCfg->LogiLength;
                memoryInfoPtr->HwId                  = pSubAreaCfg->HardwareId;
                ret                                  = E_OK;
            }
            pSubAreaCfg++;
        }
    }

#if (STD_ON == MEMACC_DEV_ERROR_DETECT)
    if (errorId != (uint8)MEMACC_E_NO_ERROR)
    {
        MemAcc_Det_ReportError((uint8)MEMACC_SID_GETMEMORYINFO, errorId);
    }
#endif

    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Return, 2, 0x06, ret);
    return ret;
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
/* PRQA S 6030 -- */
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Check the initialization status and the validity of the area ID, then get the processed length
 */
MemAcc_LengthType MemAcc_GetProcessedLength(MemAcc_AddressAreaIdType addressAreaId)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Start, 2, 0x07, addressAreaId);
    MemAcc_LengthType ret = 0u;
#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = MEMACC_E_NO_ERROR;
    if (MEMACC_INIT != MemAcc_State)
    {
        errorId = MEMACC_E_UNINIT;
    }
    else if (addressAreaId >= MemAcc_CfgPtr->AddrAreaCfgNum)
    {
        errorId = MEMACC_E_PARAM_ADDRESS_AREA_ID;
    }
    else
#endif
    {
        MemAcc_PendReqType reqJobRuntime = MemAcc_RunTimeArea[addressAreaId].CurJobInfo;
        /** Calculate the length that has been processed */
        ret = reqJobRuntime.Reqlength - reqJobRuntime.ReqlenRemain;
    }

#if (STD_ON == MEMACC_DEV_ERROR_DETECT)
    if (errorId != (uint8)MEMACC_E_NO_ERROR)
    {
        MemAcc_Det_ReportError((uint8)MEMACC_SID_GETPROCESSEDLENGTH, errorId);
    }
#endif

    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Return, 2, 0x07, ret);
    return ret;
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Check the initialization status and the validity of the input parameters, then get the job information
 */
/* PRQA S 6030 ++ */ /* VL_MTR_MemAcc_STMIF */
void MemAcc_GetJobInfo(MemAcc_AddressAreaIdType addressAreaId, MemAcc_JobInfoType* jobInfoPtr)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Start, 3, 0x08, addressAreaId, jobInfoPtr);
#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = MEMACC_E_NO_ERROR;
    if (MEMACC_INIT != MemAcc_State)
    {
        errorId = MEMACC_E_UNINIT;
    }
    else if (addressAreaId >= MemAcc_CfgPtr->AddrAreaCfgNum)
    {
        errorId = MEMACC_E_PARAM_ADDRESS_AREA_ID;
    }
    else if (NULL_PTR == jobInfoPtr)
    {
        errorId = MEMACC_E_PARAM_POINTER;
    }
    else
#endif
    {
        MemAcc_RunTimeAreaType        runningArea = MemAcc_RunTimeArea[addressAreaId];
        MemAcc_PendReqType            pendingJob  = runningArea.CurJobInfo;
        MemAcc_LockInfoType           lockInfo    = runningArea.CurLockInfo;
        uint16                        saIndex     = pendingJob.SubAreaIndex;
        const MemAcc_SubAddrAreaType* subAreaPtr  = MemAcc_CfgPtr->SubAddrAreaCfg;
        uint16                        secBatIndex = subAreaPtr[saIndex].SecBatchIndex;
        const MemAcc_SecBatType*      secBatPtr   = &(MemAcc_CfgPtr->SectorBatchRef[secBatIndex]);

        jobInfoPtr->MemAddress     = pendingJob.ReqAddress;
        jobInfoPtr->LogicalAddress = pendingJob.ReqLogicAddress;
        jobInfoPtr->Length         = pendingJob.Reqlength;
        jobInfoPtr->CurrentJob     = ((runningArea.CurJob != MEMACC_NO_JOB) || (lockInfo.JobLocked == FALSE))
                                         ? runningArea.CurJob
                                         : MEMACC_REQUESTLOCK_JOB;
        jobInfoPtr->MemResult      = runningArea.CurJobResult;
        jobInfoPtr->MemInstanceId  = secBatPtr->MemInstanceId;
        jobInfoPtr->MemLength      = secBatPtr->MemLength;
        jobInfoPtr->HwId           = subAreaPtr->HardwareId;
    }

#if (STD_ON == MEMACC_DEV_ERROR_DETECT)
    if (errorId != (uint8)MEMACC_E_NO_ERROR)
    {
        MemAcc_Det_ReportError((uint8)MEMACC_SID_GETJOBINFO, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Return, 1, 0x08);
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
/* PRQA S 6030 -- */
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Check the initialization status and the validity of the input parameters, then active the Mem driver
 */
Std_ReturnType MemAcc_ActivateMem(MemAcc_AddressType headerAddress, MemAcc_HwIdType hwId)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Start, 3, 0x14, headerAddress, hwId);
    Std_ReturnType ret = E_NOT_OK;
#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = MEMACC_E_NO_ERROR;
    if (MEMACC_INIT != MemAcc_State)
    {
        errorId = MEMACC_E_UNINIT;
    }
    else if (MemAcc_CheckMemDriverBinary(headerAddress, hwId) == FALSE)
    {
        errorId = MEMACC_E_MEM_INIT_FAILED;
    }
    else
#endif
    {
        if (MemAcc_InvocationCfg[hwId] == MEMACC_INDIRECT_DYNAMIC)
        {
            /* PRQA S 0306 ++ */ /* VL_MemAcc_0306 */
            const MemAcc_MemBinaryHeaderType* apiPtr =
                (const MemAcc_MemBinaryHeaderType*)((MemAcc_UintPtrType)headerAddress);
            /* PRQA S 0306 -- */

            MemAcc_MemBinaryHeaderType* memApi = &MemAcc_MemDriverApi[hwId];
            memApi->InitFunc                   = apiPtr->InitFunc;
            memApi->DeInitFunc                 = apiPtr->DeInitFunc;
#if (MEMACC_MEMDRIVER_DIRECT_STATIC_ENABLE != STD_ON)
            memApi->MainFunc = apiPtr->MainFunc;
#endif
            memApi->GetJobResultFunc      = apiPtr->GetJobResultFunc;
            memApi->ReadFunc              = apiPtr->ReadFunc;
            memApi->ReadFunc              = apiPtr->ReadFunc;
            memApi->EraseFunc             = apiPtr->EraseFunc;
            memApi->BlankCheckFunc        = apiPtr->BlankCheckFunc;
            memApi->PropagateErrorFunc    = apiPtr->PropagateErrorFunc;
            memApi->SuspendFunc           = apiPtr->SuspendFunc;
            memApi->ResumeFunc            = apiPtr->ResumeFunc;
            memApi->HwSpecificServiceFunc = apiPtr->HwSpecificServiceFunc;

            /** Active the Mem driver */
            memApi->InitFunc(NULL_PTR);
            ret = E_OK;
        }
    }

#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
    if (errorId != (uint8)MEMACC_E_NO_ERROR)
    {
        MemAcc_Det_ReportError(MEMACC_SID_ACTIVATEMEM, errorId);
    }
#endif

    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Return, 2, 0x14, ret);
    return ret;
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Check the initialization status, then deactive the Mem driver
 */
Std_ReturnType MemAcc_DeactivateMem(MemAcc_HwIdType hwId, MemAcc_AddressType headerAddress)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Start, 3, 0x15, headerAddress, hwId);
    Std_ReturnType ret = E_NOT_OK;
#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
    if (MEMACC_INIT != MemAcc_State)
    {
        MemAcc_Det_ReportError(MEMACC_SID_DEACTIVATEMEM, MEMACC_E_UNINIT);
    }
    else
#endif
    {
        boolean jobPending = FALSE;
        /** Check HwId and invocation */
        if ((hwId < (uint16)MEMACC_DRIVER_NUM) && (MEMACC_INDIRECT_DYNAMIC == MemAcc_InvocationCfg[hwId]))
        {
            for (uint16 index = 0u; index < MemAcc_CfgPtr->AddrAreaCfgNum; ++index)
            {
                /** Check if the job is still pending */
                if (MEMACC_JOB_PENDING == MemAcc_RunTimeArea[index].CurJobStatus)
                {
                    jobPending = TRUE;
                    break;
                }
            }
            if (FALSE == jobPending)
            {
                /** De-initial the Mem driver */
                MemAcc_MemDriverApi[hwId].DeInitFunc();
                ret = E_OK;
            }
        }
    }
    MEMACC_PARAM_UNUSED(headerAddress);
    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Return, 2, 0x15, ret);
    return ret;
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Check the initialization status and the validity of the area ID, if the request job status is pending, cancel
 * the job
 */
void MemAcc_Cancel(MemAcc_AddressAreaIdType addressAreaId)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Start, 2, 0x04, addressAreaId);
#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = MEMACC_E_NO_ERROR;
    if (MEMACC_INIT != MemAcc_State)
    {
        errorId = MEMACC_E_UNINIT;
    }
    else if (addressAreaId >= MemAcc_CfgPtr->AddrAreaCfgNum)
    {
        errorId = MEMACC_E_PARAM_ADDRESS_AREA_ID;
    }
    else
#endif
    {
        /** Check if the job is pending */
        MemAcc_RunTimeAreaType* curAreaPtr = &(MemAcc_RunTimeArea[addressAreaId]);
        if (MEMACC_JOB_PENDING == curAreaPtr->CurJobStatus)
        {
            /** It shall wait for the completion of a pending job and stop further processing of the current job */
            switch (curAreaPtr->CurReqJobStatus)
            {
            case MEMACC_REQJOB_STARTING:
            case MEMACC_REQJOB_SUSPENDING:
                /** No job is pending, the job is cancelled */
                curAreaPtr->CurJob          = MEMACC_NO_JOB;
                curAreaPtr->CurReqJobStatus = MEMACC_REQJOB_STOP;
                curAreaPtr->CurJobStatus    = MEMACC_JOB_IDLE;
                curAreaPtr->CurJobResult    = MEMACC_CANCELED;
                break;
            case MEMACC_REQJOB_PROCESSING:
            case MEMACC_REQJOB_RETRYING:
            case MEMACC_REQJOB_RESUMING:
            case MEMACC_REQJOB_CANCELING:
                /** The job is still pending, waiting for further processing */
                curAreaPtr->CurReqJobStatus = MEMACC_REQJOB_CANCELING;
                break;
            case MEMACC_REQJOB_STOP:
            default:
                /** Do nothing */
                break;
            }
        }
    }

#if (STD_ON == MEMACC_DEV_ERROR_DETECT)
    if (errorId != (uint8)MEMACC_E_NO_ERROR)
    {
        MemAcc_Det_ReportError((uint8)MEMACC_SID_CANCEL, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Return, 1, 0x04);
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Check the initialization status and the validity of the input parameters, then request the read job
 */
/* PRQA S 6030 ++ */ /* VL_MTR_MemAcc_STMIF */
Std_ReturnType MemAcc_Read(
    MemAcc_AddressAreaIdType addressAreaId,
    MemAcc_AddressType       sourceAddress,
    MemAcc_DataType*         destinationDataPtr,
    MemAcc_LengthType        length)
{
    ARTI_TRACE_N(
        USER,
        AR_CP_BSW_API,
        MemAcc,
        0,
        Bsw_Start,
        5,
        0x09,
        addressAreaId,
        sourceAddress,
        destinationDataPtr,
        length);
    Std_ReturnType ret = E_NOT_OK;
#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = MEMACC_E_NO_ERROR;
    if (MEMACC_INIT != MemAcc_State)
    {
        errorId = MEMACC_E_UNINIT;
    }
    else if (addressAreaId >= MemAcc_CfgPtr->AddrAreaCfgNum)
    {
        errorId = MEMACC_E_PARAM_ADDRESS_AREA_ID;
    }
    else if (NULL_PTR == destinationDataPtr)
    {
        errorId = MEMACC_E_PARAM_POINTER;
    }
    else if (MEMACC_NO_JOB != MemAcc_RunTimeArea[addressAreaId].CurJob)
    {
        errorId = MEMACC_E_BUSY;
    }
    else
#endif
    {
        SchM_Enter_MemAcc_Queue();
        MemAcc_RunTimeAreaType*       curAreaPtr   = &(MemAcc_RunTimeArea[addressAreaId]);
        MemAcc_LockInfoType           preLockInfo  = curAreaPtr->CurLockInfo;
        const MemAcc_AddrAreaType*    addrAreaPtr  = &(MemAcc_CfgPtr->AddrAreaCfg[addressAreaId]);
        uint16                        firstSubArea = addrAreaPtr->FirstSubAddrArea;
        uint16                        lastSubArea  = addrAreaPtr->LastSubAddrArea;
        const MemAcc_SubAddrAreaType* pSubAreaCfg  = &(MemAcc_CfgPtr->SubAddrAreaCfg[firstSubArea]);
        uint16                        secBatIndex  = pSubAreaCfg->SecBatchIndex;
        const MemAcc_SecBatType*      pSecBatPtr   = &(MemAcc_CfgPtr->SectorBatchRef[secBatIndex]);

        MemAcc_Memset((uint8*)curAreaPtr, 0u, sizeof(MemAcc_RunTimeAreaType));
        for (uint16 index = firstSubArea; (index <= lastSubArea) && ((Std_ReturnType)E_NOT_OK == ret); index++)
        {
            /* PRQA S 2834 ++ */ /* VL_MemAcc_2834 */
            if ((sourceAddress >= pSubAreaCfg->LogiStartAddr)
                && ((sourceAddress + length) <= (pSubAreaCfg->LogiStartAddr + pSubAreaCfg->LogiLength))
                && ((length % pSecBatPtr->MemMinReadSize) == 0u))
            /* PRQA S 2834 -- */
            {
                MemAcc_PendReqType* pendJobPtr = &curAreaPtr->CurJobInfo;

                curAreaPtr->ReqAreaAddrId   = addressAreaId;
                curAreaPtr->CurJob          = MEMACC_READ_JOB;
                curAreaPtr->CurJobStatus    = MEMACC_JOB_PENDING;
                curAreaPtr->CurReqJobStatus = MEMACC_REQJOB_STARTING;

                pendJobPtr->DataBufPtr      = destinationDataPtr;
                pendJobPtr->DealOffset      = 0u;
                pendJobPtr->Reqlength       = length;
                pendJobPtr->ReqlenRemain    = length;
                pendJobPtr->ReqLogicAddress = sourceAddress;
                pendJobPtr->SubAreaIndex    = index;
                MemAcc_UpdateLockInfo(addressAreaId, &preLockInfo);
                ret = E_OK;
            }
            pSecBatPtr++;
            pSubAreaCfg++;
        }
        SchM_Exit_MemAcc_Queue();

#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
        if ((Std_ReturnType)E_NOT_OK == ret)
        {
            /** The address range defined by sourceAddress and length is invalid */
            errorId = MEMACC_E_PARAM_ADDRESS_LENGTH;
        }
#endif
    }

#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
    if (errorId != (uint8)MEMACC_E_NO_ERROR)
    {
        MemAcc_Det_ReportError(MEMACC_SID_READ, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Return, 2, 0x09, ret);
    return ret;
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Check the initialization status and the validity of the input parameters, then request the write job
 */
Std_ReturnType MemAcc_Write(
    MemAcc_AddressAreaIdType addressAreaId,
    MemAcc_AddressType       targetAddress,
    const MemAcc_DataType*   sourceDataPtr,
    MemAcc_LengthType        length)
{
    ARTI_TRACE_N(
        USER,
        AR_CP_BSW_API,
        MemAcc,
        0,
        Bsw_Start,
        5,
        0x0A,
        addressAreaId,
        targetAddress,
        sourceDataPtr,
        length);
    Std_ReturnType ret = E_NOT_OK;

#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = MEMACC_E_NO_ERROR;
    if (MEMACC_INIT != MemAcc_State)
    {
        errorId = MEMACC_E_UNINIT;
    }
    else if (addressAreaId >= MemAcc_CfgPtr->AddrAreaCfgNum)
    {
        errorId = MEMACC_E_PARAM_ADDRESS_AREA_ID;
    }
    else if (NULL_PTR == sourceDataPtr)
    {
        errorId = MEMACC_E_PARAM_POINTER;
    }
    else if (MEMACC_NO_JOB != MemAcc_RunTimeArea[addressAreaId].CurJob)
    {
        errorId = MEMACC_E_BUSY;
    }
    else
#endif
    {
        SchM_Enter_MemAcc_Queue();
        MemAcc_RunTimeAreaType*       curAreaPtr   = &(MemAcc_RunTimeArea[addressAreaId]);
        MemAcc_LockInfoType           preLockInfo  = curAreaPtr->CurLockInfo;
        const MemAcc_AddrAreaType*    addrAreaPtr  = &(MemAcc_CfgPtr->AddrAreaCfg[addressAreaId]);
        uint16                        firstSubArea = addrAreaPtr->FirstSubAddrArea;
        uint16                        lastSubArea  = addrAreaPtr->LastSubAddrArea;
        const MemAcc_SubAddrAreaType* pSubAreaCfg  = &(MemAcc_CfgPtr->SubAddrAreaCfg[firstSubArea]);
        uint16                        secBatIndex  = pSubAreaCfg->SecBatchIndex;
        const MemAcc_SecBatType*      pSecBatPtr   = &(MemAcc_CfgPtr->SectorBatchRef[secBatIndex]);

        MemAcc_Memset((uint8*)curAreaPtr, 0u, sizeof(MemAcc_RunTimeAreaType));
        for (uint16 index = firstSubArea; (index <= lastSubArea) && ((Std_ReturnType)E_NOT_OK == ret); index++)
        {
            /* PRQA S 2834 ++ */ /* VL_MemAcc_2834 */
            if ((targetAddress >= pSubAreaCfg->LogiStartAddr)
                && ((targetAddress + length) <= (pSubAreaCfg->LogiStartAddr + pSubAreaCfg->LogiLength))
                && ((length % pSecBatPtr->MemWritePageSize) == 0u))
            /* PRQA S 2834 -- */
            {
                MemAcc_PendReqType* pendJobPtr = &curAreaPtr->CurJobInfo;

                curAreaPtr->ReqAreaAddrId   = addressAreaId;
                curAreaPtr->CurJob          = MEMACC_WRITE_JOB;
                curAreaPtr->CurJobStatus    = MEMACC_JOB_PENDING;
                curAreaPtr->CurReqJobStatus = MEMACC_REQJOB_STARTING;
                pendJobPtr->DataWriteBufPtr = sourceDataPtr;
                pendJobPtr->DealOffset      = 0u;
                pendJobPtr->Reqlength       = length;
                pendJobPtr->ReqlenRemain    = length;
                pendJobPtr->ReqLogicAddress = targetAddress;
                pendJobPtr->SubAreaIndex    = index;
                MemAcc_UpdateLockInfo(addressAreaId, &preLockInfo);
                ret = E_OK;
            }
            pSecBatPtr++;
            pSubAreaCfg++;
        }
        SchM_Exit_MemAcc_Queue();

#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
        if ((Std_ReturnType)E_NOT_OK == ret)
        {
            /* The address range defined by targetAddress and length is invalid */
            errorId = MEMACC_E_PARAM_ADDRESS_LENGTH;
        }
#endif
    }

#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
    if (errorId != (uint8)MEMACC_E_NO_ERROR)
    {
        MemAcc_Det_ReportError(MEMACC_SID_WRITE, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Return, 2, 0x0A, ret);
    return ret;
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Check the initialization status and the validity of the input parameters, then request the erase job
 */
Std_ReturnType
    MemAcc_Erase(MemAcc_AddressAreaIdType addressAreaId, MemAcc_AddressType targetAddress, MemAcc_LengthType length)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Start, 4, 0x0B, addressAreaId, targetAddress, length);
    Std_ReturnType ret = E_NOT_OK;

#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = MEMACC_E_NO_ERROR;
    if (MEMACC_INIT != MemAcc_State)
    {
        errorId = MEMACC_E_UNINIT;
    }
    else if (addressAreaId >= MemAcc_CfgPtr->AddrAreaCfgNum)
    {
        errorId = MEMACC_E_PARAM_ADDRESS_AREA_ID;
    }
    else if (MEMACC_NO_JOB != MemAcc_RunTimeArea[addressAreaId].CurJob)
    {
        errorId = MEMACC_E_BUSY;
    }
    else
#endif
    {
        SchM_Enter_MemAcc_Queue();
        MemAcc_RunTimeAreaType*       curAreaPtr   = &(MemAcc_RunTimeArea[addressAreaId]);
        MemAcc_LockInfoType           preLockInfo  = curAreaPtr->CurLockInfo;
        const MemAcc_AddrAreaType*    addrAreaPtr  = &(MemAcc_CfgPtr->AddrAreaCfg[addressAreaId]);
        uint16                        firstSubArea = addrAreaPtr->FirstSubAddrArea;
        uint16                        lastSubArea  = addrAreaPtr->LastSubAddrArea;
        const MemAcc_SubAddrAreaType* pSubAreaCfg  = &(MemAcc_CfgPtr->SubAddrAreaCfg[firstSubArea]);
        uint16                        secBatIndex  = pSubAreaCfg->SecBatchIndex;
        const MemAcc_SecBatType*      pSecBatPtr   = &(MemAcc_CfgPtr->SectorBatchRef[secBatIndex]);

        MemAcc_Memset((uint8*)curAreaPtr, 0u, sizeof(MemAcc_RunTimeAreaType));
        for (uint16 index = firstSubArea; (index <= lastSubArea) && ((Std_ReturnType)E_NOT_OK == ret); index++)
        {
            /* PRQA S 2834 ++ */ /* VL_MemAcc_2834 */
            if ((targetAddress >= pSubAreaCfg->LogiStartAddr)
                && ((targetAddress + length) <= (pSubAreaCfg->LogiStartAddr + pSubAreaCfg->LogiLength))
                && ((length % pSecBatPtr->MemEraseSectorSize) == 0u)
                && (((targetAddress - pSubAreaCfg->LogiStartAddr) % pSecBatPtr->MemEraseSectorSize) == 0u))
            /* PRQA S 2834 -- */
            {
                MemAcc_PendReqType* pendJobPtr = &curAreaPtr->CurJobInfo;

                curAreaPtr->ReqAreaAddrId   = addressAreaId;
                curAreaPtr->CurJob          = MEMACC_ERASE_JOB;
                curAreaPtr->CurJobStatus    = MEMACC_JOB_PENDING;
                curAreaPtr->CurReqJobStatus = MEMACC_REQJOB_STARTING;

                pendJobPtr->DealOffset      = 0u;
                pendJobPtr->Reqlength       = length;
                pendJobPtr->ReqlenRemain    = length;
                pendJobPtr->ReqLogicAddress = targetAddress;
                pendJobPtr->SubAreaIndex    = index;
                MemAcc_UpdateLockInfo(addressAreaId, &preLockInfo);
                ret = E_OK;
            }
            pSecBatPtr++;
            pSubAreaCfg++;
        }
        SchM_Exit_MemAcc_Queue();

#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
        if ((Std_ReturnType)E_NOT_OK == ret)
        {
            /** The address range defined by targetAddress and length is invalid */
            errorId = MEMACC_E_PARAM_ADDRESS_LENGTH;
        }
#endif
    }

#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
    if (errorId != (uint8)MEMACC_E_NO_ERROR)
    {
        MemAcc_Det_ReportError(MEMACC_SID_ERASE, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Return, 2, 0x0B, ret);
    return ret;
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#if (MEMACC_COMPARE_API == STD_ON)
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Check the initialization status and the validity of the input parameters, then request the compare job
 */
Std_ReturnType MemAcc_Compare(
    MemAcc_AddressAreaIdType addressAreaId,
    MemAcc_AddressType       sourceAddress,
    const MemAcc_DataType*   dataPtr,
    MemAcc_LengthType        length)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Start, 5, 0x0C, addressAreaId, sourceAddress, dataPtr, length);
    Std_ReturnType ret = E_NOT_OK;

#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = MEMACC_E_NO_ERROR;
    if (MEMACC_INIT != MemAcc_State)
    {
        errorId = MEMACC_E_UNINIT;
    }
    else if (addressAreaId >= MemAcc_CfgPtr->AddrAreaCfgNum)
    {
        errorId = MEMACC_E_PARAM_ADDRESS_AREA_ID;
    }
    else if (NULL_PTR == dataPtr)
    {
        errorId = MEMACC_E_PARAM_POINTER;
    }
    else if (MEMACC_NO_JOB != MemAcc_RunTimeArea[addressAreaId].CurJob)
    {
        errorId = MEMACC_E_BUSY;
    }
    else
#endif
    {
        SchM_Enter_MemAcc_Queue();
        MemAcc_RunTimeAreaType*       curAreaPtr   = &(MemAcc_RunTimeArea[addressAreaId]);
        MemAcc_LockInfoType           preLockInfo  = curAreaPtr->CurLockInfo;
        const MemAcc_AddrAreaType*    addrAreaPtr  = &(MemAcc_CfgPtr->AddrAreaCfg[addressAreaId]);
        uint16                        firstSubArea = addrAreaPtr->FirstSubAddrArea;
        uint16                        lastSubArea  = addrAreaPtr->LastSubAddrArea;
        const MemAcc_SubAddrAreaType* pSubAreaCfg  = &(MemAcc_CfgPtr->SubAddrAreaCfg[firstSubArea]);
        uint16                        secBatIndex  = pSubAreaCfg->SecBatchIndex;
        const MemAcc_SecBatType*      pSecBatPtr   = &(MemAcc_CfgPtr->SectorBatchRef[secBatIndex]);

        MemAcc_Memset((uint8*)curAreaPtr, 0u, sizeof(MemAcc_RunTimeAreaType));
        for (uint16 index = firstSubArea; (index <= lastSubArea) && ((Std_ReturnType)E_NOT_OK == ret); index++)
        {
            /* PRQA S 2834 ++ */ /* VL_MemAcc_2834 */
            if ((sourceAddress >= pSubAreaCfg->LogiStartAddr)
                && ((sourceAddress + length) <= (pSubAreaCfg->LogiStartAddr + pSubAreaCfg->LogiLength))
                && ((length % pSecBatPtr->MemMinReadSize) == 0u))
            /* PRQA S 2834 -- */
            {
                MemAcc_PendReqType* pendJobPtr = &curAreaPtr->CurJobInfo;

                curAreaPtr->ReqAreaAddrId   = addressAreaId;
                curAreaPtr->CurJob          = MEMACC_COMPARE_JOB;
                curAreaPtr->CurJobStatus    = MEMACC_JOB_PENDING;
                curAreaPtr->CurReqJobStatus = MEMACC_REQJOB_STARTING;

                pendJobPtr->SrcBufPtr    = dataPtr;
                pendJobPtr->DealOffset   = 0u;
                pendJobPtr->Reqlength    = length;
                pendJobPtr->ReqlenRemain = length;
                pendJobPtr->ReqAddress   = sourceAddress;
                pendJobPtr->SubAreaIndex = index;
                MemAcc_UpdateLockInfo(addressAreaId, &preLockInfo);
                ret = E_OK;
            }
            pSecBatPtr++;
            pSubAreaCfg++;
        }
        SchM_Exit_MemAcc_Queue();

#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
        if ((Std_ReturnType)E_NOT_OK == ret)
        {
            /** The address range defined by targetAddress and length is invalid */
            errorId = MEMACC_E_PARAM_ADDRESS_LENGTH;
        }
#endif
    }

#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
    if (errorId != (uint8)MEMACC_E_NO_ERROR)
    {
        MemAcc_Det_ReportError(MEMACC_SID_COMPARE, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Return, 2, 0x0C, ret);
    return ret;
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#endif
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Check the initialization status and the validity of the input parameters, then request the blank check job
 */
Std_ReturnType MemAcc_BlankCheck(
    MemAcc_AddressAreaIdType addressAreaId,
    MemAcc_AddressType       targetAddress,
    MemAcc_LengthType        length)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Start, 4, 0x0D, addressAreaId, targetAddress, length);
    Std_ReturnType ret = E_NOT_OK;

#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = MEMACC_E_NO_ERROR;
    if (MEMACC_INIT != MemAcc_State)
    {
        errorId = MEMACC_E_UNINIT;
    }
    else if (addressAreaId >= MemAcc_CfgPtr->AddrAreaCfgNum)
    {
        errorId = MEMACC_E_PARAM_ADDRESS_AREA_ID;
    }
    else if (MEMACC_NO_JOB != MemAcc_RunTimeArea[addressAreaId].CurJob)
    {
        errorId = MEMACC_E_BUSY;
    }
    else
#endif
    {
        SchM_Enter_MemAcc_Queue();
        MemAcc_RunTimeAreaType*       curAreaPtr   = &(MemAcc_RunTimeArea[addressAreaId]);
        MemAcc_LockInfoType           preLockInfo  = curAreaPtr->CurLockInfo;
        const MemAcc_AddrAreaType*    addrAreaPtr  = &(MemAcc_CfgPtr->AddrAreaCfg[addressAreaId]);
        uint16                        firstSubArea = addrAreaPtr->FirstSubAddrArea;
        uint16                        lastSubArea  = addrAreaPtr->LastSubAddrArea;
        const MemAcc_SubAddrAreaType* pSubAreaCfg  = &(MemAcc_CfgPtr->SubAddrAreaCfg[firstSubArea]);
        uint16                        secBatIndex  = pSubAreaCfg->SecBatchIndex;
        const MemAcc_SecBatType*      pSecBatPtr   = &(MemAcc_CfgPtr->SectorBatchRef[secBatIndex]);

        MemAcc_Memset((uint8*)curAreaPtr, 0u, sizeof(MemAcc_RunTimeAreaType));
        for (uint16 index = firstSubArea; (index <= lastSubArea) && ((Std_ReturnType)E_NOT_OK == ret); index++)
        {
            /* PRQA S 2834 ++ */ /* VL_MemAcc_2834 */
            if ((targetAddress >= pSubAreaCfg->LogiStartAddr)
                && ((targetAddress + length) <= (pSubAreaCfg->LogiStartAddr + pSubAreaCfg->LogiLength))
                && ((length % pSecBatPtr->MemMinReadSize) == 0u))
            /* PRQA S 2834 -- */
            {
                MemAcc_PendReqType* pendJobPtr = &curAreaPtr->CurJobInfo;

                curAreaPtr->ReqAreaAddrId   = addressAreaId;
                curAreaPtr->CurJob          = MEMACC_BLANKCHECK_JOB;
                curAreaPtr->CurJobStatus    = MEMACC_JOB_PENDING;
                curAreaPtr->CurReqJobStatus = MEMACC_REQJOB_STARTING;

                pendJobPtr->DealOffset      = 0u;
                pendJobPtr->Reqlength       = length;
                pendJobPtr->ReqlenRemain    = length;
                pendJobPtr->ReqLogicAddress = targetAddress;
                pendJobPtr->SubAreaIndex    = index;
                MemAcc_UpdateLockInfo(addressAreaId, &preLockInfo);
                ret = E_OK;
            }
            pSecBatPtr++;
            pSubAreaCfg++;
        }
        SchM_Exit_MemAcc_Queue();

#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
        if ((Std_ReturnType)E_NOT_OK == ret)
        {
            /** The address range defined by targetAddress and length is invalid */
            errorId = MEMACC_E_PARAM_ADDRESS_LENGTH;
        }
#endif
    }

#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
    if (errorId != (uint8)MEMACC_E_NO_ERROR)
    {
        MemAcc_Det_ReportError(MEMACC_SID_BLANKCHECK, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Return, 2, 0x0D, ret);
    return ret;
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Check the initialization status and the validity of the input parameters, then request the hardware specifice
 * service
 */
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
Std_ReturnType MemAcc_HwSpecificService(
    MemAcc_AddressAreaIdType  addressAreaId,
    MemAcc_HwIdType           hwId,
    MemAcc_MemHwServiceIdType hwServiceId,
    MemAcc_DataType*          dataPtr,
    MemAcc_LengthType*        lengthPtr)
/* PRQA S 3673 -- */
{
    ARTI_TRACE_N(
        USER,
        AR_CP_BSW_API,
        MemAcc,
        0,
        Bsw_Start,
        6,
        0x0E,
        addressAreaId,
        hwId,
        hwServiceId,
        dataPtr,
        lengthPtr);
    Std_ReturnType ret = E_NOT_OK;

#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = MEMACC_E_NO_ERROR;
    if (MEMACC_INIT != MemAcc_State)
    {
        errorId = MEMACC_E_UNINIT;
    }
    else if (addressAreaId >= MemAcc_CfgPtr->AddrAreaCfgNum)
    {
        errorId = MEMACC_E_PARAM_ADDRESS_AREA_ID;
    }
    else if (MEMACC_NO_JOB != MemAcc_RunTimeArea[addressAreaId].CurJob)
    {
        errorId = MEMACC_E_BUSY;
    }
    else
#endif
    {
        SchM_Enter_MemAcc_Queue();
        MemAcc_RunTimeAreaType*       curAreaPtr   = &(MemAcc_RunTimeArea[addressAreaId]);
        MemAcc_LockInfoType           preLockInfo  = curAreaPtr->CurLockInfo;
        const MemAcc_AddrAreaType*    addrAreaPtr  = &(MemAcc_CfgPtr->AddrAreaCfg[addressAreaId]);
        uint16                        firstSubArea = addrAreaPtr->FirstSubAddrArea;
        uint16                        lastSubArea  = addrAreaPtr->LastSubAddrArea;
        const MemAcc_SubAddrAreaType* pSubAreaCfg  = &(MemAcc_CfgPtr->SubAddrAreaCfg[firstSubArea]);

        MemAcc_Memset((uint8*)curAreaPtr, 0u, sizeof(MemAcc_RunTimeAreaType));
        for (uint16 index = firstSubArea; (index <= lastSubArea) && ((Std_ReturnType)E_NOT_OK == ret); index++)
        {
            if (hwId == pSubAreaCfg->HardwareId)
            {
                MemAcc_PendReqType* pendJobPtr = &curAreaPtr->CurJobInfo;

                curAreaPtr->ReqAreaAddrId   = addressAreaId;
                curAreaPtr->CurJob          = MEMACC_MEMHWSPECIFIC_JOB;
                curAreaPtr->CurJobStatus    = MEMACC_JOB_PENDING;
                curAreaPtr->CurReqJobStatus = MEMACC_REQJOB_STARTING;

                pendJobPtr->hwServiceId  = hwServiceId;
                pendJobPtr->DataBufPtr   = dataPtr;
                pendJobPtr->lengthPtr    = lengthPtr;
                pendJobPtr->SubAreaIndex = index;
                MemAcc_UpdateLockInfo(addressAreaId, &preLockInfo);
                ret = E_OK;
            }
            pSubAreaCfg++;
        }
        SchM_Exit_MemAcc_Queue();

#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
        if ((Std_ReturnType)E_NOT_OK == ret)
        {
            /** The address range defined by targetAddress and length is invalid */
            errorId = MEMACC_E_PARAM_HW_ID;
        }
#endif
    }

#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
    if (errorId != (uint8)MEMACC_E_NO_ERROR)
    {
        MemAcc_Det_ReportError(MEMACC_SID_HWSPECIFICSERVICE, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Return, 2, 0x0E, ret);
    return ret;
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Check the initialization status and the validity of the input parameters, if the address for requesting lock
 * is within the range of subaddress, set the lock value based on whether the driver is pending
 */
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
Std_ReturnType MemAcc_RequestLock(
    MemAcc_AddressAreaIdType addressAreaId,
    MemAcc_AddressType       address,
    MemAcc_LengthType        length,
    void*                    lockNotificationFctPtr)
/* PRQA S 3673 -- */
{
    ARTI_TRACE_N(
        USER,
        AR_CP_BSW_API,
        MemAcc,
        0,
        Bsw_Start,
        5,
        0x11,
        addressAreaId,
        address,
        length,
        lockNotificationFctPtr);
    boolean        find   = FALSE;
    Std_ReturnType retVal = E_NOT_OK;

#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = MEMACC_E_NO_ERROR;
    if (MEMACC_INIT != MemAcc_State)
    {
        errorId = MEMACC_E_UNINIT;
    }
    else if (addressAreaId >= MemAcc_CfgPtr->AddrAreaCfgNum)
    {
        errorId = MEMACC_E_PARAM_ADDRESS_AREA_ID;
    }
    else if (NULL_PTR == lockNotificationFctPtr)
    {
        errorId = MEMACC_E_PARAM_POINTER;
    }
    else
#endif
    {
        MemAcc_LockInfoType*          lockInfo;
        MemAcc_RunTimeAreaType*       curAreaPtr   = &(MemAcc_RunTimeArea[addressAreaId]);
        const MemAcc_AddrAreaType*    addrAreaPtr  = &(MemAcc_CfgPtr->AddrAreaCfg[addressAreaId]);
        uint16                        firstSubArea = addrAreaPtr->FirstSubAddrArea;
        uint16                        lastSubArea  = addrAreaPtr->LastSubAddrArea;
        const MemAcc_SubAddrAreaType* pSubAreaCfg  = &(MemAcc_CfgPtr->SubAddrAreaCfg[firstSubArea]);

        for (uint16 index = firstSubArea; (index <= lastSubArea) && (FALSE == find); index++)
        {
            if ((address >= pSubAreaCfg->LogiStartAddr)
                && ((address + length) <= (pSubAreaCfg->LogiStartAddr + pSubAreaCfg->LogiLength)))
            {
                lockInfo = &(curAreaPtr->CurLockInfo);
                find     = TRUE;
            }
            pSubAreaCfg++;
        }
        if (TRUE == find)
        {
            if (MEMACC_UNLOCK == lockInfo->LockStatus)
            {
                MemAcc_LockNotiType lockNotiFunc; /* PRQA S 0759 */ /* VL_MemAcc_0759 */
                /** Update the requested lock information */
                lockInfo->LockStartAddr = address;
                lockInfo->LockLength    = length;
                lockInfo->JobLocked     = FALSE;
                lockNotiFunc.generalPtr = lockNotificationFctPtr;
                lockInfo->LockNotif     = lockNotiFunc.lockNotiFuncPtr;
                /** The job is still pending and will be locked after processing is completed */
                if (MEMACC_JOB_PENDING == curAreaPtr->CurJobStatus)
                {
                    lockInfo->LockStatus = MEMACC_LOCKING;
                }
                else
                {
                    /** No job is pending, set the lock status to locked */
                    lockInfo->LockStatus = MEMACC_LOCKED;
                    lockInfo->LockNotif();
                }
                retVal = E_OK;
            }
        }

#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
        else
        {
            /** The address range defined by targetAddress and length is invalid */
            errorId = MEMACC_E_PARAM_ADDRESS_LENGTH;
        }
#endif
    }

#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
    if (errorId != (uint8)MEMACC_E_NO_ERROR)
    {
        MemAcc_Det_ReportError(MEMACC_SID_REQUESTLOCK, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Return, 2, 0x11, retVal);
    return retVal;
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Check the initialization status and the validity of the input parameters, if the address for requesting lock
 * is within the range of subaddress, set the unlock value based on whether the driver has ended
 */
Std_ReturnType
    MemAcc_ReleaseLock(MemAcc_AddressAreaIdType addressAreaId, MemAcc_AddressType address, MemAcc_LengthType length)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Start, 4, 0x12, addressAreaId, address, length);
    boolean        find   = FALSE;
    Std_ReturnType retVal = E_NOT_OK;
#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = MEMACC_E_NO_ERROR;
    if (MEMACC_INIT != MemAcc_State)
    {
        errorId = MEMACC_E_UNINIT;
    }
    else if (addressAreaId >= MemAcc_CfgPtr->AddrAreaCfgNum)
    {
        errorId = MEMACC_E_PARAM_ADDRESS_AREA_ID;
    }
    else
#endif
    {
        MemAcc_LockInfoType*          lockInfo;
        const MemAcc_AddrAreaType*    addrAreaPtr  = &(MemAcc_CfgPtr->AddrAreaCfg[addressAreaId]);
        uint16                        firstSubArea = addrAreaPtr->FirstSubAddrArea;
        uint16                        lastSubArea  = addrAreaPtr->LastSubAddrArea;
        const MemAcc_SubAddrAreaType* pSubAreaCfg  = &(MemAcc_CfgPtr->SubAddrAreaCfg[firstSubArea]);

        for (uint16 index = firstSubArea; (index <= lastSubArea) && (FALSE == find); index++)
        {
            if ((address >= pSubAreaCfg->LogiStartAddr)
                && ((address + length) <= (pSubAreaCfg->LogiStartAddr + pSubAreaCfg->LogiLength)))
            {
                lockInfo = &MemAcc_RunTimeArea[addressAreaId].CurLockInfo;
                find     = TRUE;
            }
            pSubAreaCfg++;
        }
        if (TRUE == find)
        {
            if (MEMACC_UNLOCK != lockInfo->LockStatus)
            {
#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
                if ((address != lockInfo->LockStartAddr) || (length != lockInfo->LockLength))
                {
                    errorId = MEMACC_E_PARAM_ADDRESS_LENGTH;
                }
#endif
                /** Reset the lock information */
                lockInfo->LockStartAddr = 0u;
                lockInfo->LockLength    = 0u;
                lockInfo->JobLocked     = FALSE;
                lockInfo->LockStatus    = MEMACC_UNLOCK;
                lockInfo->LockNotif     = NULL_PTR;
                retVal                  = E_OK;
            }
        }

#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
        else
        {
            /** The address range defined by targetAddress and length is invalid */
            errorId = MEMACC_E_PARAM_ADDRESS_LENGTH;
        }
#endif
    }

#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
    if (errorId != (uint8)MEMACC_E_NO_ERROR)
    {
        MemAcc_Det_ReportError(MEMACC_SID_RELEASELOCK, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Return, 2, 0x12, retVal);
    return retVal;
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
/* PRQA S 6030 -- */
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief       Service to handle the requested jobs and the internal management operations. Depending on the
 * configuration MemAcc will call the Mem driver main functions
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76680
 */
void MemAcc_MainFunction(void)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Start, 1, 0x03);
    if (MemAcc_State == MEMACC_INIT)
    {
        uint16 addrAreaNum = MemAcc_CfgPtr->AddrAreaCfgNum;
        for (uint16 iloop = 0u; iloop < addrAreaNum; ++iloop)
        {
            MemAcc_RunTimeAreaType* currentJob = &MemAcc_RunTimeArea[iloop];
            if ((MEMACC_JOB_PENDING == currentJob->CurJobStatus) && (FALSE == currentJob->CurLockInfo.JobLocked))
            {
                MemAcc_ReqJobSchedule(currentJob);
#if (MEMACC_MEMDRIVER_DIRECT_STATIC_ENABLE != STD_ON)
                /** If the requested job is not retrying, then call the Mem driver  */
                if (MEMACC_REQJOB_RETRYING != currentJob->CurReqJobStatus)
                {
                    MemAcc_ScheMemDriMainFuc(currentJob);
                }
#endif
            }
        }
    }
    ARTI_TRACE_N(USER, AR_CP_BSW_API, MemAcc, 0, Bsw_Return, 1, 0x03);
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
/* PRQA S 1503 -- */
/* ========================================== internal function definitions ========================================= */
#if (MEMACC_DEV_ERROR_DETECT == STD_ON)
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Check the redefinition tags of the driver binary file. If it is valid, check their DelimiterAddr, VendorID,
 * and DriverID
 */
/* PRQA S 6030 ++ */ /* VL_MTR_MemAcc_STMIF */
MEMACC_LOCAL boolean MemAcc_CheckMemDriverBinaryDF(const MemAcc_MemDriverHeaderType* MemDriverHeaderPtr)
{
    boolean valid = TRUE;

    if (MEMACC_MEMDRIVER_RELOCATABLE_BINARY != MemDriverHeaderPtr->Flags.RelocatableBinary)
    {
        if (0u == MemDriverHeaderPtr->DelimiterAddr)
        {
            valid = FALSE;
        }
        else
        {
            /* PRQA S 0306 ++ */ /* VL_MemAcc_0306 */
            const MemAcc_MemDriverUniqueIDType* MemDriverDelimiter =
                (MemAcc_MemDriverUniqueIDType*)((MemAcc_UintPtrType)MemDriverHeaderPtr->DelimiterAddr);
            /* PRQA S 0306 -- */
            if (((uint16)MEMACC_MEMDRIVER_ABI_VERSION != MemDriverDelimiter->ABIVersion)
                || (MemDriverDelimiter->VendorID != MemDriverHeaderPtr->UniqueID.VendorID)
                || (MemDriverDelimiter->DriverID != MemDriverHeaderPtr->UniqueID.DriverID))
            {
                valid = FALSE;
            }
            else
            {
                /** Do nothing */
            }
        }
    }
    else
    {
        if (0u != MemDriverHeaderPtr->DelimiterAddr)
        {
            valid = FALSE;
        }
    }
    return valid;
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
/* PRQA S 6030 -- */
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Check the contents related to the hardware ID, header information address and UniqueID in sequence. If all are
 * valid, then check the delimiter field
 */
MEMACC_LOCAL boolean MemAcc_CheckMemDriverBinary(MemAcc_AddressType HeaderAddr, MemAcc_HwIdType HwId)
{
    boolean valid = TRUE;
    /* PRQA S 0306 ++ */ /* VL_MemAcc_0306 */
    const MemAcc_MemDriverHeaderType* header = (MemAcc_MemDriverHeaderType*)((MemAcc_UintPtrType)HeaderAddr);
    /* PRQA S 0306 -- */

    /* PRQA S 2995 ++ */ /* VL_MemAcc_2995 */
    if (((uint16)MEMACC_DRIVER_NUM <= HwId)
        || ((MEMACC_MEMDRIVER_RELOCATABLE_BINARY != header->Flags.RelocatableBinary)
            && (header->HeaderAddr != HeaderAddr))
        || ((uint16)MEMACC_MEMDRIVER_ABI_VERSION != header->UniqueID.ABIVersion)
        || ((uint16)MEMACC_VENDOR_ID != header->UniqueID.VendorID))
    {
        valid = FALSE;
    }
    else
    {
        /** Check the delimiter field */
        valid = MemAcc_CheckMemDriverBinaryDF(header);
    }
    /* PRQA S 2995 -- */

    return valid;
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#endif
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Set the value of the target address based on the incoming data
 */
MEMACC_LOCAL void MemAcc_Memset(uint8* destPtr, const uint8 data, const MemAcc_LengthType length)
{
#if (MEMACC_FEATURE_MEMCOPY_USED_ILIB == STD_OFF)
    for (MemAcc_LengthType iloop = 0u; iloop < length; iloop++)
    {
        destPtr[iloop] = data;
    }
#else
    IStdLib_MemCpy(dst, src, len)
#endif
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Schedule the corresponding driver service interface based on the job at runtime
 */
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
MEMACC_LOCAL Std_ReturnType MemAcc_ScheduleMemServices(MemAcc_RunTimeAreaType* RuntimeJobInfo)
/* PRQA S 3673 -- */
{
    Std_ReturnType                    retVal       = E_NOT_OK;
    MemAcc_PendReqType                reqInfo      = RuntimeJobInfo->CurJobInfo;
    uint16                            subArea      = reqInfo.SubAreaIndex;
    const MemAcc_SubAddrAreaType*     subAreaPtr   = &(MemAcc_CfgPtr->SubAddrAreaCfg[subArea]);
    uint8                             hwId         = subAreaPtr->HardwareId;
    uint16                            secBatIndex  = subAreaPtr->SecBatchIndex;
    MemAcc_MemInstanceIdType          instanceId   = MemAcc_CfgPtr->SectorBatchRef[secBatIndex].MemInstanceId;
    const MemAcc_MemBinaryHeaderType* MemDriverApi = &(MemAcc_MemDriverApi[hwId]);

    switch (RuntimeJobInfo->CurJob)
    {
    case MEMACC_WRITE_JOB:
        retVal = MemDriverApi->WriteFunc(instanceId, reqInfo.ReqAddress, reqInfo.DataWriteBufPtr, reqInfo.DealSize);
        break;
    case MEMACC_READ_JOB:
        retVal = MemDriverApi->ReadFunc(instanceId, reqInfo.ReqAddress, reqInfo.DataBufPtr, reqInfo.DealSize);
        break;
    case MEMACC_COMPARE_JOB:
#if (MEMACC_COMPARE_API == STD_ON)
        retVal = MemDriverApi->ReadFunc(
            instanceId,
            reqInfo.ReqAddress,
            (MemAcc_DataType*)MemAcc_CompareBuffer,
            reqInfo.DealSize);
#endif
        break;
    case MEMACC_ERASE_JOB:
        retVal = MemDriverApi->EraseFunc(instanceId, reqInfo.ReqAddress, reqInfo.DealSize);
        break;
    case MEMACC_MEMHWSPECIFIC_JOB:
        retVal =
            MemDriverApi->HwSpecificServiceFunc(instanceId, reqInfo.hwServiceId, reqInfo.DataBufPtr, reqInfo.lengthPtr);
        break;
    case MEMACC_BLANKCHECK_JOB:
        retVal = MemDriverApi->BlankCheckFunc(instanceId, reqInfo.ReqAddress, reqInfo.DealSize);
        break;
    case MEMACC_REQUESTLOCK_JOB:
    default:
        /** Do nothing */
        break;
    }

    return retVal;
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Update the driver address information of the request
 */
MEMACC_LOCAL void MemAcc_UpdateDriverAddress(MemAcc_RunTimeAreaType* RuntimeJobInfo)
{
    MemAcc_PendReqType*           pendJobPtr  = &(RuntimeJobInfo->CurJobInfo);
    uint16                        subArea     = pendJobPtr->SubAreaIndex;
    const MemAcc_SubAddrAreaType* subAreaPtr  = &(MemAcc_CfgPtr->SubAddrAreaCfg[subArea]);
    MemAcc_AddressType            startAddr   = subAreaPtr->LogiStartAddr;
    MemAcc_AddressType            Offset      = pendJobPtr->ReqLogicAddress - startAddr;
    uint16                        secBatIndex = subAreaPtr->SecBatchIndex;

    pendJobPtr->ReqAddress = MemAcc_CfgPtr->SectorBatchRef[secBatIndex].MemStartAddr + Offset;
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Update the relevant information of the SubArea
 */
MEMACC_LOCAL void MemAcc_UpdateSubArea(MemAcc_RunTimeAreaType* RuntimeJobInfo)
{
    /** Translate logical address to physical address according to the current sub area */
    MemAcc_UpdateDriverAddress(RuntimeJobInfo);

    MemAcc_JobType                reqJob     = RuntimeJobInfo->CurJob;
    MemAcc_PendReqType*           pendJobPtr = &(RuntimeJobInfo->CurJobInfo);
    uint16                        subArea    = pendJobPtr->SubAreaIndex;
    const MemAcc_SubAddrAreaType* subAreaPtr = &(MemAcc_CfgPtr->SubAddrAreaCfg[subArea]);

    pendJobPtr->ReqJobRetries =
        (reqJob == MEMACC_WRITE_JOB)
            ? subAreaPtr->WriteRetryNum
            : ((reqJob == MEMACC_ERASE_JOB) ? subAreaPtr->EraseRetryNum
                                            : ((reqJob == MEMACC_READ_JOB) ? subAreaPtr->ReadRetryNum : 0u));
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Set the corresponding status value based on the call result of the driver's service interface
 */
MEMACC_LOCAL void MemAcc_InvocationMemService(MemAcc_RunTimeAreaType* RuntimeJobInfo)
{
    Std_ReturnType reqResult = MemAcc_ScheduleMemServices(RuntimeJobInfo);
    if ((Std_ReturnType)E_OK == reqResult)
    {
        /** The Mem driver is processing job */
        RuntimeJobInfo->CurReqJobStatus = MEMACC_REQJOB_PROCESSING;
    }
    else
    {
        /** The Mem driver is not processing the job or the job processing fails */
        RuntimeJobInfo->CurReqJobStatus = MEMACC_REQJOB_RETRYING;
        RuntimeJobInfo->CurJobResult    = MEMACC_FAILED;
    }
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Call the driver interface of the configured Mem driver interface
 */
MEMACC_LOCAL MemAcc_MemJobResultType MemAcc_GetMemJobResult(const MemAcc_RunTimeAreaType* RuntimeJobInfo)
{
    MemAcc_PendReqType                   jobInfo          = RuntimeJobInfo->CurJobInfo;
    uint16                               subArea          = jobInfo.SubAreaIndex;
    const MemAcc_SubAddrAreaType*        subAreaPtr       = &(MemAcc_CfgPtr->SubAddrAreaCfg[subArea]);
    uint8                                hwId             = subAreaPtr->HardwareId;
    uint16                               secBatIndex      = subAreaPtr->SecBatchIndex;
    MemAcc_MemInstanceIdType             instanceId       = MemAcc_CfgPtr->SectorBatchRef[secBatIndex].MemInstanceId;
    const MemAcc_MemGetJobResultFuncType getResultFuncPtr = MemAcc_MemDriverApi[hwId].GetJobResultFunc;

    /** Get the result of the job in the Mem driver */
    MemAcc_MemJobResultType jobResult = getResultFuncPtr(instanceId);
    return jobResult;
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Request job is ready
 */
MEMACC_LOCAL void MemAcc_ReqJobReady(MemAcc_RunTimeAreaType* RuntimeJobInfo)
{
    if (TRUE == MemAcc_CheckMemResource(RuntimeJobInfo))
    {
        MemAcc_UpdateSubArea(RuntimeJobInfo);
        MemAcc_CalculateLength(RuntimeJobInfo);
        MemAcc_InvocationMemService(RuntimeJobInfo);
    }
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Request job is processing
 */
MEMACC_LOCAL void MemAcc_ReqJobProcess(MemAcc_RunTimeAreaType* RuntimeJobInfo)
{
    /** Get the result of the job in the Mem driver */
    MemAcc_MemJobResultType jobResult = MemAcc_GetMemJobResult(RuntimeJobInfo);

    switch (jobResult)
    {
    case MEM_JOB_OK:
        RuntimeJobInfo->CurJobResult = MEMACC_OK;
    case MEM_ECC_CORRECTED:
        /** The job is processed successfully */
        MemAcc_JobProcessingOk(RuntimeJobInfo);
        /** The job is processed with ECC corrected */
        if ((jobResult == MEM_ECC_CORRECTED) && (0u == RuntimeJobInfo->CurJobInfo.ReqlenRemain))
        {
            RuntimeJobInfo->CurJobResult = MEMACC_ECC_CORRECTED;
        }
        break;
    case MEM_JOB_PENDING:
        /** The job is pending */
        break;
    default:
        /** The job is fail */
        RuntimeJobInfo->CurJobResult =
            (jobResult == MEM_JOB_FAILED)
                ? MEMACC_FAILED
                : (((jobResult == MEM_INCONSISTENT)
                        ? MEMACC_INCONSISTENT
                        : ((jobResult == MEM_ECC_UNCORRECTED) ? MEMACC_ECC_UNCORRECTED
                                                              : (RuntimeJobInfo->CurJobResult))));
        RuntimeJobInfo->CurReqJobStatus = MEMACC_REQJOB_RETRYING;
        break;
    }
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Request job is fail, retry it
 */
MEMACC_LOCAL void MemAcc_ReqJobRetry(MemAcc_RunTimeAreaType* RuntimeJobInfo)
{
    MemAcc_PendReqType* pendJobPtr = &(RuntimeJobInfo->CurJobInfo);
    if (pendJobPtr->ReqJobRetries > 0u)
    {
        pendJobPtr->ReqJobRetries--;

        /** Retry the same requested job */
        MemAcc_CalculateLength(RuntimeJobInfo);
        MemAcc_InvocationMemService(RuntimeJobInfo);
    }
    else
    {
        /** Do not retry the job, set the status to stop */
        RuntimeJobInfo->CurReqJobStatus = MEMACC_REQJOB_STOP;
    }
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Suspend the request job
 */
MEMACC_LOCAL void MemAcc_ReqJobSuspend(MemAcc_RunTimeAreaType* RuntimeJobInfo)
{
    MemAcc_MemJobResultType jobResult = MemAcc_GetMemJobResult(RuntimeJobInfo);
    switch (jobResult)
    {
    case MEM_JOB_OK:
    case MEM_ECC_CORRECTED:
        MemAcc_JobSuspendingOk(RuntimeJobInfo);
        /** The job is processed with ECC corrected */
        if ((jobResult == MEM_ECC_CORRECTED) && (0u == RuntimeJobInfo->CurJobInfo.ReqlenRemain))
        {
            RuntimeJobInfo->CurJobResult = MEMACC_ECC_CORRECTED;
        }
        break;
    case MEM_JOB_PENDING:
        /** The job is pending */
        break;
    default:
        /** The job is fail */
        MemAcc_JobSuspendingFail(RuntimeJobInfo, jobResult);
        break;
    }
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Resume the request job
 */
MEMACC_LOCAL void MemAcc_ReqJobResume(MemAcc_RunTimeAreaType* RuntimeJobInfo)
{
    boolean HwResourceGranted = MemAcc_CheckMemResource(RuntimeJobInfo);
    if (TRUE == HwResourceGranted)
    {
        /** Resume the same requested job */
        MemAcc_CalculateLength(RuntimeJobInfo);
        MemAcc_InvocationMemService(RuntimeJobInfo);
    }
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Cancel the request job
 */
MEMACC_LOCAL void MemAcc_ReqJobCancel(MemAcc_RunTimeAreaType* RuntimeJobInfo)
{
    MemAcc_MemJobResultType jobResult = MemAcc_GetMemJobResult(RuntimeJobInfo);

    switch (jobResult)
    {
    case MEM_JOB_OK:
    case MEM_ECC_CORRECTED:
        /** No job was processed now, update the remaining length, set status to cancelled */
        MemAcc_UpdateLength(RuntimeJobInfo);
        RuntimeJobInfo->CurReqJobStatus = MEMACC_REQJOB_STOP;
        RuntimeJobInfo->CurJobStatus    = MEMACC_JOB_IDLE;
        RuntimeJobInfo->CurJobResult    = MEMACC_CANCELED;
        break;
    case MEM_JOB_FAILED:
    case MEM_INCONSISTENT:
    case MEM_ECC_UNCORRECTED:
        /** Job was processed failed, set status to cancelled */
        RuntimeJobInfo->CurReqJobStatus = MEMACC_REQJOB_STOP;
        RuntimeJobInfo->CurJobStatus    = MEMACC_JOB_IDLE;
        RuntimeJobInfo->CurJobResult    = MEMACC_CANCELED;
        break;
    case MEM_JOB_PENDING:
    default:
        /** Do nothing */
        break;
    }
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Schedule the request job
 */
MEMACC_LOCAL void MemAcc_ReqJobSchedule(MemAcc_RunTimeAreaType* RuntimeJobInfo)
{
    if (MemAcc_ReqJobFunc[RuntimeJobInfo->CurReqJobStatus] != NULL_PTR)
    {
        MemAcc_ReqJobFunc[RuntimeJobInfo->CurReqJobStatus](RuntimeJobInfo);
    }

    if (MEMACC_REQJOB_STOP == RuntimeJobInfo->CurReqJobStatus)
    {
        MemAcc_PendReqType*           pendJobPtr = &(RuntimeJobInfo->CurJobInfo);
        uint16                        subArea    = pendJobPtr->SubAreaIndex;
        const MemAcc_SubAddrAreaType* subAreaPtr = &(MemAcc_CfgPtr->SubAddrAreaCfg[subArea]);
        /** The job is completed */
        MemAcc_ReqMemHw[subAreaPtr->HardwareId] = MEMACC_MEM_DRIVER_IDLE;

        RuntimeJobInfo->CurJob       = MEMACC_NO_JOB;
        RuntimeJobInfo->CurJobStatus = MEMACC_JOB_IDLE;

        MemAcc_LockInfoType* lockInfo = &(RuntimeJobInfo->CurLockInfo);
        if (MEMACC_LOCKING == lockInfo->LockStatus)
        {
            /** Lock the area */
            lockInfo->LockStatus = MEMACC_LOCKED;
            lockInfo->LockNotif();
        }

        /** Call the notification interface */
        MemAcc_JobEndNotification(RuntimeJobInfo);
    }
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Check whether the currently running AreaId matches the requested AreaId and whether the currently running
 * AreaId is IDLE; otherwise, there will be a driver conflict for the request
 */
MEMACC_LOCAL boolean MemAcc_CheckMemResource(const MemAcc_RunTimeAreaType* RuntimeJobInfo)
{
    boolean                       resourseValid = TRUE;
    MemAcc_PendReqType            jobInfo       = RuntimeJobInfo->CurJobInfo;
    uint16                        subArea       = jobInfo.SubAreaIndex;
    const MemAcc_SubAddrAreaType* subAreaPtr    = &(MemAcc_CfgPtr->SubAddrAreaCfg[subArea]);
    uint8                         hwId          = subAreaPtr->HardwareId;
    const uint16 reqCurAreaId  = RuntimeJobInfo->ReqAreaAddrId; /** The address area index number of current job */
    const uint16 runningAreaId = MemAcc_ReqMemHw[hwId];         /** The address area is using the hardware resource */

    if (runningAreaId == reqCurAreaId)
    {
        /** Do nothing */
    }
    else if ((uint16)MEMACC_MEM_DRIVER_IDLE == runningAreaId)
    {
        /** Set the hardware Id */
        MemAcc_ReqMemHw[hwId] = reqCurAreaId;
    }
    else
    {
        /** The hardware is busy, arrange the job scheduling based on priority */
        resourseValid = FALSE;
        MemAcc_ProcessMemPriority(runningAreaId, reqCurAreaId);
    }

    return resourseValid;
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Process Mem driver requests based on the configured priorities
 */
MEMACC_LOCAL void MemAcc_ProcessMemPriority(const uint16 runningAreaId, const uint16 reqCurAreaId)
{
    MemAcc_RunTimeAreaType* jobRunning = &(MemAcc_RunTimeArea[runningAreaId]);

    if (MemAcc_CfgPtr->AddrAreaCfg[reqCurAreaId].AddrAreaPriority
        > MemAcc_CfgPtr->AddrAreaCfg[runningAreaId].AddrAreaPriority)
    {
        if (MEMACC_REQJOB_RETRYING == jobRunning->CurReqJobStatus)
        {
            /** Continue to handle the job that need to be retried */
            if (jobRunning->CurJobInfo.ReqJobRetries > 0u)
            {
                jobRunning->CurJobInfo.ReqJobRetries--;
                jobRunning->CurReqJobStatus = MEMACC_REQJOB_RESUMING;
            }
            else
            {
                jobRunning->CurReqJobStatus = MEMACC_REQJOB_STOP;
            }
        }
        else
        {
            /** Suspend the currently running job */
            jobRunning->CurReqJobStatus = MEMACC_REQJOB_SUSPENDING;
        }
    }
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief After the job processing is completed, call the configured notification interface
 */
MEMACC_LOCAL void MemAcc_JobEndNotification(const MemAcc_RunTimeAreaType* RuntimeJobInfo)
{
    const MemAcc_AddrAreaType*          addressAreaPtr = MemAcc_CfgPtr->AddrAreaCfg;
    const MemAcc_AddrAreaJobEndNotiType endNotifFunc   = addressAreaPtr->JobEndNoti;

    if (NULL_PTR != endNotifFunc)
    {
        endNotifFunc(RuntimeJobInfo->ReqAreaAddrId, RuntimeJobInfo->CurJobResult);
    }
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Update the data size of the request according to different request job and the length of the configuration
 */
MEMACC_LOCAL void MemAcc_CalculateLength(MemAcc_RunTimeAreaType* RuntimeJobInfo)
{
    MemAcc_LengthType             reqSize     = 0u;
    MemAcc_PendReqType*           pendJobPtr  = &(RuntimeJobInfo->CurJobInfo);
    uint16                        subArea     = pendJobPtr->SubAreaIndex;
    const MemAcc_SubAddrAreaType* subAreaPtr  = &(MemAcc_CfgPtr->SubAddrAreaCfg[subArea]);
    uint8                         burstFlag   = subAreaPtr->UseBurstFlag;
    uint16                        secBatIndex = subAreaPtr->SecBatchIndex;
    const MemAcc_SecBatType*      pSecBatPtr  = &(MemAcc_CfgPtr->SectorBatchRef[secBatIndex]);
    /** Update remain length */
    MemAcc_LengthType reqRemainLength = (pSecBatPtr->MemStartAddr + subAreaPtr->LogiLength) - pendJobPtr->ReqAddress;

    if (reqRemainLength > pendJobPtr->ReqlenRemain)
    {
        /** Update request length */
        reqRemainLength = pendJobPtr->ReqlenRemain;
    }

    switch (RuntimeJobInfo->CurJob)
    {
    case MEMACC_ERASE_JOB:
        if ((TRUE == MemAcc_BurstFlagEnable(burstFlag, (uint8)MEMACC_ERASE_BURST))
            && (reqRemainLength >= pSecBatPtr->MemEraseBurstSize))
        {
            reqSize = pSecBatPtr->MemEraseBurstSize;
        }
        else
        {
            reqSize = pSecBatPtr->MemEraseSectorSize;
        }
        break;
    case MEMACC_WRITE_JOB:
        if ((TRUE == MemAcc_BurstFlagEnable(burstFlag, (uint8)MEMACC_WRITE_BURST))
            && (reqRemainLength >= pSecBatPtr->MemWriteBurstSize))
        {
            reqSize = pSecBatPtr->MemWriteBurstSize;
        }
        else
        {
            reqSize = pSecBatPtr->MemWritePageSize;
        }

        /** Generate a Bitmask for alignment calculation, obtain the offset of the address relative to the alignment
         * boundary, and get the remaining space from the address to the end of the current alignment segment */
        reqSize = ((pendJobPtr->ReqAddress % reqSize) != 0u) ? pSecBatPtr->MemWritePageSize : reqSize;
        break;
    case MEMACC_READ_JOB:
    case MEMACC_COMPARE_JOB:
    case MEMACC_BLANKCHECK_JOB:
        if ((TRUE == MemAcc_BurstFlagEnable(burstFlag, (uint8)MEMACC_READ_BURST))
            && (reqRemainLength >= pSecBatPtr->MemMaxReadSize))
        {
            reqSize = pSecBatPtr->MemMaxReadSize;
        }
        else
        {
            reqSize = pSecBatPtr->MemMinReadSize;
        }
        break;
    default:
        /** Do nothing */
        break;
    }
    pendJobPtr->DealSize = reqSize;
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#if (MEMACC_COMPARE_API == STD_ON)
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Based on the length passed in, compare whether the data of the target address and the source address are
 * consistent
 */
MEMACC_LOCAL boolean
    MemAcc_CompareData(const MemAcc_DataType* SourcePtr, const MemAcc_DataType* DestPtr, MemAcc_LengthType length)
{
    boolean retValue = (boolean)TRUE;

    for (MemAcc_LengthType counter = 0u; counter < length; counter++)
    {
        if (SourcePtr[counter] != DestPtr[counter])
        {
            retValue = (boolean)FALSE;
            break;
        }
    }
    return retValue;
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#endif
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Update the pointer of databuffer according to the requested job
 */
MEMACC_LOCAL void MemAcc_UpdateLength(MemAcc_RunTimeAreaType* RuntimeJobInfo)
{
    MemAcc_PendReqType* pendJobPtr = &(RuntimeJobInfo->CurJobInfo);
    MemAcc_LengthType   dealSize   = pendJobPtr->DealSize;

    pendJobPtr->ReqLogicAddress += dealSize;
    pendJobPtr->ReqAddress += dealSize;
    /** Calculate the remaining length that needs to be processed */
    pendJobPtr->ReqlenRemain -= dealSize;

    if (MEMACC_WRITE_JOB == RuntimeJobInfo->CurJob)
    {
        if (NULL_PTR != pendJobPtr->DataWriteBufPtr)
        {
            pendJobPtr->DataWriteBufPtr = &(pendJobPtr->DataWriteBufPtr[dealSize]);
        }
    }
    else if (MEMACC_READ_JOB == RuntimeJobInfo->CurJob)
    {
        if (NULL_PTR != pendJobPtr->DataBufPtr)
        {
            pendJobPtr->DataBufPtr = &(pendJobPtr->DataBufPtr[dealSize]);
        }
    }
    else if (
        (MEMACC_MEMHWSPECIFIC_JOB == RuntimeJobInfo->CurJob)
#if (MEMACC_COMPARE_API == STD_ON)
        || (MEMACC_COMPARE_JOB == RuntimeJobInfo->CurJob)
#endif
    )
    {
        if (NULL_PTR != pendJobPtr->SrcBufPtr)
        {
            pendJobPtr->SrcBufPtr = &(pendJobPtr->SrcBufPtr[dealSize]);
        }
    }
    else
    {
        /** Do nothing */
    }
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief It is necessary to compare the consistency of the data, and then update the remaining length. If the requested
 * length has not been processed, continue to schedule the driver service and update the status and length
 */
MEMACC_LOCAL void MemAcc_JobProcessingOk(MemAcc_RunTimeAreaType* RuntimeJobInfo)
{
    MemAcc_PendReqType* pendJobPtr = &(RuntimeJobInfo->CurJobInfo);
#if (MEMACC_COMPARE_API == STD_ON)
    if (MEMACC_COMPARE_JOB == RuntimeJobInfo->CurJob)
    {
        if ((boolean)FALSE
            == MemAcc_CompareData(pendJobPtr->SrcBufPtr, (MemAcc_DataType*)MemAcc_CompareBuffer, pendJobPtr->DealSize))
        {
            /** The job is failed */
            RuntimeJobInfo->CurReqJobStatus = MEMACC_REQJOB_STOP;
            RuntimeJobInfo->CurJobResult    = MEMACC_INCONSISTENT;
        }
    }
    if (MEMACC_REQJOB_STOP != RuntimeJobInfo->CurReqJobStatus)
#endif
    {
        MemAcc_UpdateLength(RuntimeJobInfo);

        if (0u == pendJobPtr->ReqlenRemain)
        {
            /** All lthe engths of the requested job have been processed */
            RuntimeJobInfo->CurReqJobStatus = MEMACC_REQJOB_STOP;
        }
        else
        {
            /** Continue processing the job if the Mem driver is still available */
            MemAcc_CalculateLength(RuntimeJobInfo);
            MemAcc_InvocationMemService(RuntimeJobInfo);
        }
    }
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Update the corresponding status of the request job based on the length of the remaining processing
 */
MEMACC_LOCAL void MemAcc_JobSuspendingOk(MemAcc_RunTimeAreaType* RuntimeJobInfo)
{
    MemAcc_PendReqType* pendJobPtr = &(RuntimeJobInfo->CurJobInfo);

    MemAcc_UpdateLength(RuntimeJobInfo);

    if (0u == pendJobPtr->ReqlenRemain)
    {
        /** All lthe engths of the requested job have been processed */
        RuntimeJobInfo->CurReqJobStatus = MEMACC_REQJOB_STOP;
    }
    else
    {
        uint16                        subArea    = pendJobPtr->SubAreaIndex;
        const MemAcc_SubAddrAreaType* subAreaPtr = &(MemAcc_CfgPtr->SubAddrAreaCfg[subArea]);
        /** Release the Mem driver */
        MemAcc_ReqMemHw[subAreaPtr->HardwareId] = MEMACC_MEM_DRIVER_IDLE;
        /** Resume the job */
        RuntimeJobInfo->CurReqJobStatus = MEMACC_REQJOB_RESUMING;
    }
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief If the number of retries is greater than 0, the job request will continue; otherwise, the status will be set
 * to "stop"
 */
MEMACC_LOCAL void MemAcc_JobSuspendingFail(MemAcc_RunTimeAreaType* RuntimeJobInfo, MemAcc_MemJobResultType MemJobResult)
{
    MemAcc_PendReqType* pendJobPtr = &(RuntimeJobInfo->CurJobInfo);

    RuntimeJobInfo->CurJobResult =
        (MemJobResult == MEM_JOB_FAILED)
            ? MEMACC_FAILED
            : (((MemJobResult == MEM_INCONSISTENT)
                    ? MEMACC_INCONSISTENT
                    : ((MemJobResult == MEM_ECC_UNCORRECTED) ? MEMACC_ECC_UNCORRECTED
                                                             : (RuntimeJobInfo->CurJobResult))));

    if (pendJobPtr->ReqJobRetries > 0u)
    {
        uint16                        subArea    = pendJobPtr->SubAreaIndex;
        const MemAcc_SubAddrAreaType* subAreaPtr = &(MemAcc_CfgPtr->SubAddrAreaCfg[subArea]);
        pendJobPtr->ReqJobRetries--;
        /** Release the Mem driver */
        MemAcc_ReqMemHw[subAreaPtr->HardwareId] = MEMACC_MEM_DRIVER_IDLE;
        RuntimeJobInfo->CurReqJobStatus         = MEMACC_REQJOB_RESUMING;
    }
    else
    {
        RuntimeJobInfo->CurReqJobStatus = MEMACC_REQJOB_STOP;
    }
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#if (MEMACC_MEMDRIVER_DIRECT_STATIC_ENABLE != STD_ON)
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief Traverse all the configured drivers. If the requested hardware ID matches and the calling method matches, call
 * the main function of the driver
 */
MEMACC_LOCAL void MemAcc_ScheMemDriMainFuc(const MemAcc_RunTimeAreaType* RuntimeJobInfo)
{
    MemAcc_PendReqType            jobInfo    = RuntimeJobInfo->CurJobInfo;
    uint16                        subArea    = jobInfo.SubAreaIndex;
    const MemAcc_SubAddrAreaType* subAreaPtr = &(MemAcc_CfgPtr->SubAddrAreaCfg[subArea]);

    for (uint8 index = 0u; index < (uint16)MEMACC_DRIVER_NUM; ++index)
    {
        /** Find the mem driver */
        if (subAreaPtr->HardwareId == index)
        {
            if (MEMACC_DIRECT_STATIC != MemAcc_InvocationCfg[index])
            {
                if (MemAcc_MemDriverApi[index].MainFunc != NULL_PTR)
                {
                    MemAcc_MemDriverApi[index].MainFunc();
                }
            }
        }
    }
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
#endif
#define MEMACC_START_SEC_CODE
#include "MemAcc_MemMap.h"
/**
 * @brief If the status is locked, update the lock information and update the lock status of the request job
 */
MEMACC_LOCAL void
    MemAcc_UpdateLockInfo(const MemAcc_AddressAreaIdType AddrAreaId, const MemAcc_LockInfoType* preLockInfo)
{
    if (MEMACC_LOCKED == preLockInfo->LockStatus)
    {
        MemAcc_AddressType      curLockEndAddr;
        MemAcc_RunTimeAreaType* jobRunning   = &(MemAcc_RunTimeArea[AddrAreaId]);
        MemAcc_LockInfoType*    curLockInfo  = &(jobRunning->CurLockInfo);
        MemAcc_PendReqType*     pendJobPtr   = &(jobRunning->CurJobInfo);
        MemAcc_AddressType      reqStartAddr = pendJobPtr->ReqLogicAddress;
        MemAcc_AddressType      reqEndAddr   = reqStartAddr + pendJobPtr->Reqlength;

        curLockInfo->LockStartAddr = preLockInfo->LockStartAddr;
        curLockInfo->LockLength    = preLockInfo->LockLength;
        curLockInfo->LockStatus    = preLockInfo->LockStatus;
        curLockInfo->LockNotif     = preLockInfo->LockNotif;
        curLockEndAddr             = curLockInfo->LockStartAddr + curLockInfo->LockLength;

        /** if requested area is within the locked area */
        if ((reqStartAddr < curLockEndAddr) && (reqEndAddr > curLockInfo->LockStartAddr))
        {
            curLockInfo->JobLocked = TRUE;
        }
        else
        {
            curLockInfo->JobLocked = FALSE;
        }
    }
}
#define MEMACC_STOP_SEC_CODE
#include "MemAcc_MemMap.h"
