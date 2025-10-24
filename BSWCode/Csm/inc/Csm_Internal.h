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
 **  @file               : Csm_Internal.h
 **  @author             : jie.gu
 **  @date               : 2025/03/31
 **  @vendor             : isoft
 **  @description        : Csm internal header for internal API declarations
 **
 ***********************************************************************************************************************/

#ifndef CSM_INTERNAL_H
#define CSM_INTERNAL_H

/* =================================================== inclusions =================================================== */
#include "Csm.h"
#include "SchM_Csm.h"
#if (CSM_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif
#include "CryIf.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */
#if (CSM_DEV_ERROR_DETECT == STD_ON)
/**
 * @brief          Reports a development error to the DET (Development Error Tracer).
 * @param[in]      serviceId : Identifier of the service that detected the error.
 * @param[in]      error : Error code of the detected error.
 * @reentrant      Not reentrant
 * @synchronous    TRUE
 * @trace       CPD-71205
 */
static inline void Csm_ReportDetErr(uint8 serviceId, uint8 error)
{
    (void)Det_ReportError(CSM_MODULE_ID, CSM_MODULE_INSTANCE_ID, (serviceId), (error));
}
static inline void Csm_ReportRuntimeErr(uint8 serviceId, uint8 error)
{
    (void)Det_ReportRuntimeError(CSM_MODULE_ID, CSM_MODULE_INSTANCE_ID, (serviceId), (error));
}
#endif

/* constant definitions */
#define CSM_CONST_1 (1U) /** @brief  Constant value 1 used for various purposes in the CSM module */

#define CSM_CONST_8 (8U) /** @brief  Constant value 8 used for various purposes in the CSM module */

#define CSM_CONST_8L (8uL) /** @brief  Constant value 8 used for various purposes in the CSM module */

/* invalid priority and callback id  */
#define CSM_INVALID_PRIORITY ((uint32)0xFFFFFFFFuL) /** @brief  Value indicating an invalid priority */

#define CSM_INVALID_CBKID ((uint32)0xFFFFFFFFuL) /** @brief  Value indicating an invalid callback ID */

/* ========================================= type definition ========================================= */
/*pre-compile configuration type definitions*/
/*call back notification function prototype,Reference by SWS_Csm_00970*/
/* PRQA S 3432 ++ */ /* VL_Csm_3432 */
/**
 * @brief     Type definition for callback function pointer.
 */
typedef void (*Csm_CallbackFuncType)(
    /* PRQA S 3432 -- */
    const Crypto_JobType* job,
    Crypto_ResultType     result);

/** @brief Run-time configuration type definitions*/
typedef struct Csm_JobInQueueTag
{
    struct Csm_JobInQueueTag* next; /* PRQA S 3430 */ /* VL_Csm_3430 */
    /*Asynchronous tasks need to be stored JobPrimitiveInputOutput*/
    Crypto_JobPrimitiveInputOutputType jobPriInOut;
    uint32                             jobId;       /* The job id of this job */
    uint32                             jobPriority; /* The priority of this job */
    uint32                             bufPos;      /*The position in Csm_JobQueBufUsed*/
} Csm_JobInQueueType;

/** @brief type definition for the pointer of the job in queue*/
typedef Csm_JobInQueueType* Csm_JobInQueuePtrType;

/**
 * @brief     Type definition for the runtime status of a channel.
 */
typedef struct Csm_QueueChStatusTag
{
    Csm_JobInQueueType* pendingJobQue; /* PRQA S 3432 */ /* VL_Csm_3432 */
    uint32              maxPriority;                     /*Max job priority in this queue*/
    uint32              jobCnt;                          /*Number of job in this queue*/
    boolean             dealSynJobIng;
} Csm_QueueChStatusType;

/**
 * @brief     Type definition for job notification callback configuration.
 */
typedef struct Csm_JobCbkCfgTag
{
    uint32               cbkId;  /*Id of call back function*/
    Csm_CallbackFuncType cbkFnc; /* Call back function*/
} Csm_JobCbkCfgType;

/* ========================================= external data declarations ========================================= */
/********pre-compile configuration data*********/

#if (CSM_CBKFUNCS_NUM > 0u)
/**
 * @brief Configuration array for CSM callback functions.
 */
extern const Csm_JobCbkCfgType Csm_CbkCfg[CSM_CBKFUNCS_NUM];
#endif /*CSM_CBKFUNCS_NUM > 0*/

#if (CSM_KEY_NUM > 0u)
/**
 * @brief Configuration array for CSM keys.
 */
extern const Csm_KeyCfgType Csm_KeyCfg[CSM_KEY_NUM];
#endif /*CSM_KEY_NUM > 0*/

