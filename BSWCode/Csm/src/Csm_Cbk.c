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
 ***********************************************************************************************************************
 **
 **  @file               : Csm_Cbk.c
 **  @author             : jie.gu
 **  @date               : 2025/03/31
 **  @vendor             : isoft
 **  @description        : Csm callback Interface Implementation source
 **
 **********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Csm_Cbk.h"
#include "Csm_Internal.h"

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
/*************************************************************************/
/**
 * @brief the CSM that a job has finished. This function is used by the
 * underlying layer (CRYIF).
 * Variation: {ecuc(Csm/CsmJob/CsmJobUsePort == false)} &&
 * {ecuc(Csm/CsmJobs/CsmJob.CsmJobPrimitiveRef- >CsmPrimitives/{Primitive}Config/
 * {Primitive}Processing == CRYPTO_PROCESSING_ASYNC)}
 */
/*************************************************************************/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Csm_CallbackNotification(
    /* PRQA S 3432 ++ */ /* VL_Csm_3432 */
    /* PRQA S 3673 ++ */ /* VL_QAC_3673 */
    Crypto_JobType* job,
    /* PRQA S 3673 -- */
    Crypto_ResultType result)
/* PRQA S 3432 -- */
/* PRQA S 1532 -- */
{
    const Csm_JobsCfgType*   jobCfgPtr;
    uint32                   cbkId;
    Crypto_OperationModeType mode;
#if (CSM_DEV_ERROR_DETECT == STD_ON)
    Std_ReturnType ret;

    ret = Csm_ChkCbkNotify(job, CSM_SID_CBK_NOTIFY);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        jobCfgPtr = &(Csm_JobCfg[job->jobId]);
        cbkId     = jobCfgPtr->jobPrimitiveInfo->callbackId;
        mode      = job->jobPrimitiveInputOutput.mode;
        if (Csm_JobPostponeFlag[job->jobId]) /* PRQA S 2109 */ /* VL_Csm_2109 */
        {
#if (CSM_CBKFUNCS_NUM > 0u)
            /*[SWS_Csm_01087]*/
            if (NULL_PTR != Csm_CbkCfg[cbkId].cbkFnc)
            {
                Csm_CbkCfg[cbkId].cbkFnc(job, CRYPTO_E_JOB_CANCELED);
            }
#else
            (void)(result);
            (void)(cbkId);
#endif /*CSM_CBKFUNCS_NUM > 0u*/
            Csm_JobPostponeFlag[job->jobId] = FALSE;
        }
        else
        {
            /* PRQA S 4522,1823,1317 ++ */ /* VL_Csm_4522,VL_Csm_1823,VL_Csm_1317 */
            if (0u != (mode & CRYPTO_OPERATIONMODE_FINISH))
            /* PRQA S 4522,1823,1317 -- */
            {
#if (CSM_CBKFUNCS_NUM > 0u)
                /*[SWS_Csm_01044]*/
                if (NULL_PTR != Csm_CbkCfg[cbkId].cbkFnc)
                {
                    Csm_CbkCfg[cbkId].cbkFnc(job, result);
                }
#endif /*CSM_CBKFUNCS_NUM > 0u*/
                Csm_JobStates[job->jobId] = CRYPTO_JOBSTATE_IDLE;

                /*[SWS_Csm_91017]*/
                /*TODO*/
            }
        }
    }
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"
