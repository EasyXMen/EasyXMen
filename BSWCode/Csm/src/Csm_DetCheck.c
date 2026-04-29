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
 ***********************************************************************************************************************
 **
 **  @file               : Csm_DetCheck.c
 **  @author             : jie.gu
 **  @date               : 2025/03/31
 **  @vendor             : isoft
 **  @description        : Csm detcheck Interface Implementation source
 **
 **********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Csm_Internal.h"

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
#if (CSM_DEV_ERROR_DETECT == STD_ON)

/**
 * @brief  the initialization status of a service.
 */
static Std_ReturnType Csm_ChkInitStatus(uint8 servId);

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
/*check parameters for  Csm_Init()*/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
boolean Csm_ChkInitParam(const Csm_ConfigType* configPtr)
/* PRQA S 1532 -- */
{
    boolean ret = (boolean)TRUE;
    /*[SWS_Csm_00186]*/
    if (NULL_PTR != configPtr)
    {
        ret = (boolean)FALSE;
        Csm_ReportDetErr(CSM_SID_INIT, CSM_E_INIT_FAILED);
    }
    return ret;
}

/*check parameters for  Csm_GetVersionInfo()*/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType Csm_ChkGetVersionInfo(const Std_VersionInfoType* versioninfo, uint8 sid)
/* PRQA S 1532 -- */
{
    Std_ReturnType ret = E_OK;

    if (NULL_PTR == versioninfo)
    {
        ret = E_NOT_OK;
        Csm_ReportDetErr(sid, CSM_E_PARAM_POINTER);
    }
    return ret;
}

/*check parameters for  Csm_Servicexxx()*/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType Csm_ChkServiceParam(uint32 jobId, uint8 sid)
/* PRQA S 1532 -- */
{
    Std_ReturnType         ret;
    Crypto_ServiceInfoType service;
    ret = Csm_ChkInitStatus(sid);
    if (E_OK == ret)
    {
        if (jobId > (CSM_JOB_NUM - 1uL))
        {
            ret = E_NOT_OK;
            Csm_ReportDetErr(sid, CSM_E_PARAM_HANDLE);
        }
    }
    if (E_OK == ret)
    {
        service = Csm_JobCfg[jobId].jobPrimitiveInfo->primitiveInfo->service;

        if (((CSM_SID_HASH == sid) && (service != CRYPTO_HASH))
            || ((CSM_SID_RANDOM_GEN == sid) && (service != CRYPTO_RANDOMGENERATE))
            || ((CSM_SID_MAC_GEN == sid) && (service != CRYPTO_MACGENERATE))
            || ((CSM_SID_MAC_VERIFY == sid) && (service != CRYPTO_MACVERIFY))
            || ((CSM_SID_AEADENCRYPT == sid) && (service != CRYPTO_AEADENCRYPT))
            || ((CSM_SID_AEADDECRYPT == sid) && (service != CRYPTO_AEADDECRYPT))
            || ((CSM_SID_SIG_GEN == sid) && (service != CRYPTO_SIGNATUREGENERATE))
            || ((CSM_SID_SIG_VERIFY == sid) && (service != CRYPTO_SIGNATUREVERIFY))
            || ((CSM_SID_ENCRYPT == sid) && (service != CRYPTO_ENCRYPT))
            || ((CSM_SID_DECRYPT == sid) && (service != CRYPTO_DECRYPT)))
        {
            ret = E_NOT_OK;
            Csm_ReportDetErr(sid, CSM_E_SERVICE_TYPE);
        }
    }

    return ret;
}

/*check parameters for  Csm_KeyElementSet()*/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType Csm_ChkKeyEleSetParam(uint32 keyId, const uint8* keyPtr, uint32 keyLength, uint8 sid)
/* PRQA S 1532 -- */
{
    Std_ReturnType ret;

    ret = Csm_ChkInitStatus(sid);
    if (E_OK == ret)
    {
        if (keyId > (CSM_KEY_NUM - 1uL))
        {
            ret = E_NOT_OK;
            Csm_ReportDetErr(sid, CSM_E_PARAM_HANDLE);
        }
        else if (NULL_PTR == keyPtr)
        {
            ret = E_NOT_OK;
            Csm_ReportDetErr(sid, CSM_E_PARAM_POINTER);
        }
        else if (0u == keyLength)
        {
            ret = E_NOT_OK;
            Csm_ReportDetErr(sid, CSM_E_PARAM_HANDLE);
        }
        else
        {
            /*Do nothing*/
        }
    }
    return ret;
}

