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
**  FILENAME    : CanSM.h                                                     **
**                                                                            **
**  Created on  :                                                             **
**  Author      : Wanglili                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

#ifndef CANSM_H
#define CANSM_H
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "ComM_Types.h"
#include "CanSM_Cfg.h"
#include "CanSM_BswM.h"

#if (STD_ON == CANSM_DEM_SUPPORT)
#include "Dem.h"
#endif
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define CANSM_MODULE_ID                  140U
#define CANSM_VENDOR_ID                  62U
#define CANSM_H_AR_RELEASE_MAJOR_VERSION 4u
#define CANSM_H_AR_RELEASE_MINOR_VERSION 5u
#define CANSM_H_AR_RELEASE_PATCH_VERSION 0u

#define CANSM_INSTANCE_ID                (0U)

/* error code from specification */
#define CANSM_E_NO_ERROR                  (0x00u)
#define CANSM_E_UNINIT                    (0x01u)
#define CANSM_E_PARAM_POINTER             (0x02u)
#define CANSM_E_INVALID_NETWORK_HANDLE    (0x03u)
#define CANSM_E_PARAM_CONTROLLER          (0x04u)
#define CANSM_E_PARAM_TRANSCEIVER         (0x05u)
#define CANSM_E_INVALID_PARTITION_CONTEXT (0x06u)
#define CANSM_E_NOT_IN_NO_COM             (0x0Bu)
/*Run time error*/
#define CANSM_E_MODE_REQUEST_TIMEOUT 0x0A
/* Service IDs */
#define SERVICE_ID_CANSM_INIT                               0x00
#define SERVICE_ID_CANSM_DEINIT                             0x14
#define SERVICE_ID_CANSM_REQUESTCOMMODE                     0x02
#define SERVICE_ID_CANSM_GETCURRENTCOMMODE                  0x03
#define SERVICE_ID_CANSM_STARTWAKEUPSOURCE                  0x11
#define SERVICE_ID_CANSM_STOPWAKEUPSOURCE                   0x12
#define SERVICE_ID_CANSM_GETVERSIONINFO                     0x01
#define SERVICE_ID_CANSM_SETBAUDRATE                        0x0d
#define SERVICE_ID_CANSM_SETICOMCONFIGURATION               0x0f
#define SERVICE_ID_CANSM_SETECUPASSIVE                      0x13
#define SERVICE_ID_CANSM_CONTROLLERBUSOFF                   0x04
#define SERVICE_ID_CANSM_CONTROLLERMODEINDICATION           0x07
#define SERVICE_ID_CANSM_TRANCEIVERMODEINDICATION           0x09
#define SERVICE_ID_CANSM_TXTIMEOUTEXCEPTION                 0x0b
#define SERVICE_ID_CANSM_CLEARTRCVWUFFLAGINDICATION         0x08
#define SERVICE_ID_CANSM_CHECKTRANSCEIVERWAKEFLAGINDICATION 0x0a
#define SERVICE_ID_CANSM_CONFIRMPNAVAILABILITY              0x06
#define SERVICE_ID_CANSM_CURRENTICOMCONFIGURATION           0x10
#define SERVICE_ID_CANSM_MAINFUNCTION                       0x05

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
/*Defines the values of the internal states of the CanSM module*/
typedef enum
{
    CANSM_UNINITED,
    CANSM_INITED
} CanSM_StateType;

/*Transceiver parameters of each CAN network*/
typedef struct
{
    /*ID of the CAN transceiver assigned to the configured network handle*/
    uint8 CanSMTransceiverId;
    /*the transceiver support PN or not*/
    boolean CanSMCanTrcvPnEnabled;
} CanSM_TrcvRefType;

/*CAN controller ID parameters of each CAN network*/
typedef struct
{
    /*defines the number of controllers which reference the network*/
    uint8 NetWorkRefControllerNumber;
    /*Unique handle to identify one certain CAN controller*/
    P2CONST(uint8, CANSM_VAR, CANSM_CONST) CanSMControllerId;
} CanSM_ControllerRefType;

