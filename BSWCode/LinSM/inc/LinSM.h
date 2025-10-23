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
 **  @file               : LinSM.h
 **  @author             : HuRongbo
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : API declaration and type definitions of LinSM
 **
 ***********************************************************************************************************************/
/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author       Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V01.00.00 2017-12-01  rongbo.hu     Initial version
 *  V02.00.00 2020-08-14  rongbo.hu     Update for R19-11
 *  V02.00.01 2021-08-12  rongbo.hu     Bug fix
 *  V02.00.02 2022-07-11  rongbo.hu     QAC check issue fix
 *  V02.00.03 2022-09-21  rongbo.hu     Add check if module initialized in LinSM_MainFunction()
 *  V02.00.04 2022-11-14  rongbo.hu     1. Fix array access out of bounds issues in LinSM_RequestComMode() (JIRA:
 PRD0032020-233)
 *                                      2. Change private variable keyword from static to macro LINIF_LOCAL
 *  V02.00.05 2023-03-14  rongbo.hu     Resolves an exception that is raised when LinSMConfirmationTimeout is set to
 0(JIRA:PRD0042022-590)
 *  V02.00.06 2023-03-14  rongbo.hu     LinSM_CheckSchedule,Compile warning clean.(CPT-7666)
 *  V02.00.07 2024-02-29  shengnan.sun  1. QAC_PH_2024 check.
 *                                      2. LinSM is UNINIT, NetworkHandle is NULL_PTR.
 *  V02.00.08 2024-03-09  shengnan.sun  Support multiple partition feature.
 *  V02.00.09 2024-07-10  caihong.liu   Remove the LINSM_GOTOSLEEP_TIMER requirement
 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:LinSM<br>
  RuleSorce:puhua_rules-2024.12.rcf

    \li VL_LinSM_DerefNullPtr .<br>
    Reason: Allowing the module internal to skip checking directly dereferencing Pointers.
    Risk: No risk.
    Prevention: None.

    \li VL_MTR_LinSM_STCAL
      Reason: The software structure of LinSM is defined according to the AUTOSAR standard,resulting in its metric
  being above the threshold
      Risk: Understandability and testability become overly complex
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_LinSM_3408
      Reason:Configuration parameter, design needs.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.
 */

#ifndef LINSM_H_
#define LINSM_H_

/* =================================================== inclusions =================================================== */
/** SWS_LinSM_00016 */
#include "ComStack_Types.h"
#include "LinSM_PBcfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */
/** Published information elements */
#define LINSM_VENDOR_ID                   (62u)
#define LINSM_MODULE_ID                   (141u)
#define UDPNM_AR_RELEASE_MAJOR_VERSION    (4u)
#define UDPNM_AR_RELEASE_MINOR_VERSION    (9u)
#define UDPNM_AR_RELEASE_REVISION_VERSION (0u)
#define LINSM_SW_MAJOR_VERSION            (2u)
#define LINSM_SW_MINOR_VERSION            (1u)
#define LINSM_SW_PATCH_VERSION            (0u)
/* ===================================================== macros ===================================================== */
/** instance id */
#define LINSM_INSTANCE_ID (uint8)0x00u

/** Development errors used by the LinSM module */
#define LINSM_E_UNINIT              (uint8)0x00u
#define LINSM_E_NONEXISTENT_NETWORK (uint8)0x20u
#define LINSM_E_PARAMETER           (uint8)0x30u
#define LINSM_E_PARAM_POINTER       (uint8)0x40u
#define LINSM_E_INIT_FAILED         (uint8)0x50u
#define LINSM_E_PARTITION           (uint8)0x60u
#define LINSM_E_PARTITION_UNINIT    (uint8)0x70u
#define LINSM_E_INVALID_ID          (uint8)0xFFu

/** service id */
#define LINSM_SERVICE_ID_INIT                         (uint8)0x01u
#define LINSM_SERVICE_ID_GET_VERSIONINFO              (uint8)0x02u
#define LINSM_SERVICE_ID_SCHEDULE_REQUEST             (uint8)0x10u
#define LINSM_SERVICE_ID_GET_CURRENTCOMMODE           (uint8)0x11u
#define LINSM_SERVICE_ID_REQUEST_COMMODE              (uint8)0x12u
#define LINSM_SERVICE_ID_SCHEDULEREQUEST_CONFIRMATION (uint8)0x20u
#define LINSM_SERVICE_ID_GOTOSLEEP_INDICATION         (uint8)0x03u
#define LINSM_SERVICE_ID_WAKEUP_CONFIRMATION          (uint8)0x21u
#define LINSM_SERVICE_ID_GOTOSLEEP_CONFIRMATION       (uint8)0x22u
#define LINSM_SERVICE_ID_MAIN_FUNCTION                (uint8)0x30u

/** Runtime error */
#define LINSM_E_CONFIRMATION_TIMEOUT (uint8)0x00u