/*check parameters for  Csm_KeySetValid()*/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType Csm_ChkKeySetValid(uint32 keyId, uint8 sid)
/* PRQA S 1532 -- */
{
    Std_ReturnType ret;

    ret = Csm_ChkInitStatus(sid);
    if (E_OK == ret)
    {
        if (keyId > (CSM_KEY_NUM - 1uL))
        {
            ret = E_NOT_OK;
            Csm_ReportDetErr(sid, CSM_E_PARAM_HANDLE);
        }
    }
    return ret;
}
/*check parameters for  Csm_KeySetInvalid()*/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType Csm_ChkKeySetInValid(uint32 keyId, uint8 sid)
/* PRQA S 1532 -- */
{
    Std_ReturnType ret;

    ret = Csm_ChkInitStatus(sid);
    if (E_OK == ret)
    {
        if (keyId > (CSM_KEY_NUM - 1uL))
        {
            ret = E_NOT_OK;
            Csm_ReportDetErr(sid, CSM_E_PARAM_HANDLE);
        }
    }
    return ret;
}
/*check parameters for  Csm_KeyElementGet()*/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType Csm_ChkKeyEleGet(uint32 keyId, const uint8* keyPtr, const uint32* keyLengthPtr, uint8 sid)
/* PRQA S 1532 -- */
{
    Std_ReturnType ret;

    ret = Csm_ChkInitStatus(sid);
    if (E_OK == ret)
    {
        if (keyId > (CSM_KEY_NUM - 1uL))
        {
            ret = E_NOT_OK;
            Csm_ReportDetErr(sid, CSM_E_PARAM_HANDLE);
        }
        else if ((NULL_PTR == keyPtr) || (NULL_PTR == keyLengthPtr))
        {
            ret = E_NOT_OK;
            Csm_ReportDetErr(sid, CSM_E_PARAM_POINTER);
        }
        else if (0u == *keyLengthPtr)
        {
            ret = E_NOT_OK;
            Csm_ReportDetErr(sid, CSM_E_PARAM_HANDLE);
        }
        else
        {
            /*Do nothing*/
        }
    }
    return ret;
}

/*check parameters for  Csm_KeyElementCopy()*/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType Csm_ChkKeyEleCopy(const uint32 keyId, const uint32 targetKeyId, uint8 sid)
/* PRQA S 1532 -- */
{
    Std_ReturnType ret;

    ret = Csm_ChkInitStatus(sid);
    if (E_OK == ret)
    {
        if ((keyId > (CSM_KEY_NUM - 1uL)) || (targetKeyId > (CSM_KEY_NUM - 1uL)))
        {
            ret = E_NOT_OK;
            Csm_ReportDetErr(sid, CSM_E_PARAM_HANDLE);
        }
    }
    return ret;
}

/*check parameters for  Csm_KeyCopy()*/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType Csm_ChkKeyCopy(const uint32 keyId, const uint32 targetKeyId, uint8 sid)
/* PRQA S 1532 -- */
{
    Std_ReturnType ret;

    ret = Csm_ChkInitStatus(sid);
    if (E_OK == ret)
    {
        if ((keyId > (CSM_KEY_NUM - 1uL)) || (targetKeyId > (CSM_KEY_NUM - 1uL)))
        {
            ret = E_NOT_OK;
            Csm_ReportDetErr(sid, CSM_E_PARAM_HANDLE);
        }
    }
    return ret;
}

/*check parameters for  Csm_KeyElementCopyPartial()*/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType Csm_ChkKeyEleCopyPart(uint32 keyId, uint32 targetKeyId, uint8 sid)
/* PRQA S 1532 -- */
{
    Std_ReturnType ret;

    ret = Csm_ChkInitStatus(sid);
    if (E_OK == ret)
    {
        if ((keyId > (CSM_KEY_NUM - 1uL)) || (targetKeyId > (CSM_KEY_NUM - 1uL)))
        {
            ret = E_NOT_OK;
            Csm_ReportDetErr(sid, CSM_E_PARAM_HANDLE);
        }
    }
    return ret;
}