/**
 * @brief Configuration array for CSM queues.
 */
extern const Csm_QueueCfgType Csm_QueCfg[CSM_QUEUE_NUM];

/**
 * @brief Configuration array for CSM jobs.
 */
extern const Csm_JobsCfgType Csm_JobCfg[CSM_JOB_NUM];

/*Module initialize status, TRUE initialized, FALSE not initialized*/

extern boolean Csm_InitStatus;

/*Jobs states*/
extern Crypto_JobStateType Csm_JobStates[CSM_JOB_NUM];
/* Job postpone flag, TRUE job is postpone, FALSE job is not postpone */
extern boolean Csm_JobPostponeFlag[CSM_JOB_NUM];

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
#if (CSM_DEV_ERROR_DETECT == STD_ON)
/**
 * @brief Checks the initialization parameters for the CSM module.
 *
 * This function validates the provided configuration parameters to ensure they are correct and suitable for
 * initializing the CSM module.
 *
 * @param[in] configPtr: Pointer to the configuration structure containing initialization parameters.
 *
 * @return The function returns TRUE if the parameters are valid, otherwise it returns FALSE.
 *
 * @reentrant      Reentrant but not for the same timer instance
 * @synchronous    TRUE
 * @trace       CPD-71206
 */
boolean Csm_ChkInitParam(const Csm_ConfigType* configPtr);

/**
 * @brief Checks the parameters for the Csm_GetVersionInfo function.
 *
 * This function validates the provided parameters to ensure they are correct and suitable for the Csm_GetVersionInfo
 * function.
 *
 * @param[in] versioninfo: Pointer to the version information structure.
 * @param[in] sid: Service ID for which the version information is requested.
 *
 * @return The function returns E_OK if the parameters are valid, otherwise it returns E_NOT_OK.
 *
 * @reentrant      Reentrant but not for the same timer instance
 * @synchronous    TRUE
 * @trace       CPD-71207
 */
Std_ReturnType Csm_ChkGetVersionInfo(const Std_VersionInfoType* versioninfo, uint8 sid);

/**
 * @brief Checks the parameters for the Csm_Servicexxx function.
 *
 * This function validates the provided parameters to ensure they are correct and suitable for the Csm_Servicexxx
 * function.
 *
 * @param[in] jobId: Job identifier.
 * @param[in] sid: Service ID.
 *
 * @return The function returns E_OK if the parameters are valid, otherwise it returns E_NOT_OK.
 *
 * @reentrant      Reentrant but not for the same timer instance
 * @synchronous    TRUE
 * @trace       CPD-71208
 */
Std_ReturnType Csm_ChkServiceParam(uint32 jobId, uint8 sid);

/**
 * @brief Checks the parameters for the CryIf_KeyElementSet function.
 *
 * This function validates the provided parameters to ensure they are correct and suitable for the CryIf_KeyElementSet
 * function.
 *
 * @param[in] keyId: Key identifier.
 * @param[in] keyPtr: Pointer to the key data.
 * @param[in] keyLength: Length of the key data.
 * @param[in] sid: Service ID.
 *
 * @return The function returns E_OK if the parameters are valid, otherwise it returns E_NOT_OK.
 *
 * @reentrant      Reentrant but not for the same timer instance
 * @synchronous    TRUE
 * @trace       CPD-71209
 */
Std_ReturnType Csm_ChkKeyEleSetParam(uint32 keyId, const uint8* keyPtr, uint32 keyLength, uint8 sid);

/**
 * @brief Checks the parameters for the Csm_KeySetValid function.
 *
 * This function validates the provided parameters to ensure they are correct and suitable for the Csm_KeySetValid
 * function.
 *
 * @param[in] keyId: Key identifier.
 * @param[in] sid: Service ID.
 *
 * @return The function returns E_OK if the parameters are valid, otherwise it returns E_NOT_OK.
 *
 * @reentrant      Reentrant but not for the same timer instance
 * @synchronous    TRUE
 * @trace       CPD-71210
 */
Std_ReturnType Csm_ChkKeySetValid(uint32 keyId, uint8 sid);
/**
 * @brief Checks the parameters for the Csm_KeySetInvalid function.
 *
 * This function validates the provided parameters to ensure they are correct and suitable for the Csm_KeySetInvalid
 * function.
 *
 * @param[in] keyId: Key identifier.
 * @param[in] sid: Service ID.
 *
 * @return The function returns E_OK if the parameters are valid, otherwise it returns E_NOT_OK.
 *
 * @reentrant      Reentrant but not for the same timer instance
 * @synchronous    TRUE
 * @trace       CPD-71995
 */
