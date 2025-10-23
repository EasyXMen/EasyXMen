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
 **  @file               : ComM.h
 **  @author             : xiaojian.liang
 **  @date               : 2024/08/21
 **  @vendor             : isoft
 **  @description        : ComM Interface Public Implementation header
 **
 **********************************************************************************************************************/

/** ===================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date       Author         Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V2.2.1    2024-08-21 xiaojian.liang
 *      1. Support multi-partition distribution and post-build variant
 *
 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C: 2012 Compliance Exceptions
    ModeName:ComM<br>
  RuleSorce:puhua_rules-2024.12.rcf

    \li VL_ComM_FalsePositive
      Reason: QAC false positives
      Risk: No risk
      Prevention: No action required.
    \li VL_ComM_DevelopmentError
      Reason: API parameter checking is enabled if ComMDevErrorDetect is TRUE.
      Risk: No risk
      Prevention: No action required.
    \li VL_ComM_UseStructTagPtr
      Reason: It is permissible to define or declare a pointer to an incomplete struct or union type.
      Risk: No risk
      Prevention: No action required.

    \li VL_MTR_ComM_STM19
      Reason: Multiple exit points can contribute to improved code performance in specific scenarios.
      Risk: Increases the difficulty of debugging the function.
      Prevention: Adequate testing ensures correct functionality.

    \li VL_MTR_ComM_STCAL
      Reason: The software structure of ComM is defined according to the AUTOSAR standard,resulting in its metric
  being above the threshold.
      Risk: Understandability and testability become overly complex.
      Prevention: Design and code review, and have a clear structure and annotated code.
 */

#ifndef COMM_H_
#define COMM_H_

/* =================================================== inclusions =================================================== */
#include "ComM_Cfg.h"
#include "ComStack_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */
/* Published Information */
#define COMM_VENDOR_ID                   (62u)
#define COMM_MODULE_ID                   (12u)
#define COMM_INSTANCE_ID                 (0u)
#define COMM_AR_RELEASE_MAJOR_VERSION    (4u)
#define COMM_AR_RELEASE_MINOR_VERSION    (9u)
#define COMM_AR_RELEASE_REVISION_VERSION (0u)
#define COMM_SW_MAJOR_VERSION            (2u)
#define COMM_SW_MINOR_VERSION            (2u)
#define COMM_SW_PATCH_VERSION            (0u)

/* ===================================================== macros ===================================================== */
/* Development Errors */
#define COMM_E_UNINIT                    0x01u
#define COMM_E_WRONG_PARAMETERS          0x02u
#define COMM_E_PARAM_POINTER             0x03u
#define COMM_E_INVALID_PARTITION_CONTEXT 0x05u
#define COMM_E_ALREADY_INITIALIZED       0x06u

/* Service ID */
#define COMM_SID_INIT                      0x01u
#define COMM_SID_DEINIT                    0x02u
#define COMM_SID_GETSTATUS                 0x03u
#define COMM_SID_GETINHIBITIONSTATUS       0x04u
#define COMM_SID_REQUESTCOMMODE            0x05u
#define COMM_SID_GETMAXCOMMODE             0x06u
#define COMM_SID_GETREQUESTEDCOMMODE       0x07u
#define COMM_SID_GETCURRENTCOMMODE         0x08u
#define COMM_SID_PREVENTWAKEUP             0x09u
#define COMM_SID_LIMITCHANNELTONOCOMMODE   0x0bu
#define COMM_SID_LIMITECUTONOCOMMODE       0x0cu
#define COMM_SID_READINHIBITCOUNTER        0x0du
#define COMM_SID_RESETINHIBITCOUNTER       0x0eu
#define COMM_SID_SETECUGROUPCLASSIFICATION 0x0fu
#define COMM_SID_GETVERSIONINFO            0x10u
#define COMM_SID_NM_NETWORKSTARTINDICATION 0x15u
#define COMM_SID_NM_NETWORKMODE            0x18u
#define COMM_SID_NM_PREPAREBUSSLEEPMODE    0x19u
#define COMM_SID_NM_BUSSLEEPMODE           0x1au
#define COMM_SID_NM_RESTARTINDICATION      0x1bu
#define COMM_SID_DCM_ACTIVEDIAGNOSTIC      0x1fu
#define COMM_SID_DCM_INACTIVEDIAGNOSTIC    0x20u
#define COMM_SID_COMMUNICATIONALLOWED      0x35u
#define COMM_SID_ECUM_WAKEUPINDICATION     0x2au
#define COMM_SID_ECUM_PNCWAKEUPINDICATION  0x37u
#define COMM_SID_BUSSM_MODEINDICATION      0x33u
#define COMM_SID_MAINFUNCATION             0x60u
#define COMM_SID_GETCURRENTPNCCOMMODE      0x6au
#define COMM_SID_NMUPDATEEIRA              0x6cu
#define COMM_SID_NMUPDATEERA               0x6du
#define COMM_SID_GETCHANNELAPPLICATIONID   0x70u

