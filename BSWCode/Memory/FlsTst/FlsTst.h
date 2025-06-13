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
/*********************************************************************************
**                                                                            **
**  FILENAME    : FlsTst.h                                                    **
**                                                                            **
**  Created on  :                                                             **
**  Author      : peng.wu                                                     **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S):   AUTOSAR classic Platform R19-11                       **
**                                                                            **
*******************************************************************************/

/*======================[R E V I S I O N   H I S T O R Y]=====================*/
/*  <VERSION>    <DATE>      <AUTHOR>         <REVISION LOG>
 *   V1.0.0     20210916     Huangyin         Initial version
 *   V2.0.0     20221221     Peng.Wu          Modified to version R19-11
 *   V2.0.1     20231025     pengwu           CPT-6553, FIx FlsTst_MemCompare function
 *              20231025     pengwu           1.CPT-8641, Fix compile problem when DET is disable
 *                                            2.Fix compile warning information
 *                                            3.CPT-8747, Fix FLSTST_E_INIT_FAILED report,add duplicated address
 *                                            4.Add Qac
 *              2024-5-7     pengwu           1.CPT-8937, Fix code error, add Qac
 */
/*============================================================================*/
#ifndef FLSTST_H
#define FLSTST_H

/*============================[I N C L U D E S]===============================*/
#include "FlsTst_Cfg.h"
#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
#include "Det.h"
#endif /*STD_ON == RAMTST_DEV_ERROR_DETECT*/
/*============================================================================*/

/*===================[V E R S I O N  I N F O R M A T I O N]===================*/
#define FLSTST_VENDOR_ID          62U
#define FLSTST_MODULE_ID          104U
#define FLSTST_H_AR_MAJOR_VERSION 4U
#define FLSTST_H_AR_MINOR_VERSION 5U
#define FLSTST_H_AR_PATCH_VERSION 0U
#define FLSTST_H_SW_MAJOR_VERSION 2U
#define FLSTST_H_SW_MINOR_VERSION 0U
#define FLSTST_H_SW_PATCH_VERSION 1U
#define FLSTST_INSTANCE_ID        0U
/*============================================================================*/

/*==============================[M A C R O S]=================================*/
/*================ ServiceID ==============*/
#define FLSTST_INIT_ID                 0x00U
#define FLSTST_DEINIT_ID               0x01U
#define FLSTST_STARTFGND_ID            0x02U
#define FLSTST_ABORT_ID                0x03U
#define FLSTST_SUSPEND_ID              0x04U
#define FLSTST_RESUME_ID               0x05U
#define FLSTST_GETCURRENTSTATE_ID      0x06U
#define FLSTST_GETTESTRESULTBGND_ID    0x07U
#define FLSTST_GETTESTRESULTFGND_ID    0x0FU
#define FLSTST_GETVERSIONINFO_ID       0x08U
#define FLSTST_GETTESTSIGNATUREBGND_ID 0x09U
#define FLSTST_GETTESTSIGNATUREFGND_ID 0x0AU
#define FLSTST_GETERRORDETAILS_ID      0x0BU
#define FLSTST_TESTECC_ID              0x0CU
#define FLSTST_MAINFUNCTION_ID         0x0DU
#define FLSTST_TESTCOMPLETENOTIFY_ID   0x0EU

/*============== DET Error Number===========*/
#define FLSTST_E_STATE_FAILURE       0x01U
#define FLSTST_E_PARAM_INVALID       0x02U
#define FLSTST_E_UNINIT              0x03U
#define FLSTST_E_ALREADY_INITIALIZED 0x04U
#define FLSTST_E_INIT_FAILED         0x05U
#define FLSTST_E_PARAM_POINTER       0x06U
#define FLSTST_E_TIMEOUT             0x07U

#define FlsTst_GetResource()
#define FlsTst_FreeResource()
/*============================================================================*/

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
static inline void FLSTST_DET_REPORTERROR(uint8 ApiId, uint8 ErrorId)
{
    ((void)Det_ReportError(FLSTST_MODULE_ID, FLSTST_INSTANCE_ID, ApiId, ErrorId));
}
#endif /* STD_ON == FLSTST_DEV_ERROR_DETECT */

