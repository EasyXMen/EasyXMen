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
 **  @file               : Eep_62.c
 **  @author             : peng.wu
 **  @date               : 2024/12/23
 **  @vendor             : isoft
 **  @description        : Implementation for Eep
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Eep_62.h"
#include "Eep_62_Types.h"
#include "Eep_62_Cbk.h"
#include "SchM_Eep_62.h"
#include "Eep_62_Cfg.h"
#if (                                                                                              \
    (EEP_62_DEM_E_COMPARE_FAILED_DETECT == STD_ON) || (EEP_62_DEM_E_WRITE_FAILED_DETECT == STD_ON) \
    || (EEP_62_DEM_E_READ_FAILED_DETECT == STD_ON) || (EEP_62_DEM_E_ERASE_FAILED_DETECT == STD_ON))
#include "Dem.h"
#endif
#if (EEP_62_ECUC_PARTITION_ENABLE == STD_ON)
#include "Os.h"
#endif

/* ===================================================== macros ===================================================== */
/** Size of the buffer used for data verification */
#define EEP_COMPARE_BUF_SIZE (0x400U)

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
#define EEP_62_START_SEC_CODE
#include "Eep_62_MemMap.h"
/**
 * @brief       Check the block is valid before unpacking the block header information
 * @param[in]   deviceIndex: Currently device index
 * @param[in]   Eep_RunData: Currently runtime data parameters
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76989
 */
EEP_62_LOCAL void Eep_62_Erase_Handle(uint8 deviceIndex, Eep_62_RuntimeType* Eep_RunData);

/**
 * @brief       Check the block is valid before unpacking the block header information
 * @param[in]   deviceIndex: Currently device index
 * @param[in]   Eep_RunData: Currently runtime data parameters
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76990
 */
EEP_62_LOCAL void Eep_62_Write_Handle(uint8 deviceIndex, Eep_62_RuntimeType* Eep_RunData);

/**
 * @brief       Check the block is valid before unpacking the block header information
 * @param[in]   deviceIndex: Currently device index
 * @param[in]   Eep_RunData: Currently runtime data parameters
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76991
 */
EEP_62_LOCAL void Eep_62_Read_Handle(uint8 deviceIndex, Eep_62_RuntimeType* Eep_RunData);

/**
 * @brief       Check the block is valid before unpacking the block header information
 * @param[in]   deviceIndex: Currently device index
 * @param[in]   Eep_RunData: Currently runtime data parameters
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76992
 */
EEP_62_LOCAL void Eep_62_Compare_Handle(uint8 deviceIndex, Eep_62_RuntimeType* Eep_RunData);

/**
 * @brief       Check the block is valid before unpacking the block header information
 * @param[in]   deviceIndex: Currently device index
 * @param[in]   Eep_RunData: Currently runtime data parameters
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76938
 */
EEP_62_LOCAL void Eep_62_Compare_HandleSub(uint8 deviceIndex, Eep_62_RuntimeType* Eep_RunData);
#define EEP_62_STOP_SEC_CODE
#include "Eep_62_MemMap.h"

/* ============================================ internal data definitions =========================================== */
#define EEP_62_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Eep_62_MemMap.h"
/**
 * @brief Record the status of the module @range NA
 */
EEP_62_LOCAL MemIf_StatusType Eep_62_Status[EEP_62_NUMBER_OF_EEP_MODULE];

/**
 * @brief Record the request job of the module @range NA
 */
EEP_62_LOCAL MemIf_JobResultType Eep_62_JobResult[EEP_62_NUMBER_OF_EEP_MODULE];

/**
 * @brief Record the runtime data of the module @range NA
 */
EEP_62_LOCAL Eep_62_RuntimeType Eep_62_Runtime[EEP_62_NUMBER_OF_EEP_MODULE];
#define EEP_62_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Eep_62_MemMap.h"

#define EEP_62_START_SEC_VAR_CLEARED_8
#include "Eep_62_MemMap.h"
/**
 * @brief Used for the compare Buffer od this module @range NA
 */
EEP_62_LOCAL uint8 Eep_62_CompareBuffer[EEP_62_NUMBER_OF_EEP_MODULE][EEP_COMPARE_BUF_SIZE];
#define EEP_62_STOP_SEC_VAR_CLEARED_8
#include "Eep_62_MemMap.h"

#define EEP_62_START_SEC_VAR_CLEARED_PTR
#include "Eep_62_MemMap.h"
/**
 * @brief The configuration parameter used to point to the module @range NA
 */
EEP_62_LOCAL const Eep_62_ConfigTypeSet* Eep_62_ConfigPtr;
#define EEP_62_STOP_SEC_VAR_CLEARED_PTR
#include "Eep_62_MemMap.h"

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define EEP_62_START_SEC_CODE
#include "Eep_62_MemMap.h"
/* PRQA S 1503,1532 ++ */ /* VL_Eep_62_1503,VL_QAC_OneFunRef */
/**
 * @brief Check the develop error, initialize global variables and set state machines
 */