/* Std_ReturnType-extension */
#define COMM_E_MODE_LIMITATION       2u
#define COMM_E_MULTIPLE_PNC_ASSIGNED 3u
#define COMM_E_NO_PNC_ASSIGNED       4u

/* ================================================ type definitions ================================================ */
typedef enum
{
    COMM_UNINIT,
    COMM_INIT,
} ComM_InitStatusType;

typedef enum
{
    COMM_NO_COM_NO_PENDING_REQUEST,
    COMM_NO_COM_REQUEST_PENDING,
    COMM_FULL_COM_NETWORK_REQUESTED,
    COMM_FULL_COM_READY_SLEEP,
    COMM_SILENT_COM,
} ComM_StateType;

typedef enum
{
    COMM_PNC_REQUESTED,
    COMM_PNC_READY_SLEEP,
    COMM_PNC_PREPARE_SLEEP,
    COMM_PNC_NO_COMMUNICATION,
    COMM_PNC_REQUESTED_WITH_WAKEUP_REQUEST,
} ComM_PncModeType;

#if (COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
typedef struct
{
    ComM_InhibitionStatusType EcuGroupClassification;
    uint16                    InhibitCounter;
    ComM_InhibitionStatusType InhibitionStatus[COMM_CHANNEL_NUMBER];
} ComM_InhibitionType;
#endif

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */
#if (COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
extern const ComM_InhibitionType ComM_InhibitionDefault;
extern ComM_InhibitionType       ComM_InhibitionData;
#endif

/* ========================================= external function declarations ========================================= */
/**
 * @brief Initializes the AUTOSAR Communication Manager and restarts the internal state machines.
 * @param[in]    ConfigPtr  Pointer to post-build configuration data
 * @reentrant    TRUE (Reentrant for different partitions. Non reentrant for the same partition.)
 * @synchronous  TRUE
 * @trace        CPD-59608
 */
void ComM_Init(const ComM_ConfigType* ConfigPtr);

/**
 * @brief This API de-initializes the AUTOSAR Communication Manager.
 * @reentrant    TRUE (Reentrant for different partitions. Non reentrant for the same partition.)
 * @synchronous  TRUE
 * @trace        CPD-59609
 */
void ComM_DeInit(void);

/* clang-format off */
/**
 * @brief Returns the initialization status of the AUTOSAR Communication Manager. After a call to ComM_DeInit() ComM
 * should have status COMM_UNINIT, and a new call to ComM_Init needed to make sure ComM restart internal state machines
 * to defailt values.
 * @param[out]   Status   COMM_UNINIT: The ComM is not initialized or not usable. Default value after startup or after ComM_DeInit() is called.
 *                        COMM_INIT: The ComM is initialized and usable.
 * @retval       E_OK     Successfully return of initialization status
 * @retval       E_NOT_OK Return of initialization status failed
 * @reentrant    TRUE (Reentrant for different partitions. Non reentrant for the same partition.)
 * @synchronous  TRUE
 * @trace        CPD-59610
 */
/* clang-format on */
Std_ReturnType ComM_GetStatus(ComM_InitStatusType* Status);

#if COMM_VERSION_INFO_API == STD_ON
/**
 * @brief This function returns the version information of this module.
 * @param[out]   Versioninfo  See Std_VersionInfoType
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-59614
 */
void ComM_GetVersionInfo(Std_VersionInfoType* Versioninfo);
#endif