#if (STD_ON == CANSM_DEM_SUPPORT)
/*Reference to configured DEM event to report dem errors for this CAN network*/
typedef struct
{
    /*Reference to configured DEM event,CANSM_E_BUS_OFF parameter*/
    const Dem_EventIdType* BusOffPara;

    /*Reference to configured DEM event,CANSM_E_MODE_REQUEST_TIMEOUT parameter*/
    Dem_EventIdType* ModeReqTimeoutPara;
} CanSM_DemEventParameterRefType;
#endif
/*This type contains the CAN network specific parameters of each CAN network*/
typedef struct
{
    /*This threshold defines the count of bus-offs until the bus-off recovery
     * switches from level 1 (short recovery time) to level 2 (long recovery time)*/
    uint8 CanSMBorCounterL1ToL2;
    /*This time parameter defines in ms the duration of the bus-off
     * recovery time in level 1 (short recovery time)*/
    uint16 CanSMBorTimeL1;
    /*This time parameter defines in ms the duration of the bus-off
     * recovery time in level 2 (long recovery time)*/
    uint16 CanSMBorTimeL2;
#if CANSM_BOR_TIME_TX_ENSURED_ENABLED == STD_ON
    /*This parameter defines in ms the duration of the bus-off event check*/
    uint16 CanSMBorTimeTxEnsured;
#if CANSM_BOR_TX_CONFIRMATION_POLLING_ENABLED == STD_ON
    /*polls the CanIf_GetTxConfirmationState API to decide the bus-off state to be recovered */
    boolean CanSMBorTxConfirmationPolling;
#endif
#endif
    /*This parameter defines if the <User_GetBusOffDelay> shall be called for this network*/
    boolean CanSMEnableBusOffDelay;
    /*Unique handle to identify one certain CAN network*/
    NetworkHandleType CanSMComMNetworkHandleRef;
    /*Reference to one of the transceivers managed by the CanIf module*/
    P2CONST(CanSM_TrcvRefType, CANSM_CONST, CANSM_CONST_PBCFG) TrcvRef;
    /*Reference to one of the CAN controllers managed by the CanIf module*/
    P2CONST(CanSM_ControllerRefType, CANSM_CONST, CANSM_CONST_PBCFG) ControllerRef;
#if (STD_ON == CANSM_DEM_SUPPORT)
    /*references to DemEventParameter elements*/
    P2CONST(CanSM_DemEventParameterRefType, CANSM_CONST, CANSM_CONST_PBCFG) CanSMDemEventParameterRefs;
#endif
} CanSM_ManagerNetworkType;

/*This type defines a data structure for the post build parameters of the CanSM.*/
typedef struct
{
    /*Specifies the maximal amount of mode request repetitions(0-255)*/
    uint8 CanSMModeRequestRepetitionMax;
    /*Specifies in which time duration the CanSM module shall repeat mode change requests:ms*/
    uint16 CanSMModeRequestRepetitionTime;
    /*contains the CAN network specific parameters of each CAN network*/
    P2CONST(CanSM_ManagerNetworkType, CANSM_CONST, CANSM_CONST_PBCFG) CanSMManagerNetworkRef;
} CanSM_ConfigType;

typedef enum
{
    CANSM_BSM_S_NOT_INITIALIZED,
    CANSM_BSM_DEINITPNNOTSUPPORTED,
#if (STD_ON == CANSM_PNC_SUPPORT)
    CANSM_BSM_DEINITPNSUPPORTED,
#endif
    CANSM_BSM_S_NOCOM,
    CANSM_BSM_WUVALIDATION,
    CANSM_BSM_S_PRE_FULLCOM,
    CANSM_BSM_S_FULLCOM,
#if (STD_ON == CANSM_SET_BAUDRATE_API)
    CANSM_BSM_S_CHANGE_BAUDRATE,
#endif
    CANSM_BSM_S_SILENTCOM,
    CANSM_BSM_S_SILENTCOM_BOR
} CanSM_BSM_StateType;