void Eep_62_Init(const Eep_62_ConfigType* ConfigPtr)
{
    uint8 iloop = 0u;
#if (STD_ON == EEP_62_DEV_ERROR_DETECT)
    uint8 errorId = EEP_62_E_NO_ERROR;
#if (EEP_62_NUMBER_OF_EEP_MODULE > 1u)
    for (; (iloop < EEP_62_NUMBER_OF_EEP_MODULE) && (EEP_62_E_NO_ERROR == errorId); iloop++)
#endif
    {
        if (MEMIF_UNINIT != Eep_62_Status[iloop])
        {
            errorId = EEP_62_E_INIT_FAILED;
        }
    }
    if ((EEP_62_E_NO_ERROR == errorId) && (NULL_PTR == ConfigPtr))
    {
        errorId = EEP_62_E_PARAM_POINTER;
    }
#if (EEP_62_ECUC_PARTITION_ENABLE == STD_ON)
    if (FALSE == Eep_62_CheckPartition())
    {
        errorId = EEP_62_E_WRONG_CONDITION;
    }
#endif
    if (EEP_62_E_NO_ERROR == errorId)
#endif
    {
        Eep_62_ConfigPtr = ConfigPtr->ConfigSet;
#if (EEP_62_NUMBER_OF_EEP_MODULE > 1u)
        for (iloop = 0u; iloop < EEP_62_NUMBER_OF_EEP_MODULE; iloop++)
#endif
        {
            Eep_62_Status[iloop]    = MEMIF_IDLE;
            Eep_62_JobResult[iloop] = MEMIF_JOB_OK;
            /** parameter init */
            Eep_62_Runtime[iloop].currentJob              = EEP_62_JOB_NONE;
            Eep_62_Runtime[iloop].operateAddr             = 0u;
            Eep_62_Runtime[iloop].dataBufferPtr           = NULL_PTR;
            Eep_62_Runtime[iloop].WriteBuffer             = NULL_PTR;
            Eep_62_Runtime[iloop].length                  = 0u;
            Eep_62_Runtime[iloop].CompareLength           = 0u;
            Eep_62_Runtime[iloop].StateForRunMainFunction = FALSE;
            for (uint16 jloop = 0u; jloop < EEP_COMPARE_BUF_SIZE; jloop++)
            {
                Eep_62_CompareBuffer[iloop][jloop] = 0u;
            }
            if (NULL_PTR != Eep_62_DevHwApis_at[iloop].DevInitApi)
            {
                Eep_62_DevHwApis_at[iloop].DevInitApi();
            }
            /** according to the default mode to set the current mode, read and write size. */
            Eep_62_SetMode(iloop, Eep_62_ConfigPtr[iloop].defaultMode);
        }
    }

#if (STD_ON == EEP_62_DEV_ERROR_DETECT)
    if (errorId != EEP_62_E_NO_ERROR)
    {
        Eep_Det_ReportError(EEP_62_ID_INIT, errorId);
    }
#endif
}

/**
 * @brief Check the develop error, set the parameters according to the request mode
 */
void Eep_62_SetMode(uint8 deviceIndex, MemIf_ModeType Mode)
{
#if (STD_ON == EEP_62_DEV_ERROR_DETECT)
    uint8 errorId = EEP_62_E_NO_ERROR;
    if (MEMIF_UNINIT == Eep_62_Status[deviceIndex])
    {
        errorId = EEP_62_E_UNINT;
    }
    else if (MEMIF_BUSY == Eep_62_Status[deviceIndex])
    {
        Eep_Det_ReportRunTimeError(EEP_62_ID_SETMODE, EEP_62_E_BUSY);
    }
#if (EEP_62_ECUC_PARTITION_ENABLE == STD_ON)
    else if (FALSE == Eep_62_CheckPartition())
    {
        errorId = EEP_62_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        Eep_62_Runtime[deviceIndex].currentMode = Mode;
        if (MEMIF_MODE_SLOW == Eep_62_Runtime[deviceIndex].currentMode)
        {
            /** Check if MEMIF_MODE_SLOW mode is set */
            Eep_62_Runtime[deviceIndex].maxReadSize =
                Eep_62_ConfigPtr[deviceIndex].DeviceConfigPtr->normalReadBlockSize;
            Eep_62_Runtime[deviceIndex].maxWriteSize =
                Eep_62_ConfigPtr[deviceIndex].DeviceConfigPtr->normalWriteBlockSize;
        }
        else if (MEMIF_MODE_FAST == Eep_62_Runtime[deviceIndex].currentMode)
        {
            /** Check if MEMIF_MODE_FAST mode is set */
            Eep_62_Runtime[deviceIndex].maxReadSize = Eep_62_ConfigPtr[deviceIndex].DeviceConfigPtr->fastReadBlockSize;
            Eep_62_Runtime[deviceIndex].maxWriteSize =
                Eep_62_ConfigPtr[deviceIndex].DeviceConfigPtr->fastWriteBlockSize;
        }
        else
        {
            /** do nothing */
        }
    }

#if (STD_ON == EEP_62_DEV_ERROR_DETECT)
    if (errorId != EEP_62_E_NO_ERROR)
    {
        Eep_Det_ReportError(EEP_62_ID_SETMODE, errorId);
    }
#endif
}

