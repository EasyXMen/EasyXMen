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
**  FILENAME    : MemIf.c                                                     **
**                                                                            **
**  Created on  : 2022/1/26 15:21:52                                          **
**  Author      : Haibin.Shao                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : MemIf source description                                    **
**                                                                            **
**  SPECIFICATION(S) : AUTOSAR classic Platform R19-11                        **
**                                                                            **
*******************************************************************************/

/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:MemIf<br>
  RuleSorce:puhua-rule.rcf 2.3.1

    \li PRQA S 3432 MISRA Rule 20.7 .<br>
    Reason:Function-like macros are used to allow more efficient code.

    \li PRQA S 1531 MISRA Rule 8.7 .<br>
    Reason:Configuration parameters, necessary design.
 */

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "MemIf.h"

#if (STD_ON == MEMIF_DEV_ERROR_DETECT)
#include "Det.h"
#endif

/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#ifndef MEMIF_AR_RELEASE_MAJOR_VERSION
#error " Nvm version miss"
#endif
#ifndef MEMIF_AR_RELEASE_MINOR_VERSION
#error " Nvm version miss"
#endif
#ifndef MEMIF_AR_RELEASE_PATCH_VERSION
#error " Nvm version miss"
#endif
#if ((2 != MEMIF_CFG_SW_MAJOR_VERSION) || (0 != MEMIF_CFG_SW_MINOR_VERSION) || (0 != MEMIF_CFG_SW_PATCH_VERSION))
#error " cfg version mismatching"
#endif
/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/**** Error Codes in Development-Mode *******/
#define MEMIF_E_PARAM_DEVICE  (0x01u)
#define MEMIF_E_PARAM_POINTER (0x02u)

/*---------MEMIF Service Identification-------------------------------------*/
#define MEMIF_SID_SETMODE        (0x01U)
#define MEMIF_SID_READ           (0x02U)
#define MEMIF_SID_WRITE          (0x03U)
#define MEMIF_SID_CANCEL         (0x04U)
#define MEMIF_SID_GET_STATUS     (0x05U)
#define MEMIF_SID_GET_JOB_RESULT (0x06U)
#define MEMIF_SID_INVALIDATE     (0x07U)
#define MEMIF_SID_VERSION_INFO   (0x08u)
#define MEMIF_SID_ERASE          (0x09u)

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/

#define MEMIF_START_SEC_CODE
#include "MemIf_MemMap.h"
#if (STD_ON == MEMIF_CFG_USE_FCT_API)
/******************************************************************************/
/*
 * Brief               Invokes the "Read" function of the underlying memory abstraction
 *                     module selected by the parameter DeviceIndex.
 * ServiceId           0x02
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DeviceIndex: index number of EEPROM device
 *                     BlockNumber: index number of logic block
 *                     BlockOffset:
 *                     Length:
 * Param-Name[out]     DataBufferPtr:
 * Param-Name[in/out]  None:
 * Return              Std_ReturnType:
 *                     In case development error detection is enabled for the Memory
 *                     Abstraction Interface and a development error is detected according to
 *                     SWS_MemIf_00022 the function shall return E_NOT_OK else it shall
 *                     return the value of the called function of the underlying module.
 */
/******************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
FUNC(Std_ReturnType, MEMIF_CODE)
MemIf_Read(
    VAR(uint8, AUTOMATIC) DeviceIndex,
    VAR(uint16, AUTOMATIC) BlockNumber,
    VAR(uint16, AUTOMATIC) BlockOffset,
    P2VAR(uint8, AUTOMATIC, MEMIF_APPL_DATA) DataBufferPtr,
    VAR(uint16, AUTOMATIC) Length)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    /* ----- Local Variables --------*/
    Std_ReturnType retVal = E_NOT_OK;
    /* ----- Development Error Detection --------*/
#if (MEMIF_DEV_ERROR_DETECT == STD_ON)
    if (MEMIF_NUMBER_OF_DEVICES <= DeviceIndex)
    {
        (void)Det_ReportError(MEMIF_MODULE_ID, 0U, MEMIF_SID_READ, MEMIF_E_PARAM_DEVICE);
    }
    else if (NULL_PTR == DataBufferPtr)
    {
        (void)Det_ReportError(MEMIF_MODULE_ID, 0U, MEMIF_SID_READ, MEMIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        /* ----- Implementation --------*/
        if (MemIf_MemHwaApis_at[DeviceIndex].Read != NULL_PTR)
        {
            retVal = MemIf_MemHwaApis_at[DeviceIndex].Read(BlockNumber, BlockOffset, DataBufferPtr, Length);
        }
    }
    return retVal;
}

