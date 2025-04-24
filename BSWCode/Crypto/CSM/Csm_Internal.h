/* PRQA S 3108++ */
/**
 * Copyright (C) 2024 Isoft Infrastructure Software Co., Ltd.
 * SPDX-License-Identifier: LGPL-2.1-only-with-exception OR  LicenseRef-Commercial-License
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the
 * GNU Lesser General Public License as published by the Free Software Foundation; version 2.1.
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License along with this library;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * or see <https://www.gnu.org/licenses/>.
 *
 * Alternatively, this file may be used under the terms of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License, in which case the provisions of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License shall apply instead of those of the GNU Lesser General Public License.
 *
 * You should have received a copy of the Isoft Infrastructure Software Co., Ltd.  Commercial License
 * along with this program. If not, please find it at <https://EasyXMen.com/xy/reference/permissions.html>
 */
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : Csm_Internal.h                                              **
**                                                                            **
**  Created on  :                                                             **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION : Internal implementation header for CSM                      **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/******************************************************************************
**                      Revision Control History                             **
******************************************************************************/
/*  <VERSION>    <DATE>       <AUTHOR>        <REVISION LOG>
 *    V1.0.0     2019-10-18   qinchun.yang     Initial version
 *    V1.1.0     2020-07-16   yuzhe.zhang      R19-11
 *    V2.0.0     2023-02-06   jie.gu           R19_11
 */
#ifndef CSM_INTERNAL_H
#define CSM_INTERNAL_H

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Csm.h"
#include "SchM_Csm.h"
#if (CSM_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif
#include "CryIf.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define CSM_CONST_1            (1U)
#define CSM_CONST_8            (8U)

#define CSM_INVALID_PRIORITY   ((uint32)0xFFFFFFFFU)
#define CSM_INVALID_CBKID      ((uint32)0xFFFFFFFFU)

#define CSM_AVOID_WARNING(var) ((void)var)
/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
/*********pre-compile configuration type definitions********/
/*call back notification function prototype,Reference by SWS_Csm_00970*/
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
typedef P2FUNC(void, CSM_APPL_CODE, Csm_CallbackFuncType)(
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    P2CONST(Crypto_JobType, AUTOMATIC, CSM_APPL_DATA) job,
    Crypto_ResultType result);

/*********Run-time configuration type definitions********/
typedef struct Csm_JobInQueueType
{
    P2VAR(struct Csm_JobInQueueType, TYPEDEF, CSM_VAR_NO_INIT_PTR) next; /* PRQA S 3430 */ /* MISRA Rule 20.7 */
    /*Asynchronous tasks need to be stored JobPrimitiveInputOutput*/
    Crypto_JobPrimitiveInputOutputType jobPriInOut;
    uint32 jobId;
    uint32 jobPriority;
    uint32 bufPos; /*The position in Csm_JobQueBufUsed*/
} Csm_JobInQueueType;

typedef struct Csm_JobInQueueType* Csm_JobInQueuePtrType;

/*runtime status for channel*/
typedef struct
{
    P2VAR(Csm_JobInQueueType, TYPEDEF, CSM_VAR_NO_INIT_PTR) pendingJobQue; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    uint32 maxPriority; /*Max job priority in this queue*/
    uint32 jobCnt;      /*Number of job in this queue*/
    boolean dealSynJobIng;
} Csm_QueueChStatusType;

/*configuration for job notification call back functions*/
typedef struct
{
    uint32 cbkId;
    Csm_CallbackFuncType cbkFnc;
} Csm_JobCbkCfgType;
/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/

/********pre-compile configuration data*********/

#if (CSM_CBKFUNCS_NUM > 0u)
/*callback configuration*/
extern CONST(Csm_JobCbkCfgType, CSM_CONST) Csm_CbkCfg[CSM_CBKFUNCS_NUM];
#endif /*CSM_CBKFUNCS_NUM > 0*/

#if (CSM_KEY_NUM > 0u)
/*key configuration*/
extern CONST(Csm_KeyCfgType, CSM_CONST) Csm_KeyCfg[CSM_KEY_NUM];
#endif /*CSM_KEY_NUM > 0*/

/*queue configuration*/
extern CONST(Csm_QueueCfgType, CSM_CONST) Csm_QueCfg[CSM_QUEUE_NUM];