#if (STD_ON == CANSM_PNC_SUPPORT)
typedef enum
{
    DEINITPN_S_PN_CLEAR_WUF,
    DEINITPN_S_PN_CLEAR_WUF_WAIT,
    DEINITPN_S_PN_CC_STOPPED,
    DEINITPN_S_CC_STOPPED_WAIT,
    DEINITPN_S_TRCV_NORMAL,
    DEINITPN_S_TRCV_NORMAL_WAIT,
    DEINITPN_S_TRCV_STANDBY,
    DEINITPN_S_TRCV_STANDBY_WAIT,
    DEINITPN_S_CC_SLEEP,
    DEINITPN_S_CC_SLEEP_WAIT,
    DEINITPN_S_CHECK_WFLAG_IN_CC_SLEEP,
    DEINITPN_S_CHECK_WUF_IN_CC_SLEEP_WAIT,
    DEINITPN_S_CHECK_WFLAG_IN_NOT_CC_SLEEP,
    DEINITPN_S_CHECK_WUF_IN_NOT_CC_SLEEP_WAIT
} CanSM_DeinitPnSupportedStateType;
#endif

typedef enum
{
    DEINITPNNOT_S_CC_STOPPED,
    DEINITPNNOT_S_CC_STOPPED_WAIT,
    DEINITPNNOT_S_CC_SLEEP,
    DEINITPNNOT_S_CC_SLEEP_WAIT,
    DEINITPNNOT_S_TRCV_NORMAL,
    DEINITPNNOT_S_TRCV_NORMAL_WAIT,
    DEINITPNNOT_S_TRCV_STANDBY,
    DEINITPNNOT_S_TRCV_STANDBY_WAIT
} CanSM_DeinitPnNotSupportedStateType;

typedef enum
{
    WUVALIDATION_S_TRCV_NORMAL,
    WUVALIDATION_S_TRCV_NORMAL_WAIT,
    WUVALIDATION_S_CC_STOPPED,
    WUVALIDATION_S_CC_STOPPED_WAIT,
    WUVALIDATION_S_CC_STARTED,
    WUVALIDATION_S_CC_STARTED_WAIT,
    WUVALIDATION_WAIT_WUVALIDATION_LEAVE
} CanSM_WUValidationStateType;

typedef enum
{
    PREFULLCOM_S_TRCV_NORMAL,
    PREFULLCOM_S_TRCV_NORMAL_WAIT,
    PREFULLCOM_S_CC_STOPPED,
    PREFULLCOM_S_CC_STOPPED_WAIT,
    PREFULLCOM_S_CC_STARTED,
    PREFULLCOM_S_CC_STARTED_WAIT
} CanSM_PreFullComStateType;

typedef enum
{
    FULLCOM_S_BUS_OFF_CHECK,
    FULLCOM_S_NO_BUS_OFF,
    FULLCOM_S_CC_STOPPED,
    FULLCOM_S_CC_STOPPED_WAIT,
    FULLCOM_S_CC_STARTED,
    FULLCOM_S_CC_STARTED_WAIT,
    FULLCOM_S_RESTART_CC,
    FULLCOM_S_RESTART_CC_WAIT,
    FULLCOM_S_TX_OFF
} CanSM_FullComStateType;

typedef enum
{
    CHANGEBR_CHANGE_BR_SYNC,
    CHANGEBR_S_CC_STOPPED,
    CHANGEBR_S_CC_STOPPED_WAIT,
    CHANGEBR_S_CC_STARTED,
    CHANGEBR_S_CC_STARTED_WAIT
} CanSM_ChangeBaudrateStateType;

typedef enum
{
    SILENTBOR_S_RESTART_CC,
    SILENTBOR_S_RESTART_CC_WAIT
} CanSM_SilentComBORStateType;

typedef enum
{
    CANSM_NO_REQUEST,
    CANSM_NO_COMMUNICATION,
    CANSM_SILENT_COMMUNICATION,
    CANSM_FULL_COMMUNICATION
} CanSM_RequestModeType;