/** SWS_LinSM_00220 */
/** Type used to report the current mode to the BswM */
typedef uint8 LinSM_ModeType;
#define LINSM_FULL_COM (LinSM_ModeType)0x01u
#define LINSM_NO_COM   (LinSM_ModeType)0x02u
/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief       This function initializes the LinSM.
 * @param[in]   ConfigPtr   : Pointer to the LinSM post-build configuration data.
 * @reentrant   TRUE(Reentrant for different partitions, Non Reentrant for the same partiton.)
 * @synchronous TRUE
 * @trace       CPD-56716
 */
void LinSM_Init(const LinSM_ConfigType* ConfigPtr);

#if LINSM_MASTER_NODE_SUPPORT == STD_ON
/**
 * @brief       The upper layer requests a schedule table to be changed on one LIN network
 * @param[in]   network   : Identification of the LIN channel.
 * @param[in]   schedule  : Index of the scheduled table.
 * @return      Std_ReturnType
 * @retval      E_OK      : Schedule table request has been accepted.
 * @retval      E_NOT_OK  : Schedule table switch request has not been accepted due to one of the following reasons:
 *                          LinSM has not been initialized,
 *                          Referenced channel does not exist (identification is out of range),
 *                          Referenced schedule table does not exist (identification is out of range),
 *                          Sub-state is not LINSM_FULL_COM.
 * @reentrant   TRUE
 * @synchronous FALSE
 * @trace       CPD-56717
 */
Std_ReturnType LinSM_ScheduleRequest(NetworkHandleType network, LinIf_SchHandleType schedule);
#endif

/**
 * @brief       Function to get the version information of LinSM.
 * @param[in]   versioninfo  : Pointer to where to store the version information of this module.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-56718
 */
#if LINSM_VERSION_INFO_API == STD_ON
void LinSM_GetVersionInfo(Std_VersionInfoType* versioninfo);
#endif

/**
 * @brief       Function to query the current communication mode.
 * @param[in]   network   : Identification of the LIN channel.
 * @param[in]   mode      : Returns the active mode, see ComM_ModeType for descriptions of the modes.
 * @return      Std_ReturnType
 * @retval      E_OK      : OK
 * @retval      E_NOT_OK  : Not possible to perform the request, e.g. not initialized.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-56719
 */
Std_ReturnType LinSM_GetCurrentComMode(NetworkHandleType network, ComM_ModeType* mode);

/**
 * @brief       Requesting of a communication mode.
 *              The mode switch will not be made instant. The LinSM will notify the caller when mode transition is made.
 * @param[in]   network   : Identification of the LIN channel
 * @param[in]   mode      : Request mode
 * @return      Std_ReturnType
 * @retval      E_OK    : Request accepted
 * @retval      E_NOT_OK: Not possible to perform the request, e.g. not initialized.
 * @reentrant   TRUE
 * @synchronous FALSE
 * @trace       CPD-56721
 */
Std_ReturnType LinSM_RequestComMode(NetworkHandleType network, ComM_ModeType mode);

#if LINSM_MASTER_NODE_SUPPORT == STD_ON
/**
 * @brief       The LinIf module will call this callback when the new requested schedule table is active.
 * @param[in]   network   : Identification of the LIN channel.
 * @param[in]   schedule  : Index of the scheduled table.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-56723
 */
void LinSM_ScheduleRequestConfirmation(NetworkHandleType network, LinIf_SchHandleType schedule);
#endif

#if LINSM_SLAVE_NODE_SUPPORT == STD_ON
/**
 * @brief       The LinIf will call this callback when the go to sleep command is received
 *              on the network or a bus idle timeout occurs.
 * @param[in]   network   : Identification of the LIN channel
 * @reentrant   TRUE (Reentrant for different Channels)
 * @synchronous TRUE
 * @trace       CPD-56724
 */
void LinSM_GotoSleepIndication(NetworkHandleType network);
#endif

/**
 * @brief       The LinIf will call this callback when the go to sleep command
 *              is sent successfully or not sent successfully on the network.
 * @param[in]   network   : Identification of the LIN channel
 * @param[in]   success   : True if goto sleep was successfully sent,
 *                          false otherwise
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-56726
 */
void LinSM_GotoSleepConfirmation(NetworkHandleType network, boolean success);

/**
 * @brief       The LinIf will call this callback when the wake up signal command is sent not successfully/successfully
 * on the network.
 * @param[in]   network   : Identification of the LIN channel
 * @param[in]   success   : True if wakeup was successfully sent,
 *                          false otherwise
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-56727
 */
void LinSM_WakeupConfirmation(NetworkHandleType network, boolean success);

/**
 * @brief       Periodic function that runs the timers of different request timeouts.
 * @param[in]   chlIndex  : Index of the LinSM channel
 * @reentrant   TRUE (Reentrant for different Channels)
 * @synchronous TRUE
 * @trace       CPD-56722
 */
void LinSM_MainFunction(LinSM_ChannelIndexType chlIndex);

#ifdef __cplusplus
}
#endif
#endif