/*job configuration*/
extern CONST(Csm_JobsCfgType, CSM_CONST) Csm_JobCfg[CSM_JOB_NUM];

/*Module initialize status, TRUE initialized, FALSE not initialized*/

extern VAR(boolean, CSM_VAR_CLEARED_BOOLEAN) Csm_InitStatus;

/*Jobs states*/
extern VAR(Crypto_JobStateType, CSM_VAR_NO_INIT_UNSPECIFIED) Csm_JobStates[CSM_JOB_NUM];

extern VAR(boolean, CSM_VAR_CLEARED_BOOLEAN) Csm_JobPostponeFlag[CSM_JOB_NUM];

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
#if (CSM_DEV_ERROR_DETECT == STD_ON)
FUNC(boolean, CSM_CODE)
Csm_ChkInitParam(P2CONST(Csm_ConfigType, AUTOMATIC, CSM_APPL_DATA) configPtr);

/*check parameters for  Csm_GetVersionInfo()*/
FUNC(Std_ReturnType, CSM_CODE)
Csm_ChkGetVersionInfo(P2CONST(Std_VersionInfoType, AUTOMATIC, CSM_APPL_DATA) versioninfo, VAR(uint8, AUTOMATIC) sid);

/*check parameters for  Csm_Servicexxx()*/
FUNC(Std_ReturnType, CSM_CODE)
Csm_ChkServiceParam(VAR(uint32, AUTOMATIC) jobId, VAR(uint8, AUTOMATIC) sid);

/*check parameters for  CryIf_KeyElementSet()*/
FUNC(Std_ReturnType, CSM_CODE)
Csm_ChkKeyEleSetParam(
    VAR(uint32, AUTOMATIC) keyId,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) keyPtr,
    VAR(uint32, AUTOMATIC) keyLength,
    VAR(uint8, AUTOMATIC) sid);

/*check parameters for  Csm_KeySetValid()*/
FUNC(Std_ReturnType, CSM_CODE)
Csm_ChkKeySetValid(VAR(uint32, AUTOMATIC) keyId, VAR(uint8, AUTOMATIC) sid);

/*check parameters for  Csm_KeyElementGet()*/
FUNC(Std_ReturnType, CSM_CODE)
Csm_ChkKeyEleGet(
    VAR(uint32, AUTOMATIC) keyId,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) keyPtr,
    P2CONST(VAR(uint32, AUTOMATIC), AUTOMATIC, CSM_APPL_DATA) keyLengthPtr,
    VAR(uint8, AUTOMATIC) sid);

/*check parameters for  Csm_KeyElementCopy()*/
FUNC(Std_ReturnType, CSM_CODE)
Csm_ChkKeyEleCopy(
    CONST(VAR(uint32, AUTOMATIC), CSM_APPL_DATA) keyId,
    CONST(VAR(uint32, AUTOMATIC), CSM_APPL_DATA) targetKeyId,
    VAR(uint8, AUTOMATIC) sid);

/*check parameters for  Csm_KeyCopy()*/
FUNC(Std_ReturnType, CSM_CODE)
Csm_ChkKeyCopy(
    CONST(VAR(uint32, AUTOMATIC), CSM_APPL_DATA) keyId,
    CONST(VAR(uint32, AUTOMATIC), CSM_APPL_DATA) targetKeyId,
    VAR(uint8, AUTOMATIC) sid);

/*check parameters for  Csm_KeyElementCopyPartial()*/
FUNC(Std_ReturnType, CSM_CODE)
Csm_ChkKeyEleCopyPart(VAR(uint32, AUTOMATIC) keyId, VAR(uint32, AUTOMATIC) targetKeyId, VAR(uint8, AUTOMATIC) sid);

/*check parameters for  Csm_RandomSeed()*/
FUNC(Std_ReturnType, CSM_CODE)
Csm_ChkRandomSeed(
    VAR(uint32, AUTOMATIC) keyId,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) seedPtr,
    VAR(uint32, AUTOMATIC) seedLength,
    VAR(uint8, AUTOMATIC) sid);

/*check parameters for  Csm_KeyGenerate()*/
FUNC(Std_ReturnType, CSM_CODE)
Csm_ChkKeyGenerate(VAR(uint32, AUTOMATIC) keyId, VAR(uint8, AUTOMATIC) sid);