Std_ReturnType Csm_ChkKeySetInValid(uint32 keyId, uint8 sid);

/**
 * @brief Checks the parameters for the Csm_KeyElementGet function.
 *
 * This function validates the provided parameters to ensure they are correct and suitable for the Csm_KeyElementGet
 * function.
 *
 * @param[in] keyId: Key identifier.
 * @param[out] keyPtr: Pointer to the buffer where the key data will be stored.
 * @param[out] keyLengthPtr: Pointer to the variable where the length of the key data will be stored.
 * @param[in] sid: Service ID.
 *
 * @return The function returns E_OK if the parameters are valid, otherwise it returns E_NOT_OK.
 *
 * @reentrant      Reentrant but not for the same timer instance
 * @synchronous    TRUE
 * @trace       CPD-71211
 */
Std_ReturnType Csm_ChkKeyEleGet(uint32 keyId, const uint8* keyPtr, const uint32* keyLengthPtr, uint8 sid);

/**
 * @brief Checks the parameters for the Csm_KeyElementCopy function.
 *
 * This function validates the provided parameters to ensure they are correct and suitable for the Csm_KeyElementCopy
 * function.
 *
 * @param[in] keyId: Source key identifier.
 * @param[in] targetKeyId: tKeyId Target key identifier.
 * @param[in] sid: Service ID.
 *
 * @return The function returns E_OK if the parameters are valid, otherwise it returns E_NOT_OK.
 *
 * @reentrant      Reentrant but not for the same timer instance
 * @synchronous    TRUE
 * @trace       CPD-71212
 */
Std_ReturnType Csm_ChkKeyEleCopy(const uint32 keyId, const uint32 targetKeyId, uint8 sid);

/**
 * @brief Checks the parameters for the Csm_KeyCopy function.
 *
 * This function validates the provided parameters to ensure they are correct and suitable for the Csm_KeyCopy function.
 *
 * @param[in] keyId: Source key identifier.
 * @param[in] targetKeyId: Target key identifier.
 * @param[in] sid: Service ID.
 *
 * @return The function returns E_OK if the parameters are valid, otherwise it returns E_NOT_OK.
 *
 * @reentrant      Reentrant but not for the same timer instance
 * @synchronous    TRUE
 * @trace       CPD-71213
 */
Std_ReturnType Csm_ChkKeyCopy(const uint32 keyId, const uint32 targetKeyId, uint8 sid);

/**
 * @brief Checks the parameters for the Csm_KeyElementCopyPartial function.
 *
 * This function validates the provided parameters to ensure they are correct and suitable for the
 * Csm_KeyElementCopyPartial function.
 *
 * @param[in] keyId: Source key identifier.
 * @param[in] targetKeyId: Target key identifier.
 * @param[in] sid: Service ID.
 *
 * @return The function returns E_OK if the parameters are valid, otherwise it returns E_NOT_OK.
 *
 * @reentrant      Reentrant but not for the same timer instance
 * @synchronous    TRUE
 * @trace       CPD-71214
 */
Std_ReturnType Csm_ChkKeyEleCopyPart(uint32 keyId, uint32 targetKeyId, uint8 sid);

/**
 * @brief Checks the parameters for the Csm_RandomSeed function.
 *
 * This function validates the provided parameters to ensure they are correct and suitable for the Csm_RandomSeed
 * function.
 *
 * @param[in] keyId: Key identifier.
 * @param[in] seedPtr: Pointer to the random seed data.
 * @param[in] seedLength: Length of the random seed data.
 * @param[in] sid: Service ID.
 *
 * @return The function returns E_OK if the parameters are valid, otherwise it returns E_NOT_OK.
 *
 * @reentrant      Reentrant but not for the same timer instance
 * @synchronous    TRUE
 * @trace       CPD-71215
 */
Std_ReturnType Csm_ChkRandomSeed(uint32 keyId, const uint8* seedPtr, uint32 seedLength, uint8 sid);

/**
 * @brief Checks the parameters for the Csm_KeyGenerate function.
 *
 * This function validates the provided parameters to ensure they are correct and suitable for the Csm_KeyGenerate
 * function.
 *
 * @param[in] keyId: Key identifier.
 * @param[in] sid: Service ID.
 *
 * @return The function returns E_OK if the parameters are valid, otherwise it returns E_NOT_OK.
 *
 * @reentrant      Reentrant but not for the same timer instance
 * @synchronous    TRUE
 * @trace       CPD-71216
 */
Std_ReturnType Csm_ChkKeyGenerate(uint32 keyId, uint8 sid);

