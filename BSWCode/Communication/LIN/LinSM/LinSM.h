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
**  FILENAME    : LinSM.h                                                     **
**                                                                            **
**  Created on  :                                                             **
**  Author      : HuRongbo                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION : API declaration and type definitions of LinSM               **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/* <VERSION>   <DATE>     <AUTHOR>   <REVISION LOG>
 * V1.0.0      20171201   HuRongbo   Initial version
 * V2.0.0      20200814   HuRongbo   Update for R19-11
 * V2.0.1      20210517   HuRongbo   Bug fix
 */

#ifndef LINSM_H
#define LINSM_H

/*@req <SWS_LinSM_00016> */
#include "ComStack_Types.h"
#include "LinSM_Cfg.h"
/*@req <SWS_LinSM_00012> */
#include "LinIf.h"
/*@req <SWS_LinSM_00013> */
#include "ComM.h"

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/*Published information elements*/
#define LINSM_VENDOR_ID                     62
#define LINSM_MODULE_ID                     141
#define LINSM_H_AR_RELEASE_MAJOR_VERSION    4
#define LINSM_H_AR_RELEASE_MINOR_VERSION    5
#define LINSM_H_AR_RELEASE_REVISION_VERSION 0

#define LINSM_H_SW_MAJOR_VERSION            2
#define LINSM_H_SW_MINOR_VERSION            0
#define LINSM_H_SW_PATCH_VERSION            1

/*instance id*/
#define LINSM_INSTANCE_ID (uint8)0x00

/*Development errors used by the LinSM module*/
#define LINSM_E_UNINIT              (uint8)0x00
#define LINSM_E_NONEXISTENT_NETWORK (uint8)0x20
#define LINSM_E_PARAMETER           (uint8)0x30
#define LINSM_E_PARAM_POINTER       (uint8)0x40
#define LINSM_E_INIT_FAILED         (uint8)0x50
#define LINSM_E_PARTITION           (uint8)0x60

/*service id*/
#define LINSM_SERVICE_ID_INIT                         (uint8)0x01
#define LINSM_SERVICE_ID_GET_VERSIONINFO              (uint8)0x02
#define LINSM_SERVICE_ID_SCHEDULE_REQUEST             (uint8)0x10
#define LINSM_SERVICE_ID_GET_CURRENTCOMMODE           (uint8)0x11
#define LINSM_SERVICE_ID_REQUEST_COMMODE              (uint8)0x12
#define LINSM_SERVICE_ID_SCHEDULEREQUEST_CONFIRMATION (uint8)0x20
#define LINSM_SERVICE_ID_GOTOSLEEP_INDICATION         (uint8)0x03
#define LINSM_SERVICE_ID_WAKEUP_CONFIRMATION          (uint8)0x21
#define LINSM_SERVICE_ID_GOTOSLEEP_CONFIRMATION       (uint8)0x22
#define LINSM_SERVICE_ID_MAIN_FUNCTION                (uint8)0x30

/* Runtime error */
#define LINSM_E_CONFIRMATION_TIMEOUT (uint8)0x00

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
/* Type used to report the current mode to the BswM */
/*@req <SWS_LinSM_00220> */
typedef uint8 LinSM_ModeType;
#define LINSM_FULL_COM 0x01u
#define LINSM_NO_COM   0x02u

typedef enum
{
    LINSM_MASTER,
    LINSM_SLAVE
} LinSM_NodeType;

/*@req <ECUC_LinSM_00146> */
typedef struct
{
    /* This index parameter can be used by the BswM as a SymbolicNameReference
     * target.The LinSM just forwards the request from the BswM to LinIf. */
    /*@req <ECUC_LinSM_00001 > */
    uint8 scheduleIndex;

    /* Reference to a schedule table in the LinIf configuration */
    /*@req <ECUC_LinSM_00149> */
    LinIf_SchHandleType scheduleIndexRef;
} LinSM_ScheduleType;

/*@req <ECUC_LinSM_00142> */
typedef struct
{
    /* Timeout in seconds for the goto sleep, wakeup and schedule request calls
     * to LinIf. The timeout must be longer than a goto-sleep command on the
     * bus (i.e. it is bit rate dependent). */
    /*@req <ECUC_LinSM_00144> */
    uint32 confirmationTimeout;

    /* Specifies the LIN node type of this channel. */
    /*@req <ECUC_LinSM_00211> */
    LinSM_NodeType nodeType;

    /* Timeout in ms after a failed wakeup sequence until a new wakeup
     * process is started.*/
    /*@req <ECUC_LinSM_00210> */
    uint32 silenceAfterWakeupTimeout;

    /* Indicate parameter [transceiverPassiveMode] is support or not */
    boolean isTransceiverPassiveModeSupport;

    /* Selects STANDBY (true) or SLEEP (false) transceiver mode when
     * entering LINSM_NO_COM. */
    /*@req <ECUC_LinSM_00202> */
    boolean transceiverPassiveMode;

    /* Unique handle to identify one certain LIN network. Reference to
     * one of the network handles configured in the ComM. */
    /*@req <ECUC_LinSM_00145> */
    NetworkHandleType comMNetworkHandleRef;

    /* references to a schedule that is locatedin the LinIf configuration.*/
    /*@req <ECUC_LinSM_00146> */
    P2CONST(LinSM_ScheduleType, TYPEDEF, LINSM_CONST) schedulePtr;

    /* referenced schedule table number */
    uint8 scheduleNum;
} LinSM_ChannelType;

