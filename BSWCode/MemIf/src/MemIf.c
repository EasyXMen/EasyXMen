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
 **  @file               : MemIf.c
 **  @author             : tao.yu
 **  @date               : 2024/09/19
 **  @vendor             : isoft
 **  @description        : Implementation file of MemIf
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "MemIf.h"

#if (STD_ON == MEMIF_DEV_ERROR_DETECT)
#include "Det.h"
#endif

/* ===================================================== macros ===================================================== */
/** Development Errors */
#define MEMIF_E_NO_ERROR      (0x0u)  /** API function called with no det error */
#define MEMIF_E_PARAM_DEVICE  (0x01u) /** API service called with wrong device index parameter */
#define MEMIF_E_PARAM_POINTER (0x02u) /** API service called with NULL pointer argument */

/** API IDs */
#define MEMIF_SID_READ           (0x02u) /** MemIf_Read API */
#define MEMIF_SID_WRITE          (0x03u) /** MemIf_Write API */
#define MEMIF_SID_CANCEL         (0x04u) /** MemIf_Cancel API */
#define MEMIF_SID_GET_STATUS     (0x05u) /** MemIf_GetStatus API */
#define MEMIF_SID_GET_JOB_RESULT (0x06u) /** MemIf_GetJobResult API */
#define MEMIF_SID_INVALIDATE     (0x07u) /** MemIf_InvalidateBlock API */
#define MEMIF_SID_VERSION_INFO   (0x08u) /** MemIf_GetVersionInfo API */
#define MEMIF_SID_ERASE          (0x09u) /** MemIf_EraseImmediateBlock API */

/** This macro definition represents an internal function of the MemIf module */
#ifndef MEMIF_LOCAL
#define MEMIF_LOCAL static /* PRQA S 3414 */ /* VL_MemIf_3414 */
#endif

/** This macro definition represents the MemIf module inline function */
#ifndef MEMIF_LOCAL_INLINE
#define MEMIF_LOCAL_INLINE static inline
#endif

#if (STD_ON == MEMIF_DEV_ERROR_DETECT)
/**
 * @brief          Report develop error
 * @param[in]      ApiId : Specifies which API reported the error.
 * @param[in]      ErrorId : Specify which error was reported.
 * @reentrant      Non Reentrant
 * @synchronous    TRUE
 * @trace       CPD-75689
 */
MEMIF_LOCAL_INLINE void MemIf_DetReportError(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportError(MEMIF_MODULE_ID, MEMIF_INSTANCE_ID, ApiId, ErrorId);
}
#endif

/* PRQA S 0553 EOF */ /* VL_MemIf_0553 */
/* ========================================== external function definitions ========================================= */
/* PRQA S 1503,1532 ++ */ /* VL_MemIf_1503,VL_QAC_OneFunRef */
#if (MEMIF_DEV_ERROR_DETECT == STD_ON)
#define MEMIF_START_SEC_CODE
#include "MemIf_MemMap.h"
/**
 * @brief Invokes the "Read" function of the underlying memory abstraction module selected by the parameter DeviceIndex.
 */
Std_ReturnType
    MemIf_Read(uint16 DeviceIndex, uint16 BlockNumber, uint16 BlockOffset, uint8* DataBufferPtr, uint16 Length)
{
    Std_ReturnType ret     = E_NOT_OK;
    uint8          errorId = MEMIF_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (MEMIF_NUMBER_OF_DEVICES <= DeviceIndex)
    {
        errorId = MEMIF_E_PARAM_DEVICE;
    }
    else if (NULL_PTR == DataBufferPtr)
    {
        errorId = MEMIF_E_PARAM_POINTER;
    }
    else
    {
        ret = MemIf_MemFPtr[DeviceIndex].Read(BlockNumber, BlockOffset, DataBufferPtr, Length);
    }

    /** ***** Development Error Report ***** */
    if (errorId != MEMIF_E_NO_ERROR)
    {
        MemIf_DetReportError(MEMIF_SID_READ, errorId);
    }
    return ret;
}

/**
 * @brief Invokes the "Write" function of the underlying memory abstraction module selected by the parameter
 * DeviceIndex.
 */