/*check parameters for  Csm_RandomSeed()*/
Std_ReturnType Csm_ChkRandomSeed(uint32 keyId, const uint8* seedPtr, uint32 seedLength, uint8 sid)
{
    Std_ReturnType ret;

    ret = Csm_ChkInitStatus(sid);
    if (E_OK == ret)
    {
        if (keyId > (CSM_KEY_NUM - 1uL))
        {
            ret = E_NOT_OK;
            Csm_ReportDetErr(sid, CSM_E_PARAM_HANDLE);
        }
        else if (NULL_PTR == seedPtr)
        {
            ret = E_NOT_OK;
            Csm_ReportDetErr(sid, CSM_E_PARAM_POINTER);
        }
        else if (0u == seedLength)
        {
            ret = E_NOT_OK;
            Csm_ReportDetErr(sid, CSM_E_PARAM_HANDLE);
        }
        else
        {
            /*Do nothing*/
        }
    }
    return ret;
}

/*check parameters for  Csm_KeyGenerate()*/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType Csm_ChkKeyGenerate(uint32 keyId, uint8 sid)
/* PRQA S 1532 -- */
{
    Std_ReturnType ret;

    ret = Csm_ChkInitStatus(sid);
    if (E_OK == ret)
    {
        if (keyId > (CSM_KEY_NUM - 1uL))
        {
            ret = E_NOT_OK;
            Csm_ReportDetErr(sid, CSM_E_PARAM_HANDLE);
        }
    }
    return ret;
}

/*check parameters for  Csm_KeyDerive()*/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType Csm_ChkKeyDerive(uint32 keyId, uint32 targetKeyId, uint8 sid)
/* PRQA S 1532 -- */
{
    Std_ReturnType ret;

    ret = Csm_ChkInitStatus(sid);
    if (E_OK == ret)
    {
        if ((keyId > (CSM_KEY_NUM - 1uL)) || (targetKeyId > (CSM_KEY_NUM - 1uL)))
        {
            ret = E_NOT_OK;
            Csm_ReportDetErr(sid, CSM_E_PARAM_HANDLE);
        }
    }
    return ret;
}

/*check parameters for  Csm_KeyExchangeCalcPubVal()*/
Std_ReturnType Csm_ChkExCalcPubVal(const uint8* publicValuePtr, const uint32* publicValueLengthPtr, uint8 sid)
{
    Std_ReturnType ret;

    ret = Csm_ChkInitStatus(sid);
    if (E_OK == ret)
    {
        if ((NULL_PTR == publicValuePtr) || (NULL_PTR == publicValueLengthPtr))
        {
            ret = E_NOT_OK;
            Csm_ReportDetErr(sid, CSM_E_PARAM_POINTER);
        }
        else
        {
            /*Do nothing*/
        }
    }
    return ret;
}

/*check parameters for  Csm_KeyExchangeCalcSecret()*/
Std_ReturnType Csm_ChkExCalcSecVal(const uint8* partnerPublicValuePtr, uint32 partnerPublicValueLength, uint8 sid)
{
    Std_ReturnType ret;

    ret = Csm_ChkInitStatus(sid);
    if (E_OK == ret)
    {
        if (NULL_PTR == partnerPublicValuePtr)
        {
            ret = E_NOT_OK;
            Csm_ReportDetErr(sid, CSM_E_PARAM_POINTER);
        }
        else if (0u == partnerPublicValueLength)
        {
            ret = E_NOT_OK;
            Csm_ReportDetErr(sid, CSM_E_PARAM_HANDLE);
        }
        else
        {
            /*Do nothing*/
        }
    }
    return ret;
}