typedef enum
{
    T_NO_INDICATED,
#if (STD_ON == CANSM_PNC_SUPPORT)
    T_CLEAR_WUF_INDICATED,
    T_CHECK_WFLAG_INDICATED,
#endif
    T_TRCV_NORMAL_INDICATED,
    T_TRCV_STANDBY_INDICATED,
} CanSM_CanIfIndicatedType;

typedef struct
{
    uint32 ModeRequestRepetitionTime;
    /* timer for CanIf API indicated state machine */
    uint32 waitCanIfIndicatedStartTime;
    /* timer for bus-off recovery state machine */
    uint32 busOffEventStartTime;
#if CANSM_BOR_TIME_TX_ENSURED_ENABLED == STD_ON
    /* timer for G_BUS_OFF_PASSIVE state machine */
    uint32 borTimeTxEnsuredTime;
#endif
    /* counter for bus-off recovery state machine */
    uint8 busOffCounter;
    /*counter for repeat CanIf API*/
    uint8 repeatCounter;
    boolean busOffEvent; /**< Set in CanSM_ControllerBusOff and clear before calling function CanIf_SetControllerMode */
    /* CanIf API indication type*/
    CanSM_CanIfIndicatedType canIfIndicated;
    /* request communication mode of net work */
    CanSM_RequestModeType requestComMode;
    /* current communication mode of net work */
    ComM_ModeType curComMode;
    /* NetWork current BSM state */
    CanSM_BSM_StateType curBsmState;
#if (STD_ON == CANSM_SET_BAUDRATE_API)
    /* store the BaudRateConfigID when CanSM_SetBaudrate is called successful*/
    uint16 baudRateConfigID;
    /* sub BSM state(CANSM_BSM_S_CHANGE_BAUDRATE)*/
    CanSM_ChangeBaudrateStateType changeBaudrateState;
#endif
#if (STD_ON == CANSM_GET_BUSOFF_DELAY_FUNCTION_USED)
    /* wait additionally to L1/L2 after a BusOff occurred.*/
    uint16 busOffDelayTime;
#endif
#if (STD_ON == CANSM_PNC_SUPPORT)
    /* sub BSM state(CANSM_BSM_S_PRE_NOCOM) if support PN*/
    CanSM_DeinitPnSupportedStateType deinitPnSupportedState;
#endif
    /* sub BSM state(CANSM_BSM_S_PRE_NOCOM) if not support PN*/
    CanSM_DeinitPnNotSupportedStateType deinitPnNotSupportedState;
    /* sub BSM state(CANSM_BSM_WUVALIDATION)*/
    CanSM_WUValidationStateType wuValidationState;
    /* sub BSM state(CANSM_BSM_S_PRE_FULLCOM)*/
    CanSM_PreFullComStateType preFullComState;
    /* sub BSM state(CANSM_BSM_S_FULLCOM)*/
    CanSM_FullComStateType fullComState;
    /* sub BSM state(CANSM_BSM_S_SILENTCOM_BOR)*/
    CanSM_SilentComBORStateType silentComBORState;
} CanSM_NetWorkRunTimeType;
/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/

extern CONST(CanSM_ConfigType, CANSM_CONST) CanSM_Config;

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

/******************************************************************************/
/*
 * Brief               This service initializes the CanSM module.
 * ServiceId           0x00
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ConfigPtr, Pointer to init structure for the post build
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              NA
 */
/******************************************************************************/
FUNC(void, CANSM_CODE)
CanSM_Init(P2CONST(CanSM_ConfigType, AUTOMATIC, CANSM_CONFIG_DATA) ConfigPtr);

/**
 * @brief         This service de-initializes the CanSM module.
 * @id            0x14
 * @synchronous   Synchronous
 * @reentrancy    Non Reentrant
 */
void CanSM_DeInit(void);

/**
 * @brief         This service shall change the communication mode of a CAN network to the requested one.
 * @id            0x02
 * @synchronous   Asynchronous
 * @reentrancy    Reentrant (only for different network handles)
 * @param[in]     network  Handle of destined communication network for request
 * @param[in]     comMMode Requested communication mode
 * @retval        E_OK     Service accepted
 * @retval        E_NOT_OK Service denied
 */