/******************************************************************************/
/*
 * Brief               Invokes the "Write" function of the underlying memory abstraction module
 *                     selected by the parameter DeviceIndex.
 * ServiceId           0x03
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DeviceIndex: index number of EEPROM device
 *                     BlockNumber: index number of logic block
 *                     DataBufferPtr:
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              Std_ReturnType:
 *                     In case development error detection is enabled for the Memory
 *                     Abstraction Interface and a development error is detected according to
 *                     SWS_MemIf_00022 the function shall return E_NOT_OK else it shall
 *                     return the value of the called function of the underlying module.
 */
/******************************************************************************/
FUNC(Std_ReturnType, MEMIF_CODE)
MemIf_Write(
    VAR(uint8, AUTOMATIC) DeviceIndex,
    VAR(VAR(uint16, AUTOMATIC), AUTOMATIC) BlockNumber,
    P2CONST(uint8, AUTOMATIC, MEMIF_APPL_CONST) DataBufferPtr)
{
    /* ----- Local Variables --------*/
    Std_ReturnType retVal = E_NOT_OK;
    /* ----- Development Error Detection --------*/
#if (MEMIF_DEV_ERROR_DETECT == STD_ON)
    if (MEMIF_NUMBER_OF_DEVICES <= DeviceIndex)
    {
        (void)Det_ReportError(MEMIF_MODULE_ID, 0U, MEMIF_SID_WRITE, MEMIF_E_PARAM_DEVICE);
    }
    else if (NULL_PTR == DataBufferPtr)
    {
        (void)Det_ReportError(MEMIF_MODULE_ID, 0U, MEMIF_SID_WRITE, MEMIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        /* ----- Implementation --------*/
        if (NULL_PTR != MemIf_MemHwaApis_at[DeviceIndex].Write)
        {
            retVal = MemIf_MemHwaApis_at[DeviceIndex].Write(BlockNumber, DataBufferPtr);
        }
    }
    return retVal;
}

/******************************************************************************/
/*
 * Brief               Invokes the "InvalidateBlock" function of the underlying memory abstraction
 *                     module selected by the parameter DeviceIndex.
 * ServiceId           0x07
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DeviceIndex: index number of EEPROM device
 *                     BlockNumber: index number of logic block
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              Std_ReturnType:
 *                     In case development error detection is enabled for the Memory
 *                     Abstraction Interface and a development error is detected according to
 *                     SWS_MemIf_00022 the function shall return E_NOT_OK else it shall
 *                     return the value of the called function of the underlying module.
 */
/******************************************************************************/
FUNC(Std_ReturnType, MEMIF_CODE)
MemIf_InvalidateBlock(VAR(uint8, AUTOMATIC) DeviceIndex, VAR(uint16, AUTOMATIC) BlockNumber)
{
    /* ----- Local Variables --------*/
    Std_ReturnType retVal = E_NOT_OK;
    /* ----- Development Error Detection --------*/
#if (MEMIF_DEV_ERROR_DETECT == STD_ON)
    if (MEMIF_NUMBER_OF_DEVICES <= DeviceIndex)
    {
        (void)Det_ReportError(MEMIF_MODULE_ID, 0U, MEMIF_SID_INVALIDATE, MEMIF_E_PARAM_DEVICE);
    }
    else
#endif
    {
        /* ----- Implementation --------*/
        if (NULL_PTR != MemIf_MemHwaApis_at[DeviceIndex].InvalidateBlock)
        {
            retVal = MemIf_MemHwaApis_at[DeviceIndex].InvalidateBlock(BlockNumber);
        }
    }
    return retVal;
}

/******************************************************************************/
/*
 * Brief               Invokes the "EraseImmediateBlock" function of the underlying memory
 *                     abstraction module selected by the parameter DeviceIndex.
 * ServiceId           0x09
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DeviceIndex: index number of EEPROM device
 *                     BlockNumber: index number of logic block
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              Std_ReturnType:
 *                     In case development error detection is enabled for the Memory
 *                     Abstraction Interface and a development error is detected according to
 *                     SWS_MemIf_00022 the function shall return E_NOT_OK else it shall
 *                     return the value of the called function of the underlying module.
 */
/******************************************************************************/
FUNC(Std_ReturnType, MEMIF_CODE)
MemIf_EraseImmediateBlock(VAR(uint8, AUTOMATIC) DeviceIndex, VAR(uint16, AUTOMATIC) BlockNumber)
{
    /* ----- Local Variables --------*/
    Std_ReturnType retVal = E_NOT_OK;
    /* ----- Development Error Detection --------*/
#if (MEMIF_DEV_ERROR_DETECT == STD_ON)
    if (MEMIF_NUMBER_OF_DEVICES <= DeviceIndex)
    {
        (void)Det_ReportError(MEMIF_MODULE_ID, 0U, MEMIF_SID_ERASE, MEMIF_E_PARAM_DEVICE);
    }
    else
#endif
    {
        /* ----- Implementation --------*/
        if (NULL_PTR != MemIf_MemHwaApis_at[DeviceIndex].EraseImmediateBlock)
        {
            retVal = MemIf_MemHwaApis_at[DeviceIndex].EraseImmediateBlock(BlockNumber);
        }
    }
    return retVal;
}

/******************************************************************************/
/*
 * Brief               Invokes the "Cancel" function of the underlying memory abstraction
 *                     module selected by the parameter DeviceIndex.
 * ServiceId           0x04
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DeviceIndex: index number of EEPROM device
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              None:
 */
/******************************************************************************/
FUNC(void, MEMIF_CODE) MemIf_Cancel(VAR(uint8, AUTOMATIC) DeviceIndex)
{
    /* ----- Local Variables --------*/
    /* ----- Development Error Detection --------*/
#if (MEMIF_DEV_ERROR_DETECT == STD_ON)
    if (MEMIF_NUMBER_OF_DEVICES <= DeviceIndex)
    {
        (void)Det_ReportError(MEMIF_MODULE_ID, 0U, MEMIF_SID_CANCEL, MEMIF_E_PARAM_DEVICE);
    }
    else
#endif
    {
        /* ----- Implementation --------*/
        if (NULL_PTR != MemIf_MemHwaApis_at[DeviceIndex].Cancel)
        {
            MemIf_MemHwaApis_at[DeviceIndex].Cancel();
        }
    }
}

/******************************************************************************/
/*
 * Brief               Invokes the "GetJobResult" function of the underlying memory abstraction
 *                     module selected by the parameter DeviceIndex.
 * ServiceId           0x06
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DeviceIndex: index number of EEPROM device
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              MemIf_JobResultType:
 *                     In case development error detection is enabled for the Memory
 *                     Abstraction Interface and a development error is detected according to
 *                     SWS_MemIf_00022 the function shall return MEMIF_JOB_FAILED else
 *                     it shall return the value of the called function of the underlying module.
 */
/******************************************************************************/
FUNC(MemIf_JobResultType, MEMIF_CODE) MemIf_GetJobResult(VAR(uint8, AUTOMATIC) DeviceIndex)
{
    /* ----- Local Variables --------*/
    MemIf_JobResultType retVal = MEMIF_JOB_FAILED;
    /* ----- Development Error Detection ---------*/
#if (MEMIF_DEV_ERROR_DETECT == STD_ON)
    if (MEMIF_NUMBER_OF_DEVICES <= DeviceIndex)
    {
        (void)Det_ReportError(MEMIF_MODULE_ID, 0U, MEMIF_SID_GET_JOB_RESULT, MEMIF_E_PARAM_DEVICE);
    }
    else
#endif
    {
        /* ----- Implementation ----------*/
        if (NULL_PTR != MemIf_MemHwaApis_at[DeviceIndex].GetJobResult)
        {
            retVal = MemIf_MemHwaApis_at[DeviceIndex].GetJobResult();
        }
    }
    return retVal;
}
#endif

/******************************************************************************/
/*
 * Brief               Invokes the "SetMode" functions of all underlying memory abstraction modules.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Mode: work mode of EEPROM device
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              None:
 */
/******************************************************************************/
FUNC(void, MEMIF_CODE) MemIf_SetMode(VAR(MemIf_ModeType, AUTOMATIC) Mode)
{
    uint8 iloop;
    /* ----- Implementation --------------------------------------------------*/
    for (iloop = 0U; iloop < MEMIF_NUMBER_OF_DEVICES; iloop++)
    {
        if (NULL_PTR != MemIf_MemHwaApis_at[iloop].SetMode)
        {
            MemIf_MemHwaApis_at[iloop].SetMode(Mode);
        }
    }
}

/******************************************************************************/
/*
 * Brief               Invokes the "GetStatus" function of the underlying memory abstraction
 *                     module selected by the parameter DeviceIndex.
 * ServiceId           0x05
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DeviceIndex: index number of EEPROM device
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              MemIf_StatusType:
 */
/******************************************************************************/
FUNC(MemIf_StatusType, MEMIF_CODE) MemIf_GetStatus(VAR(uint8, AUTOMATIC) DeviceIndex)
{
    /* ----- Local Variables -------*/
    MemIf_StatusType retVal = MEMIF_UNINIT;
    MemIf_StatusType ret = MEMIF_UNINIT;
    uint8 iloop;
    /* ----- Development Error Detection --------*/
#if (MEMIF_DEV_ERROR_DETECT == STD_ON)
    /* Check if DeviceIndex is out of bounds. BroadcastId (0xFF) is a valid input. */
    if (((uint8)MEMIF_BROADCAST_ID != DeviceIndex) && (MEMIF_NUMBER_OF_DEVICES <= DeviceIndex))
    {
        (void)Det_ReportError(MEMIF_MODULE_ID, 0U, MEMIF_SID_GET_STATUS, MEMIF_E_PARAM_DEVICE);
    }
    else
#endif
    {
        /* ----- Implementation ------------------------------------------------------------*/
        if ((uint8)MEMIF_BROADCAST_ID == DeviceIndex)
        {
            retVal = MEMIF_IDLE;
            /* #20 Get status of all configured MemHwA modules */
            for (iloop = 0U; iloop < MEMIF_NUMBER_OF_DEVICES; iloop++)
            {
                if (NULL_PTR != MemIf_MemHwaApis_at[iloop].GetStatus)
                {
                    /* Store each status to status mask */
                    ret = MemIf_MemHwaApis_at[iloop].GetStatus();
                }
                if (ret == MEMIF_UNINIT)
                {
                    retVal = MEMIF_UNINIT;
                }
                else if ((ret == MEMIF_BUSY) && (retVal != MEMIF_UNINIT))
                {
                    retVal = MEMIF_BUSY;
                }
                else if ((ret == MEMIF_BUSY_INTERNAL) && (retVal != MEMIF_BUSY) && (retVal != MEMIF_UNINIT))
                {
                    retVal = MEMIF_BUSY_INTERNAL;
                }
                else
                {
                    /*idle*/
                }
            }
        }
        /* Get status of MemHwA module indexed by DeviceIndex */
        else
        {
            if (NULL_PTR != MemIf_MemHwaApis_at[DeviceIndex].GetStatus)
            {
                retVal = MemIf_MemHwaApis_at[DeviceIndex].GetStatus();
            }
        }
    }
    return retVal;
}

#if (STD_ON == MEMIF_VERSION_INFO_API)
/******************************************************************************/
/*
 * Brief               Service to get the version information of this module.
 * ServiceId           0x08
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None:
 * Param-Name[out]     VersionInfo: Pointer to standard version information structure
 * Param-Name[in/out]  None:
 * Return              None:
 */
/******************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
FUNC(void, MEMIF_CODE) MemIf_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, MEMIF_APPL_DATA) VersionInfoPtr)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    /* ----- Development Error Detection --------*/
#if (STD_ON == MEMIF_DEV_ERROR_DETECT)
    if (NULL_PTR == VersionInfoPtr)
    {
        (void)Det_ReportError(MEMIF_MODULE_ID, 0U, MEMIF_SID_VERSION_INFO, MEMIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        /* ----- Implementation ------------------------------------------------------------*/
        VersionInfoPtr->moduleID = MEMIF_MODULE_ID;
        VersionInfoPtr->vendorID = MEMIF_VENDOR_ID;
        VersionInfoPtr->sw_major_version = MEMIF_SW_MAJOR_VERSION;
        VersionInfoPtr->sw_minor_version = MEMIF_SW_MINOR_VERSION;
        VersionInfoPtr->sw_patch_version = MEMIF_SW_PATCH_VERSION;
    }
}
#endif

#define MEMIF_STOP_SEC_CODE
#include "MemIf_MemMap.h"