/*check parameters for  Csm_JobKeySetValid()*/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType Csm_ChkJobKeySetValid(uint32 jobId, uint8 sid)
/* PRQA S 1532 -- */
{
    Std_ReturnType ret;

    ret = Csm_ChkInitStatus(sid);
    if (E_OK == ret)
    {
        if (jobId > (CSM_JOB_NUM - 1uL))
        {
            ret = E_NOT_OK;
            Csm_ReportDetErr(sid, CSM_E_PARAM_HANDLE);
        }
        else if (Csm_JobCfg[jobId].jobPrimitiveInfo->primitiveInfo->service != CRYPTO_KEYSETVALID)
        {
            ret = E_NOT_OK;
            Csm_ReportDetErr(sid, CSM_E_SERVICE_TYPE);
        }
        else
        {
            ret = E_OK;
        }
    }
    return ret;
}
/*check parameters for  Csm_JobKeySetInValid()*/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType Csm_ChkJobKeySetInValid(uint32 jobId, uint8 sid)
/* PRQA S 1532 -- */
{
    Std_ReturnType ret;

    ret = Csm_ChkInitStatus(sid);
    if (E_OK == ret)
    {
        if (jobId > (CSM_JOB_NUM - 1uL))
        {
            ret = E_NOT_OK;
            Csm_ReportDetErr(sid, CSM_E_PARAM_HANDLE);
        }
        else if (Csm_JobCfg[jobId].jobPrimitiveInfo->primitiveInfo->service != CRYPTO_KEYSETINVALID)
        {
            ret = E_NOT_OK;
            Csm_ReportDetErr(sid, CSM_E_SERVICE_TYPE);
        }
        else
        {
            ret = E_OK;
        }
    }
    return ret;
}
/*check parameters for Csm_JobRandomSeed()*/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType Csm_ChkJobRandomSeed(uint32 jobId, uint32 keyId, const uint8* seedPtr, uint32 seedLength, uint8 sid)
/* PRQA S 1532 -- */
{
    Std_ReturnType ret;

    ret = Csm_ChkInitStatus(sid);
    if (E_OK == ret)
    {
        if (jobId > (CSM_JOB_NUM - 1uL))
        {
            ret = E_NOT_OK;
            Csm_ReportDetErr(sid, CSM_E_PARAM_HANDLE);
        }
        else if (Csm_JobCfg[jobId].jobPrimitiveInfo->primitiveInfo->service != CRYPTO_RANDOMSEED)
        {
            ret = E_NOT_OK;
            Csm_ReportDetErr(sid, CSM_E_SERVICE_TYPE);
        }
        else
        {
            ret = Csm_ChkRandomSeed(keyId, seedPtr, seedLength, sid);
        }
    }
    return ret;
}

/*check parameters for  Csm_JobKeyGenerate()*/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType Csm_ChkJobKeyGenerate(uint32 jobId, uint32 keyId, uint8 sid)
/* PRQA S 1532 -- */
{
    Std_ReturnType ret;

    ret = Csm_ChkInitStatus(sid);
    if (E_OK == ret)
    {
        if ((keyId > (CSM_KEY_NUM - 1uL)) || (jobId > (CSM_JOB_NUM - 1uL)))
        {
            ret = E_NOT_OK;
            Csm_ReportDetErr(sid, CSM_E_PARAM_HANDLE);
        }
        else if (Csm_JobCfg[jobId].jobPrimitiveInfo->primitiveInfo->service != CRYPTO_KEYGENERATE)
        {
            ret = E_NOT_OK;
            Csm_ReportDetErr(sid, CSM_E_SERVICE_TYPE);
        }
        else
        {
            ret = E_OK;
        }
    }
    return ret;
}

/*check parameters for  Csm_JobKeyDerive()*/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType Csm_ChkJobKeyDerive(uint32 jobId, uint32 keyId, uint32 targetKeyId, uint8 sid)
/* PRQA S 1532 -- */
{
    Std_ReturnType ret;

    ret = Csm_ChkInitStatus(sid);
    if (E_OK == ret)
    {
        if ((keyId > (CSM_KEY_NUM - 1uL)) || (targetKeyId > (CSM_KEY_NUM - 1uL)) || (jobId > (CSM_JOB_NUM - 1uL)))
        {
            ret = E_NOT_OK;
            Csm_ReportDetErr(sid, CSM_E_PARAM_HANDLE);
        }
        else if (Csm_JobCfg[jobId].jobPrimitiveInfo->primitiveInfo->service != CRYPTO_KEYDERIVE)
        {
            ret = E_NOT_OK;
            Csm_ReportDetErr(sid, CSM_E_SERVICE_TYPE);
        }
        else
        {
            ret = E_OK;
        }
    }
    return ret;
}