/**
 * @brief Check the develop error, set the parameters and request read job
 */
Std_ReturnType
    Eep_62_Read(uint8 deviceIndex, Eep_62_AddressType EepromAddress, uint8* DataBufferPtr, Eep_62_LengthType Length)
{
    Std_ReturnType retVal = E_NOT_OK;
#if (STD_ON == EEP_62_DEV_ERROR_DETECT)
    uint8 errorId = EEP_62_E_NO_ERROR;
    if (MEMIF_UNINIT == Eep_62_Status[deviceIndex])
    {
        errorId = EEP_62_E_UNINT;
    }
    else if ((MEMIF_IDLE != Eep_62_Status[deviceIndex]) || (MEMIF_JOB_PENDING == Eep_62_JobResult[deviceIndex]))
    {
        Eep_Det_ReportRunTimeError(EEP_62_ID_READ, EEP_62_E_BUSY);
    }
    else if (NULL_PTR == DataBufferPtr)
    {
        errorId = EEP_62_E_PARAM_DATA;
    }
    else if (Length > (Eep_62_ConfigPtr[deviceIndex].DeviceConfigPtr->usedSize - 1U))
    {
        errorId = EEP_62_E_PARAM_ADDRESS;
    }
    else if ((Length < 1U) || (Length > (Eep_62_ConfigPtr[deviceIndex].DeviceConfigPtr->usedSize - EepromAddress)))
    {
        errorId = EEP_62_E_PARAM_LENGTH;
    }
#if (EEP_62_ECUC_PARTITION_ENABLE == STD_ON)
    else if (FALSE == Eep_62_CheckPartition())
    {
        errorId = EEP_62_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        Eep_62_Status[deviceIndex]                = MEMIF_BUSY;
        Eep_62_JobResult[deviceIndex]             = MEMIF_JOB_PENDING;
        Eep_62_Runtime[deviceIndex].currentJob    = EEP_62_JOB_READ;
        Eep_62_Runtime[deviceIndex].length        = Length;
        Eep_62_Runtime[deviceIndex].operateAddr   = EepromAddress;
        Eep_62_Runtime[deviceIndex].dataBufferPtr = (uint8*)DataBufferPtr;
        retVal                                    = E_OK;
    }

#if (STD_ON == EEP_62_DEV_ERROR_DETECT)
    if (errorId != EEP_62_E_NO_ERROR)
    {
        Eep_Det_ReportError(EEP_62_ID_READ, errorId);
    }
#endif

    return retVal;
}

/**
 * @brief Check the develop error, set the parameters and request write job
 */
Std_ReturnType Eep_62_Write(
    uint8              deviceIndex,
    Eep_62_AddressType EepromAddress,
    const uint8*       DataBufferPtr,
    Eep_62_LengthType  Length)
{
    Std_ReturnType retVal = E_NOT_OK;
#if (STD_ON == EEP_62_DEV_ERROR_DETECT)
    uint8 errorId = EEP_62_E_NO_ERROR;
    if (MEMIF_UNINIT == Eep_62_Status[deviceIndex])
    {
        errorId = EEP_62_E_UNINT;
    }
    else if ((MEMIF_IDLE != Eep_62_Status[deviceIndex]) || (MEMIF_JOB_PENDING == Eep_62_JobResult[deviceIndex]))
    {
        Eep_Det_ReportRunTimeError(EEP_62_ID_WRITE, EEP_62_E_BUSY);
    }
    else if (NULL_PTR == DataBufferPtr)
    {
        errorId = EEP_62_E_PARAM_DATA;
    }
    else if (Length > (Eep_62_ConfigPtr[deviceIndex].DeviceConfigPtr->usedSize - 1U))
    {
        errorId = EEP_62_E_PARAM_ADDRESS;
    }
    else if ((Length < 1u) || (Length > (Eep_62_ConfigPtr[deviceIndex].DeviceConfigPtr->usedSize - EepromAddress)))
    {
        errorId = EEP_62_E_PARAM_LENGTH;
    }
#if (EEP_62_ECUC_PARTITION_ENABLE == STD_ON)
    else if (FALSE == Eep_62_CheckPartition())
    {
        errorId = EEP_62_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        Eep_62_Status[deviceIndex]              = MEMIF_BUSY;
        Eep_62_JobResult[deviceIndex]           = MEMIF_JOB_PENDING;
        Eep_62_Runtime[deviceIndex].currentJob  = EEP_62_JOB_WRITE;
        Eep_62_Runtime[deviceIndex].length      = Length;
        Eep_62_Runtime[deviceIndex].operateAddr = EepromAddress;
        Eep_62_Runtime[deviceIndex].WriteBuffer = DataBufferPtr;
        retVal                                  = E_OK;
    }

#if (STD_ON == EEP_62_DEV_ERROR_DETECT)
    if (errorId != EEP_62_E_NO_ERROR)
    {
        Eep_Det_ReportError(EEP_62_ID_WRITE, errorId);
    }
#endif

    return retVal;
}