/*===============[F U N C T I O N   D E C L A R A T I O N]===============*/
/*************************************************************************/
/*
 * Brief               Service for Flash Test initialization.
 * ServiceId           0x00
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ConfigPtr: Pointer to configuration set.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
extern FUNC(void, FLSTST_CODE) FlsTst_Init(P2CONST(FlsTst_ConfigType, AUTOMATIC, FLSTST_APPL_CONST) ConfigPtr);

/*************************************************************************/
/*
 * Brief               Service for Flash Test De-Initialization.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
extern FUNC(void, FLSTST_CODE) FlsTst_DeInit(void);

/*************************************************************************/
/*
 * Brief               Service for executing foreground Flash Test.
 * ServiceId           0x02
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      FgndBlockId: Number of the foreground test to be
 *                     executed. This is dependent on configuration.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 *                       E_OK: Foreground test processed
 *                       E_NOT_OK: Foreground test not accepte
 * PreCondition        None
 */
/*************************************************************************/
#if ((STD_ON == FLSTST_START_FGND_API) && (0 != FLSTST_FGND_BLOCK_NUM))
extern FUNC(Std_ReturnType, FLSTST_CODE) FlsTst_StartFgnd(FlsTst_BlockIdFgndType FgndBlockId);
#endif /* #if ((STD_ON == FLSTST_START_FGND_API) && (0 != FLSTST_FGND_BLOCK_NUM)) */
/*************************************************************************/
/*
 * Brief               Service for aborting the Flash Test.
 * ServiceId           0x03
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
#if (0U != FLSTST_BGND_BLOCK_NUM)
extern FUNC(void, FLSTST_CODE) FlsTst_Abort(void);
#endif /* #if (0U != FLSTST_BGND_BLOCK_NUM) */
/*************************************************************************/
/*
 * Brief               Service for suspending current operation of the
 *                     Flash Test, until FlsTst_Resume is called.
 * ServiceId           0x04
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
#if ((STD_ON == FLSTST_SUSPEND_RESUME_API) && (0U != FLSTST_BGND_BLOCK_NUM))
extern FUNC(void, FLSTST_CODE) FlsTst_Suspend(void);
#endif /* #if ((STD_ON == FLSTST_SUSPEND_RESUME_API) && (0U != FLSTST_BGND_BLOCK_NUM)) */

/*************************************************************************/
/*
 * Brief               Service for continuing the Flash Test at the point
 *                     it was suspended.
 * ServiceId           0x05
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
#if ((STD_ON == FLSTST_SUSPEND_RESUME_API) && (0U != FLSTST_BGND_BLOCK_NUM))
extern FUNC(void, FLSTST_CODE) FlsTst_Resume(void);
#endif /* #if ((STD_ON == FLSTST_SUSPEND_RESUME_API) && (0U != FLSTST_BGND_BLOCK_NUM)) */

/*************************************************************************/
/*
 * Brief               Service for continuing the Flash Test at the point
 *                     it was suspended.
 * ServiceId           0x06
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              FlsTst_StateType
 *                  FLSTST_UNINIT:
 *                     The Flash Test is not initialized or not usable.
 *                  FLSTST_INIT:
 *                     The Flash Test is initialized and ready to be started.
 *                  FLSTST_RUNNING:
 *                     The Flash Test is currently running.
 *                  FLSTST_ABORTED:
 *                     The Flash Test is aborted.
 *                  FLSTST_SUSPENDED:
 *                     The Flash Test is waiting to be resumed or is waiting
 *                     to start forground mode test.
 * PreCondition        None
 */
/*************************************************************************/
#if (STD_ON == FLSTST_GET_CURRENT_STATE_API)
extern FUNC(FlsTst_StateType, FLSTST_CODE) FlsTst_GetCurrentState(void);
#endif /* #if (STD_ON == FLSTST_GET_CURRENT_STATE_API) */

/*************************************************************************/
/*
 * Brief               Service returns the Background Flash Test result.
 * ServiceId           0x07
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              FlsTst_TestResultBgndType
 * PreCondition        None
 */
/*************************************************************************/
#if ((STD_ON == FLSTST_GET_TEST_RESULT_BGND_API) && (0 != FLSTST_BGND_BLOCK_NUM))
extern FUNC(FlsTst_TestResultBgndType, FLSTST_CODE) FlsTst_GetTestResultBgnd(void);
#endif /* #if ((STD_ON == FLSTST_GET_TEST_RESULT_BGND_API) && (0 != FLSTST_BGND_BLOCK_NUM)) */

/*************************************************************************/
/*
 * Brief               Service returns the Foreground Flash Test result.
 * ServiceId           0x0f
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              FlsTst_TestResultFgndType
 * PreCondition        None
 */