Std_ReturnType MemIf_Write(uint16 DeviceIndex, uint16 BlockNumber, const uint8* DataBufferPtr)
{
    Std_ReturnType ret     = E_NOT_OK;
    uint8          errorId = MEMIF_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (MEMIF_NUMBER_OF_DEVICES <= DeviceIndex)
    {
        errorId = MEMIF_E_PARAM_DEVICE;
    }
    else if (NULL_PTR == DataBufferPtr)
    {
        errorId = MEMIF_E_PARAM_POINTER;
    }
    else
    {
        ret = MemIf_MemFPtr[DeviceIndex].Write(BlockNumber, DataBufferPtr);
    }

    /** ***** Development Error Report ***** */
    if (errorId != MEMIF_E_NO_ERROR)
    {
        MemIf_DetReportError(MEMIF_SID_WRITE, errorId);
    }
    return ret;
}

/**
 * @brief Invokes the "InvalidateBlock" function of the underlying memory abstraction module selected by the parameter
 * DeviceIndex.
 */
Std_ReturnType MemIf_InvalidateBlock(uint16 DeviceIndex, uint16 BlockNumber)
{
    Std_ReturnType ret     = E_NOT_OK;
    uint8          errorId = MEMIF_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (MEMIF_NUMBER_OF_DEVICES <= DeviceIndex)
    {
        errorId = MEMIF_E_PARAM_DEVICE;
    }
    else
    {
        ret = MemIf_MemFPtr[DeviceIndex].InvalidateBlock(BlockNumber);
    }

    /** ***** Development Error Report ***** */
    if (errorId != MEMIF_E_NO_ERROR)
    {
        MemIf_DetReportError(MEMIF_SID_INVALIDATE, errorId);
    }
    return ret;
}

/**
 * @brief Invokes the "EraseImmediateBlock" function of the underlying memory abstraction module selected by the
 * parameter DeviceIndex.
 */
Std_ReturnType MemIf_EraseImmediateBlock(uint16 DeviceIndex, uint16 BlockNumber)
{
    Std_ReturnType ret     = E_NOT_OK;
    uint8          errorId = MEMIF_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (MEMIF_NUMBER_OF_DEVICES <= DeviceIndex)
    {
        errorId = MEMIF_E_PARAM_DEVICE;
    }
    else
    {
        ret = MemIf_MemFPtr[DeviceIndex].EraseImmediateBlock(BlockNumber);
    }

    /** ***** Development Error Report ***** */
    if (errorId != MEMIF_E_NO_ERROR)
    {
        MemIf_DetReportError(MEMIF_SID_ERASE, errorId);
    }
    return ret;
}

/**
 * @brief Invokes the "Cancel" function of the underlying memory abstraction module selected by the parameter
 * DeviceIndex.
 */
void MemIf_Cancel(uint16 DeviceIndex)
{
    uint8 errorId = MEMIF_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (MEMIF_NUMBER_OF_DEVICES <= DeviceIndex)
    {
        errorId = MEMIF_E_PARAM_DEVICE;
    }
    else
    {
        MemIf_MemFPtr[DeviceIndex].Cancel();
    }

    /** ***** Development Error Report ***** */
    if (errorId != MEMIF_E_NO_ERROR)
    {
        MemIf_DetReportError(MEMIF_SID_CANCEL, errorId);
    }
}

/**
 * @brief Invokes the "GetJobResult" function of the underlying memory abstraction module selected by the parameter
 * DeviceIndex.
 */
MemIf_JobResultType MemIf_GetJobResult(uint16 DeviceIndex)
{
    MemIf_JobResultType ret;
    uint8               errorId = MEMIF_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (MEMIF_NUMBER_OF_DEVICES <= DeviceIndex)
    {
        errorId = MEMIF_E_PARAM_DEVICE;
        ret     = MEMIF_JOB_FAILED;
    }
    else
    {
        ret = MemIf_MemFPtr[DeviceIndex].GetJobResult();
    }

    /** ***** Development Error Report ***** */
    if (errorId != MEMIF_E_NO_ERROR)
    {
        MemIf_DetReportError(MEMIF_SID_GET_JOB_RESULT, errorId);
    }
    return ret;
}
#define MEMIF_STOP_SEC_CODE
#include "MemIf_MemMap.h"
#endif