/**
 * @brief Requesting of a Communication Mode by a user.
 * @param[in]    User     Handle of the user who requests a mode
 * @param[in]    ComMode  COMM_FULL_COMMUNICATION COMM_NO_COMMUNICATION
 * @retval       E_OK                    Successfully changed to the new mode
 * @retval       E_NOT_OK                Changing to the new mode failed
 * @retval       COMM_E_MODE_LIMITATION  Mode can not be granted because of mode inhibition.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-59616
 */
Std_ReturnType ComM_RequestComMode(ComM_UserHandleType User, ComM_ModeType ComMode);

/**
 * @brief Function to query the maximum allowed Communication Mode of the corresponding user.
 * @param[in]    User      Handle of the user who requests a mode
 * @param[out]   ComMode   See ComM_ModeType
 * @retval       E_OK      Successfully returned maximum allowed Communication Mode
 * @retval       E_NOT_OK  Return of maximum allowed Communication Mode failed
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-59618
 */
Std_ReturnType ComM_GetMaxComMode(ComM_UserHandleType User, ComM_ModeType* ComMode);

/**
 * @brief Function to query the currently requested Communication Mode of the corresponding user.
 * @param[in]    User      Handle of the user who requests a mode
 * @param[out]   ComMode   Name of the requested mode
 * @retval       E_OK      Successfully returned requested Communication Mode
 * @retval       E_NOT_OK  Return of requested Communication Mode failed
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-59619
 */
Std_ReturnType ComM_GetRequestedComMode(ComM_UserHandleType User, ComM_ModeType* ComMode);

/**
 * @brief Function to query the current Communication Mode. ComM shall use the corresponding interfaces of the Bus State
 * Managers to get the current Communication Mode of the network. (Call to Bus State Manager API:
 * \<Bus>SM_GetCurrentComMode(...))
 * @param[in]    User      Handle of the user who requests a mode
 * @param[out]   ComMode   See ComM_ModeType
 * @retval       E_OK      Successfully returned Communication Mode from Bus State Manager
 * @retval       E_NOT_OK  Return of Communication Mode from Bus State Manager failed
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-59621
 */
Std_ReturnType ComM_GetCurrentComMode(ComM_UserHandleType User, ComM_ModeType* ComMode);

#if COMM_PNC_SUPPORT == STD_ON
/**
 * @brief The function returns the current Communication Mode of the corresponding PNC the affected user is assigned to.
 * @param[in]    User      Handle of the user who requests a mode
 * @param[out]   ComMode   See ComM_ModeType
 * @retval       E_OK      Successfully returned the state of the PNC referenced by the given ComMUser
 * @retval       E_NOT_OK  Return of the PNC state referenced by the given ComMUser failed
 * @retval       COMM_E_MULTIPLE_PNC_ASSIGNED  Function could not provide the current mode of the PNC, since multiple
 *                                             PNCs are assigned to the affected user
 * @retval       COMM_E_NO_PNC_ASSIGNED        Function could not provide the current mode of the PNC, since no PNC is
 *                                             assigned to the affected user
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-59622
 */
Std_ReturnType ComM_GetCurrentPNCComMode(ComM_UserHandleType User, ComM_ModeType* ComMode);
#endif

#if COMM_WAKEUP_INHIBITION_ENABLED == STD_ON
/**
 * @brief Changes the inhibition status COMM_NO_WAKEUP for the corresponding channel.
 * @param[in]    Channel   See NetworkHandleType
 * @param[in]    Status    FALSE: Wake up inhibition is switched off TRUE: Wake up inhibition is switched on
 * @retval       E_OK      Successfully changed wake up status for the channel
 * @retval       E_NOT_OK  Change of wake up status for the channel failed, e.g. ComMEcuGroupClassification disables the
 *                         functionality (see ECUC_ComM_00563)
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-59624
 */
Std_ReturnType ComM_PreventWakeUp(NetworkHandleType Channel, boolean Status);
#endif