/**
 * @brief Check the develop error, set the parameters and request erase job
 */
Std_ReturnType Eep_62_Erase(uint8 deviceIndex, Eep_62_AddressType EepromAddress, Eep_62_LengthType Length)
{
    Std_ReturnType retVal = E_NOT_OK;
#if (STD_ON == EEP_62_DEV_ERROR_DETECT)
    uint8 errorId = EEP_62_E_NO_ERROR;
    if (MEMIF_UNINIT == Eep_62_Status[deviceIndex])
    {
        errorId = EEP_62_E_UNINT;
    }
    else if ((MEMIF_IDLE != Eep_62_Status[deviceIndex]) || (MEMIF_JOB_PENDING == Eep_62_JobResult[deviceIndex]))
    {
        Eep_Det_ReportRunTimeError(EEP_62_ID_ERASE, EEP_62_E_BUSY);
    }
    else if (Length > (Eep_62_ConfigPtr[deviceIndex].DeviceConfigPtr->usedSize - 1U))
    {
        errorId = EEP_62_E_PARAM_ADDRESS;
    }
    else if ((Length < 1U) || (Length > (Eep_62_ConfigPtr[deviceIndex].DeviceConfigPtr->usedSize - EepromAddress)))
    {
        errorId = EEP_62_E_PARAM_LENGTH;
    }
#if (EEP_62_ECUC_PARTITION_ENABLE == STD_ON)
    else if (FALSE == Eep_62_CheckPartition())
    {
        errorId = EEP_62_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        Eep_62_Status[deviceIndex]              = MEMIF_BUSY;
        Eep_62_JobResult[deviceIndex]           = MEMIF_JOB_PENDING;
        Eep_62_Runtime[deviceIndex].currentJob  = EEP_62_JOB_ERASE;
        Eep_62_Runtime[deviceIndex].length      = Length;
        Eep_62_Runtime[deviceIndex].operateAddr = EepromAddress;
        retVal                                  = E_OK;
    }

#if (STD_ON == EEP_62_DEV_ERROR_DETECT)
    if (errorId != EEP_62_E_NO_ERROR)
    {
        Eep_Det_ReportError(EEP_62_ID_ERASE, errorId);
    }
#endif

    return retVal;
}

/**
 * @brief Check the develop error, set the parameters and request compare job
 */
Std_ReturnType Eep_62_Compare(
    uint8              deviceIndex,
    Eep_62_AddressType EepromAddress,
    const uint8*       DataBufferPtr,
    Eep_62_LengthType  Length)
{
    Std_ReturnType retVal = E_NOT_OK;
#if (STD_ON == EEP_62_DEV_ERROR_DETECT)
    uint8 errorId = EEP_62_E_NO_ERROR;
    if (MEMIF_UNINIT == Eep_62_Status[deviceIndex])
    {
        errorId = EEP_62_E_UNINT;
    }
    else if ((MEMIF_IDLE != Eep_62_Status[deviceIndex]) || (MEMIF_JOB_PENDING == Eep_62_JobResult[deviceIndex]))
    {
        Eep_Det_ReportRunTimeError(EEP_62_ID_COMPARE, EEP_62_E_BUSY);
    }
    else if (NULL_PTR == DataBufferPtr)
    {
        errorId = EEP_62_E_PARAM_DATA;
    }
    else if (Length > (Eep_62_ConfigPtr[deviceIndex].DeviceConfigPtr->usedSize - 1U))
    {
        errorId = EEP_62_E_PARAM_ADDRESS;
    }
    else if ((Length < 1u) || (Length > (Eep_62_ConfigPtr[deviceIndex].DeviceConfigPtr->usedSize - EepromAddress)))
    {
        errorId = EEP_62_E_PARAM_LENGTH;
    }
#if (EEP_62_ECUC_PARTITION_ENABLE == STD_ON)
    else if (FALSE == Eep_62_CheckPartition())
    {
        errorId = EEP_62_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        Eep_62_Status[deviceIndex]                = MEMIF_BUSY;
        Eep_62_JobResult[deviceIndex]             = MEMIF_JOB_PENDING;
        Eep_62_Runtime[deviceIndex].currentJob    = EEP_62_JOB_COMPARE;
        Eep_62_Runtime[deviceIndex].length        = Length;
        Eep_62_Runtime[deviceIndex].CompareLength = Length;
        Eep_62_Runtime[deviceIndex].operateAddr   = EepromAddress;
        Eep_62_Runtime[deviceIndex].dataBufferPtr = &Eep_62_CompareBuffer[deviceIndex][0];
        Eep_62_Runtime[deviceIndex].WriteBuffer   = DataBufferPtr;
        retVal                                    = E_OK;
    }

#if (STD_ON == EEP_62_DEV_ERROR_DETECT)
    if (errorId != EEP_62_E_NO_ERROR)
    {
        Eep_Det_ReportError(EEP_62_ID_COMPARE, errorId);
    }
#endif

    return retVal;
}