Std_ReturnType CanSM_RequestComMode(NetworkHandleType network, ComM_ModeType comMMode);

/**
 * @brief         This service shall put out the current communication mode of a CAN network.
 * @id            0x03
 * @synchronous   Synchronous
 * @reentrancy    Reentrant
 * @param[in]     network     Network handle, whose current communication mode shall be put out
 * @param[out]    comMModePtr Pointer, where to put out the current communication mode
 * @retval        E_OK        Service accepted
 * @retval        E_NOT_OK    Service denied
 */
Std_ReturnType CanSM_GetCurrentComMode(NetworkHandleType network, ComM_ModeType* comMModePtr);

#if (STD_ON == CANSM_VERSION_INFO_API)
/******************************************************************************/
/*
 * Brief               This service puts out the version information of this
 *                     module
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      NA
 * Param-Name[out]     VersionInfo, Pointer to where to store the version
 *                     information of this module
 * Param-Name[in/out]  NA
 * Return              NA
 */
/******************************************************************************/
FUNC(void, CANSM_CODE)
CanSM_GetVersionInfo(Std_VersionInfoType* VersionInfo);
#endif /*STD_ON == CANSM_VERSION_INFO_API*/

#if (STD_ON == CANSM_SET_BAUDRATE_API)
/******************************************************************************/
/*
 * Brief               This service shall start an asynchronous process to change
 *                     the baud rate for the configured CAN controllers of a
 *                     certain CAN network.Depending on necessary baud rate
 *                     modifications the controllers might have to reset.
 * ServiceId           0x0d
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different Networks. Non reentrant for the same
 *                     Network
 * Param-Name[in]      Network, Handle of the addressed CAN network for the baud
 *                     rate change
 *                     BaudRateConfigID, a baud rate configuration by ID
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              E_OK: Service request accepted, setting of new baud rate
 *                     started
 *                     E_NOT_OK: Service request not accepted
 */
/******************************************************************************/
FUNC(Std_ReturnType, CANSM_CODE)
CanSM_SetBaudrate(NetworkHandleType Network, uint16 BaudRateConfigID);
#endif /*STD_ON == CANSM_SET_BAUDRATE_API*/

#if (STD_ON == CANSM_TX_OFFLINE_ACTIVE_SUPPORT)
/******************************************************************************/
/*
 * Brief               This function can be used to set all CanSM channels of
 *                     the ECU to a receive only mode. This mode will be kept
 *                     either until it is set back, or the ECU is reset.
 * ServiceId           0x13
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanSM_Passive, TRUE: set all CanSM channels to passive,
 *                     i.e. receive only
 *                     FALSE: set all CanSM channels back to non-passive
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              E_OK: Request accepted
 *                     E_NOT_OK: Request denied
 */
/******************************************************************************/
FUNC(Std_ReturnType, CANSM_CODE)
CanSM_SetEcuPassive(boolean CanSM_Passive);
#endif /*STD_ON == CANSM_TX_OFFLINE_ACTIVE_SUPPORT*/

/******************************************************************************/
/*
 * Brief               This function shall be called by EcuM when a wakeup source
 *                     shall be started.
 * ServiceId           0x11
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      network, Affected CAN network
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              E_OK: Request accepted E_NOT_OK: Request denied
 */
/******************************************************************************/
FUNC(Std_ReturnType, CANSM_ECUM_CODE)
CanSM_StartWakeupSource(NetworkHandleType network);

/******************************************************************************/
/*
 * Brief               This function shall be called by EcuM when a wakeup source
 *                     shall be stopped
 * ServiceId           0x12
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      network, Affected CAN network
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              E_OK: Request accepted E_NOT_OK: Request denied
 */
/******************************************************************************/
FUNC(Std_ReturnType, CANSM_ECUM_CODE)
CanSM_StopWakeupSource(NetworkHandleType network);

#endif /* CANSM_H */