#if COMM_MODE_LIMITATION_ENABLED == STD_ON
/**
 * @brief Changes the inhibition status for the channel for changing from COMM_NO_COMMUNICATION to a higher
 * Communication Mode. (See also ComM_LimitECUToNoComMode, same functionality but for all channels)
 * @param[in]    Channel   See NetworkHandleType
 * @param[in]    Status    FALSE: Limit channel to COMM_NO_COMMUNICATION disabled.
                           TRUE: Limit channel to COMM_NO_COMMUNICATION
 * @retval       E_OK      Successfully changed inhibition status for the channel
 * @retval       E_NOT_OK  Change of inhibition status for the channel failed, e.g. ComMEcuGroupClassification disables
 *                         the functionality (see ECUC_ComM_00563)
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-59625
 */
Std_ReturnType ComM_LimitChannelToNoComMode(NetworkHandleType Channel, boolean Status);

/**
 * @brief Changes the inhibition status for the ECU (=all channels) for changing from COMM_NO_COMMUNICATION to a higher
 * Communication Mode. (See also ComM_LimitChannelToNo ComMode, same functionality but for a specific channels)
 * @param[in]    Status    FALSE: Limit ECU to COMM_NO_COMMUNICATION disabled
                           TRUE: Limit ECU to COMM_NO_COMMUNICATION enabled
 * @retval       E_OK      Successfully changed inhibition status for the ECU
 * @retval       E_NOT_OK  Change of inhibition status for the ECU failed, e.g. ComMEcuGroupClassification disables the
 *                         functionality (see ECUC_ComM_00563)
 * @reentrant    FALSE
 * @synchronous  TRUE
 * @trace        CPD-59627
 */
Std_ReturnType ComM_LimitECUToNoComMode(boolean Status);
#endif

#if (COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
/**
 * @brief Returns the inhibition status of a ComM channel.
 * @param[in]     Channel   See NetworkHandleType
 * @param[out]    Status    See ComM_InhibitionStatusType
 * @retval        E_OK      Successfully returned Inhibition Status
 * @retval        E_NOT_OK  Return of Inhibition Status failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-59628
 */
Std_ReturnType ComM_GetInhibitionStatus(NetworkHandleType Channel, ComM_InhibitionStatusType* Status);

/**
 * @brief This function returns the amount of rejected COMM_FULL_COMMUNICATION user requests.
 * @param[out]   CounterValue  Amount of rejected COMM_FULL_COMMUNICATION user requests
 * @retval       E_OK          Successfully returned Inhibition Counter
 * @retval       E_NOT_OK      Return of Inhibition Counter failed
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-59629
 */
Std_ReturnType ComM_ReadInhibitCounter(uint16* CounterValue);

/**
 * @brief This function resets the Inhibited COMM_FULL_COMMUNICATION request Counter.
 * @retval       E_OK       Successfully reset of Inhibit COMM_FULL_COMMUNICATION Counter
 * @retval       E_NOT_OK   Reset of Inhibit COMM_FULL_COMMUNICATION Counter failed
 * @reentrant    FALSE
 * @synchronous  TRUE
 * @trace        CPD-59630
 */
Std_ReturnType ComM_ResetInhibitCounter(void);

/**
 * @brief Changes the ECU Group Classification status.
 * @param[in]     Status    See ComM_InhibitionStatusType
 * @retval        E_OK      Successfully change the ECU Group Classification Status
 * @retval        E_NOT_OK  Change of the ECU Group Classification Status failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-59631
 */
Std_ReturnType ComM_SetECUGroupClassification(ComM_InhibitionStatusType Status);
#endif

/**
 * @brief Indication of the actual bus mode by the corresponding Bus State Manager. ComM shall propagate the indicated
 * state to the users with means of the RTE and BswM.
 * @param[in]    Channel  See NetworkHandleType
 * @param[in]    ComMode  See ComM_ModeType
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-59645
 */
void ComM_BusSM_ModeIndication(NetworkHandleType Channel, ComM_ModeType ComMode);

#if COMM_MULTIPLE_PARTITION_ENABLED == STD_ON
#include "Os.h"
/**
 * @brief Get Channel Application ID
 * @deprecated Will be remove.
 * @param[in]    Channel  See NetworkHandleType
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @return ApplicationType
 */
ApplicationType ComM_GetChannelApplicationID(NetworkHandleType Channel);
#endif

#ifdef __cplusplus
}
#endif

#endif