/**
 * @brief Check the develop error, set the parameters and request cancel job
 */
void Eep_62_Cancel(uint8 deviceIndex)
{
#if (STD_ON == EEP_62_DEV_ERROR_DETECT)
    uint8 errorId = EEP_62_E_NO_ERROR;
    if (MEMIF_UNINIT == Eep_62_Status[deviceIndex])
    {
        errorId = EEP_62_E_UNINT;
    }
    else if (Eep_62_Runtime[deviceIndex].StateForRunMainFunction)
    {
        errorId = EEP_62_E_IN_MAINF;
    }
#if (EEP_62_ECUC_PARTITION_ENABLE == STD_ON)
    else if (FALSE == Eep_62_CheckPartition())
    {
        errorId = EEP_62_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        Eep_62_Status[deviceIndex]                = MEMIF_IDLE;
        Eep_62_Runtime[deviceIndex].currentJob    = EEP_62_JOB_NONE;
        Eep_62_Runtime[deviceIndex].length        = 0u;
        Eep_62_Runtime[deviceIndex].CompareLength = 0u;
        Eep_62_Runtime[deviceIndex].operateAddr   = 0u;
        Eep_62_Runtime[deviceIndex].dataBufferPtr = NULL_PTR;
        Eep_62_Runtime[deviceIndex].WriteBuffer   = NULL_PTR;
        if (MEMIF_JOB_PENDING == Eep_62_JobResult[deviceIndex])
        {
            Eep_62_JobResult[deviceIndex] = MEMIF_JOB_CANCELED;
            /** call the job Error notification */
            if (NULL_PTR != Eep_62_ConfigPtr[deviceIndex].jobErrorNotification)
            {
                Eep_62_ConfigPtr[deviceIndex].jobErrorNotification();
            }
            else
            {
                /** do nothing */
            }
        }
        else
        {
            /** do nothing */
        }
    }

#if (STD_ON == EEP_62_DEV_ERROR_DETECT)
    if (errorId != EEP_62_E_NO_ERROR)
    {
        Eep_Det_ReportError(EEP_62_ID_CANCEL, errorId);
    }
#endif
}

/**
 * @brief Return the recorded module status
 */
MemIf_StatusType Eep_62_GetStatus(uint8 deviceIndex)
{
    return Eep_62_Status[deviceIndex];
}

/**
 * @brief Return the recorded job result
 */
MemIf_JobResultType Eep_62_GetJobResult(uint8 deviceIndex)
{
    return Eep_62_JobResult[deviceIndex];
}

/**
 * @brief Check the develop error, execute the internal API according to request job
 */
void Eep_62_MainFunction(void)
{
#if (EEP_62_NUMBER_OF_EEP_MODULE > 1u)
    for (uint8 iloop = 0u; iloop < EEP_62_NUMBER_OF_EEP_MODULE; iloop++)
#else
    uint8 iloop = 0u;
#endif
    {
#if (EEP_62_ECUC_PARTITION_ENABLE == STD_ON)
        if (TRUE == Eep_62_CheckPartition())
#endif
        {
            Eep_62_Runtime[iloop].StateForRunMainFunction = (boolean)TRUE;
            if (MEMIF_BUSY == Eep_62_Status[iloop])
            {
                switch (Eep_62_Runtime[iloop].currentJob)
                {
                case EEP_62_JOB_ERASE:
                    Eep_62_Erase_Handle(iloop, &Eep_62_Runtime[iloop]);
                    break;
                case EEP_62_JOB_WRITE:
                    Eep_62_Write_Handle(iloop, &Eep_62_Runtime[iloop]);
                    break;
                case EEP_62_JOB_READ:
                    Eep_62_Read_Handle(iloop, &Eep_62_Runtime[iloop]);
                    break;
                case EEP_62_JOB_COMPARE:
                    Eep_62_Compare_Handle(iloop, &Eep_62_Runtime[iloop]);
                    break;
                default:
                    /** do nothing */
                    break;
                }
            }
            else
            {
                /** Eep_73_Status not in Busy state. IDLE */
            }
            Eep_62_Runtime[iloop].StateForRunMainFunction = (boolean)FALSE;
        }
    }
    return;
}