/*check parameters for  Csm_KeyDerive()*/
FUNC(Std_ReturnType, CSM_CODE)
Csm_ChkKeyDerive(VAR(uint32, AUTOMATIC) keyId, VAR(uint32, AUTOMATIC) targetKeyId, VAR(uint8, AUTOMATIC) sid);

/*check parameters for  Csm_KeyExchangeCalcPubVal()*/
FUNC(Std_ReturnType, CSM_CODE)
Csm_ChkExCalcPubVal(
    VAR(uint32, AUTOMATIC) keyId,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) publicValuePtr,
    P2CONST(uint32, AUTOMATIC, CSM_APPL_DATA) publicValueLengthPtr,
    VAR(uint8, AUTOMATIC) sid);

/*check parameters for  Csm_KeyExchangeCalcSecret()*/
FUNC(Std_ReturnType, CSM_CODE)
Csm_ChkExCalcSecVal(
    VAR(uint32, AUTOMATIC) keyId,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) partnerPublicValuePtr,
    VAR(uint32, AUTOMATIC) partnerPublicValueLength,
    VAR(uint8, AUTOMATIC) sid);

/*check parameters for  Csm_JobKeySetValid()*/
FUNC(Std_ReturnType, CSM_CODE)
Csm_ChkJobKeySetValid(VAR(uint32, AUTOMATIC) jobId, VAR(uint32, AUTOMATIC) keyId, VAR(uint8, AUTOMATIC) sid);

/*check parameters for Csm_JobRandomSeed()*/
FUNC(Std_ReturnType, CSM_CODE)
Csm_ChkJobRandomSeed(
    VAR(uint32, AUTOMATIC) jobId,
    VAR(uint32, AUTOMATIC) keyId,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) seedPtr,
    VAR(uint32, AUTOMATIC) seedLength,
    VAR(uint8, AUTOMATIC) sid);

/*check parameters for  Csm_JobKeyGenerate()*/
FUNC(Std_ReturnType, CSM_CODE)
Csm_ChkJobKeyGenerate(VAR(uint32, AUTOMATIC) jobId, VAR(uint32, AUTOMATIC) keyId, VAR(uint8, AUTOMATIC) sid);

/*check parameters for  Csm_JobKeyDerive()*/
FUNC(Std_ReturnType, CSM_CODE)
Csm_ChkJobKeyDerive(
    VAR(uint32, AUTOMATIC) jobId,
    VAR(uint32, AUTOMATIC) keyId,
    VAR(uint32, AUTOMATIC) targetKeyId,
    VAR(uint8, AUTOMATIC) sid);

/*check parameters for  Csm_JobKeyExchangeCalcPubVal()*/
FUNC(Std_ReturnType, CSM_CODE)
Csm_ChkJobExCalcPubVal(
    VAR(uint32, AUTOMATIC) jobId,
    VAR(uint32, AUTOMATIC) keyId,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) publicValuePtr,
    P2CONST(uint32, AUTOMATIC, CSM_APPL_DATA) publicValueLengthPtr,
    VAR(uint8, AUTOMATIC) sid);

/*check parameters for  Csm_JobKeyExchangeCalcSecret()*/
FUNC(Std_ReturnType, CSM_CODE)
Csm_ChkJobExCalcSec(
    VAR(uint32, AUTOMATIC) jobId,
    VAR(uint32, AUTOMATIC) keyId,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) partnerPublicValuePtr,
    VAR(uint32, AUTOMATIC) partnerPublicValueLength,
    VAR(uint8, AUTOMATIC) sid);

/*check parameters for  Csm_CancelJob()*/
FUNC(Std_ReturnType, CSM_CODE)
Csm_ChkCancelJob(VAR(uint32, AUTOMATIC) jobId, VAR(uint8, AUTOMATIC) sid);

/*check parameters for  Csm_CallbackNotification()*/
FUNC(Std_ReturnType, CSM_CODE)
Csm_ChkCbkNotify(P2CONST(Crypto_JobType, AUTOMATIC, CSM_APPL_DATA) job, VAR(uint8, AUTOMATIC) sid);

#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
#endif /* CSM_INTERNAL_H */