/*@req <ECUC_LinSM_00207> */
typedef struct
{
    /* Specifies the maximal amount of mode request repetitions without a
     * respective mode indication from the LinIf module until the LinSM module
     * reports a Default Error to the Det and tries to go back to no
     * communication. */
    /*@req <ECUC_LinSM_00208> */
    uint8 modeRequestRepetitionMax;

    /* Describes each LIN channel the LinSM is connected to. */
    /*@req <ECUC_LinSM_00142> */
    P2CONST(LinSM_ChannelType, TYPEDEF, LINSM_CONST) channelPtr;
} LinSM_ConfigType;

/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/
/* pre-compile configuration data */

extern CONST(LinSM_ConfigType, LINSM_CONST) LinSM_PCConfig;

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

/******************************************************************************/
/*
 * Brief               This function initializes the LinSM.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[in/out]  N/A
 * Param-Name[out]     N/A
 * Return              N/A
 */
/******************************************************************************/
extern FUNC(void, LINSM_CODE) LinSM_Init(P2CONST(LinSM_ConfigType, AUTOMATIC, LINSM_CONST_PBCFG) ConfigPtr);

#if (LINSM_MASTER_NODE_SUPPORT == STD_ON)
/******************************************************************************/
/*
 * Brief               The upper layer requests a schedule table to be changed
                       on one LIN network.
 * ServiceId           0x10
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      network:  Identification of the LIN channel
 *                     schedule: Pointer to the new Schedule table
 * Param-Name[in/out]  N/A
 * Param-Name[out]     N/A
 * Return              E_OK - Schedule table request has been accepted.
 *                     E_NOT_OK - Schedule table switch request has not been
 *                     accepted due to one of the following reasons:
 *                     * LinSM has not been initialized referenced channel does
 *                       not exist (identification is out of range)
 *                     * Referenced schedule table does not exist
 *                       (identification is out of range)
 *                     * Sub-state is not LINSM_FULL_COM
 */
/******************************************************************************/
extern FUNC(Std_ReturnType, LINSM_CODE) LinSM_ScheduleRequest(NetworkHandleType network, LinIf_SchHandleType schedule);
#endif /* LINSM_MASTER_NODE_SUPPORT == STD_ON */

#if (STD_ON == LINSM_VERSION_INFO_API)
/******************************************************************************/
/*
 * Brief               Returns the version information of LinSM
 * ServiceId           0x02
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      N/A
 * Param-Name[in/out]  N/A
 * Param-Name[out]     versioninfo: Pointer to where to store the version
 *                                  information of this module.
 * Return              N/A
 */
/******************************************************************************/
#if (STD_ON == LINSM_DEV_ERROR_DETECT)
#define LinSM_GetVersionInfo(VersionInfo)                               \
    do                                                                  \
    {                                                                   \
        if (NULL_PTR == (VersionInfo))                                  \
        {                                                               \
            Det_ReportError(                                            \
                LINSM_MODULE_ID,                                        \
                LINSM_INSTANCE_ID,                                      \
                LINSM_SERVICE_ID_GET_VERSIONINFO,                       \
                LINSM_E_PARAM_POINTE);                                  \
        }                                                               \
        else                                                            \
        {                                                               \
            (VersionInfo)->vendorID = LINSM_VENDOR_ID;                  \
            (VersionInfo)->moduleID = LINSM_MODULE_ID;                  \
            (VersionInfo)->sw_major_version = LINSM_H_SW_MAJOR_VERSION; \
            (VersionInfo)->sw_minor_version = LINSM_H_SW_MINOR_VERSION; \
            (VersionInfo)->sw_patch_version = LINSM_H_SW_PATCH_VERSION; \
        }                                                               \
    } while (0)
#else /* STD_ON == LINSM_DEV_ERROR_DETECT */
#define LinSM_GetVersionInfo(VersionInfo)                           \
    do                                                              \
    {                                                               \
        (VersionInfo)->vendorID = LINSM_VENDOR_ID;                  \
        (VersionInfo)->moduleID = LINSM_MODULE_ID;                  \
        (VersionInfo)->sw_major_version = LINSM_H_SW_MAJOR_VERSION; \
        (VersionInfo)->sw_minor_version = LINSM_H_SW_MINOR_VERSION; \
        (VersionInfo)->sw_patch_version = LINSM_H_SW_PATCH_VERSION; \
    } while (0)
#endif /* STD_OFF == LINSM_DEV_ERROR_DETECT */

#endif /* STD_ON == LINSM_VERSION_INFO_API */
/******************************************************************************/
/*
 * Brief               Function to query the current communication mode.
 * ServiceId           0x11
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      network: Identification of the LIN channel
 * Param-Name[in/out]  N/A
 * Param-Name[out]     mode: Returns the active mode,
 *                           see ComM_ModeType for descriptions of the modes
 * Return              E_OK - Ok
 *                     E_NOT_OK - Not possible to perform the request, e.g. not
 *                                initialized.
 */
/******************************************************************************/
extern FUNC(Std_ReturnType, LINSM_CODE) LinSM_GetCurrentComMode(NetworkHandleType network, ComM_ModeType* mode);

/******************************************************************************/
/*
 * Brief               Requesting of a communication mode.
 *                     The mode switch will not be made instant. The LinSM will
 *                     notify the caller when mode transition is made.
 * ServiceId           0x12
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      network: Identification of the LIN channel
 *                     mode:    Request mode
 * Param-Name[in/out]  N/A
 * Param-Name[out]     N/A
 * Return              E_OK - Request accepted
 *                     E_NOT_OK - Not possible to perform the request, e.g. not
 *                                initialized.
 */
/******************************************************************************/
extern FUNC(Std_ReturnType, LINSM_CODE) LinSM_RequestComMode(NetworkHandleType network, ComM_ModeType mode);

/*************************End of Public API Declarations**********************/

#endif /* LINSM_H */