#if (EEP_62_VERSION_INFO_API == STD_ON)
/**
 * @brief Check the develop error, return the version information
 */
void Eep_62_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
#if (STD_ON == EEP_62_DEV_ERROR_DETECT)
    if (NULL_PTR == versioninfo)
    {
        Eep_Det_ReportError(EEP_62_ID_GETVInfo, EEP_62_E_PARAM_DATA);
    }
    else
#endif
    {
        versioninfo->vendorID         = EEP_62_VENDOR_ID;
        versioninfo->moduleID         = EEP_62_MODULE_ID;
        versioninfo->sw_major_version = EEP_62_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = EEP_62_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = EEP_62_SW_PATCH_VERSION;
    }
}
#endif

#define EEP_62_STOP_SEC_CODE
#include "Eep_62_MemMap.h"

/* ========================================== internal function definitions ========================================= */
#define EEP_62_START_SEC_CODE
#include "Eep_62_MemMap.h"
/**
 * @brief Check the erase API before call it, the error callback is called if the api returns not ok, otherwise the end
 * callback is called
 */
EEP_62_LOCAL void Eep_62_Erase_Handle(uint8 deviceIndex, Eep_62_RuntimeType* Eep_RunData)
{
    Std_ReturnType retVal;
    if (NULL_PTR == Eep_62_DevHwApis_at[deviceIndex].DevEraseApi)
    {
        retVal = E_NOT_OK;
    }
    else
    {
        retVal = Eep_62_DevHwApis_at[deviceIndex].DevEraseApi(
            Eep_RunData->operateAddr,
            EEP_62_ERASE_VALUE,
            Eep_RunData->length);
    }
    if (E_NOT_OK == retVal)
    {
        Eep_62_JobResult[deviceIndex] = MEMIF_JOB_FAILED;
#if (STD_ON == EEP_62_DEM_E_ERASE_FAILED_DETECT)
        (void)Dem_SetEventStatus((uint8)EEP_62_E_ERASE_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
        Eep_62_Status[deviceIndex] = MEMIF_IDLE;
        Eep_RunData->currentJob    = EEP_62_JOB_NONE;
        /** call the job Error notification */
        if (NULL_PTR != Eep_62_ConfigPtr[deviceIndex].jobErrorNotification)
        {
            Eep_62_ConfigPtr[deviceIndex].jobErrorNotification();
        }
    }
    else if (E_OK == retVal)
    {
#if (STD_ON == EEP_62_DEM_E_ERASE_FAILED_DETECT)
        (void)Dem_SetEventStatus((uint8)EEP_62_E_ERASE_FAILED, DEM_EVENT_STATUS_PASSED);
#endif
        if (MEMIF_JOB_PENDING == Eep_62_JobResult[deviceIndex])
        {
            Eep_62_JobResult[deviceIndex] = MEMIF_JOB_OK;
        }
        Eep_62_Status[deviceIndex] = MEMIF_IDLE;
        Eep_RunData->currentJob    = EEP_62_JOB_NONE;
        /** call the job end notification */
        if (NULL_PTR != Eep_62_ConfigPtr[deviceIndex].jobEndNotification)
        {
            Eep_62_ConfigPtr[deviceIndex].jobEndNotification();
        }
    }
    else
    {
        /** do nothing */
    }
}

/**
 * @brief Obtain the write bytes, then check the write API before call it, the error callback is called if the api
 * returns not ok, otherwise the end callback is called
 */
EEP_62_LOCAL void Eep_62_Write_Handle(uint8 deviceIndex, Eep_62_RuntimeType* Eep_RunData)
{
    Std_ReturnType retVal;
    /** get the source address of the Buffer */
    Eep_62_LengthType bytesRemain = Eep_RunData->length;
    Eep_62_LengthType writeBytes  = Eep_RunData->maxWriteSize;
    /** if the send is small then is alignment data */
    if (bytesRemain < writeBytes)
    {
        writeBytes = (Eep_62_LengthType)(bytesRemain);
    }
    if (NULL_PTR == Eep_62_DevHwApis_at[deviceIndex].DevWriteApi)
    {
        retVal = E_NOT_OK;
    }
    else
    {
        retVal = Eep_62_DevHwApis_at[deviceIndex].DevWriteApi(
            Eep_RunData->operateAddr,
            Eep_RunData->WriteBuffer,
            writeBytes);
    }
    if (E_NOT_OK == retVal)
    {
        Eep_62_JobResult[deviceIndex] = MEMIF_JOB_FAILED;
        Eep_62_Status[deviceIndex]    = MEMIF_IDLE;
        Eep_RunData->currentJob       = EEP_62_JOB_NONE;
#if (STD_ON == EEP_62_DEM_E_WRITE_FAILED_DETECT)
        (void)Dem_SetEventStatus((uint8)EEP_62_E_WRITE_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
        /** call the job Error notification*/
        if (NULL_PTR != Eep_62_ConfigPtr[deviceIndex].jobErrorNotification)
        {
            Eep_62_ConfigPtr[deviceIndex].jobErrorNotification();
        }
    }
    else if (E_OK == retVal)
    {
#if (STD_ON == EEP_62_DEM_E_WRITE_FAILED_DETECT)
        (void)Dem_SetEventStatus((uint8)EEP_62_E_WRITE_FAILED, DEM_EVENT_STATUS_PASSED);
#endif
        /** set the next read data address and the source buffer */
        Eep_RunData->operateAddr += (Eep_62_AddressType)writeBytes;
        Eep_RunData->WriteBuffer = &Eep_RunData->WriteBuffer[writeBytes];
        /** update the remain bytes to write */
        Eep_RunData->length -= writeBytes;
        if ((Eep_62_LengthType)0u == Eep_RunData->length)
        {
            if (MEMIF_JOB_PENDING == Eep_62_JobResult[deviceIndex])
            {
                Eep_62_JobResult[deviceIndex] = MEMIF_JOB_OK;
            }
            Eep_62_Status[deviceIndex] = MEMIF_IDLE;
            Eep_RunData->currentJob    = EEP_62_JOB_NONE;
            /** call the job end notification */
            if (NULL_PTR != Eep_62_ConfigPtr[deviceIndex].jobEndNotification)
            {
                Eep_62_ConfigPtr[deviceIndex].jobEndNotification();
            }
        }
    }
    else
    {
        /** do nothing */
    }
}

/**
 * @brief Obtain the read bytes, then check the read API before call it, the error callback is called if the api returns
 * not ok, otherwise the end callback is called
 */
EEP_62_LOCAL void Eep_62_Read_Handle(uint8 deviceIndex, Eep_62_RuntimeType* Eep_RunData)
{
    Std_ReturnType retVal;
    /** get the source address of the Buffer */
    Eep_62_LengthType* bytesRemain = &Eep_RunData->length;
    Eep_62_LengthType  readBytes   = Eep_RunData->maxReadSize;
    /** if the read is small then is alignment data */
    if (*bytesRemain < readBytes)
    {
        readBytes = (Eep_62_LengthType)*bytesRemain;
    }
    if (NULL_PTR == Eep_62_DevHwApis_at[deviceIndex].DevReadApi)
    {
        retVal = E_NOT_OK;
    }
    else
    {
        retVal = Eep_62_DevHwApis_at[deviceIndex].DevReadApi(
            Eep_RunData->operateAddr,
            Eep_RunData->dataBufferPtr,
            readBytes);
    }
    if (E_NOT_OK == retVal)
    {
        Eep_62_JobResult[deviceIndex] = MEMIF_JOB_FAILED;
#if (STD_ON == EEP_62_DEM_E_READ_FAILED_DETECT)
        (void)Dem_SetEventStatus((uint8)EEP_62_E_READ_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
        Eep_62_Status[deviceIndex] = MEMIF_IDLE;
        Eep_RunData->currentJob    = EEP_62_JOB_NONE;
        /** call the job Error notification */
        if (NULL_PTR != Eep_62_ConfigPtr[deviceIndex].jobErrorNotification)
        {
            Eep_62_ConfigPtr[deviceIndex].jobErrorNotification();
        }
    }
    else if (E_OK == retVal)
    {
#if (STD_ON == EEP_62_DEM_E_WRITE_FAILED_DETECT)
        (void)Dem_SetEventStatus((uint8)EEP_62_E_READ_FAILED, DEM_EVENT_STATUS_PASSED);
#endif
        /** set the next read data address and the source buffer */
        Eep_RunData->operateAddr += (Eep_62_AddressType)readBytes;
        Eep_RunData->dataBufferPtr = &Eep_RunData->dataBufferPtr[readBytes];
        /** update the remain bytes to write */
        *bytesRemain -= readBytes;
        if (*bytesRemain == 0u)
        {
            if (MEMIF_JOB_PENDING == Eep_62_JobResult[deviceIndex])
            {
                Eep_62_JobResult[deviceIndex] = MEMIF_JOB_OK;
            }
            Eep_62_Status[deviceIndex] = MEMIF_IDLE;
            Eep_RunData->currentJob    = EEP_62_JOB_NONE;
            /** call the job end notification */
            if (NULL_PTR != Eep_62_ConfigPtr[deviceIndex].jobEndNotification)
            {
                Eep_62_ConfigPtr[deviceIndex].jobEndNotification();
            }
        }
    }
    else
    {
        /** do nothing */
    }
}

/**
 * @brief Obtain the compare bytes, then check the compare API before call it, the error callback is called if the api
 * returns not ok, otherwise the end callback is called
 */
EEP_62_LOCAL void Eep_62_Compare_Handle(uint8 deviceIndex, Eep_62_RuntimeType* Eep_RunData)
{
    Std_ReturnType retVal;
    /** get the runtime data/address of the Buffer */
    const Eep_62_LengthType* bytesRemain  = &Eep_RunData->length;
    Eep_62_LengthType        compareBytes = Eep_RunData->maxReadSize;

    /** if the read is small then is alignment data */
    if (*bytesRemain < compareBytes)
    {
        compareBytes = (Eep_62_LengthType)*bytesRemain;
    }
    if (NULL_PTR == Eep_62_DevHwApis_at[deviceIndex].DevReadApi)
    {
        retVal = E_NOT_OK;
    }
    else
    {
        retVal = Eep_62_DevHwApis_at[deviceIndex].DevReadApi(
            Eep_RunData->operateAddr,
            Eep_RunData->dataBufferPtr,
            compareBytes);
    }
    if (E_NOT_OK == retVal)
    {
        Eep_62_JobResult[deviceIndex] = MEMIF_JOB_FAILED;
        Eep_62_Status[deviceIndex]    = MEMIF_IDLE;
        Eep_RunData->currentJob       = EEP_62_JOB_NONE;
        /** call the job Error notification */
        if (NULL_PTR != Eep_62_ConfigPtr[deviceIndex].jobErrorNotification)
        {
            Eep_62_ConfigPtr[deviceIndex].jobErrorNotification();
        }
    }
    else
    {
        Eep_62_Compare_HandleSub(deviceIndex, Eep_RunData);
    }
}

/**
 * @brief Obtain the compare bytes, then check the compare API before call it, the error callback is called if the api
 * returns not ok, otherwise the end callback is called
 */
EEP_62_LOCAL void Eep_62_Compare_HandleSub(uint8 deviceIndex, Eep_62_RuntimeType* Eep_RunData)
{
    /** get the runtime data/address of the Buffer */
    Eep_62_LengthType* bytesRemain  = &Eep_RunData->length;
    Eep_62_LengthType  compareBytes = Eep_RunData->maxReadSize;

    /** set the next read data address and the source buffer */
    Eep_RunData->operateAddr += (Eep_62_AddressType)compareBytes;
    Eep_RunData->dataBufferPtr = &Eep_RunData->dataBufferPtr[compareBytes];
    /** update the remain bytes to read */
    *bytesRemain -= compareBytes;
    if ((Eep_62_LengthType)0u == *bytesRemain)
    {
        Std_ReturnType compareResult;
        /** get the comparison data/address of the Buffer */
        Eep_62_LengthType compareLength = Eep_RunData->CompareLength;
        const uint8*      referenceData = Eep_RunData->WriteBuffer;

        for (uint16 index = 0u; index < compareLength; index++)
        {
            if (referenceData[index] != Eep_62_CompareBuffer[deviceIndex][index])
            {
                compareResult = E_NOT_OK;
                break;
            }
            else
            {
                compareResult = E_OK;
            }
        }
        if (E_NOT_OK == compareResult) /* PRQA S 2963 */ /* VL_Eep_62_2963 */
        {
            Eep_62_JobResult[deviceIndex] = MEMIF_BLOCK_INCONSISTENT;
#if (STD_ON == EEP_62_DEM_E_COMPARE_FAILED_DETECT)
            (void)Dem_SetEventStatus((uint8)EEP_62_E_COMPARE_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
            Eep_62_Status[deviceIndex] = MEMIF_IDLE;
            Eep_RunData->currentJob    = EEP_62_JOB_NONE;
            /** call the job Error notification */
            if (NULL_PTR != Eep_62_ConfigPtr[deviceIndex].jobErrorNotification)
            {
                Eep_62_ConfigPtr[deviceIndex].jobErrorNotification();
            }
        }
        else if (E_OK == compareResult)
        {
#if (STD_ON == EEP_62_DEM_E_WRITE_FAILED_DETECT)
            (void)Dem_SetEventStatus((uint8)EEP_62_E_COMPARE_FAILED, DEM_EVENT_STATUS_PASSED);
#endif
            if (MEMIF_JOB_PENDING == Eep_62_JobResult[deviceIndex])
            {
                Eep_62_JobResult[deviceIndex] = MEMIF_JOB_OK;
            }
            Eep_62_Status[deviceIndex] = MEMIF_IDLE;
            Eep_RunData->currentJob    = EEP_62_JOB_NONE;
            /** call the job end notification */
            if (NULL_PTR != Eep_62_ConfigPtr[deviceIndex].jobEndNotification)
            {
                Eep_62_ConfigPtr[deviceIndex].jobEndNotification();
            }
        }
        else
        {
            /** idle */
        }
    }
}
/* PRQA S 1503,1532 -- */
#define EEP_62_STOP_SEC_CODE
#include "Eep_62_MemMap.h"