/**
 * @brief Checks the parameters for the Csm_KeyDerive function.
 *
 * This function validates the provided parameters to ensure they are correct and suitable for the Csm_KeyDerive
 * function.
 *
 * @param[in] keyId: Source key identifier.
 * @param[in] targetKeyId: Target key identifier.
 * @param[in] sid: Service ID.
 *
 * @return The function returns E_OK if the parameters are valid, otherwise it returns E_NOT_OK.
 *
 * @reentrant      Reentrant but not for the same timer instance
 * @synchronous    TRUE
 * @trace       CPD-71217
 */
Std_ReturnType Csm_ChkKeyDerive(uint32 keyId, uint32 targetKeyId, uint8 sid);

/**
 * @brief Checks the parameters for the Csm_KeyExchangeCalcPubVal function.
 *
 * This function validates the provided parameters to ensure they are correct and suitable for the
 * Csm_KeyExchangeCalcPubVal function.
 *
 * @param[out] publicValuePtr: Pointer to the buffer where the public value will be stored.
 * @param[out] publicValueLengthPtr: Pointer to the variable where the length of the public value will be stored.
 * @param[in] sid: Service ID.
 *
 * @return The function returns E_OK if the parameters are valid, otherwise it returns E_NOT_OK.
 *
 * @reentrant      Reentrant but not for the same timer instance
 * @synchronous    TRUE
 * @trace       CPD-71218
 */
Std_ReturnType Csm_ChkExCalcPubVal(const uint8* publicValuePtr, const uint32* publicValueLengthPtr, uint8 sid);

/**
 * @brief Checks the parameters for the Csm_KeyExchangeCalcSecret function.
 *
 * This function validates the provided parameters to ensure they are correct and suitable for the
 * Csm_KeyExchangeCalcSecret function.
 *
 * @param[in] partnerPublicValuePtr: Pointer to the partner's public value data.
 * @param[in] partnerPublicValueLength: Length of the partner's public value data.
 * @param[in] sid: Service ID.
 *
 * @return The function returns E_OK if the parameters are valid, otherwise it returns E_NOT_OK.
 *
 * @reentrant      Reentrant but not for the same timer instance
 * @synchronous    TRUE
 * @trace       CPD-71219
 */
Std_ReturnType Csm_ChkExCalcSecVal(const uint8* partnerPublicValuePtr, uint32 partnerPublicValueLength, uint8 sid);

/**
 * @brief Checks the parameters for the Csm_JobKeySetValid function.
 *
 * This function validates the provided parameters to ensure they are correct and suitable for the Csm_JobKeySetValid
 * function.
 *
 * @param[in] jobId: Job identifier.
 * @param[in] sid: Service ID.
 *
 * @return The function returns E_OK if the parameters are valid, otherwise it returns E_NOT_OK.
 *
 * @reentrant      Reentrant but not for the same timer instance
 * @synchronous    TRUE
 * @trace       CPD-71220
 */
Std_ReturnType Csm_ChkJobKeySetValid(uint32 jobId, uint8 sid);
/**
 * @brief Checks the parameters for the Csm_JobKeySetInValid function.
 *
 * This function validates the provided parameters to ensure they are correct and suitable for the Csm_JobKeySetInValid
 * function.
 *
 * @param[in] jobId: Job identifier.
 * @param[in] sid: Service ID.
 *
 * @return The function returns E_OK if the parameters are valid, otherwise it returns E_NOT_OK.
 *
 * @reentrant      Reentrant but not for the same timer instance
 * @synchronous    TRUE
 * @trace       CPD-71997
 */
Std_ReturnType Csm_ChkJobKeySetInValid(uint32 jobId, uint8 sid);

/**
 * @brief Checks the parameters for the Csm_JobRandomSeed function.
 *
 * This function validates the provided parameters to ensure they are correct and suitable for the Csm_JobRandomSeed
 * function.
 *
 * @param[in] jobId: Job identifier.
 * @param[in] keyId: Key identifier.
 * @param[in] seedPtr: Pointer to the random seed data.
 * @param[in] seedLength: Length of the random seed data.
 * @param[in] sid: Service ID.
 *
 * @return The function returns E_OK if the parameters are valid, otherwise it returns E_NOT_OK.
 *
 * @reentrant      Reentrant but not for the same timer instance
 * @synchronous    TRUE
 * @trace       CPD-71221
 */
Std_ReturnType Csm_ChkJobRandomSeed(uint32 jobId, uint32 keyId, const uint8* seedPtr, uint32 seedLength, uint8 sid);