/*************************************************************************/
#if ((STD_ON == FLSTST_GET_TEST_RESULT_FGND_API) && (0 != FLSTST_FGND_BLOCK_NUM))
extern FUNC(FlsTst_TestResultFgndType, FLSTST_CODE) FlsTst_GetTestResultFgnd(void);
#endif /* #if ((STD_ON == FLSTST_GET_TEST_RESULT_FGND_API) && (0 != FLSTST_FGND_BLOCK_NUM)) */

/*************************************************************************/
/*
 * Brief               Service returns the version information of this module.
 * ServiceId           0x08
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     versioninfo:Pointer to where to store the version
 *                     information of this module.
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
#if (STD_ON == FLSTST_VERSION_INFO_API)
extern FUNC(void, FLSTST_CODE) FlsTst_GetVersionInfo(Std_VersionInfoType* versionInfo);
#endif /* #if (STD_ON == FLSTST_VERSION_INFO_API) */

/*************************************************************************/
/*
 * Brief               Service returns the Flash Test result in background
 *                     mode.
 * ServiceId           0x09
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              FlsTst_TestSignatureBgndType
 * PreCondition        None
 */
/*************************************************************************/
#if ((STD_ON == FLSTST_GET_TEST_SIGNATURE_BGND_API) && (0 != FLSTST_BGND_BLOCK_NUM))
extern FUNC(FlsTst_TestSignatureBgndType, FLSTST_CODE) FlsTst_GetTestSignatureBgnd(void);
#endif /* #if ((STD_ON == FLSTST_GET_TEST_SIGNATURE_BGND_API) && (0 != FLSTST_BGND_BLOCK_NUM )) */

/*************************************************************************/
/*
 * Brief               Service returns the Flash Test result in background
 *                     mode.
 * ServiceId           0x0a
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              FlsTst_TestSignatureFgndType
 * PreCondition        None
 */
/*************************************************************************/
#if ((STD_ON == FLSTST_GET_TEST_SIGNATURE_FGND_API) && (0 != FLSTST_FGND_BLOCK_NUM))
extern FUNC(FlsTst_TestSignatureFgndType, FLSTST_CODE) FlsTst_GetTestSignatureFgnd(void);
#endif /* #if ((STD_ON == FLSTST_GET_TEST_SIGNATURE_FGND_API) && (0 != FLSTST_FGND_BLOCK_NUM)) */

/*************************************************************************/
/*
 * Brief               Service returns error detais monitored from the
 *                     Flash module.
 * ServiceId           0x0b
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              FlsTst_ErrorDetailsType
 * PreCondition        None
 */
/*************************************************************************/
#if ((STD_ON == FLSTST_GET_ERRORD_ETAILS_API) && ((0U != FLSTST_BGND_BLOCK_NUM) || (0U != FLSTST_FGND_BLOCK_NUM)))
extern FUNC(FlsTst_ErrorDetailsType, FLSTST_CODE) FlsTst_GetErrorDetails(void);
#endif /* #if ((STD_ON == FLSTST_GET_ERRORD_ETAILS_API) && ((0U != FLSTST_BGND_BLOCK_NUM) || (0U != \
          FLSTST_FGND_BLOCK_NUM))) */

/*************************************************************************/
/*
 * Brief               Service executes a test of ECC hardware. This is
 *                     only applicable in case the hardware provices
 *                     such functionality.
 * ServiceId           0x0c
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * PreCondition        None
 */
/*************************************************************************/
#if ((STD_ON == FLSTST_TEST_ECC_API) && ((0U != FLSTST_BGND_BLOCK_NUM) || (0U != FLSTST_FGND_BLOCK_NUM)))
extern FUNC(Std_ReturnType, FLSTST_CODE) FlsTst_TestEcc(void);
#endif /* #if ((STD_ON == FLSTST_TEST_ECC_API) && ((0U != FLSTST_BGND_BLOCK_NUM) || (0U != FLSTST_FGND_BLOCK_NUM))) */

/*************************************************************************/
/*
 * Brief               Service for executing the Flash Test in background
 *                     mode.
 * ServiceId           0x0d
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
#if (0U != FLSTST_BGND_BLOCK_NUM)
extern FUNC(void, FLSTST_CODE) FlsTst_MainFunction(void);
#endif /* #if (0U != FLSTST_BGND_BLOCK_NUM) */

#endif /* #ifndef FLSTST_H */