#if (MEMIF_DEV_ERROR_DETECT == STD_ON) || (MEMIF_NUMBER_OF_DEVICES > 1U)
#define MEMIF_START_SEC_CODE
#include "MemIf_MemMap.h"
/**
 * @brief Invokes the "GetStatus" function of the underlying memory abstraction module selected by the parameter
 * DeviceIndex.
 */
/* PRQA S 6030 ++ */ /* VL_MTR_MemIf_STMIF */
MemIf_StatusType MemIf_GetStatus(uint16 DeviceIndex)
{
    MemIf_StatusType ret = MEMIF_UNINIT;
    uint8            iloop;
    /* ----- Development Error Detection --------*/
#if (MEMIF_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = MEMIF_E_NO_ERROR;
    /* Check if DeviceIndex is out of bounds. BroadcastId (0xFF) is a valid input. */
    if ((MEMIF_BROADCAST_ID != DeviceIndex) && (MEMIF_NUMBER_OF_DEVICES <= DeviceIndex))
    {
        errorId = MEMIF_E_PARAM_DEVICE;
    }
    else
#endif
    {
        if (MEMIF_BROADCAST_ID == DeviceIndex)
        {
            ret = MEMIF_IDLE;
            for (iloop = 0U; iloop < MEMIF_NUMBER_OF_DEVICES; iloop++)
            {
                MemIf_StatusType status = MemIf_MemFPtr[iloop].GetStatus();
                if (status == MEMIF_UNINIT)
                {
                    ret = MEMIF_UNINIT;
                    break;
                }
                else if (status == MEMIF_BUSY)
                {
                    ret = MEMIF_BUSY;
                }
                else if ((status == MEMIF_BUSY_INTERNAL) && (ret != MEMIF_BUSY))
                {
                    ret = MEMIF_BUSY_INTERNAL;
                }
                else
                {
                    /*idle*/
                }
            }
        }
        else
        {
            ret = MemIf_MemFPtr[DeviceIndex].GetStatus();
        }
    }

#if (STD_ON == MEMIF_DEV_ERROR_DETECT)
    /** ***** Development Error Report ***** */
    if (errorId != MEMIF_E_NO_ERROR)
    {
        MemIf_DetReportError(MEMIF_SID_GET_STATUS, errorId);
    }
#endif
    return ret;
}
/* PRQA S 6030 -- */
#define MEMIF_STOP_SEC_CODE
#include "MemIf_MemMap.h"
#endif

#if (STD_ON == MEMIF_VERSION_INFO_API)
#define MEMIF_START_SEC_CODE
#include "MemIf_MemMap.h"
/**
 * @brief Service to get the version information of this module.
 */
void MemIf_GetVersionInfo(Std_VersionInfoType* VersionInfoPtr)
{
    /** ***** Development Error Checks ***** */
#if (STD_ON == MEMIF_DEV_ERROR_DETECT)
    uint8 errorId = MEMIF_E_NO_ERROR;
    if (NULL_PTR == VersionInfoPtr)
    {
        errorId = MEMIF_E_PARAM_POINTER;
    }
    else
#endif
    {
        VersionInfoPtr->moduleID         = MEMIF_MODULE_ID;
        VersionInfoPtr->vendorID         = MEMIF_VENDOR_ID;
        VersionInfoPtr->sw_major_version = MEMIF_SW_MAJOR_VERSION;
        VersionInfoPtr->sw_minor_version = MEMIF_SW_MINOR_VERSION;
        VersionInfoPtr->sw_patch_version = MEMIF_SW_PATCH_VERSION;
    }

#if (STD_ON == MEMIF_DEV_ERROR_DETECT)
    /** ***** Development Error Report ***** */
    if (errorId != MEMIF_E_NO_ERROR)
    {
        MemIf_DetReportError(MEMIF_SID_VERSION_INFO, errorId);
    }
#endif
}
#define MEMIF_STOP_SEC_CODE
#include "MemIf_MemMap.h"
#endif
/* PRQA S 1503,1532 -- */

/* ----------------------------------------------------- end of file ------------------------------------------------ */
