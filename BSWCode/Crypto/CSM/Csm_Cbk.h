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
**  FILENAME    : Csm_Cbk.H                                                   **
**                                                                            **
**  Created on  :                                                             **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION : implementation call-back header for CSM                     **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/******************************************************************************
**                      Revision Control History                             **
******************************************************************************/
/*  <VERSION>      <DATE>        <AUTHOR>        <REVISION LOG>
 *    V1.0.0      2019-10-18    qinchun.yang     Initial version
 *    V1.1.0      2020-07-16    yuzhe.zhang      R19-11
 *    V2.0.0      2023-02-06    jie.gu           R19_11
 */
#ifndef CSM_CBK_H
#define CSM_CBK_H
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Csm.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

/**
 * Notifies the CSM that a job has finished. This function is used by the
 * underlying layer (CRYIF).
 * Variation: {ecuc(Csm/CsmJob/CsmJobUsePort == false)} &&
 * {ecuc(Csm/CsmJobs/CsmJob.CsmJobPrimitiveRef- >CsmPrimitives/{Primitive}Config/
 * {Primitive}Processing == CRYPTO_PROCESSING_ASYNC)}
 * Service ID: 0x70
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):
 *     @job, Holds a pointer to the job, which has finished.
 *     @result, Contains the result of the cryptographic operation
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, CSM_CBK_NOTIFICATION_CODE)
Csm_CallbackNotification(
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_DATA) job,
    /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    VAR(Crypto_ResultType, CSM_APPL_DATA) result);

#endif /* CSM_CBK_H */