/**
 * @brief Checks the parameters for the Csm_JobKeyGenerate function.
 *
 * This function validates the provided parameters to ensure they are correct and suitable for the Csm_JobKeyGenerate
 * function.
 *
 * @param[in] jobId: Job identifier.
 * @param[in] keyId: Key identifier.
 * @param[in] sid: Service ID.
 *
 * @return The function returns E_OK if the parameters are valid, otherwise it returns E_NOT_OK.
 *
 * @reentrant      Reentrant but not for the same timer instance
 * @synchronous    TRUE
 * @trace       CPD-71222
 */
Std_ReturnType Csm_ChkJobKeyGenerate(uint32 jobId, uint32 keyId, uint8 sid);

/**
 * @brief Checks the parameters for the Csm_JobKeyDerive function.
 *
 * This function validates the provided parameters to ensure they are correct and suitable for the Csm_JobKeyDerive
 * function.
 *
 * @param[in] jobId: Job identifier.
 * @param[in] keyId: Source key identifier.
 * @param[in] targetKeyId: Target key identifier.
 * @param[in] sid: Service ID.
 *
 * @return The function returns E_OK if the parameters are valid, otherwise it returns E_NOT_OK.
 *
 * @reentrant      Reentrant but not for the same timer instance
 * @synchronous    TRUE
 * @trace       CPD-71223
 */
Std_ReturnType Csm_ChkJobKeyDerive(uint32 jobId, uint32 keyId, uint32 targetKeyId, uint8 sid);

/**
 * @brief Checks the parameters for the Csm_JobKeyExchangeCalcPubVal function.
 *
 * This function validates the provided parameters to ensure they are correct and suitable for the
 * Csm_JobKeyExchangeCalcPubVal function.
 *
 * @param[in] jobId :Job identifier.
 * @param[out] publicValuePtr: Pointer to the buffer where the public value will be stored.
 * @param[out] publicValueLengthPtr: Pointer to the variable where the length of the public value will be stored.
 * @param[in] sid: Service ID.
 *
 * @return The function returns E_OK if the parameters are valid, otherwise it returns E_NOT_OK.
 *
 * @reentrant      Reentrant but not for the same timer instance
 * @synchronous    TRUE
 * @trace       CPD-71224
 */
Std_ReturnType
    Csm_ChkJobExCalcPubVal(uint32 jobId, const uint8* publicValuePtr, const uint32* publicValueLengthPtr, uint8 sid);

/**
 * @brief Checks the parameters for the Csm_JobKeyExchangeCalcSecret function.
 *
 * This function validates the provided parameters to ensure they are correct and suitable for the
 * Csm_JobKeyExchangeCalcSecret function.
 *
 * @param[in] jobId: Job identifier.
 * @param[in] partnerPublicValuePtr: Pointer to the partner's public value data.
 * @param[in] partnerPublicValueLength: Length of the partner's public value data.
 * @param[in] sid: Service ID.
 *
 * @return The function returns E_OK if the parameters are valid, otherwise it returns E_NOT_OK.
 *
 * @reentrant      Reentrant but not for the same timer instance
 * @synchronous    TRUE
 * @trace       CPD-71225
 */
Std_ReturnType
    Csm_ChkJobExCalcSec(uint32 jobId, const uint8* partnerPublicValuePtr, uint32 partnerPublicValueLength, uint8 sid);

/**
 * @brief Checks the parameters for the Csm_CancelJob function.
 *
 * This function validates the provided parameters to ensure they are correct and suitable for the Csm_CancelJob
 * function.
 *
 * @param[in] jobId: Job identifier.
 * @param[in] sid: Service ID.
 *
 * @return The function returns E_OK if the parameters are valid, otherwise it returns E_NOT_OK.
 *
 * @reentrant      Reentrant but not for the same timer instance
 * @synchronous    TRUE
 * @trace       CPD-71226
 */
Std_ReturnType Csm_ChkCancelJob(uint32 jobId, uint8 sid);

/**
 * @brief Checks the parameters for the Csm_CbkNotify function.
 *
 * This function validates the provided parameters to ensure they are correct and suitable for the Csm_CbkNotify
 * function.
 *
 * @param[in] job: Pointer to the job structure.
 * @param[in] sid: Service ID.
 *
 * @return The function returns E_OK if the parameters are valid, otherwise it returns E_NOT_OK.
 *
 * @reentrant      Reentrant but not for the same timer instance
 * @synchronous    TRUE
 * @trace       CPD-71227
 */
Std_ReturnType Csm_ChkCbkNotify(const Crypto_JobType* job, uint8 sid);
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/

#ifdef __cplusplus
}
#endif

#endif /* CSM_INTERNAL_H */