/*check parameters for  Csm_JobKeyExchangeCalcPubVal()*/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType
    Csm_ChkJobExCalcPubVal(uint32 jobId, const uint8* publicValuePtr, const uint32* publicValueLengthPtr, uint8 sid)
/* PRQA S 1532 -- */
{
    Std_ReturnType ret = E_OK;

    if (jobId > (CSM_JOB_NUM - 1uL))
    {
        ret = E_NOT_OK;
        Csm_ReportDetErr(sid, CSM_E_PARAM_HANDLE);
    }
    else if (Csm_JobCfg[jobId].jobPrimitiveInfo->primitiveInfo->service != CRYPTO_KEYEXCHANGECALCPUBVAL)
    {
        ret = E_NOT_OK;
        Csm_ReportDetErr(sid, CSM_E_SERVICE_TYPE);
    }
    else
    {
        ret = Csm_ChkExCalcPubVal(publicValuePtr, publicValueLengthPtr, sid);
    }

    return ret;
}

/*check parameters for  Csm_JobKeyExchangeCalcSecret()*/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType
    Csm_ChkJobExCalcSec(uint32 jobId, const uint8* partnerPublicValuePtr, uint32 partnerPublicValueLength, uint8 sid)
/* PRQA S 1532 -- */
{
    Std_ReturnType ret;

    ret = Csm_ChkInitStatus(sid);
    if (E_OK == ret)
    {
        if (jobId > (CSM_JOB_NUM - 1uL))
        {
            ret = E_NOT_OK;
            Csm_ReportDetErr(sid, CSM_E_PARAM_HANDLE);
        }
        else if (Csm_JobCfg[jobId].jobPrimitiveInfo->primitiveInfo->service != CRYPTO_KEYEXCHANGECALCSECRET)
        {
            ret = E_NOT_OK;
            Csm_ReportDetErr(sid, CSM_E_SERVICE_TYPE);
        }
        else
        {
            ret = Csm_ChkExCalcSecVal(partnerPublicValuePtr, partnerPublicValueLength, sid);
        }
    }
    return ret;
}

/*check parameters for  Csm_CancelJob()*/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType Csm_ChkCancelJob(uint32 jobId, uint8 sid)
/* PRQA S 1532 -- */
{
    const Csm_JobsCfgType* jobCfgPtr;
    Std_ReturnType         ret;

    ret = Csm_ChkInitStatus(sid);
    if (E_OK == ret)
    {
        if (jobId > (CSM_JOB_NUM - 1uL))
        {
            ret = E_NOT_OK;
            Csm_ReportDetErr(sid, CSM_E_PARAM_HANDLE);
        }
        else
        {
            jobCfgPtr = &(Csm_JobCfg[jobId]);
            /*[SWS_Csm_01086]*/
            if (CRYPTO_PROCESSING_SYNC == jobCfgPtr->jobPrimitiveInfo->processingType)
            {
                ret = E_NOT_OK;
                Csm_ReportDetErr(sid, CSM_E_PROCESSING_MODE);
            }
        }
    }
    return ret;
}

/*check parameters for  Csm_CallbackNotification()*/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType Csm_ChkCbkNotify(const Crypto_JobType* job, uint8 sid)
/* PRQA S 1532 -- */
{
    Std_ReturnType ret;

    ret = Csm_ChkInitStatus(sid);
    if (E_OK == ret)
    {
        if (job->jobId > (CSM_JOB_NUM - 1uL))
        {
            ret = E_NOT_OK;
            Csm_ReportDetErr(sid, CSM_E_PARAM_HANDLE);
        }
    }
    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"

/**
 * @brief          Checks the initialization status of the CSM module.
 * @param[in]      servId : Identifier of the service to check.
 * @return         Std_ReturnType
 * @retval         E_OK: The CSM module is initialized.
 * @retval         E_NOT_OK: The CSM module is not initialized.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71234
 */
static Std_ReturnType Csm_ChkInitStatus(uint8 servId)
{
    Std_ReturnType ret = E_OK;

    if (!Csm_InitStatus)
    {
        ret = E_NOT_OK;
        Csm_ReportDetErr(servId, CSM_E_UNINIT);
    }
    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#endif                /* CSM_DEV_ERROR_DETECT == STD_ON */
/* PRQA S 0553 EOF */ /* VL_Csm_0553 */
