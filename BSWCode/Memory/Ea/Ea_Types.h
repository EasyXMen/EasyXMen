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
**  FILENAME    : Ea_Types.h                                                  **
**                                                                            **
**  Created on  : 2022/02/15                                                  **
**  Author      : tao.yu                                                      **
**  Vendor      :                                                             **
**  DESCRIPTION : Ea header description                                       **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
#ifndef EA_TYPES_H
#define EA_TYPES_H
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*  <VERSION>    <DATE>      <AUTHOR>        <REVISION LOG>
 *  V1.0.0     2022-04-15     tao.yu         initial version.
 *  V1.0.1     2022-05-13     tao.yu         code review.
 *  V2.0.0     2022-12-13     tao.yu         update version info
 */
/*==============================================================================
*                       INCLUDE FILES
==============================================================================*/
#include "Std_Types.h"
#include "Ea_Cfg.h" /* to get Eep_AddressType and Eep_LengthType from Eep*/
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
typedef P2FUNC(void, EA_CODE, Ea_VoidFunctionVoidPtr)(void);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

typedef enum
{
    EA_JOB_IDLE = 0,       /* no job */
    EA_JOB_READ = 1,       /* read job */
    EA_JOB_WRITE = 2,      /* Write job */
    EA_JOB_CANCEL = 3,     /* cancel job */
    EA_JOB_INVALIDATE = 4, /* inbalidate job */
    EA_JOB_ERASE = 5       /* erase job */
} Ea_RequestJobType;

typedef enum
{
    EA_SUB_JOB_IDLE = 0,           /* no job deal */
    EA_SUB_JOB_READ_HEADER,        /* read job read header */
    EA_SUB_JOB_READ_HEADER_OK,     /* read job read header ok */
    EA_SUB_JOB_READ_DATA,          /* read job read data */
    EA_SUB_JOB_INVALID_HEADER,     /* inbalidate job write header */
    EA_SUB_JOB_INVALID_HEADER_OK,  /* inbalidate job write header ok */
    EA_SUB_JOB_WRITE_DATA,         /* Write job write data */
    EA_SUB_JOB_WRITE_DATA_OK,      /* Write job write data ok */
    EA_SUB_JOB_WRITE_HEADER_VALID, /* Write job write valid header */
} Ea_SubJobStatusType;

typedef enum
{
    EA_EEP_JOB_IDLE = 0,      /* no job deal*/
    EA_EEP_JOB_ASYNC_READY,   /* job ready to request sub mode*/
    EA_EEP_JOB_ASYNC_PENDING, /* requested sub mode, job pending*/
} Ea_EepStateType;

typedef struct
{
    Eep_AddressType BlockStartAddress; /* block start address*/
    uint16 EaBlockNumber;              /*Block identifier (handle).*/
    uint16 EaBlockSize;                /*Size of a logical block in bytes.*/
    boolean EaImmediateData;           /*Marker for high priority data.*/
    uint32 EaNumberOfWriteCycles;      /*Number of write cycles required for this block.*/
    uint8 EaDeviceIndex;               /*Reference to the device this block is stored in.*/
} Ea_BlockConfigurationType;

typedef struct
{
    uint16 BlockNum;                                                  /* block number*/
    P2CONST(Ea_BlockConfigurationType, AUTOMATIC, EA_CONST) BlockCfg; /* block info*/
} Ea_BlockCfgType;

typedef struct
{
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    /*Reads from EEPROM.*/
    P2FUNC(Std_ReturnType, EA_APPL_CODE, Eep_ReadApi)
    (Eep_AddressType EepromAddress, uint8* DataBufferPtr, Eep_LengthType Length);
    /*Writes to EEPROM.*/
    P2FUNC(Std_ReturnType, EA_APPL_CODE, Eep_WriteApi)
    (Eep_AddressType EepromAddress, const uint8* DataBufferPtr, Eep_LengthType Length);
    /*Service for erasing EEPROM sections.*/
    P2FUNC(Std_ReturnType, EA_APPL_CODE, Eep_EraseApi)
    (Eep_AddressType EepromAddress, Eep_LengthType Length);
    /*Cancels a running job.*/
    P2FUNC(void, EA_APPL_CODE, Eep_CancelApi)(void);
    /*Returns the EEPROM status.*/
    P2FUNC(MemIf_StatusType, EA_APPL_CODE, Eep_GetStatusApi)(void);
    /*This service returns the result of the last job.*/
    P2FUNC(MemIf_JobResultType, EA_APPL_CODE, Eep_GetJobResultApi)(void);
    /*Sets the mode.*/
    P2FUNC(void, EA_APPL_CODE, Eep_SetModeApi)(MemIf_ModeType Mode);
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
} Ea_EepApiType;

typedef struct
{
    P2CONST(Ea_BlockCfgType, AUTOMATIC, EA_CONST) Ea_BlockCfg; /* block info*/
    uint8 EaDeviceNum;                                         /* Device number*/
    P2CONST(Ea_EepApiType, AUTOMATIC, EA_CONST) Ea_EepApi;     /* Device api*/
    /*Function to be used by the underlying memory abstraction to signal end of job with error.*/
    Ea_VoidFunctionVoidPtr NvmJobEndNotification;
    /*Function to be used by the underlying memory abstraction to signal end of job without error.*/
    Ea_VoidFunctionVoidPtr NvmJobErrorNotification;
} Ea_ConfigType;

#endif /* EA_TYPES_H */
